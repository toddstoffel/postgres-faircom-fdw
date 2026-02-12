/*-------------------------------------------------------------------------
 *
 * connection.c
 *		  Connection management for FairCom FDW
 *
 * Maintains a pool of connections to FairCom servers and handles
 * transaction coordination.
 *
 * Copyright (c) 2026, FairCom Corporation
 *
 *-------------------------------------------------------------------------
 */
#include "postgres.h"
#include "faircom_fdw.h"

#include "access/xact.h"
#include "mb/pg_wchar.h"
#include "miscadmin.h"
#include "utils/hsearch.h"
#include "utils/memutils.h"
#include "utils/timestamp.h"
#include "foreign/foreign.h"
#include "commands/defrem.h"

/* Connection idle timeout in seconds (30 minutes for better connection reuse) */
#define FAIRCOM_CONN_IDLE_TIMEOUT 1800

/* Connection pool - per backend process */
static FairComConnection *connection_list = NULL;

/*
 * Get a connection to FairCom server for the given foreign server
 * and user mapping. Reuses existing connections when possible.
 */
FairComConnection *
faircom_get_connection(ForeignServer *server, UserMapping *user)
{
	FairComConnection *conn;
	FairComServerOptions server_opts;
	const char *username = NULL;
	const char *password = NULL;
	CTDBRET retval;

	/* Search for existing connection */
	for (conn = connection_list; conn != NULL; conn = conn->next)
	{
		if (strcmp(conn->servername, server->servername) == 0)
		{
			/*
			 * Lazy validation approach: Skip expensive validation checks
			 * Assume connection is valid until actual usage fails
			 * This eliminates per-reuse overhead and improves performance
			 * Connection errors during operations will be caught and handled
			 */
			if (!conn->is_valid)
				continue;  /* Skip connections already marked invalid */

			/* Found existing valid connection - increment reference count */
			conn->ref_count++;
			conn->reuse_count++;
			conn->idle_since = 0;  /* No longer idle */

			/* Track connection reuse */
			global_stats.connections_reused++;

			return conn;
		}
	}

	/* Parse server options */
	faircom_parse_server_options(server, &server_opts);

	/* Get user credentials from user mapping */
	if (user && user->options)
	{
		ListCell *lc;
		foreach(lc, user->options)
		{
			DefElem *def = (DefElem *) lfirst(lc);
			if (strcmp(def->defname, "username") == 0)
				username = defGetString(def);
			else if (strcmp(def->defname, "password") == 0)
				password = defGetString(def);
		}
	}

	if (!username)
		username = "ADMIN";  /* Default */
	if (!password)
		password = "";       /* Default */

	/* Allocate new connection structure */
	conn = (FairComConnection *) MemoryContextAlloc(TopMemoryContext,
													sizeof(FairComConnection));
	memset(conn, 0, sizeof(FairComConnection));

	/* Allocate FairCom session - use CTSESSION_CTREE like the tutorials */
	conn->hSession = ctdbAllocSession(CTSESSION_CTREE);
	if (!conn->hSession)
	{
		ereport(ERROR,
				(errcode(ERRCODE_FDW_OUT_OF_MEMORY),
				 errmsg("failed to allocate FairCom session")));
	}

	/* Connect to FairCom server */
	char server_address[256];

	/* Format: #PORT@HOST - The # prefix forces TCP/IP connection! */
	snprintf(server_address, sizeof(server_address), "#%d@%s",
			 server_opts.port, server_opts.host);

FAIRCOM_DEBUG_LOG(DEBUG1, "Connecting to FairCom server: %s (user: %s)",
		 server_address, username);

	retval = ctdbLogon(conn->hSession,
					   server_address,
					   (pTEXT)username,
					   (pTEXT)password);

	faircom_check_result(retval, conn->hSession, "ctdbLogon");

	/* For SQL databases, we don't use ctdbConnect - instead we'll set table paths */
	/* The database is just the session handle */
	conn->hDatabase = conn->hSession;

	/* Initialize connection metadata */
	conn->servername = MemoryContextStrdup(TopMemoryContext, server->servername);
	conn->username = MemoryContextStrdup(TopMemoryContext, username);
	conn->in_transaction = false;
	conn->owner = CurrentResourceOwner;
	conn->ref_count = 0;			/* No active scans yet */
	conn->reuse_count = 0;		/* First time use */
	conn->idle_since = 0;		/* Not idle yet */
	conn->is_valid = true;		/* Newly created connection is valid */

	/* Add to connection list */
	conn->next = connection_list;
	connection_list = conn;

	/* Increment reference count for new connection */
	conn->ref_count++;

	/* Track new connection creation */
	global_stats.connections_created++;

	FAIRCOM_DEBUG_LOG(DEBUG1, "established new FairCom connection to %s (ref_count=%d)",
		 server_opts.host, conn->ref_count);

	return conn;
}

/*
 * Release a FairCom connection
 */
