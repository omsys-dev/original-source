/* m2rproc.c is the conversion processor for mif2rtf for FM 5.5
 * it is used by both the m2r and the m2rbook modules.
 * Last edited on 12/2/98 by Jeremy H. Griffith.
 * Copyright 1998 Omni Systems, Inc.  All Rights Reserved.
 */


#include "m2rbook.h"


/* globals */

#define MAX_FILEIO 2048

BoolT IsBookFile = False;
F_ObjHandleT CurrBookId = 0;
F_ObjHandleT MIFdocId = 0;

BoolT WriteAllGraphics = False;
BoolT WriteRefGraphics = True;
BoolT WriteMpgGraphics = True;
BoolT UseOrigGrName = False;
BoolT GraphicsFirst = False;

BoolT WriteEqns = True;
BoolT WriteGraphicsOnly = False;
IntT GrFileNumLen = 6;
IntT GrFileNumMask = 0x7FFFFFFF;

IntT GrDpiValue = 96;
IntT EqDpiValue = 120;
IntT GrFormat = 3;
StringT GrFormList[DES_GRX_FORM_COUNT + 1] = {
 "BMP", "TIFF", "WMF", "JPEG", "PNG",
 "EPS", "PICT", "CGM", "IGES", "GIF",
 "SVG", "PDF",  NULL };
/* value to use for matching pos in GrFormList */
IntT GrFormVal[DES_GRX_FORM_COUNT + 1] = {
 0,
 GF_BMP, GF_TIFF, GF_WMF, GF_JPEG, GF_PNG,
 GF_EPS, GF_PICT, GF_CGM, GF_IGES, GF_GIF,
 GF_SVG, GF_PDF
 };
/* index to GrFormList name for each type */
IntT GrFormMap[DES_GRX_FORM_COUNT + 1] = { 
 3,   /* default to WMF */
 0, 1, 2, 3, 4,
 5, 6, 7, 8, 9,
10, 11
 };

BoolT UseGrFileID = False;
StringT GrPath = NULL;

StringT CurrDir = NULL;
IntT CurrDrive = 0;
StringT OrigDir = NULL;
IntT OrigDrive = 0;

BoolT MIFExisted = False;
BoolT DeleteMIF = False;
BoolT UseMIF = False;
BoolT UseFrame8MIF = True;
BoolT UseFrame9MIF = True;
BoolT UseFrame10MIF = True;
BoolT UseFrame11MIF = True;
BoolT UseFrame12MIF = True;
BoolT DCLExisted = False;
BoolT UseDCL = False;
BoolT NeedFMFile = True;
IntT PathLen = 0;
StringT BaseName = NULL;

IntT FormType = 0;
//StringT FormTypeList[] = {
// "Help", "Standard", "FrameMIF", 
// NULL };

IntT ListType = 0;
StringT ListTypeList[] = {
 "None", "Contents", "Index", "Both",
 NULL };

IntT RefFileType = 0;
StringT RefFileTypeList[] = {
 "None", "Body", "Full", "HHW",
 NULL };

BoolT ListFileUsed = False;
StringT ListFileName = NULL;

BoolT FormOver = 0;
IntT FormLen = 0;
UCharT FormStr[5] = "-fW ";

IntT CntType = 0;
StringT CntName = NULL;
StringT CntStartFile = NULL;
StringT CntBase = NULL;
StringT CntTitle = NULL;
StringT CntTypeList[] = {
 "None", "Body", "Full",
 NULL };

BoolT UseTOC = True;
BoolT UseIX = True;
BoolT UseGenFiles = True;

BoolT UseOHTOC = True;
BoolT UseOHIX = True;
BoolT UseOHFTS = True;
BoolT UseOHREL = True;
StringT OHProjName = NULL;

BoolT MakeCNT = True;
BoolT MakeHLP = False;
BoolT KeepCompileWindow = False;
StringT CopyGraphicsFrom = NULL;
StringT CopyCSSFrom = NULL;

BoolT ApplyTemplateFile = False;
StringT TemplateFileName = NULL;
BoolT AppliedTemplateFlagsSet = False;
IntT AppliedTemplateFlags = 0;
BoolT ImportDocProps = False;
BoolT ImportDocPropsSet = False;
BoolT ConvertVars = False;
BoolT SetFrameConditions = False;
BoolT ConditionsSet = False;
BoolT GenUpBook = False;
BoolT CondDes = False;
//StringT ConfigTemplate = NULL;

StringT FileStartCommand = NULL;
StringT FileWrapCommand = NULL;
StringT FileEndCommand = NULL;
IntT FileCommandWindow = 0;
BoolT FileCommandHide = True;
BoolT FileCommandKeep = False;

StringT BookStartCommand = NULL;
StringT BookWrapCommand = NULL;
StringT BookEndCommand = NULL;
IntT BookCommandWindow = 0;
BoolT BookCommandHide = True;
BoolT BookCommandKeep = False;

BoolT OldAuto = False;

IntT ExitCode = 0;  /* dcl exit code */
IntT ExitType = 0;  /* exit path identifier */
BoolT Quit = False;

StringT BookCommandWindowList[] = {
 "Hide", "Show", "Keep", NULL
};



/* entry points */

VoidT ProcSaveAs(F_ObjHandleT docId, StringT filename)
{
	F_ObjHandleT templateId = 0;
	F_ObjHandleT bookId = 0;

	StringT extension = NULL;
	StringT mifname = NULL;
	StringT dclname = NULL;
	StringT ininame = NULL;
	StringT stdininame = NULL;
	StringT docname = NULL;
	StringT bookname = NULL;
	StringT bkbasename = NULL;
	StringT projdesc = NULL;
	StringT newname = NULL;
	StringT oldfile = NULL;
	StringT pathend = NULL;

	BoolT cancelled = False;
	BoolT templateopen = False;

	/* reinit global variables every time */
	Quit = False;
	UseInit = True;
	UseDone = True;
	DeleteMIF = False;
	UseMIF = False;
	UseFrame8MIF = True;
	UseFrame9MIF = True;
	UseFrame10MIF = True;
	UseFrame11MIF = True;
	UseFrame12MIF = True;
	GraphicsFirst = False;
	WriteAllGraphics = False;
	WriteEqns = True;
	GrFileNumLen = 8;
	GrFileNumMask = 0x7FFFFFFF;
	IsBookFile = False;
	CurrBookId = 0;
	UseGrFileID = False;
	GrPath = NULL;
	BaseName = NULL;
	HPJName = NULL;
	CntName = NULL;
	ConvertVars = False;
	ApplyTemplateFile = False;
	TemplateFileName = NULL;
	AppliedTemplateFlagsSet = False;
	ImportDocProps = False;
	AppliedTemplateFlags = 0;
	ListFileUsed = False;
	KeepCompileWindow = False;
	FileStartCommand = NULL;
	FileWrapCommand = NULL;
	FileEndCommand = NULL;
	FileCommandWindow = 0;
	FileCommandHide = True;
	FileCommandKeep = False;

#if 0
	if (((extension = F_StrRChr(filename, '.')) == NULL)
	 || (F_StrICmp(extension, RtfExt)
		&& F_StrICmp(extension, DocExt)
		&& F_StrICmp(extension, DclExt)
		&& F_StrICmp(extension, MifExt)
		&& F_StrICmp(extension, XmlExt)
		&& F_StrICmp(extension, HtmExt)
		&& F_StrICmp(extension, HtmlExt))) {
		F_ApiAlert(FileTypeErr, FF_ALERT_CONTINUE_WARN);
		return;  /* not the extensions we expect */
	}
#endif

	if ((PathLen = SetPathName(&CurrDir, filename, True)) == 0) {
		F_ApiAlert(FilePathErr, FF_ALERT_CONTINUE_WARN);
		return;  /* invalid path, must have a separator */
	}

	/* change current directory to path of dest file */
	if (CurrDir[1] == ':')
		CurrDrive = (toupper(CurrDir[0]) - 'A') + 1;

	OrigDrive = _getdrive();
	OrigDir = F_StrNew(_MAX_PATH + 1);
	_getcwd(OrigDir, _MAX_PATH);

	if (CurrDrive != OrigDrive)
		_chdrive(CurrDrive);

	ChangeDir(CurrDir);

	GrPath = CurrDir;
	BaseName = BaseFileName(filename);

	SetStrings(FormType);

	/* get info from .ini files mif2rtf.ini and file.ini */
#ifdef NEWINI
	if (!ProcessIni(CurrDir, BaseName, True, False)) {
		F_ApiAlert(IniMissing, FF_ALERT_CONTINUE_NOTE);
		F_StrFree(mifname);
		F_StrFree(dclname);
		if (CurrDrive != OrigDrive)
			_chdrive(OrigDrive);
		_chdir(OrigDir);
		F_StrFree(OrigDir);
		F_StrFree(CurrDir);
		SetupExport();
		return;
	}

#else
	stdininame = F_StrNew(PathLen + F_StrLen(StdIni));
	LastSpecIniSet = ininame = NewFileExt(filename, ".ini");
	F_StrCpy(stdininame, CurrDir);
	F_StrCat(stdininame, StdIni);
	if (!FileExists(stdininame)) {
		if (!FileExists(ininame)) {
			F_ApiAlert(IniMissing, FF_ALERT_CONTINUE_NOTE);
			//cancelled = True;
			F_StrFree(mifname);
			F_StrFree(dclname);
			F_StrFree(ininame);
			if (CurrDrive != OrigDrive)
				_chdrive(OrigDrive);
			_chdir(OrigDir);
			F_StrFree(OrigDir);
			F_StrFree(CurrDir);
			F_StrFree(stdininame);
			SetupExport();
			return;
		}
		else 
			GetIniSettings(ininame, NULL);  /* just filename.ini */
	}
	else
		GetIniSettings(stdininame, BaseName);
#endif

	M2GOpenProject(CurrProject);
	M2GOpenLog(F_ApiGetString(FV_SessionId, docId, FP_Name));

	/* use GrPath for all graphics */
	if ((GrPath == CurrDir)
	 && BaseName
	 && UseGrFileID)
		GetFileIDSetting(ininame, BaseName);

#ifndef NEWINI
	M2GIniGroup("FileCmd");
#endif
	FileCommandHide = (FileCommandWindow < 2) ? True : False;
	FileCommandKeep = (FileCommandWindow == 4) ? True : False;
	
	mifname = NewFileExt(filename, (FormType == PT_MIF) ? MIFSuffix : MifExt);
	MIFExisted = FileExists(mifname);
	dclname = NewFileExt(filename, (FormType == PT_DCL) ? DCLSuffix : DclExt);
	DCLExisted = FileExists(dclname);
	CurrGrxFile = Fm2a(NewFileExt(filename, GrxExt));

	FrameChapter = NULL;
	FrameBook = NULL;
	docname = F_ApiGetString(FV_SessionId, docId, FP_Name);
	if (docname) {
		FrameChapter = Fm2a(docname);
		bookId = IdentifyBookOfDoc(docname);
		if (bookId) {
			bookname = F_ApiGetString(FV_SessionId, bookId, FP_Name);
			FrameBook = Fm2a(bookname);
		}
	}

	/* use m2rinit dialog */
	if (!cancelled) {
		if (UseInitDialog(filename)) {
#ifdef NEWINI
			UpdateIni();
#else
			if (FileExists(stdininame))
				UpdateIniSettings(stdininame);
			else
				UpdateIniSettings(ininame);
#endif
		}
		else
			cancelled = True;

		if (!cancelled
		 && PT_htm(FormType)) {
			bookId = IdentifyBookOfDoc(docname);
			if (bookId) {
				bookname = F_ApiGetString(FV_SessionId, bookId, FP_Name);
				bkbasename = BaseFileName(bookname);
				BuildListFile(bookId, bkbasename);
				F_StrFree(bkbasename);
				F_StrFree(bookname);
			}
		}
	}

	/* do the actual conversion operation */

	if (cancelled) {
cancel:
		ExitType = 6;
		ExitCode = 0;
		ReportResults(filename);
		if (mifname)
			F_StrFree(mifname);
		F_StrFree(ininame);
	}
	else if (OnlyAuto
	 && (WrapAndShip
		|| (MakeHLP && PT_comp(FormType))
		|| ((FTSCommand || JarCommand) && PT_java(FormType)))) {
		if (OldAuto)
			WrapShipProc(docId, HPJName);
		else
			RunSystemCommands(False, False);
		F_ApiSetString(FV_SessionId, docId, FP_StatusLine, M2GProjectDone);
		if (UseDone && !ProjRemote)
			F_ApiAlert(FileProcessDone, FF_ALERT_CONTINUE_NOTE);
	}
	else {
		newname = NewFileExt(filename, DestSuffix);
		filename = newname;

		ProcUserVars(CurrIniName);
		//ProcUserVars(FileExists(stdininame) ? stdininame : ininame);
		if (Quit)
			goto cancel;

		stdininame = F_StrCopyString(CurrIniName);
		ininame = F_StrCopyString(CurrIniName);

		if (OldAuto) {
			if (FileStartCommand)
				ProcBookCommand(docId, FileStartCommand, BF_START_CMD, BF_FILE,
						 FileCommandHide, FileCommandKeep, stdininame, ininame);

			if (WrapAndShip
			 || (MakeHLP && PT_comp(FormType))
			 || ((FTSCommand || JarCommand) && PT_java(FormType)))
				WrapShipPreProc(False, docId, HPJName);
		}
		else
			RunSystemCommands(False, True);

		docname = F_ApiGetString(FV_SessionId, docId, FP_Name);

		if ((ApplyTemplateFile || ConvertVars || SetFrameConditions)
		 && !(UseMIF && MIFExisted)
		 && !(UseDCL && DCLExisted)) {
			/* if .mif, make it .fm */
			if (((extension = F_StrRChr(docname, '.')) != NULL)
			 && !F_StrICmp(extension, ".mif")) {
				F_StrCpy(extension, ".fm");
				DeleteMIF = False;
			}
			if (ApplyTemplateFile) {
				if ((templateId = OpenDocName(TemplateFileName)) == 0) {
					M2GLogEvent(logwarn, 1, "Cannot open template ", TemplateFileName, NULL, NULL, NULL);
					F_Printf(NULL, "Cannot open template %s\n", TemplateFileName);
					ApplyTemplateFile = False;
					if (F_ApiAlert(TemplateFailed, FF_ALERT_NO_DEFAULT) == -1)
						Quit = True;
				}
				else {
					templateopen = DocAlreadyOpen;
					F_ApiSetInt(FV_SessionId, templateId, FP_IsIconified, True);
					SaveDoc(docId, docname, False);
					ShowStatus(docId, filename, StatTemplate);
					FA_errno = FE_Success;
					ImportDocTemplate(docId, templateId);
					if (FA_errno != FE_Success) {
						M2GLogEvent(logwarn, 1, "Failure importing template ", TemplateFileName, NULL, NULL, NULL);
						F_Printf(NULL, "Failure importing template %s\n", TemplateFileName);
						Quit = True;
					}
					ClearStatus(docId);
					F_Printf(NULL, "Imported template %s\n", TemplateFileName);
				}
			}
			if (ConvertVars) {
				ShowStatus(docId, filename, StatConvVars);
				FA_errno = FE_Success;
				ConvertAllVars(docId);
				if (FA_errno != FE_Success) {
					M2GLogEvent(logwarn, 1, "Failure converting system variables", NULL, NULL, NULL, NULL);
					Quit = True;
				}
				ClearStatus(docId);
			}
			if (SetFrameConditions) {
				ConditionsSet = False;
				ShowStatus(docId, filename, StatSetConds);
				FA_errno = FE_Success;
				SetConditions(docId, FileExists(stdininame) ? stdininame : ininame);
				if (FA_errno != FE_Success) {
					M2GLogEvent(logwarn, 1, "Failure setting conditions", NULL, NULL, NULL, NULL);
					Quit = True;
				}
				ClearStatus(docId);
			}
		}

		if (OrigExtForMIF
		 && (FormType == PT_MIF)) {
			F_StrFree(mifname);
			mifname = F_StrCopyString(docname);
			oldfile = mifname;
			if (((pathend = F_StrRChr(mifname, '\\')) != NULL)
			 || ((pathend = F_StrRChr(mifname, '/')) != NULL))
				mifname = pathend + 1;
			mifname = MakeFileName(CurrDir, mifname);
			F_StrFree(oldfile);
		}

		if (!Quit)
			ConvertOneFile(docId, filename, mifname, dclname);
		F_StrFree(mifname);

		if ((ApplyTemplateFile || ConvertVars || ConditionsSet)
		 && !(UseMIF && MIFExisted)
		 && !(UseDCL && DCLExisted)) {
			RevertDoc(&docId);
			if (ApplyTemplateFile
			 && (templateopen == False))
				F_ApiClose(templateId, FF_CLOSE_MODIFIED);
			ConditionsSet = False;
		}

		if ((ExitType == 6) && (ExitCode == 1))
			Quit = True;


		if (!Quit
		 && PT_htm(FormType)) {
			bookId = IdentifyBookOfDoc(docname);
			if (bookId) {
				ShowStatus(bookId, bkbasename, (FormType == PT_DITA) ? StatDITA :
																			 ((FormType == PT_DCBK) ? StatDocBook : StatList));
				bookname = F_ApiGetString(FV_SessionId, bookId, FP_Name);
				bkbasename = BaseFileName(bookname);
				if (!ListFileName)
					BuildListFile(bookId, bkbasename);
				if (ListFileUsed)
					RunFileList();
				ClearStatus(bookId);
				F_StrFree(bkbasename);
				F_StrFree(bookname);
			}
		}

		if (!Quit
		 && (FormType == PT_WH4)
		 && MakeCNT) {
			bookId = IdentifyBookOfDoc(docname);
			if (bookId) {
				if (CntName)
					BuildCntFile(bookId, CntName);
				else {
					bookname = F_ApiGetString(FV_SessionId, bookId, FP_Name);
					bkbasename = BaseFileName(bookname);
					BuildCntFile(bookId, bkbasename);
					F_StrFree(bkbasename);
					F_StrFree(bookname);
				}
			}
		}

		if (!Quit) {
			if (OldAuto) {
				if (FileWrapCommand)
					ProcBookCommand(docId, FileWrapCommand, BF_WRAP_CMD, BF_FILE,
						 FileCommandHide, FileCommandKeep, stdininame, ininame);

				if (WrapAndShip
					|| (MakeHLP && PT_comp(FormType))
					|| ((FTSCommand || JarCommand) && PT_java(FormType)))
					WrapShipProc(docId, HPJName);

				if (FileEndCommand)
					ProcBookCommand(docId, FileEndCommand, BF_END_CMD, BF_FILE,
						 FileCommandHide, FileCommandKeep, stdininame, ininame);
			}
			else
				RunSystemCommands(False, False);
		}

		if (Quit
		 && !((ExitType == 6) && (ExitCode == 1))) {
			ExitType = 6;
			ExitCode = 0;
		}

		ReportResults(filename);


		if (!Quit) {
			F_ApiSetString(FV_SessionId, docId, FP_StatusLine, M2GProjectDone);
			if (UseDone && !ProjRemote)
				F_ApiAlert(FileProcessDone, FF_ALERT_CONTINUE_NOTE);
		}

		F_StrFree(docname);
		F_StrFree(newname);
		F_StrFree(ininame);
	}

	/* clean up and restore */
#ifdef NEWINI
	CloseIni();
#endif

	if (CurrDrive != OrigDrive)
		_chdrive(OrigDrive);
	_chdir(OrigDir);
	F_StrFree(OrigDir);
	F_StrFree(CurrDir);
	F_StrFree(stdininame);

	LastSpecIniSet = NULL;
	CallM2GFrame("MacroProcDone ");
	CallM2GFrame("IniProcDone ");
	M2GLogEvent(loginfo, 1, "Project finished", NULL, NULL, NULL, NULL);
	M2GCloseProject(True);
}


