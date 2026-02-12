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

 #include "fcpool.hpp"
 #include "fcexception.hpp"

void *RCESAlloc( int bufSize );
void RCESFree( void *buffer );

/*^****************************************************************************\
*
*   Constructor: FCHashIntTable
*
*   Parameters: size [IN]
*               Hash capacity size
*
*               deleteItem [IN]
*               Flag indicating if the hashed object should be deleted or not
*
\******************************************************************************/
template <class T> FCHashIntTable<T>::FCHashIntTable( int size, bool deleteItem )
{
	/* Allocate the list of hash item objects */
	_capacity = size;
	_count = 0;
	_currentPos = 0;
	_list = (FCHashIntItem<T> **) RCESAlloc( _capacity * sizeof( FCHashIntItem<T> * ) );
	if( !_list )
		throw FCException(0, NULL, "FCHashIntTable::FCHashIntTable", 0, FC_MEM_ERR, "Couldn't allocate list array", __FILE__, __LINE__ );
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
*   Destructor: FCHashIntTable
*
\******************************************************************************/
template <class T> FCHashIntTable<T>::~FCHashIntTable()
{
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
template <class T> void FCHashIntTable<T>::Add( int key, T *value )
{
	int location;
	bool moreToSearch;
	bool isLocked = _lockOwner && _lockOwner == fcGetCurrentThreadId();

	if (!isLocked)
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
	FCHashIntItem<T> *newItem = new FCHashIntItem<T>( key, value, _deleteItem);

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
				FCHashIntItem<T>* itemToRemove = _list[location - 1];
				_list[ location-1 ] = newItem;
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
template <class T> void FCHashIntTable<T>::Remove(int key, bool deleteIt)
{
	int pos;
	bool isLocked = _lockOwner && _lockOwner == fcGetCurrentThreadId();

	if (!isLocked)
		fcRWLockGetWrite(&_wrlock);

	if( (pos = _getPos(key)) < 0 ) {
		if (!isLocked)
			fcRWLockRelWrite(&_wrlock);
		throw FCException(0, NULL, "FCHashIntTable::Remove", 2, FC_NOTFOUND, "Key not found", __FILE__, __LINE__ );
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
template <class T> void FCHashIntTable<T>::Clean()
{
	bool isLocked = _lockOwner && _lockOwner == fcGetCurrentThreadId();

	/* Free the list of hash item objects */
	if (!isLocked)
		fcRWLockGetWrite(&_wrlock);
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
template <class T> T *FCHashIntTable<T>::Get(int key )
{
	bool isLocked = _lockOwner && _lockOwner == fcGetCurrentThreadId();

	if (!isLocked)
		fcRWLockGetRead(&_wrlock);

	int pos = _getPos( key );

	if (!isLocked)
		fcRWLockRelRead(&_wrlock);

	if (pos >= 0) {
		if (_list[pos]) {
			T *retval = (T *)_list[pos]->GetValue();
			_list[pos]->IncRef();
			return retval;
		}
		else
			return NULL;
		}
	else
		return NULL;
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
template <class T> T* FCHashIntTable<T>::GetByPos(int pos)
{
	if (pos >= 0) {
		T* retval = (T*)_list[pos]->GetValue();
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
*   Parameters: keepLocked [IN]
*               Flag indicating that the hash table should be unlocked until a 
*               ReleaseLock is called. This is for avoiding interpherence in 
*               multiple threads environment.
*
*   Returns:    First hashed item
*
\******************************************************************************/
template <class T> T *FCHashIntTable<T>::First(bool keepLocked)
{
	void *retval;
	bool wasLocked = _lockOwner && _lockOwner == fcGetCurrentThreadId();

	if (!wasLocked) {
		if (keepLocked) {
			fcRWLockGetWrite(&_wrlock);
			_lockOwner = fcGetCurrentThreadId();
		}
		else
			fcRWLockGetRead(&_wrlock);
	}

	if (_count == 0) {
		if (!keepLocked && !wasLocked)
			fcRWLockRelRead(&_wrlock);
		return NULL;
	}

	_currentPos = 0;

	retval = _list[_currentPos]->GetValue();
	_list[_currentPos]->IncRef();

	if(!keepLocked && !wasLocked)
		fcRWLockRelRead(&_wrlock);

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
template <class T> T *FCHashIntTable<T>::Next()
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
template <class T> T *FCHashIntTable<T>::Get()
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
*   Parameters: keepLocked [IN]
*               Flag indicating that the hash table should be unlocked until a 
*               ReleaseLock is called. This is for avoiding interpherence in 
*               multiple threads environment.
*
*   Returns:    First key or -1 in case of empty list
*
\******************************************************************************/
template <class T> int FCHashIntTable<T>::FirstK(bool keepLocked)
{
	int retval = -1;
	bool waslocked = _lockOwner && _lockOwner == fcGetCurrentThreadId();

	if (!waslocked) {
		if (keepLocked) {
			fcRWLockGetWrite(&_wrlock);
			_lockOwner = fcGetCurrentThreadId();
		}
		else
			fcRWLockGetRead(&_wrlock);
	}

	if (_count == 0) {
		if (!keepLocked && !waslocked)
			fcRWLockRelRead(&_wrlock);
		return -1;
	}

	_currentPos = 0;

	retval = _list[_currentPos]->GetItemKey();

	if(!keepLocked && !waslocked)
		fcRWLockRelRead(&_wrlock);

	return retval;
}
/*~****************************************************************************/

/*^****************************************************************************\
*
*   Method:     NextK
*               Retrieve the next key
*
*   Returns:    Next hashed key or -1 in case of error
*
\******************************************************************************/
template <class T> int FCHashIntTable<T>::NextK()
{
	int retval = -1;
	bool isLocked = _lockOwner && _lockOwner == fcGetCurrentThreadId();

	if (!isLocked)
		fcRWLockGetRead(&_wrlock);

	if( _currentPos >= (_count-1) )
	{
		if (!isLocked)
			fcRWLockRelRead(&_wrlock);
		return -1;
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
*   Returns:    Current hashed key or -1 in case of error
*
\******************************************************************************/
template <class T> int FCHashIntTable<T>::GetK()
{
	int retval = -1;
	bool isLocked = _lockOwner && _lockOwner == fcGetCurrentThreadId();

	if (!isLocked)
		fcRWLockGetRead(&_wrlock);

	if( _currentPos >= _count )
	{
		if (!isLocked)
			fcRWLockRelRead(&_wrlock);
		return -1;
	}

	retval = _list[_currentPos]->GetItemKey();

	if (!isLocked)
		fcRWLockRelRead(&_wrlock);

	return retval;
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
template <class T> bool FCHashIntTable<T>::KeyExists( int key )
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
*   Method:     _checkCapacity
*               Retrieve the next item
*
\******************************************************************************/
template <class T> void FCHashIntTable<T>::_checkCapacity()
{
	/* Check if the current list is big enough for the new object */
	if( _capacity <= _count )
	{
		_capacity += LIST_BLOCK_SIZE;
		FCHashIntItem<T> **_newList = (FCHashIntItem<T> **) RCESAlloc( _capacity * sizeof( FCHashIntItem<T> * ) );
		if( !_newList )
			throw FCException(0, NULL, "FCHashTable::_checkCapacity", 0, FC_MEM_ERR, "Not able to allocate hash list",  __FILE__, __LINE__ );

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
template <class T> int FCHashIntTable<T>::_getPos( int key )
{
	int midPoint = -1;
	int first = 0;
	int last = _count - 1;
	bool moreToSearch = first <= last;
	bool found = false;

	/* Execute a binary search for by the key */
	while( moreToSearch && !found ) 
	{
		midPoint = ( first + last) / 2;
		if (key < _list[midPoint]->GetItemKey()) {
			last = midPoint - 1;
			moreToSearch = first <= last;
		}
		else if (key > _list[midPoint]->GetItemKey()) {
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
*   Method:     Lock
*               Lock the hash table
*
\******************************************************************************/
template <class T> void FCHashIntTable<T>::Lock()
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
template <class T> void FCHashIntTable<T>::ReleaseLock()
{
	_lockOwner = 0;
	fcRWLockRelWrite(&_wrlock);
}
/*~****************************************************************************/