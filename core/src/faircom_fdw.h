/*-------------------------------------------------------------------------
 *
 * faircom_fdw.h
 *		  Foreign-data wrapper for FairCom databases
 *
 * Copyright (c) 2026, FairCom Corporation. All rights reserved.
 * Proprietary and confidential.
 *
 *-------------------------------------------------------------------------
 */
#ifndef FAIRCOM_FDW_H
#define FAIRCOM_FDW_H

#include "postgres.h"
#include "fmgr.h"
#include "foreign/fdwapi.h"
#include "foreign/foreign.h"
#include "optimizer/pathnode.h"
#include "optimizer/planmain.h"
#include "optimizer/restrictinfo.h"
#include "utils/rel.h"
#include "lib/stringinfo.h"
#include "nodes/pg_list.h"
#include "utils/resowner.h"
#include "access/reloptions.h"
#include "catalog/pg_foreign_table.h"
#include "access/xact.h"
#include "access/heapam.h"
#include "utils/numeric.h"
#include "utils/date.h"

/* FairCom CTDB SDK headers */
/* Note: FairCom headers must be included carefully to avoid name conflicts
 * with PostgreSQL. We rename conflicting symbols before including.
 */
#define errmsg faircom_errmsg
#define LOCK FAIRCOM_LOCK
#include "ctdbsdk.h"
#undef errmsg
#undef LOCK

/*
 * Performance instrumentation - Runtime statistics
 * Shared across all modules for tracking global FDW performance
 */
typedef struct FairComGlobalStats
{
	int64		plan_cache_hits;
	int64		plan_cache_misses;
	int64		query_cache_hits;
	int64		query_cache_misses;
	int64		predicate_pushdowns;
	int64		aggregate_pushdowns;
	int64		connections_reused;
	int64		connections_created;
	TimestampTz	last_reset;
} FairComGlobalStats;

/* Global statistics instance - defined in faircom_fdw.c */
extern FairComGlobalStats global_stats;

/*
 * PostgreSQL built-in aggregate function OID ranges
 * These are stable across PG 14.x but should be verified if targeting
 * other major versions. See src/include/catalog/pg_proc.dat in PG source.
 */
#define AGG_COUNT_ANY_OID		2147	/* count("any") */
#define AGG_COUNT_STAR_OID		2803	/* count(*) */
#define AGG_COUNT_MAX_OID		2150	/* upper bound of COUNT family */
#define AGG_SUM_MIN_OID			2108	/* sum(int8) - start of SUM family */
#define AGG_SUM_MAX_OID			2114	/* sum(float8) - end of SUM family */
#define AGG_AVG_MIN_OID			2101	/* avg(int8) - start of AVG family */
#define AGG_AVG_MAX_OID			2106	/* avg(float8) - end of AVG family */
#define AGG_MIN_MIN_OID			2131	/* min(int8) - start of MIN family */
#define AGG_MIN_MAX_OID			2146	/* min(text) - end of MIN family */
#define AGG_MAX_MIN_OID			2115	/* max(int8) - start of MAX family */
#define AGG_MAX_MAX_OID			2130	/* max(text) - end of MAX family */

#define IS_COUNT_AGG(oid)		((oid) == AGG_COUNT_ANY_OID || (oid) == AGG_COUNT_STAR_OID)
#define IS_SUM_AGG(oid)			((oid) >= AGG_SUM_MIN_OID && (oid) <= AGG_SUM_MAX_OID)
#define IS_AVG_AGG(oid)			((oid) >= AGG_AVG_MIN_OID && (oid) <= AGG_AVG_MAX_OID)
#define IS_MIN_AGG(oid)			((oid) >= AGG_MIN_MIN_OID && (oid) <= AGG_MIN_MAX_OID)
#define IS_MAX_AGG(oid)			((oid) >= AGG_MAX_MIN_OID && (oid) <= AGG_MAX_MAX_OID)

/*
 * Conditional debug logging macros
 * Set FAIRCOM_DEBUG to enable debug output in production builds
 * When undefined, debug statements compile to no-ops (zero overhead)
 */
#ifdef FAIRCOM_DEBUG
#define FAIRCOM_DEBUG_LOG(level, ...)  elog(level, __VA_ARGS__)
#else
#define FAIRCOM_DEBUG_LOG(level, ...)  ((void) 0)
#endif

/*
 * FDW-specific information stored in fdw_private lists
 */
