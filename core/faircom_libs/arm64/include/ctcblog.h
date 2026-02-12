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

#ifndef _CTCBLOG_H_
#define _CTCBLOG_H_

#include "ctcboptn.h"
#include "ctcbconf.h"
#include "ctcbmshl.h"
#include "ctcbstdr.h"

#ifndef __FUNCTION__
#if ((defined(_MSC_VER) && (_MSC_VER <= 1200)) || defined(ctPortNCRUNIX386) || defined(ctPortBATA_SCO5) )
#define __FUNCTION__ (cpTEXT)NULL
#endif
#endif

#ifndef __FUNCTION__
#define __FUNCTION__ (cpTEXT)__func__
#endif

typedef struct tagRTGLOG LOGOBJ, * pLOGOBJ;
struct tagRTGLOG {
	pTEXT name;
	pCT_FILE ctfile;
	pCT_INSTANCE ctinst;
#ifdef ctFeatRTG_HANDLE
	pCT_INIT conn;
	cpTEXT fname;
	cpTEXT mod;
	cpTEXT func;
	UINT line;
	CT_ERROR err;
#else /* ~ctFeatRTG_HANDLE */
#ifdef ctFeatRTG_LOGWHOFORMAT
	pTEXT whofmt;
#endif
#ifdef ctFeatRTG_TASKID
	RTGTASKID taskid;
#endif
#endif /* ~ctFeatRTG_HANDLE */
};

typedef enum LOGEVENT {
	NoneEvent    = 0x00,
	ErrorEvent   = 0x01,
	WarningEvent = 0x02,
	InfoEvent    = 0x04,
	ProfileEvent = 0x08,
	DebugEvent   = 0x10,
	notyet1Event = 0x20,
	notyet0Event = 0x40
	/* 0x80 cannot be used as it would conflict with the "default" marker in config logic */
	/* only 7 events available at the moment (TEXT bitmask) */
} LOGEVENT;

#define AllEvents (ErrorEvent | WarningEvent | InfoEvent | ProfileEvent | DebugEvent)
#define DefaultEvents (ErrorEvent | WarningEvent | InfoEvent)

typedef enum LOGEXTRAEVENT {
	NoExtraEvent       = 0x00000000,
	NoErrorEvent       = NoExtraEvent,
	NoDebugEvent       = NoExtraEvent,
	/* error events */
	GenericErrorEvent     = 0x00000001,
	AtendErrorEvent       = 0x00000002,
	NotfoundErrorEvent    = 0x00000004,
	DuplicateErrorEvent   = 0x00000008,
	UndefinedErrorEvent   = 0x00000010,
	LockedErrorEvent      = 0x00000020,
	MissingfileErrorEvent = 0x00000040,
	/* debug events */
	GenericDebugEvent  = 0x00000001,
	ConfigDebugEvent   = 0x00000002,
	PrefetchDebugEvent = 0x00000004,
	BatchaddDebugEvent = 0x00000008,
	EXTFHDebugEvent    = 0x00000010,
	FileDebugEvent     = 0x00000020,
	SwitcherDebugEvent = 0x00000040,
	TranDebugEvent     = 0x00000080,
	SqlizeDebugEvent   = 0x00000100,
	StartonreadDebugEvent = 0x00000200,
	LockDebugEvent     = 0x00000400,
	ClusterDebugEvent = 0x00000800,

	AllErrorEvents     = 0x7FFFFFFF,
	AllDebugEvents     = 0x7FFFFFFF
	/* 0x80000000 cannot be used as it would conflict with the "default" marker in config logic */
	/* only 31 events available at the moment (LONG bitmask) */
} LOGEXTRAEVENT;

#define FrequentErrorEvents (AtendErrorEvent | NotfoundErrorEvent | DuplicateErrorEvent)
#define DefaultErrorEvents (AllErrorEvents & ~FrequentErrorEvents)

#define DefaultDebugEvents (GenericDebugEvent)

#define RECPOSfmt "0x%08X"
#define RECPOSHfmt "0x%X%08X"

#ifdef ctFeatRTG_HANDLE
VOID rtgLogHandle(CTHANDLE handle, ppRTGCONF pconf, ppCT_INIT psess, ppCT_INSTANCE pinst, ppCT_FILE pfile);
pTEXT rtgLogFileName(pLOGOBJ log, CTHANDLE handle, cpTEXT name, LOGEVENT event, LOGEXTRAEVENT xevent);
TEXT rtgLogIsEnabled(pLOGOBJ log, CTHANDLE handle, cpTEXT name, LOGEVENT event, LOGEXTRAEVENT xevent);
NINT rtgLog(pLOGOBJ log, CTHANDLE handle, cpTEXT name, LOGEVENT event, LOGEXTRAEVENT xevent, cpTEXT format, ...);
NINT rtgLogError(pLOGOBJ log, CTHANDLE handle, cpTEXT name, pCT_ERROR err, pTEXT errmsg, pTEXT logmsg);
NINT rtgLogProfile(pLOGOBJ log, CTHANDLE handle, cpTEXT name, LONG8 value, pTEXT logmsg);
NINT rtgLogDebug(pLOGOBJ log, CTHANDLE handle, cpTEXT name, LOGEXTRAEVENT dbgev, pTEXT dump, pTEXT logmsg);
#endif /* ctFeatRTG_HANDLE */

