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
#ifndef _CTSQLAPI_H_
#define _CTSQLAPI_H_

#if defined UNICODE && !defined _WIN32
#include <wchar.h>
#endif

#ifndef FAIRCOM_CODE
#define FAIRCOM_CODE
#endif
#include "ctsql_lib.h"

#define SQLAPI_BATCHES
#define SQLAPI_NO_LTYPE_GETFIELD
#ifdef CTSQLAPI_SAPP
#ifndef CTSQLAPI_SRVR
#define CTSQLAPI_SRVR
#endif
#endif

#ifdef CTSQLAPI_SRVR
#define ctsqlfn(body)	SRVctsql ## body
#else
#define ctsqlfn(body)	ctsql ## body
#endif



/****************************************************************************/
#ifdef _WIN32_WCE
#ifdef CTSQLAPI_EXPORTS
#define CTSQLAPI_API __declspec(dllexport)
#else
#define CTSQLAPI_API __declspec(dllimport)
#endif
// This class is exported from the ctsqlapi.dll
class CTSQLAPI_API CCtsqlapi {
public:
	CCtsqlapi(void);
	// TODO: add your methods here.
};

extern CTSQLAPI_API int nCtsqlapi;
CTSQLAPI_API int fnCtsqlapi(void);
#endif /*  _WIN32_WCE */
/****************************************************************************/


/****************************************************************************\
*
*	defines
*
\****************************************************************************/
#define ctsqlDECL
#define CTSQL_MAX_ERRLEN	512
#ifdef ctSQL_MAX_FLDLEN
#define CTSQL_MAX_FLDLEN	ctSQL_MAX_FLDLEN
#else
#error ctSQL_MAX_FLDLEN not defined
#endif
#define CTSQL_FALSE			0
#define CTSQL_TRUE			1
/****************************************************************************\
*
*	Base types
*
\****************************************************************************/
typedef unsigned char	BIT;
typedef char			TINYINT;
typedef unsigned char	UTINYINT;
typedef short			SMALLINT;
typedef unsigned short	USMALLINT;
typedef int				INTEGER;
typedef unsigned int	UINTEGER;
typedef float			SMALLFLOAT;
typedef double			FLOATT;
#if defined(UNICODE) && !defined(ctPortWINIOT)
typedef wchar_t			CTSQLCHAR;
typedef const wchar_t		CTSQLCONSTCHAR;
#else
typedef char			CTSQLCHAR;
typedef const char		CTSQLCONSTCHAR;
#endif

#define FC_IS_LONG_TYPE(x)  (((x) == TPE_DT_LVC || (x) == TPE_DT_NLVC || (x) == TPE_DT_LVB || (x) == TPE_DT_BLOB || (x) == TPE_DT_CLOB || (x) == TPE_DT_NCLOB)?1:0)
/****************************************************************************\
*
*	portability macros
*
\****************************************************************************/
#define ctsql_alloc(size,count)		calloc((count),(size))
#define ctsql_free(ptr)				free(ptr)
#define ctsql_stralloc(size)		(CTSQLCHAR*)ctsql_alloc(sizeof(CTSQLCHAR),(size))
#define ctsql_memcpy				memcpy
#define ctsql_memset				memset
#if defined(UNICODE) && !defined(ctPortWINIOT)
#define ctsql_atoi(a)				wcstol(a, NULL, 10)
#define ctsql_atof(a)				wcstod(a, NULL)
#define ctsql_strlen(st)			((st) ? wcslen(st) : 0)
#ifdef ctSQL_SAFE_CRT // FAIRCOM_MOD

// Copy source string to destination string with size limit, ensuring the resulting string is null-terminated but may be truncated.
inline wchar_t *ctsql_strncpy( wchar_t *destptr, const wchar_t * srcptr, size_t no_of_char )
{
	if (no_of_char > 0) {
#ifdef ctPortWIN32
		wcsncpy_s(destptr, no_of_char, srcptr, _TRUNCATE);
#else
		swprintf(destptr, no_of_char, L"%ls", srcptr);
#endif
	}
	return(destptr);
}

// Append source string to destination string with size limit, ensuring the resulting string is null-terminated but may be truncated.
inline wchar_t *ctsql_strncat_s( wchar_t *dest, size_t destsize, const wchar_t * src )
{
	size_t curdestlen = wcslen(dest);
	if (destsize > curdestlen) {
#ifdef ctPortWIN32
		return(os_wcsncpy_s(dest + curdestlen, src, destsize - curdestlen));
#else
		swprintf(dest + curdestlen, destsize - curdestlen, L"%ls", src);
#endif
	}
	return(dest);
}
#else
#define ctsql_strcpy(dst,src)		wcscpy(dst,src)
#define ctsql_strcat(dst,src)		wcscat(dst,src)
#endif
#ifdef _WIN32
#define ctsql_strdup(st)			_wcsdup(st)
#else
#define ctsql_strdup(st)			wcsdup(st)
#endif
#define ctsql_strchr(st,ch)			wcschr(st,ch)
#define ctsql_strcmp(s1,s2)			wcscmp(s1,s2)
#define ctsql_strncmp(s1,s2,l)		wcsncmp(s1,s2,l)
#define ctsql_strrchr(st,ch)			wcsrchr(st,ch)
#else
#define ctsql_atoi(a)				atoi(a)
#define ctsql_atof(a)				atof(a)
#define ctsql_strlen(st)			((st) ? strlen(st) : 0)
#ifdef ctSQL_SAFE_CRT /* FAIRCOM_MOD */
#define ctsql_strncpy			ctrt_strncpy
#define ctsql_strncat_s			ctrt_strncat_s
#else
#define ctsql_strncpy			strncpy
#define ctsql_strncat_s(dst,siz,src)	strcat(dst,src)
#endif
#ifdef DH_OS_UNIX
#define ctsql_strdup(st)			strdup(st)
#else
#define ctsql_strdup(st)			_strdup(st)
#endif
#define ctsql_strchr(st,ch)			strchr(st,ch)
#define ctsql_strcmp(s1,s2)			strcmp(s1,s2)
#define ctsql_strncmp(s1,s2,l)			strncmp(s1,s2,l)
#define ctsql_strrchr(st,ch)			strrchr(st,ch)
#endif
#define ctsql_ansistrncpy		os_ansistrncpy
#define ctsql_strset(st,val)		{if (st) ctsql_free(st); st = ctsql_strdup(val);}

#ifdef UNICODE
#define CT_STR_LIT(s) L ## s
#undef CT_STRING_LITERAL
#define CT_STRING_LITERAL(s) CT_STR_LIT(s)
#define CT_PRINTF_STR "ls"
#else
#define CT_STRING_LITERAL(s) s
#define CT_PRINTF_STR "s"
#endif

#ifdef DH_OS_UNIX
/* sprintf() with size limit, ensuring the resulting string is null-terminated but may be truncated */
#ifdef UNICODE
#define ctsql_snprintf	swprintf
#else
#define ctsql_snprintf	snprintf
#endif
#else
#include <tchar.h>
/* sprintf() with size limit, ensuring the resulting string is null-terminated but may be truncated */
#define ctsql_snprintf(str,size,format,...)	_sntprintf_s(str,size,_TRUNCATE,format,__VA_ARGS__)
#endif


/****************************************************************************\
*
*	Field type
*
\****************************************************************************/
typedef SMALLINT CTSQLTYPE;

