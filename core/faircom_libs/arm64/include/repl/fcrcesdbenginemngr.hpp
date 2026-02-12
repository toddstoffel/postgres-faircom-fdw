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

#ifndef _FCRCESDBENGINEMNGR_HPP_
#define _FCRCESDBENGINEMNGR_HPP_

#include "fc_rces.h"
#include "fcrcesaux.hpp"
#include "fcrcesdbengine.hpp"
#include "fcrcesactionqueue.hpp"
#include "fclockableobj.hpp"

/* Class type definitions */
class RCESDBEngineMngr;
typedef RCESDBEngineMngr *pRCESDBEngineMngr, **ppRCESDBEngineMngr;

typedef struct TransferThrdData {
	void *fileTransfer;
	char errorMsg[RCES_MAX_ERRMSG_LEN+1];
	int errorCode;
	RCESThrdState state;
	void *transferMutex;
	char targetFileName[RCES_MAX_PATH+1];
	pRCESReplPlanMngr replPlanMngr;
	bool isDB;
} TransferThrdData, *pTransferThrdData, **ppTransferThrdData;

/* External functions defined in the data provider dynamic library */
int RCESDBCheckConn(pRCESDBEngine dbEngine, pRCESMasterProvider masterProv, int connTimeout);
int RCESDBInitDBEngineThrd(FCPool<RCESMasterConnPooled> *masterConnPool, pRCESDBEngineMngr dbEngineMngr, ppDBEngineThrdData dbEngineThrdData, void **dbEngineThrdHandle, char **errorMsg);
void RCESDBTermDBEngineThrd( pDBEngineThrdData dbEngineThrdData, void *dbEngineThrdHandle );
int RCESDBGetServerInfo( pRCESDBEngineMngr dbEngineMngr, char **errorMsg );
int RCESDBBlock( pRCESDBEngineMngr dbEngineMngr, char **errorMsg );
int RCESDBUnblock( pRCESDBEngineMngr dbEngineMngr, char **errorMsg );
void RCESDBFileTransferGetObj( pDBEngineThrdData dbEngineThrdData );
void RCESDBFileTransferGetObjForReceive( pDBEngineThrdData dbEngineThrdData );
void RCESDBFileTransferRelObj( pDBEngineThrdData dbEngineThrdData );
int RCESDBFileTransfer( pDBEngineThrdData dbEngineThrdData, const char *sourceFileName, const char *targetFileName, char **errorMsg );
int RCESDBDumpFiles(pDBEngineThrdData dbEngineThrdData, pRCESReplPlan replPlan, ppRCESReplFile replFileList, int fileCount, pRCESActionQueue queue, int actionID, char *logPos, char *tempPath, char **errorMsg,
	pReplicationDumpData dumpData);
int RCESDBReplaceFiles(pDBEngineThrdData dbEngineThrdData, pRCESReplPlan replPlan, ppRCESReplFile replFileList, int fileCount, pRCESActionQueue queue, int actionID, char *tempPath, bool forceReplace, bool isSourceCaseSensitive, const char *databaseName,
	ppRCESReplRedir redirList, int redirListCount, char **errorMsg, pReplicationDumpData dumpData);
int RCESDBBlockFile( pDBEngineThrdData dbEngineThrdData, const char *fileName, char **errorMsg );
int RCESDBUnblockFile(pDBEngineThrdData dbEngineThrdData, const char *fileName, char **errorMsg);
int RCESDBUpdateReadOnly(pDBEngineThrdData dbEngineThrdData, bool readonly, char **errorMsg);
int RCESDBUpdateRole(pDBEngineThrdData dbEngineThrdData, int serverRole, const char *replPlanNames, char **errorMsg);
int RCESDBRemoveReplAgentState(pDBEngineThrdData dbEngineThrdData, const char* agentid, bool isSourceServer, char* replStateBuffer, size_t replStateBufferLen, char** errorMsg);
int RCESDBUpdateNodeID(pDBEngineThrdData dbEngineThrdData, const char *nodeID, char **errorMsg);
int RCESDBSQLCacheReset(pDBEngineThrdData dbEngineThrdData, const char *databaseName, char **errorMsg);
int RCESDBCheckStart( pRCESMasterProvider masterProv, int osID, char **errorMsg );
int RCESDBCheckStop( char **errorMsg );
int RCESDBCheck( pRCESMasterProvider masterProv, pRCESOpSystem opSystem, ppRCESVolume volumeList, int volumeCount, const char* UUID, const char *localVolumeName, int localVolumeID, const char *localPath,
	const char *cfgFilePath, const char* licenseFilePath, bool canRemove, int *fileCount );
