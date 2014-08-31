 
// dwrtf.cc is the code for dcl Microsoft RTF writing, JHG
// Copyright (c) 1993-2005 by Omni Systems, Inc.  All rights reserved.

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
#include "dwrtf.h"
#include "dwrtffl.h"
#include "dcicu.h"
#include <windows.h>
#undef min


// start of code
char *RTFwr::FilterVer = { "DCL filter dwrtf, Ver 4.1" };
char *RTFwr::DRmifVer = { "m000" };
char *RTFwr::DWrtfVer = { "r299c" };
long RTFwr::MIFVer = 0;
char *RTFwr::ICUdll = NULL;

swlist RTFwr::SwitchList[] = {
  { "F", NULL, RTFwr::SetFormat, true },
  { "f", NULL, RTFwr::SetFormat, true },
  { NULL, NULL }
};
bool RTFwr::FormatSet = false;
bool RTFwr::ListFile = false;
char RTFwr::ListType = 'B';  //Contents or Index or Both

short RTFwr::Verbose = 0;
DCirfile *RTFwr::IniFile = NULL;
char RTFwr::OldFormType = '8';
char *RTFwr::MacroFile = NULL;
DCfrfile *RTFwr::FmtFile = NULL;
DCfrfile *RTFwr::PagesFile = NULL;
DCfrfile *RTFwr::TablesFile = NULL;
DCfrfile *RTFwr::SubFormatsFile = NULL;
DCwfile *RTFwr::CntFile = NULL;
uns RTFwr::SourceType = 0;
bool RTFwr::XMLSource = false;
bool RTFwr::UMXL = false;
bool RTFwr::FMSource = false;
char *RTFwr::CurrGrxFile = NULL;
char *RTFwr::OutputFormat = NULL;
char *RTFwr::IniFileName = NULL;

