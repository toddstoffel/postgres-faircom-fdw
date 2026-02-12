/*
 *      OFFICIAL NOTIFICATION: the following CONFIDENTIAL and PROPRIETARY
 * 	property legend shall not be removed from this source code module
 * 	for any reason.
 *
 *	This program is the CONFIDENTIAL and PROPRIETARY property
 *	of FairCom(R) Corporation. Any unauthorized use, reproduction or
 *	transfer of this computer program is strictly prohibited. The
 *      contents of this file may not be disclosed to third parties, copied or
 *      duplicated in any form, in whole or in part, without the prior written
 *      permission of the FairCom(R) Corporation.
 *
 *      Copyright (c) 1992 - 2024 FairCom Corporation.
 *	This is an unpublished work, and is subject to limited distribution and
 *	restricted disclosure only. ALL RIGHTS RESERVED.
 *
 *			RESTRICTED RIGHTS LEGEND
 *	Use, duplication, or disclosure by the Government is subject to
 *	restrictions set forth in subparagraph (c)(1)(ii) of the Rights in
 * 	Technical Data and Computer Software clause at DFARS 252.227-7013, and/or
 *      in similar or successor clauses in the FAR, DOD or NASA FAR Supplement.
 *      Unpublished rights reserved under the Copyright Laws of the United States.
 *	FairCom Corporation, 6300 West Sugar Creek Drive, Columbia, MO 65203.
 *
 *	c-treeACE(tm)	Version 13
 */

#ifndef ctREPLAGENTH
#define ctREPLAGENTH

typedef enum replicationAgentEvent_t {
	ctRAstart  =  1,/* Replication agent is starting		      */
	ctRAstop   =  2,/* Replication agent is shutting down		      */
	ctRApause  =  3,/* Replication agent is entering paused state	      */
	ctRAresume =  4,/* Replication agent is resuming operation	      */
	ctRAsconn  =  5,/* Replication agent connected to source server       */
	ctRAtconn  =  6,/* Replication agent connected to target server	      */
	ctRAslcon  =  7,/* Replication agent lost connection to source server */
	ctRAtlcon  =  8,/* Replication agent lost connection to target server */
	ctRAsdisc  =  9,/* Replication agent disconnected from source server  */
	ctRAtdisc  = 10,/* Replication agent disconnected from target server  */
	ctRAexcept = 11,/* Replication agent terminating due to exception     */
	ctRAsynstp = 12, /* Replication agent sync shutdown occurred	      */
	ctRAstopped = 13 /* Replication agent has shut down		      */
} REPEVT;

/* Replication agent supported configuration options */
enum {
	ctRAOPT_target_authfile		= 0,
	ctRAOPT_target_server		= 1,
	ctRAOPT_source_authfile		= 2,
	ctRAOPT_source_server		= 3,
	ctRAOPT_batch_size		= 4,
	ctRAOPT_read_timeout_ms		= 5,
	ctRAOPT_log_change_details	= 6,
	ctRAOPT_exception_mode		= 7,
	ctRAOPT_remember_log_pos	= 8,
	ctRAOPT_unique_id		= 9,
	ctRAOPT_lock_retry_count	= 10,
	ctRAOPT_lock_retry_sleep	= 11,
	ctRAOPT_system_event_log	= 12,
	ctRAOPT_redirect		= 13,
	ctRAOPT_dbgminlog		= 14,
	ctRAOPT_usetgtalign		= 15,
	ctRAOPT_start_position		= 16,
	ctRAOPT_full_filenames		= 17,
	ctRAOPT_notify_events		= 18,
	ctRAOPT_socket_timeout		= 19,
	ctRAOPT_file_filter		= 20,
	ctRAOPT_source_nodeid		= 21,
	ctRAOPT_target_nodeid		= 22,
	ctRAOPT_function_timing		= 23,
	ctRAOPT_exception_logging	= 24,
	ctRAOPT_data_path		= 25,
	ctRAOPT_check_update		= 26,
	ctRAOPT_extension_library	= 27,
	ctRAOPT_check_schema_id		= 28,
	ctRAOPT_source_use_tls		= 29,
	ctRAOPT_target_use_tls		= 30,
	ctRAOPT_source_tls_cert_file	= 31,
	ctRAOPT_target_tls_cert_file	= 32,
	ctRAOPT_convert_subst		= 33,
	ctRAOPT_skipops			= 34,
	ctRAOPT_syncagent		= 35,
	ctRAOPT_parallel_apply		= 36,
	ctRAOPT_check_dependencies	= 37,
	ctRAOPT_coordinate_commit	= 38,
	ctRAOPT_num_analyzer_threads	= 39,
	ctRAOPT_num_apply_threads	= 40,
	ctRAOPT_sync_log_writes		= 41,
	ctRAOPT_log_file_name		= 42,
	ctRAOPT_replicate_data_definitions = 43,
	ctRAOPT_diagnostics		= 44,
	ctRAOPT_cfg_file_name		= 45,
	ctRAOPT_delete_to_recycle_bin	= 46,
	ctRAOPT_logship_unique_id	= 47,
	ctRAOPT_file_filter_target	= 48,
	ctRAOPT_file_group_parallel_apply = 49,
	ctRAOPT_resync_file_list	= 50,
	ctRAOPT_LAST			= 51
};

/* types of replication agent threads */
typedef enum agentthreadtype_t {
	REPLAGENT_MAIN_THREAD		= 0, /* main replication agent thread */
	REPLAGENT_LOG_SHIP_THREAD	= 1, /* transaction log ship thread */
	REPLAGENT_LOG_READ_THREAD	= 2, /* transaction log read thread */
	REPLAGENT_DEPENDENCY_THREAD	= 3, /* transaction dependency check thread */
	REPLAGENT_ANALYZER_THREAD	= 4, /* analyzer threads (one or more) */
	REPLAGENT_APPLY_THREAD		= 5  /* transaction apply thread (one or more) */
} AGENT_THREAD_TYPE;

enum {
	REPLENG_OP_READ  = 0,
	REPLENG_OP_TRACK = 1,
	REPLENG_OP_APPLY = 2
};

/* number of hash bins for a writeset list */
#define NUMBER_OF_WRITESET_HASH_BINS 10

/* transaction writeset hash bin list entry */
typedef struct writesetBinEntry_t {
	struct writesetBinEntry_t *flnk; /* forward link */
	struct writesetBinEntry_t *blnk; /* backward link */
	pVOID hbuf; /* data element */
} WRITESET_BIN_ENTRY, *pWRITESET_BIN_ENTRY, **ppWRITESET_BIN_ENTRY;

struct writesetSearchRequest_t;

/* writeset search function pointer */
typedef NINT (*pWRITESET_SEARCH_FUNC)(struct writesetSearchRequest_t *pWritesetSearchRequest,pVOID pWritesetEntry);

/* type of writeset search */
typedef enum writesetSearchType_t {
	WRITESET_SEARCH_TYPE_NON_SUPERFILE = 0, /* search for non-superfile */
	WRITESET_SEARCH_TYPE_SUPERFILE_HOST = 1, /* search for superfile host */
	WRITESET_SEARCH_TYPE_SUPERFILE_MEMBER = 2 /* search for superfile member */
} WRITESET_SEARCH_TYPE;

/* a writeset search specification */
typedef struct writesetSearchRequest_t {
	ULONG8	hashValue; /* the hash value */
	UINT	indexId; /* the index identifier */
	pWRITESET_SEARCH_FUNC searchFunction; /* the function used to check entries for a match */
	UINT	keyLength; /* the length of the key value in bytes */
	WRITESET_SEARCH_TYPE searchType; /* type of file for the search */
	pTEXT	keyValue; /* the key value */
} WRITESET_SEARCH_REQUEST, *pWRITESET_SEARCH_REQUEST;

/* writeset types */
typedef enum writesetType_t {
	TABLE_WRITESET	= 1,	/* writeset of tables */
	INDEX_WRITESET	= 2,	/* a table's index writeset */
	RESOURCE_WRITESET = 3	/* a table's resource writeset */
} WRITESET_TYPE;

/* list used to store a transaction's writeset */
typedef struct writesetList_t {
	ppWRITESET_BIN_ENTRY hashBins; /* pointers to hash bins */
	NINT	numberOfBins; /* number of hash bins */
	NINT	numberOfEntries; /* number of entries in hash table */
	WRITESET_TYPE type; /* the type of writeset */
	TEXT	requiresExclusiveAccess; /* non-zero if the writeset contains an operation that requires exclusive access on the table */
} WRITESET_LIST, *pWRITESET_LIST;

/* attributes for table writeset entry */
#define TABLE_WRITESET_ENTRY_SUPERFILE_HOST	0x00000001 /* file is a superfile host */
#define TABLE_WRITESET_ENTRY_SUPERFILE_MEMBER	0x00000002 /* file is a superfile member */
#define TABLE_WRITESET_ENTRY_HOST_OP		0x00000004 /* host operation exists */
#define TABLE_WRITESET_ENTRY_MEMBER_OP		0x00000008 /* member operation exists */

/* an entry in a transaction's table writeset */
typedef struct tableWritesetEntry_t {
	ULONG8	hashValue; /* the hash value, calculated from the table name */
	ULONG8	superfileHostHashValue; /* the hash value, calculated from the superfile host name */
	WRITESET_BIN_ENTRY writesetBin; /* writeset bin links */
	WRITESET_LIST indexWriteset; /* index writeset for this table */
	WRITESET_LIST resourceWriteset; /* resource writeset for this table */
	ULONG	tableNameLength; /* length of table name in bytes */
	ULONG	superfileHostNameLength; /* length of superfile host name in bytes */
	pTEXT	superfileHostName; /* the superfile host name (if superfile member) */
	ULONG	tableAttributes; /* attributes for table */
	TEXT 	tableName[1]; /* the table name */
} TABLE_WRITESET_ENTRY, *pTABLE_WRITESET_ENTRY, **ppTABLE_WRITESET_ENTRY;

/* an entry in a transaction's index writeset */
typedef struct indexWritesetEntry_t {
	ULONG8	hashValue; /* the hash value, calculated from the key value */
	WRITESET_BIN_ENTRY writesetBin; /* writeset bin links */
	ULONG	indexId; /* index identifier (relative index number) */
	ULONG	keyValueLength; /* length of key value in bytes */
	TEXT 	keyValue[1]; /* the key value */
} INDEX_WRITESET_ENTRY, *pINDEX_WRITESET_ENTRY, **ppINDEX_WRITESET_ENTRY;

/* an entry in a transaction's resource writeset */
typedef struct resourceWritesetEntry_t {
	ULONG8	hashValue; /* the hash value, calculated from the key value */
	WRITESET_BIN_ENTRY writesetBin; /* writeset bin links */
	TEXT	keyValue[8]; /* the key value: 4-byte resource type followed by 4-byte resource number */
} RESOURCE_WRITESET_ENTRY, *pRESOURCE_WRITESET_ENTRY, **ppRESOURCE_WRITESET_ENTRY;

/* transaction dependency graph entry */
typedef struct dependencyEntry_t {
	ULONG8 tranSequenceNumber; /* transaction sequence number */
	struct ctoplhdr *pOperationList; /* operation list for the transaction */
	NINT dependencyCount; /* number of dependencies */
	struct followerListEntry_t *pFollowerList; /* list of followers */
	struct dependencyEntry_t *prev; /* pointer to previous entry in graph */
	struct dependencyEntry_t *next; /* pointer to next entry in graph */
} DEPENDENCY_GRAPH_ENTRY, *pDEPENDENCY_GRAPH_ENTRY, **ppDEPENDENCY_GRAPH_ENTRY;

