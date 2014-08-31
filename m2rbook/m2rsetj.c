/* m2rsetj.c is the FDK javahelp setup dialog handler, JHG  
 * Copyright 1998-2001 Omni Systems, Inc.  All Rights Reserved.
 */

#include "m2rbook.h"


/* defines for help .ini setup dialog */
#define DLG_SJ_PROJDESC  1

#define DLG_SJ_HELPTYPE  3

#define DLG_SJ_FM_TOC    4
#define DLG_SJ_FM_IX     5
#define DLG_SJ_FM_GEN    6

#define DLG_SJ_GEN_TOC   8
#define DLG_SJ_GEN_IX    9

#define DLG_SJ_USE_FTS  10

#define DLG_SJ_HSNAME   13
#define DLG_SJ_TITLE    15
#define DLG_SJ_CONTENTS 17
#define DLG_SJ_MAPPFX   19
#define DLG_SJ_OVERHS   20

#define DLG_SJ_TPL_CK   22

#define DLG_SJ_HELP     DLG_SJ_TPL_CK + DLG_TF_HELP
#define DLG_SJ_OK       DLG_SJ_TPL_CK + DLG_TF_OK
#define DLG_SJ_CANCEL   DLG_SJ_TPL_CK + DLG_TF_CANCEL


StringT HSName = NULL;



/* setup JH project dialog */


VoidT SetupJHIniDialog(StringT filename, F_ObjHandleT bookId)
{
	F_ObjHandleT dlgId = 0;
	F_ObjHandleT compId = 0;
	StringT basename = NULL;
	StringT compname = NULL;

	basename = BaseFileName(filename);

	/* open resource for JH Settings dialog box. */
	dlgId = F_ApiOpenResource(FO_DialogResource, "m2rsetj");
	DlgIds[DLG_SETUP_JAVH_INI - 1] = dlgId;

	/* set CurrProjectName and Path at top of dialog */
	SetDlgProject(dlgId, DLG_SJ_PROJDESC);

	SetDlgLabel(dlgId, DLG_SJ_HELPTYPE, (ProjSelType == PT_ORH) ?
																		 "OracleHelp" : "Sun JavaHelp");

	/* set up name for .hs */
	HPJName = AddFileExt(basename, ".hs");
	SetDlgText(dlgId, DLG_SJ_HSNAME, HPJName);

	/* set checkboxes for TOC and IX inclusion and generation */
	UseFmTOC = UseFmIX = False;
	UseFmGenFiles = True;
	GenTOC = GenIX = UseFTS = True;
	WrapAndShip = True;

	SetDlgState(dlgId, DLG_SJ_FM_TOC, UseFmTOC);
	SetDlgState(dlgId, DLG_SJ_FM_IX, UseFmIX);
	SetDlgState(dlgId, DLG_SJ_FM_GEN, UseFmGenFiles);

	SetDlgState(dlgId, DLG_SJ_GEN_TOC, GenTOC);
	SetDlgState(dlgId, DLG_SJ_GEN_IX, GenIX);
	SetDlgState(dlgId, DLG_SJ_USE_FTS, UseFTS);

	if (bookId) {  /* get basename of first file in book */
		compId = F_ApiGetId(FV_SessionId, bookId, FP_FirstComponentInBook);
		compname = F_ApiGetString(bookId, compId, FP_Name);
		DefaultTopicFile = BaseFileName(compname);
		F_StrFree(compname);
	}
	else
		DefaultTopicFile = F_StrCopyString(basename);

	SetDlgText(dlgId, DLG_SJ_CONTENTS, DefaultTopicFile);
	SetDlgText(dlgId, DLG_SJ_MAPPFX, MapFilePrefix);

	/* put dummy title */
	SetDlgText(dlgId, DLG_SJ_TITLE, HelpTitle);

	/* turn off everything under template and compiler */
	SetFrameTemplateDlg(dlgId, DLG_SJ_TPL_CK);

	SetDlgState(dlgId, DLG_SJ_OVERHS, False);

	SetupCancelled = True;  /* default if closed */
	F_ApiModalDialog(DLG_SETUP_JAVH_INI, dlgId);

	WriteEqns = True;
	WriteAllGraphics = True;

	F_StrFree(basename);
}


