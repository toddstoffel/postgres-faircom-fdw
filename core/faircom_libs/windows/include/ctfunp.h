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

#ifndef ctFUNPH
#define ctFUNPH

#ifndef ctPARMH
#include "ctparm.h"
#endif

#ifdef DllLoadDS
#define _lds_   DllLoadDS
#else
#define _lds_
#endif

#ifdef CTBOUND

#ifndef ctBONDH
#include "ctbond.h"
#endif

#else

#ifndef ctCOMMH
#include "ctcomm.h"
#endif

#endif

#ifdef ctBNDentryPoint
#ifdef CTPERM
#include "ctlocb.h"
#endif
#endif

#ifdef ctPortWINKE_CLIENT
#include "ctkifc.h"
		/* This header contains declarations for the functions which
		 * comprise the user mode interface to the c-tree database
		 * engine for the Windows kernel.
		 */
#endif

#ifdef ctLOCLIB
#ifdef CTBOUND
#ifdef CTPERM
#include "ctlocf.h"
#endif
#endif
#endif
#include "ctdecl.h"

#ifdef ctMTFPGreentrant
#include "ctgvar.h"
#endif

#ifndef ctGVARH
#define pinHan
#define pthHan1
#define pthHan
#define pioN
#define pupdN
#endif

#include <time.h>

