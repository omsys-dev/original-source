/* m2rbook.c is the FDK wrapper for mif2go FM book export 
 * Last edited by Jeremy H. Griffith.
 * Copyright 1998-2009 Omni Systems, Inc.  All Rights Reserved.
 */

#include "m2rbook.h"

#if 0
extern "C" {
long _ftol(double);
long _ftol2_sse(double dblSource) { return _ftol(dblSource); }
}
#endif


StringT PluginVersion = { "b114a" };
IntT FrameVersion = 0;


/* called by api at startup */

VoidT F_ApiInitialize(IntT init)
{
	UCharT buffer[_MAX_PATH];

	/* only initialize once */

	if (init != FA_Init_First)
		return;

	__try {
		/* request notification from m2r.c */
		F_ApiNotification(FA_Note_ClientCall, True);
		F_ApiNotification(FA_Note_PreMouseCommand, True);
		F_ApiNotification(FA_Note_PostMouseCommand, True);

		CurrDir = F_StrCopyString(_getcwd( buffer, _MAX_PATH ));
		FrameVersion = F_ApiGetInt(0, FV_SessionId, FP_VersionMajor);

		/* make menu changes */
		CreateFMMenu();

		/* do any needed DLL initializations */
		SetStrings(PT_WORD8);  /* set for RTF as default */
	}
	__except(EXCEPTION_EXECUTE_HANDLER) {
		F_ApiAlert("Error initializing m2rbook.", FF_ALERT_CONTINUE_NOTE);
	}
}


/* called when m2r is invoked by a SaveAs mif2rtf operation */

VoidT F_ApiNotify(IntT notification, F_ObjHandleT docId,
                  StringT filename, IntT iparm)
{
	F_ObjHandleT objId;
	F_TextRangeT txtRange;
	IntT val = 0;
	StringT msg;
	StringT term;
	static IntT statline = 0;

	__try {
		/* docId is not passed, but must be the active doc */
		if (docId == 0)
			docId = F_ApiGetId(0, FV_SessionId, FP_ActiveDoc);

		if (notification == FA_Note_PostMouseCommand) {
			if (((objId = F_ApiGetId(0, docId, FP_FirstSelectedGraphicInDoc)) != 0)
			 && (F_ApiGetObjectType(docId, objId) == FO_AFrame)) {
				/* display frame ID in hex */
				val = F_ApiGetInt(docId, objId, FP_Unique) & GrFileNumMask;
				msg = F_StrNew(GrFileNumLen + 11);
				F_Sprintf(msg, "FrameID: %0.*lx", GrFileNumLen, val);
				F_ApiSetString(FV_SessionId, docId, FP_StatusLine, msg);
				F_StrFree(msg);
				statline = 1;
			}
			else if (((objId = F_ApiGetId(0, docId, FP_SelectedTbl)) != 0)
			 && (F_ApiGetObjectType(docId, objId) == FO_Tbl)
			 && (GetKeyState(VK_SHIFT) < 0)) {
				/* display frame ID in decimal */
				val = F_ApiGetInt(docId, objId, FP_Unique);
				msg = F_StrNew(21);
				F_Sprintf(msg, "TableID: %ld", val);
				F_ApiSetString(FV_SessionId, docId, FP_StatusLine, msg);
				F_StrFree(msg);
				statline = 1;
			}
			else {
				txtRange = F_ApiGetTextRange(0, docId, FP_TextSelection);
				if (((objId = txtRange.beg.objId) != 0)
				 && (F_ApiGetObjectType(docId, objId) == FO_Pgf)
				 && (GetKeyState(VK_SHIFT) < 0)) {
					/* display para ID in decimal */
					val = F_ApiGetInt(docId, objId, FP_Unique);
					msg = F_StrNew(21);
					F_Sprintf(msg, "ParaID: %ld", val);
					F_ApiSetString(FV_SessionId, docId, FP_StatusLine, msg);
					F_StrFree(msg);
					statline = 1;
				}
			}
		}
		else if (notification == FA_Note_PreMouseCommand) {
			if (statline) {
				F_ApiSetString(FV_SessionId, docId, FP_StatusLine, "");
				statline = 0;
			}
		}
		else if (notification == FA_Note_ClientCall) {
			if (((val = strtol(filename, &term, 10)) > 0)
			 && (*term == ';')) {  /* starts with number and semi */
				FormType = ProjSelType = val;
				SetStrings(val);
				filename = term + 1;
				ProcSaveAs(docId, filename);  /* name is dest name, not source */
			}
			else if (F_StrChr(filename, '='))
				ProcProjSet(filename);
			else
				ProcProject(docId, filename); // name is project name
		}
	}
	__except(EXCEPTION_EXECUTE_HANDLER) {
		F_ApiAlert("Error processing m2rbook notification.", FF_ALERT_CONTINUE_NOTE);
	}
}


