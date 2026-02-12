/*-------------------------------------------------------------------------
 *
 * predicate.c
 *		  Predicate pushdown support for FairCom FDW
 *
 * This module analyzes PostgreSQL WHERE clauses and determines which
 * predicates can be pushed down to FairCom for remote execution.
 *
 * Copyright (c) 2026, FairCom Corporation. All rights reserved.
 * Proprietary and confidential.
 *
 *-------------------------------------------------------------------------
 */
#include "postgres.h"
#include "faircom_fdw.h"

#include "optimizer/optimizer.h"
#include "optimizer/pathnode.h"
#include "optimizer/restrictinfo.h"
#include "optimizer/clauses.h"
#include "nodes/nodeFuncs.h"
#include "nodes/makefuncs.h"
#include "catalog/pg_operator.h"
#include "catalog/pg_proc.h"
#include "catalog/pg_type.h"
#include "utils/lsyscache.h"
#include "utils/syscache.h"
#include "utils/numeric.h"
#include "utils/builtins.h"
#include "utils/array.h"

/*
 * Mapping of PostgreSQL operators to FairCom CTFIND modes
 */
typedef struct OpPushdownMap
{
	const char *opname;		/* PostgreSQL operator name (=, >, etc.) */
	CTFIND_MODE ctfind_mode; /* FairCom CTFIND mode */
	bool		can_use_index; /* Whether this can leverage indexes */
} OpPushdownMap;

/* Define custom find modes for operations not supported by CTDB */
#define CTFIND_LIKE 99   /* LIKE pattern matching (not index-based) */

/* Supported operators for pushdown */
static const OpPushdownMap supported_operators[] = {
	{"=", CTFIND_EQ, true},
	{">", CTFIND_GT, true},
	{">=", CTFIND_GE, true},
	{"<", CTFIND_LT, true},
	{"<=", CTFIND_LE, true},
	{"~~", CTFIND_LIKE, false},  /* LIKE operator (pattern matching, no index) */
	{NULL, 0, false}
};

/*
 * Check if an operator is supported for pushdown
 */
static const OpPushdownMap *
get_operator_pushdown_info(Oid opno)
{
	HeapTuple	tp;
	Form_pg_operator form;
	const char *opname;
	int i;

	tp = SearchSysCache1(OPEROID, ObjectIdGetDatum(opno));
	if (!HeapTupleIsValid(tp))
		return NULL;

	form = (Form_pg_operator) GETSTRUCT(tp);
	opname = NameStr(form->oprname);

	/* Look up operator in our supported list */
	for (i = 0; supported_operators[i].opname != NULL; i++)
	{
		if (strcmp(opname, supported_operators[i].opname) == 0)
		{
			ReleaseSysCache(tp);
			return &supported_operators[i];
		}
	}

	ReleaseSysCache(tp);
	return NULL;
}

/*
 * Forward declaration for helper function used in faircom_check_predicate_match
 */
static bool check_single_predicate(CTHANDLE hRecord, FairComPredicate *pred);
static bool is_in_list_pattern(List *predicates, int *out_attnum);
static bool simple_pattern_match(const char *text, const char *pattern);

/*
 * Simple pattern matching for LIKE operator
 * Supports % (any sequence) and _ (single character) wildcards
 * Returns true if text matches pattern
 */
static bool
simple_pattern_match(const char *text, const char *pattern)
{
	const char *t = text;
	const char *p = pattern;
	const char *star_text = NULL;
	const char *star_pattern = NULL;

	while (*t)
	{
		if (*p == '%')
		{
			/* Remember position for backtracking */
			star_pattern = ++p;
			star_text = t;
		}
		else if (*p == '_' || *p == *t)
		{
			/* Match single character or exact match */
			p++;
			t++;
		}
		else if (star_pattern)
		{
			/* Backtrack to last % */
			p = star_pattern;
			t = ++star_text;
		}
		else
		{
			return false;
		}
	}

	/* Consume trailing % wildcards */
	while (*p == '%')
		p++;

	return (*p == '\0');
}

/*
 * Check if an expression is a simple Var (column reference)
 */
static bool
is_column_ref(Expr *expr, RelOptInfo *baserel, int *varattno)
{
	if (!IsA(expr, Var))
		return false;

	Var *var = (Var *) expr;

	/* Must be a reference to the foreign table */
	if (var->varno != baserel->relid)
		return false;

	/* System columns not supported */
	if (var->varattno < 0)
		return false;

	if (varattno)
		*varattno = var->varattno;

	return true;
}

/*
 * Check if an expression is a constant
 */
static bool
is_constant(Expr *expr)
{
	return IsA(expr, Const);
}

/*
 * Determine if a RestrictInfo clause can be pushed down to FairCom
 *
 * We support:
 * - Simple binary operations: column OP constant (=, >, <, >=, <=)
 * - AND expressions: (column1 OP const1) AND (column2 OP const2)
 * - ScalarArrayOpExpr: column IN (const1, const2, ...)
 */