VoidT ProcSaveAsBook(F_ObjHandleT bookId, StringT bookname, StringT destpath)
{
	F_ObjHandleT docId = 0;
	F_ObjHandleT compId = 0;
	F_ObjHandleT templateId = 0;

	IntT comptype = 0;

	StringT docname = NULL;
	StringT basename = NULL;
	StringT bkbasename = NULL;
	StringT filename = NULL;
	StringT oldfile = NULL;
	StringT pathend = NULL;
	StringT mifname = NULL;
	StringT dclname = NULL;
	StringT bkininame = NULL;
	StringT ininame = NULL;
	StringT bctname = NULL;
	StringT newname = NULL;
	StringT projdesc = NULL;
	StringT lastTemplate = NULL;

	FILE *cfile = NULL;

	BoolT cancelled = False;
	BoolT opened = False;
	BoolT unopened = False;
	BoolT applyThisTemplateFile = False;
	BoolT templateopen = False;
	BoolT gen1 = False;
	BoolT gen2 = False;

	BoolT	sUseMIF = UseMIF = False;
	BoolT	sDeleteMIF = DeleteMIF = False;
	BoolT	sUseDCL = UseDCL = False;
	BoolT	sWriteEqns = WriteEqns = True;
	BoolT	sWriteAllGraphics = WriteAllGraphics = False;
	IntT	sEqDpiValue = EqDpiValue;
	IntT	sGrDpiValue = GrDpiValue;
	IntT	sGrFormat = GrFormat;
	BoolT	sUseGrFileID = UseGrFileID;
	BoolT	sConvertVars = ConvertVars;
	BoolT sApplyTemplateFile = False;
	StringT sTemplateFileName = NULL;
	BoolT sAppliedTemplateFlagsSet = False;
	BoolT sImportDocProps = False;
	IntT sAppliedTemplateFlags = 0;

	/* reinit global variables every time */
	SavingBook = True;

	Quit = False;
	UseInit = True;
	UseDone = True;
	GraphicsFirst = False;
	GrPath = NULL;
	BaseName = NULL;
	HPJName = NULL;
	GrFileNumLen = 8;
	GrFileNumMask = 0x7FFFFFFF;
	CntType = 0;
	CntName = NULL;
	CntBase = NULL;
	CntTitle = NULL;
	CntStartFile = NULL;
	ConvertVars = False;
	GenUpBook = False;
	ApplyTemplateFile = False;
	TemplateFileName = NULL;
	AppliedTemplateFlagsSet = False;
	ImportDocProps = False;
	AppliedTemplateFlags = 0;
	ListFileUsed = False;
	KeepCompileWindow = False;
	BookStartCommand = NULL;
	BookWrapCommand = NULL;
	BookEndCommand = NULL;
	BookCommandWindow = 0;
	BookCommandHide = True;
	BookCommandKeep = False;

	/* running from a book */
	IsBookFile = True;
	MIFExisted = False;
	CurrBookId = bookId;

	/* path is not necessarily that of book file */
	PathLen = F_StrLen(destpath) + 1;
	if ((destpath[PathLen - 2] != '/')
	 && (destpath[PathLen - 2] != '\\')) { /* ensure trailing sep */
		CurrDir = F_StrNew(++PathLen);
		F_StrCpy(CurrDir, destpath);
		CurrDir[PathLen - 2] = F_StrChr(bookname, '/') ? '/' : '\\';
		CurrDir[PathLen - 1] = '\0';
	}
	else
		CurrDir = F_StrCopyString(destpath);

	ChangeDir(CurrDir);

	if (CurrDir[1] == ':')
		CurrDrive = (_toupper(CurrDir[0]) - 'A') + 1;

	OrigDrive = _getdrive();
	OrigDir = F_StrNew(_MAX_PATH + 1);
	_getcwd(OrigDir, _MAX_PATH);

	if (CurrDrive)
		_chdrive(CurrDrive);
	ChangeDir(CurrDir);

	GrPath = CurrDir;
	SetStrings(FormType);

#ifdef NEWINI
	if (!ProcessIni(CurrDir, NULL, True, True)) {
		F_ApiAlert(IniMissing, FF_ALERT_CONTINUE_NOTE);
		F_StrFree(mifname);
		F_StrFree(dclname);
		if (CurrDrive != OrigDrive)
			_chdrive(OrigDrive);
		_chdir(OrigDir);
		F_StrFree(OrigDir);
		F_StrFree(CurrDir);
		SetupExport();
		return;
	}

#else
	/* get info from mif2rtf.ini */
	bkininame = F_StrNew(PathLen + F_StrLen(StdIni));
	F_StrCpy(bkininame, CurrDir);
	F_StrCat(bkininame, StdIni);
	if (!FileExists(bkininame)) {
		F_ApiAlert(IniMissing, FF_ALERT_CONTINUE_NOTE);
		//cancelled = True;
		F_StrFree(bkininame);
		_chdrive(OrigDrive);
		_chdir(OrigDir);
		F_StrFree(OrigDir);
		F_StrFree(CurrDir);
		SetupBkExport(bookId);
		return;
	}

	GetIniSettings(bkininame, NULL);
#endif

	M2GOpenProject(CurrProject);
	M2GOpenLog(bookname);

	/* use GrPath for all graphics */
	if ((GrPath == CurrDir)
	 && BaseName
	 && UseGrFileID)
		GetFileIDSetting(ininame, BaseName);

#ifndef NEWINI
	M2GIniGroup("BookCmd");
#endif
	BookCommandHide = (BookCommandWindow < 2) ? True : False;
	BookCommandKeep = (BookCommandWindow == 3) ? True : False;
	gen1 = GenUpBook;

	FrameBook = Fm2a(bookname);
	FrameChapter = NULL;

	/* use m2rinit dialog */
	if (!cancelled) {
		projdesc = F_StrNew(F_StrLen(CurrProject) + F_StrLen(CurrProjPath) + 3);
		sprintf(projdesc, "%s: %s", CurrProject, CurrProjPath);
		if (UseInitDialog(projdesc)) {
#ifdef NEWINI
			UpdateIni();
#else
			UpdateIniSettings(bkininame);
#endif
		}
		else
			cancelled = True;
		F_StrFree(projdesc);
		if (PT_htm(FormType)) {
			bookname = F_ApiGetString(FV_SessionId, bookId, FP_Name);
			bkbasename = BaseFileName(bookname);
			BuildListFile(bookId, bkbasename);
			F_StrFree(bkbasename);
			F_StrFree(bookname);
		}
	}

	/* do the actual conversion operation */

	if (cancelled) {
cancel:
		ExitType = 6;
		ExitCode = 0;
		ReportResults(bookname);
	}
	else if (OnlyAuto
	 && (WrapAndShip
		|| (MakeHLP && PT_comp(FormType))
		|| ((FTSCommand || JarCommand) && PT_java(FormType)))) {
		if (OldAuto)
			WrapShipProc(bookId, HPJName);
		else
			RunSystemCommands(True, False);
		F_ApiSetString(FV_SessionId, bookId, FP_StatusLine, M2GProjectDone);
		if (UseDone && !ProjRemote)
			F_ApiAlert(BookProcessDone, FF_ALERT_CONTINUE_NOTE);
	}
	else {
		sUseMIF = UseMIF;
		sDeleteMIF = DeleteMIF;
		sUseDCL = UseDCL;
		sWriteEqns = WriteEqns;
		sWriteAllGraphics = WriteAllGraphics;
		sEqDpiValue = EqDpiValue;
		sGrDpiValue = GrDpiValue;
		sGrFormat = GrFormat;
		sUseGrFileID = UseGrFileID;
		sConvertVars = ConvertVars;
		sApplyTemplateFile = ApplyTemplateFile;
		sTemplateFileName = TemplateFileName;
		sAppliedTemplateFlagsSet = AppliedTemplateFlagsSet;
		sImportDocProps = ImportDocProps;
		sAppliedTemplateFlags = AppliedTemplateFlags;

		ProcUserVars(CurrIniName);
		//ProcUserVars(bkininame);
		if (Quit)
			goto cancel;

		if (OldAuto) {
			if (BookStartCommand)
				ProcBookCommand(bookId, BookStartCommand, BF_START_CMD, BF_BOOK,
												BookCommandHide, BookCommandKeep, bkininame, NULL);

			if (WrapAndShip
			 || (MakeHLP && PT_comp(FormType))
			 || ((FTSCommand || JarCommand) && PT_java(FormType)))
				WrapShipPreProc(True, docId, HPJName);
		}
		else
			RunSystemCommands(True, True);

		/* start the .cnt file */
		if (FormType == PT_WH4) {
			bkbasename = BaseFileName(bookname);
			StartCntFile(bkbasename, &cfile);
			F_StrFree(bkbasename);
		}

		if ((FormType == PT_MIF)
		 && MakeBook) {
			// must save file in original dir with temp name, then move
			filename = NewFileExt(bookname, "tmb");
			if (!SaveMIFforDCL(bookId, filename)) /* could not save */
				F_Printf(NULL, ReportMIFFail, (StringT) filename);
			else { // move it to where it goes
				oldfile = filename;
				if (OrigExtForMIF
				 || !F_StrICmpN(MIFSuffix, ".fm", 3))
					filename = F_StrCopyString(bookname);
				else
					filename = NewFileExt(bookname, MIFBookSuffix);
				if (((pathend = F_StrRChr(filename, '\\')) != NULL)
				 || ((pathend = F_StrRChr(filename, '/')) != NULL))
					pathend++;
				pathend = MakeFileName(CurrDir, pathend ? pathend : filename);
				if (FileExists(pathend))
					DeleteFile(pathend);
				MoveFile(oldfile, pathend);
				F_StrFree(oldfile);
				F_StrFree(pathend);
				F_StrFree(filename);
				filename = NULL;
			}
			F_StrFree(filename);
			filename = NULL;
		}

ProcBookFiles:
		/* while getting filenames from book */
		for (compId = F_ApiGetId(FV_SessionId, bookId, FP_FirstComponentInBook);
		 compId; compId = F_ApiGetId(bookId, compId, FP_NextComponentInBook)) {
			comptype = F_ApiGetInt(bookId, compId, FP_BookComponentType);
			if (((comptype == FV_BK_TOC) && !UseTOC)
			 || ((comptype == FV_BK_INDEX_STAN) && !UseIX)
			 || ((comptype != FV_BK_NOT_GENERATABLE)
			  && (comptype != FV_BK_TOC)
			  && (comptype != FV_BK_INDEX_STAN)
			  && !UseGenFiles))
				continue;
			docname = F_ApiGetString(bookId, compId, FP_Name);
			if (!FileExists(docname)) {
				F_StrFree(docname);
				continue;
			}

			/* put filename into correct dest path as .rtf */
			basename = BaseFileName(docname);
			filename = F_StrNew(PathLen + F_StrLen(basename) + 4);
			F_StrCpy(filename, CurrDir);
			F_StrCat(filename, basename);
			F_StrCat(filename, PT_htm(FormType) ? HtmExt :
			                   ((FormType == PT_DCL) ? DclExt :
			                   ((FormType == PT_MIF) ? MIFSuffix : RtfExt)));

			/* create .mif name too */
			if (OrigExtForMIF
			 && (FormType == PT_MIF)) {
				mifname = F_StrCopyString(docname);
				//SetCurrPath(&mifname);
				oldfile = mifname;
				if (((pathend = F_StrRChr(mifname, '\\')) != NULL)
				 || ((pathend = F_StrRChr(mifname, '/')) != NULL))
					mifname = pathend + 1;
				mifname = MakeFileName(CurrDir, mifname);
				F_StrFree(oldfile);
			}
			else
				mifname = NewFileExt(filename, MIFSuffix ? MIFSuffix : MifExt);
			MIFExisted = FileExists(mifname);
			dclname = NewFileExt(filename, DclExt);
			DCLExisted = FileExists(dclname);
			CurrGrxFile = Fm2a(NewFileExt(filename, GrxExt));

			/* reinit .ini options for each file */
			UseMIF = sUseMIF;
			DeleteMIF = sDeleteMIF;
			UseDCL = sUseDCL;
			WriteEqns = sWriteEqns;
			WriteAllGraphics = sWriteAllGraphics;
			EqDpiValue = sEqDpiValue;
			GrDpiValue = sGrDpiValue;
			GrFormat = sGrFormat;
			UseGrFileID = sUseGrFileID;
			ConvertVars = sConvertVars;
			ApplyTemplateFile = sApplyTemplateFile;
			TemplateFileName = sTemplateFileName;
			AppliedTemplateFlagsSet = sAppliedTemplateFlagsSet;
			ImportDocProps = sImportDocProps;
			AppliedTemplateFlags = sAppliedTemplateFlags;

#ifdef NEWINI
			if (!ProcessIni(CurrDir, basename, False, True)) {
				F_StrFree(filename);
				F_StrFree(docname);
				F_StrFree(basename);
				F_StrFree(mifname);
				F_StrFree(dclname);
				continue;
			}

#else
			LastSpecIniSet = ininame = NewFileExt(filename, IniExt);
			GetIniSettings(bkininame, basename);  /* allow overrides now */
			if (!UsingFile(basename)) {
				F_StrFree(filename);
				F_StrFree(docname);
				F_StrFree(basename);
				F_StrFree(mifname);
				F_StrFree(dclname);
				continue;
			}
#endif
			M2GCloseLog(True);
			M2GOpenLog(filename);

			/* use GrPath for all graphics */
			if ((GrPath == CurrDir)
			 && BaseName
			 && UseGrFileID)
				GetFileIDSetting(ininame, BaseName);

			newname = NewFileExt(filename, DestSuffix);
			F_StrFree(filename);
			filename = newname;
			opened = unopened = False;

			docId = IdentifyDocName(docname);

			if (gen2) {
				if (docId == 0) {  /* no use if not opened */
					F_StrFree(filename);
					F_StrFree(basename);
					F_StrFree(dclname);
					F_StrFree(docname);
					continue;
				}
				opened = True;
			}
			else if (docId == 0) { /* not open */
				if ((UseMIF && MIFExisted)
				 || (UseDCL && DCLExisted))
					NeedFMFile = False;
				else
					NeedFMFile = True;
				if (((ApplyTemplateFile || ConvertVars || SetFrameConditions)
				  && NeedFMFile)
				 || WriteEqns
				 || WriteAllGraphics
				 || NeedFMFile
				 || gen1) {
					ShowStatus(docId, docname, StatOpening);
					docId = OpenDocName(docname);
					ClearStatus(docId);
					opened = True;
				}
				else
					unopened = True;
			}
			else if ((ApplyTemplateFile || ConvertVars || SetFrameConditions || gen1)
			 && !(UseMIF && MIFExisted)
			 && !(UseDCL && DCLExisted))  /* is open, so force save now */
				SaveDoc(docId, docname, False);
			F_StrFree(docname);

			if ((docId != 0)
			 || unopened) {
				if (!gen2) {
					/* import formats if needed */
					applyThisTemplateFile = False;
					if ((ApplyTemplateFile || ConvertVars || SetFrameConditions)
					 && !(UseMIF && MIFExisted)
					 && !(UseDCL && DCLExisted)) {
						if (ApplyTemplateFile
						 && ((templateId == 0)
						  || F_StrICmp(TemplateFileName, lastTemplate))) { /* need new one */
							if (templateId
							 && (templateopen == False))
								F_ApiClose(templateId, FF_CLOSE_MODIFIED);
							templateId = OpenDocName(TemplateFileName);
							templateopen = DocAlreadyOpen; /* set by OpenDocName */
							if (templateId) {
								F_ApiSetInt(FV_SessionId, templateId, FP_IsIconified, True);
								if (lastTemplate)
									F_StrFree(lastTemplate);
								lastTemplate = F_StrCopyString(TemplateFileName);
								applyThisTemplateFile = True;
							}
							else {
								M2GLogEvent(logwarn, 1, "Cannot open template ", TemplateFileName, NULL, NULL, NULL);
								F_Printf(NULL, "Cannot open template %s\n", TemplateFileName);
								if (F_ApiAlert(TemplateFailed, FF_ALERT_NO_DEFAULT) == -1) {
									Quit = True;
									break;
								}
							}
						}
						if (ApplyTemplateFile
						 && templateId) {
							ShowStatus(docId, filename, StatTemplate);
							ImportDocTemplate(docId, templateId);
							applyThisTemplateFile = True;
							if (FA_errno != FE_Success) {
								M2GLogEvent(logwarn, 1, "Failure importing template ", TemplateFileName, NULL, NULL, NULL);
								F_Printf(NULL, "Failure importing template %s\n", TemplateFileName);
								Quit = True;
								break;
							}
							ClearStatus(docId);
							F_Printf(NULL, "Imported template %s\n", TemplateFileName);
						}
						if (ConvertVars) {
							ShowStatus(docId, filename, StatConvVars);
							FA_errno = FE_Success;
							ConvertAllVars(docId);
							if (FA_errno != FE_Success) {
								M2GLogEvent(logwarn, 1, "Failure converting system variables", NULL, NULL, NULL, NULL);
								Quit = True;
								break;
							}
							ClearStatus(docId);
						}
						if (SetFrameConditions) {
							ConditionsSet = False;
							ShowStatus(docId, filename, StatSetConds);
							FA_errno = FE_Success;
							SetConditions(docId, bkininame);
							if (FA_errno != FE_Success) {
								M2GLogEvent(logwarn, 1, "Failure setting conditions", NULL, NULL, NULL, NULL);
								Quit = True;
								break;
							}
							ClearStatus(docId);
						}
					}
				}

				if (!gen1) {
					if (UseGrFileID) 
						GetFileIDSetting(bkininame, basename);

					/* generate the .rtf and .bct files */
					ConvertOneFile(docId, filename, mifname, dclname);
					F_StrFree(mifname);

					/* add .bct file to .cnt */
					if (cfile) {
						bctname = NewFileExt(filename, ".bct");
						CopyBctFile(bctname, cfile);
					}

					/* if formats imported, revert now */
					if (opened)
						F_ApiClose(docId, FF_CLOSE_MODIFIED);
					else if (applyThisTemplateFile
					 || ConvertVars
					 || ConditionsSet) {
						RevertDoc(&docId);
						ConditionsSet = False;
					}
				}
			}

			if (!gen1)
				ReportResults(filename);

			F_StrFree(filename);
			F_StrFree(basename);
			F_StrFree(dclname);

			if ((ExitType == 6)
			 && (ExitCode == 1)) {
				Quit = True;
				break;
			}

			if (Quit
			 || ((Quit = F_ApiUserCancel()) == True))
				break;
		}

		if (!Quit && gen1) {
			gen1 = False;
			gen2 = True;
			F_ApiSimpleGenerate(bookId, False, False);
			goto ProcBookFiles;
		}


		/* .cnt file completed */
		if (cfile) {
			fclose(cfile);
			cfile = NULL;
		}

		if (templateId) {
			if (templateopen == False)
				F_ApiClose(templateId, FF_CLOSE_MODIFIED);
			F_StrFree(lastTemplate);
			templateId = 0;
		}
	
		if (!Quit) {
			if (PT_htm(FormType)) {
				bookname = F_ApiGetString(FV_SessionId, bookId, FP_Name);
				bkbasename = BaseFileName(bookname);
				ShowStatus(bookId, bkbasename, (FormType == PT_DITA) ? StatDITA :
																			 ((FormType == PT_DCBK) ? StatDocBook : StatList));
				if (!ListFileName)
					BuildListFile(bookId, bkbasename);
				if (ListFileUsed)
					RunFileList();
				ClearStatus(bookId);
				F_StrFree(bkbasename);
				F_StrFree(bookname);
			}

			if (CheckLinks) {
				bookname = F_ApiGetString(FV_SessionId, bookId, FP_Name);
				bkbasename = BaseFileName(bookname);
				ShowStatus(bookId, bkbasename, StatCheck);
				ProcRefLinkErrs(bookId);
				F_StrFree(bkbasename);
				F_StrFree(bookname);
			}

			if (OldAuto) {
				if (BookWrapCommand)
					ProcBookCommand(bookId, BookWrapCommand, BF_WRAP_CMD, BF_BOOK,
													BookCommandHide, BookCommandKeep, bkininame, NULL);

				if (WrapAndShip
				 || (MakeHLP && PT_comp(FormType))
				 || ((FTSCommand || JarCommand) && PT_java(FormType)))
					WrapShipProc(bookId, HPJName);

				if (BookEndCommand)
					ProcBookCommand(bookId, BookEndCommand, BF_END_CMD, BF_BOOK,
													BookCommandHide, BookCommandKeep, bkininame, NULL);
			}
			else
				RunSystemCommands(True, False);

			F_ApiSetString(FV_SessionId, bookId, FP_StatusLine, M2GProjectDone);
			if (UseDone && !ProjRemote)
				F_ApiAlert(BookProcessDone, FF_ALERT_CONTINUE_NOTE);
		}

		if (Quit
		 && !((ExitType == 6) && (ExitCode == 1))) {
			ExitType = 6;
			ExitCode = 0;
			ReportResults(bookname);
		}
	}

	/* clean up and restore */
#ifdef NEWINI
	CloseIni();
#endif

	F_StrFree(bkininame);
	_chdrive(OrigDrive);
	_chdir(OrigDir);
	F_StrFree(OrigDir);
	F_StrFree(CurrDir);
	LastSpecIniSet = NULL;
	CallM2GFrame("MacroProcDone ");
	CallM2GFrame("IniProcDone ");
	M2GLogEvent(loginfo, 1, "Project finished", NULL, NULL, NULL, NULL);
	M2GCloseProject(True);
	SavingBook = False;
}


