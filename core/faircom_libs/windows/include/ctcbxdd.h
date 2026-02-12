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

#ifndef _CTCBXDD_H_
#define _CTCBXDD_H_

#include <assert.h>
#include "ctdbsdk.h"
#include "ctdbport.h"
#include "ctdbxml.h"

extern TEXT LOWVALUE_PLACEHOLDER;
extern TEXT HIGHVALUE_PLACEHOLDER;
extern TEXT SPACES_PLACEHOLDER;



#ifndef _CTATTRAPI_H_  /*if ctattrapi.h has not been included */
enum {
	A_SIGN_CONV = 0,
	I_SIGN_CONV = 1,
	M_SIGN_CONV = 2,
	N_SIGN_CONV = 3, /* ACU's -Dcn. This option is compatible with NCR COBOL. */
	V_SIGN_CONV = 4, /* ACU's -Dcv. This option is compatible with VAX COBOL. */
	B_SIGN_CONV = 5, /* ACU's -Dcb. MBP COBOL sign storage convention. */
	R_SIGN_CONV = 6,  /* ACU's -Dcr. Realia sign storage convention. */
	NO_SIGN_CONV = 15  /* sign conv not specified (used only when forcing a sign convention)*/
	/* NOTE: this value get casted to TEXT and the mask below applied */
};

#define SIGN_CONV_MASK (UTEXT)0xF0
#define SIGN_CONV_ERROR (UTEXT)0x00
#define SIGN_CONV_FORCEP (UTEXT)0x10
#define SIGN_CONV_FORCEN (UTEXT)0x20

enum {
	db_null,
	db_date,
	db_binary,
	db_boolean,
	db_clob,
	db_blob
};

enum {
	str_invalid = 0,
	str_ascii,
	str_utf8,
	str_utf16,
	str_utf16BE,
	str_utf16LE
};

typedef enum {
	tag_eq = 0,
	tag_neq = 1,
	tag_gt = 2,
	tag_gte = 3,
	tag_lt = 4,
	tag_lte = 5,
	tag_and = 6,
	tag_or = 7,
	tag_not = 8,
	tag_field = 9,
	tag_value = 10,
	tag_other = 11,
	tag_all = 12,
	tag_invalid = 13
} FILTERTAG;
#endif /* _CTATTRAPI_H_ */

typedef enum
{
	TABLELIST,
	FIELDLIST,
	FILTERLIST,
	IDXLIST,
	CONDLIST
} LISTTYPE;

#ifndef ctFeatRTG_ATTRAPI
typedef enum {
	XDD_CONV_NOERR = 0, /*CTDBRET_OK*/
	XDD_CONV_NOMRTMATCH = 1,
	XDD_CONV_NOMRTFILTERERR = 2,
	XDD_CONV_NOMEM = 3,
	XDD_CONV_UNDERLEN = 4,
	XDD_CONV_OVERLEN = 5,
	XDD_CONV_INTERNAL = 1000, /*CTDBRET_CALLBACK_5*/
	XDD_CONV_INVTYPE = 1001, /*CTDBRET_CALLBACK_19*/
	XDD_CONV_NOTSUPPORTED = 1002, /*CTDBRET_NOTSUPPORTED*/
	XDD_CONV_INVBOOL = 1003, /*CTDBRET_CALLBACK_10*/
	XDD_CONV_UNDERFLOW = 1004, /*CTDBRET_UNDERFLOW*/
	XDD_CONV_OVERFLOW = 1005, /*CTDBRET_OVERFLOW*/
	XDD_CONV_INVNUMBER = 1006, /*CTDBRET_INVNUMBER or CTDBRET_NULARG*/
	XDD_CONV_INVDATETIMEFORMAT = 1007, /*CTDBRET_INVFORMAT*/
	XDD_CONV_INVHOUR = 1008, /*CTDBRET_INVHOUR*/
	XDD_CONV_INVMINUTE = 1009, /*CTDBRET_INVMINUTE*/
	XDD_CONV_INVSECOND = 1010, /*CTDBRET_INVSECOND*/
	XDD_CONV_INVDATE =	1011, /*CTDBRET_INVDATE*/
	XDD_CONV_INVYEAR = 1012,/*CTDBRET_INVYEAR*/
	XDD_CONV_INVMONTH = 1013, /*CTDBRET_INVMONTH*/
	XDD_CONV_INVDAY	= 1014, /*CTDBRET_INVDAY*/
	XDD_CONV_ARGSMALL = 1015, /*CTDBRET_ARGSMALL*/
	XDD_CONV_NOTADIGIT = 1016, /*CTDBRET_CALLBACK_10*/
	XDD_CONV_BADSIGN = 1017, /*CTDBRET_CALLBACK_10*/
	XDD_CONV_INVINTSIZE = 1018, /*CTDBRET_CALLBACK_19*/
	XDD_CONV_DIGITOVERFLOW = 1019, /*CTDBRET_CALLBACK_10*/
	XDD_CONV_INVUTF = 1020 /*CTDBRET_CALLBACK_10*/
}XDD_CONV_RET;
#endif

