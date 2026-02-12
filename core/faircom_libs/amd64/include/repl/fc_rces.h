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

#ifndef _FC_RCES_HPP_
#define _FC_RCES_HPP_

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include "fcaux.hpp"
#include "fcrcesmetadata.hpp"
#include "cterrc.h"

#ifndef NO_ctFeatMEMGRID
#if defined(_WINDOWS) || defined(ctPortLINUX)
#ifndef  ctFeatMEMGRID
#define  ctFeatMEMGRID /* Turn ON Memory Grid feature */
#endif
#endif /* _WINDOWS */
#endif /* ~NO_ctFeatMEMGRID */

#define RCES_ESTIMATE_SCAN /* It adds an overhead before starting the filesystem scan, but with that, we can have a sense of progress during
                              the scan */

/* Class type forward definitions */
class RCESConfig;
class RCESMasterProvider;
class RCESSite;
class RCESHardware;
class RCESOpSystem;
class RCESVolume;
class RCESDBEngine;
class RCESReplPlan;
class RCESDynServer;
class RCESFile;
class RCESDatabase;
class RCESReplFile;
class RCESMasterConnPooled;
class RCESFileGroup;
class RCESAction;
class RCESConn;
class RCESDBFileGroup;
class RCESDBEngineGroup;
class RCESPublication;
class RCESSubscription;
class RCESAgent;
class RCESReplStat;
class RCESReplStatItem;
class RCESReplParallelStat;
class RCESReplParallelAnalyzer;
class RCESReplParallelApply;
class RCESReplExcep;
class RCESActionQueueCriteria;
class RCESActionQueueCriteriaItem;
class RCESDBEngineTrace;
class RCESLog;
class RCESReplFunc;
class RCESReplAgent;
class RCESReplMsg;
class RCESReplRedir;
class RCESAgentConsumer;
class RCESAvailGroup;
class RCESReplica;
class RCESAvailGroupStatus;
class RCESReplicaStatus;
class RCESFolderRule;
class RCESDatabaseRule;
class RCESDictionaryRule;
class RCESPublicationRule;
class RCESRule;
typedef RCESConfig *pRCESConfig, **ppRCESConfig;
typedef RCESMasterProvider *pRCESMasterProvider, **ppRCESMasterProvider;
typedef RCESSite *pRCESSite, **ppRCESSite;
typedef RCESHardware *pRCESHardware, **ppRCESHardware;
typedef RCESOpSystem *pRCESOpSystem, **ppRCESOpSystem;
typedef RCESVolume *pRCESVolume, **ppRCESVolume;
typedef RCESDBEngine *pRCESDBEngine, **ppRCESDBEngine;
typedef RCESReplPlan *pRCESReplPlan, **ppRCESReplPlan;
typedef RCESDynServer *pRCESDynServer, **ppRCESDynServer;
typedef RCESFile *pRCESFile, **ppRCESFile;
typedef RCESDatabase *pRCESDatabase, **ppRCESDatabase;
typedef RCESReplFile *pRCESReplFile, **ppRCESReplFile;
typedef RCESMasterConnPooled *pRCESMasterConnPooled, **ppRCESMasterConnPooled;
typedef RCESFileGroup *pRCESFileGroup, **ppRCESFileGroup;
typedef RCESAction *pRCESAction, **ppRCESAction;
typedef RCESConn *pRCESConn, **ppRCESConn;
typedef RCESDBFileGroup *pRCESDBFileGroup, **ppRCESDBFileGroup;
typedef RCESDBEngineGroup *pRCESDBEngineGroup, **ppRCESDBEngineGroup;
typedef RCESPublication *pRCESPublication, **ppRCESPublication;
typedef RCESSubscription *pRCESSubscription, **ppRCESSubscription;
typedef RCESAgent *pRCESAgent, **ppRCESAgent;
typedef RCESReplStat *pRCESReplStat, **ppRCESReplStat;
typedef RCESReplStatItem *pRCESReplStatItem, **ppRCESReplStatItem;
typedef RCESReplParallelStat *pRCESReplParallelStat, **ppRCESReplParallelStat;
typedef RCESReplParallelAnalyzer *pRCESReplParallelAnalyzer, **ppRCESReplParallelAnalyzer;
typedef RCESReplParallelApply *pRCESReplParallelApply, **ppRCESReplParallelApply;
typedef RCESReplExcep *pRCESReplExcep, **ppRCESReplExcep;
typedef RCESActionQueueCriteria *pRCESActionQueueCriteria, **ppRCESActionQueueCriteria;
typedef RCESActionQueueCriteriaItem *pRCESActionQueueCriteriaItem, **ppRCESActionQueueCriteriaItem;
typedef RCESDBEngineTrace *pRCESDBEngineTrace, **ppRCESDBEngineTrace;
typedef RCESLog *pRCESLog, **ppRCESLog;
typedef RCESReplFunc *pRCESReplFunc, **ppRCESReplFunc;
typedef RCESReplAgent *pRCESReplAgent, **ppRCESReplAgent;
typedef RCESReplMsg *pRCESReplMsg, **ppRCESReplMsg;
typedef RCESReplRedir *pRCESReplRedir, **ppRCESReplRedir;
typedef RCESAgentConsumer *pRCESAgentConsumer, **ppRCESAgentConsumer;
typedef RCESAvailGroup *pRCESAvailGroup, **ppRCESAvailGroup;
typedef RCESReplica *pRCESReplica, **ppRCESReplica;
typedef RCESAvailGroupStatus *pRCESAvailGroupStatus, **ppRCESAvailGroupStatus;
typedef RCESReplicaStatus *pRCESReplicaStatus, **ppRCESReplicaStatus;
typedef RCESFolderRule *pRCESFolderRule, **ppRCESFolderRule;
typedef RCESDatabaseRule *pRCESDatabaseRule, **ppRCESDatabaseRule;
typedef RCESDictionaryRule *pRCESDictionaryRule, **ppRCESDictionaryRule;
typedef RCESPublicationRule *pRCESPublicationRule, **ppRCESPublicationRule;
typedef RCESRule *pRCESRule, **ppRCESRule;

