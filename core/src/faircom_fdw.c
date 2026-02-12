/*-------------------------------------------------------------------------
 *
 * faircom_fdw.c
 *		  Foreign-data wrapper for FairCom databases - Main entry point
 *
 * This module implements the PostgreSQL FDW API callbacks and coordinates
 * the other subsystems (connection management, query translation, etc.)
 *
 * Copyright (c) 2026, FairCom Corporation
 *
 *-------------------------------------------------------------------------
 */
#include "postgres.h"
#include "faircom_fdw.h"

#include <math.h>
#include <ctype.h>

#include "access/htup_details.h"
#include "access/sysattr.h"
#include "access/table.h"
#include "catalog/pg_type.h"
#include "commands/defrem.h"
#include "commands/explain.h"
#include "executor/executor.h"
#include "foreign/fdwapi.h"
#include "foreign/foreign.h"
#include "utils/rel.h"
#include "funcapi.h"
#include "miscadmin.h"
#include "nodes/makefuncs.h"
#include "nodes/nodeFuncs.h"
#include "optimizer/cost.h"
#include "optimizer/pathnode.h"
#include "optimizer/planmain.h"
#include "optimizer/restrictinfo.h"
#include "optimizer/optimizer.h"
#include "parser/parsetree.h"
#include "utils/builtins.h"
#include "utils/date.h"
#include "utils/datetime.h"
#include "utils/guc.h"
#include "utils/lsyscache.h"
#include "utils/memutils.h"
#include "utils/numeric.h"
#include "utils/rel.h"
#include "utils/timestamp.h"
#include "storage/latch.h"
#include "storage/shm_toc.h"
#include "optimizer/appendinfo.h"

PG_MODULE_MAGIC;

/*
 * GUC parameters - User-configurable settings
 */
static int faircom_cache_ttl = 300;                    /* Query cache TTL (seconds) */
static int faircom_plan_cache_size = 500;              /* Max plan cache entries */
static int faircom_default_batch_size = 500;           /* Default fetch batch size */
static int faircom_connection_idle_timeout = 1800;     /* Connection idle timeout (seconds) */
static bool faircom_enable_aggregate_pushdown = true;  /* Enable aggregate pushdown */
static bool faircom_enable_predicate_pushdown = true;  /* Enable predicate pushdown */

/*
 * Global performance statistics instance
 */
FairComGlobalStats global_stats = {0};

/* FDW callback functions */
static void faircomGetForeignRelSize(PlannerInfo *root,
									 RelOptInfo *baserel,
									 Oid foreigntableid);

static void faircomGetForeignPaths(PlannerInfo *root,
								   RelOptInfo *baserel,
								   Oid foreigntableid);

static ForeignScan *faircomGetForeignPlan(PlannerInfo *root,
										  RelOptInfo *baserel,
										  Oid foreigntableid,
										  ForeignPath *best_path,
										  List *tlist,
										  List *scan_clauses,
										  Plan *outer_plan);

static void faircomGetForeignUpperPaths(PlannerInfo *root,
										UpperRelationKind stage,
										RelOptInfo *input_rel,
										RelOptInfo *output_rel,
										void *extra);

static void faircomBeginForeignScan(ForeignScanState *node,
									int eflags);

static TupleTableSlot *faircomIterateForeignScan(ForeignScanState *node);

static void faircomReScanForeignScan(ForeignScanState *node);

static void faircomEndForeignScan(ForeignScanState *node);

static void faircomExplainForeignScan(ForeignScanState *node,
									  ExplainState *es);

static bool faircomAnalyzeForeignTable(Relation relation,
									   AcquireSampleRowsFunc *func,
									   BlockNumber *totalpages);

static List *faircomImportForeignSchema(ImportForeignSchemaStmt *stmt,
										Oid serverOid);

/* Parallel scan functions */
static bool faircomIsForeignScanParallelSafe(PlannerInfo *root,
											 RelOptInfo *rel,
											 RangeTblEntry *rte);

static Size faircomEstimateDSMForeignScan(ForeignScanState *node,
										   ParallelContext *pcxt);

static void faircomInitializeDSMForeignScan(ForeignScanState *node,
											 ParallelContext *pcxt,
											 void *coordinate);

static void faircomReInitializeDSMForeignScan(ForeignScanState *node,
											   ParallelContext *pcxt,
											   void *coordinate);

static void faircomInitializeWorkerForeignScan(ForeignScanState *node,
												shm_toc *toc,
												void *coordinate);

static void faircomShutdownForeignScan(ForeignScanState *node);

/* Write operation functions */
static void faircomAddForeignUpdateTargets(PlannerInfo *root,
										   Index rtindex,
										   RangeTblEntry *target_rte,
										   Relation target_relation);

static List *faircomPlanForeignModify(PlannerInfo *root,
									  ModifyTable *plan,
									  Index resultRelation,
									  int subplan_index);

static void faircomBeginForeignModify(ModifyTableState *mtstate,
									  ResultRelInfo *resultRelInfo,
									  List *fdw_private,
									  int subplan_index,
									  int eflags);

static TupleTableSlot *faircomExecForeignInsert(EState *estate,
												ResultRelInfo *resultRelInfo,
												TupleTableSlot *slot,
												TupleTableSlot *planSlot);

static TupleTableSlot *faircomExecForeignUpdate(EState *estate,
												ResultRelInfo *resultRelInfo,
												TupleTableSlot *slot,
												TupleTableSlot *planSlot);

static TupleTableSlot *faircomExecForeignDelete(EState *estate,
												ResultRelInfo *resultRelInfo,
												TupleTableSlot *slot,
												TupleTableSlot *planSlot);

static void faircomEndForeignModify(EState *estate,
									ResultRelInfo *resultRelInfo);

static int faircomIsForeignRelUpdatable(Relation rel);

/*
 * Library load function
 */
extern void _PG_init(void);

void
_PG_init(void)
{
	/* Register transaction callback for connection cleanup */
	RegisterXactCallback(faircom_xact_callback, NULL);

	/* Initialize plan cache */
	faircom_plan_cache_init();

	/* Register custom GUC parameters */
	DefineCustomIntVariable("faircom_fdw.cache_ttl",
							"Query result cache TTL in seconds.",
							"Affects how long COUNT and aggregate results are cached.",
							&faircom_cache_ttl,
							300,		/* boot_val */
							0,			/* min_val */
							3600,		/* max_val */
							PGC_USERSET,
							0,
							NULL,
							NULL,
							NULL);

	DefineCustomIntVariable("faircom_fdw.plan_cache_size",
							"Maximum number of table metadata plans to cache.",
							"Higher values use more memory but improve planning for many tables.",
							&faircom_plan_cache_size,
							500,		/* boot_val */
							10,			/* min_val */
							10000,		/* max_val */
							PGC_SUSET,
							0,
							NULL,
							NULL,
							NULL);

	DefineCustomIntVariable("faircom_fdw.batch_size",
							"Default number of rows to fetch per batch.",
							"Larger values improve throughput but use more memory.",
							&faircom_default_batch_size,
							500,		/* boot_val */
							50,			/* min_val */
							2000,		/* max_val */
							PGC_USERSET,
							0,
							NULL,
							NULL,
							NULL);

	DefineCustomIntVariable("faircom_fdw.connection_idle_timeout",
							"Seconds before idle connections are closed.",
							"Longer timeouts improve connection reuse.",
							&faircom_connection_idle_timeout,
							1800,		/* boot_val (30 minutes) */
							60,			/* min_val */
							86400,		/* max_val (24 hours) */
							PGC_USERSET,
							0,
							NULL,
							NULL,
							NULL);

	DefineCustomBoolVariable("faircom_fdw.enable_aggregate_pushdown",
							 "Enable pushdown of COUNT/SUM/AVG/MIN/MAX to FairCom.",
							 "Reduces network traffic by computing aggregates remotely.",
							 &faircom_enable_aggregate_pushdown,
							 true,		/* boot_val */
							 PGC_USERSET,
							 0,
							 NULL,
							 NULL,
							 NULL);

	DefineCustomBoolVariable("faircom_fdw.enable_predicate_pushdown",
							 "Enable pushdown of WHERE conditions to FairCom.",
							 "Reduces rows transferred by filtering remotely.",
							 &faircom_enable_predicate_pushdown,
							 true,		/* boot_val */
							 PGC_USERSET,
							 0,
							 NULL,
							 NULL,
							 NULL);
}

/*
 * Extension handler function
 */
PG_FUNCTION_INFO_V1(faircom_fdw_handler);

Datum
faircom_fdw_handler(PG_FUNCTION_ARGS)
{
	FdwRoutine *routine = makeNode(FdwRoutine);

	/* Planning functions */
	routine->GetForeignRelSize = faircomGetForeignRelSize;
	routine->GetForeignPaths = faircomGetForeignPaths;
	routine->GetForeignPlan = faircomGetForeignPlan;
	routine->GetForeignUpperPaths = faircomGetForeignUpperPaths;

	/* Scanning functions */
	routine->BeginForeignScan = faircomBeginForeignScan;
	routine->IterateForeignScan = faircomIterateForeignScan;
	routine->ReScanForeignScan = faircomReScanForeignScan;
	routine->EndForeignScan = faircomEndForeignScan;

	/* Support functions */
	routine->ExplainForeignScan = faircomExplainForeignScan;
	routine->AnalyzeForeignTable = faircomAnalyzeForeignTable;
	routine->ImportForeignSchema = faircomImportForeignSchema;

	/* Parallel scan support */
	routine->IsForeignScanParallelSafe = faircomIsForeignScanParallelSafe;
	routine->EstimateDSMForeignScan = faircomEstimateDSMForeignScan;
	routine->InitializeDSMForeignScan = faircomInitializeDSMForeignScan;
	routine->ReInitializeDSMForeignScan = faircomReInitializeDSMForeignScan;
	routine->InitializeWorkerForeignScan = faircomInitializeWorkerForeignScan;
	routine->ShutdownForeignScan = faircomShutdownForeignScan;

	/*
	 * Async execution NOT supported in this version
	 * PostgreSQL 15 async FDW API is complex and requires non-blocking I/O
	 * FairCom CTDB SDK uses blocking calls, so async would provide no benefit
	 * TODO: Future - investigate if FairCom SDK can support true async I/O
	 */

	/* Write operations */
	routine->IsForeignRelUpdatable = faircomIsForeignRelUpdatable;
	routine->AddForeignUpdateTargets = faircomAddForeignUpdateTargets;
	routine->PlanForeignModify = faircomPlanForeignModify;
	routine->BeginForeignModify = faircomBeginForeignModify;
	routine->ExecForeignInsert = faircomExecForeignInsert;
	routine->ExecForeignUpdate = faircomExecForeignUpdate;
	routine->ExecForeignDelete = faircomExecForeignDelete;
	routine->EndForeignModify = faircomEndForeignModify;

	PG_RETURN_POINTER(routine);
}

/*
 * faircomGetForeignRelSize
 *		Obtain relation size estimates for a foreign table
 *		Uses plan cache to reduce repeated metadata lookups
 */
static void
faircomGetForeignRelSize(PlannerInfo *root,
						 RelOptInfo *baserel,
						 Oid foreigntableid)
{
	ForeignTable *table;
	ForeignServer *server;
	UserMapping *user;
	FairComConnection *conn;
	CTHANDLE hTable;
	char *tablename;
	ListCell *lc;
	CTDBRET ret;
	CTUINT64 row_count;
	PlanCacheEntry *cached_plan;

	/* Initialize variables */
	hTable = NULL;
	tablename = NULL;
	row_count = 0;

	/* Check plan cache for existing metadata */
	cached_plan = faircom_plan_cache_get(foreigntableid);
	if (cached_plan)
	{
		/* Use cached row count - skip FairCom connection entirely */
		baserel->rows = (cached_plan->row_count > 0) ? cached_plan->row_count : 1000;
		baserel->tuples = baserel->rows;

		/* Track plan cache hit */
		global_stats.plan_cache_hits++;

		FAIRCOM_DEBUG_LOG(DEBUG1, "GetForeignRelSize: Plan cache HIT for OID %u - using cached row count %lu (skipping FairCom connection)",
			 foreigntableid, (unsigned long)cached_plan->row_count);
		return;
	}

	/* Track plan cache miss */
	global_stats.plan_cache_misses++;

	FAIRCOM_DEBUG_LOG(DEBUG1, "GetForeignRelSize: Plan cache MISS for OID %u - will query FairCom",
		 foreigntableid);

	if (!OidIsValid(foreigntableid))
	{
		ereport(ERROR, (errcode(ERRCODE_INTERNAL_ERROR),
				errmsg("faircomGetForeignRelSize called with invalid OID: %u", foreigntableid)));
	}

	if (foreigntableid == 1)
	{
		ereport(ERROR, (errcode(ERRCODE_INTERNAL_ERROR),
				errmsg("faircomGetForeignRelSize called with OID=1 (range table index instead of actual OID!)")));
	}

	/* Get foreign table, server, and user mapping */
	table = GetForeignTable(foreigntableid);
	server = GetForeignServer(table->serverid);
	user = GetUserMapping(GetUserId(), server->serverid);

	/* Extract table name from options */
	foreach(lc, table->options)
	{
		DefElem *def = (DefElem *) lfirst(lc);
		if (strcmp(def->defname, "table_name") == 0)
		{
			tablename = defGetString(def);
			break;
		}
	}

	if (!tablename)
		tablename = get_rel_name(foreigntableid);

	/* FairCom doesn't provide accurate row counts (ctdbGetRecordCount returns 0)
	 * Opening table during planning adds 200-300ms overhead
	 * Use a reasonable default estimate and cache it for subsequent queries */
	row_count = 1000;  /* Default estimate for foreign tables */

	/* Cache the estimate to avoid repeated planning overhead */
	faircom_plan_cache_set(foreigntableid, tablename,
						   0, NULL, NULL, NULL, row_count);

	FAIRCOM_DEBUG_LOG(DEBUG1, "GetForeignRelSize: Using default row estimate %lu for table %s (FairCom doesn't provide actual counts)",
		 (unsigned long)row_count, tablename);

	/* Set row estimate */
	baserel->rows = row_count;
	baserel->tuples = baserel->rows;
}

/*
 * faircomGetForeignPaths
 *		Create possible access paths for a scan on the foreign table
 */
static void
faircomGetForeignPaths(PlannerInfo *root,
						   RelOptInfo *baserel,
						   Oid foreigntableid)
{
	List *remote_conds = NIL;
	List *local_conds = NIL;
	Cost startup_cost = 100;
	Cost run_cost;
	Cost total_cost;
	FairComFdwPlanState *plan_state;
	double selectivity = 1.0;

	/* Classify conditions into remote (pushable) and local */
	faircom_classify_conditions(root, baserel, &remote_conds, &local_conds);

	/* Calculate costs based on predicate pushdown */
	if (remote_conds != NIL)
	{
		int num_remote = list_length(remote_conds);

		/* Estimate selectivity based on number of predicates */
		/* Each predicate assumed to filter ~50% of rows */
		/* Multiple predicates: combined selectivity = 0.5^n */
		selectivity = pow(0.5, num_remote);

		/* Clamp selectivity to reasonable bounds */
		if (selectivity < 0.01)
			selectivity = 0.01;  /* At least 1% of rows */

		/* With pushdown: only fetch filtered rows */
		double estimated_filtered_rows = baserel->rows * selectivity;
		run_cost = estimated_filtered_rows * 0.01;

		FAIRCOM_DEBUG_LOG(DEBUG1, "FairCom: %d remote condition(s), estimated selectivity %.3f, filtered rows: %.0f",
			 num_remote, selectivity, estimated_filtered_rows);
	}
	else
	{
		/* No pushdown: fetch all rows */
		run_cost = baserel->rows * 0.01;

		FAIRCOM_DEBUG_LOG(DEBUG1, "FairCom: no remote conditions, using sequential scan");
	}

	total_cost = startup_cost + run_cost;

	/* Extract LIMIT from query for early scan termination.
	 * When OFFSET is also present, we fetch LIMIT+OFFSET rows so PostgreSQL
	 * can apply OFFSET on top of our results without double-skipping.
	 * OFFSET is never handled by the FDW — PostgreSQL handles it natively.
	 *
	 * LIMIT pushdown is ONLY safe when there is no ORDER BY. If PostgreSQL
	 * must sort the full result before applying LIMIT, we must return all rows.
	 */
	int64 limit_count = -1;  /* -1 means no LIMIT */
	int64 offset_count = 0;  /* Always 0 — OFFSET handled by PostgreSQL */

	if (root->parse->limitCount && root->parse->sortClause == NIL)
	{
		if (IsA(root->parse->limitCount, Const))
		{
			Const *limit_const = (Const *) root->parse->limitCount;
			if (!limit_const->constisnull)
			{
				limit_count = DatumGetInt64(limit_const->constvalue);

				/* When OFFSET is present, the FDW must return LIMIT+OFFSET rows
				 * so PostgreSQL can skip OFFSET rows and still have LIMIT remaining.
				 * Example: LIMIT 100 OFFSET 1000 → FDW returns 1100 rows,
				 * PostgreSQL skips 1000 and keeps 100.
				 */
				if (root->parse->limitOffset &&
					IsA(root->parse->limitOffset, Const))
				{
					Const *offset_const = (Const *) root->parse->limitOffset;
					if (!offset_const->constisnull)
						limit_count += DatumGetInt64(offset_const->constvalue);
				}

				FAIRCOM_DEBUG_LOG(DEBUG1, "FairCom FDW: LIMIT pushdown - will fetch at most %ld rows",
					 (long)limit_count);
			}
		}
	}

	/* Create plan state to pass to execution phase */
	plan_state = (FairComFdwPlanState *) palloc0(sizeof(FairComFdwPlanState));
	plan_state->remote_conds = remote_conds;
	plan_state->local_conds = local_conds;
	plan_state->use_index = -1;  /* Index selection happens automatically in predicate application */
	plan_state->limit_count = limit_count;
	plan_state->offset_count = offset_count;
	plan_state->startup_cost = startup_cost;
	plan_state->total_cost = total_cost;

	/* Create foreign path with plan state */
	ForeignPath *path = create_foreignscan_path(root, baserel,
												NULL,	/* default pathtarget */
												baserel->rows * selectivity,  /* Adjusted row estimate */
												startup_cost,
												total_cost,
												NIL,	/* no pathkeys */
												NULL,	/* no outer rel */
												NULL,	/* no extra plan */
												list_make1(plan_state)); /* fdw_private */

	/* Parallel scan is disabled; keep path non-parallel-safe */
	path->path.parallel_safe = false;

	add_path(baserel, (Path *) path);
}