bool
faircom_is_foreign_expr(PlannerInfo *root, RelOptInfo *baserel, Expr *expr)
{
	OpExpr *op;
	Expr *left;
	Expr *right;
	int varattno;

	/* Handle AND and OR expressions */
	if (IsA(expr, BoolExpr))
	{
		BoolExpr *boolexpr = (BoolExpr *) expr;
		ListCell *lc;

		if (boolexpr->boolop == AND_EXPR)
		{
			/* All arguments must be pushable */
			foreach(lc, boolexpr->args)
			{
				Expr *arg = (Expr *) lfirst(lc);
				if (!faircom_is_foreign_expr(root, baserel, arg))
					return false;
			}
			return true;
		}
		else if (boolexpr->boolop == OR_EXPR)
		{
			/* OR is pushable if it can be converted to IN list:
			 * All clauses must be equality (=) on the same column
			 */
			int first_attnum = -1;
			bool all_valid = true;

			foreach(lc, boolexpr->args)
			{
				Expr *arg = (Expr *) lfirst(lc);
				OpExpr *op;
				Expr *left, *right;
				int varattno;

				/* Must be OpExpr */
				if (!IsA(arg, OpExpr))
				{
					all_valid = false;
					break;
				}

				op = (OpExpr *) arg;

				/* Must be equality operator */
				if (get_operator_pushdown_info(op->opno) == NULL ||
					get_operator_pushdown_info(op->opno)->ctfind_mode != CTFIND_EQ)
				{
					all_valid = false;
					break;
				}

				/* Must be column OP constant */
				left = (Expr *) linitial(op->args);
				right = (Expr *) lsecond(op->args);

				if (!(is_column_ref(left, baserel, &varattno) && is_constant(right)) &&
					!(is_constant(left) && is_column_ref(right, baserel, &varattno)))
				{
					all_valid = false;
					break;
				}

				/* Check all on same column */
				if (first_attnum == -1)
					first_attnum = varattno;
				else if (first_attnum != varattno)
				{
					all_valid = false;
					break;
				}
			}

			return all_valid;
		}

		/* NOT and other bool ops not supported */
		return false;
	}

	/* Handle IN lists: column IN (const1, const2, ...) */
	if (IsA(expr, ScalarArrayOpExpr))
	{
		ScalarArrayOpExpr *saop = (ScalarArrayOpExpr *) expr;
		Expr *arrayexpr;

		/* Must be ANY (IN), not ALL */
		if (!saop->useOr)
			return false;

		/* Left side must be a column reference */
		if (!is_column_ref((Expr *) linitial(saop->args), baserel, &varattno))
			return false;

		/* Right side must be an array constant */
		arrayexpr = (Expr *) lsecond(saop->args);
		if (!IsA(arrayexpr, Const))
			return false;

		/* Must use equality operator */
		HeapTuple tp = SearchSysCache1(OPEROID, ObjectIdGetDatum(saop->opno));
		if (!HeapTupleIsValid(tp))
			return false;

		Form_pg_operator form = (Form_pg_operator) GETSTRUCT(tp);
		const char *opname = NameStr(form->oprname);
		bool is_eq = (strcmp(opname, "=") == 0);
		ReleaseSysCache(tp);

		return is_eq;
	}

	/* Handle simple binary operations: column OP constant */
	if (!IsA(expr, OpExpr))
		return false;

	op = (OpExpr *) expr;

	/* Must have exactly 2 arguments */
	if (list_length(op->args) != 2)
		return false;

	/* Check if operator is supported */
	if (get_operator_pushdown_info(op->opno) == NULL)
		return false;

	/* Get left and right operands */
	left = (Expr *) linitial(op->args);
	right = (Expr *) lsecond(op->args);

	/* Check for: column OP constant */
	if (is_column_ref(left, baserel, &varattno) && is_constant(right))
		return true;

	/* Check for: constant OP column (reverse) */
	if (is_constant(left) && is_column_ref(right, baserel, &varattno))
		return true;

	return false;
}

/*
 * Classify WHERE clause predicates into remote (pushable) and local
 */
void
faircom_classify_conditions(PlannerInfo *root,
							 RelOptInfo *baserel,
							 List **remote_conds,
							 List **local_conds)
{
	ListCell *lc;

	*remote_conds = NIL;
	*local_conds = NIL;

	foreach(lc, baserel->baserestrictinfo)
	{
		RestrictInfo *ri = (RestrictInfo *) lfirst(lc);

		if (faircom_is_foreign_expr(root, baserel, ri->clause))
			*remote_conds = lappend(*remote_conds, ri);
		else
			*local_conds = lappend(*local_conds, ri);
	}
}

/*
 * Extract predicate information for execution
 */
FairComPredicate *
faircom_extract_predicate(Expr *expr, TupleDesc tupdesc)
{
	OpExpr *op;
	Expr *left, *right;
	Var *var;
	Const *constant;
	FairComPredicate *pred;
	const OpPushdownMap *op_info;

	if (!IsA(expr, OpExpr))
		return NULL;

	op = (OpExpr *) expr;
	op_info = get_operator_pushdown_info(op->opno);

	if (!op_info)
		return NULL;

	left = (Expr *) linitial(op->args);
	right = (Expr *) lsecond(op->args);

	/* Determine which is the column and which is the constant */
	if (IsA(left, Var) && IsA(right, Const))
	{
		var = (Var *) left;
		constant = (Const *) right;
	}
	else if (IsA(left, Const) && IsA(right, Var))
	{
		constant = (Const *) left;
		var = (Var *) right;
	}
	else
		return NULL;

	/* Allocate predicate structure */
	pred = (FairComPredicate *) palloc0(sizeof(FairComPredicate));

	pred->attnum = var->varattno;
	pred->attname = NameStr(TupleDescAttr(tupdesc, var->varattno - 1)->attname);
	pred->ctfind_mode = op_info->ctfind_mode;
	pred->const_value = constant->constvalue;
	pred->const_isnull = constant->constisnull;
	pred->const_type = constant->consttype;

	return pred;
}

