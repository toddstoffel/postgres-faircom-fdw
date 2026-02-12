/*-------------------------------------------------------------------------
 *
 * query.c
 *		  Query execution helpers for FairCom FDW
 *
 * Helper functions for query execution.
 * Predicate pushdown logic is now in predicate.c
 *
 * Copyright (c) 2026, FairCom Corporation. All rights reserved.
 * Proprietary and confidential.
 *
 *-------------------------------------------------------------------------
 */
#include "postgres.h"
#include "faircom_fdw.h"

#include "optimizer/optimizer.h"
#include "utils/lsyscache.h"

/*
 * Convert PostgreSQL operator OID to FairCom find mode
 */
CTFIND_MODE
faircom_operator_to_find_mode(Oid opno)
{
	/* Get operator name */
	char *opname = get_opname(opno);

	if (opname == NULL)
		return CTFIND_EQ;

	if (strcmp(opname, "=") == 0)
		return CTFIND_EQ;
	else if (strcmp(opname, "<") == 0)
		return CTFIND_LT;
	else if (strcmp(opname, "<=") == 0)
		return CTFIND_LE;
	else if (strcmp(opname, ">") == 0)
		return CTFIND_GT;
	else if (strcmp(opname, ">=") == 0)
		return CTFIND_GE;
	else
		return CTFIND_EQ;  /* Default */
}