/*
 * faircomGetForeignPlan
 *		Create a ForeignScan plan node for scanning the foreign table
 */
static ForeignScan *
faircomGetForeignPlan(PlannerInfo *root,
					  RelOptInfo *baserel,
					  Oid foreigntableid,
					  ForeignPath *best_path,
					  List *tlist,
					  List *scan_clauses,
					  Plan *outer_plan)
{
	List *fdw_private = NIL;
	FairComFdwPlanState *plan_state = NULL;
	List *remote_exprs = NIL;
	List *local_exprs = NIL;
	List *fdw_scan_tlist = NIL;

	FAIRCOM_DEBUG_LOG(DEBUG1, "FairCom FDW: GetForeignPlan called");
	FAIRCOM_DEBUG_LOG(DEBUG1,  "  baserel->reloptkind: %d", baserel->reloptkind);
	FAIRCOM_DEBUG_LOG(DEBUG1, "  outer_plan: %p", outer_plan);
	FAIRCOM_DEBUG_LOG(DEBUG1, "  best_path->path.pathtype: %d", best_path->path.pathtype);
	FAIRCOM_DEBUG_LOG(DEBUG1, "  tlist length: %d", list_length(tlist));
	FAIRCOM_DEBUG_LOG(DEBUG1, "  scan_clauses length: %d", list_length(scan_clauses));
	FAIRCOM_DEBUG_LOG(DEBUG1, "  best_path->fdw_private length: %d", list_length(best_path->fdw_private));

	if (tlist != NIL)
	{
		ListCell *lc;
		int i = 0;
		foreach(lc, tlist)
		{
			TargetEntry *tle = (TargetEntry *) lfirst(lc);
			FAIRCOM_DEBUG_LOG(DEBUG1, "  tlist[%d]: resno=%d, resjunk=%d, expr_type=%d",
				 i++, tle->resno, tle->resjunk, nodeTag(tle->expr));

			/* For Var expressions, log varno and varattno */
			if (IsA(tle->expr, Var))
			{
				Var *var = (Var *) tle->expr;
				FAIRCOM_DEBUG_LOG(DEBUG1, "    Var: varno=%d, varattno=%d, vartype=%d",
					 var->varno, var->varattno, var->vartype);
			}
		}
	}

	/* Check if this is an upper relation (aggregate pushdown)
	 * Following postgres_fdw pattern: check fdw_private for our marker
	 */
	if (baserel->reloptkind == RELOPT_UPPER_REL &&
		best_path->fdw_private != NIL &&
		list_length(best_path->fdw_private) >= 2 &&
		IsA(linitial(best_path->fdw_private), String))
	{
		char *marker = strVal(linitial(best_path->fdw_private));

		if (strcmp(marker, "aggregate_pushdown") == 0)
		{
			Oid agg_fnoid = intVal(lsecond(best_path->fdw_private));
			List *fdw_scan_tlist;
			List *fdw_exprs;
			TargetEntry *tle;
			Var *var;
			Expr *expr;

			FAIRCOM_DEBUG_LOG(DEBUG1, "FairCom FDW: Creating plan for aggregate pushdown (fnoid=%u)", agg_fnoid);

			/* For aggregate pushdown, create simple fdw_private:
			 * [0] = makeString("aggregate_pushdown")
			 * [1] = makeInteger(agg_fnoid)
			 * Note: We don't store foreigntableid separately - will extract from input rel
			 */
			fdw_private = list_make2(makeString("aggregate_pushdown"),
									 makeInteger(agg_fnoid));

			/* Build fdw_scan_tlist: a simple Var representing the bigint result
			 * This is what the ForeignScan will actually output - a plain column,
			 * not an Aggref. PostgreSQL will then project the Aggref from this Var.
			 * Use OUTER_VAR to reference the result from the outer (input) plan.
			 */
			var = makeVar(OUTER_VAR,  /* varno for outer plan reference */
						  1,          /* attno - first column */
						  INT8OID,    /* COUNT returns bigint */
						  -1,         /* typmod */
						  InvalidOid, /* collation */
						  0);         /* varlevelsup */

			tle = makeTargetEntry((Expr *) var,
								  1,      /* resno */
								  NULL,   /* resname */
								  false); /* resjunk */

			fdw_scan_tlist = list_make1(tle);

			/* Build fdw_exprs - the constant bigint 0 placeholder that ForeignScan will replace */
			expr = (Expr *) makeConst(INT8OID,  /* consttype */
									   -1,        /* consttypmod */
									   InvalidOid, /* constcollid */
									   8,         /* constlen - bigint is 8 bytes */
									   Int64GetDatum(0),  /* constvalue - placeholder */
									   false,     /* constisnull */
									   true);     /* constbyval */

			fdw_exprs = list_make1(expr);

			/* Following postgres_fdw: for upper relations, return ForeignScan with:
			 * - tlist = target list (with Aggref) for the plan node
			 * - scan_relid = 0 (not scanning a single relation)
			 * - fdw_exprs = expressions the ForeignScan will compute
			 * - fdw_scan_tlist = what the scan outputs
			 * - fdw_private contains our aggregate info
			 */
			return make_foreignscan(tlist,
									NIL,  /* no local quals for aggregates */
									0,    /* scan_relid = 0 for upper rels */
									fdw_exprs,  /* Expressions to be computed */
									fdw_private,
									fdw_scan_tlist,  /* Simple Var, not Aggref */
									NIL,  /* no remote quals */
									outer_plan);
		}
	}

	/* For UPDATE/DELETE, rely on row identity registered by AddForeignUpdateTargets.
	 * The executor will fetch the junk ctid attribute via ExecGetJunkAttribute.
	 */

	if (scan_clauses != NIL)
	{
		ListCell *lc_debug;
		int clause_idx = 0;
		foreach(lc_debug, scan_clauses)
		{
			RestrictInfo *ri = (RestrictInfo *) lfirst(lc_debug);
			FAIRCOM_DEBUG_LOG(DEBUG1, "  scan_clauses[%d]: node_type=%d", clause_idx++, nodeTag(ri->clause));
		}
	}
	else
	{
		FAIRCOM_DEBUG_LOG(DEBUG1, "  scan_clauses: EMPTY (this is THE PROBLEM for UPDATE/DELETE!)");
	}

	/* Extract plan state from best_path if available */
	if (best_path->fdw_private != NIL)
		plan_state = (FairComFdwPlanState *) linitial(best_path->fdw_private);

	/* Separate scan_clauses into remote and local based on plan_state */
	if (plan_state && plan_state->remote_conds)
	{
		ListCell *lc;
		foreach(lc, scan_clauses)
		{
			RestrictInfo *ri = lfirst(lc);
			if (list_member_ptr(plan_state->remote_conds, ri))
				remote_exprs = lappend(remote_exprs, ri->clause);
			else
				local_exprs = lappend(local_exprs, ri->clause);
		}

		FAIRCOM_DEBUG_LOG(DEBUG1, "FairCom plan: %d remote exprs, %d local exprs",
			 list_length(remote_exprs), list_length(local_exprs));
	}
	else
	{
		/* No pushdown, all clauses are local */
		local_exprs = extract_actual_clauses(scan_clauses, false);
	}

	/* Store information in fdw_private:
	 * [0] = foreigntableid (Integer)
	 * [1] = remote_exprs (List of Expr)
	 * [2] = local_exprs (List of Expr)
	 * [3] = limit_count (Integer) - -1 if no LIMIT
	 * [4] = offset_count (Integer) - 0 if no OFFSET
	 */
	fdw_private = list_make5(makeInteger(foreigntableid),
							 remote_exprs,
							 local_exprs,
							 makeInteger(plan_state ? plan_state->limit_count : -1),
							 makeInteger(plan_state ? plan_state->offset_count : 0));
	/*
	 * For simple SELECT scans, scan_relid is the base relation and fdw_scan_tlist is NIL.
	 * For aggregate pushdown, scan_relid is 0 and fdw_scan_tlist specifies output.
	 * For UPDATE/DELETE, we keep scan_relid as the base relation and provide fdw_scan_tlist.
	 */
	return make_foreignscan(tlist,
							local_exprs,  /* Only local clauses checked by PostgreSQL */
							baserel->relid,
							NIL,	/* no expressions to evaluate */
							fdw_private,	/* Contains foreigntableid */
							fdw_scan_tlist,	/* NIL for SELECT, aggregate for pushdown, columns+ctid for UPDATE/DELETE */
							NIL,	/* no remote quals */
							outer_plan);
}

/*
 * faircomGetForeignUpperPaths
 *		Add paths for post-scan/join processing (aggregates, ORDER BY, LIMIT)
 */
static void
faircomGetForeignUpperPaths(PlannerInfo *root,
							UpperRelationKind stage,
							RelOptInfo *input_rel,
							RelOptInfo *output_rel,
							void *extra)
{
	GroupPathExtraData *gp_extra;
	ForeignPath *foreign_path;
	ForeignPath *upper_path;
	Cost startup_cost = 100;
	Cost total_cost = 100;
	PathTarget *target;
	List *fdw_private;
	bool can_pushdown = false;
	Aggref *aggref = NULL;

	FAIRCOM_DEBUG_LOG(DEBUG1, "FairCom FDW: GetForeignUpperPaths called - stage=%d", (int)stage);
	FAIRCOM_DEBUG_LOG(DEBUG1, "  input_rel: reloptkind=%d", input_rel->reloptkind);
	FAIRCOM_DEBUG_LOG(DEBUG1, "  output_rel: rows=%.0f", output_rel->rows);

	/* Only handle GROUP_AGG stage for aggregate pushdown */
	if (stage != UPPERREL_GROUP_AGG)
	{
		FAIRCOM_DEBUG_LOG(DEBUG1, "FairCom FDW: Not GROUP_AGG stage (stage=%d, UPPERREL_GROUP_AGG=%d)",
			 (int)stage, (int)UPPERREL_GROUP_AGG);
		return;
	}

	/* Temporary: disable aggregate pushdown at upper relation level
	 * The "Aggref in non-Agg node" error indicates we need a different approach.
	 * postgres_fdw avoids this by using partial aggregation - foreign server
	 * returns grouped rows, and PostgreSQL Agg node finalizes.
	 *
	 * For our simpler case (FairCom returns final COUNT), we'll optimize at
	 * execution time instead - detect in BeginForeignScan if the scan feeds
	 * a simple COUNT(*) Agg node, and use ctdbGetRecordCount.
	 */
	FAIRCOM_DEBUG_LOG(DEBUG1, "FairCom FDW: Aggregate pushdown disabled - will optimize during execution");
	return;
}

/*
 * faircomBeginForeignScan
 *		Initialize for a scan of a foreign table
 */