/* an entry in the follower list */
typedef struct followerListEntry_t {
	struct followerListEntry_t *next; /* next follower in list */
	pDEPENDENCY_GRAPH_ENTRY pFollower; /* pointer to follower */
} FOLLOWER_LIST_ENTRY, *pFOLLOWER_LIST_ENTRY;

/* the dependency graph */
typedef struct dependencyGraph_t {
	pDEPENDENCY_GRAPH_ENTRY head; /* first (least recently added) entry in dependency graph */
	pDEPENDENCY_GRAPH_ENTRY tail; /* last (most recently added) entry in dependency graph */
#ifdef ctThrds
	ctMUTEX lock; /* mutex used to serialize access to graph */
#endif
	volatile ULONG numberOfEntries; /* number of entries in graph */
} DEPENDENCY_GRAPH, *pDEPENDENCY_GRAPH;

#if defined(ctREPLAGENTcore) && defined(ctBEHAV_REPIMGRWTCMB)
/* Combined operation list entry. */
typedef struct cmbop {
	struct cmbop *prev; /* previous entry in cmbop list	*/
	struct cmbop *next; /* next entry in cmbop list		*/
	pctCHGB	pcurop;	    /* change buffer for this operation */
} CMBOP, *pCMBOP, **ppCMBOP;
#endif

/* an entry on the file close list */
typedef struct fileCloseListEntry_t {
	struct fileCloseListEntry_t *next; /* pointer to next entry in list */
	ULONG	fileid; /* file id to close */
} FILE_CLOSE_LIST_ENTRY, *pFILE_CLOSE_LIST_ENTRY;

/* the file close list */
typedef struct fileCloseList_t {
	pFILE_CLOSE_LIST_ENTRY head; /* pointer to first entry in list */
	pFILE_CLOSE_LIST_ENTRY tail; /* pointer to last entry in list */
} FILE_CLOSE_LIST;

/* an entry on the list of files that must be closed before this transaction is applied */
typedef struct closeFilesBeforeTranListEntry_t {
	TEXT	fileName[MAX_NAME]; /* the name of the file to close */
	ULONG	fileid; /* file id */
	RBHL	hashBin; /* hash bin links */
} CLOSE_FILES_BEFORE_TRAN_LIST_ENTRY, *pCLOSE_FILES_BEFORE_TRAN_LIST_ENTRY;

/* a list of files that must be closed before this transaction is applied */
typedef struct closeFilesBeforeTranList_t {
	ppRBHL	hashBins; /* array of hash bin pointers */
	ULONG 	numberOfFiles; /* number of file names in the list */
	ULONG 	pendingCloseCount; /* number of apply threads that haven't yet processed the file close request */
	UINT	numberOfHashBins; /* size of hash bin array */
} CLOSE_FILES_BEFORE_TRAN_LIST, *pCLOSE_FILES_BEFORE_TRAN_LIST;

/* the current search state for the list of files that must be closed before this transaction is applied */
typedef struct closeFilesBeforeTranListSearch_t {
	pCLOSE_FILES_BEFORE_TRAN_LIST_ENTRY entry; /* the current entry */
	UINT	hashBin; /* current hash bin */
} CLOSE_FILES_BEFORE_TRAN_LIST_SEARCH, *pCLOSE_FILES_BEFORE_TRAN_LIST_SEARCH;

/* a list of superfile hosts open by the caller */
typedef struct superfileHostList_t {
	ULONG 	numberOfFiles; /* number of file names in the list */
	UINT	numberOfHashBins; /* size of hash bin array */
	ppRBHL	nameHashBins; /* array of hash bin pointers for search by file name */
	ppRBHL	filnoHashBins; /* array of hash bin pointers for search by file number */
} SUPERFILE_HOST_LIST, *pSUPERFILE_HOST_LIST;

/* Tran operation list header status values. */
typedef enum ctoplhdrStatus {
	OPLH_STATUS_TRAN_BEGIN = 0, /* operation list entry for transaction whose begin we have read */
	OPLH_STATUS_TRAN_COMMIT = 1, /* operation list entry for transaction whose commit we have read */
	OPLH_STATUS_FILE_CLOSE = 2 /* operation list entry for file close operation */
} ctOPLH_STATUS;

/* Tran operation list header. */
typedef struct ctoplhdr {
	LONG8	tranno;		/* Transaction number		*/
	LONG8	tstampBegin;	/* Transaction start time	*/
	LONG8	tstampEnd;	/* Transaction end time		*/
	pctCHGB	frsop;		/* First operation in list	*/
	pctCHGB	lstop;		/* Last operation in list	*/
#if defined(ctREPLAGENTcore) && defined(ctBEHAV_REPIMGRWTCMB)
	pCMBOP	pcmbop;		/* List of combined operations	*/
#endif
#ifdef ctFeatREPLAGENTparallel
	ppRBHL	filbhl;		/* Pointer to file list hash bins	*/
	ppRBHL	fnmbhl;		/* Pointer to filename list hash bins	*/
	pDEPENDENCY_GRAPH_ENTRY dependencyGraphEntry; /* Pointer to this transaction's entry in the dependency graph */
#endif
#ifdef ctBEHAV_REPLPOS
	ctLOGP	begpos;		/* Log position of tran start	*/
	ctLOGP	sucpos;		/* Log position of SUCTRAN	*/
#endif
#ifdef ctFeatREPLENG_LOCAL_LIST
	ctLOGP	logpos;		/* Log position			*/
	RBHL	bhlop;		/* Op list hash pointers	*/
	RBHL	bhltrn;		/* Tran list hash pointers	*/
	struct ctoplhdr
		*trnnxt;	/* Next tran in commit order	*/
#endif
	FILE_CLOSE_LIST fileCloseList;	/* list of files to close when finished with this transaction */
	CLOSE_FILES_BEFORE_TRAN_LIST filesToCloseBeforeTran; /* list of files to close before applying this transaction */
	NINT	numops;		/* Number of list entries	*/
#ifdef ctFeatREPLAGENT_FILE_GROUP_FILTER
	ULONG	fileGroupId;	/* File group id for transaction */
#endif
#ifdef ctFeatREPLICAT_TDELTA
	ULONG	tdelta;		/* Time difference between commit time and
				source server's current time.	*/
#endif
	LONG	minlog;		/* Required minimum log number for this transaction */
#ifdef ctFeatREPLAGENTparallel
	NINT	filhbins;	/* Number of hash bins for file list	*/
	NINT	fnmhbins;	/* Number of hash bins for filename list*/
	WRITESET_LIST tableWriteset; /* Table level write set entries for the transaction */
	TEXT	transactionApplied; /* set to non-zero when transaction has been applied */
#endif
	TEXT	options;	/* Option bits for this tran	*/
	TEXT	status;		/* Transaction status		*/
} ctOPLH, *pctOPLH, **ppctOPLH;

/* ctOPLH options field bits: */
#define OPLHOPT_HASTDELTA	0x00000001	/* timestamp is delta */
#define OPLHOPT_SKIPTRAN	0x00000002	/* skip this transaction */
#define OPLHOPT_SYNC_COMMIT	0x00000004	/* sync commit read for this transaction */
/* NOTE: If we use 0x100 or greater, increase the size of the ctOPLH options field. */

/* non-zero if x's log position precedes y's log position */
#define PRECEDING_LOG_POSITION(x,y) ((x).lognum < (y).lognum || ((x).lognum == (y).lognum && (x).logpos < (y).logpos))

/* non-zero if x's log position precedes or is equal to y's log position */
#define PRECEDING_LOG_POSITION_INCLUSIVE(x,y) ((x).lognum < (y).lognum || ((x).lognum == (y).lognum && (x).logpos <= (y).logpos))

/* function prototype for replication exception callback function */
typedef ctCONVd NINT (*pEXCEPCBFNC)(NINT errcod, pctOPLH poplhed, pctCHGB pchgbuf, praSTAT pras, pctRASTT prepstt, pVOID context);

/* replication agent apply close file list entry */
typedef struct applyCloseFileListEntry_t {
	struct applyCloseFileListEntry_t *next; /* next entry in list */
	ULONG	fileid; /* file id */
	ctLOGP	logPosition; /* transaction log position for the operation */
} APPLY_CLOSE_FILE_LIST_ENTRY, *pAPPLY_CLOSE_FILE_LIST_ENTRY;

/* replication agent transaction state list entry state */
typedef enum tranState_t {
	TRAN_STATE_ANALYZE	= 1, /* in analysis phase */
	TRAN_STATE_DEPEND	= 2, /* in dependency check phase */
	TRAN_STATE_APPLY	= 3  /* in apply phase */
} TRAN_STATE;

/* replication agent transaction state list entry */
typedef struct tranStateListEntry_t {
	struct tranStateListEntry_t *next; /* next entry in list */
	ULONG8	transactionNumber; /* transaction number */
	LONG8	endTimestamp; /* timestamp of transaction commit */
	ctLOGP	beginPosition; /* position of this transaction's begin entry in the transaction log */
	ctLOGP	commitPosition; /* position of this transaction's commit entry in the transaction log */
	ULONG	numberOfOperations; /* number of operations in this transaction */
	LONG	minlog; /* log requirement as of the checkpoint that precedes this transaction's begin entry */
	TRAN_STATE currentState; /* current state of the transaction */
} TRAN_STATE_LIST_ENTRY, *pTRAN_STATE_LIST_ENTRY;

/* status bits for transaction state list */
typedef enum {
	TRAN_STATE_OLDEST_REMOVED	= 0x00000001 /* oldest entry has been removed */
} TRAN_STATE_STATUS;

/* replication agent transaction state list */
typedef struct tranStateList_t {
	pTRAN_STATE_LIST_ENTRY head; /* head of list (first transaction in commit order) */
	pTRAN_STATE_LIST_ENTRY tail; /* tail of list (last transaction in commit order) */
	ULONG8	transactionid; /* transaction id of last commit position */
#ifdef ctThrds
	ctMUTEX lock; /* mutex used to serialize list operations */
#endif
	ULONG	numberOfEntries; /* number of entries on list */
	ULONG	numberOfOperations; /* total number of transaction operations on list */
	ctLOGP	lastCommitPosition; /* commit (abort) position of transaction last removed from head of list */
	ULONG	removeCount; /* count of entries removed from head of list since last reset */
	TEXT	status; /* status bits */
} TRAN_STATE_LIST, *pTRAN_STATE_LIST;

/* replication agent transaction skip list entry */
typedef struct tranSkipListEntry_t {
	struct tranSkipListEntry_t *next; /* next entry in list */
	ULONG8	transactionNumber; /* transaction number */
	ctLOGP	commitPosition; /* position of this transaction's commit entry in the transaction log */
} TRAN_SKIP_LIST_ENTRY, *pTRAN_SKIP_LIST_ENTRY;

/* replication agent transaction skip list */
typedef struct tranSkipList_t {
	pTRAN_SKIP_LIST_ENTRY head; /* head of list (first transaction in commit order) */
	pTRAN_SKIP_LIST_ENTRY tail; /* tail of list (last transaction in commit order) */
	ULONG	numberOfEntries; /* number of entries on list */
} TRAN_SKIP_LIST, *pTRAN_SKIP_LIST;

#ifdef ctPortWIN32
/* replication file handle */
typedef HANDLE pREPLFP;
#else
/* replication file handle */
typedef int pREPLFP;
#endif

