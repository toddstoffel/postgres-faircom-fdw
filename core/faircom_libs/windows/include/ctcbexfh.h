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

#ifndef _CTCBEXFH_H_
#define _CTCBEXFH_H_

#include "ctcboptn.h"
#define USE_API_ENTRYPOINT
#include "ctcbapi.h"
#include "ctcblog.h"
#include "ctcbthrd.h"
#include "ctcbswtc.h"

#include <stdio.h>
#include <string.h>

#define EXTFH_LOGFCD /* turns on FCD information logging */
#ifdef ctN8
#define EXTFH_PROFILE /* turns on PROFILE logging in EXTFH */
#endif
#define EXTFH_SWITCHER /* turns on extfh file handler switcher */
#define EXTFH_LOGSWITCH /* turns on EXTFH_SWITCH logging*/
#define EXTFH_FCD_SIZE_CHECK /* turns on fcd size verification */

#ifdef EXTFH_LOGFCD
#define MAX_FCD_DUMP_SIZE 15000
#endif

#ifndef TRUE
#define TRUE 1
#endif
#ifndef FALSE
#define FALSE 0
#endif

#define MAX_PARMS 1024

#define FCD_bare_min_len 5  /*5 because of GET_FCD_VERS*/

#define BIT_0 0x01
#define BIT_1 0x02
#define BIT_2 0x04
#define BIT_3 0x08
#define BIT_4 0x10
#define BIT_5 0x20
#define BIT_6 0x40
#define BIT_7 0x80

typedef struct {
	CT_INIT	*conn;
	NINT	status;
	char	*_errno;
	long	_int_errno;
	long	_int2_errno;
	ULONG  open_files;
	TEXT   trans_active;
} MT_INIT;

#define MT ct_connection
#define CONN ((MT)?MT->conn:NULL)
#define INVALID_CONN (!MT || !MT->conn)


#define RETURN_PNT(p) ct2ANSI85p(p)
#define SE(a,b) case a: { ansi_err = b; break;}
#define ERROR_RESET if (MT){ MT->_errno=NULL; MT->_int_errno = MT->_int2_errno = 0;}


/* OPCODES */
/* first byte */
#define EXTFH_OPCODE_STANDARD              (UTEXT)0xfa
#define EXTFH_OPCODE_SPECIAL               (UTEXT)0x00
#define EXTFH_OPCODE_SPECIAL_CTREE         (UTEXT)0xc3
#define EXTFH_OPCODE_SPECIAL_UNDOC         (UTEXT)0xff
#define EXTFH_OPCODE_SPECIAL_UNDOC2        (UTEXT)0x08

/*STANDARD OPCODE byte */
#define EXTFH_OPENMODE_INPUT               (UTEXT)0x00
#define EXTFH_OPENMODE_OUTPUT              (UTEXT)0x01
#define EXTFH_OPENMODE_IO                  (UTEXT)0x02
#define EXTFH_OPENMODE_EXTEND              (UTEXT)0x03
/* DO_NOT_APPLY_TO_INDEXED { */
#define EXTFH_OPENMODE_INPUT_NO_REWIND     (UTEXT)0x04
#define EXTFH_OPENMODE_OUTPUT_NO_REWIND    (UTEXT)0x05
#define EXTFH_OPENMODE_INPUT_REVERSED      (UTEXT)0x08
/* } DO_NOT_APPLY_TO_INDEXED */

#define EXTFH_CLOSEMODE_CLOSE              (UTEXT)0x80
#define EXTFH_CLOSEMODE_WITHLOCK           (UTEXT)0x81
#ifdef DO_NOT_APPLY_TO_INDEXED
#define EXTFH_CLOSEMODE_NO_REWIND          (UTEXT)0x82
#define EXTFH_CLOSEMODE_REEL               (UTEXT)0x84
#define EXTFH_CLOSEMODE_REEL_REMOVAL       (UTEXT)0x85
#define EXTFH_CLOSEMODE_REEL_NO_REWIND     (UTEXT)0x86
#endif