#define CTSQL_INVTYPE         -1
#define CTSQL_CHAR             1
#define CTSQL_NUMERIC          2
#define CTSQL_SMALLINT         3
#define CTSQL_INTEGER          4
#define CTSQL_SMALLFLOAT       5
#define CTSQL_REAL             5
#define CTSQL_FLOAT            6
#define CTSQL_DATE             7
#define CTSQL_MONEY            8
#define CTSQL_TIME             9
#define CTSQL_TIMESTAMP       10
#define CTSQL_TINYINT         11
#define CTSQL_BINARY          12
#define CTSQL_BIT             13
#define CTSQL_LVC             14
#define CTSQL_LVB             15
#define CTSQL_BIGINT          16
#define CTSQL_NCHAR           17
#define CTSQL_NVARCHAR        18
#define CTSQL_NCHAR_CS        19
#define CTSQL_NVARCHAR_CS     20
#define CTSQL_VARCHAR         21
#define CTSQL_BLOB            22
#define CTSQL_CLOB            23
#define CTSQL_NLVC            24
#define CTSQL_NCLOB           25
#define CTSQL_VARBINARY       26
/****************************************************************************\
*
*	Transaction isolation level
*
\****************************************************************************/
typedef enum
{
    CTSQL_READ_UNCOMMITTED = 0,
    CTSQL_READ_COMMITTED = 1,
    CTSQL_REPEATABLE_READ = 2,
    CTSQL_SERIALIZABLE = 3,
    CTSQL_INVALID_ISOLATION_LEVEL = 4
} CTSQL_ISOLEVEL ;
/****************************************************************************\
*
*	Nullability flags
*
\****************************************************************************/
typedef enum
{
	CTSQL_NO_NULLS,
	CTSQL_NULLABLE,
	CTSQL_NULLABLE_UNKNOWN
} CTSQL_NULLABILITY;
/****************************************************************************\
*
*	SQL numeric type
*
\****************************************************************************/
#define SQL_NUMERIC_SIZE 16

typedef struct
{
	SMALLINT dec_num;
	TINYINT	 dec_digits[SQL_NUMERIC_SIZE + 1];
} NUMERIC;

#ifndef TEST_SP
/****************************************************************************\
*
*	SQL DATE
*
\****************************************************************************/
typedef struct
{
	SMALLINT year ;
	UTINYINT month;
	UTINYINT day ;
} SQLDATE;
/****************************************************************************\
*
*	SQL TIME
*
\****************************************************************************/
typedef struct
{
	UTINYINT  hours;
	UTINYINT  mins;
	UTINYINT  secs;
	USMALLINT msecs;
} SQLTIME;
/****************************************************************************\
*
*	SQL TIMESTAMP
*
\****************************************************************************/
typedef struct
{
	SQLDATE ts_date;
    SQLTIME ts_time;
}  SQLTIMESTAMP;
#endif /* ~TEST_SP */
/****************************************************************************\
*
*	SQL BINARY
*
\****************************************************************************/
typedef struct
{
	INTEGER	 tb_len;
	UTINYINT tb_data [CTSQL_MAX_FLDLEN] ;
} BINARY;
/****************************************************************************\
*
*	SQL BIGINT
*
\****************************************************************************/
typedef struct
{
	UINTEGER ll ;
	INTEGER  hl ;
} BIGINT;

typedef enum {
	INFO_STP,
	INFO_UDF,
	INFO_TRG
 } STP_TYPE;


typedef enum { /* see sql_paramtype_t in sql_tree.hxx*/
	SQL_INV_PARAM = -1,
	SQL_IN_PARAM = 1,
	SQL_IN_OUT_PARAM = 2,
	SQL_OUT_PARAM = 4,
	SQL_SFN_SUBS_PARAM = 5
} SQL_PARAMDIR;


#undef  CT_STRING_LITERAL
#if defined(UNICODE) && !defined(ctPortWINIOT)
#define CT_STR_LIT(s) L ## s
#define CT_STRING_LITERAL(s) CT_STR_LIT(s)
#else
#define CT_STRING_LITERAL(s) s
#endif


/****************************************************************************\
*
*	Return types
*
\****************************************************************************/
typedef INTEGER			CTSQLRET, *pCTSQLRET;

#define CTSQLRET_OK					0

#define	CTSQL_NOTFOUND				100
#define CTSQL_NULLRESULT    		101