typedef enum replDiagOptions_t {
	REPLDIAG_FILE_OPEN	= 0x00000001, /* diagnose file open/close operations of apply thread */
	REPLDIAG_TRAN_STATE	= 0x00000002, /* diagnose transaction state by log read thread */
	REPLDIAG_DEPEND_GRAPH	= 0x00000004, /* enable dependency graph logging */
	REPLDIAG_FILE_OPEN_ERR	= 0x00000008, /* enable file open failure logging */
	REPLDIAG_ANALYZER	= 0x00000010, /* enable dependency analysis logging */
	REPLDIAG_LOG_SHIP	= 0x00000020, /* enable log ship thread logging */
	REPLDIAG_LATENCY	= 0x00000040  /* enable latency logging */
} REPL_DIAG_OPTIONS;

/* option values for replciation agent's check_update configuration option */
typedef enum replCheckUpdateOptions_t {
	REPL_CHECK_UPDATE_NO = 0x00000001, /* do not check for differences in source/target record images */
	REPL_CHECK_UPDATE_YES = 0x00000002, /* check for differences in source/target record images and error if different */
	REPL_CHECK_UPDATE_LOG_ONLY = 0x00000004 /* check for differences in source/target record images and log message if different */
} REPL_CHECK_UPDATE_OPTIONS;

/* Prototype for replication agent trace function pointer. */
typedef void (*pREPLAGENT_TRACE_FUNCTION)(pctRASTT prepstt, cpTEXT fmt, ...) PRINTFLIKE(2,3);

/* Prototype for ctReplGetNextChangeDirect() function. */
typedef NINT (*pREPL_GET_NEXT_CHANGE_DIRECT)(NINT taskid,pctLOGP plogpos,pctCHGB pchgrec,NINT timeout,allocFunc_t allocFunc,freeFunc_t freeFunc);

/* call replication agent function profiling callback before function call */
#define REPLAGENT_PROFILE_ENTER_FUNCTION(prepstt,func) \
	if (prepstt && prepstt->pctgvar) { \
		prepstt->functionCall = func; \
		ctRActapicbMST(1, -ctSEPLNGreplagentfunc, prepstt->pctgvar); \
	}

/* call replication agent function profiling callback after function call */
#define REPLAGENT_PROFILE_EXIT_FUNCTION(prepstt) \
	if (prepstt && prepstt->pctgvar) { \
		prepstt->pctgvar->suerr_cod = (COUNT) rc; \
		ctRActapicbMST(0, -ctSEPLNGreplagentfunc, prepstt->pctgvar); \
	}

typedef enum replagentFunction_t {
	REPLAGENT_FUNCTION_QUEUE_READ = 1, /* ctThrdQueueRead */
	REPLAGENT_FUNCTION_ANALYZE_TRAN	= 2, /* analyzeTransaction */
	REPLAGENT_FUNCTION_ADD_TRAN_TO_GRAPH = 3, /* addTransactionToGraph */
	REPLAGENT_FUNCTION_THROTTLE_THREAD = 4, /* throttleThread */
	REPLAGENT_FUNCTION_REMOVE_GRAPH_ENTRY = 5, /* removeGraphEntry */
	REPLAGENT_FUNCTION_REMOVE_TRAN_STATE_ENTRY = 6, /* removeTranEntry */
	REPLAGENT_FUNCTION_WRITE_LOG = 7, /* writeLog */
	REPLAGENT_FUNCTION_FLUSH_LOG = 8, /* flushLog */
	REPLAGENT_FUNCTION_SIGNAL_AND_WAIT_FOR_FILE_CLOSE = 9 /* signalAndWaitForFileClose */
} REPLAGENT_FUNCTION;

/* total number of replication agent functions used in profiling */
#define NUMBER_OF_REPLAGENT_FUNCTIONS 9

#ifdef ctREPLAGENT
/* replication agent function names */
cpTEXT replagentFunctions[NUMBER_OF_REPLAGENT_FUNCTIONS] = {
	"ctThrdQueueRead",
	"analyzeTransaction",
	"addTransactionToGraph",
	"throttleThread",
	"removeGraphEntry",
	"removeTranEntry",
	"writeLog",
	"flushLog",
	"signalAndWaitForFileClose"
};
#else
/* replication agent function names */
extern cpTEXT replagentFunctions[NUMBER_OF_REPLAGENT_FUNCTIONS];
#endif

/* function pointer for adding file name to list of files open by replication apply thread */
typedef NINT (*addFileNameToOpenFileList_t)(pctRASTT prepstt, cpTEXT fileName, ULONG fileId);
/* function pointer for removing file name from list of files open by replication apply thread */
typedef void (*removeFileNameFromOpenFileList_t)(pctRASTT prepstt, cpTEXT fileName);
/* function pointer for adding entry to superfile host list */
typedef NINT (*superfileHostListAdd_t)(pSUPERFILE_HOST_LIST pSuperfileHostList, cpTEXT fileName, FILNO fileNumber, ULONG fileId, pSUPERFILE_HOST_LIST_ENTRY *ppNewListEntry);
/* function pointer for removing entry from superfile host list by file name */
typedef NINT (*superfileHostListRemoveByFileName_t)(pctCNXH pcnxhnd, pSUPERFILE_HOST_LIST pSuperfileHostList, cpTEXT fileName);
/* function pointer for removing entry from superfile host list by file number */
typedef NINT (*superfileHostListRemoveByFileNumber_t)(pctCNXH pcnxhnd, pSUPERFILE_HOST_LIST pSuperfileHostList, FILNO fileNumber);
/* function pointer for finding entry in superfile host list by superfile host name */
typedef NINT (*superfileHostListFindByName_t)(pSUPERFILE_HOST_LIST pSuperfileHostList, cpTEXT fileName, pSUPERFILE_HOST_LIST_ENTRY *ppListEntry);
/* function pointer for finding entry in superfile host list by user file number */
typedef NINT (*superfileHostListFindByFileNumber_t)(pSUPERFILE_HOST_LIST pSuperfileHostList, FILNO fileNumber, pSUPERFILE_HOST_LIST_ENTRY *ppListEntry);

/* rasttState data type */
typedef TEXT RASTT_STATE;

#ifdef ctFeatREPLAGENTparallel
#ifdef ctFeatREPLAGENT_SHARE_LOGSHIP

/* log read minlog list entry 0 holds the log ship state header */
#define FIRST_LOG_READ_MIN_LOG_ENTRY 1

#define LOG_SHIP_STATE_HEADER_VERS_V01	1 /* initial version of structure */

typedef struct logShipStateHeader_t {
	LONG	verson; /* structure version */
	LONG	minlog; /* overall minimum log requirement */
	ULONG	numberOfEntries; /* number of log read entries */
	LONG	readLogNumber;
	ULONG	readLogOffset;
	ULONG	unused[4]; /* unused space, pad to size of LOG_READ_MINLOG_LIST_ENTRY structure */
} LOG_SHIP_STATE_HEADER;

/* an entry in the list of minimum log requirements for all log readers that are using a log ship thread */
typedef struct logReadMinLogListEntry_t {
	TEXT	agentid[UNQIDLEN];
	LONG	minlog;
} LOG_READ_MINLOG_LIST_ENTRY;

/* log ship state header must be same size as log read minlog list entry */
CHECK_COMPILE_TIME_CONDITION(checkLogShipStateHeaderSize, sizeof(LOG_SHIP_STATE_HEADER) == sizeof(LOG_READ_MINLOG_LIST_ENTRY));

/* an entry in the log ship thread list */
typedef struct logShipThreadListEntry_t {
	struct logShipThreadListEntry_t* prev; /* previous entry in log ship list */
	struct logShipThreadListEntry_t* next; /* next entry in log ship list */
	pctRASTT logShipThreadState; /* log ship thread state info */
	FILE* logShipThreadStateFile;
	LOG_READ_MINLOG_LIST_ENTRY *logReadThreadMinLogList; /* minimum log requirements for log read threads that are using this log ship thread */
	pctRASTT *logReadThreadList; /* list of log read threads that are using this log ship thread */
	SYNC_CONDITION logshipSyncCondition; /* conditions for switching source server to sync mode */
	ctMUTEX	logshipSetFilterMutex; /* mutex used to serialize updates to log ship set filter indicator */
	UINT	logReadThreadCount; /* number of log read threads that are using this log ship thread */
	UINT	logReadThreadListSize; /* size of log read thread lists */
	UINT	logReadThreadsAttached; /* number of log read threads currently using the log ship thread */
	TEXT	sourceServerName[MAX_SERVER_NAME]; /* name of source server for this log ship thread */
	TEXT	logshipSetFilterMutexInit; /* non-zero if log ship set filter mutex has been initialized */
	TEXT	logshipSetFilter; /* non-zero when logship thread has been signaled to set file filter on source server */
	TEXT	logshipSetFilterAgentId[UNQIDLEN]; /* unique id of replication agent whose filter is being set */
	TEXT	hasSyncCondition; /* non-zero if a condition is set for switching source server to sync mode */
	TEXT	logshipPendingShutdown; /* non-zero if log ship is shutting down; new log readers must wait and retry */
} LOG_SHIP_THREAD_LIST_ENTRY;

#define GET_logshipSyncCondition(prepstt) prepstt->global->logShipThreadListEntry->logshipSyncCondition
#define GET_logshipSetFilterMutex(prepstt) prepstt->global->logShipThreadListEntry->logshipSetFilterMutex
#define GET_logshipSetFilterMutexInit(prepstt) prepstt->global->logShipThreadListEntry->logshipSetFilterMutexInit
#define GET_logshipSetFilter(prepstt) prepstt->global->logShipThreadListEntry->logshipSetFilter
#define GET_hasSyncCondition(prepstt) prepstt->global->logShipThreadListEntry->hasSyncCondition
#else
#define GET_logshipSyncCondition(prepstt) prepstt->global->logshipSyncCondition
#define GET_logshipSetFilterMutex(prepstt) prepstt->global->logshipSetFilterMutex
#define GET_logshipSetFilterMutexInit(prepstt) prepstt->global->logshipSetFilterMutexInit
#define GET_logshipSetFilter(prepstt) prepstt->global->logshipSetFilter
#define GET_hasSyncCondition(prepstt) prepstt->global->hasSyncCondition
#endif
#endif

/* replication agent initial log read position state */
typedef enum replInitState_t {
	REPL_INIT_POSITION_NOT_SPECIFIED		    = 0, /* no initial log read position was specified in configuration or ini file */
	REPL_INIT_POSITION_USE_CURRENT_FROM_CONFIG	    = 1, /* configuration file: use source server's current log position */
	REPL_INIT_POSITION_USE_TRANSACTION_ID_FROM_CONFIG   = 2, /* configuration file: use log position of specified transaction id */
	REPL_INIT_POSITION_USE_POSITION_FROM_CONFIG	    = 3, /* configuration file: use specified log position */
	REPL_INIT_POSITION_USE_CURRENT_FROM_INI_FILE	    = 4, /* ini file: use source server's current log position */
	REPL_INIT_POSITION_USE_TRANSACTION_ID_FROM_INI_FILE = 5, /* ini file: use log position of specified transaction id */
	REPL_INIT_POSITION_USE_POSITION_FROM_INI_FILE	    = 6  /* ini file: use specified log position */
} REPL_INIT_STATE;

/* values for rasttState field */
#define RASTT_ACTIVE		0x00 /* entry is active */
#define RASTT_BLOCKED		0x01 /* entry is blocked to external access */
#define RASTT_PENDING_DELETE	0x02 /* entry is pending delete */

/* number of transactions per commit */
#define REPLAGENT_TRANSACTION_BATCH_SIZE 1

