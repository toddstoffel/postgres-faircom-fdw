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

#include "ctreep.h"
#include "ctreplagent.h"

#ifdef ctPortWIN32
#define rxEXPORT __declspec(dllexport)
#elif defined(__GNUC__) && __GNUC__ >= 3
#define rxEXPORT __attribute__ ((visibility ("default")))
#else
#define rxEXPORT
#endif

/*
   To add a new event callback:

   1) Add a new event identifier using the current value of RXF_TOTAL to the
      RXEVT enumerated data type list. The new event identifier can be added
      in any position in EXEVT, but do not renumber the existing event
      identifiers, because code in an external library might expect these
      values to be consistent between releases.
   2) Increment RXF_TOTAL.
   3) Add the new callback function name to the end of the rxLibFuncNames array.
*/

/* External library events: */
typedef enum _rxevt {
	/* Replication agent state change events: */
	RXF_OnStartAgent		= 0,
	RXF_OnSourceConnected		= 1,
	RXF_OnTargetConnected		= 2,
	RXF_OnSourceLostConnection	= 3,
	RXF_OnTargetLostConnection	= 4,
	RXF_OnSourceDisconnected	= 5,
	RXF_OnTargetDisconnected	= 6,
	RXF_OnTerminateAgent		= 7,
	RXF_OnPauseAgent		= 8,
	RXF_OnResumeAgent		= 9,
	RXF_OnStopAgent			= 10,
	RXF_OnSyncShutdown		= 44,
	RXF_OnRestorePoint		= 45,
	RXF_OnAgentStopped		= 47,

	/* File events: */
	RXF_OnOpenFile			= 11,
	RXF_AfterOpenFile		= 12,
	RXF_OnCloseFile			= 13,
	RXF_OnAlterSchema		= 14,
	RXF_OnResyncFile		= 25,
	RXF_OnPTADMIN			= 26,
	RXF_OnTruncateFile		= 27,
	RXF_OnCREIFIL			= 28,
	RXF_OnSETDEFBLK			= 29,
	RXF_OnCreateIndex		= 30,
	RXF_OnDeleteFile		= 34,
	RXF_OnDropIndex			= 35,
	RXF_OnRENIFIL			= 36,
	RXF_OnAddIdField		= 37,
	RXF_OnDelIdField		= 38,
	RXF_OnResetIdField		= 39,
	RXF_OnXtdatrIdField		= 40,
	RXF_OnCreateDataFile		= 41,
	RXF_OnDeleteDataFile		= 42,
	RXF_OnChangeSerialNumber	= 43,
	RXF_OnCopyFile			= 46,
	RXF_OnPUTIFIL			= 48,
	RXF_OnPUTHDR			= 49,
	RXF_AfterCREIFIL		= 50,
	RXF_OnAddChangeIdField		= 51,
	RXF_OnDelChangeIdField		= 52,
	RXF_OnSetNullKeyValue		= 53,

	/* Transaction events: */
	RXF_OnStartTransaction		= 15,
	RXF_OnCommitTransaction		= 16,
	RXF_OnAbortTransaction		= 17,
	RXF_OnUserTransaction		= 18,
	RXF_OnCheckpoint		= 19,

	/* Data events: */
	RXF_OnAddRecord			= 20,
	RXF_OnUpdateRecord		= 21,
	RXF_OnDeleteRecord		= 22,
	RXF_OnDeferredKey		= 23,
	RXF_OnAddResource		= 31,
	RXF_OnUpdateResource		= 32,
	RXF_OnDeleteResource		= 33,

	/* Exception events: */
	RXF_OnException			= 24
} RXEVT;

/* Name of the functions exported by the external library. Those functions that
are prefixed with ? are optional. */
cpTEXT rxLibFuncNames[] = {
	"rxOnStartAgent",
	"?rxOnSourceConnected",
	"?rxOnTargetConnected",
	"?rxOnSourceLostConnection",
	"?rxOnTargetLostConnection",
	"?rxOnSourceDisconnected",
	"?rxOnTargetDisconnected",
	"?rxOnTerminateAgent",
	"?rxOnPauseAgent",
	"?rxOnResumeAgent",
	"?rxOnStopAgent",
	"?rxOnOpenFile",
	"?rxAfterOpenFile",
	"?rxOnCloseFile",
	"?rxOnAlterSchema",
	"?rxOnStartTransaction",
	"?rxOnCommitTransaction",
	"?rxOnAbortTransaction",
	"?rxOnUserTransaction",
	"?rxOnCheckpoint",
	"?rxOnAddRecord",
	"?rxOnUpdateRecord",
	"?rxOnDeleteRecord",
	"?rxOnDeferredKey",
	"?rxOnException",
	"?rxOnResyncFile",
	"?rxOnPTADMIN",
	"?rxOnTruncateFile",
	"?rxOnCREIFIL",
	"?rxOnSETDEFBLK",
	"?rxOnCreateIndex",
	"?rxOnAddResource",
	"?rxOnUpdateResource",
	"?rxOnDeleteResource",
	"?rxOnDeleteFile",
	"?rxOnDropIndex",
	"?rxOnRENIFIL",
	"?rxOnAddIdField",
	"?rxOnDelIdField",
	"?rxOnResetIdField",
	"?rxOnXtdatrIdField",
	"?rxOnCreateDataFile",
	"?rxOnDeleteDataFile",
	"?rxOnChangeSerialNumber",
	"?rxOnSyncShutdown",
	"?rxOnRestorePoint",
	"?rxOnCopyFile",
	"?rxOnAgentStopped",
	"?rxOnPUTIFIL",
	"?rxOnPUTHDR",
	"?rxAfterCREIFIL",
	"?rxOnAddChangeIdField",
	"?rxOnDelChangeIdField",
	"?rxOnSetNullKeyValue"

	/* Add new function names here */
};

