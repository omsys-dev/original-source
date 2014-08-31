/* m2rsetm.c is the FDK MS HTML help setup dialog handler, JHG  
 * Copyright 1998-2001 Omni Systems, Inc.  All Rights Reserved.
 */

#include "m2rbook.h"


/* defines for help .ini setup dialog */
#define DLG_SM_PROJDESC  1

#define DLG_SM_FM_TOC    2
#define DLG_SM_FM_IX     3
#define DLG_SM_FM_GEN    4

#define DLG_SM_GEN_TOC   5
#define DLG_SM_GEN_IX    6

#define DLG_SM_USE_FTS   7
#define DLG_SM_ALIAS     8
#define DLG_SM_COMPILE  10

#define DLG_SM_HHPNAME  12
#define DLG_SM_TITLE    14
#define DLG_SM_CONTENTS 16
#define DLG_SM_CSHMAP   18
#define DLG_SM_OVERHHP  19

#define DLG_SM_TPL_CK   21

#define DLG_SM_HELP     DLG_SM_TPL_CK + DLG_TF_HELP
#define DLG_SM_OK       DLG_SM_TPL_CK + DLG_TF_OK
#define DLG_SM_CANCEL   DLG_SM_TPL_CK + DLG_TF_CANCEL


BoolT CshMapFileSet = False;


/* setup HH project dialog */


VoidT SetupHHIniDialog(StringT filename, F_ObjHandleT bookId)
{
	F_ObjHandleT dlgId = 0;
	F_ObjHandleT compId = 0;
	StringT basename = NULL;
	StringT compname = NULL;

	basename = BaseFileName(filename);

	/* open resource for HH Settings dialog box. */
	dlgId = F_ApiOpenResource(FO_DialogResource, "m2rsetm");
	DlgIds[DLG_SETUP_MSHH_INI - 1] = dlgId;

	/* set CurrProjectName and Path at top of dialog */
	SetDlgProject(dlgId, DLG_SM_PROJDESC);

	/* set up name for .hhp */
	HPJName = AddFileExt(basename, ".hhp");
	SetDlgText(dlgId, DLG_SM_HHPNAME, HPJName);

	/* set up name for [MAP] file */
	CshMapFileSet = False;
	CshMapFile = NULL;
	SetDlgText(dlgId, DLG_SM_CSHMAP, "resource.h");

	/* set checkboxes for TOC and IX inclusion and generation */
	UseFmTOC = UseFmIX = False;
	UseFmGenFiles = True;
	GenTOC = GenIX = UseFTS = UseAlias = True;

	SetDlgState(dlgId, DLG_SM_FM_TOC, UseFmTOC);
	SetDlgState(dlgId, DLG_SM_FM_IX, UseFmIX);
	SetDlgState(dlgId, DLG_SM_FM_GEN, UseFmGenFiles);

	SetDlgState(dlgId, DLG_SM_GEN_TOC, GenTOC);
	SetDlgState(dlgId, DLG_SM_GEN_IX, GenIX);
	SetDlgState(dlgId, DLG_SM_USE_FTS, UseFTS);
	SetDlgState(dlgId, DLG_SM_ALIAS, UseAlias);
	SetDlgState(dlgId, DLG_SM_COMPILE, MakeHLP);

	if (bookId) {  /* get basename of first file in book */
		compId = F_ApiGetId(FV_SessionId, bookId, FP_FirstComponentInBook);
		compname = F_ApiGetString(bookId, compId, FP_Name);
		DefaultTopicFile = BaseFileName(compname);
		F_StrFree(compname);
	}
	else
		DefaultTopicFile = F_StrCopyString(basename);
	SetDlgText(dlgId, DLG_SM_CONTENTS, DefaultTopicFile);

	/* put dummy title */
	SetDlgText(dlgId, DLG_SM_TITLE, HelpTitle);

	/* turn off everything under template and compiler */
	SetFrameTemplateDlg(dlgId, DLG_SM_TPL_CK);

	SetDlgState(dlgId, DLG_SM_OVERHHP, False);

	SetupCancelled = True;  /* default if closed */
	F_ApiModalDialog(DLG_SETUP_MSHH_INI, dlgId);

	WriteEqns = True;
	WriteAllGraphics = True;

	F_StrFree(basename);
}


