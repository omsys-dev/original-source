 
// drmif.cc is the code for dcl FrameMaker MIF reading, JHG
// Copyright (c) 1993-2008 by Omni Systems, Inc.  All rights reserved.

#include "dcappl.h"
#include "dcctlg.h"
#include "dcfili.h"
#include "dcfilp.h"
#include "dcini.h"
#include "dcmacr.h"
#include "dcauto.h"
#include "drmif.h"
#include "drmiffl.h"
#include <windows.h>



// start of code

char *MIFrd::DRmifVer = { "m210a" };

swlist MIFrd::SwitchList[] = {
  { "F", NULL, MIFrd::SetFormat, true },
  { "f", NULL, MIFrd::SetFormat, true },
  { NULL, NULL }
};
bool MIFrd::FormatSet = false;

short MIFrd::Verbose;
char *MIFrd::CurrFile = NULL;
char *MIFrd::IniFileName = NULL;
char *MIFrd::OldIniName = NULL;
char MIFrd::OldFormType = NULL;
DCirfile *MIFrd::IniFile = NULL;
MIFrfile *MIFrd::MIFFile = NULL;
DCLwfile *MIFrd::DCLFile = NULL;
bool MIFrd::StartingAuto = false;
bool MIFrd::BookAuto = false;
outtyp OutputType = otnone;



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

  DCapp app(dr, ".mif");

  app.Init(argc, argv);

  if (!app.Proc(MIFrd::SwitchList)) {
    fprintf(stderr, "usage: %s [-v] [-f[g|h|j|n|w]] [-o dcl_file] mif_file\n", argv[0]);
    exit(dc_err_arg);
  }

  MIFrd::Verbose = app.verbose();

  //MIFrfile rf(app.rfile());
  //if (rf.FErr() != fok)
  //  rf.Err();

	DCirfile *inif = NULL;	// in current directory

	if (MIFrd::StartingAuto) {
		if (MIFrd::IniFileName
		 && ((inif = new DCirfile(MIFrd::IniFileName)) != NULL)
		 && (inif->FErr() == fok)
		 && (inif->Ferr == fok))
			MIFrd::IniFile = inif;  // make accessible everywhere
		else if (MIFrd::OldIniName
		 && (!MIFrd::IniFileName
			|| stricmp(MIFrd::IniFileName, MIFrd::OldIniName))
		 && ((inif = new DCirfile(MIFrd::OldIniName)) != NULL)
		 && (inif->FErr() == fok))
			MIFrd::IniFile = inif;
		else if (((inif = new DCirfile("mif2rtf.ini")) != NULL)
		 && (inif->FErr() == fok))
			MIFrd::IniFile = inif;
		else if (((inif = new DCirfile("mif2htm.ini")) != NULL)
		 && (inif->FErr() == fok))
			MIFrd::IniFile = inif;
		else
			return dc_err_ini;
		MIFrd::InitCurrProps(NULL); // set up vars per ini file
		inif->CloseFile();
		return dc_ok;
	}

  MIFrfile rf(MIFrd::CurrFile = app.rfile());
  if (rf.FErr() != fok)
    rf.Err();
	else
		MIFrd::MIFFile = &rf;

  //DCLwfile wf(app.wfile(), app.wtype());
  //if (wf.FErr() != fok)
  //  wf.Err();

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

  DCLwfile wf((slash ? (slash + 1) : wfname), app.wtype());
  if (wf.FErr() != fok)
    wf.Err();
	else
		MIFrd::DCLFile = &wf;

	// look in current directory for correct ini file
#if 0
	//DCirfile inif((MIFrd::OutType == 'N') ? "mif2htm.ini" : "mif2rtf.ini", &rf);
	DCirfile inif(MIFrd::IniFileName, &rf);
	if (inif.FErr() == fok)
		MIFrd::IniFile = &inif;  // make accessible everywhere
	else
		return dc_err_ini;
#endif

	if (MIFrd::IniFileName
	 && ((inif = new DCirfile(MIFrd::IniFileName, &wf)) != NULL)
	 && (inif->FErr() == fok)
	 && (inif->Ferr == fok))
		MIFrd::IniFile = inif;  // make accessible everywhere
	else if (MIFrd::IniFileName
	 && ((inif = new DCirfile(MIFrd::IniFileName + 1, &wf)) != NULL)
	 && (inif->FErr() == fok)
	 && (inif->Ferr == fok)) {
		MIFrd::IniFileName++;
		MIFrd::IniFile = inif;  // make accessible everywhere
	}
	else if (MIFrd::OldIniName
	 && (!MIFrd::IniFileName
	  || stricmp(MIFrd::IniFileName, MIFrd::OldIniName))
	 && ((inif = new DCirfile(MIFrd::OldIniName, &wf)) != NULL)
	 && (inif->FErr() == fok))
		MIFrd::IniFile = inif;
	else if (((inif = new DCirfile("mif2rtf.ini", &wf)) != NULL)
	 && (inif->FErr() == fok))
		MIFrd::IniFile = inif;
	else if (((inif = new DCirfile("mif2htm.ini", &wf)) != NULL)
	 && (inif->FErr() == fok))
		MIFrd::IniFile = inif;
	else
		return dc_err_ini;

	MIFrd::InitCurrProps(&rf); // set up vars per ini file

  MIFrd::ParseMIF(&rf);   // parse entire mif file
  MIFrd::WriteDCL(&wf);   // put out the dcl resources and page images

	inif->CloseFile();	// no need to check ini file for errors
  rf.CloseFile();
  wf.CloseFile();
  rf.Err();      // these two just return if no err
  wf.Err();      // otherwise they exit with the err (and a message)
  return dc_ok;  // so if we get here, it worked
}


