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

/*^***********************************/
#ifdef ctLOCBHint /* function definitions for ctlocb.h */
/*
** change API names to a local variety: e.g., ADDKEY -> fp_ADDKEY
**
** ctFPprefix default definition in ctopt2.h: "fp_"
*/
#define ctSEPLprefix ctFPprefix
/* uses common renaming below */

#endif /* end function definitions for ctlocb.h */
/*~***********************************/

/*^***********************************/
#ifdef ctLOCFHint /* function definitions for ctlocf.h */

/* uses common renaming below */
#define ctSEPLprefix loc

#endif /* end function definitions for ctlocf.h */
/*~***********************************/

/*^***********************************/
#ifdef ctLOCUHint /* function definitions for ctlocu.h */
/*
** undo locale variety rename
*/
#undef ctReplReadLogData
#undef ctReplCleanupTranStateKeys
#undef ctClusterConfig
#undef ctTruncateFile
#undef ctWorkerThrdAttach
#undef ctReplGetPosForTransactionId
#undef ctPlugin
#undef DELFILX
#undef DELIFILX
#undef DELRFILX
#undef ctAcquireSessionId
#undef ctReleaseSessionId
#undef ctSetConfigurationFile
#undef ctgetmem
#undef ctputmem
#undef mballcx
#undef mbfreex
#undef FCSInit
#undef FCSEncrypt
#undef FCSDecrypt
#undef FCSFinal
#undef RTG
#undef ctSetNullKeyValue
#undef ctGetNullKeyValue
#undef ctGetRecordConverters
#undef addChangeIDfield
#undef getChangeIDfieldNumber
#undef delChangeIDfield
#undef getChangeIDheaderValue
#undef ctSetConnectionAttribute
#undef ctFILBLKX
#undef PRMIIDX82
#undef ctNextSequenceUniqueName
#undef ctCreateUUID
#undef ctSetDatabaseEngineUUID
#undef ctGetDatabaseEngineUUID
/* ctSIBLING_SEPLNG: Add entry for new function here */

#endif /* end function definitions for ctlocu.h */
/*~***********************************/

/*^***********************************/
#ifdef ctSRCLHint
/* function definitions for ctsrcl.h */
/* uses common renaming below */
#define ctSEPLprefix mtc_

#endif /* end function definitions for ctsrcl.h */

#if !defined(ctLOCUHint)
/* Common function renaming occurs here */
#ifndef ctSEPLprefix
#error  ctSEPLprefix is not defined
#endif

