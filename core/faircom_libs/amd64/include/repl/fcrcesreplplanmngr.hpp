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

#ifndef _FCRCESREPLPLANMNGR_HPP_
#define _FCRCESREPLPLANMNGR_HPP_

#include "fc_rces.h"
#include "fcrcesaux.hpp"
#include "fcrcesdbenginemngr.hpp"
#include "fcrcesconfig.hpp"
#include "fcrcesreplplan.hpp"
#include "fcrcesreplfile.hpp"
#include "fcrcesreplagentdata.hpp"
#include "fcrcesreplmonitordata.hpp"

/* Class type definitions */
class RCESReplPlanMngr;
class RCESReplMsg;
class RCESReplMsgList;
class RCESReplFileList;
typedef RCESReplPlanMngr *pRCESReplPlanMngr, **ppRCESReplPlanMngr;
typedef RCESReplMsg *pRCESReplMsg, **ppRCESReplMsg;
typedef RCESReplFileList *pRCESReplFileList, **ppRCESReplFileList;

/* External functions defined in the data provider dynamic library */
int RCESStartReplAgent(pAgentThrdData agentThrdData, void **agentThrdHandle);
int RCESCheckReplConnected(pAgentThrdData agentThrdData);
void RCESStartReplExcepLog(pAgentThrdData agentThrdData);
void RCESStopReplExcepLog(pAgentThrdData agentThrdData);
void RCESStartReplFuncTime(pAgentThrdData agentThrdData);
void RCESStopReplFuncTime(pAgentThrdData agentThrdData);
void RCESResetReplFuncTime(pAgentThrdData agentThrdData);
void RCESPauseRepl(pAgentThrdData agentThrdData);
void RCESResumeRepl(pAgentThrdData agentThrdData);
int RCESGetReplFuncTimes(pAgentThrdData agentThrdData, char **funcTimesBuffer, int *bufferLen);
int RCESResyncRepl(pAgentThrdData agentThrdData, const char *fileList, char *resyncName, int resyncNameLen);
int RCESStartResyncThrd(pResyncThrdData resyncThrdData, void **resyncThrdHandle);
void RCESStopResyncThrd(pResyncThrdData resyncThrdData, void *resyncThrdHandle);
int RCESUpdateFileFilter(pAgentThrdData agentThrdData);

int RCESDPStartReplMonitor(pMonitorThrdData monitorThrdData, void **monitorThrdHandle);
int RCESDPReplStart(pRCESReplPlanMngr replPlanMngr, FCPool<RCESMasterConnPooled> *masterProvConnPool, FCList<RCESReplExcep> *replExcepList, RCESReplMsgList *msgList, RCESReplicationDirection direction,
	const char *logPos, char **errorMsg);
int RCESDPReplStop(pRCESReplPlanMngr replPlanMngr, RCESReplicationDirection direction, bool waitForStop, bool syncShutdown, char *currLogPos, char *currTranID, char **errorMsg);
void RCESDPReplFree(pRCESReplPlanMngr replPlanMngr, RCESReplicationDirection direction);
int RCESDPReplMonitorStart(pRCESReplPlanMngr replPlanMngr, RCESReplicationDirection direction, int refreshRate, char **errorMsg);
void RCESDPReplMonitorStop(pRCESReplPlanMngr replPlanMngr, RCESReplicationDirection direction);
void RCESDPReplMonitorFree(pRCESReplPlanMngr replPlanMngr, RCESReplicationDirection direction);

/*^****************************************************************************\
*
*   RCESReplPlanMngr
*   Recon-e-Sense replication plan manager class
*
*   Replication Plan manager class
*
\******************************************************************************/
class RCESReplPlanMngr : public RCESReplPlan
{
	public:
		/* Constructors & destructor */
		RCESReplPlanMngr(FCPool<RCESMasterConnPooled> *masterProvConnPool, pRCESReplPlan replPlan, pRCESConfig config, pRCESMasterProvider masterProv);
		RCESReplPlanMngr(FCPool<RCESMasterConnPooled> *masterProvConnPool, pRCESJson replPlan, pRCESConfig config, pRCESMasterProvider masterProv);
		virtual ~RCESReplPlanMngr();

