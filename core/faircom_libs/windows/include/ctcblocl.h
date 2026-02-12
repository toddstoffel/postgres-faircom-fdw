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

#ifndef _CTCBLOCL_H_
#define _CTCBLOCL_H_

#include "ctcboptn.h"
#include "ctcbmshl.h"

LONG RPC_CALL(pTEXT inBuffer, LONG inBufLen, pTEXT outBuffer, LONG outBufLen);
#ifdef ctFeatRTG_HANDLE
LONG ctl_server(pCT_INPUT input, VRLEN l_input, pCT_OUTPUT output, VRLEN l_output, CTHANDLE handle, cpTEXT name);
NINT ctl_local(pCT_INPUT input, VRLEN l_input, pCT_OUTPUT output, VRLEN l_output, CTHANDLE handle, cpTEXT name);
#else /* ~ctFeatRTG_HANDLE */
#ifdef ctFeatRTG_LOGWHOFORMAT
LONG ctl_server(pCT_INPUT input, VRLEN l_input, pCT_OUTPUT output, VRLEN l_output, cpVOID extra, pTEXT logwhofmt);
NINT ctl_local(pCT_INPUT input, VRLEN l_input, pCT_OUTPUT output, VRLEN l_output, cpVOID extra, pTEXT logwhofmt);
#else /* ~ctFeatRTG_LOGWHOFORMAT */
LONG ctl_server(pCT_INPUT input, VRLEN l_input, pCT_OUTPUT output, VRLEN l_output, cpVOID extra);
NINT ctl_local(pCT_INPUT input, VRLEN l_input, pCT_OUTPUT output, VRLEN l_output, cpVOID extra);
#endif /* ~ctFeatRTG_LOGWHOFORMAT */
#endif /* ~ctFeatRTG_HANDLE */

#endif /* ~_CTCBLOCL_H_ */

/* end of ctcblocl.h */
