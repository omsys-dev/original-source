/////////////////////////////////////////////////////////////////////////////
// Name:        cmdcl.cpp
// Purpose:     CM functions not part of the GUI itself
// Author:      Jeremy H. Griffith
// Modified by: 
// Created:     06/05/2012
// RCS-ID:      
// Copyright:   (c) Copyright 2012 Omni Systems CIA. LTDA.
// Licence:     
/////////////////////////////////////////////////////////////////////////////

// For compilers that support precompilation, includes "wx/wx.h".
#include "wx/wxprec.h"

#ifdef __BORLANDC__
#pragma hdrstop
#endif

#ifndef WX_PRECOMP
#include "wx/wx.h"
#endif

#include "d2gcmapp.h"
#include "wx/bookctrl.h"
#include "wx/msgdlg.h"
#include "wx/filedlg.h"

#include "configmgr.h"
#include "d2gcmapp.h"

#pragma warning( disable : 4089 4189 4710)

#include "dcglob.h"
#include "dcbase.h"
#include "dclist.h"
#include "dcnode.h"
#include "dcfile.h"
#include "dcfilr.h"
#include "dcfilw.h"
#include "dclog.h"
#include "dcfili.h"
#include "dcfilp.h"
#include "dctxfx.h"
#include "dcfmtr.h"

#include "cmdcl.h"

#include <windows.h>
#include "htmlhelp.h"


// do any inits for cmdcl, like opening log file

DCirfile *CMgr::CMIni = NULL;

long CMgr::PosX = -1;
long CMgr::PosY = -1;
long CMgr::SizeX = -1;
long CMgr::SizeY = -1;
long CMgr::ChildPosX = -1;
long CMgr::ChildPosY = -1;
pntyp CMgr::CurrPanel = pnfile;
bool CMgr::AllowSysIniSelect = false;
bool CMgr::IncludeAllSections = false;
bool CMgr::IncludeAllSettings = false;
long CMgr::cmVersion = 1;
bool CMgr::CMIniChanged = false;


void ConfigMgr::InitializeDCL(void)
{
	InitPos = NULL;
	InitSize = NULL;
	
	if (!D2gcmApp::ResultFile.IsEmpty()) {
		CMgr::ResultFileName = NewName(D2gcmApp::ResultFile.c_str());
		CMgr::ChildCM = true;
	}

  CMgr::SetCMIni();
	
	if (CMgr::PosX && CMgr::PosY)
		InitPos = new wxPoint(CMgr::PosX, CMgr::PosY);
	if (CMgr::SizeX && CMgr::SizeY)
		InitSize = new wxSize(CMgr::SizeX, CMgr::SizeY);

  CMgr::SetSysIni();
	CMgr::SetUpLog();
	CMgr::SetUpHelp();
  CMgr::SetProjIni();
}


void CMgr::SetCMIni(void)
{
	//if (FileExists("d2gcm.ini"))  // in local dir, so can use cutom ini
	//	CMIniName = "d2gcm.ini";    //  by setting currdir in shortcut

	CMIni = new DCirfile(CMIniName);
  if (!CMIni)
    return;
  if (CMIni->FErr() == fok) {

		CMIni = CMIni->SetConfigTemplate("Templates", "Configs");

		// needed for both primary and child

		if (CMIni->Section("Placement")) {
			if (CMIni->Find("SizeX")) {
				SizeX = CMIni->IntRight();
				if (SizeX < 500)
					SizeX = 500;
			}
			if (CMIni->Find("SizeY")) {
				SizeY = CMIni->IntRight();
				if (SizeY < 750)
					SizeY = 750;
			}
			if (CMIni->Find("PosX")) {
				PosX = CMIni->IntRight();
				if ((PosX < -1000) || (PosX > 3000))
					PosX = -1;
			}
			if (CMIni->Find("PosY")) {
				PosY = CMIni->IntRight();
				if ((PosY < 1) || (PosY > 1000))
					PosY = -1;
			}
		}
		if (CMIni->Section("Help")) {
			if (CMIni->Find("UseHTMLHelp"))
				UseHTMLHelp = CMIni->BoolRight();
			if (CMIni->Find("CHMPath"))
				CHMPathSet = CMIni->StrRight();
		}
		if (CMIni->Section("Colors")) {
			if (CMIni->Find("AvailableSection"))
				SectColorText = CMIni->StrRight();
			if (CMIni->Find("AvailableSetting"))
				SetColorText = CMIni->StrRight();
			if (CMIni->Find("VariableNameKey"))
				VarColorText = CMIni->StrRight();
			if (CMIni->Find("ProgramDefault"))
				DefColorText = CMIni->StrRight();
			if (CMIni->Find("SystemIni"))
				SysColorText = CMIni->StrRight();
		}

		if (ChildCM) {
			UseLog = false;
			AllowSysIniSelect = false;

			if (CMIni->Section("Placement")) {
				if (CMIni->Find("ChildPosX")) {
					PosX = CMIni->IntRight();
					if ((PosX < -3000) || (PosX > 3000))
						PosX = -1;
				}
				else
					ChildPosX = (PosX += SizeX);
				if (CMIni->Find("ChildPosY")) {
					PosY = CMIni->IntRight();
					if ((PosY < 1) || (PosY > 1000))
						PosY = -1;
				}
				else
					ChildPosY = PosY;
			}
		}
		else {  // primary
			if (CMIni->Section("Logging")) {
				if (CMIni->Find("LogFileName"))
					LogFileName = CMIni->PathRight();
				if (CMIni->Find("UseLog"))
					UseLog = CMIni->BoolRight();
				if (CMIni->Find("LogIniChains"))
					LogIniChains = CMIni->BoolRight();
				if (CMIni->Find("AddIniHistory"))
					AddIniHistory = CMIni->BoolRight();
				if (CMIni->Find("UserName"))
					UserName = CMIni->StrRight();
			}
			if (CMIni->Section("Preferences")) {
				if (CMIni->Find("AllowSysIniSelect")) 
					AllowSysIniSelect = CMIni->BoolRight();
				//if (CMIni->Find("IncludeAllSections"))
				//	IncludeAllSections = CMIni->BoolRight();
				if (CMIni->Find("CheckAlias"))
					CheckAlias = CMIni->BoolRight();
				if (CMIni->Find("GetAllInis"))
					GetAllInis = CMIni->BoolRight();
			}
			if (CMIni->Section("Help")) {
				if (CMIni->Find("Browser"))
					Browser = CMIni->StrRight();
				if (CMIni->Find("HelpFile"))
					HelpCommand = CMIni->StrRight();
				if (CMIni->Find("DefaultAlias"))
					DefaultAlias = CMIni->StrRight();
			}
		}
	}
  CMIni->CloseFile();
  CMIni = NULL;
}


void ConfigMgr::EndingUpdate(void)
{
	EndingPos = new wxPoint(GetScreenPosition());
	EndingSize = new wxSize(GetSize());

	if ((InitPos->x != EndingPos->x)
	 || (InitPos->y != EndingPos->y)) {
		CMgr::PosX = EndingPos->x;
		CMgr::PosY = EndingPos->y;
		CMgr::CMIniChanged = true;
	}

	if ((InitSize->GetWidth() != EndingSize->GetWidth())
	 || (InitSize->GetHeight() != EndingSize->GetHeight())) {
		CMgr::SizeX = EndingSize->GetWidth();
		CMgr::SizeY = EndingSize->GetHeight();
		CMgr::CMIniChanged = true;
	}

	if (CMgr::CMIniChanged)
		CMgr::UpdateCMIni();

	if (CMgr::UseLog)
		CMgr::Log->CloseFile();
}


void CMgr::UpdateCMIni(void)
{
	char buf[34];

	DCpfile::addhist(NULL);

	if (ChildCM) {
		DCpfile::putval("Placement", "ChildPosX", ltoa(PosX, buf, 10), CMIniName);
		DCpfile::putval("Placement", "ChildPosY", ltoa(PosY, buf, 10), CMIniName);
	}
	else {
		DCpfile::putval("Status", "cmVersion", ltoa(cmVersion, buf, 10), CMIniName);

		//DCpfile::putval("Preferences", "IncludeAllSections",
		//	              (IncludeAllSections ? "Yes" : "No"), CMIniName);

		//DCpfile::putval("Logging", "UseLog", (UseLog ? "Yes" : "No"), CMIniName);
		//DCpfile::putval("Logging", "LogFileName", LogFileName, CMIniName);
		//DCpfile::putval("Logging", "LogIniChains", (LogIniChains ? "Yes" : "No"), CMIniName);

		DCpfile::putval("Logging", "AddIniHistory", (AddIniHistory ? "Yes" : "No"), CMIniName);
		DCpfile::putval("Logging", "UserName", UserName, CMIniName);

		//DCpfile::putval("Help", "Browser", Browser, CMIniName);
		//DCpfile::putval("Help", "HelpFile", HelpCommand, CMIniName);

		DCpfile::putval("Placement", "PosX", ltoa(PosX, buf, 10), CMIniName);
		DCpfile::putval("Placement", "PosY", ltoa(PosY, buf, 10), CMIniName);
		DCpfile::putval("Placement", "SizeX", ltoa(SizeX, buf, 10), CMIniName);
		DCpfile::putval("Placement", "SizeY", ltoa(SizeY, buf, 10), CMIniName);
	}

	DCpfile::pclose(CMIniName);
}


DCirfile *CMgr::SysIni = NULL;


void CMgr::SetSysIni(void)
{
	SysIni = new DCirfile(SysIniName);
  if (!SysIni)
    return;
  if (SysIni->FErr() != fok) {
    SysIni->CloseFile();
    SysIni = NULL;
    return;
  }
	SysIni = SysIni->SetConfigTemplate("Templates", "Configs");
  
  if (SysIni->Section("StartingTemplates")) {
		while (SysIni->NextItem()) {
			StartFormatList.add(CMgr::SysIni->StrLeft(), ++StartCount);
			StartNameList.add(CMgr::SysIni->StrRight(), StartCount);
		}
  }
  
  if (SysIni->Section("ProjectFormatNames")) {
		while (SysIni->NextItem()) {
			ProjFormatNameList.add(SysIni->StrLeft(), ++ProjFormCount);
			ProjIniNameList.add(SysIni->StrRight(), ProjFormCount);
		}
	}

  if (SysIni->Section("OutputCodes")) {
		while (SysIni->NextItem()) {
			FormatCodeKeys.add(SysIni->StrLeft(), ++CodesCount);
			FormatCodes.add(SysIni->StrRight(), CodesCount);
		}
	}
}


bool CMgr::UseLog = true;
char *CMgr::LogFileName = "_cmlog.txt";
DClog *CMgr::Log = NULL;
unc CMgr::LogLevels[logmax] = { 0, 1, 1, 1, 1, 0 };
bool CMgr::LogIniChains = true;


void CMgr::SetUpLog(void)
{
	if (!UseLog)
    return;
        
  Log = new DClog(LogFileName, CMProgName, CMVersion, LogLevels);
  if (Log->FErr() != fok) {
    UseLog = false;
    return;
  }
	DCirfile::LogEventFunc = (logfunc) LogEvent;
}


void CMgr::LogEvent(logtype lt, unc sev, const char *desc,
 const char *d2, const char *d3, const char *d4, const char *d5)
{
	if (!UseLog)
		return;

	Log->logEvent(lt, sev, desc, d2, d3, d4, d5);
}


// help setup

DCnlist CMgr::SectReplaceNames;
DCilist CMgr::SectReplaceIdx;
long CMgr::SectReplaceNameCount = 0;
DCnlist CMgr::SettingValueNames;
DCnlist CMgr::SetValueHelpText;
long CMgr::SetValueCount = 0;
DCnlist CMgr::MacroNames;
DCnlist CMgr::MacroHelp;
long CMgr::MacroCount = 0;
//DCnlist CMgr::SectionAlias;
bool CMgr::CheckAlias = false;

DCilist CMgr::IniTypes;
ipair CMgr::IniTypesInit[] = {
 { 1, 'C'},
 { 2, 'L'},
 { 3, 'M'},
 { 4, 'F'},
 { 5, 'T'},
 { 6, 'S'},
 { 7, 'P'},
 { 8, 'D'},
 { 9, 'X'},
 NULL
};
DCvlist CMgr::AvailIniFiles;
SIni *CMgr::AvailIni = NULL;
SSection *CMgr::AvailSection = NULL;
SSetting *CMgr::AvailSetting = NULL;
DCnlist CMgr::AvailSectionNames;
DCvlist CMgr::AvailSections;
long CMgr::AvailSectionNameCount = 0;

DCvlist CMgr::AvailSuffixIniFiles;
SIni *CMgr::SuffixIni = NULL;
DCnlist CMgr::AvailVNSuffixes;
DCvlist CMgr::AvailVNSuffixSections;
DCvlist CMgr::AvailVNSuffixSets;
DCnlist CMgr::AvailVNSuffixHelp;
long CMgr::AvailVNSuffixCount = 0;

DCvlist CMgr::VKEnums2Proc;
DCnlist CMgr::VKEnumNames;
long CMgr::VKEnums2ProcCount = 0;
DCvlist CMgr::FKEnums2Proc;
DCnlist CMgr::FKEnumNames;
long CMgr::FKEnums2ProcCount = 0;

DCnlist CMgr::EnumHelp;
DCnlist CMgr::AvailSEnums;

DCnlist CMgr::SectCats;
DCnlist CMgr::SuffCats;
DCnlist CMgr::ValueCats;

DCnlist CMgr::FilePicker;
DCnlist CMgr::FilePickerOptions;
DCnlist CMgr::FilePickerFilter;
long CMgr::FilePickerCount = 0;


void CMgr::SetUpHelp(void)
{
	char *sname = NULL;
	char *setname = NULL;
	char *outname = NULL;
	char *vkset = NULL;
	char *str = NULL;
	long idx = 0;
	long typ = 0;
	DClist *dl = NULL;
	DCvlist *dvl = NULL;
	DCnlist *dnl = NULL;
	SSection *sect = NULL;
	SSection *osect = NULL;
	bool show = true;

	IniTypes.init(IniTypesInit);
	for (idx = 1; idx < (long) inilast; idx++) {
		AvailIniFiles.add(new SIni((inityp) idx), idx);
		AvailSuffixIniFiles.add(new SIni((inityp) idx), idx);
	}

	if (SysIni->Section("InternalVariables")) {
		if (SysIni->Find("FixedMacroSectionsCat"))
			FixedMacroSectionsCat = SysIni->StrRight();
		if (SysIni->Find("MacroNamesCat"))
			MacroNamesCat = SysIni->StrRight();

		if (SysIni->Find("MacrosSect"))
			MacrosSect = SysIni->StrRight();
		if (SysIni->Find("MacroVariablesSect"))
			MacroVariablesSect = SysIni->StrRight();

		if (SysIni->Find("ConfigsCat"))
			ConfigsCat = SysIni->StrRight();
		if (SysIni->Find("ParaCharFormatsCat"))
			ParaCharFormatsCat = SysIni->StrRight();
		if (SysIni->Find("TableFormatsCat"))
			TableFormatsCat = SysIni->StrRight();
		if (SysIni->Find("SubformatsCat"))
			SubformatsCat = SysIni->StrRight();
		if (SysIni->Find("PageFormatsCat"))
			PageFormatsCat = SysIni->StrRight();
		if (SysIni->Find("LanguagesCat"))
			LanguagesCat = SysIni->StrRight();
		if (SysIni->Find("MacrosCat"))
			MacrosCat = SysIni->StrRight();
		if (SysIni->Find("ContentModelsCat"))
			ContentModelsCat = SysIni->StrRight();
	}
    
  // read [Categories] and populate CatNameList (left) and CatLevelList (right)

	if (!SysIni->Section("Categories"))
		LogEvent(logerr, 1, "[Categories] section is missing altogether");
	else {	
		while (SysIni->NextItem()) {
			CatNameList.add(SysIni->StrLeft(), ++CatCount);
			CatLevelList.add(SysIni->IntRight(), CatCount);
		}
	}

	// get the category names
	if (SysIni->Section("SectionCats")) {
		while (SysIni->NextItem())
			SectCats.add(SysIni->StrLeft(), (long) SysIni->StrArrRight());
	}
	if (SysIni->Section("SuffixCats")) {
		while (SysIni->NextItem())
			SuffCats.add(SysIni->StrLeft(), (long) SysIni->StrArrRight());
	}
	if (SysIni->Section("ValueCats")) {
		while (SysIni->NextItem())
			ValueCats.add(SysIni->StrLeft(), (long) SysIni->StrRight());
	}


	if (SysIni->Section("SectionHelp")) {
		while (SysIni->NextItem()) {
			sect = NULL;
			dl = NULL;
			sname = SysIni->StrLeft();
			if (sname[strlen(sname) - 1] == '-') {
				sname[strlen(sname) - 1] = '\0';
				show = false;
			}
			else
				show = true;
			typ = IniTypes.getitem(*sname);
			AvailIni = (SIni *) AvailIniFiles.find(typ);
			if (!AvailIni)
				LogEvent(logwarn, 1, "In [SectionHelp], category prefix missing for section ", sname);
			else {
				if (((idx = AvailSectionNames.getitem(sname + 1)) != 0)
				 && ((dl = (DClist *) AvailSections.find(idx)) != NULL)) {
					while (dl->next) {
						dl = dl->next;
						osect = (SSection *) dl->id;
						if (osect->ifile->ityp == typ) {
							LogEvent(logwarn, 1, "In [SectionHelp], duplicate for section ", sname);
							dl = NULL;
							break;
						}
					}
				}
				else {
					AvailSectionNames.addnew(sname + 1, ++AvailSectionNameCount);
					AvailSections.add(dl = new DClist(), AvailSectionNameCount);
				}
				if (dl) {
					AvailIni->sections.add((sect = new SSection(sname + 1, AvailIni)),
																 ++(AvailIni->sectcount));
					AvailIni->sectnames.add(sect->name, AvailIni->sectcount);
					dl->add((long) sect);
					sect->htext = SysIni->StrRight();
					sect->nametyp = stfix;
					sect->show = show;
					long pos = 0;
					if (((pos = SectCats.getitem(sname)) !=  0)
					 || ((pos = SectCats.getitem(sname + 1)) !=  0))
						sect->cats = (char **) pos;
				}
			}
		}
	}

	if (!AvailSectionNameCount) {
		LogEvent(logwarn, 1, "[SectionHelp] section is missing altogether!");
		LogEvent(logcont, 1, "Cannot load section and setting info without it.");
		return;
	}

	// check SectionReplacements and map those names to the idx
	// of the new one in AvailSectionNames
	if (SysIni->Section("SectionReplacements")) {
		while (SysIni->NextItem()) {
			SectReplaceNames.add(SysIni->StrLeft(), ++SectReplaceNameCount);
			sname = SysIni->StrRight();
			if ((idx = SectReplaceNames.getitem(sname)) != 0)
				SectReplaceIdx.add(idx, SectReplaceNameCount);
			else if ((idx = AvailSectionNames.getitem(sname)) != 0)
				SectReplaceIdx.add(idx, SectReplaceNameCount);
			else
				LogEvent(logwarn, 1, "Replacement section ", sname, " not found.");
		}
	}


	// get the SettingValues names and help
	if (SysIni->Section("SettingValues")) {
		while (SysIni->NextItem()) {
			SettingValueNames.add(SysIni->StrLeft(), ++SetValueCount);
			SetValueHelpText.add(SysIni->StrRight(), SetValueCount);
		}
	}

	// get the macro names and help
	if (SysIni->Section("MacrosHelp")) {
		while (SysIni->NextItem()) {
			MacroNames.add(SysIni->StrLeft(), ++MacroCount);
			MacroHelp.add(SysIni->StrRight(), MacroCount);
		}
	}

	// get the enum help
	if (SysIni->Section("EnumHelp")) {
		while (SysIni->NextItem())
			EnumHelp.add(SysIni->StrLeft(), (long) SysIni->StrRight());
	}

	// get the info for file selection
	if (SysIni->Section("FilePicker")) {
		while (SysIni->NextItem()) {
			char *nm = SysIni->StrLeft();
			char *vs = SysIni->StrRight();
			char *spc = strchr(vs + 1, ' ');
			if (spc && *(spc + 1))
				*spc = '\0';
			else
				spc = NULL;
			FilePicker.add(NewName(nm), ++FilePickerCount);
			FilePickerOptions.add(NewName(vs), FilePickerCount);
			if (spc)
				FilePickerFilter.add(NewName(spc + 1), FilePickerCount);
		}
	}

#if 0
	// get the SectionAlias names
	if (CheckAlias
	 && SysIni->Section("SectionAlias")) {
		long cnt = 0;
		while (SysIni->NextItem())
			SectionAlias.add(SysIni->StrLeft(), ++cnt);
	}
#endif

	// get the HelpAlias filenames for CHM
	if (SysIni->Section("HelpAlias")) {
		while (SysIni->NextItem())
			HelpAlias.add(SysIni->StrLeft(), (long) SysIni->StrRight());
	}


	// get the VN suffixes
	if (SysIni->Section("SuffixHelp")) {
		while (SysIni->NextItem()) {
			AvailVNSuffixes.add(SysIni->StrLeft(), ++AvailVNSuffixCount);
			AvailVNSuffixHelp.add(SysIni->StrRight(), AvailVNSuffixCount);
		}
	}

	if (!AvailVNSuffixCount) {
		LogEvent(logwarn, 1, "SuffixHelp section is missing altogether");
		//return;
	}
	else {
		for (idx = 1; idx <= AvailVNSuffixCount; idx++) {
			str = AvailVNSuffixes.find(idx);
			if (str[strlen(str) - 1] == '-') {
				str[strlen(str) - 1] = '\0';
				show = false;
			}
			else
				show = true;
			typ = IniTypes.getitem(*str);
			SuffixIni = (SIni *) AvailSuffixIniFiles.find(typ);
			sect = new SSection(str + 1, SuffixIni);
			SuffixIni->sections.add(sect, ++(SuffixIni->sectcount));
			SuffixIni->sectnames.add(sect->name, SuffixIni->sectcount);
			sect->htext = AvailVNSuffixHelp.find(idx);
			sect->nametyp = stvar;
			sect->show = show;

			long pos = 0;
			if (((pos = SuffCats.getitem(str)) !=  0)
			 || ((pos = SuffCats.getitem(str + 1)) !=  0))
				sect->cats = (char **) pos;

			if (SysIni->Section("SuffixOutputs")
			 && (SysIni->Find(str)
			  || SysIni->Find(str + 1)))
				sect->formats = SysIni->StrRight();

			// see if VK
			if (SysIni->Section("VKSuffixes")
			 && (SysIni->Find(str)
			  || SysIni->Find(str + 1))
			 && ((vkset = SysIni->StrRight()) != NULL)
			 && (*vkset != '\0'))
				GetVKSettings(sect, vkset);

			// see if FK
			setname = NewName(str, strlen(str) + 4);
			strcat(setname, "Set");
			GetFKSettings(sect, setname, true);
		}
	}

	// proc enums for suffixes first

	if (VKEnums2ProcCount) {
		for (long i = 1; i <= VKEnums2ProcCount; i++)
			ProcVKEnum((SSection *) VKEnums2Proc.find(i), VKEnumNames.find(i));
		VKEnums2Proc.empty();
		VKEnums2ProcCount = 0;
	}

	if (FKEnums2ProcCount) {
		for (long i = 1; i <= FKEnums2ProcCount; i++)
			ProcFKEnum((SSetting *) FKEnums2Proc.find(i), FKEnumNames.find(i));
		FKEnums2Proc.empty();
		FKEnums2ProcCount = 0;
	}


	// use the AvailSectionNames idx for all the section lists

	// if section is in VKSections,
	// get the keys for SettingTypes and SettingValues
	// validate the keys and the existence of a TopicAlias
	if (SysIni->Section("VKSections")) {
		while (SysIni->NextItem()) {
			typ = 0;
			sname = SysIni->StrLeft();
			if (sname[strlen(sname) - 1] == '-') {
				sname[strlen(sname) - 1] = '\0';
				show = false;
			}
			else
				show = true;
			if (((idx = AvailSectionNames.getitem(sname)) == 0)
			 && (((typ = IniTypes.getitem(*sname)) == 0)
			  || ((idx = AvailSectionNames.getitem(sname + 1)) == 0)))
				LogEvent(logwarn, 1, "in [VKSection], item ", sname, " is not in [SectionHelp]");
			else if (((vkset = SysIni->StrRight()) == NULL)
			 || (*vkset == '\0'))
				LogEvent(logwarn, 1, "In [VKSection], item ", sname, " is missing types and values");
			else if ((dl = (DClist *) AvailSections.find(idx)) != NULL) {
				while (dl->next) {
					dl = dl->next;
					if (((sect = (SSection *) dl->id)->ifile->ityp == typ)
					 || !typ)  // use setting for this section
						GetVKSettings(sect, vkset);
				}
			}
		}
	}

	if (VKEnums2ProcCount) {
		for (long i = 1; i <= VKEnums2ProcCount; i++)
			ProcVKEnum((SSection *) VKEnums2Proc.find(i), VKEnumNames.find(i));
	}

	// check for sectionnameSet, and sectionnameSetHelp, whether VK or not
	// check sectionnameSetReplacements first
	// get the setting names and keys for SettingValues and the default value,
	// validate the keys and the existence of a TopicAlias
	for (dnl = AvailSectionNames.next; dnl; dnl = dnl->next) {
		setname = NewName("C", strlen(dnl->name) + 5);
		strcat(setname, dnl->name);
		outname = NewName(setname);
		strcat(setname, "set");
		dl = (DClist *) AvailSections.find(dnl->id);
		while (dl->next) {  // process each section with this name
			dl = dl->next;
			sect = (SSection *) dl->id;
			typ = sect->ifile->ityp;
			*setname = IniTypes.find(typ);
			GetFKSettings(sect, setname, false);
			if (SysIni->Section("SectionOutputs")
			 && (SysIni->Find(dnl->name)
			  || SysIni->Find(outname)))
				sect->formats = SysIni->StrRight();
		}
		DeleteName(outname);
		outname = NULL;
	}

	if (FKEnums2ProcCount) {
		for (long i = 1; i <= FKEnums2ProcCount; i++)
			ProcFKEnum((SSetting *) FKEnums2Proc.find(i), FKEnumNames.find(i));
	}


  if (SysIni->Section("Scopes")) {
		ScopeCount = 0;
		while (SysIni->NextItem()) {
			ScopeNames.add(SysIni->StrLeft(), ++ScopeCount);
			ScopeVals.add(SysIni->StrRight(), ScopeCount);
		}
  }

	SysIni->CloseFile();
	SysIni = NULL;
}


void CMgr::GetVKSettings(SSection *sect, char *vkset)
{
	char *spc = NULL;

	if (!vkset
	 || ((spc = strchr(vkset, ' ')) == NULL)
	 || (spc == vkset)) {
		LogEvent(logwarn, 1, "In [VKSections], cannot find type and value for section ", sect->name);
		return;
	}
	sect->vktypes = NewName(vkset, spc - vkset);
	sect->vkvals = NewName(spc + 1);
	sect->settyp = stvar;

	// add info from vktypes and vkvals
	long vidx = 0;

	if (*(sect->vktypes) == '*')  // special processing needed
		ProcVKSpecialSetting(sect, sect->vktypes + 1);
	else if ((vidx = SettingValueNames.getitem(sect->vktypes)) != 0)
		sect->vkshtext = SetValueHelpText.find(vidx);
	else
		LogEvent(logwarn, 1, "In [VKSections], cannot find type ", sect->vktypes, " for section ", sect->name);

	if (sect->vkvals[strlen(sect->vkvals) - 1] == '+') {
		sect->plus = true;
		//sect->vkvals[strlen(sect->vkvals) - 1] = '\0';
	}
	if (*(sect->vkvals) == '*')  // special processing needed
		ProcVKSpecialValue(sect, sect->vkvals + 1);
	else {
		if (sect->plus
		 && (SettingValueNames.getitem(sect->vkvals) == 0)
		 && _strnicmp(sect->vkvals, "E:", 2)) {
			LogEvent(logwarn, 1, "Using plus fallback for ", sect->vkvals);
			sect->vkvals[strlen(sect->vkvals) - 1] = '\0';
		}
		if ((vidx = SettingValueNames.getitem(sect->vkvals)) != 0)
			sect->vkvhtext = SetValueHelpText.find(vidx);
		else if (!_strnicmp(sect->vkvals, "E:", 2))  // see if enum
			VKEnums2Proc.add(sect, ++VKEnums2ProcCount);
		else
			LogEvent(logwarn, 1, "In [VKSections], cannot find value ", sect->vkvals, " for section ", sect->name);
	}
}


void CMgr::ProcVKSpecialSetting(SSection *sect, char *sname)
{
	bool plus = false;

	if (sname[strlen(sname) - 1] == '+')
		plus = true;

	// need to set sect->vkshtext
	if (!stricmp(sname, "HTMProp")) {
	}
	else {
		sect->vkshtext = "Special VK setting help needed here.";
		LogEvent(logwarn, 1, "In [VKSections], cannot process special type ", sect->vktypes, " for section ", sect->name);
	}
}


void CMgr::ProcVKSpecialValue(SSection *sect, char *val)
{
	// need to set sect->vkvhtext
	if (!strnicmp(val, "E:HTMProp", 9)) {  // different enums for char and para settings
		// create two different SEnums to hold them
		// add both to the SSection in vkelist, so vkecount == 2
		// at runtime, copy appropriate ptr to vkenum in ApplySettings
		VKEnums2Proc.add(sect, ++VKEnums2ProcCount);
		VKEnumNames.add("HTMPropPara", VKEnums2ProcCount);
		VKEnums2Proc.add(sect, ++VKEnums2ProcCount);
		VKEnumNames.add("HTMPropChar", VKEnums2ProcCount);
	}
	else {
		sect->vkvhtext = "Special VK value help needed here.";
		LogEvent(logwarn, 1, "In [VKSections], cannot process special value ",
		                     sect->vkvals, " for section ", sect->name);
	}
}



void CMgr::ProcVKEnum(SSection *sect, char *sectenum)
{
	// using name in vkvals, set vkenums and vkenumhtext for vkenumcount
	//if (sect->vkenumcount > 0)
	//	return;
	if (!sectenum)
		sectenum = sect->vkvals + 2;
	char *esect = NewName("Enum", strlen(sectenum) + 5);
	strcat(esect, sectenum);

	SEnum *se = GetEnumSet(esect, &(sect->vkvhtext), sect->plus);
	if (se == NULL) {
		LogEvent(logwarn, 1, "Enum section [", esect, "] not found for section ", sect->name);
		return;
	}

	// store ptr in sect
	if (sect->vkenum == NULL)  // first one, normally only one
		sect->vkenum = se;
	// handle case of multiple SEnums for different circumstances
	sect->vkelist.add((long) se);
	sect->vkecount++;
}


SEnum *CMgr::GetEnumSet(char *esect, char **htp, bool plus, SEnum *add)
{
	// see if already processed, if so store ptr and return
	SEnum *se = NULL;
	long pos = 0;
	DCnlist left;
	DCnlist right;
	long sidx = 0;
	long spos = 0;
	char *lstr = NULL;
	char *rstr = NULL;

	if (plus
	 && (esect[strlen(esect) - 1] == '+'))
		esect[strlen(esect) - 1] = '\0';
	//if (plus
	// && !SysIni->Section(esect)) {
	//	LogEvent(logwarn, 1, "Using plus fallback for ", esect);
	//	if (esect[strlen(esect) - 1] == '+')
	//		esect[strlen(esect) - 1] = '\0';
	//}

	if (add) {  // add content to calling enum
		if (!SysIni->Section(esect))
			return NULL;

		se = add;

		while (SysIni->NextItem()) {   // cache section contents
			left.add(SysIni->StrLeft(), ++sidx);
			right.add(SysIni->StrRight(), sidx);
		}

		for (spos = 1; spos <= sidx; spos++) {
			lstr = left.find(spos);
			rstr = right.find(spos);
			if (*lstr == '#') { // directive
				if (!stricmp(lstr + 1, "include")) {  // add set content
					GetEnumSet(rstr, NULL, false, se);
				}
				else
					LogEvent(logwarn, 1, "Unknown directive ", rstr, " in set ", esect);
				continue;
			}
			else {
				se->vals.add(lstr, ++(se->valcount));
				se->valhtext.add(rstr, se->valcount);
			}
		}
		return NULL;
	}

	if ((se = (SEnum *) AvailSEnums.getitem(esect)) == NULL) {
		if (!SysIni->Section(esect))  // no enum section
			return NULL;

		// found, has enums; create SEnum
		se = new SEnum(esect);
		AvailSEnums.add(esect, (long) se);

		if ((pos = EnumHelp.getitem(esect)) != 0) {
			se->vhtext = (char *) pos;
			if (plus) {
				se->vhtext = NewName(se->vhtext, strlen(se->vhtext) + 15);
				strcat(se->vhtext, " (one or more)");
			}
		}
		else if (plus)
			se->vhtext = "One or more of: ";

		while (SysIni->NextItem()) {   // cache section contents
			left.add(SysIni->StrLeft(), ++sidx);
			right.add(SysIni->StrRight(), sidx);
		}

		for (spos = 1; spos <= sidx; spos++) {
			lstr = left.find(spos);
			rstr = right.find(spos);
			if (*lstr == '#') { // directive
				if (!stricmp(lstr + 1, "include")) {  // add set content
					GetEnumSet(rstr, NULL, false, se);
				}
				else
					LogEvent(logwarn, 1, "Unknown directive ", rstr, " in set ", esect);
				continue;
			}
			else {
				se->vals.add(lstr, ++(se->valcount));
				se->valhtext.add(rstr, se->valcount);
			}
		}
	}

	if (se->vhtext
	 && htp)
		*htp = se->vhtext;

	return se;
}


void CMgr::GetFKSettings(SSection *sect, char *setname, bool suf)
{
	char *alias = NULL;
	char *sectname = NULL;
	char *newhelp = NULL;
	char *str = NULL;

	sectname = NewName(setname);
	sectname[strlen(sectname) - 3] = '\0';

	char *isect = suf ? "FKSuffixSets" : "FKSectionSets";
	if (SysIni->Section(isect)
	 && (SysIni->Find(sectname)
	  || SysIni->Find(sectname + 1))) {
		setname = SysIni->StrRight();
		newhelp = NewName(setname, strlen(setname) + 1);
		strcpy(newhelp + (strlen(setname) - 3), "Help");
		str = NewName(setname, strlen(setname) - 3);
		str[strlen(setname) - 3] = '\0';
		DeleteName(sectname);
		sectname = str;
	}

	alias = NewName("IDH_", strlen(sectname) + 5);
	strcat(alias, sectname);
	if (CheckAlias
	 && !HelpAlias.getitem(alias)) { // && sect->show  ??
		LogEvent(logwarn, 1, "Section alias \"", alias, "\" not in [HelpAlias]");
		strncpy(alias + 4, sectname + 1, strlen(sectname) - 1);
		alias[strlen(sectname) + 3] = '\0';
		if (!HelpAlias.getitem(alias))
			LogEvent(logwarn, 1, "Section alias \"", alias, "\" not in [HelpAlias]");
	}
	sect->alias = NewName(alias);
	sect->fkset = setname;
	ProcessFKSet(sect, setname, sectname, newhelp, false);
}


