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

#ifndef _CTCBBTRV_H_
#define _CTCBBTRV_H_

#include "ctcboptn.h"
#define USE_API_ENTRYPOINT
#include "ctcbapi.h"
#include "ctcbthrd.h"
/* TODO
#include "ctcblog.h"
*/

#include <stdio.h>
#include <string.h>

typedef struct {
	MUTEX mtx;
	NINT sOWNR;
	UTEXT cliID[4];
} BT_CXID, *pBT_CXID;

#define CX bt_thrd[hThread]

#define BAD_THREAD (hThread < 0)
#define INVALID_THREAD (BAD_THREAD || (!bt_thrd || !CX || !CX->conn))
#define VALIDATE_THREAD(a) \
	{ \
		if (INVALID_THREAD) { \
			CLogError(NULL, NULL, CTE_INTERFACE, CTE_INTERNAL_ERR, 0, (LogBuf, "invalid thread detected: %d", hThread)); \
			a; \
		} \
	}

#define VALID_THREAD !INVALID_THREAD

#define ERROR_RESET if (MT){ MT->_errno=NULL; MT->_int_errno = MT->_int2_errno = 0;}

#define cliID(clientID) (clientID+(16-rtgMIN(16,sizeof(CX->cliID))))

/* size constants */
#define MAX_KEY_SIZE          255

/* chunk constants */
#define XTRACTR_DIRECT_SIGN      0x80000000L
#define XTRACTR_INDIRECT_SIGN    0x80000001L
#define RECTANGLE_DIRECT_SIGN    0x80000002L
#define RECTANGLE_INDIRECT_SIGN  0x80000003L
#define TRUNC_SIGN               0x80000004L
#define APPEND_TO_BLOB           0x20000000L
#define NEXT_IN_BLOB             0x40000000L
#define CHUNK_LOG_SPECIAL        0x00100000L
#define TRUNC_AFTER_UPDATE       0x00400000L
#define PARTS_OF_KEY             0x00800000L
#define CHUNK_NOBIAS_MASK ~(NEXT_IN_BLOB     |\
                            APPEND_TO_BLOB   |\
                            PARTS_OF_KEY     |\
                            TRUNC_AFTER_UPDATE)

/* operation codes */
typedef enum BTRVOP {
	B_OPEN              = 0,
	B_CLOSE             = 1,
	B_INSERT            = 2,
	B_UPDATE            = 3,
	B_DELETE            = 4,
	B_GET_EQUAL         = 5,
	B_GET_NEXT          = 6,
	B_GET_PREVIOUS      = 7,
	B_GET_GT            = 8,
	B_GET_GE            = 9,
	B_GET_LT            = 10,
	B_GET_LE            = 11,
	B_GET_FIRST         = 12,
	B_GET_LAST          = 13,
	B_CREATE            = 14,
	B_STAT              = 15,
	B_EXTEND            = 16,
	B_SET_DIR           = 17,
	B_GET_DIR           = 18,
	B_BEGIN_TRAN        = 19,
	B_END_TRAN          = 20,
	B_ABORT_TRAN        = 21,
	B_GET_POSITION      = 22,
	B_GET_DIRECT        = 23,
	B_STEP_NEXT         = 24,
	B_STOP              = 25,
	B_VERSION           = 26,
	B_UNLOCK            = 27,
	B_RESET             = 28,
	B_SET_OWNER         = 29,
	B_CLEAR_OWNER       = 30,
	B_BUILD_INDEX       = 31,
	B_DROP_INDEX        = 32,
	B_STEP_FIRST        = 33,
	B_STEP_LAST         = 34,
	B_STEP_PREVIOUS     = 35,
	B_GET_NEXT_EXTENDED = 36,
	B_GET_PREV_EXTENDED = 37,
	B_STEP_NEXT_EXT     = 38,
	B_STEP_PREVIOUS_EXT = 39,
	B_EXT_INSERT        = 40,
	B_MISC_DATA         = 41,
	B_CONTINUOUS        = 42,

	B_SEEK_PERCENT      = 44,
	B_GET_PERCENT       = 45,

	B_CHUNK_UPDATE      = 53,

	B_KEY_EQUAL         = 55,
	B_KEY_NEXT          = 56,
	B_KEY_PREV          = 57,
	B_KEY_GT            = 58,
	B_KEY_GE            = 59,
	B_KEY_LT            = 60,
	B_KEY_LE            = 61,
	B_KEY_FIRST         = 62,
	B_KEY_LAST          = 63,

	B_EXTENDED_STAT     = 65,

	B_LOGIN             = 78
} BTRVOP;