void
MIFrd::SetFormat(char *fmt)
{
	//bool newform = false;

	// fmt can be Help, Word, Net, Java, MS HH, GenXML
	FormatSet = true;  // overrules .ini
	OldFormType = toupper(*fmt);

	char ch = '\0';
	IniFileName = DCirfile::GetIniName("_m2", fmt, &OldFormType,
	                                    &ch, &OldIniName);
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
	IniFileName = NewName("m2", 20);
	char ch = toupper(*fmt);
	if (strlen(fmt) > 2) {
		newform = true;
		if (!strnicmp(fmt, "htmlhelp", 8))
			ch = 'M', strcat(IniFileName, "htmlhelp");
		else if (!strnicmp(fmt, "javahelp", 8))
			ch = 'J', strcat(IniFileName, "javahelp");
		else if (!strnicmp(fmt, "oraclehelp", 10))
			ch = 'O', strcat(IniFileName, "oraclehelp");
		else if (!strnicmp(fmt, "eclipse", 7))
			ch = 'E', strcat(IniFileName, "eclipse");
		else if (!strnicmp(fmt, "html", 4))
			ch = 'N', strcat(IniFileName, "html");
		else if (!strnicmp(fmt, "xhtml", 5))
			ch = 'X', strcat(IniFileName, "xhtml");
		else if (!strnicmp(fmt, "xml", 3))
			ch = 'G', strcat(IniFileName, "xml");
		else if (!strnicmp(fmt, "docbook", 7))
			ch = 'B', strcat(IniFileName, "docbook");
		else if (!strnicmp(fmt, "dita", 4))
			ch = 'D', strcat(IniFileName, "dita");
		else if (!strnicmp(fmt, "omnihelp", 8))
			ch = 'C', strcat(IniFileName, "omnihelp");
		else if (!strnicmp(fmt, "rtf", 3))
			ch = '8', strcat(IniFileName, "rtf");
		else if (!strnicmp(fmt, "winhelp", 7))
			ch = 'H', strcat(IniFileName, "winhelp");
		else if (!strnicmp(fmt, "dcl", 3))
			ch = 'A', strcat(IniFileName, "dcl");
		else
			ch = 'Z', strcat(IniFileName, "html");
	}
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
			break;
		case 'H':  // WinHelp
		case '3':  // WinHelp3
		case '4':  // WinHelp4
			OutType = 'H';
			HelpType = 'H';
			Mark11 = 'M';
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
			if (!strcmp(OldIniName, "mif2htm.ini")) {
			//if (DCrfile::FileExists("m2dcl.ini")
			// || DCrfile::FileExists("mif2htm.ini")) {
				//if (strlen(IniFileName) == 2)
				//	strcat(IniFileName, "dcl");
				OutType = 'N';
				FormType = 'H';
			}
			else {
				//OutType = DCrfile::FileExists("mif2htm.ini") ? 'N' : 'S';
				//FormType = DCrfile::FileExists("mif2htm.ini") ? 'H' : 'R';
				OutType = 'S';
				FormType = 'R';
			}
			HelpType = 'S';
			break;
#endif

		default:  // unknown
			FormatSet = false;
			FormType = 'N';
			break;
	}

#if 0
	if (strlen(IniFileName) == 2)  // old format identifier
		strcpy(IniFileName, (FormType == 'R') ? "mif2rtf" : "mif2htm");

	strcat(IniFileName, ".ini");
#endif
}



// process ini-file options

bool MIFrd::DoAutomation = false;
char MIFrd::OutType = 'S'; // Standard, Help, or Net
char MIFrd::HelpType = 'S'; // Standard, Help, MS, Java
char MIFrd::FormType = 'N'; // None, Rtf, Html
char MIFrd::ObjectIDs = 'A'; // None or All
long MIFrd::CurrObjectID = 0;
char MIFrd::XrefType = 'N'; // Full or Numeric
char MIFrd::Mark11 = 'N'; // Full, Midtopic, or None
bool MIFrd::RemoveWordTocMarkers = true;
char MIFrd::ImpGraph = 'N'; // Normal, Retain or Export
bool MIFrd::ExFiles = false;
char MIFrd::EpsiUsage = 'P'; // Preview (only), EPS (only), or Retain (all)
bool MIFrd::KeepEpsi = false;
bool MIFrd::EpsiPreview = false;

uns MIFrd::DirectiveMark = 3;   // default to comment
uns MIFrd::ALinkMarkerType = 0; // default to none
uns MIFrd::ExtraIXMarkerType = 0; // default to none
char *MIFrd::ExtraIXMarkerName = NULL;
char *MIFrd::ExtraIXPrefix = NULL;

bool MIFrd::CaselessMatch = true;
bool MIFrd::SpacelessMatch = true;
bool MIFrd::WildcardMatch = true;
char *MIFrd::IDFileName = NULL;
char *MIFrd::PrjFileName = NULL;
char MIFrd::GrExFormat = NULL;

bool MIFrd::UseLog = true;
bool MIFrd::ShowLog = false;
char *MIFrd::LogFileName = "_m2g_log.txt";
char *MIFrd::HistoryFileName = "_m2g_history.txt";
char *MIFrd::EditorFileName = "notepad.exe";
DClog *MIFrd::Log = NULL;
unc MIFrd::LogLevels[logmax] = { 0, 1, 1, 1, 1, 0 };
bool MIFrd::LogIniChains = false;
char *MIFrd::MacroFile = "m2hmacro.ini";
HANDLE PMFlag = NULL;


bool
MIFrd::InitCurrProps(MIFrfile *rf)
{
	if (!IniFile)
		return false;

	// set IniFile to point to top of config chain
	IniFile = IniFile->SetConfigTemplate("Templates", "Configs");

	DCini::InitIniModule(IniFile, MIFIniSects);

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
		if (IniFile->Find("NoSymMap"))
			NoSymMap = IniFile->BoolRight();
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
		XrefType = 'N'; // change default
		ObjectIDs = 'A';
	}

	DCini::ProcAllSects();

	if (UseLog) {
		PMFlag = OpenSemaphore(SEMAPHORE_ALL_ACCESS, FALSE, "m2gpl");

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
		Log = new DClog(LogFileName, "drmif", DRmifVer, LogLevels);
		if (Log->FErr() == fok) {
			if (rf)
				Log->logEvent(loginfo, 1, "Opened file \"", rf->getFilePath(),
					"\" from FM ", FMversion);
		}
		else
			UseLog = false;
		MIFrfile::LogEventFunc = LogEvent;
		LogFileName = Log->getFilePath();
	}
	DCirfile::LogEventFunc = (logfunc) LogEvent;
	DCirfile::LogIniChain = LogIniChains;
	DCauto::LogEventFunc = (logfunc) LogEvent;
	DCauto::LogFileName = LogFileName;


	// set up automation

	DCauto::LogEventFunc = (logfunc) LogEvent;
	if (StartingAuto)
		DoAutomation = DCauto::SetUpAutomation(OutputType, IniFile, NULL, NULL, BookAuto);

	if (DCauto::UseExistingDCL) {
		if (UseLog) {
			Log->logEvent(loginfo, 1, "UseExistingDCL set, skipping MIF file ");
			Log->CloseFile();
		}
		CheckLogErrors();
		return false;
	}

	DCauto::DeleteDCL();

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
		DCauto::WrapShipPreProc(BookAuto);

	if (StartingAuto) {
		if (UseLog) {
			Log->logEvent(loginfo, 1, "Automation processing completed by drmif.");
			Log->CloseFile();
		}
		CheckLogErrors();
		return false;
	}

	if (DCauto::OnlyAuto) {  // no processing at all
		if (UseLog) {
			Log->logEvent(loginfo, 1, "OnlyAuto set, skipping MIF file ");
			Log->CloseFile();
		}
		CheckLogErrors();
		return false;
	}

	if (IniFile->Section("AttributeMarkers")) {
		while (IniFile->NextItem())
			AttrMarkers.add(IniFile->StrLeft(), (long) IniFile->StrRight());
	}

	if (OutType == 'H')      // for WinHelp
		DCini::ProcIniSect("HelpOptions");
	else if (OutType == 'S') // for Word
		DCini::ProcIniSect("WordOptions");
	else if (OutType == 'N') // for HTML/XML
		DCini::ProcIniSect("HTMLOptions");

	if (FixGraphicNames) {
		if ((GraphicRenBatFile = fopen(GraphicRenameBat, "a+t")) == NULL)
			FixGraphicNames = false;
	}

	if (UseGrFileID) {
		if (UseLocalFileID
		 && IniFile->Section("FileIDs")
		 && IniFile->Find(rf->getBaseName())) {
			GrFileID = IniFile->StrRight();
			IDFileName = NULL;
			LogEvent(loginfo, 1, "Using local file IDs ");
		}
		else if (!UseLocalFileID
		 && (IDFileName != NULL)) {
			GrFileID = GetFileID(rf->getBaseName());
#if 0
			bool tryini = true;
			DCirfile id(IDFileName);
			if (id.FErr() != fok) {
				LogEvent(logwarn, 1, "Cannot find ID file ", IDFileName);
			}
			else if (!(id.Section("FileIDs"))) {
				LogEvent(logwarn, 1, "ID file ", IDFileName, " missing IDs");
				id.CloseFile();
			}
			else if (!(id.Find(rf->getBaseName()))) {
				LogEvent(logwarn, 1, "ID missing from file ", IDFileName);
				id.CloseFile();
			}
			else {
				GrFileID = NewName(id.StrRight());
				tryini = false;
				id.CloseFile();
			}
			if (tryini) {
				if (IniFile->Section("FileIDs")
				 && IniFile->Find(rf->getBaseName()))
					GrFileID = IniFile->StrRight();
				else
					UseGrFileID = false;
			}
#endif
		}
	}

	LogEvent(loginfo, 1, "File ID is ", GrFileID);

	GenFileParaFmts.init(GenFileParaFmtsInit);

	// this section is deprecated
	if (IniFile->Section(rf->getBaseName())) {  // read file-specific info
		if (IniFile->Find("EpsiUsage"))
			EpsiUsage = IniFile->UCRight();
	}

	if ((EpsiUsage == 'R')
	 || (EpsiUsage == 'E'))
		KeepEpsi = true;
	if ((EpsiUsage == 'R')
	 || (EpsiUsage == 'P'))
		EpsiPreview = true;

	if (!EqnSuffix
	 && (MathForm == 3))
		EqnSuffix = GrSuffix;

	if ((MathForm == 2)  // scan .htm and get .gifs for equations
	 || UseAllGifs)
		ScanHTMath(rf);