void CMgr::ProcessFKSet(SSection *sect, char *setname, char *sectname,
	                      char *newhelp, bool minus, bool setalias)
{
	char *helpname = NULL;
	char *str = NULL;
	SSetting *set = NULL;
	long idx = 0;
	bool show = true;
	DCnlist left;
	DCnlist right;
	long sidx = 0;
	long spos = 0;

	if (!SysIni->Section(setname)
	 && !SysIni->Section(setname + 1)) { // no FK set
		if ((sect->settyp == stnone)  // not VK either
		 && !CheckFKSuffix(sect, setname))
			LogEvent(logwarn, 1, "FK Section [", setname, "] not found");
	}
	else {  // found, has FK settings
		if (sect->settyp == stnone)
			sect->settyp = stfix;
		else if (sect->settyp == stvar)
			sect->settyp = stboth;
		if (!newhelp) {
			newhelp = NewName(setname, strlen(setname) + 1);
			strcpy(newhelp + (strlen(setname) - 3), "Help");
		}

		while (SysIni->NextItem()) {   // cache section contents
			left.add(SysIni->StrLeft(), ++sidx);
			right.add(SysIni->StrRight(), sidx);
		}

		char *fsect = NewName(setname, strlen(setname) + 5);
		strcpy(fsect + (strlen(setname) - 3), "Outputs");

		char *osect = NULL;
		if (setalias)
			osect = NewName(setname, strlen(setname) - 3);

		for (spos = 1; spos <= sidx; spos++) {
			str = left.find(spos);
			if (*str == '#') { // directive
				if (!stricmp(str + 1, "include")) {  // add set content
					ProcessFKSet(sect, right.find(spos), sectname, NULL, false);
				}
				else if (!stricmp(str + 1, "deprecated")) {  // add as deprecated
					ProcessFKSet(sect, right.find(spos), sectname, NULL, true, true);
				}
				else if (!stricmp(str + 1, "use")) {  // add with set alias
					ProcessFKSet(sect, right.find(spos), sectname, NULL, false, true);
				}
				else
					LogEvent(logwarn, 1, "Unknown directive ", str, " in set ", setname);
				continue;
			}
			if (str[strlen(str) - 1] == '-') {
				str[strlen(str) - 1] = '\0';
				show = false;
			}
			else
				show = !minus;

			sect->fksettings.add(set = new SSetting(str, sect),
						               ++(sect->fksetcount));
			sect->fksetnames.add(set->name, sect->fksetcount);

			if ((SysIni->Section(fsect)
			  || SysIni->Section(fsect + 1))
			 && SysIni->Find(set->name))
				set->formats = SysIni->StrRight();

			// add info from fkvals
			set->plus = false;
			set->show = show;
			if (sect->alias
			 && set->name) {  // && set->show ?
				char *sp = NULL;
				if (setalias) {  // included from setname, use orig alias
					char *orig = NewName("IDH_", strlen(osect) + strlen(set->name) + 5);
					strcat(orig, osect);
					strcat(orig, "_");
					strcat(orig, set->name);
					for (sp = orig; *sp; sp++) {
						if (*sp == ' ')
							*sp = '_';
					}
					set->alias = NewName(orig);
					DeleteName(orig);
				}
				else {
					char *alias = NewName(sect->alias, strlen(sect->alias) + strlen(set->name) + 2);
					strcat(alias, "_");
					strcat(alias, set->name);
					char *sp = alias;
					for (sp = alias; *sp; sp++) {
						if (*sp == ' ')
							*sp = '_';
					}
					if (CheckAlias
					 && !HelpAlias.getitem(alias))
						LogEvent(logwarn, 1, "Setting alias \"", alias, "\" not in [HelpAlias]");
					set->alias = NewName(alias);
					DeleteName(alias);
				}
			}
			if ((set->fkvals = right.find(spos)) != NULL) {
				str = strchr(set->fkvals, ' ');
				if (str) {
					*str++ = '\0';
					if (!strncmp(str, "V:", 2) // look up V: defaults
					 && ((idx = SettingValueNames.getitem(str + 2)) != 0))
						set->dval = SetValueHelpText.find(idx);
					else
						set->dval = NewName(str);
				}
				if (set->fkvals[strlen(set->fkvals) - 1] == '+')
					set->plus = true;
				if (*(set->fkvals) == '*')  // special processing
					ProcFKSpecialValue(set, set->fkvals + 1);
				else {
					if (set->plus
					 && (SettingValueNames.getitem(set->fkvals) == 0)
					 && _strnicmp(set->fkvals, "E:", 2)) {
						LogEvent(logwarn, 1, "Using plus fallback for ", set->fkvals);
						set->fkvals[strlen(set->fkvals) - 1] = '\0';
					}
					if ((idx = SettingValueNames.getitem(set->fkvals)) != 0) {
						set->vhtext = SetValueHelpText.find(idx);
						if (set->plus) {
							set->vhtext = NewName(set->vhtext, strlen(set->vhtext) + 15);
							strcat(set->vhtext, " (one or more)");
						}
					}
					else if (!_strnicmp(set->fkvals, "E:", 2)) {  // see if enum
						FKEnums2Proc.add(set, ++FKEnums2ProcCount);
					}
					else {  // not an enum
						LogEvent(logwarn, 1, "In [", sect->fkset, "], cannot find value ", set->fkvals);
						LogEvent(logcont, 1, " for setting ", set->name, " in section ", sect->name);
					}
				}
			}
		}
		DeleteName(osect);
		DeleteName(fsect);
	}

	helpname = NewName(sectname, strlen(sectname) + 5);
	strcat(helpname, "Help");
	if (!SysIni->Section(helpname)
	 && !SysIni->Section(helpname + 1)
	 && !(newhelp
	  && SysIni->Section(newhelp))) { // no FK set
		if ((sect->settyp == stnone)  // not VK either
		 && !CheckFKSuffix(sect, helpname, true))
			LogEvent(logwarn, 1, "In [", sect->fkset, "], cannot find [", helpname, "]");
	}
	else {  // found, has FK help
		while (SysIni->NextItem()) {
			if (((str = SysIni->StrLeft()) != NULL)
			 && ((idx = sect->fksetnames.getitem(str)) != 0)
			 && ((set = (SSetting *) sect->fksettings.find(idx)) != NULL))
				set->htext = SysIni->StrRight();
		}
	}
	DeleteName(helpname);
}


void CMgr::ProcFKSpecialValue(SSetting *set, char *val)
{
	// need to set set->vhtext
	if (!strnicmp(val, "E:HTMProp", 9)) {
		if (set->plus)
			;
		else
			;
	}
	else {
		set->vhtext = "Special FK value help needed here.";
		LogEvent(logwarn, 1, "In [", set->sect->fkset, "], cannot process special value ", set->fkvals);
		LogEvent(logcont, 1, " for setting ", set->name, " in section ", set->sect->name);
	}
}


bool CMgr::CheckFKSuffix(SSection *sect, char *setname, bool help)
{
	// Set or Help not found, see if could be Suffix member
	// with a SectionHelp override setting

	SIni *sufini = (SIni *) AvailSuffixIniFiles.find((long) sect->ifile->ityp);
	if (!sufini)
		return false;

	char *sname = NewName(setname);
	long len = strlen(sname);
	if (len < 5)
		return false;
	if (!help && !stricmp(sname + (len - 3), "Set"))
		sname[len - 3] = '\0';
	else if (help && !stricmp(sname + (len - 4), "Help"))
		sname[len - 4] = '\0';

	long pos = 0;
	SSection *sufsect = NULL;
	if (((pos = sufini->sectnames.checksuffix(sname)) == 0)
	 ||  ((sufsect = (SSection *) sufini->sections.find(pos)) == NULL))
		return false;

	long idx = 0;
	long nidx = 0;
	SSetting *sufset = NULL;
	SSetting *nset = NULL;
	char *str = NULL;
	if (!help && sufsect->fksetcount) { // copy fk settings
		if (sect->settyp == stnone)
			sect->settyp = stfix;
		else if (sect->settyp == stvar)
			sect->settyp = stboth;
		sect->fkset = setname;
		for (idx = 1; idx <= sufsect->fksetcount; idx++) {
			sufset = (SSetting *) sufsect->fksettings.find(idx);
			sect->fksettings.add(nset = new SSetting(*sufset), idx);
			nset->sect = sect;
			sect->fksetnames.add(nset->name, idx);
		}
		sect->fksetcount = idx;
	}
	else if (help) {  // copy help
		for (idx = 1; idx <= sufsect->fksetcount; idx++) {
			str = sufsect->fksetnames.find(idx);
			if (((nidx = sect->fksetnames.getitem(str)) != 0)
			 && ((nset = (SSetting *) sect->fksettings.find(nidx)) != NULL)
			 && ((sufset = (SSetting *) sufsect->fksettings.find(idx)) != NULL)
			 && !nset->htext)
				nset->htext = sufset->htext;
		}
	}

	return true;  // section found, all set
}


void CMgr::ProcFKEnum(SSetting *set, char *setenum)
{
	char *vname = NULL;
	char *vhname = NULL;
	long pos = 0;
	long vcount = 0;

	// using name in fkvals, set vals and valhtext for valcount
	if (!setenum)
		setenum = set->fkvals + 2;
	char *eset = NewName("Enum", strlen(setenum) + 5);
	strcat(eset, setenum);

	SEnum *se = GetEnumSet(eset, &(set->vhtext), set->plus);
	if (se == NULL) {
		LogEvent(logwarn, 1, "Enum section [", eset, "] not found for setting ", set->name);
		LogEvent(logcont, 1, " in FK section ", set->sect->name);
		return;
	}

	// store ptr in set
	if (set->fkenum == NULL)  // first one, normally only one
		set->fkenum = se;
	else  // handle case of multiple SEnums for different circumstances
		set->fkelist.add((long) se);
	set->fkecount++;


#if 0
	if (set->plus
	 && !SysIni->Section(eset)) {
		LogEvent(logwarn, 1, "Using plus fallback for ", eset);
		eset[strlen(eset) - 1] = '\0';
	}

	if (!SysIni->Section(eset)) { // no enum section
		LogEvent(logwarn, 1, "Enum section [", eset, "] not found for setting ", set->name);
		LogEvent(logcont, 1, " in FK section ", set->sect->name);
	}
	else {  // found, has enums
		if ((pos = EnumHelp.getitem(eset)) != 0)
			set->vhtext = (char *) pos;
		if (set->plus) {
			if (set->vhtext) {
				set->vhtext = NewName(set->vhtext, strlen(set->vhtext) + 15);
				strcat(set->vhtext, " (one or more)");
			}
			else
				set->vhtext = "One or more of: ";
		}

		while (SysIni->NextItem()) {
			vname = SysIni->StrLeft();
			vhname = SysIni->StrRight();
			vcount = set->valcount;
			if ((pos = set->vals.getitem(vname)) == 0) {
				set->vals.add(vname, ++vcount);
				set->valhtext.add(vhname, vcount);
				set->valcount = vcount;
			}
			else if ((set->valhtext.find(pos) == NULL)
			 && vhname)
				set->valhtext.add(vhname, pos);
		}
	}
#endif
}



// Start panel functions

bool CMgr::FilePanelEnabled = true;

// project selection

DCirfile *CMgr::ProjIni = NULL;

// **** move this to local\manager in next PM revision

DCnlist CMgr::ProjFormatNameList;
DCnlist CMgr::ProjIniNameList;
long CMgr::ProjFormCount = 0;

DCnlist CMgr::FormatCodeKeys;
DCnlist CMgr::FormatCodes;
long CMgr::CodesCount = 0;
char CMgr::FormatCode = '\0';

DCnlist CMgr::ProjNameList;
DCilist CMgr::ProjNumList;
DCnlist CMgr::ProjFormatList;
DCnlist CMgr::ProjSrcList;
DCnlist CMgr::ProjDirList;
long CMgr::ProjCount = 0;

#ifdef _M2GCM
DCnlist CMgr::ProjFmBookList;
DCnlist CMgr::ProjFmChapList;
long CMgr::NextProjID = 1;
#endif


void CMgr::SetProjIni(void)
{
	ProjIni = new DCirfile(ProjIniName);
  if (!ProjIni)
    return;

  if (ProjIni->FErr() == fok) {

		if (ProjIni->Section("Active")) {
			while (ProjIni->NextItem()) {
				ProjNameList.add(ProjIni->StrLeft(), ++ProjCount);
				ProjNumList.add(ProjIni->IntRight(), ProjCount);
			}
		}

		if (ProjIni->Section("OutputType")) {
			long projnum = 0;
			while (ProjIni->NextItem()) {
				projnum = atoi(ProjIni->StrLeft());
				if (ProjNumList(projnum))
					ProjFormatList.add(ProjIni->StrRight(), projnum);
			}
		}

		if (ProjIni->Section("SourceDir")) {
			long projnum = 0;
			while (ProjIni->NextItem()) {
				projnum = atoi(ProjIni->StrLeft());
				if (ProjNumList(projnum))
					ProjSrcList.add(ProjIni->StrRight(), projnum);
			}
		}

		if (ProjIni->Section("OutputDir")) {
			long projnum = 0;
			while (ProjIni->NextItem()) {
				projnum = atoi(ProjIni->StrLeft());
				if (ProjNumList(projnum))
					ProjDirList.add(ProjIni->StrRight(), projnum);
			}
		}

#ifdef _M2GCM
		if (ProjIni->Section("Projects")) {
			if (ProjIni->Find("NextID"))
				NextProjID = ProjIni->IntRight();
		}

		if (ProjIni->Section("FrameBook")) {
			long projnum = 0;
			while (ProjIni->NextItem()) {
				projnum = atoi(ProjIni->StrLeft());
				if (ProjNumList(projnum))
					ProjFmBookList.add(ProjIni->StrRight(), projnum);
			}
		}

		if (ProjIni->Section("FrameChapter")) {
			long projnum = 0;
			while (ProjIni->NextItem()) {
				projnum = atoi(ProjIni->StrLeft());
				if (ProjNumList(projnum))
					ProjFmChapList.add(ProjIni->StrRight(), projnum);
			}
		}
#endif

  }
	//else {
  ProjIni->CloseFile();
  ProjIni = NULL;
	//}
}


void ConfigMgr::SetUpProjList( void )
{
	// load choices from ProjNameList
	wxArrayString pnames(0);  // not sorted
	for (unsigned int i = 1; i <= (unsigned int) CMgr::ProjCount; i++)
		pnames.Add(wxString(CMgr::ProjNameList.find(i)));
	m_projnamesel->Append(pnames);
}


void ConfigMgr::ProcProjListChoice(int idx)
{
	if (idx < 1)
		return;

	long projnum = CMgr::ProjNumList.find(idx);
	char *srcdir = CMgr::ProjSrcList.find(projnum);
	char *outdir = CMgr::ProjDirList.find(projnum);
	char *fmt = CMgr::ProjFormatList.find(projnum);
	long fnum = CMgr::ProjFormatNameList.getitem(fmt);
	char *ini = CMgr::ProjIniNameList.find(fnum);

	// outdir + '\' + ini = start
	CMgr::StartFileName = NewName(outdir,
	 strlen(outdir) + strlen(ini) + 2);
	strcat(CMgr::StartFileName, "\\");
	strcat(CMgr::StartFileName, ini);
	ProjSrcDir = wxString(srcdir);
	ProjOutDir = wxString(outdir);

	if (!CMgr::FileExists(CMgr::StartFileName)) { // announce the error
		SetStartFileText("Cannot find project start file.");
		DeleteName(CMgr::StartFileName);
		CMgr::StartFileName = NULL;
	}
}


// command line process

bool CMgr::CmdLineStart = false;
bool CMgr::CmdLineCat = false;
bool CMgr::CmdLineAct = false;
char *CMgr::SelectedAction = NULL;
DCnlist CMgr::Actions;
npair CMgr::ActInit[] = {
	{1, "AddSect"},
	{2, "AddVNSect"},
	{3, "AddSet"},
	{4, "AddVKSet"},
	{5, "AddBKSet"},
	{6, "EditSect"},
	{7, "EditSet"},
	{8, "DelSect"},
	{9, "DelSet"},
	{10, "RestSect"},
	{11, "RestSet"},
	{12, "RenSect"},
	{13, "RenSet"},
	{14, "MovSect"},
	{15, "MovSet"},
	{16, "MovFSect"},
	{17, "MovFSet"},
	{18, "CopySect"},
	{19, "CopySet"},
	{20, "MergeSect"},
	{21, "MergeSet"},
	NULL
};

bool CMgr::CmdLineSect = false;
bool CMgr::CmdLineSet = false;
bool CMgr::CmdLineIni = false;


void ConfigMgr::SetCommandLineSwitches(void)
{
	long idx = 0;
	wxTreeItemId id;

	// read switch values from D2gcmApp such as StartFile
	if (!D2gcmApp::StartFile.IsEmpty()) {
		CMgr::StartFileName = NewName(D2gcmApp::StartFile.c_str());
		CMgr::CmdLineStart = true;
		SetStartFileText(CMgr::StartFileName);
	}
	if (!D2gcmApp::Category.IsEmpty()) {
		CMgr::SelectedCategory = NewName(D2gcmApp::Category.c_str());
		idx = CMgr::CatNameList.getitem(CMgr::SelectedCategory);
		id = CMgr::CatIDList.find(idx);
		if (!D2gcmApp::SectMatch.IsEmpty())
			CMgr::SelectedSection = NewName(D2gcmApp::SectMatch.c_str());
		if (!D2gcmApp::SetMatch.IsEmpty())
			CMgr::SelectedSetting = NewName(D2gcmApp::SetMatch.c_str());
		CMgr::CmdLineCat = true;
	}
	if (!D2gcmApp::Action.IsEmpty()) {
		CMgr::SelectedAction = NewName(D2gcmApp::Action.c_str());
		CMgr::Actions.init(CMgr::ActInit);
		idx = CMgr::Actions.getitem(CMgr::SelectedAction);
		CMgr::CmdLineAct = true;
	}
	if (!D2gcmApp::Section.IsEmpty()) {
		SectionSelected = D2gcmApp::Section;
		CMgr::CmdLineSect = true;
	}
	if (!D2gcmApp::Setting.IsEmpty()) {
		SettingNameShown = D2gcmApp::Setting;
		CMgr::CmdLineSet = true;
	}
	if (!D2gcmApp::IniFileSrc.IsEmpty()) {
		SrcIniName = D2gcmApp::IniFileSrc;
		CMgr::CmdLineIni = true;
	}
	if (!D2gcmApp::IniFileDest.IsEmpty()) {
		DestIniName = D2gcmApp::IniFileDest;
		CMgr::CmdLineIni = true;
	}

	if (CMgr::CmdLineStart) {
		ApplyFiles();
		if (CMgr::CmdLineCat) {
			m_sectyptree->SelectItem(id);
			ApplyCategories();
			CMgr::CmdLineCat = false;
			if (CMgr::CmdLineAct) {
				CMgr::CurrAct = (enum actyp) idx;
				SetActButton();
				ApplyAction();
				if (CMgr::CmdLineSect) {
					SetSectButtons(SectionSelectedPos);
					if (SectionSelected == D2gcmApp::Section) {
						ApplySections();
						if (CMgr::CmdLineSet
						 && (SettingNameShown == D2gcmApp::Setting)) {
							ApplySettings();
							if (CMgr::CmdLineIni) {
								if (CMgr::SelectingSrcIni
								 && (SrcIniName == IniNameShown))
									ApplyIniVals();
								if (!CMgr::SelectingSrcIni
								 && (DestIniName == IniNameShown))
									ApplyIniVals();
							}
						}
					}
				}
			}
		}

#ifdef _M2GCM
		else {  // no cat selected, must be from m2rbook
			CMgr::VerifyProjectFileEntry(CMgr::StartFileName);
		}
#endif
	}
}


#ifdef _M2GCM
void CMgr::VerifyProjectFileEntry(char *sfile)
{
	char *namepos = NULL;
	long pos = 0;


	// get [OutputDir] and [OutputType] from sfile
	char *ininame = NULL;
	char *outdir = NULL;
	char *outtyp = NULL;

	if ((namepos = strrchr(sfile, '\\')) != NULL) {
		ininame = NewName(namepos + 1);
		outdir = NewName(sfile, namepos - sfile);
	}
	else {
		ininame = NewName(sfile);
		outdir = NewName("");
	}

	if ((pos = ProjIniNameList.getitem(ininame)) == 0) {
		LogEvent(logwarn, 1, "Unrecognized ini type: ", ininame);
		outtyp = NewName("");
	}
	else 
		outtyp = ProjFormatNameList.find(pos);


	// in [Setup], read ProjectName, FrameBook, FrameChapter
	// get [SourceDir] from FrameChapter or FrameBook,

	char *srcdir = NULL;
	char *projname = NULL;
	char *fmbook = NULL;
	char *fmchap = NULL;

	char vbuf[1024];

	*vbuf = '\0';
	DCpfile::getval("Setup", "FrameBook", "", vbuf, 1024, sfile);
	fmbook = NewName(vbuf);

	*vbuf = '\0';
	DCpfile::getval("Setup", "FrameChapter", "", vbuf, 1024, sfile);
	fmchap = NewName(vbuf);

	*vbuf = '\0';
	DCpfile::getval("Setup", "ProjectName", "", vbuf, 1024, sfile);
	projname = NewName(vbuf);

	DCpfile::pclose(sfile);

	if (fmchap 
	 && ((namepos = strrchr(fmchap, '\\')) != NULL))
		srcdir = NewName(fmchap, namepos - fmchap);
	else if (fmbook 
	 && ((namepos = strrchr(fmbook, '\\')) != NULL))
		srcdir = NewName(fmbook, namepos - fmbook);
	else
		srcdir = NewName(outdir);

	DCpfile::putval("Projects", "LastActive", projname, ProjIniName);

	// If all these match a projects.ini entry, done.
	char *str = NULL;

	if (((pos = ProjNameList.getitem(projname)) != 0)  // matching name
	 && ((str = ProjFormatList.find(pos)) != NULL)
	 && !stricmp(outtyp, str)                          // matching type
	 && ((str = ProjSrcList.find(pos)) != NULL)
	 && !stricmp(srcdir, str)                          // matching srcdir
	 && ((str = ProjDirList.find(pos)) != NULL)
	 && !stricmp(outdir, str)                          // matching outdir
	 && ((!fmbook
	   && !ProjFmBookList.find(pos))
	  || (fmbook
	   && ((str = ProjFmBookList.find(pos)) != NULL)
	   && !stricmp(fmbook, str)))                      // matching book
	 && ((!fmchap
	  && !ProjFmChapList.find(pos))
	  || (fmchap
	   && ((str = ProjFmChapList.find(pos)) != NULL)
	   && !stricmp(fmchap, str))))                     // matching chapter
		return;  // full match

	// Otherwise make new entry, with new name if necessary.
	if ((pos = ProjNameList.getitem(projname)) != 0) { // name already used
		LogEvent(logwarn, 1, "Cannot add ", projname, " to projects.ini; name already used.");
		LogEvent(logcont, 1, "Change the name in the Frame .prj to a more specific one.");
		return;
	}

	long nextid = NextProjID + 1;
	char numval[10];
	ltoa(nextid, numval, 10);
	DCpfile::putval("Projects", "NextID", numval, ProjIniName);

	ltoa(NextProjID, numval, 10);
 
	DCpfile::putval("Active", projname, numval, ProjIniName);
	DCpfile::putval("OutputType", numval, outtyp, ProjIniName);
	DCpfile::putval("SourceDir", numval, srcdir, ProjIniName);
	DCpfile::putval("OutputDir", numval, outdir, ProjIniName);
	DCpfile::putval("FrameBook", numval, fmbook, ProjIniName);
	DCpfile::putval("FrameChapter", numval, fmchap, ProjIniName);

	DCpfile::pclose(ProjIniName);
}


#endif


void ConfigMgr::SetActButton(void)
{
	switch (CMgr::CurrAct) {
		case actadd:
			m_sectaddbutton->SetValue(true);
			break;

		case actaddv:
			m_sectvnaddbutton->SetValue(true);
			break;

		case actadds:
			m_sectaddsetbutton->SetValue(true);
			break;

		case actedit:
			m_sectchangebutton->SetValue(true);
			break;

		case actedits:
			m_sectseteditbutton->SetValue(true);
			break;

		case actdel:
			m_sectdelbutton->SetValue(true);
			break;

		case actdels:
			m_sectdelsetbutton->SetValue(true);
			break;

		case actrest:
			m_sectrestbutton->SetValue(true);
			break;

		case actrests:
			m_sectrestsetbutton->SetValue(true);
			break;

		case actrenv:
			m_sectrenbutton->SetValue(true);
			break;

		case actrensv:
			m_sectrensetbutton->SetValue(true);
			break;

		case actmov:
			m_sectmovfbutton->SetValue(true);
			break;

		case actmovs:
			m_sectmovsetbutton->SetValue(true);
			break;

		case actmovf:
			m_sectmovfbutton->SetValue(true);
			break;

		case actmovsf:
			m_sectmovsetfbutton->SetValue(true);
			break;

		case actcpy:
			m_sectcpybutton->SetValue(true);
			break;

		case actcpys:
			m_sectcpysetfbutton->SetValue(true);
			break;

		case actdup:
			m_sectdupbutton->SetValue(true);
			break;

		case actdups:
			m_sectdupsetbutton->SetValue(true);
			break;

		default:
			break;
	}
}


// start template selection


DCnlist CMgr::StartFormatList;
DCnlist CMgr::StartNameList;
long CMgr::StartCount = 0;
bool CMgr::IncludeStarts = false;
bool CMgr::NeedLocalStart = false;


void ConfigMgr::SetUpStartsList( void )
{
	// load choices from StartFormatList
	//for (unsigned int i = 1; i <= (unsigned int) CMgr::StartCount; i++)
	//	m_projstarttplsel->SetString(i, CMgr::StartFormatList.find(i));
	wxArrayString snames(0);  // not sorted
	for (unsigned int i = 1; i <= (unsigned int) CMgr::StartCount; i++)
		snames.Add(wxString(CMgr::StartFormatList.find(i)));
	m_projstarttplsel->Append(snames);
}


void ConfigMgr::ProcStartsListChoice(int idx)
{
	// if selected filename in StartNameList not in local\starts,
	// copy it from system\starts to local\starts and open it there
	if (idx < 1)
		return;
	char *fname = CMgr::StartNameList.find(idx);
	char *cmd = NULL;
	CMgr::StartFileName = NewName(CMgr::LocalStartDir,
	 strlen(CMgr::LocalStartDir) + strlen(fname) + 1);
	strcat(CMgr::StartFileName, fname);
	if (!CMgr::FileExists(CMgr::StartFileName)) {
		// copy fron SystemStartDir
		cmd = NewName("copy /Y ", strlen(CMgr::SystemStartDir)
		 + strlen(fname) + strlen(CMgr::StartFileName) + 10);
		strcat(cmd, CMgr::SystemStartDir);
		strcat(cmd, fname);
		strcat(cmd, " ");
		strcat(cmd, CMgr::StartFileName);
		::RunDOSCommand(cmd, NULL, true, false, true);
	}
	ProjSrcDir = wxString(CMgr::SystemStartDir);
	ProjOutDir = wxString(CMgr::SystemStartDir);

	if (!CMgr::FileExists(CMgr::StartFileName)) { // announce the error
		SetStartFileText("Cannot find start template file.");
		DeleteName(CMgr::StartFileName);
		CMgr::StartFileName = NULL;
	}
}


// apply project


void ConfigMgr::SetIniChoice(void)
{
	m_projstarttext->Clear();
	if (CMgr::StartFileName) {
		DeleteName(CMgr::StartFileName);
		CMgr::StartFileName = NULL;
	}

	if (m_projnamebutton->GetValue()) {  // projects selected
		ProcProjListChoice(m_projnamesel->GetSelection());
	}
	else if (m_projstarttplbutton->GetValue()) {  // starts selected
		ProcStartsListChoice(m_projstarttplsel->GetSelection());
	}
	else if (m_filebrowselbutton->GetValue()) {  // browser selected
		CMgr::StartFileName = NewName((m_projdirctrl->GetFilePath()).c_str());
		CMgr::SystemStartDir = NewName((m_projdirctrl->GetPath()).c_str());
		ProjSrcDir = wxString(CMgr::SystemStartDir);
		ProjOutDir = wxString(CMgr::SystemStartDir);
	}

	if (!CMgr::StartFileName)
		return;

	SetStartFileText(CMgr::StartFileName);

#if 0
	// report bug for SetPath ****

	char *sf = NewName(CMgr::StartFileName);
	char *sfp = sf;

	sfp = strrchr(sf, '\\');
	if (sfp
	 && (*sfp == '\\'))
		*sfp = '\0';

	for (sfp = sf; *sfp; sfp++) {
		if (*sfp == '\\')
			*sfp = '/';
	}
	
	StartFilePath = wxT(sf);

	m_projdirctrl->Enable();
	m_projdirctrl->SetPath(StartFilePath);
#endif

}


void ConfigMgr::SetStartFileText(char *msg)
{
	if (!msg
	 || !*msg)
		return;

	m_projstarttext->Clear();
	m_projstarttext->WriteText(msg);

	m_catstarttext->Clear();
	m_catstarttext->WriteText(msg);

	m_actstarttext->Clear();
	m_actstarttext->WriteText(msg);

	m_sectstarttext->Clear();
	m_sectstarttext->WriteText(msg);

	m_setstarttext->Clear();
	m_setstarttext->WriteText(msg);

	m_valstarttext->Clear();
	m_valstarttext->WriteText(msg);

	m_vselstarttext->Clear();
	m_vselstarttext->WriteText(msg);
}


void ConfigMgr::ApplyFiles(void)
{
	if (!CMgr::StartFileName) {
		m_projresult->Clear();
		m_projresult->WriteText("Select start file before proceeding.");
		CMgr::CatPanelEnabled = false;
		CMgr::SectPanelEnabled = false;
		CMgr::SetPanelEnabled = false;
		CMgr::ValPanelEnabled = false;
		CMgr::VselPanelEnabled = false;
		return;
	}

	char *nstart = strrchr(CMgr::StartFileName, '\\');
	long pos = 0;
	char *fmt = NULL;
#ifdef _M2GCM
	char vbuf[256];
#endif

	if (!nstart)
		nstart = CMgr::StartFileName;
	if (*nstart == '\\')
		nstart++;

	if (((pos = CMgr::ProjIniNameList.getitem(nstart)) != 0)
	 && ((fmt = CMgr::ProjFormatNameList.find(pos)) != NULL)
	 && ((pos = CMgr::FormatCodeKeys.getitem(fmt)) != 0))
		CMgr::FormatCode = *CMgr::FormatCodes.find(pos);
#ifdef _M2GCM
	else if (stricmp(nstart, "mif2htm.ini")) {
		*vbuf = '\0';
		DCpfile::getval("Options", "Output", "", vbuf, 256, CMgr::StartFileName);
		if (*vbuf)
			CMgr::FormatCode = *vbuf;
		else
			CMgr::FormatCode = 'N';
		DCpfile::pclose(CMgr::StartFileName);
	}
	else if (stricmp(nstart, "mif2rtf.ini")) {
		*vbuf = '\0';
		DCpfile::getval("Options", "Output", "", vbuf, 256, CMgr::StartFileName);
		if (*vbuf)
			CMgr::FormatCode = *vbuf;
		else
			CMgr::FormatCode = 'W';
		DCpfile::pclose(CMgr::StartFileName);
	}
#endif
	else
		CMgr::FormatCode = '\0';



	SetStartFileText(CMgr::StartFileName);

	m_projresult->Clear();
	m_projresult->WriteText("Start file selected.");
	CMgr::CatPanelEnabled = true;
	m_catresult->Clear();
	(GetBookCtrl())->ChangeSelection((size_t) pncat);
}



// Category panel


// category selection

bool CMgr::CatPanelEnabled = true;
DCnlist CMgr::CatNameList;
DCilist CMgr::CatLevelList;
DCvlist CMgr::CatIDList;
DCvlist CMgr::CatParentList;
wxTreeItemId CMgr::CatParents[16];

unc CMgr::CategoryCode = '\0';
char *CMgr::SelectedCategory = NULL;
char *CMgr::TopCategory = NULL;
long CMgr::CatCount = 0;
long CMgr::CatNum = 0;
long CMgr::SelCatLevel = 0;
bool CMgr::SelAllTopCat = true;
inityp CMgr::SelIniTyp = inicfg;
DCilist CMgr::CatParentNums;
DCilist CMgr::CatParentLevels;

char *CMgr::ConfigsCat = "Configs";
char *CMgr::ParaCharFormatsCat = "ParaCharFormats";
char *CMgr::TableFormatsCat = "TableFormats";
char *CMgr::SubformatsCat = "Subformats";
char *CMgr::PageFormatsCat = "PageFormats";
char *CMgr::LanguagesCat = "Languages";
char *CMgr::MacrosCat = "Macros";
char *CMgr::ContentModelsCat = "ContentModels";


void ConfigMgr::SelectIniType( void )
{
	if (m_catinitypebutton->GetValue() == 0) {
		if (m_catselbutton->GetValue())
			SelectCategories();
		return;
	}

	CMgr::UseConfigs = false;
	CMgr::UseFormats = false;
	CMgr::UseLangs = false;
	CMgr::UseMacros = false;
	CMgr::UseModels = false;
	CMgr::Usage = 0;
	CMgr::CategoryCode = 'C';
	CatAlias = wxString("IDH_ZSelectCategory");

	int pos = m_catinichoice->GetCurrentSelection();
	switch (pos) {
		case 0: // configs
			CMgr::TopCategory = CMgr::ConfigsCat;
			CMgr::UseConfigs = true;
			CMgr::Usage += 1;
			CMgr::CategoryCode = 'C';
			CMgr::SelIniTyp = inicfg;
			CatAlias = wxString("IDH_ZConfigs");
			break;

#ifdef _M2GCM
		case 1: // macros
			CMgr::TopCategory = CMgr::MacrosCat;
			CMgr::UseMacros = true;
			CMgr::Usage += 8;
			CMgr::CategoryCode = 'M';
			CMgr::SelIniTyp = inimac;
			CatAlias = wxString("IDH_ZMacros");
			break;

		case 2: // content models
			CMgr::TopCategory = CMgr::ContentModelsCat;
			CMgr::UseModels = true;
			CMgr::Usage += 256;
			CMgr::CategoryCode = 'D';
			CMgr::SelIniTyp = inimod;
			CatAlias = wxString("IDH_ZContentModels");
			break;

#else
		case 1: // text formats
			CMgr::TopCategory = CMgr::ParaCharFormatsCat;
			CMgr::UseFormats = true;
			CMgr::Usage += 16;
			CMgr::CategoryCode = 'F';
			CMgr::SelIniTyp = inifmt;
			CatAlias = wxString("IDH_ZParaCharFormats");
			break;

		case 2: // table formats
			CMgr::TopCategory = CMgr::TableFormatsCat;
			CMgr::UseFormats = true;
			CMgr::Usage += 32;
			CMgr::CategoryCode = 'T';
			CMgr::SelIniTyp = initbl;
			CatAlias = wxString("IDH_ZTableFormats");
			break;

		case 3: // page formats
			CMgr::TopCategory = CMgr::PageFormatsCat;
			CMgr::UseFormats = true;
			CMgr::Usage += 128;
			CMgr::CategoryCode = 'P';
			CMgr::SelIniTyp = inipage;
			CatAlias = wxString("IDH_ZPageFormats");
			break;

		case 4: // subformats
			CMgr::TopCategory = CMgr::SubformatsCat;
			CMgr::UseFormats = true;
			CMgr::Usage += 64;
			CMgr::CategoryCode = 'S';
			CMgr::SelIniTyp = inisub;
			CatAlias = wxString("IDH_ZSubformats");
			break;

		case 5: // langs
			CMgr::TopCategory = CMgr::LanguagesCat;
			CMgr::UseLangs = true;
			CMgr::Usage += 4;
			CMgr::CategoryCode = 'L';
			CMgr::SelIniTyp = inilang;
			CatAlias = wxString("IDH_ZLanguages");
			break;

		case 6: // macros
			CMgr::TopCategory = CMgr::MacrosCat;
			CMgr::UseMacros = true;
			CMgr::Usage += 8;
			CMgr::CategoryCode = 'M';
			CMgr::SelIniTyp = inimac;
			CatAlias = wxString("IDH_ZMacros");
			break;

		case 7: // content models
			CMgr::TopCategory = CMgr::ContentModelsCat;
			CMgr::UseModels = true;
			CMgr::Usage += 256;
			CMgr::CategoryCode = 'D';
			CMgr::SelIniTyp = inimod;
			CatAlias = wxString("IDH_ZContentModels");
			break;
#endif
		default:
			CMgr::TopCategory = CMgr::ConfigsCat;
			CMgr::UseConfigs = true;
			CMgr::Usage += 1;
			CMgr::CategoryCode = 'C';
			CMgr::SelIniTyp = inicfg;
			CatAlias = wxString("IDH_ZConfigs");
			break;
	}

	CMgr::SelectedCategory = CMgr::TopCategory;
	CMgr::SelCatLevel = 1;
	CMgr::SelAllTopCat = true;

	wxString str = wxString(MakeShowName(CMgr::SelectedCategory));
	m_sectcattext->Clear();
	m_sectcattext->WriteText(str);
	m_setcattext->Clear();
	m_setcattext->WriteText(str);
	m_valcattext->Clear();
	m_valcattext->WriteText(str);
	m_vselcattext->Clear();
	m_vselcattext->WriteText(str);
}