pTEXT ctcbLogFileName(pRTGCONF conf, pCONFINST inst, pCONFFILE file, pCT_INPUT input, LOGEVENT evnt, LOGEXTRAEVENT xevnt);
TEXT ctcbLogIsEnabled(pRTGCONF conf, pCONFINST inst, pCONFFILE file, pCT_INPUT input, LOGEVENT evnt, LOGEXTRAEVENT errev);
NINT ctcbLogWrite(pTEXT logname, pTEXT msg0, pTEXT msg, pTEXT msg2);
pTEXT ctcbLogThread(pTEXT tid, size_t tidlen);
#ifdef ctFeatRTG_LOGWHOFORMAT
#ifdef ctFeatRTG_TASKID
VOID ctcbLogWho(pTEXT who, UINT wholen, pTEXT whofmt, pRTGCONF conf, pCONFINST inst, pCONFFILE file, pRTGTASKID taskid);
#else
VOID ctcbLogWho(pTEXT who, UINT wholen, pTEXT whofmt, pRTGCONF conf, pCONFINST inst, pCONFFILE file);
#endif
#endif
LOGEXTRAEVENT ctcbLogGetErrorEvent(COUNT logical);
#ifdef ctFeatRTG_TASKID
NINT ctcbLog(pRTGCONF conf, pCONFINST inst, pCONFFILE file, pCT_INPUT input, cpTEXT mod, UINT line, cpTEXT func, pRTGTASKID taskid, LOGEVENT evnt, LOGEXTRAEVENT errev, const char* format, ...);
NINT ctcbLogError(pRTGCONF conf, pCONFINST inst, pCONFFILE file, pCT_INPUT input, cpTEXT mod, UINT line, cpTEXT func, pRTGTASKID taskid, CT_ERROR *err, COUNT logical, LONG ctree, LONG system, pTEXT errmsg, pTEXT mesg);
NINT ctcbLogProfile(pRTGCONF conf, pCONFINST inst, pCONFFILE file, pCT_INPUT input, cpTEXT mod, UINT line, cpTEXT func, pRTGTASKID taskid, LONG8 value, pTEXT mesg);
NINT ctcbLogDebug(pRTGCONF conf, pCONFINST inst, pCONFFILE file, pCT_INPUT input, cpTEXT mod, UINT line, cpTEXT func, pRTGTASKID taskid, LOGEXTRAEVENT dbgev, pTEXT dump, pTEXT mesg);
#else
NINT ctcbLog(pRTGCONF conf, pCONFINST inst, pCONFFILE file, pCT_INPUT input, cpTEXT mod, UINT line, cpTEXT func, LOGEVENT evnt, LOGEXTRAEVENT errev, const char* format, ...);
NINT ctcbLogError(pRTGCONF conf, pCONFINST inst, pCONFFILE file, pCT_INPUT input, cpTEXT mod, UINT line, cpTEXT func, CT_ERROR *err, COUNT logical, LONG ctree, LONG system, pTEXT errmsg, pTEXT mesg);
NINT ctcbLogProfile(pRTGCONF conf, pCONFINST inst, pCONFFILE file, pCT_INPUT input, cpTEXT mod, UINT line, cpTEXT func, LONG8 value, pTEXT mesg);
NINT ctcbLogDebug(pRTGCONF conf, pCONFINST inst, pCONFFILE file, pCT_INPUT input, cpTEXT mod, UINT line, cpTEXT func, LOGEXTRAEVENT dbgev, pTEXT dump, pTEXT mesg);
#endif
#if !defined MULTITRD && defined ctFeatRTG_API
NINT rtg_message_func(pTEXT msg);
#endif

LOCAL pCONFINST ctcbLogGetInst(pCT_INIT conn, pCONFINST inst)
{
	if (inst == NULL)
		inst = (conn && conn->a_instance >= 0) ? conn->instance[conn->a_instance].conf : NULL;
	return inst;
}
LOCAL pCONFINST rtgLogGetInst(pCT_INIT conn, pCT_INSTANCE inst)
{
	pCONFINST conf = (inst) ? inst->conf : NULL;
	if (conf == NULL)
		conf = (conn && conn->a_instance >= 0) ? conn->instance[conn->a_instance].conf : NULL;
	return conf;
}

#define LOGBUFSIZ 256
#ifdef ctFeatRTG_HANDLE
#define LogVars \
	LOGOBJ LogObject; \
	TEXT LogBuffer[LOGBUFSIZ]
#else /* ~ctFeatRTG_HANDLE */
#define LogVars \
	char LogBuffer[LOGBUFSIZ]
#endif /* ~ctFeatRTG_HANDLE */
#define LogBuf LogBuffer,LOGBUFSIZ

#define rtgLOGSPRINTF(X) {rtgSNPRINTF X; LogBuffer[LOGBUFSIZ - 1] = '\0';}

#if !defined rtgSERVERCORE && defined ctFeatRTG_API
#ifdef ctThrdApp
/* ctWNGV is not defined in multi-thread models */
#define GETctWNGV pCTGVAR ctWNGV = (pCTGVAR)ctThrdGet()
#else
#define GETctWNGV
#endif
#endif