void
faircom_release_connection(FairComConnection *conn)
{
	FairComConnection **prev;

	if (!conn)
		return;

	/* Abort any active transaction */
	if (conn->in_transaction)
	{
		ctdbAbort(conn->hSession);
		conn->in_transaction = false;
	}

	/* Remove from connection list */
	prev = &connection_list;
	while (*prev)
	{
		if (*prev == conn)
		{
			*prev = conn->next;
			break;
		}
		prev = &(*prev)->next;
	}

	/* Cleanup FairCom session */
	if (conn->hDatabase && conn->hDatabase != conn->hSession)
	{
		ctdbDisconnect(conn->hDatabase);
		ctdbFreeDatabase(conn->hDatabase);
	}

	if (conn->hSession)
	{
		ctdbLogout(conn->hSession);
		ctdbFreeSession(conn->hSession);
	}

	/* Free connection structure */
	if (conn->servername)
		pfree(conn->servername);
	if (conn->username)
		pfree(conn->username);
	pfree(conn);

	FAIRCOM_DEBUG_LOG(DEBUG1, "released FairCom connection");
}

/*
 * Clean up idle connections that have exceeded timeout
 * Instead of immediate cleanup, connections are kept alive for reuse
 */
void
faircom_cleanup_idle_connections(void)
{
	FairComConnection **prev = &connection_list;
	FairComConnection *conn;
	TimestampTz now = GetCurrentTimestamp();
	long idle_secs;

	while ((conn = *prev) != NULL)
	{
		if (conn->ref_count == 0)
		{
			/* Mark as idle if not already marked */
			if (conn->idle_since == 0)
			{
				conn->idle_since = now;
			}

			/* Check if connection has been idle too long or is invalid */
			idle_secs = (long) timestamptz_to_time_t(now) -
						(long) timestamptz_to_time_t(conn->idle_since);

			if (!conn->is_valid || idle_secs > FAIRCOM_CONN_IDLE_TIMEOUT)
			{
				/* Remove from list and free this stale connection */
				*prev = conn->next;
				FAIRCOM_DEBUG_LOG(DEBUG1, "cleaning up %s FairCom connection after %ld seconds idle (reuse_count=%d)",
					 conn->is_valid ? "expired" : "invalid", idle_secs, conn->reuse_count);
				faircom_force_release_connection(conn);
			}
			else
			{
				/* Keep idle connection alive for reuse */
				prev = &conn->next;
			}
		}
		else
		{
			/* Connection is active, move to next */
			prev = &conn->next;
		}
	}
}

/*
 * Force release a connection regardless of reference count
 */
void
faircom_force_release_connection(FairComConnection *conn)
{
	if (!conn)
		return;

	/* Abort any active transaction */
	if (conn->in_transaction)
	{
		ctdbAbort(conn->hSession);
		conn->in_transaction = false;
	}

	/* Cleanup FairCom session */
	if (conn->hDatabase && conn->hDatabase != conn->hSession)
	{
		ctdbDisconnect(conn->hDatabase);
		ctdbFreeDatabase(conn->hDatabase);
	}

	if (conn->hSession)
	{
		ctdbLogout(conn->hSession);
		ctdbFreeSession(conn->hSession);
	}

	/* Free connection structure */
	if (conn->servername)
		pfree(conn->servername);
	if (conn->username)
		pfree(conn->username);
	pfree(conn);

	FAIRCOM_DEBUG_LOG(DEBUG1, "force released FairCom connection");
}

/*
 * Release all connections (called on backend exit)
 */
void
faircom_cleanup_connections(void)
{
	while (connection_list != NULL)
	{
		FairComConnection *conn = connection_list;
		connection_list = conn->next;
		faircom_release_connection(conn);
	}
}

/*
 * Transaction callback for coordinating PostgreSQL and FairCom transactions
 */
void
faircom_xact_callback(XactEvent event, void *arg)
{
	FairComConnection *conn;

	switch (event)
	{
		case XACT_EVENT_PRE_COMMIT:
			/* Commit all FairCom transactions */
			for (conn = connection_list; conn != NULL; conn = conn->next)
			{
				if (conn->in_transaction)
				{
					CTDBRET retval = ctdbCommit(conn->hSession);
					faircom_check_result(retval, conn->hSession, "ctdbCommit");
					conn->in_transaction = false;
				}
			}
			/* Clean up idle connections after transaction */
			faircom_cleanup_idle_connections();
			break;

		case XACT_EVENT_ABORT:
		case XACT_EVENT_PARALLEL_ABORT:
			/* Rollback all FairCom transactions */
			for (conn = connection_list; conn != NULL; conn = conn->next)
			{
				if (conn->in_transaction)
				{
					ctdbAbort(conn->hSession);
					conn->in_transaction = false;
					FAIRCOM_DEBUG_LOG(DEBUG2, "aborted FairCom transaction");
				}
			}
			/* Clean up idle connections after abort */
			faircom_cleanup_idle_connections();
			break;

		case XACT_EVENT_PREPARE:
			/* TODO: Implement two-phase commit for write operations */
			ereport(ERROR,
					(errcode(ERRCODE_FEATURE_NOT_SUPPORTED),
					 errmsg("prepared transactions are not supported with FairCom FDW")));
			break;

		default:
			/* Other events not relevant */
			break;
	}
}

/*
 * Begin a FairCom transaction if not already in one
 */
void
faircom_begin_transaction(FairComConnection *conn)
{
	CTDBRET retval;

	if (conn->in_transaction)
		return;

	if (!IsTransactionState())
		return;

	retval = ctdbBegin(conn->hSession);
	faircom_check_result(retval, conn->hSession, "ctdbBegin");

	conn->in_transaction = true;
	FAIRCOM_DEBUG_LOG(DEBUG2, "began FairCom transaction");
}