/*
 * Extract all predicates from a complex expression (including AND, OR, IN)
 * Returns a List of FairComPredicate structures
 */
List *
faircom_extract_predicates_from_expr(Expr *expr, TupleDesc tupdesc)
{
	List *result = NIL;
	ListCell *lc;

	/* Handle AND expressions - extract all sub-predicates */
	if (IsA(expr, BoolExpr))
	{
		BoolExpr *boolexpr = (BoolExpr *) expr;

		if (boolexpr->boolop == AND_EXPR)
		{
			/* Recursively extract predicates from each AND clause */
			foreach(lc, boolexpr->args)
			{
				Expr *arg = (Expr *) lfirst(lc);
				List *sub_preds = faircom_extract_predicates_from_expr(arg, tupdesc);
				result = list_concat(result, sub_preds);
			}
			return result;
		}
		else if (boolexpr->boolop == OR_EXPR)
		{
			/* OR expression handling:
			 * Try to convert to IN list pattern if all OR clauses are equality on same column
			 * e.g., "id = 1 OR id = 5 OR id = 10" -> IN list with values (1, 5, 10)
			 */
			List *or_predicates = NIL;
			int first_attnum = -1;
			bool can_convert_to_in_list = true;

			FAIRCOM_DEBUG_LOG(DEBUG1, "Processing OR expression with %d clauses", list_length(boolexpr->args));

			/* Extract predicates from each OR clause */
			foreach(lc, boolexpr->args)
			{
				Expr *arg = (Expr *) lfirst(lc);
				FairComPredicate *pred = faircom_extract_predicate(arg, tupdesc);

				if (pred == NULL)
				{
					/* Can't extract predicate - OR not fully pushable */
					can_convert_to_in_list = false;
					break;
				}

				/* Check if all predicates are EQ on same column */
				if (pred->ctfind_mode != CTFIND_EQ)
				{
					/* Non-equality predicate - can't convert to IN list */
					can_convert_to_in_list = false;
					FAIRCOM_DEBUG_LOG(DEBUG1, "OR clause has non-equality predicate (mode=%d) - can't convert to IN list",
						 pred->ctfind_mode);
					break;
				}

				if (first_attnum == -1)
				{
					first_attnum = pred->attnum;
				}
				else if (pred->attnum != first_attnum)
				{
					/* Different columns - can't convert to IN list */
					can_convert_to_in_list = false;
					FAIRCOM_DEBUG_LOG(DEBUG1, "OR clauses reference different columns (%d vs %d) - can't convert to IN list",
						 first_attnum, pred->attnum);
					break;
				}

				or_predicates = lappend(or_predicates, pred);
			}

			if (can_convert_to_in_list && or_predicates != NIL)
			{
				/* Successfully converted OR to IN list pattern */
				FAIRCOM_DEBUG_LOG(DEBUG1, "Converted OR expression to IN list with %d values on column attnum=%d",
					 list_length(or_predicates), first_attnum);
				return or_predicates;
			}
			else
			{
				/* Can't convert to IN list - OR not supported yet */
				FAIRCOM_DEBUG_LOG(DEBUG1, "OR expression cannot be converted to IN list - not yet supported");
				return NIL;
			}
		}
	}

	/* Handle IN lists: column IN (const1, const2, ...) */
	if (IsA(expr, ScalarArrayOpExpr))
	{
		ScalarArrayOpExpr *saop = (ScalarArrayOpExpr *) expr;
		Var *var;
		Const *arrayconst;
		ArrayType *array;
		int nelems;
		Datum *elem_values;
		bool *elem_nulls;
		int16 elmlen;
		bool elmbyval;
		char elmalign;
		int i;

		/* Extract column and array */
		if (!IsA(linitial(saop->args), Var))
			return NIL;

		var = (Var *) linitial(saop->args);

		if (!IsA(lsecond(saop->args), Const))
			return NIL;

		arrayconst = (Const *) lsecond(saop->args);
		if (arrayconst->constisnull)
			return NIL;

		/* Deconstruct the array */
		array = DatumGetArrayTypeP(arrayconst->constvalue);
		get_typlenbyvalalign(ARR_ELEMTYPE(array), &elmlen, &elmbyval, &elmalign);

		deconstruct_array(array, ARR_ELEMTYPE(array),
						  elmlen, elmbyval, elmalign,
						  &elem_values, &elem_nulls, &nelems);

		/* Create an equality predicate for each array element */
		for (i = 0; i < nelems; i++)
		{
			FairComPredicate *pred;

			if (elem_nulls[i])
				continue;  /* Skip NULL values */

			pred = (FairComPredicate *) palloc0(sizeof(FairComPredicate));
			pred->attnum = var->varattno;
			pred->attname = NameStr(TupleDescAttr(tupdesc, var->varattno - 1)->attname);
			pred->ctfind_mode = CTFIND_EQ;  /* IN uses equality */
			pred->const_value = elem_values[i];
			pred->const_isnull = false;
			pred->const_type = ARR_ELEMTYPE(array);

			result = lappend(result, pred);
		}

		if (nelems > 0)
		{
			FAIRCOM_DEBUG_LOG(DEBUG1, "Extracted %d predicates from IN list for column %s",
				 nelems, NameStr(TupleDescAttr(tupdesc, var->varattno - 1)->attname));
		}

		return result;
	}

	/* Handle simple OpExpr - single predicate */
	if (IsA(expr, OpExpr))
	{
		FairComPredicate *pred = faircom_extract_predicate(expr, tupdesc);
		if (pred)
			result = lappend(result, pred);
		return result;
	}

	return NIL;
}

