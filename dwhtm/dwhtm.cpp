 
// dwhtm.cpp is the code for dcl HTM writing, JHG
// Copyright (c) 1996-2006 by Omni Systems, Inc.  All rights reserved.

#include "dcappl.h"
#include "dcctlg.h"
#include "dcfili.h"
#include "dctxfx.h"
#include "dcfmtr.h"
#include "dcfilp.h"
#include "dcini.h"
#include "dcmacr.h"
#include "dcauto.h"
#include "dcmxl.h"
#include "dwhtm.h"
#include "dwhtmfl.h"
#include "dcicu.h"
#include <direct.h>
#include <windows.h>
#undef min


// start of code

char *HTMwr::FilterVer = { "DCL filter dwhtm, Ver 4.1" };
char *HTMwr::DRmifVer = { "m000" };
char *HTMwr::DWhtmVer = { "h293n" };
long HTMwr::MIFVer = 8;  // indicate Unicode for drxml
char *HTMwr::ICUdll = NULL;

swlist HTMwr::SwitchList[] = {
  { "F", NULL, HTMwr::SetFormat, true },
  { "f", NULL, HTMwr::SetFormat, true },
  { NULL, NULL }
};
bool HTMwr::FormatSet = false;
bool HTMwr::HelpForm = false;
char HTMwr::HelpType = 'S';
bool HTMwr::OracleHelp = false;
bool HTMwr::OmniHelp = false;
bool HTMwr::ListFile = false;
char HTMwr::ListType = 'B';  //Contents or Index or Both

short HTMwr::Verbose = 0;
uns HTMwr::SourceType = 0;
bool HTMwr::XMLSource = false;
bool HTMwr::UMXL = false;
bool HTMwr::UsePartFiles = false;
bool HTMwr::FMSource = false;
DCirfile *HTMwr::IniFile = NULL;
DCfrfile *HTMwr::FmtFile = NULL;
DCfrfile *HTMwr::PagesFile = NULL;
DCfrfile *HTMwr::TablesFile = NULL;
DCfrfile *HTMwr::SubFormatsFile = NULL;
char *HTMwr::OutputFormat = NULL;
char *HTMwr::IniFileName = NULL;
char HTMwr::OldFormType = 'N';


outtyp OutType = otnone;
bool HTMwr::DoAutomation = false;
bool HTMwr::EndingAuto = false;
bool HTMwr::BookAuto = false;

DCLrfile *HTMwr::DCLFile = NULL;
HTMwfile *HTMwr::HTMFile = NULL;

#ifdef _DLL

#define DllExport extern "C" __declspec( dllexport)

DllExport int
DCLmain(int argc, char* argv[])

#else

int
main(int argc, char *argv[])

#endif
{
  DCapp app(dw, ".htm");

  app.Init(argc, argv);

  if (!app.Proc(HTMwr::SwitchList)) {
    fprintf(stderr, "usage: %s [-v] [-f form] [-o htm_file] dcl_file\n", argv[0]);
    exit(dc_err_arg);
  }

  HTMwr::Verbose = app.verbose();

	if (HTMwr::ListFile) {  // entire different task
		DCrfile *rf = new DCrfile(app.rfile());
		if (rf->FErr() != fok)
			rf->Err();  // exits
		DCirfile *inihlpf	= NULL; // looks in current directory
		char *ifname = "mif2htm.ini";
		if (HTMwr::IniFileName
		 && ((inihlpf = new DCirfile(HTMwr::IniFileName, rf)) != NULL)
		 && (inihlpf->FErr() == fok)
		 && (inihlpf->Ferr == fok))
			HTMwr::IniFile = inihlpf;  // make accessible everywhere
		else if (HTMwr::IniFileName  // see if previous convention
		 && ((inihlpf = new DCirfile(HTMwr::IniFileName + 1, rf)) != NULL)
		 && (inihlpf->FErr() == fok)
		 && (inihlpf->Ferr == fok)) {
			HTMwr::IniFileName++;   // remove the leading underscore
			HTMwr::IniFile = inihlpf;  // make accessible everywhere
		}
		else if (HTMwr::IniFileName
		 && ((*(HTMwr::IniFileName + 1) = 'x') != '\0')
		 && ((inihlpf = new DCirfile(HTMwr::IniFileName, rf)) != NULL)
		 && (inihlpf->FErr() == fok)
		 && (inihlpf->Ferr == fok)) {
			HTMwr::XMLSource = true;
			DCauto::XMLSource = true;
			HTMwr::UMXL = true;
			HTMwr::IniFile = inihlpf;  // make accessible everywhere
		}
		else if (HTMwr::IniFileName
		 && ((*(HTMwr::IniFileName + 1) = 'd') != '\0')
		 && ((inihlpf = new DCirfile(HTMwr::IniFileName, rf)) != NULL)
		 && (inihlpf->FErr() == fok)
		 && (inihlpf->Ferr == fok)) {
			HTMwr::XMLSource = true;
			DCauto::XMLSource = true;
			HTMwr::IniFile = inihlpf;  // make accessible everywhere
		}
		else if (HTMwr::IniFileName
		 && ((inihlpf = new DCirfile(HTMwr::IniFileName + 1, rf)) != NULL)
		 && (inihlpf->FErr() == fok)
		 && (inihlpf->Ferr == fok)) {
			HTMwr::IniFileName++;   // remove the leading underscore
			HTMwr::IniFile = inihlpf;  // make accessible everywhere
		}
		else if ((!HTMwr::IniFileName || stricmp(HTMwr::IniFileName, ifname))
		 && ((inihlpf = new DCirfile(ifname, rf)) != NULL)
		 && (inihlpf->FErr() == fok))
			HTMwr::IniFile = inihlpf;
		else
			return dc_err_ini;

		HTMwr::IniFile = HTMwr::IniFile->SetConfigTemplate("Templates", "Configs");
		if (HTMwr::IniFile->Section("Index")
		 && HTMwr::IniFile->Find("UseIndex")
		 && HTMwr::IniFile->BoolRight())
			HTMwr::UseIndex = true;
		HTMwr::ProcListFile(rf, app.wfile());
		rf->CloseFile();
		return dc_ok;
	}

	if (HTMwr::EndingAuto) {  // another separate task
		DCirfile *inihlpf	= NULL; // looks in current directory
		char *ifname = "mif2htm.ini";
		if (HTMwr::IniFileName
		 && ((inihlpf = new DCirfile(HTMwr::IniFileName)) != NULL)
		 && (inihlpf->FErr() == fok)
		 && (inihlpf->Ferr == fok))
			HTMwr::IniFile = inihlpf;  // make accessible everywhere
		else if (HTMwr::IniFileName
		 && ((*(HTMwr::IniFileName + 1) = 'x') != '\0')
		 && ((inihlpf = new DCirfile(HTMwr::IniFileName)) != NULL)
		 && (inihlpf->FErr() == fok)
		 && (inihlpf->Ferr == fok)) {
			HTMwr::XMLSource = true;
			DCauto::XMLSource = true;
			HTMwr::UMXL = true;
			HTMwr::IniFile = inihlpf;  // make accessible everywhere
		}
		else if (HTMwr::IniFileName
		 && ((*(HTMwr::IniFileName + 1) = 'd') != '\0')
		 && ((inihlpf = new DCirfile(HTMwr::IniFileName)) != NULL)
		 && (inihlpf->FErr() == fok)
		 && (inihlpf->Ferr == fok)) {
			HTMwr::XMLSource = true;
			DCauto::XMLSource = true;
			HTMwr::IniFile = inihlpf;  // make accessible everywhere
		}
		else if ((!HTMwr::IniFileName || stricmp(HTMwr::IniFileName, ifname))
		 && ((inihlpf = new DCirfile(ifname)) != NULL)
		 && (inihlpf->FErr() == fok))
			HTMwr::IniFile = inihlpf;
		else
			return dc_err_ini;
		//HTMwr::FMSource = true;
		HTMwr::InitCurrProps(NULL);
		inihlpf->CloseFile();
		return dc_ok;
	}

	// read only from current dir
	char *rfname = app.rfile();
	char *pound = strrchr(rfname, '#');
	char *slash = strrchr(rfname, '/');
	if (!slash)
		slash = strrchr(rfname, '\\');
	if (slash
	 && pound
	 && (slash > pound)) {
		*pound = '\0';
		slash = strrchr(rfname, '/');
		if (!slash)
			slash = strrchr(rfname, '\\');
		*pound = '#';
	}

  DCLrfile rf(slash ? slash + 1 : rfname);
	HTMwr::DCLFile = &rf;
  if (rf.FErr() != fok)
    rf.Err();

	HTMctl *cp = new HTMctl;
	*cp = rf;  // read first ctl, should be source ID

	char *iname = "dwhtm.ini";

	if ((cp->dt() == stamp)
	 && (cp->maj() == 1)
	 && (cp->min() == 21)) {
		switch (HTMwr::SourceType = cp->dat()) {
			case 5:  // xml
				HTMwr::XMLSource = true;
				DCauto::XMLSource = true;
				iname = "xml2htm.ini";
				if (HTMwr::IniFileName)
					*(HTMwr::IniFileName + 1) = 'x';
				break;
			case 6:  // frame
				HTMwr::FMSource = true;
				iname = "mif2htm.ini";
#if 0
				*cp = rf;  // read second ctl, should be drmif ver
				if ((cp->dt() == ename)
				 && (cp->maj() == 1)
				 && (cp->min() == 22)
				 && (cp->dat() == 1))
					HTMwr::DRmifVer = (char *) cp->ex();
#endif
				break;
			case 24: // gpo
				iname = "gpo2htm.ini";
				break;
			case 25:  // DITA
				HTMwr::XMLSource = true;
				DCauto::XMLSource = true;
				iname = "dita2htm.ini";
				if (HTMwr::IniFileName)
					*(HTMwr::IniFileName + 1) = 'd';
				break;
			case 26:  // uDoc
				HTMwr::XMLSource = true;
				DCauto::XMLSource = true;
				HTMwr::UMXL = true;
				iname = "udoc2htm.ini";
				if (HTMwr::IniFileName)
					*(HTMwr::IniFileName + 1) = 'x';
				break;
			default:
				break;
		}
	}


	// write only to current dir
	char *wfname = app.wfile();
	pound = strrchr(wfname, '#');
	slash = strrchr(wfname, '/');
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
	if (slash)
		wfname = slash + 1;

	DCirfile *inif = NULL;	// in current directory
	if (HTMwr::IniFileName
	 && ((inif = new DCirfile(HTMwr::IniFileName, wfname)) != NULL)
	 && (inif->FErr() == fok)
	 && (inif->Ferr == fok))
		HTMwr::IniFile = inif;  // make accessible everywhere
	else if (HTMwr::XMLSource
	 && HTMwr::IniFileName
	 && ((*(HTMwr::IniFileName + 1) = 'x') != '\0')
	 && ((inif = new DCirfile(HTMwr::IniFileName, wfname)) != NULL)
	 && (inif->FErr() == fok)
	 && (inif->Ferr == fok)) {
		HTMwr::UMXL = true;
		HTMwr::IniFile = inif;  // make accessible everywhere
	}
	else if (HTMwr::IniFileName  // see if previous convention
	 && ((inif = new DCirfile(HTMwr::IniFileName + 1, wfname)) != NULL)
	 && (inif->FErr() == fok)
	 && (inif->Ferr == fok)) {
		HTMwr::IniFileName++;   // remove the leading underscore
		HTMwr::IniFile = inif;  // make accessible everywhere
	}
	else if (!HTMwr::IniFileName || stricmp(HTMwr::IniFileName, iname)
	 && ((inif = new DCirfile(iname, wfname)) != NULL)
	 && (inif->FErr() == fok))
		HTMwr::IniFile = inif;
	else
		return dc_err_ini;

  if (!HTMwr::InitCurrProps(wfname)) {  // OnlyAuto done
		rf.CloseFile();
		inif->CloseFile();
	  return dc_ok;
	}

	HTMwfile *wf = HTMwr::HTMFile;

	bool nofile = false;

  HTMwr::ReadDCL(&rf);   // read entire dcl file
  if (!HTMwr::WriteHTM(wf))   // put out the htm file
		nofile = true;

	inif->CloseFile();	// no need to check ini file for errors

	if (!HTMwr::DoAutomation) { // which already closed them
		rf.CloseFile();
	  wf->CloseFile();
	}
  rf.Err();      // these two just return if no err
  wf->Err();      // otherwise they exit with the err (and a message)

	if (nofile)
		_unlink(wfname);

  return dc_ok;  // so if we get here, it worked
}


