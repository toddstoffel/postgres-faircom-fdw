/*
 *      OFFICIAL NOTIFICATION: the following CONFIDENTIAL and PROPRIETARY
 * 	property legend shall not be removed from this source code module
 * 	for any reason.
 *
 *	This program is the CONFIDENTIAL and PROPRIETARY property
 *	of FairCom(R) Corporation. Any unauthorized use, reproduction or
 *	transfer of this computer program is strictly prohibited.
 *
 *      Copyright (c) 1992 - 2024 FairCom Corporation.
 *	This is an unpublished work, and is subject to limited distribution and
 *	restricted disclosure only. ALL RIGHTS RESERVED.
 *
 *	In addition:
 *	This Module may also contain Proprietary Information of
 *	Dharma Systems Inc. and Dharma Systems (P) Ltd.
 *	and should be treated as Confidential.
 *
 *	Copyright (C) Dharma Systems Inc.        1988-2007.
 *	Copyright (C) Dharma Systems (P) Ltd.    1988-2007.
 *
 *
 *			RESTRICTED RIGHTS LEGEND
 *	Use, duplication, or disclosure by the Government is subject to
 *	restrictions set forth in subparagraph (c)(1)(ii) of the Rights in
 * 	Technical Data and Computer Software clause at DFARS 252.227-7013.
 *	FairCom Corporation, 6300 West Sugar Creek Drive, Columbia, MO 65203.
 *
 *	c-treeACE SQL(tm)	Version 11
 */

/*
 * IMPORTANT!
 * This header file is supposed to be used only for Unix environment.
 * For Windows please use the "sqltypes.h" header file supplied by the compiler.
 */

#ifndef _SQLTYPES_H_
#define _SQLTYPES_H_

#define SQL_API
#define NEAR
#define FAR
#define EXPORT
#define PASCAL
#define VOID void
#define CALLBACK
#define _cdecl
#define __stdcall

