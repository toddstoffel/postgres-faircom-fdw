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

#ifndef _CTCBOPTN_H_
#define _CTCBOPTN_H_

#ifdef ctdbSERVER_DLL_API
#undef CTPERM
#undef ctdbCTPERM

/* Definitions used when compiling RTG in the server model in a separate layer outside the core c-tree library */
#define rtgptDestroy ctptDestroy
#define rtgGetrtgmsgcbfnc ctGetrtgmsgcbfnc()
#define rtgSetrtgmsgcbfnc(val) ctSetrtgmsgcbfnc(val)
#define rtgGetrtgmsgcbusrctx ctGetrtgmsgcbusrctx()
#define rtgSetrtgmsgcbusrctx(val) ctSetrtgmsgcbusrctx(val)


#else /* ~ctdbSERVER_DLL_API */
#ifndef NO_CTPERM
#define CTPERM
#endif
#define ctdbCTPERM /* turns on CTPERM in ctopt2.h only when MULTITRD is active */

/* Definitions used when compiling RTG within the core c-tree library */
#define rtgptDestroy ptDestroy
#define rtgGetrtgmsgcbfnc rtgmsgcbfnc
#define rtgSetrtgmsgcbfnc(val) rtgmsgcbfnc = val
#define rtgGetrtgmsgcbusrctx rtgmsgcbusrctx
#define rtgSetrtgmsgcbusrctx(val) rtgmsgcbusrctx = val

#endif /* ~ctdbSERVER_DLL_API */

#include "ctstdr.h"
#include "ctoptn.h"
#ifdef ctCLIENT
#define ctCLIENTcore
#endif
#include "ctaerr.h"
#include "cterrc.h"
#include "ctifil.h"
#include "ctparm.h"
#include "ctsrvf.h"
#ifndef MULTITRD
#include "ctcomm.h"
#endif
#ifdef MULTITRD
#include "ctbtrd.h"
#endif

#include "ctdbsdk.h"

#if defined MULTITRD && !defined ctdbSERVER_DLL_API
#define rtgSERVERCORE
#endif

#if defined(ctCLIENT) || defined(ctMTCLIENT)
#define ctcbCLIENT
#endif
#if defined(ctLOCLIB) && !defined(ctFRCSNG)
#define ctcbLOCLIBCLIENT
#endif
#if defined(ctcbCLIENT) || defined(ctcbLOCLIBCLIENT)
#define ctcbANYCLIENT
#endif
#if defined(CTBOUND) && !defined(MULTITRD) && !defined(ctcbLOCLIBCLIENT)
#define ctcbSTANDALONE
#endif

/* use critical section instead of mutex */
#define ctFeatCOBOL_USE_CRITICAL_SECTION

/* MYFUNCTION time profiling... total ticks stored in global variable total_myf */
/*#define ctFeatCOBOL_MYFUNCTION_PROFILE*/

#if defined(ctCobolIS)
#define ctFeatCOBOL_MULTITHREAD_TASKS
#endif

#if defined(ctCobolIS)
#define ctFeatCOBOL_MAX_INSTANCES 512
#else
#define ctFeatCOBOL_MAX_INSTANCES 128
#endif

/* used to mark the code where we assumed to look for
 * CTCB_FILE_TYPE_INDEXED filetype since we have no idea of
 * the filetype and for now we only support indexed files */
#define ctFeatCOBOL_FILETYPE_FORCE_INDEXED

#ifdef ctFeatRTG_CBPTR
#undef ctFeatRTG_CBPTR
#endif

#ifdef CT_MAKE_IS_LOCAL
#undef CT_MAKE_IS_LOCAL
#endif

#if ctVER3_GE(11,9,5)
#define ctFeatRTG_FILEMATCH_FIX /* <config filematch> */
#define ctFeatRTG_LOCAL_FILES /* <localinstance> */
#endif

