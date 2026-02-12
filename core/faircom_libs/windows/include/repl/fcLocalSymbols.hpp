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

#ifndef _FCLOCALSYMBOLS_HPP_
#define _FCLOCALSYMBOLS_HPP_

#include "fcaux.hpp"

void *fcGetLocalServerLibrary(char **errorMsg);
int fcLoadLocalServerSymbols(void *libHandle, char **errorMsg);
#ifdef ctFeat_CTDBSAPP_LIB
void *fcGetLocalServerAppLibrary(char **errorMsg);
#endif

#undef  __
#ifdef PROTOTYPE
#define __(args)        args
#else
#define __(args)        ()
#endif

/* Local c-tree symbol indexes */
#define	CTFN_ctplugin_get             0
#define	CTFN_ctplugin_trylock         1
#define	CTFN_ctplugin_release         2

#define CTFN_TOTAL                    3

/* Local c-tree symbol types */
typedef int (*pctplugin_get_t) __((const char *, int, void **));
typedef int (*pctplugin_trylock_t) __((void));
typedef void (*pctplugin_release_t) __((void));

/* Local c-tree symbol pointer array */
extern void *ctfnclocal[CTFN_TOTAL];

/* Local c-tree symbol pointer defines */
#define	l_ctplugin_get     (*(pctplugin_get_t)ctfnclocal[CTFN_ctplugin_get])
#define	l_ctplugin_trylock (*(pctplugin_trylock_t)ctfnclocal[CTFN_ctplugin_trylock])
#define	l_ctplugin_release (*(pctplugin_release_t)ctfnclocal[CTFN_ctplugin_release])

#endif /* _FCLOCALSYMBOLS_HPP_ */