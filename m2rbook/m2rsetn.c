/* m2rsetn.c is the FDK HTML dialog handler for mif2rtf FM 5.5  
 * Last edited on 12/3/98 by Jeremy H. Griffith.
 * Copyright 1998 Omni Systems, Inc.  All Rights Reserved.
 */

#include "m2rbook.h"


/* defines for Word .ini setup dialog */
#define DLG_SN_PROJDESC  1

#define DLG_SN_CSS_CK   2
#define DLG_SN_CSSNAME  3
#define DLG_SN_CSSBTN   4
#define DLG_SN_CSSGEN_CK   5

#define DLG_SN_FM_TOC    7
#define DLG_SN_FM_IX     8
#define DLG_SN_FM_GEN    9

#define DLG_SN_TBLADAP_RAD 11
#define DLG_SN_TBLPCNT_RAD 12
#define DLG_SN_TBLFIXD_RAD 13
#define DLG_SN_TBLDPI_BOX 14

#define DLG_SN_TBLBRD_BOX 16
#define DLG_SN_TBLSPC_BOX 18
#define DLG_SN_TBLPAD_BOX 20

#define DLG_SN_TPL_CK   22

#define DLG_SN_HELP      DLG_SN_TPL_CK + DLG_TF_HELP
#define DLG_SN_OK        DLG_SN_TPL_CK + DLG_TF_OK
#define DLG_SN_CANCEL    DLG_SN_TPL_CK + DLG_TF_CANCEL



/* setup html project dialog */


VoidT SetupHTMLIniDialog(StringT filename, F_ObjHandleT bookId)
{
	F_ObjHandleT dlgId = 0;
	StringT basename = NULL;

	basename = BaseFileName(filename);

	/* open resource for HTML Settings dialog box. */
	dlgId = F_ApiOpenResource(FO_DialogResource, "m2rsetn");
	DlgIds[DLG_SETUP_HTML_INI - 1] = dlgId;

	/* set CurrProjectName and Path at top of dialog */
	SetDlgProject(dlgId, DLG_SN_PROJDESC);

	/* set CSS variables */
	UseCSS = True;
	CssFileName = "local.css";
	SetDlgState(dlgId, DLG_SN_CSS_CK, UseCSS);
	SetDlgText(dlgId, DLG_SN_CSSNAME, CssFileName);
	SetDlgState(dlgId, DLG_SN_CSSGEN_CK, CreateCSS);
	
	/* set checkboxes for TOC and IX inclusion */
	UseFmTOC = UseFmIX = UseFmGenFiles = True;
	SetDlgState(dlgId, DLG_SN_FM_TOC, UseFmTOC);
	SetDlgState(dlgId, DLG_SN_FM_IX, UseFmIX);
	SetDlgState(dlgId, DLG_SN_FM_GEN, UseFmGenFiles);

	/* set radio buttons for tables */
	SetDlgState(dlgId, DLG_SN_TBLADAP_RAD, TableSizing == 'A');	
	SetDlgState(dlgId, DLG_SN_TBLPCNT_RAD, TableSizing == 'P');	
	SetDlgState(dlgId, DLG_SN_TBLFIXD_RAD, TableSizing == 'F');
	
	SetDlgText(dlgId, DLG_SN_TBLDPI_BOX, TableDPI);
	EnableDlgItem(dlgId, DLG_SN_TBLDPI_BOX, (TableSizing == 'F'));

	SetDlgText(dlgId, DLG_SN_TBLBRD_BOX, TblBorder);
	SetDlgText(dlgId, DLG_SN_TBLSPC_BOX, TblSpacing);
	SetDlgText(dlgId, DLG_SN_TBLPAD_BOX, TblPadding);

	/* turn off everything under Frame template */
	SetFrameTemplateDlg(dlgId, DLG_SN_TPL_CK);

	SetupCancelled = True;  /* default if closed */
	F_ApiModalDialog(DLG_SETUP_HTML_INI, dlgId);

	WriteEqns = True;
	WriteAllGraphics = True;

	F_StrFree(basename);
}