void ConfigMgr::SetUpCategoriesTree( void )
{
	// set up ini types
	m_catinichoice->SetString(0, MakeShowName(CMgr::ConfigsCat));

#ifdef _M2GCM
	m_catinichoice->SetString(1, MakeShowName(CMgr::MacrosCat));
	m_catinichoice->SetString(2, MakeShowName(CMgr::ContentModelsCat));

#else
	m_catinichoice->SetString(1, MakeShowName(CMgr::ParaCharFormatsCat));
	m_catinichoice->SetString(2, MakeShowName(CMgr::TableFormatsCat));
	m_catinichoice->SetString(3, MakeShowName(CMgr::PageFormatsCat));
	m_catinichoice->SetString(4, MakeShowName(CMgr::SubformatsCat));
	m_catinichoice->SetString(5, MakeShowName(CMgr::LanguagesCat));
	m_catinichoice->SetString(6, MakeShowName(CMgr::MacrosCat));
	m_catinichoice->SetString(7, MakeShowName(CMgr::ContentModelsCat));
#endif

	m_catinichoice->SetSelection(0);


  // set root item as level 0; not in CatLevelList since 0 is "not found"
  wxTreeItemId rootId = m_sectyptree->AddRoot(wxT("All"), 0, 0,
                          new MyTreeItemData(wxT("All")));

    // append all list members and build CatIDList
	wxTreeItemId itemId = 0;
	wxTreeItemId parentId = 0;
	wxTreeItemId *pi = NULL;
	long currLev = 0;
	long lastLev = 0;
	char *currName = NULL;
	char *showName = NULL;

	for (long i = 1; i <= CMgr::CatCount; i++) {
		// if change in level, change parent to current parent at next lavel
		// if level 1, parent is root
		if ((currLev = CMgr::CatLevelList.find(i)) > lastLev) {
			if (!lastLev) {
				parentId = rootId;
				CMgr::CatParentNums.append(0, i);
			}
			else {
				parentId = CMgr::CatParents[lastLev];
				CMgr::CatParentNums.append(CMgr::CatParentLevels.find(lastLev), i);
			}
			lastLev = currLev;
		}
		else if (currLev < lastLev) {
			if (currLev <= 1) {
				parentId = rootId;
				CMgr::CatParentNums.append(0, i);
			}
			else {
				parentId = CMgr::CatParents[currLev - 1];
				CMgr::CatParentNums.append(CMgr::CatParentLevels.find(currLev - 1), i);
			}
			lastLev = currLev;
		}
		else
			CMgr::CatParentNums.append(CMgr::CatParentLevels.find(currLev - 1), i);

		currName = CMgr::CatNameList.find(i);
		showName = MakeShowName(currName);

    CMgr::CatParents[currLev] = m_sectyptree->AppendItem(parentId, wxT(showName), -1, -1,
                                new MyTreeItemData(wxString(NewName(currName))));

		CMgr::CatParentLevels.replace(i, currLev);

		void *id = (void *) (itemId.m_pItem);
		void *pid = (void *) (parentId.m_pItem);

		CMgr::CatIDList.append(id, i);
		CMgr::CatParentList.append(pid, i);
	}

  m_sectyptree->SelectItem(CMgr::CatIDList.find(1));
  //m_sectyptree->Expand(CMgr::CatIDList.find(2));  // formats
}


void ConfigMgr::SelectCategories(void)
{
	if (m_catselbutton->GetValue() == 0) {
		if (m_catinitypebutton->GetValue())
			SelectIniType();
		return;
	}

	CMgr::SelAllTopCat = false;
	CMgr::SelIniTyp = ininone;

	if (!CMgr::SelectedCategory) {
		wxTreeItemId catid = m_sectyptree->GetSelection();
		if (catid.IsOk()) {
			MyTreeItemData *dat = (MyTreeItemData *) m_sectyptree->GetItemData(catid);
			if (dat)
				CMgr::SelectedCategory = NewName((dat->GetDesc()).c_str());
		}
	}

	if (CMgr::SelectedCategory) {
		CMgr::CatNum = CMgr::CatNameList.getitem(CMgr::SelectedCategory);
		if (CMgr::CatNum) {
			CMgr::SelectedCategory = CMgr::CatNameList.find(CMgr::CatNum);
			long catLevel = CMgr::CatLevelList.find(CMgr::CatNum);
			CMgr::SelCatLevel = catLevel;
			long catParent = 0;
			long catItem = CMgr::CatNum;
			// for subcats, detect top-level ancestor
			if (catLevel > 1) {  // find TopCategory
				while ((catParent = CMgr::CatParentNums.find(catItem)) != 0)
					catItem = catParent;
				CMgr::TopCategory = CMgr::CatNameList.find(catItem);
			}
			else
				CMgr::TopCategory = CMgr::SelectedCategory;

			CMgr::UseConfigs = false;
			CMgr::UseFormats = false;
			CMgr::UseLangs = false;
			CMgr::UseMacros = false;
			CMgr::UseModels = false;
			CMgr::Usage = 0;
			CMgr::CategoryCode = 'C';
			CatAlias = wxString("IDH_ZSelectCategory");

			if (!stricmp(CMgr::TopCategory, CMgr::ConfigsCat)) {
				CMgr::UseConfigs = true;
				CMgr::Usage += 1;
				CMgr::CategoryCode = 'C';
				CatAlias = wxString("IDH_ZConfigs");
			}
			else if (!stricmp(CMgr::TopCategory, CMgr::ParaCharFormatsCat)) {
				CMgr::UseFormats = true;
				CMgr::Usage += 16;
				CMgr::CategoryCode = 'F';
				CatAlias = wxString("IDH_ZParaCharFormats");
			}
			else if (!stricmp(CMgr::TopCategory, CMgr::TableFormatsCat)) {
				CMgr::UseFormats = true;
				CMgr::Usage += 32;
				CMgr::CategoryCode = 'T';
				CatAlias = wxString("IDH_ZTableFormats");
			}
			else if (!stricmp(CMgr::TopCategory, CMgr::SubformatsCat)) {
				CMgr::UseFormats = true;
				CMgr::Usage += 64;
				CMgr::CategoryCode = 'S';
				CatAlias = wxString("IDH_ZSubformats");
			}
			else if (!stricmp(CMgr::TopCategory, CMgr::PageFormatsCat)) {
				CMgr::UseFormats = true;
				CMgr::Usage += 128;
				CMgr::CategoryCode = 'P';
				CatAlias = wxString("IDH_ZPageFormats");
			}
			else if (!stricmp(CMgr::TopCategory, CMgr::LanguagesCat)) {
				CMgr::UseLangs = true;
				CMgr::Usage += 4;
				CMgr::CategoryCode = 'L';
				CatAlias = wxString("IDH_ZLanguages");
			}
			else if (!stricmp(CMgr::TopCategory, CMgr::MacrosCat)) {
				CMgr::UseMacros = true;
				CMgr::Usage += 8;
				CMgr::CategoryCode = 'M';
				CatAlias = wxString("IDH_ZMacros");
			}
			else if (!stricmp(CMgr::TopCategory, CMgr::ContentModelsCat)) {
				CMgr::UseModels = true;
				CMgr::Usage += 256;
				CMgr::CategoryCode = 'D';
				CatAlias = wxString("IDH_ZContentModels");
			}
		}
		else {  // not in CatNameList, use Configs
			CMgr::UseConfigs = true;
			CMgr::UseFormats = false;
			CMgr::UseLangs = false;
			CMgr::UseMacros = false;
			CMgr::UseModels = false;
			CMgr::Usage = 1;
			CMgr::CategoryCode = 'C';
			CatAlias = wxString("IDH_ZConfigs");
			CMgr::CatNum = 1;
			CMgr::SelectedCategory = CMgr::CatNameList.find(CMgr::CatNum);
			CMgr::TopCategory = CMgr::SelectedCategory;
			CMgr::SelCatLevel = 1;
		}

		if (CMgr::SelCatLevel > 1) {
			char *alias = NewName("IDH_Z", strlen(CMgr::SelectedCategory) + 6);
			strcat(alias, CMgr::SelectedCategory);
			if (CMgr::HelpAlias.getitem(alias) != 0)
				CatAlias = wxString(alias);
		}

		char *currName = CMgr::SelectedCategory;
		long scnt = strlen(currName);
		char *showName = NewName(scnt * 2);  // allow for spaces between every letter
		long j = 0;
		long k = 0;
		for (; j < scnt; j++, k++) {
			showName[k] = currName[j];
			if (k
			 && islower(showName[k])
			 && isupper(currName[j+1]))
				showName[++k] = ' ';
		}

		wxString str = wxString(MakeShowName(CMgr::SelectedCategory));
		m_sectcattext->Clear();
		m_sectcattext->WriteText(str);
		m_setcattext->Clear();
		m_setcattext->WriteText(str);
		m_valcattext->Clear();
		m_valcattext->WriteText(str);
		m_vselcattext->Clear();
		m_vselcattext->WriteText(str);
	}
}


char *ConfigMgr::MakeShowName(char *currName)
{
	long scnt = strlen(currName);
	char *showName = NewName(scnt * 2);  // allow for spaces between every letter
	long j = 0;
	long k = 0;
	for (; j < scnt; j++, k++) {
		showName[k] = currName[j];
		if (k
		 && islower(showName[k])
		 && isupper(currName[j+1]))
			showName[++k] = ' ';
	}
	return showName;
}


// match strings

char *CMgr::SelectedSection = NULL;
char *CMgr::SelectedSetting = NULL;


void ConfigMgr::ApplyCategories(void)
{
	CMgr::ActPanelEnabled = false;
	CMgr::SectPanelEnabled = false;
	CMgr::SetPanelEnabled = false;
	CMgr::ValPanelEnabled = false;
	CMgr::VselPanelEnabled = false;

	SelectCategories();
	if (!CMgr::SelectedCategory) {
		m_catresult->Clear();
		m_catresult->WriteText(wxString("No category selected."));
		return;
	}

	if (!CMgr::SelectedSection) {
		CMgr::SelectedSection = NewName((m_projstartsecttxt->GetValue()).c_str());
		if (*CMgr::SelectedSection == '\0')
			CMgr::SelectedSection = NULL;
	}
	else {
		m_projstartsecttxt->Clear();
		m_projstartsecttxt->WriteText(wxString(CMgr::SelectedSection));
	}

	if (!CMgr::SelectedSetting) {
		CMgr::SelectedSetting = NewName((m_projstartsettxt->GetValue()).c_str());
		if (*CMgr::SelectedSetting == '\0')
			CMgr::SelectedSetting = NULL;
	}
	else {
		m_projstartsettxt->Clear();
		m_projstartsettxt->WriteText(wxString(CMgr::SelectedSetting));
	}

	m_catresult->Clear();
	m_catresult->WriteText(wxString("Category selected."));

	long catnum = CMgr::IniTypes.getitem((long) CMgr::CategoryCode);
	CMgr::AvailIni = (SIni *) CMgr::AvailIniFiles.find(catnum);
	CMgr::SuffixIni = (SIni *) CMgr::AvailSuffixIniFiles.find(catnum);

	ShowAction(true);
}


// Action panel

bool CMgr::ActPanelEnabled = true;
actyp CMgr::CurrAct = actnone;


void ConfigMgr::ShowAction(bool clear)
{
	CMgr::ActPanelEnabled = true;
	(GetBookCtrl())->ChangeSelection((size_t) pnact);
	DCpfile::addhist(NULL);

	if (clear) {
		CMgr::IncludeAllSections = false;
		CMgr::IncludeAllSettings = false;
		CMgr::VNSectsOnly = false;
		CMgr::FNSectsOnly = false;
		CMgr::VKSectsOnly = false;
		CMgr::FKSectsOnly = false;
		CMgr::DelSectsOnly = false;
		CMgr::DupSectsOnly = false;
		CMgr::DelSetsOnly = false;
		CMgr::DupSetsOnly = false;
		CMgr::NoMacroSects = false;
		CMgr::DupSectsFound = false;
		CMgr::DupSetsFound = false;
		CMgr::DupWarning = false;
#if 0
		CMgr::CurrAct = actnone;
		m_sectaddbutton->SetValue(false);
		m_sectvnaddbutton->SetValue(false);
		m_sectaddsetbutton->SetValue(false);
		m_sectchangebutton->SetValue(false);
		m_sectseteditbutton->SetValue(false);
		m_sectdelbutton->SetValue(false);
		m_sectdelsetbutton->SetValue(false);
		m_sectrestbutton->SetValue(false);
		m_sectrestsetbutton->SetValue(false);
		m_sectrenbutton->SetValue(false);
		m_sectrensetbutton->SetValue(false);
		m_sectmovbutton->SetValue(false);
		m_sectmovsetbutton->SetValue(false);
		m_sectmovfbutton->SetValue(false);
		m_sectmovsetfbutton->SetValue(false);
		m_sectcpybutton->SetValue(false);
		m_sectcpysetfbutton->SetValue(false);
		m_sectdupbutton->SetValue(false);
		m_sectdupsetbutton->SetValue(false);
#endif
		m_sectvnaddtext->Disable();
		m_sectvnrentext->Disable();
		m_sectformalias->Disable();
		m_actresult->Clear();
		ActAlias.Empty();
	}


	wxString txt;

	if (CMgr::CurrAct == actfixs)
		CMgr::CurrAct = actdels;

	switch (CMgr::CurrAct) {
		case actadd:
			txt = wxString("Add a new fixed-name section to one file.");
			CMgr::IncludeAllSections = true;
			CMgr::VNSectsOnly = false;
			CMgr::FNSectsOnly = true;
			CMgr::DelSectsOnly = false;
			CMgr::DupSectsOnly = false;
			CMgr::NoMacroSects = true;
			m_sectvnaddtext->Disable();
			m_sectvnrentext->Disable();
			m_sectformalias->Disable();
			CMgr::IncludeAllSettings = false;
			CMgr::DelSetsOnly = false;
			CMgr::DupSetsOnly = false;
			ActAlias = wxString("IDH_ZAddFNSection");
			break;

		case actaddv:
			txt = wxString("Add a new variable-name section to one file.");
			CMgr::IncludeAllSections = false;
			CMgr::VNSectsOnly = true;
			CMgr::FNSectsOnly = false;
			CMgr::DelSectsOnly = false;
			CMgr::DupSectsOnly = false;
			CMgr::NoMacroSects = false;
			m_sectvnaddtext->Enable();
			m_sectvnaddtext->Clear();
			m_sectvnrentext->Disable();
			m_sectformalias->Disable();
			CMgr::IncludeAllSettings = false;
			CMgr::DelSetsOnly = false;
			CMgr::DupSetsOnly = false;
			ActAlias = wxString("IDH_ZAddVNSection");
			break;

		case actadds:
		case actaddsv:
		case actaddsb:
			txt = wxString("Add a new setting to one section.");
			CMgr::IncludeAllSections = true;
			CMgr::VNSectsOnly = false;
			CMgr::FNSectsOnly = false;
			CMgr::IncludeAllSettings = true;
			CMgr::VKSectsOnly = false;
			CMgr::FKSectsOnly = false;
			CMgr::DelSectsOnly = false;
			CMgr::DupSectsOnly = false;
			CMgr::DelSetsOnly = false;
			CMgr::DupSetsOnly = false;
			CMgr::NoMacroSects = true;
			m_sectvnaddtext->Disable();
			m_sectvnrentext->Disable();
			m_sectformalias->Disable();
			ActAlias = wxString("IDH_ZAddSetting");
			break;

#if 0
		case actaddsv:
			txt = wxString("Add a new variable-key setting to one section.");
			CMgr::IncludeAllSections = true;
			CMgr::VNSectsOnly = false;
			CMgr::FNSectsOnly = false;
			CMgr::IncludeAllSettings = true;
			CMgr::VKSectsOnly = false;
			CMgr::FKSectsOnly = false;
			CMgr::DelSectsOnly = false;
			CMgr::DupSectsOnly = false;
			CMgr::DelSetsOnly = false;
			CMgr::DupSetsOnly = false;
			CMgr::NoMacroSects = true;
			m_sectvnaddtext->Disable();
			m_sectvnrentext->Disable();
			m_sectformalias->Disable();
			ActAlias = wxString("IDH_ZAddVKSetting");
			break;
#endif

		case actedit:
			txt = wxString("Edit a full existing section in one file.");
			CMgr::IncludeAllSections = false;
			CMgr::VNSectsOnly = false;
			CMgr::FNSectsOnly = false;
			CMgr::VKSectsOnly = false;
			CMgr::FKSectsOnly = false;
			CMgr::DelSectsOnly = false;
			CMgr::DupSectsOnly = false;
			CMgr::NoMacroSects = false;
			m_sectvnaddtext->Disable();
			m_sectvnrentext->Disable();
			m_sectformalias->Disable();
			CMgr::IncludeAllSettings = false;
			CMgr::DelSetsOnly = false;
			CMgr::DupSetsOnly = false;
			ActAlias = wxString("IDH_ZEditSection");
			break;

		case actedits:
			txt = wxString("Edit an existing setting in one section.");
			CMgr::IncludeAllSections = false;
			CMgr::VNSectsOnly = false;
			CMgr::FNSectsOnly = false;
			CMgr::IncludeAllSettings = false;
			CMgr::VKSectsOnly = false;
			CMgr::FKSectsOnly = false;
			CMgr::DelSectsOnly = false;
			CMgr::DupSectsOnly = false;
			CMgr::DelSetsOnly = false;
			CMgr::DupSetsOnly = false;
			CMgr::NoMacroSects = true;
			m_sectvnaddtext->Disable();
			m_sectvnrentext->Disable();
			m_sectformalias->Disable();
			ActAlias = wxString("IDH_ZEditSetting");
			break;

		case actdel:
			txt = wxString("Delete an existing section from one file.");
			CMgr::IncludeAllSections = false;
			CMgr::VNSectsOnly = false;
			CMgr::FNSectsOnly = false;
			CMgr::VKSectsOnly = false;
			CMgr::FKSectsOnly = false;
			CMgr::DelSectsOnly = false;
			CMgr::DupSectsOnly = false;
			CMgr::NoMacroSects = false;
			m_sectvnaddtext->Disable();
			m_sectvnrentext->Disable();
			m_sectformalias->Disable();
			CMgr::IncludeAllSettings = false;
			CMgr::DelSetsOnly = false;
			CMgr::DupSetsOnly = false;
			ActAlias = wxString("IDH_ZDeleteSection");
			break;

		case actdels:
			txt = wxString("Delete an existing setting in one section.");
			CMgr::IncludeAllSections = false;
			CMgr::IncludeAllSettings = false;
			CMgr::VNSectsOnly = false;
			CMgr::FNSectsOnly = false;
			CMgr::VKSectsOnly = false;
			CMgr::FKSectsOnly = false;
			CMgr::DelSectsOnly = false;
			CMgr::DupSectsOnly = false;
			CMgr::DelSetsOnly = false;
			CMgr::DupSetsOnly = false;
			CMgr::NoMacroSects = true;
			m_sectvnaddtext->Disable();
			m_sectvnrentext->Disable();
			m_sectformalias->Disable();
			ActAlias = wxString("IDH_ZDeleteSetting");
			break;

		case actrest:
			txt = wxString("Restore a deleted section in one file.");
			CMgr::IncludeAllSections = false;
			CMgr::VNSectsOnly = false;
			CMgr::FNSectsOnly = false;
			CMgr::VKSectsOnly = false;
			CMgr::FKSectsOnly = false;
			CMgr::DelSectsOnly = true;
			CMgr::DupSectsOnly = false;
			CMgr::NoMacroSects = false;
			m_sectvnaddtext->Disable();
			m_sectvnrentext->Disable();
			m_sectformalias->Disable();
			CMgr::IncludeAllSettings = false;
			CMgr::DelSetsOnly = false;
			CMgr::DupSetsOnly = false;
			ActAlias = wxString("IDH_ZRestoreSection");
			break;

		case actrests:
			txt = wxString("Restore a deleted setting in one section.");
			CMgr::IncludeAllSections = false;
			CMgr::IncludeAllSettings = false;
			CMgr::VNSectsOnly = false;
			CMgr::FNSectsOnly = false;
			CMgr::VKSectsOnly = false;
			CMgr::FKSectsOnly = false;
			CMgr::DelSectsOnly = false;
			CMgr::DupSectsOnly = false;
			CMgr::DelSetsOnly = true;
			CMgr::DupSetsOnly = false;
			CMgr::NoMacroSects = true;
			m_sectvnaddtext->Disable();
			m_sectvnrentext->Disable();
			m_sectformalias->Disable();
			ActAlias = wxString("IDH_ZRestoreSetting");
			break;

		case actrenv:
			txt = wxString("Rename an existing variable-name section in one file.");
			CMgr::IncludeAllSections = false;
			CMgr::VNSectsOnly = true;
			CMgr::FNSectsOnly = false;
			CMgr::VKSectsOnly = false;
			CMgr::FKSectsOnly = false;
			CMgr::DelSectsOnly = false;
			CMgr::DupSectsOnly = false;
			CMgr::NoMacroSects = false;
			m_sectvnaddtext->Disable();
			m_sectvnrentext->Enable();
			m_sectvnrentext->Clear();
			if (CMgr::UseFormats) {
				m_sectformalias->Enable();
				m_sectformalias->SetValue(false);
				CMgr::AutoAlias = false;
				//CMgr::AutoAliasSet.Empty();
				//CMgr::AutoAliasVal.Empty();
			}
			CMgr::IncludeAllSettings = false;
			CMgr::DelSetsOnly = false;
			CMgr::DupSetsOnly = false;
			ActAlias = wxString("IDH_ZRenameVNSection");
			break;

		case actrensv:
			txt = wxString("Rename an existing variable-key setting in one section.");
			CMgr::IncludeAllSections = false;
			CMgr::VNSectsOnly = false;
			CMgr::FNSectsOnly = false;
			CMgr::IncludeAllSettings = false;
			CMgr::VKSectsOnly = true;
			CMgr::FKSectsOnly = false;
			CMgr::DelSectsOnly = false;
			CMgr::DupSectsOnly = false;
			CMgr::DelSetsOnly = false;
			CMgr::DupSetsOnly = false;
			CMgr::NoMacroSects = true;
			m_sectvnaddtext->Disable();
			m_sectvnrentext->Disable();
			m_sectformalias->Disable();
			ActAlias = wxString("IDH_ZRenameVKSetting");
			break;

		case actmov:
			txt = wxString("Move an existing section within one file.");
			CMgr::IncludeAllSections = false;
			CMgr::VNSectsOnly = false;
			CMgr::FNSectsOnly = false;
			CMgr::VKSectsOnly = false;
			CMgr::FKSectsOnly = false;
			CMgr::DelSectsOnly = false;
			CMgr::DupSectsOnly = false;
			CMgr::NoMacroSects = false;
			m_sectvnaddtext->Disable();
			m_sectvnrentext->Disable();
			m_sectformalias->Disable();
			CMgr::IncludeAllSettings = false;
			CMgr::DelSetsOnly = false;
			CMgr::DupSetsOnly = false;
			ActAlias = wxString("IDH_ZMoveSectionWithin");
			break;

		case actmovs:
			txt = wxString("Move an existing setting within one section.");
			CMgr::IncludeAllSections = false;
			CMgr::VNSectsOnly = false;
			CMgr::FNSectsOnly = false;
			CMgr::IncludeAllSettings = false;
			CMgr::VKSectsOnly = false;
			CMgr::FKSectsOnly = false;
			CMgr::DelSectsOnly = false;
			CMgr::DupSectsOnly = false;
			CMgr::DelSetsOnly = false;
			CMgr::DupSetsOnly = false;
			CMgr::NoMacroSects = true;
			m_sectvnaddtext->Disable();
			m_sectvnrentext->Disable();
			m_sectformalias->Disable();
			ActAlias = wxString("IDH_ZMoveSettingWithin");
			break;

		case actmovf:
			txt = wxString("Move an existing section between two files.");
			CMgr::IncludeAllSections = false;
			CMgr::VNSectsOnly = false;
			CMgr::FNSectsOnly = false;
			CMgr::VKSectsOnly = false;
			CMgr::FKSectsOnly = false;
			CMgr::DelSectsOnly = false;
			CMgr::DupSectsOnly = false;
			CMgr::NoMacroSects = false;
			m_sectvnaddtext->Disable();
			m_sectvnrentext->Disable();
			m_sectformalias->Disable();
			CMgr::IncludeAllSettings = false;
			CMgr::DelSetsOnly = false;
			CMgr::DupSetsOnly = false;
			ActAlias = wxString("IDH_ZMoveSectionBetween");
			break;

		case actmovsf:
			txt = wxString("Move an existing setting between two files.");
			CMgr::IncludeAllSections = false;
			CMgr::VNSectsOnly = false;
			CMgr::FNSectsOnly = false;
			CMgr::IncludeAllSettings = false;
			CMgr::VKSectsOnly = false;
			CMgr::FKSectsOnly = false;
			CMgr::DelSectsOnly = false;
			CMgr::DupSectsOnly = false;
			CMgr::DelSetsOnly = false;
			CMgr::DupSetsOnly = false;
			CMgr::NoMacroSects = true;
			m_sectvnaddtext->Disable();
			m_sectvnrentext->Disable();
			m_sectformalias->Disable();
			ActAlias = wxString("IDH_ZMoveSettingBetween");
			break;

		case actcpy:
			txt = wxString("Copy an existing section from one file to another.");
			CMgr::IncludeAllSections = false;
			CMgr::VNSectsOnly = false;
			CMgr::FNSectsOnly = false;
			CMgr::VKSectsOnly = false;
			CMgr::FKSectsOnly = false;
			CMgr::DelSectsOnly = false;
			CMgr::DupSectsOnly = false;
			CMgr::NoMacroSects = false;
			m_sectvnaddtext->Disable();
			m_sectvnrentext->Disable();
			m_sectformalias->Disable();
			CMgr::IncludeAllSettings = false;
			CMgr::DelSetsOnly = false;
			CMgr::DupSetsOnly = false;
			ActAlias = wxString("IDH_ZCopySection");
			break;

		case actcpys:
			txt = wxString("Copy an existing setting from one file to another.");
			CMgr::IncludeAllSections = false;
			CMgr::VNSectsOnly = false;
			CMgr::FNSectsOnly = false;
			CMgr::IncludeAllSettings = false;
			CMgr::VKSectsOnly = false;
			CMgr::FKSectsOnly = false;
			CMgr::DelSectsOnly = false;
			CMgr::DupSectsOnly = false;
			CMgr::DelSetsOnly = false;
			CMgr::DupSetsOnly = false;
			CMgr::NoMacroSects = true;
			m_sectvnaddtext->Disable();
			m_sectvnrentext->Disable();
			m_sectformalias->Disable();
			ActAlias = wxString("IDH_ZCopySetting");
			break;

		case actdup:
			txt = wxString("Merge duplicated sections in one file.");
			CMgr::IncludeAllSections = false;
			CMgr::VNSectsOnly = false;
			CMgr::FNSectsOnly = false;
			CMgr::VKSectsOnly = false;
			CMgr::FKSectsOnly = false;
			CMgr::DelSectsOnly = false;
			CMgr::DupSectsOnly = true;
			CMgr::NoMacroSects = false;
			m_sectvnaddtext->Disable();
			m_sectvnrentext->Disable();
			m_sectformalias->Disable();
			CMgr::IncludeAllSettings = false;
			CMgr::DelSetsOnly = false;
			CMgr::DupSetsOnly = false;
			ActAlias = wxString("IDH_ZMergeSections");
			break;

		case actdups:
			txt = wxString("Merge duplicated settings in one section.");
			CMgr::IncludeAllSections = false;
			CMgr::IncludeAllSettings = false;
			CMgr::VNSectsOnly = false;
			CMgr::FNSectsOnly = false;
			CMgr::VKSectsOnly = false;
			CMgr::FKSectsOnly = false;
			CMgr::DelSectsOnly = false;
			CMgr::DupSectsOnly = false;
			CMgr::DelSetsOnly = false;
			CMgr::DupSetsOnly = true;
			CMgr::NoMacroSects = true;
			m_sectvnaddtext->Disable();
			m_sectvnrentext->Disable();
			m_sectformalias->Disable();
			ActAlias = wxString("IDH_ZMergeSettings");
			break;

		case actnone:
			txt = wxString("Action not selected yet.");
			m_sectvnaddtext->Disable();
			m_sectvnrentext->Disable();
			m_sectformalias->Disable();
			ActAlias = wxString(CMgr::DefaultAlias);
			break;

		default:
			txt = wxString("Action not implemented yet.");
			m_sectvnaddtext->Disable();
			m_sectvnrentext->Disable();
			m_sectformalias->Disable();
			ActAlias = wxString(CMgr::DefaultAlias);
			break;
	}

	if (!txt.IsEmpty()) {
		m_actresult->Clear();
		m_actresult->WriteText(txt);
		m_sectacttext->Clear();
		m_sectacttext->WriteText(txt);
		m_setacttext->Clear();
		m_setacttext->WriteText(txt);
		m_valacttext->Clear();
		m_valacttext->WriteText(txt);
		m_vselacttext->Clear();
		m_vselacttext->WriteText(txt);
	}
}


DCnlist CMgr::SecsToCheck;
DCvlist CMgr::SectionsToCheck;
DCilist CMgr::SecNumsToCheck;
DCnlist CMgr::SecsToShow;
DCvlist CMgr::SectionsToShow;
DCnlist CMgr::SectionsShownData;
DCvlist CMgr::SectionsShownSection;
long CMgr::SectionsShownDataCount;
long CMgr::SectionsShownDataCurr;


void CMgr::SelectSectionsToShow(DCnlist &sectlist, long sectcount, bool avail)
{
	CSection *csect = NULL;
	CSetting *cset = NULL;
	char *cname = NULL;
	long cnum = 0;
	DClist *secs = NULL;
	bool setfound = false;
	long checkcount = 0;
	long showcount = 0;

	CMgr::SecsToCheck.empty();
	CMgr::SectionsToCheck.empty();
	CMgr::SecNumsToCheck.empty();
	CMgr::SecsToShow.empty();
	CMgr::SectionsToShow.empty();

	if (sectcount) {
		DCnlist *wild = new DCnlist;
		long wcnt = 0;
		if (CMgr::SelectedSection        // put section alone
		 && ((wcnt = sectlist.getallwild(CMgr::SelectedSection, wild)) != 0)) {
			for (long i = 1; i <= wcnt; i++) {
				char *nm = wild->find(i);
				CMgr::SecsToCheck.addnew(nm, ++checkcount);
				long idx = sectlist.getitem(nm);
				CMgr::SecNumsToCheck.add(idx, checkcount);
			}
		}
		else {
			for (long i = 1; i <= sectcount; i++) {
				CMgr::SecsToCheck.addnew(sectlist.find(i), ++checkcount);
				CMgr::SecNumsToCheck.add(i, checkcount);
			}
		}
		wild->clear();
		wcnt = 0;
		if (CMgr::SelectedSetting) { // reduce section list to those with setting
			for (long i = 1; i <= checkcount; i++) {  // check each section for setting
				cname = CMgr::SecsToCheck.find(i);
				setfound = false;
				cnum = CMgr::SecNumsToCheck.find(i);
				if (avail) {
					SSection *ssect = (SSection *) AvailIni->sections.find(cnum);
					if (ssect->fksetnames.getwild(CMgr::SelectedSetting) != 0) {
						CMgr::SecsToShow.add(cname, ++showcount);
						CMgr::SectionsToShow.add(ssect, showcount);
						setfound = true;
					}
				}
				else {
					secs = (DClist *) (CMgr::CurrSectionNameRefs.find(cnum));
					setfound = false;
					while ((secs = secs->next) != NULL) {
						csect = (CSection *) secs->id;
						if (csect
						 && CheckSection(csect, false)
						 && !setfound
						 && csect->setcount) {
							for (long j = 1; j <= csect->setcount; j++) {
								cset = (CSetting *) csect->settings.find(j);
								if (wild->wildmatch(CMgr::SelectedSetting, cset->name)) {
									CMgr::SecsToShow.add(cname, ++showcount);
									CMgr::SectionsToShow.add(csect, showcount);
									setfound = true;
									break;
								}
							}
						}
						//if (setfound)
						//	break;  // in at least one sect with this name
					}
				}
			}
		}
		if (!showcount) {  // no SelectedSetting, or none left
			for (long i = 1; i <= checkcount; i++) {
				cnum = CMgr::SecNumsToCheck.find(i);
				void *sect = NULL;
				void *firstsect = NULL;
				bool usesect = false;
				if (avail) {
					sect = AvailIni->sections.find(cnum);
					if (CheckSection(sect, avail)) {
						CMgr::SecsToShow.add(CMgr::SecsToCheck.find(i), ++showcount);
						CMgr::SectionsToShow.add(sect, showcount);
					}
				}
				else {
					secs = (DClist *) (CMgr::CurrSectionNameRefs.find(cnum));
					firstsect = NULL;
					usesect = false;
					while ((secs = secs->next) != NULL) {
						if ((sect = (void *) secs->id) != NULL) {
							if (!firstsect)
								firstsect = sect;
							if (CheckSection(sect, false))
								usesect = true;  // show if any of them usable
						}
					}
					if (usesect) {
						CMgr::SecsToShow.add(CMgr::SecsToCheck.find(i), ++showcount);
						CMgr::SectionsToShow.add(firstsect, showcount);
					}
				}
			}
		}


		wild->clear();
		delete wild;
	}
}


bool CMgr::VNSectsOnly = false;
bool CMgr::FNSectsOnly = false;
bool CMgr::VKSectsOnly = false;
bool CMgr::FKSectsOnly = false;
bool CMgr::DelSectsOnly = false;
bool CMgr::DupSectsOnly = false;
bool CMgr::DupSectsFound = false;
bool CMgr::DelSetsOnly = false;
bool CMgr::DupSetsOnly = false;
bool CMgr::DupSetsFound = false;
bool CMgr::DupWarning = false;
bool CMgr::NoMacroSects = false;

char *CMgr::FixedMacroSectionsCat = "FixedMacroSections";
char *CMgr::MacroNamesCat = "MacroNames";
char *CMgr::MacrosSect = "Macros";
char *CMgr::MacroVariablesSect = "MacroVariables";


bool CMgr::CheckSection(void *sect, bool avail)
{
	SSection *ssect = NULL;
	CSection *csect = NULL;

	if (avail) {
		ssect = (SSection *) sect;
	}
	else {
		csect = (CSection *) sect;
		ssect = csect->ssect;
	}

	if (csect
	 && !stricmp(csect->name, "end"))
		return false;
	if (ssect
	 && !stricmp(ssect->name, "end"))
		return false;

	if (csect) {
		if (DelSectsOnly
		 && (*(csect->name) != ';'))
			return false;
		if (!DelSectsOnly
		 && (*(csect->name) == ';'))
			return false;
		if (DupSectsOnly
		 && !csect->hassectdup)
			return false;
		if (DupSetsOnly
		 && !csect->hassetdup)
			return false;
	}

	if (ssect) {
		if (ssect->formats
		 && CMgr::FormatCode
		 && !(isdigit(*(ssect->formats))
		  && (*(ssect->formats + 1) == '\0'))
		 && !strchr(ssect->formats, CMgr::FormatCode))
			return false;
#ifndef _M2GCM
		if (ssect->formats
		 && strchr(ssect->formats, '1'))
			return false;
#else
		if (ssect->formats
		 && strchr(ssect->formats, '2'))
			return false;
#endif
		if ((ssect->nametyp == stfix)
		 && VNSectsOnly)
			return false;
		if ((ssect->nametyp == stvar)
		 && FNSectsOnly)
			return false;
		if ((ssect->settyp == stfix)
		 && VKSectsOnly)
			return false;
		if ((ssect->settyp == stvar)
		 && FKSectsOnly)
			return false;

		if (NoMacroSects
		 && ((ssect->settyp == stnone)
		  || ((ssect->ifile->ityp == inimac)
		   && (ssect->nametyp != stfix))))
#if 0
		 && (stricmp(SelectedCategory, FixedMacroSectionsCat)
			|| (stricmp(ssect->name, MacroVariablesSect)
			 && stricmp(ssect->name, MacrosSect))))
#endif
			return false;
	}
	else if (NoMacroSects
	 && csect
	 && ((csect->ifile->ityp == inimac)
	  || (csect->ifile->ityp == inicfg)))
		return false;

