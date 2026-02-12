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

#ifndef _CTCBCONF_H_
#define _CTCBCONF_H_

#include "ctcboptn.h"
#include "ctcbstdr.h"
#include "ctdbport.h"
#include "ctdbxmlp.h"

#define CONFDEFAULTTEXTMASK ((TEXT)(UTEXT)0x80)
#define CONFDEFAULTLONGMASK ((LONG)(ULONG)0x80000000)
#define isConfDefault(fileconf) ((sizeof(fileconf) > sizeof(TEXT)) ? (fileconf & CONFDEFAULTLONGMASK) : (fileconf & CONFDEFAULTTEXTMASK))
#define ConfValueTEXT(fileconf) (fileconf & ~CONFDEFAULTTEXTMASK)
#define ConfValueLONG(fileconf) (fileconf & ~CONFDEFAULTLONGMASK)

#define CONFDUMPMAXLINELEN 256
#define CONFDUMPMAXLEVLLEN 16

#define CTCB_FILE_TYPE_INDEXED 0
#define CTCB_FILE_TYPE_RELATIVE 1
#define CTCB_FILE_TYPE_LINE_SEQ 2
#define CTCB_FILE_TYPE_SEQ 3
#define CTCB_FILE_TYPE_ALL 4 /* this must be the last file type and it is used to mark rules that apply to all file types  */

typedef enum FILEMATCHVER {
	OriginalFileMatch = 0, /* buggy and sorted alphabetically */
	DefaultFileMatch = 1 /* sorted by order of appeareance in config file */
} FILEMATCHVER;

typedef struct tagRTGCONF RTGCONF, * pRTGCONF, ** ppRTGCONF;
typedef struct tagCONFINST CONFINST, * pCONFINST, ** ppCONFINST;
typedef struct tagCONFFILE CONFFILE, * pCONFFILE, ** ppCONFFILE;
typedef struct tagCONFOPTS CONFOPTS, * pCONFOPTS, ** ppCONFOPTS;

struct tagCONFOPTS
{
	FILEMATCHVER	_filematch;
	TEXT	allowcorrupt;
	TEXT	automkdir;
	TEXT	batchadd;
	LONG	batchaddrecs;
	TEXT	bulkaddition;
	TEXT	bulkaddprmiidx;
	TEXT	ccc;
	TEXT	cccsystem;
	TEXT	copy_noreaders;
#ifdef ctFeatRTG_ATTRAPI
	TEXT	ctattr;
#endif
	TEXT	ctfixed;
#ifdef ctFeatRTG_CTFLEXREC
	TEXT	ctflexrec;
#endif
	TEXT	datacompress;
	TEXT	datacompresstype;
	TEXT	datacompresslevel;
	TEXT	datacompressstrategy;
	TEXT	datafilesuffix[CT_EXTN_LEN];
	TEXT	datafilesuffix_d;
#ifdef ctFeatRTG_DATAFILESUFFIX_APPEND
	TEXT	datafilesuffixappend;
	TEXT	datafilesuffixcase;
#endif
	TEXT	delcurrent;
	TEXT	detectlock;
	TEXT	encrypt;
	TEXT	encrypttype;
	TEXT	extfhexit;
	TEXT	fastdup;
	TEXT	filecopy;
	TEXT	filecopyoverwrite;
	TEXT	filepool;
	LONG	filepoolsize;
	LONG	fileversion;
	TEXT	forcedelete;
	TEXT	hugefile;
	TEXT	ignorelock;
	TEXT	indexfilesuffix[CT_EXTN_LEN];
	TEXT	indexfilesuffix_d;
	TEXT	inpool;
	TEXT	inmissingpool;
	TEXT	keycheck;
	TEXT	keycompress;
	LONG	keycompressvlennod;
	LONG	keycompresstype;
	LONG	locktimeout;
	TEXT	locktype;
	TEXT	log;
	TEXT	logevents;
	LONG	logerrorevents;
	LONG	logdebugevents;
	TEXT	logdebugconfigfull;
	TEXT	logdebugextfhfcd;
	TEXT	logfile[CT_PATH_LEN];
	TEXT	logfile_d;
#ifdef ctFeatRTG_LOGWHOFORMAT
	TEXT	logwhofmt[LOGWHOMAXLEN];
	TEXT	logwhofmt_d;
#endif
	TEXT	mapname[CT_PATH_LEN];
	TEXT	mapname_d;
	TEXT	mapdir[CT_PATH_LEN];
	TEXT	mapdir_d;
	TEXT	maxlencheck;
#ifdef ctFeatRTG_MAXSIZE
	LONG	maxsize;
#endif
	TEXT	memoryfile;
	TEXT	memoryfilepersist;
	TEXT	missingpool;
	LONG	missingpoolsize;
	TEXT	normalizedrive[4];
	TEXT	normalizedrive_d;
	TEXT	normalizedriveforce;
	TEXT	normalizedriverelative;
	TEXT	normalizerelative;
	TEXT	normalizesep[4];
	TEXT	normalizesep_d;
	TEXT	optimisticadd;
	LONG	permission;
	TEXT	prefetch;
	LONG	prefetchrecs;
	LONG	prefetchttl;
	TEXT	prefetchlock;
	TEXT	readonly;
#ifdef ctFeatRTG_RECORD_FOOTER
	LONG	recordcheck;
#endif
	TEXT	retrylock;
	TEXT	rowid;
	TEXT	rowidsize;
	TEXT	rpc;
	TEXT	rpccrc;
	TEXT	runitlockdetect;
	TEXT	norunitanyunlock;
#ifdef ctFeatRTG_NORUNITLOCKDETECT_UNLOCKALL
	TEXT	norunitunlockall;
#endif
	TEXT	scancache;
	TEXT	skiplock;
	TEXT	smartcopy;
	TEXT	smartcopyrpc;
	TEXT	smartcopyraw;
	TEXT	sql;
	TEXT	sqlxdd[CT_PATH_LEN];
	TEXT	sqlxfd[CT_PATH_LEN];
	TEXT	sqlrules[CT_PATH_LEN];
	TEXT	sqldb[TPE_MAX_DBIDLEN];
	TEXT	sqldb_d;
	TEXT	sqlpw[CT_PASS_LEN];
	TEXT	sqlpw_d;
	TEXT	sqlsymb[TPE_MAX_IDLEN];
	TEXT	sqlprefix[TPE_MAX_IDLEN];
	TEXT	sqlowner[TPE_MAX_IDLEN];
	TEXT	sqlpublic;
	TEXT	sqlnumfmt;
	TEXT	startonread;
	TEXT	temporary;
	TEXT	transaction;
	TEXT	transactionautocommitdefer;
	TEXT	transactionlogging;
	TEXT	transactiondepend;
#ifdef ctFeatCOMPACT_TRUNCATE
	TEXT	truncateifexist;
#endif
	TEXT	trxholdslocks;
	TEXT	writethru;
};

