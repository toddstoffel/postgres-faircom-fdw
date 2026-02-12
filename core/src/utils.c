/*-------------------------------------------------------------------------
 *
 * utils.c
 *		  Utility functions for FairCom FDW
 *
 * Error handling, option parsing, and other helper functions.
 *
 * Copyright (c) 2026, FairCom Corporation
 *
 *-------------------------------------------------------------------------
 */
#include "postgres.h"
#include "faircom_fdw.h"

#include "catalog/pg_foreign_server.h"
#include "catalog/pg_foreign_table.h"
#include "catalog/pg_user_mapping.h"
#include "commands/defrem.h"
#include "utils/builtins.h"
#include "miscadmin.h"

/*
 * Check FairCom API result and report errors
 */
void
faircom_check_result(CTDBRET retval, CTHANDLE handle, const char *operation)
{
	if (retval == CTDBRET_OK)
		return;

	int sqlstate;
	const char *fc_errmsg = faircom_get_error_message(handle);

	/* Map FairCom error codes to PostgreSQL SQLSTATE */
	switch (retval)
	{
		case CTDBRET_NOTOPEN:
			sqlstate = ERRCODE_FDW_TABLE_NOT_FOUND;
			break;

		case CTDBRET_NULARG:
		case CTDBRET_INVARG:
			sqlstate = ERRCODE_FDW_INVALID_HANDLE;
			break;

		case CTDBRET_NOMEMORY:
			sqlstate = ERRCODE_FDW_OUT_OF_MEMORY;
			break;

		default:
			sqlstate = ERRCODE_FDW_ERROR;
			break;
	}

	ereport(ERROR,
			(errcode(sqlstate),
			 errmsg("%s failed: %s", operation, fc_errmsg),
			 errdetail("FairCom error code: %d", retval)));
}

/*
 * Get error message from FairCom handle
 */
const char *
faircom_get_error_message(CTHANDLE handle)
{
	static char errbuf[1024];

	if (handle)
	{
		CTDBRET ret = ctdbGetError(handle);
		snprintf(errbuf, sizeof(errbuf), "Error %d", ret);
		return errbuf;
	}

	return "Unknown error";
}

/*
 * Parse foreign server options
 */
void
faircom_parse_server_options(ForeignServer *server,
							  FairComServerOptions *opts)
{
	ListCell *lc;

	/* Set defaults */
	opts->host = "localhost";
	opts->port = 6597;
	opts->database = NULL;
	opts->connect_timeout = FAIRCOM_DEFAULT_TIMEOUT;

	/* Parse options */
	foreach(lc, server->options)
	{
		DefElem *def = (DefElem *) lfirst(lc);

		if (strcmp(def->defname, "host") == 0)
			opts->host = defGetString(def);
		else if (strcmp(def->defname, "port") == 0)
			opts->port = atoi(defGetString(def));
		else if (strcmp(def->defname, "database") == 0)
			opts->database = defGetString(def);
		else if (strcmp(def->defname, "connect_timeout") == 0)
			opts->connect_timeout = atoi(defGetString(def));
	}
}

/*
 * Parse foreign table options
 */
void
faircom_parse_table_options(ForeignTable *table,
							FairComTableOptions *opts)
{
	ListCell *lc;

	/* Set defaults */
	opts->table_name = NULL;
	opts->use_index = NULL;
	opts->enable_pushdown = true;

	/* Parse options */
	foreach(lc, table->options)
	{
		DefElem *def = (DefElem *) lfirst(lc);

		if (strcmp(def->defname, "faircom_table") == 0)
			opts->table_name = defGetString(def);
		else if (strcmp(def->defname, "use_index") == 0)
			opts->use_index = defGetString(def);
		else if (strcmp(def->defname, "enable_pushdown") == 0)
			opts->enable_pushdown = defGetBoolean(def);
	}

	/* Validate required options */
	if (!opts->table_name)
	{
		ereport(ERROR,
				(errcode(ERRCODE_FDW_OPTION_NAME_NOT_FOUND),
				 errmsg("faircom_table option is required for foreign tables")));
	}
}

/*
 * Get option value from a list
 */
char *
faircom_get_option_value(Oid foreignoid, const char *optname)
{
	ForeignTable *table;
	ForeignServer *server;
	UserMapping *user;
	ListCell *lc;

	/* Try table options first */
	table = GetForeignTable(foreignoid);
	foreach(lc, table->options)
	{
		DefElem *def = (DefElem *) lfirst(lc);
		if (strcmp(def->defname, optname) == 0)
			return defGetString(def);
	}

	/* Try server options */
	server = GetForeignServer(table->serverid);
	foreach(lc, server->options)
	{
		DefElem *def = (DefElem *) lfirst(lc);
		if (strcmp(def->defname, optname) == 0)
			return defGetString(def);
	}

	/* Try user mapping options */
	user = GetUserMapping(GetUserId(), table->serverid);
	foreach(lc, user->options)
	{
		DefElem *def = (DefElem *) lfirst(lc);
		if (strcmp(def->defname, optname) == 0)
			return defGetString(def);
	}

	return NULL;
}
