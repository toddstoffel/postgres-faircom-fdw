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
 * For Windows please use the "sqlext.h" header file supplied by the compiler.
 */

#ifndef _SQLEXT_H_
#define _SQLEXT_H_

#ifndef _SQL_H_
#include "sql.h"
#endif

#ifdef __cplusplus
extern "C" {
#endif

/*
 * SQL extended datatypes
 */
#define SQL_DATE                                9
#define SQL_INTERVAL                            10
#define SQL_TIME                                10
#define SQL_TIMESTAMP                           11
#define SQL_LONGVARCHAR                         (-1)
#define SQL_BINARY                              (-2)
#define SQL_VARBINARY                           (-3)
#define SQL_LONGVARBINARY                       (-4)
#define SQL_BIGINT                              (-5)
#define SQL_TINYINT                             (-6)
#define SQL_BIT                                 (-7)

/*
 * C datatype to SQL datatype mapping
 */
#define SQL_SIGNED_OFFSET                       (-20)
#define SQL_UNSIGNED_OFFSET                     (-22)

#define SQL_TYPE_NULL                           0
#define SQL_C_CHAR                              SQL_CHAR
#define SQL_C_LONG                              SQL_INTEGER
#define SQL_C_SHORT                             SQL_SMALLINT
#define SQL_C_FLOAT                             SQL_REAL
#define SQL_C_DOUBLE                            SQL_DOUBLE
#define SQL_C_NUMERIC                           SQL_NUMERIC
#define SQL_C_DATE                              SQL_DATE
#define SQL_C_TIME                              SQL_TIME
#define SQL_C_TIMESTAMP                         SQL_TIMESTAMP
#define SQL_C_TYPE_DATE                         SQL_TYPE_DATE
#define SQL_C_TYPE_TIME                         SQL_TYPE_TIME
#define SQL_C_TYPE_TIMESTAMP                    SQL_TYPE_TIMESTAMP
#define SQL_C_INTERVAL_YEAR                     SQL_INTERVAL_YEAR
#define SQL_C_INTERVAL_MONTH                    SQL_INTERVAL_MONTH
#define SQL_C_INTERVAL_DAY                      SQL_INTERVAL_DAY
#define SQL_C_INTERVAL_HOUR                     SQL_INTERVAL_HOUR
#define SQL_C_INTERVAL_MINUTE                   SQL_INTERVAL_MINUTE
#define SQL_C_INTERVAL_SECOND                   SQL_INTERVAL_SECOND
#define SQL_C_INTERVAL_YEAR_TO_MONTH            SQL_INTERVAL_YEAR_TO_MONTH
#define SQL_C_INTERVAL_DAY_TO_HOUR              SQL_INTERVAL_DAY_TO_HOUR
#define SQL_C_INTERVAL_DAY_TO_MINUTE            SQL_INTERVAL_DAY_TO_MINUTE
#define SQL_C_INTERVAL_DAY_TO_SECOND            SQL_INTERVAL_DAY_TO_SECOND
#define SQL_C_INTERVAL_HOUR_TO_MINUTE           SQL_INTERVAL_HOUR_TO_MINUTE
#define SQL_C_INTERVAL_HOUR_TO_SECOND           SQL_INTERVAL_HOUR_TO_SECOND
#define SQL_C_INTERVAL_MINUTE_TO_SECOND         SQL_INTERVAL_MINUTE_TO_SECOND
#define SQL_C_BINARY                            SQL_BINARY
#define SQL_C_BIT                               SQL_BIT
#define SQL_C_SBIGINT                           (SQL_BIGINT+SQL_SIGNED_OFFSET)
#define SQL_C_UBIGINT                           (SQL_BIGINT+SQL_UNSIGNED_OFFSET)
#define SQL_C_TINYINT                           SQL_TINYINT
#define SQL_C_SLONG                             (SQL_C_LONG+SQL_SIGNED_OFFSET)
#define SQL_C_SSHORT                            (SQL_C_SHORT+SQL_SIGNED_OFFSET)
#define SQL_C_STINYINT                          (SQL_TINYINT+SQL_SIGNED_OFFSET)
#define SQL_C_ULONG                             (SQL_C_LONG+SQL_UNSIGNED_OFFSET)
#define SQL_C_USHORT                            (SQL_C_SHORT+SQL_UNSIGNED_OFFSET)
#define SQL_C_UTINYINT                          (SQL_TINYINT+SQL_UNSIGNED_OFFSET)
#define SQL_C_DEFAULT                           99

/*
 * Interval code
 */
#define SQL_CODE_YEAR                           1
#define SQL_CODE_MONTH                          2
#define SQL_CODE_DAY                            3
#define SQL_CODE_HOUR                           4
#define SQL_CODE_MINUTE                         5
#define SQL_CODE_SECOND                         6
#define SQL_CODE_YEAR_TO_MONTH                  7
#define SQL_CODE_DAY_TO_HOUR                    8
#define SQL_CODE_DAY_TO_MINUTE                  9
#define SQL_CODE_DAY_TO_SECOND                  10
#define SQL_CODE_HOUR_TO_MINUTE                 11
#define SQL_CODE_HOUR_TO_SECOND                 12
#define SQL_CODE_MINUTE_TO_SECOND               13

#define SQL_INTERVAL_YEAR                       (100 + SQL_CODE_YEAR)
#define SQL_INTERVAL_MONTH                      (100 + SQL_CODE_MONTH)
#define SQL_INTERVAL_DAY                        (100 + SQL_CODE_DAY)
#define SQL_INTERVAL_HOUR                       (100 + SQL_CODE_HOUR)
#define SQL_INTERVAL_MINUTE                     (100 + SQL_CODE_MINUTE)
#define SQL_INTERVAL_SECOND                     (100 + SQL_CODE_SECOND)
#define SQL_INTERVAL_YEAR_TO_MONTH              (100 + SQL_CODE_YEAR_TO_MONTH)
#define SQL_INTERVAL_DAY_TO_HOUR                (100 + SQL_CODE_DAY_TO_HOUR)
#define SQL_INTERVAL_DAY_TO_MINUTE              (100 + SQL_CODE_DAY_TO_MINUTE)
#define SQL_INTERVAL_DAY_TO_SECOND              (100 + SQL_CODE_DAY_TO_SECOND)
#define SQL_INTERVAL_HOUR_TO_MINUTE             (100 + SQL_CODE_HOUR_TO_MINUTE)
#define SQL_INTERVAL_HOUR_TO_SECOND             (100 + SQL_CODE_HOUR_TO_SECOND)
#define SQL_INTERVAL_MINUTE_TO_SECOND           (100 + SQL_CODE_MINUTE_TO_SECOND)

#define SQL_UNICODE                             (-95)
#define SQL_UNICODE_VARCHAR                     (-96)
#define SQL_UNICODE_LONGVARCHAR                 (-97)
#define SQL_UNICODE_CHAR                        SQL_UNICODE

/*
 * SQLGetFunctions: additional values for fFunction to represent functions that
 * are not in the X/Open spec.
 */
#define SQL_API_ALL_FUNCTIONS                   0
#define SQL_API_SQLCOLATTRIBUTES                6
#define SQL_API_SQLBULKOPERATIONS               24
#define SQL_API_SQLDRIVERCONNECT                41
#define SQL_API_SQLBROWSECONNECT                55
#define SQL_API_SQLCOLUMNPRIVILEGES             56
#define SQL_API_SQLDESCRIBEPARAM                58
#define SQL_API_SQLEXTENDEDFETCH                59
#define SQL_API_SQLFOREIGNKEYS                  60
#define SQL_API_SQLMORERESULTS                  61
#define SQL_API_SQLNATIVESQL                    62
#define SQL_API_SQLNUMPARAMS                    63
#define SQL_API_SQLPARAMOPTIONS                 64
#define SQL_API_SQLPRIMARYKEYS                  65
#define SQL_API_SQLPROCEDURECOLUMNS             66
#define SQL_API_SQLPROCEDURES                   67
#define SQL_API_SQLSETPOS                       68
#define SQL_API_SQLSETSCROLLOPTIONS             69
#define SQL_API_SQLTABLEPRIVILEGES              70
#define SQL_API_SQLDRIVERS                      71
#define SQL_API_SQLBINDPARAMETER                72
#define SQL_API_SQLALLOCHANDLESTD               73
#define SQL_API_ODBC3_ALL_FUNCTIONS             999
#define SQL_API_ODBC3_ALL_FUNCTIONS_SIZE        250

#define SQL_FUNC_EXISTS(pfExists, uwAPI) \
                        ((*(((UWORD*) (pfExists)) + ((uwAPI) >> 4)) \
                        & (1 << ((uwAPI) & 0x000F)) \
                        ) ? SQL_TRUE : SQL_FALSE \
                       )

/*
 * Extended definitions for SQLGetInfo
 */
#define SQL_INFO_FIRST                          0
#define SQL_ACTIVE_CONNECTIONS                  0
#define SQL_ACTIVE_STATEMENTS                   1
#define SQL_DRIVER_HDBC                         3
#define SQL_DRIVER_HENV                         4
#define SQL_DRIVER_HSTMT                        5
#define SQL_DRIVER_NAME                         6
#define SQL_DRIVER_VER                          7
#define SQL_ODBC_API_CONFORMANCE                9
#define SQL_ODBC_VER                            10
#define SQL_ROW_UPDATES                         11
#define SQL_ODBC_SAG_CLI_CONFORMANCE            12
#define SQL_ODBC_SQL_CONFORMANCE                15
#define SQL_PROCEDURES                          21
#define SQL_CONCAT_NULL_BEHAVIOR                22
#define SQL_CURSOR_ROLLBACK_BEHAVIOR            24
#define SQL_EXPRESSIONS_IN_ORDERBY              27
#define SQL_MAX_OWNER_NAME_LEN                  32
#define SQL_MAX_PROCEDURE_NAME_LEN              33
#define SQL_MAX_QUALIFIER_NAME_LEN              34
#define SQL_MULT_RESULT_SETS                    36
#define SQL_MULTIPLE_ACTIVE_TXN                 37
#define SQL_OUTER_JOINS                         38
#define SQL_OWNER_TERM                          39
#define SQL_PROCEDURE_TERM                      40
#define SQL_QUALIFIER_NAME_SEPARATOR            41
#define SQL_QUALIFIER_TERM                      42
#define SQL_SCROLL_OPTIONS                      44
#define SQL_TABLE_TERM                          45
#define SQL_CONVERT_FUNCTIONS                   48
#define SQL_NUMERIC_FUNCTIONS                   49
#define SQL_STRING_FUNCTIONS                    50
#define SQL_SYSTEM_FUNCTIONS                    51
#define SQL_TIMEDATE_FUNCTIONS                  52
#define SQL_CONVERT_BIGINT                      53
#define SQL_CONVERT_BINARY                      54
#define SQL_CONVERT_BIT                         55
#define SQL_CONVERT_CHAR                        56
#define SQL_CONVERT_DATE                        57
#define SQL_CONVERT_DECIMAL                     58
#define SQL_CONVERT_DOUBLE                      59
#define SQL_CONVERT_FLOAT                       60
#define SQL_CONVERT_INTEGER                     61
#define SQL_CONVERT_LONGVARCHAR                 62
#define SQL_CONVERT_NUMERIC                     63
#define SQL_CONVERT_REAL                        64
#define SQL_CONVERT_SMALLINT                    65
#define SQL_CONVERT_TIME                        66
#define SQL_CONVERT_TIMESTAMP                   67
#define SQL_CONVERT_TINYINT                     68
#define SQL_CONVERT_VARBINARY                   69
#define SQL_CONVERT_VARCHAR                     70
#define SQL_CONVERT_LONGVARBINARY               71
#define SQL_ODBC_SQL_OPT_IEF                    73
#define SQL_CORRELATION_NAME                    74
#define SQL_NON_NULLABLE_COLUMNS                75
#define SQL_DRIVER_HLIB                         76
#define SQL_DRIVER_ODBC_VER                     77
#define SQL_LOCK_TYPES                          78
#define SQL_POS_OPERATIONS                      79
#define SQL_POSITIONED_STATEMENTS               80
#define SQL_BOOKMARK_PERSISTENCE                82
#define SQL_STATIC_SENSITIVITY                  83
#define SQL_FILE_USAGE                          84
#define SQL_COLUMN_ALIAS                        87
#define SQL_GROUP_BY                            88
#define SQL_KEYWORDS                            89
#define SQL_OWNER_USAGE                         91
#define SQL_QUALIFIER_USAGE                     92
#define SQL_QUOTED_IDENTIFIER_CASE              93
#define SQL_SUBQUERIES                          95
#define SQL_UNION                               96
#define SQL_MAX_ROW_SIZE_INCLUDES_LONG          103
#define SQL_MAX_CHAR_LITERAL_LEN                108
#define SQL_TIMEDATE_ADD_INTERVALS              109
#define SQL_TIMEDATE_DIFF_INTERVALS             110
#define SQL_NEED_LONG_DATA_LEN                  111
#define SQL_MAX_BINARY_LITERAL_LEN              112
#define SQL_LIKE_ESCAPE_CLAUSE                  113
#define SQL_QUALIFIER_LOCATION                  114
#define SQL_ACTIVE_ENVIRONMENTS                 116
#define SQL_ALTER_DOMAIN                        117
#define SQL_SQL_CONFORMANCE                     118
#define SQL_DATETIME_LITERALS                   119
#define SQL_BATCH_ROW_COUNT                     120
#define SQL_BATCH_SUPPORT                       121
#define SQL_CONVERT_WCHAR                       122
#define SQL_CONVERT_INTERVAL_DAY_TIME           123
#define SQL_CONVERT_INTERVAL_YEAR_MONTH         124
#define SQL_CONVERT_WLONGVARCHAR                125
#define SQL_CONVERT_WVARCHAR                    126
#define SQL_CREATE_ASSERTION                    127
#define SQL_CREATE_CHARACTER_SET                128
#define SQL_CREATE_COLLATION                    129
#define SQL_CREATE_DOMAIN                       130
#define SQL_CREATE_SCHEMA                       131
#define SQL_CREATE_TABLE                        132
#define SQL_CREATE_TRANSLATION                  133
#define SQL_CREATE_VIEW                         134
#define SQL_DRIVER_HDESC                        135
#define SQL_DROP_ASSERTION                      136
#define SQL_DROP_CHARACTER_SET                  137
#define SQL_DROP_COLLATION                      138
#define SQL_DROP_DOMAIN                         139
#define SQL_DROP_SCHEMA                         140
#define SQL_DROP_TABLE                          141
#define SQL_DROP_TRANSLATION                    142
#define SQL_DROP_VIEW                           143
#define SQL_DYNAMIC_CURSOR_ATTRIBUTES1          144
#define SQL_DYNAMIC_CURSOR_ATTRIBUTES2          145
#define SQL_FORWARD_ONLY_CURSOR_ATTRIBUTES1     146
#define SQL_FORWARD_ONLY_CURSOR_ATTRIBUTES2     147
#define SQL_INDEX_KEYWORDS                      148
#define SQL_INFO_SCHEMA_VIEWS                   149
#define SQL_KEYSET_CURSOR_ATTRIBUTES1           150
#define SQL_KEYSET_CURSOR_ATTRIBUTES2           151
#define SQL_ODBC_INTERFACE_CONFORMANCE          152
#define SQL_PARAM_ARRAY_ROW_COUNTS              153
#define SQL_PARAM_ARRAY_SELECTS                 154
#define SQL_SQL92_DATETIME_FUNCTIONS            155
#define SQL_SQL92_FOREIGN_KEY_DELETE_RULE       156
#define SQL_SQL92_FOREIGN_KEY_UPDATE_RULE       157
#define SQL_SQL92_GRANT                         158
#define SQL_SQL92_NUMERIC_VALUE_FUNCTIONS       159
#define SQL_SQL92_PREDICATES                    160
#define SQL_SQL92_RELATIONAL_JOIN_OPERATORS     161
#define SQL_SQL92_REVOKE                        162
#define SQL_SQL92_ROW_VALUE_CONSTRUCTOR         163
#define SQL_SQL92_STRING_FUNCTIONS              164
#define SQL_SQL92_VALUE_EXPRESSIONS             165
#define SQL_STANDARD_CLI_CONFORMANCE            166
#define SQL_STATIC_CURSOR_ATTRIBUTES1           167
#define SQL_STATIC_CURSOR_ATTRIBUTES2           168
#define SQL_AGGREGATE_FUNCTIONS                 169
#define SQL_DDL_INDEX                           170
#define SQL_DM_VER                              171
#define SQL_INSERT_STATEMENT                    172
#define SQL_ASYNC_MODE                          10021
#define SQL_MAX_ASYNC_CONCURRENT_STATEMENTS     10022
#define SQL_UNION_STATEMENT                     SQL_UNION
#define SQL_CATALOG_LOCATION                    SQL_QUALIFIER_LOCATION
#define SQL_CATALOG_NAME_SEPARATOR              SQL_QUALIFIER_NAME_SEPARATOR
#define SQL_CATALOG_TERM                        SQL_QUALIFIER_TERM
#define SQL_CATALOG_USAGE                       SQL_QUALIFIER_USAGE
#define SQL_SCHEMA_TERM                         SQL_OWNER_TERM
#define SQL_SCHEMA_USAGE                        SQL_OWNER_USAGE

/*
 * SQL_CONVERT_*  return value bitmasks
 */
#define SQL_CVT_CHAR                            0x00000001L
#define SQL_CVT_NUMERIC                         0x00000002L
#define SQL_CVT_DECIMAL                         0x00000004L
#define SQL_CVT_INTEGER                         0x00000008L
#define SQL_CVT_SMALLINT                        0x00000010L
#define SQL_CVT_FLOAT                           0x00000020L
#define SQL_CVT_REAL                            0x00000040L
#define SQL_CVT_DOUBLE                          0x00000080L
#define SQL_CVT_VARCHAR                         0x00000100L
#define SQL_CVT_LONGVARCHAR                     0x00000200L
#define SQL_CVT_BINARY                          0x00000400L
#define SQL_CVT_VARBINARY                       0x00000800L
#define SQL_CVT_BIT                             0x00001000L
#define SQL_CVT_TINYINT                         0x00002000L
#define SQL_CVT_BIGINT                          0x00004000L
#define SQL_CVT_DATE                            0x00008000L
#define SQL_CVT_TIME                            0x00010000L
#define SQL_CVT_TIMESTAMP                       0x00020000L
#define SQL_CVT_LONGVARBINARY                   0x00040000L
#define SQL_CVT_INTERVAL_YEAR_MONTH             0x00080000L
#define SQL_CVT_INTERVAL_DAY_TIME               0x00100000L
#define SQL_CVT_WCHAR                           0x00200000L
#define SQL_CVT_WLONGVARCHAR                    0x00400000L
#define SQL_CVT_WVARCHAR                        0x00800000L

/*
 * SQL_CONVERT_FUNCTIONS functions
 */
#define SQL_FN_CVT_CONVERT                      0x00000001L
#define SQL_FN_CVT_CAST                         0x00000002L

/*
 * SQL_STRING_FUNCTIONS functions
 */
#define SQL_FN_STR_CONCAT                       0x00000001L
#define SQL_FN_STR_INSERT                       0x00000002L
#define SQL_FN_STR_LEFT                         0x00000004L
#define SQL_FN_STR_LTRIM                        0x00000008L
#define SQL_FN_STR_LENGTH                       0x00000010L
#define SQL_FN_STR_LOCATE                       0x00000020L
#define SQL_FN_STR_LCASE                        0x00000040L
#define SQL_FN_STR_REPEAT                       0x00000080L
#define SQL_FN_STR_REPLACE                      0x00000100L
#define SQL_FN_STR_RIGHT                        0x00000200L
#define SQL_FN_STR_RTRIM                        0x00000400L
#define SQL_FN_STR_SUBSTRING                    0x00000800L
#define SQL_FN_STR_UCASE                        0x00001000L
#define SQL_FN_STR_ASCII                        0x00002000L
#define SQL_FN_STR_CHAR                         0x00004000L
#define SQL_FN_STR_DIFFERENCE                   0x00008000L
#define SQL_FN_STR_LOCATE_2                     0x00010000L
#define SQL_FN_STR_SOUNDEX                      0x00020000L
#define SQL_FN_STR_SPACE                        0x00040000L
#define SQL_FN_STR_BIT_LENGTH                   0x00080000L
#define SQL_FN_STR_CHAR_LENGTH                  0x00100000L
#define SQL_FN_STR_CHARACTER_LENGTH             0x00200000L
#define SQL_FN_STR_OCTET_LENGTH                 0x00400000L
#define SQL_FN_STR_POSITION                     0x00800000L
#define SQL_FN_STR_OVERLAY                      0x00600000L

/*
 * SQL_SQL92_STRING_FUNCTIONS
 */
#define SQL_SSF_CONVERT                         0x00000001L
#define SQL_SSF_LOWER                           0x00000002L
#define SQL_SSF_UPPER                           0x00000004L
#define SQL_SSF_SUBSTRING                       0x00000008L
#define SQL_SSF_TRANSLATE                       0x00000010L
#define SQL_SSF_TRIM_BOTH                       0x00000020L
#define SQL_SSF_TRIM_LEADING                    0x00000040L
#define SQL_SSF_TRIM_TRAILING                   0x00000080L

/*
 * SQL_NUMERIC_FUNCTIONS functions
 */
#define SQL_FN_NUM_ABS                          0x00000001L
#define SQL_FN_NUM_ACOS                         0x00000002L
#define SQL_FN_NUM_ASIN                         0x00000004L
#define SQL_FN_NUM_ATAN                         0x00000008L
#define SQL_FN_NUM_ATAN2                        0x00000010L
#define SQL_FN_NUM_CEILING                      0x00000020L
#define SQL_FN_NUM_COS                          0x00000040L
#define SQL_FN_NUM_COT                          0x00000080L
#define SQL_FN_NUM_EXP                          0x00000100L
#define SQL_FN_NUM_FLOOR                        0x00000200L
#define SQL_FN_NUM_LOG                          0x00000400L
#define SQL_FN_NUM_MOD                          0x00000800L
#define SQL_FN_NUM_SIGN                         0x00001000L
#define SQL_FN_NUM_SIN                          0x00002000L
#define SQL_FN_NUM_SQRT                         0x00004000L
#define SQL_FN_NUM_TAN                          0x00008000L
#define SQL_FN_NUM_PI                           0x00010000L
#define SQL_FN_NUM_RAND                         0x00020000L
#define SQL_FN_NUM_DEGREES                      0x00040000L
#define SQL_FN_NUM_LOG10                        0x00080000L
#define SQL_FN_NUM_POWER                        0x00100000L
#define SQL_FN_NUM_RADIANS                      0x00200000L
#define SQL_FN_NUM_ROUND                        0x00400000L
#define SQL_FN_NUM_TRUNCATE                     0x00800000L

/*
 * SQL_SQL92_NUMERIC_VALUE_FUNCTIONS
 */
#define SQL_SNVF_BIT_LENGTH                     0x00000001L
#define SQL_SNVF_CHAR_LENGTH                    0x00000002L
#define SQL_SNVF_CHARACTER_LENGTH               0x00000004L
#define SQL_SNVF_EXTRACT                        0x00000008L
#define SQL_SNVF_OCTET_LENGTH                   0x00000010L
#define SQL_SNVF_POSITION                       0x00000020L

/*
 * SQL_TIMEDATE_FUNCTIONS functions
 */
#define SQL_FN_TD_NOW                           0x00000001L
#define SQL_FN_TD_CURDATE                       0x00000002L
#define SQL_FN_TD_DAYOFMONTH                    0x00000004L
#define SQL_FN_TD_DAYOFWEEK                     0x00000008L
#define SQL_FN_TD_DAYOFYEAR                     0x00000010L
#define SQL_FN_TD_MONTH                         0x00000020L
#define SQL_FN_TD_QUARTER                       0x00000040L
#define SQL_FN_TD_WEEK                          0x00000080L
#define SQL_FN_TD_YEAR                          0x00000100L
#define SQL_FN_TD_CURTIME                       0x00000200L
#define SQL_FN_TD_HOUR                          0x00000400L
#define SQL_FN_TD_MINUTE                        0x00000800L
#define SQL_FN_TD_SECOND                        0x00001000L
#define SQL_FN_TD_TIMESTAMPADD                  0x00002000L
#define SQL_FN_TD_TIMESTAMPDIFF                 0x00004000L
#define SQL_FN_TD_DAYNAME                       0x00008000L
#define SQL_FN_TD_MONTHNAME                     0x00010000L
#define SQL_FN_TD_CURRENT_DATE                  0x00020000L
#define SQL_FN_TD_CURRENT_TIME                  0x00040000L
#define SQL_FN_TD_CURRENT_TIMESTAMP             0x00080000L
#define SQL_FN_TD_EXTRACT                       0x00100000L

/*
 * SQL_SQL92_DATETIME_FUNCTIONS
 */
#define SQL_SDF_CURRENT_DATE                    0x00000001L
#define SQL_SDF_CURRENT_TIME                    0x00000002L
#define SQL_SDF_CURRENT_TIMESTAMP               0x00000004L

/*
 * SQL_SYSTEM_FUNCTIONS functions
 */
#define SQL_FN_SYS_USERNAME                     0x00000001L
#define SQL_FN_SYS_DBNAME                       0x00000002L
#define SQL_FN_SYS_IFNULL                       0x00000004L

/*
 * SQL_TIMEDATE_ADD_INTERVALS and SQL_TIMEDATE_DIFF_INTERVALS functions
 */
#define SQL_FN_TSI_FRAC_SECOND                  0x00000001L
#define SQL_FN_TSI_SECOND                       0x00000002L
#define SQL_FN_TSI_MINUTE                       0x00000004L
#define SQL_FN_TSI_HOUR                         0x00000008L
#define SQL_FN_TSI_DAY                          0x00000010L
#define SQL_FN_TSI_WEEK                         0x00000020L
#define SQL_FN_TSI_MONTH                        0x00000040L
#define SQL_FN_TSI_QUARTER                      0x00000080L
#define SQL_FN_TSI_YEAR                         0x00000100L

/*
 * Generally useful constants
 */
#define SQL_SQLSTATE_SIZE               5       /* size of SQLSTATE */
#define SQL_MAX_DSN_LENGTH              32      /* maximum data source name size */
#define SQL_NO_DATA_FOUND               SQL_NO_DATA

/*
 * Env attribute
 */
#define SQL_ATTR_ODBC_VERSION           200
#define SQL_ATTR_CONNECTION_POOLING     201
#define SQL_ATTR_CP_MATCH               202

/*
 * Connection attributes
 */
#define SQL_ACCESS_MODE                 101
#define SQL_AUTOCOMMIT                  102
#define SQL_LOGIN_TIMEOUT               103
#define SQL_OPT_TRACE                   104
#define SQL_OPT_TRACEFILE               105
#define SQL_TRANSLATE_DLL               106
#define SQL_TRANSLATE_OPTION            107
#define SQL_TXN_ISOLATION               108
#define SQL_CURRENT_QUALIFIER           109
#define SQL_ODBC_CURSORS                110
#define SQL_QUIET_MODE                  111
#define SQL_PACKET_SIZE                 112

#define SQL_ATTR_CONNECTION_TIMEOUT     113
#define SQL_ATTR_DISCONNECT_BEHAVIOR    114
#define SQL_ATTR_ENLIST_IN_DTC          1207
#define SQL_ATTR_ENLIST_IN_XA           1208
#define SQL_ATTR_LOGIN_TIMEOUT          SQL_LOGIN_TIMEOUT
#define SQL_ATTR_ODBC_CURSORS           SQL_ODBC_CURSORS
#define SQL_ATTR_PACKET_SIZE            SQL_PACKET_SIZE
#define SQL_ATTR_QUIET_MODE             SQL_QUIET_MODE
#define SQL_ATTR_TRACE                  SQL_OPT_TRACE
#define SQL_ATTR_TRACEFILE              SQL_OPT_TRACEFILE
#define SQL_ATTR_TRANSLATE_LIB          SQL_TRANSLATE_DLL
#define SQL_ATTR_TRANSLATE_OPTION       SQL_TRANSLATE_OPTION
#define SQL_ATTR_TXN_ISOLATION          SQL_TXN_ISOLATION
#define SQL_ATTR_ACCESS_MODE            SQL_ACCESS_MODE
#define SQL_ATTR_AUTOCOMMIT             SQL_AUTOCOMMIT
#define SQL_ATTR_CURRENT_CATALOG        SQL_CURRENT_QUALIFIER

/*
 * Statement attributes
 */
#define SQL_QUERY_TIMEOUT               0
#define SQL_MAX_ROWS                    1
#define SQL_NOSCAN                      2
#define SQL_MAX_LENGTH                  3
#define SQL_ASYNC_ENABLE                4
#define SQL_BIND_TYPE                   5
#define SQL_CURSOR_TYPE                 6
#define SQL_CONCURRENCY                 7
#define SQL_KEYSET_SIZE                 8
#define SQL_ROWSET_SIZE                 9
#define SQL_SIMULATE_CURSOR             10
#define SQL_RETRIEVE_DATA               11
#define SQL_USE_BOOKMARKS               12
#define SQL_GET_BOOKMARK                13
#define SQL_ROW_NUMBER                  14

#define SQL_ATTR_ENABLE_AUTO_IPD        15
#define SQL_ATTR_FETCH_BOOKMARK_PTR     16
#define SQL_ATTR_PARAM_BIND_OFFSET_PTR  17
#define SQL_ATTR_PARAM_BIND_TYPE        18
#define SQL_ATTR_PARAM_OPERATION_PTR    19
#define SQL_ATTR_PARAM_STATUS_PTR       20
#define SQL_ATTR_PARAMS_PROCESSED_PTR   21
#define SQL_ATTR_PARAMSET_SIZE          22
#define SQL_ATTR_ROW_BIND_OFFSET_PTR    23
#define SQL_ATTR_ROW_OPERATION_PTR      24
#define SQL_ATTR_ROW_STATUS_PTR         25
#define SQL_ATTR_ROWS_FETCHED_PTR       26
#define SQL_ATTR_ROW_ARRAY_SIZE         27
#define SQL_ATTR_SIMULATE_CURSOR        SQL_SIMULATE_CURSOR
#define SQL_ATTR_USE_BOOKMARKS          SQL_USE_BOOKMARKS
#define SQL_ATTR_ASYNC_ENABLE           SQL_ASYNC_ENABLE
#define SQL_ATTR_CONCURRENCY            SQL_CONCURRENCY
#define SQL_ATTR_CURSOR_TYPE            SQL_CURSOR_TYPE
#define SQL_ATTR_KEYSET_SIZE            SQL_KEYSET_SIZE
#define SQL_ATTR_MAX_LENGTH             SQL_MAX_LENGTH
#define SQL_ATTR_MAX_ROWS               SQL_MAX_ROWS
#define SQL_ATTR_NOSCAN                 SQL_NOSCAN
#define SQL_ATTR_QUERY_TIMEOUT          SQL_QUERY_TIMEOUT
#define SQL_ATTR_RETRIEVE_DATA          SQL_RETRIEVE_DATA
#define SQL_ATTR_ROW_BIND_TYPE          SQL_BIND_TYPE
#define SQL_ATTR_ROW_NUMBER             SQL_ROW_NUMBER

/*
 * SQLColAttributes defines
 */
#define SQL_COLUMN_COUNT                0
#define SQL_COLUMN_NAME                 1
#define SQL_COLUMN_TYPE                 2
#define SQL_COLUMN_LENGTH               3
#define SQL_COLUMN_PRECISION            4
#define SQL_COLUMN_SCALE                5
#define SQL_COLUMN_DISPLAY_SIZE         6
#define SQL_COLUMN_NULLABLE             7
#define SQL_COLUMN_UNSIGNED             8
#define SQL_COLUMN_MONEY                9
#define SQL_COLUMN_UPDATABLE            10
#define SQL_COLUMN_AUTO_INCREMENT       11
#define SQL_COLUMN_CASE_SENSITIVE       12
#define SQL_COLUMN_SEARCHABLE           13
#define SQL_COLUMN_TYPE_NAME            14
#define SQL_COLUMN_TABLE_NAME           15
#define SQL_COLUMN_OWNER_NAME           16
#define SQL_COLUMN_QUALIFIER_NAME       17
#define SQL_COLUMN_LABEL                18

/*
 * SQLColAttributes subdefines for SQL_COLUMN_UPDATABLE
 */
#define SQL_ATTR_READONLY               0
#define SQL_ATTR_WRITE                  1
#define SQL_ATTR_READWRITE_UNKNOWN      2

/*
 * SQLColAttributes subdefines for SQL_COLUMN_SEARCHABLE
 */
#define SQL_UNSEARCHABLE                0
#define SQL_LIKE_ONLY                   1
#define SQL_ALL_EXCEPT_LIKE             2
#define SQL_SEARCHABLE                  3
#define SQL_PRED_SEARCHABLE             SQL_SEARCHABLE

/*
 * SQL_ACCESS_MODE options
 */
#define SQL_MODE_READ_WRITE             0UL
#define SQL_MODE_READ_ONLY              1UL
#define SQL_MODE_DEFAULT                SQL_MODE_READ_WRITE

/*
 * SQL_AUTOCOMMIT options
 */
#define SQL_AUTOCOMMIT_OFF              0UL
#define SQL_AUTOCOMMIT_ON               1UL
#define SQL_AUTOCOMMIT_DEFAULT          SQL_AUTOCOMMIT_ON

/*
 * SQL_OPT_TRACE options
 */
#define SQL_OPT_TRACE_OFF               0UL
#define SQL_OPT_TRACE_ON                1UL
#define SQL_OPT_TRACE_DEFAULT           SQL_OPT_TRACE_OFF
#define SQL_OPT_TRACE_FILE_DEFAULT      "\\sql.log"

/*
 * Whether an attribute is a pointer or not
 */
#define SQL_IS_POINTER                  (-4)
#define SQL_IS_UINTEGER                 (-5)
#define SQL_IS_INTEGER                  (-6)
#define SQL_IS_USMALLINT                (-7)
#define SQL_IS_SMALLINT                 (-8)

/*
 * SQL_NOSCAN options
 */
#define SQL_NOSCAN_OFF                  0UL /* FALSE */
#define SQL_NOSCAN_ON                   1UL /* TRUE */
#define SQL_NOSCAN_DEFAULT              SQL_NOSCAN_OFF

/*
 * Values for SQL_ATTR_ASYNC_ENABLE
 */
#define SQL_ASYNC_ENABLE_OFF            0UL
#define SQL_ASYNC_ENABLE_ON             1UL
#define SQL_ASYNC_ENABLE_DEFAULT        SQL_ASYNC_ENABLE_OFF

/*
 * SQL_BIND_TYPE options
 */
#define SQL_BIND_BY_COLUMN              0UL
#define SQL_BIND_TYPE_DEFAULT           SQL_BIND_BY_COLUMN

/*
 * SQL_CONCURRENCY options
 */
#define SQL_CONCUR_READ_ONLY            1
#define SQL_CONCUR_LOCK                 2
#define SQL_CONCUR_ROWVER               3
#define SQL_CONCUR_VALUES               4
#define SQL_CONCUR_DEFAULT              SQL_CONCUR_READ_ONLY

/*
 * SQL_CURSOR_TYPE options
 */
#define SQL_CURSOR_FORWARD_ONLY         0UL
#define SQL_CURSOR_KEYSET_DRIVEN        1UL
#define SQL_CURSOR_DYNAMIC              2UL
#define SQL_CURSOR_STATIC               3UL
#define SQL_CURSOR_TYPE_DEFAULT         SQL_CURSOR_FORWARD_ONLY

/*
 * SQL_KEYSET_SIZE options
 */
#define SQL_KEYSET_SIZE_DEFAULT         0UL

/*
 * SQL_RETRIEVE_DATA options
 */
#define SQL_RD_OFF                      0UL
#define SQL_RD_ON                       1UL
#define SQL_RD_DEFAULT                  SQL_RD_ON

/*
 * Extended descriptor field
 */
#define SQL_DESC_ARRAY_SIZE                     20
#define SQL_DESC_ARRAY_STATUS_PTR               21
#define SQL_DESC_BASE_COLUMN_NAME               22
#define SQL_DESC_BASE_TABLE_NAME                23
#define SQL_DESC_BIND_OFFSET_PTR                24
#define SQL_DESC_BIND_TYPE                      25
#define SQL_DESC_DATETIME_INTERVAL_PRECISION    26
#define SQL_DESC_LITERAL_PREFIX                 27
#define SQL_DESC_LITERAL_SUFFIX                 28
#define SQL_DESC_LOCAL_TYPE_NAME                29
#define SQL_DESC_MAXIMUM_SCALE                  30
#define SQL_DESC_MINIMUM_SCALE                  31
#define SQL_DESC_NUM_PREC_RADIX                 32
#define SQL_DESC_PARAMETER_TYPE                 33
#define SQL_DESC_ROWS_PROCESSED_PTR             34
#define SQL_DESC_SCHEMA_NAME                    SQL_COLUMN_OWNER_NAME
#define SQL_DESC_SEARCHABLE                     SQL_COLUMN_SEARCHABLE
#define SQL_DESC_TYPE_NAME                      SQL_COLUMN_TYPE_NAME
#define SQL_DESC_TABLE_NAME                     SQL_COLUMN_TABLE_NAME
#define SQL_DESC_UNSIGNED                       SQL_COLUMN_UNSIGNED
#define SQL_DESC_UPDATABLE                      SQL_COLUMN_UPDATABLE
#define SQL_DESC_AUTO_UNIQUE_VALUE              SQL_COLUMN_AUTO_INCREMENT
#define SQL_DESC_CASE_SENSITIVE                 SQL_COLUMN_CASE_SENSITIVE
#define SQL_DESC_CATALOG_NAME                   SQL_COLUMN_QUALIFIER_NAME
#define SQL_DESC_CONCISE_TYPE                   SQL_COLUMN_TYPE
#define SQL_DESC_DISPLAY_SIZE                   SQL_COLUMN_DISPLAY_SIZE
#define SQL_DESC_FIXED_PREC_SCALE               SQL_COLUMN_MONEY
#define SQL_DESC_LABEL                          SQL_COLUMN_LABEL

/*
 * Defines for diagnostics fields
 */
#define SQL_DIAG_CURSOR_ROW_COUNT               (-1249)
#define SQL_DIAG_ROW_NUMBER                     (-1248)
#define SQL_DIAG_COLUMN_NUMBER                  (-1247)

/*
 * Diagnostics function code
 */
#define SQL_DIAG_CALL                           7

/*
 * Define for SQL_DIAG_ROW_NUMBER and SQL_DIAG_COLUMN_NUMBER
 */
#define SQL_NO_ROW_NUMBER                       (-1)
#define SQL_NO_COLUMN_NUMBER                    (-1)
#define SQL_ROW_NUMBER_UNKNOWN                  (-2)
#define SQL_COLUMN_NUMBER_UNKNOWN               (-2)

/*
 * SQLBindParameter extensions
 */
#define SQL_SETPARAM_VALUE_MAX                  (-1L)
#define SQL_DEFAULT_PARAM                       (-5)
#define SQL_IGNORE                              (-6)
#define SQL_COLUMN_IGNORE                       SQL_IGNORE
#define SQL_LEN_DATA_AT_EXEC_OFFSET             (-100)
#define SQL_LEN_DATA_AT_EXEC(length)            (-(length)+SQL_LEN_DATA_AT_EXEC_OFFSET)

/*
 * Special return values for SQLGetData
 */
#define SQL_NO_TOTAL                    (-4)

/*
 * SQL_EXT_API_LAST is not useful with ODBC version 3.0 because some of the
 * values from X/Open are in the 10000 range
 */
#define SQL_EXT_API_LAST                SQL_API_SQLBINDPARAMETER
#define SQL_NUM_FUNCTIONS               23
#define SQL_EXT_API_START               40
#define SQL_NUM_EXTENSIONS              (SQL_EXT_API_LAST-SQL_EXT_API_START+1)

/*
 * bitmask for SQL_DYNAMIC_CURSOR_ATTRIBUTES1,
 * SQL_FORWARD_ONLY_CURSOR_ATTRIBUTES1,
 * SQL_KEYSET_CURSOR_ATTRIBUTES1, and SQL_STATIC_CURSOR_ATTRIBUTES1
 */
#define SQL_CA1_NEXT                            0x00000001L

/*
 * bitmasks for SQL_DYNAMIC_CURSOR_ATTRIBUTES2,
 * SQL_FORWARD_ONLY_CURSOR_ATTRIBUTES2,
 * SQL_KEYSET_CURSOR_ATTRIBUTES2, and SQL_STATIC_CURSOR_ATTRIBUTES2
 */
#define SQL_CA2_READ_ONLY_CONCURRENCY           0x00000001L
#define SQL_CA2_MAX_ROWS_SELECT                 0x00000080L
#define SQL_CA2_CRC_EXACT                       0x00001000L

/*
 * SQL_ODBC_API_CONFORMANCE values
 */
#define SQL_OAC_NONE                        0x0000
#define SQL_OAC_LEVEL1                      0x0001
#define SQL_OAC_LEVEL2                      0x0002

/*
 * SQL_ODBC_SAG_CLI_CONFORMANCE values
 */
#define SQL_OSCC_NOT_COMPLIANT              0x0000
#define SQL_OSCC_COMPLIANT                  0x0001

/*
 * SQL_ODBC_SQL_CONFORMANCE values
 */
#define SQL_OSC_MINIMUM                     0x0000
#define SQL_OSC_CORE                        0x0001
#define SQL_OSC_EXTENDED                    0x0002

/*
 * SQL_CONCAT_NULL_BEHAVIOR values
 */
#define SQL_CB_NULL                         0x0000
#define SQL_CB_NON_NULL                     0x0001

/*
 * SQL_SCROLL_OPTIONS mask
 */
#define SQL_SO_FORWARD_ONLY                 0x00000001L

/*
 * SQL_CORRELATION_NAME value
 */
#define SQL_CN_ANY                          0x0002

/*
 * SQL_NON_NULLABLE_COLUMNS values
 */
#define SQL_NNC_NON_NULL                    0x0001

/*
 * SQL_FILE_USAGE values
 */
#define SQL_FILE_NOT_SUPPORTED              0x0000

/*
 * SQL_POSITIONED_STATEMENTS masks
 */
#define SQL_PS_POSITIONED_DELETE            0x00000001L
#define SQL_PS_POSITIONED_UPDATE            0x00000002L
#define SQL_PS_SELECT_FOR_UPDATE            0x00000004L

/*
 * SQL_GROUP_BY values
 */
#define SQL_GB_NOT_SUPPORTED                0x0000
#define SQL_GB_GROUP_BY_EQUALS_SELECT       0x0001
#define SQL_GB_GROUP_BY_CONTAINS_SELECT     0x0002
#define SQL_GB_NO_RELATION                  0x0003
#define SQL_GB_COLLATE                      0x0004

/*
 * SQL_OWNER_USAGE masks
 */
#define SQL_OU_DML_STATEMENTS               0x00000001L
#define SQL_OU_PROCEDURE_INVOCATION         0x00000002L
#define SQL_OU_TABLE_DEFINITION             0x00000004L
#define SQL_OU_INDEX_DEFINITION             0x00000008L
#define SQL_OU_PRIVILEGE_DEFINITION         0x00000010L

/*
 * SQL_SCHEMA_USAGE masks
 */
#define SQL_SU_DML_STATEMENTS                   SQL_OU_DML_STATEMENTS
#define SQL_SU_PROCEDURE_INVOCATION             SQL_OU_PROCEDURE_INVOCATION
#define SQL_SU_TABLE_DEFINITION                 SQL_OU_TABLE_DEFINITION
#define SQL_SU_INDEX_DEFINITION                 SQL_OU_INDEX_DEFINITION
#define SQL_SU_PRIVILEGE_DEFINITION             SQL_OU_PRIVILEGE_DEFINITION

/*
 * SQL_SUBQUERIES masks
 */
#define SQL_SQ_COMPARISON                   0x00000001L
#define SQL_SQ_EXISTS                       0x00000002L
#define SQL_SQ_IN                           0x00000004L
#define SQL_SQ_QUANTIFIED                   0x00000008L
#define SQL_SQ_CORRELATED_SUBQUERIES        0x00000010L

/*
 * SQL_UNION masks
 */
#define SQL_U_UNION                         0x00000001L
#define SQL_U_UNION_ALL                     0x00000002L

/*
 * SQL_VIEW values
 */
#define SQL_CV_CREATE_VIEW                      0x00000001L
#define SQL_CV_CHECK_OPTION                     0x00000002L
#define SQL_CV_CASCADED                         0x00000004L
#define SQL_CV_LOCAL                            0x00000008L

/*
 * SQL_AGGREGATE_FUNCTIONS bitmasks
 */
#define SQL_AF_AVG                              0x00000001L
#define SQL_AF_COUNT                            0x00000002L
#define SQL_AF_MAX                              0x00000004L
#define SQL_AF_MIN                              0x00000008L
#define SQL_AF_SUM                              0x00000010L
#define SQL_AF_DISTINCT                         0x00000020L
#define SQL_AF_ALL                              (SQL_AF_AVG | SQL_AF_COUNT | SQL_AF_MAX | SQL_AF_MIN | SQL_AF_SUM | SQL_AF_DISTINCT)

/*
 * SQL_SQL_CONFORMANCE bit masks
 */
#define SQL_SC_SQL92_ENTRY                      0x00000001L

/*
 * Values for SQL_PARAM_ARRAY_ROW_COUNTS getinfo
 */
#define SQL_PARC_BATCH          1
#define SQL_PARC_NO_BATCH       2

/*
 * values for SQL_PARAM_ARRAY_SELECT_BATCH
 */
#define SQL_PAS_BATCH           1
#define SQL_PAS_NO_BATCH        2
#define SQL_PAS_NO_SELECT       3

/*
 * Bitmasks for SQL_INDEX_KEYWORDS
 */
#define SQL_IK_NONE             0x00000000L
#define SQL_IK_ASC              0x00000001L
#define SQL_IK_DESC             0x00000002L
#define SQL_IK_ALL              (SQL_IK_ASC | SQL_IK_DESC)

/*
 * Bitmasks for SQL_ASYNC_MODE
 */
#define SQL_AM_NONE             0
#define SQL_AM_CONNECTION       1
#define SQL_AM_STATEMENT        2

/*
 * SQL_CREATE_TABLE bitmasks
 */
#define SQL_CT_CREATE_TABLE                     0x00000001L
#define SQL_CT_COMMIT_PRESERVE                  0x00000002L
#define SQL_CT_COMMIT_DELETE                    0x00000004L
#define SQL_CT_GLOBAL_TEMPORARY                 0x00000008L
#define SQL_CT_LOCAL_TEMPORARY                  0x00000010L
#define SQL_CT_CONSTRAINT_INITIALLY_DEFERRED    0x00000020L
#define SQL_CT_CONSTRAINT_INITIALLY_IMMEDIATE   0x00000040L
#define SQL_CT_CONSTRAINT_DEFERRABLE            0x00000080L
#define SQL_CT_CONSTRAINT_NON_DEFERRABLE        0x00000100L
#define SQL_CT_COLUMN_CONSTRAINT                0x00000200L
#define SQL_CT_COLUMN_DEFAULT                   0x00000400L
#define SQL_CT_COLUMN_COLLATION                 0x00000800L
#define SQL_CT_TABLE_CONSTRAINT                 0x00001000L
#define SQL_CT_CONSTRAINT_NAME_DEFINITION       0x00002000L

/*
 * SQL_DDL_INDEX bitmasks
 */
#define SQL_DI_CREATE_INDEX                     0x00000001L
#define SQL_DI_DROP_INDEX                       0x00000002L

/*
 * SQL_DROP_TABLE bitmasks
 */
#define SQL_DT_DROP_TABLE                       0x00000001L

/*
 * SQL_DROP_VIEW bitmasks
 */
#define SQL_DV_DROP_VIEW                        0x00000001L

/*
 * SQL_INSERT_STATEMENT bitmasks
 */
#define SQL_IS_INSERT_LITERALS                  0x00000001L
#define SQL_IS_INSERT_SEARCHED                  0x00000002L
#define SQL_IS_SELECT_INTO                      0x00000004L

/*
 * SQL_ODBC_INTERFACE_CONFORMANCE values
 */
#define SQL_OIC_CORE                            1UL

/*
 * SQL_SQL92_GRANT bitmasks
 */
#define SQL_SG_DELETE_TABLE                     0x00000020L
#define SQL_SG_INSERT_TABLE                     0x00000040L
#define SQL_SG_INSERT_COLUMN                    0x00000080L
#define SQL_SG_REFERENCES_TABLE                 0x00000100L
#define SQL_SG_REFERENCES_COLUMN                0x00000200L
#define SQL_SG_SELECT_TABLE                     0x00000400L
#define SQL_SG_UPDATE_TABLE                     0x00000800L
#define SQL_SG_UPDATE_COLUMN                    0x00001000L

/*
 * SQL_SQL92_PREDICATES bitmasks
 */
#define SQL_SP_EXISTS                           0x00000001L
#define SQL_SP_ISNOTNULL                        0x00000002L
#define SQL_SP_ISNULL                           0x00000004L
#define SQL_SP_MATCH_FULL                       0x00000008L
#define SQL_SP_MATCH_PARTIAL                    0x00000010L
#define SQL_SP_MATCH_UNIQUE_FULL                0x00000020L
#define SQL_SP_MATCH_UNIQUE_PARTIAL             0x00000040L
#define SQL_SP_OVERLAPS                         0x00000080L
#define SQL_SP_UNIQUE                           0x00000100L
#define SQL_SP_LIKE                             0x00000200L
#define SQL_SP_IN                               0x00000400L
#define SQL_SP_BETWEEN                          0x00000800L
#define SQL_SP_COMPARISON                       0x00001000L
#define SQL_SP_QUANTIFIED_COMPARISON            0x00002000L

/*
 * SQL_SQL92_REVOKE bitmasks
 */
#define SQL_SR_USAGE_ON_DOMAIN                  0x00000001L
#define SQL_SR_USAGE_ON_CHARACTER_SET           0x00000002L
#define SQL_SR_USAGE_ON_COLLATION               0x00000004L
#define SQL_SR_USAGE_ON_TRANSLATION             0x00000008L
#define SQL_SR_GRANT_OPTION_FOR                 0x00000010L
#define SQL_SR_CASCADE                          0x00000020L
#define SQL_SR_RESTRICT                         0x00000040L
#define SQL_SR_DELETE_TABLE                     0x00000080L
#define SQL_SR_INSERT_TABLE                     0x00000100L
#define SQL_SR_INSERT_COLUMN                    0x00000200L
#define SQL_SR_REFERENCES_TABLE                 0x00000400L
#define SQL_SR_REFERENCES_COLUMN                0x00000800L
#define SQL_SR_SELECT_TABLE                     0x00001000L
#define SQL_SR_UPDATE_TABLE                     0x00002000L
#define SQL_SR_UPDATE_COLUMN                    0x00004000L

/*
 * SQL_SQL92_VALUE_EXPRESSIONS bitmasks
 */
#define SQL_SVE_COALESCE                        0x00000004L
#define SQL_SVE_NULLIF                          0x00000008L

/*
 * SQL_STANDARD_CLI_CONFORMANCE bitmasks
 */
#define SQL_SCC_XOPEN_CLI_VERSION1              0x00000001L

/*
 * Lock options in SQLSetPos
 */
#define SQL_LOCK_NO_CHANGE           0 /* FALSE */
#define SQL_LOCK_EXCLUSIVE           1 /* TRUE */
#define SQL_LOCK_UNLOCK              2

/*
 * Macros for SQLSetPos
 */
#define SQL_POSITION_TO(hstmt,irow)             SQLSetPos(hstmt,irow,SQL_POSITION,SQL_LOCK_NO_CHANGE)
#define SQL_LOCK_RECORD(hstmt,irow,fLock)       SQLSetPos(hstmt,irow,SQL_POSITION,fLock)
#define SQL_REFRESH_RECORD(hstmt,irow,fLock)    SQLSetPos(hstmt,irow,SQL_REFRESH,fLock)
#define SQL_UPDATE_RECORD(hstmt,irow)           SQLSetPos(hstmt,irow,SQL_UPDATE,SQL_LOCK_NO_CHANGE)
#define SQL_DELETE_RECORD(hstmt,irow)           SQLSetPos(hstmt,irow,SQL_DELETE,SQL_LOCK_NO_CHANGE)
#define SQL_ADD_RECORD(hstmt,irow)              SQLSetPos(hstmt,irow,SQL_ADD,SQL_LOCK_NO_CHANGE)

/*
 * Column types and scopes in SQLSpecialColumns
 */
#define SQL_BEST_ROWID                  1
#define SQL_ROWVER                      2

/*
 * Defines for SQLStatistics
 */
#define SQL_QUICK                       0
#define SQL_ENSURE                      1

/*
 * Defines for SQLTables
 */
#define SQL_ALL_CATALOGS                "%"
#define SQL_ALL_SCHEMAS                 "%"
#define SQL_ALL_TABLE_TYPES             "%"

/*
 * Options for SQLDriverConnect
 */
#define SQL_DRIVER_NOPROMPT             0
#define SQL_DRIVER_COMPLETE             1
#define SQL_DRIVER_PROMPT               2
#define SQL_DRIVER_COMPLETE_REQUIRED    3

#ifndef RC_INVOKED
RETCODE SQL_API SQLDriverConnect (SQLHDBC hdbc,
                                  SQLHWND hwnd,
                                  SQLTCHAR * szConnStrIn,
                                  SQLSMALLINT cbConnStrIn,
                                  SQLTCHAR * szConnStrOut,
                                  SQLSMALLINT cbConnStrOutMax,
                                  SQLSMALLINT * pcbConnStrOut,
                                  SQLUSMALLINT fDriverCompletion);
#endif /* RC_INVOKED */


/*
 * SQLExtendedFetch "rgfRowStatus" element values
 */
#define SQL_ROW_SUCCESS                 0
#define SQL_ROW_DELETED                 1
#define SQL_ROW_UPDATED                 2
#define SQL_ROW_NOROW                   3
#define SQL_ROW_ADDED                   4
#define SQL_ROW_ERROR                   5

/*
 * value for SQL_DESC_ARRAY_STATUS_PTR
 */
#define SQL_PARAM_SUCCESS               0
#define SQL_PARAM_PROCEED               0
#define SQL_PARAM_DIAG_UNAVAILABLE      1
#define SQL_PARAM_IGNORE                1
#define SQL_PARAM_ERROR                 5
#define SQL_PARAM_SUCCESS_WITH_INFO     6
#define SQL_PARAM_UNUSED                7

/*
 * Defines for SQLForeignKeys (returned in result set)
 */
#define SQL_CASCADE                     0
#define SQL_RESTRICT                    1
#define SQL_SET_NULL                    2
#define SQL_NO_ACTION                   3
#define SQL_SET_DEFAULT                 4
#define SQL_INITIALLY_DEFERRED          5
#define SQL_INITIALLY_IMMEDIATE         6
#define SQL_NOT_DEFERRABLE              7

/*
 * Defines for SQLBindParameter and
 * SQLProcedureColumns (returned in the result set)
 */
#define SQL_PARAM_TYPE_UNKNOWN          0
#define SQL_PARAM_INPUT                 1
#define SQL_PARAM_INPUT_OUTPUT          2
#define SQL_RESULT_COL                  3
#define SQL_PARAM_OUTPUT                4
#define SQL_RETURN_VALUE                5

#ifndef RC_INVOKED
/* This define is too large for RC */
#define SQL_ODBC_KEYWORDS \
"ABSOLUTE,ACTION,ADA,ADD,ALL,ALLOCATE,ALTER,AND,ANY,ARE,AS,ASC,ASSERTION,AT,"\
"AUTHORIZATION,AVG,BEGIN,BETWEEN,BIT,BIT_LENGTH,BOTH,BY,CASCADE,CASCADED,CASE,"\
"CAST,CATALOG,CHAR,CHAR_LENGTH,CHARACTER,CHARACTER_LENGTH,CHECK,CLOSE,COALESCE,"\
"COLLATE,COLLATION,COLUMN,COMMIT,CONNECT,CONNECTION,CONSTRAINT,CONSTRAINTS,"\
"CONTINUE,CONVERT,CORRESPONDING,COUNT,CREATE,CROSS,CURRENT,CURRENT_DATE,"\
"CURRENT_TIME,CURRENT_TIMESTAMP,CURRENT_USER,CURSOR,DATE,DAY,DEALLOCATE,"\
"DEC,DECIMAL,DECLARE,DEFAULT,DEFERRABLE,DEFERRED,DELETE,DESC,DESCRIBE,"\
"DESCRIPTOR,DIAGNOSTICS,DISCONNECT,DISTINCT,DOMAIN,DOUBLE,DROP,ELSE,END,"\
"END-EXEC,ESCAPE,EXCEPT,EXCEPTION,EXEC,EXECUTE,EXISTS,EXTERNAL,EXTRACT,FALSE,"\
"FETCH,FIRST,FLOAT,FOR,FOREIGN,FORTRAN,FOUND,FROM,FULL,GET,GLOBAL,GO,GOTO,"\
"GRANT,GROUP,HAVING,HOUR,IDENTITY,IMMEDIATE,IN,INCLUDE,INDEX,INDICATOR,"\
"INITIALLY,INNER,INPUT,INSENSITIVE,INSERT,INT,INTEGER,INTERSECT,INTERVAL,INTO,"\
"IS,ISOLATION,JOIN,KEY,LANGUAGE,LAST,LEADING,LEFT,LEVEL,LIKE,LOCAL,LOWER,"\
"MATCH,MAX,MIN,MINUTE,MODULE,MONTH,NAMES,NATIONAL,NATURAL,NCHAR,NEXT,NO,NONE,"\
"NOT,NULL,NULLIF,NUMERIC,OCTET_LENGTH,OF,ON,ONLY,OPEN,OPTION,OR,ORDER,OUTER,"\
"OUTPUT,OVERLAPS,PAD,PARTIAL,PASCAL,PLI,POSITION,PRECISION,PREPARE,PRESERVE,"\
"PRIMARY,PRIOR,PRIVILEGES,PROCEDURE,PUBLIC,READ,REAL,REFERENCES,RELATIVE,"\
"RESTRICT,REVOKE,RIGHT,ROLLBACK,ROWS,SCHEMA,SCROLL,SECOND,SECTION,SELECT,"\
"SESSION,SESSION_USER,SET,SIZE,SMALLINT,SOME,SPACE,SQL,SQLCA,SQLCODE,SQLERROR,"\
"SQLSTATE,SQLWARNING,SUBSTRING,SUM,SYSTEM_USER,TABLE,TEMPORARY,THEN,TIME,"\
"TIMESTAMP,TIMEZONE_HOUR,TIMEZONE_MINUTE,TO,TRAILING,TRANSACTION,TRANSLATE,"\
"TRANSLATION,TRIM,TRUE,UNION,UNIQUE,UNKNOWN,UPDATE,UPPER,USAGE,USER,USING,"\
"VALUE,VALUES,VARCHAR,VARYING,VIEW,WHEN,WHENEVER,WHERE,WITH,WORK,WRITE,"\
"YEAR,ZONE"

RETCODE SQL_API SQLBindParameter(SQLHSTMT hstmt,
                                 SQLUSMALLINT ipar,
                                 SQLSMALLINT fParamType,
                                 SQLSMALLINT fCType,
                                 SQLSMALLINT fSqlType,
                                 SQLULEN cbColDef,
                                 SQLSMALLINT ibScale,
                                 PTR rgbValue,
                                 SQLLEN cbValueMax,
                                 SQLLEN * pcbValue);

RETCODE SQL_API SQLBrowseConnect(SQLHDBC hdbc,
                                 SQLTCHAR * szConnStrIn,
                                 SQLSMALLINT cbConnStrIn,
                                 SQLTCHAR * szConnStrOut,
                                 SQLSMALLINT cbConnStrOutMax,
                                 SQLSMALLINT * pcbConnStrOut);

RETCODE SQL_API SQLColAttributes(SQLHSTMT hstmt,
                                 SQLUSMALLINT icol,
                                 SQLUSMALLINT fDescType,
                                 PTR rgbDesc,
                                 SQLSMALLINT cbDescMax,
                                 SQLSMALLINT * pcbDesc,
                                 SQLLEN * pfDesc);

RETCODE SQL_API SQLColumnPrivileges(SQLHSTMT hstmt,
                                    SQLTCHAR * szTableQualifier,
                                    SQLSMALLINT cbTableQualifier,
                                    SQLTCHAR * szTableOwner,
                                    SQLSMALLINT cbTableOwner,
                                    SQLTCHAR * szTableName,
                                    SQLSMALLINT cbTableName,
                                    SQLTCHAR * szColumnName,
                                    SQLSMALLINT cbColumnName);

RETCODE SQL_API SQLDescribeParam(SQLHSTMT hstmt,
                                 SQLUSMALLINT ipar,
                                 SQLSMALLINT * pfSqlType,
                                 SQLULEN * pcbColDef,
                                 SQLSMALLINT * pibScale,
                                 SQLSMALLINT * pfNullable);

RETCODE SQL_API SQLExtendedFetch(SQLHSTMT hstmt,
                                 SQLUSMALLINT fFetchType,
                                 SQLLEN irow,
                                 SQLULEN * pcrow,
                                 SQLUSMALLINT * rgfRowStatus);

RETCODE SQL_API SQLForeignKeys(SQLHSTMT hstmt,
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

RETCODE SQL_API SQLMoreResults(SQLHSTMT hstmt);

RETCODE SQL_API SQLNativeSql(SQLHDBC hdbc,
                             SQLTCHAR * szSqlStrIn,
                             SQLINTEGER cbSqlStrIn,
                             SQLTCHAR * szSqlStr,
                             SQLINTEGER cbSqlStrMax,
                             SQLINTEGER * pcbSqlStr);

RETCODE SQL_API SQLNumParams(SQLHSTMT hstmt, SQLSMALLINT * pcpar);

RETCODE SQL_API SQLParamOptions(SQLHSTMT hstmt,
                                SQLULEN crow,
                                SQLULEN * pirow);

RETCODE SQL_API SQLPrimaryKeys(SQLHSTMT hstmt,
                               SQLTCHAR * szTableQualifier,
                               SQLSMALLINT cbTableQualifier,
                               SQLTCHAR * szTableOwner,
                               SQLSMALLINT cbTableOwner,
                               SQLTCHAR * szTableName,
                               SQLSMALLINT cbTableName);

RETCODE SQL_API SQLProcedureColumns(SQLHSTMT hstmt,
                                    SQLTCHAR * szProcQualifier,
                                    SQLSMALLINT cbProcQualifier,
                                    SQLTCHAR * szProcOwner,
                                    SQLSMALLINT cbProcOwner,
                                    SQLTCHAR * szProcName,
                                    SQLSMALLINT cbProcName,
                                    SQLTCHAR * szColumnName,
                                    SQLSMALLINT cbColumnName);

RETCODE SQL_API SQLProcedures(SQLHSTMT hstmt,
                              SQLTCHAR * szProcQualifier,
                              SQLSMALLINT cbProcQualifier,
                              SQLTCHAR * szProcOwner,
                              SQLSMALLINT cbProcOwner,
                              SQLTCHAR * szProcName,
                              SQLSMALLINT cbProcName);

RETCODE SQL_API SQLSetPos(SQLHSTMT hstmt,
                          SQLSETPOSIROW irow,
                          SQLUSMALLINT fRefresh,
                          SQLUSMALLINT fLock);

RETCODE SQL_API SQLTablePrivileges(SQLHSTMT hstmt,
                                   SQLTCHAR * szTableQualifier,
                                   SQLSMALLINT cbTableQualifier,
                                   SQLTCHAR * szTableOwner,
                                   SQLSMALLINT cbTableOwner,
                                   SQLTCHAR * szTableName,
                                   SQLSMALLINT cbTableName);

#endif /* RC_INVOKED */

/*
 * Deprecated Database name define
 */
#define SQL_DATABASE_NAME               16    /* Use SQLGetConnectOption/SQL_CURRENT_QUALIFIER */

/*
 * Deprecated functions from prior versions of ODBC
 */
#ifndef RC_INVOKED
RETCODE SQL_API SQLSetScrollOptions(SQLHSTMT hstmt,
                                    SQLUSMALLINT fConcurrency,
                                    SQLLEN crowKeyset,
                                    SQLUSMALLINT crowRowset);
#endif /* RC_INVOKED */


#ifdef __cplusplus
}      /* End of extern "C" { */
#endif

#include "sqlucode.h"

#endif /* _SQLEXT_H_ */
