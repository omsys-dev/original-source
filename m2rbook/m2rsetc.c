/* m2rsetm.c is the FDK MS HTML help setup dialog handler, JHG  
 * Copyright 1998-2001 Omni Systems, Inc.  All Rights Reserved.
 */

#include "m2rbook.h"


/* defines for help .ini setup dialog */
#define DLG_SC_PROJDESC  1

#define DLG_SC_FM_TOC    2
#define DLG_SC_FM_IX     3
#define DLG_SC_FM_GEN    4

#define DLG_SC_USE_TOC   6
#define DLG_SC_USE_IX    7
#define DLG_SC_USE_FTS   8
#define DLG_SC_USE_REL   9

#if 0
#define DLG_SC_COPY_TO  10
#define DLG_SC_HP_PATH  11
#define DLG_SC_VW_PATH  13
#endif

#define DLG_SC_PROJECT  12
#define DLG_SC_TITLE    14
#define DLG_SC_CONTENTS 16
#define DLG_SC_CSS      18
//#define DLG_SC_CSHMAP   20

#define DLG_SC_TPL_CK   21

#define DLG_SC_HELP     DLG_SC_TPL_CK + DLG_TF_HELP
#define DLG_SC_OK       DLG_SC_TPL_CK + DLG_TF_OK
#define DLG_SC_CANCEL   DLG_SC_TPL_CK + DLG_TF_CANCEL

BoolT TopicFileSet = False;


/* setup OMH project dialog */


VoidT SetupOMHIniDialog(StringT filename, F_ObjHandleT bookId)
{
	F_ObjHandleT dlgId = 0;
	F_ObjHandleT compId = 0;
	StringT basename = NULL;
	StringT compname = NULL;
	StringT topicfile = NULL;

	basename = BaseFileName(filename);

	/* open resource for OMH Settings dialog box. */
	dlgId = F_ApiOpenResource(FO_DialogResource, "m2rsetc");
	DlgIds[DLG_SETUP_OMH_INI - 1] = dlgId;

	/* set CurrProjectName and Path at top of dialog */
	SetDlgProject(dlgId, DLG_SC_PROJDESC);

	/* set up name for [MAP] file */
	CshMapFileSet = False;
	CshMapFile = NULL;
	//SetDlgText(dlgId, DLG_SC_CSHMAP, "resource.h");

	/* set checkboxes for TOC and IX inclusion and search generation */
	UseFmTOC = UseFmIX = False;
	UseFmGenFiles = True;
	UseOHTOC = UseOHIX = UseOHFTS = UseOHREL = True;

	/* these are always the same */
	GenTOC = GenIX = True;
	WrapAndShip = True;

	SetDlgState(dlgId, DLG_SC_FM_TOC, UseFmTOC);
	SetDlgState(dlgId, DLG_SC_FM_IX, UseFmIX);
	SetDlgState(dlgId, DLG_SC_FM_GEN, UseFmGenFiles);

	SetDlgState(dlgId, DLG_SC_USE_TOC, UseOHTOC);
	SetDlgState(dlgId, DLG_SC_USE_IX, UseOHIX);
	SetDlgState(dlgId, DLG_SC_USE_FTS, UseOHFTS);
	SetDlgState(dlgId, DLG_SC_USE_REL, UseOHREL);

#if 0
	SetDlgState(dlgId, DLG_SC_COPY_TO, WrapAndShip);
	SetDlgText(dlgId, DLG_SC_HP_PATH, WrapPath);
	SetDlgText(dlgId, DLG_SC_VW_PATH, OHVName);
#endif

	if (bookId) {  /* get basename of first file in book */
		compId = F_ApiGetId(FV_SessionId, bookId, FP_FirstComponentInBook);
		compname = F_ApiGetString(bookId, compId, FP_Name);
		topicfile = BaseFileName(compname);
		F_StrFree(compname);
	}
	else
		topicfile = F_StrCopyString(basename);
	SetDlgText(dlgId, DLG_SC_CONTENTS, topicfile);
	DefaultTopicFile = NULL;
	TopicFileSet = False;

	SetDlgText(dlgId, DLG_SC_PROJECT, basename);

	/* put dummy title */
	SetDlgText(dlgId, DLG_SC_TITLE, HelpTitle);

	UseCSS = True;
	CssFileName = "ohmain.css";
	SetDlgText(dlgId, DLG_SC_CSS, CssFileName);

	/* turn off everything under template */
	SetFrameTemplateDlg(dlgId, DLG_SC_TPL_CK);

	SetupCancelled = True;  /* default if closed */
	F_ApiModalDialog(DLG_SETUP_OMH_INI, dlgId);

	WriteEqns = True;
	WriteAllGraphics = True;

	F_StrFree(basename);
}


