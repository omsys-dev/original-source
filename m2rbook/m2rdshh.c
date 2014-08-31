/* m2rdshh.c is the FDK designer panel for Hotspots
 * Last edited on 12/24/98 by Jeremy H. Griffith.
 * Copyright 1998 Omni Systems, Inc.  All Rights Reserved.
 */


#include "m2rbook.h"


/**************************************************************/
/* Hotspot designer page */


/* m2rdshh items */

#define DLG_DSHH_JUMPHOT_RAD   18
#define DLG_DSHH_POPHOT_RAD    19
#define DLG_DSHH_WINOVER_RAD   20
#define DLG_DSHH_POPOVER_RAD   21

#define DLG_DSHH_WINDOW_CK     23
#define DLG_DSHH_WINDOW_BOX    24

#define DLG_DSHH_FILE_CK       25
#define DLG_DSHH_FILE_BOX      26

#define DLG_DSHH_LOCAL_CK      27

#define DLG_DSHH_REFER_RAD     29
#define DLG_DSHH_REF_SX_BOX    31
#define DLG_DSHH_FIXED_RAD     32
#define DLG_DSHH_FIXED_BOX     33

#define DLG_DSHH_GREEN_CK      35
#define DLG_DSHH_ULINE_CK      36



/* help hotspot dialog events */

VoidT ProcHelpHotspotDesignerEvent(F_ObjHandleT dlgId, IntT itemNum)
{
	switch (itemNum) {
		case DLG_DSHH_JUMPHOT_RAD:
			if (GetDlgState(dlgId, itemNum)) {
				EnableItems(dlgId, DLG_DSHH_WINDOW_CK, DLG_DSHH_ULINE_CK, True);
				EnableDlgItem(dlgId, DLG_DSHH_WINDOW_BOX, GetDlgState(dlgId, DLG_DSHH_WINDOW_CK));
				EnableDlgItem(dlgId, DLG_DSHH_FILE_BOX, GetDlgState(dlgId, DLG_DSHH_FILE_CK));
				EnableDlgItem(dlgId, GetDlgState(dlgId, DLG_DSHH_FIXED_RAD) ? DLG_DSHH_FIXED_BOX :
																								DLG_DSHH_REF_SX_BOX, True);
			}
			DesignerChanged = True;
			break;
		case DLG_DSHH_POPHOT_RAD:
			if (GetDlgState(dlgId, itemNum)) {
				EnableItems(dlgId, DLG_DSHH_WINDOW_CK, DLG_DSHH_ULINE_CK, True);
				SetDlgState(dlgId, DLG_DSHH_WINDOW_CK, False);
				EnableDlgItem(dlgId, DLG_DSHH_WINDOW_CK, False);
				EnableDlgItem(dlgId, DLG_DSHH_WINDOW_BOX, False);
				EnableDlgItem(dlgId, DLG_DSHH_FILE_BOX, GetDlgState(dlgId, DLG_DSHH_FILE_CK));
				EnableDlgItem(dlgId, GetDlgState(dlgId, DLG_DSHH_FIXED_RAD) ? DLG_DSHH_FIXED_BOX :
																								DLG_DSHH_REF_SX_BOX, True);
			}
			DesignerChanged = True;
			break;
		case DLG_DSHH_WINOVER_RAD:
			if (GetDlgState(dlgId, itemNum)) {
				SetDlgState(dlgId, DLG_DSHH_WINDOW_CK, True);
				EnableDlgItem(dlgId, DLG_DSHH_WINDOW_CK, True);
				EnableDlgItem(dlgId, DLG_DSHH_WINDOW_BOX, True);
				EnableItems(dlgId, DLG_DSHH_FILE_CK, DLG_DSHH_ULINE_CK, False);
			}
			DesignerChanged = True;
			break;
		case DLG_DSHH_POPOVER_RAD:
			if (GetDlgState(dlgId, itemNum)) {
				EnableItems(dlgId, DLG_DSHH_WINDOW_CK, DLG_DSHH_ULINE_CK, False);
				SetDlgState(dlgId, DLG_DSHH_WINDOW_CK, False);
			}
			DesignerChanged = True;
			break;

		case DLG_DSHH_WINDOW_CK:
			EnableDlgItem(dlgId, DLG_DSHH_WINDOW_BOX,
													 GetDlgState(dlgId, itemNum));
			DesignerChanged = True;
			break;
		case DLG_DSHH_WINDOW_BOX:
			DesignerChanged = True;
			break;

		case DLG_DSHH_FILE_CK:
			EnableDlgItem(dlgId, DLG_DSHH_FILE_BOX,
													 GetDlgState(dlgId, itemNum));
			DesignerChanged = True;
			break;
		case DLG_DSHH_FILE_BOX:
			DesignerChanged = True;
			break;

		case DLG_DSHH_LOCAL_CK:
			DesignerChanged = True;
			break;

		case DLG_DSHH_REFER_RAD:
			EnableDlgItem(dlgId, DLG_DSHH_REF_SX_BOX,
													 GetDlgState(dlgId, itemNum));
			EnableDlgItem(dlgId, DLG_DSHH_FIXED_BOX,
													 GetDlgState(dlgId, DLG_DSHH_FIXED_RAD));
			DesignerChanged = True;
			break;
		case DLG_DSHH_REF_SX_BOX:
			DesignerChanged = True;
			break;

		case DLG_DSHH_FIXED_RAD:
			EnableDlgItem(dlgId, DLG_DSHH_FIXED_BOX,
													 GetDlgState(dlgId, itemNum));
			EnableDlgItem(dlgId, DLG_DSHH_REF_SX_BOX,
													 GetDlgState(dlgId, DLG_DSHH_REFER_RAD));
			DesignerChanged = True;
			break;
		case DLG_DSHH_FIXED_BOX:
			DesignerChanged = True;
			break;

		case DLG_DSHH_GREEN_CK:
			if (GetDlgState(dlgId, itemNum))
				SetDlgState(dlgId, DLG_DSHH_ULINE_CK, True);
			DesignerChanged = True;
			break;

		case DLG_DSHH_ULINE_CK:
			if (!GetDlgState(dlgId, itemNum))
				SetDlgState(dlgId, DLG_DSHH_GREEN_CK, False);
			DesignerChanged = True;
			break;

		default:
			break;
	}
}


