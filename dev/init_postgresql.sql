-- Create FairCom FDW extension
CREATE EXTENSION IF NOT EXISTS faircom_fdw;

-- Create server connection to FairCom
CREATE SERVER faircom_server
    FOREIGN DATA WRAPPER faircom_fdw
    OPTIONS (
        host 'faircom-server',
        port '5597',
        database 'test_emp_v4'
    );

-- Create user mapping
CREATE USER MAPPING FOR postgres
    SERVER faircom_server
    OPTIONS (
        username 'ADMIN',
        password 'ADMIN'
    );

-- Create foreign table for employees
-- NOTE: FairCom adds default "admin" schema automatically - table files are admin_employees.dat
CREATE FOREIGN TABLE employees (
    emp_id INTEGER,
    name TEXT,
    email TEXT,
    phone TEXT,
    city TEXT,
    hire_date DATE,
    salary NUMERIC(10,2),
    dept_id INTEGER,
    job_title TEXT
)
SERVER faircom_server
OPTIONS (table_name 'test_emp_v4.admin.employees');

-- Create foreign table for departments
CREATE FOREIGN TABLE departments (
    dept_id INTEGER,
    dept_name TEXT,
    location TEXT,
    budget NUMERIC(12,2),
    manager_name TEXT
)
SERVER faircom_server
OPTIONS (table_name 'test_emp_v4.admin.departments');

-- Create foreign table for projects
CREATE FOREIGN TABLE projects (
    project_id INTEGER,
    project_name TEXT,
    start_date DATE,
    end_date DATE,
    budget NUMERIC(12,2),
    status TEXT,
    dept_id INTEGER
)
SERVER faircom_server
OPTIONS (table_name 'test_emp_v4.admin.projects');

-- Create foreign table for employee_projects
CREATE FOREIGN TABLE employee_projects (
    assignment_id INTEGER,
    emp_id INTEGER,
    project_id INTEGER,
    role TEXT,
    assignment_date DATE,
    allocation_percent INTEGER
)
SERVER faircom_server
OPTIONS (table_name 'test_emp_v4.admin.employee_projects');

-- Create foreign table for timesheets
CREATE FOREIGN TABLE timesheets (
    timesheet_id INTEGER,
    emp_id INTEGER,
    project_id INTEGER,
    work_date DATE,
    hours_worked NUMERIC(4,2),
    description TEXT
)
SERVER faircom_server
OPTIONS (table_name 'test_emp_v4.admin.timesheets');
