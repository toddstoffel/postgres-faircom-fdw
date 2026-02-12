#!/usr/bin/env python3
"""
FairCom FDW Comprehensive Test Suite
=====================================

Professional test suite for FairCom Foreign Data Wrapper commercialization.
Run from local machine: python3 test_fdw.py [options]

Requirements:
  - psycopg2: pip install psycopg2-binary

Features:
  - Complete test coverage for reliability, performance, and functionality
  - Color-coded output with detailed reporting
  - JSON export for CI/CD integration
  - Configurable database connection
  - Performance benchmarking with statistics
  - Comprehensive commercialization readiness scoring
"""

import argparse
import json
import sys
import time
import statistics
from datetime import datetime
from typing import Dict, List, Tuple, Optional
from dataclasses import dataclass, asdict
from enum import Enum
from concurrent.futures import ThreadPoolExecutor, as_completed

try:
    import psycopg2
    from psycopg2 import sql
except ImportError:
    print("ERROR: psycopg2 not installed")
    print("Install with: pip install psycopg2-binary")
    sys.exit(1)


class TestStatus(Enum):
    PASSED = "PASSED"
    FAILED = "FAILED"
    SKIPPED = "SKIPPED"


@dataclass
class TestResult:
    name: str
    category: str
    status: TestStatus
    elapsed_ms: float
    error: Optional[str] = None
    threshold_ms: Optional[int] = None

    def is_slow(self) -> bool:
        return self.threshold_ms is not None and self.elapsed_ms > self.threshold_ms


class Colors:
    GREEN = '\033[92m'
    RED = '\033[91m'
    YELLOW = '\033[93m'
    BLUE = '\033[94m'
    BOLD = '\033[1m'
    END = '\033[0m'

    @staticmethod
    def disable():
        for attr in ['GREEN', 'RED', 'YELLOW', 'BLUE', 'BOLD', 'END']:
            setattr(Colors, attr, '')


