/* m2rdshb.c is the FDK designer panel for Basic Styles
 * Last edited on 12/24/98 by Jeremy H. Griffith.
 * Copyright 1998 Omni Systems, Inc.  All Rights Reserved.
 */


#include "m2rbook.h"


/**************************************************************/
/* Basic Styles designer page */


/* m2rdshb items */

#define DLG_DSHB_RETAIN_RAD   20
#define DLG_DSHB_REMAP_RAD    21
#define DLG_DSHB_REMAP_BOX    22
#define DLG_DSHB_REPLACE_RAD  23
#define DLG_DSHB_REPLACE_LIST 25

#define DLG_DSHB_REM_UNUSED_CK  26

#define DLG_DSHB_HELP_LAB     27
#define DLG_DSHB_NOHELP_RAD   29
#define DLG_DSHB_TOPIC_RAD    30
#define DLG_DSHB_HOTSPOT_RAD  31
#define DLG_DSHB_TARGET_RAD   32
#define DLG_DSHB_MACRO_RAD    33

#define DLG_DSHB_KEYWORD_CK   35
#define DLG_DSHB_AKEY_CK      36
#define DLG_DSHB_SPKEY_BOX    38


/* basic styles dialog events */

VoidT ProcBasicStyleDesignerEvent(F_ObjHandleT dlgId, IntT itemNum)
{
	StringT tstr = NULL;

	switch (itemNum) {
		case DLG_DSHB_RETAIN_RAD:
			EnableItems(dlgId, DLG_DSHB_HELP_LAB, DLG_DSHB_SPKEY_BOX,
												 (CurrPropSetNum == 1));
			EnableDlgItem(dlgId, DLG_DSHB_REMAP_BOX, False);
			EnableDlgItem(dlgId, DLG_DSHB_REPLACE_LIST, False);
			DesignerChanged = True;
			break;

		case DLG_DSHB_REMAP_RAD:
			EnableItems(dlgId, DLG_DSHB_HELP_LAB, DLG_DSHB_SPKEY_BOX, False);
			EnableDlgItem(dlgId, DLG_DSHB_REMAP_BOX, True);
			EnableDlgItem(dlgId, DLG_DSHB_REPLACE_LIST, False);
			DesignerChanged = True;
			break;

		case DLG_DSHB_REPLACE_RAD:
			EnableItems(dlgId, DLG_DSHB_HELP_LAB, DLG_DSHB_SPKEY_BOX, False);
			EnableDlgItem(dlgId, DLG_DSHB_REMAP_BOX, False);
			EnableDlgItem(dlgId, DLG_DSHB_REPLACE_LIST, True);
			DesignerChanged = True;
			break;

		case DLG_DSHB_REMAP_BOX:
		case DLG_DSHB_REPLACE_LIST:
			DesignerChanged = True;
			break;

		case DLG_DSHB_TOPIC_RAD:
			if (GetDlgState(dlgId, itemNum)
			 && !CurrDesStyle->para) {
				if (CurrDesStyle->helptyp != 1) {
					SetDlgState(dlgId, DLG_DSHB_NOHELP_RAD + CurrDesStyle->helptyp, True);
					break;
				}
				SetDlgState(dlgId, DLG_DSHB_NOHELP_RAD, True);
			}
			DesignerChanged = True;
			break;

		case DLG_DSHB_NOHELP_RAD:
		case DLG_DSHB_HOTSPOT_RAD:
		case DLG_DSHB_TARGET_RAD:
		case DLG_DSHB_MACRO_RAD:
			DesignerChanged = True;
			break;

		case DLG_DSHB_KEYWORD_CK:
		case DLG_DSHB_AKEY_CK:
			DesignerChanged = True;
			break;

		case DLG_DSHB_SPKEY_BOX:
			DesignerChanged = True;
			break;

		case DLG_DSHB_REM_UNUSED_CK:
			DesignerChanged = True;
			break;

		default:
			break;
	}
}