VOID rtgCLog(pRTGCONF conf, cpTEXT mod, NINT line, cpTEXT func, pTEXT logbuf);
VOID rtgCLogError(pRTGCONF conf, cpTEXT mod, NINT line, cpTEXT func, pCT_ERROR err, COUNT logical, NINT ctree, NINT system, pTEXT logbuf);
VOID rtgCLogWarning(pRTGCONF conf, cpTEXT mod, NINT line, cpTEXT func, pTEXT logbuf);
VOID rtgCLogProfile(pRTGCONF conf, cpTEXT mod, NINT line, cpTEXT func, LONG8 value, pTEXT logbuf);
VOID rtgCLogDebug(pRTGCONF conf, cpTEXT mod, NINT line, cpTEXT func, LOGEXTRAEVENT dbgev, pTEXT logbuf);
VOID rtgCLogDebugConfig(pRTGCONF conf, cpTEXT mod, NINT line, cpTEXT func, pTEXT dump, pTEXT logbuf);
VOID rtgCLogDebugExtfh(pRTGCONF conf, cpTEXT mod, NINT line, cpTEXT func, pTEXT dump, pTEXT logbuf);
VOID rtgGLog(pCT_INIT conn, cpTEXT mod, NINT line, cpTEXT func, pTEXT logbuf);
VOID rtgGLogError(pCT_INIT conn, cpTEXT mod, NINT line, cpTEXT func, pCT_ERROR err, COUNT logical, NINT ctree, NINT system, pTEXT logbuf);
VOID rtgGLogErrorMsg(pCT_INIT conn, cpTEXT mod, NINT line, cpTEXT func, pCT_ERROR err, COUNT logical, NINT ctree, NINT system, pTEXT errmsg, pTEXT logbuf);
VOID rtgGLogWarning(pCT_INIT conn, cpTEXT mod, NINT line, cpTEXT func, pTEXT logbuf);
VOID rtgGLogProfile(pCT_INIT conn, cpTEXT mod, NINT line, cpTEXT func, LONG8 value, pTEXT logbuf);
VOID rtgGLogDebug(pCT_INIT conn, cpTEXT mod, NINT line, cpTEXT func, LOGEXTRAEVENT dbgev, pTEXT logbuf);
VOID rtgILog(pCT_INIT conn, pCT_INSTANCE inst, cpTEXT mod, NINT line, cpTEXT func, pTEXT logbuf);
VOID rtgILogError(pCT_INIT conn, pCT_INSTANCE inst, cpTEXT mod, NINT line, cpTEXT func, COUNT logical, NINT ctree, NINT system, pTEXT logbuf);
VOID rtgILogWarning(pCT_INIT conn, pCT_INSTANCE inst, cpTEXT mod, NINT line, cpTEXT func, pTEXT logbuf);
VOID rtgILogDebug(pCT_INIT conn, pCT_INSTANCE inst, cpTEXT mod, NINT line, cpTEXT func, LOGEXTRAEVENT dbgev, pTEXT logbuf);
VOID rtgFLog(pCT_INIT conn, pCONFFILE file, cpTEXT mod, NINT line, cpTEXT func, pTEXT logbuf);
VOID rtgFLogError(pCT_INIT conn, pCONFFILE file, cpTEXT mod, NINT line, cpTEXT func, COUNT logical, NINT ctree, NINT system, pTEXT logbuf);
VOID rtgFLogWarning(pCT_INIT conn, pCONFFILE file, cpTEXT mod, NINT line, cpTEXT func, pTEXT logbuf);
VOID rtgFLogProfile(pCT_INIT conn, pCONFFILE file, cpTEXT mod, NINT line, cpTEXT func, LONG8 value, pTEXT logbuf);
VOID rtgFLogDebug(pCT_INIT conn, pCONFFILE file, cpTEXT mod, NINT line, cpTEXT func, LOGEXTRAEVENT dbgev, pTEXT logbuf);
#if !defined MULTITRD && defined ctFeatRTG_API
VOID rtgFLogErrorX(pCT_INIT conn, pCONFFILE file, pCT_FILE _ctfile, cpTEXT mod, NINT line, cpTEXT func, COUNT logical, NINT ctree, NINT system, pTEXT logbuf);
VOID rtgFLogWarningX(pCT_INIT conn, pCONFFILE file, pCT_FILE ct, cpTEXT mod, NINT line, cpTEXT func, pTEXT logbuf);
VOID rtgFLogProfileX(pCT_INIT conn, pCONFFILE file, pCT_FILE ct, cpTEXT mod, NINT line, cpTEXT func, LONG8 value, pTEXT logbuf);
VOID rtgFLogDebugX(pCT_INIT conn, pCONFFILE file, pCT_FILE ct, cpTEXT mod, NINT line, cpTEXT func, LOGEXTRAEVENT dbgev, pTEXT logbuf);
#endif
VOID rtgSLogError(pCT_INPUT in, cpTEXT mod, NINT line, cpTEXT func, pCT_ERROR err, COUNT logical, NINT ctree, NINT system, pTEXT logbuf);
VOID rtgSLogWarning(pCT_INPUT in, cpTEXT mod, NINT line, cpTEXT func, pTEXT logbuf);
VOID rtgSLogDebug(pCT_INPUT in, cpTEXT mod, NINT line, cpTEXT func, LOGEXTRAEVENT dbgev, pTEXT logbuf);


#define LOGReset rtgMEMZERO(&LogObject, rtgSIZEOF(LogObject)); LogObject.mod = __MODULE__; LogObject.line = __LINE__; LogObject.func = __FUNCTION__;

#ifdef ctFeatRTG_HANDLE
#define LOGEnabled(handle) \
		(rtgLogIsEnabled(&LogObject, handle, NULL, InfoEvent, NoExtraEvent))
#define LOG(handle, arg) \
	{ \
		LOGReset \
		if (LOGEnabled(handle)) { \
			rtgLOGSPRINTF(arg); \
			rtgLog(&LogObject, handle, NULL, InfoEvent, NoExtraEvent, LogBuffer); \
		} \
	}

#define LOGErrorEnabled(handle, name, errev) \
		(rtgLogIsEnabled(&LogObject, handle, name, ErrorEvent, errev))
