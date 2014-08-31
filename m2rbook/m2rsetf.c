/* m2rsetf.c is the FDK help dialog handler for MIF projects, JHG  
 * Copyright 2001 Omni Systems, Inc.  All Rights Reserved.
 */

#include "m2rbook.h"


/* defines for MIF .ini setup dialog */

#define DLG_SF_PROJDESC  1

#define DLG_SF_FM_TOC    2
#define DLG_SF_FM_IX     3
#define DLG_SF_FM_GEN    4

#define DLG_SF_BOOK      6
#define DLG_SF_FMEXTRAD  7
#define DLG_SF_MIFBOX   10
#define DLG_SF_BMIFBOX  12

#define DLG_SF_TPL_CK   16

#define DLG_SF_HELP     DLG_SF_TPL_CK + DLG_TF_HELP
#define DLG_SF_OK       DLG_SF_TPL_CK + DLG_TF_OK
#define DLG_SF_CANCEL   DLG_SF_TPL_CK + DLG_TF_CANCEL


BoolT MakeBook = False;
BoolT OrigExtForMIF = False;


/* setup MIF project dialog */


VoidT SetupFMIFIniDialog(StringT filename, F_ObjHandleT bookId)
{
	F_ObjHandleT dlgId = 0;

	/* open resource for Help Settings dialog box. */
	dlgId = F_ApiOpenResource(FO_DialogResource, "m2rsetf");
	DlgIds[DLG_SETUP_FMIF_INI - 1] = dlgId;

	/* set CurrProjectName and Path at top of dialog */
	SetDlgProject(dlgId, DLG_SF_PROJDESC);

	/* set checkboxes for TOC and IX inclusion */
	UseFmTOC = UseFmIX = UseFmGenFiles = True;
	MakeBook = bookId ? True : False;
	SetDlgState(dlgId, DLG_SF_FM_TOC, UseFmTOC);
	SetDlgState(dlgId, DLG_SF_FM_IX, UseFmIX);
	SetDlgState(dlgId, DLG_SF_FM_GEN, UseFmGenFiles);

	SetDlgState(dlgId, DLG_SF_BOOK, MakeBook);
	SetDlgState(dlgId, DLG_SF_FMEXTRAD + (OrigExtForMIF ? 0 : 1), True);

	SetDlgText(dlgId, DLG_SF_MIFBOX, MIFSuffix = MifExt);
	SetDlgText(dlgId, DLG_SF_BMIFBOX, MIFBookSuffix = MifExt);
	EnableDlgItem(dlgId, DLG_SF_MIFBOX, !OrigExtForMIF);
	EnableDlgItem(dlgId, DLG_SF_BMIFBOX, MakeBook && !OrigExtForMIF);

	/* turn off everything under template and compiler */
	SetFrameTemplateDlg(dlgId, DLG_SF_TPL_CK);

	SetupCancelled = True;  /* default if closed */
	F_ApiModalDialog(DLG_SETUP_FMIF_INI, dlgId);
}


VoidT ProcSetupFMIFIniDialogEvent(IntT itemNum)
{
	F_ObjHandleT dlgId = DlgIds[DLG_SETUP_FMIF_INI - 1];

	if ((itemNum >= DLG_SF_TPL_CK)
	 && (itemNum < DLG_SF_HELP))
		ProcFrameTemplateDlgEvent(dlgId, DLG_SF_TPL_CK, itemNum);

	else switch (itemNum) {

		case DLG_SF_FM_TOC:
		case DLG_SF_FM_IX:
			break;

		case DLG_SF_FMEXTRAD:
		case DLG_SF_FMEXTRAD + 1:
			EnableDlgItem(dlgId, DLG_SF_MIFBOX, !GetDlgState(dlgId, DLG_SF_FMEXTRAD));
			EnableDlgItem(dlgId, DLG_SF_BMIFBOX,
			                     !GetDlgState(dlgId, DLG_SF_FMEXTRAD)
			                  && GetDlgState(dlgId, DLG_SF_BOOK));
			break;

		case DLG_SF_BOOK:
			EnableDlgItem(dlgId, DLG_SF_BMIFBOX,
			                     GetDlgState(dlgId, itemNum)
			                  && !GetDlgState(dlgId, DLG_SF_FMEXTRAD));
			break;

		case DLG_SF_OK:
			UseFmTOC = GetDlgState(dlgId, DLG_SF_FM_TOC);
			UseFmIX = GetDlgState(dlgId, DLG_SF_FM_IX);
			UseFmGenFiles = GetDlgState(dlgId, DLG_SF_FM_GEN);

			MakeBook = GetDlgState(dlgId, DLG_SF_BOOK);
			OrigExtForMIF = GetDlgState(dlgId, DLG_SF_FMEXTRAD);

			MIFSuffix = GetDlgText(dlgId, DLG_SF_MIFBOX);
			MIFBookSuffix = GetDlgText(dlgId, DLG_SF_BMIFBOX);

			GetFrameTemplateDlg(dlgId, DLG_SF_TPL_CK);

			SetupCancelled = False;
			F_ApiClose(dlgId, 0);
			return;

		case DLG_SF_CANCEL:
			SetupCancelled = True;
			F_ApiClose(dlgId, 0);
			return;

		case DLG_SF_HELP:
			RunWinHlp(IDH_SetMIF);
			break;

		default:
			break;
	}

	F_ApiReturnValue(FR_ModalStayUp);
}


VoidT WriteIniFVars(VoidT)
{
	WriteIniCommonVars(True, False);
	M2GIniPutBool(CurrIniName, Setup, "MakeBookMIF", MakeBook);
	M2GIniPutBool(CurrIniName, Setup, "OrigExtForMIF", OrigExtForMIF);
	if (!OrigExtForMIF) {
		if (MIFSuffix == NULL)
			MIFSuffix = MifExt;
		M2GIniPutSetting(CurrIniName, Setup, "FileSuffix", MIFSuffix);
		if (MIFBookSuffix == NULL)
			MIFBookSuffix = MIFSuffix;
		M2GIniPutSetting(CurrIniName, Setup, "MIFBookSuffix", MIFBookSuffix);
	}
}



/* end of m2rsetf.c */