#if (defined(ctPortDSTINTL) || defined(ctPortDSTRTG))
#ifndef ctFeatRTG_BTRV_LOGIN_INSTANCE
#define ctFeatRTG_BTRV_LOGIN_INSTANCE /* BTRV LOGIN URI 'rtg.instance' parameter */
#endif
#ifndef ctFeatRTG_DATAFILESUFFIX_APPEND
#define ctFeatRTG_DATAFILESUFFIX_APPEND /* <datafilesuffix append> */
#endif
#ifndef ctFeatRTG_MAXSIZE
#define ctFeatRTG_MAXSIZE /* <maxsize> */
#ifndef ctFeatRTG_KEYS
#define ctFeatRTG_KEYS
#endif
#endif
#endif

#if (defined(ctPortDSTINTL) || defined(ctPortDSTRTG) || defined(ctPortHBRTG) )
#ifndef ctFeatRTG_LOCAL_FILES
#define ctFeatRTG_LOCAL_FILES /* <localinstance> */
#endif
#endif

#if defined(ctPortVERYANT)
#if ctVER_LT(12,5)
#ifndef ctBehavRTG_BULKADDITION_MEMFILE_WA
#define ctBehavRTG_BULKADDITION_MEMFILE_WA
#endif
#endif
#endif

/*
MB:200108: the #define ctFeatRTG_KEEPLOK_TRN_FIX is a workaround for an issue in
ctree's MULTIOPN_SAMUSR logic that is fixed in r201582. Since the issue has been fixed
and the workaround is the cause of another issue, we are disabling it but leave it here
disabled for future reference.
#define ctFeatRTG_KEEPLOK_TRN_FIX
*/

#if (defined ctPortWNT386 && !defined ctPortFAIRCOM_INTERNAL_GEN_DLL_RESTRICT_USE) || defined ctPortLINUX /* we are going to add platforms when tested as working */
#define ctFeatRTG_EXTFH_EXIT_ON_UNLOAD /* turn on logic to de-init c-tree at shared object unload or a program exit, not then the file open count goes to 0 */
#endif


#if ctVER3_GE(12,5,0)
#define ctFeatRTG_KEYS /* RTGKEY/RTGSEG */
#define ctFeatRTG_MAXSIZE /* <maxsize> */
#define ctFeatRTG_ATTRAPI  /* use ATTRAPI to store extended table, indices, schema information */
#define ctFeatRTG_NOSCHSEG2CLIENT /* do not send back SCHSEG information to client, recalculate the offset */
#define ctFeatRTG_GETFILEINST /* reorganize ctl_getfileinst() */
#define ctFeatRTG_ALTERSCHEMA /* ctutil -alter */
#define ctFeatRTG_CTFLEXREC /* <ctflexrec> support for hot alter table */
#define ctFeatRTG_NULLKEY /* any-segment and all-segment NULL key support */
#define ctFeatRTG_GETPERCENT /* CTS_GETPERCENT_KEY start operation */
#define ctFeatRTG_KEYNUMBER /* user-defined index numbers */
#define ctFeatRTG_GET_EXTENDED /* enhanced get record */
#define ctFeatRTG_FEATMASK /* featmask to expand structver */
#endif

#if ctVER3_GE(12,5,1)
#define ctFeatRTG_FILEPASSWORD /* file password support */
#define ctFeatRTG_BTRV_DATAONLY /* support for data-only files */
#define ctFeatRTG_WRITE_RET_RECDAT /* support write to return record data */
#endif

