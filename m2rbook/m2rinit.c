/* m2rinit.c is the init dialog handler for mif2rtf for FM 5.5
 * Last edited on 12/2/98 by Jeremy H. Griffith.
 * Copyright 1998 Omni Systems, Inc.  All Rights Reserved.
 */


#include "m2rbook.h"


/* globals */

BoolT UseInit = True;
BoolT UseDone = True;
BoolT DclOnly = False;
BoolT ProcCancelled = False;

IntT FormWanted = 1;
IntT FormNow = 1;

StringT DestSuffix = NULL;


/* init dialog elements */

#define DLG_IN_PROJDESC   1
#define DLG_IN_TYPELIST   3
#define DLG_IN_SUFFBOX    5
#define DLG_IN_USEMIF_CK  6
#define DLG_IN_DELMIF_CK  7
#define DLG_IN_USEDCL_CK  9

#define DLG_IN_NOGR_RAD  11
#define DLG_IN_EQUGR_RAD 12
#define DLG_IN_ALLGR_RAD 13

#define DLG_IN_ORIGR_CK  14
#define DLG_IN_GRONLY_CK 15

#define DLG_IN_MKCNT_CK  16
#define DLG_IN_MKHLP_CK  17
#define DLG_IN_DOWAS_CK  18

#define DLG_IN_HELP      19
#define DLG_IN_OK        20
#define DLG_IN_CANCEL    21



/* init dialog functions */