int RCESDBCheckDBTables( pRCESDBEngineMngr dbEngineMngr, pRCESDatabase database, ppRCESFile tableList, int tableCount, char **errorMsg );
int RCESDBCheckAgent(pDBEngineThrdData dbEngineThrdData, const char *agentID, bool isParallel, char **errorMsg);
int RCESDBDeleteTable(pRCESDBEngineMngr dbEngineMngr, pRCESFile file, pRCESDatabase database, char **errorMsg);
int RCESDBDropTable(pRCESDBEngineMngr dbEngineMngr, pRCESFile file, pRCESDatabase database, char **errorMsg);
int RCESDBAddReplAgent(pDBEngineThrdData dbEngineThrdData, pRCESReplPlanMngr replPlanMngr, ppRCESReplFile replFileList, int fileCount, char** errorMsg);
int RCESDBRemoveReplAgent(pDBEngineThrdData dbEngineThrdData, const char* agentID, char** errorMsg);
int RCESDBReplAgentExists(pDBEngineThrdData dbEngineThrdData, const char* agentID, bool *exists, char** errorMsg);
int RCESDBDeployFileGroups(pDBEngineThrdData dbEngineThrdData, pRCESReplPlanMngr replPlanMngr, ppRCESReplFile replFileList, int fileCount, unsigned int* replActionID, char** errorMsg);
int RCESDBCheckReplAction(pDBEngineThrdData dbEngineThrdData, const char* agentID, unsigned int replActionID, char* replPercentage, bool* completed, char** errorMsg);
int RCESDBRemoveFileGroups(pDBEngineThrdData dbEngineThrdData, const char* agentID, const char* fileGroupList, char** errorMsg);
int RCESDBStartReplAgent(pDBEngineThrdData dbEngineThrdData, const char* agentID, char** errorMsg);
int RCESDBStopReplAgent(pDBEngineThrdData dbEngineThrdData, const char* agentID, char** errorMsg);
int RCESDBPauseReplAgent(pDBEngineThrdData dbEngineThrdData, const char* agentID, char** errorMsg);
int RCESDBResumeReplAgent(pDBEngineThrdData dbEngineThrdData, const char* agentID, char** errorMsg);
int RCESDBGetReplAgentState(pDBEngineThrdData dbEngineThrdData, const char* agentID, void** stateBuffer, size_t* stateBufferLen, char** errorMsg);
int RCESDBGetReplAgentStats(pDBEngineThrdData dbEngineThrdData, const char* agentID, void** statsBuffer, size_t* statsBufferLen, char** errorMsg);
int RCESDBGetReplParallelState(pDBEngineThrdData dbEngineThrdData, const char* agentID, void** statsBuffer, size_t* statsBufferLen, char** errorMsg);

int RCESDPExtractReplState(void* stateBuffer, size_t stateBufSiz, pRCESReplStat stat, char** errorMsg);
int RCESDPExtractReplStats(void* statBuffer, size_t statBufSiz, pRCESReplStat stat, char** errorMsg);
int RCESDPExtractReplParallelState(void* stateBuffer, size_t stateBufSiz, pRCESReplParallelStat stat, char** errorMsg);
int RCESDPExtractReplParallelStats(void* statBuffer, size_t statBufSiz, pRCESReplParallelStat stat, char** errorMsg);
int RCESDPExtractReplGlobalParallelState(void* staetBuffer, size_t stateBufSiz, pRCESReplParallelStat stat, char** errorMsg);

/*^****************************************************************************\
*
*   RCESDBEngineMngr
*   Recon-e-Sense database engine manager class
*
\******************************************************************************/
class RCESDBEngineMngr : public RCESDBEngine
{
	public:
		/* Constructors & destructor */
		RCESDBEngineMngr(pRCESConfig config, FCPool<RCESMasterConnPooled> *masterConnPool, pRCESDBEngine dbEngine, int connTimeout = RCES_CHECK_DBENGINE_LONG_TIMEOUT );
		virtual ~RCESDBEngineMngr();

