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

#ifndef _CTCBERR_H_
#define _CTCBERR_H_

#include "ctdbsdk.h"
#include <errno.h>

typedef struct {
	COUNT logical;
	COUNT padERR1;
	LONG ctree;
	LONG system;
} CT_ERROR, *pCT_ERROR;

/*
** error codes for CT_ERROR.logical
*/
#define CTE_NO_ERROR         0
#define CTE_SYS_ERR          1    /* 30 */
#define CTE_PARAM_ERR        2    /* 39 */
#define CTE_TOO_MANY_FILES   3    /* 94 */
#define CTE_MODE_CLASH       4    /* 9} */
#define CTE_REC_LOCKED       5    /* 99 */
#define CTE_BROKEN           6    /* 9i */
#define CTE_DUPLICATE        7    /* 22 */
#define CTE_NOT_FOUND        8    /* 10 */
#define CTE_UNDEF_RECORD     9    /* 46 */
#define CTE_DISK_FULL       10    /* 9¦ */
#define CTE_FILE_LOCKED     11    /* 93 */
#define CTE_MISMATCH        13    /* 9? */
#define CTE_NO_MEMORY       14    /* 9t */
#define CTE_MISSING_FILE    15    /* 35 */
#define CTE_PERMISSION      16    /* 37 */
#define CTE_NO_SUPPORT      17    /* 9~ */
#define CTE_INTERFACE       19    /* 9D */
#define	CTE_LICENSE_ERR     20
#define	CTE_UNKNOWN_ERR     21
#define	CTE_TRANSACTION     22

#define CTE_MODE_CLASH_W    23
#define CTE_MODE_CLASH_RW   24
#define CTE_AT_END          25
#define CTE_SYNTAX_ERR      31
#define CTE_CONFIG_ERR      32
#define CTE_ENCRYPTED       33
#define CTE_REDIRINST       34
#define CTE_NOT_SQL         35
#define CTE_MISSING_TABLE   36
#define CTE_INCOMPATIBLE    37
#define CTE_FILE_EXISTS     38
#define CTE_NO_TRANSACTION  39
#define CTE_NOT_MODIFIABLE  40
#define CTE_NOT_CTREE_FILE  41
#define CTE_TRAN_EXIST      42
#define CTE_XDD_RULE_ERR    43
#define CTE_MISSING_IDXFILE 44
#define CTE_ALREADY_OPEN    45
#define CTE_ALREADY_CLOSE   46
#define CTE_DELETE_OPEN     47
#define CTE_CLOSE_WITH_LOCK 48
#define CTE_OPEN_MISSING    49
#define CTE_CHECK_DEFS_ERR  50
#define CTE_FILE_SIZE_ERR   51
#define CTE_ORPHAN_IDXFILE  52
#define CTE_DATABUFFER_ERR  53
#define CTE_NOT_RTG_FILE    54
#define CTE_DISCONNECT_ERR  55

#define	CTE_NOT_ME          99
#define	CTW_NO_SUPPORT     100
#define	CTW_DUP_OK         101
#define CTW_FILE_CREATED   102

#define CTE_FILTER_REDO   1001
#define CTE_FILTER_EOF    1002

/*
** error codes for CT_ERROR.ctree
*/
#define CTE_INTERNAL_ERR   (-1)
#define CTE_EXITING_THRD   (-2)
#define CTE_OLD_CLIENT     (-3)
#define CTE_OLD_SERVER     (-4)
#define CTE_INVALID_HANDLE (-5)
#define CTE_ENDIANNESS_ERR (-6)
/* the following must be the lower number for internal cobol driver errors */
#define CTE_INTERNAL_LAST  (-100)
/*
**	error messages
*/
#define F_ERRMSG_LEN	256

#define CTE_INTERNAL_ERRMSG "Internal error"