outtyp OutputType = otnone;
bool RTFwr::DoAutomation = false;
bool RTFwr::EndingAuto = false;
bool RTFwr::BookAuto = false;
DCLrfile *RTFwr::DCLFile = NULL;
bool RTFwr::LogIniChains = false;


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

  DCapp app(dw, ".rtf");

  app.Init(argc, argv);

  if (!app.Proc(RTFwr::SwitchList)) {
    fprintf(stderr, "usage: %s [-v] [-f form] [-o rtf_file] dcl_file\n", argv[0]);
    exit(dc_err_arg);
  }

  RTFwr::Verbose = app.verbose();

	if (RTFwr::ListFile) {  // entire different task
		return dc_ok;
	}
	if (RTFwr::EndingAuto) {  // another separate task
		DCirfile *inihlpf	= NULL; // looks in current directory
		char *ifname = "mif2rtf.ini";
		if (RTFwr::IniFileName
		 && ((inihlpf = new DCirfile(RTFwr::IniFileName)) != NULL)
		 && (inihlpf->FErr() == fok)
		 && (inihlpf->Ferr == fok))
			RTFwr::IniFile = inihlpf;  // make accessible everywhere
		else if (RTFwr::IniFileName
		 && ((*(RTFwr::IniFileName + 1) = 'x') != '\0')
		 && ((inihlpf = new DCirfile(RTFwr::IniFileName)) != NULL)
		 && (inihlpf->FErr() == fok)
		 && (inihlpf->Ferr == fok)) {
			RTFwr::XMLSource = true;
			RTFwr::UMXL = true;
			RTFwr::IniFile = inihlpf;  // make accessible everywhere
		}
		else if (RTFwr::IniFileName
		 && ((*(RTFwr::IniFileName + 1) = 'd') != '\0')
		 && ((inihlpf = new DCirfile(RTFwr::IniFileName)) != NULL)
		 && (inihlpf->FErr() == fok)
		 && (inihlpf->Ferr == fok)) {
			RTFwr::XMLSource = true;
			RTFwr::IniFile = inihlpf;  // make accessible everywhere
		}
		else if ((!RTFwr::IniFileName || stricmp(RTFwr::IniFileName, ifname))
		 && ((inihlpf = new DCirfile(ifname)) != NULL)
		 && (inihlpf->FErr() == fok))
			RTFwr::IniFile = inihlpf;
		else
			return dc_err_ini;
		RTFwr::FMSource = true;
		RTFwr::InitCurrProps(NULL);
		inihlpf->CloseFile();
		return dc_ok;
	}


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
	RTFwr::DCLFile = &rf;
  if (rf.FErr() != fok)
    rf.Err();

	RTFctl *cp = new RTFctl;
	*cp = rf;  // read first ctl, should be source ID

	char *iname = "dwrtf.ini";

	if ((cp->dt() == stamp)
	 && (cp->maj() == 1)
	 && (cp->min() == 21)) {
		switch (RTFwr::SourceType = cp->dat()) {
			case 5:  // xml
				RTFwr::XMLSource = true;
				iname = "xml2rtf.ini";
				if (RTFwr::IniFileName)
					*(RTFwr::IniFileName + 1) = 'd';
				break;
			case 6:  // frame
				RTFwr::FMSource = true;
				iname = "mif2rtf.ini";
				break;
			case 24: // gpo
				iname = "gpo2rtf.ini";
				break;
			case 25:  // DITA
				RTFwr::XMLSource = true;
				iname = "dita2rtf.ini";
				if (RTFwr::IniFileName)
					*(RTFwr::IniFileName + 1) = 'd';
				break;
			case 26:  // uDoc
				RTFwr::XMLSource = true;
				RTFwr::UMXL = true;
				iname = "udoc2rtf.ini";
				if (RTFwr::IniFileName)
					*(RTFwr::IniFileName + 1) = 'x';
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
  RTFwfile wf(slash ? slash + 1 : wfname);
  if (wf.FErr() != fok)
    wf.Err();

	DCirfile *inif = NULL;	// in current directory
	if (RTFwr::IniFileName
	 && ((inif = new DCirfile(RTFwr::IniFileName, &wf)) != NULL)
	 && (inif->FErr() == fok)
	 && (inif->Ferr == fok))
		RTFwr::IniFile = inif;  // make accessible everywhere
	else if (RTFwr::XMLSource
	 && RTFwr::IniFileName
	 && ((*(RTFwr::IniFileName + 1) = 'x') != '\0')
	 && ((inif = new DCirfile(RTFwr::IniFileName, wfname)) != NULL)
	 && (inif->FErr() == fok)
	 && (inif->Ferr == fok)) {
		RTFwr::UMXL = true;
		RTFwr::IniFile = inif;  // make accessible everywhere
	}
	else if (RTFwr::IniFileName  // see if previous convention
	 && ((inif = new DCirfile(RTFwr::IniFileName + 1, &wf)) != NULL)
	 && (inif->FErr() == fok)
	 && (inif->Ferr == fok)) {
		RTFwr::IniFileName++;   // remove the leading underscore
		RTFwr::IniFile = inif;  // make accessible everywhere
	}
	else if ((!RTFwr::IniFileName
	  || stricmp(RTFwr::IniFileName, iname))
	 && ((inif = new DCirfile(iname, &wf)) != NULL)
	 && (inif->FErr() == fok))
		RTFwr::IniFile = inif;
	else
		return dc_err_ini;

	if (!RTFwr::InitCurrProps(&wf)) {  // OnlyAuto done
		rf.CloseFile();
		inif->CloseFile();
	  return dc_ok;
	}

  RTFwr::ReadDCL(&rf);   // read entire dcl file
  RTFwr::WriteRTF(&wf);   // put out the rtf file

	DCpfile::pflushall();  // done with grx file

	inif->CloseFile();	// no need to check ini file for errors
	if (!RTFwr::DoAutomation) { // which already closed them
		rf.CloseFile();
	  wf.CloseFile();
	}
  rf.Err();      // these two just return if no err
  wf.Err();      // otherwise they exit with the err (and a message)
  return dc_ok;  // so if we get here, it worked
}


void
RTFwr::SetFormat(char *fmt)
{
	// fmt can be either "help" or "word"
	FormatSet = true;  // overrules .ini
	HelpForm = true;
	OldFormType = toupper(*fmt);
	char lt = '\0';
	IniFileName = DCirfile::GetIniName("_m2", fmt, &OldFormType, &lt);

	switch (lt) {
		case 'B':
		case 'C':
		case 'I':
			ListFile = true;
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
	char ch = toupper(*fmt);
	if (strlen(fmt) > 2) {
		if (!strnicmp(fmt, "winhelp", 7))
			ch = '4';
		else if (!strnicmp(fmt, "wordperfect", 11))
			ch = 'P';
		else if (!strnicmp(fmt, "word", 4))
			ch = '8';
		else if (!strnicmp(fmt, "rtf", 3))
			ch = '8';
		else
			ch = 'Z';
	}
#endif

	switch (OldFormType) {
		case '7':
			HelpForm = false;
			HelpType = 'S';
			Word8 = false;
			WordPerf = false;
			WordVerSet = true;
			OutputType = otrtf;
			//strcat(IniFileName, "rtf");
			break;
		case '8':
		case 'W':
			HelpForm = false;
			HelpType = 'S';
			Word8 = true;
			WordPerf = false;
			WordVerSet = true;
			OutputType = otrtf;
			//strcat(IniFileName, "rtf");
			break;
		case 'P':
			HelpForm = false;
			HelpType = 'S';
			Word8 = false;
			WordPerf = true;
			WordVerSet = true;
			OutputType = otrtf;
			//strcat(IniFileName, "rtf");
			break;
		case '3':  // WinHelp3
			HlpVer = '3';
			HlpVerSet = true;
			HelpType = 'H';
			OutputType = otwh;
			//strcat(IniFileName, "winhelp");
			break;
		case '4':  // WinHelp4
			HlpVer = '4';
			HlpVerSet = true;
			HelpType = 'H';
			OutputType = otwh;
			//strcat(IniFileName, "winhelp");
			break;
		case 'H':  // WinHelp
			HelpType = 'H';
			OutputType = otwh;
			//strcat(IniFileName, "winhelp");
			break;
#if 0
		case 'J':  // JavaHelp
		case 'M':  // MS HTML Help
		case 'G':  // Generic HTML (via Akira)
			HelpType = ch;
			break;
#endif

		case 'A':
			HelpForm = false;
			HelpType = 'S';
			OutputType = otdcl;
			break;
		case 'F':
			HelpForm = false;
			HelpType = 'S';
			OutputType = otmif;
			break;

		default:  // unknown
			HelpForm = false;
			FormatSet = false;
			break;
	}

	//strcat(IniFileName, ".ini");
}


HANDLE PMFlag = NULL;


// initialization phase



bool
RTFwr::InitCurrProps(RTFwfile *wf)
{
	char *fname = NULL;

  Curr.fsize = 22;
  Curr.lspace = 240;
  Curr.lang = 1033;
	Curr.fenc = 0;
	Curr.cpg = 0;
  Curr.lmode = 1;
  Curr.colr = 2;
  Curr.bcolr = 9;
	Curr.fwid = 144;
  CurrTab = new tinfo;

	if (XMLSource) {
		LogIniPrefix = UMXL ? "_x2" : "_d2";
		LogFileName = NewName(LogIniPrefix, strlen(LogIniPrefix) + 10);
		strcat(LogFileName, "g_log.txt");
		//LogFileName = UMXL ? "_x2g_log.txt" : "_d2g_log.txt";
		CharFormsNest = true;
		ShowLog = true;
	}

	if (wf) {
		CurrFileName = wf->getBaseName();
		CurrFilePath = wf->getFilePath();
		size_t cfpend = strlen(CurrFilePath) - 1;
		if ((CurrFilePath[cfpend] == '\\')
		 || (CurrFilePath[cfpend] == '/'))
			CurrFilePath[cfpend] = '\0';

		CurrFileFull = wf->getFileName();
		CurrPath = wf->getBasePath();
	}

	if (IniFile)
		IniFile = IniFile->SetConfigTemplate("Templates", "Configs");

	DCini::InitIniModule(IniFile, RTFIniSects,
	 (struct spprop *) &Curr, (struct tbprop *) &CTbl, &GraphIni,
	 &TblIni, &XrefIni, &XrefName, &Curr.stag);

	if (IniFile
	 && IniFile->Section("Options")) {	// read ini-interpretation info
		if (IniFile->Find("NoNameDel"))
			::NoNameDel = IniFile->BoolRight();
		if (IniFile->Find("NoMemDel"))
			::NoMemDel = IniFile->BoolRight();
	}

	if (HelpForm)
		UseFrameIX = false;

	DCini::ProcAllSects();

	if (ICUdll)
		DCicu::ICUdll = ICUdll;

	if (UseLog) {
		PMFlag = OpenSemaphore(SEMAPHORE_ALL_ACCESS, FALSE,
			 (XMLSource ? "d2gpm" : "m2gpl"));

		Log = new DClog(LogFileName, "dwrtf", DWrtfVer, LogLevels);
		if (Log->FErr() != fok)
			UseLog = false;
		LogFileName = Log->getFilePath();
	}
	DCirfile::LogIniChain = LogIniChains;
	DCirfile::LogEventFunc = (logfunc) LogEvent;
	DCicu::LogEventFunc = (logfunc) LogEvent;
	DCauto::LogEventFunc = (logfunc) LogEvent;
	DCauto::LogFileName = LogFileName;

	if (XMLSource
	 || EndingAuto
	 || CopyOriginalGraphics) {
		DCauto::LogEventFunc = (logfunc) LogEvent;
		DoAutomation = DCauto::SetUpAutomation(OutputType, IniFile);

		if (!DoAutomation
		 && DCauto::Compiler)
			DoAutomation = true;
	}

	if (wf
	 && UseLog)
		Log->logEvent(loginfo, 1, "Opened file ", wf->getFilePath());

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
			LogEvent(loginfo, 1, "Automation processing completed by dwrtf.");
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
			DCfrfile::FmtOutputType = fmoutrtf;
			FmtFile = FmtFile->SetConfigTemplate("Templates", "Formats");
			DCfrfile::Formats = FmtFile;
			if (FmtFile->Section("Setup")) {
				if (FmtFile->Find("MergeCurrFormatProps"))
					MergeCurrFormatProps = FmtFile->BoolRight();
			}
			if (FmtFile->Section("BaseValues")) {
				FormatBaseSet = true;
				if (FmtFile->Find("BodyBaseHeight"))
					FmtFile->BodyBaseSize = FmtFile->StrRight();
				if (FmtFile->Find("BodyBaseWidth"))
					FmtFile->IndentBaseWidth = FmtFile->StrRight();
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


	if (PrjFileName)
		SetNamePath(PrjFileName, &CurrPrjName, &CurrPrjPath);


	if (XMLSource) { 
		if (!Template && !FmtFile) {
			XML = true;
			LogEvent(logwarn, 1,
			 "Word template or format file required for XML input, missing");
		}
	}
	SetDefaultFont();

	if (FormatSet)     // main option set from the command line
		OutType = HelpForm ? 'H' : 'S';
	else if (IniFile   // read main option from ini
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
			case 'M':
				SubType = "HH";
				break;
			case 'G':
				SubType = "HTML";
				break;
			case 'H': // WinHelp
			default:
				break;
		}
		if (SubType)
			IniFile->SetType(SubType);
	}

	IdxType = XrfType = QuoType = BulType = OutType;
	if (OutType == 'H') {
		HelpForm = true;
		FootType = 'J';
		PageType = 'R';
		HFType = 'N';
		MetaType = 'E';
		TbTitleType = 1;  // title at top
		TbRuleType = 'N'; // no rules
		RemStyles = true;
		RemFonts = true;
		CntType = 'F';
		RuninType = 'N';
		ULineOver = false;
		KeepSectBreaks = false;
		FirstTabSet = true;
		TextColor = false;
		BackColor = false;
		MergeStradCells = true;
	}

	DCini::ProcIniGroup("OptionsSet");
	DCini::ProcIniGroup("GraphicsSet");
	DCini::ProcIniGroup("TableSet");

	if (IniFile
	 && IniFile->Section("GraphLineStyles")) {	// read mapping
		uns lsty = 0;
		for (uns sty = 1; sty < (GrStyleMax - 1); sty++) {
			if (sty == 7) // no remapping white
				continue;
			if (IniFile->Find(sty)) {
				lsty = (uns) IniFile->IntRight();
				if (lsty < 6)
					GrStyleMap[sty + 1] = lsty;
			}
		}
	}

	strncpy(DefaultFont.fname, GrDefFont, WmfFNameMax);
	DefaultFont.fhigh = GrDefFSize;

	// read overrides for Word or WinHelp now
	if (IniFile) {
		if (HelpForm == true) {
			SetXRFLists("HelpXrefFiles");
			DCini::ProcIniGroup("HelpSet");
			if (IniFile->Section("BrowseStart")  // read file-specific info
			 && IniFile->Find(CurrFileName))
				BrowseCurr = IniFile->IntRight();

			if (IniFile->Section("BrowsePrefix")  // read file-specific info
			 && IniFile->Find(CurrFileName))
				BrowsePrefix = IniFile->StrRight();

			DCini::ProcIniSect("Setup");  // to override FileSuffix elsewhere
			if (XrefFileSuffix == NULL)
				XrefFileSuffix = "hlp";
			CurrHelpBase = GetXrefName(CurrFileName);
			CurrHelpFile = NewName(strlen(CurrHelpBase)
			                     + strlen(XrefFileSuffix) + 2);
			strcpy(CurrHelpFile, CurrHelpBase);
			strcat(CurrHelpFile, ".");
			strcat(CurrHelpFile, XrefFileSuffix);
		}
		else { // for non-help Word
			SetXRFLists("WordXrefFiles");
			if (IniFile->Section("WordSectionFiles")
			 && IniFile->Find(CurrFileName))
				SectionPrefix = IniFile->StrRight();
			CurrHelpBase = GetXrefName(CurrFileName);
			DCini::ProcIniGroup("WordSet");
			DCini::ProcIniSect("Setup");  // to override FileSuffix elsewhere
			if (XrefFileSuffix == NULL)
				XrefFileSuffix = "rtf";
			switch (XrfType) {
				case 'F':  // deprecated
				case 'H':  // not for Word RTF
				case 'S':
				default:
					XrfType = 'S';  // former default
					break;
				case 'N':
					break;
			}
		}
		//if (*XrefFileSuffix == '.')
		//	XrefFileSuffix++;
	}

	SetFIDLists();
	CurrFilePrefix = GetFIDPrefix(CurrFileName);
	XrefFilePrefix = CurrFilePrefix;

	if (FMSource) {
		CurrGrxFile = NewName("./", strlen(CurrFileName) + 7);
		strcat(CurrGrxFile, CurrFileName);
		strcat(CurrGrxFile, ".grx");
		ClearGraphFDK();
	}

	// finished reading, determine consequences

	if (HelpForm == true) {  // mandatory overrides
		TblFillShade = false;
		if (FootType == 'V')
			FootType = 'J';
		InitFileFtn = true;	  // put context string of filename at start
		if (TbRuleType == 'C')
			TbRuleType = 'N';
		if (SideHType == 'F')
			SideHType = 'L';
		if (HlpVer == '3') {   // ensure 3.1 compatible settings
			TransparentBitmaps = false;
			QuoType = 'H';
			BulType = 'H';
			NoDing = true;
		}
		CharStylesUsed = false;
		if (XMLSource)
			WriteHPJFile();
	}

	if ((FootType == 'J')   // jumps or popups for footnotes
	 || (FootType == 'P')) {
		if (OutType != 'H')
			FootType = 'S';
		else {
			FootDiv = wf->NewDiv();  // reserve first one
			wf->SelectDiv(0);  // but don't use it yet
			FootnoteNum = 0;
		}
	}

	if (Word8 && !Word2003)
		MetaMetric = true;

	if (TransparentBitmaps == false) {
		BmcStr[3] = BmlStr[3] = BmrStr[3] = ' ';  // trim off the t
		BmcStr[4] = BmlStr[4] = BmrStr[4] = '\0';
	}

	if (WordPerf == true) {  // get rid of all frames
		CharStyles = false;
		UseLang = false;
		if (HFType != 'N') {
			HFType = 'T';
			FrameHF = false;
		}
		if (SideHType == 'F')
			SideHType = 'L';
	}

	if (CharStyles == false)
		CharStylesUsed = false;

	if (SingleSect) {
		TextFlow = new RTFgroup;
		SkippedFlow = new RTFgroup;
	}

	if (Altura) {
		HelpSectBreaks = false;
		BareFnSym = true;
	}

	DCmacr::InitMacroModule(wf, IniFile,
	 MacroFile,
	 WriteMacroChar, WritePredefinedMacro,
	 WriteCodeStoreVar, GetCodeStoreVar,
	 GetTextStore,
	 GetPredefinedVar, GetFrameUserVar);

	// restore inimod globals after macromod init
	DCini::InitIniModule(IniFile, RTFIniSects);

	if (CntType != 'N')
		SetUpCnt();

	return true;
}


void
RTFwr::LogEvent(logtype lt, unc sev, char *desc,
 char *d2, char *d3, char *d4, char *d5)
{
	if (!UseLog)
		return;

	Log->logEvent(lt, sev, desc, d2, d3, d4, d5);
}


void
RTFwr::SetNamePath(const char *pathname, char **oname, char **opath)
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
RTFwr::SetUpCnt(void)
{
	short len = 0;
	char *cname = NULL;

	if ((CntName == NULL)
	 || (CntType == 'B')) {
		len = strlen(CurrFileName);
		cname = NewName(len + 5);
		strcpy(cname, CurrFileName);
	}
	else {
		if (((cname = strrchr(CntName, '.')) != NULL)
		 && !stricmp(cname, ".cnt"))
			*cname = '\0';
		len = strlen(CntName);
		cname = NewName(len + 5);
		strcpy(cname, CntName);
	}

	if (CntType == 'B')
		strcat(cname, ".bct");
	else
		strcat(cname, ".cnt");

	CntFile = new DCwfile(cname, ftxt);
	if (CntFile->FErr() != fok) {
		CntFile = NULL;
		CntType = 'N';
		return;
	}

	if (CntType == 'F') {  // write commands
		if (CntStartFile) {  // copy contents into .cnt
			DCrfile *CntSFile = new DCrfile(CntStartFile);
			DCblock *Sblock = new DCblock;
			*Sblock = *CntSFile;
			*CntFile = *Sblock;
			CntSFile->CloseFile();
		}
		else {
			CntFile->putStr(":BASE ");
			if (CntBase)
				CntFile->putStr(CntBase);
			else {
				CntFile->putStr(CurrFileName);
				CntFile->putStr(".hlp");
			}
			CntFile->putCr();
			if (CntTitle) {
				CntFile->putStr(":TITLE ");
				CntFile->putStr(CntTitle);
				CntFile->putCr();
			}
		}
		if (CntTopHead) {
			CntFile->putStr(CntTopHead);
			CntFile->putCr();
			CntLastLevel = CntLastOrigLevel = *CntTopHead;
			CntLastForm = 1;
		}
	}
	else if (CntType == 'B') {
		if (IniFile
		 && IniFile->Section("BctFileHeads")
		 && IniFile->Find(CurrFileName)
		 && ((CntTopHead = IniFile->StrRight()) != NULL)) {
			CntFile->putStr(CntTopHead);
			CntFile->putCr();
		}
	}	
}



void 
RTFwr::WriteHPJFile(void)
{
	FILE *fini = NULL;

	if (!HPJName) {
		HPJName = NewName(CurrHelpBase, strlen(CurrHelpBase) + 5);
		strcat(HPJName, ".hpj");
	}
	else if (!strstr(HPJName, ".hpj")) {
		char *basename = HPJName;
		HPJName = NewName(basename, strlen(basename) + 5);
		strcat(HPJName, ".hpj");
	}

	if ((fini = fopen(HPJName, "rt")) != NULL) {
		fclose(fini);
		return;
	}

	if (!WriteHelpProjectFile
	 && !DoAutomation)
		return;

	if ((fini = fopen(HPJName, "wt")) == NULL)
		return;

	if (!CntName) {
		CntName = NewName(CurrHelpBase, strlen(CurrHelpBase) + 5);
		strcat(CntName, ".cnt");
	}


	// the exact sequence of entries in the .hpj is critical

	fprintf(fini, "; This file is maintained by HCW.");
	fprintf(fini, " Do not modify this file directly.\n\n");
	fprintf(fini, "\n[OPTIONS]\n");
	fprintf(fini, "COMPRESS=12 Hall Zeck\n");
	fprintf(fini, "LCID=0x409 0x0 0x0 ; U.S. English\n");
	fprintf(fini, "REPORT=Yes\n");
	fprintf(fini, "CONTENTS=%s\n", CntTopic ? CntTopic : CurrHelpBase);
	fprintf(fini, "TITLE=%s\n", CntTitle);
	fprintf(fini, "CNT=.\\%s\n", CntName);
	if (HelpCopyright) {
		fprintf(fini, "COPYRIGHT=%s", HelpCopyright);
		fprintf(fini, "%s\n", HelpCopyDate ? " %date" : "");
	}
	fprintf(fini, "HLP=.\\%s\n", CntBase ? CntBase : CurrHelpFile);

	fprintf(fini, "\n[FILES]\n");
	fprintf(fini, ".\\%s.rtf\n", CurrHelpBase);

	fprintf(fini, "\n[WINDOWS]\n");
	fprintf(fini, "main=%s\n", "\"\",(18,10,600,435),60672,(r14876671),(r12632256),f6");
	fprintf(fini, "proc=%s\n", "\"\",(653,102,360,600),20740,(r14876671),(r12632256),f3");

	fprintf(fini, "\n[CONFIG]\n");
	fprintf(fini, "BrowseButtons()\n");

	fclose(fini);
}



// DCL reading phase


rtf RTFwr::RTFpile[] = {
  { imnem,    1, 10, 0, unitset,  top,   ignore },   // page units
  { imnem,    1, 11, 0, unitset,  top,   ignore },   // lead units
  { imnem,    1, 12, 0, unitset,  top,   ignore },   // font units
  { imnem,    1, 13, 0, unitset,  top,   ignore },   // graph line units

  { ilong,    1, 21, 0, srcver,   top,   ignore },   // source version
  { ename,    1, 22, 1, drname,   top,   ignore },   // reader name

  { group,    1, 40, 1, nothing,  tx,    ignore },   // defaults set
  { endgroup, 1, 40, 1, docdefs,  tx,    ignore },   // all defaults set
  { group,    1, 40, 2, nothing,  tx,    ignore },   // resource set
  { endgroup, 1, 40, 2, docres,   tx,    ignore },   // all formats set

  { ename,    1, 50, 4, txfile,   tx,    putfile },  // named text file

  { etext,    1, 70, 1, dirapp,   top,   exapp },    // directive: appl
  { etext,    1, 70, 2, dirsec,   top,   exsec },    // directive: section
  { etext,    1, 70, 3, dirprop,  top,   exprop },   // directive: prop
  { etext,    1, 70, 4, dirval,   top,   exval },    // directive: value

  { imnem,    1, 80, 1, dbgscan,  top,   dbgwrite }, // debugging

  { group,    2,  1, 0, flowlist, tx,    flnum },    // text segment
  { imnem,    2,  2, 0, flowframe, tx,   frnum },    // frame id for text
  { imnem,    2,  3, 0, flowprev, tx,    ignore },   // prev part for text
  { ename,    2,  6, 1, flowname, tx,    ignore },   // named stream
  { imnem,    2,  5, 0, setanch,  gr,    putfr },    // frame anchor

  { etext,    2, 10, 1, flowused, tx,    puttx },    // text obj lit
  { imnem,    2, 10, 1, flowused, tx,    puttx },    // text obj lit
  { group,    2, 10, 3, anumscan, an,    putanum },  // anum text group

  { group,    2, 10, 4, nothing,  an,    ignore },   // anum ref group
  { group,    2, 10, 5, nothing,  an,    ignore },   // anum pg ref group
  { group,    2, 10, 6, nothing,  an,    ignore },   // gen text group

  { group,    2, 10, 2, nothing,  an,    ignore },   // var text group
  { ilong,    2, 10, 2, varnum,   an,    putvar },   // var text item

  { imnem,    2, 11, 0, formchk,  tx,    putbreak }, // text breaks
  { imnem,    2, 11, 1, formchk,  tx,    parstart }, // paragraph start
  { imnem,    2, 11, 2, formchk,  tx,    parend },   // paragraph end
  { imnem,    2, 11, 3, formchk,  tx,    colend },   // paragraph end
  { imnem,    2, 11, 4, formchk,  tx,    pagend },   // paragraph end
  { imnem,    2, 11,10, formchk,  tx,    linend },   // line end
  { imnem,    2, 12, 2, formchk,  tx,    putkeepn }, // keep with next
  { imnem,    2, 12, 3, formchk,  tx,    putkeep },  // keep together

  { ilong,    2, 14, 0, charset,  tx,    putcset },  // character set

  { ename,    2, 15, 1, fontlist, tx,    putfont },  // named font
  { imnem,    2, 15, 1, fontlist, tx,    putfont },  // numbered font
  { ilong,    2, 15, 2, formchk,  tx,    putfsize }, // font size
  { imnem,    2, 15, 3, formchk,  tx,    putcomp },  // font compression
  { imnem,    2, 15, 4, formchk,  tx,    putbold },  // font bolding
  { imnem,    2, 15, 5, formchk,  tx,    putital },  // font italics
  { imnem,    2, 15, 6, formchk,  tx,    putbase },  // super/subscript
  { imnem,    2, 15, 7, formchk,  tx,    putcaps },  // small/all caps
  { imnem,    2, 15, 8, formchk,  tx,    putstyl },  // shadow/outline
  { imnem,    2, 15, 9, formchk,  tx,    putrevb },  // rev bars, strike
  { imnem,    2, 15,10, formchk,  tx,    putlmod },  // underline mode
  { imnem,    2, 15,11, formchk,  tx,    putline },  // under/overline
  { imnem,    2, 15,13, formchk,  tx,    putcolr },  // text color
  { ilong,    2, 15,21, supsiz,   tx,    ignore },   // superscript size
  { ilong,    2, 15,22, supoff,   tx,    ignore },   // superscript offset
  { ilong,    2, 15,23, subsiz,   tx,    ignore },   // subscript size
  { ilong,    2, 15,24, suboff,   tx,    ignore },   // subscript offset
  { ilong,    2, 15,25, scapsiz,  tx,    ignore },   // small caps size

  { imnem,    2, 17, 2, language, tx,    putlang },  // language

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
  { endgroup, 2, 35, 3, endrunin, tx,    ignore },   // end runin	punct
  { imnem,    2, 36, 1, sideflow, tx,    ignore },   // sidehead flow
  { imnem,    2, 36, 2, sidepos,  tx,    ignore },   // sidehead position
  { ilong,    2, 36, 3, sidewid,  tx,    ignore },   // sidehead width
  { ilong,    2, 36, 4, sidegap,  tx,    ignore },   // sidehead gap

  { group,    2, 40, 0, formchk,  tx,    puttdef },  // tab def start
  { endgroup, 2, 40, 0, formchk,  tx,    endtdef },  // tab def end
  { ilong,    2, 41, 0, formchk,  tx,    puttab },   // tab description
  { ilong,    2, 42, 1, formchk,  tx,    tabfill },  // tab fill char
  { etext,    2, 42, 1, formchk,  tx,    tabfstr },  // tab fill string

  { group,    2, 50, 0, tblstart, tb,    puttbl },   // table start
  { endgroup, 2, 50, 0, tblend,   tb,    endtbl },   // table end
  { group,    2, 51, 2, tformck,  tb,    tbplace },  // table place start
  { endgroup, 2, 51, 2, tformck,  tb,    tbeplac },  // table place end
  { imnem,    2, 51, 3, tformck,  tb,    tblrult },  // table top rule
  { imnem,    2, 51, 4, tformck,  tb,    tblrulb },  // table bottom rule
  { imnem,    2, 51, 5, tformck,  tb,    tblrull },  // table left rule
  { imnem,    2, 51, 6, tformck,  tb,    tblrulr },  // table right rule
  { group,    2, 51,14, nothing,  tb,    tbldflt },  // table defaults start
  { endgroup, 2, 51,14, nothing,  tb,    tbedflt },  // table defaults end

  { imnem,    2, 51, 8, tformck,  tb,    tbatype },  // alt shade row/col
  { ilong,    2, 51, 9, tformck,  tb,    tbapern },  // alt shade normal per
  { ilong,    2, 51,10, tformck,  tb,    tbapera },  // alt shade alt period
  { imnem,    2, 51,11, tformck,  tb,    tbacolr },  // alt color
  { imnem,    2, 51,12, tformck,  tb,    tbafill },  // alt fill pattern

  { group,    2, 52, 0, tformck,  tb,    tbcol },    // column start
  { endgroup, 2, 52, 0, tformck,  tb,    tbcend },   // column end
  { ilong,    2, 53, 2, nothing,  tb,    tbcwid },   // column width
  { elong,    2, 53, 2, nothing,  tb,    tbcwids },  // column widths
  { imnem,    2, 53, 3, tformck,  tb,    tbccolr },  // column color
  { imnem,    2, 53, 4, tformck,  tb,    tbcfill },  // column fill
  { imnem,    2, 53, 5, tformck,  tb,    tbcrull },  // column left rule
  { imnem,    2, 53, 6, tformck,  tb,    tbcrulr },  // column right rule
  { imnem,    2, 53, 7, nothing,  tb,    tbcwtyp },  // column width type

  { group,    2, 54, 0, tformck,  tb,    tbrow },    // row start
  { endgroup, 2, 54, 0, tformck,  tb,    tbrend },   // row end
  { ilong,    2, 55, 2, nothing,  tb,    tbrhigh },  // row height
  { ilong,    2, 55, 3, nothing,  tb,    tbrhfix },  // row fixed height
  { ilong,    2, 55, 4, nothing,  tb,    tbrhmin },  // row min height
  { ilong,    2, 55, 5, nothing,  tb,    tbrhmax },  // row max height
  { imnem,    2, 55, 7, tformck,  tb,    tbrcolr },  // row color
  { imnem,    2, 55, 8, tformck,  tb,    tbrfill },  // row fill pattern
  { imnem,    2, 55, 9, tformck,  tb,    tbrrult },  // row top rule
  { imnem,    2, 55,10, tformck,  tb,    tbrrulb },  // row bottom rule
  { imnem,    2, 55,11, tformck,  tb,    tbrruls },  // row separator rule
  { ilong,    2, 55,12, tformck,  tb,    tbrrper },  // row rule period
  { imnem,    2, 55,13, tformck,  tb,    tbrralt },  // row alt rule

  { group,    2, 56, 0, tbcstart, tb,    tbcell },   // cell start
  { endgroup, 2, 56, 0, endtbc,   tb,    tbclend },  // cell end
  { ilong,    2, 57, 1, tbcstrd,  tb,    tbclstrd }, // cell col straddle
  { ilong,    2, 57, 2, nothing,  tb,    tbclrstr }, // cell row straddle
  { ilong,    2, 57, 3, formchk,  tb,    tbclrotr }, // cell rotation
  { imnem,    2, 57, 5, tformck,  tb,    tbclcolr }, // cell color
  { imnem,    2, 57, 6, tformck,  tb,    tbclfill }, // cell fill
  { ilong,    2, 57, 7, tformck,  tb,    tbclrult }, // cell top rule
  { ilong,    2, 57, 8, tformck,  tb,    tbclrulb }, // cell bottom rule
  { ilong,    2, 57, 9, tformck,  tb,    tbclrull }, // cell left rule
  { ilong,    2, 57,10, tformck,  tb,    tbclrulr }, // cell right rule
  { ilong,    2, 57,11, formchk,  tb,    tbclmart }, // cell top margin
  { ilong,    2, 57,12, formchk,  tb,    tbclmarb }, // cell bottom margin
  { ilong,    2, 57,13, formchk,  tb,    tbclmarl }, // cell left margin
  { ilong,    2, 57,14, formchk,  tb,    tbclmarr }, // cell right margin
  { ilong,    2, 57,15, formchk,  tb,    tbclmrtt }, // cell top marg mode
  { ilong,    2, 57,16, formchk,  tb,    tbclmrtb }, // cell bottom marg mode
  { ilong,    2, 57,17, formchk,  tb,    tbclmrtl }, // cell left marg mode
  { ilong,    2, 57,18, formchk,  tb,    tbclmrtr }, // cell right marg mode
  { imnem,    2, 57,19, formchk,  tb,    tbclvalg }, // cell vert align

  { imnem,    2, 58, 1, tformck,  tb,    tbtplace }, // table title place
  { ilong,    2, 58, 2, tformck,  tb,    tbtgap },   // table title gap
  { group,    2, 58, 3, tformck,  tb,    tbltitl },  // table title start
  { endgroup, 2, 58, 3, tformck,  tb,    tbetitl },  // table title end
#if 0
  { group,    2, 60, 0, conddef,  tx,    ignore },   // condition def start
  { imnem,    2, 61, 3, condvis,  tx,    ignore },   // condition visibility
  { imnem,    2, 62, 0, nothing,  tx,    conduse },  // condition start
  { imnem,    2, 64, 1, conddis,  tx,    ignore },   // condition display
#endif
  { group,    2, 60, 0, conddef,  tx,    ignore },   // condition def start
  { endgroup, 2, 60, 0, condedef, tx,    ignore },   // condition def end
  { ename,    2, 61, 1, condnam,  tx,    ignore },   // condition name
  { group,    2, 61, 2, condsind, tx,    ignore },   // condition indicators
  { endgroup, 2, 61, 2, condeind, tx,    ignore },   // condition indicators
  { imnem,    2, 61, 3, condvis,  tx,    ignore },   // condition visibility
  { imnem,    2, 61, 4, condcol,  tx,    ignore },   // condition color
  { ibyte,    2, 61, 4, condcol,  tx,    ignore },   // condition color
  { imnem,    2, 61, 5, condbcol, tx,    ignore },   // condition background
  { ibyte,    2, 61, 5, condbcol, tx,    ignore },   // condition background
  { imnem,    2, 61, 6, condccol, tx,    ignore },   // condition chbar color
  { ibyte,    2, 61, 6, condccol, tx,    ignore },   // condition chbar color
  { etext,    2, 61, 7, condcch,  tx,    ignore },   // condition ch chars
  { etext,    2, 61, 8, condsimg, tx,    ignore },   // condition start img
  { etext,    2, 61, 9, condsalt, tx,    ignore },   // condition start alt
  { etext,    2, 61,10, condeimg, tx,    ignore },   // condition end img
  { etext,    2, 61,11, condealt, tx,    ignore },   // condition end alt
  { imnem,    2, 62, 0, condset,  tx,    conduse },  // condition start
  { imnem,    2, 63, 0, condres,  tx,    condend },  // condition end
  { imnem,    2, 64, 1, conddis,  tx,    ignore },   // condition display
  { imnem,    2, 65, 0, condrset, tb,    condruse }, // condition row use

  { group,    2, 70, 1, nothing,  an,    ignore },   // footnote defaults
  { group,    2, 71, 0, ftnstart, an,    putftn },   // footnote start
  { endgroup, 2, 71, 0, ftnend,   an,    endftn },   // footnote end
  { imnem,    2, 72, 1, ftnform,  an,    ignore },   // footnote format
  { imnem,    2, 72, 6, nothing,  an,    setftn },   // footnote marker

  { group,    2, 80, 0, formdef,  tx,    ignore },   // format def start
  { endgroup, 2, 80, 0, formend,  tx,    ignore },   // format def end
  { ename,    2, 81, 1, formname, tx,    ignore },   // format name
  { imnem,    2, 81, 2, formtype, tx,    ignore },   // format type

  { group,    2, 81, 3, fmpxdef,  tx,    fmpxset },  // format prefix start
  { endgroup, 2, 81, 3, fmpxend,  tx,    fmpxres },  // format prefix end
  { group,    2, 81, 4, fmsxdef,  tx,    fmsxset },  // format suffix start
  { endgroup, 2, 81, 4, fmsxend,  tx,    fmsxres },  // format suffix end

  { imnem,    2, 82, 1, formscan, tx,    formcall }, // format init
  { imnem,    2, 82, 2, formscan, tx,    formcall }, // format inherit
  { imnem,    2, 82, 3, nothing,  tb,    formtbl },  // format table
  { group,    2, 83, 1, fmtxbeg,  tx,    formsave }, // format text prefix group
  { endgroup, 2, 83, 1, fmtxend,  tx,    formrest }, // format text prefix end
  { group,    2, 83, 2, nothing,  tx,    formsave }, // format text suffix group
  { endgroup, 2, 83, 2, nothing,  tx,    formrest }, // format text suffix end
  { ename,    2, 84, 0, formtag,  tx,    ftaguse },  // format tag
  { ename,    2, 85, 0, formspc,  tx,    ftagspc },  // format special

  { group,    2, 90, 0, vardef,   an,    ignore },   // var def start
  { etext,    2, 91, 1, varname,  an,    ignore },   // var name
  { group,    2, 91, 2, varform,  an,    ignore },   // var format def
  { imnem,    2, 97, 0, nothing,  an,    putvtok },   // var form tokens
  { ename,    2, 97, 0, nothing,  an,    putvtok },   // var form tokens
  { endgroup, 2, 90, 0, varend,   an,    ignore },   // var def end

	// xref format def
  { group,    2, 95, 0, xrfmdef,  an,    ignore },   // xref def group
  { etext,    2, 96, 1, xrfname,  an,    ignore },   // ref format name
  { group,    2, 96, 3, xrform,   an,    ignore },   // xref format group
  { imnem,    2, 97,35, xrftok,   an,    putxrtok }, // xref format token

	// xref usage
  { group,    2, 10, 7, xrfval,   an,    makxref },  // xref text group
  { etext,    2, 96, 2, getxref,  an,    putxref },  // cross-ref ahref
  { ilong,    2, 96, 2, getxref,  an,    putxref },  // cross-ref ahref
  { imnem,    2, 96, 3, numxrfm,  an,    xrfform },  // ref format
  { endgroup, 2, 10, 7, usexrfm,  an,    endxref },  // end of link text
  { group,    2, 98, 0, nothing,  an,    xrfcont },  // ref_cont wrapper group
  { endgroup, 2, 98, 0, nothing,  an,    xrfecont }, // ref_cont wrapper end

  { group,    2, 96, 4, nothing,  tx,    ignore },   // ref text group
  { etext,    2, 96, 4, nothing,  tx,    puttx },    // ref text string
  { ename,    2, 96, 5, gtxfile,  an,    setfile },  // ref file name
  { etext,    2, 96, 5, gtxfile,  an,    setfile },  // ref file name

	// anums
  { group,    2,100, 0, anumdef,  an,    ignore },   // anum def
  { endgroup, 2,100, 0, endanum,  an,    ignore },   // anum def end
  { ename,    2,101, 1, anumid,   an,    ignore },   // anum stream name
  { ilong,    2,101, 3, anumstrm, an,    ignore },   // anum stream id
  { ilong,    2,101, 7, anumlvl,  an,    ignore },   // anum level
  { imnem,    2,101, 2, anumtype, an,    ignore },   // anum def type
  { group,    2,101, 4, anumfmt,  an,    ignore },   // anum format group
  { endgroup, 2,101, 4, anumefmt, an,    ignore },   // anum format end
  { group,    2,101, 5, anumrfmt, an,    ignore },   // anum ref format group
  { imnem,    2,102, 0, anumform, an,    putatok },  // anum format token
  { group,    2,101, 8, nothing,  an,    ignore },   // anum value group
  { ename,    2,103, 0, anumtxt,  an,    ignore },   // starting anum val
  { ilong,    2,103, 0, anumset,  an,    ignore },   // starting anum val
  { imnem,    2, 97,24, anumdtok, an,    putdtok },  // docnum token

	// index
  { group,    2,110, 1, idxchk,   an,    idxgrp },   // index token start
  { endgroup, 2,110, 1, idxchke,  an,    idxend },   // index token end
  { imnem,    2, 97,40, nothing,  an,    idxtyp },   // index type ref

	// xref source
	{ etext,    2,110, 4, setxloc,  an,    setxref },  // cross-ref aname
	{ ilong,    2,110, 4, setxloc,  an,    setxref },  // cross-ref aname

	{ etext,    2,110, 3, setmark,  an,    markuse },  // list marker

	{ etext,    2,110, 5, formchk,  an,    sethref },  // old hyper marker
	{ etext,    2,110, 6, formchk,  an,    setsubj },  // subject marker
	{ ilong,    2,112, 1, nothing,  an,    hypopt },   // hypertext option
	{ etext,    2,112, 2, nothing,  an,    hyploc },   // hypertext location
	{ ilong,    2,112, 2, formchk,  an,    hypunq },   // unique location
	{ etext,    2,112, 3, nothing,  an,    hypfile },  // filename for link
	{ etext,    2,112, 4, formchk,  an,    hyplnk },   // gotolink
	{ ilong,    2,112, 4, formchk,  an,    hypobj },   // gotoobjectid unique
	{ etext,    2,112, 5, formchk,  an,    hyplnk },   // openlink
	{ ilong,    2,112, 5, formchk,  an,    hypobj },   // openobjectid unique
	{ etext,    2,112, 6, formchk,  an,    sethref },  // alert
	{ etext,    2,112, 7, formchk,  an,    hyplnk },   // message url
	{ etext,    2,112,10, nothing,  an,    hypalt },   // alternate location
	{ ilong,    2,112,12, formchk,  an,    hyppgn },   // page number

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

	{ imnem,    2,117, 2, nothing,  tx,    idxfld },   // index field
	{ ilong,    2,117, 2, nothing,  tx,    idxfld },   // index letter
	{ imnem,    2,118, 0, nothing,  tx,    idxprp },   // index list props

  { group,    3, 10, 0, pgdef,    gr,    ignore },   // page start
  { endgroup, 3, 10, 0, pgend,    gr,    ignore },   // page end
  { imnem,    3, 11, 0, pgattr,   gr,    ignore },   // page attribute
  { ilong,    3, 11, 0, pgattr,   gr,    ignore },   // page attribute
  { elong,    3, 12, 1, pgsize,   gr,    ignore },   // page size
  { group,    3, 20, 0, frdef,    gr,    ignore },   // frame start
  { endgroup, 3, 20, 0, fredef,   gr,    ignore },   // frame end
  { ename,    3, 21, 1, frname,   gr,    ignore },   // frame name
  { imnem,    3, 21, 0, frattr,   gr,    ignore },   // frame attributes
  { ilong,    3, 21, 0, frattr,   gr,    frmval },   // frame attributes
  { elong,    3, 22, 0, frsize,   gr,    ignore },   // frame size
  { imnem,    3, 23, 0, franch,   gr,    ignore },   // frame anchor
  { imnem,    3, 24, 0, frline,   gr,    ignore },   // frame border
  { imnem,    3, 25, 0, frpen,    gr,    ignore },   // frame border
  { imnem,    3, 26, 0, frfill,   gr,    ignore },   // frame fill
  { ename,    3, 30, 0, frprev,   gr,    ignore },   // frame preview
  { imnem,    3, 40, 0, frtext,   gr,    ignore },   // layout text

  { group,    4,  1, 0, grgroup,  gr,    ignore },   // graphic group start
  { endgroup, 4,  1, 0, gregroup, gr,    ignore },   // graphic group end
  { group,    4, 10, 0, grobj,    gr,    ignore },   // graph obj start
  { endgroup, 4, 10, 0, greobj,   gr,    ignore },   // graph obj end
  { imnem,    4, 20, 0, grline,   gr,    ignore },   // line props
  { imnem,    4, 20, 2, ruleprop, tb,    ignore },   // line color
  { ilong,    4, 20, 3, ruleprop, tb,    ignore },   // line thickness
  { ilong,    4, 20, 5, ruleprop, tb,    ignore },   // line count
  { imnem,    4, 21, 0, grpen,    gr,    ignore },   // pen props
  { imnem,    4, 22, 0, grfill,   gr,    ignore },   // fill props
  { imnem,    4, 24, 1, grarrhd,  gr,    ignore },   // arrow head style
  { imnem,    4, 24, 2, grarrtl,  gr,    ignore },   // arrow tail style

  { elong,    4, 30, 0, grshape,  gr,    ignore },   // vector shape
  { elong,    4, 40, 0, grtext,   gr,    ignore },   // graphic text

  { elong,    4, 50, 0, eqtype,   gr,    ignore },   // equation type
  { elong,    4, 51, 1, eqsize,   gr,    ignore },   // equation bounds
  { imnem,    4, 51, 2, eqalgn,   gr,    ignore },   // equation alignment
  { imnem,    4, 51, 3, eqtxsiz,  gr,    ignore },   // equation text size
  { ilong,    4, 51, 4, eqlbrk,   gr,    ignore },   // equation line break
  { ename,    4, 52, 1, eqtext,   gr,    ignore },   // equation text
  { ename,    4, 52, 2, eqgfile,  gr,    ignore },   // equation gr file

  { elong,    4, 61, 0, rasprop,  rfm,   ignore },   // raster props
  { ebyte,    4, 60, 0, rasbmap,  rfm,   ignore },   // raster bitmap
  { ebyte,    4, 62, 0, rascmap,  rfm,   ignore },   // raster colormap
  { elong,    4, 71, 0, epsprop,  rfm,   ignore },   // eps props
  { ebyte,    4, 72, 0, epshead,  rfm,   ignore },   // eps raster head
	{ ibyte,    4, 80, 0, colordef, gr,    ignore },   // color definition
	{ elong,    4, 84, 0, arrdef,   gr,    ignore },   // arrow definition
  { ename,    1, 50, 3, grfile,   gr,    ignore },   // named graphic
};
size_t RTFwr::RTFsize;
DCvlist *RTFwr::RTFlist[RtfListMax];


RTFgroup RTFwr::BaseGroup;
RTFgroup *RTFwr::GroupStack[LevelMax];
uns RTFwr::DCLlevel;
bool RTFwr::SkipCurrGroup;
uns RTFwr::SkipLevel;


void
RTFwr::ReadDCL(DCLrfile *rf)
{
  RTFctl *cp;

  SetUpRTF();  // prepare the search array
  GroupStack[0] = &BaseGroup;
  SkipCurrGroup = false;

  while (!rf->eof()) {
    cp = new RTFctl;
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

    FindRTF(cp);            // sets cp->rp to point at matching rtf struct
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

    if (SkipCurrGroup)
      continue;

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

      case rfm:
        if (ProcRf(cp))
          break;
        else continue;

      default:
        assert(0);
        return;
    }

    if (DCLlevel < LevelMax)
      GroupStack[DCLlevel]->add(*cp);
  }
}


void
RTFwr::SetUpRTF(void)
{
  size_t i;
  rtf *rtp;
  uns idx;
  DCvlist *dtlist;

  RTFsize = sizeof(RTFpile) / sizeof(rtf);
  for (i = 0, rtp = RTFpile; i < RTFsize; i++, rtp++) {
    idx = ((rtp->ctlmaj - 1) * MinorMax) + rtp->ctlmin;
    if (idx >= RtfListMax)
      continue;
    if (RTFlist[idx] == NULL)
      RTFlist[idx] = new DCvlist;
    if ((dtlist = (DCvlist *) RTFlist[idx]->find(rtp->ctldat + 1)) == NULL) {
      dtlist = new DCvlist;
      RTFlist[idx]->add(dtlist, rtp->ctldat + 1);
    }
    if (dtlist->find(rtp->ctldt + 1) == NULL)
      dtlist->add(rtp, rtp->ctldt + 1);
  }
}


void
RTFwr::FindRTF(RTFctl *cp)
{
  uns idx;
  DCvlist *dlist, *dtlist;

  idx = ((cp->maj() - 1) * MinorMax) + cp->min();
  if ((idx < RtfListMax)
   && ((dlist = (DCvlist *) RTFlist[idx]) != NULL)) {
    if (((dtlist = (DCvlist *) dlist->find(cp->dat() + 1)) != NULL)
     || ((dtlist = (DCvlist *) dlist->find(1)) != NULL)) {
      if ((cp->rp = (rtf *) dtlist->find(cp->dt() + 1)) != NULL)
        return;
      if (cp->dt() == imnem) { // fuzzy match
        if ((cp->rp = (rtf *) dtlist->find(ilong + 1)) != NULL)
          return;
      }
      else if (cp->dt() == ilong) {
        if ((cp->rp = (rtf *) dtlist->find(imnem + 1)) != NULL)
          return;
      }
    }
  }
  cp->rp = NULL;
}




// process switch


bool
RTFwr::ProcTop(RTFctl *cp)
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
			if (cp->dat() == 6) {   // Frame
				MIFVer = cp->siz();  // single-digit MIF version
				if (MIFVer >= 8)
					SetUnicodeFonts();
			}
			break;

		case drname:
			DRmifVer = (char *) cp->ex();
			break;

    case unitset:   // set units sizes
      SetUnits(cp);
      break;

		case dirapp:
			SetDirApplication(cp);
			break;
		case dirsec:
			SetDirSection(cp);
			break;
		case dirprop:
			SetDirProperty(cp);
			break;
		case dirval:
			SetDirValue(cp);
			break;

    default:
      assert(0);
      return false;
  }
  return true;
}