		/* Public services */
		static pRCESDBEngineMngr ctGetDBEngineMngr(FCPool<RCESMasterConnPooled> *masterConnPool, pRCESDBEngine dbEngine, pRCESConfig config);
		static pRCESDBEngineMngr ctGetDBEngineMngr(pRCESMasterProvider masterProv, FCPool<RCESMasterConnPooled> *masterConnPool, int dbEngineID);
		static pRCESDBEngineMngr ctGetMasterDBEngineMngr(pRCESMasterProvider masterProv, FCPool<RCESMasterConnPooled> *masterConnPool);
		static void ctDumpCancel(pReplicationDumpData dumpData);
		static void ctCheckConn(pRCESDBEngine dbEngine, pRCESMasterProvider masterProv, int connTimeout = RCES_CHECK_DBENGINE_LONG_TIMEOUT);

		/* Public methods */
		void GetServerInfo();
		void Block();
		void Unblock();
		void Start();
		void CheckDBTables( pRCESDatabase database, ppRCESFile tableList, int tableCount );
		pDBEngineThrdData GetThrdData() { return _dbEngineThrdData; };
		void SetFileTransfer();
		void ReleaseFileTransfer();
		void FileTransfer( const char *sourceFileName, const char *targetFileName );
		void DumpFiles(pRCESReplPlan replPlan, ppRCESReplFile replFileList, int replFileCount, pRCESActionQueue queue, int actionID, char *logPos, char *tempPath, pReplicationDumpData dumpData);
		void ReplaceFiles(pRCESReplPlan replPlan, ppRCESReplFile replFileList, int replFileCount, pRCESActionQueue queue, int actionID, char *tempPath, bool forceReplace, bool isSourceCaseSensitive, const char *replPlanDeps, pReplicationDumpData dumpData,
			const char *databaseName, ppRCESReplRedir redirList, int redirListCount);
		void BlockFile( const char *fileName );
		void UnblockFile(const char *fileName);
		void UpdateReadOnly(bool readOnly);
		void UpdateRole(int serverRole, const char *replPlanNames);
		void RemoveReplAgentState(const char* agentid, bool isSourceServer, char* replStateBuffer, size_t replStateBufferLen);
		bool IsDirty() { return _isDirty; };
		void CheckAgent(const char *agentID, bool isParallel);
		void Sync(pRCESMasterProvider masterProv);
		void GenerateNodeID(pRCESMasterProvider masterProv);
		void CheckNodeID();
		void DeleteTable(pRCESFile file, pRCESDatabase database);
		void DropTable(pRCESFile file, pRCESDatabase database);
		void SQLCacheReset(const char *databaseName);
		void AddReplAgent(pRCESReplPlanMngr replPlanMngr, ppRCESReplFile replFileList, int replFileCount);
		void RemoveReplAgent(const char* agentID);
		void DeployReplFileGroups(pRCESReplPlanMngr replPlanMngr, ppRCESReplFile replFileList, int replFileCount, unsigned int* replActionID);
		void CheckReplAction(const char* agentID, unsigned int replActionID, char* replPercentage, bool* completed);
		void RemoveReplFileGroups(const char* agentID, const char* fileGroupList);
		void StartReplAgent(const char* agentID);
		void StopReplAgent(const char* agentID);
		void PauseReplAgent(const char* agentID);
		void ResumeReplAgent(const char* agentID);
		void GetReplAgentStat(const char* agentID, ppRCESReplStat replStat);
		void GetReplAgentParallelStat(const char* agentID, ppRCESReplParallelStat parallelReplStat);

		int GetProcID() { return _procID; };
		int GetConnTimeout() { return _connTimeout; };
		pRCESConfig GetConfig() { return _config; };

		void SetProcID( int procID ) { _procID = procID; };
		void SetIsDirty( bool isDirty ) { _isDirty = isDirty; };

		virtual void LockMutex() { fcMutexGet(_hDBEngineMutex); };
		virtual void FreeMutex() { fcMutexRel(_hDBEngineMutex); };

	private:
		/* Private properties */
		void *_dbEngineThrdHandle;
		pDBEngineThrdData _dbEngineThrdData;
		void *_hDBEngineMutex;
		int _procID;
		bool _isDirty;
		pRCESConfig _config;
		int _connTimeout;

		/* Private methods */
		bool _findServiceByID(long processID);
		void _getModuleByID(long processID);
		void _init(FCPool<RCESMasterConnPooled> *masterConnPool);
};
/*~****************************************************************************/

#endif /* _FCRCESDBENGINEMNGR_HPP_ */