static cpTEXT ctcb_error[] = {
	/* CTE_NO_ERROR */          "Operation completed successfully",
	/* CTE_SYS_ERR */           "System error",
	/* CTE_PARAM_ERR */         "Parameter not correct",
	/* CTE_TOO_MANY_FILES */    "Too many files open. Check the FILES keyword in configuration file",
	/* CTE_MODE_CLASH */        "File is open read-only",
	/* CTE_REC_LOCKED */        "Record locked by another user",
	/* CTE_BROKEN */            "File is corrupt. Rebuild it or restore backup",
	/* CTE_DUPLICATE */         "Duplicate record not allowed",
	/* CTE_NOT_FOUND */         "Record not found",
	/* CTE_UNDEF_RECORD */      "Record position not set",
	/* CTE_DISK_FULL */         "Write error",
	/* CTE_FILE_LOCKED */       "File locked by another user",
	/* 12 */                    "",
	/* CTE_MISMATCH */          "File definition mismatch",
	/* CTE_NO_MEMORY */         "Out of memory",
	/* CTE_MISSING_FILE */      "File not found",
	/* CTE_PERMISSION */        "Not enough permissions",
	/* CTE_NO_SUPPORT */        "Unsupported function",
	/* 18 */                    "",
	/* CTE_INTERFACE */         "Interface error",
	/* 20 */                    "",
	/* 21 */                    "",
	/* 22 */                    "",
	/* CTE_MODE_CLASH_W */      "File is open write-only",
	/* CTE_MODE_CLASH_RW */     "File is not open for read and write",
	/* CTE_AT_END */            "End of file",
	/* 26 */                    "",
	/* 27 */                    "",
	/* 28 */                    "",
	/* 29 */                    "",
	/* 30 */                    "",
	/* CTE_SYNTAX_ERR */        "Configuration syntax error",
	/* CTE_CONFIG_ERR */        "Configuration error",
	/* CTE_ENCRYPTED */         "Operation not allowed. File is encrypted",
	/* 34 */                    "",
	/* CTE_NOT_SQL */           "Server/database is not SQL",
	/* CTE_MISSING_TABLE */     "SQL table not found",
	/* CTE_INCOMPATIBLE */      "Client/server incompatibility",
	/* CTE_FILE_EXISTS */       "File already exists",
	/* CTE_NO_TRANSACTION */    "No active transaction",
	/* CTE_NOT_MODIFIABLE */    "Key is not modifiable",
	/* CTE_NOT_CTREE_FILE */    "Unknown file format",
	/* CTE_TRAN_EXIST */        "Transaction already active",
	/* CTE_XDD_RULE_ERR */      "XDD rule error",
	/* CTE_MISSING_IDXFILE */   "Missing index file",
	/* CTE_ALREADY_OPEN */      "File is already open",
	/* CTE_ALREADY_CLOSE */     "File is already close",
	/* CTE_DELETE_OPEN */       "File is open",
	/* CTE_CLOSE_WITH_LOCK */   "File was closed with lock",
	/* CTE_OPEN_MISSING */      "File not found",
	/* CTE_CHECK_DEFS_ERR */    "File definition error",
	/* CTE_FILE_SIZE_ERR */     "File size exceeds limit",
	/* CTE_ORPHAN_IDXFILE */    "File is corrupted. Remove orphan index file",
	/* CTE_DATABUFFER_ERR */    "Data buffer is too short",
	/* CTE_NOT_RTG_FILE */      "File is not compatible with RTG",
	""
};

#define CTDBRET_CALLBACK_1_MSG  "Could not find schema in XDD"
#define CTDBRET_CALLBACK_2_MSG  "Record length does not match XDD information"
#define CTDBRET_CALLBACK_3_MSG  "Invalid or corrupted XDD resource"
#define CTDBRET_CALLBACK_4_MSG  "Syntax error parsing XDD resource"
#define CTDBRET_CALLBACK_5_MSG  "Internal error"
#define CTDBRET_CALLBACK_6_MSG  "Could not find field in XDD"
#define CTDBRET_CALLBACK_7_MSG  "Could not find filter in XDD"
#define CTDBRET_CALLBACK_8_MSG  "Too many schemas in XDD"
#define CTDBRET_CALLBACK_9_MSG  "Error setting filter condition on schema"
#define CTDBRET_CALLBACK_10_MSG "Field value conversion error on %s"
#define CTDBRET_CALLBACK_11_MSG "Unsupported CLOB/BLOB definition in XDD"
#define CTDBRET_CALLBACK_12_MSG "Index not supported on field type"
#define CTDBRET_CALLBACK_13_MSG "Missing or bad dbtype information in XDD"
#define CTDBRET_CALLBACK_14_MSG "Invalid filter key settings in XDD"
#define CTDBRET_CALLBACK_15_MSG "Invalid field after fields with condition in XDD"
#define CTDBRET_CALLBACK_16_MSG "Index creation requires rowid support (or fastdup turned on)"
#define CTDBRET_CALLBACK_17_MSG "Key definition does not match XDD information"
#define CTDBRET_CALLBACK_18_MSG "Missing or bad value in XDD"
#define CTDBRET_CALLBACK_19_MSG "Internal error (unexpected field type)"
#define CTDBRET_CALLBACK_20_MSG "" /* available for use */