#if defined(ctFeatRTG_FAILOVER_CLUSTER)
typedef struct tagCONFCLUS
{
	TEXT	id[IDZ];
	TEXT	mode[MAX_NAME];
	TEXT	latency[11]; /* 4 bytes digits */
	TEXT	failover;
	pCTDBLIST	servers;
} CONFCLUS, ctMEM* pCONFCLUS, ctMEM* ctMEM* ppCONFCLUS;
#endif /* ctFeatRTG_FAILOVER_CLUSTER */

typedef enum INSTTYPE {
	ClientInstance = 0,
	LocalInstance = 1,
	RedirInstance = 2,
	FallbackInstance = 3
} INSTTYPE;

struct tagCONFINST
{
	CONFOPTS	opt;
	TEXT	server[MAX_NAME];
	TEXT	user[MAX_NAME];
	TEXT	password[MAX_NAME];
	TEXT	authfile[MAX_NAME];
	TEXT	name[IDZ];
	TEXT	connect;
	INSTTYPE	type;
	COUNT	bufs;
	COUNT	fils;
	COUNT	sect;
	COUNT	dbufs;
	TEXT	logpath[MAX_NAME];
	TEXT	versioncheck;
	TEXT	endiancheck;
	TEXT	redirlib[MAX_NAME];
	TEXT	redirfunc[MAX_NAME];
	pFUNC	redir;
	TEXT	ssl;
	TEXT	sslcert[MAX_NAME];
	TEXT	temppath[MAX_NAME];
	ULONG	sortmem;
	TEXT	shmemdir[MAX_NAME];
	TEXT	reconnect;
#ifdef ctFeatRTG_FAILOVER_CLUSTER
	TEXT	cluster[IDZ];
	pCONFCLUS	clusitem;
#else /* ~ctFeatRTG_FAILOVER_CLUSTER */
	TEXT	cluster;
	UCOUNT	failovertimeout;
#endif /* ~ctFeatRTG_FAILOVER_CLUSTER */
};