/* Categories of replication events: */
typedef enum _rxevc {
	RX_EVENT_AGENT		= 1,	/* agent event			  */
	RX_EVENT_FILE		= 2,	/* file operation		  */
	RX_EVENT_TRANSACTION	= 3,	/* transaction operation	  */
	RX_EVENT_DATA		= 4,	/* data operation		  */
	RX_EVENT_EXCEPTION	= 5	/* exception			  */
} RXEVC;

/* Check structure versions for compatibility: */

#define RX_VEROP_VER1		1
#define RX_VEROP_VERSION	RX_VEROP_VER1 /* current version of rxVEROP  */

typedef struct _rxverop {
	LONG	verop_ver;	/* [OUT] Set to version of rxVEROP	  */
	LONG	event_ver;	/* [OUT] Set to version of rxEVENT	  */
	LONG	filop_ver;	/* [OUT] Set to version of rxFILOP	  */
	LONG	trnop_ver;	/* [OUT] Set to version of rxTRNOP	  */
	LONG	datop_ver;	/* [OUT] Set to version of rxDATOP	  */
	LONG	excop_ver;	/* [OUT] Set to version of rxEXCOP	  */
	LONG	filh_ver;	/* [OUT] Set to version of ctFILH	  */
} rxVEROP, *prxVEROP;

/* Parameters for file events: */

#define RX_FILOP_VER1		1
#define RX_FILOP_VERSION	RX_FILOP_VER1 /* current version of rxFILOP  */

typedef struct _rxfilop {
	pctCHGB pchgbuf;	/* [IN] File operation details		  */
	pctFILH	pfilhnd;	/* [IN] File information		  */
} rxFILOP, *prxFILOP;

/* Parameters for transaction events: */

#define RX_TRNOP_VER1		1
#define RX_TRNOP_VERSION	RX_TRNOP_VER1 /* current version of rxTRNOP  */

typedef struct _rxtrnop {
	pctCHGB pchgbuf;	/* [IN] Transaction operation details	  */
} rxTRNOP, *prxTRNOP;

/* Parameters for data events: */

#define RX_DATOP_VER1		1
#define RX_DATOP_VERSION	RX_DATOP_VER1 /* current version of rxDATOP  */

typedef struct _rxdatop {
	pctCHGB pchgbuf;	/* [IN] Data operation details		  */
	pctFILH	pfilhnd;	/* [IN] File information		  */
} rxDATOP, *prxDATOP;

/* Parameters for exceptions: */

#define RX_EXCOP_VER1		1
#define RX_EXCOP_VERSION	RX_EXCOP_VER1 /* current version of rxEXCOP  */

typedef struct _rxexcop {
	LONG	errcod;		/* [IN] Error code */
	pctCHGB pchgbuf;	/* [IN] Operation that failed */
	pctFILH	pfilhnd;	/* [IN] File information */
} rxEXCOP, *prxEXCOP;

/* Parameters for all events: */

#define RX_EVENT_VER1		1
#define RX_EVENT_VERSION	RX_EVENT_VER1 /* current version of rxEVENT  */

typedef struct _rxevent {
	LONG	versn;		   /* [IN] version of this structure	     */
	LONG	errcod;		   /* [IN] Error code			     */
	pctCNXH	psrccnxhnd;	   /* [IN] Source server connection handle   */
	pctCNXH	ptgtcnxhnd;	   /* [IN] Target server connection handle   */
	pVOID	pusrctx;	   /* [OUT] User context pointer	     */
	RXACT	action;		   /* [OUT] Set to action to take	     */
	union {
		rxVEROP verop;	   /* [IN] Parameters for version check	     */
		rxFILOP filop;     /* [IN] Parameters for file events	     */
		rxTRNOP trnop;     /* [IN] Parameters for transaction events */
		rxDATOP datop;     /* [IN] Parameters for data events	     */
		rxEXCOP excop;     /* [IN] Parameters for exceptions	     */
		REPLAGENT_SHUTDOWN_INFO shutdownInfo; /* [IN] Parameters for sync shutdown */
	} ev;
	TEXT	unqid[UNQIDLEN+1]; /* [IN] Replication agent unique ID	     */
	TEXT	srcsvn[MAX_SERVER_NAME]; /* [IN] Name of source c-tree Server */
	TEXT	tgtsvn[MAX_SERVER_NAME]; /* [IN] Name of target c-tree Server */
	TEXT	srcnodeid[IDZ];    /* [IN] Expected node ID of source server */
	TEXT	tgtnodeid[IDZ];    /* [IN] Expected node ID of target server */
} rxEVENT, *prxEVENT;

typedef VOID (*pRXFNCP)(prxEVENT prxagentev);

/* Total number of replication extension library events */
#define RXF_TOTAL 54

/* information for a replication agent extension library */
typedef struct replAgentExtensionLibraryListEntry_t {
	struct replAgentExtensionLibraryListEntry_t*
		next; /* next entry in list */
	pVOID	libraryHandle; /* Extension library handle */
	pVOID	userContext; /* Extension library user context pointer */
	pRXFNCP rxLibFuncPtrs[RXF_TOTAL];
	TEXT	libraryName[MAX_NAME]; /* Name of extension library */
} REPLAGENT_EXTENSION_LIBRARY_LIST_ENTRY;

/* single instance global variables for replication agent */
typedef struct replAgentGlobal_t {
	NINT	batchSize; /* Size of buffer for reading changes from source */
	TEXT	showChangeDetails; /* Show change details */
	TEXT	exceptionMode; /* Exception handling mode */
	TEXT	startedDBengine; /* Started embedded database engine. */
	TEXT	useSystemEventLog; /* Write messages to system event log */
	TEXT	debugMinLog; /* Diagnostic logging of minimum log requirements */
	TEXT	createFiles; /* Create files on data target if they do not exist */
	TEXT	MACOSX_GUIflag;/* Apple MAC OSX GUI interface flag */
	TEXT	useTargetAlignment; /* Use target file record alignment for source */
	TEXT	cfgFileName[MAX_NAME];	/* Configuration file name */
	TEXT	logReadConnected; /* log read has connected to source and target servers */
#ifdef ctFeatREPLICAT_SYNC
	TEXT	syncAgent; /* synchronous agent (if non-zero) */
#endif
	UREDLST redirectList; /* Filename redirection rule list */
	TEXT	isEmbedded; /* Replication agent is embedded in server */
	TEXT	skipApply; /* Skip applying operations to target */
	pEXCEPCBFNC exceptionCallbackFunc; /* Replication exception callback function */
	pVOID	exceptionCallbackContext; /* Replication exception callback context */
	TEXT	statusMessage[512]; /* agent status message */
#ifdef ctThrds
	ctMUTEX statusMessageLock; /* lock to control access to status message buffer */
#endif
#ifdef ctFeatSSL
	TEXT	srcUseTLS;	/* use TLS for connection to source server */
	TEXT	tgtUseTLS;	/* use TLS for connection to target server */
	TEXT	srcTLScertSet;	/* Is TLS certificate name specified for source server? */
	TEXT	tgtTLScertSet;	/* Is TLS certificate name specified for target server? */
	TEXT	srcTLScert[MAX_NAME];	/* TLS certficate for connection to source server */
	TEXT	tgtTLScert[MAX_NAME];	/* TLS certficate for connection to target server */
#endif
	TEXT	sourceServerAuthFile[MAX_NAME]; /* Authentication file for source server */
	TEXT	targetServerAuthFile[MAX_NAME]; /* Authentication file for target server */
	REPLAGENT_EXTENSION_LIBRARY_LIST_ENTRY*
		extensionLibraryListHead; /* first entry in extension library list */
	REPLAGENT_EXTENSION_LIBRARY_LIST_ENTRY*
		extensionLibraryListTail; /* last entry in extension library list  */
#ifdef ctFeatREPLAGENT_FILE_GROUP_FILTER
	TEXT	resyncFileName[MAX_NAME]; /* name of file containing list of files to resync */
	pTEXT	resyncFileList; /* files to resync */
	ctLOGP	stopAtLogReadPosition; /* stop agent at this log read position */
	COUNT	deployReturnCode; /* return code from deploy operation */
	TEXT	resyncId[32]; /* resync id in transaction log entry */
	TEXT	resyncComplete; /* agent sets to non-zero when resync completed */
	TEXT	clearStateOnShutdown; /* agent clears its state when shutting down */
	TEXT	deployFileGroups; /* non-zero if agent is deploying file groups */
	COUNT	deploySucceeded; /* non-zero if deploy succeeded */
	TEXT	fileGroupParallelApply; /* Use parallelism within a file group (if non-zero) */
#endif
	TEXT	parallelApply; /* Use parallelism (if non-zero) */
#if defined(ctFeatREPLAGENTparallel) || defined(ctSRVR)
	ctMUTEX	logmsgMutex; /* mutex used to serialize writes to ctreplagent.log */
	TEXT	logmsgMutexInit; /* non-zero if logmsg mutex has been initialized */
	ctMUTEX	logPositionMutex; /* mutex used to serialize access to the log reader thread's current log position state variables */
	ctMUTEX	logShipPositionMutex; /* mutex used to serialize access to the log ship thread's current log position state variables */
	ctLOGP	logShipCurrentPosition; /* current log ship log position */
	TEXT	logShipSetPosition; /* non-zero if log position has been set */
	TEXT	logShipBlockLogReader; /* non-zero if log reader must wait on log ship to be ready */
	NINT	numAnalyzerThreads;/* Number of analyzer threads */
	NINT	numApplyThreads;   /* Number of apply threads */
#ifdef ctFeatREPLAGENT_SHARE_LOGSHIP
	LOG_SHIP_THREAD_LIST_ENTRY* logShipThreadListEntry; /* log ship thread list entry that this replication agent is using */
#else
	SYNC_CONDITION logshipSyncCondition; /* conditions for switching source server to sync mode */
	ctMUTEX	logshipSetFilterMutex; /* mutex used to serialize updates to log ship set filter indicator */
	TEXT	logshipSetFilterMutexInit; /* non-zero if log ship set filter mutex has been initialized */
	TEXT	logshipSetFilter; /* non-zero when logship thread has been signaled to set file filter on source server */
	TEXT	hasSyncCondition; /* non-zero if a condition is set for switching source server to sync mode */
#endif
	TEXT	preShutdown; /* non-zero when agent plugin is starting to shut down the agent */
	TEXT	signaledLogShipShutdown; /* at shutdown, this agent had the last log reader for its log ship thread */
	TEXT	checkDependencies; /* Check transaction dependencies (if non-zero)	*/
	TEXT	coordinateCommit;  /* Order commits on target to match source commit order (if non-zero) */
#endif
	void*	fileGroupIdLogStateMap; /* map of file group ids to log state */
	LONG8	lastcommitTimestamp; /* if non-zero, timestamp of last commit */
	LONG8	checkpointTimestamp; /* timestamp of most recent checkpoint entry read from logs */
	LONG	checkpointMinLog; /* minimum log requirement as of the last checkpoint we read */
	ctLOGP	logReadPositionFromConfig; /* log read position read from config */
	ctLOGP	lastCommitPositionFromConfig; /* last commit position read from config */
	ctLOGP	lastProcessedLogPosition; /* last log position whose entry log read processed */
	ctLOGP	oldestUncommittedPosition; /* lowest log position of transaction commit we have read but not yet applied */
	ULONG	uncommittedTransactionCount; /* number of transactions read but not yet committed */
	LONG	minLogNumberFromConfig; /* minimum log read from config */
	REPL_INIT_STATE inistate; /* source of log read initialization (config/ini/default) */
	ctLOGP	checkpointPosition; /* log position of checkpoint where we saved off the minimum log requirement */
	COUNT	stateRecordIsamContext; /* ISAM context for replication state record on target server */
	TEXT	deleteToRecycleBin; /* Delete files to recycle bin on target server */
#ifdef ctThrds
	TEXT	ctreplinit; /* Has ctThrdInit been called yet? */
	TLSK	tlsiReplCnxId; /* TLS index for connection ID. */
	TLSK	tlsiReplCnxCur;	/* TLS index for connection ID. */
#endif
} REPLAGENT_GLOBAL, *pREPLAGENT_GLOBAL;

