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

#ifndef __CONFIG_TEST_C__
#include "ctstdr.h"
#include "ctoptn.h"
#endif

#if defined(ctPortWIN64)
#include "config.h.x64"

#elif defined(ctPortWIN32)
#include "config.h.w32"

#elif defined(ctPortMACOSX_64)
#include "config.h.unx" /*  "config.h.osx64" */

#elif defined(ctPortMACOSX)
#include "config.h.unx" /*  "config.h.osx32" */

#elif defined(ctPortLINUX_RISC)
#include "config.h.unx" /*  "config.h.linux_risc_64" */

#elif defined(ctPortLINUX_64)
#include "config.h.unx" /*  "config.h.linux26_64" */

#elif defined(ctPort_LINUX_KERNEL_V2_4)
#include "config.h.unx" /*  "config.h.linux24_32" */

#elif defined(ctPortLINUX)
#include "config.h.unx" /*  "config.h.linux26_32" */

#elif defined(ctPortRS6000_64)
#include "config.h.unx" /*  "config.h.aix7164" */

#elif defined(ctPortRS6000)
#include "config.h.unx" /*  "config.h.aix7132" */

#elif defined(ctPortSOLARIS_V5_9_OR_BEFORE)
#include "config.h.unx" /*  "config.h.sol0832" */

#elif defined(ctPortSOLARIS8)
#include "config.h.unx" /*  "config.h.sol0832" */

#elif defined(ctPortSOL386)
#include "config.h.unx" /*  "config.h.sol1132" */

#elif defined(ctPortSOLARIS_64)
#include "config.h.unx" /*  "config.h.sol1164" */

#elif defined(ctPortSOLARIS)
#include "config.h.unx" /*  "config.h.sol1132" */

#elif defined(ctPortHP9000_ITANIUM)
#include "config.h.unx" /*  "config.h.hpia64" */

#elif defined(ctPortHP9000_64)
#include "config.h.unx" /*  "config.h.hp964" */

#elif defined(ctPortHP9000)
#include "config.h.unx" /*  "config.h.hp932" */

#elif defined(ctPortSCO5)
#include "config.h.unx" /*  "config.h.sco5_32" */

#elif defined(ctPortQNX)
#include "config.h.unx" /*  "config.h.qnxrtp_32" */

#elif defined(ctPortQNXRTP)
#include "config.h.unx" /*  "config.h.qnxrtp_32" */

#else
#error json-c-0.13 config.h has not been set up for this platform.
#endif

/* Platform help:

#ifdef ctPort386BSD
#ifdef ctPort88OPEN
#ifdef ctPortAIX_5_2
#ifdef ctPortAIX_5_3
#ifdef ctPortALPHA
#ifdef ctPortANDROID
#ifdef ctPortAPPLE7
#ifdef ctPortAPPLEAUX
#ifdef ctPortARM_GENERIC
#ifdef ctPortBATA_SCO5
#ifdef ctPortCHORUS
#ifdef ctPortHP9000
#ifdef ctPortHP9000_64
#ifdef ctPortHP9000_ITANIUM
#ifdef ctPortITANIUM_IA64
#ifdef ctPort_LINUX_KERNEL_V2_4
#ifdef ctPortLINUX
#ifdef ctPortLINUX_64
#ifdef ctPortLINUX_ARMROBOT1_32
#ifdef ctPortLINUX_PREV2_036
#ifdef ctPortLINUX_RISC
#ifdef ctPortLINUX_S390
#ifdef ctPortLYNXOS386
#ifdef ctPortMACLNX
#ifdef ctPortMACOSX
#ifdef ctPortMACOSX_10_3
#ifdef ctPortMACOSX_64
#ifdef ctPortMACOSX_ARM64
#ifdef ctPortMIPSABI
#ifdef ctPortNETBSD386
#ifdef ctPortNLM
#ifdef ctPortQNX
#ifdef ctPortQNXRTP
#ifdef ctPortRASPBIAN
#ifdef ctPortRS6000
#ifdef ctPortRS6000_64
#ifdef ctPortRS6_SYSV_REL_3
#ifdef ctPortSCO386
#ifdef ctPortSCO5
#ifdef ctPortSGI
#ifdef ctPortSOL386
#ifdef ctPortSOLARIS
#ifdef ctPortSOLARIS10
#ifdef ctPortSOLARIS11
#ifdef ctPortSOLARIS_64
#ifdef ctPortSOLARIS_V5_9_OR_BEFORE
#ifdef ctPortSPARC_LINUX
#ifdef ctPortSUN41
#ifdef ctPortW64_ITANIUM_AMD

*/
/* end of config.h */
