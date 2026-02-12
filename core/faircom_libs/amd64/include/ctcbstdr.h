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

#ifndef _CTCBSTDR_H_
#define _CTCBSTDR_H_

#include "ctcboptn.h"
#include "ctcrsdk.h"
#include "ctcberr.h"

#include <memory.h>
#include <string.h>

#define rtgMAX(x,y) ((x)>(y)?(x):(y))
#define rtgMIN(x,y) ((x)<(y)?(x):(y))

#define rtgDIRSEPW '\\'
#define rtgDIRSEPU '/'

#define CTCB_MIN_XMLLEN 11   /* this is enough to contain the maximum string for ULONG */

cbDLLexport VOID rtgFREE(pVOID ptr);
cbDLLexport pVOID rtgMALLOC(size_t size);
cbDLLexport pVOID rtgCALLOC(size_t num, size_t size);
cbDLLexport pVOID rtgEXPALLOC(pVOID ptr, size_t size);
cbDLLexport pVOID rtgSTRDUP(cpTEXT str);

#define rtgSIZEOF(x) (ctSIZE(x))
#define rtgSIZEOFARRAY(a) (rtgSIZEOF(a)/rtgSIZEOF(a[0]))

#define rtgMEMGET(n) rtgCALLOC(1,n)
#define rtgMEMPUT(p) do{if (p != NULL) {pVOID _p = p; p = NULL; rtgFREE(_p);}}while(0);
#define rtgMEMCPY(d, s, n) cpybuf(d, s, (UINT)n)
#define rtgAMEMCPY(d, s) rtgMEMCPY(d, s, rtgSIZEOF(d))
#define rtgMEMMOVE(d, s, n) memmove(d, s, n)
#define rtgMEMSET(d, c, n) ctbfill(d, c, (size_t)n)
#define rtgMEMZERO(d, n) rtgMEMSET(d, 0, n)
#define rtgAMEMZERO(a) rtgMEMZERO(&(a), rtgSIZEOF(a))

#define rtgPTRCPY(d, s) do{if (d != NULL && s != NULL) {pTEXT p = s; rtgMEMCPY(d, &p, rtgSIZEOF(pTEXT));}}while(0);

cbDLLexport pTEXT rtgSTRSTR(cpTEXT s1, cpTEXT s2, TEXT casesensitive);
cbDLLexport pTEXT rtgSTRRSTR(cpTEXT s1, cpTEXT s2, TEXT casesensitive);

#define rtgSTRLEN(s) ((s != NULL) ? ctrt_strlen(s) : (size_t)0)

#define rtgSTRICMP(s1, s2) ctrt_stricmp(s1, s2)
#define rtgSTRNICMP(s1, s2, n) ctrt_strnicmp(s1, s2, n)
#define rtgSTRCMP(s1, s2, cs) ((cs) ? ctrt_strcmp(s1, s2) : rtgSTRICMP(s1, s2))
#define rtgSTRNCMP(s1, s2, n, cs) ((cs) ? ctrt_strncmp(s1, s2, n) : rtgSTRNICMP(s1, s2, n))

#ifdef ctFeatSAFE_CRT
#define rtgSTRNCPY(d, s, l) ctrt_strncpy(d, s, l)
#define rtgASTRCPY(d, s) rtgSTRNCPY(d, s, rtgSIZEOF(d))
#else
#define rtgSTRNCPY(d, s, l) ctrt_strncpy(d, s, l + 1)
#define rtgASTRCPY(d, s) rtgSTRNCPY(d, s, rtgSIZEOF(d)-1)
#endif

#ifdef ctFeatSAFE_CRT
#define rtgSTRCAT(d, n, s) ctrt_strncat_s(d, n, s)
#define rtgASTRCAT(d, s) rtgSTRCAT(d, rtgSIZEOF(d), s)
#else
#define rtgSTRCAT(d, n, s) strncat(d, s, n)
#define rtgASTRCAT(d, s) rtgSTRCAT(d, rtgSIZEOF(d)-1, s)
#endif

