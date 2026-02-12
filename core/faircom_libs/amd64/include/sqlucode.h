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
 * For Windows please use the "sqlucode.h" header file supplied by the compiler.
 */

#ifndef _SQLUCODE_H_
#define _SQLUCODE_H_

#ifdef __cplusplus
extern "C" {
#endif

#define SQL_WCHAR               (-8)
#define SQL_WVARCHAR            (-9)
#define SQL_WLONGVARCHAR        (-10)
#define SQL_C_WCHAR             SQL_WCHAR

#ifdef UNICODE
#define SQL_C_TCHAR             SQL_C_WCHAR
#else
#define SQL_C_TCHAR             SQL_C_CHAR
#endif

#define SQL_SQLSTATE_SIZEW      10

/*
 * UNICODE functions
 */

RETCODE SQL_API SQLBrowseConnectW(SQLHDBC hdbc,
                                  SQLTCHAR * szConnStrIn,
                                  SQLSMALLINT cbConnStrIn,
                                  SQLTCHAR * szConnStrOut,
                                  SQLSMALLINT cbConnStrOutMax,
                                  SQLSMALLINT * pcbConnStrOut);

#ifndef DH_ODBC2X
RETCODE SQL_API SQLColAttributeW(SQLHSTMT StatementHandle,
#ifndef _UNICODE
                                 SQLUSMALLINT ColumnNumber,
                                 SQLUSMALLINT FieldIdentifier,
#else
                                 SQLSMALLINT ColumnNumber,
                                 SQLSMALLINT FieldIdentifier,
#endif
                                 SQLPOINTER CharacterAttribute,
                                 SQLSMALLINT BufferLength,
                                 SQLSMALLINT * StringLength,
                                 SQLPOINTER  NumericAttribute);
#endif

RETCODE SQL_API SQLColAttributesW(SQLHSTMT hstmt,
                                  SQLUSMALLINT icol,
                                  SQLUSMALLINT fDescType,
                                  PTR rgbDesc,
                                  SQLSMALLINT cbDescMax,
                                  SQLSMALLINT * pcbDesc,
                                  SQLINTEGER * pfDesc);

RETCODE SQL_API SQLColumnPrivilegesW(SQLHSTMT hstmt,
                                     SQLTCHAR * szTableQualifier,
                                     SQLSMALLINT cbTableQualifier,
                                     SQLTCHAR * szTableOwner,
                                     SQLSMALLINT cbTableOwner,
                                     SQLTCHAR * szTableName,
                                     SQLSMALLINT cbTableName,
                                     SQLTCHAR * szColumnName,
                                     SQLSMALLINT cbColumnName);

RETCODE SQL_API SQLColumnsW(SQLHSTMT hstmt,
                            SQLTCHAR * szTableQualifier,
                            SQLSMALLINT cbTableQualifier,
                            SQLTCHAR * szTableOwner,
                            SQLSMALLINT cbTableOwner,
                            SQLTCHAR * szTableName,
                            SQLSMALLINT cbTableName,
                            SQLTCHAR * szColumnName,
                            SQLSMALLINT cbColumnName);

RETCODE SQL_API SQLConnectW(SQLHDBC hdbc,
                            SQLTCHAR * szDSN,
                            SQLSMALLINT cbDSN,
                            SQLTCHAR * szUID,
                            SQLSMALLINT cbUID,
                            SQLTCHAR * szAuthStr,
                            SQLSMALLINT cbAuthStr);

RETCODE SQL_API SQLDataSourcesW(SQLHENV EnvironmentHandle,
                                SQLUSMALLINT Direction,
                                SQLCHAR * ServerName,
                                SQLSMALLINT BufferLength1,
                                SQLSMALLINT * NameLength1,
                                SQLCHAR * Description,
                                SQLSMALLINT BufferLength2,
                                SQLSMALLINT * NameLength2);

RETCODE SQL_API SQLDescribeColW(SQLHSTMT hstmt,
                                SQLUSMALLINT icol,
                                SQLTCHAR * szColName,
                                SQLSMALLINT cbColNameMax,
                                SQLSMALLINT * pcbColName,
                                SQLSMALLINT * pfSqlType,
                                SQLUINTEGER * pcbColDef,
                                SQLSMALLINT * pibScale,
                                SQLSMALLINT * pfNullable);

RETCODE SQL_API SQLDriverConnectW(SQLHDBC hdbc,
                                  SQLHWND hwnd,
                                  SQLTCHAR * szConnStrIn,
                                  SQLSMALLINT cbConnStrIn,
                                  SQLTCHAR * szConnStrOut,
                                  SQLSMALLINT cbConnStrOutMax,
                                  SQLSMALLINT * pcbConnStrOut,
                                  SQLUSMALLINT fDriverCompletion);

RETCODE SQL_API SQLErrorW(HENV henv,
                          SQLHDBC hdbc,
                          SQLHSTMT hstmt,
                          SQLTCHAR * szSqlState,
                          SQLINTEGER * pfNativeError,
                          SQLTCHAR * szErrorMsg,
                          SQLSMALLINT cbErrorMsgMax,
                          SQLSMALLINT * pcbErrorMsg);

RETCODE SQL_API SQLExecDirectW(SQLHSTMT hstmt,
                               SQLTCHAR * szSqlStr,
                               SQLINTEGER cbSqlStr);

RETCODE SQL_API SQLForeignKeysW(SQLHSTMT hstmt,
                                SQLTCHAR * szPkTableQualifier,
                                SQLSMALLINT cbPkTableQualifier,
                                SQLTCHAR * szPkTableOwner,
                                SQLSMALLINT cbPkTableOwner,
                                SQLTCHAR * szPkTableName,
                                SQLSMALLINT cbPkTableName,
                                SQLTCHAR * szFkTableQualifier,
                                SQLSMALLINT cbFkTableQualifier,
                                SQLTCHAR * szFkTableOwner,
                                SQLSMALLINT cbFkTableOwner,
                                SQLTCHAR * szFkTableName,
                                SQLSMALLINT cbFkTableName);

#ifndef DH_ODBC2X
RETCODE SQL_API SQLGetConnectAttrW(SQLHDBC ConnectionHandle,
                                   SQLINTEGER Attribute,
                                   SQLPOINTER ValuePtr,
                                   SQLINTEGER BufferLength,
                                   SQLINTEGER *StringLengthptr);
#endif

RETCODE SQL_API SQLGetConnectOptionW(SQLHDBC hdbc,
                                     SQLUSMALLINT fOption,
                                     PTR pvParam);

RETCODE SQL_API SQLGetCursorNameW(SQLHSTMT hstmt,
                                  SQLTCHAR * szCursor,
                                  SQLSMALLINT cbCursorMax,
                                  SQLSMALLINT * pcbCursor);

RETCODE SQL_API SQLGetDescFieldW(SQLHDESC DescriptorHandle,
                                 SQLSMALLINT RecNumber,
                                 SQLSMALLINT FieldIdentifier,
                                 SQLPOINTER ValuePtr,
                                 SQLINTEGER BufferLength,
                                 SQLINTEGER *StringLengthPtr);

RETCODE SQL_API SQLGetDescRecW(SQLHDESC DescriptorHandle,
                               SQLSMALLINT RecNumber,
                               SQLTCHAR *Name,
                               SQLSMALLINT BufferLength,
                               SQLSMALLINT *StringLengthPtr,
                               SQLSMALLINT *TypePtr,
                               SQLSMALLINT *SubTypePtr,
                               SQLINTEGER *LengthPtr,
                               SQLSMALLINT *PrecisionPtr,
                               SQLSMALLINT *ScalePtr,
                               SQLSMALLINT *NullablePtr);

RETCODE SQL_API SQLGetDiagFieldW(SQLSMALLINT HandleType,
                                 SQLHANDLE Handle,
                                 SQLSMALLINT RecNumber,
                                 SQLSMALLINT DiagIdentifier,
                                 SQLPOINTER DiagInfo,
                                 SQLSMALLINT BufferLength,
                                 SQLSMALLINT *StringLength);

RETCODE SQL_API SQLGetDiagRecW(SQLSMALLINT HandleType,
                              SQLHANDLE Handle,
                              SQLSMALLINT RecNumber,
                              SQLTCHAR *Sqlstate,
                              SQLINTEGER *NativeErrorPtr,
                              SQLTCHAR *MessageText,
                              SQLSMALLINT BufferLength,
                              SQLSMALLINT *TextLengthPtr);

RETCODE SQL_API SQLGetInfoW(SQLHDBC hdbc,
                            SQLUSMALLINT fInfoType,
                            PTR rgbInfoValue,
                            SQLSMALLINT cbInfoValueMax,
                            SQLSMALLINT * pcbInfoValue);

#ifndef DH_ODBC2X
RETCODE SQL_API SQLGetStmtAttrW(SQLHSTMT StatementHandle,
                                SQLINTEGER Attribute,
                                SQLPOINTER ValuePtr,
                                SQLINTEGER BufferLength,
                                SQLINTEGER *StringLengthptr);
#endif

RETCODE SQL_API SQLGetTypeInfoW(SQLHSTMT hstmt, SQLSMALLINT fSqlType);

RETCODE SQL_API SQLNativeSqlW(SQLHDBC hdbc,
                              SQLTCHAR * szSqlStrIn,
                              SQLINTEGER cbSqlStrIn,
                              SQLTCHAR * szSqlStr,
                              SQLINTEGER cbSqlStrMax,
                              SQLINTEGER * pcbSqlStr);

RETCODE SQL_API SQLPrepareW(SQLHSTMT hstmt,
                            SQLTCHAR * szSqlStr,
                            SQLINTEGER cbSqlStr);

RETCODE SQL_API SQLPrimaryKeysW(SQLHSTMT hstmt,
                                SQLTCHAR * szTableQualifier,
                                SQLSMALLINT cbTableQualifier,
                                SQLTCHAR * szTableOwner,
                                SQLSMALLINT cbTableOwner,
                                SQLTCHAR * szTableName,
                                SQLSMALLINT cbTableName);

RETCODE SQL_API SQLProcedureColumnsW(SQLHSTMT hstmt,
                                     SQLTCHAR * szProcQualifier,
                                     SQLSMALLINT cbProcQualifier,
                                     SQLTCHAR * szProcOwner,
                                     SQLSMALLINT cbProcOwner,
                                     SQLTCHAR * szProcName,
                                     SQLSMALLINT cbProcName,
                                     SQLTCHAR * szColumnName,
                                     SQLSMALLINT cbColumnName);

RETCODE SQL_API SQLProceduresW(SQLHSTMT hstmt,
                               SQLTCHAR * szProcQualifier,
                               SQLSMALLINT cbProcQualifier,
                               SQLTCHAR * szProcOwner,
                               SQLSMALLINT cbProcOwner,
                               SQLTCHAR * szProcName,
                               SQLSMALLINT cbProcName);

#ifndef DH_ODBC2X
RETCODE SQL_API SQLSetConnectAttrW(SQLHDBC ConnectionHandle,
                                   SQLINTEGER Attribute,
                                   SQLPOINTER ValuePtr,
                                   SQLINTEGER StringLength);
#endif

RETCODE SQL_API SQLSetConnectOptionW(SQLHDBC hdbc,
                                     SQLUSMALLINT fOption,
                                     SQLUINTEGER vParam);

RETCODE SQL_API SQLSetCursorNameW(SQLHSTMT hstmt,
                                  SQLTCHAR * szCursor,
                                  SQLSMALLINT cbCursor);

RETCODE SQL_API SQLSetDescFieldW(SQLHDESC DescriptorHandle,
                                 SQLSMALLINT RecNumber,
                                 SQLSMALLINT FieldIdentifier,
                                 SQLPOINTER ValuePtr,
                                 SQLINTEGER BufferLength);

#ifndef DH_ODBC2X
RETCODE SQL_API SQLSetStmtAttrW(SQLHSTMT StatementHandle,
                                SQLINTEGER Attribute,
                                SQLPOINTER ValuePtr,
                                SQLINTEGER StringLength);
#endif

RETCODE SQL_API SQLTablePrivilegesW(SQLHSTMT hstmt,
                                    SQLTCHAR * szTableQualifier,
                                    SQLSMALLINT cbTableQualifier,
                                    SQLTCHAR * szTableOwner,
                                    SQLSMALLINT cbTableOwner,
                                    SQLTCHAR * szTableName,
                                    SQLSMALLINT cbTableName);

RETCODE SQL_API SQLSpecialColumnsW(SQLHSTMT hstmt,
                                   SQLUSMALLINT fColType,
                                   SQLTCHAR * szTableQualifier,
                                   SQLSMALLINT cbTableQualifier,
                                   SQLTCHAR * szTableOwner,
                                   SQLSMALLINT cbTableOwner,
                                   SQLTCHAR * szTableName,
                                   SQLSMALLINT cbTableName,
                                   SQLUSMALLINT fScope,
                                   SQLUSMALLINT fNullable);

RETCODE SQL_API SQLStatisticsW(SQLHSTMT hstmt,
                               SQLTCHAR * szTableQualifier,
                               SQLSMALLINT cbTableQualifier,
                               SQLTCHAR * szTableOwner,
                               SQLSMALLINT cbTableOwner,
                               SQLTCHAR * szTableName,
                               SQLSMALLINT cbTableName,
                               SQLUSMALLINT fUnique,
                               SQLUSMALLINT fAccuracy);

RETCODE SQL_API SQLTablesW(SQLHSTMT hstmt,
                           SQLTCHAR * szTableQualifier,
                           SQLSMALLINT cbTableQualifier,
                           SQLTCHAR * szTableOwner,
                           SQLSMALLINT cbTableOwner,
                           SQLTCHAR * szTableName,
                           SQLSMALLINT cbTableName,
                           SQLTCHAR * szTableType,
                           SQLSMALLINT cbTableType);


/*
 * ANSI functions
 */

RETCODE SQL_API SQLBrowseConnectA(SQLHDBC hdbc,
                                  SQLTCHAR * szConnStrIn,
                                  SQLSMALLINT cbConnStrIn,
                                  SQLTCHAR * szConnStrOut,
                                  SQLSMALLINT cbConnStrOutMax,
                                  SQLSMALLINT * pcbConnStrOut);

#ifndef DH_ODBC2X
RETCODE SQL_API SQLColAttributeA(SQLHSTMT StatementHandle,
#ifndef _UNICODE
                                 SQLUSMALLINT ColumnNumber,
                                 SQLUSMALLINT FieldIdentifier,
#else
                                 SQLSMALLINT ColumnNumber,
                                 SQLSMALLINT FieldIdentifier,
#endif
                                 SQLPOINTER CharacterAttribute,
                                 SQLSMALLINT BufferLength,
                                 SQLSMALLINT * StringLength,
                                 SQLPOINTER  NumericAttribute);
#endif

RETCODE SQL_API SQLColAttributesA(SQLHSTMT hstmt,
                                  SQLUSMALLINT icol,
                                  SQLUSMALLINT fDescType,
                                  PTR rgbDesc,
                                  SQLSMALLINT cbDescMax,
                                  SQLSMALLINT * pcbDesc,
                                  SQLINTEGER * pfDesc);

RETCODE SQL_API SQLColumnsA(SQLHSTMT hstmt,
                            SQLTCHAR * szTableQualifier,
                            SQLSMALLINT cbTableQualifier,
                            SQLTCHAR * szTableOwner,
                            SQLSMALLINT cbTableOwner,
                            SQLTCHAR * szTableName,
                            SQLSMALLINT cbTableName,
                            SQLTCHAR * szColumnName,
                            SQLSMALLINT cbColumnName);

RETCODE SQL_API SQLColumnPrivilegesA(SQLHSTMT hstmt,
                                     SQLTCHAR * szTableQualifier,
                                     SQLSMALLINT cbTableQualifier,
                                     SQLTCHAR * szTableOwner,
                                     SQLSMALLINT cbTableOwner,
                                     SQLTCHAR * szTableName,
                                     SQLSMALLINT cbTableName,
                                     SQLTCHAR * szColumnName,
                                     SQLSMALLINT cbColumnName);

RETCODE SQL_API SQLConnectA(SQLHDBC hdbc,
                            SQLTCHAR * szDSN,
                            SQLSMALLINT cbDSN,
                            SQLTCHAR * szUID,
                            SQLSMALLINT cbUID,
                            SQLTCHAR * szAuthStr,
                            SQLSMALLINT cbAuthStr);

RETCODE SQL_API SQLDataSourcesA(SQLHENV EnvironmentHandle,
                                SQLUSMALLINT Direction,
                                SQLCHAR * ServerName,
                                SQLSMALLINT BufferLength1,
                                SQLSMALLINT * NameLength1,
                                SQLCHAR * Description,
                                SQLSMALLINT BufferLength2,
                                SQLSMALLINT * NameLength2);

RETCODE SQL_API SQLDescribeColA(SQLHSTMT hstmt,
                                SQLUSMALLINT icol,
                                SQLTCHAR * szColName,
                                SQLSMALLINT cbColNameMax,
                                SQLSMALLINT * pcbColName,
                                SQLSMALLINT * pfSqlType,
                                SQLUINTEGER * pcbColDef,
                                SQLSMALLINT * pibScale,
                                SQLSMALLINT * pfNullable);

RETCODE SQL_API SQLDescribeParamA(SQLHSTMT hstmt,
                                  SQLUSMALLINT ipar,
                                  SQLSMALLINT * pfSqlType,
                                  SQLUINTEGER * pcbColDef,
                                  SQLSMALLINT * pibScale,
                                  SQLSMALLINT * pfNullable);

RETCODE SQL_API SQLDriverConnectA(SQLHDBC hdbc,
                                  SQLHWND hwnd,
                                  SQLTCHAR * szConnStrIn,
                                  SQLSMALLINT cbConnStrIn,
                                  SQLTCHAR * szConnStrOut,
                                  SQLSMALLINT cbConnStrOutMax,
                                  SQLSMALLINT * pcbConnStrOut,
                                  SQLUSMALLINT fDriverCompletion);

RETCODE SQL_API SQLErrorA(HENV henv,
                          SQLHDBC hdbc,
                          SQLHSTMT hstmt,
                          SQLTCHAR * szSqlState,
                          SQLINTEGER * pfNativeError,
                          SQLTCHAR * szErrorMsg,
                          SQLSMALLINT cbErrorMsgMax,
                          SQLSMALLINT * pcbErrorMsg);

RETCODE SQL_API SQLExecDirectA(SQLHSTMT hstmt,
                               SQLTCHAR * szSqlStr,
                               SQLINTEGER cbSqlStr);

RETCODE SQL_API SQLForeignKeysA(SQLHSTMT hstmt,
                                SQLTCHAR * szPkTableQualifier,
                                SQLSMALLINT cbPkTableQualifier,
                                SQLTCHAR * szPkTableOwner,
                                SQLSMALLINT cbPkTableOwner,
                                SQLTCHAR * szPkTableName,
                                SQLSMALLINT cbPkTableName,
                                SQLTCHAR * szFkTableQualifier,
                                SQLSMALLINT cbFkTableQualifier,
                                SQLTCHAR * szFkTableOwner,
                                SQLSMALLINT cbFkTableOwner,
                                SQLTCHAR * szFkTableName,
                                SQLSMALLINT cbFkTableName);

#ifndef DH_ODBC2X
RETCODE SQL_API SQLGetConnectAttrA(SQLHDBC ConnectionHandle,
                                   SQLINTEGER Attribute,
                                   SQLPOINTER ValuePtr,
                                   SQLINTEGER BufferLength,
                                   SQLINTEGER *StringLengthptr);
#endif

RETCODE SQL_API SQLGetConnectOptionA(SQLHDBC hdbc,
                                     SQLUSMALLINT fOption,
                                     PTR pvParam);

RETCODE SQL_API SQLGetCursorNameA(SQLHSTMT hstmt,
                                  SQLTCHAR * szCursor,
                                  SQLSMALLINT cbCursorMax,
                                  SQLSMALLINT * pcbCursor);

RETCODE SQL_API SQLGetDescFieldA(SQLHDESC DescriptorHandle,
                                 SQLSMALLINT RecNumber,
                                 SQLSMALLINT FieldIdentifier,
                                 SQLPOINTER ValuePtr,
                                 SQLINTEGER BufferLength,
                                 SQLINTEGER *StringLengthPtr);

RETCODE SQL_API SQLGetDescRecA(SQLHDESC DescriptorHandle,
                               SQLSMALLINT RecNumber,
                               SQLTCHAR *Name,
                               SQLSMALLINT BufferLength,
                               SQLSMALLINT *StringLengthPtr,
                               SQLSMALLINT *TypePtr,
                               SQLSMALLINT *SubTypePtr,
                               SQLINTEGER *LengthPtr,
                               SQLSMALLINT *PrecisionPtr,
                               SQLSMALLINT *ScalePtr,
                               SQLSMALLINT *NullablePtr);

RETCODE SQL_API SQLGetDiagFieldA(SQLSMALLINT HandleType,
                                 SQLHANDLE Handle,
                                 SQLSMALLINT RecNumber,
                                 SQLSMALLINT DiagIdentifier,
                                 SQLPOINTER DiagInfo,
                                 SQLSMALLINT BufferLength,
                                 SQLSMALLINT *StringLength);

RETCODE SQL_API SQLGetDiagRecA(SQLSMALLINT HandleType,
                              SQLHANDLE Handle,
                              SQLSMALLINT RecNumber,
                              SQLTCHAR *Sqlstate,
                              SQLINTEGER *NativeErrorPtr,
                              SQLTCHAR *MessageText,
                              SQLSMALLINT BufferLength,
                              SQLSMALLINT *TextLengthPtr);

RETCODE SQL_API SQLGetInfoA(SQLHDBC hdbc,
                            SQLUSMALLINT fInfoType,
                            PTR rgbInfoValue,
                            SQLSMALLINT cbInfoValueMax,
                            SQLSMALLINT * pcbInfoValue);

#ifndef DH_ODBC2X
RETCODE SQL_API SQLGetStmtAttrA(SQLHSTMT StatementHandle,
                                SQLINTEGER Attribute,
                                SQLPOINTER ValuePtr,
                                SQLINTEGER BufferLength,
                                SQLINTEGER *StringLengthptr);
#endif

RETCODE SQL_API SQLGetStmtOptionA(SQLHSTMT hstmt,
                                  SQLUSMALLINT fOption,
                                  PTR pvParam);

RETCODE SQL_API SQLGetTypeInfoA(SQLHSTMT hstmt, SQLSMALLINT fSqlType);

RETCODE SQL_API SQLNativeSqlA(SQLHDBC hdbc,
                              SQLTCHAR * szSqlStrIn,
                              SQLINTEGER cbSqlStrIn,
                              SQLTCHAR * szSqlStr,
                              SQLINTEGER cbSqlStrMax,
                              SQLINTEGER * pcbSqlStr);

RETCODE SQL_API SQLPrepareA(SQLHSTMT hstmt,
                            SQLTCHAR * szSqlStr,
                            SQLINTEGER cbSqlStr);

RETCODE SQL_API SQLPrimaryKeysA(SQLHSTMT hstmt,
                                SQLTCHAR * szTableQualifier,
                                SQLSMALLINT cbTableQualifier,
                                SQLTCHAR * szTableOwner,
                                SQLSMALLINT cbTableOwner,
                                SQLTCHAR * szTableName,
                                SQLSMALLINT cbTableName);

RETCODE SQL_API SQLProcedureColumnsA(SQLHSTMT hstmt,
                                     SQLTCHAR * szProcQualifier,
                                     SQLSMALLINT cbProcQualifier,
                                     SQLTCHAR * szProcOwner,
                                     SQLSMALLINT cbProcOwner,
                                     SQLTCHAR * szProcName,
                                     SQLSMALLINT cbProcName,
                                     SQLTCHAR * szColumnName,
                                     SQLSMALLINT cbColumnName);

RETCODE SQL_API SQLProceduresA(SQLHSTMT hstmt,
                               SQLTCHAR * szProcQualifier,
                               SQLSMALLINT cbProcQualifier,
                               SQLTCHAR * szProcOwner,
                               SQLSMALLINT cbProcOwner,
                               SQLTCHAR * szProcName,
                               SQLSMALLINT cbProcName);

#ifndef DH_ODBC2X
RETCODE SQL_API SQLSetConnectAttrA(SQLHDBC ConnectionHandle,
                                   SQLINTEGER Attribute,
                                   SQLPOINTER ValuePtr,
                                   SQLINTEGER StringLength);
#endif

RETCODE SQL_API SQLSetConnectOptionA(SQLHDBC hdbc,
                                     SQLUSMALLINT fOption,
                                     SQLUINTEGER vParam);

RETCODE SQL_API SQLSetCursorNameA(SQLHSTMT hstmt,
                                  SQLTCHAR * szCursor,
                                  SQLSMALLINT cbCursor);

RETCODE SQL_API SQLSetStmtOptionA(SQLHSTMT hstmt,
                                  SQLUSMALLINT fOption,
                                  SQLUINTEGER vParam);

RETCODE SQL_API SQLSpecialColumnsA(SQLHSTMT hstmt,
                                   SQLUSMALLINT fColType,
                                   SQLTCHAR * szTableQualifier,
                                   SQLSMALLINT cbTableQualifier,
                                   SQLTCHAR * szTableOwner,
                                   SQLSMALLINT cbTableOwner,
                                   SQLTCHAR * szTableName,
                                   SQLSMALLINT cbTableName,
                                   SQLUSMALLINT fScope,
                                   SQLUSMALLINT fNullable);

RETCODE SQL_API SQLStatisticsA(SQLHSTMT hstmt,
                               SQLTCHAR * szTableQualifier,
                               SQLSMALLINT cbTableQualifier,
                               SQLTCHAR * szTableOwner,
                               SQLSMALLINT cbTableOwner,
                               SQLTCHAR * szTableName,
                               SQLSMALLINT cbTableName,
                               SQLUSMALLINT fUnique,
                               SQLUSMALLINT fAccuracy);

RETCODE SQL_API SQLTablesA(SQLHSTMT hstmt,
                           SQLTCHAR * szTableQualifier,
                           SQLSMALLINT cbTableQualifier,
                           SQLTCHAR * szTableOwner,
                           SQLSMALLINT cbTableOwner,
                           SQLTCHAR * szTableName,
                           SQLSMALLINT cbTableName,
                           SQLTCHAR * szTableType,
                           SQLSMALLINT cbTableType);

RETCODE SQL_API SQLTablePrivilegesA(SQLHSTMT hstmt,
                                    SQLTCHAR * szTableQualifier,
                                    SQLSMALLINT cbTableQualifier,
                                    SQLTCHAR * szTableOwner,
                                    SQLSMALLINT cbTableOwner,
                                    SQLTCHAR * szTableName,
                                    SQLSMALLINT cbTableName);

/*
 * Mapping macros for Unicode
 */

#ifndef SQL_NOUNICODEMAP
#ifdef UNICODE

#define SQLBrowseConnect        SQLBrowseConnectW
#define SQLColAttribute         SQLColAttributeW
#define SQLColAttributes        SQLColAttributesW
#define SQLColumnPrivileges     SQLColumnPrivilegesW
#define SQLColumns              SQLColumnsW
#define SQLConnect              SQLConnectW
#define SQLDataSources          SQLDataSourcesW
#define SQLDescribeCol          SQLDescribeColW
#define SQLDriverConnect        SQLDriverConnectW
#define SQLError                SQLErrorW
#define SQLExecDirect           SQLExecDirectW
#define SQLForeignKeys          SQLForeignKeysW
#define SQLGetConnectAttr       SQLGetConnectAttrW
#define SQLGetConnectOption     SQLGetConnectOptionW
#define SQLGetCursorName        SQLGetCursorNameW
#define SQLGetDescField         SQLGetDescFieldW
#define SQLGetDescRec           SQLGetDescRecW
#define SQLGetDiagField         SQLGetDiagFieldW
#define SQLGetDiagRec           SQLGetDiagRecW
#define SQLGetInfo              SQLGetInfoW
#define SQLGetStmtAttr          SQLGetStmtAttrW
#define SQLGetTypeInfo          SQLGetTypeInfoW
#define SQLNativeSql            SQLNativeSqlW
#define SQLPrepare              SQLPrepareW
#define SQLPrimaryKeys          SQLPrimaryKeysW
#define SQLProcedureColumns     SQLProcedureColumnsW
#define SQLProcedures           SQLProceduresW
#define SQLSetConnectAttr       SQLSetConnectAttrW
#define SQLSetConnectOption     SQLSetConnectOptionW
#define SQLSetCursorName        SQLSetCursorNameW
#define SQLSetDescField         SQLSetDescFieldW
#define SQLSetStmtAttr          SQLSetStmtAttrW
#define SQLSpecialColumns       SQLSpecialColumnsW
#define SQLStatistics           SQLStatisticsW
#define SQLTablePrivileges      SQLTablePrivilegesW
#define SQLTables               SQLTablesW

#endif  /* UNICODE */
#endif  /* SQL_NOUNICODEMAP     */

#ifdef __cplusplus
}       /* End of extern "C" { */
#endif

#ifndef _SQLEXT_H_
#include <sqlext.h>
#endif

#endif /* #ifndef _SQLUCODE_H_ */
