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

void *RCESAlloc( int bufSize );

/*^****************************************************************************\
*
*   Constructor: FCHashTable
*
*   Parameters: size [IN]
*               Hash capacity size
*
*               deleteItem [IN]
*               Flag indicating if the hashed object should be deleted or not
*
\******************************************************************************/
template <class T> FCHashTable<T>::FCHashTable( int size, bool deleteItem )
{
	/* Allocate the list of hash item objects */
	_capacity = size;
	_count = 0;
	_currentPos = 0;
	_list = (FCHashItem<T> **) RCESAlloc( _capacity * sizeof( FCHashItem<T> * ) );
	if( !_list )
		throw FCException(0, NULL, "FCHashTable::FCHashTable", 0, FC_MEM_ERR, "Couldn't allocate list array", __FILE__, __LINE__);
	for( int i = 0; i < _capacity; i++ )
		_list[i] = NULL;

	/* Create mutex for synchronizing all the hash operations in multi-threads */
	fcRWLockCreate(&_wrlock);

	_deleteItem = deleteItem;
	_lockOwner = 0;
}
/*~****************************************************************************/

/*^****************************************************************************\
*
*   Destructor: FCHashTable
*
\******************************************************************************/
template <class T> FCHashTable<T>::~FCHashTable()
{
	SetRefInactive(); /* Signalize the reference counter that we already started the destruction */

	Clean();
	fcRWLockGetWrite(&_wrlock);
	RCESFree(_list);
	fcRWLockRelWrite(&_wrlock);
	fcRWLockFree(&_wrlock);
}
/*~****************************************************************************/

/*^****************************************************************************\
*
*   Method:     Add
*               Add a new item into the sorted list. If key already exists, 
*               replace it.
*
*   Parameters: key [IN]
*               Key to be added
*
*               value [IN]
*               Value object to be added
*
\******************************************************************************/
template <class T> void FCHashTable<T>::Add( const char *key, T *value )
{
	int location;
	bool moreToSearch;
	bool isLocked = _lockOwner && _lockOwner == fcGetCurrentThreadId();

	if(!isLocked)
		fcRWLockGetWrite(&_wrlock);

	location = _count;
	moreToSearch = (location > 0);

	/* Check if the current list is big enough or if it must be expanded */
	try
	{
		_checkCapacity();
	}
	catch( FCException &e )
	{
		if (!isLocked)
			fcRWLockRelWrite(&_wrlock);
		throw FCException(e);
	}

	/* Create a new hash item with the given information */
	FCHashItem<T> *newItem = new FCHashItem<T>( key, value, _deleteItem );

	/* Find the position by looking from the end to the beggining */
	while( moreToSearch ) 
	{
		switch( newItem->ComparedTo( _list[ location-1 ] ) ) 
		{
			case LESS    : 
				location--;
				moreToSearch = ( location > 0 );
				break;
			case GREATER : 
				moreToSearch = false;
				break;
			case EQUAL :
				/* If key alredy exists, update its item content */
				FCHashItem<T>* itemToRemove = _list[location - 1];
				_list[location - 1] = newItem;
				if (!isLocked)
					fcRWLockRelWrite(&_wrlock);
				/* Now that we have released the hash list write lock, decrement the reference count on the item we are removing from the list */
				delete itemToRemove;
				return;
		}
	}

	/* Move the greater items to one position a head */
	for( int index = _count; index > location; index-- )
		_list[index] = _list[index - 1];

	/* Set new item into the position */
	_list[location] = newItem;
	_count++;

	if (!isLocked)
		fcRWLockRelWrite(&_wrlock);
}
/*~****************************************************************************/

/*^****************************************************************************\
*
*   Method:     Remove
*               Remove the item by the key
*
*   Parameters: key [IN]
*               Key to be deleted
*
*               deleteIt [IN]
*               Flag indicating that the list should delete the object form the
*               list
*
\******************************************************************************/
template <class T> void FCHashTable<T>::Remove(const char *key, bool deleteIt)
{
	int pos;
	bool isLocked = _lockOwner && _lockOwner == fcGetCurrentThreadId();

	if (!isLocked)
		fcRWLockGetWrite(&_wrlock);

	if( (pos = _getPos(key)) < 0 )
	{
		if (!isLocked)
			fcRWLockRelWrite(&_wrlock);
		throw FCException(0, NULL, "FCHashTable::Remove", 2, FC_NOTFOUND, "Key not found", __FILE__, __LINE__ );
	}

	/* Free memory allocated for the item being removed */
	if (_list[pos]) {
		_list[pos]->SetDeleteItem(deleteIt);
		delete _list[pos];
	}

	/* Move the greater items to the previous position */
	for( int index = pos; index < _count; index++ )
		_list[index] = _list[index + 1];
	_count--;

	if( _currentPos >= _count )
		_currentPos = _count-1;
	if(_currentPos < 0)
		_currentPos = 0;

	if (!isLocked)
		fcRWLockRelWrite(&_wrlock);
}
/*~****************************************************************************/