#define	CTSQLRET_INTERNAL			-20000
#define	CTSQLRET_NOMEM				-20001
#define	CTSQLRET_INITFAIL			-20002
#define	CTSQLRET_INSYNTAX			-20003
#define	CTSQLRET_NOUSER				-20004
#define	CTSQLRET_NOTBL				-20005
#define	CTSQLRET_NOCOL				-20006
#define	CTSQLRET_NOCOLS				-20007
#define	CTSQLRET_INTYPES			-20008
#define	CTSQLRET_AMBCOL				-20009
#define	CTSQLRET_DUPCOL				-20010
#define	CTSQLRET_BADLEN				-20011
#define	CTSQLRET_BADPREC			-20012
#define	CTSQLRET_BADSC				-20013
#define	CTSQLRET_NOINS				-20014
#define	CTSQLRET_SUBQRETS			-20015
#define	CTSQLRET_BADNULL			-20016
#define	CTSQLRET_MOREVALS			-20017
#define	CTSQLRET_LESSVALS			-20018
#define	CTSQLRET_BADTBLREF			-20019
#define	CTSQLRET_BADGBCOL			-20020
#define	CTSQLRET_NOTGEIN_HVG		-20021
#define	CTSQLRET_NOTGEIN_SLCT 		-20022
#define	CTSQLRET_BADSETFN			-20023
#define	CTSQLRET_NOTYET				-20024
#define	CTSQLRET_AFNESTED			-20025
#define	CTSQLRET_TBLREFCNT			-20026
#define	CTSQLRET_BADSORTFNUM		-20027
#define	CTSQLRET_DUPIDX				-20028
#define	CTSQLRET_NOIDX				-20029
#define	CTSQLRET_SEGEXISTS			-20030
#define	CTSQLRET_DUPCLUST			-20031
#define	CTSQLRET_NOCLUST			-20032
#define	CTSQLRET_NOTBLSP			-20033
#define	CTSQLRET_BADFPCT			-20034
#define	CTSQLRET_NONULL				-20035
#define	CTSQLRET_NOTPREPARED		-20036
#define	CTSQLRET_EXECSELECT			-20037
#define	CTSQLRET_NOTCLOSED			-20038
#define	CTSQLRET_NOTSELECT			-20039
#define	CTSQLRET_NOTOPENED			-20040
#define	CTSQLRET_TBLEXISTS			-20041
#define	CTSQLRET_MULTIDIST			-20042
#define	CTSQLRET_TPLSZHI			-20043
#define	CTSQLRET_ARRAYSZHI			-20044
#define	CTSQLRET_NOFILE				-20045
#define	CTSQLRET_FVNONULL			-20046
#define	CTSQLRET_GRSELF				-20047
#define	CTSQLRET_RVKSELF			-20048
#define	CTSQLRET_KEYWDUSED			-20049
#define	CTSQLRET_FLDCNTHI			-20050
#define	CTSQLRET_IDXCNTHI			-20051
#define	CTSQLRET_OVERFLOW			-20052
#define	CTSQLRET_DBNOTOPEN			-20053
#define	CTSQLRET_NODB				-20054
#define	CTSQLRET_DBNOTSTARTED		-20055
#define	CTSQLRET_NOTDBA				-20056
#define	CTSQLRET_NORES				-20057
#define	CTSQLRET_TRANS_ABORTED		-20058
#define	CTSQLRET_NOTSFILES			-20059
#define	CTSQLRET_TBLNOTEMPTY		-20060
#define	CTSQLRET_INPUTSZHI			-20061
#define	CTSQLRET_NOTPATHNM			-20062
#define	CTSQLRET_DUPFILE			-20063
#define	CTSQLRET_BADATCHT			-20064
#define	CTSQLRET_BADSTMTT			-20065
#define	CTSQLRET_BADSQLDA			-20066
#define	CTSQLRET_1LOCALONLY			-20067
#define	CTSQLRET_BADARG				-20068
#define	CTSQLRET_SMLSQLDA			-20069
#define	CTSQLRET_BUFLENHI			-20070
#define	CTSQLRET_INVALID_OPN		-20071
#define	CTSQLRET_SVRNOTALLOCD		-20072
#define	CTSQLRET_STMT_TOOLONG		-20073
#define	CTSQLRET_NOVIEWCOLLIST		-20074
#define	CTSQLRET_LESSCOLNS			-20075
#define	CTSQLRET_MORECOLNS			-20076
#define	CTSQLRET_CHK_OPTION			-20077
#define	CTSQLRET_ILLEGAL_OPN		-20078
#define	CTSQLRET_MAXTBL				-20079
#define	CTSQLRET_CHKOPT				-20080
#define	CTSQLRET_INVAL_CNT			-20081
#define	CTSQLRET_BADSORTFLD			-20082
#define	CTSQLRET_NOTCOLREF			-20083
#define	CTSQLRET_OJONSUBQUERY		-20084
#define	CTSQLRET_ERRINOUTER			-20085
#define	CTSQLRET_DUPCNST			-20086
#define	CTSQLRET_COLCNTMM			-20087
#define	CTSQLRET_INVUSER 			-20088
#define CTSQLRET_SDATESETTING    	-20089
#define	CTSQLRET_NOTBLCOLLIST		-20090
#define	CTSQLRET_QSTMTTOOLONG		-20091
#define	CTSQLRET_NOTPL				-20092
#define	CTSQLRET_SYNEXISTS			-20093
#define	CTSQLRET_LINKEXISTS			-20094
#define	CTSQLRET_NOLINK				-20095
#define	CTSQLRET_MANDUSER			-20096
#define	CTSQLRET_OPNNOTALLWD		-20097
#define	CTSQLRET_MUL_ROWS			-20098
#define	CTSQLRET_FTCH_RID			-20099
#define	CTSQLRET_BADSUBQRY			-20100
#define	CTSQLRET_NOREFS				-20101
#define	CTSQLRET_PKEYNULL			-20102
#define	CTSQLRET_NOKEY				-20103
#define	CTSQLRET_INCPKEYS			-20104
#define	CTSQLRET_NOPERM				-20105
#define	CTSQLRET_BADTID				-20106
#define	CTSQLRET_REMDBNOTUP			-20107
#define	CTSQLRET_REMSVRNOTUP		-20108
#define	CTSQLRET_NOREMDB			-20109
#define	CTSQLRET_NOREMHOST			-20110
#define	CTSQLRET_REFSPRSNT			-20115
#define	CTSQLRET_CNSTVIOLN			-20116
#define	CTSQLRET_TBLINCMPL			-20117
#define	CTSQLRET_DUPCONSTRAINT		-20118
#define	CTSQLRET_NOCONSTRAINT		-20119
#define	CTSQLRET_RESERVEWORD		-20120
#define	CTSQLRET_NOPERMISSION		-20121
#define	CTSQLRET_NOPROC				-20122
#define	CTSQLRET_INVALPROC			-20123
#define CTSQL_QUERY_TERMINATE     	-20124
#define	CTSQLRET_EXCEEDSMAX_OPNCURS	-20125
#define	CTSQLRET_INV_CURSNAME		-20126
#define	CTSQLRET_BADPARAM			-20127
#define CTSQLRET_NUMRANGE        	-20128
#define CTSQLRET_DATATRUNC       	-20129
#define CTSQLRET_TRNSRLBK        	-20130
#define CTSQLRET_MISSINGPARAM    	-20131
#define	CTSQLRET_RVKRESTRICT		-20132
#define CTSQLRET_FENOTSUP        	-20133
#define CTSQLRET_INV_LONGREF     	-20134
#define	CTSQLRET_GETDIAG			-20135
#define CTSQLRET_BADCNTNS        	-20136
#define CTSQLRET_BADCNTNSTP      	-20137
#define CTSQLRET_NOCNTNSIDX      	-20138
#define CTSQLRET_BADLNGIDX       	-20139
#define CTSQLRET_PROCEXISTS      	-20140
#define CTSQLRET_JAVA_COMPILE       -20141
#define CTSQLRET_JAVA_EXEC          -20142
#define CTSQLRET_TOOMANYRECS        -20143
#define CTSQLRET_NULLVAL            -20144
#define CTSQLRET_INVALFLD           -20145
#define CTSQLRET_TRGEXISTS     		-20146
#define CTSQLRET_NOTRIGGER     		-20147
#define CTSQLRET_TRIGEXEC     		-20148
#define CTSQLRET_NOTFOUND     		-20149
#define CTSQLRET_NOVIEWRSSID     	-20150
#define CTSQLRET_USEDRTBL        	-20151
#define CTSQLRET_ALLOC_FAILED    	-20152
#define CTSQLRET_TREENOT_INCACHE 	-20153
#define CTSQLRET_GCACHE_INSERT   	-20154
#define CTSQLRET_ENVFLGSET       	-20155
#define CTSQLRET_NOSTMT           	-20156
#define CTSQLRET_NOTRIG_ALWD     	-20158
#define CTSQLRET_NOREVOKE        	-20159
#define CTSQLRET_JSPTFENOTSUP    	-20160
#define CTSQLRET_PARAMNOTREGSTRD 	-20161
#define CTSQLRET_PARAM_CANT_REGSTRD -20162
#define	CTSQLRET_REG_PARAM_NOTYET 	-20163
#define CTSQLRET_INV_ESCAPE         -20164
#define CTSQLRET_INV_ESCSEQUENCE    -20165
#define CTSQLRET_BATCHHASRESULTSET  -20174
#define CTSQLRET_QEP_INVPTBL 		-20169L
#define CTSQLRET_QEP_NORES 			-20170L
#define CTSQLRET_QEP_NOTDBA 		-20171L
#define CTSQLRET_QEP_BADAUTH 		-20172L
#define CTSQLRET_QEP_NOID 			-20173L
#define CTSQLRET_QEP_DUPID 			-20176L
#define CTSQLRET_NULLCNST 			-20175L
#define CTSQLRET_PKNULL 			-20177L
#define CTSQLRET_FUNCEXISTS 		-20182L
#define CTSQLRET_NOFUNC 			-20183L
#define	CTSQLRET_RPC				-20211
#define	CTSQLRET_DAEMON				-20212
#define	CTSQLRET_SQLSVR				-20213
#define	CTSQLRET_SERVICE			-20214
#define	CTSQLRET_INVHOST			-20215
#define	CTSQLRET_INVDB				-20216
#define	CTSQLRET_NWERR				-20217
#define	CTSQLRET_INVPROTO			-20218
#define	CTSQLRET_INVCONNAME			-20219
#define	CTSQLRET_DUPCONNAME			-20220
#define	CTSQLRET_NOACTIVECONN		-20221
#define	CTSQLRET_NOENVDEFDB			-20222
#define	CTSQLRET_MULTILOCALCON		-20223
#define	CTSQLRET_INVPROTOCOL		-20224
#define	CTSQLRET_EXCEEDEDMAXCON		-20225
#define	CTSQLRET_BADDBHDL			-20226
#define	CTSQLRET_INVHOSTNAME		-20227
#define	CTSQLRET_BADAUTH			-20228
#define	CTSQLRET_INVDATE			-20229
#define	CTSQLRET_INVDATESTR			-20230
#define	CTSQLRET_INVNUMBER			-20231
#define	CTSQLRET_INVNUMSTR			-20232
#define	CTSQLRET_INVTIME			-20233
#define	CTSQLRET_INVTIMESTR			-20234
#define	CTSQLRET_INVTSSTR			-20235
#define	CTSQLRET_DIVBYZERO			-20236
#define CTSQLRET_BADISCAN        	-20237
#define CTSQLRET_INVFORMAT       	-20238
#define CTSQLRET_INV_CHARSET     	-20239
#define CTSQLRET_INV_COLLATE     	-20240
#define CTSQLRET_SERVICE_INUSE   	-20241
#define CTSQLRET_INVTS           	-20242
#define CTSQLRET_ILLEGAL_BUFSZ   	-20243
#define CTSQLRET_COERCION        	-20244
#define CTSQLRET_MISSING_DBOPT   	-20245
#define CTSQLRET_BAD_DBOPT       	-20246
#define CTSQLRET_INV_CHARACTERS  	-20247
#define CTSQLRET_INVENVHDL       	-20254
#define CTSQLRET_IDENTIFIER_OVFLOW    -20248
#define CTSQLRET_IDENTIFIER_INCOMPCHARSET -20249
#define CTSQLRET_INV_IDENTIFIER  	-20250
#define CTSQLRET_MISSING_SQUOT   	-20251
#define CTSQLRET_MISSING_DQUOT   	-20252
#define	CTSQLRET_NO_INDVAR			-20114
#define	CTSQLRET_QUERY_ABORTED		-30001
#define CTSQLRET_INVTBLID        	-23000
#define CTSQLRET_INVIDXID        	-23001
#define	CTSQLRET_VLDONATCHT	    	-25001
#define	CTSQLRET_INVLDONATCHT		-25002
/****************************************************************************\
*
*	SQL Handles:
*
*	pCTSQLCONN
*		SQL connection handle
*
*	pCTSQLCMD
*		SQL command handle
*
*	pCTSQLCURSOR
*
*		SQL cursor handle
*
*	pCTSQLPROCINFO
*
*		Stored procedure(trigger & udf) information handle
*
\****************************************************************************/
typedef struct tagCTSQLCONN		*pCTSQLCONN, **ppCTSQLCONN;
typedef struct tagCTSQLCMD		*pCTSQLCMD, **ppCTSQLCMD;
typedef struct tagCTSQLCURSOR	*pCTSQLCURSOR, **ppCTSQLCURSOR;
typedef void *pCTSQLPROCINFO, **ppCTSQLPROCINFO;

