// m2rinid.c is the ini processor for mif2go 
// Copyright 2010 Omni Systems, Inc.  All Rights Reserved.


#include "m2rbook.h"

#ifdef NEWINI

typedef VoidT (*psfunc)(VoidT);

typedef UByteT bool;
#define false 0
#define true 1

enum styp {    // type of var to be set
 spec, yesno, chr, cap, str, path, sarr, mac, cmp,
 snum, usnum, lnum, larr, ulnum, ucnum, hex,
 ilist, val, bval, cval, ushval, bit, nbit
};

enum sectyp {  // type of key used to index section
 unknown, set, file, ch, fnt, flow, fsize, colr,
 pstyle, cstyle, pcstyle, macr, macvar,
 tstyle, tabl, graphic, xrstyle, mark, reffr,
 tag, loc, suf, jhimg, toclev, proj, wind
};

struct setinfo {
	StringT sname;  // key name of setting
	enum styp stype;   // type of var to set
	VoidT *svar;   // ptr to var to set
	psfunc sfunc; // function to call after setting
	UShortT sval;     // numeric value to set for property
	StringT cpval;  // text value to set for property
	StringT ovsec;  // section containing overrides for this
	StringT *match; // array of strings to match against
};

struct secinfo {
	StringT secname;   // key name of section
	enum sectyp sectype;  // kind of values used to find setting keys
	StringT secgroup;  // name of group to which section belongs
	struct setinfo *setarr; // array of settings in section
	bool all;        // true if used when running ProcAllSects
	enum styp stype;      // type of var to set if no array
	VoidT *svar;      // ptr to var to set if no array
	psfunc sfunc;    // function to call after setting if no array
};


// sections

StringT FDKIniSectList = NULL;

struct secinfo FDKIniSects[] = {
 { "FDK", set, "FDKSet", ISecFDK, true },
 { "Setup", set, "FDKSet", ISecFDK, true },
 { "FDK", set, "FDKSet", ISecAutomation, true },
 { "Automation", set, "FDKSet", ISecAutomation, true },

 { "Graphics", set, "Graph", ISecGraph, true },
 { "CSS", set, "CSS", ISecCss, true },
 { "Logging", set, "LoggingSet", ILogOptions, true },

 { "FDK", set, "FileCmd", ISecFileCmd, false },
 { "Automation", set, "FileCmd", ISecFileCmd, false },
 { "BookFileCommands", set, "FileCmd", ISecFileCmd, false },
 { "FDK", set, "BookCmd", ISecBookCmd, false },
 { "Automation", set, "BookCmd", ISecBookCmd, false },
 { "BookFileCommands", set, "BookCmd", ISecBookCmd, false },

 { "HelpOptions", set, "WinHelp", ISecHelp, false },
 { "HelpOptions", set, "WinHelp", ISecWinHelp, false },
 { "HelpContents", set, "WinHelp", ISecHelpCont, false },

 { "MSHtmlHelpOptions", set, "HtmlHelp", ISecHelp, false },
 { "MSHtmlHelpOptions", set, "HtmlHelp", ISecHtmlHelp, false },

 { "JavaHelpOptions", set, "JavaHelp", ISecHelp, false },
 { "JavaHelpOptions", set, "JavaHelp", ISecJavaHelp, false },

 { "OracleHelpOptions", set, "OracleHelp", ISecHelp, false },
 { "OracleHelpOptions", set, "OracleHelp", ISecOracleHelp, false },

 { "OmniHelpOptions", set, "OmniHelp", ISecHelp, false },
 { "OmniHelpOptions", set, "OmniHelp", ISecOmniHelp, false },

 { "EclipseHelpOptions", set, "EclipseHelp", ISecHelp, false },
 { "EclipseHelpOptions", set, "EclipseHelp", ISecEclipseHelp, false },
};


struct setinfo ISecFDK[] = {
 { "UseExistingDCL", yesno, &UseDCL, NULL },
 { "DeleteExistingDCL", yesno, &DeleteExistingDCL, NULL },

 { "HPJFileName", str, &HPJName, NULL },
 { "HHPFileName", str, &HPJName, NULL },
 { "HSFileName", str, &HPJName, NULL },

