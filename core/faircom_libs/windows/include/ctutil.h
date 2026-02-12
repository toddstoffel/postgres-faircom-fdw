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

#ifndef _CTUTIL_H_
#define _CTUTIL_H_

#include "ctcboptn.h"
#include "ctcbapi.h"
#include "ctcberr.h"
#include "ctcbstdr.h"
#include "ctcbjson.h"

#define INTF utlenv.intf

#define XDDEXT ((INTF == INTF_ISCOBOL) ? "iss" : "xdd")
#define XFDEXT ((INTF == INTF_ISCOBOL) ? "iss" : "xfd")

#define SIGN_ACU "ACUCOBOL-GT"
#define SIGN_MBP "MBP COBOL"
#define SIGN_DG  "Data General"
#define SIGN_IBM "IBM"
#define SIGN_MF  "Micro Focus"
#define SIGN_NCR "NCR COBOL"
#define SIGN_REA "Realia COBOL"
#define SIGN_VAX "VAX COBOL"

enum {
	INFO_PARAM=0,
	INFO_TRON,
	INFO_COMPRESS,
	INFO_FILESIZE,
	INFO_CONV,
	INFO_ALTER,
	INFO_AUGMENT,
	INFO_MAKEIDX,
	INFO_FULL
};

static cpTEXT ctutil_ksegtype[] = {
	"String",
	"Integer",
	"Float",
	"Date",
	"Time",
	"Decimal",
	"Money",
	"Logical",
	"String",
	"Bfloat",
	"LString",
	"ZString",
	"",
	"",
	"Unsigned Binary",
	"Autoincrement",
	"",
	"NumericSTS",
	"NumericSA",
	"Currency",
	"Timestamp",
	"",
	"",
	"",
	"",
	"WString",
	"WZString",
	"GUID",
	"NumericSLB",
	"NumericSLS",
	"",
	"NumericSTB",
	"Null",
	NULL
};

static cpTEXT ctutil_getinterrmsg(NINT num)
{
	NINT i;
	cpTEXT msg;

	for (i = 0, msg = NULL; i < rtgSIZEOFARRAY(ctcb_int_error); i++)
	{
		if (num == ctcb_int_error[i].num)
		{
			msg = ctcb_int_error[i].msg;
			break;
		}
	}
	return msg;
}

/*
**	ctutil commands
*/
typedef enum CTUTIL_COMMAND_ID {
	_CTINVALID = -1,

	CTUTIL_ALTER = 0,
	CTUTIL_INFO,
	CTUTIL_SETOWNER,
	CTUTIL_CLROWNER,
	CTUTIL_EXIST,
	CTUTIL_AUGMENT,
	CTUTIL_PARTITION,
	CTUTIL_MAKE,
	CTUTIL_MAKEIDX,
	CTUTIL_SQLINFO,
	CTUTIL_SQLLINK,
	CTUTIL_SQLUNLINK,
	CTUTIL_SQLIZE,
	CTUTIL_SQLREFRESH,

	_CTPROFILE = 0,
	_CTINFO,
	_CTPARAM,
	_CTSETPATH,
	_CTRESETFN,
	_CTTRON,
	_CTCOPY,
	_CTRENAME,
	_CTREMOVE,
	_CTCHECK,
	_CTREBUILD,
	_CTCOMPACT,
	_CTGETIMG,
	_CTMAKEIMG,
	_CTCHECKIMG,
	_CTRBLIMG,
	_CTADDIMG,
	_CTLOAD,
	_CToldLOAD,
	_CTUNLOAD,
	_CToldUNLOAD,
	_CTLOADTEXT,
	_CTUNLOADTEXT,
	_CTMAXSIZE,
	_CTSEGMENT,
	_CTSQLINFO,
	_CTSQLLINK,
	_CTSQLUNLINK,
	_CTCONV,
	_CTXFD2XDD,
	_CTSQLCHECK,
	_CTSQLIZE,
	_CTMAKE,
	_CTCOMPRESS,
	_CTUNCOMPRESS,
	_CTCRYPTCONF,
	_CTCLONE,
	_CTDDF2XDD,
	_CTUPGRADE,
	_CTFILECOPY,
	_CTTEST,
	_CTSQLREFRESH,
	_CTFILEID,
	_CTAPPLYRULES,

	_CTFIX401,
	_CTFIXMAXLEN,
	_CTUNLOAD407,
	_CTADDRTGI,
	_CTFIXDUPSCAN,
	_CTUPG2ATTR,

	_CTRUN,

	_CTBUTIL,
	_CTBUTIL_STAT = 0,
	_CTBUTIL_CLONE,
	_CTBUTIL_COPY,
	_CTBUTIL_RENAME,
	_CTBUTIL_LOAD,
	_CTBUTIL_SAVE,
	_CTBUTIL_OPTIMIZE,
} CTUTIL_COMMAND_ID;

