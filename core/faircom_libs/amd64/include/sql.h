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
 * For Windows please use the "sql.h" header file supplied by the compiler.
 */

#ifndef _SQL_H_
#define _SQL_H_

#ifndef _SQLTYPES_H_
#include "sqltypes.h"
#endif

/*
 * SQL return values
 */
#define SQL_SUCCESS                0
#define SQL_SUCCESS_WITH_INFO      1
#define SQL_STILL_EXECUTING        2
#define SQL_NEED_DATA              99
#define SQL_NO_DATA                100
#define SQL_ERROR                  (-1)
#define SQL_INVALID_HANDLE         (-2)
#define SQL_SUCCEEDED(rc)          (((rc)&(~1))==0)

/*
 * SQL data type codes
 */
#define SQL_UNKNOWN_TYPE        0
#define SQL_CHAR                1
#define SQL_NUMERIC             2
#define SQL_DECIMAL             3
#define SQL_INTEGER             4
#define SQL_SMALLINT            5
#define SQL_FLOAT               6
#define SQL_REAL                7
#define SQL_DOUBLE              8
#define SQL_DATETIME            9
#define SQL_VARCHAR             12

/*
 * One-parameter shortcuts for date/time data types
 */
#define SQL_TYPE_DATE      91
#define SQL_TYPE_TIME      92
#define SQL_TYPE_TIMESTAMP 93

/*
 * Length/indicator values
 */
#define SQL_NULL_DATA             (-1)
#define SQL_DATA_AT_EXEC          (-2)
#define SQL_MAX_MESSAGE_LENGTH    512
#define SQL_DATE_LEN              10
#define SQL_TIME_LEN              8  /* add P+1 if precision is nonzero */
#define SQL_TIMESTAMP_LEN         19 /* add P+1 if precision is nonzero */

/*
 * Handle type identifiers
 */
#define SQL_HANDLE_ENV             1
#define SQL_HANDLE_DBC             2
#define SQL_HANDLE_STMT            3
#define SQL_HANDLE_DESC            4

/*
 * Flags for null-terminated string
 */
#define SQL_NTS                    (-3)
#define SQL_NTSL                   (-3L)

/*
 * Environment attribute
 */
#define SQL_ATTR_OUTPUT_NTS    10001

/*
 * Connection attributes
 */
#define SQL_ATTR_AUTO_IPD      10001
#define SQL_ATTR_METADATA_ID   10014

/*
 * Statement attributes
 */
#define SQL_ATTR_APP_ROW_DESC       10010
#define SQL_ATTR_APP_PARAM_DESC     10011
#define SQL_ATTR_IMP_ROW_DESC       10012
#define SQL_ATTR_IMP_PARAM_DESC     10013
#define SQL_ATTR_CURSOR_SCROLLABLE  (-1)
#define SQL_ATTR_CURSOR_SENSITIVITY (-2)

/*
 * SQL_ATTR_CURSOR_SCROLLABLE values
 */
#define SQL_NONSCROLLABLE               0
#define SQL_SCROLLABLE                  1

/*
 * Identifiers of fields in the diagnostics area
 */
#define SQL_DIAG_RETURNCODE             1
#define SQL_DIAG_NUMBER                 2
#define SQL_DIAG_ROW_COUNT              3
#define SQL_DIAG_SQLSTATE               4
#define SQL_DIAG_NATIVE                 5
#define SQL_DIAG_MESSAGE_TEXT           6
#define SQL_DIAG_CALL                   7
#define SQL_DIAG_DYNAMIC_FUNCTION       7
#define SQL_DIAG_CLASS_ORIGIN           8
#define SQL_DIAG_SUBCLASS_ORIGIN        9
#define SQL_DIAG_CONNECTION_NAME        10
#define SQL_DIAG_SERVER_NAME            11
#define SQL_DIAG_DYNAMIC_FUNCTION_CODE  12

/*
 * Diagnostics function codes
 */