 { "GraphicsFirst", yesno, &GraphicsFirst, NULL },
 { "ConvertVariables", yesno, &ConvertVars, NULL },
 { "SetFrameConditions", yesno, &SetFrameConditions, NULL },
 { "GenerateBook", yesno, &GenUpBook, NULL },
 { "CheckLinks", yesno, &CheckLinks, NULL },
 { "CheckLinkLog", path, &CheckLinkLog, NULL },
 { "LinkLogAlways", yesno, &LinkLogAlways, NULL },
 { "ConversionDesigner", yesno, &CondDes, NULL },
 { "UseInitDialog", yesno, &UseInit, NULL },
 { "UseDoneDialog", yesno, &UseDone, NULL },
 { "UseExistingMIF", yesno, &UseMIF, NULL },
 { "UseFrame8MIF", yesno, &UseFrame8MIF, NULL },
 { "UseFrame9MIF", yesno, &UseFrame9MIF, NULL },
 { "UseFrame10MIF", yesno, &UseFrame10MIF, NULL },
 { "UseFrame11MIF", yesno, &UseFrame11MIF, NULL },
 { "UseFrame12MIF", yesno, &UseFrame12MIF, NULL },
 // { "DeleteMIF", yesno, &DeleteMIF, NULL },
 { "DeleteExistingMIF", yesno, &DeleteExistingMIF, NULL },
 { "MakeCombinedCnt", yesno, &MakeCNT, NULL },

 { "CompileHelp", yesno, &MakeHLP, NULL },
 { "KeepCompileWindow", yesno, &KeepCompileWindow, NULL },

 { "UseFrameTOC", yesno, &UseTOC, NULL },
 { "UseFrameIX", yesno, &UseIX, NULL },
 { "UseFrameGenFiles", yesno, &UseGenFiles, NULL },
 { "WriteEquations", yesno, &WriteEqns, NULL },
 { "WriteAllGraphics", yesno, &WriteAllGraphics, NULL },
 { "WriteRefPageGraphics", yesno, &WriteRefGraphics, NULL },
 { "WriteMasterPageGraphics", yesno, &WriteMpgGraphics, NULL },
 { "UseGraphicFileID", yesno, &UseGrFileID, NULL },

 { "ApplyTemplateFile", yesno, &ApplyTemplateFile, ApplyTemplateFileSet },
 { "TemplateFileName", path, &TemplateFileName, TemplateFileNameSet },
 { "AppliedTemplateFlags", lnum, &AppliedTemplateFlags, AppliedTemplFlagsSet },
 { "ImportDocProps", yesno, &ImportDocProps, ImportDocPrSet },

 { "IDFileName", path, &CurrFileIDPath, NULL },
 { "PrjFileName", path, &CurrPrjFilePath, NULL },

 { "WordSuffix", str, &WordSuffix, NULL },
 { "WordPerfectSuffix", str, &WordPerfectSuffix, NULL },
 { "HTMLSuffix", str, &HTMLSuffix, NULL },
 { "XMLSuffix", str, &XMLSuffix, NULL },
 { "FileSuffix", str, &FileSuffix, NULL },

 { "MIFSuffix", str, &MIFSuffix, NULL },
 { "MIFBookSuffix", str, &MIFBookSuffix, NULL },
 { "DCLSuffix", str, &DCLSuffix, NULL },

 { "GraphicNameDigits", lnum, &GrFileNumLen, GraphicNameDigitsSet },
 { "EquationExportDPI", lnum, &EqDpiValue, EqDpiValueSet },
 { "GraphicExportDPI", lnum, &GrDpiValue, GrDpiValueSet },
 { "GraphicExportFormat", cmp, &GrFormat, NULL, 0, NULL, NULL, GrFormList },

 // MIF
 { "MakeBookMIF", yesno, &MakeBook, NULL },
 { "OrigExtForMIF", yesno, &OrigExtForMIF, NULL },

 { NULL, spec, NULL, NULL }
};


