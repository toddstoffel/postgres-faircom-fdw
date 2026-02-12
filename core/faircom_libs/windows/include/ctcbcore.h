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

#ifndef _CTCBCORE_H_
#define _CTCBCORE_H_

#include "ctcboptn.h"
#include "ctcbmshl.h"
#include "ctcbtype.h"
#include "ctdbxml.h"

typedef struct {
	ULONG	type;
	ULONG	num;
	union {
		ULONG	len;
		TEXT	name[FCRNAM_LEN];
	} u;
} RESID;

typedef struct {
	RESID	id;
	TEXT	sign;		/* COBOL sign convention */
} RESFEAT, * pRESFEAT;
typedef struct {
	RESID	id;
	TEXT	sign;   /* COBOL sign convention */
	UTEXT	rechdr; /* record header length */
	TEXT	srlidx; /* file has serial index */
} RESFEAT1, * pRESFEAT1;

typedef struct {
	RESID	id;
	VRLEN	maxlen;
	LONG	blockfact;
	LONG	prealloc;
	LONG	extension;
	TEXT	compress;
	TEXT	comment[CT_COMMENT_LEN];
	/* end of RESINFO structure */
	VRLEN	minlen;
	/* end of RESINFO1 structure */
} RESINFO1, * pRESINFO1;

typedef struct {
	RESID	id;
	TEXT	def[1];
} RESDEFS, * pRESDEFS;

typedef enum {
	SAVEINFO_CREATE = 0,
	SAVEINFO_ADD_KEYS = 1,
	SAVEINFO_ADD_RTGI = 2,
	SAVEINFO_ADD_XDD_INFO = 3, /* updates the RTGI/ATTR information with XDD specific info like signconv*/
	SAVEINFO_ADD_XDD_SCHEMAS = 4, /* add/updates the XDD */
	SAVEINFO_UPD_RTGI = 5 /* updates the RTGI/ATTR information with */
} SAVEINFO_MODE;

