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
#include "fcexception.hpp"
#include "fclockableobj.hpp"
#include "fchashitem.hpp"

#ifdef _WINDOWS
#ifdef _DEBUG
#include <type_traits>

#pragma warning(push) 
#pragma warning(disable : 4127) 
#endif
#endif

 void *RCESAlloc( int bufSize );

/*^****************************************************************************\
*
*   Constructor: FCList
*
\******************************************************************************/
template <class T> FCList<T>::FCList()
{
#ifdef _WINDOWS
#ifdef _DEBUG
	if (IsRefControlObject && !std::is_base_of<FCRefControl, T>::value)
		throw FCException(0, NULL, "FCList::FCList", 0, FC_INT_ERR, "Class should be derived from FCRefControl to be used with this template class", __FILE__, __LINE__);
#endif
#endif

	/* Allocate the list of hash item objects */
	_capacity = LIST_BLOCK_SIZE;
	_count = 0;
	_currentPos = 0;
	_list = (T **) RCESAlloc( _capacity * sizeof(T *) );
	if( !_list )
		throw FCException(0, NULL, "FCList::FCList", 0, FC_MEM_ERR, "Couldn't allocate list array", __FILE__, __LINE__ );
	for( int i = 0; i < _capacity; i++ )
		_list[i] = NULL;

	/* Create mutex for synchronizing all the hash operations in multi-threads */
	_hListMutex = fcMutexCreate();
}
/*~****************************************************************************/

/*^****************************************************************************\
*
*   Destructor: FCList
*
\******************************************************************************/
template <class T> FCList<T>::~FCList()
{
	Clean();
	fcMutexGet( _hListMutex );
	RCESFree( _list );
	fcMutexRel( _hListMutex );
	fcMutexFree( _hListMutex );
}
/*~****************************************************************************/

/*^****************************************************************************\
*
*   Method:     Add
*               Add a new item into the unsorted list.
*
*   Parameters: value [IN]
*               Value object to be added
*
\******************************************************************************/
template <class T> void FCList<T>::Add( T *value )
{
	fcMutexGet(_hListMutex);

	/* Check if the current list is big enough or if it must be expanded */
	try
	{
		_checkCapacity();
	}
	catch( FCException &e )
	{
		fcMutexRel( _hListMutex );
		throw FCException(e);
	}

	/* Add the given object into the list */
	_list[_count++] = value;

	if (value && IsRefControlObject)
		FCRefControl::IncRef((pFCRefControl)value);

	fcMutexRel( _hListMutex );
}
/*~****************************************************************************/

/*^****************************************************************************\
*
*   Method:     Remove
*               Remove the item by the object pointer
*
*   Parameters: value [IN]
*               Object to be removed
*
*               deleteIt [IN]
*               Flag indicating that the list should delete the object form the
*               list
*
\******************************************************************************/
template <class T> void FCList<T>::Remove( T *value, bool deleteIt )
{
	fcMutexGet(_hListMutex);

	int pos = GetPos( value );
	if( pos < 0 ) {
		fcMutexRel( _hListMutex );
		throw FCException(0, NULL, "FCList::Remove", 2, FC_NOTFOUND, "Object not found", __FILE__, __LINE__ );
	}

	/* Free memory allocated for the item being removed */
	if( deleteIt && _list[pos] && IsRefControlObject)
		FCRefControl::DecRef(_list[pos]);

	/* Move the greater items to the previous position */
	for( int index = pos; index < _count; index++ )
		_list[index] = _list[index + 1];
	_count--;

	fcMutexRel( _hListMutex );
}
/*~****************************************************************************/

/*^****************************************************************************\
*
*   Method:     Remove
*               Remove the item by the object index
*
*   Parameters: index [IN]
*               Object index to be removed
*
*               deleteIt [IN]
*               Flag indicating that the list should delete the object form the
*               list
*
\******************************************************************************/
template <class T> void FCList<T>::Remove( int index, bool deleteIt )
{
	fcMutexGet(_hListMutex);

	/* Check if the given index is valid */
	if (index < 0 || index > _count) {
		fcMutexRel(_hListMutex);
		throw FCException(0, NULL, "FCList::Remove", 2, FC_NOTFOUND, "Invalid index", __FILE__, __LINE__);
	}

	/* Free memory allocated for the item being removed */
	T* value = _list[index];
	if( deleteIt && value && IsRefControlObject)
		FCRefControl::DecRef(value);

	/* Move the greater items to the previous position */
	for( int idx = index; idx < _count; idx++ )
		_list[idx] = _list[idx + 1];
	_count--;

	fcMutexRel( _hListMutex );
}
/*~****************************************************************************/

/*^****************************************************************************\
*
*   Method:     Clean
*               Remove all the items from the list
*
*   Parameters: deleteIt [IN]
*               Flag indicating that the list should delete the object form the
*               list
*
\******************************************************************************/
template <class T> void FCList<T>::Clean( bool deleteIt )
{
	/* Free the list of item objects */
	fcMutexGet(_hListMutex);
	for (int i = 0; i < _count; i++)
	{
		if( _list[i] )
		{
			T* value = _list[i];
			if( deleteIt && value && IsRefControlObject)
				FCRefControl::DecRef((pFCRefControl)value);
			value = NULL;
		}
	}
	_count = 0;
	fcMutexRel( _hListMutex );
}
/*~****************************************************************************/

