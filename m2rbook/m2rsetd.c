/* m2rsetd.c is the FDK DITA dialog handler
 * Copyright 2007 Omni Systems, Inc.  All Rights Reserved.
 */

#include "m2rbook.h"


/* defines for DITA .ini setup dialog */
#define DLG_SD_PROJDESC  1

#define DLG_SD_CSS_CK   2
#define DLG_SD_CSSNAME  3
#define DLG_SD_CSSBTN   4
#define DLG_SD_CSSGEN_CK   5

#define DLG_SD_FM_TOC    7
#define DLG_SD_FM_IX     8
#define DLG_SD_FM_GEN    9

#define DLG_SD_DEFTOP_BOX 13
#define DLG_SD_DEFPAR_BOX 15
#define DLG_SD_DEFCHR_BOX 17
#define DLG_SD_DEFV10_RAD 19
#define DLG_SD_DEFV11_RAD 20

#define DLG_SD_TPL_CK   22

#define DLG_SD_HELP      DLG_SD_TPL_CK + DLG_TF_HELP
#define DLG_SD_OK        DLG_SD_TPL_CK + DLG_TF_OK
#define DLG_SD_CANCEL    DLG_SD_TPL_CK + DLG_TF_CANCEL

StringT DITADefTopic = "concept";
StringT DITADefPara = "p";
StringT DITADefChar = "ph";
BoolT DITADefVer11 = True;


/* setup DITA project dialog */

VoidT SetupDITAIniDialog(StringT filename, F_ObjHandleT bookId)
{
	F_ObjHandleT dlgId = 0;
	StringT basename = NULL;

	basename = BaseFileName(filename);

	/* open resource for HTML Settings dialog box. */
	dlgId = F_ApiOpenResource(FO_DialogResource, "m2rsetd");
	DlgIds[DLG_SETUP_DITA_INI - 1] = dlgId;

	/* set CurrProjectName and Path at top of dialog */
	SetDlgProject(dlgId, DLG_SD_PROJDESC);

	/* set CSS variables */
	UseCSS = CreateCSS = False;
	CssFileName = "local.css";
	SetDlgState(dlgId, DLG_SD_CSS_CK, UseCSS);
	SetDlgText(dlgId, DLG_SD_CSSNAME, CssFileName);
	SetDlgState(dlgId, DLG_SD_CSSGEN_CK, CreateCSS);
	
	/* set checkboxes for TOC and IX inclusion */
	UseFmTOC = UseFmIX = UseFmGenFiles = False;
	SetDlgState(dlgId, DLG_SD_FM_TOC, UseFmTOC);
	SetDlgState(dlgId, DLG_SD_FM_IX, UseFmIX);
	SetDlgState(dlgId, DLG_SD_FM_GEN, UseFmGenFiles);

	/* set defaults for text boxes */
	SetDlgText(dlgId, DLG_SD_DEFTOP_BOX, DITADefTopic);
	SetDlgText(dlgId, DLG_SD_DEFPAR_BOX, DITADefPara);
	SetDlgText(dlgId, DLG_SD_DEFCHR_BOX, DITADefChar);
	SetDlgState(dlgId, DLG_SD_DEFV10_RAD, !DITADefVer11);	
	SetDlgState(dlgId, DLG_SD_DEFV11_RAD, DITADefVer11);	

	/* turn off everything under Frame template */
	SetFrameTemplateDlg(dlgId, DLG_SD_TPL_CK);

	SetupCancelled = True;  /* default if closed */
	F_ApiModalDialog(DLG_SETUP_DITA_INI, dlgId);

	WriteEqns = True;
	WriteAllGraphics = True;

	F_StrFree(basename);
}


VoidT ProcSetupDITAIniDialogEvent(IntT itemNum)
{
	F_ObjHandleT dlgId = DlgIds[DLG_SETUP_DITA_INI - 1];
	StringT newname = NULL;
	IntT err = 0;

	if ((itemNum >= DLG_SD_TPL_CK)
	 && (itemNum < DLG_SD_HELP))
		ProcFrameTemplateDlgEvent(dlgId, DLG_SD_TPL_CK, itemNum);

	switch (itemNum) {

		case DLG_SD_CSS_CK:
			if (GetDlgState(dlgId, itemNum)) {
				if (!UseCSS) { /* enable all items */
					EnableItems(dlgId, DLG_SD_CSSNAME, DLG_SD_CSSGEN_CK, True);
					UseCSS = CreateCSS = True;
				}
			}
			else {
				if (UseCSS) { /* disable all items */
					EnableItems(dlgId, DLG_SD_CSSNAME, DLG_SD_CSSGEN_CK, False);
					UseCSS = CreateCSS = False;
				}
			}
			break;

		case DLG_SD_CSSNAME:
			/* prohibit direct changes from typing in box */
			SetDlgText(dlgId, DLG_SD_CSSNAME, CssFileName);
			break;

		case DLG_SD_CSSBTN:
			M2GGetOpenFile(ChooseCSSFileStr, CurrProjPath, &newname);
			if (!err
			 && (newname != NULL)) {
				SetDlgText(dlgId, DLG_SD_CSSNAME, CssFileName = newname);
				newname = NULL;
			}
			break;
		
		case DLG_SD_OK:
			CreateCSS = GetDlgState(dlgId, DLG_SD_CSSGEN_CK);

			UseFmTOC = GetDlgState(dlgId, DLG_SD_FM_TOC);
			UseFmIX = GetDlgState(dlgId, DLG_SD_FM_IX);
			UseFmGenFiles = GetDlgState(dlgId, DLG_SD_FM_GEN);

			DITADefTopic = GetDlgText(dlgId, DLG_SD_DEFTOP_BOX);
			DITADefPara = GetDlgText(dlgId, DLG_SD_DEFPAR_BOX);
			DITADefChar = GetDlgText(dlgId, DLG_SD_DEFCHR_BOX);
			DITADefVer11 = GetDlgState(dlgId, DLG_SD_DEFV11_RAD);

			GetFrameTemplateDlg(dlgId, DLG_SD_TPL_CK);

			SetupCancelled = False;
			F_ApiClose(dlgId, 0);
			return;

		case DLG_SD_CANCEL:
			SetupCancelled = True;
			F_ApiClose(dlgId, 0);
			return;

		case DLG_SD_HELP:
			RunWinHlp(IDH_SetDITA);
			break;

		default:
			break;
	}
	F_ApiReturnValue(FR_ModalStayUp);
}


VoidT WriteIniDVars(VoidT)
{
	WriteIniCommonVars(True, True);
	SetCSSIniOptions();

	M2GIniPutSetting(CurrIniName, "DITAOptions", "DITAVer", DITADefVer11 ? "1" : "0");
	M2GIniPutSetting(CurrIniName, "DITAOptions", "DefTopic", DITADefTopic);
	M2GIniPutSetting(CurrIniName, "DITAOptions", "DefParaElem", DITADefPara);
	M2GIniPutSetting(CurrIniName, "DITAOptions", "DefCharElem", DITADefChar);
}

/* end of m2rsetd.c */