typedef enum MATCHTYPE {
	NoMatch = 0,
	ExactMatch = 1,
	BeginswithMatch = 2,
	EndswithMatch = 3,
	ContainsMatch = 4,
} MATCHTYPE;

#define MINFILEPRIORITY -32767
#define MAXFILEPRIORITY +32767
#define DEFFILEPRIORITY 0
#define UNSETFILEPRIORITY -32768

struct tagCONFFILE
{
	CONFOPTS	opt;
	pCONFINST	institem;
	TEXT	name[MAX_NAME];
	TEXT	dir[MAX_NAME];
	MATCHTYPE	namematch;
	MATCHTYPE	dirmatch;
	COUNT		priority;
	TEXT		casesensitive;
	TEXT		filetype;
	UINT		ordinal;
};

struct tagRTGCONF
{
	HANDLEID	handleid;
	CONFOPTS	opt;
	pCTDBLIST	instlist;
	pCTDBLIST	filelist;
	pTEXT		xml;
	size_t		xmllen;
#if defined(ctFeatRTG_FAILOVER_CLUSTER)
	pCTDBLIST	cluslist;
#endif /* ctFeatRTG_FAILOVER_CLUSTER */
	TEXT	path[CT_PATH_LEN];
};

typedef struct
{
	char*	str;		/* mode name */
	NINT	mode;		/* mode value */
} CTCONFMODE, *pCTCONFMODE;

#define CONF_ERRMSG_DEPRECATED_ELEM	"Option <%s> near line %d is deprecated"
#define CONF_ERRMSG_INVALID_ELEM	"Invalid element <%s> near line %d"
#define CONF_ERRMSG_INVALID_ATTR1	"Invalid <%s> attribute '%s' near line %d"
#define CONF_ERRMSG_INVALID_ATTVAL1	"Invalid <%s %s> attribute value '%s' near line %d"
#define CONF_ERRMSG_TOOLONG_ATTVAL1	"Oversized <%s %s> attribute value '%s' near line %d"
#define CONF_ERRMSG_INVALID_ATTVAL2	"Invalid <%s><%s %s> attribute value '%s' near line %d"
#define CONF_ERRMSG_INVALID_ATTR3	"Invalid <%s><%s><%s> attribute '%s' near line %d"
#define CONF_ERRMSG_INVALID_ATTVAL3	"Invalid <%s><%s><%s %s> attribute value '%s' near line %d"
#define CONF_ERRMSG_INVALID_CONT1	"Invalid <%s> content '%s' near line %d"
#define CONF_ERRMSG_TOOLONG_CONT1	"Oversized <%s> content '%s' near line %d"
#define CONF_ERRMSG_INVALID_CONT2	"Invalid <%s><%s> content '%s' near line %d"
#define CONF_ERRMSG_TOOLONG_CONT2	"Oversized <%s><%s> content '%s' near line %d"
#define CONF_ERRMSG_INVALID_CONT3	"Invalid <%s><%s><%s> content '%s' near line %d"
#define CONF_ERRMSG_NOMEMTOALLOC	"Not enough memory to allocate configuration item"
#define CONF_ERRMSG_NOMEMTOADDLIST	"Not enough memory to add configuration item to list"
#define CONF_ERRMSG_NOTSUPPORTED	"File name or directory format not supported"
#define CONF_ERRMSG_INVALIDXMLTREE	"Unexpected invalid parse tree"

#define CONF_ERR_OK				0
#define CONF_ERR_TOOLONG		1
#define CONF_ERR_INVALID		2
#define CONF_ERR_NOMEMTOALLOC	3
#define CONF_ERR_NOMEMTOADDLIST	4
#define CONF_ERR_NOTSUPPORTED	5

#define CONF_OP_SET				0
#define CONF_OP_ADD				4
#define CONF_OP_DEL				8
#define CONF_OP_DUMP			16
#define CONF_OP_GLOBAL			0
#define CONF_OP_INSTANCE		1
#define CONF_OP_LOCALINSTANCE	2
#define CONF_OP_FILE			3

#define CONF_OP_INIT			(CONF_OP_ADD|CONF_OP_GLOBAL)
#define CONF_OP_ADDINST			(CONF_OP_ADD|CONF_OP_INSTANCE)
#define CONF_OP_ADDLOCINST		(CONF_OP_ADD|CONF_OP_LOCALINSTANCE)
#define CONF_OP_ADDFILE			(CONF_OP_ADD|CONF_OP_FILE)
#define CONF_OP_SETGLOBAL		(CONF_OP_SET|CONF_OP_GLOBAL)
#define CONF_OP_SETINST			(CONF_OP_SET|CONF_OP_INSTANCE)
#define CONF_OP_SETLOCINST		(CONF_OP_SET|CONF_OP_LOCALINSTANCE)
#define CONF_OP_SETFILE			(CONF_OP_SET|CONF_OP_FILE)
#define CONF_OP_TERM			(CONF_OP_DEL|CONF_OP_GLOBAL)