BoolT UseInitDialog(StringT projdesc)
{
	F_ObjHandleT dlgId = 0;

	F_ObjHandleT descriptId = 0;
	F_ObjHandleT typeListId = 0;
	F_StringsT dlgTypeStrings;

	UCharT errstr[256];
	StringT newsuff = NULL;

	FormNow = FormWanted = IsBookFile ? ProjSelType : FormType;
	SetDestSuffix();

	if (!UseInit || ProjRemote) {
#if 0
		if ((FormNow == FormType)
		 && PT_rtf(FormNow)) {
			FormOver = False; /* same as default, no switch needed */
			FormLen = 0;
		}
		else {
			FormType = FormNow;
			FormOver = True;
			FormLen = 4;
			FormStr[2] = TypeIdList[FormType - 1];
		}
#else
		FormType = FormNow;
		FormOver = True;
		FormLen = 4;
		FormStr[2] = TypeIdList[FormType - 1];
		CurrFullForm = FullFormList[FormType - 1];
#endif
		return True;
	}

	if ((dlgId = F_ApiOpenResource(FO_DialogResource, InitDialogID)) == 0) {
		F_Printf(NULL, InitDialogErr, (IntT) FA_errno);
		F_Sprintf(errstr, InitDialogErr, (IntT) FA_errno);
		if (F_ApiAlert(errstr, FF_ALERT_NO_DEFAULT) == -1)
			return False;
		return True;
	}

 	/* open the init dialog box */
	DlgIds[DLG_INIT - 1] = dlgId;

	descriptId = F_ApiDialogItemId(dlgId, DLG_IN_PROJDESC);
	F_ApiSetString(dlgId, descriptId, FP_Label, projdesc);

	/* set format to produce */
	dlgTypeStrings.val = TypeNameList;
	dlgTypeStrings.len = PROJ_TYPES + 1;
	typeListId = F_ApiDialogItemId(dlgId, DLG_IN_TYPELIST);
	F_ApiSetStrings(dlgId, typeListId, FP_Labels, &dlgTypeStrings);

	SetDlgState(dlgId, DLG_IN_TYPELIST, FormNow);
	SetDlgText(dlgId, DLG_IN_SUFFBOX, DestSuffix);
	EnableDlgItem(dlgId, DLG_IN_SUFFBOX,
   ((FormNow == PT_MIF) && OrigExtForMIF) ? False : True);

	/* set MIF usage */
	if (!IsBookFile
	 && !MIFExisted) {  /* no MIF available to use */
		UseMIF = False;
		EnableDlgItem(dlgId, DLG_IN_USEMIF_CK, False);
	}
	else 
		EnableDlgItem(dlgId, DLG_IN_USEMIF_CK, True);

	SetDlgState(dlgId, DLG_IN_USEMIF_CK, UseMIF);	
	SetDlgState(dlgId, DLG_IN_DELMIF_CK, DeleteMIF);
	SetDlgState(dlgId, DLG_IN_USEDCL_CK, UseDCL);	

	/* set graphics options */
	SetDlgState(dlgId, DLG_IN_NOGR_RAD, !(WriteAllGraphics || WriteEqns));	
	SetDlgState(dlgId, DLG_IN_EQUGR_RAD, WriteEqns);	
	SetDlgState(dlgId, DLG_IN_ALLGR_RAD, WriteAllGraphics);	

	SetDlgState(dlgId, DLG_IN_GRONLY_CK, WriteGraphicsOnly);

	if (PT_htm(FormNow))	{
		SetDlgState(dlgId, DLG_IN_ORIGR_CK, UseOrigGrName);
		EnableDlgItem(dlgId, DLG_IN_ORIGR_CK, True);
	}
	else {
		SetDlgState(dlgId, DLG_IN_ORIGR_CK, False);
		EnableDlgItem(dlgId, DLG_IN_ORIGR_CK, False);
	}

	/* set WinHelp options */
	if (FormNow == PT_WH4) {  /* WinHelp 4, WinHelp 3 has no .cnt */
		if (IsBookFile) { /* always makes .cnt */
			SetDlgState(dlgId, DLG_IN_MKCNT_CK, True);
			EnableDlgItem(dlgId, DLG_IN_MKCNT_CK, False);
		}
		else { /* individual file, so optional */
			SetDlgState(dlgId, DLG_IN_MKCNT_CK, MakeCNT);
			EnableDlgItem(dlgId, DLG_IN_MKCNT_CK, True);
		}
	}
	else { /* turn off */
		SetDlgState(dlgId, DLG_IN_MKCNT_CK, False);
		EnableDlgItem(dlgId, DLG_IN_MKCNT_CK, False);
	}

	SetDlgState(dlgId, DLG_IN_MKHLP_CK, MakeHLP);
	EnableDlgItem(dlgId, DLG_IN_MKHLP_CK, PT_comp(FormType) ? True : False);
	SetDlgState(dlgId, DLG_IN_DOWAS_CK, WrapAndShip);
	EnableDlgItem(dlgId, DLG_IN_DOWAS_CK, True);

	/* display the init dialog box */
	FA_errno = FE_Success;
	F_ApiModalDialog(DLG_INIT, dlgId);

	if (FA_errno != FE_Success) {
		F_Printf(NULL, InitDialogErr, (IntT) FA_errno);
		F_Sprintf(errstr, InitDialogErr, (IntT) FA_errno);
		if (F_ApiAlert(errstr, FF_ALERT_NO_DEFAULT) == -1)
			return False;
		return True;
	}

	DclOnly = False;
	if (!ProcCancelled) {
		if (DestSuffix[0] != '.') {
			newsuff = F_StrNew(F_StrLen(DestSuffix) + 2);
			newsuff[0] = '.';
			newsuff[1] = '\0';
			F_StrCat(newsuff, DestSuffix);
			F_StrFree(DestSuffix);
			DestSuffix = newsuff;
		}
		if (!F_StrICmp(DestSuffix, ".dcl")) {
			DclOnly = True;
			FormNow = PT_DCL;
		}
		else
			FileSuffix = DestSuffix;
		switch (FormNow) {
			case PT_WORD7:
			case PT_WORD8:
				WordSuffix = DestSuffix;
				break;
			case PT_WORDP:
				WordPerfectSuffix = DestSuffix;
				break;
			case PT_HTML:
			case PT_JAVH:
			case PT_MSHH:
			case PT_ORH:
			case PT_EH:
			case PT_OMH:
			case PT_XHTM:
				HTMLSuffix = DestSuffix;
				break;
			case PT_XML:
				XMLSuffix = DestSuffix;
				break;
			case PT_MIF:
				MIFSuffix = DestSuffix;
				break;
			case PT_DCL:
				DCLSuffix = DestSuffix;
				break;
			default:
				break;
		}

#if 0
		if ((FormNow == FormType)
		 && PT_rtf(FormNow)) {
			FormOver = False; /* same as default, no switch needed */
			FormLen = 0;
		}
		else {
			FormType = FormNow;
			FormOver = True;
			FormLen = 4;
			FormStr[2] = TypeIdList[FormType - 1];
		}
#else
		FormType = FormNow;
		FormOver = True;
		FormLen = 4;
		FormStr[2] = TypeIdList[FormType - 1];
		CurrFullForm = FullFormList[FormType - 1];
#endif
	}

	return (!ProcCancelled);
}


