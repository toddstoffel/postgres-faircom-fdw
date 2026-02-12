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

#ifndef _CTCBCUST_H_
#define _CTCBCUST_H_

#include "ctcboptn.h"
#include "ctcbremt.h"

#if defined __CTRTG_C
#include "ctcbmshl.h"
/* in RTG driver we use (and so need to activate) RTG_FUNC1 */
/* this also means that we need to provide a function named RTG1 */
/* and a function named get_rtg_func1_name */
#ifndef ctFeatRTG_FUNC1
#define ctFeatRTG_FUNC1
#endif

#define MY_FUNCTIONz RTG1

LOCAL cpTEXT rtg_func_names[] = {
	"", /* function number zero is not used */
	"RTG_MAKE",
	"RTG_OPEN",
	"RTG_CLOSE",
	"RTG_WRITE",
	"RTG_READ",
	"RTG_START",
	"RTG_NEXTPREV",
	"RTG_REWDEL",
	"RTG_DELETE",
	"RTG_UNLOCK",
	"RTG_VERSION",
	"RTG_SQLINFO",
	"RTG_SQLLINK",
	"RTG_COPY",
	"RTG_INFO",
	"RTG_ADDIDX",
	"RTG_REMOVE",
	"RTG_RENAME",
	"deprecated",
	"RTG_FILEWORD",
	"RTG_ACCESS",
	"RTG_ALTER"
};

#define RTG_FUNC_MAX (sizeof(rtg_func_names)/sizeof(*rtg_func_names) - 1)

/*^************************************/
/*
** Get the operation name for the specified RTG function opcode.
*/
cpTEXT get_rtg_func1_name (pTEXT inBuffer)
{
	LONG	fnc;
	cpTEXT	name;

	memcpy(&fnc, inBuffer, sizeof(LONG));

	switch (fnc) {
case FN_RTG1:
		{
		CT_INPUT *input = (CT_INPUT *)inBuffer;

		if (!input) {
			name = "RTG_FUNC_NULL";
			break;
		}

		if (input->function < 1 || input->function > RTG_FUNC_MAX) {
			name = "RTG_FUNC_OUT_OF_RANGE";
			break;
		}

		name = rtg_func_names[input->function];
		break;
		}

default:
		name = "RTG_UNKNOWN";
		break;
	}

	return (name);
}
/*~************************************/

/*^************************************/
/*
** Get the RTG sub function number for the specified RTG function opcode.
*/
#ifdef PROTOTYPE
NINT get_rtg_func1_number (pTEXT inBuffer)
#else
NINT get_rtg_func1_number (inBuffer)
pTEXT inBuffer;
#endif
{
	LONG	fnc;
	NINT	number;

	memcpy(&fnc, inBuffer, sizeof(LONG));

	switch (fnc) {
case FN_RTG1:
		{
		CT_INPUT *input = (CT_INPUT *)inBuffer;

		if (!input) {
			number = 0;
			break;
		}

		if (input->function < 1 || input->function > RTG_FUNC_MAX) {
			number = 0;
			break;
		}

		number = CTI_RTG_FIRST + input->function - 1;
		break;
		}

default:
		number = 0;
		break;
	}

	return (number);
}
/*~************************************/