#define CONF_DATACOMPRESSION_TYPE_RLE	"rle"
#define CONF_DATACOMPRESSION_TYPE_ZLIB	"zlib"

#define CONF_ENCRYPT_TYPE_CAMO	"camo"
#define CONF_ENCRYPT_TYPE_DES8	"des8"
#define CONF_ENCRYPT_TYPE_DES16	"des16"
#define CONF_ENCRYPT_TYPE_DES24	"des24"
#define CONF_ENCRYPT_TYPE_TWF16	"twofish16"
#define CONF_ENCRYPT_TYPE_TWF24	"twofish24"
#define CONF_ENCRYPT_TYPE_TWF32	"twofish32"
#define CONF_ENCRYPT_TYPE_BLF8	"blowfish8"
#define CONF_ENCRYPT_TYPE_BLF9	"blowfish9"
#define CONF_ENCRYPT_TYPE_BLF10	"blowfish10"
#define CONF_ENCRYPT_TYPE_BLF11	"blowfish11"
#define CONF_ENCRYPT_TYPE_BLF12	"blowfish12"
#define CONF_ENCRYPT_TYPE_BLF13	"blowfish13"
#define CONF_ENCRYPT_TYPE_BLF14	"blowfish14"
#define CONF_ENCRYPT_TYPE_BLF15	"blowfish15"
#define CONF_ENCRYPT_TYPE_BLF16	"blowfish16"
#define CONF_ENCRYPT_TYPE_BLF17	"blowfish17"
#define CONF_ENCRYPT_TYPE_BLF18	"blowfish18"
#define CONF_ENCRYPT_TYPE_BLF19	"blowfish19"
#define CONF_ENCRYPT_TYPE_BLF20	"blowfish20"
#define CONF_ENCRYPT_TYPE_BLF21	"blowfish21"
#define CONF_ENCRYPT_TYPE_BLF22	"blowfish22"
#define CONF_ENCRYPT_TYPE_BLF23	"blowfish23"
#define CONF_ENCRYPT_TYPE_BLF24	"blowfish24"
#define CONF_ENCRYPT_TYPE_BLF25	"blowfish25"
#define CONF_ENCRYPT_TYPE_BLF26	"blowfish26"
#define CONF_ENCRYPT_TYPE_BLF27	"blowfish27"
#define CONF_ENCRYPT_TYPE_BLF28	"blowfish28"
#define CONF_ENCRYPT_TYPE_BLF29	"blowfish29"
#define CONF_ENCRYPT_TYPE_BLF30	"blowfish30"
#define CONF_ENCRYPT_TYPE_BLF31	"blowfish31"
#define CONF_ENCRYPT_TYPE_BLF32	"blowfish32"
#define CONF_ENCRYPT_TYPE_BLF33	"blowfish33"
#define CONF_ENCRYPT_TYPE_BLF34	"blowfish34"
#define CONF_ENCRYPT_TYPE_BLF35	"blowfish35"
#define CONF_ENCRYPT_TYPE_BLF36	"blowfish36"
#define CONF_ENCRYPT_TYPE_BLF37	"blowfish37"
#define CONF_ENCRYPT_TYPE_BLF38	"blowfish38"
#define CONF_ENCRYPT_TYPE_BLF39	"blowfish39"
#define CONF_ENCRYPT_TYPE_BLF40	"blowfish40"
#define CONF_ENCRYPT_TYPE_BLF41	"blowfish41"
#define CONF_ENCRYPT_TYPE_BLF42	"blowfish42"
#define CONF_ENCRYPT_TYPE_BLF43	"blowfish43"
#define CONF_ENCRYPT_TYPE_BLF44	"blowfish44"
#define CONF_ENCRYPT_TYPE_BLF45	"blowfish45"
#define CONF_ENCRYPT_TYPE_BLF46	"blowfish46"
#define CONF_ENCRYPT_TYPE_BLF47	"blowfish47"
#define CONF_ENCRYPT_TYPE_BLF48	"blowfish48"
#define CONF_ENCRYPT_TYPE_BLF49	"blowfish49"
#define CONF_ENCRYPT_TYPE_BLF50	"blowfish50"
#define CONF_ENCRYPT_TYPE_BLF51	"blowfish51"
#define CONF_ENCRYPT_TYPE_BLF52	"blowfish52"
#define CONF_ENCRYPT_TYPE_BLF53	"blowfish53"
#define CONF_ENCRYPT_TYPE_BLF54	"blowfish54"
#define CONF_ENCRYPT_TYPE_BLF55	"blowfish55"
#define CONF_ENCRYPT_TYPE_BLF56	"blowfish56"
#define CONF_ENCRYPT_TYPE_AES16	"aes16"
#define CONF_ENCRYPT_TYPE_AES24	"aes24"
#define CONF_ENCRYPT_TYPE_AES32	"aes32"

