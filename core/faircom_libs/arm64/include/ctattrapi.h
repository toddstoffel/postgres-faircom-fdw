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

#ifndef _CTATTRAPI_H_
#define _CTATTRAPI_H_

/* exposed types */
/* handles*/
typedef pVOID CTATTR_HANDLE; /* attributes handle*/
typedef pVOID CTATTR_ITERATOR; /* attributes iterator */


typedef enum {
	CTATTR_ATTR_TYPE_INVALID,
	CTATTR_ATTR_TYPE_STRING, /* pTEXT expected*/
	CTATTR_ATTR_TYPE_BINARY, /* pTEXT expected encoded in base64 in JSON*/
	CTATTR_ATTR_TYPE_INT32, /* pLONG expected */
	CTATTR_ATTR_TYPE_INT64, /* pLONG8 expected */
	CTATTR_ATTR_TYPE_BOOLEAN, /* pNINT expected */
	CTATTR_ATTR_TYPE_DOUBLE, /* double * expected */
	CTATTR_ATTR_TYPE_OBJECT /* attribute value is an object, handled with dedicated functions */
} CTATTR_ATTRIBUTE_TYPE;

/* defined attributes. It is not important that similar attributes stay grouped together.*/
/* It is important to have no holes in the numeric sequence of values since these are used */
/* as array index. */
typedef enum
{
	CTATTR_INVALID = 0,
	CTATTR_STRUCT_SCHEMAS, /** internal structure element, not for explicit use */
	CTATTR_STRUCT_FIELDS, /** internal structure element, not for explicit use */
	CTATTR_STRUCT_INDEXES, /** internal structure element, not for explicit use */
	CTATTR_STRUCT_SEGMENTS, /** internal structure element, not for explicit use */
	CTATTR_STRUCT_MRTCONDS, /** internal structure element, not for explicit use */
	CTATTR_STRUCT_FLDCONDS, /** internal structure element, not for explicit use */
	CTATTR_STRUCT_RULES, /** internal structure element, not for explicit use */
	CTATTR_STRUCT_RULE_FILTER, /** internal structure element, not for explicit use */
	CTATTR_STRUCT_RULE_FILTERTEXT, /** internal structure element, not for explicit use */

	CTATTR_FIELD_FIELD_NAME, /** field name (ppTEXT)*/
	CTATTR_FIELD_STR_ENCODING, /** string encoding name per ICU libray */

	CTATTR_TABLE_MINRECLEN, /** minimum record length (as used in RTG) (pLONG)*/
	CTATTR_TABLE_MAXRECLEN, /** maximum recorf length (as used in RTG) (pLONG)*/
	CTATTR_TABLE_RTGNUMCONV, /** numeric convention for COBOL compatible data types (ppTEXT - parsable)*/
	CTATTR_TABLE_RECHDRSIZ, /** record header size CTDB/RTG (pLONG)*/
	CTATTR_TABLE_SRLSEGSIZE, /** defined size of SRLSEG (pLONG) */
	CTATTR_TABLE_ROWIDIDX, /** table has a rowid index (i.e. unique index with 1 and only 1 segment with type SRLSEG) (pNINT) */

	CTATTR_TABLE_RTGBLOCKFACT, /** RTG block factor (pLONG)*/
	CTATTR_TABLE_RTGPREALLOC, /** RTG prealloc (pLONG)*/
	CTATTR_TABLE_RTGEXTENSION, /** RTG table extension (pLONG)*/
	CTATTR_TABLE_RTGCOMPRESSION, /** RTG table compression as defined by the runtime, may not match the actual physical file (pNINT) */
	CTATTR_TABLE_RTGCOMMENT, /** RTG table comment (ppTEXT)*/

	CTATTR_TABLE_RTGSTRUCTVER, /** RTG required structver to properly handle this file (pLONG)*/
	CTATTR_TABLE_RTGDUPSCANWA, /** RTG dupscanwa (pNINT)*/

	CTATTR_INDEX_RTGDUP, /** RTG duplicate index mode (pLONG)*/
	CTATTR_INDEX_NULL_KEY_VALUE, /** NULL key value for index (pLONG)*/
	CTATTR_INDEX_NULL_KEY_LEN, /** NULL key value length for index (pLONG) */
	CTATTR_INDEX_KEYNUMBER, /** key number (pLONG) */
	CTATTR_INDEX_APIINTERNAL, /** internal index nature (ppTEXT - parsable)*/
	CTATTR_INDEX_PRIMARY, /** internal index nature (ppTEXT - parsable)*/

	CTATTR_SEGMENT_RTGTYPE, /** RTG segment type (ppTEXT - parsable) */
	CTATTR_SEGMENT_RTGMODE, /** RTG segment mode (pLONG) */  /* this is not "parsable" as it may be the "OR" of multiple values*/
	CTATTR_SEGMENT_NULLVALUE, /** RTG null segment value (pTEXT) */


	CTATTR_SCHEMA_RTGTYPEMAPPING, /** RTG type mapping version (pLONG)*/
	CTATTR_SCHEMA_RTGONERROR, /** RTG global schema conversion error handling definition (ppTEXT - parsable)*/
	CTATTR_SCHEMA_MRTCONDITION, /** for MRT the condition rule number for this schema (pLONG)*/
	CTATTR_SCHEMA_PADCHAR, /** fixed string padding character CTATTR_RESOURCE_VER_MULTISCHEMA (pLONG) */
	CTATTR_SCHEMA_DLMCHAR, /** fixed string terminator character CTATTR_RESOURCE_VER_MULTISCHEMA (pLONG) */
	CTATTR_SCHEMA_DLMPOS, /** fixed string terminator character position (ppTEXT - parsable) */

	CTATTR_FIELD_SIZE, /** field size in bytes (pLONG)*/
	CTATTR_FIELD_OFFSET, /** field offset (pLONG)*/
	CTATTR_FIELD_PRECISION, /** field precision (digits for RTG type) CTATTR_RESOURCE_VER_MULTISCHEMA (pLONG)*/
	CTATTR_FIELD_SCALE, /** field scale CTATTR_RESOURCE_VER_MULTISCHEMA (pLONG)*/
	CTATTR_FIELD_TYPE, /** field type (pTEXT - parsable)*/
	CTATTR_FIELD_SQLTYPE, /** SQL mapping type for field (pTEXT - parsable)*/
	CTATTR_FIELD_RTGFORMAT, /** field RTG type format (pTEXT)*/
	CTATTR_FIELD_JULIANBASE, /** base date for julian dates (YYYYMMDD) or "integer-of-date" as used in some COBOLS (pTEXT)*/
	CTATTR_FIELD_HIDDEN, /** field is hidden from SQL (pNINT)*/

	CTATTR_FIELD_DEFVALUETYPE, /** default field value type (pTEXT - parsable) */
	CTATTR_FIELD_DEFVALUELEN, /**default field value len once expanded with last byte repetition */
	CTATTR_FIELD_DEFVALUE, /** default field value Base64 encoded with last byte repetition padding compression (Binary) */
	CTATTR_FIELD_RTGONERROR, /** RTG on conversion error (pTEXT - parsable) */
	CTATTR_FIELD_RTGONSIGNERR, /** RTG on conversion error due to sign (pTEXT - parsable) */
	CTATTR_FIELD_RTGFALSEBINLEN, /** RTG default false value length (pLONG) */
	CTATTR_FIELD_RTGFALSEBINVAL, /** RTG default false value (Binary) */
	CTATTR_FIELD_RTGTRUEBINLEN, /** RTG default true value length (pLONG) */
	CTATTR_FIELD_RTGTRUEBINVAL, /** RTG default true value (Binary) */
	CTATTR_FIELD_RTGNULLINDICATOR, /** RTG - NULL indicator field name that acts as null indicatorsize (PSQL add 1 byte null indicator field before a nullable field) */
	CTATTR_FIELD_RTGLONGSIZEFLD, /** RTG LONG FIELD - field containing the long field size (pTEXT) */
	CTATTR_FIELD_RTGLONGITEMSIZE, /** RTG LONG FIELD - size of the LONG FIELD base field (pLONG)*/
	CTATTR_FIELD_RTGLONGMAXSIZE, /** RTG LONG FIELD - maximum size of the LONFGFIELD (pLONG) */
	CTATTR_FIELD_VFLDCONDITION, /** the condition rule number for this virtual field (pLONG)*/
	CTATTR_FIELD_NOTNULL, /** the field has been defined "not null" CTATTR_RESOURCE_VER_MULTISCHEMA (pTEXT - parsable)*/
	CTATTR_FIELD_BINFLAG, /** the "binaryflag" CTATTR_RESOURCE_VER_MULTISCHEMA (pTEXT - parsable)*/
	CTATTR_FIELD_RTGBITNUM, /** for bit field (BT_BIT) the target bit number (pLONG)*/
	CTATTR_FIELD_RTGBTLONGHANDLE, /** RTG LONG FIELD - field containing the BT_?LOB header (pTEXT) */

	CTATTR_RULE_TABLENAME, /** MRT table name */
	CTATTR_RULE_KEYNUM, /** MRT key number (1 is the first key) to aid with filter (pLONG) */
	CTATTR_RULE_KEYVALMINLEN,/** MRT key lower bound size (pLONG)*/
	CTATTR_RULE_KEYVALMIN, /** MRT key lower bound value to aid with filter (Binary)*/
	CTATTR_RULE_KEYVALMAXLEN, /** MRT key upper bound size (pLONG)*/
	CTATTR_RULE_KEYVALMAX, /** MRT key upper bound value to aid with filter (Binary)*/
	CTATTR_RULE_KEYSEARCHMODE, /** MRT key search mode to aid with filter (pTEXT - parsable)*/

	/* from here we keep entries them grouped here and in this sequence to match old XDD sequence and make it easy to convert */
	CTATTR_RULE_FILTER_EQ,
	CTATTR_RULE_FILTER_NEQ,
	CTATTR_RULE_FILTER_GT,
	CTATTR_RULE_FILTER_GTE,
	CTATTR_RULE_FILTER_LT,
	CTATTR_RULE_FILTER_LTE,
	CTATTR_RULE_FILTER_AND,
	CTATTR_RULE_FILTER_OR,
	CTATTR_RULE_FILTER_NOT,
	CTATTR_RULE_FILTER_FIELD,
	CTATTR_RULE_FILTER_VALUE,
	CTATTR_RULE_FILTER_OTHER,
	CTATTR_RULE_FILTER_ALL,
	/* end of grouped entries */

	/* !!! when adding here, remember to :                 */
	/*         update CTATTR_KEY keys[] in ctattrapi.c     */
	/*         FOR FIELDS attributes                       */
	/*             (ctattrapi.?) consider if the CTATTR_FLDINFO structure and _ctAttrFieldGetAttributes function need modifications */
	/*             CTDB: (ctdbjson.c) consider if the _ctdbPutFieldXtdInfo function needs modifications */
	/*             RTG: (ctcbxdd.c) consider if the cts_AttrCopyField2XDD, cts_XDDCopyField2Attr functions need modifications */
	/*         FOR INDEX attributes                       */
	/*             CTDB: (ctdbjson.c) consider if the _ctdbSyncIndexAttributes() function needs modifications */
	/*             RTG: (ctcbcore.c) consider if the cts_info2Attr, cts_Attr2info functions need modifications */
	/*             RTG: (ctdbxdd.c) consider if the  _XDD2Attr, _Attr2XDD  functions need modifications */
	/*             RTG: (ctdbxfd.c) consider if the  GenCTMAKEfromXDD functions need modifications */
	/*         FOR SEGMENT attributes                       */
	/*             CTDB: (ctdbjson.c) consider if the _ctdbSetIndexAttributes() function needs modifications */
	/*             RTG: (ctcbcore.c) consider if the cts_info2Attr, cts_Attr2info */
	/*             RTG: (ctdbxdd.c) consider if the  _XDD2Attr, _Attr2XDD  functions need modifications */
	/*             RTG: (ctdbxfd.c) consider if the  GenCTMAKEfromXDD functions need modifications */

	CTATTR_INVALID_LAST /* this must be the last one */
} CTATTR_ATTRIBUTE;