/* LIST file functions */


VoidT BuildListFile(F_ObjHandleT bookId, StringT defname)
{
	FILE *cfile = NULL;
	F_ObjHandleT compId = 0;
	IntT comptype = 0;
	StringT docname = NULL;
	StringT partname = NULL;
	StringT basename = NULL;
	StringT tempname = NULL;
	StringT ptempname = NULL;
	StringT extname = NULL;

	if (PT_help(FormType)
	 && (FormType != PT_OMH)
	 && (FormType != PT_EH)
	 && HPJName
	 && *HPJName)
		defname = HPJName;

	tempname = Fm2a(defname);
	defname = tempname;

	/* start the .lst file */
	if (F_StrSubString(defname, ".lst") == -1)
		ListFileName = NewFileExt(defname, ".lst");
	else {
		ListFileName = F_StrNew(F_StrLen(defname));
		F_StrCpy(ListFileName, defname);
	}
	SetCurrPath(&ListFileName);

	StartListFile(ListFileName, &cfile);

	/* while getting filenames from book */
	compId = F_ApiGetId(FV_SessionId, bookId, FP_FirstComponentInBook);
	while (compId) {	/* add filename to list */
		docname = F_ApiGetString(bookId, compId, FP_Name);
		ptempname = Fm2a(docname);
		comptype = F_ApiGetInt(bookId, compId, FP_BookComponentType);
		if ((comptype == FV_BK_NOT_GENERATABLE)
		 || ((comptype == FV_BK_TOC) && UseTOC)
		 || ((comptype == FV_BK_INDEX_STAN) && UseIX)
		 || ((comptype != FV_BK_TOC)
			&& (comptype != FV_BK_INDEX_STAN)
			&& UseGenFiles)) {
			switch (comptype) {
				case FV_BK_TOC:
					extname = ".toc";
					break;
				case FV_BK_LIST_FIGURE:
				case FV_BK_LIST_TABLE:
				case FV_BK_LIST_REFERENCES:
					extname = ".frm";
					break;
				case FV_BK_INDEX_STAN:
					extname = ".idx";
					break;
				case FV_BK_INDEX_AUTHOR:
				case FV_BK_INDEX_SUBJECT:
				case FV_BK_INDEX_MARKER:
				case FV_BK_INDEX_REFERENCES:
					extname = ".bkm";
					break;
				case FV_BK_NOT_GENERATABLE:
					extname = ".chp";
					break;
				default:
					extname = ".gen";
					break;
			}
			basename = BaseFileName(ptempname);
			partname = AddFileExt(basename, extname);
			if (UsingFile(basename))
					fprintf(cfile, "%s\n", partname);
			F_StrFree(basename);
			F_StrFree(partname);
		}
		F_StrFree(docname);
		F_StrFree(ptempname);
		compId = F_ApiGetId(bookId, compId, FP_NextComponentInBook);
	}

	/* .lst file completed */
	fclose(cfile);
	F_Free(tempname);
}


