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
 *      Copyright (c) 2013 - 2022 FairCom Corporation.
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

#include "fcaux.hpp"
#include "fcexception.hpp"
#include "fclockableobj.hpp"

#ifdef _WINDOWS
#ifdef _DEBUG
#include <type_traits>

#pragma warning(push) 
#pragma warning(disable : 4127) 
#endif
#endif

 void RCESFree( void *buffer );

/*^****************************************************************************\
*
*   Constructor: FCHashItem
*
*   Parameters: key [IN]
*               Key value
*
*               value [IN]
*               Value content
*
*               deleteItem [IN]
*               Flag indicating if the hashed object should be deleted or not
*
\******************************************************************************/
template <class T> FCHashItem<T>::FCHashItem( const char *key, T *value, bool deleteItem )
{
	_key = dupStr( key );
	_value = value;
	_deleteItem = deleteItem;

#ifdef _WINDOWS
#ifdef _DEBUG
#pragma warning(disable : 4127)
	if (IsRefControlObject && !std::is_base_of<FCRefControl, T>::value)
		throw FCException(0, NULL, "FCHashItem::FCHashItem", 0, FC_INT_ERR, "Class should be derived from FCRefControl to be used with this template class", __FILE__, __LINE__);
#endif
#endif

	/* Increment the object's reference */
	if (value && IsRefControlObject)
		((pFCRefControl)value)->IncRef();
}
/*~****************************************************************************/

/*^****************************************************************************\
*
*   Destructor: FCHashItem
*
\******************************************************************************/
template <class T> FCHashItem<T>::~FCHashItem()
{
	if( _key )
		RCESFree( _key );

	if (_deleteItem && _value) {
		if (!IsRefControlObject)
			RCESFree(_value);
		else
			FCRefControl::DecRef((pFCRefControl)_value); /* Decrement object's reference, in case it is the last one, delete it */
	}
}
/*~****************************************************************************/

/*^****************************************************************************\
*
*   Method:     IncRef
*               Increment hashed object reference count
*
\******************************************************************************/
template <class T> void FCHashItem<T>::IncRef()
{
	if (_value && IsRefControlObject)
		((pFCRefControl)_value)->IncRef();
}
/*~****************************************************************************/

/*^****************************************************************************\
*
*   Method:     DecRef
*               Decrement hashed object reference count
*
\******************************************************************************/
template <class T> void FCHashItem<T>::DecRef()
{
	if (_value && IsRefControlObject)
		FCRefControl::DecRef((pFCRefControl)_value);
}
/*~****************************************************************************/

/*^****************************************************************************\
*
*   Method:     ResetRefCount
*               Reset reference counter
*
\******************************************************************************/
template <class T> void FCHashItem<T>::ResetRefCount()
{
	if (_value && IsRefControlObject)
		((pFCRefControl)_value)->ResetRefCount();
}
/*~****************************************************************************/

/*^****************************************************************************\
*
*   Method:     ComparedTo
*               Compare current and given items.
*
*   Parameters: otherItem [IN]
*               Item to be compared with the current
*
*   Returns:    LESS, GREATER or EQUAL
*
\******************************************************************************/
template <class T> RelationType FCHashItem<T>::ComparedTo( FCHashItem<T> *otherItem ) const
{
	int retval = strcmp( _key, otherItem->GetItemKey() );
	if( retval < 0 )
		return LESS;
	else if( retval > 0 )
		return GREATER;
	else 
		return EQUAL;
}
/*~****************************************************************************/