/* some atributes have a string mnemonic value that needs to be validated and interpreted */
/* here is the struct used to interpret them */
typedef struct
{
	char*	str;		/* mode name */
	NINT    len;		/* comparison len: 0 entire buffer, x only x chars */
	NINT	mode;		/* mode value */
} CTATTR_MNEMONIC, *pCTATTR_MNEMONIC;

typedef enum {
	A_SIGN_CONV = 0,
	I_SIGN_CONV = 1,
	M_SIGN_CONV = 2,
	N_SIGN_CONV = 3, /* ACU's -Dcn. This option is compatible with NCR COBOL. */
	V_SIGN_CONV = 4, /* ACU's -Dcv. This option is compatible with VAX COBOL. */
	B_SIGN_CONV = 5, /* ACU's -Dcb. MBP COBOL sign storage convention. */
	R_SIGN_CONV = 6,  /* ACU's -Dcr. Realia sign storage convention. */
	NO_SIGN_CONV = 15  /* sign conv not specified (used only when forcing a sign convention)*/
	/* NOTE: this value get casted to TEXT and the mask below applied */
} CTATTR_RTGNUMCONV;

#define SIGN_CONV_MASK (UTEXT)0xF0
#define SIGN_CONV_ERROR (UTEXT)0x00
#define SIGN_CONV_FORCEP (UTEXT)0x10
#define SIGN_CONV_FORCEN (UTEXT)0x20

typedef enum {
	onConvertErrorUnDef = 0,
	onConvertErrorNULL = 1,
	onConvertErrorChkDef = 2,
	onConvertErrorStrict = 3,
	onConvertErrorValue = 4
} CTATTR_CONVERSION_ERROR;

typedef enum {
	dlm_buffer_end = 0,  /* delimiter is last byte or the field buffer in the record */
	dlm_string_end = 1  /* delimiter is the first byte after the string in the field buffer then the buffer is padded till the end with no further delimiter */
} CTATTR_DLMPOS_VALUES;
/* this controls the fixed length string padding and delimiter behavior. Assuming 0x00 is the delimiter and 0x20 is the padding
* on a 5 bytes string set to "A" the padding to the end of teh buffer will be
* dlm_buffer_end: (default)   0x61 20 20 20 00
* dlm_string_end:             0x61 00 20 20 20
*/

typedef enum {
	CTDEF_NODFLT = 0,
	CTDEF_LITERAL = 1,
	CTDEF_USER = 2,
	CTDEF_NULL = 3,
	CTDEF_SYSDATE = 4,
	CTDEF_SYSTIME = 5,
	CTDEF_SYSTIMESTAMP = 6,
	CTDEF_UID = 7,
	CTDEF_BINARY = 8
} CTATTR_DEFAULT_TYPE;