typedef struct
{
	pRCESDBEngineGroup group;
	pRCESDBEngine dbEngine;
} RCESDBEngineGroupItem, *pRCESDBEngineGroupItem, **ppRCESDBEngineGroupItem;

#define RCESBigInt        FCBigInt
#define RCESSize          FCSize
#define RCESDateTime      FCDateTime
#define RCES_MAX_DATETIME FC_MAX_DATETIME

/*^****************************************************************************\
*
*   RCES Replication API
*
\******************************************************************************/
int ctReplConnectX(pRCESConfig config, ppRCESConn replConn, char **errorMsg);
int ctReplConnectX(const char *masterHost, const char *masterServerName, const char *masterSQLPort, const char *masterDatabase, ppRCESConn replConn, char **errorMsg);
int ctReplDisconnect(pRCESConn replConn);
int ctReplSetTrace(pRCESConn replConn, bool enableWebTrace, bool globalEnableTrace, ppRCESDBEngineTrace traceList, int traceListCount);
int ctReplGetTrace(pRCESConn replConn, bool *enableWebTrace, bool *globalEnableTrace);
int ctReplGetLatestLogs(pRCESConn replConn, int latestCount, short maxDetailLevel, short maxSevLevel, bool showDBEngine, bool showReplStudio, bool showReplAgent, bool showWebServer,
	int dbEngineID, const char *funcName, int errorCode, ppRCESLog *logList, int *logCount);
int ctReplGetLogs(pRCESConn replConn, RCESDateTime startTime, RCESDateTime endTime, short maxDetailLevel, short maxSevLevel, bool showDBEngine, bool showReplStudio, bool showReplAgent,
	bool showWebServer, int dbEngineID, const char *funcName, int errorCode, int logListSize, int logStart, ppRCESLog *logList, int *logCount, int *totalLogs);
int ctLogArchive(pRCESConn replConn, int daysToArchive);

