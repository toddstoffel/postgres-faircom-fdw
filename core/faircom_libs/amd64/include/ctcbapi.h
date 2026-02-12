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

#ifndef _CTCBAPI_H_
#define _CTCBAPI_H_

#include "ctcboptn.h"
#include "ctdbport.h"
#include "ctcbtype.h"
#include "ctcbthrd.h"

#include "ctrepl.h" /* for ctSERVER_NOT_ACTIVE_ERROR */

/* configuration types */
typedef enum CONFTYPE {
	CONF_NONE = 0,
	CONF_OLD = 1,
	CONF_NEW = 2
} CONFTYPE;
/* file extensions */
typedef enum FILEXTYPE {
	CT_DAT = 0,
	CT_IDX = 1
} FILEXTYPE;
/* transaction operations */
typedef enum TRANOP {
	CT_COMMIT = 1,
	CT_ROLLBACK = 2
} TRANOP;
/* path comparison */
typedef enum PATHCMP {
	EXACT = 0,
	PARTOF = 1
} PATHCMP;
/* image operations */
typedef enum IMAGEOP {
	CTMAKE_INVALID = 0,
	CTMAKE_FILE2IMG = 1,
	CTMAKE_IMG2MAKE = 2,
	CTMAKE_MAKE2IMG = 3,
	CTMAKE_IMG2RBL = 4,
	CTMAKE_IMGCHECK = 5,
	CTMAKE_IMGADDIDX = 6,
	CTMAKE_IMGIDX2MAKE = 7
} IMAGEOP;
/* copy modes */
typedef enum COPYMODE {
	CTCOPY_COPY_REPLACE = 0,
	CTCOPY_CLONE_REPLACE,
	CTCOPY_FILECOPY,
	CTCOPY_FILECOPY_REPLACE,
	CTCOPY_UPGRADE,
	CTCOPY_COPY,
	CTCOPY_CLONE
} COPYMODE;
/* key segment mismatches */
typedef enum KEYMISMATCH {
	KEYMISMATCH_KEYSEG = -1,
	KEYMISMATCH_KEYTYP = -2,
	KEYMISMATCH_SEGOFF = 1,
	KEYMISMATCH_SEGLEN = 2,
	KEYMISMATCH_SEGTYP = 3,
	KEYMISMATCH_SEGMOD = 4
} KEYMISMATCH;
/* key segment checks */
typedef enum KEYCHECK {
	KEYCHECK_SEGOFF = 0x01,
	KEYCHECK_SEGLEN = 0x02,
	KEYCHECK_SEGTYP = 0x04,
	KEYCHECK_SEGMOD = 0x08
} KEYCHECK;

typedef struct {
	UCOUNT idx;
	UCOUNT off;
	UCOUNT len;
} AUTOINC;

