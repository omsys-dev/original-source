// drxml.cpp is the code for dcl XML reading, JHG
// Copyright (c) 2008 by Omni Systems, Inc.  All rights reserved.

#include "dcappl.h"
#include "dcctlg.h"
#include "dcfili.h"
#include "dcfilp.h"
#include "dctxfx.h"
#include "dcfmtr.h"
#include "dcini.h"
#include "dcindx.h"
#include "dcelem.h"
#include "dcmacr.h"
#include "dcauto.h"
#include "drxml.h"
#ifdef DITA
#include "expat.h"
#endif
#ifdef UDOC
#include "mxlparser.h"
#include "drxmlud.h"
#endif
#include "drxmlfl.h"
#include "drxdvfl.h"
#include <direct.h>
#include <windows.h>


// start of code

#ifdef DITA
#ifdef SAMPLE
char *XMLrd::DRxmlVer = { "d005e" };  // demo version identifier
#else
char *XMLrd::DRxmlVer = { "x005e" };
#endif
#endif
#ifdef UDOC
#ifdef SAMPLE
char *XMLrd::DRxmlVer = { "v001" };  // demo version identifier
#else
char *XMLrd::DRxmlVer = { "u001" };
#endif
#endif


swlist XMLrd::SwitchList[] = {
  { "F", NULL, XMLrd::SetFileFormat, true },
  { "f", NULL, XMLrd::SetFileFormat, true },
  { "U", NULL, XMLrd::SetUMXL, false },
  { "u", NULL, XMLrd::SetUMXL, false },
  { NULL, NULL }
};
bool XMLrd::FormatSet = false;
bool XMLrd::PrintProject = false;

short XMLrd::Verbose;
char *XMLrd::IniFileName = NULL;
DCirfile *XMLrd::IniFile = NULL;
XMLrfile *XMLrd::XMLFile = NULL;
DCLwfile *XMLrd::DCLFile = NULL;
DCirfile *XMLrd::LangFile = NULL;
DCfrfile *XMLrd::FmtFile = NULL;
DCfrfile *XMLrd::SubFormatsFile = NULL;
//bool XMLrd::OldNumbering = true;
//bool XMLrd::OldXrefs = true;
outtyp OutputType = otnone;
bool XMLrd::OutputHTML = false;
bool XMLrd::UMXL = false;

char *XMLrd::OutTypeNames[] = {
 "", "RTF", "WinHelp", "HTML", "XHTML",
 "XML", "DITA", "DocBook", "uDoc", 
 "Eclipse", "HTMLHelp", "JavaHelp", "OmniHelp", "OracleHelp",
 "DCL", "MIF", "PDF", "Print", NULL
};
char *XMLrd::OutTypeName = NULL;
bool XMLrd::IsRtf = false;


#ifdef _DLL

#define DllExport extern "C" __declspec( dllexport)

