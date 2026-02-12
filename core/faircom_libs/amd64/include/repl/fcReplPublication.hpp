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

#ifndef _FCREPLPUBLICATION_HPP_
#define _FCREPLPUBLICATION_HPP_

#include "fcReplCommon.hpp"
#include "fcReplConn.hpp"
#include "fcReplFile.hpp"
#include "fcrcespublication.hpp"
#include "fcrcesdictrule.hpp"
#include "fcrcesfilerule.hpp"
#include "fcrcesfolderrule.hpp"

/* Forward class declaration */
class FCREPLPublication;
class FCREPLRule;
class FCREPLDictionaryRule;
class FCREPLFileRule;
class FCREPLFolderRule;
class FCREPLPublicationRule;
typedef FCREPLPublication *pFCREPLPublication, **ppFCREPLPublication;
typedef FCREPLRule *pFCREPLRule, **ppFCREPLRule;
typedef FCREPLDictionaryRule *pFCREPLDictionaryRule, **ppFCREPLDictionaryRule;
typedef FCREPLFileRule *pFCREPLFileRule, **ppFCREPLFileRule;
typedef FCREPLFolderRule *pFCREPLFolderRule, **ppFCREPLFolderRule;
typedef FCREPLPublicationRule *pFCREPLPublicationRule, **ppFCREPLPublicationRule;

/* Publication mode */
typedef enum
{
	FCREPL_PUBLICATION_MODE_UNDEF = -1,
	FCREPL_PUBLICATION_MODE_FILE = 0,
	FCREPL_PUBLICATION_MODE_FOLDER = 1,
	FCREPL_PUBLICATION_MODE_DATABASE = 2,
	FCREPL_PUBLICATION_MODE_DICTIONARY = 3,
	FCREPL_PUBLICATION_MODE_DICT_FOLDER = 4
} FCREPLPublicationMode;

/* Rule type */
typedef enum
{
	FCREPL_PUBLICATION_RULE_TYPE_UNDEF = -1,
	FCREPL_PUBLICATION_RULE_TYPE_INCLUDE = 0,
	FCREPL_PUBLICATION_RULE_TYPE_EXCLUDE = 1
} FCREPLPublicationRuleType;

/* Publication rule type */
typedef enum
{
	FCREPL_RULE_TYPE_UNDEF = -1,
	FCREPL_RULE_TYPE_NAME = 0,
	FCREPL_RULE_TYPE_WILDCARD = 1,
	FCREPL_RULE_TYPE_REGEX = 2
} FCREPLRuleType;

/*^****************************************************************************\
*
*   Faircom Replication API C/C++ - Publication
*
\******************************************************************************/

/*^****************************************************************************\
*
*   Function:   fcReplGetPublications
*               Retrieve all the Publication entity objects
*
*   Parameters: replConn [IN]
*               Replication connection handle
*
*               dbEngine [IN]
*               DBEngine Identification
*
*               publicationListSize [IN]
*               Publication list count to be retrieved
*
*               publicationStart [IN]
*               Publication list start record
*
*               publicationList [OUT]
*               Publication entity list
*
*               publicationCount [OUT]
*               Number of Publication entities in the list
*
*               totalPublications [OUT]
*               Total number of Publication entities
*
*   Return:     Error code
*
\******************************************************************************/
FCREPL_API int fcReplGetPublications(pFCREPLConn replConn, int dbEngine, int publicationListSize, int publicationStart, ppFCREPLPublication *publicationList, int *publicationCount, int *totalPublications);

/*^****************************************************************************\
*
*   Function:   fcReplGetPublicationsByName
*               Retrieve the Publication list by Name, or partially filtered
*               by name.
*
*   Parameters: replConn [IN]
*               Replication connection handle
*
*               dbEngine [IN]
*               DBEngine ID
*
*               name [IN]
*               Publication name
*
*               partial [IN]
*               Flag indicating that the search by name should use partial name
*
*               publicationListSize [IN]
*               Publication list count to be retrieved
*
*               publicationStart [IN]
*               Publication list start record
*
*               publicationList [OUT]
*               Publication entity list
*
*               publicationCount [OUT]
*               Number of Publication entities in the list
*
*               totalPublications [OUT]
*               Total number of Publication entities
*
*   Return:     Error code
*
\******************************************************************************/
FCREPL_API int fcReplGetPublicationsByName(pFCREPLConn replConn, int dbEngine, const char *name, bool partial, int publicationListSize, int publicationStart, ppFCREPLPublication *publicationList, int *publicationCount, int *totalPublications);

