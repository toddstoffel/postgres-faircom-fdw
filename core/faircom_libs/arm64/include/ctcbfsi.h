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

#ifndef _CTCBFSI_H_
#define _CTCBFSI_H_

#include "ctcboptn.h"
#define USE_API_ENTRYPOINT
#include "ctcbapi.h"
#include "ctcbacu.h"
#include "ctcblog.h"
#include "ctcbstdr.h"
#include "ctcbthrd.h"

#include <stdio.h>
#include <string.h>

#define MAX_PARMS 1024

typedef struct {
	CT_INIT	*conn;
	NINT	status;
	pVOID	pointer;
	short	_errno;
	long	_int_errno;
	long	_int2_errno;
	short	_no_lock;
	char	*_errmsg;
	short	_implied_transaction;
	short	_in_commit;
	short	_in_transaction;
	NINT	taskcnt;
	MUTEX	taskmtx;
	NINT	taskext;
} MT_INIT;

typedef struct {
	long	not_used;
	CT_FILE	*file;
} ACU_FILE;

#ifdef ctThrds
#define MT ct_connection[hThread]
#else /* ~ctThrds */
#define MT ct_connection
#endif /* ~ctThrds */

#ifdef ctThrds
#define INVALID_THREAD (hThread < 0 || hThread >= MAX_THREADS ||!ct_connection || !MT || !MT->conn)
#define VALIDATE_THREAD(fn, a) \
	{ \
		if (INVALID_THREAD) { \
			CLogError(NULL, NULL, CTE_INTERFACE, CTE_INTERNAL_ERR, 0, (LogBuf, "invalid thread detected: %d", hThread)); \
			a; \
		} \
	}
#else /* ~ctThrds */
#define INVALID_THREAD (!MT || !MT->conn)
#define VALIDATE_THREAD(fn, a) if (INVALID_THREAD) a;
#endif /* ~ctThrds */
#define VALID_THREAD !INVALID_THREAD

#define VALIDATE_PARAMETER(p,f,a) \
	{ \
		if (p == NULL) { \
			CT_ERROR err; \
			CLogError(NULL, &err, E_PARAM_ERR, CTE_INTERNAL_ERR, 0, (LogBuf, "invalid parameter detected: %s", f)); \
			ct_geterr(&f_errno, &f_int_errno, &f_int2_errno, NULL, &err, NULL); \
			a; \
		} \
	}

VOID ct_geterr(short *_errno, long *_int_errno, long *_int2_errno, char *_errmsg, CT_ERROR *err, pTEXT msg);

#define INTF fsienv.intf

#define SERIALIZE_THREAD(a) if (!ct_serthrd(hThread, INTF)) a;
#define UNSERIALIZE_THREAD(a) if (!ct_unserthrd(hThread, INTF)) a;

int ct2fsi(NINT hThread, NINT n, INTFTYPE intf);
void *ct2fsip(NINT hThread, void *p, INTFTYPE intf);


#ifdef ctThrds
#define RETURN(n) ct2fsi(hThread, n, INTF)
#define RETURN_PNT(p) ct2fsip(hThread, p, INTF)
#else /* ~ctThrds */
#define RETURN(n) ct2fsi(-1, n, INTF)
#define RETURN_PNT(p) ct2fsip(-1, p,INTF)
#endif /* ~ctThrds */

#define ERROR_RESET f_int_errno = f_int2_errno = f_errno = 0

/* Context values for ct_commit() and ct_rollback() functions */
#define	COMMIT_CONTEXT_PROGRAMMED	0	/* Indicates that commit is being called in order to end an explicitly programmed transaction. */
#define	COMMIT_CONTEXT_UNKNOWN		1	/* Indicates that commit is being called in order to end an implied transaction. */
#define	COMMIT_CONTEXT_NO_TRX		2	/* Indicates that commit is being called outside of a transaction. */

#endif /* ~_CTCBFSI_H_ */

/* end of ctcbfsi.h */