#define CONF_EXTFHEXIT_ATUNLOAD 0
#define CONF_EXTFHEXIT_ATFILECLOSE 1

LOCAL LONG GetLongSetting(LONG cfgsett, pLONG appsett)
{
	LONG sett;
	if (appsett && isConfDefault(cfgsett))
		sett = *appsett;	/* use application setting */
	else
		sett = ConfValueLONG(cfgsett); /* use config setting */
	return sett;
}

LOCAL TEXT GetByteSetting(TEXT cfgsett, pTEXT appsett)
{
	TEXT sett;
	if (appsett && isConfDefault(cfgsett))
		sett = *appsett;	/* use application setting */
	else
		sett = ConfValueTEXT(cfgsett); /* use config setting */
	return sett;
}

LOCAL LONG GetBitSetting(LONG sett, TEXT cfgsett, pLONG appsett, LONG bitmask)
{
	if (appsett && isConfDefault(cfgsett)) {
		/* use application setting */
		if ((*appsett) & bitmask)
			sett |= bitmask;
		else
			sett &= ~bitmask;
	} else {
		/* use config setting */
		if (ConfValueTEXT(cfgsett))
			sett |= bitmask;
		else
			sett &= ~bitmask;
	}
	return sett;
}

LOCAL LONG GetLocktypeSetting(LONG sett, TEXT cfgsett, pLONG appsett, LONG bitmask)
{
	if (appsett && isConfDefault(cfgsett)) {
		/* use application setting */
		if ((*appsett) & bitmask)
			sett |= bitmask;
		else
			sett &= ~bitmask;
	} else {
		/* use config setting */
		if (ConfValueTEXT(cfgsett))
			sett &= ~bitmask;
		else
			sett |= bitmask;
	}
	return sett;
}

pRTGCONF ctcbConfGet(pCTXML handle, TEXT empty, pNINT errflag, pTEXT errmsg, size_t errmsglen, INTFTYPE intf, pTEXT ctree_log);
VOID ctcbConfPut(ppRTGCONF conf);
VOID ctcbConfSort(pRTGCONF conf);
NINT ctcbConfSettingSet(pCONFOPTS opt, cpTEXT option, cpTEXT value, TEXT level, TEXT explct);
pCONFFILE ctcbConfFileCreate(pCTDBLIST filelist, pCONFINST institem, pCONFOPTS optsitem, pTEXT errmsg, size_t errmsglen, INTFTYPE intf, cpTEXT name, cpTEXT dir, cpTEXT priority, cpTEXT casesensitive, cpTEXT filetype);
NINT ctcbConfFileRemove(pCTDBLIST list, pCONFFILE item);
NINT ctcbConfInstSetAttr(pCONFINST institem, cpTEXT name, cpTEXT value, NINT lineno, pTEXT errmsg, size_t errmsglen);
pCONFINST ctcbConfInstanceCreate(pCTDBLIST instlist, pCONFOPTS optsitem, pTEXT errmsg, size_t errmsglen, INTFTYPE intf, INSTTYPE type);
NINT ctcbConfInstanceRemove(pCTDBLIST list, pCONFINST item);
NINT ctcbConfFileGetDefault(pCTDBLIST filelist, ppCONFINST deflinst, ppCONFFILE deflfile, TEXT filetype);
TEXT ctcbConfFileType(TEXT filetype);
pTEXT ctcbConfDump(pRTGCONF conf, pTEXT bufptr, size_t *buflen, TEXT full);

