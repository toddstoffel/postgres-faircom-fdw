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

#ifndef _CTCBMRSH_H_
#define _CTCBMRSH_H_

#include "ctcboptn.h"
#include "ctcust.h"
#include "ctcberr.h"
#include "ctcbtype.h"
#include "ctcbapi.h"
#include "ctcbstdr.h"

#define CT_DO_CHECKSUM      'y'
#define CT_DONT_CHECKSUM    'n'

#define VARFEAT_STARTONREAD 0x01
#define VARFEAT_RECORDSET   0x02

/*
**	client communication - local & server
*/

#define CT_INPUT_COMMON1 \
	/* !! THIS STRUCTURE MUST NOT CHANGE since is used in VERSION handshake !! */\
	TEXT	reservedOPC2[sizeof(OPC2) - OPC2_CVR_SIZ];\
	/* checksum must be the first structure member */\
	LONG	checksum;\
	ULONG	structver;\
	TEXT	do_checksum;\
	TEXT	byteswap;\
	TEXT	function;\
	TEXT	driver;\
	COUNT	padCOMMON1;\
	TEXT	padCOMMON2;\
	TEXT	logevents;\
	LONG	logerrorevents;\
	LONG	logdebugevents;
	/* to avoid alignment issue, logfile must be at an offset multiple of 8 */\
#ifdef ctFeatRTG_CBPTR
#define CT_INPUT_COMMON CT_INPUT_COMMON1 \
	CBPTR	logfile          /* store pTEXT in a LONG8 to avoid problems in 32/64 bit environments */
#else /* ~ctFeatRTG_CBPTR */
#define CT_INPUT_COMMON CT_INPUT_COMMON1 \
	LONG	logfile[2] /* store pTEXT in a LONG[2] to avoid problems in 32/64 bit environments */
#endif /* ~ctFeatRTG_CBPTR */

typedef struct {
	CT_INPUT_COMMON;
} CT_INPUT, *pCT_INPUT;

#define CT_OUTPUT_COMMON \
	/* checksum must be the first structure member */\
	LONG	checksum;\
	ULONG	structver;\
	LONG	backlen;\
	LONG	padCOMMON1; /* pad to align CT_ERROR to a multiple of 8 */\
	CT_ERROR	err;\
	LONG	padCOMMON2\
	/* to avoid alignment issue, CBPTRs must be at an offset multiple of 8 */

typedef struct {
	CT_OUTPUT_COMMON;
} CT_OUTPUT, *pCT_OUTPUT;


/*
**	MAKE command - create a file
*/

typedef struct {
	CT_INPUT_COMMON;
	/* place new 8-byte long members above this line */
	LONG	pad4[4];        /* reserved for future use */
	LONG	drvfilemode;
	LONG	env;			/* environment */
	LONG	x8mode;
	/* place new 4-byte long members above this line */
	CT_OPEN1	make;
	COUNT	pad2[4];        /* reserved for future use */
	/* place new 2-byte long members above this line */
	TEXT	pad1[7];        /* reserved for future use */
	TEXT	encrtype;
	TEXT	comptype;
	TEXT	complevl;
	TEXT	compstgy;
	TEXT	datext[CT_EXTN_LEN];
	TEXT	idxext[CT_EXTN_LEN];
} CT_MAKE1_INPUT, *pCT_MAKE1_INPUT;
typedef struct {
	CT_INPUT_COMMON;
	/* place new 8-byte long members above this line */
	LONG	pad4[2];        /* reserved for future use, when using make sure to check rtgSTRUCTVERHAS(..., CT_STRUCTVER_CTMAKE2INPUT_0)*/
	LONG	permmask;
	LONG	drvfilemod2;
	LONG	drvfilemode;
	LONG	env;            /* environment */
	LONG	x8mode;
	/* place new 4-byte long members above this line */
	CT_OPEN2	make;
	COUNT	pad2[4];        /* reserved for future use, when using make sure to check rtgSTRUCTVERHAS(..., CT_STRUCTVER_CTMAKE2INPUT_0)*/
	/* place new 2-byte long members above this line */
	TEXT	pad1[3];        /* reserved for future use, when using make sure to check rtgSTRUCTVERHAS(..., CT_STRUCTVER_CTMAKE2INPUT_0)*/
	TEXT	useattrAPI;
	TEXT	encrypt;
	TEXT	srlsegsiz;
	UTEXT	rechdr;
	TEXT	encrtype;
	TEXT	comptype;
	TEXT	complevl;
	TEXT	compstgy;
	TEXT	datext[CT_EXTN_LEN];
	TEXT	idxext[CT_EXTN_LEN];
} CT_MAKE2_INPUT, *pCT_MAKE2_INPUT;
typedef struct {
	CT_INPUT_COMMON;
	CT_OPEN3	make;
	/* place new 8-byte long members above this line */
	LONG	pad4[4];        /* reserved for future use, when using make sure to check rtgSTRUCTVERHAS(..., CT_STRUCTVER_CTMAKE2INPUT_0)*/
	LONG	permmask;
	LONG	drvfilemod2;
	LONG	drvfilemode;
	LONG	env;            /* environment */
	LONG	x8mode;
	/* place new 4-byte long members above this line */
	COUNT	pad2[8];        /* reserved for future use, when using make sure to check rtgSTRUCTVERHAS(..., CT_STRUCTVER_CTMAKE2INPUT_0)*/
	/* place new 2-byte long members above this line */
	TEXT	pad1[3];        /* reserved for future use, when using make sure to check rtgSTRUCTVERHAS(..., CT_STRUCTVER_CTMAKE2INPUT_0)*/
	UTEXT	wordlen;
	TEXT	useattrAPI;
	TEXT	encrypt;
	UTEXT	srlsegsiz;
	UTEXT	rechdr;
	TEXT	encrtype;
	TEXT	comptype;
	TEXT	complevl;
	TEXT	compstgy;
	TEXT	datext[CT_EXTN_LEN];
	TEXT	idxext[CT_EXTN_LEN];
	TEXT	var[MEMU];
} CT_MAKE3_INPUT, *pCT_MAKE3_INPUT;

/* before CT_STRUCTVER_TBLLOCKER */
typedef struct {
	CT_OUTPUT_COMMON;
	/* place new 8-byte long members above this line */
	/* place new 4-byte long members above this line */
	/* place new 2-byte long members above this line */
} CT_MAKE1_OUTPUT, *pCT_MAKE1_OUTPUT;
/* end CT_STRUCTVER_TBLLOCKER */

typedef struct {
	CT_OUTPUT_COMMON;
	/* place new 8-byte long members above this line */
	RTGTASKID locker_taskid;
	/* place new 4-byte long members above this line */
	/* place new 2-byte long members above this line */
} CT_MAKE2_OUTPUT, *pCT_MAKE2_OUTPUT;

#define FUNC_CT_MAKE 1

#ifdef CT_MAKE_IS_LOCAL
#define FUNC_CTMAKE(status, conn, in, lin, out, lout, cnf) {\
	CT_FILE ct = {0};\
	ct.filno = -1;\
	ct.instance = inst;\
	ct.ctinst = &conn->instance[inst];\
	rtgSNPRINTF(ct.rname, rtgSIZEOF(ct.rname) - 1, "%s%s", (in)->make.name, (in)->datext ? (in)->datext : "");\
	CTL_CALL(FUNC_CT_MAKE, &status, conn, (pCT_INPUT)in, lin, out, lout, cnf, &ct)\
}
#else /* ~CT_MAKE_IS_LOCAL */
#ifdef ctFeatRTG_HANDLE
#define FUNC_CTMAKE(status, handle, in, lin, out, lout, cnf, name) CT_CALL(FUNC_CT_MAKE, &status, handle, (pCT_INPUT)in, lin, (pCT_OUTPUT)out, lout, cnf, name);
#else /* ~ctFeatRTG_HANDLE */
#define FUNC_CTMAKE(status, conn, in, lin, out, lout, cnf) {\
	CT_FILE ct = {0};\
	ct.filno = -1;\
	ct.instance = inst;\
	ct.ctinst = &conn->instance[inst];\
	rtgSNPRINTF(ct.rname, rtgSIZEOF(ct.rname) - 1, "%s%s", (in)->make.name, (in)->datext ? (in)->datext : "");\
	CT_CALL(FUNC_CT_MAKE, &status, conn, (pCT_INPUT)in, lin, (pCT_OUTPUT)out, lout, cnf, &ct);\
}
#endif /* ~ctFeatRTG_HANDLE */
#endif /* ~CT_MAKE_IS_LOCAL */