typedef struct ctreedbsFunctionPointers_t {
	pREPL_GET_NEXT_CHANGE_DIRECT pctReplGetNextChangeDirect; /* ctReplGetNextChangeDirect function pointer */
} CTREEDBS_FUNCTION_POINTERS;

#define NUMBER_OF_CTREEDBS_FUNCTION_POINTERS 1

typedef struct _ctrastt {
	/* These settings are configurable in ctreplagent.cfg */

	TEXT	slclsvn[MAX_SERVER_NAME]; /* Name of target c-tree Server		*/
	TEXT	smstsvn[MAX_SERVER_NAME]; /* Name of source c-tree Server		*/
	NINT	stimeout;	   /* Timeout for reading changes from source		*/
	TEXT	smstpos;	   /* Remember repl agent log position on source	*/
	TEXT	sunqid[UNQIDLEN + 1];/* Replication agent unique ID			*/
#ifdef ctFeatREPLAGENT_SHARE_LOGSHIP
	TEXT	slogShipUniqueId[UNQIDLEN+1];/* Log ship thread unique ID		*/
	pTEXT	logShipFilter; /* file filter set by log read for log ship to set on source server */
#endif
	TEXT	sfltnam[MAX_NAME]; /* Replication file filter list name			*/
	TEXT	ssrcnodeid[IDZ];   /* Expected node ID of source server			*/
	TEXT	stgtnodeid[IDZ];   /* Expected node ID of target server			*/
	TEXT	sfnctim;	   /* Function timing option				*/

	/* Internal state variables */

	pRALFN	sctRAlog;	/* Replication agent logging function			*/
	pReplAgentAdjustMsgListReferenceCount replAgentAdjustMsgListReferenceCount;
				/* function to increment or decrement msgList reference count */
#ifdef ctFeatATOMICop
	AULONG	sshtdwn;	/* Shutdown indicator					*/
#else
	ULONG	sshtdwn;	/* Shutdown indicator					*/
#endif
	TEXT	spause;		/* Pause indicator					*/
	TEXT	ssetlgpos;
	pFILE	slogfp;		/* Replication agent log file handle			*/
	NINT	spndcmt;	/* Number of transactions pending commit completion	*/
#ifdef ctThrds
	LONG8	strncnt;	/* Number of transactions read.				*/
	LONG8	strnprv;	/* Previous transaction count.				*/
#endif
	NINT	strylcl;	/* Target server connection attempt count.		*/
	NINT	strymst;	/* Source server connection attempt count.		*/
	NINT	slclrc;		/* Target server connection attempt error code.		*/
	NINT	smstrc;		/* Source server connection attempt error code.		*/
	NINT	stbeg;
	TEXT	sdetlog[MAX_NAME];
	ctCNXI	scnxinf[2];
	ctCHGB	schgbuf;
	pctCHGB	pOperationChangeBuffer; /* Change buffer for current operation.		*/
	cpTEXT	pCallingFunction; /* Calling function for this operation.		*/
	ctLOGP	slogpos;
	ctLOGP	slastcmtlogpos;	/* Log position of last transaction committed on target	*/
	ctLOGP	slastredlogpos;	/* Log position of last entry read from source		*/
	ULONG8	stransactionid;	/* Transaction id of last commit from source		*/
	raSTAT	sraSTATi;
	NINT	stgtstate;
	NINT	srepflags;	/* Replication flags					*/
	NINT	srepstate;	/* Replication state bits				*/
	FILNO	sxplgdat;	/* Replication exception log data file number		*/
	NINT	srednodeid[2];	/* Non-zero if we read node ID from server		*/
	TEXT	ssrvnodeid[2][32]; /* Server's current node ID				*/
#ifdef ctBEHAV_REPLPOS
	LONG	sminlog;	/* Minimum log required on source server		*/
	LONG	sminlogSaved;	/* Minimum log value last saved to target server 	*/
#endif
	LONG	ssocktimeout;	/* Timeout for socket send and receive operations	*/
	ULONG	refcnt;		/* reference count on this repstt pointer		*/
	RASTT_STATE rasttState; /* current state of this entry				*/
	ctCNXH	scnxhnd[2];
	ULONG	sreploptions;	/* Replication agent options				*/
	pVOID	sctlibhandle;	/* Local c-tree library handle				*/
	NINT	exceptionLogOptions; /* Exception logging options */
	REPL_CHECK_UPDATE_OPTIONS schkupd; /* Check old record image on update */
#ifdef ctFeatREPLICAT_FLXDAT
	NINT	schkschmid;	/* Check schema ID on add/del/upd/hotalter */
#endif
	TEXT	logFileName[MAX_NAME]; /* replication agent text log file name (default ctreplagent.log) */
	TEXT	replicateDataDefinitions; /* if non-zero, replicate data definition operations (file create, etc.) */
	REPL_DIAG_OPTIONS diagOptions; /* diagnostic options */
#ifdef ctFeatREPLAGENTparallel
	NINT	tranAnalysisQueueId; /* transaction analysis queue id */
	NINT	dependCheckQueueId; /* dependency check queue id */
	NINT	tranReadyQueueId; /* transaction ready queue id */
	NINT	numberOfTranReadyQueues; /* count of transaction ready queues */
	pNINT	pTranReadyQueueList; /* list of transaction ready queue handles */
	pNINT	pApplyNotificationQueues; /* queues to notify apply threads of events */
	AGENT_THREAD_TYPE threadType; /* type of replication agent thread */
	NINT	threadInstance; /* instance number of the specified type of thread */
	struct _ctrastt **pParallelThreadState; /* replication state pointer array for parallel threads */
	NINT	numParallelThreadEntries; /* number of replication state pointers for parallel threads */
	AULONG	threadCount; /* current number of running threads */
	AULONG	pausedThreadCount; /* current number of paused threads */
	pREPLAGENT_TRACE_FUNCTION pTraceFunction; /* trace function */
	FILNO	logShipStateTableFilno; /* file number of log ship state table */
	pREPLFP	localLogFilePointer; /* local transaction log file pointer */
	TEXT	localLogFileIsOpen; /* indicates if local log is open */
	TEXT	syncLogWrites; /* indicates if local transaction log writes are synchronous */
	TEXT	localLogFilePath[MAX_NAME]; /* path where local logs are stored */
	pAPPLY_CLOSE_FILE_LIST_ENTRY pFileCloseListHead; /* file close list head (used by apply threads) */
	pAPPLY_CLOSE_FILE_LIST_ENTRY pFileCloseListTail; /* file close list tail (used by apply threads) */
	TRAN_STATE_LIST tranStateList; /* transaction state list */
	TRAN_SKIP_LIST tranSkipList; /* transaction skip list, used when resuming operation */
	DEPENDENCY_GRAPH dependencyGraph; /* dependency graph */
	FILNO	tranStateIndexKeyno; /* file number of transaction state index */
	ctLOGP	tranAnalysisQueueLastOffset; /* the log position of the last tran written to the analysis queue */
	NINT	taskid; /* taskid of connection to in-process server (logread) */
	CTREEDBS_FUNCTION_POINTERS ctreedbsFunctionPointers; /* server DLL function pointers */
	pVOID ctreedbsHandle; /* handle to dynamically-loaded ctreedbs server library */
	NINT	functionCall; /* current function call */
	NINT	errorCode; /* current error code */
	NINT	sourceTaskId; /* task id of this thread's connection to the source server */
	NINT	targetTaskId; /* task id of this thread's connection to the target server */
	ULONG	numberOfOpenFiles; /* number of open files (apply thread) */
	pCTGVAR pctgvar; /* c-tree global variable pointer */
	ULONG8	threadId; /* thread id */
	CLOSE_FILES_BEFORE_TRAN_LIST filesOpenByApply; /* list of files currently open by apply thread */
	addFileNameToOpenFileList_t pAddFileNameToOpenFileList; /* function for adding file name to list of open files */
	removeFileNameFromOpenFileList_t pRemoveFileNameFromOpenFileList; /* function for removing file name from list of open files */
	SUPERFILE_HOST_LIST superfileHosts; /* list of superfile hosts open by this thread */
	superfileHostListAdd_t pAddToSuperfileHostList; /* add entry to superfile host list */
	superfileHostListRemoveByFileName_t pRemoveFromSuperfileHostListByFileName; /* remove entry from superfile host list by file name */
	superfileHostListRemoveByFileNumber_t pRemoveFromSuperfileHostListByFileNumber; /* remove entry from superfile host list by file number */
	superfileHostListFindByName_t pSearchSuperfileHostListByFileName; /* find entry in superfile host list by file name */
	superfileHostListFindByFileNumber_t pSearchSuperfileHostListByFileNumber; /* find entry in superfile host list by file number */
#endif /* ctFeatREPLAGENTparallel */
	pREPLAGENT_GLOBAL global; /* replication agent state variables that are single instance (not per thread) */
	TEXT	freeGlobal; /* non-zero if ctReplInitXtd() allocated global */
#ifdef ctFeatREPLAGENTprofile
#define ctMAXfncREPL 256 /* ctMAXfnc */
#define REPLAGENT_FUNCTION_ARRAY_SIZE ctMAXfncREPL + ctSEPLNGlimit + NUMBER_OF_REPLAGENT_FUNCTIONS + 1
	ctRFSTT	srfstt[REPLAGENT_FUNCTION_ARRAY_SIZE]; /* replication function stats	*/
	LONG8	srfsttbas;	/* replication base time */
	LONG8	srfsttint;	/* replication internal time */
#endif
	TEXT	sdatpath[MAX_NAME]; /* replication data directory */
	ctLOGP	slastresynclogpos;	/* Log position of last resync executed by the replication agent	*/
#ifdef ctFeatREPL_FILTER_CHANGE
	FILNO	sffchgdat;		/* File filter change datno */
	ctREPFFCHG sffchgst;		/* File filter change status */
	TEXT	sffchgmsg[ctREPL_ERROR_LEN];	/* File filter change error message */
	NINT	sffchgerr;		/* File filter change error code */
	ctMUTEX sffchgmtx;		/* File filter change mutex */
#endif
	TEXT	sunreg;		/* Unregister indicator */
	TEXT	syncshutdown;	/* Sync on shutdown */
	TEXT	settargetserverwritable; /* set target server writable on shutdown */
#ifdef ctFeatREPLAGENTdll
	pVOID   msgList;	/* Pointer to external connection pool */
#endif
#ifdef ctFeatREPLAGENTargs
	NINT	argc;		/* Count of command-line arguments */
	ppTEXT	argv;		/* Command-line argument list */
#endif
} ctRASTT;

