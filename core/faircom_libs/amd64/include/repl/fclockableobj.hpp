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
 *      Copyright (c) 2013 - 2024 FairCom Corporation.
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

#ifndef _FCLOCKABLEOBJ_HPP_
#define _FCLOCKABLEOBJ_HPP_

#include "fcaux.hpp"
#include <assert.h>

/* Class type definitions */
class FCRefControl;
typedef FCRefControl *pFCRefControl, **ppFCRefControl;

/*^****************************************************************************\
 *
 *   FCRefControl
 *   Class for managing object reference counting and automatic deletion
 *
\******************************************************************************/
class FCRefControl
{
	public:
		/* Constructors & destructor */
		FCRefControl();
		virtual ~FCRefControl();

		/* Static Methods */
		static void DecRef(pFCRefControl obj);
		static int GetRef(pFCRefControl obj);
		static void IncRef(pFCRefControl obj);

		/* Public methods */
		void IncRef();
		bool DecRef(bool zerovalid = false);
		int GetRef();
		bool IsActive();
		void ResetRefCount();

	protected:
		void SetRefInactive();

	private:
		/* Private Properties */
		int _refCount;
		void *_mutex;
		bool _active;

		/* Private Methods */
};
/*~****************************************************************************/

/*^****************************************************************************\
 *
 *   FCRWLocker
 *   Helper class for automatically lock/unlock objects throw Read/Write lock
 *
\******************************************************************************/
class FCRWLocker
{
public:
	/* Constructors & destructor */
	FCRWLocker(fcWRLock *wrLock) { _locked = true; _wrLock = wrLock; fcRWLockGetRead(wrLock); };
	virtual ~FCRWLocker() { if (_locked) fcRWLockRelRead(_wrLock); };

	/* Public methods */
	void Unlock() { if (_locked) fcRWLockRelRead(_wrLock); _locked = false; };

private:
	/* Private Properties */
	bool _locked;
	void *_wrLock;

	/* Private Methods */
};
/*~****************************************************************************/

#endif /* _FCLOCKABLEOBJ_HPP_ */