#define LOGError(handle, _logical, _ctree, _system, arg) \
	{ \
		LOGReset \
		if (LOGErrorEnabled(handle, NULL, AllErrorEvents)) { \
			LogObject.err.logical = _logical; \
			LogObject.err.ctree = _ctree; \
			LogObject.err.system = _system; \
			rtgLOGSPRINTF(arg); \
			rtgLogError(&LogObject, handle, NULL, NULL, NULL, LogBuffer); \
		} else if (LogObject.conn != NULL) { \
			pCT_INIT _conn = rtgGetHandle(handle, HANDLEID_SESSION); \
			ctcbSetError(&_conn->err, _logical, _ctree, _system, _conn->errmsg, LogBuffer); \
		} \
	}
#define LOGErrorX(handle, _logical, _ctree, _system, _err, _errmsg, arg) \
	{ \
		LOGReset \
		if (LOGErrorEnabled(handle, NULL, AllErrorEvents)) { \
			LogObject.err.logical = _logical; \
			LogObject.err.ctree = _ctree; \
			LogObject.err.system = _system; \
			rtgLOGSPRINTF(arg); \
			rtgLogError(&LogObject, handle, NULL, _err, _errmsg, LogBuffer); \
		} else { \
			ctcbSetError(_err, _logical, _ctree, _system, _errmsg, LogBuffer); \
		} \
	}
#define LOGErrorByName(handle, name, _logical, _ctree, _system, arg) \
	{ \
		LOGReset \
		if (LOGErrorEnabled(handle, name, AllErrorEvents)) { \
			LogObject.err.logical = _logical; \
			LogObject.err.ctree = _ctree; \
			LogObject.err.system = _system; \
			rtgLOGSPRINTF(arg); \
			rtgLogError(&LogObject, handle, name, NULL, NULL, LogBuffer); \
		} else if (LogObject.conn != NULL) { \
			ctcbSetError(&LogObject.conn->err, _logical, _ctree, _system, LogObject.conn->errmsg, LogBuffer); \
		} \
	}

#define LOGWarningEnabled(handle, name) \
		(rtgLogIsEnabled(&LogObject, handle, name, WarningEvent, NoExtraEvent))
#define LOGWarning(handle, arg) \
	{ \
		LOGReset \
		if (LOGWarningEnabled(handle, NULL)) { \
			rtgLOGSPRINTF(arg); \
			rtgLog(&LogObject, handle, NULL, WarningEvent, NoExtraEvent, LogBuffer); \
		} \
	}
#define LOGWarningByName(handle, name, arg) \
	{ \
		LOGReset \
		if (LOGWarningEnabled(handle, name)) { \
			rtgLOGSPRINTF(arg); \
			rtgLog(&LogObject, handle, name, WarningEvent, NoExtraEvent, LogBuffer); \
		} \
	}

#define LOGProfileEnabled(handle) \
		(rtgLogIsEnabled(&LogObject, handle, NULL, ProfileEvent, NoExtraEvent))
#define LOGProfile(handle, value, arg) \
	{ \
		LOGReset \
		if (LOGProfileEnabled(handle)) { \
			rtgLOGSPRINTF(arg); \
			rtgLogProfile(&LogObject, handle, NULL, value, LogBuffer); \
		} \
	}

#define LOGDebugEnabled(handle, name, dbgev) \
		(rtgLogIsEnabled(&LogObject, handle, name, DebugEvent, dbgev))
#define LOGDebug(handle, dbgev, dump, arg) \
	{ \
		LOGReset \
		if (LOGDebugEnabled(handle, NULL, dbgev)) { \
			rtgLOGSPRINTF(arg); \
			rtgLogDebug(&LogObject, handle, NULL, dbgev, dump, LogBuffer); \
		} \
	}
#define LOGDebugByName(handle, name, dbgev, dump, arg) \
	{ \
		LOGReset \
		if (LOGDebugEnabled(handle, name, dbgev)) { \
			rtgLOGSPRINTF(arg); \
			rtgLogDebug(&LogObject, handle, name, dbgev, dump, LogBuffer); \
		} \
	}
#endif /* ctFeatRTG_HANDLE */

/* log macros by configuration */
#define CLogEnabled(conf) \
		ctcbLogIsEnabled(conf, NULL, NULL, NULL, InfoEvent, NoExtraEvent)
#define CLog(conf, arg) \
	{ \
		if (CLogEnabled(conf)) { \
			rtgLOGSPRINTF(arg); \
			rtgCLog(conf, __MODULE__, __LINE__, __FUNCTION__, LogBuffer); \
		} \
	}
#define CLogErrorEnabled(conf, errev) \
		ctcbLogIsEnabled(conf, NULL, NULL, NULL, ErrorEvent, errev)
#define CLogError(conf, err, logical, ctree, system, arg) \
	{ \
		if (CLogErrorEnabled(conf, AllErrorEvents)) { \
			rtgLOGSPRINTF(arg); \
			rtgCLogError(conf, __MODULE__, __LINE__, __FUNCTION__, err, logical, ctree, system, LogBuffer); \
		} else { \
			ctcbSetError(err, logical, ctree, system, NULL, NULL); \
		} \
	}
#define CLogWarningEnabled(conf) \
		ctcbLogIsEnabled(conf, NULL, NULL, NULL, WarningEvent, NoExtraEvent)
#define CLogWarning(conf, arg) \
	{ \
		if (CLogWarningEnabled(conf)) { \
			rtgLOGSPRINTF(arg); \
			rtgCLogWarning(conf, __MODULE__, __LINE__, __FUNCTION__, LogBuffer); \
		} \
	}
#define CLogProfileEnabled(conf) \
		ctcbLogIsEnabled(conf, NULL, NULL, NULL, ProfileEvent, NoExtraEvent)
#define CLogProfile(conf, value, arg) \
	{ \
		if (CLogProfileEnabled(conf)) { \
			rtgLOGSPRINTF(arg); \
			rtgCLogProfile(conf, __MODULE__, __LINE__, __FUNCTION__, value, LogBuffer); \
		} \
	}
