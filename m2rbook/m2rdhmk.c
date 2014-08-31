/* m2rdhmk.c is the FDK designer panel for Help Marker
 * Last edited on 1/9/99 by Jeremy H. Griffith.
 * Copyright 1998, 1999 Omni Systems, Inc.  All Rights Reserved.
 */


#include "m2rbook.h"


/**************************************************************/
/* Help Marker designer page */


/* m2rdhmk items */

#define DLG_DHMK_AL_LAB   18
#define DLG_DHMK_AL0_RAD  19
#define DLG_DHMK_AL4_RAD  20
#define DLG_DHMK_AL5_RAD  21
#define DLG_DHMK_AL6_RAD  22
#define DLG_DHMK_ALC_RAD  23
#define DLG_DHMK_ALC_BOX  24

#define DLG_DHMK_T11N_RAD 27
#define DLG_DHMK_T11M_RAD 28
#define DLG_DHMK_T11F_RAD 29

#define DLG_DHMK_DM0_RAD  32
#define DLG_DHMK_DM3_RAD  33
#define DLG_DHMK_DMC_RAD  34
#define DLG_DHMK_DMC_BOX  35

#define DLG_DHMK_NXPOP_CK 37



/* Help Marker dialog events */

VoidT ProcHelpMarkerDesignerEvent(F_ObjHandleT dlgId, IntT itemNum)
{
	switch (itemNum) {

		case DLG_DHMK_AL0_RAD:
		case DLG_DHMK_AL4_RAD:
		case DLG_DHMK_AL5_RAD:
		case DLG_DHMK_AL6_RAD:
			EnableDlgItem(dlgId, DLG_DHMK_ALC_BOX, False);
			DesignerChanged = True;
			break;

		case DLG_DHMK_ALC_RAD:
			EnableDlgItem(dlgId, DLG_DHMK_ALC_BOX, True);
			DesignerChanged = True;
			break;

		case DLG_DHMK_ALC_BOX:
			DesignerChanged = True;
			break;

		case DLG_DHMK_T11N_RAD:
		case DLG_DHMK_T11M_RAD:
		case DLG_DHMK_T11F_RAD:
			DesignerChanged = True;
			break;

		case DLG_DHMK_DM0_RAD:
		case DLG_DHMK_DM3_RAD:
			EnableDlgItem(dlgId, DLG_DHMK_DMC_BOX, False);
			DesignerChanged = True;
			break;

		case DLG_DHMK_DMC_RAD:
			EnableDlgItem(dlgId, DLG_DHMK_DMC_BOX, True);
			DesignerChanged = True;
			break;

		case DLG_DHMK_DMC_BOX:
			DesignerChanged = True;
			break;

		case DLG_DHMK_NXPOP_CK:
			DesignerChanged = True;
			break;

		default:
			break;
	}
}


VoidT SetHelpMarkerDesigner(F_ObjHandleT dlgId)
{
	if (CurrPropSetNum == DES_PROPSET_HELP) {
		EnableItems(dlgId, DLG_DHMK_AL_LAB, DLG_DHMK_T11F_RAD, True);
		EnableDlgItem(dlgId, DLG_DHMK_ALC_BOX, (CurrDesText.alink_mark == 4));
		EnableDlgItem(dlgId, DLG_DHMK_NXPOP_CK, True);
	}
	else {
		EnableItems(dlgId, DLG_DHMK_AL_LAB, DLG_DHMK_T11F_RAD, False);
		EnableDlgItem(dlgId, DLG_DHMK_NXPOP_CK, False);
	}

	EnableDlgItem(dlgId, DLG_DHMK_DMC_BOX, (CurrDesText.directive_mark == 2));

	SetDlgState(dlgId, DLG_DHMK_AL0_RAD + CurrDesText.alink_mark, True);
	SetDlgText(dlgId, DLG_DHMK_ALC_BOX, CurrDesText.alink_cust);

	SetDlgState(dlgId, DLG_DHMK_T11N_RAD + CurrDesText.typ11_mark, True);

	SetDlgState(dlgId, DLG_DHMK_DM0_RAD + CurrDesText.directive_mark, True);
	SetDlgText(dlgId, DLG_DHMK_DMC_BOX, CurrDesText.directive_cust);

	SetDlgState(dlgId, DLG_DHMK_NXPOP_CK, CurrDesText.no_xref_popup);

	DesignerChanged = False;
}


VoidT GetHelpMarkerDesigner(F_ObjHandleT dlgId)
{
	CurrDesText.alink_mark = GetRadValue(dlgId, DLG_DHMK_AL0_RAD,
																							DLG_DHMK_ALC_RAD);
	CurrDesText.alink_cust = GetDlgText(dlgId, DLG_DHMK_ALC_BOX);

	CurrDesText.typ11_mark = GetRadValue(dlgId, DLG_DHMK_T11N_RAD,
																							DLG_DHMK_T11F_RAD);

	CurrDesText.directive_mark = GetRadValue(dlgId, DLG_DHMK_DM0_RAD,
																									DLG_DHMK_DMC_RAD);
	CurrDesText.directive_cust = GetDlgText(dlgId, DLG_DHMK_DMC_BOX);

	CurrDesText.no_xref_popup = GetDlgState(dlgId, DLG_DHMK_NXPOP_CK);

	TextChanged = True;
	DesignerChanged = False;
}

VoidT FreeHelpMarkerDesigner(F_ObjHandleT dlgId)
{
	/* no resources to release */
}


/* end of file m2rdhmk.c */