#if 0
	if (ssect
	 && (SelCatLevel > 1)) { // exclude any outside category
		if (!ssect->cats)
			return false;
		for (long pos = 0; ssect->cats[pos]; pos++) {
			if (!stricmp(ssect->cats[pos], SelectedCategory)) {
				found = true;
				break;
			}
		}
		if (!found)
			return false;
	}

	if (csect
	 && (SelCatLevel > 1)) {
		if (csect->isinvalid)
			return false;
		if (csect->ismacro
		 && !found
		 && csect->ifile->ityp != inimac)
			return false;
	}

	if (ssect
	 && (SelCatLevel == 1) // exclude any in subcategory
	 && !CMgr::SelAllTopCat
	 && ssect->cats) {
		if ((ssect->cats[1] != NULL)   // more than one cat, must be subcat
		 || ((ssect->cats[0] != NULL)  // not explicitly set to level 1
		  && stricmp(ssect->cats[0], SelectedCategory)))
			return false;
	}

	if (csect
	 && (CMgr::SelCatLevel == 1)
	 && !CMgr::SelAllTopCat) {
		if (csect->ismacro
		 && (csect->ifile->ityp == inimac))
			return false;
		if (csect->ismacro
		 && UseMacros)
			return false;
	}
#endif

	char **cats = NULL;
	bool found = false;

	if (csect
	 && csect->cats)
		cats = csect->cats;
	else if (ssect
	 && ssect->cats)
		cats = ssect->cats;
	if (cats
	 && !CMgr::SelAllTopCat) {
		for (long pos = 0; cats[pos]; pos++) {
			if (!stricmp(cats[pos], SelectedCategory)) {
				found = true;
				break;
			}
		}
		if (!found)
			return false;
	}
	else if (CMgr::SelAllTopCat) {
		if (ssect
		 && (ssect->ifile->ityp != CMgr::SelIniTyp))
			return false;
	}

	if (SelCatLevel > 1) { // exclude any outside category
		if (csect) {
			if (csect->isinvalid)
				return false;
			if (csect->ismacro) {
				if (!UseMacros)
					return false;
				if (stricmp(SelectedCategory, MacroNamesCat))
					return false;
			}
		}
		if (!cats
		 && csect
		 && !csect->ismacro)
			return false;
	}
	else if (!CMgr::SelAllTopCat) {   // at level 1, exclude any in subcategory
#if 0
		if (cats
		 && ((cats[1] != NULL)   // more than one cat, must be subcat
		  || ((cats[0] != NULL)  // not explicitly set to level 1
		   && stricmp(cats[0], SelectedCategory))))
			return false;
#endif
		if (csect) {
			if (csect->ismacro
			 && UseMacros)
				return false;
		}
	}

	if (csect) {
		if (!DupSectsOnly
		 && csect->hassectdup)
			DupSectsFound = true;
		if (!DupSetsOnly
		 && csect->hassetdup)
			DupSetsFound = true;
	}

	return true;
}


void ConfigMgr::AddSectionsToShow(const char *name, void *vp, bool all)
{
	if (all   // don't duplicate file sections with available ones
	 && (SectionsShown.Index(name) != wxNOT_FOUND))
		return;
	if (all  // exclude deprecated sections 
	 && (((SSection *) vp)->show == false))
		return;
	SectionsShown.Add(wxString(name));
	//if (CMgr::IncludeAllSections)
	CMgr::SectionsShownData.addnew(name, ++CMgr::SectionsShownDataCount);
	CMgr::SectionsShownSection.add(vp, CMgr::SectionsShownDataCount);
}


void ConfigMgr::ApplyAction(void)
{
	CMgr::SectPanelEnabled = false;
	CMgr::SetPanelEnabled = false;
	CMgr::ValPanelEnabled = false;
	CMgr::VselPanelEnabled = false;

	ShowAction();

	if (CMgr::CurrAct == actnone) {
		m_actresult->Clear();
		m_actresult->WriteText(wxString("Action not selected yet."));
		return;
	}

	if (CMgr::GetAllInis)
		CMgr::GetIniFileChains();
	else
		CMgr::GetIniFiles();

	m_sectresult->Clear();
	SectionsShown.Clear();
	CMgr::SectionsShownData.empty();
	CMgr::SectionsShownSection.empty();
	CMgr::SectionsShownDataCount = 0;
	void *vp = NULL;

	CMgr::SelectSectionsToShow(CMgr::CurrSectionNames, CMgr::CurrSectionNameCount, false);
	for (DCnlist *sec = CMgr::SecsToShow.next; sec; sec = sec->next) {
		vp = CMgr::SectionsToShow.find (sec->id);
		AddSectionsToShow(sec->name, vp, false);
	}

	CMgr::SectionsShownDataCurr = CMgr::SectionsShownDataCount;

	if (CMgr::IncludeAllSections) {
		CMgr::SelectSectionsToShow(CMgr::AvailIni->sectnames, CMgr::AvailIni->sectcount, true);
		for (DCnlist *sec = CMgr::SecsToShow.next; sec; sec = sec->next) {
			vp = CMgr::SectionsToShow.find (sec->id);
			AddSectionsToShow(sec->name, vp, true);
		}
	}



	if (!CMgr::DupWarning) {
		if (CMgr::DupSectsFound
		 && (CMgr::CurrAct !=	actdup)) {
			CMgr::CurrAct =	actdup;
			m_sectdupbutton->SetValue(true);
			ShowAction();
			m_actresult->Clear();
			m_actresult->WriteText(wxString("Duplicate sections found; Apply to fix."));
			CMgr::DupWarning = true;
			return;
		}
		if (CMgr::DupSetsFound
		 && (CMgr::CurrAct !=	actdups)) {
			CMgr::CurrAct =	actdups;
			m_sectdupsetbutton->SetValue(true);
			ShowAction();
			m_actresult->Clear();
			m_actresult->WriteText(wxString("Duplicate settings found; Apply to fix."));
			CMgr::DupWarning = true;
			return;
		}
	}
	else
		CMgr::DupWarning = false;

	if (CMgr::CurrIniCount > 0)
		ShowSections();
	else {
		m_actresult->Clear();
		m_actresult->WriteText("No ini files found for the selected category.");
	}
}



// Section panel


bool CMgr::SectPanelEnabled = true;
wxString CMgr::VNSectNewName;
wxString CMgr::VNSectRename;
bool CMgr::CurrSectExists = false;
bool CMgr::CurrSectVN = false;
bool CMgr::CurrSectFN = false;
bool CMgr::CurrSectVK = false;
bool CMgr::CurrSectFK = false;
bool CMgr::CurrSectDel = false;
bool CMgr::CurrSectDup = false;
bool CMgr::CurrSectMacro = false;

bool CMgr::AutoAlias = false;
wxString CMgr::AutoAliasSet;
wxString CMgr::AutoAliasVal;


void ConfigMgr::ShowSections(const char *selname)
{
	m_sectctrl->DeleteAllItems();
	wxString wtxt;

	if (SectionsShown.IsEmpty()   // no sections suitable for the action found
	 && (CMgr::CurrAct != actaddv)) {  // ok since new name can be entered

		switch (CMgr::CurrAct) {
			case actadd:
				wtxt = "No fixed-name sections found that can be added.";
				break;

			case actedit:
				wtxt = "No sections found to edit.";
				break;

			case actadds:
			case actedits:
				wtxt = "No sections found where settings can be added or edited.";
				break;

			case actdel:
				wtxt = "No sections found to delete.";
				break;

			case actdels:
				wtxt = "No sections found where settings can be deleted.";
				break;

			case actrest:
				wtxt = "No deleted sections found to restore.";
				break;

			case actrests:
				wtxt = "No sections found where deleted settings can be restored.";
				break;

			case actrenv:
				wtxt = "No variable-name sections found to rename.";
				break;

			case actrensv:
				wtxt = "No sections found where variable-name settings can be renamed.";
				break;

			case actmov:
			case actmovf:
			case actcpy:
				wtxt = "No sections found to move or copy.";
				break;

			case actmovs:
			case actmovsf:
			case actcpys:
				wtxt = "No sections found where settings can be moved or copied.";
				break;

			case actdup:
				wtxt = "No duplicated sections found to merge.";
				break;

			case actdups:
				wtxt = "No sections found where settings are duplicated.";
				break;

			default:
				break;
		}

		m_actresult->Clear();
		m_actresult->WriteText(wtxt);
		CMgr::SectPanelEnabled = false;
		return;
	}

	CMgr::SectPanelEnabled = true;
	CMgr::CurrSectExists = false;
	CMgr::CurrSectVN = false;
	CMgr::CurrSectFN = false;
	CMgr::CurrSectVK = false;
	CMgr::CurrSectFK = false;
	CMgr::CurrSectDel = false;
	CMgr::CurrSectDup = false;
	CMgr::CurrSectMacro = false;

	SSection *sect = NULL;

	// columns are zero-based
	long cnt = SectionsShown.GetCount();
	long idx = 0;
	wxString name;
	long idval = 0;
	char *htxt = NULL;
	char *txt = "No Help for this section";
	CSection *csect = NULL;
	bool vnsect = false;
	bool exist = false;
	bool invalid = false;
	long selpos = -1;
	long pos = 0;
	bool setpos = false;

	if (!selname
	 && !SectionSelected.IsEmpty()) {
		selname = SectionSelected.c_str();
		setpos = true;
	}

	for (long i = 0; i < cnt; i++) {
		name = SectionsShown.Item(i);
		if ((selpos == -1)
		 && selname
		 && !stricmp(selname, name.c_str())) {
			selpos = i;
			if (setpos)
				SectionSelectedPos = i;
		}
    m_sectctrl->InsertItem(i, name);
		idval = CMgr::SectionsShownData.getitem(name.c_str());
		csect = NULL;
		vnsect = exist = invalid = false;
		if (idval <= CMgr::SectionsShownDataCurr) { // existing section
			exist = true;
			csect = (CSection *) CMgr::SectionsShownSection.find(idval);
			if (csect) {
				if ((csect->ssect == NULL)
					|| (csect->ssect->nametyp == stvar))
					vnsect = true;
				if (csect->isinvalid
				 || csect->hasinvalidset)
					invalid = true;
			}
		}
		if (csect
		 && (csect->htext))  // for formats or macros
			htxt = csect->htext;
		else if ((idx = CMgr::AvailIni->sectnames.getitem(name.c_str())) != 0) {
			sect = (SSection *) CMgr::AvailIni->sections.find(idx);
			if (sect->htext)
				htxt = sect->htext;
			else if (sect->vkshtext)
				htxt = sect->vkshtext;
		}
		//else if ((pos = CMgr::MacroNames.getitem(name.c_str())) != 0)
		//	htxt = NewName(CMgr::MacroHelp.find(pos));
		else if (vnsect
		 && csect->ssect) {
			if (csect->ssect->htext)
				htxt = csect->ssect->htext;
			else if (csect->ssect->vkshtext)
				htxt = csect->ssect->vkshtext;
		}
		else
			htxt = txt;
    m_sectctrl->SetItem(i, 1, htxt);

		if (invalid)
			m_sectctrl->SetItemTextColour(i, *InvalidColor);  // invalid
		else if (!exist)
			m_sectctrl->SetItemTextColour(i, *SectColor);  // avail section
		else if (vnsect)
			m_sectctrl->SetItemTextColour(i, *VarColor);  // VN section
	}

	m_secthelptext->Clear();

	if ((selpos == -1)
	 && (CMgr::CurrAct == actaddv)) {
		SectionSelected.Empty();
		SectionSelectedPos = -1;
	}
	else {
		selpos = 0;
		m_sectctrl->EnsureVisible(selpos);
		m_sectctrl->SetItemState(selpos, wxLIST_STATE_SELECTED | wxLIST_STATE_FOCUSED,
																wxLIST_STATE_SELECTED);
		SetSectButtons(selpos);
	}

	m_sectresult->Clear();

	if ((m_sectctrl->GetItemCount() == 1)
	 && !CMgr::ChildCM
	 && (CMgr::CurrAct != actaddv)
	 && (CMgr::CurrAct != actrenv)) {
		m_sectctrl->SetItemState(0, wxLIST_STATE_SELECTED | wxLIST_STATE_FOCUSED,
																wxLIST_STATE_SELECTED);
		(GetBookCtrl())->ChangeSelection((size_t) pnsec);
		ApplySections();
	}
	else
		(GetBookCtrl())->ChangeSelection((size_t) pnsec);
}


void ConfigMgr::SetUpSectionCtrl(void)
{
	m_sectctrl->ClearAll();
  m_sectctrl->InsertColumn(0, wxT("Name"), wxLIST_FORMAT_LEFT, 180);
  m_sectctrl->InsertColumn(1, wxT("Purpose"), wxLIST_FORMAT_LEFT, 200);
}


void ConfigMgr::SetSectButtons(long pos)
{
	// handle (pos == -1) case
	if (pos == -1)
		return;

	// called when section clicked or doubleclicked
	SectionSelected = m_sectctrl->GetItemText(pos);
	SectionSelectedPos = pos;
	SetAvailSection();

	bool vname = false;
	bool exists = false;
	bool invalid = false;
	bool invalidset = false;
	bool hassets = false;
	bool hashelp = false;

	if (CMgr::AvailSection) 
		vname = (CMgr::AvailSection->nametyp > stfix);
	else
		vname = true;

  wxListItem col1;
  col1.m_mask = wxLIST_MASK_TEXT;
  col1.m_col = 1;
  col1.m_itemId = pos;
	m_sectctrl->GetItem(col1);
	SectionHelpShown = col1.GetText();

	long idval = CMgr::SectionsShownData.getitem(SectionSelected.c_str());
	if (CMgr::IncludeAllSections
	 && (idval > CMgr::SectionsShownDataCurr))
		// section not in curr files, set add, no delete, no change
		CMgr::CurrSectExists = false;
	else   // section exists, allow delete and change
		CMgr::CurrSectExists = exists = true;

	CSection *csect = NULL;
	if (exists
	 && ((csect = (CSection *) CMgr::SectionsShownSection.find(idval)) != NULL)) {
		if ((csect->ssect == NULL)
			|| (csect->ssect->nametyp == stvar))
			vname = true;
		if (csect->isinvalid)
			invalid = true;
		if (csect->hasinvalidset)
			invalidset = true;
		if (csect->setcount)
			hassets = true;
		if (csect->htext)
			hashelp = true;
	}

	if (csect) {
	  CMgr::CurrSectDel = (*(csect->name) == ';');
	  CMgr::CurrSectDup = csect->hassectdup;
	}

	CMgr::CurrSectVN = vname;
	CMgr::CurrSectFN = !vname;

	bool vkey = false;
	bool fkey = false;

	if (CMgr::AvailSection) {
		if (CMgr::AvailSection->settyp == stfix)
			fkey = true;
		else if (CMgr::AvailSection->settyp == stvar)
			vkey = true;
		else if (CMgr::AvailSection->settyp == stboth)
			fkey = vkey = true;
	}

	CMgr::CurrSectVK = vkey;
	CMgr::CurrSectFK = fkey;
	CMgr::CurrSectMacro = (!fkey && !vkey);

	SectAlias = GetAlias(SectionSelected);

	wxString str = wxString("Section ");
	if (exists)
		str += wxString("is in current inis and ");
	else
		str += wxString("is not in any current inis and ");
	if (!vname)
		str += wxString("is fixed-name ");
	else if (!fkey && !vkey) {
		if (hashelp || !hassets)
		str += wxString("is a macro ");
	}
	else
		str += wxString("is variable-name ");
	if (fkey && vkey)
		str += wxString("with both fixed and variable keys ");
	else if (fkey)
		str += wxString("with fixed keys ");
	else if (vkey)
		str += wxString("with variable keys ");
	if (CMgr::CurrSectDel)
		str += wxString("and is deleted ");
	if (CMgr::CurrSectDup)
		str += wxString("and has a duplicate ");
	if (invalid)
		str += wxString("and is invalid in this ini ");
	if (invalidset)
		str += wxString("and has invalid settings ");
	m_secthelptext->Clear();
	m_secthelptext->WriteText(str);
}


void ConfigMgr::SetAvailSection(void)
{
	long idval = CMgr::SectionsShownData.getitem(SectionSelected.c_str());
	if (idval > CMgr::SectionsShownDataCurr) { // not existing
		CMgr::AvailSection = (SSection *) CMgr::SectionsShownSection.find(idval);
		return;
	}
	CSection *csect = (CSection *) CMgr::SectionsShownSection.find(idval);
	CMgr::AvailSection = NULL;
	if (csect
	 && csect->ssect)
		CMgr::AvailSection = csect->ssect;
	else {
		idval = CMgr::AvailIni->sectnames.getitem(SectionSelected.c_str());
		CMgr::AvailSection = (SSection *) CMgr::AvailIni->sections.find(idval);
	}
}


sectstat CMgr::SectionExists(const char *sname)
{
	if (CurrSectionNames.getitem(sname))   // duplicates existing section
		return sectcurr;
	if (AvailSectionNames.getitem(sname))  // duplicates FN section name
		return sectavail;
	return sectnone;  // OK  to use for new VN section name
}


void ConfigMgr::DeselectSections(void)
{
	long pos = m_sectctrl->GetNextItem(-1, wxLIST_NEXT_ALL, wxLIST_STATE_SELECTED);
	if (pos == -1)
		return;
	m_sectctrl->SetItemState(pos, ~(wxLIST_STATE_SELECTED | wxLIST_STATE_FOCUSED),
																wxLIST_STATE_SELECTED);
	SectionSelectedPos = -1;
}


void ConfigMgr::ApplySections(const char *showset)
{
	CMgr::SetPanelEnabled = false;
	CMgr::ValPanelEnabled = false;
	CMgr::VselPanelEnabled = false;
	CMgr::CurrSectExists = false;

	long pos = m_sectctrl->GetNextItem(-1, wxLIST_NEXT_ALL, wxLIST_STATE_SELECTED);

	if ((pos == -1)
	 && (CMgr::CurrAct != actaddv)) {
		m_sectresult->Clear();
		m_sectresult->WriteText(wxString("Select section before proceeding."));
		return;
	}

	if (pos == -1) {
		SectionSelected.Empty();
		SectionSelectedPos = -1;
		CMgr::AvailSection = NULL;
	}
	else if (pos > -1) {
		SectionSelected = m_sectctrl->GetItemText(pos);
		SectionSelectedPos = pos;

		m_setsecttext->Clear();
		m_setsecttext->WriteText(SectionSelected);

		SetAvailSection();
		//CMgr::AvailSection = (SSection *) CMgr::AvailIni->sections.find(CMgr::AvailIni->sectnames.getitem(SectionSelected));

		wxListItem col1;
		col1.m_mask = wxLIST_MASK_TEXT;
		col1.m_col = 1;
		col1.m_itemId = pos;
		m_sectctrl->GetItem(col1);
		SectionHelpShown = col1.GetText();
	}


	// is section VK? 
	bool vnsect = false;
	bool macro = false;

	bool vkey = false;
	bool fkey = false;

	if (CMgr::AvailSection) {
		if (CMgr::AvailSection->settyp == stfix)
			fkey = true;
		else if (CMgr::AvailSection->settyp == stvar)
			vkey = true;
		else if (CMgr::AvailSection->settyp == stboth)
			fkey = vkey = true;

		if (CMgr::AvailSection->nametyp == stvar)
			vnsect = true;
	}
	else
		vnsect = true;

	if (vnsect
	 && ((CMgr::CategoryCode == 'M')
	 || (!fkey && !vkey)))
		macro = true;

	CMgr::CurrSectVN = vnsect;
	CMgr::CurrSectFN = !vnsect;
	CMgr::CurrSectVK = vkey;
	CMgr::CurrSectFK = fkey;
	CMgr::CurrSectMacro = macro;


	if ((CMgr::CurrAct == actaddsb)  // use generic setting type
	 ||(CMgr::CurrAct == actaddsv))
		CMgr::CurrAct = actadds;


	if (CMgr::CurrAct == actadds) {  // determine setting type
		if (vkey && fkey)
			CMgr::CurrAct = actaddsb;
		else if (vkey)
			CMgr::CurrAct = actaddsv;
		m_setvkrentext->Disable();
	}


	sectstat st = sectnone;
	bool showsettings = true;  // to show settings panel next, else ini panel
	wxString action;
	wxString nexthelp;

	if (CMgr::CurrAct == actfixs)
		CMgr::CurrAct = actdels;

	switch (CMgr::CurrAct) {
		case actadd:   // add selected FN section
			if (!CMgr::CurrSectFN) {
				m_sectresult->Clear();
				m_sectresult->WriteText(wxString("Selected section is not fixed-name."));
				return;
			}
			action = wxString("Add selected section to one file.");
			nexthelp = wxString("Choose file in which to add section.");
			showsettings = false;
			break;

		case actadds: //  add FK setting
			if (CMgr::CurrSectMacro) {
				m_sectresult->Clear();
				m_sectresult->WriteText(wxString("Selected section is a macro."));
				return;
			}
			CMgr::IncludeAllSettings = true;
			action = wxString("Add a fixed-key setting to selected section in one file.");
			nexthelp = wxString("Choose fixed-key setting to add from above list.");
			m_setvkaddtext->Disable();
			break;

		case actaddsb: //  add FK or VK setting
			if (CMgr::CurrSectMacro) {
				m_sectresult->Clear();
				m_sectresult->WriteText(wxString("Selected section is a macro."));
				return;
			}
			CMgr::IncludeAllSettings = true;
			action = wxString("Add a fixed or variable-key setting to selected section in one file.");
			nexthelp = wxString("Pick from list or enter name for new setting below.");
			m_setvkaddtext->Enable();
			m_setvkaddtext->Clear();
			break;

		case actaddsv:    //  add VK setting
			if (CMgr::CurrSectMacro) {
				m_sectresult->Clear();
				m_sectresult->WriteText(wxString("Selected section is a macro."));
				return;
			}
			CMgr::IncludeAllSettings = false;
			action = wxString("Add a variable-key setting to selected section in one file.");
			nexthelp = wxString("Pick from list or enter name for new setting below.");
			m_setvkaddtext->Enable();
			break;

		case actaddv: // VN section
			// get name from m_sectvnaddtext
			CMgr::VNSectNewName = m_sectvnaddtext->GetValue();
			if (CMgr::VNSectNewName.IsEmpty()) {
				if (pos == -1) {
					m_sectresult->Clear();
					m_sectresult->WriteText(wxString("Name needed for new section."));
					return;
				}
				else if (CMgr::CurrSectFN) {
					m_sectresult->Clear();
					m_sectresult->WriteText(wxString("Selected section is not variable-name."));
					return;
				}
				else if (!SectionSelected.IsEmpty()) { // selected vn section for name
					CMgr::VNSectNewName = SectionSelected;
					m_sectvnaddtext->WriteText(SectionSelected);
				}
				else {
					m_sectresult->Clear();
					m_sectresult->WriteText(wxString("Name needed for new section."));
					return;
				}
			}
			else {
				SectionSelected = CMgr::VNSectNewName;
				SectionSelectedPos = -1;
				SectionHelpShown = wxString("---");
			}
			action = wxString("Add new variable-name section to one file.");
			nexthelp = wxString("Choose file in which to add section.");
			vnsect = true;
			showsettings = false;
			break;

		case actedit:     // any existing sect, maybe macro
			action = wxString("Edit full existing section in one file.");
			nexthelp = wxString("Choose file in which to edit section.");
			showsettings = false;
			break;

		case actedits:   //  edit existing setting
			action = wxString("Edit a setting in selected section in one file.");
			nexthelp = wxString("Choose setting to edit from above list.");
			m_setvkaddtext->Disable();
			m_setvkrentext->Disable();
			break;

		case actdel:   // any existing sect
			action = wxString("Delete selected section from one file.");
			nexthelp = wxString("Choose file from which to delete section.");
			showsettings = false;
			break;

		case actdels:   // any existing setting
			action = wxString("Delete a setting from selected section in one file.");
			nexthelp = wxString("Choose setting to delete from above list.");
			m_setvkaddtext->Disable();
			m_setvkrentext->Disable();
			break;

		case actrest:   // any deleted section
			action = wxString("Restore selected section in one file.");
			nexthelp = wxString("Choose file in which to restore section.");
			showsettings = false;
			break;

		case actrests:   // any deleted setting
			action = wxString("Restore a setting from selected section in one file.");
			nexthelp = wxString("Choose setting to restore from above list.");
			m_setvkaddtext->Disable();
			m_setvkrentext->Disable();
			break;

		case actrenv:   // VN section
			// get new name from m_sectvnrentext
			if (CMgr::CurrSectFN) {
				m_sectresult->Clear();
				m_sectresult->WriteText(wxString("Selected section is not variable-name."));
				return;
			}
			CMgr::VNSectRename = m_sectvnrentext->GetValue();
			if (CMgr::VNSectRename.IsEmpty()) {
				m_sectresult->Clear();
				m_sectresult->WriteText(wxString("Name needed for renamed section."));
				return;
			}
			action = wxString("Rename a variable-name section in one file.");
			nexthelp = wxString("Choose file in which to rename section.");
			if (CMgr::UseFormats
			 && m_sectformalias->GetValue()) {
				CMgr::AutoAlias = true;
				//CMgr::AutoAliasSet = SectionSelected;
				//CMgr::AutoAliasVal = CMgr::VNSectRename;
			}
			showsettings = false;
			vnsect = true;
			break;

		case actrensv:
			action = wxString("Rename a variable-key setting in selected section in one file.");
			nexthelp = wxString("Choose setting to rename, and enter new name below.");
			m_setvkaddtext->Disable();
			m_setvkrentext->Enable();
			break;

		case actmov:
			action = wxString("Move selected section within one file.");
			nexthelp = wxString("Choose file in which to move section.");
			showsettings = false;
			break;

		case actmovs:
			action = wxString("Move a setting within the selected section in one file.");
			nexthelp = wxString("Choose setting to move within its section from above list.");
			m_setvkaddtext->Disable();
			m_setvkrentext->Disable();
			break;

		case actmovf:
			action = wxString("Move selected section from one file to another.");
			nexthelp = wxString("Choose file from which to move section.");
			showsettings = false;
			break;

		case actmovsf:
			action = wxString("Move a setting from one file to another.");
			nexthelp = wxString("Choose setting to move to another file from above list.");
			m_setvkaddtext->Disable();
			m_setvkrentext->Disable();
			break;

		case actcpy:
			action = wxString("Copy selected section from one file to another.");
			nexthelp = wxString("Choose file from which to copy section.");
			showsettings = false;
			break;

		case actcpys:
			action = wxString("Copy a setting from one file to another.");
			nexthelp = wxString("Choose setting to copy to another file from above list.");
			m_setvkaddtext->Disable();
			m_setvkrentext->Disable();
			break;

		case actdup:   // any existing sect
			action = wxString("Merge duplicated sections in one file.");
			nexthelp = wxString("Choose file in which to merge duplicated section.");
			showsettings = false;
			break;

		case actdups:   // any existing setting
			action = wxString("Merge duplicated setting in selected section in one file.");
			nexthelp = wxString("Choose setting to merge from above list.");
			m_setvkaddtext->Disable();
			m_setvkrentext->Disable();
			break;

		case actnone:
			m_sectresult->Clear();
			m_sectresult->WriteText(wxString("Action not selected yet."));
			break;

		default:
			m_sectresult->Clear();
			m_sectresult->WriteText(wxString("Action not implemented yet."));
			return;
	}

	if (!SectionHelpShown.IsEmpty()) {
		m_setsecthelptext->Clear();
		m_setsecthelptext->WriteText(SectionHelpShown);
		m_valsecthelptext->Clear();
		m_valsecthelptext->WriteText(SectionHelpShown);
		m_vselsecthelptext->Clear();
		m_vselsecthelptext->WriteText(SectionHelpShown);
	}


	if (!showsettings) {  // go direct to ini-file page
		SettingNameShown = wxString("---");
		SettingValShown = wxString("---");
		UpdateSettingSelections();
		UpdateValueSelections();
		CMgr::SelectingSrcIni = true;
		SetUpValues(true);
		m_valresult->Clear();
		m_valacttext->Clear();
		m_valacttext->WriteText(action);
		m_valhelptext->Clear();
		m_valhelptext->WriteText(nexthelp);
		return;
	}

	// setting panel is next stop

	m_setresult->Clear();
	m_setacttext->Clear();
	m_setacttext->WriteText(action);
	m_sethelptext->Clear();
	m_sethelptext->WriteText(nexthelp);

	m_setvkaddtext->Clear();
	m_setvkrentext->Clear();
	CMgr::VKSetNewName.Empty();
	CMgr::VKSetRename.Empty();

	// if setting operation, produce setting list

	long snum = CMgr::CurrSectionNames.getitem(SectionSelected.c_str());
	DCnlist nv;
	long showcount = 0;
	SettingsShownCurr = -1;
	SettingsShown.Clear();
	SettingValsShown.Clear();
	CMgr::SettingObjsShown.empty();
	SelSetPos = 0;

	if (!snum) {   // section not in curr files
		if (CMgr::IncludeAllSettings
		 && CMgr::AvailSection) {   // get available settings
			if (CMgr::AvailSection->fksetcount // has FK settings
			 && CMgr::AvailSection->show) {    // not deprecated
				DCnlist *dcn = CMgr::AvailSection->fksetnames.next;
				DCvlist *dcv = CMgr::AvailSection->fksettings.next;
				while (dcn) {
					SSetting *sset = (SSetting *) dcv->vp;
					if (sset->formats
					 && CMgr::FormatCode
					 && !strchr(sset->formats, CMgr::FormatCode))
						goto eloop;;
#ifndef _M2GCM
					if (sset->formats
					 && strchr(sset->formats, '1'))
						goto eloop;;
#else
					if (sset->formats
					 && strchr(sset->formats, '2'))
						goto eloop;;
#endif
					if (sset->show) {
						SettingsShown.Add(wxString(dcn->name));
						SettingValsShown.Add(wxString(((SSetting *) dcv->vp)->dval));
						if (CMgr::SelectedSetting
						 && nv.wildmatch(CMgr::SelectedSetting, dcn->name)
						 && (SelSetPos == 0))
							SelSetPos = showcount;
						showcount++;
					}
eloop:		dcn = dcn->next;
					dcv = dcv->next;
				}
			}
			ShowSettings(showset, vnsect);
			return;
		}
		else if (vkey || vnsect) {
			ShowSettings(showset, vnsect);
			return;
		}
		else {
			m_sectresult->Clear();
			m_sectresult->WriteText(wxString("Section not in current files."));
			return;
		}
	}

	DClist *refs = (DClist *) CMgr::CurrSectionNameRefs.find(snum);

	if (!refs) {
		if (vkey || vnsect)
			ShowSettings(showset, vnsect);
		else {
			m_sectresult->Clear();
			m_sectresult->WriteText(wxString("Section not in current files."));
		}
		return;
	}

	long cnt = refs->count();
	if (cnt == 0) {
		m_sectresult->Clear();
		m_sectresult->WriteText(wxString("Section not in current files."));
		return;
	}

	CSection *csect = NULL;
	CSetting *cset = NULL;

	if (cnt == 1) {  // just one instance of section in tree
		csect = (CSection *) (refs->next->id);
		for (long i = 1; i <= csect->setcount; i++) {
			showcount++;
			cset = (CSetting *) csect->settings.find(i);
			if ((CMgr::DelSetsOnly && (*(cset->name) != ';'))
			 || (!CMgr::DelSetsOnly && (*(cset->name) == ';')))
				continue;
			if (CMgr::DupSetsOnly
			 && !cset->hassetdup)
				continue;
			SSetting *sset = cset->sset;
			if (sset) {
				if (sset->formats
				 && CMgr::FormatCode
				 && !strchr(sset->formats, CMgr::FormatCode))
					continue;
#ifndef _M2GCM
				if (sset->formats
				 && strchr(sset->formats, '1'))
					continue;
#else
				if (sset->formats
				 && strchr(sset->formats, '2'))
					continue;
#endif
			}
			SettingsShown.Add(wxString(cset->name));
			CMgr::SettingObjsShown.add(cset, showcount);
			SettingValsShown.Add(wxString(cset->cval));
			if (CMgr::SelectedSetting
			 && nv.wildmatch(CMgr::SelectedSetting, cset->name)
			 && (SelSetPos == 0))
				SelSetPos = showcount;
		}
	}
	else {  // multiple instances, merge settings lists
		for (long sec = 1; sec <= cnt; sec++) {
			csect = (CSection *) (refs->number(sec));
			for (long i = 1; i <= csect->setcount; i++) {
				cset = (CSetting *) csect->settings.find(i);
				if ((CMgr::DelSetsOnly && (*(cset->name) != ';'))
				 || (!CMgr::DelSetsOnly && (*(cset->name) == ';')))
					continue;
				if (CMgr::DupSetsOnly
				 && !cset->hassetdup)
					continue;
				SSetting *sset = cset->sset;
				if (sset) {
					if (sset->formats
					 && CMgr::FormatCode
					 && !strchr(sset->formats, CMgr::FormatCode))
						continue;
#ifndef _M2GCM
					if (sset->formats
					 && strchr(sset->formats, '1'))
						continue;
#else
					if (sset->formats
					 && strchr(sset->formats, '2'))
						continue;
#endif
				}
				if (!nv(cset->name)) {
					showcount++;
					nv.add(cset->name, i);
					SettingsShown.Add(wxString(cset->name));
					CMgr::SettingObjsShown.add(cset, showcount);
					SettingValsShown.Add(wxString(cset->cval));
					if (CMgr::SelectedSetting
					 && nv.wildmatch(CMgr::SelectedSetting, cset->name)
					 && (SelSetPos == 0))
						SelSetPos = showcount;
				}
			}
		}
	}

	SettingsShownCurr = showcount - 1;

	if (CMgr::IncludeAllSettings
	 && CMgr::AvailSection) { // add settings not in curr files
		if (CMgr::AvailSection->fksetcount // has FK settings
		 && CMgr::AvailSection->show) {    // not deprecated
			DCnlist *dcn = CMgr::AvailSection->fksetnames.next;
			DCvlist *dcv = CMgr::AvailSection->fksettings.next;
			while (dcn) {
				SSetting *sset = (SSetting *) dcv->vp;
				if (sset->formats
				 && CMgr::FormatCode
				 && !strchr(sset->formats, CMgr::FormatCode))
					goto eloop2;
#ifndef _M2GCM
				if (sset->formats
				 && strchr(sset->formats, '1'))
					goto eloop2;
#else
				if (sset->formats
				 && strchr(sset->formats, '2'))
					goto eloop2;
#endif
				if (sset->show
				 && (SettingsShown.Index(dcn->name, false) == wxNOT_FOUND)) {
					showcount++;
					SettingsShown.Add(wxString(dcn->name));
					SettingValsShown.Add(wxString(((SSetting *) dcv->vp)->dval));
					if (CMgr::SelectedSetting
					 && nv.wildmatch(CMgr::SelectedSetting, dcn->name)
					 && (SelSetPos == 0))
						SelSetPos = showcount;
				}
eloop2:	dcn = dcn->next;
				dcv = dcv->next;
			}
		}
	}

	ShowSettings(showset, vnsect);
}



// Settings processing

bool CMgr::SetPanelEnabled = true;
DCvlist CMgr::SettingObjsShown;
wxString CMgr::VKSetNewName;
wxString CMgr::VKSetRename;


