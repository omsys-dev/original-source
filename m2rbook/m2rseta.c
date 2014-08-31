/* m2rseta.c is the FDK setup dialog handler for ASCII DCL projects, JHG  
 * Copyright 2001 Omni Systems, Inc.  All Rights Reserved.
 */

#include "m2rbook.h"


/* defines for MIF .ini setup dialog */

#define DLG_SA_PROJDESC  1

#define DLG_SA_FM_TOC    2
#define DLG_SA_FM_IX     3
#define DLG_SA_FM_GEN    4

#define DLG_SA_GXRAD     7
#define DLG_SA_GXRAD2    8
#define DLG_SA_GXERAD    9

#define DLG_SA_GX_BASE  11
#define DLG_SA_GX_BMP   11
#define DLG_SA_GX_CDR   12
#define DLG_SA_GX_EPS   13
#define DLG_SA_GX_GIF   14
#define DLG_SA_GX_JPG   15
#define DLG_SA_GX_OLE   16
#define DLG_SA_GX_PCT   17
#define DLG_SA_GX_PCX   18
#define DLG_SA_GX_RF    19
#define DLG_SA_GX_TIF   20
#define DLG_SA_GX_WMF   21
#define DLG_SA_GX_WPG   22
#define DLG_SA_GX_LAST  22
#define DLG_SA_GX_COUNT ((DLG_SA_GX_LAST - DLG_SA_GX_BASE) + 1)

#define DLG_SA_NMCBOX   25
#define DLG_SA_DGCBOX   28

#define DLG_SA_TPL_CK   31

#define DLG_SA_HELP     DLG_SA_TPL_CK + DLG_TF_HELP
#define DLG_SA_OK       DLG_SA_TPL_CK + DLG_TF_OK
#define DLG_SA_CANCEL   DLG_SA_TPL_CK + DLG_TF_CANCEL



IntT ImpGraphics = 0;  /* default 0=Normal, 1=Retain, 2=Export */

StringT ImpGraphicsVals[3] = {
 "Normal", "Retain", "Export"
};

StringT ImpGraphicsNameChars = "4";
StringT ImpGraphicsDigits = "4";

/* shows default graphics export for each type (r/o) */
BoolT GxRef[DLG_SA_GX_COUNT] = {
 False, True, True, True,
 True, False, True, True,
 False, True, False, True
};

/* shows current graphics export for each type (r/w) */
/* same as GxRef if ImportGraphics=Normal, else all  */
/* True if ImportGraphics=Export, or False if =Retain */
BoolT GxDef[DLG_SA_GX_COUNT] = {
 False, True, True, True,
 True, False, True, True,
 False, True, False, True
};

/* shows user-selected graphics export for each type */
BoolT GxUsage[DLG_SA_GX_COUNT] = {
 False, True, True, True,
 True, False, True, True,
 False, True, False, True
};


/* setup DCL project dialog */


VoidT SetupADCLIniDialog(StringT filename, F_ObjHandleT bookId)
{
	F_ObjHandleT dlgId = 0;
	IntT i = 0;

	/* open resource for Help Settings dialog box. */
	dlgId = F_ApiOpenResource(FO_DialogResource, "m2rseta");
	DlgIds[DLG_SETUP_ADCL_INI - 1] = dlgId;

	/* set CurrProjectName and Path at top of dialog */
	SetDlgProject(dlgId, DLG_SA_PROJDESC);

	/* set checkboxes for TOC and IX inclusion */
	UseFmTOC = UseFmIX = UseFmGenFiles = True;
	SetDlgState(dlgId, DLG_SA_FM_TOC, UseFmTOC);
	SetDlgState(dlgId, DLG_SA_FM_IX, UseFmIX);
	SetDlgState(dlgId, DLG_SA_FM_GEN, UseFmGenFiles);

	/* set rad and checkboxes for graphic export */
	SetDlgState(dlgId, DLG_SA_GXRAD + ImpGraphics, True);
	SetImpGrDef(dlgId, ImpGraphics, True);

	/* set numbers for export names */
	SetDlgText(dlgId, DLG_SA_NMCBOX, ImpGraphicsNameChars);
	SetDlgText(dlgId, DLG_SA_DGCBOX, ImpGraphicsDigits);

	/* turn off everything under template and compiler */
	SetFrameTemplateDlg(dlgId, DLG_SA_TPL_CK);

	SetupCancelled = True;  /* default if closed */
	F_ApiModalDialog(DLG_SETUP_ADCL_INI, dlgId);
}


