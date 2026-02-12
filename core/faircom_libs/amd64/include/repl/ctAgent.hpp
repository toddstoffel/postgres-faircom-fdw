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

#ifndef _CTAGENT_HPP_
#define _CTAGENT_HPP_

#include "fcrcesaux.hpp"
#include "ctplugin.h"
#include "fcrcesreplplan.hpp"
#include "fcrcesrecblock.hpp"
#include "fcrcesaction.hpp"

#define CTAGENT_DESCRIPTION "Replication Manager agent"
#define CTAGENT_CFG_TYPE PLUGIN_CFGTYPE_JSON

typedef enum
{
	AGENT_START = 0,
	AGENT_STARTED = 1,
	AGENT_STOP = 2,
	AGENT_STOPED = 3
} AgentState;

typedef struct ctagentthd {
	AgentState state;

	pCTPLUGINDATA ctPluginData;

	char *logfile;
	char *memphistype;
	char *memphisname;
	char *memphisport;
	char *memphissqlport;
	char *memphishost;
	char *memphisdb;
	int memphismaxconn;

	int qthrdcount;
	int ctcheck;
	char *ctcheckmask;
	int dbcheck;
	int verscancount;
	int vershowname;
	int connpoolsize;
	int scanthrdcount;
	int loadthrdcount;
	int advfullmask;
	int trace;
	int webtrace;
	char *ipAddress;
	int acceptconn;
	char *jvmLibPath;

	int managed;
	char *cfgFileList;
	int ipv6Support;
	int addPublicIP;
	int usereplicationagentmanager;
	int dbengineBehindNAT;
} CTAGENTTHD, * pCTAGENTTHD;

/* call plugin modes */
typedef enum ctAgentPluginOption_t {

	INVALID_CALL     = -1, /* invalid operation */

	/* Availability Group and Replication options */
	GET_SERVER_LIST  = 1,  /* get the server list */
	GET_SERVER_ROLE  = 2,  /* get the server role */
	IS_AGENT_RUNNING = 3,  /* check if agent is running */
	START_AGENT      = 4,  /* start an unmanaged replication agent */
	STOP_AGENT       = 5,  /* stop an unmanaged replication agent */

	/* Web server options */
	UPD_LOCAL_WEB_SERVER = 10,
	CHECK_MASTER_SERVER  = 11,
	GET_AGENT_CONFIG = 12,

	/* Failover/Cluster options */
	IS_AVAILABILITY_GROUP_MEMBER = 13,
	GET_SYNC_STATE = 14,
	SET_SYNC_STATE = 15,
	GET_RECOVERY_INFO_FROM_PRIMARY = 16,
	GET_RECOVERY_JSON_FILE_NAME = 17,
	GET_REPL_PLAN_NAME_PREFIX = 18,
	LOAD_FAILOVER_MEM_GRID = 19
} ctAGENT_PLUGIN_OPTION;

#define ctAGENT_GET_SERVER_LIST      "getserverlist"
#define ctAGENT_GET_SERVER_ROLE      "getserverrole"
#define ctAGENT_IS_AGENT_RUNNING     "isagentrunning"
#define ctAGENT_START_AGENT          "start"
#define ctAGENT_STOP_AGENT           "stop"
#define ctAGENT_UPD_LOCAL_WEB_SERVER "updlocalwebserver"
#define ctAGENT_CHECK_MASTER_SERVER  "checkmasterserver"
#define ctAGENT_GET_AGENT_CONFIG     "getAgentConfig"
#define ctAGENT_IS_AVAILABILITY_GROUP_MEMBER "isAvailabilityGroupMember"
#define ctAGENT_GET_SYNC_STATE	     "getsyncstate"
#define ctAGENT_SET_SYNC_STATE	     "setsyncstate"
#define ctAGENT_GET_RECOVERY_INFO_FROM_PRIMARY "getRecoveryInfoFromPrimary"
#define ctAGENT_GET_RECOVERY_JSON_FILE_NAME "getRecoveryJsonFileName"
#define ctAGENT_GET_REPL_PLAN_NAME_PREFIX "getReplPlanNamePrefix"
#define ctAGENT_LOAD_FAILOVER_MEM_GRID "loadFailoverMemoryGrid"

#define SITE "local"

typedef short int(*p_ctrcvlog)(char *, int);
extern p_logger agentlog;

/* Ops Manager agent export symbols */
extern "C" rcesEXPORT int ctPlugin_init(pCTPLUGININFO pluginInfo, p_logger logger, const char *path, const char *configDir);
extern "C" rcesEXPORT int ctPlugin_term();
extern "C" rcesEXPORT int ctPlugin_start(pCTPLUGINDATA ctPluginData);
extern "C" rcesEXPORT int ctPlugin_stop();
extern "C" rcesEXPORT int ctPlugin_call(int operation, const char *input, char *output, int *outputSize);

void EmbLog(const char *msg, int errCd);

#endif /* _CTAGENT_HPP_ */