static void
faircomBeginForeignScan(ForeignScanState *node, int eflags)
{
	FairComScanState *scanstate;
	ForeignScan *fsplan = (ForeignScan *) node->ss.ps.plan;
	Oid foreigntableid;
	ForeignTable *table;
	ForeignServer *server;
	UserMapping *user;
	char *tablename = NULL;
	ListCell *lc;
	CTDBRET ret;
	char actual_tablename[256];  /* Persistent storage for converted table name */
	RangeTblEntry *rte;  /* Declare at top for C90 compliance */
	TupleDesc tupdesc;
	TupleDesc rel_tupdesc = NULL;
	Relation rel = NULL;
	TupleDesc plan_tupdesc;
	int i;

	/* Allocate scan state */
	scanstate = (FairComScanState *) palloc0(sizeof(FairComScanState));
	scanstate->first_iteration = true;
	scanstate->eof_reached = false;
	scanstate->has_predicates = false;
	scanstate->find_mode = 0;

	/* Get foreign table OID from range table */
	if (fsplan->scan.scanrelid > 0)
	{
		/* Single table scan */
		rte = exec_rt_fetch(fsplan->scan.scanrelid, node->ss.ps.state);
		foreigntableid = rte->relid;
	}
	else
	{
		ereport(ERROR,
				(errcode(ERRCODE_FDW_ERROR),
				 errmsg("FairCom FDW internal error: invalid scan_relid=%d", fsplan->scan.scanrelid)));
	}

	/* Open relation EARLY to get tuple descriptor safely
	 * Use relation tuple descriptor instead of scan slot to avoid NULL pointer issues
	 */
	rel = table_open(foreigntableid, NoLock);
	rel_tupdesc = RelationGetDescr(rel);

	/* Use relation tuple descriptor for all operations (safer than scan slot in JOIN contexts) */
	tupdesc = rel_tupdesc;

	/* Initialize LIMIT/OFFSET tracking */
	scanstate->limit_count = -1;  /* No limit by default */
	scanstate->offset_count = 0;   /* No offset by default */
	scanstate->rows_returned = 0;
	scanstate->rows_skipped = 0;

	/* Initialize aggregate pushdown tracking */
	scanstate->is_aggregate_pushdown = false;
	scanstate->aggregate_fnoid = InvalidOid;
	scanstate->aggregate_result = 0;
	scanstate->aggregate_returned = false;
	scanstate->aggregate_attnum = 0;
	scanstate->aggregate_type = InvalidOid;
	scanstate->aggregate_count = 0;
	scanstate->aggregate_sum_int = 0;
	scanstate->aggregate_sum = 0.0;
	scanstate->aggregate_min = 0.0;
	scanstate->aggregate_max = 0.0;
	scanstate->aggregate_isnull = true;

	/* Check if this scan feeds into a simple COUNT(*) aggregate
	 * Look at parent node to see if it's an Agg node with COUNT(*)
	 * This is a runtime optimization - we detect aggregate queries during execution
	 *
	 * CRITICAL: Only for single-table scans (scan_relid > 0).
	 * Do NOT attempt aggregate pushdown for JOINs (scan_relid = 0) as the plan
	 * structure is completely different and causes segfaults.
	 */
	if (fsplan->scan.scanrelid > 0 &&
		node->ss.ps.plan && node->ss.ps.state && node->ss.ps.state->es_plannedstmt)
	{
		PlannedStmt *pstmt = node->ss.ps.state->es_plannedstmt;
		Plan *parent_plan = node->ss.ps.plan;

		/* Walk up to find parent Agg node if any */
		/* Note: In simple COUNT(*) queries, the Agg is the top plan node */
		if (IsA(pstmt->planTree, Agg))
		{
			Agg *agg_node = (Agg *) pstmt->planTree;

			/* Check if it's a simple COUNT(*) aggregate:
			 * - Single aggregate
			 * - No grouping
			 * - Outer plan is our ForeignScan
			 */
			if (list_length(agg_node->plan.targetlist) == 1 &&
				agg_node->numCols == 0 &&  /* No GROUP BY */
				IsA(agg_node->plan.lefttree, ForeignScan) &&
				agg_node->plan.lefttree == node->ss.ps.plan)
			{
				TargetEntry *tle = (TargetEntry *) linitial(agg_node->plan.targetlist);

				if (IsA(tle->expr, Aggref))
				{
					Aggref *aggref = (Aggref *) tle->expr;
					AttrNumber attnum = 0;
					Oid column_type = InvalidOid;

					/* Extract column being aggregated (if any) */
					if (list_length(aggref->args) > 0)
					{
						TargetEntry *arg_tle = (TargetEntry *) linitial(aggref->args);
						if (IsA(arg_tle->expr, Var))
						{
							Var *var = (Var *) arg_tle->expr;
							attnum = var->varattno;
							column_type = var->vartype;
						}
					}

					/* Skip aggregates with DISTINCT, FILTER, or ORDER BY */
					if (aggref->aggdistinct != NIL || aggref->aggfilter != NULL || aggref->aggorder != NIL)
					{
						FAIRCOM_DEBUG_LOG(DEBUG1, "FairCom FDW: Aggregate has DISTINCT/FILTER/ORDER BY - disabling pushdown");
					}
					/* Check aggregate type and enable pushdown */
					else if (aggref->aggfnoid == 2147 || aggref->aggfnoid == 2803)  /* COUNT */
					{
						scanstate->is_aggregate_pushdown = true;
						scanstate->aggregate_fnoid = aggref->aggfnoid;
						scanstate->aggregate_attnum = attnum;

						/* Track aggregate pushdown usage */
						if (faircom_enable_aggregate_pushdown)
							global_stats.aggregate_pushdowns++;

						FAIRCOM_DEBUG_LOG(DEBUG1, "FairCom FDW: Detected COUNT aggregate (fnoid=%u, attnum=%d)",
							 aggref->aggfnoid, attnum);
					}
						/* SUM, AVG, MIN, MAX detection is intentionally omitted.
					 * Only COUNT has a working pushdown execution path.
					 * Other aggregates fall through to PostgreSQL's native computation.
					 */
				}
			}
		}
	}

	/* Initialize batch fetching */
	scanstate->batch_values = NULL;
	scanstate->batch_nulls = NULL;
	scanstate->batch_size = 0;
	scanstate->batch_position = 0;
	scanstate->batch_natts = 0;

	/* Initialize batch optimization tracking */
	scanstate->current_batch_limit = FAIRCOM_FETCH_BATCH_SIZE;
	scanstate->avg_row_size = 0;
	scanstate->record_size = 0;  /* Will be populated after table open */
	scanstate->optimal_batch_size = FAIRCOM_FETCH_BATCH_SIZE;  /* Default until calculated */
	scanstate->total_batches = 0;
	scanstate->total_rows_fetched = 0;

	/* Always track rowids for row identity */
	scanstate->batch_rowids = NULL;

	/* foreigntableid, rel, and rel_tupdesc already retrieved above */

	/* Get foreign table, server, and user mapping */
	table = GetForeignTable(foreigntableid);
	server = GetForeignServer(table->serverid);
	user = GetUserMapping(GetUserId(), server->serverid);

	/* Extract table name from options */
	foreach(lc, table->options)
	{
		DefElem *def = (DefElem *) lfirst(lc);
		if (strcmp(def->defname, "table_name") == 0)
		{
			tablename = defGetString(def);
			break;
		}
	}

	if (!tablename)
		tablename = get_rel_name(foreigntableid);

	FAIRCOM_DEBUG_LOG(DEBUG1, "Opening FairCom table: %s", tablename);
	scanstate->tablename = pstrdup(tablename);

	/* Get connection to FairCom */
	scanstate->conn = faircom_get_connection(server, user);
	if (!scanstate->conn)
		ereport(ERROR,
				(errcode(ERRCODE_FDW_UNABLE_TO_ESTABLISH_CONNECTION),
				 errmsg("could not connect to FairCom server")));

	/* Use database handle if connected to a database, otherwise use session */
	scanstate->hSession = scanstate->conn->hDatabase ?
						  scanstate->conn->hDatabase :
						  scanstate->conn->hSession;

	/* Allocate table handle */
	scanstate->hTable = ctdbAllocTable(scanstate->hSession);
	if (!scanstate->hTable)
		ereport(ERROR,
				(errcode(ERRCODE_FDW_OUT_OF_MEMORY),
				 errmsg("failed to allocate FairCom table handle")));

	/* Parse table name: database.schema.table -> database.dbs path + schema_table name */
	/* For test_emp_v4.admin.employees, convert to path "test_emp_v4.dbs" and table "admin_employees" */
	{
		char table_path[256];
		char *first_dot = strchr(tablename, '.');
		char *last_dot = strrchr(tablename, '.');

		if (first_dot && last_dot && first_dot != last_dot)
		{
			/* Three-part name: database.schema.table */
			size_t db_len = first_dot - tablename;
			size_t schema_len = last_dot - first_dot - 1;
			const char *schema_start = first_dot + 1;
			const char *table_start = last_dot + 1;

			/* Build path: database.dbs */
			if (db_len < sizeof(table_path) - 4)
			{
				memcpy(table_path, tablename, db_len);
				strcpy(table_path + db_len, ".dbs");

				/* Build table name: schema_table */
				if (schema_len + 1 + strlen(table_start) < sizeof(actual_tablename))
				{
					memcpy(actual_tablename, schema_start, schema_len);
					actual_tablename[schema_len] = '_';
					strcpy(actual_tablename + schema_len + 1, table_start);

					ret = ctdbSetTablePath(scanstate->hTable, (pTEXT)table_path);
					if (ret != CTDBRET_OK)
						FAIRCOM_DEBUG_LOG(DEBUG1, "Failed to set table path: %d", ret);

					tablename = actual_tablename;
				}
			}
		}
	}

	/* Open the table */
	ret = ctdbOpenTable(scanstate->hTable, (pTEXT)tablename, CTOPEN_NORMAL);
	if (ret != CTDBRET_OK)
		ereport(ERROR,
				(errcode(ERRCODE_FDW_TABLE_NOT_FOUND),
				 errmsg("could not open FairCom table \"%s\"", tablename),
				 errdetail("FairCom error code: %d", ret)));

	/* Get actual record size from FairCom and calculate optimal batch size */
	{
		VRLEN record_size = ctdbGetRecordSize(scanstate->hTable);
		if (record_size > 0)
		{
			scanstate->record_size = (int)record_size;

			/* Calculate optimal batch size targeting FAIRCOM_TARGET_BATCH_MEMORY (1MB) per batch */
			int optimal_batch = FAIRCOM_TARGET_BATCH_MEMORY / record_size;

			/* Clamp to min/max batch size limits */
			if (optimal_batch < FAIRCOM_MIN_BATCH_SIZE)
				optimal_batch = FAIRCOM_MIN_BATCH_SIZE;
			else if (optimal_batch > FAIRCOM_MAX_BATCH_SIZE)
				optimal_batch = FAIRCOM_MAX_BATCH_SIZE;

			scanstate->optimal_batch_size = optimal_batch;

			FAIRCOM_DEBUG_LOG(DEBUG1,
				"FairCom record size: %d bytes, optimal batch size: %d rows (target: %d KB/batch)",
				record_size, optimal_batch, (optimal_batch * record_size) / 1024);
		}
		else
		{
			/* Failed to get record size - use defaults */
			scanstate->record_size = 0;
			scanstate->optimal_batch_size = FAIRCOM_FETCH_BATCH_SIZE;
			FAIRCOM_DEBUG_LOG(DEBUG1,
				"Failed to get record size from FairCom (ret=%d), using default batch size %d",
				record_size, FAIRCOM_FETCH_BATCH_SIZE);
		}
	}

	if (scanstate->is_aggregate_pushdown && list_length(fsplan->fdw_private) >= 3)
	{
		List *local_exprs = (List *) list_nth(fsplan->fdw_private, 2);
		if (local_exprs != NIL)
		{
			FAIRCOM_DEBUG_LOG(DEBUG1, "FairCom FDW: Local quals present - disabling aggregate pushdown");
			scanstate->is_aggregate_pushdown = false;
		}
	}

	/* Handle aggregate pushdown (COUNT, SUM, AVG, MIN, MAX) */
	if (scanstate->is_aggregate_pushdown)
	{
		char *predicate_str = "";
		bool is_count = (scanstate->aggregate_fnoid == 2147 || scanstate->aggregate_fnoid == 2803);
		List *remote_exprs = NIL;
		bool has_where_clause = false;
		int64 cached_result;

		if (is_count)
		{
			FAIRCOM_DEBUG_LOG(DEBUG1, "FairCom FDW: Executing aggregate pushdown COUNT(*)");
		}
		else
		{
			FAIRCOM_DEBUG_LOG(DEBUG1, "FairCom FDW: Executing aggregate pushdown (fnoid=%u) on column %d",
				 scanstate->aggregate_fnoid, scanstate->aggregate_attnum);
		}

		/* Check if we have a WHERE clause (remote predicates) */
		if (list_length(fsplan->fdw_private) >= 2)
		{
			remote_exprs = (List *) list_nth(fsplan->fdw_private, 1);
			has_where_clause = (remote_exprs != NIL);
		}

		/* Build predicate string for cache key */
		if (has_where_clause)
		{
			StringInfoData pred_buf;
			ListCell *lc;
			bool first = true;

			initStringInfo(&pred_buf);
			foreach(lc, remote_exprs)
			{
				Expr *expr = (Expr *) lfirst(lc);
				char *expr_str = nodeToString(expr);

				if (!first)
					appendStringInfoString(&pred_buf, " AND ");
				appendStringInfoString(&pred_buf, expr_str);
				first = false;
			}
			predicate_str = pred_buf.data;
		}
		else
		{
			predicate_str = "";
		}

		/* Check cache first - uses FAIRCOM_CACHE_DEFAULT_TTL (300 seconds) */
		if (faircom_cache_get_count(tablename, predicate_str, &cached_result, faircom_cache_ttl))
		{
			/* Cache hit! Return cached result immediately */
			scanstate->aggregate_result = cached_result;

			/* Track query cache hit */
			global_stats.query_cache_hits++;

			FAIRCOM_DEBUG_LOG(DEBUG1, "FairCom FDW: Using cached COUNT(*) result = %lld", (long long) cached_result);

			/* Store scanstate and return - no need to query FairCom */
			if (rel)
			{
				table_close(rel, NoLock);
				rel = NULL;
			}
			node->fdw_state = scanstate;
			return;
		}

		/* Track query cache miss */
		global_stats.query_cache_misses++;

		/* Cache miss - need to execute query */
		FAIRCOM_DEBUG_LOG(DEBUG1, "FairCom FDW: Cache miss, executing COUNT(*) query");

		/* Allocate record handle needed for ctdbGetRecordCount */
		scanstate->hRecord = ctdbAllocRecord(scanstate->hTable);
		if (!scanstate->hRecord)
			ereport(ERROR,
					(errcode(ERRCODE_FDW_OUT_OF_MEMORY),
					 errmsg("failed to allocate FairCom record handle")));

		/* Check if we have predicates - if so, count manually with filtering */
		if (has_where_clause)
		{
			/* COUNT(*) with WHERE clause - iterate and count matching records */
			int64 filtered_count = 0;
			CTDBRET fetch_ret;
			List *predicates = NIL;
			ListCell *lc_expr;

			FAIRCOM_DEBUG_LOG(DEBUG1, "FairCom FDW: COUNT(*) with WHERE - extracting predicates and counting filtered records");

			/* Extract predicate information from each remote expression */
			foreach(lc_expr, remote_exprs)
			{
				Expr *expr = (Expr *) lfirst(lc_expr);
				List *expr_preds = faircom_extract_predicates_from_expr(expr, rel_tupdesc);
				predicates = list_concat(predicates, expr_preds);
			}

			if (predicates != NIL)
			{
				scanstate->has_predicates = true;
				scanstate->active_predicates = predicates;

				/* Try to use an index for the first predicate */
				CTFIND_MODE find_mode = faircom_apply_predicates(scanstate->hTable,
										  scanstate->hRecord,
										  predicates,
										  rel_tupdesc);
				scanstate->find_mode = find_mode;

				FAIRCOM_DEBUG_LOG(DEBUG1, "FairCom FDW: Using find_mode=%d for COUNT(*) with WHERE", find_mode);
			}

			/* Check if this is an IN list pattern (find_mode == -1) */
			if (scanstate->find_mode == -1)
			{
				/* IN list optimization for COUNT(*) */
				FAIRCOM_DEBUG_LOG(DEBUG1, "Using IN list optimization for COUNT(*) with WHERE");

				/* For each IN list value, do a targeted lookup */
				ListCell *lc_pred;
				filtered_count = 0;

				foreach(lc_pred, predicates)
				{
					FairComPredicate *pred = (FairComPredicate *) lfirst(lc_pred);
					int field_num = pred->attnum - 1;
					CTDBRET lookup_ret;

					/* Set field value */
					switch (pred->const_type)
					{
						case INT2OID:
						case INT4OID:
							ctdbSetFieldAsSigned(scanstate->hRecord, field_num, DatumGetInt32(pred->const_value));
							break;
						case INT8OID:
							ctdbSetFieldAsSigned(scanstate->hRecord, field_num, (CTSIGNED)DatumGetInt64(pred->const_value));
							break;
						case FLOAT4OID:
						case FLOAT8OID:
							ctdbSetFieldAsFloat(scanstate->hRecord, field_num, DatumGetFloat8(pred->const_value));
							break;
						case NUMERICOID:
							{
								/* Use cached conversion to avoid repeated DirectFunctionCall1 overhead */
								if (!pred->cached_float8_valid)
								{
									float8 float_val = DatumGetFloat8(DirectFunctionCall1(numeric_float8, pred->const_value));
									pred->cached_float8_value = float_val;
									pred->cached_float8_valid = true;
								}
								ctdbSetFieldAsSigned(scanstate->hRecord, field_num, (CTSIGNED)(int64)pred->cached_float8_value);
							}
							break;
						default:
							continue;
					}

					/* Try to find this value */
					lookup_ret = ctdbFindRecord(scanstate->hRecord, CTFIND_EQ);
					if (lookup_ret == CTDBRET_OK)
						filtered_count++;
				}

				FAIRCOM_DEBUG_LOG(DEBUG1, "FairCom FDW: COUNT(*) with IN list returned %lld rows",
					 (long long)filtered_count);
			}
			else
			{
				/* Normal predicate handling (not IN list) */

			/* Start iteration based on find mode */
			if (scanstate->find_mode != 0)
			{
				/* Use index-based find */
				fetch_ret = ctdbFindRecord(scanstate->hRecord, scanstate->find_mode);
			}
			else
			{
				/* Sequential scan */
				fetch_ret = ctdbFirstRecord(scanstate->hRecord);
			}

			/* Iterate through records and count matches */
			while (fetch_ret == CTDBRET_OK)
			{
				/* Check range scan stop condition FIRST (before predicate match check)
				 * This is critical for performance: once we've left the range, stop scanning.
				 */
				if (scanstate->find_mode != 0 && scanstate->find_mode != CTFIND_EQ)
				{
					if (faircom_should_stop_range_scan(scanstate->hRecord,
													   scanstate->active_predicates,
													   scanstate->find_mode))
					{
						FAIRCOM_DEBUG_LOG(DEBUG1, "FairCom FDW: Stopping aggregate range scan early (outside predicate range)");
						break;  /* Exit the while loop - we're done */
					}
				}

				/* If using index and single predicate, trust the index - no need to recheck */
				/* Otherwise, check if record matches all predicates */
				if (scanstate->find_mode == 0 || list_length(predicates) > 1)
				{
					/* No index OR multiple predicates - must check all predicates */
					if (!faircom_check_predicate_match(scanstate->hRecord, scanstate->active_predicates))
					{
						/* Get next record */
						fetch_ret = ctdbNextRecord(scanstate->hRecord);
						continue;
					}
				}
				/* else: using index with single predicate, all records match by definition */

				filtered_count++;

				/* For non-COUNT aggregates, accumulate the column value */
				if (!is_count && scanstate->aggregate_attnum > 0)
				{
					NINT field_num = scanstate->aggregate_attnum - 1;  /* 0-based */

					/* Check for NULL */
					if (ctdbIsNullField(scanstate->hRecord, field_num))
					{
						/* NULL values don't contribute to SUM/AVG/MIN/MAX */
					}
					else
					{
						scanstate->aggregate_isnull = false;  /* We have at least one non-NULL value */

						double value = 0.0;

						/* Convert field to double for accumulation */
						/* Try to get as integer types first, then floating point */
						if (scanstate->aggregate_type == INT2OID || scanstate->aggregate_type == INT4OID)
						{
							CTSIGNED int_val;
							ctdbGetFieldAsSigned(scanstate->hRecord, field_num, &int_val);
							value = (double) int_val;
						}
						else if (scanstate->aggregate_type == INT8OID)
						{
							CTBIGINT int_val;
							ctdbGetFieldAsBigint(scanstate->hRecord, field_num, &int_val);
							value = (double) int_val;
						}
						else
						{
							/* Float, NUMERIC, or other - get as float */
							CTFLOAT float_val;
							ctdbGetFieldAsFloat(scanstate->hRecord, field_num, &float_val);
							value = (double) float_val;
						}

						/* Accumulate based on aggregate type */
						if (scanstate->aggregate_fnoid >= 2108 && scanstate->aggregate_fnoid <= 2114)  /* SUM */
						{
							scanstate->aggregate_sum += value;
						}
						else if (scanstate->aggregate_fnoid >= 2101 && scanstate->aggregate_fnoid <= 2106)  /* AVG */
						{
							scanstate->aggregate_sum += value;
							scanstate->aggregate_count++;
						}
						else if (scanstate->aggregate_fnoid >= 2131 && scanstate->aggregate_fnoid <= 2146)  /* MIN */
						{
							if (scanstate->aggregate_count == 0 || value < scanstate->aggregate_min)
								scanstate->aggregate_min = value;
							scanstate->aggregate_count++;
						}
						else if (scanstate->aggregate_fnoid >= 2115 && scanstate->aggregate_fnoid <= 2130)  /* MAX */
						{
							if (scanstate->aggregate_count == 0 || value > scanstate->aggregate_max)
								scanstate->aggregate_max = value;
							scanstate->aggregate_count++;
						}
					}
				}

				/* Get next record */
				fetch_ret = ctdbNextRecord(scanstate->hRecord);
			}

			if (is_count)
			{
				FAIRCOM_DEBUG_LOG(DEBUG1, "FairCom FDW: COUNT(*) with WHERE returned %lld rows",
					 (long long) filtered_count);
				scanstate->aggregate_result = filtered_count;
				scanstate->aggregate_isnull = false;
			}
			else if (scanstate->aggregate_fnoid >= 2108 && scanstate->aggregate_fnoid <= 2114)  /* SUM */
			{
				FAIRCOM_DEBUG_LOG(DEBUG1, "FairCom FDW: SUM aggregate with WHERE computed sum=%.2f from %lld rows",
					 scanstate->aggregate_sum, (long long) filtered_count);
			}
			else if (scanstate->aggregate_fnoid >= 2101 && scanstate->aggregate_fnoid <= 2106)  /* AVG */
			{
				FAIRCOM_DEBUG_LOG(DEBUG1, "FairCom FDW: AVG aggregate with WHERE computed sum=%.2f, count=%lld",
					 scanstate->aggregate_sum, (long long) scanstate->aggregate_count);
			}
			else if (scanstate->aggregate_fnoid >= 2131 && scanstate->aggregate_fnoid <= 2146)  /* MIN */
			{
				FAIRCOM_DEBUG_LOG(DEBUG1, "FairCom FDW: MIN aggregate with WHERE computed min=%.2f from %lld rows",
					 scanstate->aggregate_min, (long long) scanstate->aggregate_count);
			}
			else if (scanstate->aggregate_fnoid >= 2115 && scanstate->aggregate_fnoid <= 2130)  /* MAX */
			{
				FAIRCOM_DEBUG_LOG(DEBUG1, "FairCom FDW: MAX aggregate with WHERE computed max=%.2f from %lld rows",
					 scanstate->aggregate_max, (long long) scanstate->aggregate_count);
			}

			}  /* End normal (non-IN list) predicate handling */

			/* Cache the result */
			faircom_cache_set_count(tablename, predicate_str, filtered_count, FAIRCOM_CACHE_DEFAULT_TTL);

			/* Close table and free handles - we're done scanning */
			ctdbFreeRecord(scanstate->hRecord);
			scanstate->hRecord = NULL;

			/* Store scanstate and return */
			if (rel)
			{
				table_close(rel, NoLock);
				rel = NULL;
			}
			node->fdw_state = scanstate;
			return;
		}

		/* No predicates - do full scan for COUNT to avoid inaccurate ctdbGetRecordCount */
		if (is_count)
		{
			CTDBRET fetch_ret;
			int64 full_count = 0;

			fetch_ret = ctdbFirstRecord(scanstate->hRecord);
			while (fetch_ret == CTDBRET_OK)
			{
				full_count++;
				fetch_ret = ctdbNextRecord(scanstate->hRecord);
			}

			scanstate->aggregate_result = full_count;
			scanstate->aggregate_isnull = false;
			FAIRCOM_DEBUG_LOG(DEBUG1, "FairCom FDW: COUNT(*) full scan returned %lld rows", (long long) full_count);

			/* Cache the result */
			faircom_cache_set_count(tablename, predicate_str, full_count, FAIRCOM_CACHE_DEFAULT_TTL);

			/* Close record handle - we're done */
			ctdbFreeRecord(scanstate->hRecord);
			scanstate->hRecord = NULL;

			node->fdw_state = scanstate;
			if (rel)
			{
				table_close(rel, NoLock);
				rel = NULL;
			}
			return;
		}
		/* For SUM/AVG/MIN/MAX, we need to scan all records - fall through to normal scan */
		if (!is_count)
		{
			/* Full sequential scan to accumulate aggregate values */
			CTDBRET fetch_ret;
			int64 row_count = 0;

			FAIRCOM_DEBUG_LOG(DEBUG1, "FairCom FDW: Starting full scan for aggregate (fnoid=%u) on column %d",
				 scanstate->aggregate_fnoid, scanstate->aggregate_attnum);

			/* Start from first record */
			fetch_ret = ctdbFirstRecord(scanstate->hRecord);

			while (fetch_ret == CTDBRET_OK)
			{
				row_count++;

				/* Accumulate the column value */
				if (scanstate->aggregate_attnum > 0)
				{
					NINT field_num = scanstate->aggregate_attnum - 1;  /* 0-based */

					/* Check for NULL */
					if (ctdbIsNullField(scanstate->hRecord, field_num))
					{
						/* NULL values don't contribute */
					}
					else
					{
						scanstate->aggregate_isnull = false;

						double value = 0.0;

						/* Convert field to double */
						if (scanstate->aggregate_type == INT2OID || scanstate->aggregate_type == INT4OID)
						{
							CTSIGNED int_val;
							ctdbGetFieldAsSigned(scanstate->hRecord, field_num, &int_val);
							value = (double) int_val;
						}
						else if (scanstate->aggregate_type == INT8OID)
						{
							CTBIGINT int_val;
							ctdbGetFieldAsBigint(scanstate->hRecord, field_num, &int_val);
							value = (double) int_val;
						}
						else
						{
							CTFLOAT float_val;
							ctdbGetFieldAsFloat(scanstate->hRecord, field_num, &float_val);
							value = (double) float_val;
						}

						/* Accumulate based on aggregate type */
						if (scanstate->aggregate_fnoid >= 2108 && scanstate->aggregate_fnoid <= 2114)  /* SUM */
						{
							scanstate->aggregate_sum += value;
						}
						else if (scanstate->aggregate_fnoid >= 2101 && scanstate->aggregate_fnoid <= 2106)  /* AVG */
						{
							scanstate->aggregate_sum += value;
							scanstate->aggregate_count++;
						}
						else if (scanstate->aggregate_fnoid >= 2131 && scanstate->aggregate_fnoid <= 2146)  /* MIN */
						{
							if (scanstate->aggregate_count == 0 || value < scanstate->aggregate_min)
								scanstate->aggregate_min = value;
							scanstate->aggregate_count++;
						}
						else if (scanstate->aggregate_fnoid >= 2115 && scanstate->aggregate_fnoid <= 2130)  /* MAX */
						{
							if (scanstate->aggregate_count == 0 || value > scanstate->aggregate_max)
								scanstate->aggregate_max = value;
							scanstate->aggregate_count++;
						}
					}
				}

				/* Get next record */
				fetch_ret = ctdbNextRecord(scanstate->hRecord);
			}

			/* Log result */
			if (scanstate->aggregate_fnoid >= 2108 && scanstate->aggregate_fnoid <= 2114)  /* SUM */
			{
				FAIRCOM_DEBUG_LOG(DEBUG1, "FairCom FDW: SUM aggregate computed sum=%.2f from %lld rows",
					 scanstate->aggregate_sum, (long long) row_count);
			}
			else if (scanstate->aggregate_fnoid >= 2101 && scanstate->aggregate_fnoid <= 2106)  /* AVG */
			{
				FAIRCOM_DEBUG_LOG(DEBUG1, "FairCom FDW: AVG aggregate computed sum=%.2f, count=%lld from %lld total rows",
					 scanstate->aggregate_sum, (long long) scanstate->aggregate_count, (long long) row_count);
			}
			else if (scanstate->aggregate_fnoid >= 2131 && scanstate->aggregate_fnoid <= 2146)  /* MIN */
			{
				FAIRCOM_DEBUG_LOG(DEBUG1, "FairCom FDW: MIN aggregate computed min=%.2f from %lld rows",
					 scanstate->aggregate_min, (long long) row_count);
			}
			else if (scanstate->aggregate_fnoid >= 2115 && scanstate->aggregate_fnoid <= 2130)  /* MAX */
			{
				FAIRCOM_DEBUG_LOG(DEBUG1, "FairCom FDW: MAX aggregate computed max=%.2f from %lld rows",
					 scanstate->aggregate_max, (long long) row_count);
			}

			/* Close table and free handles - we're done scanning */
			ctdbFreeRecord(scanstate->hRecord);
			scanstate->hRecord = NULL;

			/* Store scanstate and return */
			if (rel)
			{
				table_close(rel, NoLock);
				rel = NULL;
			}
			node->fdw_state = scanstate;
			return;
		}
	}

	/* Allocate record handle */
	scanstate->hRecord = ctdbAllocRecord(scanstate->hTable);
	if (!scanstate->hRecord)
		ereport(ERROR,
				(errcode(ERRCODE_FDW_OUT_OF_MEMORY),
				 errmsg("failed to allocate FairCom record handle")));

	/* NOTE: ctdbSetBatch() does not actually batch ctdbNextRecord() calls
	 * Each ctdbNextRecord() is still a separate network round trip
	 * Network latency (~7-8ms to holly.local) creates per-record overhead
	 * This is a limitation of the FairCom C-Tree API architecture
	 */

	/* Apply predicate pushdown if we have remote expressions */
	if (list_length(fsplan->fdw_private) >= 2)
	{
		List *remote_exprs = (List *) list_nth(fsplan->fdw_private, 1);

		if (remote_exprs != NIL)
		{
			List *predicates = NIL;
			ListCell *lc_expr;

			/* Extract predicate information from each remote expression */
			/* This now handles AND, OR, and IN lists */
			foreach(lc_expr, remote_exprs)
			{
				Expr *expr = (Expr *) lfirst(lc_expr);
				List *expr_preds = faircom_extract_predicates_from_expr(expr, rel_tupdesc);
				predicates = list_concat(predicates, expr_preds);
			}

			/* Store ALL predicates for runtime checking (even if no index used) */
			if (predicates != NIL)
			{
				scanstate->has_predicates = true;
				scanstate->active_predicates = predicates;

				/* Try to use an index for the first predicate */
				CTFIND_MODE find_mode = faircom_apply_predicates(scanstate->hTable,
										  scanstate->hRecord,
										  predicates,
										  rel_tupdesc);
				scanstate->find_mode = find_mode;

				FAIRCOM_DEBUG_LOG(DEBUG1, "FairCom FDW: Stored %d predicate(s) for runtime checking (find_mode=%d)",
					 list_length(predicates), find_mode);
			}
		}
	}

	/* Extract LIMIT/OFFSET if present in fdw_private */
	if (list_length(fsplan->fdw_private) >= 5)
	{
		scanstate->limit_count = intVal(list_nth(fsplan->fdw_private, 3));
		scanstate->offset_count = intVal(list_nth(fsplan->fdw_private, 4));

		if (scanstate->limit_count > 0)
		{
			FAIRCOM_DEBUG_LOG(DEBUG1, "FairCom FDW: LIMIT %ld detected - will stop scan early",
				 (long)scanstate->limit_count);
		}

		if (scanstate->offset_count > 0)
		{
			FAIRCOM_DEBUG_LOG(DEBUG1, "FairCom FDW: OFFSET %ld detected - will skip first %ld rows in FairCom",
				 (long)scanstate->offset_count, (long)scanstate->offset_count);
		}
	}

	/* Allocate batch buffers in executor memory context (persists for entire scan)
	 * Use MAX_BATCH_SIZE to support adaptive sizing up to maximum
	 */
	{
		int max_batch = FAIRCOM_MAX_BATCH_SIZE;  /* Allocate for maximum possible size */
		int natts = tupdesc->natts;
		int i;

		scanstate->batch_natts = natts;
		scanstate->batch_values = (Datum **) palloc0(max_batch * sizeof(Datum *));
		scanstate->batch_nulls = (bool **) palloc0(max_batch * sizeof(bool *));

		for (i = 0; i < max_batch; i++)
		{
			scanstate->batch_values[i] = (Datum *) palloc0(natts * sizeof(Datum));
			scanstate->batch_nulls[i] = (bool *) palloc0(natts * sizeof(bool));
		}

		/* Allocate rowid array for row identity */
		scanstate->batch_rowids = (CTROWID *) palloc0(max_batch * sizeof(CTROWID));

		FAIRCOM_DEBUG_LOG(DEBUG1, "Allocated batch buffer for %d records x %d attributes (adaptive sizing enabled)",
			 max_batch, natts);

		/* Allocate field number cache and populate immediately
		 * Pre-caching field numbers eliminates expensive string lookups during batch fetching
		 * This is a critical performance optimization for queries fetching many rows
		 */
		scanstate->field_nums = (int *) palloc0(natts * sizeof(int));

		/* Pre-populate field number cache using table metadata */
		for (i = 0; i < natts; i++)
		{
			Form_pg_attribute attr = TupleDescAttr(tupdesc, i);
			const char *colname;
			NINT fieldNum;

			if (attr->attisdropped)
			{
				scanstate->field_nums[i] = -1;
				continue;
			}

			colname = NameStr(attr->attname);

			/* Skip system/junk columns like ctid - they don't exist in FairCom */
			if (strcmp(colname, "ctid") == 0 || attr->atttypid == TIDOID)
			{
				scanstate->field_nums[i] = -1;  /* Mark as not mapped */
				FAIRCOM_DEBUG_LOG(DEBUG2, "FairCom FDW: Skipping junk column '%s' in field mapping", colname);
				continue;
			}

			/* Look up field number by name (try record first, then table) */
			fieldNum = ctdbGetFieldNumberByName(scanstate->hRecord, (pTEXT)colname);
			if (fieldNum < 0)
				fieldNum = ctdbGetFieldNumberByName(scanstate->hTable, (pTEXT)colname);

			/* Try case-insensitive match if exact match failed */
			if (fieldNum < 0)
			{
				NINT num_fields = ctdbGetTableFieldCount(scanstate->hTable);
				char upper_colname[256];
				int j, k;

				/* Convert column name to uppercase */
				for (j = 0; colname[j] && j < sizeof(upper_colname) - 1; j++)
					upper_colname[j] = toupper((unsigned char)colname[j]);
				upper_colname[j] = '\0';

				/* Try each field for case-insensitive match */
				for (k = 0; k < num_fields; k++)
				{
					CTHANDLE hField = ctdbGetField(scanstate->hTable, k);
					if (hField)
					{
						pTEXT fname = ctdbGetFieldName(hField);
						if (fname)
						{
							char upper_fname[256];
							int m;
							for (m = 0; fname[m] && m < sizeof(upper_fname) - 1; m++)
								upper_fname[m] = toupper((unsigned char)fname[m]);
							upper_fname[m] = '\0';

							if (strcmp(upper_fname, upper_colname) == 0)
							{
								fieldNum = k;
								FAIRCOM_DEBUG_LOG(DEBUG2, "Pre-cached FairCom field (case-insensitive): %s -> %s at position %d",
									 colname, fname, fieldNum);
								break;
							}
						}
					}
				}
			}

			scanstate->field_nums[i] = fieldNum;

			if (fieldNum < 0)
				elog(WARNING, "Column '%s' not found in FairCom table - will return NULL", colname);
		}

		scanstate->field_nums_cached = true;
		FAIRCOM_DEBUG_LOG(DEBUG1, "Pre-cached %d field number mappings (eliminates per-row string lookups)", natts);
	}

	/* Initialize parallel scan state if this is a parallel worker */
	scanstate->parallel_state = NULL;
	scanstate->is_parallel_worker = false;
	scanstate->worker_start_row = 0;
	scanstate->worker_end_row = -1;  /* -1 means no limit */

	/* Close relation - we've copied the tuple descriptor info we need */
	if (rel)
		table_close(rel, NoLock);

	node->fdw_state = (void *) scanstate;
}

