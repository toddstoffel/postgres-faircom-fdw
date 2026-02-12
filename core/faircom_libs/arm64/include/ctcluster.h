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

#ifndef ctCLUSTERH
#define ctCLUSTERH

/* values for server sync attribute stored in cluster */
typedef enum serverSyncAttr {
	SERVER_SYNC_ATTR_UNKNOWN = 1, /* server sync mode not stored in cluster */
	SERVER_IS_PRIMARY_IN_ASYNC_MODE = 2, /* server is primary server in async mode */
	SERVER_IS_PRIMARY_IN_SYNC_MODE = 3, /* server is primary server in sync mode */
	SERVER_IS_SECONDARY_WITH_PRIMARY_ASYNC = 4, /* server is secondary and primary is in async mode */
	SERVER_IS_SECONDARY_WITH_PRIMARY_SYNC = 5 /* server is secondary and primary is in sync mode */
#define LAST_SERVER_SYNC_ATTR 5
} SERVER_SYNC_ATTR;

NINT getServerSyncReplicationAttrFromCluster(SERVER_SYNC_ATTR *pServerSyncAttr, pTEXT errbuf, NINT errbuflen);
NINT setServerSyncReplicationAttr(SERVER_SYNC_ATTR serverSyncAttr, TEXT updateCluster, pTEXT errbuf, NINT errbuflen);
NINT initServerSyncReplicationAttr(pTEXT errbuf, NINT errbuflen);
NINT checkServerPendingSyncMode(pctLOGP pLogCopyPosition, pTEXT errbuf, UINT errbuflen);
NINT getPartnerServerRole(SERVER_ROLE *pPartnerServerRole, TEXT logmsg, pTEXT errbuf, ULONG errbuflen);
NINT getServerRoleInCluster(SERVER_ROLE *pServerRoleInCluster, pTEXT errbuf, ULONG errbuflen);
NINT getRecoveryInfoFromPrimary(pSYNC_COMMIT_RECOVERY_STATE pSyncCommitRecoveryState, pTEXT errbuf, ULONG errbuflen);
NINT addReplicationAgentPluginToConfig(pTEXT errbuf, ULONG errbuflen);
NINT startReplicationAgentPlugin(pTEXT errbuf, ULONG errbuflen);
NINT stopReplicationAgentPlugin(pTEXT errbuf, ULONG errbuflen);
NINT completeStartupRoleActions(pTEXT errbuf, ULONG errbuflen);
NINT runSetSyncStateScript(pTEXT option, pTEXT errbuf, NINT errbuflen);

extern ctRWLOCK syncCommitRegionRWL;

#endif /* ~ctCLUSTERH */

/* end of ctcluster.h */
