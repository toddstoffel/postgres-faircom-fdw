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

#ifndef _CTVARIANT_H
#define _CTVARIANT_H

typedef enum {  /* these are the supported datatypes and type encoding combinations. */
	/* base types (based on nav types)*/
	ct_variant_INVALID        = 0x00000000,
	ct_variant_VARCHAR        = 0x00000001,
	ct_variant_VARBINARY      = 0x00000002,
	ct_variant_TINYINT        = 0x00000003,
	ct_variant_SMALLINT       = 0x00000004,
	ct_variant_INTEGER        = 0x00000005,
	ct_variant_FLOAT          = 0x00000006,
	ct_variant_DOUBLE         = 0x00000007,
	ct_variant_NUMBER         = 0x00000008,
	ct_variant_DATE           = 0x00000009,  /* CTDATE */
	ct_variant_TIME           = 0x0000000a,  /* CTTIMEMS*/
	ct_variant_TIMESTAMP      = 0x0000000b,  /* CTDATETIME  (with milliseconds support) */
	ct_variant_BIGINT         = 0x0000000c,
	ct_variant_BIT            = 0x0000000d,
	ct_variant_JSON           = 0x0000000e,  /* JSON document as string */
	/* when adding a new base type, increase ct_variant_LAST_BASIC below
	   and update variant_basic in ctdbvrnt.c*/

	ct_variant_LAST_BASIC     = 0x0000000f,  /* MUST be less than 0x00001000 and have the "next available value */

	/* extended native types */
	ct_variant_UTF16          = 0x00001000, /* not '\0\0' terminated since the size is determined by the varyant size */
	ct_variant_ALPHAEDIT      = 0x00001001,
	ct_variant_JUSTAN         = 0x00001002,
	ct_variant_NUMSIGNED      = 0x00001003,
	ct_variant_NUMSIGNSEP     = 0x00001004,
	ct_variant_NUMSEPLEAD     = 0x00001005,
	ct_variant_NUMLEADING     = 0x00001006,
	ct_variant_SIGNEDCOMP2    = 0x00001007,
	ct_variant_SIGNEDCOMP3    = 0x00001008,
	ct_variant_NUMUNSIGNED    = 0x00001009,
	ct_variant_COMP3          = 0x0000100a,
	ct_variant_COMP2          = 0x0000100b,
	ct_variant_COMP6          = 0x0000100c,
	ct_variant_BINARYSIGNED   = 0x0000100d, /* this is an integer in big ending format */
	ct_variant_BINARYUNSIGNED = 0x0000100e, /* this is an unsigned integer in big ending format */
	ct_variant_BT_DATE        = 0x0000100f,
	ct_variant_BT_TIME        = 0x00001010,
	ct_variant_NUMEDIT        = 0x00001011,
	/* when adding a new base type, increase ct_variant_LAST_CTNATIVE below
	   and update variant_native in ctdbvrnt.c */
	/* ct_variant_LAST_CTNATIVE MUST be less than 0x00002000 and have the "next available value */
	ct_variant_LAST_CTNATIVE  = 0x00001012, /* up to 0x00001FFF*/

	/* advanced types */
	ct_variant_XML            = 0x00002000,
	ct_variant_HTML           = 0x00002001,
	ct_variant_JAVASCRIPT     = 0x00002002,
	ct_variant_SQL_SCRIPT     = 0x00002003,
	ct_variant_CSS            = 0x00002004,
	ct_variant_CSV            = 0x00002005,
	ct_variant_CSV_SCHEMA     = 0x00002006,
	ct_variant_MARKDOWN       = 0x00002007,
	ct_variant_PLAIN          = 0x00002008,
	ct_variant_RTF            = 0x00002009,
	ct_variant_TAB_SEPARATED_VALUES = 0x0000200a,
	ct_variant_TURTLE         = 0x0000200b,
	ct_variant_VCARD          = 0x0000200c,
	/* when adding a new base type, increase ct_variant_LAST_TEXT below
	   and update variant_ext_text in ctdbvrnt.c */
	/* ct_variant_LAST_TEXT MUST be less than 0x00003000 and have the "next available value */
	ct_variant_LAST_TEXT      = 0x0000200d, /* up to 0x00002FFF*/

	ct_variant_MP4            = 0x00003000,
	ct_variant_QUICKTIME      = 0x00003001,
	ct_variant_BMP            = 0x00003002,
	ct_variant_EMF            = 0x00003003,
	ct_variant_GIF            = 0x00003004,
	ct_variant_JPEG           = 0x00003005,
	ct_variant_SVG            = 0x00003006,
	ct_variant_PNG            = 0x00003007,
	ct_variant_TIFF           = 0x00003008,
	ct_variant_FLAC           = 0x00003009,
	ct_variant_TTF            = 0x0000300a,
	ct_variant_MPEG           = 0x0000300b,
	ct_variant_OGG            = 0x0000300c,
	ct_variant_MP4A_LATM      = 0x0000300d,
	ct_variant_MPA_ROBUST     = 0x0000300e,
	ct_variant_MPEG4_GENERIC  = 0x0000300f,
	ct_variant_OPUS           = 0x00003010,
	ct_variant_SP_MIDI        = 0x00003011,
	ct_variant_RTP_MIDI       = 0x00003012,
	ct_variant_OTF            = 0x00003013,
	/* when adding a new base type, increase ct_variant_LAST_RESERVED below
	  and update variant_ext in ctdbvrnt.c */
	/* ct_variant_LAST_RESERVED MUST be less than 0x00100000 and have the "next available value */
	ct_variant_LAST_RESERVED  = 0x00003014  /* up to 0x000FFFFF*/

} ct_variant_datatype;  /* these are the supported datayep and type encoding combinations. */


/* The "typeEncoding" specifies additional steps for encoding and decoding the "value" to/from disk */
typedef enum {
	ct_tenc_invalid = 0,
	ct_tenc_native = 1 /* no encodige, store on disk "as is" */
} ct_variant_typeenc;


/* The "valueEncoding" specifies additional steps for encoding and decoding the "value" in JSON */
typedef enum {
	_ct_encoding_binary = -1, /* there is no mnemonic for this value used internally to signify that we reached a binary buffer */
	ct_encoding_invalid = 0,
	ct_encoding_string = 1,
	ct_encoding_integer = 2, /* 8 bytes integer */
	ct_encoding_float = 3, /* 8 bytes */
	ct_encoding_boolean = 4,
	ct_encoding_json = 5 ,

	/* particular string encoding for datetime and binaries */
	ct_encoding_base64 = 6,
	ct_encoding_hex = 7,
	ct_encoding_iso8601 = 8
	/* when adding a new encoding update variant_ext in ctdbvrnt.c */

} ct_variant_encoding;

#endif /* _CTVARIANT_H */

/* end of ctvariant.h */

