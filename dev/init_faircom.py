#!/usr/bin/env python3
"""Initialize FairCom test_emp_v4 database using REST API"""

import json
import time
import requests
import re

API_URL = "http://localhost:8080/api"
DB_NAME = "test_emp_v4"
SQL_FILE = "/opt/faircom/init_faircom.sql"
BATCH_SIZE = 50

def wait_for_api():
    """Wait for FairCom API to be ready"""
    print("Waiting for FairCom API...")
    for _ in range(60):
        try:
            requests.post(API_URL, timeout=1)
            time.sleep(3)
            return True
        except:
            time.sleep(1)
    return False

def create_session():
    """Create admin session and return auth token"""
    payload = {
        "api": "admin",
        "apiVersion": "1.0",
        "action": "createSession",
        "params": {
            "username": "ADMIN",
            "password": "ADMIN"
        }
    }

    response = requests.post(API_URL, json=payload)
    result = response.json()

    if result.get("errorCode") != 0:
        raise Exception(f"Auth failed: {result.get('errorMessage')}")

    token = result.get("authToken")
    print(f"Got auth token")
    return token

def create_database(token):
    """Create the database"""
    payload = {
        "api": "db",
        "apiVersion": "1.0",
        "authToken": token,
        "action": "createDatabase",
        "params": {
            "databaseName": DB_NAME
        }
    }

    response = requests.post(API_URL, json=payload)
    result = response.json()

    # Check for errors - ignore if database already exists
    if result.get("errorCode") != 0:
        error_msg = result.get("errorMessage", "Unknown error")
        if "already exists" in error_msg.lower() or "duplicate" in error_msg.lower():
            print(f"Database '{DB_NAME}' already exists - continuing")
            return
        else:
            print(f"WARNING: Database creation returned: {error_msg}")
            print("Continuing anyway - database may already exist")
            return

    print(f"Database '{DB_NAME}' created successfully")
    time.sleep(2)

def parse_sql_file(create_only=False):
    """Parse SQL file into individual statements"""
    with open(SQL_FILE, 'r') as f:
        content = f.read()

    # Remove comments
    content = re.sub(r'--.*$', '', content, flags=re.MULTILINE)

    # Split by semicolons
    statements = [s.strip() for s in content.split(';') if s.strip()]

    # Filter to only CREATE TABLE if requested
    if create_only:
        statements = [s for s in statements if s.upper().startswith('CREATE TABLE')]

    print(f"Parsed {len(statements)} SQL statements")
    return statements

def parse_create_table_to_json(sql):
    """Convert CREATE TABLE SQL to JSON API format"""
    # Extract table name - format: CREATE TABLE admin.tablename (...)
    table_match = re.search(r'CREATE TABLE\s+(?:admin\.)?(\w+)\s*\(', sql, re.IGNORECASE)
    if not table_match:
        return None

    table_name = table_match.group(1)

    # Extract column definitions between parentheses
    columns_match = re.search(r'\((.*)\)', sql, re.DOTALL)
    if not columns_match:
        return None

    columns_text = columns_match.group(1)

    # Parse each column definition
    # Need to split by commas but not inside parentheses
    fields = []
    current_col = ""
    paren_depth = 0

    for char in columns_text:
        if char == '(':
            paren_depth += 1
            current_col += char
        elif char == ')':
            paren_depth -= 1
            current_col += char
        elif char == ',' and paren_depth == 0:
            # This is a column separator, not part of a type definition
            if current_col.strip():
                fields.append(parse_column_definition(current_col.strip()))
            current_col = ""
        else:
            current_col += char

    # Don't forget the last column
    if current_col.strip():
        fields.append(parse_column_definition(current_col.strip()))

    # Filter out None values (parsing failures) and PRIMARY KEY constraints
    fields = [f for f in fields if f is not None]

    return {
        'tableName': table_name,
        'fields': fields
    }