struct CTUTIL_COMMAND {
	const CTUTIL_COMMAND_ID id;
	pTEXT tag;
	pTEXT oldtag;
	const NINT nparms;
	const NINT nopts;
} static const ctutil_command[] = {
	/* id           tag           oldtag          mandatory,optional parameters */
	{_CTPROFILE,    "-profile",   "_ctprofile",   1,0},
	{_CTINFO,       "-info",      "_ctinfo",      1,2},
	{_CTPARAM,      "-param",     "_ctparam",     1,0},
	{_CTSETPATH,    "-setpath",   "_ctsetpath",   1,0},
	{_CTRESETFN,    "",           "_ctresetfn",   1,0},
	{_CTTRON,       "-tron",      "_cttron",      2,0},
	{_CTCOPY,       "-copy",      "_ctcopy",      2,0},
	{_CTRENAME,     "-rename",    "_ctrename",    2,1},
	{_CTREMOVE,     "-remove",    "_ctremove",    1,0},
	{_CTCHECK,      "-check",     "",             1,2},
	{_CTREBUILD,    "-rebuild",   "_ctrebuild",   1,2},
	{_CTCOMPACT,    "-compact",   "_ctcompact",   1,2},
	{_CTGETIMG,     "-getimg",    "_ctgetimg",    1,0},
	{_CTMAKEIMG,    "-makeimg",   "_ctmakeimg",   2,0},
	{_CTCHECKIMG,   "-checkimg",  "_ctcheckimg",  2,0},
	{_CTRBLIMG,     "-rblimg",    "_ctrblimg",    2,0},
	{_CTADDIMG,     "-addimg",    "",             2,0},
#ifdef ctFeatCOMPACT_TRUNCATE
	{_CTLOAD,       "-load",      "",             2,5},
#else
	{_CTLOAD,       "-load",      "",             2,4},
#endif
	{_CToldLOAD,    "_ctload",    "_ctload",      3,1},
	{_CTUNLOAD,     "-unload",    "",             2,4},
	{_CToldUNLOAD,  "_ctunload",  "_ctunload",    2,0},
	{_CTLOADTEXT,   "-loadtext",  "_ctloadtext",  3,1},
	{_CTUNLOADTEXT, "-unloadtext","_ctunloadtext",2,0},
	{_CTMAXSIZE,    "-maxsize",   "",             2,0},
	{_CTSEGMENT,    "-segment",   "",             3,0},
	{_CTSQLINFO,    "-sqlinfo",   "_ctsqlinfo",   1,2},
	{_CTSQLLINK,    "-sqllink",   "_ctsqllink",   2,5},
	{_CTSQLUNLINK,  "-sqlunlink", "_ctsqlunlink", 2,4},
	{_CTCONV,       "-conv",      "-sign",        1,1},
	{_CTXFD2XDD,    "-xfd2xdd",   "",             1,1},
	{_CTSQLCHECK,   "-sqlcheck",  "",             2,2},
	{_CTSQLIZE,     "-sqlize",    "_ctsqlize",    3,7},

	{_CTMAKE,       "-make",      "_ctmake",      2,0},
	{_CTCOMPRESS,   "-compress",  "",             1,0},
	{_CTUNCOMPRESS, "-uncompress","",             1,0},
	{_CTCRYPTCONF,  "-cryptconf", "",             2,0},
	{_CTCLONE,      "-clone",     "",             2,0},
	{_CTDDF2XDD,    "-ddf2xdd",   "",             1,2},
	{_CTUPGRADE,    "-upgrade",   "",             1,1},
	{_CTFILECOPY,   "-filecopy",  "",             2,1},
	{_CTTEST,       "-test",      "",             1,1},
	{_CTSQLREFRESH, "-sqlrefresh","_ctsqlrefresh",3,6},
	{_CTFILEID,     "-fileid",    "",             1,0},
	{_CTAPPLYRULES, "-applyrules","",             2,0}
},
ctutil_run = { _CTRUN, "-run", "", 1,0 },
ctbutil_command[] = {
	/* id             tag        mandatory,optional parameters */
	{_CTBUTIL_STAT,     "-stat", "",     1, 0},
	{_CTBUTIL_CLONE,    "-clone", "",    2, 1},
	{_CTBUTIL_COPY,     "-copy", "",     2, 0},
	{_CTBUTIL_RENAME,   "-rename", "",   2, 1},
	{_CTBUTIL_LOAD,     "-load", "",     2, 1},
	{_CTBUTIL_SAVE,     "-save", "",     2, 0},
	{_CTBUTIL_OPTIMIZE, "-optimize", "", 1, 2}
},
ctutil_cmd[] = {
	{CTUTIL_ALTER,      "-alter",     "", 2, 1},
	{CTUTIL_INFO,       "-info",      "", 1, 2},
	{CTUTIL_SETOWNER,   "-setowner",  "", 3, 0},
	{CTUTIL_CLROWNER,   "-clrowner",  "", 1, 0},
	{CTUTIL_EXIST,      "-exist",     "", 1, 0},
	{CTUTIL_AUGMENT,    "-augment",   "", 2, 0},
	{CTUTIL_PARTITION,  "-partition", "", 3, 0}
#ifdef ctFeatRTG_SQLINFO_ON_OPEN
	,{CTUTIL_MAKE,       "-make",      "", 2, 0}
#endif
#ifdef ctFeatRTG_ADDIDX_UPDATE
	,{CTUTIL_MAKEIDX,    "-makeidx",   "", 2, 0}
#endif
#ifdef ctFeatRTG_CTUTIL_UTILAPI_SQL
	,{CTUTIL_SQLINFO,    "-sqlinfo",   "", 1, 3},
	{CTUTIL_SQLLINK,    "-sqllink",   "", 2, 5},
	{CTUTIL_SQLUNLINK,  "-sqlunlink", "", 2, 4},
	{CTUTIL_SQLIZE,     "-sqlize",    "", 3, 7},
	{CTUTIL_SQLREFRESH, "-sqlrefresh","", 3, 6}
#endif
};