/*
 * faircom_fill_batch
 *		Fill the batch buffer with records from FairCom
 *		Returns the number of records fetched, or 0 on EOF
 *
 *		Features adaptive batch sizing based on row size:
 *		- Small rows (<100 bytes): Use max batch size (2000 rows)
 *		- Medium rows (100-1000 bytes): Use default (500 rows)
 *		- Large rows (>1KB): Use min batch size (50 rows)
 */
static int
faircom_fill_batch(FairComScanState *scanstate, TupleDesc tupdesc)
{
	int batch_count = 0;
	int max_batch;
	CTDBRET ret;
	int i, j;
	int natts = tupdesc->natts;
	int batch_row_bytes = 0;  /* Track bytes in this batch for adaptive sizing */

	/* Use optimal batch size calculated from ctdbGetRecordSize() if available.
	 * This provides better performance than adaptive sizing based on sampled rows.
	 * Falls back to adaptive sizing if record size wasn't available.
	 */
	if (scanstate->record_size > 0)
	{
		/* Use pre-calculated optimal batch from ctdbGetRecordSize() */
		max_batch = scanstate->optimal_batch_size;
		FAIRCOM_DEBUG_LOG(DEBUG2, "Using optimal batch size %d (record_size=%d bytes)",
			 max_batch, scanstate->record_size);
	}
	else if (scanstate->avg_row_size == 0)
	{
		/* First batch and no record size - use default */
		max_batch = FAIRCOM_FETCH_BATCH_SIZE;
	}
	else if (scanstate->avg_row_size < FAIRCOM_SMALL_ROW_THRESHOLD)
	{
		/* Small rows - fetch more */
		max_batch = FAIRCOM_MAX_BATCH_SIZE;
		FAIRCOM_DEBUG_LOG(DEBUG1, "Small rows detected (avg %d bytes) - using large batch size %d",
			 scanstate->avg_row_size, max_batch);
	}
	else if (scanstate->avg_row_size > FAIRCOM_LARGE_ROW_THRESHOLD)
	{
		/* Large rows - fetch fewer */
		max_batch = FAIRCOM_MIN_BATCH_SIZE;
		FAIRCOM_DEBUG_LOG(DEBUG1, "Large rows detected (avg %d bytes) - using small batch size %d",
			 scanstate->avg_row_size, max_batch);
	}
	else
	{
		/* Medium rows - use default */
		max_batch = FAIRCOM_FETCH_BATCH_SIZE;
	}

	/* Cap batch size to remaining LIMIT rows to avoid over-fetching */
	if (scanstate->limit_count > 0)
	{
		int64 remaining = scanstate->limit_count - scanstate->rows_returned;
		if (remaining <= 0)
		{
			scanstate->eof_reached = true;
			return 0;
		}
		if (remaining < max_batch)
			max_batch = (int)remaining;
	}

	scanstate->current_batch_limit = max_batch;

	/* Verify batch buffer integrity */
	if (scanstate->batch_values == NULL || scanstate->batch_nulls == NULL)
	{
		elog(ERROR, "Batch buffers not allocated - this should never happen");
	}

	if (scanstate->batch_natts != natts)
	{
		elog(ERROR, "Tuple descriptor mismatch: expected %d attrs, got %d",
			 scanstate->batch_natts, natts);
	}

	/* Skip OFFSET rows if needed (only on first batch) */
	if (scanstate->offset_count > 0 && scanstate->rows_skipped < scanstate->offset_count)
	{
		FAIRCOM_DEBUG_LOG(DEBUG1, "Skipping first %ld rows for OFFSET", (long)scanstate->offset_count);

		while (scanstate->rows_skipped < scanstate->offset_count && !scanstate->eof_reached)
		{
			/* Fetch next record to skip */
			if (scanstate->first_iteration)
			{
				if (scanstate->has_predicates && scanstate->find_mode != 0)
				{
					ret = ctdbFindRecord(scanstate->hRecord, scanstate->find_mode);
				}
				else
				{
					ret = ctdbFirstRecord(scanstate->hRecord);
				}
				scanstate->first_iteration = false;
			}
			else
			{
				ret = ctdbNextRecord(scanstate->hRecord);
			}

			/* Check for EOF */
			if (ret == INOT_ERR)
			{
				scanstate->eof_reached = true;
				FAIRCOM_DEBUG_LOG(DEBUG1, "EOF reached during OFFSET skip at row %ld of %ld",
					 (long)scanstate->rows_skipped, (long)scanstate->offset_count);
				break;
			}

			if (ret != CTDBRET_OK)
			{
				faircom_check_result(ret, scanstate->hRecord, "skipping OFFSET rows");
				scanstate->eof_reached = true;
				break;
			}

			/* Check predicates even for skipped rows */
			if (scanstate->has_predicates)
			{
				if (!faircom_check_predicate_match(scanstate->hRecord, scanstate->active_predicates))
				{
					/* Doesn't count toward OFFSET if it doesn't match predicates */
					continue;
				}
			}

			scanstate->rows_skipped++;
		}

		FAIRCOM_DEBUG_LOG(DEBUG1, "Skipped %ld rows for OFFSET", (long)scanstate->rows_skipped);
	}

	/* Parallel worker: Get next batch of rows to scan */
	if (scanstate->is_parallel_worker && scanstate->parallel_state)
	{
		FairComParallelScanState *pstate = scanstate->parallel_state;
		uint64 my_start;

		/* Atomically claim next batch of rows */
		my_start = pg_atomic_fetch_add_u64(&pstate->next_row, pstate->batch_size);

		/* Check if we're past the end of the table */
		if (my_start >= (uint64)pstate->total_rows)
		{
			FAIRCOM_DEBUG_LOG(DEBUG1, "Parallel worker: No more rows to fetch (start=%lu, total=%ld)",
				 (unsigned long)my_start, (long)pstate->total_rows);
			scanstate->eof_reached = true;
			return 0;
		}

		/* Calculate this worker's row range */
		scanstate->worker_start_row = (int64)my_start;
		scanstate->worker_end_row = Min((int64)(my_start + pstate->batch_size), pstate->total_rows);

		FAIRCOM_DEBUG_LOG(DEBUG1, "Parallel worker: Fetching rows %ld to %ld (batch_size=%d)",
			 (long)scanstate->worker_start_row, (long)scanstate->worker_end_row,
			 pstate->batch_size);

		/* Skip to worker's starting position */
		/* Note: This is sequential - could be optimized with FairCom row positioning API */
		if (scanstate->first_iteration)
		{
			ret = ctdbFirstRecord(scanstate->hRecord);
			if (ret == CTDBRET_OK)
			{
				/* Skip to our starting position */
				int64 skip_count = 0;
				while (skip_count < scanstate->worker_start_row && ret == CTDBRET_OK)
				{
					ret = ctdbNextRecord(scanstate->hRecord);
					skip_count++;
				}

				if (ret != CTDBRET_OK && ret != INOT_ERR)
				{
					elog(WARNING, "Parallel worker failed to seek to row %ld (ret=%d)",
						 (long)scanstate->worker_start_row, ret);
					scanstate->eof_reached = true;
					return 0;
				}
			}
			scanstate->first_iteration = false;
		}
	}

	/* ========== IN LIST OPTIMIZATION ========== */
	/* If find_mode is -1, this indicates an IN list pattern was detected.
	 * Use optimized multi-lookup execution instead of scan+filter approach.
	 * This does one index lookup per IN list value, which is much faster
	 * for selective queries like WHERE id IN (1,5,10).
	 */
	if (scanstate->has_predicates && scanstate->find_mode == -1)
	{
		FAIRCOM_DEBUG_LOG(DEBUG1, "Using IN list optimization path");

		batch_count = faircom_fetch_in_list_batch(scanstate->hTable,
												   scanstate->hRecord,
												   scanstate->active_predicates,
												   tupdesc,
												   scanstate->batch_values,
												   scanstate->batch_nulls,
												   max_batch);

		/* Mark EOF after IN list batch (all values fetched) */
		scanstate->eof_reached = true;
		scanstate->first_iteration = false;
		return batch_count;
	}
	/* ========== END IN LIST OPTIMIZATION ========== */

	/* Fetch records until batch is full or EOF */
	while (batch_count < max_batch && !scanstate->eof_reached)
	{
		/* Fetch next record */
		if (scanstate->first_iteration)
		{
			if (scanstate->has_predicates && scanstate->find_mode != 0)
			{
				ret = ctdbFindRecord(scanstate->hRecord, scanstate->find_mode);
			}
			else
			{
				ret = ctdbFirstRecord(scanstate->hRecord);
			}
			scanstate->first_iteration = false;
		}
		else
		{
			ret = ctdbNextRecord(scanstate->hRecord);
		}

		/* Check for end of file */
		if (ret == INOT_ERR)
		{
			scanstate->eof_reached = true;
			break;
		}

		if (ret != CTDBRET_OK)
		{
			faircom_check_result(ret, scanstate->hRecord, "fetching record");
			scanstate->eof_reached = true;
			break;
		}

		/* Field numbers are pre-cached in BeginForeignScan for optimal performance
		 * No on-demand caching needed - field_nums array is already populated
		 */

		/* Check range scan stop condition FIRST (before predicate match check)
		 * This is critical for performance: once we've left the range, stop scanning.
		 * Only applies to indexed range scans (GT/LT/GE/LE).
		 */
		if (scanstate->has_predicates && scanstate->find_mode != 0 && scanstate->find_mode != CTFIND_EQ)
		{
			if (faircom_should_stop_range_scan(scanstate->hRecord,
											   scanstate->active_predicates,
											   scanstate->find_mode))
			{
				FAIRCOM_DEBUG_LOG(DEBUG1, "FairCom FDW: Stopping range scan early (outside predicate range)");
				scanstate->eof_reached = true;
				break;  /* Exit the while loop - we're done */
			}
		}

		/* Check if record matches all predicates */
		/* Note: Even if we used one predicate for index lookup, we still need to check ALL predicates */
		if (scanstate->has_predicates)
		{
			if (!faircom_check_predicate_match(scanstate->hRecord, scanstate->active_predicates))
				continue;
		}

		/* Handle LIMIT - stop if we've returned enough rows */
		/* NOTE: We do NOT handle OFFSET here because PostgreSQL applies it on top of our results */
		if (scanstate->limit_count > 0 && scanstate->rows_returned >= scanstate->limit_count)
		{
			scanstate->eof_reached = true;
			break;
		}

		/* Parallel worker: Stop if we've reached our assigned row range */
		if (scanstate->is_parallel_worker && scanstate->worker_end_row >= 0)
		{
			int64 current_row = scanstate->worker_start_row + scanstate->total_rows_fetched + batch_count;
			if (current_row >= scanstate->worker_end_row)
			{
				scanstate->eof_reached = true;
				break;
			}
		}

		/* Convert FairCom record to Datum array */
		for (j = 0; j < natts; j++)
		{
			Form_pg_attribute attr = TupleDescAttr(tupdesc, j);
			Datum value;
			bool isnull;
			int fieldNum;

			if (attr->attisdropped)
			{
				scanstate->batch_values[batch_count][j] = (Datum) 0;
				scanstate->batch_nulls[batch_count][j] = true;
				continue;
			}

			/* Use cached field number instead of expensive string lookup */
			fieldNum = scanstate->field_nums[j];

			if (fieldNum < 0)
			{
				scanstate->batch_values[batch_count][j] = (Datum) 0;
				scanstate->batch_nulls[batch_count][j] = true;
				continue;
			}

			value = faircom_to_datum(scanstate->hRecord, fieldNum, attr->atttypid,
										   attr->atttypmod, &isnull);

			scanstate->batch_values[batch_count][j] = value;
			scanstate->batch_nulls[batch_count][j] = isnull;

			/* Track approximate row size for adaptive batching
			 * Only track for non-null values
			 */
			if (!isnull)
			{
				/* Rough estimate based on data type */
				switch (attr->atttypid)
				{
					case INT2OID:
					case BOOLOID:
						batch_row_bytes += 2;
						break;
					case INT4OID:
					case FLOAT4OID:
					case DATEOID:
						batch_row_bytes += 4;
						break;
					case INT8OID:
					case FLOAT8OID:
					case TIMESTAMPOID:
					case TIMESTAMPTZOID:
						batch_row_bytes += 8;
						break;
					case TEXTOID:
					case VARCHAROID:
					case BPCHAROID:
						/* For text, estimate based on VARSIZE if varlena */
						if (attr->attlen == -1)
							batch_row_bytes += VARSIZE_ANY(DatumGetPointer(value));
						else
							batch_row_bytes += attr->attlen;
						break;
					default:
						/* Generic estimate */
						if (attr->attlen > 0)
							batch_row_bytes += attr->attlen;
						else
							batch_row_bytes += 32;  /* Conservative estimate */
						break;
				}
			}
		}

		/* Store rowid for row identity */
		{
			CTROWID rowid;
			ret = ctdbGetRowid(scanstate->hRecord, &rowid);
			if (ret == CTDBRET_OK)
			{
				scanstate->batch_rowids[batch_count] = rowid;
			}
			else
			{
				elog(WARNING, "FairCom FDW: Failed to get rowid (ret=%d)", ret);
				scanstate->batch_rowids[batch_count] = 0;
			}
		}

		batch_count++;

		/* Increment rows returned counter for LIMIT tracking */
		scanstate->rows_returned++;
	}

	/* Update batch statistics for adaptive sizing */
	scanstate->total_batches++;
	scanstate->total_rows_fetched += batch_count;

	/* Calculate rolling average row size */
	if (batch_count > 0 && batch_row_bytes > 0)
	{
		int current_avg_row = batch_row_bytes / batch_count;

		if (scanstate->avg_row_size == 0)
		{
			/* First batch - use direct average */
			scanstate->avg_row_size = current_avg_row;
		}
		else
		{
			/* Rolling average: 80% previous + 20% current */
			scanstate->avg_row_size = (scanstate->avg_row_size * 4 + current_avg_row) / 5;
		}
	}

	return batch_count;
}