#define EXTFH_STARTMODE_EQUAL_PRIME        (UTEXT)0xe8
#define EXTFH_STARTMODE_EQUAL_ANY          (UTEXT)0xe9
#define EXTFH_STARTMODE_GREATER            (UTEXT)0xea
#define EXTFH_STARTMODE_NOT_LESS           (UTEXT)0xeb
#define EXTFH_STARTMODE_LESS               (UTEXT)0xfe
#define EXTFH_STARTMODE_NOT_GREATER        (UTEXT)0xff

#define EXTFH_READMODE_PREV_WITHNOLOCK     (UTEXT)0x8c
#define EXTFH_READMODE_SEQ_WITHNOLOCK      (UTEXT)0x8d
#define EXTFH_READMODE_RAND_WITHNOLOCK     (UTEXT)0x8e
#define EXTFH_READMODE_DIR_WITHNOLOCK      (UTEXT)0x8f
#define EXTFH_STEPMODE_NEXT_WITHNOLOCK     (UTEXT)0x90
#define EXTFH_STEPMODE_PREV_WITHNOLOCK     (UTEXT)0x91
#define EXTFH_STEPMODE_FIRST_WITHNOLOCK    (UTEXT)0x92
#define EXTFH_STEPMODE_LAST_WITHNOLOCK     (UTEXT)0x93

#define EXTFH_READMODE_DIR                 (UTEXT)0xc9
#define EXTFH_STEPMODE_NEXT                (UTEXT)0xca
#define EXTFH_STEPMODE_PREV                (UTEXT)0xcb
#define EXTFH_STEPMODE_FIRST               (UTEXT)0xcc
#define EXTFH_STEPMODE_LAST                (UTEXT)0xcd

#define EXTFH_STEPMODE_LAST_WITHLOCK       (UTEXT)0xce
#define EXTFH_STEPMODE_LAST_WITHKEPTLOCK   (UTEXT)0xcf
#define EXTFH_STEPMODE_FIRST_WITHLOCK      (UTEXT)0xd0
#define EXTFH_STEPMODE_FIRST_WITHKEPTLOCK  (UTEXT)0xd1
#define EXTFH_STEPMODE_PREV_WITHLOCK       (UTEXT)0xd2
#define EXTFH_STEPMODE_PREV_WITHKEPTLOCK   (UTEXT)0xd3
#define EXTFH_STEPMODE_NEXT_WITHLOCK       (UTEXT)0xd4
#define EXTFH_STEPMODE_NEXT_WITHKEPTLOCK   (UTEXT)0xd5
#define EXTFH_READMODE_DIR_WITHLOCK        (UTEXT)0xd6
#define EXTFH_READMODE_DIR_WITHKEPTLOCK    (UTEXT)0xd7
#define EXTFH_READMODE_SEQ_WITHLOCK        (UTEXT)0xd8
#define EXTFH_READMODE_SEQ_WITHKEPTLOCK    (UTEXT)0xd9
#define EXTFH_READMODE_RAND_WITHLOCK       (UTEXT)0xda
#define EXTFH_READMODE_RAND_WITHKEPTLOCK   (UTEXT)0xdb
#define EXTFH_READMODE_PREV_WITHLOCK       (UTEXT)0xde
#define EXTFH_READMODE_PREV_WITHKEPTLOCK   (UTEXT)0xdf

#define EXTFH_READMODE_POS                 (UTEXT)0xf1
#define EXTFH_READMODE_SEQ                 (UTEXT)0xf5
#define EXTFH_READMODE_RAND                (UTEXT)0xf6
#define EXTFH_READMODE_PREV                (UTEXT)0xf9