#ifdef SAMPLE
	SetUpSample();
#endif

	return true;
}


void
MIFrd::LogEvent(logtype lt, unc sev, const char *desc,
 const char *d2, const char *d3, const char *d4, const char *d5)
{
	if (!UseLog)
		return;

	Log->logEvent(lt, sev, desc, d2, d3, d4, d5);
}




// list of all interpreted MIF statements

mif MIFrd::mifpile[] = {
  { "MIFFile",           numval,     stamp, 1, 21,  6, new_file },
  { "Units",             l_units,    imnem, 1, 10,  0, unit_set },

  { "ID",                numval,     ilong, 0,  0,  0, mif_id },
  { "BRect",             brectval,   elong, 0,  0,  0, mif_rect },
  { "ShapeRect",         srectval,   elong, 0,  0,  0, mif_rect },

  { "ColorCatalog",      newgroup,   group, 0, 20,  0, pseudogroup },
  { "Color",             newgroup,   ibyte, 4, 80,  0, color_def },
  { "ColorTag",          nameval,    ename, 0,  0,  0, color_tag },
  { "ColorCyan",         colorval,   ilong, 0,  0,  1, color_val },
  { "ColorMagenta",      colorval,   ilong, 0,  0,  2, color_val },
  { "ColorYellow",       colorval,   ilong, 0,  0,  3, color_val },
  { "ColorBlack",        colorval,   ilong, 0,  0,  4, color_val },

  { "Unique",            numval,     ilong, 2,112,  2, mif_uniq }
};

size_t MIFrd::mifpile_size = (sizeof(mifpile) / sizeof(mif));

size_t MIFrd::miflist_size;
mif **MIFrd::miflist;



// mif enumerations used in above statements

// add starting enum used below for each to dc_mtyp (in dcrmif.h)
// between the listword and list_end enumerations

char *MIFrd::wordpile[] = {
  (char *) (int) l_units, "Uin", "Ucm", "Umm", "Upica", "Upt", "Udd", "Ucc", NULL,
  NULL
};

size_t MIFrd::wordlist_size = (list_end - listword);
cpp *MIFrd::wordlist;

char *MIFrd::YesNoList[] = {
  "No", "no", "Yes", "yes", NULL
};



// globals to hold parsing info

unc MIFrd::MIFversion;
char *MIFrd::FMversion;

char *MIFrd::MIFstring;
mifunit MIFrd::MIFunit;
long MIFrd::MIFval;
point MIFrd::MIFpt;
rect MIFrd::MIFrect;
bool MIFrd::ConvertToEms;

DCblock *MIFrd::MIFraster; // holds last raster read
DCblock *MIFrd::MIFvector; // holds last vector read
DCblock *MIFrd::MIFepsi;   // holds last epsi object read
rastyp MIFrd::MIFrastype;  // type of raster
vectyp MIFrd::MIFvectype;  // type of vector
long MIFrd::MIFlines;      // line count in epsi object 

bool MIFrd::MultipleOLE = false;  // for OLE, save all WMFs
DClist MIFrd::OLEimages;
long MIFrd::OLEimageCount = 0;


// global comparison func for qsort and bsearch


int
mifcmp(const void *mp1, const void *mp2)
{
  char *p1 = (*((mif **) mp1))->name;
  char *p2 = (*((mif **) mp2))->name;

  return strcmp(p1, p2);
}



// MIF parsing functions