/*
 * faircomIterateForeignScan
 *		Fetch one row from the foreign source
 */
static TupleTableSlot *
faircomIterateForeignScan(ForeignScanState *node)
{
	FairComScanState *scanstate = (FairComScanState *) node->fdw_state;
	TupleTableSlot *slot = node->ss.ss_ScanTupleSlot;
	TupleDesc tupdesc = slot->tts_tupleDescriptor;
	int i;
	static bool logged_tupdesc = false;

	if (!logged_tupdesc)
	{
		FAIRCOM_DEBUG_LOG(DEBUG1, "FairCom FDW: IterateForeignScan - scan slot has %d attributes", tupdesc->natts);
		for (i = 0; i < tupdesc->natts; i++)
		{
			Form_pg_attribute attr = TupleDescAttr(tupdesc, i);
			FAIRCOM_DEBUG_LOG(DEBUG1, "  Slot attr %d: name=%s, type=%u", i, NameStr(attr->attname), attr->atttypid);
		}
		logged_tupdesc = true;
	}

	ExecClearTuple(slot);

	/* Handle aggregate pushdown */
	if (scanstate->is_aggregate_pushdown)
	{
		bool is_count = (scanstate->aggregate_fnoid == 2147 || scanstate->aggregate_fnoid == 2803);

		/* Special handling for COUNT - return N empty rows */
		if (is_count)
		{
			/* Check if we've returned all COUNT rows */
			if (scanstate->rows_returned >= scanstate->aggregate_result)
			{
				/* All rows returned - signal EOF */
				return slot;  /* Empty slot = EOF */
			}

			/* Return an empty row (all columns NULL) */
			for (i = 0; i < tupdesc->natts; i++)
			{
				slot->tts_values[i] = (Datum) 0;
				slot->tts_isnull[i] = true;
			}

			ExecStoreVirtualTuple(slot);
			scanstate->rows_returned++;

			/* Log progress  every 100 rows or at completion */
			if (scanstate->rows_returned % 100 == 0 ||
				scanstate->rows_returned == scanstate->aggregate_result)
			{
				FAIRCOM_DEBUG_LOG(DEBUG1, "FairCom FDW: COUNT pushdown - returned %lld/%lld rows",
					 (long long) scanstate->rows_returned,
					 (long long) scanstate->aggregate_result);
			}

			return slot;
		}

		/* Only COUNT aggregate pushdown is implemented.
		 * This path should never be reached since BeginForeignScan
		 * only sets is_aggregate_pushdown for COUNT.
		 */
		scanstate->aggregate_returned = true;
		return slot;
	}

	/* Check if we need to fetch a new batch */
	if (scanstate->batch_position >= scanstate->batch_size)
	{
		/* Refill the batch buffer */
		scanstate->batch_size = faircom_fill_batch(scanstate, tupdesc);
		scanstate->batch_position = 0;

		/* If batch is empty, we've reached EOF */
		if (scanstate->batch_size == 0)
		{
			FAIRCOM_DEBUG_LOG(DEBUG1, "FairCom scan complete");
			return slot;  /* Return empty slot */
		}
	}

	/* Fill slot from batch arrays */
	for (i = 0; i < tupdesc->natts; i++)
	{
		slot->tts_values[i] = scanstate->batch_values[scanstate->batch_position][i];
		slot->tts_isnull[i] = scanstate->batch_nulls[scanstate->batch_position][i];
	}

	/* Set tts_tid so ctid can be fetched as a system column */
	{
		CTROWID rowid = scanstate->batch_rowids[scanstate->batch_position];
		if (rowid > 0)
		{
			if (((uint64)rowid >> 48) != 0)
				ereport(ERROR,
						(errcode(ERRCODE_FDW_ERROR),
						 errmsg("rowid value does not fit in ctid encoding: %lld", (long long)rowid)));

			ItemPointerSet(&slot->tts_tid,
						   (BlockNumber)(((uint64)rowid >> 16) & 0xFFFFFFFF),
						   (OffsetNumber)((uint64)rowid & 0xFFFF));
		}
		else
		{
			ItemPointerSetInvalid(&slot->tts_tid);
		}
	}

	scanstate->batch_position++;
	ExecStoreVirtualTuple(slot);

	return slot;
}

/*
 * faircomReScanForeignScan
 *		Restart the scan from the beginning
 *		Required for nested loops and other query patterns that re-execute scans
 */
