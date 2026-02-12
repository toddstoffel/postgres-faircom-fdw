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

/*
This header file intent is to inventory and identify the Open Source's used by
FairCom within it's technology. We have added the ability to turn ON/OFF
various support in order to control what in included and what is to be excluded
*/


#ifndef ctOPNSRCH
#define ctOPNSRCH

/*^*****************************************************************************/
/* A Bison parser, made by GNU Bison 2.4.2.  */
/* Skeleton interface for Bison's Yacc-like parsers in C

      Copyright (C) 1984, 1989-1990, 2000-2006, 2009-2010 Free Software
   Foundation, Inc.

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.

   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */
#define ctOPNSRC_GNU_Bison_2_4_2 /* A Bison parser, made by GNU Bison 2.4.2. - Skeleton interface for Bison's Yacc-like parsers in C - Copyright (C) 1984, 1989-1990, 2000-2006, 2009-2010 Free Software Foundation, Inc. */

/*~*****************************************************************************/


/*^*****************************************************************************/
/*
File:	ConvertUTF.h
File:	ConvertUTF.C
Author: Mark E. Davis
 * Copyright 1994-1999 IBM Corp.. All rights reserved.
 *
 * IBM Corp. grants the user a nonexclusive right and license to use, execute,
 * display, modify, and prepare and/or have prepared derivative works of the
 * code, including but not limited to creating software products which
 * incorporate the code or derivative works thereof.
 *
 * IBM Corp. grants the user a right and license to reproduce and distribute
 * the code as long as this entire copyright notice is reproduced in the code
 * or reproduction.
 *
 * The code is provided AS-IS, and IBM Corp. disclaims all warranties,
 * either express or implied, including, but not limited to implied
 * warranties of merchantability and fitness for a particular purpose.
 * In no event will IBM Corp. be liable for any damages whatsoever (including,
 * without limitation, damages for loss of business profits, business
 * interruption, loss of business information, or other pecuniary
 * loss) arising out of the use or inability to use this code, even
 * if IBM Corp. has been advised of the possibility of such damages.
 * Because some states do not allow the exclusion or limitation of
 * liability for consequential or incidental damages, the above
 * limitation may not apply to you.
*/

#define ctOPNSRC_ConvertUTF	/* ConvertUTF.C and ConvertUTF.H - Copyright 1994-1999 IBM Corp.. All rights reserved. Author: Mark E. Davis */

/*~*****************************************************************************/


/*^*****************************************************************************/
/******************************************************************************\
*
*	MD5 - Message Digest Algorithm
*
*	Copyright (C) 1991, RSA Data Security, Inc. All rights reserved.
*
*	License to copy and use this software is granted provided that it
*	is identified as the "RSA Data Security, Inc. MD5 Message-Digest
*	Algorithm" in all material mentioning or referencing this software
*	or this function.
*
*	License is also granted to make and use derivative works provided
*	that such works are identified as "derived from the RSA Data
*	Security, Inc. MD5 Message-Digest Algorithm" in all material
*	mentioning or referencing the derived work.
*
*	RSA Data Security, Inc. makes no representations concerning either
*	the merchantability of this software or the suitability of this
*	software for any particular purpose. It is provided "as is"
*	without express or implied warranty of any kind.
*
*	These notices must be retained in any copies of any part of this
*	documentation and/or software.
*
*	Digest size is 16 bytes long
*
\******************************************************************************/
#define ctOPNSRC_RSA_MD5	/* MD5 - Message Digest Algorithm - Copyright (C) 1991, RSA Data Security, Inc. All rights reserved.*/

/*~*****************************************************************************/


/*^*****************************************************************************/
/******************************************************************************\
*
*	DES
*
*	DES encryption algorithm based on description published by
*	Bruce Schneier,	Applied Cryptography 2nd Edition, ISBN 0-471-12845-7
*
\******************************************************************************/
#define ctOPNSRC_DES_ENCRYPTION	/* DES encryption algorithm based on description published by Bruce Schneier,	Applied Cryptography 2nd Edition, ISBN 0-471-12845-7 */