#define ctFeatRTG_BTRV_LOGIN_INSTANCE /* BTRV LOGIN URI 'rtg.instance' parameter */
#define ctFeatRTG_DATAFILESUFFIX_APPEND /* <datafilesuffix append> */
#define ctFeatRTG_LOGWHOFORMAT /* <log whoformat> */
#define ctFeatRTG_TASKID /* substitution specifier '%o' fotr server task ID */
#define ctFixRTG_LOGDEBUGCONFIG /* fix log.debug.config for dynamic config */
#define ctFixRTG_PREFETCH_ON_LOCKED /* fix prefetch on locked record */
#define ctFeatRTG_NODESIZE /* index node size in ctutil -info output */
#define ctBehavRTG_BULKADDITION_IDX_XCREBLK /* get XCREblk from index file */
#define ctBehavRTG_MAKE_FROM_XDD_WITH_NO_KEYS /* allow creating files from XDD that specify no key */

#if ctVER3_GE(12,6,0)
#define ctFeatRTG_TWOPHASE_COMMIT /* enable two-phase commit */
#define ctFeatRTG_RECORDSET /* extended record rewrite */
#define ctFeatRTG_NORUNITLOCKDETECT_UNLOCKALL /* <runitlockdetect unlockall> */
#define ctFeatRTG_FIXMINLEN_OVER_64KB /* support for fixed record length > 64KB */
#define ctFeatRTG_VARMINLEN_OVER_64KB /* support for variable minimum record length > 64KB */
#define ctFeatRTG_OPEN_FOR_INFO /* displays RTG resource version in ctutil -info */
#define ctFixRTG_BULKADDITION_ADD2END /* fix bulkaddition rebuild by using ctADD2END file mode */
#define ctFeatRTG_SESSIONMEM /* allocate RPC memory by session rather than by file */
#define ctFeatRTG_ACCESS /* support for checking existence of a file/directory */
#define ctFeatRTG_ALTERSCHEMAv2 /* new version of ctutil -alter with closed file */
#define ctFeatRTG_DATA_FILTERING /* support for record data filtering */
#define ctFeatRTG_AUGMENT /* ctutil -augment command */
#define ctFeatRTG_RECORD_FOOTER /* support for handling records with greater minlen/maxlen */
#define ctFeatRTG_SQLINFO_ON_OPEN /* support passing an XDD string to open to attempt sqlinfo */
#define ctFeatRTG_CTUTIL_UTILAPI_SQL /* use RTG Util API to for ctutil -sql commands */
#endif

#if ctVER3_GE(12,6,2)
#define ctFeatRTG_RETRYOPEN /* retry open if file is already open exclusive */
#endif

#if ctVER3_GE(12,6,3)
#define ctFeatRTG_PARTITION /* ctutil -partition command */
#define ctFeatRTG_HANDLE /* support for internal RTG handles */
#define ctFeatRTG_ADDIDX_UPDATE /* add index's update option */
#endif

#ifdef STREE_TRUNK
#endif

#ifdef ctFeatRTG_HANDLE
#ifndef ctFeatRTG_GETFILEINST
#error #define ctFeatRTG_HANDLE requires #define ctFeatRTG_GETFILEINST
#endif
#ifndef ctFeatRTG_LOGWHOFORMAT
#error #define ctFeatRTG_HANDLE requires #define ctFeatRTG_LOGWHOFORMAT
#endif
#ifndef ctFeatRTG_TASKID
#error #define ctFeatRTG_HANDLE requires #define ctFeatRTG_TASKID
#endif
#ifndef ctcbSTANDALONE
#ifndef ctFeatRTG_API
#error #define ctFeatRTG_HANDLE requires #define ctFeatRTG_API
#endif
#endif
#endif

#ifdef ctFeatRTG_CTUTIL_UTILAPI_SQL
#ifndef ctFeatRTG_SQLINFO_ON_OPEN
#error #define ctFeatRTG_CTUTIL_UTILAPI_SQL requires #define ctFeatRTG_SQLINFO_ON_OPEN
#endif
#endif

#ifdef ctFeatRTG_DATA_FILTERING
#ifndef ctFeatRTGfldfltr
#error #define ctFeatRTG_DATA_FILTERING requires #define ctFeatRTGfldfltr
#endif
#endif