VoidT
GraphicNameDigitsSet(VoidT)
{
	IntT i = 0;

	if (GrFileNumLen < 6)
		GrFileNumLen = 6;
	else if (GrFileNumLen > 10)
		GrFileNumLen = 10;

	GrFileNumMask = 0;
	for (i = 0; i < GrFileNumLen; i++) {
		GrFileNumMask <<= 4;
		GrFileNumMask += 0x0F;
	}
}

VoidT
GrDpiValueSet(VoidT)
{
	if (GrDpiValue < 50)
		GrDpiValue = 50;
	else if (GrDpiValue > 1200)
		GrDpiValue = 1200;
}

VoidT
EqDpiValueSet(VoidT)
{
	if (EqDpiValue < 50)
		EqDpiValue = 50;
	else if (EqDpiValue > 1200)
		EqDpiValue = 1200;
}

VoidT
ApplyTemplateFileSet(VoidT)
{
	AppliedTemplateFlags = 0;
	AppliedTemplateFlagsSet = False;
	ImportDocPropsSet = False;
}

VoidT
TemplateFileNameSet(VoidT)
{
	TemplateFileName = Unquote(TemplateFileName);
}

VoidT
AppliedTemplFlagsSet(VoidT)
{
	AppliedTemplateFlagsSet = True;
}

VoidT
ImportDocPrSet(VoidT)
{
	ImportDocPropsSet = True;
}


BoolT MakeFts = false;
BoolT MakeJar = false;
BoolT MakeArchive = false;
StringT ArchiveFrom = NULL;
StringT CopyBeforeFiles = NULL;
StringT CopyBeforeFrom = NULL;
StringT CopyAfterFiles = NULL;
StringT CopyAfterFrom = NULL;


struct setinfo ISecAutomation[] = {
 { "CompileHelp", yesno, &MakeHLP, NULL },
 { "KeepCompileWindow", yesno, &KeepCompileWindow, NULL },

 { "WrapAndShip", yesno, &WrapAndShip, NULL },
 { "CopyGraphicsFrom", str, &CopyGraphicsFrom, NULL },
 { "CopyCssFrom", str, &CopyCSSFrom, NULL },
 { "WrapPath", str, &WrapPath, NULL },
 { "ShipPath", str, &ShipPath, NULL },
 { "WrapCopyFiles", str, &WrapCopyFiles, NULL },
 { "GraphCopyFiles", str, &GraphCopyFiles, NULL },
 { "CssCopyFiles", str, &CSSCopyFiles, NULL },
 { "EmptyWrapPath", yesno, &EmptyWrapPath, NULL },
 { "EmptyGraphPath", yesno, &EmptyGraphPath, NULL },
 { "EmptyOutputDir", cmp, &EmptyOutputDir, NULL, 0, NULL, NULL, EmptyOutputDirList },
 { "EmptyOutputFiles", str, &EmptyOutputFiles, NULL },
 { "DeleteExistingMIF", yesno, &DeleteExistingMIF, NULL },
 { "LogAuto", yesno, &LogAuto, NULL },
 { "OnlyAuto", yesno, &OnlyAuto, NULL },

 { "ArchiveCommand", str, &ArchiveCommand, NULL },
 { "ArchiveStartParams", str, &ArchiveStartParams, NULL },
 { "ArchiveEndParams", str, &ArchiveEndParams, NULL },

 { "ArchiveName", str, &ArchiveName, NULL },
 { "ArchiveVer", str, &ArchiveVer, NULL },
 { "ArchiveExt", str, &ArchiveExt, NULL },
 { "MoveArchive", yesno, &MoveArchive, NULL },

 { "RunfmDiagnostics", yesno, &RunfmDiagnostics, NULL },

  // new additions ****
 { "MakeFTS", yesno, &MakeFts, NULL },
 { "MakeJar", yesno, &MakeJar, NULL },
 { "MakeArchive", yesno, &MakeArchive, NULL },
 { "ArchiveFrom", str, &ArchiveFrom, NULL },

 { "CopyBeforeFiles", str, &CopyBeforeFiles, NULL },
 { "CopyBeforeFrom", str, &CopyBeforeFrom, NULL },
 { "CopyAfterFiles", str, &CopyAfterFiles, NULL },
 { "CopyAfterFrom", str, &CopyAfterFrom, NULL },