/*^****************************************************************************\
*
*   Method:     Clean
*               Remove all the items from the list
*
\******************************************************************************/
template <class T> void FCHashTable<T>::Clean()
{
	bool isLocked = _lockOwner && _lockOwner == fcGetCurrentThreadId();

	/* Free the list of hash item objects */
	if (!isLocked) {
		fcRWLockGetWrite(&_wrlock);
		const int listCount = _count;
		if (listCount) {
			/* To avoid a deadlock that can occur if we free the list entries while
			holding the list mutex, we allocate a list to hold the entries and free
			the entries in that list after we release the list mutex. */
			const int usedListSize = listCount * sizeof(FCHashItem<T> *);
			FCHashItem<T>** tempList = (FCHashItem<T> **)RCESAlloc(usedListSize);
			if (tempList) {
				/* copy the entries to our newly allocated list */
				memcpy(tempList, _list, usedListSize);

				/* clear the entries in the original list */
				memset(_list, 0, usedListSize);
				_currentPos = _capacity = _count = 0;

				/* unlock the list */
				fcRWLockRelWrite(&_wrlock);

				/* free the entries in the copy of the list */
				for (int i = 0; i < listCount; i++) {
					if (tempList[i])
						delete tempList[i];
				}
				RCESFree(tempList);
				return;
			}
			/* We could not allocate memory for a copy of the list, so execute the code below that deletes
			the entries from the original list while holding the write lock. */
		}
		else {
			/* no entries in list */
			_currentPos = _capacity = 0;
			fcRWLockRelWrite(&_wrlock);
			return;
		}
	}

	for (int i = 0; i < _count; i++)
	{
		if( _list[i] )
		{
			delete _list[i];
			_list[i] = NULL;
		}
	}
	_currentPos = _capacity = _count = 0;
	if (!isLocked)
		fcRWLockRelWrite(&_wrlock);
}
/*~****************************************************************************/

/*^****************************************************************************\
*
*   Method:     Get
*               Retrieve the hashed value for the given key. The returned object 
*               have the reference automatically increased, it is the caller 
*               responsibility to decrement it when its usage is done.
*
*   Parameters: key [IN]
*               Key to be retrieved
*
*   Returns:    Hashed value for the given key, or NULL if it is not found
*
\******************************************************************************/
template <class T> T *FCHashTable<T>::Get( const char *key )
{
	bool isLocked = _lockOwner && _lockOwner == fcGetCurrentThreadId();
	T* retval = NULL;

	if (!key)
		return NULL;

	if (!isLocked)
		fcRWLockGetRead(&_wrlock);

	int pos = _getPos( key );

	if (pos >= 0) {
		if (_list[pos]) {
			retval = (T *)_list[pos]->GetValue();
			_list[pos]->IncRef();
		}
	}

	if (!isLocked)
		fcRWLockRelRead(&_wrlock);

	return retval;
}
/*~****************************************************************************/

/*^****************************************************************************\
*
*   Method:     GetByPos
*               Retrieve the hashed value by given position. The returned object 
*               have the reference automatically increased, it is the caller 
*               responsibility to decrement it when its usage is done.
*
*   Parameters: pos [IN]
*               Position to be retrieved
*
*   Returns:    Hashed value for the given position, or NULL if it is not found
*
\******************************************************************************/
template <class T> T *FCHashTable<T>::GetByPos(int pos)
{
	if (pos >= 0) {
		T *retval = (T *)_list[pos]->GetValue();
		_list[pos]->IncRef();
		return retval;
	}
	else
		return NULL;
}
/*~****************************************************************************/

/*^****************************************************************************\
*
*   Method:     First
*               Retrieve the first item. The returned object have the reference 
*               automatically increased, it is the caller responsibility to 
*               decrement it when its usage is done.
*
*   Returns:    First hashed item
*
\******************************************************************************/
template <class T> T *FCHashTable<T>::First()
{
	void *retval;
	bool wasLocked = _lockOwner && _lockOwner == fcGetCurrentThreadId();

	if (!wasLocked) {
		fcRWLockGetWrite(&_wrlock);
		_lockOwner = fcGetCurrentThreadId();
	}

	if (_count == 0)
		return NULL;

	_currentPos = 0;

	retval = _list[_currentPos]->GetValue();
	_list[_currentPos]->IncRef();

	return (T *)retval;
}
/*~****************************************************************************/