void ConfigMgr::ShowSettings(const char *setname, bool vname)
{
	m_setlistctrl->DeleteAllItems();
	bool vkey = true;
	if (CMgr::AvailSection)
		vkey = (CMgr::AvailSection->settyp > stfix);
	CMgr::AvailSetting =  NULL;

	if (SettingsShown.IsEmpty()
	 && !vkey      // unless VK
	 && !vname) {  // or VN
		m_sectresult->Clear();
		m_sectresult->WriteText(wxString("No suitable settings found in section."));
		CMgr::SetPanelEnabled = false;
		return;
	}

	m_sectresult->Clear();
	m_sectresult->WriteText(wxString("Section selected."));
	CMgr::SetPanelEnabled = true;
	m_setresult->Clear();

	bool sectop = false;

	SSetting *set = NULL;
	const char *name = NULL;
	char *htxt = NULL;
	long si = 0;
	CSetting *cset = NULL;
	long setpos = -1;
	bool preset = false;

	if (!setname
	 && !SettingNameShown.IsEmpty()) {
		setname = SettingNameShown.c_str();
		preset = true;
	}

	// columns are zero-based
	long cnt = SettingsShown.GetCount();

	if (cnt) {
		for (long i = 0; i < cnt; i++) {
			m_setlistctrl->InsertItem(i, SettingsShown.Item(i));
			m_setlistctrl->SetItem(i, 1, SettingValsShown.Item(i));
			// **** get setting help text
			name = (SettingsShown.Item(i)).c_str();
			if ((setpos == -1)
			 && setname
			 && !stricmp(setname, name)) {
				setpos = i;
				if (preset)
					SelSetPos = i;
			}
			if (CMgr::AvailSection
			 && ((si = CMgr::AvailSection->fksetnames.getitem(name)) != 0)
			 && ((set = (SSetting *) CMgr::AvailSection->fksettings.find(si)) != NULL))
				htxt = set->htext;
			else
				htxt = wxT("No help for setting");
			m_setlistctrl->SetItem(i, 2, htxt);
			if (CMgr::IncludeAllSettings
			 && (i > SettingsShownCurr))
				m_setlistctrl->SetItemTextColour(i, *SetColor);  // available FK
			if ((cset = (CSetting *) CMgr::SettingObjsShown.find(i + 1)) != NULL) {
				if (cset->settyp == stvar)
					m_setlistctrl->SetItemTextColour(i, *VarColor);  // VK setting
				if (cset->isinvalid)
					m_setlistctrl->SetItemTextColour(i, *InvalidColor);
			}
		}

		if (cnt == 1)
			setpos = 0;

		if (setpos > -1) {
			SelSetPos = setpos;
			m_setlistctrl->EnsureVisible(SelSetPos);
			m_setlistctrl->SetItemState(SelSetPos, wxLIST_STATE_SELECTED | wxLIST_STATE_FOCUSED,
																						 wxLIST_STATE_SELECTED);
			SetSetButtons(SelSetPos);
		}
		else
			SelSetPos = -1;
	}
	else
		m_setresult->Clear();

	if ((cnt == 1)
	 && !CMgr::ChildCM
	 && (CMgr::CurrAct != actaddsv)
	 && (CMgr::CurrAct != actaddsb)
	 && (CMgr::CurrAct != actrensv))
		ApplySettings();
	else {
		if ((	m_setvkaddtext->IsEnabled()
		  || m_setvkrentext->IsEnabled())
		 && !CMgr::ChildCM
		 && CMgr::AvailSection
		 && CMgr::AvailSection->vktypes
		 && CMgr::ValueCats.getitem(CMgr::AvailSection->vktypes))
			m_setvarkeybutton->Enable();
		else
			m_setvarkeybutton->Disable();

		(GetBookCtrl())->ChangeSelection((size_t) pnset);
	}
}


void ConfigMgr::SetUpSettingsCtrl(void)
{
	m_setlistctrl->ClearAll();
  m_setlistctrl->InsertColumn(0, wxT("Name"), wxLIST_FORMAT_LEFT, 130);
  m_setlistctrl->InsertColumn(1, wxT("Value"), wxLIST_FORMAT_LEFT, 80);
  m_setlistctrl->InsertColumn(2, wxT("Purpose"), wxLIST_FORMAT_LEFT, 180);
}


void ConfigMgr::SetSetButtons(long pos)
{
	// called when setting clicked or doubleclicked

	if (pos > -1) {
		SettingNameShown = m_setlistctrl->GetItemText(pos);
		SelSetPos = SettingNameShownPos = pos;
	}

	// does setting exist?
	bool exist = (pos > -1)
						   && (!CMgr::IncludeAllSettings
		            || (pos <= SettingsShownCurr));

	wxString setname;
	if (((CMgr::CurrAct == actaddsv)
	  || (CMgr::CurrAct == actaddsb))
	 && !CMgr::VKSetNewName.IsEmpty())
		setname = CMgr::VKSetNewName;
	else if (pos > -1)
		setname = SettingNameShown;

	if (!setname.IsEmpty()) {
		if (CMgr::AvailSection
		 && (CMgr::AvailSection->settyp == stfix))
			SetAlias = GetAlias(SectionSelected, setname);
		else
			SetAlias = SectAlias;
	}

	wxString txt;

	switch (CMgr::CurrAct) {
		case actadds:
		case actaddsb:
		case actaddsv:
			txt = wxString("Apply to choose file in which to add setting.");
			break;

		case actedits:
			if (!exist)
				txt = wxString("Apply to choose file in which to add setting to edit.");
			else
				txt = wxString("Apply to choose file in which to edit setting.");
			break;

		case actdels:
			if (!exist)
				txt = wxString("Selected setting is not in any current ini.");
			else
				txt = wxString("Apply to choose file in which to delete setting.");
			break;

		case actrests:
			if (!exist)
				txt = wxString("Selected setting is not in any current ini.");
			else
				txt = wxString("Apply to choose file in which to restore setting.");
			break;

		case actrensv:
			if (!exist)
				txt = wxString("Apply to add new setting named per \"rename\" below.");
			else
				txt = wxString("Apply to choose file in which to rename setting.");
			break;

		case actmovs:
			if (!exist)
				txt = wxString("Selected setting is not in any current ini.");
			else
				txt = wxString("Apply to choose file within which to move setting.");
			break;

		case actmovsf:
			if (!exist)
				txt = wxString("Selected setting is not in any current ini.");
			else
				txt = wxString("Apply to choose file from which to move setting.");
			break;

		case actcpys:
			if (!exist)
				txt = wxString("Selected setting is not in any current ini.");
			else
				txt = wxString("Apply to choose file from which to copy setting.");
			break;

		case actdups:
			if (!exist)
				txt = wxString("Selected setting is not in any current ini.");
			else
				txt = wxString("Apply to choose file in which to merge duplicated setting.");
			break;


		default:
			txt = wxString("Apply to choose ini file.");
			break;
	}

	if (!setname.IsEmpty()
	 && !txt.IsEmpty()) {
		m_sethelptext->Clear();
		m_sethelptext->WriteText(txt);
	}
}


void ConfigMgr::SetAvailSetting(long lpos)
{
	long pos = 0;
	const char *sname = SettingNameShown.c_str();
	CSetting *cset = NULL;

	if ((lpos > -1)
	 && ((cset = (CSetting *) CMgr::SettingObjsShown.find(lpos + 1)) != NULL)
	 && cset->sset)
		CMgr::AvailSetting = cset->sset;
	else if (CMgr::AvailSection
	 && ((pos = CMgr::AvailSection->fksetnames.getitem(sname)) != 0))
		CMgr::AvailSetting = (SSetting *) CMgr::AvailSection->fksettings.find(pos);
	else
		CMgr::AvailSetting = NULL;
}


sectstat CMgr::SettingExists(const char *sname)
{
	if (CurrSettingNames.getitem(sname))   // duplicates existing setting
		return sectcurr;
	//if (AvailSettingNames.getitem(sname))  // duplicates FN setting name
	//	return sectavail;
	return sectnone;  // OK  to use for new VN section name
}


void ConfigMgr::DeselectSettings(void)
{
	long pos = m_setlistctrl->GetNextItem(-1, wxLIST_NEXT_ALL, wxLIST_STATE_SELECTED);
	if (pos == -1)
		return;
	m_setlistctrl->SetItemState(pos, ~(wxLIST_STATE_SELECTED | wxLIST_STATE_FOCUSED),
																wxLIST_STATE_SELECTED);
	SelSetPos = -1;
}


void ConfigMgr::ProcVarKeySelect(void)
{
	char *res = NULL;
	char *val = CMgr::AvailSection->vktypes;
	long pickfile = 0;

	if ((pickfile = CMgr::FilePicker.getitem(val)) != 0)
		res = GetFileInfo(pickfile);
	else
		res = InvokeChildCM(val);

	if (res) {
		if (m_setvkaddtext->IsEnabled()) {
			m_setvkaddtext->Clear();
			m_setvkaddtext->WriteText(wxString(res));
		}
		else if (m_setvkrentext->IsEnabled()) {
			m_setvkrentext->Clear();
			m_setvkrentext->WriteText(wxString(res));
		}
	}
}


bool ConfigMgr::CheckSettingName(wxString& name)
{
	if (name.IsEmpty())
		return false;

	const char *np = name.c_str();

	if (*np == ';')  // commeent
		return false;


	for (; *np; np++) {  // with value
		if (*np == '=')
			return false;
	}

	return true;
}


void ConfigMgr::ApplySettings(void)
{
	CMgr::ValPanelEnabled = false;
	CMgr::VselPanelEnabled = false;
	CMgr::SelectingSrcIni = true;

	bool vkset = false;
	bool sectop = false;
	sectstat st = sectnone;

	long pos = m_setlistctrl->GetNextItem(-1, wxLIST_NEXT_ALL, wxLIST_STATE_SELECTED);

	if (CMgr::CurrAct == actfixs)
		CMgr::CurrAct = actdels;

	switch (CMgr::CurrAct) {
		case actaddsv:
		case actaddsb:
			vkset = true;
			CMgr::VKSetNewName = m_setvkaddtext->GetValue();
			if (CMgr::VKSetNewName.IsEmpty()) {
				if (pos == -1) {
 					m_setresult->Clear();
					m_setresult->WriteText(wxString("Name needed for new setting."));
					return;
				}
				else { // selected vk setting for name
					CMgr::VKSetNewName = SettingNameShown;
					m_setvkaddtext->WriteText(SettingNameShown);
				}
			}
			else if (!CheckSettingName(CMgr::VKSetNewName)) {
 				m_setresult->Clear();
				m_setresult->WriteText(wxString("Name contains unacceptable characters."));
				return;
			}
			else
				SettingNameShown = CMgr::VKSetNewName;
			break;

		case actrensv:
			vkset = true;
			CMgr::VKSetRename = m_setvkrentext->GetValue();
			if (pos == -1) {
				m_setresult->Clear();
				m_setresult->WriteText(wxString("Select setting to rename from list."));
				return;
			}
			if (CMgr::VKSetRename.IsEmpty()) {
				m_setresult->Clear();
				m_setresult->WriteText(wxString("Name needed for renamed setting."));
				return;
			}
			else if (!CheckSettingName(CMgr::VKSetRename)) {
 				m_setresult->Clear();
				m_setresult->WriteText(wxString("Name contains unacceptable characters."));
				return;
			}
			break;

		case actmovs:
			break;

		case actmovsf:
			break;

		case actcpys:
			break;

		case actdups:
			break;

		default:
			break;
	}

	if ((pos == -1)
	 && !vkset
	 && !sectop) {
		m_setresult->Clear();
		m_setresult->WriteText(wxString("Select setting before proceeding."));
		return;
	}

	if (vkset
	 && (pos == -1)) {
		SettingValShown = wxString("---");
		UpdateSettingSelections();
		UpdateValueSelections();
	}
	else {
		if (pos > -1) 
			SettingNameShown = m_setlistctrl->GetItemText(pos);
		SettingNameShownPos = pos;

		if (SettingNameShown.IsEmpty()) {
			if (!vkset
			 && !sectop) {
				m_setresult->Clear();
				m_setresult->WriteText(wxString("Select setting before proceeding."));
				return;
			}
		}
		else
			SetAvailSetting(pos);

		wxListItem col1;
		col1.m_mask = wxLIST_MASK_TEXT;
		col1.m_col = 1;
		col1.m_itemId = pos;
		if (pos > -1) {
			m_setlistctrl->GetItem(col1);
			SettingValShown = col1.GetText();
		}

		if (pos > -1) {
			UpdateSettingSelections();
			UpdateValueSelections();
		}
	}

	m_valhelptext->Clear();
	wxString txt;

	switch (CMgr::CurrAct) {
		case actadds:
		case actaddsv:
		case actaddsb:
			txt = wxString("Choose file in which to add setting.");
			break;

		case actedits:
			txt = wxString("Choose file in which to edit setting.");
			break;

		case actdels:
			txt = wxString("Choose file in which to delete setting.");
			break;

		case actrests:
			txt = wxString("Choose file in which to restore setting.");
			break;

		case actrensv:
			txt = wxString("Choose file in which to rename setting.");
			break;

		case actmovs:
			txt = wxString("Choose file within which to move setting.");
			break;

		case actmovsf:
			txt = wxString("Choose file from which to move setting.");
			break;

		case actcpys:
			txt = wxString("Choose file from which to copy setting.");
			break;

		case actdups:
			txt = wxString("Choose file in which to merge duplicated setting.");
			break;

		default:
			break;
	}

	if (!txt.IsEmpty())
		m_valhelptext->WriteText(txt);

	SetUpValues(sectop, vkset);
}


void ConfigMgr::SetUpValues(bool sectop, bool vkset)
{
	m_valsecttext->Clear();
	m_valsecttext->WriteText(SectionSelected);

	m_vselsecttext->Clear();
	m_vselsecttext->WriteText(SectionSelected);

	long snum = 0;
	DClist *refs = NULL;
	long cnt = 0;

	if (sectop) {
		snum = CMgr::CurrSectionNames.getitem(SectionSelected.c_str());
		if (snum) {   // in current files
			refs = (DClist *) CMgr::CurrSectionNameRefs.find(snum);
			if (refs)
				cnt = refs->count();
		}
	}
	else if (!SettingNameShown.IsEmpty()) {
		snum = CMgr::CurrSettingNames.getitem(SettingNameShown.c_str());

		if (!snum
		 && !vkset
		 && !CMgr::IncludeAllSettings) {
			m_setresult->Clear();
			m_setresult->WriteText(wxString("No such setting in current .ini files."));
			CMgr::ValPanelEnabled = false;
			return;
		}

		if (snum) {
			refs = (DClist *) CMgr::CurrSettingNameRefs.find(snum);
			if (refs)
				cnt = refs->count();
			if (!CMgr::IncludeAllSettings
			 && !vkset
			 && (cnt == 0)) {
				m_setresult->Clear();
				m_setresult->WriteText(wxString("No such setting in current .ini files."));
				return;
			}
		}
	}

	m_valsethelptext->Clear();
	m_vselsethelptext->Clear();

	SetValVisibility(sectop);

	if (!sectop) {
		//if (CMgr::AvailSection)
		//	CMgr::AvailSetting = (SSetting *) CMgr::AvailSection->fksettings.find(CMgr::AvailSection->fksetnames.getitem(SettingNameShown));
		if (CMgr::AvailSetting
		 && CMgr::AvailSetting->htext) {
			SettingHelpShown = CMgr::AvailSetting->htext;
			m_valsethelptext->WriteText(SettingHelpShown);
			m_vselsethelptext->WriteText(SettingHelpShown);
		}
	}

	CSection *csect = NULL;
	CSetting *cset = NULL;


	ValuesShown.Clear();
	ValueScopes.Clear();
	ValueFiles.Clear();
	ValueSys.Clear();

	// get the first reference to the setting
	// make sure it is in the selected section;
	//  if not, go on to next reference
	// if its Cini has previni, back up to first previni
	//   and write empty value for the prev ones
	// write its own Cini value
	// if it has nextini, move up and check for the setting 
	//   in the selected section; write it or empty value
	// for display, reverse order so start is at bottom

	if (sectop) {
		for (long i = 1; i <= cnt; i++) {
			csect = (CSection *) (refs->number(i));
			if (!stricmp(csect->name, SectionSelected.c_str())
			 && ((CMgr::CurrAct != actdup)
			  || csect->hassectdup))
				break; // first appropriate csect found
			csect = NULL;
		}
	}
	else {
		for (long i = 1; i <= cnt; i++) {
			cset = (CSetting *) (refs->number(i));
			if (!stricmp(cset->sect->name, SectionSelected.c_str())
			 && ((CMgr::CurrAct != actdups)
			  || cset->hassetdup))
				break; // first appropriate cset found
			cset = NULL;
		}
	}

	long setidx = 0;

	CIni *ci = cset ? cset->sect->ifile : (csect ? csect->ifile : NULL);
	CIni *pi = (cset || csect) ? ci->previni : (CIni *) CMgr::CurrIniFiles.find(1);
	while (pi) {
		if (pi->previni)
			pi = pi->previni;
		else
			break;
	}

	char buf[12] = "";

	while (pi
	 && (pi != ci)) {
		ValuesShown.Add(wxT("---"));
		ValueScopes.Add(GetIniScope(pi));
		ValueFiles.Add(wxString(pi->name));
		ValueSys.Add(wxString(ultoa((unsigned long) pi, buf, 10)));
		setidx++;	
		pi = pi->nextini;
	}

	if (!cset
	 && !sectop) {
		ShowValues(sectop);
		return;
	}

	if (ci) {
		ValuesShown.Add(sectop ? SectionSelected : wxString(cset->cval));
		ValueScopes.Add(GetIniScope(ci));
		ValueFiles.Add(wxString(ci->name));
		ValueSys.Add(wxString(ultoa((unsigned long) ci, buf, 10)));
		setidx++;
	}

	CIni *ni = ci ? ci->nextini : NULL;
	const char *nextval = NULL;
	CSection *nsect = NULL;
	CSetting *nset = NULL;

	while (ni) {
		long scnt = ni->sectcount;
		for (long s = 1; s <= scnt; s++) {
			nsect = (CSection *) ni->sections.find(s);
			if (nsect
			 && nsect->name
			 && !stricmp(nsect->name, SectionSelected.c_str())
			 && ((CMgr::CurrAct != actdup)
			  || nsect->hassectdup)
			 && ((CMgr::CurrAct != actdups)
			  || nsect->hassetdup)) {
				if (sectop)
					nextval = SectionSelected.c_str();
				else if (nsect->setcount) {
					for (long t = 1; t <= nsect->setcount; t++) {
						nset = (CSetting *) nsect->settings.find(t);
						if (nset
						 && nset->name
						 && !stricmp(nset->name, SettingNameShown.c_str())
						 && ((CMgr::CurrAct != actdups)
						  || nset->hassetdup)) {
							nextval = nset->cval;
							break;
						}
					}
				}
			}
			if (nextval) {
				ValuesShown.Add(wxString(nextval));
				break;
			}
		}
		if (nextval)
			nextval = NULL;
		else
			ValuesShown.Add(wxT("---"));
		ValueScopes.Add(GetIniScope(ni));
		ValueFiles.Add(wxString(ni->name));
		ValueSys.Add(wxString(ultoa((unsigned long) ni, buf, 10)));
		setidx++;
		ni = ni->nextini;
	}

	ShowValues(sectop);
}


wxString ConfigMgr::GetIniScope(CIni *ci)
{
	if (ci->scope)
		return wxString(ci->scope);
	if (ci->sys)
		return wxString("System");
	if (ci->path
	 && !strcmp(ci->path, CMgr::StartFileName))
		return wxString("Current project only");
	return wxString("Unknown");
}


void ConfigMgr::UpdateSettingSelections(void)
{
	SettingSelected = SettingNameShown;;
	SettingSelected << "=" << SettingValShown;

	m_valsettext->Clear();
	m_valsettext->WriteText(SettingSelected);

	m_vselsettext->Clear();
	m_vselsettext->WriteText(SettingSelected);
}


void ConfigMgr::UpdateValueSelections(void)
{
	m_currvaltext->Clear();
	m_currvaltext->WriteText(SettingValShown);
;
	m_vselcurrvaltext->Clear();
	m_vselcurrvaltext->WriteText(SettingValShown);

	if (!SettingValsShown.IsEmpty()
	 && (SelSetPos > -1)) {
		SettingValsShown.Item(SelSetPos) = SettingValShown;
		m_setlistctrl->SetItem(SelSetPos, 1, SettingValShown);
	}
}


void ConfigMgr::SetValVisibility(bool sectop)
{
	m_valpanel->Freeze();

	m_valset->Show(!sectop);
	m_valsettext->Show(!sectop);
	m_valsetsizer->Layout();

	m_valsethelp->Show(!sectop);
	m_valsethelptext->Show(!sectop);
	m_valsethelpsizer->Layout();

	m_currval->Show(!sectop);
	m_currvaltext->Show(!sectop);
	m_valcurrvalsizer->Layout();

	m_valpanelsizer->Layout();
	m_valpanel->Thaw();
}



// Values panel


bool CMgr::ValPanelEnabled = true;

DCnlist CMgr::ScopeNames;
DCnlist CMgr::ScopeVals;
long CMgr::ScopeCount;

CIni *CMgr::SelectedIni = NULL;
CSection *CMgr::SelectedSect = NULL;
CSetting *CMgr::SelectedSet = NULL;
long CMgr::SelIniPos = 0;
bool CMgr::SelectingSrcIni = true;
bool CMgr::SectInIni = false;

void ConfigMgr::ShowValues(bool sectop)
{
	m_selinictrl->DeleteAllItems();  // leaves columns set up
	m_setresult->Clear();

	if (ValuesShown.IsEmpty()
	 && !sectop) {
		if ((SettingNameShownPos == -1)
		 && !SettingsShown.IsEmpty())
			m_setresult->WriteText(wxString("Select setting before proceeding."));
		else
			m_setresult->WriteText(wxString("Cannot find any ini files suitable for this setting."));
		CMgr::ValPanelEnabled = false;
		return;
	}

	if (sectop) {
		m_sectresult->Clear();
		m_sectresult->WriteText(wxString("Section to process selected."));
		ValAlias = GetAlias(SectionSelected);
	}
	else if (SetAlias) {
		ValAlias = SetAlias;
	}
	else {
		wxString setname;
		m_setresult->WriteText(wxString("Setting selected."));
		if (CMgr::CurrSectVK
		 && !CMgr::VKSetNewName.IsEmpty())
			setname = CMgr::VKSetNewName;
		else
			setname = SettingNameShown;
		ValAlias = GetAlias(SectionSelected, setname);
	}
	CMgr::ValPanelEnabled = true;

	// columns are zero-based
  m_selinictrl->InsertItem(0, "Internal default");
  m_selinictrl->SetItemData(0, (long) 0);
	if (sectop)
	 m_selinictrl->SetItem(0, 2, wxString("Not applicable for sections"));
	else if (CMgr::AvailSetting)
	 m_selinictrl->SetItem(0, 2, CMgr::AvailSetting->dval);
  m_selinictrl->SetItemTextColour(0, *DefColor);

	long cnt = ValuesShown.GetCount();
	CIni *sys = NULL;
	wxString ininame;
	long selpos = -1;
	wxString setininame;

	if ((CMgr::CurrAct > actmovs)
	 && (CMgr::CurrAct < actdup)  // has src and dest inis
	 && CMgr::SelectingSrcIni)
		setininame = SrcIniName;
	else
		setininame = DestIniName;

	if (setininame.IsEmpty()
	 && IniNameShown)
		setininame = IniNameShown;

	for (long i = 1; i <= cnt; i++) {
		sys = (CIni *) strtoul(ValueSys.Item(cnt - i).c_str(), NULL, 10);
    m_selinictrl->InsertItem(i, ValueScopes.Item(cnt - i));
    m_selinictrl->SetItemData(i, (unsigned long) sys);
    ininame = ValueFiles.Item(cnt - i);
    m_selinictrl->SetItem(i, 1, ininame);
		if (ininame == setininame) {
			selpos = i;
			IniNameShown = ininame;
		}
    m_selinictrl->SetItem(i, 2, ValuesShown.Item(cnt - i));
		if (sys->sys)
		  m_selinictrl->SetItemTextColour(i, *SysColor);
	}

	if (selpos == -1)
		selpos = cnt;

	m_selinictrl->SetItemState(selpos, wxLIST_STATE_SELECTED | wxLIST_STATE_FOCUSED,
		                              wxLIST_STATE_SELECTED);
	m_selinictrl->EnsureVisible(selpos);
	SelIniSelection = selpos;

	m_valresult->Clear();
	(GetBookCtrl())->ChangeSelection((size_t) pnini);
}


void ConfigMgr::SetUpSelIniCtrl( void )
{
	m_selinictrl->ClearAll();
  m_selinictrl->InsertColumn(0, wxT("Scope"), wxLIST_FORMAT_LEFT, 120);
  m_selinictrl->InsertColumn(1, wxT("Ini Name"), wxLIST_FORMAT_LEFT, 140);
  m_selinictrl->InsertColumn(2, wxT("Value"), wxLIST_FORMAT_LEFT, 150);
}


void ConfigMgr::AdjustIniSel(void)
{
	SelIniSelection = CMgr::SelIniPos;
	long lim = m_selinictrl->GetItemCount() - 1;
	CIni *ci = NULL;

	if ((SelIniSelection < 1)
	 && (lim >= 0)) {
		SelIniSelection = 1;
		if (!CMgr::AllowSysIniSelect) {
			while (((ci = (CIni *) m_selinictrl->GetItemData(SelIniSelection)) != NULL)
			 && (ci->sys == true)
			 && (SelIniSelection < lim))
				SelIniSelection++;
		}
		m_selinictrl->SetItemState(SelIniSelection, 
			                         wxLIST_STATE_SELECTED | wxLIST_STATE_FOCUSED,
															 wxLIST_STATE_SELECTED);
		CMgr::SelIniPos = SelIniSelection;
		return;
	}

	if (CMgr::AllowSysIniSelect
	 || ((CMgr::SelectingSrcIni)
	  && ((CMgr::CurrAct == actcpy)
	   || (CMgr::CurrAct == actcpys)))
	 || (SelIniSelection >= lim))
		return;

	while (((CIni *) m_selinictrl->GetItemData(SelIniSelection))->sys
	 && (SelIniSelection < lim))
		SelIniSelection++;
	if (SelIniSelection != CMgr::SelIniPos) {
		m_selinictrl->SetItemState(SelIniSelection,
			                         wxLIST_STATE_SELECTED | wxLIST_STATE_FOCUSED,
															 wxLIST_STATE_SELECTED);
		CMgr::SelIniPos = SelIniSelection;
	}
}