#ifdef ctFeatRTG_KEYS
#define FUNC__CTMAKE(in, lin, out, lout) cts_make((pCT_MAKE3_INPUT)in, lin, (pCT_MAKE2_OUTPUT)out, lout)
#else /* ~ctFeatRTG_KEYS */
#define FUNC__CTMAKE(in, lin, out, lout) cts_make((pCT_MAKE2_INPUT)in, lin, (pCT_MAKE2_OUTPUT)out, lout)
#endif /* ~ctFeatRTG_KEYS */

/*
**	OPEN command - open a file
*/

typedef struct {
	CT_INPUT_COMMON;
#ifdef ctFeatRTG_CBPTR
	CBPTR	pad8;
#else
	LONG	pad8[2];
#endif
	/* place new 8-byte long members above this line */
	LONG	pad4[4];        /* reserved for future use */
	LONG	drvfilemod2;
	LONG	drvfilemode;
	LONG	maintmode;
	LONG	env;			/* environment */
	VRLEN	minlen;
	VRLEN	maxlen;
	ULONG	segsiz;
	LONG	segmax;
	LONG	mxfilz;
	LONG	mxfilzh;
	/* place new 4-byte long members above this line */
	COUNT	nkeys;
	UCOUNT	retrycount;
	/* place new 2-byte long members above this line */
	TEXT	pad1[8];        /* reserved for future use */
	TEXT	signconv;
	TEXT	comptype;
	TEXT	complevl;
	TEXT	compstgy;
	TEXT	name[CT_PATH_LEN];
	TEXT	datext[CT_EXTN_LEN];
	TEXT	idxext[CT_EXTN_LEN];
} CT_OPEN1_INPUT, *pCT_OPEN1_INPUT;
typedef struct {
	CT_INPUT_COMMON;
#ifdef ctFeatRTG_CBPTR
	CBPTR	pad8;
#else
	LONG	pad8[2];
#endif
	/* place new 8-byte long members above this line */
	LONG	pad4[4];        /* reserved for future use */
	LONG	drvfilemod2;
	LONG	drvfilemode;
	LONG	maintmode;
	LONG	env;			/* environment */
	VRLEN	minlen;
	VRLEN	maxlen;
	ULONG	segsiz;
	LONG	segmax;
	LONG	mxfilz;
	LONG	mxfilzh;
	/* place new 4-byte long members above this line */
	COUNT	nkeys;
	UCOUNT	retrycount;
	/* place new 2-byte long members above this line */
	TEXT	pad1[7];        /* reserved for future use */
	UTEXT	wordlen;
	TEXT	signconv;
	TEXT	comptype;
	TEXT	complevl;
	TEXT	compstgy;
	TEXT	name[CT_PATH_LEN];
	TEXT	datext[CT_EXTN_LEN];
	TEXT	idxext[CT_EXTN_LEN];
	TEXT	var[MEMU];
} CT_OPEN2_INPUT, * pCT_OPEN2_INPUT;

typedef struct {
	CT_OUTPUT_COMMON;
#ifdef ctFeatRTG_CBPTR
	CBPTR	pad8;
#else
	LONG	pad8[2];
#endif
/* place new 8-byte long members above this line */
	LONG	pad4[3];        /* reserved for future use */
	LONG	cmptype;
	LONG	cccstatus;
	LONG	drvfilemode;
	LONG	maintmode;
	ULONG	minlen;
	ULONG	maxlen;
	LONG	got_filtyp;
	LONG	got_physiz;
	LONG	got_physizh;
	LONG	got_iphysiz;
	LONG	got_iphysizh;
	LONG	got_filmod;
	LONG	got_flmode2;
	LONG	got_flmode3;
	LONG	got_ifilmod;
	LONG	got_numrecords;
	LONG	got_delrecords;
	ULONG	got_segsiz;
	LONG	got_segmax;
	ULONG	xmllen;
	LONG	blockfact;
	LONG	prealloc;
	LONG	extension;
	LONG	record;
	LONG	recordh;
	/* place new 4-byte long members above this line */
	COUNT	pad2[4];        /* reserved for future use */
	COUNT	filno;
	COUNT	context;
	COUNT	io_context;
	UCOUNT	nkeys;
	COUNT	maxkeysize;
	COUNT	keyno;
	/* place new 2-byte long members above this line */
	TEXT	pad1[8];        /* reserved for future use */
	TEXT	got_segmented;
	TEXT	got_encrypted;
	TEXT	is_huge;
	TEXT	signconv;
	TEXT	comment[CT_COMMENT_LEN];
	TEXT	compress;
	TEXT	encrypt;
	TEXT	trans[256];
	CT_KEY1	key[CT_MAX_KEYS];
} CT_OPEN1_OUTPUT, *pCT_OPEN1_OUTPUT;
typedef struct {
	CT_OUTPUT_COMMON;
	LONG8	pad8;
	/* place new 8-byte long members above this line */
	LONG	env;
#ifdef ctFeatMAXFILE32
	LONG	filno;          /* file number */
#else
	LONG	filno32;        /* file number */
#endif
	RTGTASKID	locker_taskid;
	LONG	cmptype;
	LONG	cccstatus;
	LONG	drvfilemode;
	LONG	maintmode;
	ULONG	minlen;
	ULONG	maxlen;
	LONG	got_filtyp;
	LONG	got_physiz;
	LONG	got_physizh;
	LONG	got_iphysiz;
	LONG	got_iphysizh;
	LONG	got_filmod;
	LONG	got_flmode2;
	LONG	got_flmode3;
	LONG	got_ifilmod;
	LONG	got_numrecords;
	LONG	got_delrecords;
	ULONG	got_segsiz;
	LONG	got_segmax;
	ULONG	xmllen;
	LONG	blockfact;
	LONG	prealloc;
	LONG	extension;
	LONG	record;
	LONG	recordh;
	/* place new 4-byte long members above this line */
	COUNT	pad2[1];        /* reserved for future use */
	COUNT	autoinc[3];     /* autoinc info [0]=idxno [1]=segoff [2]=seglen */
#ifdef ctFeatMAXFILE32
	COUNT	filno16;        /* file number */
#else
	COUNT	filno;          /* file number */
#endif
	COUNT	context;
	COUNT	io_context;
	UCOUNT	nkeys;
	COUNT	maxkeysize;
	COUNT	keyno;
	/* place new 2-byte long members above this line */
	TEXT	pad1[5];        /* reserved for future use */
	TEXT	ctbtrv;
	TEXT	srlsegsiz;
	UTEXT	rechdr;
	TEXT	got_segmented;
	TEXT	got_encrypted;
	TEXT	is_huge;
	TEXT	signconv;
	TEXT	comment[CT_COMMENT_LEN];
	TEXT	compress;
	TEXT	encrypt;
	TEXT	trans[256];
	CT_KEY2	key[CT_MAX_KEYS];
} CT_OPEN2_OUTPUT, *pCT_OPEN2_OUTPUT;
typedef struct {
	CT_OUTPUT_COMMON;
	LONG8	pad8;
	/* place new 8-byte long members above this line */
	LONG	pad4[4];
	LONG	got_mxfilz;
	LONG	got_mxfilzh;
	LONG	env;
#ifdef ctFeatMAXFILE32
	LONG	filno;          /* file number */
#else
	LONG	filno32;        /* file number */
#endif
	RTGTASKID	locker_taskid;
	LONG	cmptype;
	LONG	drvfilemod2;
	LONG	drvfilemode;
	LONG	maintmode;
	ULONG	minlen;
	ULONG	maxlen;
	LONG	got_filtyp;
	LONG	got_physiz;
	LONG	got_physizh;
	LONG	got_iphysiz;
	LONG	got_iphysizh;
	LONG	got_filmod;
	LONG	got_flmode2;
	LONG	got_flmode3;
	LONG	got_ifilmod;
	LONG	got_numrecords;
	LONG	got_delrecords;
	ULONG	got_segsiz;
	LONG	got_segmax;
	ULONG	xmllen;
	LONG	blockfact;
	LONG	prealloc;
	LONG	extension;
	LONG	record;
	LONG	recordh;
	/* place new 4-byte long members above this line */
	COUNT	pad2[6];        /* reserved for future use */
	UCOUNT	nodesize;
	COUNT	autoinc[3];     /* autoinc info [0]=idxno [1]=segoff [2]=seglen */
#ifdef ctFeatMAXFILE32
	COUNT	filno16;        /* file number */
#else
	COUNT	filno;          /* file number */
#endif
	COUNT	context;
	COUNT	io_context;
	UCOUNT	nkeys;
	COUNT	maxkeysize;
	COUNT	keyno;
	/* place new 2-byte long members above this line */
	TEXT	pad1[6];        /* reserved for future use */
	TEXT	has_word;       /* YES=file is password protected, HYS=file was open with NOPASS */
	TEXT	resver;
	TEXT	ctbtrv;
	TEXT	srlsegsiz;
	UTEXT	rechdr;
	TEXT	got_segmented;
	TEXT	got_encrypted;
	TEXT	is_huge;
	TEXT	signconv;
	TEXT	compress;
	TEXT	encrypt;
	TEXT	comment[CT_COMMENT_LEN];
	TEXT	trans[256];
	TEXT	var[MEMU];
} CT_OPEN3_OUTPUT, * pCT_OPEN3_OUTPUT;