#ifdef DO_NOT_APPLY_TO_INDEXED
#define EXTFH_WRITEMODE_WRITE_BEFORE       (UTEXT)0xe1
#define EXTFH_WRITEMODE_WRITE_AFTER        (UTEXT)0xe2
#define EXTFH_WRITEMODE_WRITE_BEFORE_TAB   (UTEXT)0xe3
#define EXTFH_WRITEMODE_WRITE_AFTER_TAB    (UTEXT)0xe4
#define EXTFH_WRITEMODE_WRITE_BEFORE_PAGE  (UTEXT)0xe5
#define EXTFH_WRITEMODE_WRITE_AFTER_PAGE   (UTEXT)0xe6
#define EXTFH_WRITEMODE_WRITE_BEFORE_MNEM  (UTEXT)0xec
#define EXTFH_WRITEMODE_WRITE_AFTER_MNEM   (UTEXT)0xed
#endif
#define EXTFH_WRITEMODE_WRITE              (UTEXT)0xf3
#define EXTFH_WRITEMODE_REWRITE            (UTEXT)0xf4
#define EXTFH_WRITEMODE_DELETE             (UTEXT)0xf7

#define EXTFH_DELETEFILE                   (UTEXT)0xf8
#define EXTFH_UNLOCK                       (UTEXT)0x0e /*Releases all record locks held by the run unit on a named file.*/

#define EXTFH_COMMIT                       (UTEXT)0xdc /*Releases all record locks in all files held by this run unit.*/
#define EXTFH_ROLLBACK                     (UTEXT)0xdd /*Releases all record locks in all files held by this run unit.*/

/* SORT opcodes (on top of standard opcodes) Not sure why any of these may reach EXTFH as they should not apply */
#ifdef DO_NOT_APPLY
#define SORT_INIT                          (UTEXT)0x01
#define SORT_INITMERGE                     (UTEXT)0x40
#define SORT_RELEASE_REC                   (UTEXT)0xf3
#define SORT_TERM_INPUT                    (UTEXT)0x10
#define SORT_RETURNS_REC                   (UTEXT)0xf5
#endif
#define SORT_TERM                          (UTEXT)0x88  /* we have evidence of this opcode being used */

/* SPECIAL OPCODES */
#define EXTFH_DYNREDIR_LOAD                (UTEXT)0x00
#define EXTFH_GETFILEHDR                   (UTEXT)0x03 /* officially undocumented, works on opened files and the returned record is the file header (128 bytes) */
#define EXTFH_FILEINFO                     (UTEXT)0x06
#define EXTFH_CREATE_INDEXFILE             (UTEXT)0x07
#define EXTFH_NEXT_PHYREC                  (UTEXT)0x08
#define EXTFH_ADD_KEYVALUE                 (UTEXT)0x09
#define EXTFH_REINDEX_FILE                 (UTEXT)0x0b
#define EXTFH_FLUSH_FILE                   (UTEXT)0x0c
#define EXTFH_UNLOCK_REC                   (UTEXT)0x0f

/* CTREE EXTFH EXTENSION OPCODES */
#define EXTFH_OPENMODE_INPUT_SQL           (UTEXT)0x00
#define EXTFH_OPENMODE_OUTPUT_SQL          (UTEXT)0x01
#define EXTFH_OPENMODE_IO_SQL              (UTEXT)0x02
#define EXTFH_OPENMODE_EXTEND_SQL          (UTEXT)0x03
#define EXTFH_DELETEFILE_SQL               (UTEXT)0xf8
#define EXTFH_CTREE_INIT                   (UTEXT)0xfe
#define EXTFH_CTREE_EXIT                   (UTEXT)0xff

/* SPECIAL UNDOCUMENTED OPCODES */
#define EXTFH_TRAN_PREPARE                 (UTEXT)0x07
#define EXTFH_TRAN_COMMITPREPARED          (UTEXT)0x08

/* SPECIAL UNDOCUMENTED OPCODES 2 */
#define EXTFH_CLOSE_MVS_LEVEL              (UTEXT)0x02