/* Site */
int ctGetSites(pRCESConn replConn, ppRCESSite *siteList, int *siteCount);
int ctGetSite(pRCESConn replConn, const char *name, ppRCESSite site);
int ctPersistSite(pRCESConn replConn, pRCESSite site);
int ctRemoveSite(pRCESConn replConn, int siteID, bool recursive);

/* Hardware */
int ctGetHardwares(pRCESConn replConn, int siteID, ppRCESHardware *hardwareList, int *hardwareCount);
int ctGetHardware(pRCESConn replConn, int siteID, const char *name, ppRCESHardware hardware);
int ctGetHardware(pRCESConn replConn, const char *mac, ppRCESHardware hardware);
int ctPersistHardware(pRCESConn replConn, pRCESHardware hardware);
int ctRemoveHardware(pRCESConn replConn, int hardwareID, bool recursive);

/* OpSystem */
int ctGetOSs(pRCESConn replConn, int hardwareID, ppRCESOpSystem *osList, int *osCount);
int ctGetOS(pRCESConn replConn, int hardwareID, const char *name, ppRCESOpSystem opSystem);
int ctGetOS(pRCESConn replConn, const char *hostName, ppRCESOpSystem opSystem);
int ctGetOSByIP(pRCESConn replConn, const char *ipAddress, ppRCESOpSystem opSystem);
int ctPersistOS(pRCESConn replConn, pRCESOpSystem opSystem);
int ctRemoveOS(pRCESConn replConn, int osID, bool recursive);

/* Volume */
int ctGetVolumes(pRCESConn replConn, int osID, ppRCESVolume *volumeList, int *volumeCount);
int ctGetVolume(pRCESConn replConn, int osID, const char *name, ppRCESVolume volume);
int ctPersistVolume(pRCESConn replConn, pRCESVolume volume);
int ctRemoveVolume(pRCESConn replConn, int volumeID, bool recursive);

/* DBEngine */
int ctGetDBEngines(pRCESConn replConn, int osID, ppRCESDBEngine *dbEngineList, int *dbEngineCount);
int ctGetDBEngine(pRCESConn replConn, int osID, int workingVolume, const char *workingDir, ppRCESDBEngine dbEngine);
int ctPersistDBEngine(pRCESConn replConn, pRCESDBEngine dbEngine);
int ctRemoveDBEngine(pRCESConn replConn, int dbEngineID, bool recursive);
int ctAuthDBEngine(pRCESConn replConn, int dbEngineID, const char *login, const char *pass);
int ctCheckDBEngineStatus(pRCESConn replConn, int dbEngineID, bool checkConn, ppRCESDBEngine *dbEngineList, int *dbEngineCount, int *actionID);
int ctSetDBEngineManaged(pRCESConn replConn, int dbEngineID, bool managed);
int ctSetDBEngineAcceptConn(pRCESConn replConn, int dbEngineID, bool acceptConn);
int ctSetDBEngineReadOnly(pRCESConn replConn, int dbEngineID, bool readOnly);
int ctSetDBEngineStatus(pRCESConn replConn, int dbEngineID, RCESEngineStatus status, bool managed);

/* Database */
int ctGetDatabases(pRCESConn replConn, int dbEngineID, ppRCESDatabase *databaseList, int *databaseCount);
int ctGetDatabase(pRCESConn replConn, int dbEngineID, const char *name, ppRCESDatabase database);
int ctPersistDatabase(pRCESConn replConn, pRCESDatabase database);
int ctRemoveDatabase(pRCESConn replConn, int databaseID);
int ctAddFilesToDatabase(pRCESConn replConn, int databaseId, int *fileList, int fileCount);