#define FUNC_CT_OPEN 2

#ifdef ctFeatRTG_HANDLE
#define FUNC_CTOPEN(status, conn, in, lin, out, lout, cnf, name) CT_CALL(FUNC_CT_OPEN, &status, conn, (pCT_INPUT)in, lin, (pCT_OUTPUT)out, lout, cnf, name)
#else /* ~ctFeatRTG_HANDLE */
#define FUNC_CTOPEN(status, conn, in, lin, out, lout, cnf, ct) CT_CALL(FUNC_CT_OPEN, &status, conn, (pCT_INPUT)in, lin, (pCT_OUTPUT)out, lout, cnf, ct)
#endif /* ~ctFeatRTG_HANDLE */
#ifdef ctFeatRTG_KEYS
#define FUNC__CTOPEN(in, lin, out, lout) cts_open((pCT_OPEN2_INPUT)in, lin, (pCT_OPEN3_OUTPUT)(pVOID)out, lout)
#else
#define FUNC__CTOPEN(in, lin, out, lout) cts_open((pCT_OPEN2_INPUT)in, lin, (pCT_OPEN2_OUTPUT)(pVOID)out, lout)
#endif

/*
**	CLOSE command - close a file
*/

typedef struct {
	CT_INPUT_COMMON;
#ifdef ctFeatRTG_CBPTR
	CBPTR	pad8;
#else
	LONG	pad8[2];
#endif
/* place new 8-byte long members above this line */
	LONG	avl4;
	LONG	drvfilemode;
	/* place new 4-byte long members above this line */
	COUNT	filno;
	COUNT	context;
	COUNT	io_context;
	/* place new 2-byte long members above this line */
	TEXT	name[CT_PATH_LEN];
} CT_CLOSE1_INPUT, *pCT_CLOSE1_INPUT;
typedef struct {
	CT_INPUT_COMMON;
#ifdef ctFeatRTG_CBPTR
	CBPTR	pad8;
#else
	LONG	pad8[2];
#endif
/* place new 8-byte long members above this line */
	LONG	pad4[3];        /* reserved for future use */
	LONG	env;
	LONG	env2;
#ifdef ctFeatMAXFILE32
	LONG	filno;          /* file number */
#else
	LONG	filno32;        /* file number */
#endif
	LONG	avl4;
	LONG	drvfilemode;
	/* place new 4-byte long members above this line */
	COUNT	pad2[4];        /* reserved for future use */
#ifdef ctFeatMAXFILE32
	COUNT	filno16;        /* file number */
#else
	COUNT	filno;          /* file number */
#endif
	COUNT	context;
	COUNT	io_context;
	/* place new 2-byte long members above this line */
	TEXT	pad1[10];        /* reserved for future use */
	TEXT	name[CT_PATH_LEN];
} CT_CLOSE2_INPUT, *pCT_CLOSE2_INPUT;
typedef struct {
	CT_INPUT_COMMON;
#ifdef ctFeatRTG_CBPTR
	CBPTR	pad8;
#else
	LONG	pad8[2];
#endif
	/* place new 8-byte long members above this line */
	LONG	pad4[2];        /* reserved for future use */
	LONG	flmode2;
	LONG	env;
	LONG	env2;
#ifdef ctFeatMAXFILE32
	LONG	filno;          /* file number */
#else
	LONG	filno32;        /* file number */
#endif
	LONG	avl4;
	LONG	drvfilemode;
	/* place new 4-byte long members above this line */
	COUNT	pad2[4];        /* reserved for future use */
#ifdef ctFeatMAXFILE32
	COUNT	filno16;        /* file number */
#else
	COUNT	filno;          /* file number */
#endif
	COUNT	context;
	COUNT	io_context;
	/* place new 2-byte long members above this line */
	TEXT	pad1[9];        /* reserved for future use */
	UTEXT	wordlen;
	TEXT	name[CT_PATH_LEN];
	TEXT	var[MEMU];
} CT_CLOSE3_INPUT, * pCT_CLOSE3_INPUT;

typedef struct {
	CT_OUTPUT_COMMON;
	/* place new 8-byte long members above this line */
#ifdef ctFeatRTG_CBPTR
	CBPTR	pad8;
#else
	LONG	pad8[2];
#endif
/* place new 4-byte long members above this line */
	LONG	avl4;
	/* place new 2-byte long members above this line */
} CT_CLOSE_OUTPUT, * pCT_CLOSE_OUTPUT;

#define FUNC_CT_CLOSE 3

#ifdef ctFeatRTG_HANDLE
#define FUNC_CTCLOSE(status, conn, in, lin, out, lout) CT_CALL(FUNC_CT_CLOSE, &status, conn, (pCT_INPUT)in, lin, (pCT_OUTPUT)out, lout, NULL, NULL)
#else /* ~ctFeatRTG_HANDLE */
#define FUNC_CTCLOSE(status, conn, in, lin, out, lout, cnf, ct) CT_CALL(FUNC_CT_CLOSE, &status, conn, (pCT_INPUT)in, lin, (pCT_OUTPUT)out, lout, cnf, ct)
#endif /* ~ctFeatRTG_HANDLE */
#define FUNC__CTCLOSE(in, lin, out, lout) cts_close((pCT_CLOSE3_INPUT)in, lin, (CT_CLOSE_OUTPUT *)(pVOID)out, lout)


typedef struct {
	CT_INPUT_COMMON;
#ifdef ctFeatRTG_CBPTR
	CBPTR	pad8;
#else
	LONG	pad8[2];
#endif
/* place new 8-byte long members above this line */
	ULONG	minsiz;
	ULONG	maxsiz;
#ifdef ctFeatMAXFILE32
	LONG	filno;          /* file number */
#else
	LONG	filno32;        /* file number */
#endif
	LONG	env2;
	LONG	avl4;
	LONG	drvfilemode;    /* file's mode */
	LONG	env;            /* environment */
	LONG	keytype;        /* key type */
	ULONG	minlen;         /* min len */
	ULONG	maxlen;         /* max len */
	LONG	current_offset; /* current record offset */
	LONG	current_offseth;/* current record offset (highword) */
	ULONG	fetsiz;         /* prefetch buffer size */
	ULONG	batrdy;         /* number of records to process in batchadd buffer */
	VRLEN	reclen;         /* record len for rewrites */
	ULONG	current_recnum; /* current record number */
	/* place new 4-byte long members above this line */
	TEXT	srlsegsiz;        /* serial seg size */
	UTEXT	minsizpad;
	COUNT	pad2[1];        /* reserved for future use */
#ifdef ctFeatMAXFILE32
	COUNT	filno16;        /* file number */
#else
	COUNT	filno;          /* file number */
#endif
	COUNT	keyno;          /* key number */
	COUNT	keylen;         /* key length */
	COUNT	context;        /* if -1 actual context is ok */
	COUNT	keysiz;         /* key size */
	COUNT	autoinc[3];     /* autoinc info [0]=idxno [1]=segoff [2]=seglen */
	/* place new 2-byte long members above this line */
	TEXT	varfeat;
	UTEXT	rechdr;         /* record header length */
	TEXT	var[MEMU];      /* variable input area */
} CT_COMM_INPUT, * pCT_COMM_INPUT;