static void
faircomReScanForeignScan(ForeignScanState *node)
{
	FairComScanState *scanstate = (FairComScanState *) node->fdw_state;

	FAIRCOM_DEBUG_LOG(DEBUG1, "FairCom FDW: ReScan called - resetting scan state");

	if (scanstate)
	{
		/* Reset scan state flags */
		scanstate->first_iteration = true;
		scanstate->eof_reached = false;
		scanstate->rows_returned = 0;
		scanstate->rows_skipped = 0;

		/* Clear batch buffer */
		scanstate->batch_size = 0;
		scanstate->batch_position = 0;

		/* Reset aggregate state if doing aggregate pushdown */
		if (scanstate->is_aggregate_pushdown)
		{
			scanstate->aggregate_returned = false;
			/* Don't reset aggregate_result - it's already computed */
		}

		/* Reset parallel worker state */
		if (scanstate->is_parallel_worker && scanstate->parallel_state)
		{
			FairComParallelScanState *pstate = scanstate->parallel_state;

			/* Reset atomic counter to beginning */
			pg_atomic_write_u64(&pstate->next_row, 0);

			scanstate->worker_start_row = 0;
			scanstate->worker_end_row = -1;

			FAIRCOM_DEBUG_LOG(DEBUG1, "ReScan: Reset parallel worker state");
		}

		FAIRCOM_DEBUG_LOG(DEBUG1, "FairCom FDW: ReScan complete - ready to scan from beginning");
	}
}

/*
 * faircomEndForeignScan
 *		Finish scanning and release resources
 */
static void
faircomEndForeignScan(ForeignScanState *node)
{
	FairComScanState *scanstate = (FairComScanState *) node->fdw_state;

	if (scanstate)
	{
		/* Log final batch statistics (single-table scan) */
		if (scanstate->total_batches > 0)
		{
			float avg_batch = (float)scanstate->total_rows_fetched / scanstate->total_batches;
			FAIRCOM_DEBUG_LOG(DEBUG1, "FairCom scan complete: %d batches, %d rows, %.1f rows/batch, %d bytes/row avg",
				 scanstate->total_batches, scanstate->total_rows_fetched,
				 avg_batch, scanstate->avg_row_size);
		}

		/* Free batch buffers if allocated */
		if (scanstate->batch_values)
		{
			int i;
			for (i = 0; i < FAIRCOM_MAX_BATCH_SIZE; i++)  /* Free maximum size */
			{
				if (scanstate->batch_values[i])
					pfree(scanstate->batch_values[i]);
				if (scanstate->batch_nulls[i])
					pfree(scanstate->batch_nulls[i]);
			}
			pfree(scanstate->batch_values);
			pfree(scanstate->batch_nulls);
			scanstate->batch_values = NULL;
			scanstate->batch_nulls = NULL;
		}

		/* Free FairCom record handle */
		if (scanstate->hRecord)
		{
			ctdbFreeRecord(scanstate->hRecord);
			scanstate->hRecord = NULL;
		}

		/* Close and free FairCom table handle */
		if (scanstate->hTable)
		{
			ctdbCloseTable(scanstate->hTable);
			ctdbFreeTable(scanstate->hTable);
			scanstate->hTable = NULL;
		}

		/* Decrement connection reference count - don't release yet */
		if (scanstate->conn)
		{
			scanstate->conn->ref_count--;
			FAIRCOM_DEBUG_LOG(DEBUG2, "scan ended, connection ref_count now %d", scanstate->conn->ref_count);
			/* Connection stays alive for reuse within transaction */
			scanstate->conn = NULL;
		}

		/* Free table name */
		if (scanstate->tablename)
			pfree(scanstate->tablename);

		/* Free scan state */
		pfree(scanstate);
		node->fdw_state = NULL;
	}
}

/*
 * faircomExplainForeignScan
 *		Produce extra output for EXPLAIN
 */
static void
faircomExplainForeignScan(ForeignScanState *node, ExplainState *es)
{
	FairComScanState *scanstate = (FairComScanState *) node->fdw_state;

	/* Show batch statistics if scan has started */
	if (scanstate && scanstate->total_batches > 0)
	{
		char batch_info[256];

		snprintf(batch_info, sizeof(batch_info),
				 "batches=%d rows=%d batch_size=%d avg_row=%d bytes",
				 scanstate->total_batches,
				 scanstate->total_rows_fetched,
				 scanstate->current_batch_limit,
				 scanstate->avg_row_size);

		ExplainPropertyText("FairCom Batch Stats", batch_info, es);

		/* Show OFFSET skipping statistics */
		if (scanstate->rows_skipped > 0)
		{
			char skip_info[128];
			snprintf(skip_info, sizeof(skip_info),
					 "skipped=%ld rows (OFFSET optimization)",
					 (long)scanstate->rows_skipped);
			ExplainPropertyText("FairCom OFFSET", skip_info, es);
		}

		/* Show batch efficiency */
		if (scanstate->total_batches > 0)
		{
			float avg_batch_fill = (float)scanstate->total_rows_fetched / scanstate->total_batches;
			char efficiency[128];
			snprintf(efficiency, sizeof(efficiency),
					 "%.1f rows/batch (%.0f%% fill rate)",
					 avg_batch_fill,
					 (avg_batch_fill / scanstate->current_batch_limit) * 100);
			ExplainPropertyText("FairCom Batch Efficiency", efficiency, es);
		}

		/* Show parallel worker information */
		if (scanstate->is_parallel_worker && scanstate->parallel_state)
		{
			char parallel_info[256];
			snprintf(parallel_info, sizeof(parallel_info),
					 "worker_range=%ld-%ld nworkers=%d",
					 (long)scanstate->worker_start_row,
					 (long)scanstate->worker_end_row,
					 scanstate->parallel_state->nworkers);
			ExplainPropertyText("FairCom Parallel Worker", parallel_info, es);
		}

		/* Show aggregate pushdown status */
		if (scanstate->is_aggregate_pushdown)
		{
			const char *agg_type = "UNKNOWN";
			if (scanstate->aggregate_fnoid >= 2147 && scanstate->aggregate_fnoid <= 2150)
				agg_type = "COUNT";
			else if (scanstate->aggregate_fnoid >= 2108 && scanstate->aggregate_fnoid <= 2114)
				agg_type = "SUM";
			else if (scanstate->aggregate_fnoid >= 2101 && scanstate->aggregate_fnoid <= 2106)
				agg_type = "AVG";
			else if (scanstate->aggregate_fnoid >= 2131 && scanstate->aggregate_fnoid <= 2146)
				agg_type = "MIN";
			else if (scanstate->aggregate_fnoid >= 2115 && scanstate->aggregate_fnoid <= 2130)
				agg_type = "MAX";

			char agg_info[128];
			snprintf(agg_info, sizeof(agg_info),
					 "type=%s pushed_down=yes",
					 agg_type);
			ExplainPropertyText("FairCom Aggregate", agg_info, es);
		}

		/* Show LIMIT pushdown info */
		if (scanstate->limit_count > 0)
		{
			char limit_info[128];
			snprintf(limit_info, sizeof(limit_info),
					 "limit=%ld rows_fetched=%ld",
					 (long)scanstate->limit_count,
					 (long)scanstate->rows_returned);
			ExplainPropertyText("FairCom Limit", limit_info, es);
		}

		/* Show predicate pushdown info */
		if (scanstate->has_predicates)
		{
			char pred_info[128];
			snprintf(pred_info, sizeof(pred_info),
					 "predicates=%d index_mode=%d",
					 list_length(scanstate->active_predicates),
					 scanstate->find_mode);
			ExplainPropertyText("FairCom Predicates", pred_info, es);
		}
	}
	else
	{
		char batch_info[128];
		char cache_info[128];
		char stats_info[256];

		/* Show current GUC configuration */
		snprintf(batch_info, sizeof(batch_info),
				 "default_size=%d adaptive=enabled",
				 faircom_default_batch_size);
		ExplainPropertyText("FairCom Batch Config", batch_info, es);

		snprintf(cache_info, sizeof(cache_info),
				 "plan_cache=enabled max_entries=%d ttl=%ds",
				 faircom_plan_cache_size,
				 FAIRCOM_PLAN_CACHE_TTL);
		ExplainPropertyText("FairCom Plan Cache", cache_info, es);

		/* Show global performance statistics */
		if (es->verbose && (global_stats.plan_cache_hits + global_stats.plan_cache_misses > 0))
		{
			int64 total_plan = global_stats.plan_cache_hits + global_stats.plan_cache_misses;
			int plan_hit_rate = (int)((global_stats.plan_cache_hits * 100) / total_plan);

			snprintf(stats_info, sizeof(stats_info),
					 "plan_cache_hit_rate=%d%% (%lld/%lld)",
					 plan_hit_rate,
					 (long long)global_stats.plan_cache_hits,
					 (long long)total_plan);
			ExplainPropertyText("FairCom Performance", stats_info, es);

			if (global_stats.query_cache_hits + global_stats.query_cache_misses > 0)
			{
				int64 total_query = global_stats.query_cache_hits + global_stats.query_cache_misses;
				int query_hit_rate = (int)((global_stats.query_cache_hits * 100) / total_query);

				snprintf(stats_info, sizeof(stats_info),
						 "query_cache_hit_rate=%d%% (%lld/%lld)",
						 query_hit_rate,
						 (long long)global_stats.query_cache_hits,
						 (long long)total_query);
				ExplainPropertyText("FairCom Query Cache", stats_info, es);
			}

			if (global_stats.aggregate_pushdowns > 0 || global_stats.predicate_pushdowns > 0)
			{
				snprintf(stats_info, sizeof(stats_info),
						 "aggregates=%lld predicates=%lld",
						 (long long)global_stats.aggregate_pushdowns,
						 (long long)global_stats.predicate_pushdowns);
				ExplainPropertyText("FairCom Pushdown Stats", stats_info, es);
			}

			if (global_stats.connections_created > 0)
			{
				int64 total_conn = global_stats.connections_created + global_stats.connections_reused;
				int reuse_rate = total_conn > 0 ? (int)((global_stats.connections_reused * 100) / total_conn) : 0;

				snprintf(stats_info, sizeof(stats_info),
						 "connection_reuse_rate=%d%% (%lld/%lld)",
						 reuse_rate,
						 (long long)global_stats.connections_reused,
						 (long long)total_conn);
				ExplainPropertyText("FairCom Connections", stats_info, es);
			}
		}
	}
}

/*
 * faircomAnalyzeForeignTable
 *		Collect statistics for a foreign table
 *		Returns true if we can provide statistics, false otherwise
 */
static bool
faircomAnalyzeForeignTable(Relation relation,
						   AcquireSampleRowsFunc *func,
						   BlockNumber *totalpages)
{
	ForeignTable *table;
	ForeignServer *server;
	UserMapping *user;
	FairComConnection *conn;
	CTHANDLE hSession = NULL;
	CTHANDLE hTable = NULL;
	CTHANDLE hRecord = NULL;
	CTDBRET ret;
	CTUINT64 row_count = 0;

	/* Get foreign table/server/user info */
	table = GetForeignTable(RelationGetRelid(relation));
	server = GetForeignServer(table->serverid);
	user = GetUserMapping(GetUserId(), server->serverid);

	FAIRCOM_DEBUG_LOG(DEBUG1, "AnalyzeForeignTable: Collecting statistics for table %s",
		 RelationGetRelationName(relation));

	/* Get connection from pool */
	conn = faircom_get_connection(server, user);
	if (!conn || !conn->hSession)
	{
		elog(WARNING, "AnalyzeForeignTable: Failed to get connection");
		return false;
	}

	hSession = conn->hSession;

	/* Get table name from foreign table options */
	ListCell *lc;
	char *tablename = NULL;
	foreach(lc, table->options)
	{
		DefElem *def = (DefElem *) lfirst(lc);
		if (strcmp(def->defname, "table_name") == 0)
		{
			tablename = defGetString(def);
			break;
		}
	}

	if (!tablename)
	{
		elog(WARNING, "AnalyzeForeignTable: No table_name option found");
		return false;
	}

	/* Allocate table handle */
	hTable = ctdbAllocTable(hSession);
	if (!hTable)
	{
		elog(WARNING, "AnalyzeForeignTable: Failed to allocate table handle");
		return false;
	}

	/* Parse FairCom table name (database.schema.table) */
	char table_path[256];
	char actual_tablename[256];
	char *first_dot = strchr(tablename, '.');
	char *last_dot = strrchr(tablename, '.');

	if (first_dot && last_dot && first_dot != last_dot)
	{
		/* Three-part name: database.schema.table */
		size_t db_len = first_dot - tablename;
		size_t schema_len = last_dot - first_dot - 1;
		const char *schema_start = first_dot + 1;
		const char *table_start = last_dot + 1;

		/* Build path: database.dbs */
		if (db_len < sizeof(table_path) - 4)
		{
			memcpy(table_path, tablename, db_len);
			strcpy(table_path + db_len, ".dbs");

			/* Build table name: schema_table */
			if (schema_len + 1 + strlen(table_start) < sizeof(actual_tablename))
			{
				memcpy(actual_tablename, schema_start, schema_len);
				actual_tablename[schema_len] = '_';
				strcpy(actual_tablename + schema_len + 1, table_start);

				ret = ctdbSetTablePath(hTable, (pTEXT)table_path);
				if (ret != CTDBRET_OK)
					FAIRCOM_DEBUG_LOG(DEBUG1, "Failed to set table path: %d", ret);

				tablename = actual_tablename;
			}
		}
	}

	/* Open table */
	ret = ctdbOpenTable(hTable, (pTEXT)tablename, CTOPEN_NORMAL);
	if (ret != CTDBRET_OK)
	{
		elog(WARNING, "AnalyzeForeignTable: Failed to open table %s: %d", tablename, ret);
		ctdbFreeTable(hTable);
		return false;
	}

	/* Allocate record */
	hRecord = ctdbAllocRecord(hTable);
	if (!hRecord)
	{
		elog(WARNING, "AnalyzeForeignTable: Failed to allocate record");
		ctdbCloseTable(hTable);
		ctdbFreeTable(hTable);
		return false;
	}

	/* Get row count */
	ret = ctdbGetRecordCount(hRecord, &row_count);
	if (ret != CTDBRET_OK)
	{
		elog(WARNING, "AnalyzeForeignTable: ctdbGetRecordCount failed: %d", ret);
		/* Try to estimate by scanning - but don't fail */
		row_count = 100000;  /* Default estimate */
	}

	FAIRCOM_DEBUG_LOG(DEBUG1, "AnalyzeForeignTable: Table %s has %llu rows", tablename, (unsigned long long)row_count);

	/* Clean up */
	ctdbFreeRecord(hRecord);
	ctdbCloseTable(hTable);
	ctdbFreeTable(hTable);

	/* Estimate pages (PostgreSQL uses 8KB pages, assume ~100 rows per page) */
	if (row_count > 0)
	{
		*totalpages = (BlockNumber)((row_count + 99) / 100);  /* Round up */
		if (*totalpages == 0)
			*totalpages = 1;
	}
	else
	{
		*totalpages = 1;
	}

	FAIRCOM_DEBUG_LOG(DEBUG1, "AnalyzeForeignTable: Estimated %u pages for %llu rows",
		 *totalpages, (unsigned long long)row_count);

	/* Don't provide sample function - let PostgreSQL handle sampling itself
	 * Return false to indicate we can't provide custom sampling
	 * PostgreSQL will use default sampling which works fine
	 */
	*func = NULL;

	/* Return false - we provide page estimate but not custom sampling */
	return false;
}

/*
 * faircomImportForeignSchema
 *		Import foreign schema from FairCom
 */
static List *
faircomImportForeignSchema(ImportForeignSchemaStmt *stmt, Oid serverOid)
{
	/* Delegate to schema.c */
	return faircom_import_foreign_schema(stmt, serverOid);
}

/*
 * Validator for foreign server options
 */
PG_FUNCTION_INFO_V1(faircom_fdw_validator);

Datum
faircom_fdw_validator(PG_FUNCTION_ARGS)
{
	List	   *options_list = untransformRelOptions(PG_GETARG_DATUM(0));
	Oid			catalog = PG_GETARG_OID(1);
	ListCell   *cell;

	FAIRCOM_DEBUG_LOG(DEBUG1, "faircom_fdw_validator: catalog=%u", catalog);

	/* Validate options based on catalog type:
	 * catalog values: ForeignTableRelationId, ForeignServerRelationId, UserMappingRelationId
	 * We use catalog Oid to determine which options are valid
	 */

	foreach(cell, options_list)
	{
		DefElem *def = (DefElem *) lfirst(cell);
		bool valid = false;

		/* SERVER options (catalog == ForeignServerRelationId) */
		if (strcmp(def->defname, "host") == 0 ||
			strcmp(def->defname, "port") == 0 ||
			strcmp(def->defname, "database") == 0 ||
			strcmp(def->defname, "connect_timeout") == 0)
		{
			valid = true;
			/* Validate port if specified */
			if (strcmp(def->defname, "port") == 0)
			{
				char *val = defGetString(def);
				char *endptr;
				long port = strtol(val, &endptr, 10);
				if (*endptr != '\0' || port < 1 || port > 65535)
					ereport(ERROR,
							(errcode(ERRCODE_FDW_INVALID_OPTION_INDEX),
							 errmsg("invalid port number: %s", val)));
			}
		}
		/* USER MAPPING options */
		else if (strcmp(def->defname, "username") == 0 ||
				 strcmp(def->defname, "password") == 0)
		{
			valid = true;
		}
		/* FOREIGN TABLE options */
		else if (strcmp(def->defname, "table_name") == 0)
		{
			valid = true;
			/* Validate format: database.schema.table */
			char *val = defGetString(def);
			if (!strchr(val, '.'))
				ereport(WARNING,
						(errmsg("table_name should use format 'database.schema.table'")));
		}
		else if (strcmp(def->defname, "faircom_table") == 0)
		{
			/* Legacy option - accept but warn */
			ereport(WARNING,
					(errmsg("option 'faircom_table' is deprecated, use 'table_name' instead")));
			valid = true;
		}
		else if (strcmp(def->defname, "use_index") == 0 ||
				 strcmp(def->defname, "enable_pushdown") == 0)
		{
			valid = true;
			/* Validate boolean for enable_pushdown */
			if (strcmp(def->defname, "enable_pushdown") == 0)
			{
				char *val = defGetString(def);
				if (strcmp(val, "true") != 0 && strcmp(val, "false") != 0)
					ereport(ERROR,
							(errcode(ERRCODE_FDW_INVALID_OPTION_INDEX),
							 errmsg("enable_pushdown must be 'true' or 'false'")));
			}
		}

		if (!valid)
		{
			ereport(ERROR,
					(errcode(ERRCODE_FDW_INVALID_OPTION_NAME),
					 errmsg("invalid option \"%s\"", def->defname),
					 errhint("Valid SERVER options: host, port, database, connect_timeout"),
					 errhint("Valid USER MAPPING options: username, password"),
					 errhint("Valid FOREIGN TABLE options: table_name, use_index, enable_pushdown")));
		}
	}

	PG_RETURN_VOID();
}