/*^****************************************************************************\
*
*   Method:     Next
*               Retrieve the next item. The returned object have the reference 
*               automatically increased, it is the caller responsibility to 
*               decrement it when its usage is done.
*
*   Returns:    Next hashed item
*
\******************************************************************************/
template <class T> T *FCHashTable<T>::Next()
{
	void *retval;
	bool isLocked = _lockOwner && _lockOwner == fcGetCurrentThreadId();

	if (!isLocked)
		fcRWLockGetRead(&_wrlock);

	if( _currentPos >= (_count-1) )
	{
		if (!isLocked)
			fcRWLockRelRead(&_wrlock);
		return NULL;
	}

	retval = _list[++_currentPos]->GetValue();
	_list[_currentPos]->IncRef();

	if (!isLocked)
		fcRWLockRelRead(&_wrlock);

	return (T *)retval;
}
/*~****************************************************************************/

/*^****************************************************************************\
*
*   Method:     Get
*               Retrieve the current hashed item. The returned object have the 
*               reference automatically increased, it is the caller 
*               responsibility to decrement it when its usage is done.
*
*   Returns:    Current hashed item
*
\******************************************************************************/
template <class T> T *FCHashTable<T>::Get()
{
	void *retval;
	bool isLocked = _lockOwner && _lockOwner == fcGetCurrentThreadId();

	if (!isLocked)
		fcRWLockGetRead(&_wrlock);

	if( _currentPos >= _count )
	{
		if (!isLocked)
			fcRWLockRelRead(&_wrlock);
		return NULL;
	}

	retval = _list[_currentPos]->GetValue();
	_list[_currentPos]->IncRef();

	if (!isLocked)
		fcRWLockRelRead(&_wrlock);

	return (T *)retval;
}
/*~****************************************************************************/

/*^****************************************************************************\
*
*   Method:     FirstK
*               Retrieve the first key
*
*   Returns:    First key
*
\******************************************************************************/
template <class T> const char *FCHashTable<T>::FirstK()
{
	bool waslocked = _lockOwner && _lockOwner == fcGetCurrentThreadId();

	if (!waslocked) {
		fcRWLockGetWrite(&_wrlock);
		_lockOwner = fcGetCurrentThreadId();
	}

	if (_count == 0)
		return NULL;

	_currentPos = 0;

	return _list[_currentPos]->GetItemKey();
}
/*~****************************************************************************/

/*^****************************************************************************\
*
*   Method:     NextK
*               Retrieve the next key
*
*   Returns:    Next hashed key
*
\******************************************************************************/
template <class T> const char *FCHashTable<T>::NextK()
{
	const char *retval;
	bool isLocked = _lockOwner && _lockOwner == fcGetCurrentThreadId();

	if (!isLocked)
		fcRWLockGetRead(&_wrlock);

	if( _currentPos >= (_count-1) )
	{
		if (!isLocked)
			fcRWLockRelRead(&_wrlock);
		return NULL;
	}

	retval = _list[++_currentPos]->GetItemKey();

	if (!isLocked)
		fcRWLockRelRead(&_wrlock);

	return retval;
}
/*~****************************************************************************/

/*^****************************************************************************\
*
*   Method:     GetK
*               Retrieve the current key value
*
*   Returns:    Current hashed key
*
\******************************************************************************/
template <class T> const char *FCHashTable<T>::GetK()
{
	const char *retval;
	bool isLocked = _lockOwner && _lockOwner == fcGetCurrentThreadId();

	if (!isLocked)
		fcRWLockGetRead(&_wrlock);

	if( _currentPos >= _count )
	{
		if (!isLocked)
			fcRWLockRelRead(&_wrlock);
		return NULL;
	}

	retval = _list[_currentPos]->GetItemKey();

	if (!isLocked)
		fcRWLockRelRead(&_wrlock);

	return retval;
}
/*~****************************************************************************/

/*^****************************************************************************\
*
*   Method:     GetKByPos
*               Retrieve the hashed key by given position
*
*   Parameters: pos [IN]
*               Position to be retrieved
*
*   Returns:    Hashed key for the given position, or NULL if it is not found
*
\******************************************************************************/
template <class T> const char *FCHashTable<T>::GetKByPos(int pos)
{
	if (pos >= 0)
		return _list[pos]->GetItemKey();
	else
		return NULL;
}
/*~****************************************************************************/

