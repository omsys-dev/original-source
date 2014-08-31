/* m2rdtpl.c is the FDK designer panel for Templates
 * Last edited on 1/7/99 by Jeremy H. Griffith.
 * Copyright 1998, 1999 Omni Systems, Inc.  All Rights Reserved.
 */


#include "m2rbook.h"


/**************************************************************/
/* Templates designer page */


/* m2rdtpl items */

#define DLG_DTPL_FMTPL_CK  18
#define DLG_DTPL_FMTPL_BOX 19
#define DLG_DTPL_FMTPL_BTN 20

#define DLG_DTPL_PSTYL_CK  21
#define DLG_DTPL_CSTYL_CK  22
#define DLG_DTPL_TABLS_CK  23
#define DLG_DTPL_XREFS_CK  24
#define DLG_DTPL_CONDS_CK  25
#define DLG_DTPL_CFNTS_CK  26

#define DLG_DTPL_PAGES_CK  28
#define DLG_DTPL_REFPGS_CK 29
#define DLG_DTPL_VARS_CK   30
#define DLG_DTPL_COLORS_CK 31
#define DLG_DTPL_EQUS_CK   32
#define DLG_DTPL_DOCS_CK   33

#define DLG_DTPL_OVERS_CK  35
#define DLG_DTPL_PBRKS_CK  36

#define DLG_DTPL_WDTPL_CK  38
#define DLG_DTPL_AUTOUP_CK 39
#define DLG_DTPL_WDTPL_BOX 40
#define DLG_DTPL_WDTPL_BTN 41


StringT FMTemplateName = NULL;
StringT WDTemplateName = NULL;



/* Templates dialog events */

VoidT ProcTemplateDesignerEvent(F_ObjHandleT dlgId, IntT itemNum)
{
	StringT newname = NULL;
	IntT err = 0;

	switch (itemNum) {

		case DLG_DTPL_FMTPL_CK:
			EnableItems(dlgId, DLG_DTPL_FMTPL_BOX, DLG_DTPL_OVERS_CK,
												 GetDlgState(dlgId, itemNum));
			DesignerChanged = True;
			break;

		case DLG_DTPL_FMTPL_BOX:
			/* prohibit direct changes from typing in box */
			SetDlgText(dlgId, DLG_DTPL_FMTPL_BOX, FMTemplateName);
			break;

		case DLG_DTPL_FMTPL_BTN:
#if 0
			err = F_ApiChooseFile(&newname, ChooseTemplateFileStr,
								CurrProjPath, FMTemplateName, FV_ChooseSelect, (StringT) "");
#else
			M2GGetOpenFile(ChooseTemplateFileStr, CurrProjPath, &newname);
#endif
			if (!err
			 && (newname != NULL)) {
				SetDlgText(dlgId, DLG_DTPL_FMTPL_BOX, FMTemplateName = newname);
				newname = NULL;
			}
			DesignerChanged = True;
			break;

		case DLG_DTPL_PSTYL_CK:
		case DLG_DTPL_CSTYL_CK:
		case DLG_DTPL_TABLS_CK:
		case DLG_DTPL_XREFS_CK:
		case DLG_DTPL_CONDS_CK:
		case DLG_DTPL_CFNTS_CK:

		case DLG_DTPL_PAGES_CK:
		case DLG_DTPL_REFPGS_CK:
		case DLG_DTPL_VARS_CK:
		case DLG_DTPL_COLORS_CK:
		case DLG_DTPL_EQUS_CK:
		case DLG_DTPL_DOCS_CK:

		case DLG_DTPL_OVERS_CK:
		case DLG_DTPL_PBRKS_CK:

			DesignerChanged = True;
			break;

		case DLG_DTPL_WDTPL_CK:
			EnableItems(dlgId, DLG_DTPL_AUTOUP_CK, DLG_DTPL_WDTPL_BTN,
												 GetDlgState(dlgId, itemNum));
			DesignerChanged = True;
			break;

		case DLG_DTPL_AUTOUP_CK:
			DesignerChanged = True;
			break;

		case DLG_DTPL_WDTPL_BOX:
			/* prohibit direct changes from typing in box */
			SetDlgText(dlgId, DLG_DTPL_WDTPL_BOX, WDTemplateName);
			break;

		case DLG_DTPL_WDTPL_BTN:
#if 0
			err = F_ApiChooseFile(&newname, ChooseTemplateFileStr,
								CurrProjPath, WDTemplateName, FV_ChooseSelect, (StringT) "");
#else
			M2GGetOpenFile(ChooseTemplateFileStr, CurrProjPath, &newname);
#endif
			if (!err
			 && (newname != NULL)) {
				SetDlgText(dlgId, DLG_DTPL_WDTPL_BOX, WDTemplateName = newname);
				newname = NULL;
			}
			DesignerChanged = True;
			break;

		default:
			break;
	}
}