#ifdef __cplusplus
extern "C" {
#endif

/*
 * Windows-style typedefs
 */
typedef int                     BOOL;
typedef unsigned char           BYTE;
typedef char                    CHAR;
typedef unsigned long           DWORD;
typedef int                     (*FARPROC)();
typedef int                     GLOBALHANDLE;
typedef VOID *                  HANDLE;
typedef VOID *                  HINSTANCE;
typedef VOID *                  HKEY;
typedef VOID *                  HMODULE;
typedef VOID *                  HWND;
typedef unsigned long           LPARAM;
typedef BOOL *                  LPBOOL;
typedef BYTE *                  LPBYTE;
typedef const char *            LPCSTR;
typedef const char *            LPCWSTR;
typedef DWORD *                 LPDWORD;
typedef char *                  LPSTR;
typedef VOID *                  LPVOID;
typedef char *                  LPWSTR;
typedef VOID *                  PHKEY;
typedef VOID *                  PVOID;
typedef char                    TCHAR;
typedef unsigned int            UINT;
typedef char                    VCHAR;
typedef unsigned short          WORD;
typedef unsigned int            WPARAM;
typedef const TCHAR*            LPCTSTR;
typedef TCHAR *                 LPTSTR;

/*
 * SQL Data Types
 */
typedef unsigned char           SQLCHAR;
typedef unsigned char           UCHAR;
typedef signed char             SCHAR;
typedef SCHAR                   SQLSCHAR;
typedef unsigned char           SQLDATE;
typedef unsigned char           SQLDECIMAL;
typedef double                  SQLDOUBLE;
typedef double                  SQLFLOAT;
typedef short int               SWORD;
typedef unsigned short int      UWORD;
typedef signed long             SLONG;
typedef unsigned long           ULONG;
typedef signed short            SSHORT;
typedef unsigned short          USHORT;
typedef double                  SDOUBLE;
typedef double                  LDOUBLE;
typedef float                   SFLOAT;
typedef unsigned char           SQLNUMERIC;
typedef void *                  SQLPOINTER;
typedef long                    SQLADDRESS;
typedef float                   SQLREAL;
typedef short                   SQLSMALLINT;
typedef unsigned short          SQLUSMALLINT;
typedef unsigned char           SQLTIME;
typedef unsigned char           SQLTIMESTAMP;
typedef unsigned char           SQLVARCHAR;
typedef unsigned long int       BOOKMARK;
typedef wchar_t                 SQLWCHAR;

#ifndef DH_64BIT_MODEL
typedef long                    SQLINTEGER;
typedef unsigned long           SQLUINTEGER;
typedef long int                SDWORD;
typedef unsigned long int       UDWORD;
#else
typedef int                     SQLINTEGER;
typedef unsigned int            SQLUINTEGER;
typedef int                     SDWORD;
typedef unsigned int            UDWORD;
#endif

#ifdef UNICODE
typedef SQLWCHAR                SQLTCHAR;
#else
typedef SQLCHAR                 SQLTCHAR;
#endif

#ifdef DH_64BIT_MODEL
#include <stddef.h>
typedef ptrdiff_t               ODBCINT64;
typedef size_t	           	UODBCINT64;
typedef ODBCINT64		SQLLEN;
typedef UODBCINT64		SQLULEN;
typedef UODBCINT64		SQLSETPOSIROW;
#else
#define SQLLEN			SQLINTEGER
#define SQLULEN			SQLUINTEGER
#define SQLSETPOSIROW		SQLUSMALLINT
#ifdef HAVE_LONG_LONG
typedef long long               ODBCINT64;
typedef unsigned long long      UODBCINT64;
# else
struct __bigint_struct
{
    int             hiword;
    unsigned int    loword;
};
struct __bigint_struct_u
{
    unsigned int    hiword;
    unsigned int    loword;
};
typedef struct __bigint_struct ODBCINT64;
typedef struct __bigint_struct_u UODBCINT64;
#endif
#endif
#ifdef ODBCINT64
typedef ODBCINT64               SQLBIGINT;
#endif
#ifdef UODBCINT64
typedef UODBCINT64              SQLUBIGINT;
#endif

typedef SQLSMALLINT             SQLRETURN;
typedef SQLSMALLINT             RETCODE;

typedef void*                   PTR;
typedef PTR                     SQLHANDLE;
typedef SQLHANDLE               SQLHENV;
typedef SQLHANDLE               SQLHDBC;
typedef SQLHANDLE               SQLHSTMT;
typedef SQLHANDLE               SQLHDESC;
typedef SQLHANDLE               HENV;
typedef SQLHANDLE               HDBC;
typedef SQLHANDLE               HSTMT;
typedef SQLHANDLE               SQLHWND;

/*
 * Generic Data Structures
 */

typedef struct tagDATE_STRUCT
{
        SQLSMALLINT    year;
        SQLUSMALLINT   month;
        SQLUSMALLINT   day;
} DATE_STRUCT;

typedef DATE_STRUCT             SQL_DATE_STRUCT;

typedef struct tagTIME_STRUCT
{
        SQLUSMALLINT   hour;
        SQLUSMALLINT   minute;
        SQLUSMALLINT   second;
} TIME_STRUCT;

typedef TIME_STRUCT             SQL_TIME_STRUCT;

typedef struct tagTIMESTAMP_STRUCT
{
        SQLSMALLINT    year;
        SQLUSMALLINT   month;
        SQLUSMALLINT   day;
        SQLUSMALLINT   hour;
        SQLUSMALLINT   minute;
        SQLUSMALLINT   second;
        SQLUINTEGER    fraction;
} TIMESTAMP_STRUCT;

typedef TIMESTAMP_STRUCT        SQL_TIMESTAMP_STRUCT;

typedef enum
{
  SQL_IS_YEAR             = 1,
  SQL_IS_MONTH            = 2,
  SQL_IS_DAY              = 3,
  SQL_IS_HOUR             = 4,
  SQL_IS_MINUTE           = 5,
  SQL_IS_SECOND           = 6,
  SQL_IS_YEAR_TO_MONTH    = 7,
  SQL_IS_DAY_TO_HOUR      = 8,
  SQL_IS_DAY_TO_MINUTE    = 9,
  SQL_IS_DAY_TO_SECOND    = 10,
  SQL_IS_HOUR_TO_MINUTE   = 11,
  SQL_IS_HOUR_TO_SECOND   = 12,
  SQL_IS_MINUTE_TO_SECOND = 13
} SQLINTERVAL;

typedef struct tagSQL_YEAR_MONTH
{
        SQLUINTEGER     year;
        SQLUINTEGER     month;
} SQL_YEAR_MONTH_STRUCT;

typedef struct tagSQL_DAY_SECOND
{
        SQLUINTEGER     day;
        SQLUINTEGER     hour;
        SQLUINTEGER     minute;
        SQLUINTEGER     second;
        SQLUINTEGER     fraction;
} SQL_DAY_SECOND_STRUCT;

typedef struct tagSQL_INTERVAL_STRUCT
{
  SQLINTERVAL     interval_type;
  SQLSMALLINT     interval_sign;
  union {
          SQL_YEAR_MONTH_STRUCT   year_month;
          SQL_DAY_SECOND_STRUCT   day_second;
  } intval;

} SQL_INTERVAL_STRUCT;

#define SQL_MAX_NUMERIC_LEN             16
typedef struct tagSQL_NUMERIC_STRUCT
{
  SQLCHAR       precision;
  SQLSCHAR      scale;
  SQLCHAR       sign;   /* 1 if positive, 0 if negative */
  SQLCHAR       val[SQL_MAX_NUMERIC_LEN];
} SQL_NUMERIC_STRUCT;

#ifdef __cplusplus
}       /* End of extern "C" { */
#endif

#endif  /* #ifndef _SQLTYPES_H_ */