#ifdef ctFeatRTG_ALTERSCHEMAv2
#ifndef ctFeatRTG_ALTERSCHEMA
#error #define ctFeatRTG_ALTERSCHEMAv2 requires #define ctFeatRTG_ALTERSCHEMA
#endif
#ifndef ctFeatRTG_SESSIONMEM
#error #define ctFeatRTG_ALTERSCHEMAv2 requires #define ctFeatRTG_SESSIONMEM
#endif
#ifndef ctFeatRTG_FILEPASSWORD
#error #define ctFeatRTG_ALTERSCHEMAv2 requires #define ctFeatRTG_FILEPASSWORD
#endif
#endif

#ifdef ctFeatRTG_ACCESS
#ifndef ctFeatRTG_SESSIONMEM
#error #define ctFeatRTG_ACCESS requires #define ctFeatRTG_SESSIONMEM
#endif
#endif

#ifdef ctFeatRTG_NORUNITLOCKDETECT_UNLOCKALL
#if !defined(ctBEHAV_MULTIOPN_SAMUSR_A) && defined(MULTITRD)
#error #define ctFeatRTG_NORUNITLOCKDETECT_UNLOCKALL requires #define ctBEHAV_MULTIOPN_SAMUSR_A
#endif
#endif

#ifdef ctFeatRTG_VARMINLEN_OVER_64KB
#ifndef ctFeatRTG_FIXMINLEN_OVER_64KB
#error #define ctFeatRTG_VARMINLEN_OVER_64KB requires #define ctFeatRTG_FIXMINLEN_OVER_64KB
#endif
#endif

#ifdef ctFeatRTG_FILEPASSWORD
#ifndef ctFeatCOMPACT_TRUNCATE
#error #define ctFeatRTG_FILEPASSWORD requires #define ctFeatCOMPACT_TRUNCATE
#endif
#else /* ~ctFeatRTG_FILEPASSWORD */
#ifdef ctFeatRTG_SESSIONMEM
#error #define ctFeatRTG_SESSIONMEM requires #define ctFeatRTG_FILEPASSWORD
#endif
#ifdef ctFeatRTG_PARTITION
#error #define ctFeatRTG_PARTITION requires #define ctFeatRTG_FILEPASSWORD
#endif
#ifdef ctFeatRTG_SQLINFO_ON_OPEN
#error #define ctFeatRTG_SQLINFO_ON_OPEN requires #define ctFeatRTG_FILEPASSWORD
#endif
#ifdef ctFeatRTG_RETRYOPEN
#error #define ctFeatRTG_RETRYOPEN requires #define ctFeatRTG_FILEPASSWORD
#endif
#endif /* ~ctFeatRTG_FILEPASSWORD */

#ifdef ctFeatRTG_RECORDSET
#ifndef ctFeatRTG_GET_EXTENDED
#error #define ctFeatRTG_RECORDSET requires #define ctFeatRTG_GET_EXTENDED
#endif
#endif

#ifndef ctFeatRTG_KEYS
#ifdef ctFeatRTG_ADDIDX_UPDATE
#error #define ctFeatRTG_ADDIDX_UPDATE requires #define ctFeatRTG_KEYS
#endif
#ifdef ctFeatRTG_SESSIONMEM
#error #define ctFeatRTG_SESSIONMEM requires #define ctFeatRTG_KEYS
#endif
#ifdef ctFeatRTG_FILEPASSWORD
#error #define ctFeatRTG_FILEPASSWORD requires #define ctFeatRTG_KEYS
#endif
#ifdef ctFeatRTG_MAXSIZE
#error #define ctFeatRTG_MAXSIZE requires #define ctFeatRTG_KEYS
#endif
#ifdef ctFeatRTG_NODESIZE
#error #define ctFeatRTG_NODESIZE requires #define ctFeatRTG_KEYS
#endif
#ifdef ctFeatRTG_NULLKEY
#error #define ctFeatRTG_NULLKEY requires #define ctFeatRTG_KEYS
#endif
#ifdef ctFeatRTG_KEYNUMBER
#error #define ctFeatRTG_KEYNUMBER requires #define ctFeatRTG_KEYS
#endif
#ifdef ctFeatRTG_NORUNITLOCKDETECT_UNLOCKALL
#error #define ctFeatRTG_NORUNITLOCKDETECT_UNLOCKALL requires #define ctFeatRTG_KEYS
#endif
#ifdef ctFeatRTG_OPEN_FOR_INFO
#error #define ctFeatRTG_OPEN_FOR_INFO requires #define ctFeatRTG_KEYS
#endif
#endif /* ~ctFeatRTG_KEYS */