/*^****************************************************************************\
*
*   Method:     KeyExists
*               Check if the given key exists in the list
*
*   Parameters: key [IN]
*               Key to be added
*
*   Returns:    true if the key exists, otherwise returns false
*
\******************************************************************************/
template <class T> bool FCHashTable<T>::KeyExists( const char *key )
{
	bool retval;
	bool isLocked = _lockOwner && _lockOwner == fcGetCurrentThreadId();

	if (!isLocked)
		fcRWLockGetRead(&_wrlock);

	retval = _getPos( key ) >= 0;

	if (!isLocked)
		fcRWLockRelRead(&_wrlock);

	return retval;
}
/*~****************************************************************************/

/*^****************************************************************************\
*
*   Method:     GetCount
*               Retrieve the number of items in the Hash Table
*
*   Returns:    Item count
*
\******************************************************************************/
template <class T> int FCHashTable<T>::GetCount()
{
	int retval = 0;
	bool isLocked = _lockOwner && _lockOwner == fcGetCurrentThreadId();

	if (!isLocked)
		fcRWLockGetRead(&_wrlock);

	retval = _count;

	if (!isLocked)
		fcRWLockRelRead(&_wrlock);

	return retval;
}
/*~****************************************************************************/

/*^****************************************************************************\
*
*   Method:     _checkCapacity
*               Retrieve the next item
*
\******************************************************************************/
template <class T> void FCHashTable<T>::_checkCapacity()
{
	/* Check if the current list is big enough for the new object */
	if( _capacity <= _count )
	{
		_capacity += LIST_BLOCK_SIZE;
		FCHashItem<T> **_newList = (FCHashItem<T> **) RCESAlloc( _capacity * sizeof( FCHashItem<T> * ) );
		if( !_newList )
			throw FCException(0, NULL, "FCHashTable::_checkCapacity", 0, FC_MEM_ERR, "Not able to allocate hash list", __FILE__, __LINE__ );

		for( int i = 0; i < _count; i++ )
			_newList[i] = _list[i];
		RCESFree( _list );
		_list = _newList;
	}
}
/*~****************************************************************************/

/*^****************************************************************************\
*
*   Method:     _getPos
*               Retrieve the position for the given key
*
*   Parameters: key [IN]
*               Key to be added
*
*   Returns:    Position for the given key or -1 if it's not found
*
\******************************************************************************/
template <class T> int FCHashTable<T>::_getPos( const char *key )
{
	int retval;
	int midPoint = -1;
	int first = 0;
	int last = _count - 1;
	bool moreToSearch = first <= last;
	bool found = false;

	/* Execute a binary search for by the key */
	while( moreToSearch && !found ) 
	{
		midPoint = ( first + last) / 2;
		retval = strcmp(key, _list[midPoint]->GetItemKey());
		if (retval < 0) {
			last = midPoint - 1;
			moreToSearch = first <= last;
		}
		else if (retval > 0) {
			first = midPoint + 1;
			moreToSearch = first <= last;
		}
		else
			found = true;
	}

	if( found )
		return midPoint;
	else
		return -1;
}
/*~****************************************************************************/

/*^****************************************************************************\
*
*   Method:     GetPos
*               Retrieve position of a given key
*
*   Returns:    Item position
*
\******************************************************************************/
template <class T> int FCHashTable<T>::GetPos(const char *key)
{
	int retval;
	bool isLocked = _lockOwner && _lockOwner == fcGetCurrentThreadId();

	if (!isLocked)
		fcRWLockGetRead(&_wrlock);

	retval = _getPos(key);

	if (!isLocked)
		fcRWLockRelRead(&_wrlock);

	return retval;
}
/*~****************************************************************************/

/*^****************************************************************************\
*
*   Method:     Lock
*               Lock the hash table
*
\******************************************************************************/
template <class T> void FCHashTable<T>::Lock()
{
	fcRWLockGetWrite(&_wrlock);
	_lockOwner = fcGetCurrentThreadId();
}
/*~****************************************************************************/

/*^****************************************************************************\
*
*   Method:     ReleaseLock
*               Release lock acquired for a First() or FirstK() with keep lock
*
\******************************************************************************/
template <class T> void FCHashTable<T>::ReleaseLock()
{
	_lockOwner = 0;
	fcRWLockRelWrite(&_wrlock);
}
/*~****************************************************************************/

/*^****************************************************************************\
*
*   Method:     ResetRefCount
*               Reset the reference counter for all the lements in the list to 1
*
\******************************************************************************/
template <class T> void FCHashTable<T>::ResetRefCount()
{
	bool isLocked = _lockOwner && _lockOwner == fcGetCurrentThreadId();

	if (!isLocked)
		fcRWLockGetRead(&_wrlock);

	for (int i = 0; i < _count; i++)
		_list[i]->ResetRefCount();

	if (!isLocked)
		fcRWLockRelRead(&_wrlock);
}
/*~****************************************************************************/