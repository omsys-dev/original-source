/* m2rdtxb.c is the FDK designer panel for Basic Text
 * Last edited on 12/24/98 by Jeremy H. Griffith.
 * Copyright 1998 Omni Systems, Inc.  All Rights Reserved.
 */


#include "m2rbook.h"


/**************************************************************/
/* Basic Text designer page */


/* m2rdtxb items */

#define DLG_DTXB_WHTX_CK 18
#define DLG_DTXB_COLR_CK 19
#define DLG_DTXB_HCOL_CK 20
#define DLG_DTXB_ANCH_CK 21
#define DLG_DTXB_CHST_CK 23
#define DLG_DTXB_XLSP_CK 24
#define DLG_DTXB_SCAP_CK 25
#define DLG_DTXB_RUNIN_CK 26

#define DLG_DTXB_RMLT_CK 29
#define DLG_DTXB_RMRT_CK 30

#define DLG_DTXB_SHDL_RAD 33
#define DLG_DTXB_SHDI_RAD 34
#define DLG_DTXB_SHDF_RAD 35
#define DLG_DTXB_SHDN_RAD 36

#define DLG_DTXB_SHSP_CK  37

#define DLG_DTXB_SHVS_LAB 39
#define DLG_DTXB_SHVS_BOX 40

#define DLG_DTXB_FNS_RAD 43
#define DLG_DTXB_FNE_RAD 44
#define DLG_DTXB_FNN_RAD 45
#define DLG_DTXB_FNJ_RAD 46
#define DLG_DTXB_FNP_RAD 47
#define DLG_DTXB_FNV_RAD 48


/* basic text dialog events */

VoidT ProcBasicTextDesignerEvent(F_ObjHandleT dlgId, IntT itemNum)
{
	DesignerChanged = True; // no special enabling needed during use
}

VoidT SetBasicTextDesigner(F_ObjHandleT dlgId)
{
	EnableDlgItem(dlgId, DLG_DTXB_HCOL_CK, (CurrPropSetNum == DES_PROPSET_HELP));
	EnableDlgItem(dlgId, DLG_DTXB_FNJ_RAD, (CurrPropSetNum == DES_PROPSET_HELP));
	EnableDlgItem(dlgId, DLG_DTXB_FNP_RAD, (CurrPropSetNum == DES_PROPSET_HELP));
	EnableDlgItem(dlgId, DLG_DTXB_CHST_CK, (CurrPropSetNum == DES_PROPSET_WORD));
	EnableDlgItem(dlgId, DLG_DTXB_SHDF_RAD, (CurrPropSetNum == DES_PROPSET_WORD));
	EnableDlgItem(dlgId, DLG_DTXB_SHSP_CK, (CurrPropSetNum == DES_PROPSET_WORD));
	EnableDlgItem(dlgId, DLG_DTXB_SHVS_LAB, (CurrPropSetNum == DES_PROPSET_WORD));
	EnableDlgItem(dlgId, DLG_DTXB_SHVS_BOX, (CurrPropSetNum == DES_PROPSET_WORD));
	EnableDlgItem(dlgId, DLG_DTXB_FNV_RAD, (CurrPropSetNum == DES_PROPSET_WORD));

	SetDlgState(dlgId, DLG_DTXB_WHTX_CK, CurrDesText.hide_white);
	SetDlgState(dlgId, DLG_DTXB_COLR_CK, CurrDesText.keep_color);
	SetDlgState(dlgId, DLG_DTXB_HCOL_CK, CurrDesText.hyper_color);
	SetDlgState(dlgId, DLG_DTXB_ANCH_CK, CurrDesText.keep_anchor);
	SetDlgState(dlgId, DLG_DTXB_CHST_CK, CurrDesText.char_styles_used);
	SetDlgState(dlgId, DLG_DTXB_XLSP_CK, CurrDesText.exact_space);
	SetDlgState(dlgId, DLG_DTXB_SCAP_CK, CurrDesText.small_caps ? True : False);
	SetDlgState(dlgId, DLG_DTXB_RUNIN_CK, CurrDesText.runin_heads ? True : False);

	SetDlgState(dlgId, DLG_DTXB_RMLT_CK, (CurrDesText.rm_tabs & 1) == 1);
	SetDlgState(dlgId, DLG_DTXB_RMRT_CK, (CurrDesText.rm_tabs & 2) == 2);

	SetDlgState(dlgId, DLG_DTXB_SHDL_RAD + CurrDesText.sidehds, True);
	SetDlgState(dlgId, DLG_DTXB_SHSP_CK, CurrDesText.anchor_spanners);
	SetDlgText(dlgId, DLG_DTXB_SHVS_BOX, CurrDesText.sidehd_vs);

	SetDlgState(dlgId, DLG_DTXB_FNS_RAD + CurrDesText.fnotes, True);

	DesignerChanged = False;
}

VoidT GetBasicTextDesigner(F_ObjHandleT dlgId)
{
	CurrDesText.hide_white = GetDlgState(dlgId, DLG_DTXB_WHTX_CK);
	CurrDesText.keep_color = GetDlgState(dlgId, DLG_DTXB_COLR_CK);
	CurrDesText.hyper_color = GetDlgState(dlgId, DLG_DTXB_HCOL_CK);
	CurrDesText.keep_anchor = GetDlgState(dlgId, DLG_DTXB_ANCH_CK);
	CurrDesText.char_styles_used = GetDlgState(dlgId, DLG_DTXB_CHST_CK);
	CurrDesText.exact_space = GetDlgState(dlgId, DLG_DTXB_XLSP_CK);
	CurrDesText.small_caps = GetDlgState(dlgId, DLG_DTXB_SCAP_CK) ? 1 : 0;
	CurrDesText.runin_heads = GetDlgState(dlgId, DLG_DTXB_RUNIN_CK) ? 1 : 0;

	CurrDesText.rm_tabs = (GetDlgState(dlgId, DLG_DTXB_RMLT_CK) ? 1 : 0)
											+ (GetDlgState(dlgId, DLG_DTXB_RMRT_CK) ? 2 : 0);

	CurrDesText.sidehds = GetRadValue(dlgId, DLG_DTXB_SHDL_RAD,
																					 DLG_DTXB_SHDN_RAD);
	CurrDesText.anchor_spanners = GetDlgState(dlgId, DLG_DTXB_SHSP_CK);
	CurrDesText.sidehd_vs = GetDlgText(dlgId, DLG_DTXB_SHVS_BOX);

	CurrDesText.fnotes = GetRadValue(dlgId, DLG_DTXB_FNS_RAD,
																					DLG_DTXB_FNV_RAD);
	TextChanged = True;
	DesignerChanged = False;
}

VoidT FreeBasicTextDesigner(F_ObjHandleT dlgId)
{
	/* no resources to release */
}


/* end of file m2rdtxb.c */