#define ctReplReadLogData		xppf(ctSEPLprefix,ctReplReadLogData)
#define ctReplCleanupTranStateKeys	xppf(ctSEPLprefix,ctReplCleanupTranStateKeys)
#define ctClusterConfig                 xppf(ctSEPLprefix,ctClusterConfig)
#define ctTruncateFile                  xppf(ctSEPLprefix,ctTruncateFile)
#define ctWorkerThrdAttach		xppf(ctSEPLprefix,ctWorkerThrdAttach)
#define ctReplGetPosForTransactionId	xppf(ctSEPLprefix,ctReplGetPosForTransactionId)
#define ctPlugin			xppf(ctSEPLprefix,ctPlugin)
#define DELFILX				xppf(ctSEPLprefix,DELFILX)
#define DELIFILX			xppf(ctSEPLprefix,DELIFILX)
#define DELRFILX			xppf(ctSEPLprefix,DELRFILX)
#define ctAcquireSessionId		xppf(ctSEPLprefix,ctAcquireSessionId)
#define ctReleaseSessionId		xppf(ctSEPLprefix,ctReleaseSessionId)
#define ctSetConfigurationFile		xppf(ctSEPLprefix,ctSetConfigurationFile)
#define mballcx				xppf(ctSEPLprefix,mballcx)
#define mbfreex				xppf(ctSEPLprefix,mbfreex)
#define RTG				xppf(ctSEPLprefix,RTG)
#define ctSetNullKeyValue		xppf(ctSEPLprefix,ctSetNullKeyValue)
#define ctGetNullKeyValue		xppf(ctSEPLprefix,ctGetNullKeyValue)
#define ctGetRecordConverters		xppf(ctSEPLprefix,ctGetRecordConverters)
#define addChangeIDfield		xppf(ctSEPLprefix,addChangeIDfield)
#define getChangeIDfieldNumber		xppf(ctSEPLprefix,getChangeIDfieldNumber)
#define delChangeIDfield		xppf(ctSEPLprefix,delChangeIDfield)
#define getChangeIDheaderValue		xppf(ctSEPLprefix,getChangeIDheaderValue)
#define ctSetConnectionAttribute	xppf(ctSEPLprefix,ctSetConnectionAttribute)
#define ctFILBLKX			xppf(ctSEPLprefix,ctFILBLKX)
#define PRMIIDX82 			xppf(ctSEPLprefix,PRMIIDX82)
#define ctNextSequenceUniqueName	xppf(ctSEPLprefix,ctNextSequenceUniqueName)
#define ctCreateUUID			xppf(ctSEPLprefix,ctCreateUUID)
#define ctSetDatabaseEngineUUID		xppf(ctSEPLprefix,ctSetDatabaseEngineUUID)
#define ctGetDatabaseEngineUUID		xppf(ctSEPLprefix,ctGetDatabaseEngineUUID)
#define dyndmpsetopt			xppf(ctSEPLprefix,dyndmpsetopt)
#define dyndmp				xppf(ctSEPLprefix,dyndmp)

