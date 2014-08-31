/* m2rdfnt.c is the FDK designer panel for Fonts
 * Last edited on 12/28/98 by Jeremy H. Griffith.
 * Copyright 1998 Omni Systems, Inc.  All Rights Reserved.
 */


#include "m2rbook.h"


/**************************************************************/
/* Font designer page */


/* m2rdfnt items */

#define DLG_DFNT_RMAP_CK   18
#define DLG_DFNT_RMAP_LIST 19
#define DLG_DFNT_FTYP0_RAD 21
#define DLG_DFNT_FTYP6_RAD 27
#define DLG_DFNT_FWID_BOX  30

#define DLG_DFNT_DEF_FWID  33
#define DLG_DFNT_DEF_LSPC  35
#define DLG_DFNT_DEF_LIST  37
#define DLG_DFNT_DEF_FSIZ  39

#define DLG_DFNT_WMF_LIST  42
#define DLG_DFNT_WMF_FSIZ  44

#define DLG_DFNT_BMC_CK    46
#define DLG_DFNT_BMC_LIST  47

#define DLG_DFNT_UNUSED_CK 48


/* font dialog events */

VoidT ProcFontDesignerEvent(F_ObjHandleT dlgId, IntT itemNum)
{
	switch (itemNum) {

		case DLG_DFNT_RMAP_CK:
			EnableDlgItem(dlgId, DLG_DFNT_RMAP_LIST,
													 GetDlgState(dlgId, itemNum));
			DesignerChanged = True;
			break;
		case DLG_DFNT_RMAP_LIST:
			DesignerChanged = True;
			break;

		case DLG_DFNT_FTYP0_RAD:
		case DLG_DFNT_FTYP6_RAD:
			DesignerChanged = True;
			break;

		case DLG_DFNT_FWID_BOX:
			DesignerChanged = True;
			break;

		case DLG_DFNT_DEF_FWID:
		case DLG_DFNT_DEF_LSPC:
		case DLG_DFNT_DEF_LIST:
		case DLG_DFNT_DEF_FSIZ:
			DesignerChanged = True;
			break;

		case DLG_DFNT_WMF_LIST:
		case DLG_DFNT_WMF_FSIZ:
			DesignerChanged = True;
			break;

		case DLG_DFNT_BMC_CK:
			EnableDlgItem(dlgId, DLG_DFNT_BMC_LIST,
													 GetDlgState(dlgId, itemNum));
			DesignerChanged = True;
			break;
		case DLG_DFNT_BMC_LIST:
			DesignerChanged = True;
			break;

		case DLG_DFNT_UNUSED_CK:
			DesignerChanged = True;
			break;

		default:
			break;
	}
}


VoidT SetFontDesigner(F_ObjHandleT dlgId)
{
	SetDlgState(dlgId, DLG_DFNT_RMAP_CK, CurrFont->remap);

	SetFontListState(dlgId, DLG_DFNT_RMAP_LIST, CurrFont->rem_name);
	EnableDlgItem(dlgId, DLG_DFNT_RMAP_LIST, CurrFont->remap);

	SetDlgState(dlgId, DLG_DFNT_FTYP0_RAD + CurrFont->ftyp, True);
	SetDlgText(dlgId, DLG_DFNT_FWID_BOX, CurrFont->fwid);

	SetDlgText(dlgId, DLG_DFNT_DEF_FWID, CurrDesText.def_fwid);
	SetDlgText(dlgId, DLG_DFNT_DEF_LSPC, CurrDesText.def_lspace);
	SetFontListState(dlgId, DLG_DFNT_DEF_LIST, CurrDesText.def_font);
	SetDlgText(dlgId, DLG_DFNT_DEF_FSIZ, CurrDesText.def_fsize);

	SetFontListState(dlgId, DLG_DFNT_WMF_LIST, CurrDesText.wmf_font);
	SetDlgText(dlgId, DLG_DFNT_WMF_FSIZ, CurrDesText.wmf_fsize);

	if (CurrPropSetNum == DES_PROPSET_HELP) {
		SetDlgState(dlgId, DLG_DFNT_BMC_CK, CurrDesText.bmc_used);
		SetFontListState(dlgId, DLG_DFNT_BMC_LIST, CurrDesText.bmc_font);
		EnableDlgItem(dlgId, DLG_DFNT_BMC_LIST, CurrDesText.bmc_used);
	}
	else
		EnableItems(dlgId, DLG_DFNT_BMC_CK, DLG_DFNT_BMC_LIST, False);

	SetDlgState(dlgId, DLG_DFNT_UNUSED_CK, CurrDesText.rem_unused_fonts);

	DesignerChanged = False;
	FontListChanged = False;
}


VoidT GetFontDesigner(F_ObjHandleT dlgId)
{
	CurrFont->remap = GetDlgState(dlgId, DLG_DFNT_RMAP_CK);
	CurrFont->rem_name = GetFontListText(dlgId, DLG_DFNT_RMAP_LIST);
	CurrFont->ftyp = GetRadValue(dlgId, DLG_DFNT_FTYP0_RAD,
																			DLG_DFNT_FTYP6_RAD);
	CurrFont->fwid = GetDlgText(dlgId, DLG_DFNT_FWID_BOX);

	CurrDesText.def_fwid = GetDlgText(dlgId, DLG_DFNT_DEF_FWID);
	CurrDesText.def_lspace = GetDlgText(dlgId, DLG_DFNT_DEF_LSPC);
	CurrDesText.def_font = GetFontListText(dlgId, DLG_DFNT_DEF_LIST);
	CurrDesText.def_fsize = GetDlgText(dlgId, DLG_DFNT_DEF_FSIZ);

	CurrDesText.wmf_font = GetFontListText(dlgId, DLG_DFNT_WMF_LIST);
	CurrDesText.wmf_fsize = GetDlgText(dlgId, DLG_DFNT_WMF_FSIZ);

	CurrDesText.bmc_used = GetDlgState(dlgId, DLG_DFNT_BMC_CK);
	CurrDesText.bmc_font = GetFontListText(dlgId, DLG_DFNT_BMC_LIST);

	CurrDesText.rem_unused_fonts = GetDlgState(dlgId, DLG_DFNT_UNUSED_CK);

	TextChanged = True;
	FontChanged = True;
	DesignerChanged = False;
}


VoidT FreeFontDesigner(F_ObjHandleT dlgId)
{
	/* no resources to release */
}


/* end of file m2rdfnt.c */