struct tagCT_FILE {
	HANDLEID	handleid;
	TEXT	name[CT_PATH_LEN];
	TEXT	rname[CT_PATH_LEN];
#ifdef ctFeatMAXFILE32
	FILNO	filno;
#else
	COUNT	filno;
#endif
	LONG	drvfilemode;
	LONG	drvfilemod2;
	LONG	current_offset;  /* current record pointer */
	LONG	current_offseth; /* current record pointer (highword) */
	COUNT	context;
	COUNT	io_context;
	COUNT	act_context;
	COUNT	curr_index;
	COUNT	instance;
	ULONG	reconnected;
	LONG	got_filtyp;
	LONG	got_physiz;
	LONG	got_iphysiz;
	LONG	got_physizh;
	LONG	got_iphysizh;
	LONG	got_filmod;
	LONG	got_flmode2;
	LONG	got_flmode3;
	LONG	got_cmptype;
	LONG	got_ifilmod;
	LONG	got_numrecords;
	LONG	got_delrecords;
	ULONG	got_segsiz;
	LONG	got_segmax;
	TEXT	got_segmented;
	TEXT	got_encrypted;
#ifdef ctFeatRTG_MAXSIZE
	LONG	got_mxfilz;
	LONG	got_mxfilzh;
#endif
	TEXT	is_huge;
	TEXT	signconv;
	ULONG	xmllen;
	ULONG	minlen;
	ULONG	maxlen;
#ifdef ctFeatRTG_RECORD_FOOTER
	ULONG	minsiz;
	ULONG	maxsiz;
#endif
#ifdef ctFeatRTG_NODESIZE
	UCOUNT	nodesize;
#endif
	COUNT	nkeys;
	COUNT	maxkeysize;
	pTEXT	filter;
	pTEXT	filter_mask;
	VRLEN	filter_size;
	pTEXT	fetptr;          /* prefetch buffer pointer */
	VRLEN	fetsiz;          /* prefetch buffer size */
	pTEXT	fetpos;          /* current record position in prefetch buffer */
	ULONG	fetret;          /* number of records to process in prefetch buffer */
	COUNT	fetcnx;          /* context in which prefetch was executed */
	LONG8	fetttl;          /* prefetch time to live */
	LONG8	fettime;         /* time of prefetch */
	pTEXT	batptr;          /* batchadd buffer pointer */
	ULONG	batsiz;          /* batchadd buffer size */
	ULONG	batlen;          /* batchadd buffer length */
	ULONG	batrdy;          /* number of records to process in batchadd buffer */
	ULONG	batadd;          /* number of records successfully added */
	LONG	blockfact;
	LONG	prealloc;
	LONG	extension;
	TEXT	compress;
	TEXT	comment[CT_COMMENT_LEN];
	TEXT	trans[256];
	TEXT	fileformat;
	UTEXT	rechdr;
	UTEXT	srlsegsiz;
	NINT	openthrd;
	pCONFFILE	conf;
	CT_INSTANCE	*ctinst;
	AUTOINC	autoinc;
	ULONG	last_recnum;     /* record number of last record accessed either read or write */
	LONG	last_offset;     /* record offset of last record accessed either read or write for EXTFH */
	LONG	last_offseth;    /* record offset of last record accessed either read or write for EXTFH (highword) */
	LONG	last_retrvd;     /* record offset of last record read */
	LONG	last_retrvdh;    /* record offset of last record read (highword) */
	LONG	env;
	LONG	io_env;
	LONG	env2;
	LONG	trnum;
	LONG	trnumh;
	pTEXT	recptr;
	TEXT	delcurrent;
	TEXT	delcurrent_found;
	TEXT	delcurrent_key[CT_MAX_KEY_SIZE];
	ULONG8	delcurrent_trnid;
#ifdef ctFeatRTG_KEYS
	CT_OPEN3	ct_open;
#else /* ~ctFeatRTG_KEYS */
	CT_OPEN2	ct_open;
#endif /* ~ctFeatRTG_KEYS */
	TEXT	filepool_reopen;
	TEXT	startread;
	COUNT	startread_keyno;
	COUNT	startread_keylen;
	COUNT	startread_keysiz;
	LONG	startread_keytype;
	TEXT	startread_key[MAXLEN];
#ifdef ctFeatRTG_FILEPASSWORD
	TEXT	word[CT_FLPW_LEN];
#endif
	TEXT ctbtrv;
	TEXT resver;
	VRLEN	var_len;
	/*
	**	var[] must be large enough to have
	**
	ifdef ctFeatRTG_KEYS
	**	RTGKEY[nkeys];
	**	RTGSEG[nsegs];
	else
	**	CT_KEY[nkeys];		this is static !!!
	endif
	**	CT_COMM_INPUT
	**	key[maxkeysize];
	**	CT_COMM_OUTPUT
	**	vkey[maxkeysize];
	**	rec[maxlen+rechdr];
	**
	*/
	TEXT	var[MEMU];
};

#define ACU_VISION      1
#define ACU_CTREE       2

#ifdef _WIN32
#define CT_OSDIR '\\'
#else
#define CT_OSDIR '/'
#endif
#define CT_DIRSEPW '\\'
#define CT_DIRSEPU '/'

#define INIT_BUFS  ((COUNT)1280) /* 10 MB */
#define INIT_NFILS ((COUNT)32)
#define INIT_SECT  ((COUNT)64) /* 8 KB */

#define INS_DECLARED        0x0001
#define INS_ACTIVE          0x0002
#define INS_CLOSED          0x0004
#define INS_READ_ONLY       0x0008
#define INS_CLASH           0x0010
#define INS_CORRUPT         0x0020
#define INS_CLIENT          0x0040
#define INS_LOCAL           0x0080
#define INS_REDIR           0x0100
#define INSTANCE_DECLARED(c,i) (c->instance[i].status & INS_DECLARED)
#define INSTANCE_ACTIVE(c,i) (c->instance[i].status & INS_ACTIVE)
#define INSTANCE_CLOSED(c,i) (c->instance[i].status & INS_CLOSED)
#define INSTANCE_READ_ONLY(c,i) (c->instance[i].status & INS_READ_ONLY)
#define INSTANCE_CLASH(c,i) (c->instance[i].status & INS_CLASH)
#define INSTANCE_CORRUPT(c,i) (c->instance[i].status & INS_CORRUPT)
#define INSTANCE_CLIENT(c,i) (c->instance[i].status & INS_CLIENT)
#define INSTANCE_LOCAL(c,i) (c->instance[i].status & INS_LOCAL)
#define INSTANCE_REDIR(c,i) (c->instance[i].status & INS_REDIR)