/* File */
int ctLoadFiles(pRCESConn replConn, int volumeID, const char *rootTreePath, const char *maskFilter, bool ctreeOnly, bool recursive, bool physicalCheck, bool wait, int *actionID);
int ctGetFiles(pRCESConn replConn, int volumeID, const char *rootTreePath, const char *mask, bool recursive, bool refresh, bool ctreeOnly, int fileListSize, int fileStart, ppRCESFile *fileList, int *fileCount, int *totalFiles);
int ctGetFiles(pRCESConn replConn, int databaseID, int fileListSize, int fileStart, ppRCESFile *fileList, int *fileCount, int *totalFiles);
int ctGetFile(pRCESConn replConn, int volumeID, const char *path, const char *name, pRCESFile *file);
int ctGetFile(pRCESConn replConn, int osID, const char *fullFileName, pRCESFile *file);
int ctPersistFile(pRCESConn replConn, pRCESFile file);
int ctRemoveFile(pRCESConn replConn, int fileID, bool recursive);

/* File Group */
int ctGetFileGroups(pRCESConn replConn, int dbEngineID, ppRCESFileGroup *fileGroupList, int *fileGroupCount);
int ctGetFileGroup(pRCESConn replConn, int dbEngineID, const char *name, ppRCESFileGroup fileGroup);
int ctGetFileGroup(pRCESConn replConn, int fileGroupID, ppRCESFileGroup fileGroup);
int ctPersistFileGroup(pRCESConn replConn, pRCESFileGroup fileGroup);
int ctRemoveFileGroup(pRCESConn replConn, int fileGroupId, bool recursive);
int ctGetFilesFromGroup(pRCESConn replConn, int fileGroupID, int fileListSize, int fileStart, ppRCESFile *fileList, int *fileCount, int *totalFiles);
int ctAddFilesToGroup(pRCESConn replConn, int fileGroupID, int *fileIDList, int fileCount);
int ctRemoveFilesFromGroup(pRCESConn replConn, int fileGroupID, int *fileIDList, int fileCount);

/* Replication Plan */
int ctGetReplPlans(pRCESConn replConn, ppRCESReplPlan *replPlanList, int *replPlanCount);
int ctPersistReplPlan(pRCESConn replConn, pRCESReplPlan replPlan);
int ctRemoveReplPlan(pRCESConn replConn, int replPlanID, bool recursive);
int ctReplGetSubscriptions(pRCESConn replConn, int replPlanID, int replSubsListSize, int replSubsStart, ppRCESSubscription *subsList, int *subsCount, int *totalReplSubs);
int ctReplDeploy(pRCESConn replConn, int replPlanID, int *actionID);
int ctReplStart(pRCESConn replConn, int replPlanID, const char *sourceLogPos, const char *targetLogPos, bool ignoreDeploy, int *actionID);
int ctReplStop(pRCESConn replConn, int replPlanID, int *actionID);
int ctReplCheck(pRCESConn replConn, int replPlanID, bool checkConn, ppRCESReplPlan *replPlanList, int *replPlanCount, int *actionID);
int ctCancelRepl(pRCESConn replConn, int replPlanID, int actionID);
int ctReplStartMonitor(pRCESConn replConn, int replPlanID, int refreshRate);
int ctReplStopMonitor(pRCESConn replConn, int replPlanID);
int ctReplGetStats(pRCESConn replConn, int replPlanID, RCESDateTime begin, RCESDateTime end, int statListSize, int statStart, ppRCESReplStat *statList, int *statCount, int *totalReplStats);
int ctReplGetParallelStats(pRCESConn replConn, int replPlanID, RCESDateTime begin, RCESDateTime end, int statListSize, int statStart, ppRCESReplParallelStat *statList, int *statCount, int *totalReplStats);
int ctReplStartExcepLog(pRCESConn replConn, int replPlanID);
int ctReplStopExcepLog(pRCESConn replConn, int replPlanID);
int ctReplGetExceptions(pRCESConn replConn, int replPlanID, RCESDateTime begin, RCESDateTime end, const char *dumpFileName, int excepListSize, int excepStart, ppRCESReplExcep *excepList, int *excepCount, int *totalReplExceps);
int ctReplCleanExceptions(pRCESConn replConn, int replPlanID, RCESDateTime begin, RCESDateTime end, bool deleteFlag, int *totalReplExceps);
int ctReplPlanFuncTimeStart(pRCESConn replConn, int replPlanID);
int ctReplPlanFuncTimeStop(pRCESConn replConn, int replPlanID);
int ctReplPlanFuncTimeReset(pRCESConn replConn, int replPlanID);
int ctReplPlanGetFuncTimes(pRCESConn replConn, int replPlanID, ppRCESReplFunc *sourceFuncList, int *sourceFuncCount, ppRCESReplFunc *targetFuncList, int *targetFuncCount);
int ctReplPlanPause(pRCESConn replConn, int replPlanID);
int ctReplPlanResume(pRCESConn replConn, int replPlanID);
int ctGetTempReplPlanName(pRCESConn replConn, char *tempReplPlanName);
int ctScanReplAgent(pRCESConn replConn, int osID, const char *basePath, ppRCESReplAgent *replAgentList, int *replAgentCount);
int ctImportReplAgent(pRCESConn replConn, bool simpleMode, ppRCESReplAgent replAgentList, int replAgentCount);
int ctReplPlanAckExceptions(pRCESConn replConn, int replPlanID);
int ctResyncFiles(pRCESConn replConn, int replPlanID, int *fileIDList, int fileCount, bool addNewFiles, int *actionID);
int ctGetArchivedReplPlans(pRCESConn replConn, ppRCESReplPlan *replPlanList, int *replPlanCount);
int ctReplPlanArchive(pRCESConn replConn, int replPlanID);
int ctReplPlanRestore(pRCESConn replConn, int replPlanID);