#define CLogDebugEnabled(conf, dbgev) \
		ctcbLogIsEnabled(conf, NULL, NULL, NULL, DebugEvent, dbgev)
#define CLogDebug(conf, dbgev, arg) \
	{ \
		if (CLogDebugEnabled(conf, AllDebugEvents)) { \
			rtgLOGSPRINTF(arg); \
			rtgCLogDebug(conf, __MODULE__, __LINE__, __FUNCTION__, dbgev, LogBuffer); \
		} \
	}
#define CLogDebugConfig(conf, dump, arg) \
	{ \
		if (CLogDebugEnabled(conf, ConfigDebugEvent)) { \
			rtgLOGSPRINTF(arg); \
			rtgCLogDebugConfig(conf, __MODULE__, __LINE__, __FUNCTION__, dump, LogBuffer); \
		} \
	}
#define CLogDebugExtfh(conf, dump, arg) \
	{ \
		if (CLogDebugEnabled(conf, EXTFHDebugEvent)) { \
			rtgLOGSPRINTF(arg); \
			rtgCLogDebugExtfh(conf, __MODULE__, __LINE__, __FUNCTION__, dump, LogBuffer); \
		} \
	}

/* log macros by connection */
#define LogEnabled(conn) \
		(conn && ctcbLogIsEnabled(conn->conf, ctcbLogGetInst(conn, NULL), NULL, NULL, InfoEvent, NoExtraEvent))
#define Log(conn, arg) \
	{ \
		if (LogEnabled(conn)) { \
			rtgLOGSPRINTF(arg); \
			rtgGLog(conn, __MODULE__, __LINE__, __FUNCTION__, LogBuffer); \
		} \
	}
#define LogErrorEnabled(conn, errev) \
		(conn && ctcbLogIsEnabled(conn->conf, ctcbLogGetInst(conn, NULL), NULL, NULL, ErrorEvent, errev))
#define LogError(conn, err, logical, ctree, system, arg) \
	{ \
		if (LogErrorEnabled(conn, AllErrorEvents)) { \
			rtgLOGSPRINTF(arg); \
			rtgGLogError(conn, __MODULE__, __LINE__, __FUNCTION__, err, logical, ctree, system, LogBuffer); \
		} else { \
			ctcbSetError(err, logical, ctree, system, NULL, NULL); \
		} \
	}
#define LogErrorMsg(conn, err, logical, ctree, system, errmsg, arg) \
	{ \
		rtgLOGSPRINTF(arg); \
		if (LogErrorEnabled(conn, AllErrorEvents)) { \
			rtgGLogErrorMsg(conn, __MODULE__, __LINE__, __FUNCTION__, err, logical, ctree, system, errmsg, LogBuffer); \
		} else { \
			ctcbSetError(err, logical, ctree, system, errmsg, LogBuffer); \
		} \
	}
#define LogWarningEnabled(conn) \
		(conn && ctcbLogIsEnabled(conn->conf, ctcbLogGetInst(conn, NULL), NULL, NULL, WarningEvent, NoExtraEvent))
#define LogWarning(conn, arg) \
	{ \
		if (LogWarningEnabled(conn)) { \
			rtgLOGSPRINTF(arg); \
			rtgGLogWarning(conn, __MODULE__, __LINE__, __FUNCTION__, LogBuffer); \
		} \
	}
#define LogProfileEnabled(conn) \
		(conn && ctcbLogIsEnabled(conn->conf, ctcbLogGetInst(conn, NULL), NULL, NULL, ProfileEvent, NoExtraEvent))
#define LogProfile(conn, value, arg) \
	{ \
		if (LogProfileEnabled(conn)) { \
			rtgLOGSPRINTF(arg); \
			rtgGLogProfile(conn, __MODULE__, __LINE__, __FUNCTION__, value, LogBuffer); \
		} \
	}
#define LogDebugEnabled(conn, dbgev) \
		(conn && ctcbLogIsEnabled(conn->conf, ctcbLogGetInst(conn, NULL), NULL, NULL, DebugEvent, dbgev))
#define LogDebug(conn, dbgev, arg) \
	{ \
		if (LogDebugEnabled(conn, AllDebugEvents)) { \
			rtgLOGSPRINTF(arg); \
			rtgGLogDebug(conn, __MODULE__, __LINE__, __FUNCTION__, dbgev, LogBuffer); \
		} \
	}
#define LogDebugConfig(conn, dump, arg) \
	{ \
		if (LogDebugEnabled(conn, ConfigDebugEvent)) { \
			rtgLOGSPRINTF(arg); \
			if (conn) rtgCLogDebugConfig(conn->conf, __MODULE__, __LINE__, __FUNCTION__, dump, LogBuffer); \
		} \
	}

/* log macros by instance */
#define ILogEnabled(conn, inst) \
		(conn && ctcbLogIsEnabled(conn->conf, rtgLogGetInst(conn, inst), NULL, NULL, InfoEvent, NoExtraEvent))
#define ILog(conn, inst, arg) \
	{ \
		if (ILogEnabled(conn, inst)) { \
			rtgLOGSPRINTF(arg); \
			rtgILog(conn, inst, __MODULE__, __LINE__, __FUNCTION__, LogBuffer); \
		} \
	}
#define ILogErrorEnabled(conn, inst, errev) \
		(conn && ctcbLogIsEnabled(conn->conf, rtgLogGetInst(conn, inst), NULL, NULL, ErrorEvent, errev))