VoidT ProcSetupHTMLIniDialogEvent(IntT itemNum)
{
	F_ObjHandleT dlgId = DlgIds[DLG_SETUP_HTML_INI - 1];
	StringT newname = NULL;
	IntT err = 0;

	if ((itemNum >= DLG_SN_TPL_CK)
	 && (itemNum < DLG_SN_HELP))
		ProcFrameTemplateDlgEvent(dlgId, DLG_SN_TPL_CK, itemNum);

	switch (itemNum) {

		case DLG_SN_CSS_CK:
			if (GetDlgState(dlgId, itemNum)) {
				if (!UseCSS) { /* enable all items */
					EnableItems(dlgId, DLG_SN_CSSNAME, DLG_SN_CSSGEN_CK, True);
					UseCSS = True;
				}
			}
			else {
				if (UseCSS) { /* disable all items */
					EnableItems(dlgId, DLG_SN_CSSNAME, DLG_SN_CSSGEN_CK, False);
					UseCSS = False;
				}
			}
			break;

		case DLG_SN_CSSNAME:
			/* prohibit direct changes from typing in box */
			SetDlgText(dlgId, DLG_SN_CSSNAME, CssFileName);
			break;

		case DLG_SN_CSSBTN:
#if 0
			err = F_ApiChooseFile(&newname, ChooseCSSFileStr,
								CurrProjPath, CssFileName, FV_ChooseSave, (StringT) "");
#else
			M2GGetOpenFile(ChooseCSSFileStr, CurrProjPath, &newname);
#endif
			if (!err
			 && (newname != NULL)) {
				SetDlgText(dlgId, DLG_SN_CSSNAME, CssFileName = newname);
				newname = NULL;
			}
			break;
		
		case DLG_SN_TBLFIXD_RAD:
			EnableDlgItem(dlgId, DLG_SN_TBLDPI_BOX, GetDlgState(dlgId, itemNum));
			break;


		case DLG_SN_OK:
			CreateCSS = GetDlgState(dlgId, DLG_SN_CSSGEN_CK);

			UseFmTOC = GetDlgState(dlgId, DLG_SN_FM_TOC);
			UseFmIX = GetDlgState(dlgId, DLG_SN_FM_IX);
			UseFmGenFiles = GetDlgState(dlgId, DLG_SN_FM_GEN);

			if (GetDlgState(dlgId, DLG_SN_TBLADAP_RAD))
				TableSizing = 'A';
			else if (GetDlgState(dlgId, DLG_SN_TBLPCNT_RAD))
				TableSizing = 'P';
			else
				TableSizing = 'F';

			if (TableSizing == 'F')
				TableDPI = GetDlgText(dlgId, DLG_SN_TBLDPI_BOX);

			TblBorder = GetDlgText(dlgId, DLG_SN_TBLBRD_BOX);
			TblSpacing = GetDlgText(dlgId, DLG_SN_TBLSPC_BOX);
			TblPadding = GetDlgText(dlgId, DLG_SN_TBLPAD_BOX);

			GetFrameTemplateDlg(dlgId, DLG_SN_TPL_CK);

			SetupCancelled = False;
			F_ApiClose(dlgId, 0);
			return;

		case DLG_SN_CANCEL:
			SetupCancelled = True;
			F_ApiClose(dlgId, 0);
			return;

		case DLG_SN_HELP:
			RunWinHlp(IDH_SetHTML);
			break;

		default:
			break;
	}
	F_ApiReturnValue(FR_ModalStayUp);
}


VoidT WriteIniNVars(VoidT)
{
	WriteIniCommonVars(True, True);
	SetCSSIniOptions();

	M2GIniPutSetting(CurrIniName, "Tables", "TableSizing",
		                            (TableSizing == 'A') ? "Adaptive" :
															 ((TableSizing == 'F') ? "Fixed" : "Percent"));
	if (TableSizing == 'F')
		M2GIniPutSetting(CurrIniName, "Tables", "TableDPI", TableDPI);

	M2GIniPutSetting(CurrIniName, "Tables", "Border", TblBorder);
	M2GIniPutSetting(CurrIniName, "Tables", "Spacing", TblSpacing);
	M2GIniPutSetting(CurrIniName, "Tables", "Padding", TblPadding);
}


/* end of m2rsetn.c */