VoidT ProcInitDialogEvent(IntT itemNum)
{
	F_ObjHandleT dlgId = DlgIds[DLG_INIT - 1];

	switch (itemNum) {
		case DLG_IN_TYPELIST:
			FormNow = GetDlgState(dlgId, itemNum);
			SetDestSuffix();
			SetDlgText(dlgId, DLG_IN_SUFFBOX, DestSuffix);
			if (FormNow == PT_WH3) {
				SetDlgState(dlgId, DLG_IN_MKCNT_CK, False);
				EnableDlgItem(dlgId, DLG_IN_MKCNT_CK, False);
				EnableDlgItem(dlgId, DLG_IN_MKHLP_CK, True);
				EnableDlgItem(dlgId, DLG_IN_SUFFBOX, False);
			}
			else if (FormNow == PT_WH4) {
				EnableDlgItem(dlgId, DLG_IN_MKCNT_CK, True);
				EnableDlgItem(dlgId, DLG_IN_MKHLP_CK, True);
				EnableDlgItem(dlgId, DLG_IN_SUFFBOX, False);
			}
			else if (PT_hh(FormNow)) {  /* HTML-based Help */
				SetDlgState(dlgId, DLG_IN_MKCNT_CK, False);
				EnableDlgItem(dlgId, DLG_IN_MKCNT_CK, False);
				//SetDlgState(dlgId, DLG_IN_MKHLP_CK, MakeHLP);
				EnableDlgItem(dlgId, DLG_IN_MKHLP_CK, True);
			}
			else {
				SetDlgState(dlgId, DLG_IN_MKCNT_CK, False);
				SetDlgState(dlgId, DLG_IN_MKHLP_CK, False);
				EnableDlgItem(dlgId, DLG_IN_MKCNT_CK, False);
				EnableDlgItem(dlgId, DLG_IN_MKHLP_CK, False);
				EnableDlgItem(dlgId, DLG_IN_SUFFBOX,
				 ((FormNow == PT_MIF) && OrigExtForMIF) ? False : True);
			}
			break;

		case DLG_IN_OK:
			if ((FormNow != PT_MIF) || !OrigExtForMIF)
				DestSuffix = GetDlgText(dlgId, DLG_IN_SUFFBOX);
			UseMIF = GetDlgState(dlgId, DLG_IN_USEMIF_CK);
			DeleteMIF = GetDlgState(dlgId, DLG_IN_DELMIF_CK);
			UseDCL = GetDlgState(dlgId, DLG_IN_USEDCL_CK);

			if ((FormNow == PT_WH4)
			 && IsBookFile) // otherwise not enabled for setting
				MakeCNT = GetDlgState(dlgId, DLG_IN_MKCNT_CK);
			MakeHLP = GetDlgState(dlgId, DLG_IN_MKHLP_CK);
			WrapAndShip = GetDlgState(dlgId, DLG_IN_DOWAS_CK);

			WriteEqns = GetDlgState(dlgId, DLG_IN_EQUGR_RAD);
			WriteAllGraphics = GetDlgState(dlgId, DLG_IN_ALLGR_RAD);
			WriteGraphicsOnly = GetDlgState(dlgId, DLG_IN_GRONLY_CK);

			if (PT_htm(FormNow))
				UseOrigGrName = GetDlgState(dlgId, DLG_IN_ORIGR_CK);

			ProcCancelled = False;
			F_ApiClose(dlgId, 0);
			return;

		case DLG_IN_CANCEL:
			ProcCancelled = True;
			F_ApiClose(dlgId, 0);
			return;

		case DLG_IN_HELP:
			RunWinHlp(IDH_Export);
			break;

		default:
			break;
	}

	F_ApiReturnValue(FR_ModalStayUp);
}


VoidT SetDestSuffix(VoidT)
{
	if (FileSuffix)
		DestSuffix = FileSuffix;
	else switch (FormNow) {
		case PT_WORD7:
		case PT_WORD8:
			DestSuffix = WordSuffix;
			break;
		case PT_WORDP:
			DestSuffix = WordPerfectSuffix;
			break;
		case PT_HTML:
		case PT_JAVH:
		case PT_MSHH:
		case PT_OMH:
		case PT_ORH:
		case PT_EH:
		case PT_XHTM:
			DestSuffix = HTMLSuffix;
			break;
		case PT_XML:
		case PT_DITA:
		case PT_DCBK:
			DestSuffix = XMLSuffix;
			break;
		case PT_MIF:
			DestSuffix = MIFSuffix;
			break;
		case PT_DCL:
			DestSuffix = DCLSuffix;
			break;
		default:
			DestSuffix = RtfExt;
			break;
	}
}


/* End of m2rinit.c */