typedef struct {
	CT_OUTPUT_COMMON;
#ifdef ctFeatRTG_CBPTR
	CBPTR	pad8;
#else
	LONG	pad8[2];
#endif
/* place new 8-byte long members above this line */
	LONG	pad4[2];        /* reserved for future use */
	RTGTASKID	locker_taskid;  /* record locker taskid in case of lock error */
	ULONG	batadd;        /* number of records added in batch */
	LONG	avl4;
	LONG	env;            /* affected env */
	LONG	record;         /* affected record offset */
	LONG	recordh;        /* affected record offset (highword) */
	ULONG	fetret;         /* number of records in prefetch buffer */
	ULONG	recnum;         /* record number */
	/* place new 4-byte long members above this line */
	COUNT	pad2[2];        /* reserved for future use */
	COUNT	context;        /* current context -1 means no changes */
	COUNT	keyno;          /* affected index id */
	/* place new 2-byte long members above this line */
	TEXT	pad1[8];        /* reserved for future use */
	TEXT	var[MEMU];      /* variable output area */
} CT_COMM_OUTPUT, * pCT_COMM_OUTPUT;

/*
**	WRITE command - write a record
*/

typedef CT_COMM_INPUT CT_WRITE_INPUT, * pCT_WRITE_INPUT;
typedef CT_COMM_OUTPUT CT_WRITE_OUTPUT, * pCT_WRITE_OUTPUT;

#define FUNC_CT_WRITE 4

#ifdef ctFeatRTG_HANDLE
#define FUNC_CTWRITE(status, conn, in, lin, out, lout) CT_CALL(FUNC_CT_WRITE, &status, conn, (pCT_INPUT)in, lin, (pCT_OUTPUT)out, lout, NULL, NULL)
#else /* ~ctFeatRTG_HANDLE */
#define FUNC_CTWRITE(status, conn, in, lin, out, lout, cnf, ct) CT_CALL(FUNC_CT_WRITE, &status, conn, (pCT_INPUT)in, lin, (pCT_OUTPUT)out, lout, cnf, ct)
#endif /* ~ctFeatRTG_HANDLE */
#define FUNC__CTWRITE(in, lin, out, lout) cts_write((CT_WRITE_INPUT *)in, lin, (CT_WRITE_OUTPUT *)(pVOID)out, lout)

/*
**	READ command - read a record
*/

typedef CT_COMM_INPUT CT_READ_INPUT, * pCT_READ_INPUT;
typedef CT_COMM_OUTPUT CT_READ_OUTPUT, * pCT_READ_OUTPUT;

#define FUNC_CT_READ 5

#ifdef ctFeatRTG_HANDLE
#define FUNC_CTREAD(status, conn, in, lin, out, lout) CT_CALL(FUNC_CT_READ, &status, conn, (pCT_INPUT)in, lin, (pCT_OUTPUT)out, lout, NULL, NULL)
#else /* ~ctFeatRTG_HANDLE */
#define FUNC_CTREAD(status, conn, in, lin, out, lout, cnf, ct) CT_CALL(FUNC_CT_READ, &status, conn, (pCT_INPUT)in, lin, (pCT_OUTPUT)out, lout, cnf, ct)
#endif /* ~ctFeatRTG_HANDLE */
#define FUNC__CTREAD(in, lin, out, lout) cts_read((CT_READ_INPUT *)in, lin, (CT_READ_OUTPUT *)(pVOID)out, lout)

/*
**	START command - scan an index reading records
*/

typedef CT_COMM_INPUT CT_START_INPUT, * pCT_START_INPUT;
typedef CT_COMM_OUTPUT CT_START_OUTPUT, * pCT_START_OUTPUT;

#define FUNC_CT_START 6

#ifdef ctFeatRTG_HANDLE
#define FUNC_CTSTART(status, conn, in, lin, out, lout) CT_CALL(FUNC_CT_START, &status, conn, (pCT_INPUT)in, lin, (pCT_OUTPUT)out, lout, NULL, NULL)
#else /* ~ctFeatRTG_HANDLE */
#define FUNC_CTSTART(status, conn, in, lin, out, lout, cnf, ct) CT_CALL(FUNC_CT_START, &status, conn, (pCT_INPUT)in, lin, (pCT_OUTPUT)out, lout, cnf, ct)
#endif /* ~ctFeatRTG_HANDLE */
#define FUNC__CTSTART(in, lin, out, lout) cts_start((CT_START_INPUT *)in, lin, (CT_START_OUTPUT *)(pVOID)out, lout)

/*
**	NEXTPREV command - scan an index reading records
*/

#define FUNC_CT_NEXTPREV 7

#ifdef ctFeatRTG_HANDLE
#define FUNC_CTNEXTPREV(status, conn, in, lin, out, lout) CT_CALL(FUNC_CT_NEXTPREV, &status, conn, (pCT_INPUT)in, lin, (pCT_OUTPUT)out, lout, NULL, NULL)
#else /* ~ctFeatRTG_HANDLE */
#define FUNC_CTNEXTPREV(status, conn, in, lin, out, lout, cnf, ct) CT_CALL(FUNC_CT_NEXTPREV, &status, conn, (pCT_INPUT)in, lin, (pCT_OUTPUT)out, lout, cnf, ct)
#endif /* ~ctFeatRTG_HANDLE */
#define FUNC__CTNEXTPREV(in, lin, out, lout) cts_nextprev((pCT_COMM_INPUT)in, lin, (pCT_COMM_OUTPUT)(pVOID)out, lout)

/*
**	REWDEL command - rewrite or delete a record
*/

typedef CT_COMM_INPUT CT_REWDEL_INPUT, * pCT_REWDEL_INPUT;
typedef CT_COMM_OUTPUT CT_REWDEL_OUTPUT, * pCT_REWDEL_OUTPUT;

#define CT_REWDEL_PRE CT_WRITE_PRE
#define CT_REWDEL_POST CT_WRITE_POST

#define FUNC_CT_REWDEL 8

#ifdef ctFeatRTG_HANDLE
#define FUNC_CTREWDEL(status, conn, in, lin, out, lout) CT_CALL(FUNC_CT_REWDEL, &status, conn, (pCT_INPUT)in, lin, (pCT_OUTPUT)out, lout, NULL, NULL)
#else /* ~ctFeatRTG_HANDLE */
#define FUNC_CTREWDEL(status, conn, in, lin, out, lout, cnf, ct) CT_CALL(FUNC_CT_REWDEL, &status, conn, (pCT_INPUT)in, lin, (pCT_OUTPUT)out, lout, cnf, ct)
#endif /* ~ctFeatRTG_HANDLE */
#define FUNC__CTREWDEL(in, lin, out, lout) cts_rewdel((CT_REWDEL_INPUT *)in, lin, (CT_REWDEL_OUTPUT *)(pVOID)out, lout)

#define FUNC_CT_DELETE 9

