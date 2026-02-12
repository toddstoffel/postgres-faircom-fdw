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

 /**
  * @file ctcbutil.h
  * @brief An API to perform utility operations on RTG files.
  * @author FairCom Corporation
  * @date 2021/06/28
  */

#ifndef _CTCBUTIL_H_
#define _CTCBUTIL_H_

#include "ctcboptn.h"
#include "ctcbapi.h"
#include "ctcbjson.h"
#ifdef ctCobolBTRV
#include "ctcbbtrv.h"
#endif

typedef struct tagRTGUTIL RTGUTIL, * pRTGUTIL;
struct tagRTGUTIL {
	json_object *jroot;
	pCT_INIT conn;
	pRTGCONF conf;
	CT_ERROR err;
	TEXT errmsg[F_ERRMSG_LEN];
};

/**
 * Allocate the RTGUTIL handle
 *
 * @return - a pointer to the RTGUTIL handle
 */
cbDLLexport pRTGUTIL rtgUtilNew(VOID);
/**
 * Initialize the RTGUTIL operation
 *
 * @param handle - the pointer to the RTGUTIL handle returned by rtgUtilNew()
 * @param configfile - the string with the RTG configuration file path
 * @param nodename - the string with the RTG client node name
 * @return - the string in JSON format with the operation result
 */
cbDLLexport pCT_INIT rtgUtilInit(pRTGUTIL handle, cpTEXT configfile, cpTEXT nodename);
/**
 * Execute the RTGUTIL operation
 *
 * @param handle - the pointer to the RTGUTIL handle returned by rtgUtilNew()
 * @param input - the string in JSON format with the operation data
 * @param configfile - the string with the RTG configuration file path
 * @return - the string in JSON format with the operation result
 */
cbDLLexport cpTEXT rtgUtilExec(pRTGUTIL handle, cpTEXT input, cpTEXT configfile);
/**
 * Terminate the RTGUTIL operation
 *
 * @param handle - the pointer to the RTGUTIL handle returned by rtgUtilNew()
 */
cbDLLexport VOID rtgUtilExit(pRTGUTIL handle);
/**
  * Free the RTGUTIL handle
  *
  * @param handle - a pointer to the RTGUTIL handle
  */
cbDLLexport VOID rtgUtilFree(pRTGUTIL handle);

#endif	/* _CTCBUTIL_H_ */

/* end of ctcbutil.h */