DllExport int
DCLmain(int argc, char* argv[])
{
  static bool dll_init = false;

  if (dll_init) {
    fprintf(stderr, "%s: DLL reinitialization error\n", argv[0]);
    exit(dc_err_arg);
  }
  dll_init = true;


#else

int
main(int argc, char *argv[])
{

#endif

  DCapp app(dr, ".xml");

  app.Init(argc, argv);

  if (!app.Proc(XMLrd::SwitchList)) {
    fprintf(stderr, "usage: %s [-v] [-f[g|h|j|n|w]] [-o dcl_file] xml_file\n", argv[0]);
    exit(dc_err_arg);
  }

  XMLrd::Verbose = app.verbose();
	DCirfile *inif = NULL;

	if (XMLrd::StartingAuto) {
		if (XMLrd::IniFileName
		 && ((inif = new DCirfile(XMLrd::IniFileName)) != NULL)
		 && (inif->FErr() == fok)
		 && (inif->Ferr == fok))
			XMLrd::IniFile = inif;  // make accessible everywhere
		else
			return dc_err_ini;
		XMLrd::InitCurrProps(NULL); // set up vars per ini file
		inif->CloseFile();
		return dc_ok;
	}

	XMLrd::CurrFile = app.rfile();

	// write only to current dir
	char *wfname = app.wfile();
	char *pound = strrchr(wfname, '#');
	char *slash = strrchr(wfname, '/');
	if (!slash)
		slash = strrchr(wfname, '\\');
	if (slash
	 && pound
	 && (slash > pound)) {
		*pound = '\0';
		slash = strrchr(wfname, '/');
		if (!slash)
			slash = strrchr(wfname, '\\');
		*pound = '#';
	}

	// look in current directory for correct ini file
	if (XMLrd::IniFileName
	 && ((inif = new DCirfile(XMLrd::IniFileName, XMLrd::CurrFile)) != NULL)
	 && (inif->FErr() == fok)
	 && (inif->Ferr == fok))
		XMLrd::IniFile = inif;  // make accessible everywhere
#ifdef DITA
	else if (XMLrd::IniFileName
	 && ((inif = new DCirfile(XMLrd::IniFileName + 1, XMLrd::CurrFile)) != NULL)
	 && (inif->FErr() == fok)
	 && (inif->Ferr == fok)) {
		XMLrd::IniFileName++;
		XMLrd::IniFile = inif;  // make accessible everywhere
	}
#endif
	else
		return dc_err_ini;

	if (!XMLrd::InitCurrProps(XMLrd::CurrFile)) { // set up vars per ini file
		inif->CloseFile();	 // OnlyAuto, no processing
		return dc_ok;
	}

  XMLrfile rf(XMLrd::CurrFile);
  if (rf.FErr() != fok)
    rf.Err();
	else
		XMLrd::XMLFile = &rf;

  DCLwfile wf((slash ? (slash + 1) : wfname), app.wtype());
  if (wf.FErr() != fok)
    wf.Err();
	else
		XMLrd::DCLFile = &wf;

  XMLrd::SetUpPartFiles(&wf);

	DCmacr::InitMacroModule((DCwfile *) &wf, inif, NULL, NULL,
	    NULL, NULL, NULL, NULL, NULL);

  XMLrd::ParseXML(&rf);   // parse entire xml file
  XMLrd::WriteDCL(&wf);   // put out the dcl resources and page images

	inif->CloseFile();	// no need to check ini file for errors
  rf.CloseFile();
  wf.CloseFile();
  wf.Err();      // if error, exits with the err (and a message)
  return dc_ok;  // so if we get here, it worked
}


void
XMLrd::SetUMXL(char *fmt)
{
	UMXL = true;

	// set any changed defaults from DITA here
	KeepDraftComments = false;

}


void
XMLrd::SetFileFormat(char *fmt)
{
	//bool newform = false;
	//unc cnt = 0;

	// fmt can be Help, Word, Net, Java, MS HH, GenXML
	FormatSet = true;  // overrules .ini
	OldFormType = toupper(*fmt);
	//IniFileName = DCirfile::GetIniName("_d2", fmt, &OldFormType);

#if 0
	if (UMXL) {
		LogIniPrefix = XMLud::LogIniPrefix;
		LogFileName = NewName(LogIniPrefix, strlen(LogIniPrefix) + 10);
		strcat(LogFileName, "g_log.txt");
		HistoryFileName = NewName(LogIniPrefix, strlen(LogIniPrefix) + 14);
		strcat(HistoryFileName, "g_history.txt");
	}
#endif

	char ch = '\0';
	IniFileName = DCirfile::GetIniName(LogIniPrefix,
		                                 fmt, &OldFormType, &ch);
	switch (ch) {
		case 'L':
			StartingAuto = true;
			BookAuto = true;
			break;

		case 'F':
			StartingAuto = true;
			BookAuto = false;
			break;

		default:
			break;
	}

#if 0
	IniFileName = NewName("d2", 20);
	char ch = toupper(*fmt);
	if (strlen(fmt) > 2) {
		newform = true;
		if (!strnicmp(fmt, "htmlhelp", 8))
			ch = 'M', cnt = 8, strcat(IniFileName, "htmlhelp");
		else if (!strnicmp(fmt, "javahelp", 8))
			ch = 'J', cnt = 8, strcat(IniFileName, "javahelp");
		else if (!strnicmp(fmt, "oraclehelp", 10))
			ch = 'O', cnt = 10, strcat(IniFileName, "oraclehelp");
		else if (!strnicmp(fmt, "eclipse", 7))
			ch = 'E', cnt = 7, strcat(IniFileName, "eclipse");
		else if (!strnicmp(fmt, "html", 4))
			ch = 'N', cnt = 4, strcat(IniFileName, "html");
		else if (!strnicmp(fmt, "xhtml", 5))
			ch = 'X', cnt = 5, strcat(IniFileName, "xhtml");
		else if (!strnicmp(fmt, "xml", 3))
			ch = 'G', cnt = 3, strcat(IniFileName, "xml");
		else if (!strnicmp(fmt, "docbook", 7))
			ch = 'B', cnt = 7, strcat(IniFileName, "docbook");
		else if (!strnicmp(fmt, "dita", 4))
			ch = 'D', cnt = 4, strcat(IniFileName, "dita");
		else if (!strnicmp(fmt, "omnihelp", 8))
			ch = 'C', cnt = 8, strcat(IniFileName, "omnihelp");
		else if (!strnicmp(fmt, "rtf", 3))
			ch = '8', cnt = 3, strcat(IniFileName, "rtf");
		else if (!strnicmp(fmt, "winhelp", 7))
			ch = 'H', cnt = 7, strcat(IniFileName, "winhelp");
		else if (!strnicmp(fmt, "dcl", 3))
			ch = 'A', cnt = 3, strcat(IniFileName, "dcl");
		else
			ch = 'Z', strcat(IniFileName, "html");
	}
	strcat(IniFileName, ".ini");
#endif

	switch (OldFormType) {
		case 'W':
		case '7':
		case '8':
		case 'P':
			OutType = 'S';
			HelpType = 'S';
			FormType = 'R';
			OutputType = otrtf;
			PrintProject = true;
			break;
		case 'H':  // WinHelp
		case '3':  // WinHelp3
		case '4':  // WinHelp4
			OutType = 'H';
			HelpType = 'H';
			FormType = 'R';
			OutputType = otwh;
			break;

		case 'J':  // JavaHelp
			OutputType = otjh;
			goto hhelp;
		case 'M':  // MS HTML Help
			OutputType = othh;
			goto hhelp;
		case 'O':  // OracleHelp
			OutputType = otohj;
			goto hhelp;
		case 'C':  // OmniHelp
			OutputType = otomh;
			goto hhelp;
		case 'E':  // Eclipse
			OutputType = oteh;

		hhelp:
			OutType = 'N';
			HelpType = ch;
			FormType = 'H';
			break;

		case 'N':  // Net, for mif2htm
			OutputType = othtm;
			goto htmf;
		case 'X':  // XHTML
			OutputType = otxhtm;
			goto htmf;
		case 'G':  // Generic XML
			OutputType = otxml;
			goto htmf;
		case 'D':  // DITA
			OutputType = otdita;
			goto htmf;
		case 'B':  // DocBook
			OutputType = otdbk;
			goto htmf;
		case 'U':  // uDoc
			OutputType = otudoc;
			goto htmf;

		case 'A':  // ASCII DCL
			OutputType = otdcl;
			goto htmf;

		case 'F':  // Frame MIF
			OutputType = otmif;
			OutType = 'N';
			HelpType = 'S';
			FormType = 'R';
			break;

		htmf:
			OutType = 'N';
			HelpType = 'S';
			FormType = 'H';
			break;

#if 0
		case 'A':  // ASCII DCL
			if (!newform) {
				OutType = FileExists("xml2htm.ini") ? 'N' : 'S';
				FormType = FileExists("xml2htm.ini") ? 'H' : 'R';
			}
			else
				OutType = 'N', FormType = 'H';
			HelpType = 'S';
			break;
#endif

		default:  // unknown
			FormatSet = false;
			FormType = 'N';
			break;
	}

	if ((OutputType == othtm) 
	 || (OutputType == otxhtm))
		OutputHTML = true;

	OutTypeName = OutTypeNames[OutputType];
	IsRtf = (!stricmp(OutTypeName, "rtf")) ? true : false;
}




// process ini-file options

bool XMLrd::StartingAuto = false;
bool XMLrd::BookAuto = false;

bool XMLrd::DoAutomation = false;
char XMLrd::OutType = 'S'; // Standard, Help, or Net
char XMLrd::HelpType = 'S'; // Standard, Help, MS, Java
char XMLrd::FormType = 'N'; // None, Rtf, Html
char XMLrd::OldFormType = 'N'; // single-letter -f flag
char XMLrd::ObjectIDs = 'A'; // None or All
long XMLrd::CurrObjectID = 0;

bool XMLrd::CaselessMatch = true;
bool XMLrd::SpacelessMatch = true;
bool XMLrd::WildcardMatch = true;
bool XMLrd::ProcConrefs = true;
char *XMLrd::IDFileName = NULL;
char *XMLrd::PrjFileName = NULL;
char *XMLrd::ConfigTemplate = NULL;
char *XMLrd::LanguageText = NULL;

bool XMLrd::UseLog = true;
bool XMLrd::ShowLog = true;

#ifdef DITA
char *XMLrd::LogIniPrefix = "_d2";
char *XMLrd::LogFileName = "_d2g_log.txt";
char *XMLrd::HistoryFileName = "_d2g_history.txt";
#endif

#ifdef UDOC
char *XMLrd::LogIniPrefix = "_x2";
char *XMLrd::LogFileName = "_x2g_log.txt";
char *XMLrd::HistoryFileName = "_x2g_history.txt";
#endif

char *XMLrd::EditorFileName = "notepad.exe";
DClog *XMLrd::Log = NULL;
unc XMLrd::LogLevels[logmax] = { 0, 1, 1, 1, 1, 0 };
bool XMLrd::LogIniChains = false;

char *XMLrd::MacroFile = "d2gmacro.ini";

DCnlist XMLrd::Align;
npair XMLrd::AlignInit[] = {
 { 1, "left"},
 { 2, "center"},
 { 3, "right"},
 NULL
};
DCnlist XMLrd::Valign;
npair XMLrd::ValignInit[] = {
 { 1, "middle"},
 { 2, "top"},
 { 3, "bottom"},
 NULL
};
DCnlist XMLrd::TrademarkTypes;
npair XMLrd::TrademarkTypesInit[] = {
 { 1, "tm"},
 { 2, "reg"},
 { 3, "service"},
 NULL
};

spprop XMLrd::CurrElem;
char **XMLrd::CurrElemType = NULL;
bool XMLrd::SetupComplete = false;
HANDLE PMFlag = NULL;


bool
XMLrd::InitCurrProps(char *rfname)
{
	char *fname = NULL;

	if (!IniFile)
		return false;

	// set IniFile to point to top of config chain
	IniFile = IniFile->SetConfigTemplate("Templates", "Configs");

	if (IniFile->Section("Options")) {	// read ini-interpretation info
		if (IniFile->Find("CaselessMatch"))
			IniFile->SetMatch(1, IniFile->BoolRight());
		if (IniFile->Find("SpacelessMatch"))
			IniFile->SetMatch(2, IniFile->BoolRight());
		if (IniFile->Find("WildcardMatch"))
			IniFile->SetMatch(3, IniFile->BoolRight());
		if (IniFile->Find("NoNameDel"))
			::NoNameDel = IniFile->BoolRight();
		if (IniFile->Find("NoMemDel"))
			::NoMemDel = IniFile->BoolRight();
	}


	if ((!FormatSet)
	 && IniFile->Section("Options")) {
		if (IniFile->Find("Output"))
			OutType = IniFile->UCRight();
		if (IniFile->Find("HelpType"))
			HelpType = IniFile->UCRight();
	}

	if (HelpType != 'S') {
		char *SubType = NULL;

		switch (HelpType) {
			case 'J':
				SubType = "Java";
				break;
			case 'O':
				SubType = "Oracle";
				break;
			case 'E':
				SubType = "Eclipse";
				break;
			case 'M':
				SubType = "HH";
				break;
			case 'G':
				SubType = "HTML";
				break;
			case 'C':
				SubType = "OH";
				break;
			case 'H': // WinHelp
			default:
				break;
		}
		if (SubType)
			IniFile->SetType(SubType);
	}

	if (OutType != 'S') {
		ObjectIDs = 'A';
	}

	DCini::InitIniModule(IniFile, XMLIniSects, &CurrElem);
	DCini::ProcAllSects();
	IniFile->GetElemSets();


	// set up logging

	if (UseLog) {
#ifdef DITA
		PMFlag = OpenSemaphore(SEMAPHORE_ALL_ACCESS, FALSE, "d2gpm");
#endif
#ifdef UDOC
		PMFlag = OpenSemaphore(SEMAPHORE_ALL_ACCESS, FALSE, "x2gpm");
#endif

		if (ShowLog
		 && !PMFlag
		 && DCrfile::FileExists(LogFileName)) {  // append to history
			char *cmd = NewName("type ", strlen(LogFileName) + 
				                           strlen(HistoryFileName) + 8);
			strcat(cmd, LogFileName);
			strcat(cmd, " >>");
			strcat(cmd, HistoryFileName);
			DCauto::RunDOSCommand(cmd);
			strcpy(cmd, "del /Q ");
			strcat(cmd, LogFileName);
			DCauto::RunDOSCommand(cmd);
			DeleteName(cmd);
		}
#ifdef DITA
		Log = new DClog(LogFileName, "drxml", DRxmlVer, LogLevels);
#endif
#ifdef UDOC
		Log = new DClog(LogFileName, "drmxl", DRxmlVer, LogLevels);
#endif
		if (Log->FErr() != fok)
			UseLog = false;
		XMLrfile::LogEventFunc = LogEvent;
		//LogFileName = Log->getFilePath();
	}
	DCirfile::LogEventFunc = (logfunc) LogEvent;
	DCirfile::LogIniChain = LogIniChains;


	// set up automation

	DCauto::LogEventFunc = (logfunc) LogEvent;
	DCauto::LogFileName = LogFileName;
	DoAutomation = DCauto::SetUpAutomation(OutputType, IniFile,
		                                     NULL, NULL, BookAuto);

	if (DCauto::UseExistingDCL) {
		if (UseLog) {
			Log->logEvent(loginfo, 1, "UseExistingDCL set, skipping XML file ");
			Log->CloseFile();
			CheckLogErrors();
		}
		return false;
	}

	DCauto::DeleteDCL();

	if (UseLog)
		Log->logEvent(loginfo, 1, "Opened XML project file ", CurrFile);

	if (LogIniChains)
		DCirfile::WriteIniChain("Configs");

	if (LogIniChains)
		Log->logEvent(loginfo, 1, "Processed Config ini chains");

	if (IniFile->Section("Templates")
	 && (IniFile->Find("Macros")
	  || IniFile->Find("MacroFile")))
		MacroFile = IniFile->PathRight();
	DCauto::MacroFile = MacroFile;

	if (DoAutomation)
		//DCauto::WrapShipPreProc(false);
		DCauto::WrapShipPreProc(BookAuto);

	if (StartingAuto) {
		if (UseLog) {
			Log->logEvent(loginfo, 1, "Automation processing completed by drxml.");
			Log->CloseFile();
		}
		CheckLogErrors();
		return false;
	}

	if (DCauto::OnlyAuto) {  // no processing at all
		if (UseLog) {
			Log->logEvent(loginfo, 1, "OnlyAuto set, skipping XML file ");
			Log->CloseFile();
		}
		CheckLogErrors();
		return false;
	}

	// set up language files

	if (IniFile->Section("Templates")
	 && (IniFile->Find("Languages")
	  || IniFile->Find("LanguageText"))) {
		LanguageText = IniFile->PathRight();
		LangFile = GetLangFile(&LanguageText, &CurrLang, true);
		if (!LangFile) {
			LanguageText = NULL;
			CurrLang = "en";
			LangFile = GetLangFile(&LanguageText, &CurrLang, true);
		}
		if (LangFile) {
			LangStack.add(LangFile, ++LangLevel);
			LangDepthStack.add(1, LangLevel);
			LangAttrStack.add(CurrLang, LangLevel);
		}
	}

	if (LogIniChains)
		Log->logEvent(loginfo, 1, "Processed language file");

	// set up templates files

	if (IniFile->Section("Templates")
	 && IniFile->Find("Formats")) {
		fname = IniFile->PathRight();
		FmtFile = new DCfrfile(fname);
		if (FmtFile->FErr() != fok) {
			LogEvent(logwarn,1,"Formats file not found: ", fname);
			FmtFile = NULL;
		}
		else {
			FmtFile = FmtFile->SetConfigTemplate("Templates", "Formats");
			DCfrfile::Formats = FmtFile;
		}
	}

	if ((IniFile->Section("Templates")
	  && IniFile->Find("SubFormats")
	  && ((fname = IniFile->PathRight()) != NULL))
	 || (FmtFile
	  && FmtFile->Section("Templates")
		&& FmtFile->Find("SubFormats")
	  && ((fname = FmtFile->PathRight()) != NULL))) {
		SubFormatsFile = new DCfrfile(fname);
		if (SubFormatsFile->FErr() != fok) {
			LogEvent(logwarn,1,"SubFormats file not found: ", fname);
			SubFormatsFile = NULL;
		}
		else {
			SubFormatsFile = SubFormatsFile->SetConfigTemplate("Templates", "SubFormats");
			DCfrfile::SubFormats = SubFormatsFile;
		}
	}

	if (LogIniChains)
		Log->logEvent(loginfo, 1, "Processed format files");


#ifdef DITA
	XMLrfile::LocalDTDPath = DTDPath;
#endif

	if (OutType == 'H')      // for WinHelp
		DCini::ProcIniSect("HelpOptions");
	else if (OutType == 'S') // for Word
		DCini::ProcIniSect("WordOptions");
	else if (OutType == 'N') // for HTML/XML
		DCini::ProcIniSect("HTMLOptions");

	DCini::ProcIniSect("Setup");  // to override FileSuffix elsewhere

	if (LogIniChains)
		Log->logEvent(loginfo, 1, "Processed Help sections");


	// set up DCix functions

	char *form = NULL;
	long lev = 0;
	DCnlist *ixentforms = new DCnlist();
	if (IniFile->Section("IndexEntryFormats")) {
		// level = format
		while (IniFile->NextItem()) {
			lev = atoi(IniFile->StrLeft());
			form = IniFile->StrRight();
			ixentforms->add(form, lev);
		}
	}
	DCnlist *ixrefforms = new DCnlist();
	if (IniFile->Section("IndexRefParaFormats")) {
		// level = format
		while (IniFile->NextItem()) {
			lev = atoi(IniFile->StrLeft());
			form = IniFile->StrRight();
			ixrefforms->add(form, lev);
		}
	}

	if (!MakeDefaultIDX)
		UseDefList = false;

	DCixset::LogEventFunc = (logfunc) LogEvent;
	char *basename = NewName(strlen(CurrFile));
	GetBaseName(CurrFile, NULL, basename);
	DCixset::listset = new DCixset(IniFile, LangFile, basename,
	 UseCompactForm, (FormType == 'R'), ixentforms, ixrefforms,
	 IDXTitleFormat, IDXTOCFormat, IDXFile, IDXSuffix,
	 UseIndexHeading, UseIndexLeader, UseIndexLetters, 
	 UseIndexTopLetters, MakeDefaultIDX, SortSeeAlsoFirst,
	 IgnoreLeadingCharsIX, IgnoreCharsIX);

#if 0
	// set up predefined variables
	PredefineVariable("StepImportanceRequired", ImportanceRequired);
	PredefineVariable("StepImportanceOptional", ImportanceOptional);
#endif

	// set up footnotes and table footnotes
	//SetFootnoteInfo(false);
	//SetFootnoteInfo(true);

	// set up alignment lookups
	Align.init(AlignInit);
	Valign.init(ValignInit);
	GraphPos = Align(GraphicAlignment);

	ChunkByTypes.init(InitChunkByTypes);
	ChunkSelTypes.init(InitChunkSelTypes);

	LinkRoles.init(LinkRolesInit);
	LinkScopes.init(LinkScopesInit);
	LinkTopicTypes.init(LinkTopicTypesInit);
	LinkingTypes.init(LinkingTypesInit);
	LinkForms.init(LinkFormsInit);
	LinkCollTypes.init(LinkCollTypesInit);

#ifdef UDOC
	GenListNames.init(InitUMXLGenListNames);
#endif
#ifdef DITA
	GenListNames.init(InitGenListNames);
#endif
	HyperTypes.init(InitHyperTypes);
	ImageMapShapes.init(ImageMapShapesInit);
	TrademarkTypes.init(TrademarkTypesInit);

#if 0
	NoteTypes.init(NoteTypesInit);
	NoteHeads.init(NoteHeadsInit);
	NoteFormats.init(NoteFormatsInit);
	NoteGroups.init(NoteGroupsInit);
	NoteHeads.all(SetNoteGroup);

	TaskHeadTypes.init(TaskHeadTypesInit);
	TaskHeads.init(TaskHeadsInit);
	TaskHeadGroups.init(TaskHeadGroupsInit);
	TaskHeads.all(SetTaskHeadGroup);
#endif
	TaskHeadTypes.init(TaskHeadTypesInit);

	SetDefFormatPrefixHeads(false);
	SetDefFormatPrefixHeads(true);

	SetQuotes();

#ifdef DITA
	SetCatalogs();
#endif

	//if (UMXL) {
#ifdef UDOC
		XMLud::ProcUMXLElemTypes();
		XMLud::SetUpLists();
#endif
	//}
	//else
#ifdef DITA
		ProcCurrElemTypes();
#endif

	SetFilterAttributes();
	InitGlossAbbr();

#ifdef SAMPLE
	SetUpSample();
#endif

	InitDocRef = new DCmapelem(DCelem(0));

	if (LogIniChains)
		Log->logEvent(loginfo, 1, "Finished ini processing");

	SetupComplete = true;
	return true;
}


void
XMLrd::LogEvent(logtype lt, unc sev, const char *desc,
 const char *d2, const char *d3, const char *d4, const char *d5)
{
	if (!UseLog)
		return;

	Log->logEvent(lt, sev, desc, d2, d3, d4, d5);
}



DCnlist XMLrd::LangFiles;
DCnlist XMLrd::LangIDs;


DCirfile *
XMLrd::GetLangFile(char **ltext, char **langid, bool first)
{
	DCirfile *lfile = NULL;
	DCirfile *ofile = NULL;

	if (ltext
	 && *ltext) {
		if (LangFiles(*ltext))
			return (DCirfile *) LangFiles(*ltext);
		lfile = new DCirfile(*ltext);
		if (lfile->FErr() != fok) {
			lfile->CloseFile();
			delete lfile;
			LogEvent(logwarn,1,"Language file not found: ", *ltext);
			return NULL;
		}
		ofile = lfile;
		lfile = lfile->SetConfigTemplate("Templates", "Languages");
		if (!LangFiles(*ltext))
			LangFiles.addnew(*ltext, (long) lfile);
	}
	else if (langid
	 && *langid) {
		if (LangIDs(*langid))
			return (DCirfile *) LangIDs(*langid);
		char *ntext = NewName("%omsyshome%\\d2g\\local\\lang\\local_d2g_lang_", 
			strlen(*langid) + 46);
		strcat(ntext, *langid);
		strcat(ntext, ".ini");
		lfile = new DCirfile(ntext);
		if (ltext)
			*ltext = ntext;
		if (lfile->FErr() != fok) {
			char *hyph = NULL;
			if ((hyph = strchr(*langid, '-')) != NULL) {
				lfile->CloseFile();
				delete lfile;
				strcpy(ntext + 42 + (hyph - *langid), ".ini");
 				lfile = new DCirfile(ntext);
			}
			if (lfile->FErr() != fok) {
				LogEvent(logwarn,1,"Language file not found for: ", *langid);
				lfile->CloseFile();
				delete lfile;
				return NULL;
			}
		}
		ofile = lfile;
		lfile = lfile->SetConfigTemplate("Templates", "Languages");
		if (!LangIDs(*langid))
			LangIDs.addnew(*langid, (long) lfile);
	}

	char *lname = ofile->getBaseName();
	char *lpt = NULL;
	if (!strnicmp(lname, "local_d2g_lang_", 15)) {
		if (langid)
			*langid = NewName(lname + 15);
		else
			CurrLang =  NewName(lname + 15);
	}
	else if ((lpt = strstr(lname, "lang_")) != NULL) {
		if (langid)
			*langid = NewName(lpt + 5);
		else
			CurrLang =  NewName(lpt + 5);
	}
	else if (first)
		LogEvent(logwarn,1,"Language not identified, assuming \"en\"");
	else 
		LogEvent(logwarn,1,"Language not identified for ", *ltext);
	RestoreLangFile(lfile);
	return lfile;
}


void
XMLrd::RestoreLangFile(DCirfile *lfile)
{
	DCini::InitIniModule(lfile, LangIniSects);
	DCini::ProcAllSects();  // get LangFile settings
	// restore config setup
	DCini::InitIniModule(IniFile, XMLIniSects, &CurrElem);
	DCini::ProcAllSects();  // override lang settings

	// reset derivative vars if already set
	if (QuoteChars[0])
		SetQuotes();
	if (PrefixRuninCount) {
		DCirfile *prevlang = LangFile;
		LangFile = lfile;
		PrefixRuninNames.empty();
		PrefixRuninGroups.empty();
		PrefixRuninCount = 0;
		SetDefFormatPrefixHeads(false);
		SetDefFormatPrefixHeads(true);
#if 0
		char *nm = NULL;
		DCgroup *grp = NULL;
		for (long i = 1; i <= PrefixRuninCount; i++) {
			nm = PrefixRuninNames.find(i);
			grp = (DCgroup *) PrefixRuninGroups.find(i);
			grp->empty();
			WriteRuninHead(grp, nm);
		}
#endif
		LangFile = prevlang;
	}
}


void
XMLrd::ResetLangFile(void)
{
	if (LangLevel < 2)
		return;
	LangFile = (DCirfile *) LangStack.find(1);
	CurrLang = LangAttrStack.find(1);
	LanguageText = LangFile->getFilePath();
	while (LangLevel > 1) {
		LangStack.remove(LangLevel);
		LangDepthStack.remove(LangLevel);
		LangAttrStack.remove(LangLevel);
		LangLevel--;
	}
	RestoreLangFile(LangFile);
}


char *
XMLrd::GetParentLang(DCelem *einfo)
{
	while (einfo->parent) {
		if (einfo->parent->lang)
			return (einfo->parent->lang);
		einfo = einfo->parent;
	}
	return LangAttrStack.find(1);
}


#ifdef DITA

void
XMLrd::SetCatalogs(void)
{
	// set XMLrfile::Catalogs and CatalogPathUsed
	if (!IniFile->Section("Catalogs")
	 || (IniFile->Find("UseCatalogs")
	  && (IniFile->BoolRight() == false))) {
		XMLrfile::CatalogPathUsed = false;
		return;
	}

	char *cat = NULL;
	long cnum = 0;

	if (IniFile->Find("CatalogKeys")) {
		char **cats = IniFile->StrArrRight();
		while (*cats) {
			if (IniFile->Find(*cats)
			 && ((cat = IniFile->StrRight()) != NULL)) {
				XMLrfile::Catalogs.add(cat, ++cnum);
			}
			else
				LogEvent(logwarn,1,"Catalog key not found: ", *cats);
			cats++;
		}
	}
	else if (IniFile->Find("BaseCatalog")
	 && ((cat = IniFile->StrRight()) != NULL)) {
		XMLrfile::Catalogs.add(cat, cnum = 1);
	}

	if (!cnum) {
		LogEvent(logwarn,1,"No XML Catalogs found");
		XMLrfile::CatalogPathUsed = false;
	}
}


void
XMLrd::ProcCurrElemTypes(void)
{
	long i = 0;

	ElemTypesCount = TopicElemTypesCount;
	ElemTypes = (etype **) new char[sizeof(etype *) * ElemTypesCount];

	for (i = 0; i < ElemTypesCount; i++)
		ElemTypes[i] = &TopicElemTypes[i];

	qsort(ElemTypes, ElemTypesCount, sizeof(etype *), CompElemTypes);

	if (IniFile->Section("ElementTypes")) {

		EpNames.init(EpNameInit);

		// scan section, getting key and its props

		etype *key = NULL;
		char **evals = NULL;
		DCnlist etypes;
		DCvlist ekeys;
		long tnum = 0;
		char *str = NULL;

		while (IniFile->NextItem()) {
			if ((str = IniFile->StrLeft()) == NULL)
				break;
			key = new etype;
			key->ename = NormalizeClass(str);
			evals = IniFile->StrArrRight();
			ConvertSarr(evals, key);
			etypes.addsorted(str, ++tnum);
			ekeys.add(key, tnum);
		}

		etype **arr = ElemTypes;
		int cnt = ElemTypesCount;
		etype **ep = NULL;
		DCvlist newelems;
		//DCnlist newkeys;
		long nnum = 0;
		long pos = 0;

		for (i = 1; i <= tnum; i++) {
			//key = new etype;
			//key->ename = CurrElem.stag = (char *) etypes.dequeue();
			//CurrElemType = NULL;
			//DCini::ProcIniSect("ElementTypes");  // sets CurrElemType
			// convert CurrElemType to unc array, set count
			//ConvertSarr(CurrElemType, key);

			etypes.dequeue(&pos);
			key = (etype *) ekeys.find(pos);
			ep = (etype **) bsearch(&key, arr, cnt, sizeof(etype *),
																		CompElemTypes);
			if (ep)	{// replace original table entry if any
				(*ep)->cnt = key->cnt;
				memcpy((*ep)->eprop, key->eprop, epCntMax);
				delete key;
			}
			else {
				newelems.add(key, ++nnum);
				//newkeys.addsorted(NewName(key->ename), nnum);
			}
		}

		// insert new entries in order to ElemTypes
		if (nnum) {
			ElemTypesCount += nnum;
			etype **et = (etype **) new char[sizeof(etype *) * ElemTypesCount];
			long j = 0;
			long k = 0;
			//long n = 0;
			//DCnlist *ki = &newkeys;
			for (i = 1; i <= nnum; i++) {
				//ki = ki->next;
				//n = ki->id;
				//key = (etype *) newelems.find(n);
				key = (etype *) newelems.dequeue();
				while (j < ElemTypesCount) {
					if (stricmp(ElemTypes[j]->ename, key->ename) < 0)
						et[k++] = ElemTypes[j++];
					else
						break;
				}
				et[k++] = key;
			}
			while ((k < ElemTypesCount)
			 && (j <= cnt))
				et[k++] = ElemTypes[j++];

			delete [] ElemTypes;
			ElemTypes = et;
		}
	}

	DCelem::SetElemTypes(ElemTypes, ElemTypesCount);
}
#endif


void 
XMLrd::ConvertSarr(char **sarr, etype *et)
{
	if (!sarr)
	  return;

	unc *ep = et->eprop;
	et->cnt = 0;

	while (*sarr) {
		*ep = (unc) EpNames.getitem(*sarr);
		if (*ep) {
			ep++;
			et->cnt++;
			if (et->cnt == epCntMax)
				break;
		}
		else {
			LogEvent(logwarn, 1, "Invalid element type: ", *sarr,
			                     " for: ", et->ename, " ignored");
		}
		sarr++;
	}

	if (*sarr) {  // overflow
		LogEvent(logwarn, 2, "Too many element types for: ", et->ename,
			       ", ignoring: ", *sarr, " and any following it");
	}
}




// XML parsing functions

char *XMLrd::CurrFile;

DCvlist XMLrd::DocsToGo;
DCvlist XMLrd::DocRefs;
long XMLrd::CurrDocLevel = 1;
long XMLrd::DocCount = 0;
long XMLrd::CurrDocNum = 0;
long XMLrd::NextDocNum = 0;
DCmapelem *XMLrd::CurrDocRef = NULL;
DCmapelem *XMLrd::InitDocRef = NULL;
DCtopdoc *XMLrd::CurrDocInfo = NULL;
DCvlist XMLrd::DocInfos;

long XMLrd::CurrTopicNum = 0;
DCtopic *XMLrd::CurrTopicInfo = NULL;

DCnlist XMLrd::DocRefsProcessed;
DCmlist XMLrd::DocNamesProcessed;
DCmlist XMLrd::ConrefDocNamesProcessed;
char *XMLrd::DocRef = NULL;
char *XMLrd::DocName = NULL;
char *XMLrd::DocBasePath = NULL;
char *XMLrd::DocBaseName = NULL;
char *XMLrd::MainDocName = NULL;
char *XMLrd::MainBaseName = NULL;
char *XMLrd::DocID = NULL;
char *XMLrd::DocElement = NULL;
char *XMLrd::DTDPath = NULL;
bool XMLrd::FirstTopic = false;
bool XMLrd::ConrefTopic = false;
bool XMLrd::SkipTopic = false;
long XMLrd::SkipNesting = 0;

DCvlist XMLrd::MapsToGo;
DCvlist XMLrd::MapRefs;
long XMLrd::CurrMapLevel = 1;
long XMLrd::MapCount = 0;
long XMLrd::CurrMapNum = 0;
DCmapelem *XMLrd::CurrMapRef = NULL;
long XMLrd::CurrMapPos = 0;
DCmap *XMLrd::CurrMapInfo = NULL;
DCnlist XMLrd::MapNamesProcessed;
DCnlist XMLrd::ConrefMapNamesProcessed;
DCvlist XMLrd::MapInfos;

bool XMLrd::KeydefsDone = false;
bool XMLrd::ProcessingKeyrefMaps = false;
DCvlist XMLrd::KeyrefMapsToGo;
DCvlist XMLrd::KeyrefMapRefs;

DCnlist XMLrd::DocRoots;
char *XMLrd::DocRoot = NULL;
bool XMLrd::InitialParse = true;
bool XMLrd::GenerateMapIfMissing = true;
bool XMLrd::UsingBookmap = false;


void
XMLrd::ParseXML(XMLrfile *rf)
{
	char *pound = NULL;

#ifdef DITA
	SetNewConditionSet(DitavalFile);
#endif

	//if (!ProcessDitavalFile())
	//	SetCondAttrs();

	ParseHref(CurrFile, &DocName, &DocID, &DocElement);
	if (strstr(DocName, ".bookmap"))
		UsingBookmap = true;
	MainDocName = DCLFile->getFileName();
	MainBaseName = DCLFile->getBaseName();
	rf->SetHandlers(StartElement, EndElement, ProcChar, ProcPI, UMXL);
	while (InitialParse) {
		InitialParse = false;
		CurrDocRef = InitDocRef;
		CurrDepth = 0;
#ifdef DITA
		rf->ParseDoc();  // parses entire doc
#endif
#ifdef UDOC
		rf->ParseMxlDoc();  // parses entire doc
#endif
		if (InitialParse)
			rf->ReopenFile(DocName, CurrFile);
	}
	FormatUsed.empty();
	ProcIntRefs();

	InMap = true;

	while (MapsToGo.next) 
		ParseMap(rf, (char **) MapsToGo.dequeue());

	bool pstart = ParaStarted;
	ParaStarted = false;

	KeydefsDone = true;
	ResolveKeydefs();
	ResolveMapKeyrefs();

	ProcessingKeyrefMaps = true;

	while (KeyrefMapsToGo.next) 
		ParseMap(rf, (char **) KeyrefMapsToGo.dequeue());

	ProcessingKeyrefMaps = false;

	if (PageStarted)
		EndPage();

	ProcessMapConrefs(rf);
	ProcessT2MConrefs(rf);

	if (PageStarted) {
		ParaStarted = pstart;
		pstart = false;
		EndPage();
	}

	InMap = false;
	CurrElems.empty();
	CurrEInfo.empty();
	DocRoots.empty();
	CurrDepth = 0;
	CurrRootDepth = 0;
	CurrElemDepth = 0;
	SkipMapRef = false;
	SkipMapRefDepth = 0;

	while (DocsToGo.next)
		ParseDocument(rf, (char **) DocsToGo.dequeue());

	if (PageNum < 1)
		return;

	if (ProcConrefs) {
		ProcessM2TConrefs(rf);
		while (ConrefT2TCount > ConrefsT2TProcessed)
			ProcessT2TConrefs(rf);
	}
	ConrefsDone = true;

	ProcessAnums();

	ProcessGeneratedLists();

	ProcessLinks();
	ProcessXrefs();

	SetFirstPage();
	for (PageSeqNum = 1; PageSeqNum < LastPageSeq; PageSeqNum++)
		ConnectPage(PageSeqNum + 1, PageSeqNum);

	ResetLangFile(); // use default for rel topics
	ProcessRelatedTopics();
}


void 
XMLrd::ParseMap(XMLrfile *rf, char **pref)
{
	dc_ferr Ferr = fok;
	char *ref = pref ? *pref : NULL;
	ResetLangFile();

	if (!ProcessingConrefs) {
		if (ProcessingKeyrefMaps)
			CurrMapRef = (DCmapelem *) KeyrefMapRefs.dequeue();
		else
			CurrMapRef = (DCmapelem *) MapRefs.dequeue();
	}
	if (!ref
	 || !CurrMapRef)
		return;

	SetMapInfo(ref);
	DocName = DocID = DocElement = NULL;
	ParseHref(ref, &DocName, &DocID, &DocElement);
	rf->ReopenFile(DocName, ref);
	DocRoot = NULL;

	CurrMapInfo->mref = CurrMapRef;
	CurrMapLevel = CurrMapRef->mlevel;
	CurrMapPos = CurrMapRef->mcount;
	if ((Ferr = rf->FErr()) != fok) {
		LogEvent(logwarn, 1, "Error opening file: ", rf->ErrMsg[((int) Ferr) - 1]);
		return;
	}
	CurrBranch = CurrMapRef->branch;
#ifdef UDOC
	//if (UMXL)
		XMLud::CurrConds = (condset *) CurrMapRef->conds;
	//else
#endif
#ifdef DITA
		CurrCondSet = (conditions *) CurrMapRef->conds;
	CurrCondRefLevel = CurrMapRef->condlevel;
#endif
	if (CurrMapRef->cmaptxt)        // group with containing conref
		TextGroup = CurrMapRef->cmaptxt;
	else if (CurrMapRef->elemtext)  // group with containing topicref
		TextGroup = CurrMapRef->elemtext;
	CurrDepth = 0;
#ifdef DITA
	rf->ParseDoc();
#endif
#ifdef UDOC
	rf->ParseMxlDoc();
#endif
	if (CurrMapRef->cmaptxt)
		EndPara(CurrMapRef->cmaptxt);
	if (CurrMapRef->elemtext)  // end of containing map entry
		TextGroup = CurrMapRef->lasttext ?
			CurrMapRef->lasttext : PageTextGroup;  // restore normal group
	FormatUsed.empty();
	ProcIntRefs();
	CurrMapRef = NULL;
}


void 
XMLrd::ParseDocument(XMLrfile *rf, char **pref, bool cnref)
{
	dc_ferr Ferr = fok;
	char *ref = pref ? *pref : NULL;
	ResetLangFile();

	if (!cnref) {
		CurrDocRef = (DCmapelem *) DocRefs.dequeue();
		//if (!CurrDocRef)
		//	CurrDocRef = InitDocRef;
	}
	//if (!ref || !CurrDocRef)
	//	return;

	if (CurrDocRef) {
		CurrDocLevel = CurrDocRef->mlevel;
		CurrBranch = CurrDocRef->branch;
		if (CurrDocRef->role == roleres)
			InResTopic = true;
	}
#ifdef UDOC
	if (CurrDocRef
	 && CurrDocRef->checkType(epPre))
		XMLud::WriteCodedocrefFile(CurrDocRef);
	else if (CurrDocRef
	 && CurrDocRef->checkType(epExt))
		return;  // just copy file later
	else 
#endif
	if (CurrDocRef
	 && (CurrDocRef->scope == scexternal))
		return;
	else if (CurrDocRef
	 && CurrDocRef->gendoc)
		SetGeneratedFile(CurrDocRef);
	else if (!ref)  // marker for nested map, remove
		return;
	else if (CurrDocRef->navtitle == ref)  // topichead
		CreateTitleOnlyTopic();
	else {
		SetDocInfo(ref);
		rf->ReopenFile(DocName, ref);
		DocRoot = NULL;
		CurrDocInfo->mref = CurrDocRef;
		CurrDocRef->docnum = CurrDocNum;
		if ((Ferr = rf->FErr()) != fok) {
			LogEvent(logwarn, 1, "Error opening file: ", rf->ErrMsg[((int) Ferr) - 1]);
			return;
		}
#ifdef UDOC
		XMLud::CurrConds = (condset *) CurrDocRef->conds;
#endif
#ifdef DITA
		CurrCondSet = (conditions *) CurrDocRef->conds;
		CurrCondRefLevel = CurrDocRef->condlevel;
#endif
		ChunkDoc = false;
		ChunkDocSet = false;
		ChunkTopic = false;
		CurrBranchDepth = 0;
		FirstTopic = true;
		CurrDepth = 0;
#ifdef DITA
		rf->ParseDoc();
#endif
#ifdef UDOC
		rf->ParseMxlDoc();
#endif
	}
	if (PageStarted)
		EndPage();
	FormatUsed.empty();
	ProcIntRefs();
	CurrDocRef = NULL;
	InResTopic = false;
}


bool
XMLrd::UseReplacementMap(void)
{
	// return true to cancel current operation
	char *docname = NewName(DocName);
	char *slash = strrchr(docname, '/');
	if (!slash)
		slash = strrchr(docname, '\\');
	char *mapdir = NULL;
	short maplen = 0;
	if (slash) {  // remove path
		maplen = slash - docname;
		mapdir = NewName(docname, maplen);
		docname = slash + 1;
	}
	char *mapname = NewName(docname, strlen(docname) + 8);
	char *per = strrchr(mapname, '.');
	if (per
	 && (strnicmp(per, ".dita", 5)
	  || strnicmp(per, ".xml", 4)))
		*per = '\0';
	strcat(mapname, ".ditamap");
	char *pmapname = mapname;
	if (slash) {
		pmapname = NewName(mapdir, maplen + strlen(mapname) + 2);
		pmapname[maplen] = *slash;
		strcpy(pmapname + maplen + 1, mapname);
	}

	DCrfile *rf = new DCrfile(pmapname);
	if (rf->FErr() == fok) {  // existing map found, use it
		rf->CloseFile();
		delete rf;
		InitialParse = true;
		DocName = CurrFile = pmapname;
		DocElement = DocID = NULL;
		return true;
	}
	rf->CloseFile();
	delete rf;

	if (!GenerateMapIfMissing) {
		DeleteName(mapname);
		DeleteName(docname);
		if (slash) {
			DeleteName(mapdir);
			DeleteName(pmapname);
		}
		return false;
	}
	char *command = NewName("dita2map -i \"", strlen(IniFileName) +
	        strlen(DTDPath) + strlen(mapname) + strlen(docname) + 30);
	strcat(command, IniFileName);
	strcat(command, "\" -d \"");
	strcat(command, DTDPath);
	strcat(command, "\" -o \"");
	strcat(command, mapname);
	strcat(command, "\" \"");
	strcat(command, docname);
	strcat(command, "\"");

	int rval = 0;
	char buf[RefBufLen];
	char *cwd = NULL;
	if (slash) {
		cwd = _getcwd(buf, RefBufLen);
		_chdir(mapdir);
	}
	rval = ::system(command);
	if (slash)
		_chdir(cwd);

	if (rval != 0) {  // error creating map
		DeleteName(mapname);
		DeleteName(docname);
		if (slash) {
			DeleteName(mapdir);
			DeleteName(pmapname);
		}
		return false;
	}
	InitialParse = true;
	DocName = CurrFile = pmapname;
	DocElement = DocID = NULL;
	return true;
}



// DCL generation functions


void
XMLrd::WriteDCL(DCLwfile *wf)
{
	if (PageNum < 1)
		return;

	if (!DCLPartFile)
		WriteDCLFile(wf);
	else {
		long part = 1;
		for (;;) {
			WriteDCLFile(wf, part);
			if ((++part > DCLPartFile)
			 || ((DCLPartFileName = DCLPartFileNames.find(part)) == NULL))
				break;
			wf->ReopenFile(DCLPartFileName);
		}
	}
	
	if (UseLog) {
		LogEvent(loginfo, 1, "Processing completed by drxml.");
		Log->CloseFile();
		CheckLogErrors();
	}
}

// parts begin at part, chapter, appendices, appendix
// if not a bookmap, uses top heading level (heading1)

// if UsePartFiles, at start of part inc DCLPartFile, 
//  set DCLPartPages and DCLPartFileNames.


bool XMLrd::UsePartFiles = false;  // [Options]UsePartFiles
char *XMLrd::PartFileLevel = NULL;        // [Options]PartFileLevel
long XMLrd::PartFileLevelNum = 0;  // numeric level value
long XMLrd::DCLPartFile = 0;       // current part, ends up as last
DCnlist XMLrd::DCLPartFileNames;   // part file name by part
DCnlist XMLrd::DCLPartBaseNames;   // part base name by part
char *XMLrd::DCLPartFileName = NULL;
char *XMLrd::DCLPartBaseName = NULL;
DCwfile *XMLrd::DCLPartListFile = NULL;


void
XMLrd::SetUpPartFiles(DCLwfile *wf)
{
	if (!UsePartFiles)
		return;

	char *bname = wf->getBaseName();
	DCLPartBaseNames.addnew(bname, 1);
	char *fname = wf->getFileName();
	DCLPartFileNames.addnew(fname, 1);
	DCLPartFile = 1;

	if (PartFileLevel)
		PartFileLevelNum = GlossResetElems(PartFileLevel);

	// create lst file
	char *lname = NewName(bname, strlen(bname) + 4);
	strcat(lname, ".lst");
	DCLPartListFile = new DCwfile(lname, ftxt);
	DCLPartListFile->putStr("LIST for ");
	DCLPartListFile->putStr(DCLPartListFile->getFilePath());
	DCLPartListFile->putCr();
	DCLPartListFile->putStr(fname);
	DCLPartListFile->putCr();
}


void 
XMLrd::CheckPartStart(DCelem *elem)
{
	// called at start of topic root
	// abbrev has already determined if start
	DCtopelem *tel = (DCtopelem *) elem;;
	DCmapelem *mel = tel->topic->mref;
	long erst = mel->abreset;
	if ((PartFileLevelNum
	  && (erst <= PartFileLevelNum))
	 || (!PartFileLevelNum
	  && ((IsBookmap    // starting part, chapter, appendices, appendix
	    && (erst == 2) || (erst == 3))
	   || (!IsBookmap   // starting heading1
	    && (erst == 4))))) {
		
		DCLPartFile++;
		if (!elem->elemuid) {
			char *id = elem->id;
			if (!id) {
				id = NewName("part", 15);
				strcat(id, itoa(DCLPartFile, id + 4, 10));
			}
			char *ref = NewName("#", strlen(id) + 2);
			strcat(ref, id);
			char *nref = FixReference(ref);
			elem->elemuid = MakeAnchorName(nref, elem);
			DeleteName(ref);
			DeleteName(nref);
		}
		MakePartFileName(elem->elemuid, PageNum);
	}
}

void
XMLrd::MakePartFileName(char *name, long page)
{
	// called with basename after new number set
	DCLPartBaseNames.addnew(name, DCLPartFile);
	char *ext = MainDocName + strlen(MainBaseName);
	char *fname = NewName(name, strlen(name) + strlen(ext) + 1);
	strcat(fname, ext);
	DCLPartFileNames.addnew(fname, DCLPartFile);

	// write name to .lst file for project
	DCLPartListFile->putStr(fname);
	DCLPartListFile->putCr();

	// set start page
	DCLPartPages.add(page, DCLPartFile);
	DCLPageParts.add(DCLPartFile, page);
}


DCilist XMLrd::DCLPartPages;    // starting page number for each part
DCilist XMLrd::DCLPageParts;    // part number for each starting page


void
XMLrd::WriteDCLFile(DCLwfile *wf, long part)
{
	DCctl *cp = NULL;
	long i = 0;

  // put timestamp at head of dcl file
  time_t tval;
  cp = new DCctl(stamp, 1, 21, UMXL ? 26 : 25, (unl) time(&tval));
  *wf = *cp; 

	// write module version
	cp = new DCctl(ename, 1, 22, 1);
	cp->ex(DRxmlVer);
  *wf = *cp;
 
	cp = new DCctl(imnem, 1, 24, 1, part ? 1 : 0);
  *wf = *cp;

	WriteDCLHead(wf);

	long pg = 0;
	DCgroup *grp = NULL;

	long pt = 0;
	bool started = (part == 1) ? true : false;

  // write out page images from maps and topics
  for (i = 1; i <= LastPageSeq; i++) {
		pg = PageSequence.find(i);
		if (!pg)
			continue;
		if (part) {
			pt = DCLPageParts.find(pg);
			if (pt == part)
				started = true;
			else if (pt    // different part
			 && started)   // ends this one
				break;
			if (!started)
				continue;
		}
		grp = (DCgroup *) PageList.find(pg);
		if (!grp)
			continue;
		grp->write(*wf);
	}

  // write EOF
  *wf = DCctl(ilong, 1, 60, 1, wf->tell());
}


long XMLrd::DCLHeadDiv = 0;


void
XMLrd::WriteDCLHead(DCLwfile *wf)
{
	DCctl *cp = NULL;
	long i = 0;

	if (DCLHeadDiv) {
		wf->SelectDiv(0);
		wf->WriteDiv(DCLHeadDiv);
		wf->SelectDiv(0);
		return;
	}

	DCLHeadDiv = wf->NewDiv();

  // set up defaults group
  DCgroup *gp = new DCgroup(DCctl(group, 1, 40, 1));

	cp = new DCctl(imnem, 1, 10, 3);  // page units points (hundredths)
  gp->add(*cp);

  // identify default char set for names, etc., as UTF-8
  cp = new DCctl(ilong, 2, 14, 128, 8);
  gp->add(*cp);

  // identify user for user-defined ctls, "UDOC" or "DITA"
#ifdef UDOC
  cp = new DCctl(ibyte, 1, 20, 200, 0x55444F43);
#endif
#ifdef DITA
  cp = new DCctl(ibyte, 1, 20, 200, 0x44495441);
#endif
  gp->add(*cp);

  // write out defaults group
  gp->write(*wf);


  // set up resources group
  gp = new DCgroup(DCctl(group, 1, 40, 2));

#ifdef UDOC
	if (XMLud::OutputKeyDefs)
		gp->add(* (DCctl *) XMLud::OutputKeyDefs);
#endif

	WriteFootnoteStreamDefs(gp, 1, false);
	WriteFootnoteStreamDefs(gp, 2, true);

	if (NumStreamIDs > 2)
		WriteFormatStreamDefs(gp);

	for (i = 1; i <= CurrVar; i++)
		WriteVariableDef(gp, i);

	if (XrefFormDefCount) {
		for (i = 1; i <= XrefFormDefCount; i++) {
			DCgroup *xgp = new DCgroup(DCctl(group, 2, 95, (uns) i));
			gp->add(*xgp);
			cp = new DCctl(etext, 2, 96, 1);
			if ((XrefForm = XrefFormNameDefs.find(i)) != NULL)
				cp->ex(XrefForm, strlen(XrefForm));
			xgp->add(*cp);
			//cp = new DCctl(group, 2, 96, 3);
			//xgp->add(*cp);
			DCgroup *ngp = new DCgroup(DCctl(group, 2, 96, 3));
			xgp->add(*ngp);
			WriteXrefFormatDef(ngp, XrefFormDefs.find(i), XrefForm);
			//cp = new DCctl(endgroup, 2, 96, 3);
			//xgp->add(*cp);
		}
	}
	else if (XrefCount) {  // write one xref format def
		DCgroup *xgp = new DCgroup(DCctl(group, 2, 95, 1));
		gp->add(*xgp);
		cp = new DCctl(etext, 2, 96, 1);
		cp->ex(XrefTextFormat, strlen(XrefTextFormat));
		xgp->add(*cp);
		//cp = new DCctl(group, 2, 96, 3);
		//xgp->add(*cp);
		DCgroup *ngp = new DCgroup(DCctl(group, 2, 96, 3));
		xgp->add(*ngp);
		cp = new DCctl(imnem, 2, 97, 35, 4);
		ngp->add(*cp);
		//cp = new DCctl(endgroup, 2, 96, 3);
		//xgp->add(*cp);
	}

	if (CondFlagCount) {
		for (i = 1; i <= CondFlagCount; i++) {
#ifdef UDOC
			//if (UMXL)
				XMLud::ProcCondFlagDef(gp, i);
			//else
#endif
#ifdef DITA
				ProcCondFlagDef(gp, i);
#endif
		}
	}

	long fnum = 0;
	DCilist formchange;
	char *fname = NULL;

	for (i = 1; i <= BlockFormCount; i++) {
		//cp = new DCctl(group, 2, 80, (uns) i);
		//gp->add(*cp);
		DCgroup *ngp = new DCgroup(DCctl(group, 2, 80, (uns) i));
		gp->add(*ngp);
		cp = new DCctl(ename, 2, 81, 1);
		cp->ex(fname = (char *) BlockFormatList.dequeue(&fnum));
		formchange.add(i, fnum);
		ngp->add(*cp);
		cp = new DCctl(imnem, 2, 81, 2, 1);
		ngp->add(*cp);
		if (FmtFile
		 && ((fname = FmtFile->GetNumFmtName(fname)) != NULL)
		 && NumFormatNames.getitem(fname))
			WriteFormatNumPrefix(fname, ngp);
		//cp = new DCctl(endgroup, 2, 80, (uns) i);
		//gp->add(*cp);
	}
	for (i = 1; i <= BlockFormUseCount; i++) {
		cp = BlockFormats.dequeue();
		cp->siz(formchange.find(cp->siz()));
	}
	formchange.empty();

	for (i = BlockFormCount + 1;
	     i <= (InlineFormCount + BlockFormCount); i++) {
		//cp = new DCctl(group, 2, 80, (uns) i);
		//gp->add(*cp);
		DCgroup *ngp = new DCgroup(DCctl(group, 2, 80, (uns) i));
		gp->add(*ngp);
		cp = new DCctl(ename, 2, 81, 1);
		cp->ex(InlineFormatList.dequeue(&fnum));
		formchange.add(i, fnum);
		ngp->add(*cp);
		cp = new DCctl(imnem, 2, 81, 2, 2);
		ngp->add(*cp);
		//cp = new DCctl(endgroup, 2, 80, (uns) i);
		//gp->add(*cp);
	}
	for (i = 1; i <= InlineFormUseCount; i++) {
		cp = InlineFormats.dequeue();
		cp->siz(formchange.find(cp->siz()));
	}
	formchange.empty();

	for (i = InlineFormCount + BlockFormCount + 1;
		   i <= (InlineFormCount + BlockFormCount + TableFormCount); i++) {
		//cp = new DCctl(group, 2, 80, (uns) i);
		//gp->add(*cp);
		DCgroup *ngp = new DCgroup(DCctl(group, 2, 80, (uns) i));
		gp->add(*ngp);
		cp = new DCctl(ename, 2, 81, 1);
		cp->ex(TableFormatList.dequeue(&fnum));
		formchange.add(i, fnum);
		ngp->add(*cp);
		cp = new DCctl(imnem, 2, 81, 2, 3);
		ngp->add(*cp);
		//cp = new DCctl(endgroup, 2, 80, (uns) i);
		//gp->add(*cp);
	}
	for (i = 1; i <= TableFormUseCount; i++) {
		cp = TableFormats.dequeue();
		cp->siz(formchange.find(cp->siz()));
	}
	formchange.empty();

  // write out resources group
  gp->write(*wf);

	wf->SelectDiv(0);
	wf->WriteDiv(DCLHeadDiv);
	wf->SelectDiv(0);
}


void
XMLrd::CheckLogErrors(void)
{
	if (Log->ErrWarn) {
		if (PMFlag)   // running from plugin
			ReleaseSemaphore(PMFlag, 1L, NULL);  // puts semaphore in signalled state
		else if (ShowLog) { // display cop of log file
			char *tlog = NewName(LogFileName, strlen(LogFileName) + 4);
			char *dot = strrchr(tlog, '.');
			if (dot)
				strcpy(dot, "xml.tmp");
			char *cmd = NewName("copy /Y ", (strlen(LogFileName) * 2) + 14);
			strcat(cmd, LogFileName);
			strcat(cmd, " ");
			strcat(cmd, tlog);
			DCauto::RunDOSCommand(cmd, true, false, false);
			DeleteName(cmd);
			cmd = NewName(EditorFileName, strlen(EditorFileName) + 
																					strlen(tlog) + 1);
			strcat(cmd, " ");
			strcat(cmd, tlog);
			DeleteName(tlog);
			DCauto::RunDOSCommand(cmd, true, false, false);
			DeleteName(cmd);
		}
	}
}



// utilities


char *XMLrd::UnitAbbrs[] = {
 "px", "in",
 "pc", "pt",
 "cm", "mm",
 "em", "ex"
};


long
XMLrd::GetNumericAttr(const char *aval)
{
	const char *uptr = aval;
	double uval = atof(aval);

	while (*uptr
	 && (isdigit((int) *uptr)
	  || (*uptr == '.')))
		uptr++;
	if (*uptr == '\0')  // no units = points for 1.0, px for 1.1
		return ConvertVal(uval, (XMLrfile::DITAver && !UMXL) ? px : mu);
	for (unc i = 0; i < mu; i++) {
		if (!strncmp(uptr, UnitAbbrs[i], 2))
			break;
	}
	if (i == mu)
		LogEvent(logwarn, 1, "Unknown size unit ", uptr);
	return ConvertVal(uval, (enum units) i);
}


long
XMLrd::ConvertVal(double val, units unit)
{
	// convert from specified unit to .01 pt units
	switch (unit) {
		case px:  // pixels, assume 96dpi
			val *= 75;
			break;

		case in:  // inches, 72 pts
			val *= 7200;
			break;

		case pc:  // picas, 6 pts
			val *= 600;
			break;
		case pt:  // pts themselves
			val *= 100;
			break;

		case cm:  // centimeters, 2.54/in  
			val = (val * 720000) / 254;
			break;
		case mm:  // millimeters, 25.4/in
			val = (val * 72000) / 254;
			break;

		case em:  // relative unit, assume 10pt base
			val *= 1000;
			break;
		case ex:  // relative unit, assume half of em
			val *= 2000;
			break;

		case mu:  // unknown
		default:  // assume points for dita 1.0
			val *= 100;
			break;
	}
	return (long) floor(val);
}



void 
XMLrd::ProcAttrs(const char **attp, attrp *alist)
{
	long i = 0;

	if (!attp)
		return;

	while (*attp) {
		for (i = 0; alist[i].name != NULL; i++) {
			if (!strcmp(*attp, alist[i].name)) {
				*(alist[i].var) = *(attp + 1);
				break;
			}
		}
		attp += 2;
	}
}

#if 0
char *
XMLrd::GetAttr(const char **attp, const char *name)
{
	char *val = NULL;

	while (*attp) {
		if (!strcmp(*attp, name)) {
			val = NewName(*(attp + 1));
			break;
		}
		attp += 2;
	}

	return val;
}
#endif

// end of drxml.cpp

