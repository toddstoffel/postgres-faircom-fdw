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

#ifndef _CTCBTYPE_H_
#define _CTCBTYPE_H_

#include "ctcboptn.h"
#include "ctcberr.h"
#include "ctcbconf.h"

/* Increase CT_COMPATIBILITY_ID when changing INPUT/OUTPUT structures */
#include "ctcbcomp.h"

#include "ctvers.h"
#define CT_VERSION CT_GLOBAL_DISPLAY "-" CT_GLOBAL_BUILD

#define SETFALSE(n,b) { if ( (n) & (b) ) n ^= ( (n) & (b) ) ; }

#ifdef ctFeatRTG_CBPTR
#if defined(CBPTR32) || !defined(ctN8)
/* compile time option was specified to use 32-bit CBPTR */
typedef LONG CBPTR;

#ifdef ct8P
#error Compiling the COBOL driver without native 64-bit integer support is only supported for 32-bit compiles not 64-bit compiles
#endif

#else
typedef LONG8 CBPTR;
#endif
#endif /* ctFeatRTG_CBPTR */


#define CT_INIT_PARMS                4
#define CT_MAX_BUFS            (0x7FFF) /* maximum signed 2-bytes integer value */
#define CT_MAX_FILS            (0x7FFF) /* maximum signed 2-bytes integer value */
#define CT_MAX_SECT       (0x10000/128) /* maximum unsigned 2-bytes integer value divided by 128 */

#define CT_MAX_KEYS               (128)
#ifdef  ctPortHBRTG
#define CT_MAX_SEGS                (72)
#else
#define CT_MAX_SEGS                (16)
#endif
#define CT_MAX_KEY_SIZE         MAXLEN
#define CT_MAX_RECORD_SIZE     (0xFFFF)

/* the condition below must match the opposite condition that #undef ctFeatIICT in ctopt2.h */
#if (defined(MULTITRD) && defined(ctCACHE_MEMORY) && defined(ctDYNHASH) && defined(ctBEHAV_USRSEMA) && defined(TRANPROC) && defined(ctMULTIOPN) && defined(CTS_ISAM))
#ifndef ctFeatIICT
#define TRANSACTION_INSTANCE
#endif
#endif /* ~MULTITRD */

#ifdef TRANSACTION_INSTANCE
#ifdef MULTITRD
#error TRANSACTION_INSTANCE not supported in MULTITRD environments because \
the function ctThrdHandle() returns the instance ID instead of the thread ID. \
Therefore only one instance per thread can be used in MULTITRD environments.
#endif
#if defined(CTBOUND) && defined(NOTFORCE) && defined(TRANPROC)
#error TRANSACTION_INSTANCE not supported in standalone TRANPROC environments \
because multiple INTISAM() create transaction logs which cannot coexist. \
Therefore only one instance can be used in standalone TRANPROC environments.
#endif
#define CT_MAX_USER_INSTANCES       ctFeatCOBOL_MAX_INSTANCES
#define CT_MAX_INSTANCES            CT_MAX_USER_INSTANCES * 2
#define USER_INSTANCE_NBR(i)        ((i<CT_MAX_USER_INSTANCES)?i:i-CT_MAX_USER_INSTANCES)
#else
#ifdef MULTITRD
/*
 * In MULTITRD environments the function ctGetThrdId() returns an instance ID
 * rather than a real thread ID. All the multi-thread logic in COBOL interface
 * relies on ctThrdHandle() to return a unique thread ID per system thread.
 * Therefore only one instance per thread can be used in MULTITRD environments.
 */
#define CT_MAX_INSTANCES             1
#else
#define CT_MAX_INSTANCES             ctFeatCOBOL_MAX_INSTANCES
#endif
#define USER_INSTANCE_NBR(i)         i
#endif

#define MAX_THREADS                 (MXU2 > CT_MAX_INSTANCES ? MXU2 : CT_MAX_INSTANCES)

