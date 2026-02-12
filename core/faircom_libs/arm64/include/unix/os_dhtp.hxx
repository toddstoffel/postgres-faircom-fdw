
/*
 *	Copyright (C) Dharma Systems Inc.        1992 - 2024.
 *	Copyright (C) Dharma Systems (P) Ltd.    1992 - 2024.
 *
 *	This Module contains Proprietary Information of
 *	Dharma Systems Inc. and Dharma Systems (P) Ltd.  
 *	and should be treated as Confidential.
 */

/*
 * $RCSfile: os_dhtp.hxx,v $ $Revision: 1.1.1.1 $Date: 2006/08/04 07:25:50 $
 * $State: Exp $
 */

/* --------------------------------------------------------------------
* Change history
* --------------------------------------------------------------------
* Project number   : DC094, DC095
* Date             : 01 - 03 - 1997
* Description      : 1. For better portability of Dharma/SQL across 
*		      various platforms, Dharma specific datatypes 
*		      are defined for the data types. Dharma/SQL source 
*		      uses Dharma data types. Based on the  platform
*		      the definition for the datatypes will get 
*		      modified.
*		      2. The range of the datatypes has been defined
*		      for uniformity.
*		      3. The definition for character data type will
*		      get modified based on the code format supported.
*		      Ex : UNICODE
*		      4. The assumption that the null terminator for
*		      a string is 1 byte wide becomes invalid with
*		      code formats like UNICODE where the null 
*		      terminator is of 2 bytes. DH_NULLTERMSZ is
*		      defined to handle such discrepancies.
*		      5. To support UNICODE format, the string 
*		      literals and character literals has to pass
*		      through the macros DH_STRING_LITERAL and
*		      DH_CHAR_LITERAL.
*		      6. The assumption that the sizeof(any pointer)
*		      is 4 bytes wide becomes invalid in UNICODE.
*		      DH_VOIDSTARSZ is provided to handle this
*		      discrepancy.
* -------------------------------------------------------------------
*/

/* -------------------------------------------------------------------------
 * Change History
 * -------------------------------------------------------------------------
 * Project Number : DC125
 * Date           : 1997/06/20
 * Description    : Transformation of YEAR and HOUR scalar function for
 *                  effective usage of indexes.
 *                  Like predicate transformation
 * -------------------------------------------------------------------------
*/
                                                    
/*--------------------------------------------------------------------------
// Change History
//--------------------------------------------------------------------------
// Project Number      :DS_19960412_472
// Date                : 12-06_1997
// Project             : Bugfix - DS_19960412_472 (time_t used incosistently)
//--------------------------------------------------------------------------
*/

/* --------------------------------------------------------------------
// Change history
// --------------------------------------------------------------------
// Project number   : DC126/DS_19960624_552
// Date             : 1996/06/06 
// Title            : Problems when turn up DBL_MAX to system values. 
// --------------------------------------------------------------------
*/

/* ------------------------------------------------------------------------
 * Change history
 * ------------------------------------------------------------------------
 * Project number : DC142
 * Date           : 1997/06/23
 * Title          : 64-bit int representation for big_t (long long support)
 * ------------------------------------------------------------------------
 */                                                                   

/*
// -------------------------------------------------------------------------
// Change History
// -------------------------------------------------------------------------
// Project Number : DC359
// Date           : 1999-JAN-29
// Title          : Porting of ODBC SDK ver 6.2 to Linux OS.
// -------------------------------------------------------------------------
*/
/* ------------------------------------------------------------------------
 * Change history
 * ------------------------------------------------------------------------
 * Project number : DC514 - DC_20020327_3005
 * Date           : 2002/04/09
 * Title          : dh_address_t used for holding address expressions can be
 *                  replaced by the ANSI standard type ptrdiff_t declared
 *                  by <stddef.h>.
 * ------------------------------------------------------------------------
 */ 
/* ------------------------------------------------------------------------
 * Change history
 * ------------------------------------------------------------------------
 * Project number : dc624
 * Date           : 2003-SEP-16
 * Title          : Integration of the project "dc616 : Porting
 *                  Dharma/SQL 7.4+  to 64 bit jdk 1.4.2"
 * Description    : Defined dh_longlong_t.
 * ------------------------------------------------------------------------
 */
