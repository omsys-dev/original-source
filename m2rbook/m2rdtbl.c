/* m2rdtbl.c is the FDK designer panel for Tables
 * Last edited on 12/27/98 by Jeremy H. Griffith.
 * Copyright 1998 Omni Systems, Inc.  All Rights Reserved.
 */


#include "m2rbook.h"


/**************************************************************/
/* Table designer page */


/* m2rdtbl items */

#define DLG_DTBL_TITA_RAD 19
#define DLG_DTBL_TITT_RAD 20
#define DLG_DTBL_TITB_RAD 21

#define DLG_DTBL_COL_PCT  24
#define DLG_DTBL_COL_ADD  27

#define DLG_DTBL_RULC_RAD 31
#define DLG_DTBL_RULN_RAD 32
#define DLG_DTBL_RULV_RAD 33
#define DLG_DTBL_RULB_RAD 34
#define DLG_DTBL_RULD_RAD 35
#define DLG_DTBL_RULT_RAD 36
#define DLG_DTBL_RULS_RAD 37
#define DLG_DTBL_RULP_RAD 38

#define DLG_DTBL_TGRS_RAD 41
#define DLG_DTBL_TGRN_RAD 42
#define DLG_DTBL_TGRO_RAD 43

#define DLG_DTBL_STRIP_CK 45
#define DLG_DTBL_CONT_CK  46
#define DLG_DTBL_CONT_VAR 47

#define DLG_DTBL_SHEET_CK  48
#define DLG_DTBL_SHEET_VAR 49


/* Table dialog events */

VoidT ProcTableDesignerEvent(F_ObjHandleT dlgId, IntT itemNum)
{
	switch (itemNum) {

		case DLG_DTBL_TITA_RAD:
		case DLG_DTBL_TITT_RAD:
		case DLG_DTBL_TITB_RAD:

		case DLG_DTBL_COL_PCT:
		case DLG_DTBL_COL_ADD:

		case DLG_DTBL_RULC_RAD:
		case DLG_DTBL_RULN_RAD:
		case DLG_DTBL_RULV_RAD:
		case DLG_DTBL_RULB_RAD:
		case DLG_DTBL_RULD_RAD:
		case DLG_DTBL_RULT_RAD:
		case DLG_DTBL_RULS_RAD:
		case DLG_DTBL_RULP_RAD:

		case DLG_DTBL_TGRS_RAD:
		case DLG_DTBL_TGRN_RAD:
		case DLG_DTBL_TGRO_RAD:

		case DLG_DTBL_STRIP_CK:

		case DLG_DTBL_CONT_CK:
		case DLG_DTBL_CONT_VAR:

		case DLG_DTBL_SHEET_CK:
		case DLG_DTBL_SHEET_VAR:
			DesignerChanged = True;
			break;

		default:
			break;
	}
}

VoidT SetTableDesigner(F_ObjHandleT dlgId)
{
	EnableDlgItem(dlgId, DLG_DTBL_RULC_RAD, (CurrPropSetNum == DES_PROPSET_WORD));
	EnableDlgItem(dlgId, DLG_DTBL_STRIP_CK, (CurrPropSetNum == DES_PROPSET_HELP));
	EnableDlgItem(dlgId, DLG_DTBL_CONT_CK, (CurrPropSetNum == DES_PROPSET_WORD));
	EnableDlgItem(dlgId, DLG_DTBL_CONT_VAR, (CurrPropSetNum == DES_PROPSET_WORD));
	EnableDlgItem(dlgId, DLG_DTBL_SHEET_CK, (CurrPropSetNum == DES_PROPSET_WORD));
	EnableDlgItem(dlgId, DLG_DTBL_SHEET_VAR, (CurrPropSetNum == DES_PROPSET_WORD));

	SetDlgState(dlgId, DLG_DTBL_TITA_RAD + CurrDesText.tbl_titles, True);

	SetDlgText(dlgId, DLG_DTBL_COL_PCT, CurrDesText.tbcol_scale);
	SetDlgText(dlgId, DLG_DTBL_COL_ADD, CurrDesText.tbcol_add);

	SetDlgState(dlgId, DLG_DTBL_RULC_RAD + CurrDesText.tbl_rules, True);
	SetDlgState(dlgId, DLG_DTBL_TGRS_RAD + CurrDesText.tbl_graphics, True);

	SetDlgState(dlgId, DLG_DTBL_STRIP_CK, CurrDesText.tbl_strip);

	SetDlgState(dlgId, DLG_DTBL_CONT_CK, CurrDesText.tbl_use_cont);
	SetDlgText(dlgId, DLG_DTBL_CONT_VAR, CurrDesText.tbl_cont_var);

	SetDlgState(dlgId, DLG_DTBL_SHEET_CK, CurrDesText.tbl_use_sheet);
	SetDlgText(dlgId, DLG_DTBL_SHEET_VAR, CurrDesText.tbl_sheet_var);

	DesignerChanged = False;
}


VoidT GetTableDesigner(F_ObjHandleT dlgId)
{
	CurrDesText.tbl_titles = GetRadValue(dlgId, DLG_DTBL_TITA_RAD,
																							DLG_DTBL_TITB_RAD);

	CurrDesText.tbcol_scale = GetDlgText(dlgId, DLG_DTBL_COL_PCT);
	CurrDesText.tbcol_add = GetDlgText(dlgId, DLG_DTBL_COL_ADD);

	CurrDesText.tbl_rules = GetRadValue(dlgId, DLG_DTBL_RULC_RAD,
																						 DLG_DTBL_RULP_RAD);

	CurrDesText.tbl_graphics = GetRadValue(dlgId, DLG_DTBL_TGRS_RAD,
																								DLG_DTBL_TGRO_RAD);

	CurrDesText.tbl_strip = GetDlgState(dlgId, DLG_DTBL_STRIP_CK);

	CurrDesText.tbl_use_cont = GetDlgState(dlgId, DLG_DTBL_CONT_CK);
	CurrDesText.tbl_cont_var = GetDlgText(dlgId, DLG_DTBL_CONT_VAR);

	CurrDesText.tbl_use_sheet = GetDlgState(dlgId, DLG_DTBL_SHEET_CK);
	CurrDesText.tbl_sheet_var = GetDlgText(dlgId, DLG_DTBL_SHEET_VAR);

	TextChanged = True;
	DesignerChanged = False;
}


VoidT FreeTableDesigner(F_ObjHandleT dlgId)
{
	/* no resources to release */
}


/* end of file m2rdtbl.c */

