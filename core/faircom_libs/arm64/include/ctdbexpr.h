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

/*^*****************************************************************************/

#ifndef _REGEXP_H_
#define _REGEXP_H_

/******************************************************************************\
*
*	Header files
*
\******************************************************************************/
#include "ctdbsdk.h"
#include "ctdbport.h"
#include <ctype.h>

#ifndef ctOPNSRCH
#include "ctopnsrc.h"		/* c-tree Open Source COntroll Header - inventory of all Open SOurce Projects used by FairCom */
#endif

#ifdef ctOPNSRC_REGCOMP_REGEXEC	/* regcomp and regexec - Copyright (c) 1986 by University of Toronto. Written by Henry Spencer - Permission is granted to anyone to use this software for any purpose */
/*
 * regcomp and regexec
 *
 *	Copyright (c) 1986 by University of Toronto.
 *	Written by Henry Spencer.  Not derived from licensed software.
 *
 *	Permission is granted to anyone to use this software for any
 *	purpose on any computer system, and to redistribute it freely,
 *	subject to the following restrictions:
 *
 *	1. The author is not responsible for the consequences of use of
 *		this software, no matter how awful, even if they arise
 *		from defects in it.
 *
 *	2. The origin of this software must not be misrepresented, either
 *		by explicit claim or by omission.
 *
 *	3. Altered versions must be plainly marked as such, and must not
 *		be misrepresented as being the original software.
 ***
 *** THIS IS AN ALTERED VERSION.  It was altered by John Gilmore,
 *** hoptoad!gnu, on 27 Dec 1986, to add \n as an alternative to |
 *** to assist in implementing egrep.
 ***
 *** THIS IS AN ALTERED VERSION.  It was altered by John Gilmore,
 *** hoptoad!gnu, on 27 Dec 1986, to add \< and \> for word-matching
 *** as in BSD grep and ex.
 ***
 *** THIS IS AN ALTERED VERSION.  It was altered by John Gilmore,
 *** hoptoad!gnu, on 28 Dec 1986, to optimize characters quoted with \.
 ***
 *** THIS IS AN ALTERED VERSION.  It was altered by James A. Woods,
 *** ames!jaw, on 19 June 1987, to quash a regcomp() redundancy.
 ***
 *** THIS IS AN ALTERED VERSION. It was altered by Ricardo Machado
 *** on 21 Jul 2000, to add a new interface for FairCom CTDB SDK
 *** the header file was rename to ctdbexpr.h and the source file
 *** regexp.c was renamed to ctdbexpr.c
 ***
 *** THIS IS AN ALTERED VERSION. It was altered by Jon Klaas
 *** on 12 Sep 2023, to mark symbols for optional export linkage.
 ***
 */


/******************************************************************************\
*
*	Guard against C++ compilers
*
\******************************************************************************/
#ifdef __cplusplus
extern "C" {
#endif
/******************************************************************************\
*
*	Regular expression
*
\******************************************************************************/
#define NSUBEXP  30

enum RXERR
{
	RXERR_NONE,				/*  0 - No error detected */
	RXERR_NULL,				/*  1 - NULL pointer passed */
	RXERR_TOOBIG,			/*  2 - Regular expression too big */
	RXERR_NOMEMORY,			/*  3 - Out of heap space */
	RXERR_TOOMANYPAR,		/*  4 - Too many parenthesis () */
	RXERR_MISSINGPAR,		/*  5 - Unmatched parenthesis () */
	RXERR_JUNK,				/*  6 - Junk on end of expression */
	RXERR_EMPTY,			/*  7 - *+ operand could be empty */
	RXERR_NESTED,			/*  8 - nested *?+ */
	RXERR_INVRANGE,			/*  9 - invalid [] range */
	RXERR_MISSINGBRACKET,	/* 10 - Unmatched [] bracket */
	RXERR_NOTHING,			/* 11 - ?+* follows nothing */
	RXERR_TRAILING,			/* 12 - Trailing \\ */
	RXERR_CORRUPTED,		/* 13 - corrupted program */
	RXERR_BADMEMORY,		/* 14 - memory corruption */
	RXERR_BADPOINTER,		/* 15 - corrupted pointer */
	RXERR_BADOPCODE,		/* 16 - corrupted opcode */
	RXERR_NOMATCH,			/* 17 - No matched string */
	RXERR_SIZE,				/* 18 - Size of destination string too small */
	RXERR_INDEX,			/* 19 - Invalid index */

	RXERR_INTERNAL			/* Internal error detected */
};

typedef struct _regexp	_regexp;

typedef struct regexp
{
	_regexp* prog;
	char*	Text[ NSUBEXP ];
	int	reg_err;
	char	*reg_parse;
	int	reg_npar;
	char	reg_dummy;
	char	*reg_code;
	long	reg_size;
	const char *reg_input;
	char	*reg_bol;
	const char **reg_startp;
	const char **reg_endp;

} regexp;

ctdbEXPORT regexp* ctdbDECL ctdb_regnew( void );
ctdbEXPORT void ctdbDECL ctdb_regfree( regexp* regx );
ctdbEXPORT int ctdbDECL ctdb_regcomp( regexp* regx, char* string );
ctdbEXPORT int ctdbDECL ctdb_regexec( regexp* regx, const char* string );
ctdbEXPORT char* ctdbDECL ctdb_regtext( regexp* regx, int index );
ctdbEXPORT int ctdbDECL ctdb_regerror( regexp* regx );

/******************************************************************************/
#ifdef __cplusplus
}
#endif
/******************************************************************************/
#endif /* ctOPNSRC_REGCOMP_REGEXEC */

#endif /* _REGEXP_H_ */
/******************************************************************************/

/* end of ctdbexpr.h */

