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

/* Header files. ***********************************************************/

#ifdef PROTOTYPE
#include <stdarg.h>
#else
#include <varargs.h>
#endif


/* Global constants. *******************************************************/

/*
 * Uncomment this define to log times to disk.
#define ctLOGTIMESTODISK
 */

#define SVN_DEFAULT "FAIRCOMS"	/* Default server name.		*/
#define UID_DEFAULT ""		/* Default user name.		*/
#define UPW_DEFAULT ""		/* Default user password.	*/
#define MAXTRD	     8		/* Maximum threads to display.	*/
#define NUML	     6
#define NAML        24
#define CLSL         2
#ifdef ctFeatFULLTEXTIDX
#define SERL	     8
#else
#define SERL	     4
#endif
#define PADG        (128 - ctSIZE(COUNT) - SERL - ctSIZE(COUNT) - NUML - NAML - CLSL)

#ifndef ctrt_printf
#define ctrt_printf printf /* ensure console (non-GUI) output */
#endif

#ifdef 	ctPortQNXRTP
#define USE_ANSI
#endif

#ifndef USE_ANSI
#ifdef ctPortUNIX
#ifdef ERR
#undef ERR	/* conflict with curses.h */
#endif
/* #define USE_CURSES */
#ifdef USE_CURSES
#define ctCURSES_H <curses.h>
#include ctCURSES_H
#endif
#ifdef VOID
#undef VOID
#define VOID void
#endif
#else
#define USE_ANSI
#endif
#endif /* ~USE_ANSI */

#ifdef ctPortWIN32
#ifndef WTMTAP_C
#undef USE_ANSI
#endif
#endif

#ifdef ctPortMAC
#undef USE_ANSI
#endif

#ifdef USE_ANSI

#ifdef ctPortWNTALPHA
#include <wincon.h>
#define LOC(y,x)                             \
{                                            \
    COORD pos = {x-1, y-1};                  \
    SetConsoleCursorPosition(ghStdOut, pos); \
}
#define CLS()     { int i; for (i=0; i<=MAXY; i++) printf("\n"); }

#else  /* ~ctPortWNTALPHA */

#define LOC(y,x)  printf("%c[%d;%dH", 27, y, x)
#define CLS()     printf("%c[H%c[J", 27, 27)

#endif /* ~ctPortWNTALPHA */

#define MAXY 24
#define MAXX 80
#define WINY (MAXY-2) /* Number of lines for thread output. */
#define Y 0
#define X 1

#endif /* USE_ANSI */


/* Global structures. ******************************************************/

typedef struct		/* Data record structure. */
{
	COUNT dlfg;
	TEXT  unqu[NUML];
#ifdef ctFeatFULLTEXTIDX
	LONG8 sern; /* Auto serial number. */
#else
	LONG  sern; /* Auto serial number. */
#endif
	COUNT dlen;
	TEXT  name[NAML];
	TEXT  clss[CLSL];
#ifdef ctFeatJSONfldtyp
	TEXT  doc[PADG];
#else
	TEXT  grbg[PADG];
#endif
} DREC;

#ifdef WTMTAP_C
typedef struct		/* Window information. */
{
	HWND  hwnd;
	ULONG iLine;
} ctWINCTL;
#endif /* WTMTAP_C */

typedef struct		/* Application options. */
{
	TEXT	action;		/* Type of action.			*/
	TEXT	filestat;	/* Create new files?			*/
	TEXT	mode;		/* Transaction processing mode.		*/
	LONG	trials;		/* Number of trials.			*/
	pTEXT	svn;		/* Server name.				*/
#ifdef ctCLIENT
	pTEXT	targetServerName; /* Name of target server name for replication test. */
#endif
	pTEXT	uid;		/* User name.				*/
	pTEXT	upw;		/* User password.			*/
	pTEXT	enc;		/* File encryption cipher.		*/
	pTEXT	filnam;		/* Data file name.			*/
	TEXT	filnamx[MAX_NAME]; /* Data file name with extension.	*/
#ifdef ctCMPREC
	NINT	compress;	/* Data record compression indicator.	*/
#endif
#ifdef ctFeatFLEXREC
	NINT	flexrec;	/* Flexible record file.		*/
#endif
#ifdef ctFeatVARLENnodefmt
	NINT	vlennod;	/* Use variable-length node data format.*/
	NINT	trnmrkopt;	/* Transaction mark optimized storage.	*/
	NINT	cmpoff;		/* Compressed offsets in index nodes.	*/
#endif
	NINT	replicate;	/* Enable replication for data file.	*/
	NINT	dfridx;		/* Defer index operations.		*/
#ifdef ctHUGEFILE
	NINT	my_huge;	/* Use 64-bit file offset.		*/
#endif
#ifdef ctFeatFULLTEXTIDX
	NINT	fultxtidx;	/* Create full text index on Name field */
#endif
#ifdef ctFeatJSONfldtyp
	NINT	jsonidx;	/* Create CT_JSON field and index	*/
#endif
	UCOUNT	seed;		/* Random number seed.			*/
	COUNT	trnlen;		/* Transaction length.			*/
	NINT	maxfiles;	/* Maximum number of files.		*/
	NINT	threads;	/* Number of threads.			*/
	NINT	quiet;		/* Quiet mode.				*/
	NINT	vlen;		/* Use variable-length data file.	*/
	NINT	trepl;		/* Use local/master replication.	*/
	NINT	opnfiles;	/* Open additional files.		*/
	NINT	timeLimitSec;	/* Time limit in seconds.		*/
	LONG	round;		/* Round number (1, 2, or 3).		*/
	TEXT	changeCache;	/* Change cache sizes at runtime	*/
	TEXT	scannerPage;	/* Use scanner cache pages		*/
	TEXT	IDfield;	/* Add an identity field		*/
#ifdef ctFeatCLUSTER_OPTIONS
	TEXT	failover;	/* Enable failover.			*/
	TEXT	unmanagedFailover; /* Failover is not managed by memphis. */
#endif
} APPOPTS, *pAPPOPTS;


/* Function prototypes. ****************************************************/
#ifdef PROTOTYPE
VOID appDescribe(VOID);
#else
VOID appDescribe();
#endif
#ifdef CTMTAP_C
#ifdef PROTOTYPE
VOID th_printf(NINT t, pTEXT arg0, ... ) PRINTFLIKE(2,3);
#else
VOID th_printf();
#endif
#else  /* ~CTMTAP_C */
VOID th_printf(LONG proc_id, pTEXT fmt, ... ) PRINTFLIKE(2,3);
#endif /* ~CTMTAP_C */


/* Global variables. *******************************************************/

COUNT     trnlen;	/* Transaction length (operations per tran).	*/
UCOUNT    seed;		/* Random number seed.				*/
NINT      tfin;		/* Number of threads which have finished.	*/
NINT      qid;		/* Inter-thread FIFO message queue ID.		*/
NINT	  quiet;	/* Quiet mode.					*/
NINT	  allrounds;	/* If non-zero, run all 3 rounds.		*/
LONG8	  totops;	/* Total operations performed.			*/
ctMUTEX   tfin_mtx;	/* Mutex for thread finished counter.		*/
#if (defined(USE_CURSES) || defined(USE_ANSI))
ctMUTEX   tprt_mtx;	/* Mutex for displaying thread output.		*/
#endif /* USE_CURSES || USE_ANSI */
TEXT      mode;		/* Transaction processing mode.			*/
TEXT      action;	/* Type of action.				*/
NINT	  abortFactor;	/* Abort every N transactions.			*/
pTEXT     svn;		/* Server name.					*/
pTEXT     uid;		/* User name.					*/
pTEXT     upw;		/* User password.				*/
ctBLOCK	  guiblk;	/* Block used to wait for window creation.	*/
#ifdef WTMTAP_C
ctWINCTL  ghwnd[MAXTRD];/* Information for each thread's window.	*/
#endif /* WTMTAP_C */
#ifdef TRANPROC
COUNT	  filmod;	/* File mode.					*/
#endif
#ifdef ctLOGTIMESTODISK
FILE	 *fpTIMES;	/* Working file pointer to log info to disk.	*/
#endif

#if (defined(USE_CURSES) || defined(USE_ANSI))
NINT      width;          /* Display width for each thread. */
#else  /* ~(USE_CURSES || USE_ANSI) */
#define width 80
#endif /* ~(USE_CURSES || USE_ANSI) */

#ifdef USE_CURSES
WINDOW   *win[MAXTRD];  /* Window list. */
WINDOW   *bdr[MAXTRD];	/* Border list. */
NINT	 did_initscr;	/* Initialized curses window handling support. */
#endif

#ifdef USE_ANSI
#ifdef ctPortWNTALPHA
HANDLE    ghStdOut;
#endif
COUNT     currpos[MAXTRD][2]; /* Current window position for threads. */
COUNT     buftop[MAXTRD];     /* Top of display in win buffer. */
TEXT      win[WINY][MAXX];      /* Window buffer for all threads. */
#endif
VRLEN     inbufsiz = 0;		/* Input buffer size for CTUSERX() calls  */
VRLEN     outbufsiz = 0;	/* Output buffer size for CTUSERX() calls */

#ifdef ctSRVR
#ifdef ctThrds
#ifdef ctFeatSRVDLL
#ifndef ctFeatMULTIDLLS
#define ctAppSERVER	/* Application linked to server. */
#endif
#endif
#endif
#endif

NINT	sigflg = 0;

#ifdef ctAppSERVER
#include <signal.h>

/*
 * sighandler: When SIGINT received, set shutdown flag.
 */

/*^***********************************/
static void sighandler(int signum)
{
	sigflg = 1;
}
/*~***********************************/

#endif /* ctAppSERVER */

/* ************************************************************************* */
/* ************************************************************************* */

/*
 * sysdate: Return current system time.
 */

static LONG sysdate(struct tm *ptm)
{
	time_t	ltime;

	time(&ltime);
	ctrt_localtime_r(&ltime, ptm);
	return((LONG)ltime);
} /* sysdate() */


/* ************************************************************************* */
/* ************************************************************************* */

/*
 * rate: Compute and display elapsed time and time per operation for a trial.
 */

static void rate(NINT proc_id,LONG lstop,LONG lstart,LONG trials,pTEXT th_str)
{
	if (!trials)
		return;
	lstop -= lstart;        /* Elapsed seconds. */
	th_printf(proc_id,
"Elapsed time: %d (sec)\nTime per op: %d (sec/1000) Ops per sec: %d\n",
		lstop, (lstop * 1000) / trials, trials/(lstop ? lstop : 1));

#ifdef ctLOGTIMESTODISK
	fprintf(fpTIMES,
"\n%s  Elapsed time : %d (sec) time per op = %d (sec/1000) ops per sec = %d\n\n",
		th_str, lstop, (lstop * 1000) / trials, trials/(lstop ? lstop : 1));
#endif
} /* rate() */


#ifdef TRANPROC
/* ************************************************************************* */
/* ************************************************************************* */

/*
 * tbeg: Start a transaction.
 */

static COUNT tbeg(pNINT ptlen,pNINT ptrst)
{
	COUNT	status = (COUNT)0;
#ifdef ctFeatMULTIDLLS
	NINT	CURENV = pCURENV[ctAppGetTid()];
#endif

	if (*ptlen == 0)
	{
		*ptlen = trnlen;
		*ptrst = 0;
		if (!TRANBEG((COUNT)(mode == 't' ?
		     (ctPREIMG | ctENABLE_BLK) : (ctTRNLOG | ctENABLE_BLK))))
			status = ctGetUserErrorCode();
	}
	return(status);
} /* tbeg() */
#endif


#ifdef TRANPROC
/* ************************************************************************* */
/* ************************************************************************* */

/*
 * tend: Commit or abort a transaction, or set or restore a save point.
 */

static COUNT tend(NINT proc_id,pNINT ptlen,NINT status,pNINT ptrst,pNINT tcnt,NINT tchg,NINT lokmod)
{
#ifdef ctFeatMULTIDLLS
	NINT	CURENV = pCURENV[ctAppGetTid()];
#endif

	if (status)
		if (*ptlen == trnlen || ++(*ptrst) > 3)
		{
			*ptlen = *ptrst = 0;
			TRANABT();
			return(status);
		}
		else
		{
			TRANRST(-1);
			return(status);
		}

	(*ptlen)--;
	if (*ptlen == 0)
	{
		*ptrst = 0;
		if (status)
			status = TRANABT();
		else
		{
			if (abortFactor && !(rand() % abortFactor))
				status = TRANABT();
			else
				status = TRANEND(lokmod);
			if (status == 0)
			{
				*tcnt += tchg;
				if (!quiet)
					th_printf(proc_id, "\r  TT %d", *tcnt);
			}
		}
		return(status);
	} else
		TRANSAV();

	return(0);
} /* tend() */
#endif


/* ************************************************************************* */
/* ************************************************************************* */

/*
 * unif: Return a number in the range l..u.
 */

static UCOUNT unif(UCOUNT l,UCOUNT u)
{
	UCOUNT retval;

	seed = 11737 * seed + 3731;
	retval = (seed % ((UCOUNT) (u - l + 1))) + l;

	return(retval);
} /* unif() */


/* ************************************************************************* */
/* ************************************************************************* */

/*
 * setval: Generate a field value.
 */

static VOID setval(PFAST pTEXT tp,COUNT vl,COUNT fl,COUNT vr)
{
	FAST COUNT i;
	COUNT      keyl;
	TEXT       lstchr;

	for (i = 0; i < vl; i++)
		*tp++ = 'A' + unif(0,vr);
	lstchr = *(tp - 1);
	keyl   = unif(vl,fl);
	for (i = vl; i < keyl; i++)
		*tp++ = lstchr;
	for (i = keyl; i < fl; i++)
		*tp++ = ' ';
} /* setval() */