// DCL directives


char *RTFwr::DirAppName = NULL;
bool RTFwr::DirLocal = false;

char *RTFwr::DirSecName = NULL;
short RTFwr::DirSecNum = 0;

char *RTFwr::DirSecNames[] = {
 "Defaults", "Options", "Graphics", "GraphFiles",
 "Fonts", "FontWidths", "FontTypes", "Styles",
 "HelpTags", "HelpData", "HelpBrowse", "TextFlows",
 "RefFrameFormats", "XrefFiles", "WordOptions", "HelpOptions",
 "Table", NULL };

char *RTFwr::DirPropName = NULL;
short RTFwr::DirPropNum = 0;

char **RTFwr::DirPropNames[] = {
 NULL, DirOptionsProps, DirGraphicsProps, NULL,
 NULL, NULL, NULL, NULL,
 NULL, NULL, DirHelpBrowseProps, NULL,
 NULL, NULL, DirOptionsProps, DirOptionsProps,
 DirTableProps, NULL };
char *RTFwr::DirOptionsProps[] = {
 "GraphText", "FilePaths", "FileNames", "Metafiles",
 "OccludedTabs", "EmptyFrames", "TableTitles", "TableRules",
 "SHVertAdjust", "RefFrameFormat", "FrameStyle", "TableGraphics",
 "TableRuleAdjust", "StripTables", "UseParaAnchors", "TableWidthsFixed",
 "HideWhiteText", "TextRescale", "TableRescale", "GraphicRescale", NULL };