/*^****************************************************************************\
*
*   Function:   fcReplGetPublication
*               Retrieve the Publication entity object by its ID.
*
*   Parameters: replConn [IN]
*               Replication connection handle
*
*               publicationID [IN]
*               Publication identification
*
*               publication [OUT]
*               Publication entity retrieved
*
*   Return:     Error code
*
\******************************************************************************/
FCREPL_API int fcReplGetPublication(pFCREPLConn replConn, int publicationID, ppFCREPLPublication publication);

/*^****************************************************************************\
*
*   Function:   fcReplPersistPublication
*               Add a Publication entity into the database. If there already
*               is a publication with the given ID, update the existing record.
*
*   Parameters: replConn [IN]
*               Replication connection handle
*
*               publication [IN]
*               Publication entity to be persisted
*
*   Return:     Error code
*
\******************************************************************************/
FCREPL_API int fcReplPersistPublication(pFCREPLConn replConn, pFCREPLPublication publication);

/*^****************************************************************************\
*
*   Function:   fcReplRemovePublication
*               Delete the Publication by its ID.
*
*   Parameters: replConn [IN]
*               Replication connection handle
*
*               publicationID [IN]
*               Publication identification
*
*               recursive [IN]
*               Recursive flag
*
*   Return:     Error code
*
\******************************************************************************/
FCREPL_API int fcReplRemovePublication(pFCREPLConn replConn, int publicationID, bool recursive);

/*^****************************************************************************\
*
*   Method:     fcReplGetRulesFromPublication
*               Retrieve the list of Rules from the given Publication.
*
*   Parameters: replConn [IN]
*               Replication connection handle
*
*               publication [IN]
*               Publication entity object
*
*               mode [IN]
*               Rules mode to be retrieved
*
*               ruleListSize [IN]
*               Rule list count to be retrieved
*
*               ruleStart [IN]
*               Rule list start record
*
*               ruleList [OUT]
*               Rule entity list
*
*               ruleCount [OUT]
*               Number of Rule entities in the list
*
*               totalRules [OUT]
*               Total Rules
*
*   Returns:    RCES error code
*
\******************************************************************************/
FCREPL_API int fcReplGetRulesFromPublication(pFCREPLConn replConn, pFCREPLPublication publication, FCREPLPublicationMode mode, int ruleListSize, int ruleStart, ppFCREPLRule *ruleList, int *ruleCount, int *totalRules);

/*^****************************************************************************\
*
*   Method:     fcReplPersistFolderRules
*               Persist folder rules for the publication
*
*   Parameters: masterProv [IN]
*               Master provider
*
*               publicationID [IN]
*               Publication identification
*
*               folderRuleList [IN]
*               List of folder rules entities to be persisted
*
*               folderRuleCount [IN]
*               Number of folder rules in the list
*
*   Return:     Error code
*
\******************************************************************************/
FCREPL_API int fcReplPersistFolderRules(pFCREPLConn replConn, int publicationID, ppFCREPLFolderRule folderRuleList, int folderRuleCount);

/*^****************************************************************************\
*
*   Method:     fcReplAddFolderRule
*               Persist folder rules for the publication
*
*   Parameters: masterProv [IN]
*               Master provider
*
*               folderRule [IN]
*               Existing folder rule
*
*               rule [IN]
*               Rule to be added
*
*   Return:     Error code
*
\******************************************************************************/
FCREPL_API int fcReplAddPublicationRule(pFCREPLConn replConn, pFCREPLFolderRule folderRule, pFCREPLPublicationRule rule);

/*^****************************************************************************\
*
*   Method:     fcReplPersistDictionaryRule
*               Persist a publication dictionary rule
*
*   Parameters: masterProv [IN]
*               Master provider
*
*               publicationID [IN]
*               Publication identification
*
*               dictRule [IN]
*               Dictionary rule entity to be persisted
*
*
\******************************************************************************/
FCREPL_API int fcReplPersistDictionaryRule(pFCREPLConn replConn, int publicationID, pFCREPLDictionaryRule dictRule);

