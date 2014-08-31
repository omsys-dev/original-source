/* m2rini.c is the FDK ini file handler for mif2rtf FM 5.5  
 * Last edited on 11/28/98 by Jeremy H. Griffith.
 * Copyright 1998 Omni Systems, Inc.  All Rights Reserved.
 */

#include "m2rbook.h"


/* globals */

static UCharT IniBuf[MAX_PROFILE];
#define MAX_KEYLIST 4096
static UCharT SectBuf[MAX_KEYLIST];
static StringT SectPart = NULL;


#ifndef NEWINI

/* new ini functionality via m2gframe */
typedef VoidT (*psfunc)(VoidT);

typedef UByteT bool;
#define false 0
#define true 1

enum styp {    // type of var to be set
 spec, yesno, chr, cap, str, sarr, mac, cmp,
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
AppliedTemplFlagsSet(VoidT)
{
	AppliedTemplateFlagsSet = True;
}

VoidT
ImportDocPrSet(VoidT)
{
	ImportDocPropsSet = True;
}


struct setinfo ISecFDK[] = {
 { "HPJFileName", str, &HPJName, NULL },
 { "HHPFileName", str, &HPJName, NULL },
 { "HSFileName", str, &HPJName, NULL },

 { "GraphicsFirst", yesno, &GraphicsFirst, NULL },
 { "ConvertVariables", yesno, &ConvertVars, NULL },
 { "SetFrameConditions", yesno, &SetFrameConditions, NULL },
 { "GenerateBook", yesno, &GenUpBook, NULL },
 { "CheckLinks", yesno, &CheckLinks, NULL },
 { "CheckLinkLog", str, &CheckLinkLog, NULL },
 { "LinkLogAlways", yesno, &LinkLogAlways, NULL },
 { "ConversionDesigner", yesno, &CondDes, NULL },
 { "UseInitDialog", yesno, &UseInit, NULL },
 { "UseDoneDialog", yesno, &UseDone, NULL },
 { "UseExistingMIF", yesno, &UseMIF, NULL },
 { "UseFrame8MIF", yesno, &UseFrame8MIF, NULL },
 { "UseFrame9MIF", yesno, &UseFrame9MIF, NULL },
 { "DeleteMIF", yesno, &DeleteMIF, NULL },
 { "DeleteExistingMIF", yesno, &DeleteExistingMIF, NULL },
 { "UseExistingDCL", yesno, &UseDCL, NULL },
 { "DeleteExistingDCL", yesno, &DeleteExistingDCL, NULL },
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
 { "TemplateFileName", str, &TemplateFileName, NULL },
 { "AppliedTemplateFlags", lnum, &AppliedTemplateFlags, AppliedTemplFlagsSet },
 { "ImportDocProps", yesno, &ImportDocProps, ImportDocPrSet },

 { "IDFileName", str, &CurrFileIDPath, NULL },
 { "PrjFileName", str, &CurrPrjFilePath, NULL },

 { "WordSuffix", str, &WordSuffix, NULL },
 { "WordPerfectSuffix", str, &WordPerfectSuffix, NULL },
 { "HTMLSuffix", str, &HTMLSuffix, NULL },
 { "XMLSuffix", str, &XMLSuffix, NULL },
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

// { "UseLocalFileID", yesno, &UseLocalFileID, NULL },
// { "EquationFrameExpand", snum, &EqFrExpand, NULL },
 { NULL, spec, NULL, NULL }
};


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

 { NULL, spec, NULL, NULL }
};


struct setinfo ISecHtmHelp[] = {
 { "HPJFileName", str, &HPJName, NULL },
 { "MakeCombinedCnt", yesno, &MakeCNT, NULL },

 { "HHPFileName", str, &HPJName, NULL },
 { "HSFileName", str, &HPJName, NULL },
 { "ProjectName", str, &HPJName, NULL },
 { "Compiler", str, &Compiler, NULL },

 { "ListType", cmp, &ListType, NULL, 0, NULL, NULL, ListTypeList },
 { "RefFileType", cmp, &RefFileType, NULL, 0, NULL, NULL, RefFileTypeList },

 { "WriteHelpSetFile", yesno, &WriteHelpSetFile, NULL },
 { "WriteHelpProjectFile", yesno, &WriteHelpProjectFile, NULL },
 { "HSPathNames", yesno, &HSPathNames, NULL },
 { "HTMLSubdir", str, &HTMLSubdir, NULL },
 { "GraphSubdir", str, &GraphSubdir, NULL },
 { "JavaRootFiles", str, &JavaRootFiles, NULL },
 { "EmptyJavaHTMLSubdir", yesno, &EmptyJavaHTMLSubdir, NULL },
 { "EmptyJavaGraphSubdir", yesno, &EmptyJavaGraphSubdir, NULL },
 { "UseManifest", yesno, &EclipseUseManifest, NULL },

 { "ZipCommand", str, &ZipCommand, NULL },
 { "ZipParams", str, &ZipParams, NULL },

 { "UseFTS", yesno, &UseFTS, NULL },
 { "FTSCommand", str, &FTSCommand, NULL },
 { "JarCommand", str, &JarCommand, NULL },
 { "OHViewPath", str, &OHVName, NULL },
 { "OHVFiles", str, &OHVFiles, NULL },
 { "OHProjFileXhtml", yesno, &OHProjFileXhtml, NULL },