char *RTFwr::DirGraphicsProps[] = {
 "GraphText", "FilePaths", "FileNames", "Metafiles",
 "BackMode", "StretchMode", "BitmapDPI", "BitmapFlip",
 "TextScale", "TextWidth", "ClipText", "ClipType",
 "ClipLimit", "GrVertAdjust", "GrTableSpace", "FrameExactHeight",
 "UseTopSpaceAbove", "NoBlankFirstGTLine", "WrapAllFrames", "EmbedBMPsInWMFs",
 NULL };
char *RTFwr::DirTableProps[] = {
 "TableTitles", "TableRules", "TableGraphics",
 "TableRuleAdjust", "StripTables", "TableWidthsFixed",
 "TableRescale", NULL };
char *RTFwr::DirHelpBrowseProps[] = {
 "TitleBrowse", "Start", "Step", "Digits",
 "Prefix",
 NULL };

char *RTFwr::DirValName = NULL;
long RTFwr::DirValNum = 0;
bool RTFwr::DirValBool = false;


void
RTFwr::SetDirApplication(RTFctl *cp)
{
	if (DirAppName)
		DeleteName(DirAppName);
	DirAppName = NewName((char *) cp->ex(), cp->siz());
	if ((!_strnicmp(DirAppName, "dwrtf", 5))
	 || (!_strnicmp(DirAppName, "mif2rtf", 7)))
		DirLocal = true;
	else
		DirLocal = false;
}