typedef enum {
	fa_unknown = 0,
	fa_name = 1,
	fa_size = 2,
	fa_offset = 3,
	fa_type = 4,
	fa_digits = 5,
	fa_scale = 6,
	fa_dbtype = 7,
	fa_format = 8,
	fa_hidden = 9,
	fa_bindefault = 10,
	fa_cbdefault = 11,
	fa_onConvertError = 12,
	fa_onSignError = 13,
	fa_cbfalse = 14,
	fa_cbtrue = 15,
	fa_condition = 16,
	fa_jbase = 17,
	fa_strenc =18,
	fa_size_fld = 19,
	fa_nullindicator = 20,
	fa_size_item = 21,
	fa_max_size = 22,
	fa_binfalse = 23,
	fa_bintrue = 24,
	fa_bitnum = 25,
	fa_lob_handle = 26,
	fa_nullable = 27
} FIELDATTR;

#ifndef _CTATTRAPI_H_
/* conversion error behavior settings */
#define onConvertErrorUnDef 0
#define onConvertErrorNULL 1
#define onConvertErrorChkDef 2
#define onConvertErrorStrict 3
#define onConvertErrorValue 4
#endif /*_CTATTRAPI_H_*/

/* ctdb operations state */
#define STATE_NORMAL 0
#define STATE_ALTERFULL 1

/****************************************************************************\
*
*	FILTERINFO
*	Filter information retrieved from XML definitions.
*
\****************************************************************************/
typedef struct tagFILTERINFO
{
	UCOUNT	number;			/* filter number		*/
	pTEXT	table;			/* table view name		*/
	pTEXT	criteria;		/* filter criteria		*/
	COUNT	keyno;			/* filter key number		*/
	pTEXT	keyvalmin;		/* filter key value		*/
	VRLEN	keylenmin;		/* significant length of keyvalmin*/
	pTEXT	keyvalmax;		/* filter key max value (for range searches) */
	VRLEN	keylenmax;		/* significant length of keyvalmax*/
	NINT	keysearch;		/* filter key searching method */
	NINT	discr_off;		/* discriminant field offset */
	NINT	discr_size;		/* discriminant field size */
	FILTERTAG	discr_op;		/* discriminant operator for index */
	pTEXT	discr_value;		/* discriminant value */
} FILTERINFO, ctMEM* pFILTERINFO;

typedef struct tagFILTERLISTITEM
{
	NINT	number;
	pTEXT	table;
	pTEXT	content;
	NINT	keyno;
	pTEXT	keyvalmin;
	VRLEN	keylenmin;
	pTEXT	keyvalmax;
	VRLEN	keylenmax;
	NINT	keysearch;
	NINT	discr_off;
	NINT	discr_size;
	FILTERTAG	discr_op;
	pTEXT	discr_value;

} FILTERLISTITEM, ctMEM* pFILTERLISTITEM;

typedef struct tagIDXLISTITEM
{
/*	NINT	SegCount;*/
	NINT	duplicate;
	pCTDBLIST	fields;
	COUNT	matched;
	CTBOOL	primary;
} IDXLISTITEM, ctMEM* pIDXLISTITEM;