#ifdef ctHUGEFILE
#define CT_KEY_DUPLICATE_PLUS        (2*ctSIZE(ctRECPT))
#define CT_KEY_DUPLICATE_PLUS_NOHUGE (ctSIZE(ctRECPT))
#else
#define CT_KEY_DUPLICATE_PLUS        4
#endif

/*
** keytype values
*/
#define CT_KEY_DUPLICATES       0x00000001
#define CT_KEY_REPEATDUPS      (0x00000002|CT_KEY_DUPLICATES)
#define CT_KEY_NOT_MODIFIABLE   0x00000004
#define CT_KEY_COMPRESS_LEADING 0x00000010
#define CT_KEY_COMPRESS_PADDING 0x00000020
#define CT_KEY_NULLKEY_ANYSEGS  0x00000040
#define CT_KEY_NULLKEY_CHKNUL   0x00000080
#define CT_KEY_COMPRESS_VLENNOD 0x00000100
#define CT_KEY_COMPRESS_RLE     0x00000200
#define CT_KEY_NULLKEY_ALLSEGS  0x00000400

#define CT_KEY_DEFINITIONS (CT_KEY_DUPLICATES|CT_KEY_NOT_MODIFIABLE|CT_KEY_NULLKEY_CHKNUL|CT_KEY_NULLKEY_ANYSEGS|CT_KEY_NULLKEY_ALLSEGS)


/* mutually exclusive sequential operations */
#define CT_KEY_SEQ_FIRST        0x00001000
#define CT_KEY_SEQ_NEXT         0x00002000
#define CT_KEY_SEQ_PREV         0x00004000
#define CT_KEY_SEQ_LAST         0x00008000

/* mutually exclusive start operations */
#define CT_KEY_START_EQ         0x00010000
#define CT_KEY_START_GT         0x00020000
#define CT_KEY_START_GTE        0x00040000
#define CT_KEY_START_LT         0x00080000
#define CT_KEY_START_LTE        0x00100000
#define CT_KEY_START_CURRENT    0x00200000
#define CT_KEY_START_ATPERCENT  0x00400000
#define CT_KEY_START_GETPERCENT 0x00410000
#define CT_KEY_START_OPS        (CT_KEY_START_EQ|CT_KEY_START_GT|CT_KEY_START_GTE|CT_KEY_START_LT|CT_KEY_START_LTE|CT_KEY_START_ATPERCENT|CT_KEY_START_GETPERCENT)
/* start options */
#define CT_KEY_START_WITHSIZE   0x00800000

/* mutually exclusive update operations */
#define CT_UPD_REWRITE          0x01000000
#define CT_UPD_DELETE           0x02000000
/* mutually exclusive insert operations */
#define CT_ADD_RET_RECNUM       0x01000000
#define CT_ADD_RET_RECOFF       0x02000000
#define CT_ADD_RETPOS           (CT_ADD_RET_RECNUM|CT_ADD_RET_RECOFF)
/* common options */
#define CT_ISAM_PTR_UPD         0x04000000
#define CT_KEY_REL_ADDR         0x10000000
#define CT_ADD_RET_RECDAT       0x20000000

/* mutually exclusive sqllink operations */
#define CT_SQLLINK_LINK         0x00
#define CT_SQLLINK_UNLINK       0x01
/* sqllink options */
#define CT_SQLLINK_SKIPMISSING  0x40
#define CT_SQLLINK_REFRESH      0x80

/*
** keyno values
*/
#define CT_KEY_DATASCAN         0x1000 /* assigned to: keyno = CT_KEY_DATASCAN */
#define CT_KEY_STEP             0x2000 /* added to: keyno |= CT_KEY_STEP */
#define CT_KEY_SRLIDX           0x4000 /* added to: keyno |= CT_KEY_SRLIDX */
#if (CT_KEY_DATASCAN < CT_MAX_KEYS) || (CT_KEY_STEP < CT_MAX_KEYS) || (CT_KEY_SRLIDX < CT_MAX_KEYS)
#error Since the CT_KEY_DATASCAN|CT_KEY_STEP|CT_KEY_SRLIDX macros are assigned to keyno \
parameter, they must be set to a greater number than the maximum number of \
indices per data file to avoid conflicts with valid keyno values.
#endif