/*
 * faircomIsForeignScanParallelSafe
 *		Determine if this foreign scan is safe for parallel execution
 */
static bool
faircomIsForeignScanParallelSafe(PlannerInfo *root,
								 RelOptInfo *rel,
								 RangeTblEntry *rte)
{
	/*
	 * Parallel scans are disabled for now due to observed backend crashes during
	 * aggregate + join queries. Re-enable after validating parallel worker safety.
	 */
	FAIRCOM_DEBUG_LOG(DEBUG1, "FairCom: Parallel scan disabled to avoid instability");
	return false;
}

/*
 * faircomEstimateDSMForeignScan
 *		Estimate shared memory needed for parallel coordination
 */
static Size
faircomEstimateDSMForeignScan(ForeignScanState *node,
							   ParallelContext *pcxt)
{
	/* We need space for FairComParallelScanState */
	return sizeof(FairComParallelScanState);
}

/*
 * faircomInitializeDSMForeignScan
 *		Initialize shared memory for parallel scan
 */
static void
faircomInitializeDSMForeignScan(ForeignScanState *node,
								 ParallelContext *pcxt,
								 void *coordinate)
{
	FairComParallelScanState *pstate = (FairComParallelScanState *) coordinate;
	FairComScanState *scanstate = (FairComScanState *) node->fdw_state;

	/* Initialize parallel state */
	pstate->nworkers = pcxt->nworkers;
	pstate->total_rows = (int64) node->ss.ps.plan->plan_rows;
	pg_atomic_init_u64(&pstate->next_row, 0);

	/* Each worker gets a batch of rows to minimize contention */
	pstate->batch_size = Max(100, (int) (pstate->total_rows / (pstate->nworkers * 10)));

	FAIRCOM_DEBUG_LOG(DEBUG1, "FairCom: Initialized parallel scan - workers=%d total_rows=%ld batch_size=%d",
		 pstate->nworkers, (long)pstate->total_rows, pstate->batch_size);
}

/*
 * faircomReInitializeDSMForeignScan
 *		Reset parallel scan state for rescan
 */
static void
faircomReInitializeDSMForeignScan(ForeignScanState *node,
								   ParallelContext *pcxt,
								   void *coordinate)
{
	FairComParallelScanState *pstate = (FairComParallelScanState *) coordinate;

	/* Reset row counter for rescan */
	pg_atomic_write_u64(&pstate->next_row, 0);

	FAIRCOM_DEBUG_LOG(DEBUG1, "FairCom: Reinitialized parallel scan");
}

/*
 * faircomInitializeWorkerForeignScan
 *		Initialize worker-specific state
 */
static void
faircomInitializeWorkerForeignScan(ForeignScanState *node,
									shm_toc *toc,
									void *coordinate)
{
	FairComParallelScanState *pstate = (FairComParallelScanState *) coordinate;
	FairComScanState *scanstate = (FairComScanState *) node->fdw_state;

	/* Store pointer to shared parallel state */
	scanstate->parallel_state = pstate;
	scanstate->is_parallel_worker = true;

	FAIRCOM_DEBUG_LOG(DEBUG1, "FairCom: Worker initialized - will fetch rows in batches of %d",
		 pstate->batch_size);
}

/*
 * ======================================================================
 * WRITE OPERATIONS (INSERT/UPDATE/DELETE)
 * ======================================================================
 */

/*
 * Modification state for INSERT/UPDATE/DELETE operations
 */

/*
 * faircomAddForeignUpdateTargets
 *		Add resjunk columns needed for UPDATE/DELETE on foreign tables
 */
static void
faircomAddForeignUpdateTargets(PlannerInfo *root,
								Index rtindex,
								RangeTblEntry *target_rte,
								Relation target_relation)
{
	Var		   *var;

	FAIRCOM_DEBUG_LOG(DEBUG1, "FairCom FDW: AddForeignUpdateTargets called with rtindex=%d", rtindex);

	/*
	 * Add ctid as a junk column to identify the row for UPDATE/DELETE.
	 * We use ctid as a placeholder for the FairCom record position.
	 * Use the official PostgreSQL API which properly registers the row identity column.
	 */
	var = makeVar(rtindex,
				  SelfItemPointerAttributeNumber,
				  TIDOID,
				  -1,
				  InvalidOid,
				  0);

	/* Register it as a row-identity column needed by this target rel */
	add_row_identity_var(root, var, rtindex, "ctid");

	FAIRCOM_DEBUG_LOG(DEBUG1, "FairCom FDW: AddForeignUpdateTargets complete - registered ctid as row identity");
}

/*
 * faircomPlanForeignModify
 *		Plan an INSERT/UPDATE/DELETE operation on a foreign table
 */
static List *
faircomPlanForeignModify(PlannerInfo *root,
						 ModifyTable *plan,
						 Index resultRelation,
						 int subplan_index)
{
	CmdType		operation = plan->operation;
	RangeTblEntry *rte = planner_rt_fetch(resultRelation, root);
	Relation	rel;
	TupleDesc	tupdesc;
	List	   *targetAttrs = NIL;
	int			attnum;

	/* Open relation to get tuple descriptor */
	rel = table_open(rte->relid, NoLock);
	tupdesc = RelationGetDescr(rel);

	/*
	 * Construct list of attribute numbers for the modify target list
	 * (attributes to be modified)
	 */
	if (operation == CMD_INSERT || operation == CMD_UPDATE)
	{
		for (attnum = 1; attnum <= tupdesc->natts; attnum++)
		{
			Form_pg_attribute attr = TupleDescAttr(tupdesc, attnum - 1);

			if (!attr->attisdropped)
				targetAttrs = lappend_int(targetAttrs, attnum);
		}
	}

	table_close(rel, NoLock);

	/*
	 * Return the result as a list:
	 * [0] = operation type (CMD_INSERT, CMD_UPDATE, CMD_DELETE)
	 * [1] = target attribute list
	 */
	return list_make2(makeInteger(operation), targetAttrs);
}

/*
 * faircomBeginForeignModify
 *		Begin an INSERT/UPDATE/DELETE operation on a foreign table
 */
static void
faircomBeginForeignModify(ModifyTableState *mtstate,
						  ResultRelInfo *resultRelInfo,
						  List *fdw_private,
						  int subplan_index,
						  int eflags)
{
	FairComModifyState *fmstate;
	Relation	rel = resultRelInfo->ri_RelationDesc;
	TupleDesc	tupdesc = RelationGetDescr(rel);
	ForeignTable *table;
	ForeignServer *server;
	UserMapping *user;
	List	   *options;
	ListCell   *lc;
	char	   *database = NULL;
	char	   *path = NULL;
	char	   *host = NULL;
	int			port = 5597;  /* default FairCom port */
	char	   *username = NULL;
	char	   *password = NULL;
	char		server_address[256];
	int			ret;

	/* Allocate and initialize modify state */
	fmstate = (FairComModifyState *) palloc0(sizeof(FairComModifyState));
	fmstate->rel = rel;
	fmstate->tupdesc = tupdesc;
	fmstate->num_attrs = tupdesc->natts;

	/* Extract operation type and target attributes from fdw_private */
	fmstate->operation = intVal(linitial(fdw_private));
	if (list_length(fdw_private) >= 2)
	{
		List *targetAttrs = (List *) lsecond(fdw_private);
		int natts = list_length(targetAttrs);
		int i = 0;

		fmstate->target_attrs = (AttrNumber *) palloc(natts * sizeof(AttrNumber));
		foreach(lc, targetAttrs)
		{
			fmstate->target_attrs[i++] = lfirst_int(lc);
		}
	}

	/* Get table, server, and user mapping */
	table = GetForeignTable(RelationGetRelid(rel));
	server = GetForeignServer(table->serverid);
	user = GetUserMapping(GetUserId(), server->serverid);

	/* Extract options from table, server, and user mapping */
	options = NIL;
	options = list_concat(options, table->options);
	options = list_concat(options, server->options);
	options = list_concat(options, user->options);

	foreach(lc, options)
	{
		DefElem    *def = (DefElem *) lfirst(lc);

		if (strcmp(def->defname, "table_name") == 0)
			fmstate->tablename = defGetString(def);
		else if (strcmp(def->defname, "host") == 0)
			host = defGetString(def);
		else if (strcmp(def->defname, "port") == 0)
			port = atoi(defGetString(def));
		else if (strcmp(def->defname, "username") == 0)
			username = defGetString(def);
		else if (strcmp(def->defname, "password") == 0)
			password = defGetString(def);
	}

	if (!fmstate->tablename)
		ereport(ERROR,
				(errcode(ERRCODE_FDW_ERROR),
				 errmsg("table_name option is required")));

	if (!host)
		ereport(ERROR,
				(errcode(ERRCODE_FDW_ERROR),
				 errmsg("host option is required")));

	/* Format server address: #PORT@HOST - The # prefix forces TCP/IP connection */
	snprintf(server_address, sizeof(server_address), "#%d@%s", port, host);

	/* Parse table name: database.schema.table */
	{
		char *dot1 = strchr(fmstate->tablename, '.');
		char *dot2 = dot1 ? strchr(dot1 + 1, '.') : NULL;
		char actual_tablename[256];
		char table_path[256];

		if (!dot2)
			ereport(ERROR,
					(errcode(ERRCODE_FDW_ERROR),
					 errmsg("table_name must be in format: database.schema.table")));

		/* Extract parts */
		size_t db_len = dot1 - fmstate->tablename;
		size_t schema_len = dot2 - dot1 - 1;
		char *schema_start = dot1 + 1;
		char *table_start = dot2 + 1;

		/* Build database path: database.dbs */
		snprintf(table_path, sizeof(table_path), "%.*s.dbs", (int)db_len, fmstate->tablename);
		path = pstrdup(table_path);

		/* Build table name: schema_table */
		snprintf(actual_tablename, sizeof(actual_tablename), "%.*s_%s",
				 (int)schema_len, schema_start, table_start);

		/* Store the actu table name for ctdbOpenTable */
		fmstate->tablename = pstrdup(actual_tablename);

		FAIRCOM_DEBUG_LOG(DEBUG1, "FairCom FDW: Parsed table - path=%s, table=%s", path, fmstate->tablename);
	}

	/* Connect to FairCom */
	fmstate->hSession = ctdbAllocSession(CTSESSION_CTDB);
	if (!fmstate->hSession)
		ereport(ERROR,
				(errcode(ERRCODE_FDW_OUT_OF_MEMORY),
				 errmsg("failed to allocate FairCom session")));

	FAIRCOM_DEBUG_LOG(DEBUG1, "FairCom FDW: Connecting to server=%s, username=%s for write operation",
		 server_address, username ? username : "(null)");

	ret = ctdbLogon(fmstate->hSession, server_address, username, password);
	if (ret != CTDBRET_OK)
	{
		ctdbFreeSession(fmstate->hSession);
		ereport(ERROR,
				(errcode(ERRCODE_FDW_UNABLE_TO_ESTABLISH_CONNECTION),
				 errmsg("could not connect to FairCom server: error %d", ret)));
	}

	/* For SQL databases, we don't use ctdbConnect - the database is just the session handle */
	fmstate->hDatabase = fmstate->hSession;

	/* Open table */
	fmstate->hTable = ctdbAllocTable(fmstate->hDatabase);
	if (!fmstate->hTable)
	{
		ctdbDisconnect(fmstate->hDatabase);
		ctdbFreeDatabase(fmstate->hDatabase);
		ctdbLogout(fmstate->hSession);
		ctdbFreeSession(fmstate->hSession);
		ereport(ERROR,
				(errcode(ERRCODE_FDW_OUT_OF_MEMORY),
				 errmsg("failed to allocate FairCom table handle")));
	}

	/* Set table path */
	ret = ctdbSetTablePath(fmstate->hTable, (pTEXT)path);
	if (ret != CTDBRET_OK)
		FAIRCOM_DEBUG_LOG(DEBUG1, "FairCom FDW: Failed to set table path: %d", ret);

	ret = ctdbOpenTable(fmstate->hTable, fmstate->tablename, CTOPEN_NORMAL);
	if (ret != CTDBRET_OK)
	{
		ctdbFreeTable(fmstate->hTable);
		ctdbLogout(fmstate->hSession);
		ctdbFreeSession(fmstate->hSession);
		ereport(ERROR,
				(errcode(ERRCODE_FDW_TABLE_NOT_FOUND),
				 errmsg("could not open FairCom table: %s", fmstate->tablename)));
	}

	/* Allocate record handle */
	fmstate->hRecord = ctdbAllocRecord(fmstate->hTable);
	if (!fmstate->hRecord)
	{
		ctdbCloseTable(fmstate->hTable);
		ctdbFreeTable(fmstate->hTable);
		ctdbLogout(fmstate->hSession);
		ctdbFreeSession(fmstate->hSession);
		ereport(ERROR,
				(errcode(ERRCODE_FDW_OUT_OF_MEMORY),
				 errmsg("failed to allocate FairCom record handle")));
	}

	/* Start a transaction for write operations */
	ret = ctdbBegin(fmstate->hSession);
	if (ret != CTDBRET_OK)
		FAIRCOM_DEBUG_LOG(DEBUG1, "FairCom FDW: Failed to start transaction: %d (may be okay for some configs)", ret);

	/*
	 * For UPDATE/DELETE, find the ctid junk column in the subplan's target list.
	 * The planner automatically adds this via AddForeignUpdateTargets callback.
	 */
	if (fmstate->operation == CMD_UPDATE || fmstate->operation == CMD_DELETE)
	{
		Plan	   *subplan = outerPlanState(mtstate)->plan;

		fmstate->ctidAttno = ExecFindJunkAttributeInTlist(subplan->targetlist, "ctid");
		if (!AttributeNumberIsValid(fmstate->ctidAttno))
			ereport(ERROR,
					(errcode(ERRCODE_FDW_ERROR),
					 errmsg("could not find junk ctid column")));

		FAIRCOM_DEBUG_LOG(DEBUG1, "FairCom FDW: Found ctid junk column at attribute %d in planSlot",
			 fmstate->ctidAttno);
	}

	resultRelInfo->ri_FdwState = fmstate;
}

/*
 * faircomExecForeignInsert
 *		Insert one tuple into the foreign table
 */