enum {
	BTRVOP_POSBLK = 1,
	BTRVOP_FILNAM = 2,
	BTRVOP_ALLINST = 4,
	BTRVOP_SPECIAL = 8
};

/* operation bias codes */
#define S_WAIT_LOCK    100
#define S_NOWAIT_LOCK  200    /* function code bias for lock                */
#define M_WAIT_LOCK    300    /* function code bias for multiple loop lock  */
#define M_NOWAIT_LOCK  400    /* function code bias for multiple lock       */
#define NOWRITE_WAIT   500    /* function code bias when ins/del/upd should */
#define CCURR_T_BIAS  1000    /* function code bias for concurrent trans    */

#define B_LONG_OWNER_NAME_BIAS 17000

/* open modes */
#define B_NORMAL          0x00
#define B_ACCELERATED     0xFF
#define B_READONLY        0xFE
#define B_VERIFY          0xFD
#define B_EXCLUSIVE       0xFC

/* create modes */
#define KEY_BIAS                            50
#define DROP_BUT_NO_RENUMBER                0x80  /* key num bias for Drop  */
                                                  /* Preserves key #s       */
#define CREATE_SUPPLEMENTAL_AS_THIS_KEY_NUM 0x80  /* key bias for Create SI */
#define CREATE_NEW_FILE                     0xFF
#define DONT_CREATE_WITH_TTS                0xFE
#define CREATE_NEW_FILE_NO_TTS              0xFD

#define IGNORE_KEY                          0xFFFF   /* ignore the key number */
#define ALTERNATE_STAT_BUF                  0xFFFF         /* use with B_STAT */

/* file flags */
#define VAR_RECS                0x0001
#define BLANK_TRUNC             0x0002
#define PRE_ALLOC               0x0004
#define DATA_COMP               0x0008
#define KEY_ONLY                0x0010
#define BALANCED_KEYS           0x0020
#define FREE_10                 0x0040
#define FREE_20                 0x0080
#define FREE_30                 ( FREE_10 | FREE_20 )
#define DUP_PTRS                0x0100
#define INCLUDE_SYSTEM_DATA     0x0200
#define SPECIFY_KEY_NUMS        0x0400
#define VATS_SUPPORT            0x0800
#define NO_INCLUDE_SYSTEM_DATA  0x1200
#define PAGE_COMPRESSED         0x2000

/* key flags */
#define DUP             0x0001                   /* Duplicates allowed mask */
#define MOD             0x0002                       /* Modifiable key mask */
#define BIN             0x0004          /* Binary or extended key type mask */
#define NUL             0x0008                             /* Null key mask */
#define SEG             0x0010                        /* Segmented key mask */
#define ALT             0x0020         /* Alternate collating sequence mask */
#define NUMBERED_ACS    0x0420                  /* Use Numbered ACS in file */
#define NAMED_ACS       0x0C20                     /* Use Named ACS in file */

#define DESC_KEY        0x0040                /* Key stored descending mask */
#define REPEAT_DUPS_KEY 0x0080            /* Dupes handled w/ unique suffix */
#define EXTTYPE_KEY     0x0100          /* Extended key types are specified */
#define MANUAL_KEY      0x0200   /* Manual key which can be optionally null */
                                        /* (then key is not inc. in B-tree) */
#define NOCASE_KEY      0x0400                      /* Case insensitive key */
#define KEYONLY_FILE    0x4000                        /* key only type file */
#define PENDING_KEY     0x8000         /* Set during a create or drop index */
#define ALLOWABLE_KFLAG_PRE6 0x037F          /* (before ver 6.0, no nocase. */

