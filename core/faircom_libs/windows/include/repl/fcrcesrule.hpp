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

#ifndef _FCRCESRULE_HPP_
#define _FCRCESRULE_HPP_

#include "fc_rces.h"
#include "fcaux.hpp"
#include "fcrcesaux.hpp"

extern "C" {
#include "fcrcesjson.h"
}

/*^****************************************************************************\
*
*   RCESRule
*   Recon-e-Sense publication rule class
*
\******************************************************************************/
class RCESRule
{
public:
	/* Constructors & destructor */
	RCESRule();
	RCESRule(pRCESRule rule);
	virtual ~RCESRule();

	/* Public services */

	/* Public methods */
	virtual pRCESJson GetJson() = 0;

	RCESPublicationRuleType GetType() { return _type; };
	FCList<RCESPublicationRule> *GetNameRules() { return _nameRuleList; };
	FCList<RCESPublicationRule> *GetWildcardRules() { return _wildcardRuleList; };
	FCList<RCESPublicationRule> *GetRegexRules() { return _regexRuleList; };

	void SetType(RCESPublicationRuleType type) { _type = type; };

	bool MatchesRules(const char *name, bool caseSensitive = false);
	bool Matches(pRCESRule rule);

protected:
	RCESPublicationRuleType _type;
	FCList<RCESPublicationRule> *_nameRuleList;
	FCList<RCESPublicationRule> *_wildcardRuleList;
	FCList<RCESPublicationRule> *_regexRuleList;

private:
	/* Private properties */

	/* Private methods */
	void _init();
};
/*~****************************************************************************/

/* Class type definitions */
typedef RCESRule *pRCESRule, **ppRCESRule;

#endif /* _FCRCESRULE_HPP_ */