/* start operations */
typedef enum {
	CTS_INVALID_KEY = 0,
	CTS_EQ_KEY = 1,
	CTS_GT_KEY = 2,
	CTS_GTE_KEY = 3,
	CTS_LT_KEY = 4,
	CTS_LTE_KEY = 5,
	CTS_CURRENT_KEY = 6,
	CTS_ATPERCENT_KEY = 7,
	CTS_GETPERCENT_KEY = 8,
	CTS_DATASCAN = 1024
} STARTOP;

/* sequential operations */
typedef enum {
	CTS_NEXT_RECORD = 1,
	CTS_PREV_RECORD = 2,
	CTS_FIRST_RECORD = 3,
	CTS_LAST_RECORD = 4,
	CTS_NEXT_OR_FIRST_RECORD = 5,
	CTS_PREV_OR_LAST_RECORD = 6
} SEQOP;

typedef struct {
	COUNT	off;
	COUNT	len;
} CT_SEG1, *pCT_SEG1;
typedef struct {
	UCOUNT	off;
	UCOUNT	len;
	UTEXT	typ;
	UTEXT	mod;
} CT_SEG2, *pCT_SEG2;

typedef struct {
	LONG	type;
	COUNT	nsegs;
	CT_SEG1	seg[CT_MAX_SEGS];
} CT_KEY1, *pCT_KEY1;
typedef struct {
	LONG	type;
	UCOUNT	nsegs;
	CT_SEG2	seg[CT_MAX_SEGS];
	UTEXT	nulval;
} CT_KEY2, *pCT_KEY2;

typedef struct {
	pVOID file;
	TEXT used;
} CT_FILEHASH;

typedef struct {
	UCOUNT size;
	UCOUNT inuse;
	UCOUNT freesize;
	CT_FILEHASH *hash;
} CT_FILEPOOL;

typedef struct tagRTGMEM {
	pTEXT bufptr;
	ULONG bufsiz;
	TEXT inuse;
	struct tagRTGMEM *next;
} RTGMEM, * pRTGMEM;

CTHANDLE rtgGetHandle(CTHANDLE handle, HANDLEID handleid);

typedef struct tagCT_INIT CT_INIT, * pCT_INIT, ** ppCT_INIT;
typedef struct tagCT_INSTANCE CT_INSTANCE, * pCT_INSTANCE, ** ppCT_INSTANCE;
typedef struct tagCT_FILE CT_FILE, * pCT_FILE, ** ppCT_FILE;

struct tagCT_INSTANCE {
	HANDLEID	handleid;
	pCT_INIT conn;
	COUNT	init[CT_INIT_PARMS];
	TEXT	server_name[CT_SERV_LEN];
	TEXT	server_user[CT_USER_LEN];
	TEXT	server_word[CT_PASS_LEN];
	COUNT	server_prof;
	TEXT	nodename[CT_NODE_LEN];
	TEXT	id[32];
	TEXT	ssl;
	pTEXT	sslcert;
	COUNT	num;
	COUNT	status;
	COUNT	transaction;
	COUNT	trlevel;
	LONG	trnum;
	LONG	trnumh;
	TEXT	multiopn_difusr;
	TEXT	iict;
	TEXT	copyfile;
	TEXT	mtclient;
	TEXT	xfrfile;
	TEXT	byteswap;
	TEXT	pathsep;
	TEXT	do_checksum;
	COUNT	keycompress;
	TEXT	isSQL;
	TEXT	version[CT_VERSION_LEN];
	LONG	compatid;
	ULONG	structver;
	LONG	logons;
	ULONG8	trncnt;
	pCONFINST	conf;
	pTEXT	shmemdir;
	LONG	maxdkey;
	LONG	maxkseg;
	TEXT	reconnect;
	ULONG	reconnected;
	UCOUNT	socktimeout;
#ifdef ctFeatRTG_FAILOVER_CLUSTER
	pCONFCLUS	cluster;
#else /* ~ctFeatRTG_FAILOVER_CLUSTER */
	TEXT	cluster;
	UCOUNT	failovertimeout;
#endif /* ~ctFeatRTG_FAILOVER_CLUSTER */
#ifdef ctFeatRTG_KEYS
	pRTGKEY	keys;
#endif /* ctFeatRTG_KEYS */
#ifdef ctFeatRTG_TASKID
	RTGTASKID	taskid;
#endif
#ifdef ctFeatRTG_FEATMASK
	ULONG	featmask;
#endif
};