/*^****************************************************************************\
*
*   Function:   fcReplGetFilesFromPublication
*               Retrieve the list of Files from the given Publication.
*
*   Parameters: replConn [IN]
*               Replication connection handle
*
*               publication [IN]
*               Publication object
*
*               fileListSize [IN]
*               File list count to be retrieved
*
*               fileStart [IN]
*               File list start record
*
*               fileList [OUT]
*               File entity list
*
*               fileCount [OUT]
*               Number of File entities in the list
*
*               totalFiles [OUT]
*               Total File
*
*   Returns:    Error code
*
\******************************************************************************/
FCREPL_API int fcReplGetFilesFromPublication(pFCREPLConn replConn, pFCREPLPublication publication, int fileListSize, int fileStart, ppFCREPLFile *fileList, int *fileCount, int *totalFiles);

/*^****************************************************************************\
*
*   Function:   fcReplAddFilesToPublication
*               Add list of Files to a group.
*
*   Parameters: replConn [IN]
*               Replication connection handle
*
*               publicationID [IN]
*               Existing Publication identification
*
*               fileIDList [IN]
*               Existing File identification list
*
*               fileCount [IN]
*               Number of Files to be added
*
*   Returns:    Error code
*
\******************************************************************************/
FCREPL_API int fcReplAddFilesToPublication(pFCREPLConn replConn, int publicationID, int *fileIDList, int fileCount);

/*^****************************************************************************\
*
*   Function:   fcReplRemoveFilesFromPublication
*               Remove list of Files from a Publication.
*
*   Parameters: replConn [IN]
*               Replication connection handle
*
*               publicationID [IN]
*               Existing Publication identification
*
*               fileIDList [IN]
*               Existing File identification list
*
*               fileCount [IN]
*               Number of Files to be removed
*
*   Returns:    Error code
*
\******************************************************************************/
FCREPL_API int fcReplRemoveFilesFromPublication(pFCREPLConn replConn, int publicationID, int *fileIDList, int fileCount);

/*^****************************************************************************\
*
*   Function:   fcReplGetTempPubName
*               Retrieve the next available default name for a Publication.
*
*   Parameters: replConn [IN]
*               Replication connection handle
*
*               tempPubName [OUT]
*               Temporary publication name
*
*   Returns:    Error code
*
\******************************************************************************/
FCREPL_API int fcReplGetTempPubName(pFCREPLConn replConn, char *tempPubName);


/*^****************************************************************************\
*
*   Class:      FCREPLPublication
*               Represents a group of file to be published. In order to set a
*               replication, this publication must be subscribed, which forms
*               a FCREPLSubscription, which is assigned to a FCREPLReplPlan
*               (Replication Plan).
*
\******************************************************************************/
class FCREPL_API FCREPLPublication : public RCESPublication
{
public:
	FCREPLPublication();
	FCREPLPublication(pFCREPLJson publication);

/*
** Getters
*/

/*
	Inherited getters

	// Publication identification.
	int GetID();

	// DBEngine identification.
	// It is linked to FCREPLDBEngine class.
	int GetDBEngineID();

    // DBEngine
    pRCESDBEngine GetDBEngine();

	// Database identification.
	// It is linked to FCREPLDatabase class.
	int GetDatabaseID();

    // Database
    pRCESDatabase GetDatabase();

	// Publication name.
	const char *GetName();

	// Publication description.
	const char *GetDescription();

	// Recursive.
	bool IsRecursive();
*/


/*
** Setters
*/

	// DBEngine entity object
	void SetDBEngine(pFCREPLDBEngine dbEngine) { ((pRCESPublication)this)->SetDBEngine((pRCESDBEngine)dbEngine); };

	// Publication name.
	void SetName(const char *name) { ((pRCESPublication)this)->SetName(name); };

	// Publication description.
	void SetDescription(const char *description) { ((pRCESPublication)this)->SetDescription(description); };

	// Recursive.
	void SetRecursive(bool recursive) { ((pRCESPublication)this)->SetRecursive(recursive); };

/*
	Inherited setters

	// Publication identification.
	void SetID(int id) ;

	// DBEngine identification.
	// It is linked to FCREPLDBEngine class.
	void SetDBEngineID(int dbEngineID);

    // DBEngine
    void SetDBEngine(pRCESDBEngine dbEngine);

	// Database identification.
	// It is linked to FCREPLDatabase class.
	void SetDatabaseID(int databaseID);

    // Database
    void SetDatabase(pRCESDatabase database);

    // Name
    void SetName(const char *name);

    // Description
    void SetDescription(const char *description);
*/
};