void ConfigMgr::ApplyIniVals(void)
{
	CMgr::SelIniPos = m_selinictrl->GetNextItem(-1, wxLIST_NEXT_ALL, wxLIST_STATE_SELECTED);
	CMgr::VselPanelEnabled = false;

	AdjustIniSel();

	if (CMgr::SelIniPos < 1) {
		m_valresult->Clear();
		m_valresult->WriteText(wxString("Select .ini file before proceeding."));
		return;
	}

  wxListItem col1;
  col1.m_mask = wxLIST_MASK_TEXT;
  col1.m_col = 1;
  col1.m_itemId = CMgr::SelIniPos;
	m_selinictrl->GetItem(col1);
	IniNameShown = col1.GetText();

	if (IniNameShown.IsEmpty()) {
		m_valresult->Clear();
		m_valresult->WriteText(wxString("Select .ini file before proceeding."));
		return;
	}

  wxListItem col2;
  col2.m_mask = wxLIST_MASK_TEXT;
  col2.m_col = 2;
  col2.m_itemId = CMgr::SelIniPos;
	m_selinictrl->GetItem(col2);
	IniValShown = col2.GetText();

	if (IniValShown.IsEmpty()
	 || (IniValShown == "---"))
		CMgr::SectInIni = false;
	else
		CMgr::SectInIni = true;

	optyp sectop = opnone;
	const char *sect = SectionSelected.c_str();;
	const char *nsect = NULL;
	const char *rsect = NULL;
	const char *set = NULL;
	const char *rset = NULL;
	const char *sval = NULL;
	const char *path = NULL;
	CMgr::SelectedIni = NULL;

	DCvlist *dvl = &CMgr::CurrIniFiles;
	while (dvl->next) {
		dvl = dvl->next;
		if (!strcmp(IniNameShown.c_str(), ((CIni *) (dvl->vp))->name)) {
			CMgr::SelectedIni = (CIni *) (dvl->vp);
			break;
		}
	}

	if (CMgr::SelectedIni)
		path = CMgr::SelectedIni->path;
	else {
		m_valresult->Clear();
		m_valresult->WriteText(wxString("Cannot find selected .ini file."));
		return;
	}

	bool macro = false;
	bool list = true;
	bool sectlist = false;
	bool setlist = false;
	bool copy = false;
	bool move = false;
	bool merge = false;
	bool interfile = false;

	m_vselmac = m_vselmacedit;
	m_vselmacedit->Show(false);
	m_vselmacadd->Show(false);
	//SetVselVisibility(macro, list);

	if (CMgr::CurrAct == actfixs)
		CMgr::CurrAct = actdels;

	switch (CMgr::CurrAct) {
		case actadd:
			m_valresult->Clear();
			nsect = NULL;
			if (CMgr::AvailSection          // FN or error
			 && CMgr::AvailSection->name)
				nsect = CMgr::AvailSection->name;
			if (!nsect) {
				m_valresult->WriteText(wxString("No new name for new fixed-name section."));
				return;
			}
			m_valresult->WriteText(wxString("Adding fixed-name section in selected .ini file."));
			SectionSelected = wxString(nsect);
			macro = true;
			if (!CMgr::SectInIni)
				m_vselmac = m_vselmacadd;
			SetVselVisibility(macro);
			break;

		case actaddv:
			nsect = NULL;
			if (!CMgr::VNSectNewName.IsEmpty())   // VN
				nsect = CMgr::VNSectNewName.c_str();
			if (!nsect) {
				m_valresult->WriteText(wxString("No new name for new variable-name section."));
				return;
			}
			m_valresult->WriteText(wxString("Adding variable-name section in selected .ini file."));
			SectionSelected = wxString(nsect);
			macro = true;
			if (!CMgr::SectInIni)
				m_vselmac = m_vselmacadd;
			SetVselVisibility(macro);
			break;

		case actadds:
			m_valresult->Clear();
			m_valresult->WriteText(wxString("Adding fixed-key setting to selected .ini file."));
			break;

		case actaddsv:
			m_valresult->Clear();
			if (CMgr::VKSetNewName.IsEmpty()) { // VK
				m_valresult->WriteText(wxString("No new name for new variable-key setting."));
				return;
			}
			m_valresult->WriteText(wxString("Adding variable-key setting to selected .ini file."));
			SettingNameShown = CMgr::VKSetNewName;
			break;

		case actedit:
			macro = true;
			if (!CMgr::SectInIni)
				m_vselmac = m_vselmacadd;
			SetVselVisibility(macro);
			m_valresult->Clear();
			m_valresult->WriteText(wxString("Editing full section in selected .ini file."));
			break;

		case actedits:
			m_valresult->Clear();
			m_valresult->WriteText(wxString("Changing value in selected .ini file."));
			break;

		case actdel:
			macro = true;
			m_valresult->Clear();
			if (!strcmp(IniValShown.c_str(), "---")) { // error
				m_valresult->WriteText(wxString("Section.not found in selected .ini file."));
				return;
			}
			SetVselVisibility(macro);
			m_valresult->WriteText(wxString("Deleting section from selected .ini file."));
			break;

		case actdels:
			m_valresult->Clear();
			if (!strcmp(IniValShown.c_str(), "---")) { // error
				m_valresult->WriteText(wxString("Setting.not found in selected .ini file."));
				return;
			}
			m_valresult->WriteText(wxString("Deleting setting from selected .ini file."));
			break;

		case actrest:
			macro = true;
			m_valresult->Clear();
			if (!strcmp(IniValShown.c_str(), "---")) { // error
				m_valresult->WriteText(wxString("Deleted section.not found in selected .ini file."));
				return;
			}
			SetVselVisibility(macro);
			m_valresult->WriteText(wxString("Restoring deleted section in selected .ini file."));
			break;

		case actrests:
			m_valresult->Clear();
			if (!strcmp(IniValShown.c_str(), "---")) { // error
				m_valresult->WriteText(wxString("Deleted setting.not found in selected .ini file."));
				return;
			}
			m_valresult->WriteText(wxString("Restoring deleted setting in selected .ini file."));
			break;

		case actrenv:
			m_valresult->Clear();
			if (!strcmp(IniValShown.c_str(), "---")) { // error
				m_valresult->WriteText(wxString("Section.not found in selected .ini file."));
				return;
			}
			if (CMgr::VNSectRename.IsEmpty()) { // error
				m_valresult->WriteText(wxString("No new name for renamed section."));
				return;
			}
			macro = true;
			SetVselVisibility(macro);
			m_vselrentext->Clear();
			m_vselrentext->WriteText(CMgr::VNSectRename);
			m_valresult->WriteText(wxString("Renaming section in selected .ini file."));
			break;

		case actrensv:
			m_valresult->Clear();
			if (!strcmp(IniValShown.c_str(), "---")) { // error
				m_valresult->WriteText(wxString("Setting.not found in selected .ini file."));
				return;
			}
			if (CMgr::VKSetRename.IsEmpty()) {  // error
				m_valresult->WriteText(wxString("No new name for renamed setting."));
				return;
			}
			m_valresult->WriteText(wxString("Renaming setting in selected .ini file."));
			SetVselVisibility(macro);
			m_vselrentext->Clear();
			m_vselrentext->WriteText(CMgr::VKSetRename);
			break;

		case actmov:
			if (!strcmp(IniValShown.c_str(), "---")) { // error
				m_valresult->Clear();
				m_valresult->WriteText(wxString("Section.not found in selected .ini file."));
				return;
			}
			move = list = sectlist = true;
			break;

		case actmovs:
			if (!strcmp(IniValShown.c_str(), "---")) { // error
				m_valresult->Clear();
				m_valresult->WriteText(wxString("Setting.not found in selected .ini file."));
				return;
			}
			move = list = setlist = true;
			break;

		case actmovf:
			if (CMgr::SelectingSrcIni) {
				m_valresult->Clear();
				if (!strcmp(IniValShown.c_str(), "---")) { // error
					m_valresult->WriteText(wxString("Section.not found in selected .ini file."));
					return;
				}
				SetSrcIniVars();
				CMgr::SelectingSrcIni = false;
				ShowValues(true);
				m_valhelptext->Clear();
				m_valhelptext->WriteText(wxString("Choose file to which to move section."));
				m_valresult->WriteText(wxString("Select destination ini for moved section."));
				return;
			}
			if (strcmp(IniValShown.c_str(), "---")) { // error
				m_valresult->Clear();
				m_valresult->WriteText(wxString("Section.already exists in destination .ini."));
				return;
			}
			SetDestIniVars();
			move = interfile = macro = true;
			break;

		case actmovsf:
			if (CMgr::SelectingSrcIni) {
				m_valresult->Clear();
				if (!strcmp(IniValShown.c_str(), "---")) { // error
					m_valresult->WriteText(wxString("Setting.not found in selected .ini file."));
					return;
				}
				SetSrcIniVars();
				CMgr::SelectingSrcIni = false;
				ShowValues(false);
				m_valhelptext->Clear();
				m_valhelptext->WriteText(wxString("Choose file to which to move setting."));
				m_valresult->WriteText(wxString("Select destination ini for moved setting."));
				return;
			}
			if (strcmp(IniValShown.c_str(), "---")) { // error
				m_valresult->Clear();
				m_valresult->WriteText(wxString("Setting.already exists in destination .ini."));
				return;
			}
			SetDestIniVars();
			move = interfile = true;
			break;

		case actcpy:
			if (CMgr::SelectingSrcIni) {
				m_valresult->Clear();
				if (!strcmp(IniValShown.c_str(), "---")) { // error
					m_valresult->WriteText(wxString("Section.not found in selected .ini file."));
					return;
				}
				SetSrcIniVars();
				CMgr::SelectingSrcIni = false;
				ShowValues(true);
				m_valhelptext->Clear();
				m_valhelptext->WriteText(wxString("Choose file to which to copy section."));
				m_valresult->WriteText(wxString("Select destination ini for copied section."));
				return;
			}
			if (strcmp(IniValShown.c_str(), "---")) { // error
				m_valresult->Clear();
				m_valresult->WriteText(wxString("Section.already exists in destination .ini."));
				return;
			}
			SetDestIniVars();
			copy = interfile = macro = true;
			break;

		case actcpys:
			if (CMgr::SelectingSrcIni) {
				m_valresult->Clear();
				if (!strcmp(IniValShown.c_str(), "---")) { // error
					m_valresult->WriteText(wxString("Setting.not found in selected .ini file."));
					return;
				}
				SetSrcIniVars();
				CMgr::SelectingSrcIni = false;
				ShowValues(false);
				m_valhelptext->Clear();
				m_valhelptext->WriteText(wxString("Choose file to which to copy setting."));
				m_valresult->WriteText(wxString("Select destination ini for copied setting."));
				return;
			}
			if (strcmp(IniValShown.c_str(), "---")) { // error
				m_valresult->Clear();
				m_valresult->WriteText(wxString("Setting.already exists in destination .ini."));
				return;
			}
			SetDestIniVars();
			copy = interfile = true;
			break;

		case actdup:
			macro = true;
			merge = true;
			m_valresult->Clear();
			if (!strcmp(IniValShown.c_str(), "---")) { // error
				m_valresult->WriteText(wxString("Section.not found in selected .ini file."));
				return;
			}
			SetVselVisibility(macro);
			m_valresult->WriteText(wxString("Merging duplicated section in selected .ini file."));
			break;

		case actdups:
			merge = true;
			m_valresult->Clear();
			if (!strcmp(IniValShown.c_str(), "---")) { // error
				m_valresult->WriteText(wxString("Setting.not found in selected .ini file."));
				return;
			}
			m_valresult->WriteText(wxString("Merging duplicated setting in selected .ini file."));
			break;

		default:
			m_valresult->Clear();
			m_valresult->WriteText(wxString("Action not supported yet."));
			return;
	}

	CMgr::SelectedSect = NULL;
	CSection *csect = NULL;
	if (CMgr::SelectedIni) {
		dvl = &(CMgr::SelectedIni->sections);
		while (dvl->next) {
			dvl = dvl->next;
			csect = (CSection *) (dvl->vp);
			if (!strcmp(SectionSelected.c_str(), csect->name)
			 && ((CMgr::CurrAct != actdup)
			  || csect->hassectdup)
			 && ((CMgr::CurrAct != actdups)
			  || csect->hassetdup)) {
				CMgr::SelectedSect = csect;
				break;
			}
		}
	}
	csect = CMgr::SelectedSect;


	CMgr::SelectedSet = NULL;
	CSetting *cset = NULL;
	if (CMgr::SelectedSect
	 && !macro) {
		dvl = &(CMgr::SelectedSect->settings);
		while (dvl->next) {
			dvl = dvl->next;
			cset = (CSetting *) (dvl->vp);
			if (!strcmp(SettingNameShown.c_str(), cset->name)
			 && ((CMgr::CurrAct != actdups)
				|| cset->hassetdup)) {
				CMgr::SelectedSet = cset;
				break;
			}
		}
	}
	cset = CMgr::SelectedSet;

	// check for special processing
	char *valname = NULL;
	bool ispara = true;
	if (CMgr::AvailSection
	 && CMgr::AvailSection->vkvals
	 && (*(CMgr::AvailSection->vkvals) == '*')) {
		valname = CMgr::AvailSection->vkvals + 1;
		if (CMgr::AvailSection->vkecount > 1) {  //  choice of VK enums
			if (!stricmp(valname, "E:HTMProp")) {  // different enums for char and para settings
				if (CMgr::AvailSection->vkecount == 2) {
					// determine if setting is para or char, set ispara
					if (!stricmp(SettingNameShown.Right(4).c_str(), "Char"))
						ispara = false;
					if (ispara)  // first in list
						CMgr::AvailSection->vkenum = (SEnum *) CMgr::AvailSection->vkelist.next->id;
					else  // char, second in list
						CMgr::AvailSection->vkenum = (SEnum *) CMgr::AvailSection->vkelist.next->next->id;
					CMgr::AvailSection->vkvhtext = CMgr::AvailSection->vkenum->vhtext;
				}
			}
		}
	}
	else if (CMgr::AvailSetting
	 && CMgr::AvailSetting->fkvals
	 && (*(CMgr::AvailSetting->fkvals) == '*')) {
		valname = CMgr::AvailSetting->fkvals + 1;
		if (CMgr::AvailSetting->fkecount > 1) {  //  choice of FK enums

		}
	}

	bool invalset = false;
	if (csect
	 && csect->ssect
	 && cset)
		invalset = cset->isinvalid;


	// gather info for setting new value

	m_vselinitext->Clear();
	m_vselinitext->WriteText(IniNameShown);
	m_vselvaltext->Clear();
	m_vselvalhelptext->Clear();

	DCnlist *dnl = NULL;
	//CSection *csect = NULL;
	//CSetting *cset = NULL;
	SettingValsAvailable.Clear();
	SettingValsHelp.Clear();
	SettingValsPos = 0;
	wxString vstr;
	wxString vhstr;
	long pos = 0;
	char *bptr = NULL;
	char *bpend = NULL;

	// get values possible for SettingNameShown in  SectionSelected from SysIni
	if (invalset) {  // get names for possible settings in this section
		SettingValsAvailable.Add(vstr = "---");
		SettingValsHelp.Add(vhstr = "---");

#if 0
		dnl = &(csect->ssect->fksetnames);
		SSetting *sset = NULL;
		while ((dnl = dnl->next) != NULL) {
			if (dnl->name) {
				vhstr.Empty();
				SettingValsAvailable.Add(wxString(dnl->name));
				if (dnl->id) {
					sset = (SSetting *) (csect->ssect->fksettings.find(dnl->id));
					vhstr = wxString(sset->htext);
				}
				if (vhstr.IsEmpty())
					vhstr = wxString("No help for this setting");
				SettingValsHelp.Add(vhstr);
			}
		}
		SettingValsPos = 0; // first instance until we have better matching
		long len = SettingNameShown.Length();
		long pos = 0;
		bptr = NewName(SettingNameShown.c_str(), len + 1);
		bpend = bptr + len;
		while (bpend > bptr) {
			*bpend = '*';
			*(bpend + 1) = '\0';
			if ((pos = csect->ssect->fksetnames.getwild(bptr)) != 0) {
				SettingValsPos = pos - 1;
				break;
			}
			bpend--;
		}
		vstr = SettingValsAvailable.Item(SettingValsPos);
		vhstr = SettingValsHelp.Item(SettingValsPos);
		macro = false;
		CMgr::CurrAct = actfixs;
#endif
	}
	else if (macro) {
		vstr = "---";
		if (GetMacroValue())
			MacroValue = wxString(CMgr::MacroBuffer);
		else
			MacroValue = wxString("");
		SettingValsAvailable.Add(MacroValue);
		if ((pos = CMgr::MacroNames.getitem(SectionSelected.c_str())) != 0)
			vhstr = CMgr::MacroHelp.find(pos);
		else
			vhstr = "---";
		SettingValsHelp.Add(vhstr);
	}
	else if (merge) {  // get values for each setting instance
		DCnlist sn;
		bool isenum = false;
		long eval = 0;
		wxString vh;
		if (CMgr::AvailSetting
		 && CMgr::AvailSetting->fkenum)   // set of enums
			isenum = true;
		if (ConfigMgr::GetIniFileAll(SectionSelected.c_str(),
		                             SettingNameShown.c_str(),
		                             CMgr::MacroBuffer, 4096, 
			                           CMgr::SelectedIni->path)) {
			// parse macro buffer for nul-delimited setting values
			bptr = CMgr::MacroBuffer;
			pos = 0;
			vhstr = "---";
			SettingValsPos = 0; // always use first instance
			while (*bptr) {
				bpend = bptr;
				while (*bpend)
					bpend++;
				if ((*bptr == ' ')
				 && (*(bptr + 1) != ' '))
					bptr++;
				if (!pos)
					vstr = wxString(bptr);
				if (sn(bptr) == 0) {  // not already in list
					SettingValsAvailable.Add(wxString(bptr));
					sn.add(bptr, ++pos);
					if (isenum
					 && ((eval = CMgr::AvailSetting->fkenum->vals.getitem(bptr)) != 0)
					 && !(vh = CMgr::AvailSetting->fkenum->valhtext.find(eval)).IsEmpty())
						SettingValsHelp.Add(vh);
					else
						SettingValsHelp.Add(vhstr);
				}
				bptr = bpend + 1;
			}
		}
		// add any from avail that are not already included
		if (isenum) {
			wxString vs;
			dnl = &(CMgr::AvailSetting->fkenum->vals);
			while (dnl->next) {
				dnl = dnl->next;
				vs = dnl->name;
				if (sn(dnl->name) == 0) {
					SettingValsAvailable.Add(vs);
					SettingValsHelp.Add(vh = CMgr::AvailSetting->fkenum->valhtext.find(dnl->id));
				}
			}
		}
	}
	else if (move) {
		long pos = 0;
		if (sectlist) {  // list of sections or settings
			dvl = &(CMgr::SelectedIni->sections);
			while (dvl->next) {
				dvl = dvl->next;
				csect = (CSection *) (dvl->vp);
				if (!stricmp(csect->name, "end"))
					continue;
				SettingValsAvailable.Add(wxString(csect->name));
				if (!stricmp(csect->name, CMgr::SelectedSect->name))
					SettingValsPos = pos;
				vhstr = wxString("---");
				if (csect->htext)
					vhstr = wxString(csect->htext);
				else if (csect->ssect) {
					if (csect->ssect->htext)
						vhstr = wxString(csect->ssect->htext);
					else if (csect->ssect->vkshtext)
						vhstr = wxString(csect->ssect->vkshtext);
				}
				SettingValsHelp.Add(vhstr);
				pos++;
			}
		}
		else if (setlist) {
			dvl = &(CMgr::SelectedSect->settings);
			while (dvl->next) {
				dvl = dvl->next;
				cset = (CSetting *) (dvl->vp);
				SettingValsAvailable.Add(wxString(cset->name));
				if (!stricmp(cset->name, CMgr::SelectedSet->name))
					SettingValsPos = pos;
				vhstr = wxString("---");
				if (cset->sset) {
					if (cset->sset->htext)
						vhstr = wxString(cset->sset->htext);
				}
				SettingValsHelp.Add(vhstr);
				pos++;
			}
		}
		else { // moving a single setting
			cset = CMgr::SelectedSet;
			SettingValsAvailable.Add(vstr = cset->cval);
			vhstr = wxString("---");
			if (cset->sset) {
				if (cset->sset->htext)
					vhstr = wxString(cset->sset->htext);
			}
			SettingValsHelp.Add(vhstr);
		}
	}
	else if (copy) {  // copying a single setting
		cset = CMgr::SelectedSet;
		SettingValsAvailable.Add(vstr = cset->cval);
		vhstr = wxString("---");
		if (cset->sset) {
			if (cset->sset->htext)
				vhstr = wxString(cset->sset->htext);
		}
		SettingValsHelp.Add(vhstr);
	}
	else if (CMgr::AvailSetting
	 && !CMgr::AvailSetting->fkenum
	 && CMgr::AvailSetting->htext) {  // single or no value in fkvals
		if (CMgr::SelectedSet
		 && CMgr::SelectedSet->cval)    // use current value if any
			vstr = CMgr::SelectedSet->cval;
		else if (!stricmp(CMgr::AvailSetting->name, "help")
		 || !stricmp(CMgr::AvailSetting->name, "category"))
			// use effective value as default, to keep Carolyn happy
			vstr = m_currvaltext->GetValue();
		else                            // use internal default
			vstr = CMgr::AvailSetting->dval;
		SettingValsAvailable.Add(vstr);
		SettingValsHelp.Add(vhstr = CMgr::AvailSetting->htext);
	}
	else if (CMgr::AvailSetting
	 && CMgr::AvailSetting->fkenum) {  // set of enums
		long pos = 0;
		wxString vs;
		wxString vh;
		vstr.Empty();
		vhstr.Empty();
		SettingValsPos = -1;
		dnl = &(CMgr::AvailSetting->fkenum->vals);
		while (dnl->next) {
			dnl = dnl->next;
			SettingValsAvailable.Add(vs = dnl->name);
			if (vstr.IsEmpty())
				vstr = vs;
			SettingValsHelp.Add(vh = CMgr::AvailSetting->fkenum->valhtext.find(dnl->id));
			if (vhstr.IsEmpty())
				vhstr = vh;
			if (!stricmp(dnl->name, (CMgr::SelectedSet ?
			                         CMgr::SelectedSet->cval :
															 CMgr::AvailSetting->dval))) {
				SettingValsPos = pos;
				vstr = vs;
				vhstr = vh;
			}
			pos++;
		}
		if (CMgr::AvailSetting->vhtext)
			vhstr = CMgr::AvailSetting->vhtext;
	}
	else if (CMgr::AvailSetting
	 && CMgr::AvailSetting->vhtext) { // single VK value 
		SettingValsAvailable.Add(vstr = (CMgr::SelectedSet ?
			                               CMgr::SelectedSet->cval :
																		 CMgr::AvailSetting->dval));
		SettingValsHelp.Add(vhstr = CMgr::AvailSetting->vhtext);
	}
	else if (CMgr::AvailSection
	 && CMgr::AvailSection->vkenum) {  // set of enums
		long pos = 0;
		wxString vs;
		wxString vh;
		vstr.Empty();
		vhstr.Empty();
		SettingValsPos = -1;
		dnl = &(CMgr::AvailSection->vkenum->vals);
		while (dnl->next) {
			dnl = dnl->next;
			SettingValsAvailable.Add(vs = dnl->name);
			if (vstr.IsEmpty())
				vstr = vs;
			SettingValsHelp.Add(vh = CMgr::AvailSection->vkenum->valhtext.find(dnl->id));
			if (vhstr.IsEmpty())
				vhstr = vh;
			if (CMgr::SelectedSet
			 && !stricmp(dnl->name, CMgr::SelectedSet->cval)) {
				SettingValsPos = pos;
				vstr = vs;
				vhstr = vh;
			}
			pos++;
		}
		if (CMgr::AvailSection->vkvhtext)
			vhstr = CMgr::AvailSection->vkvhtext;
	}
	else if (CMgr::AvailSection
	 && CMgr::AvailSection->vkvhtext) {
		vstr = "---";
		vhstr = CMgr::AvailSection->vkvhtext;
		SettingValsAvailable.Add(vstr);
		SettingValsHelp.Add(vhstr);
	}
	else { // not a clue
		if (CMgr::SelectedSet)
			vstr = CMgr::SelectedSet->cval;
		else 
			vstr = "---";
		SettingValsAvailable.Add(vstr);
		SettingValsHelp.Add(vhstr = "---");
	}
	if (CMgr::CurrAct != actfixs)
		m_vselvaltext->WriteText(vstr);
	m_vselvalhelptext->WriteText(vhstr);

	if (SettingValsAvailable.GetCount() == 1)
		list = false;

	SetVselVisibility(macro, list);
	ShowSelValues(macro, list);
}


void ConfigMgr::SetSrcIniVars(void)
{
	SrcIniPos = CMgr::SelIniPos;
	SrcIniName = IniNameShown;
	SrcIniVal = IniValShown;
	SrcIniSel = CMgr::SelectedIni;
}


void ConfigMgr::SetDestIniVars(void)
{
	DestIniPos = CMgr::SelIniPos;
	DestIniName = IniNameShown;
	DestIniVal = IniValShown;
	DestIniSel = CMgr::SelectedIni;

	CMgr::SelIniPos = SrcIniPos;
	IniNameShown = SrcIniName;
	IniValShown = SrcIniVal;
	CMgr::SelectedIni = SrcIniSel;

	m_vseldesttext->Clear();
	m_vseldesttext->WriteText(wxString(DestIniSel->name));
}


void ConfigMgr::SetVselVisibility(bool macro, bool list)
{
	m_vselpanel->Freeze();
	bool del = false;
	bool rest = false;
	bool ren = false;
	bool mov = false;
	bool cpy = false;
	bool fix = false;
	bool interfile = false;
	bool sect = false;
	bool set = false;
	VselSelectionLocked = false;

	switch (CMgr::CurrAct) {
		case actdel:  // delete section
			del = true;
			sect = true;
			break;

		case actdels:  // delete setting
			del = true;
			set = true;
			break;

		case actrest:  // restore section
			rest = true;
			sect = true;
			break;

		case actrests:  // restore setting
			rest = true;
			set = true;
			break;

		case actrenv:  // rename section
			ren = true;
			sect = true;
			break;

		case actrensv:  // rename setting
			ren = true;
			set = true;
			break;

		case actmov:  // move section in file
			mov = true;
			sect = true;
			VselSelectionLocked = true;
			break;

		case actmovf:  // move section between files
			interfile = true;
			sect = true;
			break;

		case actcpy:  // copy section between files
			cpy = true;
			interfile = true;
			sect = true;
			break;

		case actmovs:  // move setting in section
			mov = true;
			set = true;
			VselSelectionLocked = true;
			break;

		case actmovsf:  // move setting between files
			interfile = true;
			set = true;
			break;

		case actcpys:  // copy setting between files
			cpy = true;
			interfile = true;
			set = true;
			break;

		case actfix:
			fix = true;
			sect = true;
			break;

		case actfixs:
			fix = true;
			set = true;
			break;

		default:
			break;
	}

	if (sect) {
		if (ren || cpy || rest || interfile) {
			m_selval->Show(false);
			m_vselmov->Show(false);
			m_vseldel->Show(false);
			m_vselfix->Show(false);
			m_selvalctrl->Show(false);
			m_vselmac->Show(true);
			m_vselmactext->Show(true);
			m_vselone->Show(false);
			m_vselonetext->Show(false);
			m_vselvalbutton->Show(false);
			m_vselvalhelpvertsizer->Layout();
			m_vselctrlsizer->Layout();
		}
		else if (mov) {
			m_selval->Show(false);
			m_vselmov->Show(true);
			m_vseldel->Show(false);
			m_vselfix->Show(false);
			m_selvalctrl->Show(list);
			m_vselmac->Show(!list);
			m_vselmactext->Show(!list);
			m_vselone->Show(false);
			m_vselonetext->Show(false);
			m_vselvalbutton->Show(false);
			m_vselvalhelpvertsizer->Layout();
			m_vselctrlsizer->Layout();
		}
		else if (del) {
			m_selval->Show(false);
			m_vselmov->Show(false);
			m_vseldel->Show(true);
			m_vselfix->Show(false);
			m_selvalctrl->Show(false);
			m_vselmac->Show(false);
			m_vselmactext->Show(true);
			m_vselone->Show(false);
			m_vselonetext->Show(false);
			m_vselvalbutton->Show(false);
			m_vselvalhelpvertsizer->Layout();
			m_vselctrlsizer->Layout();
		}
		else if (fix) {
			m_selval->Show(false);
			m_vselmov->Show(false);
			m_vseldel->Show(false);
			m_vselfix->Show(true);
			m_selvalctrl->Show(true);
			m_vselmac->Show(false);
			m_vselmactext->Show(false);
			m_vselone->Show(false);
			m_vselonetext->Show(false);
			m_vselvalbutton->Show(false);
			m_vselvalhelpvertsizer->Layout();
			m_vselctrlsizer->Layout();
		}
	}
	else if (set) {
		if (ren || cpy || rest || interfile || del) {
			m_selval->Show(false);
			m_vselmov->Show(false);
			m_vseldel->Show(false);
			m_vselfix->Show(false);
			m_selvalctrl->Show(false);
			m_vselmac->Show(false);
			m_vselmactext->Show(false);
			m_vselone->Show(false);
			m_vselonetext->Show(false);
			m_vselvalbutton->Show(false);
			m_vselvalhelpvertsizer->Layout();
			m_vselctrlsizer->Layout();
		}
		else if (mov) {
			m_selval->Show(false);
			m_vselmov->Show(true);
			m_vseldel->Show(false);
			m_vselfix->Show(false);
			m_selvalctrl->Show(true);
			m_vselmac->Show(false);
			m_vselmactext->Show(false);
			m_vselone->Show(false);
			m_vselonetext->Show(false);
			m_vselvalbutton->Show(false);
			m_vselvalhelpvertsizer->Layout();
			m_vselctrlsizer->Layout();
		}
		else if (fix) {
			m_selval->Show(false);
			m_vselmov->Show(false);
			m_vseldel->Show(false);
			m_vselfix->Show(true);
			m_selvalctrl->Show(true);
			m_vselmac->Show(false);
			m_vselmactext->Show(false);
			m_vselone->Show(false);
			m_vselonetext->Show(false);
			m_vselvalbutton->Show(false);
			m_vselvalhelpvertsizer->Layout();
			m_vselctrlsizer->Layout();
		}
	}
	else {
		m_selval->Show(!macro && list);
		m_vselmov->Show(false);
		m_vseldel->Show(false);
		m_selvalctrl->Show(!macro && list);
		m_vselmac->Show(macro);
		m_vselmactext->Show(macro);
		m_vselone->Show(!macro);
		m_vselonetext->Show(!macro);
		m_vselvalbutton->Show(!macro);
		m_vselvalhelpvertsizer->Layout();
		m_vselctrlsizer->Layout();
	}

	if (sect) {
		m_vselset->Show(false);
		m_vselsettext->Show(false);
		m_vselsetsizer->Layout();

		m_vselsethelp->Show(false);
		m_vselsethelptext->Show(false);
		m_vselsethelpsizer->Layout();
	}
	else {
		m_vselset->Show(!macro);
		m_vselsettext->Show(!macro);
		m_vselsetsizer->Layout();

		m_vselsethelp->Show(!macro);
		m_vselsethelptext->Show(!macro);
		m_vselsethelpsizer->Layout();
	}

	if (interfile) {
		m_vseldest->Show(true);
		m_vseldesttext->Show(true);
		m_vseldestsizer->Layout();
	}
	else {
		m_vseldest->Show(false);
		m_vseldesttext->Show(false);
		m_vseldestsizer->Layout();
	}

	if (mov) {
		// and prevent change of selection
		m_vselupbutton->Show(true);
		m_vseldnbutton->Show(true);
		m_vselmovsizer->Layout();
	}
	else {
		m_vselupbutton->Show(false);
		m_vseldnbutton->Show(false);
		m_vselmovsizer->Layout();
	}

	if (ren) {
		m_vselren->Show(true);
		m_vselrentext->Show(true);
		m_vselrensizer->Layout();
	}
	else {
		m_vselren->Show(false);
		m_vselrentext->Show(false);
		m_vselrensizer->Layout();
	}

	if (ren || mov || cpy || rest || interfile) {
		m_vselval->Show(false);
		m_vselname->Show(false);
		m_vselvaltext->Show(false);
		m_vselvalsizer->Layout();

		m_vselvalhelp->Show(false);
		m_vselnamehelp->Show(false);
		m_vselvalhelptext->Show(false);
		m_vselvalhelpsizer->Layout();

		m_vselcurrval->Show(false);
		m_vselcurrvaltext->Show(false);
		m_vselcurrvalsizer->Layout();
	}
	else if (fix) {
		m_vselval->Show(false);
		m_vselname->Show(true);
		m_vselvaltext->Show(true);
		m_vselvalsizer->Layout();

		m_vselvalhelp->Show(false);
		m_vselnamehelp->Show(true);
		m_vselvalhelptext->Show(true);
		m_vselvalhelpsizer->Layout();

		m_vselcurrval->Show(true);
		m_vselcurrvaltext->Show(true);
		m_vselcurrvalsizer->Layout();
	}
	else {
		m_vselval->Show(!macro);
		m_vselname->Show(false);
		m_vselvaltext->Show(!macro);
		m_vselvalsizer->Layout();

		m_vselvalhelp->Show(!macro);
		m_vselnamehelp->Show(false);
		m_vselvalhelptext->Show(!macro);
		m_vselvalhelpsizer->Layout();

		m_vselcurrval->Show(!macro);
		m_vselcurrvaltext->Show(!macro);
		m_vselcurrvalsizer->Layout();
	}

	m_vselpanelsizer->Layout();
	m_vselpanel->Thaw();
}




// Value select panel

bool CMgr::VselPanelEnabled = true;
long CMgr::SelValPos = 0;


void ConfigMgr::ShowSelValues(bool macro, bool list)
{
	m_selvalctrl->DeleteAllItems();  // leaves columns set up

	if (SettingValsAvailable.IsEmpty()) {
		m_valresult->Clear();
		m_valresult->WriteText(wxString("Select .ini file before proceeding."));
		CMgr::VselPanelEnabled = false;
		return;
	}

	InitialSelVal = true;
	InitialValue = m_vselvaltext->GetValue();
	ShowingMacro = macro;
	ShowingList = list;

	m_valresult->Clear();
	m_valresult->WriteText(wxString(".ini file selected."));
	CMgr::VselPanelEnabled = true;

	EditingVal = false;
	SingleValSet = false;
	m_vselvalbutton->Disable();

	// columns are zero-based
	long cnt = SettingValsAvailable.GetCount();

	if (macro) {
		m_vselmactext->Clear();
		m_vselmactext->WriteText(SettingValsAvailable.Item(0));
	}
	else if (!list) {
		m_vselonetext->Clear();
		if (CMgr::CurrAct != actfixs)
			m_vselonetext->WriteText(SettingValsAvailable.Item(0));
		SingleValSet = true;
		if (!CMgr::ChildCM) {
			char *fval = NULL;
			char *vval = NULL;
			if (CMgr::AvailSetting
		   && CMgr::AvailSetting->fkvals) {
				fval = CMgr::AvailSetting->fkvals;
				if (CMgr::ValueCats.getitem(fval)
				 || CMgr::FilePicker.getitem(fval))
					m_vselvalbutton->Enable();
			}
		  if (CMgr::AvailSection
		   && CMgr::AvailSection->vkvals) {
				vval = CMgr::AvailSection->vkvals;
				if (CMgr::ValueCats.getitem(vval)
				 || CMgr::FilePicker.getitem(vval))
					m_vselvalbutton->Enable();
			}
		}
	}
	else {
		m_vselonetext->Clear();
		for (long i = 0; i < cnt; i++) {
			m_selvalctrl->InsertItem(i, SettingValsAvailable.Item(i));
			m_selvalctrl->SetItem(i, 1, SettingValsHelp.Item(i));
		}

		m_selvalctrl->SetItemState(SettingValsPos,
															 wxLIST_STATE_SELECTED | wxLIST_STATE_FOCUSED,
															 wxLIST_STATE_SELECTED);
		m_selvalctrl->EnsureVisible(SettingValsPos);
		CMgr::SelValPos = SettingValsPos;
		if (SettingValsPos >= 0)
			SelValSelected = SettingValsAvailable.Item(SettingValsPos);
	}

	StartValPos = SettingValsPos;
	m_vselresult->Clear();
	(GetBookCtrl())->ChangeSelection((size_t) pnval);
	if (CMgr::CurrAct == actfixs)
		m_vselvaltext->Clear();
}


void ConfigMgr::SetUpSelValCtrl( void )
{
	m_selvalctrl->ClearAll();
  m_selvalctrl->InsertColumn(0, wxT("Value"), wxLIST_FORMAT_LEFT, 180);
  m_selvalctrl->InsertColumn(1, wxT("Purpose"), wxLIST_FORMAT_LEFT, 230);
}


// show result on Values panel only after edit or select

void ConfigMgr::ValEdited(void)
{
	if (ShowingMacro)
		MacroValue = EditedSelVal;
	else
		SelValSelected = EditedSelVal;

	if (m_vselonetext->IsShown()) {
		if (CMgr::AvailSetting
		 && CMgr::AvailSetting->plus
		 && (m_vselonetext->GetValue() != "---")) {
			if (!m_vselonetext->IsEmpty())
				m_vselonetext->WriteText(wxString(" "));
		}
		else if (CMgr::AvailSection
		 && CMgr::AvailSection->plus
		 && (m_vselonetext->GetValue() != "---")) {
			if (!m_vselonetext->IsEmpty())
				m_vselonetext->WriteText(wxString(" "));
		}
		else
			m_vselonetext->Clear();
		m_vselonetext->WriteText(SelValSelected);
	}
	// pos is always 0, first and only item
}


void ConfigMgr::ProcSelButtonSelect(void)
{
	char *res = NULL;
	char *val = NULL;
	char *sep = " ";
	char *prefix = NULL;
	char *suffix = NULL;
	bool plus = false;
	long pickfile = 0;

	// get value from child CM
	if (SelValSelected == "<format>") {
		plus = true;
		res = InvokeChildCM("CF");
		sep = NULL;
		prefix = "<";
		suffix = ">";
	}
	else if (CMgr::AvailSetting
	 && CMgr::AvailSetting->fkvals) {
		val = CMgr::AvailSetting->fkvals;
		plus = CMgr::AvailSetting->plus;
		if ((pickfile = CMgr::FilePicker.getitem(val)) != 0)
			res = GetFileInfo(pickfile);
		else
			res = InvokeChildCM(val);
		if (!stricmp((val + strlen(val) - 3), "dot"))
			prefix = ".";  // used for CSS selector
	}
	else if (CMgr::AvailSection
	 && CMgr::AvailSection->vkvals) {
		val = CMgr::AvailSection->vkvals;
		plus = CMgr::AvailSection->plus;
		if ((pickfile = CMgr::FilePicker.getitem(val)) != 0)
			res = GetFileInfo(pickfile);
		else
			res = InvokeChildCM(val);
		if (!stricmp((val + strlen(val) - 4), "head"))
			suffix = "Head";  // used for RuninHeadText
	}

	if (val
	 && strstr(val, "BldBlk"))
		sep = NULL;

	// process received value
	if (res) {
		if (m_vselonetext->IsShown()) {
			if (!plus
			 || (m_vselonetext->GetValue() == "---"))
				m_vselonetext->Clear();
			else if (sep
			 && !m_vselonetext->IsEmpty())
				m_vselonetext->WriteText(wxString(sep));

			if (prefix)
				m_vselonetext->WriteText(wxString(prefix));
			m_vselonetext->WriteText(wxString(res));
			if (suffix)
				m_vselonetext->WriteText(wxString(suffix));
		}
	}
}


void ConfigMgr::ProcSelValSelect(long pos)
{
	bool plus = false;
	char *sep = " ";
	char *val = NULL;

	if (VselSelectionLocked) {
		if (pos == SettingValsPos)
			return;
		m_selvalctrl->SetItemState(SettingValsPos,
															 wxLIST_STATE_SELECTED | wxLIST_STATE_FOCUSED,
															 wxLIST_STATE_SELECTED);
		m_selvalctrl->EnsureVisible(SettingValsPos);
		CMgr::SelValPos = SettingValsPos;
	}
	else if (ValuesShown.GetCount() > 1) { // more than one item is enum
		if ((CMgr::SelValPos = pos) != SettingValsPos)
			SettingValsPos = pos;
		SelValSelected = SettingValsAvailable.Item(pos);
		if (InitialSelVal) {
			InitialSelVal = false;
			return;
		}
		if (CMgr::AvailSetting) {
			plus = CMgr::AvailSetting->plus;
			val = CMgr::AvailSetting->fkvals;
		}
		else if (CMgr::AvailSection) {
			plus = CMgr::AvailSection->plus;
			val = CMgr::AvailSection->vkvals;
		}
		if (val
		 && strstr(val, "BldBlk"))
			sep = NULL;

		if (!plus
		 || (m_vselonetext->GetValue() == "---"))
			m_vselonetext->Clear();
		else if (sep
		 && !m_vselonetext->IsEmpty())
			m_vselonetext->WriteText(wxString(sep));

		// if selected item is "<format>", activate Select button
		// and replace with selected format in <>.
		if (SelValSelected == "<format>") {
			m_vselvalbutton->Enable();
		}
		else {
			m_vselvalbutton->Disable();
			if (CMgr::CurrAct != actfixs)
				m_vselonetext->WriteText(SelValSelected);
		}
	}
}


void ConfigMgr::MoveSelVal(bool up)
{
	if (!VselSelectionLocked)
		return;

	// swap sel val item with the one above or below
	long cnt = SettingValsAvailable.GetCount();

	if ((up && (SettingValsPos < 1))
	 || (!up && (SettingValsPos >= (cnt - 1))))
		return;

	long src = SettingValsPos;
	long dest = up ? (src - 1) : (src + 1);

	wxString name = SettingValsAvailable.Item(src);
	wxString help = SettingValsHelp.Item(src);
	SettingValsAvailable.Item(src) = SettingValsAvailable.Item(dest);
	SettingValsHelp.Item(src) = SettingValsHelp.Item(dest);
	SettingValsAvailable.Item(dest) = name;
	SettingValsHelp.Item(dest) = help;
	SettingValsPos = dest;

	m_selvalctrl->SetItem(src, 0, SettingValsAvailable.Item(src));
	m_selvalctrl->SetItem(src, 1, SettingValsHelp.Item(src));
	m_selvalctrl->SetItem(dest, 0, SettingValsAvailable.Item(dest));
	m_selvalctrl->SetItem(dest, 1, SettingValsHelp.Item(dest));

	m_selvalctrl->SetItemState(SettingValsPos,
														 wxLIST_STATE_SELECTED | wxLIST_STATE_FOCUSED,
														 wxLIST_STATE_SELECTED);
	m_selvalctrl->EnsureVisible(SettingValsPos);
	CMgr::SelValPos = SettingValsPos;
}


