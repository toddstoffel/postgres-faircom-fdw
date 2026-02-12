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

#ifndef _CTCBACU_H_
#define _CTCBACU_H_

#define	Finput                  0		/* open modes */
#define Foutput                 1
#define Fio                     2
#define	Fextend                 3
#define	Fopen_mask              3
#define	Ftext                   4		/* vs. Fbinary, the default */
#define	Fbinary                 0		/* convenience */
#define	Flocal_only             8
#define	Fmulti_lock             0x0010
#define	Fmulti_records          0x0020		/* only used in i_open, r_open, s_open */
#define	Fcache_local            0x0020		/* only used in os_open */
#define	Ftruncate               0x0040
#define	Fread_lock              0x0100
#define	Fwrite_lock             0x0200
#define	Fbuffered               0x0400
#define	Fmass_update           (Fbuffered | Fwrite_lock)
#define	Flock_mask             (Fbuffered | Fwrite_lock | Fread_lock)
#define	Fis_device              0x0800		/* internal use only */
#define	Fopt_lock               0x1000		/* internal use only */
#define	Fappend                 0x2000		/* internal use only */
#define	Ftrans                  0x4000
#define	Fbulk_add              (0x8000 | Fbuffered | Fwrite_lock)
#define	FAccessSequential       0x010000
#define	FAccessRandom           0x020000
#define	FAccessDynamic         (FAccessSequential | FAccessRandom)
#define	Fassign_external        0x040000
#define	Flargefile              0x080000	/* internal use only */

#define	F_EQUALS                0		/* start modes */
#define	F_NOT_LESS              1
#define	F_GREATER               2
#define	F_LESS                  3
#define	F_NOT_GREATER           4

/* Errors (in f_errno) */

#define E_SYS_ERR               1
#define E_PARAM_ERR             2
#define E_TOO_MANY_FILES        3
#define	E_MODE_CLASH            4
#define E_REC_LOCKED            5
#define	E_BROKEN                6
#define	E_DUPLICATE             7
#define	E_NOT_FOUND             8
#define E_UNDEF_RECORD          9
#define E_DISK_FULL             10
#define	E_FILE_LOCKED           11
#define	E_REC_CHANGED           12
#define	E_MISMATCH              13
#define	E_NO_MEMORY             14
#define	E_MISSING_FILE          15
#define	E_PERMISSION            16
#define	E_NO_SUPPORT            17
#define	E_NO_LOCKS              18
#define	E_INTERFACE             19
#define	E_LICENSE_ERR           20
#define	E_UNKNOWN_ERR           21
#define	E_TRANSACTION           22
#define	E_NOT_ME                99
#define	W_NO_SUPPORT            100
#define	W_DUP_OK                101

/* Transaction Errors (in f_log_errno) */

#define E_LOG_EXTERNAL		1
#define E_LOG_TOO_MANY		2
#define E_LOG_MISSING		3
#define E_LOG_PERMISSION	4
#define E_LOG_SYS_ERR		5
#define E_LOG_CORRUPT		6
#define E_LOG_LOCKED		7
#define E_LOG_NO_MEMORY		8
#define E_LOG_DISK_FULL		9
#define	E_NO_LOG		10
#define E_RB_LOG_CORRUPT	11
#define	E_LOG_INCOMPLETE	12
#define	E_OPEN_NOT_LOGGED	13
#define E_LOG_INTERFACE		14
#define	E_LOG_REMOTE		15
#define E_LOG_NESTED_START	16
#define E_LOG_TEMP		17
#define W_LOG_NO_SUPPORT	99

#ifdef ctCobolIS
#define ctAstdlib_f_errmsg              Astdlib_f_errmsg
#define ctAstdlib_f_errno               Astdlib_f_errno
#define ctAstdlib_f_int_errno           Astdlib_f_int_errno
#define ctAstdlib_f_int2_errno          Astdlib_f_int2_errno
#define ctAstdlib_f_no_lock             Astdlib_f_no_lock
#define ctAvision_i_implied_transaction Avision_i_implied_transaction
#define ctAvision_i_in_commit           Avision_i_in_commit
#define ctAvision_i_in_transaction      Avision_i_in_transaction
#endif

#ifndef	f_errmsg
extern	cbDLLexport char		**ctAstdlib_f_errmsg(void);
#define	f_errmsg	(*ctAstdlib_f_errmsg())
typedef char		**(*p_Astdlib_f_errmsg)(void);
#endif	/* f_errmsg */

#ifndef	f_errno
extern	cbDLLexport short		*ctAstdlib_f_errno(void);
#define	f_errno		(*ctAstdlib_f_errno())
typedef	short		*(*p_Astdlib_f_errno)(void);
#endif	/* f_errno */

