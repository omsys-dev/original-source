/* m2rdshs.c is the FDK designer panel for Help Topic
 * Last edited on 12/24/98 by Jeremy H. Griffith.
 * Copyright 1998 Omni Systems, Inc.  All Rights Reserved.
 */


#include "m2rbook.h"


/**************************************************************/
/* Help Topic designer page */


/* m2rdshs items */

#define DLG_DSHS_NORMAL_RAD   18
#define DLG_DSHS_SLIDER_RAD   19
#define DLG_DSHS_RESUME_RAD   20

#define DLG_DSHS_BROWSE_CK    22
#define DLG_DSHS_BR_PX_BOX    24

#define DLG_DSHS_CONT_CK      25
#define DLG_DSHS_CONT_V_RAD   27
#define DLG_DSHS_CONT_H_RAD   28
#define DLG_DSHS_CONT_T_RAD   29
#define DLG_DSHS_CONT_B_RAD   30
#define DLG_DSHS_CONT_LEV_BOX 32

#define DLG_DSHS_WINDOW_CK    33
#define DLG_DSHS_WINDOW_BOX   34

#define DLG_DSHS_NO_TITLE_CK  35
#define DLG_DSHS_TITLE_SX_CK  36
#define DLG_DSHS_TITLE_SX_BOX 37

#define DLG_DSHS_REFER_CK     38
#define DLG_DSHS_REF_SX_BOX   40

#define DLG_DSHS_NSCR_CK3     42
#define DLG_DSHS_XNSCR_CK3    43


/* help topic dialog events */

VoidT ProcHelpTopicDesignerEvent(F_ObjHandleT dlgId, IntT itemNum)
{
	switch (itemNum) {
		case DLG_DSHS_NORMAL_RAD:
		case DLG_DSHS_SLIDER_RAD:
			EnableItems(dlgId, DLG_DSHS_BROWSE_CK, DLG_DSHS_XNSCR_CK3, True);
			EnableDlgItem(dlgId, DLG_DSHS_BR_PX_BOX, CurrDesStyle->browse);
			EnableItems(dlgId, DLG_DSHS_CONT_V_RAD, DLG_DSHS_CONT_B_RAD,
												 CurrDesStyle->cont);
			EnableDlgItem(dlgId, DLG_DSHS_CONT_LEV_BOX, CurrDesStyle->cont);
			EnableDlgItem(dlgId, DLG_DSHS_WINDOW_BOX, CurrDesStyle->window);
			EnableDlgItem(dlgId, DLG_DSHS_TITLE_SX_BOX,
										CurrDesStyle->titsx && !CurrDesStyle->notitle);
			EnableDlgItem(dlgId, DLG_DSHS_TITLE_SX_CK, !CurrDesStyle->notitle);
			EnableDlgItem(dlgId, DLG_DSHS_REF_SX_BOX, CurrDesStyle->refer);
			DesignerChanged = True;
			break;

		case DLG_DSHS_RESUME_RAD:
			EnableItems(dlgId, DLG_DSHS_BROWSE_CK, DLG_DSHS_XNSCR_CK3, False);
			DesignerChanged = True;
			break;

		case DLG_DSHS_BROWSE_CK:
			EnableDlgItem(dlgId, DLG_DSHS_BR_PX_BOX,
													 GetDlgState(dlgId, itemNum));
			DesignerChanged = True;
			break;
		case DLG_DSHS_BR_PX_BOX:
			DesignerChanged = True;
			break;

		case DLG_DSHS_CONT_CK:
			EnableItems(dlgId, DLG_DSHS_CONT_V_RAD, DLG_DSHS_CONT_B_RAD,
													 GetDlgState(dlgId, itemNum));
			EnableDlgItem(dlgId, DLG_DSHS_CONT_LEV_BOX,
													 GetDlgState(dlgId, itemNum));
			DesignerChanged = True;
			break;
		case DLG_DSHS_CONT_V_RAD:
		case DLG_DSHS_CONT_H_RAD:
		case DLG_DSHS_CONT_T_RAD:
		case DLG_DSHS_CONT_B_RAD:
		case DLG_DSHS_CONT_LEV_BOX:
			DesignerChanged = True;
			break;

		case DLG_DSHS_WINDOW_CK:
			EnableDlgItem(dlgId, DLG_DSHS_WINDOW_BOX,
													 GetDlgState(dlgId, itemNum));
			DesignerChanged = True;
			break;
		case DLG_DSHS_WINDOW_BOX:
			DesignerChanged = True;
			break;

		case DLG_DSHS_NO_TITLE_CK:
			EnableItems(dlgId, DLG_DSHS_TITLE_SX_CK, DLG_DSHS_TITLE_SX_BOX,
													 !GetDlgState(dlgId, itemNum));
			DesignerChanged = True;
			break;
		case DLG_DSHS_TITLE_SX_CK:
			EnableDlgItem(dlgId, DLG_DSHS_TITLE_SX_BOX,
													 GetDlgState(dlgId, itemNum));
			DesignerChanged = True;
			break;
		case DLG_DSHS_TITLE_SX_BOX:
			DesignerChanged = True;
			break;

		case DLG_DSHS_REFER_CK:
			EnableDlgItem(dlgId, DLG_DSHS_REF_SX_BOX,
													 GetDlgState(dlgId, itemNum));
			DesignerChanged = True;
			break;
		case DLG_DSHS_REF_SX_BOX:
			DesignerChanged = True;
			break;

		case DLG_DSHS_NSCR_CK3:
		case DLG_DSHS_XNSCR_CK3:
			DesignerChanged = True;
			break;

		default:
			break;
	}
}