typedef enum
{
	CTDB_BINARY_UNKNOWN = 0,		/* table created by c-treeDB prior to v9  */
									/* it indicates that CT_ARRAY or CT_2STRING*/
									/* field may or may not have extra 4 bytes*/
									/* at the beginning of data				  */
	CTDB_BINARY_FIXED = 1,		/* table created by c-treeDB with v9 and  */
								/* after, CT_ARRAY and CT_2STRING fields  */
								/* are handled correctly				  */
	CTDB_BINARY_WITH_LENGTH = 2,	/* table created prior to v9, but table   */
									/* was operated only with c-treeACE SQL and   */
									/* CT_ARRAY and CT_2STRING fields have    */
									/* four bytes at the beginning of the data*/
									/* resource was updated by utility        */
	CTDB_BINARY_WITHOUT_LENGTH = 3,	/* table created by c-treeDB prior to v9  */
									/* but utility has updated this resource  */
									/* following types are handled correctly (as CTDB_BINARY_FIXED) */
									/* but used to indicate SQL types sharing CT_2STRING/CT_4STRING c-tree type other than varbinary/lvarbinary */

	CTDB_BINARY_VARCHAR = 4,  /* VARCHAR/LVARCHAR field created by c-treeSQL in v10 */
	CTDB_BINARY_CLOB = 5, /* CLOB field created by c-treeSQL in v10*/
	CTDB_BINARY_BLOB = 6, /* BLOB field created by c-treeSQL in v10*/
	CTDB_NUMBER_MONEY = 7, /* NUMBER field used to store MONEY type (instead of using  CT_CURRENCY) */
	CTDB_FIELDFLAG_USER1 = 8,  /* field used to store USER type */
	CTDB_FIELDFLAG_USER2 = 9,  /* field used to store USER type */
	CTDB_FIELDFLAG_USER3 = 10, /* field used to store USER type */
	CTDB_FIELDFLAG_USER4 = 11, /* field used to store USER type */
	CTDB_FIELDFLAG_USER5 = 12, /* field used to store USER type */
	CTDB_FIELDFLAG_USER6 = 13, /* field used to store USER type */
	CTDB_FIELDFLAG_USER7 = 14, /* field used to store USER type */
	CTDB_FIELDFLAG_USER8 = 15, /* field used to store USER type */
	CTDB_FIELDFLAG_USER9 = 16, /* field used to store USER type */
	CTDB_FIELDFLAG_USER10 = 17, /* field used to store USER type */
	CTDB_BINARY_MAX /* Largest Enum value */
} CTDB_BINARY_FLAG;

#define COBOL_INTOFDATE_BASE 0xFFFFFFFF  /* special CTDATE value to be used as JULIAN DATE for dates based on cobol's integer-of-date values */

enum {
	db_null = 0,
	db_date = 1,
	db_datetime = 2,
	db_time = 3,
	db_binary = 4,
	db_boolean = 5,
	db_clob = 6,
	db_blob = 7
};

typedef enum {
	tag_eq = 0,
	tag_neq = 1,
	tag_gt = 2,
	tag_gte = 3,
	tag_lt = 4,
	tag_lte = 5,
	tag_and = 6,
	tag_or = 7,
	tag_not = 8,
	tag_field = 9,
	tag_value = 10,
	tag_other = 11,
	tag_all = 12,
	tag_invalid = 13
} FILTERTAG;

enum {
	str_invalid = 0,
	str_ascii,
	str_utf8,
	str_utf16,
	str_utf16BE,
	str_utf16LE
};

typedef enum
{
	MRT_IDX_INVALID = -1,
	MRT_IDX_SET = 0,
	MRT_IDX_RNG = 1
} MRT_IDX_METHOD;


typedef struct
{
	pTEXT name; /* points to JSON memory */
	pTEXT str_encoding; /* string encoding per ICU library */  /* points to JSON memory */
	pTEXT	typeformat; /* type format - for now used only with date-time*/ /* points to JSON memory */
	pTEXT	errorSQLValue;	/* stringZ reppresenting the SQL value in case of error. */ /* points to JSON memory */
	pTEXT	size_field;	/* stringZ reppresenting the name of the field containing the size. */ /* points to JSON memory */
	pTEXT	nullindicator;  /* field holding NULL status */ /* points to JSON memory */
	pTEXT	binfalse;
	pTEXT	bintrue;
	/*pTEXT	condition; not yet in use for future */
	pTEXT	defvalue;		/* field default value stored as string		*/
	pTEXT	lobhandle;	/* stringZ reppresenting the name of the field containing the BTRV ?LOB header. */ /* points to JSON memory */
	VRLEN	offset;
	VRLEN	binfalselen;
	VRLEN	bintruelen;
	VRLEN	itemsize; /* occurs depending on, occurring item size */
	VRLEN	maxsize; /* occurs depending on, max size in bytes*/
	VRLEN	deflength;		/* length of defvalue						*/
	LONG	DBtype; /* forced "DB" type (in SQL) */
	ULONG	jbase; /*CTDATE in ctdb*/
	NINT	nonull; /* field null flag: YES nul not allowed		*/
	NINT	type;
	UCOUNT	precision;
	UCOUNT	scale;
	NINT	on_error;
	CTDB_BINARY_FLAG binflag; /* field binary flag						*/
	CTATTR_DEFAULT_TYPE defflag; /* indicate if default value is set and how */
	UCOUNT size;
	TEXT	hidden;
	UTEXT	on_sign;
	TEXT    bitnum; /*0-7 bit number for BT_BIT field (0 less significative) */
	/* next item used only during XDD parsing */
	COUNT aliasfor; /* in preparing "schema0" from conditions field: -1 not overlapping. -2 badly overlapping no way out, >=0 overlap with field # in field array */
} CTATTR_FLDINFO;

typedef struct
{
	pConvMap schema; /*followed by names in the same memory buffer */
	CTATTR_FLDINFO *fields;
	VRLEN entries; /* entries in fields */
	VRLEN rechdr;
	CTATTR_RTGNUMCONV numconv;
	COUNT refcount;
	TEXT alloc;
	TEXT sort;
} CTATTR_FIELDS_ARRAY;
#define FLD_ARR_GETNAMES(fldarr) ((fldarr->schema)?(((pTEXT)fldarr->schema)+ fldarr->schema->maplen):NULL)


/* API internal index types */
#define CT_APIIDX_NO 0
#define CT_APIIDX_CTDBROWID 1
#define CT_APIIDX_CTDBRECBYT 2
#define CT_APIIDX_RTGSRLIDX 3

/* RTG segment types*/
/* key segment modes */
#define CT_KMOD_NOTRANSFORM     0x00
#define CT_KMOD_NOCASE          0x01
#define CT_KMOD_EXTTYPE         0x08
#define CT_KMOD_DESC            0x10
#define CT_KMOD_ACS             0x20
#define CT_KMOD_SCHSEG          0x80 /* segment mode based on c-tree SCHSEG, for now this is used only during MAKE */

