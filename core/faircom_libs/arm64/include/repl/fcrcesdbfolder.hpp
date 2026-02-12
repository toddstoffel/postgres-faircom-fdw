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

#ifndef _FCRCESDBFOLDER_HPP_
#define _FCRCESDBFOLDER_HPP_

#include "fc_rces.h"
#include "fcaux.hpp"
#include "fcrcesaux.hpp"
#include "fcrcesfile.hpp"
#include "fclist.hpp"

extern "C" {
#include "fcrcesjson.h"
}

#define RCES_DBFOLDER_LIST "databaseFolderList"
#define RCES_DBFOLDER_TABLELIST "tableList"
#define RCES_DBFOLDER_ID "id"
#define RCES_DBFOLDER_LISTTABLES "listTables"

/*^****************************************************************************\
*
*   RCESDBFolder
*   Recon-e-Sense database folder class
*
*   File System file information
*
\******************************************************************************/
class RCESDBFolder : public RCESFile
{
public:
	/* Constructors & destructor */
	RCESDBFolder(pRCESFile folder);
	virtual ~RCESDBFolder();

	/* Public services */

	/* Public methods */
	FCList<RCESFile> *GetTableList() { return _tableList; };

	void AddTable(pRCESFile table) { _tableList->Add(new RCESFile(table)); };

	pRCESJson GetJson(bool listTables);

private:
	/* Private properties */
	FCList<RCESFile> *_tableList;
};
/*~****************************************************************************/

/* Class type definitions */
typedef RCESDBFolder *pRCESDBFolder, **ppRCESDBFolder;

#endif /* _FCRCESDBFOLDER_HPP_ */
