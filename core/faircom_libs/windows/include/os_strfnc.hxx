
/*
 *	Copyright (C) Dharma Systems Inc.        1988-2011.
 *	Copyright (C) Dharma Systems (P) Ltd.    1988-2011.
 *
 *	This Module contains Proprietary Information of
 *	Dharma Systems Inc. and Dharma Systems (P) Ltd.  
 *	and should be treated as Confidential.
 */

/* --------------------------------------------------------------------
 * Change history
 * --------------------------------------------------------------------
 * Project number   : DC777
 * Date             : 2014-MAY-19
 * Title            : Dharma/SQL Access 14.0 Release.
 * ---------------------------------------------------------------------
 */

/* DC777 Begin */
#ifndef os_strfnc_included
#define os_strfnc_included

#include "string.h"

#ifdef ctSQL_SAFE_CRT // FAIRCOM_MOD

// Copy source string to destination string with size limit, ensuring the resulting string is null-terminated but may be truncated.
inline dh_char_t *os_strncpy_s( dh_char_t *destptr, const dh_char_t * srcptr, size_t no_of_char )
{
	if (no_of_char > 0) {
		_tcsncpy_s(destptr, no_of_char, srcptr, _TRUNCATE);
	}
	return(destptr);
}

// Append source string to destination string with size limit, ensuring the resulting string is null-terminated but may be truncated.
inline dh_char_t *os_strncat_s( dh_char_t *dest, size_t destsize, const dh_char_t * src )
{
	size_t curdestlen = _tcslen(dest);
	if (destsize > curdestlen)
		return(os_strncpy_s(dest + curdestlen, src, destsize - curdestlen));
	return(dest);
}

// Copy source string to destination string with size limit, ensuring the resulting string is null-terminated but may be truncated.
inline wchar_t *os_wcsncpy_s( wchar_t *destptr, const wchar_t * srcptr, size_t no_of_char )
{
	if (no_of_char > 0) {
		wcsncpy_s(destptr, no_of_char, srcptr, _TRUNCATE);
	}
	return(destptr);
}

// Append source string to destination string with size limit, ensuring the resulting string is null-terminated but may be truncated.
inline wchar_t *os_wcsncat_s( wchar_t *dest, size_t destsize, const wchar_t * src )
{
	size_t curdestlen = wcslen(dest);
	if (destsize > curdestlen)
		return(os_wcsncpy_s(dest + curdestlen, src, destsize - curdestlen));
	return(dest);
}

// Append source string to destination string with size limit, ensuring the resulting string is null-terminated but may be truncated.
inline char *os_ansistrncat_s( char *dest, size_t destsize, const char * src )
{
	size_t curdestlen = strlen(dest);
	if (destsize > curdestlen) {
		strncpy_s(dest + curdestlen, destsize - curdestlen, src, _TRUNCATE);
	}
	return(dest);
}

/* sprintf() with size limit, ensuring the resulting string is null-terminated but may be truncated */
#define os_snprintf(str,size,format,...)	_sntprintf_s(str,size,_TRUNCATE,format,__VA_ARGS__)

/* sprintf() with size limit, ensuring the resulting string is null-terminated but may be truncated */
#define os_ansisnprintf(str,size,format,...)	_snprintf_s(str,size,_TRUNCATE,format,__VA_ARGS__)

#endif // ctSQL_SAFE_CRT

//#define ctSQL_SAFE_CRT_TODO // can enable this for a module that is not yet ready to use safe string copy functions

#if !defined(ctSQL_SAFE_CRT) || defined(ctSQL_SAFE_CRT_TODO) // FAIRCOM_MOD

//#ifndef ctSQL_SAFE_CRT // for testing purposes
//#error ctSQL_SAFE_CRT is not defined
//#endif

inline  dh_char_t    *os_strcpy (dh_char_t * str1, const dh_char_t * str2) 
{ 
    return (str2 ? (dh_char_t *)(_tcscpy (str1, (_TCHAR *)str2)) : (dh_char_t *)(_tcscpy(str1,(_TCHAR *)"")) );
}

inline dh_char_t *os_strncpy( dh_char_t *destptr, const dh_char_t * srcptr, dh_i32_t no_of_char )
{
    return (_tcsncpy (destptr, srcptr, no_of_char)) ;
}

#endif // !ctSQL_SAFE_CRT || ctSQL_SAFE_CRT_TODO

#endif
/* DC777 End */