typedef struct FairComFdwPlanState
{
	/* Bitmap of columns to fetch */
	Bitmapset  *attrs_used;

	/* Predicates that can be pushed down to FairCom */
	List	   *remote_conds;

	/* Predicates that must be evaluated locally */
	List	   *local_conds;

	/* Index number to use (-1 for sequential scan) */
	int			use_index;

	/* Find mode for index scan */
	CTFIND_MODE	find_mode;

	/* LIMIT/OFFSET values from query (-1 if no LIMIT, 0 if no OFFSET) */
	int64		limit_count;
	int64		offset_count;

	/* Estimated costs */
	Cost		startup_cost;
	Cost		total_cost;
} FairComFdwPlanState;

/*
 * Predicate information for pushdown
 */
typedef struct FairComPredicate
{
	int			attnum;			/* PostgreSQL attribute number */
	const char *attname;		/* Column name */
	CTFIND_MODE	ctfind_mode;	/* FairCom find mode (EQ, GT, etc.) */
	Datum		const_value;	/* Constant value to compare */
	bool		const_isnull;	/* Is constant NULL? */
	Oid			const_type;		/* PostgreSQL type of constant */

	/* NUMERIC conversion cache - avoids repeated numeric_float8 calls in per-row loops */
	float8		cached_float8_value;	/* Cached NUMERIC→float8 conversion result */
	bool		cached_float8_valid;	/* TRUE if cached_float8_value is populated */
} FairComPredicate;

/* Forward declaration */
typedef struct FairComConnection FairComConnection;

/*
 * FDW-specific information for write operations (INSERT/UPDATE/DELETE)
 */
typedef struct FairComModifyState
{
	Relation	rel;
	FairComConnection *conn;	/* Pooled connection (owns session lifecycle) */
	CTHANDLE	hTable;
	CTHANDLE	hRecord;
	char	   *tablename;
	int			operation;		/* CMD_INSERT, CMD_UPDATE, or CMD_DELETE */
	TupleDesc	tupdesc;
	int			num_attrs;
	AttrNumber *target_attrs;	/* Attribute numbers for modify target list */
	AttrNumber	ctidAttno;		/* Attribute number of ctid junk column in planSlot */
} FairComModifyState;

/*
 * FDW-specific information for ForeignScanState
 */
typedef struct FairComScanState
{
	/* Connection */
	FairComConnection *conn;

	/* FairCom handles */
	CTHANDLE	hSession;
	CTHANDLE	hTable;
	CTHANDLE	hRecord;

	/* Scan state */
	bool		first_iteration;
	bool		eof_reached;
	bool		has_predicates;		/* Whether predicates were applied */
	CTFIND_MODE find_mode;			/* Find mode for first record (if has_predicates) */
	List	   *active_predicates;	/* List of FairComPredicate for range checking */

	/* LIMIT/OFFSET support */
	int64		limit_count;		/* LIMIT value (-1 if none) */
	int64		offset_count;		/* OFFSET value (0 if none) */
	int64		rows_returned;		/* Number of rows returned so far */
	int64		rows_skipped;		/* Number of rows skipped for OFFSET */

	/* Aggregate pushdown support */
	bool		is_aggregate_pushdown;	/* True if doing aggregate pushdown */
	Oid			aggregate_fnoid;		/* Which aggregate function (e.g., F_COUNT_ANY) */
	int64		aggregate_result;		/* Result of aggregate (for COUNT) */
	bool		aggregate_returned;		/* Whether we've returned the aggregate row */

	/* Extended aggregate support (SUM, AVG, MIN, MAX) */
	AttrNumber	aggregate_attnum;		/* Column number being aggregated (0 for COUNT(*)) */
	Oid			aggregate_type;			/* Data type of aggregated column */
	int64		aggregate_count;		/* Row count for AVG computation */
	int64		aggregate_sum_int;		/* Integer sum accumulator */
	double		aggregate_sum;			/* Float/numeric sum accumulator */
	double		aggregate_min;			/* MIN accumulator */
	double		aggregate_max;			/* MAX accumulator */
	bool		aggregate_isnull;		/* Whether aggregate result is NULL */

	/* Connection information */
	char	   *servername;
	char	   *tablename;

	/* Plan information */
	FairComFdwPlanState *plan;

	/* Batch fetching - store Datum arrays instead of TupleTableSlots */
	Datum	  **batch_values;		/* Array of Datum arrays */
	bool	  **batch_nulls;		/* Array of bool arrays */
	CTROWID	   *batch_rowids;		/* Array of FairCom rowids for UPDATE/DELETE */
	int			batch_size;			/* Number of rows currently in batch */
	int			batch_position;		/* Current position in batch */
	int			batch_natts;		/* Number of attributes per row */
	int		   *field_nums;			/* Cached FairCom field numbers (one per attribute) */
	bool		field_nums_cached;	/* Whether field numbers have been cached yet */

	/* Batch optimization tracking */
	int			current_batch_limit;	/* Dynamic batch size (adapts to row size) */
	int			avg_row_size;			/* Rolling average row size in bytes */
	int			record_size;			/* Actual record size from ctdbGetRecordSize() */
	int			optimal_batch_size;		/* Calculated optimal batch size for this table */
	int			total_batches;			/* Total number of batches fetched */
	int			total_rows_fetched;		/* Total rows across all batches */

	/* Target list for projection */
	List	   *target_attrs;
} FairComScanState;