void
RTFwr::SetDirSection(RTFctl *cp)
{
	if (DirLocal == false)
		return;
	if (DirSecName)
		DeleteName(DirSecName);
	DirSecName = NewName((char *) cp->ex(), cp->siz());
	DirSecNum = 0;
	for (short i = 0; DirSecNames[i]; i++) {
		if (!_stricmp(DirSecName, DirSecNames[i])) {
			DirSecNum = i + 1;
			break;
		}
	}
}


void
RTFwr::SetDirProperty(RTFctl *cp)
{
	if (!DirLocal
	 || !DirSecNum)
		return;
	if (DirPropName)
		DeleteName(DirPropName);
	DirPropName = NewName((char *) cp->ex(), cp->siz());
	DirPropNum = 0;
	char **cpp;
	if ((cpp = DirPropNames[DirSecNum - 1]) != NULL) {
		for (short i = 0; cpp[i]; i++) {
			if (!_stricmp(DirPropName, cpp[i])) {
				DirPropNum = i + 1;
				break;
			}
		}
	}
}


void
RTFwr::SetDirValue(RTFctl *cp)
{
	if (DirLocal == false)
		return;
	if (DirValName)
		DeleteName(DirValName);
	DirValName = NewName((char *) cp->ex(), cp->siz());
	DirValNum = atol(DirValName);
	if (!_stricmp(DirValName, "yes")
	 || !_stricmp(DirValName, "true")
	 || !strcmp(DirValName, "1"))
		DirValBool = true;
	else
		DirValBool = false;

	// set the required local variable according to section and property

	switch (DirSecNum) {
		case 1:  // Defaults
			break;

		case 15:  // WordOptions
			if (OutType == 'S')
				goto options;
			break;

		case 16:  // HelpOptions
			if (OutType == 'H')
				goto options;
			break;

		case 17:// Table
			switch (DirPropNum) {
				case 1: // TableTitles
					TbTitleType = (char) DirValNum;
					break;
				case 2: // TableRules
					TbRuleType = toupper(*DirValName);
					break;
				case 3: // TableGraphics
					TbGraphics = toupper(*DirValName);
					break;
				case 4: // TableRuleAdjust
					TbRuleAdjust = DirValBool;
					break;
				case 5: // StripTables
					StripTables = DirValBool;
					break;
				case 6: // TableWidthsFixed
					TbFixed = DirValBool;
					break;
				case 7: // TableRescale
					TableRescale = (uns) DirValNum;
					break;

				default:
					break;
			}
			break;

		case 2:  // Options
		options:
			switch (DirPropNum) {
				case 1:  // GraphText
					GrTxType = toupper(*DirValName);
					break;
				case 2:  // FilePaths
					GrPaths = toupper(*DirValName);
					break;
				case 3:  // FileNames
					GrNames = toupper(*DirValName);
					break;
				case 4:  // Metafiles
					MetaType = toupper(*DirValName);
					break;
				case 5: // OccludedTabs
					OccTabType = toupper(*DirValName);
					break;
				case 6: // EmptyFrames
					EmpFrType = toupper(*DirValName);
					break;
				case 7: // TableTitles
					TbTitleType = (char) DirValNum;
					break;
				case 8: // TableRules
					TbRuleType = toupper(*DirValName);
					break;
				case 9: // SHVertAdjust
					SHVAdjust = (short) DirValNum;
					break;
				case 10: // RefFrameFormat
					if (RefFrameFormat && *RefFrameFormat)
						DeleteName(RefFrameFormat);
					RefFrameFormat = NewName(DirValName);
					if (RefFrameFormat == NULL)
						RefFrameFormat = "";
					break;
				case 11: // FrameStyle
					if (FrameStyle)
						DeleteName(FrameStyle);
					FrameStyle = NewName(DirValName);
					break;
				case 12: // TableGraphics
					TbGraphics = toupper(*DirValName);
					break;
				case 13: // TableRuleAdjust
					TbRuleAdjust = DirValBool;
					break;
				case 14: // StripTables
					StripTables = DirValBool;
					break;
				case 15: // UseParaAnchors
					UseParaAnchors = DirValBool;
					break;
				case 16: // TableWidthsFixed
					TbFixed = DirValBool;
					break;
				case 17: // HideWhiteText
					HideWhite = DirValBool;
					break;

				case 18: // TextRescale
					TextRescale = (uns) DirValNum;
					break;
				case 19: // TableRescale
					TableRescale = (uns) DirValNum;
					break;
				case 20: // GraphicRescale
					GraphicRescale = (uns) DirValNum;
					break;

				default:
					break;
			}
			break;

		case 3:  // Graphics
			switch (DirPropNum) {
				case 1:  // GraphText
					GrTxType = toupper(*DirValName);
					break;
				case 2:  // FilePaths
					GrPaths = toupper(*DirValName);
					break;
				case 3:  // FileNames
					GrNames = toupper(*DirValName);
					break;
				case 4:  // Metafiles
					MetaType = toupper(*DirValName);
					break;
				case 5:  // BackMode
					GrBkMode = (short) DirValNum;
					break;
				case 6:  // StretchMode
					GrStretchMode = (short) DirValNum;
					break;
				case 7:  // BitmapDPI
					BitmapDPI = (short) DirValNum;
					break;
				case 8:  // BitmapFlip
					BitmapFlip = DirValBool;
					break;
				case 9:  // TextScale
					GrTxScale = (short) DirValNum;
					if (GrTxScale == 0)
						GrTxScale = 100;
					break;
				case 10: // TextWidth
					GrTxWidth = (short) DirValNum;
					break;
				case 11: // ClipText
					ClipText = DirValBool;
					break;
				case 12: // ClipType
					ClipType = toupper(*DirValName);
					break;
				case 13: // ClipLimit
					ClipLimit = (short) DirValNum;
					break;
				case 14: // GrVertAdjust
					GrVert = (short) DirValNum;
					break;
				case 15: // GrTableSpace
					GrTbSpace = DirValBool;
					break;
				case 16: // FrameExactHeight
					FrExHigh = DirValBool;
					break;
				case 17: // UseTopSpaceAbove
					UseTopSpaceAbove = DirValBool;
					break;
				case 18: // NoBlankFirstGTLine
					NoBlankFirstGTLine = DirValBool;
					break;
				case 19: // WrapAllFrames
					WrapAllFrames = DirValBool;
					break;
				case 20: // EmbedBMPsInWMFs
					EmbedBMPs = DirValBool;
					break;
				default:
					break;
			}
			break;

		case 4:  // GraphFiles
		case 5:  // Fonts
		case 6:  // FontWidths
		case 7:  // FontTypes
		case 8:  // Styles
		case 9:  // HelpTags
		case 10: // HelpData
			break;

		case 11: // HelpBrowse
			switch (DirPropNum) {
				case 1:  // TitleBrowse
					TitleBrowse = DirValBool;
					break;
				case 2:  // Start
					BrowseCurr = (int) DirValNum;
					break;
				case 3:  // Step
					BrowseStep = (int) DirValNum;
					break;
				case 4:  // Digits
					BrowseDigits = (int) DirValNum;
					if (BrowseDigits > BrowseDigitsMax)
						BrowseDigits = BrowseDigitsMax;
					break;
				case 5:  // Prefix
					BrowsePrefix = NewName(DirValName);
					break;
				default:
					break;
			}
			break;

		case 12: // TextFlows
		case 13: // RefFrameFormats
		case 14: // XrefFiles
			break;

		default:
			break;
	}
}