/* --------------------------------------------------------------------------
 * Change History
 * --------------------------------------------------------------------------
 * Project Number : DC771
 * Date           : 2011-JAN-27 
 * Title          : SDK12.0 Unicode build on Linux.  
 * --------------------------------------------------------------------------
 */
/* -------------------------------------------------------------------------
 * Change History
 * --------------------------------------------------------------------------
 * Project number   : DS_20140314_0004
 * Date             : 2014-MAR-14
 * Title            : Scrollable Cursors
 * --------------------------------------------------------------------------
 */
/* --------------------------------------------------------------------------
 * Change History
 * --------------------------------------------------------------------------
 * Project number   : DC777
 * Date             : 2014-APR-09
 * Title            : Dharma/SQL Access 14.0 Release.
 * --------------------------------------------------------------------------
 */
/* -------------------------------------------------------------------------
 * Change History
 * --------------------------------------------------------------------------
 * Project number   : DC_20170106_4264
 * Date             : 2017-JAN-06
 * Title            : Support for SEQUENCES.
 * --------------------------------------------------------------------------
 */

#ifndef	os_dhtp_included
#define	os_dhtp_included

#if defined (c_plusplus) && !defined (__cplusplus)
#define	__cplusplus
#endif

#if !defined (c_plusplus) && defined (__cplusplus)
#define	c_plusplus
#endif