/*
 * Connection cache entry
 */
typedef struct FairComConnection
{
	CTHANDLE	hSession;
	CTHANDLE	hDatabase;
	char	   *servername;
	char	   *username;
	bool		in_transaction;
	ResourceOwner owner;
	int			ref_count;		/* Number of active scans using this connection */
	int			reuse_count;	/* Total number of times connection was reused */
	TimestampTz	idle_since;		/* When connection became idle (ref_count=0) */
	bool		is_valid;		/* FALSE if connection failed validation */
	struct FairComConnection *next;
} FairComConnection;

/*
 * Server and table options
 */
typedef struct FairComServerOptions
{
	char	   *host;
	int			port;
	char	   *database;
	int			connect_timeout;
} FairComServerOptions;

typedef struct FairComTableOptions
{
	char	   *table_name;
	char	   *use_index;
	bool		enable_pushdown;
} FairComTableOptions;

/* Function declarations */

/* connection.c */
extern FairComConnection *faircom_get_connection(ForeignServer *server,
												  UserMapping *user);
extern void faircom_release_connection(FairComConnection *conn);
extern void faircom_cleanup_connections(void);
extern void faircom_xact_callback(XactEvent event, void *arg);
extern void faircom_cleanup_idle_connections(void);
extern void faircom_force_release_connection(FairComConnection *conn);
extern void faircom_begin_transaction(FairComConnection *conn);

/* schema.c */
extern List *faircom_import_foreign_schema(ImportForeignSchemaStmt *stmt,
										   Oid serverOid);
extern Oid faircom_type_to_pg_type(CTDBTYPE ctdbtype);

/* predicate.c - Predicate pushdown support */
extern bool faircom_is_foreign_expr(PlannerInfo *root,
									RelOptInfo *baserel,
									Expr *expr);
extern void faircom_classify_conditions(PlannerInfo *root,
										 RelOptInfo *baserel,
										 List **remote_conds,
										 List **local_conds);
extern FairComPredicate *faircom_extract_predicate(Expr *expr, TupleDesc tupdesc);
extern List *faircom_extract_predicates_from_expr(Expr *expr, TupleDesc tupdesc);
extern CTFIND_MODE faircom_apply_predicates(CTHANDLE hTable,
											 CTHANDLE hRecord,
											 List *predicates,
											 TupleDesc tupdesc);
extern bool faircom_check_predicate_match(CTHANDLE hRecord, List *predicates);
extern bool faircom_should_stop_range_scan(CTHANDLE hRecord, List *predicates, CTFIND_MODE find_mode);
extern int faircom_fetch_in_list_batch(CTHANDLE hTable,
									   CTHANDLE hRecord,
									   List *predicates,
									   TupleDesc tupdesc,
									   Datum **batch_values,
									   bool **batch_nulls,
									   int max_batch);

/* query.c */
extern CTFIND_MODE faircom_operator_to_find_mode(Oid opno);

/* types.c */
extern Datum faircom_to_datum(CTHANDLE hRecord, int fieldNum,
						  Oid pgType, int32 typmod, bool *isNull);
extern void faircom_datum_to_field(Datum value, Oid pgType,
								   CTHANDLE hRecord, int fieldNum);

/* utils.c */
extern void faircom_check_result(CTDBRET retval, CTHANDLE handle,
								 const char *operation);
extern const char *faircom_get_error_message(CTHANDLE handle);
extern void faircom_parse_server_options(ForeignServer *server,
										 FairComServerOptions *opts);