/*
 * Detect if all predicates are EQ on the same column (IN list pattern)
 * Returns true if this is an IN list, false otherwise
 * If true, outputs the column attribute number in out_attnum
 */
static bool
is_in_list_pattern(List *predicates, int *out_attnum)
{
	ListCell *lc;
	int first_attnum = -1;

	if (list_length(predicates) <= 1)
		return false;  /* Not an IN list if only one predicate */

	foreach(lc, predicates)
	{
		FairComPredicate *pred = (FairComPredicate *) lfirst(lc);

		/* All predicates must be EQ mode */
		if (pred->ctfind_mode != CTFIND_EQ)
			return false;

		/* All predicates must be on the same column */
		if (first_attnum == -1)
			first_attnum = pred->attnum;
		else if (pred->attnum != first_attnum)
			return false;  /* Different columns, not an IN list */
	}

	if (first_attnum != -1 && out_attnum)
		*out_attnum = first_attnum;

	return (first_attnum != -1);
}

/*
 * Apply predicates to FairCom table/record
 * Returns the CTFIND_MODE to use for the first record lookup, or 0 if no predicates
 *
 * Special case: If all predicates are EQ on the same column (IN list pattern),
 * returns -1 to signal that the caller should use optimized IN list batch lookup
 * instead of the normal iteration approach.
 */