/* key types */
#define STRING_TYPE           0
#define INTEGER_TYPE          1
#define IEEE_TYPE             2
#define DATE_TYPE             3
#define TIME_TYPE             4
#define DECIMAL_TYPE          5
#define MONEY_TYPE            6
#define LOGICAL_TYPE          7
#define NUMERIC_TYPE          8
#define BFLOAT_TYPE           9
#define LSTRING_TYPE         10
#define ZSTRING_TYPE         11
#define UNSIGNED_BINARY_TYPE 14
#define AUTOINCREMENT_TYPE   15
#define BIT_TYPE             16
#define NUMERICSTS_TYPE      17
#define NUMERICSA_TYPE       18
#define CURRENCY_TYPE        19
#define TIMESTAMP_TYPE       20
#define WSTRING_TYPE         25
#define WZSTRING_TYPE        26
#define GUID_TYPE            27
#define NUMERICSLB_TYPE      28
#define NUMERICSLS_TYPE      29
#define DATETIME_TYPE        30
#define NUMERICSTB_TYPE      31

#define NULL_INDICATOR_TYPE  255

/* signature types */
#define ALT_ID               0xAC
#define COUNTRY_CODE_PAGE_ID 0xAD
#define ISR_ID               0xAE

/* return codes */
#define B_NO_ERROR                          0
#define B_INVALID_FUNCTION                  1
#define B_IO_ERROR                          2
#define B_FILE_NOT_OPEN                     3
#define B_KEY_VALUE_NOT_FOUND               4
#define B_DUPLICATE_KEY_VALUE               5
#define B_INVALID_KEYNUMBER                 6
#define B_DIFFERENT_KEYNUMBER               7
#define B_POSITION_NOT_SET                  8
#define B_END_OF_FILE                       9
#define B_MODIFIABLE_KEYVALUE_ERROR         10
#define B_FILENAME_BAD                      11
#define B_FILE_NOT_FOUND                    12
#define B_EXTENDED_FILE_ERROR               13
#define B_PREIMAGE_OPEN_ERROR               14
#define B_PREIMAGE_IO_ERROR                 15
#define B_EXPANSION_ERROR                   16
#define B_CLOSE_ERROR                       17
#define B_DISKFULL                          18
#define B_UNRECOVERABLE_ERROR               19
#define B_RECORD_MANAGER_INACTIVE           20
#define B_KEYBUFFER_TOO_SHORT               21
#define B_DATALENGTH_ERROR                  22
#define B_POSITIONBLOCK_LENGTH              23
#define B_PAGE_SIZE_ERROR                   24
#define B_CREATE_IO_ERROR                   25
#define B_NUMBER_OF_KEYS                    26
#define B_INVALID_KEY_POSITION              27
#define B_INVALID_RECORD_LENGTH             28
#define B_INVALID_KEYLENGTH                 29
#define B_NOT_A_BTRIEVE_FILE                30
#define B_FILE_ALREADY_EXTENDED             31
#define B_EXTEND_IO_ERROR                   32
#define B_BTR_CANNOT_UNLOAD                 33
#define B_INVALID_EXTENSION_NAME            34
#define B_DIRECTORY_ERROR                   35
#define B_TRANSACTION_ERROR                 36
#define B_TRANSACTION_IS_ACTIVE             37
#define B_TRANSACTION_FILE_IO_ERROR         38
#define B_END_TRANSACTION_ERROR             39
#define B_TRANSACTION_MAX_FILES             40
#define B_OPERATION_NOT_ALLOWED             41
#define B_INCOMPLETE_ACCEL_ACCESS           42
#define B_INVALID_RECORD_ADDRESS            43
#define B_NULL_KEYPATH                      44
#define B_INCONSISTENT_KEY_FLAGS            45
#define B_ACCESS_TO_FILE_DENIED             46
#define B_MAXIMUM_OPEN_FILES                47
#define B_INVALID_ALT_SEQUENCE_DEF          48
#define B_KEY_TYPE_ERROR                    49
#define B_OWNER_ALREADY_SET                 50
#define B_INVALID_OWNER                     51
#define B_ERROR_WRITING_CACHE               52
#define B_INVALID_INTERFACE                 53
#define B_VARIABLE_PAGE_ERROR               54
#define B_AUTOINCREMENT_ERROR               55
#define B_INCOMPLETE_INDEX                  56
#define B_EXPANED_MEM_ERROR                 57
#define B_COMPRESS_BUFFER_TOO_SHORT         58
#define B_FILE_ALREADY_EXISTS               59
#define B_REJECT_COUNT_REACHED              60
#define B_SMALL_EX_GET_BUFFER_ERROR         61
#define B_INVALID_GET_EXPRESSION            62
#define B_INVALID_EXT_INSERT_BUFF           63
#define B_OPTIMIZE_LIMIT_REACHED            64
#define B_INVALID_EXTRACTOR                 65
#define B_RI_TOO_MANY_DATABASES             66
#define B_RIDDF_CANNOT_OPEN                 67
#define B_RI_CASCADE_TOO_DEEP               68
#define B_RI_CASCADE_ERROR                  69
#define B_RI_VIOLATION                      71
#define B_RI_REFERENCED_FILE_CANNOT_OPEN    72
#define B_RI_OUT_OF_SYNC                    73
#define B_END_CHANGED_TO_ABORT              74
#define B_RI_CONFLICT                       76
#define B_CANT_LOOP_IN_SERVER               77
#define B_DEAD_LOCK                         78
#define B_PROGRAMMING_ERROR                 79
#define B_CONFLICT                          80
#define B_LOCKERROR                         81
#define B_LOST_POSITION                     82
#define B_READ_OUTSIDE_TRANSACTION          83
#define B_RECORD_INUSE                      84
#define B_FILE_INUSE                        85
#define B_FILE_TABLE_FULL                   86
#define B_NOHANDLES_AVAILABLE               87
#define B_INCOMPATIBLE_MODE_ERROR           88