 { "DefaultTopic", str, &DefaultTopicFile, NULL },
 { "DefaultTopicFile", str, &DefaultTopicFile, NULL },
 { "HelpFileTitle", str, &HelpTitle, NULL },

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

struct setinfo ISecCss[] = {
 { "CssPath", str, &CSSPath, NULL },
 { NULL, spec, NULL, NULL }
};

struct setinfo ISecGraph[] = {
 { "UseOriginalGraphicNames", yesno, &UseOrigGrName, NULL },
 { "GraphPath", str, &GraphPath, NULL },
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

// LoggingSet


static BoolT UseLog = true;
static StringT LogFileName = "_m2g_log.txt";
static UCharT LogLevels[logmax] = { 0, 1, 1, 1, 1, 0 };

struct setinfo ILogOptions[] = {
 { "UseLog", yesno, &UseLog, NULL },
 { "LogFileName", str, &LogFileName, NULL },
 { "LogErrors", ucnum, &LogLevels[logerr], NULL },
 { "LogWarnings", ucnum, &LogLevels[logwarn], NULL },
 { "LogQuerys", ucnum, &LogLevels[logquery], NULL },
 { "LogInfo", ucnum, &LogLevels[loginfo], NULL },
 { "LogDebug", ucnum, &LogLevels[logdebug], NULL },
 { NULL, spec, NULL, NULL }
};


struct secinfo FDKIniSects[] = {
 { "FDK", set, "FDKSet", ISecFDK, true },
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

