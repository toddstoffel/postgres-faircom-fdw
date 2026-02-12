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

#ifndef _FCRCESREPLFILE_HPP_
#define _FCRCESREPLFILE_HPP_

#include "fc_rces.h"
#include "fcaux.hpp"
#include "fclist.hpp"
#include "fcrcesfti.hpp"
#include "fcrcessubscription.hpp"

 /* Class type definitions */
class RCESIndexFileName;
typedef RCESIndexFileName* pRCESIndexFileName, ** ppRCESIndexFileName;

/*^****************************************************************************\
*
*   RCESReplFile
*   Recon-e-Sense replication file class
*
*   Replication source and target relationship
*
\******************************************************************************/
class RCESReplFile : public FCRefControl
{
public:
	/* Constructors & destructor */
	RCESReplFile();
	RCESReplFile(pRCESReplFile replFile);
	RCESReplFile(pRCESMasterProvider masterProv, pRCESSubscription subscription, pRCESFile sourceFile, pRCESFile targetFile);
	virtual ~RCESReplFile();

	/* Public services */
	static bool HasDupFileName(ppRCESReplFile replFileList, int fileCount);

	/* Public methods */
	bool IsCtree() { return _fileType == RCES_CTREP_QUALIF || _fileType == RCES_CTREP_QUALIF_RTG || _fileType == RCES_CTREESUPER; };
	char *GetSourceFullFileName();
	char *GetTargetFullFileName();
	char *GetSourceFullFilePath();
	char *GetTargetFullFilePath();

	short GetFileType() { return _fileType; };
	int GetSourceFileID() { return _sourceFileID; };
	int GetSourceOS() { return _sourceOS; };
	int GetSourceVolumeID() { return _sourceVolumeID; };
	const char *GetSourceVolume() { return _sourceVolume; };
	const char *GetSourcePath() { return _sourcePath; };
	const char *GetSourceFileName() { return _sourceFileName; };
	bool GetSourceIsPartitioned() { return _sourceIsPartitioned; };
	int GetSourceDatabaseID() { return _sourceDatabaseID; };
	int GetTargetFileID() { return _targetFileID; };
	int GetTargetOS() { return _targetOS; };
	int GetTargetVolumeID() { return _targetVolumeID; };
	const char *GetTargetVolume() { return _targetVolume; };
	const char *GetTargetPath() { return _targetPath; };
	const char *GetTargetFileName() { return _targetFileName; };
	bool GetTargetIsPartitioned() { return _targetIsPartitioned; };
	int GetTargetDatabaseID() { return _targetDatabaseID; };
	bool GetAddDatabase() { return _addDatabase; };
	int GetSubscriptionID() { return _subscriptionID; };
	pRCESSubscription GetSubscription() { return _subscription; };
	bool IsReverse() { return _reverse; }
	FCList<RCESFTI> *GetFTIList() { return _ftiList; };
	const char *GetSourceTableName() { return _sourceTableName; };
	const char *GetTargetTableName() { return _targetTableName; };