struct tagCT_INIT {
	HANDLEID	handleid;
	CT_ERROR	err;
	TEXT	fname[CT_PATH_LEN];
	TEXT	oname[CT_PATH_LEN];
	TEXT	rname[CT_PATH_LEN];
	TEXT	basename[CT_PATH_LEN];
	TEXT	dirname[CT_PATH_LEN];
	TEXT	envstring[CT_PATH_LEN];
	VOID	*avail;
	COUNT	n_instance;
	COUNT	a_instance;
	COUNT	options;
	COUNT	in_transaction;
	TEXT	exclusive_tran;
#ifdef ctFeatRTG_TWOPHASE_COMMIT
	TEXT	prepared_tran;
#endif
	TEXT	is_ctutil;
	pRTGCONF	conf;
	INTFTYPE	intf;
	LONG	locker_taskid;
	CT_FILEPOOL	*filepool;
	CT_INSTANCE	instance[CT_MAX_INSTANCES];
	TEXT	errmsg[F_ERRMSG_LEN];
	pTEXT	nodename;
#ifdef ctFeatRTG_SESSIONMEM
	RTGMEM	memin;
	RTGMEM	memout;
#endif
};

typedef struct {
	CT_INIT *conn;
	CT_ERROR _err;
	UTEXT cliID[4];
	pCONFFILE currfile;
	ULONG curropen;
} RTGTHRD, *pRTGTHRD, **ppRTGTHRD;

typedef struct {
	INTFTYPE intf;
	TEXT is_ctutil;
} RTGENV, *pRTGENV;

#ifndef TRUE
#define TRUE 1
#endif

#ifndef FALSE
#define FALSE 0
#endif

#define CT_EXTENSION_SIZE    0

/*
**	OPEN command - open a file
*/

/* drvfilemod2 at the moment used only by MAKE */
#define CT_OPN2_RESERVED            0x00000001

#define CT_OPN2_NO_RUNIT_EXCLUSIVE  0x00000002
#define CT_OPN2_NO_RUNIT_UNLOCKALL  0x00000004
#define CT_OPN2_ALLOW_NOPASS_OPEN   0x00000008
#define CT_OPN2_RETRYOPEN           0x00000010

#define CT_MAKE_CTCOMPRESSREC       0x00400000
#define CT_MAKE_CTAUGMENTEDFXD      0x00800000
#define CT_MAKE_CTAUTOMKDIR         0x01000000
#define CT_MAKE_MEMFILE_NOPERSIST   0x02000000
#define CT_MAKE_TRUNCATEIFEXIST     0x04000000
#define CT_MAKE_CTFLEXREC           0x08000000
#define CT_MAKE_KEYNUMBER           0x10000000
#define CT_MAKE_TEMPFILE            0x40000000