 { "OldAuto", yesno, &OldAuto, NULL },

 { NULL, spec, NULL, NULL }
};


struct setinfo ISecFileCmd[] = {
 { "FileStartCommand", str, &FileStartCommand, NULL },
 { "FileWrapCommand", str, &FileWrapCommand, NULL },
 { "FileEndCommand", str, &FileEndCommand, NULL },
 { "FileCommandWindow", cmp, &FileCommandWindow, NULL, 0, NULL, NULL, BookCommandWindowList },
 { "AskForUserVars", cmp, &AskForUserVars, NULL, 0, NULL, NULL, UserVarList },
 { NULL, spec, NULL, NULL }
};

struct setinfo ISecBookCmd[] = {
 { "BookStartCommand", str, &BookStartCommand, NULL },
 { "BookWrapCommand", str, &BookWrapCommand, NULL },
 { "BookEndCommand", str, &BookEndCommand, NULL },
 { "BookCommandWindow", cmp, &BookCommandWindow, NULL, 0, NULL, NULL, BookCommandWindowList },
 { "AskForUserVars", cmp, &AskForUserVars, NULL, 0, NULL, NULL, UserVarList },
 { NULL, spec, NULL, NULL }
};


struct setinfo ISecGraph[] = {
 { "UseOriginalGraphicNames", yesno, &UseOrigGrName, NULL },
 { "GraphPath", str, &GraphPath, NULL },
 { NULL, spec, NULL, NULL }
};

struct setinfo ISecCss[] = {
 { "CssPath", str, &CSSPath, NULL },
 { NULL, spec, NULL, NULL }
};


// LoggingSet

BoolT UseLog = true;
BoolT ShowLog = true;
StringT LogFileName = "_m2g_log.txt";
StringT HistoryFileName = "_m2g_history.txt";
StringT EditorFileName = "notepad.exe";
UCharT LogLevels[logmax] = { 0, 1, 1, 1, 1, 0 };

struct setinfo ILogOptions[] = {
 { "UseLog", yesno, &UseLog, NULL },
 { "ShowLog", yesno, &ShowLog, NULL },
 { "LogFileName", path, &LogFileName, NULL },
 { "HistoryFileName", path, &HistoryFileName, NULL },
 { "EditorFileName", path, &EditorFileName, NULL },
 { "LogErrors", ucnum, &LogLevels[logerr], NULL },
 { "LogWarnings", ucnum, &LogLevels[logwarn], NULL },
 { "LogQuerys", ucnum, &LogLevels[logquery], NULL },
 { "LogInfo", ucnum, &LogLevels[loginfo], NULL },
 { "LogDebug", ucnum, &LogLevels[logdebug], NULL },
 { NULL, spec, NULL, NULL }
};


// help systems

struct setinfo ISecHelp[] = {
 { "DefaultTopic", str, &DefaultTopicFile, NULL },
 { "DefaultTopicFile", str, &DefaultTopicFile, NULL },
 { "HelpFileTitle", str, &HelpTitle, NULL },
 { "ListType", cmp, &ListType, NULL, 0, NULL, NULL, ListTypeList },
 { "RefFileType", cmp, &RefFileType, NULL, 0, NULL, NULL, RefFileTypeList },

 { NULL, spec, NULL, NULL }
};

struct setinfo ISecWinHelp[] = {
 { "HPJFileName", str, &HPJName, NULL },
 { "MakeCombinedCnt", yesno, &MakeCNT, NULL },
 { "Compiler", str, &Compiler, NULL },

 { NULL, spec, NULL, NULL }
};

struct setinfo ISecHelpCont[] = {
 { "CntType", cmp, &CntType, NULL, 0, NULL, NULL, CntTypeList },
 { "CntName", str, &CntName, NULL },
 { "CntStartFile", str, &CntStartFile, NULL },
 { "CntBase", str, &CntBase, NULL },
 { "CntTitle", str, &CntTitle, NULL },

 { NULL, spec, NULL, NULL }
};

struct setinfo ISecHtmlHelp[] = {
 { "HHPFileName", str, &HPJName, NULL },
 { "WriteHelpProjectFile", yesno, &WriteHelpProjectFile, NULL },
 { "Compiler", str, &Compiler, NULL },
 { "UseFTS", yesno, &UseFTS, NULL },