void
MIFrd::ParseMIF(MIFrfile *rf)
{
  MIFversion = rf->MIFver; // 1 (not '1') for 1.x, 2 for 2.x, etc.
	FMversion = rf->FMver;   // Frame version string, such as 8.0p273

  // set up miflist for binary search
  miflist_size = mifpile_size + miftx_size + mifan_size + miftb_size
                 + mifgr_size;
  miflist = new mif *[miflist_size];
  mif **mifptr = miflist;
  for (size_t i = 0; i < mifpile_size; i++, mifptr++)
    *mifptr = &mifpile[i];
  for (i = 0; i < miftx_size; i++, mifptr++)
    *mifptr = &miftxpile[i];
  for (i = 0; i < mifan_size; i++, mifptr++)
    *mifptr = &mifanpile[i];
  for (i = 0; i < miftb_size; i++, mifptr++)
    *mifptr = &miftbpile[i];
  for (i = 0; i < mifgr_size; i++, mifptr++)
    *mifptr = &mifgrpile[i];
  qsort((void *) miflist, miflist_size, sizeof(mif *), &mifcmp);

  // set up wordlist for lookup of mif enums
  wordlist = new cpp[wordlist_size];
  for (i = 0; i < wordlist_size; i++)
    wordlist[i] = NULL;
  int wordindex = 0;
  for (cpp cpptr = wordpile; *cpptr != NULL; cpptr++) {
    wordindex = ((int) *cpptr) - ((int) listword);
    wordlist[wordindex] = ++cpptr;
    while (*cpptr != NULL)
      cpptr++;
  }
  for (cpptr = wordtxpile; *cpptr != NULL; cpptr++) {
    wordindex = ((int) *cpptr) - ((int) listword);
    wordlist[wordindex] = ++cpptr;
    while (*cpptr != NULL)
      cpptr++;
  }
  for (cpptr = wordanpile; *cpptr != NULL; cpptr++) {
    wordindex = ((int) *cpptr) - ((int) listword);
    wordlist[wordindex] = ++cpptr;
    while (*cpptr != NULL)
      cpptr++;
  }
  for (cpptr = wordtbpile; *cpptr != NULL; cpptr++) {
    wordindex = ((int) *cpptr) - ((int) listword);
    wordlist[wordindex] = ++cpptr;
    while (*cpptr != NULL)
      cpptr++;
  }
  for (cpptr = wordgrpile; *cpptr != NULL; cpptr++) {
    wordindex = ((int) *cpptr) - ((int) listword);
    wordlist[wordindex] = ++cpptr;
    while (*cpptr != NULL)
      cpptr++;
  }

  // set up pseudogroups for which no mif statement identifies correct time
  MakePseudo(psg_page);
  MakePseudo(psg_tflow);
  MakePseudo(psg_anum);
  MakePseudo(psg_fnote);
	//if (ExFiles)
		MakePseudo(psg_exfile);
  MakePseudo(psg_base);  // make base-level pseudogroup last

  // set up text buffer
  TextObjLit = new DCctl(etext, 2, 10, 1);
  TextBuf = new DCblock(TextBufMax);

  // parse the entire mif file
  mif key;
  mif *kp = &key;
  mif **kpp = &kp;
  mif *mp;
  mif **mpp;
  int ch;
	char *ynstr = NULL;

  while (!rf->FErr() && !rf->eof()) {
    rf->skipWhite();
    switch (ch = rf->getChar()) {
      case '<':  // start of statement
        break;
      case '>':  // end of statement group
        if (EndProc[MIFlevel]) {
          (*EndProc[MIFlevel])();
          EndProc[MIFlevel] = NULL;
        }
        if (MIFlevel)
          MIFlevel--;
        continue;
      case '=':  // inset, collect automatically
        rf->ungetChar(ch);
        rf->getImport();
        MIFraster = rf->MIFraster;
        MIFvector = rf->MIFvector;
        MIFepsi = rf->MIFepsi;
        MIFrastype = rf->MIFrastype;
        MIFvectype = rf->MIFvectype;
        MIFlines = rf->MIFlines;
        continue;
      case '#':  // comment
      default:   // define, include, and errors
        rf->skipLine();
        continue;
    }
    key.name = rf->getWord();
    mpp = (mif **) bsearch((void *) kpp, (void *) miflist,
                                        miflist_size, sizeof(mif *), &mifcmp);
    if (mpp == NULL) {            // unknown statement, ignore it
      rf->skipStatement();
      continue;
    }
    mp = *mpp;

    switch (mp->mtype) {      // extract arg if any
      case empty:
        rf->skipStatement();
        break;
      case noteval:  // may be number or group
        ch = rf->getChar();
        rf->ungetChar(ch);
        if (ch != '<') {
          MIFval = rf->getALong();
          rf->skipStatement();
          break;
        }
        // fall through if group
      case newgroup:
        MIFlevel++;
        GroupStack[MIFlevel] = GroupStack[MIFlevel - 1];
        // stack item is changed by spec function if also new dcl group
        break;
      case stringval:
        MIFstring = rf->getString();
#ifdef SAMPLE
				SampConvert(MIFstring);
#endif
        rf->skipStatement();
        break;
			case fileval:
        MIFstring = rf->getString();
				ConvertFileString();
				ConvertStringName(false);
				if (MIFstring
				 && (MIFstring[0] == '.')
				 && (MIFstring[1] == '/'))
					memmove(MIFstring, MIFstring + 2, strlen(MIFstring) - 1);
        rf->skipStatement();
        break;
      case vdefval:
        MIFstring = rf->getString();
				ConvertStringName(true);
        rf->skipStatement();
        break;
      case nameval:
        MIFstring = rf->getString();
				ConvertStringName(false);
        rf->skipStatement();
        break;
      case unameval:
        MIFstring = rf->getString();
        rf->skipStatement();
        break;
      case numval:
        MIFval = rf->getALong();
        rf->skipStatement();
        break;
      case fillval:
        MIFval = rf->getALong();
        MIFval = (MIFval == 15) ? 0 : MIFval + 1;
        rf->skipStatement();
        break;
      case separ:
        MIFval = rf->getALong() + 1;
        rf->skipStatement();
        break;
			case colorval:
				ColorVal = rf->getADouble();
        rf->skipStatement();
        break;
			case emval:
        MIFval = rf->getVal();
        MIFunit = rf->getUnit();
        rf->skipStatement();
				ConvertToEms = false;
				if (MIFunit == percent)
					break;
				if (MIFunit == pt)
					ConvertToEms = true;
				// otherwise unitless, handled as percent
        break;
      case sizeval:
        MIFval = rf->getVal();
        MIFunit = rf->getUnit();
        rf->skipStatement();
				if (MIFunit > umax)
					break;
        if (Upage != M2Dunits[MIFunit])
          ConvUnits(page_u);
        break;
      case fontval:
        MIFval = rf->getVal();
        MIFunit = rf->getUnit();
        rf->skipStatement();
				if (MIFunit > umax)
					break;
        if (Ufont != M2Dunits[MIFunit])
          ConvUnits(font_u);
        break;
      case leadval:
        MIFval = rf->getVal();
        MIFunit = rf->getUnit();
        rf->skipStatement();
				if (MIFunit > umax)
					break;
        if (Ulead != M2Dunits[MIFunit])
          ConvUnits(lead_u);
        break;
      case glineval:
        MIFval = rf->getVal();
        MIFunit = rf->getUnit();
        rf->skipStatement();
				if (MIFunit > umax)
					break;
        if (Ugline != M2Dunits[MIFunit])
          ConvUnits(gline_u);
        break;
      case ptval:
        MIFpt.x = rf->getVal();
        MIFpt.y = rf->getVal();
        MIFunit = rf->getUnit();
        rf->skipStatement();
        if (Upage != M2Dunits[MIFunit])
          ConvPt(page_u);
        break;
      case arectval:
				goto rectval;
      case brectval:  // use BRect only if no ShapeRect
				if (!ShapeRect)
					goto rectval;
        rf->skipStatement();
				break;
      case srectval:  // always use ShapeRect if present
				ShapeRect = true;
				goto rectval;
			rectval:
        MIFrect.l = rf->getVal();
        MIFrect.t = rf->getVal();
        MIFrect.w = rf->getVal();
        MIFrect.h = rf->getVal();
        MIFunit = rf->getUnit();
        rf->skipStatement();
        if (Upage != M2Dunits[MIFunit])
          ConvRect(page_u);
        break;
      case leadrect:
        MIFrect.l = rf->getVal();
        MIFrect.t = rf->getVal();
        MIFrect.w = rf->getVal();
        MIFrect.h = rf->getVal();
        MIFunit = rf->getUnit();
        rf->skipStatement();
        if (Ulead != M2Dunits[MIFunit])
          ConvRect(lead_u);
        break;
      case mathval:
        MIFstring = rf->getString(MathFullMax);
        rf->skipStatement();
        break;
      case yorn:
        MIFval = (LookUp((ynstr = rf->getWord()), YesNoList) > 1) ? 1 : 0;
        rf->skipStatement();
				if (MIFval == -1)
					LogEvent(logwarn, 3, "Unknown Yes/No term ", ynstr);
        break;
      case listword:
      default:  // used only for wordlist identifiers
        MIFstring = rf->getWord();
        rf->skipStatement();
        MIFval = LookUp(MIFstring, wordlist[mp->mtype - listword]);
				if (MIFval == -1)
					LogEvent(logdebug, 3, "Unknown wordlist item ", MIFstring);
        break;
    }

    switch (mp->spec) {       // do MIF-to-DCL processing
      case ignore:
        break;
      case none:
        MakeCtl(mp);
        break;
      case pseudogroup:
        MakePseudo( (psg_id) mp->min);
        break;
      case dclgroup:
        MakeDclGroup(mp);
        break;
      case unit_set:
        if (MIFval < 0)
          break;
        rf->SetUnits = (mifunit) MIFval;
        ProcUnits();
        break;
      case new_file:
				LogEvent(logwarn, 1, "Duplicate MIF header");
        rf->Ferr = fform;  // second mif header is bad news
        break;

      case mif_id:
        ProcGrID();
        break;
      case mif_rect:
        ProcGrRect();
        break;

      case color_def:
        ProcColorDef();
        break;
      case color_tag:
        ProcColorTag();
        break;
      case color_val:
        ProcColorVal(mp->dat);
        break;

      case mif_uniq:
        ProcUnique(mp);
        break;

      default:
        if (mp->spec > styp_gr)
          ProcGr(mp);
        else if (mp->spec > styp_tb)
          ProcTb(mp);
        else if (mp->spec > styp_an)
          ProcAn(mp);
        else if (mp->spec > styp_tx)
          ProcTx(mp);
        else assert(0);
        break;
    }
  }

  if (Verbose && (MIFlevel != 0))
    printf("MIFlevel = %d at end of parsing.\n", MIFlevel);

#if 0
  // remove parsing lists and text buffer
  delete [] miflist;
  delete [] wordlist;
  delete TextObjLit;
  delete TextBuf;

  // clear allocated lists
  FormNameList.clear();
  FormTagNameList.clear();
  CFormNameList.clear();
  FrameNameList.clear();
  RFrameNameList.clear();
  PageNameList.clear();
  VarNameList.clear();
  XrefNameList.clear();

  // deallocate anum structs
  anum *a;
  alevel *al;
  for (i = 1; i <= PARefNum; i++) {
    a = (anum *) AnumStructList.find(i);
    assert(a);
    if (a->fnt)
      DeleteName(a->fnt);
    while (al = a->next) {
      a->next = al->next;
      delete al;
    }
    delete a;
  }
#endif

#ifdef VECINSET
	printf("Usage of PICT opcodes:\n");
	for (i = 0; i < 256; i++) {
		if (PictUsage[i])
			printf("0x%X: %d\n", i, PictUsage[i]);
	}
#endif
}