CTFIND_MODE
faircom_apply_predicates(CTHANDLE hTable,
						 CTHANDLE hRecord,
						 List *predicates,
						 TupleDesc tupdesc)
{
	ListCell *lc;
	CTFIND_MODE result_mode = 0;
	bool first_predicate = true;
	char index_name[256];
	NINT index_nbr;
	CTDBRET ret;
	FairComPredicate *index_pred = NULL;
	int in_list_attnum;

	if (predicates == NIL)
		return 0;

	/* Check if this is an IN list pattern (multiple EQ on same column) */
	if (is_in_list_pattern(predicates, &in_list_attnum))
	{
		FAIRCOM_DEBUG_LOG(DEBUG1, "Detected IN list pattern with %d values on column attnum=%d",
			 list_length(predicates), in_list_attnum);
		FAIRCOM_DEBUG_LOG(DEBUG1, "Will use optimized multi-lookup execution instead of scan+filter");

		/* Return special value -1 to signal IN list optimization should be used */
		return -1;
	}

	/* Select the best predicate for index usage:
	 * Skip LIKE predicates (cannot use index)
	 * Prefer GT/GE/LT/LE over EQ (range scans better than point lookups for IN lists)
	 * Use the first non-EQ predicate if available, otherwise use first predicate
	 */
	foreach(lc, predicates)
	{
		FairComPredicate *pred = (FairComPredicate *) lfirst(lc);
		/* Skip LIKE predicates - they cannot use indexes */
		if (pred->ctfind_mode == CTFIND_LIKE)
			continue;
		if (pred->ctfind_mode != CTFIND_EQ)
		{
			index_pred = pred;
			break;
		}
	}

	/* If no range predicate found, use first non-LIKE predicate */
	if (index_pred == NULL)
	{
		foreach(lc, predicates)
		{
			FairComPredicate *pred = (FairComPredicate *) lfirst(lc);
			if (pred->ctfind_mode != CTFIND_LIKE)
			{
				index_pred = pred;
				break;
			}
		}
	}

	/* If all predicates are LIKE, we cannot use index */
	if (index_pred == NULL)
	{
		FAIRCOM_DEBUG_LOG(DEBUG1, "All predicates are LIKE - cannot use index, will scan all records");
		return 0;
	}

	/* Try index name based on field: idx_{fieldname} */
	snprintf(index_name, sizeof(index_name), "idx_%s", index_pred->attname);

	FAIRCOM_DEBUG_LOG(DEBUG1, "Looking for index named '%s' for field %s (mode=%d)",
		 index_name, index_pred->attname, index_pred->ctfind_mode);

	index_nbr = ctdbGetIndexNbrByName(hTable, index_name);
	FAIRCOM_DEBUG_LOG(DEBUG1, "ctdbGetIndexNbrByName returned: %d", index_nbr);

	if (index_nbr >= 0)
	{
		/* Set default index on the RECORD handle, not the table */
		ret = ctdbSetDefaultIndex(hRecord, index_nbr);
		if (ret == CTDBRET_OK)
		{
			FAIRCOM_DEBUG_LOG(DEBUG1, "[OK] Set default index to '%s' (index %d) for field %s",
				 index_name, index_nbr, index_pred->attname);
		}
		else
		{
			FAIRCOM_DEBUG_LOG(DEBUG1, "Failed to set default index '%s': %d", index_name, ret);
		}
	}
	else
	{
		FAIRCOM_DEBUG_LOG(DEBUG1, "No index found with name '%s' for field %s",
			 index_name, index_pred->attname);
	}

	/* Apply ONLY the selected index predicate for the initial lookup
	 * All other predicates will be checked during iteration
	 */
	{
		FairComPredicate *pred = index_pred;
		NINT field_num = -1;
		char upper_name[256];
		int i;
		NINT num_fields;

		/* Get number of fields */
		num_fields = ctdbGetTableFieldCount(hTable);

		/* Manual field lookup by iterating through all fields */
		for (i = 0; i < num_fields; i++)
		{
			CTHANDLE hField = ctdbGetField(hTable, i);
			if (hField)
			{
				pTEXT fname = ctdbGetFieldName(hField);
				if (fname && strcmp(fname, pred->attname) == 0)
				{
					field_num = i;
					FAIRCOM_DEBUG_LOG(DEBUG1, "Found FairCom field: %s at position %d", pred->attname, field_num);
					break;
				}
			}
		}

		/* If not found with exact match, try case-insensitive */
		if (field_num < 0)
		{
			for (i = 0; pred->attname[i] && i < sizeof(upper_name) - 1; i++)
				upper_name[i] = toupper((unsigned char)pred->attname[i]);
			upper_name[i] = '\0';

			for (i = 0; i < num_fields; i++)
			{
				CTHANDLE hField = ctdbGetField(hTable, i);
				if (hField)
				{
					pTEXT fname = ctdbGetFieldName(hField);
					char upper_fname[256];
					int j;

					if (fname)
					{
						for (j = 0; fname[j] && j < sizeof(upper_fname) - 1; j++)
							upper_fname[j] = toupper((unsigned char)fname[j]);
						upper_fname[j] = '\0';

						if (strcmp(upper_fname, upper_name) == 0)
						{
							field_num = i;
							FAIRCOM_DEBUG_LOG(DEBUG1, "Found FairCom field (case-insensitive): %s at position %d", pred->attname, field_num);
							break;
						}
					}
				}
			}
		}

		if (field_num < 0)
		{
			elog(WARNING, "could not find FairCom field: %s", pred->attname);
			return 0;  /* No index used */
		}

		FAIRCOM_DEBUG_LOG(DEBUG1, "Applying index predicate on field: %s (position %d)", pred->attname, field_num);

		/* Skip null constants */
		if (pred->const_isnull)
			return 0;  /* No index used */

		/* Apply filter based on data type */
		switch (pred->const_type)
		{
			case INT2OID:
			case INT4OID:
				{
					int32 value = DatumGetInt32(pred->const_value);
					ctdbSetFieldAsSigned(hRecord, field_num, value);
					result_mode = pred->ctfind_mode;
					FAIRCOM_DEBUG_LOG(DEBUG2, "Applied index predicate: %s = %d (field %d, mode=%d)",
						 pred->attname, value, field_num, result_mode);
				}
				break;

			case INT8OID:
				{
					int64 value = DatumGetInt64(pred->const_value);
					ctdbSetFieldAsSigned(hRecord, field_num, (CTSIGNED)value);
					result_mode = pred->ctfind_mode;
					FAIRCOM_DEBUG_LOG(DEBUG2, "Applied index predicate: %s = %lld (field %d, mode=%d)",
						 pred->attname, (long long)value, field_num, result_mode);
				}
				break;

			case FLOAT4OID:
			case FLOAT8OID:
				{
					float8 value = DatumGetFloat8(pred->const_value);
					ctdbSetFieldAsFloat(hRecord, field_num, value);
					result_mode = pred->ctfind_mode;
					FAIRCOM_DEBUG_LOG(DEBUG2, "Applied index predicate: %s = %f (field %d, mode=%d)",
						 pred->attname, value, field_num, result_mode);
				}
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

					ctdbSetFieldAsFloat(hRecord, field_num, pred->cached_float8_value);
					result_mode = pred->ctfind_mode;
					FAIRCOM_DEBUG_LOG(DEBUG2, "Applied index predicate: %s = %f (field %d, mode=%d from NUMERIC)",
						 pred->attname, pred->cached_float8_value, field_num, result_mode);
				}
				break;

			default:
				FAIRCOM_DEBUG_LOG(DEBUG1, "Unsupported predicate type: %u for field %s",
					 pred->const_type, pred->attname);
				break;
		}
	}

	return result_mode;
}

/*
 * Check if we should stop a range scan
 *
 * For indexed range scans (GT/LT/GE/LE), once we encounter a record that fails
 * the index predicate, all subsequent records will also fail (due to index ordering).
 * This function detects that condition so we can stop scanning early.
 *
 * Returns true if we should STOP scanning, false if we should continue.
 */