VoidT StartListFile(StringT defname, FILE **cfilep)
{
	if ((*cfilep = fopen(defname, "wt")) == NULL)
		return;

	/* put first file line, LIST and comment */
	fprintf(*cfilep, "LIST for %s\n", defname);

	fflush(*cfilep);
}


VoidT RunFileList(void)
{
	StringT command = NULL;
	StringT space = NULL;
	IntT baselen = 0;

	if (DclOnly
	 || PT_rtf(FormType)
	 || (FormType == PT_MIF)
	 || (FormType == PT_DCL))
		return;

	/* invoke dcl so that stderr goes to Frame Console */
	baselen = F_StrLen(ListFileName);
	if ((space = F_StrChr(ListFileName, ' ')) != NULL)
		baselen += 2;

	command = F_StrNew(baselen + 17);
	F_StrCpy(command, "dcl -t htm ");

	if (FormType == PT_MSHH)
		F_StrCat(command, "-fM");
	else if (FormType == PT_JAVH)
		F_StrCat(command, "-fJ");
	else if (FormType == PT_ORH)
		F_StrCat(command, "-fO");
	else if (FormType == PT_EH)
		F_StrCat(command, "-fE");
	else if (FormType == PT_OMH)
		F_StrCat(command, "-fC");
	else if (FormType == PT_DITA)
		F_StrCat(command, "-fD");
	else if (FormType == PT_DCBK)
		F_StrCat(command, "-fB");

	if (ListType == LST_TOC)
		F_StrCat(command, "C ");
	else if (ListType == LST_IX)
		F_StrCat(command, "I ");
	else if (ListType == LST_BOTH)
		F_StrCat(command, "B ");

	if (space)
		F_StrCat(command, "\"");
	F_StrCat(command, ListFileName);
	if (space)
		F_StrCat(command, "\"");

	RunCommand(command, CurrDir, True, True, False);
	F_StrFree(command);
}


/* cnt file functions */

VoidT BuildCntFile(F_ObjHandleT bookId, StringT defname)
{
	FILE *cfile = NULL;
	F_ObjHandleT compId = 0;
	StringT docname = NULL;
	StringT bctname = NULL;
	StringT basename = NULL;
	StringT newname = NULL;

	/* start the .cnt file */
	if (F_StrSubString(defname, ".cnt") == -1) {
		newname = NewFileExt(defname, ".cnt");
		StartCntFile(newname, &cfile);
		F_StrFree(newname);
	}
	else
		StartCntFile(defname, &cfile);

	if (!cfile)
		return;

	/* while getting filenames from book */
	compId = F_ApiGetId(FV_SessionId, bookId, FP_FirstComponentInBook);
	while (compId) {	/* add .bct file to .cnt */
		docname = F_ApiGetString(bookId, compId, FP_Name);
		basename = BaseFileName(docname);
		bctname = AddFileExt(basename, ".bct");
		if (UsingFile(basename))
			CopyBctFile(bctname, cfile);
		F_StrFree(docname);
		F_StrFree(basename);
		F_StrFree(bctname);
		compId = F_ApiGetId(bookId, compId, FP_NextComponentInBook);
	}

	/* .cnt file completed */
	fclose(cfile);
}


VoidT StartCntFile(StringT defname, FILE **cfilep)
{
	BoolT CNSet = False;
	BoolT CBSet = False;

	/* ensure .bcts were specified, from ini info */
	if (CntType != 2)
		return;

	if (!(CntName && *CntName)) {
		CNSet = True;
		CntName = NewFileExt(defname, ".cnt");
	}

	if ((*cfilep = fopen(CntName, "wt")) == NULL)
		return;

	/* put file header together, startfile or (base and title) */
	if (!CntStartFile
	 || !CopyBctFile(CntStartFile, *cfilep)) {
		if (!(CntBase && *CntBase)) {
			CBSet = True;
			CntBase = NewFileExt(defname, ".hlp");
		}
		fprintf(*cfilep, ":BASE %s\n", CntBase);

		if (CntTitle)
			fprintf(*cfilep, ":TITLE %s\n", CntTitle);
	}
	fflush(*cfilep);

	/* clean up */
	if (CNSet) {
		if (CntName)
			F_StrFree(CntName);
		CntName = NULL;
	}
	if (CBSet) {
		if (CntBase)
			F_StrFree(CntBase);
		CntBase = NULL;
	}
	if (CntTitle)
		F_StrFree(CntTitle);
	CntTitle = NULL;
	if (CntStartFile)
		F_StrFree(CntStartFile);
	CntStartFile = NULL;
}


BoolT CopyBctFile(StringT bctname, FILE *dfile)
{
	FILE *sfile = NULL;
	UCharT buffer[MAX_FILEIO];
	IntT len = 0;

	if ((sfile = fopen(bctname, "rt")) == NULL)
		return False;

	while (!feof(sfile)) {
		len = fread(buffer, 1L, MAX_FILEIO, sfile);
		if (ferror(sfile)) {
			break;
		}
		if (((IntT) fwrite(buffer, 1L, len, dfile)) != len)
			return False;
		if (ferror(dfile))
			return False;
	}
	fclose(sfile);
	fflush(dfile);
	return True;
}


/* ini file access */

StringT LastIniSet = NULL;
StringT LastSpecIniSet = NULL;

#ifndef NEWINI