#define CT_LOCK_WAIT         0x0001
#define CT_FAST_DELETE       0x0002
#define CT_READ_LOCK_TEST    0x0004
#define CT_RETURN_LOCKED_REC 0x0008
#define CT_INTERNAL_LOCKS    0x0010
#define CT_EXTRA_KEYS_OK     0x0020

#define DISCONNECTED_ERR(err) (err != NO_ERROR && (err == SERVER_FAILOVER_ERR || ctSERVER_NOT_ACTIVE_ERROR(err) || ctSERVER_FAILOVER_RETRY_CONNECT_ERROR(err)))
#define RECONNECT_ENABLED(conn, inst) (conn->instance[inst].reconnect)
#define DISCONNECTED_FILE(conn, ct) (INSTANCE_CLOSED(conn, ct->instance) || \
							conn->instance[ct->instance].reconnected != ct->reconnected)

/*
**	client side
*/
cbDLLexport CONFTYPE ctl_initconf(pRTGENV env, cpTEXT path, ppRTGCONF pconf, pCT_ERROR err, pTEXT errmsg);
cbDLLexport pCT_INIT ctl_init(pRTGENV env, pRTGCONF conf, pCT_ERROR _err, pTEXT _errmsg, pTEXT nodename);
cbDLLexport VOID ctl_exitcnf(pRTGCONF conf);
cbDLLexport COUNT ctl_exitinst(CT_INIT *conn, COUNT ii);
cbDLLexport VOID ctl_exit(CT_INIT *conn, ppRTGCONF conf);

#ifdef ctFeatRTG_SESSIONMEM
pVOID ctl_sessmem(TEXT in, pCT_INIT conn, ULONG size);
#define rtgGETINPUTMEM(conn,size) ctl_sessmem(YES,conn,size)
#define rtgGETOUTPUTMEM(conn,size) ctl_sessmem(NO,conn,size)
#define rtgPUTINPUTMEM(conn) ctl_sessmem(YES,conn,0xffffffff)
#define rtgPUTOUTPUTMEM(conn) ctl_sessmem(NO,conn,0xffffffff)
#endif /* ctFeatRTG_SESSIONMEM */

NINT ctl_rpc(pCONFFILE file);
NINT ctl_compatible(pCT_INIT conn, COUNT instnum, pCONFFILE file);
NINT ctl_version(pCT_INIT conn, COUNT instnum);

