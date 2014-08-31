/* m2rdcnt.c is the FDK designer panel for Help Contents
 * Last edited on 1/5/99 by Jeremy H. Griffith.
 * Copyright 1998, 1999 Omni Systems, Inc.  All Rights Reserved.
 */


#include "m2rbook.h"


/**************************************************************/
/* Help Contents designer page */


/* m2rdcnt items */

#define DLG_DCNT_CTF_RAD  19
#define DLG_DCNT_CTB_RAD  20
#define DLG_DCNT_CTN_RAD  21

#define DLG_DCNT_CNT_CK   22
#define DLG_DCNT_CNT_BOX  23

#define DLG_DCNT_BAS_CK   24
#define DLG_DCNT_BAS_BOX  25

#define DLG_DCNT_TIT_CK   26
#define DLG_DCNT_TIT_BOX  27

#define DLG_DCNT_CSF_CK   28
#define DLG_DCNT_CSF_BOX  29

#define DLG_DCNT_FHD_CK   30
#define DLG_DCNT_FHD_BOX  31

#define DLG_DCNT_BTI_CK   32
#define DLG_DCNT_BTI_BOX  33

#define DLG_DCNT_FIL_CK   35
#define DLG_DCNT_WIN_CK   36
#define DLG_DCNT_PRI_WIN  38



/* Help Contents dialog events */

VoidT ProcHelpContentsDesignerEvent(F_ObjHandleT dlgId, IntT itemNum)
{
	switch (itemNum) {

		case DLG_DCNT_CTF_RAD:
		case DLG_DCNT_CTB_RAD:
		case DLG_DCNT_CTN_RAD:
			DesignerChanged = True;
			break;

		case DLG_DCNT_CNT_CK:
			EnableDlgItem(dlgId, DLG_DCNT_CNT_BOX,
													 GetDlgState(dlgId, itemNum));
			DesignerChanged = True;
			break;

		case DLG_DCNT_CNT_BOX:
			DesignerChanged = True;
			break;

		case DLG_DCNT_BAS_CK:
			EnableDlgItem(dlgId, DLG_DCNT_BAS_BOX,
													 GetDlgState(dlgId, itemNum));
			DesignerChanged = True;
			break;

		case DLG_DCNT_BAS_BOX:
			DesignerChanged = True;
			break;

		case DLG_DCNT_TIT_CK:
			EnableDlgItem(dlgId, DLG_DCNT_TIT_BOX,
													 GetDlgState(dlgId, itemNum));
			DesignerChanged = True;
			break;

		case DLG_DCNT_TIT_BOX:
			DesignerChanged = True;
			break;

		case DLG_DCNT_CSF_CK:
			EnableDlgItem(dlgId, DLG_DCNT_CSF_BOX,
													 GetDlgState(dlgId, itemNum));
			DesignerChanged = True;
			break;

		case DLG_DCNT_CSF_BOX:
			DesignerChanged = True;
			break;

		case DLG_DCNT_FHD_CK:
			EnableDlgItem(dlgId, DLG_DCNT_FHD_BOX,
													 GetDlgState(dlgId, itemNum));
			DesignerChanged = True;
			break;

		case DLG_DCNT_FHD_BOX:
			DesignerChanged = True;
			break;

		case DLG_DCNT_BTI_CK:
			EnableDlgItem(dlgId, DLG_DCNT_BTI_BOX,
													 GetDlgState(dlgId, itemNum));
			DesignerChanged = True;
			break;

		case DLG_DCNT_BTI_BOX:
			DesignerChanged = True;
			break;

		case DLG_DCNT_FIL_CK:
			DesignerChanged = True;
			break;

		case DLG_DCNT_WIN_CK:
			EnableDlgItem(dlgId, DLG_DCNT_PRI_WIN,
													 GetDlgState(dlgId, itemNum));
			DesignerChanged = True;
			break;

		case DLG_DCNT_PRI_WIN:
			DesignerChanged = True;
			break;

		default:
			break;
	}
}