#define B_DEVICE_TABLE_FULL                 90
#define B_SERVER_ERROR                      91
#define B_TRANSACTION_TABLE_FULL            92
#define B_INCOMPATIBLE_LOCK_TYPE            93
#define B_PERMISSION_ERROR                  94
#define B_SESSION_NO_LONGER_VALID           95
#define B_COMMUNICATIONS_ERROR              96
#define B_DATA_MESSAGE_TOO_SMALL            97
#define B_INTERNAL_TRANSACTION_ERROR        98
#define B_REQUESTER_CANT_ACCESS_RUNTIME     99
#define B_NO_CACHE_BUFFERS_AVAIL            100
#define B_NO_OS_MEMORY_AVAIL                101
#define B_NO_STACK_AVAIL                    102
#define B_CHUNK_OFFSET_TOO_LONG             103
#define B_LOCALE_ERROR                      104
#define B_CANNOT_CREATE_WITH_BAT            105
#define B_CHUNK_CANNOT_GET_NEXT             106
#define B_CHUNK_INCOMPATIBLE_FILE           107

#define B_TRANSACTION_TOO_COMPLEX           109

#define B_ARCH_BLOG_OPEN_ERROR              110
#define B_ARCH_FILE_NOT_LOGGED              111
#define B_ARCH_FILE_IN_USE                  112
#define B_ARCH_LOGFILE_NOT_FOUND            113
#define B_ARCH_LOGFILE_INVALID              114
#define B_ARCH_DUMPFILE_ACCESS_ERROR        115
#define B_LOCATOR_FILE_INDICATOR            116

#define B_NO_SYSTEM_LOCKS_AVAILABLE         130
#define B_FILE_FULL                         132
#define B_MORE_THAN_5_CONCURRENT_USERS      133

#define B_ISR_READ_ERROR                    134  /* Old definition     */
#define B_ISR_NOT_FOUND                     134  /* New definition     */