/****************************************************************************\
*
*	FIELDINFO
*	Field information retrieved from XML definitions.
*
\****************************************************************************/
typedef struct tagFIELDINFO
{
	UCOUNT	offset;			/* position in COBOL record	*/
	UCOUNT	size;			/* size of COBOL type		*/
	UCOUNT	type;			/* COBOL type				*/
	UCOUNT	dbtype;			/* SQL type					*/
	UCOUNT	digits;			/* numeric precision		*/
	UCOUNT	scale;			/* numeric scale			*/
	pTEXT	format;			/* SQL type format			*/
	/*UCOUNT	filter;	*/		/* MRT filter number		*/
	NINT	filterno;		/* VFIELD condition number */
	TEXT	idx_type;		/* index type allowed on the field	*/
	pTEXT	defaul;			/* default value			*/
	VRLEN	deflen;			/* default value buffer len */
	pTEXT	falsebuf;		/* false value in cobol buffer (we impose that the len matches the field len) */
	pTEXT	truebuf;		/* true value in cobol buffer (we impose that the len matches the field len) */
	pTEXT	errorSQLValue;		/* stringZ containing the SQL value in case of error. */
#ifdef DBDEFAULT
	pTEXT	ldefault;		/* logical default value, always a stringZ */
#endif
	/*pTEXT	cbdefault;*/ /* stringZ reppresenting default value for cobol */ /* PF 120403 not used */
	NINT	hidden;			/* the field is hidden in SQL */
	UCOUNT	doda_ftype;	/* type indicator				*/
	UCOUNT	doda_flen;	/* item length information			*/
	NINT	on_error;	/* on conversion error behavior */
	UTEXT	on_sign;	/* on sign conversion error */
	CTDATE	jbase;		/* julian date base			*/
	NINT	strenc;		/* character set using in cobol string */
	NINT	size_fld_idx; /* the actual position in the doda for the size field */
	TEXT	forced_conv; /* set to NO_SIGN_CONV to indicate no forcing */
	pTEXT	null_indicator; /* the name of the field where to store the null indicator*/
	NINT	null_ind_fldnum; /* the number of the field where to store the null indicator */
	UCOUNT	itemsize; /*occurs depending on, occurring item size */
	UCOUNT	maxsize; /* lob/occurs depending on max size in bytes */
} FIELDINFO, ctMEM* pFIELDINFO;

typedef struct tagFIELDLISTITEM
{
	pTEXT	name;
	NINT	size;
	NINT	offset;
	NINT	type;
	NINT	dbtype;
	NINT	digits;
	NINT	scale;
	pTEXT	format;
	NINT	hidden;
	NINT	on_error;
	NINT    filterno;
	pTEXT	defaul;
	VRLEN	deflen;
	UTEXT	on_sign;
#ifdef DBDEFAULT
	pTEXT	ldefault; /* logical default value, always a stringZ */
#endif
	pTEXT	cbdefault; /* stringZ reppresenting default value for cobol */
	pTEXT	cbfalse; /* stringZ reppresenting FALSE boolean value for cobol */
	pTEXT	cbtrue; /* stringZ reppresenting TRUE boolean value for cobol */
	pTEXT	binfalse;
	VRLEN	binfalselen;
	pTEXT	bintrue;
	VRLEN	bintruelen;
	pTEXT	errorSQLValue;	/* stringZ reppresenting the SQL value in case of error. */
	pTEXT	size_field;	/* stringZ reppresenting the name of the field containing the size. */
	CTDATE	jbase;
	NINT	strenc; /* character set using in cobol string */
	NINT	itemsize; /* occurs depending on, occurring item size */
	NINT	maxsize; /* occurs depending on, max size in bytes*/
	pTEXT	nullindicator;
} FIELDLISTITEM, ctMEM* pFIELDLISTITEM;

typedef struct tagIDXFIELDLISTITEM
{
	NINT	size;
	NINT	offset;
	COUNT	segmode;
} IDXFIELDLISTITEM, ctMEM* pIDXFIELDLISTITEM;