// RTF writing phase

bool RTFwr::FlowWritten = false;


void
RTFwr::WriteRTF(RTFwfile *wf)
{
	// use page format file to adjust these globals
	GetPageFormatFileInfo();
	Margins = LMarg + RMarg + Gutter;
	BodyWide = PaperWide - Margins;
	if (!TblFullWidthWasSet)
		TblFullWidth = BodyWide;

  // put out char set and language info
  wf->putCW(CharSet);

  wf->putCWN("ansicpg", DefForm.cpg ? (short) DefForm.cpg : 1252);

  //if (UseLang)
  //  wf->putCWN("lang", DefForm.lang);

  wf->putCWN("deff", DefForm.fnum);

	if (FmtFile
	 && FmtFile->Section("Required"))
		DefineRequiredFormats(FmtFile);

  // put out font info
  WriteFonts(wf);

  // put out color map
  WriteColorTable(wf);

  // put out styles
  WriteStyles(wf);

  // put out doc props
	WriteInfo(wf);
  WriteDocProps(wf);

  // put out sections, each with headers and footers as needed
	if (SingleSect)
		WriteFlow(wf, 0);
  else for (uns i = 1; i <= SectNum; i++) {
    if (SectUsedList.find(i)
		 && !SectSkipList.find(i))
      WriteFlow(wf, (uns) SectList.find(i));
  }

	if (*CntHeadStr != '\0') { // last one was V
		unc lev = 0;
		LastTopicEmpty = TopicEmpty;
		WriteOutCntEntry(lev);
	}

	if (CntFile)
		CntFile->CloseFile();

	//if (FlowWritten)
	//  wf->putCW("sect");

	if (MXLwr::WriteKeydefLib) {
		DCnlist *dcn = &MXLwr::SourceKeydefs;
		char *keys = NULL;
		char *src = NULL;
		char *out = NULL;
		while ((dcn = dcn->next) != NULL) {
			keys = (char *) dcn->name;
			src = (char *) dcn->id;
			out = GetFinalFileName(src);
			MXLwr::SetOutputKeydef(keys, out);
		}
		MXLwr::WriteOutputKeydefLib();
	}


	DCicu::CloseCodePage();

	if (CopyOriginalGraphics
	 && GraphCopyCount) {
		if (!XMLSource
		 || !DoAutomation)
			DCauto::CopyOriginalGraphicsProc(&GraphFileCopyList, &GraphFileFixedCopyList,
			                                 GraphCopyCount);
		else {
			DCauto::OriginalGraphics = &GraphFileCopyList;
			DCauto::OriginalFixedGraphics = &GraphFileFixedCopyList;
			DCauto::OriginalGraphicsCount = GraphCopyCount;
		}
	}

	if (DoAutomation) {
		if (XMLSource) {
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
		LogEvent(loginfo, 1, "Processing completed by dwrtf.");
		Log->CloseFile();
		CheckLogErrors();
	}
}


char *
RTFwr::GetFinalFileName(char *src)
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

	return outid;
}