#define B_ISR_FORMAT_INVALID                135  /* Old definition     */
#define B_ISR_INVALID                       135  /* New definition     */
#define B_ACS_NOT_FOUND                     136
#define B_CANNOT_CONVERT_RP                 137
#define B_INVALID_NULL_INDICATOR            138
#define B_INVALID_KEY_OPTION                139
#define B_INCOMPATIBLE_CLOSE                140
#define B_INVALID_USERNAME                  141
#define B_INVALID_DATABASE                  142
#define B_NO_SSQL_RIGHTS                    143
#define B_ALREADY_LOGGED_IN                 144
#define B_NO_DATABASE_SERVICES              145
#define B_DUPLICATE_SYSTEM_KEY              146
#define B_LOG_SEGMENT_MISSING               147
#define B_ROLL_FORWARD_ERROR                148
#define B_SYSTEM_KEY_INTERNAL               149
#define B_DBS_INTERNAL_ERROR                150
#define B_NESTING_DEPTH_ERROR               151

#define B_INVALID_PARAMETER_TO_MKDE         160

/* User Count Manager Return code */
#define B_USER_COUNT_LIMIT_EXCEEDED         161

#define B_CLIENT_TABLE_FULL                 162
#define B_LAST_SEGMENT_ERROR                163

/* Btrieve Login API related codes */
#define B_LOGIN_FAILED_BAD_USERNAME         170
#define B_LOGIN_FAILED_BAD_PASSWORD         171
#define B_LOGIN_FAILED_BAD_DATABASE         172
#define B_LOGIN_ALREADY_LOGGED_IN           173
#define B_LOGIN_LOGOUT_FAILED               174
#define B_LOGIN_WRONG_URI_FORMAT            175
#define B_LOGIN_FILE_AND_TABLE_NOT_FOUND    176
#define B_LOGIN_TABLE_NOT_IN_DATABASE       177
#define B_LOGIN_DIRECTORY_NOT_IN_DATABASE   178

#define B_MK_INVALID_SESSION               3006
#define B_MK_INVALID_CONFIG                3008

typedef enum RECPOS_TYPE {
	FILOFF4,
	FILOFF8,
	SRLNUM4
} RECPOS_TYPE;

typedef struct
{
	CT_FILE	*file;
	RECPOS_TYPE	recpos;
	ULONG inrec_offset;
	ULONG inrec_recnum;
} POS_BLOCK;

#define POS_BLOCK_SIZE        128

#pragma pack(1)
typedef struct
{
	UCOUNT recLength;
	UCOUNT pageSize;
	UTEXT indexCount;
	UTEXT version;
	TEXT  reservedInt[4];
	UCOUNT flags;
	UTEXT duplicatePtrs;
	TEXT  reservedByte;
	COUNT preallocPages;
} B_FILE_SPECS;

typedef struct
{
	COUNT position;
	COUNT length;
	UCOUNT flags;
	TEXT  reservedInt[4];
	UTEXT  type;
	TEXT  nullValue;
	TEXT  reservedShort[2];
	UTEXT manualKeyNumber;
	UTEXT acsNumber;
} B_KEY_SPECS;

typedef struct
{
	UTEXT sig;
	TEXT  name[8];
	UTEXT acs[256];
} B_ACS_SPECS;

typedef struct
{
	UTEXT sig;
	TEXT  name[16];
	UTEXT acs[256];
} B_ISR_SPECS;

typedef struct
{
	UCOUNT length;
	TEXT  begin[2];
} B_GETEXT_HEAD_SPECS;

typedef struct
{
	UCOUNT fltmaxrej;
	UCOUNT fltnterms;
} B_GETEXT_FILT_SPECS;

typedef struct
{
	UTEXT fldtype;
	UCOUNT fldlength;
	UCOUNT fldoffset;
	UTEXT comparison;
	UTEXT more;
} B_GETEXT_FILTFLD_SPECS;

typedef struct
{
	UCOUNT nbrofrecords;
	UCOUNT nbroffields;
} B_GETEXT_DESC_SPECS;

typedef struct
{
	UCOUNT length;
	UCOUNT offset;
} B_GETEXT_DESCFLD_SPECS;