#define SQL_DIAG_UNKNOWN_STATEMENT      0
#define SQL_DIAG_ALTER_DOMAIN           3
#define SQL_DIAG_ALTER_TABLE            4
#define SQL_DIAG_CREATE_ASSERTION       6
#define SQL_DIAG_CALL                   7
#define SQL_DIAG_CREATE_CHARACTER_SET   8
#define SQL_DIAG_CREATE_COLLATION       10
#define SQL_DIAG_DELETE_WHERE           19
#define SQL_DIAG_CREATE_DOMAIN          23
#define SQL_DIAG_DROP_ASSERTION         24
#define SQL_DIAG_DROP_CHARACTER_SET     25
#define SQL_DIAG_DROP_COLLATION         26
#define SQL_DIAG_DROP_DOMAIN            27
#define SQL_DIAG_DROP_SCHEMA            31
#define SQL_DIAG_DROP_TABLE             32
#define SQL_DIAG_DROP_TRANSLATION       33
#define SQL_DIAG_DROP_VIEW              36
#define SQL_DIAG_DYNAMIC_DELETE_CURSOR  38
#define SQL_DIAG_GRANT                  48
#define SQL_DIAG_INSERT                 50
#define SQL_DIAG_REVOKE                 59
#define SQL_DIAG_CREATE_SCHEMA          64
#define SQL_DIAG_CREATE_TABLE           77
#define SQL_DIAG_CREATE_TRANSLATION     79
#define SQL_DIAG_DYNAMIC_UPDATE_CURSOR  81
#define SQL_DIAG_UPDATE_WHERE           82
#define SQL_DIAG_CREATE_VIEW            84
#define SQL_DIAG_SELECT_CURSOR          85
#define SQL_DIAG_CREATE_INDEX           (-1)
#define SQL_DIAG_DROP_INDEX             (-2)

/*
 * Statement attribute values for cursor sensitivity
 */
#define SQL_UNSPECIFIED     0
#define SQL_INSENSITIVE     1
#define SQL_SENSITIVE       2

/*
 * GetTypeInfo() request for all data types
 */
#define SQL_ALL_TYPES       0

/*
 * Default conversion code for SQLBindCol(), SQLBindParam() and SQLGetData()
 */
#define SQL_DEFAULT        99

/*
 * SQLGetData() code indicating that the application row descriptor
 * specifies the data type
 */
#define SQL_ARD_TYPE                    (-99)
#define DHARMADRIVER_NO_DATA_LEFT       (-100)

/*
 * SQL date/time type subcodes
 */
#if (ODBCVER >= 0x0300)
#define SQL_CODE_DATE       1
#define SQL_CODE_TIME       2
#define SQL_CODE_TIMESTAMP  3
#endif

/*
 * CLI option values
 */
#if (ODBCVER >= 0x0300)
#define SQL_FALSE           0
#define SQL_TRUE            1
#endif

/*
 * Values of NULLABLE field in descriptor
 */
#define SQL_NO_NULLS        0
#define SQL_NULLABLE        1

/*
 * Value returned by SQLGetTypeInfo() to denote that it is
 * not known whether or not a data type supports null values.
 */
#define SQL_NULLABLE_UNKNOWN  2

/*
 * Values returned by SQLGetTypeInfo() to show WHERE clause
 * supported
 */
#define SQL_PRED_NONE     0
#define SQL_PRED_CHAR     1
#define SQL_PRED_BASIC    2

/*
 * Values of UNNAMED field in descriptor
 */
#define SQL_NAMED           0
#define SQL_UNNAMED         1

/*
 * Values of ALLOC_TYPE field in descriptor
 */
#define SQL_DESC_ALLOC_AUTO 1
#define SQL_DESC_ALLOC_USER 2

/*
 * FreeStmt() options
 */
#define SQL_CLOSE           0
#define SQL_DROP            1
#define SQL_UNBIND          2
#define SQL_RESET_PARAMS    3

/*
 * Codes used for FetchOrientation in SQLFetchScroll(),
 * and in SQLDataSources()
*/
#define SQL_FETCH_NEXT      1
#define SQL_FETCH_FIRST     2

/*
 * Other codes used for FetchOrientation in SQLFetchScroll()
 */
#define SQL_FETCH_LAST      3
#define SQL_FETCH_PRIOR     4
#define SQL_FETCH_ABSOLUTE  5
#define SQL_FETCH_RELATIVE  6

/*
 * SQLEndTran() options
 */
#define SQL_COMMIT          0
#define SQL_ROLLBACK        1

/*
 * null handles returned by SQLAllocHandle()
 */
#define SQL_NULL_HENV       0
#define SQL_NULL_HDBC       0
#define SQL_NULL_HSTMT      0
#define SQL_NULL_HDESC      0
#define SQL_NULL_DESC       0

/*
 * null handle used in place of parent handle when allocating HENV
 */
#define SQL_NULL_HANDLE     0L

/*
 * Values that may appear in the result set of SQLSpecialColumns()
 */