	void SetFileType(short fileType) { _fileType = fileType; };
	void SetSourceFileID(int sourceFileID) { _sourceFileID = sourceFileID; };
	void SetSourceOS(int sourceOS) { _sourceOS = sourceOS; };
	void SetSourceVolumeID(int sourceVolumeID) { _sourceVolumeID = sourceVolumeID; };
	void SetSourceVolume(const char *sourceVolume);
	void SetSourcePath(const char *sourcePath);
	void SetSourceFileName(const char *sourceFileName);
	void SetSourceIsPartitioned(bool sourceIsPartitioned) { _sourceIsPartitioned = sourceIsPartitioned; };
	void SetTargetFileID(int targetFileID) { _targetFileID = targetFileID; };
	void SetTargetOS(int targetOS) { _targetOS = targetOS; };
	void SetTargetVolumeID(int targetVolumeID) { _targetVolumeID = targetVolumeID; };
	void SetTargetVolume(const char *targetVolume);
	void SetTargetPath(const char *targetPath);
	void SetTargetFileName(const char *targetFileName);
	void SetTargetIsPartitioned(bool targetIsPartitioned) { _targetIsPartitioned = targetIsPartitioned; };
	void SetAddDatabase(bool addDatabase) { _addDatabase = addDatabase; };
	void SetSubscriptionID(int subscriptionID) { _subscriptionID = subscriptionID; };
	void SetSubscription(pRCESSubscription subscription) { if (_subscription) delete _subscription; _subscription = new RCESSubscription(subscription); };
	void SetIsReverse(bool reverse) { _reverse = reverse; };
	void SetFTIList(FCList<RCESFTI> *ftiList) { _ftiList = ftiList;  };
	void SetaidxList(FCList<RCESIndexFileName>* aidxList) { _aidxList = aidxList; };
	FCList<RCESIndexFileName>* GetaidxList() { return _aidxList; };

private:
	/* Private properties */
	short _fileType;
	int _sourceFileID;
	int _sourceOS;
	int _sourceVolumeID;
	char* _sourceVolume;
	char* _sourcePath;
	char* _sourceFileName;
	bool _sourceIsPartitioned;
	int _sourceDatabaseID;
	int _targetFileID;
	int _targetOS;
	int _targetVolumeID;
	char* _targetVolume;
	char* _targetPath;
	char* _targetFileName;
	bool _targetIsPartitioned;
	int _targetDatabaseID;
	bool _addDatabase;
	int _subscriptionID;
	pRCESSubscription _subscription;
	bool _reverse;
	char _fullFileName[FC_MAX_PATH + 1];
	FCList<RCESFTI> *_ftiList;
	char *_sourceTableName;
	char *_targetTableName;
	FCList<RCESIndexFileName>* _aidxList;
};
/*~****************************************************************************/

/*^****************************************************************************\
*
*   RCESReplFileList
*   Recon-e-Sense replication file list class
*
*   It manages the two lists of Replication Files, for the original and reverse
*   directions
*
\******************************************************************************/
class RCESReplFileList
{
	public:
		/* Constructors & destructor */
		RCESReplFileList();
		virtual ~RCESReplFileList();

		ppRCESReplFile GetReplFileList() { return _replFileList; };
		int GetReplFileCount() { return _replFileCount; };
		ppRCESReplFile GetRevReplFileList() { return _revReplFileList; };
		int GetRevReplFileCount() { return _revReplFileCount; };

		void SetReplFileList( ppRCESReplFile replFileList ) { _replFileList = replFileList; };
		void SetReplFileCount(int replFileCount) { _replFileCount = replFileCount; };
		void SetRevReplFileList( ppRCESReplFile revReplFileList ) { _revReplFileList = revReplFileList; };
		void SetRevReplFileCount(int revReplFileCount) { _revReplFileCount = revReplFileCount; };
		void AddReplFile( pRCESReplFile replFile ) { _replFileList[ _replFileCount++ ] = replFile; };
		void AddRevReplFile( pRCESReplFile revReplFile) { _revReplFileList[ _revReplFileCount++ ] = revReplFile; };

	private:
		/* Private properties */
		ppRCESReplFile _replFileList;
		int _replFileCount;
		ppRCESReplFile _revReplFileList;
		int _revReplFileCount;
};
/*~****************************************************************************/

/*^****************************************************************************\
*
*   RCESIndexFileName
*   Recon-e-Sense index file name class
*
*   It manages the index file name holding its source and target paths
*
\******************************************************************************/
class RCESIndexFileName : public FCRefControl
{
	public:
		/* Constructors & destructor */
		RCESIndexFileName(const char *sourceIndexFileName, const char *targetIndexFileName);
		RCESIndexFileName(pRCESIndexFileName indexFileName);
		virtual ~RCESIndexFileName();

		/* Public methods */
		const char* GetSourceFileName() { return _sourceFileName; };
		const char* GetTargetFileName() { return _targetFileName; };

	private:
		/* Private properties */
		char* _sourceFileName;
		char* _targetFileName;
};

#endif /* _FCRCESREPLFILE_HPP_ */