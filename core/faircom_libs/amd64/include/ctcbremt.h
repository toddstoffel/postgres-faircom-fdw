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

#ifndef _CTCBREMT_H_
#define _CTCBREMT_H_

#include "ctcboptn.h"
#include "ctcbmshl.h"

LONG cts_checksum(pTEXT buf, VRLEN buflen);
VOID cts_swap(TEXT op, pVOID buffer, TEXT isinput);
NINT cts_remote(pTEXT input, LONG l_input, pTEXT output, LONG l_output, pLONG n_output, TEXT rpc);

NINT toCT_MAKE1_INPUT(pCT_MAKE1_INPUT to, pCT_MAKE2_INPUT from, VRLEN *l_to);
NINT toCT_MAKE2_INPUT(pCT_MAKE2_INPUT to, pCT_MAKE1_INPUT from, VRLEN *l_to);
#ifdef ctFeatRTG_KEYS
NINT toCT_MAKE2_INPUTfrom3(pCT_MAKE2_INPUT to, pCT_MAKE3_INPUT from, VRLEN* l_to);
#endif /* ctFeatRTG_KEYS */
NINT toCT_OPEN1_OUTPUT(pCT_OPEN1_OUTPUT to, pCT_OPEN2_OUTPUT from, VRLEN *l_to);
NINT toCT_OPEN2_OUTPUT(pCT_OPEN2_OUTPUT to, pCT_OPEN1_OUTPUT from, VRLEN *l_to);
#ifdef ctFeatRTG_KEYS
NINT toCT_OPEN2_OUTPUTfrom3(pCT_OPEN2_OUTPUT to, pCT_OPEN3_OUTPUT from, VRLEN* l_to);
NINT toCT_OPEN3_OUTPUTfrom2(pCT_OPEN3_OUTPUT to, pCT_OPEN2_OUTPUT from, VRLEN *l_to);
#endif /* ctFeatRTG_KEYS */
NINT toCT_CLOSE1_INPUT(pCT_CLOSE1_INPUT to, pCT_CLOSE3_INPUT from, VRLEN *l_to);
NINT toCT_CLOSE3_INPUT(pCT_CLOSE3_INPUT to, pCT_CLOSE1_INPUT from, VRLEN *l_to);
NINT toCT_UNLOCK1_INPUT(pCT_UNLOCK1_INPUT to, pCT_UNLOCK2_INPUT from, VRLEN *l_to);
NINT toCT_UNLOCK2_INPUT(pCT_UNLOCK2_INPUT to, pCT_UNLOCK1_INPUT from, VRLEN *l_to);
NINT toCT_SQLINFO1_INPUT(pCT_SQLINFO1_INPUT to, pCT_SQLINFO2_INPUT from, VRLEN *l_to);
NINT toCT_SQLINFO2_INPUT(pCT_SQLINFO2_INPUT to, pCT_SQLINFO1_INPUT from, VRLEN *l_to);
NINT toCT_SQLLINK1_INPUT(pCT_SQLLINK1_INPUT to, pCT_SQLLINK2_INPUT from, VRLEN *l_to);
NINT toCT_SQLLINK2_INPUT(pCT_SQLLINK2_INPUT to, pCT_SQLLINK1_INPUT from, VRLEN *l_to);
NINT toCT_ADDIDX1_INPUT(pCT_ADDIDX1_INPUT to, pCT_ADDIDX2_INPUT from, VRLEN *l_to);
#ifdef ctFeatRTG_KEYS
NINT toCT_ADDIDX2_INPUTfrom3(pCT_ADDIDX2_INPUT to, pCT_ADDIDX3_INPUT from, VRLEN* l_to);
#endif /* ctFeatRTG_KEYS */
NINT toCT_ADDIDX2_INPUT(pCT_ADDIDX2_INPUT to, pCT_ADDIDX1_INPUT from, VRLEN *l_to);

#endif /* ~_CTCBREMT_H_ */

/* end of ctcbremt.h */