#define SQL_SCOPE_CURROW        0
#define SQL_SCOPE_TRANSACTION   1
#define SQL_SCOPE_SESSION       2

#define SQL_PC_UNKNOWN          0
#define SQL_PC_NON_PSEUDO       1
#define SQL_PC_PSEUDO           2

/*
 * Reserved value for the IdentifierType argument of SQLSpecialColumns()
 */
#define SQL_ROW_IDENTIFIER  1

/*
 * Reserved values for UNIQUE argument of SQLStatistics()
 */
#define SQL_INDEX_UNIQUE    0
#define SQL_INDEX_ALL       1

/*
 * Values that may appear in the result set of SQLStatistics()
 */
#define SQL_INDEX_CLUSTERED 1
#define SQL_INDEX_HASHED    2
#define SQL_INDEX_OTHER     3

/*
 * SQLGetFunctions() values to identify ODBC APIs
 */
#define SQL_API_SQLALLOCCONNECT         1
#define SQL_API_SQLALLOCENV             2
#define SQL_API_SQLALLOCSTMT            3
#define SQL_API_SQLBINDCOL              4
#define SQL_API_SQLCANCEL               5
#define SQL_API_SQLCOLATTRIBUTE         6
#define SQL_API_SQLCONNECT              7
#define SQL_API_SQLDESCRIBECOL          8
#define SQL_API_SQLDISCONNECT           9
#define SQL_API_SQLERROR               10
#define SQL_API_SQLEXECDIRECT          11
#define SQL_API_SQLEXECUTE             12
#define SQL_API_SQLFETCH               13
#define SQL_API_SQLFREECONNECT         14
#define SQL_API_SQLFREEENV             15
#define SQL_API_SQLFREESTMT            16
#define SQL_API_SQLGETCURSORNAME       17
#define SQL_API_SQLNUMRESULTCOLS       18
#define SQL_API_SQLPREPARE             19
#define SQL_API_SQLROWCOUNT            20
#define SQL_API_SQLSETCURSORNAME       21
#define SQL_API_SQLSETPARAM            22
#define SQL_API_SQLTRANSACT            23
#define SQL_API_SQLCOLUMNS             40
#define SQL_API_SQLGETCONNECTOPTION    42
#define SQL_API_SQLGETDATA             43
#define SQL_API_SQLGETFUNCTIONS        44
#define SQL_API_SQLGETINFO             45
#define SQL_API_SQLGETSTMTOPTION       46
#define SQL_API_SQLGETTYPEINFO         47
#define SQL_API_SQLPARAMDATA           48
#define SQL_API_SQLPUTDATA             49
#define SQL_API_SQLSETCONNECTOPTION    50
#define SQL_API_SQLSETSTMTOPTION       51
#define SQL_API_SQLSPECIALCOLUMNS      52
#define SQL_API_SQLSTATISTICS          53
#define SQL_API_SQLTABLES              54
#define SQL_API_SQLDATASOURCES         57
#define SQL_API_SQLALLOCHANDLE       1001
#define SQL_API_SQLBINDPARAM         1002
#define SQL_API_SQLCLOSECURSOR       1003
#define SQL_API_SQLCOPYDESC          1004
#define SQL_API_SQLENDTRAN           1005
#define SQL_API_SQLFREEHANDLE        1006
#define SQL_API_SQLGETCONNECTATTR    1007
#define SQL_API_SQLGETDESCFIELD      1008
#define SQL_API_SQLGETDESCREC        1009
#define SQL_API_SQLGETDIAGFIELD      1010
#define SQL_API_SQLGETDIAGREC        1011
#define SQL_API_SQLGETENVATTR        1012
#define SQL_API_SQLGETSTMTATTR       1014
#define SQL_API_SQLSETCONNECTATTR    1016
#define SQL_API_SQLSETDESCFIELD      1017
#define SQL_API_SQLSETDESCREC        1018
#define SQL_API_SQLSETENVATTR        1019
#define SQL_API_SQLSETSTMTATTR       1020
#define SQL_API_SQLFETCHSCROLL       1021

/*
 * Information requested by SQLGetInfo()
 */