#ifdef ctFeatRTG_HANDLE
#define FUNC_CTDELETE(status, conn, in, lin, out, lout) CT_CALL(FUNC_CT_DELETE, &status, conn, (pCT_INPUT)in, lin, (pCT_OUTPUT)out, lout, NULL, NULL)
#else /* ~ctFeatRTG_HANDLE */
#define FUNC_CTDELETE(status, conn, in, lin, out, lout, cnf, ct) CT_CALL(FUNC_CT_DELETE, &status, conn, (pCT_INPUT)in, lin, (pCT_OUTPUT)out, lout, cnf, ct)
#endif /* ~ctFeatRTG_HANDLE */
#define FUNC__CTDELETE(in, lin, out, lout) cts_delete((CT_REWDEL_INPUT *)in, lin, (CT_REWDEL_OUTPUT *)(pVOID)out, lout)

/*
**	UNLOCK command - free all the locks placed on a file
*/

typedef struct {
	CT_INPUT_COMMON;
	/* place new 8-byte long members above this line */
	LONG	rec_offset;
	LONG	rec_offseth;
	/* place new 4-byte long members above this line */
	COUNT	filno;
	/* place new 2-byte long members above this line */
} CT_UNLOCK1_INPUT, *pCT_UNLOCK1_INPUT;
typedef struct {
	CT_INPUT_COMMON;
	/* place new 8-byte long members above this line */
	LONG	pad4[4];        /* reserved for future use */
#ifdef ctFeatMAXFILE32
	LONG	filno;          /* file number */
#else
	LONG	filno32;        /* file number */
#endif
	LONG	rec_offset;
	LONG	rec_offseth;
	/* place new 4-byte long members above this line */
	COUNT	pad2[4];        /* reserved for future use */
#ifdef ctFeatMAXFILE32
	COUNT	filno16;        /* file number */
#else
	COUNT	filno;          /* file number */
#endif
	/* place new 2-byte long members above this line */
	TEXT	pad1[10];        /* reserved for future use */
} CT_UNLOCK2_INPUT, *pCT_UNLOCK2_INPUT;

typedef struct {
	CT_OUTPUT_COMMON;
} CT_UNLOCK_OUTPUT, *pCT_UNLOCK_OUTPUT;

#define FUNC_CT_UNLOCK 10

#ifdef ctFeatRTG_HANDLE
#define FUNC_CTUNLOCK(status, conn, in, lin, out, lout) CT_CALL(FUNC_CT_UNLOCK, &status, conn, (pCT_INPUT)in, lin, (pCT_OUTPUT)out, lout, NULL, NULL)
#else /* ~ctFeatRTG_HANDLE */
#define FUNC_CTUNLOCK(status, conn, in, lin, out, lout, cnf, ct) CT_CALL(FUNC_CT_UNLOCK, &status, conn, (pCT_INPUT)in, lin, (pCT_OUTPUT)out, lout, cnf, ct)
#endif /* ~ctFeatRTG_HANDLE */
#define FUNC__CTUNLOCK(in, lin, out, lout) cts_unlock((pCT_UNLOCK2_INPUT)in, lin, (CT_UNLOCK_OUTPUT *)(pVOID)out, lout)

/*
**	VERSION command - return version information
*/

typedef struct {
	CT_INPUT_COMMON;
	/* place new 8-byte long members above this line */
	/* place new 4-byte long members above this line */
	/* place new 2-byte long members above this line */
} CT_VERSION1_INPUT, * pCT_VERSION1_INPUT;
typedef struct {
	CT_INPUT_COMMON;
	/* place new 8-byte long members above this line */
	LONG	featmask;
	LONG	pad4[3];
	/* place new 4-byte long members above this line */
	/* place new 2-byte long members above this line */
} CT_VERSION2_INPUT, * pCT_VERSION2_INPUT;

typedef struct {
	CT_OUTPUT_COMMON;
	/* place new 8-byte long members above this line */
	LONG	compatid;
	/* place new 4-byte long members above this line */
	/* place new 2-byte long members above this line */
	TEXT	version[CT_VERSION_LEN];
} CT_VERSION1_OUTPUT, * pCT_VERSION1_OUTPUT;
typedef struct {
	CT_OUTPUT_COMMON;
	/* place new 8-byte long members above this line */
	LONG	compatid;
	/* place new 4-byte long members above this line */
	/* place new 2-byte long members above this line */
	TEXT	version[CT_VERSION_LEN];
	LONG	featmask;
	LONG	pad4[3];
} CT_VERSION2_OUTPUT, * pCT_VERSION2_OUTPUT;

#define FUNC_CT_VERSION 11

#ifdef ctFeatRTG_HANDLE
#define FUNC_CTVERSION(status, conn, in, lin, out, lout) CTS_CALL(FUNC_CT_VERSION, &status, conn, (pCT_INPUT)in, lin, (pCT_OUTPUT)out, lout, NULL, NULL)
#else /* ~ctFeatRTG_HANDLE */
#define FUNC_CTVERSION(status, conn, in, lin, out, lout, cnf) CTS_CALL(FUNC_CT_VERSION, &status, conn, (pCT_INPUT)in, lin, (pCT_OUTPUT)out, lout, cnf, NULL)
#endif /* ~ctFeatRTG_HANDLE */
#define FUNC__CTVERSION(in, lin, out, lout) cts_version((pCT_VERSION2_INPUT)in, lin, (pCT_VERSION2_OUTPUT)(pVOID)out, lout)

/*
**	SQLINFO command - write resource records for SQL info
*/

typedef struct {
	CT_INPUT_COMMON;
	/* place new 8-byte long members above this line */
	LONG	drvfilemode;
	LONG	env;            /* environment */
	ULONG	xmllen;
	/* place new 4-byte long members above this line */
	COUNT	filno;          /* file number */
	/* place new 2-byte long members above this line */
	TEXT	var[MEMU];      /* variable input area */
} CT_SQLINFO1_INPUT, *pCT_SQLINFO1_INPUT;
typedef struct {
	CT_INPUT_COMMON;
	/* place new 8-byte long members above this line */
	LONG	pad4[3];        /* reserved for future use */
	LONG	drvfilemod2;
#ifdef ctFeatMAXFILE32
	LONG	filno;          /* file number */
#else
	LONG	filno32;        /* file number */
#endif
	LONG	drvfilemode;
	LONG	env;            /* environment */
	ULONG	xmllen;
	/* place new 4-byte long members above this line */
	COUNT	pad2[3];        /* reserved for future use */
	UCOUNT	alteraction;    /* alter table action */
#ifdef ctFeatMAXFILE32
	COUNT	filno16;        /* file number */
#else
	COUNT	filno;          /* file number */
#endif
	/* place new 2-byte long members above this line */
	UTEXT	wordlen;
	TEXT	nopass;         /* permit read-only access w/out file password */
	TEXT	pad1[4];        /* reserved for future use */
	TEXT	var[MEMU];      /* variable input area */
} CT_SQLINFO2_INPUT, *pCT_SQLINFO2_INPUT;

typedef struct {
	CT_OUTPUT_COMMON;
	/* place new 8-byte long members above this line */
	LONG	env;            /* environment */
	ULONG	xmllen;
	/* place new 4-byte long members above this line */
	/* place new 2-byte long members above this line */
	TEXT	var[MEMU];      /* variable output area */
} CT_SQLINFO_OUTPUT, *pCT_SQLINFO_OUTPUT;

#define FUNC_CT_SQLINFO 12

#ifdef ctFeatRTG_HANDLE
#define FUNC_CTSQLINFO(status, conn, in, lin, out, lout) CT_CALL(FUNC_CT_SQLINFO, &status, conn, (pCT_INPUT)in, lin, (pCT_OUTPUT)out, lout, NULL, NULL)
#else /* ~ctFeatRTG_HANDLE */
#define FUNC_CTSQLINFO(status, conn, in, lin, out, lout, cnf) CT_CALL(FUNC_CT_SQLINFO, &status, conn, (pCT_INPUT)in, lin, (pCT_OUTPUT)out, lout, cnf, NULL)
#endif /* ~ctFeatRTG_HANDLE */
#define FUNC__CTSQLINFO(in, lin, out, lout) cts_sqlinfo((pCT_SQLINFO2_INPUT)in, lin, (CT_SQLINFO_OUTPUT *)(pVOID)out, lout)