/* called in response to a hypertext message */

VoidT F_ApiMessage(StringT message, F_ObjHandleT docId, F_ObjHandleT objId)
{
	IntT val = 0;
	StringT term;

	__try {
		/* if docId is not passed, must be the active doc */
		if (docId == 0)
			docId = F_ApiGetId(0, FV_SessionId, FP_ActiveDoc);

		if (((val = strtol(message, &term, 10)) > 0)
		 && (*term == ';')) {  /* starts with number and semi */
			FormType = ProjSelType = val;
			SetStrings(val);
			message = term + 1;
			ProcSaveAs(docId, message);  /* name is dest name, not source */
		}
		else {
			if (docId == 0)
				docId = F_ApiGetId(0, FV_SessionId, FP_ActiveBook);
			ProcProject(docId, message); // name is project name
		}
	}
	__except(EXCEPTION_EXECUTE_HANDLER) {
		F_ApiAlert("Error processing m2rbook hypertext command.", FF_ALERT_CONTINUE_NOTE);
	}
}


/* called when one of our commands is invoked */

VoidT F_ApiCommand(IntT command)
{
	__try {
		switch(command) {
			case EXPORT_FILE:
				ExportFile();
				break;
			case EXPORT_BOOK:
				ExportBook();
				break;
			case EXPORT_SETUP:
				SetupExport();
				break;
			case EXPORT_BK_SETUP:
				SetupBkExport(0);
				break;
			case WASH_FILE:
				WashFile();
				break;
			case WASH_BOOK:
				WashBook();
				break;
			case WASH_BOOK_ALL:
				WashBookAll();
				break;
	#if 0
			case CONVERSION_DES:
				InitDesigner();
				break;
	#endif
			default:  /* error */
				return;
		}
	}
	__except(EXCEPTION_EXECUTE_HANDLER) {
		F_ApiAlert("Error processing m2rbook command.", FF_ALERT_CONTINUE_NOTE);
	}
}


/* called when dialog is up */

VoidT F_ApiDialogEvent(IntT dlgNum, IntT itemNum, IntT modifiers)
{
	__try {
		switch (dlgNum) {
			case DLG_CHOOSE_PROJECT:
				ProcProjDialogEvent(itemNum);
				break;
			case DLG_SETUP_HLP_INI:
				ProcSetupHelpIniDialogEvent(itemNum);
				break;
			case DLG_SETUP_WRD_INI:
				ProcSetupWordIniDialogEvent(itemNum);
				break;
			case DLG_SETUP_HTML_INI:
				ProcSetupHTMLIniDialogEvent(itemNum);
				break;
			case DLG_SETUP_DITA_INI:
				ProcSetupDITAIniDialogEvent(itemNum);
				break;
			case DLG_SETUP_DCBK_INI:
				ProcSetupDocBookIniDialogEvent(itemNum);
				break;
			case DLG_SETUP_FMIF_INI:
				ProcSetupFMIFIniDialogEvent(itemNum);
				break;
			case DLG_SETUP_ADCL_INI:
				ProcSetupADCLIniDialogEvent(itemNum);
				break;
			case DLG_SETUP_MSHH_INI:
				ProcSetupHHIniDialogEvent(itemNum);
				break;
			case DLG_SETUP_OMH_INI:
				ProcSetupOMHIniDialogEvent(itemNum);
				break;
			case DLG_SETUP_JAVH_INI:
				ProcSetupJHIniDialogEvent(itemNum);
				break;
			case DLG_SETUP_EH_INI:
				ProcSetupEHIniDialogEvent(itemNum);
				break;

			case DLG_INIT:
				ProcInitDialogEvent(itemNum);
				break;
			case DLG_DEF_VAR:
				ProcVarDialogEvent(itemNum);
				break;
			default: // must be the Designer
				if ((dlgNum > DLG_DESIGNER_BASE)
				 && ((dlgNum -= DLG_DESIGNER_BASE) <= DLG_DES_COUNT))
					ProcDesignerEvent(dlgNum, itemNum);
				break;
		}
	}
	__except(EXCEPTION_EXECUTE_HANDLER) {
		F_ApiAlert("Error processing m2rbook dialog event.", FF_ALERT_CONTINUE_NOTE);
	}
}


