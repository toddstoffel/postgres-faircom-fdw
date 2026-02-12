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

#ifndef _CTCBXDDV_H_
#define _CTCBXDDV_H_

#include "ctcboptn.h"

#ifdef ctFeatRTG_ATTRAPI
typedef enum {
	XDD_CONV_NOERR = 0, /*CTDBRET_OK*/
	XDD_CONV_NOMRTMATCH = 1,
	XDD_CONV_NOMRTFILTERERR = 2,
	XDD_CONV_NOMEM = 3,
	XDD_CONV_UNDERLEN = 4,
	XDD_CONV_OVERLEN = 5,
	XDD_CONV_INTERNAL = 1000, /*CTDBRET_CALLBACK_5*/
	XDD_CONV_INVTYPE = 1001, /*CTDBRET_CALLBACK_19*/
	XDD_CONV_NOTSUPPORTED = 1002, /*CTDBRET_NOTSUPPORTED*/
	XDD_CONV_INVBOOL = 1003, /*CTDBRET_CALLBACK_10*/
	XDD_CONV_UNDERFLOW = 1004, /*CTDBRET_UNDERFLOW*/
	XDD_CONV_OVERFLOW = 1005, /*CTDBRET_OVERFLOW*/
	XDD_CONV_INVNUMBER = 1006, /*CTDBRET_INVNUMBER or CTDBRET_NULARG*/
	XDD_CONV_INVDATETIMEFORMAT = 1007, /*CTDBRET_INVFORMAT*/
	XDD_CONV_INVHOUR = 1008, /*CTDBRET_INVHOUR*/
	XDD_CONV_INVMINUTE = 1009, /*CTDBRET_INVMINUTE*/
	XDD_CONV_INVSECOND = 1010, /*CTDBRET_INVSECOND*/
	XDD_CONV_INVDATE = 1011, /*CTDBRET_INVDATE*/
	XDD_CONV_INVYEAR = 1012,/*CTDBRET_INVYEAR*/
	XDD_CONV_INVMONTH = 1013, /*CTDBRET_INVMONTH*/
	XDD_CONV_INVDAY = 1014, /*CTDBRET_INVDAY*/
	XDD_CONV_ARGSMALL = 1015, /*CTDBRET_ARGSMALL*/
	XDD_CONV_NOTADIGIT = 1016, /*CTDBRET_CALLBACK_10*/
	XDD_CONV_BADSIGN = 1017, /*CTDBRET_CALLBACK_10*/
	XDD_CONV_INVINTSIZE = 1018, /*CTDBRET_CALLBACK_19*/
	XDD_CONV_DIGITOVERFLOW = 1019, /*CTDBRET_CALLBACK_10*/
	XDD_CONV_INVUTF = 1020 /*CTDBRET_CALLBACK_10*/
} XDD_CONV_RET;
#endif

cbDLLexport pVOID ct_XDDOpen(COUNT signconv, TEXT text[CT_PATH_LEN]);
cbDLLexport pVOID ct_XDDOpenX(COUNT signconv, TEXT text[CT_PATH_LEN], TEXT log);
cbDLLexport VOID ct_XDDClose(pVOID hdl);
cbDLLexport LONG ct_XDDCheck(pVOID hdl, pUTEXT buffer, LONG buflen, pTEXT msg, size_t msglen, pLONG offset, pLONG size, pLONG type);
cbDLLexport LONG ct_XDDCheckX(pVOID hdl, pUTEXT buffer, LONG buflen, pTEXT msg, size_t msglen, pLONG offset, pLONG size, pLONG type, pLONG schema);

#endif /* ~_CTCBXDDV_H_ */

/* end of ctcbxddv.h */