#ifndef ctFeatRTG_ATTRAPI
/****************************************************************************\
*
*	TABLEINFO
*	Table information retrieved from eXtended Data Definitions.
*
\****************************************************************************/
typedef struct
{
	VRLEN	minreclen;		/* minimum record length */
	VRLEN	maxreclen;		/* maximum record length */
	/* VRLEN	recsize;*/		/* record length */ /* 120402 Paolo does not see any use for it */
#ifdef FIELD_FIELD /* does not matter if defiend or not... here just as a bookmark for future change*/
	/* for now in but to be removed */
	COUNT	fixed_delta;	/* fixed size portion buffer delta due to conversion */
	COUNT	delta;			/* total buffer delta due to conversion */
	NINT	on_error;		/* see onConvertErrorACTIONS array in content (ctdbxdd.c)*/
#else
	COUNT	fixed_delta;	/* fixed size portion buffer delta due to conversion */
	COUNT	delta;			/* total buffer delta due to conversion */
	NINT	on_error;		/* see onConvertErrorACTIONS array in content (ctdbxdd.c)*/
#endif
	pDATOBJ	dodaptr;		/* pointer to the DODA */
	NINT	doda_size;		/* size of the DODA */
	NINT	dodacount;		/* number of field in the DODA */
	pFIELDINFO	dusrptr;	/* pointer to the DODA(XTD)*/
	NINT	dusr_size;		/* size of the DODA(XTD)*/
/* virtual fields */
	pDATOBJ	vdodaptr;		/* pointer to the vDODA */
	NINT	vdoda_size;		/* size of the vDODA */
	NINT	vdodacount;		/* number of field in the vDODA */
	pFIELDINFO	vdusrptr;	/* pointer to the vDODA(XTD)*/
	NINT	vdusr_size;		/* size of the vDODA(XTD)*/
	pCTDBLIST fldconditions; /* virtual fields condition list (filter text)*/
/* end of virtual fields */
	TEXT	sign_conv;		/* sign convention */
	pFILTERINFO	fltrptr;	/* pointer to filter info (MRT)*/
	NINT	fltrcount;		/* number of filters (MRT)*/
	TEXT	isvtablehost;		/* true if table is host for vtable */
	pISEG	isegbuf;		/* buffer for splitted isegs */
	COUNT	primary;		/* number of the primary index (if any)*/
	VRLEN	maxlen;			/* maximum field length (for field to be converted) */
	TEXT	tmap_ver;		/* type mapping version */
	NINT	rechdr;			/* record header size */
	TEXT	state;			/* use to track CTDB operation state (used during ctdbAlterTable) */
	COUNT	dupidx;			/* count of duplicate indices */
	pRESINFO RTGinfo;		/* RTG information set by ISAM API*/
} TABLEINFO, ctMEM* pTABLEINFO;
#endif

typedef struct tagTABLELISTITEM
{
	NINT	minreclen;
	NINT	maxreclen;
	NINT	on_error;
	TEXT	keydef;
	TEXT	tmap_ver;    /* type mapping version */
	pCTXMLNODE schema;   /* pointer to the schema definition in xdd tree */
} TABLELISTITEM, ctMEM* pTABLELISTITEM;

#define IS_NUMB_NEG(n)	((n.dec_digits[0] & 0x80) ? 0:1)
#define IS_INT_NEG(signed, a) ((signed)?((a&0x80) ? 1:0):0) /* it's the opposite with NUMB...*/

#ifndef ctFeatRTG_ATTRAPI
typedef struct
{
	char*	str;		/* mode name */
	NINT	mode;		/* mode value */
} CTXDDMODE, *pCTXDDMODE;
#endif


/* data type structures */
struct btdate {
	UTEXT day;
	UTEXT month;
	UCOUNT year;
};

struct bttime {
	UTEXT hsec;
	UTEXT sec;
	UTEXT min;
	UTEXT hours;
};