 { NULL, spec, NULL, NULL }
};

struct setinfo ISecJavaHelp[] = {
 { "HSFileName", str, &HPJName, NULL },
 { "WriteHelpSetFile", yesno, &WriteHelpSetFile, NULL },
 { "HSPathNames", yesno, &HSPathNames, NULL },
 { "HTMLSubdir", str, &HTMLSubdir, NULL },
 { "GraphSubdir", str, &GraphSubdir, NULL },

 { "UseFTS", yesno, &UseFTS, NULL },
 { "FTSCommand", str, &FTSCommand, NULL },
 { "JarCommand", str, &JarCommand, NULL },

 { "JavaRootFiles", str, &JavaRootFiles, NULL },
 { "EmptyJavaHTMLSubdir", yesno, &EmptyJavaHTMLSubdir, NULL },
 { "EmptyJavaGraphSubdir", yesno, &EmptyJavaGraphSubdir, NULL },

 { NULL, spec, NULL, NULL }
};

struct setinfo ISecOracleHelp[] = {
 { "HSFileName", str, &HPJName, NULL },
 { "WriteHelpSetFile", yesno, &WriteHelpSetFile, NULL },
 { "HSPathNames", yesno, &HSPathNames, NULL },
 { "HTMLSubdir", str, &HTMLSubdir, NULL },
 { "GraphSubdir", str, &GraphSubdir, NULL },

 { "UseFTS", yesno, &UseFTS, NULL },
 { "FTSCommand", str, &FTSCommand, NULL },

 { "JavaRootFiles", str, &JavaRootFiles, NULL },
 { "EmptyJavaHTMLSubdir", yesno, &EmptyJavaHTMLSubdir, NULL },
 { "EmptyJavaGraphSubdir", yesno, &EmptyJavaGraphSubdir, NULL },

 { NULL, spec, NULL, NULL }
};

struct setinfo ISecOmniHelp[] = {
 { "ProjectName", str, &HPJName, NULL },
 { "DefaultTopic", str, &DefaultTopicFile, NULL },
 { "DefaultTopicFile", str, &DefaultTopicFile, NULL },

 { "OHViewPath", str, &OHVName, NULL },
 { "OHVFiles", str, &OHVFiles, NULL },
 { "OHProjFileXhtml", yesno, &OHProjFileXhtml, NULL },

 { NULL, spec, NULL, NULL }
};

struct setinfo ISecEclipseHelp[] = {
 { "UseManifest", yesno, &EclipseUseManifest, NULL },
 { "ZipCommand", str, &ZipCommand, NULL },
 { "ZipParams", str, &ZipParams, NULL },

 { NULL, spec, NULL, NULL }
};


StringT IniNameList[PROJ_TYPES] = {
 "_m2rtf.ini", "_m2rtf.ini", "_m2rtf.ini",
 "_m2winhelp.ini", "_m2winhelp.ini",
 "_m2html.ini", "_m2javahelp.ini", "_m2htmlhelp.ini", "_m2omnihelp.ini",
 "_m2oraclehelp.ini", "_m2eclipse.ini", "_m2xhtml.ini", "_m2dita.ini",
 "_m2docbook.ini", "_m2xml.ini", "_m2mif.ini", "_m2dcl.ini"
// 'S', 'T'
};
StringT CurrIniName = NULL;

StringT FullFormList[PROJ_TYPES] = {
 "rtf", "rtf", "rtf",
 "winhelp", "winhelp",
 "html", "javahelp", "htmlhelp", "omnihelp",
 "oraclehelp", "eclipse", "xhtml", "dita",
 "docbook", "xml", "mif", "dcl"
};
StringT CurrFullForm = NULL;

StringT Setup = "Setup";
BoolT HasFDK = False;
BoolT HasSetup = False;
BoolT RenameFDK = False;

StringT FrameBook = NULL;
StringT FrameChapter = NULL;
StringT ProjectName = NULL;


// functions


// process ini chain when FormType is known
// for books, process at start for book commands
//  and again before each chapter is opened
// close ini immediately after processing
// call with FM path and basename set