#ifdef ctFeatRTG_KEYS
NINT ctl_chkkeyseg(pRTGKEY lk, pRTGSEG ls, pRTGKEY rk, pRTGSEG rs, UTEXT check);
VOID ctl_mrgkeyseg(pRTGKEY sk, pRTGSEG ss, pRTGKEY dk, pRTGSEG ds);
#ifdef ctFeatRTG_FILEPASSWORD
cbDLLexport NINT ctl_make(pCT_INIT conn, pCT_OPEN3 make, pRTGKEY keys, cpTEXT passwd);
pCT_FILE ctl_open2(pCT_INIT conn, pCT_OPEN3 op, pRTGKEY keys, pTEXT name, cpTEXT passwd, COUNT inst, pCONFFILE file);
#ifdef ctFeatRTG_SQLINFO_ON_OPEN
cbDLLexport pCT_FILE ctl_open1(pCT_INIT conn, pCT_OPEN3 op, pRTGKEY keys, cpTEXT passwd, pTEXT exdd);
pTEXT ctl_xfd2xdd(pCT_INIT conn, cpTEXT path, pCT_ERROR err, pTEXT errmsg, size_t errmsgsiz);
NINT ctl_sqlize2(pCT_INIT conn, pCT_OPEN3 op, pTEXT name, cpTEXT passwd, COUNT inst, pCONFFILE file, TEXT refresh, pTEXT exdd);
#endif /* ctFeatRTG_SQLINFO_ON_OPEN */
cbDLLexport pCT_FILE ctl_open(pCT_INIT conn, pCT_OPEN3 op, pRTGKEY keys, cpTEXT passwd);
cbDLLexport NINT ctl_sqlize(pCT_INIT conn, pCT_OPEN3 op, pTEXT name, cpTEXT passwd, COUNT inst, pCONFFILE file, TEXT refresh);
#else /* ~ctFeatRTG_FILEPASSWORD */
cbDLLexport NINT ctl_make(pCT_INIT conn, pCT_OPEN3 make, pRTGKEY keys);
cbDLLexport pCT_FILE ctl_open(pCT_INIT conn, pCT_OPEN3 op, pRTGKEY keys);
pCT_FILE ctl_open2(pCT_INIT conn, pCT_OPEN3 op, pRTGKEY keys, pTEXT name, COUNT inst, pCONFFILE file);
cbDLLexport NINT ctl_sqlize(pCT_INIT conn, pCT_OPEN3 op, pTEXT name, COUNT inst, pCONFFILE file, TEXT refresh);
#endif /* ~ctFeatRTG_FILEPASSWORD */
NINT ctl_addidx(pCT_INIT conn, pCT_FILE ct, pRTGKEY key, UCOUNT nkeys, pTEXT trans, TEXT update);
#else /* ~ctFeatRTG_KEYS */
cbDLLexport NINT ctl_make(pCT_INIT conn, pCT_OPEN2 make, pCT_KEY2 keys);
NINT ctl_chkkeyseg(pCT_KEY2 lk, pCT_SEG2 ls, pCT_KEY2 rk, pCT_SEG2 rs, UTEXT check);
VOID ctl_mrgkeyseg(pCT_KEY2 sk, pCT_SEG2 ss, pCT_KEY2 dk, pCT_SEG2 ds);
cbDLLexport pCT_FILE ctl_open(pCT_INIT conn, pCT_OPEN2 op, pCT_KEY2 keys);
pCT_FILE ctl_open2(pCT_INIT conn, pCT_OPEN2 op, pCT_KEY2 keys, pTEXT name, COUNT inst, pCONFFILE file);
cbDLLexport NINT ctl_sqlize(pCT_INIT conn, pCT_OPEN2 op, pTEXT name, COUNT inst, pCONFFILE file, TEXT refresh);
NINT ctl_addidx(pCT_INIT conn, pCT_FILE ct, pCT_KEY2 key, UCOUNT nkeys, pTEXT trans);
#endif /* ~ctFeatRTG_KEYS */
cbDLLexport NINT ctl_close(CT_INIT *, CT_FILE *);
NINT ctl_close2(pCT_INIT conn, pCT_FILE ct, TEXT callsrv);
NINT ctl_info(pCT_INIT conn, pCT_FILE ct);
cbDLLexport NINT ctl_sqlinfo(pCT_INIT conn, pCT_FILE ct, pTEXT xml, TEXT sign, pTEXT rules);
cbDLLexport NINT ctl_sqllink(pCT_INIT conn, cpTEXT name, cpTEXT dbname, cpTEXT adminpw, TEXT unlink, cpTEXT symname, cpTEXT tblowner, TEXT grantpub, cpTEXT prefix, TEXT logerror, TEXT refresh, TEXT skipmissing);
#ifdef ctFeatRTG_ALTERSCHEMA
#ifdef ctFeatRTG_ALTERSCHEMAv2
cbDLLexport NINT ctl_alter(pCT_INIT conn, cpTEXT name, cpTEXT word, NINT action, pTEXT xml, pTEXT rules);
#else
cbDLLexport NINT ctl_alter(pCT_INIT conn, pCT_FILE ct, NINT action, pTEXT xml, pTEXT rules);
#endif
#endif
#ifdef ctFeatRTG_FILEPASSWORD
cbDLLexport NINT ctl_fileword(pCT_INIT conn, pCT_FILE ct, cpTEXT word, TEXT nopass);
#ifdef ctFeatRTG_PARTITION
NINT ctl_partition(pCT_INIT conn, cpTEXT name, cpTEXT word, cpTEXT rule, NINT index);
#endif /* ctFeatRTG_PARTITION */
#endif /* ctFeatRTG_FILEPASSWORD */
cbDLLexport NINT ctl_write(pCT_INIT conn, pCT_FILE ct, pTEXT rec, ULONG len, LONG mode);
NINT ctl_write2(pCT_INIT conn, pCT_FILE ct, pTEXT buffer, ULONG buflen, LONG addmode);
cbDLLexport COUNT ctl_buildkey(CT_INIT *conn, CT_FILE *, pTEXT rec, COUNT keyno, pTEXT key, COUNT maxkeysize);
cbDLLexport NINT ctl_start(pCT_INIT conn, pCT_FILE ct, LONG mode, LONG mode2, STARTOP op, COUNT keyno, pTEXT rec, pTEXT key, COUNT keysize);
#ifdef ctFeatRTG_GET_EXTENDED
cbDLLexport NINT ctl_read(pCT_INIT conn, pCT_FILE ct, LONG mode, LONG mode2, COUNT keyno, pTEXT rec, ULONG len, pTEXT key, pRTGRSET rset);
cbDLLexport NINT ctl_nextprev(pCT_INIT conn, pCT_FILE ct, LONG mode, LONG mode2, SEQOP op, pTEXT rec, ULONG len, pRTGRSET rset);
#else /* ~ctFeatRTG_GET_EXTENDED */
cbDLLexport NINT ctl_read(pCT_INIT conn, pCT_FILE ct, LONG mode, LONG mode2, COUNT keyno, pTEXT rec, ULONG len, pTEXT key);
cbDLLexport NINT ctl_nextprev(pCT_INIT conn, pCT_FILE ct, LONG mode, LONG mode2, SEQOP op, pTEXT rec, ULONG len);
#endif /* ~ctFeatRTG_GET_EXTENDED */
#ifdef ctFeatRTG_RECORDSET
cbDLLexport NINT ctl_rewdel(pCT_INIT conn, pCT_FILE ct, LONG keymode, pTEXT rec, ULONG len, pRTGRSET rset);
#else /* ~ctFeatRTG_RECORDSET */
cbDLLexport NINT ctl_rewdel(pCT_INIT conn, pCT_FILE ct, LONG keymode, pTEXT rec, ULONG len);
#endif /* ~ctFeatRTG_RECORDSET */
NINT ctl_startinsttransaction(CT_INIT *conn, COUNT ii);
cbDLLexport NINT ctl_starttransaction(CT_INIT *conn, TEXT excl);
#ifdef ctFeatRTG_TWOPHASE_COMMIT
NINT ctl_preptransaction(pCT_INIT conn);
#endif /* ctFeatRTG_TWOPHASE_COMMIT */
NINT ctl_endinsttransaction(CT_INIT *conn, TRANOP mode, COUNT ii, TEXT xplict, TEXT keeplocks);
cbDLLexport NINT ctl_endtransaction(CT_INIT *conn, TRANOP mode, TEXT keeplocks);
NINT ctl_execute(CT_INIT *conn, pTEXT cmd);
#ifdef ctFeatRTG_FILEPASSWORD
cbDLLexport NINT ctl_execos(pCT_INIT conn, NINT argc, ppTEXT argv, FILE* fp, cpTEXT word);
#else /* ~ctFeatRTG_FILEPASSWORD */
cbDLLexport NINT ctl_execos(pCT_INIT conn, NINT argc, ppTEXT argv, FILE* fp);
#endif /* ~ctFeatRTG_FILEPASSWORD */

