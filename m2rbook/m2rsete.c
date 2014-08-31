/* m2rsete.c is the FDK EclipseHelp setup dialog handler, JHG  
 * Copyright 1998-2006 Omni Systems, Inc.  All Rights Reserved.
 */

#include "m2rbook.h"


/* defines for help .ini setup dialog */
#define DLG_SE_PROJDESC  1

#define DLG_SE_FM_TOC    2
#define DLG_SE_FM_IX     3
#define DLG_SE_FM_GEN    4

#define DLG_SE_GEN_IX    6
#define DLG_SE_GEN_CSH   7

#define DLG_SE_PLNAME   10
#define DLG_SE_PLID     12
#define DLG_SE_PROVID   14
#define DLG_SE_FIRST    16
#define DLG_SE_OVERPL   17
#define DLG_SE_OVERCX   18

#define DLG_SE_TPL_CK   20

#define DLG_SE_HELP     DLG_SE_TPL_CK + DLG_TF_HELP
#define DLG_SE_OK       DLG_SE_TPL_CK + DLG_TF_OK
#define DLG_SE_CANCEL   DLG_SE_TPL_CK + DLG_TF_CANCEL


StringT PluginName = "Mif2Go Eclipse Help";
StringT PluginID = "com.mif2go.help";
StringT Provider = "mif2go.com";

BoolT WritePluginFile = True;
BoolT WriteContextFile = True;
BoolT GenCSH = True;


/* setup EH project dialog */

VoidT SetupEHIniDialog(StringT filename, F_ObjHandleT bookId)
{
	F_ObjHandleT dlgId = 0;
	F_ObjHandleT compId = 0;
	StringT basename = NULL;
	StringT compname = NULL;

	basename = BaseFileName(filename);

	/* open resource for JH Settings dialog box. */
	dlgId = F_ApiOpenResource(FO_DialogResource, "m2rsete");
	DlgIds[DLG_SETUP_EH_INI - 1] = dlgId;

	/* set CurrProjectName and Path at top of dialog */
	SetDlgProject(dlgId, DLG_SE_PROJDESC);

	/* set up name for plugin */
	PluginName = F_StrCopyString(basename);
	SetDlgText(dlgId, DLG_SE_PLNAME, PluginName);
	SetDlgText(dlgId, DLG_SE_PLID, PluginID);
	SetDlgText(dlgId, DLG_SE_PROVID, Provider);

	/* set checkboxes for TOC and IX inclusion and generation */
	UseFmTOC = UseFmIX = False;
	UseFmGenFiles = True;
	GenCSH = True;
	GenIX = False;
	WrapAndShip = True;

	SetDlgState(dlgId, DLG_SE_FM_TOC, UseFmTOC);
	SetDlgState(dlgId, DLG_SE_FM_IX, UseFmIX);
	SetDlgState(dlgId, DLG_SE_FM_GEN, UseFmGenFiles);

	SetDlgState(dlgId, DLG_SE_GEN_IX, GenIX);
	SetDlgState(dlgId, DLG_SE_GEN_CSH, GenCSH);

	if (bookId) {  /* get basename of first file in book */
		compId = F_ApiGetId(FV_SessionId, bookId, FP_FirstComponentInBook);
		compname = F_ApiGetString(bookId, compId, FP_Name);
		DefaultTopicFile = AddFileExt(BaseFileName(compname), ".htm");
		F_StrFree(compname);
	}
	else
		DefaultTopicFile = AddFileExt(basename, ".htm");

	SetDlgText(dlgId, DLG_SE_FIRST, DefaultTopicFile);

	/* turn off everything under template and compiler */
	SetFrameTemplateDlg(dlgId, DLG_SE_TPL_CK);

	SetDlgState(dlgId, DLG_SE_OVERPL, False);
	SetDlgState(dlgId, DLG_SE_OVERCX, True);

	SetupCancelled = True;  /* default if closed */
	F_ApiModalDialog(DLG_SETUP_EH_INI, dlgId);

	WriteEqns = True;
	WriteAllGraphics = True;

	F_StrFree(basename);
}


VoidT ProcSetupEHIniDialogEvent(IntT itemNum)
{
	F_ObjHandleT dlgId = DlgIds[DLG_SETUP_EH_INI - 1];
	StringT newname = NULL;
	IntT err = 0;

	if ((itemNum >= DLG_SE_TPL_CK)
	 && (itemNum < DLG_SE_HELP))
		ProcFrameTemplateDlgEvent(dlgId, DLG_SE_TPL_CK, itemNum);

	else switch (itemNum) {

		case DLG_SE_OK:
			GenCSH = GetDlgState(dlgId, DLG_SE_GEN_CSH);
			GenIX = GetDlgState(dlgId, DLG_SE_GEN_IX);

			UseFmTOC = GetDlgState(dlgId, DLG_SE_FM_TOC);
			UseFmIX = GetDlgState(dlgId, DLG_SE_FM_IX);
			UseFmGenFiles = GetDlgState(dlgId, DLG_SE_FM_GEN);

			GetFrameTemplateDlg(dlgId, DLG_SE_TPL_CK);

			PluginName = GetDlgText(dlgId, DLG_SE_PLNAME);
			PluginID = GetDlgText(dlgId, DLG_SE_PLID);
			Provider = GetDlgText(dlgId, DLG_SE_PROVID);

			DefaultTopicFile = GetDlgText(dlgId, DLG_SE_FIRST);
			WritePluginFile = GetDlgState(dlgId, DLG_SE_OVERPL);
			WriteContextFile = GetDlgState(dlgId, DLG_SE_OVERCX);

			SetupCancelled = False;
			F_ApiClose(dlgId, 0);
			return;

		case DLG_SE_CANCEL:
			SetupCancelled = True;
			F_ApiClose(dlgId, 0);
			return;

		case DLG_SE_HELP:
			RunWinHlp(IDH_SetEclipse);
			break;

		default:
			break;
	}
	F_ApiReturnValue(FR_ModalStayUp);
}


VoidT WriteIniEVars(BoolT book)
{
	StringT temp1 = NULL;

	WriteIniCommonVars(True, True);
	M2GIniPutSetting(CurrIniName, "EclipseHelpOptions", "PluginName", temp1 = Fm2a(PluginName));
		F_Free(temp1);
	M2GIniPutSetting(CurrIniName, "EclipseHelpOptions", "PluginID", temp1 = Fm2a(PluginID));
		F_Free(temp1);
	M2GIniPutSetting(CurrIniName, "EclipseHelpOptions", "PluginProvider", temp1 = Fm2a(Provider));
		F_Free(temp1);
	M2GIniPutBool(CurrIniName, "EclipseHelpOptions", "WritePlugin", WritePluginFile);
	M2GIniPutSetting(CurrIniName, "EclipseHelpOptions", "TocTopic", temp1 = Fm2a(DefaultTopicFile));
		F_Free(temp1);
	M2GIniPutBool(CurrIniName, "EclipseHelpOptions", "UseIndex", GenIX);
	M2GIniPutBool(CurrIniName, "EclipseHelpOptions", "UseContext", GenCSH);
	M2GIniPutBool(CurrIniName, "EclipseHelpOptions", "WriteContext", WriteContextFile);
	M2GIniPutSetting(CurrIniName, "EclipseHelpOptions", "RefFileType", book ? "Body" : "Full");
	M2GIniPutSetting(CurrIniName, "EclipseHelpOptions", "ListType", GenIX ? "Both" : "Contents" );
}


/* end of m2rsete.c */