/* ************************************************************************* */
/* ************************************************************************* */

/*
 * getskey: Return a number in the range l..u.
 */

static COUNT getskey(COUNT l,COUNT u)
{
	COUNT retval;

	retval = unif(l,u);
	return(retval);
} /* getskey() */


/* ************************************************************************* */
/* ************************************************************************* */

/*
 * getnum: Return the numeric (LONG) value of a string.
 */

static LONG getnum(pTEXT tp)
{
	LONG retval;

	retval = 0L;
	do
	{
		if (*tp >= '0' && *tp <= '9')
		{
			retval *= 10;
			retval += (*tp - '0');
		}
	} while (*tp++);
	return(retval);
} /* getnum() */


/* ************************************************************************* */
/* ************************************************************************* */

/*
 * getwrd3: Prompt user for input and get user's input.
 */

static VOID getwrd3(pTEXT tp,NINT size,pTEXT prompt)
{
	pTEXT sp;
	TEXT  dflt[MAX_NAME];

	sp = tp;
	ctrt_strncpy(dflt,tp,sizeof(dflt));

loop:
	tp = sp;
#ifdef ctMACINTOSH
	ctrt_printf("\nEnter %s: [%s]\n",prompt,dflt);
	gets(tp);
	while (size)
	{
#else
	ctrt_printf("\nEnter %s: [%s] ",prompt,dflt);
	fflush(stdout);
	while (size)
	{
		*tp = (TEXT)getchar(); /* if possible change to a no echo form */
#endif
		if (*tp == '\r' || *tp == '\n' || *tp == '\0')
		{
			if (sp != tp)
				*tp = '\0';
			else
				*tp = *dflt;
			if (ctrt_strcmp((pTEXT) sp,(pTEXT) "?") == 0)
			{
				ctrt_printf("\n");
				goto loop;
			}
			return;
		}
		tp++;
		size--;
	}
	*(tp - 1) = '\0';
} /* getwrd3() */


/* ************************************************************************* */
/* ************************************************************************* */

/*
 * getstr2: Get a string from user.
 */

static pTEXT getstr2(pTEXT dflt,pTEXT prompt)
{
	getwrd3(dflt,MAX_NAME,prompt);
	return(dflt);
} /* getstr2() */


/* ************************************************************************* */
/* ************************************************************************* */

/*
 * getchr: Get a string from user; return a character.
 */

static TEXT getchr(TEXT dflt,pTEXT prompt)
{
	TEXT inpbuf[MAX_NAME], inpchr;

	ctcr_sprintf(inpbuf,sizeof(inpbuf),"%c",dflt);
	getwrd3(inpbuf,MAX_NAME,prompt);
	inpchr = inpbuf[0];
	return(inpchr);
} /* getchr() */


/* ************************************************************************* */
/* ************************************************************************* */

/*
 * getlng: Get a string from user; return a LONG.
 */

static LONG getlng(LONG dflt,pTEXT prompt)
{
	LONG inplng;
	TEXT inpbuf[MAX_NAME];

	ctcr_sprintf(inpbuf, sizeof(inpbuf), "%d", dflt);
	getwrd3(inpbuf, MAX_NAME, prompt);
	inplng = getnum(inpbuf);
	return(inplng);
} /* getlng() */


/* ************************************************************************* */
/* ************************************************************************* */

/*
 * getcnt: Get a string from user; return a UCOUNT.
 */

static UCOUNT getcnt(COUNT dflt,pTEXT prompt)
{
	TEXT   inpbuf[MAX_NAME];
	UCOUNT inpcnt;

	ctcr_sprintf(inpbuf, sizeof(inpbuf), "%d", dflt);
	getwrd3(inpbuf, MAX_NAME, prompt);
	inpcnt = (UCOUNT)getnum(inpbuf);
	return(inpcnt);
} /* getcnt() */

#ifdef ctFeatFULLTEXTIDX

#include "ctdbsdk.h"

/*^***********************************/
static NINT ctdbinit (pCTHANDLE phSession,pCTHANDLE phDatabase,pCTHANDLE phTable,pCTHANDLE phRecord,pTEXT dbname)
{
	NINT	rc;
	CTHANDLE hSession = 0,hDatabase = 0,hTable = 0,hRecord = 0;

	/* allocate session handle */
	if (!(hSession = ctdbAllocSession(CTSESSION_CTDB))) {
		rc = UALC_ERR;
		printf("Error: Failed to allocate session: %d\n",
			rc);
		goto err_ret;
	}
	if (!(hDatabase = ctdbAllocDatabase(hSession))) {
		rc = ctdbGetError(hSession);
		printf("Error: Failed to allocate database handle: %d\n",
			rc);
		goto err_ret;
	}
	if (!(hTable = ctdbAllocTable(hDatabase))) {
		rc = ctdbGetError(hDatabase);
		printf("Error: Failed to allocate table handle: %d\n",
			rc);
		goto err_ret;
	}
	if (!(hRecord = ctdbAllocRecord(hTable))) {
		rc = ctdbGetError(hTable);
		printf("Error: Failed to allocate record handle: %d\n",
			rc);
		goto err_ret;
	}

	ctdbSetSessionParams(hSession,CT_SECT,8192 / 128);
	if ((rc = ctdbLogon(hSession, svn, uid, upw))) {
		if (rc == FNOP_ERR) {
			if ((rc = ctdbCreateSession(hSession,svn,uid,upw))) {
				if (rc == DOPN_ERR)
					rc = NO_ERROR;
				else {
					printf("Error: Failed to create session: %d\n",
						rc);
					goto err_ret;
				}
			}
			rc = ctdbLogon(hSession, svn, uid, upw);
		}
		if (rc) {
			printf("Error: Failed to initialize c-tree: %d\n",
				rc);
			goto err_ret;
		}
		if ((rc = ctdbCreateDatabase(hSession,dbname,"."))) {
			printf("Error: Failed to create database: %d\n",
				rc);
			goto err_ret;
		}
	}
	if ((rc = ctdbConnect(hDatabase,dbname))) {
		if (rc == INOT_ERR) {
			if ((rc = ctdbCreateDatabase(hSession,dbname,"."))) {
				printf("Error: Failed to create database: %d\n",
					rc);
				goto err_ret;
			}
			rc = ctdbConnect(hDatabase,dbname);
		}
		if (rc) {
			printf("Error: Failed to connect to database: %d\n",
				rc);
			goto err_ret;
		}
	}

err_ret:
	if (rc) {
		if (hRecord)
			ctdbFreeRecord(hRecord);
		if (hTable)
			ctdbFreeTable(hTable);
		if (hDatabase)
			ctdbFreeDatabase(hDatabase);
		if (hSession)
			ctdbFreeSession(hSession);
	} else {
		*phSession = hSession;
		*phDatabase = hDatabase;
		*phTable = hTable;
		*phRecord = hRecord;
	}
	return(rc);
}
/*~***********************************/

/*^***********************************/
static NINT ctdbinit2 (CTHANDLE hSession,CTHANDLE hDatabase,CTHANDLE hTable,pTEXT tableName,NINT creidx)
{
	CTDBRET	rc;
	CTHANDLE pFTI;
	TEXT	buf[MAX_NAME];

	if ((rc = ctdbOpenTable(hTable, tableName, CTOPEN_NORMAL))) {
		if (rc == CTDBRET_NOSUCHTABLE) {
			if ((rc = ctdbAddTable(hDatabase,tableName,""))) {
				printf("Error: ctdbAddTable() failed: %d\n",
					rc);
				goto err_ret;
			}
			if ((rc = ctdbOpenTable(hTable, tableName, CTOPEN_NORMAL))) {
				printf("Error: ctdbOpenTable() failed: %d\n",
					rc);
				goto err_ret;
			}
		} else {
			printf("Error: ctdbOpenTable() failed: %d\n",
				rc);
			goto err_ret;
		}
	}

	if (!creidx)
		goto err_ret;

	/* Create a full text index on the Name field. */
	ctrt_snprintf(buf, sizeof(buf), "%s_fts", tableName);
	if (!(pFTI = ctdbAddFTI(hTable, buf))) {
		rc = ctdbGetError(hTable);
		printf("Error: ctdbAddFTI() failed: %d\n",
			rc);
		goto err_ret;
	}
	if ((rc = ctdbAddFTIFieldByName(pFTI, 0, "Name", CTDB_FTI_MODE_REG))) {
		printf("Error: ctdbAddFTIFieldByName() failed: %d\n",
			rc);
		goto err_ret;
	}
	if ((rc = ctdbSetFTIOption(pFTI, CTDB_FTI_OPTION_TOKENIZER, NULL, CTDB_FTI_TOKENIZER_PORTER))) {
		printf("Error: ctdbSetFTIOption(CTDB_FTI_OPTION_TOKENIZER) failed: %d\n",
			rc);
		goto err_ret;
	}
	if ((rc = ctdbSetFTIOption(pFTI, CTDB_FTI_OPTION_TOKENSIZE, NULL, 50))) {
		printf("Error: ctdbSetFTIOption(CTDB_FTI_OPTION_TOKENSIZE) failed: %d\n",
			rc);
		goto err_ret;
	}

	if ((rc = ctdbAlterTable(hTable, CTDB_ALTER_NORMAL))) {
		printf("Error: ctdbAlterTable() failed: %d\n",
			rc);
		goto err_ret;
	}
	ctdbCloseTable(hTable);

err_ret:
	return((NINT) rc);
}
/*~***********************************/

/*^***********************************/
static void ctdbterm (NINT lgon,pCTHANDLE phSession,pCTHANDLE phRecord,pCTHANDLE phTable,pCTHANDLE phDatabase)
{

	if (lgon)
		ctdbLogout(*phSession);
	if (*phRecord) {
		ctdbFreeRecord(*phRecord);
		*phRecord = NULL;
	}
	if (*phTable) {
		ctdbFreeTable(*phTable);
		*phTable = NULL;
	}
	if (*phDatabase) {
		ctdbFreeDatabase(*phDatabase);
		*phDatabase = NULL;
	}
	if (*phSession) {
		ctdbFreeSession(*phSession);
		*phSession = NULL;
	}
}
/*~***********************************/

#endif /* ctFeatFULLTEXTIDX */

DREC	dodablk;	/* Data record buffer.	*/

DATOBJ doda[] = {	/* Field definitions for data record. */
		{"Dlfg", (pTEXT) &dodablk.dlfg, CT_INT2},
		{"Unqu", (pTEXT)  dodablk.unqu, CT_FSTRING, NUML},
#ifdef ctFeatFULLTEXTIDX
		{"$ROWID$", (pTEXT) &dodablk.sern, CT_INT8},
#else
		{"Sern", (pTEXT) &dodablk.sern, CT_INT4},
#endif
		{"Dlen", (pTEXT) &dodablk.dlen, CT_INT2},
		{"Name", (pTEXT)  dodablk.name, CT_FSTRING, NAML},
		{"Clss", (pTEXT)  dodablk.clss, CT_FSTRING, CLSL},
#ifdef ctFeatJSONfldtyp
		{"Doc", (pTEXT)  dodablk.doc, CT_JSON, PADG}
#else
		{"Grbg", (pTEXT)  dodablk.grbg, CT_FSTRING, PADG}
#endif
	};

/* ************************************************************************* */
/* ************************************************************************* */

/*
Uncomment #define QA_UPDLEN to test variable-length record updates that increase
the record length. And for a fixed length file we disable space management so
that the update writes the updated record at a new offset.
*/

/* #define QA_UPDLEN */

/*
Uncomment #define QA_TRANRECOV to test automatic recovery involving file rename
and delete operations.
*/

/* #define QA_TRANRECOV */

#define QAPRMIDX1	1	/* add index */
#define QARENAME1	2	/* rename file */
#define QADELETE1	3	/* delete file */
#define QAFILEID1	4	/* change file ID */
#define QACACHEUSE	5	/* test cache usage */

#ifdef ctPortWIN32
#define QAKILLCMD "kill -f"
#else
#define QAKILLCMD "kill"
#endif

TEXT	  qaaction = 0;	/* QA test action.				*/
pTEXT	  qanewfil;	/* New file name for QA test.			*/
LONG	  qakillpid;	/* Process ID of process to terminate.		*/

/*
Uncomment #define QA_TRAN_NONTRAN to test switching files between transaction
control and no transaction control. Then run:

ctmtap fc mT aa n1000 uADMIN pADMIN sFAIRCOMS
*/

/* #define QA_TRAN_NONTRAN */

#ifdef ctrt_exit
#undef ctrt_exit
#endif
#define ctrt_exit exit

#ifdef QA_TRAN_NONTRAN
#define CTVFYFIL_RECOVERY
#define main vfyfil_main
#define usage vfyfil_usage
#include "../special/utils/ctvfyfil.c"
#undef main
#undef usage
#endif

IFIL markdata;

#ifdef ctCLIENT

/*^***********************************/
/*
** Check if the return code indicates that the connection to the server
** has been lost.
*/
static NINT chklostconn (NINT rc)
{
	if (rc == ARQS_ERR || rc == ARSP_ERR || rc == TUSR_ERR ||
	    rc == SHUT_ERR || rc == NINT_ERR || rc == TRQS_ERR ||
	    rc == TRSP_ERR || rc == QTBK_PND)
		return(YES);

	return(NO);
}
/*~***********************************/

/*^***********************************/
/*
   Diagnostic callback function for cluster feature.

   [IN] context- Pointer to user context.
   [IN] opcode- Operation code.
   [IN] message- Message to display.
*/
static NINT clusterDiagFunction (pVOID context, NINT opcode, pTEXT format, ...)
{
	va_list argptr;
	TEXT	messageBuffer[1024];
	pTEXT	tp;
	NINT	i;
	size_t	remainingLength;

	va_start(argptr, format);
	tp = messageBuffer;
	i = ctrt_snprintf(tp, sizeof(messageBuffer), "ClusterDiag: [%d] ", opcode);
	if (i < 0)
		goto err_ret;
	tp += i;
	remainingLength = sizeof(messageBuffer) - i;
	ctcr_vsprintf(tp, remainingLength, format, argptr);
	printf("%s\n", messageBuffer);

err_ret:
	return(0);
}
/*~***********************************/

#endif /* ctCLIENT */

NINT cacheSizeThread(pVOID t)
{
	NINT	proc_id;
	COUNT	status;
	pTEXT	cacheOptions[2] = {
		"set_cache_options {\"dat_memory\": \"15 mb\", \"idx_memory\": \"15 mb\", \"timeout\": 5}",
		"set_cache_options {\"dat_memory\": \"10 mb\", \"idx_memory\": \"10 mb\", \"timeout\": 5}"
	};
	TEXT	qmsg[20];
	TEXT	i;
	NINT	numThreads = (NINT)(NLONG)t;

	proc_id = 0;
	th_printf(proc_id, "\nCache size thread starting\n");

	if ((status = INTISAMX(16, 12, 16, 16, 0, uid, upw, svn)))
	{
		th_printf(proc_id, "\nError: Could not init (%d,%d)\n", ctGetIsamErrorCode(), ctGetIsamFileNbr());
		goto exit_thrd;
	}

	i = 0;
	while (!sigflg && tfin < numThreads) {
		if ((status = ctSETCFG(setcfgCONFIG_OPTION, cacheOptions[i]))) {
			th_printf(proc_id, "\nError: Could not change cache size: %d\n", status);
			sigflg = YES;
			goto exit_thrd;
		}
		i = !i;
		ctThrdSleep(500);
	}

exit_thrd:
	ctThrdMutexGet(&tfin_mtx);
	++tfin;
	ctThrdMutexRel(&tfin_mtx);
	th_printf(proc_id, "\nCache size thread exiting\n");
	ctrt_snprintf(qmsg, sizeof(qmsg), "Thread %d finished.", proc_id + 1);
	ctThrdQueueWrite(qid, qmsg, (NINT)strlen(qmsg) + 1);
	ctThrdExit();

	ctrt_printf(
		"ctThrdExit should not return. Call FairCom Tech Support\n");

	return(0);
}

#ifdef ctCLIENT
/*^***********************************/
/*
   Get the server's current role.

   [OUT] serverRole- On success, the server's role is returned here.

   Returns a c-tree error code. NO_ERROR indicates success.
*/
static NINT getServerRole (SERVER_ROLE* pServerRole)
{
	NINT	rc;
	VRLEN	outlen;
	TEXT	serverRoleValue[16];

	if (!pServerRole) {
		return(PNUL_ERR);
	}

	outlen = sizeof(serverRoleValue);
	if ((rc = ctReplAgentOp(ctRAOPgetserverrole, serverRoleValue, 0, serverRoleValue, &outlen))) {
		ctrt_printf("Error: Failed to get server role: %d\n",
			rc);
		fflush(stdout);
		goto err_ret;
	}

	*pServerRole = atoi(serverRoleValue);

err_ret:
	return(rc);
}
/*~***********************************/
#endif

#ifdef ctCLIENT
/*^***********************************/
/*
   Get the server's current role.

   [OUT] serverRole- On success, the server's role is returned here.

   Returns a c-tree error code. NO_ERROR indicates success.
*/
static NINT serverIsWritable(char* isWritable)
{
	NINT	rc;
	TEXT	buf[4];

	if (!isWritable)
		return PNUL_ERR;

	buf[0] = '\0';
	rc = GETNAM(-1, buf, sizeof(buf), READONLYSERVER);
	if (rc) {
		printf("Error: Could not check read only server attribute: %d\n", rc);
		goto err_ret;
	}
	if (!strcmp(buf, "n"))
		*isWritable = YES;
	else
		*isWritable = NO;

err_ret:
	return rc;
}
/*~***********************************/
#endif

/*
 * tcode: Run a trial (executed by each thread).
 */

NINT tcode (pVOID t)
{
	NINT		proc_id;
	LONG		round;
	LONG		trials;
	LONG		itrial = 0,lstart,lstop;
	NINT		tlen,trst;
	NINT		status = 0;
	NINT		tcnt = 0,dfridx,opnfiles;
	VRLEN		reclen;
	COUNT		finds,locks;
	COUNT		key;
	FILNO		datno;
	COUNT		usrprf = 0;
	pAPPOPTS	pOptions;
	DREC		updblk,datblk;
	TEXT		th_str[48];
	TEXT		keybuf[80];
	TEXT		newbuf[80];
	TEXT		qmsg[20];
	TEXT		timebuf[26];
	TEXT		filnamx[MAX_NAME];
#ifdef ctFeatFULLTEXTIDX
	CTHANDLE	hSession = NULL;
	CTHANDLE	hDatabase = NULL;
	CTHANDLE	hTable = NULL;
	CTHANDLE	hRecord = NULL;
	NINT		lgon = NO,nrec;
#endif
#ifdef ctFeatMULTIDLLS
	NINT		CURENV;
#endif
	NINT		vlen;
#ifdef QA_TRAN_NONTRAN
	NINT		tt = 0;
#endif
#ifdef QA_UPDLEN
	pTEXT		updbuf = NULL;
	NINT		updlen = 128;
#endif
	TEXT		reconnecting = NO;
	pTEXT		serverName = svn ? svn : "FAIRCOMS";
#ifdef ctCLIENT
	TEXT		serverNames[2][MAX_NAME] = {
			"",
			""
	};
	UINT		currentServer = 0;
	UINT		numServerNames = 0;
#endif
	struct tm	ct_tm; /* Start/end date and time. */

	pOptions = (pAPPOPTS)t;
	vlen     = pOptions->vlen;
	if (pOptions->trepl)
		usrprf = USERPRF_TRANREPL;
	trials   = pOptions->trials;
	dfridx	 = pOptions->dfridx;
	opnfiles = pOptions->opnfiles;
	proc_id  = (pOptions->round % 100L) - 1;
	round = (pOptions->round / 100L) + 1L;
	ctrt_strncpy(filnamx, pOptions->filnamx, sizeof(filnamx));
#ifdef ctFeatMULTIDLLS
	ctAppSetTid(proc_id+1);
	CURENV = pCURENV[ctAppGetTid()];
	ctAppSwitchCtreeEnv(CTTYP_SRV);
#endif
#ifdef ctFeatMULTIDLLS
	if ((proc_id+1) % 2) {
		ctAppSwitchCtreeEnv(CTTYP_MTC);
		if ((status = ctThrdAttach())) {
			th_printf(proc_id, "Error: Unable to attach thread: %d\n", status);
			goto exit_thrd;
		}
	}
#endif

	th_printf(proc_id, "Started thread.\n");

	ctrt_snprintf(th_str, sizeof(th_str), " Round #%d  Thread #%d", round, proc_id+1);

#ifdef ctFeatCLUSTER_OPTIONS
	if (pOptions->failover) {
		FILE* fp = ctrt_fopen("failover.txt", "r");
		NINT i;

		if (fp) {
			size_t	slen;
			for (i=0; i<2; i++) {
				if (fgets(serverNames[i], sizeof(serverNames[i]), fp)) {
					++numServerNames;
					if ((slen = strlen(serverNames[i])) && serverNames[i][slen - 1] == '\n') {
						serverNames[i][slen - 1] = '\0';
					}
				}
			}
			fclose(fp);
			if (numServerNames) {
				printf("Read server names from file failover.txt\n");
				serverName = serverNames[currentServer];
			}
		}
	}
#endif

	lstart = 0L;
	tlen = trst = 0;

	/* Stop the thread. ***************************************************/
	if (action == 'S')
	{
		STPUSR();
		goto exit_thrd;
	}

	itrial = 1;
#ifdef ctFeatCLUSTER_OPTIONS
	if (pOptions->failover) {
		cpTEXT failoverMode = pOptions->unmanagedFailover ? ctCLUSTER_OPTION_SET_FAILOVER_UNMANAGED : ctCLUSTER_OPTION_SET_FAILOVER_MANAGED;

		/* enable failover support */
		if ((status = ctSetClusterOption(ctCLUSTER_OPTION_FAILOVER, failoverMode))) {
			printf("Error: Could not enable failover: %d\n", status);
			goto exit_thrd;
		}
		printf("Enabled failover feature.\n");

		if ((status = ctSetClusterOption(ctCLUSTER_OPTION_DIAG_CALLBACK,(pTEXT) clusterDiagFunction))) {
			printf("Error: Could not set diagnostic callback function: %d\n", status);
			goto exit_thrd;
		}
		printf("Set failover diagnostic callback function.\n");

		if ((status = ctSetClusterOption(ctCLUSTER_OPTION_FAILOVER_TIMEOUT_SEC, "120"))) {
			printf("Error: Could not set failover timeout: %d\n",
				status);
			goto exit_thrd;
		}
		printf("Set failover timeout.\n");

		if ((status = ctSetCommProtocolOption(ctCOMMOPT_SOCKET_TIMEOUT, "120"))) {
			printf("Error: Could not set socket timeout: %d\n",
				status);
			goto exit_thrd;
		}
		printf("Set socket timeout.\n");

		if (numServerNames == 2) {
			TEXT	serverNameBuffer[2 * MAX_NAME + 1];

			ctrt_snprintf(serverNameBuffer, sizeof(serverNameBuffer), "%s;%s",
				serverNames[0], serverNames[1]);
			if ((status = ctSetClusterOption(ctCLUSTER_OPTION_SERVER_LIST, serverNameBuffer))) {
				printf("Error: Could not set server list: %d\n", status);
				goto exit_thrd;
			}
			printf("Set server list.\n");

			if (pOptions->unmanagedFailover == NO) {
				if ((status = ctSetClusterOption(ctCLUSTER_OPTION_SERVER_REQUIREMENTS, "{\"connectionMode\": \"write\"}"))) {
					printf("Error: Could not set server requirements: %d\n", status);
					goto exit_thrd;
				}
				printf("Set server requirements.\n");
			}
		}
	}
reconnect:
#endif
#ifdef ctFeatFULLTEXTIDX
	if (action == 'F') {
		if (!(status = ctdbinit(&hSession,&hDatabase,&hTable,&hRecord,"test"))) {
			lgon = YES;
			if ((status = ctdbinit2(hSession,hDatabase,hTable,pOptions->filnam,NO))) {
				goto exit_thrd;
			}
		}
	} else
#endif
	{
#ifdef ctCLIENT
		printf("\nConnecting to server: %s\n",
			serverName[0] ? serverName : "FAIRCOMS");
#endif
		if ((status = INTISAMX(16, 12, 16, 16, usrprf, uid, upw, serverName)))
		{
#ifdef ctFeatCLUSTER_OPTIONS
			if (status == ASKY_ERR && pOptions->failover && numServerNames)
				goto reconnect;
			if (reconnecting && status == ASKY_ERR) {
				ctThrdSleep(500);
				goto reconnect;
			}
#endif
			th_printf(proc_id, "\nCould not init (%d,%d)\n", ctGetIsamErrorCode(), ctGetIsamFileNbr());
			goto exit_thrd;
		}
#ifdef ctFeatCLUSTER_OPTIONS
		else if (pOptions->unmanagedFailover) {
			char	isWritable = NO;

			do {
				SERVER_ROLE	serverRole;

				/* check that the server is primary */
				status = getServerRole(&serverRole);
				if (status) {
					th_printf(proc_id, "\nCould not check if server is primary server: %d\n",
						status);
					goto exit_thrd;
				}
				if (serverRole != SERVER_ROLE_PRIMARY) {
					th_printf(proc_id, "Server is not primary server (current role is %d). Retrying...\n",
						serverRole);
					ctThrdSleep(500);
					currentServer = 1 - currentServer;
					serverName = serverNames[currentServer];
					CLISAM();
					goto reconnect;
				}
				/* check that the server is writable */
				isWritable = NO;
				status = serverIsWritable(&isWritable);
				if (status) {
					th_printf(proc_id, "\nCould not check if server is writable: %d\n",
						status);
					goto exit_thrd;
				}
			} while (!isWritable);
		}
#endif
	}

#ifdef ctCLIENT
	th_printf(proc_id,"Connected to server.\n");
#else
	th_printf(proc_id,"Initialized c-tree Plus.\n");
#endif

#ifdef ctCLIENT
	reconnecting = NO;
#endif

#ifdef QA_TRAN_NONTRAN
again:
#endif
#ifdef QA_TRANRECOV
	if (qaaction == QACACHEUSE) {
		UINT	i;
		LONG	numrecs;
		UINT	maxrec;
		UTEXT	j;
		FILNO	datno2;
		ctGSMS	gsms; /* system snapshot */
		ctGFMS	gfms; /* file snapshot */
		TEXT	datnobuf[16];
		TEXT	datno2buf[16];

		if ((status = ctSNAPSHOT(ctPSSsystem, datnobuf, &gsms, sizeof(gsms)))) {
			printf("Error: Failed to get system snapshot: %d\n",
				status);
			goto exit_thrd;
		}
		printf("   server's data cache size: " ctLLnd(12) "\n", (LONG8)gsms.sct_dxbuf* gsms.sct_ndsec * 128);
		printf("   server's cache page size: %12d\n", gsms.sct_ndsec * 128);
		printf(" number of data cache pages: %12u\n", gsms.sct_dxbuf);
		/* calculate number of records to read so that the two files use about 75% of the data cache space */
		maxrec = gsms.sct_dxbuf * 3 / 8 * (gsms.sct_ndsec * 128) / markdata.dreclen;
		if ((datno = OPNRFILX(-1, "mark.dat",
			ctSHARED | ctDUPCHANEL, NULL)) == -1) {
			th_printf(proc_id, "Could not open mark.dat files (%d,%d,%d)\n",
				ctGetIsamErrorCode(), ctGetIsamFileNbr(), ctGetSystemErrorCode());
			if (ctGetIsamErrorCode() == FNOP_ERR)
				th_printf(proc_id, "Have you created the files?\n");
			goto exit_thrd;
		}
		if ((numrecs = DATENT(datno)) == -1) {
			printf("Error: Could not get record count: %d\n",
				uerr_cod);
			goto exit_thrd;
		}
		printf("mark.dat  number of records: %12u\n", numrecs);
		if ((UINT)numrecs < maxrec) {
			printf("Error: Data file has %u records but %u records are required for the cache use test based on the server's current cache size.\n",
				(UINT)numrecs, maxrec);
		}
		ctrt_snprintf(datnobuf, sizeof(datnobuf), "%d", datno);

		if ((datno2 = OPNRFILX(-1, "mark2.dat",
			ctSHARED | ctDUPCHANEL, NULL)) == -1) {
			th_printf(proc_id, "Could not open mark2.dat files (%d,%d,%d)\n",
				ctGetIsamErrorCode(), ctGetIsamFileNbr(), ctGetSystemErrorCode());
			if (ctGetIsamErrorCode() == FNOP_ERR)
				th_printf(proc_id, "Have you created the files?\n");
			goto exit_thrd;
		}
		if ((numrecs = DATENT(datno)) == -1) {
			printf("Error: Could not get record count: %d\n",
				uerr_cod);
			goto exit_thrd;
		}
		printf("mark2.dat number of records: %12u\n", numrecs);
		if ((UINT)numrecs < maxrec) {
			printf("Error: Data file has %u records but %u records are required for the cache use test based on the server's current cache size.\n",
				(UINT)numrecs, maxrec);
		}
		CLRFIL(datno2);
		printf("  number of records to read: %12u\n", maxrec);

		printf("reading mark.dat...\n");
		status = FRSREC(datno, &datblk);
		for (i = 0; !status && i < maxrec; i++) {
			status = NXTREC(datno, &datblk);
		}
		if ((status = ctSNAPSHOT(ctPSSfile, datnobuf, &gfms, sizeof(gfms)))) {
			printf("Error: Failed to get file snapshot: %d\n",
				status);
			goto exit_thrd;
		}
		printf("mark.dat  cache pages: %6u\n", gfms.datcnt);

		for (j = 0; j < 3; j++) {
			if ((datno2 = OPNRFILX(-1, "mark2.dat",
				ctSHARED | ctDUPCHANEL, NULL)) == -1) {
				th_printf(proc_id, "Could not open files (%d,%d,%d)\n",
					ctGetIsamErrorCode(), ctGetIsamFileNbr(), ctGetSystemErrorCode());
				if (ctGetIsamErrorCode() == FNOP_ERR)
					th_printf(proc_id, "Have you created the files?\n");
				goto exit_thrd;
			}
			ctrt_snprintf(datno2buf, sizeof(datno2buf), "%d", datno2);

			printf("reading mark2.dat...\n");
			status = FRSREC(datno2, &datblk);
			for (i = 0; !status && i < maxrec; i++) {
				status = NXTREC(datno2, &datblk);
			}

			if ((status = ctSNAPSHOT(ctPSSfile, datnobuf, &gfms, sizeof(gfms)))) {
				printf("Error: Failed to get file snapshot: %d\n",
					status);
				goto exit_thrd;
			}
			printf("mark.dat  cache pages: %6u\n", gfms.datcnt);

			if ((status = ctSNAPSHOT(ctPSSfile, datno2buf, &gfms, sizeof(gfms)))) {
				printf("Error: Failed to get file snapshot: %d\n",
					status);
				goto exit_thrd;
			}
			printf("mark2.dat cache pages: %6u\n", gfms.datcnt);
			printf("closing mark2.dat\n");
			CLRFIL(datno2);
		}
		goto exit_thrd;
	}
#endif
	/* Open data and index files. */
#ifdef ctFeatFULLTEXTIDX
	if (action == 'F') {
		datno = ((pCTDBTABLE) hTable)->datno;
	} else
#endif
	if ((datno = OPNRFILX(-1, filnamx,
	    ctSHARED | ctDUPCHANEL, NULL)) == -1) {
		th_printf(proc_id,"Could not open files (%d,%d,%d)\n",
			ctGetIsamErrorCode(), ctGetIsamFileNbr(), ctGetSystemErrorCode());
		if (ctGetIsamErrorCode()  == FNOP_ERR)
			th_printf(proc_id, "Have you created the files?\n");
		goto exit_thrd;
	}

	th_printf(proc_id, "Opened data and index files.\n");
	if (pOptions->scannerPage) {
		if ((status = PUTHDR(datno, YES, ctSCANCACHEhdr))) {
			th_printf(proc_id, "Error: Could not enable scan cache pages: %d\n", status);
		} else {
			th_printf(proc_id, "Enabled scan cache pages\n");
		}
	}
	if (opnfiles) {
		th_printf(proc_id, "Opening %d additional file(s)...\n",
			opnfiles);
		for (itrial=0; itrial<opnfiles; itrial++) {
			TEXT	fn[MAX_NAME];
			COUNT	filno;

			ctrt_snprintf(fn, sizeof(fn), "test%03d.dat",itrial);
			filno = (COUNT) (5 + itrial);
			if (OPNFIL(filno, fn, ctSHARED)) {
				COUNT uec = ctGetUserErrorCode();
				if (uec == FNOP_ERR) {
					if (!CREDATX(filno,fn,16,0,ctFIXED,0L,NULL,NULL))
						continue;
					th_printf(proc_id,"Could not create additional files (%d)\n",
						uec);
					goto exit_thrd;
				}
				th_printf(proc_id,"Could not open additional files (%d)\n",
					uec);
				goto exit_thrd;
			}
		}
	}

#ifdef TRANPROC
	ctThrdMutexGet(&tfin_mtx);
#ifndef QA_TRAN_NONTRAN
	if (filmod == (COUNT)-1)
#endif
		filmod = (COUNT)GETFIL(datno, FILMOD);
	ctThrdMutexRel(&tfin_mtx);

	if ((COUNT)(filmod & ctLOGFIL))
	{
		th_printf(proc_id, "Using full transaction processing.\n");
		mode = 'T';
	}
	else if ((COUNT)(filmod & ctPREIMG))
	{
		th_printf(proc_id, "Using no log transaction processing.\n");
		mode = 't';
	}
	else
	{
		th_printf(proc_id, "Using no transaction processing.\n");
		mode = 'N';
	}
#else
	th_printf(proc_id, "Using no transaction processing.\n");
#endif

	lstart = sysdate(&ct_tm);
	ctrt_asctime_r(&ct_tm, timebuf);
	th_printf(proc_id,"Start time: %s\n", timebuf);
#ifdef ctLOGTIMESTODISK
	fprintf(fpTIMES,"%s  Start time   : %s",th_str,timebuf);
#endif

	/* Run cttestfunc() routine.*******************************************/
	if (action == 'T')
	{
		LONG8	tbas, tstart, tstop = 0;
		LONG	elapsed;
#ifdef TRANPROC
		if (mode != 'N')
		{
			ctrt_hrtimbas(tbas);
			ctrt_hrtimer(tstart);
			for ( ; itrial <= trials; itrial++)
			{
				if (sigflg)
					break;
				status = cttestfunc(0);
				if (status) {
					th_printf(proc_id,"\n (%d:%d)",itrial,status);
					break;
				}

				/* assignments intended */
				status = (tbeg(&tlen,&trst) || cttestfunc(0));
				if (status)
					th_printf(proc_id,"\n (%d:%d)\n",itrial,status);
				status = tend(proc_id,&tlen,status,&trst,&tcnt,1,ctFREE);
				if (status)
					th_printf(proc_id,"\n (%dT%d)\n",itrial,status);
			}
		}
		else
#endif
		{
			ctrt_hrtimbas(tbas);
			ctrt_hrtimer(tstart);
			for ( ; itrial <= trials; itrial++)
			{
				if (sigflg)
					break;
				status = cttestfunc(0);
				if (status) {
					th_printf(proc_id,"\n (%d:%d)",itrial,status);
					break;
				}
			}
		}
		ctrt_hrtimer(tstop);
		if (tstop) {
			elapsed = (LONG) ((tstop - tstart) * 1000 / tbas);
			th_printf(proc_id,"Elapsed time: %d msec.\n", elapsed);
			th_printf(proc_id,"%d calls/sec.\n",
				(LONG) ((LONG8)trials * 1000 / (elapsed ? elapsed : 1)));
		}
		goto exit_thrd;
	}
#ifndef ctBNDSRVR
	/* Run CTUSERX() routine.*******************************************/
	else if (action == 'X')
	{
		LONG8	tbas, tstart, tstop = 0;
		LONG	elapsed;
		pTEXT	inbuf, outbuf;
		VRLEN	outsiz;

		if (inbufsiz)
			inbuf = mballc(1, inbufsiz);
		else
			inbuf = NULL;
		if (outbufsiz)
			outbuf = mballc(1, outbufsiz);
		else
			outbuf = NULL;

		ctrt_hrtimbas(tbas);
		ctrt_hrtimer(tstart);
		for ( ; itrial <= trials; itrial++)
		{
			if (sigflg)
				break;
			outsiz = outbufsiz;
			status = CTUSERX(">LOCAL>", inbuf, inbufsiz, outbuf,
				&outsiz);
			if (status) {
				th_printf(proc_id,"\n (%d:%d)",itrial,status);
				break;
			}
		}

		ctrt_hrtimer(tstop);
		if (tstop) {
			elapsed = (LONG) ((tstop - tstart) * 1000 / tbas);
			if (!elapsed)
				elapsed = 1;
			ctrt_printf("Elapsed time: %d msec.\n", elapsed);
			ctrt_printf("%d calls/sec.\n", trials / (elapsed ? elapsed : 1) * 1000);
		}

		if (inbuf)
			mbfree(inbuf);
		if (outbuf)
			mbfree(outbuf);
		goto exit_thrd;
	}
#endif /* ~ctBNDSRVR */

	/* Add data. **********************************************************/
	if (action == 'A')
	{
		th_printf(proc_id,"Adding records...\n");

		memset(&datblk,0,sizeof(datblk));
		for ( ; itrial <= trials; itrial++)
		{
			if (sigflg)
				break;
			setval(datblk.unqu,NUML,NUML,27);
			setval(datblk.name,8,NAML,16);
			setval(datblk.clss,CLSL,CLSL,4);
#ifdef ctFeatJSONfldtyp
			{
			LONG	len;
			/* set the json field value */
			ctrt_snprintf(datblk.doc + sizeof(LONG), sizeof(datblk.doc) - sizeof(LONG), "{\"id\": %d}", itrial);
			len = (LONG) strlen(datblk.doc + sizeof(LONG)) + 1;
			memcpy(datblk.doc,&len,sizeof(LONG));
			}
#endif
#ifdef TRANPROC
			if (mode != 'N')
			{
				/* assignments intended */
				if ((status=tbeg(&tlen,&trst)) ||
				    vlen && (status=ADDVREC(datno,&datblk,sizeof(datblk))) ||
				    !vlen && (status=ADDREC(datno,&datblk)))
					th_printf(proc_id,"\n (%d:%d:%.6s)\n",
						itrial,status,datblk.unqu);
				status = tend(proc_id,&tlen,status,&trst,&tcnt,1,ctFREE);
				if (status)
					th_printf(proc_id,"\n (%dT%d)\n",itrial,status);
			}
			else
#endif
			{
		                /* assignments intended */
				if ((status = LKISAM(ctENABLE_BLK)) ||
				    vlen && (status = ADDVREC(datno,&datblk,sizeof(datblk))) ||
				    !vlen && (status = ADDREC(datno,&datblk)))
					th_printf(proc_id,"\n (%d:%d:%.6s)\n",
						itrial,status,datblk.unqu);
				++tcnt;
				if (!quiet && !(itrial % 100))
					th_printf(proc_id, "\r  TT %d", tcnt);

				/* assignment intended */
				if (status = LKISAM(ctFREE))
					th_printf(proc_id,"\n (%dL%d)\n",itrial,status);
			}
#ifdef ctCLIENT
			if (status == ARQS_ERR || status == ARSP_ERR ||
			    status == TUSR_ERR || status == SHUT_ERR ||
			    status == NINT_ERR)
				break;
#endif
#ifdef ctFeatCLUSTER_OPTIONS
			if (pOptions->failover && status == ROSV_ERR)
				break;
#endif
		}

		goto exit_thrd;
	}

	/* Get, Delete, or Update. ********************************************/
	switch (action)
	{
	case 'G':
		th_printf(proc_id,"Retrieving records...\n");
		break;
#ifdef ctFeatFULLTEXTIDX
	case 'F':
		th_printf(proc_id,"Retrieving records using full text index...\n");
		break;
#endif
	case 'D':
		th_printf(proc_id,"Deleting records...\n");
		break;

	case 'U':
		th_printf(proc_id,"Updating records...\n");
		break;
	}

	for ( ; itrial <= trials; itrial++)
	{
		if (sigflg)
			break;
		finds = locks = 0;
gfind:
		for (key = 0; key < 30; key++)
			keybuf[key] = 0;
#ifdef QA_TRAN_NONTRAN
		key = (COUNT) (datno + 1);
#else
		key = getskey((COUNT)(datno+1+dfridx),(COUNT)(datno+2));
#endif
		switch (key-datno)
		{
		case 2:
			setval(keybuf,NUML,NUML,27);
		        break;
		case 1:
			setval(keybuf,8,NAML,16);
			break;
		}

#ifdef TRANPROC
		if (mode != 'N')
		/* Transaction processing. *******************************************/
		{
			if (tbeg(&tlen,&trst))
			{
				th_printf(proc_id,"\n (%d:%d)\n",itrial,ctGetIsamErrorCode());
				continue;
			}
tlock1:
			if (action == 'B') {
				reclen = sizeof(datblk);
				status = BLKIREC(key,ctBLKIREC_GTE,1,"1",keybuf,&datblk,
					vlen ? &reclen : NULL);
			} else {
				status = GTEREC(key,keybuf,&datblk);
			}
			if (status != 0)
			{
				if (status == 101 && finds++ < 10)
				{
/* If we didn't find a matching key, don't abort the transaction.
					TRANABT();
					tlen = 0;
*/
					goto gfind;
				}
				if (status != 42 && status != 101 && !(action == 'B' && status == NTIM_ERR))
					th_printf(proc_id,"\n (%d:%d:%d:%s)\n",
						itrial,status,key,keybuf);
				if (status == 42)
				{
					if (locks++ < 10)
						goto tlock1;
					else
						th_printf(proc_id,"\n (%d:%d)\n",itrial,status);
				}

				/* assignment intended */
				if (status = TRANABT())
					th_printf(proc_id,"\n (%dT%d)",itrial,status);
				tlen = 0;
#ifdef ctFeatCLUSTER_OPTIONS
				if (pOptions->failover && (status == SERVER_FAILOVER_ERR || chklostconn(status) || status == ROSV_ERR)) {
					printf("Lost connection. Reconnecting...\n");
					reconnecting = YES;
					goto reconnect;
				}
#endif
				continue;
			}

			switch (action)
			{
#ifdef ctFeatFULLTEXTIDX
case 'F': /* Full text search. ***********************************************/
			datblk.clss[0] = '\0';
			if ((status = ctdbFTSearchOn(hRecord, datblk.name, 0))) {
				th_printf(proc_id,"Error: ctdbFTSearchOn() failed: %d\n",
					status);
				goto exit_thrd;
			}
			nrec = 0;
			status = ctdbFirstRecord(hRecord);
			while (!status) {
				++nrec;
				status = ctdbNextRecord(hRecord);
			}
			if (status != INOT_ERR) {
				th_printf(proc_id,"F: status=%d for [%s]\n",
					status,datblk.name);
			} else if (nrec == 0)
				th_printf(proc_id,"F: nrec=%d for [%s]\n",
					nrec,datblk.name);
			if ((status = ctdbFTSearchOff(hRecord))) {
				th_printf(proc_id,"Error: ctdbFTSearchOff() failed: %d\n",
					status);
				goto exit_thrd;
			}
			status = tend(proc_id,&tlen,status,&trst,&tcnt,0,ctFREE);
			if (status)
				th_printf(proc_id,"\n (%dT%d)",itrial,status);
			break;
#endif
case 'G': /* Get. ************************************************************/
case 'B': /* Blocking ISAM read. *********************************************/
			if (!quiet)
			{
				/* Truncate key value for display purposes. */
				if (width >= 6)
					keybuf[width-6] = '\0';
				if (!(itrial % 100))
					th_printf(proc_id,"\r G [%s]", keybuf);
			}
			status = tend(proc_id,&tlen,status,&trst,&tcnt,0,ctFREE);
			if (status)
				th_printf(proc_id,"\n (%dT%d)",itrial,status);
			break;

case 'D': /* Delete. *********************************************************/
			if (vlen && (status = DELVREC(datno)) != 0 ||
			    !vlen && (status = DELREC(datno)) != 0)
				th_printf(proc_id," (D%d:%d)",itrial,status);
			else if (!quiet)
			{
				/* Truncate key value for display purposes. */
				if (width >= 6)
					keybuf[width-6] = '\0';
				if (!(itrial % 100))
					th_printf(proc_id,"\r D [%s]", keybuf);
			}
			status = tend(proc_id,&tlen,status,&trst,&tcnt,-1,ctFREE);
			if (status)
				th_printf(proc_id,"\n (%dT%d)",itrial,status);
			break;

case 'U': /* Update. *********************************************************/
			cpybuf(&updblk,&datblk,128);
			/* Choose a key to update. */
			switch (getskey((COUNT)(datno+1+dfridx),(COUNT)(datno+3))-datno)
			{
		case 2:
			cpybuf(keybuf,datblk.unqu,NUML);
			setval(updblk.unqu,NUML,NUML,27);
			cpybuf(newbuf,updblk.unqu,NUML);
			newbuf[NUML] = '\0';
			break;

		case 1:
			cpybuf(keybuf,datblk.name,NAML);
			setval(updblk.name,8,NAML,16);
			cpybuf(newbuf,updblk.name,NAML);
			newbuf[NAML] = '\0';
			break;

		case 3:
			cpybuf(keybuf,datblk.clss,CLSL);
			setval(updblk.clss,CLSL,CLSL,4);
			cpybuf(newbuf,updblk.clss,CLSL);
			newbuf[CLSL] = '\0';
			break;
			}
#ifdef ctFeatJSONfldtyp
			{
			LONG	len;
			/* set the json field value */
			ctrt_snprintf(updblk.doc + sizeof(LONG), sizeof(updblk.doc) - sizeof(LONG), "{\"id\": %d}", itrial);
			len = (LONG) strlen(updblk.doc + sizeof(LONG)) + 1;
			memcpy(updblk.doc,&len,sizeof(LONG));
			}
#endif
#ifdef QA_UPDLEN
			if (vlen) {
				if (updbuf) {
					mbfree(updbuf);
				}
				++updlen;
				updbuf = mballc(1,updlen);
				memcpy(updbuf,&updblk,sizeof(updblk));
				if ((status = RWTVREC(datno,updbuf,updlen)) != 0) {
					th_printf(proc_id,"\n (%ld:%d)\n",itrial,status);
				}
			} else if (!vlen) {
				if ((status = DELREC(datno)) != 0) {
					th_printf(proc_id,"\n (%ld:%d)\n",itrial,status);
				} else if ((status = ADDREC(datno,&updblk)) != 0) {
					th_printf(proc_id,"\n (%ld:%d)\n",itrial,status);
				}
			}
#else
			if (vlen && (status = RWTVREC(datno,&updblk,sizeof(updblk))) != 0 ||
			    !vlen && (status = RWTREC(datno,&updblk)) != 0)
				th_printf(proc_id,"\n (R%d:%d)",itrial,status);
#endif
			else if (!quiet)
			{
				/* Truncate key value for display purposes. */
				if (width >= 6)
					newbuf[width-6] = '\0';
				if (!(itrial % 100))
					th_printf(proc_id,"\r U [%s]", newbuf);
			}

			status = tend(proc_id,&tlen,status,&trst,&tcnt,0,ctKEEP);
			if (status)
				th_printf(proc_id,"\n (%dT%d)",itrial,status);
			LKISAM(ctFREE);
			} /* end switch */
		}
		else
#endif
		/* No transaction processing. ****************************************/
		{
			if (LKISAM(ctENABLE_BLK))
			{
				th_printf(proc_id,"\n (%d:%d)\n",itrial,ctGetIsamErrorCode());
				continue;
			}
tlock0:
			if (action == 'B') {
				reclen = sizeof(datblk);
				status = BLKIREC(key,ctBLKIREC_GTE,1,"1",keybuf,&datblk,
					vlen ? &reclen : NULL);
			} else {
				status = GTEREC(key,keybuf,&datblk);
			}
			if (status != 0) {
				if (status == 101 && finds++ < 10)
				{
					LKISAM(ctFREE);
					goto gfind;
				}
				if (status != 42 && status != 101 && !(action == 'B' && status == NTIM_ERR))
					th_printf(proc_id,"\n (%d:%d:%d:%s)\n",
						itrial,status,key,keybuf);
				if (status == 42)
					goto tlock0;
				LKISAM(ctFREE);
				continue;
			}

			switch (action)
			{
#ifdef ctFeatFULLTEXTIDX
case 'F': /* Full text search. ***********************************************/
			datblk.clss[0] = '\0';
			if ((status = ctdbFTSearchOn(hRecord, datblk.name, 0))) {
				th_printf(proc_id,"Error: ctdbFTSearchOn() failed: %d\n",
					status);
				goto exit_thrd;
			}
			nrec = 0;
			status = ctdbFirstRecord(hRecord);
			while (!status) {
				++nrec;
				status = ctdbNextRecord(hRecord);
			}
			if (status != INOT_ERR) {
				th_printf(proc_id,"F: status=%d for [%s]\n",
					status,datblk.name);
			} else if (nrec == 0)
				th_printf(proc_id,"F: nrec=%d for [%s]\n",
					nrec,datblk.name);
			if ((status = ctdbFTSearchOff(hRecord))) {
				th_printf(proc_id,"Error: ctdbFTSearchOff() failed: %d\n",
					status);
				goto exit_thrd;
			}
			LKISAM(ctFREE);
			break;
#endif
case 'G': /* Get. ************************************************************/
case 'B': /* Blocking ISAM read. *********************************************/
			if (!quiet)
			{
				/* Truncate key value for display purposes. */
				if (width >= 6)
					keybuf[width-6] = '\0';
				if (!(itrial % 100))
					th_printf(proc_id,"\r G [%s]", keybuf);
			}
			LKISAM(ctFREE);
			break;

case 'D': /* Delete. *********************************************************/
			if (vlen && (status = DELVREC(datno)) != 0 ||
			    !vlen && (status = DELREC(datno)) != 0)
				th_printf(proc_id,"\n (%d:%d)\n",itrial,status);
			else if (!quiet)
			{
				/* Truncate key value for display purposes. */
				if (width >= 6)
					keybuf[width-6] = '\0';
				if (!(itrial % 100))
					th_printf(proc_id,"\r D [%s]", keybuf);
			}
			LKISAM(ctFREE);
			break;

case 'U': /* Update. *********************************************************/
			cpybuf(&updblk,&datblk,128);
			/* Choose a key to update. */
			switch (getskey((COUNT)(datno+1+dfridx),(COUNT)(datno+3))-datno)
			{
		case 2:
/*
			cpybuf(keybuf,datblk.unqu,NUML);
			setval(updblk.unqu,NUML,NUML,27);
			cpybuf(newbuf,updblk.unqu,NUML);
			newbuf[NUML] = '\0';
			break;
*/
		case 1:
			cpybuf(keybuf,datblk.name,NAML);
			setval(updblk.name,8,NAML,16);
			cpybuf(newbuf,updblk.name,NAML);
			newbuf[NAML] = '\0';
			break;
		case 3:
			cpybuf(keybuf,datblk.clss,CLSL);
			setval(updblk.clss,CLSL,CLSL,4);
			cpybuf(newbuf,updblk.clss,CLSL);
			newbuf[CLSL] = '\0';
			break;
			}
#ifdef ctFeatJSONfldtyp
			{
			LONG	len;
			/* set the json field value */
			ctrt_snprintf(updblk.doc + sizeof(LONG), sizeof(updblk.doc) - sizeof(LONG), "{\"id\": %d}", itrial);
			len = (LONG) strlen(updblk.doc + sizeof(LONG)) + 1;
			memcpy(updblk.doc,&len,sizeof(LONG));
			}
#endif
#ifdef QA_UPDLEN
			if (vlen) {
				if (updbuf) {
					mbfree(updbuf);
				}
				++updlen;
				updbuf = mballc(1,updlen);
				memcpy(updbuf,&updblk,sizeof(updblk));
				if ((status = RWTVREC(datno,updbuf,updlen)) != 0) {
					th_printf(proc_id,"\n (%ld:%d)\n",itrial,status);
				}
			} else if (!vlen) {
				if ((status = DELREC(datno)) != 0) {
					th_printf(proc_id,"\n (%ld:%d)\n",itrial,status);
				} else if ((status = ADDREC(datno,&updblk)) != 0) {
					th_printf(proc_id,"\n (%ld:%d)\n",itrial,status);
				}
			}
#else
			if (vlen && (status = RWTVREC(datno,&updblk,sizeof(updblk))) != 0 ||
			    !vlen && (status = RWTREC(datno,&updblk)) != 0)
				th_printf(proc_id,"\n (%d:%d)\n",itrial,status);
#endif
			else if (!quiet)
			{
				/* Truncate key value for display purposes. */
				if (width >= 6)
					newbuf[width-6] = '\0';
				if (!(itrial % 100))
					th_printf(proc_id,"\r U [%s]", newbuf);
			}
			LKISAM(ctFREE);
			} /* end switch */
		}
#ifdef ctCLIENT
		if (status == ARQS_ERR || status == ARSP_ERR ||
		    status == TUSR_ERR || status == SHUT_ERR ||
		    status == NINT_ERR)
			break;
#endif
#ifdef ctFeatCLUSTER_OPTIONS
		if (pOptions->failover && status == ROSV_ERR)
			break;
#endif
	}

exit_thrd:
	if (lstart)
	{
		lstop = sysdate(&ct_tm);
		ctrt_asctime_r(&ct_tm, timebuf);
		th_printf(proc_id,"\nEnd time: %s", timebuf);
#ifdef ctLOGTIMESTODISK
		fprintf(fpTIMES,"%s  End time     : %s ",th_str,timebuf);
#endif
		rate(proc_id,lstop,lstart,trials,th_str);
		th_printf(proc_id, "***** Successful Run\n\n");
	}
#ifdef QA_TRANRECOV
	{
	NINT	rc;
	TEXT	killcmd[32];
	TEXT	newnam[MAX_NAME];

	if (!qanewfil)
		qanewfil = "";
	switch (qaaction) {
case QAPRMIDX1:
		rc = CLIFIL(&markdata);
		rc = OPNIFIL(&markdata);
		markdata.dfilno = markdata.tfilno;
		markdata.tfilno = -1;
		ctrt_snprintf(newnam,sizeof(newnam),"%s.idx", qanewfil);
		markdata.ix[0].aidxnam = newnam;
		if ((rc = PRMIIDX(&markdata))) {
			printf("Error: PRMIIDX failed: %d\n", rc);
			exit(1);
		} else {
			printf("Successfully added index.\n");
			if (qakillpid) {
				ctrt_snprintf(killcmd,sizeof(killcmd),QAKILLCMD " -9 %d", qakillpid);
				system(killcmd);
			}
			ctThrdSleep(1000);
			exit(0);
		}
		break;
case QARENAME1:
		rc = CLIFIL(&markdata);
		markdata.dfilmod &= ~ctNONEXCLUSIVE;
		markdata.ifilmod &= ~ctNONEXCLUSIVE;
		rc = OPNIFIL(&markdata);
		markdata.dfilno = markdata.tfilno;
		markdata.pfilnam = qanewfil;
		ctrt_snprintf(newnam,sizeof(newnam),"%s.idx", qanewfil);
		markdata.ix[0].aidxnam = newnam;
		TRANBEG(ctTRNLOG | ctENABLE);
		if ((rc = RENIFIL(&markdata))) {
			printf("Error: RENIFIL failed: %d\n", rc);
			exit(1);
		} else {
			printf("Successfully renamed files.\n");
			TRANEND(ctFREE);
			if (qakillpid) {
				ctrt_snprintf(killcmd,sizeof(killcmd),QAKILLCMD " -9 %d", qakillpid);
				system(killcmd);
			}
			ctThrdSleep(1000);
			exit(0);
		}
		break;
case QADELETE1:
		rc = CLIFIL(&markdata);
		markdata.dfilmod &= ~ctNONEXCLUSIVE;
		markdata.ifilmod &= ~ctNONEXCLUSIVE;
		rc = OPNIFIL(&markdata);
		markdata.dfilno = markdata.tfilno;
		markdata.pfilnam = qanewfil;
		ctrt_snprintf(newnam,sizeof(newnam),"%s.idx", qanewfil);
		markdata.ix[0].aidxnam = newnam;
		TRANBEG(ctTRNLOG | ctENABLE);
		if ((rc = DELIFIL(&markdata))) {
			printf("Error: DELIFIL failed: %d\n", rc);
			exit(1);
		} else {
			printf("Successfully deleted files.\n");
			TRANEND(ctFREE);
			if (qakillpid) {
				ctrt_snprintf(killcmd,sizeof(killcmd),QAKILLCMD " -9 %d", qakillpid);
				system(killcmd);
			}
			ctThrdSleep(1000);
			exit(0);
		}
		break;
case QAFILEID1:
		{
		ULONG	timeid;

		rc = CLIFIL(&markdata);
		markdata.dfilmod &= ~ctNONEXCLUSIVE;
		markdata.ifilmod &= ~ctNONEXCLUSIVE;
		rc = OPNIFIL(&markdata);
		markdata.dfilno = markdata.tfilno;
		markdata.pfilnam = qanewfil;
		ctrt_snprintf(newnam,sizeof(newnam),"%s.idx", qanewfil);
		markdata.ix[0].aidxnam = newnam;
		TRANBEG(ctTRNLOG | ctENABLE);
		if ((timeid = GETFIL(markdata.tfilno,TIMEID)) == -1) {
			printf("Error: GETFIL(TIMEID) failed: %d\n", rc);
			exit(1);
		}
		timeid++;
		if ((rc = PUTHDR(markdata.tfilno,timeid,ctTIMEIDhdr))) {
			printf("Error: PUTHDR failed: %d\n", rc);
			exit(1);
		} else {
			printf("Successfully changed file ID.\n");
			TRANEND(ctFREE);
			if (qakillpid) {
				ctrt_snprintf(killcmd,sizeof(killcmd),QAKILLCMD " -9 %d", qakillpid);
				system(killcmd);
			}
			ctThrdSleep(1000);
			exit(0);
		}
		}
		break;
	}
	}
#endif
#ifdef QA_TRAN_NONTRAN
    if (tt % 2) {
	if (markdata.dfilmod & ctTRNLOG) {
		markdata.dfilmod &= ~ctTRNLOG;
		markdata.ifilmod &= ~ctTRNLOG;
		TRANEND(ctFREE);
	} else {
		markdata.dfilmod |= ctTRNLOG;
		markdata.ifilmod |= ctTRNLOG;
	}
	CLRFIL(datno);
	if ((datno = OPNRFILX(-1, filnamx,
	    ctEXCLUSIVE, NULL)) == -1) {
		th_printf(proc_id,"Could not open files (%d,%d,%d)\n",
			ctGetIsamErrorCode(), ctGetIsamFileNbr(), ctGetSystemErrorCode());
		if (ctGetIsamErrorCode() == FNOP_ERR)
			th_printf(proc_id, "Have you created the files?\n");
		goto exit_thrd;
	}
	markdata.dfilmod &= ~ctNONEXCLUSIVE;
	markdata.ifilmod &= ~ctNONEXCLUSIVE;
	for (itrial=0; itrial<2; itrial++) {
		if ((status = PUTFIL((COUNT) (datno + itrial),itrial == 0 ? markdata.dfilmod : markdata.ifilmod))) {
			th_printf(proc_id, "Could not switch tran mode for file %d: %d\n",
				itrial,status);
			goto exit_thrd;
		}
	}
	markdata.dfilmod |= ctSHARED;
	markdata.ifilmod |= ctSHARED;
    }
	CLRFIL(datno);
	{
 	pTEXT	argvlist[] = {
		"ctvfyfil", "-excl", "-chkdatkeys", "-chkidxrec", "-u", uid,
		"-p", upw, "-s", serverName, "mark.dat", NULL
	};
	/* Check state of file. */
	if ((status = vfyfil_main(sizeof(argvlist)/sizeof(*argvlist),argvlist))) {
		th_printf(proc_id, "Error: File verification failed.\n");
		goto exit_thrd2;
	}
	}
	++tt;
	action = 3 + 2*'A' - action;
	goto again;
exit_thrd2:
#endif /* QA_TRAN_NONTRAN */

#ifdef ctFeatFULLTEXTIDX
	if (action == 'F') {
		ctdbterm(lgon,&hSession,&hRecord,&hTable,&hDatabase);
	} else
#endif
	{
		STPUSR();
	}
#ifdef ctFeatMULTIDLLS
	if ((proc_id+1) % 2) {
		if ((status = ctThrdDetach()))
			th_printf(proc_id, "Error: Unable to detach thread: %d\n", status);
		ctAppSwitchCtreeEnv(CTTYP_SRV);
	}
#endif
#ifdef ctFeatCLUSTER_OPTIONS
	if (pOptions->failover && (status == SERVER_FAILOVER_ERR || chklostconn(status) || status == ROSV_ERR)) {
		printf("Lost connection. Reconnecting...\n");
		reconnecting = YES;
		goto reconnect;
	}
#endif
    	mbfree(pOptions);

	ctThrdMutexGet(&tfin_mtx);
	++tfin;
	totops += itrial;
	ctThrdMutexRel(&tfin_mtx);
	ctrt_snprintf(qmsg, sizeof(qmsg), "Thread %d finished.", proc_id+1);
	ctThrdQueueWrite(qid, qmsg, (NINT)strlen(qmsg)+1);
	ctThrdExit();

        ctrt_printf(
"ctThrdExit should not return. Call FairCom Tech Support\n");

	return(0);
} /* tcode() */


/* ************************************************************************* */
/* ************************************************************************* */

/*
** The following global data and file definitions only to be used by the main
** thread at start-up.
**
** NOTE: global variables and static stack variables are not appropriate for
** use by individual threads unless the variables are not changed by threads,
** and are not meant to be different for each thread. In the event that global
** variables are meant to be changed by individual threads, then a Mutex must
** be used to control access.
*/

ISEG segments[] = {	/* Key segment definitions. */
		{SegOff(DREC,name),16,REGSEG},
		{SegOff(DREC,unqu),NUML,REGSEG},
		{SegOff(DREC,clss),CLSL,REGSEG},
#ifdef ctAppNOTIFY_LOCAL_COPY
		{SegOff(DREC,sern),SERL,INTSEG}
#else
		{SegOff(DREC,sern),SERL,SRLSEG}
#endif
	};

IIDX ndxs[] = {		/* Index definitions. */
		{20,COL_BOTH,1,1,32,1, segments},
		{10, 0,1,0, 0,1,&segments[1]},
#ifdef ctAppNOTIFY
		{ CLSL+SERL, 0,0,0, 0,2,&segments[2]}
#else
		{4+CLSL+SERL, 0,1,1,32,2,&segments[2]}
#endif
#ifdef ctFeatFULLTEXTIDX
		,{SERL, 0,0,0,0,1,&segments[3],"$ROWID$"}
#endif
	};

IFIL markdata = {	/* Data and index file definitions. */
		"",
		-1,			/* file #        */
		128,			/* record length */
		8192,			/* dat xtdsiz	 */
		ctSHARED | ctPERMANENT,	/* dat file mode */
#ifdef ctFeatFULLTEXTIDX
		4,			/* nbr of idxs	 */
#else
		3,			/* nbr of idxs	 */
#endif
		0,			/* idx xtdsiz	 */
		ctSHARED | ctPERMANENT,	/* idx file mode */
		ndxs			/* idx defs	 */
	};

/*
 * appGetArgs: Process command-line arguments or prompt user.
 */
static NINT appGetArgs (NINT argc, ppTEXT argv, pAPPOPTS pOptions)
{
	pTEXT	tp;		/* Pointer to current command line argument. */
	NINT	rc = NO_ERROR;	/* Return code.		*/
	TEXT	fc;		/* File creation flag.	*/
#ifdef ctFeatMULTIDLLS
	NINT	CURENV = pCURENV[ctAppGetTid()];
#endif
#ifdef ctHUGEFILE
	XCREblk xcreblk[2];
#endif
#ifdef TRANPROC
	TEXT	tranActive = NO;
#endif

	pOptions->svn = mballc(1,(UINT)MAX_NAME);
#ifdef ctCLIENT
	pOptions->targetServerName = mballc(1,(UINT)MAX_NAME);
#endif
	pOptions->uid = mballc(1,(UINT)MAX_NAME);
	pOptions->upw = mballc(1,(UINT)MAX_NAME);
	pOptions->enc = mballc(1,IDZ);

	/* DEFAULTS */
	pOptions->action   = 'G';
	pOptions->filestat = 'n';
	pOptions->mode     = 'N';
	pOptions->trials   = 100L;
	ctrt_strncpy(pOptions->svn, SVN_DEFAULT, MAX_NAME);
#ifdef ctCLIENT
	pOptions->targetServerName[0] = '\0';
#endif
	ctrt_strncpy(pOptions->uid, UID_DEFAULT, MAX_NAME);
	ctrt_strncpy(pOptions->upw, UPW_DEFAULT, MAX_NAME);
	pOptions->enc[0] = '\0';
	pOptions->filnam = "";
	pOptions->filnamx[0] = '\0';
#ifdef ctCMPREC
	pOptions->compress = NO;
#endif
#ifdef ctFeatFLEXREC
	pOptions->flexrec = NO;
#endif
#ifdef ctFeatVARLENnodefmt
	pOptions->vlennod = NO;
	pOptions->trnmrkopt = NO;
	pOptions->cmpoff = NO;
#endif
#ifdef ctFeatCLUSTER_OPTIONS
	pOptions->failover = NO;
	pOptions->unmanagedFailover = NO;
#endif
	pOptions->replicate= NO;
	pOptions->dfridx   = NO;
#ifdef ctHUGEFILE
	pOptions->my_huge     = NO;
#endif
#ifdef ctFeatFULLTEXTIDX
	pOptions->fultxtidx = NO;
#endif
#ifdef ctFeatJSONfldtyp
	pOptions->jsonidx  = NO;
#endif
	pOptions->seed     = 1;
	pOptions->trnlen   = 1;
	pOptions->maxfiles = 0;
	pOptions->threads  = 1;
	pOptions->vlen     = NO;
	pOptions->trepl    = NO;
	pOptions->opnfiles = 0;
	pOptions->timeLimitSec = 0;
	pOptions->quiet    = NO;
	pOptions->changeCache = NO;
	pOptions->scannerPage = NO;
	pOptions->IDfield = NO;

	if (argc <= 1) /* if command line args not entered */
	{
		appDescribe();

		pOptions->threads = getcnt((COUNT)pOptions->threads, "Number of Threads");
		pOptions->action  = toupper(getchr(pOptions->action, "Action; A)dd, G)et, D)elete, U)pdate"));
		pOptions->trials  = getlng((LONG)pOptions->trials, "Number of trials");
		fc      = getchr(pOptions->filestat, "Create new files? Y)es, N)o");
		if ((fc == 'y') || (fc == 'Y'))
		{
			pOptions->filestat = 'c';
#ifdef TRANPROC
			pOptions->mode    = getchr(pOptions->mode, "Transaction mode; full T)ran, no log t)ran, N)o tran");
			if (pOptions->mode == 'T')
			{
				markdata.dfilmod |= ctTRNLOG;
				markdata.ifilmod |= ctTRNLOG;
			}
			else if (pOptions->mode == 't')
			{
				markdata.dfilmod |= ctPREIMG;
				markdata.ifilmod |= ctPREIMG;
			}
#endif
		}
		else
		{
			ctrt_printf("\nNote: Will use transaction mode set at file creation time.\n");
			mode = 'D';
		}

		pOptions->seed    = getcnt(pOptions->trnlen, "Random number seed");
#ifdef ctCLIENT
		pOptions->svn     = getstr2(pOptions->svn, "Server name");
#endif

		ctrt_printf("\nParameters are:  threads [%d] action [%c] trials [%d]\n",
			pOptions->threads, pOptions->action, pOptions->trials);
		ctrt_printf("                 mode [%c] create [%c] seed [%d]\n",
			pOptions->mode, pOptions->filestat, pOptions->seed);
		ctrt_printf("\nPress ENTER to continue...");
		getchar();
	}
	else
	{
	        /* COMMAND LINE */
	        while (--argc > 0)
		{
			tp = *++argv;
			if (!strncmp(*argv,"qa:",3)) {
				if (!strncmp(*argv + 3,"i1:",3)) {
					qaaction = QAPRMIDX1;
					qanewfil = *argv + 6;
				} else if (!strncmp(*argv + 3,"r1:",3)) {
					qaaction = QARENAME1;
					qanewfil = *argv + 6;
				} else if (!strncmp(*argv + 3,"kp:",3)) {
					qakillpid = atol(*argv + 6);
				} else if (!strcmp(*argv + 3,"d1")) {
					qaaction = QADELETE1;
				} else if (!strcmp(*argv + 3,"f1")) {
					qaaction = QAFILEID1;
				} else if (!strcmp(*argv + 3,"cache")) {
					qaaction = QACACHEUSE;
				} else {
					ctrt_printf("Error: Unsupported qa: option %s\n",
						*argv);
					goto err_ret;
				}
#ifndef QA_TRANRECOV
				if (qaaction) {
					ctrt_printf("Error: The qa: option requires this test program to be compiled with #define QA_TRANRECOV.\n");
					goto err_ret;
				}
#endif
			} else
			switch (*tp)
			{
case 'A':
case 'a':		/* Action. */
			pOptions->action = toupper(*++tp);
			/*
			** CTUSERX() test option. The option format is:
			**
			** aX(i,o) where i specifies the CTUSERX() input buffer
			** size and o specifies the CTUSERX() output buffer
			** size in bytes.
			*/
			if (pOptions->action == 'X') {
#ifdef ctBNDSRVR
				ctrt_printf("This operational model does not support the CTUSERX() option.\n");
				goto err_ret;
#else
				pTEXT	sp;

				sp = tp + 1;
				if (*sp == '(') {
					++sp;
					inbufsiz = atoi(sp);
					while (*sp && *sp != ',')
						++sp;
					if (*sp == ',') {
						++sp;
						outbufsiz = atoi(sp);
					}
				}
#endif
			}
			break;
case 'E':
case 'e':		/* Encrypt files. */
			ctrt_strncpy(pOptions->enc, tp+1, IDZ);
			break;
#ifdef ctCMPREC
case 'C':
case 'c':
#ifdef ctBlockCompress
			if (*(tp + 1) == 'b') {
				pOptions->compress = HYS;
				break;
			}
#endif
			pOptions->compress = YES;
			break;
#endif
case 'D':
case 'd':
			pOptions->dfridx = YES;
			break;
case 'F':
case 'f':		/* Create files. */
			pOptions->filestat = *++tp;
			break;
#ifdef TRANPROC
case 'G':
case 'g':		/* Transaction length (operations per transaction). */
			pOptions->trnlen = atoi(++tp);
			break;
case 'H':
case 'h':
			pOptions->filnam = ++tp;
			ctrt_snprintf(pOptions->filnamx, sizeof(pOptions->filnamx), "%s.dat",pOptions->filnam);
			break;
case 'L':
case 'l':
			pOptions->timeLimitSec = atoi(++tp);
			break;
case 'M':
case 'm':		/* Transaction mode or other options. */
			pOptions->mode = *++tp;
			if (pOptions->mode == 'T') /* mT */
			{
				markdata.dfilmod |= ctTRNLOG;
				markdata.ifilmod |= ctTRNLOG;
			}
			else if (pOptions->mode == 't') /* mt */
			{
				markdata.dfilmod |= ctPREIMG;
				markdata.ifilmod |= ctPREIMG;
			}
			else if (pOptions->mode == 'p') /* mp */
			{
				abortFactor = atoi(++tp);
				if (abortFactor < 0)
					abortFactor = 0;
			}
			else if (pOptions->mode == 'r') /* mr */
			{
				pOptions->replicate = YES;
				if (ndxs[2].ikeydup) {
					ndxs[2].ikeylen -= 4;
					ndxs[2].ikeydup = NO;
					ndxs[2].inulkey = NO;
				}
			}
			else if (pOptions->mode == 's') /* ms */
			{
#ifdef ctCLIENT
				/* Target server name. */
				ctrt_strncpy(pOptions->targetServerName, tp+1, MAX_NAME);
#endif
			}
#ifdef ctFeatFLEXREC
			else if (pOptions->mode == 'f') /* mf */
				pOptions->flexrec = YES;
#endif
			else if (pOptions->mode == 'l') /* ml */
			{
				pOptions->trepl = YES;
			}
#ifdef ctHUGEFILE
			else if (pOptions->mode == 'h') /* mh */
			{
				pOptions->my_huge = YES;
				ndxs[0].ikeylen += 4;
				ndxs[1].ikeylen += 4;
				ndxs[2].ikeylen += 4;
			}
#endif
#ifdef ctFeatFULLTEXTIDX
			else if (pOptions->mode == 'u') /* mu */
				pOptions->fultxtidx = YES;
#endif
#ifdef ctFeatJSONfldtyp
			else if (pOptions->mode == 'j') { /* mj */
				pOptions->jsonidx = YES;
				pOptions->vlen = YES;
			}
#endif
#ifdef ctFeatVARLENnodefmt
			else if (pOptions->mode == 'x') /* mx */
				pOptions->vlennod = YES;
			else if (pOptions->mode == 'm') /* mm */
				pOptions->trnmrkopt = YES;
			else if (pOptions->mode == 'c') /* mc */
				pOptions->cmpoff = YES;
#endif
#ifdef ctFeatCLUSTER_OPTIONS
			else if (pOptions->mode == 'q') { /* mq */
				pOptions->failover = YES;
				if (*++tp == 'u')
					pOptions->unmanagedFailover = YES; /* mqu */
			}
#endif
			else if (pOptions->mode == 'a') /* ma */
				pOptions->changeCache = YES;
			else if (pOptions->mode == 'g') /* mg */
				pOptions->scannerPage = YES;
			else if (pOptions->mode == 'd') /* md */
				pOptions->IDfield = YES;
			break;
#endif
case 'N':
case 'n':		/* Number of trials. */
			pOptions->trials = atol(++tp);
			break;
case 'O':
case 'o':		/* Open additional files. */
			pOptions->opnfiles = atoi(tp+1);
			break;
case 'P':
case 'p':		/* User password. */
			ctrt_strncpy(pOptions->upw, tp+1, MAX_NAME);
			break;
case 'Q':
case 'q':
			pOptions->quiet = YES;
			break;
case 'R':
case 'r':
			if (*(tp + 1) == 'r') { /* rr */
				allrounds = YES;
				break;
			}
			/* Random number seed. */
			pOptions->seed = atoi(++tp);
			break;
case 'S':
case 's':		/* Server name. */
			ctrt_strncpy(pOptions->svn, tp+1, MAX_NAME);
			break;
case 'U':
case 'u':		/* User name. */
			ctrt_strncpy(pOptions->uid, tp+1, MAX_NAME);
			break;
case 'V':
case 'v':
			pOptions->vlen = YES;
			break;
case 'X':
case 'x':		/* Maximum number of files. */
			pOptions->maxfiles = atoi(++tp);
			break;
case 'T':
case 't':		/* Number of threads. */
			pOptions->threads = atoi(++tp);
			break;
			}
		}
	}  /* end command line */

#ifdef ctCLIENT
#ifndef ctPortWINKE_CLIENT
	if (pOptions->maxfiles)
		ctmaxparm = pOptions->maxfiles;
#endif
#endif

	if (!strchr("ABDGUSTX", pOptions->action)
#ifdef ctFeatFULLTEXTIDX
	    && pOptions->action != 'F'
#endif
	) {
		ctrt_printf("Invalid action '%c'\n", pOptions->action);
		goto err_ret;
	}

	ctrt_printf("\ntrials=%d mode=%c seed=%d action=%c trnlen=%d threads=%d\n",
		pOptions->trials, pOptions->mode, pOptions->seed,
		pOptions->action, pOptions->trnlen, pOptions->threads);

#ifdef ctmarkLOGTODISK
	if ((fpTIMES = fopen("TIMES.LOG","at")) == NULL)
	{
		ctrt_printf("Cannot open TIMES.LOG file!  Program will now exit!");
		goto err_ret;
	}
	fprintf(fpTIMES,"\ntrials=%ld mode=%c seed=%d action=%c trnlen=%d threads=%d\n\n",
		pOptions->trials, pOptions->mode, pOptions->seed,
		pOptions->action, pOptions->trnlen, pOptions->threads);
#endif

	if (pOptions->threads < 1)
	{
		ctrt_printf("The minimum number of threads is 1.\n");
		goto err_ret;
	}

	if (pOptions->threads > MAXTRD && !pOptions->quiet)
	{
		ctrt_printf("\n\n  NOTE: You have chosen to run %d threads;\
 all %d threads will run but only\n", pOptions->threads, pOptions->threads);
		ctrt_printf("        %d threads will be shown on-screen.\n", MAXTRD);
		ctrt_printf("\nPress ENTER to continue...");
		getchar();
	}

	if (!pOptions->filnam[0]) {
		/* File name not specified, so use default. */
		if (pOptions->vlen)
			pOptions->filnam = "markv";
		else
			pOptions->filnam = "mark";
		ctrt_snprintf(pOptions->filnamx, sizeof(pOptions->filnamx), "%s.dat",pOptions->filnam);
	}

	if (pOptions->vlen)
	{
		markdata.dfilmod |= ctVLENGTH;
	}

	markdata.pfilnam = pOptions->filnam;
	if (pOptions->dfridx) {
		markdata.ix[0].ikeytyp |= KTYP_DFRIDX;
	}

	/* Create data and index files if specified. */
	if (pOptions->filestat == 'c' || pOptions->filestat == 'C')
	{
#ifdef ctFeatFULLTEXTIDX
		CTHANDLE hSession;
		CTHANDLE hDatabase;
		CTHANDLE hTable;
		CTHANDLE hRecord;

		if (pOptions->fultxtidx) {
			if ((rc = ctdbinit(&hSession,&hDatabase,&hTable,&hRecord,"test")))
				goto err_ret;
			rc = ctdbDeleteTable(hDatabase,pOptions->filnam,NULL);
			ctdbterm(YES,&hSession,&hRecord,&hTable,&hDatabase);
		}
#endif
		if (INTISAMX(16, 12, 16, 16, 0, pOptions->uid, pOptions->upw, pOptions->svn))
		{
			ctrt_printf("\nCould not init (%d,%d)\n", ctGetIsamErrorCode(), ctGetIsamFileNbr());
			goto err_ret;
		}

		/* Attempt to delete existing data and index files. */
		if ((markdata.tfilno = OPNRFIL(-1,pOptions->filnamx,ctEXCLUSIVE)) >= 0) {
#ifdef QA_TRANRECOV
			TRANBEG(ctTRNLOG | ctENABLE);
#endif
			if (!DELRFIL(markdata.tfilno))
				ctrt_printf("\nDeleted existing data and index files.\n");
#ifndef QA_TRANRECOV
			if (ctGetUserErrorCode() == TNON_ERR) {
				TRANBEG(ctTRNLOG | ctENABLE);
				if (!DELRFIL(markdata.tfilno))
					ctrt_printf("\nDeleted existing data and index files.\n");
				TRANEND(ctFREE);
			}
#endif
#ifdef QA_TRANRECOV
			TRANEND(ctFREE);
#endif
			markdata.tfilno = -1;
		}

		if (pOptions->enc[0]) {
			TEXT	enckey[PWZ+1];

			memset(enckey, 0xbf, PWZ);
			enckey[PWZ] = '\0';
			if ((rc = ctSETENCRYPT(pOptions->enc, enckey, PWZ))) {
				ctrt_printf("\nCould not enable file encryption: %d\n", rc);
				goto err_ret;
			}

			ctrt_printf("\nSuccessfully enabled encryption for the file.\n");
		}

#ifdef TRANPROC
		if (pOptions->replicate) {
			TRANBEG(ctTRNLOG | ctENABLE);
			tranActive = YES;
		}
#endif
#ifdef ctCMPREC
		if (pOptions->compress
#ifdef ctFeatFLEXREC
		    || pOptions->flexrec
#endif
#ifdef QA_UPDLEN
		    || !pOptions->vlen
#endif
		) {
			memset(xcreblk, 0, sizeof(xcreblk));
#ifdef QA_UPDLEN
			if (!pOptions->vlen) {
				/* Disable space management in fixed length data
				file. */
				xcreblk[0].x8mode = ctADD2END;
			}
#endif
			if (pOptions->compress == YES
#ifdef ctFeatFLEXREC
			    || pOptions->flexrec
#endif
			) {
				if (!pOptions->vlen)
					xcreblk[0].splval = ctAugmentedFxd;
				markdata.dfilmod |= ctVLENGTH;
			}
			if (pOptions->compress == YES) {
				xcreblk[0].splval |= ctCompressRec;
				if ((rc = ctSETCOMPRESS(ctCMPRECzlib, 1, NULL, NULL, 0))) {
					ctrt_printf("\nCould not enable compression for %s (%d,%d)\n",
						markdata.pfilnam, ctGetIsamErrorCode(), ctGetIsamFileNbr());
					goto err_ret;
				}
			}
#ifdef ctBlockCompress
			else if (pOptions->compress == HYS) {
				/* Enable block compression for data and index files. */
				xcreblk[0].splval |= ctBlockCompress;
				xcreblk[1].splval |= ctBlockCompress;
				/* Disable key compression. */
				ndxs[0].ikeytyp = 0;
			}
#endif
#ifdef ctFeatFLEXREC
			if (pOptions->flexrec)
				xcreblk[0].splval |= ctFLEXREC;
#endif
#ifdef ctFeatVARLENnodefmt
			if (pOptions->vlennod) {
				NINT	i;
				for (i=0; i<markdata.dnumidx; i++ ) {
					markdata.ix[i].ikeytyp |= KTYP_VLENNOD;
					/* turn off prefix compression because the new node format doesn't support it */
					markdata.ix[i].ikeytyp &= ~COL_PREFIX;
					if (pOptions->trnmrkopt)
						markdata.ix[i].ikeytyp |= KTYP_TRNMRKOPT;
					if (pOptions->cmpoff)
						markdata.ix[i].ikeytyp |= KTYP_CMPOFFSET;
				}
			}
#endif
#ifdef QA_TRANRECOV
			if (markdata.dfilmod & ctSHADOW) {
				xcreblk[0].x8mode |= ctTRANDEP;
				xcreblk[1].x8mode |= ctTRANDEP;
			}
#endif

			/* Create new data and index files. */
			rc = CREIFILX8(&markdata, NULL, NULL, 0L, NULL, NULL, xcreblk);
		} else
#endif
		{
#ifdef ctHUGEFILE
			memset(xcreblk, 0, sizeof(xcreblk));
			if (pOptions->my_huge) {
				xcreblk[0].x8mode = ctFILEPOS8;
				xcreblk[1].x8mode = ctFILEPOS8;
			}
#ifdef ctFeatVARLENnodefmt
			if (pOptions->vlennod) {
				NINT	i;
				for (i=0; i<markdata.dnumidx; i++ ) {
					markdata.ix[i].ikeytyp |= KTYP_VLENNOD;
					/* turn off prefix compression because the new node format doesn't support it */
					markdata.ix[i].ikeytyp &= ~COL_PREFIX;
					if (pOptions->trnmrkopt)
						markdata.ix[i].ikeytyp |= KTYP_TRNMRKOPT;
					if (pOptions->cmpoff)
						markdata.ix[i].ikeytyp |= KTYP_CMPOFFSET;
				}
			}
#endif
#ifdef QA_TRANRECOV
			if (markdata.dfilmod & ctSHADOW) {
				xcreblk[0].x8mode |= ctTRANDEP;
				xcreblk[1].x8mode |= ctTRANDEP;
			}
#endif
			/* Create new data and index files. */
			rc = CREIFILX8(&markdata, NULL, NULL, 0L, NULL, NULL, xcreblk);
#else
			rc = CREIFILX(&markdata, NULL, NULL, 0L, NULL, NULL);
#endif
		}
		if (rc)
		{
			ctrt_printf("\nCould not create %s (%d,%d,%d)\n",
				markdata.pfilnam, ctGetIsamErrorCode(), ctGetIsamFileNbr(), ctGetSystemErrorCode());
			goto err_ret;
		}

		if (pOptions->replicate) {
			if ((rc = PUTHDR(markdata.tfilno,YES,ctREPLICATEhdr))) {
				ctrt_printf("Error: Failed to enable replication for file: %d\n",
					rc);
				goto err_ret;
			}
		}

#ifdef ctFeatJSONfldtyp
		if (!pOptions->vlen) {
			/* For a fixed length data file, a CT_JSON field is not
			supported, so use CT_FSTRING instead. */
			doda[6].ftype = CT_FSTRING;
		}
#endif
		if (pOptions->IDfield) {
			doda[6].ftype = CT_INT4U;
		}
		if (PUTDODA(markdata.tfilno, doda, (UCOUNT)7))
			ctrt_printf("\nError on PUTDODA (%d)", ctGetUserErrorCode());

#ifdef ctFeatJSONfldtyp
		if (pOptions->jsonidx) {
			ISEG	json_iseg = {6, 4, SCHSEG};
			IIDX	json_iidx = {8, 0, YES, NO, 0, 1, &json_iseg};
			pctKSEGDEF	pksegs[1];
			ctKSEGDEF sd;
			TEXT	buf[MAX_NAME];

			ctrt_snprintf(buf, sizeof(buf), "%s_doc_id.idx", markdata.pfilnam);
			json_iidx.aidxnam = buf;

			/* Set the extended segment info. */
			memset(&sd,0,sizeof(sd));
			sd.kseg_type = ctKSEG_TYPE_JSON;
			/* use source type from schema */
			sd.kseg_styp = ctKSEG_STYP_PROVIDED;
			/* set key segment length */
			sd.kseg_ssiz = 4;
			/* set c-tree data type of key value */
			sd.kseg_comp = CT_INT4;
			/* set the json key name that is being indexed */
			ctrt_strncpy(sd.kseg_desc,"id",sizeof(sd.kseg_desc));

			pksegs[0] = &sd;
			if ((rc = PUTKSEGDEF(ctKSEGcreidx,1,(pctKSEGDEF) pksegs))) {
				printf("Error: Failed to set extended key segment information for new index: %d\n",
					rc);
				goto err_ret;
			}

			markdata.dnumidx = 1;
			markdata.ix = &json_iidx;
			markdata.dfilno = markdata.tfilno;
			markdata.tfilno = -1;
#ifdef ctHUGEFILE
			if (pOptions->my_huge) {
				json_iidx.ikeylen += 4;
			}
#endif
			if ((rc = PRMIIDX(&markdata))) {
				printf("Error: Failed to create index on json doc field: %d\n",
					rc);
				goto err_ret;
			}
			printf("Successfully created index on json doc field.\n");
		}
#endif
		if (pOptions->IDfield) {
			if ((rc = addIDfield(markdata.tfilno, 1, 1, 6))) {
				printf("Error: Failed to add identity field: %d\n",
					rc);
				goto err_ret;
			}
		}
#ifdef TRANPROC
		if (tranActive) {
			rc = TRANEND(ctFREE);
			tranActive = NO;
		}
#endif

#ifdef ctFeatFULLTEXTIDX
		if (pOptions->fultxtidx) {
			CLIFIL(&markdata);
			rc = ctdbinit(&hSession,&hDatabase,&hTable,&hRecord,"test");
			if (!rc) {
				rc = ctdbinit2(hSession,hDatabase,hTable,pOptions->filnam,YES);
			}
			ctdbterm(YES,&hSession,&hRecord,&hTable,&hDatabase);
			if (rc) {
				STPUSR();
				goto err_ret;
			}

		}
#endif
		STPUSR();
	}

	/* Set globals. */
	mode	= pOptions->mode;
	svn	= pOptions->svn;
	uid	= pOptions->uid;
	upw	= pOptions->upw;
	trnlen	= pOptions->trnlen;
	quiet	= pOptions->quiet;
	seed	= pOptions->seed;
	action	= pOptions->action;
#ifdef TRANPROC
	filmod  = (COUNT)-1;
#endif

	return (rc ? 1 : 0);

err_ret:
#ifdef TRANPROC
	if (tranActive) {
		TRANABT();
	}
#endif
	return (1);
} /* appGetArgs() */


/* ************************************************************************* */
/* ************************************************************************* */

/*
 * appThrdCreate: Create threads which execute the tcode() function.
 */

NINT appThrdCreate(pAPPOPTS pOptions)
{
	NINT	nthrd = 0;
	NINT	i;
#ifdef ctFeatMULTIDLLS
	NINT	CURENV = pCURENV[ctAppGetTid()];
#endif

	if (pOptions->changeCache)
	{
		if (ctThrdCreate(cacheSizeThread, (pVOID)(NLONG)pOptions->threads, NULL, 0) == NO_ERROR)
		{
			++nthrd;
		}
	}
	for (i=0; i<pOptions->threads; i++)
	{
		pAPPOPTS	pOptions2 = (pAPPOPTS)mballc(1,ctSIZE(APPOPTS));
		cpybuf(pOptions2, pOptions, ctSIZE(APPOPTS));
		pOptions2->round = (LONG)(i+1 + 100*(pOptions->round-1));

		if ((pOptions->round == 3) && !i)
		{
			/* Initialize counter which tracks number of completed threads. */
			ctThrdMutexGet(&tfin_mtx);
			tfin = 0;
			ctThrdMutexRel(&tfin_mtx);
		}

		if (ctThrdCreate(tcode,(pVOID)pOptions2,(pVOID)0,(LONG)0) == NO_ERROR)
		{
			++nthrd;
#ifdef WTMTAP_C
			ctrt_printf("\tappThrdCreate: Created thread #%d.\n", i+1);
#endif /* WTMTAP_C */
		}
	}

	return (nthrd);
} /* appThrdCreate() */


/* ************************************************************************* */
/* ************************************************************************* */

/*
 * appThrdWait: Wait on c-tree Plus threads to complete.
 */
#ifdef PROTOTYPE
NINT appThrdWait(pAPPOPTS pOptions, NINT nthrd)
#else
NINT appThrdWait(pOptions, nthrd)
pAPPOPTS	pOptions;
NINT		nthrd;
#endif
{
	NINT	qlen;		/* Number of characters in queue. */
	NINT	rc;		/* Return code. */
	TEXT	qbuf[32];	/* To read in queue contents. */
#ifdef ctFeatMULTIDLLS
	NINT	CURENV = pCURENV[ctAppGetTid()];
#endif

	if (pOptions->round != 3)
	{
#ifdef WTMTAP_C
		ctrt_printf("\tappThrdWait: Using inter-thread FIFO queues for notification.\n");
#endif /* WTMTAP_C */
		/* Use inter-thread FIFO queues to wait for all threads to finish. */
		/* Each thread places a message in the queue before exiting. */
		while (nthrd)
		{				/* FairCom MT API: Please note #04 */
			/* Wait indefinitely for a message; get its length. */
#ifdef WTMTAP_C
			ctrt_printf("\tappThrdWait: Waiting for message to arrive in queue.\n");
#endif /* WTMTAP_C */
			if (pOptions->timeLimitSec) {
				if ((qlen = ctThrdQueueMlen(qid,pOptions->timeLimitSec * 1000)) == -NTIM_ERR) {
					printf("Time limit exceeded. Signaling threads to stop.\n");
					sigflg = YES;
					/* wait for threads to stop */
					qlen = ctThrdQueueMlen(qid,ctWAITFOREVER);
				}
			} else {
				qlen = ctThrdQueueMlen(qid,ctWAITFOREVER);
			}

			/* Retrieve the message we waited for. */
			*qbuf = '\0';
			rc = ctThrdQueueRead(qid,qbuf,sizeof(qbuf)-1,ctNOWAIT);
#ifdef WTMTAP_C
			if (rc)
				ctrt_printf("\tappThrdWait: Unable to read queue, error = %d\n", rc);
			else
				ctrt_printf("\tappThrdWait: Read queue entry: length=%d, contents=[%s]\n",
					qlen, qbuf);
#else  /* ~WTMTAP_C */
			if (rc)
				th_printf(0,"Unable to read queue, error = %d\n", rc);
			else
				th_printf(0,"Read queue entry: [%s]\n\n", qbuf);
#endif /* ~WTMTAP_C */
			--nthrd;
		}
	}
	else
	{
#ifdef WTMTAP_C
		ctrt_printf("\tappThrdWait: Using global variable and mutex for notification.\n");
#endif
		/* Use value of tfin to wait for all threads to finish. */
		/* Each thread increments tfin before it exits. */
		do
		{
				/* FairCom MT API: Please note #05 */
			ctThrdMutexGet(&tfin_mtx);
			rc = tfin;
			ctThrdMutexRel(&tfin_mtx);
			if (rc >= nthrd)
				break;
#ifdef WTMTAP_C
			ctrt_printf("\tappThrdWait: Number of threads finished: %d\n", rc);
#endif /* WTMTAP_C */
		} while (!ctThrdSleep((LONG) 100));
#ifdef WTMTAP_C
		ctrt_printf("\tappThrdWait: Number of threads finished: %d\n", rc);
#endif /* WTMTAP_C */
	}

	return (0);
} /* appThrdWait() */


/* end ctmtap.h */