VoidT SetBasicStyleDesigner(F_ObjHandleT dlgId)
{
	IntT val = 0;

	EnableItems(dlgId, DLG_DSHB_HELP_LAB, DLG_DSHB_SPKEY_BOX,
	 (CurrPropSetNum == 1) && !(CurrDesStyle->remap || CurrDesStyle->replace));

	if (CurrDesStyle->remap)
		SetDlgState(dlgId, DLG_DSHB_REMAP_RAD, True);
	else if (CurrDesStyle->replace)
		SetDlgState(dlgId, DLG_DSHB_REPLACE_RAD, True);
	else
		SetDlgState(dlgId, DLG_DSHB_RETAIN_RAD, True);

	SetDlgText(dlgId, DLG_DSHB_REMAP_BOX, CurrDesStyle->rem_name);
	// do something if style exists, or add to style list
	EnableDlgItem(dlgId, DLG_DSHB_REMAP_BOX, CurrDesStyle->remap);

	SetLabels(dlgId, DLG_DSHB_REPLACE_LIST, &StyleStrings);
	SetStyleListState(dlgId, DLG_DSHB_REPLACE_LIST, CurrDesStyle->rep_name);
	EnableDlgItem(dlgId, DLG_DSHB_REPLACE_LIST, CurrDesStyle->replace);

	if ((CurrDesStyle->helptyp == 1)
	 && !CurrDesStyle->para)
		CurrDesStyle->helptyp = 0;
	SetDlgState(dlgId, DLG_DSHB_NOHELP_RAD + CurrDesStyle->helptyp, True);

	SetDlgState(dlgId, DLG_DSHB_KEYWORD_CK, CurrDesStyle->usekey);
	SetDlgState(dlgId, DLG_DSHB_AKEY_CK, CurrDesStyle->akey);
	SetDlgText(dlgId, DLG_DSHB_SPKEY_BOX, CurrDesStyle->keys);

	SetDlgState(dlgId, DLG_DSHB_REM_UNUSED_CK, CurrDesText.rem_unused_sty);

}


VoidT GetBasicStyleDesigner(F_ObjHandleT dlgId)
{
	IntT val = 0;

	CurrDesStyle->remap = GetDlgState(dlgId, DLG_DSHB_REMAP_RAD);
	CurrDesStyle->rem_name = GetDlgText(dlgId, DLG_DSHB_REMAP_BOX);
	CurrDesStyle->replace = GetDlgState(dlgId, DLG_DSHB_REPLACE_RAD);
	if (CurrDesStyle->replace
	 && ((val = GetDlgState(dlgId, DLG_DSHB_REPLACE_LIST)) != -1))
		CurrDesStyle->rep_name = F_StrCopyString(DesStyleList[val]);
	else
		CurrDesStyle->rep_name = NULL;

	CurrDesStyle->helptyp = GetRadValue(dlgId, DLG_DSHB_NOHELP_RAD,
																						 DLG_DSHB_MACRO_RAD);
	if ((CurrDesStyle->helptyp == 1)
	 && !CurrDesStyle->para) {
		CurrDesStyle->helptyp = 0;
		SetDlgState(dlgId, DLG_DSHB_NOHELP_RAD, True);
	}

	CurrDesStyle->usekey = GetDlgState(dlgId, DLG_DSHB_KEYWORD_CK);
	CurrDesStyle->akey = GetDlgState(dlgId, DLG_DSHB_AKEY_CK);
	CurrDesStyle->keys = GetDlgText(dlgId, DLG_DSHB_SPKEY_BOX);

	CurrDesText.rem_unused_sty = GetDlgState(dlgId, DLG_DSHB_REM_UNUSED_CK);

	StylesChanged = True;
}


VoidT FreeBasicStyleDesigner(F_ObjHandleT dlgId)
{
	/* no resources to release */
}


/* end of file m2rdshb.c */