#define g_ctRAlog	 prepstt->sctRAlog
#define g_shtdwn	 prepstt->sshtdwn
#define g_pause		 prepstt->spause
#define g_setlgpos	 prepstt->ssetlgpos
#define g_logfp		 prepstt->slogfp
#define g_pndcmt	 prepstt->spndcmt
#define g_trncnt	 prepstt->strncnt
#define g_trnprv	 prepstt->strnprv
#define g_trylcl	 prepstt->strylcl
#define g_trymst	 prepstt->strymst
#define g_lclrc		 prepstt->slclrc
#define g_mstrc		 prepstt->smstrc
#define g_tbeg		 prepstt->stbeg
#define g_detlog	 prepstt->sdetlog
#define g_cnxhnd	 prepstt->scnxhnd
#define g_cnxinf	 prepstt->scnxinf
#define g_chgbuf	 prepstt->schgbuf
#define g_logpos	 prepstt->slogpos
#define g_lastcmtlogpos	 prepstt->slastcmtlogpos
#define g_lastresynclogpos	 prepstt->slastresynclogpos
#define g_lastredlogpos	 prepstt->slastredlogpos
#define g_transactionid	 prepstt->stransactionid
#define g_lastredtstamp	 prepstt->slastredtstamp
#define g_lastnotranpos	 prepstt->slastnotranpos
#define g_agentindex	 prepstt->sagentindex
#define g_agentid	 prepstt->sagentid
#define g_sourcesrvrid	 prepstt->ssourcesrvrid
#define g_raSTATi	 prepstt->sraSTATi
#define g_inistate	 prepstt->sinistate
#define g_tgtstate	 prepstt->stgtstate
#define g_repflags	 prepstt->srepflags
#define g_repstate	 prepstt->srepstate
#define g_rednodeid      prepstt->srednodeid
#define g_srvnodeid      prepstt->ssrvnodeid
#define g_minlog	 prepstt->sminlog
#define g_socktimeout	 prepstt->ssocktimeout
#define g_rfstt		 prepstt->srfstt
#define g_rfsttint	 prepstt->srfsttint
#define g_rfsttbas	 prepstt->srfsttbas
#define g_unreg		 prepstt->sunreg

/* These settings are configurable in ctreplagent.cfg */
#define g_lclsvn	 prepstt->slclsvn
#define g_mstsvn	 prepstt->smstsvn
#define g_timeout	 prepstt->stimeout
#define g_mstpos	 prepstt->smstpos
#define g_unqid		 prepstt->sunqid
#define g_logShipUniqueId prepstt->slogShipUniqueId
#define g_xplgdat	 prepstt->sxplgdat
#define g_reploptions    prepstt->sreploptions
#define g_fltnam	 prepstt->sfltnam
#define g_srcnodeid	 prepstt->ssrcnodeid
#define g_tgtnodeid	 prepstt->stgtnodeid
#define g_fnctim	 prepstt->sfnctim
#define g_datpath	 prepstt->sdatpath
#define g_chkupd	 prepstt->schkupd
#define g_chkschmid	 prepstt->schkschmid
#define g_pusrctx	 prepstt->spusrctx
#define g_logFileName	 prepstt->logFileName

#ifdef ctFeatREPL_FILTER_CHANGE
#define g_ffchgdat	 prepstt->sffchgdat
#define g_ffchgst	 prepstt->sffchgst
#define g_ffchgerr	 prepstt->sffchgerr
#define g_ffchgmsg	 prepstt->sffchgmsg
#endif /* ctFeatREPL_FILTER_CHANGE */

/* Support external notification of replication agent events */
#define ctFeatREPLAGENTnotify

#ifdef ctFeatREPLAGENTprofile
typedef struct rfstwrk {
	LONG8	tim;
	LONG8	cnt;
	LONG	fnc;
} RFSTW, *pRFSTW;

/* State information for a replication admin connection. */
typedef struct _radstt {
	pRFSTW	prfstw;	/* Current function timings.			*/
	pRFSTW	prfstwP;/* Previous function timings.			*/
	LONG8	prvtim; /* Timestamp for previous function timings.	*/
} RADSTT, *pRADSTT;
#endif /* ctFeatREPLAGENTprofile */

static const int MAX_ANALYZER_THREADS = 16; /* maximum number of analyzer threads */
static const int MAX_APPLY_THREADS = 8192; /* maximum number of apply threads */

/* replication agent server connection options */
typedef enum racopt_t {
	RACOPT_apply_filter = 1 /* apply replication file filter */
} RACOPT;

/* ctReplSetAgentState() mode parameter values: */
typedef enum setagentstatemode_t {
	SET_AGENT_STATE_DELETE_STATE_POINTER	= 0, /* remove replication state pointer from agent state pointer list */
	SET_AGENT_STATE_ADD_STATE_POINTER	= 1, /* add replication state pointer to agent state pointer list */
	SET_AGENT_STATE_UPDATE_LOG_SHIP_LIST	= 2, /* update state in log ship list */
	CHECK_SYNC_LICENSE			= 3, /* Check if agent is licensed for synchronous replication */
	CHECK_PARALLEL_LICENSE			= 4, /* Check if agent is licensed for parallel replication */
	CONNECT_TO_LOCAL_SERVER			= 5, /* replication agent connecting to embedded server */
	CONNECT_TO_MEMPHIS			= 6, /* connecting to memphis */
	SET_AGENT_STATE_BLOCK_STATE_POINTER	= 7, /* block state pointer access */
	GET_REPL_SHUTDOWN_MODE			= 8, /* get the replication agent shutdown mode */
	SET_REPLAGENT_MANAGER_QUEUE_ID		= 9, /* set the replication agent queue id */
	CHECK_EXTENSION_LICENSE = 10  /* Check if the agent is licensed for extension library */
} SETAGENTSTATE_MODE;

/* replication agent transaction analyzer queue message format */
typedef struct analyzerQueueMsg_t {
	ULONG8	tranSequenceNumber; /* transaction sequence number */
	pctOPLH	tranOperationList; /* transaction operation list */
} ANALYZER_QUEUE_MESSAGE, *pANALYZER_QUEUE_MESSAGE;

/* replication agent apply notification queue message type */
typedef enum applyNotificationQueueMsgType_t {
	CLOSE_FILE_IN_LOG	= 1, /* notification of file close at specified log position */
	MUST_CLOSE_THESE_FILES	= 2, /* notification of other apply thread requiring this apply thread to close the specified files */
	CLOSE_FILES_COMPLETED	= 3 /* notification that apply thread has finished closing the specified files */
} APPLY_NOTIFICATION_QUEUE_MESSAGE_TYPE;

/* replication agent apply notification queue message format */
typedef struct applyNotificationQueueMsg_t {
	LONG	messageType; /* type of message */
	ULONG8	dataValue; /* data for the operation */
	ctLOGP	logPosition; /* transaction log position for the operation */
} APPLY_NOTIFICATION_QUEUE_MESSAGE, *pAPPLY_NOTIFICATION_QUEUE_MESSAGE;

/* format of apply notification queue message of type MUST_CLOSE_THESE_FILES */
typedef struct mustCloseFilesQueueMessage_t {
	LONG	messageType; /* type of message: MUST_CLOSE_THESE_FILES */
	ULONG	requestingThreadInstance; /* the apply thread instance that is requesting files to be closed */
	ULONG	numberOfFiles; /* number of files in file name list */
	TEXT	fileNameList[1]; /* null-terminated list of names of files to close */
} MUST_CLOSE_FILES_QUEUE_MESSAGE, *pMUST_CLOSE_FILES_QUEUE_MESSAGE;

/* possible states of a replicated transaction */
typedef enum replTranState_t {
	REPL_TRAN_PENDING	= 1, /* transaction is pending apply */
	REPL_TRAN_COMMITTED	= 2, /* transaction has been committed */
	REPL_TRAN_ABORTED	= 3  /* transaction has been aborted */
} REPL_TRAN_STATE;

/* replication agent main thread stats */
typedef struct replAgentMainStats_t {
	LONG	threadType;	/* thread type				*/
	LONG	structVersion;	/* version of this structure		*/
	TEXT	sourceServer[64]; /* name of source server		*/
	TEXT	targetServer[64]; /* name of target server		*/
} MAIN_THREAD_STATS, *pMAIN_THREAD_STATS;

#define MAIN_THREAD_STATS_VERS_V01	1 /* initial version of main thread stats structure */
#define MAIN_THREAD_STATS_VERS_CUR	MAIN_THREAD_STATS_VERS_V01 /* current version of main thread stats structure */
#define MAIN_THREAD_STATS_SIZE_CUR	sizeof(MAIN_THREAD_STATS) /* size of main thread stats structure */

/* log ship thread stats */
typedef struct replLogShipThreadStats_t {
	LONG	threadType;	/* thread type				*/
	LONG	structVersion;	/* version of this structure		*/
	LONG8	fnseqno;	/* current function call sequence #	*/
	ctLOGP	logPosition;	/* Log position of current scan pos	*/
	LONG	curstate;	/* current status of thread		*/
	LONG	constate;	/* server connection status		*/
	LONG	errorCode;	/* current error code			*/
	LONG	sourceTaskId;	/* task id of connection to source server */
	TEXT	curfunction[64]; /* current function			*/
} LOGSHIP_THREAD_STATS, *pLOGSHIP_THREAD_STATS;

#define LOGSHIP_THREAD_STATS_VERS_V01	1 /* initial version of logship thread stats structure */
#define LOGSHIP_THREAD_STATS_VERS_CUR	LOGSHIP_THREAD_STATS_VERS_V01 /* current version of logship thread stats structure */
#define LOGSHIP_THREAD_STATS_SIZE_CUR	sizeof(LOGSHIP_THREAD_STATS) /* size of logship thread stats structure */

/* log read thread stats version 1 */
typedef struct replLogReadThreadStats1_t {
	LONG	threadType;	/* thread type				*/
	LONG	structVersion;	/* version of this structure		*/
	LONG8	fnseqno;	/* current function call sequence #	*/
	ctLOGP	logPosition;	/* Log position of current scan pos	*/
	LONG	curstate;	/* current status of thread		*/
	LONG	constate;	/* server connection status		*/
	LONG	errorCode;	/* current error code			*/
	LONG	sourceTaskId;	/* task id of connection to source server */
	LONG	targetTaskId;	/* task id of connection to target server */
	LONG	tdelta;		/* Relative time of last commit (sec)	*/
	TEXT	curfunction[64]; /* current function			*/
} LOGREAD_THREAD_STATS1;

/* log read thread stats version 2 */
typedef struct replLogReadThreadStats_t {
	LONG	threadType;	/* thread type				*/
	LONG	structVersion;	/* version of this structure		*/
	LONG8	fnseqno;	/* current function call sequence #	*/
	ctLOGP	logPosition;	/* Log position of current scan pos	*/
	ctLOGP	oldestUncommittedLogPosition; /* Log position of oldest transaction read but not committed yet */
	LONG	curstate;	/* current status of thread		*/
	LONG	constate;	/* server connection status		*/
	LONG	errorCode;	/* current error code			*/
	LONG	sourceTaskId;	/* task id of connection to source server */
	LONG	targetTaskId;	/* task id of connection to target server */
	LONG	tdelta;		/* Relative time of last commit (sec)	*/
	TEXT	curfunction[64]; /* current function			*/
} LOGREAD_THREAD_STATS, *pLOGREAD_THREAD_STATS;

#define LOGREAD_THREAD_STATS_VERS_V01	1 /* initial version of logread thread stats structure */
#define LOGREAD_THREAD_STATS_VERS_V02	2 /* second version of logread thread stats structure */
#define LOGREAD_THREAD_STATS_VERS_CUR	LOGREAD_THREAD_STATS_VERS_V02 /* current version of logread thread stats structure */
#define LOGREAD_THREAD_STATS_SIZE_V01	sizeof(LOGREAD_THREAD_STATS1) /* size of logread thread stats structure version 1 */
#define LOGREAD_THREAD_STATS_SIZE_CUR	sizeof(LOGREAD_THREAD_STATS) /* size of logread thread stats structure version 2 */