/* export command implementations */

VoidT ExportFile(VoidT)
{
	StringT filename = NULL;
	StringT tempname = NULL;
	StringT basename = NULL;
	StringT projname = NULL;
	StringT newname = NULL;
	StringT pathname = NULL;
	StringT extension = NULL;
	StringT filext = NULL;
	F_ObjHandleT docId = 0;
	F_ObjHandleT bookId = 0;
	IntT pathlen = 0;
	IntT err = 0;
	IntT autosave = 0;
	BoolT proj = False;

	if ((autosave = F_ApiGetInt(0, FV_SessionId, FP_AutoSave)) != False)
		F_ApiSetInt(0, FV_SessionId, FP_AutoSave, False);

	docId = F_ApiGetId(0, FV_SessionId, FP_ActiveDoc);
	//filename = Fm2a(F_ApiGetString(FV_SessionId, docId, FP_Name));
	filename = F_ApiGetString(FV_SessionId, docId, FP_Name);

	if ((pathlen = SetPathName(&pathname, filename, True)) == 0)
		pathname = "";

	projname = NewFileExt(filename, PrjExt);
	basename = BaseFileName(filename);

	if (FileExists(projname))   /* select project */
		proj = True;
 	else if ((bookId = IdentifyBookOfDoc(filename)) != 0) {
		F_StrFree(filename);
		F_StrFree(projname);
		filename = F_ApiGetString(FV_SessionId, bookId, FP_Name);
		projname = NewFileExt(filename, PrjExt);
		if (FileExists(projname))   /* select book project */
			proj = True;
	}

	if (proj) {
		EditProjDialog(filename);
		if (ProjCancelled) {
			F_ApiAlert(ExportFileCancelled, FF_ALERT_CONTINUE_NOTE);
			goto AllDone;
		}
		SetStrings(ProjSelType);
		filext = AddFileExt(basename, DestExt);
		FormType = ProjSelType;
		newname = MakeFileName(CurrProjPath, filext);
	}
	else 	{
		F_ApiAlert(IniMissing, FF_ALERT_CONTINUE_NOTE);
		SetupExport();
		goto AllDone;
#if 0
		/* run file-selection (save) dialog to get full path to dest file */
		SetStrings(PT_WORD7);
		filext = AddFileExt(basename, RtfExt);
		FormType = ProjSelType = PT_WORD7;
		err = F_ApiChooseFile(&newname, ChooseDestFileStr,
													pathname, filext, FV_ChooseSave, (StringT) "");
		if (!err
		 && ((extension = F_StrRChr(newname, '.')) != NULL)
		 && (!F_StrICmp(extension, HtmExt)
		  || !F_StrICmp(extension, HtmlExt))) {
			ProjSelType = PT_HTML;
			SetStrings(PT_HTML);
		}
#endif
	}

	if (err)
		F_ApiAlert(ExportFileCancelled, FF_ALERT_CONTINUE_NOTE);
	else
		ProcSaveAs(docId, newname);

	/* clean up */
	F_StrFree(newname);
AllDone:
	F_StrFree(filename);
	F_StrFree(basename);
	F_StrFree(projname);
	if (pathlen)
		F_StrFree(pathname);
	F_StrFree(filext);
	if (autosave)
		F_ApiSetInt(0, FV_SessionId, FP_AutoSave, True);
}