#define FUNC_CT_ALTER1 19

#ifdef ctFeatRTG_HANDLE
#define FUNC_CTALTER1(status, conn, in, lin, out, lout) CT_CALL(FUNC_CT_ALTER1, &status, conn, (pCT_INPUT)in, lin, (pCT_OUTPUT)out, lout, NULL, NULL)
#else /* ~ctFeatRTG_HANDLE */
#define FUNC_CTALTER1(status, conn, in, lin, out, lout, cnf) CT_CALL(FUNC_CT_ALTER1, &status, conn, (pCT_INPUT)in, lin, (pCT_OUTPUT)out, lout, cnf, NULL)
#endif /* ~ctFeatRTG_HANDLE */
#define FUNC__CTALTER1(in, lin, out, lout) cts_alter((pCT_SQLINFO2_INPUT)in, lin, (CT_SQLINFO_OUTPUT *)(pVOID)out, lout)

#define FUNC_CT_FILEWORD 20

#ifdef ctFeatRTG_HANDLE
#define FUNC_CTFILEWORD(status, conn, in, lin, out, lout) CT_CALL(FUNC_CT_FILEWORD, &status, conn, (pCT_INPUT)in, lin, (pCT_OUTPUT)out, lout, NULL, NULL)
#else /* ~ctFeatRTG_HANDLE */
#define FUNC_CTFILEWORD(status, conn, in, lin, out, lout, cnf) CT_CALL(FUNC_CT_FILEWORD, &status, conn, (pCT_INPUT)in, lin, (pCT_OUTPUT)out, lout, cnf, NULL)
#endif /* ~ctFeatRTG_HANDLE */
#define FUNC__CTFILEWORD(in, lin, out, lout) cts_fileword((pCT_SQLINFO2_INPUT)in, lin, (CT_SQLINFO_OUTPUT *)(pVOID)out, lout)

/*
**	SQLLINK command - make a file accessible from sql
*/

typedef struct {
	CT_INPUT_COMMON;
	/* place new 8-byte long members above this line */
	/* place new 4-byte long members above this line */
	/* place new 2-byte long members above this line */
	TEXT	logerror;
	TEXT	opcode;
	TEXT	grant_public;
	TEXT	name[CT_PATH_LEN];
	TEXT	dbname[TPE_MAX_DBIDLEN];
	TEXT	symname[TPE_MAX_IDLEN];
	TEXT	prefix[TPE_MAX_IDLEN];
	TEXT	server_name[CT_SERV_LEN];
	TEXT	admin_word[CT_PASS_LEN];
	TEXT	datext[CT_EXTN_LEN];
	TEXT	idxext[CT_EXTN_LEN];
	TEXT	table_owner[TPE_MAX_IDLEN];
} CT_SQLLINK1_INPUT, *pCT_SQLLINK1_INPUT;
typedef struct {
	CT_INPUT_COMMON;
	/* place new 8-byte long members above this line */
	LONG	pad4[4];
	/* place new 4-byte long members above this line */
	/* place new 2-byte long members above this line */
	TEXT	logerror;
	TEXT	opcode;
	TEXT	grant_public;
	TEXT	name[CT_PATH_LEN];
	TEXT	dbname[TPE_MAX_DBIDLEN];
	TEXT	symname[TPE_MAX_IDLEN];
	TEXT	prefix[TPE_MAX_IDLEN];
	TEXT	server_name[CT_SERV_LEN];
	TEXT	admin_word[CT_PASS_LEN];
	TEXT	datext[CT_EXTN_LEN];
	TEXT	idxext[CT_EXTN_LEN];
	TEXT	table_owner[TPE_MAX_IDLEN];
} CT_SQLLINK2_INPUT, *pCT_SQLLINK2_INPUT;

typedef struct {
	CT_OUTPUT_COMMON;
	/* place new 8-byte long members above this line */
	/* place new 4-byte long members above this line */
	/* place new 2-byte long members above this line */
} CT_SQLLINK_OUTPUT, *pCT_SQLLINK_OUTPUT;

#define FUNC_CT_SQLLINK 13


#define FUNC_CTSQLLINK(status, conn, in, lin, out, lout, cnf, name) CT_CALL(FUNC_CT_SQLLINK, &status, conn, (pCT_INPUT)in, lin, (pCT_OUTPUT)out, lout, cnf, name)
#define FUNC__CTSQLLINK(in, lin, out, lout) cts_sqllink((pCT_SQLLINK2_INPUT)in, lin, (pCT_SQLLINK_OUTPUT)(pVOID)out, lout)

/*
**	COPY command - copy a file
*/

typedef struct {
	CT_INPUT_COMMON;
	/* place new 8-byte long members above this line */
	LONG	drvfilemode;    /* file modes */
	LONG	todrvfilemode;  /* file modes */
	LONG	env;            /* environment */
	LONG	keytype;        /* key type */
	LONG	current_offset; /* current record offset */
	LONG	current_offseth;/* current record offset (highword) */
	VRLEN	maxlen;         /* max len */
#ifdef ctFeatMAXFILE32
	COUNT	fromfilno16;    /* source file number */
	COUNT	tofilno16;      /* destination file number */
#else
	COUNT	fromfilno;      /* source file number */
	COUNT	tofilno;        /* destination file number */
#endif
	COUNT	keyno;          /* key number */
	COUNT	context;        /* if -1 actual context is ok */
	TEXT	padALIGN[3];
	UTEXT	rechdr;         /* record header length */
	/* place new 2-byte long members above this line */
	TEXT	pad1[8];        /* reserved for future use */
#ifdef ctFeatMAXFILE32
	LONG	fromfilno;      /* source file number */
	LONG	tofilno;        /* destination file number */
#else
	LONG	fromfilno32;    /* source file number */
	LONG	tofilno32;      /* destination file number */
#endif
	LONG	toenv;
} CT_COPY_INPUT, *pCT_COPY_INPUT;

typedef struct {
	CT_OUTPUT_COMMON;
	/* place new 8-byte long members above this line */
	/* place new 4-byte long members above this line */
	/* place new 2-byte long members above this line */
} CT_COPY_OUTPUT, *pCT_COPY_OUTPUT;

#define FUNC_CT_COPY 14

#ifdef ctFeatRTG_HANDLE
#define FUNC_CTCOPY(status, conn, in, lin, out, lout) CT_CALL(FUNC_CT_COPY, &status, conn, (pCT_INPUT)in, lin, (pCT_OUTPUT)out, lout, NULL, NULL)
#else /* ~ctFeatRTG_HANDLE */
#define FUNC_CTCOPY(status, conn, in, lin, out, lout, cnf) CT_CALL(FUNC_CT_COPY, &status, conn, (pCT_INPUT)in, lin, (pCT_OUTPUT)out, lout, cnf, NULL)
#endif /* ~ctFeatRTG_HANDLE */
#define FUNC__CTCOPY(in, lin, out, lout) cts_copy((CT_COPY_INPUT *)in, lin, (CT_COPY_OUTPUT *)(pVOID)out, lout)

/*
**	INFO command - get info from a file
*/

typedef struct {
	CT_INPUT_COMMON;
	/* place new 8-byte long members above this line */
	LONG	pad4[3];        /* reserved for future use */
#ifdef ctFeatMAXFILE32
	LONG	filno;          /* file number */
#else
	LONG	filno32;        /* file number */
#endif
	/* place new 4-byte long members above this line */
	COUNT	pad2[4];        /* reserved for future use */
#ifdef ctFeatMAXFILE32
	COUNT	filno16;        /* file number */
#else
	COUNT	filno;          /* file number */
#endif
	COUNT	nkeys;
	/* place new 2-byte long members above this line */
	TEXT	pad1[8];        /* reserved for future use */
} CT_INFO_INPUT, * pCT_INFO_INPUT;