NINT ctl_getinstance(pCT_INIT conn, pRTGCONF conf, COUNT instnum);
cbDLLexport NINT ctl_setinstance(pCT_INIT conn, COUNT instnum);
NINT ctl_reginst(pCT_INSTANCE inst);
TEXT ctl_getdirsep(pTEXT name, size_t len);
cbDLLexport pTEXT ctl_filename(pTEXT path);
VOID ctl_dirname(pTEXT path, pTEXT dir, VRLEN dirlen);
#define ctl_basename(p) ctl_filename(p)
pTEXT ctl_replsep(pTEXT path, TEXT sep);
cbDLLexport NINT ctl_normalize(pRTGCONF conf, cpTEXT filepath, pTEXT outpath, size_t outpathlen);
cbDLLexport pCONFFILE ctl_conffile(pRTGCONF conf, cpTEXT name);
cbDLLexport pCONFFILE ctl_conffile2(pRTGCONF conf, cpTEXT name, TEXT normalize);
cbDLLexport pCONFFILE ctl_conffile3(pRTGCONF conf, cpTEXT name, TEXT filetype, TEXT normalize);
cbDLLexport pCONFINST ctl_confinst(pRTGCONF conf, COUNT inst);
#ifdef ctFeatRTG_GETFILEINST
cbDLLexport COUNT ctl_getfileinst(CT_INIT *conn, cpTEXT iname, ppCONFFILE conf);
#else
cbDLLexport COUNT ctl_getfileinst(CT_INIT *conn, ppTEXT name);
cbDLLexport COUNT ctl_getfileinst2(CT_INIT *conn, ppTEXT name, ppCONFFILE conf);
COUNT ctl_getfileinst_no(CT_INIT *conn, ppTEXT name, ppCONFFILE file);
#endif
NINT ctl_expandname(CT_INIT *conn, pTEXT format, pTEXT dest, NINT destlen, pTEXT filepath);
NINT ctl_mapfile(CT_INIT *conn, pCONFFILE file, pTEXT oname, size_t onamelen);