#define VALID_NBR_OF_ARGS ( \
	(ctutil_command[cmd].nopts) ? \
		((argc-1) >= ctutil_command[cmd].nparms && \
		(argc-1) <= (ctutil_command[cmd].nparms + ctutil_command[cmd].nopts)) \
	: \
		((argc-1) == ctutil_command[cmd].nparms) \
	)
#define CTBUTIL_VALID_ARGS(nargs, cmd) ctutil_validargs(nargs, &ctbutil_command[cmd])
#define CTUTIL_VALID_ARGS(nargs, cmd) ctutil_validargs(nargs, &ctutil_cmd[cmd])
static TEXT ctutil_validargs(NINT nargs, const struct CTUTIL_COMMAND *command)
{
	TEXT valid = NO;
	if (command->nopts)
		valid = ((nargs) >= command->nparms && (nargs) <= (command->nparms + command->nopts));
	else
		valid = ((nargs) == command->nparms);
	return valid;
}

#define CTUTIL_GETCOMMANDID(argv) ctutil_getcommandid(argv, ctutil_command, rtgSIZEOFARRAY(ctutil_command))
#define CTBUTIL_GETCOMMANDID(argv) ctutil_getcommandid(argv, ctbutil_command, rtgSIZEOFARRAY(ctbutil_command))
#define CTUTIL_GETCMDID(argv) ctutil_getcommandid(argv, ctutil_cmd, rtgSIZEOFARRAY(ctutil_cmd))
static NINT ctutil_getcommandid(pTEXT argv[], const struct CTUTIL_COMMAND command[], const NINT maxcommands)
{
	NINT i;
	NINT cmd;

	for (i=0, cmd=-1; i<maxcommands; i++)
	{
		if (!rtgSTRICMP(argv[0], command[i].tag) ||
			!rtgSTRICMP(argv[0], command[i].oldtag))
		{
			cmd = command[i].id;
			break;
		}
		else
		if (!rtgSTRNICMP(argv[0], command[i].tag, strlen(argv[0])) ||
			!rtgSTRNICMP(argv[0], command[i].oldtag, strlen(argv[0]))) {
			if (cmd < 0) {
				cmd = ctutil_command[i].id;
			} else {
				cmd = -1;
				break;
			}
		}
	}

	return(cmd);
}

