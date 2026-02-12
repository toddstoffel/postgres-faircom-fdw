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

#ifndef _CTCBXFD_H_
#define _CTCBXFD_H_

/****************************************************************************\
*
*	header files
*
\****************************************************************************/
#include "ctdbsdk.h"
#include "ctdbport.h"
#include "ctdbxml.h"
#include "ctcboptn.h"
#include "ctcbtype.h"

/****************************************************************************\
*
*   Protect against C++ compilers
*
\****************************************************************************/
#ifdef __cplusplus
extern "C" {
#endif
pCTDBLIST ctdbLOCAL _ctdbListAlloc(void);


/****************************************************************************\
*
*	CTXFDNODE
*	Definition of each parsed XML node.
*
\****************************************************************************/
typedef struct tagCTXFD		CTXFD, *pCTXFD;
typedef struct tagCTXFDKEY	CTXFDKEY, *pCTXFDKEY;
typedef struct tagCTXFDKEYSEG	CTXFDKEYSEG, *pCTXFDKEYSEG;
typedef struct tagCTXFDCONDDET	CTXFDCONDDET, *pCTXFDCONDDET;
typedef struct tagCTXFDFIELD	CTXFDFIELD, *pCTXFDFIELD;
typedef struct tagCTXFDOCCURS	CTXFDOCCURS, *pCTXFDOCCURS;

#define CTXFD_MAXSTRLEN 256
#define CTXFD_MAXSTACK 1024
#define CTXFD_MAXYYSTRLEN 1024
#define CTXFD_BTRV_VERSION 10000

struct tagCTXFD
{
	pVOID		yyscanner;
	pTEXT		instr;		/* input text to be parsed						*/
	NINT		size;		/* size in bytes of input text					*/
	NINT		pos;
	TEXT		errmsg[CTXFD_MAXSTRLEN];/* error message buffer							*/
	NINT		errflag;	/* error flag: >0 indicate errors. -1 indicates XFD in XML format 	*/
	NINT		debug; /* set to != 0 to output debug information */
	NINT		lineno;
	pVOID		stack[CTXFD_MAXSTACK];
	NINT		stackptr;

	/**/
	TEXT basename[CTXFD_MAXSTRLEN];
	TEXT selectname[CTXFD_MAXSTRLEN];
	TEXT xfd_fromacu;
	NINT xfdversion;
	NINT filetype;
	LONG minlen;
	LONG maxlen;
	LONG nbrofkeys;
	pCTDBLIST keys;
	LONG nbrofconds;
	pCTDBLIST conditions;
	pCTDBLIST fields;
	pCTDBLIST cout;
	pTEXT tmpptr; /* pointer used in the grammar to pass information during condition parsing */
};

struct tagCTXFDKEY
{
	LONG dupl;
	pCTDBLIST segments;
	pCTDBLIST fieldnames;
};

struct tagCTXFDKEYSEG
{
	LONG offset;
	LONG size;
};

struct tagCTXFDCONDDET
{
	LONG number;
	LONG type;
	LONG lcond;
	LONG rcond;
	pTEXT field;
	pTEXT value;
	pTEXT tblname;
	CTBOOL inuse;
};

#define BINDEFAULT 1
#define CBDEFAULT  2

struct tagCTXFDFIELD
{
	LONG    offset;
	LONG    size;
	COUNT   type;
	COUNT   digits;
	COUNT   scale;
	COUNT   usertype;
	COUNT   deftype;
	LONG    condition;
	LONG    level;
	LONG    lobhdloffset;
	TEXT    hidden;
	TEXT    indexed;
	TEXT    nullable;
	pTEXT   fld_name;
	pTEXT   datefmt;
	pTEXT   fld_name_ext;
	pTEXT   def;
	pTEXT   lobhdl;
};

struct tagCTXFDOCCURS
{
	LONG occur;
	LONG size;
	pCTDBLIST fields;
};

union tag_xfd_STYPE {
LONG yInt;
TEXT yStr[CTXFD_MAXYYSTRLEN];
pVOID yPtr;
} ;
typedef union tag_xfd_STYPE YYSTYPE;
#define YYSTYPE_IS_DECLARED 1
#define YYSTYPE_IS_TRIVIAL 1

/****************************************************************************/

#define YY_EXTRA_TYPE   pCTXFD

int     xfd_lex(YYSTYPE *, void *);
int     xfd_lex_init(void **);
int     xfd_lex_destroy(void *);
void    xfd_set_extra(YY_EXTRA_TYPE, void *);
int     xfd_parse(pCTXFD, void *);
void    xfd_error (pCTXFD, void *, pTEXT);

pTEXT fldstrdup (pTEXT const txt);
pCTXFDKEY XFDAllocKeyInfo();
pCTXFDKEYSEG XFDAllocKeySeg(LONG offset, LONG size);
pCTXFDCONDDET XFDAllocConditionDetails();
pCTXFDFIELD XFDAllocField();
pCTXFDOCCURS XFDAllocOccurs();
pCTXFDOCCURS XFDFreeOccurs (pCTXFDOCCURS occ);
pCTXFDFIELD XFDFieldDup(pCTXFDFIELD src);

NINT CTXFDPushStack(pCTXFD handle, pVOID ptr);
pVOID CTXFDPopStack(pCTXFD handle);
NINT CTXFDIsStackEmpty(pCTXFD handle);

VOID XFDParseASCII(pCTXFD handle, pTEXT buffer, NINT size);
pTEXT fldstrcpy (pTEXT dest, size_t destlen, const pTEXT src);

cbDLLexport pCTXFD XFDInit();
cbDLLexport VOID XFDFree(pCTXFD handle);
cbDLLexport NINT XFDParse(pCTXFD handle, pTEXT buffer, NINT size);
#ifdef ctFeatRTG_KEYS
cbDLLexport NINT GenCTMAKEfromXDD(pCTXML hXDD, pCT_OPEN3 makeinfo, pRTGKEY key);
#else /* ~ctFeatRTG_KEYS */
cbDLLexport NINT GenCTMAKEfromXDD(pCTXML hXDD, pCT_OPEN2 makeinfo, pCT_KEY2 key);
#endif /* ~ctFeatRTG_KEYS */
cbDLLexport size_t XFDGetXMLText (pCTXFD XFD, pTEXT buffer, size_t size);
cbDLLexport size_t XDDGetXMLText (pCTXML XDD, pTEXT buffer, size_t size);
cbDLLexport pCTXML ApplyXDDRules(pTEXT xdd, size_t xddlen, pTEXT rules, size_t ruleslen, pTEXT errmsg, size_t errmsglen, pNINT errflag);


/* BTRV DDF */

typedef struct {
	NINT offset;
	NINT len;
} DDF_fdef;

#define fileDDF_len(X) ((pTEXT)(X+fileDDF[0].offset))
#define fileDDF_id(X) ((fileDDF[1].len == 2)?((COUNT)*(pCOUNT)(X+fileDDF[1].offset)) : ((LONG)*(pLONG)(X+fileDDF[1].offset)))
#define fileDDF_name(X) ((pTEXT)(X+fileDDF[2].offset))
#define fileDDF_name_size ((COUNT)fileDDF[2].len)
#define fileDDF_location(X) ((pTEXT)(X+fileDDF[3].offset))
#define fileDDF_location_size ((COUNT)(fileDDF[3].len))
#define fileDDF_flags(X) ((fileDDF[4].len == 1)?((TEXT)(X[fileDDF[4].offset])) : ((LONG)*(pLONG)(X+fileDDF[4].offset)))
#define fileDDF_reserved(X) ((pTEXT)(X[fileDDF[5].offset]))

#define fieldDDF_len(X) ((pTEXT)(X+fieldDDF[0].offset))
#define fieldDDF_id(X) ((fieldDDF[1].len == 2)?((UCOUNT)*(pUCOUNT)(X+fieldDDF[1].offset)) : ((ULONG)*(pULONG)(X+fieldDDF[1].offset)))
#define fieldDDF_fileid(X) ((fieldDDF[2].len == 2)?((UCOUNT)*(pUCOUNT)(X+fieldDDF[2].offset)) : ((ULONG)*(pULONG)(X+fieldDDF[2].offset)))
#define fieldDDF_name(X) ((pTEXT)(X+fieldDDF[3].offset))
#define fieldDDF_name_size(X) fieldDDF[3].len
#define fieldDDF_type(X) ((UTEXT)(X[fieldDDF[4].offset]))
#define fieldDDF_offset(X) ((fieldDDF[5].len == 2)?((UCOUNT)*(pUCOUNT)(X+fieldDDF[5].offset)) : ((ULONG)*(pULONG)(X+fieldDDF[5].offset)))
#define fieldDDF_size(X) ((fieldDDF[6].len == 2)?((UCOUNT)*(pUCOUNT)(X+fieldDDF[6].offset)) : ((ULONG)*(pULONG)(X+fieldDDF[6].offset)))
#define fieldDDF_dec(X) ((fieldDDF[7].len == 1)?((UTEXT)(X[fieldDDF[7].offset])) : ((UCOUNT)*(pUCOUNT)(X+fieldDDF[7].offset)))
#define fieldDDF_flags(X) ((fieldDDF[8].len == 2)?((UCOUNT)*(pUCOUNT)(X+fieldDDF[8].offset)) : ((ULONG)*(pULONG)(X+fieldDDF[8].offset)))
#define fieldDDF_flags_size(X) fieldDDF[8].len


#define indexDDF_len(X) ((pTEXT)(X+indexDDF[0].offset))
#define indexDDF_fileid(X) ((indexDDF[1].len == 2)?((UCOUNT)*(pUCOUNT)(X+indexDDF[1].offset)) : ((ULONG)*(pULONG)(X+indexDDF[1].offset)))
#define indexDDF_fieldid(X) ((indexDDF[2].len == 2)?((UCOUNT)*(pUCOUNT)(X+indexDDF[2].offset)) : ((ULONG)*(pULONG)(X+indexDDF[2].offset)))
#define indexDDF_idxno(X) ((indexDDF[3].len == 2)?((UCOUNT)*(pUCOUNT)(X+indexDDF[3].offset)) : ((ULONG)*(pULONG)(X+indexDDF[3].offset)))
#define indexDDF_segno(X) ((indexDDF[4].len == 2)?((UCOUNT)*(pUCOUNT)(X+indexDDF[4].offset)) : ((ULONG)*(pULONG)(X+indexDDF[4].offset)))
#define indexDDF_flags(X) ((indexDDF[5].len == 2)?((UCOUNT)*(pUCOUNT)(X+indexDDF[5].offset)) : ((ULONG)*(pULONG)(X+indexDDF[5].offset)))


typedef enum BT_types
{
	BT_CHAR = 0,
	BT_INTEGER = 1,
	BT_FLOAT = 2,
	BT_DATE = 3,
	BT_TIME = 4,
	BT_DECIMAL = 5,
	BT_MONEY = 6,
	BT_LOGICAL = 7,
	BT_NUMERIC = 8,
	BT_BFLOAT = 9,
	BT_LSTRING = 10,
	BT_ZSTRING = 11,
	BT_UBINARY = 14,
	BT_AUTOINCR = 15,
	BT_BIT =16,
	BT_NUMERICSTS = 17,
	BT_NUMERICSA = 18,
	BT_CURRENCY = 19,
	BT_TIMESTAMP = 20,
	BT_BLOB = 21,
	BT_WSTRING = 25,
	BT_WZSTRING = 26,
	BT_GUID = 27,
	BT_NUMERICSLB = 28,
	BT_NUMERICSLS = 29,
	BT_DATETIME = 30,
	BT_NUMERICSTB = 31,
	BT_BINBUFF = 200, /* PF psql BINARY, this type is not defined in psql, added here to distinguish from BT_CHAR */
	BT_CLOB = 201, /* PF psql CLOB, this type is not defined in psql, added here to distinguish from BT_BLOB */
	BT_NOTNULL_IND = 227,
	BT_FILLER = 228, /* seems unused. using it because this is what is set in fld.type by XFDEmitXMLFiller*/
	BT_INT4 = 254, /* placeholder for 4 bytes integer (mapped to CT_INT4) */
	BT_IDX_NAME_IND = 255

} BT_types;

cbDLLexport pCTXFD DDF2XFD(DDF_fdef *fileDDF, pTEXT buffer, pTEXT ddfdir);
cbDLLexport pCTXFD DDF2XFD_2(DDF_fdef* fileDDF, pTEXT buffer, pTEXT ddfdir, TEXT uselogicalname);

#ifdef __cplusplus
}
#endif /* ~__cplusplus */

#define DEBUG_LEX        1
#define DEBUG_YRULES	 2

#endif /* ~_CTCBXFD_H_ */

/* end of ctcbxfd.h */