typedef struct {
	CT_OUTPUT_COMMON;
	/* place new 8-byte long members above this line */
	LONG	pad4[4];        /* reserved for future use */
	/* place new 4-byte long members above this line */
	COUNT	pad2[4];        /* reserved for future use */
	LONG	got_physiz;
	LONG	got_physizh;
	LONG	got_iphysiz;
	LONG	got_iphysizh;
	LONG	got_numrecords;
	LONG	got_delrecords;
	/* place new 2-byte long members above this line */
	TEXT	pad1[8];        /* reserved for future use */
} CT_INFO_OUTPUT, * pCT_INFO_OUTPUT;

#define FUNC_CT_INFO 15

#ifdef ctFeatRTG_HANDLE
#define FUNC_CTINFO(status, conn, in, lin, out, lout) CT_CALL(FUNC_CT_INFO, &status, conn, (pCT_INPUT)in, lin, (pCT_OUTPUT)out, lout, NULL, NULL)
#else /* ~ctFeatRTG_HANDLE */
#define FUNC_CTINFO(status, conn, in, lin, out, lout, cnf) CT_CALL(FUNC_CT_INFO, &status, conn, (pCT_INPUT)in, lin, (pCT_OUTPUT)out, lout, cnf, NULL)
#endif /* ~ctFeatRTG_HANDLE */
#define FUNC__CTINFO(in, lin, out, lout) cts_info((CT_INFO_INPUT *)in, lin, (CT_INFO_OUTPUT *)(pVOID)out, lout)

/*
**	ADDIDX command - add new indices to a file
*/

typedef struct {
	CT_INPUT_COMMON;
	CT_OPEN2	make;
	/* place new 8-byte long members above this line */
	LONG	drvfilemode;
	LONG	env;
	LONG	x8mode;
	/* place new 4-byte long members above this line */
	COUNT	filno;
	/* place new 2-byte long members above this line */
	UTEXT	rechdr;
	TEXT	is_huge;
	TEXT	is_vlen;
} CT_ADDIDX1_INPUT, *pCT_ADDIDX1_INPUT;
typedef struct {
	CT_INPUT_COMMON;
	CT_OPEN2	make;
	/* place new 8-byte long members above this line */
	LONG	pad4[4]; /* reserved for future use - remeber to update cts_swap when using a element */
#ifdef ctFeatMAXFILE32
	LONG	filno;          /* file number */
#else
	LONG	filno32;        /* file number */
#endif
	LONG	drvfilemode;
	LONG	env;
	LONG	x8mode;
	/* place new 4-byte long members above this line */
	COUNT	pad2[4]; /* reserved for future use - remeber to update cts_swap when using a element */
#ifdef ctFeatMAXFILE32
	COUNT	filno16;        /* file number */
#else
	COUNT	filno;          /* file number */
#endif
	/* place new 2-byte long members above this line */
	TEXT	pad1[6]; /* reserved for future use */
	UTEXT	rechdr;
	TEXT	is_huge;
	TEXT	is_vlen;
	TEXT	srlsegsiz;
} CT_ADDIDX2_INPUT, *pCT_ADDIDX2_INPUT;
typedef struct {
	CT_INPUT_COMMON;
	CT_OPEN3	make;
	/* place new 8-byte long members above this line */
	LONG	pad4[4]; /* reserved for future use - remeber to update cts_swap when using a element */
#ifdef ctFeatMAXFILE32
	LONG	filno;          /* file number */
#else
	LONG	filno32;        /* file number */
#endif
	LONG	drvfilemode;
	LONG	env;
	LONG	x8mode;
	/* place new 4-byte long members above this line */
	COUNT	pad2[4]; /* reserved for future use - remeber to update cts_swap when using a element */
#ifdef ctFeatMAXFILE32
	COUNT	filno16;        /* file number */
#else
	COUNT	filno;          /* file number */
#endif
	/* place new 2-byte long members above this line */
	TEXT	pad1[18]; /* reserved for future use */
	UTEXT	rechdr;
	TEXT	is_huge;
	TEXT	is_vlen;
	TEXT	srlsegsiz;
	TEXT	var[MEMU];
} CT_ADDIDX3_INPUT, * pCT_ADDIDX3_INPUT;
typedef struct {
	CT_OUTPUT_COMMON;
	/* place new 8-byte long members above this line */
	/* place new 4-byte long members above this line */
	/* place new 2-byte long members above this line */
} CT_ADDIDX_OUTPUT, * pCT_ADDIDX_OUTPUT;

#define FUNC_CT_ADDIDX 16

#ifdef ctFeatRTG_HANDLE
#define FUNC_CTADDIDX(status, conn, in, lin, out, lout) CT_CALL(FUNC_CT_ADDIDX, &status, conn, (pCT_INPUT)in, lin, (pCT_OUTPUT)out, lout, NULL, NULL)
#else /* ~ctFeatRTG_HANDLE */
#define FUNC_CTADDIDX(status, conn, in, lin, out, lout, cnf) CT_CALL(FUNC_CT_ADDIDX, &status, conn, (pCT_INPUT)in, lin, (pCT_OUTPUT)out, lout, cnf, NULL)
#endif /* ~ctFeatRTG_HANDLE */
#ifdef ctFeatRTG_KEYS
#define FUNC__CTADDIDX(in, lin, out, lout) cts_addidx((pCT_ADDIDX3_INPUT)in, lin, (pCT_ADDIDX_OUTPUT)(pVOID)out, lout)
#else
#define FUNC__CTADDIDX(in, lin, out, lout) cts_addidx((pCT_ADDIDX2_INPUT)in, lin, (pCT_ADDIDX_OUTPUT)(pVOID)out, lout)
#endif

/*
**	REMOVE command - delete a file
*/

typedef struct {
	CT_INPUT_COMMON;
	/* place new 8-byte long members above this line */
	LONG	pad4[4];
	LONG	env;
	LONG	drvfilemode;
	/* place new 4-byte long members above this line */
	COUNT	pad2[8];
	/* place new 2-byte long members above this line */
	TEXT	pad1[16];
	TEXT	forcedelete;
	TEXT	datext[CT_EXTN_LEN];
	TEXT	idxext[CT_EXTN_LEN];
	TEXT	name[CT_PATH_LEN];
} CT_REMOVE1_INPUT, * pCT_REMOVE1_INPUT;
typedef struct {
	CT_INPUT_COMMON;
	/* place new 8-byte long members above this line */
	LONG	pad4[3];
	LONG	drvfilemod2;
	LONG	env;
	LONG	drvfilemode;
	ULONG	xmllen;
	/* place new 4-byte long members above this line */
	COUNT	pad2[5];
	UCOUNT	alteraction;
	/* place new 2-byte long members above this line */
	TEXT	pad1[15];
	UTEXT	wordlen;
	TEXT	forcedelete;
	TEXT	datext[CT_EXTN_LEN];
	TEXT	idxext[CT_EXTN_LEN];
	TEXT	name[CT_PATH_LEN];
	TEXT	var[MEMU];
} CT_REMOVE2_INPUT, * pCT_REMOVE2_INPUT;

typedef struct {
	CT_OUTPUT_COMMON;
	/* place new 8-byte long members above this line */
	RTGTASKID locker_taskid;
	/* place new 4-byte long members above this line */
	/* place new 2-byte long members above this line */
} CT_REMOVE_OUTPUT, * pCT_REMOVE_OUTPUT;

#define FUNC_CT_REMOVE 17

#define FUNC_CTREMOVE(status, conn, in, lin, out, lout, cnf, name) CT_CALL(FUNC_CT_REMOVE, &status, conn, (pCT_INPUT)in, lin, (pCT_OUTPUT)out, lout, cnf, name)
#define FUNC__CTREMOVE(in, lin, out, lout) cts_remove((pCT_REMOVE2_INPUT)in, lin, (pCT_REMOVE_OUTPUT)(pVOID)out, lout)

#define FUNC_CT_ALTER 22

#define FUNC_CTALTER(status, conn, in, lin, out, lout, cnf, name) CT_CALL(FUNC_CT_ALTER, &status, conn, (pCT_INPUT)in, lin, (pCT_OUTPUT)out, lout, cnf, name)
#define FUNC__CTALTER(in, lin, out, lout) cts_alter((pCT_REMOVE2_INPUT)in, lin, (pCT_REMOVE_OUTPUT)(pVOID)out, lout)