/****************************************************************************\
*
*	c-treeACE SQL connection functions
*
\****************************************************************************/
#ifdef __cplusplus
extern "C" {
#endif
CTSQLRET ctsqlDECL ctsqlfn(SetSSL)(pCTSQLCONN hConn, CTSQLCHAR* certificate);
pCTSQLCONN ctsqlDECL ctsqlfn(NewConnection)(void);
#ifdef CTSQLAPI_SRVR
pCTSQLCONN ctsqlDECL SAPPctsqlNewConnection(BIT attach);
#endif
void ctsqlDECL ctsqlfn(FreeConnection)(pCTSQLCONN hConn);
CTSQLRET ctsqlDECL ctsqlfn(Connect)(pCTSQLCONN hConn, const CTSQLCHAR* dbname, const CTSQLCHAR* username, const CTSQLCHAR* password);
CTSQLRET ctsqlDECL ctsqlfn(Disconnect)(pCTSQLCONN hConn);
CTSQLRET ctsqlDECL ctsqlfn(Attach)(pCTSQLCONN hConn);
CTSQLRET ctsqlDECL ctsqlfn(Detach)(pCTSQLCONN hConn);
CTSQLCHAR* ctsqlDECL ctsqlfn(GetDatabase)(pCTSQLCONN hConn);
CTSQLCHAR* ctsqlDECL ctsqlfn(GetUsername)(pCTSQLCONN hConn);
CTSQLCHAR* ctsqlDECL ctsqlfn(GetPassword)(pCTSQLCONN hConn);
BIT ctsqlDECL ctsqlfn(IsActiveConn)(pCTSQLCONN hConn);
CTSQLRET ctsqlDECL ctsqlfn(Begin)(pCTSQLCONN hConn);
CTSQLRET ctsqlDECL ctsqlfn(Commit)(pCTSQLCONN hConn);
CTSQLRET ctsqlDECL ctsqlfn(Abort)(pCTSQLCONN hConn);
CTSQLCHAR* ctsqlDECL ctsqlfn(GetErrorMessage)(pCTSQLCONN hConn);
CTSQLRET ctsqlDECL ctsqlfn(GetError)(pCTSQLCONN hConn);
void ctsqlDECL ctsqlfn(ClearError)(pCTSQLCONN hConn);
CTSQLRET ctsqlDECL ctsqlfn(SetIsolationLevel)(pCTSQLCONN hConn, CTSQL_ISOLEVEL level);
CTSQL_ISOLEVEL ctsqlDECL ctsqlfn(GetIsolationLevel)(pCTSQLCONN hConn);
BIT ctsqlDECL ctsqlfn(GetAutoCommit)(pCTSQLCONN hConn);
CTSQLRET ctsqlDECL ctsqlfn(SetAutoCommit)(pCTSQLCONN hConn, BIT flag);
CTSQLRET ctsqlDECL ctsqlfn(SetTypeCheckOnGet)(pCTSQLCONN hConn, BIT flag);
CTSQLRET ctsqlDECL ctsqlfn(ClearAutoCommit)(pCTSQLCONN hConn);
INTEGER ctsqlDECL ctsqlfn(GetErrorPos)(pCTSQLCONN hConn);
BIT ctsqlDECL ctsqlfn(IsUnicodeLib)(void);
CTSQLRET ctsqlDECL ctsqlfn(SetPreserveCursor)(pCTSQLCONN hCmd, BIT flag);
#ifdef __cplusplus
}
#endif
/****************************************************************************\
*
*	c-treeACE SQL command functions
*
\****************************************************************************/
#ifdef __cplusplus
extern "C" {
#endif
pCTSQLCMD ctsqlDECL ctsqlfn(NewCommand)(pCTSQLCONN hConn);
void ctsqlDECL ctsqlfn(FreeCommand)(pCTSQLCMD hCmd);
CTSQLRET ctsqlDECL ctsqlfn(GetParameter)(pCTSQLCMD hCmd, INTEGER index, void* buffer, INTEGER length);
CTSQLRET ctsqlDECL ctsqlfn(SetParameter)(pCTSQLCMD hCmd, INTEGER index, CTSQLTYPE ptype, INTEGER length, BIT isnull, const void* buffer);
CTSQLRET ctsqlDECL ctsqlfn(SetParameterAsString)(pCTSQLCMD hCmd, INTEGER index, const CTSQLCHAR* buffer);
CTSQLRET ctsqlDECL ctsqlfn(SetIntegerParameter)(pCTSQLCMD hCmd, INTEGER index, CTSQLTYPE ptype, BIT isnull, INTEGER buffer);
CTSQLRET ctsqlDECL ctsqlfn(SetBigIntParameter)(pCTSQLCMD hCmd, INTEGER index, BIT isnull, BIGINT* buffer);
CTSQLRET ctsqlDECL ctsqlfn(SetNumericParameter)(pCTSQLCMD hCmd, INTEGER index, CTSQLTYPE ptype, BIT isnull, NUMERIC* buffer);
CTSQLRET ctsqlDECL ctsqlfn(SetFloatParameter)(pCTSQLCMD hCmd, INTEGER index, CTSQLTYPE ptype, BIT isnull, FLOATT buffer);
CTSQLRET ctsqlDECL ctsqlfn(SetDateParameter)(pCTSQLCMD hCmd, INTEGER index, BIT isnull, SQLDATE* buffer);
CTSQLRET ctsqlDECL ctsqlfn(SetTimeParameter)(pCTSQLCMD hCmd, INTEGER index, BIT isnull, SQLTIME* buffer);
CTSQLRET ctsqlDECL ctsqlfn(SetTimeStampParameter)(pCTSQLCMD hCmd, INTEGER index, BIT isnull, SQLTIMESTAMP* buffer);
CTSQLRET ctsqlDECL ctsqlfn(SetNCharParameter)(pCTSQLCMD hCmd, INTEGER index, CTSQLTYPE ptype, BIT isnull, const CTSQLCHAR* buffer, INTEGER len);
CTSQLRET ctsqlDECL ctsqlfn(SetCharParameter)(pCTSQLCMD hCmd, INTEGER index, CTSQLTYPE ptype, BIT isnull, const char* buffer, INTEGER len);
CTSQLRET ctsqlDECL ctsqlfn(SetBinaryParameter)(pCTSQLCMD hCmd, INTEGER index, BIT isnull, void* buffer, INTEGER size);
INTEGER ctsqlDECL ctsqlfn(GetParameterCount)(pCTSQLCMD hCmd);
CTSQLTYPE ctsqlDECL ctsqlfn(GetParameterType)(pCTSQLCMD hCmd, INTEGER index);
INTEGER ctsqlDECL ctsqlfn(GetParameterLength)(pCTSQLCMD hCmd, INTEGER index);
SMALLINT ctsqlDECL ctsqlfn(GetParameterPrecision)(pCTSQLCMD hCmd, INTEGER index);
INTEGER ctsqlDECL ctsqlfn(GetParameterPrecisionX)(pCTSQLCMD hCmd, INTEGER index);
SMALLINT ctsqlDECL ctsqlfn(GetParameterScale)(pCTSQLCMD hCmd, INTEGER index);
CTSQL_NULLABILITY ctsqlDECL ctsqlfn(GetParameterNullFlag)(pCTSQLCMD hCmd, INTEGER index);
CTSQLCHAR* ctsqlDECL ctsqlfn(GetParameterName)(pCTSQLCMD hCmd, INTEGER index);
INTEGER ctsqlDECL ctsqlfn(GetParameterIndexByName)(pCTSQLCMD hCmd, const CTSQLCHAR* name);
SQL_PARAMDIR ctsqlDECL ctsqlfn(GetParameterDirection)(pCTSQLCMD hCmd, INTEGER index);
CTSQLRET ctsqlDECL ctsqlfn(SetParameterScale)(pCTSQLCMD hCmd, INTEGER index, SMALLINT scale);
CTSQLRET ctsqlDECL ctsqlfn(SetParameterPrecision)(pCTSQLCMD hCmd, INTEGER index, SMALLINT precision);
CTSQLRET ctsqlDECL ctsqlfn(SetParameterPrecisionX)(pCTSQLCMD hCmd, INTEGER index, INTEGER precision);
CTSQLRET ctsqlDECL ctsqlfn(Prepare)(pCTSQLCMD hCmd, CTSQLCONSTCHAR* sqlcmd);
CTSQLRET ctsqlDECL ctsqlfn(Execute)(pCTSQLCMD hCmd, ppCTSQLCURSOR pCursor);
CTSQLRET ctsqlDECL ctsqlfn(ExecuteDirect)(pCTSQLCMD hCmd, const CTSQLCHAR* sqlcmd);
INTEGER ctsqlDECL ctsqlfn(AffectedRows)(pCTSQLCMD hCmd);
CTSQLRET ctsqlDECL ctsqlfn(Close)(pCTSQLCMD hCmd);
CTSQLRET ctsqlDECL ctsqlfn(SetBlob)(pCTSQLCMD hCmd, INTEGER colnumber, void* buffer, INTEGER size, INTEGER offset);
INTEGER ctsqlDECL ctsqlfn(GetTimeout)(pCTSQLCMD hCmd);
CTSQLRET ctsqlDECL ctsqlfn(SetTimeout)(pCTSQLCMD hCmd, INTEGER timeout);
BIT ctsqlDECL ctsqlfn(IsSelect)(pCTSQLCMD hCmd);
BIT ctsqlDECL ctsqlfn(IsProcedureCall)(pCTSQLCMD hCmd);
CTSQLRET ctsqlDECL ctsqlfn(SetTupleCount)(pCTSQLCMD hCmd, INTEGER count);
INTEGER ctsqlDECL ctsqlfn(GetTupleCount)(pCTSQLCMD hCmd);
CTSQLRET ctsqlDECL ctsqlfn(PrepareBatch)(pCTSQLCMD hCmd, CTSQLCONSTCHAR* sqlcmd, INTEGER batch_size);
INTEGER ctsqlDECL ctsqlfn(NextBatchItem)(pCTSQLCMD hCmd);
CTSQLRET ctsqlDECL ctsqlfn(SetNumericParameterAsString)(pCTSQLCMD hCmd, INTEGER index, CTSQLTYPE ptype, BIT isnull, const CTSQLCHAR* buffer);
CTSQLRET ctsqlDECL ctsqlfn(GetNumericParameterAsString)(pCTSQLCMD hCmd, INTEGER index, CTSQLCHAR* buffer, INTEGER length);
BIT ctsqlDECL ctsqlfn(IsParameterNull)(pCTSQLCMD hCmd, INTEGER index);
CTSQLRET ctsqlDECL ctsqlfn(SetScrollableCursor)(pCTSQLCMD hCmd, BIT flag);
BIT ctsqlDECL ctsqlfn(GetScrollableCursor)(pCTSQLCMD hCmd);
pCTSQLCONN ctsqlDECL ctsqlfn(GetConnectionFromCommand)(pCTSQLCMD hCmd);
TINYINT* ctsqlDECL ctsqlfn(GetParameterAddress)(pCTSQLCMD hCmd, INTEGER index);
#ifdef __cplusplus
}
#endif
/****************************************************************************\
*
*	c-treeACE SQL cursor functions
*
\****************************************************************************/
#ifdef __cplusplus
extern "C" {
#endif
pCTSQLCURSOR ctsqlDECL ctsqlfn(NewCursor)(pCTSQLCMD hCmd);
pCTSQLCURSOR ctsqlDECL ctsqlfn(NewDACursor)(void *psqlda);
void ctsqlDECL ctsqlfn(FreeCursor)(pCTSQLCURSOR hCursor);
INTEGER ctsqlDECL ctsqlfn(GetColumnCount)(pCTSQLCURSOR hCursor);
INTEGER ctsqlDECL ctsqlfn(GetColumnNameLen)(pCTSQLCURSOR hCursor, INTEGER colnumber);
CTSQLCHAR* ctsqlDECL ctsqlfn(GetColumnName)(pCTSQLCURSOR hCursor, INTEGER colnumber);
TINYINT* ctsqlDECL ctsqlfn(GetVariableAddress)(pCTSQLCURSOR hCursor, INTEGER colnumber);
CTSQLRET ctsqlDECL ctsqlfn(GetColumnTitle)(pCTSQLCURSOR hCursor, INTEGER colnumber, CTSQLCHAR* colname, int size);
CTSQLTYPE ctsqlDECL ctsqlfn(GetColumnType)(pCTSQLCURSOR hCursor, INTEGER colnumber);
CTSQLCHAR* ctsqlDECL ctsqlfn(GetColumnCharset)(pCTSQLCURSOR hCursor, INTEGER colnumber);
INTEGER ctsqlDECL ctsqlfn(GetColumnLength)(pCTSQLCURSOR hCursor, INTEGER colnumber);
SMALLINT ctsqlDECL ctsqlfn(GetColumnPrecision)(pCTSQLCURSOR hCursor, INTEGER colnumber);
INTEGER ctsqlDECL ctsqlfn(GetColumnPrecisionX)(pCTSQLCURSOR hCursor, INTEGER colnumber);
SMALLINT ctsqlDECL ctsqlfn(GetColumnScale)(pCTSQLCURSOR hCursor, INTEGER colnumber);
CTSQL_NULLABILITY ctsqlDECL ctsqlfn(IsNullable)(pCTSQLCURSOR hCursor, INTEGER colnumber);
CTSQLRET ctsqlDECL ctsqlfn(Next)(pCTSQLCURSOR hCursor);
CTSQLRET ctsqlDECL ctsqlfn(Prev)(pCTSQLCURSOR hCursor);
CTSQLRET ctsqlDECL ctsqlfn(First)(pCTSQLCURSOR hCursor);
CTSQLRET ctsqlDECL ctsqlfn(Last)(pCTSQLCURSOR hCursor);
CTSQLRET ctsqlDECL ctsqlfn(JumpTo)(pCTSQLCURSOR hCursor, INTEGER nth);
CTSQLRET ctsqlDECL ctsqlfn(GetRow)(pCTSQLCURSOR hCursor, INTEGER nth);

BIT ctsqlDECL ctsqlfn(IsColumnNull)(pCTSQLCURSOR hCursor, INTEGER colnumber);
CTSQLRET ctsqlDECL ctsqlfn(GetNChar)(pCTSQLCURSOR hCursor, INTEGER colnumber, CTSQLCHAR* buffer);
CTSQLRET ctsqlDECL ctsqlfn(GetChar)(pCTSQLCURSOR hCursor, INTEGER colnumber, char* buffer);
CTSQLRET ctsqlDECL ctsqlfn(GetNumeric)(pCTSQLCURSOR hCursor, INTEGER colnumber, NUMERIC* buffer);
CTSQLRET ctsqlDECL ctsqlfn(GetSmallInt)(pCTSQLCURSOR hCursor, INTEGER colnumber, SMALLINT* buffer);
CTSQLRET ctsqlDECL ctsqlfn(GetInteger)(pCTSQLCURSOR hCursor, INTEGER colnumber, INTEGER* buffer);
CTSQLRET ctsqlDECL ctsqlfn(GetReal)(pCTSQLCURSOR hCursor, INTEGER colnumber, SMALLFLOAT* buffer);
CTSQLRET ctsqlDECL ctsqlfn(GetFloat)(pCTSQLCURSOR hCursor, INTEGER colnumber, FLOATT* buffer);
CTSQLRET ctsqlDECL ctsqlfn(GetDate)(pCTSQLCURSOR hCursor, INTEGER colnumber, SQLDATE* buffer);
CTSQLRET ctsqlDECL ctsqlfn(GetMoney)(pCTSQLCURSOR hCursor, INTEGER colnumber, NUMERIC* buffer);
CTSQLRET ctsqlDECL ctsqlfn(GetTime)(pCTSQLCURSOR hCursor, INTEGER colnumber, SQLTIME* buffer);
CTSQLRET ctsqlDECL ctsqlfn(GetTimeStamp)(pCTSQLCURSOR hCursor, INTEGER colnumber, SQLTIMESTAMP* buffer);
CTSQLRET ctsqlDECL ctsqlfn(GetTinyInt)(pCTSQLCURSOR hCursor, INTEGER colnumber, TINYINT* buffer);
CTSQLRET ctsqlDECL ctsqlfn(GetBinary)(pCTSQLCURSOR hCursor, INTEGER colnumber, BINARY* buffer);
INTEGER ctsqlDECL ctsqlfn(GetBytes)(pCTSQLCURSOR hCursor, INTEGER colnumber, UTINYINT buffer[], INTEGER size);
CTSQLRET ctsqlDECL ctsqlfn(GetBit)(pCTSQLCURSOR hCursor, INTEGER colnumber, BIT* buffer);
CTSQLRET ctsqlDECL ctsqlfn(GetBigInt)(pCTSQLCURSOR hCursor, INTEGER colnumber, BIGINT* buffer);
CTSQLRET ctsqlDECL ctsqlfn(GetBlob)(pCTSQLCURSOR hCursor, INTEGER colnumber, void* buffer, INTEGER size, INTEGER offset, INTEGER *bytesread);
CTSQLRET ctsqlDECL ctsqlfn(GetBlobByAddress)(pCTSQLCURSOR hCursor, TINYINT* address, void* buffer, INTEGER size, INTEGER offset, INTEGER *bytesread);
CTSQLRET ctsqlDECL ctsqlfn(GetTableName)(pCTSQLCURSOR hCursor, INTEGER colnumber, CTSQLCHAR* buffer, INTEGER size);
CTSQLRET ctsqlDECL ctsqlfn(GetBaseTableName)(pCTSQLCURSOR hCursor, INTEGER colnumber, CTSQLCHAR* buffer, INTEGER size);
CTSQLRET ctsqlDECL ctsqlfn(GetBaseColumnName)(pCTSQLCURSOR hCursor, INTEGER colnumber, CTSQLCHAR* buffer, INTEGER size);
INTEGER ctsqlDECL ctsqlfn(IsSearchable)(pCTSQLCURSOR hCursor, INTEGER colnumber);
INTEGER ctsqlDECL ctsqlfn(IsUpdatable)(pCTSQLCURSOR hCursor, INTEGER colnumber);
CTSQLRET ctsqlDECL ctsqlfn(GetNumericAsString)(pCTSQLCURSOR hCursor, INTEGER colnumber, CTSQLCHAR* buffer, INTEGER size);
pCTSQLCMD ctsqlDECL ctsqlfn(GetCommandFromCursor)(pCTSQLCURSOR hCursor);
pCTSQLCONN ctsqlDECL ctsqlfn(GetConnectionFromCursor)(pCTSQLCURSOR hCursor);
#ifdef __cplusplus
}
#endif
/****************************************************************************\
*
*	data conversion functions
*
\****************************************************************************/
#ifdef __cplusplus
extern "C" {
#endif
CTSQLRET ctsqlDECL ctsqlfn(Compare)(CTSQLTYPE dtype, const void* buffer1, INTEGER length1, const void* buffer2, INTEGER length2, INTEGER* result);
CTSQLRET ctsqlDECL ctsqlfn(Convert)(CTSQLTYPE stype, const void* buffer1, INTEGER length1, const CTSQLCHAR* fmt, CTSQLTYPE dtype, void* buffer2, INTEGER length2);
int ctsqlDECL ctsqlfn(DayOfWeek)(SQLDATE date);
CTSQLRET ctsqlDECL ctsqlfn(AddNumeric)(NUMERIC* num1, NUMERIC* num2, NUMERIC* res);
CTSQLRET ctsqlDECL ctsqlfn(SubNumeric)(NUMERIC* num1, NUMERIC* num2, NUMERIC* res);
CTSQLRET ctsqlDECL ctsqlfn(MulNumeric)(NUMERIC* num1, NUMERIC* num2, NUMERIC* res);
CTSQLRET ctsqlDECL ctsqlfn(DivNumeric)(NUMERIC* num1, NUMERIC* num2, NUMERIC* res);
CTSQLRET ctsqlDECL ctsqlfn(RoundNumeric)(NUMERIC* num, INTEGER precision, INTEGER scale);
CTSQLRET ctsqlDECL ctsqlfn(NumericToString)(NUMERIC* num, CTSQLCHAR* str, INTEGER len);
CTSQLRET ctsqlDECL ctsqlfn(StringToNumeric)(const CTSQLCHAR *str, INTEGER len, NUMERIC* num);
#ifdef __cplusplus
}
#endif
/****************************************************************************/