VoidT GetIniSettings(StringT ininame, StringT filename)
{
	UCharT ch = 0;
	BoolT is = False;
	IntT i = 0;
	StringT str = NULL;
	StringT fn = NULL;

	if (!FileExists(ininame))
		return;
	if (LastIniSet)
		F_Free(LastIniSet);
	LastIniSet = Fm2a(ininame);
	if (filename)
		fn = Fm2a(filename);

	M2GSetIni(LastIniSet, fn);
	M2GIniSects();  // reads all FDK settings and one Graphics

	if (TemplateFileName)
		TemplateFileName = Unquote(TemplateFileName);

	M2GIniGroup("HtmHelp"); // *HelpOptions

	if (PT_wh(FormType))
		M2GIniGroup("WinHelp"); // HelpContents

	// make inis writeable
	_chmod(LastIniSet, _S_IWRITE);

	_chmod(CurrFileIDPath, _S_IWRITE);

	// overrides
	if (FormType == PT_OMH) {
		MakeCNT = True;
		ListType = LST_BOTH;
	}

	if ((FormType == PT_DITA)
	 || (FormType == PT_DCBK)) {
		ListFileUsed = True;
		ListType = LST_BOTH;
	}

	if ((RefFileType == RF_BODY)  /* body */
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

	if ((FormType == PT_WH4)
	 && (CntType == 1))
		MakeCNT = False;

	if (WriteAllGraphics)  /* mutually exclusive */
		WriteEqns = False;

	if (fn)
		F_Free(fn);
}

#endif

#if 0
// original form for reference
VoidT GetIniSettings(StringT ininame, BoolT full)
{
	// full was True when ininame was mif2*.ini, False for filename.ini
	// this func was called twice, one for each ini, so the local one overwrote
	UCharT ch = 0;
	BoolT is = False;
	IntT i = 0;
	StringT str = NULL;

	if (!FileExists(ininame))
		return;

	_chmod(ininame, _S_IWRITE);

	TrueFalseIni(ininame, "FDK", "GraphicsFirst", &GraphicsFirst);
	TrueFalseIni(ininame, "FDK", "ConvertVariables", &ConvertVars);
	TrueFalseIni(ininame, "FDK", "GenerateBook", &GenUpBook);
	TrueFalseIni(ininame, "FDK", "ConversionDesigner", &CondDes);

	if (FormType == PT_MIF) {
		TrueFalseIni(ininame, "FDK", "MakeBookMIF", &MakeBook);
		TrueFalseIni(ininame, "FDK", "OrigExtForMIF", &OrigExtForMIF);
	}

	GetIniString(ininame, "FDK", "IDFileName", &CurrFileIDPath);
	_chmod(CurrFileIDPath, _S_IWRITE);

	if (PT_htm(FormType)) {
		TrueFalseIni(ininame, "FDK", "UseInitDialog", &UseInit);
		if ((FormType == PT_MSHH)    /* MS HTML Help */
		 || (FormType == PT_JAVH)    /* JavaHelp */
		 || (FormType == PT_ORH)     /* OracleHelp */
		 || (FormType == PT_OMH)) {  /* OmniHelp */
			switch (FormType) {
				case PT_MSHH:
					str = "MSHtmlHelpOptions";
					break;
				case PT_JAVH:
				case PT_ORH:
					str = "JavaHelpOptions";
					break;
				case PT_OMH:
					str = "OmniHelpOptions";
					break;
				default:
					str = "HelpOptions";
					break;
			}
			if (FormType == PT_OMH) {
				MakeCNT = True;
				ListType = LST_BOTH;
			}
			else {
				GetIniMatch(ininame, str, "ListType", ListTypeList, &ListType);
				TrueFalseIni(ininame, str, "WriteHelpSetFile", &WriteHelpSetFile);
				TrueFalseIni(ininame, str, "WriteHelpProjectFile", &WriteHelpProjectFile);
				TrueFalseIni(ininame, str, "HSPathNames", &HSPathNames);
			}
			GetIniMatch(ininame, str, "RefFileType", RefFileTypeList, &RefFileType);
			if ((RefFileType == RF_BODY)  /* body */
			 && (ListType > LST_NONE))
				ListFileUsed = True;
			GetIniString(ininame, str, (FormType == PT_ORH) ? "DefaultTopic" :
			                                                  "DefaultTopicFile", &DefaultTopicFile);
			GetIniString(ininame, str, "HelpFileTitle", &HelpTitle);
			TrueFalseIni(ininame, str, "UseFTS", &UseFTS);
		}
	}
	else if (full
	 && PT_rtf(FormType)) {
		// GetIniMatch(ininame, "Options", "Output", FormTypeList, &FormType);
		if (PT_wh(FormType)) { /* WinHelp */
			GetIniUC(ininame, "HelpOptions", "HelpVersion", &ch);
			if (ch == '3')
				FormType = PT_WH3;
			else
				FormType = PT_WH4;
		}
		else if (PT_word(FormType)) {
			TrueFalseIni(ininame, "WordOptions", "Word8", &is);
			if (is)
				FormType = PT_WORD8;
			else {
				TrueFalseIni(ininame, "WordOptions", "WordPerfect", &is);
				FormType = is ? PT_WORDP : PT_WORD7;
			}
		}
	
		TrueFalseIni(ininame, "FDK", "UseInitDialog", &UseInit);

		if (FormType == PT_WH4) {
			GetIniMatch(ininame, "HelpContents", "CntType", CntTypeList, &CntType);

			if (CntType == CNT_BODY) {
				GetIniPath(ininame, "HelpContents", "CntName", &CntName);
				GetIniPath(ininame, "HelpContents", "CntStartFile", &CntStartFile);
				GetIniString(ininame, "HelpContents", "CntBase", &CntBase);
				GetIniString(ininame, "HelpContents", "CntTitle", &CntTitle);
			}
		}
	}

	GetIniString(ininame, "FDK", "WordSuffix", &WordSuffix);
	if (WordSuffix == NULL)
		WordSuffix = RtfExt;
	GetIniString(ininame, "FDK", "WordPerfectSuffix", &WordPerfectSuffix);
	if (WordPerfectSuffix == NULL)
		WordPerfectSuffix = RtfExt;
	GetIniString(ininame, "FDK", "HTMLSuffix", &HTMLSuffix);
	if (HTMLSuffix == NULL)
		HTMLSuffix = HtmExt;
	GetIniString(ininame, "FDK", "XMLSuffix", &XMLSuffix);
	if (XMLSuffix == NULL)
		XMLSuffix = XmlExt;
	GetIniString(ininame, "FDK", "MIFSuffix", &MIFSuffix);
	if (MIFSuffix == NULL)
		MIFSuffix = MifExt;
	GetIniString(ininame, "FDK", "MIFBookSuffix", &MIFBookSuffix);
	if (MIFBookSuffix == NULL)
		MIFBookSuffix = MIFSuffix;
	GetIniString(ininame, "FDK", "DCLSuffix", &DCLSuffix);
	if (DCLSuffix == NULL)
		DCLSuffix = DclExt;

	TrueFalseIni(ininame, "FDK", "UseExistingMIF", &UseMIF);
	TrueFalseIni(ininame, "FDK", "DeleteMIF", &DeleteMIF);
	TrueFalseIni(ininame, "FDK", "UseExistingDCL", &UseDCL);

	if (FormType == PT_WH4) {
		TrueFalseIni(ininame, "FDK", "MakeCombinedCnt", &MakeCNT);
		if (CntType == 1)
			MakeCNT = False;
	}

	if (PT_help(FormType)) {
		TrueFalseIni(ininame, "FDK", "CompileHelp", &MakeHLP);
		TrueFalseIni(ininame, "FDK", "KeepCompileWindow", &KeepCompileWindow);
	}

	if (PT_wh(FormType))
		GetIniString(ininame, "FDK", "HPJFileName", &HPJName);
	else if (PT_java(FormType)) {
		GetIniString(ininame, "FDK", "HSFileName", &HPJName);
		if (FormType == PT_JAVH)
			GetIniString(ininame, "FDK", "JHIndexerPath", &JHIndexerPath);
	}
	else if (FormType == PT_MSHH)
		GetIniString(ininame, "FDK", "HHPFileName", &HPJName);
	else if (FormType == PT_OMH)
		GetIniString(ininame, "FDK", "OHProjPath", &HPJName);

	TrueFalseIni(ininame, "FDK", "UseFrameTOC", &UseTOC);
	TrueFalseIni(ininame, "FDK", "UseFrameIX", &UseIX);
	TrueFalseIni(ininame, "FDK", "UseFrameGenFiles", &UseGenFiles);

	TrueFalseIni(ininame, "FDK", "WriteEquations", &WriteEqns);
	TrueFalseIni(ininame, "FDK", "WriteAllGraphics", &WriteAllGraphics);
	TrueFalseIni(ininame, "FDK", "WriteRefPageGraphics", &WriteRefGraphics);
	TrueFalseIni(ininame, "FDK", "WriteMasterPageGraphics", &WriteMpgGraphics);
	if (PT_htm(FormType))
		TrueFalseIni(ininame, "Graphics", "UseOriginalGraphicNames", &UseOrigGrName);

	if (WriteAllGraphics)  /* mutually exclusive */
		WriteEqns = False;
	GetIniMinMax(ininame, "FDK", "GraphicNameDigits", 4, 8, &GrFileNumLen);
	GrFileNumMask = 0;
	for (i = 0; i < GrFileNumLen; i++) {
		GrFileNumMask <<= 4;
		GrFileNumMask += 0x0F;
	}

	GetIniMinMax(ininame, "FDK", "EquationExportDPI", 50, 1200, &EqDpiValue);
	
	GetIniMinMax(ininame, "FDK", "GraphicExportDPI", 50, 1200, &GrDpiValue);

	i = 0;
	GetIniMatch(ininame, "FDK", "GraphicExportFormat", GrFormList, &i);
	if (i)
		GrFormat = GrFormVal[i];

	/* use GrPath for all graphics */
	TrueFalseIni(ininame, "FDK", "UseGraphicFileID", &UseGrFileID);
	if ((GrPath == CurrDir)
	 && BaseName
	 && UseGrFileID)
		GetFileIDSetting(ininame, BaseName);

	/* see if template doc is specified */
	TrueFalseIni(ininame, "FDK", "ApplyTemplateFile", &ApplyTemplateFile);

	if (ApplyTemplateFile
	 && !IniBufEmpty()) {
		GetIniString(ininame, "FDK", "TemplateFileName", &TemplateFileName);
		SetCurrPath(&TemplateFileName);
		AppliedTemplateFlags = 0;
		GetIniInt(ininame, "FDK", "AppliedTemplateFlags", &AppliedTemplateFlags);
		AppliedTemplateFlagsSet = IniBufEmpty() ? False : True;
		TrueFalseIni(ininame, "FDK", "ImportDocProps", &ImportDocProps);
		ImportDocPropsSet = IniBufEmpty() ? False : True;
	}
}
#endif


BoolT SavingBook = False;

BoolT UsingFile(StringT basename)
{
	BoolT ret = True;
	StringT bname = NULL;

	if (!SavingBook
	 || !CurrIniName)
		return True;

	bname = Fm2a(basename);
	TrueFalseIni(CurrIniName, "BookFileUsage", bname, &ret);
	F_StrFree(bname);

	return ret;
}


StringT CurrFileIDPath = NULL;
StringT NextFileID = NULL;


VoidT GetFileIDSetting(StringT ininame, StringT basename)
{
	BoolT localID = False;
	StringT in;
	StringT bn;

	in = Fm2a(ininame);
	bn = Fm2a(basename);

	GetIniString(in, "FDK", "IDFileName", &CurrFileIDPath);
	if (IniBufEmpty() && (CurrFileIDPath == NULL)) {
		localID = True;
		CurrFileIDPath = F_StrCopyString(in);
	}
	else
		_chmod(in, _S_IWRITE);

	GetIniPath(CurrFileIDPath, "FileIDs", bn, &GrPath);
	if (IniBufEmpty() && !localID)   /* check in local .ini */
		GetIniPath(in, "FileIDs", bn, &GrPath);
	if (IniBufEmpty()) {  /* need to create entry for file */
		SetFileID(bn);
		GetIniPath(CurrFileIDPath, "FileIDs", bn, &GrPath);
	}
	if (!IniBufEmpty())
		M2GLogEvent(loginfo, 1, "File ID is ", GetIniBuf(), NULL, NULL, NULL);

	if (localID) {
		//F_StrFree(CurrFileIDPath);
		CurrFileIDPath = NULL;
	}
	F_StrFree(NextFileID);
	NextFileID = NULL;

	F_Free(in);
	F_Free(bn);
}


VoidT SetFileID(StringT name)
{
	StringT nid = NULL;

	GetIniString(CurrFileIDPath, "FDK", "NextFileID", &NextFileID);
	if (IniBufEmpty())
		NextFileID = F_StrCopyString("aa");
	while (FileIDUsed(NextFileID)) {
		if (!IncAlpha(NextFileID)) {
			nid = F_StrNew(F_StrLen(NextFileID) + 2);
			F_StrCpy(nid, NextFileID);
			F_StrFree(NextFileID);
			NextFileID = nid;
			F_StrCat(NextFileID, "a");
		}
	}
	if (name) {
		SetIniStr(CurrFileIDPath, "FileIDs", name, NextFileID);
		if (!IncAlpha(NextFileID))
			F_StrCat(NextFileID, "a");
	}
	SetIniStr(CurrFileIDPath, "FDK", "NextFileID", NextFileID);
}



BoolT FileIDUsed(StringT id)
{
	StringT ckid = NULL;
	BoolT rval = False;

	GetIniSect(CurrFileIDPath, "FileIDs");
	while ((ckid = GetIniSectKey()) != NULL) {
		if (!F_StrCmp(id, ckid)) {
			rval = True;
			break;
		}
	}
	return rval;
}


BoolT IncAlpha(StringT str)
{
	BoolT done = False;
	StringT cpt = NULL;

	if (!str || (*str == '\0'))
		return False;

	for (cpt = str + F_StrLen(str) - 1; (!done) && (cpt >= str); cpt--) {
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
				done = True;
				break;
			case 'N':  // and upper-case O
				*cpt = 'P';
				done = True;
				break;

			default:
				if (isalnum(*cpt)) {
					(*cpt)++;
					done = True;
				}
				break;			
		}
	}

	return done;  // false for overflow
}


VoidT CreateIDFile(VoidT)
{
	/* create mif2go.ini in dir with project file */

}


#ifndef NEWINI

VoidT UpdateIniSettings(StringT ininame)
{
	StringT math = NULL;
	StringT in = NULL;

	if (!FileExists(ininame))
		return;

	in = Fm2a(ininame);
	SetIniStr(in, "FDK", "PluginVersion", PluginVersion);

	if (WriteAllGraphics)  /* ensure graphics will be used */
		SetIniTF(in, "Graphics", "UseGraphicPreviews", True);
	else if (WriteEqns)    /* or that eqns will be seen */
		SetIniTF(in, "Graphics", "UseGraphicPreviews", False);

	GetIniString(in, "Options", "MathType", &math);
	if (math) {  /* ensure correct setting if present */
		F_StrFree(math);
		SetIniStr(in, "Options", "MathType", "3");
	}

	SetIniTF(in, "FDK", "WriteEquations", WriteEqns);
	SetIniTF(in, "FDK", "WriteAllGraphics", WriteAllGraphics);

	if (PT_htm(FormType))
		SetIniTF(in, "Graphics", "UseOriginalGraphicNames", UseOrigGrName);

	if (FormType == PT_WORDP)
		SetIniStr(in, "FDK", "WordPerfectSuffix", WordPerfectSuffix);
	else if (PT_word(FormType))
		SetIniStr(in, "FDK", "WordSuffix", WordSuffix);
 	else if (PT_xml(FormType))
		SetIniStr(in, "FDK", "XMLSuffix", XMLSuffix);
 	else if (FormType == PT_MIF) {
		SetIniStr(in, "FDK", "MIFSuffix", MIFSuffix);
		SetIniStr(in, "FDK", "MIFBookSuffix", MIFBookSuffix);
	}
 	else if (FormType == PT_DCL)
		SetIniStr(in, "FDK", "DCLSuffix", DCLSuffix);
	else if (PT_htm(FormType)) {
		SetIniStr(in, "FDK", "HTMLSuffix", HTMLSuffix);
		if (CurrPrjFilePath)
			SetIniStr(in, "FDK", "PrjFileName", CurrPrjFilePath);
	}
	if (CurrFileIDPath)
		SetIniStr(in, "FDK", "IDFileName", CurrFileIDPath);

	SetIniTF(in, "FDK", "UseExistingMIF", UseMIF);
	SetIniTF(in, "FDK", "DeleteMIF", DeleteMIF);
	SetIniTF(in, "FDK", "UseExistingDCL", UseDCL);

	if (PT_wh(FormType))
		SetIniTF(in, "HelpOptions", "MakeCombinedCnt", MakeCNT);

	SetIniTF(in, "Automation", "WrapAndShip", WrapAndShip);

	if (PT_comp(FormType))
		SetIniTF(in, "Automation", "CompileHelp", MakeHLP);
	F_Free(in);
}
#endif


/* conversion operations */


VoidT ConvertOneFile(F_ObjHandleT docId, StringT filename,
 StringT mifname, StringT dclname)
{
	StringT command = NULL;
	FilePathT *fmifpath = NULL;
	StringT aname = NULL;
	StringT fname = NULL;
	StringT dname = NULL;
	StringT	dcbname = NULL;
	StringT	docname = NULL;
	StringT	tmpfilename = NULL;
	FilePathT *fdcbpath = NULL;
	IntT grnum = 0;

	time(&StartTime);

	ExitType = 0;
	ExitCode = 0;

	if (GraphicsFirst) {
		if (WriteEqns) {
			ShowStatus(docId, filename, StatEqWriting);
			ExportGraphics(docId, GrPath, GrFormat, EqDpiValue, GT_EQN);
			ClearStatus(docId);
		}
		else if (WriteAllGraphics) {
			ShowStatus(docId, filename, StatGrWriting);
			ExportGraphics(docId, GrPath, GrFormat, GrDpiValue, GT_ALL);
			ClearStatus(docId);
		}

		if (WriteGraphicsOnly)
			return;

		if ((Quit = F_ApiUserCancel()) == True)
			return;
	}

	if (!WriteGraphicsOnly) {
		if (FormType == PT_MIF) {
			if (!(UseMIF && MIFExisted)) {
				docname = F_ApiGetString(FV_SessionId, docId, FP_Name);
				tmpfilename = NewFileExt(docname, "tmb");
				if (!SaveMIFforDCL(docId, tmpfilename)) /* could not save */
					F_Printf(NULL, ReportMIFFail, (StringT) mifname);
				else { // move into place
					if (FileExists(mifname))
						DeleteFile(mifname);
					MoveFile(tmpfilename, mifname);
				}
				F_StrFree(docname);
				F_StrFree(tmpfilename);
			}
			return;
		}

		aname = Fm2a(mifname);
		if (DeleteExistingMIF
		 && !UseMIF
		 && MIFExisted) {
			command = F_StrNew(F_StrLen(aname) + 10);
			sprintf(command, "del /q \"%s\"", aname);
			RunDOSCommand(command, True, False);
			F_StrFree(command);
			MIFExisted = False;
		}

		dcbname = NewFileExt(aname, "dcb");
		fname = Fm2a(filename);
		dname = Fm2a(dclname);

		if (!(UseMIF && MIFExisted)
		 && !(UseDCL && DCLExisted)
		 && !SaveMIFforDCL(docId, aname)) {  	/* could not save MIF */
			F_Printf(NULL, ReportMIFFail, (StringT) aname);
		}
		else {	/* invoke dcl so that stderr goes to Frame Console */

			if (!GraphicsFirst && !DclOnly) {
				ClearIniSect(CurrGrxFile, GrGenSect);
				ClearIniSect(CurrGrxFile, GrDpiSect);
			}

			command = SetCommand(fname, aname, dcbname, dname, PP1);
			if (!command)
				return;
			ShowStatus(docId, filename, StatConverting);
			M2GCloseLog(False);
			RunCommand(command, CurrDir, True, True, False);
			F_StrFree(command);

			if (PT_rtf(FormType)) {
				command = SetCommand(fname, aname, dcbname, dname, PP2);
				RunCommand(command, CurrDir, True, True, False);
				F_StrFree(command);
			}
			M2GOpenLog(filename);
			ClearStatus(docId);

			if ((Quit = F_ApiUserCancel()) == True)
				return;
		}
	}

	if (!GraphicsFirst
	 && !DclOnly
	 && (FormType != PT_DCL)) {
		if (WriteEqns) {
			ShowStatus(docId, filename, StatEqWriting);
			grnum = ExportGraphics(docId, GrPath, GrFormat, EqDpiValue, GT_EQN);
			ClearStatus(docId);
		}
		else if (WriteAllGraphics) {
			ShowStatus(docId, filename, StatGrWriting);
			grnum = ExportGraphics(docId, GrPath, GrFormat, GrDpiValue, GT_ALL);
			ClearStatus(docId);
		}

		if (WriteGraphicsOnly)
			return;

		if ((Quit = F_ApiUserCancel()) == True)
			return;

		if (PT_rtf(FormType)) {  /* play it again */

			if (grnum) { /* but only if graphics actually written */
				command = SetCommand(fname, aname, dcbname, dname, PP3);

				ShowStatus(docId, filename, StatConverting);
				M2GCloseLog(False);
				RunCommand(command, CurrDir, True, True, False);
				ClearStatus(docId);
				M2GOpenLog(filename);
				F_StrFree(command);
			}

			//fdcbpath = F_PathNameToFilePath(dcbname, NULL, FDefaultPath);
			//F_DeleteFile(fdcbpath);
			//F_FilePathFree(fdcbpath);


			if ((Quit = F_ApiUserCancel()) == True)
				return;
		}

#if 0
		if (DeleteMIF
		 && (FormType != PT_MIF)) {
			fmifpath = F_PathNameToFilePath(mifname, NULL, FDefaultPath);
			F_DeleteFile(fmifpath);
			F_FilePathFree(fmifpath);
		}
#endif
	}

	F_StrFree(dcbname);
	F_StrFree(dname);
	F_StrFree(fname);
	F_StrFree(aname);
}