/*~*****************************************************************************/
/******************************************************************************\
*
*	Blowfish
*
*	Blowfish encryption algorithm implementation based on code made public by
*	Bruce Schneier of Counterpane Internet Security Inc.
*	Please refer to www.counterpane.com/blowfish.html
*	Quote from Counterpane web site about Blowfish:
*	"Blowfish is unpatented and license-free, and is available free
*	for all uses. "
*
\******************************************************************************/
#define ctOPNSRC_BLOWFISH	/* Blowfish encryption algorithm implementation based on code made public by Bruce Schneier of Counterpane Internet Security Inc. */
/*~*****************************************************************************/


/*^*****************************************************************************/

/******************************************************************************\
*
*	Rijndael
*
*	Rijndael encryption algorithm implementation based on code made public by
*	the Rijndael web page as one of NIST AES finalist
*	Please refer to http://www.esat.kuleuven.ac.be/~rijmen/rijndael/
*	Quote from Rijndael web site:
*	"Rijndael is available for free. You can use it for whatever purposes
*	you want, irrespective of whether it is accepted as AES or not. "
*
\******************************************************************************/

/*
// Copyright in this code is held by Dr B.R. Gladman but free direct or
// derivative use is permitted subject to acknowledgement of its origin
// and subject to any constraints placed on the use of the algorithm by
// its designers (if such constraints may exist, this will be indicated
// below).
//
// Dr. B. R. Gladman (brian.gladman@btinternet.com). 25th January 2000.
//
// This is an implementation of Rijndael, an encryption algorithm designed
// by Daemen and Rijmen and submitted as a candidate algorithm for the
// Advanced Encryption Standard programme of the US National Institute of
// Standards and Technology.
//
// The designers of Rijndael have not placed any constraints on the use of
// this algorithm.
*/

#define ctOPNSRC_RIJNDAEL	/* Rijndael encryption algorithm implementation based on code made public by the Rijndael web page as one of NIST AES finalist  */

/*~*****************************************************************************/


/*^*****************************************************************************/
/******************************************************************************\
*
*	Twofish
*
*	Twofish encryption algorithm implementation based on code made public by
*	Counterpane Internet Security Inc, as one of the NIST AES finalist
*	Please refer to www.counterpane.com/twofish.html
*	Quote from Counterpane web site about Twofish:
*	"Twofish is unpatented, and the source code is uncopyrighted and
*	license-free; it is free for all uses. "
*
\******************************************************************************/
#define ctOPNSRC_TWOFISH	/* Twofish encryption algorithm implementation based on code made public by Counterpane Internet Security Inc, as one of the NIST AES finalist  */

/*~*****************************************************************************/

/*^*****************************************************************************/
/* This is an independent implementation of the encryption algorithm:   */
/*                                                                      */
/*         Twofish by Bruce Schneier and colleagues                     */
/*                                                                      */
/* which is a candidate algorithm in the Advanced Encryption Standard   */
/* programme of the US National Institute of Standards and Technology.  */
/*                                                                      */
/* Copyright in this implementation is held by Dr B R Gladman but I     */
/* hereby give permission for its free direct or derivative use subject */
/* to acknowledgment of its origin and compliance with any conditions   */
/* that the originators of the algorithm place on its exploitation.     */
/*                                                                      */
/* My thanks to Doug Whiting and Niels Ferguson for comments that led   */
/* to improvements in this implementation.                              */
/*                                                                      */
/* Dr Brian Gladman (gladman@seven77.demon.co.uk) 14th January 1999     */
#define ctOPNSRC_TWOFISH_2	/* Twofish by Bruce Schneier and colleagues - Copyright in this implementation is held by Dr B R Gladman */

/*~*****************************************************************************/

