/* m2gmenu.cpp is the menu processor for mif2go
 * Copyright 2005 Omni Systems, Inc.  All Rights Reserved.
 */

#include "m2gframe.h"


/* globals */

#if 0
StringT CvtFileStr = "Convert file...";
StringT CvtBookStr = "Convert book...";
StringT CvtRptStr = "Repeat conversion...";
StringT NewPrjStr = "New project...";
StringT ModPrjStr = "Modify project...";
StringT SelPrjStr = "Select project...";
StringT ConDesStr = "Conversion Designer...";
StringT EditIniStr = "Edit mif2rtf.ini...";
#endif


VoidT M2GFrame::CreateFMMenu(VoidT)
{
#if 0
	F_ObjHandleT mainMenuId;
	F_ObjHandleT qMainMenuId;
	F_ObjHandleT bookMainMenuId;
	F_ObjHandleT qBookMainMenuId;

	F_ObjHandleT helpMenuId;
	F_ObjHandleT m2gMenuId;

	F_ObjHandleT cvtFileCmdId;
	F_ObjHandleT cvtBookCmdId;
	F_ObjHandleT cvtRepCmdId;

	F_ObjHandleT sep1Id;
	F_ObjHandleT newPrjCmdId;
	F_ObjHandleT modPrjCmdId;
	F_ObjHandleT selPrjCmdId;

	F_ObjHandleT sep2Id;
	F_ObjHandleT conDesCmdId;
	F_ObjHandleT editIniCmdId;
#endif


#if 0
	F_ObjHandleT fileMenuId;
	F_ObjHandleT qFileMenuId;
	F_ObjHandleT bookFileMenuId;
	F_ObjHandleT qBookFileMenuId;

	F_ObjHandleT saveAsCmdId;
	F_ObjHandleT preferencesCmdId;

	F_ObjHandleT exportFileCmdId;
	F_ObjHandleT exportBookCmdId;
	F_ObjHandleT exportSetupCmdId;
	F_ObjHandleT exportBkSetupCmdId;
#endif

#if 0
	F_ObjHandleT formatMenuId;
	F_ObjHandleT headFootCmdId;
	F_ObjHandleT convDesCmdId;
#endif

#if 0
	mainMenuId = F_ApiGetNamedObject(FV_SessionId, FO_Menu, "!MakerMainMenu");
	qMainMenuId = F_ApiGetNamedObject(FV_SessionId, FO_Menu, "!QuickMakerMainMenu");
	bookMainMenuId = F_ApiGetNamedObject(FV_SessionId, FO_Menu, "!BookMainMenu");
	qBookMainMenuId = F_ApiGetNamedObject(FV_SessionId, FO_Menu, "!QuickBookMainMenu");

	helpMenuId = F_ApiGetNamedObject(FV_SessionId, FO_Menu, "!HelpMenu");

	m2gMenuId = F_ApiDefineAndAddMenu(mainMenuId, "Mif2GoMenu", "Mif2Go");
	F_ApiSetId(mainMenuId, m2gMenuId, FP_NextMenuItemInMenu, 0);
	F_ApiAddMenuToMenu(qMainMenuId, m2gMenuId);
	F_ApiSetId(qMainMenuId, m2gMenuId, FP_NextMenuItemInMenu, helpMenuId);
	F_ApiAddMenuToMenu(bookMainMenuId, m2gMenuId);
	F_ApiSetId(bookMainMenuId, m2gMenuId, FP_NextMenuItemInMenu, helpMenuId);
	F_ApiAddMenuToMenu(qBookMainMenuId, m2gMenuId);
	F_ApiSetId(qBookMainMenuId, m2gMenuId, FP_NextMenuItemInMenu, helpMenuId);

	cvtFileCmdId = F_ApiDefineAndAddCommand(CVT_FILE, m2gMenuId,
									"CvtM2gFile", CvtFileStr, "");
	F_ApiSetInt(FV_SessionId, cvtFileCmdId, FP_EnabledWhen,
							 FV_ENABLE_NEEDS_DOCP_ONLY);
	cvtBookCmdId = F_ApiDefineAndAddCommand(CVT_BOOK, m2gMenuId,
									"CvtM2gBook", CvtBookStr, "");
	F_ApiSetInt(FV_SessionId, cvtBookCmdId, FP_EnabledWhen,
							 FV_ENABLE_NEEDS_BOOKP_ONLY);
	cvtRepCmdId = F_ApiDefineAndAddCommand(REP_CVT, m2gMenuId,
									"CvtM2gRep", CvtRptStr, "");

	sep1Id = F_ApiGetNamedObject(FV_SessionId, FO_MenuItemSeparator, "Separator1");
	F_ApiAddCommandToMenu(m2gMenuId, sep1Id);

	newPrjCmdId = F_ApiDefineAndAddCommand(NEW_PRJ, m2gMenuId,
									"NewM2gPrj", NewPrjStr, "");
	modPrjCmdId = F_ApiDefineAndAddCommand(MOD_PRJ, m2gMenuId,
									"ModM2gPrj", ModPrjStr, "");
	selPrjCmdId = F_ApiDefineAndAddCommand(SEL_PRJ, m2gMenuId,
									"SelM2gPrj", SelPrjStr, "");

	sep2Id = F_ApiGetNamedObject(FV_SessionId, FO_MenuItemSeparator, "Separator2");
	F_ApiAddCommandToMenu(m2gMenuId, sep2Id);

	conDesCmdId = F_ApiDefineAndAddCommand(CON_DES, m2gMenuId,
									"ConDes", ConDesStr, "");
	editIniCmdId = F_ApiDefineAndAddCommand(EDIT_INI, m2gMenuId,
									"EditIni", EditIniStr, "");
#endif



	// keep original file-menu commands

#if 0
	fileMenuId = F_ApiGetNamedObject(FV_SessionId, FO_Menu, "FileMenu");
	qFileMenuId = F_ApiGetNamedObject(FV_SessionId, FO_Menu, "QuickFileMenu");
	bookFileMenuId = F_ApiGetNamedObject(FV_SessionId, FO_Menu, "BookFileMenu");
	qBookFileMenuId = F_ApiGetNamedObject(FV_SessionId, FO_Menu, "QuickBookFileMenu");

	saveAsCmdId = F_ApiGetNamedObject(FV_SessionId, FO_Command, "SaveAs");
	preferencesCmdId = F_ApiGetNamedObject(FV_SessionId, FO_MenuItemSeparator, "Separator4");

	exportFileCmdId = F_ApiDefineAndAddCommand(EXPORT_FILE, fileMenuId,
									"ExportM2RFile", SaveFileStr, "");
	F_ApiSetInt(FV_SessionId, exportFileCmdId, FP_EnabledWhen,
							 FV_ENABLE_NEEDS_DOCP_ONLY);
	F_ApiSetId(fileMenuId, exportFileCmdId, FP_PrevMenuItemInMenu, saveAsCmdId);
	F_ApiAddCommandToMenu(qFileMenuId, exportFileCmdId);
	F_ApiSetId(qFileMenuId, exportFileCmdId, FP_PrevMenuItemInMenu, saveAsCmdId);

	exportBookCmdId = F_ApiDefineAndAddCommand(EXPORT_BOOK, bookFileMenuId,
									"ExportM2RBook", SaveFileStr, "");
	F_ApiSetInt(FV_SessionId, exportBookCmdId, FP_EnabledWhen,
							 FV_ENABLE_NEEDS_BOOKP_ONLY);
	F_ApiSetId(bookFileMenuId, exportBookCmdId, FP_PrevMenuItemInMenu, saveAsCmdId);
	F_ApiAddCommandToMenu(qBookFileMenuId, exportBookCmdId);
	F_ApiSetId(qBookFileMenuId, exportBookCmdId, FP_PrevMenuItemInMenu, saveAsCmdId);

	exportSetupCmdId = F_ApiDefineAndAddCommand(EXPORT_SETUP, fileMenuId,
									"ExportM2RSetup", SetupFileStr, "");
	F_ApiSetInt(FV_SessionId, exportSetupCmdId, FP_EnabledWhen,
							 FV_ENABLE_NEEDS_DOCP_ONLY);
	F_ApiSetId(fileMenuId, exportSetupCmdId, FP_NextMenuItemInMenu, preferencesCmdId);

	exportBkSetupCmdId = F_ApiDefineAndAddCommand(EXPORT_BK_SETUP, bookFileMenuId,
									"ExportM2RBkSetup", SetupFileStr, "");
	F_ApiSetInt(FV_SessionId, exportBkSetupCmdId, FP_EnabledWhen,
							 FV_ENABLE_NEEDS_BOOKP_ONLY);
	F_ApiSetId(bookFileMenuId, exportBkSetupCmdId, FP_NextMenuItemInMenu, preferencesCmdId);
#endif

#if 0
	formatMenuId = F_ApiGetNamedObject(FV_SessionId, FO_Menu, "FormatMenu");
	headFootCmdId = F_ApiGetNamedObject(FV_SessionId, FO_Command, "HeadersAndFootersMenu");
	convDesCmdId = F_ApiDefineAndAddCommand(CONVERSION_DES, formatMenuId,
									"ConversionDesigner", ConvDesStr, "");
	F_ApiSetId(fileMenuId, convDesCmdId, FP_PrevMenuItemInMenu, headFootCmdId);
#endif

}

