/* m2rsetb.c is the FDK DocBook dialog handler
 * Copyright 2007 Omni Systems, Inc.  All Rights Reserved.
 */

#include "m2rbook.h"


/* defines for DocBook .ini setup dialog */
#define DLG_SB_PROJDESC  1

#define DLG_SB_CSS_CK   2
#define DLG_SB_CSSNAME  3
#define DLG_SB_CSSBTN   4
#define DLG_SB_CSSGEN_CK   5

#define DLG_SB_FM_TOC    7
#define DLG_SB_FM_IX     8
#define DLG_SB_FM_GEN    9

#define DLG_SB_CHENT_RAD  12
#define DLG_SB_CHFULL_RAD 13
#define DLG_SB_DEFEXT_BOX 15
#define DLG_SB_DEFROT_BOX 17
#define DLG_SB_CALS_CK    18
#define DLG_SB_DEFNAM_BOX 21
#define DLG_SB_DEFTTL_BOX 23

#define DLG_SB_TPL_CK   25

#define DLG_SB_HELP      DLG_SB_TPL_CK + DLG_TF_HELP
#define DLG_SB_OK        DLG_SB_TPL_CK + DLG_TF_OK
#define DLG_SB_CANCEL    DLG_SB_TPL_CK + DLG_TF_CANCEL

BoolT DocBookChapFull = False;
BoolT UseCALSTableModel = True;
StringT DocBookRoot = "book";
StringT DocBookName = "bookfile.xml";
StringT DocBookTitle = "Your Book Title";


/* setup DocBook project dialog */

VoidT SetupDocBookIniDialog(StringT filename, F_ObjHandleT bookId)
{
	F_ObjHandleT dlgId = 0;
	StringT basename = NULL;

	basename = BaseFileName(filename);

	/* open resource for HTML Settings dialog box. */
	dlgId = F_ApiOpenResource(FO_DialogResource, "m2rsetb");
	DlgIds[DLG_SETUP_DCBK_INI - 1] = dlgId;

	/* set CurrProjectName and Path at top of dialog */
	SetDlgProject(dlgId, DLG_SB_PROJDESC);

	/* set CSS variables */
	UseCSS = False;
	CssFileName = "local.css";
	SetDlgState(dlgId, DLG_SB_CSS_CK, UseCSS);
	SetDlgText(dlgId, DLG_SB_CSSNAME, CssFileName);
	SetDlgState(dlgId, DLG_SB_CSSGEN_CK, CreateCSS);
	EnableItems(dlgId, DLG_SB_CSSNAME, DLG_SB_CSSGEN_CK, False);

	/* set checkboxes for TOC and IX inclusion */
	UseFmTOC = UseFmIX = UseFmGenFiles = False;
	SetDlgState(dlgId, DLG_SB_FM_TOC, UseFmTOC);
	SetDlgState(dlgId, DLG_SB_FM_IX, UseFmIX);
	SetDlgState(dlgId, DLG_SB_FM_GEN, UseFmGenFiles);

	/* set defaults for text boxes */
	if (bookId) {
		DocBookChapFull = False;
		DocBookExt = ".ent";
		DocBookRoot = "book";
		DocBookName = AddFileExt(BaseFileName(filename), ".xml");
		DocBookTitle = "Your Book Title";
	}
	else {
		DocBookChapFull = True;
		DocBookExt = ".xml";
		DocBookRoot = "article";
		DocBookName = "";
		DocBookTitle = "";
	}
	SetDlgState(dlgId, DLG_SB_CHENT_RAD, !DocBookChapFull);	
	SetDlgState(dlgId, DLG_SB_CHFULL_RAD, DocBookChapFull);	
	SetDlgText(dlgId, DLG_SB_DEFEXT_BOX, DocBookExt);
	SetDlgText(dlgId, DLG_SB_DEFROT_BOX, DocBookRoot);
	SetDlgState(dlgId, DLG_SB_CALS_CK, UseCALSTableModel);	
	SetDlgText(dlgId, DLG_SB_DEFNAM_BOX, DocBookName);
	SetDlgText(dlgId, DLG_SB_DEFTTL_BOX, DocBookTitle);

	if (!bookId)
		EnableItems(dlgId, DLG_SB_DEFNAM_BOX - 2, DLG_SB_DEFTTL_BOX, False);

	/* turn off everything under Frame template */
	SetFrameTemplateDlg(dlgId, DLG_SB_TPL_CK);

	SetupCancelled = True;  /* default if closed */
	F_ApiModalDialog(DLG_SETUP_DCBK_INI, dlgId);

	WriteEqns = True;
	WriteAllGraphics = True;

	F_StrFree(basename);
}