static TupleTableSlot *
faircomExecForeignInsert(EState *estate,
						 ResultRelInfo *resultRelInfo,
						 TupleTableSlot *slot,
						 TupleTableSlot *planSlot)
{
	FairComModifyState *fmstate = (FairComModifyState *) resultRelInfo->ri_FdwState;
	TupleDesc	tupdesc = slot->tts_tupleDescriptor;
	int			ret;
	int			attnum;

	/* Clear the record buffer */
	ret = ctdbClearRecord(fmstate->hRecord);
	if (ret != CTDBRET_OK)
		ereport(ERROR,
				(errcode(ERRCODE_FDW_ERROR),
				 errmsg("failed to clear FairCom record buffer: error %d", ret)));

	FAIRCOM_DEBUG_LOG(DEBUG1, "FairCom FDW: Populating %d attributes for INSERT", tupdesc->natts);

	/* Populate record with values from slot */
	slot_getallattrs(slot);

	for (attnum = 0; attnum < tupdesc->natts; attnum++)
	{
		Form_pg_attribute attr = TupleDescAttr(tupdesc, attnum);
		Datum		value = slot->tts_values[attnum];
		bool		isnull = slot->tts_isnull[attnum];

		if (attr->attisdropped)
			continue;

		if (isnull)
		{
			FAIRCOM_DEBUG_LOG(DEBUG1, "FairCom FDW: Column %d (%s) is NULL", attnum, NameStr(attr->attname));
			/* NULL values - FairCom will handle appropriately */
			continue;
		}

		FAIRCOM_DEBUG_LOG(DEBUG1, "FairCom FDW: Setting column %d (%s) type %u",
			 attnum, NameStr(attr->attname), attr->atttypid);

		/* Map PostgreSQL type to FairCom type and set field */
		switch (attr->atttypid)
		{
			case INT2OID:
				ret = ctdbSetFieldAsSigned(fmstate->hRecord, attnum,
										   DatumGetInt16(value));
				break;

			case INT4OID:
				ret = ctdbSetFieldAsSigned(fmstate->hRecord, attnum,
										   DatumGetInt32(value));
				break;

			case INT8OID:
				ret = ctdbSetFieldAsBigint(fmstate->hRecord, attnum,
										   DatumGetInt64(value));
				break;

			case FLOAT4OID:
			case FLOAT8OID:
				ret = ctdbSetFieldAsFloat(fmstate->hRecord, attnum,
										  DatumGetFloat8(value));
				break;

			case NUMERICOID:
				{
					Numeric		num = DatumGetNumeric(value);
					double		dval = DatumGetFloat8(DirectFunctionCall1(numeric_float8,
												   NumericGetDatum(num)));
					ret = ctdbSetFieldAsFloat(fmstate->hRecord, attnum, dval);
				}
				break;

			case BOOLOID:
				ret = ctdbSetFieldAsBool(fmstate->hRecord, attnum,
										 DatumGetBool(value) ? YES : NO);
				break;

			case VARCHAROID:
			case TEXTOID:
			case BPCHAROID:
				{
					char	   *str = TextDatumGetCString(value);
					ret = ctdbSetFieldAsString(fmstate->hRecord, attnum, str);
					pfree(str);
				}
				break;

			case DATEOID:
				{
					DateADT		pgdate = DatumGetDateADT(value);
					CTDATE		ctdate;
					int			year, month, day;
					int			jd;

					/*
					 * Convert PostgreSQL DateADT to FairCom CTDATE using proper APIs
					 * 1. Convert PostgreSQL DateADT (days since 2000-01-01) to Julian day
					 * 2. Convert Julian day to year/month/day components
					 * 3. Use FairCom's ctdbDatePack to create CTDATE
					 */
					jd = pgdate + POSTGRES_EPOCH_JDATE;
					j2date(jd, &year, &month, &day);
					ret = ctdbDatePack(&ctdate, year, month, day);
					if (ret == CTDBRET_OK)
						ret = ctdbSetFieldAsDate(fmstate->hRecord, attnum, ctdate);
				}
				break;

			case TIMEOID:
			case TIMETZOID:
				{
					/* Time value - needs conversion */
					ret = ctdbSetFieldAsSigned(fmstate->hRecord, attnum,
											   DatumGetInt64(value));
				}
				break;

			case TIMESTAMPOID:
			case TIMESTAMPTZOID:
				{
					/* Timestamp - needs conversion to FairCom format */
					ret = ctdbSetFieldAsBigint(fmstate->hRecord, attnum,
											   DatumGetInt64(value));
				}
				break;

			default:
				/* Unsupported type - try as string */
				{
					Oid			typoutput;
					bool		typisvarlena;
					char	   *str;

					getTypeOutputInfo(attr->atttypid, &typoutput, &typisvarlena);
					str = OidOutputFunctionCall(typoutput, value);
					ret = ctdbSetFieldAsString(fmstate->hRecord, attnum, str);
					pfree(str);
				}
				break;
		}

		if (ret != CTDBRET_OK)
			ereport(ERROR,
					(errcode(ERRCODE_FDW_ERROR),
					 errmsg("failed to set field value for column %s",
							NameStr(attr->attname))));
	}

	/* Write the record to FairCom */
	/* Set default index (index 0 = primary key) for the write operation */
	ret = ctdbSetDefaultIndex(fmstate->hTable, 0);
	if (ret != CTDBRET_OK)
		FAIRCOM_DEBUG_LOG(DEBUG1, "FairCom FDW: Failed to set default index: %d", ret);

	FAIRCOM_DEBUG_LOG(DEBUG1, "FairCom FDW: Writing record to FairCom table");
	ret = ctdbWriteRecord(fmstate->hRecord);
	if (ret != CTDBRET_OK)
		ereport(ERROR,
				(errcode(ERRCODE_FDW_ERROR),
				 errmsg("failed to insert record into FairCom table: error %d", ret)));

	/* Invalidate cached COUNT results for this table after data change */
	faircom_cache_invalidate(fmstate->tablename);

	FAIRCOM_DEBUG_LOG(DEBUG1, "FairCom FDW: INSERT successful");
	return slot;
}

/*
 * faircomExecForeignUpdate
 *		Update one tuple in the foreign table
 */
static TupleTableSlot *
faircomExecForeignUpdate(EState *estate,
						 ResultRelInfo *resultRelInfo,
						 TupleTableSlot *slot,
						 TupleTableSlot *planSlot)
{
	FairComModifyState *fmstate = (FairComModifyState *) resultRelInfo->ri_FdwState;
	TupleDesc	tupdesc = slot->tts_tupleDescriptor;
	CTDBRET		ret;
	int			attnum;
	Datum		ctid_datum;
	bool		ctid_isnull;
	ItemPointer	ctid;
	CTROWID		rowid;

	FAIRCOM_DEBUG_LOG(DEBUG1, "FairCom FDW: UPDATE operation starting");

	/*
	 * Extract ctid from planSlot using the attribute number found in BeginForeignModify.
	 * This is the standard FDW pattern from postgres_fdw for UPDATE/DELETE row identification.
	 */
	ctid_datum = ExecGetJunkAttribute(planSlot, fmstate->ctidAttno, &ctid_isnull);

	if (ctid_isnull)
		ereport(ERROR,
				(errcode(ERRCODE_FDW_ERROR),
				 errmsg("ctid is NULL in planSlot for UPDATE")));

	ctid = (ItemPointer) DatumGetPointer(ctid_datum);
	FAIRCOM_DEBUG_LOG(DEBUG1, "FairCom FDW: Extracted ctid from attribute %d - block=%u, offset=%u",
		 fmstate->ctidAttno,
		 ItemPointerGetBlockNumber(ctid),
		 ItemPointerGetOffsetNumber(ctid));

	/* Decode ctid back to FairCom rowid */
	rowid = ((CTROWID)ItemPointerGetBlockNumber(ctid) << 16) |
			 ItemPointerGetOffsetNumber(ctid);

	FAIRCOM_DEBUG_LOG(DEBUG1, "FairCom FDW: Decoded rowid: %lld", (long long)rowid);

	/* Find record by rowid (loads record buffer) */
	ret = ctdbFindRowid(fmstate->hRecord, rowid, CTFIND_EQ);
	if (ret != CTDBRET_OK)
		ereport(ERROR,
				(errcode(ERRCODE_FDW_ERROR),
				 errmsg("failed to find record for UPDATE by rowid: error %d", ret)));

	/*
	 * Step 3: Update the fields with new values from slot
	 */
	slot_getallattrs(slot);

	FAIRCOM_DEBUG_LOG(DEBUG1, "FairCom FDW: Starting field updates, natts=%d", tupdesc->natts);

	for (attnum = 0; attnum < tupdesc->natts; attnum++)
	{
		Form_pg_attribute attr = TupleDescAttr(tupdesc, attnum);
		Datum		value;
		bool		isnull;

		FAIRCOM_DEBUG_LOG(DEBUG1, "FairCom FDW: Processing field %d, name=%s, dropped=%d",
			 attnum, NameStr(attr->attname), attr->attisdropped);

		if (attr->attisdropped)
			continue;

		value = slot->tts_values[attnum];
		isnull = slot->tts_isnull[attnum];

		FAIRCOM_DEBUG_LOG(DEBUG1, "FairCom FDW: Field %s, isnull=%d", NameStr(attr->attname), isnull);

		/* Handle NULL values */
		if (isnull)
		{
			FAIRCOM_DEBUG_LOG(DEBUG1, "FairCom FDW: Clearing field %d for NULL", attnum);
			ret = ctdbClearField(fmstate->hRecord, attnum);
			if (ret != CTDBRET_OK)
				ereport(ERROR,
						(errcode(ERRCODE_FDW_ERROR),
						 errmsg("failed to clear field for NULL value in UPDATE")));
			continue;
		}

		FAIRCOM_DEBUG_LOG(DEBUG1, "FairCom FDW: Setting field %s, type=%u", NameStr(attr->attname), attr->atttypid);

		/* Set field value based on PostgreSQL type - reuse INSERT logic */
		switch (attr->atttypid)
		{
			case INT2OID:
				FAIRCOM_DEBUG_LOG(DEBUG1, "FairCom FDW: Setting INT2 field %d", attnum);
				ret = ctdbSetFieldAsSigned(fmstate->hRecord, attnum,
										   DatumGetInt16(value));
				break;

			case INT4OID:
				ret = ctdbSetFieldAsSigned(fmstate->hRecord, attnum,
										   DatumGetInt32(value));
				break;

			case INT8OID:
				ret = ctdbSetFieldAsBigint(fmstate->hRecord, attnum,
										   DatumGetInt64(value));
				break;

			case FLOAT4OID:
			case FLOAT8OID:
				ret = ctdbSetFieldAsFloat(fmstate->hRecord, attnum,
										  DatumGetFloat8(value));
				break;

			case NUMERICOID:
				{
					Numeric		num = DatumGetNumeric(value);
					double		dval = DatumGetFloat8(DirectFunctionCall1(numeric_float8,
												   NumericGetDatum(num)));
					ret = ctdbSetFieldAsFloat(fmstate->hRecord, attnum, dval);
				}
				break;

			case BOOLOID:
				ret = ctdbSetFieldAsBool(fmstate->hRecord, attnum,
										 DatumGetBool(value) ? YES : NO);
				break;

			case VARCHAROID:
			case TEXTOID:
			case BPCHAROID:
				{
					char	   *str = TextDatumGetCString(value);
					ret = ctdbSetFieldAsString(fmstate->hRecord, attnum, str);
					pfree(str);
				}
				break;

			case DATEOID:
				{
					DateADT		pgdate = DatumGetDateADT(value);
					CTDATE		ctdate;
					int			year, month, day;
					int			jd;

					jd = pgdate + POSTGRES_EPOCH_JDATE;
					j2date(jd, &year, &month, &day);
					ret = ctdbDatePack(&ctdate, year, month, day);
					if (ret == CTDBRET_OK)
						ret = ctdbSetFieldAsDate(fmstate->hRecord, attnum, ctdate);
				}
				break;

			case TIMEOID:
			case TIMETZOID:
				ret = ctdbSetFieldAsSigned(fmstate->hRecord, attnum,
										   DatumGetInt64(value));
				break;

			case TIMESTAMPOID:
			case TIMESTAMPTZOID:
				ret = ctdbSetFieldAsBigint(fmstate->hRecord, attnum,
										   DatumGetInt64(value));
				break;

			default:
				/* Unsupported type - try as string */
				{
					Oid			typoutput;
					bool		typisvarlena;
					char	   *str;

					getTypeOutputInfo(attr->atttypid, &typoutput, &typisvarlena);
					str = OidOutputFunctionCall(typoutput, value);
					ret = ctdbSetFieldAsString(fmstate->hRecord, attnum, str);
					pfree(str);
				}
				break;
		}

		if (ret != CTDBRET_OK)
			ereport(ERROR,
					(errcode(ERRCODE_FDW_ERROR),
					 errmsg("failed to set field value for UPDATE on column %s: error %d",
							NameStr(attr->attname), ret)));
	}

	/* Step 3: Write the updated record back to FairCom */
	FAIRCOM_DEBUG_LOG(DEBUG1, "FairCom FDW: Writing updated record to FairCom table");
	ret = ctdbWriteRecord(fmstate->hRecord);
	if (ret != CTDBRET_OK)
		ereport(ERROR,
				(errcode(ERRCODE_FDW_ERROR),
				 errmsg("failed to update record in FairCom table: error %d", ret)));

	/* Invalidate cached COUNT results for this table after data change */
	faircom_cache_invalidate(fmstate->tablename);

	FAIRCOM_DEBUG_LOG(DEBUG1, "FairCom FDW: UPDATE successful");
	return slot;
}

/*
 * faircomExecForeignDelete
 *		Delete one tuple from the foreign table
 */
static TupleTableSlot *
faircomExecForeignDelete(EState *estate,
						 ResultRelInfo *resultRelInfo,
						 TupleTableSlot *slot,
						 TupleTableSlot *planSlot)
{
	FairComModifyState *fmstate = (FairComModifyState *) resultRelInfo->ri_FdwState;
	Datum		ctid_datum;
	bool		ctid_isnull;
	ItemPointer ctid;
	CTROWID		rowid;
	CTDBRET		ret;

	FAIRCOM_DEBUG_LOG(DEBUG1, "FairCom FDW: DELETE operation starting");

	/* Extract row identity (ctid) from the junk column in planSlot */
	ctid_datum = ExecGetJunkAttribute(planSlot, fmstate->ctidAttno, &ctid_isnull);
	if (ctid_isnull)
		ereport(ERROR,
				(errcode(ERRCODE_FDW_ERROR),
				 errmsg("ctid is NULL in planSlot for DELETE")));

	ctid = (ItemPointer) DatumGetPointer(ctid_datum);
	FAIRCOM_DEBUG_LOG(DEBUG1, "FairCom FDW: Extracted ctid for DELETE - block=%u, offset=%u",
		 ItemPointerGetBlockNumber(ctid),
		 ItemPointerGetOffsetNumber(ctid));

	/* Decode ctid back to FairCom rowid */
	rowid = ((CTROWID)ItemPointerGetBlockNumber(ctid) << 16) |
			 ItemPointerGetOffsetNumber(ctid);

	FAIRCOM_DEBUG_LOG(DEBUG1, "FairCom FDW: Decoded rowid for DELETE: %lld",
		 (long long)rowid);

	/* Find record by rowid (loads record buffer) */
	ret = ctdbFindRowid(fmstate->hRecord, rowid, CTFIND_EQ);
	if (ret != CTDBRET_OK)
		ereport(ERROR,
				(errcode(ERRCODE_FDW_ERROR),
				 errmsg("failed to find record for DELETE by rowid: error %d", ret)));

	/* Delete the record */
	ret = ctdbDeleteRecord(fmstate->hRecord);
	if (ret != CTDBRET_OK)
		ereport(ERROR,
				(errcode(ERRCODE_FDW_ERROR),
				 errmsg("failed to delete record from FairCom table: error %d", ret)));

	/* Invalidate cached COUNT results for this table after data change */
	faircom_cache_invalidate(fmstate->tablename);

	FAIRCOM_DEBUG_LOG(DEBUG1, "FairCom FDW: DELETE successful");

	return slot;
}

/*
 * faircomEndForeignModify
 *		Finish an INSERT/UPDATE/DELETE operation on a foreign table
 */
static void
faircomEndForeignModify(EState *estate,
						ResultRelInfo *resultRelInfo)
{
	FairComModifyState *fmstate = (FairComModifyState *) resultRelInfo->ri_FdwState;

	if (fmstate)
	{
		/* Commit the transaction */
		if (fmstate->hSession)
		{
			int ret = ctdbCommit(fmstate->hSession);
			if (ret != CTDBRET_OK)
				FAIRCOM_DEBUG_LOG(DEBUG1, "FairCom FDW: Failed to commit transaction: %d", ret);
		}

		if (fmstate->hRecord)
			ctdbFreeRecord(fmstate->hRecord);
		if (fmstate->hTable)
		{
			ctdbCloseTable(fmstate->hTable);
			ctdbFreeTable(fmstate->hTable);
		}
		/* For SQL databases, hDatabase is just an alias for hSession, don't free separately */
		if (fmstate->hSession)
		{
			ctdbLogout(fmstate->hSession);
			ctdbFreeSession(fmstate->hSession);
		}

		pfree(fmstate);
		resultRelInfo->ri_FdwState = NULL;
	}
}

/*
 * faircomIsForeignRelUpdatable
 *		Report which update operations the specified foreign table supports
 *
 * Returns a bitmask of update operations supported:
 *   - (1 << CMD_INSERT) for INSERT
 *   - (1 << CMD_UPDATE) for UPDATE
 *   - (1 << CMD_DELETE) for DELETE
 */
static int
faircomIsForeignRelUpdatable(Relation rel)
{
	int flags = (1 << CMD_INSERT) | (1 << CMD_UPDATE) | (1 << CMD_DELETE);

	/* We support INSERT, UPDATE, and DELETE on all foreign tables */
	return flags;
}

/*
 * ======================================================================
 * END WRITE OPERATIONS
 * ======================================================================
 */

/*
 * faircomShutdownForeignScan
 *		Clean up after parallel scan
 */
static void
faircomShutdownForeignScan(ForeignScanState *node)
{
	FairComScanState *scanstate = (FairComScanState *) node->fdw_state;

	if (scanstate && scanstate->is_parallel_worker)
	{
		FAIRCOM_DEBUG_LOG(DEBUG1, "FairCom: Parallel worker shutting down");
	}
}