void
HTMwr::SetFormat(char *fmt)
{
	bool newform = false;
	unc cnt = 0;

	FormatSet = true;  // overrules .ini
	HelpForm = true;
	Xhtml = false;
	Xml = false;
	DCfrfile::FmtOutputType = fmouthtm;
	OldFormType = toupper(*fmt);
	char lt = '\0';
	IniFileName = DCirfile::GetIniName("_m2", fmt, &OldFormType, &lt);
	switch (lt) {
		case 'B':
		case 'C':
		case 'I':
			ListFile = true;
			if (OldFormType != 'C')  // which always uses the default, B
				ListType = lt;
			break;

		case 'L':
			EndingAuto = true;
			BookAuto = true;
			break;

		case 'F':
			EndingAuto = true;
			BookAuto = false;
			break;

		default:
			break;
	}

#if 0
	IniFileName = NewName("m2", 20);
	if (strlen(fmt) > 2) {
		newform = true;
		if (!strnicmp(fmt, "htmlhelp", 8))
			ch = 'M', cnt = 8;
		else if (!strnicmp(fmt, "javahelp", 8))
			ch = 'J', cnt = 8;
		else if (!strnicmp(fmt, "oraclehelp", 10))
			ch = 'O', cnt = 10;
		else if (!strnicmp(fmt, "eclipse", 7))
			ch = 'E', cnt = 7;
		else if (!strnicmp(fmt, "html", 4))
			ch = 'N', cnt = 4;
		else if (!strnicmp(fmt, "xhtml", 5))
			ch = 'X', cnt = 5;
		else if (!strnicmp(fmt, "xml", 3))
			ch = 'G', cnt = 3;
		else if (!strnicmp(fmt, "docbook", 7))
			ch = 'B', cnt = 7;
		else if (!strnicmp(fmt, "dita", 4))
			ch = 'D', cnt = 4;
		else if (!strnicmp(fmt, "omnihelp", 8))
			ch = 'C', cnt = 8;
		else
			ch = 'Z';
	}
#endif

	switch (OldFormType) {
		case 'N':
			OutputFormat = "HTML";
			HelpForm = false;
			HelpType = 'S';
			OutType = othtm;
			//strcat(IniFileName, "html");
			break;
		case 'X':  // XHTML
			OutputFormat = "XHTML";
			Xhtml = true;
			HelpForm = false;
			HelpType = 'S';
			OutType = otxhtm;
			//strcat(IniFileName, "xhtml");
			break;

		case 'J':  // JavaHelp
			OutputFormat = "JavaHelp";
			HelpType = 'J';
			OracleHelp = false;
			OutType = otjh;
			//strcat(IniFileName, "javahelp");
			break;
		case 'O':  // OracleHelp
			OutputFormat = "Oracle Help for Java";
			HelpType = 'J';
			OracleHelp = true;
			OutType = otohj;
			//strcat(IniFileName, "oraclehelp");
			break;
		case 'E':  // Eclipse
			OutputFormat = "Eclipse Help";
			HelpType = 'E';
			Eclipse = true;
			OutType = oteh;
			//strcat(IniFileName, "eclipse");
			break;
		case 'M':  // MS HTML Help
			OutputFormat = "MS HTML Help";
			HelpType = 'M';
			OutType = othh;
			//strcat(IniFileName, "htmlhelp");
			break;
		case 'C':  // OmniHelp
			OutputFormat = "OmniHelp";
			HelpType = 'C';
			OmniHelp = true;
			OutType = otomh;
			//strcat(IniFileName, "omnihelp");
			break;

		case 'G':  // XML
			OutputFormat = "XML";
			DCfrfile::FmtOutputType = fmoutxml;
			Xml = true;
			HelpForm = false;
			HelpType = 'S';
			OutType = otxml;
			//strcat(IniFileName, "xml");
			break;
		case 'B':  // DocBook XML
			OutputFormat = "DocBook";
			DCfrfile::FmtOutputType = fmoutxml;
			Xml = true;
			HXMwr::HXML = true;
			HXMwr::DocBook = true;
			HelpForm = false;
			HelpType = 'S';
			OutType = otdbk;
			//strcat(IniFileName, "docbook");
			break;
		case 'D':  // DITA
			OutputFormat = "DITA";
			DCfrfile::FmtOutputType = fmoutxml;
			Xml = true;
			HXMwr::HXML = true;
			HXMwr::DITA = true;
			HelpForm = false;
			HelpType = 'S';
			OutType = otdita;
			//strcat(IniFileName, "dita");
			break;
		case 'U':  // uDoc
			OutputFormat = "uDoc";
			DCfrfile::FmtOutputType = fmoutxml;
			Xml = true;
			HXMwr::HXML = true;
			HXMwr::uDoc = true;
			UseDOCTYPE = false;
			HelpForm = false;
			HelpType = 'S';
			OutType = otudoc;
			break;

		case 'A':
			OutputFormat = "ASCII DCL";
			HelpForm = false;
			HelpType = 'S';
			OutType = otdcl;
			break;
		case 'F':
			OutputFormat = "Frame MIF";
			HelpForm = false;
			HelpType = 'S';
			OutType = otmif;
			break;

		default:  // unknown
			OutputFormat = "Unknown";
			HelpForm = false;
			FormatSet = false;
			OutType = otnone;
			//strcat(IniFileName, "html");
			break;
	}

#if 0
	strcat(IniFileName, ".ini");

	// second char specifies list type
	if (newform) {
		if (cnt
		 && ((ch = *(fmt + cnt)) != '\0')) {
			ListFile = true;
			if (!OmniHelp)
				ListType = toupper(ch);
		}			
	}
	else if ((ch = *(fmt + 1)) != '\0') {
		ListFile = true;
		if (!OmniHelp)
			ListType = toupper(ch);
	}
#endif
}



// initialization phase


char *HTMwr::CurrFileName = NULL;
char *HTMwr::CurrBaseName = NULL;
char *HTMwr::CurrFilePath = NULL;
char *HTMwr::CurrFileFull = NULL;
DCwfile *HTMwr::CssFile = NULL;
long HTMwr::CurrentDiv = 0;
long HTMwr::ObjectDiv = 0;
long HTMwr::ExtrObjectDiv = 0;
unc HTMwr::ClassSpaceChar = 0;
bool HTMwr::ClassIsTag = false;
bool HTMwr::SelectorIncludesTag = true;
bool HTMwr::LowerCaseCSS = false;
bool HTMwr::UseElements = false;
bool HTMwr::ValidOnly = false;
bool HTMwr::AllowNobr = true;

bool HTMwr::UseLog = true;
bool HTMwr::ShowLog = false;
bool HTMwr::ShowUndefinedFormats = true;
char *HTMwr::LogIniPrefix = "_m2";
char *HTMwr::LogFileName = "_m2g_log.txt";
char *HTMwr::EditorFileName = "notepad.exe";
DClog *HTMwr::Log = NULL;
unc HTMwr::LogLevels[logmax] = { 0, 1, 1, 1, 1, 0 };
bool HTMwr::LogIniChains = false;
HANDLE PMFlag = NULL;