def parse_column_definition(line):
    """Parse a single column definition"""
    if not line or line.upper().startswith('PRIMARY KEY'):
        return None

    # Parse: column_name TYPE(size) [NOT NULL] [PRIMARY KEY]
    parts = line.split()
    if len(parts) < 2:
        return None

    col_name = parts[0]
    col_type_raw = parts[1]

    # Extract type and length
    type_match = re.match(r'(\w+)(?:\((\d+)(?:,(\d+))?\))?', col_type_raw)
    if not type_match:
        return None

    base_type = type_match.group(1).lower()
    type_length = type_match.group(2)
    type_scale = type_match.group(3)

    # Map SQL types to JSON API types
    type_map = {
        'integer': 'integer',
        'int': 'integer',
        'varchar': 'varchar',
        'char': 'char',
        'date': 'date',
        'decimal': 'decimal',
        'numeric': 'decimal',
        'float': 'float',
        'double': 'double'
    }

    json_type = type_map.get(base_type, 'varchar')

    field = {
        'name': col_name,
        'type': json_type
    }

    # Add length for string types
    if json_type in ['varchar', 'char'] and type_length:
        field['length'] = int(type_length)

    # Add length/scale for decimal (API expects 'length' not 'precision')
    if json_type == 'decimal' and type_length:
        field['length'] = int(type_length)
        if type_scale:
            field['scale'] = int(type_scale)
        else:
            field['scale'] = 0

    # Check for PRIMARY KEY (API expects integer 0 or 1)
    if 'PRIMARY KEY' in line.upper():
        field['primaryKey'] = 1

    # NOT NULL is not supported by the createTable API - skip it

    return field

def create_table_via_api(token, table_def):
    """Create table using JSON API"""
    payload = {
        'api': 'db',
        'apiVersion': '1.0',
        'action': 'createTable',
        'params': {
            'databaseName': DB_NAME,
            'tableName': table_def['tableName'],
            'fields': table_def['fields']
        },
        'authToken': token
    }

    response = requests.post(API_URL, json=payload)
    result = response.json()

    if result.get('errorCode') != 0:
        error_msg = result.get('errorMessage', 'Unknown error')
        error_code = result.get('errorCode', 'N/A')
        # Ignore if table already exists
        if 'already exists' in error_msg.lower() or 'duplicate' in error_msg.lower():
            print(f"Table {table_def['tableName']} already exists - skipping")
            return True
        print(f"ERROR creating table {table_def['tableName']}: [{error_code}] {error_msg}")
        return False

    print(f"Created table: {table_def['tableName']}")
    return True

def execute_batch(token, statements):
    """Execute a batch of SQL statements"""
    payload = {
        "api": "db",
        "apiVersion": "1.0",
        "authToken": token,
        "action": "runSQLStatements",
        "params": {
            "databaseName": DB_NAME,
            "sqlStatements": statements
        }
    }

    response = requests.post(API_URL, json=payload)
    result = response.json()

    if result.get("errorCode") != 0:
        return False, result.get("errorMessage", "Unknown error")

    # Check individual statement results
    reactions = result.get("result", {}).get("reactions", [])
    errors = [r for r in reactions if r.get("errorCode") != 0]

    if errors:
        return False, errors[0].get("errorMessage", "Statement failed")

    return True, None