bool
faircom_should_stop_range_scan(CTHANDLE hRecord,
							   List *predicates,
							   CTFIND_MODE find_mode)
{
	ListCell *lc;
	FairComPredicate *index_pred = NULL;

	/* Only relevant for range scans */
	if (find_mode == CTFIND_EQ || find_mode == 0)
		return false;

	/* Find the predicate that matches the find_mode (the index predicate) */
	foreach(lc, predicates)
	{
		FairComPredicate *pred = (FairComPredicate *) lfirst(lc);
		if (pred->ctfind_mode == find_mode)
		{
			index_pred = pred;
			break;
		}
	}

	if (index_pred == NULL)
		return false;  /* Shouldn't happen, but be safe */

	/* Check if current record violates the range predicate
	 * If it does, all subsequent records will too (due to index ordering)
	 */
	CTSIGNED int_value;
	int32 const_int32;
	int64 const_int64;
	CTFLOAT float_value;
	float8 const_float;
	CTDBRET ret;
	bool stop_scan = false;

	switch (index_pred->const_type)
	{
		case INT2OID:  /* smallint */
		case INT4OID:  /* int */
			ret = ctdbGetFieldAsSigned(hRecord, index_pred->attnum - 1, &int_value);
			if (ret != CTDBRET_OK)
				return false;

			if (index_pred->const_type == INT2OID)
				const_int32 = (int32) DatumGetInt16(index_pred->const_value);
			else
				const_int32 = (int32) DatumGetInt32(index_pred->const_value);

			switch (find_mode)
			{
				case CTFIND_GT:   /* salary > 50000: stop when salary <= 50000 */
					stop_scan = (int_value <= const_int32);
					break;
				case CTFIND_GE:   /* salary >= 50000: stop when salary < 50000 */
					stop_scan = (int_value < const_int32);
					break;
				case CTFIND_LT:   /* salary < 50000: stop when salary >= 50000 */
					stop_scan = (int_value >= const_int32);
					break;
				case CTFIND_LE:   /* salary <= 50000: stop when salary > 50000 */
					stop_scan = (int_value > const_int32);
					break;
				default:
					break;
			}
			break;

		case INT8OID:  /* bigint */
			ret = ctdbGetFieldAsSigned(hRecord, index_pred->attnum - 1, &int_value);
			if (ret != CTDBRET_OK)
				return false;

			const_int64 = DatumGetInt64(index_pred->const_value);

			switch (find_mode)
			{
				case CTFIND_GT:
					stop_scan = (int_value <= const_int64);
					break;
				case CTFIND_GE:
					stop_scan = (int_value < const_int64);
					break;
				case CTFIND_LT:
					stop_scan = (int_value >= const_int64);
					break;
				case CTFIND_LE:
					stop_scan = (int_value > const_int64);
					break;
				default:
					break;
			}
			break;

		case FLOAT4OID:  /* real */
		case FLOAT8OID:  /* double precision */
		case NUMERICOID: /* numeric/decimal */
			ret = ctdbGetFieldAsFloat(hRecord, index_pred->attnum - 1, &float_value);
			if (ret != CTDBRET_OK)
				return false;

			if (index_pred->const_type == FLOAT4OID)
				const_float = (float8) DatumGetFloat4(index_pred->const_value);
			else if (index_pred->const_type == FLOAT8OID)
				const_float = DatumGetFloat8(index_pred->const_value);
			else  /* NUMERICOID */
			{
				/* Use cached conversion to avoid repeated DirectFunctionCall1 overhead */
				if (!index_pred->cached_float8_valid)
				{
					Datum float8_datum = DirectFunctionCall1(numeric_float8, index_pred->const_value);
					index_pred->cached_float8_value = DatumGetFloat8(float8_datum);
					index_pred->cached_float8_valid = true;
				}
				const_float = index_pred->cached_float8_value;
			}

			switch (find_mode)
			{
				case CTFIND_GT:
					stop_scan = (float_value <= const_float);
					break;
				case CTFIND_GE:
					stop_scan = (float_value < const_float);
					break;
				case CTFIND_LT:
					stop_scan = (float_value >= const_float);
					break;
				case CTFIND_LE:
					stop_scan = (float_value > const_float);
					break;
				default:
					break;
			}
			break;

		default:
			/* For other types (strings, dates, etc.), we can't easily determine
			 * stop conditions without more complex comparison logic. Fall back to
			 * checking all records.
			 */
			return false;
	}

	if (stop_scan)
	{
		FAIRCOM_DEBUG_LOG(DEBUG1, "FairCom FDW: Range scan stop condition detected - ending scan early");
	}

	return stop_scan;
}

/*
 * Check if the current record matches the applied predicates
 * Used during iteration to determine when to stop scanning
 */
bool
faircom_check_predicate_match(CTHANDLE hRecord,
							   List *predicates)
{
	ListCell *lc;
	int *checked_eq_fields;  /* Fields where we've processed IN lists (multiple EQs) */
	int num_checked = 0;
	int max_checked = 10;  /* Initial size */

	if (predicates == NIL)
		return true;  /* No predicates, all records match */

	/* Track fields where we've handled IN lists to avoid re-processing */
	checked_eq_fields = (int *) palloc(max_checked * sizeof(int));

	/* Check each predicate
	 * - For EQ predicates: group by field and use OR logic (IN list)
	 * - For other predicates (GT/LT/GE/LE): check each individually (AND logic)
	 */
	foreach(lc, predicates)
	{
		FairComPredicate *pred = (FairComPredicate *) lfirst(lc);
		bool field_already_checked = false;
		bool field_matched = false;
		int i;

		/* For EQ predicates, check if we've already processed this field's IN list */
		if (pred->ctfind_mode == CTFIND_EQ)
		{
			for (i = 0; i < num_checked; i++)
			{
				if (checked_eq_fields[i] == pred->attnum)
				{
					field_already_checked = true;
					break;
				}
			}

			if (field_already_checked)
				continue;  /* Already processed all EQ predicates for this field */

			/* Count how many EQ predicates exist for this field */
			ListCell *find_lc;
			int num_eq_for_field = 0;

			foreach(find_lc, predicates)
			{
				FairComPredicate *p = (FairComPredicate *) lfirst(find_lc);
				if (p->attnum == pred->attnum && p->ctfind_mode == CTFIND_EQ)
					num_eq_for_field++;
			}

			/* If multiple EQ predicates for same field, use OR logic (IN list) */
			if (num_eq_for_field > 1)
			{
				/* Check if record matches ANY of the values */
				foreach(find_lc, predicates)
				{
					FairComPredicate *eq_pred = (FairComPredicate *) lfirst(find_lc);

					if (eq_pred->attnum == pred->attnum && eq_pred->ctfind_mode == CTFIND_EQ)
					{
						if (check_single_predicate(hRecord, eq_pred))
						{
							field_matched = true;
							break;
						}
					}
				}

				if (!field_matched)
				{
					pfree(checked_eq_fields);
					return false;  /* None of the IN list values matched */
				}

				/* Mark this field's IN list as checked */
				if (num_checked >= max_checked)
				{
					max_checked *= 2;
					checked_eq_fields = (int *) repalloc(checked_eq_fields, max_checked * sizeof(int));
				}
				checked_eq_fields[num_checked++] = pred->attnum;
				continue;  /* IN list handled, move to next predicate */
			}
		}

		/* Single predicate (GT/LT/GE/LE or single EQ) - must match (AND logic) */
		if (!check_single_predicate(hRecord, pred))
		{
			pfree(checked_eq_fields);
			return false;
		}
	}

	pfree(checked_eq_fields);
	return true;  /* All predicates matched */
}