#ifdef ctFeatRTG_FILEPASSWORD
cbDLLexport NINT ctl_check(pCT_INIT conn, pTEXT name, cpTEXT word, COUNT chkkey, TEXT full, pVFYMSGCBFNC fnMessage, pVFYPRGCBFNC fnProgress, pVFYERRCBFNC fnError);
#ifdef ctFeatRTG_RETRYOPEN
cbDLLexport NINT ctl_remove(pCT_INIT conn, pTEXT name, cpTEXT word, TEXT retryopen);
NINT ctl_rename(pCT_INIT conn, pTEXT namefrom, cpTEXT word, pTEXT nameto, TEXT overwrite, TEXT retryopen);
#else /* ~ctFeatRTG_RETRYOPEN */
cbDLLexport NINT ctl_remove(pCT_INIT conn, pTEXT name, cpTEXT word);
NINT ctl_rename(pCT_INIT conn, pTEXT namefrom, cpTEXT word, pTEXT nameto, TEXT overwrite);
#endif /* ~ctFeatRTG_RETRYOPEN */
NINT ctl_copy(pCT_INIT conn, pTEXT namefrom, pTEXT nameto, cpTEXT word, COPYMODE mode);
#else /* ~ctFeatRTG_FILEPASSWORD */
cbDLLexport NINT ctl_check(pCT_INIT conn, pTEXT name, COUNT chkkey, TEXT full, pVFYMSGCBFNC fnMessage, pVFYPRGCBFNC fnProgress, pVFYERRCBFNC fnError);
cbDLLexport NINT ctl_remove(pCT_INIT conn, pTEXT name);
NINT ctl_rename(pCT_INIT conn, pTEXT namefrom, pTEXT nameto, TEXT overwrite);
NINT ctl_copy(pCT_INIT conn, pTEXT namefrom, pTEXT nameto, COPYMODE mode);
#endif /* ~ctFeatRTG_FILEPASSWORD */
NINT ctl_unlock(pCT_INIT conn, pCT_FILE file);
NINT ctl_unlockrec(pCT_INIT conn, pCT_FILE file, LONG offset, LONG offset_h);
#ifdef ctFeatRTG_ACCESS
NINT ctl_access(pCT_INIT conn, cpTEXT path, LONG mode, pTEXT exist, pLONG8 size, pTEXT ctime);
#endif

cbDLLexport pVOID ctl_config(pRTGENV env, CT_INIT *conn, NINT op, pRTGCONF conf, pVOID item, NINT argc, cpTEXT *argv);
#ifdef NOT_YET
NINT ctl_confsetinstattr(pRTGCONF conf, pCONFINST institem, cpTEXT option, cpTEXT value);
#endif
pCONFINST ctl_confaddinst(CT_INIT *conn, INTFTYPE intf, pRTGCONF conf, INSTTYPE instype, cpTEXT attr[], UINT nattr);
NINT ctl_confdelinst(pRTGCONF conf, pCONFINST item);
pCONFFILE ctl_confaddfile(CT_INIT *conn, INTFTYPE intf, pRTGCONF conf, pCONFINST institem, cpTEXT name, cpTEXT dir, cpTEXT priority, cpTEXT casesensitive, cpTEXT filetype);
pCONFFILE ctl_confaddfile2(CT_INIT *conn, INTFTYPE intf, pRTGCONF conf, pCONFINST institem, TEXT sort, cpTEXT name, cpTEXT dir, cpTEXT priority, cpTEXT casesensitive, cpTEXT filetype);
NINT ctl_confdelfile(pRTGCONF conf, pCONFFILE item);
NINT ctl_confsetinstopt(pRTGCONF conf, pCONFINST institem, cpTEXT option, cpTEXT value);
NINT ctl_confsetfileopt(pRTGCONF conf, pCONFFILE fileitem, cpTEXT option, cpTEXT value);

cbDLLexport NINT ctl_connect(CT_INIT *conn, COUNT instance);
cbDLLexport ULONG ctl_logons(CT_INIT *conn);