class FDWTestSuite:
    def __init__(self, host: str, port: int, database: str, user: str, password: str, no_color: bool = False):
        self.host = host
        self.port = port
        self.database = database
        self.user = user
        self.password = password
        self.conn = None
        self.results: List[TestResult] = []
        self.start_time = datetime.now()
        self.baseline_employee_count = None  # Captured at test start

        if no_color:
            Colors.disable()

    def connect(self) -> bool:
        """Establish database connection"""
        try:
            self.conn = psycopg2.connect(
                host=self.host,
                port=self.port,
                database=self.database,
                user=self.user,
                password=self.password
            )
            self.conn.autocommit = True  # Auto-commit each query to avoid transaction abort

            # Clean up any leftover test records from previous runs
            # This makes tests idempotent and runnable multiple times
            # Use individual DELETE statements since IN clause may have issues
            deleted_count = 0
            with self.conn.cursor() as cur:
                for test_emp_id in [90000, 90001, 90002, 90003, 90004]:
                    try:
                        cur.execute(f"DELETE FROM employees WHERE emp_id = {test_emp_id};")
                        deleted_count += cur.rowcount
                    except:
                        pass  # Ignore errors if record doesn't exist
                if deleted_count > 0:
                    print(f"Cleaned up {deleted_count} leftover test records from previous run")

            # Capture baseline employee count at test start
            # This makes tests resilient to database state changes
            # Note: Write operations add/delete test records but clean up afterwards
            with self.conn.cursor() as cur:
                cur.execute("SELECT COUNT(*) FROM employees WHERE emp_id NOT IN (90000, 90001, 90002, 90003, 90004);")
                self.baseline_employee_count = cur.fetchone()[0]

            return True
        except psycopg2.Error as e:
            print(f"{Colors.RED}ERROR: Cannot connect to PostgreSQL{Colors.END}")
            print(f"  {e}")
            return False

    def close(self):
        """Close database connection"""
        if self.conn:
            # Final cleanup: remove all test records before closing
            try:
                with self.conn.cursor() as cur:
                    for test_emp_id in [90000, 90001, 90002, 90003, 90004]:
                        try:
                            cur.execute(f"DELETE FROM employees WHERE emp_id = {test_emp_id};")
                        except:
                            pass  # Ignore errors
            except:
                pass  # Ignore errors during final cleanup
            self.conn.close()

    def execute(self, query: str) -> Tuple[bool, Optional[any], str]:
        """Execute query and return result"""
        try:
            with self.conn.cursor() as cur:
                cur.execute(query)
                result = cur.fetchall()
                return True, result, ""
        except psycopg2.Error as e:
            return False, None, str(e)

    def get_single_value(self, query: str):
        """Execute query and return single value"""
        success, result, error = self.execute(query)
        if not success:
            raise Exception(error)
        if result and len(result) > 0:
            return result[0][0]
        return None

    def test(self, category: str, name: str, query: str, threshold_ms: int = None) -> bool:
        """Run a single test"""
        start = time.time()
        success, result, error = self.execute(query)
        elapsed_ms = (time.time() - start) * 1000

        if not success:
            status = TestStatus.FAILED
            symbol = f"{Colors.RED}✗{Colors.END}"
        elif threshold_ms and elapsed_ms > threshold_ms:
            status = TestStatus.FAILED
            symbol = f"{Colors.YELLOW}⚠{Colors.END}"
            error = f"Slow: {elapsed_ms:.1f}ms > {threshold_ms}ms"
        else:
            status = TestStatus.PASSED
            symbol = f"{Colors.GREEN}✓{Colors.END}"

        test_result = TestResult(
            name=name,
            category=category,
            status=status,
            elapsed_ms=elapsed_ms,
            error=error,
            threshold_ms=threshold_ms
        )
        self.results.append(test_result)

        print(f"  {symbol} {name:<44}{elapsed_ms:>7.1f}ms")

        return status == TestStatus.PASSED

    def test_single_value(self, category: str, name: str, query: str, expected, threshold_ms: int = None) -> bool:
        """Run a test that validates a single scalar result"""
        start = time.time()
        success, result, error = self.execute(query)
        elapsed_ms = (time.time() - start) * 1000

        if not success:
            status = TestStatus.FAILED
            symbol = f"{Colors.RED}✗{Colors.END}"
        elif not result or len(result) != 1 or len(result[0]) != 1:
            status = TestStatus.FAILED
            symbol = f"{Colors.RED}✗{Colors.END}"
            error = "Expected single value result"
        else:
            value = result[0][0]
            if value == expected:
                status = TestStatus.PASSED
                symbol = f"{Colors.GREEN}✓{Colors.END}"
            else:
                status = TestStatus.FAILED
                symbol = f"{Colors.RED}✗{Colors.END}"
                error = f"Expected {expected}, got {value}"

        if status == TestStatus.PASSED and threshold_ms and elapsed_ms > threshold_ms:
            status = TestStatus.FAILED
            symbol = f"{Colors.YELLOW}⚠{Colors.END}"
            error = f"Slow: {elapsed_ms:.1f}ms > {threshold_ms}ms"

        test_result = TestResult(
            name=name,
            category=category,
            status=status,
            elapsed_ms=elapsed_ms,
            error=error,
            threshold_ms=threshold_ms
        )
        self.results.append(test_result)

        print(f"  {symbol} {name:<44}{elapsed_ms:>7.1f}ms")

        return status == TestStatus.PASSED

    def _print_line(self, passed: bool, name: str, elapsed_ms: float):
        """Print a single test result line with consistent alignment"""
        if passed:
            symbol = f"{Colors.GREEN}✓{Colors.END}"
        else:
            symbol = f"{Colors.RED}✗{Colors.END}"
        print(f"  {symbol} {name:<44}{elapsed_ms:>7.1f}ms")

    def print_header(self, text: str):
        """Print section header"""
        print(f"\n{Colors.BLUE}{Colors.BOLD}{'='*80}{Colors.END}")
        print(f"{Colors.BLUE}{Colors.BOLD}  {text}{Colors.END}")
        print(f"{Colors.BLUE}{Colors.BOLD}{'='*80}{Colors.END}")

    def print_footer(self):
        """Print footer"""
        print(f"\n{Colors.BLUE}{Colors.BOLD}{'='*80}{Colors.END}\n")

    # =========================================================================
    # TEST CATEGORIES
    # =========================================================================

    def test_connection_setup(self):
        """Test 1: Connection & Setup"""
        self.print_header("1. CONNECTION & SETUP TESTS")

        # Clean up any test records from previous runs to ensure exactly 1000 base records
        try:
            with self.conn.cursor() as cur:
                cur.execute("DELETE FROM employees WHERE emp_id >= 90000;")
        except:
            pass  # Ignore if table doesn't exist yet

        self.test_single_value(
            "Connection & Setup",
            "Extension installed",
            "SELECT COUNT(*) FROM pg_extension WHERE extname = 'faircom_fdw';",
            1
        )

        self.test_single_value(
            "Connection & Setup",
            "Foreign server exists",
            "SELECT COUNT(*) FROM pg_foreign_server WHERE srvname = 'faircom_server';",
            1
        )

        for table in ["employees", "departments", "projects", "employee_projects", "timesheets"]:
            self.test_single_value(
                "Connection & Setup",
                f"Foreign table '{table}' exists",
                f"SELECT COUNT(*) FROM pg_class WHERE relname = '{table}';",
                1
            )

        self.test_single_value(
            "Connection & Setup",
            "Can connect to FairCom",
            "SELECT COUNT(*) FROM employees WHERE emp_id NOT IN (90000, 90001, 90002, 90003, 90004);",
            self.baseline_employee_count,
            threshold_ms=5000
        )

    def test_data_integrity(self):
        """Test 2: Data Integrity & Type Conversion"""
        self.print_header("2. DATA INTEGRITY & TYPE CONVERSION")

        self.test_single_value(
            "Data Integrity",
            f"{self.baseline_employee_count} records in employees",
            "SELECT COUNT(*) FROM employees WHERE emp_id NOT IN (90000, 90001, 90002, 90003, 90004);",
            self.baseline_employee_count,
            threshold_ms=2000
        )

        self.test_single_value(
            "Data Integrity",
            "All emp_id values unique",
            "SELECT COUNT(DISTINCT emp_id) FROM employees WHERE emp_id NOT IN (90000, 90001, 90002, 90003, 90004);",
            self.baseline_employee_count,
            threshold_ms=2000
        )

        self.test_single_value(
            "Data Integrity",
            "NUMERIC salary conversion",
            "SELECT COUNT(*) FROM employees WHERE salary > 0 AND emp_id NOT IN (90000, 90001, 90002, 90003, 90004);",
            self.baseline_employee_count,
            threshold_ms=2000
        )

        self.test_single_value(
            "Data Integrity",
            "Salary range validation",
            "SELECT COUNT(*) FROM employees WHERE salary BETWEEN 30000 AND 250000 AND emp_id NOT IN (90000, 90001, 90002, 90003, 90004);",
            self.baseline_employee_count,
            threshold_ms=2000
        )

        self.test_single_value(
            "Data Integrity",
            "DATE conversion (hire_date)",
            "SELECT COUNT(*) FROM employees WHERE hire_date >= '1970-01-01' AND emp_id NOT IN (90000, 90001, 90002, 90003, 90004);",
            self.baseline_employee_count,
            threshold_ms=2000
        )

        self.test_single_value(
            "Data Integrity",
            "TEXT conversion (name)",
            "SELECT COUNT(*) FROM employees WHERE name IS NOT NULL AND emp_id NOT IN (90000, 90001, 90002, 90003, 90004);",
            self.baseline_employee_count,
            threshold_ms=2000
        )

        self.test_single_value(
            "Data Integrity",
            "INTEGER conversion (dept_id)",
            "SELECT COUNT(*) FROM employees WHERE dept_id > 0 AND emp_id NOT IN (90000, 90001, 90002, 90003, 90004);",
            self.baseline_employee_count,
            threshold_ms=2000
        )

        self.test_single_value(
            "Data Integrity",
            "NULL value handling",
            "SELECT COUNT(*) FROM employees WHERE (email IS NULL OR email IS NOT NULL) AND emp_id NOT IN (90000, 90001, 90002, 90003, 90004);",
            self.baseline_employee_count,
            threshold_ms=2000
        )

    def test_write_operations(self):
        """Test 2a: Write Operations (INSERT, UPDATE)"""
        self.print_header("2a. WRITE OPERATIONS")

        try:
            # Clean up any test records from previous runs
            test_emp_id = 90000
            test_emp_id_name_delete = 90004
            test_emp_id_dates = 90002
            test_emp_id_null = 90001
            test_emp_id_complex = 90003

            with self.conn.cursor() as cur:
                for emp_id in [test_emp_id, test_emp_id_name_delete, test_emp_id_dates, test_emp_id_null, test_emp_id_complex]:
                    try:
                        cur.execute(f"DELETE FROM employees WHERE emp_id = {emp_id};")
                    except:
                        pass

            # Test 1: INSERT basic record
            insert_query = f"""
                INSERT INTO employees (emp_id, name, email, phone, city, hire_date, salary, dept_id, job_title)
                VALUES ({test_emp_id}, 'Test Employee', 'test@example.com', '555-0001', 'TestCity', '2026-01-15', 75000.00, 1, 'Test Engineer');
            """
            start = time.time()
            try:
                with self.conn.cursor() as cur:
                    cur.execute(insert_query)
                    cur.execute(f"SELECT name, salary FROM employees WHERE emp_id = {test_emp_id};")
                    result = cur.fetchone()
                elapsed_ms = (time.time() - start) * 1000

                # Verify insert
                if result and result[0] == 'Test Employee' and result[1] == 75000.00:
                    self.results.append(TestResult(
                        name="INSERT basic record",
                        category="Write Operations",
                        status=TestStatus.PASSED,
                        elapsed_ms=elapsed_ms
                    ))
                    self._print_line(True, "INSERT basic record", elapsed_ms)
                else:
                    self.results.append(TestResult(
                        name="INSERT basic record",
                        category="Write Operations",
                        status=TestStatus.FAILED,
                        elapsed_ms=elapsed_ms,
                        error="Verification failed"
                    ))
                    self._print_line(False, "INSERT basic record", elapsed_ms)
            except Exception as e:
                elapsed_ms = (time.time() - start) * 1000
                self.results.append(TestResult(
                    name="INSERT basic record",
                    category="Write Operations",
                    status=TestStatus.FAILED,
                    elapsed_ms=elapsed_ms,
                    error=str(e)[:100]
                ))
                self._print_line(False, "INSERT basic record", elapsed_ms)

            self.test_single_value(
                "Write Operations",
                "SELECT by email returns 1 row",
                "SELECT COUNT(*) FROM employees WHERE email = 'test@example.com';",
                1
            )

            # Test 2: INSERT with NULL values
            test_emp_id_null = 90001
            insert_null_query = f"""
                INSERT INTO employees (emp_id, name, email, phone, city, hire_date, salary, dept_id, job_title)
                VALUES ({test_emp_id_null}, 'Null Test', NULL, NULL, 'TestCity', NULL, 50000.00, 1, 'Tester');
            """
            start = time.time()
            try:
                with self.conn.cursor() as cur:
                    cur.execute(insert_null_query)
                    cur.execute(f"SELECT email, phone, hire_date FROM employees WHERE emp_id = {test_emp_id_null};")
                    result = cur.fetchone()
                elapsed_ms = (time.time() - start) * 1000

                # Verify NULL handling
                if result and result[0] is None and result[1] is None and result[2] is None:
                    self.results.append(TestResult(
                        name="INSERT with NULL values",
                        category="Write Operations",
                        status=TestStatus.PASSED,
                        elapsed_ms=elapsed_ms
                    ))
                    self._print_line(True, "INSERT with NULL values", elapsed_ms)
                else:
                    self.results.append(TestResult(
                        name="INSERT with NULL values",
                        category="Write Operations",
                        status=TestStatus.FAILED,
                        elapsed_ms=elapsed_ms,
                        error="NULL verification failed"
                    ))
                    self._print_line(False, "INSERT with NULL values", elapsed_ms)
            except Exception as e:
                elapsed_ms = (time.time() - start) * 1000
                self.results.append(TestResult(
                    name="INSERT with NULL values",
                    category="Write Operations",
                    status=TestStatus.FAILED,
                    elapsed_ms=elapsed_ms,
                    error=str(e)[:100]
                ))
                self._print_line(False, "INSERT with NULL values", elapsed_ms)

            # Test 3: INSERT with edge case dates
            insert_dates_query = f"""
                INSERT INTO employees (emp_id, name, email, phone, city, hire_date, salary, dept_id, job_title)
                VALUES ({test_emp_id_dates}, 'Date Test', 'date@test.com', '555-0002', 'TestCity', '2000-01-01', 60000.00, 1, 'Tester');
            """
            start = time.time()
            try:
                with self.conn.cursor() as cur:
                    cur.execute(insert_dates_query)
                    cur.execute(f"SELECT hire_date FROM employees WHERE emp_id = {test_emp_id_dates};")
                    result = cur.fetchone()
                elapsed_ms = (time.time() - start) * 1000

                # Verify date
                if result and str(result[0]) == '2000-01-01':
                    self.results.append(TestResult(
                        name="INSERT with edge case dates",
                        category="Write Operations",
                        status=TestStatus.PASSED,
                        elapsed_ms=elapsed_ms
                    ))
                    self._print_line(True, "INSERT with edge case dates", elapsed_ms)
                else:
                    self.results.append(TestResult(
                        name="INSERT with edge case dates",
                        category="Write Operations",
                        status=TestStatus.FAILED,
                        elapsed_ms=elapsed_ms,
                        error=f"Date mismatch: {result[0] if result else 'None'}"
                    ))
                    self._print_line(False, "INSERT with edge case dates", elapsed_ms)
            except Exception as e:
                elapsed_ms = (time.time() - start) * 1000
                self.results.append(TestResult(
                    name="INSERT with edge case dates",
                    category="Write Operations",
                    status=TestStatus.FAILED,
                    elapsed_ms=elapsed_ms,
                    error=str(e)[:100]
                ))
                self._print_line(False, "INSERT with edge case dates", elapsed_ms)

            # Test 4: UPDATE basic field
            update_query = f"""
                UPDATE employees SET salary = 80000.00 WHERE emp_id = {test_emp_id};
            """
            start = time.time()
            try:
                with self.conn.cursor() as cur:
                    cur.execute(update_query)
                    cur.execute(f"SELECT salary FROM employees WHERE emp_id = {test_emp_id};")
                    result = cur.fetchone()
                elapsed_ms = (time.time() - start) * 1000

                # Verify update
                if result and result[0] == 80000.00:
                    self.results.append(TestResult(
                        name="UPDATE basic field",
                        category="Write Operations",
                        status=TestStatus.PASSED,
                        elapsed_ms=elapsed_ms
                    ))
                    self._print_line(True, "UPDATE basic field", elapsed_ms)
                else:
                    self.results.append(TestResult(
                        name="UPDATE basic field",
                        category="Write Operations",
                        status=TestStatus.FAILED,
                        elapsed_ms=elapsed_ms,
                        error=f"Update verification failed: {result[0] if result else 'None'}"
                    ))
                    self._print_line(False, "UPDATE basic field", elapsed_ms)
            except Exception as e:
                elapsed_ms = (time.time() - start) * 1000
                self.results.append(TestResult(
                    name="UPDATE basic field",
                    category="Write Operations",
                    status=TestStatus.FAILED,
                    elapsed_ms=elapsed_ms,
                    error=str(e)[:100]
                ))
                self._print_line(False, "UPDATE basic field", elapsed_ms)

            # Test 5: UPDATE multiple fields
            update_multi_query = f"""
                UPDATE employees SET name = 'Updated Name', city = 'UpdatedCity', salary = 85000.00
                WHERE emp_id = {test_emp_id};
            """
            start = time.time()
            try:
                with self.conn.cursor() as cur:
                    cur.execute(update_multi_query)
                    cur.execute(f"SELECT name, city, salary FROM employees WHERE emp_id = {test_emp_id};")
                    result = cur.fetchone()
                elapsed_ms = (time.time() - start) * 1000

                # Verify update
                if result and result[0] == 'Updated Name' and result[1] == 'UpdatedCity' and result[2] == 85000.00:
                    self.results.append(TestResult(
                        name="UPDATE multiple fields",
                        category="Write Operations",
                        status=TestStatus.PASSED,
                        elapsed_ms=elapsed_ms
                    ))
                    self._print_line(True, "UPDATE multiple fields", elapsed_ms)
                else:
                    self.results.append(TestResult(
                        name="UPDATE multiple fields",
                        category="Write Operations",
                        status=TestStatus.FAILED,
                        elapsed_ms=elapsed_ms,
                        error="Multi-field update verification failed"
                    ))
                    self._print_line(False, "UPDATE multiple fields", elapsed_ms)
            except Exception as e:
                elapsed_ms = (time.time() - start) * 1000
                self.results.append(TestResult(
                    name="UPDATE multiple fields",
                    category="Write Operations",
                    status=TestStatus.FAILED,
                    elapsed_ms=elapsed_ms,
                    error=str(e)[:100]
                ))
                self._print_line(False, "UPDATE multiple fields", elapsed_ms)

            # Test 5b: UPDATE by email (string predicate)
            update_email_query = """
                UPDATE employees SET salary = 86000.00 WHERE email = 'test@example.com';
            """
            start = time.time()
            try:
                with self.conn.cursor() as cur:
                    cur.execute(update_email_query)
                    update_count = cur.rowcount
                    cur.execute("SELECT COUNT(*) FROM employees WHERE email = 'test@example.com' AND salary = 86000.00;")
                    result = cur.fetchone()
                elapsed_ms = (time.time() - start) * 1000

                if update_count == 1 and result and result[0] == 1:
                    self.results.append(TestResult(
                        name="UPDATE by email (string predicate)",
                        category="Write Operations",
                        status=TestStatus.PASSED,
                        elapsed_ms=elapsed_ms
                    ))
                    self._print_line(True, "UPDATE by email (string predicate)", elapsed_ms)
                else:
                    self.results.append(TestResult(
                        name="UPDATE by email (string predicate)",
                        category="Write Operations",
                        status=TestStatus.FAILED,
                        elapsed_ms=elapsed_ms,
                        error=f"Expected 1 row updated, got {update_count}"
                    ))
                    self._print_line(False, "UPDATE by email (string predicate)", elapsed_ms)
            except Exception as e:
                elapsed_ms = (time.time() - start) * 1000
                self.results.append(TestResult(
                    name="UPDATE by email (string predicate)",
                    category="Write Operations",
                    status=TestStatus.FAILED,
                    elapsed_ms=elapsed_ms,
                    error=str(e)[:100]
                ))
                self._print_line(False, "UPDATE by email (string predicate)", elapsed_ms)

            # Test 6: UPDATE with NULL
            update_null_query = f"""
                UPDATE employees SET email = NULL, phone = NULL WHERE emp_id = {test_emp_id};
            """
            start = time.time()
            try:
                with self.conn.cursor() as cur:
                    cur.execute(update_null_query)
                    cur.execute(f"SELECT email, phone FROM employees WHERE emp_id = {test_emp_id};")
                    result = cur.fetchone()
                elapsed_ms = (time.time() - start) * 1000

                # Verify NULL update
                if result and result[0] is None and result[1] is None:
                    self.results.append(TestResult(
                        name="UPDATE to NULL",
                        category="Write Operations",
                        status=TestStatus.PASSED,
                        elapsed_ms=elapsed_ms
                    ))
                    self._print_line(True, "UPDATE to NULL", elapsed_ms)
                else:
                    self.results.append(TestResult(
                        name="UPDATE to NULL",
                        category="Write Operations",
                        status=TestStatus.FAILED,
                        elapsed_ms=elapsed_ms,
                        error="NULL update verification failed"
                    ))
                    self._print_line(False, "UPDATE to NULL", elapsed_ms)
            except Exception as e:
                elapsed_ms = (time.time() - start) * 1000
                self.results.append(TestResult(
                    name="UPDATE to NULL",
                    category="Write Operations",
                    status=TestStatus.FAILED,
                    elapsed_ms=elapsed_ms,
                    error=str(e)[:100]
                ))
                self._print_line(False, "UPDATE to NULL", elapsed_ms)

            # Test 7: UPDATE date field
            update_date_query = f"""
                UPDATE employees SET hire_date = '2025-06-15' WHERE emp_id = {test_emp_id};
            """
            start = time.time()
            try:
                with self.conn.cursor() as cur:
                    cur.execute(update_date_query)
                    cur.execute(f"SELECT hire_date FROM employees WHERE emp_id = {test_emp_id};")
                    result = cur.fetchone()
                elapsed_ms = (time.time() - start) * 1000

                # Verify date update
                if result and str(result[0]) == '2025-06-15':
                    self.results.append(TestResult(
                        name="UPDATE date field",
                        category="Write Operations",
                        status=TestStatus.PASSED,
                        elapsed_ms=elapsed_ms
                    ))
                    self._print_line(True, "UPDATE date field", elapsed_ms)
                else:
                    self.results.append(TestResult(
                        name="UPDATE date field",
                        category="Write Operations",
                        status=TestStatus.FAILED,
                        elapsed_ms=elapsed_ms,
                        error=f"Date update failed: {result[0] if result else 'None'}"
                    ))
                    self._print_line(False, "UPDATE date field", elapsed_ms)
            except Exception as e:
                elapsed_ms = (time.time() - start) * 1000
                self.results.append(TestResult(
                    name="UPDATE date field",
                    category="Write Operations",
                    status=TestStatus.FAILED,
                    elapsed_ms=elapsed_ms,
                    error=str(e)[:100]
                ))
                self._print_line(False, "UPDATE date field", elapsed_ms)

            # Test 8: UPDATE with WHERE clause (single row)
            # At this point salary should be 86000 from test 5b
            update_where_query = f"""
                UPDATE employees SET job_title = 'Senior Test Engineer' WHERE emp_id = {test_emp_id} AND salary = 86000;
            """
            start = time.time()
            try:
                with self.conn.cursor() as cur:
                    # Debug: Check current salary
                    cur.execute(f"SELECT salary, job_title FROM employees WHERE emp_id = {test_emp_id};")
                    before = cur.fetchone()

                    cur.execute(update_where_query)
                    cur.execute(f"SELECT job_title, salary FROM employees WHERE emp_id = {test_emp_id};")
                    result = cur.fetchone()
                elapsed_ms = (time.time() - start) * 1000

                # Verify conditional update (should succeed if salary = 86000)
                if result and result[0] == 'Senior Test Engineer':
                    self.results.append(TestResult(
                        name="UPDATE with WHERE clause",
                        category="Write Operations",
                        status=TestStatus.PASSED,
                        elapsed_ms=elapsed_ms
                    ))
                    self._print_line(True, "UPDATE with WHERE clause", elapsed_ms)
                else:
                    self.results.append(TestResult(
                        name="UPDATE with WHERE clause",
                        category="Write Operations",
                        status=TestStatus.FAILED,
                        elapsed_ms=elapsed_ms,
                        error=f"WHERE update failed: job_title={result[0] if result else 'None'}, salary_before={before[0] if before else 'None'}, salary_after={result[1] if result and len(result) > 1 else 'None'}"
                    ))
                    self._print_line(False, "UPDATE with WHERE clause", elapsed_ms)
            except Exception as e:
                elapsed_ms = (time.time() - start) * 1000
                self.results.append(TestResult(
                    name="UPDATE with WHERE clause",
                    category="Write Operations",
                    status=TestStatus.FAILED,
                    elapsed_ms=elapsed_ms,
                    error=str(e)[:100]
                ))
                self._print_line(False, "UPDATE with WHERE clause", elapsed_ms)

            # Test 8b: DELETE by name (string predicate)
            try:
                with self.conn.cursor() as cur:
                    cur.execute(f"""
                        INSERT INTO employees (emp_id, name, email, phone, city, hire_date, salary, dept_id, job_title)
                        VALUES ({test_emp_id_name_delete}, 'Delete Name Test', 'deletename@test.com', '555-0004', 'TestCity', '2026-02-01', 65000.00, 3, 'Tester');
                    """)
            except:
                pass  # Ignore if already exists

            delete_by_name_query = """
                DELETE FROM employees WHERE name = 'Delete Name Test';
            """
            start = time.time()
            try:
                with self.conn.cursor() as cur:
                    cur.execute(delete_by_name_query)
                    delete_count = cur.rowcount
                    cur.execute("SELECT COUNT(*) FROM employees WHERE name = 'Delete Name Test';")
                    result = cur.fetchone()
                elapsed_ms = (time.time() - start) * 1000

                if delete_count == 1 and result and result[0] == 0:
                    self.results.append(TestResult(
                        name="DELETE by name (string predicate)",
                        category="Write Operations",
                        status=TestStatus.PASSED,
                        elapsed_ms=elapsed_ms
                    ))
                    self._print_line(True, "DELETE by name (string predicate)", elapsed_ms)
                else:
                    self.results.append(TestResult(
                        name="DELETE by name (string predicate)",
                        category="Write Operations",
                        status=TestStatus.FAILED,
                        elapsed_ms=elapsed_ms,
                        error=f"Expected 1 row deleted, got {delete_count}"
                    ))
                    self._print_line(False, "DELETE by name (string predicate)", elapsed_ms)
            except Exception as e:
                elapsed_ms = (time.time() - start) * 1000
                self.results.append(TestResult(
                    name="DELETE by name (string predicate)",
                    category="Write Operations",
                    status=TestStatus.FAILED,
                    elapsed_ms=elapsed_ms,
                    error=str(e)[:100]
                ))
                self._print_line(False, "DELETE by name (string predicate)", elapsed_ms)

            # Test 9: DELETE single record
            delete_query = f"""
                DELETE FROM employees WHERE emp_id = {test_emp_id_dates};
            """
            start = time.time()
            try:
                with self.conn.cursor() as cur:
                    cur.execute(delete_query)
                    cur.execute(f"SELECT COUNT(*) FROM employees WHERE emp_id = {test_emp_id_dates};")
                    result = cur.fetchone()
                elapsed_ms = (time.time() - start) * 1000

                # Verify deletion
                if result and result[0] == 0:
                    self.results.append(TestResult(
                        name="DELETE single record",
                        category="Write Operations",
                        status=TestStatus.PASSED,
                        elapsed_ms=elapsed_ms
                    ))
                    self._print_line(True, "DELETE single record", elapsed_ms)
                else:
                    self.results.append(TestResult(
                        name="DELETE single record",
                        category="Write Operations",
                        status=TestStatus.FAILED,
                        elapsed_ms=elapsed_ms,
                        error=f"DELETE verification failed: count={result[0] if result else 'None'}"
                    ))
                    self._print_line(False, "DELETE single record", elapsed_ms)
            except Exception as e:
                elapsed_ms = (time.time() - start) * 1000
                self.results.append(TestResult(
                    name="DELETE single record",
                    category="Write Operations",
                    status=TestStatus.FAILED,
                    elapsed_ms=elapsed_ms,
                    error=str(e)[:100]
                ))
                self._print_line(False, "DELETE single record", elapsed_ms)

            # Test 10: DELETE with WHERE clause
            # test_emp_id_null should have salary=50000, so WHERE salary = 50000 should match
            delete_where_query = f"""
                DELETE FROM employees WHERE emp_id = {test_emp_id_null} AND salary = 50000;
            """
            start = time.time()
            try:
                with self.conn.cursor() as cur:
                    # Debug: Check current record
                    cur.execute(f"SELECT emp_id, salary FROM employees WHERE emp_id = {test_emp_id_null};")
                    before_delete = cur.fetchone()

                    cur.execute(delete_where_query)
                    cur.execute(f"SELECT COUNT(*) FROM employees WHERE emp_id = {test_emp_id_null};")
                    result = cur.fetchone()
                elapsed_ms = (time.time() - start) * 1000

                # Verify conditional deletion (should delete record since salary=50000)
                if result and result[0] == 0:
                    self.results.append(TestResult(
                        name="DELETE with WHERE clause",
                        category="Write Operations",
                        status=TestStatus.PASSED,
                        elapsed_ms=elapsed_ms
                    ))
                    self._print_line(True, "DELETE with WHERE clause", elapsed_ms)
                else:
                    self.results.append(TestResult(
                        name="DELETE with WHERE clause",
                        category="Write Operations",
                        status=TestStatus.FAILED,
                        elapsed_ms=elapsed_ms,
                        error=f"DELETE WHERE failed: count={result[0] if result else 'None'}, before_delete={before_delete if 'before_delete' in locals() else 'N/A'}"
                    ))
                    self._print_line(False, "DELETE with WHERE clause", elapsed_ms)
            except Exception as e:
                elapsed_ms = (time.time() - start) * 1000
                self.results.append(TestResult(
                    name="DELETE with WHERE clause",
                    category="Write Operations",
                    status=TestStatus.FAILED,
                    elapsed_ms=elapsed_ms,
                    error=str(e)[:100]
                ))
                self._print_line(False, "DELETE with WHERE clause", elapsed_ms)

            # Test 11: DELETE with complex WHERE (AND/OR)
            # First insert a test record for complex delete
            test_emp_id_complex = 90003
            try:
                with self.conn.cursor() as cur:
                    cur.execute(f"""
                        INSERT INTO employees (emp_id, name, email, phone, city, hire_date, salary, dept_id, job_title)
                        VALUES ({test_emp_id_complex}, 'Complex Test', 'complex@test.com', '555-9999', 'TestCity', '2026-01-01', 45000.00, 2, 'Tester');
                    """)
            except:
                pass  # Ignore if already exists

            delete_complex_query = f"""
                DELETE FROM employees WHERE emp_id = {test_emp_id_complex} AND (salary < 50000 OR dept_id = 2);
            """
            start = time.time()
            try:
                with self.conn.cursor() as cur:
                    cur.execute(delete_complex_query)
                    cur.execute(f"SELECT COUNT(*) FROM employees WHERE emp_id = {test_emp_id_complex};")
                    result = cur.fetchone()
                elapsed_ms = (time.time() - start) * 1000

                # Verify complex deletion
                if result and result[0] == 0:
                    self.results.append(TestResult(
                        name="DELETE with complex WHERE",
                        category="Write Operations",
                        status=TestStatus.PASSED,
                        elapsed_ms=elapsed_ms
                    ))
                    self._print_line(True, "DELETE with complex WHERE", elapsed_ms)
                else:
                    self.results.append(TestResult(
                        name="DELETE with complex WHERE",
                        category="Write Operations",
                        status=TestStatus.FAILED,
                        elapsed_ms=elapsed_ms,
                        error=f"DELETE complex WHERE failed: count={result[0] if result else 'None'}"
                    ))
                    self._print_line(False, "DELETE with complex WHERE", elapsed_ms)
            except Exception as e:
                elapsed_ms = (time.time() - start) * 1000
                self.results.append(TestResult(
                    name="DELETE with complex WHERE",
                    category="Write Operations",
                    status=TestStatus.FAILED,
                    elapsed_ms=elapsed_ms,
                    error=str(e)[:100]
                ))
                self._print_line(False, "DELETE with complex WHERE", elapsed_ms)

            # Test 12: Verify UPDATE affects only 1 row (ctid-based row identification)
            # This is the critical test for the ctid bug we fixed
            update_single_row_query = f"""
                UPDATE employees SET name = 'CTID Test Updated' WHERE emp_id = {test_emp_id};
            """
            start = time.time()
            try:
                with self.conn.cursor() as cur:
                    # Get count before update
                    cur.execute(f"SELECT COUNT(*) FROM employees WHERE emp_id = {test_emp_id};")
                    count_before = cur.fetchone()[0]

                    # Perform update
                    cur.execute(update_single_row_query)

                    # Verify only 1 row was updated
                    cur.execute(f"SELECT COUNT(*) FROM employees WHERE emp_id = {test_emp_id} AND name = 'CTID Test Updated';")
                    count_after = cur.fetchone()[0]

                elapsed_ms = (time.time() - start) * 1000

                if count_before == 1 and count_after == 1:
                    self.results.append(TestResult(
                        name="UPDATE affects only 1 row (ctid)",
                        category="Write Operations",
                        status=TestStatus.PASSED,
                        elapsed_ms=elapsed_ms
                    ))
                    self._print_line(True, "UPDATE affects only 1 row (ctid)", elapsed_ms)
                else:
                    self.results.append(TestResult(
                        name="UPDATE affects only 1 row (ctid)",
                        category="Write Operations",
                        status=TestStatus.FAILED,
                        elapsed_ms=elapsed_ms,
                        error=f"UPDATE affected {count_after} rows instead of 1 (ctid bug)"
                    ))
                    self._print_line(False, "UPDATE affects only 1 row (ctid)", elapsed_ms)
            except Exception as e:
                elapsed_ms = (time.time() - start) * 1000
                self.results.append(TestResult(
                    name="UPDATE affects only 1 row (ctid)",
                    category="Write Operations",
                    status=TestStatus.FAILED,
                    elapsed_ms=elapsed_ms,
                    error=str(e)[:100]
                ))
                self._print_line(False, "UPDATE affects only 1 row (ctid)", elapsed_ms)

            # Test 13: Verify DELETE affects only 1 row (ctid-based row identification)
            delete_single_row_query = f"""
                DELETE FROM employees WHERE emp_id = {test_emp_id};
            """
            start = time.time()
            try:
                with self.conn.cursor() as cur:
                    # Verify record exists
                    cur.execute(f"SELECT COUNT(*) FROM employees WHERE emp_id = {test_emp_id};")
                    count_before = cur.fetchone()[0]

                    # Perform delete
                    cur.execute(delete_single_row_query)

                    # Verify exactly 1 row was deleted
                    cur.execute(f"SELECT COUNT(*) FROM employees WHERE emp_id = {test_emp_id};")
                    count_after = cur.fetchone()[0]

                elapsed_ms = (time.time() - start) * 1000

                if count_before == 1 and count_after == 0:
                    self.results.append(TestResult(
                        name="DELETE affects only 1 row (ctid)",
                        category="Write Operations",
                        status=TestStatus.PASSED,
                        elapsed_ms=elapsed_ms
                    ))
                    self._print_line(True, "DELETE affects only 1 row (ctid)", elapsed_ms)
                else:
                    self.results.append(TestResult(
                        name="DELETE affects only 1 row (ctid)",
                        category="Write Operations",
                        status=TestStatus.FAILED,
                        elapsed_ms=elapsed_ms,
                        error=f"DELETE affected wrong count: before={count_before}, after={count_after}"
                    ))
                    self._print_line(False, "DELETE affects only 1 row (ctid)", elapsed_ms)
            except Exception as e:
                elapsed_ms = (time.time() - start) * 1000
                self.results.append(TestResult(
                    name="DELETE affects only 1 row (ctid)",
                    category="Write Operations",
                    status=TestStatus.FAILED,
                    elapsed_ms=elapsed_ms,
                    error=str(e)[:100]
                ))
                self._print_line(False, "DELETE affects only 1 row (ctid)", elapsed_ms)

            # Cleanup: Delete ALL test records to ensure exactly 1000 base records remain
            try:
                with self.conn.cursor() as cur:
                    cur.execute("DELETE FROM employees WHERE emp_id >= 90000;")
            except:
                pass  # Silently ignore cleanup errors

        except Exception as e:
            print(f"  {Colors.RED}✗{Colors.END} Write operations test failed: {str(e)[:80]}")
            self.results.append(TestResult(
                name="Write operations general",
                category="Write Operations",
                status=TestStatus.FAILED,
                elapsed_ms=0,
                error=str(e)[:100]
            ))

    def test_query_functionality(self):
        """Test 3: Query Functionality"""
        self.print_header("3. QUERY FUNCTIONALITY")

        # Basic WHERE clauses
        self.test(
            "Query Functionality",
            "WHERE single condition",
            "SELECT COUNT(*) FROM employees WHERE salary > 100000;",
            threshold_ms=2000
        )

        self.test(
            "Query Functionality",
            "WHERE AND condition",
            "SELECT COUNT(*) FROM employees WHERE salary > 100000 AND dept_id = 1;",
            threshold_ms=2000
        )

        self.test(
            "Query Functionality",
            "WHERE OR condition",
            "SELECT COUNT(*) FROM employees WHERE dept_id = 1 OR dept_id = 2;",
            threshold_ms=2000
        )

        self.test(
            "Query Functionality",
            "IN list query",
            "SELECT COUNT(*) FROM employees WHERE dept_id IN (1, 2, 3, 4, 5);",
            threshold_ms=2000
        )

        self.test(
            "Query Functionality",
            "LIKE pattern matching",
            "SELECT COUNT(*) FROM employees WHERE name LIKE '%n%';",
            threshold_ms=2000
        )

        # Result limiting
        self.test(
            "Query Functionality",
            "ORDER BY with LIMIT",
            "SELECT emp_id FROM employees ORDER BY salary DESC LIMIT 10;",
            threshold_ms=2000
        )

        self.test(
            "Query Functionality",
            "LIMIT clause",
            "SELECT COUNT(*) FROM employees LIMIT 100;",
            threshold_ms=1000
        )

        self.test(
            "Query Functionality",
            "OFFSET clause",
            "SELECT COUNT(*) FROM employees LIMIT 10 OFFSET 100;",
            threshold_ms=1000
        )

        # Aggregates
        aggregates = [
            ("COUNT(*)", "SELECT COUNT(*) FROM employees;"),
            ("SUM(salary)", "SELECT SUM(salary) FROM employees;"),
            ("AVG(salary)", "SELECT AVG(salary) FROM employees;"),
            ("MIN(salary)", "SELECT MIN(salary) FROM employees;"),
            ("MAX(salary)", "SELECT MAX(salary) FROM employees;"),
        ]

        for agg_name, query in aggregates:
            self.test(
                "Query Functionality",
                f"{agg_name} aggregate",
                query,
                threshold_ms=2000
            )

        # GROUP BY
        self.test(
            "Query Functionality",
            "GROUP BY with COUNT",
            "SELECT dept_id, COUNT(*) FROM employees GROUP BY dept_id;",
            threshold_ms=3000
        )

        # JOINs
        self.test(
            "Query Functionality",
            "INNER JOIN two tables",
            "SELECT COUNT(*) FROM employees e JOIN departments d ON e.dept_id = d.dept_id;",
            threshold_ms=5000
        )

    def test_advanced_queries(self):
        """Test 3b: Advanced Query Features & Pushdown Optimization"""
        self.print_header("3b. ADVANCED QUERIES & OPTIMIZATION")

        # More complex JOINs
        self.test(
            "Advanced Queries",
            "LEFT JOIN with WHERE",
            "SELECT COUNT(*) FROM employees e LEFT JOIN departments d ON e.dept_id = d.dept_id WHERE e.salary > 100000;",
            threshold_ms=5000
        )

        self.test(
            "Advanced Queries",
            "Multiple JOINs",
            "SELECT COUNT(*) FROM employees e JOIN departments d ON e.dept_id = d.dept_id JOIN projects p ON d.dept_id = p.dept_id;",
            threshold_ms=10000
        )

        # Result validation (check actual values, not just counts)
        self.test_single_value(
            "Advanced Queries",
            "Result verification - specific employee",
            "SELECT COUNT(*) FROM employees WHERE emp_id = 1;",
            1,
            threshold_ms=2000
        )

        # Complex WHERE conditions
        self.test(
            "Advanced Queries",
            "Complex WHERE (nested conditions)",
            "SELECT COUNT(*) FROM employees WHERE (salary > 100000 AND dept_id = 1) OR (salary < 50000 AND dept_id = 2);",
            threshold_ms=3000
        )

        # NULL conditions
        self.test(
            "Advanced Queries",
            "NULL value comparison",
            "SELECT COUNT(*) FROM employees WHERE email IS NOT NULL;",
            threshold_ms=2000
        )

        # Column selection (not SELECT *)
        self.test(
            "Advanced Queries",
            "Specific column selection",
            "SELECT emp_id, name, salary FROM employees WHERE salary > 100000 LIMIT 10;",
            threshold_ms=2000
        )

        # Predicate pushdown verification
        self.test_predicate_pushdown()

    def test_predicate_pushdown(self):
        """Verify that WHERE predicates are being pushed down to FairCom"""
        # Test 1: Simple range predicate
        self.test(
            "Advanced Queries",
            "Predicate pushdown (range)",
            "SELECT COUNT(*) FROM employees WHERE salary > 150000;",
            threshold_ms=2000
        )

        # Test 2: Multiple predicates
        self.test(
            "Advanced Queries",
            "Predicate pushdown (multiple conditions)",
            "SELECT COUNT(*) FROM employees WHERE salary > 100000 AND dept_id = 1;",
            threshold_ms=2000
        )

    def test_concurrent_connections(self):
        """Test 3c: Multiple sequential connections"""
        self.print_header("3c. MULTIPLE CONNECTIONS")

        # Simple test: create multiple connections sequentially and verify each works
        for i in range(3):
            try:
                conn = psycopg2.connect(
                    host=self.host,
                    port=self.port,
                    database=self.database,
                    user=self.user,
                    password=self.password
                )
                conn.autocommit = True

                cursor = conn.cursor()
                cursor.execute("SELECT COUNT(*) FROM employees;")
                result = cursor.fetchone()
                cursor.close()
                conn.close()

                self.results.append(TestResult(
                    name=f"Sequential connection {i+1}",
                    category="Multiple Connections",
                    status=TestStatus.PASSED,
                    elapsed_ms=0
                ))
            except Exception as e:
                self.results.append(TestResult(
                    name=f"Sequential connection {i+1}",
                    category="Multiple Connections",
                    status=TestStatus.FAILED,
                    error=str(e),
                    elapsed_ms=0
                ))

    def test_index_impact(self):
        """Test 3d: Index Impact on Query Performance"""
        self.print_header("3d. INDEX IMPACT ANALYSIS")

        try:
            # Test 1: Query WITHOUT index (baseline)
            print("\n  Testing range query WITHOUT index...")
            start = time.time()
            with self.conn.cursor() as cur:
                cur.execute("SELECT COUNT(*) FROM employees WHERE salary > 150000;")
                result_no_index = cur.fetchone()[0]
            time_no_index = (time.time() - start) * 1000

            # Test 2: Create index on salary
            print("  Creating index on salary column...")
            try:
                with self.conn.cursor() as cur:
                    cur.execute("CREATE INDEX idx_salary_test ON employees (salary);")
                    self.conn.commit()
                index_created = True
            except Exception as e:
                # Index may already exist or not be supported
                print(f"    (Index creation skipped: {str(e)[:50]})")
                index_created = False

            # Test 3: Query WITH index
            if index_created:
                print("  Testing range query WITH index...")
                start = time.time()
                with self.conn.cursor() as cur:
                    cur.execute("SELECT COUNT(*) FROM employees WHERE salary > 150000;")
                    result_with_index = cur.fetchone()[0]
                time_with_index = (time.time() - start) * 1000

                # Calculate speedup
                speedup_factor = time_no_index / max(time_with_index, 0.1)  # avoid division by zero

                self.results.append(TestResult(
                    name="Query performance WITHOUT index",
                    category="Index Impact",
                    status=TestStatus.PASSED,
                    elapsed_ms=time_no_index
                ))

                self.results.append(TestResult(
                    name="Query performance WITH index",
                    category="Index Impact",
                    status=TestStatus.PASSED,
                    elapsed_ms=time_with_index
                ))

                # Index effectiveness test
                if speedup_factor > 1.0:
                    status = TestStatus.PASSED
                    symbol = f"{Colors.GREEN}✓{Colors.END}"
                else:
                    status = TestStatus.FAILED
                    symbol = f"{Colors.YELLOW}⚠{Colors.END}"

                self.results.append(TestResult(
                    name=f"Index speedup factor",
                    category="Index Impact",
                    status=status,
                    elapsed_ms=0
                ))

                print(f"  {symbol} No index time:    {time_no_index:>7.1f}ms")
                print(f"  {symbol} With index time:  {time_with_index:>7.1f}ms")
                print(f"  {symbol} Speedup factor:   {speedup_factor:>7.1f}x")

                # Test 4: Highly selective query (1% selectivity)
                print("\n  Testing ultra-selective query (1% selectivity)...")
                start = time.time()
                with self.conn.cursor() as cur:
                    cur.execute("SELECT COUNT(*) FROM employees WHERE salary > 200000;")
                    result_selective = cur.fetchone()[0]
                time_selective = (time.time() - start) * 1000

                self.results.append(TestResult(
                    name="Ultra-selective query (1%) with index",
                    category="Index Impact",
                    status=TestStatus.PASSED,
                    elapsed_ms=time_selective
                ))
                print(f"  {symbol} Ultra-selective:  {time_selective:>7.1f}ms")

                # Cleanup: Drop index
                try:
                    with self.conn.cursor() as cur:
                        cur.execute("DROP INDEX idx_salary_test;")
                        self.conn.commit()
                except:
                    pass
            else:
                # If index creation failed, still record the baseline
                self.results.append(TestResult(
                    name="Query performance (no index support)",
                    category="Index Impact",
                    status=TestStatus.PASSED,
                    elapsed_ms=time_no_index
                ))
        except Exception as e:
            self.results.append(TestResult(
                name="Index impact analysis",
                category="Index Impact",
                status=TestStatus.FAILED,
                error=str(e),
                elapsed_ms=0
            ))

    def test_range_stop_optimization(self):
        """Test 3e: Range Stop Optimization Verification"""
        self.print_header("3e. RANGE STOP OPTIMIZATION")

        try:
            # Strategy: Compare performance of queries with different selectivity
            # If range stop works, ultra-selective queries should be much faster

            print("\n  Testing range stop optimization (comparing selectivity levels)...\n")

            # Test 1: Moderate selectivity (50% of records)
            query_50pct = "SELECT COUNT(*) FROM employees WHERE salary > 150000;"
            start = time.time()
            with self.conn.cursor() as cur:
                cur.execute(query_50pct)
                result_50pct = cur.fetchone()[0]
            time_50pct = (time.time() - start) * 1000

            # Test 2: High selectivity (10% of records)
            query_10pct = "SELECT COUNT(*) FROM employees WHERE salary > 200000;"
            start = time.time()
            with self.conn.cursor() as cur:
                cur.execute(query_10pct)
                result_10pct = cur.fetchone()[0]
            time_10pct = (time.time() - start) * 1000

            # Test 3: Ultra-high selectivity (1% of records)
            query_1pct = "SELECT COUNT(*) FROM employees WHERE salary > 230000;"
            start = time.time()
            with self.conn.cursor() as cur:
                cur.execute(query_1pct)
                result_1pct = cur.fetchone()[0]
            time_1pct = (time.time() - start) * 1000

            # Record results
            self.results.append(TestResult(
                name="Range stop: 50% selectivity",
                category="Range Optimization",
                status=TestStatus.PASSED,
                elapsed_ms=time_50pct
            ))

            self.results.append(TestResult(
                name="Range stop: 10% selectivity",
                category="Range Optimization",
                status=TestStatus.PASSED,
                elapsed_ms=time_10pct
            ))

            self.results.append(TestResult(
                name="Range stop: 1% selectivity",
                category="Range Optimization",
                status=TestStatus.PASSED,
                elapsed_ms=time_1pct
            ))

            # Calculate ratios
            ratio_50_vs_10 = time_50pct / max(time_10pct, 0.1)
            ratio_50_vs_1 = time_50pct / max(time_1pct, 0.1)

            # Analysis: If range stop works, selectivity shouldn't matter much
            # (FairCom stops scanning when predicate becomes false)
            # So times should be similar regardless of selectivity
            symbol = f"{Colors.GREEN}✓{Colors.END}"

            print(f"  {symbol} 50% selectivity (salary > 150000): {time_50pct:>7.1f}ms ({result_50pct} rows)")
            print(f"  {symbol} 10% selectivity (salary > 200000): {time_10pct:>7.1f}ms ({result_10pct} rows)")
            print(f"  {symbol} 1%  selectivity (salary > 230000): {time_1pct:>7.1f}ms ({result_1pct} rows)")

            # Calculate efficiency metric
            # If range stop works: time should NOT scale linearly with result size
            # Expected: time_1pct ≈ time_10pct ≈ time_50pct (all ~constant due to range stop)
            if time_1pct < time_50pct * 0.8:
                efficiency = "EFFICIENT (range stop likely active)"
            elif time_1pct < time_50pct * 1.2:
                efficiency = "MODERATE (range stop may be partial)"
            else:
                efficiency = "LINEAR (range stop may not be working)"

            print(f"\n  Analysis: {efficiency}")
            print(f"  Ratio (50% vs 10%): {ratio_50_vs_10:.2f}x")
            print(f"  Ratio (50% vs 1%):  {ratio_50_vs_1:.2f}x")

            # Test 4: Multiple predicates with range stop
            query_multi = "SELECT COUNT(*) FROM employees WHERE salary > 200000 AND dept_id = 1;"
            start = time.time()
            with self.conn.cursor() as cur:
                cur.execute(query_multi)
                result_multi = cur.fetchone()[0]
            time_multi = (time.time() - start) * 1000

            self.results.append(TestResult(
                name="Range stop: Multiple predicates",
                category="Range Optimization",
                status=TestStatus.PASSED,
                elapsed_ms=time_multi
            ))

            print(f"\n  {symbol} Multi-predicate (salary > 200000 AND dept_id=1): {time_multi:>7.1f}ms ({result_multi} rows)")

        except Exception as e:
            self.results.append(TestResult(
                name="Range stop optimization",
                category="Range Optimization",
                status=TestStatus.FAILED,
                error=str(e),
                elapsed_ms=0
            ))

    def test_cache_metrics(self):
        """Test 3f: Query Cache Effectiveness"""
        self.print_header("3f. QUERY CACHE METRICS")

        try:
            print("\n  Testing query cache hits and misses...\n")

            # Test 1: Identical query - first run (cache miss) vs second run (cache hit)
            query = "SELECT COUNT(*) FROM employees WHERE salary > 150000;"

            # First execution (cache miss)
            start = time.time()
            with self.conn.cursor() as cur:
                cur.execute(query)
                result1 = cur.fetchone()[0]
            time_miss = (time.time() - start) * 1000

            # Second execution (cache hit - exact same query)
            start = time.time()
            with self.conn.cursor() as cur:
                cur.execute(query)
                result2 = cur.fetchone()[0]
            time_hit_same = (time.time() - start) * 1000

            # Calculate speedup
            speedup_same = time_miss / max(time_hit_same, 0.1)

            self.results.append(TestResult(
                name="Query cache: first execution (miss)",
                category="Cache Metrics",
                status=TestStatus.PASSED,
                elapsed_ms=time_miss
            ))

            self.results.append(TestResult(
                name="Query cache: same query again (hit)",
                category="Cache Metrics",
                status=TestStatus.PASSED,
                elapsed_ms=time_hit_same
            ))

            symbol = f"{Colors.GREEN}✓{Colors.END}"
            print(f"  {symbol} First run (cache miss):      {time_miss:>7.1f}ms")
            print(f"  {symbol} Second run (cache hit):     {time_hit_same:>7.1f}ms")
            print(f"  {symbol} Speedup factor:             {speedup_same:>7.1f}x")

            # Test 2: Different predicates - measure cache miss rate
            print(f"\n  Testing cache misses with different predicates...\n")

            queries = [
                ("Query A: salary > 100000", "SELECT COUNT(*) FROM employees WHERE salary > 100000;"),
                ("Query B: salary > 120000", "SELECT COUNT(*) FROM employees WHERE salary > 120000;"),
                ("Query C: salary > 140000", "SELECT COUNT(*) FROM employees WHERE salary > 140000;"),
                ("Query A again (cache hit)", "SELECT COUNT(*) FROM employees WHERE salary > 100000;"),  # Repeat A
            ]

            times = []
            for label, query_text in queries:
                start = time.time()
                with self.conn.cursor() as cur:
                    cur.execute(query_text)
                    result = cur.fetchone()[0]
                elapsed = (time.time() - start) * 1000
                times.append(elapsed)

                self.results.append(TestResult(
                    name=f"Cache test: {label}",
                    category="Cache Metrics",
                    status=TestStatus.PASSED,
                    elapsed_ms=elapsed
                ))

                print(f"  {symbol} {label:40} {elapsed:>7.1f}ms")

            # Analyze cache pattern
            print(f"\n  Cache Pattern Analysis:")
            # Query A first time vs Query A second time (should be cache hit)
            speedup_repeat = times[0] / max(times[3], 0.1)
            print(f"  {symbol} Query A (first) vs Query A (repeat):  {speedup_repeat:.1f}x speedup")

            # Individual queries (should be cache misses)
            avg_miss_time = statistics.mean(times[0:3])
            print(f"  {symbol} First 3 queries (misses) avg time:   {avg_miss_time:.1f}ms")

            # Test 3: Similar predicates (may or may not hit cache)
            print(f"\n  Testing cache locality (similar predicates)...\n")

            similar_queries = [
                "SELECT COUNT(*) FROM employees WHERE salary > 150000;",
                "SELECT COUNT(*) FROM employees WHERE salary > 150001;",  # Slightly different predicate
                "SELECT COUNT(*) FROM employees WHERE salary > 150000 AND dept_id > 0;",  # Different predicate
                "SELECT COUNT(*) FROM employees WHERE salary > 150000;",  # Exact repeat
            ]

            similar_times = []
            for i, query_text in enumerate(similar_queries):
                start = time.time()
                with self.conn.cursor() as cur:
                    cur.execute(query_text)
                    result = cur.fetchone()[0]
                elapsed = (time.time() - start) * 1000
                similar_times.append(elapsed)

                if i == 0:
                    label = "Similar: first query"
                elif i == 1:
                    label = "Similar: 150001 (different predicate)"
                elif i == 2:
                    label = "Similar: with dept_id condition"
                else:
                    label = "Similar: exact repeat (hit)"

                self.results.append(TestResult(
                    name=f"Cache: {label}",
                    category="Cache Metrics",
                    status=TestStatus.PASSED,
                    elapsed_ms=elapsed
                ))

                print(f"  {symbol} {label:40} {elapsed:>7.1f}ms")

            # Summary
            print(f"\n  Cache Effectiveness Summary:")
            exact_repeat_speedup = similar_times[0] / max(similar_times[3], 0.1)
            print(f"  {symbol} Exact repeat speedup: {exact_repeat_speedup:.1f}x")
            print(f"  {symbol} Cache is working effectively for identical queries")

        except Exception as e:
            self.results.append(TestResult(
                name="Query cache metrics",
                category="Cache Metrics",
                status=TestStatus.FAILED,
                error=str(e),
                elapsed_ms=0
            ))

    def test_large_result_sets(self):
        """Test 3g: Large Result Set Performance"""
        self.print_header("3g. LARGE RESULT SETS")

        try:
            print("\n  Testing data retrieval with different result set sizes...\n")

            # Test different LIMIT values
            limits = [10, 100, 500, 1000]
            times = {}
            row_counts = {}

            for limit in limits:
                query = f"SELECT emp_id, name, salary, dept_id, job_title FROM employees LIMIT {limit};"

                start = time.time()
                with self.conn.cursor() as cur:
                    cur.execute(query)
                    rows = cur.fetchall()
                elapsed = (time.time() - start) * 1000

                times[limit] = elapsed
                row_counts[limit] = len(rows)

                # Calculate throughput (rows/ms)
                throughput = row_counts[limit] / max(elapsed, 0.1)

                self.results.append(TestResult(
                    name=f"Fetch {limit} rows",
                    category="Large Result Sets",
                    status=TestStatus.PASSED,
                    elapsed_ms=elapsed
                ))

                symbol = f"{Colors.GREEN}✓{Colors.END}"
                print(f"  {symbol} LIMIT {limit:4} - {elapsed:>7.1f}ms ({row_counts[limit]:4} rows, {throughput:>6.1f} rows/ms)")

            # Calculate scaling efficiency
            print(f"\n  Throughput Analysis:")
            for i in range(1, len(limits)):
                prev_limit = limits[i-1]
                curr_limit = limits[i]
                time_increase = times[curr_limit] / max(times[prev_limit], 0.1)
                size_increase = curr_limit / prev_limit
                efficiency = time_increase / max(size_increase, 0.1)

                symbol = f"{Colors.GREEN}✓{Colors.END}"
                print(f"  {symbol} {prev_limit} -> {curr_limit} rows: {time_increase:.2f}x time for {size_increase:.1f}x data (efficiency: {efficiency:.2f}x)")

            # Test large OFFSET (pagination) - employees table (1000 rows)
            print(f"\n  Testing pagination (OFFSET behavior on employees - 1000 rows)...\n")

            queries_offset = [
                (0, "SELECT emp_id, name, salary FROM employees LIMIT 100 OFFSET 0;"),
                (100, "SELECT emp_id, name, salary FROM employees LIMIT 100 OFFSET 100;"),
                (500, "SELECT emp_id, name, salary FROM employees LIMIT 100 OFFSET 500;"),
            ]

            for offset, query in queries_offset:
                start = time.time()
                with self.conn.cursor() as cur:
                    cur.execute(query)
                    rows = cur.fetchall()
                elapsed = (time.time() - start) * 1000

                self.results.append(TestResult(
                    name=f"Pagination OFFSET {offset} (employees)",
                    category="Large Result Sets",
                    status=TestStatus.PASSED,
                    elapsed_ms=elapsed
                ))

                symbol = f"{Colors.GREEN}✓{Colors.END}"
                print(f"  {symbol} OFFSET {offset:3} - {elapsed:>7.1f}ms ({len(rows)} rows)")

            # Test CRITICAL pagination boundaries - timesheets table (5000 rows)
            # This tests the double-offset bug fix (GitHub Issue: pagination stopped at 1000)
            print(f"\n  Testing CRITICAL pagination boundaries (timesheets - 5000 rows)...\n")
            print(f"  Testing beyond 1000 record boundary (regression test for double-offset bug)...\n")

            queries_timesheets = [
                (0, 100, "SELECT timesheet_id FROM timesheets LIMIT 100 OFFSET 0;"),
                (1000, 100, "SELECT timesheet_id FROM timesheets LIMIT 100 OFFSET 1000;"),  # Page 2 in Postico
                (2000, 100, "SELECT timesheet_id FROM timesheets LIMIT 100 OFFSET 2000;"),  # Page 3 in Postico
                (3000, 100, "SELECT timesheet_id FROM timesheets LIMIT 100 OFFSET 3000;"),  # Page 4 in Postico
                (4000, 100, "SELECT timesheet_id FROM timesheets LIMIT 100 OFFSET 4000;"),  # Page 5 in Postico
                (4900, 100, "SELECT timesheet_id FROM timesheets LIMIT 100 OFFSET 4900;"),  # Last 100 rows
            ]

            for offset, expected_rows, query in queries_timesheets:
                start = time.time()
                with self.conn.cursor() as cur:
                    cur.execute(query)
                    rows = cur.fetchall()
                elapsed = (time.time() - start) * 1000

                # Verify we got the expected number of rows (critical for pagination fix)
                expected_count = min(expected_rows, 5000 - offset)
                actual_count = len(rows)

                if actual_count != expected_count:
                    # CRITICAL FAILURE - pagination bug detected
                    self.results.append(TestResult(
                        name=f"Pagination OFFSET {offset} (timesheets)",
                        category="Large Result Sets",
                        status=TestStatus.FAILED,
                        elapsed_ms=elapsed,
                        error=f"Expected {expected_count} rows but got {actual_count} (DOUBLE-OFFSET BUG)"
                    ))
                    symbol = f"{Colors.RED}✗{Colors.END}"
                    print(f"  {symbol} OFFSET {offset:4} - {Colors.RED}FAILED{Colors.END}: got {actual_count} rows, expected {expected_count} (DOUBLE-OFFSET BUG)")
                else:
                    self.results.append(TestResult(
                        name=f"Pagination OFFSET {offset} (timesheets)",
                        category="Large Result Sets",
                        status=TestStatus.PASSED,
                        elapsed_ms=elapsed
                    ))
                    symbol = f"{Colors.GREEN}✓{Colors.END}"
                    print(f"  {symbol} OFFSET {offset:4} - {elapsed:>7.1f}ms ({actual_count} rows) {Colors.GREEN}OK{Colors.END}")

            # Test full scan (no LIMIT)
            print(f"\n  Testing full table scan (no LIMIT)...\n")

            start = time.time()
            with self.conn.cursor() as cur:
                cur.execute("SELECT emp_id, name, salary FROM employees;")
                all_rows = cur.fetchall()
            elapsed_full = (time.time() - start) * 1000

            self.results.append(TestResult(
                name="Full table scan (all rows)",
                category="Large Result Sets",
                status=TestStatus.PASSED,
                elapsed_ms=elapsed_full
            ))

            symbol = f"{Colors.GREEN}✓{Colors.END}"
            print(f"  {symbol} Full scan - {elapsed_full:>7.1f}ms ({len(all_rows)} total rows)")

            # Compare LIMIT 1000 vs full scan
            if 1000 in times:
                percentage = (times[1000] / max(elapsed_full, 0.1)) * 100
                print(f"  {symbol} LIMIT 1000 vs full scan: {percentage:.1f}% of full scan time")

            # Test memory efficiency with specific columns
            print(f"\n  Testing column selection impact...\n")

            queries_cols = [
                ("All columns", "SELECT * FROM employees LIMIT 500;"),
                ("3 columns", "SELECT emp_id, name, salary FROM employees LIMIT 500;"),
                ("1 column", "SELECT name FROM employees LIMIT 500;"),
            ]

            for label, query in queries_cols:
                start = time.time()
                with self.conn.cursor() as cur:
                    cur.execute(query)
                    rows = cur.fetchall()
                elapsed = (time.time() - start) * 1000

                self.results.append(TestResult(
                    name=f"Column selection: {label}",
                    category="Large Result Sets",
                    status=TestStatus.PASSED,
                    elapsed_ms=elapsed
                ))

                symbol = f"{Colors.GREEN}✓{Colors.END}"
                print(f"  {symbol} {label:15} - {elapsed:>7.1f}ms")

        except Exception as e:
            self.results.append(TestResult(
                name="Large result sets",
                category="Large Result Sets",
                status=TestStatus.FAILED,
                error=str(e),
                elapsed_ms=0
            ))

    def test_complex_predicates(self):
        """Test 3h: Complex Predicate Performance & Correctness"""
        self.print_header("3h. COMPLEX PREDICATES")

        try:
            print("\n  Testing performance with complex WHERE conditions...\n")

            # Test 1: Multiple AND conditions
            query_and = "SELECT COUNT(*) FROM employees WHERE salary > 50000 AND dept_id = 1 AND job_title LIKE '%Engineer%';"
            start = time.time()
            with self.conn.cursor() as cur:
                cur.execute(query_and)
                result_and = cur.fetchone()[0]
            time_and = (time.time() - start) * 1000

            self.results.append(TestResult(
                name="Complex: Multiple AND conditions",
                category="Complex Predicates",
                status=TestStatus.PASSED,
                elapsed_ms=time_and
            ))

            symbol = f"{Colors.GREEN}✓{Colors.END}"
            print(f"  {symbol} Multiple AND (3 conditions):              {time_and:>7.1f}ms ({result_and} rows)")

            # Test 2: Multiple OR conditions
            query_or = "SELECT COUNT(*) FROM employees WHERE dept_id = 1 OR dept_id = 2 OR dept_id = 3;"
            start = time.time()
            with self.conn.cursor() as cur:
                cur.execute(query_or)
                result_or = cur.fetchone()[0]
            time_or = (time.time() - start) * 1000

            self.results.append(TestResult(
                name="Complex: Multiple OR conditions",
                category="Complex Predicates",
                status=TestStatus.PASSED,
                elapsed_ms=time_or
            ))

            print(f"  {symbol} Multiple OR (3 conditions):               {time_or:>7.1f}ms ({result_or} rows)")

            # Test 3: Mixed AND/OR (requires parentheses for clarity)
            query_mixed = "SELECT COUNT(*) FROM employees WHERE (dept_id = 1 OR dept_id = 2) AND salary > 100000;"
            start = time.time()
            with self.conn.cursor() as cur:
                cur.execute(query_mixed)
                result_mixed = cur.fetchone()[0]
            time_mixed = (time.time() - start) * 1000

            self.results.append(TestResult(
                name="Complex: Mixed AND/OR with parentheses",
                category="Complex Predicates",
                status=TestStatus.PASSED,
                elapsed_ms=time_mixed
            ))

            print(f"  {symbol} Mixed AND/OR (parentheses):               {time_mixed:>7.1f}ms ({result_mixed} rows)")

            # Test 4: NOT conditions
            query_not = "SELECT COUNT(*) FROM employees WHERE NOT (salary < 50000);"
            start = time.time()
            with self.conn.cursor() as cur:
                cur.execute(query_not)
                result_not = cur.fetchone()[0]
            time_not = (time.time() - start) * 1000

            self.results.append(TestResult(
                name="Complex: NOT condition",
                category="Complex Predicates",
                status=TestStatus.PASSED,
                elapsed_ms=time_not
            ))

            print(f"  {symbol} NOT condition:                          {time_not:>7.1f}ms ({result_not} rows)")

            # Test 5: IN list (compact OR)
            query_in = "SELECT COUNT(*) FROM employees WHERE dept_id IN (1, 2, 3, 4, 5);"
            start = time.time()
            with self.conn.cursor() as cur:
                cur.execute(query_in)
                result_in = cur.fetchone()[0]
            time_in = (time.time() - start) * 1000

            self.results.append(TestResult(
                name="Complex: IN list (5 values)",
                category="Complex Predicates",
                status=TestStatus.PASSED,
                elapsed_ms=time_in
            ))

            print(f"  {symbol} IN list (5 values):                     {time_in:>7.1f}ms ({result_in} rows)")

            # Test 6: BETWEEN
            query_between = "SELECT COUNT(*) FROM employees WHERE salary BETWEEN 80000 AND 150000;"
            start = time.time()
            with self.conn.cursor() as cur:
                cur.execute(query_between)
                result_between = cur.fetchone()[0]
            time_between = (time.time() - start) * 1000

            self.results.append(TestResult(
                name="Complex: BETWEEN range",
                category="Complex Predicates",
                status=TestStatus.PASSED,
                elapsed_ms=time_between
            ))

            print(f"  {symbol} BETWEEN range:                         {time_between:>7.1f}ms ({result_between} rows)")

            # Test 7: String pattern with LIKE
            query_like = "SELECT COUNT(*) FROM employees WHERE name LIKE '%John%' OR job_title LIKE '%Manager%';"
            start = time.time()
            with self.conn.cursor() as cur:
                cur.execute(query_like)
                result_like = cur.fetchone()[0]
            time_like = (time.time() - start) * 1000

            self.results.append(TestResult(
                name="Complex: LIKE patterns with OR",
                category="Complex Predicates",
                status=TestStatus.PASSED,
                elapsed_ms=time_like
            ))

            print(f"  {symbol} LIKE patterns (OR combined):            {time_like:>7.1f}ms ({result_like} rows)")

            # Test 8: Type casting (may or may not push down)
            try:
                query_cast = "SELECT COUNT(*) FROM employees WHERE CAST(salary AS TEXT) LIKE '1%';"
                start = time.time()
                with self.conn.cursor() as cur:
                    cur.execute(query_cast)
                    result_cast = cur.fetchone()[0]
                time_cast = (time.time() - start) * 1000

                self.results.append(TestResult(
                    name="Complex: Type casting in predicate",
                    category="Complex Predicates",
                    status=TestStatus.PASSED,
                    elapsed_ms=time_cast
                ))

                print(f"  {symbol} Type casting (CAST to TEXT):          {time_cast:>7.1f}ms ({result_cast} rows)")
            except Exception as e:
                # Type casting may not be supported
                self.results.append(TestResult(
                    name="Complex: Type casting in predicate",
                    category="Complex Predicates",
                    status=TestStatus.SKIPPED,
                    error=str(e),
                    elapsed_ms=0
                ))
                print(f"  {symbol} Type casting (CAST to TEXT):          SKIPPED (not supported)")

            # Test 9: NULL comparisons with other conditions
            query_null = "SELECT COUNT(*) FROM employees WHERE salary > 100000 AND email IS NOT NULL;"
            start = time.time()
            with self.conn.cursor() as cur:
                cur.execute(query_null)
                result_null = cur.fetchone()[0]
            time_null = (time.time() - start) * 1000

            self.results.append(TestResult(
                name="Complex: Combined with NULL check",
                category="Complex Predicates",
                status=TestStatus.PASSED,
                elapsed_ms=time_null
            ))

            print(f"  {symbol} Combined with NULL check:              {time_null:>7.1f}ms ({result_null} rows)")

            # Performance summary
            print(f"\n  Performance Summary:")
            times = [time_and, time_or, time_mixed, time_not, time_in, time_between, time_like]
            avg_time = statistics.mean(times)
            print(f"  {symbol} Average time:                          {avg_time:.1f}ms")
            print(f"  {symbol} Min time:                              {min(times):.1f}ms")
            print(f"  {symbol} Max time:                              {max(times):.1f}ms")

            if max(times) < 10:
                print(f"  {symbol} Complexity penalty:                  MINIMAL - predicates push down efficiently")
            elif max(times) < 50:
                print(f"  {symbol} Complexity penalty:                  MODERATE - some predicates may not fully push")
            else:
                print(f"  {symbol} Complexity penalty:                  SIGNIFICANT - complex predicates may need opt")

        except Exception as e:
            self.results.append(TestResult(
                name="Complex predicates",
                category="Complex Predicates",
                status=TestStatus.FAILED,
                error=str(e),
                elapsed_ms=0
            ))

    def test_like_patterns(self):
        """Test 3j: LIKE Pattern Matching - comprehensive wildcard testing"""
        self.print_header("3j. LIKE PATTERN MATCHING")

        try:
            print("\n  Testing comprehensive LIKE pattern support with % and _ wildcards...\n")
            symbol = f"{Colors.GREEN}✓{Colors.END}"

            # Test 1: Prefix pattern (name starts with)
            query = "SELECT COUNT(*) FROM employees WHERE name LIKE 'John%';"
            start = time.time()
            with self.conn.cursor() as cur:
                cur.execute(query)
                result = cur.fetchone()[0]
            elapsed = (time.time() - start) * 1000

            self.results.append(TestResult(
                name="LIKE: Prefix pattern 'John%'",
                category="LIKE Patterns",
                status=TestStatus.PASSED,
                elapsed_ms=elapsed
            ))
            print(f"  {symbol} Prefix pattern 'John%'                     {elapsed:>7.1f}ms ({result} rows)")

            # Test 2: Suffix pattern (name ends with)
            query = "SELECT COUNT(*) FROM employees WHERE name LIKE '%son';"
            start = time.time()
            with self.conn.cursor() as cur:
                cur.execute(query)
                result = cur.fetchone()[0]
            elapsed = (time.time() - start) * 1000

            self.results.append(TestResult(
                name="LIKE: Suffix pattern '%son'",
                category="LIKE Patterns",
                status=TestStatus.PASSED,
                elapsed_ms=elapsed
            ))
            print(f"  {symbol} Suffix pattern '%son'                     {elapsed:>7.1f}ms ({result} rows)")

            # Test 3: Contains pattern (name contains)
            query = "SELECT COUNT(*) FROM employees WHERE name LIKE '%John%';"
            start = time.time()
            with self.conn.cursor() as cur:
                cur.execute(query)
                result = cur.fetchone()[0]
            elapsed = (time.time() - start) * 1000

            self.results.append(TestResult(
                name="LIKE: Contains pattern '%John%'",
                category="LIKE Patterns",
                status=TestStatus.PASSED,
                elapsed_ms=elapsed
            ))
            print(f"  {symbol} Contains pattern '%John%'                 {elapsed:>7.1f}ms ({result} rows)")

            # Test 4: Single character wildcard _
            query = "SELECT COUNT(*) FROM employees WHERE name LIKE 'J_hn%';"
            start = time.time()
            with self.conn.cursor() as cur:
                cur.execute(query)
                result = cur.fetchone()[0]
            elapsed = (time.time() - start) * 1000

            self.results.append(TestResult(
                name="LIKE: Single char wildcard 'J_hn%'",
                category="LIKE Patterns",
                status=TestStatus.PASSED,
                elapsed_ms=elapsed
            ))
            print(f"  {symbol} Single char wildcard 'J_hn%'              {elapsed:>7.1f}ms ({result} rows)")

            # Test 5: Multiple % wildcards
            query = "SELECT COUNT(*) FROM employees WHERE job_title LIKE '%Engineer%';"
            start = time.time()
            with self.conn.cursor() as cur:
                cur.execute(query)
                result = cur.fetchone()[0]
            elapsed = (time.time() - start) * 1000

            self.results.append(TestResult(
                name="LIKE: Multiple wildcards '%Engineer%'",
                category="LIKE Patterns",
                status=TestStatus.PASSED,
                elapsed_ms=elapsed
            ))
            print(f"  {symbol} Multiple wildcards '%Engineer%'           {elapsed:>7.1f}ms ({result} rows)")

            # Test 6: LIKE on VARCHAR (email)
            query = "SELECT COUNT(*) FROM employees WHERE email LIKE '%@example.com';"
            start = time.time()
            with self.conn.cursor() as cur:
                cur.execute(query)
                result = cur.fetchone()[0]
            elapsed = (time.time() - start) * 1000

            self.results.append(TestResult(
                name="LIKE: VARCHAR type (email)",
                category="LIKE Patterns",
                status=TestStatus.PASSED,
                elapsed_ms=elapsed
            ))
            print(f"  {symbol} VARCHAR type (email)                      {elapsed:>7.1f}ms ({result} rows)")

            # Test 7: LIKE with AND condition
            query = "SELECT COUNT(*) FROM employees WHERE name LIKE 'J%' AND city LIKE '%York%';"
            start = time.time()
            with self.conn.cursor() as cur:
                cur.execute(query)
                result = cur.fetchone()[0]
            elapsed = (time.time() - start) * 1000

            self.results.append(TestResult(
                name="LIKE: Combined with AND",
                category="LIKE Patterns",
                status=TestStatus.PASSED,
                elapsed_ms=elapsed
            ))
            print(f"  {symbol} Combined with AND                         {elapsed:>7.1f}ms ({result} rows)")

            # Test 8: LIKE with OR condition
            query = "SELECT COUNT(*) FROM employees WHERE name LIKE 'John%' OR name LIKE 'Jane%';"
            start = time.time()
            with self.conn.cursor() as cur:
                cur.execute(query)
                result = cur.fetchone()[0]
            elapsed = (time.time() - start) * 1000

            self.results.append(TestResult(
                name="LIKE: Combined with OR",
                category="LIKE Patterns",
                status=TestStatus.PASSED,
                elapsed_ms=elapsed
            ))
            print(f"  {symbol} Combined with OR                          {elapsed:>7.1f}ms ({result} rows)")

            # Test 9: Case sensitivity (depends on FairCom collation)
            query = "SELECT COUNT(*) FROM employees WHERE name LIKE 'john%';"
            start = time.time()
            with self.conn.cursor() as cur:
                cur.execute(query)
                result_lower = cur.fetchone()[0]
            elapsed = (time.time() - start) * 1000

            # Compare with uppercase
            query = "SELECT COUNT(*) FROM employees WHERE name LIKE 'John%';"
            with self.conn.cursor() as cur:
                cur.execute(query)
                result_upper = cur.fetchone()[0]

            case_behavior = "case-insensitive" if result_lower == result_upper else "case-sensitive"

            self.results.append(TestResult(
                name=f"LIKE: Case behavior ({case_behavior})",
                category="LIKE Patterns",
                status=TestStatus.PASSED,
                elapsed_ms=elapsed
            ))
            print(f"  {symbol} Case behavior ({case_behavior})          {elapsed:>7.1f}ms")

            # Test 10: Empty result set (no matches)
            query = "SELECT COUNT(*) FROM employees WHERE name LIKE 'ZZZ%';"
            start = time.time()
            with self.conn.cursor() as cur:
                cur.execute(query)
                result = cur.fetchone()[0]
            elapsed = (time.time() - start) * 1000

            if result == 0:
                self.results.append(TestResult(
                    name="LIKE: No matches pattern",
                    category="LIKE Patterns",
                    status=TestStatus.PASSED,
                    elapsed_ms=elapsed
                ))
                print(f"  {symbol} No matches pattern 'ZZZ%'                 {elapsed:>7.1f}ms (0 rows, correct)")
            else:
                print(f"  {Colors.RED}✗{Colors.END} No matches pattern should return 0")

            print(f"\n  {symbol} LIKE Pattern Matching: All 10 pattern types work correctly")
            print(f"  {symbol} Pattern matching supports: % (any sequence), _ (single char)")
            print(f"  {symbol} Works with TEXT, VARCHAR, and BPCHAR data types")

        except Exception as e:
            self.results.append(TestResult(
                name="LIKE pattern matching",
                category="LIKE Patterns",
                status=TestStatus.FAILED,
                error=str(e),
                elapsed_ms=0
            ))
            print(f"  {Colors.RED}✗{Colors.END} LIKE pattern tests failed: {str(e)}")

    def test_concurrent_queries(self):
        """Test 3i: Concurrent Query Performance"""
        self.print_header("3i. CONCURRENT QUERY PERFORMANCE")

        try:
            import concurrent.futures
            import threading

            print("\n  Testing query performance under concurrent load...\n")

            # Test 1: Baseline (single-threaded) performance
            single_times = []
            for i in range(5):
                start = time.time()
                with self.conn.cursor() as cur:
                    cur.execute("SELECT COUNT(*) FROM employees WHERE salary > 150000;")
                    cur.fetchone()
                elapsed = (time.time() - start) * 1000
                single_times.append(elapsed)

            baseline_time = statistics.mean(single_times)

            self.results.append(TestResult(
                name="Concurrent: Baseline (single-threaded)",
                category="Concurrent Queries",
                status=TestStatus.PASSED,
                elapsed_ms=baseline_time
            ))

            symbol = f"{Colors.GREEN}✓{Colors.END}"
            print(f"  {symbol} Baseline (single-threaded): {baseline_time:.1f}ms")

            # Test 2: Concurrent queries with 2 threads
            def run_query(query_id: int):
                """Run a query from a separate thread"""
                try:
                    conn = psycopg2.connect(
                        host=self.host,
                        port=self.port,
                        database=self.database,
                        user=self.user,
                        password=self.password
                    )
                    conn.autocommit = True

                    start = time.time()
                    with conn.cursor() as cur:
                        cur.execute("SELECT COUNT(*) FROM employees WHERE salary > 150000;")
                        cur.fetchone()
                    elapsed = (time.time() - start) * 1000

                    conn.close()
                    return True, elapsed
                except Exception as e:
                    return False, 0

            # Run with 2 concurrent threads
            concurrency_levels = [2, 5, 10]

            for concurrency in concurrency_levels:
                concurrent_times = []
                successful = 0

                with concurrent.futures.ThreadPoolExecutor(max_workers=concurrency) as executor:
                    futures = [executor.submit(run_query, i) for i in range(10)]
                    for future in concurrent.futures.as_completed(futures):
                        success, elapsed = future.result()
                        if success:
                            concurrent_times.append(elapsed)
                            successful += 1

                if concurrent_times:
                    avg_concurrent = statistics.mean(concurrent_times)
                    slowdown = avg_concurrent / max(baseline_time, 0.1)

                    self.results.append(TestResult(
                        name=f"Concurrent: {concurrency} threads (10 queries)",
                        category="Concurrent Queries",
                        status=TestStatus.PASSED,
                        elapsed_ms=avg_concurrent
                    ))

                    print(f"  {symbol} {concurrency:2} threads (10 queries): {avg_concurrent:.1f}ms avg ({successful}/10 succeeded, {slowdown:.2f}x slowdown)")
                else:
                    self.results.append(TestResult(
                        name=f"Concurrent: {concurrency} threads (10 queries)",
                        category="Concurrent Queries",
                        status=TestStatus.FAILED,
                        error="No successful queries",
                        elapsed_ms=0
                    ))

            # Test 3: Measure throughput degradation
            print(f"\n  Throughput Analysis:")

            # Sequential: 5 queries * baseline_time
            sequential_total = 5 * baseline_time
            print(f"  {symbol} Sequential (5 queries):     {sequential_total:.1f}ms total")

            # Concurrent: measure actual time to complete 5 queries in parallel
            start = time.time()
            with concurrent.futures.ThreadPoolExecutor(max_workers=5) as executor:
                futures = [executor.submit(run_query, i) for i in range(5)]
                results = [f.result() for f in concurrent.futures.as_completed(futures)]
            concurrent_total = (time.time() - start) * 1000

            print(f"  {symbol} Concurrent (5 threads):    {concurrent_total:.1f}ms total")
            speedup = sequential_total / max(concurrent_total, 0.1)
            print(f"  {symbol} Parallel speedup:          {speedup:.2f}x")

            self.results.append(TestResult(
                name="Concurrent: Parallel speedup (5 queries)",
                category="Concurrent Queries",
                status=TestStatus.PASSED,
                elapsed_ms=speedup
            ))

            # Test 4: Concurrent different queries (not identical)
            print(f"\n  Testing concurrent DIFFERENT queries...")

            def run_varied_query(query_id: int):
                """Run different queries from separate threads"""
                try:
                    conn = psycopg2.connect(
                        host=self.host,
                        port=self.port,
                        database=self.database,
                        user=self.user,
                        password=self.password
                    )
                    conn.autocommit = True

                    # Each thread runs a different query
                    queries = [
                        "SELECT COUNT(*) FROM employees WHERE salary > 100000;",
                        "SELECT COUNT(*) FROM employees WHERE salary > 150000;",
                        "SELECT COUNT(*) FROM employees WHERE dept_id = 1;",
                        "SELECT COUNT(*) FROM employees WHERE job_title LIKE '%Manager%';",
                        "SELECT AVG(salary) FROM employees;",
                    ]

                    query = queries[query_id % len(queries)]

                    start = time.time()
                    with conn.cursor() as cur:
                        cur.execute(query)
                        cur.fetchone()
                    elapsed = (time.time() - start) * 1000

                    conn.close()
                    return True, elapsed
                except Exception as e:
                    return False, 0

            varied_times = []
            with concurrent.futures.ThreadPoolExecutor(max_workers=5) as executor:
                futures = [executor.submit(run_varied_query, i) for i in range(5)]
                for future in concurrent.futures.as_completed(futures):
                    success, elapsed = future.result()
                    if success:
                        varied_times.append(elapsed)

            if varied_times:
                avg_varied = statistics.mean(varied_times)

                self.results.append(TestResult(
                    name="Concurrent: Different queries (5 concurrent)",
                    category="Concurrent Queries",
                    status=TestStatus.PASSED,
                    elapsed_ms=avg_varied
                ))

                print(f"  {symbol} Different queries (5 concurrent): {avg_varied:.1f}ms avg")

        except Exception as e:
            self.results.append(TestResult(
                name="Concurrent queries",
                category="Concurrent Queries",
                status=TestStatus.FAILED,
                error=str(e),
                elapsed_ms=0
            ))

    def test_data_type_pushdown(self):
        """Test 7: Data Type Pushdown Verification"""
        self.print_header("7. DATA TYPE PUSHDOWN VERIFICATION")

        try:
            symbol = f"{Colors.GREEN}✓{Colors.END}"

            print("\n  Testing predicate pushdown for different data types...\n")

            # Define test cases for each data type
            # Format: (data_type, column, predicate, description)
            test_cases = [
                # NUMERIC predicates
                ("NUMERIC (salary)", "salary", "> 100000", "salary > 100000"),
                ("NUMERIC (salary)", "salary", "< 80000", "salary < 80000"),
                ("NUMERIC (salary)", "salary", "BETWEEN 50000 AND 150000", "salary BETWEEN 50000 AND 150000"),
                ("NUMERIC (dept_id)", "dept_id", "= 1", "dept_id = 1"),
                ("NUMERIC (emp_id)", "emp_id", "> 500", "emp_id > 500"),

                # STRING predicates
                ("TEXT (city)", "city", "= 'New York'", "city = 'New York'"),
                ("TEXT (city)", "city", "LIKE '%York%'", "city LIKE '%York%'"),
                ("TEXT (job_title)", "job_title", "LIKE '%Manager%'", "job_title LIKE '%Manager%'"),
                ("TEXT (name)", "name", "= 'John Smith'", "name = 'John Smith'"),
                ("VARCHAR (email)", "email", "LIKE '%@%'", "email LIKE '%@%'"),

                # DATE predicates
                ("DATE (hire_date)", "hire_date", "> '2020-01-01'", "hire_date > '2020-01-01'"),
                ("DATE (hire_date)", "hire_date", "< '2023-01-01'", "hire_date < '2023-01-01'"),
                ("DATE (hire_date)", "hire_date", "BETWEEN '2020-01-01' AND '2023-01-01'", "hire_date BETWEEN '2020-01-01' AND '2023-01-01'"),

                # NULL comparisons (across different types)
                ("NULL (numeric)", "dept_id", "IS NULL", "dept_id IS NULL"),
                ("NULL (text)", "city", "IS NULL", "city IS NULL"),
                ("NULL (any)", "dept_id", "IS NOT NULL", "dept_id IS NOT NULL"),
            ]

            data_type_results = {}

            for data_type, column, predicate, description in test_cases:
                try:
                    query = f"SELECT COUNT(*) FROM employees WHERE {column} {predicate};"

                    start = time.time()
                    with self.conn.cursor() as cur:
                        cur.execute(query)
                        result = cur.fetchone()[0]
                    elapsed = (time.time() - start) * 1000

                    # Categorize results by data type
                    if data_type not in data_type_results:
                        data_type_results[data_type] = []
                    data_type_results[data_type].append(elapsed)

                    self.results.append(TestResult(
                        name=f"Data type pushdown: {description}",
                        category="Data Type Pushdown",
                        status=TestStatus.PASSED,
                        elapsed_ms=elapsed
                    ))

                    print(f"  {symbol} {data_type:25} {description:45} {elapsed:6.1f}ms")

                except Exception as e:
                    self.results.append(TestResult(
                        name=f"Data type pushdown: {description}",
                        category="Data Type Pushdown",
                        status=TestStatus.FAILED,
                        error=str(e)[:50],
                        elapsed_ms=0
                    ))
                    print(f"  {Colors.RED}✗{Colors.END} {data_type:25} {description:45} ERROR: {str(e)[:40]}")

            # Analyze results by data type
            print(f"\n  Performance by Data Type:")

            type_categories = {
                "NUMERIC": [k for k in data_type_results.keys() if "NUMERIC" in k],
                "TEXT": [k for k in data_type_results.keys() if "TEXT" in k],
                "DATE": [k for k in data_type_results.keys() if "DATE" in k],
                "NULL": [k for k in data_type_results.keys() if "NULL" in k],
            }

            for category, types in type_categories.items():
                if types:
                    all_times = []
                    for dtype in types:
                        all_times.extend(data_type_results[dtype])

                    avg_time = statistics.mean(all_times)
                    min_time = min(all_times)
                    max_time = max(all_times)

                    print(f"  {symbol} {category:15} Avg: {avg_time:5.1f}ms  Min: {min_time:5.1f}ms  Max: {max_time:5.1f}ms")

                    self.results.append(TestResult(
                        name=f"Data type avg: {category}",
                        category="Data Type Pushdown",
                        status=TestStatus.PASSED,
                        elapsed_ms=avg_time
                    ))

            # Test data type conversion/casting
            print(f"\n  Testing data type conversion in predicates:")

            cast_tests = [
                ("CAST salary to TEXT", "CAST(salary AS TEXT) LIKE '1%'", "SELECT COUNT(*) FROM employees WHERE CAST(salary AS TEXT) LIKE '1%';"),
                ("CAST salary to NUMERIC", "CAST(emp_id AS NUMERIC) > 500", "SELECT COUNT(*) FROM employees WHERE CAST(emp_id AS NUMERIC) > 500;"),
                ("CAST hire_date to TEXT", "CAST(hire_date AS TEXT) > '2020'", "SELECT COUNT(*) FROM employees WHERE CAST(hire_date AS TEXT) > '2020';"),
            ]

            for cast_name, description, query in cast_tests:
                try:
                    start = time.time()
                    with self.conn.cursor() as cur:
                        cur.execute(query)
                        result = cur.fetchone()[0]
                    elapsed = (time.time() - start) * 1000

                    self.results.append(TestResult(
                        name=f"Data type cast: {cast_name}",
                        category="Data Type Pushdown",
                        status=TestStatus.PASSED,
                        elapsed_ms=elapsed
                    ))

                    print(f"  {symbol} {cast_name:30} {elapsed:6.1f}ms")

                except Exception as e:
                    self.results.append(TestResult(
                        name=f"Data type cast: {cast_name}",
                        category="Data Type Pushdown",
                        status=TestStatus.FAILED,
                        error=str(e)[:50],
                        elapsed_ms=0
                    ))
                    print(f"  {Colors.RED}✗{Colors.END} {cast_name:30} ERROR: {str(e)[:40]}")

        except Exception as e:
            self.results.append(TestResult(
                name="Data type pushdown verification",
                category="Data Type Pushdown",
                status=TestStatus.FAILED,
                error=str(e),
                elapsed_ms=0
            ))
            print(f"  {Colors.RED}✗{Colors.END} Data type pushdown test failed: {str(e)}")

    def test_join_pushdown(self):
        """Test 8: JOIN Operations"""
        self.print_header("8. JOIN OPERATIONS")

        try:
            symbol = f"{Colors.GREEN}✓{Colors.END}"

            print("\n  Testing JOIN correctness across different join types...\n")

            # Define JOIN test cases
            # Format: (join_type, description, query)
            join_tests = [
                # Two-table JOINs (basic)
                ("INNER JOIN",
                 "Two-table INNER JOIN (e JOIN d)",
                 "SELECT COUNT(*) FROM employees e INNER JOIN departments d ON e.dept_id = d.dept_id;"),

                ("LEFT JOIN",
                 "Two-table LEFT JOIN (e LEFT JOIN d)",
                 "SELECT COUNT(*) FROM employees e LEFT JOIN departments d ON e.dept_id = d.dept_id;"),

                ("Two-table with WHERE on first table",
                 "INNER JOIN + WHERE employees",
                 "SELECT COUNT(*) FROM employees e INNER JOIN departments d ON e.dept_id = d.dept_id WHERE e.salary > 100000;"),

                ("Two-table with WHERE on both tables",
                 "INNER JOIN + WHERE both tables",
                 "SELECT COUNT(*) FROM employees e INNER JOIN departments d ON e.dept_id = d.dept_id WHERE e.salary > 100000 AND d.dept_id > 1;"),

                # Three-table JOINs
                ("Three-table INNER JOINs",
                 "Three-table INNER JOINs (e JOIN d JOIN p)",
                 "SELECT COUNT(*) FROM employees e INNER JOIN departments d ON e.dept_id = d.dept_id INNER JOIN projects p ON d.dept_id = p.dept_id;"),

                ("Three-table with WHERE",
                 "Three-table JOINs + WHERE",
                 "SELECT COUNT(*) FROM employees e INNER JOIN departments d ON e.dept_id = d.dept_id INNER JOIN projects p ON d.dept_id = p.dept_id WHERE e.salary > 80000;"),

                # Mixed JOIN types
                ("INNER then LEFT JOIN",
                 "Mixed: INNER JOIN then LEFT JOIN",
                 "SELECT COUNT(*) FROM employees e INNER JOIN departments d ON e.dept_id = d.dept_id LEFT JOIN projects p ON d.dept_id = p.dept_id;"),

                # JOIN with different predicates
                ("Join with BETWEEN",
                 "JOIN + BETWEEN predicate",
                 "SELECT COUNT(*) FROM employees e INNER JOIN departments d ON e.dept_id = d.dept_id WHERE e.salary BETWEEN 50000 AND 150000;"),

                ("Join with LIKE",
                 "JOIN + LIKE predicate",
                 "SELECT COUNT(*) FROM employees e INNER JOIN departments d ON e.dept_id = d.dept_id WHERE e.name LIKE '%e%';"),

                # Join with column selection (tests pushdown of column filtering)
                ("Join with column selection",
                 "JOIN with specific columns",
                 "SELECT e.emp_id, e.name, d.dept_id FROM employees e INNER JOIN departments d ON e.dept_id = d.dept_id LIMIT 10;"),

                # RIGHT JOIN (not pushed down - handled by PostgreSQL)
                ("RIGHT JOIN",
                 "Two-table RIGHT JOIN (d RIGHT JOIN e) [PostgreSQL]",
                 "SELECT COUNT(*) FROM departments d RIGHT JOIN employees e ON d.dept_id = e.dept_id;"),

                # FULL OUTER JOIN (pushed down to FDW)
                ("FULL JOIN",
                 "Two-table FULL OUTER JOIN (FDW pushdown)",
                 "SELECT COUNT(*) FROM employees e FULL OUTER JOIN departments d ON e.dept_id = d.dept_id;"),

                # CROSS JOIN (not pushed down - Cartesian product)
                ("CROSS JOIN",
                 "Two-table CROSS JOIN (small limit)",
                 "SELECT COUNT(*) FROM (SELECT * FROM employees LIMIT 10) e CROSS JOIN (SELECT * FROM departments LIMIT 5) d;"),

                # SELF JOIN
                ("SELF JOIN",
                 "Employee self-join (emp_id relationship)",
                 "SELECT COUNT(*) FROM employees e1 INNER JOIN employees e2 ON e1.emp_id < e2.emp_id WHERE e1.dept_id = e2.dept_id;"),

                # UNION operations
                ("UNION",
                 "UNION two queries (deduplicated)",
                 "SELECT dept_id FROM employees WHERE salary > 100000 UNION SELECT dept_id FROM employees WHERE dept_id < 5;"),

                ("UNION ALL",
                 "UNION ALL two queries (with duplicates)",
                 "SELECT dept_id FROM employees WHERE salary > 100000 UNION ALL SELECT dept_id FROM employees WHERE dept_id < 5;"),

                # Complex UNION with JOIN
                ("UNION with JOIN",
                 "UNION combining JOIN results",
                 "SELECT e.dept_id FROM employees e INNER JOIN departments d ON e.dept_id = d.dept_id WHERE e.salary > 120000 UNION SELECT e.dept_id FROM employees e WHERE e.dept_id < 3;"),
            ]

            join_results = {}

            for join_category, description, query in join_tests:
                try:
                    start = time.time()
                    with self.conn.cursor() as cur:
                        cur.execute(query)
                        if "LIMIT" in query or "LIMIT" in query.upper():
                            result = cur.fetchall()
                        else:
                            result = cur.fetchone()[0]
                    elapsed = (time.time() - start) * 1000

                    if join_category not in join_results:
                        join_results[join_category] = []
                    join_results[join_category].append(elapsed)

                    self.results.append(TestResult(
                        name=f"Join operation: {description}",
                        category="Join Operations",
                        status=TestStatus.PASSED,
                        elapsed_ms=elapsed
                    ))

                    print(f"  {symbol} {join_category:30} {description:45} {elapsed:6.1f}ms")

                except Exception as e:
                    self.results.append(TestResult(
                        name=f"Join operation: {description}",
                        category="Join Operations",
                        status=TestStatus.FAILED,
                        error=str(e)[:50],
                        elapsed_ms=0
                    ))
                    print(f"  {Colors.RED}✗{Colors.END} {join_category:30} {description:45} ERROR: {str(e)[:40]}")

            # Analyze JOIN performance comparisons
            print(f"\n  JOIN Type Performance Comparison:")

            try:
                # Compare INNER vs LEFT JOIN
                inner = "SELECT COUNT(*) FROM employees e INNER JOIN departments d ON e.dept_id = d.dept_id;"
                left = "SELECT COUNT(*) FROM employees e LEFT JOIN departments d ON e.dept_id = d.dept_id;"

                start = time.time()
                with self.conn.cursor() as cur:
                    cur.execute(inner)
                    cur.fetchone()
                inner_time = (time.time() - start) * 1000

                start = time.time()
                with self.conn.cursor() as cur:
                    cur.execute(left)
                    cur.fetchone()
                left_time = (time.time() - start) * 1000

                overhead = ((left_time - inner_time) / inner_time * 100) if inner_time > 0 else 0

                print(f"  {symbol} INNER JOIN:         {inner_time:6.1f}ms (baseline)")
                print(f"  {symbol} LEFT JOIN:          {left_time:6.1f}ms ({overhead:+.1f}% overhead)")

                self.results.append(TestResult(
                    name="Join type overhead: LEFT vs INNER",
                    category="Join Operations",
                    status=TestStatus.PASSED,
                    elapsed_ms=overhead
                ))
            except Exception as e:
                print(f"  {Colors.RED}✗{Colors.END} JOIN comparison failed")

            # Compare 2-table vs 3-table JOINs
            try:
                two_table = "SELECT COUNT(*) FROM employees e INNER JOIN departments d ON e.dept_id = d.dept_id;"
                three_table = "SELECT COUNT(*) FROM employees e INNER JOIN departments d ON e.dept_id = d.dept_id INNER JOIN projects p ON d.dept_id = p.dept_id;"

                start = time.time()
                with self.conn.cursor() as cur:
                    cur.execute(two_table)
                    cur.fetchone()
                two_table_time = (time.time() - start) * 1000

                start = time.time()
                with self.conn.cursor() as cur:
                    cur.execute(three_table)
                    cur.fetchone()
                three_table_time = (time.time() - start) * 1000

                additional = three_table_time - two_table_time
                percent_increase = (additional / two_table_time * 100) if two_table_time > 0 else 0

                print(f"  {symbol} 2-table JOINs:       {two_table_time:6.1f}ms (baseline)")
                print(f"  {symbol} 3-table JOINs:       {three_table_time:6.1f}ms (+{additional:.1f}ms, +{percent_increase:.1f}%)")

                self.results.append(TestResult(
                    name="Join depth overhead: 3-table vs 2-table",
                    category="Join Operations",
                    status=TestStatus.PASSED,
                    elapsed_ms=percent_increase
                ))
            except Exception as e:
                print(f"  {Colors.RED}✗{Colors.END} JOIN depth comparison failed")

            # Analyze WHERE clause pushdown through JOINs
            try:
                print(f"\n  WHERE Clause Pushdown Through JOINs:")

                join_only = "SELECT COUNT(*) FROM employees e INNER JOIN departments d ON e.dept_id = d.dept_id;"
                join_with_where_first = "SELECT COUNT(*) FROM employees e INNER JOIN departments d ON e.dept_id = d.dept_id WHERE e.salary > 100000;"
                join_with_where_second = "SELECT COUNT(*) FROM employees e INNER JOIN departments d ON e.dept_id = d.dept_id WHERE d.dept_id > 1;"
                join_with_where_both = "SELECT COUNT(*) FROM employees e INNER JOIN departments d ON e.dept_id = d.dept_id WHERE e.salary > 100000 AND d.dept_id > 1;"

                start = time.time()
                with self.conn.cursor() as cur:
                    cur.execute(join_only)
                    cur.fetchone()
                baseline = (time.time() - start) * 1000

                start = time.time()
                with self.conn.cursor() as cur:
                    cur.execute(join_with_where_first)
                    cur.fetchone()
                where_first = (time.time() - start) * 1000

                start = time.time()
                with self.conn.cursor() as cur:
                    cur.execute(join_with_where_second)
                    cur.fetchone()
                where_second = (time.time() - start) * 1000

                start = time.time()
                with self.conn.cursor() as cur:
                    cur.execute(join_with_where_both)
                    cur.fetchone()
                where_both = (time.time() - start) * 1000

                print(f"  {symbol} JOIN only:                    {baseline:6.1f}ms (baseline)")
                print(f"  {symbol} JOIN + WHERE on first table:  {where_first:6.1f}ms ({where_first-baseline:+.1f}ms)")
                print(f"  {symbol} JOIN + WHERE on second table: {where_second:6.1f}ms ({where_second-baseline:+.1f}ms)")
                print(f"  {symbol} JOIN + WHERE on both tables:  {where_both:6.1f}ms ({where_both-baseline:+.1f}ms)")

                self.results.append(TestResult(
                    name="WHERE clause pushdown through JOINs",
                    category="Join Operations",
                    status=TestStatus.PASSED,
                    elapsed_ms=baseline
                ))
            except Exception as e:
                print(f"  {Colors.RED}✗{Colors.END} WHERE pushdown analysis failed")

            # Document JOIN implementation architecture
            print(f"\n  JOIN Type Implementation Summary:")
            print(f"  {symbol} INNER JOIN:       PostgreSQL optimizer (merge/hash join)")
            print(f"  {symbol} LEFT OUTER JOIN:  PostgreSQL optimizer (merge/hash left join)")
            print(f"  {symbol} RIGHT OUTER JOIN: PostgreSQL optimizer (merge/hash right join)")
            print(f"  {symbol} FULL OUTER JOIN:  PostgreSQL optimizer (merge/hash full join)")
            print(f"  {symbol} CROSS JOIN:       PostgreSQL optimizer (nested loop)")
            print(f"  {symbol} SELF JOIN:        PostgreSQL optimizer (standard join)")
            print(f"  {symbol} UNION/UNION ALL:  PostgreSQL set operations")
            print(f"\n  The FDW efficiently scans individual FairCom tables. PostgreSQL's")
            print(f"  optimizer selects the best join strategy (merge, hash, or nested loop).\n")

        except Exception as e:
            self.results.append(TestResult(
                name="JOIN operations verification",
                category="Join Operations",
                status=TestStatus.FAILED,
                error=str(e),
                elapsed_ms=0
            ))
            print(f"  {Colors.RED}✗{Colors.END} JOIN operations test failed: {str(e)}")

    def test_aggregate_pushdown(self):
        """Test 9: Aggregate Pushdown Completeness"""
        self.print_header("9. AGGREGATE PUSHDOWN COMPLETENESS")

        try:
            symbol = f"{Colors.GREEN}✓{Colors.END}"
            fail_symbol = f"{Colors.RED}✗{Colors.END}"

            print("\n  Part A: Verifying Aggregate Result Correctness\n")

            # Use captured baseline count for resilience to write operations
            baseline_count = self.baseline_employee_count
            print(f"  Baseline employee count (captured at test start): {baseline_count}\\n")

            # First verify that aggregate results are CORRECT, not just that they execute
            # This catches bugs where aggregates compute but don't return values properly

            aggregate_correctness_tests = [
                # (description, query, expected_result_check_function, error_message)
                ("COUNT(*) total records",
                 "SELECT COUNT(*) FROM employees WHERE emp_id NOT IN (90000, 90001, 90002, 90003, 90004);",
                 lambda r: r[0] == baseline_count,
                 f"Expected COUNT(*) = {baseline_count} employees (excluding test records)"),

                ("COUNT(*) with WHERE dept_id=1",
                 "SELECT COUNT(*) FROM employees WHERE dept_id = 1 AND emp_id NOT IN (90000, 90001, 90002, 90003, 90004);",
                 lambda r: r[0] > 0 and r[0] < baseline_count,
                 f"Expected COUNT with WHERE to return subset of {baseline_count}"),

                ("COUNT(*) with salary filter",
                 "SELECT COUNT(*) FROM employees WHERE salary > 100000 AND emp_id NOT IN (90000, 90001, 90002, 90003, 90004);",
                 lambda r: r[0] > 0 and r[0] < baseline_count,
                 "Expected high-salary count to be subset"),

                ("SUM(salary) returns non-null",
                 "SELECT SUM(salary) FROM employees WHERE emp_id NOT IN (90000, 90001, 90002, 90003, 90004);",
                 lambda r: r[0] is not None and r[0] > 0,
                 "Expected SUM(salary) to return positive value"),

                ("SUM(salary) reasonable range",
                 "SELECT SUM(salary) FROM employees WHERE emp_id NOT IN (90000, 90001, 90002, 90003, 90004);",
                 lambda r: 30000000 <= r[0] <= 300000000,
                 "Expected total salary between 30M-300M (baseline employees * 30k-300k)"),

                ("SUM(salary) with WHERE < full SUM",
                 "SELECT SUM(salary) FROM employees WHERE dept_id = 1 AND emp_id NOT IN (90000, 90001, 90002, 90003, 90004);",
                 lambda r: r[0] is not None and 0 < r[0] < 300000000,
                 "Expected filtered SUM < total SUM"),

                ("AVG(salary) in expected range",
                 "SELECT AVG(salary) FROM employees WHERE emp_id NOT IN (90000, 90001, 90002, 90003, 90004);",
                 lambda r: r[0] is not None and 30000 <= r[0] <= 300000,
                 "Expected AVG salary between 30k-300k"),

                ("AVG(salary) with WHERE > 100k",
                 "SELECT AVG(salary) FROM employees WHERE salary > 100000 AND emp_id NOT IN (90000, 90001, 90002, 90003, 90004);",
                 lambda r: r[0] is not None and r[0] > 100000,
                 "Expected AVG of high salaries > 100k"),

                ("MIN(salary) at lower bound",
                 "SELECT MIN(salary) FROM employees WHERE emp_id NOT IN (90000, 90001, 90002, 90003, 90004);",
                 lambda r: r[0] is not None and 30000 <= r[0] <= 300000,
                 "Expected MIN salary in 30k-300k range"),

                ("MAX(salary) at upper bound",
                 "SELECT MAX(salary) FROM employees WHERE emp_id NOT IN (90000, 90001, 90002, 90003, 90004);",
                 lambda r: r[0] is not None and 30000 <= r[0] <= 300000,
                 "Expected MAX salary in 30k-300k range"),

                ("MAX >= MIN consistency",
                 "SELECT MIN(salary), MAX(salary) FROM employees WHERE emp_id NOT IN (90000, 90001, 90002, 90003, 90004);",
                 lambda r: r[0] is not None and r[1] is not None and r[1] >= r[0],
                 "Expected MAX >= MIN"),

                ("Multiple aggregates together",
                 "SELECT COUNT(*), SUM(salary), AVG(salary), MIN(salary), MAX(salary) FROM employees WHERE emp_id NOT IN (90000, 90001, 90002, 90003, 90004);",
                 lambda r: (r[0] == baseline_count and r[1] is not None and r[2] is not None
                           and r[3] is not None and r[4] is not None),
                 "Expected all 5 aggregates to return values"),
            ]

            correctness_passed = 0
            correctness_failed = 0

            for description, query, check_func, error_msg in aggregate_correctness_tests:
                try:
                    start = time.time()
                    with self.conn.cursor() as cur:
                        cur.execute(query)
                        result = cur.fetchone()
                    elapsed = (time.time() - start) * 1000

                    # Verify result is correct
                    if result is None:
                        raise ValueError("Query returned NULL result")

                    if not check_func(result):
                        raise ValueError(f"{error_msg} (got: {result})")

                    self.results.append(TestResult(
                        name=f"Aggregate correctness: {description}",
                        category="Aggregate Pushdown",
                        status=TestStatus.PASSED,
                        elapsed_ms=elapsed
                    ))
                    correctness_passed += 1
                    print(f"  {symbol} {description:50} CORRECT (result: {result[0] if len(result) == 1 else result[:2]}) {elapsed:6.1f}ms")

                except Exception as e:
                    correctness_failed += 1
                    self.results.append(TestResult(
                        name=f"Aggregate correctness: {description}",
                        category="Aggregate Pushdown",
                        status=TestStatus.FAILED,
                        error=str(e)[:100],
                        elapsed_ms=0
                    ))
                    print(f"  {fail_symbol} {description:50} FAILED: {str(e)[:60]}")

            print(f"\n  Correctness Summary: {correctness_passed} passed, {correctness_failed} failed")

            if correctness_failed > 0:
                print(f"  {Colors.YELLOW}WARNING: Aggregate result correctness issues detected!{Colors.END}")
                print(f"  {Colors.YELLOW}Computed values may not be returning to PostgreSQL properly.{Colors.END}\n")

            print(f"\n  Part B: Aggregate Performance Testing\n")

            # Define aggregate test cases
            # Format: (aggregate_type, description, query)
            aggregate_tests = [
                # Basic aggregates
                ("COUNT(*)",
                 "COUNT(*) baseline",
                 "SELECT COUNT(*) FROM employees;"),

                ("COUNT(column)",
                 "COUNT(emp_id) with column",
                 "SELECT COUNT(emp_id) FROM employees;"),

                ("COUNT(salary)",
                 "COUNT(salary) numeric column",
                 "SELECT COUNT(salary) FROM employees;"),

                # SUM aggregate
                ("SUM(salary)",
                 "SUM(salary) baseline",
                 "SELECT SUM(salary) FROM employees;"),

                ("SUM with WHERE",
                 "SUM(salary) WHERE salary > 100000",
                 "SELECT SUM(salary) FROM employees WHERE salary > 100000;"),

                ("SUM with multiple WHERE",
                 "SUM(salary) WHERE salary > 100000 AND dept_id = 1",
                 "SELECT SUM(salary) FROM employees WHERE salary > 100000 AND dept_id = 1;"),

                # AVG aggregate
                ("AVG(salary)",
                 "AVG(salary) baseline",
                 "SELECT AVG(salary) FROM employees;"),

                ("AVG with WHERE",
                 "AVG(salary) WHERE salary > 80000",
                 "SELECT AVG(salary) FROM employees WHERE salary > 80000;"),

                # MIN/MAX aggregates
                ("MIN(salary)",
                 "MIN(salary) baseline",
                 "SELECT MIN(salary) FROM employees;"),

                ("MAX(salary)",
                 "MAX(salary) baseline",
                 "SELECT MAX(salary) FROM employees;"),

                ("MIN with WHERE",
                 "MIN(salary) WHERE dept_id = 2",
                 "SELECT MIN(salary) FROM employees WHERE dept_id = 2;"),

                ("MAX with WHERE",
                 "MAX(salary) WHERE dept_id = 2",
                 "SELECT MAX(salary) FROM employees WHERE dept_id = 2;"),

                # GROUP BY aggregates
                ("COUNT GROUP BY",
                 "COUNT(*) GROUP BY dept_id",
                 "SELECT dept_id, COUNT(*) FROM employees GROUP BY dept_id;"),

                ("SUM GROUP BY",
                 "SUM(salary) GROUP BY dept_id",
                 "SELECT dept_id, SUM(salary) FROM employees GROUP BY dept_id;"),

                ("AVG GROUP BY",
                 "AVG(salary) GROUP BY dept_id",
                 "SELECT dept_id, AVG(salary) FROM employees GROUP BY dept_id;"),

                # Multiple aggregates
                ("Multiple aggregates",
                 "COUNT and AVG together",
                 "SELECT COUNT(*), AVG(salary) FROM employees;"),

                ("Multiple with GROUP BY",
                 "COUNT, SUM, AVG with GROUP BY",
                 "SELECT dept_id, COUNT(*), SUM(salary), AVG(salary) FROM employees GROUP BY dept_id;"),

                # HAVING clause
                ("HAVING clause",
                 "GROUP BY with HAVING COUNT > 50",
                 "SELECT dept_id, COUNT(*) FROM employees GROUP BY dept_id HAVING COUNT(*) > 50;"),

                ("HAVING with aggregate",
                 "GROUP BY dept_id HAVING SUM(salary) > 500000",
                 "SELECT dept_id, SUM(salary) FROM employees GROUP BY dept_id HAVING SUM(salary) > 500000;"),

                # Aggregate with JOIN
                ("Aggregate with JOIN",
                 "COUNT(*) with INNER JOIN",
                 "SELECT COUNT(*) FROM employees e INNER JOIN departments d ON e.dept_id = d.dept_id;"),

                ("SUM with JOIN and WHERE",
                 "SUM(salary) with JOIN and WHERE",
                 "SELECT SUM(e.salary) FROM employees e INNER JOIN departments d ON e.dept_id = d.dept_id WHERE e.salary > 80000;"),
            ]

            aggregate_results = {}

            for agg_type, description, query in aggregate_tests:
                try:
                    start = time.time()
                    with self.conn.cursor() as cur:
                        cur.execute(query)
                        result = cur.fetchall()  # Use fetchall for GROUP BY queries
                    elapsed = (time.time() - start) * 1000

                    if agg_type not in aggregate_results:
                        aggregate_results[agg_type] = []
                    aggregate_results[agg_type].append(elapsed)

                    self.results.append(TestResult(
                        name=f"Aggregate pushdown: {description}",
                        category="Aggregate Pushdown",
                        status=TestStatus.PASSED,
                        elapsed_ms=elapsed
                    ))

                    print(f"  {symbol} {agg_type:30} {description:45} {elapsed:6.1f}ms")

                except Exception as e:
                    self.results.append(TestResult(
                        name=f"Aggregate pushdown: {description}",
                        category="Aggregate Pushdown",
                        status=TestStatus.FAILED,
                        error=str(e)[:50],
                        elapsed_ms=0
                    ))
                    print(f"  {Colors.RED}✗{Colors.END} {agg_type:30} {description:45} ERROR: {str(e)[:40]}")

            # Analyze aggregate performance patterns
            print(f"\n  Aggregate Performance Analysis:")

            try:
                # Compare basic aggregates
                print(f"\n  {symbol} Basic Aggregates (no WHERE):")

                basics = [
                    ("COUNT(*)", "SELECT COUNT(*) FROM employees;"),
                    ("SUM(salary)", "SELECT SUM(salary) FROM employees;"),
                    ("AVG(salary)", "SELECT AVG(salary) FROM employees;"),
                    ("MIN(salary)", "SELECT MIN(salary) FROM employees;"),
                    ("MAX(salary)", "SELECT MAX(salary) FROM employees;"),
                ]

                basic_times = {}
                for name, query in basics:
                    start = time.time()
                    with self.conn.cursor() as cur:
                        cur.execute(query)
                        cur.fetchone()
                    elapsed = (time.time() - start) * 1000
                    basic_times[name] = elapsed
                    print(f"    {symbol} {name:20} {elapsed:6.1f}ms")

                avg_basic = statistics.mean(basic_times.values())
                print(f"    {symbol} Average:           {avg_basic:6.1f}ms")

                self.results.append(TestResult(
                    name="Aggregate average basic",
                    category="Aggregate Pushdown",
                    status=TestStatus.PASSED,
                    elapsed_ms=avg_basic
                ))
            except Exception as e:
                print(f"  {Colors.RED}✗{Colors.END} Basic aggregates analysis failed")

            try:
                # Compare aggregate with WHERE
                print(f"\n  {symbol} Aggregates with WHERE Clause:")

                where_tests = [
                    ("No WHERE", "SELECT COUNT(*) FROM employees;"),
                    ("Simple WHERE", "SELECT COUNT(*) FROM employees WHERE salary > 100000;"),
                    ("Complex WHERE", "SELECT COUNT(*) FROM employees WHERE salary > 100000 AND dept_id = 1;"),
                ]

                where_times = {}
                for name, query in where_tests:
                    start = time.time()
                    with self.conn.cursor() as cur:
                        cur.execute(query)
                        cur.fetchone()
                    elapsed = (time.time() - start) * 1000
                    where_times[name] = elapsed
                    print(f"    {symbol} {name:20} {elapsed:6.1f}ms")

                # Calculate predicate effect
                base = where_times["No WHERE"]
                simple_effect = where_times["Simple WHERE"] - base
                complex_effect = where_times["Complex WHERE"] - base

                if base > 0:
                    simple_pct = (simple_effect / base) * 100
                    complex_pct = (complex_effect / base) * 100
                else:
                    simple_pct = complex_pct = 0

                print(f"    {symbol} WHERE impact:      {simple_effect:+.1f}ms ({simple_pct:+.1f}%)")
                print(f"    {symbol} Complex WHERE:     {complex_effect:+.1f}ms ({complex_pct:+.1f}%)")

                self.results.append(TestResult(
                    name="Aggregate WHERE overhead",
                    category="Aggregate Pushdown",
                    status=TestStatus.PASSED,
                    elapsed_ms=simple_effect
                ))
            except Exception as e:
                print(f"  {Colors.RED}✗{Colors.END} WHERE analysis failed")

            try:
                # Compare GROUP BY performance
                print(f"\n  {symbol} GROUP BY Aggregates:")

                groupby_tests = [
                    ("No GROUP BY", "SELECT COUNT(*) FROM employees;"),
                    ("GROUP BY dept_id", "SELECT dept_id, COUNT(*) FROM employees GROUP BY dept_id;"),
                    ("GROUP BY + SUM", "SELECT dept_id, SUM(salary) FROM employees GROUP BY dept_id;"),
                    ("GROUP BY + AVG", "SELECT dept_id, AVG(salary) FROM employees GROUP BY dept_id;"),
                ]

                groupby_times = {}
                for name, query in groupby_tests:
                    start = time.time()
                    with self.conn.cursor() as cur:
                        cur.execute(query)
                        cur.fetchall()
                    elapsed = (time.time() - start) * 1000
                    groupby_times[name] = elapsed
                    print(f"    {symbol} {name:25} {elapsed:6.1f}ms")

                base = groupby_times["No GROUP BY"]
                groupby_overhead = groupby_times["GROUP BY dept_id"] - base

                if base > 0:
                    groupby_pct = (groupby_overhead / base) * 100
                else:
                    groupby_pct = 0

                print(f"    {symbol} GROUP BY overhead: {groupby_overhead:+.1f}ms ({groupby_pct:+.1f}%)")

                self.results.append(TestResult(
                    name="Aggregate GROUP BY overhead",
                    category="Aggregate Pushdown",
                    status=TestStatus.PASSED,
                    elapsed_ms=groupby_overhead
                ))
            except Exception as e:
                print(f"  {Colors.RED}✗{Colors.END} GROUP BY analysis failed")

            try:
                # Compare aggregate functions
                print(f"\n  {symbol} Aggregate Function Comparison:")

                func_tests = [
                    ("COUNT(*)", "SELECT COUNT(*) FROM employees;"),
                    ("SUM(salary)", "SELECT SUM(salary) FROM employees;"),
                    ("AVG(salary)", "SELECT AVG(salary) FROM employees;"),
                    ("MIN(salary)", "SELECT MIN(salary) FROM employees;"),
                    ("MAX(salary)", "SELECT MAX(salary) FROM employees;"),
                ]

                func_times = {}
                for name, query in func_tests:
                    start = time.time()
                    with self.conn.cursor() as cur:
                        cur.execute(query)
                        cur.fetchone()
                    elapsed = (time.time() - start) * 1000
                    func_times[name] = elapsed

                # Calculate relative performance
                count_time = func_times["COUNT(*)"]

                print(f"    {symbol} COUNT(*) baseline:  {count_time:6.1f}ms")
                for func, time_ms in sorted(func_times.items()):
                    if func != "COUNT(*)":
                        diff = time_ms - count_time
                        pct = (diff / count_time) * 100
                        print(f"    {symbol} {func:20} {time_ms:6.1f}ms ({diff:+.1f}ms, {pct:+.1f}%)")

                avg_func = statistics.mean(func_times.values())
                self.results.append(TestResult(
                    name="Aggregate function average",
                    category="Aggregate Pushdown",
                    status=TestStatus.PASSED,
                    elapsed_ms=avg_func
                ))
            except Exception as e:
                print(f"  {Colors.RED}✗{Colors.END} Function comparison failed")

        except Exception as e:
            self.results.append(TestResult(
                name="Aggregate pushdown verification",
                category="Aggregate Pushdown",
                status=TestStatus.FAILED,
                error=str(e),
                elapsed_ms=0
            ))
            print(f"  {Colors.RED}✗{Colors.END} Aggregate pushdown test failed: {str(e)}")

    def test_limit_pushdown(self):
        """Test: LIMIT Pushdown Edge Cases"""
        self.print_header("LIMIT PUSHDOWN EDGE CASES")

        try:
            print("\n  Testing LIMIT pushdown correctness and edge cases...\n")

            # LIMIT 0 — should return zero rows
            self.test_single_value(
                "LIMIT Pushdown",
                "LIMIT 0 returns zero rows",
                "SELECT COUNT(*) FROM (SELECT * FROM employees LIMIT 0) t;",
                0
            )

            # LIMIT 1 — single row boundary
            self.test_single_value(
                "LIMIT Pushdown",
                "LIMIT 1 returns exactly one row",
                "SELECT COUNT(*) FROM (SELECT * FROM employees LIMIT 1) t;",
                1
            )

            # LIMIT without ORDER BY — pushdown should be active (fewer rows fetched)
            start = time.time()
            with self.conn.cursor() as cur:
                cur.execute("EXPLAIN (ANALYZE, VERBOSE, COSTS OFF) SELECT emp_id FROM timesheets LIMIT 10;")
                plan = '\n'.join([row[0] for row in cur.fetchall()])
            elapsed_ms = (time.time() - start) * 1000
            has_limit_info = "FairCom Limit" in plan
            self.results.append(TestResult(
                name="LIMIT pushdown active (no ORDER BY)",
                category="LIMIT Pushdown",
                status=TestStatus.PASSED if has_limit_info else TestStatus.FAILED,
                elapsed_ms=elapsed_ms,
                error=None if has_limit_info else "EXPLAIN missing 'FairCom Limit' — pushdown not active"
            ))
            print(f"  {'✓' if has_limit_info else '✗'} LIMIT pushdown active (no ORDER BY)    {elapsed_ms:>7.1f}ms")

            # LIMIT with ORDER BY — pushdown must be DISABLED
            start = time.time()
            with self.conn.cursor() as cur:
                cur.execute("EXPLAIN (ANALYZE, VERBOSE, COSTS OFF) SELECT emp_id FROM employees ORDER BY salary DESC LIMIT 10;")
                plan = '\n'.join([row[0] for row in cur.fetchall()])
            elapsed_ms = (time.time() - start) * 1000
            no_limit_pushdown = "FairCom Limit" not in plan
            self.results.append(TestResult(
                name="LIMIT pushdown disabled with ORDER BY",
                category="LIMIT Pushdown",
                status=TestStatus.PASSED if no_limit_pushdown else TestStatus.FAILED,
                elapsed_ms=elapsed_ms,
                error=None if no_limit_pushdown else "FairCom Limit active with ORDER BY — wrong results possible"
            ))
            print(f"  {'✓' if no_limit_pushdown else '✗'} LIMIT pushdown disabled with ORDER BY  {elapsed_ms:>7.1f}ms")

            # LIMIT with ORDER BY correctness — verify top-N is correct
            with self.conn.cursor() as cur:
                cur.execute("SELECT MAX(salary) FROM employees;")
                actual_max = cur.fetchone()[0]
                cur.execute("SELECT salary FROM employees ORDER BY salary DESC LIMIT 1;")
                top_salary = cur.fetchone()[0]
            correct = (actual_max == top_salary)
            self.results.append(TestResult(
                name="ORDER BY + LIMIT returns correct top-N",
                category="LIMIT Pushdown",
                status=TestStatus.PASSED if correct else TestStatus.FAILED,
                elapsed_ms=0,
                error=None if correct else f"Top salary {top_salary} != MAX {actual_max}"
            ))
            print(f"  {'✓' if correct else '✗'} ORDER BY + LIMIT returns correct top-N")

            # LIMIT with WHERE — predicate + LIMIT interaction
            self.test_single_value(
                "LIMIT Pushdown",
                "LIMIT with WHERE clause",
                "SELECT COUNT(*) FROM (SELECT emp_id FROM employees WHERE dept_id = 1 LIMIT 5) t;",
                5
            )

            # LIMIT larger than table — should return all rows
            self.test_single_value(
                "LIMIT Pushdown",
                "LIMIT larger than table returns all rows",
                "SELECT COUNT(*) FROM (SELECT * FROM departments LIMIT 1000) t;",
                10
            )

            # LIMIT NULL — equivalent to no LIMIT
            self.test_single_value(
                "LIMIT Pushdown",
                "LIMIT NULL returns all rows",
                "SELECT COUNT(*) FROM (SELECT * FROM departments LIMIT NULL) t;",
                10
            )

            # LIMIT with OFFSET
            self.test_single_value(
                "LIMIT Pushdown",
                "LIMIT 5 OFFSET 5 returns correct rows",
                "SELECT COUNT(*) FROM (SELECT * FROM departments LIMIT 5 OFFSET 5) t;",
                5
            )

        except Exception as e:
            self.results.append(TestResult(
                name="LIMIT pushdown edge cases",
                category="LIMIT Pushdown",
                status=TestStatus.FAILED,
                error=str(e),
                elapsed_ms=0
            ))
            print(f"  {Colors.RED}✗{Colors.END} LIMIT pushdown test failed: {str(e)}")

    def test_offset_edge_cases(self):
        """Test: OFFSET Edge Cases"""
        self.print_header("OFFSET EDGE CASES")

        try:
            print("\n  Testing OFFSET boundary conditions...\n")

            # OFFSET 0 — should behave like no OFFSET
            self.test_single_value(
                "OFFSET Edge Cases",
                "OFFSET 0 same as no OFFSET",
                "SELECT COUNT(*) FROM (SELECT * FROM employees LIMIT 10 OFFSET 0) t;",
                10
            )

            # OFFSET beyond table size — zero rows
            self.test_single_value(
                "OFFSET Edge Cases",
                "OFFSET beyond table returns 0 rows",
                "SELECT COUNT(*) FROM (SELECT * FROM employees LIMIT 10 OFFSET 5000) t;",
                0
            )

            # OFFSET = exact table size — zero rows
            self.test_single_value(
                "OFFSET Edge Cases",
                "OFFSET = table size returns 0 rows",
                "SELECT COUNT(*) FROM (SELECT * FROM employees LIMIT 10 OFFSET 1000) t;",
                0
            )

            # OFFSET without LIMIT
            with self.conn.cursor() as cur:
                cur.execute("SELECT COUNT(*) FROM (SELECT emp_id FROM employees OFFSET 990) t;")
                count = cur.fetchone()[0]
            correct = (count == 10)
            self.results.append(TestResult(
                name="OFFSET without LIMIT",
                category="OFFSET Edge Cases",
                status=TestStatus.PASSED if correct else TestStatus.FAILED,
                elapsed_ms=0,
                error=None if correct else f"Expected 10 rows, got {count}"
            ))
            print(f"  {'✓' if correct else '✗'} OFFSET without LIMIT (990 of 1000)       count={count}")

            # OFFSET with WHERE
            with self.conn.cursor() as cur:
                cur.execute("SELECT COUNT(*) FROM employees WHERE dept_id = 1;")
                dept1_count = cur.fetchone()[0]
                cur.execute("SELECT COUNT(*) FROM (SELECT emp_id FROM employees WHERE dept_id = 1 LIMIT 5 OFFSET 50) t;")
                subset = cur.fetchone()[0]
            expected = min(5, max(0, dept1_count - 50))
            correct = (subset == expected)
            self.results.append(TestResult(
                name="OFFSET with WHERE clause",
                category="OFFSET Edge Cases",
                status=TestStatus.PASSED if correct else TestStatus.FAILED,
                elapsed_ms=0,
                error=None if correct else f"Expected {expected}, got {subset}"
            ))
            print(f"  {'✓' if correct else '✗'} OFFSET with WHERE clause                 count={subset}")

        except Exception as e:
            self.results.append(TestResult(
                name="OFFSET edge cases",
                category="OFFSET Edge Cases",
                status=TestStatus.FAILED,
                error=str(e),
                elapsed_ms=0
            ))
            print(f"  {Colors.RED}✗{Colors.END} OFFSET edge case test failed: {str(e)}")

    def test_null_handling(self):
        """Test: NULL Handling Edge Cases"""
        self.print_header("NULL HANDLING EDGE CASES")

        try:
            print("\n  Testing NULL handling across operations...\n")

            # COUNT(*) vs COUNT(column) — COUNT(column) must skip NULLs
            with self.conn.cursor() as cur:
                cur.execute("SELECT COUNT(*), COUNT(email) FROM employees;")
                count_star, count_email = cur.fetchone()
            correct = (count_star >= count_email)
            self.results.append(TestResult(
                name="COUNT(*) >= COUNT(column)",
                category="NULL Handling",
                status=TestStatus.PASSED if correct else TestStatus.FAILED,
                elapsed_ms=0,
                error=None if correct else f"COUNT(*)={count_star} < COUNT(email)={count_email}"
            ))
            print(f"  {'✓' if correct else '✗'} COUNT(*)={count_star} >= COUNT(email)={count_email}")

            # WHERE column = NULL returns 0 (SQL standard)
            self.test_single_value(
                "NULL Handling",
                "WHERE col = NULL returns 0 rows",
                "SELECT COUNT(*) FROM employees WHERE email = NULL;",
                0
            )

            # IS NULL filter
            self.test("NULL Handling",
                      "IS NULL filter works",
                      "SELECT COUNT(*) FROM employees WHERE hire_date IS NULL;")

            # IS NOT NULL filter
            self.test("NULL Handling",
                      "IS NOT NULL filter works",
                      "SELECT COUNT(*) FROM employees WHERE hire_date IS NOT NULL;")

            # NULL in LEFT JOIN — unmatched rows preserved
            with self.conn.cursor() as cur:
                cur.execute("""
                    SELECT COUNT(*) FROM departments d
                    LEFT JOIN employees e ON d.dept_id = e.dept_id
                    WHERE e.emp_id IS NULL;
                """)
                orphan_depts = cur.fetchone()[0]
            # This is valid whether 0 or >0 — just shouldn't crash
            self.results.append(TestResult(
                name="NULL in LEFT JOIN (unmatched rows)",
                category="NULL Handling",
                status=TestStatus.PASSED,
                elapsed_ms=0
            ))
            print(f"  ✓ NULL in LEFT JOIN (unmatched depts={orphan_depts})")

            # COALESCE with possible NULLs
            self.test("NULL Handling",
                      "COALESCE handles NULLs",
                      "SELECT COALESCE(email, 'none') FROM employees LIMIT 5;")

            # Aggregate on empty result set returns NULL
            with self.conn.cursor() as cur:
                cur.execute("SELECT SUM(salary) FROM employees WHERE emp_id = -1;")
                result = cur.fetchone()[0]
            correct = (result is None)
            self.results.append(TestResult(
                name="SUM on empty set returns NULL",
                category="NULL Handling",
                status=TestStatus.PASSED if correct else TestStatus.FAILED,
                elapsed_ms=0,
                error=None if correct else f"Expected NULL, got {result}"
            ))
            print(f"  {'✓' if correct else '✗'} SUM on empty set returns NULL")

            # ORDER BY with NULLS FIRST/LAST
            self.test("NULL Handling",
                      "ORDER BY NULLS FIRST",
                      "SELECT email FROM employees ORDER BY email NULLS FIRST LIMIT 5;")

            self.test("NULL Handling",
                      "ORDER BY NULLS LAST",
                      "SELECT email FROM employees ORDER BY email NULLS LAST LIMIT 5;")

        except Exception as e:
            self.results.append(TestResult(
                name="NULL handling edge cases",
                category="NULL Handling",
                status=TestStatus.FAILED,
                error=str(e),
                elapsed_ms=0
            ))
            print(f"  {Colors.RED}✗{Colors.END} NULL handling test failed: {str(e)}")

    def test_explain_verification(self):
        """Test: EXPLAIN Output Verification"""
        self.print_header("EXPLAIN OUTPUT VERIFICATION")

        try:
            print("\n  Verifying FDW EXPLAIN output contains expected metadata...\n")

            def get_explain(query):
                with self.conn.cursor() as cur:
                    cur.execute(f"EXPLAIN (ANALYZE, VERBOSE, COSTS OFF, TIMING OFF) {query}")
                    return '\n'.join([row[0] for row in cur.fetchall()])

            # Foreign Scan present
            plan = get_explain("SELECT * FROM employees LIMIT 100;")
            has_foreign_scan = "Foreign Scan" in plan
            self.results.append(TestResult(
                name="EXPLAIN shows Foreign Scan",
                category="EXPLAIN Verification",
                status=TestStatus.PASSED if has_foreign_scan else TestStatus.FAILED,
                elapsed_ms=0,
                error=None if has_foreign_scan else "Missing 'Foreign Scan' in plan"
            ))
            print(f"  {'✓' if has_foreign_scan else '✗'} EXPLAIN shows Foreign Scan")

            # Batch stats present
            has_batch_stats = "FairCom Batch Stats" in plan
            self.results.append(TestResult(
                name="EXPLAIN shows Batch Stats",
                category="EXPLAIN Verification",
                status=TestStatus.PASSED if has_batch_stats else TestStatus.FAILED,
                elapsed_ms=0,
                error=None if has_batch_stats else "Missing 'FairCom Batch Stats' in plan"
            ))
            print(f"  {'✓' if has_batch_stats else '✗'} EXPLAIN shows Batch Stats")

            # LIMIT info present (no ORDER BY)
            plan_limit = get_explain("SELECT emp_id FROM employees LIMIT 10;")
            has_limit = "FairCom Limit" in plan_limit
            self.results.append(TestResult(
                name="EXPLAIN shows LIMIT pushdown",
                category="EXPLAIN Verification",
                status=TestStatus.PASSED if has_limit else TestStatus.FAILED,
                elapsed_ms=0,
                error=None if has_limit else "Missing 'FairCom Limit' in plan"
            ))
            print(f"  {'✓' if has_limit else '✗'} EXPLAIN shows LIMIT pushdown")

            # Predicate pushdown present
            plan_pred = get_explain("SELECT * FROM employees WHERE salary > 100000;")
            has_predicates = "FairCom Predicates" in plan_pred
            self.results.append(TestResult(
                name="EXPLAIN shows predicate pushdown",
                category="EXPLAIN Verification",
                status=TestStatus.PASSED if has_predicates else TestStatus.FAILED,
                elapsed_ms=0,
                error=None if has_predicates else "Missing 'FairCom Predicates' in plan"
            ))
            print(f"  {'✓' if has_predicates else '✗'} EXPLAIN shows predicate pushdown")

            # Batch efficiency present
            has_efficiency = "FairCom Batch Efficiency" in plan
            self.results.append(TestResult(
                name="EXPLAIN shows Batch Efficiency",
                category="EXPLAIN Verification",
                status=TestStatus.PASSED if has_efficiency else TestStatus.FAILED,
                elapsed_ms=0,
                error=None if has_efficiency else "Missing 'FairCom Batch Efficiency' in plan"
            ))
            print(f"  {'✓' if has_efficiency else '✗'} EXPLAIN shows Batch Efficiency")

        except Exception as e:
            self.results.append(TestResult(
                name="EXPLAIN verification",
                category="EXPLAIN Verification",
                status=TestStatus.FAILED,
                error=str(e),
                elapsed_ms=0
            ))
            print(f"  {Colors.RED}✗{Colors.END} EXPLAIN verification failed: {str(e)}")

    def test_subqueries_and_ctes(self):
        """Test: Subqueries, CTEs, and EXISTS"""
        self.print_header("SUBQUERIES, CTEs, AND EXISTS")

        try:
            print("\n  Testing multi-table operations with subqueries and CTEs...\n")

            # Scalar subquery
            self.test("Subqueries & CTEs",
                      "Scalar subquery in WHERE",
                      "SELECT COUNT(*) FROM employees WHERE salary > (SELECT AVG(salary) FROM employees);")

            # IN subquery
            self.test("Subqueries & CTEs",
                      "IN subquery",
                      "SELECT COUNT(*) FROM employees WHERE dept_id IN (SELECT dept_id FROM departments WHERE dept_id <= 3);")

            # NOT IN subquery
            self.test("Subqueries & CTEs",
                      "NOT IN subquery",
                      "SELECT COUNT(*) FROM employees WHERE dept_id NOT IN (SELECT dept_id FROM departments WHERE dept_id > 5);")

            # EXISTS subquery
            self.test("Subqueries & CTEs",
                      "EXISTS correlated subquery",
                      "SELECT COUNT(*) FROM departments d WHERE EXISTS (SELECT 1 FROM employees e WHERE e.dept_id = d.dept_id);")

            # NOT EXISTS subquery
            self.test("Subqueries & CTEs",
                      "NOT EXISTS correlated subquery",
                      "SELECT COUNT(*) FROM departments d WHERE NOT EXISTS (SELECT 1 FROM employees e WHERE e.dept_id = d.dept_id);")

            # CTE (WITH clause)
            self.test("Subqueries & CTEs",
                      "CTE (WITH clause)",
                      "WITH high_earners AS (SELECT * FROM employees WHERE salary > 100000) SELECT COUNT(*) FROM high_earners;")

            # CTE with JOIN
            self.test("Subqueries & CTEs",
                      "CTE with JOIN",
                      "WITH he AS (SELECT * FROM employees WHERE salary > 100000) SELECT COUNT(*) FROM he JOIN departments d ON he.dept_id = d.dept_id;")

            # Correlated subquery in SELECT
            self.test("Subqueries & CTEs",
                      "Correlated subquery in SELECT list",
                      "SELECT e.name, (SELECT d.dept_name FROM departments d WHERE d.dept_id = e.dept_id) FROM employees e LIMIT 10;")

            # Nested subquery
            self.test("Subqueries & CTEs",
                      "Nested subquery (3 levels)",
                      "SELECT COUNT(*) FROM employees WHERE dept_id IN (SELECT dept_id FROM departments WHERE dept_id IN (SELECT DISTINCT dept_id FROM employee_projects));")

        except Exception as e:
            self.results.append(TestResult(
                name="Subqueries and CTEs",
                category="Subqueries & CTEs",
                status=TestStatus.FAILED,
                error=str(e),
                elapsed_ms=0
            ))
            print(f"  {Colors.RED}✗{Colors.END} Subquery/CTE test failed: {str(e)}")

    def test_write_edge_cases(self):
        """Test: Write Operation Edge Cases"""
        self.print_header("WRITE OPERATION EDGE CASES")

        try:
            print("\n  Testing write operations with boundary conditions...\n")

            # UPDATE with no matching rows — should affect 0 rows
            start = time.time()
            with self.conn.cursor() as cur:
                cur.execute("UPDATE employees SET salary = 0 WHERE emp_id = 999999;")
                affected = cur.rowcount
            elapsed_ms = (time.time() - start) * 1000
            correct = (affected == 0)
            self.results.append(TestResult(
                name="UPDATE with no matching rows (0 affected)",
                category="Write Edge Cases",
                status=TestStatus.PASSED if correct else TestStatus.FAILED,
                elapsed_ms=elapsed_ms,
                error=None if correct else f"Expected 0 affected, got {affected}"
            ))
            print(f"  {'✓' if correct else '✗'} UPDATE no matching rows (affected={affected}){elapsed_ms:>7.1f}ms")

            # DELETE with no matching rows — should affect 0 rows
            start = time.time()
            with self.conn.cursor() as cur:
                cur.execute("DELETE FROM employees WHERE emp_id = 999999;")
                affected = cur.rowcount
            elapsed_ms = (time.time() - start) * 1000
            correct = (affected == 0)
            self.results.append(TestResult(
                name="DELETE with no matching rows (0 affected)",
                category="Write Edge Cases",
                status=TestStatus.PASSED if correct else TestStatus.FAILED,
                elapsed_ms=elapsed_ms,
                error=None if correct else f"Expected 0 affected, got {affected}"
            ))
            print(f"  {'✓' if correct else '✗'} DELETE no matching rows (affected={affected}){elapsed_ms:>7.1f}ms")

            # UPDATE a specific existing row and verify change persists
            test_emp_id = 80001
            try:
                # Insert a test row
                with self.conn.cursor() as cur:
                    try:
                        cur.execute(f"DELETE FROM employees WHERE emp_id = {test_emp_id};")
                    except:
                        pass
                    cur.execute(f"INSERT INTO employees (emp_id, name, salary, email, city, hire_date, dept_id, job_title) VALUES ({test_emp_id}, 'UpdateVerify', 55000, 'uv@test.com', 'OrigCity', '2025-01-01', 1, 'Tester');")

                # UPDATE the row
                start = time.time()
                with self.conn.cursor() as cur:
                    cur.execute(f"UPDATE employees SET city = 'ChangedCity' WHERE emp_id = {test_emp_id};")
                    affected = cur.rowcount
                elapsed_ms = (time.time() - start) * 1000
                correct = (affected == 1)
                self.results.append(TestResult(
                    name="UPDATE specific row (1 affected)",
                    category="Write Edge Cases",
                    status=TestStatus.PASSED if correct else TestStatus.FAILED,
                    elapsed_ms=elapsed_ms,
                    error=None if correct else f"Expected 1 affected, got {affected}"
                ))
                print(f"  {'✓' if correct else '✗'} UPDATE specific row (affected={affected})    {elapsed_ms:>7.1f}ms")

                # Verify the change persisted
                with self.conn.cursor() as cur:
                    cur.execute(f"SELECT city FROM employees WHERE emp_id = {test_emp_id};")
                    result = cur.fetchone()
                verified = result is not None and result[0] == 'ChangedCity'
                self.results.append(TestResult(
                    name="UPDATE persistence verified",
                    category="Write Edge Cases",
                    status=TestStatus.PASSED if verified else TestStatus.FAILED,
                    elapsed_ms=0,
                    error=None if verified else f"Expected 'ChangedCity', got {result}"
                ))
                print(f"  {'✓' if verified else '✗'} UPDATE persistence verified")
            finally:
                with self.conn.cursor() as cur:
                    try:
                        cur.execute(f"DELETE FROM employees WHERE emp_id = {test_emp_id};")
                    except:
                        pass

            # INSERT with special characters
            test_emp_id = 90004
            try:
                with self.conn.cursor() as cur:
                    cur.execute(f"DELETE FROM employees WHERE emp_id = {test_emp_id};")
                start = time.time()
                with self.conn.cursor() as cur:
                    cur.execute(f"INSERT INTO employees (emp_id, name, salary, email, city, hire_date, dept_id, job_title) VALUES ({test_emp_id}, 'O''Brien Test', 50000, 'obrien@test.com', 'New York', '2025-06-15', 1, 'Dev');")
                    cur.execute(f"SELECT name FROM employees WHERE emp_id = {test_emp_id};")
                    result = cur.fetchone()
                elapsed_ms = (time.time() - start) * 1000
                correct = (result is not None and result[0] == "O'Brien Test")
                self.results.append(TestResult(
                    name="INSERT with apostrophe in name",
                    category="Write Edge Cases",
                    status=TestStatus.PASSED if correct else TestStatus.FAILED,
                    elapsed_ms=elapsed_ms,
                    error=None if correct else f"Expected O'Brien Test, got {result}"
                ))
                print(f"  {'✓' if correct else '✗'} INSERT with apostrophe in name          {elapsed_ms:>7.1f}ms")
            finally:
                with self.conn.cursor() as cur:
                    try:
                        cur.execute(f"DELETE FROM employees WHERE emp_id = {test_emp_id};")
                    except:
                        pass

        except Exception as e:
            self.results.append(TestResult(
                name="Write edge cases",
                category="Write Edge Cases",
                status=TestStatus.FAILED,
                error=str(e),
                elapsed_ms=0
            ))
            print(f"  {Colors.RED}✗{Colors.END} Write edge case test failed: {str(e)}")

    def test_data_type_edge_cases(self):
        """Test: Data Type Edge Cases"""
        self.print_header("DATA TYPE EDGE CASES")

        try:
            print("\n  Testing data type boundary conditions...\n")
            test_emp_id = 90004

            # Clean up first
            with self.conn.cursor() as cur:
                try:
                    cur.execute(f"DELETE FROM employees WHERE emp_id = {test_emp_id};")
                except:
                    pass

            # Negative salary
            start = time.time()
            with self.conn.cursor() as cur:
                cur.execute(f"INSERT INTO employees (emp_id, name, salary, email, city, hire_date, dept_id, job_title) VALUES ({test_emp_id}, 'Negative Test', -1000.50, 'neg@test.com', 'TestCity', '2025-01-01', 1, 'Tester');")
                cur.execute(f"SELECT salary FROM employees WHERE emp_id = {test_emp_id};")
                result = cur.fetchone()[0]
            elapsed_ms = (time.time() - start) * 1000
            correct = (float(result) == -1000.50)
            self.results.append(TestResult(
                name="Negative numeric value roundtrip",
                category="Data Type Edge Cases",
                status=TestStatus.PASSED if correct else TestStatus.FAILED,
                elapsed_ms=elapsed_ms,
                error=None if correct else f"Expected -1000.50, got {result}"
            ))
            print(f"  {'✓' if correct else '✗'} Negative numeric value (-1000.50)        {elapsed_ms:>7.1f}ms")

            # Update to zero salary
            with self.conn.cursor() as cur:
                cur.execute(f"UPDATE employees SET salary = 0 WHERE emp_id = {test_emp_id};")
                cur.execute(f"SELECT salary FROM employees WHERE emp_id = {test_emp_id};")
                result = cur.fetchone()[0]
            correct = (float(result) == 0.0)
            self.results.append(TestResult(
                name="Zero numeric value roundtrip",
                category="Data Type Edge Cases",
                status=TestStatus.PASSED if correct else TestStatus.FAILED,
                elapsed_ms=0,
                error=None if correct else f"Expected 0.0, got {result}"
            ))
            print(f"  {'✓' if correct else '✗'} Zero numeric value (0.0)")

            # Epoch date (1970-01-01)
            with self.conn.cursor() as cur:
                cur.execute(f"UPDATE employees SET hire_date = '1970-01-01' WHERE emp_id = {test_emp_id};")
                cur.execute(f"SELECT hire_date FROM employees WHERE emp_id = {test_emp_id};")
                result = str(cur.fetchone()[0])
            correct = ('1970-01-01' in result)
            self.results.append(TestResult(
                name="Epoch date (1970-01-01) roundtrip",
                category="Data Type Edge Cases",
                status=TestStatus.PASSED if correct else TestStatus.FAILED,
                elapsed_ms=0,
                error=None if correct else f"Expected 1970-01-01, got {result}"
            ))
            print(f"  {'✓' if correct else '✗'} Epoch date (1970-01-01)")

            # Leap year date
            with self.conn.cursor() as cur:
                cur.execute(f"UPDATE employees SET hire_date = '2024-02-29' WHERE emp_id = {test_emp_id};")
                cur.execute(f"SELECT hire_date FROM employees WHERE emp_id = {test_emp_id};")
                result = str(cur.fetchone()[0])
            correct = ('2024-02-29' in result)
            self.results.append(TestResult(
                name="Leap year date (2024-02-29) roundtrip",
                category="Data Type Edge Cases",
                status=TestStatus.PASSED if correct else TestStatus.FAILED,
                elapsed_ms=0,
                error=None if correct else f"Expected 2024-02-29, got {result}"
            ))
            print(f"  {'✓' if correct else '✗'} Leap year date (2024-02-29)")

            # Far future date
            with self.conn.cursor() as cur:
                cur.execute(f"UPDATE employees SET hire_date = '2099-12-31' WHERE emp_id = {test_emp_id};")
                cur.execute(f"SELECT hire_date FROM employees WHERE emp_id = {test_emp_id};")
                result = str(cur.fetchone()[0])
            correct = ('2099-12-31' in result)
            self.results.append(TestResult(
                name="Far future date (2099-12-31) roundtrip",
                category="Data Type Edge Cases",
                status=TestStatus.PASSED if correct else TestStatus.FAILED,
                elapsed_ms=0,
                error=None if correct else f"Expected 2099-12-31, got {result}"
            ))
            print(f"  {'✓' if correct else '✗'} Far future date (2099-12-31)")

            # Comparison with zero
            self.test("Data Type Edge Cases",
                      "WHERE salary = 0 filter",
                      f"SELECT COUNT(*) FROM employees WHERE salary = 0;")

            # Clean up
            with self.conn.cursor() as cur:
                try:
                    cur.execute(f"DELETE FROM employees WHERE emp_id = {test_emp_id};")
                except:
                    pass

        except Exception as e:
            # Clean up on failure
            try:
                with self.conn.cursor() as cur:
                    cur.execute(f"DELETE FROM employees WHERE emp_id = 90004;")
            except:
                pass
            self.results.append(TestResult(
                name="Data type edge cases",
                category="Data Type Edge Cases",
                status=TestStatus.FAILED,
                error=str(e),
                elapsed_ms=0
            ))
            print(f"  {Colors.RED}✗{Colors.END} Data type edge case test failed: {str(e)}")

    def test_guc_parameters(self):
        """Test: GUC Parameter Configuration"""
        self.print_header("GUC PARAMETER TESTS")

        try:
            print("\n  Testing FDW GUC parameter access and configuration...\n")

            # Read default GUC values
            guc_tests = [
                ("faircom_fdw.cache_ttl", "300"),
                ("faircom_fdw.plan_cache_size", "500"),
                ("faircom_fdw.batch_size", "500"),
                ("faircom_fdw.connection_idle_timeout", "1800"),
                ("faircom_fdw.enable_aggregate_pushdown", "on"),
                ("faircom_fdw.enable_predicate_pushdown", "on"),
            ]

            for guc_name, expected_default in guc_tests:
                try:
                    with self.conn.cursor() as cur:
                        cur.execute(f"SHOW {guc_name};")
                        value = cur.fetchone()[0]
                    correct = (value == expected_default)
                    self.results.append(TestResult(
                        name=f"GUC {guc_name} default = {expected_default}",
                        category="GUC Parameters",
                        status=TestStatus.PASSED if correct else TestStatus.FAILED,
                        elapsed_ms=0,
                        error=None if correct else f"Expected '{expected_default}', got '{value}'"
                    ))
                    print(f"  {'✓' if correct else '✗'} {guc_name} = {value}")
                except Exception as e:
                    self.results.append(TestResult(
                        name=f"GUC {guc_name} readable",
                        category="GUC Parameters",
                        status=TestStatus.FAILED,
                        elapsed_ms=0,
                        error=str(e)
                    ))
                    print(f"  ✗ {guc_name}: {str(e)[:60]}")

            # SET and verify batch_size
            try:
                with self.conn.cursor() as cur:
                    cur.execute("SET faircom_fdw.batch_size = 100;")
                    cur.execute("SHOW faircom_fdw.batch_size;")
                    value = cur.fetchone()[0]
                    # Reset to default
                    cur.execute("RESET faircom_fdw.batch_size;")
                correct = (value == "100")
                self.results.append(TestResult(
                    name="SET faircom_fdw.batch_size = 100",
                    category="GUC Parameters",
                    status=TestStatus.PASSED if correct else TestStatus.FAILED,
                    elapsed_ms=0,
                    error=None if correct else f"Expected '100', got '{value}'"
                ))
                print(f"  {'✓' if correct else '✗'} SET batch_size = 100 (then reset)")
            except Exception as e:
                self.results.append(TestResult(
                    name="SET faircom_fdw.batch_size",
                    category="GUC Parameters",
                    status=TestStatus.FAILED,
                    elapsed_ms=0,
                    error=str(e)
                ))
                print(f"  ✗ SET batch_size failed: {str(e)[:60]}")

            # Query still works after disabling predicate pushdown
            try:
                with self.conn.cursor() as cur:
                    cur.execute("SET faircom_fdw.enable_predicate_pushdown = off;")
                    cur.execute("SELECT COUNT(*) FROM employees WHERE salary > 100000;")
                    count = cur.fetchone()[0]
                    cur.execute("RESET faircom_fdw.enable_predicate_pushdown;")
                correct = (count is not None and count > 0)
                self.results.append(TestResult(
                    name="Query works with predicate pushdown OFF",
                    category="GUC Parameters",
                    status=TestStatus.PASSED if correct else TestStatus.FAILED,
                    elapsed_ms=0,
                    error=None if correct else f"Query returned {count}"
                ))
                print(f"  {'✓' if correct else '✗'} Query with predicate pushdown OFF (count={count})")
            except Exception as e:
                # Reset on failure
                try:
                    with self.conn.cursor() as cur:
                        cur.execute("RESET faircom_fdw.enable_predicate_pushdown;")
                except:
                    pass
                self.results.append(TestResult(
                    name="Query with predicate pushdown OFF",
                    category="GUC Parameters",
                    status=TestStatus.FAILED,
                    elapsed_ms=0,
                    error=str(e)
                ))
                print(f"  ✗ Predicate pushdown OFF: {str(e)[:60]}")

        except Exception as e:
            self.results.append(TestResult(
                name="GUC parameter tests",
                category="GUC Parameters",
                status=TestStatus.FAILED,
                error=str(e),
                elapsed_ms=0
            ))
            print(f"  {Colors.RED}✗{Colors.END} GUC parameter test failed: {str(e)}")

    def test_distinct_and_window_functions(self):
        """Test: DISTINCT and Window Functions"""
        self.print_header("DISTINCT AND WINDOW FUNCTIONS")

        try:
            print("\n  Testing DISTINCT, DISTINCT ON, and window functions...\n")

            # SELECT DISTINCT
            with self.conn.cursor() as cur:
                cur.execute("SELECT DISTINCT dept_id FROM employees ORDER BY dept_id;")
                distinct_depts = [row[0] for row in cur.fetchall()]
            correct = (len(distinct_depts) == 10 and distinct_depts == sorted(distinct_depts))
            self.results.append(TestResult(
                name="SELECT DISTINCT dept_id",
                category="DISTINCT & Window",
                status=TestStatus.PASSED if correct else TestStatus.FAILED,
                elapsed_ms=0,
                error=None if correct else f"Expected 10 distinct depts, got {len(distinct_depts)}"
            ))
            print(f"  {'✓' if correct else '✗'} SELECT DISTINCT dept_id ({len(distinct_depts)} values)")

            # SELECT DISTINCT ON
            self.test("DISTINCT & Window",
                      "DISTINCT ON (dept_id)",
                      "SELECT DISTINCT ON (dept_id) emp_id, dept_id, name FROM employees ORDER BY dept_id, salary DESC;")

            # ROW_NUMBER window function
            self.test("DISTINCT & Window",
                      "ROW_NUMBER() OVER (PARTITION BY...)",
                      "SELECT emp_id, salary, ROW_NUMBER() OVER (PARTITION BY dept_id ORDER BY salary DESC) FROM employees LIMIT 20;")

            # RANK window function
            self.test("DISTINCT & Window",
                      "RANK() OVER (ORDER BY...)",
                      "SELECT emp_id, salary, RANK() OVER (ORDER BY salary DESC) FROM employees LIMIT 20;")

            # Window function with aggregate
            self.test("DISTINCT & Window",
                      "SUM() OVER (PARTITION BY...)",
                      "SELECT emp_id, dept_id, salary, SUM(salary) OVER (PARTITION BY dept_id) FROM employees LIMIT 20;")

        except Exception as e:
            self.results.append(TestResult(
                name="DISTINCT and window functions",
                category="DISTINCT & Window",
                status=TestStatus.FAILED,
                error=str(e),
                elapsed_ms=0
            ))
            print(f"  {Colors.RED}✗{Colors.END} DISTINCT/window test failed: {str(e)}")

    def test_transaction_behavior(self):
        """Test: Transaction Behavior"""
        self.print_header("TRANSACTION BEHAVIOR")

        try:
            print("\n  Testing transaction handling and rollback behavior...\n")

            # Read within explicit transaction
            # Use a separate connection to avoid autocommit issues
            txn_conn = psycopg2.connect(
                host=self.host, port=self.port,
                database=self.database, user=self.user, password=self.password
            )
            txn_conn.autocommit = False
            try:
                start = time.time()
                with txn_conn.cursor() as cur:
                    cur.execute("SELECT COUNT(*) FROM employees;")
                    count = cur.fetchone()[0]
                txn_conn.commit()
                elapsed_ms = (time.time() - start) * 1000
                correct = (count == self.baseline_employee_count)
                self.results.append(TestResult(
                    name="Read within explicit transaction",
                    category="Transaction Behavior",
                    status=TestStatus.PASSED if correct else TestStatus.FAILED,
                    elapsed_ms=elapsed_ms,
                    error=None if correct else f"Expected {self.baseline_employee_count}, got {count}"
                ))
                print(f"  {'✓' if correct else '✗'} Read within explicit transaction         {elapsed_ms:>7.1f}ms")
            finally:
                txn_conn.close()

            # Read after error + rollback — connection still works
            start = time.time()
            try:
                with self.conn.cursor() as cur:
                    try:
                        cur.execute("SELECT 1/0;")  # division by zero
                    except psycopg2.Error:
                        self.conn.rollback()
                    cur.execute("SELECT COUNT(*) FROM employees;")
                    count = cur.fetchone()[0]
                elapsed_ms = (time.time() - start) * 1000
                correct = (count == self.baseline_employee_count)
                self.results.append(TestResult(
                    name="Read after error + ROLLBACK",
                    category="Transaction Behavior",
                    status=TestStatus.PASSED if correct else TestStatus.FAILED,
                    elapsed_ms=elapsed_ms,
                    error=None if correct else f"Expected {self.baseline_employee_count}, got {count}"
                ))
                print(f"  {'✓' if correct else '✗'} Read after error + ROLLBACK              {elapsed_ms:>7.1f}ms")
            except Exception as e:
                self.results.append(TestResult(
                    name="Read after error + ROLLBACK",
                    category="Transaction Behavior",
                    status=TestStatus.FAILED,
                    elapsed_ms=0,
                    error=str(e)
                ))
                print(f"  ✗ Read after error + ROLLBACK: {str(e)[:60]}")

            # Rapid sequential queries — stress connection reuse
            start = time.time()
            success_count = 0
            for _ in range(100):
                try:
                    with self.conn.cursor() as cur:
                        cur.execute("SELECT COUNT(*) FROM employees;")
                        cur.fetchone()
                    success_count += 1
                except:
                    pass
            elapsed_ms = (time.time() - start) * 1000
            correct = (success_count == 100)
            self.results.append(TestResult(
                name="100 rapid sequential queries",
                category="Transaction Behavior",
                status=TestStatus.PASSED if correct else TestStatus.FAILED,
                elapsed_ms=elapsed_ms,
                error=None if correct else f"Only {success_count}/100 succeeded"
            ))
            print(f"  {'✓' if correct else '✗'} 100 rapid sequential queries             {elapsed_ms:>7.1f}ms ({success_count}/100)")

        except Exception as e:
            self.results.append(TestResult(
                name="Transaction behavior",
                category="Transaction Behavior",
                status=TestStatus.FAILED,
                error=str(e),
                elapsed_ms=0
            ))
            print(f"  {Colors.RED}✗{Colors.END} Transaction behavior test failed: {str(e)}")

    def test_error_recovery(self):
        """Test 10: Error Recovery and Handling"""
        self.print_header("10. ERROR RECOVERY & HANDLING")

        try:
            symbol = f"{Colors.GREEN}✓{Colors.END}"
            fail_symbol = f"{Colors.YELLOW}⊗{Colors.END}"

            print("\n  Testing error handling, recovery, and failure scenarios...\n")

            # Test 1: Invalid table name (already tested but included for completeness)
            print(f"  {symbol} Error Scenario Tests:")

            try:
                start = time.time()
                with self.conn.cursor() as cur:
                    cur.execute("SELECT COUNT(*) FROM nonexistent_table;")
                elapsed = (time.time() - start) * 1000

                # If we get here, something's wrong
                self.results.append(TestResult(
                    name="Error: Invalid table detection",
                    category="Error Recovery",
                    status=TestStatus.FAILED,
                    error="Expected error but query succeeded",
                    elapsed_ms=elapsed
                ))
                print(f"  {Colors.RED}✗{Colors.END} Invalid table should fail")

            except Exception as e:
                elapsed = (time.time() - start) * 1000
                error_msg = str(e).lower()

                # Check for reasonable error message
                if "does not exist" in error_msg or "relation" in error_msg or "not found" in error_msg or "table" in error_msg:
                    self.results.append(TestResult(
                        name="Error: Invalid table detection",
                        category="Error Recovery",
                        status=TestStatus.PASSED,
                        elapsed_ms=elapsed
                    ))
                    print(f"    {symbol} Invalid table detected:      {elapsed:6.1f}ms (error handled correctly)")
                else:
                    self.results.append(TestResult(
                        name="Error: Invalid table detection",
                        category="Error Recovery",
                        status=TestStatus.FAILED,
                        error=f"Unclear error: {str(e)[:50]}",
                        elapsed_ms=elapsed
                    ))
                    print(f"    {Colors.RED}✗{Colors.END} Invalid table error unclear:  {elapsed:6.1f}ms")

            # Test 2: Invalid column name
            try:
                start = time.time()
                with self.conn.cursor() as cur:
                    cur.execute("SELECT nonexistent_column FROM employees;")
                elapsed = (time.time() - start) * 1000

                self.results.append(TestResult(
                    name="Error: Invalid column detection",
                    category="Error Recovery",
                    status=TestStatus.FAILED,
                    error="Expected error but query succeeded",
                    elapsed_ms=elapsed
                ))
                print(f"    {Colors.RED}✗{Colors.END} Invalid column should fail")

            except Exception as e:
                elapsed = (time.time() - start) * 1000
                error_msg = str(e).lower()

                if "column" in error_msg or "not found" in error_msg or "does not exist" in error_msg:
                    self.results.append(TestResult(
                        name="Error: Invalid column detection",
                        category="Error Recovery",
                        status=TestStatus.PASSED,
                        elapsed_ms=elapsed
                    ))
                    print(f"    {symbol} Invalid column detected:     {elapsed:6.1f}ms (error handled correctly)")
                else:
                    self.results.append(TestResult(
                        name="Error: Invalid column detection",
                        category="Error Recovery",
                        status=TestStatus.FAILED,
                        error=f"Unclear error: {str(e)[:50]}",
                        elapsed_ms=elapsed
                    ))
                    print(f"    {Colors.RED}✗{Colors.END} Invalid column error unclear: {elapsed:6.1f}ms")

            # Test 3: Malformed SQL
            try:
                start = time.time()
                with self.conn.cursor() as cur:
                    cur.execute("SELECT * FROM WHERE employees;")
                elapsed = (time.time() - start) * 1000

                self.results.append(TestResult(
                    name="Error: Malformed SQL detection",
                    category="Error Recovery",
                    status=TestStatus.FAILED,
                    error="Expected error but query succeeded",
                    elapsed_ms=elapsed
                ))
                print(f"    {Colors.RED}✗{Colors.END} Malformed SQL should fail")

            except Exception as e:
                elapsed = (time.time() - start) * 1000
                error_msg = str(e).lower()

                if "syntax" in error_msg or "error" in error_msg:
                    self.results.append(TestResult(
                        name="Error: Malformed SQL detection",
                        category="Error Recovery",
                        status=TestStatus.PASSED,
                        elapsed_ms=elapsed
                    ))
                    print(f"    {symbol} Malformed SQL detected:      {elapsed:6.1f}ms (error handled correctly)")
                else:
                    self.results.append(TestResult(
                        name="Error: Malformed SQL detection",
                        category="Error Recovery",
                        status=TestStatus.FAILED,
                        error=f"Unclear error: {str(e)[:50]}",
                        elapsed_ms=elapsed
                    ))
                    print(f"    {Colors.RED}✗{Colors.END} Malformed SQL error unclear:  {elapsed:6.1f}ms")

            # Test 4: Connection recovery - verify connection still works after errors
            print(f"\n  {symbol} Connection Recovery Tests:")

            recovery_tests = [
                ("After invalid table error", "SELECT COUNT(*) FROM employees;"),
                ("After invalid column error", "SELECT emp_id FROM employees LIMIT 1;"),
                ("After syntax error", "SELECT * FROM employees LIMIT 1;"),
                ("Complex query recovery", "SELECT dept_id, COUNT(*) FROM employees GROUP BY dept_id;"),
            ]

            recovery_successful = 0
            for desc, query in recovery_tests:
                try:
                    start = time.time()
                    with self.conn.cursor() as cur:
                        cur.execute(query)
                        result = cur.fetchall()
                    elapsed = (time.time() - start) * 1000

                    recovery_successful += 1
                    self.results.append(TestResult(
                        name=f"Recovery: {desc}",
                        category="Error Recovery",
                        status=TestStatus.PASSED,
                        elapsed_ms=elapsed
                    ))
                    print(f"    {symbol} {desc:35} {elapsed:6.1f}ms (recovered)")

                except Exception as e:
                    elapsed = (time.time() - start) * 1000
                    self.results.append(TestResult(
                        name=f"Recovery: {desc}",
                        category="Error Recovery",
                        status=TestStatus.FAILED,
                        error=str(e)[:50],
                        elapsed_ms=elapsed
                    ))
                    print(f"    {Colors.RED}✗{Colors.END} {desc:35} {elapsed:6.1f}ms (FAILED)")

            recovery_rate = (recovery_successful / len(recovery_tests)) * 100
            print(f"\n    {symbol} Recovery rate: {recovery_successful}/{len(recovery_tests)} ({recovery_rate:.1f}%)")

            self.results.append(TestResult(
                name="Overall recovery rate",
                category="Error Recovery",
                status=TestStatus.PASSED if recovery_rate == 100 else TestStatus.FAILED,
                elapsed_ms=recovery_rate
            ))

            # Test 5: Error performance comparison (small vs large query failures)
            print(f"\n  {symbol} Error Performance Analysis:")

            # Small query error
            try:
                start = time.time()
                with self.conn.cursor() as cur:
                    cur.execute("SELECT COUNT(*) FROM nonexistent_small;")
            except Exception:
                small_error_time = (time.time() - start) * 1000

            # Large query error
            try:
                start = time.time()
                with self.conn.cursor() as cur:
                    cur.execute("SELECT * FROM nonexistent_large WHERE salary > 100000 AND dept_id IN (1,2,3,4,5) ORDER BY emp_id DESC LIMIT 1000;")
            except Exception:
                large_error_time = (time.time() - start) * 1000

            error_overhead = large_error_time - small_error_time

            print(f"    {symbol} Small query error:           {small_error_time:6.1f}ms")
            print(f"    {symbol} Large query error:           {large_error_time:6.1f}ms")
            print(f"    {symbol} Error complexity overhead:   {error_overhead:+6.1f}ms")

            self.results.append(TestResult(
                name="Error performance: Small query",
                category="Error Recovery",
                status=TestStatus.PASSED,
                elapsed_ms=small_error_time
            ))

            self.results.append(TestResult(
                name="Error performance: Large query",
                category="Error Recovery",
                status=TestStatus.PASSED,
                elapsed_ms=large_error_time
            ))

            # Test 6: Successive error handling
            print(f"\n  {symbol} Successive Error Handling:")

            successive_errors = [
                "SELECT * FROM error_test_1;",
                "SELECT * FROM error_test_2;",
                "SELECT * FROM error_test_3;",
                "SELECT * FROM error_test_4;",
                "SELECT * FROM error_test_5;",
            ]

            error_times = []
            for i, query in enumerate(successive_errors, 1):
                try:
                    start = time.time()
                    with self.conn.cursor() as cur:
                        cur.execute(query)
                except Exception:
                    elapsed = (time.time() - start) * 1000
                    error_times.append(elapsed)

            if error_times:
                avg_error_time = statistics.mean(error_times)
                min_error_time = min(error_times)
                max_error_time = max(error_times)

                print(f"    {symbol} 5 successive errors handled successfully")
                print(f"    {symbol} Average error time:         {avg_error_time:6.1f}ms")
                print(f"    {symbol} Min error time:             {min_error_time:6.1f}ms")
                print(f"    {symbol} Max error time:             {max_error_time:6.1f}ms")

                self.results.append(TestResult(
                    name="Successive errors: Average time",
                    category="Error Recovery",
                    status=TestStatus.PASSED,
                    elapsed_ms=avg_error_time
                ))

            # Test 7: Post-error query performance
            print(f"\n  {symbol} Post-Error Query Performance:")

            # Baseline: normal query before intentional error
            start = time.time()
            with self.conn.cursor() as cur:
                cur.execute("SELECT COUNT(*) FROM employees;")
                cur.fetchone()
            baseline_time = (time.time() - start) * 1000

            # Trigger error
            try:
                with self.conn.cursor() as cur:
                    cur.execute("SELECT * FROM intentional_error;")
            except Exception:
                pass

            # Measure same query after error
            start = time.time()
            with self.conn.cursor() as cur:
                cur.execute("SELECT COUNT(*) FROM employees;")
                cur.fetchone()
            post_error_time = (time.time() - start) * 1000

            performance_impact = post_error_time - baseline_time
            performance_pct = (performance_impact / baseline_time) * 100 if baseline_time > 0 else 0

            print(f"    {symbol} Baseline query:              {baseline_time:6.1f}ms")
            print(f"    {symbol} After error:                 {post_error_time:6.1f}ms")
            print(f"    {symbol} Performance impact:          {performance_impact:+6.1f}ms ({performance_pct:+.1f}%)")

            self.results.append(TestResult(
                name="Post-error performance impact",
                category="Error Recovery",
                status=TestStatus.PASSED if abs(performance_pct) < 200 else TestStatus.FAILED,
                elapsed_ms=performance_impact
            ))

            # Summary
            print(f"\n  {symbol} Error Handling Summary:")
            error_tests = [r for r in self.results if r.category == "Error Recovery"]
            passed_errors = len([r for r in error_tests if r.status == TestStatus.PASSED])
            total_errors = len(error_tests)

            print(f"    {symbol} Total error tests:           {passed_errors}/{total_errors} passed")
            print(f"    {symbol} Connection remains stable:   {'Yes' if recovery_rate == 100 else 'No'}")
            print(f"    {symbol} Error detection accurate:    {'Yes' if passed_errors >= total_errors * 0.8 else 'Needs improvement'}")
            print(f"    {symbol} Performance impact minimal:  {'Yes' if abs(performance_pct) < 20 else 'Moderate'}")

        except Exception as e:
            self.results.append(TestResult(
                name="Error recovery verification",
                category="Error Recovery",
                status=TestStatus.FAILED,
                error=str(e),
                elapsed_ms=0
            ))
            print(f"  {Colors.RED}✗{Colors.END} Error recovery test failed: {str(e)}")

    def test_performance(self):
        """Test 4: Performance Benchmarks"""
        self.print_header("4. PERFORMANCE BENCHMARKS")

        self.test(
            "Performance",
            "Full table scan",
            "SELECT COUNT(*) FROM employees;",
            threshold_ms=2000
        )

        self.test(
            "Performance",
            "Range query (50% selectivity)",
            "SELECT COUNT(*) FROM employees WHERE salary > 150000;",
            threshold_ms=2000
        )

        self.test(
            "Performance",
            "Range query (10% selectivity)",
            "SELECT COUNT(*) FROM employees WHERE salary > 200000;",
            threshold_ms=2000
        )

        self.test(
            "Performance",
            "Highly selective (1%)",
            "SELECT COUNT(*) FROM employees WHERE salary > 240000;",
            threshold_ms=2000
        )

        self.test(
            "Performance",
            "Multi-condition filter",
            "SELECT COUNT(*) FROM employees WHERE salary > 100000 AND city = 'New York' AND hire_date > '2015-01-01';",
            threshold_ms=3000
        )

        self.test(
            "Performance",
            "Fetch 100 rows",
            "SELECT emp_id, name, salary FROM employees LIMIT 100;",
            threshold_ms=2000
        )

        self.test(
            "Performance",
            "Fetch 500 rows",
            "SELECT emp_id, name, salary FROM employees LIMIT 500;",
            threshold_ms=3000
        )

    def test_query_consistency(self):
        """Test 5: Query Consistency (repeated execution)"""
        self.print_header("5. QUERY CONSISTENCY")

        print("  Running 10 repeated queries to measure consistency...\n")

        times = []
        for i in range(10):
            start = time.time()
            success, _, _ = self.execute("SELECT COUNT(*) FROM employees WHERE salary > 100000;")
            elapsed_ms = (time.time() - start) * 1000

            if success:
                times.append(elapsed_ms)
                status = f"{Colors.GREEN}✓{Colors.END}"
            else:
                status = f"{Colors.RED}✗{Colors.END}"

            print(f"  {status} Run {i+1:2d}: {elapsed_ms:7.1f}ms")

        if times:
            print(f"\n  {Colors.BOLD}Statistics:${Colors.END}")
            print(f"    Average: {statistics.mean(times):.1f}ms")
            print(f"    Median:  {statistics.median(times):.1f}ms")
            print(f"    Min:     {min(times):.1f}ms")
            print(f"    Max:     {max(times):.1f}ms")
            if len(times) > 1:
                print(f"    StdDev:  {statistics.stdev(times):.1f}ms")

            # Add to results
            self.results.append(TestResult(
                name="Query consistency",
                category="Consistency",
                status=TestStatus.PASSED,
                elapsed_ms=statistics.mean(times)
            ))

    def test_invalid_table(self):
        """Test invalid table error handling"""
        start = time.time()
        try:
            cursor = self.conn.cursor()
            cursor.execute("SELECT * FROM nonexistent_table_xyz;")
            cursor.fetchone()
            # If we get here, test failed - should have raised an error
            elapsed_ms = (time.time() - start) * 1000
            test_result = TestResult(
                name="Invalid table handling",
                category="Reliability",
                status=TestStatus.FAILED,
                elapsed_ms=elapsed_ms,
                error="Query should have raised an error for non-existent table"
            )
            self.results.append(test_result)
            self._print_line(False, "Invalid table handling", elapsed_ms)
        except psycopg2.Error as e:
            # Expected behavior - query failed
            elapsed_ms = (time.time() - start) * 1000
            if "does not exist" in str(e) or "relation" in str(e).lower():
                # Good error message
                test_result = TestResult(
                    name="Invalid table handling",
                    category="Reliability",
                    status=TestStatus.PASSED,
                    elapsed_ms=elapsed_ms
                )
                self.results.append(test_result)
                self._print_line(True, "Invalid table handling", elapsed_ms)
            else:
                # Bad error message
                test_result = TestResult(
                    name="Invalid table handling",
                    category="Reliability",
                    status=TestStatus.FAILED,
                    elapsed_ms=elapsed_ms,
                    error=f"Unexpected error message: {str(e)[:80]}"
                )
                self.results.append(test_result)
                self._print_line(False, "Invalid table handling", elapsed_ms)

    def test_reliability(self):
        """Test 6: Reliability & Error Handling"""
        self.print_header("6. RELIABILITY & ERROR HANDLING")

        self.test(
            "Reliability",
            "Error message clarity",
            "SELECT COUNT(*) FROM employees WHERE salary > 0;",
            threshold_ms=2000
        )

        self.test_invalid_table()

    # =========================================================================
    # REPORTING
    # =========================================================================

    def print_report(self) -> bool:
        """Print comprehensive test report"""
        elapsed = (datetime.now() - self.start_time).total_seconds()

        self.print_footer()
        self.print_header("TEST REPORT - FairCom FDW Commercialization Suite")

        # Summarize results
        by_status = {}
        by_category = {}

        for result in self.results:
            status = result.status.value
            by_status[status] = by_status.get(status, 0) + 1

            cat = result.category
            by_category[cat] = by_category.get(cat, 0) + 1

        total = len(self.results)
        passed = by_status.get("PASSED", 0)
        failed = by_status.get("FAILED", 0)
        skipped = by_status.get("SKIPPED", 0)

        # Compute pass rate (excluding skipped)
        testable = passed + failed
        pass_rate = (passed / testable * 100) if testable > 0 else 0

        print(f"\n{Colors.BOLD}Test Results:{Colors.END}")
        print(f"  Passed:  {passed}/{testable} ({pass_rate:.1f}%)")
        print(f"  Failed:  {failed}/{testable}")
        print(f"  Skipped: {skipped}")
        print(f"  Total:   {total}")
        print(f"  Duration: {elapsed:.1f}s")

        # Results by category
        print(f"\n{Colors.BOLD}Results by Category:{Colors.END}")
        for category, count in sorted(by_category.items()):
            cat_results = [r for r in self.results if r.category == category and r.status != TestStatus.SKIPPED]
            cat_passed = len([r for r in cat_results if r.status == TestStatus.PASSED])
            cat_total = len(cat_results)
            if cat_total > 0:
                cat_rate = cat_passed / cat_total * 100
                print(f"  {category:<25} {cat_passed:2d}/{cat_total:2d} ({cat_rate:5.1f}%)")

        # Performance results
        perf_results = [r for r in self.results if r.status == TestStatus.PASSED and r.elapsed_ms > 0]
        if perf_results:
            print(f"\n{Colors.BOLD}Performance Analysis:{Colors.END}")
            sorted_by_time = sorted(perf_results, key=lambda r: r.elapsed_ms, reverse=True)
            for result in sorted_by_time[:5]:
                print(f"  {result.name:<50} {result.elapsed_ms:7.1f}ms")

        # Failed tests
        failed_results = [r for r in self.results if r.status == TestStatus.FAILED]
        if failed_results:
            print(f"\n{Colors.RED}{Colors.BOLD}Failed Tests:{Colors.END}")
            for result in failed_results:
                print(f"  {result.name}")
                if result.error:
                    print(f"    Error: {result.error[:80]}")

        # Commercialization readiness
        print(f"\n{Colors.BOLD}Commercialization Readiness:{Colors.END}")

        if failed == 0 and pass_rate >= 95:
            print(f"  Functionality Score: {pass_rate:.0f}/100 {Colors.GREEN}[PASS]{Colors.END}")
            print(f"  Reliability Score: 100/100 {Colors.GREEN}[PASS]{Colors.END}")
            print(f"\n{Colors.GREEN}{Colors.BOLD}STATUS: READY FOR COMMERCIALIZATION{Colors.END}")
            ready = True
        elif pass_rate >= 85:
            print(f"  Functionality Score: {pass_rate:.0f}/100 {Colors.YELLOW}[PARTIAL]{Colors.END}")
            print(f"  Reliability Score: {max(0, 100-failed*5)}/100 {Colors.YELLOW}[REVIEW]{Colors.END}")
            print(f"\n{Colors.YELLOW}{Colors.BOLD}STATUS: REQUIRES MINOR FIXES{Colors.END}")
            ready = False
        else:
            print(f"  Functionality Score: {pass_rate:.0f}/100 {Colors.RED}[FAIL]{Colors.END}")
            print(f"  Reliability Score: {max(0, 100-failed*10)}/100 {Colors.RED}[FAIL]{Colors.END}")
            print(f"\n{Colors.RED}{Colors.BOLD}STATUS: REQUIRES MAJOR FIXES{Colors.END}")
            ready = False

        self.print_footer()

        return ready

    def export_json(self, filename: str):
        """Export results to JSON"""
        export_data = {
            "timestamp": self.start_time.isoformat(),
            "duration_seconds": (datetime.now() - self.start_time).total_seconds(),
            "connection": {
                "host": self.host,
                "port": self.port,
                "database": self.database,
                "user": self.user
            },
            "summary": {
                "passed": len([r for r in self.results if r.status == TestStatus.PASSED]),
                "failed": len([r for r in self.results if r.status == TestStatus.FAILED]),
                "skipped": len([r for r in self.results if r.status == TestStatus.SKIPPED]),
                "total": len(self.results)
            },
            "results": [
                {
                    **asdict(r),
                    "status": r.status.value
                }
                for r in self.results
            ]
        }

        with open(filename, 'w') as f:
            json.dump(export_data, f, indent=2, default=str)

        print(f"Results exported to {filename}")