/* END OPCODES */

/* FLAGS for EXTFH_DELETEFILE fcd[2] per MF email*/
/* The FAF8 opcode can have different meanings when going through fhredir. If you can review ahead of the call it will help.

 Values for 3rd byte as follows..
       78  FAF8-DELETE-FILE-FLAG value 1.
       78  FAF8-CHECK-FILE-EXIST-FLAG value 2.
       78  FAF8-RENAME-FILE-FLAG value 3.
       78  FAF8-COPY-FILE-FLAG   value 4.
       78  FAF8-LOCATE-FILE-FLAG value 5.
       78  FAF8-CREATE-DIR-FLAG value 6.
       78  FAF8-DELETE-DIR-FLAG value 7.
	   */
#define DELETE_FILE_FLAG      0x31
#define CHECK_FILE_EXIST_FLAG 0x32
#define RENAME_FILE_FLAG      0x33
#define COPY_FILE_FLAG        0x34
#define LOCATE_FILE_FLAG      0x35
#define CREATE_DIR_FLAG       0x36
#define DELETE_DIR_FLAG       0x37


#define FCD_AVAL_VERSIONS  2

/* FCD FIELDS */
#define FCD_STATUS_KEY_1       0
#define FCD_STATUS_KEY_2       1
#define FCD_LENGTH             2
#define FCD_VERSION            3
#define FCD_FILEORGANIZATION   4
#define FCD_ACCESSMODE         5
#define FCD_OPENMODE           6
#define FCD_STATUS_TYPE        7
#define FCD_FILEHANDLE         8
#define FCD_RECORDPOINTER      9
#define FCD_MINRECORDLENGTH   10
#define FCD_MAXRECORDLENGTH   11
#define FCD_FILENAMELENGTH    12
#define FCD_LOCKMODEFLAGS     13
#define FCD_KEYDEFINITION     14
#define FCD_FILENAME          15
#define FCD_RECORDLENGTH      16
#define FCD_CONFIGFLAGS       17
#define FCD_KEYNUMBER         18
#define FCD_KEYLENGTH         19
#define FCD_OTHERFLAGS        20
#define FCD_RECORDINGMODE     21
#define FCD_REL_ADDR_OFFSET   22
#define FCD_REL_ADDR_BIG_H    23
#define FCD_REL_ADDR_BIG_L    24
#define FCD_LOCKTYPE          25
#define FCD_FS_FLAGS          26
#define FCD_RETRY_COUNT       27
#define EXTRA_FCD_XDDLEN      28
#define EXTRA_FCD_XDDPTR      29
#define EXTRA_FCD_DBLEN       30
#define EXTRA_FCD_DBPTR       31
#define EXTRA_FCD_PWDLEN      32
#define EXTRA_FCD_PWDPTR      33
#define FCD_DATA_COMPRESS     34
#define FCD_CONFIG_FLAGS2     35
#define FCD_RELATIVE_KEY      36
#define FCD_FAKE_LASTBYTE     37  /* used internally to point to the last byte of the FCD*/
#define FCD_SESSION_ID        38
#define FCD_COL_SEQ_PTR       39
#define FCD_RM_STATUS         40
#define FCD_TRANS_LOG         41
#define FCD_IDXNAME           42
#define FCD_IDXNAMELENGTH     43

/* ANSI 74 error codes */
#define E_74_SUCCESS              "00"
#define E_74_IO_MISSINGFILE       "9\x0d" /* RT013 */
#define E_74_IO_INVALID_OPEN_MODE "9\x25" /* RT037 */
#define E_74_IO_CLOSE_WITH_LOCK   "9\xd2" /* RT210 */
#define E_74_IO_DEF_CONFLICT      "9\x8b" /* RT139 */
#define E_74_IO_ALREADY_OPEN      "9\x8d" /* RT141 */
#define E_74_IO_CLOSE_CLOSED      "9\x8e" /* RT142 */
#define E_74_IO_INVSEQDELREW      "9\x8f" /* RT143 */
#define E_74_IO_BOUNDVIOL         "9\x90" /* RT144 */
#define E_74_IO_NOCURR            "9\x92" /* RT146 */
#define E_74_IO_NOTFORINPUT       "9\x93" /* RT147 */
#define E_74_IO_NOTFOROUTPUT      "9\x94" /* RT148 */
#define E_74_IO_NOTFORIO          "9\x95" /* RT149 */