struct CTUTIL_INT_ERROR {
	const NINT num;
	cpTEXT msg;
} static const ctcb_int_error[] = {
	/* num                  msg */
	{IDRK_ERR,              "Too many keys for data file"},
	{IKRS_ERR,              "Too many key parts in index"},
	{ARQS_ERR,              "Server connection was interrupted" },
	{ARSP_ERR,              "Server connection was interrupted" },
	{ASKY_ERR,              "Server is not active or not reacheable" },
	{SHUT_ERR,              "Server is shutting down" },
	{RNOT_ERR,              "Could not find extended data definitions" },
	{DZRO_ERR,              "Could not find extended data definitions" },
	{DEFP_ERR,              "User does not have permission to redefine the file" },
	{LUID_ERR,              "Invalid user" },
	{LPWD_ERR,              "Invalid password" },
	{SACS_ERR,              "User does not have permission to access the file" },
	{SWRT_ERR,              "User does not have permission to write to file" },
	{SDLT_ERR,              "User does not have permission to delete the file" },
	{LGST_ERR,              "Invalid user" },
	{DUPJ_ERR,              "Duplicate records successfully purged" },
	{DUPL_ERR,              "Unexpected duplicates found and listed in file logged in CTSTATUS.FCS"},
	{LDPA_ERR,              "User authentication failed" },
	{SSLCONN_ERR,           "Could not establish transport layer security connection"},
	{DCPT_ERR,              "Damaged data records detected. Restore data file from backup or use -repairdata and attempt to restore to a functional state"},
	{CTDBRET_NOSUCHFIELD,   "Cannot identify field for key part"},
	{CTDBRET_FIELDEXIST,    "Field already exists"},
	{CTDBRET_INVTYPE,       "Invalid field type"},
	{CTDBRET_TABLEEXIST,    "Table already exists"},
	{CTDBRET_INTERNAL,      "CTDB internal error"},
	{CTDBRET_INDEXEXIST,    "Index already exists"},
	{CTDBRET_NOTSUPPORTED,  "Feature not supported"},
	{CTDBRET_NOSUCHDATABASE,"Database does not exist"},
	{CTDBRET_CALLBACK_1,    CTDBRET_CALLBACK_1_MSG},
	{CTDBRET_CALLBACK_2,    CTDBRET_CALLBACK_2_MSG},
	{CTDBRET_CALLBACK_3,    CTDBRET_CALLBACK_3_MSG},
	{CTDBRET_CALLBACK_4,    CTDBRET_CALLBACK_4_MSG},
	{CTDBRET_CALLBACK_5,    CTDBRET_CALLBACK_5_MSG},
	{CTDBRET_CALLBACK_6,    CTDBRET_CALLBACK_6_MSG},
	{CTDBRET_CALLBACK_7,    CTDBRET_CALLBACK_7_MSG},
	{CTDBRET_CALLBACK_8,    CTDBRET_CALLBACK_8_MSG},
	{CTDBRET_CALLBACK_9,    CTDBRET_CALLBACK_9_MSG},
	{CTDBRET_CALLBACK_10,   CTDBRET_CALLBACK_10_MSG},
	{CTDBRET_CALLBACK_11,   CTDBRET_CALLBACK_11_MSG},
	{CTDBRET_CALLBACK_12,   CTDBRET_CALLBACK_12_MSG},
	{CTDBRET_CALLBACK_13,   CTDBRET_CALLBACK_13_MSG},
	{CTDBRET_CALLBACK_14,   CTDBRET_CALLBACK_14_MSG},
	{CTDBRET_CALLBACK_15,   CTDBRET_CALLBACK_15_MSG},
	{CTDBRET_CALLBACK_16,   CTDBRET_CALLBACK_16_MSG},
	{CTDBRET_CALLBACK_17,   CTDBRET_CALLBACK_17_MSG},
	{CTDBRET_CALLBACK_18,   CTDBRET_CALLBACK_18_MSG},
	{CTDBRET_CALLBACK_19,   CTDBRET_CALLBACK_19_MSG},
	{CTDBRET_CALLBACK_20,   CTDBRET_CALLBACK_20_MSG},
	{CTDBRET_RESERVEDNAME,  "Invalid use of reserved name"},
	{CTDBRET_INVFILENAME,   "File name contains invalid character"},
	{CTDBRET_INVRTGXDD,     "Invalid XDD"},
	{CTDB_ERR_FLDCNTHI,     "Too many overall index parts"},
	{CTDB_ERR_IDXCNTHI,     "Too many fields specified"},
	{CTDB_ERR_ID_OVFLOW,    "Identifier (table/field/owner) too long"},
};

#define CLEAR_ERROR(e) { e.system = e.ctree = e.logical = 0 ; }
#define SET_ERROR(e,l,c) { e.logical = l ; e.ctree = c ; e.system = 0 ; }

#endif /* ~_CTCBERR_H_ */

/* end of ctcberr.h */
