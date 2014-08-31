/* m2rdgr.c is the FDK designer panel for Basic Graphics
 * Last edited on 1/1/99 by Jeremy H. Griffith.
 * Copyright 1998, 1999 Omni Systems, Inc.  All Rights Reserved.
 */


#include "m2rbook.h"

/**************************************************************/
/* Basic Graphics designer page */


/* m2rdgr items */

#define DLG_DGR_FPRV_CK   18
#define DLG_DGR_FPRV_DPI  19

#define DLG_DGR_FEQN_CK   20
#define DLG_DGR_FEQN_PCT  21
#define DLG_DGR_FEQN_DPI  23

#define DLG_DGR_FPRV_LIST 25

#define DLG_DGR_INTRN_RAD 28
#define DLG_DGR_EXTRN_RAD 29

#define DLG_DGR_FEXACT_CK 30
#define DLG_DGR_EQN_VERT  32
#define DLG_DGR_GR_VERT   34

#define DLG_DGR_NM_BMP_CK 35
#define DLG_DGR_NMCHARS   37
#define DLG_DGR_NMDIGITS  39
#define DLG_DGR_NMD_LAB   40


/* text layout dialog events */

VoidT ProcBasicGraphicsDesignerEvent(F_ObjHandleT dlgId, IntT itemNum)
{
	switch (itemNum) {

		case DLG_DGR_FPRV_CK:
			EnableDlgItem(dlgId, DLG_DGR_FPRV_LIST, 
															GetDlgState(dlgId, itemNum)
													 || GetDlgState(dlgId, DLG_DGR_FEQN_CK));
			EnableDlgItem(dlgId, DLG_DGR_FPRV_DPI, GetDlgState(dlgId, itemNum));
			DesignerChanged = True;
			break;

		case DLG_DGR_FPRV_LIST:
		case DLG_DGR_FPRV_DPI:
			DesignerChanged = True;
			break;

		case DLG_DGR_FEQN_CK:
			EnableDlgItem(dlgId, DLG_DGR_FPRV_LIST, 
															GetDlgState(dlgId, itemNum)
													 || GetDlgState(dlgId, DLG_DGR_FPRV_CK));
			EnableDlgItem(dlgId, DLG_DGR_FEQN_PCT, GetDlgState(dlgId, itemNum));
			EnableDlgItem(dlgId, DLG_DGR_FEQN_DPI, GetDlgState(dlgId, itemNum));
			DesignerChanged = True;
			break;

		case DLG_DGR_FEQN_PCT:
		case DLG_DGR_FEQN_DPI:
			DesignerChanged = True;
			break;


		case DLG_DGR_INTRN_RAD:
			if (CurrPropSetNum == 0)
				EnableItems(dlgId, DLG_DGR_FEXACT_CK, DLG_DGR_GR_VERT, True);
			EnableItems(dlgId, DLG_DGR_NM_BMP_CK, DLG_DGR_NMD_LAB, False);
			DesignerChanged = True;
			break;

		case DLG_DGR_EXTRN_RAD:
			if (CurrPropSetNum == 0)
				EnableItems(dlgId, DLG_DGR_FEXACT_CK, DLG_DGR_GR_VERT, False);
			EnableItems(dlgId, DLG_DGR_NM_BMP_CK, DLG_DGR_NMD_LAB, True);
			DesignerChanged = True;
			break;

		case DLG_DGR_FEXACT_CK:
		case DLG_DGR_EQN_VERT:
		case DLG_DGR_GR_VERT:
			DesignerChanged = True;
			break;

		case DLG_DGR_NM_BMP_CK:
		case DLG_DGR_NMCHARS:
		case DLG_DGR_NMDIGITS:
			DesignerChanged = True;
			break;

		default:
			break;
	}
}


