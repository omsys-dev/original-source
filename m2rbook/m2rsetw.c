/* m2rsetw.c is the FDK Word dialog handler for mif2rtf FM 5.5  
 * Last edited on 12/1/98 by Jeremy H. Griffith.
 * Copyright 1998 Omni Systems, Inc.  All Rights Reserved.
 */

#include "m2rbook.h"


/* defines for Word .ini setup dialog */
#define DLG_SW_PROJDESC  1

#define DLG_SW_WTPL_CK   2
#define DLG_SW_WTPLNAME  3
#define DLG_SW_WTPLBTN   4
#define DLG_SW_TPLAUTO_CK   5

#define DLG_SW_FM_TOC    7
#define DLG_SW_FM_IX     8
#define DLG_SW_FM_GEN    9

#define DLG_SW_FRAMSIDE_RAD 11
#define DLG_SW_INDTSIDE_RAD 12
#define DLG_SW_LEFTSIDE_RAD 13

#define DLG_SW_FULLHF_RAD 15
#define DLG_SW_TEXTHF_RAD 16
#define DLG_SW_NOHF_RAD   17
#define DLG_SW_FRAMHF_CK  18

#define DLG_SW_TPL_CK   20

#define DLG_SW_HELP     DLG_SW_TPL_CK + DLG_TF_HELP
#define DLG_SW_OK       DLG_SW_TPL_CK + DLG_TF_OK
#define DLG_SW_CANCEL   DLG_SW_TPL_CK + DLG_TF_CANCEL



/* setup help project dialog */


VoidT SetupWordIniDialog(StringT filename, F_ObjHandleT bookId)
{
	F_ObjHandleT dlgId = 0;
	StringT basename = NULL;
	BoolT Word = True;

	basename = BaseFileName(filename);

	if (ProjSelType == 3)
		Word = False;

	/* open resource for Word Settings dialog box. */
	dlgId = F_ApiOpenResource(FO_DialogResource, "m2rsetw");
	DlgIds[DLG_SETUP_WRD_INI - 1] = dlgId;

	/* set CurrProjectName and Path at top of dialog */
	SetDlgProject(dlgId, DLG_SW_PROJDESC);

	/* turn off everything under Word template */
	UseWordTemplate = False;
	SetDlgState(dlgId, DLG_SW_WTPL_CK, UseWordTemplate);
	SetDlgText(dlgId, DLG_SW_WTPLNAME, WordTemplateName);
	SetDlgState(dlgId, DLG_SW_TPLAUTO_CK, WordTemplateAuto);
	EnableItems(dlgId, DLG_SW_WTPLNAME, DLG_SW_TPLAUTO_CK, False);

	/* set checkboxes for TOC and IX inclusion */
	UseFmTOC = UseFmIX = UseFmGenFiles = True;
	SetDlgState(dlgId, DLG_SW_FM_TOC, UseFmTOC);
	SetDlgState(dlgId, DLG_SW_FM_IX, UseFmIX);
	SetDlgState(dlgId, DLG_SW_FM_GEN, UseFmGenFiles);

	/* set radio buttons for sideheads and headers/footers */
	if (!Word
	 && (Sideheads == 'F'))
		Sideheads = 'I';
	SetDlgState(dlgId, DLG_SW_FRAMSIDE_RAD, Sideheads == 'F');	
	EnableDlgItem(dlgId, DLG_SW_FRAMSIDE_RAD, Word);	
	SetDlgState(dlgId, DLG_SW_INDTSIDE_RAD, Sideheads == 'I');	
	SetDlgState(dlgId, DLG_SW_LEFTSIDE_RAD, Sideheads == 'L');	

	if (!Word
	 && (Headfeet == 'S'))
		Headfeet = 'T';
	SetDlgState(dlgId, DLG_SW_FULLHF_RAD, Headfeet == 'S');	
	EnableDlgItem(dlgId, DLG_SW_FULLHF_RAD, Word);	
	SetDlgState(dlgId, DLG_SW_TEXTHF_RAD, Headfeet == 'T');
	SetDlgState(dlgId, DLG_SW_NOHF_RAD, Headfeet == 'N');

	if (!Word)
		FramedHF = False;	
	SetDlgState(dlgId, DLG_SW_FRAMHF_CK, FramedHF);

	/* turn off everything under Frame template */
	SetFrameTemplateDlg(dlgId, DLG_SW_TPL_CK);

	SetupCancelled = True;  /* default if closed */
	F_ApiModalDialog(DLG_SETUP_WRD_INI, dlgId);

	WriteEqns = True;
	WriteAllGraphics = False;

	F_StrFree(basename);
}