/*^****************************************************************************\
*
*   Method:     Get
*               Retrieve the object by its index. Object reference counter is 
*               incremented, so it is caller responsability to decrement it when 
*               it is not in use anymore.
*
*   Parameters: index [IN]
*               Object index to be retrieved
*
*   Returns:    Object found
*
\******************************************************************************/
template <class T> T *FCList<T>::Get( int index)
{
	fcMutexGet(_hListMutex);

	/* Check if the given index is valid */
	if (index < 0 || index > _count) {
		fcMutexRel(_hListMutex);
		throw FCException(0, NULL, "FCList::Get", 2, FC_NOTFOUND, "Invalid index", __FILE__, __LINE__);
	}

	T* value = _list[index];
	if (value && IsRefControlObject)
		FCRefControl::IncRef((pFCRefControl)value);

	fcMutexRel( _hListMutex );
		
	return value;
}
/*~****************************************************************************/

/*^****************************************************************************\
*
*   Method:     First
*               Retrieve the first item. Object reference counter is 
*               incremented, so it is caller responsability to decrement it when 
*               it is not in use anymore.
*
*   Parameters: keepLocked [IN]
*               Flag indicating that the hash table should be unlocked until a 
*               ReleaseLock is called. This is for avoiding interpherence in 
*               multiple threads environment.
*
*   Returns:    First hashed item
*
\******************************************************************************/
template <class T> T *FCList<T>::First(bool keepLocked)
{
	T *retval;

	fcMutexGet(_hListMutex);

	if( _count == 0 )
	{
		if(!keepLocked)
			fcMutexRel( _hListMutex );
		return NULL;
	}

	_currentPos = 0;

	retval = _list[_currentPos];

	if (retval && IsRefControlObject)
		FCRefControl::IncRef((pFCRefControl)retval);

	if(!keepLocked) 
		fcMutexRel( _hListMutex );

	return retval;
}
/*~****************************************************************************/

/*^****************************************************************************\
*
*   Method:     Next
*               Retrieve the next item. Object reference counter is 
*               incremented, so it is caller responsability to decrement it when 
*               it is not in use anymore.
*
*   Returns:    Next hashed item
*
\******************************************************************************/
template <class T> T *FCList<T>::Next()
{
	T *retval;

	fcMutexGet(_hListMutex);

	if( _currentPos >= (_count-1) )
	{
		fcMutexRel( _hListMutex );
		return NULL;
	}

	retval = _list[++_currentPos];

	if (retval && IsRefControlObject)
		FCRefControl::IncRef((pFCRefControl)retval);

	fcMutexRel( _hListMutex );

	return retval;
}
/*~****************************************************************************/

/*^****************************************************************************\
*
*   Method:     Pull
*               Pull the first item from the list. It will return the item 
*               object in case there is at least one and remove it, otherwise 
*               return NULL. As the returned object is removed from the list, 
*               it should have its reference counter decrement, but as it will 
*               be used by the caller, it should be incremented. So, the 
*               reference counter is not changed, but it is still caller 
*               responsability to decrement it when it is not in use anymore.
*
*   Returns:    First hashed item
*
\******************************************************************************/
template <class T> T *FCList<T>::Pull()
{
	T *retval;

	fcMutexGet(_hListMutex);

	if( _count == 0 )
	{
		fcMutexRel( _hListMutex );
		return NULL;
	}

	_currentPos = 0;

	retval = _list[_currentPos];

	/* Move the greater items to the previous position */
	for( int index = 0; index < _count; index++ )
		_list[index] = _list[index + 1];
	_count--;

	fcMutexRel( _hListMutex );

	return retval;
}
/*~****************************************************************************/

/*^****************************************************************************\
*
*   Method:     _checkCapacity
*               Retrieve the next item
*
\******************************************************************************/
template <class T> void FCList<T>::_checkCapacity()
{
	/* Check if the current list is big enough for the new object */
	if( _capacity <= _count )
	{
		_capacity += LIST_BLOCK_SIZE;
		T **_newList = (T **) RCESAlloc( _capacity * sizeof(T *) );
		if( !_newList )
			throw FCException(0, NULL, "FCList::_checkCapacity", 0, FC_MEM_ERR, "Not able to allocate hash list", __FILE__, __LINE__ );

		for( int i = 0; i < _count; i++ )
			_newList[i] = _list[i];
		RCESFree( _list );
		_list = _newList;
	}
}
/*~****************************************************************************/

/*^****************************************************************************\
*
*   Method:     GetPos
*               Retrieve the position for the given object
*
*   Parameters: value [IN]
*               Key to be added
*
*   Returns:    Position for the given object or -1 if it's not found
*
\******************************************************************************/
template <class T> int FCList<T>::GetPos( T *value )
{
	int retval = -1;

	for( int i = 0; i < _count && retval < 0; i++ )
	{
		if( _list[i] == value )
			retval = i;
	}

	return retval;
}
/*~****************************************************************************/

/*^****************************************************************************\
*
*   Method:     ReleaseLock
*               Release lock acquired for a First() or FirstK() with keep lock
*
\******************************************************************************/
template <class T> void FCList<T>::ReleaseLock()
{
	fcMutexRel( _hListMutex );
}
/*~****************************************************************************/

/*^****************************************************************************\
*
*   Method:     AcquireLock
*               Acquire lock
*
\******************************************************************************/
template <class T> void FCList<T>::AcquireLock()
{
	fcMutexGet(_hListMutex);
}
/*~****************************************************************************/