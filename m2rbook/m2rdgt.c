/* m2rdgt.c is the FDK designer panel for Graphics Text
 * Last edited on 12/27/98 by Jeremy H. Griffith.
 * Copyright 1998 Omni Systems, Inc.  All Rights Reserved.
 */


#include "m2rbook.h"

/**************************************************************/
/* Graphics Text designer page */


/* m2rdgr items */

#define DLG_DGT_GTEXT_LAB 18
#define DLG_DGT_EMBED_RAD 19
#define DLG_DGT_FRAME_RAD 20

#define DLG_DGT_CMOVE_RAD 23
#define DLG_DGT_CSHOW_RAD 24
#define DLG_DGT_CDEL_RAD  25

#define DLG_DGT_CLIP_LIM  27

#define DLG_DGT_TX_SCALE  30
#define DLG_DGT_TX_VERT   32

#define DLG_DGT_TX_WID_CK 33
#define DLG_DGT_TX_WIDTH  34

#define DLG_DGT_TRANS_RAD 37
#define DLG_DGT_OPAQ_RAD  38

#define DLG_DGT_TOP_SP_CK 39
#define DLG_DGT_TOP_BL_CK 40


/* graphic text dialog events */

VoidT ProcGraphicsTextDesignerEvent(F_ObjHandleT dlgId, IntT itemNum)
{
	switch (itemNum) {

		case DLG_DGT_EMBED_RAD:
		case DLG_DGT_FRAME_RAD:
			DesignerChanged = True;
			break;

		case DLG_DGT_CMOVE_RAD:
		case DLG_DGT_CSHOW_RAD:
		case DLG_DGT_CDEL_RAD:
			DesignerChanged = True;
			break;

		case DLG_DGT_CLIP_LIM:
			DesignerChanged = True;
			break;

		case DLG_DGT_TX_SCALE:
		case DLG_DGT_TX_VERT:
			DesignerChanged = True;
			break;

		case DLG_DGT_TX_WID_CK:
			EnableDlgItem(dlgId, DLG_DGT_TX_WIDTH,
													 GetDlgState(dlgId, itemNum));
			DesignerChanged = True;
			break;
		case DLG_DGT_TX_WIDTH:
			DesignerChanged = True;
			break;

		case DLG_DGT_TRANS_RAD:
		case DLG_DGT_OPAQ_RAD:
			DesignerChanged = True;
			break;

		case DLG_DGT_TOP_SP_CK:
		case DLG_DGT_TOP_BL_CK:
			DesignerChanged = True;
			break;

		default:
			break;
	}
}


VoidT SetGraphicsTextDesigner(F_ObjHandleT dlgId)
{

	SetDlgState(dlgId, (CurrDesGraph.graph_text == 0) ? DLG_DGT_EMBED_RAD :
																											DLG_DGT_FRAME_RAD, True);

	SetDlgState(dlgId, DLG_DGT_CMOVE_RAD + CurrDesGraph.clip_type, True);
	SetDlgText(dlgId, DLG_DGT_CLIP_LIM, CurrDesGraph.clip_limit);

	SetDlgText(dlgId, DLG_DGT_TX_SCALE, CurrDesGraph.text_scale);
	SetDlgText(dlgId, DLG_DGT_TX_VERT, CurrDesGraph.text_vert);

	if (!F_StrCmp(CurrDesGraph.text_width, "0")) {
		SetDlgText(dlgId, DLG_DGT_TX_WIDTH, "35");
		SetDlgState(dlgId, DLG_DGT_TX_WID_CK, False);
	}
	else {
		SetDlgText(dlgId, DLG_DGT_TX_WIDTH, CurrDesGraph.text_width);
		SetDlgState(dlgId, DLG_DGT_TX_WID_CK, True);
	}

	SetDlgState(dlgId, DLG_DGT_TRANS_RAD + CurrDesGraph.back_mode, True);

	SetDlgState(dlgId, DLG_DGT_TOP_SP_CK, CurrDesGraph.sp_above);
	SetDlgState(dlgId, DLG_DGT_TOP_BL_CK, CurrDesGraph.blank_first);

	DesignerChanged = False;
}


VoidT GetGraphicsTextDesigner(F_ObjHandleT dlgId)
{
	CurrDesGraph.graph_text = GetDlgState(dlgId, DLG_DGT_EMBED_RAD) ? 0 : 1;

	CurrDesGraph.clip_type = GetRadValue(dlgId, DLG_DGT_CMOVE_RAD,
																								DLG_DGT_CDEL_RAD);
	CurrDesGraph.clip_limit = GetDlgText(dlgId, DLG_DGT_CLIP_LIM);

	CurrDesGraph.text_scale = GetDlgText(dlgId, DLG_DGT_TX_SCALE);
	CurrDesGraph.text_vert = GetDlgText(dlgId, DLG_DGT_TX_VERT);

	CurrDesGraph.text_width = GetDlgState(dlgId, DLG_DGT_TX_WID_CK) ?
														GetDlgText(dlgId, DLG_DGT_TX_WIDTH) : "0";
	
	CurrDesGraph.back_mode = GetDlgState(dlgId, DLG_DGT_TRANS_RAD) ? 0 : 1;

	CurrDesGraph.sp_above = GetDlgState(dlgId, DLG_DGT_TOP_SP_CK);
	CurrDesGraph.blank_first = GetDlgState(dlgId, DLG_DGT_TOP_BL_CK);

	GraphicsChanged = True;
	DesignerChanged = False;
}


VoidT FreeGraphicsTextDesigner(F_ObjHandleT dlgId)
{
	/* no resources to release */
}


/* end of file m2rdgt.c */