#define SQL_MAX_DRIVER_CONNECTIONS           0
#define SQL_MAX_CONCURRENT_ACTIVITIES        1
#define SQL_DATA_SOURCE_NAME                 2
#define SQL_FETCH_DIRECTION                  8
#define SQL_SERVER_NAME                     13
#define SQL_SEARCH_PATTERN_ESCAPE           14
#define SQL_DBMS_NAME                       17
#define SQL_DBMS_VER                        18
#define SQL_ACCESSIBLE_TABLES               19
#define SQL_ACCESSIBLE_PROCEDURES           20
#define SQL_CURSOR_COMMIT_BEHAVIOR          23
#define SQL_DATA_SOURCE_READ_ONLY           25
#define SQL_DEFAULT_TXN_ISOLATION           26
#define SQL_IDENTIFIER_CASE                 28
#define SQL_IDENTIFIER_QUOTE_CHAR           29
#define SQL_MAX_COLUMN_NAME_LEN             30
#define SQL_MAX_CURSOR_NAME_LEN             31
#define SQL_MAX_SCHEMA_NAME_LEN             32
#define SQL_MAX_CATALOG_NAME_LEN            34
#define SQL_MAX_TABLE_NAME_LEN              35
#define SQL_SCROLL_CONCURRENCY              43
#define SQL_TXN_CAPABLE                     46
#define SQL_USER_NAME                       47
#define SQL_TXN_ISOLATION_OPTION            72
#define SQL_INTEGRITY                       73
#define SQL_GETDATA_EXTENSIONS              81
#define SQL_NULL_COLLATION                  85
#define SQL_ALTER_TABLE                     86
#define SQL_ORDER_BY_COLUMNS_IN_SELECT      90
#define SQL_SPECIAL_CHARACTERS              94
#define SQL_MAX_COLUMNS_IN_GROUP_BY         97
#define SQL_MAX_COLUMNS_IN_INDEX            98
#define SQL_MAX_COLUMNS_IN_ORDER_BY         99
#define SQL_MAX_COLUMNS_IN_SELECT          100
#define SQL_MAX_COLUMNS_IN_TABLE           101
#define SQL_MAX_INDEX_SIZE                 102
#define SQL_MAX_ROW_SIZE                   104
#define SQL_MAX_STATEMENT_LEN              105
#define SQL_MAX_TABLES_IN_SELECT           106
#define SQL_MAX_USER_NAME_LEN              107
#define SQL_OJ_CAPABILITIES                115
#define SQL_XOPEN_CLI_YEAR               10000
#define SQL_CURSOR_SENSITIVITY           10001
#define SQL_DESCRIBE_PARAMETER           10002
#define SQL_CATALOG_NAME                 10003
#define SQL_COLLATION_SEQ                10004
#define SQL_MAX_IDENTIFIER_LEN           10005

#define SQL_MAXIMUM_IDENTIFIER_LENGTH           SQL_MAX_IDENTIFIER_LEN
#define SQL_MAXIMUM_DRIVER_CONNECTIONS          SQL_MAX_DRIVER_CONNECTIONS
#define SQL_MAXIMUM_CONCURRENT_ACTIVITIES       SQL_MAX_CONCURRENT_ACTIVITIES
#define SQL_MAXIMUM_COLUMN_NAME_LENGTH          SQL_MAX_COLUMN_NAME_LEN
#define SQL_MAXIMUM_CURSOR_NAME_LENGTH          SQL_MAX_CURSOR_NAME_LEN
#define SQL_MAXIMUM_SCHEMA_NAME_LENGTH          SQL_MAX_SCHEMA_NAME_LEN
#define SQL_MAXIMUM_CATALOG_NAME_LENGTH         SQL_MAX_CATALOG_NAME_LEN
#define SQL_TRANSACTION_CAPABLE                 SQL_TXN_CAPABLE
#define SQL_TRANSACTION_ISOLATION_OPTION        SQL_TXN_ISOLATION_OPTION
#define SQL_MAXIMUM_COLUMNS_IN_GROUP_BY         SQL_MAX_COLUMNS_IN_GROUP_BY
#define SQL_MAXIMUM_COLUMNS_IN_INDEX            SQL_MAX_COLUMNS_IN_INDEX
#define SQL_MAXIMUM_COLUMNS_IN_ORDER_BY         SQL_MAX_COLUMNS_IN_ORDER_BY
#define SQL_MAXIMUM_COLUMNS_IN_SELECT           SQL_MAX_COLUMNS_IN_SELECT
#define SQL_MAXIMUM_INDEX_SIZE                  SQL_MAX_INDEX_SIZE
#define SQL_MAXIMUM_ROW_SIZE                    SQL_MAX_ROW_SIZE
#define SQL_MAXIMUM_STATEMENT_LENGTH            SQL_MAX_STATEMENT_LEN
#define SQL_MAXIMUM_TABLES_IN_SELECT            SQL_MAX_TABLES_IN_SELECT
#define SQL_MAXIMUM_USER_NAME_LENGTH            SQL_MAX_USER_NAME_LEN
#define SQL_OUTER_JOIN_CAPABILITIES             SQL_OJ_CAPABILITIES