void ConfigMgr::ApplySelVals(void)
{
	// we have all info now, rewrite the value in the selected ini
	if (EditingVal) {
		m_selvalctrl->EndEditLabel(false);
		ValEdited();
	}
	EndValPos = SettingValsPos;

	if (!ShowingMacro)
		SelValSelected = m_vselonetext->GetValue();
	//else
	//	SelValSelected = SettingValsAvailable.Item(SettingValsPos);

	char *sect = NewName(SectionSelected.c_str());
	char *set = NewName(SettingNameShown.c_str());
	char *sval = NewName(SelValSelected.c_str());
	char *path = CMgr::SelectedIni->path;
	char *fname = CMgr::SelectedIni->name;

	char *sv = sval;
	if (!strcmp(sval, "---"))
		sv = " ";

	char *rsect = NULL;
	char *rset = NULL;

	m_vselresult->Clear();
	m_actresult->Clear();

	wxString txt;

	switch (CMgr::CurrAct) {
		case actadd:
		case actaddv:
			set = NULL;
			AddHist("added [", sect);
			MacroValue = m_vselmactext->GetValue();
			if (UpdateIniFile(sect, set, " ", path)
			 && ((MacroValue == SettingValsAvailable.Item(0))
			  || (AddHist("added [", sect),
			      ChangeSectIniFile(sect, MacroValue.c_str(), path)))) {
				ShowAction(true);
				m_actresult->Clear();
				m_actresult->WriteText(wxString("Section added in selected .ini file."));
				return;
			}
			break;

		case actadds:
		case actaddsv:
		case actaddsb:
			AddHist("added ", set);
			if (UpdateIniFile(sect, set, sv, path)) {
				ShowAction(true);
				m_actresult->Clear();
				m_actresult->WriteText(wxString("Setting added in selected .ini file."));
				return;
			}
			break;

		case actdel:
			set = NULL;
			sval = NULL;
			AddHist("deleted [", sect);
			MacroValue = m_vselmactext->GetValue();
			if (((MacroValue == SettingValsAvailable.Item(0))
			  || ChangeSectIniFile(sect, MacroValue.c_str(), path))
		   && (AddHist("deleted [", sect),
			     UpdateIniFile(sect, set, sval, path))) {
				CMgr::DupSetsFound = false;
				ShowAction(true);
				m_actresult->Clear();
				m_actresult->WriteText(wxString("Section deleted from selected .ini file."));
				return;
			}
			break;

		case actdels:
			sval = NULL;
			AddHist("deleted ", set);
			if (UpdateIniFile(sect, set, sval, path)) {
				ShowAction(true);
				m_actresult->Clear();
				m_actresult->WriteText(wxString("Setting deleted from selected .ini file."));
				return;
			}
			break;

		case actrest:
			set = NULL;
			AddHist("restored [", sect);
			MacroValue = m_vselmactext->GetValue();
			if (RestoreIniFile(sect, set, path)
			 && ((MacroValue == SettingValsAvailable.Item(0))
			  || (AddHist("restored [", sect + 1),
			      ChangeSectIniFile(sect + 1, MacroValue.c_str(), path)))) {
				ShowAction(true);
				m_actresult->Clear();
				m_actresult->WriteText(wxString("Section restored in selected .ini file."));
				return;
			}
			break;

		case actrests:
			AddHist("restored ", set);
			if (RestoreIniFile(sect, set, path)) {
				ShowAction(true);
				m_actresult->Clear();
				m_actresult->WriteText(wxString("Setting restored in selected .ini file."));
				return;
			}
			break;

		case actedit:
			AddHist("edited [", sect);
			MacroValue = m_vselmactext->GetValue();
			if (ChangeSectIniFile(sect, MacroValue.c_str(), path)) {
				CMgr::DupSetsFound = false;
				ShowAction(true);
				m_actresult->Clear();
				m_actresult->WriteText(wxString("Section edited in selected .ini file."));
				return;
			}
			break;

		case actedits:
			AddHist("edited ", set, InitialValue.c_str());
			if (UpdateIniFile(sect, set, sv, path)) {
				ShowAction(true);
				m_actresult->Clear();
				m_actresult->WriteText(wxString("Setting edited in selected .ini file."));
				return;
			}
			break;

		case actrenv:
			rsect = NewName(CMgr::VNSectRename.c_str());  // VN
			MacroValue = m_vselmactext->GetValue();
			AddHist("renamed [", rsect, sect);
			if (ChangeSectIniFile(sect, MacroValue.c_str(), path)
			 && RenSectIniFile(sect, rsect, path)) {
				if (CMgr::AutoAlias) {
					AddHist("renamed [", rsect, sect);
					if (UpdateIniFile("FormatAliases", sect, rsect, path)) {
						CMgr::AutoAlias = false;
						//CMgr::AutoAliasSet.Empty();
						//CMgr::AutoAliasVal.Empty();
					}
				}
				ShowAction(true);
				m_actresult->Clear();
				m_actresult->WriteText(wxString("Section renamed in selected .ini file."));
				return;
			}
			break;

		case actrensv:
			rset = NewName(CMgr::VKSetRename.c_str()); // VK
			AddHist("renamed ", rset, set);
			if (RenSetIniFile(sect, set, rset, path)) {
				ShowAction(true);
				m_actresult->Clear();
				m_actresult->WriteText(wxString("Setting renamed in selected .ini file."));
				return;
			}
			break;

		case actmov:
			if (StartValPos == EndValPos) {
				m_vselresult->WriteText(wxString("Item not moved."));
				return;
			}
			AddHist("moved [", sect, NULL, (StartValPos > EndValPos) ? " up" : " down");
			if (MovSectIniFile(sect, StartValPos, EndValPos, path)) {
				ShowAction(true);
				m_actresult->Clear();
				m_actresult->WriteText(wxString("Section moved in selected .ini file."));
				return;
			}
			break;

		case actmovs:
			if (StartValPos == EndValPos) {
				m_vselresult->WriteText(wxString("Item not moved."));
				return;
			}
			AddHist("moved ", set, NULL, (StartValPos > EndValPos) ? " up" : " down");
			if (MovSetIniFile(sect, set, StartValPos, EndValPos, path)) {
				ShowAction(true);
				m_actresult->Clear();
				m_actresult->WriteText(wxString("Setting moved in selected section."));
				return;
			}
			break;

		case actmovf:
			set = NULL;
			MacroValue = m_vselmactext->GetValue();
			AddHist("moved [", sect, fname);
			if (UpdateIniFile(sect, set, sv, DestIniSel->path)
			 && ChangeSectIniFile(sect, MacroValue.c_str(), DestIniSel->path)
			 && (AddHist("moved [", sect, DestIniSel->name, NULL, "to"),
			     UpdateIniFile(sect, set, NULL, path))) {
				ShowAction(true);
				m_actresult->Clear();
				m_actresult->WriteText(wxString("Section moved to selected destination .ini file."));
				return;
			}
			break;

		case actmovsf:
			AddHist("moved ", set, fname);
			if (UpdateIniFile(sect, set, sv, DestIniSel->path)
			 && (AddHist("moved ", set, DestIniSel->name, NULL, "to"),
			     UpdateIniFile(sect, set, NULL, path))) {
				ShowAction(true);
				m_actresult->Clear();
				m_actresult->WriteText(wxString("Setting moved to selected destination .ini file."));
				return;
			}
			break;

		case actcpy:
			set = NULL;
			AddHist("copied [", sect, fname);
			MacroValue = m_vselmactext->GetValue();
			if (UpdateIniFile(sect, set, " ", DestIniSel->path)
			 && (AddHist("edited [", sect, fname),
			     ChangeSectIniFile(sect, MacroValue.c_str(), DestIniSel->path))) {
				ShowAction(true);
				m_actresult->Clear();
				m_actresult->WriteText(wxString("Section copied to selected destination .ini file."));
				return;
			}
			break;

		case actcpys:
			AddHist("copied ", set, fname);
			if (UpdateIniFile(sect, set, sv, DestIniSel->path)) {
				ShowAction(true);
				m_actresult->Clear();
				m_actresult->WriteText(wxString("Setting copied to selected destination .ini file."));
				return;
			}
			break;

		case actdup:
			set = NULL;
			sval = NULL;
			AddHist("merged duplicates of [", sect);
			MacroValue = m_vselmactext->GetValue();
			if (ChangeSectIniFile(sect, MacroValue.c_str(), path)
		   && (AddHist("deleted duplicate [", sect),
			     DeleteIniFileAll(sect, set, path))) {
				CMgr::DupSectsFound = false;
				ShowAction(true);
				m_actresult->Clear();
				m_actresult->WriteText(wxString("Duplicate sections merged in selected .ini file."));
				return;
			}
			break;

		case actdups:
			if (!sv
			 || !*sv) {
				m_vselresult->WriteText(wxString("Select value to use for this setting."));
				return;
			}
			AddHist("edited merged ", set, InitialValue.c_str());
			if (UpdateIniFile(sect, set, sv, path)
		   && (AddHist("deleted duplicate ", set),
			     DeleteIniFileAll(sect, set, path))) {
				CMgr::DupSetsFound = false;
				ShowAction(true);
				m_actresult->Clear();
				m_actresult->WriteText(wxString("Duplicate settings merged in selected section."));
				return;
			}
			break;

		//case actfix:
		//	break;

		case actfixs:
			if (!sv || !*sv) {
				sval = NULL;
				CMgr::CurrAct = actdels;
				AddHist("deleted ", set);
				if (UpdateIniFile(sect, set, sval, path)) {
					ShowAction(true);
					m_actresult->Clear();
					m_actresult->WriteText(wxString("Setting deleted from selected .ini file."));
					return;
				}
			}
			else {
				CMgr::CurrAct = actrensv;
				AddHist("corrected ", sv, set);
				if (RenSetIniFile(sect, set, sv, path)) {
					ShowAction(true);
					m_actresult->Clear();
					m_actresult->WriteText(wxString("Setting corrected in selected .ini file."));
					return;
				}
			}
			break;

		case actnone:
			m_vselresult->Clear();
			m_vselresult->WriteText(wxString("Action not selected yet."));
			break;

		default:
			m_vselresult->Clear();
			m_vselresult->WriteText(wxString("Action not implemented yet."));
			return;
	}

	// only get here for error in ini-file update
	if (!DCpfile::MovErrFptr)
		m_vselresult->WriteText(wxString("Could not update .ini file."));
	else {
		m_vselresult->WriteText(wxString("File not updated, changes in .new."));
		CMgr::LogEvent(logwarn, 1, "Error in MoveFileEx(): ",
					                      ltoa(DCpfile::MovErrVal, NewName(16), 10));
	}
	DCpfile::addhist(NULL);
}


void ConfigMgr::UpdateValueSelected(wxString &val)
{
	char *str = NewName(val.c_str());
	m_vselvaltext->Clear();
	m_vselvaltext->WriteText(val);
	CMgr::SelectedSet->cval = str;
	if (IsEffectiveVal(CMgr::SelIniPos)) {
		SettingValShown = val;
		UpdateValueSelections();
	}
	UpdateSettingSelections();
	m_selinictrl->SetItem(CMgr::SelIniPos, 2, val);
	ValuesShown.Item(ValuesShown.GetCount() - CMgr::SelIniPos) = val;
}


bool ConfigMgr::IsEffectiveVal(long inipos)
{
	long cnt = ValuesShown.GetCount();
	const char *str = NULL;

	for (long i = inipos + 1; i <= cnt; i++) {
		str = ValuesShown.Item(cnt - i).c_str();
		if (strcmp(str, "---"))
			return false;  // another val is below it
	}

	return true;
}


void ConfigMgr::AddHist(const char *act, const char *obj,
	     const char *from, const char *mv, const char *fromto)
{
	if (!CMgr::AddIniHistory)
		return;

	wxString str = wxString(";");

	wxDateTime dt = wxDateTime::Now();
	str += dt.FormatISODate();
	str += " ";
	str += dt.FormatISOTime();
	str += ": ";

	if (CMgr::UserName) {
		str += CMgr::UserName;
		str += " ";
	}

	str += act;
	bool sect = (str.Last() == '[');

	str += obj;
	if (sect)
		str += ']';

	if (from
	 && fromto) {
		str += " ";
		str += fromto;
		str += " \"";
		str += from;
		str += "\"";
	}
	else if (from) {
		str += " from \"";
		str += from;
		str += "\"";
	}
	else if (mv)
		str += mv;

	DCpfile::addhist(str.c_str());
}



// macro panel

char CMgr::MacroBuffer[4096];


bool ConfigMgr::GetMacroValue(void)
{
	// get macro from current ini
	char *sect = NewName(SectionSelected.c_str());
	char *path = CMgr::SelectedIni->path;
	if (CMgr::CurrAct == actdup) { // get all copies of section in file
		if (ConfigMgr::GetIniFileAll(sect, NULL, CMgr::MacroBuffer, 4096, path))
			return true;
		return false;
	}
	else if (ConfigMgr::GetIniFile(sect, "", "", CMgr::MacroBuffer, 4096, path))
		return true;

	if (CMgr::CurrAct == actrest)  // accept no substitutes
		return false;

	// if missing, read from one above with it
	DCvlist *dvl = &CMgr::CurrIniFiles;
	DCvlist *dvs = NULL;
	CIni *ini = NULL;

	while (dvl->next) {  // get up to current one
		dvl = dvl->next;
		if (!strcmp(CMgr::SelectedIni->name, ((CIni *) (dvl->vp))->name))
			break;
	}
	while (dvl->next) {  // if any past it, check for section
		dvl = dvl->next;
		if (((CIni *) (dvl->vp))->sectcount) {
			dvs = &(((CIni *) (dvl->vp))->sections);
			while (dvs->next) {  // look for macro section
				dvs = dvs->next;
				if (!strcmp(SectionSelected.c_str(), ((CSection *) (dvs->vp))->name)) {
					ini = (CIni *) (dvl->vp);
					path = ini->path;
					if (ConfigMgr::GetIniFile(sect, "", "", CMgr::MacroBuffer, 4096, path))
						return true;
				}
			}
		}
	}

	if ((CMgr::CurrAct == actadd)
	 || (CMgr::CurrAct == actaddv)) {
		CMgr::MacroBuffer[0] = '\0';
		strcat (CMgr::MacroBuffer, "\n[");
		strcat (CMgr::MacroBuffer, sect);
		strcat (CMgr::MacroBuffer, "]\n");
		return true;
	}

	return false;
}



// preferences panel


void ConfigMgr::SysIniSelect(bool yn)
{
	CMgr::AllowSysIniSelect = yn;
}


bool CMgr::AddIniHistory = false;
const char *CMgr::UserName = NULL;

char *CMgr::SectColorText = "rgb(96,96,96)";
char *CMgr::SetColorText = "rgb(96,96,96)";
char *CMgr::VarColorText = "rgb(96,96,96)";
char *CMgr::DefColorText = "rgb(96,96,96)";
char *CMgr::SysColorText = "rgb(96,96,96)";
char *CMgr::InvalidColorText = "rgb(255,0,0)";


void ConfigMgr::InitColors(void)
{
	InvalidColor = new wxColour(wxString(CMgr::InvalidColorText));

	SectColor = new wxColour(wxString(CMgr::SectColorText));
	if (SectColor) {
		m_optcolorsectctrl->SetColour(*SectColor);
		m_optcolorsecttext->SetForegroundColour(*SectColor);
	}

	SetColor = new wxColour(wxString(CMgr::SetColorText));
	if (SetColor) {
		m_optcolorsetctrl->SetColour(*SetColor);
		m_optcolorsettext->SetForegroundColour(*SetColor);
	}

	VarColor = new wxColour(wxString(CMgr::VarColorText));
	if (VarColor) {
		m_optcolorvarctrl->SetColour(*VarColor);
		m_optcolorvartext->SetForegroundColour(*VarColor);
	}

	DefColor = new wxColour(wxString(CMgr::DefColorText));
	if (DefColor) {
		m_optcolordefctrl->SetColour(*DefColor);
		m_optcolordeftext->SetForegroundColour(*DefColor);
	}

	SysColor = new wxColour(wxString(CMgr::SysColorText));
	if (SysColor) {
		m_optcolorsysctrl->SetColour(*SysColor);
		m_optcolorsystext->SetForegroundColour(*SysColor);
	}

	m_optbrowserctrl->Clear();
	if (CMgr::Browser)
		m_optbrowserctrl->SetValue(wxString(CMgr::Browser));
	else
		m_optbrowserctrl->SetValue(wxString("Select or enter path for browser"));

	m_optbrowserctrl->Append(wxString(CMgr::Chrome));
	m_optbrowserctrl->Append(wxString(CMgr::Firefox));
	m_optbrowserctrl->Append(wxString(CMgr::IExplore));
	m_optbrowserctrl->Append(wxString(CMgr::Opera));
}


// Help buttons

char *CMgr::Browser = NULL;
char *CMgr::Firefox = "\"C:\\Program Files (x86)\\Mozilla Firefox\\firefox.exe\"";
char *CMgr::IExplore = "\"C:\\Program Files\\Internet Explorer\\iexplore.exe\"";
char *CMgr::Chrome = "\"C:\\Program Files (x86)\\Google\\Chrome\\Application\\chrome.exe\" --allow-file-access-from-files";
char *CMgr::Opera = "\"C:\\Program Files\\Opera\\opera.exe\"";

char *CMgr::DefaultAlias = "IDH_Dummy";

bool CMgr::UseHTMLHelp = true;
DCnlist CMgr::HelpAlias;  // Alias = (long) filename
char *CMgr::CHMPath = NULL;


wxString ConfigMgr::GetAlias(const wxString& sect, const wxString& set)
{
	wxString alias = "IDH_";
	char cat[2];

	if (sect.IsEmpty())
		return wxString("");

	cat[0] = CMgr::CategoryCode;
	cat[1] = 0;

	if (!set.IsEmpty())
		alias = alias << cat << sect << "_" << set;
	else
		alias = alias << cat << sect;

	return alias;
}


void ConfigMgr::CallHelp(const wxString& alias)
{
	if (alias.IsEmpty())
		return;

	if (CMgr::UseHTMLHelp) {
		long idx = 0;
		if (!CMgr::CHMPath)
			CMgr::CHMPath = ::ConvertEnvironment(CMgr::CHMPathSet);
		if ((idx = CMgr::HelpAlias.getitem(alias.c_str())) != 0)
			HtmlHelp(NULL, CMgr::CHMPath, HH_DISPLAY_TOPIC, idx);
		else
			CMgr::LogEvent(logwarn, 1, "Alias \"", alias.c_str(), "\" not in [HelpAlias]");
	}
	else {  // OmniHelp
		if (!CMgr::HelpCommand)
			CMgr::HelpCommand = CMgr::AltHelpCommand;

		if (!CMgr::Browser)
			CMgr::Browser = CMgr::Chrome;

		wxString cmd = wxString(CMgr::Browser) << " " << wxString(CMgr::HelpCommand) << alias;
		::RunDOSCommand(cmd.c_str());
	}
}



// pfile operations


bool ConfigMgr::UpdateIniFile(const char *sect, const char *set,
	                            const char *sval, const char *path)
{
	if (!DCpfile::MovErrFptr) {
		if (!DCpfile::putval(sect, set, sval, path)) {
			CMgr::LogEvent(logerr, 1, "Failed to update [", sect, "] in file ", path);
			CMgr::LogEvent(logcont, 1, "for setting ", set ? set : "NULL", " = ",
																 sval ? sval : "NULL");
			DCpfile::pclose(path);
			return false;
		}
	}

	if (!DCpfile::pclose(path)) {
		ReportMoveErr();
		return false;
	}
	return true;
}


bool ConfigMgr::GetIniFile(const char *sect, const char *set, 
	       const char *dval, char *val, unsigned long len, const char *path)
{
	if (!DCpfile::MovErrFptr) {
		if (!DCpfile::getval(sect, set, dval, val, len, path)) {
			CMgr::LogEvent(logerr, 1, "Failed to read [", sect, "] in file ", path);
			if (set
			 && *set)
				CMgr::LogEvent(logcont, 1, "for setting ", set);
			DCpfile::pclose(path);
			return false;
		}
	}

	if (!DCpfile::pclose(path)) {
		ReportMoveErr();
		return false;
	}
	return true;
}


bool ConfigMgr::GetIniFileAll(const char *sect, const char *set, 
	       char *val, unsigned long len, const char *path)
{
	if (!DCpfile::MovErrFptr) {
		if (!DCpfile::getvalall(sect, set, val, len, path)) {
			if (set
			 && *set) {
				CMgr::LogEvent(logerr, 1, "Failed to read [", sect, "] for all settings ", set);
				CMgr::LogEvent(logcont, 1, " in file ", path);
			}
			else
				CMgr::LogEvent(logerr, 1, "Failed to read all [", sect, "]s in file ", path);
			DCpfile::pclose(path);
			return false;
		}
	}

	if (!DCpfile::pclose(path)) {
		ReportMoveErr();
		return false;
	}
	return true;
}


bool ConfigMgr::DeleteIniFileAll(const char *sect, const char *set, const char *path)
{
	if (!DCpfile::MovErrFptr) {
		if (!DCpfile::deleteall(sect, set, path)) {
			if (set
			 && *set) {
				CMgr::LogEvent(logerr, 1, "Failed to delete in [", sect, "] duplicate settings ", set);
				CMgr::LogEvent(logcont, 1, " in file ", path);
			}
			else
				CMgr::LogEvent(logerr, 1, "Failed to delete all duplicates of [", sect, "]s in file ", path);
			DCpfile::pclose(path);
			return false;
		}
	}

	if (!DCpfile::pclose(path)) {
		ReportMoveErr();
		return false;
	}
	return true;
}


bool ConfigMgr::ChangeSectIniFile(const char *sect, const char *sval,
	                                const char *path)
{
	if (!DCpfile::MovErrFptr) {
		if (!DCpfile::chsect(sect, sval, path)) {
			CMgr::LogEvent(logerr, 1, "Failed to update [", sect, "] in file ", path);
			DCpfile::pclose(path);
			return false;
		}
	}

	if (!DCpfile::pclose(path)) {
		ReportMoveErr();
		return false;
	}
	return true;
}


bool ConfigMgr::MovSectIniFile(const char *sect, long opos, long npos,
	                                const char *path)
{
	if (!DCpfile::MovErrFptr) {
		if (!DCpfile::movsect(sect, opos, npos, path)) {
			CMgr::LogEvent(logerr, 1, "Failed to move [", sect, "] in file ", path);
			DCpfile::pclose(path);
			return false;
		}
	}

	if (!DCpfile::pclose(path)) {
		ReportMoveErr();
		return false;
	}
	return true;
}


bool ConfigMgr::MovSetIniFile(const char *sect, const char *set,
                          long opos, long npos, const char *path)
{
	if (!DCpfile::MovErrFptr) {
		if (!DCpfile::movset(sect, set, opos, npos, path)) {
			CMgr::LogEvent(logerr, 1, "Failed to move ", set, "in [", sect, "]");
			CMgr::LogEvent(logcont, 1, "in file ", path);
			DCpfile::pclose(path);
			return false;
		}
	}

	if (!DCpfile::pclose(path)) {
		ReportMoveErr();
		return false;
	}
	return true;
}


bool ConfigMgr::RestoreIniFile(const char *sect, const char *set, const char *path)
{
	if (!DCpfile::MovErrFptr) {
		if (!DCpfile::rest(sect, set, path)) {
			if (set) {
				CMgr::LogEvent(logerr, 1, "Failed to restore setting ", set, " in section [", sect, "]" );
				CMgr::LogEvent(logcont, 1, "in file ", path);
			}
			else {
				CMgr::LogEvent(logerr, 1, "Failed to restore section [", sect, "]" );
				CMgr::LogEvent(logcont, 1, "in file ", path);
			}
			DCpfile::pclose(path);
			return false;
		}
	}

	if (!DCpfile::pclose(path)) {
		ReportMoveErr();
		return false;
	}
	return true;
}


bool ConfigMgr::RenSetIniFile(const char *sect, const char *set, 
	                            const char *nset, const char *path)
{
	if (!DCpfile::MovErrFptr) {
		if (!DCpfile::renset(sect, set, nset, path)) {
			CMgr::LogEvent(logerr, 1, "Failed to rename setting ", set, " in section [", sect, "]" );
			CMgr::LogEvent(logcont, 1, "in file ", path, " to ", nset);
			DCpfile::pclose(path);
			return false;
		}
	}

	if (!DCpfile::pclose(path)) {
		ReportMoveErr();
		return false;
	}
	return true;
}


bool ConfigMgr::RenSectIniFile(const char *sect, const char *nsect,
	                             const char *path)
{
	if (!DCpfile::MovErrFptr) {
		if (!DCpfile::rensect(sect, nsect, path)) {
			CMgr::LogEvent(logerr, 1, "Failed to rename [", sect, "] in file ", path);
			CMgr::LogEvent(logcont, 1, "to [", nsect, "]");
			DCpfile::pclose(path);
			return false;
		}
	}

	if (!DCpfile::pclose(path)) {
		ReportMoveErr();
		return false;
	}
	return true;
}


void ConfigMgr::ReportMoveErr(void)
{
	CMgr::LogEvent(logerr, 1, "Failed to update file ", DCpfile::MovErrFptr->getBaseName(),
		 ", file .new left with changes.");
}



// ini-file reading and processing


char *CMgr::StartFileName = NULL;
char *CMgr::MacroFileName = NULL;
char *CMgr::LangFileName = NULL;
char *CMgr::FmtFileName = NULL;
char *CMgr::PagesFileName = NULL;
char *CMgr::TablesFileName = NULL;
char *CMgr::SubFormatsName = NULL;
    
bool CMgr::UseConfigs = true;
bool CMgr::UseFormats = false;
bool CMgr::UseLangs = false;
bool CMgr::UseMacros = false;
bool CMgr::UseModels = false;
uns CMgr::Usage = 1;

DCirfile *CMgr::IniFile = NULL;
DCirfile *CMgr::MacroFile = NULL;
DCirfile *CMgr::LangFile = NULL;
DCfrfile *CMgr::FmtFile = NULL;
DCfrfile *CMgr::PagesFile = NULL;
DCfrfile *CMgr::TablesFile = NULL;
DCfrfile *CMgr::SubFormatsFile = NULL;

DCvlist CMgr::CurrIniFiles;
long CMgr::CurrIniCount = 0;
DCvlist CMgr::CurrSections;
long CMgr::CurrSectionCount = 0;
DCnlist CMgr::CurrSectionNames;
DCvlist CMgr::CurrSectionNameRefs;
long CMgr::CurrSectionNameCount = 0;
DCvlist CMgr::CurrSettings;
long CMgr::CurrSettingCount = 0;
DCnlist CMgr::CurrSettingNames;
DCvlist CMgr::CurrSettingNameRefs;
long CMgr::CurrSettingNameCount = 0;


void CMgr::GetIniFiles( void )
{
  // based on selected start, load config tree but unlinked
  // if category calls for format, lang, or macro, load that tree too

	static char *lastStart = NULL;

	if (lastStart) {
		CurrIniFiles.empty();
		CurrIniCount = 0;
		CurrSections.empty();
		CurrSectionCount = 0;
		CurrSectionNames.empty();
		CurrSectionNameRefs.empty();
		CurrSectionNameCount = 0;
		CurrSettings.empty();
		CurrSettingCount = 0;
		CurrSettingNames.empty();
		CurrSettingNameRefs.empty();
		CurrSettingNameCount = 0;
	}
	lastStart = StartFileName;

	// set IniFile to point to top of config chain
	if (StartFileName
	 && *StartFileName) {
		// snm is specific .ini basename, can be NULL
		IniFile = new DCirfile(StartFileName);
		if (IniFile->FErr() != fok) {
			IniFile->CloseFile();
			IniFile = NULL;
      LogEvent(logerr, 1, "Cannot open config file ", StartFileName);
      return;
		}
		else {
      IniFile = IniFile->SetConfigTemplate("Templates", "Configs");
			if (UseConfigs)
				SetCInisForChain(&(IniFile->IniFileList), inicfg);
		}
	}

	if (LogIniChains)
		LogEvent(loginfo, 1, "Processed config files");
        

	if (UseMacros) {
    if (IniFile->Section("Templates")
     && (IniFile->Find("Macros")
      || IniFile->Find("MacroFile"))) {
      MacroFileName = IniFile->PathRight();
      MacroFile = new DCirfile(MacroFileName);
      if (MacroFile->FErr() != fok) {
        MacroFile->CloseFile();
        LogEvent(logwarn, 1, "Macro file not found: ", MacroFileName);
        MacroFile = NULL;
      }
      else {
        MacroFile = MacroFile->SetConfigTemplate("Templates", "Macros", false);
				SetCInisForChain(&(MacroFile->IniFileList), inimac);
			}
    }

    if (LogIniChains)
      LogEvent(loginfo, 1, "Processed macro files");
  }
        
	// set up language files
	if (UseLangs) {
    if (IniFile->Section("Templates")
     && (IniFile->Find("Languages")
      || IniFile->Find("LanguageText"))) {
      LangFileName = IniFile->PathRight();
      LangFile = new DCirfile(LangFileName);
      if (LangFile->FErr() != fok) {
        LangFile->CloseFile();
        LogEvent(logwarn, 1, "Language file not found: ", LangFileName);
        LangFile = NULL;
      }
      else {
        LangFile = LangFile->SetConfigTemplate("Templates", "Languages", false);
				SetCInisForChain(&(LangFile->IniFileList), inilang);
			}
    }

    if (LogIniChains)
      LogEvent(loginfo, 1, "Processed language file");
  }

	// set up formats files

	if (UseFormats) {
    if (IniFile->Section("Templates")
     && IniFile->Find("Formats")) {
      FmtFileName = IniFile->PathRight();
      FmtFile = new DCfrfile(FmtFileName);
      if (FmtFile->FErr() != fok) {
        FmtFile->CloseFile();
        LogEvent(logwarn, 1, "Formats file not found: ", FmtFileName);
        FmtFile = NULL;
      }
      else {
        FmtFile = FmtFile->SetConfigTemplate("Templates", "Formats");
        DCfrfile::Formats = FmtFile;
				if (Usage & 16)
					SetCInisForChain(&(FmtFile->IniFileList), inifmt);
      }
    }

    if ((IniFile->Section("Templates")
      && IniFile->Find("Pages")
      && ((PagesFileName = IniFile->PathRight()) != NULL))
     || (FmtFile
      && FmtFile->Section("Templates")
        && FmtFile->Find("Pages")
      && ((PagesFileName = FmtFile->PathRight()) != NULL))) {
      PagesFile = new DCfrfile(PagesFileName);
      if (PagesFile->FErr() != fok) {
        PagesFile->CloseFile();
        LogEvent(logwarn, 1, "Pages file not found: ", PagesFileName);
        PagesFile = NULL;
      }
      else {
        PagesFile = PagesFile->SetConfigTemplate("Templates", "Pages", false);
        DCfrfile::Pages = PagesFile;
				if (Usage & 128)
					SetCInisForChain(&(PagesFile->IniFileList), inipage);
      }
    }

    if ((IniFile->Section("Templates")
      && IniFile->Find("Tables")
      && ((TablesFileName = IniFile->PathRight()) != NULL))
     || (FmtFile
      && FmtFile->Section("Templates")
        && FmtFile->Find("Tables")
      && ((TablesFileName = FmtFile->PathRight()) != NULL))) {
      TablesFile = new DCfrfile(TablesFileName);
      if (TablesFile->FErr() != fok) {
        TablesFile->CloseFile();
        LogEvent(logwarn, 1, "Tables file not found: ", TablesFileName);
        TablesFile = NULL;
      }
      else {
        TablesFile = TablesFile->SetConfigTemplate("Templates", "Tables", false);
        DCfrfile::Tables = TablesFile;
				if (Usage & 32)
					SetCInisForChain(&(TablesFile->IniFileList), initbl);
      }
    }

    if ((IniFile->Section("Templates")
      && IniFile->Find("SubFormats")
      && ((SubFormatsName = IniFile->PathRight()) != NULL))
     || (FmtFile
      && FmtFile->Section("Templates")
      && FmtFile->Find("SubFormats")
      && ((SubFormatsName = FmtFile->PathRight()) != NULL))) {
      SubFormatsFile = new DCfrfile(SubFormatsName);
      if (SubFormatsFile->FErr() != fok) {
        LogEvent(logwarn, 1, "SubFormats file not found: ", SubFormatsName);
        SubFormatsFile = NULL;
      }
      else {
				SubFormatsFile = SubFormatsFile->SetConfigTemplate("Templates", "SubFormats", false);
				DCfrfile::SubFormats = SubFormatsFile;
				if (Usage & 64)
					SetCInisForChain(&(SubFormatsFile->IniFileList), inisub);
      }
    }

    if (LogIniChains)
      LogEvent(loginfo, 1, "Processed format files");
	}

	// **** add processing for content-model files listed in configs

    
    // reset config and format chains not to link
  if (IniFile) {
		DCirfile *ift = NULL;
		DCvlist *il = &(IniFile->IniFileList);
		while ((il = il->next) != NULL) {
			ift = (DCirfile *) il->vp;
			if (ift)
        ift->SetSecFile(NULL);
		}
  }

  if (UseFormats
   && FmtFile) {
		DCfrfile *ift = NULL;
		DCvlist *il = &(FmtFile->IniFileList);
		while ((il = il->next) != NULL) {
			ift = (DCfrfile *) il->vp;
			if (ift)
        ift->SetSecFile(NULL);
		}
	}

	CurrIniFiles.all(GetIniSectionInfo);
}


bool CMgr::GetAllInis = true;

long CMgr::ConfigLevel = 0;
long CMgr::MacroLevel = 0;

long CMgr::LangLevel = 0;
DCnlist CMgr::LangNameList;
DCnlist CMgr::LangChainList;
DCvlist CMgr::LangFileList;

long CMgr::FormatLevel = 0;
DCnlist CMgr::FormatNameList;
DCnlist CMgr::FormatChainList;
DCvlist CMgr::FormatFileList;

long CMgr::PagesLevel = 0;
DCnlist CMgr::PagesNameList;
DCnlist CMgr::PagesChainList;
DCvlist CMgr::PagesFileList;

long CMgr::TablesLevel = 0;
DCnlist CMgr::TablesNameList;
DCnlist CMgr::TablesChainList;
DCvlist CMgr::TablesFileList;

long CMgr::SubFormatsLevel = 0;
DCnlist CMgr::SubFormatsNameList;
DCnlist CMgr::SubFormatsChainList;
DCvlist CMgr::SubFormatsFileList;


void CMgr::GetIniFileChains( void )
{
	static char *lastStart = NULL;

	if (lastStart) {
		CurrIniFiles.empty();
		CurrIniCount = 0;
		CurrSections.empty();
		CurrSectionCount = 0;
		CurrSectionNames.empty();
		CurrSectionNameRefs.empty();
		CurrSectionNameCount = 0;
		CurrSettings.empty();
		CurrSettingCount = 0;
		CurrSettingNames.empty();
		CurrSettingNameRefs.empty();
		CurrSettingNameCount = 0;
	}
	lastStart = StartFileName;

	ConfigLevel = 0;
	MacroLevel = 0;

	LangLevel = 0;
	LangNameList.empty();
	LangChainList.empty();
	LangFileList.empty();

	FormatLevel = 0;
	FormatNameList.empty();
	FormatChainList.empty();
	FormatFileList.empty();

	PagesLevel = 0;
	PagesNameList.empty();
	PagesChainList.empty();
	PagesFileList.empty();

	TablesLevel = 0;
	TablesNameList.empty();
	TablesChainList.empty();
	TablesFileList.empty();

	SubFormatsLevel = 0;
	SubFormatsNameList.empty();
	SubFormatsChainList.empty();
	SubFormatsFileList.empty();


	long i = 0;
	long j = 0;


	// set IniFile to point to top of config chain
	if (StartFileName
	 && *StartFileName) {
		// snm is specific .ini basename, can be NULL
		IniFile = new DCirfile(StartFileName);
		if (IniFile->FErr() != fok) {
			IniFile->CloseFile();
			IniFile = NULL;
      LogEvent(logerr, 1, "Cannot open config file ", StartFileName);
      return;
		}
		else {
      IniFile = IniFile->SetConfigTemplate("Templates", "Configs", false);
			if (UseConfigs)
				SetCInisForChain(&(IniFile->IniFileList), inicfg);
			MacroLevel = ConfigLevel = IniFile->IniLevel;
		}
	}
	if (LogIniChains) {
		DCirfile::WriteIniChain("Configs");
		LogEvent(loginfo, 1, "Processed config files");
	}


	if (UseMacros) {  // step through cfg files, get macro file refs for each
		for (i = 1; i <= ConfigLevel; i++) {
			DCirfile *inif = (DCirfile *) IniFile->IniFileList.find(i);
			if (inif->Section("Templates")
			 && (inif->Find("Macros")
				|| inif->Find("MacroFile"))) {// macro branch here
				MacroFileName = inif->PathRight();
				MacroFile = new DCirfile(MacroFileName);
				if (MacroFile->FErr() != fok) {
					MacroFile->CloseFile();
					LogEvent(logwarn, 1, "Macro file not found: ", MacroFileName);
					MacroFile = NULL;
				}
				else {
					MacroFile = MacroFile->SetConfigTemplate("Templates", "Macros", false);
					for (j = 1; j <= MacroFile->IniLevel; j++) {
						char *nm = MacroFile->IniNameList.find(j);
						if (IniFile->IniNameList.getitem(nm) != 0) {
							ClearDupIniRefs(&(MacroFile->IniFileList), j);
							break;  // rest of this chain in list
						}
						IniFile->IniNameList.add(nm, ++MacroLevel);
						IniFile->IniChainList.add(MacroFile->IniChainList.find(j), MacroLevel);
						IniFile->IniFileList.add(MacroFile->IniFileList.find(j), MacroLevel);
					}
				}
			}
		}
		IniFile->IniLevel = MacroLevel;
		SetCInisForChain(&(IniFile->IniFileList), inimac);
		if (LogIniChains) {
			DCirfile::WriteIniChain("Macros");
      LogEvent(loginfo, 1, "Processed macro files");
		}
  }

	// set up language files
	if (UseLangs) {
		for (i = 1; i <= ConfigLevel; i++) {
			DCirfile *inif = (DCirfile *) IniFile->IniFileList.find(i);
			if (inif->Section("Templates")
			 && (inif->Find("Languages")
				|| inif->Find("LanguageText"))) {
				LangFileName = inif->PathRight();
				LangFile = new DCirfile(LangFileName);
				if (LangFile->FErr() != fok) {
					LangFile->CloseFile();
					LogEvent(logwarn, 1, "Language file not found: ", LangFileName);
					LangFile = NULL;
				}
				else {
					LangFile = LangFile->SetConfigTemplate("Templates", "Languages", false);
					for (j = 1; j <= LangFile->IniLevel; j++) {
						char *nm = LangFile->IniNameList.find(j);
						if (LangNameList.getitem(nm) != 0) {
							ClearDupIniRefs(&(LangFile->IniFileList), j);
							break;  // rest of this chain in list
						}
						LangNameList.add(nm, ++LangLevel);
						LangChainList.add(LangFile->IniChainList.find(j), LangLevel);
						LangFileList.add(LangFile->IniFileList.find(j), LangLevel);
					}
				}
			}
		}
		SetCInisForChain(&LangFileList, inilang);

		if (LogIniChains) {
 			DCirfile::WriteIniChain("Languages");
			LogEvent(loginfo, 1, "Processed language files");
		}
  }

	// process formats for each ini, and other fmt inis
	// for each ini and each format and each of themselves
	if (UseFormats) {
		DCfrfile::Formats = NULL;
		DCfrfile::Pages = NULL;
		DCfrfile::Tables = NULL;
		DCfrfile::SubFormats = NULL;

		for (i = 1; i <= ConfigLevel; i++) {
			DCirfile *inif = (DCirfile *) IniFile->IniFileList.find(i);
			FmtFile = NULL;

			if (inif->Section("Templates")
			 && inif->Find("Formats")) {
				FmtFileName = inif->PathRight();
				FmtFile = new DCfrfile(FmtFileName);
				if (FmtFile->FErr() != fok) {
					FmtFile->CloseFile();
					LogEvent(logwarn, 1, "Formats file not found: ", FmtFileName);
					FmtFile = NULL;
				}
				else {
					FmtFile = FmtFile->SetConfigTemplate("Templates", "Formats", false);
					if (!DCfrfile::Formats)
						DCfrfile::Formats = FmtFile;
					for (j = 1; j <= FmtFile->IniLevel; j++) {
						char *nm = FmtFile->IniNameList.find(j);
						if (FormatNameList.getitem(nm) != 0) {
							ClearDupFmtIniRefs(&(FmtFile->IniFileList), j);
							break;  // rest of this chain in list
						}
						FormatNameList.add(nm, ++FormatLevel);
						FormatChainList.add(FmtFile->IniChainList.find(j), FormatLevel);
						FormatFileList.add(FmtFile->IniFileList.find(j), FormatLevel);
					}
				}
			}

			if (inif->Section("Templates")
			 && inif->Find("Pages")
			 && ((PagesFileName = inif->PathRight()) != NULL))
				SetPagesFiles();

			if (inif->Section("Templates")
			 && inif->Find("Tables")
			 && ((TablesFileName = inif->PathRight()) != NULL))
				SetTablesFiles();

			if (inif->Section("Templates")
			 && inif->Find("SubFormats")
			 && ((SubFormatsName = inif->PathRight()) != NULL))
				SetSubFormatsFiles();

			if (FmtFile) {
				for (j = 1; j <= FmtFile->IniLevel; j++) {
					DCfrfile *fmtf = (DCfrfile *) FmtFile->IniFileList.find(j);

					if (fmtf->Section("Templates")
					 && fmtf->Find("Pages")
					 && ((PagesFileName = fmtf->PathRight()) != NULL))
						SetPagesFiles();

					if (fmtf->Section("Templates")
					 && fmtf->Find("Tables")
					 && ((TablesFileName = fmtf->PathRight()) != NULL))
						SetTablesFiles();

					if (fmtf->Section("Templates")
					 && fmtf->Find("SubFormats")
					 && ((SubFormatsName = fmtf->PathRight()) != NULL))
						SetSubFormatsFiles();
				}
			}
    }

		if (Usage & 16)
			SetCInisForChain(&FormatFileList, inifmt);
		else if (Usage & 32)
			SetCInisForChain(&TablesFileList, initbl);
		else if (Usage & 64)
			SetCInisForChain(&SubFormatsFileList, inisub);
		else if (Usage & 128)
			SetCInisForChain(&PagesFileList, inipage);

    if (LogIniChains) {
 			DCirfile::WriteIniChain("Formats");
      LogEvent(loginfo, 1, "Processed format files");
		}
	}


	CurrIniFiles.all(GetIniSectionInfo);
}


