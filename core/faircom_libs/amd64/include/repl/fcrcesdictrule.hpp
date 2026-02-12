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

#ifndef _FCRCESDICTRULE_HPP_
#define _FCRCESDICTRULE_HPP_

#include "fc_rces.h"
#include "fcaux.hpp"
#include "fcrcesaux.hpp"
#include "fcrcespublicationrule.hpp"
#include "fcrcesrule.hpp"

extern "C" {
#include "fcrcesjson.h"
}

#define RCES_DICTRULE_LIST "dictionaryRuleList"
#define RCES_DICTRULE_DATABASE "database"
#define RCES_DICTRULE_DATABASENAME "databaseName"
#define RCES_DICTRULE_FILENAME "dictionaryFileName"

/*^****************************************************************************\
*
*   RCESDictionaryRule
*   Recon-e-Sense publication dictionary rule class
*
\******************************************************************************/
class RCESDictionaryRule : public RCESRule
{
public:
	/* Constructors & destructor */
	RCESDictionaryRule();
	RCESDictionaryRule(pRCESDictionaryRule dictRule);
	RCESDictionaryRule(pRCESJson dictRule);
	virtual ~RCESDictionaryRule();

	/* Public services */
	static int ctGetDictionaryRules(pRCESMasterProvider masterProv, int publicationID, ppRCESDictionaryRule dictRuleList);
	static int ctGetDictionaryRules(pRCESMasterProvider masterConn, int publicationID, ppRCESJson result);
	static int ctPersistDictionaryRule(pRCESMasterProvider masterProv, int publicationID, pRCESDictionaryRule dictRuleList);
	static int ctGetPublicationFiles(pRCESMasterProvider masterProv, int publicationID, ppRCESFile dictFile);

	/* Public methods */
	int GetID() { return _id; };
	pRCESDatabase GetDatabase() { return _database; };
	pRCESFile GetDictFile() { return _dictFile; };

	void SetID(int databaseID) { _id = databaseID; };
	void SetDatabase(pRCESDatabase database) { _database = database; };
	void SetDictFile(pRCESFile dictFile) { _dictFile = dictFile; };

	pRCESJson GetJson();
	bool MatchesTarget(pRCESMasterProvider masterProv, pRCESDictionaryRule databaseRule, pRCESSubscription subscription);

private:
	/* Private properties */
	int _id;
	pRCESDatabase _database;
	pRCESFile _dictFile;

	/* Private methods */
	void _init();
};
/*~****************************************************************************/

/* Class type definitions */
typedef RCESDictionaryRule *pRCESDictionaryRule, **ppRCESDictionaryRule;

#endif /* _FCRCESDICTRULE_HPP_ */