/*^************************************/
/*
** Get the file name for the specified RTG operation.
*/
pTEXT get_rtg_file1_name(pTEXT inBuffer)
{
	LONG	fnc;
	pTEXT	name = NULL;

	memcpy(&fnc, inBuffer, sizeof(LONG));

	switch (fnc) {
	case FN_RTG1:
	{
		CT_INPUT *input = (CT_INPUT *)inBuffer;

		if (!input) {
			break;
		}

		if (input->function < 1 || input->function > RTG_FUNC_MAX) {
			break;
		}

		switch (input->function) {
		case FUNC_CT_MAKE:
#ifdef ctFeatRTG_KEYS
			if (rtgSTRUCTVERHAS(input->structver, CT_STRUCTVER_KEYS))
				name = ((pCT_MAKE3_INPUT)input)->make.name;
			else
#endif /* ctFeatRTG_KEYS */
			if (input->structver >= CT_STRUCTVER_KEYSEGMODE)
				name = ((pCT_MAKE2_INPUT)input)->make.name;
			else
				name = ((pCT_MAKE1_INPUT)input)->make.name;
			break;
		case FUNC_CT_OPEN:
			name = ((pCT_OPEN2_INPUT)input)->name;
			break;
		case FUNC_CT_SQLLINK:
			if (rtgSTRUCTVERHAS(input->structver, CT_STRUCTVER_STRUCT2019))
				name = ((pCT_SQLLINK2_INPUT)input)->name;
			else
				name = ((pCT_SQLLINK1_INPUT)input)->name;
			break;
		case FUNC_CT_REMOVE:
			name = ((pCT_REMOVE2_INPUT)input)->name;
			break;
		case FUNC_CT_RENAME:
			name = ((pCT_RENAME2_INPUT)input)->fname;
			break;
		case FUNC_CT_ACCESS:
			name = ((pCT_ACCESS_INPUT)input)->path;
			break;
#ifdef ctSRVR
		case FUNC_CT_CLOSE:
		case FUNC_CT_WRITE:
		case FUNC_CT_READ:
		case FUNC_CT_START:
		case FUNC_CT_NEXTPREV:
		case FUNC_CT_REWDEL:
		case FUNC_CT_DELETE:
		case FUNC_CT_UNLOCK:
		case FUNC_CT_SQLINFO:
		case FUNC_CT_COPY:
		case FUNC_CT_INFO:
		case FUNC_CT_ADDIDX:
		{
#ifdef MULTITRD
			NINT		 sOWNR = OWNER;
			pCTGV		 lctgv = ctgv[sOWNR];
#endif
			FILNO fl = -1;

			switch (input->function) {
			case FUNC_CT_CLOSE:
				if (rtgSTRUCTVERHAS(input->structver, CT_STRUCTVER_STRUCT2019))
					fl = ((pCT_CLOSE2_INPUT)input)->filno;
				else
					fl = ((pCT_CLOSE1_INPUT)input)->filno;
				break;
			case FUNC_CT_UNLOCK:
				if (rtgSTRUCTVERHAS(input->structver, CT_STRUCTVER_STRUCT2019))
					fl = ((pCT_UNLOCK2_INPUT)input)->filno;
				else
					fl = ((pCT_UNLOCK1_INPUT)input)->filno;
				break;
			case FUNC_CT_SQLINFO:
				if (rtgSTRUCTVERHAS(input->structver, CT_STRUCTVER_STRUCT2019))
					fl = ((pCT_SQLINFO2_INPUT)input)->filno;
				else
					fl = ((pCT_SQLINFO1_INPUT)input)->filno;
				break;
			case FUNC_CT_COPY:
				fl = ((pCT_COPY_INPUT)input)->fromfilno;
				break;
			case FUNC_CT_INFO:
				fl = ((pCT_INFO_INPUT)input)->filno;
				break;
			case FUNC_CT_ADDIDX:
#ifdef ctFeatRTG_KEYS
				if (rtgSTRUCTVERHAS(input->structver, CT_STRUCTVER_KEYS))
					fl = ((pCT_ADDIDX3_INPUT)input)->filno;
				else
#endif /* ctFeatRTG_KEYS */
				if (rtgSTRUCTVERHAS(input->structver, CT_STRUCTVER_STRUCT2019))
					fl = ((pCT_ADDIDX2_INPUT)input)->filno;
				else
					fl = ((pCT_ADDIDX1_INPUT)input)->filno;
				break;
			case FUNC_CT_WRITE:
			case FUNC_CT_REWDEL:
			case FUNC_CT_DELETE:
				fl = ((CT_COMM_INPUT *)input)->filno;
				break;
			case FUNC_CT_READ:
			case FUNC_CT_START:
			case FUNC_CT_NEXTPREV:
				fl = ((CT_COMM_INPUT *)input)->keyno;
				break;
			case FUNC_CT_ALTER:
			case FUNC_CT_FILEWORD:
				fl = ((pCT_SQLINFO2_INPUT)input)->filno;
				break;
			default:
				break;
			}

			if (fl >= 0 && fl < cthghfil && ct_fusr[sOWNR]) {
				pCTFILE hnum, knum;
				NINT	sys_filno;

				if ((sys_filno = (ct_fusr[sOWNR] +
					fl)->flmp) < 0)
					break;

				if (!(knum = ctfcbhdr[sys_filno]))
					break;

				if (!(hnum = cthkey(knum)))
					break;

				name = hnum->flname;
			}
			break;
		}
#endif
		default:
			break;
		}
		break;
	}

	default:
		break;
	}

	return (name);
}
/*~************************************/

#define RPC_STATE HYS
#else /*__CTRTG_C*/
#define RPC_STATE YES
#endif /*__CTRTG_C*/

#ifndef CTCB_FUNC_NAMES_ONLY

#ifdef PROTOTYPE
LONG MY_FUNCTIONz(pTEXT inBuffer, LONG inBufLen, pTEXT outBuffer, LONG outBufLen, pLONG newOutLen)
#else
LONG MY_FUNCTIONz(inBuffer,inBufLen,outBuffer,outBufLen,newOutLen)
pTEXT	inBuffer;
LONG	inBufLen;
pTEXT	outBuffer;
LONG	outBufLen;
pLONG 	newOutLen;
#endif
{
	LONG myNewOutLen = outBufLen;
	LONG retval;
	inBuffer -= (sizeof(OPC2)-OPC2_CVR_SIZ);
	inBufLen += (sizeof(OPC2)-OPC2_CVR_SIZ);
	retval = cts_remote(inBuffer, inBufLen, outBuffer, outBufLen, newOutLen, RPC_STATE);
	return(retval);
}

#endif /* !CTCB_FUNC_NAMES_ONLY */

#if defined __CTRTG_C
#undef MY_FUNCTIONz
#endif

#endif /* ~_CTCBCUST_H_ */

/* end of ctcbcust.h */
