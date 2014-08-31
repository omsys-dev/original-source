/* m2rdgi.c is the FDK designer panel for Imported Graphics
 * Copyright 2000 Omni Systems, Inc.  All Rights Reserved.
 */


#include "m2rbook.h"



/**************************************************************/
/* Imported Graphics designer page */


/* m2rdgi items */

#define DLG_DGI_EXPORT_RAD 19
#define DLG_DGI_RETAIN_RAD 20

#define DLG_DGI_NM_CHARS   22
#define DLG_DGI_NM_DIGITS  24

#define DLG_DGI_NOPATH_CK  28
#define DLG_DGI_MAPNM_CK   29

#define DLG_DGI_GRFILES_CK 30
#define DLG_DGI_FILES_LIST 31

#define DLG_DGI_OLDNEW_LAB 33
#define DLG_DGI_OLDNM_BOX  34
#define DLG_DGI_OLDNM_POP  35
#define DLG_DGI_NEWNM_BOX  37
#define DLG_DGI_NEWNM_POP  38

#define DLG_DGI_WRAPBMP_CK 41
#define DLG_DGI_WRAPWMF_CK 42
#define DLG_DGI_WRAPEQN_CK 43

#define DLG_DGI_EPSI_P_RAD 46
#define DLG_DGI_EPSI_E_RAD 47
#define DLG_DGI_EPSI_B_RAD 48



/* imported graphics dialog events */

VoidT ProcImportedGraphicsDesignerEvent(F_ObjHandleT dlgId, IntT itemNum)
{
	BoolT mapping = False;

	switch (itemNum) {

		case DLG_DGI_EXPORT_RAD:
		case DLG_DGI_RETAIN_RAD:
			ExportGraphsChanged = True;
			DesignerChanged = True;
			break;

		case DLG_DGI_NM_CHARS:
		case DLG_DGI_NM_DIGITS:
			DesignerChanged = True;
			break;

		case DLG_DGI_NOPATH_CK:
		case DLG_DGI_MAPNM_CK:
			DesignerChanged = True;
			break;

		case DLG_DGI_GRFILES_CK:
			mapping = GetDlgState(dlgId, itemNum);
			EnableDlgItem(dlgId, DLG_DGI_FILES_LIST, mapping);
			ExportGraphsChanged = True;
			DesignerChanged = True;
			if (mapping) /* turn on only, not off */
				SetDlgState(dlgId, DLG_DGI_MAPNM_CK, True);
			break;

		case DLG_DGI_FILES_LIST:
			DesignerChanged = True;
			break;

		case DLG_DGI_WRAPBMP_CK:
		case DLG_DGI_WRAPWMF_CK:
		case DLG_DGI_WRAPEQN_CK:
			DesignerChanged = True;
			break;

		case DLG_DGI_EPSI_P_RAD:
		case DLG_DGI_EPSI_E_RAD:
		case DLG_DGI_EPSI_B_RAD:
			DesignerChanged = True;
			break;

		default:
			break;
	}
}


VoidT SetImportedGraphicsDesigner(F_ObjHandleT dlgId)
{
	EnableItems(dlgId, DLG_DGI_OLDNEW_LAB, DLG_DGI_NEWNM_POP, False);

	SetDlgState(dlgId, CurrExportGraph->retain ? DLG_DGI_RETAIN_RAD :
																							 DLG_DGI_EXPORT_RAD, True);

	SetLabels(dlgId, DLG_DGI_FILES_LIST, &GrFormStrings);
	if (CurrExportGraph->remap) {
		SetDlgState(dlgId, DLG_DGI_GRFILES_CK, True);
		SetGrExFormListState(dlgId, DLG_DGI_FILES_LIST, CurrExportGraph->remap);
		EnableDlgItem(dlgId, DLG_DGI_FILES_LIST, True);
	}
	else {
		SetDlgState(dlgId, DLG_DGI_GRFILES_CK, False);
		SetDlgState(dlgId, DLG_DGI_FILES_LIST, 1);
		EnableDlgItem(dlgId, DLG_DGI_FILES_LIST, False);
	}

	SetDlgText(dlgId, DLG_DGI_NM_CHARS, CurrDesGraph.exname_chars);
	SetDlgText(dlgId, DLG_DGI_NM_DIGITS, CurrDesGraph.exname_digits);

	SetDlgState(dlgId, DLG_DGI_NOPATH_CK, CurrDesGraph.strip_path);
	SetDlgState(dlgId, DLG_DGI_MAPNM_CK, CurrDesGraph.remap_name);

	SetDlgState(dlgId, DLG_DGI_WRAPBMP_CK, CurrDesGraph.wrap_bmp);
	SetDlgState(dlgId, DLG_DGI_WRAPWMF_CK, CurrDesGraph.wrap_wmf);
	SetDlgState(dlgId, DLG_DGI_WRAPEQN_CK, CurrDesGraph.wrap_eqn);

	SetDlgState(dlgId, DLG_DGI_EPSI_P_RAD + CurrDesGraph.epsi_usage, True);

	DesignerChanged = False;
}


VoidT GetImportedGraphicsDesigner(F_ObjHandleT dlgId)
{
	StringT mapped = NULL;

	CurrExportGraph->retain = GetDlgState(dlgId, DLG_DGI_RETAIN_RAD);

	if (GetDlgState(dlgId, DLG_DGI_GRFILES_CK)) {
		mapped = GetGrExFormListText(dlgId, DLG_DGI_FILES_LIST);
		if ((mapped != CurrExportGraph->remap)
		 && ((mapped == NULL)
		  || (CurrExportGraph->remap == NULL)
		  || F_StrICmp(mapped, CurrExportGraph->remap)))
			CurrExportGraph->remap = mapped;
	}
	else
		CurrExportGraph->remap = NULL;

	CurrDesGraph.exname_chars = GetDlgText(dlgId, DLG_DGI_NM_CHARS);
	CurrDesGraph.exname_digits = GetDlgText(dlgId, DLG_DGI_NM_DIGITS);

	CurrDesGraph.strip_path = GetDlgState(dlgId, DLG_DGI_NOPATH_CK);
	CurrDesGraph.remap_name = GetDlgState(dlgId, DLG_DGI_MAPNM_CK);

	CurrDesGraph.wrap_bmp = GetDlgState(dlgId, DLG_DGI_WRAPBMP_CK);
	CurrDesGraph.wrap_wmf = GetDlgState(dlgId, DLG_DGI_WRAPWMF_CK);
	CurrDesGraph.wrap_eqn = GetDlgState(dlgId, DLG_DGI_WRAPEQN_CK);

	CurrDesGraph.epsi_usage = GetRadValue(dlgId, DLG_DGI_EPSI_P_RAD,
																							 DLG_DGI_EPSI_B_RAD);

	GraphicsChanged = True;
	DesignerChanged = False;
}


VoidT FreeImportedGraphicsDesigner(F_ObjHandleT dlgId)
{
	/* no resources to release */
}



/* end of file m2rdgi.c */

