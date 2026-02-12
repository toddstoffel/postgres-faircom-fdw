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
 *      Copyright (c) 2013 - 2024 FairCom Corporation.
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
 */

#ifndef _FCRCESPLATFORM_H_
#define _FCRCESPLATFORM_H_

#ifndef _LINUX
#define _LINUX
#endif

#include <unistd.h>
#include <sys/times.h>
#include <pthread.h>
#include <time.h>
#include <stdint.h>
#include "fc_rces.h"

#define FCAPI
#define FC_FUNC_RET void *
#define FCULONG unsigned long
#define FC_WAIT_TIMEOUT EBUSY

typedef long long TIMER;
#define CURRTIME time( NULL )
typedef time_t _TIMER;

#define FCBigInt int64_t

#define GetCurrentDir getcwd

#define rcesEXPORT
#define rcesIMPORT extern

#ifdef ctPortMACOSX
#ifdef ctCobolIS
#define RCES_DYNLIB_EXT ".jnilib"
#define RCES_CLIENT_DYN_LIB "libmtclient.jnilib"
#define RCES_SERVER_DYN_LIB "libctreedbs.jnilib"
#define RCES_SQL_DYN_LIB "libctsqlapi.jnilib"
#define RCES_CTREPLAGENT_LIBNAME "libctreplagent.jnilib"
#define RCES_MEMGRID_DYN_LIB "librcesmemgrid.jnilib"
#define RCES_DPCTREE_DYN_LIB "librcesdpctree.jnilib"
#else
#define RCES_DYNLIB_EXT ".dylib"
#define RCES_CLIENT_DYN_LIB "libmtclient.dylib"
#define RCES_SERVER_DYN_LIB "libctreedbs.dylib"
#define RCES_SQL_DYN_LIB "libctsqlapi.dylib"
#define RCES_CTREPLAGENT_LIBNAME "libctreplagent.dylib"
#define RCES_MEMGRID_DYN_LIB "librcesmemgrid.dylib"
#define RCES_DPCTREE_DYN_LIB "librcesdpctree.dylib"
#endif
#else
#define RCES_DYNLIB_EXT ".so"
#define RCES_CLIENT_DYN_LIB "libmtclient.so"
#define RCES_SERVER_DYN_LIB "libctreedbs.so"
#define RCES_SQL_DYN_LIB "libctsqlapi.so"
#define RCES_CTREPLAGENT_LIBNAME "libctreplagent.so"
#define RCES_MEMGRID_DYN_LIB "librcesmemgrid.so"
#define RCES_DPCTREE_DYN_LIB "librcesdpctree.so"
#endif
#define RCES_APP_EXT ""
#define RCES_MAIN_INIT_LIB "ctsrvrmaininit"

#endif /* _FCRCESPLATFORM_H_ */