/* key segment types*/
#define CT_KTYP_NOT_SUPPORTED   199
#define CT_KTYP_STRING          0
#define CT_KTYP_INTEGER         1
#define CT_KTYP_FLOAT           2
#define CT_KTYP_DATE            3
#define CT_KTYP_TIME            4
#define CT_KTYP_DECIMAL         5  /* or COMP3*/
#define CT_KTYP_MONEY           6
#define CT_KTYP_LOGICAL         7
#define CT_KTYP_NUMERIC         8  /* A sign convention */
#define CT_KTYP_BFLOAT          9
#define CT_KTYP_LSTRING         10
#define CT_KTYP_ZSTRING         11
#define CT_KTYP_UNSIGNED_BINARY 14
#define CT_KTYP_AUTOINC         15
#define CT_KTYP_NUMERICSTS      17
#define CT_KTYP_NUMERICSA       18
#define CT_KTYP_CURRENCY        19
#define CT_KTYP_TIMESTAMP       20
#define CT_KTYP_WSTRING         25
#define CT_KTYP_WZSTRING        26
#define CT_KTYP_GUID            27
#define CT_KTYP_NUMERICSLB      28
#define CT_KTYP_NUMERICSLS      29
#define CT_KTYP_NUMERICSTB      31
#define CT_KTYP_NULL_INDICATOR  32
#define CT_KTYP_DATETIME        33
#define CT_KTYP_NUMERICST_GEN   34 /* sign (trailing) with convention determined by file's convention*/
#define CT_KTYP_NUMERICSTS_GEN  35 /* sign trailing separate with convention determined by file's convention*/
#define CT_KTYP_NUMERICSLS_GEN  36 /* sign leading separate with convention determined by file's convention*/
#define CT_KTYP_NUMERICSL_GEN   37 /* sign leading with convention determined by file's convention*/
#define CT_KTYP_SCOMP2          38 /* signed COMP-2 */
#define CT_KTYP_SIGNED_BINARY   39 /* signed binary (integer in bigendian format) */
#define CT_KTYP_JUSTAN          40 /* justified alphanum */



/* end of RTG segment types*/


/* resource version */
#define CTATTR_RESOURCE_VER_ORIG 1
#define CTATTR_RESOURCE_VER_MULTISCHEMA 2
#define CTATTR_RESOURCE_VER CTATTR_RESOURCE_VER_MULTISCHEMA /* table attributes resource version*/

/* ctAttrSchemaGetDODA modes*/
#define CTATTR_SCHEMAGET_DODA  0x0000 /* regular doda */
#define CTATTR_SCHEMAGET_DODAHOST 0x0001 /* main doda (used for filters) for MRT host */
#define CTATTR_SCHEMAGET_VDODA 0x0002 /* doda for virtual fields */
#define CTATTR_SCHEMAGET_MASK 0x00ff /*mask for actual modes. we have 255 modes and for now 1 modifier */

/* ctAttrSchemaGetDODA modifiers */
#define CTATTR_SCHEMAGET_REFNAMES 0x0100 /* field names are reference into the attribute handle */
#define CTATTR_SCHEMAGET_SCHNAMES 0x0200 /* field names are stored after schema in memory allocated to the schema, doda references them */
#define CTATTR_SCHEMAGET_FLDNBR   0x0400 /* doda_size is the number of fields*/


/* internal CTDB fields*/
#define CTATTR_ROWID_FIELD "$ROWID$"
#define CTATTR_DEL_FIELD "$DELFLD$"
#define CTATTR_NUL_FIELD "$NULFLD$"

/*alloc and free memory prototypes */
typedef pVOID(* ctAttrAllocFunc)(VRLEN size);
typedef VOID (* ctAttrFreeFunc)(pVOID ptr);

/* exposed API*/
/**
 * return whether the Attribute applies to a Table
 *
 * @param attr the attribute to check
 */
TEXT ctAttrIsTableAttribute(CTATTR_ATTRIBUTE attr);

/**
 * return whether the Attribute applies to a Schema
 *
 * @param attr the attribute to check
 */
TEXT ctAttrIsSchemaAttribute(CTATTR_ATTRIBUTE attr);

/**
 * return whether the Attribute applies to a Field
 *
 * @param attr the attribute to check
 */
TEXT ctAttrIsFieldAttribute(CTATTR_ATTRIBUTE attr);

/**
 * return whether the Attribute applies to an Index
 *
 * @param attr the attribute to check
 */
TEXT ctAttrIsIndexAttribute(CTATTR_ATTRIBUTE attr);

/**
 * return whether the Attribute applies to a Segment
 *
 * @param attr the attribute to check
 */
TEXT ctAttrIsSegmentAttribute(CTATTR_ATTRIBUTE attr);

/**
 * return whether the Attribute applies to a Condition
 *
 * @param attr the attribute to check
 */
TEXT ctAttrIsConditionAttribute(CTATTR_ATTRIBUTE attr);

/**
 * return whether the Attribute applies to a Rule
 *
 * @param attr the attribute to check
 */
TEXT ctAttrIsRuleAttribute(CTATTR_ATTRIBUTE attr);

/* handles */
/**
 * Alloc attribute handle for Table
 *
 */
CTATTR_HANDLE ctAttrTableAllocHandle(VOID);

/**
 * Free the Table attribute handle
 *
 * @param TblHdl the table handle to free
 *
 * @return error code
 */
COUNT ctAttrTableFreeHandle(CTATTR_HANDLE TblHdl);


/**
 * Copy the Table attribute handle, the copy is a full copy with no memory reference to the source
 *
 * @param dest the dstination table handle to fill with information copied from "src"
 *
 * @param src the source table handle copy information from "src"
 *
 * @return error code
 */
COUNT ctAttrTableCopyHandle(CTATTR_HANDLE dest, CTATTR_HANDLE src);

/**
 * Reset the Table attribute handle to the initial state (as it is after allocation)
 *
 * @param TblHdl the Table handle to reset
 *
 * @return error code
 */
COUNT ctAttrTableResetHandle(CTATTR_HANDLE TblHdl);

/**
 * Associate a Schema handle to the Table handle. It adds the schema definition to
 * the table definition.
 *
 * @param TblHdl the Table handle
 * @param SchHdl the Schema handle
 *
 * @return error code
 */
COUNT ctAttrTableAddSchema(CTATTR_HANDLE TblHdl, CTATTR_HANDLE SchHdl);


#ifdef NEEDS_REVIEW
/**
* Associate a Schema handle to the Table handle. It adds the schema definition to
* the table definition at the specified position.
*
* @param TblHdl the Table handle
* @param pos the position
* @param FldHdl the Schema handle
*
* @return error code
*/
COUNT ctAttrTableInsertSchema(CTATTR_HANDLE TblHdl, ULONG pos, CTATTR_HANDLE SchHdl);
#endif

/**
 * Delete the Nth schemafrom the table definition.
 *
 * @param TblHdl the Table handle
 *
 * @param item the schema (0 based) to remove
 *
 * @return error code
 */
COUNT ctAttrTableDeleteNthSchema(CTATTR_HANDLE TblHdl, UNLONG item);

/**
 * Delete all schemas definitions from the table definition.
 *
 * @param TblHdl the Table handle
 *
 * @return error code
 */
COUNT ctAttrTableDeleteAllSchemas(CTATTR_HANDLE TblHdl);

/**
 * Associate a Condition handle to the Table handle. It adds the Condition definition to
 * the table definition. The Puropose of condition in the table is to define MRTs
 *
 * @param TblHdl the Table handle
 * @param CndHdl the Condition handle
 *
 * @return error code
 */
COUNT ctAttrTableAddCondition(CTATTR_HANDLE TblHdl, CTATTR_HANDLE CndHdl);

/**
 * Delete the condition definitions from the table definition.
 *
 * @param TblHdl the Table handle
 *
 * @return error code
 */
COUNT ctAttrTableDeleteCondtion(CTATTR_HANDLE TblHdl);

/**
 * Alloc attribute handle for Schema
 *
 */
CTATTR_HANDLE ctAttrSchemaAllocHandle(VOID);

/**
 * Free the Schema attribute handle
 *
 * @param SchHdl the Schema handle to free
 *
 * @return error code
 */
COUNT ctAttrSchemaFreeHandle(CTATTR_HANDLE SchHdl);