VoidT ProcSetupJHIniDialogEvent(IntT itemNum)
{
	F_ObjHandleT dlgId = DlgIds[DLG_SETUP_JAVH_INI - 1];
	StringT newname = NULL;
	IntT err = 0;

	if ((itemNum >= DLG_SJ_TPL_CK)
	 && (itemNum < DLG_SJ_HELP))
		ProcFrameTemplateDlgEvent(dlgId, DLG_SJ_TPL_CK, itemNum);

	else switch (itemNum) {

		case DLG_SJ_OK:
			GenTOC = GetDlgState(dlgId, DLG_SJ_GEN_TOC);
			GenIX = GetDlgState(dlgId, DLG_SJ_GEN_IX);

			UseFmTOC = GetDlgState(dlgId, DLG_SJ_FM_TOC);
			UseFmIX = GetDlgState(dlgId, DLG_SJ_FM_IX);
			UseFmGenFiles = GetDlgState(dlgId, DLG_SJ_FM_GEN);

			UseFTS = GetDlgState(dlgId, DLG_SJ_USE_FTS);

			GetFrameTemplateDlg(dlgId, DLG_SJ_TPL_CK);

			HPJName = GetDlgText(dlgId, DLG_SJ_HSNAME);

			DefaultTopicFile = GetDlgText(dlgId, DLG_SJ_CONTENTS);
			HelpTitle = GetDlgText(dlgId, DLG_SJ_TITLE);
			WriteHelpSetFile = GetDlgState(dlgId, DLG_SJ_OVERHS);
			MapFilePrefix = GetDlgText(dlgId, DLG_SJ_MAPPFX);

			SetupCancelled = False;
			F_ApiClose(dlgId, 0);
			return;

		case DLG_SJ_CANCEL:
			SetupCancelled = True;
			F_ApiClose(dlgId, 0);
			return;

		case DLG_SJ_HELP:
			RunWinHlp(IDH_SetJava);
			break;

		default:
			break;
	}
	F_ApiReturnValue(FR_ModalStayUp);
}


VoidT WriteIniJVars(StringT filename, BoolT book, UCharT form)
{
	StringT temp1 = NULL;
	StringT basename = NULL;
	StringT section = NULL;

	WriteIniCommonVars(True, True);
	section = (form == 'J') ? "JavaHelpOptions" : "OracleHelpOptions" ;
	if ((HPJName == NULL)
	 || (F_StrSubString(HPJName, ".hs") == -1)) {
		basename = Fm2a(temp1 = BaseFileName(filename));
		F_Free(temp1);
		HPJName = F_StrNew(F_StrLen(basename) + 4);
		F_StrCpy(HPJName, basename);
		F_StrCat(HPJName, ".hs");
		F_Free(basename);
	}
	M2GIniPutSetting(CurrIniName, section, "HSFileName", temp1 = Fm2a(HPJName));
	F_Free(temp1);
	if (FTSCommand) {
		M2GIniPutSetting(CurrIniName, section, "FTSCommand", temp1 = Fm2a(FTSCommand));
		F_Free(temp1);
	}
	M2GIniPutBool(CurrIniName, section, "WriteHelpSetFile", WriteHelpSetFile);
	M2GIniPutSetting(CurrIniName, section, "DefaultTopic", temp1 = Fm2a(DefaultTopicFile));
	F_Free(temp1);
	M2GIniPutSetting(CurrIniName, section, "HelpFileTitle", temp1 = Fm2a(HelpTitle));
	F_Free(temp1);
	M2GIniPutSetting(CurrIniName, section, "RefFileType",
			                              (!GenTOC && !GenIX) ? "None" :
			                              book ? "Body" : "Full");
	M2GIniPutSetting(CurrIniName, section, "ListType",
			                          (GenTOC && GenIX) ? "Both" :
			                          GenTOC ? "Contents" :
																GenIX  ? "Index" : "Both");
	if (MapFilePrefix && *MapFilePrefix)
		M2GIniPutSetting(CurrIniName, section, "MapFilePrefix", MapFilePrefix);
}


/* end of m2rsetj.c */