static VOID ctutil_printerror(NINT logical, NINT ctree, NINT system, cpTEXT errmsg)
{
	cpTEXT rtgerr = NULL;

	if (logical < 0)
		rtgerr = "Initialization error";
	else
	if (logical < rtgSIZEOFARRAY(ctcb_error))
		rtgerr = ctcb_error[logical];
	if (rtgerr)
		fprintf(stderr, "\n%s.\n", rtgerr);

	if (ctree < 0 && ctree > CTE_INTERNAL_LAST)
	{
		if (errmsg && *errmsg)
			fprintf(stderr, "%s\n", errmsg);
	}
	else
	if (ctree)
	{
		fprintf(stderr, "c-tree status (%d)(%d)\n", ctree, system);
		if (errmsg && *errmsg && rtgerr && rtgSTRCMP(errmsg, rtgerr, NO))
			fprintf(stderr, "%s\n", errmsg);
		else
		{
			if ((errmsg = ctutil_getinterrmsg(ctree)) != NULL)
				fprintf(stderr, "%s\n", errmsg);
		}
	}
	else
	if (system)
	{
		if (errmsg && *errmsg && rtgerr && rtgSTRCMP(errmsg, rtgerr, NO))
			fprintf(stderr, "%s\n", errmsg);
		else
		{
			TEXT errbuf[256] = { 0 };

			fprintf(stderr, "system status (%d)\n", system);
			rtgASTRERROR(system, errbuf);
			fprintf(stderr, "%s\n", errbuf);
		}
	}
	else
	{
		if (errmsg && *errmsg && rtgerr && rtgSTRCMP(errmsg, rtgerr, NO))
			fprintf(stderr, "%s\n", errmsg);
	}
}

LOCAL NINT ctutil_close(CT_INIT *conn, pCT_FILE ctfile)
{
	NINT retval;

	CT_ERROR err = conn->err; /* save errors */
	retval = ctl_close(conn, ctfile);
	conn->err = err; /* restore errors */

	return retval;
}

NINT ctutil_exist(json_object* jrequest, cpTEXT path);
#ifdef ctFeatRTG_FILEPASSWORD
NINT ctutil_sqlinfo(json_object* jrequest, cpTEXT name, cpTEXT word, cpTEXT xddfile, ppTEXT opts);
NINT ctutil_sqllink(json_object* jrequest, TEXT unlink, cpTEXT name, cpTEXT word, cpTEXT adminpw, cpTEXT dbname, ppTEXT opts);
NINT ctutil_sqlize(json_object* jrequest, TEXT refresh, cpTEXT name, cpTEXT word, cpTEXT xddfile, cpTEXT adminpw, cpTEXT dbname, ppTEXT opts);
#ifdef ctFeatRTG_SQLINFO_ON_OPEN
NINT ctutil_make(json_object* jrequest, cpTEXT name, cpTEXT word, cpTEXT xddfile);
#endif
#ifdef ctFeatRTG_ADDIDX_UPDATE
NINT ctutil_makeidx(json_object* jrequest, cpTEXT name, cpTEXT word, cpTEXT xddfile);
#endif
#ifdef ctFeatRTG_PARTITION
NINT ctutil_partition(json_object* jrequest, cpTEXT name, cpTEXT word, cpTEXT rule, cpTEXT index);
#endif
#ifdef ctFeatRTG_AUGMENT
NINT ctutil_augment(json_object* jrequest, cpTEXT name, cpTEXT word, cpTEXT maxlen);
#endif
NINT ctutil_fileword(json_object* jrequest, cpTEXT name, cpTEXT word, cpTEXT newword, TEXT mode);
#ifdef ctFeatRTG_ALTERSCHEMA
NINT ctutil_alter(json_object* jrequest, cpTEXT name, cpTEXT word, pTEXT xddfile, ppTEXT opts);
#endif /* ctFeatRTG_ALTERSCHEMA */
NINT ctutil_info(json_object* jrequest, cpTEXT name, cpTEXT word, ppTEXT opts);