/**
 * Reset the Schema attribute handle to the initial state (as it is after allocation)
 *
 * @param TblHdl the Table handle to reset
 *
 * @return error code
 */
COUNT ctAttrSchemaResetHandle(CTATTR_HANDLE SchHdl);

/**
 * Associate a Field handle to the Schema handle. It adds the field definition to
 * the schema definition after existing fields.
 *
 * @param SchHdl the Schema handle
 * @param FldHdl the Field handle
 *
 * @return error code
 */
COUNT ctAttrSchemaAddField(CTATTR_HANDLE SchHdl, CTATTR_HANDLE FldHdl);

/**
 * Associate a Condition handle to the Schema handle. It adds the condition definition to
 * the schema definition.
 *
 * @param SchHdl the Schema handle
 * @param CndHdl the Condition handle
 *
 * @return error code
 */
COUNT ctAttrSchemaAddCondition(CTATTR_HANDLE SchHdl, CTATTR_HANDLE CndHdl);

/**
 * Alloc attribute handle for Field
 *
 */
CTATTR_HANDLE ctAttrFieldAllocHandle(VOID);

/**
 * Free the Field attribute handle
 *
 * @param FldHdl the Field handle to free
 *
 * @return error code
 */
COUNT ctAttrFieldFreeHandle(CTATTR_HANDLE FldHdl);

/**
* Reset Field attribute handle to the initial state(as it is after allocation)
*
* @param FldHdl the Field handle to reset
*
* @return error code
*/
COUNT ctAttrFieldResetHandle(CTATTR_HANDLE FldHdl);

/**
 * Alloc attribute handle for Index
 *
 */
CTATTR_HANDLE ctAttrIndexAllocHandle(VOID);

/**
 * Free the Index attribute handle
 *
 * @param IdxHdl the index handle to free
 *
 * @return error code
 */
COUNT ctAttrIndexFreeHandle(CTATTR_HANDLE IdxHdl);

/**
 * Reset the Index attribute handle to the initial state (as it is after allocation)
 *
 * @param IdxHdl the Index handle to reset
 *
 * @return error code
 */
COUNT ctAttrIndexResetHandle(CTATTR_HANDLE IdxHdl);

/**
 * Associate an Index handle to the Table handle. It adds the index attributes definition to
 * the table definition.
 *
 * @param TblHdl the Table handle
 * @param IdxHdl the Schema handle
 *
 * @return error code
 */
COUNT ctAttrTableAddIndex(CTATTR_HANDLE TblHdl, CTATTR_HANDLE IdxHdl);

/**
 * Associate an Index handle to the Table handle. It insert the index attributes definition to
 * the table definition at the specified position.
 *
 * @param TblHdl the Table handle
 * @param IdxHdl the Schema handle
 * @param ReplaceNull set to YES to indecate that if at "pos" there is a NULL object it is considered
 *        a placeholder and need to be replaced, so the insert doe not expand the array number of entries
 *        but simply replace the NULL entry at "pos"
 *
 * @return error code
 */
COUNT ctAttrTableInsertIndex(CTATTR_HANDLE TblHdl, ULONG pos, CTATTR_HANDLE IdxHdl, UTEXT ReplaceNull);

/**
 * Alloc attribute handle for Segment
 *
 */
CTATTR_HANDLE ctAttrSegmentAllocHandle(VOID);

/**
 * Free the Segment attribute handle
 *
 * @param SegHdl the table handle to free
 *
 * @return error code
 */
COUNT ctAttrSegmentFreeHandle(CTATTR_HANDLE SegHdl);

/**
 * Reset the Segment attribute handle to the initial state (as it is after allocation)
 *
 * @param SegHdl the Segment handle to reset
 *
 * @return error code
 */
COUNT ctAttrSegmentResetHandle(CTATTR_HANDLE SegHdl);

/**
 * Associate a Segment handle to the Index handle. It adds the segment attributes definition to
 * the index definition.
 *
 * @param IdxHdl the Index handle
 * @param SegHdl the Segment handle
 *
 * @return error code
 */
COUNT ctAttrIndexAddSegment(CTATTR_HANDLE IdxHdl, CTATTR_HANDLE SegHdl);

/**
 * Alloc attribute handle for Condition
 *
 */
CTATTR_HANDLE ctAttrConditionAllocHandle(VOID);

/**
* Free the Condition attribute handle
*
* @param Handle the Condition handle to free
*
* @return error code
*/
COUNT ctAttrConditionFreeHandle(CTATTR_HANDLE Handle);

/**
 * Reset the Condition attribute handle to the initial state (as it is after allocation)
 *
 * @param Handle the Condition handle to reset
 *
 * @return error code
 */
COUNT ctAttrConditionResetHandle(CTATTR_HANDLE Handle);

/**
 * Associate a Field handle to the Condition handle. It adds the field definition to
 * the condition definition after existing fields. Fields are then used in Rules
 *
 * @param CndHdl the Condition handle
 * @param FldHdl the Field handle
 *
 * @return error code
 */
COUNT ctAttrConditionAddField(CTATTR_HANDLE CndHdl, CTATTR_HANDLE FldHdl);

/**
 * Associate a Rule handle to the Condition handle. It adds the rule definition to
 * the condition definition after existing rules.
 *
 * @param CndHdl the Condition handle
 * @param RulHdl the Rule handle
 *
 * @return error code
 */
COUNT ctAttrConditionAddRule(CTATTR_HANDLE CndHdl, CTATTR_HANDLE RulHdl);

/**
 * Alloc attribute handle for Rule
 *
 */
CTATTR_HANDLE ctAttrRuleAllocHandle(VOID);

/**
* Free the Rule attribute handle
*
* @param Handle the Rule handle to free
*
* @return error code
*/
COUNT ctAttrRuleFreeHandle(CTATTR_HANDLE Handle);

/**
 * Reset the Rule attribute handle to the initial state (as it is after allocation)
 *
 * @param Handle the Rule handle to reset
 *
 * @return error code
 */
COUNT ctAttrRuleResetHandle(CTATTR_HANDLE Handle);

/**
 * Alloc attribute handle for Rule filter
 *
 */
CTATTR_HANDLE ctAttrRuleFilterAllocHandle(VOID);

/**
* Free the Rule filter attribute handle
*
* @param Handle the Rule filter handle to free
*
* @return error code
*/
COUNT ctAttrRuleFilterFreeHandle(CTATTR_HANDLE ptr);

/**
 * Reset the Rule filter attribute handle to the initial state (as it is after allocation)
 *
 * @param Handle the Rule filter handle to reset
 *
 * @return error code
 */
COUNT ctAttrRuleFilterResetHandle(CTATTR_HANDLE RlfHdl);

/**
 * Associate a Rule filter handle to the Rule handle. It adds the filter definition to
 * the rule definition. Mutually exclusive with ctAttrRuleAddRuleFilterText
 *
 * @param RulHdl the Rule handle
 * @param RflHnd the Rule filter handle
 *
 * @return error code
 */
COUNT ctAttrRuleAddRuleFilter(CTATTR_HANDLE RulHdl, CTATTR_HANDLE RflHnd);

/**
 * Associate a filter text to the Rule handle. It adds the filter definition to
 * the rule definition. Mutually exclusive with ctAttrRuleAddRuleFilter
 *
 * @param RulHdl the Rule handle
 * @param filter the filter text
 *
 * @return error code
 */
COUNT ctAttrRuleAddRuleFilterText(CTATTR_HANDLE RulHdl, pTEXT filter);
/**
 * define an operator for the rule.
 * for unary operator specify lvalue only
 *
 * @param FltHnd the Rule filter handle
 * @param op the opertor attribute
 * @param lvalue the Rule filter handle for the left hand side operator argument, or for the argument for unary operators
 * @param rvalue the Rule filter handle for the right hand side operator argument
 *
 * @return error code
 */