/* ANSI 85 error codes */
#define E_85_SUCCESS              "00"
#define W_85_IO_DUPL              "02"
#define W_85_IO_WRONG_LENGTH      "04"
#define W_85_IO_FILE_CREATED      "05"
#define E_85_IO_ENDFILE           "10"
#define E_85_IO_DUPL              "22"
#define E_85_IO_NOREC             "23"
#define E_85_IO_INVALIDWRITE      "24"
#define E_85_IO_SYSTEM            "30"
#define E_85_IO_OPENMISSINGFILE   "35"
#define E_85_IO_INVALID_OPEN_MODE "37"
#define E_85_IO_CLOSE_WITH_LOCK   "38"
#define E_85_IO_DEF_CONFLICT      "39"
#define E_85_IO_ALREADY_OPEN      "41"
#define E_85_IO_CLOSE_CLOSED      "42"
#define E_85_IO_INVSEQDELREW      "43"
#define E_85_IO_BOUNDVIOL         "44"
#define E_85_IO_NOCURR            "46"
#define E_85_IO_NOTFORINPUT       "47"
#define E_85_IO_NOTFOROUTPUT      "48"
#define E_85_IO_NOTFORIO          "49"

/* extended file status code - first digit is a ASCII '9' second digit is a binary number */
#define E_85_IO_TOOMANY           "9\x0e" /* RT014 */
#define E_85_IO_LOCKED            "9\x44" /* RT068 */
#define E_85_IO_FLOCKED           "9\x41" /* RT065 */
#define E_85_IO_NOTSUPP           "9\x45" /* RT069 */ /* Illegal argument to isam module*/
#define E_85_IO_NO_REMAPPABLE     "9\x4d" /* RT077 */ /* Internal ISAM module error */
#define E_85_IO_BROKEN            "9\x29" /* RT041 */ /* corrupt index file ?? */
#define E_85_IO_PERMISSION        "9\x23" /* RT035 */ /* Attempt to access a file with incorrect permission */
#define E_85_IO_DELETE_OPEN       "9\x1d" /* RT029 */ /* Attempt to delete open file. */
#define E_85_IO_NO_MEMORY         "9\x69" /* RT105 */ /* Memory allocation error. */
#define E_85_IO_BADARG            "9\x22" /* RT034 */ /* Incorrect mode or file descriptor. */
#define E_85_IO_DISKFULL          "9\x07" /* RT007 */ /* Disk space exhausted. */
#define E_85_IO_MISSINGFILE       "9\x0d" /* RT013 */ /* File not found. */
#define E_85_IO_BADINDEXEDFILE    "9\x2b" /* RT043 */ /* File information missing for indexed file. */
#define E_85_IO_NOT_CTREE_FILE    "9\x47" /* RT071 */ /* Bad indexed file format. */
#define E_85_IO_FILE_SIZE_ERR     "9\xc4" /* RT194 */ /* File size too large. */
#define E_85_IO_LOAD_FAILURE      "9\xc6" /* RT198 */ /* Load failure. */
#define E_85_IO_NETWORK_COMM_ERR  "9\xd1" /* RT209 */ /* Network communication error (Recoverable). */

/* GENERAL BEHAVIOR OPTIONS*/
#define WE_SET_CONFIG_BIT_4
#define MF_LOCK_FIX

#endif /* ~_CTCBEXFH_H_ */

/* end of ctcbexfh.h */
