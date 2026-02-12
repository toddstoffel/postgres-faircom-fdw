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

/* LINUX 386 FairCom Threading setup */

/*************************************/
#if (defined(ctSRVR) || defined(ctBNDSRVR))
#ifndef __PTHREAD__
#define ctNoThdParms
#endif
#else
#define ctThrdNoStackParm
#endif
/*************************************/


/*^***********************************/
#ifdef ctPortFAIRCOM_THREADS

#define ctNEED_TID

#ifdef ctSRVR
#define ctTRDDUMP
#endif

#define THRD_BUF	jmp_buf

#define ctThrdTPC	5
#define ctThrdTSP	4

#ifdef  ctPortLINUX_REDHAT_5_2
#define ct_epc	environs[0].__jmpbuf[JB_PC]
#define ct_esp	environs[0].__jmpbuf[JB_SP]
#else
#define ct_epc	environs[0].__pc
#define ct_esp	environs[0].__sp
#endif


#define ctsysnap	usleep

#define OWNER   ((NINT)((Threads_ *)getThreadCtl())->current->refcon)

#endif /* ctPortFAIRCOM_THREADS */
/*~***********************************/

/*^***********************************/
#ifdef ctPortNATIVE_THREADS
#if (defined(ctSRVR) || defined(ctBNDSRVR))

#ifndef	__PTHREAD__
#define	__PTHREAD__
#endif

#ifdef	__PTHREAD__

#define ctNEED_TID
#define	ctKERNAL_HAS_THREADS
#define	ctSTOPSRVR_MAINTHREAD_CLEAR	/* in ctsrvr.c */
/* #define USE_SETCONCURRENCY */	/* setconcurrency() for MP */
#define	USE_NANOSLEEP		/*  nanosleep()  */

#define OWNER   pthread_OWNER()

#define	endThread	ctendthread

#define ctSEMA
#define ctSEMAunxpthd	/* use Unix pthread sema definitions in ctsema.h */

#endif	/* __PTHREAD__ */

#endif /* ctSRVR or ctBNDSRVR */
#endif /* ctPortNATIVE_THREADS */
/*~***********************************/


/*^***********************************/
#ifdef ctPortNATIVE_THREADS
#ifdef ctThrds

#ifdef	JAVA_THREAD

#define ctThrdCriticalIn	mutex_lock(&ctcrit);
#define ctThrdCriticalOut	mutex_unlock(&ctcrit);
extern	mutex_t			ctcrit;
#define CTMUTEX
typedef struct ctmutex {
	mutex_t 	ptmutex;
	sigset_t	smask;
	int		init;
} ctMUTEX;
#define CTBLOCK
typedef struct ctblck {
	mutex_t 	ptmutex;
	cond_t		cond;
	sigset_t	smask;
	int             init;
} ctBLOCK;

#else

#define ctThrdCriticalIn	pthread_mutex_lock(&ctcrit);
#define ctThrdCriticalOut	pthread_mutex_unlock(&ctcrit);
extern	pthread_mutex_t		ctcrit;

#define CTMUTEX
typedef struct ctmutex {
	pthread_mutex_t ptmutex;
	sigset_t	smask;
	int		init;
} ctMUTEX;
#define CTBLOCK
typedef struct ctblck {
	pthread_mutex_t ptmutex;
	pthread_cond_t	cond;
	sigset_t	smask;
	int             init;
} ctBLOCK;

#endif	/* ~JAVA_THREAD */

#endif /* ctThrds */
#endif /* ctPortNATIVE_THREADS */
/*~***********************************/

/* end of ctsthd.h */
