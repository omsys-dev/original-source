/* m2rdtxh.c is the FDK designer panel for Help Text
 * Last edited on 1/4/99 by Jeremy H. Griffith.
 * Copyright 1998, 1999 Omni Systems, Inc.  All Rights Reserved.
 */


#include "m2rbook.h"


/**************************************************************/
/* Help Text designer page */


/* m2rdtxh items */

#define DLG_DTXH_NSCR_CK 18
#define DLG_DTXH_XNSC_CK 19

#define DLG_DTXH_NTIN_CK 20
#define DLG_DTXH_NTSP_CK 21

#define DLG_DTXH_TBRK_CK 22

#define DLG_DTXH_BR_DIGS 25
#define DLG_DTXH_BR_STEP 27
#define DLG_DTXH_BR_ST   29
#define DLG_DTXH_BR_PX   31

#define DLG_DTXH_BULBMPS 33
#define DLG_DTXH_BULFILE 34
#define DLG_DTXH_BULDING 35

#define DLG_DTXH_KEY_MAX 37
#define DLG_DTXH_TAB_MAX 39


/* Help text dialog events */

VoidT ProcHelpTextDesignerEvent(F_ObjHandleT dlgId, IntT itemNum)
{
	switch (itemNum) {
		case DLG_DTXH_NSCR_CK:
		case DLG_DTXH_XNSC_CK:

		case DLG_DTXH_NTIN_CK:
		case DLG_DTXH_NTSP_CK:

		case DLG_DTXH_TBRK_CK:

		case DLG_DTXH_BR_DIGS:
		case DLG_DTXH_BR_STEP:
		case DLG_DTXH_BR_ST:
		case DLG_DTXH_BR_PX:

		case DLG_DTXH_BULBMPS:
		case DLG_DTXH_BULFILE:
		case DLG_DTXH_BULDING:

		case DLG_DTXH_KEY_MAX:
		case DLG_DTXH_TAB_MAX:
			DesignerChanged = True;
			break;

		default:
			break;
	}
}

VoidT SetHelpTextDesigner(F_ObjHandleT dlgId)
{
	SetDlgState(dlgId, DLG_DTXH_NSCR_CK, !CurrDesText.scroll_title);
	SetDlgState(dlgId, DLG_DTXH_XNSC_CK, CurrDesText.xnscroll_title);
	SetDlgState(dlgId, DLG_DTXH_NTIN_CK, !CurrDesText.ind_title);
	SetDlgState(dlgId, DLG_DTXH_NTSP_CK, !CurrDesText.space_title);
	SetDlgState(dlgId, DLG_DTXH_TBRK_CK, CurrDesText.linebk_title);

	SetDlgText(dlgId, DLG_DTXH_BR_DIGS, CurrDesText.browse_digits);
	SetDlgText(dlgId, DLG_DTXH_BR_STEP, CurrDesText.browse_step);
	SetDlgText(dlgId, DLG_DTXH_BR_ST, CurrDesText.browse_start);
	SetDlgText(dlgId, DLG_DTXH_BR_PX, CurrDesText.browse_prefix);

	SetDlgState(dlgId, DLG_DTXH_BULBMPS, CurrDesText.bullet_bmc);
	SetDlgText(dlgId, DLG_DTXH_BULFILE, CurrDesText.bullet_file);
	SetDlgState(dlgId, DLG_DTXH_BULDING, CurrDesText.dingbat_bmp);

	SetDlgText(dlgId, DLG_DTXH_KEY_MAX, CurrDesText.keyword_limit);
	SetDlgText(dlgId, DLG_DTXH_TAB_MAX, CurrDesText.help_tab_limit);

	DesignerChanged = False;
}

VoidT GetHelpTextDesigner(F_ObjHandleT dlgId)
{
	CurrDesText.scroll_title = !GetDlgState(dlgId, DLG_DTXH_NSCR_CK);
	CurrDesText.xnscroll_title = GetDlgState(dlgId, DLG_DTXH_XNSC_CK);
	CurrDesText.ind_title = !GetDlgState(dlgId, DLG_DTXH_NTIN_CK);
	CurrDesText.space_title = !GetDlgState(dlgId, DLG_DTXH_NTSP_CK);
	CurrDesText.linebk_title = GetDlgState(dlgId, DLG_DTXH_TBRK_CK);

	CurrDesText.browse_digits = GetDlgText(dlgId, DLG_DTXH_BR_DIGS);
	CurrDesText.browse_step = GetDlgText(dlgId, DLG_DTXH_BR_STEP);
	CurrDesText.browse_start = GetDlgText(dlgId, DLG_DTXH_BR_ST);
	CurrDesText.browse_prefix = GetDlgText(dlgId, DLG_DTXH_BR_PX);

	CurrDesText.bullet_bmc = GetDlgState(dlgId, DLG_DTXH_BULBMPS);
	CurrDesText.bullet_file = GetDlgText(dlgId, DLG_DTXH_BULFILE);
	CurrDesText.dingbat_bmp = GetDlgState(dlgId, DLG_DTXH_BULDING);

	CurrDesText.keyword_limit = GetDlgText(dlgId, DLG_DTXH_KEY_MAX);
	CurrDesText.help_tab_limit = GetDlgText(dlgId, DLG_DTXH_TAB_MAX);

	TextChanged = True;
	DesignerChanged = False;
}

VoidT FreeHelpTextDesigner(F_ObjHandleT dlgId)
{
	/* no resources to release */
}


/* end of file m2rdtxh.c */