VoidT ProcSetupOMHIniDialogEvent(IntT itemNum)
{
	F_ObjHandleT dlgId = DlgIds[DLG_SETUP_OMH_INI - 1];
	StringT newname = NULL;
	IntT err = 0;

	if ((itemNum >= DLG_SC_TPL_CK)
	 && (itemNum < DLG_SC_HELP))
		ProcFrameTemplateDlgEvent(dlgId, DLG_SC_TPL_CK, itemNum);

	else switch (itemNum) {

		//case DLG_SC_CSHMAP:
		//	CshMapFileSet = True;
		//	break;

		case DLG_SC_CONTENTS:
			TopicFileSet = True;
			break;

		case DLG_SC_OK:
			UseFmTOC = GetDlgState(dlgId, DLG_SC_FM_TOC);
			UseFmIX = GetDlgState(dlgId, DLG_SC_FM_IX);
			UseFmGenFiles = GetDlgState(dlgId, DLG_SC_FM_GEN);

			UseOHTOC = GetDlgState(dlgId, DLG_SC_USE_TOC);
			UseOHIX = GetDlgState(dlgId, DLG_SC_USE_IX);
			UseOHFTS = GetDlgState(dlgId, DLG_SC_USE_FTS);
			UseOHREL = GetDlgState(dlgId, DLG_SC_USE_REL);

#if 0
			WrapAndShip = GetDlgState(dlgId, DLG_SC_COPY_TO);
			WrapPath = GetDlgText(dlgId, DLG_SC_HP_PATH);
			OHVName = GetDlgText(dlgId, DLG_SC_VW_PATH);
#endif

			GetFrameTemplateDlg(dlgId, DLG_SC_TPL_CK);

			OHProjName = GetDlgText(dlgId, DLG_SC_PROJECT);
			HelpTitle = GetDlgText(dlgId, DLG_SC_TITLE);
			CssFileName = GetDlgText(dlgId, DLG_SC_CSS);

			//if (TopicFileSet)
				DefaultTopicFile = GetDlgText(dlgId, DLG_SC_CONTENTS);
			//if (CshMapFileSet)
			//	CshMapFile = GetDlgText(dlgId, DLG_SC_CSHMAP);

			SetupCancelled = False;
			F_ApiClose(dlgId, 0);
			return;

		case DLG_SC_CANCEL:
			SetupCancelled = True;
			F_ApiClose(dlgId, 0);
			return;

		case DLG_SC_HELP:
			RunWinHlp(IDH_SetOMHelp);
			break;

		default:
			break;
	}
	F_ApiReturnValue(FR_ModalStayUp);
}


VoidT WriteIniCVars(BoolT book)
{
	StringT temp1 = NULL;

	WriteIniCommonVars(True, True);
	M2GIniPutSetting(CurrIniName, "OmniHelpOptions", "ProjectName", temp1 = Fm2a(OHProjName));
		F_Free(temp1);
	M2GIniPutSetting(CurrIniName, "OmniHelpOptions", "HelpFileTitle", temp1 = Fm2a(HelpTitle));
		F_Free(temp1);
	M2GIniPutSetting(CurrIniName, "OmniHelpOptions", "DefaultTopic", DefaultTopicFile ? DefaultTopicFile : "");
	if (UseOHTOC || UseOHIX || UseOHFTS || UseOHREL) {
		temp1 = F_StrNew(17);
		if (UseOHTOC)
			F_StrCat(temp1, " Toc");
		if (UseOHIX)
			F_StrCat(temp1, " Idx");
		if (UseOHFTS)
			F_StrCat(temp1, " Fts");
		if (UseOHREL)
			F_StrCat(temp1, " Rel");
		M2GIniPutSetting(CurrIniName, "OmniHelpOptions", "NavElems", temp1);
		F_Free(temp1);
	}
	M2GIniPutSetting(CurrIniName, "OmniHelpOptions", "MainCssName",temp1 = Fm2a(CssFileName));
	M2GIniPutSetting(CurrIniName, "OmniHelpOptions", "IECssName", temp1);
	M2GIniPutSetting(CurrIniName, "OmniHelpOptions", "N6CssName", temp1);
	M2GIniPutSetting(CurrIniName, "OmniHelpOptions", "N4CssName", temp1);
	M2GIniPutSetting(CurrIniName, "CSS", "CssFileName", temp1);
		F_Free(temp1);
	M2GIniPutBool(CurrIniName, "OmniHelpOptions", "UseFTS", UseOHFTS);
	M2GIniPutSetting(CurrIniName, "OmniHelpOptions", "RefFileType", book ? "Body" : "Full");
}


/* end of m2rsetc.c */