/*^****************************************************************************\
*
*   Class:      FCREPLRule
*               Represents a publication rule.
*
\******************************************************************************/
class FCREPL_API FCREPLRule : public RCESRule
{
public:
	FCREPLRule();
	FCREPLRule(pFCREPLRule rule);

	/*
	** Getters
	*/

	/*
		Inherited getters

		RCESPublicationRuleType GetType() { return _type; };
		FCList<RCESPublicationRule> *GetNameRules() { return _nameRuleList; };
		FCList<RCESPublicationRule> *GetWildcardRules() { return _wildcardRuleList; };
		FCList<RCESPublicationRule> *GetRegexRules() { return _regexRuleList; };
	*/

	/*
	** Setters
	*/
};

/*^****************************************************************************\
*
*   Class:      FCREPLDictionaryRule
*               Represents a publication dictionary rule.
*
\******************************************************************************/
class FCREPL_API FCREPLDictionaryRule : public RCESDictionaryRule
{
public:
	FCREPLDictionaryRule();
	FCREPLDictionaryRule(pFCREPLDictionaryRule dictRule);

	pRCESJson GetJson() { return RCESDictionaryRule::GetJson(); }

	/*
	** Getters
	*/

	/*
		Inherited getters

		int GetID() { return _id; };
		pFCREPLDatabase GetDatabase() { return _database; };
		pFCREPLFile GetDictFile() { return _dictFile; };
	*/

	/*
	** Setters
	*/

	/*
	Inherited setters

	void SetID(int databaseID) { _id = databaseID; };
	void SetDatabase(pRCESDatabase database) { _database = database; };
	void SetDictFile(pRCESFile dictFile) { _dictFile = dictFile; };
	*/
};

/*^****************************************************************************\
*
*   Class:      FCREPLFileRule
*               Represents a publication dictionary rule.
*
\******************************************************************************/
class FCREPL_API FCREPLFileRule : public RCESFileRule
{
public:
	FCREPLFileRule(pFCREPLFile file);

	pRCESJson GetJson() { return RCESFileRule::GetJson(); }

	/*
	** Getters
	*/

	/*
		Inherited getters

	pFCREPLFile GetFile() { return _file); };
	*/


	/*
	** Setters
	*/
};

/*^****************************************************************************\
*
*   Class:      FCREPLFolderRule
*               Represents a publication folder rule.
*
\******************************************************************************/
class FCREPL_API FCREPLFolderRule : public RCESFolderRule
{
public:
	FCREPLFolderRule();
	FCREPLFolderRule(pFCREPLFolderRule dictRule);

	pRCESJson GetJson() { return RCESFolderRule::GetJson(); }

	/*
	** Getters
	*/

	/*
		Inherited getters

	int GetID() { return _id; };
	int GetVolumeID() { return _volumeID; };
	const char *GetVolumeName() { return _volumeName; };
	const char *GetPath() { return _path; };
	*/

	/*
	** Setters
	*/

	void SetVolumeName(const char *volumeName) { ((pRCESFolderRule)this)->SetVolumeName(volumeName); };
	void SetPath(const char *path) { ((pRCESFolderRule)this)->SetPath(path); };

	/*
	Inherited setters

	void SetID(int fileID) { _id = fileID; };
	void SetVolumeID(int volumeID) { _volumeID = volumeID; };
	*/
};

/*^****************************************************************************\
*
*   Class:      FCREPLPublicationRule
*               Represents a publication rule.
*
\******************************************************************************/
class FCREPL_API FCREPLPublicationRule : public RCESPublicationRule
{
public:
	FCREPLPublicationRule();
	FCREPLPublicationRule(const char* rule, FCREPLRuleType type);

	/*
	** Getters
	*/

	/*
		Inherited getters

	int GetID() { return _id; };
	int GetVolumeID() { return _volumeID; };
	const char *GetVolumeName() { return _volumeName; };
	const char *GetPath() { return _path; };
	*/

	/*
	** Setters
	*/

	void SetRule(const char* rule) { ((pRCESPublicationRule)this)->SetRule(rule); };
	void SetType(FCREPLRuleType type) { ((pRCESPublicationRule)this)->SetType(RCESRuleType(type)); };

	/*
	Inherited setters

	void SetID(int fileID) { _id = fileID; };
	void SetVolumeID(int volumeID) { _volumeID = volumeID; };
	*/
};

#endif /* _FCREPLPUBLICATION_HPP_ */