VoidT ProcSetupHHIniDialogEvent(IntT itemNum)
{
	F_ObjHandleT dlgId = DlgIds[DLG_SETUP_MSHH_INI - 1];
	StringT newname = NULL;
	IntT err = 0;

	if ((itemNum >= DLG_SM_TPL_CK)
	 && (itemNum < DLG_SM_HELP))
		ProcFrameTemplateDlgEvent(dlgId, DLG_SM_TPL_CK, itemNum);

	else switch (itemNum) {

		case DLG_SM_CSHMAP:
			CshMapFileSet = True;
			break;

		case DLG_SM_OK:
			GenTOC = GetDlgState(dlgId, DLG_SM_GEN_TOC);
			GenIX = GetDlgState(dlgId, DLG_SM_GEN_IX);

			UseFmTOC = GetDlgState(dlgId, DLG_SM_FM_TOC);
			UseFmIX = GetDlgState(dlgId, DLG_SM_FM_IX);
			UseFmGenFiles = GetDlgState(dlgId, DLG_SM_FM_GEN);

			UseFTS = GetDlgState(dlgId, DLG_SM_USE_FTS);
			UseAlias = GetDlgState(dlgId, DLG_SM_ALIAS);
			MakeHLP = GetDlgState(dlgId, DLG_SM_COMPILE);

			GetFrameTemplateDlg(dlgId, DLG_SM_TPL_CK);

			HPJName = GetDlgText(dlgId, DLG_SM_HHPNAME);

			DefaultTopicFile = GetDlgText(dlgId, DLG_SM_CONTENTS);
			HelpTitle = GetDlgText(dlgId, DLG_SM_TITLE);
			WriteHelpProjectFile = GetDlgState(dlgId, DLG_SM_OVERHHP);

			if (CshMapFileSet)
				CshMapFile = GetDlgText(dlgId, DLG_SM_CSHMAP);

			SetupCancelled = False;
			F_ApiClose(dlgId, 0);
			return;

		case DLG_SM_CANCEL:
			SetupCancelled = True;
			F_ApiClose(dlgId, 0);
			return;

		case DLG_SM_HELP:
			RunWinHlp(IDH_SetHHelp);
			break;

		default:
			break;
	}
	F_ApiReturnValue(FR_ModalStayUp);
}


VoidT WriteIniMVars(StringT filename, BoolT book)
{
	StringT temp1 = NULL;
	StringT basename = NULL;

	WriteIniCommonVars(True, True);
	M2GIniPutBool(CurrIniName, "Automation", "CompileHelp", MakeHLP);
	M2GIniPutSetting(CurrIniName, "MSHtmlHelpOptions", "HHPFileName", temp1 = Fm2a(HPJName));
		F_Free(temp1);
	M2GIniPutBool(CurrIniName, "MSHtmlHelpOptions", "WriteHelpProjectFile", WriteHelpProjectFile);
	M2GIniPutSetting(CurrIniName, "MSHtmlHelpOptions", "HelpFileTitle", temp1 = Fm2a(HelpTitle));
		F_Free(temp1);

	basename = Fm2a(temp1 = BaseFileName(filename));
	F_Free(temp1);
	DefaultChmFile = F_Alloc(F_StrLen(basename) + 5, NO_DSE);
	F_StrCpy(DefaultChmFile, basename);
	F_StrCat(DefaultChmFile, ".chm");
	F_Free(basename);
	M2GIniPutSetting(CurrIniName, "MSHtmlHelpOptions", "DefaultChmFile", DefaultChmFile);

	M2GIniPutSetting(CurrIniName, "MSHtmlHelpOptions", "DefaultTopicFile", temp1 = Fm2a(DefaultTopicFile));
		F_Free(temp1);
	M2GIniPutBool(CurrIniName, "MSHtmlHelpOptions", "UseFTS", UseFTS);
	M2GIniPutSetting(CurrIniName, "MSHtmlHelpOptions", "RefFileType",
		                                  (!GenTOC && !GenIX) ? "None" :
			                                book ? "Body" : "Full");
	M2GIniPutSetting(CurrIniName, "MSHtmlHelpOptions", "ListType",
		                              (GenTOC && GenIX) ? "Both" :
			                            GenTOC ? "Contents" :
																	GenIX  ? "Index" : "Both");
	if (CshMapFile) {
		M2GIniPutSetting(CurrIniName, "MSHtmlHelpOptions", "CshMapFile", temp1 = Fm2a(CshMapFile));
		F_Free(temp1);
	}
	M2GIniPutBool(CurrIniName, "MSHtmlHelpOptions", "MakeAliasFile", UseAlias);
}


/* end of m2rsetm.c */