#if defined(ctFeatRTG_NULLKEY) && !defined(ctFeatNULLKEYVAL)
#error #define ctFeatRTG_NULLKEY requires #define ctFeatNULLKEYVAL
#endif

#if defined ctFeatRTG_ATTRAPI && (CTATTR_RESOURCE_VER < CTATTR_RESOURCE_VER_MULTISCHEMA)
#error CTATTR_RESOURCE_VER should be at minimum  CTATTR_RESOURCE_VER_MULTISCHEMA
#endif

#if defined ctFeatRTG_ATTRAPI && !defined CTDB_TBLATTR
#error ctFeatRTG_ATTRAPI requires CTDB_TBLATTR
#endif

#if defined ctFeatRTG_ALTERSCHEMA && !defined ctFeatRTG_ATTRAPI
#error ctFeatRTG_ALTERSCHEMA requires ctFeatRTG_ATTRAPI
#endif

#ifndef NO_ctFeatRTG_SQL
#define ctFeatRTG_SQL
#endif

#ifdef _MSC_VER
#pragma warning(1 : 4701)
#if (_MSC_VER < 1400)
#pragma warning(disable : 4761)
#endif
#endif

#ifndef UNREFERENCED_PARAMETER
#define UNREFERENCED_PARAMETER(P) (P=P)
#endif

#ifdef _WIN32
#define cbDLLexport __declspec(dllexport)
#define cbDLLimport __declspec(dllimport)
#else
#define cbDLLexport
#define cbDLLimport extern
#endif

#define MEMU (8)
#define MEMW(n) (((n) % MEMU) ? (n) + MEMU - (n) % MEMU : (n))
#define MEMDW(n) (((n) % (16)) ? (n) + (16) - (n) % (16) : (n))

#define CT_PATH_LEN             (255+1) /* includes terminator */
#define CT_EXTN_LEN                 16
#define CT_SERV_LEN           MAX_NAME
#define CT_USER_LEN                IDZ
#define CT_PASS_LEN                 64
#define CT_NODE_LEN           MAX_NAME
#define CT_VERSION_LEN              32
#define CT_COMMENT_LEN           (30+1) /* includes terminator */
#define CT_FLPW_LEN             (255+1) /* includes terminator */

#define LOGWHOMAXLEN 64

#if !defined(TPE_MAX_IDLEN)
#define TPE_MAX_IDLEN	64
#endif
#if !defined(TPE_MAX_DBIDLEN)
#define TPE_MAX_DBIDLEN	TPE_MAX_IDLEN
#endif

/* interface types */
typedef enum INTFTYPE {
	/* do not change these values! they affect licensing and c/s, just add at the end.*/
	INTF_INVALID_ACE = 0,
	INTF_ACUCOBOL = 1,
	INTF_ISCOBOL = 2,
	INTF_EXTFH = 3,
	INTF_BTRV = 4
	/* do not change these values! they affect licensing and c/s, just add at the end.*/
} INTFTYPE;

#define INTFNAME(a) ( \
	(a == INTF_ACUCOBOL) ? "ACUCOBOL" : \
	(a == INTF_ISCOBOL) ? "isCOBOL" : \
	(a == INTF_EXTFH) ? "Micro Focus COBOL and ExtFH" : \
	(a == INTF_BTRV) ? "RM/COBOL and BTRV" : "" )

