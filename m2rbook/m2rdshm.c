/* m2rdshm.c is the FDK designer panel for Help Macro
 * Last edited on 12/24/98 by Jeremy H. Griffith.
 * Copyright 1998 Omni Systems, Inc.  All Rights Reserved.
 */


#include "m2rbook.h"


/**************************************************************/
/* Help Macro designer page */


/* m2rdshm items */

#define DLG_DSHM_MACRO_LAB      18
#define DLG_DSHM_MACRO_MBOX     19

#define DLG_DSHM_BUILD_CK       20
#define DLG_DSHM_BUILD_BOX      21

#define DLG_DSHM_GREEN_CK       23
#define DLG_DSHM_ULINE_CK       24


/* help macro dialog events */

VoidT ProcHelpMacroDesignerEvent(F_ObjHandleT dlgId, IntT itemNum)
{
	switch (itemNum) {

		case DLG_DSHM_MACRO_MBOX:
			DesignerChanged = True;
			break;

		case DLG_DSHM_BUILD_CK:
			EnableDlgItem(dlgId, DLG_DSHM_BUILD_BOX,
													 GetDlgState(dlgId, itemNum));
			DesignerChanged = True;
			break;

		case DLG_DSHM_BUILD_BOX:
			DesignerChanged = True;
			break;

		case DLG_DSHM_GREEN_CK:
			if (GetDlgState(dlgId, itemNum))
				SetDlgState(dlgId, DLG_DSHM_ULINE_CK, True);
			DesignerChanged = True;
			break;

		case DLG_DSHM_ULINE_CK:
			if (!GetDlgState(dlgId, itemNum))
				SetDlgState(dlgId, DLG_DSHM_GREEN_CK, False);
			DesignerChanged = True;
			break;

		default:
			break;
	}
}


VoidT SetHelpMacroDesigner(F_ObjHandleT dlgId)
{
	if (CurrDesStyle->remap
	 || CurrDesStyle->replace)
		EnableItems(dlgId, DLG_DSHM_MACRO_LAB, DLG_DSHM_ULINE_CK, False);
	else if (CurrDesStyle->helptyp == 1) {
		SetDlgLabel(dlgId, DLG_DSHM_MACRO_LAB, TopicEntryMacro);
		EnableItems(dlgId, DLG_DSHM_MACRO_LAB, DLG_DSHM_BUILD_CK, True);
		EnableDlgItem(dlgId, DLG_DSHM_BUILD_BOX, CurrDesStyle->build);
		EnableItems(dlgId, DLG_DSHM_GREEN_CK, DLG_DSHM_ULINE_CK, False);
	}
	else if (CurrDesStyle->helptyp == 4) {
		SetDlgLabel(dlgId, DLG_DSHM_MACRO_LAB, MacroHotspot);
		EnableItems(dlgId, DLG_DSHM_MACRO_LAB, DLG_DSHM_ULINE_CK, True);
		EnableItems(dlgId, DLG_DSHM_BUILD_CK, DLG_DSHM_BUILD_BOX, False);
	}
	else
		EnableItems(dlgId, DLG_DSHM_MACRO_LAB, DLG_DSHM_ULINE_CK, False);

	SetDlgText(dlgId, DLG_DSHM_MACRO_MBOX, CurrDesStyle->mac_name);

	SetDlgState(dlgId, DLG_DSHM_BUILD_CK, CurrDesStyle->build);
	SetDlgText(dlgId, DLG_DSHM_BUILD_BOX, CurrDesStyle->build_tag);

	SetDlgState(dlgId, DLG_DSHM_GREEN_CK, !CurrDesStyle->ungreen);
	SetDlgState(dlgId, DLG_DSHM_ULINE_CK, !CurrDesStyle->nouline);
}


VoidT GetHelpMacroDesigner(F_ObjHandleT dlgId)
{
	CurrDesStyle->mac_name = GetDlgText(dlgId, DLG_DSHM_MACRO_MBOX);

	CurrDesStyle->build = GetDlgState(dlgId, DLG_DSHM_BUILD_CK);
	CurrDesStyle->build_tag = GetDlgText(dlgId, DLG_DSHM_BUILD_BOX);

	CurrDesStyle->ungreen = !GetDlgState(dlgId, DLG_DSHM_GREEN_CK);
	CurrDesStyle->nouline = !GetDlgState(dlgId, DLG_DSHM_ULINE_CK);

	StylesChanged = True;
}


VoidT FreeHelpMacroDesigner(F_ObjHandleT dlgId)
{
	/* no resources to release */
}


/* end of file m2rdshm.c */