VoidT ProcSetupDocBookIniDialogEvent(IntT itemNum)
{
	F_ObjHandleT dlgId = DlgIds[DLG_SETUP_DCBK_INI - 1];
	StringT newname = NULL;
	IntT err = 0;

	if ((itemNum >= DLG_SB_TPL_CK)
	 && (itemNum < DLG_SB_HELP))
		ProcFrameTemplateDlgEvent(dlgId, DLG_SB_TPL_CK, itemNum);

	switch (itemNum) {

		case DLG_SB_CSS_CK:
			if (GetDlgState(dlgId, itemNum)) {
				if (!UseCSS) { /* enable all items */
					EnableItems(dlgId, DLG_SB_CSSNAME, DLG_SB_CSSGEN_CK, True);
					UseCSS = True;
				}
			}
			else {
				if (UseCSS) { /* disable all items */
					EnableItems(dlgId, DLG_SB_CSSNAME, DLG_SB_CSSGEN_CK, False);
					UseCSS = False;
				}
			}
			break;

		case DLG_SB_CSSNAME:
			/* prohibit direct changes from typing in box */
			SetDlgText(dlgId, DLG_SB_CSSNAME, CssFileName);
			break;

		case DLG_SB_CSSBTN:
			M2GGetOpenFile(ChooseCSSFileStr, CurrProjPath, &newname);
			if (!err
			 && (newname != NULL)) {
				SetDlgText(dlgId, DLG_SB_CSSNAME, CssFileName = newname);
				newname = NULL;
			}
			break;

		case DLG_SB_CHENT_RAD:
			SetDlgText(dlgId, DLG_SB_DEFEXT_BOX, ".ent");
			SetDlgText(dlgId, DLG_SB_DEFROT_BOX, "book");
			EnableItems(dlgId, DLG_SB_DEFNAM_BOX - 2, DLG_SB_DEFTTL_BOX, True);
			break;

		case DLG_SB_CHFULL_RAD:
			SetDlgText(dlgId, DLG_SB_DEFEXT_BOX, ".xml");
			SetDlgText(dlgId, DLG_SB_DEFROT_BOX, "article");
			EnableItems(dlgId, DLG_SB_DEFNAM_BOX - 2, DLG_SB_DEFTTL_BOX, False);
			break;
		
		case DLG_SB_OK:
			CreateCSS = GetDlgState(dlgId, DLG_SB_CSSGEN_CK);

			UseFmTOC = GetDlgState(dlgId, DLG_SB_FM_TOC);
			UseFmIX = GetDlgState(dlgId, DLG_SB_FM_IX);
			UseFmGenFiles = GetDlgState(dlgId, DLG_SB_FM_GEN);

			DocBookExt = GetDlgText(dlgId, DLG_SB_DEFEXT_BOX);
			DocBookRoot = GetDlgText(dlgId, DLG_SB_DEFROT_BOX);
			UseCALSTableModel = GetDlgState(dlgId, DLG_SB_CALS_CK);

			DocBookChapFull = GetDlgState(dlgId, DLG_SB_CHFULL_RAD);
			if (DocBookChapFull == False) {
				DocBookName = GetDlgText(dlgId, DLG_SB_DEFNAM_BOX);
				DocBookTitle = GetDlgText(dlgId, DLG_SB_DEFTTL_BOX);
			}

			GetFrameTemplateDlg(dlgId, DLG_SB_TPL_CK);

			SetupCancelled = False;
			F_ApiClose(dlgId, 0);
			return;

		case DLG_SB_CANCEL:
			SetupCancelled = True;
			F_ApiClose(dlgId, 0);
			return;

		case DLG_SB_HELP:
			RunWinHlp(IDH_SetDocBook);
			break;

		default:
			break;
	}
	F_ApiReturnValue(FR_ModalStayUp);
}


VoidT WriteIniBVars(VoidT)
{
	StringT temp1 = NULL;

	WriteIniCommonVars(True, True);
	SetCSSIniOptions();

	XMLSuffix = DocBookExt;
	M2GIniPutSetting(CurrIniName, Setup, "XMLSuffix", XMLSuffix);

	M2GIniPutBool(CurrIniName, "Tables", "UseCALSModel", UseCALSTableModel);

	M2GIniPutSetting(CurrIniName, "DocBookOptions", "DocBookRoot", DocBookRoot);
	M2GIniPutBool(CurrIniName, "DocBookOptions", "WriteBookFile", DocBookChapFull);
	if (!DocBookChapFull) {
		M2GIniPutSetting(CurrIniName, "DocBookOptions", "BookFileName", temp1 = Fm2a(DocBookName));
		F_Free(temp1);
		M2GIniPutSetting(CurrIniName, "DocBookOptions", "BookFileTitle", temp1 = Fm2a(DocBookTitle));
		F_Free(temp1);
	}
}



/* end of m2rsetb.c */