cbDLLexport NINT ctl_initthrd(VOID);
cbDLLexport NINT ctl_termthrd(VOID);
#ifdef ctThrds
NINT ctl_getthrdhandle(VOID);
NINT ctl_attachthrd(VOID);
NINT ctl_detachthrd(VOID);
#endif /* ctThrds */
NINT ctl_inimtx(MUTEX *mtx, CT_ERROR *err);
NINT ctl_getmtx(MUTEX *mtx, CT_ERROR *err);
NINT ctl_relmtx(MUTEX *mtx, CT_ERROR *err);
NINT ctl_clsmtx(MUTEX *mtx, CT_ERROR *err);

CT_FILEPOOL *ctl_initFP(CT_INIT *conn, UCOUNT size);
#ifdef ctFeatRTG_KEYS
pVOID ctl_findFP(pCT_INIT conn, pCONFFILE conffile, CT_FILEPOOL* Handle, pCT_OPEN3 ctopen, pTEXT name);
#else /* ~ctFeatRTG_KEYS */
pVOID ctl_findFP(pCT_INIT conn, pCONFFILE conffile, CT_FILEPOOL *Handle, pCT_OPEN2 ctopen, pTEXT name);
#endif /* ~ctFeatRTG_KEYS */
COUNT ctl_addFP(CT_INIT *conn, CT_FILEPOOL *Handle, pCONFFILE conf, pVOID ctfile);
VOID ctl_delFP(CT_INIT *conn, pCONFFILE conffile, CT_FILEPOOL *Handle, pTEXT name);
VOID ctl_closeFP(CT_INIT *conn, pCT_FILE ct);
VOID ctl_exitFP(CT_INIT *conn, CT_FILEPOOL *Handle);

cbDLLexport NINT ctl_forcelocal(CT_INIT *);

typedef enum {
	rtgMAKE,
	rtgOPEN,
	rtgCLOSE,
	rtgSTART,
	rtgREAD,
	rtgNEXTPREV,
	rtgWRITE,
	rtgREWDEL,
	rtgSTARTTRANSACTION,
	rtgENDTRANSACTION,
	rtgREMOVE,
	rtgRENAME,
	rtgCOPY,
	rtgUNLOCK,
	rtgUNLOCKREC,
	rtgADDIDX,
	rtgSQLINFO,
	rtgSQLLINK,
} RTGOP;

typedef union {
	pVOID p;
	LONG l;
	COUNT c;
	TEXT b;
} RTGPARM;

cbDLLexport NINT rtgCALL(RTGOP opcode, CT_INIT *conn, pCT_FILE ctfile, RTGPARM parm[]);

