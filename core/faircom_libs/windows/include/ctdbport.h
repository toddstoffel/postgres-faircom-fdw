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

#ifndef _CTDBPORT_H_
#define _CTDBPORT_H_

/*^****************************************************************************\
*
*   Header Files
*
\******************************************************************************/
#include "ctreep.h"

/*^****************************************************************************\
*
*	Make sure UNICODE flag is set
*
\******************************************************************************/
#ifdef ctUNICODE
#ifndef ctdbUNICODE
#define ctdbUNICODE
#endif
#endif

#ifdef CTDB_SYMBOL_PREFIX
#include "ctdbsymb.h"
#endif

/*^****************************************************************************\
*
*   Protect against C++ compilers
*
\******************************************************************************/
#ifdef __cplusplus
extern "C" {
#endif
/*^****************************************************************************\
*
*   Misc defines for internal needs
*
\******************************************************************************/
#ifdef MULTITRD
#define ctdbMULTITRD
#endif
#ifdef ctSRVR
/*#define ctdbSETMSGLEN*/ /* Internal Server-side com buffer control */
#define ctdbSRVINTERNALUSERCOUNT /* Maintain Internal c-tree Server user count (ctnusers) */
#endif

#ifdef ctHUGEFILE
#define ctdb_ctGETHGH		ctGETHGH
#define ctdb_ctSETHGH		ctSETHGH
#else
#define ctdb_ctGETHGH()		(0)
#define ctdb_ctSETHGH(a)	((void)0)
#endif

#define ctdbMUST_FIX_LATER

#define ctdbSQL_TRANSACTION		1


/*^****************************************************************************\
*
*   ctdb max and min macros
*
\******************************************************************************/
#define ctdb_min(a,b)	((a) < (b) ? (a) : (b))
#define ctdb_max(a,b)	((a) > (b) ? (a) : (b))

/*^****************************************************************************\
*
*   CTDBK DEBUG FLAGS
*
\******************************************************************************/
#define CTDB_DEBUG_FLAG			0
#define CTDB_DEBUG_FILE			"ctdbdbug.txt"
#define CTDB_MAX_DISPLAY_DATA	64

typedef struct tagCTDBDBUG	CTDBDBUG, ctMEM* pCTDBDBUG, ctMEM* ctMEM* ppCTDBDBUG;

struct tagCTDBDBUG
{
	FILE*	file;
};


CTDBRET ctdbLOCAL _ctdbStartDebug(pCTDBDBUG hDebug, pTEXT fileName);
CTDBRET ctdbLOCAL _ctdbEndDebug(pCTDBDBUG hDebug);
pTEXT ctdbLOCAL _ctdbFieldTypeName(CTDBTYPE ftype);
/*^****************************************************************************\
*
*   C runtime functions
*
\******************************************************************************/
#ifndef ctdb_printf
#define ctdb_printf     printf /* ensure console (non-GUI) output */
#endif

#ifndef ctdb_sscanf
#define ctdb_sscanf	ctrt_sscanf
#endif

#ifndef ctdb_fscanf
#define ctdb_fscanf	ctrt_fscanf
#endif

#ifndef ctdb_fprintf
#define ctdb_fprintf    ctrt_fprintf
#endif

#ifndef ctdb_fopen
#define ctdb_fopen	ctrt_fopen
#endif

#ifndef ctdb_fclose
#define ctdb_fclose	ctrt_fclose
#endif

#ifndef ctdb_sizeof
#define ctdb_sizeof     sizeof
#endif

#ifndef ctdb_exit
#define ctdb_exit       ctrt_exit
#endif

#ifndef ctdb_abs
#define ctdb_abs        abs
#endif

#ifndef ctdb_labs
#define ctdb_labs       labs
#endif

#ifndef ctdb_fabs
#define ctdb_fabs       fabs
#endif

#ifndef ctdb_modf
#define ctdb_modf       modf
#endif

#ifndef ctdb_ceil
#define ctdb_ceil       ceil
#endif

#ifndef ctdb_floor
#define ctdb_floor      floor
#endif

#ifndef ctdb_access
#define ctdb_access	ctrt_access
#endif

#ifndef ctdb_memset
#define ctdb_memset     ctrt_memset
#endif

#ifndef ctdb_getcwd
#define ctdb_getcwd	ctrt_getcwd
#endif

#ifndef ctdb_chdir
#define ctdb_chdir	ctrt_chdir
#endif

#ifndef ctdb_memcpy
#define ctdb_memcpy     memcpy
#endif

#ifndef ctdb_memcmp
#define ctdb_memcmp     ctrt_memcmp
#endif

#ifdef InstantC
#define ctdb_memset     memset
#else
#undef  ctdb_memset /* RAB:060613: warning in ctPortNLM SQL Port */
#define ctdb_memset     ctrt_memset
#endif

#ifndef ctdb_strchr
#define ctdb_strchr     strchr
#endif

#ifndef ctdb_strrchr
#define ctdb_strrchr	strrchr
#endif

#ifndef ctdb_strlen
#define ctdb_strlen(p)  ((p) ? ctrt_strlen(p) : 0)
#endif

#ifndef ctdb_strncat_s
#define ctdb_strncat_s	ctrt_strncat_s
#endif

#ifndef ctdb_strncpy
#define ctdb_strncpy    ctrt_strncpy
#endif

#ifndef ctdb_vsnprintf
#define ctdb_vsnprintf	ctrt_vsnprintf
#endif

#ifndef ctdb_snprintf
#define ctdb_snprintf	ctrt_snprintf
#endif

#ifndef ctdb_strcmp
#ifdef ctdbUNICODE
#define ctdb_strcmp     ctdb_u8_strcmp
#else
#define ctdb_strcmp     ctrt_strcmp
#endif
#endif

#ifndef ctdb_strncmp
#ifdef ctdbUNICODE
#define ctdb_strncmp    ctdb_u8_strncmp
#else
#define ctdb_strncmp    strncmp
#endif
#endif

#ifndef ctdb_stricmp
#ifdef ctdbUNICODE
#define ctdb_stricmp    ctdb_u8_stricmp
#else
#define ctdb_stricmp    ctrt_stricmp
#endif
#endif

#ifndef ctdb_strstr
#define ctdb_strstr     strstr
#endif

#ifndef ctdb_atoi
#define ctdb_atoi       atoi
#endif

#ifndef ctdb_strtoul
#define ctdb_strtoul	strtoul
#endif

#ifndef ctdb_atof
#define ctdb_atof       atof
#endif

#ifndef ctdb_atol
#define ctdb_atol		atol
#endif

#ifndef ctdb_atoll
#define ctdb_atoll		atoll
#endif

#ifndef ctdb_isspace
#ifdef ctdbUNICODE
#define ctdb_isspace	ctdb_u8_isspace
#else
#define ctdb_isspace	isspace
#endif
#endif

#ifndef ctdb_isdigit
#ifdef ctdbUNICODE
#define ctdb_isdigit	ctdb_u8_isdigit
#else
#define ctdb_isdigit	isdigit
#endif
#endif


#ifndef ctdb_gmtime_r
#define ctdb_gmtime_r ctrt_gmtime_r
#endif
/*~****************************************************************************/

/*^****************************************************************************\
*
*   Currency defines
*
\******************************************************************************/
#define CTDB_MAX_TEXT		127
#define CTDB_MIN_TEXT		-128
#define CTDB_CURRENCY_DEC	10000
#define CTDB_CURRENCY_DIG	4
/*^****************************************************************************\
*
*   Path separator definitions
*
\******************************************************************************/
#ifndef ctdbPATH_SEP            /* if ctree has one defined */
#ifdef  ctPATH_SEP
#define ctdbPATH_SEP ctPATH_SEP
#else
#ifdef ctPortUNIX
#define ctdbPATH_SEP    '/'
#else
#define ctdbPATH_SEP    '\\'
#endif
#endif /* ~ctPATH_SEP   */
#endif /* ~ctdbPATH_SEP */

#ifndef ctdbEXT_SEP
#define ctdbEXT_SEP		'.'
#endif

#ifdef ctPortUNIX
#define CTDB_IS_PATHSEP(c) ((c) == '/')
#else
#define CTDB_IS_PATHSEP(c) ((c) == '/' || (c) == '\\')
#endif

ctdbEXPORT TEXT ctdbDECL _ctdbGetPathSeparator(CTHANDLE hSession);
/*~****************************************************************************/

/*^****************************************************************************\
*
*   CTDB Alignment
*
\******************************************************************************/
#define ctdbALIGNMENT		ALIGNMENT_COMP
/*~****************************************************************************/

/*^****************************************************************************\
*
*   c-tree Plus API calls implemented
*
\******************************************************************************/
#define ctapi_INTISAMX      INTISAMX
#define ctapi_CLISAM        CLISAM
#define ctapi_mballc        mballc
#define ctapi_mbfree        mbfree
#define ctapi_ctThrdInit    ctThrdInit
#define ctapi_ctThrdTerm    ctThrdTerm
#define ctapi_OPNRFILX      OPNRFILX
#define ctapi_OPNIFILX      OPNIFILX
#define ctapi_RBLIFILX      RBLIFILX
#define ctapi_RBLIFILX8     RBLIFILX8
#define ctapi_CMPIFILX      CMPIFILX
#define ctapi_CMPIFILX8     CMPIFILX8
#define ctapi_SETCBRBL      SETCBRBL
#define ctapi_CREIFILX      CREIFILX
#ifdef CT_V7                    /* Use only with V7 */
#define ctapi_CREIFILX8     CREIFILX8
#endif
#define ctapi_PUTDODA       PUTDODA
#define ctapi_PUTHDR        PUTHDR
#define ctapi_CLRFIL        CLRFIL
#define ctapi_CLIFIL        CLIFIL
#define ctapi_CLIFILz       CLIFILz
#define ctapi_OPNIFIL       OPNIFIL
#define ctapi_dltfil        dltfil
#define ctapi_SYSCFG        SYSCFG
#define ctapi_GETIFIL       GETIFIL
#define svapi_igetifil      igetifil
#define ctapi_GETDODA       GETDODA
#define ctapi_ctadjadr      ctadjadr
#define ctapi_ctotAlign     ctotAlign
#define ctapi_GETFIL        GETFIL
#define ctapi_LKISAM        LKISAM
#define ctapi_ADDVREC       ADDVREC
#define ctapi_ADDREC        ADDREC
#define ctapi_TRANBEG       TRANBEG
#define ctapi_TRANABT       TRANABT
#define ctapi_TRANABTX      TRANABTX
#define ctapi_TRANRDY       TRANRDY
#define ctapi_TRANSAV       TRANSAV
#define ctapi_SPCLSAV       SPCLSAV
#define ctapi_TRANRST       TRANRST
#define ctapi_TRANEND       TRANEND
#define ctapi_SAVPCLR       SAVPCLR
#define ctapi_EQLREC        EQLREC
#define ctapi_FRSREC        FRSREC
#define ctapi_FRSVREC       FRSVREC
#define ctapi_LSTREC        LSTREC
#define ctapi_LSTVREC       LSTVREC
#define ctapi_REDVREC       REDVREC
#define ctapi_RWTREC        RWTREC
#define ctapi_SETCURI       SETCURI
#define ctapi_RWTVREC       RWTVREC
#define ctapi_RWTPREC       RWTPREC
#define ctapi_UPDCURI       UPDCURI
#define ctapi_RRDREC        RRDREC
#define ctapi_GETVLEN       GETVLEN
#define ctapi_GETCURP       GETCURP
#define ctapi_DELVREC       DELVREC
#define ctapi_DELREC        DELREC
#define ctapi_NXTREC        NXTREC
#define ctapi_NXTVREC       NXTVREC
#define ctapi_OPNIFILX      OPNIFILX
#define ctapi_PRMIIDX       PRMIIDX
#define ctapi_PRMIIDX8      PRMIIDX8
#define ctapi_TMPIIDX       TMPIIDX
#define ctapi_TMPIIDX8      TMPIIDXX8
#define ctapi_DELRFIL       DELRFIL
#define ctapi_REDIREC       REDIREC
#define ctapi_ctsfill       ctsfill
#define ctapi_frmkey        frmkey
#define ctapi_frmkeyx       frmkeyx
#define ctapi_GTEREC        GTEREC
#define ctapi_GTREC         GTREC
#define ctapi_LTEREC        LTEREC
#define ctapi_LTREC         LTREC
#define ctapi_PRVREC        PRVREC
#define ctapi_PRVVREC       PRVVREC
#define ctapi_FRSSET        FRSSET
#define ctapi_NXTSET        NXTSET
#define ctapi_PRVSET        PRVSET
#define ctapi_LSTSET        LSTSET
#define ctapi_RRDREC        RRDREC
#define ctapi_memcmp        ctdb_memcmp
#define ctapi_OPNICON       OPNICON
#define ctapi_CLSICON       CLSICON
#define ctapi_CHGICON       CHGICON
#define ctapi_CHGSET        CHGSET
#define ctapi_DATENT        DATENT
#define ctapi_REDIVREC      REDIVREC
#define ctapi_EQLVREC       EQLVREC
#define ctapi_GTEVREC       GTEVREC
#define ctapi_GTVREC        GTVREC
#define ctapi_LTVREC        LTVREC
#define ctapi_LTEVREC       LTEVREC
#define ctapi_FRSVSET       FRSVSET
#define ctapi_NXTVSET       NXTVSET
#define ctapi_PRVVSET       PRVVSET
#define ctapi_LSTVSET       LSTVSET
#define ctapi_RRDVREC       RRDVREC
#define ctapi_DATENT        DATENT
#define ctapi_IDXENT        IDXENT
#define ctapi_ctDISTINCTest ctDISTINCTest
#define ctapi_ctDISTINCTset ctDISTINCTset
#define ctapi_cpybuf        cpybuf
#define ctapi_AVLFILNUM		AVLFILNUM
#define ctapi_CREDATX		CREDATX
#define ctapi_CREDATX8		CREDATX8
#define ctapi_CLSFIL		CLSFIL
#define ctapi_OPNFIL		OPNFIL
#define ctapi_OPNFILX		OPNFILX
#define ctapi_DELFIL		DELFIL
#define ctapi_ADDRES		ADDRES
#define ctapi_GETRES		GETRES
#define ctapi_UPDRES		UPDRES
#define ctapi_DELRES		DELRES
#define ctapi_frmkey		frmkey
#define ctapi_FRSSET		FRSSET
#define ctapi_LSTSET		LSTSET
#define ctapi_PRVSET		PRVSET
#define ctapi_NXTSET		NXTSET
#define ctapi_FRESET		FRESET
#define ctapi_MIDSET		MIDSET
#define ctapi_REGCTREE		REGCTREE
#define ctapi_UNRCTREE		UNRCTREE
#define ctapi_SWTCTREE		SWTCTREE
#define ctapi_WCHCTREE		WCHCTREE
#define ctapi_DELIFIL		DELIFIL
#define ctapi_TFRMKEY		TFRMKEY
#define ctapi_LOKREC		LOKREC
#define ctapi_DROPIDX		ctDROPIDX
#define ctapi_FRESETN		FRESETN
#define ctapi_TRANISO		TRANISO
#define ctapi_FRSKEY		FRSKEY
#define ctapi_LSTKEY		LSTKEY
#define ctapi_PRVKEY		PRVKEY
#define ctapi_NXTKEY		NXTKEY
#define ctapi_EQLKEY		EQLKEY
#define ctapi_LTKEY			LTKEY
#define ctapi_LTEKEY		LTEKEY
#define ctapi_GTKEY			GTKEY
#define ctapi_GTEKEY		GTEKEY
#define ctapi_PUTFIL		PUTFIL
#define ctapi_PUTIFILX		PUTIFILX
#define ctapi_GETNAM		GETNAM
#define ctapi_RENIFILX		RENIFILX

#ifdef CTDB_FILTER_020617
#define ctapi_SETFLTR		SETFLTR
#define ctapi_SETFLTRN		SETFLTRN
#define ctapi_SETFLTRX		SETFLTRX
#endif

#define ctapi_ALCRNG		ALCRNG
#define ctapi_FRERNG		FRERNG
#define ctapi_FRSRNG		FRSRNG
#define ctapi_LSTRNG		LSTRNG
#define ctapi_NXTRNG		NXTRNG
#define ctapi_PRVRNG		PRVRNG
#define ctapi_FRSVRNG		FRSVRNG
#define ctapi_LSTVRNG		LSTVRNG
#define ctapi_NXTVRNG		NXTVRNG
#define ctapi_PRVVRNG		PRVVRNG

#define ctapi_SETNODE		SETNODE

#define ctapi_PUTKSEGDEF	PUTKSEGDEF
#define ctapi_GETKSEGDEF	GETKSEGDEF

#define ctapi_SYSCFG		SYSCFG
#define ctapi_BATSETX		BATSETX
#define ctapi_CHGBAT		CHGBAT
#define ctapi_FREBATN		FREBATN

#define ctapi_GETXCREBLK	GETXCREBLK
#define ctapi_SETOPS		SETOPS
#define ctapi_BLKIREC		BLKIREC
#define ctapi_RNGENT		RNGENT
#define ctapi_PTADMIN		PTADMIN

/*~****************************************************************************/

/*^****************************************************************************\
*
* Defaults used for CTDB_LOGON (INTISAMX)
*
\******************************************************************************/
#define ctdbDEFAULT_ibuffers	64
#define ctdbDEFAULT_dbuffers	64
#define ctdbDEFAULT_bufs	    10
#define ctdbDEFAULT_fils	    32
#define ctdbDEFAULT_sect	    32
#define ctdbDEFAULT_dbufs	    10


#ifdef ctMTFPG
#define ctdbDEFAULT_userprf (USERPRF_NTKEY)		     /* NO AUTO TFRMKEY */
#else
#ifdef ctSRVR
#define ctdbDEFAULT_userprf (USERPRF_NTKEY)		     /* NO AUTO TFRMKEY */
#else
#define ctdbDEFAULT_userprf (USERPRF_NTKEY | USERPRF_CLRCHK) /* NO AUTO TFRMKEY */
				      			     /* clear transaction logs */
#endif
#endif

#ifdef ctFeatMAX_NAME_4K
#define CTDB_MAXPATH			(MAX_NAME+256)
#else
#define CTDB_MAXPATH			512
#endif
/*~****************************************************************************/

/*^****************************************************************************\
*
*   Defaults used to open a file
*
\******************************************************************************/
#define ctdbOPEN_FILEMODE 		    (ctVIRTUAL | ctSHARED)
#define ctdbEXCLUSIVE_MODE 		    (ctEXCLUSIVE | ctPERMANENT)
#ifdef TRANPROC
#define ctdbCREATE_DATA_FILEMODE 	(ctEXCLUSIVE | ctPERMANENT | ctTRNLOG | ctFIXED)
#define ctdbCREATE_INDEX_FILEMODE	(ctTRNLOG)
#else
#define ctdbCREATE_DATA_FILEMODE 	(ctEXCLUSIVE | ctPERMANENT | ctFIXED)
#define ctdbCREATE_INDEX_FILEMODE	(0)
#endif
#define ctdbSOPEN_FILEMODE 		    (ctEXCLUSIVE | ctVIRTUAL)
/*~****************************************************************************/


/*^****************************************************************************\
*
*   Date conversions
*
\******************************************************************************/
#define NXTCENTURY	        40	    /* any 2 digit year < NXTCENTURY implies 20xx */
/*~****************************************************************************/

/*^****************************************************************************\
*
*   Default file extensions
*
\******************************************************************************/
#define ctdbDAT_EXTENT	    ".dat"
#define ctdbIDX_EXTENT	    ".idx"
#define ctdbDAT_NULEXT	    ""
/*~****************************************************************************/

/*^****************************************************************************\
*
*   Defaults used to create a table for the first time
*
\******************************************************************************/
#define ctdbDEFAULT_dxtdsiz	8192	        /* default data file ext size	*/
#define ctdbDEFAULT_ixtdsiz	8192	        /* index file ext size		    */
/*~****************************************************************************/

/*^****************************************************************************\
*
*   CTDB C API internal type definitions
*
\******************************************************************************/
typedef pIFIL    ctMEM *ppIFIL;
typedef pDATOBJ  ctMEM *ppDATOBJ;
typedef pConvMap ctMEM *ppConvMap;
/*~****************************************************************************/

/*^****************************************************************************\
*
*   CTDB C API internal error reporting
*
\******************************************************************************/
#define CTDBERROR(h,c)	if ((c)!=CTDBRET_OK) ctdbSetError(h,(c))
/*~****************************************************************************/

/*^****************************************************************************\
*
*   Session drop file list handle
*
\******************************************************************************/
typedef struct tagCTDBDROP	CTDBDROP, ctMEM* pCTDBDROP, ctMEM* ctMEM* ppCTDBDROP;
#define CTDBDROP_SIZE		ctdb_sizeof(struct tagCTDBDROP)

struct tagCTDBDROP
{
	FILNO	filno; /* Could be either a datno or keyno */
	pTEXT	fname;
	CTBOOL	isindex;
};

CTDBRET ctdbLOCAL _ctdbAddDropList(CTHANDLE Handle, FILNO datno, pTEXT fname, CTBOOL isindex);
CTDBRET ctdbLOCAL _ctdbDelDropList(CTHANDLE Handle, FILNO datno, pTEXT fname);
CTDBRET ctdbLOCAL _ctdbClearDropList(CTHANDLE Handle);
CTDBRET ctdbLOCAL _ctdbAbortDropList(CTHANDLE Handle);

/*~****************************************************************************/


/*^****************************************************************************\
*
*   Session alter table information block
*	keeps information of tables that performed full alter table during the
*	current transaction
*
\******************************************************************************/
typedef struct tagCTDBALTER	CTDBALTER, ctMEM* pCTDBALTER, ctMEM* ctMEM* ppCTDBALTER;
#define CTDBALTER_SIZE		sizeof(CTDBALTER)

struct tagCTDBALTER
{
	pTEXT		org_name;	/* original table name */
	pTEXT		org_path;	/* original table path */
	FILNO		org_datno;	/* original table datno */
	CTOPEN_MODE	org_mode;	/* original table open mode */
	pTEXT		tmp_name;	/* temporary table name */
	pTEXT		tmp_path;	/* temporary table path */
	FILNO		tmp_datno;	/* temporary table datno */
	CTOPEN_MODE	tmp_mode;	/* temporary table open mode */
};
/*~****************************************************************************/


/*^****************************************************************************\
*
*   Superfile support
*
\******************************************************************************/
typedef struct tagCTDBSUPER		CTDBSUPER;

struct tagCTDBSUPER
{
	pTEXT	name;		/* superfile name: drive + directory + name + ext	*/
	FILNO	datno;		/* superfile data file number						*/
	CTBOOL	isexcl;		/* indicates if superfile was open exclusive		*/
	CTHANDLE Handle;	/* session handle									*/
	CTDB_TABLE_TYPE mode; /* indicate if superfile is session or database	*/
#ifdef CTDB_DICTTRANCRTL
	CTBEGIN_MODE dict_tranmode;
#endif
#ifdef CTDB_SUPERFILE_AUTO
	CTCREATE_MODE filmod;
#endif
};

pCTDBSUPER ctdbLOCAL _ctdbSuperAlloc(CTHANDLE Handle);
pCTDBSUPER ctdbLOCAL _ctdbSuperFree(pCTDBSUPER pSuper);
CTDBRET ctdbLOCAL _ctdbSuperSetPath(pCTDBSUPER pSuper, pTEXT path);
CTDBRET ctdbLOCAL _ctdbSuperSetName(pCTDBSUPER pSuper, pTEXT path, pTEXT name, pTEXT ext);
pTEXT ctdbLOCAL _ctdbSuperGetName(pCTDBSUPER pSuper);
CTBOOL ctdbLOCAL _ctdbSuperIsOpen(pCTDBSUPER pSuper);
CTBOOL ctdbLOCAL _ctdbSuperIsExclusive(pCTDBSUPER pSuper);
CTDBRET ctdbLOCAL _ctdbSuperClose(pCTDBSUPER pSuper);
CTDBRET ctdbLOCAL _ctdbSuperCreate(pCTDBSUPER pSuper);
CTDBRET ctdbLOCAL _ctdbSuperOpen(pCTDBSUPER pSuper, CTBOOL exclusive);
CTDBRET ctdbLOCAL _ctdbSuperReopen(pCTDBSUPER pSuper, CTBOOL exclusive);
CTDBRET ctdbLOCAL _ctdbSuperDrop(pCTDBSUPER pSuper);
/*~****************************************************************************/

/*^****************************************************************************\
*
*   Default monetary fields precision and scale
*	The default precision and scale of the "numeric" or monetary capable field
*	types is set the default maximum values that each field type can hold:
*
*	CT_MONEY:
*	based on a four byte signed integer, the maximum value for CT_MONEY is
*	21474836.47 which gives it a precision of 10 (notice that some 10 digits values
*   are outside of the data type boundaries) the decimal point should not be counted)
*   and a scale of 2.
*
*	CT_CURRENCY:
*	based on a eight byte signed integer, the maximum value for CT_CURRENCY is
*	922337203685477.5807 which gives it a precision of 19 (notice that some 19
*	 digits values ar outside the boundaries data type boundaries) and a scale of 4.
*
*	CT_NUMBER
*	based on a scaled BCD implementation, the maximum value of CT_NUMBER is
*	99999999999999999999999999999999 which gvies it a precision of 32 and a
*	scale of 0. CT_NUMBER scale may vary to implement different values such
*	as 999999999999999999999999.99999999 whose precision is still 32 and the
*	scale is 8.
*
\******************************************************************************/
#define CT_MONEY_PRECISION		10
#define CT_CURRENCY_PRECISION	19
#define CT_NUMBER_PRECISION		32

#define CT_MONEY_SCALE			2
#define CT_CURRENCY_SCALE		4
#define CT_NUMBER_SCALE			0
/*~****************************************************************************/

/*^****************************************************************************\
*
*   UNIFRMAT support
*
\******************************************************************************/
#ifdef CTDB_UNIFRMAT
#define ctSWAP16(n)    ((((n) & 0x00ff) << 8) | (((n) & 0xff00) >> 8))
#define ctSWAP32(n)    ((ctSWAP16((n) & 0xffff) << 16) | (ctSWAP16(((n) & 0xffff0000ul) >> 16)))

#ifdef UNIFRMAT
#define _ctdbSwap16(v)	ctSWAP16(v)
#define _ctdbSwap32(v)	ctSWAP32(v)
#else /* UNIFRMAT */
#ifdef ctCLIENT
#ifdef ctPortWINKE_CLIENT
#define _ctdbSwap32(v) (v)
#define _ctdbSwap16(v) (v)
#else
#define _ctdbSwap32(v)	((ctsflvr[ctBYTEORDER] && ctsflvr[ctBYTEORDER] != ct_uflvr) ? ctSWAP32(v) : (v))
#define _ctdbSwap16(v)	((ctsflvr[ctBYTEORDER] && ctsflvr[ctBYTEORDER] != ct_uflvr) ? ctSWAP16(v) : (v))
#endif
#endif /* ctCLIENT */
#endif /* UNIFRMAT */
#endif /* CTDB_UNIFRMAT */

#ifndef ctSWAP16
#define ctSWAP16(n)		(n)
#endif

#ifndef ctSWAP32
#define ctSWAP32(n)		(n)
#endif

#ifndef _ctdbSwap32
#define _ctdbSwap32(v)	(v)
#endif

#ifndef _ctdbSwap16
#define _ctdbSwap16(v)	(v)
#endif
/*~****************************************************************************/

/*^****************************************************************************\
*
*   Generic resource buffer
*
\******************************************************************************/
#define CTDB_RES_RESERVED	65536

typedef struct
{
	ULONG	restyp;
	ULONG	resnum;
	TEXT	resnam[1];
} CTDBRES, ctMEM* pCTDBRES;

typedef struct
{
	ULONG	restyp;
	ULONG	resnum;
	ULONG	reslen;
} CTDBRESLEN, ctMEM* pCTDBRESLEN;
/*~****************************************************************************/


/*^****************************************************************************\
*
*   CTDB_RESOURCE_TYPE
*
\******************************************************************************/
#define CTDB_RESOURCE_TYPE 		FC_CTDB
/*~****************************************************************************/

/*^****************************************************************************\
*
*   Table/field additional information resources
*
\******************************************************************************/
#define FLDRES_NUM			FCRES_DFLD
#define FLDRESDFTL_NUM			FCRES_DFL2
#define FLDRES_NAME			FCRES_DFLD_NAME
#define FLDRESDFTL_NAME			FCRES_DFL2_NAME
#define FLDRES_SIZE			20

/*#define FLDRES_VER			4 */ /* added binflg */
/*#define FLDRES_VER			5 */ /* added deflen and capability to store default values in a separate resource */
#define FLDRES_VER			6 /* added deftype to store default value type */


typedef struct
{
	COUNT	nullflag;	/* NO field can be nul YES field can not be nul	*/
	UCOUNT	prec;		/* field precision				*/
	COUNT	scale;		/* field scale					*/
	COUNT	binflag;	/* indicate how to handle binary fields		*/
	LONG	deflen;		/* default field value length			*/
	COUNT	deftype;	/* default field type (literals vs. others)     */
	TEXT	reserved[18];	/* reserved for future use			*/
} CTFLDX, ctMEM* pCTFLDX;

typedef struct
{
	ULONG	rtype;		/* resource type: UIDR_TYPE		*/
	ULONG	rnum;		/* resource number: UIDR_NUM		*/
	TEXT	rname[8];	/* resource name: UIDR_NAME		*/
	ULONG	version;	/* resource version number		*/
	CTFLDX	fldr[4];	/* field additional information		*/
} CTDBFLDR, ctMEM * pCTDBFLDR;

typedef struct
{
	ULONG	rtype;		/* resource type: UIDR_TYPE	*/
	ULONG	rnum;		/* resource number: UIDR_NUM	*/
	TEXT	rname[8];	/* resource name: UIDR_NAME	*/
	ULONG	version;	/* resource version number	*/
	TEXT	defvalues[1];	/* default field values		*/
} CTDBFLDR_DFTL, ctMEM * pCTDBFLDR_DFTL;

typedef struct
{
	pTEXT	defvalue;
	VRLEN	defvaluelen;
	CTDEF_TYPE	defvaluetype;
} CTDBFLD_DFTL_VALUE, ctMEM * pCTDBFLD_DFTL_VALUE;



pCTDBFLDR ctdbLOCAL _ctdbAllocFldx(CTHANDLE hTable);
CTDBRET ctdbLOCAL _ctdbPutFldx(CTHANDLE hTable);
CTDBRET ctdbLOCAL _ctdbGetFldx(CTHANDLE hTable);

CTDBRET ctdbLOCAL _sqlFixError(CTDBRET errcode, CTDB_FIELD_METATYPE rtype, pUTEXT src, VRLEN srclen, pUTEXT dst, NINT on_error, pTEXT errorSQLValue, pTEXT defval, VRLEN deflen, TEXT nulls);
#ifdef CTDB_TBLATTR
CTDBRET ctdbLOCAL _ctdbPutTblXtdInfo(CTHANDLE hTable, LONG forcedversion, CTATTR_HANDLE TblAttrHnd, CTATTR_HANDLE SchAttrHnd);
CTDBRET ctdbLOCAL _ctdbGetTblXtdInfo(CTHANDLE hTable, UINT VTableNumb);
CTATTR_HANDLE ctdbLOCAL _ctdbLoadTblAttr(pCTDBTABLE pTable, CTDBRET *status);
CTDBRET ctdbLOCAL _ctdbSyncIndexAttributes(pCTDBTABLE pTable, CTATTR_HANDLE TblHnd, CTBOOL save);
CTDBRET ctdbLOCAL _ctdbSyncSchemaAttributes(pCTDBTABLE pTable, CTBOOL save);
CTDBRET ctdbLOCAL _ctdbSyncTableAttributes(pCTDBTABLE pTable, CTBOOL save);
VOID ctdbLOCAL _ctdb_freeTableAttributes(pCTDBTABLEATTR attr);

CTDBRET ctdbLOCAL _ctdbDODAFromAttr(CTATTR_HANDLE SchAttrHnd, UTEXT rechdr, CTATTR_RTGNUMCONV numconv, TEXT rowidx, UTEXT srlsegsiz, UTEXT struct_align, ppConvMap schemaptr, ppDATOBJ dodaptr, pVRLEN schemasize, pVRLEN dodasize, TEXT mode);
VOID ctdbLOCAL CTATTR_FLDINFO_to_CTDBFIELDATTR(CTATTR_FLDINFO* attr, pCTDBFIELDATTR fldinfo, VRLEN fieldlen);
CTDBRET ctdbLOCAL _ctdbFilterFromAttr(CTATTR_HANDLE Hnd, CTATTR_HANDLE ParentHnd, VRLEN rechdr, CTATTR_RTGNUMCONV numconv, TEXT usefldNames, pVOID out);


#ifdef ctPortRTG
CTDBRET RTGXDD_FixSegMode(pCTDBFIELD pField, CTSEG_MODE* SegMode);
CTDBRET RTGXDD_AttrTableLoad(FILNO datno, pVOID localTag, CTDBRET rv, CTATTR_HANDLE TblHnd);
CTDBRET RTGXDD_GetSchema(pCTDBTABLE pTable, CTDBRET rv);
CTDBRET RTGXDD_GetDoda(pCTDBTABLE pTable, CTDBRET rv);
CTDBRET RTGXDD_TableClose(pCTDBTABLE pTable);
CTDBRET RTGXDD_AlterTable(pCTDBTABLE pTable, pNINT action);
CTDBRET RTGXDD_AttrTableSave(CTATTR_HANDLE Handle, FILNO datno);
#endif

#endif
pUTEXT ctdbLOCAL String2Foreign(pUTEXT invalue, NINT outtype, NINT outsize, NINT outdigits, NINT outscale, CTATTR_RTGNUMCONV outconv, CTDBRET* error);
CTDBTYPE ctdbLOCAL _ctdbRemapFieldType(pCTDBFIELD pField);
CTDBRET ctdbLOCAL _NormalizeForeignData(pUTEXT src, pUTEXT dest, VRLEN dest_siz, UCOUNT Ftype, UCOUNT Fsize, UCOUNT Fdigits, UCOUNT Fscale, UTEXT src_conv, pTEXT errinfo, size_t errinfolen);
pVOID ctdbDECL _ctdbOnAlloc(VRLEN size);
void ctdbDECL _ctdbOnFree(pVOID ptr);


/*~****************************************************************************/

/*^****************************************************************************\
*
*   CTBINARY type
*
\******************************************************************************/
typedef struct tagCTBINARY	ctMEM* pCTBINARY, ctMEM* ctMEM* ppCTBINARY;

struct tagCTBINARY
{
	LONG	size;
	UTEXT	data[1024 * 32];
};
/*~****************************************************************************/

/*^****************************************************************************\
*
*   Field padding resource
*
\******************************************************************************/
#ifdef CTDB_PAD_RESOURCE
#define CTDBFPAD_SIZE		sizeof(CTDBFPAD)
#define CTDBFPAD_NUM		FCRES_DPAD
#define CTDBFPAD_NAME		FCRES_DPAD_NAME
#define CTDBFPAD_VERSION	2

typedef struct
{
	ULONG	rtype;		/* resource type: UIDR_TYPE		*/
	ULONG	rnum;		/* resource number: UIDR_NUM	*/
	TEXT    rname[8];	/* resource name: UIDR_NAME		*/
	ULONG	version;	/* resource version number		*/
	ULONG	high;		/* resource offset high 32 bits	*/
	ULONG	low;		/* resource offset low 32 bits	*/
	LONG	padchar;	/* padding character			*/
	LONG	dlmchar;	/* field delimiter character	*/
	ULONG	reserved[55]; /* reserved for future use	*/
} CTDBFPAD, ctMEM* pCTDBFPAD;

CTDBRET ctdbLOCAL _ctdbGetPadResource(pCTDBTABLE pTable, pCTDBFPAD pPad, CTBOOL LockIt);
CTDBRET ctdbLOCAL _ctdbAddPadResource(pCTDBTABLE pTable, pCTDBFPAD pPad);
CTDBRET ctdbLOCAL _ctdbUpdPadResource(pCTDBTABLE pTable, pCTDBFPAD pPad);
CTDBRET ctdbLOCAL _ctdbReadPadChar(pCTDBTABLE pTable);
#endif /* CTDB_PAD_RESOURCE */

CTDBRET ctdbLOCAL _ctdbPadField(pTEXT pField, NINT field_size, NINT field_len, NINT padchar, NINT dlmchar, CTATTR_DLMPOS_VALUES dlmpos );
CTDBRET ctdbLOCAL _ctdbTrimField(pTEXT pField, NINT field_size, pNINT field_len, NINT padchar, NINT dlmchar, CTATTR_DLMPOS_VALUES dlmpos);
NINT ctdbLOCAL _ctdb_GetSQLMaxFieldLen(CTHANDLE Handle);
/*~****************************************************************************/

/* check that _DICT_MAXRES is positive */
typedef char checkDictMaxResSize[_DICT_MAXRES > 0];

#define _DICT_PERMMASK          (OPF_ALL | GPF_READ | GPF_WRITE | WPF_READ | WPF_WRITE)
#define _DICT_SESSION_EXT       ".fsd"
#define _DICT_DATABASE_EXT      ".fdd"
#define _DICT_SUPER_EXT         ".tbl"
#define _DICT_DATA_EXT          ".dat"
#define _DICT_INDEX_EXT         ".idx"

typedef enum
{
	_DICT_FLD_TYPE      = 0,
	_DICT_FLD_STATUS    = 1,
	_DICT_FLD_LOGICNAME = 2,
	_DICT_FLD_LINK      = 3,
	_DICT_FLD_LINKNBR   = 4,
	_DICT_FLD_PATH      = 5,
	_DICT_FLD_SUPEXT    = 6,
	_DICT_FLD_DATEXT    = 7,
	_DICT_FLD_IDXEXT    = 8,
	_DICT_FLD_VERSION   = 9,
	_DICT_FLD_COUNTER   =10,
	_DICT_FLD_UID       =11,
	_DICT_FLD_OWNER     =12,
	_DICT_FLD_PHYNAME   =13,
	_DICT_FLD_RESERVED  =14
} DICTFLDNUMB;

typedef struct
{
	TEXT	 op;
	pTEXT	 fName;
	CTDBTYPE fType;
	NINT	 iNbr;
	DICTFLDNUMB	 fNbr;
	NINT	 fLen;
} CTDBFILEDEF;


/*~****************************************************************************/

/*^****************************************************************************\
*
*   CTDB C API Dictionary structure
*
\******************************************************************************/
struct tagCTDBDICT
{
    CTHANDLE 	hSession;          			/* Session handle             		*/
	pCTDBSUPER	hSuper;				/* Dictionary superfile handle		*/
	pTEXT		hSupername;			/* Superfile name			*/
	pCTDBTABLE	hTable;				/* Dictionary table handle		*/
	pCTDBRECORD	hRecord;			/* Dictionary record handle		*/
	CTBOOL		is_session;			/* indicates session dictionary		*/
	CTBOOL		is_exclusive;			/* Dictionary open exclusive		*/
	CTBOOL		is_active;			/* Dictionary is open			*/
	pTEXT		filename;			/* Dictionary original filename		*/
	CTBOOL		locked;				/* indicate if dictionary is locked	*/
	CTBOOL		suspended;			/* ISAM locks suspended			*/
	CTLOCK_MODE	lockmode;			/* previous lock mode			*/
};
/*~****************************************************************************/

#ifdef CTDB_TBLATTR
/*^****************************************************************************\
*
*   CTDB C API Attributes functions
*
\******************************************************************************/
pCTDBTABLEATTR ctdbLOCAL _ctdb_allocTableAttributes();
VOID ctdbLOCAL ctdb_freeTableAttributes(pCTDBTABLEATTR attr);

pCTDBFIELDATTR ctdbLOCAL _ctdb_allocFieldAttributes();
VOID ctdbLOCAL _ctdb_freeFieldAttributes(pCTDBFIELDATTR attr);
#endif

#define _ctdb_isForeignDataType(dtype) (dtype >= CT_ALPHAEDIT && dtype <= CT_BT_BLOB)


/*^****************************************************************************\
*
*   CTDB C API Dictionary functions
*
\******************************************************************************/
pCTDBDICT ctdbLOCAL _ctdbAllocDict(CTHANDLE Handle, CTBOOL is_session);
VOID ctdbLOCAL _ctdbFreeDict(pCTDBDICT pDict);
CTDBRET ctdbLOCAL _ctdbCreateDict(pCTDBDICT pDict, pTEXT FileName, CTBOOL superfile);
CTDBRET ctdbLOCAL _ctdbOpenDict(pCTDBDICT pDict, pTEXT FileName, CTBOOL OpenExclusive, CTBOOL superfile);
CTDBRET ctdbLOCAL _ctdbReopenDict(pCTDBDICT pDict, CTBOOL OpenExclusive);
CTDBRET ctdbLOCAL _ctdbCloseDict(pCTDBDICT pDict);
CTDBRET ctdbLOCAL _ctdbDeleteDict(pCTDBDICT pDict);
CTDBRET ctdbLOCAL _ctdbClearDict(pCTDBDICT pDict);
CTDBRET ctdbLOCAL _ctdbWriteDict(pCTDBDICT pDict);
CTDBRET ctdbLOCAL _ctdbRemoveDict(pCTDBDICT pDict, ULONG type, ULONG Status, cpTEXT Name);
CTDBRET ctdbLOCAL _ctdbRemoveDictByUID(pCTDBDICT pDict, ULONG uid);
CTDBRET ctdbLOCAL _ctdbFirstDict(pCTDBDICT pDict, ULONG Type, ULONG Status);
CTDBRET ctdbLOCAL _ctdbNextDict(pCTDBDICT pDict, ULONG Type, ULONG Status);
CTDBRET ctdbLOCAL _ctdbFindDict(pCTDBDICT pDict, ULONG Type, ULONG Status, cpTEXT Name);
CTDBRET ctdbLOCAL _ctdbFindDictByUID(pCTDBDICT pDict, ULONG uid);
CTDBRET ctdbLOCAL _ctdbGetDict(pCTDBDICT pDict, pDICTDATA pData);
CTDBRET ctdbLOCAL _ctdbPutDict(pCTDBDICT pDict, pDICTDATA pData);
CTDBRET ctdbLOCAL _ctdbGetDictUID(pCTDBDICT pDict, pULONG puid);
NINT ctdbLOCAL _ctdbGetDictCount(pCTDBDICT pDict, ULONG Type, ULONG Status);
FILNO ctdbLOCAL _ctdbGetDatno(CTHANDLE hTable);
CTDBRET ctdbLOCAL _ctdbLockDict(pCTDBDICT pDict, CTLOCK_MODE mode);
CTDBRET ctdbLOCAL _ctdbUnlockDict(pCTDBDICT pDict);

CTDBRET ctdbLOCAL _ctdbDictAddDatabase(CTHANDLE hSession, pTEXT name, pTEXT path, CTBOOL superfile);
CTDBRET ctdbLOCAL _ctdbDictDropDatabase(CTHANDLE hSession, pTEXT name);
CTDBRET ctdbLOCAL _ctdbDictFirstDatabase(CTHANDLE hSession, pDICTDATA pData);
CTDBRET ctdbLOCAL _ctdbDictNextDatabase(CTHANDLE hSession, pDICTDATA pData);
CTDBRET ctdbLOCAL _ctdbDictFindDatabase(CTHANDLE hSession, cpTEXT name, pDICTDATA pData);
CTDBRET ctdbLOCAL _ctdbDictFindDatabaseByUID(CTHANDLE hSession, ULONG uid, pDICTDATA pData);
NINT ctdbLOCAL _ctdbDictGetDatabaseCount(CTHANDLE hSession);
CTDBRET ctdbLOCAL _ctdbDictGetDatabaseUID(CTHANDLE hSession, cpTEXT name, pULONG puid);
CTDBRET ctdbLOCAL _ctdbDictAddTable(CTHANDLE hDatabase, pTEXT logical_name, pTEXT physical_name, pTEXT path, pTEXT fext, pTEXT mirr_name, pTEXT mirr_path, UCOUNT dict_mark, pTEXT owner);
CTDBRET ctdbLOCAL _ctdbDictAddActiveTable(CTHANDLE hDatabase, pCTDBTABLE pTable);
CTDBRET ctdbLOCAL _ctdbDictDropTable(CTHANDLE hDatabase, cpTEXT name);
CTDBRET ctdbLOCAL _ctdbDictFirstTable(CTHANDLE hDatabase, pDICTDATA pData);
CTDBRET ctdbLOCAL _ctdbDictNextTable(CTHANDLE hDatabase, pDICTDATA pData);
CTDBRET ctdbLOCAL _ctdbDictFindTable(CTHANDLE hDatabase, cpTEXT name, pDICTDATA pData);
CTDBRET ctdbLOCAL _ctdbDictFindTableByUID(CTHANDLE hDatabase, ULONG uid, pDICTDATA pData);
CTDBRET ctdbLOCAL _ctdbDictReserveTable(CTHANDLE hDatabase, pTEXT logical_name, pTEXT physical_name, pULONG puid);
NINT ctdbLOCAL _ctdbDictGetTableCount(CTHANDLE hDatabase);
CTDBRET ctdbLOCAL _ctdbDictGetTableUID(CTHANDLE hDatabase, pTEXT name, pULONG puid);

CTDBRET ctdbLOCAL _ctdbDictAddIndex(CTHANDLE hDatabase, CTHANDLE hTable, NINT indexno);
CTDBRET ctdbLOCAL _ctdbDictAddAllIndex(CTHANDLE hDatabase, CTHANDLE hTable);
CTDBRET ctdbLOCAL _ctdbDictUpdateIndex(CTHANDLE hDatabase, CTHANDLE hTable);
CTDBRET ctdbLOCAL _ctdbDictDropIndex(CTHANDLE hDatabase, cpTEXT name);
CTDBRET ctdbLOCAL _ctdbDictDropAllIndex(CTHANDLE hDatabase, cpTEXT name);
CTDBRET ctdbLOCAL _ctdbDictFindIndex(CTHANDLE hDatabase, cpTEXT name, pDICTDATA pData);
CTDBRET ctdbLOCAL _ctdbDictFindIndexByUID(CTHANDLE hDatabase, ULONG uid, pDICTDATA pData);
CTDBRET ctdbLOCAL _ctdbDictReserveIndex(CTHANDLE hDatabase, cpTEXT name, pULONG puid);
CTDBRET ctdbLOCAL _ctdbDictUpdateIndexName(CTHANDLE hDatabase, cpTEXT newname, cpTEXT oldname);

CTDBRET ctdbLOCAL _ctdbDumpDict(CTHANDLE hSession, CTHANDLE hDatabase);
CTDBRET ctdbLOCAL _ctdbDictMarkSQLDB(CTHANDLE hDatabase, UCOUNT SQLDBVersion, NINT sqlmark);
UCOUNT ctdbLOCAL _ctdbDictGetSQLDBVersion(CTHANDLE hDatabase);
CTDBRET ctdbLOCAL _ctdbDictMoveTable(CTHANDLE hDatabase, cpTEXT oldname, cpTEXT newpath, cpTEXT new_logical, cpTEXT new_physical);

CTDBRET ctdbLOCAL _ctdbSYSCFG(pVOID bufptr);
CTDBRET ctdbLOCAL _ctdbGETNAM(FILNO filno, pTEXT buffer, VRLEN buflen, COUNT mode);
FILNO ctdbLOCAL _ctdbIdxno(pCTDBRECORD pRec);
CTDBRET ctdbLOCAL _ctdbDictAddVTable(pCTDBDATABASE pDatabase, pTEXT ParentName, pTEXT Name, UINT Number, VTABLE_TYPE Vtype);
#ifdef ctdbMULTITRD
pVOID ctdbLOCAL _ctdbGetSessionlqmsg(CTHANDLE Handle);
pVOID ctdbLOCAL _ctdbSetSessionlqmsg(CTHANDLE Handle, pVOID ptr);
#endif
/*~****************************************************************************/

/*^****************************************************************************\
*
*   Default defines for CTDB SDK tables
*
\******************************************************************************/
#define CTDB_DEF_TABLENAME          "table"
#define CTDB_DEF_TABLEFEXT          ".dat"
#define CTDB_DEF_TABLEIEXT          ".idx"
#define CTDB_DEF_CREATEMODE         (ctVLENGTH)
#define CTDB_DEF_OPENMODE           (ctVIRTUAL | ctSHARED)
#define CTDB_DEF_OPENMODE_CREATE    (ctPERMANENT | ctEXCLUSIVE)
#ifdef _CTATTRAPI_H_  /*ctattrapi.h included*/
#define CTDB_DEL_FIELD              CTATTR_DEL_FIELD
#define CTDB_NUL_FIELD              CTATTR_NUL_FIELD
#else
#define CTDB_DEL_FIELD              "$DELFLD$"
#define CTDB_NUL_FIELD              "$NULFLD$"
#endif
#define CTDB_DEL_NUMBER     		-1
#define CTDB_NUL_NUMBER     		-2

#define CTDB_RECBYT_INDEX			"$RECBYT$"
#define CTDB_RECBYT_KEYLEN			9
#define CTDB_RECBYT_KEYDUP			1
#define CTDB_ROWID_INDEX			"$ROWID$"
#define CTDB_ROWID_KEYLEN			8
#define CTDB_ROWID_KEYDUP			0
#ifdef _CTATTRAPI_H_  /*ctattrapi.h included*/
#define CTDB_ROWID_FIELD			CTATTR_ROWID_FIELD
#else
#define CTDB_ROWID_FIELD			"$ROWID$"
#endif
#define CTDB_ROWID_NUMBER			-3

#define CTDB_ROWID_FLDHANDLE		(pVOID)(NLONG)0x0001  /* invalid pointer used as placehoder */
#define CTDB_INTERNAL_FIELDS		3
#define CTDB_DEF_PERMMASK           (OPF_ALL | GPF_READ | GPF_WRITE | WPF_READ | WPF_WRITE)
/*~****************************************************************************/

/*^****************************************************************************\
*
*   CTDB C API Handle list functions
*
\******************************************************************************/
CTDBRET ctdbLOCAL _ctdbDeactivateChildren(CTHANDLE parent);
CTDBRET ctdbLOCAL _ctdbLinkChild(CTHANDLE child);
CTDBRET ctdbLOCAL _ctdbUnlinkChild(CTHANDLE child);
CTDBRET ctdbLOCAL _ctdbUnlinkChildren(CTHANDLE parent, NINT handle_id);
/*~****************************************************************************/

/*^****************************************************************************\
*
*   CTDB C API Record movement
*
\******************************************************************************/
#define CTDB_MOVE_FIRST     1
#define CTDB_MOVE_LAST      2
#define CTDB_MOVE_NEXT      3
#define CTDB_MOVE_PREV      4
#define CTDB_MOVE_JUMP      5
/*~****************************************************************************/


/*^****************************************************************************\
*
*   CTDB C API field, index name trimming
*
\******************************************************************************/
#define CTDB_NAME_NOTTRIMMED 0x00
#define CTDB_NAME_LTRIMMED 0x01
#define CTDB_NAME_RTRIMMED 0x02
#define CTDB_NAME_TRIMMED (CTDB_NAME_LTRIMMED|CTDB_NAME_RTRIMMED)
/*~****************************************************************************/


/*^****************************************************************************\
*
*   CTDB C API Utility functions
*
\******************************************************************************/
ctdbEXPORT pVOID _ctdb_alloc(VRLEN size, VRLEN count);
ctdbEXPORT pVOID _ctdb_alloc_ex(VRLEN size, VRLEN count, CTDB_ALLOC_TYPE type);
ctdbEXPORT pVOID _ctdb_realloc(pVOID ptr, VRLEN size, VRLEN count, VRLEN newCount);
ctdbEXPORT pVOID _ctdb_free(pVOID ptr);
ctdbEXPORT pVOID _ctdb_free_ex(pVOID ptr, CTDB_ALLOC_TYPE type);
ctdbEXPORT pTEXT _ctdb_stralloc(VRLEN size);
ctdbEXPORT pTEXT _ctdb_wstralloc(VRLEN size);
ctdbEXPORT pTEXT _ctdb_strdup(cpTEXT str);
ctdbEXPORT pTEXT _ctdb_strlastchar(pTEXT str);
CTHANDLE ctdbLOCAL _ctdbSelectHandle(CTHANDLE Handle, NINT HandleId);
NINT ctdbLOCAL _ctdbGetHandleId(CTHANDLE Handle);
pTEXT ctdbLOCAL _ctdb_MakeFilePath(cpTEXT Path, cpTEXT Name, TEXT path_sep, pVRLEN pBufferSize);
VRLEN ctdbLOCAL _ctdb_lastdelimiter(cpTEXT str, TEXT delimiter);
CTDBRET ctdbLOCAL _ctdb_GetFilePath(cpTEXT FileName, pTEXT pPath, VRLEN PathSize, TEXT path_sep);
CTDBRET ctdbLOCAL _ctdb_GetFileName(cpTEXT FileName, pTEXT pName, VRLEN NameSize, TEXT path_sep);
CTDBRET ctdbLOCAL _ctdb_GetFileNameXtd(cpTEXT FileName, pTEXT pName, VRLEN NameSize, TEXT path_sep, pTEXT ext);
CTDBRET ctdbLOCAL _ctdb_GetFileExt(cpTEXT FileName, pTEXT pExt, VRLEN ExtSize, TEXT path_sep);
#ifdef CTDB_MIRROR
CTDBRET ctdbLOCAL _ctdb_GetMirrorFileName(cpTEXT FileName, pTEXT pName, VRLEN NameSize, TEXT path_sep);
#endif
CTDBRET ctdbLOCAL _ctdb_FullPath(pTEXT Path, VRLEN PathSize);
CTDBRET ctdbLOCAL _ctdbFileExist(pTEXT Path, pTEXT Name, pTEXT Ext, TEXT path_sep);
VOID ctdbLOCAL _ctdbFileNameOnly(pTEXT Name);
NINT ctdbLOCAL _ctdbLocateDatabase(pCTDBSESSION pSession, pTEXT Name);
NINT ctdbLOCAL _ctdbLocateTable(pCTDBDATABASE pDatabase, cpTEXT Name);
CTDBRET ctdbLOCAL _ctdbUnlinkTable(pCTDBTABLE pTable, pTEXT fName, pTEXT fPath, pTEXT fExt, pTEXT password);

#ifdef CTDB_PATHPREFIX
CTDBRET ctdbLOCAL _ctdbFixPathPrefix(CTHANDLE Handle, pTEXT inpath, pTEXT outpath);
CTDBRET ctdbLOCAL _ctdbFixIfilPath(CTHANDLE Handle, pIFIL infil, ppIFIL outfil);
void ctdbLOCAL _ctdbFixIfilFree(pIFIL ifilptr);
#endif

CTDBRET ctdbLOCAL _ctdbClearAlterList(pCTDBTABLE pTable);
CTDBRET ctdbLOCAL _ctdbCleanupAlterTable(pCTDBSESSION pSession, CTBOOL isAbort);

CTDBRET ctdbLOCAL _ctdbGetSerial(pCTDBTABLE pTable, pCTROWID rowid);
CTDBRET ctdbLOCAL _ctdbGetSerialExt(pCTDBTABLE pTable, pULONG pHigh, pULONG pLow, CTBOOL Incr);
CTHANDLE ctdbLOCAL _ctdbPutIndex(CTHANDLE Handle, pTEXT name, CTDBKEY KeyType, CTBOOL AllowDuplicates, CTDB_NULKEY NullFlag);
CTDBRET ctdbLOCAL _ctdb_AddIDField(CTHANDLE hTable);
CTDBRET ctdbLOCAL _ctdb_AddChangeIDField(CTHANDLE hTable);
pA_STFIELDS ctdbLOCAL _ctdb_genAutoSysTimeFields_defs(pCTDBTABLE pTable);
CTDBRET ctdbLOCAL _ctdb_saveAutoSysTimeFields(pCTDBTABLE pTable, pA_STFIELDS pdefs, TEXT remove);
CTBOOL ctdbLOCAL _ctdbIsInvalidName( cpTEXT Name, NINT trim_check);
CTDBRET ctdbLOCAL _ctdbAddFromDefs (pCTDBTABLE pTable, CTDBFILEDEF *defs);
CTBOOL ctdbLOCAL _ctdbIsSegKsegUsed(pCTDBISEG pIseg);
#ifdef ctdbUNICODE
CTBOOL ctdbLOCAL _ctdbIsIdxKsegUsed(pCTDBINDEX pIdx);
CTBOOL ctdbLOCAL _ctdbIsTblKsegUsed(pCTDBTABLE pTable);
#endif
#ifdef CTDB_CNDX_IDX
pTEXT ctdbLOCAL _ctdbGetIdxcndxExpr(pCTDBINDEX pIndex);
#endif
CTBOOL ctdbLOCAL _ctdbGetIdxTempFlag(pCTDBINDEX pIndex);
COUNT ctdbLOCAL _ctdbGetIndexikeytyp(CTHANDLE Handle);
CTDBRET ctdbLOCAL _ctdbSetDatabaseDropLastMRT(CTHANDLE Handle, CTBOOL value);
CTDBRET ctdbLOCAL _ctdbSetDatabaseDropDelOpt(CTHANDLE Handle, TEXT value);
VRLEN ctdbLOCAL _ctdbGetPhysicalFieldLength(CTHANDLE Handle);
CTDBRET ctdbLOCAL _ctdbSetCreateMode(CTHANDLE Handle, CTCREATE_MODE mode);
CTDBRET ctdbLOCAL _ctdbRetrieveSegmentKSEG(pCTDBISEG pSeg, NINT segno);
pctKSEGDEF ctdbLOCAL _ctdbBuildKSEG(pCTDBISEG pISeg);
CTHANDLE ctdbDECL _ctdbGetIndex(CTHANDLE Handle, NINT IndexNumber, CTBOOL internals);

/*^****************************************************************************\
*
*   CTDB list functions
*
\******************************************************************************/
#define CTDBLIST_EXPAND			32
#define CTDBLIST_RESIZE			32

typedef struct tagCTDBLIST CTDBLIST;

struct tagCTDBLIST
{
	NINT	size;
	NINT	count;
	ppVOID	list;
};

ctdbEXPORT pCTDBLIST _ctdbListAlloc(void);
ctdbEXPORT pCTDBLIST _ctdbListDup(pCTDBLIST hList);
ctdbEXPORT pCTDBLIST _ctdbListFree(pCTDBLIST hList);
ctdbEXPORT NINT _ctdbListCount(pCTDBLIST hList);
ctdbEXPORT NINT _ctdbListSize(pCTDBLIST hList);
ctdbEXPORT pVOID _ctdbListItem(pCTDBLIST hList, NINT Index);
ctdbEXPORT CTDBRET _ctdbListAdd(pCTDBLIST hList, pVOID Item);
ctdbEXPORT CTDBRET _ctdbListClear(pCTDBLIST hList);
ctdbEXPORT CTDBRET _ctdbListDelete(pCTDBLIST hList, NINT Index);
ctdbEXPORT CTDBRET _ctdbListDeleteEx(pCTDBLIST hList, pVOID Item);
ctdbEXPORT CTDBRET _ctdbListExchange(pCTDBLIST hList, NINT Index1, NINT Index2);
ctdbEXPORT CTDBRET _ctdbListExpand(pCTDBLIST hList);
ctdbEXPORT NINT _ctdbListIndexOf(pCTDBLIST hList, pVOID Item);
ctdbEXPORT CTDBRET _ctdbListInsert(pCTDBLIST hList, NINT Index, pVOID Item);
ctdbEXPORT CTDBRET _ctdbListMove(pCTDBLIST hList, NINT CurIndex, NINT NewIndex);
ctdbEXPORT CTDBRET _ctdbListReplace(pCTDBLIST hList, NINT Index, pVOID Item);
ctdbEXPORT VOID _ctdbListSort(pCTDBLIST hList, int (* _PtListCompare)(const void *, const void*));

/*~****************************************************************************/

/*^****************************************************************************\
*
*   CTDB File/Directory management API.
*
\*****************************************************************************/


/*^****************************************************************************\
*
*	CTDBFF
*	Portable File Find structure
*
\******************************************************************************/
#ifdef ctPortUNIX
#define  ctDIRENT_H	<dirent.h>
#include ctDIRENT_H
#endif
#ifdef ctPortWINKE
typedef void *WIN32_FIND_DATA;
#endif
typedef struct
{
#ifdef ctPortWIN32
#ifndef ctPortNLM
	HANDLE	hFind;
#if defined (ctPortWINCE_SRVR) || defined (UNICODE)
	WIN32_FIND_DATAW fd;
	wchar_t name[MAX_NAME];	/* needed for UNICODE convertion */
#else
	WIN32_FIND_DATA fd;
#endif
#endif
#endif
#ifdef ctPortUNIX
	DIR		*hFind;
	struct dirent *fd;
	struct dirent *buffer;
	TEXT path[CTDB_MAXPATH];
#endif
#ifdef ctPortNLM
	DIR 		*hFind;
	struct dirent *fd;
	TEXT path[CTDB_MAXPATH];
#endif
} CTDBFF, ctMEM* pCTDBFF;
/*~****************************************************************************/

ctdbEXPORT CTDBRET ctdbDECL ctdbCreateDir(pTEXT str);
ctdbEXPORT CTDBRET ctDECL ctdbCopyFile(pTEXT srcFile, pTEXT dstFile);
ctdbEXPORT CTDBRET ctDECL ctdbDeleteFile(pTEXT srcFile);
ctdbEXPORT CTDBRET ctDECL ctdbRenameFile(pTEXT srcFile, pTEXT dstFile);
ctdbEXPORT pTEXT ctDECL ctdbFindFirstFile(pCTDBFF pFF, pTEXT path);
ctdbEXPORT pTEXT ctDECL ctdbFindNextFile(pCTDBFF pFF);
ctdbEXPORT void ctDECL ctdbFindClose(pCTDBFF pFF);
NINT ctDECL _ctdbPathiCmp(pTEXT cs, pTEXT ct);
VOID ctDECL _ctdbReducePath(pTEXT in, pTEXT out, VRLEN len, COUNT insensitive);

/*~****************************************************************************/


/*^****************************************************************************\
*
*   dictionary merge API
*
\******************************************************************************/

#ifdef CTDB_DICTMERGE_API
struct tagMERGEINFO
{
	DICTDATA info;
	NINT todo;
};
typedef struct tagMERGEINFO    MERGEINFO, ctMEM *pMERGEINFO, ctMEM * ctMEM *ppMERGEINFO;
#endif
/*~****************************************************************************/

/*^****************************************************************************\
*
*   CTDB C runtime non-portable functions
*
\******************************************************************************/
pTEXT ctdbLOCAL _ctdb_strrev(pTEXT str);
/*~****************************************************************************/

/*^****************************************************************************\
*
*   CTDB timespec
*
\******************************************************************************/
typedef struct tagTIMESPEC CTTIMESPEC, ctMEM* pCTTIMESPEC;

struct tagTIMESPEC
{
	time_t	tv_sec;
	long	tv_nsec;
};


/*~****************************************************************************/

/*^****************************************************************************\
*
*   CTDB time functions
*
\******************************************************************************/
CTDBRET ctdbLOCAL _ctdbSysTime(pNINT pYear, pNINT pMonth, pNINT pDay, pNINT pHour, pNINT pMinute, pNINT pSecond);
CTDBRET ctdbLOCAL _ctdbUTCTime(time_t time,pNINT pYear, pNINT pMonth, pNINT pDay, pNINT pHour, pNINT pMinute, pNINT pSecond);
CTDBRET ctdbLOCAL _ctdbUnixTime(pCTTIMESPEC tp);
/*^****************************************************************************\
*
*   CTDB C API Bit manipulation functions
*
\******************************************************************************/
VOID ctdbLOCAL _ctdbFlipBit(pUTEXT pArray, NINT BitNumber, CTBOOL flag);
VOID ctdbLOCAL _ctdbSetBit(pUTEXT pArray, NINT BitNumber);
VOID ctdbLOCAL _ctdbClearBit(pUTEXT pArray, NINT BitNumber);
CTBOOL ctdbLOCAL _ctdbTestBit(pUTEXT pArray, NINT BitNumber);

/*^****************************************************************************\
*
*   c-tree internal function interface
*
\******************************************************************************/
CTDBRET ctdbLOCAL _ctdbCREIFILX(pCTDBTABLE pTable);
CTDBRET ctdbLOCAL _ctdbCREIFILXX(pIFIL ifilptr, pDATOBJ dodaptr, NINT nfields, pTEXT fileword, LONG permmask, pTEXT groupid, pTEXT dext, pTEXT iext);
CTDBRET ctdbLOCAL _ctdbOPNRFILX(CTHANDLE Handle, cpTEXT filnam, NINT filmod, cpTEXT fileword, cpTEXT dext, FILNO *pdatno, COUNT datcount);
CTDBRET ctdbLOCAL _ctdbGETIFIL(FILNO datno, ppIFIL ppIfil, pVRLEN pSize);
CTDBRET ctdbLOCAL _ctdbGETDODA(FILNO datno, ppDATOBJ ppDoda, pVRLEN pSize);
CTDBRET ctdbLOCAL _ctdbGETSCHEMA(FILNO datno, ppConvMap ppSchema, pVRLEN pSize);
CTDBRET ctdbLOCAL _ctdbGETSCHEMANAMES(FILNO datno, ppTEXT Names, pVRLEN pSize);
CTDBRET ctdbLOCAL _ctdbCLRFIL(FILNO datno);
CTDBRET ctdbLOCAL _ctdbDELRFIL(FILNO datno);
CTDBRET ctdbLOCAL _ctdbRENIFIL(pIFIL ifilptr);
CTDBRET ctdbLOCAL _ctdbRENIFILX(pCTDBTABLE pTable, pIFIL ifilptr);
CTDBRET ctdbLOCAL _ctdbLKISAM(COUNT mode);
CTDBRET ctdbLOCAL _ctdbGETFIL(FILNO datno, COUNT mode, pLONG pInfo);
CTDBRET ctdbLOCAL _ctdbOPNICON(FILNO datno, pCOUNT pContext);
CTDBRET ctdbLOCAL _ctdbCLSICON(COUNT context);
CTDBRET ctdbLOCAL _ctdbCHGICON(COUNT context);
CTDBRET ctdbLOCAL _ctdbGETCURP(FILNO datno, pCTOFFSET pOffset);
CTDBRET ctdbLOCAL _ctdbGETVLEN(FILNO datno, pVRLEN pLen);
CTDBRET ctdbLOCAL _ctdbREDVREC(FILNO datno, pVOID recbuf, VRLEN recsize);
CTDBRET ctdbLOCAL _ctdbMOVREC(pCTDBRECORD pRecord,  NINT Direction, pVRLEN pLen);
CTDBRET ctdbLOCAL _ctdbMOVKEY(FILNO datno, FILNO idxno, pCTOFFSET pOffset, pVOID pKeyval, NINT keylen, NINT Direction);
CTDBRET ctdbLOCAL _ctdbfrmkey(FILNO idxno, pUTEXT recbuf, pTEXT key, ctRECPT pntr, VRLEN datlen);
CTDBRET ctdbLOCAL _ctdbFINDREC(pCTDBRECORD pRecord,  pVOID target, NINT FindMode, pVRLEN pLen);
CTDBRET ctdbLOCAL _ctdbFINDKEY(FILNO datno, FILNO idxno, pVOID target, pVOID keyval, NINT keylen, pCTOFFSET pOffset, NINT FindMode);
CTDBRET ctdbLOCAL _ctdbSETCURI(FILNO datno, CTOFFSET offset, pVOID recbuf, VRLEN reclen);
CTDBRET ctdbLOCAL _ctdbREDIREC(FILNO datno, CTOFFSET offset, pVOID recbuf);
CTDBRET ctdbLOCAL _ctdbREDIVREC(FILNO datno, CTOFFSET offset, pVOID recbuf, pVRLEN pLen);
CTDBRET ctdbLOCAL _ctdbADDREC(FILNO datno, pVOID recbuf, pNINT errfil);
CTDBRET ctdbLOCAL _ctdbADDVREC(FILNO datno, pVOID recbuf, VRLEN reclen, pNINT errfil);
CTDBRET ctdbLOCAL _ctdbRWTREC(FILNO datno, pVOID recbuf, pNINT errfil);
CTDBRET ctdbLOCAL _ctdbRWTVREC(FILNO datno, pVOID recbuf, VRLEN reclen, pNINT errfil);
CTDBRET ctdbLOCAL _ctdbRWTPREC(FILNO datno, pVOID recbuf, VRLEN reclen, pNINT errfil);
CTDBRET ctdbLOCAL _ctdbDELREC(FILNO datno);
CTDBRET ctdbLOCAL _ctdbDELVREC(FILNO datno);
CTDBRET ctdbLOCAL _ctdbAVLFILNBR(NINT numfils, FILNO *pDatno);
CTDBRET ctdbLOCAL _ctdbCREDAT(CTHANDLE Handle, FILNO datno, pTEXT filnam, UCOUNT datlen, UCOUNT xtdsiz, COUNT filmod, LONG permmask, pTEXT groupid, pTEXT fileword);
CTDBRET ctdbLOCAL _ctdbCLSFIL(FILNO datno);
CTDBRET ctdbLOCAL _ctdbOPNFIL(CTHANDLE Handle, FILNO datno, pTEXT filnam, COUNT filmod);
CTDBRET ctdbLOCAL _ctdbOPNFILX(pCTDBTABLE pTable, pTEXT filnam, COUNT filmod, FILNO *filno, COUNT datcount);
CTDBRET ctdbLOCAL _ctdbDELFIL(FILNO datno);
CTDBRET ctdbLOCAL _ctdbGetNumberOfRecords(FILNO filnum, CTBOOL IsVarLen, pCTUINT64 pCount);
CTDBRET ctdbLOCAL _ctdbGetNumberOfIndexEntries(FILNO idxno, pCTUINT64 pCount);
CTDBRET ctdbLOCAL _ctdbIDXENT(FILNO idxno, pLONG pCount);
CTDBRET ctdbLOCAL _ctdbctDISTINCTest(FILNO idxno, pLONG pCount);
CTDBRET ctdbLOCAL _ctdbctDISTINCTset(FILNO idxno, NINT action, pLONG pCount);
CTDBRET ctdbLOCAL _ctdbMOVSET(FILNO datno, FILNO idxno, pVOID target, pVOID recbuf, COUNT siglen, pCTOFFSET pOffset, NINT Direction, pVRLEN pLen);
CTDBRET ctdbLOCAL _ctdbMOVRNG(pCTDBRECORD pRecord, NINT Direction, pVRLEN pLen);
CTDBRET ctdbLOCAL _ctdbMOVFTS(FILNO datno, pVOID recbuf, pCTOFFSET pOffset, NINT Direction, pVRLEN pLen);
CTDBRET ctdbLOCAL _ctdbFRESET(void);
CTDBRET ctdbLOCAL _ctdbFRESETN(COUNT setnum);
CTDBRET ctdbLOCAL _ctdbREGCTREE(cpTEXT regid);
CTDBRET ctdbLOCAL _ctdbUNRCTREE(cpTEXT regid);
CTDBRET ctdbLOCAL _ctdbSWTCTREE(cpTEXT regid);
pTEXT ctdbLOCAL _ctdbWCHCTREE();
CTDBRET ctdbLOCAL _ctdbRBLIFILX(CTHANDLE Handle, pIFIL ifilptr, pTEXT datext, pTEXT idxext, LONG permmask, pTEXT groupid, pTEXT fileword);
CTDBRET ctdbLOCAL _ctdbRBLIFILX8(CTHANDLE Handle, pIFIL ifilptr, pTEXT datext, pTEXT idxext, LONG permmask, pTEXT groupid, pTEXT fileword, pXCREblk xcreblk);
CTDBRET ctdbLOCAL _ctdbCMPIFILX(CTHANDLE Handle, pIFIL ifilptr, pTEXT datext, pTEXT idxext, LONG permmask, pTEXT groupid, pTEXT fileword);
CTDBRET ctdbLOCAL _ctdbCMPIFILX8(CTHANDLE Handle, pIFIL ifilptr, pTEXT datext, pTEXT idxext, LONG permmask, pTEXT groupid, pTEXT fileword, pXCREblk xcreblk);
COUNT ctdbLOCAL _ctdbSETCBRBL(pRBLCBFNC funcptr, UCOUNT step);
CTDBRET ctdbLOCAL _ctdbTFRMKEY(FILNO keyno, pVOID target);
CTDBRET ctdbLOCAL _ctdbADDRES(FILNO datno, pVOID resptr, VRLEN varlen);
CTDBRET ctdbLOCAL _ctdbGETRES(FILNO datno, pVOID resptr, pVOID buffer, VRLEN bufsize, COUNT resmode);
CTDBRET ctdbLOCAL _ctdbGETRES2(pCTDBTABLE pTable, pULONG resin, ppVOID buffer);
CTDBRET ctdbLOCAL _ctdbUPDRES(FILNO datno, pVOID buffer, VRLEN bufsize);
CTDBRET ctdbLOCAL _ctdbREADRES(FILNO datno, pVOID resptr, pVOID resbuf, VRLEN buflen, COUNT mode, pCTOFFSET pOffset);
CTDBRET ctdbLOCAL _ctdbDELRES(FILNO datno, pVOID buffer);
CTDBRET ctdbLOCAL _ctdbLOKREC(FILNO datno, COUNT mode, CTOFFSET recbyt);
CTDBRET ctdbLOCAL _ctdbPRMIIDX(pCTDBTABLE pTable, pIFIL ifilptr);
CTDBRET ctdbLOCAL _ctdbPRMIIDX2(pCTDBTABLE pTable, pIFIL ifilptr, ctINDEX_ATTRIBUTES* pIndexAttributes);
CTDBRET ctdbLOCAL _ctdbTMPIIDX(pCTDBTABLE pTable, pIFIL ifilptr);
CTDBRET ctdbLOCAL _ctdbDROPIDX(FILNO idxno);
CTDBRET ctdbLOCAL _ctdbRELOADIFIL(pCTDBTABLE pTable);
CTDBRET ctdbLOCAL _ctdbTRANISO(COUNT isolevel);
CTDBRET ctdbLOCAL _ctdbAGGRLOCK(FILNO datno);
CTBOOL ctdbLOCAL _ctdbISHUGEFILE(FILNO datno);
LONG ctdbLOCAL _ctdbPartKeyNo(FILNO datno);
LONG ctdbLOCAL _ctdbPartCallParm(pCTDBTABLE pTable);
VOID ctdbLOCAL _ctdbGetFLMOD3(FILNO datno, CTBOOL *compressed, CTBOOL *fixed, CTBOOL *noflush, CTBOOL *flexrec, CTBOOL *insertonly);
CTDBRET ctdbLOCAL _ctdbRELOADSCHEMA(pCTDBTABLE pTable);
CTLOCK_MODE ctdbLOCAL _ctdbGETLOK(FILNO datno, CTOFFSET recbyt);

#ifdef CTDB_FILTER_020617
CTDBRET ctdbLOCAL _ctdbSETFLTR(FILNO datno, pTEXT condexpr);
CTDBRET ctdbLOCAL _ctdbSETFLTRN(FILNO datno, UCOUNT fltnum, UCOUNT fltopts, pTEXT condexpr);
CTDBRET ctdbLOCAL _ctdbSETFLTRX(FILNO datno, pVOID expr);
#endif /* CTDB_FILTER_020617 */
#ifdef CTDB_AUTO_SQLIMPORT
CTBOOL  ctdbLOCAL _ctdbSQLcallbackXtd(pVOID ctx, CTSQLCB_MODE mode, pTEXT msg, CTBOOL def, pVOID extra);
CTDBRET ctdbLOCAL _ctdbSQLUnlinkTable(CTHANDLE Handle, cpTEXT tbname, CTBOOL relink);
CTDBRET ctdbLOCAL _ctdbSQLLinkTable(CTHANDLE Handle, cpTEXT tbname, CTBOOL relink);
VOID ctdbLOCAL _ctdbSQLWarning(CTHANDLE hnd, pTEXT fmt, ...) PRINTFLIKE(2,3);
VOID ctdbLOCAL _ctdbSQLError(CTHANDLE hnd, pTEXT fmt, ...) PRINTFLIKE(2,3);
CTBOOL ctdbLOCAL _ctdbSQLConfirm(CTHANDLE hnd, CTSQLCB_MODE mode, pTEXT msg, CTBOOL def);
CTDBRET ctdbLOCAL _ctdbSQLBuildSchema(pTEXT TXTname, CTHANDLE Handle, pTEXT owner, pTEXT tbname, pTEXT tbpath);
#endif

CTDBRET ctdbLOCAL _ctdbFRCKEY(FILNO keyno, pVOID keyptr, COUNT percent, pCTOFFSET recbyt);
LONG ctdbLOCAL _ctdbESTKEY(FILNO keyno, pVOID key1, pVOID key2);
CTDBRET ctdbLOCAL _ctdbPUTIFILX(pCTDBTABLE pTable, pIFIL ifilptr);
CTDBRET ctdbLOCAL _ctdbUpdateSerial(pCTDBTABLE pTable, CTROWID rowid);
CTBOOL ctdbLOCAL _ctdbIsSCHSEG(NINT segmode);
CTBOOL ctdbLOCAL _ctdbNeedsGNXSEGSegment(pCTDBFIELD pField);

CTDBRET ctdbLOCAL _ctdbPUTKSEGDEF(NINT filno, NINT segno, pctKSEGDEF pkdef);
CTDBRET ctdbLOCAL _ctdbGETKSEGDEF(NINT filno, NINT segno, pctKSEGDEF pkdef);

CTDBRET ctdbLOCAL _ctdbBATSETX(FILNO filno,pVOID request,pVOID bufptr,VRLEN bufsiz,ULONG mode);
CTDBRET ctdbLOCAL _ctdbFREBATN(pCTDBRECORD pRecord);
CTDBRET ctdbLOCAL _ctdbCHGBAT(pCTDBRECORD pRecord);

CTDBRET ctdbLOCAL _ctdbGETXCREBLK(FILNO datno, pXCREblk xcreblk);
CTDBRET ctdbLOCAL _ctdbSETOPS(LONG mode, VRLEN status, pLONG state);
CTDBRET ctdbLOCAL _ctdbCLIFIL(pIFIL ifil);
CTDBRET ctdbLOCAL _ctdbOPNIFILX(pIFIL ifil, pTEXT fileword, pTEXT dext, pTEXT iext, FILNO *pdatno);
CTDBRET ctdbLOCAL _ctdbRemoveFTI(pCTDBTABLE pTable, COUNT Number);
CTDBRET ctdbLOCAL _ctdbCreateFTI(pCTDBTABLE pTable, COUNT Number, CTBOOL recreate);
CTDBRET ctdbLOCAL _ctdbGetFTIResources(pCTDBTABLE pTable);
TEXT ctdbLOCAL _ctdbIsVarlenField(pCTDBFIELD pField);
TEXT ctdbLOCAL _ctdbIsVarlenType(CTDBTYPE ftype);
CTDBRET ctdbLOCAL _ctdbRenameTable(pCTDBTABLE pTable, pTEXT filename, ppTEXT orgList, ppTEXT tmpList, NINT listcount);
pCTDBDATA ctdbLOCAL _ctdbGetData(pCTDBRECORD pRecord, VRLEN FieldNbr);
CTBOOL ctdbLOCAL _ctdbIsNameTableDep(cpTEXT tablename, cpTEXT name, TEXT pathsep, cpTEXT *from, cpTEXT *to);
CTBOOL ctdbLOCAL _ctdbGetFieldAsWithCallback(CTHANDLE Handle, NINT FieldNbr);
pCTDBVFIELDINFO ctdbLOCAL _ctdbGetFieldDataVirtInfo(CTHANDLE Handle, NINT FieldNbr);
CTDBTYPE ctdbLOCAL _ctdbGetFieldDataType(CTHANDLE Handle, NINT FieldNbr);
NINT ctdbLOCAL _ctdbGetDodaFieldInfo(CTHANDLE hRecord, NINT field_no, pNINT ftype, pLONG fdbtype);
pTEXT ctdbLOCAL _ctdbGenerateFilterNames(CTHANDLE hRecord);
CTDBRET _ForeignGetAs_i(pCTDBDATA pFieldData, pVOID destination, pVRLEN pdestsize, CTDB_FIELD_METATYPE rtype, TEXT from_isnull);
CTBOOL _IsNull_i(pCTDBDATA pField);
CTDB_FIELD_METATYPE getMetaType(CTDBTYPE type, pVRLEN msize, pCTDBFIELDATTR attr, TEXT is_set);
CTDBRET _ForeignSetAs_i(pCTDBDATA pFieldData, pVOID value, VRLEN size, CTDB_FIELD_METATYPE rtype);
CTDBRET _Foreign2MetaType(pDATOBJ dodaline, pUTEXT src, pUTEXT dst, pVRLEN pdstsize, UCOUNT Fdigits, UCOUNT Fscale, CTDB_FIELD_METATYPE rtype, CTATTR_RTGNUMCONV num_conv, TEXT on_sign, pTEXT conv_error_msg, size_t conv_error_msg_len);
CTHANDLE ctdbLOCAL _ictdbAddField(CTHANDLE Handle, cpTEXT FieldName, CTDBTYPE FieldType, VRLEN FieldLength, pCTDBLIST hList, CTBOOL internal);
CTHANDLE ctdbLOCAL _ictdbGetRecordHandle(CTHANDLE Handle, CTBOOL ctreecalls);
CTDBRET ctdbLOCAL _ctdbRecordExtendAndReRead(CTHANDLE handle, VRLEN reclen);
CTDBRET ctdbLOCAL _ctdbSetNumberField(pCTDBDATA pData, pCTNUMBER pValue);
CTDBRET ctdbLOCAL _ctdbSetFloatField(pCTDBDATA pData, CTFLOAT value);
CTDBRET ctdbLOCAL _ctdbSetSignedField(pCTDBDATA pData, CTSIGNED value);
CTDBRET ctdbLOCAL _ctdbSetUnsignedField(pCTDBDATA pData, CTUNSIGNED value);
CTDBRET ctdbLOCAL _ctdbSetBigIntField(pCTDBDATA pData, CTBIGINT value);
CTDBRET ctdbLOCAL _ctdbSetUnsignedBigIntField(pCTDBDATA pData, CTUBIGINT value);
CTDBRET ctdbLOCAL _ctdbSetBoolField(pCTDBDATA pData, CTBOOL value);
VRLEN _ctdbAdjustFieldLength(UCOUNT ftype, UCOUNT definedLen);

#ifdef	CTDBCEPTLAYER	/* If intercept layer defined */
#include "ctdbapi1.h"	/* Create an intercept layer with ctadapi1.c */
#endif


/*^****************************************************************************\
*
*   CTDB internal MISC function
*
\******************************************************************************/
#ifdef CTDB_BATCH_65536
COUNT _ctdbAvalilableBatchNo(pCTDBSESSION pSession);
void _ctdbReleaseBatchNo(pCTDBSESSION pSession, COUNT batchnum);
#endif
LONG _ctdbXCRExtdsiz(NINT extension);

/*^****************************************************************************\
*
*   CTDB table router handle
*
\******************************************************************************/
struct tagCTDBROUTER
{
	CTBOOL		r_active;		/* indicate router is active				*/
	CTDBRET		r_error;		/* router errors. use ctdbGetRouterError()	*/
	CTHANDLE	r_table;		/* router table handle						*/
	CTHANDLE	r_record;		/* router table record handle				*/
	UCOUNT		r_nbrfields;	/* number of fields in field mapping		*/
	pUCOUNT		r_fieldmap;		/* field mapping. The index of the vector	*/
								/* corresponds to the child table field 	*/
								/* number and the contents of the vector 	*/
								/* indicate	the field number of the parent	*/
								/* table									*/
};
/*^****************************************************************************\
*
*   CTDB table router resource
*
\******************************************************************************/
typedef struct tagCTDBROUTERRES	CTDBROUBERRES, ctMEM* pCTDBROUTERRES;

struct tagCTDBROUTERRES
{
	pTEXT	name;
	pTEXT	path;
	VRLEN	nbrfields;
	pUTEXT	fieldmap;
};
/*~****************************************************************************/


/*^****************************************************************************\
*
*   CTDB sqlimport context structure (used by SQLLinkCallbackXtd implementation)
*
\******************************************************************************/
#ifdef CTDB_NEW_SQLIMP

/* PADTYPE bitmask:
bit 0 = force pad enabled..: 0=off/1=on
bit 1 = padding character..: 0=zero/1=space
bit 2 = delimiter character: 0=zero/1=space
*/
typedef enum {
	DEFAULT_PADTYPE = 0x0,
	ZEROES_PADTYPE = 0x1,
	SPACE_PADDING = 0x2,
	SPACESWITHZERO_PADTYPE = 0x3,
	SPACE_DELIMITER = 0x4,
	SPACES_PADTYPE = 0x7
} impPADTYPE;

typedef struct context {
	CTBOOL skipfld;
	pTEXT symname;
	pTEXT prefix;
	pTEXT pidxname;
	ULONG maxfldlen;
	CTBOOL skipidx;
	CTBOOL promote;
	CTBOOL promote_check;
	CTBOOL partseg;
	ULONG tbluid;
	CTBOOL grantpub;
	CTBOOL grantpub_ro;
	CTBOOL relink;
	CTBOOL nonconvids;
	impPADTYPE frcpad;
	PRINTFNC print_message;
	GETINPUT fget_input;
	pVOID usertag;
	CTBOOL keepextra;
	pTEXT rowidfld;
	pTEXT rowididx;
} impctx;
#endif
/*~****************************************************************************/

#define ctVBSZlimit 10  /* retry limit on VBSZ_ERR error */


/*^****************************************************************************\
*
* variant helper funtions
*
\******************************************************************************/
CTDBTYPE _ctdbVariantBasedatatype(ct_variant_datatype arg);

/*~****************************************************************************/

#ifdef __cplusplus
}
#endif

#endif /* ~_CTDBPORT_H_ */

/* end of ctdbport.h */