void CMgr::SetPagesFiles(void)
{
	long j = 0;

	PagesFile = new DCfrfile(PagesFileName);
	if (PagesFile->FErr() != fok) {
		PagesFile->CloseFile();
		LogEvent(logwarn, 1, "Pages file not found: ", PagesFileName);
		PagesFile = NULL;
	}
	else {
		PagesFile = PagesFile->SetConfigTemplate("Templates", "Pages", false);
		if (!DCfrfile::Pages)
			DCfrfile::Pages = PagesFile;
		for (j = 1; j <= PagesFile->IniLevel; j++) {
			char *nm = PagesFile->IniNameList.find(j);
			if (PagesNameList.getitem(nm) != 0) {
				ClearDupFmtIniRefs(&(PagesFile->IniFileList), j);
				break;  // rest of this chain in list
			}
			PagesNameList.add(nm, ++PagesLevel);
			PagesChainList.add(PagesFile->IniChainList.find(j), PagesLevel);
			PagesFileList.add(PagesFile->IniFileList.find(j), PagesLevel);
		}
	}
}


void CMgr::SetTablesFiles(void)
{
	long j = 0;

	TablesFile = new DCfrfile(TablesFileName);
	if (TablesFile->FErr() != fok) {
		TablesFile->CloseFile();
		LogEvent(logwarn, 1, "Tables file not found: ", TablesFileName);
		TablesFile = NULL;
	}
	else {
		TablesFile = TablesFile->SetConfigTemplate("Templates", "Tables", false);
		if (!DCfrfile::Tables)
			DCfrfile::Tables = TablesFile;
		for (j = 1; j <= TablesFile->IniLevel; j++) {
			char *nm = TablesFile->IniNameList.find(j);
			if (TablesNameList.getitem(nm) != 0) {
				ClearDupFmtIniRefs(&(TablesFile->IniFileList), j);
				break;  // rest of this chain in list
			}
			TablesNameList.add(nm, ++TablesLevel);
			TablesChainList.add(TablesFile->IniChainList.find(j), TablesLevel);
			TablesFileList.add(TablesFile->IniFileList.find(j), TablesLevel);
		}
	}
}


void CMgr::SetSubFormatsFiles(void)
{
	long j = 0;

	SubFormatsFile = new DCfrfile(SubFormatsName);
	if (SubFormatsFile->FErr() != fok) {
		LogEvent(logwarn, 1, "SubFormats file not found: ", SubFormatsName);
		SubFormatsFile = NULL;
	}
	else {
		SubFormatsFile = SubFormatsFile->SetConfigTemplate("Templates", "SubFormats", false);
		if (!DCfrfile::SubFormats)
			DCfrfile::SubFormats = SubFormatsFile;
		for (j = 1; j <= SubFormatsFile->IniLevel; j++) {
			char *nm = SubFormatsFile->IniNameList.find(j);
			if (SubFormatsNameList.getitem(nm) != 0) {
				ClearDupFmtIniRefs(&(SubFormatsFile->IniFileList), j);
				break;  // rest of this chain in list
			}
			SubFormatsNameList.add(nm, ++SubFormatsLevel);
			SubFormatsChainList.add(SubFormatsFile->IniChainList.find(j), SubFormatsLevel);
			SubFormatsFileList.add(SubFormatsFile->IniFileList.find(j), SubFormatsLevel);
		}
	}
}


void CMgr::ClearDupIniRefs(DCvlist *flist, long start)
{
	if (!flist
	 || !start)
		return;

	DCvlist *vpl = NULL;
	DCirfile *ff = NULL;

	for (flist->find(start, &vpl); vpl; vpl = vpl->next) {
		if ((ff = (DCfrfile *) vpl->vp) != NULL)
			ff->CloseFile();
	}
}


void CMgr::ClearDupFmtIniRefs(DCvlist *flist, long start)
{
	if (!flist
	 || !start)
		return;

	DCvlist *vpl = NULL;
	DCfrfile *ff = NULL;

	for (flist->find(start, &vpl); vpl; vpl = vpl->next) {
		if ((ff = (DCfrfile *) vpl->vp) != NULL)
			ff->CloseFile();
	}
}


void CMgr::SetCInisForChain(DCvlist *chain, inityp it)
{
	if (!chain)
		return;

	CIni *ini = NULL;
	CIni *lastini = NULL;

	while (chain->next) {
		chain = chain->next;
		if ((it > inimac)
		 && (it < inimod))
			ini = new CIni((DCfrfile *) (chain->vp), it);
		else
			ini = new CIni((DCirfile *) (chain->vp), it);
		CurrIniFiles.append(ini, ++CurrIniCount);
		if (lastini) {
			lastini->nextini = ini;
			ini->previni = lastini;
		}
		lastini = ini;
	}
}
 

void CMgr::GetIniSectionInfo(void *vp, long i)
{
	if (!i)
		return;

	CIni *ci = (CIni *) vp;
	inityp it = ci->ityp;

	if ((it > inimac)
	 && (it < inimod))
		ci->GetFSectionInfo(i);
	else
		ci->GetSectionInfo(i);
}


bool CMgr::CheckCfgMacro(const char* name)
{
	long idx = 0;
	DClist *dl = NULL;
	bool cfg = false;
	bool mac = false;
	SSection *sect = NULL;
	SIni *suff = NULL;

	if (!stricmp(SelectedCategory, FixedMacroSectionsCat)
	 && (!stricmp(name, MacroVariablesSect)
	  || !stricmp(name, MacrosSect)))
		return true;

	if ((idx = AvailSectionNames.getitem(name)) == 0) {
		suff = (SIni *) AvailSuffixIniFiles.find((long) inicfg);
		if (suff
		 && (suff->sectnames.checksuffix(name) != 0))
			return false;
		return true;  // not in lists at all, must be macro
	}
	if ((dl = (DClist *) AvailSections.find(idx)) != NULL) {
		cfg = mac = false;
		while (dl->next) {
			dl = dl->next;
			sect = (SSection *) dl->id;
			if (sect->ifile->ityp == inicfg)
				cfg = true;
			else if (sect->ifile->ityp == inimac)
				mac = true;
		}
		if (mac)
			return true;			
	}
	if (MacroNames.getitem(name) != 0)
		return true;
	if (cfg)
		return false;

	return true;
}


// CIni functions


CIni::CIni(DCirfile *ifl, inityp it)
{
	long pos = 0;

	ini = ifl;
	fini = NULL;
	ityp = it;
	if (ifl) {
		name = NewName(ifl->getBaseName());
		path = NewName(ifl->getFilePath());
		if (ifl->Scope)
			scope = NewName(ifl->Scope);
		else if ((pos = CMgr::ScopeNames.getitem(name)) != 0)
			scope = CMgr::ScopeVals.find(pos);
	}
	previni = nextini = NULL;

	// get SIni based on ityp
	si = (SIni *) CMgr::AvailIniFiles.find(ityp);

	lastsect = sectcount = 0;
	DCnlist *nada = NULL;
	if (path
	 && nada->wildmatch("*\\system\\*", path))
		sys = true;
	else
		sys = false;
}

CIni::CIni(DCfrfile *ifl, inityp it)
{
	long pos = 0;

	ini = NULL;
	fini = ifl;
	ityp = it;

	if (ifl) {
		name = NewName(ifl->getBaseName());
		path = NewName(ifl->getFilePath());
		if (ifl->Scope)
			scope = NewName(ifl->Scope);
		else if ((pos = CMgr::ScopeNames.getitem(name)) != 0)
			scope = CMgr::ScopeVals.find(pos);
	}
	previni = nextini = NULL;

	// get SIni based on ityp
	si = (SIni *) CMgr::AvailIniFiles.find(ityp);

	lastsect = sectcount = 0;
	DCnlist *nada = NULL;
	if (path
	 && nada->wildmatch("*\\system\\*", path))
		sys = true;
	else
		sys = false;
}

CIni::~CIni()
{
	if (sectcount)
		sections.clear();
}


void CIni::GetSectionInfo(long i)
{
	// get the section info from ini, create CSections
	// not in constructor because need to wait until
	// after chaining is disabled to get individually

	DCnlist *names = ini->SectNames();
	CSection *sect = NULL;
	long id = 0;

	hasinvalidsect = false;
	hassectdup = false;

	while (names->next) {
		names = names->next;
		if ((ityp == inimac)
		 && (i <= CMgr::ConfigLevel)
		 && !CMgr::CheckCfgMacro(names->name))  //exclude config sections
			continue;
		// **** check names->name in [ReplacementSections]
		// **** this may give you settings to rename too
		if ((id = sectnames(names->name)) != 0) {
			hassectdup = true;
			if ((sect = (CSection *) sections.find(id)) != NULL)
				sect->hassectdup = true;
		}
		else {  // not a dup, add it
			sectnames.add(names->name, ++sectcount);
			sect = new CSection(names->name, this);
			sections.append(sect, sectcount);
		}
	}
	lastsect = sectcount;

	// if (names to replace) do rename in ini now
	// get info needed for rename, like full path

	ini->CloseFile();
	delete ini;

	ini = NULL;
	// now do the actual renames of sections plus settings
	// and log each change as it is made

}


void CIni::GetFSectionInfo(long)
{
	// get the Formats section info from ini, create CSections
	// not in constructor because need to wait until
	// after chaining is disabled to get individually

	DCnlist *names = fini->SectNames();
	CSection *sect = NULL;
	long id = 0;

	hasinvalidsect = false;
	hassectdup = false;

	while (names->next) {
		names = names->next;
		// **** check names->name in [ReplacementSections]
		// **** this may give you settings to rename too
		if ((id = sectnames(names->name)) != 0) {
			hassectdup = true;
			if ((sect = (CSection *) sections.find(id)) != NULL)
				sect->hassectdup = true;
		}
		else {  // not a dup, add it
			sectnames.add(names->name, ++sectcount);
			sect = new CSection(names->name, this);
			sections.append(sect, sectcount);
		}
	}
	lastsect = sectcount;

	// if (names to replace) do rename in ini now
	// get info needed for rename, like full path

	fini->CloseFile();
	delete fini;

	fini = NULL;
	// now do the actual renames of sections plus settings
	// and log each change as it is made

}


// CSection functions


CSection::CSection(const char *nm, CIni *cini)
{
	name = NewName(nm);
	ifile = cini;
	lastset = setcount = 0;
	ssect = NULL;
	htext = NULL;
	isinvalid = false;
	ismacro = false;
	hassectdup = false;
	hassetdup = false;
	hasinvalidset = false;

	inityp it = cini->ityp;
	long pos = 0; 
	DClist *nmlist = NULL;

	CMgr::CurrSections.append(this, ++CMgr::CurrSectionCount);
	if ((pos = CMgr::CurrSectionNames(name)) != 0) {
		nmlist = (DClist *) CMgr::CurrSectionNameRefs.find(pos);
		if (nmlist)
			nmlist->add((long) this);
	}
	else {
		CMgr::CurrSectionNames.add(name, ++CMgr::CurrSectionNameCount);
		CMgr::CurrSectionNameRefs.add(nmlist = new DClist(),
		                                   CMgr::CurrSectionNameCount);
		nmlist->add((long) this);
	}

	// get nametyp and settyp using info in SSection
	if ((pos = CMgr::AvailIni->sectnames.getitem(name)) != 0)
		ssect = (SSection *) CMgr::AvailIni->sections.find(pos);
	else if ((pos = CMgr::SuffixIni->sectnames.checksuffix(name)) != 0)
		ssect = (SSection *) CMgr::SuffixIni->sections.find(pos);
	else if ((it == inifmt)
	 && ((pos = CMgr::SuffixIni->sectnames.getitem("Para")) != 0))
		ssect = (SSection *) CMgr::SuffixIni->sections.find(pos);
	else if (it != inimac) {   // no ssect, not in a macro ini, looks bad
		if ((pos = CMgr::MacroNames.getitem(name)) != 0) {
			ismacro = true;  // really is a macro
			htext = NewName(CMgr::MacroHelp.find(pos));
		}
		else
			isinvalid = true;
	}

	DCirfile *ifl = ((it > inimac) && (it < inimod)) ? cini->fini : cini->ini;
	// this downcast for fmts works because DCfrfile is derived from DCirfile,
	// and Section(), NextItem(), SrtLeft(), and StrRight() are all DCirfile

	CSetting *set = NULL;
	char *setnm = NULL;
	char *setnp = NULL;
	char *setval = NULL;
	long id = 0;
	CSection *lsec = NULL;
	DClist *dl = NULL;
	bool macroset = false;

	// get the settings from ifile, create CSettings
	if (ifl->Section(nm)) {
		while (ifl->NextItem()) {
			//  **** check for replacement setting names
			// use the new ones, put list where ini rename sees it
			setnm = ifl->StrLeft();
			setval = ifl->StrRight();
			for (setnp = (setnm + strlen(setnm)); setnp > setnm; setnp--) {
				if (isascii(*(setnp - 1))
				 && isspace(*(setnp - 1)))
					*(setnp - 1) = '\0';
				else
					break;
			}

			if ((!setval
			  || !*setval)
			 || (strchr(setnm, '\\')
			  || strchr(setnm, '{')
			  || strchr(setnm, '<'))) {
				macroset = true;
				continue;
			}
			if (!strcmp(setnm, ";=")
			 && (*setval == '='))  // a nasty Carolyn comment
				continue;
			if (strchr(setnm, '<')
			 || ((it == inimac)
			  && (!setval || !*setval)))
				continue;
			if (!stricmp(setnm, ";=help")
			 && !setcount) {
				htext = setval;;
				ismacro = true;  // must be a macro
				isinvalid = false;
			}

			if ((it > inimac)
		 	 && (it < inimod)) {
				if (!stricmp(setnm, "category")) {  // format category
					if (setval && *setval)
						cats = ifl->StrArrRight();
					dl = nmlist;;
					while ((dl = dl->next) != NULL) {
						lsec = (CSection *) dl->id;
						if (lsec == this)
							break;
						if (!lsec->cats)
							lsec->cats = cats;
					}
				}
				else if (!stricmp(setnm, "help")) {  // format help
					if (setval && *setval)
						htext = NewName(setval);
					dl = nmlist;;
					while ((dl = dl->next) != NULL) {
						lsec = (CSection *) dl->id;
						if (lsec == this)
							break;
						if (!lsec->htext)
							lsec->htext = htext;
					}
				}
			}

			if (((id = setnames(setnm)) != 0)
			 && (*setnm != ';')) {  // do not count deleted settings as dups
				hassetdup = true;
				if ((set = (CSetting *) settings.find(id)) != NULL)
					set->hassetdup = true;
			}
			else {  // not a dup, add it
				setnames.add(setnm, ++setcount);
				set = new CSetting(setnm, setval, this);
				settings.append(set, setcount);
			}
		}
		if (hasinvalidset
		 && nmlist->next
		 && (nmlist->next->id != (long) this))
			((CSection *) nmlist->next->id)->hasinvalidset = true;

	}
	lastset = setcount;

	if ((!ssect
	  || ((ssect->nametyp == stvar)
	   && (ssect->settyp == stnone)))
	 && !setcount) {
		ismacro = true;
		isinvalid = false;
	}

	if (macroset
	 && !setcount) {
		ismacro = true;
		isinvalid = false;
	}

	if (isinvalid)
		cini->hasinvalidsect = true;

	if (((it > inimac)
	  && (it < inimod))
	 && !cats) {  // try to get cats from lower format if any
		dl = nmlist;;
		while ((dl = dl->next) != NULL) {
			lsec = (CSection *) dl->id;
			if (lsec == this)
				break;
			if (lsec->cats)
				cats = lsec->cats;
		}
	}
}

CSection::~CSection()
{
	if (name)
		DeleteName(name);
	if (setcount)
		settings.clear();
}



// CSetting functions


CSetting::CSetting(char *nm, char *val, CSection *s)
{
	name = NewName(nm);
	sect = s;
	hassetdup = false;
	isinvalid = false;

	if (val
	 && (*val == ' ')) {  // one must have been trimmed off
		cval = NewName(" ", strlen(val) + 1);
		strcat(cval, val);
	}
	else
		cval = NewName(val);

	long pos = 0; 
	DClist *nmlist = NULL;
;
	CMgr::CurrSettings.append(this, ++CMgr::CurrSettingCount);
	if ((pos = CMgr::CurrSettingNames(name)) != 0) {
		nmlist = (DClist *) CMgr::CurrSettingNameRefs.find(pos);
		if (nmlist)
			nmlist->add((long) this);
	}
	else {
		CMgr::CurrSettingNames.add(name, ++CMgr::CurrSettingNameCount);
		CMgr::CurrSettingNameRefs.add(nmlist = new DClist(),
		                                   CMgr::CurrSettingNameCount);
		nmlist->add((long) this);
	}

#if 0
	// set help for formats
	inityp it = s->ifile->ityp;
	if ((it > inimac)
	 && (it < inimod)) {  // a format
		if (!stricmp(name, "help")
		 && (val && *val))
			s->htext = NewName(val);
	}
#endif

	// get settyp using info in SSetting
	long idx = 0;

	if (sect->ssect) {
		settyp = sect->ssect->settyp;
		if (sect->ssect->fksetcount
		 && ((idx = sect->ssect->fksetnames.getitem(name)) != 0)) {
			sset = (SSetting *) (sect->ssect->fksettings.find(idx));
			settyp = stfix;
		}
		else if (settyp == stboth)
			settyp = stvar;
		else if ((settyp == stfix)
		 && (*name != ';')
		 && stricmp(name, "category")) {
			isinvalid = true;
			sect->hasinvalidset = true;
			CMgr::LogEvent(logerr, 1, "Incorrect fixed setting name \"", name, "\" in [", sect->name, "]");
			CMgr::LogEvent(logcont, 1, "in file ", sect->ifile->path);
		}
	}
}

CSetting::~CSetting()
{
	if (name)
		DeleteName(name);
	if (cval)
		DeleteName(cval);
}


// SIni functions


SIni::SIni(inityp it)  // used for category of avail sections
{
	ityp = it;
	sectcount = 0;
	//CMgr::AvailIniFiles.add(this, (long) it);
}

SIni::~SIni()
{
	if (sectcount) {
		sections.clear();
		sectnames.clear();
	}
}


// SSection functions


SSection::SSection(char *nm, SIni *sini)
{
	name = nm;
	ifile = sini;
	fksetcount = 0;
	show = true;
}


SSection::~SSection()
{
	if (name)
		DeleteName(name);
	if (fksetcount) {
		fksettings.clear();
		fksetnames.clear();
	}
}


// SSetting functions


SSetting::SSetting(char *nm, SSection *s)
{
	name = NewName(nm);
	sect = s;
	fkvals = NULL;
	//valcount = 0;
	show = true;
}

SSetting::~SSetting()
{
	if (name)
		DeleteName(name);
	if (dval)
		DeleteName(dval);
}


// SEnum functions


SEnum::SEnum(char *nm)
{
	name = NewName(nm);
	vhtext = NULL;
	valcount = 0;
}


SEnum::~SEnum()
{
	if (name)
		DeleteName(name);
}




// running a child CM

char *CMgr::ResultFileName = NULL;
char *CMgr::ChildResult = NULL;
bool CMgr::ChildCM = false;


char *ConfigMgr::InvokeChildCM(const char *valuetype, const char *sect)
{
	// valuetype is csect->ssect->vktypes if setname for VK settings
	// else sset->fkvals for an FK setting or ssect->vkvals for VK

	long pos = CMgr::ValueCats.getitem(valuetype);
	if (!pos)
		return NULL;
	const char *childcat = (const char *) pos;
	char *spc = NULL;
	char *cat = NewName(childcat);
	if ((spc = strchr(cat, ' ')) != NULL) {
		*spc = '\0';
		sect = spc + 1;
	}
	RunChildCM(cat, sect);
	ReadResultFile();
	DeleteName(cat);
	return CMgr::ChildResult;
}


void ConfigMgr::RunChildCM(const char *childcat, const char *sect)
{
	// launch with current start, ValueCat, actedit, so Section shown
	char *pcmd = ConvertEnvironment(CMgr::CMexePath);
;	char *pstart = NewName(CMgr::StartFileName);
	CMgr::ResultFileName = ConvertEnvironment(CMgr::CMResPath);

	long len = strlen(pcmd) + strlen(pstart) + strlen(childcat)
		          + strlen(CMgr::ResultFileName) + 26;
	if (sect)
		len += (strlen(sect) * 2) + 14;;

	char *cmd = NewName(pcmd, len);
	strcat(cmd, " -s ");
	strcat(cmd, pstart);
	strcat(cmd, " -c ");
	strcat(cmd, childcat);
	if (sect) {
		strcat(cmd, " -a EditSet ");
		strcat(cmd, " -msect ");
		strcat(cmd, sect);
		strcat(cmd, " -sect ");
		strcat(cmd, sect);
	}
	else
		strcat(cmd, " -a EditSect ");
	strcat(cmd, " -r ");
	strcat(cmd, CMgr::ResultFileName);

	RunCommand(cmd, NULL, false, true);
}

void ConfigMgr::WriteResultFile(wxString &res)
{
	// open file for write and put ResultStr into it, close file
	if (!CMgr::ChildCM)
		return;

	DCwfile *wf = new DCwfile(CMgr::ResultFileName, ftxt);
	wf->putStr(res.c_str());
	wf->putCr();
	wf->CloseFile();
	delete wf;
}

void ConfigMgr::EndChildCM(void)
{
	if (CMgr::ChildCM)
		EndModal(0);
}

void ConfigMgr::ReadResultFile(void)
{
	// read result into ChildResult string and delete file
	if (!wxFile::Exists(CMgr::ResultFileName))
		return;

	wxTextFile file;

	if (!file.Open(wxString(CMgr::ResultFileName)))
		return;

	size_t lines = file.GetLineCount();  // should be one line
	CMgr::ChildResult = NewName(file[0].c_str());
	DeleteFile(CMgr::ResultFileName);
}



// getting file information



#ifdef WIN32

// using Windows dialog

char *ConfigMgr::GetFileInfo(long typ)
{
	char *type = CMgr::FilePickerOptions.find(typ);
	char *filt = CMgr::FilePickerFilter.find(typ);
	char *title = "Select file for setting:";
	char *sdir = NULL;
	bool exist = false;

	// process options
	if (type) {
		if (strchr(type, 'S'))  // src dir
			sdir = NewName(ProjSrcDir.c_str());
		else                    // output dir
			sdir = NewName(ProjOutDir.c_str());
		if (strchr(type, 'W'))  // writable
			exist = false;
		else                    // existing
			exist = true;
	}

	LPOPENFILENAME openinfo = CMgr::SetOpenInfo(sdir, filt, title, exist);
	int retval = GetOpenFileName(openinfo);
	if (!retval) {
		unl errnum = CommDlgExtendedError();
		char nbuf[10];
		if (errnum)  // zero if user cancelled
			CMgr::LogEvent(logerr, 1, "Failure for Open File dialog, error ", ultoa(errnum, nbuf, 10));
		return NULL;
	}

	if (type) {  // sort out which parts to return
		if (strstr(type, "PBX"))  // full path
			return NewName(openinfo->lpstrFile);
		if (strstr(type, "PB"))  // path without ext
			return NewName(openinfo->lpstrFile, openinfo->nFileExtension);
		if (strstr(type, "BX"))  // basename only
			return NewName(openinfo->lpstrFile + openinfo->nFileOffset);
		if (strchr(type, 'B'))   // basename without ext
			return NewName(openinfo->lpstrFile + openinfo->nFileOffset,
						  (openinfo->nFileExtension - openinfo->nFileOffset) - 1);
		if (strchr(type, 'X'))   // ext only
			return NewName(openinfo->lpstrFile + openinfo->nFileExtension);
		if (strchr(type, 'P'))   // path only
			return NewName(openinfo->lpstrFile, openinfo->nFileOffset - 1);
	}

	return NewName(openinfo->lpstrFile);
}


char CMgr::OFNFilterBuffer[256];
char CMgr::OFNPathBuffer[1024];
char CMgr::OFNFileBuffer[256];


LPOPENFILENAME CMgr::SetOpenInfo(char *dir, char *filter,
	                                char *title, bool exist)
{
	char *op = OFNFilterBuffer;
	char *fp = NULL;
	char *efilt = "All files (*.*)|*.*";
	if (filter) {
		fp = filter;
		while (*fp) {
			if (*fp == '|')
				*op = '\0';
			else
				*op = *fp;
			fp++, op++;
		}
		*op++ = '\0';
	}
	fp = efilt;
	while (*fp) {
		if (*fp == '|')
			*op = '\0';
		else
			*op = *fp;
		fp++, op++;
	}
	*op++ = '\0';
	*op++ = '\0';

	OFNPathBuffer[0] = '\0';

	long flags = 0;
	if (exist)
		flags = OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST;
	else
		flags = OFN_OVERWRITEPROMPT;

	LPOPENFILENAME ofn = new OPENFILENAME;
	ofn->lStructSize = sizeof(OPENFILENAME);
	ofn->hwndOwner = NULL;
	ofn->hInstance = NULL;
	ofn->lpstrFilter = OFNFilterBuffer;
	ofn->lpstrCustomFilter = NULL;
	ofn->nMaxCustFilter = 0;
	ofn->nFilterIndex = 0;
	ofn->lpstrFile = OFNPathBuffer;
	ofn->nMaxFile = 1024;
	ofn->lpstrFileTitle = OFNFileBuffer;
	ofn->nMaxFileTitle = 256;
	ofn->lpstrInitialDir = dir;
	ofn->lpstrTitle = title;
	ofn->Flags = flags;
	ofn->nFileOffset = 0;
	ofn->nFileExtension = 0;
	ofn->lpstrDefExt = NULL;
	ofn->lCustData = NULL;
	ofn->lpfnHook = NULL;
	ofn->lpTemplateName = NULL;

	return ofn;
}


#else

// using wx dialog
char *ConfigMgr::GetFileInfo(long typ)
{
	char *type = CMgr::FilePickerOptions.find(typ);
	char *filt = CMgr::FilePickerFilter.find(typ);
	wxString title = wxT("Select file for setting:");
	wxString dir = wxEmptyString;
	wxString fname = wxEmptyString;
	wxString filter = filt ? wxString(filt) : wxEmptyString;
	if (filt)
		filter += wxT("|");
	filter += wxString("All files (*.*)|*.*");
	long opts = wxOPEN;
	// process options
	if (type) {
		if (strchr(type, 'S'))  // src dir
			dir = ProjSrcDir;
		else                    // output dir
			dir = ProjOutDir;
		if (strchr(type, 'W'))  // writable
			opts = wxOPEN;
		else                    // existing
			opts = wxOPEN | wxFILE_MUST_EXIST;
	}

	wxFileDialog dlg(NULL, title, dir, fname, filter, opts);
	dlg.SetDirectory(dir);
	dlg.wxFileDialogBase::SetPath(dir + wxT('\\'));
	// also takes pos and size
	
	if (dlg.ShowModal() != wxID_OK)
		return NULL;

	if (type) {  // sort out which parts to return
		char *ret = NULL;
		char *ext = NULL;
		if (strstr(type, "PBX"))  // full path
			return NewName((dlg.GetPath()).c_str());
		if (strstr(type, "PB")) { // path without ext
			ret = NewName((dlg.GetPath()).c_str());
			if ((ext = strrchr(ret, '.')) != NULL)
				*ext = '\0';
			return ret;
		}
		if (strstr(type, "BX"))  // basename only
			return NewName((dlg.GetFilename()).c_str());
		if (strchr(type, 'B')) {  // basename without ext
			ret = NewName((dlg.GetFilename()).c_str());
			if ((ext = strrchr(ret, '.')) != NULL)
				*ext = '\0';
			return ret;
		}
		if (strchr(type, 'X')) {  // ext only
			ret = NewName((dlg.GetFilename()).c_str());
			if ((ext = strrchr(ret, '.')) != NULL)
				return ext + 1;
			return "";
		}
		if (strchr(type, 'P'))   // path only
			return NewName((dlg.GetDirectory()).c_str());
	}

	return NewName((dlg.GetPath()).c_str());
}

#endif


// utilities


bool CMgr::FileExists(char *fname)
{
	if (!fname)
		return false;

	DCfile *fp = new DCfile(fname);

	if (fp->FErr() == fno) {
		delete fp;
		return false;
	}

	fp->CloseFile();
	delete fp;
	return true;
}


// globals for running commands

char *ComSpec = NULL;
unl ExitType = 0;
unl LocalExitCode = 0;
char PathBuf[MAXPATH];
bool StopCommand = false;
HANDLE CurrProc = NULL;
LPSTARTUPINFO startInfo = NULL;
PROCESS_INFORMATION *procInfo = NULL;


void RunCommand(const char *command, const char *dir, bool hide, bool synch)
{
#ifdef WIN32
	int fmHigh = 0;
	int fmWide = 0;
	int fmLeft = 0;
	int fmTop = 0;

	ExitType = 3;

	startInfo = (LPSTARTUPINFO) NewMem(sizeof(STARTUPINFO));
	if (startInfo == NULL)
		return;

	startInfo->cb = sizeof(STARTUPINFO);
	startInfo->dwFlags = STARTF_USESHOWWINDOW;
	startInfo->wShowWindow = hide ? SW_HIDE : SW_SHOWNORMAL;

	procInfo = (PROCESS_INFORMATION *) NewMem(sizeof(PROCESS_INFORMATION));
	if (procInfo == NULL) {
		DeleteMem(startInfo);
		return;
	}

	ExitType = 0;

	if (!dir) {
		PathBuf[0] = '\0';
		dir = _getcwd(PathBuf, MAXPATH);
	}

	StopCommand = false;

	if (CreateProcess(NULL, (char *) command, NULL, NULL, TRUE, 0,
										NULL, dir, startInfo, procInfo)) {
		CurrProc = procInfo->hProcess;
		if (synch)
			WaitForCurrProcess();
		//WaitForSingleObject(procInfo->hProcess, INFINITE);
	}
	else {
		ExitType = 1;
		LocalExitCode = GetLastError();
		DeleteMem(startInfo);
		DeleteMem(procInfo);
	}

#else

	/* for Mac and UNIX */
	ExitType = 0;
	if ((ExitCode = system(command)) != 0)
		ExitType = 4;

#endif
}


void WaitForCurrProcess(void)
{
	unl objs = 1;
	unl res = 0;
	HANDLE proc[2];
	proc[0] = CurrProc;
	proc[1] = 0;

	while (!StopCommand) {
		res = MsgWaitForMultipleObjects(objs, proc, FALSE,
				                            INFINITE, QS_ALLINPUT);
		if (res == WAIT_OBJECT_0)
			break;
		if (res == WAIT_FAILED) {
			ExitType = 1;
			LocalExitCode = GetLastError();
			break;
		}
		if (res == (WAIT_OBJECT_0 + objs)) {
			MSG msg;
			while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) { 
				// if it's a quit message we're out of here 
        if (msg.message == WM_QUIT)
					return; 
				// otherwise dispatch it 
				TranslateMessage(&msg);
				DispatchMessage(&msg); 
				if (((msg.message == WM_KEYDOWN)
					&& (msg.wParam == VK_CANCEL))
				 || ((msg.message == WM_CHAR)
					&& (msg.wParam == 0x03))) {
					ExitType = 6;
					LocalExitCode = 1;
					StopCommand = true;
					break;
				}
			} // end of PeekMessage while loop 
		}
	}

	if (!StopCommand
	 && !GetExitCodeProcess(procInfo->hProcess, &LocalExitCode)) {
		ExitType = 2;
		LocalExitCode = GetLastError();
	}
	CloseHandle(procInfo->hProcess);
	CloseHandle(procInfo->hThread);
	DeleteMem(startInfo);
	DeleteMem(procInfo);
}


void RunDOSCommand(const char *cmd, const char *dir,
                   bool hide, bool keep, bool synch)
{
	char *command = NULL;

	if (!cmd
	 || (*cmd == '\0'))
		return;

	if (ComSpec == NULL) {
		ComSpec = getenv("comspec");
		if (ComSpec == NULL)
			strcpy(ComSpec, "command");
	}

	command = NewName(ComSpec, strlen(ComSpec) + strlen(cmd) + 5);
	strcat(command, keep ? " /k " : " /c ");
	strcat(command, cmd);

	RunCommand(command, dir, hide, synch);

	DeleteName(command);
}


char *ConvertEnvironment(char *str)
{
	if (!str
	 || !*str)
		return str;

	if (!strchr(str, '%'))
		return str;

	char *cpt = str;
	char *estart = NULL;
	char *eend = NULL;

	for (cpt = str; *cpt; cpt++) {
		if (*cpt == '%') {
			if (!estart)
				estart = cpt + 1;
			else {
				eend = cpt;
				break;
			}
		}
	}

	if (!estart)
		return str;
	if (!eend)
		eend = cpt;

	char *env = NewName(estart, eend - estart);
	char *val = getenv(env);
	if (val == NULL)
		return str;

	char *nstr = NewName((strlen(str) + strlen(val)) - strlen(env));
	if (estart > (str + 1)) {
		strncpy(nstr, str, estart - (str + 1));
		nstr[estart - (str + 1)] = '\0';
  }
	strcat(nstr, val);
	strcat(nstr, eend + 1);

	char *rstr = NULL;
	for (;;) {
		rstr = ConvertEnvironment(nstr);
		if (!rstr)
			return nstr;
		if (nstr == rstr)
			break;
		nstr = rstr;
	}

	return rstr;
}



// end of cmdcl.cpp