/*
 * SQL_ALTER_TABLE bitmasks
 */
#define SQL_AT_ADD_COLUMN                       0x00000001L
#define SQL_AT_DROP_COLUMN                      0x00000002L
#define SQL_AT_ADD_CONSTRAINT                   0x00000008L

/*
 * SQL_ASYNC_MODE values
 */
#define SQL_AM_NONE                         0
#define SQL_AM_CONNECTION                   1
#define SQL_AM_STATEMENT                    2

/*
 * SQL_CURSOR_COMMIT_BEHAVIOR values
 */
#define SQL_CB_DELETE                       0
#define SQL_CB_CLOSE                        1
#define SQL_CB_PRESERVE                     2

/*
 * SQL_FETCH_DIRECTION bitmasks
 */
#define SQL_FD_FETCH_NEXT                   0x00000001L
#define SQL_FD_FETCH_FIRST                  0x00000002L
#define SQL_FD_FETCH_LAST                   0x00000004L
#define SQL_FD_FETCH_PRIOR                  0x00000008L
#define SQL_FD_FETCH_ABSOLUTE               0x00000010L
#define SQL_FD_FETCH_RELATIVE               0x00000020L

/*
 * SQL_IDENTIFIER_CASE values
 */
#define SQL_IC_UPPER                        1
#define SQL_IC_LOWER                        2
#define SQL_IC_SENSITIVE                    3
#define SQL_IC_MIXED                        4

/*
 * SQL_OJ_CAPABILITIES bitmasks
 */
#define SQL_OJ_LEFT                         0x00000001L
#define SQL_OJ_RIGHT                        0x00000002L
#define SQL_OJ_FULL                         0x00000004L
#define SQL_OJ_NESTED                       0x00000008L
#define SQL_OJ_NOT_ORDERED                  0x00000010L
#define SQL_OJ_INNER                        0x00000020L
#define SQL_OJ_ALL_COMPARISON_OPS           0x00000040L

/*
 * SQL_SCROLL_CONCURRENCY bitmasks
 */
#define SQL_SCCO_READ_ONLY                  0x00000001L
#define SQL_SCCO_LOCK                       0x00000002L
#define SQL_SCCO_OPT_ROWVER                 0x00000004L
#define SQL_SCCO_OPT_VALUES                 0x00000008L

/*
 * SQL_TXN_CAPABLE values
 */
#define SQL_TC_NONE                         0
#define SQL_TC_DML                          1
#define SQL_TC_ALL                          2
#define SQL_TC_DDL_COMMIT                   3
#define SQL_TC_DDL_IGNORE                   4

/*
 * SQL_TXN_ISOLATION_OPTION bitmasks
 */
#define SQL_TXN_READ_UNCOMMITTED                0x00000001L
#define SQL_TRANSACTION_READ_UNCOMMITTED        SQL_TXN_READ_UNCOMMITTED
#define SQL_TXN_READ_COMMITTED                  0x00000002L
#define SQL_TRANSACTION_READ_COMMITTED          SQL_TXN_READ_COMMITTED
#define SQL_TXN_REPEATABLE_READ                 0x00000004L
#define SQL_TRANSACTION_REPEATABLE_READ         SQL_TXN_REPEATABLE_READ
#define SQL_TXN_SERIALIZABLE                    0x00000008L
#define SQL_TRANSACTION_SERIALIZABLE            SQL_TXN_SERIALIZABLE

/*
 * SQL_NULL_COLLATION values
 */
#define SQL_NC_HIGH                         0
#define SQL_NC_LOW                          1