BoolT ProcessIni(StringT path, StringT base,
 BoolT atstart, BoolT isbook)
{
	StringT apath = Fm2a(path);
	StringT abase = Fm2a(base);
	StringT inipath = NULL;
	StringT lptr = NULL;

	// determine ini name based on format
	StringT ininame = IniNameList[FormType - 1];
	UCharT fmt = TypeIdList[FormType - 1];

	CurrFullForm = FullFormList[FormType - 1];

	if (CurrIniName) {
		F_StrFree(CurrIniName);
		CurrIniName = NULL;
	}

	inipath = MakeFileName(apath, ininame);

	if (!FileExists(inipath)) {  // no new ini, try old
		F_StrFree(inipath);
		inipath = MakeFileName(apath, StdIni);
	}
	if (!FileExists(inipath)) {  // no old ini either
		F_StrFree(inipath);
		F_StrFree(apath);
		F_StrFree(abase);
		return False;
	}

	CurrIniName = inipath;

	M2GSetIni(inipath, abase);

	M2GIniSectList();  // gets list of sections nul-separated
	RenameFDK = HasFDK = HasSetup = False;
	if (FDKIniSectList) {  // see if FDK and not Setup
		lptr = FDKIniSectList;
		while (*lptr) {
			if (!HasFDK && !F_StrICmp(lptr, "FDK"))
				HasFDK = True;
			else if (!HasSetup && !F_StrICmp(lptr, "Setup"))
				HasSetup = True;
			lptr += F_StrLen(lptr) + 1;
		}
		if (HasFDK) // && !HasSetup)  if both, make Setup an orphan
			RenameFDK = True;
	}

	M2GIniSects();  // reads all FDK settings and one Graphics

	// read sections for help
	switch (fmt) {
		case 'H':
			M2GIniGroup("WinHelp");
			break;

		case 'M':
			M2GIniGroup("HtmlHelp");
			break;

		case 'J':
			M2GIniGroup("JavaHelp");
			break;

		case 'O':
			M2GIniGroup("OracleHelp");
			break;

		case 'C':
			M2GIniGroup("OmniHelp");
			break;

		case 'E':
			M2GIniGroup("EclipseHelp");
			break;

		default:
			break;
	}

	// handle overrides
	switch (fmt) {

		case 'B':
		case 'D':
			ListFileUsed = True;
			ListType = LST_BOTH;
			break;

		case 'C':
			MakeCNT = True;
			ListType = LST_BOTH;
			break;

		case 'H':
			if ((FormType == PT_WH4)
			 && (CntType == 1))
				MakeCNT = False;
			break;

		default:
			break;
	}

	if ((RefFileType == RF_BODY)  // book
	 && (ListType > LST_NONE))
		ListFileUsed = True;

	if (WordSuffix == NULL)
		WordSuffix = RtfExt;
	if (WordPerfectSuffix == NULL)
		WordPerfectSuffix = RtfExt;
	if (HTMLSuffix == NULL)
		HTMLSuffix = HtmExt;
	if (XMLSuffix == NULL)
		XMLSuffix = XmlExt;

	if (MIFSuffix == NULL)
		MIFSuffix = MifExt;
	if (MIFBookSuffix == NULL)
		MIFBookSuffix = MIFSuffix;
	if (DCLSuffix == NULL)
		DCLSuffix = DclExt;

	if (FileSuffix) {
		WordSuffix = WordPerfectSuffix = FileSuffix;
		HTMLSuffix = XMLSuffix = FileSuffix;
		//MIFSuffix = DCLSuffix = FileSuffix;
	}


	if (atstart)
		M2GIniGroup(isbook ? "BookCmd" : "FileCmd");

	// close ini now to prevent conflict with updates
	CallM2GFrame("IniProcDone ");

	F_StrFree(apath);
	F_StrFree(abase);

	return True;
}



// write any updated variables to file at end of process
// after closing ini but before running dcl conversion
// call with values set, use dcfilp