StringT SetCommand(StringT filename, StringT mifname, StringT dcbname,
                   StringT dclname, IntT pass)
{
	StringT command = NULL;
	IntT baselen = 0;
	StringT extension = NULL;
	StringT space = NULL;

	baselen = F_StrLen(filename);
	if ((extension = F_StrRChr(filename, '.')) != NULL)
		baselen -= F_StrLen(extension);
	if ((space = F_StrChr(filename, ' ')) != NULL)
		baselen += 2;

	if (DclOnly
	 || (FormType == PT_DCL)) {  // ASCII DCL
		if (UseDCL && DCLExisted)
			return NULL;  // nothing to do
		if (F_StrICmp(filename + baselen, DclExt)) {  // different ext
			if (space)
				baselen += 2;
			command = F_StrNew(baselen + F_StrLen(filename) + 20 + FormLen);
			F_StrCpy(command, "dcl -o ");
			if (space)
				F_StrCat(command, "\"");
			F_StrCat(command, filename);
			if (space)
				F_StrCat(command, "\"");
			F_StrCat(command, " -t dcl ");
		}
		else {
			command = F_StrNew(baselen + 16 + FormLen);
			F_StrCpy(command, "dcl -t dcl ");
		}
	}
	else if (PT_htm(FormType)) {  // HTML 
		if (F_StrICmp(filename + baselen, HtmExt)) {  // different ext
			if (space)
				baselen += 2;
			command = F_StrNew(baselen + F_StrLen(filename) + 20 + FormLen);
			F_StrCpy(command, "dcl -o ");
			if (space)
				F_StrCat(command, "\"");
			F_StrCat(command, filename);
			if (space)
				F_StrCat(command, "\"");
			F_StrCat(command, " -t htm ");
		}
		else {
			command = F_StrNew(baselen + 16 + FormLen);
			F_StrCpy(command, "dcl -t htm ");
		}
	}
	else if (PT_rtf(FormType)) {   // RTF 
		if (F_StrICmp(filename + baselen, RtfExt)  // different ext
		 || (pass == PP1)) {  // RTF default for first pass
			if (space)
				baselen += 2;
			command = F_StrNew(baselen + F_StrLen(filename) + 20 + FormLen);
			F_StrCpy(command, "dcl -o ");
			if (space)
				F_StrCat(command, "\"");
			F_StrCat(command, (pass == PP1) ? dcbname : filename);
			if (space)
				F_StrCat(command, "\"");
			F_StrCat(command, (pass == PP1) ? " -t dcb " : " -t rtf ");
		}
		else {  // RTF default for second pass
			command = F_StrNew(baselen + 9 + FormLen);
			F_StrCpy(command, "dcl ");
		}
	}

	if (FormOver)
		F_StrCat(command, FormStr);
	if (space)
		F_StrCat(command, "\"");
	F_StrCat(command, (UseDCL && DCLExisted) ? dclname :
	                  (pass == PP1) ? mifname : dcbname);
	if (space)
		F_StrCat(command, "\"");

	return command;
}


BoolT SaveMIFforDCL(F_ObjHandleT docId, StringT filename)
{
	IntT i;
	F_PropValsT saveParams;
	F_PropValsT *saveRetParamsp;
	StringT aname = NULL;

	FA_errno = FE_Success;

	saveParams = F_ApiGetSaveDefaultParams();
	if (saveParams.len == 0) {
		ExitCode = FA_errno;
		ExitType = 5;
		return False;  /* not saved */
	}

	i = F_ApiGetPropIndex(&saveParams, FS_FileType);

	if (FrameVersion >= 12) {
		if (UseFrame12MIF)
			saveParams.val[i].propVal.u.ival = FV_SaveFmtInterchange120;
		else
			saveParams.val[i].propVal.u.ival = FV_SaveFmtInterchange70;
	}
	else if (FrameVersion == 11) {
		if (UseFrame11MIF)
			saveParams.val[i].propVal.u.ival = FV_SaveFmtInterchange110;
		else
			saveParams.val[i].propVal.u.ival = FV_SaveFmtInterchange70;
	}
	else if (FrameVersion == 10) {
		if (UseFrame10MIF)
			saveParams.val[i].propVal.u.ival = FV_SaveFmtInterchange100;
		else
			saveParams.val[i].propVal.u.ival = FV_SaveFmtInterchange70;
	}
	else if (FrameVersion == 9) {
		if (UseFrame9MIF)
			saveParams.val[i].propVal.u.ival = FV_SaveFmtInterchange90;
		else
			saveParams.val[i].propVal.u.ival = FV_SaveFmtInterchange70;
	}
	else if ((FrameVersion == 8)
	 && UseFrame8MIF)
		saveParams.val[i].propVal.u.ival = FV_SaveFmtInterchange80;
	else
		saveParams.val[i].propVal.u.ival = FV_SaveFmtInterchange;

	saveRetParamsp = NULL;

	aname = A2fm(filename);

	ShowStatus(docId, aname, StatSavingMIF);
	MIFdocId = F_ApiSave(docId, aname, &saveParams, &saveRetParamsp);
	ClearStatus(docId);

	F_ApiDeallocatePropVals(&saveParams);
	F_ApiDeallocatePropVals(saveRetParamsp);

	F_StrFree(aname);

	if (FA_errno != FE_Success) {
		ExitCode = FA_errno;
		ExitType = 5;
		return False;  /* not saved */
	}

	if ((Quit = F_ApiUserCancel()) == True)
		return False;

	return True;
}


VoidT ConvertAllVars(F_ObjHandleT docId)
{
	F_ObjHandleT varId = 0;
	F_ObjHandleT varNextId = 0;
	F_ObjHandleT varFmtId = 0;
	IntT vfType = 0;

	varId = F_ApiGetId(FV_SessionId, docId, FP_FirstVarInDoc);
	while (varId) {
		varNextId = F_ApiGetId(docId, varId, FP_NextVarInDoc);
		varFmtId = F_ApiGetId(docId, varId, FP_VarFmt);
		vfType = F_ApiGetInt(docId, varFmtId, FP_SystemVar);
		switch (vfType) {
			case FV_VAR_CURRENT_DATE_LONG:
			case FV_VAR_CURRENT_DATE_SHORT:
			case FV_VAR_MODIFICATION_DATE_LONG:
			case FV_VAR_MODIFICATION_DATE_SHORT:
			case FV_VAR_CREATION_DATE_LONG:
			case FV_VAR_CREATION_DATE_SHORT:
			case FV_VAR_FILE_NAME_LONG:
			case FV_VAR_FILE_NAME_SHORT:
				ConvertToText(docId, varId);
				break;
			default:
				break;
		}

		FA_errno = FE_Success;
		varId = varNextId;
	}
}


VoidT SetConditions(F_ObjHandleT docId, StringT ininame)
{
	F_ObjHandleT condId = 0;
	F_ObjHandleT condNextId = 0;
	StringT condName = NULL;
	BoolT val = False;
	StringT in = NULL;
	StringT cn = NULL;

	in = Fm2a(ininame);
	condId = F_ApiGetId(FV_SessionId, docId, FP_FirstCondFmtInDoc);
	while (condId) {
		condNextId = F_ApiGetId(docId, condId, FP_NextCondFmtInDoc);
		condName = F_ApiGetString(docId, condId, FP_Name);
		cn = Fm2a(condName);
		F_Free(condName);
		TrueFalseIni(in, "ConditionsShown", cn, &val);
		if (!IniBufEmpty()
		 && (F_ApiGetInt(docId, condId, FP_CondFmtIsShown) != val)) {
			F_ApiSetInt(docId, condId, FP_CondFmtIsShown, val);
			if (val == False)
				F_ApiSetInt(FV_SessionId, docId, FP_ShowAll, False);
			ConditionsSet = True;
		}
		FA_errno = FE_Success;
		F_Free(cn);
		condId = condNextId;
	}
	F_Free(in);
}


VoidT ConvertToText(F_ObjHandleT docId, F_ObjHandleT itemId)
{
	F_TextRangeT tr;
	F_TextItemsT tis;
	F_TextItemT telem;
	F_TextLocT trm;
	F_TextLocT trmstart;
	F_TextLocT trmend;
	F_TextLocT trmref;
	F_ObjHandleT txobj;
	UIntT obtype;
	F_ObjHandleT tframe;
	F_PropValsT tprops;
	F_TextRangeT trx;
	IntT tadded = 0;
	UIntT ti = 0;

	tr = F_ApiGetTextRange(docId, itemId, FP_TextRange);

	trmref = tr.beg;
	txobj = trmref.objId;
	if (!txobj)
		return;
	obtype = F_ApiGetObjectType(docId, txobj);
	if (obtype == FO_Pgf) {
		tframe = F_ApiGetId(docId, txobj, FP_InTextFrame);
		if (!tframe
		 || OnMasterRefPage(docId, tframe))
			return;
	}
	else if (obtype == FO_TextLine) {
		if (OnMasterRefPage(docId, txobj))
			return;
	}
	else   // should not be possible, punt
		return;

	trmend = tr.end;
	trmstart = trmend;
	trmstart.offset++;  // position after end of item to write text
	tadded = 0;

	tis = F_ApiGetText(docId, itemId, FTI_String | FTI_CharPropsChange | FTI_LineEnd);

	for (ti = 0; ti < tis.len; ti++) {
		telem = tis.val[ti];
		switch (telem.dataType) {
			case FTI_String:
				trm = trmend;
				trm.offset++;
				trmend = F_ApiAddText(docId, &trm, telem.u.sdata);
				tadded += (trmend.offset - trm.offset) + 1;
				break;
			case FTI_LineEnd:
				trmend.offset--;
				break;
			case FTI_CharPropsChange:
				if (trmstart.offset <= trmend.offset) {
					tprops = F_ApiGetTextProps(docId, &trmref);  // in xref
					// set props for trmstart to trmend, set up for next
					trx.beg = trmstart;
					trx.end = trmend;
					F_ApiSetTextProps(docId, &trx, &tprops);
					F_ApiDeallocatePropVals(&tprops);
					trmstart = trmend;
					trmstart.offset++;
					trmref.offset += tadded;
					tadded = 0;
				}
				break;
			default:
				break;
		}
	}

	if (trmstart.offset <= trmend.offset) {
		tprops = F_ApiGetTextProps(docId, &trmref);  // in xref
		// set props for final text part
		trx.beg = trmstart;
		trx.end = trmend;
		F_ApiSetTextProps(docId, &trx, &tprops);
		F_ApiDeallocatePropVals(&tprops);
	}

	F_ApiDeallocateTextItems(&tis);
	F_ApiDelete(docId, itemId);
}


BoolT OnMasterRefPage(F_ObjHandleT docId, F_ObjHandleT frame)
{
	F_ObjHandleT pframe = 0;
	F_ObjHandleT first;
	F_ObjHandleT last;

	/* get to top-level frame on its page */
	pframe = F_ApiGetId(docId, frame, FP_FrameParent);
	while (pframe) {
		frame = pframe;
		pframe = F_ApiGetId(docId, frame, FP_FrameParent);
		if (pframe == frame)
			break;
	}
	if (!frame)
		return False;

	/* check all reference pages */
	first = F_ApiGetId(FV_SessionId, docId, FP_FirstRefPageInDoc);
	last = F_ApiGetId(FV_SessionId, docId, FP_LastRefPageInDoc);
	while (first) {
		if (frame == F_ApiGetId(docId, first, FP_PageFrame))
			return True;
		if (first == last)
			break;
		first = F_ApiGetId(docId, first, FP_PageNext);
	}

	/* check all master pages */
	first = F_ApiGetId(FV_SessionId, docId, FP_FirstMasterPageInDoc);
	last = F_ApiGetId(FV_SessionId, docId, FP_LastMasterPageInDoc);
	while (first) {
		if (frame == F_ApiGetId(docId, first, FP_PageFrame))
			return True;
		if (first == last)
			break;
		first = F_ApiGetId(docId, first, FP_PageNext);
	}

	return False;
}


// move all required files into the compile dir, then compile
StringT FTSCommand = NULL;
StringT JarCommand = NULL;
StringT ArchiveCommand = NULL;
StringT ArchiveStartParams = NULL;
StringT ArchiveEndParams = NULL;

