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

/***********************************************************************/
#ifndef CTPLUGIN_H
#define CTPLUGIN_H

#include <stddef.h>

#define CTPLUGIN_DATA_VERSION 1
#define CTPLUGIN_API_VERSION 2

#define CTPLUGIN_MAX_NAME 1024

#define CTPLUGIN_CONFIG_DIR "config"

#define CTPLUGIN_CTAGENT_NAME "ctagent"
#define CTPLUGIN_CTHTTPD_NAME "cthttpd"

#ifdef ctPortWIN32
#define DLLEXPORT __declspec(dllexport)
#else
#define DLLEXPORT
#endif

#define PLUGIN_MAX_ERRMSG 1024
#define PLUGIN_MAX_DESC 1024

/* Plugin configuration file type */
#define PLUGIN_CFGTYPE_CFG  "CFG"
#define PLUGIN_CFGTYPE_JSON "JSON"
#define PLUGIN_CFGTYPE_NONE "NONE"

#define ctUUID_STR_SIZE (8 + 1 + 4 + 1 + 4 + 1 + 4 + 1 + 12 + 1) /* UUID format: 11223344-5566-7788-0102-030405060708 */

typedef enum {
	ctPLUGIN_LIC_MODE_MEMPHISNODE = 1, /* ctAgent license (Memphis Node) */
	ctPLUGIN_LIC_MODE_HIGHAVAIL = 2,   /* High Availability license */
	ctPLUGIN_LIC_MODE_HTTPD = 3,       /* HTTPD license */
	ctPLUGIN_LIC_MODE_MQTT = 4,        /* MQTT license */
	ctPLUGIN_LIC_MODE_REST = 5,        /* REST license */
	ctPLUGIN_LIC_MODE_THINGWORX = 6,   /* Thingworx license */
	ctPLUGIN_LIC_MODE_OPC = 7,         /* OPC license */
	ctPLUGIN_LIC_MODE_TIMESTAMP = 8,   /* Timestamp (aggregation) license */
	ctPLUGIN_LIC_MODE_MEMGRID = 9,     /* Memory Grid license */
	ctPLUGIN_LIC_MODE_REPL = 10,        /* Classic replication agent */
	ctPLUGIN_LIC_MODE_modbus = 11,	   /* modbus license */
	ctPLUGIN_LIC_MODE_siemensUDT = 12, /* siemensUDT license */
	ctPLUGIN_LIC_MODE_MQTTNotify = 13 /* MQTTNotify license */
} ctPLUGIN_LIC_MODE;

typedef enum
{
	PLUGIN_LOADED = 0,
	PLUGIN_INITIALIZING = 1,
	PLUGIN_INITIALIZED_NOTUSED = 2,
	PLUGIN_STARTING = 3,
	PLUGIN_STARTED = 4,
	PLUGIN_STOPING = 5,
	PLUGIN_TERMINATING = 6,
	PLUGIN_PROMOTING = 7
} ctPLUGIN_STATUS;

/* ctplugin_load() error logging modes */
typedef enum ctplugin_load_error_logging_mode
{
	ctPLUGIN_LOG_ERRORS = 0, /* log all errors */
	ctPLUGIN_SUPPRESS_ALL_ERRORS = 1, /* suppress all errors */
	ctPLUGIN_SUPPRESS_LOADING_PLUGIN_NOT_IN_CONFIG = 2 /* suppress only error about loading a plugin that is not in config file */
} ctPLUGIN_LOAD_ERROR_LOGGING_MODE;

/* plugin status values that indicate that the plugin has been started */
#define ctPLUGIN_HAS_BEEN_STARTED(status) ((status) == PLUGIN_STARTING || (status) == PLUGIN_STARTED || (status) == PLUGIN_PROMOTING)

/* plugin status values that indicate that the plugin is starting */
#define ctPLUGIN_IS_STARTING(status) ((status) == PLUGIN_INITIALIZING || (status) == PLUGIN_STARTING || (status) == PLUGIN_PROMOTING)

typedef int(*p_ctplugin_checkLicense)(ctPLUGIN_LIC_MODE);
int ctplugin_checkLicense(ctPLUGIN_LIC_MODE mode);

typedef void(*p_ctrelease_taskId)(void);
void ctplugin_releaseTaskId(void);

typedef int(*p_ctPluginExec)(int, char *, char *, long *);

struct tag_plugin; /* forward declaration */