/* DBEngine Group */
int ctGetDBEngineGroups(pRCESConn replConn, int dGroupListSize, int dGroupStart, ppRCESDBEngineGroup *dGroupList, int *dGroupCount, int *totalDBEngineGroups);
int ctGetDBEngineGroupsByName(pRCESConn replConn, const char *name, bool partial, int dGroupListSize, int dGroupStart, ppRCESDBEngineGroup *dGroupList, int *dGroupCount, int *totalDBEngineGroups);
int ctGetDBEngineGroup(pRCESConn replConn, int dbEngineGroupID, ppRCESDBEngineGroup dbEngineGroup);
int ctPersistDBEngineGroup(pRCESConn replConn, pRCESDBEngineGroup dbEngineGroup);
int ctRemoveDBEngineGroup(pRCESConn replConn, int dbEngineGroupId);
int ctGetDBEnginesFromGroup(pRCESConn replConn, int dbEngineGroupID, int dbEngineListSize, int dbEngineStart, ppRCESDBEngine *dbEngineList, int *dbEngineCount, int *totalDBEngines);
int ctAddDBEnginesToGroup(pRCESConn replConn, int dbEngineGroupID, int *dbEngineIDList, int dbEngineCount);
int ctRemoveDBEnginesFromGroup(pRCESConn replConn, int dbEngineGroupID, int *dbEngineIDList, int dbEngineCount);
int ctGetDBEnginesByGroup(pRCESConn replConn, int dbEngineGroupItemListSize, int dbEngineGroupItemStart, ppRCESDBEngineGroupItem *dbEngineGroupItemList, int *dbEngineGroupItemCount, int *totalDBEngineGroupItems);