VoidT SetHelpContentsDesigner(F_ObjHandleT dlgId)
{
	EnableDlgItem(dlgId, DLG_DCNT_CNT_BOX, CurrDesText.cnt_name_used);
	EnableDlgItem(dlgId, DLG_DCNT_BAS_BOX, CurrDesText.cnt_base_used);
	EnableDlgItem(dlgId, DLG_DCNT_TIT_BOX, CurrDesText.cnt_title_used);
	EnableDlgItem(dlgId, DLG_DCNT_CSF_BOX, CurrDesText.cnt_start_file_used);
	EnableDlgItem(dlgId, DLG_DCNT_FHD_BOX, CurrDesText.cnt_top_head_used);
	EnableDlgItem(dlgId, DLG_DCNT_BTI_BOX, CurrDesText.cnt_b_topic_used);
	EnableDlgItem(dlgId, DLG_DCNT_PRI_WIN, CurrDesText.cnt_wins);

	SetDlgState(dlgId, DLG_DCNT_CTF_RAD + CurrDesText.cnt_type, True);

	SetDlgState(dlgId, DLG_DCNT_CNT_CK, CurrDesText.cnt_name_used);
	SetDlgText(dlgId, DLG_DCNT_CNT_BOX, CurrDesText.cnt_name);

	SetDlgState(dlgId, DLG_DCNT_BAS_CK, CurrDesText.cnt_base_used);
	SetDlgText(dlgId, DLG_DCNT_BAS_BOX, CurrDesText.cnt_base);

	SetDlgState(dlgId, DLG_DCNT_TIT_CK, CurrDesText.cnt_title_used);
	SetDlgText(dlgId, DLG_DCNT_TIT_BOX, CurrDesText.cnt_title);

	SetDlgState(dlgId, DLG_DCNT_CSF_CK, CurrDesText.cnt_start_file_used);
	SetDlgText(dlgId, DLG_DCNT_CSF_BOX, CurrDesText.cnt_start_file);

	SetDlgState(dlgId, DLG_DCNT_FHD_CK, CurrDesText.cnt_top_head_used);
	SetDlgText(dlgId, DLG_DCNT_FHD_BOX, CurrDesText.cnt_top_head);

	SetDlgState(dlgId, DLG_DCNT_BTI_CK, CurrDesText.cnt_b_topic_used);
	SetDlgText(dlgId, DLG_DCNT_BTI_BOX, CurrDesText.cnt_b_topic);

	SetDlgState(dlgId, DLG_DCNT_FIL_CK, CurrDesText.cnt_files);
	SetDlgState(dlgId, DLG_DCNT_WIN_CK, CurrDesText.cnt_wins);
	SetDlgText(dlgId, DLG_DCNT_PRI_WIN, CurrDesText.cnt_main_win);

	DesignerChanged = False;
}


VoidT GetHelpContentsDesigner(F_ObjHandleT dlgId)
{
	CurrDesText.cnt_type = GetRadValue(dlgId, DLG_DCNT_CTF_RAD,
																						DLG_DCNT_CTN_RAD);

	CurrDesText.cnt_name_used = GetDlgState(dlgId, DLG_DCNT_CNT_CK);
	CurrDesText.cnt_name = GetDlgText(dlgId, DLG_DCNT_CNT_BOX);

	CurrDesText.cnt_base_used = GetDlgState(dlgId, DLG_DCNT_BAS_CK);
	CurrDesText.cnt_base = GetDlgText(dlgId, DLG_DCNT_BAS_BOX);

	CurrDesText.cnt_title_used = GetDlgState(dlgId, DLG_DCNT_TIT_CK);
	CurrDesText.cnt_title = GetDlgText(dlgId, DLG_DCNT_TIT_BOX);

	CurrDesText.cnt_start_file_used = GetDlgState(dlgId, DLG_DCNT_CSF_CK);
	CurrDesText.cnt_start_file = GetDlgText(dlgId, DLG_DCNT_CSF_BOX);

	CurrDesText.cnt_top_head_used = GetDlgState(dlgId, DLG_DCNT_FHD_CK);
	CurrDesText.cnt_top_head = GetDlgText(dlgId, DLG_DCNT_FHD_BOX);

	CurrDesText.cnt_b_topic_used = GetDlgState(dlgId, DLG_DCNT_BTI_CK);
	CurrDesText.cnt_b_topic = GetDlgText(dlgId, DLG_DCNT_BTI_BOX);

	CurrDesText.cnt_files = GetDlgState(dlgId, DLG_DCNT_FIL_CK);
	CurrDesText.cnt_wins = GetDlgState(dlgId, DLG_DCNT_WIN_CK);
	CurrDesText.cnt_main_win = GetDlgText(dlgId, DLG_DCNT_PRI_WIN);

	TextChanged = True;
	DesignerChanged = False;
}

VoidT FreeHelpContentsDesigner(F_ObjHandleT dlgId)
{
	/* no resources to release */
}


/* end of file m2rdcnt.c */