extern void faircom_parse_table_options(ForeignTable *table,
										FairComTableOptions *opts);

/* Batch fetch size for performance optimization
 * Increased to 500 for better throughput (was 100)
 * Can be overridden per query based on row size
 */
#define FAIRCOM_FETCH_BATCH_SIZE 500
#define FAIRCOM_MIN_BATCH_SIZE 50
#define FAIRCOM_MAX_BATCH_SIZE 2000

/* Adaptive batch sizing thresholds (bytes per row) */
#define FAIRCOM_SMALL_ROW_THRESHOLD 100   /* < 100 bytes = small row */
#define FAIRCOM_LARGE_ROW_THRESHOLD 1000  /* > 1KB = large row */

/* Target memory per batch for optimal sizing (1MB) */
#define FAIRCOM_TARGET_BATCH_MEMORY (1024 * 1024)  /* 1 MB per batch */

/* Default connection timeout */
#define FAIRCOM_DEFAULT_TIMEOUT 30

/* Query result cache structures */
typedef struct QueryCacheEntry
{
	char *cache_key;           /* Unique key: table_name + predicates */
	int64 result_value;        /* Cached result (for COUNT) */
	TimestampTz cached_at;     /* When was this cached */
	int ttl_seconds;           /* Time to live */
	struct QueryCacheEntry *next;  /* Linked list */
} QueryCacheEntry;

typedef struct QueryCache
{
	QueryCacheEntry *entries;  /* Head of linked list */
	int entry_count;           /* Number of entries */
	int max_entries;           /* Max cache size */
} QueryCache;

/* cache.c - Query result caching */
extern void faircom_cache_init(void);
extern void faircom_cache_cleanup(void);
extern bool faircom_cache_get_count(const char *table_name, const char *predicates,
                                     int64 *result, int ttl_seconds);
extern void faircom_cache_set_count(const char *table_name, const char *predicates,
                                     int64 result, int ttl_seconds);
extern void faircom_cache_invalidate(const char *table_name);
extern void faircom_cache_clear_expired(void);

/* Default cache TTL in seconds - increased to 300 for better hit rates */
#define FAIRCOM_CACHE_DEFAULT_TTL 300
#define FAIRCOM_CACHE_MAX_ENTRIES 1000

/* Plan cache structures - cache table metadata and planning decisions */
typedef struct PlanCacheIndexInfo
{
	int index_number;          /* FairCom index number */
	char *index_name;          /* Index name */
	int field_count;           /* Number of fields in index */
	char **field_names;        /* Field names in index */
	struct PlanCacheIndexInfo *next;
} PlanCacheIndexInfo;

typedef struct PlanCacheEntry
{
	Oid foreign_table_oid;     /* Foreign table OID (key) */
	char *table_name;          /* FairCom table name */
	int column_count;          /* Number of columns */
	char **column_names;       /* Column names */
	Oid *column_types;         /* Column PostgreSQL types */
	PlanCacheIndexInfo *indexes; /* Linked list of indexes */
	CTUINT64 row_count;        /* Cached row count */
	TimestampTz cached_at;     /* When was this cached */
	TimestampTz last_accessed; /* When was this last accessed (for LRU) */
	bool is_valid;             /* Still valid? */
	struct PlanCacheEntry *next;
} PlanCacheEntry;

typedef struct PlanCache
{
	PlanCacheEntry *entries;   /* Head of linked list */
	int entry_count;           /* Number of cached plans */
	int max_entries;           /* Max cache size */
} PlanCache;

/* plan_cache.c - Query plan caching */
extern void faircom_plan_cache_init(void);
extern void faircom_plan_cache_cleanup(void);
extern PlanCacheEntry *faircom_plan_cache_get(Oid foreign_table_oid);
extern void faircom_plan_cache_set(Oid foreign_table_oid, const char *table_name,
								   int column_count, char **column_names,
								   Oid *column_types, PlanCacheIndexInfo *indexes,
								   CTUINT64 row_count);
extern void faircom_plan_cache_invalidate(Oid foreign_table_oid);
extern void faircom_plan_cache_clear_all(void);

/* Plan cache configuration */
#define FAIRCOM_PLAN_CACHE_MAX_ENTRIES 500  /* Increased from 100 for better hit rate */
#define FAIRCOM_PLAN_CACHE_TTL 300  /* 5 minutes */

#endif   /* FAIRCOM_FDW_H */