/****************************************************************************\
*
*	misc functions
*
\****************************************************************************/
#ifdef __cplusplus
extern "C" {
#endif
CTSQLRET ctsqlDECL ctsqlfn(CleanPooled)(pCTSQLCONN hConn);
pCTSQLPROCINFO ctsqlDECL ctsqlfn(NewProcInfo)(CTSQLCHAR* owner, CTSQLCHAR* name, STP_TYPE kind);
void ctsqlDECL ctsqlfn(FreeProcInfo)(pCTSQLPROCINFO hprocinfo);
CTSQLRET ctsqlDECL ctsqlfn(DumpStp)(pCTSQLCONN hConn, pCTSQLPROCINFO hprocinfo);
CTSQLRET ctsqlDECL ctsqlfn(DumpStpRaw)(pCTSQLCONN hConn, pCTSQLPROCINFO hprocinfo, void** buffer, INTEGER* bufferLen);
CTSQLRET ctsqlDECL ctsqlfn(DeployStp)(pCTSQLCONN hConn, pCTSQLPROCINFO hprocinfo);
CTSQLRET ctsqlDECL ctsqlfn(DeployStpRaw)(pCTSQLCONN hConn, pCTSQLPROCINFO hprocinfo, void* buffer, INTEGER bufferLen);
void ctsqlDECL ctsqlfn(LogStp)(CTSQLCHAR* msg);
void ctsqlDECL ctsqlfn(ErrStp)(CTSQLCHAR* msg);

#ifdef __cplusplus
}
#endif