typedef struct
{
	TEXT sig[4];
	ULONG subfunction;
} B_STATEXT_SPECS;

/* Platform-Dependent Definitions for BTI_FAR and BTI_PASCAL */
#define BTI_FAR

typedef unsigned long         BTI_ULONG;

/* Platform-Independent Data Types */
typedef unsigned char         BTI_BYTE;
typedef BTI_BYTE     BTI_FAR* BTI_BUFFER_PTR;

typedef struct
{
	BTI_ULONG ChunkOffset;
	BTI_ULONG ChunkLen;
	BTI_BUFFER_PTR dataP;       /* When used w/ "direct" api, init to NULL. */
} CHUNK;

typedef struct {
	BTI_ULONG      RecordAddress;
	BTI_ULONG      Signature;
	BTI_ULONG      NumChunks;
	CHUNK          Chunk[1];
}  GET_XTRACTR;

typedef struct {
	BTI_ULONG RecordAddress;
	BTI_ULONG Signature;
	BTI_ULONG NumRows;
	BTI_ULONG Offset;
	BTI_ULONG BytesPerRow;
	BTI_ULONG BtrDistanceBetweenRows; /* Btrieve's count of bytes between */
									/* beginning of two consecutive rows. */
	BTI_BUFFER_PTR dataP;
	BTI_ULONG AppDistanceBetweenRows;     /* App's count of bytes between */
									/* beginning of two consecutive rows. */
} GET_RECTANGLE;

typedef struct {
	BTI_ULONG          Signature;        /* XTRACTR_DIRECT_SIGN */
	BTI_ULONG          NumChunks;
	CHUNK          Chunk[1];
}  PUT_XTRACTR;

typedef struct {
	BTI_ULONG Signature;                         /* RECTANGLE_DIRECT_SIGN */
	BTI_ULONG NumRows;
	BTI_ULONG Offset;
	BTI_ULONG BytesPerRow;
	BTI_ULONG BtrDistanceBetweenRows; /* Btrieve's count of bytes between */
									/* beginning of two consecutive rows. */
	BTI_BUFFER_PTR dataP;
	BTI_ULONG AppDistanceBetweenRows;     /* App's count of bytes between */
									   /* beginning of two consecutive rows.     */
} PUT_RECTANGLE;

typedef struct {
	BTI_ULONG Signature;        /* TRUNC_SIGN */
	BTI_ULONG ChunkOffset;
}  PUT_TRUNC;
#pragma pack()

#ifdef ctPortWIN32
#define BTI_API ctCONV COUNT WINAPI
#else
#define BTI_API ctCONV COUNT ctDECL
#endif

/* must be exported in .DEF with DLL ordinal #2 */
BTI_API BTRCALLID(
	UCOUNT  operation,
	pVOID   posBlock,
	pVOID   dataBuffer,
	pULONG  dataLength,
	pVOID   keyBuffer,
	UTEXT   keyLength,
	TEXT    keyNumber,
	pUTEXT  clientID);
/* must be exported in .DEF with DLL ordinal #1 */
BTI_API BTRCALL(
	UCOUNT  operation,
	pVOID   posBlock,
	pVOID   dataBuffer,
	pULONG  dataLength,
	pVOID   keyBuffer,
	UTEXT   keyLength,
	TEXT    keyNumber);
cbDLLexport
BTI_API BTRVID(
	UCOUNT  operation,
	pVOID   posBlock,
	pVOID   dataBuffer,
	pUCOUNT dataLength,
	pVOID   keyBuffer,
	COUNT   keyNumber,
	pUTEXT  clientID);
cbDLLexport
BTI_API BTRV(
	UCOUNT  operation,
	pVOID   posBlock,
	pVOID   dataBuffer,
	pUCOUNT dataLength,
	pVOID   keyBuffer,
	COUNT   keyNumber);
/* must be exported in .DEF with DLL ordinal #6 */
BTI_API WBTRVSTOP(VOID);

COUNT btMapError(LONG logical, LONG ctree, LONG system);

#endif /* ~_CTCBBTRV_H_ */

/* end of ctcbbtrv.h */