StringT ArchiveName = NULL;
StringT ArchiveVer = NULL;
StringT ArchiveExt = NULL;

StringT WrapPath = ".\\wrap";
StringT ShipPath = ".\\..\\ship";
StringT CSSPath = NULL;
StringT WrapCopyFiles = NULL;
StringT GraphCopyFiles = NULL;
StringT CSSCopyFiles = NULL;
StringT JavaRootFiles = NULL;
StringT ZipCommand = NULL;
StringT ZipParams = NULL;
StringT OHVFiles = NULL;
BoolT OHProjFileXhtml = False;
StringT EmptyOutputFiles = NULL;

IntT EmptyOutputDir = 0;
StringT EmptyOutputDirList[] = {
 "Never", "File", "Book",
 NULL };

BoolT WrapAndShip = False;
BoolT DeleteExistingMIF = False;
BoolT DeleteExistingDCL = False;
BoolT EmptyWrapPath = True;
BoolT EmptyGraphPath = False;
BoolT EmptyJavaHTMLSubdir = True;
BoolT EmptyJavaGraphSubdir = False;
BoolT LogAuto = False;
BoolT OnlyAuto = False;
BoolT MoveArchive = False;
BoolT EclipseUseManifest = True;


VoidT WrapShipPreProc(BoolT isBook, F_ObjHandleT docId, StringT filename)
{
	StringT odfiles = NULL;

	if (LogAuto)
		F_Printf(NULL, "cd \"%s\"\n", CurrDir);

	ShowStatus(docId, filename, StatClean);

	// DeleteExistingMIF process
	if (DeleteExistingMIF
	 && !UseMIF)
		DelCompFiles("*.mif\0\0");

	// DeleteExistingDCL process
	if (DeleteExistingDCL
	 && !UseDCL)
		DelCompFiles("*.dcb\0*.dcl\0\0");

	// EmptyOutputDir process
	if (EmptyOutputDir == (isBook ? EOD_BOOK : EOD_FILE)) {
		if (EmptyOutputFiles && *EmptyOutputFiles) {
			_strlwr(EmptyOutputFiles);
			if (strstr(EmptyOutputFiles, "dcl")
			 || strstr(EmptyOutputFiles, "dcb")) {
				if (LogAuto)
					F_Printf(NULL, "Illegal file extension in EmptyOutputFiles: dcl or dcb\n");
				return;
			}
			else
				odfiles = MakeFileList(EmptyOutputFiles);
		}

		if (!odfiles) {
			switch (FormType) {
				case PT_WH3:  // WinHelp 3
				case PT_WH4:  // WinHelp 4
				case PT_WORD7:
				case PT_WORD8:
				case PT_WORDP:
					odfiles = "*.rtf\0*.grx\0\0";
					break;

				case PT_JAVH:  // JavaHelp
				case PT_ORH:   // OracleHelp
				case PT_MSHH:  // MS HTML Help
				case PT_OMH:   // OmniHelp
				case PT_EH:    // EclipseHelp
				case PT_HTML:
				case PT_XHTM:
					odfiles = "*.htm\0*.html\0*.ref\0*.grx\0\0";
					break;

				case PT_XML:
					odfiles = "*.xml\0*.ref\0*.grx\0\0";
					break;

				case PT_DITA:
					odfiles = "*.dita\0*.ditamap\0*.bookmap\0*.ref\0*.grx\0\0";
					break;

				case PT_DCBK:
					odfiles = "*.xml\0*.ent\0*.ref\0*.grx\0\0";
					break;

				case PT_MIF:
				case PT_DCL:
				default:
					break;
			}
		}

		if (odfiles)
			DelCompFiles(odfiles);
	}
}

VoidT WrapShipProc(F_ObjHandleT docId, StringT filename)
{
	StringT command = NULL;
	StringT compiler = NULL;
	StringT suffix = NULL;
	StringT bookname = NULL;
	StringT bkbasename = NULL;
	StringT compdir = NULL;
	//StringT wrapdir = NULL;
	StringT tempcurrdir = NULL;
	StringT graphdir = NULL;
	StringT htmdir = NULL;
	StringT cssdir = NULL;
	StringT tempdir = NULL;
	StringT tempname = NULL;
	StringT ixfilename = NULL;
	StringT wcfiles = NULL;
	StringT gcfiles = NULL;
	StringT ccfiles = NULL;
	StringT jrfiles = NULL;
	StringT ohfiles = NULL;
	UCharT pbuf[_MAX_PATH]; 
	IntT nlen = 0;
	BoolT usecss = False;
	BoolT shippable = False;
	BoolT archived = False;

	// ensure correct starting dir
	if (CurrDrive)
		_chdrive(CurrDrive);
	_chdir(CurrDir);

	if (WrapCopyFiles && *WrapCopyFiles)
		wcfiles = MakeFileList(WrapCopyFiles);
	if (GraphCopyFiles && *GraphCopyFiles)
		gcfiles = MakeFileList(GraphCopyFiles);
	if (CSSCopyFiles && *CSSCopyFiles)
		ccfiles = MakeFileList(CSSCopyFiles);

	switch (FormType) {
		case PT_WH3:  // WinHelp 3
			suffix = ".hpj";
			// *.rtf, *.bmp, *.wmf, *.shg, *.cnt, *.hpj, *.h
			if (!wcfiles)
				wcfiles = "*.rtf\0*.cnt\0*.hpj\0*.h\0\0";
			if (!gcfiles)
				gcfiles = "*.bmp\0*.wmf\0*.shg\0\0";
			if (MakeHLP)
				compiler = Compiler ? Compiler : "hcp";
			break;
		case PT_WH4:  // WinHelp 4
			suffix = ".hpj";
			// *.rtf, *.bmp, *.wmf, *.shg, *.cnt, *.hpj, *.h
			if (!wcfiles)
				wcfiles = "*.rtf\0*.cnt\0*.hpj\0*.h\0\0";
			if (!gcfiles)
				gcfiles = "*.bmp\0*.wmf\0*.shg\0\0";
			if (MakeHLP)
				compiler = Compiler ? Compiler : "hcw /c /e";
			break;
		case PT_WORD7:
		case PT_WORD8:
		case PT_WORDP:
			// *.rtf, *.bmp, *.wmf
			if (!wcfiles)
				wcfiles = "*.rtf\0\0";
			if (!gcfiles)
				gcfiles = "*.bmp\0*.wmf\0\0";
			break;
		case PT_JAVH:  // JavaHelp
		case PT_ORH:   // OracleHelp
			suffix = ".hs";
			// *.htm, *.js, *.xml, *.hs, *.jhm, *.gif, *.jpg, *.png, *.svg
			if (!wcfiles)
				wcfiles = "*.htm\0*.js\0\0";
			if (!gcfiles)
				gcfiles = "*.gif\0*.jpg\0*.png\0*.svg\0\0";
			if (JavaRootFiles && *JavaRootFiles)
				jrfiles = MakeFileList(JavaRootFiles);
			if (!jrfiles)
				jrfiles = "*.hs\0*.jhm\0*.xml\0\0";
			usecss = True;
			break;
		case PT_EH:   // EclipseHelp
			suffix = ".xml";
			// *.htm, *.js, *.xml, *.MF, *.gif, *.jpg, *.png, *.svg
			if (!wcfiles)
				wcfiles = "*.htm\0*.js\0*.xml\0*.MF\0\0";
			if (!gcfiles)
				gcfiles = "*.gif\0*.jpg\0*.png\0*.svg\0\0";
			jrfiles = "doc.zip\0*.xml\0\0";
			usecss = True;
			break;
		case PT_MSHH:  // MS HTML Help
			//filename = NULL;
			suffix = ".hhp";
			// *.htm, *.js, *.hh?, *.h, *.gif, *.jpg, *.png, *.svg
			if (!wcfiles)
				wcfiles = "*.htm\0*.js\0*.hh?\0*.h\0\0";
			if (!gcfiles)
				gcfiles = "*.gif\0*.jpg\0*.png\0*.svg\0\0";
			if (MakeHLP)
				compiler = Compiler ? Compiler : "hhc";
			usecss = True;
			break;
		case PT_OMH:  // OmniHelp
			// *.htm, *.js, *.gif, *.jpg, *.png, *.svg
			if (!wcfiles)
				wcfiles = "*.htm\0*.js\0\0";
			if (!gcfiles)
				gcfiles = "*.gif\0*.jpg\0*.png\0*.svg\0\0";
			if (OHVFiles && *OHVFiles)
				ohfiles = MakeFileList(OHVFiles);
			if (!ohfiles)
				ohfiles = OHProjFileXhtml ? "ox*.htm\0ox*.js\0oh*.js\0oh*.css\0\0" : "oh*.*\0\0";
			usecss = True;
			break;
		case PT_HTML:
		case PT_XHTM:
		case PT_XML:
			// *.htm, *.html, *.xml, *.js, *.txt, *.dtd, *.mod, *.ent, and *.xsd
			// *.gif, *.jpg, *.png, *.svg
			if (!wcfiles)
				wcfiles = "*.htm\0*.html\0*.xml\0*.js\0*.txt\0*.dtd\0*.mod\0*.ent\0*.xsd\0\0";
			if (!gcfiles)
				gcfiles = "*.gif\0*.jpg\0*.png\0*.svg\0\0";
			usecss = True;
			break;
		case PT_DITA:
			// *.dita, *.ditamap, *.bookmap, *.dtd, *.mod, *.ent, and *.xsd
			// *.gif, *.jpg, *.png, *.svg
			if (!wcfiles)
				wcfiles = "*.dita\0*.ditamap\0*.bookmap\0*.dtd\0*.mod\0*.ent\0*.xsd\0\0";
			if (!gcfiles)
				gcfiles = "*.gif\0*.jpg\0*.png\0*.svg\0\0";
			usecss = True;
			break;
		case PT_DCBK:
			// *.xml, *.ent
			// *.gif, *.jpg, *.png, *.svg
			if (!wcfiles)
				wcfiles = "*.xml\0*.ent\0\0";
			if (!gcfiles)
				gcfiles = "*.gif\0*.jpg\0*.png\0*.svg\0\0";
			usecss = True;
			break;
		case PT_MIF:
			if (!wcfiles)
				wcfiles = "*.mif\0\0";
			if (!gcfiles)
				gcfiles = "*.bmp\0*.wmf\0*.gif\0*.jpg\0*.png\0*.svg\0*.tif\0\0";
			break;
		case PT_DCL:
			if (!wcfiles)
				wcfiles = "*.dcl\0\0";
			if (!gcfiles)
				gcfiles = "*.bmp\0*.wmf\0*.gif\0*.jpg\0*.png\0*.svg\0*.tif\0\0";
			break;
		default:
			break;
	}

	if (!ccfiles && usecss)
		ccfiles = "*.css\0*.xsl\0\0";

	// if no output file specified, use bookname
	if ((filename == NULL)
	 || (*filename == '\0')) {
		bookname = F_ApiGetString(FV_SessionId, docId, FP_Name);
		bkbasename = BaseFileName(bookname);
		if (!suffix)
			filename = F_StrCopyString(bkbasename);
		else
			filename = AddFileExt(bkbasename, suffix);
		F_StrFree(bkbasename);
		F_StrFree(bookname);
	}


	// move files to WrapPath and compile
	if ((WrapAndShip
	  || compiler
	  || FTSCommand)
	 && WrapPath
	 && *WrapPath) {
		ShowStatus(docId, filename, StatWrap);
		compdir = MakeDirPath(CurrDir, WrapPath, False);  // empty after graphdir known

#if 0
		wrapdir = Unquote(WrapPath);
		RemoveSlash(wrapdir);
		if ((wrapdir[0] != '\\')
		 && (wrapdir[1] != ':')) {
			compdir = F_StrNew(F_StrLen(CurrDir) + F_StrLen(wrapdir) + 2);
			F_StrCpy(compdir, CurrDir);
			F_StrCat(compdir, "\\");
			F_StrCat(compdir, wrapdir);
		}
		else
			compdir = F_StrCopyString(wrapdir);

		if (!F_StrIEqual(compdir, CurrDir)) {  // make and empty it first
			compdir = F_StrCopyString(_fullpath(pbuf, compdir, _MAX_PATH));
			if (!PathExists(compdir)) {
				if (LogAuto)
					F_Printf(NULL, "mkdir \"%s\"\n", compdir);
				MakeDir(compdir);
			}
		}
#endif

		htmdir = compdir;  // normal case, non-java

		graphdir = MakeDirPath(compdir, GraphPath, EmptyGraphPath
		    && (!F_StrIEqual(graphdir, compdir) || EmptyWrapPath));

		if (usecss)
			cssdir = MakeDirPath(compdir, CSSPath, False);

		if (EmptyWrapPath
		 && !F_StrIEqual(compdir, CurrDir)) {
			if (LogAuto)
				F_Printf(NULL, "del \"%s\" /q\n", compdir);
			DeleteDirContents(compdir);
		}

		if (PT_java(FormType)) {  
			CopyCompFiles(jrfiles, compdir);
			graphdir = MakeDirPath(compdir, GraphSubdir, EmptyJavaGraphSubdir);
			// use [JavaHelpOptions]MapFilePrefix minus trailing / as suffix
			htmdir = MakeDirPath(compdir, HTMLSubdir, EmptyJavaHTMLSubdir);
			cssdir = htmdir;
			if (usecss)
				cssdir = MakeDirPath(htmdir, CSSPath, False);
		}

		CopyCompFiles(wcfiles, htmdir);
		if (usecss)  // splittable types
			RemoveStartSplitFiles(docId, filename, htmdir);

		CopyCompFiles(gcfiles, graphdir);
		if (usecss)
			CopyCompFiles(ccfiles, cssdir);

		if (OHVName
		 && (*OHVName == '%')
		 && (F_StrLen(OHVName) == 1)) {
			if (OHProjFileXhtml)
				OHVName = "%omsyshome%\\common\\system\\omnihelp\\ohvxml";
			else
				OHVName = "%omsyshome%\\common\\system\\omnihelp\\ohvhtm";
		}
		if ((FormType == PT_OMH)
		 && OHVName
		 && (F_StrLen(OHVName) > 2)) {
			OHVName = AddSlash(Unquote(OHVName));
			CopyCompFilesSrc(ohfiles, compdir, OHVName);
		}
	}

	if ((WrapAndShip
	  || compiler
	  || FTSCommand)
	 && CopyGraphicsFrom) {
		CopyGraphicsFrom = AddSlash(Unquote(F_StrCopyString(CopyGraphicsFrom)));
		CopyCompFilesSrc(gcfiles, graphdir, CopyGraphicsFrom);
	}

	if ((WrapAndShip
	  || compiler
	  || FTSCommand)
	 && CopyCSSFrom
	 && usecss) {
		CopyCSSFrom = AddSlash(Unquote(F_StrCopyString(CopyCSSFrom)));
		CopyCompFilesSrc(ccfiles, cssdir, CopyCSSFrom);
	}

	if (WrapAndShip
	  || compiler
	  || FTSCommand) {
		tempcurrdir = CurrDir;
		CurrDir = _fullpath(pbuf, compdir, _MAX_PATH);  // set for subsequent commands
		if (LogAuto)
			F_Printf(NULL, "cd \"%s\"\n", CurrDir);
	}


	// do the compile for WH and HH
	if (compiler) {
		ShowStatus(docId, filename, StatCompile);
		tempname = filename;  // name the project file
		command = F_StrNew(F_StrLen(tempname) + F_StrLen(compiler) + 2);
		sprintf(command, "%s %s", compiler, tempname);
		if (LogAuto)
			F_Printf(NULL, "%s\n", command);
		RunDOSCommand(command, False, KeepCompileWindow);  // always show
		F_StrFree(command);
		if (ShipPath && *ShipPath) {  // do .hlp or .chm
			if (PT_wh(FormType))
				filename = NewFileExt(filename, ".hlp");
			else if (FormType == PT_MSHH)
				filename = NewFileExt(filename, ".chm");
			shippable = True;
		}
	}


	// do the packaging for Eclipse
	if (FormType == PT_EH) {
		if (EclipseUseManifest) {
			if (!PathExists("META-INF")) {
				if (LogAuto)
					F_Printf(NULL, "mkdir META-INF\n");
				MakeDir("META-INF");
			}
			command = "move /Y MANIFEST.MF META-INF";
			if (LogAuto)
				F_Printf(NULL, "%s\n", command);
			RunDOSCommand(command, False, KeepCompileWindow);  // always show
		}
		if (ZipCommand && *ZipCommand && ZipParams && *ZipParams) { // make doc.zip
			ZipCommand = Unquote(ZipCommand);
			command = F_StrNew(F_StrLen(ZipCommand) + F_StrLen(ZipParams) + 4);
			sprintf(command, "\"%s\" %s", ZipCommand, ZipParams);
			if (LogAuto)
				F_Printf(NULL, "%s\n", command);
			RunDOSCommand(command, False, KeepCompileWindow);  // always show
			F_StrFree(command);
		}
	}


	// do the FTS indexing and jarring for Java
	if (PT_java(FormType)) {
		if (FTSCommand
		 && *FTSCommand) {
			FTSCommand = Unquote(FTSCommand);
			if (FormType == PT_JAVH) {
				ShowStatus(docId, filename, StatJHFTS);
				tempname = "JavaHelpSearch";
				RemoveDir(tempname);  // remove any previous version
				if (LogAuto)
					F_Printf(NULL, "rmdir \"%s\"\n", tempname);
				tempname = HTMLSubdir;    // name the dir, not the .hs, for jhindexer
			}
			else if (FormType == PT_ORH) {  // name the dir and the index file
				ShowStatus(docId, filename, StatOHJFTS);
				ixfilename = NewFileExt(filename, ".idx");
				//tempname = F_StrNew(F_StrLen(ixfilename) + F_StrLen(HTMLSubdir) + 2);
				tempname = F_StrNew(F_StrLen(CurrDir) + F_StrLen(ixfilename) + 2);
				sprintf(tempname, "%s %s", CurrDir, ixfilename);  // name the dir and the .idx
			}
			command = F_StrNew(F_StrLen(tempname) + F_StrLen(FTSCommand) + 2);
			sprintf(command, "%s %s", FTSCommand, tempname);
			if (LogAuto)
				F_Printf(NULL, "%s\n", command);
			RunDOSCommand(command, False, KeepCompileWindow);  // always show
			F_StrFree(command);
		}

		if (JarCommand
		 && *JarCommand) {
			ShowStatus(docId, filename, StatJar);
			JarCommand = Unquote(JarCommand);
			filename = NewFileExt(filename, ".jar");
			// allow for cvf, *, 2 quotes, 3 spaces, and 1 nul
			command = F_StrNew(F_StrLen(JarCommand) + F_StrLen(filename) + 10);
			sprintf(command, "\"%s\" cvf %s *", JarCommand, filename);
			if (LogAuto)
				F_Printf(NULL, "%s\n", command);
			RunDOSCommand(command, True, False);
			F_StrFree(command);
			shippable = True;
		}
	}

	// do zip or other archiving
	if (WrapAndShip
	 && ArchiveCommand
	 && *ArchiveCommand) {
		ShowStatus(docId, filename, StatArchive);
		ArchiveCommand = Unquote(ArchiveCommand);
		if (ArchiveName) {
			tempname = F_StrChr(ArchiveName, '.');
			nlen = F_StrLen(ArchiveName);

			if (ArchiveVer)
				nlen += F_StrLen(ArchiveVer);

			if (ArchiveExt)
				nlen += F_StrLen(ArchiveExt);
			else
				nlen += 3;
			filename = F_StrNew(nlen + 2);

			if (tempname)
				F_StrCpyN(filename, ArchiveName, tempname - ArchiveName);
			else
				F_StrCpy(filename, ArchiveName);

			if (ArchiveVer)
				F_StrCat(filename, ArchiveVer);

			if (ArchiveExt) {
				if (ArchiveExt[0] != '.')
					F_StrCat(filename, ".");
				F_StrCat(filename, ArchiveExt);
			}
			else if (tempname)
				F_StrCat(filename, tempname);
			else  // no extension, add one
				F_StrCat(filename, ".zip");
		}
		else if (FormType == PT_EH)
			filename = "plugin.zip";
		else
			filename = NewFileExt(filename, ".zip");

		nlen = F_StrLen(ArchiveCommand) + F_StrLen(filename);
		if (ArchiveStartParams)
			nlen += F_StrLen(ArchiveStartParams);
		if (ArchiveEndParams)
			nlen += F_StrLen(ArchiveEndParams);
		nlen += 6; // 2 quotes, 3 spaces, and 1 nul
		command = F_StrNew(nlen);

		sprintf(command, "\"%s\" %s %s %s", ArchiveCommand,
											(ArchiveStartParams ? ArchiveStartParams : ""),
											filename,
											(ArchiveEndParams ? ArchiveEndParams : ""));

		if (LogAuto)
			F_Printf(NULL, "%s\n", command);
		RunDOSCommand(command, True, False);
		F_StrFree(command);
		shippable = True;
		archived = True;
	}

	// copy to ship path if any
	if (WrapAndShip
	 && ShipPath && *ShipPath
	 && shippable) {
		ShowStatus(docId, filename, StatShip);
		ShipPath = Unquote(ShipPath);
		if (!F_StrIEqual(ShipPath, CurrDir)) {  // make it first
			if (!PathExists(ShipPath)) {
				if (LogAuto)
					F_Printf(NULL, "mkdir \"%s\"\n", ShipPath);
				MakeDir(ShipPath);       // harmless even if it exists
			}
		}
		command = F_StrNew(F_StrLen(filename) + F_StrLen(ShipPath) + 14);
		sprintf(command, "%s /Y \"%s\" \"%s\"", (MoveArchive ? "move" : "copy"),
		                      filename, ShipPath);
		if (LogAuto)
			F_Printf(NULL, "%s\n", command);
		RunDOSCommand(command, True, False);
		if (PT_wh(FormType)
		 && !archived) {
			filename = NewFileExt(filename, ".cnt");
			sprintf(command, "%s /Y \"%s\" \"%s\"", (MoveArchive ? "move" : "copy"),
														filename, ShipPath);
			if (LogAuto)
				F_Printf(NULL, "%s\n", command);
			RunDOSCommand(command, True, False);
		}
		F_StrFree(command);
	}

	if (tempcurrdir)
		CurrDir = tempcurrdir;
	if (!F_StrIEqual(compdir, graphdir))
		F_StrFree(graphdir);
	F_StrFree(compdir);

	ClearStatus(docId);
}