VoidT ExportBook(VoidT)
{
	F_ObjHandleT bookId = 0;

	StringT bookname = NULL;

	IntT autosave = 0;

	/* get active book info */
	bookId = F_ApiGetId(0, FV_SessionId, FP_ActiveBook);
	bookname = F_ApiGetString(FV_SessionId, bookId, FP_Name);
	if ((autosave = F_ApiGetInt(0, FV_SessionId, FP_AutoSave)) != False)
		F_ApiSetInt(0, FV_SessionId, FP_AutoSave, False);

	/* select project */
	EditProjDialog(bookname);
	if (ProjCancelled) { /* repeat this here to avoid goto */
		F_ApiAlert(ExportFileCancelled, FF_ALERT_CONTINUE_NOTE);
		F_StrFree(bookname);
		if (autosave)
			F_ApiSetInt(0, FV_SessionId, FP_AutoSave, True);
		return;
	}

	SetStrings(ProjSelType);
	FormType = ProjSelType;

	/* make the actual conversion */
	ProcSaveAsBook(bookId, bookname, CurrProjPath);

	/* clean up */
	F_StrFree(bookname);
	if (autosave)
		F_ApiSetInt(0, FV_SessionId, FP_AutoSave, True);
}



/* setup command implementations */

VoidT SetupExport(VoidT)
{
	F_ObjHandleT docId = 0;
	F_ObjHandleT bookId = 0;
	StringT filename = NULL;
	StringT destpath = NULL;
	StringT ininame = NULL;
	StringT bookname = NULL;
	StringT projname = NULL;
	BoolT proj = False;

	docId = F_ApiGetId(0, FV_SessionId, FP_ActiveDoc);
	filename = F_ApiGetString(FV_SessionId, docId, FP_Name);
	//filename = Fm2a(F_ApiGetString(FV_SessionId, docId, FP_Name));
	projname = NewFileExt(filename, PrjExt);

	if (!FileExists(projname)   /* select project */
	 && ((bookId = IdentifyBookOfDoc(filename)) != 0)) {
		F_StrFree(projname);
		bookname = F_ApiGetString(FV_SessionId, bookId, FP_Name);
		projname = NewFileExt(bookname, PrjExt);
		if (FileExists(projname)   /* select book project */
		 && (F_ApiAlert(DoBookSetup, FF_ALERT_YES_DEFAULT) != -1)) {
			F_StrFree(filename);
			F_StrFree(bookname);
			F_StrFree(projname);
			SetupBkExport(bookId);
			return;
		}
	}

	EditProjDialog(filename);
	if (ProjCancelled) { /* repeat this here to avoid goto */
		F_StrFree(filename);
		return;
	}

	SetStrings(ProjSelType);

	//ininame = MakeFileName(CurrProjPath, StdIni);
	destpath = Fm2a(CurrProjPath);
	ininame = MakeFileName(destpath, IniNameList[ProjSelType - 1]);
	F_StrFree(destpath);

	if (!FileExists(ininame)) {
		RunSetupDialog(filename, (F_ObjHandleT) 0);

		if (!SetupCancelled) {
			WriteStartingIni(CurrProjPath, filename, False, CurrProjType);
			F_StrFree(ininame);
			ininame = F_StrCopyString(CurrIniName);
		}
	}

	if (!SetupCancelled) {
		WriteHelpProj(CurrProjPath, filename, False, CurrProjType, ininame);
		if (FileExists(ininame)) {
			TrueFalseIni(ininame, "FDK", "ConversionDesigner", &CondDes);
			if (PT_rtf(ProjSelType) && CondDes && (FrameVersion < 9))
				InitDesigner(ProjSelType, ininame, docId, False);
			else
				EditIniFile(ininame);
		}
	}

	F_StrFree(filename);
	F_StrFree(ininame);
}


VoidT SetupBkExport(F_ObjHandleT bookId)
{
	StringT bookname = NULL;
	StringT ininame = NULL;
	StringT destpath = NULL;

	if (bookId == 0)
		bookId = F_ApiGetId(0, FV_SessionId, FP_ActiveBook);
	bookname = F_ApiGetString(FV_SessionId, bookId, FP_Name);

	EditProjDialog(bookname);
	if (ProjCancelled) { /* repeat this here to avoid goto */
		F_StrFree(bookname);
		return;
	}

	SetStrings(ProjSelType);

	//ininame = MakeFileName(CurrProjPath, StdIni);
	destpath = Fm2a(CurrProjPath);
	ininame = MakeFileName(destpath, IniNameList[ProjSelType - 1]);
	F_StrFree(destpath);

	if (!FileExists(ininame)) {
		RunSetupDialog(bookname, bookId);

		if (!SetupCancelled) {
			WriteStartingIni(CurrProjPath, bookname, True, CurrProjType);
			F_StrFree(ininame);
			ininame = F_StrCopyString(CurrIniName);
		}
	}

	if (!SetupCancelled) {
		WriteHelpProj(CurrProjPath, bookname, True, CurrProjType, ininame);

		if (FileExists(ininame)) {
			TrueFalseIni(ininame, "FDK", "ConversionDesigner", &CondDes);
			if (PT_rtf(ProjSelType) && CondDes && (FrameVersion < 9))
				InitDesigner(ProjSelType, ininame, bookId, True);
			else
				EditIniFile(ininame);
		}
	}

	F_StrFree(bookname);
	F_StrFree(ininame);
}