#if defined (__cplusplus)
extern          "C"
{
#endif

#if !defined (__cplusplus)
#define	inline static
#endif

#include   "stdio.h"
#include   "limits.h"
#if (!defined(DH_OS_UNIX_FREEBSD8) && !defined(DH_OS_UNIX_MACOS) && !defined(DH_OS_UNIX_QNXRTP) )
#include   "values.h"
#endif
#include   "time.h"

#if (defined(DH_OS_UNIX_FREEBSD8) || defined(DH_OS_UNIX_MACOS) && !defined(DH_OS_UNIX_QNXRTP))
#include   <stddef.h>
#include   <stdlib.h>
#else
#include   "malloc.h"		/* DC514 - DC_20020327_3005 */
#endif

#ifdef DH_OS_UNIX_QNXRTP
#include   <stddef.h>
#endif

/* DC569 DC_20020427_3051 */
/* long long is supported */
 #define DH_LONGLONG_SUPPORT 

/* DC094 , DC095 BEGIN
* Definitions for Dharma specific  data types. 
*/

/* DC142 dharma specifil long long data type */

//dc624 BEGIN : Integration of the project "dc616 : Porting
//              Dharma/SQL 7.4+  to 64 bit jdk 1.4.2"

// dh_longlong_t is required to make stored procedures and trigger to
// work correctly. This type is used in C++ side of JSP to cast the address
// before passing to Java side. Without this casting, 32 bit address was not
// passed correctly to Java side(Java side uses a 64 bit integer to hold the
// address). But, when DH_LONGLONG_SUPPORT was defined, big_t type was not
// working correctly. Hence, as a hack, dh_longlong_t is mapped without
// setting DH_LONGLONG_SUPPORT flag. The problem with big_t when
// DH_LONGLONG_SUPPORT is defined should be fixed.

#ifdef DH_LONGLONG_SUPPORT
typedef   long long             dh_longlong_t;
typedef   unsigned long long    dh_ulonglong_t; //DC_20170106_4264
#endif

//dc624 END : Integration of the project "dc616 : Porting
//            Dharma/SQL 7.4+  to 64 bit jdk 1.4.2"

typedef   char    		dh_i8_t;
typedef   unsigned char 	dh_u8_t;
typedef   short    		dh_i16_t;
// 64bit build Begin
#if defined DH_64BIT_MODEL
typedef   int                   dh_i32_t;
typedef   unsigned int          dh_u32_t;
#else
typedef   long     		dh_i32_t;
typedef   unsigned long         dh_u32_t;
#endif
// 64bit build End
typedef   float    		dh_f32_t;
typedef   double   		dh_f64_t;

typedef   unsigned short   	dh_u16_t;
//typedef   unsigned long    	dh_u32_t;

typedef   unsigned char    	dh_boolean;
/* DS_20140314_0004 BEGIN */
typedef   dh_boolean            dh_flags;
enum dh_flags_enum
{
  DH_FLAG_DYNAMIC  = 1,
  DH_FLAG_SCROLLABLE
};

/* DS_20140314_0004 END */

typedef   FILE     		dh_file_t;

#ifdef DH_OS_UNIX_QNXRTP
typedef unsigned short  ushort;
#endif

/* DC431 BEGIN
   A new type dh_address_t introduced to hold addresses. This is int in 32
   bit build and long(64) in 64 bit build
 */
/* DC777 Begin */
/* Changes for version build on SCO platform. */
#if (defined DH_OS_UNIX_SCO)
#if defined DH_64BIT_MODEL
typedef long dh_address_t;
#else
typedef int dh_address_t;
#endif
#else
/* DC777 End */
typedef   ptrdiff_t             dh_address_t; /* DC514 - DC_20020327_3005 */
#endif /* DC777 */

/* END DC431 */                                        

/* DS_19960412_472 BEGIN
   A new variable dh_os_time_t is type defined as time_t
*/
typedef   time_t		dh_os_time_t;
/*
   DS_19960412_472 END
*/

/* UNICODE related definitions for character data type. */

#ifndef UNICODE //FAIRCOM_MOD
typedef   char 			dh_char_t;
typedef   unsigned char 	dh_uchar_t;
#else
typedef   wchar_t             	dh_char_t;
#if defined (__cplusplus) /* unsupported in "C" */
#define RAB_220601_FIX	/* Newer compilers did not like the 'unsigned' token */
#ifdef  RAB_220601_FIX
typedef   wchar_t    	dh_uchar_t;
#else
typedef   unsigned wchar_t    	dh_uchar_t;
#endif
#endif
#endif

#ifdef FAIRCOM_MOD
#ifdef CTSQL_UNIX_SHMEM
typedef   void *		SOCKET;
#else
/* DC777 Begin */
/* Porting Customer changes to Dharma/SQL 14.0 */
typedef   dh_i32_t		SOCKET;
/* DC777 End */
#endif
#endif

/* Data type range definitions */

/* Data type range definitions */
/* DS_19960624_552 START 
 * The range of system dependent data types are actually defined  in
 * /usr/include/limits.h or /usr/include/values.h. The range of these
 * data types may vary from one system to another. Instead of hard 
 * coding the range of the Dharma data types, their range should 
 * be set to the defined value of the corresponding system data type
 * which it maps to.
 */ 

#ifdef SCHAR_MIN
	#define   DH_MIN_I8     SCHAR_MIN
#else
	#define   DH_MIN_I8      -128
#endif

#ifdef SCHAR_MAX
	#define   DH_MAX_I8     SCHAR_MAX
#else
	#define   DH_MAX_I8      127
#endif


#ifdef SHRT_MIN
	#define   DH_MIN_I16    SHRT_MIN
#else
	#define   DH_MIN_I16    -32768
#endif

#ifdef SHRT_MAX
	#define   DH_MAX_I16    SHRT_MAX 
#else
	#define   DH_MAX_I16     32767
#endif


#ifdef INT_MIN
	#define   DH_MIN_I32    INT_MIN	
#else
	#define   DH_MIN_I32    -2147483648L
#endif

#ifdef INT_MAX 
	#define   DH_MAX_I32    INT_MAX
#else
	#define   DH_MAX_I32    2147483647L
#endif

/* DC142 max and min values for I64  begin */
/* DC777 Begin */
/* Changes for version build on SCO platform. */
/* #ifdef LLONG_MAX */
#if (defined LLONG_MAX) && (!defined DH_OS_UNIX_SCO)
/* DC777 End */
	#define   DH_MAX_I64    LLONG_MAX
#else
        //DC627
        /*#define   DH_MAX_I64    9223372036854775807*/
        #define   DH_MAX_I64    9223372036854775807LL
        //DC627

#endif

/* DC777 Begin */
/* Changes for version build on SCO platform. */
/* #ifdef LLONG_MIN */
#if (defined LLONG_MIN) && (!defined DH_OS_UNIX_SCO)
/* DC777 End  */
	#define   DH_MIN_I64    LLONG_MIN
#else
        //DC627
	//#define   DH_MIN_I64    -9223372036854775808 
	#define   DH_MIN_I64    -9223372036854775807LL 
        //DC627
#endif
/* DC142 max and min values for I64  end */

#ifdef  FLT_MIN                 
	#define   DH_MIN_F32    FLT_MIN 	
#else
	#define   DH_MIN_F32    1.175494351E-38F
#endif

#ifdef FLT_MAX
	#define   DH_MAX_F32    FLT_MAX	
#else
	#define   DH_MAX_F32    3.402823466E+38F
#endif


#ifdef  DBL_MAX
	#define   DH_MIN_F64    DBL_MIN
#else
	#define   DH_MIN_F64    2.2250738585072014E-308
#endif


#ifdef  DBL_MAX
	#define   DH_MAX_F64    DBL_MAX	
#else
	#define   DH_MAX_F64    1.7976931348623157E+308
#endif

/*  DS_19960624_522 END */

/* UNICODE related definitions for literals.*/

#ifndef UNICODE  //FAIRCOM_MOD from DH_UNICODE
/* DC777 Begin */
/* Esqlc uses the C preprocessor to allow #defines in .pc files
 * because the preprocessor evaluates #defines. It is necessary
 * to be able to include several #defines again when the actual
 * .c file is generated, so move those #defines to a seperate 
 * file os_strlit.hxx.
 */
/* Moved DH_STRING_LITERAL definitions to os_strlit.hxx file */
/* DC777 End */
#define DH_CHAR_LITERAL(char_lit)       char_lit
#define DH_WCHAR_LITERAL(wchar_lit)     L ## wchar_lit    // DC761  // DC771
#else
// DC771 Begin
#define DH_WCHAR_LITERAL_1(wchar_lit)  L ## wchar_lit
#define DH_WCHAR_LITERAL(wchar_lit)     DH_WCHAR_LITERAL_1(wchar_lit) // DC761
#define DH_CHAR_LITERAL_1(char_lit)       L ## char_lit
#define DH_CHAR_LITERAL(char_lit)       DH_CHAR_LITERAL_1(char_lit)
// DC771 End
#endif 
 
/* DC777 Begin */
#include "os_strlit.hxx" 
#include "os_strfnc.hxx"
/* DC777 End */

/* UNICODE related definitions for null terminator.*/
#define DH_NULLTERM_SZ 		sizeof(dh_char_t)


#define DH_VOIDSTAR_SZ 		sizeof(void *)


/*DC125 begin
 *The max value of Char which is 127 right now and it has
 *to be changed according to unicode later
 */
// DC771 Begin
#ifdef _UNICODE
#define DH_CHAR_MAX              0xfffe
#else
#define DH_CHAR_MAX              127
#endif
//DC771 End

/* DC125 end */
                      
/* DC361 DC_19990401_682 BEGIN
** The scheme for minimum and default value of stmt cache size is changed.
** Now there are 8 const values related to stmt cache size.
** These values are OS dependent and should be set based on memory
** available. While setting these values, it should be remebered that
** cache is shared by all threads in thread mode while each process has
** its own cache. These are:- */
const dh_i16_t DEFAULT_STATIC_STMT_CACHE_SIZE_THREAD = 250;
const dh_i16_t DEFAULT_STATIC_STMT_CACHE_SIZE_PROCESS = 50;
const dh_i16_t DEFAULT_DYN_STMT_CACHE_SIZE_THREAD = 250;
const dh_i16_t DEFAULT_DYN_STMT_CACHE_SIZE_PROCESS = 50;
/* DC777 Begin */
/* const dh_i16_t MAX_STATIC_STMT_CACHE_SIZE_THREAD = 800; */
const dh_i16_t MAX_STATIC_STMT_CACHE_SIZE_THREAD = 16000;
/* DC777 End */
const dh_i16_t MAX_STATIC_STMT_CACHE_SIZE_PROCESS = 150;
/* DC777 Begin */
/* const dh_i16_t MAX_DYN_STMT_CACHE_SIZE_THREAD = 800; */
const dh_i16_t MAX_DYN_STMT_CACHE_SIZE_THREAD = 16000;
/* DC777 End */
const dh_i16_t MAX_DYN_STMT_CACHE_SIZE_PROCESS = 150;
/* The cache size can be explicitly specified at runtime using environment
** variables - DH_DYN_CACHED_STATEMENTS and DH_STATIC_CACHED_STATEMENTS.
** The minimum permissible value of cache size is 1.
** DC361 DC_19990401_682 END */

#if defined (__cplusplus)
}				/* extern "C" */

#endif
#endif