#ifdef ctSRCLHint
/* should these be common also? */
#define ctSetClusterOption              xppf(ctSEPLprefix,ctSetClusterOption)
#define ctReducePath			xppf(ctSEPLprefix,ctReducePath)
#define ctRemoveRelativePathReferences	xppf(ctSEPLprefix,ctRemoveRelativePathReferences)
#define makCREIFIL			xppf(ctSEPLprefix,makCREIFIL)
#define freCREIFIL			xppf(ctSEPLprefix,freCREIFIL)
#define makPUTIFIL			xppf(ctSEPLprefix,makPUTIFIL)
#define frePUTIFIL			xppf(ctSEPLprefix,frePUTIFIL)
#define makRENIFIL			xppf(ctSEPLprefix,makRENIFIL)
#define freRENIFIL			xppf(ctSEPLprefix,freRENIFIL)
#define makDELIFIL			xppf(ctSEPLprefix,makDELIFIL)
#define freDELIFIL			xppf(ctSEPLprefix,freDELIFIL)
#define makDROPIDX			xppf(ctSEPLprefix,makDROPIDX)
#define freDROPIDX			xppf(ctSEPLprefix,freDROPIDX)
#define makRESADD			xppf(ctSEPLprefix,makRESADD)
#define freRESADD			xppf(ctSEPLprefix,freRESADD)
#define makRESUPD			xppf(ctSEPLprefix,makRESUPD)
#define freRESUPD			xppf(ctSEPLprefix,freRESUPD)
#define makRESDEL			xppf(ctSEPLprefix,makRESDEL)
#define freRESDEL			xppf(ctSEPLprefix,freRESDEL)
#define makADDIDFIELD			xppf(ctSEPLprefix,makADDIDFIELD)
#define freADDIDFIELD			xppf(ctSEPLprefix,freADDIDFIELD)
#define makDELIDFIELD			xppf(ctSEPLprefix,makDELIDFIELD)
#define freDELIDFIELD			xppf(ctSEPLprefix,freDELIDFIELD)
#define makRESETIDFIELD			xppf(ctSEPLprefix,makRESETIDFIELD)
#define freRESETIDFIELD			xppf(ctSEPLprefix,freRESETIDFIELD)
#define makXTDATRIDFIELD		xppf(ctSEPLprefix,makXTDATRIDFIELD)
#define freXTDATRIDFIELD		xppf(ctSEPLprefix,freXTDATRIDFIELD)
#define makADDCHANGEIDFIELD		xppf(ctSEPLprefix,makADDCHANGEIDFIELD)
#define freADDCHANGEIDFIELD		xppf(ctSEPLprefix,freADDCHANGEIDFIELD)
#define makDELCHANGEIDFIELD		xppf(ctSEPLprefix,makDELCHANGEIDFIELD)
#define freDELCHANGEIDFIELD		xppf(ctSEPLprefix,freDELCHANGEIDFIELD)
#define makSETDEFBLK			xppf(ctSEPLprefix,makSETDEFBLK)
#define freSETDEFBLK			xppf(ctSEPLprefix,freSETDEFBLK)
#define makCreateIsamIndex		xppf(ctSEPLprefix,makCreateIsamIndex)
#define freCreateIsamIndex		xppf(ctSEPLprefix,freCreateIsamIndex)
#define makCREDATX8			xppf(ctSEPLprefix,makCREDATX8)
#define freCREDATX8			xppf(ctSEPLprefix,freCREDATX8)
#define makDELFIL			xppf(ctSEPLprefix,makDELFIL)
#define freDELFIL			xppf(ctSEPLprefix,freDELFIL)
#define makSERIALNUM			xppf(ctSEPLprefix,makSERIALNUM)
#define freSERIALNUM			xppf(ctSEPLprefix,freSERIALNUM)
#define makPTADMIN			xppf(ctSEPLprefix,makPTADMIN)
#define makSETNULLKEYVALUE		xppf(ctSEPLprefix,makSETNULLKEYVALUE)
#define freSETNULLKEYVALUE		xppf(ctSEPLprefix,freSETNULLKEYVALUE)
#define mballc				xppf(ctSEPLprefix,mballc)
#define mbfree				xppf(ctSEPLprefix,mbfree)
#define revobj				xppf(ctSEPLprefix,revobj)
#define revbyt				xppf(ctSEPLprefix,revbyt)
#define revwrd				xppf(ctSEPLprefix,revwrd)
#define revlng				xppf(ctSEPLprefix,revlng)
#define ictcnvfn			xppf(ctSEPLprefix,ictcnvfn)
#define ctdll_load			xppf(ctSEPLprefix,ctdll_load)
#define ctdll_unload			xppf(ctSEPLprefix,ctdll_unload)
#define ctrt_asctime_r			xppf(ctSEPLprefix,ctrt_asctime_r)
#define ctrt_localtime_r		xppf(ctSEPLprefix,ctrt_localtime_r)
#define ctrt_gmtime_r			xppf(ctSEPLprefix,ctrt_gmtime_r)
#define ctrt_timegm			xppf(ctSEPLprefix,ctrt_timegm)
#define ctrt_mktime         xppf(ctSEPLprefix,ctrt_mktime)
#define ctOWNER				xppf(ctSEPLprefix,ctOWNER)
#define makcpyfil			xppf(ctSEPLprefix,makcpyfil)
#define ctcdelm				xppf(ctSEPLprefix,ctcdelm)
#define mak_ctTruncateFile		xppf(ctSEPLprefix,mak_ctTruncateFile)
#define ctiflnam			xppf(ctSEPLprefix,ctiflnam)
#define dyndmpsetopt			xppf(ctSEPLprefix,dyndmpsetopt)
#define uredirect			xppf(ctSEPLprefix,uredirect)
#define uredirectadd			xppf(ctSEPLprefix,uredirectadd)
#define ictrt_filmtc			xppf(ctSEPLprefix,ictrt_filmtc)
#define ctrt_filmtc			xppf(ctSEPLprefix,ctrt_filmtc)
#define ctdate_r			xppf(ctSEPLprefix,ctdate_r)
#define cpylod				xppf(ctSEPLprefix,cpylod)
#define cpynbuf				xppf(ctSEPLprefix,cpynbuf)
#define ctReplSetTranStateKeyValue	xppf(ctSEPLprefix,ctReplSetTranStateKeyValue)
#define ctReplGetTranStateKeyValue	xppf(ctSEPLprefix,ctReplGetTranStateKeyValue)
#define ctCommEnc			xppf(ctSEPLprefix,ctCommEnc)
#define ctCommDec			xppf(ctSEPLprefix,ctCommDec)
#define ctPrintProductVersion		xppf(ctSEPLprefix,ctPrintProductVersion)
#define ctGetUserErrorCode		xppf(ctSEPLprefix,ctGetUserErrorCode)
#define ctGetIsamErrorCode		xppf(ctSEPLprefix,ctGetIsamErrorCode)
#define ctGetIsamFileNbr		xppf(ctSEPLprefix,ctGetIsamFileNbr)
#define ctGetSystemErrorCode		xppf(ctSEPLprefix,ctGetSystemErrorCode)
#define ctSetUserErrorCode		xppf(ctSEPLprefix,ctSetUserErrorCode)
#define ctSetIsamErrorCode		xppf(ctSEPLprefix,ctSetIsamErrorCode)
#define ctSetIsamFileNbr		xppf(ctSEPLprefix,ctSetIsamFileNbr)
#define ctSetSystemErrorCode		xppf(ctSEPLprefix,ctSetSystemErrorCode)
#define ctAlterSchema			xppf(ctSEPLprefix,ctAlterSchema)
#define ctCloseSequence			xppf(ctSEPLprefix,ctCloseSequence)
#define ctCreateSequence		xppf(ctSEPLprefix,ctCreateSequence)
#define ctDeleteSequence		xppf(ctSEPLprefix,ctDeleteSequence)
#define ctCreateAuthFile		xppf(ctSEPLprefix,ctCreateAuthFile)
#define ctConnStateAlloc		xppf(ctSEPLprefix,ctConnStateAlloc)
#define ctConnStateFree			xppf(ctSEPLprefix,ctConnStateFree)
#define ctConnStateGet			xppf(ctSEPLprefix,ctConnStateGet)
#define ctDeleteBackgroundLoadStatus	xppf(ctSEPLprefix,ctDeleteBackgroundLoadStatus)
#define ctfcpAddFileCopyOperation	xppf(ctSEPLprefix,ctfcpAddFileCopyOperation)
#define ctfcpAllocateHandle		xppf(ctSEPLprefix,ctfcpAllocateHandle)
#define ctfcpFreeHandle			xppf(ctSEPLprefix,ctfcpFreeHandle)
#define ctfcpCopyFile			xppf(ctSEPLprefix,ctfcpCopyFile)
#define ctfcpGetErrorBuffer		xppf(ctSEPLprefix,ctfcpGetErrorBuffer)
#define ctfcpRemoveFileCopyOperation	xppf(ctSEPLprefix,ctfcpRemoveFileCopyOperation)
#define ctfcpSetErrorBuffer		xppf(ctSEPLprefix,ctfcpSetErrorBuffer)
#define ctfcpSetCallback		xppf(ctSEPLprefix,ctfcpSetCallback)
#define ctfcpSetCopyFilenames		xppf(ctSEPLprefix,ctfcpSetCopyFilenames)
#define ctfcpSetCopyFilePassword	xppf(ctSEPLprefix,ctfcpSetCopyFilePassword)
#define ctfcpSetCopyOptions		xppf(ctSEPLprefix,ctfcpSetCopyOptions)
#define ctfcpSetServerParameters	xppf(ctSEPLprefix,ctfcpSetServerParameters)
#define ctGetBackgroundLoadStatus	xppf(ctSEPLprefix,ctGetBackgroundLoadStatus)
#define ctGetCurrentSequenceValue	xppf(ctSEPLprefix,ctGetCurrentSequenceValue)
#define ctSetCurrentSequenceValue	xppf(ctSEPLprefix,ctSetCurrentSequenceValue)
#define ctGetNextSequenceValue		xppf(ctSEPLprefix,ctGetNextSequenceValue)
#define ctGetFailOverState		xppf(ctSEPLprefix,ctGetFailOverState)
#define ctResetFailOverState		xppf(ctSEPLprefix,ctResetFailOverState)
#define ctGetITIMdefer			xppf(ctSEPLprefix,ctGetITIMdefer)
#define ctIncrementITIMfailed		xppf(ctSEPLprefix,ctIncrementITIMfailed)
#define ctIncrementITIMretry		xppf(ctSEPLprefix,ctIncrementITIMretry)
#define ctGetITIMlimit			xppf(ctSEPLprefix,ctGetITIMlimit)
#define ctGetMAX_DAT_KEY		xppf(ctSEPLprefix,ctGetMAX_DAT_KEY)
#define ctGetMAX_KEY_SEG		xppf(ctSEPLprefix,ctGetMAX_KEY_SEG)
#define ctGetRebuildMemory		xppf(ctSEPLprefix,ctGetRebuildMemory)
#define ctSetRebuildMemory		xppf(ctSEPLprefix,ctSetRebuildMemory)
#define ctGetRelativeKeyno		xppf(ctSEPLprefix,ctGetRelativeKeyno)
#define ctGetSequenceAttrs		xppf(ctSEPLprefix,ctGetSequenceAttrs)
#define ctSetSequenceAttrs		xppf(ctSEPLprefix,ctSetSequenceAttrs)
#define ctOpenSequence			xppf(ctSEPLprefix,ctOpenSequence)
#define ctReplAgentSetArgs		xppf(ctSEPLprefix,ctReplAgentSetArgs)
#define ctReplAgentSetCallback		xppf(ctSEPLprefix,ctReplAgentSetCallback)
#define ctReplAgentSetState		xppf(ctSEPLprefix,ctReplAgentSetState)
#define ctReplCheckExcludedFiles	xppf(ctSEPLprefix,ctReplCheckExcludedFiles)
#define ctReplCopyFileHandleFields	xppf(ctSEPLprefix,ctReplCopyFileHandleFields)
#define ctReplGetNextChangeDirect	xppf(ctSEPLprefix,ctReplGetNextChangeDirect)
#define ctReplInitFileHandle		xppf(ctSEPLprefix,ctReplInitFileHandle)
#define ctReplInitXtd			xppf(ctSEPLprefix,ctReplInitXtd)
#define ctSETSEG			xppf(ctSEPLprefix,ctSETSEG)
#define ctSetTransactionId		xppf(ctSEPLprefix,ctSetTransactionId)
#define CTSQLCTL			xppf(ctSEPLprefix,CTSQLCTL)
#define CTSRVCTL			xppf(ctSEPLprefix,CTSRVCTL)
#define ctSQLImportTable		xppf(ctSEPLprefix,ctSQLImportTable)
#define ctSYSPUT			xppf(ctSEPLprefix,ctSYSPUT)
#define cttdrAddTempDirOperation	xppf(ctSEPLprefix,cttdrAddTempDirOperation)
#define cttdrAllocateHandle		xppf(ctSEPLprefix,cttdrAllocateHandle)
#define cttdrFreeHandle			xppf(ctSEPLprefix,cttdrFreeHandle)
#define cttdrGetErrorBuffer		xppf(ctSEPLprefix,cttdrGetErrorBuffer)
#define cttdrRemoveTempDirOperation	xppf(ctSEPLprefix,cttdrRemoveTempDirOperation)
#define cttdrSetCallback		xppf(ctSEPLprefix,cttdrSetCallback)
#define cttdrSetErrorBuffer		xppf(ctSEPLprefix,cttdrSetErrorBuffer)
#define cttdrSetTempDirNames		xppf(ctSEPLprefix,cttdrSetTempDirNames)
#define cttdrSetTempDirOptions		xppf(ctSEPLprefix,cttdrSetTempDirOptions)
#define cttdrTempDir			xppf(ctSEPLprefix,cttdrTempDir)
#define ctThrdClr			xppf(ctSEPLprefix,ctThrdClr)
#define ctThrdNum			xppf(ctSEPLprefix,ctThrdNum)
#define ctThrdLIFOWriteDirect		xppf(ctSEPLprefix,ctThrdLIFOWriteDirect)
#define ctThrdPut			xppf(ctSEPLprefix,ctThrdPut)
#define ctThrdQueueOnClose		xppf(ctSEPLprefix,ctThrdQueueOnClose)
#define ctThrdQueueReadDirect		xppf(ctSEPLprefix,ctThrdQueueReadDirect)
#define ctThrdQueueReadWithSignal	xppf(ctSEPLprefix,ctThrdQueueReadWithSignal)
#define ctThrdQueueSignal		xppf(ctSEPLprefix,ctThrdQueueSignal)
#define ctThrdQueueWriteDirect		xppf(ctSEPLprefix,ctThrdQueueWriteDirect)
#define ctThrdSharedCriticalCancelReservation		xppf(ctSEPLprefix,ctThrdSharedCriticalCancelReservation)
#define ctThrdSharedCriticalEnter	xppf(ctSEPLprefix,ctThrdSharedCriticalEnter)
#define ctThrdSharedCriticalEnterExcl	xppf(ctSEPLprefix,ctThrdSharedCriticalEnterExcl)
#define ctThrdSharedCriticalExit	xppf(ctSEPLprefix,ctThrdSharedCriticalExit)
#define ctThrdSharedCriticalExitExcl	xppf(ctSEPLprefix,ctThrdSharedCriticalExitExcl)
#define ctThrdSharedCriticalInit	xppf(ctSEPLprefix,ctThrdSharedCriticalInit)
#define ctThrdSharedCriticalTerm	xppf(ctSEPLprefix,ctThrdSharedCriticalTerm)
#define ctVerifyFile			xppf(ctSEPLprefix,ctVerifyFile)
#define ctVERIFYidx			xppf(ctSEPLprefix,ctVERIFYidx)
#define ctGETDODAX			xppf(ctSEPLprefix,ctGETDODAX)
#define PUTDODAX			xppf(ctSEPLprefix,PUTDODAX)
#define ctSetOWNER			xppf(ctSEPLprefix,ctSetOWNER)
#define SPCLSAV			xppf(ctSEPLprefix,SPCLSAV)
#define CTUSERX			xppf(ctSEPLprefix,CTUSERX)
#define ctReplMasterControl	xppf(ctSEPLprefix,ctReplMasterControl)
#define ctSETCLIENTID	xppf(ctSEPLprefix,ctSETCLIENTID)
#define ctSetCommProtocolOption	xppf(ctSEPLprefix,ctSetCommProtocolOption)


#endif /* ctSRCLHint */


#if !defined(ctLOCFHint)
/* Don't rename these functions in loclib model  */
#define ctgetmem			xppf(ctSEPLprefix,ctgetmem)
#define ctputmem			xppf(ctSEPLprefix,ctputmem)
#define FCSInit				xppf(ctSEPLprefix,FCSInit)
#define FCSEncrypt			xppf(ctSEPLprefix,FCSEncrypt)
#define FCSDecrypt			xppf(ctSEPLprefix,FCSDecrypt)
#define FCSFinal			xppf(ctSEPLprefix,FCSFinal)
#endif

/* ctSIBLING_SEPLNG: Add entry for new function here */

#endif /* ~ctLOCUHint */
/*~***********************************/

/* end ctsepl.h */