#ifndef	f_int_errno
extern	cbDLLexport long		*ctAstdlib_f_int_errno(void);
#define	f_int_errno	(*ctAstdlib_f_int_errno())
typedef long		*(*p_Astdlib_f_int_errno)(void);
#endif	/* f_int_errno */

#ifndef	f_int2_errno
extern	cbDLLexport long		*ctAstdlib_f_int2_errno(void);
#define	f_int2_errno	(*ctAstdlib_f_int2_errno())
typedef long			*(*p_Astdlib_f_int2_errno)(void);
#endif	/* f_int2_errno */

#ifndef	f_no_lock
extern	cbDLLexport short		*ctAstdlib_f_no_lock(void);
#define	f_no_lock	(*ctAstdlib_f_no_lock())
typedef short		*(*p_Astdlib_f_no_lock)(void);
#endif	/* f_no_lock */

#ifndef	i_implied_transaction
extern	cbDLLexport short			*ctAvision_i_implied_transaction(void);
#define	i_implied_transaction	(*ctAvision_i_implied_transaction())
typedef short		*(*p_Avision_i_implied_transaction)(void);
#endif	/* i_implied_transaction */

#ifndef	i_in_commit
extern	cbDLLexport short			*ctAvision_i_in_commit(void);
#define	i_in_commit		(*ctAvision_i_in_commit())
typedef short		*(*p_Avision_i_in_commit)(void);
#endif	/* i_in_commit */

#ifndef	i_in_transaction
extern	cbDLLexport short			*ctAvision_i_in_transaction(void);
#define	i_in_transaction	(*ctAvision_i_in_transaction())
typedef short		*(*p_Avision_i_in_transaction)(void);
#endif	/* i_in_transaction */

typedef struct {
	/*  Indexed file functions  */
    int      (*p_init)(int, int *);
    void     (*p_exit) (void);
    int      (*p_make) (char *, char *, char *, char *, char *, char *);
    void     *(*p_open) (char *, int, char *);
    int      (*p_close) (void *);
    unsigned (*p_read) (void *, char *, int);
    unsigned (*p_next) (void *, char *);
    unsigned (*p_previous) (void *, char *);
    int      (*p_start) (void *, char *, int, int, int);
    int      (*p_write) (void *, char *, unsigned);
    int      (*p_rewrite) (void *, char *, unsigned);
    int      (*p_delete) (void *, char *);
    int      (*p_unlock) (void *);
    int      (*p_info) (void *, int, char *);
    void     (*p_sync) (int);
    int      (*p_remove) (char *);
    int      (*p_rename) (char *, char *);
    int      (*p_version) (char *, char *, char *, int, long*);
    void     (*p_abilities) (void *);
    int      (*p_execute) (char *, char *);
    int      (*p_copy) (char *, char *);
    int      (*p_begin) (void);
    int      (*p_commit) (int);
    int      (*p_rollback) (int);
    int      (*p_recover) (void);

	/*  Setup functions  */
    void     *(*p_register) (char *, void *);
    void     (*p_init_cfg) (void);

	/*  Relative file functions  */
    int      (*pr_init) (void);
    void     (*pr_exit) (void);
    int      (*pr_make) (char *, char *, long);
    void     *(*pr_open)  (char *, int, unsigned, unsigned, long);
    int      (*pr_unlock) (void *);
    int      (*pr_close) (void *);
    unsigned (*pr_read) (void *, char *, long);
    unsigned (*pr_next) (void *, char *);
    unsigned (*pr_previous) (void *, char *);
    int      (*pr_start) (void *, long, int);
    int      (*pr_write)  (void *, char *, unsigned, long);
    int      (*pr_delete) (void *, long);
    int      (*pr_rewrite) (void *, char *, unsigned, long);
    int      (*pr_remove) (char *);
    int      (*pr_rename) (char *, char *);
    int      (*pr_copy) (char *, char *);
    int      (*pr_info) (void *, int, char *);

	/*  Sequential file functions  */
    int      (*ps_exit) (void *, int);
    int      (*ps_make) (char *, char *);
    void     *(*ps_open) (char *, int, unsigned, int, unsigned, int, char *, int);
    int      (*ps_close) (void *, int);
    int      (*ps_read) (void *, char *);
    int      (*ps_write) (void *, char *, unsigned, int);
    int      (*ps_rewrite) (void *, char *, unsigned);
    int      (*ps_remove) (char *);
    int      (*ps_rename) (char *, char *);
    int      (*ps_copy) (char *, char *);
    void     *(*ps_seek) (void *, void *, int);
    int      (*ps_info) (void *, int, char *);
} I_DISPATCH;

typedef	struct _i_abilities {
	unsigned	has_previous : 1;
	unsigned	alternate_collate : 1;
} I_ABILITIES;

#endif /* ~_CTCBACU_H_ */

/* end of ctcbacu.h */
