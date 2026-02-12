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

#ifndef _FCAUX_HPP_
#define _FCAUX_HPP_

#define ctOPT2H			/* Only include the basic defines in ctoptn.h , not ctopt2.h */
#include "ctoptn.h"
#undef ctOPT2H

#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include "ctplatform.h"
#include "ctjson.h"

#define FC_METADATA_USER_NAME "admin"
#define FC_METADATA_DB_NAME "faircom"
#define FC_METADATA_MAX_CONN 32
#define FC_METADATA_INACTIVE_TIMEOUT 32
#ifndef ctPortANDROID
#define FC_METADATA_AUTH_FILE_NAME "./web/localAuthFile.set"
#else
#define FC_METADATA_AUTH_FILE_NAME "localAuthFile.set"
#endif

#define FC_MAX_CONFIG_LEN 256
#define FC_MAX_ERRMSG_LEN 1024
#define FC_DATETIME_STRLEN 30
#define FC_MAX_PATH 4096
#define FC_CONN_POOL_SHRINK 5 /* Frequence of master connection pool cleanup check in seconds */
#define FC_BLOCK_SIZE 8192
#define FC_BUFFER_SIZE 1024
#define FC_MAX_IDENTNAME 64
#define LIST_BLOCK_SIZE 100    /* Block size for incremental list capacity expand */
#define FC_MAX_TOKEN_LEN 256
#define FC_READLOCK_TIMEOUT 3000

#define FCSize FCBigInt
#define FCDateTime time_t
#define FC_MAX_DATETIME ULONG_MAX

#define SEP '/'
#define NOT_SEP '\\'
#define SEPSTR "/"
#define NOT_SEPSTR "\\"
#define DOUBLE_SEP "//"

#define FC_CONN_TIMEOUT    "10" /* Number of seconds waiting for a connection to the server */
#define FC_CONN_NO_TIMEOUT "0"  /* No socket timeout */

 /* Integration Service Framework */
#define INTEGRATION_DIR    "integration"

#ifdef CT_THINGWORX
#define TWRUC_NAME "ctthingworx.always-onRUC"
#endif

typedef enum {
	THRD_WAIT = 0,   /* Wait for the thread initilization  */
	THRD_RUN = 1,    /* RUN state  */
	THRD_STOP = 2,   /* STOP state */
	THRD_STOPPED = 3, /* STOPPED state */
	THRD_ERROR = 4,  /* Thread error */
	THRD_EXEC = 5    /* Thread execute */
} ThrdState, *pThrdState;

typedef struct ThrdData {
	int errorCd;
	char msg[FC_MAX_ERRMSG_LEN + 1];
	ThrdState state;
	void *context;
	void *mutex;
	char *userword;
} ThrdData, *pThrdData, **ppThrdData;

typedef FC_FUNC_RET(FCAPI *FC_THRD_FUNCTION)(void *fcThrdParam);
typedef FC_THRD_FUNCTION pFC_THRD_FUNCTION;


/* String manipulation */
char *dupStr(const char *str);
char *dupStr(const char *buffer, size_t bufferLen);
void strToUpper( char *str );
void strToLower(char *str);
int fcstricmp(const char *str1, const char *str2);
int fcstrnicmp(const char *str1, const char *str2, int len);
int fcstrcmp(const char *str1, const char *str2);
int fcstrncmp(const char *str1, const char *str2, int len);
bool CheckWildcard(const char *fileName, const char *mask);
unsigned int HashUInt(const char *string);

void _revobj(char* tp, int len);

/* Mutex */
void *fcMutexCreate();
void fcMutexFree(void *mutex);
FCULONG fcMutexGet(void *mutex, int waitTime = -1);
void fcMutexRel(void *mutex);


/* Read/Write Lock */
typedef
#ifdef _WINDOWS
SRWLOCK
#else
pthread_rwlock_t
#endif
fcWRLock;

void fcRWLockCreate(void *lock);
void fcRWLockFree(void *lock);
void fcRWLockGetRead(void *lock);
void fcRWLockRelRead(void *lock);
void fcRWLockGetWrite(void *lock);
void fcRWLockRelWrite(void *lock);
void fcRWLockTryGetRead(void *lock, int timeout);


/* Thread */
void *fcThrdCreate(pFC_THRD_FUNCTION fcThrdFunction, void *fcThrdParam, bool detach = false);
void fcThrdFree(void *thrdHandle);
FCULONG fcThrdWait(void *thrdHandle);
void fcThrdWaitMulti(int thrdCount, void **thrdHandleList);
void fcSleep(int milliseconds);
FCThreadID fcGetCurrentThreadId();


/* File System */
typedef struct fcFileData {
	char name[FC_MAX_PATH + 1];
	bool isDir;
	FCSize size;
	FCDateTime createTime;
	FCDateTime updateTime;
} FCFileData, *pFCFileData, **ppFCFileData;

typedef struct fcFileFindHandle {
	void *findHandle;
	FCFileData fileData;
#ifndef _WINDOWS
	char *mask;
	char *path;
	char fileDirName[FC_MAX_PATH + 1];
#endif
} FCFileFindHandle, *pFCFileFindHandle, **ppFCFileFindHandle;

pFCFileFindHandle fcFindFileFirst(const char *dir);
void fcFindFileClose(pFCFileFindHandle fileFindHandle);
int fcFindFileNext(pFCFileFindHandle fileFindHandle);

pFCFileFindHandle fcFindDirFirst(const char *path);
void fcFindDirClose(pFCFileFindHandle dirFindHandle);
int fcFindDirNext(pFCFileFindHandle dirFindHandle);

void fcCheckDir(const char *path);
void fcDeleteFileDir(const char *path);
int fcMoveFile(const char *sourceFileName, const char *targetFileName);
void fcCopyFile(const char *sourceFileName, const char *targetFileName);
void fcValidatePath(char *path);
void fcValidateDrive(const char *volumeName);
bool fcCheckFile(const char *fileName);
int fcComparePath(const char *path1, const char *path2, int len);
int fcGetCurrentDir(char *pathBuf, int bufLen);
int fcGetCurrentBinName(char *execName, int bufLen);
void fcBuildFullPath(const char *rootPath, const char *relativePath, char *absolutePath);
void fcBuildFullPath(const char *relativePath, char *absolutePath);
int fcGetHostName(char **hostName, char **errorMsg);
bool isRelativePath(const char* path);

TIMER fcGetSysTimer();
void AddJson(pCTJson output, const char* path, pCTJson element, char* errorMsg);

#ifdef _WINDOWS
void GetWindowsErrorMsg(DWORD errcod, char* syserr, size_t bufsiz);
#endif

#define IsRefControlObject (!fc_is_same<T, int>::value && !fc_is_same<T, char>::value && !fc_is_same<T, void>::value)

#endif /* _FCAUX_HPP_ */