#if 0
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
#endif


char *
RTFwr::MakeOutID(char *out, char *id)
{
	CurrFilePrefix = GetFIDPrefix(StripSuf(CurrFileName));
	long len = strlen(CurrFilePrefix) + 2;
	if (id)
		len += strlen(out) + strlen(id) + 5;
	else
		len += (strlen(out) * 2) + 5;

	char *outid = NewName("R", len);
	strcat(outid, CurrFilePrefix);
	strcat(outid, "U");

	char *dot = strrchr(out, '.');
	if (dot) {
		strncat(outid, out, dot - out);
		strcat(outid, dot + 1);
		strcat(outid, "E");
		if (id)
			strcat(outid, id);
		else
			strncat(outid, out, dot - out);
	}
	else {
		strcat(outid, out);
		strcat(outid, "E");
		strcat(outid, id ? id : out);
	}
	return outid;
}


void
RTFwr::CheckLogErrors(void)
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


void
RTFwr::WriteInfo(RTFwfile *wf)
{
	char *cpt = FilterVer;
	FilterVer = NewName(strlen(cpt) + strlen(DRmifVer) + strlen(DWrtfVer) + 3);
	sprintf(FilterVer, "%s %s %s", cpt, DRmifVer, DWrtfVer);

  wf->putEnd();
  wf->putGS();
  wf->putCW("info");
  wf->putGS();
  wf->putCW("doccomm");
	wf->putStr(FilterVer);
  wf->putGE();
  wf->putGE();
}


