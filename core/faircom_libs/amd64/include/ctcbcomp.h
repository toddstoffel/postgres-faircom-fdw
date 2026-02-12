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

#ifndef _CTCBCOMP_H_
#define _CTCBCOMP_H_

#define rtgFEATMASK_FILEPASSWORD        0x00000001 /* file password support */
#define rtgFEATMASK_WRITE_RET_RECDAT    0x00000002 /* return record data upon write */
#define rtgFEATMASK_RECORDSET           0x00000004 /* extended record rewrite */
#define rtgFEATMASK_NORUNIT_UNLOCKALL   0x00000008 /* <runitlockdetect unlockall> */
#define rtgFEATMASK_FIXMINLEN_OVER_64KB 0x00000010 /* support for fixed record length > 64KB */
#define rtgFEATMASK_OPEN_FOR_INFO       0x00000020 /* displays RTG resource version in ctutil -info */
#define rtgFEATMASK_BULKADD_ADD2END     0x00000040 /* pass flmode2 to cts_close to fix bulkaddition with ctADD2END */
#define rtgFEATMASK_ACCESS              0x00000080 /* support for checking existence of a file/directory */
#define rtgFEATMASK_DATA_FILTERING      0x00000100 /* support for recordset filtering */
#define rtgFEATMASK_AUGMENT             0x00000200 /* support for changing maxlen */
#define rtgFEATMASK_RETRYOPEN           0x00000400 /* <retryopen> */

#define CT_COMPATIBILITY_ID 34

#define CT_STRUCTVER_ENCRYPTYPE       1 /* introduced encryption type */
#define CT_STRUCTVER_KEYSEGMODE       2 /* introduced key segment modes */
#define CT_STRUCTVER_RECHDRSRLIDX     3 /* introduced record header and serial index */
#define CT_STRUCTVER_DUPSCANWA        4 /* introduced dup scan work-around */
#define CT_STRUCTVER_READRETRECNUM    5 /* reads return record number */
#define CT_STRUCTVER_CTSINFO          6 /* introduced cts_info() */
#define CT_STRUCTVER_NEXTORFIRST      7 /* introduced CTS_NEXT_OR_FIRST_RECORD */
#define CT_STRUCTVER_CTSCOPYRECHDR    8 /* rechdr passed to cts_copy() */
#define CT_STRUCTVER_EXTTYPE_KEY      9 /* introduced support for BTRV extended key types*/
#define CT_STRUCTVER_TBLLOCKER       10 /* locker id support for table open/make */
#define CT_STRUCTVER_NEXTEXECUTED    11 /* introduced flag to signal consecutive next ops */
#define CT_STRUCTVER_CTSREMOVERENAME 12 /* introduced remove/rename server-side functions */
#define CT_STRUCTVER_CTSSQLLINK      13 /* introduced cts_sqllink server-side function */

#define CT_STRUCTVER_LAST            (ULONG)15 /* last incremental before bitmask was introduced */

#define CT_STRUCTVER_DELCURRENT      0x00000010 /* introduced delcurrent optimization */
#define CT_STRUCTVER_RESQLIZE        0x00000020 /* refresh sqlize */
#define CT_STRUCTVER_FILEID          0x00000040 /* reset fileid */
#define CT_STRUCTVER_PREFETCHPREV    0x00000080 /* prefetch previous */
#define CT_STRUCTVER_SRLSEGSIZ       0x00000100 /* support for dynamic SRLSEG size */
#define CT_STRUCTVER_FILEPERMISSION  0x00000200 /* file permission support */
#define CT_STRUCTVER_SCANCACHE       0x00000400 /* <scancache> support */
#define CT_STRUCTVER_REPAIRDATA      0x00000800 /* -repairdata rebuild option support */
#define CT_STRUCTVER_STARTONREAD     0x00001000 /* <startonread> support */
#define CT_STRUCTVER_STRUCT2019      0x00002000 /* RPC structures expansion */
#define CT_STRUCTVER_MAXFILE32       0x00004000 /* 4 bytes file numbers in RPC structures */
#define CT_STRUCTVER_BULKADDOPT      0x00008000 /* bulkaddition optimizations */
#define CT_STRUCTVER_SQLSKIPMISSING  0x00010000 /* sqlunlink skip missing table */
#define CT_STRUCTVER_NOctNOENCRYP    0x00020000 /* create without ctNOENCRYP file mode */
#define CT_STRUCTVER_MEMFILE_PERSIST 0x00040000 /* <memoryfile persist> support */
#define CT_STRUCTVER_CTBTRV_FILES    0x00080000 /* CTBTRV files support */
#define CT_STRUCTVER_TRUNCATE_FILE   0x00100000 /* file truncate support */
#define CT_STRUCTVER_CTMAKE2INPUT_0  0x00200000 /* the padding bytes in CT_MAKE2_INPUT have been initialized to 0, was not the case before */
#define CT_STRUCTVER_TRUNCATEIFEXIST 0x00400000 /* truncate if file already exists */
#define CT_STRUCTVER_KEYS            0x00800000 /* optimized key definition structure */
#define CT_STRUCTVER_CTATTR          0x01000000 /* file resources based on CTATTR (V12 format) -- BE CAREFUL see CT_STRUCTVER_CTATTR in ctdbIsRTGTable*/
#define CT_STRUCTVER_LOGWHOFORMAT    0x02000000 /* <log whoformat> to configure the format of the "who" log line portion */
#define CT_STRUCTVER_NODESIZE        0x04000000 /* index node size in ctutil -info output */
#define CT_STRUCTVER_CTFLEXREC       0x08000000 /* <ctflexrec> support for hot alter table */
#define CT_STRUCTVER_NULLKEY         0x10000000 /* any-segment and all-segment NULL key support */
#define CT_STRUCTVER_GETPERCENT_KEYNUMBER 0x20000000 /* supports for get percentage and key numbers */
#define CT_STRUCTVER_GETEXTENDED     0x40000000 /* extended read support */
#define CT_STRUCTVER_FEATMASK        0x80000000 /* featmask to expand structver */

#define CT_STRUCTVER_ID ctcbStructverID()

#endif /* ~_CTCBCOMP_H_ */

/* end of ctcbcomp.h */