		/* Public services */
		static void ctGetReplPlanMngr(FCPool<RCESMasterConnPooled> *masterProvConnPool, int replPlanID, pRCESMasterProvider masterProv, void *localServerData, ppRCESReplPlanMngr replPlanMngr);
		static void ctGetReplPlanMngr(FCPool<RCESMasterConnPooled> *masterProvConnPool, pRCESConfig config, void* localServerData, const char *replPlanFileName, RCESReplicationDirection *direction,
		                              ppRCESReplPlanMngr replPlanMngr);
		static int ctStop(pRCESConfig config, const char *replPlanID, RCESReplicationDirection direction, char *logPos, char *tranID, char **errorMsg );
		static int ctStopMonitor(pRCESConfig config, const char *replPlanID, RCESReplicationDirection direction, char **errorMsg );
		static int ctPersistDirtyInfo( pRCESMasterProvider masterProv, pRCESReplPlanMngr replPlanMngr );

		/* Public methods */
		void Start(FCPool<RCESMasterConnPooled> *masterProvConnPool, pRCESMasterProvider provider, RCESReplicationDirection direction, const char *logPos);
		void Stop( RCESReplicationDirection direction, bool waitForStop, bool syncShutdown, char *currLogPos, char *currTranID);
		bool Check(RCESReplicationDirection direction, int *errorCode, char *errorMsg);
		void StartReplAgent( RCESReplicationDirection direction, FCPool<RCESMasterConnPooled> *masterProvConnPool);
		void StartReplMonitor( RCESReplicationDirection direction, int refreshRate );
		void DumpFileList( pRCESMasterProvider masterProv, RCESReplicationDirection direction, int currOSID );
		RCESReplicationDirection GetCurrDirection( int currOSID );
		void Sync( pRCESMasterProvider masterProv );
		void StartMonitor( RCESReplicationDirection direction, int refreshRate );
		void StopMonitor( RCESReplicationDirection direction );
		void StartExcepLog( RCESReplicationDirection direction );
		void StopExcepLog( RCESReplicationDirection direction );
		void StartFuncTime( RCESReplicationDirection direction );
		void StopFuncTime( RCESReplicationDirection direction );
		void ResetFuncTime( RCESReplicationDirection direction );
		void GetFuncTimes( RCESReplicationDirection direction, char **funcTimesBuffer, int *funcTimesLen );
		void Pause( RCESReplicationDirection direction );
		void Resume( RCESReplicationDirection direction );
		void DumpLocalParams(pRCESConfig config, pRCESMasterProvider masterProv, RCESReplicationDirection direction );
		void Resync(FCPool<RCESMasterConnPooled> *masterConnPool, RCESReplicationDirection direction, pRCESReplFileList replFileList, bool addNewFiles, int actionID);
		void CleanResyncStatus(RCESReplicationDirection direction);
		void CreateConfigFile(ppRCESReplFile replFileList, int fileCount, const char* suffix, char** configFileName);

		pRCESDBEngine GetSourceDBEngine() { return _sourceDBEngineMngr; };
		pRCESDBEngine GetTargetDBEngine() { return _targetDBEngineMngr; };
		pRCESDBEngineMngr GetSourceDBEngineMngr() { return _sourceDBEngineMngr; };
		pRCESDBEngineMngr GetTargetDBEngineMngr() { return _targetDBEngineMngr; };
		pRCESConfig GetConfig() { return _config; };
		FCHashTable<char> *GetRedirection() { return _redirectList; };
		void *GetInitReplAgentData() { return _initReplAgentData; };
		void **GetReplAgentDataList() { return _replAgentData?_replAgentData->replAgentDatatList:NULL; };
		pAgentThrdData *GetThrdDataList() { return _replAgentData?_replAgentData->agentThrdData:NULL; };
		void **GetThrdHandleList() { return _replAgentData?_replAgentData->agentThrdHandle:NULL; };
		pMonitorThrdData *GetMonitorThrdDataList() { return _replMonitorData?_replMonitorData->monitorThrdData:NULL; };
		void **GetMonitorThrdHandleList() { return _replMonitorData?_replMonitorData->monitorThrdHandle:NULL; };
		bool HasDictPublication() { return _hasDictPublication; };