VoidT SetHelpTopicDesigner(F_ObjHandleT dlgId)
{
	if ((CurrDesStyle->helptyp != 1)
	 || CurrDesStyle->remap
	 || CurrDesStyle->replace)
		EnableItems(dlgId, DLG_DSHS_NORMAL_RAD, DLG_DSHS_XNSCR_CK3, False);
	else if (CurrDesStyle->topictyp == 2) {
		EnableItems(dlgId, DLG_DSHS_NORMAL_RAD, DLG_DSHS_RESUME_RAD, True);
		EnableItems(dlgId, DLG_DSHS_BROWSE_CK, DLG_DSHS_XNSCR_CK3, False);
	}
	else {	
		EnableItems(dlgId, DLG_DSHS_NORMAL_RAD, DLG_DSHS_XNSCR_CK3, True);
		EnableDlgItem(dlgId, DLG_DSHS_BR_PX_BOX, CurrDesStyle->browse);
		EnableItems(dlgId, DLG_DSHS_CONT_V_RAD, DLG_DSHS_CONT_B_RAD,
											 CurrDesStyle->cont);
		EnableDlgItem(dlgId, DLG_DSHS_CONT_LEV_BOX, CurrDesStyle->cont);
		EnableDlgItem(dlgId, DLG_DSHS_WINDOW_BOX, CurrDesStyle->window);
		EnableDlgItem(dlgId, DLG_DSHS_TITLE_SX_BOX,
									CurrDesStyle->titsx && !CurrDesStyle->notitle);
		EnableDlgItem(dlgId, DLG_DSHS_TITLE_SX_CK, !CurrDesStyle->notitle);
		EnableDlgItem(dlgId, DLG_DSHS_REF_SX_BOX, CurrDesStyle->refer);
	}

	SetDlgState(dlgId, DLG_DSHS_NORMAL_RAD + CurrDesStyle->topictyp, True);

	SetDlgState(dlgId, DLG_DSHS_BROWSE_CK, CurrDesStyle->browse);
	SetDlgText(dlgId, DLG_DSHS_BR_PX_BOX, CurrDesStyle->br_px);

	SetDlgState(dlgId, DLG_DSHS_CONT_CK, CurrDesStyle->cont);
	SetDlgState(dlgId, DLG_DSHS_CONT_V_RAD + CurrDesStyle->conttyp, True);
	SetDlgText(dlgId, DLG_DSHS_CONT_LEV_BOX, CurrDesStyle->contlev);

	SetDlgState(dlgId, DLG_DSHS_WINDOW_CK, CurrDesStyle->window);
	SetDlgText(dlgId, DLG_DSHS_WINDOW_BOX, CurrDesStyle->win_name);

	SetDlgState(dlgId, DLG_DSHS_NO_TITLE_CK, CurrDesStyle->notitle);
	SetDlgState(dlgId, DLG_DSHS_TITLE_SX_CK, CurrDesStyle->titsx);
	SetDlgText(dlgId, DLG_DSHS_TITLE_SX_BOX, CurrDesStyle->title_sx);

	SetDlgState(dlgId, DLG_DSHS_REFER_CK, CurrDesStyle->refer);
	SetDlgText(dlgId, DLG_DSHS_REF_SX_BOX, CurrDesStyle->ref_sx);

	SetDlgState(dlgId, DLG_DSHS_NSCR_CK3,
	 CurrDesStyle->noscr_used ? (CurrDesStyle->noscroll ? 1 : 0) : 2);
	SetDlgState(dlgId, DLG_DSHS_XNSCR_CK3,
	 CurrDesStyle->xtscr_used ? (CurrDesStyle->xtscroll ? 1 : 0) : 2);
}