#define ILogError(conn, inst, logical, ctree, system, arg) \
	{ \
		if (ILogErrorEnabled(conn, inst, AllErrorEvents)) { \
			rtgLOGSPRINTF(arg); \
			rtgILogError(conn, inst, __MODULE__, __LINE__, __FUNCTION__, logical, ctree, system, LogBuffer); \
		} else { \
			if (conn) ctcbSetError(&conn->err, logical, ctree, system, NULL, NULL); \
		} \
	}
#define ILogWarningEnabled(conn, inst) \
		(conn && ctcbLogIsEnabled(conn->conf, rtgLogGetInst(conn, inst), NULL, NULL, WarningEvent, NoExtraEvent))
#define ILogWarning(conn, inst, arg) \
	{ \
		if (ILogWarningEnabled(conn, inst)) { \
			rtgLOGSPRINTF(arg); \
			rtgILogWarning(conn, inst, __MODULE__, __LINE__, __FUNCTION__, LogBuffer); \
		} \
	}
#define ILogDebugEnabled(conn, inst, dbgev) \
		(conn && ctcbLogIsEnabled(conn->conf, rtgLogGetInst(conn, inst), NULL, NULL, DebugEvent, dbgev))
#define ILogDebug(conn, inst, dbgev, arg) \
	{ \
		if (ILogDebugEnabled(conn, inst, AllDebugEvents)) { \
			rtgLOGSPRINTF(arg); \
			rtgILogDebug(conn, inst, __MODULE__, __LINE__, __FUNCTION__, dbgev, LogBuffer); \
		} \
	}

/* log macros by file */
#define FLogEnabled(conn, file) \
		(conn && ctcbLogIsEnabled(conn->conf, ctcbLogGetInst(conn, NULL), file, NULL, InfoEvent, NoExtraEvent))
#define FLog(conn, file, arg) \
	{ \
		if (FLogEnabled(conn, file)) { \
			rtgLOGSPRINTF(arg); \
			rtgFLog(conn, file, __MODULE__, __LINE__, __FUNCTION__, LogBuffer); \
		} \
	}
#define FLogErrorEnabled(conn, file, errev) \
		(conn && ctcbLogIsEnabled(conn->conf, ctcbLogGetInst(conn, NULL), file, NULL, ErrorEvent, errev))
#define FLogError(conn, file, logical, ctree, system, arg) \
	{ \
		if (FLogErrorEnabled(conn, file, AllErrorEvents)) { \
			rtgLOGSPRINTF(arg); \
			rtgFLogError(conn, file, __MODULE__, __LINE__, __FUNCTION__, logical, ctree, system, LogBuffer); \
		} else { \
			ctcbSetError(&conn->err, logical, ctree, system, NULL, NULL); \
		} \
	}
#if !defined MULTITRD && defined ctFeatRTG_API
#define FLogErrorX(conn, file, ct, logical, ctree, system, arg) \
	{ \
		if (FLogErrorEnabled(conn, file, AllErrorEvents)) { \
			rtgLOGSPRINTF(arg); \
			rtgFLogErrorX(conn, file, ct, __MODULE__, __LINE__, __FUNCTION__, logical, ctree, system, LogBuffer); \
		} else { \
			ctcbSetError(&conn->err, logical, ctree, system, NULL, NULL); \
		} \
	}
#else
#define FLogErrorX(conn, file, ct, logical, ctree, system, arg) \
	FLogError(conn, file, logical, ctree, system, arg)
#endif
#define FLogErrorByName(conn, name, logical, ctree, system, arg) \
	{ \
		pCONFFILE _conf = (conn) ? ctl_conffile(conn->conf, name) : NULL; \
		if (_conf && FLogErrorEnabled(conn, _conf, AllErrorEvents)) { \
			rtgLOGSPRINTF(arg); \
			rtgFLogError(conn, _conf, __MODULE__, __LINE__, __FUNCTION__, logical, ctree, system, LogBuffer); \
		} else { \
			ctcbSetError(&conn->err, logical, ctree, system, NULL, NULL); \
		} \
	}
#define FLogWarningEnabled(conn, file) \
		(conn && ctcbLogIsEnabled(conn->conf, ctcbLogGetInst(conn, NULL), file, NULL, WarningEvent, NoExtraEvent))
#define FLogWarning(conn, file, arg) \
	{ \
		if (FLogWarningEnabled(conn, file)) { \
			rtgLOGSPRINTF(arg); \
			rtgFLogWarning(conn, file, __MODULE__, __LINE__, __FUNCTION__, LogBuffer); \
		} \
	}
#if !defined MULTITRD && defined ctFeatRTG_API
#define FLogWarningX(conn, file, ct, arg) \
	{ \
		if (FLogWarningEnabled(conn, file)) { \
			rtgLOGSPRINTF(arg); \
			rtgFLogWarningX(conn, file, ct, __MODULE__, __LINE__, __FUNCTION__, LogBuffer); \
		} \
	}
#else
#define FLogWarningX(conn, file, ct, arg) \
	FLogWarning(conn, file, arg)
#endif
#define FLogWarningByName(conn, name, arg) \
	{ \
		pCONFFILE _conf = (conn) ? ctl_conffile(conn->conf, name) : NULL; \
		if (_conf && FLogWarningEnabled(conn, _conf)) { \
			rtgLOGSPRINTF(arg); \
			rtgFLogWarning(conn, _conf, __MODULE__, __LINE__, __FUNCTION__, LogBuffer); \
		} \
	}
#define FLogProfileEnabled(conn, file) \
		(conn && ctcbLogIsEnabled(conn->conf, ctcbLogGetInst(conn, NULL), file, NULL, ProfileEvent, NoExtraEvent))