COUNT ctAttrRuleFilterAddOperator(CTATTR_HANDLE FltHnd, CTATTR_ATTRIBUTE op, CTATTR_HANDLE lvalue, CTATTR_HANDLE rvalue);


/* attributes*/
/**
* Add an attribute and sets its value to a Handle. This function allow setting
* Table, Schema, Fields etc. attributes. The function performs a check to verify
* that the attribute belongs to the specified Handle.
*
* @param Hdl the Handle to the the attrbute for.
*
* @param attribute the attribute to set
*
* @param value pointer to the value to set the attribute to. it must be the proper C type expected
*              by the attribute.
*
* @return error code
*/
COUNT ctAttrSetAttributeValue(CTATTR_HANDLE Hdl, CTATTR_ATTRIBUTE attribute, pVOID value);

/**
* Add an attribute and sets its value to a Handle encoded in base64 with padding compression.
* This function allow setting Table, Schema, Fields etc. attributes containing binary data.
* The function performs a check to verify that the attribute belongs to the specified Handle.
*
* @param Hdl the Handle to the the attrbute for.
*
* @param attribute the attribute to set
*
* @param value pointer to the value to set the attribute to.
*
* @param size size in bytes of the binary buffer pointer by value.
*
* @return error code
*/
COUNT ctAttrSetAttributeBinaryValue(CTATTR_HANDLE Hdl, CTATTR_ATTRIBUTE attribute, cpTEXT value, VRLEN size);

/**
* Retrieve the value of the specified attribute in the specified Handle.
* The function performs a check to verify that the attribute belongs to
* the specified Handle.
*
* @param Hdl the Handle into which look for the attribute
*
* @param attribute the attribute to retrieve
*
* @param value pointer to the value to set to the attribute value, it must be the proper C type expected
*              by the attribute. In case of ppTEXT t will be a reference to internal memory guarantted valid
*              as long as the "Hdl" is valid and the attribute not changed.
*
* @return error code
*/
COUNT ctAttrGetAttributeValue(CTATTR_HANDLE Hdl, CTATTR_ATTRIBUTE attribute, pVOID value);

/**
* Retrieve the value of the specified attribute in the specified Handle.
* The function performs a check to verify that the attribute belongs to
* the specified Handle.
*
* @param Hdl the Handle into which look for the attribute
*
* @param attribute the attribute to retrieve
*
* @param value pointer to a buffer where the attribute value is returned.
*
* @param size size of the buffer passed in. in output the number of bytes
* used (or required if not large enough) to store the binary value.
*
* @param pad when set to 1 pad the input buffer using the last stored byte
*
* @return error code
*/
COUNT ctAttrGetAttributeBinaryValue(CTATTR_HANDLE Hdl, CTATTR_ATTRIBUTE attribute, pTEXT value, pVRLEN size, TEXT pad);


/**
* Retrieve the value of the specified attribute in the specified Handle.
* the value is a LONG which is the interpretation of the attribute value for
* attributs which value is a mnemonic string for a specific setting.
* The function performs a check to verify that the attribute belongs to
* the specified Handle.
*
* @param Hdl the Handle into which look for the attribute
*
* @param attribute the attribute to retrieve
*
* @param parsedvalue pointer to a variable where to sotre the parsed value.
*
* @param info pointer to a string contaigin extra information. the pointer points to internal memory
* valid as long as the Hdl is valis and the atribute value is not changed.
*
* @return error code
*/
COUNT ctAttrGetParsedAttributeValue(CTATTR_HANDLE Hdl, CTATTR_ATTRIBUTE attribute, pLONG parsedvalue, ppTEXT info);


/* structural items scanning */
/**
* Retrieve number of Schemas defined on the Table
*
* @param TblHdl the Table Handle
*
* @return the number of defined Schemas
*/
UNLONG ctAttrTableNumberOfSchemas(CTATTR_HANDLE TblHdl);

/**
* Fullfill the Schema handle passed in with information for the Nth Schema
*
* @param TblHdl the Table Handle the Schema belongs to.
*
* @param item the number of the item to retrieve (0 based).
*
* @param SchHdl the Schema handle to fullfill with the Nth Schema information
*
* @return error code
*/
COUNT ctAttrTableGetNthSchema(CTATTR_HANDLE TblHdl, UNLONG item, CTATTR_HANDLE SchHdl);

/**
* Retrieve number of Indexes defined on the Table
*
* @param TblHdl the Table Handle
*
* @return the number of defined Indexes
*/
UNLONG ctAttrTableNumberOfIndexes(CTATTR_HANDLE TblHdl);

/**
* Fullfill the Index handle passed in with information for the Nth Index
*
* @param TblHdl the Table Handle the Index belongs to.
*
* @param item the number of the item to retrieve (0 based).
*
* @param IdxHdl the Index handle to fullfill with the Nth Index information
*
* @return error code
*/
COUNT ctAttrTableGetNthIndex(CTATTR_HANDLE TblHdl, UNLONG item, CTATTR_HANDLE IdxHdl);

/**
* Delete the Nth Index
*
* @param TblHdl the Table Handle the Index belongs to.
*
* @param item the number of the item to delete(0 based).
*
* @return error code
*/
COUNT ctAttrTableDeleteNthIndex(CTATTR_HANDLE TblHdl, UNLONG item);

/**
* Fullfill the Condition handle passed in with information for the condition defined on the table
*
* @param TblHdl the Table Handle the Index belongs to.
*
* @param CndHdl the Condition handle to fullfill with the information
*
* @return error code
*/
COUNT ctAttrTableGetCondition(CTATTR_HANDLE TblHdl, CTATTR_HANDLE CndHdl);

/**
* Retrieve number of Schemas defined on the Table
*
* @param SchHdl the Schema Handle
*
* @return the number of Fields in the schema
*/
UNLONG ctAttrSchemaNumberOfFields(CTATTR_HANDLE SchHdl);

/**
* Fullfill the Field handle passed in with information for the Nth Field
*
* @param  SchHdl the Schema handle the Field belongs to.
*
* @param item the number of the item to retrieve (0 based).
*
* @param FldHdl the Schema handle to fullfill with the Nth Schema information
*
* @return error code
*/
COUNT ctAttrSchemaGetNthField(CTATTR_HANDLE SchHdl, UNLONG item, CTATTR_HANDLE FldHdl);

/**
* Delete the Nth Field
*
* @param SchHdl the Schema Handle the Field belongs to.
*
* @param item the number of the item to delete(0 based).
*
* @return error code
*/
COUNT ctAttrSchemaDeleteNthField(CTATTR_HANDLE SchHdl, UNLONG item);

/**
* Fullfill the Condition handle passed in with information for the condition defined on the schema
* the condidion on schemas defined the rule that apply to virtual fields.
*
* @param  SchHdl the Schema handle the Field belongs to.
*
* @param FldHdl the Schema handle to fullfill with the Nth Schema information
*
* @return error code
*/
COUNT ctAttrSchemaGetCondition(CTATTR_HANDLE SchHdl, CTATTR_HANDLE CndHdl);


/**
* Retrieve number of Indexes defined on the Table
*
* @param IdxHdl the Index Handle
*
* @return the number of segments in the Index
*/
UNLONG ctAttrIndexNumberOfSegments(CTATTR_HANDLE IdxHdl);