VoidT SetHelpHotspotDesigner(F_ObjHandleT dlgId)
{
	if ((CurrDesStyle->helptyp != 2)
	 || CurrDesStyle->remap
	 || CurrDesStyle->replace)
		EnableItems(dlgId, DLG_DSHH_JUMPHOT_RAD, DLG_DSHH_ULINE_CK, False);
	else if (CurrDesStyle->hotsptyp == 2) {
		EnableItems(dlgId, DLG_DSHH_JUMPHOT_RAD, DLG_DSHH_WINDOW_BOX, True);
		EnableItems(dlgId, DLG_DSHH_FILE_CK, DLG_DSHH_ULINE_CK, False);
	}
	else if (CurrDesStyle->hotsptyp == 3) {
		EnableItems(dlgId, DLG_DSHH_JUMPHOT_RAD, DLG_DSHH_POPOVER_RAD, True);
		EnableItems(dlgId, DLG_DSHH_WINDOW_CK, DLG_DSHH_ULINE_CK, False);
	}
	else {
		EnableItems(dlgId, DLG_DSHH_JUMPHOT_RAD, DLG_DSHH_ULINE_CK, True);
		EnableDlgItem(dlgId, DLG_DSHH_WINDOW_BOX, CurrDesStyle->window);
		EnableDlgItem(dlgId, DLG_DSHH_FILE_BOX, CurrDesStyle->fileref);
		EnableDlgItem(dlgId, DLG_DSHH_REF_SX_BOX, !CurrDesStyle->fixed);
		EnableDlgItem(dlgId, DLG_DSHH_FIXED_BOX, CurrDesStyle->fixed);
	}

	SetDlgState(dlgId, DLG_DSHH_JUMPHOT_RAD + CurrDesStyle->hotsptyp, True);

	SetDlgState(dlgId, DLG_DSHH_WINDOW_CK, CurrDesStyle->window);
	SetDlgText(dlgId, DLG_DSHH_WINDOW_BOX, CurrDesStyle->win_name);

	SetDlgState(dlgId, DLG_DSHH_FILE_CK, CurrDesStyle->fileref);
	SetDlgText(dlgId, DLG_DSHH_FILE_BOX, CurrDesStyle->file_name);

	SetDlgState(dlgId, DLG_DSHH_LOCAL_CK, CurrDesStyle->local);

	SetDlgState(dlgId, CurrDesStyle->fixed ? DLG_DSHH_FIXED_RAD:
																					 DLG_DSHH_REFER_RAD, True);
	SetDlgText(dlgId, DLG_DSHH_REF_SX_BOX, CurrDesStyle->ref_sx);
	SetDlgText(dlgId, DLG_DSHH_FIXED_BOX, CurrDesStyle->fixed_name);

	SetDlgState(dlgId, DLG_DSHH_GREEN_CK, !CurrDesStyle->ungreen);
	SetDlgState(dlgId, DLG_DSHH_ULINE_CK, !CurrDesStyle->nouline);
}


VoidT GetHelpHotspotDesigner(F_ObjHandleT dlgId)
{
	CurrDesStyle->hotsptyp = GetRadValue(dlgId, DLG_DSHH_JUMPHOT_RAD,
																							DLG_DSHH_POPOVER_RAD);

	CurrDesStyle->window = GetDlgState(dlgId, DLG_DSHH_WINDOW_CK);
	CurrDesStyle->win_name = GetDlgText(dlgId, DLG_DSHH_WINDOW_BOX);

	CurrDesStyle->fileref = GetDlgState(dlgId, DLG_DSHH_FILE_CK);
	CurrDesStyle->file_name = GetDlgText(dlgId, DLG_DSHH_FILE_BOX);

	CurrDesStyle->local = GetDlgState(dlgId, DLG_DSHH_LOCAL_CK);

	CurrDesStyle->fixed = GetDlgState(dlgId, DLG_DSHH_FIXED_RAD);
	CurrDesStyle->ref_sx = GetDlgText(dlgId, DLG_DSHH_REF_SX_BOX);
	CurrDesStyle->fixed_name = GetDlgText(dlgId, DLG_DSHH_FIXED_BOX);

	CurrDesStyle->ungreen = !GetDlgState(dlgId, DLG_DSHH_GREEN_CK);
	CurrDesStyle->nouline = !GetDlgState(dlgId, DLG_DSHH_ULINE_CK);

	StylesChanged = True;
}


VoidT FreeHelpHotspotDesigner(F_ObjHandleT dlgId)
{
	/* no resources to release */
}


/* end of file m2rdshh.c */