#ifdef CTSQLAPI_SAPP
#define ctsqlSetSSL ctsqlfn(SetSSL)
#define ctsqlNewConnection() SAPPctsqlNewConnection(0)
#define ctsqlNewConnectionOverCtree() SAPPctsqlNewConnection(1)
#define ctsqlFreeConnection ctsqlfn(FreeConnection)
#define ctsqlConnect ctsqlfn(Connect)
#define ctsqlDisconnect ctsqlfn(Disconnect)
#define ctsqlAttach ctsqlfn(Attach)
#define ctsqlDetach ctsqlfn(Detach)
#define ctsqlGetDatabase ctsqlfn(GetDatabase)
#define ctsqlGetUsername ctsqlfn(GetUsername)
#define ctsqlGetPassword ctsqlfn(GetPassword)
#define ctsqlIsActiveConn ctsqlfn(IsActiveConn)
#define ctsqlBegin ctsqlfn(Begin)
#define ctsqlCommit ctsqlfn(Commit)
#define ctsqlAbort ctsqlfn(Abort)
#define ctsqlGetErrorMessage ctsqlfn(GetErrorMessage)
#define ctsqlGetError ctsqlfn(GetError)
#define ctsqlClearError ctsqlfn(ClearError)
#define ctsqlSetIsolationLevel ctsqlfn(SetIsolationLevel)
#define ctsqlGetIsolationLevel ctsqlfn(GetIsolationLevel)
#define ctsqlGetAutoCommit ctsqlfn(GetAutoCommit)
#define ctsqlSetAutoCommit ctsqlfn(SetAutoCommit)
#define ctsqlSetTypeCheckOnGet ctsqlfn(SetTypeCheckOnGet)
#define ctsqlClearAutoCommit ctsqlfn(ClearAutoCommit)
#define ctsqlGetErrorPos ctsqlfn(GetErrorPos)
#define ctsqlIsUnicodeLib ctsqlfn(IsUnicodeLib)
#define ctsqlSetPreserveCursor ctsqlfn(SetPreserveCursor)