VoidT ProcSetupWordIniDialogEvent(IntT itemNum)
{
	F_ObjHandleT dlgId = DlgIds[DLG_SETUP_WRD_INI - 1];
	StringT newname = NULL;
	IntT i = 0;
	IntT err = 0;

	if ((itemNum >= DLG_SW_TPL_CK)
	 && (itemNum < DLG_SW_HELP))
		ProcFrameTemplateDlgEvent(dlgId, DLG_SW_TPL_CK, itemNum);

	else switch (itemNum) {

		case DLG_SW_WTPL_CK:
			if (GetDlgState(dlgId, itemNum)) {
				if (!UseWordTemplate) { /* enable all items */
					EnableItems(dlgId, DLG_SW_WTPLNAME, DLG_SW_TPLAUTO_CK, True);
					UseWordTemplate = True;
				}
			}
			else {
				if (UseWordTemplate) { /* disable all items */
					EnableItems(dlgId, DLG_SW_WTPLNAME, DLG_SW_TPLAUTO_CK, False);
					UseWordTemplate = False;
				}
			}
			break;

		case DLG_SW_WTPLNAME:
			/* prohibit direct changes from typing in box */
			SetDlgText(dlgId, DLG_SW_WTPLNAME, WordTemplateName);
			break;

		case DLG_SW_WTPLBTN:
#if 0
			err = F_ApiChooseFile(&newname, ChooseWordTemplateFileStr,
								CurrProjPath, WordTemplateName, FV_ChooseSelect, (StringT) "");
#else
			M2GGetOpenFile(ChooseWordTemplateFileStr, CurrProjPath, &newname);
#endif
			if (!err
			 && (newname != NULL)) {
				SetDlgText(dlgId, DLG_SW_WTPLNAME, WordTemplateName = newname);
				newname = NULL;
			}
			break;

		case DLG_SW_OK:
			if (UseWordTemplate) {
				WordTemplateName = GetDlgText(dlgId, DLG_SW_WTPLNAME);
				WordTemplateAuto = GetDlgState(dlgId, DLG_SW_TPLAUTO_CK);
			}

			UseFmTOC = GetDlgState(dlgId, DLG_SW_FM_TOC);
			UseFmIX = GetDlgState(dlgId, DLG_SW_FM_IX);
			UseFmGenFiles = GetDlgState(dlgId, DLG_SW_FM_GEN);

			if (GetDlgState(dlgId, DLG_SW_FRAMSIDE_RAD))
				Sideheads = 'F';
			else if (GetDlgState(dlgId, DLG_SW_INDTSIDE_RAD))
				Sideheads = 'I';
			else
				Sideheads = 'L';

			if (GetDlgState(dlgId, DLG_SW_FULLHF_RAD))
				Headfeet = 'S';
			else if (GetDlgState(dlgId, DLG_SW_TEXTHF_RAD))
				Headfeet = 'T';
			else
				Headfeet = 'N';

			FramedHF = GetDlgState(dlgId, DLG_SW_FRAMHF_CK);

			GetFrameTemplateDlg(dlgId, DLG_SW_TPL_CK);

			SetupCancelled = False;
			F_ApiClose(dlgId, 0);
			return;

		case DLG_SW_CANCEL:
			SetupCancelled = True;
			F_ApiClose(dlgId, 0);
			return;

		case DLG_SW_HELP:
			RunWinHlp(IDH_SetPRTF);
			break;

		default:
			break;
	}
	F_ApiReturnValue(FR_ModalStayUp);
}


VoidT WriteIniWVars(VoidT)
{
	StringT temp1 = NULL;

	WriteIniCommonVars(True, True);
	if (UseWordTemplate) {
		M2GIniPutSetting(CurrIniName, "WordOptions", "Template", temp1 = Fm2a(WordTemplateName));
		F_Free(temp1);
		M2GIniPutSetting(CurrIniName, "WordOptions", "TemplateAutoUpdate", WordTemplateAuto ? "Yes" : "No");
	}

	M2GIniPutSetting(CurrIniName, "WordOptions", "HeadFoot", 
		                             (Headfeet == 'S') ? "Standard" :
																((Headfeet == 'T') ? "Text" : "None"));
	M2GIniPutBool(CurrIniName, "WordOptions", "HFFramed", FramedHF);

	M2GIniPutSetting(CurrIniName, "WordOptions", "Sideheads",
		                              (Sideheads == 'F') ? "Frame" :
																 ((Sideheads == 'I') ? "Indent" : "Left"));
}


/* end of m2rsetw.c */