/*
 * Check a single predicate against the current record
 * Returns true if the predicate matches, false otherwise
 */
static bool
check_single_predicate(CTHANDLE hRecord, FairComPredicate *pred)
{
	CTSIGNED int_value;
	int32 const_int32;
	int64 const_int64;
	CTFLOAT float_value;
	float8 const_float;

	switch (pred->const_type)
	{
		case INT2OID:
		case INT4OID:
			const_int32 = DatumGetInt32(pred->const_value);
			if (ctdbGetFieldAsSigned(hRecord, pred->attnum - 1, &int_value) != CTDBRET_OK)
				return false;

			switch (pred->ctfind_mode)
			{
				case CTFIND_EQ: return (int_value == const_int32);
				case CTFIND_GT: return (int_value > const_int32);
				case CTFIND_GE: return (int_value >= const_int32);
				case CTFIND_LT: return (int_value < const_int32);
				case CTFIND_LE: return (int_value <= const_int32);
				default: return false;
			}

		case INT8OID:
			const_int64 = DatumGetInt64(pred->const_value);

			if (ctdbGetFieldAsSigned(hRecord, pred->attnum - 1, &int_value) != CTDBRET_OK)
				return false;

			switch (pred->ctfind_mode)
			{
				case CTFIND_EQ: return (int_value == const_int64);
				case CTFIND_GT: return (int_value > const_int64);
				case CTFIND_GE: return (int_value >= const_int64);
				case CTFIND_LT: return (int_value < const_int64);
				case CTFIND_LE: return (int_value <= const_int64);
				default: return false;
			}

		case NUMERICOID:
			/* Use cached conversion to avoid repeated DirectFunctionCall1 overhead */
			if (!pred->cached_float8_valid)
			{
				float8 float_val = DatumGetFloat8(DirectFunctionCall1(numeric_float8, pred->const_value));
				pred->cached_float8_value = float_val;
				pred->cached_float8_valid = true;
			}
			const_float = pred->cached_float8_value;

			if (ctdbGetFieldAsFloat(hRecord, pred->attnum - 1, &float_value) != CTDBRET_OK)
				return false;

			switch (pred->ctfind_mode)
			{
				case CTFIND_EQ: return (float_value == const_float);
				case CTFIND_GT: return (float_value > const_float);
				case CTFIND_GE: return (float_value >= const_float);
				case CTFIND_LT: return (float_value < const_float);
				case CTFIND_LE: return (float_value <= const_float);
				default: return false;
			}

		case FLOAT4OID:
		case FLOAT8OID:
			const_float = DatumGetFloat8(pred->const_value);
			if (ctdbGetFieldAsFloat(hRecord, pred->attnum - 1, &float_value) != CTDBRET_OK)
				return false;

			switch (pred->ctfind_mode)
			{
				case CTFIND_EQ: return (float_value == const_float);
				case CTFIND_GT: return (float_value > const_float);
				case CTFIND_GE: return (float_value >= const_float);
				case CTFIND_LT: return (float_value < const_float);
				case CTFIND_LE: return (float_value <= const_float);
				default: return false;
			}

		case TEXTOID:
		case VARCHAROID:
		case BPCHAROID:
		{
			char *const_str = TextDatumGetCString(pred->const_value);
			char field_text[1024];  /* Buffer for field string */
			int cmp_result;
			bool result;

			/* Get field as string */
			if (ctdbGetFieldAsString(hRecord, pred->attnum - 1, field_text, sizeof(field_text)) != CTDBRET_OK)
			{
				pfree(const_str);
				return false;
			}

			/* Handle LIKE pattern matching */
			if (pred->ctfind_mode == CTFIND_LIKE)
			{
				result = simple_pattern_match(field_text, const_str);
				pfree(const_str);
				return result;
			}

			cmp_result = strcmp(field_text, const_str);
			pfree(const_str);

			/* String comparison operators */
			switch (pred->ctfind_mode)
			{
				case CTFIND_EQ: return (cmp_result == 0);
				case CTFIND_GT: return (cmp_result > 0);
				case CTFIND_GE: return (cmp_result >= 0);
				case CTFIND_LT: return (cmp_result < 0);
				case CTFIND_LE: return (cmp_result <= 0);
				default: return false;
			}
		}

		default:
			/* Unsupported type - assume match for safety */
			return true;
	}
}