void
RTFctl::write(DCfile& wf)
{
  if (rp == NULL)
    return;

	// sort out table processing
  if (rp->write_op == puttbl)
    RTFwr::ProcTbl( (RTFwfile *) &wf, this);
  else if (rp->write_op == endtbl)
    RTFwr::WriteTbl( (RTFwfile *) &wf);
  else if (RTFwr::InTbl)
    RTFwr::ProcTblProp(this);

	// write everything else
  else switch (rp->write_op) {
		case dbgwrite:         // place to set a breakpoint
			if ((this->siz() & 2) != 2)
				break;
#ifdef _DEBUG
			__asm int 3
#endif
			break;
		case putfile:
			RTFwr::WriteFromExtFile( (RTFwfile *) &wf, this);
			break;
		case flnum:
			RTFwr::WriteFlowNum( (RTFwfile *) &wf, this);
			break;
		case putfr:
			RTFwr::WriteFrameCtl( (RTFwfile *) &wf, this);
			break;
		case puttx:
			RTFwr::WriteText( (RTFwfile *) &wf, this);
			break;
		case parstart:
			RTFwr::StartPara( (RTFwfile *) &wf, this);
			break;
		case parend:
			RTFwr::EndPara( (RTFwfile *) &wf);
			break;
		case linend:
			RTFwr::EndLine( (RTFwfile *) &wf, this);
			break;
		case colend:
			RTFwr::EndColumn( (RTFwfile *) &wf, this);
			break;
		case pagend:
			RTFwr::EndPage( (RTFwfile *) &wf, this);
			break;
		case putvar:
			RTFwr::PutVarForm( (RTFwfile *) &wf, this);
			break;
		case putvtok:
			RTFwr::PutVarToken( (RTFwfile *) &wf, this);
			break;
		case putanum:
			RTFwr::PutAnumVal( (RTFwfile *) &wf, true);
			break;
		case putatok:
		case putdtok:
			RTFwr::WriteAnumToken( (RTFwfile *) &wf, this);
			break;
		case idxfld:
			RTFwr::InsertIndexTOCItem( (RTFwfile *) &wf, this);
			break;
		case idxprp:
			RTFwr::SetIXListProps(this);
			break;

		case conduse:
			RTFwr::SetCond(this);
			break;
		case condend:
			RTFwr::EndCond(this);
			break;

		case markuse:
			RTFwr::WriteMarker( (RTFwfile *) &wf, this);
			break;

		case sethref:
			RTFwr::PutHypertext( (RTFwfile *) &wf, this);
			break;
		case setsubj:
			RTFwr::PutHyperSubject( (RTFwfile *) &wf, this);
			break;
		case hyploc:
			RTFwr::PutHyperLoc( (RTFwfile *) &wf, this);
			break;
		case hypalt:
			RTFwr::PutHyperAlt( (RTFwfile *) &wf, this);
			break;
		case hyppgn:
			RTFwr::SetCurrPgn(this);
			break;
		case hypunq:
			RTFwr::PutHyperUnique( (RTFwfile *) &wf, this);
			break;
		case hypfile:
			RTFwr::PutHyperFile( (RTFwfile *) &wf, this);
			break;
		case hyplnk:
			RTFwr::PutHyperLnk( (RTFwfile *) &wf, this);
			break;
		case hypobj:
			RTFwr::PutHyperObj( (RTFwfile *) &wf, this);
			break;
		case xrfform:
			//RTFwr::ProcXrefFormatUse(this->siz());
			break;
		case makxref:
			RTFwr::WriteXRVal( (RTFwfile *) &wf, this);
			break;
		case xrfcont:
			RTFwr::WriteXRCont( (RTFwfile *) &wf, this, true);
			break;
		case xrfecont:
			RTFwr::WriteXRCont( (RTFwfile *) &wf, this, false);
			break;
		case putxrtok:
			RTFwr::WriteXRToken( (RTFwfile *) &wf, this);
			break;
		case putxref:
			RTFwr::StartAnchor( (RTFwfile *) &wf, this, true);
			break;
		case setxref:
			RTFwr::StartAnchor( (RTFwfile *) &wf, this, false);
			break;
		case endxref:
			RTFwr::EndAnchor( (RTFwfile *) &wf);
			break;
    case putftn:
			RTFwr::WriteTextFtnStart((RTFwfile *) &wf);
			break;
    case endftn:
			RTFwr::WriteTextFtnEnd((RTFwfile *) &wf);
			break;
		case frnum:
			RTFwr::CheckTextFrame((RTFwfile *) &wf, this);
			break;
    case formsave:
			RTFwr::InTextPx = true;
			if (!RTFwr::WriteAnums)
				break;
			if (RTFwr::CharStylesUsed)
				RTFwr::WriteText((RTFwfile *) &wf);
			else
				RTFwr::SetCurrProps(this);
			break;

		default:
			RTFwr::SetCurrProps(this);
			break;
	}
}


void
RTFgroup::insert(RTFctl& nc, RTFctl& oc)
{
  if (!first || (first->ctl == &oc)) {
    first = new RTFitem(&nc, first);
    return;
  }
  
  for (RTFitem* ip = first; ip->next != NULL; ip = ip->next) {
    if (ip->next->ctl == &oc)
      break;
  }
  ip->next = new RTFitem(&nc, ip->next);
}

void
RTFgroup::append(RTFctl& nc, RTFctl& oc)
{
  if (!first) {
    first = new RTFitem(&nc);
    return;
  }

  for (RTFitem* ip = first; ip->next != NULL; ip = ip->next) {
    if (ip->ctl == &oc)
			break;
  }
  ip->next = new RTFitem(&nc, ip->next);
}

void
RTFgroup::replace(RTFctl& nc, RTFctl& oc)
{
  for (RTFitem* ip = first; ip != NULL; ip = ip->next) {
    if (ip->ctl == &oc)	{
		  ip->ctl = &nc;
			break;
		}
  }
}

RTFctl*
RTFgroup::getnext(RTFitem** it)
{ 
	if (*it != NULL) { 
		*it = (*it)->next; 
		return (*it) ? (*it)->ctl : NULL; 
	}
	if (first != NULL) { 
		*it = first; 
		return (*it)->ctl; 
	}
	*it = NULL; 
	return NULL; 
}


// utility functions


pprop RTFwr::SavedCurrStack[SavedStateMax];
pprop RTFwr::SavedLastStack[SavedStateMax];
pprop RTFwr::SavedParaStack[SavedStateMax];
uns RTFwr::SavedLevel;


void
RTFwr::SaveState(void)
{
  if (++SavedLevel < SavedStateMax)	{
    SavedCurrStack[SavedLevel - 1] = Curr;
    SavedLastStack[SavedLevel - 1] = Last;
    SavedParaStack[SavedLevel - 1] = ParaForm;
	}
}


void
RTFwr::RestoreState(StateType stype)
{
  if ((SavedLevel > 0)
   && (--SavedLevel < SavedStateMax))	{
		if ((stype == CurrState)
		 || (stype == FullState))
			Curr = SavedCurrStack[SavedLevel];
		if ((stype == LastState)
		 || (stype == FullState))
	    Last = SavedLastStack[SavedLevel];
    ParaForm = SavedParaStack[SavedLevel];
	}
}



uns RTFwr::UnitMul[UnitsMax] = {  72,   72,  2,  72 };
uns RTFwr::UnitDiv[UnitsMax] = { 500, 1270, 10, 336 };

uns RTFwr::UBaseMul[UnitBasesMax] = {  72,   72,  2,  72 };
uns RTFwr::UBaseDiv[UnitBasesMax] = { 500, 1270, 10, 336 };


void
RTFwr::SetUnits(RTFctl *cp)
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
RTFwr::GetTwips(long val, unittype ut)
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
RTFwr::GetOrig(long val, unittype ut)
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

// end of dwrtf.cc