VoidT SetBasicGraphicsDesigner(F_ObjHandleT dlgId)
{
	EnableItems(dlgId, DLG_DGR_FEXACT_CK, DLG_DGR_GR_VERT, (CurrPropSetNum == 0)
																											&& !CurrDesGraph.external);
	EnableItems(dlgId, DLG_DGR_NM_BMP_CK, DLG_DGR_NMD_LAB, CurrDesGraph.external);

	SetDlgState(dlgId, DLG_DGR_FPRV_CK, CurrDesGraph.fdkprv_used);
	SetDlgText(dlgId, DLG_DGR_FPRV_DPI, CurrDesGraph.fdkprv_dpi);
	EnableDlgItem(dlgId, DLG_DGR_FPRV_DPI, CurrDesGraph.fdkprv_used);

	SetLabels(dlgId, DLG_DGR_FPRV_LIST, &GrFormStrings);
	SetGrFormListState(dlgId, DLG_DGR_FPRV_LIST, CurrDesGraph.fdkgr_form);
	EnableDlgItem(dlgId, DLG_DGR_FPRV_LIST,
											 CurrDesGraph.fdkprv_used
										|| CurrDesGraph.fdkeqn_used);

	SetDlgState(dlgId, DLG_DGR_FEQN_CK, CurrDesGraph.fdkeqn_used);
	SetDlgText(dlgId, DLG_DGR_FEQN_PCT, CurrDesGraph.fdkeqn_frm ? 
																			CurrDesGraph.fdkeqn_frm : "125");
	SetDlgText(dlgId, DLG_DGR_FEQN_DPI, CurrDesGraph.fdkeqn_dpi ? 
																			CurrDesGraph.fdkeqn_dpi : "120");
	EnableDlgItem(dlgId, DLG_DGR_FEQN_PCT, CurrDesGraph.fdkeqn_used);
	EnableDlgItem(dlgId, DLG_DGR_FEQN_DPI, CurrDesGraph.fdkeqn_used);

	SetDlgState(dlgId, CurrDesGraph.external ? DLG_DGR_EXTRN_RAD :
																						 DLG_DGR_INTRN_RAD, True);

	SetDlgState(dlgId, DLG_DGR_FEXACT_CK, CurrDesGraph.fexact);
	SetDlgText(dlgId, DLG_DGR_EQN_VERT, CurrDesGraph.eqn_vert);
	SetDlgText(dlgId, DLG_DGR_GR_VERT, CurrDesGraph.gr_vert);

	SetDlgState(dlgId, DLG_DGR_NM_BMP_CK, CurrDesGraph.name_bmp);
	SetDlgText(dlgId, DLG_DGR_NMCHARS, CurrDesGraph.wmfname_chars);
	SetDlgText(dlgId, DLG_DGR_NMDIGITS, CurrDesGraph.wmfname_digits);

	DesignerChanged = False;
}


VoidT GetBasicGraphicsDesigner(F_ObjHandleT dlgId)
{
	CurrDesGraph.fdkprv_used = GetDlgState(dlgId, DLG_DGR_FPRV_CK);
	CurrDesGraph.fdkprv_dpi = GetDlgText(dlgId, DLG_DGR_FPRV_DPI);

	CurrDesGraph.fdkgr_form = GetGrFormListText(dlgId, DLG_DGR_FPRV_LIST);

	CurrDesGraph.fdkeqn_used = GetDlgState(dlgId, DLG_DGR_FEQN_CK);
	CurrDesGraph.fdkeqn_frm = GetDlgText(dlgId, DLG_DGR_FEQN_PCT);
	CurrDesGraph.fdkeqn_dpi = GetDlgText(dlgId, DLG_DGR_FEQN_DPI);

	CurrDesGraph.external = GetDlgState(dlgId, DLG_DGR_EXTRN_RAD);

	CurrDesGraph.fexact = GetDlgState(dlgId, DLG_DGR_FEXACT_CK);
	CurrDesGraph.eqn_vert = GetDlgText(dlgId, DLG_DGR_EQN_VERT);
	CurrDesGraph.gr_vert = GetDlgText(dlgId, DLG_DGR_GR_VERT);

	CurrDesGraph.name_bmp = GetDlgState(dlgId, DLG_DGR_NM_BMP_CK);
	CurrDesGraph.wmfname_chars = GetDlgText(dlgId, DLG_DGR_NMCHARS);
	CurrDesGraph.wmfname_digits = GetDlgText(dlgId, DLG_DGR_NMDIGITS);

	GraphicsChanged = True;
	DesignerChanged = False;
}


VoidT FreeBasicGraphicsDesigner(F_ObjHandleT dlgId)
{
	/* no resources to release */
}


/* end of file m2rdgr.c */