#ifdef ctFeatRTG_KEYS
#ifdef ctFeatRTG_FILEPASSWORD
COUNT cts_make3(pCT_MAKE3_INPUT input, pCT_MAKE2_OUTPUT output, pTEXT makename, cpTEXT filwrd, pIFIL ifil, LONG xfilmod, pTEXT datext, pTEXT idxext, LONG permmask, pXCREblk xcb, pRESDEFS* xddptr, pVRLEN xddlen);
COUNT cts_make2(pCT_MAKE3_INPUT input, pCT_MAKE2_OUTPUT output, pCT_OPEN3 make, pRTGKEY keys, cpTEXT filwrd, pIFIL ifil, LONG xfilmod, pTEXT datext, pTEXT idxext, LONG permmask, pXCREblk xcb, CTBOOL haskdefs, pRESINFO info, pTEXT created);
#else /* ~ctFeatRTG_FILEPASSWORD */
COUNT cts_make3(pCT_MAKE3_INPUT input, pCT_MAKE2_OUTPUT output, pTEXT makename, pIFIL ifil, LONG xfilmod, pTEXT datext, pTEXT idxext, LONG permmask, pXCREblk xcb, pRESDEFS* xddptr, pVRLEN xddlen);
COUNT cts_make2(pCT_MAKE3_INPUT input, pCT_MAKE2_OUTPUT output, pCT_OPEN3 make, pRTGKEY keys, pIFIL ifil, LONG xfilmod, pTEXT datext, pTEXT idxext, LONG permmask, pXCREblk xcb, CTBOOL haskdefs, pRESINFO info, pTEXT created);
#endif /* ~ctFeatRTG_FILEPASSWORD */
NINT cts_make(pCT_MAKE3_INPUT input, VRLEN l_input, pCT_MAKE2_OUTPUT output, pVRLEN l_output);
NINT cts_open(pCT_OPEN2_INPUT input, VRLEN l_input, pCT_OPEN3_OUTPUT output, pVRLEN l_output);
#else /* ~ctFeatRTG_KEYS */
COUNT cts_make3(pCT_MAKE2_INPUT input, pCT_MAKE2_OUTPUT output, pTEXT makename, pIFIL ifil, LONG xfilmod, pTEXT datext, pTEXT idxext, LONG permmask, pXCREblk xcb, pRESDEFS* xddptr, pVRLEN xddlen);
COUNT cts_make2(pCT_MAKE2_INPUT input, pCT_MAKE2_OUTPUT output, pCT_OPEN2 make, pCT_KEY2 keys, pIFIL ifil, LONG xfilmod, pTEXT datext, pTEXT idxext, LONG permmask, pXCREblk xcb, CTBOOL gnxseg, pRESINFO info, pTEXT created);
NINT cts_make(pCT_MAKE2_INPUT input, VRLEN l_input, pCT_MAKE2_OUTPUT output, pVRLEN l_output);
NINT cts_open(pCT_OPEN2_INPUT input, VRLEN l_input, pCT_OPEN2_OUTPUT output, pVRLEN l_output);
#endif /* ~ctFeatRTG_KEYS */
#ifdef ctFeatRTG_KEYS
NINT cts_addidx(pCT_ADDIDX3_INPUT, VRLEN, pCT_ADDIDX_OUTPUT, pVRLEN);
#else
NINT cts_addidx(pCT_ADDIDX2_INPUT, VRLEN, pCT_ADDIDX_OUTPUT, pVRLEN);
#endif
NINT cts_close(pCT_CLOSE3_INPUT, VRLEN, CT_CLOSE_OUTPUT *, pVRLEN);
NINT cts_info(pCT_INFO_INPUT, VRLEN , pCT_INFO_OUTPUT, pVRLEN );
NINT cts_sqlinfo(pCT_SQLINFO2_INPUT, VRLEN, CT_SQLINFO_OUTPUT *, pVRLEN);
NINT cts_sqllink(pCT_SQLLINK2_INPUT, VRLEN, pCT_SQLLINK_OUTPUT, pVRLEN);
#ifdef ctFeatRTG_ALTERSCHEMA
#ifdef ctFeatRTG_ALTERSCHEMAv2
NINT cts_alter(pCT_REMOVE2_INPUT input, VRLEN l_input, pCT_REMOVE_OUTPUT output, pVRLEN l_output);
#else
NINT cts_alter(pCT_SQLINFO2_INPUT input, VRLEN l_input, CT_SQLINFO_OUTPUT* output, pVRLEN l_output);
#endif
#endif
#ifdef ctFeatRTG_FILEPASSWORD
NINT cts_fileword(pCT_SQLINFO2_INPUT input, VRLEN l_input, CT_SQLINFO_OUTPUT* output, pVRLEN l_output);
#ifdef ctFeatRTG_PARTITION
NINT cts_partition(pCT_REMOVE2_INPUT input, VRLEN l_input, pCT_REMOVE_OUTPUT output, pVRLEN l_output);
#endif /* ctFeatRTG_PARTITION */
#endif /* ctFeatRTG_FILEPASSWORD */
#ifdef ctFeatRTG_ACCESS
NINT cts_access(pCT_ACCESS_INPUT input, VRLEN l_input, pCT_ACCESS_OUTPUT output, pVRLEN l_output);
#endif