StringT MakeFileList(StringT filenames)
{
	StringT flist = NULL;
	StringT fp = NULL;
	StringT flp = NULL;
	BoolT sp = False;
	BoolT start = True;

	flp = flist = F_StrNew(F_StrLen(filenames) + 2);
	for (fp = filenames; *fp; fp++) {
		if ((*fp == ' ')  // delimiters
		 || (*fp == '\t')
		 || (*fp == ',')) {
			if (start || sp)  // leading or repeated
				continue;
			sp = True;
			*flp++ = '\0';
		}
		else {
			sp = False;
			start = False;
			*flp++ = *fp;
		}
	}
	if (!sp) // last delimiter
		*flp++ = '\0'; 
	*flp = '\0'; // terminator
	return flist;
}

StringT MakeDirPath(StringT base, StringT path, BoolT empty)
{
	StringT dir = base;
	static UCharT pbuf[_MAX_PATH]; 

	if (path && *path) {
		path = Unquote(path);
		RemoveSlash(path);
		if ((path[0] != '\\')
		 && (path[1] != ':')) {
			dir = F_StrNew(F_StrLen(base) + F_StrLen(path) + 2);
			F_StrCpy(dir, base);
			F_StrCat(dir, "\\");
			F_StrCat(dir, path);
		}
		else
			dir = F_StrCopyString(path);

		if (!F_StrIEqual(dir, CurrDir)) {
			dir = F_StrCopyString(_fullpath(pbuf, dir, _MAX_PATH));
			if (!PathExists(dir)) {
				if (LogAuto)
					F_Printf(NULL, "mkdir \"%s\"\n", dir);
				MakeDir(dir);
			}
			else if (empty) {
				if (LogAuto)
					F_Printf(NULL, "del \"%s\" /q\n", dir);
				DeleteDirContents(dir);
			}
		}
	}
	return dir;
}


VoidT CopyCompFiles(StringT filelist, StringT dest)
{
	ProcCompFileList(filelist, dest, "", False);
}

VoidT CopyCompFilesSrc(StringT filelist, StringT dest, StringT src)
{
	ProcCompFileList(filelist, dest, src, False);
}

VoidT DelCompFiles(StringT filelist)
{
	ProcCompFileList(filelist, "", "", True);
}

VoidT ProcCompFileList(StringT filelist, StringT dest, StringT src, BoolT del)
{
	StringT command = NULL;
	StringT item = NULL;
	IntT maxitemlen = 9;   // allow for *.ditamap
	static UCharT pbuf[_MAX_PATH]; 

	if (!del
	 && F_StrIEqual(_fullpath(pbuf, dest, _MAX_PATH), CurrDir))
		return;

	for (item = filelist; *item != '\0'; item += F_StrLen(item) + 1) {
		if (F_StrLen(item) > maxitemlen)
			maxitemlen = F_StrLen(item);
	}

	command = (del ? F_StrNew(maxitemlen + 10)
	               : F_StrNew(F_StrLen(dest) + F_StrLen(src) + maxitemlen + 14));
	
	for (item = filelist; *item != '\0'; item += F_StrLen(item) + 1) {
		if (del)  // del /Q item
			sprintf(command, "del /Q \"%s\"", item);
		else      // copy /Y item dest
			sprintf(command, "copy /Y \"%s%s\" \"%s\"", src, item, dest);
		if (LogAuto)
			F_Printf(NULL, "%s\n", command);
		RunDOSCommand(command, True, False);
		if (ExitCode == 1)
			ExitCode = 0;  // ignore missing file errors
	}

	F_StrFree(command);
}


VoidT RemoveStartSplitFiles(F_ObjHandleT docId,
	                          StringT filename, StringT htmdir)
{
	FILE *cfile = NULL;
	UCharT ListBuf[MAX_PROFILE];
	StringT rname = NULL;
	StringT val = NULL;
	StringT tdir = NULL;
	StringT command = NULL;
	StringT bookname = NULL;
	StringT bkbasename = NULL;

	// get ListFileName in CurrDir, or create it from book filename
	if (!ListFileName) {
		bookname = F_ApiGetString(FV_SessionId, docId, FP_Name);
		bkbasename = BaseFileName(bookname);
		BuildListFile(docId, bkbasename);
		F_StrFree(bkbasename);
		F_StrFree(bookname);
	}
	// verify first line starts with "LIST"
	if ((cfile = fopen(ListFileName, "rt")) == NULL)
		return;
	if (!fgets(ListBuf, MAX_PROFILE, cfile))
		return;
	if (!F_StrEqualN(ListBuf, "LIST", 4))
		return;
	tdir = CurrDir;
	command = F_StrNew(256);

	while (fgets(ListBuf, MAX_PROFILE, cfile)) {
		// for each subsequent line:
		ListBuf[F_StrLen(ListBuf) - 1] = '\0';  // trim newline
		rname = NewFileExt(ListBuf, ".ref");  // make .ref name 
		SetCurrPath(&rname);
		// see if [Splits]StartSplit=Yes
		GetRawIniString(rname, "Splits", "StartSplit", &val);
		if (val && F_StrEqualN(val, "Yes", 3)) {
			F_StrFree(val);
			val = NULL;
			GetRawIniString(rname, "SplitFileNames", "1", &val);
			if (val) { // delete that file in htmdir
				sprintf(command, "del \"%s\" /q", val);
				CurrDir = htmdir;
				RunDOSCommand(command, True, False);
				CurrDir = tdir;
				F_StrFree(val);
				val = NULL;
			}
		}
		F_StrFree(rname);
	}
	fclose(cfile);
	F_StrFree(command);
}



/* End of m2rproc.c */