#define FLogProfile(conn, file, value, arg) \
	{ \
		if (FLogProfileEnabled(conn, file)) { \
			rtgLOGSPRINTF(arg); \
			rtgFLogProfile(conn, file, __MODULE__, __LINE__, __FUNCTION__, value, LogBuffer); \
		} \
	}
#if !defined MULTITRD && defined ctFeatRTG_API
#define FLogProfileX(conn, file, ct, value, arg) \
	{ \
		if (FLogProfileEnabled(conn, file)) { \
			rtgLOGSPRINTF(arg); \
			rtgFLogProfileX(conn, file, ct, __MODULE__, __LINE__, __FUNCTION__, value, LogBuffer); \
		} \
	}
#else
#define FLogProfileX(conn, file, ct, value, arg) \
	FLogProfile(conn, file, value, arg)
#endif
#define FLogDebugEnabled(conn, file, dbgev) \
		(conn && ctcbLogIsEnabled(conn->conf, ctcbLogGetInst(conn, NULL), file, NULL, DebugEvent, dbgev))
#define FLogDebug(conn, file, dbgev, arg) \
	{ \
		if (FLogDebugEnabled(conn, file, AllDebugEvents)) { \
			rtgLOGSPRINTF(arg); \
			rtgFLogDebug(conn, file, __MODULE__, __LINE__, __FUNCTION__, dbgev, LogBuffer); \
		} \
	}
#if !defined MULTITRD && defined ctFeatRTG_API
#define FLogDebugX(conn, file, dbgev, ct, arg) \
	{ \
		if (FLogDebugEnabled(conn, file, AllDebugEvents)) { \
			rtgLOGSPRINTF(arg); \
			rtgFLogDebugX(conn, file, ct, __MODULE__, __LINE__, __FUNCTION__, dbgev, LogBuffer); \
		} \
	}
#else
#define FLogDebugX(conn, file, dbgev, ct, arg) \
	FLogDebug(conn, file, dbgev, arg)
#endif
#define FLogDebugByName(conn, name, dbgev, arg) \
	{ \
		pCONFFILE _conf = (conn) ? ctl_conffile(conn->conf, name) : NULL; \
		if (_conf && FLogDebugEnabled(conn, _conf, AllDebugEvents)) { \
			rtgLOGSPRINTF(arg); \
			rtgFLogDebug(conn, _conf, __MODULE__, __LINE__, __FUNCTION__, dbgev, LogBuffer); \
		} \
	}

/* server log macros */
#define SLogEnabled(in) \
		ctcbLogIsEnabled(NULL, NULL, NULL, (CT_INPUT*)in, InfoEvent, NoExtraEvent)
#define SLog(in, arg) \
	{ \
		if (SLogEnabled((CT_INPUT*)in)) { \
			rtgLOGSPRINTF(arg); \
			rtgLog((CT_INPUT*)in, __MODULE__, __LINE__, __FUNCTION__, LogBuffer); \
		} \
	}
#define SLogErrorEnabled(in, errev) \
		ctcbLogIsEnabled(NULL, NULL, NULL, (CT_INPUT*)in, ErrorEvent, errev)
#define SLogError(in, err, logical, ctree, system, arg) \
	{ \
		if (in && SLogErrorEnabled((CT_INPUT*)in, AllErrorEvents)) { \
			rtgLOGSPRINTF(arg); \
			rtgSLogError((CT_INPUT*)in, __MODULE__, __LINE__, __FUNCTION__, err, logical, ctree, system, LogBuffer); \
		} else { \
			ctcbSetError(err, logical, ctree, system, NULL, NULL); \
		} \
	}
#define SLogWarningEnabled(in) \
		ctcbLogIsEnabled(NULL, NULL, NULL, (CT_INPUT*)in, WarningEvent, NoExtraEvent)
#define SLogWarning(in, arg) \
	{ \
		if (SLogWarningEnabled((CT_INPUT*)in)) { \
			rtgLOGSPRINTF(arg); \
			rtgSLogWarning((CT_INPUT*)in, __MODULE__, __LINE__, __FUNCTION__, LogBuffer); \
		} \
	}
#define SLogDebugEnabled(in, dbgev) \
		ctcbLogIsEnabled(NULL, NULL, NULL, (CT_INPUT*)in, DebugEvent, dbgev)
#define SLogDebug(in, dbgev, arg) \
	{ \
		if (SLogDebugEnabled((CT_INPUT*)in, AllDebugEvents)) { \
			rtgLOGSPRINTF(arg); \
			rtgSLogDebug((CT_INPUT*)in, __MODULE__, __LINE__, __FUNCTION__, dbgev, LogBuffer); \
		} \
	}

#ifdef ctFeatRTG_HANDLE
#ifdef CLog
#undef CLog
#endif
#define CLog(conf, arg) LOG(conf, arg)
#ifdef CLogError
#undef CLogError
#endif
#define CLogError(conf, _err, logical, ctree, system, arg) LOGErrorX(conf, logical, ctree, system, _err, NULL, arg)
#ifdef CLogWarning
#undef CLogWarning
#endif
#define CLogWarning(conf, arg) LOGWarning(conf, arg)
#ifdef CLogDebugEnabled
#undef CLogDebugEnabled
#endif
#define CLogDebugEnabled(conf, dbgev) LOGDebugEnabled(conf, NULL, dbgev)
#ifdef CLogDebug
#undef CLogDebug
#endif
#define CLogDebug(conf, dbgev, arg) LOGDebug(conf, dbgev, NULL, arg)
#ifdef CLogDebugConfig
#undef CLogDebugConfig
#endif
#define CLogDebugConfig(conf, dump, arg) LOGDebug(conf, ConfigDebugEvent, dump, arg)
#ifdef CLogDebugExtfh
#undef CLogDebugExtfh
#endif
#define CLogDebugExtfh(conf, dump, arg) LOGDebug(conf, EXTFHDebugEvent, dump, arg)