#if 0
/* designer command implementations */


VoidT GetDesigner(VoidT)
{
	F_ObjHandleT docId = 0;
	StringT filename = NULL;
	StringT ininame = NULL;

	docId = F_ApiGetId(0, FV_SessionId, FP_ActiveDoc);
	filename = F_ApiGetString(FV_SessionId, docId, FP_Name);

	CheckProjDialog(filename);
	if (ProjCancelled) { /* repeat this here to avoid goto */
		F_StrFree(filename);
		return;
	}

	SetStrings(ProjSelType >= 6);

	ininame = MakeFileName(CurrProjPath, StdIni);

	if (FileExists(ininame)) {
		if (ProjSelType >= 6)
			EditIniFile(ininame);
		else
			InitDesigner(ProjSelType, ininame, docId, False);
	}
	else {

	}

	F_StrFree(filename);
	F_StrFree(ininame);
}


VoidT GetBkDesigner(VoidT)
{
	F_ObjHandleT bookId = 0;
	StringT bookname = NULL;
	StringT ininame = NULL;

	bookId = F_ApiGetId(0, FV_SessionId, FP_ActiveBook);
	bookname = F_ApiGetString(FV_SessionId, bookId, FP_Name);

	EditProjDialog(bookname);
	if (ProjCancelled) { /* repeat this here to avoid goto */
		F_StrFree(bookname);
		return;
	}

	SetStrings(ProjSelType >= 6);

	ininame = MakeFileName(CurrProjPath, StdIni);

	if (!FileExists(ininame)) {
		if ((ProjSelType == 4)
		 || (ProjSelType == 5))
			SetupHelpIniDialog(bookname, bookId);
		else if (ProjSelType < 4)
			SetupWordIniDialog(bookname, bookId);
		else if (ProjSelType >= 6)
			SetupHTMLIniDialog(bookname, bookId);

		if (((ProjSelType < 4)
		  && !SetupWordCancelled)
		 || (((ProjSelType == 4)
		   || (ProjSelType == 5))
		  && !SetupHelpCancelled)
		 || ((ProjSelType >= 6)
		  && !SetupHTMLCancelled))
			WriteBasicIni(CurrProjPath, bookname, True, CurrProjType);
	}

	if (FileExists(ininame)) {
		if (ProjSelType >= 6)
			EditIniFile(ininame);
		else
			InitDesigner(ProjSelType, ininame, bookId, True);
	}

	F_StrFree(bookname);
	F_StrFree(ininame);
}

#endif

// end of m2gmenu.cpp