/* analyzer thread stats */
typedef struct replAnalyzerThreadStats_t {
	LONG	threadType;	/* thread type				*/
	LONG	structVersion;	/* version of this structure		*/
	LONG8	fnseqno;	/* current function call sequence #	*/
	LONG	threadInstance;	/* thread instance			*/
	LONG	curstate;	/* current status of thread		*/
	LONG	errorCode;	/* current error code			*/
	TEXT	curfunction[64]; /* current function			*/
} ANALYZER_THREAD_STATS, *pANALYZER_THREAD_STATS;

#define ANALYZER_THREAD_STATS_VERS_V01	1 /* initial version of analyzer thread stats structure */
#define ANALYZER_THREAD_STATS_VERS_CUR	ANALYZER_THREAD_STATS_VERS_V01 /* current version of analyzer thread stats structure */
#define ANALYZER_THREAD_STATS_SIZE_CUR	sizeof(ANALYZER_THREAD_STATS) /* size of analyzer thread stats structure */

/* dependency check thread stats */
typedef struct replDependencyThreadStats_t {
	LONG	threadType;	/* thread type				*/
	LONG	structVersion;	/* version of this structure		*/
	LONG8	fnseqno;	/* current function call sequence #	*/
	LONG	numberOfGraphEntries; /* number of entries in dependency graph */
	LONG	curstate;	/* current status of thread		*/
	LONG	errorCode;	/* current error code			*/
	TEXT	curfunction[64]; /* current function			*/
} DEPENDENCY_THREAD_STATS, *pDEPENDENCY_THREAD_STATS;

#define DEPENDENCY_THREAD_STATS_VERS_V01	1 /* initial version of dependency thread stats structure */
#define DEPENDENCY_THREAD_STATS_VERS_CUR	DEPENDENCY_THREAD_STATS_VERS_V01 /* current version of dependency thread stats structure */
#define DEPENDENCY_THREAD_STATS_SIZE_CUR	sizeof(DEPENDENCY_THREAD_STATS) /* size of dependency thread stats structure */

/* apply thread stats */
typedef struct replApplyThreadStats_t {
	LONG	threadType;	/* thread type				*/
	LONG	structVersion;	/* version of this structure		*/
	LONG8	fnseqno;	/* current function call sequence #	*/
	LONG	threadInstance;	/* thread instance			*/
	ctLOGP	logPosition;	/* Log position of current scan pos	*/
	LONG	curstate;	/* current status of thread		*/
	LONG	constate;	/* server connection status		*/
	LONG	errorCode;	/* current error code			*/
	ULONG	numberOfOpenFiles; /* current number of open data files */
	LONG	targetTaskId;	/* task id of connection to target server */
	TEXT	curfunction[64]; /* current function			*/
} APPLY_THREAD_STATS, *pAPPLY_THREAD_STATS;

#define APPLY_THREAD_STATS_VERS_V01	1 /* initial version of apply thread stats structure */
#define APPLY_THREAD_STATS_VERS_CUR	APPLY_THREAD_STATS_VERS_V01 /* current version of apply thread stats structure */
#define APPLY_THREAD_STATS_SIZE_CUR	sizeof(APPLY_THREAD_STATS) /* size of apply thread stats structure */

/* oldest uncommitted transaction information */
typedef struct oldestUncommittedTranStats_t {
	LONG	structVersion;	/* version of this structure */
	ULONG	uncommittedTransactionCount; /* number of transactions read but not yet committed */
	ctLOGP	lastProcessedLogPosition; /* last log position whose entry log read processed */
	ctLOGP	oldestUncommittedPosition; /* lowest log position of transaction commit we have read but not yet applied */
} OLDEST_UNCOMMITTED_TRAN_STATS;

#define OLDEST_UNCOMMITTED_TRAN_STATS_VERS_V01	1 /* initial version of structure */
#define OLDEST_UNCOMMITTED_TRAN_STATS_VERS_CUR	OLDEST_UNCOMMITTED_TRAN_STATS_VERS_V01 /* current version of structure */
#define OLDEST_UNCOMMITTED_TRAN_STATS_SIZE_CUR	sizeof(OLDEST_UNCOMMITTED_TRAN_STATS) /* size of structure */

/* replication agent xml filter update options */
typedef enum replXmlUpdate_t {
	REPL_XML_UPDATE_NONE = 0x00000000, /* no updates */
	REPL_XML_UPDATE_FILES = 0x00000001, /* add to list of files */
	REPL_XML_UPDATE_PURPOSES = 0x00000002 /* set purposes to the specified values */
} REPL_XML_UPDATE;

/* modes used when shutting down the replication agent */
typedef enum replShutdownMode_t {
	REPL_SHUTDOWN_NONE = 0, /* not shutting down */
	REPL_SHUTDOWN_NORMAL = 1, /* shut down immediately */
	REPL_SHUTDOWN_SYNC = 2, /* sync shutdown: wait to apply all changes from logs */
	REPL_SHUTDOWN_SYNC_IF_MEMPHIS = 3 /* sync shutdown if using memphis availability group */
} REPL_SHUTDOWN_MODE;

/* transaction mode for replication agent local state file */
#define ctREPL_LOCAL_STATE_TRAN_MODE (ctPREIMG | ctWRITETHRU)

/* pbegpos value to pass to ctReplOpListDestroy() to indicate that caller is passing a pctOPLH in the pchgbuf parameter */
#define OP_LIST_DESTROY_PCHGBUF_IS_POPLH ((pctLOGP) 1)

/* Values for structVersion field in ctSTART_REPLAGENT_MANAGER_OPTIONS structure: */
typedef enum ctStartReplagentManagerOptionsVersion_t {
	ctSTART_REPLAGENT_MANAGER_OPTIONS_VERSION_1 = 1, /* initial version of structure */
	ctSTART_REPLAGENT_MANAGER_OPTIONS_VERSION_CURRENT = 1 /* current version of structure */
} ctSTART_REPLAGENT_MANAGER_OPTIONS_VERSION;

/* Replication agent manager run modes */
typedef enum ctReplagentManagerMode_t {
	ctREPLAGENT_MANAGER_MODE_EMBEDDED = 1, /* replagent manager is running in target server */
	ctREPLAGENT_MANAGER_MODE_STANDALONE = 2 /* replagent manager is running in its own process */
} ctREPLAGENT_MANAGER_MODE;

/* Options for the startReplagentManager() API function: */
typedef struct ctStartReplagentManagerOptions_t {
	ULONG	structVersion; /* the version of the structure */
	ULONG	mode; /* the agent mode */
} ctSTART_REPLAGENT_MANAGER_OPTIONS;

/* fixed length size of ctSTART_REPLAGENT_MANAGER_OPTIONS data */
#define ctSTART_REPLAGENT_MANAGER_OPTIONS_insize_V01 (sizeof(ULONG) + sizeof(ctUUID))

/* maximum size of a file group name */
#define MAX_FILE_GROUP_NAME_LENGTH IDZ

/* an entry in the replication agent file group list */
typedef struct replAgentFileGroupListEntry_t {
	TEXT	fileGroup[MAX_FILE_GROUP_NAME_LENGTH]; /* name of file group */
} REPLAGENT_FILE_GROUP_LIST_ENTRY;

#define REPLAGENT_ACTION_NONE 0 /* no action */
#define REPLAGENT_ACTION_RUNNING 1 /* replication action is running */
#define REPLAGENT_ACTION_SUCCEEDED 2 /* most recent replication action succeeded */
#define REPLAGENT_ACTION_FAILED 3 /* most recent replication action failed */

/* status information for a replication agent action */
typedef struct replagentActionStatus_t {
	LONG	errorCode; /* the error code */
	LONG	actionType; /* the type of action */
	ULONG	requestId; /* unique id for the action */
	TEXT	status; /* current status of the action */
	TEXT	percentComplete; /* percentage complete */
	TEXT	errorMessage[512]; /* error message */
} REPLAGENT_ACTION_STATUS;

/* An entry in the file group ID list */
typedef struct fileGroupIdListEntry_t {
	ULONG	fileGroupId; /* unique id for the file group filter	*/
	TEXT	fileGroupName[MAX_NAME]; /* name of this file group filter */
} FILE_GROUP_ID_LIST_ENTRY;

/* Parameters for the ctReplGetNextChangeDirect() call with operation type REPL_GET_NEXT_CHANGE_DIRECT_OPERATION_GET_FILE_GROUP_IDS */
typedef struct getFileGroupFilterParams_t {
	FILE_GROUP_ID_LIST_ENTRY* fileGroupIdList;
	NINT	fileGroupIdCount;
	void*	(*allocate)(LONG); /* function pointer for allocating memory, matching prototype of ctReplAllocMem() */
	void	(*free)(void*); /* function pointer for freeing memory, matching prototype of ctReplFreeMem() */
} GET_FILE_GROUP_FILTER_PARAMS;

/* An entry in the file group ID log state map */
typedef struct fileGroupIdLogStateMapEntry_t {
	ctLOGP	lastCommitLogPosition;
	ctLOGP	readLogPosition;
} FILE_GROUP_ID_LOG_STATE_MAP_ENTRY;

/* transaction log position information for a file group */
typedef struct fileGroupLogPosInfo_t {
	ctLOGP	lastCommitLogPosition;
	ctLOGP	readLogPosition;
} FILE_GROUP_LOG_POS_INFO;

#define LOGSHIP_DEBUG_LOGID LOGID_2959

