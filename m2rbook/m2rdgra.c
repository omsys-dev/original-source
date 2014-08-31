/* m2rdgra.c is the FDK designer panel for Graphic Frames
 * Last edited on 1/13/99 by Jeremy H. Griffith.
 * Copyright 1998, 1999 Omni Systems, Inc.  All Rights Reserved.
 */


#include "m2rbook.h"


#ifdef REFF_USED
/* Reference Frame functions */

RefFrameT *CurrRefFrame = NULL;


#endif

/**************************************************************/
/* Graphic Frames designer page */

BoolT RefFrameChanged = False;


/* m2rdgra items */

#define DLG_DGRA_RF0_RAD 18
#define DLG_DGRA_RF1_RAD 19
#define DLG_DGRA_RF2_RAD 20
#define DLG_DGRA_RST_LAB 21
#define DLG_DGRA_RST_LIST 22

#define DLG_DGRA_RD0_RAD 25
#define DLG_DGRA_RD1_RAD 26
#define DLG_DGRA_RD2_RAD 27
#define DLG_DGRA_RDS_LAB 28
#define DLG_DGRA_RDS_LIST 29

#define DLG_DGRA_FST_CK  31
#define DLG_DGRA_FST_LIST 32

#define DLG_DGRA_EMF_LAB 33
#define DLG_DGRA_EMFS_RAD 34
#define DLG_DGRA_EMFR_RAD 35
#define DLG_DGRA_EMFI_RAD 36

#define DLG_DGRA_BDN_RAD 38
#define DLG_DGRA_BDO_RAD 39
#define DLG_DGRA_BDI_RAD 40
#define DLG_DGRA_BDF_RAD 41


/* Graphic Frames dialog events */

VoidT ProcGraphicFramesDesignerEvent(F_ObjHandleT dlgId, IntT itemNum)
{
	StringT bmc_val = NULL;

	switch (itemNum) {

#ifdef REFF_USED

		case DLG_DGRA_RF0_RAD:
		case DLG_DGRA_RF1_RAD:
			EnableItems(dlgId, DLG_DGRA_RST_LAB, DLG_DGRA_RST_LIST, False);
			RefFrameChanged = True;
			DesignerChanged = True;
			break;

		case DLG_DGRA_RF2_RAD:
			EnableItems(dlgId, DLG_DGRA_RST_LAB, DLG_DGRA_RST_LIST, True);
			RefFrameChanged = True;
			DesignerChanged = True;
			break;

		case DLG_DGRA_RST_LIST:
			RefFrameChanged = True;
			DesignerChanged = True;
			break;

#endif

		case DLG_DGRA_RD0_RAD:
		case DLG_DGRA_RD1_RAD:
		case DLG_DGRA_RD2_RAD:
			DesignerChanged = True;
			break;

		case DLG_DGRA_RDS_LIST:
			DesignerChanged = True;
			break;

		case DLG_DGRA_FST_CK:
			EnableDlgItem(dlgId, DLG_DGRA_FST_LIST,
													 GetDlgState(dlgId, itemNum));
			DesignerChanged = True;
			break;

		case DLG_DGRA_FST_LIST:
			DesignerChanged = True;
			break;

		case DLG_DGRA_EMFS_RAD:
		case DLG_DGRA_EMFR_RAD:
		case DLG_DGRA_EMFI_RAD:
			DesignerChanged = True;
			break;

		case DLG_DGRA_BDN_RAD:
		case DLG_DGRA_BDO_RAD:
		case DLG_DGRA_BDI_RAD:
		case DLG_DGRA_BDF_RAD:
			DesignerChanged = True;
			break;

		default:
			break;
	}
}


VoidT SetGraphicFramesDesigner(F_ObjHandleT dlgId)
{
#ifdef REFF_USED
	EnableItems(dlgId, DLG_DGRA_RST_LAB, DLG_DGRA_RST_LIST, (CurrRefFrame->typ == 2));

	SetDlgState(dlgId, DLG_DGRA_RF0_RAD + CurrRefFrame->typ, True);
	SetLabels(dlgId, DLG_DGRA_RST_LIST, &StyleStrings);
	SetStyleListState(dlgId, DLG_DGRA_RST_LIST, CurrRefFrame->form);
#else
	EnableItems(dlgId, DLG_DGRA_RF0_RAD, DLG_DGRA_RST_LIST, False);
#endif

	EnableItems(dlgId, DLG_DGRA_RDS_LAB, DLG_DGRA_RDS_LIST, True);

	SetDlgState(dlgId, DLG_DGRA_RD0_RAD + CurrDesGraph.refframe_typ, True);
	SetLabels(dlgId, DLG_DGRA_RDS_LIST, &StyleStrings);
	SetStyleListState(dlgId, DLG_DGRA_RDS_LIST, CurrDesGraph.refframe_form);

	SetLabels(dlgId, DLG_DGRA_FST_LIST, &StyleStrings);
	SetStyleListState(dlgId, DLG_DGRA_FST_LIST, CurrDesGraph.frstyle_name);
	EnableDlgItem(dlgId, DLG_DGRA_FST_LIST, CurrDesGraph.frstyle_used);
	SetDlgState(dlgId, DLG_DGRA_FST_CK, CurrDesGraph.frstyle_used);

	SetDlgState(dlgId, DLG_DGRA_EMFS_RAD + CurrDesGraph.empty_frames, True);

	SetDlgState(dlgId, DLG_DGRA_BDN_RAD + CurrDesGraph.ras_borders, True);

	RefFrameChanged = False;
	DesignerChanged = False;
}


VoidT GetGraphicFramesDesigner(F_ObjHandleT dlgId)
{
#ifdef REFF_USED
	CurrRefFrame->typ = GetRadValue(dlgId, DLG_DGRA_RF0_RAD,
																					DLG_DGRA_RF2_RAD);
	CurrRefFrame->form = GetStyleListText(dlgId, DLG_DGRA_RST_LIST);
#endif

	CurrDesGraph.refframe_typ = GetRadValue(dlgId, DLG_DGRA_RD0_RAD,
																								 DLG_DGRA_RD2_RAD);
	CurrDesGraph.refframe_form = GetStyleListText(dlgId, DLG_DGRA_RDS_LIST);

	if ((CurrDesGraph.frstyle_used = GetDlgState(dlgId, DLG_DGRA_FST_CK)) == True)
		CurrDesGraph.frstyle_name = GetStyleListText(dlgId, DLG_DGRA_FST_LIST);
	else
		CurrDesGraph.frstyle_name = NULL;

	CurrDesGraph.empty_frames = GetRadValue(dlgId, DLG_DGRA_EMFS_RAD,
																								 DLG_DGRA_EMFI_RAD);

	CurrDesGraph.ras_borders = GetRadValue(dlgId, DLG_DGRA_BDN_RAD,
																							 DLG_DGRA_BDF_RAD);

	GraphicsChanged = True;
	DesignerChanged = False;
}


VoidT FreeGraphicFramesDesigner(F_ObjHandleT dlgId)
{
	/* no resources to release */
}


/* end of file m2rdgra.c */

