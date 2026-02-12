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

#ifndef _CTPLATFORM_H_
#define _CTPLATFORM_H_

#ifndef _LINUX
#define _LINUX
#endif

#include <unistd.h>
#include <sys/times.h>
#include <pthread.h>
#include <time.h>
#include <stdint.h>
#include <errno.h>

#define FCAPI
#define FC_FUNC_RET void *
#define FCULONG unsigned long
#define FCULONG8 unsigned long long
#define FCLLu "%llu"
#define FC_WAIT_TIMEOUT EBUSY
typedef long long TIMER;
#define CURRTIME time( NULL )
typedef time_t _TIMER;
#define localtime localtime

#define FCBigInt int64_t

#define FCThreadID pthread_t

#define GetCurrentDir getcwd

#ifdef ctPortMACOSX
#ifdef ctCobolIS
#define CT_DYNLIB_EXT ".jnilib"
#define CT_CLIENT_DYN_LIB "libmtclient.jnilib"
#define CT_SERVER_DYN_LIB "libctreedbs.jnilib"
#define CT_SERVER_APP_DYN_LIB "libctdbsapp.jnilib"
#define CT_SQL_DYN_LIB "libctsqlapi.jnilib"
#else
#define CT_DYNLIB_EXT ".dylib"
#define CT_CLIENT_DYN_LIB "libmtclient.dylib"
#define CT_SERVER_DYN_LIB "libctreedbs.dylib"
#define CT_SERVER_APP_DYN_LIB "libctdbsapp.dylib"
#define CT_SQL_DYN_LIB "libctsqlapi.dylib"
#endif
#else
#define CT_DYNLIB_EXT ".so"
#define CT_CLIENT_DYN_LIB "libmtclient.so"
#define CT_SERVER_DYN_LIB "libctreedbs.so"
#define CT_SERVER_APP_DYN_LIB "libctdbsapp.so"
#define CT_SQL_DYN_LIB "libctsqlapi.so"
#endif
#define CT_APP_EXT ""
#define CT_MAIN_INIT_LIB "ctsrvrmaininit"

#endif /* _CTPLATFORM_H_ */