def main():
    import os
    # Wait for API
    if not wait_for_api():
        print("ERROR: FairCom API not ready")
        return 1

    # Create session
    token = create_session()

    # Create database
    print(f"\nCreating database: {DB_NAME}")
    create_database(token)

    # Create tables via SQL DDL (not JSON API) to get proper RowId handling
    # NOTE: Removed 'admin.' schema prefix - CTDB API expects flat table names
    print("\nCreating tables via SQL DDL...")
    create_table_statements = [
        "CREATE TABLE departments (dept_id INTEGER PRIMARY KEY, dept_name VARCHAR(100), location VARCHAR(100), budget NUMERIC(12,2), manager_name VARCHAR(100))",
        "CREATE TABLE projects (project_id INTEGER PRIMARY KEY, project_name VARCHAR(150), start_date DATE, end_date DATE, budget NUMERIC(12,2), status VARCHAR(20), dept_id INTEGER)",
        "CREATE TABLE employees (emp_id INTEGER PRIMARY KEY, name VARCHAR(100), email VARCHAR(100), phone VARCHAR(20), city VARCHAR(50), hire_date DATE, salary NUMERIC(10,2), dept_id INTEGER, job_title VARCHAR(100))",
        "CREATE TABLE employee_projects (assignment_id INTEGER PRIMARY KEY, emp_id INTEGER, project_id INTEGER, role VARCHAR(50), assignment_date DATE, allocation_percent INTEGER)",
        "CREATE TABLE timesheets (timesheet_id INTEGER PRIMARY KEY, emp_id INTEGER, project_id INTEGER, work_date DATE, hours_worked NUMERIC(4,2), description VARCHAR(200))"
    ]

    existing_tables = 0
    created_tables = 0
    for stmt in create_table_statements:
        success, error = execute_batch(token, [stmt])
        if success:
            table_name = stmt.split()[2]  # Now just "tablename" without schema
            print(f"Created table: {table_name}")
            created_tables += 1
        else:
            error_text = (error or "").lower()
            if "already exists" in error_text or "duplicate" in error_text:
                table_name = stmt.split()[2]
                print(f"Table {table_name} already exists - skipping")
                existing_tables += 1
                continue
            print(f"ERROR creating table: {error}")
            return 1

    print(f"\nCreated {created_tables} tables")

    force_reload = os.environ.get("FORCE_RELOAD", "").lower() in ("1", "true", "yes")

    if created_tables == 0 and existing_tables > 0 and not force_reload:
        print("All tables already exist - skipping data load")
        return 0

    if created_tables == 0 and existing_tables > 0 and force_reload:
        print("All tables already exist - FORCE_RELOAD enabled, clearing data")
        clear_statements = [
            "DELETE FROM timesheets",
            "DELETE FROM employee_projects",
            "DELETE FROM employees",
            "DELETE FROM projects",
            "DELETE FROM departments",
        ]
        success, error = execute_batch(token, clear_statements)
        if not success:
            print(f"ERROR clearing existing data: {error}")
            return 1

    # Load data via dbload from CSV files
    print("\nLoading data from CSV files...")
    import csv

    # Define CSV files and their corresponding insert statements
    # NOTE: Removed 'admin.' prefix to match flat table names
    csv_files = {
        'departments': {
            'path': '/opt/faircom/csv/departments.csv',
            'table': 'departments',
            'columns': ['dept_id', 'dept_name', 'location', 'budget', 'manager_name']
        },
        'projects': {
            'path': '/opt/faircom/csv/projects.csv',
            'table': 'projects',
            'columns': ['project_id', 'project_name', 'start_date', 'end_date', 'budget', 'status', 'dept_id']
        },
        'employees': {
            'path': '/opt/faircom/csv/employees.csv',
            'table': 'employees',
            'columns': ['emp_id', 'name', 'email', 'phone', 'city', 'hire_date', 'salary', 'dept_id', 'job_title']
        },
        'employee_projects': {
            'path': '/opt/faircom/csv/employee_projects.csv',
            'table': 'employee_projects',
            'columns': ['assignment_id', 'emp_id', 'project_id', 'role', 'assignment_date', 'allocation_percent']
        },
        'timesheets': {
            'path': '/opt/faircom/csv/timesheets.csv',
            'table': 'timesheets',
            'columns': ['timesheet_id', 'emp_id', 'project_id', 'work_date', 'hours_worked', 'description']
        }
    }

    total_records = 0

    for table_name, table_info in csv_files.items():
        csv_path = table_info['path']
        table = table_info['table']
        columns = table_info['columns']

        if not os.path.exists(csv_path):
            print(f"WARNING: {csv_path} not found")
            continue

        # Read CSV and generate INSERT statements
        insert_statements = []
        with open(csv_path, 'r') as f:
            reader = csv.reader(f)
            for row in reader:
                if len(row) != len(columns):
                    print(f"WARNING: Row has {len(row)} columns, expected {len(columns)}")
                    continue

                # Quote string values, leave numbers unquoted
                values = []
                for i, val in enumerate(row):
                    # Numeric columns: dept_id, project_id, emp_id, assignment_id, allocation_percent,
                    # salary, budget, timesheet_id, hours_worked
                    numeric_cols = ['dept_id', 'project_id', 'emp_id', 'assignment_id', 'allocation_percent',
                                   'salary', 'budget', 'timesheet_id', 'hours_worked']
                    if columns[i] in numeric_cols:
                        values.append(val)
                    else:
                        # Quote string values and escape single quotes
                        escaped = val.replace("'", "''")
                        values.append(f"'{escaped}'")

                cols_str = ', '.join(columns)
                vals_str = ', '.join(values)
                insert_statements.append(f"INSERT INTO {table} ({cols_str}) VALUES ({vals_str})")

        # Execute in batches
        batch_count = (len(insert_statements) + BATCH_SIZE - 1) // BATCH_SIZE
        for batch_num in range(batch_count):
            start_idx = batch_num * BATCH_SIZE
            end_idx = min(start_idx + BATCH_SIZE, len(insert_statements))
            batch = insert_statements[start_idx:end_idx]

            success, error = execute_batch(token, batch)
            if not success:
                print(f"ERROR loading {table_name}: {error}")
                return 1

        total_records += len(insert_statements)
        print(f"Loaded {table_name}: {len(insert_statements)} records")

    print(f"\nDatabase initialization complete - {total_records} records loaded from CSV files")

    return 0

if __name__ == "__main__":
    exit(main())