COUNT cts_getInternalFields(RESINFO *info);
NINT cts_write(CT_WRITE_INPUT *, VRLEN, CT_WRITE_OUTPUT *, pVRLEN);
NINT cts_read(pCT_COMM_INPUT input, VRLEN l_input, pCT_COMM_OUTPUT output, pVRLEN l_output);
NINT cts_start(CT_START_INPUT *, VRLEN, CT_START_OUTPUT *, pVRLEN);
NINT cts_datascan(pCT_COMM_INPUT input, VRLEN l_input, pCT_COMM_OUTPUT output, pVRLEN l_output);
NINT cts_nextprev(pCT_COMM_INPUT input, VRLEN l_input, pCT_COMM_OUTPUT output, pVRLEN l_output);
NINT cts_nextprev2(pCT_COMM_INPUT input, VRLEN l_input, pCT_COMM_OUTPUT output, pVRLEN l_output);
NINT cts_nextprev3(pCT_COMM_INPUT input, VRLEN l_input, pCT_COMM_OUTPUT output, pVRLEN l_output);
NINT cts_nextprev4(pCT_COMM_INPUT input, pCT_COMM_OUTPUT output, FILNO keyno, pTEXT key, ppTEXT checkey);
NINT cts_rewdel(pCT_REWDEL_INPUT input, VRLEN l_input, pCT_REWDEL_OUTPUT output, pVRLEN l_output);
NINT cts_rewdel2(pCT_REWDEL_INPUT input, VRLEN l_input, pCT_REWDEL_OUTPUT output, pVRLEN l_output);
NINT cts_delete(CT_REWDEL_INPUT *, VRLEN, CT_REWDEL_OUTPUT *, pVRLEN);
NINT cts_unlock(pCT_UNLOCK2_INPUT, VRLEN, CT_UNLOCK_OUTPUT *, pVRLEN);
NINT cts_version(pCT_VERSION2_INPUT, VRLEN, pCT_VERSION2_OUTPUT, pVRLEN);
NINT cts_copy(CT_COPY_INPUT *, VRLEN , CT_COPY_OUTPUT *, pVRLEN );
NINT cts_remove(pCT_REMOVE2_INPUT input, VRLEN l_input, pCT_REMOVE_OUTPUT output, pVRLEN l_output);
NINT cts_rename(pCT_RENAME2_INPUT input, VRLEN l_input, pCT_RENAME_OUTPUT output, pVRLEN l_output);

#ifdef ctFeatMAXFILE32
#ifdef ctFeatRTG_RECORD_FOOTER
COUNT cts_readrec(pCT_COMM_INPUT input, pCT_COMM_OUTPUT output, pTEXT recbuf, pULONG reclen, FILNO keyno, pTEXT chkkey);
#else /* ~ctFeatRTG_RECORD_FOOTER */
COUNT cts_readrec(pCT_COMM_INPUT input, pCT_COMM_OUTPUT output, pTEXT recbuf, pULONG reclen, TEXT isvlen, FILNO keyno, pTEXT chkkey);
#endif /* ~ctFeatRTG_RECORD_FOOTER */
#else /* ~ctFeatMAXFILE32 */
#ifdef ctFeatRTG_RECORD_FOOTER
COUNT cts_readrec(pCT_COMM_INPUT input, pCT_COMM_OUTPUT output, pTEXT recbuf, pULONG reclen, COUNT keyno, pTEXT chkkey);
#else /* ~ctFeatRTG_RECORD_FOOTER */
COUNT cts_readrec(pCT_COMM_INPUT input, pCT_COMM_OUTPUT output, pTEXT recbuf, pULONG reclen, TEXT isvlen, COUNT keyno, pTEXT chkkey);
#endif /* ~ctFeatRTG_RECORD_FOOTER */
#endif /* ~ctFeatMAXFILE32 */

#ifdef ctFeatMAXFILE32
ULONG cts_getrecord(pCT_COMM_INPUT input, pCT_COMM_OUTPUT output, pTEXT recbuf, ULONG bufsiz, pTEXT key, FILNO keyno, pTEXT chkkey);
ULONG cts_getrecord2(pCT_COMM_INPUT input, pCT_COMM_OUTPUT output, pTEXT recbuf, ULONG bufsiz, FILNO keyno, pTEXT chkkey);
#else /* ~ctFeatMAXFILE32 */
ULONG cts_getrecord(pCT_COMM_INPUT input, pCT_COMM_OUTPUT output, pTEXT recbuf, ULONG bufsiz, pTEXT key, COUNT keyno, pTEXT chkkey);
ULONG cts_getrecord2(pCT_COMM_INPUT input, pCT_COMM_OUTPUT output, pTEXT recbuf, ULONG bufsiz, COUNT keyno, pTEXT chkkey);
#endif /* ~ctFeatMAXFILE32 */