/****************************************************************************\
*
*	ctxmlCallback
*	Type of callback function that each node will call
*
\****************************************************************************/
typedef NINT (*ctxmlCallback)(pCTXMLNODE Node, CTXMLTYPE type, pTEXT name, pTEXT value, NINT level, pTEXT suffix);

/****************************************************************************\
*
*	c-tree XML function prototypes
*
\****************************************************************************/
#ifdef ctFeatRTG_ATTRAPI
COUNT _XDD2Attr(CTATTR_HANDLE TblHnd, FILNO datno, pRESINFO info, pTEXT XDD, pTEXT msg, size_t msglen);
COUNT cts_XDD2Attr(FILNO datno, pRESINFO info, pTEXT XDD, pTEXT msg, size_t msglen);
COUNT cts_Attr2XDD(FILNO datno, pRESINFO rtgi, size_t* bufsiz, pTEXT buf);
#else
XDD_CONV_RET ctdbDECL Cobol2Number(pUTEXT src, pFIELDINFO src_info, UTEXT src_conv, pCTNUMBER numb, pTEXT errinfo);
XDD_CONV_RET ctdbDECL Cobol2Integer(pUTEXT src, pFIELDINFO src_info, UTEXT src_conv, pCTBIGINT numb, pTEXT errinfo);
XDD_CONV_RET ctdbDECL NormalizeData(pUTEXT src, pTEXT dest, VRLEN dest_siz, UCOUNT Ftype, UCOUNT Fsize, UCOUNT Fdigits, UCOUNT Fscale, UTEXT src_conv, pTEXT errinfo);
CTDBRET ctdbDECL Number2Cobol(pCTNUMBER numb, pTEXT dst, UCOUNT Ftype, UCOUNT Fsize, UCOUNT Fdigits, UCOUNT Fscale, UTEXT dst_conv);
CTDBRET ctdbDECL Integer2Cobol(pUTEXT numb, CTDB_FIELD_METATYPE mt, pTEXT dst, UCOUNT Ftype, UCOUNT Fsize, UCOUNT Fdigits, UCOUNT Fscale, UTEXT dst_conv);
CTDBRET ctdbDECL Number2Int(pCTNUMBER numb, pTEXT dst, UCOUNT Ftype, UCOUNT Fsize, UCOUNT Fdigits, UCOUNT Fscale, UTEXT dst_conv);
CTDBRET ctdbDECL Cobolize(pUTEXT src, pTEXT dst, UCOUNT Ftype, UCOUNT Fsize, UCOUNT Fdigits, UCOUNT Fscale, UTEXT dst_conv);
pTEXT ConvertValue(pTEXT value, NINT type, NINT size, NINT digits, NINT scale, UTEXT conv);
CTDBRET MapError(XDD_CONV_RET errcode);
pCTDBLIST ctxmlGetTABLELIST(pCTXML handle, NINT filter, pRESINFO RTGinfo);
pCTDBLIST ctxmlGetFIELDLIST(pCTXML handle,  pCTXMLNODE parent, TEXT sign_conv, pRESINFO RTGinfo, pCTDBLIST cond_list);
pCTDBLIST ctxmlGetFILTERLIST(pCTXML handle, NINT filter, TEXT sign_conv, pRESINFO RTGinfo);
pCTDBLIST ctxmlGetCONDLIST(pCTXML handle, pCTXMLNODE parent, TEXT sign_conv, pRESINFO RTGinfo);
pCTDBLIST ctxmlGetIDXLIST(pCTXML handle, pRESINFO RTGinfo);
CTDBRET ctxmlAddIDX(pCTXML handle, pIDXLISTITEM item, NINT rechdr);
VOID ctxmlPutTABLELIST(pCTDBLIST list);
VOID ctxmlPutFIELDLIST(pCTDBLIST list);
VOID ctxmlPutFILTERLIST(pCTDBLIST list);
VOID ctxmlPutIDXLIST(pCTDBLIST list);
VOID ctxmlPutCONDLIST(pCTDBLIST list);
VOID ctxmlPutConvertedValue(pTEXT value);
#endif


#endif /* ~_CTCBXDD_H_ */

/* end of ctcbxdd.h */
