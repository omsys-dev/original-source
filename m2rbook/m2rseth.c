/* m2rseth.c is the FDK help setup dialog handler, JHG  
 * Copyright 1998-2001 Omni Systems, Inc.  All Rights Reserved.
 */

#include "m2rbook.h"


/* defines for help .ini setup dialog */
#define DLG_SH_PROJDESC  1

#define DLG_SH_HLPNAME   3
#define DLG_SH_CNTNAME   5
#define DLG_SH_FM_TOC    6
#define DLG_SH_FM_IX     7
#define DLG_SH_FM_GEN    8

#define DLG_SH_CNT_CK    9

#define DLG_SH_HPJ_CK   11
//#define DLG_SH_HP_PATH  11
#define DLG_SH_HPJNAME  13
#define DLG_SH_TITLE    15
#define DLG_SH_CONTENTS 17
#define DLG_SH_COPYR    19
#define DLG_SH_DATECOPY 20

#define DLG_SH_TPL_CK   22

#define DLG_SH_HELP     DLG_SH_TPL_CK + DLG_TF_HELP
#define DLG_SH_OK       DLG_SH_TPL_CK + DLG_TF_OK
#define DLG_SH_CANCEL   DLG_SH_TPL_CK + DLG_TF_CANCEL



/* setup help project dialog */


VoidT SetupHelpIniDialog(StringT filename, F_ObjHandleT bookId)
{
	F_ObjHandleT dlgId = 0;
	F_ObjHandleT compId = 0;
	StringT basename = NULL;
	StringT compname = NULL;

	basename = BaseFileName(filename);

	/* open resource for Help Settings dialog box. */
	dlgId = F_ApiOpenResource(FO_DialogResource, "m2rseth");
	DlgIds[DLG_SETUP_HLP_INI - 1] = dlgId;

	/* set CurrProjectName and Path at top of dialog */
	SetDlgProject(dlgId, DLG_SH_PROJDESC);

	/* set up names for .hlp, .cnt, .hpj, and contents */
	HelpFileName = AddFileExt(basename, ".hlp");
	SetDlgText(dlgId, DLG_SH_HLPNAME, HelpFileName);

	CntFileName = AddFileExt(basename, ".cnt");
	SetDlgText(dlgId, DLG_SH_CNTNAME, CntFileName);

	HPJName = AddFileExt(basename, ".hpj");
	SetDlgText(dlgId, DLG_SH_HPJNAME, HPJName);

	/* set checkboxes for TOC and IX inclusion, and combined .cnt */
	UseFmTOC = UseFmIX = False;
	UseFmGenFiles = True;
	MakeCombCnt = bookId ? True : False;
	SetDlgState(dlgId, DLG_SH_FM_TOC, UseFmTOC);
	SetDlgState(dlgId, DLG_SH_FM_IX, UseFmIX);
	SetDlgState(dlgId, DLG_SH_FM_GEN, UseFmGenFiles);
	SetDlgState(dlgId, DLG_SH_CNT_CK, MakeCombCnt);
	//SetDlgText(dlgId, DLG_SH_HP_PATH, WrapPath);

	if (bookId) {  /* get basename of first file in book */
		compId = F_ApiGetId(FV_SessionId, bookId, FP_FirstComponentInBook);
		compname = F_ApiGetString(bookId, compId, FP_Name);
		HelpContentsTopic = BaseFileName(compname);
		F_StrFree(compname);
	}
	else
		HelpContentsTopic = F_StrCopyString(basename);
	SetDlgText(dlgId, DLG_SH_CONTENTS, HelpContentsTopic);

	/* put dummy title and copyright */
	SetDlgText(dlgId, DLG_SH_TITLE, HelpTitle);
	SetDlgText(dlgId, DLG_SH_COPYR, HelpCopyright);

	/* turn off everything under template and compiler */
	SetFrameTemplateDlg(dlgId, DLG_SH_TPL_CK);

	SetDlgState(dlgId, DLG_SH_HPJ_CK, MakeHLP);
	/* EnableItems(dlgId, DLG_SH_HPJ_CK + 1, DLG_SH_DATECOPY, False); */

	SetupCancelled = True;  /* default if closed */
	F_ApiModalDialog(DLG_SETUP_HLP_INI, dlgId);

	WriteEqns = True;
	WriteAllGraphics = True;

	F_StrFree(basename);
}