VoidT GetHelpTopicDesigner(F_ObjHandleT dlgId)
{
	CurrDesStyle->topictyp = GetRadValue(dlgId, DLG_DSHS_NORMAL_RAD,
																							DLG_DSHS_RESUME_RAD);

	CurrDesStyle->browse = GetDlgState(dlgId, DLG_DSHS_BROWSE_CK);
	CurrDesStyle->br_px = GetDlgText(dlgId, DLG_DSHS_BR_PX_BOX);

	CurrDesStyle->cont = GetDlgState(dlgId, DLG_DSHS_CONT_CK);
	CurrDesStyle->conttyp = GetRadValue(dlgId, DLG_DSHS_CONT_V_RAD,
																						 DLG_DSHS_CONT_B_RAD);
	CurrDesStyle->contlev = GetDlgText(dlgId, DLG_DSHS_CONT_LEV_BOX);

	CurrDesStyle->window = GetDlgState(dlgId, DLG_DSHS_WINDOW_CK);
	CurrDesStyle->win_name = GetDlgText(dlgId, DLG_DSHS_WINDOW_BOX);

	CurrDesStyle->notitle = GetDlgState(dlgId, DLG_DSHS_NO_TITLE_CK);
	CurrDesStyle->titsx = GetDlgState(dlgId, DLG_DSHS_TITLE_SX_CK);
	CurrDesStyle->title_sx = GetDlgText(dlgId, DLG_DSHS_TITLE_SX_BOX);

	CurrDesStyle->refer = GetDlgState(dlgId, DLG_DSHS_REFER_CK);
	CurrDesStyle->ref_sx = GetDlgText(dlgId, DLG_DSHS_REF_SX_BOX);

	switch (GetDlgState(dlgId, DLG_DSHS_NSCR_CK3)) {
		case 0:
			CurrDesStyle->noscroll = False;
			CurrDesStyle->noscr_used = True;
			break;
		case 1:
			CurrDesStyle->noscroll = True;
			CurrDesStyle->noscr_used = True;
			break;
		case 2:
			CurrDesStyle->noscroll = False;
			CurrDesStyle->noscr_used = False;
			break;
		default:
			break;
	}

	switch (GetDlgState(dlgId, DLG_DSHS_XNSCR_CK3)) {
		case 0:
			CurrDesStyle->xtscroll = False;
			CurrDesStyle->xtscr_used = True;
			break;
		case 1:
			CurrDesStyle->xtscroll = True;
			CurrDesStyle->xtscr_used = True;
			break;
		case 2:
			CurrDesStyle->xtscroll = False;
			CurrDesStyle->xtscr_used = False;
			break;
		default:
			break;
	}

	StylesChanged = True;
}


VoidT FreeHelpTopicDesigner(F_ObjHandleT dlgId)
{
	/* no resources to release */
}


/* end of file m2rdshs.c */