/****************************************************************************\
*
*	c-treeACE SQL command functions
*
\****************************************************************************/

#define ctsqlNewCommand ctsqlfn(NewCommand)
#define ctsqlFreeCommand ctsqlfn(FreeCommand)
#define ctsqlGetParameter ctsqlfn(GetParameter)
#define ctsqlSetParameter ctsqlfn(SetParameter)
#define ctsqlSetParameterAsString ctsqlfn(SetParameterAsString)
#define ctsqlSetIntegerParameter ctsqlfn(SetIntegerParameter)
#define ctsqlSetBigIntParameter ctsqlfn(SetBigIntParameter)
#define ctsqlSetNumericParameter ctsqlfn(SetNumericParameter)
#define ctsqlSetFloatParameter ctsqlfn(SetFloatParameter)
#define ctsqlSetDateParameter ctsqlfn(SetDateParameter)
#define ctsqlSetTimeParameter ctsqlfn(SetTimeParameter)
#define ctsqlSetTimeStampParameter ctsqlfn(SetTimeStampParameter)
#define ctsqlSetNCharParameter ctsqlfn(SetNCharParameter)
#define ctsqlSetCharParameter ctsqlfn(SetCharParameter)
#define ctsqlSetBinaryParameter ctsqlfn(SetBinaryParameter)
#define ctsqlGetParameterCount ctsqlfn(GetParameterCount)
#define ctsqlGetParameterType ctsqlfn(GetParameterType)
#define ctsqlGetParameterLength ctsqlfn(GetParameterLength)
#define ctsqlGetParameterPrecision ctsqlfn(GetParameterPrecision)
#define ctsqlGetParameterPrecisionX ctsqlfn(GetParameterPrecisionX)
#define ctsqlGetParameterScale ctsqlfn(GetParameterScale)
#define ctsqlGetParameterNullFlag ctsqlfn(GetParameterNullFlag)
#define ctsqlGetParameterName ctsqlfn(GetParameterName)
#define ctsqlGetParameterIndexByName ctsqlfn(GetParameterIndexByName)
#define ctsqlGetParameterDirection ctsqlfn(GetParameterDirection)
#define ctsqlSetParameterScale ctsqlfn(SetParameterScale)
#define ctsqlSetParameterPrecision ctsqlfn(SetParameterPrecision)
#define ctsqlSetParameterPrecisionX ctsqlfn(SetParameterPrecisionX)
#define ctsqlPrepare ctsqlfn(Prepare)
#define ctsqlExecute ctsqlfn(Execute)
#define ctsqlExecuteDirect ctsqlfn(ExecuteDirect)
#define ctsqlAffectedRows ctsqlfn(AffectedRows)
#define ctsqlClose ctsqlfn(Close)
#define ctsqlSetBlob ctsqlfn(SetBlob)
#define ctsqlGetTimeout ctsqlfn(GetTimeout)
#define ctsqlSetTimeout ctsqlfn(SetTimeout)
#define ctsqlIsSelect ctsqlfn(IsSelect)
#define ctsqlIsProcedureCall ctsqlfn(IsProcedureCall)
#define ctsqlSetTupleCount ctsqlfn(SetTupleCount)
#define ctsqlGetTupleCount ctsqlfn(GetTupleCount)
#define ctsqlPrepareBatch ctsqlfn(PrepareBatch)
#define ctsqlNextBatchItem ctsqlfn(NextBatchItem)
#define ctsqlSetNumericParameterAsString ctsqlfn(SetNumericParameterAsString)
#define ctsqlGetNumericParameterAsString ctsqlfn(GetNumericParameterAsString)
#define ctsqlIsParameterNull ctsqlfn(IsParameterNull)
#define ctsqlSetScrollableCursor ctsqlfn(SetScrollableCursor)
#define ctsqlGetScrollableCursor ctsqlfn(GetScrollableCursor)
#define ctsqlGetConnectionFromCommand ctsqlfn(GetConnectionFromCommand)
#define ctsqlGetParameterAddress ctsqlfn(GetParameterAddress)