VoidT ProcSetupHelpIniDialogEvent(IntT itemNum)
{
	F_ObjHandleT dlgId = DlgIds[DLG_SETUP_HLP_INI - 1];
	StringT newname = NULL;
	IntT err = 0;

	if ((itemNum >= DLG_SH_TPL_CK)
	 && (itemNum < DLG_SH_HELP))
		ProcFrameTemplateDlgEvent(dlgId, DLG_SH_TPL_CK, itemNum);

	else switch (itemNum) {

		case DLG_SH_OK:
			HelpFileName = GetDlgText(dlgId, DLG_SH_HLPNAME);
			CntFileName = GetDlgText(dlgId, DLG_SH_CNTNAME);

			UseFmTOC = GetDlgState(dlgId, DLG_SH_FM_TOC);
			UseFmIX = GetDlgState(dlgId, DLG_SH_FM_IX);
			UseFmGenFiles = GetDlgState(dlgId, DLG_SH_FM_GEN);

			MakeHLP = GetDlgState(dlgId, DLG_SH_HPJ_CK);
			MakeCombCnt = GetDlgState(dlgId, DLG_SH_CNT_CK);

			//WrapPath = GetDlgText(dlgId, DLG_SH_HP_PATH);

			GetFrameTemplateDlg(dlgId, DLG_SH_TPL_CK);

			HPJName = GetDlgText(dlgId, DLG_SH_HPJNAME);
			HelpContentsTopic = GetDlgText(dlgId, DLG_SH_CONTENTS);
			HelpTitle = GetDlgText(dlgId, DLG_SH_TITLE);
			HelpCopyright = GetDlgText(dlgId, DLG_SH_COPYR);
			HelpCopyDate = GetDlgState(dlgId, DLG_SH_DATECOPY);

			SetupCancelled = False;
			F_ApiClose(dlgId, 0);
			return;

		case DLG_SH_CANCEL:
			SetupCancelled = True;
			F_ApiClose(dlgId, 0);
			return;

		case DLG_SH_HELP:
			RunWinHlp(IDH_SetWinH);
			break;

		default:
			break;
	}
	F_ApiReturnValue(FR_ModalStayUp);
}


VoidT WriteIniHVars(BoolT book, UCharT form)
{
	StringT temp1 = NULL;
	StringT basename = NULL;

	WriteIniCommonVars(True, True);
	M2GIniPutBool(CurrIniName, "Automation", "CompileHelp", MakeHLP);

	basename = Fm2a(temp1 = BaseFileName(HelpFileName));
	F_Free(temp1);
	M2GIniPutSetting(CurrIniName, "HelpOptions", "XrefFileDefault", basename);

	M2GIniPutSetting(CurrIniName, "HelpOptions", "HPJFileName", temp1 = Fm2a(HPJName));
	F_Free(temp1);
	M2GIniPutSetting(CurrIniName, "HelpOptions", "HelpVersion", (form == '3') ? "3" : "4");
	M2GIniPutBool(CurrIniName, "HelpOptions", "MakeCombinedCnt", MakeCombCnt);

	M2GIniPutSetting(CurrIniName, "HelpContents", "CntType", book || MakeCombCnt ? "Body" : "Full");
	if (book || MakeCombCnt) {
		SetCntVars(basename, NULL);
		M2GIniPutSetting(CurrIniName, "HelpContents", "CntTopic", temp1 = Fm2a(HelpContentsTopic));
		F_Free(temp1);
		M2GIniPutSetting(CurrIniName, "HelpContents", "CntBase", temp1 = Fm2a(HelpFileName));
		F_Free(temp1);
		M2GIniPutSetting(CurrIniName, "HelpContents", "CntName", temp1 = Fm2a(CntFileName));
		F_Free(temp1);
	}

	F_Free(basename);
}



/* end of m2rseth.c */