typedef TEXT checkSizeOfRxLibFuncNamesArray[sizeof(rxLibFuncNames) / sizeof(*rxLibFuncNames) == RXF_TOTAL];

	/* Agent state change events: */
extern rxEXPORT VOID rxOnStartAgent(prxEVENT prxagentev);
extern rxEXPORT VOID rxOnSourceConnected(prxEVENT prxagentev);
extern rxEXPORT VOID rxOnTargetConnected(prxEVENT prxagentev);
extern rxEXPORT VOID rxOnSourceLostConnection(prxEVENT prxagentev);
extern rxEXPORT VOID rxOnTargetLostConnection(prxEVENT prxagentev);
extern rxEXPORT VOID rxOnSourceDisconnected(prxEVENT prxagentev);
extern rxEXPORT VOID rxOnTargetDisconnected(prxEVENT prxagentev);
extern rxEXPORT VOID rxOnTerminateAgent(prxEVENT prxagentev);
extern rxEXPORT VOID rxOnPauseAgent(prxEVENT prxagentev);
extern rxEXPORT VOID rxOnResumeAgent(prxEVENT prxagentev);
extern rxEXPORT VOID rxOnStopAgent(prxEVENT prxagentev);
extern rxEXPORT VOID rxOnSyncShutdown(prxEVENT prxagentev);
extern rxEXPORT VOID rxOnRestorePoint(prxEVENT prxagentev);
extern rxEXPORT VOID rxOnAgentStopped(prxEVENT prxagentev);

	/* File events: */
extern rxEXPORT VOID rxOnOpenFile(prxEVENT prxagentev);
extern rxEXPORT VOID rxAfterOpenFile(prxEVENT prxagentev);
extern rxEXPORT VOID rxOnCloseFile(prxEVENT prxagentev);
extern rxEXPORT VOID rxOnAlterSchema(prxEVENT prxagentev);
extern rxEXPORT VOID rxOnResyncFile(prxEVENT prxagentev);
extern rxEXPORT VOID rxOnPTADMIN(prxEVENT prxagentev);
extern rxEXPORT VOID rxOnTruncateFile(prxEVENT prxagentev);
extern rxEXPORT VOID rxOnCREIFIL(prxEVENT prxagentev);
extern rxEXPORT VOID rxOnSETDEFBLK(prxEVENT prxagentev);
extern rxEXPORT VOID rxOnCreateIndex(prxEVENT prxagentev);
extern rxEXPORT VOID rxOnDeleteFile(prxEVENT prxagentev);
extern rxEXPORT VOID rxOnDropIndex(prxEVENT prxagentev);
extern rxEXPORT VOID rxOnRENIFIL(prxEVENT prxagentev);
extern rxEXPORT VOID rxOnAddIdField(prxEVENT prxagentev);
extern rxEXPORT VOID rxOnDelIdField(prxEVENT prxagentev);
extern rxEXPORT VOID rxOnResetIdField(prxEVENT prxagentev);
extern rxEXPORT VOID rxOnXtdatrIdField(prxEVENT prxagentev);
extern rxEXPORT VOID rxOnCreateDataFile(prxEVENT prxagentev);
extern rxEXPORT VOID rxOnDeleteDataFile(prxEVENT prxagentev);
extern rxEXPORT VOID rxOnChangeSerialNumber(prxEVENT prxagentev);
extern rxEXPORT VOID OnCopyFile(prxEVENT prxagentev);
extern rxEXPORT VOID rxOnPUTIFIL(prxEVENT prxagentev);
extern rxEXPORT VOID rxOnPUTHDR(prxEVENT prxagentev);
extern rxEXPORT VOID rxAfterCREIFIL(prxEVENT prxagentev);
extern rxEXPORT VOID rxOnAddChangeIdField(prxEVENT prxagentev);
extern rxEXPORT VOID rxOnDelChangeIdField(prxEVENT prxagentev);

	/* Transaction events: */
extern rxEXPORT VOID rxOnStartTransaction(prxEVENT prxagentev);
extern rxEXPORT VOID rxOnCommitTransaction(prxEVENT prxagentev);
extern rxEXPORT VOID rxOnAbortTransaction(prxEVENT prxagentev);
extern rxEXPORT VOID rxOnUserTransaction(prxEVENT prxagentev);
extern rxEXPORT VOID rxOnCheckpoint(prxEVENT prxagentev);

	/* Data events: */
extern rxEXPORT VOID rxOnAddRecord(prxEVENT prxagentev);
extern rxEXPORT VOID rxOnUpdateRecord(prxEVENT prxagentev);
extern rxEXPORT VOID rxOnDeleteRecord(prxEVENT prxagentev);
extern rxEXPORT VOID rxOnDeferredKey(prxEVENT prxagentev);
extern rxEXPORT VOID rxOnAddResource(prxEVENT prxagentev);
extern rxEXPORT VOID rxOnUpdateResource(prxEVENT prxagentev);
extern rxEXPORT VOID rxOnDeleteResource(prxEVENT prxagentev);

	/* Exception events: */
extern rxEXPORT VOID rxOnException(prxEVENT prxagentev);

/* end of ctrepluser.h */