typedef enum
{
	Allowcorrupt,
	Automkdir,
	Batchaddition,
	BatchadditionRecords,
	Bulkaddition,
	BulkadditionPrmiidx,
#ifdef ctFeatRTG_ATTRAPI
	Ctattr,
#endif
	Ctfixed,
#ifdef ctFeatRTG_CTFLEXREC
	Ctflexrec,
#endif
	Datacompression,
	DatacompressionLevel,
	DatacompressionStrategy,
	DatacompressionType,
	Datafilesuffix,
#ifdef ctFeatRTG_DATAFILESUFFIX_APPEND
	DatafilesuffixAppend,
	DatafilesuffixCasesensitive,
#endif
	Delcurrent,
	Detectlock,
	Encryption,
	EncryptionType,
	Extfhexit,
	Fastdup,
	Filecopy,
	FilecopyOverwrite,
	Filepool,
	FilepoolSize,
	Fileversion,
	Forcedelete,
	Hugefile,
	Inpool,
	InMissingpool,
	Keycheck,
	Keycompression,
	KeycompressionVlennod,
	KeycompressionLeading,
	KeycompressionPadding,
	KeycompressionRle,
	Ignorelock,
	Indexfilesuffix,
	Locktimeout,
	Locktype,
	Log,
	LogError,
	LogErrorMissingfile,
	LogErrorAtend,
	LogErrorLocked,
	LogErrorDuplicate,
	LogErrorNotfound,
	LogErrorUndefined,
	LogDebug,
	LogDebugBatchaddition,
	LogDebugCluster,
	LogDebugConfig,
	LogDebugConfigFull,
	LogDebugExtfh,
	LogDebugExtfhFcd,
	LogDebugFile,
	LogDebugGeneric,
	LogDebugLock,
	LogDebugPrefetch,
	LogDebugSqlize,
	LogDebugStartonread,
	LogDebugSwitcher,
	LogDebugTransaction,
	LogInfo,
	LogWarning,
	LogProfile,
	LogFile,
#ifdef ctFeatRTG_LOGWHOFORMAT
	LogWhoformat,
#endif
	Map,
	MapName,
	MapDir,
	Maxlencheck,
#ifdef ctFeatRTG_MAXSIZE
	Maxsize,
#endif
	Memoryfile,
	MemoryfilePersist,
	MissingPool,
	MissingPoolSize,
	Normalize,
	NormalizeDrive,
	NormalizeDriveForce,
	NormalizeDriveRelative,
	NormalizeRelative,
	NormalizeSep,
	Optimisticadd,
	Permission,
	PermissionGroup,
	PermissionGroupDef,
	PermissionGroupDelete,
	PermissionGroupRead,
	PermissionGroupWrite,
	PermissionOwner,
	PermissionOwnerDef,
	PermissionOwnerDelete,
	PermissionOwnerRead,
	PermissionOwnerWrite,
	PermissionWorld,
	PermissionWorldDef,
	PermissionWorldDelete,
	PermissionWorldRead,
	PermissionWorldWrite,
	Prefetch,
	PrefetchAllowwriters,
	PrefetchRecords,
	PrefetchTtl,
	Readonly,
#ifdef ctFeatRTG_RECORD_FOOTER
	Recordcheck,
#endif
	Retrylock,
	Rowid,
	RowidSize,
	Rpc,
	RpcCrc,
	Runitlockdetect,
	RunitlockdetectAnyunlock,
#ifdef ctFeatRTG_NORUNITLOCKDETECT_UNLOCKALL
	RunitlockdetectUnlockall,
#endif
	Scancache,
	Skiplock,
	Smartcopy,
	SmartcopyRaw,
	SmartcopyRpc,
	Sqlize,
	SqlizeConvention,
	SqlizeDatabase,
	SqlizeOwner,
	SqlizePassword,
	SqlizePrefix,
	SqlizePublic,
	SqlizeRules,
	SqlizeSymbolic,
	SqlizeXdd,
	SqlizeXfd,
	Startonread,
	Temporary,
	Transaction,
	TransactionDeferautocommit,
	TransactionLogging,
	TransactionFileoperations,
#ifdef ctFeatCOMPACT_TRUNCATE
	Truncateifexist,
#endif
	Trxholdslocks,
	Writethru
} CONFSETTINGID;

typedef enum
{
	None,
	Bool,
	Integer,
	String,
	Custom,
	Deprecated
} CONFSETTINGTYPE;

typedef struct
{
	cpTEXT	keyword;
	CONFSETTINGID	id;
	CONFSETTINGTYPE	type;
} CONFSETTING, ctMEM* pCONFSETTING, ctMEM* ctMEM* ppCONFSETTING;

#endif /* ~_CTCBCONF_H_ */

/* end of ctcbconf.h */
