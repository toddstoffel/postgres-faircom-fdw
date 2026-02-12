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

#ifndef _CTCBSWTC_H_
#define _CTCBSWTC_H_

#include "ctcboptn.h"
#include "ctcbconf.h"
#include "ctcbapi.h"

typedef NINT (* EXTFH_fn)(
	UTEXT *op,
	UTEXT *fcd
);

#ifdef ctPortWIN32
typedef COUNT (WINAPI* BTRCALLID_fn)(
#else
typedef COUNT (ctDECL* BTRCALLID_fn)(
#endif
	UCOUNT  operation,
	pVOID   posBlock,
	pVOID   dataBuffer,
	pULONG  dataLength,
	pVOID   keyBuffer,
	UTEXT   keyLength,
	TEXT    keyNumber,
	pUTEXT  clientID
);

typedef union _SwitchHandler_fn {
	pFUNC fn;
	BTRCALLID_fn BTRCALLID;
	EXTFH_fn EXTFH;
} SwitchHandler_fn;

typedef struct _SWITCH_HANDLE
{
	LONG8        validate;
	SwitchHandler_fn fpHandler;
	pVOID        hFile;
	pVOID        hTag;
} SWITCH_HANDLE, *pSWITCH_HANDLE;

#ifdef ctN8
#define VALIDATE 0x600DF00D0DF00D60
#else
#define VALIDATE_HI 0x600DF00D
#define VALIDATE_LO 0x0DF00D60
#endif

NINT ctcbSwtcLoadRedir(pRTGCONF conf, INTFTYPE intf);
SwitchHandler_fn ctcbSwtcGetHandler(pRTGCONF conf, INTFTYPE intf, pTEXT filnam, TEXT filorg, SwitchHandler_fn fpHandler, INSTTYPE *type);
pSWITCH_HANDLE ctcbSwtcAlloc(pVOID hFile, SwitchHandler_fn fpHandler, pVOID hTag);
VOID ctcbSwtcFree(pSWITCH_HANDLE *hSwitch);
VOID ctcbSwtcSet(pSWITCH_HANDLE hSwitch, pVOID hFile, SwitchHandler_fn fpHandler, pVOID hTag);
VOID ctcbSwtcReset(pSWITCH_HANDLE hSwitch);
NINT ctcbSwtcIsValid(pSWITCH_HANDLE hSwitch);

#endif /* ~_CTCBSWTC_H_ */

/* end of ctcbswtc.h */
