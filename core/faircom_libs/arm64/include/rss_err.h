

/*
 *	Copyright (C) Dharma Systems Inc.        1992 - 2024.
 *	Copyright (C) Dharma Systems (P) Ltd.    1992 - 2024.
 *
 *	This Module contains Proprietary Information of
 *	Dharma Systems Inc. and Dharma Systems (P) Ltd.
 *	and should be treated as Confidential.
 */

/*
 * $RCSfile: rss_err.h,v $ $Revision: 1.1.1.1 $Date: 2006/08/04 07:25:43 $
 * $State: Exp $
 */

/* RSS error codes */

#ifndef	rss_err_included
#define	rss_err_included

/* Tpl manager error codes */
#define	ETPL_FIELD_EXISTS	-10000
#define	ETPL_FIELD_NOT_FOUND	-10001
#define	ETPL_TUPLE_NOT_FOUND	-10002
#define	ETPL_END_OF_TUPLE	-10003
#define	ETPL_BAD_HINT		-10004
#define	ETPL_SEGID_MISMATCH	-10005
#define	ETPL_TBLID_MISMATCH	-10006
#define	ETPL_NOTENOUGH_OFP	-10007
#define	ETPL_SEGID_NULL		-10008
#define	ETPL_PREVTID_NOT_FOUND	-10009
#define ETPL_NOT_ENOUGH_SPACE	-10010
#define	ETPL_BPG_MISMATCH	-10011
#define	ETPL_SCAN_EOP    	-10012

/*DC306 (DS_19980603_873 bug fix) BEGIN  */

#define ETPL_NOT_FOUND		-10013

/*DC306 (DS_19980603_873 bug fix) END  */


/* Index manager error codes */
#define	EIX_INDEX_TOO_LONG	-10100
#define	EIX_SCAN_END		-10101
#define	EIX_INVAL_DUP		-10102
#define	EIX_INDEX_NOT_FOUND	-10103
#define	EIX_FLD_TOO_LONG	-10104
#define	EIX_INCON_CURSOR	-10105
#define EIX_INVAL_METHOD	-10106
#define EIX_SCAN_EOP	        -10107

/*DC306 (DS_19980603_873 bug fix) BEGIN  */

#define EIX_INV_DUP		-10108

/*DC306 (DS_19980603_873 bug fix) END  */

/* Ptpl manager error codes */
#define EPTPL_OVFLOW 		-10200
#define EPTPL_BAD_INDEX		EPTPL_OVFLOW
#define EPTPL_FIRST_PAGE_SMALL  -10201

/* Lock manager error codes */
#define ELCK_BAD_LOCK		-10300
#define ELCK_AGAIN		-10301
#define ELCK_NORESRC		-10302
#define ELCK_TIMEOUT		-10303
#define ELCK_DEADLOCK		-10304
#define ELCK_SVRNOWAIT		-10305
#define ELCK_BAD_REFCNT		-10306
#define ELCK_TRANSID_MISMATCH	-10307
#define ELCK_BAD_LCKHDL		-10308
#define ELCK_UPDATE_NOWAIT      -10309  /* DC_20120626_4208 */

/* Log manager error codes */
#define	ELOG_BAD_FSIZE		-10400
#define	ELOG_FILE_NOT_FOUND	-10401
#define	ELOG_OPEN		-10402
#define	ELOG_FILE_BUSY		-10403
#define	ELOG_BAD_ENTRY		-10404
#define	ELOG_NO_FILE		-10405
#define	ELOG_EOF		-10406
#define	ELOG_NOT_RECOVERED	-10407
#define ELOG_NO_SPACE		-10408
#define ELOG_OPEN_FAIL		-10409
#define	ELOG_READ_FAIL		-10410
#define ELOG_SIZE_NOTEQ		-10411
#define ELOG_NAME_LONG		-10412
#define ELOG_DUMMY_LOGE 	-10413
#define ELOG_WRONG_FILE		-10414
#define ELOG_ARCHIVE_FAILED	-10415

/* Log manager status codes */
#define	LOG_DO_CHECK_POINT	 10499

/* Cache manager error codes */

/* Segment manager error codes */
#define	ESEG_BADINIT		-10600
#define	ESEG_NOMORE_SEG		-10601
#define	ESEG_BAD_FILE		-10602
#define	ESEG_EOF		-10603
#define	ESEG_BAD_NO		-10604
#define	ESEG_NOMORE_FILES	-10605
#define	ESEG_BAD_HINT		-10606
#define	ESEG_PGALLOC_FAIL	-10607
#define	ESEG_BAD_FSIZE		-10608
#define	ESEG_NULL		-10609

/* Fileid manager error codes */
#define EFNO_FNO_TOOBIG		-10700
#define EFNO_OPEN		-10701

/* DSL error codes */
#define	EDSL_BAD_DSLNO		-10800
#define	EDSL_DEADLOCK		-10801

/* Events manager error codes */
#define	EEM_TIMEOUT		-10900

/* SHM error code */

/* VEC_LIST error code */
#define	EVL_DUP_IN_SORTLIST	-10920

/* Transaction manager error codes */
#define	ETM_INVAL_TRANSID	-11100
#define	ETM_NO_TABSPACE		-11101
#define	ETM_NO_TDS		-11102
#define	ETM_TDS_NOT_FOUND	-11103
#define	ETM_ABORTED_TRANS	-11104
#define	ETM_ACTIVE_TRANS	-11105
#define	ETM_INVAL_NULL_COORD	-11106
#define	ETM_BEGIN_DISABLED	-11107
#define	ETM_BAD_LVL		-11108
#define	ETM_BAD_HANDLE		-11109
#define	ETM_TRANS_LOCKED	-11110
#define	ETM_BAD_ISOLN_LVL	-11111
#define ETM_ISOLN_LVL_CANNOT_CHANGE     -11112L   /* DC737  */
#ifdef FAIRCOM_MOD
#define ETM_NULL_RDS_TRAN	-11113
#endif

/* Transaction manager status codes */
#define	TM_END_TRANS		11199

/* Snapshot manager error codes */
#define ESM_SNAPSHOT_OLD	-11200
#define ESM_INVALID_SLSN	-11201

/* UUID error codes */

/* DTM error codes */

/* Storage Manager Error Codes (rss_hdl_t) */

#define ERH_INFO_TYPE_NOT_SUPPORTED	-11300
#define ERH_INDEX_TYPE_NOT_SUPPORTED    -11301

 /* DC698 BEGIN */


#define JRSS_ERR_CLS_OR_MID_NOT_FOUND   -11302


 /* DC698 END */

/* To check if the transaction got aborted due to internal error */
#define	tpe_rss_abort(st)	(st &&\
				    (st == ELCK_TIMEOUT ||\
				     st == ELCK_DEADLOCK ||\
				     st == ELOG_NO_SPACE ||\
				     st == ELCK_SVRNOWAIT ||\
				     st == ELOG_ARCHIVE_FAILED))

#if defined (FAIRCOM_MOD) && defined (ctSQL_AUTO_ROLLBACK)
#define	fc_abort_tran(st)	(st &&  (st == -17086 || st == -17071 ))
#endif

/* This is global error ment to be used in RSS & RDS both */
#define RSS_ERR_QUERY_ABORTED 		-30001

#endif	/* rss_err_included */