typedef struct {
	UCOUNT	off;
	UCOUNT	len;
	UTEXT	pad[1];
	UTEXT	nulval;
	UTEXT	typ;
	UTEXT	mod;
} RTGSEG, * pRTGSEG;
typedef struct {
	ULONG	segoff;
	LONG	type;
	UCOUNT	nsegs;
	UCOUNT	len;
	UTEXT	pad[2];
	UTEXT	num;
	UTEXT	nulval;
} RTGKEY, * pRTGKEY;

typedef ULONG RTGTASKID, * pRTGTASKID;

#define rtgADDROFVAR(x) ((pTEXT)(x) + MEMW((pTEXT)&((x)->var[0]) - (pTEXT)(x)))
#ifdef ctFeatRTG_KEYS
#define rtgADDROFRTGKEY(x) ((pRTGKEY)(rtgADDROFVAR(x)))
#define rtgADDROFRTGSEG(x) ((pRTGSEG)(rtgADDROFRTGKEY(x) + (x)->nkeys))
#define rtgNUMOFRTGSEG(x) (rtgNumOfRTGSEG(rtgADDROFRTGKEY(x), (x)->nkeys))
#define rtgSIZEOFRTGKEY(n) (rtgSIZEOF(RTGKEY) * n)
#define rtgSIZEOFRTGSEG(n) (rtgSIZEOF(RTGSEG) * n)
#define rtgSIZEOFRTGKEYSEG(x) (rtgSizeOfRTGKEYSEG(rtgADDROFRTGKEY(x), (x)->nkeys))
#define rtgADDROFKEYSEG(k) ((pRTGSEG)((pTEXT)(k) + (k)->segoff))
#endif /* ctFeatRTG_KEYS */

#ifdef ctFeatRTG_GET_EXTENDED
/* recordset feature bitmask */
#define RTGRSET_FEAT_RECFIELDS   0x01
#define RTGRSET_FEAT_RSET_FILTER 0x02

/* recordset modes */
#define RTGRSET_MODE_BTRVGETEXT 0x01
#define RTGRSET_MODE_BTRVCHUNK  0x02
#define RTGRSET_MODE_BTRVTRUNC  0x03

typedef struct {
	ULONG	len;
	COUNT	pad;
	UTEXT	feat;
	UTEXT	mode;
} RTGRSET, * pRTGRSET;
typedef struct {
	ULONG	nrecs;
	COUNT	pad;
	UCOUNT	nflds;
} RTGRSET1, * pRTGRSET1;
#ifdef ctFeatRTG_DATA_FILTERING
typedef struct {
	ULONG	nrecs;
	ULONG	nrejs;
	UCOUNT	nflts;
	UCOUNT	nflds;
} RTGRSET2, * pRTGRSET2;
typedef struct {
	ULONG	off;
	ULONG	len;
	COUNT	type;
	COUNT	pad2;
	UTEXT	mode;
	UTEXT	more;
	UTEXT	like;
	TEXT	conv;
} RTGFLT, * pRTGFLT;
typedef struct {
	COUNT type;
	UTEXT conv;
} RTGFLDTYPE, * pRTGFLDTYPE;
#endif /* ctFeatRTG_DATA_FILTERING */
typedef struct {
	ULONG	off;
	ULONG	len;
} RTGFLD, * pRTGFLD;
#endif /* ctFeatRTG_GET_EXTENDED */

typedef enum {
	HANDLEID_INVALID = 0x0,
	HANDLEID_SESSION = 0x1,
	HANDLEID_INSTANCE = 0x2,
	HANDLEID_FILE = 0x3,
	HANDLEID_CONF = 0x5
} HANDLEID;

#endif /* ~_CTCBOPTN_H_ */

/* end of ctcboptn.h */
