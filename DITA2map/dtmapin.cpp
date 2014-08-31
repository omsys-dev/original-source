// dtmapin.cpp processes dita2map.ini settings, JHG
// Copyright (c) 2009 by Jeremy H. Griffith.  All rights reserved.

#include "dcappl.h"
#include "dcctlg.h"
#include "dcfili.h"
#include "dcfilp.h"
#include "dcini.h"
#include "dcelem.h"
#include "expat.h"
#include "d2m.h"
#include "drxmlfl.h"
#include "dwhtmfl.h"


secinfo D2M::D2MIniSects[] = {
 { "Options", set, "OptionsSet", ISecOptions, true },
 { "Setup", set, NULL, ISecSetup, true },
 { "Logging", set, "LoggingSet", ILogOptions, true },

 { "MapGeneration", set, NULL, ISecMapOptions, true },

 { NULL, unknown, NULL, NULL, false }
};


// OptionsSet

setinfo D2M::ISecOptions[] = {
 { "CaselessMatch", yesno, &CaselessMatch, CaselessMatchSet },
 { "SpacelessMatch", yesno, &SpacelessMatch, SpacelessMatchSet },
 { "WildcardMatch", yesno, &WildcardMatch, WildcardMatchSet },

 { NULL, spec, NULL, NULL }
};


void
D2M::CaselessMatchSet(char *)
{
	IniFile->SetMatch(1, CaselessMatch);
}

void
D2M::SpacelessMatchSet(char *)
{
	IniFile->SetMatch(2, SpacelessMatch);
}

void
D2M::WildcardMatchSet(char *)
{
	IniFile->SetMatch(3, WildcardMatch);
}


// SetupSet

setinfo D2M::ISecSetup[] = {
 { "ConfigTemplate", str, &ConfigTemplate, NULL },
 { "DTDPath", str, &DTDPath, NULL },

 { NULL, spec, NULL, NULL }
};


// LoggingSet

setinfo D2M::ILogOptions[] = {
 { "UseLog", yesno, &UseLog, NULL },
 { "LogFileName", str, &LogFileName, NULL },
 { "LogErrors", ucnum, &LogLevels[logerr], NULL },
 { "LogWarnings", ucnum, &LogLevels[logwarn], NULL },
 { "LogQuerys", ucnum, &LogLevels[logquery], NULL },
 { "LogInfo", ucnum, &LogLevels[loginfo], NULL },
 { "LogDebug", ucnum, &LogLevels[logdebug], NULL },

 { NULL, spec, NULL, NULL }
};



// MapOptionsSet

setinfo D2M::ISecMapOptions[] = {
 { "UseTopicShortdesc", yesno, &UseTopicShortdesc, NULL },
 { "MapTitle", str, &MapTitle, NULL },
 { "MapRootElem", str, &MapRootElem, NULL },
 { "MapTitleElem", str, &MapTitleElem, NULL },
 { "MapLanguage", str, &MapLanguage, NULL },
 { "PublicID", str, &PublicID, NULL },
 { "SystemID", str, &SystemID, NULL },

 { NULL, spec, NULL, NULL }
};



// end of dtmapin.cpp

