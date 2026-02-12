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
#ifndef CTDUMP_H
#define CTDUMP_H
static cpTEXT kw_dmp[] = {
		"!DELAY",
		"!DATE",
		"!TIME",
		"!FREQ",
		"!DUMP",
		"!DAY",
		"!COMMENT",
		"!#FCB",
		"!PAGE_SIZE",
		"!SEGMENT",
		"!EXT_SIZE",
		"!REDIRECT",
		"!RECURSE",
		"!IMMEDIATE_RESTORE_LOGFILE",
		"!REPL_RESYNC_ID",
		"!DIAGNOSTICS",
		"!RECOVER_DETAILS",
		"!RECOVER_MEMLOG",
		"!REPL_UNIQUE_ID",
		/*
		** keywords which require an argument should be inserted
		** BEFORE this comment. Keywords without an argument should
		** be inserted AFTER the !SKIP keyword.
		*/
		"!SKIP",
		"!DELETE",
		"!BLOCK_RETURN",
		"!FORWARD_ROLL",
		"!FILES",
		"!END",
		"!ENDSEGMENT",
		"!PROTECT",
		"!PROTECT_LOW",
		"!IMMEDIATE_RESTORE",
		"!COPY_NONCTREE",
		"!CLNIDXX",
		"!IMMEDIATE_RESTORE_WITH_PROGRESS",
		"!EXCLUDE_FILES",
		"!NONCTREEFILES",
		"!CONVERT_PATHSEP",
		"!REDIRECT_IFIL",
		"!OLD_REDIRECT",
		"!ALLOW_TRAN",
		"!KEEP_LOGS"
		};

enum dump_keywords {
	dmpDELAY = 0,
	dmpDATE,
	dmpTIME,
	dmpFREQ,
	dmpDUMP,
	dmpDAY,
	dmpCOMMENT,
	dmpFCB,			/* ctrdmp */
	dmpPAGE_SIZE,		/* ctrdmp */
	dmpSEGMENT,
	dmpEXT_SIZE,
	dmpREDIRECT,
	dmpRECURSE,
	dmpIMMEDIATE_RESTORE_LOGFILE,
	dmpREPL_RESYNC_ID, 	/* unique ID for replication file resync operation */
	dmpDIAGNOSTICS, 	/* ctrdmp */
	dmpRECOVER_DETAILS,	/* ctrdmp */
	dmpRECOVER_MEMLOG,	/* ctrdmp */
	dmpREPL_UNIQUE_ID,	/* unique ID for replication agent */
	dmpSKIP,		/* ctrdmp */
	dmpDELETE,		/* ctrdmp */
	dmpBLOCK_RETURN,
	dmpFORWARD_ROLL,	/* ctrdmp */
	dmpFILES,
	dmpEND,
	dmpENDSEGMENT,
	dmpPROTECT,
	dmpPROTECT_LOW,
	dmpIMMEDIATE_RESTORE,
	dmpCOPY_NONCTREE,
	dmpCLNIDXX,		/* ctrdmp */
	dmpIMMEDIATE_RESTORE_WITH_PROGRESS,
	dmpEXCLUDE_FILES,
	dmpNONCTREEFILES,
	dmpCONVERT_PATHSEP, 	/* ctrdmp */
	dmpREDIRECT_IFIL,	/* ctrdmp */
	dmpOLD_REDIRECT,	/* ctrdmp */
	dmpALLOW_TRAN,
	dmpKEEP_LOGS,		/* ctrdmp */
	dmpLAST
};

/* new keywords should be handled in both dump (ctddmp.c) and restore (ctrdmp.c) */

typedef char checkkw_dmpSize[sizeof(kw_dmp)/sizeof(*kw_dmp) == dmpLAST];
#endif