 { "HelpOptions", set, "HtmHelp", ISecHtmHelp, false },
 { "MSHtmlHelpOptions", set, "HtmHelp", ISecHtmHelp, false },
 { "JavaHelpOptions", set, "HtmHelp", ISecHtmHelp, false },
 { "OmniHelpOptions", set, "HtmHelp", ISecHtmHelp, false },
 { "EclipseHelpOptions", set, "HtmHelp", ISecHtmHelp, false },
 { "HelpContents", set, "WinHelp", ISecHelpCont, false },
};

#endif


VoidT CallM2GFrame(StringT str)
{
	StringT arg = NULL;
	IntT ret = FE_Success;

	arg = F_StrNew(F_StrLen(str) + 6);
	sprintf(arg, "m2rb:%s", str);
	ret = F_ApiCallClient("M2GFrame", arg);
	F_StrFree(arg);
}



VoidT M2GOpenProject(StringT proj)
{
	StringT arg = NULL;
	StringT levs = F_StrNew(logmax + 1);
	IntT i = 0;
	StringT prj;

	if (!UseLog)
		return;
	prj = Fm2a(proj);
	for (i = 0; i < logmax; i++)
		levs[i] = LogLevels[i] + '0';
	levs[i] = '\0';

	arg = F_StrNew(F_StrLen(PluginVersion) + F_StrLen(prj)
	               + F_StrLen(LogFileName) + F_StrLen(HistoryFileName)
	               + F_StrLen(EditorFileName) + F_StrLen(CurrDir)
	               + logmax + 20);
	sprintf(arg, "OpenProject %s,%s,%s%s,%s,%s,%s,%s",
		               PluginVersion, prj, levs, LogFileName,
		               HistoryFileName, EditorFileName, CurrDir,
	                 (ShowLog ? "1" : "0"));

	CallM2GFrame(arg);
	F_StrFree(arg);
	F_Free(prj);
}


VoidT M2GCloseProject(BoolT final)
{
	StringT arg = NULL;

	if (!UseLog)
		return;

	arg = F_StrNew(15);
	sprintf(arg, "CloseProject %c", final ? '1' : '0');

	CallM2GFrame(arg);
	F_StrFree(arg);

	if (final) {  // restore defaults
		LogFileName = "_m2g_log.txt";
		LogLevels[logerr] = 1;
		LogLevels[logwarn] = 1;
		LogLevels[logquery] = 1;
		LogLevels[loginfo] = 1;
		LogLevels[logdebug] = 0;
	}
}


VoidT M2GOpenLog(StringT fname)
{
	StringT arg = NULL;
	StringT fn;

	if (!UseLog)
		return;

	fn = Fm2a(fname);
	arg = F_StrNew(F_StrLen(fn) + 9);
	sprintf(arg, "OpenLog %s", fn);

	CallM2GFrame(arg);
	F_StrFree(arg);
	F_Free(fn);
}


VoidT M2GCloseLog(BoolT final)
{
	StringT arg = NULL;

	if (!UseLog)
		return;

	arg = F_StrNew(11);
	sprintf(arg, "CloseLog %c", final ? '1' : '0');

	CallM2GFrame(arg);
	F_StrFree(arg);
}


VoidT M2GLogEvent(UCharT lt, UCharT sev, StringT d1, StringT d2,
 StringT d3, StringT d4, StringT d5)
{
	StringT arg = NULL;

	if (!UseLog)
		return;

	arg = F_StrNew((d1 ? F_StrLen(d1) : 0) + (d2 ? F_StrLen(d2) : 0)
               + (d3 ? F_StrLen(d3) : 0) + (d4 ? F_StrLen(d4) : 0)
               + (d5 ? F_StrLen(d5) : 0) + 12);

	sprintf(arg, "LogEvent %c%c%s%s%s%s%s", lt + '0', sev + '0', (d1 ? d1 : ""),
                (d2 ? d2 : ""), (d3 ? d3 : ""), (d4? d4 : ""), (d5 ? d5 : ""));

	CallM2GFrame(arg);
	F_StrFree(arg);
}



VoidT M2GGetOpenFile(StringT title, StringT path, StringT *pname)
{
	StringT arg = NULL;

	arg = F_StrNew(45);
	sprintf(arg, "GetOpenFile %ld %ld %ld",
		               (UIntT) title, (UIntT) path, (UIntT) pname);

	CallM2GFrame(arg);
	F_StrFree(arg);
}


VoidT M2GGetOpenDir(StringT title, StringT path, StringT *pname)
{
	StringT arg = NULL;

	arg = F_StrNew(44);
	sprintf(arg, "GetOpenDir %ld %ld %ld",
		               (UIntT) title, (UIntT) path, (UIntT) pname);

	CallM2GFrame(arg);
	F_StrFree(arg);
}


VoidT M2GGetMyDocDir(StringT pname)
{
	StringT arg = NULL;

	arg = F_StrNew(23);
	sprintf(arg, "GetMyDocDir %ld", (UIntT) pname);

	CallM2GFrame(arg);
	F_StrFree(arg);
}


VoidT M2GWaitFileDone(StringT pname)
{
	StringT arg = NULL;

	arg = F_StrNew(24);
	sprintf(arg, "WaitFileDone %ld", (UIntT) pname);

	CallM2GFrame(arg);
	F_StrFree(arg);
}


VoidT M2GSetIni(StringT iname, StringT fname)
{
	StringT arg = NULL;

	if (fname) {
		arg = F_StrNew(F_StrLen(iname) + F_StrLen(fname) + 15);
		sprintf(arg, "IniSetup \"%s\" \"%s\"", iname, fname);
	} 
	else {
		arg = F_StrNew(F_StrLen(iname)+ 12);
		sprintf(arg, "IniSetup \"%s\"", iname);
	}
	CallM2GFrame(arg);
	F_StrFree(arg);
}

VoidT M2GIniSects(VoidT)
{
	StringT arg = NULL;

	arg = F_StrNew(21);
	sprintf(arg, "IniSects %ld", (UIntT) FDKIniSects);

	CallM2GFrame(arg);
	F_StrFree(arg);
}

VoidT M2GIniSectList(VoidT)
{
	StringT arg = NULL;

	arg = F_StrNew(24);
	sprintf(arg, "IniSectList %ld", (UIntT) &FDKIniSectList);

	CallM2GFrame(arg);
	F_StrFree(arg);
}

VoidT M2GIniGroup(StringT group)
{
	StringT arg = NULL;

	arg = F_StrNew(21);
	sprintf(arg, "IniGroup %s", group);

	CallM2GFrame(arg);
	F_StrFree(arg);
}

VoidT M2GIniGetString(StringT sect, StringT key, StringT *val)
{
	StringT arg = NULL;

	arg = F_StrNew(F_StrLen(sect) + F_StrLen(key) + 25);
	sprintf(arg, "IniString [%s]%s=%ld", sect, key, (UIntT) val);

	CallM2GFrame(arg);
	F_StrFree(arg);
}


VoidT SetIniBuf(StringT ininame, StringT section, StringT key)
{
	StringT inistr = NULL;

	if ((LastIniSet
	  && !strcmp(LastIniSet, ininame))
	 || (LastSpecIniSet
	  && !strcmp(LastSpecIniSet, ininame)))  // call config chain
		M2GIniGetString(section, key, &inistr);

	if (inistr
	 && (F_StrLen(inistr) < MAX_PROFILE))
		F_StrCpy(IniBuf, inistr);
	else
		GetPrivateProfileString(section, key, "", IniBuf, MAX_PROFILE, ininame);
}

#if 0

VoidT M2GIniGetMacroString(StringT sect, StringT key, StringT *val)
{
	StringT arg = NULL;

	arg = F_StrNew(F_StrLen(sect) + F_StrLen(key) + 30);
	sprintf(arg, "IniMacroString [%s]%s=%ld", sect, key, (UIntT) val);

	CallM2GFrame(arg);
	F_StrFree(arg);
}


VoidT SetIniBufMacro(StringT section, StringT key)
{
	StringT inistr = NULL;
	M2GIniGetMacroString(section, key, &inistr);

	if (inistr
	 && (F_StrLen(inistr) < MAX_PROFILE))
		F_StrCpy(IniBuf, inistr);
}
#endif

VoidT M2GIniGetSect(StringT sect, StringT *val)
{
	StringT arg = NULL;

	if (!sect) {
		arg = F_StrNew(22);
		sprintf(arg, "IniSect =%ld", (UIntT) val);
	}
	else {
		arg = F_StrNew(F_StrLen(sect) + 24);
		sprintf(arg, "IniSect [%s]=%ld", sect, (UIntT) val);
	}

	CallM2GFrame(arg);
	F_StrFree(arg);
}


VoidT SetSectBuf(StringT ininame, StringT section)
{
	StringT inistr = NULL;

	if ((LastIniSet
	  && !strcmp(LastIniSet, ininame))
	 || (LastSpecIniSet
	  && !strcmp(LastSpecIniSet, ininame)))  // call config chain
		M2GIniGetSect(section, &inistr);

	if (inistr
	 && (F_StrLen(inistr) < MAX_KEYLIST))
		F_StrCpy(SectBuf, inistr);
	else
		GetPrivateProfileString(section, NULL, "", SectBuf, MAX_KEYLIST, ininame);
}


VoidT SetSectionBuf(StringT ininame, StringT section, StringT *bptr)
{
	StringT inistr = NULL;

	if ((LastIniSet
	  && !strcmp(LastIniSet, ininame))
	 || (LastSpecIniSet
	  && !strcmp(LastSpecIniSet, ininame)))  // call config chain
		M2GIniGetSect(section, &inistr);

	if (inistr)
		*bptr = inistr;
}



/* .ini-file read functions */

BoolT IniBufEmpty(VoidT)
{
	return (*IniBuf == '\0');
}

StringT GetIniBuf(VoidT)
{
	return IniBuf;
}


VoidT GetIniMatch(StringT ininame, StringT section, StringT key,
									 StringT *list, IntT *varp)
{
	UCharT ch = 0;
	UCharT ch2 = 0;
	IntT i = 0;
	StringT buf = NULL;

	SetIniBuf(ininame, section, key);
	buf = A2fm(IniBuf);

	while (*buf != 0) {
		if (isspace(*buf))
			buf++;
		else
			break;
	}

	if (*buf == 0)
		return;

	for (i = 0; list[i] != NULL; i++) {
		if (!_stricmp(buf, list[i])) {
			*varp = i + 1;
			return;
		}			
	}		
}


VoidT GetIniUC(StringT ininame, StringT section, StringT key,
									 UCharT *chp)
{
	UCharT ch;
	StringT buf = IniBuf;

	SetIniBuf(ininame, section, key);

	while (*buf != 0) {
		if (isspace(*buf))
			buf++;
		else {
			ch = *buf;
			break;
		}
	}

	if (ch == 0)
		return;

	ch = toupper(ch);

	if (ch)
		*chp = ch;
}


VoidT GetIniInt(StringT ininame, StringT section, StringT key,
							  IntT *varp)
{
	SetIniBuf(ininame, section, key);

	if (*IniBuf)
		*varp = atol(IniBuf);
}


VoidT GetIniMinMax(StringT ininame, StringT section, StringT key,
							  IntT vmin, IntT vmax, IntT *varp)
{
	IntT val;

	SetIniBuf(ininame, section, key);

  /* enforce min and max */
	if (((val = atol(IniBuf)) >= vmin)
	 && (val <= vmax))
		*varp = val;
}


VoidT GetIniPath(StringT ininame, StringT section, StringT key,
								 StringT *path)
{
	StringT buf;

	SetIniBuf(ininame, section, key);

	if (*IniBuf != '\0') {
		buf = A2fm(IniBuf);
		if (((F_StrChr(buf, '/')
			 || F_StrChr(buf, '\\'))
		  && (*buf != '.'))
		 || F_StrChr(buf, ':')) {
			*path = F_StrNew(F_StrLen(buf) + 1);
			F_StrCpy(*path, buf);
		}
		else if ((*buf == '.')
		 && ((*(buf + 1) == '/') || (*(buf + 1) == '\\'))) {
			*path = F_StrNew(F_StrLen(CurrDir) + F_StrLen(buf + 2) + 1);
			F_StrCpy(*path, CurrDir);
			F_StrCat(*path, buf + 2);
		}
		else {
			*path = F_StrNew(F_StrLen(CurrDir) + F_StrLen(buf) + 1);
			F_StrCpy(*path, CurrDir);
			F_StrCat(*path, buf);
		}
		F_Free(buf);
	}
}


VoidT GetIniString(StringT ininame, StringT section, StringT key,
									 StringT *str)
{
	SetIniBuf(ininame, section, key);

	if (*IniBuf != '\0')
		*str = A2fm(IniBuf);

	/* formerly:
		*str = F_StrNew(F_StrLen(IniBuf) + 1);
		F_StrCpy(*str, IniBuf);
	*/
}


VoidT GetRawIniString(StringT ininame, StringT section, StringT key,
									 StringT *str)
{
	SetIniBuf(ininame, section, key);

	if (*IniBuf != '\0') {
		*str = F_StrNew(F_StrLen(IniBuf) + 1);
		F_StrCpy(*str, IniBuf);
	}
}

#if 0

VoidT GetRawMacroString(StringT section, StringT key, StringT *str)
{
	SetIniBufMacro(section, key);

	if (*IniBuf != '\0') {
		*str = F_StrNew(F_StrLen(IniBuf) + 1);
		F_StrCpy(*str, IniBuf);
	}
}

#endif

VoidT GetIniStrInt(StringT ininame, StringT section, IntT kval,
									 StringT *str)
{
	UCharT key[10];

	sprintf(key, "%ld", kval);

	SetIniBuf(ininame, section, key);

	if (*IniBuf != '\0')
		*str = A2fm(IniBuf);
}


VoidT TrueFalseIni(StringT ininame, StringT section, StringT key,
									 BoolT *varp)
{
	UCharT ch = 0;
	StringT buf = IniBuf;

	SetIniBuf(ininame, section, key);

	while (*buf != 0) {
		if (isspace(*buf))
			buf++;
		else {
			ch = *buf;
			break;
		}
	}

	if (ch == 0)
		return;

	ch = toupper(ch);

	if ((ch == 'Y')
	 || (ch == 'J')
	 || (ch == 'O')
	 || (ch == '1')
	 || (ch == 'T'))
		*varp = True;
	else if ((ch == 'N')
	 || (ch == '0')
	 || (ch == 'F'))
		*varp = False;
}



VoidT GetIniSect(StringT ininame, StringT section)
{
	SetSectBuf(ininame, section);
	// GetPrivateProfileString(section, NULL, "", SectBuf, MAX_KEYLIST, ininame);
	SectPart = SectBuf;
}


BoolT CheckSectPart(VoidT)
{
	if (!SectPart)
		return False;

	if (*SectPart == '\0') {
		SectPart = NULL;
		return False;
	}

	if ((SectPart + F_StrLen(SectPart)) > (SectBuf + MAX_KEYLIST)) {
		SectPart = NULL;
		return False;
	}

	return True;
}


StringT GetIniSectKey(VoidT)
{
	StringT str = NULL;

	if (!CheckSectPart())
		return NULL;
	str = A2fm(SectPart);
	SectPart += (F_StrLen(SectPart) + 1);

	return str;
}


// used only for clearing .grx

StringT GetRawIniSectKey(VoidT)
{
	StringT str = NULL;

	if (!CheckSectPart())
		return NULL;
	str = F_StrCopyString(SectPart);
	SectPart += (F_StrLen(SectPart) + 1);

	return str;
}


IntT GetIniSectCount(VoidT)
{
	IntT count = 0;
	StringT str = NULL;

	if (!SectPart)
		return 0;

	if ((*SectPart == '\0')
	 || ((SectPart + F_StrLen(SectPart)) > (SectBuf + MAX_KEYLIST))) {
		SectPart = NULL;
		return 0;
	}

	for (str = SectPart;
	     (str < (SectBuf + MAX_KEYLIST)) && (*str != '\0');
	     str += (F_StrLen(str) + 1))
		count++;

	return count;
}


StringT GetFullIniSect(StringT inifile, StringT name)
{
	//FILE *ifp = NULL;
	//static UCharT sbuf[1024];
	static StringT linebuf = NULL;
	//IntT len = F_StrLen(name);
	//IntT linelen = 0;
	StringT temp = NULL;
	//BoolT inSect = False;

	if (!inifile || !name)
		return NULL;

	SetSectionBuf(inifile, name, &temp);
	if (temp) {
		linebuf = F_StrNew(F_StrLen(temp) + 1);
		F_StrCpy(linebuf, temp);
		return linebuf;
	}
	return NULL;

#if 0
	if (linebuf) {
		F_Free(linebuf);
		linebuf = NULL;
	}
	if ((ifp = fopen(inifile, "rb")) == NULL)
		return NULL;
	while (fgets(sbuf, 1024, ifp)) {
		if ((sbuf[0] == '[')
		 && !F_StrICmpN(name, sbuf + 1, len)
		 && (sbuf[len + 1] == ']'))
			inSect = True;
		else if (inSect) {
			if (sbuf[0] == '[')
				break;
			if (sbuf[0] == ';')
				continue;
			temp = F_StrNew(linelen + F_StrLen(sbuf) + 1);
			if (linelen) {
				F_StrCpy(temp, linebuf);
				F_Free(linebuf);
			}
			F_StrCat(temp, sbuf);
			linebuf = temp;
			linelen = F_StrLen(linebuf);			
		}
	}
	fclose(ifp);
	if (inSect)
		return linebuf;
	return NULL;
#endif
}


// ref-file checking

#define MAXRLINKS 1024
#define MAXBUFSIZ 1024
#define MAXMSGSIZ 1024
#define MAXOVERFL 100

typedef struct {
 IntT id;
 IntT tp;
 StringT fn;
} RLinkT;

typedef struct {
 IntT cnt;
 RLinkT *val[MAXRLINKS];
} RLinksT;


// private vars and functions

RLinksT RefFileLinks;
IntT RefFileLinkStart = 0;
BoolT DocRefLinkErrs = False;
BoolT RefFileLinkOverflow = False;


IntT GetRefLinkErrs(StringT fname)
{
	FILE *ifp = NULL;
	static UCharT sbuf[MAXBUFSIZ];
	UIntT prop = 0;
	StringT refname;
	StringT refpath;
	StringT cpt;
	StringT ept;
	BoolT inSect = False;
	RLinkT *rlink = NULL;

	refname = NewFileExt(BaseFileName(fname), "ref");
	refpath = MakeFileName(CurrDir, refname);
	if ((ifp = fopen(refpath, "rb")) == NULL) {
		F_Free(refname);
		F_Free(refpath);
		return 0;
	}

	if (RefFileLinkOverflow)
		return 0;

	while (fgets(sbuf, MAXBUFSIZ, ifp)) {
		if ((sbuf[0] == '[')
		 && !F_StrICmpN("Links", sbuf + 1, 5)
		 && (sbuf[6] == ']'))
			inSect = True;
		else if (inSect) {
			if (sbuf[0] == '[')
				break;
			if (sbuf[0] == ';')
				continue;
			if ((cpt = F_StrChr(sbuf, '*')) == NULL)
				continue;
			if ((ept = F_StrChr(sbuf, '=')) == NULL)
				continue;
			rlink = (RLinkT *) F_Alloc(sizeof(RLinkT), NO_DSE);
			if (!rlink)
				break;
			RefFileLinks.val[RefFileLinks.cnt++] = rlink;
			*--cpt = '\0';  // space before asterisk is end
			rlink->fn = F_StrCopyString(ept + 1);
			*ept = '\0';
			while (--ept >= sbuf) {
				if (!isdigit(*ept))
					break;
			}
			rlink->id = F_StrAlphaToInt(++ept);
			prop = GetPrivateProfileInt("LinkProps", sbuf, 0, refpath);
			rlink->tp = ((prop & 2) == 2)? 1 : 3;
			if (RefFileLinks.cnt == MAXRLINKS)
				break;
		}
	}
	fclose(ifp);
	F_Free(refname);
	F_Free(refpath);
	return (RefFileLinks.cnt - RefFileLinkStart);
}

VoidT WriteRefLinkErrs(StringT fmname, F_ObjHandleT docId, F_ObjHandleT bookId)
{
	RLinkT *rlink = NULL;
	UCharT emsg[MAXMSGSIZ];
	IntT i = 0;

	if (GetRefLinkErrs(fmname)) {
		DocRefLinkErrs = True;
		if (RefFileLinks.cnt == MAXRLINKS) {
			RefFileLinks.cnt = MAXOVERFL;
			RefFileLinkOverflow = True;
		}
		for (i = RefFileLinkStart; i < RefFileLinks.cnt; i++) {
			rlink = RefFileLinks.val[i]; 
			F_Sprintf(emsg, "log -b=%i -d=%i -o=%i --Link to file \"%s\" is broken.",
				bookId, docId, rlink->id, rlink->fn);
			F_ApiCallClient((StringT) "BookErrorLog", emsg);
		}
		RefFileLinkStart = i;
	}
}

VoidT SaveBookErrorLog(F_ObjHandleT bookId)
{
	F_ObjHandleT logId = 0;
	F_ObjHandleT markId = 0;
	IntT i = 0;
	StringT markStr = NULL;
	StringT mpt = NULL;
  IntT mval = 0;
	RLinkT *rlink = NULL;

	logId = IdentifyDocLabel("Book Error Log");
	if (!logId)
		logId = IdentifyDocName("");
	if (!logId)
		return;
	RefFileLinkStart = 0;
	F_ApiSetInt(FV_SessionId, logId, FP_DocIsViewOnly, False);
	markId = F_ApiGetId(FV_SessionId, logId, FP_FirstMarkerInDoc);
	for ( ; markId != 0;
	     markId = F_ApiGetId(logId, markId, FP_NextMarkerInDoc)) {
		markStr = F_ApiGetString(logId, markId, FP_MarkerText);
		if (!F_StrEqualN(markStr, "openObjectId", 12)
		 || ((mpt = F_StrRChr(markStr, ':')) == NULL))
			continue;
		mval = F_StrAlphaToInt(mpt + 3);
		rlink = RefFileLinks.val[RefFileLinkStart];
		if (!rlink
		 || (rlink->id != mval)) {  // try to resyncronize
			for (i = 0; i < RefFileLinks.cnt; i++) {
				rlink = RefFileLinks.val[i];
				if (!rlink)
					continue;
				if (rlink->id == mval)
					break;
			}
			if (!rlink
			 || (rlink->id != mval))
				continue;
			RefFileLinkStart = i;
		}
		*++mpt = rlink->tp + '0';
		F_ApiSetString(logId, markId, FP_MarkerText, markStr);
		F_Free(rlink->fn);
		F_Free(rlink);
		RefFileLinks.val[RefFileLinkStart++] = NULL;
	}
	if (CheckLinkLog
	 && (RefFileLinks.cnt || LinkLogAlways))
		SaveDoc(logId, CheckLinkLog, True);
	F_ApiSetInt(FV_SessionId, logId, FP_DocIsViewOnly, True);
	if (ProjRemote)
		F_ApiClose(logId, FF_CLOSE_MODIFIED);
}


// public vars and entry point

BoolT CheckLinks = False;
StringT CheckLinkLog = NULL;
BoolT LinkLogAlways = True;

VoidT ProcRefLinkErrs(F_ObjHandleT bookId)
{
	UCharT emsg[MAXMSGSIZ];
	FILE *cfile = NULL;
	F_ObjHandleT compId = 0;
	IntT comptype = 0;
	StringT docname = NULL;
	F_ObjHandleT docId = 0;
	IntT i = 0;
	RLinkT *rlink = NULL;

	SaveActiveDocBook();
	RefFileLinkStart = 0;
	RefFileLinkOverflow = False;
	compId = F_ApiGetId(FV_SessionId, bookId, FP_FirstComponentInBook);
	while (compId) {	/* add filename to list */
		docname = F_ApiGetString(bookId, compId, FP_Name);
		comptype = F_ApiGetInt(bookId, compId, FP_BookComponentType);
		if ((comptype == FV_BK_NOT_GENERATABLE)
		 || ((comptype == FV_BK_TOC) && UseTOC)
		 || ((comptype == FV_BK_INDEX_STAN) && UseIX)
		 || ((comptype != FV_BK_TOC)
			&& (comptype != FV_BK_INDEX_STAN)
			&& UseGenFiles)) {
			docId = OpenDocName(docname);
			DocRefLinkErrs = False;
			WriteRefLinkErrs(docname, docId, bookId);
			if (!DocAlreadyOpen && !DocRefLinkErrs)
				F_ApiClose(docId, FF_CLOSE_MODIFIED);
		}
		F_StrFree(docname);
		compId = F_ApiGetId(bookId, compId, FP_NextComponentInBook);
	}

	if (RefFileLinks.cnt == 0) {
		F_Printf(NULL, "CheckLinks: No link errors found.\n");
		if (LinkLogAlways && !ProjRemote) {
			F_Sprintf(emsg, "log -b=%i -d=0 -o=0 --No broken links found.",
				bookId);
			F_ApiCallClient((StringT) "BookErrorLog", emsg);
			RestoreActiveDocBook();
		}
		return;
	}
	if (RefFileLinkOverflow) {
		F_Printf(NULL, "CheckLinks: Too many link errors, over %d.\n",
		         (IntT) MAXRLINKS);
		F_Printf(NULL, " Only the first %d link errors reported.\n",
		         (IntT) MAXOVERFL);
		for (i = MAXOVERFL; i < MAXRLINKS; i++) {  // clear the rest
			rlink = RefFileLinks.val[i];
			if (!rlink)
				continue;
			F_Free(rlink->fn);
			F_Free(rlink);
			RefFileLinks.val[i] = NULL;
		}
	}
	else if (RefFileLinks.cnt == 1)
		F_Printf(NULL, "CheckLinks: just one link error found.\n");
	else
		F_Printf(NULL, "CheckLinks: %d link errors.\n", RefFileLinks.cnt);

	SaveBookErrorLog(bookId);
	RestoreActiveDocBook();
}



/* ini-file writing */


VoidT SetIniStr(StringT ininame, StringT section, StringT key, StringT val)
{
	StringT temp = Fm2a(val);
	IntT err = 0;
	SetLastError(err);
	if (WritePrivateProfileString(section, key, temp, ininame) == FALSE)
		err = GetLastError();
	F_Free(temp);
}


VoidT SetRawIniStr(StringT ininame, StringT section, StringT key, StringT val)
{
	IntT err = 0;
	SetLastError(err);
	if (WritePrivateProfileString(section, key, val, ininame) == FALSE)
		err = GetLastError();
}


VoidT SetIniInt(StringT ininame, StringT section, StringT key, IntT val)
{
	static UCharT valstr[12];

	sprintf(valstr, "%ld", val);
	WritePrivateProfileString(section, key, (StringT) valstr, ininame);
}


VoidT SetIniTF(StringT ininame, StringT section, StringT key, BoolT val)
{
	WritePrivateProfileString(section, key, val ? "Yes" : "No", ininame);
}


VoidT ClearIniSect(StringT ininame, StringT section)
{
	StringT str = NULL;

	GetIniSect(ininame, section);
	while ((str = GetRawIniSectKey()) != NULL)
		WritePrivateProfileString(section, str, NULL, ininame);
}



/* ini-file editing */

StringT IniEditor = "notepad";

VoidT EditIniFile(StringT ininame)
{
	StringT command;
	StringT in;

	in = Fm2a(ininame);
	command = F_StrNew(F_StrLen(in) + 9);
	sprintf(command, "%s %s", IniEditor, in);

	RunCommand(command, CurrProjPath, False, False, True);
	F_StrFree(command);
	F_Free(in);
}



/* Frame to and from ANSI conversions */

/* These return a StringT, remember to F_Free(str) when done with it */

UCharT F2Amap[256] = {
   0,   0,   0,   0,   0,   0,   0,   0,
   9,  10,0xb6,0xa7,   0,   0,   0,   0,
 ' ', ' ', ' ', ' ', ' ',   0,   0,   0,
   0,   0,   0,   0,   0,   0,   0,   0,

 ' ', '!', '"', '#', '$', '%', '&', '\'',
 '(', ')', '*', '+', ',', '-', '.', '/',
 '0', '1', '2', '3', '4', '5', '6', '7',
 '8', '9', ':', ';', '<', '=', '>', '?',

 '@', 'A', 'B', 'C', 'D', 'E', 'F', 'G',
 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O',
 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W',
 'X', 'Y', 'Z', '[', '\\',']', '^', '_',

 '@', 'a', 'b', 'c', 'd', 'e', 'f', 'g',
 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o',
 'p', 'q', 'r', 's', 't', 'u', 'v', 'w',
 'x', 'y', 'z', '{', '|', '}', '~',   0,

  0xC4, 0xC5, 0xC7, 0xC9, 0xD1, 0xD6, 0xDC, 0xE1,
  0xE0, 0xE2, 0xE4, 0xE3, 0xE5, 0xE7, 0xE9, 0xE8,
  0xEA, 0xEB, 0xED, 0xEC, 0xEE, 0xEF, 0xF1, 0xF3,
  0xF2, 0xF4, 0xF6, 0xF5, 0xFA, 0xF9, 0xFB, 0xFC,

  0x86, 0xB0, 0xA2, 0xA3, 0xA7, 0x95, 0xB6, 0xDF,
  0xAE, 0xA9, 0x99, 0xB4, 0xA8, 0xA6, 0xC6, 0xD8,
  0xD7, 0xB1, 0xF0, 0x8A, 0xA5, 0xB5, 0xB9, 0xB2,
  0xB3, 0xBC, 0xBD, 0xAA, 0xBA, 0xBE, 0xE6, 0xF8,

  0xBF, 0xA1, 0xAC, 0xD0, 0x83, 0xDD, 0xFD, 0xAB,
  0xBB, 0x85, 0xFE, 0xC0, 0xC3, 0xD5, 0x8C, 0x9C,
  0x96, 0x97, 0x93, 0x94, 0x91, 0x92, 0xF7, 0xDE,
  0xFF, 0x9F,  '/', 0xA4, 0x8B, 0x9B,  'f',  'f',

  0x87, 0xB7, 0x82, 0x84, 0x89, 0xC2, 0xCA, 0xC1,
  0xCB, 0xC8, 0xCD, 0xCE, 0xCF, 0xCC, 0xD3, 0xD4,
  0x9A, 0xD2, 0xDA, 0xDB, 0xD9,  'i', 0x88, 0x98,
  0xAF,    0,    0, 0xB0, 0xB8, 0xA8,    0, 0xFF
};


StringT Fm2a(StringT fmname)
{
	StringT aname = NULL;
	StringT stra = NULL;
	StringT strf = NULL;

	//if (FrameVersion >= 8)
	//	return fmname;

	if (fmname == NULL)
		return NULL;

	stra = aname = F_StrCopyString(strf = fmname);
	while (*strf) {
		*stra = F2Amap[*strf++];
		if (*stra)
			stra++;
	}
	*stra = '\0';

	return aname;
}


UCharT A2Fmap[256] = {
   0,   0,   0,   0,   0,   0,   0,   0,
   0,   8,   9,   0,   0,   0,   0,   0,
   0,   0,   0,   0,   0,   0,   0,   0,
   0,   0,   0,   0,   0,   0,   0,   0,

 ' ', '!', '"', '#', '$', '%', '&', '\'',
 '(', ')', '*', '+', ',', '-', '.', '/',
 '0', '1', '2', '3', '4', '5', '6', '7',
 '8', '9', ':', ';', '<', '=', '>', '?',

 '@', 'A', 'B', 'C', 'D', 'E', 'F', 'G',
 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O',
 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W',
 'X', 'Y', 'Z', '[', '\\',']', '^', '_',

 '`', 'a', 'b', 'c', 'd', 'e', 'f', 'g',
 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o',
 'p', 'q', 'r', 's', 't', 'u', 'v', 'w',
 'x', 'y', 'z', '{', '|', '}', '~',   0,

   0,    0, 0xe2, 0xc4, 0xe3, 0xc9, 0xa0, 0xe0,
0xf6, 0xe4, 0xb3, 0xdc, 0xce,    0,    0,    0,
   0, 0xd4, 0xd5, 0xd2, 0xd3, 0xa5, 0xd0, 0xd1,
0xf7, 0xaa, 0xf0, 0xdd, 0xcf,    0,    0, 0xd9,

0x11, 0xc1, 0xa2, 0xa3, 0xdb, 0xb4, 0xad, 0xa4,
0xac, 0xa9, 0xbb, 0xc7, 0xc2, 0x2d, 0xa8, 0xf8,
0xfb, 0xb1, 0xb7, 0xb8, 0xab, 0xb5, 0xa6, 0xe1,
0xfc, 0xb6, 0xbc, 0xc8, 0xb9, 0xba, 0xbd, 0xc0,

0xcb, 0xe7, 0xe5, 0xcc, 0x80, 0x81, 0xae, 0x82,
0xe9, 0x83, 0xe6, 0xe8, 0xed, 0xea, 0xeb, 0xec,
0xc3, 0x84, 0xf1, 0xee, 0xef, 0xcd, 0x85, 0xb0,
0xaf, 0xf4, 0xf2, 0xf3, 0x86, 0xc5, 0xd7, 0xa7,

0x88, 0x87, 0x89, 0x8b, 0x8a, 0x8c, 0xbe, 0x8d,
0x8f, 0x8e, 0x90, 0x91, 0x93, 0x92, 0x94, 0x95,
0xb2, 0x96, 0x98, 0x97, 0x99, 0x9b, 0x9a, 0xd6,
0xbf, 0x9d, 0x9c, 0x9e, 0x9f, 0xc6, 0xca, 0xd8
};


StringT A2fm(StringT aname)
{
	StringT fmname = NULL;
	StringT stra = NULL;
	StringT strf = NULL;

	//if (FrameVersion >= 8)
	//	return aname;

	if (aname == NULL)
		return NULL;

	strf = fmname = F_StrCopyString(stra = aname);
	while (*stra) {
		*strf = A2Fmap[*stra++];
		if (*strf)
			strf++;
	}
	*strf = '\0';
	
	return fmname;
}


/* end of m2rini.c */


