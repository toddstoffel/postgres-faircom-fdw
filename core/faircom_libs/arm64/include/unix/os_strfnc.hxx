
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
#ifdef FAIRCOM_MOD
#if (defined _UNICODE) || (defined DH_WIDECHAR)
#include <wchar.h>
#endif
#else
#include "wchar.h"
#endif

inline dh_char_t    *os_strcpy (dh_char_t *str1, const dh_char_t *str2)
{
    #if (defined _UNICODE) || (defined DH_WIDECHAR)
        return (str2 ? wcscpy (str1, str2) : (dh_char_t *) 0);
    #else
        return (str2 ? strcpy (str1, str2) : (dh_char_t *) 0);
    #endif
}

inline dh_char_t    *os_strncpy (dh_char_t *str1, const dh_char_t *str2, dh_u32_t n)
{
    #if (defined _UNICODE) || (defined DH_WIDECHAR)
        return wcsncpy (str1, str2, (unsigned int) n);
    #else
        return strncpy (str1, str2, (unsigned int) n);
    #endif
}
#endif
/* DC777 End */