#define FUNC_CT_PARTITION 23

#define FUNC_CTPARTITION(status, conn, in, lin, out, lout, cnf, name) CT_CALL(FUNC_CT_PARTITION, &status, conn, (pCT_INPUT)in, lin, (pCT_OUTPUT)out, lout, cnf, name)
#define FUNC__CTPARTITION(in, lin, out, lout) cts_partition((pCT_REMOVE2_INPUT)in, lin, (pCT_REMOVE_OUTPUT)(pVOID)out, lout)

/*
**	RENAME command - rename a file
*/

typedef struct {
	CT_INPUT_COMMON;
	/* place new 8-byte long members above this line */
	LONG	pad4[4];
	LONG	env;
	LONG	drvfilemode;
	/* place new 4-byte long members above this line */
	COUNT	pad2[8];
	/* place new 2-byte long members above this line */
	TEXT	pad1[16];
	TEXT	overwrite;
	TEXT	fdatext[CT_EXTN_LEN];
	TEXT	tdatext[CT_EXTN_LEN];
	TEXT	fidxext[CT_EXTN_LEN];
	TEXT	tidxext[CT_EXTN_LEN];
	TEXT	fname[CT_PATH_LEN];
	TEXT	tname[CT_PATH_LEN];
} CT_RENAME1_INPUT, * pCT_RENAME1_INPUT;
typedef struct {
	CT_INPUT_COMMON;
	/* place new 8-byte long members above this line */
	LONG	pad4[3];
	LONG	drvfilemod2;
	LONG	env;
	LONG	drvfilemode;
	/* place new 4-byte long members above this line */
	COUNT	pad2[8];
	/* place new 2-byte long members above this line */
	TEXT	pad1[15];
	UTEXT	wordlen;
	TEXT	overwrite;
	TEXT	fdatext[CT_EXTN_LEN];
	TEXT	tdatext[CT_EXTN_LEN];
	TEXT	fidxext[CT_EXTN_LEN];
	TEXT	tidxext[CT_EXTN_LEN];
	TEXT	fname[CT_PATH_LEN];
	TEXT	tname[CT_PATH_LEN];
	TEXT	var[MEMU];
} CT_RENAME2_INPUT, * pCT_RENAME2_INPUT;

typedef struct {
	CT_OUTPUT_COMMON;
	/* place new 8-byte long members above this line */
	RTGTASKID locker_taskid;
	/* place new 4-byte long members above this line */
	/* place new 2-byte long members above this line */
} CT_RENAME_OUTPUT, * pCT_RENAME_OUTPUT;

#define FUNC_CT_RENAME 18

#define FUNC_CTRENAME(status, conn, in, lin, out, lout, cnf, name) CT_CALL(FUNC_CT_RENAME, &status, conn, (pCT_INPUT)in, lin, (pCT_OUTPUT)out, lout, cnf, name)
#define FUNC__CTRENAME(in, lin, out, lout) cts_rename((pCT_RENAME2_INPUT)in, lin, (pCT_RENAME_OUTPUT)(pVOID)out, lout)

/*
**	ACCESS command - check existence of a file/directory
*/

typedef struct {
	CT_INPUT_COMMON;
	/* place new 8-byte long members above this line */
	LONG	mode;
	LONG	pad4[3];
	/* place new 4-byte long members above this line */
	COUNT	pad2[8];
	/* place new 2-byte long members above this line */
	TEXT	pad1[16];
	TEXT	datext[CT_EXTN_LEN];
	TEXT	idxext[CT_EXTN_LEN];
	TEXT	path[CT_PATH_LEN];
	TEXT	var[MEMU];
} CT_ACCESS_INPUT, * pCT_ACCESS_INPUT;

typedef struct {
	CT_OUTPUT_COMMON;
	/* place new 8-byte long members above this line */
	ULONG	size;
	ULONG	size_h;
	LONG	pad4[2];
	/* place new 4-byte long members above this line */
	LONG	pad2[6];
	UCOUNT	cr_year;
	/* place new 2-byte long members above this line */
	TEXT	pad1[10];
	TEXT	exist;
	UTEXT	cr_mon;
	UTEXT	cr_mday;
	UTEXT	cr_hour;
	UTEXT	cr_min;
	UTEXT	cr_sec;
} CT_ACCESS_OUTPUT, * pCT_ACCESS_OUTPUT;

#define FUNC_CT_ACCESS 21

#define FUNC_CTACCESS(status, conn, in, lin, out, lout, cnf, name) CT_CALL(FUNC_CT_ACCESS, &status, conn, (pCT_INPUT)in, lin, (pCT_OUTPUT)out, lout, cnf, name)
#define FUNC__CTACCESS(in, lin, out, lout) cts_access((pCT_ACCESS_INPUT)in, lin, (pCT_ACCESS_OUTPUT)(pVOID)out, lout)



VOID CT_COMM_PRE(CT_INIT *conn, CT_COMM_INPUT *inp, CT_FILE *ct, COUNT contx);
VOID CT_COMM_POST(CT_INIT *conn, CT_COMM_OUTPUT *out, CT_FILE *ct);
VOID CT_WRITE_PRE(CT_INIT *conn, CT_WRITE_INPUT *inp, CT_FILE *ct, LONG mode);
VOID CT_WRITE_POST_OFFSETS(CT_FILE *ct, LONG record, LONG recordh, LONG mode, ULONG recnum);
VOID CT_WRITE_POST(CT_INIT *conn, CT_WRITE_OUTPUT *out, CT_FILE *ct, LONG mode) ;
VOID CT_READ_PRE(CT_INIT *conn, CT_READ_INPUT *inp, CT_FILE *ct, LONG mode, LONG mode2);
VOID CT_READ_POST_OFFSETS(CT_FILE *ct, LONG record, LONG recordh, LONG mode, ULONG recnum, pTEXT rec);
VOID CT_READ_POST(CT_INIT *conn, CT_READ_OUTPUT *out, CT_FILE *ct, LONG mode);
#ifdef ctFeatRTG_HANDLE
VOID CT_CALL(UTEXT func, pNINT status, CTHANDLE handle, pCT_INPUT in, VRLEN lin, pCT_OUTPUT out, VRLEN lout, pCONFFILE cnf, cpVOID extra);
VOID CTS_CALL(UTEXT func, pNINT status, CTHANDLE handle, pCT_INPUT in, VRLEN lin, pCT_OUTPUT out, VRLEN lout, pVOID cnf, cpVOID extra);
VOID CTL_CALL(UTEXT func, pNINT status, CTHANDLE handle, pCT_INPUT in, VRLEN lin, pCT_OUTPUT out, VRLEN lout, pCONFFILE cnf, cpVOID extra);
#else /* ~ctFeatRTG_HANDLE */
VOID CT_CALL(UTEXT func, pNINT status, pCT_INIT conn, pCT_INPUT in, VRLEN lin, pCT_OUTPUT out, VRLEN lout, pCONFFILE cnf, cpVOID extra);
VOID CTS_CALL(UTEXT func, pNINT status, pCT_INIT conn, pCT_INPUT in, VRLEN lin, pCT_OUTPUT out, VRLEN lout, pVOID cnf, cpVOID extra);
VOID CTL_CALL(UTEXT func, pNINT status, pCT_INIT conn, pCT_INPUT in, VRLEN lin, pCT_OUTPUT out, VRLEN lout, pCONFFILE cnf, cpVOID extra);
#endif /* ~ctFeatRTG_HANDLE */

#define CT_FILEOP_POST(conn,output) \
{ \
	if ((output)->err.ctree == FNOP_ERR && (output)->err.system == FCNF_COD) \
		conn->locker_taskid = (output)->locker_taskid; \
}
#define CT_OPEN_POST CT_FILEOP_POST
#define CT_MAKE_POST CT_FILEOP_POST
#define CT_REMOVE_POST CT_FILEOP_POST
#define CT_RENAME_POST CT_FILEOP_POST

#endif /* ~_CTCBMRSH_H_ */

/* end of ctcbmrsh.h */