VoidT UpdateIni(VoidT)
{
	StringT Setup = "Setup";

	// update CurrIniName
	if (RenameFDK) {
		M2GIniPRenSect(CurrIniName, "FDK", Setup);
		RenameFDK = False;
		if (HasSetup)
			M2GLogEvent(logwarn, 1, "[FDK] renamed to [Setup], ", 
				"but [Setup] already existed.", NULL, NULL, NULL);
	}

	// SetIniStr(in, "FDK", "PluginVersion", PluginVersion);
	M2GIniPutSetting(CurrIniName, Setup, "PluginVersion", PluginVersion);
	M2GIniPutSetting(CurrIniName, Setup, "ProjectName", ProjectName);
	M2GIniPutSetting(CurrIniName, Setup, "FrameBook", FrameBook);
	M2GIniPutSetting(CurrIniName, Setup, "FrameChapter", FrameChapter);

	if (WriteAllGraphics)  /* ensure graphics will be used */
		// SetIniTF(in, "Graphics", "UseGraphicPreviews", True);
		M2GIniPutBool(CurrIniName, "Graphics", "UseGraphicPreviews", True);
	else if (WriteEqns)    /* or that eqns will be seen */
		// SetIniTF(in, "Graphics", "UseGraphicPreviews", False);
		M2GIniPutBool(CurrIniName, "Graphics", "UseGraphicPreviews", False);

	// SetIniTF(in, "FDK", "WriteEquations", WriteEqns);
	M2GIniPutBool(CurrIniName, Setup, "WriteEquations", WriteEqns);
	// SetIniTF(in, "FDK", "WriteAllGraphics", WriteAllGraphics);
	M2GIniPutBool(CurrIniName, Setup, "WriteAllGraphics", WriteAllGraphics);

	if (PT_htm(FormType))
		// SetIniTF(in, "Graphics", "UseOriginalGraphicNames", UseOrigGrName);
		M2GIniPutBool(CurrIniName, "Graphics", "UseOriginalGraphicNames", UseOrigGrName);

#if 0
	if (FileSuffix) {
		M2GIniPutSetting(CurrIniName, Setup, "FileSuffix", FileSuffix);
 		if (FormType == PT_MIF)
			M2GIniPutSetting(CurrIniName, Setup, "MIFBookSuffix", MIFBookSuffix);
	}
	else {
		if (FormType == PT_WORDP)
			// SetIniStr(in, "FDK", "WordPerfectSuffix", WordPerfectSuffix);
			M2GIniPutSetting(CurrIniName, Setup, "WordPerfectSuffix", WordPerfectSuffix);
		else if (PT_word(FormType))
			// SetIniStr(in, "FDK", "WordSuffix", WordSuffix);
			M2GIniPutSetting(CurrIniName, Setup, "WordSuffix", WordSuffix);
 		else if (PT_xml(FormType))
			// SetIniStr(in, "FDK", "XMLSuffix", XMLSuffix);
			M2GIniPutSetting(CurrIniName, Setup, "XMLSuffix", XMLSuffix);
		else if (PT_htm(FormType)) {
			// SetIniStr(in, "FDK", "HTMLSuffix", HTMLSuffix);
			M2GIniPutSetting(CurrIniName, Setup, "HTMLSuffix", HTMLSuffix);
			//if (CurrPrjFilePath)
				// SetIniStr(in, "FDK", "PrjFileName", CurrPrjFilePath);
			//	M2GIniPutSetting(CurrIniName, Setup, "PrjFileName", CurrPrjFilePath);
		}
 		else if (FormType == PT_MIF) {
			// SetIniStr(in, "FDK", "MIFSuffix", MIFSuffix);
			M2GIniPutSetting(CurrIniName, Setup, "MIFSuffix", MIFSuffix);
			// SetIniStr(in, "FDK", "MIFBookSuffix", MIFBookSuffix);
			M2GIniPutSetting(CurrIniName, Setup, "MIFBookSuffix", MIFBookSuffix);
		}
 		else if (FormType == PT_DCL)
			// SetIniStr(in, "FDK", "DCLSuffix", DCLSuffix);
			M2GIniPutSetting(CurrIniName, Setup, "DCLSuffix", DCLSuffix);
	}

	if (CurrFileIDPath)
		// SetIniStr(in, "FDK", "IDFileName", CurrFileIDPath);
		M2GIniPutSetting(CurrIniName, Setup, "IDFileName", CurrFileIDPath);
#endif

	// SetIniTF(in, "FDK", "UseExistingMIF", UseMIF);
	M2GIniPutBool(CurrIniName, Setup, "UseExistingMIF", UseMIF);
	// SetIniTF(in, "FDK", "DeleteMIF", DeleteMIF);
	//M2GIniPutBool(CurrIniName, Setup, "DeleteMIF", False);
	// SetIniTF(in, "FDK", "UseExistingDCL", UseDCL);
	M2GIniPutBool(CurrIniName, Setup, "UseExistingDCL", UseDCL);

	//if (PT_wh(FormType))
		// SetIniTF(in, "HelpOptions", "MakeCombinedCnt", MakeCNT);
	//	M2GIniPutBool(CurrIniName, "HelpOptions", "MakeCombinedCnt", MakeCNT);

	// SetIniTF(in, "Automation", "WrapAndShip", WrapAndShip);
	M2GIniPutBool(CurrIniName, "Automation", "WrapAndShip", WrapAndShip);

	if (PT_comp(FormType))
		// SetIniTF(in, "Automation", "CompileHelp", MakeHLP);
		M2GIniPutBool(CurrIniName, "Automation", "CompileHelp", MakeHLP);

	M2GIniPUpdate(CurrIniName);
}