VoidT SetTemplateDesigner(F_ObjHandleT dlgId)
{
	IntT val = 0;

	FMTemplateName = CurrDesText.fm_tpl_name;
	WDTemplateName = CurrDesText.wd_tpl_name;

	EnableItems(dlgId, DLG_DTPL_FMTPL_BOX, DLG_DTPL_PBRKS_CK, CurrDesText.fm_tpl_apply);
	EnableItems(dlgId, DLG_DTPL_AUTOUP_CK, DLG_DTPL_WDTPL_BTN, CurrDesText.wd_tpl_apply);

	SetDlgState(dlgId, DLG_DTPL_FMTPL_CK, CurrDesText.fm_tpl_apply);
	SetDlgText(dlgId, DLG_DTPL_FMTPL_BOX, FMTemplateName);

	val = CurrDesText.fm_tpl_flags;
	SetDlgState(dlgId, DLG_DTPL_PSTYL_CK, (val & FF_UFF_PGF) ? True : False);
	SetDlgState(dlgId, DLG_DTPL_CSTYL_CK, (val & FF_UFF_FONT) ? True : False);
	SetDlgState(dlgId, DLG_DTPL_TABLS_CK, (val & FF_UFF_TABLE) ? True : False);
	SetDlgState(dlgId, DLG_DTPL_XREFS_CK, (val & FF_UFF_XREF) ? True : False);
	SetDlgState(dlgId, DLG_DTPL_CONDS_CK, (val & FF_UFF_COND) ? True : False);
	SetDlgState(dlgId, DLG_DTPL_CFNTS_CK, (val & FF_UFF_COMBINED_FONTS) ? True : False);
	SetDlgState(dlgId, DLG_DTPL_PAGES_CK, (val & FF_UFF_PAGE) ? True : False);
	SetDlgState(dlgId, DLG_DTPL_REFPGS_CK, (val & FF_UFF_REFPAGE) ? True : False);
	SetDlgState(dlgId, DLG_DTPL_VARS_CK, (val & FF_UFF_VAR) ? True : False);
	SetDlgState(dlgId, DLG_DTPL_COLORS_CK, (val & FF_UFF_COLOR) ? True : False);
	SetDlgState(dlgId, DLG_DTPL_EQUS_CK, (val & FF_UFF_MATH) ? True : False);
	SetDlgState(dlgId, DLG_DTPL_DOCS_CK, (val & FF_UFF_DOCUMENT_PROPS) ? True : False);
	SetDlgState(dlgId, DLG_DTPL_OVERS_CK, (val & FF_UFF_REMOVE_EXCEPTIONS) ? True : False);
	SetDlgState(dlgId, DLG_DTPL_PBRKS_CK, (val & FF_UFF_REMOVE_PAGE_BREAKS) ? True : False);

	SetDlgState(dlgId, DLG_DTPL_WDTPL_CK, CurrDesText.wd_tpl_apply);
	SetDlgText(dlgId, DLG_DTPL_WDTPL_BOX, WDTemplateName);
	SetDlgState(dlgId, DLG_DTPL_AUTOUP_CK, CurrDesText.wd_tpl_auto);

	DesignerChanged = False;
}


VoidT GetTemplateDesigner(F_ObjHandleT dlgId)
{
	IntT val = 0;

	CurrDesText.fm_tpl_apply = GetDlgState(dlgId, DLG_DTPL_FMTPL_CK);
	CurrDesText.fm_tpl_name = GetDlgText(dlgId, DLG_DTPL_FMTPL_BOX);

	val += GetDlgState(dlgId, DLG_DTPL_PSTYL_CK) ? FF_UFF_PGF : 0;
	val += GetDlgState(dlgId, DLG_DTPL_CSTYL_CK) ? FF_UFF_FONT : 0;
	val += GetDlgState(dlgId, DLG_DTPL_TABLS_CK) ? FF_UFF_TABLE : 0;
	val += GetDlgState(dlgId, DLG_DTPL_XREFS_CK) ? FF_UFF_XREF : 0;
	val += GetDlgState(dlgId, DLG_DTPL_CONDS_CK) ? FF_UFF_COND : 0;
	val += GetDlgState(dlgId, DLG_DTPL_CFNTS_CK) ? FF_UFF_COMBINED_FONTS : 0;
	val += GetDlgState(dlgId, DLG_DTPL_PAGES_CK) ? FF_UFF_PAGE : 0;
	val += GetDlgState(dlgId, DLG_DTPL_REFPGS_CK) ? FF_UFF_REFPAGE : 0;
	val += GetDlgState(dlgId, DLG_DTPL_VARS_CK) ? FF_UFF_VAR : 0;
	val += GetDlgState(dlgId, DLG_DTPL_COLORS_CK) ? FF_UFF_COLOR : 0;
	val += GetDlgState(dlgId, DLG_DTPL_EQUS_CK) ? FF_UFF_MATH : 0;
	val += GetDlgState(dlgId, DLG_DTPL_DOCS_CK) ? FF_UFF_DOCUMENT_PROPS : 0;
	val += GetDlgState(dlgId, DLG_DTPL_OVERS_CK) ? FF_UFF_REMOVE_EXCEPTIONS : 0;
	val += GetDlgState(dlgId, DLG_DTPL_PBRKS_CK) ? FF_UFF_REMOVE_PAGE_BREAKS : 0;
	CurrDesText.fm_tpl_flags = val;

	CurrDesText.wd_tpl_apply = GetDlgState(dlgId, DLG_DTPL_WDTPL_CK);
	CurrDesText.wd_tpl_name = GetDlgText(dlgId, DLG_DTPL_WDTPL_BOX);
	CurrDesText.wd_tpl_auto = GetDlgState(dlgId, DLG_DTPL_AUTOUP_CK);

	TextChanged = True;
	DesignerChanged = False;
}


VoidT FreeTemplateDesigner(F_ObjHandleT dlgId)
{
	/* no resources to release */
}


/* end of file m2rdtpl.c */

