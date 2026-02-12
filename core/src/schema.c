/*-------------------------------------------------------------------------
 *
 * schema.c
 *		  Schema discovery and type mapping for FairCom FDW
 *
 * Implements IMPORT FOREIGN SCHEMA and data type conversions between
 * FairCom and PostgreSQL.
 *
 * Copyright (c) 2026, FairCom Corporation. All rights reserved.
 * Proprietary and confidential.
 *
 *-------------------------------------------------------------------------
 */
#include "postgres.h"
#include "faircom_fdw.h"

#include "access/htup_details.h"
#include "catalog/pg_type.h"
#include "commands/defrem.h"
#include "miscadmin.h"
#include "utils/builtins.h"
#include "utils/lsyscache.h"

/*
 * Import foreign schema from FairCom database
 * Generates CREATE FOREIGN TABLE statements for tables in the remote schema
 */
List *
faircom_import_foreign_schema(ImportForeignSchemaStmt *stmt, Oid serverOid)
{
	ForeignServer *server;
	UserMapping *user;
	FairComConnection *conn;
	List *commands = NIL;
	StringInfoData buf;

	FAIRCOM_DEBUG_LOG(DEBUG1, "ImportForeignSchema: Importing from remote schema '%s'",
		 stmt->remote_schema);

	/* Get server and user mapping */
	server = GetForeignServer(serverOid);
	user = GetUserMapping(GetUserId(), serverOid);

	/* Get connection */
	conn = faircom_get_connection(server, user);
	if (!conn || !conn->hSession)
	{
		ereport(ERROR,
				(errcode(ERRCODE_FDW_UNABLE_TO_ESTABLISH_CONNECTION),
				 errmsg("could not connect to FairCom server")));
	}

	/* For now, generate a template foreign table statement
	 * User needs to specify table_name option manually
	 *
	 * Full implementation would use ctdbFindTable/ctdbFirstTable/ctdbNextTable
	 * to enumerate tables in the database and generate CREATE statements
	 */

	initStringInfo(&buf);
	appendStringInfo(&buf,
		"-- IMPORT FOREIGN SCHEMA partially implemented\n"
		"-- Use this template to create foreign tables:\n\n"
		"-- CREATE FOREIGN TABLE %s.example_table (\n"
		"--     id INTEGER,\n"
		"--     name TEXT,\n"
		"--     value NUMERIC\n"
		"-- ) SERVER %s\n"
		"-- OPTIONS (table_name '%s.admin.example_table');",
		stmt->local_schema,
		server->servername,
		stmt->remote_schema);

	commands = lappend(commands, pstrdup(buf.data));

	elog(NOTICE, "ImportForeignSchema: Generated template - modify table_name option for your tables");

	return commands;
}

/*
 * Map FairCom CTDB type to PostgreSQL type OID
 */
Oid
faircom_type_to_pg_type(CTDBTYPE ctdbtype)
{
	switch (ctdbtype)
	{
		case CT_BOOL:
			return BOOLOID;

		case CT_INT2:
			return INT2OID;

		case CT_INT4:
			return INT4OID;

		case CT_INT8:
			return INT8OID;

		case CT_SFLOAT:
			return FLOAT4OID;

		case CT_DFLOAT:
			return FLOAT8OID;

		case CT_CHAR:
		case CT_CHARU:
			return CHAROID;

		case CT_STRING:
		case CT_FSTRING:
		case CT_F2STRING:
		case CT_F4STRING:
		case CT_VARBINARY:
			return TEXTOID;

		case CT_DATE:
		case CT_TIME:
		case CT_TIMES:
			return TIMESTAMPOID;

		case CT_NUMBER:
		case CT_CURRENCY:
		case CT_MONEY:
			return NUMERICOID;

		case CT_LVB:
			return BYTEAOID;

		default:
			ereport(ERROR,
					(errcode(ERRCODE_FDW_INVALID_DATA_TYPE),
					 errmsg("unsupported FairCom data type: %d", ctdbtype)));
			return InvalidOid;
	}
}

/*
 * Get PostgreSQL type name for a FairCom type
 */
const char *
faircom_type_to_pg_typename(CTDBTYPE ctdbtype)
{
	switch (ctdbtype)
	{
		case CT_BOOL:
			return "boolean";
		case CT_INT2:
			return "smallint";
		case CT_INT4:
			return "integer";
		case CT_INT8:
			return "bigint";
		case CT_SFLOAT:
			return "real";
		case CT_DFLOAT:
			return "double precision";
		case CT_CHAR:
		case CT_CHARU:
			return "char";
		case CT_STRING:
		case CT_FSTRING:
		case CT_F2STRING:
		case CT_F4STRING:
			return "text";
		case CT_DATE:
		case CT_TIME:
		case CT_TIMES:
			return "timestamp";
		case CT_NUMBER:
		case CT_CURRENCY:
		case CT_MONEY:
			return "numeric";
		case CT_VARBINARY:
		case CT_LVB:
			return "bytea";
		default:
			return "text";  /* Fallback */
	}
}