#ifdef ctFeatRTG_KEYS
#ifdef ctFeatRTG_FILEPASSWORD
cbDLLexport NINT rtg_make(pCT_INIT conn, pCT_OPEN3 op, pRTGKEY key, pTEXT word);
cbDLLexport pCT_FILE rtg_open(pCT_INIT conn, pCT_OPEN3 op, pRTGKEY key, pTEXT word);
#else /* ~ctFeatRTG_FILEPASSWORD */
cbDLLexport NINT rtg_make(pCT_INIT conn, pCT_OPEN3 op, pRTGKEY key);
cbDLLexport pCT_FILE rtg_open(pCT_INIT conn, pCT_OPEN3 op, pRTGKEY key);
#endif /* ~ctFeatRTG_FILEPASSWORD */
NINT rtg_addidx(pCT_INIT conn, pCT_FILE ct, pRTGKEY key, UCOUNT nkeys, pTEXT trans, TEXT update);
#else /* ~ctFeatRTG_KEYS */
cbDLLexport NINT rtg_make(pCT_INIT conn, pCT_OPEN2 op, pCT_KEY2 key);
cbDLLexport pCT_FILE rtg_open(pCT_INIT conn, pCT_OPEN2 op, pCT_KEY2 key);
NINT rtg_addidx(pCT_INIT conn, pCT_FILE ct, pCT_KEY2 key, UCOUNT nkeys, pTEXT trans);
#endif /* ~ctFeatRTG_KEYS */
cbDLLexport NINT rtg_close(CT_INIT *conn, CT_FILE *ct);
cbDLLexport NINT rtg_start(CT_INIT *conn, CT_FILE *ct, LONG mode, LONG mode2, STARTOP op, COUNT keyno, pTEXT rec, pTEXT key, COUNT keysize);
#ifdef ctFeatRTG_GET_EXTENDED
cbDLLexport NINT rtg_read(pCT_INIT conn, pCT_FILE ct, LONG mode, LONG mode2, COUNT keyno, pTEXT rec, ULONG len, pTEXT key, pVOID xtd);
cbDLLexport NINT rtg_nextprev(pCT_INIT conn, pCT_FILE ct, LONG mode, LONG mode2, SEQOP op, pTEXT rec, ULONG len, pVOID xtd);
#else /* ~ctFeatRTG_GET_EXTENDED */
cbDLLexport NINT rtg_read(pCT_INIT conn, pCT_FILE ct, LONG mode, LONG mode2, COUNT keyno, pTEXT rec, ULONG len, pTEXT key);
cbDLLexport NINT rtg_nextprev(pCT_INIT conn, pCT_FILE ct, LONG mode, LONG mode2, SEQOP op, pTEXT rec, ULONG len);
#endif /* ~ctFeatRTG_GET_EXTENDED */
#ifdef ctFeatRTG_RECORDSET
cbDLLexport NINT rtg_rewdel(pCT_INIT conn, pCT_FILE ct, LONG keymode, pTEXT rec, ULONG len, pVOID xtd);
#else /* ~ctFeatRTG_RECORDSET */
cbDLLexport NINT rtg_rewdel(pCT_INIT conn, pCT_FILE ct, LONG keymode, pTEXT rec, ULONG len);
#endif /* ~ctFeatRTG_RECORDSET */
cbDLLexport NINT rtg_write(CT_INIT *conn, CT_FILE *ct, pTEXT rec, ULONG len, LONG mode);
cbDLLexport NINT rtg_starttransaction(CT_INIT *conn, TEXT excl);
cbDLLexport NINT rtg_endtransaction(CT_INIT *conn, TRANOP op, TEXT keeplocks);
#ifdef ctFeatRTG_FILEPASSWORD
#ifdef ctFeatRTG_RETRYOPEN
NINT rtg_remove(pCT_INIT conn, pTEXT name, pTEXT word, TEXT retryopen);
NINT rtg_rename(pCT_INIT conn, pTEXT namefrom, pTEXT word, pTEXT nameto, TEXT overwrite, TEXT retryopen);
#else /* ~ctFeatRTG_RETRYOPEN */
NINT rtg_remove(pCT_INIT conn, pTEXT name, pTEXT word);
NINT rtg_rename(pCT_INIT conn, pTEXT namefrom, pTEXT word, pTEXT nameto, TEXT overwrite);
#endif /* ~ctFeatRTG_RETRYOPEN */
NINT rtg_copy(pCT_INIT conn, pTEXT namefrom, pTEXT nameto, pTEXT word, COPYMODE mode);
#else /* ~ctFeatRTG_FILEPASSWORD */
NINT rtg_remove(pCT_INIT conn, pTEXT name);
NINT rtg_rename(pCT_INIT conn, pTEXT namefrom, pTEXT nameto, TEXT overwrite);
NINT rtg_copy(pCT_INIT conn, pTEXT namefrom, pTEXT nameto, COPYMODE mode);
#endif /* ~ctFeatRTG_FILEPASSWORD */
NINT rtg_unlock(CT_INIT *conn, CT_FILE *file);
NINT rtg_unlockrec(CT_INIT *conn, CT_FILE *file, LONG offset, LONG offset_h);
cbDLLexport NINT rtg_sqlinfo(CT_INIT *conn, CT_FILE *ct, pTEXT xml, TEXT sign, pTEXT rules);
cbDLLexport NINT rtg_sqllink(CT_INIT *conn, pTEXT name, pTEXT dbname, pTEXT adminpw, TEXT unlink, pTEXT symname, pTEXT tblowner, TEXT grantpub, pTEXT prefix, TEXT logerror, TEXT refresh, TEXT skipmissing);

#ifdef USE_API_ENTRYPOINT
#define ctl_make rtg_make
#define ctl_open rtg_open
#define ctl_close rtg_close
#define ctl_start rtg_start
#define ctl_read rtg_read
#define ctl_nextprev rtg_nextprev
#define ctl_write rtg_write
#define ctl_rewdel rtg_rewdel
#define ctl_starttransaction rtg_starttransaction
#define ctl_endtransaction rtg_endtransaction
#define ctl_remove rtg_remove
#define ctl_rename rtg_rename
#define ctl_copy rtg_copy
#define ctl_unlock rtg_unlock
#define ctl_unlockrec rtg_unlockrec
#define ctl_addidx rtg_addidx
#define ctl_sqlinfo rtg_sqlinfo
#define ctl_sqllink rtg_sqllink
#endif /* USE_API_ENTRYPOINT */


#endif /* ~_CTCBAPI_H_ */

/* end of ctcbapi.h */
