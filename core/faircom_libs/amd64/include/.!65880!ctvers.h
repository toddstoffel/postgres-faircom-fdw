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

#ifndef __CTVERS_H__
#define __CTVERS_H__

/* ctSIBLING_VERSIONS: Product Version Numbers References */

#define ctVER_MAJOR		13	/* Major version number.	    	*/
#define ctVER_MINOR		0 	/* Minor version number.	    	*/
#define ctVER_MINI		0	/* Mini version number.	    	    	*/

#define ctFDB_MAJOR		13	/* Major version number for FDB.  */
#define ctFDB_MINOR		0 	/* Minor version number for FDB.  */
#define ctFDB_MINI		0	/* Mini version number for FDB.   */

#define ctEDGE_MAJOR		4	/* Major version number for EDGE. */
#define ctEDGE_MINOR		2 	/* Minor version number for EDGE. */
#define ctEDGE_MINI		0	/* Mini version number for EDGE.*/

#define ctRTG_MAJOR		4	/* Major version number for RTG.  */
#define ctRTG_MINOR		2 	/* Minor version number for RTG.  */
#define ctRTG_MINI		0	/* Mini version number for RTG. */

#define ctVER_REVSN		162	/* Product Revision number.	*/
#define ctVER_REAL_REVSN	348279	/* Real SVN Revision number.	*/

#define ctVER_BASE_REVSN	0	/* Base SVN Revision number.	*/
#define ctVER_BASE_REAL_REVSN	0	/* Real Base rev. number (if patched).	*/

#define ctVER_BLDYY		24	/* Build year. */
#define ctVER_BLDMM		05	/* Build month. */
#define ctVER_BLDDD		24	/* Build day. */

#define ctVER_BASE_BLDYY	24	/* Base build year. */
#define ctVER_BASE_BLDMM	05	/* Base build month. */
#define ctVER_BASE_BLDDD	24	/* Base build day.*/

#define ctVER_CYEAR		2024	/* Current year.		    */

#ifdef ctPortEDGE
#define ctDISPLAY_MAJOR		ctEDGE_MAJOR
#define ctDISPLAY_MINOR		ctEDGE_MINOR
#define ctDISPLAY_MINI		ctEDGE_MINI
#else
#ifdef ctPortRTG
#define ctDISPLAY_MAJOR		ctRTG_MAJOR
#define ctDISPLAY_MINOR		ctRTG_MINOR
#define ctDISPLAY_MINI		ctRTG_MINI
#else
#define ctDISPLAY_MAJOR		ctFDB_MAJOR
#define ctDISPLAY_MINOR		ctFDB_MINOR
#define ctDISPLAY_MINI		ctFDB_MINI
#endif
#endif

/*
** Use this macro to check if the major.minor.mini code version is greater than
** or equal to the specified version. For example:
**
** #if ctVER_GE(10,0,1) // if version 10.0.1 and later
*/
#define ctVER3_GE(major_ver,minor_ver,mini_ver) \
	((ctVER_MAJOR > (major_ver)) || \
	 (ctVER_MAJOR == (major_ver) && (ctVER_MINOR > (minor_ver) || \
	 ctVER_MINOR == (minor_ver) && ctVER_MINI >= (mini_ver))))

/*
** Use this macro to check if the code version is greater than or equal to the
** specified version. For example:
**
** #if ctVER_GE(9,6) // if version 9.6 and later
*/
#define ctVER_GE(major_ver,minor_ver) ((ctVER_MAJOR > (major_ver)) || (ctVER_MAJOR == (major_ver) && ctVER_MINOR >= (minor_ver)))

/*
** Use this macro to check if the code version is less than the specified
** version. For example:
**
** #if ctVER_LT(9,6) // if prior to version 9.6
*/
#define ctVER_LT(major_ver,minor_ver) ((ctVER_MAJOR < (major_ver)) || (ctVER_MAJOR == (major_ver) && ctVER_MINOR < (minor_ver)))

#define ctstringify2(x) #x
#define ctstringify(x) ctstringify2(x)
#define ictconcat2(x,y) x ## y
#define ictconcat(x,y) ictconcat2(x,y)

#ifdef UNICODE
#define uctstringify3(x) L ## x
#define uctstringify2(x) uctstringify3(#x)
#define uctstringify(x) uctstringify2(x)
#endif /* UNICODE */

/*^**************************************************/
#ifndef CT_GLOBAL_VERSION
#define CT_GLOBAL_VERSION 	ctstringify(ctVER_MAJOR.ctVER_MINOR.ctVER_MINI.ctVER_REVSN)
#endif
#ifndef CT_GLOBAL_DISPLAY
#define CT_GLOBAL_DISPLAY 	ctstringify(ctDISPLAY_MAJOR.ctDISPLAY_MINOR.ctDISPLAY_MINI.ctVER_REVSN)
#endif
#ifndef CT_GLOBAL_BUILD
#define CT_GLOBAL_BUILD	  	ctstringify(ctVER_BLDDT)
#endif
/*~**************************************************/


/*^**************************************************/
#ifdef UNICODE
#define ctPRODUCT_VERSION_STR_UNI ctstringify(ctDISPLAY_MAJOR) L"." \
	uctstringify(ctDISPLAY_MINOR) L"." uctstringify(ctDISPLAY_MINI) L"." uctstringify(ctVER_REVSN) \
	L"(Build-" uctstringify(ctVER_BLDDT) L")"

#define ctPRODUCT_VERSION_LICENSE_STR_UNI ctstringify(ctVER_MAJOR) L"." \
	uctstringify(ctDISPLAY_MINOR) L"." uctstringify(ctDISPLAY_MINI) L"." uctstringify(ctVER_REVSN) \
	L"(Build-" uctstringify(ctVER_BLDDT) L")"

#endif /* UNICODE */

#define ctPRODUCT_VERSION_STR	ctstringify(ctDISPLAY_MAJOR) "." \
	ctstringify(ctDISPLAY_MINOR) "." ctstringify(ctDISPLAY_MINI) "." ctstringify(ctVER_REVSN) \
	"(Build-" ctstringify(ctVER_BLDDT) ")"

#define ctPRODUCT_VERSION_LICENSE_STR	ctstringify(ctVER_MAJOR) "." \
	ctstringify(ctDISPLAY_MINOR) "." ctstringify(ctDISPLAY_MINI) "." ctstringify(ctVER_REVSN) \
	"(Build-" ctstringify(ctVER_BLDDT) ")"

#define ctPRODUCT_VERSION_NUM	ctDISPLAY_MAJOR, ctDISPLAY_MINOR, ctDISPLAY_MINI, ctVER_REVSN

/*~**************************************************/