#define rtgSNPRINTF ctrt_snprintf
#define rtgSPRINTFCATCOUNT(dst, src, dstlen, cntlen, arg) \
{ \
	rtgSNPRINTF arg; \
	cntlen += strlen(src); \
	if (dst && (strlen(dst)+strlen(src)) < dstlen) { \
		rtgSTRCAT(dst, dstlen, src); \
	} \
}

#define rtgACCESS(p,m) ctrt_access(p,m)
#define rtgSTAT(p,s) stat(p,s)
#define rtgLOCALTIME(s,d) ctrt_localtime_r(s,d)
#define rtgUNLINK(n) ctrt_unlink(n)

#define rtgISSPACE(p) isspace((int)p)
#define rtgISDIGIT(p) isdigit((int)p)
#define rtgISPRINT(p) isprint((int)p)

#define rtgATOL atol
#ifdef ctN8
#ifdef ctPortUNIX
#define rtgATOLL atoll
#else
#define rtgATOLL _atoi64
#endif
#endif

#define rtgSTRERROR(e,s,l) ctrt_strerror_r(e,s,l)
#define rtgASTRERROR(e,s) rtgSTRERROR(e,s,rtgSIZEOFARRAY(s))

typedef VOID (*pFUNC)();

#ifdef _WIN32

#ifdef UNICODE
#define ctLoadLib(l)               LoadLibraryA(l)
#else /* ~UNICODE */
#define ctLoadLib(l)               LoadLibrary(l)
#endif /* ~UNICODE */
#define ctFreeLib(h)               !FreeLibrary(h)
#define ctGetFunc(h,f,p)           (void (*)(void))GetProcAddress((HINSTANCE)h,f)
#define ctLoadLibErr()             GetLastError()
#define ctLoadLibErrMsg()          NULL

#else  /* ~_WIN32 */

/* If compiling with gcc 3.2.2 on HP/UX 11: */
#ifdef HPUX
#if ((__GNUC__ == 3) && (__GNUC_MINOR__ == 2) && (__GNUC_PATCHLEVEL__ == 2))
/*
** Use shl_load(), shl_unload(), and shl_findsym() instead of
** dlopen(), dlclose(), and dlsym().
*/
#define ctUSE_SHL_FUNC
#endif
#endif

#ifdef ctUSE_SHL_FUNC

#include <dl.h>
#define ctLoadLib(l)            shl_load(l,BIND_IMMEDIATE,0L)
#define ctFreeLib(h)            shl_unload(h)
#define ctGetFunc(h,f,p)        shl_findsym(h,f,TYPE_PROCEDURE,&p)
#define ctLoadLibErr()          errno
#define ctLoadLibErrMsg()       NULL

#else  /* ~ctUSE_SHL_FUNC */

#include <dlfcn.h>
#define ctLoadLib(l)            dlopen(l,RTLD_NOW|RTLD_LOCAL)
#define ctFreeLib(h)            dlclose(h)
#define ctGetFunc(h,f,p)        dlsym(h,f)
#define ctLoadLibErr()          0
#define ctLoadLibErrMsg()       dlerror()

#endif /* ~ctUSE_SHL_FUNC */

#endif /* ~_WIN32 */

#ifdef ctFeatRTG_FEATMASK
ULONG rtgFEATMASK(VOID);
ULONG rtgFEATMASKHAS(pULONG mask, ULONG bits);
#endif

#define rtgSTRUCTVERHAS(structver, feature) ((feature <= CT_STRUCTVER_LAST) ? (structver >= feature) : (structver & feature))
ULONG ctcbStructverID(VOID);

pTEXT rtgBASENAME(pTEXT path);