#ifdef Log
#undef Log
#endif
#define Log(conn, arg) LOG(conn, arg)
#ifdef LogError
#undef LogError
#endif
#define LogError(conn, err, logical, ctree, system, arg) LOGError(conn, logical, ctree, system, arg)
#ifdef LogErrorMsg
#undef LogErrorMsg
#endif
#define LogErrorMsg(conn, _err, logical, ctree, system, errmsg, arg) LOGErrorX(conn, logical, ctree, system, _err, errmsg, arg)
#ifdef LogWarning
#undef LogWarning
#endif
#define LogWarning(conn, arg) LOGWarning(conn, arg)
#ifdef LogProfileEnabled
#undef LogProfileEnabled
#endif
#define LogProfileEnabled(conn) LOGProfileEnabled(conn)
#ifdef LogProfile
#undef LogProfile
#endif
#define LogProfile(conn, value, arg) LOGProfile(conn, value, arg)
#ifdef LogDebugEnabled
#undef LogDebugEnabled
#endif
#define LogDebugEnabled(conn, dbgev) LOGDebugEnabled(conn, NULL, dbgev)
#ifdef LogDebug
#undef LogDebug
#endif
#define LogDebug(conn, dbgev, arg) LOGDebug(conn, dbgev, NULL, arg)
#ifdef LogDebugConfig
#undef LogDebugConfig
#endif
#define LogDebugConfig(conn, dump, arg) LOGDebug(conn, ConfigDebugEvent, dump, arg)

#ifdef ILog
#undef ILog
#endif
#define ILog(conn, inst, arg) LOG(inst, arg)
#ifdef ILogError
#undef ILogError
#endif
#define ILogError(conn, inst, logical, ctree, system, arg) LOGError(inst, logical, ctree, system, arg)
#ifdef ILogWarning
#undef ILogWarning
#endif
#define ILogWarning(conn, inst, arg) LOGWarning(inst, arg)
#ifdef ILogDebug
#undef ILogDebug
#endif
#define ILogDebug(conn, inst, dbgev, arg) LOGDebug(inst, dbgev, NULL, arg)

#ifdef FLogErrorByName
#undef FLogErrorByName
#endif
#define FLogErrorByName(conn, name, logical, ctree, system, arg) LOGErrorByName(conn, name, logical, ctree, system, arg)
#ifdef FLogErrorX
#undef FLogErrorX
#endif
#define FLogErrorX(conn, file, ct, logical, ctree, system, arg) LOGError(ct, logical, ctree, system, arg)
#ifdef FLogWarningByName
#undef FLogWarningByName
#endif
#define FLogWarningByName(conn, name, arg) LOGWarningByName(conn, name, arg)
#ifdef FLogWarningX
#undef FLogWarningX
#endif
#define FLogWarningX(conn, file, ct, arg) LOGWarning(ct, arg)
#ifdef FLogProfileEnabled
#undef FLogProfileEnabled
#endif
#define FLogProfileEnabled(conn, file) LOGProfileEnabled(conn)
#ifdef FLogProfileX
#undef FLogProfileX
#endif
#define FLogProfileX(conn, file, ct, value, arg) LOGProfile(ct, value, arg)
#ifdef FLogDebugByName
#undef FLogDebugByName
#endif
#define FLogDebugByName(conn, name, dbgev, arg) LOGDebugByName(conn, name, dbgev, NULL, arg)
#ifdef FLogDebugX
#undef FLogDebugX
#endif
#define FLogDebugX(conn, file, dbgev, ct, arg) LOGDebug(ct, dbgev, NULL, arg)

#ifdef SLog
#undef SLog
#endif
#define SLog(in, err, logical, ctree, system, arg) LOG(in, logical, ctree, system, arg)
#ifdef SLogError
#undef SLogError
#endif
#define SLogError(in, _err, logical, ctree, system, arg) LOGErrorX(in, logical, ctree, system, _err, NULL, arg)
#ifdef SLogWarning
#undef SLogWarning
#endif
#define SLogWarning(in, arg) LOGWarning(in, arg)
#ifdef SLogDebugEnabled
#undef SLogDebugEnabled
#endif
#define SLogDebugEnabled(in, dbgev) LOGDebugEnabled(in, NULL, dbgev)
#ifdef SLogDebug
#undef SLogDebug
#endif
#define SLogDebug(in, dbgev, arg) LOGDebug(in, dbgev, NULL, arg)
#endif /* ctFeatRTG_HANDLE */

/* profile macros */
#define ProfileInit \
	LONG8 telap, tstart, tstop, hrtimbas; ctZERO64(telap); ctZERO64(tstart); ctZERO64(tstop); ctZERO64(hrtimbas);

#define ProfileStart \
	ctrt_hrtimer(tstart);

#define ProfileStop \
	ctrt_hrtimer(tstop); \
	ctCOPY64(telap, tstop); \
	ctSUBTO64(telap, tstart);

#define ProfileCalcTime \
	ctrt_hrtimbas(hrtimbas); \
	ctCOPY64(tstart, telap); \
	ctDIVBY64(tstart, hrtimbas); \
	ctCOPY64(tstop, telap); \
	ctMODBY64(tstop, hrtimbas); \
	ctMULTBY64(tstart, 1000000); \
	ctCOPY64(telap, tstart); \
	ctMULTBY64(tstop, 1000000); \
	ctADDTO64(telap, tstop); \
	ctDIVBY64(telap, hrtimbas);

#endif /* ~_CTCBLOG_H_ */

/* end of ctcblog.h */
