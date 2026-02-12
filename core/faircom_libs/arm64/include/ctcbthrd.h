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

#ifndef _CTCBTHRD_H_
#define _CTCBTHRD_H_

#include "ctcboptn.h"

#ifdef ctPortHP9000
/* WORKAROUND: manually #define _POSIX_THREADS on HP-UX platforms
 * since it is not defined by default. The following is a comment
 * found in sys/unistd.h of HP-UX:
	 * The following defines are specified in the standard, but are not yet
	 * implemented:
	 *
	 *        _POSIX_THREADS                     can't be defined until all
	 *                                           features are implemented
 * The implementation seems however to be sufficiently complete
 * to permit compiling and running with _POSIX_THREADS. */
#ifndef _POSIX_THREADS
#define _POSIX_THREADS
#endif
#endif

#if defined(ctPortSCO386) && (__OPENSERVER__ <= 507)
/* SCO OSR5 does not support threading therefore we are assuming
 * that Micro Focus COBOL runtime for this platform is singlethreaded. */
#define ctFeatCOBOL_SINGLETHREADED_RUNTIME
#elif defined(ctCobolACU)
/* ACUCOBOL runtime is singlethreaded. */
#define ctFeatCOBOL_SINGLETHREADED_RUNTIME
#endif

#ifdef _POSIX_THREADS
#include <pthread.h>
typedef pthread_mutex_t MUTEX;
#elif defined(_MT)
#ifdef ctFeatCOBOL_USE_CRITICAL_SECTION
typedef CRITICAL_SECTION MUTEX;
#else
typedef HANDLE MUTEX;
#endif /* ~ctFeatCOBOL_USE_CRITICAL_SECTION */
#elif defined(ctFeatCOBOL_SINGLETHREADED_RUNTIME)
typedef int MUTEX;
#else
#error Multithreading API not supported.
#endif

#ifdef PTHREAD_MUTEX_INITIALIZER
/* Initializing mutex at declaration, so no init is needed at runtime. */
#define ctFeatRTG_STATIC_MUTEX_INIT
#endif

#ifdef PROTOTYPE
int ctcbThrdMtxInit (MUTEX *mtx);
int ctcbThrdMtxGet (MUTEX *mtx);
int ctcbThrdMtxRel (MUTEX *mtx);
int ctcbThrdMtxClose (MUTEX *mtx);
int ctcbThrdHandle (void);
void ctcbThrdSleep (unsigned int msec);
#else
int ctcbThrdMtxInit (mtx);
int ctcbThrdMtxInit (mtx);
int ctcbThrdMtxGet (mtx);
int ctcbThrdMtxRel (mtx);
int ctcbThrdMtxClose (mtx);
int ctcbThrdHandle ();
void ctcbThrdSleep (msec);
#endif

#endif /* ~_CTCBTHRD_H_ */

/* end of ctcbthrd.h */
