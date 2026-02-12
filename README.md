# FairCom Foreign Data Wrapper for PostgreSQL

![Version](https://img.shields.io/badge/version-1.0.4-blue)
![PostgreSQL](https://img.shields.io/badge/PostgreSQL-14-blue)
![Platform](https://img.shields.io/badge/platform-x86__64%20%7C%20ARM64-green)
![License](https://img.shields.io/badge/license-Proprietary-red)

## Overview

FairCom's PostgreSQL Foreign Data Wrapper provides standard SQL access to FairCom databases through PostgreSQL's foreign table interface. Applications issue queries through PostgreSQL's SQL engine while the FDW translates operations into FairCom's native c-treeDB API calls.

PostgreSQL's optimizer decides which operations to push down to the FairCom server and which to execute locally. Simple scans with predicates run on the FairCom server to reduce network transfer. Complex operations like joins and window functions execute in PostgreSQL's engine. The implementation includes connection pooling, adaptive batch sizing, and query result caching.

Applications written for PostgreSQL can query FairCom data with minimal code changes using standard client libraries across Python, Java, Node.js, and other languages.

## Table of Contents

- [Features](#features)
- [Requirements](#requirements)
- [Quick Start](#quick-start)
- [Configuration Reference](#configuration-reference)
- [Architecture](#architecture)
- [Troubleshooting](#troubleshooting)
- [Packages](#packages)
- [Building from Source](#building-from-source)
- [License](#license)

## Features

### Operations Pushed Down to FairCom

These operations are translated to c-treeDB API calls and executed on the FairCom server, minimizing data transfer:

| Operation | Description |
|-----------|-------------|
| **Table Scans** | Full and partial table scans via c-treeDB record navigation |
| **Predicate Pushdown** | WHERE clauses (`=`, `<`, `>`, `<=`, `>=`, `<>`, `BETWEEN`, `IN`, `IS NULL`) |
| **LIKE Patterns** | Pattern matching with `%` and `_` wildcards |
| **Column Projection** | Only requested columns are read from FairCom |
| **Aggregate Functions** | `COUNT(*)` and `COUNT(column)` computed server-side |
| **LIMIT** | Row count limits applied at the scan level (without ORDER BY) |
| **Index Selection** | Automatic index detection for equality and range queries |
| **Write Operations** | `INSERT`, `UPDATE`, `DELETE` executed directly on FairCom |

### Operations Handled by PostgreSQL

PostgreSQL's SQL engine handles these operations locally after fetching data from FairCom:

| Operation | Description |
|-----------|-------------|
| **JOINs** | `INNER`, `LEFT`, `RIGHT`, `FULL OUTER`, `CROSS`, self joins |
| **Sorting** | `ORDER BY` clauses |
| **Grouping** | `GROUP BY` and `HAVING` clauses |
| **Subqueries** | Correlated and uncorrelated subqueries |
| **Set Operations** | `UNION`, `UNION ALL`, `INTERSECT`, `EXCEPT` |
| **Window Functions** | `ROW_NUMBER`, `RANK`, `LEAD`, `LAG`, etc. |

### Performance

| Feature | Details |
|---------|---------|
| **Adaptive Batch Sizing** | 50–2,000 rows per batch, adjusted at runtime based on row width (target: 1 MB per batch) |
| **Connection Pooling** | Per-backend connection reuse with lazy validation and configurable idle timeout |
| **Plan Caching** | LRU cache for table metadata (default: 500 entries, 5-minute TTL) |
| **Query Result Caching** | Up to 1,000 cached results for repeated aggregate queries (configurable TTL) |
| **Parallel Scan Support** | Integrates with PostgreSQL's parallel query infrastructure |

### Supported Data Types

| PostgreSQL Type | FairCom Types |
|-----------------|---------------|
| `boolean` | `CT_BOOL` |
| `smallint` | `CT_INT2` |
| `integer` | `CT_INT4` |
| `bigint` | `CT_INT8` |
| `real` | `CT_SFLOAT` |
| `double precision` | `CT_DFLOAT` |
| `text` | `CT_STRING`, `CT_FSTRING`, `CT_F2STRING`, `CT_F4STRING`, `CT_VARBINARY` |
| `char` | `CT_CHAR`, `CT_CHARU` |
| `numeric` | `CT_NUMBER`, `CT_CURRENCY`, `CT_MONEY` |
| `timestamp` | `CT_DATE`, `CT_TIME`, `CT_TIMES` |
| `bytea` | `CT_LVB` |

## Requirements

| Component | Version |
|-----------|---------|
| PostgreSQL | 14 |
| FairCom server | Accessible via c-treeDB protocol on TCP port (default 6597) |
| Operating system | Ubuntu 22.04+, Rocky Linux 9+, or compatible |
| Architecture | `x86_64` (amd64) or `aarch64` (arm64) |

## Quick Start

### Try It with Docker

Clone the repository and start the environment:

```bash
git clone https://github.com/toddstoffel/postgres-faircom-fdw.git
cd postgres-faircom-fdw/dev
docker-compose up -d
```

This starts a FairCom server and a PostgreSQL instance with the FDW pre-installed. Once both containers are healthy:

| Service | Host | Port | Credentials |
|---------|------|------|-------------|
| PostgreSQL | `localhost` | `5432` | user `postgres`, password `postgres`, database `testdb` |
| FairCom | `localhost` | `6597` | user `ADMIN`, password `ADMIN`, database `test_emp_v4` |

Connect to PostgreSQL and run queries against FairCom tables:

```bash
psql -h localhost -U postgres -d testdb
```

The Docker environment is configured through `dev/.env`:

| Variable | Default | Description |
|----------|---------|-------------|
| `FDW_BUILD_MODE` | `package` | `package` installs from GitHub Releases; `source` compiles from `core/src/` |
| `FDW_RELEASE_VERSION` | `latest` | Release to install in package mode (`latest` or a specific tag like `v1.0.4`) |

After changing `.env`, rebuild the PostgreSQL container:

```bash
docker-compose up -d --build postgres
```

### Install the Package

Download the appropriate package from [GitHub Releases](https://github.com/toddstoffel/postgres-faircom-fdw/releases/latest).

**Debian / Ubuntu:**

```bash
sudo dpkg -i postgresql-14-faircom-fdw_<version>-1_<arch>.deb
sudo apt-get -f install
```

**RHEL / CentOS / Fedora:**

```bash
sudo rpm -Uvh postgresql-14-faircom-fdw-<version>-1.el9.<arch>.rpm
```

### Configure the Extension

```sql
CREATE EXTENSION faircom_fdw;

CREATE SERVER faircom_server
  FOREIGN DATA WRAPPER faircom_fdw
  OPTIONS (host 'faircom-host', port '6597', database 'mydb');

CREATE USER MAPPING FOR postgres
  SERVER faircom_server
  OPTIONS (username 'ADMIN', password 'secret');
```

### Create Foreign Tables

```sql
CREATE FOREIGN TABLE employees (
    emp_id    INTEGER,
    name      TEXT,
    dept_id   INTEGER,
    salary    NUMERIC(10,2)
) SERVER faircom_server
  OPTIONS (table_name 'mydb.admin.employees');
```

The `table_name` option uses the format `database.schema.table`. The FDW translates this internally: `mydb.admin.employees` becomes database path `mydb.dbs` and table `admin_employees`.

### Import Schema

Generate foreign table templates from an existing FairCom database:

```sql
IMPORT FOREIGN SCHEMA "mydb.admin"
  FROM SERVER faircom_server
  INTO public;
```

### Query

```sql
-- Predicate pushdown: WHERE filters execute on the FairCom server
SELECT name, salary FROM employees WHERE dept_id = 3;

-- JOINs: PostgreSQL joins data from multiple FairCom tables locally
SELECT e.name, d.dept_name
FROM employees e
JOIN departments d ON e.dept_id = d.dept_id;

-- Aggregate pushdown: COUNT executes on FairCom, avoiding full scan
SELECT COUNT(*) FROM employees;

-- Write operations: INSERT, UPDATE, DELETE execute directly on FairCom
INSERT INTO employees (emp_id, name, salary) VALUES (1001, 'Jane Doe', 95000);
UPDATE employees SET salary = salary * 1.10 WHERE dept_id = 3;
DELETE FROM employees WHERE emp_id = 1001;
```

## Configuration Reference

### Server Options

Specified in `CREATE SERVER ... OPTIONS (...)`:

| Option | Type | Default | Description |
|--------|------|---------|-------------|
| `host` | text | `localhost` | FairCom server hostname or IP address |
| `port` | integer | `6597` | FairCom server port (1–65535) |
| `database` | text | — | FairCom database name |
| `connect_timeout` | integer | `30` | Connection timeout in seconds |

### User Mapping Options

Specified in `CREATE USER MAPPING ... OPTIONS (...)`:

| Option | Type | Default | Description |
|--------|------|---------|-------------|
| `username` | text | `ADMIN` | FairCom server username |
| `password` | text | *(empty)* | FairCom server password |

### Foreign Table Options

Specified in `CREATE FOREIGN TABLE ... OPTIONS (...)`:

| Option | Required | Default | Description |
|--------|----------|---------|-------------|
| `table_name` | Yes | — | FairCom table in `database.schema.table` format |
| `use_index` | No | *(auto)* | Force a specific index for scans |
| `enable_pushdown` | No | `true` | Enable predicate pushdown for this table |

### Server Parameters (GUCs)

Tunable at runtime with `SET` or in `postgresql.conf`:

| Parameter | Type | Default | Range | Description |
|-----------|------|---------|-------|-------------|
| `faircom_fdw.batch_size` | integer | `500` | 50–2,000 | Rows fetched per batch (overridden by adaptive sizing) |
| `faircom_fdw.cache_ttl` | integer | `300` | 0–3,600 | Query result cache TTL in seconds (0 disables caching) |
| `faircom_fdw.plan_cache_size` | integer | `500` | 10–10,000 | Maximum plan cache entries (superuser only) |
| `faircom_fdw.connection_idle_timeout` | integer | `1800` | 60–86,400 | Seconds before idle connections close |
| `faircom_fdw.enable_aggregate_pushdown` | boolean | `true` | — | Push `COUNT` aggregates to FairCom |
| `faircom_fdw.enable_predicate_pushdown` | boolean | `true` | — | Push WHERE clauses to FairCom |

### EXPLAIN Output

`EXPLAIN ANALYZE` includes FairCom-specific metrics:

```
FairCom Batch: rows_fetched=1000 batches=2 avg_batch_size=500 optimal_batch_size=500
FairCom Predicates: predicates=1 index_mode=1
FairCom Plan Cache: plan_cache=enabled max_entries=500 ttl=300s
```

With `EXPLAIN (ANALYZE, VERBOSE)`:

```
FairCom Performance: plan_cache_hit_rate=95%
FairCom Query Cache: query_cache_hit_rate=80%
FairCom Pushdown Stats: aggregates=1 predicates=3
```

## Architecture

```
+--------------------------+         +--------------------------+
|     PostgreSQL 14        |         |     FairCom Server       |
|                          |         |                          |
|  +--------------------+  |         |  +--------------------+  |
|  |    SQL Engine      |  |         |  |    c-treeDB API    |  |
|  |  (joins, sorts,    |  |         |  |   (data engine)    |  |
|  |   window funcs)    |  |         |  +--------------------+  |
|  +--------+-----------+  |         |                          |
|           |              |  TCP    |                          |
|  +--------+-----------+  | <-----> |                          |
|  |   FairCom FDW      |  |  6597   |                          |
|  |  (scans, preds,    |  |         |                          |
|  |   writes, aggs)    |  |         |                          |
|  +--------------------+  |         |                          |
+--------------------------+         +--------------------------+
```

The FDW communicates with FairCom through its native c-treeDB C API over TCP. PostgreSQL's optimizer selects the execution strategy: operations that benefit from server-side execution (scans, predicates, aggregates) are pushed down, while operations that benefit from PostgreSQL's algorithms (joins, sorting, window functions) execute locally.

**Connection management:** Connections are pooled per PostgreSQL backend process. When a query reuses a server, the existing connection is returned without re-authentication. Idle connections are cleaned up after the configured timeout (default: 30 minutes).

**Batch fetching:** Rows are fetched in batches to balance throughput and memory. The batch size adapts at runtime based on row width: narrow rows use larger batches (up to 2,000), wide rows use smaller batches (down to 50), targeting approximately 1 MB per batch.

## Troubleshooting

### Common Errors

| Error | Cause | Resolution |
|-------|-------|------------|
| `table not found (error 12)` | `table_name` option is incorrect | Verify the `database.schema.table` format. The database name must match exactly. |
| `authentication failure (error 133)` | Invalid credentials | Check `username` and `password` in the `USER MAPPING`. The default username is `ADMIN`. |
| `could not establish connection` | FairCom server unreachable | Verify `host` and `port` in the `SERVER` definition. Confirm the FairCom server is running and the port is accessible. |
| `option "table_name" is required` | Missing required option | Add `OPTIONS (table_name 'db.schema.table')` to the `CREATE FOREIGN TABLE` statement. |
| `option "faircom_table" is deprecated` | Using legacy option name | Replace `faircom_table` with `table_name` in the foreign table definition. |

### Debug Logging

Debug logging is compiled out by default for zero runtime overhead. To enable it, rebuild the extension with:

```bash
make PG_CPPFLAGS="$(PG_CPPFLAGS) -DFAIRCOM_DEBUG"
```

Then set the PostgreSQL log level:

```sql
SET client_min_messages = debug1;
```

Debug output includes connection lifecycle events, batch fetching, predicate pushdown decisions, and cache hit/miss statistics.

### Verifying Pushdown

Use `EXPLAIN` to confirm operations are pushed to FairCom:

```sql
EXPLAIN (ANALYZE, VERBOSE) SELECT COUNT(*) FROM employees WHERE dept_id = 3;
```

Look for `FairCom Predicates` and `FairCom Pushdown Stats` in the output. If predicates show `predicates=0` when you expect pushdown, check that `enable_pushdown` is `true` on the foreign table and that `faircom_fdw.enable_predicate_pushdown` is `true`.

## Packages

| Format | Platform | File |
|--------|----------|------|
| `.deb` | Debian, Ubuntu | `postgresql-14-faircom-fdw_<version>-1_<arch>.deb` |
| `.rpm` | RHEL, CentOS, Fedora | `postgresql-14-faircom-fdw-<version>-1.el9.<arch>.rpm` |
| Debug (`.ddeb`) | Debian, Ubuntu | `postgresql-14-faircom-fdw-dbgsym_<version>-1_<arch>.ddeb` |
| Debug (`.rpm`) | RHEL, CentOS, Fedora | `postgresql-14-faircom-fdw-debuginfo-<version>-1.el9.<arch>.rpm` |

Supported architectures: `x86_64` (amd64), `aarch64` (arm64)

Download from [GitHub Releases](https://github.com/toddstoffel/postgres-faircom-fdw/releases/latest).

## Building from Source

Requirements: PostgreSQL 14 development headers, GCC, Make.

```bash
cd core
make PG_CONFIG=/usr/lib/postgresql/14/bin/pg_config
sudo make PG_CONFIG=/usr/lib/postgresql/14/bin/pg_config install
```

The Makefile uses PGXS and automatically detects the platform architecture to link against the correct FairCom SDK libraries in `core/faircom_libs/<arch>/`.

To build distribution packages in Docker (no host dependencies required):

```bash
./packaging/build-deb-docker.sh    # Debian/Ubuntu .deb
./packaging/build-rpm-docker.sh    # RHEL/CentOS .rpm
```

## License

Copyright &copy; 2026 FairCom Corporation. All rights reserved.

This software is proprietary and confidential. See [LICENSE](LICENSE) for terms.