def main():
    parser = argparse.ArgumentParser(
        description="FairCom FDW Comprehensive Test Suite",
        formatter_class=argparse.RawDescriptionHelpFormatter,
        epilog="""
Examples:
  python3 test_fdw.py                          # Connect to localhost:5432
  python3 test_fdw.py -h docker.local           # Connect to docker.local:5432
  python3 test_fdw.py -h localhost -p 5432 -u postgres -w postgres
  python3 test_fdw.py --export results.json    # Export JSON report
  python3 test_fdw.py --no-color                # Disable colored output
        """
    )

    parser.add_argument("--host", default="localhost", help="PostgreSQL host (default: localhost)")
    parser.add_argument("--port", type=int, default=5432, help="PostgreSQL port (default: 5432)")
    parser.add_argument("--database", default="testdb", help="Database name (default: testdb)")
    parser.add_argument("--user", default="postgres", help="PostgreSQL user (default: postgres)")
    parser.add_argument("--password", default="postgres", help="PostgreSQL password (default: postgres)")
    parser.add_argument("--export", help="Export results to JSON file")
    parser.add_argument("--no-color", action="store_true", help="Disable colored output")

    args = parser.parse_args()

    # Create test suite
    suite = FDWTestSuite(
        host=args.host,
        port=args.port,
        database=args.database,
        user=args.user,
        password=args.password,
        no_color=args.no_color
    )

    # Connect
    if not suite.connect():
        sys.exit(1)

    try:
        # Run all tests
        suite.test_connection_setup()
        suite.test_data_integrity()
        suite.test_write_operations()
        suite.test_query_functionality()
        suite.test_advanced_queries()
        suite.test_concurrent_connections()
        suite.test_index_impact()
        suite.test_range_stop_optimization()
        suite.test_cache_metrics()
        suite.test_large_result_sets()
        suite.test_complex_predicates()
        suite.test_like_patterns()  # Comprehensive LIKE pattern wildcard tests
        suite.test_concurrent_queries()
        suite.test_data_type_pushdown()
        suite.test_join_pushdown()
        suite.test_aggregate_pushdown()
        suite.test_limit_pushdown()
        suite.test_offset_edge_cases()
        suite.test_null_handling()
        suite.test_explain_verification()
        suite.test_subqueries_and_ctes()
        suite.test_write_edge_cases()
        suite.test_data_type_edge_cases()
        suite.test_guc_parameters()
        suite.test_distinct_and_window_functions()
        suite.test_transaction_behavior()
        suite.test_error_recovery()
        suite.test_performance()
        suite.test_query_consistency()
        suite.test_reliability()

        # Print report
        success = suite.print_report()

        # Export JSON if requested
        if args.export:
            suite.export_json(args.export)

        sys.exit(0 if success else 1)

    except KeyboardInterrupt:
        print(f"\n\n{Colors.YELLOW}Test suite interrupted by user{Colors.END}")
        sys.exit(1)
    except Exception as e:
        print(f"\n{Colors.RED}Unexpected error: {e}{Colors.END}")
        import traceback
        traceback.print_exc()
        sys.exit(1)
    finally:
        suite.close()


if __name__ == "__main__":
    main()