// group end processing


void (*MIFrd::EndProc[LevelMax])(void);



// default ctl processing


DCgroup *MIFrd::GroupStack[LevelMax];
DCctl *MIFrd::CtlStack[LevelMax];
int MIFrd::MIFlevel;


DCctl *
MIFrd::MakeCtl(mif *mp, bool attach)
{
  time_t tval;

  DCctl *cp = new DCctl(mp->dtype, mp->maj, mp->min, mp->dat);

  switch (mp->dtype) {
    case imnem:
      if (MIFval == -1)  // not found in list, already logged
        break;
      if (mp->dat == 0)
        cp->dat((uns) MIFval);
      else cp->siz(MIFval);
      break;
    case ibyte:
    case ishort:
    case ilong:
      cp->siz(MIFval);
      break;
    case elong:
      if (mp->mtype == ptval)
        cp->ex((void *) &MIFpt, sizeof(long) * 2);
      else if ((mp->mtype == arectval)
			 || (mp->mtype == brectval)
			 || (mp->mtype == srectval))
        cp->ex((void *) &MIFrect, sizeof(long) * 4);
      break;
    case ename:
    case etext:
      cp->ex((void *) MIFstring, strlen(MIFstring) + 1);
      break;
    case group:
    case endgroup:
      if (mp->dat == 0)
        cp->dat((uns) MIFval);
      break;
    case stamp:
      tval = time(&tval);
      cp->siz(tval);
      break;
    case ebyte:
    case eshort:
    case edoub:
    case ecode:
    default:
      assert(0);
      break;
  }

  if (attach)  // fasten it to the appropriate group
		GroupStack[MIFlevel]->add(*cp);
	return cp;
}


void
MIFrd::MakeDclGroup(mif *mp)
{
  DCgroup *gp = new DCgroup(DCctl(mp->dtype, mp->maj, mp->min,
                                  (uns) (mp->dat ? mp->dat : MIFval)));
  GroupStack[MIFlevel]->add(*gp);
  GroupStack[MIFlevel] = gp;
}


void
MIFrd::ProcUnique(mif *mp)
{
	if (InHiddenMarker) {
		InHiddenMarker = false;
		return;
	}
	if (BeginElement)  // ignore element start IDs
		return;
	CurrObjectID = MIFval;
	if (StartingFrame) { // graphic or table
		MakeCtl(mp);
		StartingFrame = false;
	}
	else if (IndexGroup) {  // put inside current index group
		DCctl *cp = MakeCtl(mp, false);
		IndexGroup->add(*cp);
		IndexGroup = NULL;
	}
	else if ((ObjectIDs != 'N')
	 && !BeginElement)
	  MakeCtl(mp);
	if (MathParaID == 0)
		MathParaID = MIFval;
}


