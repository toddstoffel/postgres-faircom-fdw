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
 *      Copyright (c) 2013 - 2024 FairCom Corporation.
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
 */

#ifndef _FCRCESFAILOVERMNGR_HPP_
#define _FCRCESFAILOVERMNGR_HPP_

#include "fc_rces.h"
#include "fcrcesaux.hpp"
#include "fcrcesavailgroup.hpp"
#include "fclockableobj.hpp"

#define RCES_FAILOVER_CURRDBENGINE "currentDbEngine"
#define RCES_FAILOVER_DBENGINECONN "redirectedDbEngine"

/* Class type definitions */
class RCESFailOverMngr;
class RCESMasterConfig;
class CTMemGridNode;
class CTMemGridClient;
typedef RCESFailOverMngr *pRCESFailOverMngr, **ppRCESFailOverMngr;
typedef CTMemGridNode *pCTMemGridNode, **ppCTMemGridNode;
typedef CTMemGridClient *pCTMemGridClient, **ppCTMemGridClient;
typedef RCESMasterConfig *pRCESMasterConfig;

/*^****************************************************************************\
*
*   RCESFailOverMngr
*   Recon-e-Sense failover manager class
*
*   Failover manager class
*
\******************************************************************************/
class RCESFailOverMngr : public FCRefControl
{
	public:
		/* Constructors & destructor */
		RCESFailOverMngr(pRCESConfig config, pRCESAvailGroup availGroup);
		virtual ~RCESFailOverMngr();

		/* Public services */
		static pRCESFailOverMngr ctGetFailOverMngr(pRCESConfig config, pRCESAvailGroup availGroup);
		static pRCESFailOverMngr ctGetFailOverMngr(pRCESConfig config, pRCESMasterProvider masterProv = NULL, int DBEngineID = -1);
		static void ctLoadAllActiveFailOverMngr(pRCESConfig config, pRCESMasterProvider masterProv);
		static pRCESFailOverMngr ctGetFailOverMngrByRepl(pRCESMasterProvider masterProv, int ReplPlanID);
		static void ctUpdateFailOverMngr(pRCESMasterProvider masterProv, pRCESAvailGroup availGroup);
		static pRCESFailOverMngr ctCheckLocalFailOverMngr(pRCESConfig config, pRCESMasterProvider masterProv);
		static void ctCheckRemoteFailOverMngr(pRCESMasterProvider masterProv, pRCESDBEngine remoteDBEngine, int *actionID);
		static void ctUpdateAvailGroupStatus(pRCESMasterProvider masterProv, pRCESAvailGroup availGroup, RCESEngineStatus newStatus, RCESEngineStatus* currentStatus, RCESReplStatus newReplicaStatus, const char *statusMsg, int actionID);
		static void ctUpdateAvailGroupStatusFromMemphis(pRCESMasterProvider masterProv, pRCESAvailGroup availGroup, int actionID);
		static void ctUpdateReplicaDBStatus(pRCESConfig config, int DBEngineID, RCESEngineStatus newStatus);
		static void ctLoadAvailGroupsToMemphis(pRCESMasterProvider masterProv);
		static const char *GetConnModeStr(RCESFailoverConnMode connMode);
		static RCESFailoverConnMode GetConnMode(const char *connModeStr);
		static bool IsReplicaInMemoryGrid(pRCESConfig config, pRCESAvailGroup availGroup, int DBEngineID);

		/* Public methods */
		void Start(pRCESMasterProvider masterProv, int localDBEngineID);
		void Stop();
		void LoadAvailGroupToMemGrid(pRCESMasterProvider masterProv);
		void LoadAvailGroupToMemphis(pRCESMasterProvider masterProv);
		void LoadReplicaToMemGrid(pRCESMasterProvider masterProv, int localDBEngineID);
		void UpdateAvailGroupStatus(pRCESMasterProvider masterProv, RCESEngineStatus newStatus, RCESEngineStatus* currentStatus, RCESReplStatus newReplicaStatus, const char *statusMsg, int actionID);
		void UpdateAvailGroupStatusFromMemphis(pRCESMasterProvider masterProv, int actionID);
		void UpdateAvailGroupStatusFromMemGrid();
		void UpdateReplicaStatus(int DBEngineID, RCESEngineStatus newStatus);
		void SetAvailGroup(pRCESAvailGroup availGroup);
		void SetAvailGroupSyncState(int dbEngineID, bool isSync, int *availGroupID = NULL);
		pRCESAvailGroupStatus GetAvailGroupStatusWithReplicas();
		pCTJson GetClusterInformation(const char *input, int localDBEngineID);
		int GetDBEngineToConnect(const char *input);
		bool IsReplicaInMemoryGrid(int dbEngineID);

		bool IsActive() { return _isActive; };
		RCESEngineStatus GetStatus() { if (_availGroup) return _availGroup->GetStatus(); else return RCES_NOTSUP; }
		pRCESAvailGroup GetAvailGroup();

	private:
		/* Private properties */
		bool _isActive;
		pRCESConfig _config;
		pRCESMasterConfig _masterConfig;
		FCPool<RCESMasterConnPooled> *_masterConnPool;
		pRCESAvailGroup _availGroup;
		int _localDBEngineID;
		void *_hFailOverMutex;
		FCThreadID _failOverMutexOwner;
		int _failOverMutexLocation;
#define LAST_FAILOVER_MUTEX_LOCATION 15
#ifdef ctFeatMEMGRID
		pCTMemGridNode _gridNode;
		pCTMemGridClient _gridClient;
#endif

		/* private methods */
		RCESEngineStatus _getAvailGroupCurrStatus();
};
/*~****************************************************************************/

#endif /* _FCRCESFAILOVERMNGR_HPP_ */