/* Publication */
int ctGetPublications(pRCESConn replConn, int dbEngine, int publicationListSize, int publicationStart, ppRCESPublication *publicationList, int *publicationCount, int *totalPublications);
int ctGetPublicationsByName(pRCESConn replConn, int dbEngine, const char *name, bool partial, int publicationListSize, int publicationStart, ppRCESPublication *publicationList, int *publicationCount, int *totalPublications);
int ctGetPublication(pRCESConn replConn, int publicationID, ppRCESPublication publication);
int ctPersistPublication(pRCESConn replConn, pRCESPublication publication);
int ctRemovePublication(pRCESConn replConn, int publicationID, bool recursive);
int ctGetFilesFromPublication(pRCESConn replConn, pRCESPublication publication, int fileListSize, int fileStart, ppRCESFile *fileList, int *fileCount, int *totalFiles);
int ctAddFilesToPublication(pRCESConn replConn, int publicationID, int *fileIDList, int fileCount);
int ctRemoveFilesFromPublication(pRCESConn replConn, int publicationID, int *fileIDList, int fileCount);
int ctGetFileGroupsFromPublication(pRCESConn replConn, int publicationID, int fileGroupListSize, int fileGroupStart, ppRCESFileGroup *fileGroupList, int *fileGroupCount, int *totalFileGroups);
int ctAddFileGroupsToPublication(pRCESConn replConn, int publicationID, int *fileGroupIDList, int fileGroupCount);
int ctRemoveFileGroupsFromPublication(pRCESConn replConn, int publicationID, int *fileGroupIDList, int fileGroupCount);
int ctGetTempPubName(pRCESConn replConn, char *tempPubName);

/* Subscription */
int ctGetSubscriptions(pRCESConn replConn, int publication, int dbEngine, int subscriptionListSize, int subscriptionStart, ppRCESSubscription *subscriptionList, int *subscriptionCount, int *totalSubscriptions);
int ctGetSubscriptionsByName(pRCESConn replConn, int publication, int dbEngine, const char *name, bool partial, int subscriptionListSize, int subscriptionStart, ppRCESSubscription *subscriptionList,
	int *subscriptionCount, int *totalSubscriptions);
int ctGetSubscription(pRCESConn replConn, int subscriptionID, ppRCESSubscription subscription);
int ctPersistSubscription(pRCESConn replConn, pRCESSubscription subscription);
int ctRemoveSubscription(pRCESConn replConn, int subscriptionID);
int ctSubsFilesPreview(pRCESConn replConn, int publicationID, ppRCESReplRedir redirList, int redirListCount, int dbEngineID, int fileListSize, int fileStart, ppRCESFile *fileList, int *fileCount, int *totalFiles);
int ctGetTempSubsName(pRCESConn replConn, char *tempSubsName);

/* Action */
int ctCheckAction(pRCESConn replConn, int actionID, bool checkGroup, ppRCESAction action);
int ctCancelAction(pRCESConn replConn, int actionID, bool checkGroup);
int ctActionArchive(pRCESConn replConn, int daysToArchive);

/* Agent */
int ctGetAgents(pRCESConn replConn, int osID, ppRCESAgent *agentList, int *agentCount);
int ctGetAgent(pRCESConn replConn, int agentID, ppRCESAgent agent);
int ctGetAgentByDBEngine(pRCESConn replConn, int dbEngineID, ppRCESAgent agent);
int ctGetAgentByPath(pRCESConn replConn, int localVolumeID, const char *localDir, ppRCESAgent agent);
int ctPersistAgent(pRCESConn replConn, pRCESAgent agent);
int ctRemoveAgent(pRCESConn replConn, int agentID, bool recursive);
int ctCheckAgentStatus(pRCESConn replConn, int agentID, ppRCESAgent *agentList, int *agentCount);
int ctSetAgentStatus(pRCESConn replConn, int agentID, RCESAgentStatus status);

/* Agent Consumer */
int ctGetAgentConsumers(pRCESConn replConn, int agentID, ppRCESAgentConsumer *agentConsumerList, int *agentConsumerCount);
int ctGetAgentConsumersByDB(pRCESConn replConn, int dbEngineID, ppRCESAgentConsumer *agentConsumerList, int *agentConsumerCount);
int ctLoadActionExec(pRCESConn replConn, int agentID, RCESConsumerType consumerType, const char *libName, int thrdCount, int osID = -1);
int ctLoadActionExecByDB(pRCESConn replConn, int dbEngineID, RCESConsumerType consumerType, const char *libName, int thrdCount);
int ctUnloadActionExec(pRCESConn replConn, int agentID, RCESConsumerType consumerType, int osID = -1);
int ctUnloadActionExecByDB(pRCESConn replConn, int dbEngineID, RCESConsumerType consumerType);

#endif /* _FC_RCES_HPP_ */