#ifdef ctFeatMAXFILE32
NINT cts_GETIFIL(CT_INPUT *input, FILNO filno, ppIFIL isam, CT_ERROR *err);
#else
NINT cts_GETIFIL(CT_INPUT *input, COUNT filno, ppIFIL isam, CT_ERROR *err);
#endif
#ifdef ctFeatMAXFILE32
NINT cts_GETXCREBLK(CT_INPUT *input, FILNO filno, LONG drvfilemode, UCOUNT nkeys, pIFIL pifil, ppXCREblk ppxcb, pCOUNT pxcbCount, CT_ERROR *err);
#else
NINT cts_GETXCREBLK(CT_INPUT *input, COUNT filno, LONG drvfilemode, UCOUNT nkeys, pIFIL pifil, ppXCREblk ppxcb, pCOUNT pxcbCount, CT_ERROR *err);
#endif
#ifdef ctFeatRTG_KEYS
NINT cts_getinfo(pCT_OPEN2_INPUT input, pCT_OPEN3_OUTPUT output, pIFIL ifil, pRESINFO info);
#else
NINT cts_getinfo(pCT_OPEN2_INPUT input, pCT_OPEN2_OUTPUT output, pIFIL ifil, pRESINFO info);
#endif
pTEXT cts_setcompress(pTEXT attrstr, size_t attrstrlen, TEXT comptype, TEXT complevl, TEXT compstgy);
#ifdef ctFeatMAXFILE32
COUNT cts_LOKREC(pLONG env, FILNO filno, COUNT mode, LONG record, LONG recordh, pCT_INPUT input);
COUNT cts_LKISAM(pLONG env, FILNO filno, COUNT mode);
#else
COUNT cts_LOKREC(pLONG env, COUNT filno, COUNT mode, LONG record, LONG recordh, pCT_INPUT input);
COUNT cts_LKISAM(pLONG env, COUNT filno, COUNT mode);
#endif
NINT cts_CHGICON(COUNT input, pCOUNT output);
#ifdef ctFeatMAXFILE32
COUNT ctcbGetInfo(FILNO filno, pRESINFO info, pCT_INPUT input, pCT_ERROR err, pIFIL ifil);
COUNT ctcbSaveInfo(FILNO datno, pRESINFO info, pCT_INPUT input, pCT_ERROR err, pTEXT XDD, SAVEINFO_MODE mode);
LONG cts_GETRES(FILNO datno, pVOID resptr, pVOID bufptr, VRLEN bufsiz, COUNT resmode);
#else
COUNT ctcbGetInfo(COUNT filno, pRESINFO info, pCT_INPUT input, pCT_ERROR err, pIFIL ifil);
COUNT ctcbSaveInfo(COUNT datno, pRESINFO info, pCT_INPUT input, pCT_ERROR err, pTEXT XDD, SAVEINFO_MODE mode);
LONG cts_GETRES(COUNT datno, pVOID resptr, pVOID bufptr, VRLEN bufsiz, COUNT resmode);
#endif

pTEXT cts_getcrec(size_t reclen);
VOID cts_putcrec(pTEXT recptr);

#ifdef ctFeatRTG_ATTRAPI
COUNT cts_info2Attr(FILNO datno, pRESINFO info, pCT_INPUT input, pCT_ERROR err, SAVEINFO_MODE mode);
COUNT cts_Attr2info(FILNO filno, pRESINFO info, TEXT keepHnd, TEXT reslen);
#endif

#endif /* ~_CTCBCORE_H_ */

/* end of ctcbcore.h */