/* drvfilemode */
#define CT_OPEN_FILE_SHARED         0x00000000
#define CT_OPEN_FILE_INPUT          0x00000001
#define CT_OPEN_FILE_IO             0x00000002
#define CT_OPEN_FILE_OUTPUT         0x00000004
#define CT_OPEN_FILE_EXTEND         0x00000008
#define CT_OPEN_FILE_NOREADERS      0x00000010
#define CT_OPEN_FILE_NOWRITERS      0x00000020
#define CT_OPEN_FILE_MASSUPDATE     0x00000040
#define CT_OPEN_FILE_BULKADDITION   0x00000080
#define CT_OPEN_FILE_MULTILOCK      0x00000100
#define CT_OPEN_WITH_ROLLBACK       0x00000200
#define CT_OPEN_NO_KEYCHECK         0x00000400
#define CT_OPEN_MAXLEN_CHECK        0x00000800
#define CT_OPEN_SMARTCOPY_INPUT     0x00001000
#define CT_OPEN_SMARTCOPY_OUTPUT    0x00002000
#define CT_OPEN_SMARTCOPY          (CT_OPEN_SMARTCOPY_INPUT | CT_OPEN_SMARTCOPY_OUTPUT)
#define CT_OPEN_SMARTCOPY_RPC       0x00004000
#define CT_OPEN_SMARTCOPY_RAW       0x00008000
#define CT_OPEN_VARLEN              0x00010000
#define CT_OPEN_PREIMG              0x00020000
#define CT_OPEN_TRNLOG             (0x00040000 | CT_OPEN_PREIMG)
#define CT_OPEN_WRITETHRU           0x00080000
#define CT_OPEN_FILE_ALWAYS         0x00100000
#define CT_OPEN_DATAONLY            0x00200000
#define CT_OPEN_DUPSCANWA           0x00400000 /* DUPSCANWA active on the file */
#define CT_OPEN_SCANCACHE           0x00800000
#define CT_OPEN_FILE_FOR_INFO       0x01000000
#define CT_OPEN_TRANDEP             0x02000000
#define CT_OPEN_OPTIMISTICADD       0x04000000
#define CT_OPEN_NO_RUNITLOCKDETECT  0x08000000
#define CT_OPEN_NO_RUNIT_ANYUNLOCK  0x10000000
#define CT_OPEN_NO_OVERWRITE        0x20000000
#define CT_OPEN_SRLIDX              0x40000000
#define CT_OPEN_NO_READFIL          0x80000000

/* maintmode */
#define CT_OPEN_FILE_FOR_REBUILD    0x00000001
#define CT_OPEN_FILE_FOR_COMPACT    0x00000002
#define CT_OPEN_FILE_FOR_VERIFY     0x00000004
#define CT_OPEN_FILE_REPAIRDATA     0x00000008
#define CT_OPEN_FILE_PURGEDUPS      0x00000010
#define CT_OPEN_FILE_DELETEIDX      0x00000020
#define CT_OPEN_FILE_COMPRESS       0x00000040
#define CT_OPEN_FILE_UNCOMPRESS     0x00000080
#define CT_SET_FILE_SH              0x00000100
#define CT_SET_FILE_PI              0x00000200
#define CT_SET_FILE_TN              0x00000400
#define CT_SET_FILE_WT              0x00000800
#define CT_SET_FILE_MODES          (CT_SET_FILE_PI|CT_SET_FILE_TN|CT_SET_FILE_WT|CT_SET_FILE_SH)
#define CT_SET_FILE_SEG             0x00001000
#define CT_SET_FILE_RTGI            0x00002000
#define CT_SET_FILE_FILEID          0x00004000
#define CT_CHECK_FILE_DEFS          0x00008000
#define CT_OPEN_FILE_FOR_TRUNCATE   0x00010000
#define CT_SET_FILE_MAXSIZE         0x00020000
#define CT_SET_FILE_ATTR            0x00040000
#define CT_SET_RECORD_MAXLEN        0x00080000

#define CT_OPEN_OPERATIONS         (CT_OPEN_FILE_FOR_REBUILD|CT_OPEN_FILE_FOR_COMPACT|CT_OPEN_FILE_FOR_VERIFY|\
                                    CT_OPEN_FILE_COMPRESS|CT_OPEN_FILE_UNCOMPRESS|\
                                    CT_SET_FILE_MODES|CT_SET_FILE_SEG|CT_SET_FILE_RTGI|CT_SET_FILE_FILEID|\
                                    CT_CHECK_FILE_DEFS|CT_SET_FILE_MAXSIZE|CT_SET_FILE_ATTR|CT_SET_RECORD_MAXLEN)