bool
HTMwr::InitCurrProps(char *wfname)
{
	char *fname = NULL;

#if 0
	char *cpt = FilterVer;
	FilterVer = NewName(strlen(cpt) + strlen(DRmifVer) + strlen(DWhtmVer) + 3);
	sprintf(FilterVer, "%s %s %s", cpt, DRmifVer, DWhtmVer);
#endif
	SystemVarNames.init(SystemVarNamesSet);
	if (!Suffixes.next)
		Suffixes.init(SuffixesInit);

	if (XMLSource) {  // set defaults for DITA2Go
		LogIniPrefix = UMXL ? "_x2" : "_d2";
		LogFileName = NewName(LogIniPrefix, strlen(LogIniPrefix) + 10);
		strcat(LogFileName, "g_log.txt");
		SelectorIncludesTag = false;
		NoClassLists = false;
		NoAttribLists = true;
		UseIndex = true;
		//RefFileType = 'F';
		ShowLog = true;
		UseOrigGrName = true;
		UseChunking = true;
		DCauto::XMLSource = true;
	}

	if (IniFile) {
		if ((FinalIniParent = IniFile->GetSecFile()) == NULL)
			FinalIniParent = IniFile;

		// set IniFile to point to top of config chain
		IniFile = IniFile->SetConfigTemplate("Templates", "Configs");

		HXMwr::IniFile = IniFile;

		DCini::InitIniModule(IniFile, HTMIniSects,
		 (struct spprop *) &Curr, (struct tbprop *) &CTbl, &GraphIni,
		 &TblIni, &XrefIni, &XrefName, &FormName);

		if (IniFile->Section("Options")) {

			if (IniFile->Find("NoNameDel"))
				::NoNameDel = IniFile->BoolRight();

			if (IniFile->Find("NoMemDel"))
				::NoMemDel = IniFile->BoolRight();

			if (!FormatSet
			 && IniFile->Find("Output")) {
				DCfrfile::FmtOutputType = fmouthtm;
				HelpType = (char) IniFile->UCRight();
				if (HelpType == 'X') {
					HelpType = 'S';
					Xhtml = true;
				}
				else if (HelpType == 'G') {
					HelpType = 'S';
					DCfrfile::FmtOutputType = fmoutxml;
					Xml = true;
				}
				else if (HelpType == 'B') {
					HelpType = 'S';
					DCfrfile::FmtOutputType = fmoutxml;
					Xml = true;
					HXMwr::HXML = true;
					HXMwr::HXref = true;
					HXMwr::DocBook = true;
				}
				else if (HelpType == 'D') {
					HelpType = 'S';
					DCfrfile::FmtOutputType = fmoutxml;
					Xml = true;
					HXMwr::HXML = true;
					HXMwr::HXref = true;
					HXMwr::DITA = true;
				}
				HelpForm = (HelpType == 'S') ? false : true;
			}
			if ((HelpType == 'J')
			 || (HelpType == 'E')) {
				UseFontFace = false;
				QuotedEncoding = false;
			}
			else
				JHVersion2 = false;
		}

		if (Xhtml || Xml) { // set XML defaults here
			UTF8 = true;
			XMLEncoding = "UTF-8";
			LowerCaseCSS = true;
			if (Xml) {
				ContentType = "application/xml";
				ClassIsTag = true;
				AllowOver = false;
				if (!Xhtml) {
					XMLRoot = "doc";
					UseAnums = false;
					UseTypographicElements = false;
					FootType = 'I';
					UseFootXrefTag = true;
					CellAlignAttributes = false;
					CALS = true;
				}
			}

			if (HXMwr::DocBook)
				HXMwr::InitDocBook();

			if (HXMwr::DITA) {
				HXMwr::InitDITA();
				XrefFormatIsXrefClass = true;
				UseConditionalFlagging = false;
			}
		}
		else  if (!XMLSource) {
			UTF8 = false;
			XMLEncoding = "ISO-8859-1";
		}

		if ((HelpType == 'J')
		 || (HelpType == 'E')) {
			UseFTS = true;
			LowerCaseCSS = true;
		}

		if (HelpType == 'J') {
			UseListTypeAttribute = true;
			UsePxSuffix = false;
		}

		if ((HelpType == 'M')
		 || (HelpType == 'J')
		 || (HelpType == 'E')
		 || (HelpType == 'C')
		 || HXMwr::HXML)
			UseIndex = true;


		// finished setting defaults, get user settings

		DCini::ProcAllSects();
		if (HXMwr::DocBook)
			DCini::ProcIniSect("DocBookOptions");
		if (HXMwr::DITA)
			DCini::ProcIniSect("DITAOptions");

		if (PrjFileName)
			SetNamePath(PrjFileName, &CurrPrjName, &CurrPrjPath);

		if (ICUdll)
			DCicu::ICUdll = ICUdll;
	}

	if (UseLog) {
		PMFlag = OpenSemaphore(SEMAPHORE_ALL_ACCESS, FALSE,
			 (XMLSource ? "d2gpm" : "m2gpl"));

		Log = new DClog(LogFileName, "dwhtm", DWhtmVer, LogLevels);
		if (Log->FErr() != fok)
			UseLog = false;
		LogFileName = NewName(Log->getFilePath());
	}
	DCirfile::LogIniChain = LogIniChains;
	DCirfile::LogEventFunc = (logfunc) LogEvent;
	DCicu::LogEventFunc = (logfunc) LogEvent;
	DCauto::LogEventFunc = (logfunc) LogEvent;
	DCauto::LogFileName = NewName(LogFileName);

	if (XMLSource)
	  RefFileType =  UsePartFiles ? 'B' : 'F';

	if ((XMLSource
	  && !UsePartFiles)
	 || EndingAuto
	 || CopyOriginalGraphics) {
		DCauto::IndexNavType = IndexNavType;
		DoAutomation = DCauto::SetUpAutomation(OutType, IniFile,
			                                     CssPath, GraphPath);
		if (!DoAutomation
		 && ((DCauto::Compiler && DCauto::MakeHLP)
		  || DCauto::FTSCommand))
			DoAutomation = true;
	}

	HTMwfile *wf = NULL;

	if (wfname) {	// really going to write to output file
		wf = new HTMwfile(wfname);

		if (wf->FErr() != fok)
			wf->Err();

		HTMFile = wf;
		if (HXMwr::DITA)
			wf->setClassAttrName("outputclass");

		if (PrintProject)
			ExcludedTopics = &ExcludeFromPrint;

		if (UseLog)
			Log->logEvent(loginfo, 1, "Opened file \"", wf->getFilePath(),
			"\" for ", OutputFormat);
	}

	if (LogIniChains)
		DCirfile::WriteIniChain("Configs");

	if (IniFile->Section("Templates")
	 && (IniFile->Find("Macros")
	  || IniFile->Find("MacroFile")))
		MacroFile = IniFile->PathRight();
	DCauto::MacroFile = MacroFile;

	if (DCauto::OnlyAuto
		|| EndingAuto) {
		if (DoAutomation)
			DCauto::WrapShipProc(EndingAuto ? "EndAuto.dcl" : DCLFile->getFileName());
		if (UseLog) {
			LogEvent(loginfo, 1, "Automation processing completed by dwhtm.");
			Log->CloseFile();
			CheckLogErrors();
		}
		return false;
	}

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
			if (FmtFile->Section("Setup")
			 && FmtFile->Find("MergeCurrFormatProps"))
				MergeCurrFormatProps = FmtFile->BoolRight();
			if (FmtFile->Section("BaseValues")) {
				FormatBaseSet = true;
				if (FmtFile->Find("BodyBaseHeight"))
					FmtFile->BodyBaseSize = FmtFile->StrRight();
				else
					FmtFile->BodyBaseVal = (double) CssBodyFontTwips;
				if (FmtFile->Find("BodyBaseWidth"))
					FmtFile->IndentBaseWidth = FmtFile->StrRight();
				else
					FmtFile->IndentBaseVal = (double) CssIndentBaseTwips;
			}
		}
	}

	if ((IniFile->Section("Templates")
	  && IniFile->Find("Pages")
	  && ((fname = IniFile->PathRight()) != NULL))
	 || (FmtFile
	  && FmtFile->Section("Templates")
		&& FmtFile->Find("Pages")
	  && ((fname = FmtFile->PathRight()) != NULL))) {
		PagesFile = new DCfrfile(fname);
		if (PagesFile->FErr() != fok) {
			LogEvent(logwarn,1,"Pages file not found: ", fname);
			PagesFile = NULL;
		}
		else {
			PagesFile = PagesFile->SetConfigTemplate("Templates", "Pages");
			DCfrfile::Pages = PagesFile;
		}
	}

	if ((IniFile->Section("Templates")
	  && IniFile->Find("Tables")
	  && ((fname = IniFile->PathRight()) != NULL))
	 || (FmtFile
	  && FmtFile->Section("Templates")
		&& FmtFile->Find("Tables")
	  && ((fname = FmtFile->PathRight()) != NULL))) {
		TablesFile = new DCfrfile(fname);
		if (TablesFile->FErr() != fok) {
			LogEvent(logwarn,1,"Tables file not found: ", fname);
			TablesFile = NULL;
		}
		else {
			TablesFile = TablesFile->SetConfigTemplate("Templates", "Tables");
			DCfrfile::Tables = TablesFile;
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


	if (HelpType == 'C') {
		OmniHelp = true;
		HTMLVersion = 4;
		NoFont = true;
		Basefont = false;
		AlignAttributes = false;
		UseFTS = true;
		DropJSLocation = "none";
		DCini::ProcIniGroup("OHSet");
		if (((RefFileType == 'B')
		  || (RefFileType == 'F'))
		 && ((ListType == 'B')
		  || (ListType == 'C')))
			MakeContents = true;
		if (UnicodeFTS) {
			Encoding = 'U';
			UTF8 = true;
			NoEnc = false;
			XMLEncoding = "UTF-8";
			NumericCharRefs = false;
		}
		if (UseFTS)
			InitOHSearch();
		MakeAliasFile = true;
		if (OHProjFileXhtml)
			Xhtml = true;
	}

	if (NoWrap)
		wf->SetNoWrap(true);

	if (Xhtml) {
		HTMLVersion = 4;
		wf->SetXhtml(true);
		if (!FootSep)
			FootSep = FootSepX;
	}
	else if (Xml) {
		wf->SetXml(true);
		if (!FootSep)
			FootSep = FootSepX;
		UseHeadAndBody = false;
		AlignAttributes = false;
		if (HXMwr::DITA || HXMwr::DocBook)
			GraphScale = true;
		else
			GraphScale = false;
		TableAttributes = false;
		CellColorAttributes = false;
		if (HXMwr::DITA
		 && !AliasPrefix)
			AliasPrefix = NewName("HIDC_, IDH_");
	}
	else {
		if (!FootSep)
			FootSep = FootSepH;
	}

	if (HelpType == 'E')
		DCini::ProcIniGroup("EHSet");

	if (HelpType == 'J') {
		NoANames = (OracleHelp || JHVersion2) ? false : true;
		if (XMLSource)
			DCini::ProcIniGroup(OracleHelp ? "OHJXSet" : "JHXSet");
		else {
			DCini::ProcIniGroup("JavaSet");
			DCini::ProcIniGroup(OracleHelp ? "OHJSet" : "JHSet");
		}
		if (OracleHelp)
			JHVersion2 = false;
		if (OracleHelp || !JHVersion2) {
			UseGlossary = false;
			UseFavorites = false;
		}
		if (JHVersion2) {
			JHDtdVer = "2_0";
			JHFileVer = "2.0";
		}
		HTMLVersion = 3;
		PrologDType = PrologDType3;
		if (((RefFileType == 'B')
		  || (RefFileType == 'F'))
		 && ((ListType == 'B')
		  || (ListType == 'C')))
			MakeContents = true;
	}

	CurrFileName = NewName(wf->getBaseName());
	CurrBaseName = NewName(wf->getBaseName());
	CurrFilePath = NewName(wf->getFilePath());
	size_t cfpend = strlen(CurrFilePath) - 1;
	if ((CurrFilePath[cfpend] == '\\')
	 || (CurrFilePath[cfpend] == '/'))
		CurrFilePath[cfpend] = '\0';
	CurrFileFull = NewName(wf->getFileName());

	SetXRFLists();
	SetFIDLists();
	CurrFilePrefix = GetFIDPrefix(CurrFileName);
	LogEvent(loginfo, 1, "File ID is ", CurrFilePrefix);

	CurrPxLen = strlen(CurrFilePrefix);
	XrefFilePrefix = CurrFilePrefix;
	XrefFile = NULL;

	RefFileName = NewName(strlen(CurrFileName) + 7);
	sprintf(RefFileName, "./%s.ref", CurrFileName);

	if (FMSource) {
		GrxFileName = NewName(strlen(CurrFileName) + 7);
		sprintf(GrxFileName, "./%s.grx", CurrFileName);
	}
	else if (UsePartFiles) {
		GrcFileName = NewName(strlen(CurrFileName) + 7);
		sprintf(GrcFileName, "./%s.grc", CurrFileName);
	}

	SetUpRefFile();

	if (WriteCssStylesheet == 'A')  // generating CSS
		StartCSSFile();

	if (WriteCssStylesheet == 'O') { // initialize CSS if not present
		DCrfile *rf = new DCrfile(CssName);
		if (rf->FErr() == fok)
			rf->CloseFile();
		else
			StartCSSFile();
	}

	if (HelpType == 'M') {  // MS HTML Help setup
		UseCodePage = true;
		NoEnc = true;
		MakeAliasFile = true;
		DCini::ProcIniGroup("HHSet");
		if (XMLSource) {
			if (MakeAliasFile
			 && !AliasPrefix)
				AliasPrefix = NewName("HIDC_, IDH_");
		}
		ObjectDiv = wf->NewDiv();  // create OBJECT bins
		ExtrObjectDiv = wf->NewDiv();
		wf->SelectDiv(0);          //  but don't use them yet
		if (((RefFileType == 'B')
		  || (RefFileType == 'F'))
		 && ((ListType == 'B')
		  || (ListType == 'C')))
			MakeContents = true;

		if ((DefaultChmFile == NULL)
		 && IniFile
		 && IniFile->Section("ChmFiles")
		 && IniFile->Find(CurrFileName))
			DefaultChmFile = IniFile->StrRight();

		if (DefaultChmFile == NULL)  // not in the .ini
			DefaultChmFile = NewName(CurrFileName);
	}

	if (HelpFileLanguage) {  // can also be for OH
		DCicu::SetCodePage(HelpFileLanguage);
		XMLEncoding = UnicodeFTS ? "UTF-8" : DCicu::XMLEncoding;
		if (DCicu::CodePage == 0) {
			UseCodePage = false;  // for HH
			NoEnc = false;
			UnicodeFTS = false;   // for OH
			LogEvent(logwarn, 1, "Reqested locale requires Unicode: ",
			 HelpFileLanguage);
		}
		else if ((DCicu::CodePage == 1255)
		 || (DCicu::CodePage == 1256))
			LogEvent(logwarn, 1, "RTL locale not fully supported: ",
			 HelpFileLanguage);
	}

	SetFileSequence();  // need names above to be set up first

	if (MakeAliasFile && AliasPrefix)
		SetAliasPxs();

	if (IniFile
	 && IniFile->Section("FontSize")) {	// read custom size info
		for (uns i = 2; i <= FontSizesMax; i++) {
			if (IniFile->Find(i))
				FontSizeVector[i - 2] = (uns) IniFile->IntRight();					
		}			
	}

  finfo *finf = new finfo;
	finf->fname = BaseFontName;

  finf->fnum = Curr.fnum = 1;
  finf->ftype = 1;
	finf->ftsym = 0;
  FontList.add(finf, 1);
  FontNameList.add(finf->fname, 1);
	Curr.algn = 1;
  Curr.fsize = BaseSize;
  Curr.lspace = 240;
  Curr.colr = 2;  // black
  CurrTab = new tinfo;

	if (IniFile
	 && IniFile->Section("Titles")) {	// read title
		if (IniFile->Find(CurrFileName)) {
			Title = IniFile->StrRight();
			TitleSet = true;
		}
	}

	DCini::ProcIniSect("Graphics"); // after Help sections

	SetFilterAttributes();

	DCmacr::InitMacroModule(wf, IniFile, MacroFile,
	 WriteMacroChar, WritePredefinedMacro,
	 WriteCodeStoreVar, GetCodeStoreVar,
	 GetTextStore, GetPredefinedVar,
	 GetFrameUserVar, GetFrameUserArrVar,
	 ValidOnly,  // from here on, for back compatibility only
	 NameUndefMacroVars, NameUndefMacros,
	 FixMacroQuotes, MacroVarNesting, OmitMacroReturns,
	 MacroNestMax, RepeatMax, WhileMax);
	DCmacr::WriteMacroTextSingleChar = &WriteTextSingleChar;

	DCirfile::LogIniChain = LogIniChains = false;  // all logged now

	// restore inimod globals changed by macrmod
	DCini::InitIniModule(IniFile, HTMIniSects);

	return true;
}


void
HTMwr::LogEvent(logtype lt, unc sev, char *desc,
 char *d2, char *d3, char *d4, char *d5)
{
	if (!UseLog)
		return;

	Log->logEvent(lt, sev, desc, d2, d3, d4, d5);
}


void
HTMwr::SetNamePath(const char *pathname, char **oname, char **opath)
{
	if (!pathname)
		return;

	char *slash = strrchr(pathname, '\\');
	if (!slash)
		slash = strrchr(pathname, '/');

	if (opath) {
		if (!slash)
			*opath = NewName("");
		else
			*opath = NewName(pathname, slash - pathname);
	}
	if (oname) {
		if (!slash) 
			*oname = NewName(pathname);
		else
			*oname = NewName(slash + 1);
	}
}


void
HTMwr::StartCSSFile(void)
{
	CssFile = new DCwfile(CssName, ftxt);
	if (CssFile->FErr() != fok)
		WriteCssStylesheet = 'N';  // failed write, just use
	else {
		if (GeneratorTag == 'C') {
			CssFile->putStr("/* ");
			CssFile->putStr(CssName);
			CssFile->putStr(" produced by ");
			CssFile->putStr(XMLSource ? (UMXL ? "uDoc2Go " : "DITA2Go ") : "Mif2Go ");
			CssFile->putStr(FilterVer);
			CssFile->putStr(" */");
			CssFile->putCr();
		}
		if (IniFile
		 && IniFile->Section("CSSStartMacro")) {
			CssFile->putCr();
			char *txt = IniFile->SecCont(true);
			DCmacr::WriteMacro((HTMwfile *) CssFile, txt);
			DeleteName(txt);
			CssFile->putCr();
		}
		CssFile->putCr();
		if (FmtFile)
			InitElementForms();
		else if (CssBodyFontSize
		 && CssBodyFontTag) {
			char *txt = NewName(NumStrMax);
			sprintf(txt, "%ld", CssBodyFontSize);
			CssFile->putStr("body { font-size: ");
			CssFile->putStr(txt);
			DeleteName(txt);
			CssFile->putStr(CssUnits[CssBodyFontUnit]);
			CssFile->putStr(" }");
			CssFile->putCr();
			CssFile->putCr();
		}
	}
}

bool HTMwr::CaselessMatch = true;
bool HTMwr::SpacelessMatch = true;
bool HTMwr::WildcardMatch = true;
unc HTMwr::WriteMadeWithGraphic = 'M';
unc HTMwr::Encoding = 'U';
char *HTMwr::Ansi = NULL;



// DCL reading phase


htm HTMwr::HTMpile[] = {
  { imnem,    1, 10, 0, unitset,  top,   ignore },   // page units
  { imnem,    1, 11, 0, unitset,  top,   ignore },   // lead units
  { imnem,    1, 12, 0, unitset,  top,   ignore },   // font units
  { imnem,    1, 13, 0, unitset,  top,   ignore },   // graph line units

  { ilong,    1, 21, 0, srcver,   top,   ignore },   // source version
  { ename,    1, 22, 1, drname,   top,   ignore },   // reader name
	{ imnem,    1, 24, 1, ispart,   top,   ignore },   // is a part file

  { group,    1, 40, 1, nothing,  tx,    ignore },   // defaults set
  { endgroup, 1, 40, 1, docdefs,  tx,    ignore },   // all defaults set
  { group,    1, 40, 2, nothing,  tx,    ignore },   // resource set
  { endgroup, 1, 40, 2, docres,   tx,    ignore },   // all formats set

  { ename,    1, 50, 4, txfile,   tx,    putfile },  // named text file

  { imnem,    1, 80, 1, dbgscan,  top,   dbgwrite }, // debugging

  { group,    2,  1, 0, flowlist, tx,    flnum },    // text segment
  { endgroup, 2,  1, 0, flowend,  tx,    flend },    // end text segment
  { imnem,    2,  4, 0, nothing,  tx,    puttbr },   // topic break
  { ename,    2,  6, 1, flowname, tx,    ignore },   // named stream
  { imnem,    2,  5, 0, setanch,  gr,    putfr },    // frame anchor

  { etext,    2, 10, 1, flowused, tx,    puttx },    // text obj lit
  { imnem,    2, 10, 1, flowused, tx,    puttx },    // text obj lit

  { group,    2, 10, 2, nothing,  an,    ignore },   // var text group
  { ilong,    2, 10, 2, varnum,   an,    putvar },   // var text item
  { ilong,    2, 10, 8, varanum,  an,    putavar },  // var array item
  { group,    2, 90, 0, vardef,   an,    ignore },   // var def start
  { etext,    2, 91, 1, varname,  an,    ignore },   // var name
  { group,    2, 91, 2, varform,  an,    ignore },   // var format def
  { ilong,    2, 92, 1, varacnt,  an,    ignore },   // var array count
  { group,    2, 92, 2, varaelem, an,    ignore },   // var array elem
  { ilong,    2, 92, 3, varaidx,  an,    ignore },   // var array index
  { group,    2, 92, 4, varaform, an,    ignore },   // var array format
  { imnem,    2, 97,24, varsys,   an,    putvsys },  // sys var (chapnum)

  { group,    2, 10, 3, nothing,  an,    ansgrp },   // anum text group
  { endgroup, 2, 10, 3, nothing,  an,    anegrp },   // end anum text group

#ifdef UNUSED
  { group,    2, 10, 4, nothing,  an,    ignore },   // anum ref group
  { group,    2, 10, 5, nothing,  an,    ignore },   // anum pg ref group
  { group,    2, 10, 6, nothing,  an,    ignore },   // gen text group
#endif

  { imnem,    2, 11, 0, formchk,  tx,    putbreak }, // text breaks
  { imnem,    2, 11, 1, formchk,  tx,    parstart }, // paragraph start
  { imnem,    2, 11, 2, formchk,  tx,    parend },   // paragraph end
  { imnem,    2, 11,10, formchk,  tx,    linend },   // line end

  { imnem,    2, 12, 1, formchk,  tx,    putkeep },  // keep with prev
  { imnem,    2, 12, 2, formchk,  tx,    putkeepn }, // keep with next

  //{ ilong,    2, 14, 0, charset,  tx,    putcset },  // character set

  { ename,    2, 15, 1, fontlist, tx,    putfont },  // named font
  { imnem,    2, 15, 1, fontlist, tx,    putfont },  // numbered font
  { ilong,    2, 15, 2, formchk,  tx,    putfsize }, // font size
  { imnem,    2, 15, 4, formchk,  tx,    putbold },  // font bolding
  { imnem,    2, 15, 5, formchk,  tx,    putital },  // font italics
  { imnem,    2, 15, 6, formchk,  tx,    putbase },  // super/subscript
  { imnem,    2, 15, 7, formchk,  tx,    putcaps },  // small/all caps
  { imnem,    2, 15, 8, formchk,  tx,    putstyl },  // shadow/outline
  { imnem,    2, 15, 9, formchk,  tx,    putrevb },  // rev bars, strike
  { imnem,    2, 15,10, formchk,  tx,    putlmod },  // underline mode
  { imnem,    2, 15,11, formchk,  tx,    putline },  // under/overline
  { imnem,    2, 15,13, formchk,  tx,    putcolr },  // text color

  //{ imnem,    2, 17, 2, language, tx,    putlang },  // language
  { ilong,    2, 20, 1, formchk,  tx,    putdown },  // rel space	down
  { ilong,    2, 20, 2, formchk,  tx,    putup },    // rel space	up
  { ilong,    2, 30, 1, formchk,  tx,    putright }, // rel space	right
  { ilong,    2, 30, 2, formchk,  tx,    putleft },  // rel space	left

  { ilong,    2, 21, 0, formchk,  tx,    putlspc },  // line space
  { ilong,    2, 22, 0, formchk,  tx,    putpspc },  // para space
  { ilong,    2, 31, 0, formchk,  tx,    putalgn },  // para alignment
  { ilong,    2, 32, 0, formchk,  tx,    putindt },  // para indents
  { ilong,    2, 34, 3, formchk,  tx,    putkern },  // track kerning

  { imnem,    2, 35, 1, formchk,  tx,    putspan },  // place span
  { imnem,    2, 35, 2, formchk,  tx,    putside },  // place side
  { etext,    2, 35, 3, formchk,  tx,    putrun },   // place runin	punct
  { group,    2, 35, 3, setrunin, tx,    putrunin }, // start runin	punct
  { endgroup, 2, 35, 3, endrunin, tx,    eptrunin }, // end runin	punct
  { imnem,    2, 36, 1, sideflow, tx,    ignore },   // sidehead flow

  { group,    2, 40, 0, formchk,  tx,    puttdef },  // tab def start
  { endgroup, 2, 40, 0, formchk,  tx,    endtdef },  // tab def end
  { ilong,    2, 41, 0, formchk,  tx,    puttab },   // tab description
  { ilong,    2, 42, 1, formchk,  tx,    tabfill },  // tab fill char
  { etext,    2, 42, 1, formchk,  tx,    tabfstr },  // tab fill string

  { group,    2, 50, 0, tbstart,  tb,    puttbl },   // table start
  { endgroup, 2, 50, 0, tbend,    tb,    endtbl },   // table end
  { group,    2, 51, 2, tformck,  tb,    tbplace },  // table place start
  { endgroup, 2, 51, 2, tformck,  tb,    tbeplac },  // table place end
  { ilong,    2, 51, 3, tformck,  tb,    tblrult },  // table top rule
  { ilong,    2, 51, 4, tformck,  tb,    tblrulb },  // table bottom rule
  { ilong,    2, 51, 5, tformck,  tb,    tblrull },  // table left rule
  { ilong,    2, 51, 6, tformck,  tb,    tblrulr },  // table right rule

  { imnem,    2, 51, 8, tformck,  tb,    tblalrc },  // table row/col alt
  { imnem,    2, 51, 9, tformck,  tb,    tblncnt },  // table norm count
  { imnem,    2, 51,10, tformck,  tb,    tblacnt },  // table alt count
  { imnem,    2, 51,11, tformck,  tb,    tblaclr },  // table alt color
  { imnem,    2, 51,12, tformck,  tb,    tblafll },  // table alt fill pattern

	{ group,    2, 51,14, tformck,  tb,    tbldflt },  // table defaults start
  { endgroup, 2, 51,14, tformck,  tb,    tbedflt },  // table defaults end

  { group,    2, 52, 0, tformck,  tb,    tbcol },    // column start
  { endgroup, 2, 52, 0, tformck,  tb,    tbcend },   // column end
  { ilong,    2, 53, 1, nothing,  tb,    tbcols },   // column count
  { ilong,    2, 53, 2, nothing,  tb,    tbcwid },   // column width
  { imnem,    2, 53, 7, tblchk,   tb,    tbcwtyp },  // column width type
  { elong,    2, 53, 2, tblchk,   tb,    tbcwids },  // column widths
  { imnem,    2, 53, 3, tformck,  tb,    tbccolr },  // column color
  { imnem,    2, 53, 4, tformck,  tb,    tbcfill },  // column fill
  { ilong,    2, 53, 5, tformck,  tb,    tbcrull },  // column left rule
  { ilong,    2, 53, 6, tformck,  tb,    tbcrulr },  // column right rule

  { group,    2, 54, 0, trstart,  tb,    tbrow },    // row start
  { endgroup, 2, 54, 0, trend,    tb,    tbrend },   // row end
  { ilong,    2, 55, 2, nothing,  tb,    tbrhigh },  // row height
  { ilong,    2, 55, 3, nothing,  tb,    tbrhfix },  // row fixed height
  { ilong,    2, 55, 4, nothing,  tb,    tbrhmin },  // row min height
  { ilong,    2, 55, 5, nothing,  tb,    tbrhmax },  // row max height
  { imnem,    2, 55, 7, tformck,  tb,    tbrcolr },  // row color
  { imnem,    2, 55, 8, tformck,  tb,    tbrfill },  // row fill pattern
  { ilong,    2, 55, 9, tformck,  tb,    tbrrult },  // row top rule
  { ilong,    2, 55,10, tformck,  tb,    tbrrulb },  // row bottom rule
  { ilong,    2, 55,11, tformck,  tb,    tbrruls },  // row separator rule

  { group,    2, 56, 0, tcstart,  tb,    tbcell },   // cell start
  { endgroup, 2, 56, 0, tcend,    tb,    tbclend },  // cell end
  { ilong,    2, 57, 1, nothing,  tb,    tbclstrd }, // cell col straddle
  { ilong,    2, 57, 2, nothing,  tb,    tbclrstr }, // cell row straddle
  { imnem,    2, 57, 5, tformck,  tb,    tbclcolr }, // cell color
  { imnem,    2, 57, 6, tformck,  tb,    tbclfill }, // cell fill
  { ilong,    2, 57, 7, tformck,  tb,    tbclrult }, // cell top rule
  { ilong,    2, 57, 8, tformck,  tb,    tbclrulb }, // cell bottom rule
  { ilong,    2, 57, 9, tformck,  tb,    tbclrull }, // cell left rule
  { ilong,    2, 57,10, tformck,  tb,    tbclrulr }, // cell right rule
  { ilong,    2, 57,11, tmargck,  tb,    tbclmart }, // cell top margin
  { ilong,    2, 57,12, tmargck,  tb,    tbclmarb }, // cell bottom margin
  { ilong,    2, 57,13, tmargck,  tb,    tbclmarl }, // cell left margin
  { ilong,    2, 57,14, tmargck,  tb,    tbclmarr }, // cell right margin
  { ilong,    2, 57,15, tmargck,  tb,    tbclmrtt }, // cell top marg mode
  { ilong,    2, 57,16, tmargck,  tb,    tbclmrtb }, // cell bottom marg mode
  { ilong,    2, 57,17, tmargck,  tb,    tbclmrtl }, // cell left marg mode
  { ilong,    2, 57,18, tmargck,  tb,    tbclmrtr }, // cell right marg mode
  { imnem,    2, 57,19, formchk,  tx,    tbclvalg }, // cell vert align

  { imnem,    2, 58, 1, tformck,  tb,    tbtplace }, // table title place
  { ilong,    2, 58, 2, tformck,  tb,    tbtgap },   // table title gap
  { group,    2, 58, 3, tbltits,  tb,    tbltitl },  // table title start
  { endgroup, 2, 58, 3, tbltite,  tb,    tbetitl },  // table title end

  { group,    2, 60, 0, conddef,  tx,    ignore },   // condition def start
  { endgroup, 2, 60, 0, condproc, tx,    ignore },   // condition def end
  { ename,    2, 61, 1, condnam,  tx,    ignore },   // condition name
  { group,    2, 61, 2, condind,  tx,    ignore },   // condition indicators
  { imnem,    2, 61, 3, condvis,  tx,    ignore },   // condition visibility
  { imnem,    2, 61, 4, condfcol, tx,    ignore },   // condition fgcolor
  { ibyte,    2, 61, 4, condfcol, tx,    ignore },   // condition fgcolor
  { imnem,    2, 61, 5, condbcol, tx,    ignore },   // condition bgcolor
  { ibyte,    2, 61, 5, condbcol, tx,    ignore },   // condition bgcolor
  { imnem,    2, 61, 6, condchcol, tx,   ignore },   // condition chbar color
  { ibyte,    2, 61, 6, condchcol, tx,   ignore },   // condition chbar color
  { etext,    2, 61, 7, condcchr, tx,    ignore },   // condition chbar chars
  { etext,    2, 61, 8, condstim, tx,    ignore },   // condition start image
  { etext,    2, 61, 9, condstalt, tx,   ignore },   // condition start alt
  { etext,    2, 61,10, condenim, tx,    ignore },   // condition end image
  { etext,    2, 61,11, condenalt, tx,   ignore },   // condition end
  { etext,    2, 61,12, nothing,  tx,    condclass },// condition class
  { imnem,    2, 62, 0, condset,  tx,    conduse },  // condition start
  { imnem,    2, 63, 0, condres,  tx,    condend },  // condition end
  { imnem,    2, 64, 1, conddis,  tx,    ignore },   // condition display
  { imnem,    2, 65, 0, condrset, tb,    condruse }, // condition row use

  { group,    2, 70, 1, nothing,  an,    ignore },   // footnote defaults
  { group,    2, 71, 0, ftnstart, an,    putftn },   // footnote start
  { endgroup, 2, 71, 0, ftnend,   an,    endftn },   // footnote end
  { imnem,    2, 72, 1, ftnform,  an,    ignore },   // footnote format
  { imnem,    2, 72, 6, nothing,  an,    setftn },   // footnote marker

  { imnem,    2, 75, 1, tinset,   tx,    tistart },  // inset start
  { ename,    2, 75, 2, tiname,   tx,    ignore },   // inset name
  { ename,    2, 75, 3, tifile,   tx,    ignore },   // inset file name
  { imnem,    2, 75, 8, tinend,   tx,    tiend  },   // inset end
  { imnem,    2, 77, 1, tiflow,   tx,    ignore },   // inset flow props
  { imnem,    2, 77, 2, timflw,   tx,    ignore },   // inset main flow
  { imnem,    2, 77, 3, tipgsp,   tx,    ignore },   // inset page space
  { ename,    2, 77, 4, tiftag,   tx,    ignore },   // inset flow tag

  { group,    2, 80, 0, formdef,  tx,    ignore },   // format def start
  { endgroup, 2, 80, 0, formend,  tx,    ignore },   // format def end
  { ename,    2, 81, 1, formname, tx,    ignore },   // format name
  { imnem,    2, 81, 2, formtype, tx,    ignore },   // format type
  { group,    2, 81, 3, formpfx,  tx,    ignore },   // format prefix group
  { endgroup, 2, 81, 3, formpfe,  tx,    ignore },   // format prefix end
  { group,    2, 81, 4, formsfx,  tx,    ignore },   // format suffix group
  { endgroup, 2, 81, 4, formsfe,  tx,    ignore },   // format suffix end

  { imnem,    2, 82, 1, formscan, tx,    formcall }, // format init
  { imnem,    2, 82, 2, formscch, tx,    formch },   // format inherit
  { imnem,    2, 82, 3, tblform,  tb,    formtbl },  // format table
  { group,    2, 83, 1, formchk,  tx,    formpx },   // format text prefix group
  { endgroup, 2, 83, 1, formchk,  tx,    formrest }, // format text prefix end
  { group,    2, 83, 2, formchk,  tx,    formsx },   // format text suffix group
  { endgroup, 2, 83, 2, formchk,  tx,    formrest }, // format text suffix end
  { ename,    2, 84, 0, formtag,  tx,    ftaguse },  // format tag
  { etext,    2, 85, 0, formspc,  tx,    ftagspc },  // format special

  { group,    2, 95, 0, xrffnum,  an,    ignore },   // ref format def
  { etext,    2, 96, 1, xrfname,  an,    ignore },   // ref format name
  { group,    2, 96, 4, nothing,  tx,    ignore },   // ref text group
  { etext,    2, 96, 4, nothing,  tx,    puttx },    // ref text string
  { ename,    2, 96, 5, nothing,  an,    setfile },  // ref file name
  { etext,    2, 96, 5, nothing,  an,    setfile },  // ref file name
  { etext,    2, 96, 6, nothing,  an,    settitle }, // title for link
  { group,    2, 98, 0, nothing,  an,    ignore },   // ref_cont wrapper group
  { endgroup, 2, 98, 0, nothing,  an,    ignore },   // ref_cont wrapper end

  { group,    2,100, 0, anumdef,  an,    ignore },   // anum def
  { endgroup, 2,100, 0, endanum,  an,    ignore },   // anum def end
  { imnem,    2,101, 2, anumtype, an,    ignore },   // anum def type
  { group,    2,101, 4, anform,   an,    ignore },   // anum format start
  { endgroup, 2,101, 4, anfend,   an,    ignore },   // anum format end
  { group,    2,101, 5, anrform,  an,    ignore },   // anum ref-form start
  { endgroup, 2,101, 5, anrfend,  an,    ignore },   // anum ref-form end
  { group,    2,101, 8, formchk,  an,    anstart },  // anum value start
  { endgroup, 2,101, 8, formchk,  an,    anend },    // anum value end
  { group,    2,101, 9, ansyms,   an,    ignore },   // anum symbols start
  { endgroup, 2,101, 9, anesyms,  an,    ignore },   // anum symbols end
  { imnem,    2,102, 0, anvtype,  an,    ignore },   // anum value type
  { ename,    2,103, 0, anumtxt,  an,    ignore },   // starting anum val
  { ilong,    2,103, 0, anumset,  an,    ignore },   // starting anum val

  { group,    2,110, 1, formchk,  an,    idxgrp },   // index token start
  { endgroup, 2,110, 1, formchk,  an,    idxend },   // index token end
  { imnem,    2, 97,40, nothing,  an,    idxtyp },   // index type ref

	{ etext,    2,110, 3, setmark,  an,    markuse },  // list marker

  { group,    2, 10, 7, formchk,  an,    begxref },  // xref text group
  { etext,    2, 96, 2, formchk,  an,    putxref },  // cross-ref ahref
  { ilong,    2, 96, 2, formchk,  an,    putxref },  // cross-ref ahref
  { imnem,    2, 96, 3, nothing,  an,    xrfform },  // cross-ref format
  { endgroup, 2, 10, 7, formchk,  an,    endxref },  // end of link text
	{ etext,    2,110, 4, formchk,  an,    setxref },  // cross-ref aname
	{ ilong,    2,110, 4, formchk,  an,    setxref },  // cross-ref aname

	{ etext,    2,110, 5, formchk,  an,    sethref },  // old hyper marker
	{ etext,    2,110, 6, formchk,  an,    alink },    // mapped subject marker
	{ ilong,    2,112, 1, nothing,  an,    hypopt },   // hypertext option
	{ etext,    2,112, 2, formchk,  an,    hyploc },   // hypertext location
	{ ilong,    2,112, 2, formchk,  an,    hypunq },   // unique location
	{ etext,    2,112, 3, formchk,  an,    hypfile },  // filename for link
	{ etext,    2,112, 4, formchk,  an,    hyplnk },   // gotolink
	{ ilong,    2,112, 4, formchk,  an,    hypobj },   // gotoobjectid unique
	{ etext,    2,112, 5, formchk,  an,    hyplnk },   // openlink
	{ ilong,    2,112, 5, formchk,  an,    hypobj },   // openobjectid unique
	{ etext,    2,112, 6, formchk,  an,    hypall },   // alert and misc
	{ etext,    2,112, 7, formchk,  an,    hypurl },   // message url
	{ etext,    2,112, 8, formchk,  an,    hypmac },   // HTML macro
	{ etext,    2,112,10, formchk,  an,    hyploc },   // alternate location
	{ ilong,    2,112,11, formchk,  an,    hyppag },   // gotopage
	{ ilong,    2,112,12, formchk,  an,    hyppgn },   // page number
	{ etext,    2,112,15, formchk,  an,    hyptarg },  // target window
	{ etext,    2,112,16, formchk,  an,    hypcsh },   // topic alias

	{ group,    2,113, 1, ukeyset,  tx,    ignore },   // uDoc output key info
	{ endgroup, 2,113, 1, ukeyeset, tx,    ignore },   // uDoc output key info
	{ etext,    2,113, 2, ukeyout,  tx,    ignore },   // uDoc output type
	{ etext,    2,113, 3, ukeylib,  tx,    ignore },   // uDoc output lib path
	{ group,    2,113, 4, ukeyitm,  tx,    ignore },   // uDoc source keys
	{ endgroup, 2,113, 4, ukeyeitm, tx,    ignore },   // uDoc source keys
	{ etext,    2,113, 5, ukeykeys, tx,    ignore },   // uDoc output key
	{ etext,    2,113, 6, ukeysrc,  tx,    ignore },   // uDoc output key src
	{ etext,    2,113, 7, ukeyid,   tx,    ignore },   // uDoc output lib id
	{ etext,    2,113, 8, ukeyiref, tx,    ignore },   // uDoc output key idref

	{ imnem,    2,117, 2, idxchk,   tx,    idxref },   // generated index
	{ ilong,    2,117, 2, nothing,  tx,    idxref },   // generated index
	{ imnem,    2,118, 0, nothing,  tx,    idxprp },   // generated index

	{ imnem,    2,122, 0, mapref,   tx,    ignore },   // map topic/head
	{ imnem,    2,122, 3, mapdsc,   tx,    ignore },   // map description
	{ imnem,    2,122, 4, relref,   tx,    ignore },   // rel topic
	{ imnem,    2,122, 5, reldsc,   tx,    ignore },   // rel description

  { group,    3, 10, 0, pgdef,    gr,    ignore },   // page start
  { endgroup, 3, 10, 0, pgend,    gr,    ignore },   // page end
  { imnem,    3, 11, 0, pgattr,   gr,    ignore },   // page attribute
  { elong,    3, 12, 1, pgsize,   gr,    ignore },   // page size
  { group,    3, 20, 0, frdef,    gr,    ignore },   // frame start
  { endgroup, 3, 20, 0, fredef,   gr,    ignore },   // frame end
  { imnem,    3, 21, 0, frattr,   gr,    ignore },   // frame attributes
  { ilong,    3, 21, 0, frattr,   gr,    ignore },   // frame attributes
  { elong,    3, 22, 0, frsize,   gr,    ignore },   // frame size
  { imnem,    3, 23, 0, franch,   gr,    ignore },   // frame anchor
  { imnem,    3, 24, 0, frline,   gr,    ignore },   // frame border
  { imnem,    3, 25, 0, frpen,    gr,    ignore },   // frame border
  { imnem,    3, 26, 0, frfill,   gr,    ignore },   // frame fill
  { ename,    3, 30, 0, frprev,   gr,    ignore },   // frame preview
  { group,    3, 35, 1, frprop,   gr,    ignore },   // frame prop start
  { ename,    3, 36, 1, frpname,  gr,    ignore },   // frame prop name
  { ename,    3, 36, 2, frpval,   gr,    ignore },   // frame prop val
  { endgroup, 3, 35, 1, freprop,  gr,    ignore },   // frame prop end
  { imnem,    3, 40, 0, frtext,   gr,    ignore },   // layout text

  { ename,    4, 52, 1, eqform,   gr,    ignore },   // equation MathFullForm
  { ename,    4, 52, 2, eqfile,   gr,    ignore },   // equation file

  { ename,    1, 50, 3, grfile,   gr,    ignore },   // named graphic


  { group,    4,  1, 0, grgroup,  gr,    ignore },   // graphic group start
  { endgroup, 4,  1, 0, gregroup, gr,    ignore },   // graphic group end
  { group,    4, 10, 0, grobj,    gr,    ignore },   // graph obj start
  { endgroup, 4, 10, 0, greobj,   gr,    ignore },   // graph obj end
  { elong,    4, 40, 0, grtext,   gr,    ignore },   // graphic text
#ifdef UNUSED
  { imnem,    4, 20, 0, grline,   gr,    ignore },   // line props
  { imnem,    4, 20, 2, ruleprop, tb,    ignore },   // line color
  { ilong,    4, 20, 3, ruleprop, tb,    ignore },   // line thickness
  { ilong,    4, 20, 5, ruleprop, tb,    ignore },   // line count
  { imnem,    4, 21, 0, grpen,    gr,    ignore },   // pen props
  { imnem,    4, 22, 0, grfill,   gr,    ignore },   // fill props
  { elong,    4, 30, 0, grshape,  gr,    ignore },   // vector shape
  { elong,    4, 61, 0, rasprop,  rfm,   ignore },   // raster props
  { ebyte,    4, 60, 0, rasbmap,  rfm,   ignore },   // raster bitmap
  { ebyte,    4, 62, 0, rascmap,  rfm,   ignore },   // raster colormap
#endif
  { ilong,    4, 63, 1, frdpi,    gr,    ignore },   // Frame dpi value
  { ibyte,    4, 80, 0, colrdef,  tb,    ignore },   // color definition
  { ename,    4, 80, 0, colrname, tb,    ignore },   // color name

  { group,    4, 90, 0, nothing,  gr,    object },   // object start
  { endgroup, 4, 90, 0, nothing,  gr,    putobj },   // object end
  { imnem,    4, 91, 0, nothing,  gr,    objiprop }, // object props
  { ilong,    4, 91, 0, nothing,  gr,    objiprop }, // object props
  { ename,    4, 91, 0, nothing,  gr,    objtprop }, // object props
  { etext,    4, 91, 0, nothing,  gr,    objtprop }, // object props
  { group,    4, 92, 1, nothing,  gr,    objparam }, // object param
  { endgroup, 4, 92, 1, nothing,  gr,    objeparm }, // obj param end
  { etext,    4, 92, 2, nothing,  gr,    objnparm }, // obj param name
  { etext,    4, 92, 3, nothing,  gr,    objvparm }, // obj param val
  { imnem,    4, 92, 4, nothing,  gr,    objtparm }, // obj param type

  { group,    4, 95, 0, imagemap, gr,    ignore },   // image map start
  { endgroup, 4, 95, 0, endimap,  gr,    ignore },   // image map end
  { ename,    4, 96, 0, imapprop, gr,    ignore },   // image map props
  { imnem,    4, 96, 0, imapprop, gr,    ignore },   // image map props
  { group,    4, 97, 0, maparea,  gr,    ignore },   // map area start
  { endgroup, 4, 97, 0, endmapa,  gr,    ignore },   // map area end
  { etext,    4, 98, 0, mapaprop, gr,    ignore },   // map area props
  { elong,    4, 99, 0, mapshape, gr,    ignore },   // map area shape
};
size_t HTMwr::HTMsize;
DCvlist *HTMwr::HTMlist[HtmListMax];


HTMgroup HTMwr::BaseGroup;
HTMgroup *HTMwr::GroupStack[LevelMax];
uns HTMwr::DCLlevel;
bool HTMwr::SkipCurrGroup;
uns HTMwr::SkipLevel;


void
HTMwr::ReadDCL(DCLrfile *rf)
{
  HTMctl *cp;

  SetUpHTM();  // prepare the search array
  GroupStack[0] = &BaseGroup;

  while (!rf->eof()) {
    cp = new HTMctl;
    *cp = *rf;

    if (cp->dt() == endgroup) {
      if (DCLlevel > 0)
        DCLlevel--;
			if (SkipCurrGroup) {
				if (SkipLevel > DCLlevel)
	        SkipCurrGroup = false;
        continue;
      }
    }

		if (SkipCurrGroup) {
			if (SkipLevel <= DCLlevel)
				continue;
    }

    FindHTM(cp);            // sets cp->rp to point at matching htm struct
    if (cp->rp == NULL) {   // or to NULL if no match
      if (cp->dt() == group)
        rf->skipGroup();
      delete cp;
      continue;
    }

    if (cp->dt() == group) {
      DCLlevel++;
      if (DCLlevel < LevelMax)
        GroupStack[DCLlevel] = GroupStack[DCLlevel - 1];
    }

    switch((cp->rp)->module) {
      case none:      // do not process, just save on group
        break;

      case omit:      // discard the CTL (but not its group members)
        continue;

      case top:
        if (ProcTop(cp))
          break;
        else continue;

      case tx:
        if (ProcTx(cp))
          break;
        else continue;

      case an:
        if (ProcAn(cp))
          break;
        else continue;

      case tb:
        if (ProcTb(cp))
          break;
        else continue;

      case gr:
        if (ProcGr(cp))
          break;
        else continue;

#ifdef UNUSED
      case rfm:
        if (ProcRf(cp))
          break;
        else continue;
#endif

      default:
        assert(0);
				LogEvent(logdebug, 1, "Invalid case in ReadDCL()");
        return;
    }

    if (DCLlevel < LevelMax)
      GroupStack[DCLlevel]->add(*cp);
  }
}


void
HTMwr::SetUpHTM(void)
{
  size_t i;
  htm *rtp;
  uns idx;
  DCvlist *dtlist;

  HTMsize = sizeof(HTMpile) / sizeof(htm);
  for (i = 0, rtp = HTMpile; i < HTMsize; i++, rtp++) {
    idx = ((rtp->ctlmaj - 1) * MinorMax) + rtp->ctlmin;
    if (idx >= HtmListMax)
      continue;
    if (HTMlist[idx] == NULL)
      HTMlist[idx] = new DCvlist;
    if ((dtlist = (DCvlist *) HTMlist[idx]->find(rtp->ctldat + 1)) == NULL) {
      dtlist = new DCvlist;
      HTMlist[idx]->add(dtlist, rtp->ctldat + 1);
    }
    if (dtlist->find(rtp->ctldt + 1) == NULL)
      dtlist->add(rtp, rtp->ctldt + 1);
  }
}


void
HTMwr::FindHTM(HTMctl *cp)
{
  uns idx;
  DCvlist *dlist, *dtlist;

  idx = ((cp->maj() - 1) * MinorMax) + cp->min();
  if ((idx < HtmListMax)
   && ((dlist = (DCvlist *) HTMlist[idx]) != NULL)) {
    if (((dtlist = (DCvlist *) dlist->find(cp->dat() + 1)) != NULL)
     || ((dtlist = (DCvlist *) dlist->find(1)) != NULL)) {
      if ((cp->rp = (htm *) dtlist->find(cp->dt() + 1)) != NULL)
        return;
      if (cp->dt() == imnem) { // fuzzy match
        if ((cp->rp = (htm *) dtlist->find(ilong + 1)) != NULL)
          return;
      }
      else if (cp->dt() == ilong) {
        if ((cp->rp = (htm *) dtlist->find(imnem + 1)) != NULL)
          return;
      }
    }
  }
  cp->rp = NULL;
}


// configuration options


// for now, all false; eventually provide command-line switches to change them
bool HTMwr::CharStyles;  // true for best Word effect, false for WP
bool HTMwr::UseLang;     // true causes WP to lose mind



// process switch


bool
HTMwr::ProcTop(HTMctl *cp)
{
  switch((cp->rp)->scan_op) {
    case nothing:   // just put with its group for later
      break;

    case pass:      // ignore, don't save 
      return false;

		case dbgscan:   // place to set breakpoint
			if ((cp->siz() & 1) != 1)
				break;
#ifdef _DEBUG
			__asm int 3
#endif
			break;

		case srcver:
			if (cp->dat() == 6)    // Frame
				MIFVer = cp->siz();  // single-digit MIF version
			break;

		case drname:
			DRmifVer = (char *) cp->ex();
			{
				char *cpt = FilterVer;
				FilterVer = NewName(strlen(cpt) + strlen(DRmifVer) + strlen(DWhtmVer) + 3);
				sprintf(FilterVer, "%s %s %s", cpt, DRmifVer, DWhtmVer);
			}
			break;

		case ispart:
			RefFileType = cp->siz() ? 'B' : 'F';
			break;

    case unitset:   // set units sizes
      SetUnits(cp);
      break;

    default:
      assert(0);
			LogEvent(logdebug, 1, "Invalid case in ProcTop()");
      return false;
  }
  return true;
}



// HTM writing phase

uns HTMwr::HTMLVersion = 4;
bool HTMwr::UseFrameSet = false;
unc HTMwr::GeneratorTag = 'C';  // Comment, Meta, None

char *HTMwr::PrologDType2 = {"\"-//IETF//DTD HTML 2.0//EN\""};
char *HTMwr::PrologDType3 = {"\"-//W3C//DTD HTML 3.2 Final//EN\""};
char *HTMwr::PrologDType4 = {"\"-//W3C//DTD HTML 4.01 Transitional//EN\""};
char *HTMwr::PrologDType4F = {"\"-//W3C//DTD HTML 4.01 Frameset//EN\""};
char *HTMwr::PrologDType = PrologDType4;
bool HTMwr::PrologDTypeSet = false;

char *HTMwr::PrologDTD4 = {"  \"http://www.w3.org/TR/1999/REC-html401-19991224/loose.dtd\""};
char *HTMwr::PrologDTD4F = {"  \"http://www.w3.org/TR/1999/REC-html401-19991224/frameset.dtd\""};
char *HTMwr::PrologDTD = NULL;
bool HTMwr::PrologDTDSet = false;

bool HTMwr::Xhtml = false;
bool HTMwr::Xml = false;
bool HTMwr::UseDOCTYPE = true;
char *HTMwr::PrologDTypeX = {"\"-//W3C//DTD XHTML 1.0 Transitional//EN\""};
char *HTMwr::PrologDTDX = {"  \"http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd\""};
char *HTMwr::XhtmlDefNS = {"http://www.w3.org/1999/xhtml"};
char *HTMwr::XhtmlNS = NULL;;
char *HTMwr::XhtmlLang = {"en"};
char *HTMwr::XHLangAttr = {"xml:lang"};

bool HTMwr::UseXMLDeclaration = true;
char *HTMwr::XMLVer = {"1.0"};
char *HTMwr::XMLEncoding = {"UTF-8"};
bool HTMwr::QuotedEncoding = false;
bool HTMwr::UseXMLRoot = true;
char *HTMwr::XMLRoot = {"html"};
char *HTMwr::ContentType = "text/html";
bool HTMwr::UseHeadAndBody = true;
bool HTMwr::BodyContentOnly = false;
bool HTMwr::UTF8 = false;
bool HTMwr::NoEnc = false;
bool HTMwr::NumericCharRefs = true;


bool
HTMwr::WriteHTM(HTMwfile *wf)
{
	uns i = 0;
	bool hascontent = false;

  for (i = 1; i <= NextSectNum; i++) {
		if (SectUsedList.find(i)
		 && !SectSkipList.find(i)) {
			hascontent = true;
			break;
		}
  }

	if (!hascontent) {  // nothing to write
		LogEvent(logwarn, 1, "No content in output file, not written");
		return false;
	}

	XrefFilePrefix = CurrFilePrefix;
	XrefFile = NULL;
	FileStarted = false;

	if (HXMwr::DITA) {
		HXMwr::ProcDITATopicVars(); // for last topic during scan
		HXMwr::DITATopicCount = HXMwr::DITATopicNum;
		HXMwr::DITATopicNum = 0;
		HXMwr::DITAFileStart = true;
	}

	SplitID = 0;
	ParaRefNum = StartingParaRef;
	FootnoteNum = 0;
	FirstPart = true;
	LastPart = SplittingFile ? false : true;

	ProcNameTitleMacros(wf);
	MarkList.all(GetMetaAttrs);
	if (UseNavtitleMarkers)
		ProcContentsTitles();

	if (HXMwr::DITA) {
		if (HXMwr::DITATypeNamePrefixUsed)
			HXMwr::SetTopicTypePrefixes(wf);
		HXMwr::DITASetTopicInfo();
	}

	// was at end of scanning phase

	CurrConds = NULL;

	//IntRefCount = IntRefList.count();
	//IntRefNum = 0;

	if (RefFile)
		IntRefList.all(ProcIntRef);

	if (!CurrXFileTitle)
		CurrXFileTitle = Title;

	if (!CurrFSTitle)
		CurrFSTitle = Title;

	if (!HXMwr::DITAMapTitle
	 && (Title != DefTitle))
		HXMwr::DITAMapTitle = Title;

	ProcSplitRefInfo();

	EvalTopicStartMacros(wf, 1);

	if (!BodyContentOnly) {
		PutInsertMacro(wf, "BeginFile");
		WriteProlog(wf, 1);
		if (!HXMwr::DITA)
			PutInsertMacro(wf, "Entities");

		if (UseHeadAndBody) {
			if (UseDITAIndex
			 && !SplittingFile)
				WriteHead(wf, WriteIndexCSS);
			else
				WriteHead(wf);
			PutInsertMacro(wf, "FirstFrames");

			wf->startBlock("body");
			if (FilterConditions)
				wf->putAttrStr(FilterBarBodyAttr);
			WriteAttrib(wf, "body");
			wf->endElem(true);

			WriteBaseProps(wf);
			if (UseDITAIndex
			 && !SplittingFile)
				WriteIndexJS(wf);
		}
	}

	if (!HXMwr::DITA) {
		PutInsertMacro(wf, SplittingFile ? "FirstTop" : "NonsplitTop");
		if (!MinimizeLines)
			wf->putEnd();
	}

  // put out sections, each with headers and footers as needed
  for (i = 1; i <= NextSectNum; i++) {
     if (SectUsedList.find(i)
		  && !SectSkipList.find(i))
       WriteFlow(wf, (uns) SectList.find(i));
  }

	// EndAllNestedXmlBlocks(wf);

	// for DITA, write all closing divs here ****
	if (HXMwr::HXML)
		HXMwr::EndAllDITATags(wf);

	if (Extracting)
		EndFileExtract(wf);

	FinishFtns(wf, true);

	ParaRefNum = SplittingFile ? StartSplitParaRef : StartingParaRef;

	PutInsertMacro(wf, SplittingFile ? "LastBottom" : "NonsplitBottom");
	PutScript(wf);
	PutObjects(wf, true);

	if (!BodyContentOnly) {
		if (UseHeadAndBody) {
			wf->putEnd();
			wf->endBlock("body");  // wrap it up
		}
		PutInsertMacro(wf, "LastEnd");

		if (HXMwr::DITA) {
			if (HXMwr::NestTopicFiles
			 && !HXMwr::SplitTopicFiles) {
				while (HXMwr::LastXMLLevel) {
					wf->endBlock((char *) HXMwr::XMLCloseTags.find(HXMwr::LastXMLLevel--));
					wf->putEnd();
				}
			}
			else {
				wf->endBlock(XMLRoot);
				wf->putEnd();
			}
			if (HXMwr::DITAComposite)
				wf->endBlock("dita");
		}
		else if (UseXMLRoot)
			wf->endBlock(XMLRoot);
	}
	if (!MinimizeLines)
		wf->putEnd();

	SetSplitRefInfo();
	PutExtRefInfo(wf);

	if (FMSource)
		PutGraphGenInfo();

	if (CssFile) {
		WriteCssTableFormats();
		if (FmtFile
		 && FmtFile->Section("Required"))
			DefineRequiredFormats(FmtFile);
		WriteIndexFileFormats();
		WriteWingSymFormats();
		if (IniFile
		 && IniFile->Section("CSSEndMacro")) {
			char *txt = NULL;
			CssFile->putCr();
			DCmacr::WriteMacro((HTMwfile *) CssFile, txt = IniFile->SecCont(true));
			DeleteName(txt);
		}
		if (UseConditionalFlagging
		 && AppendFlagsFile)
			WriteFlagsCSSFile();
		CssFile->CloseFile();
	}

	if (UseConditionalFlagging
	 && (!AppendFlagsFile
	  || !CssFile))
		WriteFlagsCSSFile();

	if (WriteSpacerFile)
		WriteSpacerImage();

	if (WriteMadeWithFile)
		WriteMadeWithImage();

	if (DropDownUsed)
		WriteDropLinkIcons();

	if ((HelpForm ||
	  (UseIndex
		 && SplittingFile
	   && !HXMwr::HXML))
	 && (ContentsUsed
	  || IndexRefCount
	  || ALinkCount
	  || OmniHelp
	  || Eclipse)) 
		WriteReferenceFiles(wf);

	// for DITA, write map info here ****
	if (HXMwr::DITA) {
		if (WriteVarFile
		 && (DITAVarType == 'C'))
			WriteVarElementsFile(wf);			
		HXMwr::WriteDITAMap(wf);
	}

	FixupFileRefs();

	if (WriteDropJSFile)
		WriteDropDownJSFile();

	WriteTemplatePages();

	if (MXLwr::WriteKeydefLib) {
		if (!MXLwr::OutputKeydefLibSrc) {
			char *CurrDir = _getcwd(NewName(1024), 1024);
			char *libsrc = NewName(CurrDir, strlen(CurrDir) + 8);
			strcat(libsrc, "/_wrap/");
			// for OH, add project file name and a #  *****
			MXLwr::SetOutputKeydefLibProps(libsrc);
		}
		DCnlist *dcn = &MXLwr::SourceKeydefs;
		char *keys = NULL;
		char *src = NULL;
		char *out = NULL;
		while ((dcn = dcn->next) != NULL) {
			keys = (char *) dcn->name;
			src = (char *) dcn->id;
			out = GetFinalFileName(src, MXLwr::UseKeydefAliases);
			MXLwr::SetOutputKeydef(keys, out);
		}
		MXLwr::WriteOutputKeydefLib();
	}

	if (UseCodePage
	 || UnicodeFTS)
		DCicu::CloseCodePage();

	if (CopyOriginalGraphics
	 && GraphCopyCount) {
		if (XMLSource
		 && UsePartFiles)
			PutGraphCopyInfo();
		else if (!XMLSource
		 || !DoAutomation)
			DCauto::CopyOriginalGraphicsProc(&GraphFileCopyList, &GraphFileFixedCopyList,
			                                 GraphCopyCount, GraphPath);
		else {
			DCauto::OriginalGraphics = &GraphFileCopyList;
			DCauto::OriginalFixedGraphics = &GraphFileFixedCopyList;
			DCauto::OriginalGraphicsCount = GraphCopyCount;
		}
	}

	if (DoAutomation) {
		if (XMLSource
	   && !UsePartFiles) {
			char *bname = NewName(DCLFile->getFileName());
			DCLFile->CloseFile();
			wf->CloseFile();
			DCauto::WrapShipProc(bname);
			DeleteName(bname);
		}
		else {
			DCLFile->CloseFile();
			wf->CloseFile();
		}
	}

	if (UseLog) {
		LogEvent(loginfo, 1, "Processing completed by dwhtm.");
		Log->CloseFile();
		CheckLogErrors();
	}

	return true;
}


char *
HTMwr::GetFinalFileName(char *src, bool alias)
{
	if (!src)
		return "missing";

	char *slash = strrchr(src, '\\');
	if (!slash)
		slash = strrchr(src, '/');
	char *out = NewName(slash ? slash + 1 : src);
	char *outid = NULL;
	char *hash = strrchr(out, '#');
	if (hash) { // has added id
		*hash = '\0';
		outid = MakeOutID(out, hash + 1);
	}
	else
		outid = MakeOutID(out);
	char *sid = NULL;
	DCpfile::getval("IntRefs", outid, "", RefBuf, RefBufLen, RefFileName);

	if (*RefBuf) {
		sid = NewName(RefBuf);

		// if alias, return the first CSH alias for the file
		if (alias) {
			char *oname = NULL;
			if ((oname = MXLwr::KeydefAliases.find(atol(sid))) != NULL) {
				DeleteName(out);
				DeleteName(sid);
				return oname;
			}
		}

	// else return the new file name containing
	// the src file content, with a # if needed
		char *ofile = NULL;
		if ((ofile = GetSplitFileName(sid)) != NULL) {
			DeleteName(out);
			DeleteName(sid);
			return ofile;
		}

		DeleteName(sid);
	}

	// if all else fails, return the src file name w/o path

	return out;
}


char *
HTMwr::MakeOutID(char *out, char *id)
{
	char *outid = NULL;
	if (id)
		outid = NewName("u", strlen(out) + strlen(id) + 5);
	else
		outid = NewName("u", (strlen(out) * 2) + 5);
	char *dot = strrchr(out, '.');
	if (dot) {
		strncat(outid, out, dot - out);
		strcat(outid, dot + 1);
		strcat(outid, "e");
		if (id)
			strcat(outid, id);
		else
			strncat(outid, out, dot - out);
	}
	else {
		strcat(outid, out);
		strcat(outid, "e");
		strcat(outid, id ? id : out);
	}
	return outid;
}


char *
HTMwr::GetSplitFileName(char *sid)
{
	DCpfile::getval("SplitFileNames", sid, "", RefBuf, RefBufLen, RefFileName);
	if (*RefBuf)
		return NewName(RefBuf);
	return NULL;
}

void
HTMwr::CheckLogErrors(void)
{
	if (Log->ErrWarn) {
		if (PMFlag)  // running from plugin or PM
			ReleaseSemaphore(PMFlag, 1L, NULL);  // puts semaphore in signalled state
		else if (ShowLog) { // display log file
#if 0
			char *cmd = NewName(EditorFileName, strlen(EditorFileName) +
																					strlen(LogFileName) + 1);
			strcat(cmd, " ");
			strcat(cmd, LogFileName);
			DCauto::RunDOSCommand(cmd, true, false, false);
#endif
			char *tlog = NewName(LogFileName, strlen(LogFileName) + 4);
			char *dot = strrchr(tlog, '.');
			if (dot)
				strcpy(dot, "htm.tmp");
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


void
HTMwr::WriteProlog(HTMwfile *wf, long sid)
{
	if (HXMwr::uDoc) {
		wf->startBlock(XMLRoot);
		return;
	}

	if (WriteCssLink
	 && !CssBrowserDetect
	 && !CssFullPath
	 && CssName
	 && CssPath && *CssPath) {
		size_t len = strlen(CssPath);
		CssFullPath = NewName(len + strlen(CssName) + 2);
		strcpy(CssFullPath, CssPath);
		if (CssFullPath[len - 1] != '/')
			CssFullPath[len] = '/';
		strcat(CssFullPath, CssName);
	}
	else
		CssPath = NULL;

	if (UseConditionalFlagging
	 && CSSFlagsFile
	 && ReferenceFlagsFile
	 && CssPath && *CssPath) {
		size_t len = strlen(CssPath);
		FlagsFullPath = NewName(len + strlen(CSSFlagsFile) + 2);
		strcpy(FlagsFullPath, CssPath);
		if (FlagsFullPath[len - 1] != '/')
			FlagsFullPath[len] = '/';
		strcat(FlagsFullPath, CSSFlagsFile);
	}

	if (Xhtml) {
		if (UseXMLDeclaration)
			wf->putXML(XMLEncoding, XMLVer);
		if (!PrologDTypeSet || !PrologDType)
			PrologDType = PrologDTypeX;
		if (!PrologDTDSet || !PrologDTD)
			PrologDTD = PrologDTDX;
	}
	else if (Xml) {
		wf->putXML(XMLEncoding, XMLVer);
		if (!PrologDTypeSet || !PrologDType)
			PrologDType = NULL;
		// <?xml:stylesheet href="local.css" type="text/css" charset="UTF-8"?>
		if (WriteCssLink) {
			if (CssBrowserDetect)
				DCmacr::WriteMacro(wf, CssBrowserDetect);
			else {
				wf->startPI("xml-stylesheet");
				wf->strAttr("charset", "UTF-8");
				wf->strAttr("href", CssFullPath ? CssFullPath : CssName);
				wf->strAttr("type", "text/css");
				wf->endPI();
				wf->putEnd();
			}
		}
		if (UseConditionalFlagging
		 && CSSFlagsFile
		 && ReferenceFlagsFile) {
				wf->startPI("xml-stylesheet");
				wf->strAttr("charset", "UTF-8");
				wf->strAttr("href", FlagsFullPath ? FlagsFullPath : CSSFlagsFile);
				wf->strAttr("type", "text/css");
				wf->endPI();
				wf->putEnd();
		}
	}
	else {
		if (!PrologDType)
			PrologDType = UseFrameSet ? PrologDType4F : PrologDType4;
		if (PrologDTD
		 && !stricmp(PrologDTD, PrologDTD4)
		 && UseFrameSet)
			PrologDTD = PrologDTD4F;
	}

	if (!HXMwr::DITA) {
		if (UseDOCTYPE) {
			//if (HXMwr::DocBook)
			//	SetDocBookProlog();
			if (HTMLVersion == 5)
				wf->putStr("<!DOCTYPE html>\n");
			else
				wf->putDT(XMLRoot, PrologDType, PrologDTD, !HXMwr::HXML); 
		}
		if (UseXMLRoot)
			wf->startBlock(XMLRoot);
		FinishProlog(wf);
	}
}


void
HTMwr::FinishProlog(HTMwfile *wf)
{
	if (UseXMLRoot
	 && !HXMwr::DITAComposite) {
		if (Xml || Xhtml) {
			if (XhtmlNS)
				wf->strAttr("xmlns", XhtmlNS);
			else if (Xhtml)
				wf->strAttr("xmlns", XhtmlDefNS);
			wf->strAttr(XHLangAttr, XhtmlLang);
		}
		if (Xhtml)
			wf->strAttr("lang", XhtmlLang);
		wf->endElem(true);
		wf->putEnd();
	}

	if (GeneratorTag == 'C') {
		wf->startComment();
		wf->putStr("generated by ");
		wf->putStr(FilterVer);
		wf->endComment();
	}

	wf->putEnd();
}

char *HTMwr::AttrBody = NULL;
char *HTMwr::AttrTable = NULL;
char *HTMwr::AttrRow = NULL;
char *HTMwr::AttrCellHead = NULL;
char *HTMwr::AttrCellBody = NULL;

void
HTMwr::WriteAttrib(HTMwfile *wf, char *aname)
{
	if (!aname)
		return;

	char *attr = NULL;
	if (!stricmp(aname, "body"))
		attr = AttrBody;
	else if (!stricmp(aname, "table"))
		attr = AttrTable;
	else if (!stricmp(aname, "row"))
		attr = AttrRow;
	else if (!stricmp(aname, "tr"))
		attr = AttrRow;
	else if (!stricmp(aname, "th"))
		attr = AttrCellHead;
	else if (!stricmp(aname, "td"))
		attr = AttrCellBody;

	if (attr)
		wf->putAttrStr(DCmacr::GetMacro(attr));

#if 0
	if (IniFile
	 && IniFile->Section("Attributes")
	 && IniFile->Find(aname)) {
		//wf->putChar(' ');
		//DCmacr::WriteMacro(wf, IniFile->StrRight());
		wf->putAttrStr(DCmacr::GetMacro(IniFile->StrRight()));
	}
	//if (OmniHelp && !stricmp(aname, "body"))
	//	wf->putStr(" onload=\"loaded()\"");
#endif
}


bool HTMwr::InTextRun = false;

void
HTMctl::write(DCfile& wf)
{
  frame *fr;

  if (rp == NULL)
    return;

	bool wasTextRun = HTMwr::InTextRun;
	HTMwr::InTextRun = false;

  if (rp->write_op == puttbl)
    HTMwr::ProcTbl( (HTMwfile *) &wf, this);
  else if (rp->write_op == endtbl)
    HTMwr::WriteTbl( (HTMwfile *) &wf);
  else if (HTMwr::InTbl)
    HTMwr::ProcTblProp((HTMwfile *) &wf, this);
	else switch (rp->write_op) {
		case dbgwrite:         // place to set a breakpoint
			if ((this->siz() & 2) != 2)
				break;
#ifdef _DEBUG
			__asm int 3
#endif
			break;
		case putfile:  // add contents of named file as text here
			HTMwr::WriteFromExtFile( (HTMwfile *) &wf, this);
			break;
		case flnum:
			HTMwr::WriteFlowNum( (HTMwfile *) &wf, this);
			break;
		case flend:
			//if (HTMwr::InsertIndexTOC)
			//	HTMwr::InsertIndexTOCItem((HTMwfile *) &wf);
			break;
		case puttbr:
			HTMwr::PutInsertMacro((HTMwfile *) &wf, "TopicBreak");
			break;

		case puttx:
			if (wasTextRun)
				HTMwr::InTextRun = true;
			HTMwr::WriteText( (HTMwfile *) &wf, this);
			if (!HTMwr::InIdxGroup)
				HTMwr::InTextRun = true;
			break;
		case parstart:
			HTMwr::InTextRun = false;
			HTMwr::StartPara( (HTMwfile *) &wf, this);
			break;
		case parend:
			HTMwr::InTextRun = false;
			HTMwr::EndPara( (HTMwfile *) &wf);
			break;
		case linend:
			if (wasTextRun)
				HTMwr::InTextRun = true;
			HTMwr::EndLine( (HTMwfile *) &wf, this);
			break;

		case conduse:
			HTMwr::CondStart(this->dat());
			break;
		case condend:
			HTMwr::CondEnd(this->dat());
			break;

		case condclass:
			if (!HTMwr::CurrCondClass)
				HTMwr::CurrCondClass = (char *) this->ex();
			else {
				char *str = NewName(HTMwr::CurrCondClass,
					                  strlen(HTMwr::CurrCondClass) + this->siz() + 2);
				strcat(str, " ");
				strcat(str, (char *) this->ex());
				HTMwr::CurrCondClass = str;
			}
			break;

#if 0
		case mapuse:
			HTMwr::UseMapDesc(this);
			break;
#endif

		case putvar:
			HTMwr::PutVarForm( (HTMwfile *) &wf, this);
			break;
		case putavar:
			HTMwr::PutVarArrItemForm( (HTMwfile *) &wf, this);
			break;
		case putvsys:
			HTMwr::PutSystemVar( (HTMwfile *) &wf, this);
			break;
		case putfr:
	    //if (((fr = (frame *) HTMwr::FrameList.find(this->dat())) != NULL)
			// && ((fr->fpos < 2) || HTMwr::InTbCell)) {
	    if (((fr = (frame *) HTMwr::FrameList.find(this->dat())) != NULL)
			 && (HTMwr::ParaRefNum != HTMwr::PRefFrameList(fr))
			 && (HTMwr::ParaRefNum != HTMwr::PRuninFrameList(fr))) {
				HTMwr::ImageRefNum = (long) this;
			  HTMwr::WriteFrame( (HTMwfile *) &wf, fr);
			}
			break;


		case object:
			HTMwr::StartObject( (HTMwfile *) &wf, this);
			break;
		case objiprop:
			HTMwr::SetObjectIntProp( (HTMwfile *) &wf, this);
			break;
		case objtprop:
			HTMwr::SetObjectTextProp( (HTMwfile *) &wf, this);
			break;
		case objparam:
			HTMwr::StartObjectParam( (HTMwfile *) &wf, this);
			break;
		case objeparm:
			HTMwr::EndObjectParam( (HTMwfile *) &wf, this);
			break;
		case objnparm:
			HTMwr::SetObjectParamName( (HTMwfile *) &wf, (char *) this->ex());
			break;
		case objvparm:
			HTMwr::SetObjectParamVal( (HTMwfile *) &wf, (char *) this->ex());
			break;
		case objtparm:
			HTMwr::SetObjectParamType( (HTMwfile *) &wf, this->siz());
			break;

		case putobj:
			HTMwr::PutObject( (HTMwfile *) &wf, this);
			break;

		case sethref:
			HTMwr::PutHypertext( (HTMwfile *) &wf, this);
			break;
		case hyploc:
			HTMwr::PutHyperLoc( (HTMwfile *) &wf, this);
			break;
		case hypcsh:
			HTMwr::PutTopicAlias( (HTMwfile *) &wf, this);
			break;
		case hyptarg:
			HTMwr::PutHyperTarget(this);
			break;
		case hypunq:
		case hyppgn:
			HTMwr::PutHyperUnique( (HTMwfile *) &wf, this);
			break;
		case hypfile:
			//HTMwr::ProcHyperFile(this);
			break;
		case hypmac:
			HTMwr::PutHyperMacro( (HTMwfile *) &wf, this);
			break;
		case xrfform:
			HTMwr::ProcXrefFormatUse(this);
			break;
		case begxref:
			HTMwr::StartXrefText( (HTMwfile *) &wf, this);
			break;
		case putxref:
			HTMwr::XrefLinkIDNext = false;
			HTMwr::StartAnchor( (HTMwfile *) &wf, this);
			break;
		case setxref:
			HTMwr::PutXrefAName( (HTMwfile *) &wf, this);
			break;
		case endxref:
			HTMwr::EndAnchor( (HTMwfile *) &wf);
			break;
		case idxgrp:
			//HTMwr::InTextRun = false;
			HTMwr::WriteText( (HTMwfile *) &wf, NULL);
			HTMwr::InIdxGroup = true;
			HTMwr::IdxSeeAlso = false;
			HTMwr::StartingIdxGroup = true;
			HTMwr::InTextRun = false;
			HTMwr::SetCondIndex( (long) this);
			break;
		case idxend:
			//HTMwr::IdxEnded = true;
			HTMwr::InIdxGroup = false;
			HTMwr::StartingIdxGroup = false;
			HTMwr::InTextRun = false;
			break;
		case idxtyp:
			HTMwr::ProcIdxType(this);
			break;
		case markuse:
			HTMwr::WriteMarker( (HTMwfile *) &wf, this);
			break;
		case idxref:
			HTMwr::InsertIndexTOCItem( (HTMwfile *) &wf, this);
			break;
		case idxprp:
			HTMwr::SetIXListProps(this);
			break;

		case formch:
			if ((size
			  && !HTMwr::InPara)
			 || (!size
			  && (data == 2)
			  && HTMwr::InPara
			  && !HTMwr::InIdxGroup)) {
				if (wasTextRun
				 || ((HTMwr::CharBlockState == CbStart)
				  && !size
				  && (data == 2)))
					HTMwr::InTextRun = true;
				HTMwr::WriteText( (HTMwfile *) &wf, NULL);
			}
			if (!HTMwr::InPara)
				break;
			// fall through
		case formcall:
			//if (HTMwr::InHyper)
			//	HTMwr::EndAHref( (HTMwfile *) &wf);
			if (HTMwr::InIdxGroup)
				break;
      HTMwr::UseForm((HTMwfile *) &wf, this);
			HTMwr::InTextRun = false;
			break;
    case putftn:
			HTMwr::SaveCondStateFtn(false);
			HTMwr::WriteFtnStart((HTMwfile *) &wf);
			break;
    case endftn:
			HTMwr::WriteFtnEnd((HTMwfile *) &wf);
			HTMwr::RestoreCondStateFtn();
			break;

		case tistart:
			HTMwr::StartTextInset( (HTMwfile *) &wf, this);
			break;
		case tiend:
			HTMwr::EndTextInset( (HTMwfile *) &wf, this);
			break;

		case formpx:
		case formsx:
			HTMwr::InFormPx = true;
		case anstart:
			HTMwr::PutAnumMacro((HTMwfile *) &wf, true);
			break;
		case formrest:
			HTMwr::InFormPx = false;
		case anend:
			HTMwr::PutAnumMacro((HTMwfile *) &wf, false);
			break;

		default:
			HTMwr::SetCurrProps((HTMwfile *) &wf, this);
			break;
	}
}



// utility functions


pprop HTMwr::SavedStateStack[SavedStateMax];
pprop HTMwr::PFStateStack[SavedStateMax];
tinfo HTMwr::SavedTabStack[SavedStateMax];
uns HTMwr::SavedLevel;


void
HTMwr::SaveState(void)
{
  if (++SavedLevel < SavedStateMax) {
    SavedStateStack[SavedLevel - 1] = Curr;
    PFStateStack[SavedLevel - 1] = ParaForm;
    SavedTabStack[SavedLevel - 1] = *CurrTab;
	}
}


void
HTMwr::RestoreState(void)
{
  // Last = Curr;
  if ((SavedLevel > 0)
   && (--SavedLevel < SavedStateMax)) {
    Curr = SavedStateStack[SavedLevel];
    ParaForm = PFStateStack[SavedLevel];
    *CurrTab = SavedTabStack[SavedLevel];
	}
}



// get base name from path or URL


bool
HTMwr::GetBaseName(char *name, char *&base, bool ext)
{
	if (!name)
		return false;

	char *nn = NULL;

	// extract base as part from last slash to last dot
	char *ref = NewName(name);
	char *last = ref + strlen(ref) - 1;
	if ((*last == '/')
	 || (*last == '\\'))
		*last = '\0';

	char *slash = strrchr(ref, '/');
	char *bslash = strrchr(ref, '\\');
	if (bslash > slash)
		slash = bslash + 1;
	else if (slash)
		slash++;
	else
		slash = ref;

	char *dot = NULL;
	char *dot2 = NULL;
	if (!ext)
		dot = strrchr(slash, '.');
	if (dot == NULL)
		nn = NewName(slash);
	else {
		*dot = '\0';
		dot2 = strrchr(slash, '.');
		if (dot2 == NULL)
			nn = NewName(slash);
		else
			nn = NewName(dot2 + 1);
	}

	DeleteName(ref);

	if (!strcmp(nn, name)) {
		DeleteName(nn);
		return false;
	}

	base = nn;
	return true;
}



// convert units


uns HTMwr::UnitMul[UnitsMax] = {  72,   72,  2,  72 };
uns HTMwr::UnitDiv[UnitsMax] = { 500, 1270, 10, 336 };

uns HTMwr::UBaseMul[UnitBasesMax] = {  72,   72,  2,  72 };
uns HTMwr::UBaseDiv[UnitBasesMax] = { 500, 1270, 10, 336 };


void
HTMwr::SetUnits(HTMctl *cp)
{
  uns ut = cp->min() - 10;
  if (ut >= UnitsMax)
    return;
  uns uu = cp->dat() - 1;
  if (uu >= UnitBasesMax)
    return;

  long uvmul = UBaseMul[uu];
  long uvdiv = UBaseDiv[uu];
  long uscale = cp->siz();
  if ((uscale != 0) && (uscale != 1000)) {
    if (uscale < 1000)
      uvdiv = (uvdiv * 1000) / uscale;
    else if (uscale > 1000)
      uvmul = (uvmul * (uscale / 100)) / 10;
  }
  UnitMul[ut] = (uns) uvmul;
  UnitDiv[ut] = (uns) uvdiv;
}


short
HTMwr::GetTwips(long val, unittype ut)
{
  short sign = 1;
  if (val < 0) {
    sign = -1;
    val = -val;
  }

  if (ut >= UnitsMax)
    ut = (unittype) 0;
  uns umul = UnitMul[ut];
  uns udiv = UnitDiv[ut];

  return ((((val * umul) + (udiv / 2)) / udiv) * sign);
}


long
HTMwr::GetOrig(long val, unittype ut)
{
  short sign = 1;
  if (val < 0) {
    sign = -1;
    val = -val;
  }

  if (ut >= UnitsMax)
    ut = (unittype) 0;
  uns umul = UnitMul[ut];
  uns udiv = UnitDiv[ut];

  return ((((val * udiv) + (umul / 2)) / umul) * sign);
}


short HTMwr::ConversionDPI = 96;

short
HTMwr::PixelsFromTwips(short twips)
{
	long val = (long) twips;
	return (short) ((val * ConversionDPI) / 1440L);
}


// end of dwhtm.cpp


