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

#ifndef _CTCBXMLI_H_
#define _CTCBXMLI_H_

#include "ctcbtype.h"

typedef struct {
	TEXT _type;
	TEXT _mode;
#ifdef ctFeatRTG_NULLKEY
	TEXT _nulval;
#endif

	UTEXT	type;
	UTEXT	mode;
#ifdef ctFeatRTG_NULLKEY
	UTEXT	nulval;
#endif
} RESSEG, *pRESSEG;

typedef struct {
	TEXT _dup;

	LONG	dup;
	UCOUNT	nsegs;
#ifdef ctFeatRTG_KEYS
	UCOUNT	maxsegs;
	pRESSEG	seg;
#else /* ~ctFeatRTG_KEYS */
	RESSEG	seg[CT_MAX_SEGS];
#endif /* ~ctFeatRTG_KEYS */
#ifdef ctFeatRTG_KEYNUMBER
	UCOUNT	keynum;
#endif
} RESKEY, *pRESKEY;

typedef enum {
	RESINFO_RTGI_NONE = 0,
	RESINFO_RTGI_XML = 1,
#ifdef ctFeatRTG_ATTRAPI
	RESINFO_RTGI_ATTRIBUTES = 2,
#endif
	RESINFO_RTGI_INVALID_LAST
} RTGIVER;

typedef struct {
	RTGIVER __rtgi;
	TEXT	__cobf;
	TEXT	__cobn;

	TEXT	_minlen;
	TEXT	_maxlen;
	TEXT	_blockfact;
	TEXT	_prealloc;
	TEXT	_extension;
	TEXT	_compress;
	TEXT	_comment;
	TEXT	_convention;
	TEXT	_rechdr;
	TEXT	_srlidx;
	TEXT	_dupscanwa;
	TEXT	_srlsegsiz;
	TEXT	_XDDrechdrmap;
#ifdef ctFeatRTG_ATTRAPI
	TEXT    _XDDxmllen;
#endif
#ifdef ctFeatRTG_KEYNUMBER
	TEXT	_keynum;
#endif

	VRLEN	minlen;
	VRLEN	maxlen;
	LONG	blockfact;
	LONG	prealloc;
	LONG	extension;
	TEXT	compress;
	TEXT	comment[CT_COMMENT_LEN];
	UCOUNT	nkeys;
#ifdef ctFeatRTG_KEYS
	UCOUNT	maxkeys;
	pRESKEY	key;
#else /* ~ctFeatRTG_KEYS */
	RESKEY	key[CT_MAX_KEYS];
#endif /* ~ctFeatRTG_KEYS */
	TEXT	convention;
	UTEXT	rechdr;
	TEXT	srlidx;
	TEXT	dupscanwa;
	ULONG	version; /* bits required in server structver to be able to act on the file */
	TEXT	srlsegsiz;
	TEXT	XDDrechdrmap; /* how record header is mapped: 0 - ctdb field (hidden in SQL), 1 - CTDB internal fields */
#ifdef ctFeatRTG_ATTRAPI
	ULONG	XDDxmllen;
	CTATTR_HANDLE TblHnd;
#endif
} RESINFO, *pRESINFO;

VOID RESINFOfree(pRESINFO info);
pRESINFO RESINFOalloc(UCOUNT nkeys, UCOUNT nsegs);
VOID RESINFOzero(pRESINFO info);

NINT ctxml2RESINFO(pCTXML hXML, pRESINFO out);
VOID ctRESINFO2Text(pTEXT dst, size_t *dstlen, pRESINFO in);
size_t ctxmlGetText(pCTXML XDD, pTEXT buffer, size_t size);

#endif /* ~_CTCBXMLI_H_ */

/* end of ctcbxmli.h */