		void SetSourceDBEngineMngr( pRCESDBEngineMngr sourceDBEngineMngr ) { _sourceDBEngineMngr = sourceDBEngineMngr; };
		void SetTargetDBEngineMngr( pRCESDBEngineMngr targetDBEngineMngr ) { _targetDBEngineMngr = targetDBEngineMngr; };
		void SetInitReplAgentData( void *replAgentData ) { _initReplAgentData = replAgentData; };

		virtual void LockMutex() { fcMutexGet(_hReplPlanMutex); };
		virtual void FreeMutex() { fcMutexRel( _hReplPlanMutex ); };

	private:
		/* Private properties */
		pRCESDBEngineMngr _sourceDBEngineMngr;
		pRCESDBEngineMngr _targetDBEngineMngr;
		void *_initReplAgentData;
		pReplicationAgentData _replAgentData;
		pReplicationMonitorData _replMonitorData;
		FCHashTable<char> *_redirectList; /* Redirection list */
		pRCESConfig _config;
		void *_hReplPlanMutex;
		pReplicationResyncData _resyncData;
		FCList<RCESReplExcep> *_replExcepList;
		pReplExcepThrdData _excepLoadThrdData;
		void *_excepLoadThrdHandle;
		RCESReplMsgList *_msgList;
		pReplMsgThrdData _msgLoadThrdData;
		void *_msgLoadThrdHandle;
		bool _hasDictPublication;

		/* private methods */
		void _addRedirection( const char* sourceDir, const char* targetDir );
		void _checkTargetDB( pRCESMasterProvider masterProvider, RCESReplicationDirection direction );
		void _loadRedirection( pRCESMasterProvider provider );
		void _init(pRCESMasterProvider masterProv, FCPool<RCESMasterConnPooled> *connPool);
		void _cleanLocalParams( RCESReplicationDirection direction );
		void _addRedirList( pRCESJson replPlanJsonObj );
		void _loadRedirList( pRCESJson replPlanJsonObj );
		void _startResyncCheckThrd(RCESReplicationDirection direction, const char *resyncName, int actionID, FCPool<RCESMasterConnPooled> *masterConnPool);
		void _stopResyncCheckThrd(RCESReplicationDirection direction);
		void _startReplExcepThread(FCPool<RCESMasterConnPooled> *masterProvConnPool);
		void _stopReplExcepThread();
		void _startReplMsgThread(FCPool<RCESMasterConnPooled> *masterProvConnPool);
		void _stopReplMsgThread();
};
/*~****************************************************************************/

/*^****************************************************************************\
*
*   RCESReplMsg
*   Recon-e-Sense replication message class
*
\******************************************************************************/
class RCESReplMsg : public FCRefControl
{
public:
	/* Constructors & destructor */
	RCESReplMsg() { _errorCd = RCES_SUCCESS; _message[0] = 0; };
	RCESReplMsg(int errorCd, const char* msg) { _errorCd = errorCd; strcpy(_message, msg); };
	virtual ~RCESReplMsg() { SetRefInactive(); /* Signalize the reference counter that we already started the destruction */ };

	/* Public methods */
	int GetErrorCode() { return _errorCd; };
	const char* GetMessage() { return _message; };

	void SetErrorCode(int errorCd) { _errorCd = errorCd; };
	void SetMessage(const char* msg) { strcpy(_message, msg); };

private:
	/* private properties */
	int _errorCd;
	char _message[RCES_MAX_ERRMSG_LEN + 1];
};
/*~****************************************************************************/

/*^****************************************************************************\
*
*   RCESReplMsgList
*   Recon-e-Sense replication message list class
*
\******************************************************************************/
class RCESReplMsgList : public FCRefControl
{
public:
	/* Constructors & destructor */
	RCESReplMsgList() { _msgList = new FCList<RCESReplMsg>(); };
	virtual ~RCESReplMsgList() {
		SetRefInactive(); /* Signalize the reference counter that we already started the destruction */
		if (_msgList) {
			delete _msgList;
			_msgList = NULL;
		}
	};

	/* Public methods */
	FCList<RCESReplMsg>* GetMessageList() { return _msgList; };

private:
	/* private properties */
	FCList<RCESReplMsg>* _msgList;
};
/*~****************************************************************************/

#endif /* _FCRCESREPLPLANMNGR_HPP_ */