VoidT RunSetupDialog(StringT name, F_ObjHandleT id)
{
	if (PT_wh(ProjSelType))
		SetupHelpIniDialog(name, id);
	else if (PT_word(ProjSelType))
		SetupWordIniDialog(name, id);
	else if (ProjSelType == PT_MSHH)
		SetupHHIniDialog(name, id);
	else if (ProjSelType == PT_OMH)
		SetupOMHIniDialog(name, id);
	else if (PT_java(ProjSelType))
		SetupJHIniDialog(name, id);
	else if (ProjSelType == PT_EH)
		SetupEHIniDialog(name, id);
	else if (ProjSelType == PT_DITA)
		SetupDITAIniDialog(name, id);
	else if (ProjSelType == PT_DCBK)
		SetupDocBookIniDialog(name, id);
	else if (PT_htm(ProjSelType))
		SetupHTMLIniDialog(name, id);
	else if (ProjSelType == PT_MIF)
		SetupFMIFIniDialog(name, id);
	else if (ProjSelType == PT_DCL)
		SetupADCLIniDialog(name, id);
}



// wash command implementations

VoidT WashFile(VoidT)
{
	F_ObjHandleT docId = 0;
	StringT	docname = NULL;
	StringT	tmpfilename = NULL;

	docId = F_ApiGetId(0, FV_SessionId, FP_ActiveDoc);
	WashOne(docId);
}


VoidT WashBook(VoidT)
{
	F_ObjHandleT bookId = 0;
	F_ObjHandleT mifBookId = 0;

	bookId = F_ApiGetId(0, FV_SessionId, FP_ActiveBook);
	WashOne(bookId);
}


VoidT WashBookAll(VoidT)
{
	F_ObjHandleT bookId = 0;
	F_ObjHandleT compId = 0;
	F_ObjHandleT docId = 0;
	StringT	docname = NULL;
	StringT	tmpfilename = NULL;
	BoolT opened = False;

	bookId = F_ApiGetId(0, FV_SessionId, FP_ActiveBook);
	bookId = WashOne(bookId);
	for (compId = F_ApiGetId(FV_SessionId, bookId, FP_FirstComponentInBook);
	 compId; compId = F_ApiGetId(bookId, compId, FP_NextComponentInBook)) {
		docname = F_ApiGetString(bookId, compId, FP_Name);
		if (!FileExists(docname)) {
			F_StrFree(docname);
			continue;
		}
		docId = IdentifyDocName(docname);
		if (!docId) {
			ShowStatus(docId, docname, StatOpening);
			docId = OpenDocName(docname);
			ClearStatus(docId);
			opened = True;
		}
		else
			opened = False;
		docId = WashOne(docId);
		if (opened)
			F_ApiClose(docId, FF_CLOSE_MODIFIED);
	}
}


F_ObjHandleT WashOne(F_ObjHandleT docId)
{
	StringT	docname = NULL;
	StringT	tmpfilename = NULL;

	docname = F_ApiGetString(FV_SessionId, docId, FP_Name);
	tmpfilename = NewFileExt(docname, "tmb");
	if (!SaveMIFforDCL(docId, tmpfilename)) /* could not save */
		F_Printf(NULL, ReportMIFFail, (StringT) tmpfilename);
	else { // move into place
		F_ApiClose(docId, FF_CLOSE_MODIFIED);
		docId = OpenDocName(tmpfilename);
		SaveDoc(docId, docname, True);
		DeleteFile(tmpfilename);
		F_Printf(NULL, "File washed via MIF: %s\n", (StringT) docname);
	}

	F_StrFree(docname);
	F_StrFree(tmpfilename);
	return docId;
}



/* end of m2rbook.c */