void
MIFrd::ConvertStringName(bool vdef)
{
	bool vform = false;

	// process MIFstring for use as name, with int'l chars converted
	// use some of the ProcHex() logic, 
	unc *p = (unc *) MIFstring;
	while (*p) {
		if ((*p < ' ') || (*p >= 127) || (*p == '\\'))
			break;
		if (vdef) {
			if (*p == '<')
				vform = true;
			else if (*p == '>')
				vform = false;
		}
		p++;
	}
	if (*p == '\0')
		return;  // only normal text in MIFstring

	short len = strlen((char *) p);   // remaining space
	unc *raw = (unc *) NewName((char *) p);  // copy rest to raw
	unc ch = 0;
	unc och = 0;
	unl uch = 0;
	short ulen = 0;

	for (unc *r = raw; *r && len; r++) {
		if ((och = *r) > 127) {
			if ((MIFversion > 6)
			 && ((ulen = (short) ::IsUTF8(r)) > 0)) {
				uch = ::UTF8(&r);
				r--;
			}
			else         // not UTF-8
				uch = och;

			// convert uch to closest och
			switch (uch) {
				case 0x2000:  // spaces
				case 0x2001:
				case 0x2002:
				case 0x2003:
				case 0x2004:
				case 0x2005:
				case 0x2006:
				case 0x2007:
				case 0x2008:
				case 0x2009:
				case 0x200A:
					och = ' ';
					break;

				case 0x2010:  // dashes
				case 0x2011:
				case 0x2012:
				case 0x2013:
				case 0x2014:
				case 0x2015:
					och = '-';
					break;

				case 0x2018:  // left single quote
				case 0x2019:  // right single quote
				case 0x201A:
				case 0x201B:
					och = '\'';
					break;

				case 0x201C:  // left dbl quote
				case 0x201D:  // right dbl quote
				case 0x201E:
				case 0x201F:
					och = '"';
					break;

				case 0x2044:  // fraction slash
					och = '/';
					break;

				default:
					if (uch < 0x100) // treat as Latin-1
						och =  (unc) (uch & 0xFF);
					else
						och = 0x7F;
					break;
			}
		}
		else {
			och = 0;

			if (vdef) {
				if (*r == '<')
					vform = true;
				else if (*r == '>')
					vform = false;
				else if (vform == false) {
					*p++ = *r;
					len--;
					continue;
				}
			}
			if (*r == '\\') {  // backslash escape
				r++;
				if (*r == 'x') { // hex value
					r++;
					ch = (*r - ((*r > '9') ? ((*r > 'F') ? 'a'-10 : 'A'-10) : '0'));
					r++;
					ch = (ch << 4)
						 + (*r - ((*r > '9') ? ((*r > 'F') ? 'a'-10 : 'A'-10) : '0'));
					r++;
					assert(*r == ' ');
					if (ch < 0x80) { 
						switch (ch) {  // process hex results
							case 10: // para break, process as symbol
								if (!Asian)
									ch = 0xA6; // will go through ANSItab
								break;
							case 11: // end of flow, process as symbol
								if (!Asian)
									ch = 0xA4;
								break;

							case 16: // numeric space
							case 17: // hard space
							case 18: // thin space
							case 19: // en space
							case 20: // em space
								ch = ' ';
								break;

							case 21: // hard hyphen, \-
								ch = '-';
								break;

							default:
								break;
						}
					}
					else if (!Asian) { // chars 0x80 and up
						switch (ch) {  // process hex results
							case 0xD2: // double quotes
							case 0xD3:
								ch = '"';
								break;
							case 0xD4: // single quotes
							case 0xD5:
								ch = '\'';
								break;

							case 0xDA: // frac bar
								ch = '/';
								break;

							case 0xDE: // fi ligature, make "fi"
								*p++ = 'f';
								len--;
								ch = 'i';
								break;
							case 0xDF: // fl ligature, make "fl"
								*p++ = 'f';
								len--;
								ch = 'l';
								break;

							default:
								break;
						}
					}
				}
				else { // restore any other backslashes
					*p++ = '\\';
					len--;
					ch = *r;
				}
			}
			else ch = *r;  // no backslash
		}

		if (och == 0x7F) { // use UTF-8
			if (ulen <= len) {
				len -= ulen;
				p = ::UTF8(uch, p);
			}
			else {
				*p++ = '?';
				len--;
			}
			continue;	
		}
		else {
			if (och)
				ch = och;
			else if ((ch > 127)  // remap any high bit chars to 1252
			 && !NoSymMap
			 && !Asian)
				ch = ANSItab[ch - 128];

			if (ch >= ' ') {  // skip ctrl chars and 0
				*p++ = ch;
				len--;
			}
		}
	}	

	*p = '\0';  // terminate MIFstring
	DeleteName((char *) raw);
}



void
MIFrd::ConvertFileString(void)
{
	char *rdp = MIFstring;
	char *wrp = MIFstring;
	bool unixRoot = false;

	while (*rdp) {
		if (*rdp == '<') {
			rdp++;
			switch (*rdp) {
				case 'c':
					if ((wrp == MIFstring)
					 && (!unixRoot))
						*wrp++ = '.';
					*wrp++ = '/';
					unixRoot = false;
					break;
				case 'u':
					*wrp++ = '.';
					*wrp++ = '.';
					break;
				case 'r':
					// followed by <c>
					unixRoot = true;
					break;
				case 'v':
					// followed by drive with colon
					break;
				default:
					break;
			}
			rdp += 2;
		}
		else
			*wrp++ = *rdp++;
	}
	*wrp = '\0';
}




// pseudogroup processing


DCgroup *MIFrd::GroupList[psg_last];
uns MIFrd::CurrPG;


void
MIFrd::MakePseudo(psg_id pgnum)
{
  DCgroup *gp = new DCgroup;
  CurrPG = (uns) pgnum;
  GroupList[CurrPG] = gp;
  GroupStack[MIFlevel] = gp;
}



// units processing


uns MIFrd::M2Dunits[umax] = { 1, 2, 2, 3, 3, 4, 4 };

uns MIFrd::Upage = umax;
uns MIFrd::Ulead = umax;
uns MIFrd::Ufont = umax;
uns MIFrd::Ugline = umax;


void
MIFrd::ProcUnits()
{
  if (Upage != umax) // only set once in dcl
    return;
  Upage = M2Dunits[MIFval];
  MakeUnits(page_u, Upage);  // make ctl for page units
}


void
MIFrd::MakeUnits(dc_ucat umin, uns dat)
{
  DCctl *cp = new DCctl(imnem, 1, (uns) umin, dat, (dat == 1) ? 100 : 0);
  GroupList[psg_base]->add(*cp);  // scale inches units to .00001
}