#ifdef ctFeatREPL_EXCEPCB
extern ctCONVr NINT         ctReplAgentSetExcepCB(pEXCEPCBFNC excepCB, pVOID conextCB, pctRASTT prepstt);
#endif
extern NINT logship_thread(pVOID params);
extern NINT logread_thread(pVOID params);
extern NINT analyzer_thread(pVOID params);
extern NINT dependency_thread(pVOID params);
extern NINT apply_thread(pVOID params);
extern NINT ctReplAgentConnectToServer(NINT srvtyp, RACOPT options, NINT prevrc, pNINT pconnerr, pctRASTT prepstt);
extern NINT ctRAnotifyevent(pctRASTT prepstt,REPEVT replevent,NINT errcod);
#ifdef ctFeatREPLAGENT_SHARE_LOGSHIP
extern NINT updateLogShipState(pctRASTT prepstt, FILE* logShipMinLogStateFile, const ctLOGP* lastLogReadPosition);
#else
extern NINT readLocalState(const ctRASTT* prepstt, pctLOGP plastread, char* errbuf, size_t errbuflen);
extern NINT clearLocalState(const ctRASTT* prepstt, char* errbuf, size_t errbuflen);
extern NINT updateLocalState(pctRASTT prepstt, pctLOGP plastread, char* errbuf, size_t errbuflen);
extern NINT openLogShipStateTable(pctRASTT prepstt);
extern void closeLogShipStateTable(pctRASTT prepstt);
#endif
extern NINT chklostconn(NINT rc);
extern NINT GetTranStats(pctLOGP plogpos, pLONG pminlog, pctRASTT prepstt, pctGTMS pgtmsbuf);
extern NINT connectToLocalServer(pctRASTT prepstt);
extern void disconnectFromLocalServer(void);
extern NINT copyReplState(pctRASTT prepsttIn,AGENT_THREAD_TYPE threadType,pctRASTT prepsttOut);
extern NINT getLocalLogDirectory(pctRASTT prepstt,TEXT withLocalDir);
extern NINT ctRAinitLogReader(pctRASTT prepstt);
extern void ctRAtermLogReader(pctRASTT prepstt);
extern NINT ctRArun(pctRASTT prepstt);
extern NINT ctRAExceptionLogInit(pctRASTT prepstt);
extern NINT replEnterThread(pctRASTT prepstt,cpTEXT threadDescription);
extern NINT replExitThread(pctRASTT prepstt,NINT rc);
extern NINT ctRAapply(pctRASTT prepstt,pctOPLH poplhed,pNINT pLostSourceConnection,pNINT pLostTargetConnection,pTEXT pBreakout);
extern NINT ctReplTranFileListCreate(pctOPLH poplhed);
extern void ctReplTranFileListDestroy(pctOPLH poplhed);
extern void ctReplTranFileListAdd(pctOPLH poplhed, pctFILH pfilhnd);
extern NINT ctReplTranFileListGet(pctOPLH poplhed, ULONG dattfl, ppctFILH ppfilhnd);
extern NINT ctReplTranFileNameListCreate(pctOPLH poplhed);
extern void ctReplTranFileNameListDestroy(pctOPLH poplhed);
extern NINT copyReplFileHandle(pctOPLH poplhed, const ctFILH *pfilhnd, pctFILH *ppFileHandleCopy);
extern NINT replAgentCloseFile(pctRASTT prepstt,pctFILH pfilhnd,NINT opmode,pNINT pbreakout,pNINT pLostTargetConnection);
extern NINT setTranStateListEntryState(pctRASTT prepstt,LONG8 transactionNumber,TRAN_STATE state);
extern NINT removeTransactionFromTranStateList(pctRASTT prepstt,LONG8 transactionNumber);
extern NINT getOldestTransactionInTranStateList(pctRASTT prepstt,pTRAN_STATE_LIST_ENTRY pResult);
extern NINT initTranWritesets(pctRASTT prepstt, pctOPLH pOperationList);
extern void destroyTranWritesets(pctRASTT prepstt, pctOPLH pOperationList);
extern NINT findWritesetEntry(pctRASTT prepstt, pWRITESET_LIST pWritesetList, pWRITESET_SEARCH_REQUEST pWritesetSearchRequest, ppVOID ppWritesetEntry);
extern NINT initWriteset(pctRASTT prepstt, pWRITESET_LIST pWriteset,WRITESET_TYPE writesetType);
extern void destroyWriteset(pctRASTT prepstt, pWRITESET_LIST pWriteset);
extern NINT initDependencyGraph(pctRASTT prepstt);
extern void destroyDependencyGraph(pctRASTT prepstt);
extern NINT removeEntryFromDependencyGraph(pctRASTT prepstt, pANALYZER_QUEUE_MESSAGE pAnalyzerQueueMessage);
extern NINT compareTableWritesetEntry(struct writesetSearchRequest_t *pWritesetSearchRequest,pVOID pWritesetEntry);
extern NINT compareIndexWritesetEntry(struct writesetSearchRequest_t *pWritesetSearchRequest,pVOID pWritesetEntry);
extern NINT compareResourceWritesetEntry(struct writesetSearchRequest_t *pWritesetSearchRequest,pVOID pWritesetEntry);
extern NINT checkTableWritesetOverlap(pctRASTT prepstt, pWRITESET_LIST pTableWritesetA, pWRITESET_LIST pTableWritesetB, pTEXT pOverlap);
extern NINT initAuthFileMutex(void);
extern void destroyAuthFileMutex(void);
extern NINT replSetCurrentLogShipPosition(pctRASTT prepstt, const ctLOGP *pCurrentLogPosition);
extern NINT replGetCurrentLogShipPosition(pctRASTT prepstt, pctLOGP pCurrentLogPosition);
extern NINT replGetCurrentLogReadPosition(pctRASTT prepstt, pctLOGP pCurrentLogPosition, pctLOGP pReadLogPosition, pctLOGP pLastCommitPosition);
extern void replPauseThread(pctRASTT prepstt);
extern NINT ctRAreconnect(NINT srvtyp, pctLOGP plogpos, pctRASTT prepstt, NINT erc);
extern void replTraceLog(pctRASTT prepstt, cpTEXT fmt, ...) PRINTFLIKE(2,3);
extern void replSetThreadDescription(pctRASTT prepstt,cpTEXT description,NINT threadInstance);
extern NINT openTranStateIndex(pctRASTT prepstt);
extern VOID ctRActapicbMST(NINT mode, NINT func, pVOID pctgvar);
extern NINT getTaskidForConnection(pctRASTT prepstt,pNINT pTaskId);
extern NINT tranReadyQueueWrite(pctRASTT prepstt, pANALYZER_QUEUE_MESSAGE pAnalyzerQueueMessage, NINT messageLength);
extern void replThrottleThread(pctRASTT prepstt,NINT sleepMsec);
extern NINT logShipWaitForTransactionLogFlush(pctCNXH pcnxhnd,LONG lastOffsetWritten,NINT timeoutMsec);
extern void clearExclusiveOpenList(pctCNXH pcnxhnd, TEXT closeFiles);
extern NINT ctReplCloseFileNameListCreate(pCLOSE_FILES_BEFORE_TRAN_LIST pCloseFilesBeforeTranList);
extern NINT ctReplCloseFileNameListAdd(pctRASTT prepstt, pCLOSE_FILES_BEFORE_TRAN_LIST pCloseFilesBeforeTranList, cpTEXT fileName, ULONG fileId);
extern NINT ctReplCloseFileNameListGet(pCLOSE_FILES_BEFORE_TRAN_LIST pCloseFilesBeforeTranList, cpTEXT fileName, pCLOSE_FILES_BEFORE_TRAN_LIST_ENTRY *ppListEntry);
extern NINT ctReplCloseFileNameListRemove(pctRASTT prepstt, pCLOSE_FILES_BEFORE_TRAN_LIST pCloseFilesBeforeTranList, cpTEXT fileName);
extern NINT ctReplCloseFileNameListGetNext(pCLOSE_FILES_BEFORE_TRAN_LIST pCloseFilesBeforeTranList, pCLOSE_FILES_BEFORE_TRAN_LIST_SEARCH pSearchResult);
extern void ctReplCloseFileNameListDestroy(pCLOSE_FILES_BEFORE_TRAN_LIST pCloseFilesBeforeTranList);
extern void analyzerConvertFileName(cpTEXT fileName, TEXT outputBuffer[MAX_NAME]);
extern NINT superfileHostListCreate(pSUPERFILE_HOST_LIST pSuperfileHostList);
extern void superfileHostListDestroy(pSUPERFILE_HOST_LIST pSuperfileHostList);
extern NINT superfileHostListAdd(pSUPERFILE_HOST_LIST pSuperfileHostList, cpTEXT fileName, FILNO fileNumber, ULONG fileId, pSUPERFILE_HOST_LIST_ENTRY *ppNewListEntry);
extern NINT superfileHostListRemoveByFileName(pctCNXH pcnxhnd, pSUPERFILE_HOST_LIST pSuperfileHostList, cpTEXT fileName);
extern NINT superfileHostListRemoveByFileNumber(pctCNXH pcnxhnd, pSUPERFILE_HOST_LIST pSuperfileHostList, FILNO fileNumber);
extern NINT superfileHostListFindByFileNumber(pSUPERFILE_HOST_LIST pSuperfileHostList, FILNO fileNumber, pSUPERFILE_HOST_LIST_ENTRY *ppListEntry);
extern NINT superfileHostListFindByName(pSUPERFILE_HOST_LIST pSuperfileHostList, cpTEXT fileName, pSUPERFILE_HOST_LIST_ENTRY *ppListEntry);
extern NINT ctRAFilterChangeInit(pctRASTT prepstt, FILNO *ffchgdat);
extern NINT ctRAFilterChangeApply(pctRASTT prepstt, cpTEXT agentId, pTEXT errorMsg, int errorMsgLen);
extern void signalShutdown(pctRASTT prepstt);
extern NINT openReplAgentLog(pctRASTT prepstt);
extern void closeReplAgentLog(pctRASTT prepstt);
extern NINT ctRAlogmsg(pctRASTT prepstt,NINT id,NINT error, NINT msgtyp, cpTEXT fmt, ...) PRINTFLIKE(5,6);
extern NINT ctReplAgentSetState(NINT mode, pctRASTT prepstt);
extern NINT unblockTranReadyQueueReaders(pctRASTT prepstt);
extern void calculateReplicationLatency(pctRASTT prepstt, LONG* latency);
extern NINT addLogReaderToLogShipThreadList(pctRASTT prepstt);
extern NINT getLogReadMinimumLogRequirement(pctRASTT prepstt, LONG *minlog, FILE *logShipMinLogStateFile);
extern NINT ctReplArrayAlloc(pNINT ptot, pNINT pcur, ppVOID anchor, NINT objsiz, NINT incr);
extern void ctReplArrayFree(pVOID anchor);
extern void removeUnneededLogShipFiles(pctRASTT prepstt);
extern void closeLogShipThreadStateFile(pctRASTT prepstt);
extern ctCONVr NINT startReplagentManager(const ctSTART_REPLAGENT_MANAGER_OPTIONS* options, char* errorBuffer, size_t errorBufferSize);
extern ctCONVr void stopReplagentManager(void);
extern void setReplagentStatusMessage(pctRASTT prepstt, cpTEXT message);
extern void getReplagentStatusMessage(pctRASTT prepstt, pTEXT outputBuffer, size_t outputLength);
extern NINT replGetLogReadStartPosition(pctRASTT prepstt, ctLOGP* initialLogReadPosition, LONG* initialMinimumLogNumber);
extern NINT ctReplFileListCreate(pctCNXH pcnxhnd);
extern void ctReplFileListDestroy(pctCNXH pcnxhnd);
extern NINT ctReplFileListGet(pctCNXH pcnxhnd, ULONG dattfl, ppctFILH pfilhnd);
extern void ctReplFileListPrint(pctCNXH pcnxhnd);
extern NINT ctReplOpListDestroy(pctCNXH pcnxhnd, pctCHGB pchgbuf, pctLOGP pbegpos);
extern void ctReplAgentSetShutdownFlag(ctRASTT* prepstt, REPL_SHUTDOWN_MODE value, REPL_SHUTDOWN_MODE* previousValue);
extern void ctReplAgentClearShutdownFlag(ctRASTT* prepstt, REPL_SHUTDOWN_MODE* previousValue);
#ifdef __cplusplus
extern "C" {
#endif
extern void* fileGroupListCreate(void);
extern int fileGroupListInsert(void* fileGroupList, const char* fileGroup);
extern void fileGroupListDestroy(void* fileGroupList);
extern int fileGroupListGetAll(void* fileGroupList, char*** fileGroupArray, size_t* fileGroupCount, void* (*allocator)(size_t));
extern void* fileGroupIdLogStateMapCreate(size_t elementSize);
extern int fileGroupIdLogStateMapInsert(void* fileGroupIdLogStateMap, unsigned long fileGroupId, const void* fileGroupIdLogStateMapEntry, size_t fileGroupIdLogStateMapEntryLength);
extern void* fileGroupIdLogStateMapGet(const void* fileGroupIdLogStateMap, unsigned long fileGroupId);
extern void fileGroupIdLogStateMapRemoveByCondition(void* fileGroupIdLogStateMap, int (*func)(const void*, const void*), const void* context);
extern size_t fileGroupIdLogStateMapGetSize(const void* fileGroupIdLogStateMap);
extern void fileGroupIdLogStateMapDestroy(void* fileGroupIdLogStateMap);
#ifdef __cplusplus
}
#endif

/* non-zero if the replication agent is unmanaged */
#define REPLAGENT_IS_UNMANAGED(prepstt) (prepstt->global->isEmbedded && !(prepstt->srepflags & ctRAFLGreplManager))

#endif /* ~ctREPLAGENTH */

/* end of ctreplagent.h */