/****************************************************************************\
*
*	c-treeACE SQL cursor functions
*
\****************************************************************************/

#define ctsqlNewCursor ctsqlfn(NewCursor)
#define ctsqlNewDACursor ctsqlfn(NewDACursor)
#define ctsqlFreeCursor ctsqlfn(FreeCursor)
#define ctsqlGetColumnCount ctsqlfn(GetColumnCount)
#define ctsqlGetColumnNameLen ctsqlfn(GetColumnNameLen)
#define ctsqlGetColumnName ctsqlfn(GetColumnName)
#define ctsqlGetVariableAddress ctsqlfn(GetVariableAddress)
#define ctsqlGetColumnTitle ctsqlfn(GetColumnTitle)
#define ctsqlGetColumnType ctsqlfn(GetColumnType)
#define ctsqlGetColumnCharset ctsqlfn(GetColumnCharset)
#define ctsqlGetColumnLength ctsqlfn(GetColumnLength)
#define ctsqlGetColumnPrecision ctsqlfn(GetColumnPrecision)
#define ctsqlGetColumnPrecisionX ctsqlfn(GetColumnPrecisionX)
#define ctsqlGetColumnScale ctsqlfn(GetColumnScale)
#define ctsqlIsNullable ctsqlfn(IsNullable)
#define ctsqlNext ctsqlfn(Next)
#define ctsqlPrev ctsqlfn(Prev)
#define ctsqlFirst ctsqlfn(First)
#define ctsqlLast ctsqlfn(Last)
#define ctsqlJumpTo ctsqlfn(JumpTo)
#define ctsqlGetRow ctsqlfn(GetRow)

#define ctsqlIsColumnNull ctsqlfn(IsColumnNull)
#define ctsqlGetNChar ctsqlfn(GetNChar)
#define ctsqlGetChar ctsqlfn(GetChar)
#define ctsqlGetNumeric ctsqlfn(GetNumeric)
#define ctsqlGetSmallInt ctsqlfn(GetSmallInt)
#define ctsqlGetInteger ctsqlfn(GetInteger)
#define ctsqlGetReal ctsqlfn(GetReal)
#define ctsqlGetFloat ctsqlfn(GetFloat)
#define ctsqlGetDate ctsqlfn(GetDate)
#define ctsqlGetMoney ctsqlfn(GetMoney)
#define ctsqlGetTime ctsqlfn(GetTime)
#define ctsqlGetTimeStamp ctsqlfn(GetTimeStamp)
#define ctsqlGetTinyInt ctsqlfn(GetTinyInt)
#define ctsqlGetBinary ctsqlfn(GetBinary)
#define ctsqlGetBytes ctsqlfn(GetBytes)
#define ctsqlGetBit ctsqlfn(GetBit)
#define ctsqlGetBigInt ctsqlfn(GetBigInt)
#define ctsqlGetBlob ctsqlfn(GetBlob)
#define ctsqlGetBlobByAddress ctsqlfn(GetBlobByAddress)
#define ctsqlGetTableName ctsqlfn(GetTableName)
#define ctsqlGetBaseTableName ctsqlfn(GetBaseTableName)
#define ctsqlGetBaseColumnName ctsqlfn(GetBaseColumnName)
#define ctsqlIsSearchable ctsqlfn(IsSearchable)
#define ctsqlIsUpdatable ctsqlfn(IsUpdatable)
#define ctsqlGetNumericAsString ctsqlfn(GetNumericAsString)
#define ctsqlGetCommandFromCursor ctsqlfn(GetCommandFromCursor)
#define ctsqlGetConnectionFromCursor ctsqlfn(GetConnectionFromCursor)

/****************************************************************************\
*
*	data conversion functions
*
\****************************************************************************/

#define ctsqlCompare ctsqlfn(Compare)
#define ctsqlConvert ctsqlfn(Convert)
#define ctsqlDayOfWeek ctsqlfn(DayOfWeek)
#define ctsqlAddNumeric ctsqlfn(AddNumeric)
#define ctsqlSubNumeric ctsqlfn(SubNumeric)
#define ctsqlMulNumeric ctsqlfn(MulNumeric)
#define ctsqlDivNumeric ctsqlfn(DivNumeric)
#define ctsqlRoundNumeric ctsqlfn(RoundNumeric)
#define ctsqlNumericToString ctsqlfn(NumericToString)
#define ctsqlStringToNumeric ctsqlfn(StringToNumeric)
/****************************************************************************/

/****************************************************************************\
*
*	misc functions
*
\****************************************************************************/

#define ctsqlCleanPooled ctsqlfn(CleanPooled)
#define ctsqlNewProcInfo ctsqlfn(NewProcInfo)
#define ctsqlFreeProcInfo ctsqlfn(FreeProcInfo)
#define ctsqlDumpStp ctsqlfn(DumpStp)
#define ctsqlDumpStpRaw ctsqlfn(DumpStpRaw)
#define ctsqlDeployStp ctsqlfn(DeployStp)
#define ctsqlDeployStpRaw ctsqlfn(DeployStpRaw)
#define ctsqlLogStp ctsqlfn(LogStp)
#define ctsqlErrStp ctsqlfn(ErrStp)
#endif /* CTSQLAPI_SAPP */

#endif /* _CTSQLAPI_H_ */

/* end of ctsqlsdk.h */