NINT ctexec(pCT_INIT conn, NINT argc, ppTEXT argv, cpTEXT word);
NINT _ctinfo(pCT_INIT conn, pTEXT name, cpTEXT word, NINT func, ppTEXT opts);
NINT _ctsetpath(pCT_INIT conn, pTEXT name, cpTEXT word);
NINT _cttron(pCT_INIT conn, pTEXT name, cpTEXT word, pTEXT func);
NINT _ctcompress(pCT_INIT conn, NINT argc, ppTEXT argv, cpTEXT word);
NINT _ctunload(pCT_INIT conn, pTEXT name, cpTEXT word, pTEXT flat, ppTEXT opts);
NINT _ctload(pCT_INIT conn, pTEXT name, cpTEXT word, pTEXT flat, ppTEXT opts);
NINT _ctsegment(pCT_INIT conn, pTEXT name, cpTEXT word, pTEXT size, pTEXT segs);
NINT _ctsqlinfo(pCT_INIT conn, pTEXT name, cpTEXT word, pTEXT xddfile, ppTEXT opts);
NINT _ctsqlcheck(pCT_INIT conn, pTEXT name, cpTEXT word, pTEXT xddfile, ppTEXT opts);
NINT _ctsqlize(pCT_INIT conn, pTEXT name, cpTEXT word, pTEXT xfdfile, pTEXT adminpw, pTEXT dbname, pTEXT *opts, TEXT refresh);
NINT _ctmake(pCT_INIT conn, pTEXT name, cpTEXT word, pTEXT xfdfile);
NINT _ctcheck(pCT_INIT conn, pTEXT name, cpTEXT word, pTEXT *opts);
NINT _ctfileid(CT_INIT* conn, pTEXT name, cpTEXT word);
#ifdef ctFeatRTG_MAXSIZE
NINT _ctmaxsize(pCT_INIT conn, pTEXT name, cpTEXT word, pTEXT size);
#endif /* ctFeatRTG_MAXSIZE */
#else /* ~ctFeatRTG_FILEPASSWORD */
#ifdef ctFeatRTG_ALTERSCHEMA
NINT ctutil_alter(json_object* jrequest, cpTEXT name, pTEXT xddfile, ppTEXT opts);
#endif /* ctFeatRTG_ALTERSCHEMA */
NINT ctutil_info(json_object* jrequest, cpTEXT name, ppTEXT opts);

NINT ctexec(CT_INIT *conn, NINT argc, ppTEXT argv);
NINT _ctinfo(CT_INIT *conn, pTEXT name, NINT func, ppTEXT opts);
NINT _ctsetpath(CT_INIT *conn, pTEXT name);
NINT _cttron(CT_INIT *conn, pTEXT name, pTEXT func);
NINT _ctcompress(CT_INIT *conn, NINT argc, ppTEXT argv);
NINT _ctunload(CT_INIT *conn, pTEXT name, pTEXT flat, ppTEXT opts);
NINT _ctload(CT_INIT *conn, pTEXT name, pTEXT flat, ppTEXT opts);
NINT _ctsegment(CT_INIT *conn, pTEXT name, pTEXT size, pTEXT segs);
NINT _ctsqlinfo(pCT_INIT conn, pTEXT name, pTEXT xddfile, ppTEXT opts);
NINT _ctsqlcheck(CT_INIT *conn, pTEXT name, pTEXT xddfile, ppTEXT opts);
NINT _ctsqlize(CT_INIT *conn, pTEXT name, pTEXT xfdfile, pTEXT adminpw, pTEXT dbname, pTEXT *opts, TEXT refresh);
NINT _ctmake(CT_INIT *conn, pTEXT name, pTEXT xfdfile);
NINT _ctcheck(CT_INIT *conn, pTEXT name, pTEXT *opts);
NINT _ctfileid(CT_INIT *conn, pTEXT name);
#ifdef ctFeatRTG_MAXSIZE
NINT _ctmaxsize(pCT_INIT conn, pTEXT name, pTEXT size);
#endif /* ctFeatRTG_MAXSIZE */
#endif /* ctFeatRTG_FILEPASSWORD */
NINT _ctprofile(CT_INIT *conn, pTEXT name);
NINT _ctsqllink(CT_INIT *conn, pTEXT filname, pTEXT dbname, pTEXT adminpw, pTEXT *opts);
NINT _ctsqlunlink(CT_INIT *conn, pTEXT filname, pTEXT dbname, pTEXT adminpw, pTEXT *opts);
NINT _ctxfd2xdd(CT_INIT *conn, pTEXT xfdfile, pTEXT rules);
NINT _ctddf2xdd(CT_INIT *conn, pTEXT xfddir, ppTEXT opts);
NINT _ctcryptconf(CT_INIT *conn, pTEXT cfgfile, pTEXT outfile);
#ifdef REMOVED_noFeatCOBOL
NINT _ctbugs(CT_INIT *conn);
NINT _ctresetfn(CT_INIT *conn, pTEXT name);
#endif
NINT _cttest(CT_INIT *conn, pTEXT *opts);

#endif	/* _CTUTIL_H_ */

/* end of ctutil.h */