typedef struct {
	/* place new 8-byte long members above this line */
	LONG	mode;
	LONG	operation;
	ULONG	minlen;
	ULONG	maxlen;
	ULONG	segsiz;
	LONG	segmax;
	LONG	blockfact;
	LONG	prealloc;
	LONG	extension;
	/* place new 4-byte long members above this line */
	CT_KEY1	key[CT_MAX_KEYS];
	UCOUNT	nkeys;
	UCOUNT	retrycount; /* blocking lock timeout in seconds */
	/* place new 2-byte long members above this line */
	TEXT	signconv;
	TEXT	fileformat;
	TEXT	compress;
	TEXT	encrypt;
	TEXT	name[CT_PATH_LEN];
	TEXT	comment[CT_COMMENT_LEN];
	TEXT	trans[256];
} CT_OPEN1, *pCT_OPEN1;
typedef struct {
	/* place new 8-byte long members above this line */
	LONG	mode;
	LONG	operation;
	ULONG	minlen;
	ULONG	maxlen;
	ULONG	segsiz;
	LONG	segmax;
	LONG	blockfact;
	LONG	prealloc;
	LONG	extension;
	/* place new 4-byte long members above this line */
	CT_KEY2	key[CT_MAX_KEYS];
	UCOUNT	nkeys;
	UCOUNT	retrycount; /* blocking lock timeout in seconds */
	/* place new 2-byte long members above this line */
	TEXT	signconv;
	TEXT	fileformat;
	TEXT	compress;
	TEXT	encrypt;
	TEXT	name[CT_PATH_LEN];
	TEXT	comment[CT_COMMENT_LEN];
	TEXT	trans[256];
} CT_OPEN2, *pCT_OPEN2;
typedef struct {
	/* place new 8-byte long members above this line */
	LONG	pad4[4];
	LONG	mode2;
	LONG	mxfilz;
	LONG	mxfilzh;
	LONG	mode;
	LONG	operation;
	ULONG	minlen;
	ULONG	maxlen;
	ULONG	segsiz;
	LONG	segmax;
	LONG	blockfact;
	LONG	prealloc;
	LONG	extension;
	/* place new 4-byte long members above this line */
	COUNT	pad2[8];
	UCOUNT	nkeys;
	UCOUNT	retrycount; /* blocking lock timeout in seconds */
	/* place new 2-byte long members above this line */
	TEXT	pad1[9];
	TEXT	signconv;
	TEXT	fileformat;
	TEXT	compress;
	TEXT	encrypt;
	TEXT	name[CT_PATH_LEN];
	TEXT	comment[CT_COMMENT_LEN];
	TEXT	trans[256];
} CT_OPEN3, *pCT_OPEN3;

/*
**	MAKE command - create a file
*/

#define CT_MAKE_REBUILD 1

/* the following control configurable behavior and should be reset everytime on env2 */
#define CT2_WRITE_RETURN_RECOFF    0x00000001
#define CT2_WRITE_RETURN_RECNUM    0x00000002
#define CT2_WRITE_RETURN_RECPOS    (CT2_WRITE_RETURN_RECOFF | CT2_WRITE_RETURN_RECNUM)
#define CT2_KEEP_POS_ON_ERR        0x00000004
#define CT2_KEEP_POS_AT_END        0x00000008
#define CT2_POS_ON_NEXT_PREV       0x00000010
#define CT2_NEXT_EXECUTED          0x00000020
#define CT2_PREV_EXECUTED          0x00000040
#define CT2_DELCURRENT             0x00000100
#define CT2_DEFERAUTOTRAN          0x00000200
#define CT2_WRITE_EXECUTED         0x00001000
#define CT2_REWRITE_EXECUTED       0x00002000
#define CT2_DELETE_EXECUTED        0x00004000
#define CT2_UPDATE_EXECUTED        (CT2_WRITE_EXECUTED | CT2_REWRITE_EXECUTED | CT2_DELETE_EXECUTED)
#define CT2_FILEPOOL_REOPEN        0x00008000
#define CT2_WRITE_RETURN_RECDAT    0x00010000

