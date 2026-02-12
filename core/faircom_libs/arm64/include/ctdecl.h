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

#ifndef ctDECLH
#define ctDECLH

#include "ctifil.h"

#ifdef ctFeatSRVCLI
#include "ctsrcl.h"	/* Add mtc_ prefix to MT client API function names. */
#endif

#include "ctparm.h"

#ifdef ctFeatREPLICAT
#include "ctrepl.h"
#endif

#ifdef __cplusplus
extern "C" {
#endif

extern ctCONV  NINT  ctDECL ctMBprefix(pTEXT dp,NINT nn);
extern ctCONV  CTERR ctDECL ctSETEXITFNC(ctexitfnc_t exitfnc);

#ifdef ctThrds
#include "ctatrd.h"
extern ctCONV  NINT  ctDECL ctThrdInit(NINT ,LONG ,pctINIT );
extern ctCONV  NINT  ctDECL ctThrdTerm(VOID );
extern ctCONV  NINT  ctDECL ctThrdCreate(pctFUNC ,pVOID ,pVOID ,LONG );
extern ctCONV  VOID  ctDECL ctThrdExit(VOID );
extern ctCONV  NINT  ctDECL ctThrdAttach(VOID );
extern ctCONV  NINT  ctDECL ctThrdDetach(VOID );
extern ctCONV  NINT  ctDECL ctThrdSleep(LONG );
extern ctCONV  pVOID ctDECL ctThrdData(VOID );
extern ctCONV  NINT  ctDECL ctThrdDataSet(pVOID );
extern ctCONV  NINT  ctDECL ctThrdMutexInit(pctMUTEX );
extern ctCONV  NINT  ctDECL ctThrdMutexGet(pctMUTEX );
extern ctCONV  NINT  ctDECL ctThrdMutexTry(pctMUTEX );
extern ctCONV  NINT  ctDECL ctThrdMutexRel(pctMUTEX );
extern ctCONV  NINT  ctDECL ctThrdMutexCls(pctMUTEX );
extern ctCONV  NINT  ctDECL ctThrdBlockInit(pctBLOCK );
extern ctCONV  NINT  ctDECL ctThrdBlockGet(pctBLOCK ,LONG );
extern ctCONV  NINT  ctDECL ctThrdBlockWait(pctBLOCK ,LONG );
extern ctCONV  NINT  ctDECL ctThrdBlockRel(pctBLOCK );
extern ctCONV  NINT  ctDECL ctThrdBlockCls(pctBLOCK );
extern ctCONV  NINT  ctDECL ctThrdQueueOpen(VOID );
extern ctCONV  NINT  ctDECL ctThrdQueueClose(NINT );
extern ctCONV  NINT  ctDECL ctThrdQueueSignal(NINT );
extern ctCONV  NINT  ctDECL ctThrdQueueWrite(NINT ,pVOID ,NINT );
extern ctCONV  NINT  ctDECL ctThrdQueueWriteDirect(NINT ,pVOID ,NINT );
extern ctCONV  NINT  ctDECL ctThrdLIFOWriteDirect(NINT ,pVOID ,NINT );
extern ctCONV  NINT  ctDECL ctThrdLIFOWrite(NINT ,pVOID ,NINT );
extern ctCONV  NINT  ctDECL ctThrdQueueRead(NINT ,pVOID ,NINT ,LONG );
extern ctCONV  NINT  ctDECL ctThrdQueueReadWithSignal(NINT ,pVOID ,NINT ,LONG ,pNINT );
extern ctCONV  NINT  ctDECL ctThrdQueueReadDirect(NINT ,ppVOID ,pNINT ,LONG );
extern ctCONV  NINT  ctDECL ctThrdQueueMlen(NINT ,LONG );
extern ctCONV  LONG  ctDECL ctThrdQueueCount(NINT );
extern ctCONV  NINT  ctDECL ctThrdQueueOnClose(NINT, void(*func)(void *,NINT));
extern ctCONV  NINT  ctDECL ctThrdSemapInit(pctSEMAP ,NINT );
#ifdef ctDBGRWLCNT
extern ctCONV  NINT  ctDECL ctThrdSemapGet(pctSEMAP ,NINT );
#else
extern ctCONV  NINT  ctDECL ctThrdSemapGet(pctSEMAP );
#endif
extern ctCONV  NINT  ctDECL ctThrdSemapTry(pctSEMAP );
extern ctCONV  NINT  ctDECL ctThrdSemapRel(pctSEMAP );
extern ctCONV  NINT  ctDECL ctThrdSemapCls(pctSEMAP );
extern ctCONV  NINT  ctDECL ctThrdHandle(VOID );
extern ctCONV  pVOID ctDECL ctThrdGet(VOID);
extern ctCONV  pVOID ctDECL ctThrdClr(VOID);
extern ctCONV  VOID  ctDECL ctThrdPut(VOID);
extern ctCONV  NINT  ctDECL ctThrdSet(VOID);
extern ctCONV  NINT  ctDECL ctThrdNum(VOID);
extern         pVOID        ctThrdID(VOID);
extern ctCONV  NINT  ctDECL ctWorkerThrdAttach(VOID);
#endif	/* ctThrds 	*/

#ifdef ctBNDentryPointMutex
extern ctCONV  VOID  ctDECL ictGetThrdUniqueID(pULONG ptrduid);
extern ctCONV  NINT  ctDECL ictSetThrdSessInfo(NINT sOWNR, NINT setgbl);
extern ctCONV  NINT  ctDECL ictCheckThrdSessId(NINT sOWNR);
extern ctCONV  NINT  ctDECL ictRepairThrdState(NINT sOWNR);
extern ctCONV COUNT  ctDECL iUNRCTREE(NINT sOWNR, NINT allinst);
#endif

extern ctCONV  NINT  ctDECL ctNotify(NINT opcode,NINT objhandle,NINT qhandle,NINT contents,NINT controls);
extern ctCONV  NINT  ctDECL ctCallback(NINT opcode,NINT objhandle,ctCallbackPtr cbptr,NINT contents,NINT controls);
extern ctCONV  NINT  ctDECL ctSysQueueOpen(pTEXT qname,NINT qmode);
extern ctCONV  NINT  ctDECL ctSysQueueClose(NINT qhandle);
extern ctCONV  NINT  ctDECL ctSysQueueWrite(NINT qhandle,pVOID message,NINT msglen);
extern ctCONV  NINT  ctDECL ctSysQueueLIFOWrite(NINT qhandle,pVOID message,NINT msglen);
extern ctCONV  NINT  ctDECL ctSysQueueRead(NINT qhandle,pVOID buffer,NINT buflen,LONG timeout);
extern ctCONV  NINT  ctDECL ctSysQueueMlen(NINT qhandle,LONG timeout);
extern ctCONV  NINT  ctDECL ctSysQueueCount(NINT qhandle);

extern ctCONV  NINT  ctDECL ctLOKLST(FILNO datno,ctRECPT recbyt,NINT NbrUsers,pLOCKID userlist,pLONG pNbrLockers,pLONG pNbrWaiters);
extern ctCONV  NINT  ctDECL ctQUIET(pTEXT filespec,LONG timeoutSEC,LONG action);

extern ctCONV  NINT ctDECL AddRecordBucket(FILNO datno,pVOID recbuf,ppVOID pfndbuf,ppTEXT pdatorg);
extern ctCONV  NINT ctDECL GetRecordBucket(FILNO datno,pTEXT recbuf,ppVOID pfndbuf,ppTEXT pdatorg);
extern ctCONV  VOID ctDECL UnlockRecordBucket(pVOID datbuf,NINT flags);
extern ctCONV  NINT ctDECL ctLOKTIMOUT(FILNO datno,LONG mode,LONG timeoutSEC);
extern ctCONV  LONG ctDECL ctLOKDYN(LONG action);
extern ctCONV  LONG ctDECL ctTransferFile(pctXFRFIL pxfr);
extern ctCONV  NINT ctDECL ctMEMSTAT(LONG mode,pVOID datblk);
extern ctCONV  LONG ctDECL xtdatrIDfield(FILNO datno,LONG xtdatr);

extern ctCONV  NINT ctDECL ctfcpAllocateHandle(pFCPHND pfcphnd);
extern ctCONV  NINT ctDECL ctfcpFreeHandle(FCPHND fcphnd);
extern ctCONV  NINT ctDECL ctfcpAddFileCopyOperation(FCPHND fcphnd,pFCPOPR pfcpopr);
extern ctCONV  NINT ctDECL ctfcpRemoveFileCopyOperation(FCPOPR fcpopr);
extern ctCONV  NINT ctDECL ctfcpSetErrorBuffer(FCPHND fcphnd,pTEXT errbuf,LONG errbufsiz);
extern ctCONV pTEXT ctDECL ctfcpGetErrorBuffer(FCPHND fcphnd,pNINT perrcod);
extern ctCONV  NINT ctDECL ctfcpSetCallback(FCPHND fcphnd,pCPYFNC pcpyfnc,pVOID pusrdata);
extern ctCONV  NINT ctDECL ctfcpSetCopyOptions(FCPOPR fcpopr,LONG options);
extern ctCONV  NINT ctDECL ctfcpSetCopyFilenames(FCPOPR fcpopr,NINT totnames,pCTFNAM srcnames,pCTFNAM dstnames);
extern ctCONV  NINT ctDECL ctfcpSetCopyFilePassword(FCPOPR fcpopr,pTEXT fileword);
extern ctCONV  NINT ctDECL ctfcpSetServerParameters(FCPOPR fcpopr,pTEXT userid,pTEXT userword,pTEXT servername);
extern ctCONV  NINT ctDECL ctfcpCopyFile(FCPHND fcphnd);

extern ctCONV  NINT ctDECL cttdrAllocateHandle(pTDRHND ptdrhnd);
extern ctCONV  NINT ctDECL cttdrFreeHandle(TDRHND tdrhnd);
extern ctCONV  NINT ctDECL cttdrAddTempDirOperation(TDRHND tdrhnd, pTDROPR ptdropr);
extern ctCONV  NINT ctDECL cttdrRemoveTempDirOperation(TDROPR tdropr);
extern ctCONV  NINT ctDECL cttdrSetErrorBuffer(TDRHND tdrhnd, pTEXT errbuf, LONG errbufsiz);
extern ctCONV pTEXT ctDECL cttdrGetErrorBuffer(TDRHND tdrhnd, pNINT perrcod);
extern ctCONV  NINT ctDECL cttdrSetCallback(TDRHND tdrhnd, pTDRFNC ptdrfnc, pVOID pusrdata);
extern ctCONV  NINT ctDECL cttdrSetTempDirOptions(TDROPR tdropr, LONG options);
extern ctCONV  NINT ctDECL cttdrSetTempDirNames(TDROPR tdropr, NINT totnames, pCTTDRNAM dirnames);
extern ctCONV  NINT ctDECL cttdrTempDir(TDRHND tdrhnd);
extern ctCONV  NINT ctDECL ctReplLogFileResync(pTEXT resyncData);
extern ctCONV  NINT ctDECL ctSetCurrentSuperfileHost(FILNO hstno);
extern ctCONV  NINT ctDECL ctGetBackgroundLoadStatus(FILNO datno,pTEXT loadid,ppBGLDRES ppbgldres,pNINT preslen);
extern ctCONV  NINT ctDECL ctDeleteBackgroundLoadStatus(FILNO datno,pTEXT loadid);
extern ctCONV  NINT ctDECL ctGetAccountExpirationTime(pTEXT userid,pctEXPTM pactexp,pctEXPTM pupwexp);
extern ctCONV  NINT ctDECL ctGetConnectionInfo(NINT versn,pctCONINF pconninfo,pNINT pnconnections);
extern ctCONV  NINT ctDECL ctSyncReplControl(pTEXT operation);
extern ctCONV  NINT ctDECL addAutoSysTimeFields(FILNO datno, pA_STFIELDS defs);
extern ctCONV  NINT ctDECL RemoveAutoSysTimeFields(FILNO datno);
extern ctCONV VRLEN ctDECL WhichAutoSysTimeFields(FILNO datno, pVOID bufptr, VRLEN bufsiz);
extern ctCONV  NINT ctDECL UpdateAutoSysTimeFields(FILNO datno, pA_STFIELDS defs);
extern ctCONV  NINT ctDECL GetFileRegions(pFILE_REGION_REQUEST pFileRegionRequest);
extern ctCONV  NINT ctDECL ctGetMAX_DAT_KEY(void);
extern ctCONV  NINT ctDECL ctGetMAX_KEY_SEG(void);
#ifdef ctFeatRTG_API
extern ctCONV pRTGMSGCBFNC ctDECL ctGetrtgmsgcbfnc(void);
extern ctCONV  void ctDECL ctSetrtgmsgcbfnc(pRTGMSGCBFNC fnc);
extern ctCONV pVOID ctDECL ctGetrtgmsgcbusrctx(void);
extern ctCONV  void ctDECL ctSetrtgmsgcbusrctx(pVOID);
#endif
extern ctCONV ULONG ctDECL ctGetITIMlimit(void);
extern ctCONV ULONG ctDECL ctGetITIMdefer(void);
extern ctCONV  void ctDECL ctIncrementITIMretry(void);
extern ctCONV  void ctDECL ctIncrementITIMfailed(void);
#ifdef ctSTRCH
extern ctCONV  NINT ctDECL ctFileIsOnReplicateFileList(pTEXT filnam, REPL_FILE_FILTER_PURPOSE purpose);
#endif
extern ctCONV  void ctDECL ctSetAlterTableBit(TEXT yesno);
extern ctCONV  void ctDECL ctSetFilterNBit(TEXT yesno);
extern ctCONV  LONG ctDECL ctSetSuppressRUCBBit(TEXT yesno, LONG orgvalue);
extern ctCONV  void ctDECL ctSetIsExprVal(TEXT yesno);
extern ctCONV COUNT ctDECL ctGetCondIdxError(void);
extern ctCONV  void ctDECL ctSetCondIdxError(COUNT errcod);
extern ctCONV  NINT ctDECL ctIsTransactionActive(void);
extern ctCONV  NINT ctDECL ctGetTransactionMode(void);
extern ctCONV  NINT ctDECL ctAcquireSessionId(void);
extern ctCONV  void ctDECL ctReleaseSessionId(NINT sessid);
extern ctCONV  NINT ctDECL ctSetConfigurationFile(pTEXT cfgfil);
extern ctCONV pTEXT ctDECL ctGetCondIdxNames(FILNO datno);
extern ctCONV  void ctDECL ctSetNOIDGENBit(TEXT yesno);
extern ctCONV  NINT ctDECL ctclbkeval(pTEXT Clbk, LONG typflt, FILNO datno, pVOID Recptr, pConvMap Schema, VRLEN fixlen, VRLEN datlen);
extern ctCONV  NINT ctDECL ctGetServerShutdownFlag(void);
extern ctCONV  void ctDECL ctmakeOWNER(NINT sOWNR);
extern        pTEXT        ctgetmem(VRLEN iosize);
extern         VOID        ctputmem(pTEXT loc);
extern        pTEXT        mballcx(NINT numobj, UINT sizobj, NINT mode);
extern         VOID        mbfreex(pVOID objptr, NINT mode);
extern ctCONV  NINT ctDECL addChangeIDfield(FILNO datno, NINT fieldno);
extern ctCONV  NINT ctDECL getChangeIDfieldNumber(FILNO datno, pNINT pFieldNumber);
extern ctCONV  NINT ctDECL getChangeIDheaderValue(FILNO datno, ctCHGID *pChangeIdValue);
extern ctCONV  NINT ctDECL delChangeIDfield(FILNO datno);
extern ctCONV  NINT ctDECL ctSetConnectionAttribute(CTCONNECTION_ATTRIBUTE attr, cpVOID value);
extern ctCONV  NINT ctDECL ctCreateUUID(ctCREATE_UUID_OPTIONS* options, ctUUID* uuid, long* systemErrorCode);
extern ctCONV  NINT ctDECL ctSetDatabaseEngineUUID(ctSET_DATABASE_ENGINE_UUID_OPTIONS* options);
extern ctCONV  NINT ctDECL ctGetDatabaseEngineUUID(ctUUID* uuid);

/* ctSIBLING_SEPLNG: Add prototype for new function here */

extern ctCONV  NINT  ctDECL SetXtdFileOpenMode(LONG mode);
extern ctCONV  NINT  ctDECL addIDfield(FILNO datno,LONG8 startval,LONG delta,NINT fieldno);
extern ctCONV  CTERR ctDECL ADDKEY(FILNO keyno,pVOID target,LONG recbyt,COUNT typadd);
extern ctCONV  CTERR ctDECL ADDREC(FILNO datno,pVOID recptr);
extern ctCONV  CTERR ctDECL ADDRES(FILNO datno,pVOID resptr,VRLEN varlen);
extern ctCONV  CTERR ctDECL ADDVREC(FILNO datno,pVOID recptr,VRLEN varlen);
extern ctCONV  CTERR ctDECL ALCBAT(COUNT numbat);
extern ctCONV  CTERR ctDECL ALCRNG(FILNO keyno,NINT segcount,pVOID lrange,pVOID urange,pNINT operators);
extern ctCONV   LONG ctDECL ESTRNG(FILNO keyno,NINT segcount,pVOID lrange,pVOID urange,pNINT operators,pLONG segconsec);
extern ctCONV  CTERR ctDECL ALCSET(COUNT numset);
extern ctCONV  FILNO ctDECL AVLFILNUM(COUNT numfils);
extern ctCONV  CTERR ctDECL BATSET(FILNO filno,pVOID request,pVOID bufptr,VRLEN bufsiz,UCOUNT mode);
extern ctCONV  CTERR ctDECL BATSETX(FILNO filno,pVOID request,pVOID bufptr,VRLEN bufsiz,ULONG mode);
extern ctCONV  CTERR ctDECL BLKIREC(FILNO filno,NINT opcode,LONG timeoutsec,pTEXT blockcond,pVOID target,pVOID recptr,pVRLEN plen);
extern ctCONV  CTERR ctDECL CHGBAT(COUNT batnum);
extern ctCONV  CTERR ctDECL CHGHST(COUNT hstnum);
extern ctCONV  CTERR ctDECL CHGICON(COUNT contextid);
extern ctCONV  CTERR ctDECL CHGSET(COUNT setnum);
extern ctCONV  CTERR ctDECL CLIFIL(pIFIL ifilptr);
extern ctCONV  CTERR ctDECL CLNIDXX(pTEXT filnam,pTEXT fileword);
extern ctCONV  CTERR ctDECL CLISAM(VOID );
extern ctCONV  CTERR ctDECL CLCONN(VOID );
extern ctCONV  CTERR ctDECL CLRFIL(FILNO datno);
extern ctCONV  CTERR ctDECL CLSFIL(FILNO filno,COUNT filmod);
extern ctCONV  CTERR ctDECL CLSICON(COUNT contextid);
extern ctCONV  NINT  ctDECL ctCLSNAM(pTEXT filnam,pTEXT fileword);
extern ctCONV  CTERR ctDECL CMPIFIL(pIFIL ifilptr);
extern ctCONV  CTERR ctDECL CMPIFILX(pIFIL ifilptr,pTEXT dataextn,pTEXT indxextn,LONG permmask,pTEXT groupid,pTEXT fileword);
extern ctCONV  CTERR ctDECL CMPIFILX8(pIFIL ifilptr,pTEXT dataextn, pTEXT indxextn,LONG permmask,pTEXT groupid,pTEXT fileword, pXCREblk pxcreblk);
extern ctCONV  CTERR ctDECL CREDAT(FILNO datno,pTEXT filnam,UCOUNT datlen,UCOUNT xtdsiz,COUNT filmod);
extern ctCONV  CTERR ctDECL CREDATX(FILNO datno,pTEXT filnam,UCOUNT datlen,UCOUNT xtdsiz,COUNT filmod,LONG permmask,pTEXT groupid,pTEXT fileword);

#ifdef ctHUGEFILE
extern ctCONV  CTERR ctDECL CREDATX8(FILNO datno,pTEXT filnam,UCOUNT datlen,
	UCOUNT xtdsiz,COUNT filmod,LONG permmask,pTEXT groupid,pTEXT fileword,
	pXCREblk pxcreblk);
extern ctCONV  CTERR ctDECL CREIDXX8(FILNO keyno,pTEXT filnam,COUNT keylen,
	COUNT keytyp,COUNT keydup,COUNT nomemb,UCOUNT xtdsiz,COUNT filmod,
	LONG permmask,pTEXT groupid,pTEXT fileword,pXCREblk pxcreblk);
extern ctCONV  CTERR ctDECL CREIFILX8(pIFIL ifilptr,pTEXT dataextn,
	pTEXT indxextn,LONG permmask,pTEXT groupid,pTEXT fileword,
	pXCREblk pxcreblk);
extern ctCONV  CTERR ctDECL RBLIFILX8(pIFIL ifilptr,pTEXT dataextn,
	pTEXT indxextn,LONG permmask,pTEXT groupid,pTEXT fileword,
	pXCREblk pxcreblk);
extern ctCONV  CTERR ctDECL PRMIIDX8(pIFIL ifilptr,
	pXCREblk pxcreblk);
extern ctCONV  CTERR ctDECL PRMIIDX82(pIFIL ifilptr, pXCREblk pxcreblk, ULONG numberOfExtendedCreateBlocks, ctINDEX_ATTRIBUTES *pIndexAttributes);
extern ctCONV  CTERR ctDECL TMPIIDXX8(pIFIL ifilptr,LONG permmask,
	pTEXT groupid,pTEXT fileword,pXCREblk pxcreblk);
extern ctCONV  NINT  ctDECL ctSETHGH(LONG hw);
extern ctCONV  LONG  ctDECL ctGETHGH(VOID );
#endif /* ctHUGEFILE */

extern ctCONV  CTERR ctDECL CREIDX(FILNO keyno,pTEXT filnam,COUNT keylen,COUNT keytyp,COUNT keydup,COUNT nomemb,
					UCOUNT xtdsiz,COUNT filmod);
extern ctCONV  CTERR ctDECL CREIDXX(FILNO keyno,pTEXT filnam,COUNT keylen,COUNT keytyp,COUNT keydup,COUNT nomemb,
					UCOUNT xtdsiz,COUNT filmod,LONG permmask,pTEXT groupid,pTEXT fileword);
extern ctCONV  CTERR ctDECL CREIFIL(pIFIL ifilptr);
extern ctCONV  CTERR ctDECL CREIFILX(pIFIL ifilptr,pTEXT dataextn,pTEXT indxextn,LONG permmask,
					pTEXT groupid,pTEXT fileword);
extern ctCONV  CTERR ctDECL CREISAM(cpTEXT filnam);
extern ctCONV  CTERR ctDECL CREISAMX(cpTEXT filnam,COUNT userprof,cpTEXT userid,cpTEXT userword,cpTEXT servname,
					LONG permmask,cpTEXT groupid,cpTEXT fileword);
extern ctCONV  CTERR ctDECL CREMEM(FILNO keyno,COUNT keylen,COUNT keytyp,COUNT keydup,COUNT membno);
extern ctCONV  CTERR ctDECL CREMEMX(FILNO keyno,COUNT keylen,COUNT keytyp,COUNT keydup,COUNT membno,FILNO ufil);
extern ctCONV  LONG  ctDECL CTASYNC(LONG handle,UINT mode,VRLEN bufsiz,pTEXT bufptr);
extern ctCONV  CTERR ctDECL CTCHKPNT(VOID );
extern ctCONV  CTERR ctDECL CTCHKPNTX(NINT logset);
extern ctCONV  NINT  ctDECL ctCopyFile(pCPYFIL pcpyfil);
extern ctCONV  CTERR ctDECL CTFLUSH(FILNO filno);
extern ctCONV  LONG  ctDECL CTFLUSHX(FILNO filno,NINT mode,LONG parm);
extern ctCONV  CTERR ctDECL CTHIST(FILNO filno,pVOID target,pVOID bufptr,LONG recbyt,VRLEN bufsiz,UCOUNT mode);
extern ctCONV  UINT  ctDECL ctotAlign(UINT align);
extern ctCONV  CTERR ctDECL CTSBLDX(pTEXT filnam,pTEXT fileword);
extern ctCONV  NINT  ctDECL ctSETCFG(NINT option, cpTEXT optionValue);
extern ctCONV  NINT  ctDECL ctSETCOMPRESS(NINT comptype,NINT compvrsn,pTEXT dllname,pTEXT attrstr,VRLEN attrlen);
extern ctCONV  CTERR ctDECL ctSETENCRYPT(cpTEXT mod,cpTEXT key,VRLEN keylen);
extern ctCONV  CTERR ctDECL ctSETSEG(FILNO filno,NINT aseg,NINT tseg,pSEGMDEF pseg);
extern ctCONV  CTERR ctDECL CTSQL(pVOID ,pVOID ,VRLEN, COUNT ,LONG );
extern ctCONV  NINT  ctDECL ctTempDir(pTMPDIR ptmpdir);
extern ctCONV  CTERR ctDECL cttestfunc(COUNT);
extern ctCONV  CTERR ctDECL cttseg(COUNT segpos,COUNT mod,COUNT slen,pTEXT tarptr,pCOUNT aq,pConvMap mp);
extern ctCONV  CTERR ctDECL ctuseg(COUNT spos,COUNT mod,COUNT slen,pTEXT tarptr,pCOUNT aq,pConvMap mp);
#ifdef ctPREV_66A3_CTUSER
extern ctCONV  LONG  ctDECL CTUSER(pTEXT command);
#else
extern ctCONV LONG ctDECL iCTUSER(pTEXT command,pTEXT bufptr,VRLEN bufsiz,pVRLEN poutlen);
extern ctCONV LONG ctDECL CTUSER(pTEXT command,pTEXT bufptr,VRLEN bufsiz);
#ifdef ctBEHAV_CTUSERX
extern ctCONV LONG ctDECL CTUSERX( pTEXT ctlbufptr, pTEXT inbufptr,VRLEN inbufsiz,pTEXT outbufptr,pVRLEN poutbufsiz);
#else
extern ctCONV LONG ctDECL CTUSERX( pTEXT ctlbufptr,VRLEN ctlbufsiz, pTEXT inbufptr,VRLEN inbufsiz,pTEXT outbufptr,VRLEN outbufsiz);
#endif
extern ctCONV LONG ctDECL CTSQLCTL(pTEXT ctlbufptr,VRLEN ctlbufsiz, pTEXT inbufptr,VRLEN inbufsiz,pTEXT outbufptr,VRLEN outbufsiz);
extern ctCONV LONG ctDECL CTSRVCTL(pTEXT ctlbufptr,VRLEN ctlbufsiz, pTEXT inbufptr,VRLEN inbufsiz,pTEXT outbufptr,VRLEN outbufsiz);

extern ctCONV LONG ctDECL CT_USER(pTEXT inString, pTEXT outBuffer,pVRLEN outBufLen);
extern ctCONV LONG ctDECL CT_USERX( pTEXT ctlbufptr, pTEXT inbufptr,VRLEN inbufsiz,pTEXT outbufptr,pVRLEN poutbufsiz);
extern ctCONV LONG ctDECL CT_SQLCTL(pTEXT ctlbufptr,pVRLEN ctlbufsiz, pTEXT inbufptr,pVRLEN inbufsiz,pTEXT outbufptr,pVRLEN outbufsiz);
extern ctCONV LONG ctDECL CT_SRVCTL(pTEXT ctlbufptr,pVRLEN ctlbufsiz, pTEXT inbufptr,pVRLEN inbufsiz,pTEXT outbufptr,pVRLEN outbufsiz);

#endif
extern ctCONV  LONG  ctDECL DATENT(FILNO datno);
extern ctCONV  LONG  ctDECL DELBLD(FILNO keyno,pVOID target);
extern ctCONV  CTERR ctDECL DELCHK(FILNO keyno,pVOID target,LONG recbyt);
extern ctCONV  NINT  ctDECL delIDfield(FILNO datno);
extern ctCONV  CTERR ctDECL DELIFIL(pIFIL ifilptr);
extern ctCONV  CTERR ctDECL DELIFILX(pctDELETE_IFIL_XTD_OPTIONS pDeleteIFILXtdOptions);
extern ctCONV  CTERR ctDECL DELFIL(FILNO filno);
extern ctCONV  CTERR ctDECL DELFILX(FILNO filno, pctDELETE_FILE_XTD_OPTIONS pDeleteFileXtdOptions);
extern ctCONV  CTERR ctDECL DELREC(FILNO datno);
extern ctCONV  CTERR ctDECL DELRES(FILNO datno,pVOID resptr);
extern ctCONV  CTERR ctDECL DELRFIL(FILNO datno);
extern ctCONV  CTERR ctDECL DELRFILX(FILNO datno, pctDELETE_FILE_XTD_OPTIONS pDeleteFileXtdOptions);
extern ctCONV  CTERR ctDECL DELVREC(FILNO datno);
extern ctCONV  LONG  ctDECL ctDISTINCTest(FILNO keyno);
extern ctCONV  LONG  ctDECL ctDISTINCTestXtd(FILNO keyno,LONG nsegs,pDSTSEGV vsegs,pLONG asegs);
extern ctCONV  LONG  ctDECL ctDISTINCTset(FILNO filno,NINT action);
extern ctCONV  LONG  ctDECL ctDISTINCTsetXtd(FILNO filno,NINT action,LONG nsegs,pDSTSEGV vsegs,pLONG asegs);
extern ctCONV  CTERR ctDECL ctDROPIDX(FILNO keyno);
extern ctCONV  NINT  ctDECL ctImpersonateTask(NINT mode, NINT taskid);
extern ctCONV  CTERR ctDECL ENARES(FILNO datno);
extern ctCONV  LONG  ctDECL EQLKEY(FILNO keyno,pVOID target);
extern ctCONV  CTERR ctDECL EQLREC(FILNO keyno,pVOID target,pVOID recptr);
extern ctCONV  LONG  ctDECL ESTKEY(FILNO keyno,pVOID idxval1,pVOID idxval2);
extern ctCONV  CTERR ctDECL EXPIFIL(pIFIL ifilptr);
extern ctCONV  CTERR ctDECL EXPIFILX(pIFIL ifilptr,pTEXT dataextn,pTEXT indxextn,
					LONG permmask,pTEXT groupid,pTEXT fileword);
extern ctCONV  NINT  ctDECL ctFILBLK(cpTEXT filename,LONG action);
extern ctCONV  NINT  ctDECL ctFILBLKX(const ctFILE_BLOCK_XTD_OPTIONS *pctFileBlockXtdOptions);
extern ctCONV  NINT  ctDECL ctFILELIST(pTEXT filnam,pLONG pvalue,pLONG pmember,pNINT pscale,NINT list,NINT action);
extern ctCONV  NINT  ctDECL ctFILMTX(cpTEXT filename,NINT opcode);
extern ctCONV  NINT  ctDECL ctFILWCD(pTEXT infilnam,NINT opcode,pTEXT outfilnam,pVRLEN poutfillen);
extern ctCONV  LONG  ctDECL FRCKEY(FILNO keyno,pVOID idxval,COUNT fractal);
extern ctCONV  CTERR ctDECL FREBAT(VOID );
extern ctCONV  CTERR ctDECL FREBATN(COUNT batnum);
extern ctCONV  CTERR ctDECL FREHST(VOID );
extern ctCONV  CTERR ctDECL FREHSTN(COUNT hstnum);
extern ctCONV  CTERR ctDECL FRERNG(FILNO keyno);
extern ctCONV  CTERR ctDECL FRESET(VOID );
extern ctCONV  CTERR ctDECL FRESETN(COUNT setnum);
extern ctCONV  COUNT ctDECL frmkey(FILNO keyno,pTEXT recptr,pTEXT txt,LONG pntr,VRLEN datlen);
extern ctCONV  COUNT ctDECL frmkeyx(FILNO keyno,pTEXT recptr,pTEXT txt,LONG pntr,VRLEN datlen,NINT mode);
extern ctCONV  LONG  ctDECL FRSKEY(FILNO keyno,pVOID idxval);
extern ctCONV  CTERR ctDECL FRSREC(FILNO filno,pVOID recptr);
extern ctCONV  CTERR ctDECL FRSRNG(FILNO keyno,pVOID recptr);
extern ctCONV  CTERR ctDECL FRSSET(FILNO keyno,pVOID target,pVOID recptr,COUNT siglen);
extern ctCONV  CTERR ctDECL GETALTSEQ(FILNO keyno,pCOUNT altseq);
#ifdef ctCONDIDX
extern ctCONV  VRLEN ctDECL GETCRES(FILNO datno,LONG bufsiz,pVOID idxcnd);
extern ctCONV  VRLEN ctDECL GETCIDX(FILNO keyno,LONG bufsiz,pVOID idxcnd);
#endif
extern ctCONV  pVOID ctDECL GETCTREE(cpTEXT regid);
extern ctCONV  pTEXT ctDECL GETCURK(FILNO keyno,pVOID idxval,pVRLEN plen);
extern ctCONV  pTEXT ctDECL GETCURKL(FILNO keyno,pVOID idxval);
extern ctCONV  LONG  ctDECL GETCURP(FILNO datno);
extern ctCONV  VRLEN ctDECL GETDODA(FILNO datno,LONG buflen,pVOID bufptr,COUNT mode);
extern ctCONV  VRLEN ctDECL GETDODAX(FILNO datno,LONG buflen,pVOID bufptr,COUNT mode,NINT dodaNUM);
extern ctCONV  LONG  ctDECL GETFIL(FILNO filno,COUNT mode);
extern ctCONV  NINT  ctDECL GETFILX(FILNO filno,COUNT mode,pVOID bufptr,pNINT pbuflen);
extern ctCONV  NINT  ctDECL getIDfield(FILNO datno,NINT mode,pLONG8 retval);
extern ctCONV  VRLEN ctDECL GETIFIL(FILNO datno,LONG buflen,pVOID bufptr);
extern ctCONV  VRLEN ctDECL GETIFILX(FILNO datno,LONG buflen,pVOID bufptr,NINT ifilNUM);
extern ctCONV  CTERR ctDECL GETMNAME(FILNO superFileNo, pTEXT nambuf, VRLEN buflen,COUNT memberFileNo);
extern ctCONV  CTERR ctDECL GETNAM(FILNO filno,pTEXT nambuf,VRLEN buflen,COUNT mode);
extern ctCONV  LONG  ctDECL GETRES(FILNO datno,pVOID resptr,pVOID bufptr,VRLEN bufsiz,COUNT resmode);
extern ctCONV  VRLEN ctDECL GETVLEN(FILNO datno);
extern ctCONV  NINT  ctDECL GETXCREBLK(FILNO filno,pXCREblk pxcreblk);
extern ctCONV  LONG  ctDECL GTEKEY(FILNO keyno,pVOID target,pVOID idxval);
extern ctCONV  CTERR ctDECL GTEREC(FILNO keyno,pVOID target,pVOID recptr);
extern ctCONV  LONG  ctDECL GTKEY(FILNO keyno,pVOID target,pVOID idxval);
extern ctCONV  CTERR ctDECL GTREC(FILNO keyno,pVOID target,pVOID recptr);
extern ctCONV  VRLEN ctDECL GTVLEN(FILNO datno,LONG recbyt);
extern ctCONV  LONG  ctDECL IDXENT(FILNO keyno);
extern ctCONV  CTERR ctDECL INTISAM(COUNT bufs,FILNO fils,COUNT sect);
extern ctCONV  CTERR ctDECL INTISAMX(COUNT bufs,FILNO fils,COUNT sect,COUNT dbufs,
					COUNT userprof,cpTEXT userid,cpTEXT userword,cpTEXT servname);
extern ctCONV  CTERR ctDECL INTREE(COUNT bufs,FILNO fils,COUNT sect);
extern ctCONV  CTERR ctDECL INTREEX(COUNT bufs,FILNO fils,COUNT sect,COUNT dbufs,
					COUNT userprof,cpTEXT userid,cpTEXT userword,cpTEXT servname);
extern ctCONV  CTERR ctDECL IOPERFORMANCE(pVOID bufptr);
extern ctCONV  CTERR ctDECL IOPERFORMANCEX(pVOID bufptr);
extern ctCONV  CTERR ctDECL LKISAM(COUNT lokmod);
extern ctCONV  CTERR ctDECL LOADKEY(FILNO keyno,pVOID target,LONG recbyt,COUNT typadd);
extern ctCONV  CTERR ctDECL ctLOKDMP(FILNO refno,pTEXT dumpname,COUNT mode);
extern ctCONV  CTERR ctDECL LOKREC(FILNO datno,COUNT lokmod,LONG recbyt);
extern ctCONV  LONG  ctDECL LSTKEY(FILNO keyno,pVOID idxval);
extern ctCONV  CTERR ctDECL LSTREC(FILNO filno,pVOID recptr);
extern ctCONV  CTERR ctDECL LSTRNG(FILNO keyno,pVOID recptr);
extern ctCONV  CTERR ctDECL LSTSET(FILNO keyno,pVOID target,pVOID recptr,COUNT siglen);
extern ctCONV  LONG  ctDECL LTEKEY(FILNO keyno,pVOID target,pVOID idxval);
extern ctCONV  CTERR ctDECL LTEREC(FILNO keyno,pVOID target,pVOID recptr);
extern ctCONV  LONG  ctDECL LTKEY(FILNO keyno,pVOID target,pVOID idxval);
extern ctCONV  CTERR ctDECL LTREC(FILNO keyno,pVOID target,pVOID recptr);
extern ctCONV  CTERR ctDECL MIDSET(FILNO keyno,pVOID recptr,COUNT siglen);
extern ctCONV  LONG  ctDECL NEWREC(FILNO datno);
extern ctCONV  LONG  ctDECL NEWVREC(FILNO datno,VRLEN varlen);
extern ctCONV  pTEXT ctDECL NXTCTREE(VOID );
extern ctCONV  LONG  ctDECL NXTKEY(FILNO keyno,pVOID idxval);
extern ctCONV  CTERR ctDECL NXTREC(FILNO filno,pVOID recptr);
extern ctCONV  CTERR ctDECL NXTRNG(FILNO keyno,pVOID recptr);
extern ctCONV  CTERR ctDECL NXTSET(FILNO keyno,pVOID recptr);
extern ctCONV  CTERR ctDECL OPNFIL(FILNO ufilno,pTEXT filnam,COUNT filmod);
extern ctCONV  CTERR ctDECL OPNFILX(FILNO filno,pTEXT filnam,COUNT filmod,pTEXT fileword);
extern ctCONV  COUNT ctDECL OPNICON(FILNO datno,FILNO keyno,COUNT contextid);
extern ctCONV  CTERR ctDECL OPNIFIL(pIFIL ifilptr);
extern ctCONV  CTERR ctDECL OPNIFILX(pIFIL ifilptr,pTEXT dataextn,pTEXT indxextn,pTEXT fileword);
extern ctCONV  CTERR ctDECL OPNISAM(cpTEXT filnam);
extern ctCONV  CTERR ctDECL OPNISAMX(cpTEXT filnam,COUNT userprof,cpTEXT userid,cpTEXT userword,cpTEXT servname,cpTEXT fileword);
extern ctCONV  FILNO ctDECL OPNRFIL(FILNO filno,pTEXT filnam,COUNT filmod);
extern ctCONV  FILNO ctDECL OPNRFILX(FILNO filno,pTEXT filnam,COUNT filmod,pTEXT fileword);
extern ctCONV  LONG  ctDECL ORDKEY(FILNO keyno,pVOID target,VRLEN offset,pVOID idxval);
extern         COUNT        PERFORM(COUNT status);
extern ctCONV  CTERR ctDECL PRMIIDX(pIFIL ifilptr);
extern ctCONV  LONG  ctDECL PRVKEY(FILNO keyno,pVOID idxval);
extern ctCONV  CTERR ctDECL PRVREC(FILNO filno,pVOID recptr);
extern ctCONV  CTERR ctDECL PRVRNG(FILNO keyno,pVOID recptr);
extern ctCONV  CTERR ctDECL PRVSET(FILNO keyno,pVOID recptr);
extern ctCONV  CTERR ctDECL PTADMIN(FILNO datno,cpVOID partdesc,LONG prawno,COUNT ptmode);
#ifdef ctCONDIDX
extern ctCONV  CTERR ctDECL PUTCRES(FILNO datno,pVOID idxcnd,VRLEN varlen);
#endif
extern ctCONV  CTERR ctDECL PUTDODA(FILNO datno,pDATOBJ doda,UCOUNT numfld);
extern ctCONV  CTERR ctDECL PUTDODAX(FILNO datno,pDATOBJ doda,UCOUNT numfld,NINT dodaNUM);
extern ctCONV  CTERR ctDECL PUTFIL(FILNO filno,COUNT filmod);
extern ctCONV  CTERR ctDECL PUTHDR(FILNO filno,LONG hdrval,COUNT mode);
extern ctCONV  CTERR ctDECL PUTIFIL(pIFIL ifilptr);
extern ctCONV  CTERR ctDECL PUTIFILX(pIFIL ifilptr,pTEXT dataextn,pTEXT indxextn,pTEXT fileword);
extern ctCONV  CTERR ctDECL PUTIFILX8(pIFIL ifilptr,pTEXT dataextn,pTEXT indxextn,pTEXT fileword,pXCREblk pxcreblk);
extern ctCONV  CTERR ctDECL RBLDATX(FILNO datno,pTEXT datnam,UCOUNT datlen,UCOUNT xtdsiz,COUNT filemd,pTEXT fileword,NINT instnc,NINT purge,pLONG prtlbyts);
extern ctCONV  CTERR ctDECL RBLIDX(FILNO datno,pTEXT datname,FILNO keyno,pTEXT fileword,ppFILE ptfp,pTEXT tfp_name,pLONG duprej,NINT options,pLONG srlrej);
extern ctCONV  CTERR ctDECL RBLMEM(FILNO datno,pTEXT datname,FILNO keyno,COUNT membno,pTEXT fileword,ppFILE ptfp,pTEXT tfp_name,pLONG duprej,NINT options,pLONG srlrej);
extern ctCONV  CTERR ctDECL RBLMEMX(FILNO datno,pTEXT datname,FILNO keyno,COUNT membno,pTEXT fileword,ppFILE ptfp,pTEXT tfp_name,pLONG duprej,NINT options,pLONG srlrej,FILNO ufil);
extern ctCONV  CTERR ctDECL TMPIIDX(pIFIL ifilptr);
extern ctCONV  CTERR ctDECL TMPIIDXX(pIFIL ifilptr,LONG permmask,pTEXT groupid,pTEXT fileword);
extern ctCONV  CTERR ctDECL TRANABT(VOID );
extern ctCONV  CTERR ctDECL TRANABTX(COUNT lokmod);
extern ctCONV  CTERR ctDECL TSTFILNUM(FILNO filno);
extern ctCONV  LONG  ctDECL TSTVREC(FILNO datno,VRLEN varlen);
extern ctCONV  CTERR ctDECL RBLIIDX(pIFIL ifilptr);
extern ctCONV  CTERR ctDECL RBLIFIL(pIFIL ifilptr);
extern ctCONV  CTERR ctDECL RBLIFILX(pIFIL ifilptr,pTEXT dataextn,pTEXT indxextn,
				LONG permmask,pTEXT groupid,pTEXT fileword);
extern ctCONV  CTERR ctDECL RDVREC(FILNO datno,LONG recbyt,pVOID recptr,VRLEN bufsiz);
extern ctCONV  CTERR ctDECL REDIREC(FILNO datno,LONG recbyt,pVOID recptr);
extern ctCONV  CTERR ctDECL REDREC(FILNO datno,LONG recbyt,pVOID recptr);
extern ctCONV  CTERR ctDECL REDVREC(FILNO datno,pVOID recptr,VRLEN bufsiz);
extern ctCONV  CTERR ctDECL REGCTREE(cpTEXT regid);
extern ctCONV  CTERR ctDECL ctRENFIL(FILNO filno,pTEXT newname);
extern ctCONV  CTERR ctDECL RENIFIL(pIFIL ifilptr);
extern ctCONV  CTERR ctDECL RENIFILX(pIFIL ifilptr,pTEXT dataextn,pTEXT indxextn);
extern ctCONV  NINT  ctDECL resetIDfield(FILNO datno, LONG8 nxtval);
extern ctCONV  NINT  ctDECL RESETFLTR(FILNO datno,UCOUNT mode);
extern ctCONV  CTERR ctDECL RETREC(FILNO datno,LONG recbyt);
extern ctCONV  CTERR ctDECL RETVREC(FILNO datno,LONG recbyt);
extern ctCONV  LONG  ctDECL RNGENT(FILNO keyno, pVOID key1, pVOID key2);
extern ctCONV  CTERR ctDECL RRDREC(FILNO datno,pVOID recptr);
extern ctCONV  CTERR ctDECL RWTREC(FILNO datno,pVOID recptr);
extern ctCONV  CTERR ctDECL RWTPREC(FILNO datno,pVOID recptr,VRLEN varlen);
extern ctCONV  CTERR ctDECL RWTVREC(FILNO datno,pVOID recptr,VRLEN varlen);
extern ctCONV  CTERR ctDECL SAVPCLR(VOID );
extern ctCONV  CTERR ctDECL SECURITY(FILNO filno,pVOID bufptr,VRLEN bufsiz,COUNT mode);
extern ctCONV  LONG  ctDECL SERIALNUM(FILNO datno);
extern ctCONV  CTERR ctDECL SESSINC(COUNT delta,pTEXT passkey);
extern ctCONV  CTERR ctDECL SESSVAL(COUNT sessions,pTEXT passkey);
extern ctCONV  CTERR ctDECL SETALTSEQ(FILNO keyno,pCOUNT altseq);
extern ctCONV  CTERR ctDECL SETCURI(FILNO datno,LONG recbyt,pVOID recptr,VRLEN datlen);
extern ctCONV  CTERR ctDECL SETDEFBLK(FILNO datno,pDEFDEF defp);
extern ctCONV  CTERR ctDECL SETDEFBLKX(FILNO datno,pDEFDEF defp,NINT alt);
extern ctCONV  CTERR ctDECL SETFLTR(FILNO datno,pTEXT expression);
extern ctCONV  CTERR ctDECL SETFLTRN(FILNO datno,UCOUNT fltnum,UCOUNT fltopts,pTEXT expression);
extern ctCONV  NINT  ctDECL SETLOGPATH(pTEXT path,NINT mode);
extern ctCONV  CTERR ctDECL SETNODE(cpTEXT nodename);
extern ctCONV  LONG  ctDECL SETOPS(LONG ops,VRLEN dat);
extern ctCONV  CTERR ctDECL SETVARBYTS(FILNO filno,pUTEXT pvbyte);
#ifndef CTBOUND
extern ctCONV  CTERR ctDECL SETSFLV(COUNT mode, UTEXT value);
extern ctCONV  CTERR ctDECL SETWNAM(pTEXT pName);
#endif
extern ctCONV  NINT  ctDECL ctSNAPSHOT(NINT opcode,pTEXT rqstdesc,pVOID snapbufr,VRLEN buflen);
extern ctCONV  LONG  ctDECL SPCLSAV(VOID );
extern ctCONV  CTERR ctDECL SQR(pVOID bufptr,VRLEN bufsiz,COUNT funcId,LONG cursorNumber);
extern ctCONV  CTERR ctDECL STPSRV(pTEXT userword,pTEXT servname,COUNT delay);
extern ctCONV  CTERR ctDECL STPSRVX(pTEXT admnuser,pTEXT userword,pTEXT servname,COUNT delay);
extern ctCONV  CTERR ctDECL STPUSR(VOID );
extern ctCONV  CTERR ctDECL STPUSRA(COUNT mode);
extern ctCONV  CTERR ctDECL SWTCTREE(cpTEXT regid);
extern ctCONV  CTERR ctDECL SYSCFG(pVOID bufptr);
extern ctCONV  CTERR ctDECL SYSLOG(COUNT evclass,LONG event,pTEXT buffer,VRLEN buflen);
extern ctCONV  CTERR ctDECL SYSMON(COUNT mode,LONG timeout,pTEXT buffer,VRLEN buflen);
extern ctCONV  NINT  ctDECL ctSYSPUT(NINT parmcode,LONG parmval);
extern ctCONV  CTERR ctDECL TESTHUGE(FILNO filno);
extern ctCONV  pTEXT ctDECL TFRMKEY(FILNO keyno,pVOID tarptr);
extern ctCONV  CTERR ctDECL TMPNAME(pTEXT bufptr,VRLEN bufsiz);
extern         LONG         TRANBAK(COUNT trnmod,LONG trnval);
extern ctCONV  LONG  ctDECL TRANBEG(COUNT mode);
extern ctCONV  CTERR ctDECL TRANRDY(VOID );
extern ctCONV  CTERR ctDECL TRANEND(COUNT mode);
extern ctCONV  CTERR ctDECL TRANISO(COUNT level);
extern ctCONV  CTERR ctDECL TRANRST(COUNT savpnt);
extern ctCONV  COUNT ctDECL TRANSAV(VOID );
extern ctCONV  LONG  ctDECL TRANUSR(LONG filno,LONG poshw,LONG poslw,LONG offset,LONG attribute,pVOID buffer,VRLEN bufsiz);
extern ctCONV  CTERR ctDECL UNRCTREE(cpTEXT regid);
extern ctCONV  NINT  ctDECL UpdateRecordOffsetForKey(FILNO keyno,pVOID target,LONG8 recbyt);
extern ctCONV  CTERR ctDECL UPDCIDX(FILNO keyno,cpTEXT condexpr);
extern ctCONV  CTERR ctDECL UPDCURI(FILNO datno,COUNT mode);
extern ctCONV  NINT  ctDECL ctUPDTICU(pTEXT filnam,pTEXT fileword);
extern ctCONV  CTERR ctDECL UPDRES(FILNO datno,pVOID resptr,VRLEN varlen);
extern ctCONV  pTEXT ctDECL WCHCTREE(VOID );
extern ctCONV  NINT  ctDECL wchIDfield(FILNO datno);
extern ctCONV  CTERR ctDECL WRTREC(FILNO datno,LONG recbyt,pVOID recptr);
extern ctCONV  CTERR ctDECL WRTVREC(FILNO datno,LONG recbyt,pVOID recptr,VRLEN varlen);
extern ctCONV  CTERR ctDECL setfndval(pVOID bufptr,VRLEN bufsiz);
extern ctCONV COUNT ctVDECL ctree(COUNT fn,FILNO filno,pVOID ptr1,pLONG plong,pVOID ptr2,pVRLEN psize,COUNT mode);

extern ctCONV  CTERR ctDECL EQLVREC(FILNO keyno,pVOID target,pVOID recptr,pVRLEN plen);
extern ctCONV  CTERR ctDECL FRSVREC(FILNO filno,pVOID recptr,pVRLEN plen);
extern ctCONV  CTERR ctDECL GTEVREC(FILNO keyno,pVOID target,pVOID recptr,pVRLEN plen);
extern ctCONV  CTERR ctDECL GTVREC(FILNO keyno,pVOID target,pVOID recptr,pVRLEN plen);
extern ctCONV  CTERR ctDECL LSTVREC(FILNO filno,pVOID recptr,pVRLEN plen);
extern ctCONV  CTERR ctDECL LTEVREC(FILNO keyno,pVOID target,pVOID recptr,pVRLEN plen);
extern ctCONV  CTERR ctDECL LTVREC(FILNO keyno,pVOID target,pVOID recptr,pVRLEN plen);
extern ctCONV  CTERR ctDECL NXTVREC(FILNO filno,pVOID recptr,pVRLEN plen);
extern ctCONV  CTERR ctDECL PRVVREC(FILNO filno,pVOID recptr,pVRLEN plen);

extern ctCONV  CTERR ctDECL FRSVRNG(FILNO keyno,pVOID recptr,pVRLEN plen);
extern ctCONV  CTERR ctDECL LSTVRNG(FILNO keyno,pVOID recptr,pVRLEN plen);
extern ctCONV  CTERR ctDECL NXTVRNG(FILNO keyno,pVOID recptr,pVRLEN plen);
extern ctCONV  CTERR ctDECL PRVVRNG(FILNO keyno,pVOID recptr,pVRLEN plen);

extern ctCONV  CTERR ctDECL FRSVSET(FILNO keyno,pVOID target,pVOID recptr,COUNT siglen,pVRLEN plen);
extern ctCONV  CTERR ctDECL LSTVSET(FILNO keyno,pVOID target,pVOID recptr,COUNT siglen,pVRLEN plen);
extern ctCONV  CTERR ctDECL MIDVSET(FILNO keyno,pVOID recptr,COUNT siglen,pVRLEN plen);
extern ctCONV  CTERR ctDECL NXTVSET(FILNO keyno,pVOID recptr,pVRLEN plen);
extern ctCONV  CTERR ctDECL PRVVSET(FILNO keyno,pVOID recptr,pVRLEN plen);
extern ctCONV  CTERR ctDECL REDIVREC(FILNO datno,LONG recbyt,pVOID recptr,pVRLEN plen);
extern ctCONV  NINT  ctDECL PUTKSEGDEF(NINT filno,NINT segno,pctKSEGDEF pkdef);
extern ctCONV  NINT  ctDECL GETKSEGDEF(NINT filno,NINT segno,pctKSEGDEF pkdef);
extern ctCONV  NINT  ctDECL XFMKSEGDEF(NINT seghnd,pVOID src,NINT srclen,NINT srctyp,pVOID dest,NINT destlen);

#ifdef RB_CALLBACK
extern ctCONV  CTERR ctDECL SETCBRBL(pRBLCBFNC funcptr,UCOUNT step);
extern ctCONV pctRBCBST ctDECL GETCBRBLST(VOID);
#endif

extern NINT ctu8TOu16(pTEXT u8str,pWCHAR u16str,VRLEN u16byt);
extern NINT ctu16TOu8(pWCHAR u16str,pTEXT u8str,VRLEN u8byt);
extern UINT ctua_wcstrlen(cpTEXT unalignstr);
extern UINT ctua_wcstrnlen(cpTEXT unalignstr, size_t len);
extern UINT ct_ucs2strlen(const WCHAR * ucs2str);
extern UINT ct_ucs2strnlen(const WCHAR * ucs2str, size_t len);
extern NINT ctu8TOu16S(pTEXT u8str, VRLEN u8byt, pWCHAR u16str, VRLEN u16byt);

extern ctCONV NINT GetServerInfo(pTEXT buffer, NINT bufsiz);
extern ctCONV NINT GetServerInfoXtd(pTEXT buffer, NINT bufsiz, UCOUNT port, LONG sec);

extern ctCONV  CTERR ctDECL SETFLTRX(FILNO datno, pVOID expr);

extern  ctCONV CTERR ctDECL ctklusr(COUNT task,cpTEXT uid);
extern  ctCONV CTERR ctDECL USERINFO(COUNT task,pUSRPRF pup);
extern  ctCONV NINT  ctDECL USERINFOX(pUSRINFX pusrinfx);
extern  ctCONV CTERR ctDECL USERLIST(pCOUNT pc);
extern	ctCONV CTERR ctDECL ctGetIsamErrorCode(VOID);
extern	ctCONV FILNO ctDECL ctGetIsamFileNbr(VOID);
#ifdef ctPortWINKE_CLIENT
extern	ctCONV LONG  ctDECL ctGetSystemErrorCode(VOID);
#else
extern	ctCONV CTERR ctDECL ctGetSystemErrorCode(VOID);
#endif
extern	ctCONV CTERR ctDECL ctGetUserErrorCode(VOID);
extern	ctCONV VOID  ctDECL ctSetIsamErrorCode(CTERR);
extern	ctCONV VOID  ctDECL ctSetIsamFileNbr(FILNO);
#ifdef ctPortWINKE_CLIENT
extern	ctCONV VOID  ctDECL ctSetSystemErrorCode(LONG);
#else
extern	ctCONV VOID  ctDECL ctSetSystemErrorCode(CTERR);
#endif
extern	ctCONV VOID  ctDECL ctSetUserErrorCode(CTERR);
extern	ctCONV CTERR ctDECL ctgetcndxerr(VOID);
extern	ctCONV CTERR ctDECL ctsetcndxerr(COUNT errcod);

#ifdef ctFeatFILEUSERLOCKinfo
extern ctCONV  NINT  ctDECL ctGetOpenFiles(COUNT ConnectionNumber, pctFILINF pFileInfo, pLONG pNbrFiles);
extern ctCONV  NINT  ctDECL ctGetOpenFilesXtd(COUNT ConnectionNumber, pctFILINFX pFileInfoXtd, pLONG pNbrFiles);
extern ctCONV  NINT  ctDECL ctGetFileUsers(pTEXT FileName, pctUSRINF pFileUserInfo, pLONG pNbrUsers);
extern ctCONV  NINT  ctDECL ctGetFileLocks(pTEXT FileName, NINT options, pctLOKINF pFileLockInfo, pLONG pNbrLocks);
#endif

extern ctCONV  NINT  ctDECL ctCreateSequence(pctSEQATTR pseqattr);
extern ctCONV  NINT  ctDECL ctDeleteSequence(pTEXT seqnam);
extern ctCONV  NINT  ctDECL ctOpenSequence(pTEXT seqnam, pLONG pseqhnd);
extern ctCONV  NINT  ctDECL ctCloseSequence(LONG seqhnd);
extern ctCONV  NINT  ctDECL ctGetSequenceAttrs(LONG seqhnd, pctSEQATTR pseqattr);
extern ctCONV  NINT  ctDECL ctSetSequenceAttrs(LONG seqhnd, pctSEQATTR pseqattr);
extern ctCONV  NINT  ctDECL ctGetCurrentSequenceValue(LONG seqhnd, pLONG8 pcurval, pNINT punkval);
extern ctCONV  NINT  ctDECL ctSetCurrentSequenceValue(LONG seqhnd, pLONG8 pnewval);
extern ctCONV  NINT  ctDECL ctGetNextSequenceValue(LONG seqhnd, pLONG8 pnxtval, pNINT punkval);
extern ctCONV  NINT  ctDECL ctNextSequenceUniqueName(pTEXT buffer, pVRLEN bufsiz);
extern ctCONV  NINT  ctDECL ctReplMasterControl(LONG mode, pVOID data);
extern ctCONV  NINT  ctDECL ctVERIFYidx(FILNO keyno, pctVFYIDXin pvfyin, pctVFYIDXout pvfyout);
extern ctCONV  NINT  ctDECL ctVerifyFile(pctVFYFIL pvfyfil);
extern ctCONV COUNT  ctDECL ctSetCtTempDirName(pTEXT temppath);
extern ctCONV  NINT  ctDECL ctSETCLIENTID(pVOID pclientid, NINT clientidlen);
extern ctCONV  NINT  ctDECL ctDeferredIndexControl(pDFKCTL pdfkctl);
extern ctCONV  NINT  ctDECL ctRecordUpdateCallbackControl(pRUCBCTL prucbctl);
extern ctCONV  NINT  ctDECL ictRecordUpdateCallbackControl(pRUCBCTL prucbctl,NINT direct);
extern ctCONV  NINT  ctDECL ctFullText(pFTSP pftsp);
extern ctCONV  NINT  ctDECL AllocFTSearch(FILNO datno, COUNT FTSno, pTEXT query, NINT mode, pFTSERR pftserr);
extern ctCONV  NINT  ctDECL FreeFTSearch(FILNO datno);
extern ctCONV  NINT  ctDECL FirstInFT(FILNO datno, pVOID recptr, pVRLEN plen);
extern ctCONV  NINT  ctDECL LastInFT(FILNO datno, pVOID recptr, pVRLEN plen);
extern ctCONV  NINT  ctDECL NextInFT(FILNO datno, pVOID recptr, pVRLEN plen);
extern ctCONV  NINT  ctDECL PrevInFT(FILNO datno, pVOID recptr, pVRLEN plen);

/* Connection state functions: */
extern ctCONV  NINT  ctDECL ctConnStateAlloc(NINT mode,NINT alcsiz,ppVOID ppvalue);
extern ctCONV  NINT  ctDECL ctConnStateFree(NINT mode);
extern ctCONV  NINT  ctDECL ctConnStateGet(NINT mode,ppVOID ppvalue);

extern ctCONV  NINT  ctDECL ctSetCommProtocolOption(NINT option,pVOID value);
extern ctCONV  NINT  ctDECL ctSetClientLibraryOption(NINT option, pVOID value);
extern ctCONV TEXT ctDECL ctGetFailOverState();
extern ctCONV VOID ctDECL ctResetFailOverState();
extern ctCONV  NINT  ctDECL ctSetClusterOption(ctCLUSTER_OPTION option,cpTEXT value);
extern ctCONV  NINT  ctDECL ctClusterConfig(ctCLUSTER_INPUT_OPTION inputOptions,pTEXT inputBuffer,ctCLUSTER_OUTPUT_OPTION outputOptions,pTEXT outputBuffer,pVRLEN pOutputBufferSize);
/* Alter the table's schema using the specified record descriptor. */
extern ctCONV  NINT  ctDECL ctAlterSchema(FILNO datno, pVOID precdsc);
extern ctCONV  NINT  ctDECL ctGetRecordConverters(FILNO datno, pTEXT buffer, pVRLEN pbuflen);
extern ctCONV  FILE * ctDECL getCtreeSTDOUT(void);
extern ctCONV  FILE * ctDECL getCtreeSTDERR(void);
extern ctCONV void ctDECL setCtreeSTDOUT(FILE * out);
extern ctCONV void ctDECL setCtreeSTDERR(FILE * err);
extern ctCONV void ctDECL set_debug_callback(NINT location,DBG_FUNC callback);
extern ctCONV const TEXT * ctDECL getCTSTATUSname(void);
extern ctCONV NINT ctDECL setCTSTATUSname(const char * name);
extern ctCONV NINT ctDECL ctSetConfigurationOptions(const char *optionString);
extern ctCONV NINT ctDECL ctSetLicenseOptions(const char *licenseString);
extern ctCONV NINT ctDECL ctSetTransactionId(ULONG8 transactionId);
extern ctCONV NINT ctDECL ctTruncateFile(pTEXT filename,pTEXT fileword);
extern ctCONV NINT ctDECL ctGetRelativeKeyno(FILNO datno, NINT relkey, pFILNO pKeyno);
extern ctCONV NINT ctDECL ctPlugin(ctPLUGIN_COMMAND command, pTEXT inputBuffer, pTEXT outputBuffer, pVRLEN pOutputBufferSize);
extern ctCONV NINT ctDECL ctCreateAuthFile(pTEXT sn, pTEXT outfile, size_t outsiz, pTEXT errbuf, size_t errbuflen);
extern ctCONV NINT ctDECL ctSetNullKeyValue(FILNO keyno, cpTEXT nullkey, ULONG keylen);
extern ctCONV NINT ctDECL ctGetNullKeyValue(FILNO keyno, pTEXT nullkey, pULONG pkeylen);
extern ctCONV NINT ctDECL ctSetAdvancedEncryption(NINT value);
extern ctCONV NINT ctDECL ctGetAdvancedEncryption(void);
extern ctCONV NINT ctDECL ctSetChangeEncryptionOnCompact(NINT value);

extern ctCONV void ctDECL ctReducePath(pTEXT in, pTEXT out, VRLEN len, COUNT insensitive);
extern ctCONV void ctDECL ctRemoveRelativePathReferences(pTEXT in, pTEXT out, VRLEN len);
extern ctCONV NINT ctDECL makCREIFIL(pTEXT inbuf,VRLEN inlen,NINT bytord,ppCREATE_IFIL_PARAMS ppCreateIfilParams);
extern ctCONV void ctDECL freCREIFIL(pCREATE_IFIL_PARAMS outbuf);
extern ctCONV NINT ctDECL makPUTIFIL(pTEXT inbuf, VRLEN inlen, NINT bytord, ppPUTIFIL_PARAMS ppPutIfilParams);
extern ctCONV void ctDECL frePUTIFIL(pPUTIFIL_PARAMS pPutIfilParams);
extern ctCONV NINT ctDECL makRENIFIL(pTEXT inbuf,VRLEN inlen,NINT bytord,ppRENAME_IFIL_PARAMS ppRenameIfilParams);
extern ctCONV void ctDECL freRENIFIL(pRENAME_IFIL_PARAMS outbuf);
extern ctCONV NINT ctDECL makDELIFIL(pTEXT inbuf,VRLEN inlen,NINT bytord,ppDELETE_IFIL_PARAMS ppDeleteIfilParams);
extern ctCONV void ctDECL freDELIFIL(pDELETE_IFIL_PARAMS outbuf);
extern ctCONV NINT ctDECL makDROPIDX(pTEXT inbuf,VRLEN inlen,NINT bytord,ppDROP_INDEX_PARAMS ppDropIndexParams);
extern ctCONV void ctDECL freDROPIDX(pDROP_INDEX_PARAMS outbuf);
extern ctCONV NINT ctDECL makRESADD(pTEXT inbuf,VRLEN inlen,NINT bytord,ppRESOURCE_ADD_PARAMS ppResourceAddParams);
extern ctCONV void ctDECL freRESADD(pRESOURCE_ADD_PARAMS pResourceAddParams);
extern ctCONV NINT ctDECL makRESUPD(pTEXT inbuf,VRLEN inlen,NINT bytord,ppRESOURCE_UPDATE_PARAMS ppResourceUpdateParams);
extern ctCONV void ctDECL freRESUPD(pRESOURCE_UPDATE_PARAMS pResourceUpdateParams);
extern ctCONV NINT ctDECL makRESDEL(pTEXT inbuf,VRLEN inlen,NINT bytord,ppRESOURCE_DELETE_PARAMS ppResourceDeleteParams);
extern ctCONV void ctDECL freRESDEL(pRESOURCE_DELETE_PARAMS pResourceDeleteParams);
extern ctCONV NINT ctDECL makADDIDFIELD(pTEXT inbuf,VRLEN inlen,NINT bytord,ppADD_IDFIELD_PARAMS ppAddIdFieldParams);
extern ctCONV void ctDECL freADDIDFIELD(pADD_IDFIELD_PARAMS pAddIdFieldParams);
extern ctCONV NINT ctDECL makDELIDFIELD(pTEXT inbuf,VRLEN inlen,NINT bytord,ppDEL_IDFIELD_PARAMS ppDelIdFieldParams);
extern ctCONV void ctDECL freDELIDFIELD(pDEL_IDFIELD_PARAMS pDelIdFieldParams);
extern ctCONV NINT ctDECL makRESETIDFIELD(pTEXT inbuf,VRLEN inlen,NINT bytord,ppRESET_IDFIELD_PARAMS ppResetIdFieldParams);
extern ctCONV void ctDECL freRESETIDFIELD(pRESET_IDFIELD_PARAMS pResetIdFieldParams);
extern ctCONV NINT ctDECL makXTDATRIDFIELD(pTEXT inbuf,VRLEN inlen,NINT bytord,ppXTDATR_IDFIELD_PARAMS ppXtdatrIdFieldParams);
extern ctCONV void ctDECL freXTDATRIDFIELD(pXTDATR_IDFIELD_PARAMS pXtdatrIdFieldParams);
extern ctCONV NINT ctDECL makADDCHANGEIDFIELD(pTEXT inbuf, VRLEN inlen, NINT bytord, ppADD_CHANGEIDFIELD_PARAMS ppAddChangeIdFieldParams);
extern ctCONV void ctDECL freADDCHANGEIDFIELD(pADD_CHANGEIDFIELD_PARAMS pAddChangeIdFieldParams);
extern ctCONV NINT ctDECL makDELCHANGEIDFIELD(pTEXT inbuf, VRLEN inlen, NINT bytord, ppDEL_CHANGEIDFIELD_PARAMS ppDelChangeIdFieldParams);
extern ctCONV void ctDECL freDELCHANGEIDFIELD(pDEL_CHANGEIDFIELD_PARAMS pDelChangeIdFieldParams);
extern ctCONV NINT ctDECL makSETDEFBLK(pTEXT inbuf,VRLEN inlen,NINT bytord,ppSETDEFBLKX_PARAMS ppSetDefBlkXParams);
extern ctCONV void ctDECL freSETDEFBLK(pSETDEFBLKX_PARAMS pSetDefBlkXParams);
extern ctCONV NINT ctDECL makCreateIsamIndex(pTEXT inbuf,VRLEN inlen,NINT bytord,ppCREATE_ISAM_INDEX_PARAMS ppCreateIsamIndexParams);
extern ctCONV void ctDECL freCreateIsamIndex(pCREATE_ISAM_INDEX_PARAMS outbuf);
extern ctCONV NINT ctDECL makCREDATX8(pTEXT inbuf,VRLEN inlen,NINT bytord,ppCREATE_DATA_FILE_PARAMS ppCreateDataFileParams);
extern ctCONV void ctDECL freCREDATX8(pCREATE_DATA_FILE_PARAMS outbuf);
extern ctCONV NINT ctDECL makDELFIL(pTEXT inbuf,VRLEN inlen,NINT bytord,ppDELETE_DATA_FILE_PARAMS ppDeleteDataFileParams);
extern ctCONV void ctDECL freDELFIL(pDELETE_DATA_FILE_PARAMS outbuf);
extern ctCONV NINT ctDECL makSERIALNUM(pTEXT inbuf,VRLEN inlen,NINT bytord,ppSERIALNUM_PARAMS ppSerialNumParams);
extern ctCONV void ctDECL freSERIALNUM(pSERIALNUM_PARAMS pSerialNumParams);
extern ctCONV NINT ctDECL makPTADMIN(pTEXT inbuf,VRLEN inlen,ppctPTAPM poutbuf);
extern ctCONV NINT ctDECL makSETNULLKEYVALUE(cpTEXT inbuf, VRLEN inlen, TEXT bytord, SET_NULL_KEY_VALUE_PARAMS** setNullKeyValueParams);
extern ctCONV void ctDECL freSETNULLKEYVALUE(SET_NULL_KEY_VALUE_PARAMS* setNullKeyValueParams);
extern ctCONV VOID ctDECL revobj(pTEXT tp,NINT len);
extern ctCONV VOID ctDECL revbyt(pTEXT tp,NINT len);
extern ctCONV VOID ctDECL revwrd(pTEXT tp,NINT len);
extern ctCONV VOID ctDECL revlng(pTEXT tp,NINT len);
extern ctCONV NINT ctDECL ictcnvfn(pTEXT fn, size_t bufsize, cpTEXT dirnam);
extern ctCONV NINT ctDECL ctdll_load(pCTDLL pctdll);
extern ctCONV NINT ctDECL ctdll_unload(pCTDLL pctdll);
extern ctCONV pTEXT ctDECL ctrt_asctime_r(const struct tm *tim, pTEXT buf);
extern ctCONV struct tm *  ctDECL ctrt_localtime_r(const time_t *tim, struct tm *res);
extern ctCONV struct tm *  ctDECL ctrt_gmtime_r(const time_t *tim, struct tm *res);
extern ctCONV time_t ctDECL ctrt_timegm(struct tm* time);
extern ctCONV time_t ctDECL ctrt_mktime(struct tm* time);
extern ctCONV VOID ctDECL ctPrintProductVersion(pTEXT productname);
extern ctCONV NINT ctDECL  ctOWNER(VOID);
extern ctCONV NINT ctDECL  makcpyfil(pTEXT inbuf, VRLEN inlen, pTEXT errbuf, LONG errbufsiz, ppTEXT poutbuf);
extern ctCONV NINT  ctDECL mak_ctTruncateFile(pTEXT inbuf,VRLEN inlen,pTEXT errbuf,LONG errbufsiz,ppTEXT poutbuf);
extern ctCONV VRLEN ctDECL ctcdelm(cpTEXT tp,NINT ch,ULONG len);
extern ctCONV VOID  ctDECL cpylod(ppTEXT hdp,const VOID * sp,UINT n);
extern ctCONV VOID  ctDECL cpynbuf(pTEXT dp,cpTEXT sp,UINT n);
extern ctCONV NINT  ctDECL ctrt_filmtc(cpTEXT pp,cpTEXT tp);
extern ctCONV NINT  ctDECL ictrt_filmtc(cpTEXT pp,cpTEXT tp,NINT options);
extern ctCONV pTEXT ctDECL ctdate_r(LONG pt, pTEXT buf);
extern ctCONV LONG  ctDECL cttime(VOID );
extern ctCONV VOID  ctDECL ctiflnam(pIFILBLK piblk,pTEXT nambuf,NINT mode,pTEXT ext);
extern ctCONV NINT  ctDECL dyndmpsetopt(NINT option,pVOID value);
extern ctCONV NINT  ctDECL uredirect(pTEXT oldbuf,pTEXT newbuf,pTEXT msgsuc,pTEXT msgerr,pUREDLST puredlst);
extern ctCONV NINT  ctDECL uredirectadd(cpTEXT kv,pUREDLST puredlst);
extern ctCONV COUNT ctDECL dyndmp(pVOID bufptr, VRLEN bufsiz);
extern ctCONV VOID  ctDECL ctCommEnc(pTEXT pMsg1, ULONG msg1Len, pTEXT pMsg2, ULONG msg2Len);
extern ctCONV VOID  ctDECL ctCommDec(pTEXT pMsg1, ULONG msg1Len, pTEXT pMsg2, ULONG msg2Len);
extern ctCONV VOID  ctDECL ctSetOWNER(NINT sOWNR);
extern ctCONV TEXT ctGetNullBit(const UTEXT* BitArray, NINT BitNumber);
extern ctCONV void ctSetNullBit(UTEXT* BitArray, NINT BitNumber, TEXT SetBit);
extern ctCONV NINT ctDECL ctSetServerConnectionState(ULONG8 mode, const void* value);

#ifdef __cplusplus
}
#endif

#endif /* ctDECLH */

/* end of ctdecl.h */