/*^*****************************************************************************/
/*
 * regcomp and regexec -- regsub and regerror are elsewhere
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
 *** THIS IS AN ALTERED VERSION.  It was altered by John Gilmore,
 *** hoptoad!gnu, on 27 Dec 1986, to add \n as an alternative to |
 *** to assist in implementing egrep.
 *** THIS IS AN ALTERED VERSION.  It was altered by John Gilmore,
 *** hoptoad!gnu, on 27 Dec 1986, to add \< and \> for word-matching
 *** as in BSD grep and ex.
 *** THIS IS AN ALTERED VERSION.  It was altered by John Gilmore,
 *** hoptoad!gnu, on 28 Dec 1986, to optimize characters quoted with \.
 *** THIS IS AN ALTERED VERSION.  It was altered by James A. Woods,
 *** ames!jaw, on 19 June 1987, to quash a regcomp() redundancy.
 *** THIS IS AN ALTERED VERSION. It was altered by Ricardo Machado
 *** on 21 Jul 2000, to add a new interface for FairCom CTDB SDK
 *** THIS IS AN ALTERED VERSION. It was altered by Paolo Fassi
 *** on 19 Jan 2022, to support flexible number of sub expressions (NSUBEXP)
 *
 * Beware that some of this code is subtly aware of the way operator
 * precedence is structured in regular expressions.  Serious changes in
 * regular-expression syntax might require a total rethink.
 */

#define ctOPNSRC_REGCOMP_REGEXEC	/* regcomp and regexec - Copyright (c) 1986 by University of Toronto. Written by Henry Spencer - Permission is granted to anyone to use this software for any purpose */

/*~*****************************************************************************/

/*^*****************************************************************************/
/*
 * Copyright 1985, 1990 by Mortice Kern Systems Inc.  All rights reserved.
 *
 * Automaton to interpret LALR(1) tables.
 *
 * Macros:
 *      yyclearin - clear the lookahead token.
 *      yyerrok - forgive a pending error
 *      YYERROR - simulate an error
 *      YYACCEPT - halt and return 0
 *      YYABORT - halt and return 1
 *      YYRETURN(value) - halt and return value.  You should use this
 *              instead of return(value).
 *      YYREAD - ensure yychar contains a lookahead token by reading
 *              one if it does not.  See also YYSYNC.
 *      YYRECOVERING - 1 if syntax error detected and not recovered
 *              yet; otherwise, 0.
 *
 * Preprocessor flags:
 *      YYDEBUG - includes debug code if 1.  The parser will print
 *               a travelogue of the parse if this is defined as 1
 *               and yydebug is non-zero.
 *              yacc -t sets YYDEBUG to 1, but not yydebug.
 *      YYSSIZE - size of state and value stacks (default 150).
 *      YYLR2 - defined if lookahead is needed to resolve R/R or S/R conflicts
 *      YYSYNC - if defined, yacc guarantees to fetch a lookahead token
 *              before any action, even if it doesnt need it for a decision.
 *              If YYSYNC is defined, YYREAD will never be necessary unless
 *              the user explicitly sets yychar = -1
 *
 * Copyright (c) 1983, by the University of Waterloo
 */
#define ctOPNSRC_MKS_LALR	/* Automaton to interpret LALR(1) tables. - Copyright 1985, 1990 by Mortice Kern Systems Inc.  All rights reserved. */

/*~*****************************************************************************/


/*^*****************************************************************************/
/* JSON C LIB (json-c-0.13)
 *
 * Copyright (c) 2004, 2005 Metaparadigm Pte. Ltd.
 * Michael Clark <michael@metaparadigm.com>
 * Copyright (c) 2009 Hewlett-Packard Development Company, L.P.
 *
 * This library is free software; you can redistribute it and/or modify
 * it under the terms of the MIT license. See COPYING for details.
 *
 */
#define ctOPNSRC_JSON_C		/* JSON C LIB (json-c-0.13) - This library is free software; you can redistribute it and/or modify it under the terms of the MIT license */

/*~*****************************************************************************/

/*^*****************************************************************************/
/*~*****************************************************************************/

/*^*****************************************************************************/
/*~*****************************************************************************/


#endif /* ~ctOPNSRCH */

/* end of ctopnsrc.h */