void
MIFrd::ConvUnits(dc_ucat utype)
{
  uns wanted = 0;
  uns current = M2Dunits[MIFunit];

  switch (utype) {
    case page_u:
      if (Upage == umax)   // not put out yet
        MakeUnits(page_u, Upage = current);
      wanted = Upage;
      break;
    case font_u:
      if (Ufont == umax)   // not put out yet
        MakeUnits(font_u, Ufont = current);
      wanted = Ufont;
      break;
    case lead_u:
      if (Ulead == umax)   // not put out yet
        MakeUnits(lead_u, Ulead = current);
      wanted = Ulead;
      break;
    case gline_u:
      if (Ugline == umax)   // not put out yet
        MakeUnits(gline_u, Ugline = current);
      wanted = Ugline;
      break;
    default:  // wrong
      assert(0);
      return;
  }

  if (wanted == current)
    return;

  // make MIFval right for units wanted
  switch (current) {
    case 1: // val is in inches
      switch (wanted) {
        case 1:
          break;
        case 2:
          MIFval = (MIFval * 254) / 1000;
          break;
        case 3:
          MIFval = (MIFval * 72) / 1000;
          break;
        case 4:
          MIFval = (MIFval * 672) / 10000;
          break;
        default:
          break;
      }
      break;
    case 2: // val is in microns
      switch (wanted) {
        case 1:
          MIFval = (MIFval * 1000) / 254;
          break;
        case 2:
          break;
        case 3:
          MIFval = (MIFval * 72) / 254;
          break;
        case 4:
          MIFval = (MIFval * 672) / 2540;
          break;
        default:
          break;
      }
      break;
    case 3: // val is in pts * 100
      switch (wanted) {
        case 1:
          MIFval = (MIFval * 1000) / 72;
          break;
        case 2:
          MIFval = (MIFval * 254) / 72;
          break;
        case 3:
          break;
        case 4:
          MIFval = (MIFval * 672) / 720;
          break;
        default:
          break;
      }
      break;
    case 4: // val is in Didot * 100
      switch (wanted) {
        case 1:
          MIFval = (MIFval * 10000) / 672;
          break;
        case 2:
          MIFval = (MIFval * 2540) / 672;
          break;
        case 3:
          MIFval = (MIFval * 720) / 672;
          break;
        case 4:
          break;
        default:
          break;
      }
      break;
    default:  // bad
      assert(0);
      break;
  }
}


void
MIFrd::ConvPt(dc_ucat utype)
{
  MIFval = MIFpt.x;
  ConvUnits(utype);
  MIFpt.x = MIFval;

  MIFval = MIFpt.y;
  ConvUnits(utype);
  MIFpt.y = MIFval;
}


void
MIFrd::ConvRect(dc_ucat utype)
{
  MIFval = MIFrect.l;
  ConvUnits(utype);
  MIFrect.l = MIFval;

  MIFval = MIFrect.t;
  ConvUnits(utype);
  MIFrect.t = MIFval;

  MIFval = MIFrect.w;
  ConvUnits(utype);
  MIFrect.w = MIFval;

  MIFval = MIFrect.h;
  ConvUnits(utype);
  MIFrect.h = MIFval;
}



// graphics common properties processing


uns MIFrd::CurrID;
rect MIFrd::CurrGrRect;


void
MIFrd::ProcGrID()
{
  CurrID = (uns) MIFval;
}


void
MIFrd::ProcGrRect()
{
  CurrGrRect = MIFrect;
}


// color processing

double MIFrd::ColorVal;
char *MIFrd::ColorTag;
long MIFrd::ColorVals[ColorsMax];
DCnlist MIFrd::ColorDefs;
long MIFrd::LastColor = 8;

void
MIFrd::ProcColorDef(void)
{
	ColorTag = NULL;
	ColorVals[0] = 0;
	ColorVals[1] = 0;
	ColorVals[2] = 0;
	ColorVals[3] = 0;
  EndProc[MIFlevel] = EndColorDef;
}

void
MIFrd::EndColorDef(void)
{
	DCctl *cp = NULL;
	long cnum = 1; // default black

	// see if color is standard; if not, write RGB definition
	if ((ColorVals[0] == 0)
	 && (ColorVals[1] == 0)
	 && (ColorVals[2] == 0)
	 && (ColorVals[3] == 100000))
		cnum = 1;  // black
	else if ((ColorVals[0] == 0)
	 && (ColorVals[1] == 0)
	 && (ColorVals[2] == 0)
	 && (ColorVals[3] == 0))
		cnum = 2;  // white
	else if ((ColorVals[0] == 0)
	 && (ColorVals[1] == 100000)
	 && (ColorVals[2] == 100000)
	 && (ColorVals[3] == 0))
		cnum = 3;  // red
	else if ((ColorVals[0] == 100000)
	 && (ColorVals[1] == 0)
	 && (ColorVals[2] == 100000)
	 && (ColorVals[3] == 0))
		cnum = 4;  // green
	else if ((ColorVals[0] == 100000)
	 && (ColorVals[1] == 100000)
	 && (ColorVals[2] == 0)
	 && (ColorVals[3] == 0))
		cnum = 5;  // blue
	else if ((ColorVals[0] == 100000)
	 && (ColorVals[1] == 0)
	 && (ColorVals[2] == 0)
	 && (ColorVals[3] == 0))
		cnum = 6;  // cyan
	else if ((ColorVals[0] == 0)
	 && (ColorVals[1] == 100000)
	 && (ColorVals[2] == 0)
	 && (ColorVals[3] == 0))
		cnum = 7;  // magenta
	else if ((ColorVals[0] == 0)
	 && (ColorVals[1] == 0)
	 && (ColorVals[2] == 100000)
	 && (ColorVals[3] == 0))
		cnum = 8;  // yellow
	else {  // requires definition
		cnum = ++LastColor;
		short cols[4];
		cols[0] = 1;  // to specify RGB
		// convert to RGB; default is black for now ****
		short black = (uns) ((255L * ColorVals[3]) / 100000);
		cols[1] = cols[2] = cols[3] = 255 - black;
		cols[1] -= (uns) ((255L * ColorVals[0]) / 100000);  // red
		cols[2] -= (uns) ((255L * ColorVals[1]) / 100000);  // green
		cols[3] -= (uns) ((255L * ColorVals[2]) / 100000);  // blue
		for (uns i = 1; i < 4; i++) {
			if (cols[i] < 0)
				cols[i] = 0;
			else if (cols[i] > 255)
				cols[i] = 255;
		}
		// put out result
		cp = new DCctl(ibyte, 4, 80, (uns) cnum,
		 ((long) cols[0] << 24) | ((long) cols[1] << 16) |
		 ((long) cols[2] <<  8) | ((long) cols[3]));
		GroupStack[MIFlevel]->add(*cp);
	}

	// add ColorTag to list with color number to use
	if (ColorTag != NULL) {
		ColorDefs.add(ColorTag, cnum);
		cp = new DCctl(ename, 4, 80, (uns) cnum);
		cp->ex(ColorTag);
		GroupStack[MIFlevel]->add(*cp);
	}
}

void
MIFrd::ProcColorTag(void)
{
	if (*MIFstring != '\0')
		ColorTag = NewName(MIFstring);
}

void
MIFrd::ProcColorVal(short num)
{
	if (num <= ColorsMax) // CMYK values, given as 1..4
		ColorVals[num - 1] = (long) (ColorVal * 1000.0); // 100000 = 100%
}

void
MIFrd::ProcColor(void)
{
	// set MIFval to color ID, or 0 to ignore color in MIFstring
	MIFval = ColorDefs(MIFstring);
}



// utility functions


long
MIFrd::LookUp(char *str, cpp slist)
{
  long i;

  for (i = 0; slist[i] != NULL; i++)
    if (!strcmp(str, slist[i]))
      return i;
  return -1;
}



