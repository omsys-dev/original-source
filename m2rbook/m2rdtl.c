/* m2rdtl.c is the FDK designer panel for Text Layout
 * Last edited on 12/24/98 by Jeremy H. Griffith.
 * Copyright 1998 Omni Systems, Inc.  All Rights Reserved.
 */


#include "m2rbook.h"


/**************************************************************/
/* Text Layout designer page */


/* m2rdtl items */

#define DLG_DTL_TFLOW_LAB  18
#define DLG_DTL_TFSEP_RAD  19
#define DLG_DTL_TFNORM_RAD 20
#define DLG_DTL_TFSKIP_RAD 21
#define DLG_DTL_TFSING_CK  22

#define DLG_DTL_TFRAM_CK   24
#define DLG_DTL_TFWRAP_CK  25
#define DLG_DTL_COLUMN_CK  26
#define DLG_DTL_COLUMN_BOX 27

#define DLG_DTL_PGBKN_RAD  29
#define DLG_DTL_PGBKF_RAD  30
#define DLG_DTL_PGBKR_RAD  31

#define DLG_DTL_HFTS_RAD   34
#define DLG_DTL_HFTT_RAD   35
#define DLG_DTL_HFTN_RAD   36

#define DLG_DTL_HFVS_BOX   39

#define DLG_DTL_HFRAM_CK   40
#define DLG_DTL_HFWRAP_CK  41
#define DLG_DTL_HFRAM_BOX  43


/* text layout dialog events */

VoidT ProcTextLayoutDesignerEvent(F_ObjHandleT dlgId, IntT itemNum)
{
	switch (itemNum) {

		case DLG_DTL_TFSEP_RAD:
		case DLG_DTL_TFNORM_RAD:
		case DLG_DTL_TFSKIP_RAD:
			DesignerChanged = True;
			break;

		case DLG_DTL_TFSING_CK:
			EnableItems(dlgId, DLG_DTL_TFLOW_LAB, DLG_DTL_TFSKIP_RAD,
													 !GetDlgState(dlgId, itemNum));
			DesignerChanged = True;
			break;

		case DLG_DTL_TFRAM_CK:
			EnableDlgItem(dlgId, DLG_DTL_TFWRAP_CK,
													 GetDlgState(dlgId, itemNum));
			DesignerChanged = True;
			break;

		case DLG_DTL_TFWRAP_CK:
			DesignerChanged = True;
			break;

		case DLG_DTL_COLUMN_CK:
			EnableDlgItem(dlgId, DLG_DTL_COLUMN_BOX,
													 GetDlgState(dlgId, itemNum));
			DesignerChanged = True;
			break;

		case DLG_DTL_COLUMN_BOX:
			DesignerChanged = True;
			break;

		case DLG_DTL_PGBKN_RAD:
		case DLG_DTL_PGBKF_RAD:
		case DLG_DTL_PGBKR_RAD:
			DesignerChanged = True;
			break;

		case DLG_DTL_HFTS_RAD:
		case DLG_DTL_HFTT_RAD:
			EnableItems(dlgId, DLG_DTL_HFVS_BOX, DLG_DTL_HFRAM_BOX, True);
			EnableItems(dlgId, DLG_DTL_HFWRAP_CK, DLG_DTL_HFRAM_BOX,
													 GetDlgState(dlgId, DLG_DTL_HFRAM_CK));
			DesignerChanged = True;
			break;

		case DLG_DTL_HFTN_RAD:
			EnableItems(dlgId, DLG_DTL_HFVS_BOX, DLG_DTL_HFRAM_BOX, False);
			DesignerChanged = True;
			break;

		case DLG_DTL_HFRAM_CK:
			EnableItems(dlgId, DLG_DTL_HFWRAP_CK, DLG_DTL_HFRAM_BOX,
													 GetDlgState(dlgId, itemNum));
			DesignerChanged = True;
			break;

		case DLG_DTL_HFWRAP_CK:
		case DLG_DTL_HFRAM_BOX:
			DesignerChanged = True;
			break;

		case DLG_DTL_HFVS_BOX:
			DesignerChanged = True;
			break;

		default:
			break;
	}
}