NINT ctcbFileExist(pTEXT path);
NINT ctcbIsXmlFile(pTEXT path);
cbDLLexport NINT ctcbIsXml(pTEXT str);
#ifdef ctFeatMAXFILE32
FILNO rtgOPNRFILX(FILNO filno, pTEXT filnam, COUNT filmod, LONG xfilmod, cpTEXT filwrd);
FILNO rtgOPNRFIL(FILNO filno, pTEXT filnam, COUNT filmod, LONG xfilmod);
CTERR rtgOPNFILX(FILNO filno, pTEXT filnam, COUNT filmod, cpTEXT filwrd);
CTERR rtgOPNFIL(FILNO filno, pTEXT filnam, COUNT filmod);
#else /* ~ctFeatMAXFILE32 */
COUNT rtgOPNRFILX(COUNT filno, pTEXT filnam, COUNT filmod, LONG xfilmod, cpTEXT filwrd);
COUNT rtgOPNRFIL(COUNT filno, pTEXT filnam, COUNT filmod, LONG xfilmod);
COUNT rtgOPNFILX(COUNT filno, pTEXT filnam, COUNT filmod, cpTEXT filwrd);
COUNT rtgOPNFIL(COUNT filno, pTEXT filnam, COUNT filmod);
#endif /* ~ctFeatMAXFILE32 */
#ifdef ctFeatRTG_FILEPASSWORD
pIFIL rtgGETIFIL(pTEXT filnam, cpTEXT filwrd);
#else /* ~ctFeatRTG_FILEPASSWORD */
pIFIL rtgGETIFIL(pTEXT filnam);
#endif /* ~ctFeatRTG_FILEPASSWORD */
int rtgISSPACESTR(const char *s);
void* ctcbLoadLibrary(const char *name);
int ctcbLoadLibraryError(void);
char* ctcbLoadLibraryErrorMessage(void);
void (*ctcbGetProcAddress(void* hlib, const char *name))(void);
VOID ctcbSetError(CT_ERROR *err, COUNT logical, LONG ctree, LONG system, pTEXT errmsg, pTEXT msg);
VOID ctcbGetError(pCOUNT _errno, pLONG _int_errno, pLONG _int2_errno, pTEXT _errmsg, CT_ERROR *err, cpTEXT msg);
ULONG ctcbGetPID(VOID);
ULONG ctcbGetTID(VOID);
NINT rtgGETENV(cpTEXT var, ppTEXT val);
cbDLLexport NINT ctcbEncryptFile(pTEXT inpfile, pTEXT outfile, CT_ERROR *err, pTEXT errmsg, size_t errmsglen);
size_t ctcbDecryptFile(pTEXT inpfile, pTEXT outbuf, size_t outlen, CT_ERROR *err);
pTEXT rtgBase64Encode(cpTEXT inputBuffer, VRLEN inlen, pTEXT outBuffer, pVRLEN outlen);
cbDLLexport pTEXT rtgBase64Decode(cpTEXT inputBuffer, pTEXT outbuf, pVRLEN outsize);
#ifdef ctFeatRTG_TASKID
NINT ctcbExpandName(pTEXT format, pTEXT dest, NINT destlen, pTEXT filedir, pTEXT filename, pCOUNT instance, pRTGTASKID taskid);
#else
NINT ctcbExpandName(pTEXT format, pTEXT dest, NINT destlen, pTEXT filedir, pTEXT filename, pCOUNT instance);
#endif
RTGTASKID ctcb_filelocktaskid(pTEXT filname);
NINT ctcbGetRecordNumber(ULONG *recnum, pTEXT recbuf, UTEXT hdrsiz, UTEXT srlsiz);
#ifdef ctFeatRTG_KEYS
UCOUNT rtgNumOfRTGSEG(pRTGKEY keys, UCOUNT nkeys);
UCOUNT rtgMaxRTGSEG(pRTGKEY keys, UCOUNT nkeys);
size_t rtgSizeOfRTGKEYSEG(pRTGKEY keys, UCOUNT nkeys);
VOID rtgAdjustRTGKEY(pRTGKEY key, UCOUNT nkeys, LONG offset);
UCOUNT rtgDeleteRTGKEY(pRTGKEY key, UCOUNT nkeys);
TEXT rtgAdjustRTGSEG(pRTGSEG seg, UCOUNT nsegs, COUNT rechdr, COUNT internal_fields);
#endif
#ifdef ctFeatRTG_GET_EXTENDED
UCOUNT rtgNumOfRTGRSETFLD(pRTGRSET rset);
size_t rtgSizeOfRTGRSET(pRTGRSET rset);
#endif /* ctFeatRTG_GET_EXTENDED */

#endif /* ~_CTCBSTDR_H_ */

/* end of ctcbstdr.h */