// DCL generation functions


void
MIFrd::WriteDCL(DCLwfile *wf)
{
	DCctl *cp = NULL;

	// put out all ext files now
	GroupList[psg_exfile]->execwf(WriteGrFiles, *wf);
	
  // put timestamp at head of dcl file
  time_t tval;
  *wf = DCctl(stamp, 1, 21, 6, (unl) time(&tval));
	cp = new DCctl(ilong, 1, 21, 6, MIFversion);
  *wf = *cp; 
	cp = new DCctl(ename, 1, 22, 1);
	cp->ex(DRmifVer);
  *wf = *cp; 

  // set up defaults group
  DCgroup *gp = new DCgroup(DCctl(group, 1, 40, 1));

  // identify default char set for names, etc.
  cp = new DCctl(ilong, 2, 14, 64, 8);
  gp->add(*cp);

  // identify user for user-defined ctls
  cp = new DCctl(ibyte, 1, 20, 200, 0x464D4946);
  gp->add(*cp);

  // collect defaults from pseudogroups base, doc, fnote, and color
  gp->merge(*GroupList[psg_base]);
  GroupList[psg_base] = NULL;
  gp->merge(*GroupList[psg_doc]);
  GroupList[psg_doc] = NULL;
  gp->merge(*GroupList[psg_fnote]);
  GroupList[psg_fnote] = NULL;
  gp->merge(*GroupList[psg_color]);
  GroupList[psg_color] = NULL;

  // write out defaults group
  gp->write(*wf);

  // set up resources group
  gp = new DCgroup(DCctl(group, 1, 40, 2));

  // collect resources from pseudogroups anum, var, xref, and cond
  gp->merge(*GroupList[psg_anum]);
  GroupList[psg_anum] = NULL;
  gp->merge(*GroupList[psg_var]);
  GroupList[psg_var] = NULL;
  gp->merge(*GroupList[psg_xref]);
  GroupList[psg_xref] = NULL;
  gp->merge(*GroupList[psg_cond]);
  GroupList[psg_cond] = NULL;

  // collect formats from pseudogroups pgf, font, rule, and tbl
  gp->merge(*GroupList[psg_pgf]);
  GroupList[psg_pgf] = NULL;
  gp->merge(*GroupList[psg_font]);
  GroupList[psg_font] = NULL;
  gp->merge(*GroupList[psg_rule]);
  GroupList[psg_rule] = NULL;
  gp->merge(*GroupList[psg_tbl]);
  GroupList[psg_tbl] = NULL;

  // write out resources group
  gp->write(*wf);

  // write out page images from pseudogroups page and tflow
  for (uns i = 1; i <= PageID; i++) {
     *wf = DCctl(group, 3, 10, i);
     PageFrameIndex.write(i, *wf);
     PageTextIndex.write(i, *wf);
     *wf = DCctl(endgroup, 3, 10, i);
  }

  // write EOF
  *wf = DCctl(ilong, 1, 60, 1, wf->tell());

	if (UseLog) {
		LogEvent(loginfo, 1, "Processing completed by drmif.");
		Log->CloseFile();
		CheckLogErrors();
	}
}


void
MIFrd::CheckLogErrors(void)
{
	if (Log->ErrWarn) {
		if (PMFlag)   // running from plugin
			ReleaseSemaphore(PMFlag, 1L, NULL);  // puts semaphore in signalled state
		else if (ShowLog) { // display log file
			char *cmd = NewName(EditorFileName, strlen(EditorFileName) + 
																					strlen(LogFileName) + 1);
			strcat(cmd, " ");
			strcat(cmd, LogFileName);
			DCauto::RunDOSCommand(cmd, true, false, false);
		}
	}
}


// ID file functions


char MIFrd::RefBuf[RefBufLen];


char *
MIFrd::GetFileID(char *name)
{
	if (!name || (*name == '\0'))
		return "";

	char *px = NULL;

	if (!IDFileName
	 && !FindIDFile())
		return "xx";

	DCpfile::getval("FileIDs", name, "", RefBuf, RefBufLen, IDFileName);
	if (*RefBuf == '\0') {
		DCpfile::getval("FDK", "NextFileID", "", RefBuf, RefBufLen, IDFileName);
		if (*RefBuf == '\0')
			strcpy(RefBuf, "aa");

		// check to see if next ID is already in use, ugly reverse lookup
		while (FileIDUsed(RefBuf)) {
			if (!IncAlpha(RefBuf))
				strcat(RefBuf, "a");
		}
		px = NewName(RefBuf);
		if (!IncAlpha(RefBuf))
			strcat(RefBuf, "a");
		DCpfile::putval("FDK", "NextFileID", RefBuf, IDFileName);
		DCpfile::putval("FileIDs", name, px, IDFileName);
		DCpfile::pflushall();
	}
	else
		px = NewName(RefBuf);

	//DeleteName(name);
	return px;
}


bool
MIFrd::IncAlpha(char *str)
{
	if (!str || (*str == '\0'))
		return false;

	bool done = false;

	for (char *cpt = str + strlen(str) - 1; (!done) && (cpt >= str); cpt--) {
		switch (*cpt) {
			case 'z':
				*cpt = 'a';
				break;
			case 'Z':
				*cpt = 'A';
				break;
			case '9':
				*cpt = '0';
				break;
			case 'k':  // skip lower-case L
				*cpt = 'm';
				done = true;
				break;
			case 'N':  // and upper-case O
				*cpt = 'P';
				done = true;
				break;

			default:
				if (isalnum(*cpt)) {
					(*cpt)++;
					done = true;
				}
				break;			
		}
	}

	return done;  // false for overflow
}


bool
MIFrd::FileIDUsed(char *id)
{
	char *fid = NewName(id);
	char *buf = new char[MaxIniLen];
	char *bpt = buf;
	bool rval = false;

	DCpfile::getval("FileIDs", NULL, "", buf, MaxIniLen, IDFileName);
	while (*bpt) {
		DCpfile::getval("FileIDs", bpt, "", RefBuf, RefBufLen, IDFileName);
		if (!stricmp(fid, RefBuf)) {
			rval = true;
			break;
		}
		while (*bpt)
			bpt++;
		bpt++;
	}

	strcpy(id, fid);  // restore original
	delete [] buf;
	return rval;
}


bool
MIFrd::FindIDFile(void)
{
	FILE *fp = NULL;

	// try current dir for mif2go.ini
	IDFileName = NewName("./mif2go.ini");
	if ((fp = fopen(IDFileName, "rb")) != NULL) {
		fclose(fp);
		return true;
	}	
	DeleteName(IDFileName);

	// try parent directory
	IDFileName = NewName("../mif2go.ini");
	if ((fp = fopen(IDFileName, "rb")) != NULL) {
		fclose(fp);
		return true;
	}	
	DeleteName(IDFileName);

	// try dir containing template, if any
	// give up
	return false;
}



// end of drmif.cc