/* the following control configurable behavior and should be reset everytime on env */
#define CTS_TRANSACTION_FILEOPS    0x00000001
#define CTS_TRANSACTION_ACTIVE     0x00000002
#define CTS_WAIT_FOR_LOCK          0x00000004
#define CTS_PUT_A_LOCK             0x00000008
#define CTS_RETURN_LOCKED_REC      0x00000010
#define CTS_MULTILOCK              0x00000020
#define CTS_READ_LOCK_TEST         0x00000040
#define CTS_RUNITLOCKDETECT        0x00000080
#define CTS_IGNORE_LOCK            0x00000100

/* by default read ops update ISAM ptr while write ops don't update ISAM ptr
 * the following modes serve to override defaults */
#define CTS_ISAM_PTR_UPD           0x00000200
#define CTS_READ_ISAM_PTR_UPD_NO   CTS_ISAM_PTR_UPD
#define CTS_WRITE_ISAM_PTR_UPD_YES CTS_ISAM_PTR_UPD

#define CTS_REL_ADDR               0x00000400
#define CTS_SKIP_LOCK              0x00000800
#define CTS_USE_IICT               0x00001000
#define CTS_BATCH_ADDITION         0x00002000
#define CTS_PREFETCH_LOCK          0x00004000
#define CTS_RETURN_KEY             0x00008000
#define CTS_NO_RUNIT_ANYUNLOCK     0x00010000
#define CTS_START_UNLOCK           0x00020000
#define CTS_LOCAL_INSTANCE         0x00040000
#define CTS_LOCK_FILE              0x00080000

/* the following are "States" and must be retained among calls */
/* using only the most significant byte */
#define CTS_START_EXECUTED         0x01000000
#define CTS_ATEND_ON_NEXT          0x02000000
#define CTS_ATEND_ON_PREV          0x04000000
#define CTS_ATEND_REACHED          (CTS_ATEND_ON_NEXT | CTS_ATEND_ON_PREV)
#define CTS_POS_NEVER_SET          0x08000000
#define CTS_LKISAM_SUSP            0x10000000
#define CTS_BULKADD_PRMIIDX        0x40000000
/* the following mask is used in CT_COMM_POST to set keep "states" from out->env */
#define CTS_STATE_MASK             0xff000000

/* the following are "States" and must be retained among calls on both env and io_env */
/* using only the most significant 4 bits */
#define CTS_PREFETCH_NEXT_EXECUTED 0x00100000
#define CTS_PREFETCH_PREV_EXECUTED 0x00200000
#define CTS_PREFETCH_EXECUTED      (CTS_PREFETCH_NEXT_EXECUTED | CTS_PREFETCH_PREV_EXECUTED)
/* the following mask is used in CT_COMM_POST to set keep "both-env-states" from out->env */
#define CTS_ALLENV_STATE_MASK      0x00f00000

/* Access the 'var' member only through CT_FILE_VAR macro to avoid memory alignment issues */
#define CT_FILE_VAR(ct) ((char *)ct + MEMW((char *)&ct->var[0] - (char *)ct))
#ifndef ctFeatRTG_SESSIONMEM
#ifdef ctFeatRTG_KEYS
#define CT_FILE_DYNAMIC(ct) (pTEXT)(rtgADDROFVAR(ct) + MEMW(rtgSIZEOFRTGKEYSEG(ct)))
#else
#define CT_FILE_KEYS(ct) (CT_KEY2 *)(CT_FILE_VAR(ct))
#define CT_FILE_DYNAMIC(ct) (char *)(CT_FILE_VAR(ct) + MEMW(sizeof(CT_KEY2) * ct->nkeys))
#endif
#endif /* ~ctFeatRTG_SESSIONMEM */

#endif /* ~_CTCBTYPE_H_ */

/* end of ctcbtype.h */