/**
* Fullfill the Segment handle passed in with information for the Nth Segment
*
* @param IdxHdl the Index handle the Segment belongs to.
*
* @param item the number of the item to retrieve (0 based).
*
* @param SegHdl the Segment handle to fullfill with the Nth Segment information
*
* @return error code
*/
COUNT ctAttrIndexGetNthSegment(CTATTR_HANDLE IdxHdl, UNLONG item, CTATTR_HANDLE SegHdl);

/**
* Retrieve number of Fileds defined on the Condition
*
* @param CndHdl the Condition Handle
*
* @return the number of segments in the Index
*/
UNLONG ctAttrConditionNumberOfFields(CTATTR_HANDLE CndHdl);

/**
* Fullfill the Field handle passed in with information for the Nth Field defined on the Condition
*
* @param CndHdl the Condition handle the Field belongs to.
*
* @param item the number of the item to retrieve (0 based).
*
* @param FldHdl the Field handle to fullfill with the Nth Field information
*
* @return error code
*/
COUNT ctAttrConditionGetNthField(CTATTR_HANDLE CndHdl, UNLONG item, CTATTR_HANDLE FldHdl);

/**
* Delete the Nth Field defined on the Condition
*
* @param CndHdl the Condition Handle the Rule belongs to.
*
* @param item the number of the item to delete(0 based).
*
* @return error code
*/
COUNT ctAttrConditionDeleteNthField(CTATTR_HANDLE CndHdl, UNLONG item);

/**
* Retrieve number of Rules defined on the Condition
*
* @param CndHdl the Condition Handle
*
* @return the number of segments in the Index
*/
UNLONG ctAttrConditionNumberOfRules(CTATTR_HANDLE CndHdl);

/**
* Fullfill the Rule handle passed in with information for the Nth Rule defined on the Condition
*
* @param CndHdl the Condition handle the Field belongs to.
*
* @param item the number of the item to retrieve (0 based).
*
* @param RulHdl the Field handle to fullfill with the Nth Rule information
*
* @return error code
*/
COUNT ctAttrConditionGetNthRule(CTATTR_HANDLE CndHdl, UNLONG item, CTATTR_HANDLE RulHdl);

/**
* Delete the Nth Rule
*
* @param CndHdl the Condition Handle the Rule belongs to.
*
* @param item the number of the item to delete(0 based).
*
* @return error code
*/
COUNT ctAttrConditionDeleteNthRule(CTATTR_HANDLE CndHdl, UNLONG item);

/**
* Fullfill the Rule filter handle passed in with information for the Rule filter defined on the Rule
*
* @param RulHdl the Rule handle.
*
* @param RflHdl the Rule filter handle to fullfill with the Rule filter information
*
* @return error code
*/
COUNT ctAttrRuleGetRuleFilter(CTATTR_HANDLE RulHdl, CTATTR_HANDLE RflHdl);

/**
* set filter to point to the filter string.
*
* @param RulHdl the Rule handle.
*
* @param filter in output the filter text. on Erro NULL. on success it points to internal memory holding the filter text.
*               the memory is valid until the handle is valid.
*
* @return error code
*/
COUNT ctAttrRuleGetRuleFilterText(CTATTR_HANDLE RulHdl, ppTEXT filter);

/**
* Retrieves the operator for the rule filter passed in
*
* @param FltHnd the Rule filter handle
*
* @return the operator as an attribute
*/
CTATTR_ATTRIBUTE ctAttrRuleFilterGetOperator(CTATTR_HANDLE FltHnd);

/**
* Fullfill the Rule filter handle passed in (valHnd) with information for the Nth Argument in the Rule filter (RflHdl)
*
* @param RflHdl the Rule filter handle containing the arguments
*
* @param item the number of the item to retrieve (0 based).
*
* @param valHnd the Rule filter handle to fullfill with the Nth argument information
*
* @return error code
*/
COUNT ctattrRuleFilterGetNthArg(CTATTR_HANDLE RflHdl, UNLONG item, CTATTR_HANDLE valHnd);



/* attributes iterator*/

/**
* Free the memory associated with an iterator
*
* @param iterator the iterator to free
*
*/
VOID ctAttrFreeIterator(CTATTR_ITERATOR iterator);

/**
* Allocate an iterator to scan the specified handle attributes.
*
* @param Hnd the Table, Schema, Field etc. handle scope of the iterator
*
* @return the iterator. NULL on error
*/
CTATTR_ITERATOR ctAttrAllocIterator(CTATTR_HANDLE Hnd);

/**
* retrieve the next attribute using the iterator
*
* @param iter the iterator
*
* @param attr the attribute found
*
* @param attrtype the attribue data type
*
* @param value the attribute value (to be cast to a pointer to the "attrtype")
*
* @return error code.
*/
COUNT ctAttrIteratorNext(CTATTR_ITERATOR iter, CTATTR_ATTRIBUTE *attr, CTATTR_ATTRIBUTE_TYPE *attrtype, pVOID value); /* return 0 on success, 1 if at end*/

/**
* retrieve the number of attributes specified in the Handle
*
* @param Hdl the handle
*
* @param structitems the number of structural items found
*
* @return the number of attributes, set structitems to the number of structural items found.
*         worth noticing that unkonwn to the AttrAPI structural items are counted as attributes.
*/
UNLONG ctAttrNumberOfAttributes(CTATTR_HANDLE Hdl, UNLONG *structitems);

/* persist and retrieve attributes*/
/**
* persist the Table Handle attributes and all attributes for associate handles into the data file.
*
* @param TblHdl the Table Handle
*
* @param datno the file number where to store the information
*
* @param forced_version the resource version to use (0 to use the most current).
*
* @param update specify if an attempt to update an existing rsource should be done.
*
* @return error code.
*/
COUNT ctAttrTableSave(CTATTR_HANDLE Handle, FILNO datno, LONG forced_version, TEXT update);

/**
* Fullfill the tableHandle with information retrievedfrom the specified data file.
*
* @param TblHdl the Table Handle
*
* @param datno the file number where to store the information
*
* @param res_ver [output] resource version (may be used for backward compatibility)
*
* @return error code.
*/

COUNT ctAttrTableLoad(CTATTR_HANDLE Handle, FILNO datno, pLONG res_ver);

/**
* the JSON content of the attribute resource (pretty printed).
*
* @param TblHdl the Table Handle
*
* @return the JOSON text or NULL in case of error. The returned pointer points to memory released with table handle.
*/
pTEXT ctAttrTableGetJSON(CTATTR_HANDLE Handle);

/**
* Fullfill the fieldarray with information retrieved from the specified Condition handle
*
* @param CndHdl the Condition Handle
*
* @param infields Field array to fullfill
*
* @param rechdr the record header size
*
* @param numconv the numeric convention in use
*
* @param sort set to YES to alphabetically sort the array on fieldname
*
* @param alloc set to YES to alloc memory instead of reference to the JSON
*
* @return error code.
*/
COUNT ctAttrConditionGetFieldsArray(CTATTR_HANDLE CndHdl, CTATTR_FIELDS_ARRAY *infields, VRLEN rechdr, CTATTR_RTGNUMCONV numconv, TEXT sort, TEXT alloc);

/**
* Fullfill the fieldarray with information retrieved from the specified Schema handle
*
* @param SchHdl the Schema Handle
*
* @param infields Field array to fullfill
*
* @param rechdr the record header size
*
* @param numconv the numeric convention in use
*
* @param sort set to YES to alphabetically sort the array on fieldname
*
* @param alloc set to YES to alloc memory instead of reference to the JSON
*
* @return error code.
*/
COUNT ctAttrSchemaGetFieldsArray(CTATTR_HANDLE SchHdl, CTATTR_FIELDS_ARRAY* infields, VRLEN rechdr, CTATTR_RTGNUMCONV numconv, TEXT sort, TEXT alloc);

