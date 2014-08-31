/* m2rdsht.c is the FDK designer panel for Targets
 * Last edited on 12/24/98 by Jeremy H. Griffith.
 * Copyright 1998 Omni Systems, Inc.  All Rights Reserved.
 */


#include "m2rbook.h"


/**************************************************************/
/* Target designer page */


/* m2rdsht items */

#define DLG_DSHT_JUMPTARG_RAD   18
#define DLG_DSHT_POPCONT_RAD    19

#define DLG_DSHT_LOCAL_CK       21

#define DLG_DSHT_REFER_CK       22
#define DLG_DSHT_REF_SX_BOX     23

#define DLG_DSHT_CONT_CK        25
#define DLG_DSHT_CONT_V_RAD     27
#define DLG_DSHT_CONT_H_RAD     28
#define DLG_DSHT_CONT_T_RAD     29
#define DLG_DSHT_CONT_B_RAD     30
#define DLG_DSHT_CONT_LEV_BOX   32



/* help target dialog events */

VoidT ProcHelpTargetDesignerEvent(F_ObjHandleT dlgId, IntT itemNum)
{
	switch (itemNum) {

		case DLG_DSHT_JUMPTARG_RAD:
		case DLG_DSHT_POPCONT_RAD:
			DesignerChanged = True;
			break;

		case DLG_DSHT_LOCAL_CK:
			DesignerChanged = True;
			break;

		case DLG_DSHT_REFER_CK:
			EnableDlgItem(dlgId, DLG_DSHT_REF_SX_BOX,
													 GetDlgState(dlgId, itemNum));
			DesignerChanged = True;
			break;

		case DLG_DSHT_REF_SX_BOX:
			DesignerChanged = True;
			break;

		case DLG_DSHT_CONT_CK:
			EnableItems(dlgId, DLG_DSHT_CONT_V_RAD, DLG_DSHT_CONT_B_RAD,
													 GetDlgState(dlgId, itemNum));
			EnableDlgItem(dlgId, DLG_DSHT_CONT_LEV_BOX,
													 GetDlgState(dlgId, itemNum));
			DesignerChanged = True;
			break;
		case DLG_DSHT_CONT_V_RAD:
		case DLG_DSHT_CONT_H_RAD:
		case DLG_DSHT_CONT_T_RAD:
		case DLG_DSHT_CONT_B_RAD:
		case DLG_DSHT_CONT_LEV_BOX:
			DesignerChanged = True;
			break;

		default:
			break;
	}
}


VoidT SetHelpTargetDesigner(F_ObjHandleT dlgId)
{
	if ((CurrDesStyle->helptyp != 3)
	 || CurrDesStyle->remap
	 || CurrDesStyle->replace)
		EnableItems(dlgId, DLG_DSHT_JUMPTARG_RAD, DLG_DSHT_CONT_LEV_BOX, False);
	else {
		EnableItems(dlgId, DLG_DSHT_JUMPTARG_RAD, DLG_DSHT_CONT_LEV_BOX, True);
		EnableDlgItem(dlgId, DLG_DSHT_REF_SX_BOX, CurrDesStyle->refer);
		EnableItems(dlgId, DLG_DSHT_CONT_V_RAD, DLG_DSHT_CONT_B_RAD,
											 CurrDesStyle->cont);
		EnableDlgItem(dlgId, DLG_DSHT_CONT_LEV_BOX, CurrDesStyle->cont);
	}

	SetDlgState(dlgId, CurrDesStyle->pop_cont ? DLG_DSHT_POPCONT_RAD :
																						 DLG_DSHT_JUMPTARG_RAD, True);

	SetDlgState(dlgId, DLG_DSHT_LOCAL_CK, CurrDesStyle->local);

	SetDlgState(dlgId, DLG_DSHT_REFER_CK, CurrDesStyle->refer);
	SetDlgText(dlgId, DLG_DSHT_REF_SX_BOX, CurrDesStyle->ref_sx);

	SetDlgState(dlgId, DLG_DSHT_CONT_CK, CurrDesStyle->cont);
	SetDlgState(dlgId, DLG_DSHT_CONT_V_RAD + CurrDesStyle->conttyp, True);
	SetDlgText(dlgId, DLG_DSHT_CONT_LEV_BOX, CurrDesStyle->contlev);
}


VoidT GetHelpTargetDesigner(F_ObjHandleT dlgId)
{
	CurrDesStyle->pop_cont = GetDlgState(dlgId, DLG_DSHT_POPCONT_RAD);

	CurrDesStyle->local = GetDlgState(dlgId, DLG_DSHT_LOCAL_CK);

	CurrDesStyle->refer = GetDlgState(dlgId, DLG_DSHT_REFER_CK);
	CurrDesStyle->ref_sx = GetDlgText(dlgId, DLG_DSHT_REF_SX_BOX);

	CurrDesStyle->cont = GetDlgState(dlgId, DLG_DSHT_CONT_CK);
	CurrDesStyle->conttyp = GetRadValue(dlgId, DLG_DSHT_CONT_V_RAD,
																						 DLG_DSHT_CONT_B_RAD);
	CurrDesStyle->contlev = GetDlgText(dlgId, DLG_DSHT_CONT_LEV_BOX);

	StylesChanged = True;
}


VoidT FreeHelpTargetDesigner(F_ObjHandleT dlgId)
{
	/* no resources to release */
}


/* end of file m2rdsht.c */