typedef struct tag_pluginData {
	int    version;     /* Plugin data version */
	int    isembed;     /* Plugin is running embedded in a server - some plugins can run also in standalone mode */
	int    loadmode;    /* Plugin load mode: before or after server startup */
	char  *localPath;   /* Server's local path */
	char  *srvnam;      /* Server's name */
#if defined(ctPortWIN32) || defined(_WINDOWS)
	char   jvmLibName[CTPLUGIN_MAX_NAME + 1];  /* JVM library name */
#else
	char  *jvmLibName;  /* JVM library name */
#endif
	int   *readonly;    /* Server's readonly flag */
	int   *cttflg;      /* Flag indicating if the server is active or shutting down */
	void  *context;     /* Generic buffer for send and receive information from and to plugin */
	p_ctplugin_checkLicense  licenseCheckerFunc; /* License check function pointer */
	p_ctrelease_taskId releaseTaskIdFunc; /* Release task id function pointer */
	int    sqlPort;     /* SQL port */
	char   isamPort[256]; /* Server port/name*/
	char  *cfgFile;     /* ctsrvr.cfg file path */
	char  *licenseFile; /* license file path */
	char  *defaultDB;   /* Default SQL database */
	void *pluginList; /* List of runnable plugins */
	p_ctPluginExec ctPluginExec;
	void *pch;  /* plugin channel handle */
	char* configDir;
	char uuid[ctUUID_STR_SIZE];
	struct tag_plugin* plugin;
} CTPLUGINDATA, * pCTPLUGINDATA;

typedef struct tag_pluginInfo {
	int         version;                            /* Plugin info version */
	char        description[PLUGIN_MAX_DESC + 1];   /* Plugin description */
	const char *cfgFileType;                        /* Type of configuration file: JSON, CFG, etc */
	char        cfgFilePath[CTPLUGIN_MAX_NAME + 1]; /* Plugin configuration file path */
	p_ctplugin_checkLicense licChecker; 		/* License File Check Function Pointer */
} CTPLUGININFO, *pCTPLUGININFO;

typedef int(*p_logger)(const char *, int);
typedef short int (*p_ctLogMsgAdvlog)(int id, char audience, int severity, int userid,const char * oldmsg, int error, int errorContext, int cterr, int syserr,const char * function,int action,const char * codeloc);

#ifdef ctFeatNEWctrcvlogPLUGIN
typedef int(*p_ctplugin_init)(pCTPLUGININFO, void *, const char *, int apiversion);
#else
typedef int(*p_ctplugin_init)(pCTPLUGININFO, p_logger, const char *, const char *);
#endif
typedef int(*p_ctplugin_term)(void);
typedef int(*p_ctplugin_start)(pCTPLUGINDATA);
typedef int(*p_ctplugin_stop)(void);
typedef int(*p_ctplugin_call)(int, const char *, char *, int *);

typedef struct tag_pluginFuncs {
	p_ctplugin_init  init;
	p_ctplugin_term  term;
	p_ctplugin_start start;
	p_ctplugin_stop  stop;
	p_ctplugin_call  call;
} CTPLUGINFUNCS;

typedef int(*p_callback_init)(void);
typedef int(*p_callback_term)(void);
typedef int(*p_callback_desc)(char *,size_t *,int);

struct callbackdesc {
	char * name;
	void * addr;
};

struct callableFuncs {
	p_callback_init init;   /* global scope init */
	p_callback_term term;   /* global scopre term */
	p_callback_desc describe;  /* describe functions number and names */
	struct callbackdesc * functions; /* callbackdesc array */
	size_t num; /* number of elements in functions */
};

typedef void(*p_ctplugin_updatestatus)(struct tag_plugin*, ctPLUGIN_STATUS);

typedef struct tag_plugin {
	char name[CTPLUGIN_MAX_NAME];
	char path[CTPLUGIN_MAX_NAME];
	union {
		CTPLUGINFUNCS functions;
		struct callableFuncs cb;
	} u;
	void *handle;
	int type;
	int errorCd;
	pCTPLUGINDATA data;
	pCTPLUGININFO info;
	ctPLUGIN_STATUS status;
	void* mutex;
	p_ctplugin_updatestatus ctplugin_updatestatus;
} CTPLUGIN, *pCTPLUGIN, **ppCTPLUGIN;

typedef struct tag_plugin_list {
	ppCTPLUGIN list;
	unsigned int count; /* number of elements */
	int (*load_fn)(pCTPLUGIN,int,char *,size_t);
	void (*unload_fn)(pCTPLUGIN);
} CTPLUGIN_LIST;

int ctplugin_load(int, const char *name, int mode, ctPLUGIN_LOAD_ERROR_LOGGING_MODE errorLoggingMode);
static int ctplugin_load_services();
void ctplugin_unload(int);
void ctplugin_stop(const char *name);
DLLEXPORT int ctplugin_get(const char *pluginName, int wait, ppCTPLUGIN plugin);
DLLEXPORT int ctplugin_trylock();
DLLEXPORT void ctplugin_release();
int ctplugin_start_runnable(pCTPLUGIN plugin, int mode, char * msg, size_t msglen);
int ctplugin_stop_runnable(pCTPLUGIN plugin, char * msg, size_t msglen);
int ctplugin_call_runnable(pCTPLUGIN plugin, int command, char * args, char * outputBuffer, int * pOutputBufferSize);
int ctplugin_waitForInit(int type);

#endif /* CTPLUGIN_H */