/**
* Free the fieldarray memory if there is no more reference to it, otherwise decrease the reference count.
*
* @param fields Field array to free
*
* @param numconv the numeric convention in use
*
*/
VOID ctAttrFieldsArrayFree(CTATTR_FIELDS_ARRAY *fields);

/**
* get a reference to the fieldarray by increasing the reference count.
* the "reference" as well as the original need to be released by calling ctAttrFieldsArrayFree.
*
* @param fields Field array
*
* @return Field array pointer (matches the fields paramter)
*
*/
CTATTR_FIELDS_ARRAY* ctAttrFieldsArrayGetReference(CTATTR_FIELDS_ARRAY* fields);

/**
* alloc the fieldarray
*
*/
CTATTR_FIELDS_ARRAY *ctAttrFieldsArrayAlloc(VOID);

/**
* Find the field named "fieldname" in the fieldarray
*
* @param fields Field array to search into
*
* @param fieldname the field name to search for
*
* @return field information or NULL if not found
*
*/
CTATTR_FLDINFO *ctAttrFieldsArrayFind(CTATTR_FIELDS_ARRAY *fields, pTEXT fieldname);

/**
* Sort the field array by field name
*
* @param fields Field array to sort
*
*/
VOID ctAttrFieldsArraySort(CTATTR_FIELDS_ARRAY* fields);

/**
* return the Mnemonic associated to the provided value for the specified attribute.
*
* @param attr the attribute
*
* @param value the value
*
* @return the a pointer to mnemonic string
*         Pointed memory is valid until the Handle is not release or the JSON text regenerated.
*/
cpTEXT ctAttrFindAttributeMnemonic(CTATTR_ATTRIBUTE attr, NINT value);

/**
* Check if the table has condition defined.
*
* @param TblHdl the Table Handle.
*
* @return YES if the table has conditions NO otherwise.
*/
TEXT ctAttrTableHasCondition(CTATTR_HANDLE TblHdl);

/**
* Check if the Schema has condition defined.
*
* @param SchHdl the Schema Handle.
*
* @return YES if the schema has condition NO otherwise.
*/
TEXT ctAttrSchemaHasCondition(CTATTR_HANDLE SchHdl);

/**
* Fullfill the fieldarray with information retrieved from the specified filno and Schema
*
* @param datno the file to extract the filed array from.
*
* @param schemano the schama number to extract (use -1 for MRT host)
*
* @param fldarr Field array to fullfill
*
* @param doSchema Fill also schema and names in Field array
*
* @return error code.
*/
COUNT ctAttrFileGetSchemaFieldsArray(FILNO datno, NINT schemano, CTATTR_FIELDS_ARRAY *fldarr, TEXT doSchema);


/**
*  reconstruct SCHEMA, DODA, fldarray from the attributes.
*
* @param SchAttrHnd [IN]
*				Schema/Condition attribute handle
*
* @param rechdr [IN]
*               record header size in bytes
*
* @param rowidx [IN]
*               1 if rowid (SRLSEG index) is present, 0 if not.
*
* @param srlsegsiz [IN]
*               size in bytes of the serial segment (0 if not used)
*
* @param struct_align [IN]
*               struct_alignment
*
* @param schemaptr [OUT]
*               pointer to the "schema" allocated interannly by ctdb_alloc. The user needs to release the memory calling  _ctdb_free.
*
* @param dodaptr [OUT]
*               pointer to the "schema" allocated interannly by ctdb_alloc. The user needs to release the memory calling  _ctdb_free.
*
* @param schemasize [OUT]
*               the size in bytes of the buffer allocated for the schema.
*
* @param dodasize [INOUT]
*               in input the size in bytes of the record (minlen), used with DODAFromATTR_DODAmrthost
*               in output the number of fields (for DODAFromATTR_DODAput and DODAFromATTR_DODAmrthost)
*                         the size in bytes of the buffer allocated for the schema
*
* @param mem_alloc [IN]
*               function to allocate DODA, SCHEMA ..., NAMES, FLDARRAY. if NULL internal MemAlloc will be used
*
* @param schemamem_freesize [IN]
*               function to free DODA, SCHEMA ... , NAMES, FLDARRAY. if NULL internal MemAlloc will be used
*
* @param mode [IN]
*               the "mode" for this call. Valid values:
*               CTATTR_SCHEMAGET_DODA - regular doda
*               CTATTR_SCHEMAGET_DODAHOST -  main doda (used for filters) for MRT host, for immediate PUTDODA
*               CTATTR_SCHEMAGET_VDODA - doda for virtual fields (not complete yet).
*
*                - can be or'd in the mode to indicate that field names can be direct
*                             reference into the Schema/Condition attribute handle.
*
* @return error code.
*/

COUNT  ctAttrSchemaGetDODA(CTATTR_HANDLE SchAttrHnd, UTEXT rechdr,
                           TEXT rowidx, UTEXT srlsegsiz, UTEXT struct_align,
                           pConvMap* schemaptr, pDATOBJ* dodaptr, pVRLEN schemasize, pVRLEN dodasize,
                           ctAttrAllocFunc mem_alloc, ctAttrFreeFunc mem_free, COUNT mode);


/**
*  generates filter text.
*
* @param CndHnd [IN]
*				Condition handle
*
* @param item [IN]
*               item (rule) number to generate the filter text for.
*
* @param fields [IN]
*               field array for the CndHnd.
*
* @param usefldNames [IN]
*               YES to generate a filter using field names, NO to use the "Field" function instead
*
* @param filterText [OUT]
*               pointer to a stinrg (char *) where the function allocae memory and stores the filter text. Memory needs to be released using the "mem_free" function.
*
* @param mem_alloc [IN]
*               pointer to the function that needs to be used to allocate memory (must be specified).
*
* @param mem_free [IN]
*               pointer to the function that needs to be used to free memory (must be specified).
**
* @return error code.
*/
COUNT ctAttrRuleGenerateFilterText(CTATTR_HANDLE CndHnd, UNLONG item, CTATTR_FIELDS_ARRAY* fields, TEXT usefldNames, ppTEXT filterText, ctAttrAllocFunc mem_alloc, ctAttrFreeFunc mem_free);

/**
*  generates filter PTREE array.
*
* @param SchHnd [IN]
*              Schema handle
*
* @param rechdr [IN]
*               record header length.
*
* @param numconv [IN]
*               numeric convention in use (RTG).
*
* @param filters [OUT]
*               pointer to an array of PTREE that will be allocated by the function usinf the mem_alloc function passed in.
*               The user needs to free this array and the PTREE it points to
*
* @param mem_alloc [IN]
*               pointer to the function that needs to be used to allocate memory (must be specified).
*
* @param mem_free [IN]
*               pointer to the function that needs to be used to free memory (must be specified).
**
* @return error code.
*/
COUNT ctAttrSchemaGetFilters(CTATTR_HANDLE SchHnd, VRLEN rechdr, CTATTR_RTGNUMCONV numconv, pVOID* filters, pNINT entries, ctAttrAllocFunc mem_alloc, ctAttrFreeFunc mem_free);

/**
* return the json property name associated for the specified attribute.
*
* @param attr the attribute
*
* @return the a pointer to mjson property name string
*/
cpTEXT ctAttrFindAttributePropertyName(CTATTR_ATTRIBUTE attr);
#endif /* ~_CTATTRAPI_H_ */

/* end of ctattrapi.h */