#ifdef __cplusplus
extern "C" {
#endif

#ifdef ctPortWINCE
ctCONV void ctDECL *ctrt_bsearch(const void *key, const void *base, size_t nmemb,size_t size,int (*compar)(const void *, const void *));
#endif
extern  void 	     assignUnalignedToInt8(LONG8 * dest, const TEXT * src, const size_t size);
extern  NINT         check_cofiles(pCTFILE ctnum,NINT sOWNR);
extern  NINT         testIDfield_parameters(LONG xtdatr,LONG ovrlen,LONG8 value,LONG delta);
extern  NINT         testIDfield_overflow(pCTFILE dnum,LONG8 savcur);
extern  NINT         filopnlev(COUNT filmod,NINT redshr,LONG xom);
extern  NINT         cofile_readonly(pCTFILE ctnum,NINT ufil,NINT sOWNR);
extern  NINT         fcb_state(NINT sOWNR);
extern  pTEXT        cnvbuf38(pCTFILE dnum,pTEXT recbuf,VRLEN bufsiz,LONG cv,NINT datno,NINT sOWNR);
#ifdef ctN8
extern  ULONG8       DiagnosticIncrementNopndg(NINT stat);
#endif
#ifdef ctDBGfcqa
extern  NINT         chkidxsemopn(pTREBUF fndbuf,NINT sOWNR,NINT opcode);
extern  NINT         chkdatsemopn(NINT sOWNR,NINT opcode);
extern  NINT         ctChgBufStatusWordNewOld(pGENBUF buf,NINT NewState,NINT OldState,NINT AlwaysSucceed,NINT loc);
#else
extern  NINT         ctChgBufStatusWordNewOld(pGENBUF buf,NINT NewState,NINT OldState,NINT AlwaysSucceed);
#endif
extern  NINT         intfcbkep(VOID );
extern  NINT         rmvfcbkep(pCTFILE ctnum);
extern  NINT         pshfcbstk(pCTFILE ctnum);
extern  NINT         clnrvnum(pCTFILE ctnum,NINT ufilno,NINT sOWNR);
#ifdef ctFeatATOMICop
extern  LONG         ctInterlockedMax(volatile AULONG *max,LONG candidate);
extern  LONG         ctInterlockedModIncrement(volatile AULONG *val,LONG base);
#endif
#ifdef ctFeatATOMICop64
extern  LONG8        ctInterlockedAdd8(volatile AULONG8 *sum,LONG8 delta);
#endif
extern  pTEXT        ctrtnam_ppnd(pTEXT ppndstr,NINT filno,NINT fld pthHan);
extern	COUNT        iFRERNG(FILNO keyno,NINT cleanPart);
extern	NINT         initPARTSERbuf(NINT keyno pthHan);
extern  NINT         chkforhdrlok(pSEMA sema,NINT own);
extern  NINT         ct_interr(NINT errcod,NINT locale,pTEXT msg pthHan);
extern  NINT         chgIDfield(pCTFILE dnum,pHWM(pFUSR,fu) pTEXT recptr,VRLEN datlen pthHan);
extern  NINT         cthdradj(pCTFILE ctnum);
extern  NINT         setIDfield(pCTFILE ctnum,pHWM(NINT,datno) pTEXT recptr,VRLEN datlen,NINT func pthHan);
extern  NINT         setChangeIDfield(pCTFILE ctnum,pHWM(NINT,datno) pTEXT recptr,VRLEN datlen,NINT func pthHan);
extern  NINT         setAutoSTfields(pCTFILE ctnum, pHWM(NINT, datno) pTEXT recptr, VRLEN datlen, TEXT event pthHan);
extern  NINT         setidstseg(pIIDX idxp,pLONG dstsegs,pLONG dstsegl,NINT keydup);
extern  pTEXT        ctfldadr(cpTEXT recptr,const ConvMap* mp,pUINT ln,pNINT varflg,VRLEN datlen,VRLEN fixlen,NINT fldno,pUINT dtype pthHan);
extern  NINT         ctmapDARtoFCB(pCTFILE ctnum,pDAR dar pthHan);
extern  NINT         ctmapFCBtoDAR(pCTFILE ctnum,ULONG resnum,LONG dartyp,NINT delflg pthHan);
extern  NINT         ctgetDAR(pCTFILE knum,NINT serr,UINT filmod pthHan);
extern  NINT         ADDDAR(FILNO filno,pDAR resptr,VRLEN varlen);
extern  NINT         DELDAR(pCTFILE ctnum,pDAR dar,FILNO filno pthHan);
extern  NINT         ctfillseg(pTEXT tp,UTEXT fill,NINT len,NINT segmod);
extern  NINT         chkidxsiz(pCTFILE hnum pthHan);
#ifdef ctDYNHASH
extern  VOID         setdynhsh(pHSHSTT phs,UINT bins,UINT max,UINT loadF,UINT rehashF);
#endif
extern  VOID         set_cttflg_L(NINT loc);
extern  NINT         chkkeydup(COUNT ikeydup,pCTFILE knum pthHan);
extern  NINT         setautodup(pCTFILE ctnum);
extern  NINT         ctcmpprv(pTREBUF bp pthHan);
extern  NINT         ctcmpnxt(pTREBUF bp);
extern  UINT         ctAGGRbufcnt(NINT sOWNR);
extern  UINT         ctAGGRdatcnt(NINT sOWNR);
extern  VOID	     reset_idxavl(pTREBUF buf,NINT sOWNR,NINT resetFlag,NINT bufcntflg);
extern  NINT         ctDBGcoverage(pTEXT comment);
extern  NINT         initctlist(NINT list);
extern  NINT         ctsnamel(pTEXT fn,pCTFILE hnum);
extern  NINT         tstfilsiz(pCTFILE ctnum);
extern  NINT         chkdelstk(pCTFILE ctnum pthHan);
extern  LONG         iFRCKEY(FILNO keyno,pVOID idxval,COUNT fractal);
extern  ctRECPT      iFRCKEY8(FILNO keyno,pVOID idxval,ULONG8 fractal, ULONG8 scale);
extern  NINT         chkfilblk(pTEXT filename,NINT mode,NINT sOWNR);
extern  NINT         ctchkWC(pTEXT pp);
extern  NINT         ctusrlst(pCTFILE ctnum,pTEXT fn,NINT owner,NINT action,NINT loc);
extern  NINT         ctFSpath(pTEXT filename,NINT mode);
extern  NINT         ctFScopy(pTEXT source,pTEXT destination,NINT mode);
extern  NINT         ctFScopyXtd(pTEXT source,pTEXT destination,NINT mode,pVOID clbk,pVOID usrdata);
extern  NINT         ctFSunxcopy(pTEXT source,pTEXT destination,NINT mode);
extern  NINT         ctFSunxcopyXtd(pTEXT source,pTEXT destination,NINT mode,pCPYFNC clbk,pCPYSTT pcpystt);
extern  pFILE        ctFSfopen(pTEXT filename,pTEXT openmode,NINT credir);
extern  NINT         ctFSgetfilesize(pFILE fp,pLONG8 pfilsiz);
extern  NINT         ctFSgetfilesizefd(NINT fd,pLONG8 pfilsiz);
extern  NINT         ctFSdirtraverse(pTEXT dirname, NINT mode, pDirTraverseFunc clbk);
extern  NINT	     ctchkcrepath(pTEXT fn, NINT mode, pNINT perror);
#ifndef ctCLIENT
extern  pTEXT        ctgetfnm(pNINT memtyp pinHan);
extern  VOID         ctputfnm(pTEXT pfn,NINT memtyp pinHan);
extern  NINT         dec_netrecr(pCTFILE dnum,NINT datno,pLhw LONG recbyt,NINT dec pinHan);
#endif
extern  NINT         ctbasdir(pTEXT basdir,pTEXT filename);
extern  NINT 	     buildAidxnam(TEXT * outbuf,TEXT * aidxnam, NINT outsiz, TEXT * pfilnam);
extern  NINT 	     buildAidxnam2(TEXT * outbuf,TEXT * aidxnam, NINT outsiz, TEXT * pfilnam, TEXT * mirrdat);
extern  NINT         icpyname(pTEXT dp,pTEXT sp,UINT nn);
extern  NINT         ctFnSysXfr(pTEXT fn);
extern  NINT         ctFnSysFrc(NINT mbtype,pTEXT fn);
extern  NINT         tstname(pTEXT fn);
extern  NINT         ctTranLockOut(NINT starting);
extern  NINT         ctfmatch(pCTFILE hnum,pLONG fid);
extern  ULONG        ctsetisamflmd(ULONG fmode,ULONG mmode);
extern  NINT         iWRTREC(FILNO datno,pRhw ctRECPT recbyt,VRLEN vlen,VRLEN sizhdr,pVOID recptr,pCTFILE dnum pthHan);
extern  COUNT        rwtpkey81(FILNO datno,pTEXT notbuf,ppDFRKY ppdfrky,pTEXT recptr,ctRECPT pntr2,ctRECPT pntr,ctRECPT old_pntr2,ctRECPT old_pntr,VRLEN odatlen,VRLEN ocmplen,VRLEN oflxdat,VRLEN datlen pthHan);
extern  VOID         ctputdsh(pDATBUF buf,NINT list,NINT end);
extern  NINT         ctigetrowl(pCTFILE dnum,LONG resnum,NINT datno,NINT sOWNR);
extern  NINT         chksecf(pCTFILE dnum,pVOID recptr,VRLEN newlen,pVOID oldptr,VRLEN oldlen,NINT sOWNR);
extern  NINT         ctlogio(pTEXT lognam,pTEXT description,pTEXT buf,VRLEN iosize,VRLEN maxsize);
extern  pTEXT        ctlname(pTEXT lognam,size_t lognamlen,LONG ln,NINT filetype);
extern  NINT         chkrvnum(NINT datno,NINT keyno pthHan);
extern  NINT         ctatoprefillst(pTEXT filnam);
extern  NINT         ctatotrnfillst(pTEXT filnam);
extern  NINT         chkunqkey(NINT datno,NINT keyno,NINT dbglog pthHan);
extern  ctRECPT      ctnxtphy(pCTFILE ctnum,pRhw ctRECPT pntr,pVHDR pvrhdr,NINT mode pthHan);
extern  COUNT        ctdoRNG(FILNO keyno,pVOID recptr,pVRLEN plen,NINT mode);
extern  NINT         ctPREFNCapiALL(NINT fn,NINT filno);
#ifdef ctTESTsrvdllquiet
extern  NINT         ctPOSTFNCapi(NINT fn,NINT filno,NINT errcod);
#else
extern  VOID         ctPOSTFNCapi(NINT fn,NINT filno,NINT errcod);
#endif
extern  ctCONV VOID  ctDECL ctFinal_exit(int mode, int ret );
extern  NINT	     nctSysQueueWrite(NINT qhandle,pVOID msg,NINT msglen);
extern  NINT         ctsetupdflg(NINT keyno,NINT flag pthHan);
extern  ULONG        ctmemfil(pTEXT filnam,pLONG phw);
extern  ULONG        ctcmpfil(pTEXT filnam);
extern  ULONG        ctfscmpfil(pTEXT filnam);
#ifdef ctCOMMH
extern  NINT         ctaddsocklst(NLONG socket, ctUADDR_T clientAddr);
extern  VOID         ctgetsocklst(NINT usrn, pNLONG psocket, ctUADDR_T * pclientAddr);
#endif
extern  VOID         ctrmvsocklst(VOID);
extern  LONG         shm_socket_force_close(pVOID soc);
extern  VOID         ctGetConnectionLimits(pLONG pmaxnodes, pLONG pmaxconnpernode, pLONG plocalconnonly);
extern  VOID         ctSetConnectionLimits(LONG nodes, LONG connpernode, LONG localonly);
extern  NINT         ctNodeListCreate(VOID);
#ifdef ctCOMMH
extern  NINT         ctNodeListAdd(ctUADDR_T ipaddress);
extern  NINT         ctNodeListDel(ctUADDR_T ipaddress);
#endif
extern  VOID         ctNodeListDump(FILE *fp);
extern  VOID         ctNodeListDestroy(VOID);
extern  VOID         ctSQLfree(VOID);
extern  NINT         ctdisc(NINT usrn);
extern  VOID         ct_udefer_yld(ULONG usec);
extern  NINT         ctrt_usrmtc(pTEXT pp,pTEXT tp);
#ifdef ctFeatSNAPSHOT
extern  UINT         histobox(LONG8 testime,LONG8 boxwidth,NINT nbrboxes,NINT virtualboxes);
extern  VOID         histoprt(pFILE sfp,pTEXT descrition,LONG8 cumtime,LONG8 totcount,pULONG boxes,NINT nbrboxes,NINT virtualboxes,LONG8 boxwidth,LONG8 maxtime);
extern  NINT         ctperfm(NINT pevent,LONG8 t1,LONG8 t2,LONG8 t3,NINT sOWNR,pTEXT str);
extern  NINT         ctSNAPSHOTascii(pTEXT ssmsg,NINT sOWNR);
extern  NINT         ctUSSLIST(NINT action,NINT list,NINT sOWNR,pTEXT userid);
extern  NINT         ctSNAPSHOTfilelist(pTEXT filnam);
extern  NINT         ctSNAPSHOTfstop(NINT tries,NINT sOWNR);
extern  NINT         ctSNAPSHOTthrd(NLONG launch);
extern  NINT         ctSNAPSHOTlaunch(NLONG launch,NINT sOWNR);
#endif
extern  NINT         ctauththrd(void);
extern  NINT         ctauthstop(void);
extern  NINT         ctrecyclebinthrd(void);
extern  NINT         ctrecyclebinstop(void);
#ifdef ctDBGDLNGCNT
extern  NINT         ctCMPdlng(ULONG ah,ULONG al,ULONG bh,ULONG bl,NINT loc);
extern  VOID         ctADDdlng(pLONG pah,pLONG pal,LONG bh,LONG bl,NINT loc);
extern  VOID         ctSUBdlng(pLONG pah,pLONG pal,LONG bh,LONG bl,NINT loc);
extern  VOID         ctINCdlng(pLONG pah,pLONG pal,NINT loc);
extern  VOID         ctDECdlng(pLONG pah,pLONG pal,NINT loc);
extern  VOID         ctMULdlng(pLONG pah,pLONG pal,LONG bl,NINT loc);
extern  VOID         ctDIVdlng(pLONG pah,pLONG pal,LONG bl,NINT loc);
extern  LONG         ctMODdlng(LONG ah,LONG al,LONG bl,NINT loc);
#endif
#ifdef ctDBGCALLBACK
extern  NINT         debug_callback(NINT location);
extern  NINT         debug_callback_ptr(NINT location,void * ptr);
extern  NINT         debug_callback_rc(NINT location,NINT rc);
#endif
#ifdef ctFeatDYNPARTRUL
extern  NINT         ctpr_parse_str(pCTFILE dnum, NINT datno, cpTEXT str, pVOID prul pthHan);
extern  VOID         ctpr_clean_rule(ppVOID pprul pthHan);
#else
extern  NINT         ctpr_parse_str(pCTFILE dnum,pTEXT str);
extern  VOID         ctpr_clean_rule(ppVOID pprul);
#endif
extern  NINT         ctgetalgnchnl(pCTFILE ttnum,NINT nbrchn);
extern  NINT         ctfilchn(pTEXT filnam);
extern  NINT         hostkeyl(NINT keyno,NINT pkeyno pthHan);
extern  NINT         ctrstlok(FILNO filno,pLhw LONG pntr pthHan);
extern  NINT         ctsrlseg(pCTFILE dnum,pUCOUNT psrlpos,pCOUNT psrllen,NINT keyno,NINT segno,pISEG piseg pthHan);
extern  NINT         ctsrlseghdr(pCTFILE dnum, NINT keyno, NINT segno, pISEG piseg pthHan);
extern  NINT         ctsplche(pCTFILE ctnum,pTEXT filnam);
extern  LONG         ctpriche(pCTFILE ctnum,pTEXT filnam,pNINT pmbr,pNINT pscl);
extern  LONG         ctpriidx(pCTFILE ctnum,pTEXT filnam,pNINT pmbr,pNINT pscl);
extern  NINT         ctno_che(pCTFILE ctnum,pTEXT filnam);
extern  NINT         ctkepopn(pTEXT filnam,NINT sOWNR);
extern  NINT         ctno_fls(pTEXT filnam);
extern  NINT         ctsplflush(pCTFILE ctnum,NINT mode  pthHan);
extern  NINT         ctfilbuflst(pGENBUF genbuf,pCTFILE hnum,NINT buftyp pthHan);
extern  VOID         ctchkeval(VOID );
extern  VOID         setctEOK(NINT filno,NINT kk pthHan);
extern  NINT         ctkillquietchk(VOID );
#ifdef ctPortWIN32
extern  NINT	     ctWINexit(NINT num);
extern  NINT         FcServerGUIStart(NINT nCmdShow);
extern  NINT	     chkctSERVICE(void);

#ifndef ctPortWDARM
extern  int	     ctLogEvent(DWORD eventId, TCHAR *szArgs[]);
#endif

#endif
extern  VOID	     clsksglst(pNINT pcur,pNINT ptot,ppKSEGLST plst);
#ifdef MULTITRD
extern  NINT         ctconvert38(pTEXT src,pConvMap smap,pConvBlkN blk,VRLEN slen,pTEXT dst,pConvMap dmap,VRLEN dlen,UINT dstalgn,VRLEN fixlen,NINT oneRec);
extern  NINT         resetfcb(NINT reqfil);
extern  NINT         ctusrloks(pFUSR fu);
extern  NINT         ctsetABstate(NINT action,LONG state);
extern  NINT         ctValidTran(VOID );
extern  VOID         ctcatendclr(pCTFILE ctnum,NINT sOWNR);
extern  NINT         ctchkfilnot(pCTFILE ctnum,LONG hwt,LONG lwt,LONG hw,LONG lw,LONG varlen,pTEXT notbuf,NINT opcode,VRLEN reclen,pTEXT recptr,NINT sOWNR);
extern  NINT         qucancel(NINT owner);
#ifdef ctGVARH
extern  NINT         quclose(pCTGV lctgv);
#endif
extern  NINT         qnremove(NINT qhandle,pNOTIFY pnot,pVOID pqueue);
extern  NINT	     ctidlestop(VOID);
extern  NINT         ctBACKGROUNDflushstop(VOID);
extern  NINT         bgflsterm(NINT sOWNR);
extern  NINT	     ctcancelidle(VOID );
extern  NINT	     ctaddwork(NINT worktype,NINT sOWNR);
extern  NINT	     ctrmvwork(NINT worktype,NINT sOWNR);
extern  LONG         ctgetash(NINT sOWNR);
extern  VOID	     setnmda(pLQMSG logq);
extern  VOID	     setuginfo(pLQMSG lqp,NINT sOWNR);
extern  VOID         cclnup2(pCTFILE ctnum,NINT sOWNR,FILNO ufilno);
#else
extern  VOID         cclnup2(pCTFILE ctnum);
#endif
extern  VOID         ctinsp(pULONG *fz);
extern  NINT	     ctabnusr(NINT task);
extern  NINT	     ctrt_press(pTEXT tp,COUNT err);
extern  VOID	     ctClrScr(void);
extern  NINT	     iTRANABT(void);
extern  NINT	     chkseglst(pCTFILE ctnum,NINT seqseg,FILNO filno pthHan);
extern  NINT	     ctcamosys(pTEXT tp);
extern  NINT	     ctslogfil(VOID);
extern  UNIQFILE_RETURN ct_uniqfile_fd(RNDFILE fd1,RNDFILE fd2);
extern  VOID	     ctputmemc(pTEXT loc);
extern  NINT         ctchkunqidx(pXCREblk pxcreblk,pIFIL ifilptr,pIIDX idxp,pNINT idxstt,NINT nomemb,NINT prtnkey);
#ifdef ctPARTITION
extern  pPARTRES     formpr(pCTFILE dnum,NINT prMODE,NINT rawno,pVRLEN vlen pthHan);
extern  NINT         ctpartrbld(pIFILBLK pifilblk pthHan);
extern  NINT         usekincmap(pCTFILE knum,pTEXT keybuf,pTEXT cnvbuf pthHan);
extern  NINT         setkincmap(pCTFILE dnum,pIFIL filptr,NINT datno,NINT kk pthHan);
extern  NINT         kprawno(NINT datno,NINT keyno,cpTEXT prikey pthHan);
#ifdef ctFeatDYNPARTRUL
extern  NINT         kprawno_i(NINT datno,NINT keyno,cpTEXT prikey pthHan);
#endif
extern  NINT         ctchkprtidx(pXCREblk pxcreblk,pIFIL ifilptr,pIFIL ip,pNINT idxstt,NINT prtnkey);
extern  ctRECPT      cthostfrc(pCTFILE knum,NINT keyno,pVOID idxval,COUNT fractal,ULONG scale,ULONG fac_scale pthHan);
extern  ctRECPT      cthostfrc8(pCTFILE knum,NINT keyno,pVOID idxval,ULONG8 fractal,ULONG8 scale,ULONG fac_scale pthHan);
#ifdef pctLFNdef
extern  ctRECPT      cthostest(pctLFN sfn,pCTFILE knum,NINT keyno,pVOID idxval1,pVOID idxval2,NINT segcount pthHan);
#endif
extern  NINT         cthostord(pCTFILE knum,pVOID target,pORDKEY_PART_INFO pOrdKeyPartInfo pthHan);
extern  NINT         ctpartdelete(pCTFILE dnum,FILNO datno pthHan);
extern  NINT         ctpartrule(pCTFILE dnum,NINT datno pthHan);
extern  NINT         ctsetpres(pCTFILE ctnum,FILNO ufilno pthHan);
extern  NINT         ctuniqpartundo(pCTFILE dnum,NINT datno,NINT suckey,NINT action,NINT rawno,NINT instnc,VRLEN varlen,pTEXT recptr,LONG hw,ctRECPT pntr pthHan);
extern  NINT         ctpartdrop(pCTFILE dnum,pCTFILE knum,NINT datno,NINT keyno pthHan);
extern  NINT         ctpartaddk(NINT hkeyno,pTEXT target,NINT prawno,NINT instnc,NINT hugflg pthHan);
extern  NINT         ctxfrres(NINT hstno,NINT partno pthHan);
extern  NINT         ctpartiidx(pCTFILE dnum,NINT datno,NINT keyno,pIFILBLK pifilblk,NINT rblflg,NINT exsidx pthHan);
extern  NINT         ctuniqpartrwt(pCTFILE dnum,pNINT suckey,pNINT errfil,NINT datno,NINT orawno,NINT oinstnc,NINT nrawno,NINT ninstnc,VRLEN varlen,pTEXT recptr,LONG hw,ctRECPT pntr pthHan);
extern  CTERR        ctuniqpartSetGlobalValuesInRecordImage(pCTFILE dnum, NINT datno, VRLEN varlen, pTEXT recptr pthHan);
extern  NINT         ctuniqpart(pCTFILE dnum,pNINT suckey,pNINT errfil,NINT datno,NINT rawno,NINT srlflg,VRLEN varlen,pTEXT recptr,LONG hw,ctRECPT pntr pthHan);
extern  ctRECPT      cthostent(pCTFILE ctnum,NINT filno pthHan);
extern  ctRECPT      cthostdst(pCTFILE ctnum,NINT filno pthHan);
extern  ctRECPT      cthostdst_set(pCTFILE ctnum,NINT filno,NINT action,NINT tranflag pthHan);
#ifdef pctLFNdef
extern  ctRECPT      part_ser(pctLFN kfn,pCTFILE knum,NINT keyno,pTEXT target,pTEXT idxval,NINT direction pthHan);
#endif
extern  COUNT        ctp_seqrec(pCTFILE ctnum,NINT mode,FILNO filno,pTEXT recptr,pVRLEN plen pthHan);
extern  COUNT        ctp_bndrec(pCTFILE ctnum,NINT mode,FILNO filno,pTEXT recptr,pVRLEN plen pthHan);
extern  NINT         ctpartcls(pCTFILE ctnum pthHan);
extern  NINT         ctpartres(NINT datno pthHan);
#ifdef ctFeatDYNPARTRUL
extern  NINT         ctpartcln(pCTFILE ctnum pthHan);
#else
extern  NINT         ctpartcln(pCTFILE ctnum);
#endif
extern  NINT         ctoverloadpart(pCTFILE dnum,pLONG phw,LONG curpt);
extern  LONG         ctunfoldpart(pCTFILE dnum,pLONG phw);
extern  NINT         ctunfoldpartno(pCTFILE dnum,pLONG phw,pNINT pprawno,NINT dbgflg pthHan);
extern  NINT         hostpartno(pCTFILE dnum,pNINT pprawno,NINT datno,LONG prawno pthHan);
#endif
extern  NINT         ctlangtext(LONG lang);
extern  VOID         ctspin(pCTFILE ctnum pthHan);
extern  NINT         hostcuri(NINT datno,NINT partno pthHan);
extern  NINT         partcuri(NINT datno,NINT partno pthHan);
extern  NINT         ctpartno(NINT datno,pVOID recptr,pNINT pprawno,VRLEN varlen,NINT creflg, NINT frmflg pthHan);
extern  pUTEXT       cthexdmp(const UTEXT * memadr,pUTEXT bufadr,UINT memlen);
extern  NINT         cttsthdrlok(pCTFILE ctnum);
extern  NINT         ctbalhdrlok(pCTFILE ctnum,NINT hdrlok);
extern  COUNT        iPUTCRES(FILNO datno,pVOID idxcnd,VRLEN varlen,CONDIDX_CALL_TYPE calltype,NINT keyno,NINT indexConditionType);
extern  COUNT        iUPDCIDX(FILNO keyno,cpTEXT condexpr,CONDIDX_CALL_TYPE calltype,NINT newrky);
extern  VRLEN        iGETCIDX(FILNO keyno,LONG bufsiz,pVOID idxcnd,CONDIDX_CALL_TYPE calltype);
extern  NINT         cpyname(pTEXT dp,pTEXT sp,UINT nn);
extern  NINT         ctksgmap(NINT keyno pthHan);
extern  NINT         getksegdef(pCTFILE ctnum pthHan);
#ifdef ctCLIENTcore
extern  NINT         ctunic(NINT keyno,NINT segno,pConvBlkN cp,pTEXT recpos,pTEXT lstpos,NINT slen,pTEXT keytxt pinHan);
extern  NINT         ctksegswpchk(NINT keyno,NINT segno,NINT srclen,NINT srctyp);
#else
extern  NINT         ctunic(NINT keyno,NINT segno,pConvBlkN cp,pTEXT recpos,pTEXT lstpos,NINT slen,pTEXT keytxt pthHan);
extern  ctCONV       NINT ctDECL ctksegswpchk(NINT keyno,NINT segno,NINT srclen,NINT srctyp);
#endif
extern  VRLEN        ctwalkfld(pConvBlkN vp,pTEXT recpos,pTEXT lstpos,TEXT dbyte,VRLEN datlen);
extern  NINT         chkrecbyt(NINT datno,NINT keyno pthHan);
extern  NINT         ctkeycamo(pCTFILE ctnum,pTEXT buf,NINT buflen);
extern  pIFIL        cnvifilnp(ppTEXT pnp,pTEXT sfilnam,pTEXT dext,pTEXT iext,FILNO datno pthHan);
extern  pIFIL        cnvifilnpx(ppTEXT pnp,pTEXT sfilnam,pTEXT dext,pTEXT iext,FILNO datno pthHan,NINT ifilno);
extern  NINT         ctsysview(NINT loc pthHan);
#ifdef ctCACHE_MEMORY
extern  pTEXT        ctcacheget(NINT numobj,pNINT rawsiz,NINT cachesiz,ppTEXT baseadr);
extern  VOID         ctcacheput(ppTEXT baseadr);
extern  pTEXT        ctcacheary(NINT numobj,NINT rawsiz,NINT cachesiz,ppTEXT aryadr);
#endif
#ifdef ctDBG_MsgLen
extern  VOID         setMsgLen(pTEXT tp,NINT seg,NINT loc,VRLEN val);
extern  VOID         addMsgLen(pTEXT tp,NINT seg,NINT loc,VRLEN val);
extern  VRLEN        getMsgLen(pTEXT tp,NINT seg);
#endif
#ifdef ctLockOutNamidx
extern  NINT         ctTestNamidx(pTEXT nambuf);
extern  NINT         ctLockNamidx(NINT filno);
extern  VOID         ctFreeNamidx(NINT filno);
#endif
extern  NINT         inamidxCRE(pTEXT nambuf,size_t nambuflen);
extern  VOID         inamidxDEL(pTEXT nambuf,pTEXT auxptr);
extern  NINT         ct_locltest(pTEXT filnam);
extern  VOID         tstfcrpvuln(pCTFILE ctnum,TEXT * flname);
extern  NINT         ctprtmb(NINT mode,pTEXT prtbuf,ULONG val);
extern  NINT         ctrenpath(pTEXT fname,pTEXT rname,pTEXT mp pthHan);
extern  NINT         suppressDEL(NINT filno);
extern  UINT         setverson(pCTFILE ctnum);
extern  NINT         tstverson(pCTFILE3 ctnum3,pCTFILE8 ctnum8);
#ifdef MULTITRD
extern  NINT         ctprichethrd(pDEDLST pd);
extern  NINT         ctpriidxthrd(pDEDLST pd);
extern  NINT         getksegfu(pFUSR fu,pCTFILE ltnum,NINT sOWNR);
extern  NINT         tstnambuf(pTEXT fn,NINT sOWNR);
extern  NINT         ctgetusr(pNINT pusr);
extern  LONG         dcvrtim(pLONG ptime);
#endif
extern  NINT	     drevtim(pTEXT buf,size_t bufsiz,LONG gtime);
extern  VOID         clearifil(NINT filno,NINT idxflg,NINT supflg pthHan);
extern  NINT         lowrenfil(pCTFILE ctnum,pTEXT fname,pTEXT rname,NINT mflag pthHan);
extern  VOID         ctloadpath(pTEXT fname,pTEXT pname);
extern  LONG         iCTFLUSH(FILNO filno,NINT waitmode,NINT xmode,pLONG pparm);
extern  NINT         ctstuffpwd(pTEXT buf,pTEXT userid,pTEXT userword);
extern  NINT         ct_chkfil(pTEXT fn,pLONG idkey pthHan);
extern  NINT         ct_fullname(pTEXT filename,pTEXT fullname,NINT buflen);
#ifndef ct_fullname_long
extern  NINT         ct_fullname_long(pTEXT filename,pTEXT fullname,NINT buflen);
#else
extern  NINT         ct_fullname_ext(pTEXT filename,pTEXT fullname,NINT buflen,NINT mode);
#endif
extern  ULONG        ct_diskavl(pTEXT fn, pLONG phw);
#ifdef ctFeatCFG_SCALE_N8
extern  LONG8        ct_diskfull(pCTFILE ctnum,ULONG extion);
#else
extern  NINT         ct_diskfull(pCTFILE ctnum,ULONG extion);
#endif
extern  NINT         nosupmember(pCTFILE ctnum,NINT dfrmod pthHan);
extern  UNIQFILE_RETURN ct_uniqfile(pTEXT fn1,pTEXT fn2);
#ifdef ctPortWIN32
extern  NINT         ct_getuniqfile_fd(HANDLE handle, LONG8 *devID, LONG8 *fileIdx);
#endif
extern  NINT         ct_getuniqfile(cpTEXT fname, LONG8 *devID, LONG8 *fileIdx);
extern  LONG 	     ct_findfile(pctOS_FILE_FIND_INFO ctFindFile);
extern  VOID         ctrefnot(VOID );
extern  NINT         ctsnp(pTEXT np,pTEXT fn);
extern  NINT         ctcpyfil(FILNO filno,pTEXT mp,VRLEN bufsiz,NINT exact);
extern  NINT         ctsetmir(FILNO filno,pTEXT mp pthHan);
#ifdef ctHUGEFILEsegm
#ifdef ctCLIENTcore
extern  pRESSEGM     ctstrsegmdef(NINT aseg,NINT nseg,pSEGMDEF pseg pinHan);
#else
extern  pRESSEGM     ctstrsegmdef(NINT aseg,NINT nseg,pSEGMDEF pseg pthHan);
#endif
extern  COUNT        SETSEGz(FILNO filno,pRESSEGM pres);
extern  VOID         mgbsegm(pLONG pseghw,pLONG pseglw,pTEXT pmb);
extern  NINT         ctputsegmdef(pCTFILE ctnum pthHan);
extern  VOID         cthdrsegm(pCTFILE ctnum,pTEXT tp,pTEXT oname,pTEXT hstnm,NINT seg);
extern  pTEXT        ctnamsegmdef(pCTFILE ctnum,pTEXT tp,NINT seg);
extern  pTEXT        ctsizsegmdef(pCTFILE ctnum,pTEXT tp,NINT seg);
extern  NINT         ctatosegmdef(pTEXT basnam,NINT ii,pTEXT outbuf);
extern  NINT         ctalcsegmdef(pCTFILE ctnum,NINT nseg);
extern  NINT         ctfresegmdef(pCTFILE ctnum,NINT status);
extern  NINT         ctopnsegmdef(pCTFILE ctnum,FILNO ufilno,UCOUNT filmod);
extern  NINT         ctclssegmdef(pCTFILE ctnum,NINT cloze,NINT fredef,NINT seglmt);
extern  RNDFILE      ctdecsegmdef(NINT op_code,pCTFILE ctnum,LONG hw,LONG recbyt,pLONG pahw,pLONG parecbyt,pVRLEN piosize,NINT seghdrflg,pUINT pseg pthHan);
extern  NINT         ctdecsegmext(pCTFILE ctnum,pULONG pextion);
extern  NINT         ctsetmges(pCTFILE ctnum,FILNO ufilno pthHan);
#endif /* ~ctHUGEFILEsegm */

extern  UCOUNT       chkrecsiz(pCTFILE ctnum);
extern  UINT         setrecsiz(pCTFILE ctnum);
extern  LONG         begbytnod(pCTFILE knum);
extern  ctRECPT      begbytfix(pCTFILE ctnum);
extern  ctRECPT      begbytvar(pCTFILE ctnum);
#ifdef ctCAMO
extern  pTEXT	     mballccam(VRLEN iosize pthHan);
extern  VOID	     mbfrencam(ppVOID pobjptr pthHan);
extern  NINT         ctxtrcamo(pCTFILE ctnum);
extern  VOID         ctsetcamo(pCTFILE ctnum,ULONG filkey,FILNO ufilno);
extern  VOID         ctclncamo(pCTFILE ctnum);
extern  pTEXT        ctcamo81(pTEXT bufadr,pLhw LONG recbyt,VRLEN iosize,NINT cammod,LONG camthr,pTEXT camkey,LONG camfln,LONG camver);
extern  NINT         ctchkcamo(VOID );
extern  VOID         ctfnccamo(VOID );
extern  NINT         ctcamosdkinit(VOID );
extern  NINT         ctcamoi(VOID );
#ifdef MULTITRD
extern  NINT         ctgetcamo(pCTFILE ctnum,NINT sOWNR,FILNO ufilno);
extern  NINT         ctprfcamo(NINT sOWNR);
#else
extern  NINT         ctgetcamo(pCTFILE ctnum);
extern  NINT         ctprfcamo(VOID );
#endif
#endif /* ctCAMO */

extern  LONG         dodaflen(UINT ft,UINT fl);

extern  UINT         cthash_shift(pCTFILE ctnum);
extern  NINT         ctfpglok81(RNDFILE lfd,pCTFILE ctnum,pLhw LONG offset,LONG range,NINT mode);
extern  NINT         ctrtgetpos81(pFILE datfp, pLONG8 pgetpos);
extern  NINT         ctrtsetpos81(pFILE datfp, pLhw LONG recbyt);
extern  VRLEN        aGETDODAX(FILNO datno,LONG buflen,pVOID bufptr,NINT olign,COUNT mode,NINT dodaNUM);
extern  COUNT        iGETNAM(FILNO filno,pTEXT nambuf,VRLEN buflen,COUNT mode pthHan);
#ifdef ctThrdFPG
extern  pVOID        ctserl(NINT fn);
extern  NINT         ctunserl(NINT fn);
#endif
extern  UCOUNT       ctsetextsiz(pCTFILE ctnum,UCOUNT xtdsiz,UINT untlen);
extern  COUNT        rensup(FILNO filno,pTEXT oldnam,pTEXT newnam);
extern 	pTEXT        chkifilnp(pIFIL tp,pTEXT filnam,LONG context);
extern  NINT         ctmenu(ppTEXT pi,pTEXT hd,TEXT q,pTEXT quit,NINT ni,NINT mhlp);
extern  VOID         upbuf(pTEXT tp);
extern  NINT         press(pTEXT tp,COUNT err);
extern  NINT         chkwrd(pTEXT tp,size_t tplen,pTEXT prompt1,pTEXT prompt2,NINT whlp);
extern  VOID         getid(pTEXT prompt,NINT offset,NINT hlp);
extern  VOID         getwrd(pTEXT tp,NINT size,pTEXT prompt,NINT whlp,NINT masked);
extern  pTEXT        ctGetMaskedInput(pTEXT buffer,NINT bufsiz);
extern  VOID         getsrv(VOID );
extern  NINT         getstpsrv(VOID );
extern  NINT         getioh(pTEXT tp,size_t tplen,NINT hlp);
#ifdef ctFeatADMIN_API
extern  NINT	     getiob(pTEXT tp,size_t tplen,NINT hlp);
#endif
extern  ULONG        gethdate(pTEXT msg,ULONG curdat,NINT hlp);
extern  VOID         errmsg(COUNT rc);
extern  COUNT        ct_chkplen(pVRLEN );
extern  NINT         icttseg(NINT segpos, NINT mod, NINT slen, pTEXT tarptr, pCOUNT aq, pConvMap mp, NINT atf, FILNO keyno, NINT segno);
#ifdef ctCLIENT
extern  VOID         clearcndx(pLOCLAUX ap);
#else /* ~ctCLIENT */
extern  NINT         ctchkcidx(FILNO datno,COUNT rkeyno);
extern  COUNT        iRDVREC(FILNO datno,ctRECPT recbyt,pVOID recptr,VRLEN bufsiz,NINT redmod,pVRLEN pactlen pthHan);
extern  ctRECPT      iGETCURP(FILNO datno pthHan);
extern  pTEXT        iGETCURK(FILNO keyno,pVOID idxval,pVRLEN plen pthHan);
extern  NINT         ctscnmrk81(pCTFILE ctnum,pVHDR pvhdr,pRhw ctRECPT recbyt,pFsiz ULONG fsize,NINT level);
#ifdef ctFLEXFILE
extern  pCTFILE      ctgetfcb(NINT ff pthHan);
extern  VOID         ctputfcb(NINT ff,NINT addmemb pthHan);
extern  FILNO        iAVLFILNUM(COUNT numfils, UINT imode);
#ifdef MULTITRD
extern  NINT         redofils(NINT fils,NINT sOWNR);
extern  VOID         undoisam(NINT omax,NINT sOWNR);
extern  COUNT        UNDOFLEX(COUNT oldmax);
#else  /* ~MULTITRD */
extern  NINT         redoisam(NINT fils);
extern  NINT         redofils(NINT fils);
#endif /* ~MULTITRD */
#endif /* CTFLEXFILE */
#endif /* ~ctCLIENT */
extern  NINT         ictuseg(COUNT spos,COUNT mod,COUNT slen,pTEXT tarptr,pCOUNT aq,pConvMap mp,FILNO keyno,NINT segno);
extern  NINT         ctdallc(pNINT ptot, pNINT pcur, ppVOID anchor, NINT objsiz, NINT incr);
extern  NINT         ctdallcx(pNINT ptot, pNINT pcur, ppVOID anchor, NINT objsiz, NINT incr, ULONG dallcOptions);
extern  NINT         ctfillff81(pCTFILE ctnum,pRhw ctRECPT begpos,VRLEN iosize);
#ifdef DBG123
extern  NINT         dbg123(pTEXT msg,pTEXT fn,pTEXT rechdr,ctRECPT recbyt);
#endif
#ifdef ctLOG_NEWNOD
extern  NINT         ctlog_newnod(NINT fn,NINT op,NINT km,NINT ow,LONG recbyt);
#endif
#ifdef ctLOG_FILEIO
extern  NINT         ctlog_fileio(NINT fn,NINT op,NINT lc,NINT ow,ctRECPT recbyt,VRLEN iosize,pTEXT buf);
#endif
extern  NINT         ctcmpdlng(ULONG ah,ULONG al,ULONG bh,ULONG bl);
#ifdef ctCLIENT
#ifdef ctCLIENTcore
extern  pConvMap     ctlfschget(FILNO datno pinHan);
extern  NINT         ctsetnlkey(FILNO keyno,NINT cidxflg pinHan);
extern  NINT         ctl_geticon(NINT contextid pinHan);
#endif
#endif
extern  VOID         freectlist(NINT i);
extern  COUNT        iSETCURI(FILNO datno,ctRECPT recbyt,pVOID recptr,VRLEN datlen,FILNO keyno,FILNO skpkey,NINT autolok);
#ifdef ctCONDIDX
extern  pCIFIL       ctigetcidx(FILNO datno, pVOID idxcndIn, VRLEN idxcndLen, GETCRES_OPTION options);
extern  NINT         ctevalcidx(pCIFIL pcifil,FILNO datno,COUNT relkey,pVOID recptr,pConvMap schema,VRLEN fixlen,VRLEN datlen);
extern  VOID         ctfreecidx(pCIFIL pcifil,LONG tfilter);
extern  VOID         ctfcidxstk(ppVOID pstk);
extern  pVOID        ctcpydatflt(pVOID ptrflt,LONG typflt);
#endif
#ifdef ctFILE_ACCESS
extern  NINT         ctFileAccess(pTEXT filnam);
#endif
#ifdef MULTITRD
extern  COUNT        ctdelupl81(pLhw LONG snode,pCTFILE knum,ppTEXT lstkey,IDXSTATE * idxstate);
extern  COUNT        ctdelupl81_rcv(pLhw LONG snode,pCTFILE knum, IDXSTATE * idxstate);
#else
extern  COUNT        ctdelupl81(pLhw LONG snode,pCTFILE knum, IDXSTATE * idxstate);
#endif
extern  NINT         nismfil(pIFIL ifilptr);
extern  pXCREblk     chkxcre(pIFIL ifilptr,pXCREblk pxcreblk,ULONG nxcreblk,pCOUNT perr pthHan);
extern  VOID         clrxcre(pXCREblk lx,pXCREblk pxcreblk);
extern  LONG         tsthghwrd(VOID );
#ifdef MULTITRD
extern  LONG         ctGETHGHi(NINT );
extern  NINT         ctSETHGHi(LONG ,NINT );
#else
#ifdef InstantC
extern  LONG         ctGETHGHi(NINT);
#else
extern  LONG         ctGETHGHi(pthHan1 );
#endif
extern  NINT         ctSETHGHi(LONG pthHan);
#endif
extern  COUNT        ctinsert81(pTREBUF,const struct keyOpInput *,LONG,LONG,NINT,IDXSTATE *);
#ifdef DBGtree
extern  NINT         ctlogtree_init(NINT mode);
extern  NINT         ctlogtree_done(VOID );
extern  NINT         ctlogtree_walk(pCTFILE knum,LONG node,NINT lokind);
extern  NINT         ctlogtree_comp(pTEXT kv1,pTEXT kv2,pCTFILE knum);
extern  NINT         ctlogtree_dump(VOID );
#endif
extern  VOID         ctclrcon(FILNO datno,NINT mode,pVOID ptr);
extern  NINT         ctclrlockon(VOID );
extern  NINT         ctclrlockonbits(ULONG bitsToClear);
extern  COUNT        iretrec(FILNO datno,LONG recbyt,NINT chgent);
#ifdef DBG9477
extern  NINT         log9477(pCTFILE knum,LONG node,NINT locale);
#endif

#ifdef UNIFRMAT
extern  NINT         ctconvert18(pTEXT tp,pConvMap map,pConvBlkN blk,VRLEN len,UTEXT dbyte,NINT cnv_mode,VRLEN fixlen);
extern  VOID         rev_shdr(pTEXT tp);
extern  VOID         rev_rhdr(pTEXT tp);
extern  VOID         rev_thdr(pTEXT tp);
extern  VOID         rev_nhdr(pTEXT tp);
extern  VOID         rev_vhdr(pTEXT tp);
#endif /* UNIFRMAT */

extern  NINT         rev_fhdr(pTEXT tp);
extern  VOID         ctsetkeypos81(SYSNO keyno,pLhw LONG node,NINT elm,pVOID kp,COUNT keylen);
extern  COUNT        rblsavres(FILNO datno,FILNO keyno,pTEXT idxname,pTEXT fileword,pUINT pxflmod,pctENCATR pencattr);
extern  FILNO        ctrvrng(FILNO hghno);
#ifndef ctCLIENT
extern  NINT         ctuutbl81(pCTFILE dnum,pRhw ctRECPT pntr,FILNO datno,NINT recr,NINT options pinHan);
#endif
#ifdef FPUTFGET
#ifdef LOCK_TEST
extern  COUNT        chkutbl81(SYSNO datno,NINT lokmod,pRhw ctRECPT pntr);
#endif
#endif
extern  pTEXT        rbgetbuf(VRLEN bufsiz);
#ifdef ctMTFPG_LOKCNF
extern  NINT         ctchkmlok(pCTFILE ctnum,pNINT mbrmap);
extern  NINT         ctuhashx(LONG uid,LONG pntr);
#else
#ifdef ctDYNHASH
extern  UINT         ctuhashz(FILNO filno,ULONG8 pntr,NINT adjidx pinHan);
#else
extern  NINT         ctuhash(FILNO filno,LONG pntr);
#endif
#endif
#ifdef FASTCOMP
extern  COUNT        CTCOMP(pTEXT val1,pTEXT val2,COUNT keylen);
extern  UCOUNT       CTCOMPU(pTEXT np, pTEXT op, UCOUNT smlen);
#endif
extern  VRLEN        igetifilx(FILNO datno,LONG buflen,pVOID bufptr,pLONG pdf,NINT ifilNUM,NINT outFilnoBytes);
#ifdef MULTITRD
#ifdef ctGVARH
extern  NINT         ctchkadmn(pCTGV lctgv);
#endif
extern  NINT         redoisam(NINT fils,NINT sOWNR,NINT oldfil);
extern  NINT         ct_tflstt(pCTFILE ,NINT ,NINT ,LONG );
extern  NINT         ctTCdone(VOID );
extern  NINT         ctendthread(NINT , NINT, NINT (*)(VOID ));
extern  NINT         pthread_OWNER();
extern  NINT         ctmemmon(VOID );
extern  NINT         ctunblkq(NINT qindex);
extern  NINT         ctblkbsy(pSEMAblk sema);
#ifdef ctFeatUSRBLKTIMOUT
extern  NINT         cttimbsy(pSEMAtim sema);
#endif
extern  pVOID        ctgvallc(NINT sOWNR,NINT alloc,UINT trdflg);
extern  NINT         ctUserAlive(NINT chkcomm);
extern  NINT         validusr(pLQMSG l,NINT t);
extern  NINT         validusrXtd(pLQMSG l, pAdmFils f);
extern  pTEXT        ctGetServerName(NINT protocol);

#endif
#ifndef ctrt_filcmp
extern  ctCONV  NINT ctDECL ctrt_filcmp(cpTEXT n1,cpTEXT n2);
#endif
#ifndef ctrt_filncmp
extern  ctCONV  NINT ctDECL ctrt_filncmp(cpTEXT n1,cpTEXT n2,NINT nn);
#endif

#ifndef	ctrt_stricmp
extern ctCONV NINT ctDECL ctrt_stricmp(const TEXT * n1,const TEXT * n2);
#endif
#ifndef	ctrt_strnicmp
extern ctCONV NINT ctDECL ctrt_strnicmp(cpTEXT n1,cpTEXT n2,size_t l);
#endif

extern const TEXT * ctrt_stristr(const TEXT * data,const TEXT * substr);

#ifndef ctrt_gets
extern 	ctCONV pTEXT ctDECL ctrt_gets(pTEXT str);
#endif

extern ctCONV NINT ctDECLV ctrt_printf_default_impl(const TEXT * fmt, ... ) PRINTFLIKE(1,2);

extern  NINT         blksrlint(NINT,NINT);
#ifndef nd_digPROTO
#define nd_digPROTO
extern  NINT         nd_dig(pTEXT digbuf,NINT digno);
extern  VOID         nd_digenc(pTEXT digbuf,NINT digno,NINT val);
#endif
extern  pTEXT        uTFRMKEY(FILNO keyno,pVOID tarptr);
extern  pTEXT        i2TFRMKEY(FILNO keyno,pVOID tarptr,LONG pntr,NINT usepntr,NINT unctry);
extern  pTEXT        lTFRMKEY(FILNO keyno,pVOID tarptr);
extern  NINT         ctsysint(pCTINIT1 pconfig);
extern  VOID         ctsysdint(VOID);
extern  VOID         ctclnhdr(pCTFILE ctnum);
extern  COUNT        chkvfin(FILNO datno,COUNT k,COUNT slen);

#ifndef CTBOUND
extern  NINT         ctlogadmn(pLQMSG l);
extern  NINT         ctcomsum(pUCOUNT pu,NINT reqflg);
extern  NINT         ctcomchk(pUCOUNT pu,NINT reqflg);
#ifdef ctCLIENTcore
extern pMTEXT recommbuf(pSrvrGbls pGlobals, VRLEN len, VRLEN olen  pinHan);
#endif
extern ctCONV  COUNT ctDECL COMMBUF(pULONG memchg,LONG newslct,VRLEN bufsiz,pLQMSG lqp);
extern pCommFuncPtrs ctCommLoad(ppVOID commGbl,pTEXT dllname);
extern VOID ctCommUnload(ppVOID commGbl);
#endif
extern  VOID         ctlnhdr(pCTFILE ctnum);
extern  COUNT        ishtifil(pIFIL ifilptr,FILNO datno,NINT cloze,pctDELETE_FILE_XTD_OPTIONS pDeleteFileXtdOptions);
extern  pTREBUF      ctintnod81(pTREBUF buf,pCTFILE knum,pLhw LONG node,NINT resetFlag);
extern  COUNT        ctgetsec(FILNO filno,pCTFILE ctnum,pTEXT uid,pTEXT gid,pTEXT pwd,pLONG pm);
extern  NINT         ctkw(cpTEXT kw,cpTEXT *kw_lst,NINT nkw);
#ifdef ctCLIENT
#ifdef ctCLIENTcore
extern  pCOUNT       ctlfsegget(FILNO keyno,COUNT numseg,ppCOUNT paltseq pinHan);
extern  COUNT        ctgetseginfo(FILNO keyno,COUNT segno,COUNT mode pinHan);
#endif
#else /* ctCLIENT */
extern  pCOUNT       ctlfsegget(FILNO keyno,COUNT numseg,ppCOUNT paltseq pinHan);
extern  COUNT        ctgetseginfo(FILNO keyno,COUNT segno,COUNT mode pinHan);
#endif /* ctCLIENT */
extern  VOID         ctlfsegput(FILNO keyno,COUNT segno,COUNT pos,COUNT len,COUNT mod);
extern  VOID	     ctmakid(pLONG id,pCTFILE ctnum);
extern  pTEXT        ctrtnam(pTEXT sp);
extern  NINT         ctspcopn(NINT op_code,pCTFILE ctnum);
extern  NINT         cttrnopn(NINT op_code,pCTFILE ctnum);
#ifndef ctCLIENT
extern  VOID         ctuftbl(FILNO ufil,pCTFILE ctnum,NINT fileunlock pthHan);
extern  pLOKS        ctultbl81(pCTFILE dnum,pRhw ctRECPT pntr,FILNO datno,NINT lokmod,NINT ismflg pthHan);
extern  VOID         cturtbl(NINT nfil,NINT ofil pthHan);
#endif
extern ctCONV  LONG  ctDECL ctdidx(COUNT idx,pTEXT names);
extern  VOID         setifil(pIFIL ifilptr,pIFILBLK piblk,pTEXT dataextn,pTEXT indxextn,pTEXT fileword,LONG permmask,pTEXT groupid);
extern         COUNT iTMPNAMEext(pTEXT bufptr,VRLEN bufsiz,pTEXT ext);
extern ctCONV  COUNT ctDECL iTMPNAME(pTEXT bufptr,VRLEN bufsiz);
extern  COUNT        ctkeytrn(pCTFILE knum);
extern  ctRECPT      seqikey(FILNO keyno,pTEXT idxval,NINT mode);
extern  COUNT        putdodasx(FILNO datno,pDATOBJ doda,UCOUNT numfld,NINT dodaNUM);
extern  COUNT        putdodanx(FILNO datno,pDATOBJ doda,UCOUNT numfld,NINT dodaNUM);
extern  NINT	     inamidx(NINT ctNAMINDXmode,pCTFILE ctnum,pLONG id,LONG filno,NINT locale,NINT dwnlcl);
extern  LONG         srvfileid(NINT incflg pthHan);
#ifdef ctSRVR
extern  NINT         zeroDARmsg(FILNO datno,NINT sOWNR);
extern  NINT         ctchkfmode(pXCREblk px,UINT fm);
extern  NINT         ctchkbmode(UINT fn,pIFILBLK pi);
extern  NINT         ctevent(NINT sOWNR,LONG evclass,LONG event,LONG error,cpTEXT uid,cpTEXT nod,UINT buflen,cpTEXT bufptr);
extern  NINT         cteventx(NINT sOWNR,LONG evclass,LONG event,LONG error,ULONG instid,cpTEXT uid,cpTEXT nod,UINT buflen,cpTEXT bufptr);
extern  NINT         cteventy(NINT sOWNR,LONG evclass,LONG event,LONG error,ULONG instid,cpTEXT uid,cpTEXT nod,UINT buflen,cpTEXT bufptr,NINT toQue);
extern  NINT         ctrbktfls(VOID);
extern  NINT         ctsyslog(VOID );
extern  NINT         ctsrltyp(VOID );
extern  NINT         ctlaunch(pTEXT );
extern  NINT         ctusersig_down(pTEXT);
extern  VOID         ctsrvclndwn(VOID );
extern  NINT         ctoutoff(NINT sOWNR);
extern  NINT         ctsyslogstop(VOID);
#endif
extern  ctCONV COUNT ctDECL ctaddusr(FILNO datno,pVOID recptr);
#ifdef ctFeatLOGLMT
extern  ctCONV COUNT ctDECL ictaddusr(FILNO datno,pVOID recptr,NINT newusr);
#endif
extern  ctCONV COUNT ctDECL ctchgusr(pTEXT bufptr);
extern  void         mbfreel(pVOID objptr);
extern  void         mbfrenl(ppVOID pobjptr);
#ifdef ctHFREE
extern  VOID         cthfree(pVOID p);
#endif
extern  COUNT        igetdefblkx(FILNO filno,pDEFDEF defp,COUNT srvrmode,NINT alt);
extern  COUNT        ctsetseq(pCTFILE knum,pCOUNT aq);
#ifdef MULTITRD
extern  VOID         ctsetnmda(pLOGBLK logb,pLQMSG logq,SYSNO fils);
extern  VOID         ctsetuginfo(pLQMSG lqp);
extern  COUNT        ctsetres(pCTFILE ctnum,FILNO ufilno,UCOUNT filmod,pCRESEC pcresec);
#else
extern  COUNT        ctsetres(pCTFILE ctnum,UCOUNT filmod);
#endif
extern  NINT         ctfusrclr(NINT mode,NINT sOWNR, pUINT entries);
extern  VOID         ctfilok(VOID );
extern  pTEXT	     trimExtension(pTEXT name, pTEXT ext);
extern  pTEXT        makifil(pTEXT buf,pTEXT dext,pTEXT iext,LONG defwrd,UINT rlen,pUINT plen,NINT mode);
#ifndef CTBOUND
extern  COUNT        ctgetseg(FILNO keyno,pTEXT bufptr,VRLEN size);
extern  COUNT        ctgetmap(FILNO datno,pTEXT bufptr,VRLEN size);
#else
extern  NINT         ctStopUser(VOID );
extern  VOID         ctStopServer(NINT retmod);
#endif
extern ctCONV  COUNT ctDECL OPNIFILz(pIFILBLK pifilblk);
extern ctCONV  COUNT ctDECL PUTIFILz(pIFILBLK pifilblk);
extern ctCONV  COUNT ctDECL RENIFILz(pIFILBLK pifilblk);
extern ctCONV  COUNT ctDECL CREIFILz(pIFILBLK pifilblk);
extern ctCONV  COUNT ctDECL PRMIIDXz(pIFILBLK pifilblk);
extern ctCONV  COUNT ctDECL RBLIIDXz(pIFILBLK pifilblk);
extern ctCONV  COUNT ctDECL TMPIIDXz(pIFILBLK pifilblk);
extern ctCONV  COUNT ctDECL RBLIFILz(pIFILBLK pifilblk);
extern ctCONV  COUNT ctDECL CMPIFILz(pIFILBLK pifilblk);
extern         COUNT CLIFILz(pIFILBLK pifilblk);
extern         COUNT DELIFILz(pIFILBLK pifilblk);
extern  COUNT        iintree(pCTINIT1 pconfig);
extern ctCONV  COUNT ctDECL TRANRDY(VOID );
extern  UINT         ctadjadr(UINT alignment,UINT fkind,pVOID fadr);
extern  NINT         ctadjfld(UINT fkind,pTEXT fadr,VRLEN flen);
extern  pVOID        ctrdefx(FILNO datno,pUINT plen,NINT type,NINT alt);
extern  COUNT        getrhdr(pCTFILE dnum,LONG pntr,pRESHDR presblk,NINT namode);
extern  COUNT        putrhdr(pCTFILE dnum,LONG pntr,pRESHDR presblk pthHan);
extern  NINT         ctredres(pCTFILE dnum,LONG pntr,pVOID bufptr,VRLEN bufsiz,pRESHDR presblk,COUNT resmode);
extern ctCONV  COUNT ctDECL GETDEFBLK(FILNO datno,pDEFDEF defp);
extern  COUNT        ctrstdefx(FILNO datno,pIFIL ip,pTEXT dext,pTEXT iext,LONG mode HWt(NINT) HWv(alt));
#ifndef cpybig
extern  pTEXT        bigadr(pTEXT tp,VRLEN offset);
extern  VOID         cpybig(pTEXT dp,pTEXT sp,VRLEN n);
#endif
#ifdef MULTITRD
extern  COUNT        getintr(FILE* ifd, pCOUNT pbufs, pCOUNT pkeys, pCOUNT psecs, pCOUNT pdats, COUNT userprof, pLQMSG lqp, LOGBLK_FILES_TYPE files);
#else
extern  COUNT        getintr(FILE *ifd,pCOUNT pbufs,pCOUNT pkeys,pCOUNT psecs,pCOUNT pdats,COUNT userprof);
#endif
extern  ULONG        ctfsize81(pPLhw pCTFILE ctnum,RNDFILE cfd);
#ifdef ctHUGEFILEsegm
extern  ULONG        ctgsize81(pPLhw pCTFILE ctnum,RNDFILE cfd);
#endif
#ifndef ctFeatNEWctrcvlogPLUGIN
extern  COUNT        ctrcvlog(cpTEXT msg,NINT err);
#endif
extern  VOID         ctrcvlogcls(VOID);

extern  pTEXT        ctdatetime_name(LONG pt, pTEXT buf, TEXT sep);

extern  COUNT        cthstopn(pCTFILE hnum,FILNO ufilno,pTEXT filnam,UCOUNT filmod);
extern  COUNT        ctcresi(LONG hdrpos,FILNO datno,UCOUNT filmod);
#ifdef MULTITRD
extern  COUNT        cresmem(LONG hdrpos,FILNO hstno,FILNO memno,pTEXT afilnam,pTEXT sfilnam, UCOUNT len,COUNT keytyp,COUNT keydup,COUNT memb,UCOUNT filmod,pCRESMEM_PARAMS pCresmemParams,pCRESEC pcresec);
#else
extern  COUNT        cresmem(LONG hdrpos,FILNO hstno,FILNO memno,pTEXT afilnam,pTEXT sfilnam, UCOUNT len,COUNT keytyp,COUNT keydup,COUNT memb,UCOUNT filmod,pCRESMEM_PARAMS pCresmemParams);
#endif
extern  NINT         ctsname1(cpTEXT fn);
extern  NINT         ctsname4(pTEXT fn,NINT baslen,NINT hstno,pTEXT sfn);
#ifndef ctCLIENT

#ifndef thType
#define thType	     /* RAB: Error with MSC6 with ctjump.c */
#endif
extern  LONG         ctdhupd81(pPLhw LONG chg,NINT chgser,pNINT errcod,pCTFILE dnum,FILNO ufil thType);

#ifdef ctFeatBLOCKCACHE
extern  pTEXT	     ctgetblk(NINT listype);
#endif
extern  pLSTITM      ctgetlst(NINT listype pinHan);
extern  NINT         ctputlst(pLSTITM lp,NINT listype pinHan);
#ifdef ctBEHAV_MEMSUB_SPCLST
extern  VOID         ifreectlist(NINT i,pVOID spclst);
extern  pLSTITM      ictgetlst(NINT listype,pVOID spclst pinHan);
extern  NINT         ictputlst(pLSTITM lp,NINT listype,pVOID spclst pinHan);
#endif
#endif
#ifndef ctsfill
extern  VOID         ctsfill(pVOID dp,NINT ch,UINT n);
#endif
#ifndef ctbfill
extern  VOID         ctbfill(pVOID dp,NINT ch,VRLEN n);
#endif
extern  pTEXT        ctnt_getmem(VRLEN iosize); /* communications use - see ctasup.c*/
extern  VOID         ctnt_putmem(pTEXT loc);	/* communications use - see ctasup.c*/
extern  VOID         ctputmemn(ppVOID ploc);
extern  LONG 	     ctmemsiz(pTEXT loc);
#ifndef CTBOUND
extern  VOID         ctbldlcl(NINT sOWNR,FILNO filno,pCTFILE ctnum,NINT auxflg);
#endif
extern  VOID         ctcatend(NINT mode,NINT loc,SYSNO fil,LONG pos);
extern  NINT         ctcfill(pTEXT tp,NINT ch,VRLEN len);
extern  LONG         ctpstk(pTEXT sp);
extern  VOID         ctrstk(VOID );
#ifndef ctThrdFPG
extern  NINT _lds_   ctdefer(LONG );
#endif
extern 	NLONG _lds_ 	ctGetSrVariable(NINT this_var); /* Get Server Variable */
extern 	LONG _lds_ 	ctNPctrt_printf(pTEXT p);

extern  time_t	     cttime_t(VOID );

#ifndef ctsysnap
extern  NINT	     ctsysnap(LONG);
#endif
extern  COUNT        iaddkey(SYSNO keyno,pTEXT target,LONG recbyt,NINT typadd);
extern  COUNT        iiaddkey(const struct keyOpInput * info, struct keyOpState * keystate);
extern  COUNT        adroot81(pCTFILE knum,LONG lpntr2,LONG lpntr,LONG rpntr2,LONG rpntr,pTEXT idxval,IDXSTATE *);
extern  pTREBUF      newnod81(PFAST pCTFILE knum,pPLhw pLONG pnode,NINT virgin);
extern  COUNT        ctwrtbuf(pDATBUF db,UINT loc pthHan);
extern  NINT         ctflushd(pCTFILE ctnum,NINT keep,ULONG8 offset);
#ifdef ctFeatLOGFLUSH_MOD
extern  NINT         ctblock81y(NINT op_code,pCTFILE ctnum,pLhw LONG recbyt,pTEXT recptr,VRLEN len,pLONG pretsiz,pLONG moghgh pthHan);
#else
extern  NINT         ctblock81(NINT op_code,pCTFILE ctnum,pLhw LONG recbyt,pTEXT recptr,VRLEN len,pLONG pretsiz pthHan);
#endif
extern ctCONV pTEXT ctDECL mballc(NINT numobj,UINT sizobj);
extern  pTEXT	     mballc64(ULONG8 size);
extern  void	     mbfree64(pVOID ptr);
extern  void         mbfren(ppVOID pobjptr);
extern ctCONV VOID ctDECL mbfree(pVOID objptr);
#ifdef MTMEMRY
extern  LONG         arep(COUNT mode);
#endif
extern  COUNT        uerr(COUNT err_no);
extern  NINT         usys(NINT sys_cod);
extern  VOID         terr(NINT err_no);
extern  VOID         ctchknum(NINT opmode);
extern  VOID         ctinrnum(NINT opmode,RNDFILE retval);
#ifndef cpylodl
extern  VOID         cpylodl(ppTEXT hdp,const VOID * sp,VRLEN n);
#endif
extern  VOID         cpysrc(pVOID dp,TEXT ** hsp,UINT n);
#ifndef cpysrcl
extern  VOID         cpysrcl(pVOID dp,TEXT ** hsp,VRLEN n);
#endif
extern  NINT         ctcomexc81(NINT mode,pLhwt LONG tran,pGENBUF gbuf pthHan);
extern  VOID         ctlowhsh(pBHL pbhl,pBHL anchor,NINT end);
extern  COUNT        ctfixdel(pCTFILE ctnum,pCTFILE knum);
#ifdef ctFeatMEMFILE
extern  VOID         ctexpmemrec(pCTFILE knum pthHan);
extern  pTREBUF      ctnodmemrec(pCTFILE hnum,LONG hw,LONG node pthHan);
extern  LONG         ctnxtmemrec(pCTFILE ctnum,pLONG phw,LONG recbyt pthHan);
extern  VOID         ctrmvmemrec(pCTFILE dnum,LONG hw,LONG recbyt pthHan);
extern  LONG         ctdecmemrec(pCTFILE ctnum,LONG hw,LONG recbyt pthHan);
extern  LONG         ctincmemrec(pCTFILE ctnum,LONG hw,LONG recbyt pthHan);
#endif
extern  COUNT        ctio81z(NINT op_code,pCTFILE ctnum,pLhw LONG recbyt,pVOID bufadr,VRLEN iosize,VRLEN torigin,NINT ctioatrb pthHan pioN);
extern  COUNT        ctio81x(NINT op_code,pCTFILE ctnum,pLhw LONG recbyt,pVOID bufadr,VRLEN iosize,VRLEN torigin pthHan pioN);
extern  COUNT        ctio81(NINT op_code,pCTFILE ctnum,pLhw LONG recbyt,pVOID bufadr,VRLEN iosize pthHan pioN);
extern  COUNT        ctio(NINT op_code,pCTFILE ctnum,LONG recbyt,pVOID bufadr,VRLEN iosize pthHan pioN);
extern  NINT         ctrelhdr(pCTFILE ctnum);
extern  void         ctrelbuf(NINT bad,pTREBUF bp,NINT sOWNR);
extern  NINT         ctreldat(pDATBUF db,NINT sOWNR,NINT listonly);
extern  UCOUNT       diffimag(pTEXT newptr,UCOUNT newlen,pTEXT oldptr,UCOUNT oldlen);
extern  NINT         compar(pTEXT val1,pTEXT val2,pCTFILE knum);
extern  ppTEXT       chkcopy(VOID );
extern  pTEXT        ctputk81(FILNO keyno,pCTFILE knum,pTEXT recptr,pLhw LONG pntr,NINT mode,VRLEN datlen pthHan);
extern  NINT         tstrec81(PFAST pCTFILE dnum,pLhw LONG recbyt);
extern  COUNT        prthdr(COUNT filno);
extern  COUNT        prtnod(COUNT filno,pLhw LONG nodex,NINT intact_debug);
extern  COUNT        chkidx(COUNT filno,NINT intact_debug);
extern  COUNT        iidelchk(const struct keyOpInput * input, struct keyOpState * keystate);
extern  COUNT        idelchk(SYSNO keyno,pTEXT target,LONG recbyt,NINT blkmode);
extern  NINT         ctdnode(pTEXT sp);
extern  NINT	     ctdnodestop(VOID);
extern  NINT	     DNodeSpecialRequest(const CTFILE * knum, NINT sOWNR,COUNT mode);
extern  NINT	     RemoveMemberFromDnode(const CTFILE * knum, NINT sOWNR);
extern  void	     freeruslst(CTFILE * ctnum);
extern  NINT         ct_maxlogdefer_stop(VOID);
extern  NINT         ctKEEPOPENclosestop(VOID);
extern  COUNT        iPTADMIN(FILNO datno,pVOID partdesc,LONG prawno,NINT ptmode);
extern  void         createPART_AUTO_PURGEthrd(void);
extern  NINT         ctPART_AUTO_PURGEstop(void);
extern  NINT         ctDISKFULLactionstop(VOID);
#ifdef ctPortMACOSX_COCOA
extern  VOID	     ctqmsgstop(NINT qid);
#endif
extern  VOID         ctqnode81(pCTFILE knum,pLhw LONG node,NINT mode);
extern  NINT         ctrspac(pTEXT sp);
extern  NINT	     ctrspacstop(VOID);
extern  NINT         reclaim_super(LONG mbrnum,pCTFILE hnum,pLhw LONG hdrpos1 pthHan);
extern  NINT         ctprmque(pRQMSG rr,NINT nn,NINT sOWNR,NINT filno);
extern  NINT         reclaim_queue(pCTFILE hnum,LONG mbrnum,pTEXT filnam,pLhw LONG lw,NINT tflg,NINT spcfil pthHan);
extern  NINT         ctinitrspac(NINT sOWNR);
extern  VOID         ctrmvexc(pTREBUF buffer,UINT elm);
extern  COUNT        ctdelkey(const struct keyOpInput * input, IDXSTATE * idxstate);
extern  COUNT        rtnode81(pTREBUF buffer,pLhw LONG node,pCTFILE knum);
extern  VOID         rtfredoda(pDATOBJ dodaptr);
extern  VOID         rtdrvr_free(pVOID pr,pCOUNT fna);
#ifdef MULTITRD
extern  SYSNO        ckflmp(FILNO filno,NINT owner);
extern  pFUSR        ctrvfl(NINT sOWNR,SYSNO afil);
extern  COUNT        ctdwnfil(NINT sOWNR,SYSNO filno,NINT cloze,FILNO ufil,NINT direct,NINT locale);
#ifdef ctSSrel2
extern  COUNT        ctaddblk(ppBLULST blkh,ppBLULST blkt,NINT sOWNR,pRECLOK cp,pVOID cb,NINT loktyp,NINT hdrflg,NINT ssrlzr);
#else
extern  COUNT        ctaddblk(ppBLULST blkh,ppBLULST blkt,NINT sOWNR,pRECLOK cp,pVOID cb,NINT loktyp,NINT hdrflg);
#endif
extern  NINT         ctremblk(ppBLULST blkh,ppBLULST blkt,pRECLOK cp,pCTFILE ctnum);
#endif
extern  void	     ctfreeFileSema(pCTFILE ctnum);
extern  SYSNO        chkopnx(pTEXT fn,NINT mode,UINT filmod,pUINT pauxmod);
extern  NINT         chkredf(FILNO ufil,pCTFILE ctnum pthHan);
extern  TEXT         chkredsrv(pthHan1);
extern  SYSNO        getctf(void);
extern  NINT         getctm(COUNT blocks,pCTFILE btnum);
extern  VOID         retctf(SYSNO filno);
extern  COUNT        ctfree(NINT mode);
extern  NINT         ctstpsrv(VOID );
extern  NINT         ctrelusr(NINT owner);
extern  LONG         extfil(pCTFILE ctnum,VRLEN rsize,VRLEN align,NINT lock);
extern  pCTFILE      tstfnm(SYSNO filno pthHan);
#ifdef ctFeatFILEBLOCK
extern  NINT         filblk_clrusrblks(NINT sOWNR);
extern  pCTFILE      tstifnmcls(FILNO filno,NINT cloze,NINT sOWNR);
extern  NINT         filblk_suspend(NINT ufilno,NINT sOWNR,NINT abortable);
extern  pTEXT        ctgetaltlist(pCTFILE ctnum);
extern  NINT         filblk_termuser(pTEXT filnam,NINT sOWNR);
extern  NINT         filblk_clsfil(NINT isamfile,NINT usrno,NINT sOWNR);
extern  pFUSR        ctrvfblkmp(NINT sOWNR,SYSNO afil,NINT datno);
extern  pFUSR        ctrvfblku(NINT sOWNR,SYSNO afil,NINT test);
extern  pFUSR        ctrvfblk(NINT sOWNR,SYSNO afil,NINT mode,NINT test);
extern  pCTFILE      tstbfnm(SYSNO sysno,FILNO usrno,NINT sOWNR);
#endif
#ifdef ctFeatQUIET
extern  NINT         quiet_suspend(NINT blkstt,NINT sOWNR,NINT readmode);
#endif
#ifndef ctBEHAV_AbortOnClose
extern  pCTFILE      tstcnm(SYSNO filno pthHan);
#endif
extern  NINT         vtclosex(NINT loopcnt);
extern  COUNT        vtclose(VOID );
extern  pFILE	     ctgetstream(cpTEXT sn,cpTEXT mode,NINT repeat);
extern  pFILE	     ctfrestream(cpTEXT sn,cpTEXT mode,NINT channel);
extern  VOID         inrfil(PFAST pCTFILE ctnum,NINT mode,NINT direct);
extern  COUNT        tstupd(pCTFILE knum pthHan pupdN);
extern  COUNT        redhdr(PFAST pCTFILE ctnum,NINT mode);
extern  COUNT        filhdr2(pCTFILE ctnum);
extern  COUNT        filhdr(PFAST pCTFILE ctnum);
extern  COUNT        wrthdr(PFAST pCTFILE ctnum);
extern  COUNT        wrthdrhgh(pCTFILE ctnum);
extern  COUNT 	     iopnfil(SYSNO filno,pTEXT filnam,UCOUNT filmod);
extern  COUNT        iopnfilx(SYSNO filno,pTEXT filnam,UCOUNT filmod,UINT auxmod);
extern  COUNT        wrtnod(PFAST pTREBUF buffer);
extern  COUNT        iclsfilx(SYSNO filno,UCOUNT filmod,NINT xmode);
extern  COUNT        iclsfil(SYSNO filno,UCOUNT filmod);
extern ctCONV  COUNT ctDECL reset_cur(PFAST FILNO keyno,pRhw ctRECPT pntr,pTEXT recptr);
#ifdef ctCLIENT
extern  ctCONV COUNT ctDECL reset_cur2(PFAST FILNO keyno,pRhw ctRECPT pntr,pTEXT recptr,pVRLEN plen);
#else
extern  ctCONV COUNT ctDECL reset_cur2(PFAST FILNO keyno,pRhw ctRECPT pntr,pTEXT recptr,pVRLEN plen pthHan);
#endif
extern  COUNT        RTSCRIPT(pTEXT script,pDATOBJ extdoda,pTEXT outname,LONG options,pTEXT errbuf,VRLEN buflen,COUNT filmod);
extern  LONG         chkism(FILNO datno);
extern  VOID         setsrlpos(pCTFILE ctnum,pTEXT recptr pthHan);
#ifdef ctHUGEFILE
extern  COUNT        putrhdr8(pCTFILE dnum,LONG hw,LONG pntr,pRESHDR8 presblk pthHan);
extern  pTEXT        makifilx(pTEXT buf,pTEXT dext,pTEXT iext,LONG defwrd,UINT rlen,pUINT plen,NINT mode,ppXCREblk ppxcreblk,NINT pndidx,NINT inFilnoBytes,NINT outFilnoBytes);
extern  pTEXT        cptifilx(pIFIL ifilp,pTEXT buf,pVRLEN plen,pTEXT dext,pTEXT iext,pXCREblk pxcreblk,NINT pndidx,pCPTIFIL_PARAMS pCptIfilParams,NINT outFilnoBytes);
extern  VOID         setifil8(pIFIL ifilptr,pIFILBLK piblk,pTEXT dataextn,pTEXT indxextn,pTEXT fileword,LONG permmask,pTEXT groupid,pXCREblk pxcreblk);
extern  NINT         rev_fhdr2(pTEXT tp);
extern  VOID         rev_rhdr8(pTEXT tp);
extern  COUNT        getrhdr8(pCTFILE dnum,LONG pntr2,LONG pntr,pRESHDR8 presblk,NINT namode);
extern  NINT         ctredres8(pCTFILE dnum,LONG pntr2,LONG pntr1,pVOID bufptr,VRLEN bufsiz,pRESHDR8 presblk,COUNT resmode);
extern  VOID         iundo81(COUNT op_code,FILNO datno,COUNT i,LONG pntr2,LONG pntr,LONG old_pntr2,LONG old_pntr);
#else  /* ~ctHUGEFILE */
extern  VOID         iundo81(COUNT op_code,FILNO datno,COUNT i,LONG pntr,LONG old_pntr);
#endif /* ~ctHUGEFILE */
#ifdef ctHUGEFILE
extern  COUNT        rwtikey81(FILNO datno,pTEXT notbuf,ppDFRKY ppdfrky,pTEXT recptr,LONG pntr2,LONG pntr,LONG old_pntr2,LONG old_pntr,VRLEN odatlen,NINT ocmplen,NINT oflxdat,VRLEN datlen pthHan);
extern  COUNT        addikey81(FILNO datno,pTEXT notbuf,ppDFRKY ppdfrky,pTEXT recptr,LONG hw,LONG pntr,VRLEN datlen pthHan);
extern  COUNT        delikey81(FILNO datno,pTEXT notbuf,ppDFRKY ppdfrky,ctRECPT hw,ctRECPT pntr,VRLEN datlen,NINT cmplen,NINT flxdat pthHan);
#else  /* ~ctHUGEFILE */
extern  COUNT        rwtikey81(FILNO datno,pTEXT recptr,LONG pntr,LONG old_pntr,VRLEN datlen pthHan);
extern  COUNT        addikey81(FILNO datno,pTEXT recptr,LONG pntr,VRLEN datlen pthHan);
extern  COUNT        delikey81(FILNO datno,ctRECPT pntr pthHan);
#endif /* ~ctHUGEFILE */
#ifdef MULTITRD
extern  NINT         ctiisam(pLOGBLK plog,pLQMSG lqp);
extern  VOID         ctfisam(NINT sOWNR,SYSNO fils);
#else
extern  NINT         ctiisam(COUNT bufs,SYSNO fils,COUNT sect,COUNT dbufs,COUNT userprof);
extern  VOID         ctfisam(SYSNO fils);
#endif
extern  VOID         rtfisam(VOID);
extern  COUNT        setudat(FILE *ifd,FILNO datno);
extern  COUNT        setukey(FILE *ifd,FILNO keyno,COUNT keytyp);
extern  pCTFILE      tstifnm(FILNO filno pthHan);
#ifndef ctBEHAV_AbortOnClose
extern  pCTFILE      tsticnm(FILNO filno pthHan);
#endif
extern  COUNT        ierr(COUNT err_cod,FILNO file_no);
extern  NINT         addlok81(pCTFILE dnum,pRhw ctRECPT pntr,FILNO datno);
extern  TEXT         ucase(TEXT t);
extern  NINT         ctasskey(FILNO keyno, FILNO datno, pCTFILE knum, pCTFILE dnum, pTEXT recptr, PFAST pTEXT txt, UINT txtlen, VRLEN datlen, NINT pflag);
extern  COUNT        getdatr(FILE *ifd,pFILNO pdatno,pTEXT pname,pUCOUNT pdatln,pCOUNT pdkeys,pUCOUNT pxtsiz,pCOUNT pfilmd);
extern  COUNT        getidxr(FILE *ifd,FILNO datno,COUNT dfilemd,COUNT j,pFILNO pkeyno,pTEXT pname,pCOUNT pklen,pCOUNT pktyp,pCOUNT pdflg,pCOUNT pnmem,pUCOUNT pxtsiz,pCOUNT pfilmd);
extern  COUNT        getambr(FILE *ifd,FILNO datno,COUNT dfilemd,COUNT j,FILNO keyno,pCOUNT pklen,pCOUNT pktyp,pCOUNT pdflg);
extern  VOID         ctsetlog(pCTFILE ctnum,UCOUNT filmod);

#ifdef ctHUGEFILE
extern  VOID         setxcreblk(pXCREblk pxcreblk,pCTFILE ctnum);
extern  COUNT        icredat8(SYSNO datno,pTEXT filnam,pTEXT sfilnam,UCOUNT datlen,UCOUNT xtdsiz,UCOUNT filmod,pCTFILE hnum,LONG hp2,LONG hp1,pXCREblk pxcreblk);
extern  COUNT        icreidx8(SYSNO keyno,pTEXT filnam,pTEXT sfilnam,COUNT keylen,COUNT keytyp,COUNT keydup,COUNT nomemb,UCOUNT xtdsiz,UCOUNT filmod,pCTFILE hnum,LONG hp2,LONG hp1,pXCREblk pxcreblk);
extern  NINT         frmlkey8(pCTFILE ctnum,pTEXT pkeybuf,pTEXT plen,ctRECPT hw,ctRECPT pntr);
#endif /* ctHUGEFILE */
extern  VOID         ctadddlng(pLONG pah,pLONG pal,LONG bh,LONG bl);
extern  VOID         ctsubdlng(pLONG pah,pLONG pal,LONG bh,LONG bl);
extern  VOID         ctincdlng(pLONG pah,pLONG pal);
extern  VOID         ctdecdlng(pLONG pah,pLONG pal);
extern  VOID         ctdivdlng(pLONG pah,pLONG pal,LONG bl);
extern  VOID         ctdiv64(pLONG pah,pLONG pal,LONG bh,LONG bl);
extern  LONG         ctmoddlng(LONG ah,LONG al,LONG bl);
extern  VOID         ctmuldlng(pLONG pah,pLONG pal,LONG bl);
#ifdef ctMULTIOPN
extern  NINT         iunlock81(NINT usr,pLhw LONG pos,NINT ufil,pCTFILE ctnum);
#else
extern  NINT         iunlock81(NINT usr,pLhw LONG pos,pCTFILE ctnum);
#endif
extern  COUNT        cts_lok81(NINT usrn,SYSNO filno,NINT loktyp,NINT mode,pLhw LONG recbyt,NINT dlkfre);

extern  COUNT        icredat(SYSNO datno,pTEXT filnam,pTEXT sfilnam,UCOUNT datlen,UCOUNT xtdsiz,UCOUNT filmod,pCTFILE hnum,LONG hp);
extern  NINT         ctidxcap(pCTFILE ,COUNT ,COUNT ,COUNT ,NINT );
extern  COUNT        icreidx(SYSNO keyno,pTEXT filnam,pTEXT sfilnam,COUNT keylen,COUNT keytyp,COUNT keydup,COUNT nomemb,UCOUNT xtdsiz,UCOUNT filmod,pCTFILE hnum,LONG hp);
extern  pCTFILE      icremem(SYSNO keyno,COUNT keylen,COUNT keytyp,COUNT keydup,COUNT membno,FILNO ufil);
extern  UINT         ctelmexc81(pTREBUF buffer,NINT elm,pPLhwt pULONG ptran);
extern  NINT         ctexcp(pTREBUF buffer,NINT elm,NINT adflag,LONG tran);
#ifdef MULTITRD
extern  NINT         ctgetkbf(pCTFILE knum,NINT sOWNR,FILNO keyno,COUNT keylen);
#else
extern  NINT         ctgetkbf(pCTFILE knum,COUNT keylen);
#endif
#ifndef ctCLIENT
extern  NINT         nodser(PFAST pTREBUF buffer,pTEXT idxval,TEXT stratg,NINT adflag, IDXSTATE * idxstate pthHan);
#endif
extern  VOID         ctputhsh(pTREBUF buf,NINT list,NINT end);
extern  VOID         ctclrhsh(pTREBUF buf pthHan);
#ifndef ctCLIENT
extern  pTREBUF      ctgetnod81(NINT lokind,pLhw LONG node,pCTFILE knum pthHan);
extern  pTREBUF      ctgetnod(NINT lokind,LONG node,pCTFILE knum pthHan);
extern  pTREBUF      lrubuf81(NINT mode,pLhw LONG node,pCTFILE knum,pULONG pidxbufins pthHan);
#endif
extern  VOID         ctnodcap(pTREBUF buffer,pCTFILE knum,NINT lf);
#ifdef FPUTFGET
extern  LONG         gtroot(pCTFILE knum pthHan);
#else
#ifdef ctHUGEFILE
extern  LONG         gtroot(pCTFILE knum pthHan);
#endif
#endif
extern  pTEXT        valpnt(pTREBUF buffer,PFAST NINT elm);
extern  pTEXT        mrkpnt(pTREBUF buffer,NINT elm);
extern  NINT         ctgethghlen(pTREBUF buffer);
extern  LONG         nodpnt81(pPLhw pTREBUF buffer,PFAST NINT elm);
extern  LONG         drnpnt81(pPRhw pTREBUF buffer,PFAST NINT elm);
extern  NINT         setidxcorrupt(pCTFILE knum,pTEXT errmsg,UINT auxcod,NINT serr pthHan pupdN);
extern  void         invspcmgtidx(pCTFILE knum,pTEXT errmsg);
extern  NINT         nodpnt81e(pLONG plw,pPLhw pTREBUF buffer,PFAST NINT elm);
extern  pTEXT        drnpnt81p(pctRECPT plw,pPRhw pTREBUF buffer,PFAST NINT elm);
extern  pTEXT        expval(PFAST pTREBUF bp,NINT n);
extern  COUNT        nodctl81(pCTFILE ctnum,pLhw LONG node,pTREBUF buffer,pVHDR pvrhdr,LONG mbrnum,NINT typflg);
extern  COUNT        ct_strip(PFAST pCTFILE knum);
extern  COUNT        iloadkey81(const struct keyOpInput * input,pPLhw pLONG pnv);
extern  VOID         newleaf(PFAST pTREBUF bp,PFAST pCTFILE kp);
extern  VOID         nonleaf(PFAST pTREBUF bp,PFAST pCTFILE kp);
extern  COUNT        rerr(COUNT err_cod,FILNO file_no);
extern  COUNT        yesno(VOID );
extern  COUNT        vcparm(pUCOUNT pp,UCOUNT rv,pTEXT txt);
extern  COUNT        vtparm(pTEXT pp,UCOUNT rv,pTEXT txt);
extern  COUNT        prnprm(UCOUNT hv,UCOUNT rv,pTEXT txt);
extern  VOID         ctupdkey(pCTFILE knum);
extern  COUNT        scndat(pCTFILE dnum,FILNO datno,pNINT preshdrok,NINT options,pLONG prtlbyts);
extern  COUNT        cts_lok(NINT usrn,COUNT filno,NINT loktyp,NINT mode,LONG recbyt,NINT dlkfre);
extern  COUNT        LOCK81(pLhw LONG node,pCTFILE knum);
extern  COUNT        LOCK(LONG node,pCTFILE knum);
extern  NINT         iunlock(NINT usr,LONG pos,pCTFILE ctnum);
extern  COUNT        UNLOCK81(pLhw LONG node,pCTFILE knum);
extern  COUNT        DLOCK81(pLhw LONG recbyt,pCTFILE dnum,NINT lokmod);
extern  COUNT        RLOCK81(pLhw LONG recbyt,pCTFILE dnum,NINT lokmod);
extern  COUNT        UDLOCK81(pLhw LONG recbyt,pCTFILE dnum);
extern  LONG         ieqlkey(pCTFILE knum,pTEXT target);
extern  LONG         igtekey(pCTFILE knum,pTEXT target,pTEXT idxval);
extern  LONG         ilstkey(pCTFILE knum,pTEXT idxval);
extern  LONG         igtkey(pCTFILE knum,pTEXT target,pTEXT idxval);
extern  NINT         ctscnexc(pTREBUF buffer,NINT elm,TEXT forceCheckMarks pthHan);
extern  NINT         ctrevexc(pTREBUF buffer,NINT elm,TEXT forceCheckMarks pthHan);
extern  LONG         fndkey(PFAST pCTFILE knum,pTEXT target,TEXT stratg,pTEXT idxval,IDXSTATE * idxstate);
extern  COUNT        setset(FILNO keyno,pTEXT target,PFAST COUNT siglen);
extern  COUNT        igetaltseq(FILNO keyno,pCOUNT altseq,COUNT srvrmode);
extern  COUNT        hdrupd(pCTFILE knum,LONG chgnum);
extern  COUNT        ctmrkexc81(pTREBUF buffer,NINT elm,UINT adflag,NINT shflag,pLhwt LONG trbf,pCMACTN pcmactn);
extern  VOID         ctrstexc(pCTFILE ctnum,pTREBUF buf);
#ifndef ctCLIENT
extern  LONG         ctabtexc81(pPLhw NINT mode,NINT keyno,pLhw LONG node,pLhwt LONG tran,pPLhwt pLONG ptran,NINT num,pTREBUF buffer,pABTLST pap pthHan);
#endif
extern  UINT         ctabhash(pLhw LONG node);
extern  VOID         ctabtaddtrn(pCTFILE knum, pABTLST nap);
extern  COUNT        ctabtnod(pCTFILE ctnum, NINT amt);
extern  COUNT        ctclup(NINT sOWNR,pTREBUF buffer,NINT tranmode,NINT shrink);
extern  COUNT        putnod(PFAST pTREBUF buf,PFAST COUNT nodsiz,NINT hash,NINT tryulk pthHan);
extern  VOID         prpdup81(pTEXT ip, const struct keyOpInput * input);
extern  pTREBUF      movrgt(pTEXT idxval,PFAST pCTFILE knum,PFAST pTREBUF buffer,NINT adflag,IDXSTATE * idxstate);
extern  COUNT        ideladd81(pCTFILE dnum, const struct keyOpInput * delInput, const CTKEYREF* KeyToAdd, COUNT relkey, NINT cf, NINT cont, KEYSTATE* keystate);
extern  NINT         insexp81(PFAST pTREBUF bp,pTEXT ip, ULONG8 pntr,NINT distinct,NINT mrkclen,IDXSTATE * idxstate);
extern  COUNT        delexp(PFAST pTREBUF bp,NINT distinct);
extern  LONG         inewvrec(pCTFILE vnum,FILNO ufil,VRLEN varlen,VRLEN falgn,NINT lnkflg,NINT cntflg,NINT serflg);
extern  COUNT        iretvrec(SYSNO datno,FILNO ufil,LONG recbyt,NINT pmode,NINT coalback);
#ifdef ctCMPREC
extern  NINT         ctCMPREChndl(pCMPRECRES pres pthHan);
extern	VOID         retCMPRECres(pCTFILE dnum pthHan);
extern  NINT         getCMPRECres(FILNO ufilno,pCTFILE dnum pthHan);
extern  NINT         setCMPRECres(FILNO ufilno,pCTFILE dnum pthHan);
extern  NINT         iGTVLEN(pCTFILE vnum,FILNO datno,ctRECPT hw,ctRECPT recbyt,pVHDR pvrhdr,pVRLEN pactlen,pNINT pcmpflg,pNINT pflxdat,NINT conv pthHan);
extern  COUNT        getvhdr81x(PFAST pCTFILE vnum,pLhw LONG pntr,pVHDR phdr,pNINT psizhdr,pVRLEN pactlen,pVRLEN pflxdat pthHan);
extern  pCMPRECFNC   ctCMPRECinit(NINT comptype,NINT compvrsn,pTEXT dllname,pTEXT attrstr,VRLEN attrlen pthHan);
extern  pVOID        ctRLE_CmpInit(pCMPRECFNC pfnc);
extern  pVOID        ctRLE_ExpInit(pCMPRECFNC pfnc);
#endif
extern  COUNT        getvhdr81(PFAST pCTFILE vnum,pLhw LONG pntr,pVHDR phdr pthHan);
extern  COUNT        putvhdr81(PFAST pCTFILE vnum,pLhw LONG pntr,pVHDR phdr pthHan);
extern  COUNT        frmlkey(pCTFILE ctnum,pTEXT pkeybuf,pTEXT plen,ctRECPT pntr);
extern  NINT         chkvhdr(PFAST pVHDR phdr);
#ifndef ctCLIENT
extern  VRLEN        prprdv81(SYSNO datno,pRhw ctRECPT recbyt,pTEXT recptr,VRLEN bufsiz,VRLEN orgbuf,NINT redmod,pVRLEN purclen,pVRLEN pactlen,pNINT pcmpflg,pVRLEN pflxdat,ppTEXT ppcnvrec pthHan);
#endif
extern  LONG         chkvsm(FILNO datno);
extern  COUNT        ctVERIFY(FILNO keyno,pLONG pcnt);
extern  NINT         leaf_extra(FILNO keyno,FILNO datno);
extern  NINT         ctseek81(RNDFILE cfd,pLhw LONG recbyt pthHan);
extern  NINT         ctswitch(pCTFILE ctnum);
extern  COUNT        ictio81y(NINT op_code,pCTFILE ctnum,pLhw LONG recbyt,pVOID bufadr,VRLEN iosize,pLONG pretsiz,VRLEN torigin,pLONG moghgh pthHan pioN);
extern  COUNT        ictio81x(NINT op_code,pCTFILE ctnum,pLhw LONG recbyt,pVOID bufadr,VRLEN iosize,pLONG pretsiz,VRLEN torigin pthHan pioN);
extern  COUNT        ictio81(NINT op_code,pCTFILE ctnum,pLhw LONG recbyt,pVOID bufadr,VRLEN iosize,pLONG pretsiz pthHan pioN);
extern  COUNT        ictio(NINT op_code,pCTFILE ctnum,LONG recbyt,pVOID bufadr,VRLEN iosize,pLONG pretsiz pthHan pioN);
extern  COUNT        ctsysio81(COUNT op_code,pCTFILE ctnum,NINT chl,pLhw LONG recbyt,pVOID bufadr,VRLEN iosize,pLONG pretsiz pthHan pioN);
extern  COUNT        dltfil(cpTEXT filnam);
extern  COUNT        dltfilx(cpTEXT filnam,NINT mflg,NINT options);
extern  COUNT        cleandir(pTEXT dirnam, pTEXT errMsg, VRLEN errMsgLen);
extern  NINT         cleandirrecurs(pTEXT dirnam);
extern  COUNT        cnvdltfil(pTEXT fn);
extern  COUNT        cnvdltfilx(pTEXT fn,NINT mflg,NINT options);
extern  COUNT        renfil(cpTEXT oldnam,cpTEXT newnam);
extern  COUNT        renfilx(cpTEXT oldnam,cpTEXT newnam,NINT mflg,NINT options);
#ifdef ctMIRROR
extern  COUNT        dltfilm(cpTEXT filnam);
extern  COUNT        cnvdltfilm(pTEXT fn);
extern  COUNT        renfilm(cpTEXT oldnam,cpTEXT newnam);
#endif
extern  NINT         cttcre(pNINT, NINT(*)(VOID), UINT, NLONG);
extern  NINT         cttcreXtd(pNINT, NINT(*)(VOID), UINT, NLONG, void*);
extern  NINT         ctqcre(NINT qindex,NINT qsize);
extern  NINT         ctqwrt(NINT qindex,pVOID qmsgptr,NINT qmsglen);
extern  NINT         ctqwrt2(NINT qindex,pVOID qmsgptr,NINT qmsglen,NINT mode);
extern  NINT         ctqred(NINT qindex,pVOID qmsgptr,LONG timeout);
extern  NINT         ctqfind(const NINT qindex,NINT (*)(enum QUEUE_OP *,const void *,void *,const NINT),const void * target,void * message,UINT * msgsiz);
extern NINT ctqchk(pDQMSG);
extern  NINT         ctmqwrt(NINT mode,pTEXT msg,COUNT rc);
extern  VOID         cttsksetup(VOID );
extern  NINT _lds_   ctsemclr(pSEMA sema,NINT own);
extern  NINT         io_ctsemclr(pCTFILE ctnum,NINT chl,NINT owner);
#ifdef ctDBGSEMCNT
extern  NINT         ctsemrqsCNT(pSEMA sema,LONG wait,NINT own,NINT loc);
extern  NINT         ctsemwatCNT(pSEMA sema,LONG wait,NINT own,NINT loc);
extern  NINT         io_ctsemrqs(pCTFILE ctnum,NINT chl,LONG wait,NINT sOWNR,NINT loc);
#else
extern  NINT         ctsemwat(pSEMA sema,LONG wait,NINT own);
extern  NINT _lds_   ctsemrqs(pSEMA sema,LONG wait,NINT own);
extern  NINT         io_ctsemrqs(pCTFILE ctnum,NINT chl,LONG wait,NINT owner);
#endif
extern  NINT _lds_   ctsemcls(pSEMA sema);
#ifdef ctSEMBLK
extern  NINT _lds_   ctblkclr(pSEMAblk sema,NINT own);
extern  NINT _lds_   ctblkcls(pSEMAblk sema);
#ifdef ctDBGSEMCNT
extern  NINT         ctblkrqsCNT(pSEMAblk sema,LONG wait,NINT own,NINT loc);
extern  NINT         ctblkwatCNT(pSEMAblk sema,LONG wait,NINT own,NINT loc);
#else
extern  NINT _lds_   ctblkrqs(pSEMAblk sema,LONG wait,NINT own);
extern  NINT         ctblkwat(pSEMAblk sema,LONG wait,NINT own);
#endif
#endif

#ifdef ctSEMTIM
extern  NINT _lds_   cttimclr(pSEMAtim sema,NINT own);
extern  NINT _lds_   cttimcls(pSEMAtim sema);
#ifdef ctDBGSEMCNT
extern  NINT         cttimrqsCNT(pSEMAtim sema,LONG wait,NINT own,NINT loc);
extern  NINT         cttimwatCNT(pSEMAtim sema,LONG wait,NINT own,NINT loc);
#else
extern  NINT _lds_   cttimrqs(pSEMAtim sema,LONG wait,NINT own);
extern  NINT         cttimwat(pSEMAtim sema,LONG wait,NINT own);
#endif
#endif

#ifdef ctDBGhdrrqs
#ifdef ctDBGSEMCNT
extern NINT          cthdrrqs(pSEMAmut sema,NINT own,NINT loc);
#else
extern NINT          cthdrrqs(pSEMAmut sema,NINT own);
#endif
#endif
#ifdef ctDBGSEMCNT
extern  NINT         ctmutrqsCNT(pSEMAmut sema,NINT own,NINT loc);
#endif

#ifdef ctSEMUTX
#ifndef ctMTXtoWLOCK
extern  NINT         ctmutclr(pSEMAmut sema,NINT own);
extern  NINT         ctmutcls(pSEMAmut sema);
#ifndef ctDBGSEMCNT
extern  NINT         ctmutrqs(pSEMAmut sema,NINT own);
#endif
#endif
#endif /* ctSEMUTX */
#ifdef ctDBGSEMBLK
extern  NINT         ctmutzro(pSEMAmut sema,NINT own);
#endif

#ifdef ctFeatRWLOCK
#ifdef ctDBGRWLCNT
extern  NINT         ctThrdSemapGetAll(pctSEMAP semap,NINT loc);
#else
extern  NINT         ctThrdSemapRelAll(pctSEMAP semap);
#endif
#if defined(ctDBGRWLCNT) || defined(ctDBGSEMCNT)
extern  NINT         ctrwlrqs(pRWLOCKmut rwlock,NINT loktyp,NINT own,NINT loc);
extern  NINT         ctrwltry(pRWLOCKmut rwlock,NINT loktyp,NINT own,NINT loc);
#else
extern  NINT         ctrwlrqs(pRWLOCKmut rwlock,NINT loktyp,NINT own);
extern  NINT         ctrwltry(pRWLOCKmut rwlock,NINT loktyp,NINT own);
#endif
extern  NINT         ctrwlclr(pRWLOCKmut rwlock,NINT own);
extern  NINT         ctrwlcls(pRWLOCKmut rwlock);
#endif
#ifdef ctFeatDFRIDXsrvr
#ifdef ctDBGSEMCNT
extern  NINT         ctDFRKrqs(pctRWLOCK rwlock,NINT loktyp,NINT own,NINT loc);
#else
extern  NINT         ctDFRKrqs(pctRWLOCK rwlock,NINT loktyp,NINT own);
#endif
extern  NINT         ctDFRKclr(pctRWLOCK rwlock,NINT own);
#endif

#ifdef ctSEMCOM
#ifndef CTBOUND
extern  NINT	     ctcomclr(pLQMSG ,NINT , NINT);
extern  NINT	     ctcomrqs(pLQMSG ,NINT , LONG ,NINT);
extern  NINT	     ctcomwat(pLQMSG ,NINT , LONG ,NINT);
#endif
#endif

extern  NINT         ctlclose(pCTFILE ctnum,COUNT clmode,NINT cleanup);
extern  COUNT        mbclosx(pCTFILE ctnum,RNDFILE cfd,COUNT clmode);
extern  COUNT        mbclos(pCTFILE ctnum,COUNT clmode);
#ifdef ctMIRROR
extern  NINT         ctswtmir(pCTFILE ctnum,NINT err);
extern  COUNT        ctmirhdr81(pCTFILE ctnum,pRhw ctRECPT mhdrpos);
extern  NINT         ctmname(pTEXT fn,ppTEXT mp);
extern  NINT         ctmalert(pTEXT pn,pTEXT mn,NINT err);
extern  COUNT        ctmopen(pCTFILE ctnum,UCOUNT filmod,NINT cleanup);
extern  RNDFILE      ctopen(pCTFILE ctnum,UCOUNT filmod);
extern  COUNT        ctmcreate(pCTFILE ctnum,NINT idxflag,UCOUNT filmod);
#ifdef MULTITRD
extern  VOID         ctfrcmir(pCTFILE ctnum,NINT sOWNR);
#else
extern  VOID         ctfrcmir(pCTFILE ctnum);
#endif
#endif
extern  RNDFILE      mbopen(pCTFILE ctnum,pTEXT fn,ULONG opmode);
extern  RNDFILE      mbcratx(pCTFILE ctnum,pTEXT filnam,UINT opmode);
extern  RNDFILE      mbcrat(pCTFILE ctnum,UINT opmode);
extern  RNDFILE      ctlcreate(pCTFILE ctnum,pTEXT filnam,NINT idxflag,UINT filmod pthHan);
extern  COUNT        mbsave(pCTFILE ctnum);
#ifndef ctsave
extern  COUNT        ctsave(pCTFILE ctnum);
#endif
extern  VOID         flushdos(FILNO datno);
extern  FILNO        filrng(FILNO filno,NINT nm,UCOUNT filmod);
extern  FILNO        filrngnam(NINT filno,NINT nm,UINT filmod, UINT imode,pTEXT fn);
extern  COUNT        setimap(NINT i,pIFIL ip,NINT keyno,NINT exsidx,NINT creflg);
extern  COUNT        setimapx(NINT i,pIFIL ip,NINT keyno,NINT exsidx,NINT creflg,NINT ikeyno);
#ifdef BIGCHECK
extern  NINT         CTCFILL(pTEXT tp, NINT ch, UINT smlen);
#endif

#ifdef MULTITRD
extern  COUNT        INTREEz(pLOGBLK plog,pLQMSG lqp);
extern  COUNT        INTISAMz(pLOGBLK plog,pLQMSG lqp);
extern  COUNT        CREISAMz(pISAMBLK pisam,pLQMSG lqp);
extern  COUNT        OPNISAMz(pISAMBLK pisam,pLQMSG lqp);

extern  VOID         cttsus(LONG tid);
extern  NINT         ctloglog(NINT sOWNR,cpTEXT userid,NINT mode);
extern  NINT         ctclrsrvr(pLQMSG lqp);
extern  NINT         ctqcls(NINT qindex);
extern  NINT         ctqxst(NINT qindex);
extern  NINT         ctqxstd(NINT qindex);
extern  NINT         PIFlush(NINT sOWNR,VRLEN iosize);
extern  COUNT        ctugfil(NINT t);
extern  VOID         ctclsugfil(NINT t);
extern  COUNT        ctsetsec(pCTFILE ctnum,pCRESEC pcresec,FILNO filno,LONG respos,pTEXT userid);
extern  COUNT        ctchksec(pCTFILE ctnum,pTEXT fileword,FILNO filno,NINT admflg);
extern  VOID         killall(NINT sOWNR);
extern  VOID         ctsetuginfo(pLQMSG lqp);
extern  NINT         ctismem(VRLEN iosize,NINT sOWNR);
extern  NINT         ctmemavl(NINT sOWNR,VRLEN iosize,NINT list,NINT syslmt,NINT usrlmt,NINT mattr,NINT force,pLSTANC la);
extern  NINT         blkcmtint(NINT sOWNR,NINT mode,COUNT lokmod);
extern  NINT	     RequestNodePruning(const CTFILE * knum,const NINT sOWNR);
#ifndef CTBOUND
extern  NINT         ctntio(COUNT func,pTEXT mp,pVAB pvab,pLQMSG lqp);
extern  NINT         ctlogoff(NINT sOWNR,pTEXT userid);
#endif
extern  VOID         setcommbuf(pULONG memchg,pLQMSG lqp);
extern  VOID         ctdec(pTEXT dp,pTEXT sp,NINT len,LONG k1,LONG k2);
extern  VOID         ctdeck(pTEXT dp,pTEXT sp,NINT len,LONG k);
extern  NINT         ctblksrvr(pLQMSG  lqp);
extern  COUNT        srvrwait(NINT sOWNR,pLQMSG l,LONG delay,UCOUNT noMem);
#ifndef ctPortWINKE
extern  VOID         initctsrvr(VOID);
extern  VOID         startctsrvr(VOID);
extern  VOID         stopctsrvr(VOID);
extern  VOID         srvrcopyright(cpTEXT legend);
extern  VOID         srvrflashmsg(cpTEXT legend);
#endif
extern  VOID         ctenc(pTEXT dp,pTEXT sp,NINT len,LONG k1,LONG k2);
extern  NINT         ctclrclnt(pLQMSG lqp);
extern  NINT         ctkilblk(NINT ,NINT );

#ifdef ___FNPglobals___
extern  Thread_     *makeThread(ProcPtr thread,int32 size,int32 refcon,int32 priority,int32 freq,int32 narg,VOID *parm);
extern  byte        *memory(int32 nbr, int32 size);
extern  VOID         delayDefer(LONG len);
extern  VOID        *threadInit(VOID *term);
extern  VOID         killThread(Thread_ *t);
extern  int32        getThdTime(VOID);
extern  Thread_     *myThread(VOID);
extern  VOID         sleepThread(Thread_ *t, int32 max);
extern  VOID         DEFER(VOID);
#endif /* ___FNPglobals___ */
#ifdef MULTITRD
extern  VOID	     ctchgpre(pCTFILE ctnum,NINT domembers);
#endif
#endif /* MULTITRD */

extern  NINT         ctcnvfn(pTEXT fn, size_t bufsize);
extern  NINT         ctcnvmn(pTEXT fn, size_t bufsize);
extern  NINT         ictgetgvarsize(NINT locgvsize);
extern  VOID         ctidxfrsh(FILNO keyno);
extern  COUNT        chkidxhdr(NINT,pIIDX,FILNO,COUNT,COUNT);
extern  NINT 	     isfcr(pRESHDR presblk,NINT hugeflg);

#ifdef TRANPROC
#ifdef iSYNC_LOG
extern COUNT ctsync(pCTFILE	ctnum,NINT mode);
extern COUNT ctsyncx(RNDFILE fd,pTEXT filnam,pLONG ptflmod2,NINT mode);
#endif
#endif

NINT   ctfdatasync(RNDFILE myfd);

#ifdef ctFRCSNG
#ifdef ctPOSTFNC
extern  COUNT        intfrmkey(FILNO keyno,pTEXT recptr,pTEXT txt,LONG pntr,VRLEN datlen);
extern  COUNT        intfrmkeyx(FILNO keyno,pTEXT recptr,pTEXT txt,LONG pntr,VRLEN datlen,NINT mode);
extern  COUNT        intifrmkey(FILNO keyno,FILNO datno,pCTFILE knum,pCTFILE dnum,pTEXT recptr,pTEXT txt,UINT txtlen,LONG pntr,VRLEN datlen);
extern  pTEXT        intTFRMKEY(FILNO keyno,pVOID tarptr);
extern  pTEXT        intlTFRMKEY(FILNO keyno,pVOID tarptr);
extern  COUNT        intTSTFILNUM(FILNO filno);
#endif
#endif

#ifdef ctWATCH_THRD_START_STOP
extern  NINT         ctWatchThrdStartStop(NINT Mode, NINT sOWNR, NINT (*taskptr)(VOID));
#endif

extern  void         ctSETupdflg(UINT updLoc,pCTFILE ctnum,TEXT flg);
#ifdef ctSRVR
#ifdef ctPortWINCE_SRVR_SERVICE
__declspec(dllexport)
#endif
extern  NINT         Faircom_Server(NINT argc,pTEXT argv[]);
#endif
#ifdef ctBEHAV_SHTLOOP
extern  VOID         ct_rspshtdwnsig(NINT sig);
extern  VOID         ct_setshtdwnsig(VOID );
#endif
extern  int          ctGetOSVersion(void);
extern  NINT         ctcompress(pVOID source, VRLEN sourceLen, pVOID dest, pVRLEN destLen);
extern  NINT         ctdecompress(pVOID source, VRLEN sourceLen, pVOID dest, pVRLEN destLen);
extern  NINT         ctcompress2(pCTFILE dnum,pVOID source, VRLEN sourceLen, pVOID dest, pVRLEN destLen);
extern  NINT         ctdecompress2(pCTFILE dnum,pVOID source, VRLEN sourceLen, pVOID dest, pVRLEN destLen);
extern  LONG         ghat(pTEXT retval);
extern  VOID         setctDEVKEY(NINT newvalue);
extern  NINT         ctsrvset(pCTINIT1 pc,NINT init);
extern  NINT         ctcmd(pCTINIT1 pc,NINT argc,ppTEXT argv,NINT init);
extern  NINT         ctcfg(pCTINIT1 pc,NINT init);
extern  NINT         ct_tstxclbits(ULONG mode,ULONG options);
extern  VOID         ctdmpstack(NINT loc);
extern  VOID         ctdmpstackxtd(NINT loc,NINT * logused,NINT options);
extern  NINT         ctWINchdir(pTEXT path);
#ifdef ctPortWINKE
extern  VOID         ctResetOWNER(NINT sOWNR);
extern  VOID         makeOWNER(NINT sOWNR);
#ifdef ctPortWINKEfltdrvr
extern  VOID         ctSetDriverOptions(PDRIVER_OBJECT pdo, PFLT_FILTER pflt, PFLT_INSTANCE pfltinst, char *basedir);
#else
extern  VOID         ctSetDriverOptions(PDRIVER_OBJECT pdo, char *basedir);
#endif
#ifdef ctPortWINKEcrt
extern	FILE *       ctrt_fopen(const char *filename, const char *mode);
extern	size_t       ctrt_fread(void *buffer, size_t size, size_t count, FILE *stream);
extern	size_t       ctrt_fwrite(void *buffer, size_t size, size_t count, FILE *stream);
extern	char *       ctrt_fgets(char *string, int num, FILE *stream);
extern	long         ctrt_ftell(FILE *stream);
extern  int          ctrt_fseek(FILE *stream, long offset, int origin);
extern	int          ctrt_fscanf(FILE *stream, const char *fmt, ...);
extern	int          ctrt_fprintf(FILE *stream, const char *fmt, ...);
extern	int          ctrt_fflush(FILE *stream);
extern	int          ctrt_fclose(FILE *stream);
extern	int          ctrt_scanf(const char *buf, ...);
extern	char *       ctrt_tmpnam(char *buf);
extern	double       ctrt_atof(const char *string);
extern	double       ctrt_ceil(double x);
extern	double       ctrt_floor(double x);
extern	long         ctrt_strtol(const char *nptr, char **endptr, int base);
extern	double       ctrt_fmod(double x, double y);
extern	double       ctrt_modf(double x, double *y);
#endif
extern	pTEXT	     ctrt_alloc(NINT n, UINT s);
extern	pTEXT	     ctrt_alloc_np(NINT n, UINT s);
extern	NINT	     ctptolstadd(UNLONG pid, UNLONG tid, NINT cid);
extern	NINT	     ctptolstdelbypid(UNLONG pid);
extern	NINT	     ctptolstdelbypidtid(UNLONG pid, UNLONG tid);
#endif
#if defined(ctPortSTI_STANDALONE) && !defined(ctUSE_TEMPENV)
extern  pTEXT        ctrt_tmpnam( pTEXT tmp );
#endif
extern  ctCONV NINT  ctDECL ctFileCreateCallback(FILNO filno, pCTFILE ctnum pthHan);
extern  ctCONV NINT  ctDECL ctFileOpenCallback(FILNO filno, pCTFILE ctnum pthHan);
extern  ctCONV NINT  ctDECL ctFileCloseCallback(FILNO filno, pCTFILE ctnum pthHan);
extern  COUNT        addconbuf(pCTFILE dnum, pIFIL ifilptr);
extern  COUNT        dltconbuf(pCTFILE dnum, FILNO datno, FILNO dltkeyno, NINT nmem);
#ifdef TRANPROC
extern  NINT         chkismtrnupd(FILNO datno, NINT sOWNR, pNINT pUpdated);
#endif
extern	VOID         ctlogusrcnt(NINT sOWNR, pCTFILE ctnum, NINT chg, pTEXT desc);
extern	VOID         ctprtusrcnt(VOID);
extern	VOID         ctaddsemlst(pVOID sema, NINT objtyp);
extern	VOID         ctdelsemlst(pVOID sema, NINT objtyp);
extern  VOID         ctdmpsemlst(NINT sOWNR);
#ifdef ctSRVR
extern  VOID         ntKillAccept(pSrvrGbls pGlobals, pTEXT svnam);
#endif
extern  int          ct_usleep(LONG  usec);
extern  NINT         stopsrvrchk(VOID);
extern  VOID         ThreadSetup(VOID);
extern  VOID         ctInitAdmFils(pAdmFils f, NINT i);
extern 	COUNT	     ctOpnAdmFils(pTEXT fn,NINT i,NINT sOWNR);
extern 	VOID	     ctClsAdmFils(NINT i,NINT sOWNR);
extern 	COUNT	     ctOpnAdmFilsXtd(pTEXT fn, pAdmFils f, NINT sOWNR);
extern 	VOID	     ctClsAdmFilsXtd(pAdmFils f, NINT sOWNR);
extern  VOID         ctSetThreadStackSize(VOID);
extern  NINT         ctdbgfcbio_filmtc(pTEXT filnam);
extern  VOID         ctbpf(VOID);
extern  VOID         ctbpfx(NINT level);
extern  VOID         ctdbgdelay(NINT level,UINT reltim,NINT freq);
extern  VOID         ctbpfDATPOS(VOID);
extern  VOID         ctbpfNOdmpstack(VOID);
#ifdef ctUNICODE
extern  pVOID        ctSQLSetUCollator (pctKSEGDEF ksgdef);
extern  pVOID        ctSQLGetUCollator(VOID);
extern  VOID         ctSQLPutUCollator(VOID);
#endif
#ifdef ctFeatGNSEMAhsh
extern  LONG         cthshval81(pLhw LONG node,UINT kn);
#endif
extern  LONG         ctdshval81(UINT fn, pLhw LONG bb);
extern  ULONG        ipaddrtolng(cpTEXT ipaddr);
extern  NINT         readfilenames(pFILE ifp,pTEXT filnam1,pTEXT filnam2);
extern  NINT         stripwhitespace(pTEXT tp);
extern  NINT         cttrntimoutstop(VOID);
extern  NINT         ctCPU_LIMITstop(VOID);
extern	NINT	     ctBroadcastStop(VOID);
extern  NINT         ctREQUEST_TIME_MONITORstop(NINT shtdwn, NINT new_interval);
extern  NINT         ctDISTcountsStop(VOID );
extern  void         become_daemon();
extern  NINT         ctGetCPU_AFFINITY(pNINT pnumcpus, NINT mode, pNINT psyserr);
extern  NINT         ctSetCPU_AFFINITY(cpTEXT cpulist, pNINT psyserr);
#ifdef ctPortWIN32
extern  VOID         ctShowBalloonTipMessage(LPTSTR title, LPTSTR message, NINT msgtype, NINT timeout);
#endif
#ifdef ctFeatL10_handshake /* ctSIBLING_HANDSHAKE */
extern  NINT         lcbytmatches(UINT lcbyt,ULONG lcbytatr, TEXT SQL);
#else
extern  NINT         lcbytmatches(UINT lcbyt,ULONG lcbytatr);
#endif
#ifdef ctBEHAV_STRICTOEM_LOGONu_CS_VERSION_CHECK
extern  NINT         ctVersionBuildIdChkMatch(pCVBLK cvblk);
#endif
#ifdef ctFEAT_OEM_CUSTOM_HANDSHAKE
extern  NINT         lcbytCUSTOM_OEMmatches(UINT lcbyt, ULONG lcbytatr, NINT itMatches);
#endif
extern  UINT         isdevsrv(NINT srltype);
#ifndef ctFeatL10_handshake /* ctSIBLING_HANDSHAKE */
extern  NINT         chkdevprefix(UINT bytsrv, UINT lcbyt);
#endif
extern  int          ctrt_strerror_r(int errnum, char *buffer, size_t bufsize);
extern  VOID         ctinitclnver(NINT sOWNR);
extern  VOID         ictGetFileNameFromFCB(pCTFILE ctnum, pTEXT nambuf, NINT namlen);
extern	VOID         preprocessconfig(VOID);
extern	VOID         resetkwds(VOID);
extern  NINT         ctImpersonateTaskCleanup(NINT xchgcomm);
extern  VOID         ctExchangeUserCommVars(NINT ownr);
extern  VOID         ctGetWindowsErrorMsg(NINT errcod, pTEXT syserr, NINT bufsiz);
extern  NINT         ctUserPreFnc(NINT sOWNR, LONG mode);
extern  VOID         ctUserPostFnc(NINT sOWNR);
extern  NINT	     ctSETCFGBoolean(const TEXT * value,NINT * option);
extern  NINT	     ctSETCFGInteger(const TEXT * value,NINT * option);
extern  NINT	     ctSETCFGIntegerUnsigned(const TEXT * value,UINT * option);
extern  NINT         ctSETCFGdyndmpdfr(cpTEXT value);
extern  NINT         ctSETCFGdyndmpdfrint(cpTEXT value);
extern  NINT         ctSETCFGmaxfilewaitsecs(cpTEXT value);
extern  NINT         ctSETCFGmaxdfridxlogs(cpTEXT value);
extern  NINT         ctSETCFGmaxrepllogs(cpTEXT value);
extern  NINT	     ctSETCFG_STACK_DUMP(cpTEXT value);
extern  NINT	     ctSETCFG_SORT_MEMORY(cpTEXT value,NINT logerror);
extern  NINT	     ctSETCFG_MAX_HANDLES(cpTEXT value);
extern  NINT	     ctSETCFG_MAX_REBUILD_QUEUE(cpTEXT value,NINT logerror);
#ifdef ctGVARH
extern IDXSTATE * getUserIdxOpState(USERVAR * var);
extern void relUserIdxOpState(IDXSTATE * idxstate,USERVAR * var);
#endif
#ifdef MULTITRD
#ifdef ctGVARH
extern  void         ctGetSerialNbrFromRecordImage(const CTFILE* dnum, const char* recptr, pCTGV lctgv);
extern  NINT         ctGetIdentityFromRecordImage(const CTFILE* dnum, const char* recptr, VRLEN datlen, pCTGV lctgv);
extern  NINT         ctKEEPOPENclose(pCTGV lctgv);
#endif
#endif
extern  COUNT        ctStatusLogWrite(pTEXT msg,NINT err);
extern	NINT         getcfgsubsetnum(NINT setnum,cpTEXT kv);
extern  NINT         CheckTransferFileParams(pctXFRFIL pxfr);
extern  NINT         ctRunCallbackInit(CTFILE * ctnum,NINT sOWNR);
#ifdef ctCONDIDX
extern  NINT         ctSaveFreeCondIdx(CTFILE * ctnum,pCIFIL pcifil, LONG tfilter, NINT sOWNR);
#endif
extern  NINT         ctConnGroupCheckTaskId(NINT conngrptaskid, pNINT pconndlt);
extern  NINT         ctConnGroupGetTaskId(pTEXT usrid, NINT rmvid);
extern  NINT         ctConnGroupClear(pNINT pconndlt);
extern  UINT         ctMEMFILEhash(pCTFILE ctnum, pVOID recmem);
extern  NINT         ictCreateSequenceFile(VOID);
extern  NINT         ctrt_get_strlen(pTEXT str, NINT max);
extern  NINT         ictCreateSequence(pctSEQATTR pseqattr, NINT len);
extern  NINT         ictGetSequenceAttrs(LONG seqhnd, pctSEQATTR pseqattr, pNINT plen);
extern  NINT         ctsetrcvmessage(pTEXT fmt, ...) PRINTFLIKE(1,2);
extern  VOID         ctresetrcvmessage(VOID);
extern  VOID         ctlogrcvmessage(VOID);
extern  NINT         needcmtlok(pCTFILE ctnum, ULONG recbyt, UINT redlen);
extern  NINT         ctnobuffillst(pTEXT filnam);
extern  NINT         ctnobuffcreOK(UCOUNT filmod, UCOUNT xflmod);
extern  NINT         ctnobuffopnOK(pCTFILE ctnum);
#ifdef ctFeatBLOCKCACHE
extern  pVOID        ctCacheAlloc(size_t size, NINT mode);
extern  VOID         ctCacheFree(pVOID p);
#endif
extern  NINT         datfltlstadd(UCOUNT fltnum, ppctDTFLT ppflthed, ppctDTFLT ppfltnew);
extern  NINT         datfltlstappend(ppctDTFLT ppflthed, ppctDTFLT ppfltnew);
extern  pctDTFLT     datfltlstfnd(UCOUNT fltnum, pctDTFLT pfltlsthed);
extern  NINT         datfltlstrmv(ppctDTFLT ppfltlsthed, pctDTFLT pfltlst, NINT keep);
extern  NINT         ctInitVSSwriter(VOID);
extern  NINT         ctTermVSSwriter(VOID);
#ifdef ctFeatRWLOCKwindows
#ifdef ctFeatRWLOCKctree
extern  VOID WINAPI  ctInitializeRWLock(pctRWL prwlock);
extern  VOID WINAPI  ctAcquireRWLockExclusive(pctRWL prwlock);
extern  VOID WINAPI  ctAcquireRWLockShared(pctRWL prwlock);
extern  BOOLEAN WINAPI  ctTryAcquireRWLockExclusive(pctRWL prwlock);
extern  BOOLEAN WINAPI  ctTryAcquireRWLockShared(pctRWL prwlock);
extern  VOID WINAPI  ctReleaseRWLockExclusive(pctRWL prwlock);
extern  VOID WINAPI  ctReleaseRWLockShared(pctRWL prwlock);
extern  VOID WINAPI  ctFinalizeRWLock(pctRWL prwlock);
#else
extern  VOID WINAPI  ctInitializeCS(LPCRITICAL_SECTION pcs);
extern  VOID WINAPI  ctAcquireCS(LPCRITICAL_SECTION pcs);
extern  BOOLEAN WINAPI  ctTryAcquireCS(LPCRITICAL_SECTION pcs);
extern  VOID WINAPI  ctReleaseCS(LPCRITICAL_SECTION pcs);
#endif
#endif
extern  NINT         hpinit(void);
extern  void         hpterm(void);
extern  NINT         ctChangeMasterPassword(pctENCMOD pctencmod pthHan);
extern  NINT         ctredirect(pTEXT oldbuf,pTEXT newbuf,pTEXT msgsuc,pTEXT msgerr);
extern  NINT         ctredirectadd(pTEXT kv);
extern  NINT         ctredirectifil(pCTFILE dnum pthHan);
extern  NINT         ctredirectifil_list(pTEXT fillst);
#ifdef ctPortRS6000
#ifdef ctFeatATOMICop
extern  LONG         aix_ctInterlockedCompareExchange32(AULONG *ptarget, LONG newval, LONG curval);
#ifdef ct8P
extern  AULONG8      aix_ctInterlockedCompareExchange64(AULONG8 *ptarget, LONG8 newval, LONG8 curval);
#endif
#endif
extern  long long    aixtimbas(void);
extern  long long    aixhrtime(void);
#endif
extern  NINT         store_usrtrn(NINT mode,NINT sOWNR);
extern  NINT         restore_usrtrn(NINT sOWNR);
extern  NINT         chgpartattrs(pCTFILE ctnum,FILNO ufilno,LONG hdrval,COUNT mode);
extern  NINT         part_ren(pIFILBLK pifilblk);
extern  NINT         ctfnhashadd(pTEXT filnam, NINT filnum, NINT ownr);
extern  NINT         ctfnhashget(pTEXT filnam, pNINT pfilnum, NINT ownr);
extern  NINT         ctfnhashdel(pTEXT filnam, NINT ownr);
extern  NINT         ctfnhashfre(VOID);
#ifdef MULTITRD
#ifdef ctGVARH
extern  NINT         decmsgbuf(pTEXT bufptr, VRLEN bufsiz, pCTGV lctgv);
extern  NINT         ctgncrqs(pCTGV lctgv);
extern  NINT         ctgncrel(pCTGV lctgv);
#endif
#endif
#ifdef ctFeatTRACK_CLIENT_ADDR
extern  VOID         ctConvertToLocalIP(ctUADDR_T * pclientAddr);
extern  NINT	     GetIPAddrForConnection(NINT taskid, pTEXT buf, NINT bufsiz);
#endif
extern  VOID         ctSetVERSIONID2(pULONG pver2);
extern  VOID         ctGetVERSIONID2(ULONG ver2, pULONG pmini, pULONG pbuild);
extern  VOID         ctSetVERSIONID_BASE(pULONG pbase_blddt);
extern  VOID         ctGetVERSIONID_BASE(ULONG base_blddt, pULONG pbase_blddt);
extern  VOID         ctsetalign(TEXT align);
extern  COUNT        setflicerr(NINT typ,NINT sOWNR);
extern  NINT         ctdll_getsym(pVOID dlHandle,pTEXT fncnam,ppVOID pfncptr,pTEXT errbuf,NINT errbuflen);
#ifdef ctFeatOPENSSL
extern  NINT         ctinitsslsupport(pCTSSLI pssli);
extern  NINT         cttermsslsupport(pCTSSLI pssli);
#endif
#ifdef ctFeatREMBUFADR
#ifdef ctGVARH
extern  NINT         chkremnod(pCTFILE knum, pCTGV lctgv,ULONG options, pCRNI pcrni, NINT sOWNR,NINT reqtyp);
#endif
#endif
extern  NINT         ctgetdbstats(ULONG8 *pct_dbrqs, ULONG8 *pct_dbhit);
extern  NINT         ctgetibstats(ULONG8 *pct_ibrqs, ULONG8 *pct_ibhit);
#ifdef ctDBGRCVR
extern  VOID         ctTranRecovInit(VOID);
extern  VOID ctDECLV ctTranRecovLog(pTEXT fmt, ...) PRINTFLIKE(1,2);
extern  VOID         ctTranRecovFinal(VOID);
#endif
#if defined(ctCMPREC) && defined(ctFeatZLIBdynamic)
extern  NINT         ctzlib_load(pTEXT errbuf, NINT errbuflen);
extern  NINT         ctzlib_unload(VOID);
#endif
extern  NINT         ctCheckClientInfo(pTEXT user_id, pVOID pclient_version, pVOID pclientid, NINT clientid_len);
#ifdef ctFeatRTG_API
extern  NINT         ctRTGusrMem(NINT sOWNR, NINT alloc);
extern  NINT         ctRTGprintf(cpTEXT fmt, ...) PRINTFLIKE(1,2);
extern  NINT         ctRTGsendMessage(cpTEXT msg);
#endif
#ifdef ctCAMO
#ifndef ctCLIENT
extern  NINT         setencatr(pctENCATR pencattr,FILNO datno pinHan);
extern  NINT         rstencatr(pctENCATR pencattr pinHan);
#endif
#endif
#ifdef ctFeatINTERNAL_FILE_CONTROL
extern  NINT         ifc_check_and_signal(pCTFILE ctnum,pTEXT filnam,NINT mode,pNINT psfob pinHan);
extern  NINT         ifc_wait_for_close(pNINT pdidclose pinHan);
extern  NINT         ifc_resume_threads(NINT didclose pinHan);
extern  NINT         chkfilectrlstate(volatile AULONG *pstatevar);
extern  NINT         setfileopnblk(pTEXT filnam,NINT mode pthHan);
extern  NINT         chkfileopnblk(pTEXT filnam,pNINT opnblkd pthHan);
#endif
#ifdef ctFeatSET_THREAD_NAME
extern  void         ctrtSetThreadName(cpTEXT threadname);
#endif
extern  NINT         ctSNAPSHOTmemalc(pctGMMS pgmms,VRLEN bufsiz,NINT mode pthHan);
extern  NINT         setmemlistopt(pTEXT name,NINT val);
extern  NINT         ctCopyFileChangeID(pTEXT filnam pthHan);
extern  NINT         ctCopyFileVerify(pTEXT source,pTEXT destination,pNINT psyserr,pCPYFNC clbk,pCPYSTT pcpystt);
extern  NINT         ctflushdirmeta(pTEXT filnam);
extern  NINT         ctilokrqs(NINT usrn,NINT filno,pCTFILE ctnum,NINT loktyp,NINT blocking,NINT sOWNR);
extern  NINT         ctilokrel(NINT usrn,NINT filno,pCTFILE ctnum,NINT loktyp,NINT sOWNR);
extern  NINT         chklokord(pCTFILE ctnum,NINT sOWNR);
#ifdef MULTITRD
extern  VOID         cttlkfreloks(pCTFILE dnum,pFUSR fu,NINT ufil,NINT sOWNR);
extern  VOID         ctdbgiict(pTEXT msg,NINT loc,pFUSR fu);
#endif
extern  NINT         ctssl_showerror(VOID);
extern  NINT         ctsetinactive(NINT sOWNR,NINT chkstt);
extern  NINT         ctgetinactivetime(VOID);
extern  NINT         ctgetactivestatus(NINT sOWNR);
extern  NINT         chkquantum(NINT sOWNR,pLONG premain);
extern  NINT         chkmaxactconn(NINT sOWNR,NINT usedelay);
extern  NINT         reactivateconnection(NINT sOWNR, LONG8 diff);
extern  NINT         ctkbufrefresh(FILNO datno,FILNO keyno pthHan);
extern  pTEXT        ctputask(FILNO keyno,pCTFILE knum,pTEXT keyval,NINT mode,TEXT keyind pthHan);
extern  NINT         dfrkCanDeferIdx(pCTFILE dnum,pCTFILE knum,pIIDX idxp);
extern  NINT         dfrkOpBufAlloc(ppDFRKY ppdfrky,pCTFILE dnum,FILNO datno,NINT opcode,LONG8 orecbyt,LONG8 recbyt,VRLEN oreclen,VRLEN reclen,NINT sOWNR);
extern  VOID         dfrkOpBufFree(pDFRKY pdfrky);
extern  NINT         dfrkOpBufSetRecordImage(pDFRKY pdfrky,pCTFILE dnum,pTEXT recptr,NINT cmplen,NINT flxdat,FILNO datno);
extern  NINT         dfrkOpBufSetIndexInfo(pDFRKY pdfrky,pDFRKLD pdfrkyld,NINT sOWNR);
extern  NINT         dfrkOpBufGetSize(pDFRKY pdfrky);
extern  NINT         dfrkOpBufEnqueue(pDFRKY pdfrky);
extern  VOID         dfrkClearStats(VOID);
extern  VOID         dfrkLogStats(pFILE fp);
extern  NINT         dfrkErrorCallbackWrapper(NINT errcod,pDFRKY pdfrky,pDFRST pdfrst);
extern  NINT         dfrkErrorCallback(NINT errcod,pDFKCTX pdfkctx,pDFRKY pdfrky,pDFRST pdfrst);
extern  NINT         dfrkTranThrd(pVOID p);
extern  NINT         dfrkNonTranThrd(pVOID p);
extern  COUNT        dfrkUpdateHeader(pCTFILE knum,pHWM(pFUSR,fu) ctRECPT chgnum,LONG8 recbyt);
extern  NINT         dfrOpBufAddIdxno(pDFRKY pdfrky,LONG sysno);
extern  NINT         dfrkOpBufGetIdxOffset(pDFRKY pdfrky);
extern  NINT         dfrkLogTranState(pLhwt LONG tranno,NINT endtype pthHan);
extern  NINT         dfrkStopThreads(VOID);
extern  NINT         dfrkIndexLoadThrd(pVOID p);
extern  NINT         dfrkStateInit(pDFKCTX pdfkctx);
extern  NINT         dfrkPerformOp(pDFKCTX pdfkctx,pDFRKY pdfrky,pRUCBSTT prucbstt);
extern  NINT         dfrkGetDeferredIndexResource(FILNO filno,pCTFILE dnum pthHan);
extern  NINT         dfrkUpdateDeferredIndexResource(FILNO datno,pTEXT filnam,pDFRKRPM pdfrkresprm pthHan);
extern  void         dfrkFreeDeferredIndexResource(pCTFILE dnum pthHan);
#ifdef MULTITRD
extern  NINT         dfrkAllocPendingIndexUserInfo(pCTFILE dnum,pFUSR fu);
extern  NINT         dfrkCheckForNewIndex(pCTFILE dnum,FILNO datno,NINT sOWNR);
extern  NINT         dfrkUpdateUserDeferredIndexInfo(pCTFILE dnum,FILNO datno,NINT numpndidx,pIIDX ppndidx,FILNO keyno pthHan);
extern  NINT         dfrkDeferOpForUnopenedIndex(pCTFILE ctnum,pFUSR fu,ppDFRKY ppdfrky,pDFRKY pdfrky,NINT sOWNR);
extern  NINT         dfrkSaveStartupLogPos(void);
#endif
extern  NINT	     LogReaderInitFromIni(const TEXT * name, NINT allowCurrent, pDFKCTX pdfkctx);
#ifdef ctFeatONLINE_REBUILD
extern  NINT	     RebuildSettfllst(pctCNXH pcnxhnd,ULONG dattfl,ULONG idxtfl,NINT relno);
#endif
extern  NINT         ctGetLinuxKernelVersion(pNINT pmaj,pNINT pmin,pTEXT errbuf,NINT buflen);
extern  NINT         ctGetRecordUpdateCallbackResource(FILNO filno,pCTFILE dnum pthHan);
extern  VOID         ctFreeRecordUpdateCallbackResource(pCTFILE dnum pthHan);
extern  NINT         rucbInvokeFileCallbacks(NINT callbk,pCTFILE ctnum,FILNO datno,pRUCBLST ptgtrucb,NINT i pthHan);
extern  NINT         rucbInvokeRecordCallbacks(NINT callbk, pCTFILE ctnum, FILNO datno, pHWM(pFUSR*, pfu) pDFRKY pdfrky, pNINT pnqueuedcb, pRUCBSTT prucbstt pthHan);
extern  VOID         rucbusropen(pCTFILE dnum,FILNO datno pthHan);
extern  VOID         rucbusrclose(pCTFILE dnum,FILNO datno pthHan);
extern  NINT         rucbGetStateVars(pRUCBSTT prucbstt, LONG8 tranno, pTEXT recbuf, VRLEN reclen);
#ifdef ctSRVR
extern  NINT         clicbInit(NINT sOWNR,pLQMSG lqp,pLQMSG2 plqmsg2,preqPARMB pbp,prspPARMB pqb);
extern  NINT         clicbCallback(pCCBST pccbst,NINT sOWNR,pVOID pdata,NINT datalen,NINT opcode,NINT sendnow);
extern  NINT         clicbFinal(NINT sOWNR);
#endif
extern  NINT         ctembagent_main(NINT sOWNR, NINT isEmbed);
extern  NINT         ctembagentstop();
extern  VOID         set_cmtdly_min(NINT usec);
extern  NINT	     get_tranac_cnt(VOID);
extern  NINT         cthttpd_main(NINT sOWNR);
extern  NINT         cthttpdstop(VOID);
extern  NINT         ctSSLthrdinit(VOID);
extern  NINT         ctSSLthrdterm(VOID);
extern  NINT         ctSSLgetsrvcertfile(cpTEXT *ppsrvcertfile);
extern  NINT         ctSSLgetclientcertinfo(ppTEXT,ppTEXT,ppTEXT);
extern  NINT         ctSSLgetdbglog(pTEXT logNameBuffer, size_t logNameBufferLength);
extern  NINT	     ctSSLinitDebugLogMutex(void);
extern  void	     ctSSLcloseDebugLogMutex(void);
extern  NINT	     ctSSLlockDebugLog(void);
extern  void	     ctSSLunlockDebugLog(void);
extern  void	     getctime(char *datebuf, size_t datebuflen);
#if defined(ctGVARH) && (defined(ctNOGLOBALS) || defined(ctFeatSRVDLL))
extern  VOID         ctFreeWNGV(pCTGVAR pc);
extern  void         clearsslpassphrase(pCTGVAR pc);
#endif
extern  NINT         FTS_OpenFileCallback(pRUCBF prucbf);
extern  NINT         FTS_CloseFileCallback(pRUCBF prucbf);
extern  NINT         FTS_RecordUpdateCallback(pRUCBF prucbf,pRUCBO pdfrrec);
extern  NINT         FTgetctxsize(VOID);
extern  NINT         updatectreeusergroups(pTEXT pusrid, NINT numldapgroups, pTEXT pldapgroups);
extern  NINT         ictGetOpenFiles(COUNT ConnectionNumber, NINT FileInfoType, pVOID pFileInfo, pLONG pNbrFiles);
extern  NINT         ictGetFileUsers(pTEXT FileName, pctUSRINF pFileUserInfo, pLONG pNbrUsers, TEXT chkadmn);
extern  NINT         ictGetFileLocks(pTEXT FileName, NINT options, pctLOKINF pFileLockInfo, pLONG pNbrLocks);
extern COUNT         ifrmkey(FILNO keyno,FILNO datno,pCTFILE knum,pCTFILE dnum,pTEXT recptr,pTEXT txt,UINT txtlen,LONG pntr,VRLEN datlen);
extern pTEXT         ii2TFRMKEY(FILNO keyno,FILNO datno,pCTFILE knum,pCTFILE dnum,pVOID tarptr,LONG pntr,NINT usepntr,NINT unctry,NINT atf);
extern  NINT         getidxres(FILNO datno,FILNO keyno,pTEXT idxname,pTEXT fileword,pTEXT resname,pFILNO presno,COUNT numemb,pUINT pxflmod,pctENCATR pencattr,pctIDXATR pidxattr,TEXT already_open pthHan);
extern  NINT         putidxres(FILNO keyno,FILNO resno pthHan);
extern  NINT         ctmakexclusive(pCTFILE ctnum,FILNO filno,NINT sOWNR);
extern  NINT         chkcamoblk(ULONG pagesize,pTEXT errbuf,NINT errbuflen);
#ifdef ctFeatWAITFORopt
extern  NINT         ct_wfGET(NINT waiter,NINT blocker);
extern  VOID         ct_wfINC(NINT waiter,NINT blocker);
extern  VOID         ct_wfDEC(NINT waiter,NINT blocker);
#endif
extern  NINT         ctinitcmprec(pCTFILE dnum,FILNO datno pthHan);
extern  NINT         ctStandardizeLibName(cpTEXT libnam, TEXT always, pTEXT outnam, size_t outlen);
extern  NINT         flxrConvertRecordToCurrentSchema(pCTFILE vnum,LONG hw,LONG recbyt,LONG hdrsiz,LONG padsiz,FLXDAT flxdat,pVRLEN pactlen,ppTEXT pconvbuf pthHan);
extern  NINT         flxrConvertRecordToNextSchema(pCTFILE vnum, cpTEXT recptr, TEXT byteswap, VRLEN reclen, pRCDSCF prcdsc, pTEXT poutrec, pVRLEN pconvlen);
extern  NINT         flxrerr(NINT rc);
extern  NINT         getFLEXRECres(FILNO ufilno,pCTFILE dnum pthHan);
extern  NINT         setFLEXRECres(FILNO ufilno,pCTFILE dnum pthHan);
extern  VOID         putFLEXRECres(pCTFILE dnum pthHan);
extern  NINT         ctgetrmd(pCTFILE vnum,pctRMD prmd);
extern  NINT         chkschsrl(pIFIL ifilptr,FILNO filno,UCOUNT savpos,COUNT savlen,pNINT phsrlsch,pFILNO pkeyno);
/*********************************************************************/
/* singly-linked list functions */
extern  NINT         ctslstcre(ppCTSLST pplist);
extern  NINT         ctslstadd(pCTSLST plist,pVOID pele,NINT esiz);
extern  NINT         ctslstfrs(pCTSLST plist,ppVOID ppele);
extern  NINT         ctslstnxt(pCTSLST plist,ppVOID ppele);
/* ctslst element compare function */
typedef NINT (* pctslstcmpfnc_t)(pVOID ptarget,pVOID pcur);
extern  NINT         ctslstdel(pCTSLST plist,pVOID ptarget,pctslstcmpfnc_t pcmpfnc);
extern  NINT         ctslstfre(pCTSLST plist);
/*********************************************************************/
extern  NINT         mfpGetUniqueFileId(RNDFILE fd,pCTFILE ctnum);
extern  NINT         cptAlterSchema(pVOID prcdsc, NINT bytord, NINT outputVersion, ppTEXT poutbuf, pULONG pmsglen);
extern  NINT         makAlterSchema(cpTEXT inbuf, VRLEN inlen, ppTEXT poutbuf);
extern  void         freAlterSchema(pVOID buf);
extern  NINT         cpt_ctTruncateFile(pctTRUNCATE_FILE_REQUEST pTruncateFileRequest,NINT bytord,ppTEXT poutbuf,pULONG pmsglen);
extern  LONG         TRANSYS(LONG subtype,LONG filno,LONG poshw,LONG poslw,LONG attribute,pVOID buffer,VRLEN bufsiz);
extern  NINT         cptPTADMIN(pctPTAPM pptaprm,NINT bytord,ppTEXT poutbuf,pULONG pmsglen);
extern  NINT         cptksegdef(pctKSEGDEFsep pkdefsep,NINT bytord,ppTEXT poutbuf,pNINT poutlen);
extern  NINT         makksegdef(pTEXT inbuf,VRLEN inlen,pNINT pnsegdef,ppctKSEGDEF *poutbuf);
extern  NINT         unmarshalIndexConditions(cpTEXT inbuf, ULONG inlen, cpTEXT** pIndexConditions, ULONG* pNumberOfIndexConditions);

extern  pVOID        ctOpenUCollator(pctKSEGDEF);
extern  VOID         ctCloseUCollator(pVOID);

#ifdef ctFeatRP_AUTO
extern  void         ctsubsysARP_clear(void);
extern  void         ctsubsysARP_get(pctSUARP parp);
#endif
extern  NINT         change_subsystem_config(pTEXT kw,pTEXT value);

extern  NINT         ctKeepOpenLockAddRef(pCTFILE ctnum,NINT opcode,pNINT palreadyopen);
extern  NINT         ctKeepOpenLockDelRef(pCTFILE ctnum,pNINT pcanclose);
extern  NINT         ctKeepOpenLockSerl(void);
extern  NINT         ctKeepOpenLockUnserl(void);

extern  NINT         ctserl_lok(NINT loc);
extern  NINT         ctunserl_lok(void);

extern  NINT         clrbufupd(pTREBUF buf pthHan);
extern  VOID         clrpag(pDATBUF db pthHan);
extern  NINT         ctTranReplDiagLogTran(NINT func,NINT loc,NINT merr,NINT lerr pthHan);
extern  VOID         ctDiskFullActionFree(VOID);
extern  VOID         rasttfre(VOID);
extern  NINT         clfchk(pTEXT filespec,pNINT pinlist pthHan);
extern  NINT         clfget(pTEXT outbuf,pVRLEN poutlen pthHan);
extern  NINT         clffre(VOID);
extern  NINT         ctSETCFG_SQLTraceCtreeError(const TEXT * value);
extern  NINT         ctSETCFGchecklockfile(pTEXT value);
extern  NINT         ctSETCFGatotrncre(cpTEXT value);
extern  NINT         ctSETCFGunckcomp(cpTEXT value);
extern  NINT         ctSETCFGfscompressuncseg(cpTEXT value);
extern  NINT         ctSETCFGshmmaxspinners(cpTEXT value);
extern  NINT         ctSETCFG_SYNC_COMMIT_MAX_SPINNERS(cpTEXT value);
extern  NINT         ctSETCFGrcvchkdupfid(cpTEXT value);
extern  NINT         ctSETCFGchksysfid(cpTEXT value);
extern  NINT         ctSETCFGchkpmon(pTEXT value);
extern  NINT         ctSETCFGreadonlyserver(pTEXT value);
extern  NINT         ctSETCFGnontranfsflushoff(pTEXT value);
extern  NINT         ctSETCFGfilopnopt(cpTEXT value);
extern  NINT         ctSETCFGpforetrylimit(cpTEXT value);
extern  NINT         ctSETCFG_LEAF_NODE_READ_LOCK(pTEXT value);
extern  NINT         ctSETCFG_SUPPRESS_PATH_IN_IFIL(pTEXT value);
extern  NINT         ctSETCFG_REPL_NODEID(pTEXT value,TEXT persistValue);
#ifdef ctFeatPREIMAGE_SWAP
extern  NINT         ctSETCFG_MAX_PREIMAGE_DATA(pTEXT value);
extern  NINT         ctSETCFG_MAX_PREIMAGE_SWAP(pTEXT value);
#endif
#ifdef ctFeatOPEN_FILES_ALERT
extern  NINT         ctSETCFG_OPEN_FILES_ALERT_THRESHOLD(cpTEXT value);
extern  void         ctOpenFilesAlertAction(NINT NumberOfOpenFiles);
#endif
#ifdef ctFeatTRACK_CLIENT_ADDR
extern  NINT         ctSETCFG_LOCAL_CONNECTION_ONLY(pTEXT value);
#endif
extern  NINT         ctSETCFG_FIPS_ENCRYPTION(const TEXT * value);
#ifdef ctCAMO
extern  NINT	     ctgetencattr(pCTFILE ctnum, FILNO ufilno,NINT sOWNR,NINT * attr);
#endif
#ifdef ctFeatOPENSSL3
extern  NINT	     initKeyExchangeSize(void *);
#endif
extern  void	     ctLogTemplateCryptoInit(CTFILE * tnum, const CTFILE * ctnum);
extern  int          DumpSync(FILE * dump);
extern  NINT         ctGetIdFieldOffsetInRecord(FILNO datno,pTEXT recbuf,VRLEN datlen,pConvMap schema,UCOUNT reclen,NINT idfield,pNINT poffset,pNINT plen);
#if defined(ctFeatDISTismcnt) && defined(ctNUM_ISAM_COUNTERS)
extern  NINT         ctInitDistributedIsamCounters(void);
extern  void         ctIncrementDistributedIsamCounter(ctICTYP counter,NINT sOWNR);
extern  NINT         ctGetAggregateDistributedIsamCounters(pLONG8 pcounters[ctNUM_ISAM_COUNTERS]);
extern  void         ctFreeDistributedIsamCounters(void);
#endif
extern  NINT         ddnotify_restore(pVOID pctddgv,COUNT phase,COUNT errcod,LONG8 filsiz,pTEXT filnam);
extern  NINT         dfaInvokeScriptW32(pDFDEF pdfadef,LONG8 avail,pTEXT errbuf,NINT errbufsiz,pNINT psyserr);
extern  NINT         dfaInvokeScriptUNX(pDFDEF pdfadef,LONG8 avail,pTEXT errbuf,NINT errbufsiz,pNINT psyserr);
extern NINT	     InvokeScript(TEXT * command, TEXT * errbuf, NINT errbufsiz, NINT * psyserr);
extern  pTEXT        ctGetDumpRestoreEventDescription(NINT drevent);
extern  NINT         ctCheckDumpRestoreEvent(pTEXT buf,NINT len,pNINT pdrevent);
#if !defined(ctCLIENT) || defined(ctCLIENTcore)
extern  NINT         ictSetCurrentSuperfileHost(FILNO hstno pinHan);
#endif
#ifdef ctSRVR
extern  NINT         ictAssignSuperfileHost(pFUSR fu,pCTFILE ctnum pinHan);
#endif
extern  void         ctlogfpglok(pTEXT filnam,pTEXT fnc,NINT mode,LONG8 offset);
extern  LONG         remap(pCTFILE ctnum,pPLhw LONG recbyt,NINT hdrflg,NINT memb);
#ifdef ctSRVR
extern  void         fusrlstadd(pFUSR fu,NINT iict pthHan);
extern  void         fusrlstdel(pFUSR fu,NINT cls pthHan);
#endif
extern  VOID         ct_substname_init(VOID);
extern  VOID         ct_substname_term(VOID);
extern  NINT         ct_substname(pTEXT filnam,pTEXT afilnam,NINT bufsiz);
extern  NINT         ct_substnameXtd(pTEXT filnam,pTEXT afilnam,NINT bufsiz,pTEXT substDrives);
extern  NINT         ctrcvchkfilnam(NINT twonam,pTEXT fnlog,pCTFILE ctnum,pNINT pres);
extern  NINT         ctrt_rcvfilcmp(pTEXT fn1,pTEXT fn2);
#ifdef MULTITRD
extern  NINT 	     CheckUserBreakout(NINT sOWNR);
#endif
extern  int          ctIsUniprocessorSystem(void);
extern  NINT         JSNparsekeydef(cpTEXT jsnkey,ppJSNKDSW ppjsnkdsw);
extern  VOID         JSNfreekeydef(pJSNKDSW pjsnkdsw);
extern  NINT         JSNgetkeyval(pctKSEGDEF pkdef,pTEXT fld,NINT fldlen,pTEXT key,NINT keylen,NINT transform,pNINT pisnull,pNINT poutlen,pNINT psyserr);
extern  void         ctThrdQueueTerm(void);
extern  NINT         ctSetLicenseFile(pTEXT license);
extern  NINT         ctSetLocalDirectory(pTEXT localdir);

#ifdef ctGVARH

#if defined(ctFeatUNCSEG_KEYCOMPRESS) || defined(ctFeatFILESYScompressUNCSEG)
extern  NINT         chkuncseg(pIIDX idxp,pCTGV lctgv);
#else
#define              chkuncseg(idxp,lctgv)
#endif

#ifdef ctFeatFILESYScompressUNCSEG
extern  VOID         rstuncseg(pCTGV lctgv);
#else
#define              rstuncseg(lctgv)
#endif

#ifdef ctPortWINIOT
extern HMODULE ctGetKernelModule();
#endif

#ifdef ctFeatPREIMAGE_SWAP
extern  void         ctInitPreimageSwapFile(pCTGV lctgv);
extern  NINT         ctCreatePreimageSwapFile(pCTGV lctgv);
extern  NINT         ctGetPreimageSwapFile(pCTGV lctgv,pNINT psysfilno,pNINT pusrfilno);
extern  void         ctFreePreimageSwapFile(pCTGV lctgv);
#endif

#endif /* ctGVARH */

#ifdef ctDBGfcqa
extern  void         qachkkeyofflst(pTREBUF buffer,NINT nelm);
#endif
extern  void         ctupdkoffmapINS(pTREBUF buffer,NINT ipos,UCOUNT keyoff,UCOUNT keysize,pTEXT offbuf,pNINT pshftlen);
extern  NINT         kcRLE_Init(void);
extern  void         kcRLE_Exit(void);
extern  NINT         kcRLE_Compress(pVOID src,VRLEN slen,pVOID dst,pVRLEN pdlen pthHan);
extern  NINT         kcRLE_Decompress(pVOID src,VRLEN slen,pVOID dst,pVRLEN pdlen);
extern  NINT         ctGetVarintLen(pTEXT p);
extern  NINT         ctVarintLen(ULONG8 v);
extern  NINT         ctPutVarint(pTEXT p, ULONG8 v);
extern  NINT         ctGetVarint(pTEXT p, pULONG8 v);
extern  NINT         ctGetVarint32(pTEXT p, pULONG v);
extern  NINT         ctgetcmpmrk(ULONG8 tranno,NINT mark,TEXT mrkcval[10]);
extern  NINT         ctgetucmpmrk(TEXT mrkcval[10],pULONG8 ptranno);
extern  NINT         pforetry(NINT cnt,NINT fnc);
extern  void         frechkopnx(NINT sOWNR);
#ifndef ctCLIENT
extern  NINT         ctdelkeyundoadd(pTREBUF buffer,pTEXT idxval,NINT mode,LONG hw,LONG pntr,pTEXT eventbuf,IDXSTATE * idxstate pinHan);
extern  NINT         ctdelkeytrn(pTREBUF buffer,pTEXT idxval,NINT mode,pLONG padf2,pLONG padf1,IDXSTATE * idxstate pinHan);
#endif
extern  NINT         ctcmprecovRecover(pTEXT ErrorBuffer,size_t ErrorBufferLength pthHan);
extern  NINT         ctcmprecovSetTranState(LONG8 tranno,NINT undo);
extern  NINT         GETKSEGDEFwrap(FILNO keyno,COUNT segno,pctKSEGDEF *ppkdef pthHan);
extern  void         frecrd(void);
extern  void         ctcmprecovFinish(void);
extern  NINT         ctrcvlstinit(void);
extern  NINT         ctrcvlstadd(pDDMPHDR dp);
extern  void         ctrcvlstfinal(void);
extern  NINT         ctrcvopnlstinit(void);
extern  NINT         ctrcvopnlstadd(pTEXT filnam,NINT sysno);
extern  NINT         ctrcvopnlstdel(pTEXT filnam);
extern  void         ctrcvopnlstfinal(void);
extern  void         ctFreeChkpntFillst(void);
extern  NINT         ctAllocChkpntFillst(void);

#ifdef ctFeatREPLICAT_SYNC_SRVR
#ifdef ctGVARH
extern NINT ctSyncReplAddAgentDependencyToTransaction(pCTFILE ctnum,pFUSR fu,pCTGV lctgv);
extern NINT ctSyncReplWaitForSecondaryCommit(const CTGV* lctgv, LONG8 tranno, ULONG cmtlognum, ULONG cmtlogpos);
extern NINT ctSyncReplCreateReplStateFiles(NINT sOWNR, pTEXT errorBuffer, size_t errorBufferSize);
extern NINT ctSyncReplAgentListInit(NINT sOWNR);
extern NINT ctSyncReplAgentListFinal(NINT sOWNR);
extern NINT ctSyncReplGetReplState(NINT sOWNR, pTEXT errorBuffer, size_t errorBufferSize);
extern NINT ctSyncReplGetAgentDependencyForFile(pCTFILE ctnum,pCTGV lctgv);
extern void ctSyncReplFreeTranAgentDependencyList(pCTGV lctgv);
#endif
extern void *ctSyncReplAllocCommitBlockListEntry(void);
extern void ctSyncReplFreeCommitBlockListEntry(void *pvsrpcmt);
extern void ctSyncReplSetCommitBlock(void *pvsrpcmt,ULONG cmtlognum,ULONG cmtlogpos,NINT sOWNR);
extern NINT ctSyncReplAgentListAddRef(cpTEXT agentId, pSRASTT *psraList, NINT sOWNR);
extern void ctSyncReplAgentListRemoveRef(pSRASTT sraEntry, NINT sOWNR);
extern void ctSyncReplLogTime(pTEXT desc,ULONG log,ULONG pos,ULONG8 tranno);
extern NINT ctSRFhashInit(void);
extern void ctSRFhashFinal(void);
extern NINT ctSRPCLInit(void);
extern void ctSRPCLFinal(void);
extern NINT srWaitForTransactionLogFlush(LONG lastOffsetWritten,NINT timeoutMsec);
extern void srNotifyTransactionLogFlush(ULONG logFlushPosition);
extern int checksucfls(LONG suclog,ULONG sucpos);
extern NINT ctSaveSyncReplRequirements(NINT sOWNR);
extern void ctFreeSyncReplRequirements(pTEXT buf);
extern NINT ct_replflushlog_stop(void);
#endif

#ifdef ctFeatAUTOSYSTIME
VRLEN A_STFIELDS_size(COUNT entries);
VRLEN A_STFIELDS_stream_size(TEXT mode, COUNT entries);
NINT stream2A_STFIELDS(pTEXT inbuf, pTEXT poutbuf, pVRLEN poutsize);
NINT A_STFIELDS2stream(TEXT mode, pA_STFIELDS pdefs, TEXT bytord, ppTEXT poutbuf, pVRLEN poutsize pthHan);
NINT AutoST_InitInfo(CTFILE *dnum);
VOID AutoST_FreeInfo(CTFILE *dnum);
#endif

extern NINT ctSetRecordUpdateCallbackResource(NINT mode,pVOID params pthHan);
extern NINT ctSetRecordUpdateCallbackResourceForPartitions(pCTFILE dnum,FILNO datno,NINT mode,pVOID params pthHan);
extern NINT increasePartitionBase(pTEXT partitionHostFileName,NINT rawno pthHan);
extern NINT ictAdjustOffsetForResource(pCTFILE dnum,FILNO datno,SCAN_DIRECTION scanDirection,pULONG8 pOffset pthHan);
extern const char *ctGetConfigurationOptionBuffer(void);
extern void ctFreeConfigurationOptionBuffer(void);
extern const char *ctGetLicenseOptionBuffer(void);
extern void ctFreeLicenseOptionBuffer(void);
extern  void  clearulkcache(void);
#ifdef ctFeatFILEBLOCK
extern void dbgFileBlock_LogMessage(char *msg,NINT sOWNR);
extern void dbgFileBlock_LogAltlist(pTEXT desc,pVOID altlist,pUSRLSTANC pu,NINT sOWNR);
#endif
extern void ctsetatodep(pCTFILE dnum,pXCREblk pxcreblk pthHan);

extern NINT chkIDfields(pTEXT bp,NINT update);
extern NINT chkactdelta(pTEXT bp,NINT update);
extern NINT chkdfrkops(pTEXT bp,NINT update);
extern NINT chktransactionid(pTEXT bp,pULONG8 pTransactionId);
extern void adjnent(VRLEN vlen,NINT mode,pLONG bp,LONG lpos,NINT updmod);
#if !defined(ctCLIENT) || defined(ctCLIENTcore)
extern NINT ictSetTransactionId(ULONG8 transactionId pinHan);
#endif
extern void processAugmentedLogValues(pTEXT bp,NINT update);
#ifdef ctSQL_SERVER_LOG_SIZE
extern NINT ctSetSQL_SERVER_LOG_SIZE(pTEXT kv);
#endif

extern  void *	     fccalloc(size_t num,size_t size);
extern  void	     fcfree(void * ptr);

extern pTEXT getFilnoOutputOffset(NINT sOWNR,NINT factor,NINT *ploclsize);
extern  void writeFilnoToOutputOffset(FILNO filno,pTEXT tp,NINT loclsize);
extern CTERR copyIFIL(pIFIL ifil,pTEXT dataExtension,pTEXT indexExtension,LONG defwrd,NINT mode,pIFIL *pIfilCopy);
extern CTERR copyIIDX(pIIDX iidx,NINT numidx,pIIDX *pIidxCopy,pTEXT *pIidxStream);
extern CTERR iADDRES(SYSNO sysno,FILNO datno,pVOID resptr,VRLEN varlen);
extern  NINT ictGetOrDelBackgroundLoadResource(FILNO datno,pTEXT loadid,NINT delop,ppBGLDRES ppbgldres,pNINT preslen);
extern CTERR iDELRES(SYSNO sysno,FILNO datno,pVOID resptr);
extern CTERR iENARES(SYSNO sysno,FILNO datno);
extern VRLEN iGETIFILX(FILNO datno,LONG bufsiz,pVOID bufptr,NINT ifilNUM,NINT fourByteFilno);
extern  LONG iGETRES(SYSNO sysno,FILNO datno,pVOID resptr,pVOID bufptr,VRLEN bufsiz,COUNT resmode);
extern COUNT ictRENFIL(SYSNO sysno,FILNO filno,pTEXT newname);
extern COUNT iLOKREC(SYSNO sysno,FILNO datno,CT_LOCK_MODE lokmod,ctRECPT recbyt);
extern CTERR iUPDRES(SYSNO sysno,FILNO datno,pVOID resptr,VRLEN varlen);
#ifdef ctFeatPLUGIN_CB
extern NINT ctLinkPluginCallback(CTDLL * ctdll);
extern NINT ctUnlinkPluginCallback(CTDLL * ctdll);
#endif
extern  NINT logFileIdChange(cpTEXT fileName, LONG idkey[3]);
extern  NINT ctSETCFG_LOG_FILE_ID_CHANGE(pTEXT value);
extern  NINT ctSETCFG_RECOVERY_RENAME_CHECK_FILE_ID(pTEXT value);
extern NINT ctPendingFileOpenListAdd(pTEXT fileName, NINT taskid, NINT opcode, NINT loc);
extern NINT ctPendingFileOpenListGet(pTEXT fileName, pNINT taskid, pNINT popcode, NINT loc);
extern NINT ctPendingFileOpenListDelete(pTEXT fileName, NINT opcode, NINT loc);
extern void ctPendingFileOpenListFree(void);
extern CTERR parthostAddRecord(pCTFILE ctnum, FILNO datno, pTEXT recptr, VRLEN varlen pthHan);
extern CTERR parthostUpdateRecord(pCTFILE ctnum, FILNO datno, pTEXT recptr, VRLEN varlen pthHan);

#ifdef CTPERM /* this block contains code for internal use only */

#ifdef ctFeatSYSTEM_FILE_ID_LIST
extern NINT ctSysidListAdd(pTEXT fileName,LONG8 devid, LONG8 fileidx, NINT taskid, pNINT pFoundTaskid, pSYSIDELE *ppListEntry, NINT loc);
extern NINT ctSysidListGet(LONG8 devid, LONG8 fileidx, pNINT taskid);
extern NINT ctSysidListSetPendingClose(LONG8 devid, LONG8 fileidx);
extern NINT ctSysidListDelete(pTEXT fileName,LONG8 devid, LONG8 fileidx, NINT loc);
extern NINT ctSysidListInit(void);
extern void ctSysidListFree(void);
extern NINT ctSETCFG_SYSTEM_FILE_ID_LIST(pTEXT value);
extern void sysidHashCloseLogFile(NINT sOWNR);
extern NINT systemFileIdListChangeConfig(void);
#endif

extern NINT ctGetSystemFileId(pCTFILE ctnum,pLONG8 pdevid,pLONG8 pfileidx);
#ifdef ctCMPREC
extern NINT getUserCompressionAttributes(pUSER_COMPRESSION_ATTRIBUTES pUserCompressionAttributes);
extern void freeUserCompressionAttributes(pUSER_COMPRESSION_ATTRIBUTES pUserCompressionAttributes,TEXT restore);
extern NINT setCompressionAttributes(pCMPRECRES pres,pUSER_COMPRESSION_ATTRIBUTES pOriginalCompressionAttributes);
#endif /* ctCMPREC */
extern void closeHistoryDebugLog(void);
extern NINT ctReplLoadLogShipList(NINT sOWNR);
extern void ctReplFreeLogShipList(NINT sOWNR);
extern void ctTurnOffKeepOpenAtLogoff(NINT sOWNR);
extern int ctplugin_load_on_demand(const char * pluginSpecification);
#if defined(ctCLIENT) && defined(ctCLIENTcore)
extern NINT encmsgbuf(pTEXT buf, NINT mode, VRLEN bufsiz, ppTEXT poutbuf, pVRLEN poutlen pinHan);
#endif
#ifdef ctFeatSYSTEM_TIMER_RESOLUTION
extern NINT ctSETCFG_SYSTEM_TIMER_RESOLUTION(cpTEXT value);
extern void clearTimerResolution(void);
#endif
#ifdef ctFeatCHECK_FILENAME_VALIDITY
extern NINT fileNameIsValid(pTEXT fileName);
extern NINT ctSETCFG_CHECK_FILENAME_VALIDITY(pTEXT value);
#endif
extern NINT ctSETCFG_OPENSSL_ENCRYPTION(const TEXT * value);
extern NINT ctSETCFG_SYNC_COMMIT_TIMEOUT(cpTEXT value);
extern int ctgetDefaultCipherMode(void);
extern NINT getSubstDriveListLength(pTEXT substDrives);
extern NINT copySubstDriveList(pTEXT substDrives,pTEXT outputBuffer, pNINT pOutputBufferLength);
extern NINT cptCREIFIL(pCREATE_IFIL_PARAMS pCreateIfilParams,NINT bytord,ppTEXT poutbuf,pULONG poutlen);
extern NINT cptPUTIFIL(pPUTIFIL_PARAMS pPutIfilParams, NINT bytord, ppTEXT poutbuf, pULONG poutlen);
extern NINT cptRENIFIL(pRENAME_IFIL_PARAMS pRenameIfilParams,NINT bytord,ppTEXT poutbuf,pULONG poutlen);
extern NINT cptDELIFIL(pDELETE_IFIL_PARAMS pDeleteIfilParams,NINT bytord,ppTEXT poutbuf,pULONG poutlen);
extern NINT cptDROPIDX(pDROP_INDEX_PARAMS pDropIndexParams,NINT bytord,ppTEXT poutbuf,pULONG poutlen);
extern NINT cptRESADD(pRESOURCE_ADD_PARAMS pResourceAddParams,NINT bytord,ppTEXT poutbuf,pULONG poutlen);
extern NINT cptRESUPD(pRESOURCE_UPDATE_PARAMS pResourceUpdateParams,NINT bytord,ppTEXT poutbuf,pULONG poutlen);
extern NINT cptRESDEL(pRESOURCE_DELETE_PARAMS pResourceDeleteParams,NINT bytord,ppTEXT poutbuf,pULONG poutlen);
extern NINT cptADDIDFIELD(pADD_IDFIELD_PARAMS pAddIdFieldParams,NINT bytord,ppTEXT poutbuf,pULONG poutlen);
extern NINT cptDELIDFIELD(pDEL_IDFIELD_PARAMS pDelIdFieldParams,NINT bytord,ppTEXT poutbuf,pULONG poutlen);
extern NINT cptRESETIDFIELD(pRESET_IDFIELD_PARAMS pResetIdFieldParams,NINT bytord,ppTEXT poutbuf,pULONG poutlen);
extern NINT cptXTDATRIDFIELD(pXTDATR_IDFIELD_PARAMS pXtdatrIdFieldParams,NINT bytord,ppTEXT poutbuf,pULONG poutlen);
extern NINT cptADDCHANGEIDFIELD(pADD_CHANGEIDFIELD_PARAMS pAddChangeIdFieldParams, NINT bytord, ppTEXT poutbuf, pULONG poutlen);
extern NINT cptDELCHANGEIDFIELD(pDEL_CHANGEIDFIELD_PARAMS pDelChangeIdFieldParams, NINT bytord, ppTEXT poutbuf, pULONG poutlen);
extern NINT cptSETDEFBLK(pSETDEFBLKX_PARAMS pSetDefBlkXParams,NINT bytord,ppTEXT poutbuf,pULONG poutlen);
extern NINT cptCreateIsamIndex(pCREATE_ISAM_INDEX_PARAMS pCreateIsamIndexParams,NINT bytord,ppTEXT poutbuf,pULONG poutlen);
extern NINT cptCREDATX8(pCREATE_DATA_FILE_PARAMS pCreateDataFileParams,NINT bytord,ppTEXT poutbuf,pULONG poutlen);
extern NINT cptDELFIL(pDELETE_DATA_FILE_PARAMS pDeleteDataFileParams,NINT bytord,ppTEXT poutbuf,pULONG poutlen);
extern NINT cptSERIALNUM(pSERIALNUM_PARAMS pSerialNumParams,NINT bytord,ppTEXT poutbuf,pULONG poutlen);
extern NINT cptRestorePoint(pRESTORE_POINT_PARAMS pRestorePointParams,NINT bytord,ppTEXT poutbuf,pULONG poutlen);
extern NINT WriteUpdatedPagesByFile(CTFILE * ctnum, const NINT mode, ULONG flushLimit,ULONG * pagesWritten, NINT sOWNR);
extern NINT ClearFileState(FILNO datno, CLEAR_FILE_STATE_OPTIONS options);
#ifdef ctFeatREPLICAT_SYNC_SRVR
extern NINT ctSyncReplAgentListGetInfo(ULONG agentHandleCount, pULONG agentHandleList, pTEXT outputBuffer, pUINT pOutputBufferSize, NINT sOWNR);
extern NINT chkreplagentinfo(pTEXT bp,pSYNC_COMMIT_TRAN_LIST_AGENT_INFO *outptr,pULONG pNumEntries);
extern NINT readRecoveryConfig(pSYNC_COMMIT_RECOVERY_STATE pSyncCommitRecoveryState, pTEXT targetAgentId, pREPLAGENT_SHUTDOWN_INFO pReplAgentShutdownInfo, pTEXT errbuf, ULONG errbuflen);
extern NINT ctSetServerRole(SERVER_ROLE serverRole, pTEXT errbuf, ULONG errbuflen);
extern NINT ctGetServerRole(SERVER_ROLE *pServerRole, pTEXT errbuf, ULONG errbuflen);
extern const TEXT * ctGetServerRoleDesc(SERVER_ROLE role);
extern NINT configureServerForRole(SERVER_ROLE currentServerRole, pSYNC_COMMIT_RECOVERY_STATE pSyncCommitRecoveryState, pTEXT errbuf, ULONG errbuflen);
extern NINT startReplAgentOnDemote_stop(void);
#endif
#ifdef ctREPLICATION
extern NINT ctrepfillst(pTEXT filnam, REPL_FILE_FILTER_PURPOSE purpose);
#endif
#if defined(ctSRVR) && defined(ctGVARH)
extern NINT isTransferFileAllowed(pCTGV lctgv);
#endif
extern void getBaseDirectory(TEXT baseDirectory[MAX_NAME]);
extern NINT ctReplLogSERIALNUM(pCTFILE dnum,FILNO datno,pSERIALNUM_PARAMS pSerialNumParams pthHan);
extern void cleanupRecoveryConfig(void);
extern NINT ctSETCFG_RECOVERY_CONFIG(pTEXT kv);
extern NINT ctSETCFG_REPL_USE_LAST_LOGPOS(pTEXT kv);
extern NINT ctSETCFG_CHECK_CLUSTER_ROLE(pTEXT kv);
extern NINT cfgkw(pCTINIT1 pc,pTEXT kw,pTEXT kv,NINT cfgmode);
extern NINT ctSETCFG_SECONDARY_STARTUP_TIMEOUT_SEC(pTEXT kv);
extern NINT ctSETCFG_ABANDONED_QUIET_SUSPEND_LOGON(pTEXT value);
extern void performPreShutdownActions(NINT sOWNR);
extern NINT syncReplBlockSyncCommit(NINT sOWNR, SYNC_COMMIT_BLOCK_EVENT blockEvent);
extern NINT syncReplUnblockSyncCommit(NINT sOWNR);
extern void clearPreShutdownFlag(void);
extern NINT ctSETCFG_REPLAGENT_STARTUP_TIMEOUT(cpTEXT value);
extern void dbgSyncRepl_LogMessage(NINT sOWNR, cpTEXT fmt, ...) PRINTFLIKE(2,3);
extern void dbgSyncRepl_CloseLogFile(NINT sOWNR);
extern void dbgRecycleBin_LogMessage(NINT sOWNR, cpTEXT fmt, ...) PRINTFLIKE(2, 3);
extern void dbgRecycleBin_CloseLogFile(NINT sOWNR);
extern NINT getReplicationExceptionCount(pULONG8 pExceptionCount, pTEXT errbuf, VRLEN errbuflen);
extern NINT ctSETCFG_MULTILINE_STATUS_LOG_MESSAGE(pTEXT value);
extern NINT ctFSEnableCompression(pCTFILE ctnum, pCOUNT psyserr);
extern NINT ctSETCFG_REPLICATE_ALTER_TABLE(pTEXT value);
#ifdef ctFeatBACKGROUNDflushRATE
extern NINT getbgflsrate(pTEXT kv, pNINT pmsec, pNINT pflsh, pNINT prc);
#endif
extern NINT ctAlterSchemaForPartitionMembers(FILNO datno, pCTFILE hnum, pRCDSCN precdsc);
extern NINT ctSETCFG_BLOCKING_OPEN_RETRY_LIMIT(cpTEXT value);
extern NINT ctSETCFG_BLOCKING_OPEN_RETRY_DEFER(cpTEXT value);
extern NINT ctSETCFG_SYSLOG_NONHUGE(cpTEXT value);
extern NINT ctSETCFG_SYSLOG_V11_REUSE(cpTEXT value);
extern NINT ctSETCFG_SYSLOG_EXCLUDE_SQL_USER(cpTEXT value);
extern NINT checkSystemLogAttributes(NINT sOWNR, pTEXT errbuf, size_t errbuflen);
#ifdef CTPERM_ENABLED_IN_CTOPT2_H
extern NINT iaddIDfield(FILNO datno,LONG8 startval,LONG delta,NINT fieldno,ADD_ID_FIELD_OPTIONS addIDfieldOptions);
extern NINT addIDfieldToPartMbrs(pCTFILE ctnum,FILNO datno,LONG8 startval,LONG delta,NINT fieldno pthHan);
#ifdef ctFeatAUTOSYSTIME
extern NINT iaddAutoSysTimeFields(FILNO datno,pA_STFIELDS pDefs,ADD_AUTO_SYSTIME_FIELDS_OPTIONS addAutoSysTimeFieldsOptions);
extern NINT addAutoSysTimeFieldToPartMbrs(pCTFILE ctnum,FILNO datno,pA_STFIELDS pDefs pthHan);
#endif
#endif
#ifdef ctDBG_AUTOXCREBLK
extern void checkautoxcre(pIFIL ifilptr, pXCREblk pxcreblk);
#endif
extern COUNT i2CREDATX8(FILNO datno,pTEXT filnam,UCOUNT datlen,
	UCOUNT xtdsiz,COUNT filmod,LONG permmask,pTEXT groupid,
	pTEXT fileword,pXCREblk pxcreblk,TEXT checkxcreblk);
extern COUNT i2CREIDXX8 (FILNO keyno,pTEXT filnam,COUNT keylen,COUNT keytyp,
	COUNT keydup,COUNT nomemb,UCOUNT xtdsiz,COUNT filmod,LONG permmask,
	pTEXT groupid,pTEXT fileword,pXCREblk pxcreblk,TEXT checkxcreblk);
#ifdef ctFeatSHMP_ERR_SET_PERMISSION
extern BOOL AddAccessRights(PWCHAR accountName, DWORD dwAccessMask, PWCHAR errbuf, size_t errbuflen);
#endif
extern void setRecycleBinDefaults(void);
extern NINT createsubdirs(pTEXT fileName, pNINT psyserr);
extern COUNT iDELFILX(FILNO filno, pctDELETE_FILE_XTD_OPTIONS pDeleteFileXtdOptions, pULONG8 pDeleteSequenceNo, time_t *pTimeDeleted, ULONG fileseq);
extern NINT ctSETCFG_SYSCLOSE_REOPEN_CHECK_DIFF(pTEXT value);
extern COUNT iSECURITY(FILNO filno, pVOID bufptr, VRLEN bufsiz, COUNT mode, TEXT clientCall);
#if defined(ctSRVR) && defined(ctGVARH)
extern NINT idecmsgbuf(pTEXT bufptr, pVRLEN pbufsiz, pCTGV lctgv);
extern NINT fbOpenPartitionMembers(pCTFILE ctnum, FILNO datno, pCTGV lctgv);
extern NINT fbOpenOnePartitionMember(pCTFILE ctnum, FILNO datno, FILNO ufilno, NINT rawno, pCTGV lctgv);
#endif
extern NINT getCacheSizeFromConfigValue(cpTEXT configValue, TEXT dataCache, pLONG8 pCacheSize, pTEXT msgbuf, size_t msgbuflen);
#ifdef ctFeatSCANCACHE
#ifdef ctGVARH
extern void clearScannerPagesForFile(pFUSR fu, pCTGV lctgv);
#endif
#endif
extern COUNT serverDllLogon(TEXT isamLogon, COUNT bufs, FILNO fils, COUNT sect, COUNT dbufs, COUNT userprof, cpTEXT userid, cpTEXT userword, cpTEXT servname);
#ifdef ctSRVR
#ifdef ctDBGfblku
extern void ctSETfblku(pFUSR fu, NINT newval, NINT ownr, UINT updLoc);
#else
#define ctSETfblku(fu, newval, ownr, updLoc) ((fu)->fblku = (newval))
#endif
#endif
extern NINT ctSETCFG_SHMEM_CONNECT_TIMEOUT(cpTEXT value);
extern NINT setReplicationLogRequirement(pTEXT replUniqueID, LONG lowlog, NINT sOWNR);
extern NINT checkChangeIdValue(pCTFILE dnum, FILNO datno, pTEXT recptr, VRLEN datlen, NINT func pthHan);
extern NINT setChangeIDfileAttributes(pCTFILE ctnum pthHan);
extern NINT offsetIsCurrentIsamPosition(FILNO datno, LONG hw, ctRECPT lw pthHan);

/* callback function prototype for findMatchingFileNames() */
typedef NINT(*ctFindFileCallback_t)(pTEXT filename, pVOID context);
extern NINT findMatchingFileNames(pTEXT filename, TEXT recurse, ctFindFileCallback_t callback, pVOID context);

extern NINT getPartitionMembersLogicalSize(pCTFILE ctnum, FILNO ufilno, ULONG8* filesize, NINT sOWNR);

/* callback function prototype for primeCacheForPartitionMembers() */
typedef NINT(*PCAS_primeCacheLoop_t)(pCTFILE ctnum, ULONG8 filesize, ULONG8* pBytesCached, pTEXT errbuf, size_t errbuflen, TEXT* breakout, NINT sOWNR);
extern NINT primeCacheForPartitionMembers(PCAS_primeCacheLoop_t primecachefunc, pCTFILE ctnum, FILNO ufilno, ULONG8 filesize, ULONG8* pBytesCached, pTEXT errbuf, size_t errbuflen, NINT sOWNR);

/* callback function prototype for changeSecurityForPartitionMembers() */
typedef COUNT (*changeSecurityLoop_t)(FILNO filno, pVOID bufptr, VRLEN bufsiz, COUNT mode, TEXT clientCall);
extern NINT changeSecurityForPartitionMembers(changeSecurityLoop_t changeSecurityFunc, CTFILE* ctnum, FILNO ufilno, pVOID bufptr, VRLEN bufsiz, COUNT mode, TEXT clientCall);

extern NINT ctGetFieldAttributes(pConvMap mp, NINT fieldNumber, pUINT fieldLength, pUINT fieldType);
extern NINT addEmptyCachePage(pDATBUF db, NINT sOWNR, NINT ctCPcurState);
#if defined(ctFeatDFRIDXsrvr) && defined(ctGVARH)
extern void restorePendingIndexInfo(pSHADLST sl pthHan);
#endif
extern NINT calculatePropertiesForSchemaChange(FILNO datno, pRCDSCF prcdsc);
extern NINT ctSETCFG_FLEXREC_OPTIMIZE(cpTEXT value);
extern void getConditionalResourceLen(cpTEXT idxcnd, pCOUNT pscond, pCOUNT pdcond, pNINT pexplen, pNINT pdexplen, pNINT ptotlen);
extern NINT marshalExtendedKeySegmentDefinitionsForCallIfil8(ctINDEX_ATTRIBUTES* indexAttributes, NINT bytord, ppTEXT poutbuf, pNINT poutlen);
extern NINT marshalIndexConditionsForCallIfil8(ctINDEX_ATTRIBUTES* indexAttributes, TEXT bytord, pTEXT* poutbuf, ULONG* poutlen);
#ifdef MULTITRD
extern ULONG getDefaultMultiOpenMode(void);
#endif
extern void ctsetlst(SETLST_MODE mode, VRLEN nodepage, VRLEN datapage, UINT nidx, UINT ndat);
extern void freeUUIDresources(void);
extern void ctClearOWNERthreadId(NINT sOWNR);
extern void ctSetOWNERthreadId(NINT sOWNR);

/* values for the options parameter to invalidatefile() */
typedef enum ctInvalidateFileOptions_t {
	ctINVALIDATE_FILE_NO_OPTIONS = 0x00000000,
	ctINVALIDATE_FILE_FORCE_REOPEN = 0x00000001,
	ctINVALIDATE_FILE_FORCE_READONLY = 0x00000002
} ctINVALIDATE_FILE_OPTIONS;

extern void invalidateFile(pCTFILE ctnum, NINT sOWNR, ctINVALIDATE_FILE_OPTIONS options);
extern int ctLogDumpReadTransactionNumber(const char* tp, LONG8* tn);
extern NINT ctSETCFG_AUTO_TRANDEP(const char* value);
extern FILNO ctDropIndexGetSavedKeyno(const IIDX* idxp);

#ifdef MULTITRD
extern TEXT fpilkflt(const CTFILE* ctnum);
extern void ctfusrinit(FUSR* fu);
#endif
extern void appendTotalFilesToCommBuffer(NINT loclsize, char* tp, NINT nfiles, NINT sOWNR);

extern pTEXT imballcx(NINT numobj, UINT sizobj, NINT mode, ULONG mballcOptions);
extern pTEXT ictgetmem(VRLEN iosize, ULONG getmemOptions);
#if defined(ctGVARH) && defined(ctNOGLOBALS)
extern void ctClearWNGV(CTGVAR* ctwngv);
#endif
extern NINT ctSSL_client_start(VOID);
extern NINT ctllhashload(void);
extern NINT ctllhashadd(const char* nam, TEXT typ, LONG max);
extern NINT ctllhashupd(const char* nam, TEXT typ, LONG max);
extern NINT ctllhashdel(const char* nam, TEXT typ);
extern NINT ctllhashinc(const char* nam, TEXT typ);
extern NINT ctllhashdec(const char* nam, TEXT typ);
extern NINT ictllhashadd(const char* nam, TEXT typ, LONG max, TEXT pimg);
extern NINT ictllhashupd(const char* nam, TEXT typ, LONG max, TEXT pimg);
extern NINT ictllhashdel(const char* nam, TEXT typ, TEXT pimg);
extern void ctllhashfre(void);
extern COUNT ctMEMDMP(COUNT refno, pTEXT dumpname, COUNT mode);
extern NINT addMinLogRequirement(const char* instname, LONG minlog);

#endif /* CTPERM - end of block containing code for internal use only */

#if defined(ctCLIENT) && defined(ctCLIENTcore)

#ifdef ctFeatCLUSTER_OPTIONS
extern void freeClusterState(pCLIENT_CLUSTER_STATE pClusterState);
#endif
#ifdef ctFeatCLIENT_NOTIFY
extern void ctSetCliOptionsValueBit(ULONG val);
extern void ctClearCliOptionsValueBit(ULONG val);
extern ULONG ctGetCliOptionsValue();
extern UCOUNT ctGetCliBrodCastPort();
extern void setCurrentFailoverUniqueID(const char newFailoverUniqueID[FAILOVER_UNIQUE_ID_LEN]);
#endif
extern NINT iGetServerInfoXtd(pTEXT buffer, NINT bufsiz, UCOUNT port, LONG sec, NINT mode, pGET_SERVER_INFO_XTD_PARAMS pGetServerInfoXtdParams);
extern NINT startClient_BroadcastRead(void);
extern void stopClient_BroadcastRead(void);

#endif /* ctCLIENT && ctCLIENTcore */

extern NINT ctsetsysid(pCTFILE ctnum);
extern NINT ctSETCFGvsswriterQuiesceTimeout(cpTEXT value);
extern void ct_set_actlen(pTREBUF buffer);
#ifdef ctDBGfcqa
extern void CTSETACTLEN(pTREBUF buffer, NINT actlen_value, setactlenloc_t location);
#endif
extern NINT ctSETCFG_FILE_CLOSE_FLUSH(pTEXT value);
extern NINT ctSETCFG_CLEANUP_ABLIST_ON_ABORT(pTEXT value);
extern NINT ctSETCFG_HEAP_DEBUG_LEVEL(cpTEXT value);
extern NINT ctSETCFG_HEAP_DEBUG_EXCLUSION_LIST(cpTEXT value);
extern NINT ctSETCFG_QUIET_MAX_WAIT(cpTEXT value);
extern NINT ctOSgetpid(void);
extern NINT ListHasStackSupport(NINT);
#ifdef ctGVARH
#if !defined(ctCLIENT) || defined(ctCLIENTcore)
extern void ctMonitor_ITIM_ERR(itim_err_context_t errorContext pinHan);
#endif
#endif
extern NINT makRestorePoint(pTEXT inbuf,VRLEN inlen,NINT bytord,ppRESTORE_POINT_PARAMS ppRestorePointParams);
extern void freRestorePoint(pRESTORE_POINT_PARAMS pRestorePointParams);
extern NINT cptcpyfil(pCPYFIL pcpyfil, NINT bytord, ppTEXT poutbuf, pULONG pmsglen);
#ifdef ctFeatREPLICAT
extern cpTEXT getReplicationOpCodeName(REPL_OPCODES opcode);
#endif
extern int ctstrpcmp(const char ** s1,const char ** s2);
extern int comparULONG8(const ULONG8 * key,const ULONG8 * can);
extern int comparULONG(const ULONG * key,const ULONG * can);
extern int comparNINT(const NINT * key,const NINT * can);
extern NINT ctVectorAdd(CTVECTOR * pvect,void * item);
extern NINT ctVectorInit(CTVECTOR * pvect,size_t size,ULONG vectorOptions);
extern void ctVectorTerm(CTVECTOR * pvect);
extern NINT ctVectorGet(const CTVECTOR * pvect,NINT index,void ** item);
extern NINT ctVectorGetCopy(const CTVECTOR * pvect,NINT index,void * item,size_t itemsize);
extern NINT ctVectorInsert(CTVECTOR * pvect, NINT index, void * item);
extern NINT ctVectorDelete(CTVECTOR * pvect, NINT index);
extern NINT ctVectorSortedInsert(CTVECTOR * pvect, void * item);
extern NINT ctVectorFindSorted(const CTVECTOR * pvect, NINT * index, const void * key);
extern NINT ctVectorFindSortedLTE(const CTVECTOR * pvect, NINT * index, const void * key);
extern NINT ctStackPush(CTSTACK * pstack,void * item);
extern NINT ctStackPop(CTSTACK * pstack,void * item,size_t itemsize);

#define ctStackInit(hdl,size,opt) ctVectorInit(hdl,size,opt)
#define ctStackTerm(hdl) ctVectorTerm(hdl)

double getLogRate(const LOGTIME * t1, const LOGTIME * t2);
NINT getCurrentServerLogtime(LOGTIME * t, NINT sOWNR);
NINT logposCompare(const ctLOGP * p1, const ctLOGP * p2);
NINT validateFileFeatures(const CTFILE * ctnum pthHan);

#ifndef ctCLIENT
NINT LogMUSR_ERR(NINT err);
#endif
extern NINT makDeleteIFILXtdOptions(pTEXT inbuf, VRLEN inlen, pctDELETE_IFIL_XTD_OPTIONS *ppDeleteIFILXtdOptions);
extern void freDeleteIFILXtdOptions(pctDELETE_IFIL_XTD_OPTIONS pDeleteIFILXtdOptions);
extern NINT makctFileBlockXtdOptions(cpTEXT inbuf, VRLEN inlen, ctFILE_BLOCK_XTD_OPTIONS **ppctFileBlockXtdOptions);
extern int ctosGetenv(const char *,char *,size_t);
extern const TEXT * ctGetPath(NINT);
#ifdef ctFeatFIPS
#ifdef ctFeatOPENSSL3
extern NINT ctinitFIPS(pCTSSLI pssli);
extern void cttermFIPS(pCTSSLI pssli);
#endif
#endif
extern void log_crypto_error(const TEXT * msg,NINT err);
extern NINT InsertSpaceIndexRequest(CTFILE * ctnum,ULONG8 offset,ULONG size,NINT sOWNR);
extern NINT GetAvailFileSpace(CTFILE * ctnum,VRLEN size,ULONG8 * offset,NINT sOWNR);
extern NINT cttruncate(CTFILE * ctnum, ULONG8 eof);
extern NINT ctTRANtruncate(CTFILE * ctnum,LONG hw,LONG lw);
extern NINT iMOVRES(SYSNO,FILNO,pVOID,ULONG8,VRLEN,NINT);
extern ctCONV NINT ctDECL MOVRES(FILNO,pVOID,ULONG8,VRLEN);
extern NINT validateRebuildHeaderChanges(const CTFILE* orignum, const CTFILE* newnum, TEXT * msg, size_t msglen);
extern NINT onlineRebuild(pIFILBLK ifilblk, TEXT * dataname, NINT mode);
extern NINT TempDirDeleteAtStartup(void);
extern NINT ctCloseFTmpDir(void);
extern VOID tranlog(pCTINIT1 pconfig);
extern VOID tranlogInit(void);
extern ULONG getLOG_USAGE(void);
extern void GetUUIDString(const ctUUID* uuid, char* buf, size_t bufsize);
extern NINT cptSETNULLKEYVALUE(const SET_NULL_KEY_VALUE_PARAMS* setNullKeyValueParams, TEXT bytord, ppTEXT poutbuf, pULONG poutlen);
extern const TEXT * reopenCheckSystemFileId(pCTFILE tnum,pCTFILE ctnum,NINT *fileChanged,NINT sOWNR, NINT * rc);
#ifdef ctFeatX509_AUTH
#ifdef NO_BOUND
extern int evalUserCertificate(const X509 * cert, LQMSG * l , struct cert_username_rule* rules);
#endif
#endif

#ifdef __cplusplus
}
#endif

#endif /* ~ctFUNPH */

/* end of ctfunp.h */