/*
 * Execute optimized IN list query using multiple targeted index lookups
 * This function is called when faircom_apply_predicates() returns -1,
 * indicating an IN list pattern was detected.
 *
 * Instead of doing one index lookup + filtering, this does one index lookup
 * per IN list value, which is much faster for selective IN lists.
 *
 * Returns number of records fetched into the batch arrays.
 */
int
faircom_fetch_in_list_batch(CTHANDLE hTable,
							 CTHANDLE hRecord,
							 List *predicates,
							 TupleDesc tupdesc,
							 Datum **batch_values,
							 bool **batch_nulls,
							 int max_batch)
{
	ListCell *lc;
	int batch_count = 0;
	char index_name[256];
	NINT index_nbr;
	CTDBRET ret;
	FairComPredicate *first_pred = NULL;
	int field_num = -1;

	if (predicates == NIL || max_batch <= 0)
		return 0;

	first_pred = (FairComPredicate *) linitial(predicates);
	field_num = first_pred->attnum - 1;  /* Convert to 0-based */

	FAIRCOM_DEBUG_LOG(DEBUG1, "IN list optimization: Executing %d targeted index lookups for field %s",
		 list_length(predicates), first_pred->attname);

	/* Try to use index for the field */
	snprintf(index_name, sizeof(index_name), "idx_%s", first_pred->attname);
	index_nbr = ctdbGetIndexNbrByName(hTable, index_name);

	if (index_nbr < 0)
	{
		/* Try default index (0) */
		FAIRCOM_DEBUG_LOG(DEBUG1, "Index '%s' not found, trying default index", index_name);
		index_nbr = 0;  /* Use default/primary index */
	}

	if (index_nbr >= 0)
	{
		ret = ctdbSetDefaultIndex(hTable, index_nbr);
		if (ret != CTDBRET_OK)
			elog(WARNING, "Failed to set default index to %d", index_nbr);
		else
			FAIRCOM_DEBUG_LOG(DEBUG1, "Using index %d for IN list lookups", index_nbr);
	}

	/* Execute one index lookup for each IN list value */
	foreach(lc, predicates)
	{
		FairComPredicate *pred = (FairComPredicate *) lfirst(lc);

		if (batch_count >= max_batch)
		{
			FAIRCOM_DEBUG_LOG(DEBUG1, "IN list batch full (%d records), stopping", batch_count);
			break;
		}

		/* Set the field value for this lookup */
		switch (pred->const_type)
		{
			case INT2OID:
			case INT4OID:
				{
					int32 value = DatumGetInt32(pred->const_value);
					ctdbSetFieldAsSigned(hRecord, field_num, value);
					FAIRCOM_DEBUG_LOG(DEBUG2, "IN list lookup: %s = %d", pred->attname, value);
				}
				break;

			case INT8OID:
				{
					int64 value = DatumGetInt64(pred->const_value);
					ctdbSetFieldAsSigned(hRecord, field_num, (CTSIGNED)value);
					FAIRCOM_DEBUG_LOG(DEBUG2, "IN list lookup: %s = %lld", pred->attname, (long long)value);
				}
				break;

			case FLOAT4OID:
			case FLOAT8OID:
				{
					float8 value = DatumGetFloat8(pred->const_value);
					ctdbSetFieldAsFloat(hRecord, field_num, value);
					FAIRCOM_DEBUG_LOG(DEBUG2, "IN list lookup: %s = %f", pred->attname, value);
				}
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
					ctdbSetFieldAsFloat(hRecord, field_num, pred->cached_float8_value);
					FAIRCOM_DEBUG_LOG(DEBUG2, "IN list lookup: %s = %f (from NUMERIC)", pred->attname, pred->cached_float8_value);
				}
				break;

			default:
				FAIRCOM_DEBUG_LOG(DEBUG1, "Unsupported type %u for IN list lookup", pred->const_type);
				continue;
		}

		/* Execute the index lookup */
		ret = ctdbFindRecord(hRecord, CTFIND_EQ);

		if (ret == CTDBRET_OK)
		{
			/* Found a matching record - add to batch */
			FAIRCOM_DEBUG_LOG(DEBUG2, "  -> Found matching record");

			/* Extract values into batch arrays */
			for (int i = 0; i < tupdesc->natts; i++)
			{
				Form_pg_attribute attr = TupleDescAttr(tupdesc, i);

				if (attr->attisdropped)
				{
					batch_nulls[batch_count][i] = true;
					continue;
				}

				batch_values[batch_count][i] = faircom_to_datum(hRecord, i, attr->atttypid,
												   attr->atttypmod, &batch_nulls[batch_count][i]);
			}

			batch_count++;
		}
		else if (ret == INOT_ERR)
		{
			/* Not found - this is OK for IN lists (value not in table) */
			FAIRCOM_DEBUG_LOG(DEBUG2, "  -> No match (OK for IN list)");
		}
		else
		{
			/* Real error */
			elog(WARNING, "IN list lookup failed with error %d", ret);
			faircom_check_result(ret, hRecord, "IN list index lookup");
		}
	}

	FAIRCOM_DEBUG_LOG(DEBUG1, "IN list optimization: Retrieved %d records from %d lookups",
		 batch_count, list_length(predicates));

	return batch_count;
}