#ifdef __cplusplus
extern "C" {
#endif

RETCODE SQL_API SQLAllocConnect(HENV henv, SQLHDBC * phdbc);

RETCODE SQL_API SQLAllocEnv(SQLHENV *phenv);

#ifndef DH_ODBC2X
RETCODE SQL_API SQLAllocHandle(SQLSMALLINT HandleType,
                               SQLHANDLE InputHandle,
                               SQLHANDLE *OutputHandle);
#endif

RETCODE SQL_API SQLAllocStmt(SQLHDBC hdbc, SQLHSTMT * phstmt);

RETCODE SQL_API SQLBindCol(SQLHSTMT hstmt,
                           SQLUSMALLINT icol,
                           SQLSMALLINT fCType,
                           PTR rgbValue,
                           SQLLEN cbValueMax,
                           SQLLEN * pcbValue);

RETCODE SQL_API SQLBindParam(SQLHSTMT hstmt,
                                 SQLUSMALLINT ipar,
                                 SQLSMALLINT fParamType,
                                 SQLSMALLINT fCType,
                                 SQLULEN Precision,
                                 SQLSMALLINT ibScale,
                                 PTR rgbValue,
                                 SQLLEN * pcbValue);

RETCODE SQL_API SQLCancel(SQLHSTMT hstmt);

#ifndef DH_ODBC2X
RETCODE SQL_API SQLCloseCursor(SQLHSTMT hstmt);

RETCODE SQL_API SQLColAttribute(SQLHSTMT StatementHandle,
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
                                SQLLEN *  NumericAttribute);
#endif



RETCODE SQL_API SQLColumns(SQLHSTMT hstmt,
                           SQLTCHAR * szTableQualifier,
                           SQLSMALLINT cbTableQualifier,
                           SQLTCHAR * szTableOwner,
                           SQLSMALLINT cbTableOwner,
                           SQLTCHAR * szTableName,
                           SQLSMALLINT cbTableName,
                           SQLTCHAR * szColumnName,
                           SQLSMALLINT cbColumnName);

RETCODE SQL_API SQLConnect(SQLHDBC hdbc,
                           SQLTCHAR * szDSN,
                           SQLSMALLINT cbDSN,
                           SQLTCHAR * szUID,
                           SQLSMALLINT cbUID,
                           SQLTCHAR * szAuthStr,
                           SQLSMALLINT cbAuthStr);

RETCODE SQL_API SQLCopyDesc(SQLHDESC SourceDescHandle,
                            SQLHDESC TargetDescHandle);

RETCODE SQL_API SQLDataSources(SQLHENV EnvironmentHandle,
                               SQLUSMALLINT Direction,
                               SQLCHAR * ServerName,
                               SQLSMALLINT BufferLength1,
                               SQLSMALLINT * NameLength1,
                               SQLCHAR * Description,
                               SQLSMALLINT BufferLength2,
                               SQLSMALLINT * NameLength2);

RETCODE SQL_API SQLDescribeCol(SQLHSTMT hstmt,
                               SQLUSMALLINT icol,
                               SQLTCHAR * szColName,
                               SQLSMALLINT cbColNameMax,
                               SQLSMALLINT * pcbColName,
                               SQLSMALLINT * pfSqlType,
                               SQLULEN * pcbColDef,
                               SQLSMALLINT * pibScale,
                               SQLSMALLINT * pfNullable);

RETCODE SQL_API SQLDisconnect(SQLHDBC hdbc);

#ifndef DH_ODBC2X
RETCODE SQL_API SQLEndTran(SQLSMALLINT HandleType,
                           SQLHANDLE Handle,
                           SQLSMALLINT CompletionType);
#endif

RETCODE SQL_API SQLError(HENV henv,
                         SQLHDBC hdbc,
                         SQLHSTMT hstmt,
                         SQLTCHAR * szSqlState,
                         SQLINTEGER * pfNativeError,
                         SQLTCHAR * szErrorMsg,
                         SQLSMALLINT cbErrorMsgMax,
                         SQLSMALLINT * pcbErrorMsg);

RETCODE SQL_API SQLExecDirect(SQLHSTMT hstmt,
                              SQLTCHAR * szSqlStr,
                              SQLINTEGER cbSqlStr);

RETCODE SQL_API SQLExecute(SQLHSTMT hstmt);

RETCODE SQL_API SQLFetch(SQLHSTMT hstmt);

RETCODE SQL_API SQLFetchScroll(SQLHSTMT hstmt,
                               SQLSMALLINT FetchOrientation,
                               SQLLEN FetchOffset);

RETCODE SQL_API SQLFreeConnect(SQLHDBC hdbc);

RETCODE SQL_API SQLFreeEnv(HENV henv);

#ifndef DH_ODBC2X
RETCODE SQL_API SQLFreeHandle(SQLSMALLINT HandleType, SQLHANDLE Handle);
#endif

RETCODE SQL_API SQLFreeStmt(SQLHSTMT hstmt, SQLUSMALLINT fOption);

#ifndef DH_ODBC2X
RETCODE SQL_API SQLGetConnectAttr(SQLHDBC ConnectionHandle,
                                  SQLINTEGER Attribute,
                                  SQLPOINTER ValuePtr,
                                  SQLINTEGER BufferLength,
                                  SQLINTEGER *StringLengthptr);
#endif

RETCODE SQL_API SQLGetConnectOption(SQLHDBC hdbc,
                                    SQLUSMALLINT fOption,
                                    PTR pvParam);

RETCODE SQL_API SQLGetCursorName(SQLHSTMT hstmt,
                                 SQLTCHAR * szCursor,
                                 SQLSMALLINT cbCursorMax,
                                 SQLSMALLINT * pcbCursor);

RETCODE SQL_API SQLGetData(SQLHSTMT hstmt,
                           SQLUSMALLINT icol,
                           SQLSMALLINT fCType,
                           PTR rgbValue,
                           SQLLEN cbValueMax,
                           SQLLEN * pcbValue);

RETCODE SQL_API SQLGetDescField(SQLHDESC DescriptorHandle,
                                SQLSMALLINT RecNumber,
                                SQLSMALLINT FieldIdentifier,
                                SQLPOINTER ValuePtr,
                                SQLINTEGER BufferLength,
                                SQLINTEGER *StringLengthPtr);

RETCODE SQL_API SQLGetDescRec(SQLHDESC DescriptorHandle,
                              SQLSMALLINT RecNumber,
                              SQLTCHAR *Name,
                              SQLSMALLINT BufferLength,
                              SQLSMALLINT *StringLengthPtr,
                              SQLSMALLINT *TypePtr,
                              SQLSMALLINT *SubTypePtr,
                              SQLLEN *LengthPtr,
                              SQLSMALLINT *PrecisionPtr,
                              SQLSMALLINT *ScalePtr,
                              SQLSMALLINT *NullablePtr);

RETCODE SQL_API SQLGetDiagField(SQLSMALLINT HandleType,
                                SQLHANDLE Handle,
                                SQLSMALLINT RecNumber,
                                SQLSMALLINT DiagIdentifier,
                                SQLPOINTER DiagInfo,
                                SQLSMALLINT BufferLength,
                                SQLSMALLINT *StringLength);

RETCODE SQL_API SQLGetDiagRec(SQLSMALLINT HandleType,
                              SQLHANDLE Handle,
                              SQLSMALLINT RecNumber,
                              SQLTCHAR *Sqlstate,
                              SQLINTEGER *NativeErrorPtr,
                              SQLTCHAR *MessageText,
                              SQLSMALLINT BufferLength,
                              SQLSMALLINT *TextLengthPtr);

RETCODE SQL_API SQLGetEnvAttr(SQLHENV EnvironmentHandle,
                              SQLINTEGER Attribute,
                              SQLPOINTER Value,
                              SQLINTEGER BufferLength,
                              SQLINTEGER *StringLength);

RETCODE SQL_API SQLGetFunctions(SQLHDBC hdbc,
                                SQLUSMALLINT fFunction,
                                SQLUSMALLINT * pfExists);

RETCODE SQL_API SQLGetInfo(SQLHDBC hdbc,
                           SQLUSMALLINT fInfoType,
                           PTR rgbInfoValue,
                           SQLSMALLINT cbInfoValueMax,
                           SQLSMALLINT * pcbInfoValue);

#ifndef DH_ODBC2X
RETCODE SQL_API SQLGetStmtAttr(SQLHSTMT StatementHandle,
                               SQLINTEGER Attribute,
                               SQLPOINTER ValuePtr,
                               SQLINTEGER BufferLength,
                               SQLINTEGER *StringLengthptr);
#endif

RETCODE SQL_API SQLGetStmtOption(SQLHSTMT hstmt,
                                 SQLUSMALLINT fOption,
                                 PTR pvParam);

RETCODE SQL_API SQLGetTypeInfo(SQLHSTMT hstmt, SQLSMALLINT fSqlType);

RETCODE SQL_API SQLNumResultCols(SQLHSTMT hstmt, SQLSMALLINT * pccol);

RETCODE SQL_API SQLParamData(SQLHSTMT hstmt, PTR * prgbValue);

RETCODE SQL_API SQLPrepare(SQLHSTMT hstmt,
                           SQLTCHAR * szSqlStr,
                           SQLINTEGER cbSqlStr);

RETCODE SQL_API SQLPutData(SQLHSTMT hstmt, PTR rgbValue, SQLLEN cbValue);

RETCODE SQL_API SQLRowCount(SQLHSTMT hstmt, SQLLEN * pcrow);

#ifndef DH_ODBC2X
RETCODE SQL_API SQLSetConnectAttr(SQLHDBC ConnectionHandle,
                                  SQLINTEGER Attribute,
                                  SQLPOINTER ValuePtr,
                                  SQLINTEGER StringLength);
#endif

RETCODE SQL_API SQLSetConnectOption(SQLHDBC hdbc,
                                    SQLUSMALLINT fOption,
                                    SQLULEN vParam);

RETCODE SQL_API SQLSetCursorName(SQLHSTMT hstmt,
                                 SQLTCHAR * szCursor,
                                 SQLSMALLINT cbCursor);

RETCODE SQL_API SQLSetDescField(SQLHDESC DescriptorHandle,
                                SQLSMALLINT RecNumber,
                                SQLSMALLINT FieldIdentifier,
                                SQLPOINTER ValuePtr,
                                SQLINTEGER BufferLength);

RETCODE SQL_API SQLSetDescRec(SQLHDESC DescriptorHandle,
                              SQLSMALLINT RecNumber,
                              SQLSMALLINT Type,
                              SQLSMALLINT SubType,
                              SQLLEN Length,
                              SQLSMALLINT Precision,
                              SQLSMALLINT Scale,
                              SQLPOINTER DataPtr,
                              SQLLEN *StringLengthPtr,
                              SQLLEN *IndicatorPtr);

#ifndef DH_ODBC2X
RETCODE SQL_API SQLSetEnvAttr(SQLHENV EnvironmentHandle,
                              SQLINTEGER Attribute,
                              SQLPOINTER Value,
                              SQLINTEGER StringLength);
#endif

RETCODE SQL_API SQLSetParam(SQLHSTMT hstmt,
                            SQLUSMALLINT ipar,
                            SQLSMALLINT fCType,
                            SQLSMALLINT fSqlType,
                            SQLULEN cbColDef,
                            SQLSMALLINT ibScale,
                            PTR rgbValue,
                            SQLLEN * pcbValue);

#ifndef DH_ODBC2X
RETCODE SQL_API SQLSetStmtAttr(SQLHSTMT StatementHandle,
                               SQLINTEGER Attribute,
                               SQLPOINTER ValuePtr,
                               SQLINTEGER StringLength);
#endif

RETCODE SQL_API SQLSetStmtOption(SQLHSTMT hstmt,
                                 SQLUSMALLINT fOption,
                                 SQLULEN vParam);

RETCODE SQL_API SQLSpecialColumns(SQLHSTMT hstmt,
                                  SQLUSMALLINT fColType,
                                  SQLTCHAR * szTableQualifier,
                                  SQLSMALLINT cbTableQualifier,
                                  SQLTCHAR * szTableOwner,
                                  SQLSMALLINT cbTableOwner,
                                  SQLTCHAR * szTableName,
                                  SQLSMALLINT cbTableName,
                                  SQLUSMALLINT fScope,
                                  SQLUSMALLINT fNullable);

RETCODE SQL_API SQLStatistics(SQLHSTMT hstmt,
                              SQLTCHAR * szTableQualifier,
                              SQLSMALLINT cbTableQualifier,
                              SQLTCHAR * szTableOwner,
                              SQLSMALLINT cbTableOwner,
                              SQLTCHAR * szTableName,
                              SQLSMALLINT cbTableName,
                              SQLUSMALLINT fUnique,
                              SQLUSMALLINT fAccuracy);

RETCODE SQL_API SQLTables(SQLHSTMT hstmt,
                          SQLTCHAR * szTableQualifier,
                          SQLSMALLINT cbTableQualifier,
                          SQLTCHAR * szTableOwner,
                          SQLSMALLINT cbTableOwner,
                          SQLTCHAR * szTableName,
                          SQLSMALLINT cbTableName,
                          SQLTCHAR * szTableType,
                          SQLSMALLINT cbTableType);

RETCODE SQL_API SQLTransact(HENV henv,
                            SQLHDBC hdbc,
                            SQLUSMALLINT fType);

#ifdef __cplusplus
}       /* End of extern "C" { */
#endif
#endif  /* #ifndef _SQL_H_ */