VoidT CloseIni(VoidT)
{
	M2GIniPClose(CurrIniName);
}



// processing for dcfilp uses


VoidT M2GIniGetSetting(StringT fpath, StringT sect, StringT key, StringT *vp)
{
	StringT arg = NULL;
	UIntT val = (UIntT) vp;
	StringT valp = F_StrNew(2048);
	StringT nvalp = NULL;
	*vp = valp;

	arg = F_StrNew(F_StrLen(fpath) + F_StrLen(sect) + F_StrLen(key) + 32);
	sprintf(arg, "IniGetSetting \"%s\" [%s]%s=%ld", fpath, sect, key, val);

	CallM2GFrame(arg);
	F_StrFree(arg);
	nvalp = F_StrCopyString(valp);
	*vp = nvalp;
	F_StrFree(valp);
}


VoidT M2GIniPutSetting(StringT fpath, StringT sect, StringT key, StringT val)
{
	StringT arg = NULL;

	arg = F_StrNew(F_StrLen(fpath) + F_StrLen(sect) + F_StrLen(key) + F_StrLen(val) + 22);
	sprintf(arg, "IniPutSetting \"%s\" [%s]%s=%s", fpath, sect, key,
		                                             val ? val : "");

	CallM2GFrame(arg);
	F_StrFree(arg);
}


VoidT M2GIniPRenSect(StringT fpath, StringT sect, StringT nsect)
{
	StringT arg = NULL;

	arg = F_StrNew(F_StrLen(fpath) + F_StrLen(sect) + F_StrLen(nsect) + 17);
	sprintf(arg, "IniPRenSect \"%s\" %s=%s", fpath, sect, nsect);

	CallM2GFrame(arg);
	F_StrFree(arg);
}


VoidT M2GIniPutBool(StringT fpath, StringT sect, StringT key, BoolT val)
{
	M2GIniPutSetting(fpath, sect, key, val ? "Yes" : "No");
}


VoidT M2GIniPutInt(StringT fpath, StringT sect, StringT key, IntT val)
{
	static UCharT buf[20];

	M2GIniPutSetting(fpath, sect, key, _itoa(val, buf, 10));
}


VoidT M2GIniPUpdate(StringT fpath)
{
	StringT arg = NULL;

	arg = F_StrNew(F_StrLen(fpath) + 11);
	sprintf(arg, "IniPUpdate %s", fpath);

	CallM2GFrame(arg);
	F_StrFree(arg);
}


VoidT M2GIniPClose(StringT fpath)
{
	StringT arg = NULL;

	arg = F_StrNew(F_StrLen(fpath) + 11);
	sprintf(arg, "IniPClose %s", fpath);

	CallM2GFrame(arg);
	F_StrFree(arg);
}

#endif



// end of m2rinid.c