VoidT ProcSetupADCLIniDialogEvent(IntT itemNum)
{
	F_ObjHandleT dlgId = DlgIds[DLG_SETUP_ADCL_INI - 1];
	IntT i = 0;

	if ((itemNum >= DLG_SA_TPL_CK)
	 && (itemNum < DLG_SA_HELP))
		ProcFrameTemplateDlgEvent(dlgId, DLG_SA_TPL_CK, itemNum);

	else switch (itemNum) {

		case DLG_SA_FM_TOC:
		case DLG_SA_FM_IX:
			break;

		case DLG_SA_GXRAD:
		case DLG_SA_GXRAD2:
		case DLG_SA_GXERAD:
			i = GetRadValue(dlgId, DLG_SA_GXRAD, DLG_SA_GXERAD);
			SetImpGrDef(dlgId, i, True);
			break;

		case DLG_SA_OK:
			UseFmTOC = GetDlgState(dlgId, DLG_SA_FM_TOC);
			UseFmIX = GetDlgState(dlgId, DLG_SA_FM_IX);
			UseFmGenFiles = GetDlgState(dlgId, DLG_SA_FM_GEN);

			ImpGraphics = GetRadValue(dlgId, DLG_SA_GXRAD, DLG_SA_GXERAD);
			SetImpGrDef(dlgId, ImpGraphics, False);

			/* get checkboxes for graphic export */
			for (i = 0; i < DLG_SA_GX_COUNT; i++)
				GxUsage[i] = GetDlgState(dlgId, i + DLG_SA_GX_BASE);

			ImpGraphicsNameChars = GetDlgText(dlgId, DLG_SA_NMCBOX);
			ImpGraphicsDigits = GetDlgText(dlgId, DLG_SA_DGCBOX);

			GetFrameTemplateDlg(dlgId, DLG_SA_TPL_CK);

			SetupCancelled = False;
			F_ApiClose(dlgId, 0);
			return;

		case DLG_SA_CANCEL:
			SetupCancelled = True;
			F_ApiClose(dlgId, 0);
			return;

		case DLG_SA_HELP:
			RunWinHlp(IDH_SetDCL);
			break;

		default:
			break;
	}

	F_ApiReturnValue(FR_ModalStayUp);
}


VoidT WriteIniAVars(VoidT)
{
	WriteIniCommonVars(True, True);
	PutGraphExportP();
}


VoidT SetImpGrDef(F_ObjHandleT dlgId, IntT gdef, BoolT set)
{
	IntT i = 0;

	switch (gdef) {
		case 0:  // Normal, default
			for (i = 0; i < DLG_SA_GX_COUNT; i++)
				GxDef[i] = GxRef[i];
			break;
		case 1:  // Retain, False
			for (i = 0; i < DLG_SA_GX_COUNT; i++)
				GxDef[i] = False;
			break;
		case 2:  // Export, True
			for (i = 0; i < DLG_SA_GX_COUNT; i++)
				GxDef[i] = True;
			break;
		default:
			break;
	}

	if (set)
		for (i = 0; i < DLG_SA_GX_COUNT; i++)
			SetDlgState(dlgId, i + DLG_SA_GX_BASE, GxDef[i]);
}
	

VoidT PutGraphExport(FILE *fini)
{
	IntT i = 0;
	StringT pname = NULL;

	fprintf(fini, "\n[GraphExport]\n");
	fprintf(fini, "ImportGraphics=%s\n", ImpGraphicsVals[ImpGraphics]);
	fprintf(fini, "ExportNameChars=%s\n", ImpGraphicsNameChars);
	fprintf(fini, "ExportNumDigits=%s\n", ImpGraphicsDigits);

	for (i = 0; i <= (DLG_SA_GX_LAST - DLG_SA_GX_BASE); i++) {
		if (GxDef[i] != GxUsage[i]) {
			pname = F_StrNew(F_StrLen(ImportDesNames[i + 1]) + 16);
			F_Sprintf(pname, "Export%sFiles=%s", 
			     ImportDesNames[i + 1], GxUsage[i] ? "Yes" : "No");
			fprintf(fini, "%s\n", pname);
			F_StrFree(pname);
		}
	}
	fprintf(fini, "\n");
}


VoidT PutGraphExportP(VoidT)
{
	IntT i = 0;
	StringT pname = NULL;

	M2GIniPutSetting(CurrIniName, "GraphExport", "ImportGraphics", ImpGraphicsVals[ImpGraphics]);
	M2GIniPutSetting(CurrIniName, "GraphExport", "ExportNameChars", ImpGraphicsNameChars);
	M2GIniPutSetting(CurrIniName, "GraphExport", "ExportNumDigits", ImpGraphicsDigits);

	for (i = 0; i <= (DLG_SA_GX_LAST - DLG_SA_GX_BASE); i++) {
		if (GxDef[i] != GxUsage[i]) {
			pname = F_StrNew(F_StrLen(ImportDesNames[i + 1]) + 12);
			F_Sprintf(pname, "Export%sFiles", ImportDesNames[i + 1]);
			M2GIniPutBool(CurrIniName, "GraphExport", pname, GxUsage[i]);
			F_StrFree(pname);
		}
	}
}


/* end of m2rseta.c */