VoidT SetTextLayoutDesigner(F_ObjHandleT dlgId)
{
	EnableItems(dlgId, DLG_DTL_TFRAM_CK, DLG_DTL_HFRAM_BOX, (CurrPropSetNum == 0));
	if (CurrPropSetNum == 0) {
		EnableDlgItem(dlgId, DLG_DTL_TFWRAP_CK, CurrDesText.text_frames);
		EnableDlgItem(dlgId, DLG_DTL_COLUMN_BOX, (*CurrDesText.tcols != '0'));
		EnableItems(dlgId, DLG_DTL_HFVS_BOX, DLG_DTL_HFRAM_BOX,
											(CurrDesText.hd_ft != 2));
		EnableItems(dlgId, DLG_DTL_HFWRAP_CK, DLG_DTL_HFRAM_BOX, 
											CurrDesText.hf_framed);
	}

	SetDlgState(dlgId, DLG_DTL_TFSEP_RAD + CurrTextFlow->status, True);
	SetDlgState(dlgId, DLG_DTL_TFSING_CK, CurrDesText.single_flow);

	SetDlgState(dlgId, DLG_DTL_TFRAM_CK, CurrDesText.text_frames);
	SetDlgState(dlgId, DLG_DTL_TFWRAP_CK, CurrDesText.tframe_wrap);

	SetDlgState(dlgId, DLG_DTL_COLUMN_CK, (*CurrDesText.tcols != '0'));
	SetDlgText(dlgId, DLG_DTL_COLUMN_BOX, CurrDesText.tcols);

	SetDlgState(dlgId, DLG_DTL_PGBKN_RAD + CurrDesText.pg_break, True);

	SetDlgState(dlgId, DLG_DTL_HFTS_RAD + CurrDesText.hd_ft, True);
	SetDlgText(dlgId, DLG_DTL_HFVS_BOX, CurrDesText.hdft_vs);

	SetDlgState(dlgId, DLG_DTL_HFRAM_CK, CurrDesText.hf_framed);
	SetDlgState(dlgId, DLG_DTL_HFWRAP_CK, CurrDesText.hfframe_wrap);
	SetDlgText(dlgId, DLG_DTL_HFRAM_BOX, CurrDesText.hfframe_sp);

	DesignerChanged = False;
}


VoidT GetTextLayoutDesigner(F_ObjHandleT dlgId)
{
	CurrTextFlow->status = GetRadValue(dlgId, DLG_DTL_TFSEP_RAD,
																						DLG_DTL_TFSKIP_RAD);

	CurrDesText.single_flow = GetDlgState(dlgId, DLG_DTL_TFSING_CK);

	CurrDesText.text_frames = GetDlgState(dlgId, DLG_DTL_TFRAM_CK);
	CurrDesText.tframe_wrap = GetDlgState(dlgId, DLG_DTL_TFWRAP_CK);

	if (GetDlgState(dlgId, DLG_DTL_COLUMN_CK))
		CurrDesText.tcols = GetDlgText(dlgId, DLG_DTL_COLUMN_BOX);
	else
		CurrDesText.tcols = "0";

	CurrDesText.pg_break = GetRadValue(dlgId, DLG_DTL_PGBKN_RAD,
																						DLG_DTL_PGBKR_RAD);

	CurrDesText.hd_ft = GetRadValue(dlgId, DLG_DTL_HFTS_RAD,
																				 DLG_DTL_HFTN_RAD);
	CurrDesText.hdft_vs = GetDlgText(dlgId, DLG_DTL_HFVS_BOX);

	CurrDesText.hf_framed = GetDlgState(dlgId, DLG_DTL_HFRAM_CK);
	CurrDesText.hfframe_wrap = GetDlgState(dlgId, DLG_DTL_HFWRAP_CK);
	CurrDesText.hfframe_sp = GetDlgText(dlgId, DLG_DTL_HFRAM_BOX);

	TextChanged = True;
	TextFlowChanged = True;
	DesignerChanged = False;
}

VoidT FreeTextLayoutDesigner(F_ObjHandleT dlgId)
{
	/* no resources to release */
}


/* end of file m2rdtl.c */

