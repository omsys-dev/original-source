/* m2rdgrb.c is the FDK designer panel for Bitmaps
 * Last edited on 1/13/99 by Jeremy H. Griffith.
 * Copyright 1998, 1999 Omni Systems, Inc.  All Rights Reserved.
 */


#include "m2rbook.h"


#ifdef BMC_USED

/* Bitmap Character functions */

BitmapCharT *CurrBitmapChar = NULL;
StringT *BmcCharacterStrings = NULL;
StringT *BmcBitmapStrings = NULL;
StringT *DefBitmapStrings[DEF_BITMAP_COUNT + 2] = {
	"Bitmaps", "bullet.bmp", "chiclet.bmp", "closed.bmp",
	"document.bmp", "do-it.bmp", "emdash.bmp", "onestep.bmp",
	"open.bmp", "prcarrow.bmp", "shortcut.bmp", "<delete>"
};
	
BitmapCharT *DesBitmapChars;
BitmapCharT *CurrBitmapChar;
BitmapCharT DefBitmapChar = { "", 0, NULL, False, NULL, NULL };
BitmapCharT FirstBitmapChar = { "", 0, NULL, False, NULL, NULL };
BitmapCharT *LastBitmapChar = &FirstBitmapChar;
BitmapCharT ClipboardBitmapChar;
BitmapCharT *FoundBitmapChar = NULL;
BoolT BitmapCharChanged = False;

StringT *DesBitmapCharList = NULL;
IntT CurrBitmapCharNum = 0;
IntT DesBitmapCharCount = 0;
IntT DesBitmapCharAlloc = 0;
BoolT BitmapCharListChanged = False;


VoidT InitDesignerBitmapChars(VoidT)
{
	LastBitmapChar = &FirstBitmapChar;
	FirstBitmapChar.next = NULL;
	AddDesBitmapChar("40");
	LastBitmapChar->used = False;
	LastBitmapChar->num = 40;
	LastBitmapChar->graphic = "bullet.bmp";

	ReadBitmapCharsFromIni(CurrIni, True);	
	InitBitmapCharList();
}


VoidT AddDesBitmapChar(StringT name)
{
	BitmapCharT *curr = NULL;

	if (!InBitmapCharList(name)) {
		curr = (BitmapCharT *) F_Alloc(sizeof(BitmapCharT), NO_DSE);
		if (curr) {
			F_ClearPtr(curr, sizeof(BitmapCharT));
			curr->name = F_StrCopyString(name);
		}
		LastBitmapChar->next = curr;
		LastBitmapChar = curr;
		DesBitmapCharCount++;
	}
}


BoolT InBitmapCharList(StringT sname)
{
	BitmapCharT *item = FirstBitmapChar.next;

	FoundBitmapChar = NULL;

	while (item) {
		if (!F_StrICmp(sname, item->name)) {
			FoundBitmapChar = item;
			return True;
		}
		item = item->next;
	}
	return False;
}


VoidT InitBitmapCharList(VoidT)
{
	IntT i = 0;
	BitmapCharT *curr = NULL;

	if (DesBitmapCharList
	 && (DesBitmapCharAlloc < DesBitmapCharCount)) {
		F_Free(DesBitmapCharList);
		DesBitmapCharList = NULL;
	}

	if (DesBitmapCharList == NULL) {
		DesBitmapCharList = (StringT *) F_Alloc((DesBitmapCharCount + 1) * sizeof(StringT), NO_DSE);
		DesBitmapCharList[0] = "Bitmap Chars";
		DesBitmapCharAlloc = DesBitmapCharCount;
	}

	/* make list from flows */
	curr = DesBitmapChars;
	i = 0;
	while (curr) {
		DesBitmapCharList[++i] = curr->name;
		curr = (BitmapCharT *) curr->next;
	}

	/* sort the DesBitmapCharList entries 1..DesBitmapCharCount alphabetically */
	if (DesBitmapCharCount > 1)
		qsort(&DesBitmapCharList[1], DesBitmapCharCount, sizeof(StringT), CmpStrings);

	SetNewBitmapChar(1);  /* sets CurrBitmapChar and CurrBitmapCharNum */
	BitmapCharListChanged = True;


	/* set the global namelist entries to the new info */
	DesNameLists[DES_NAMES_BITMAPS].nameList = DesBitmapCharList;
	DesNameLists[DES_NAMES_BITMAPS].nameCount = DesBitmapCharCount;
	DesNameLists[DES_NAMES_BITMAPS].nameSel = CurrBitmapCharNum;
}


VoidT SetNewBitmapChar(IntT num)
{
	BitmapCharT *curr = NULL;
	StringT sname = NULL;

	CurrBitmapCharNum = num;
	sname = DesBitmapCharList[num];
	curr = DesBitmapChars;
	while (curr
	 && F_StrICmp(sname, curr->name))
		curr = (BitmapCharT *) curr->next;
	if (curr)
		CurrBitmapChar = curr;
}


VoidT FreeDesignerBitmapChars(VoidT)
{
	BitmapCharT *last = DesBitmapChars;
	BitmapCharT *curr = NULL;

	if (BitmapCharChanged)
		WriteBitmapCharsToIni(CurrIni, True);

	while (last) {
		F_StrFree(last->name);

		// free all the other 10 strings too

		curr = (BitmapCharT *) (last->next);
		F_Free(last);
		last = curr;
	}

	DesBitmapChars = NULL;
	DesBitmapCharCount = 0;
	DesBitmapCharAlloc = 0;
	CurrBitmapChar = NULL;
	FirstBitmapChar.next = NULL;

	F_Free(DesBitmapCharList);
	DesBitmapCharList = NULL;
	CurrBitmapCharNum = 0;
}


IntT FindBitmapCharInList(StringT sname)
{
	IntT num = 1;

	if (sname && *sname) {
		while (num <= DesBitmapCharCount) {
			if (!F_StrICmp(sname, DesBitmapCharList[num]))
				return num;
			num++;
		}
	}

	return -1;

}


StringT BitmapChars = { "BitmapChars" };


VoidT ReadBitmapCharsFromIni(StringT ininame, BoolT confirm)
{
	BitmapCharT *curr = NULL;
	StringT sname = NULL;

	GetIniSect(ininame, BitmapChars);
	while ((sname = GetIniSectKey()) != NULL) {
		for (curr = FirstBitmapChar.next; curr; curr = (BitmapCharT *) curr->next) {
			if (!F_StrICmp(sname, curr->name))
				break;
			if (!curr->next) {
				curr->next = F_Alloc(sizeof(BitmapCharT), NO_DSE);
				curr = (BitmapCharT *) curr->next;
				F_ClearPtr(curr, sizeof(BitmapCharT));
				curr->name = F_StrCopyString(sname);
				curr->next = NULL;
				LastBitmapChar = curr;
				DesBitmapCharCount++;
				break;
			}
		}
		curr->used = True;
		if (F_CharIsNumeric(curr->name[0])
		 && F_CharIsNumeric(curr->name[1]))  // numeric
			curr->num = atoi(curr->name);
		else
			curr->num = (IntT) (curr->name[0]);
		GetIniString(ininame, BitmapChars, sname, &curr->graphic);
	}

	BitmapCharChanged = False;
}


VoidT WriteBitmapCharsToIni(StringT ininame, BoolT confirm)
{
	BitmapCharT *curr = NULL;

	if (BitmapCharChanged == False)
		return;

	for (curr = DesBitmapChars; curr; curr = (BitmapCharT *) curr->next)
		SetIniStr(ininame, BitmapChars, curr->name, curr->used ? curr->bitmap : NULL);

	BitmapCharChanged = False;
}


BoolT SetBitmapChar(StringT *chp, BoolT to_num)
{
	IntT num = 0;

	if (F_CharIsNumeric((*chp)[0])
	 && F_CharIsNumeric((*chp)[1])) {    // now numeric
		if (!to_num) {   // make char
			num = atoi(*chp);
			F_Sprintf(*chp, "%c", (IntT) num);
			return true;
		}
	}
	else {                      // now in character form
		if (to_num)) { // make first char numeric
			num = (IntT) ((*chp)[0]);
			F_StrFree(*chp);
			*chp = F_StrNew(4);
			F_Sprintf(*chp, "%ld", (IntT) num);
			return true;
		}
	}
}

#endif

/**************************************************************/
/* Bitmaps designer page */

BoolT BitmapCharChanged = False;


/* m2rdgrb items */

#define DLG_DGRB_BMC_BOX  3

#define DLG_DGRB_BMC_CK  18
#define DLG_DGRB_BMP_BOX 19
#define DLG_DGRB_BMP_POP 20
#define DLG_DGRB_BMC_RAD 22
#define DLG_DGRB_BMN_RAD 23

#define DLG_DGRB_DPI_CK  25
#define DLG_DGRB_DPI_BOX 26
#define DLG_DGRB_FLIP_CK 27
#define DLG_DGRB_TRAN_CK 28

#define DLG_DGRB_CMPR_CK 29
#define DLG_DGRB_SM0_RAD 32
#define DLG_DGRB_SM1_RAD 33
#define DLG_DGRB_SM2_RAD 34
#define DLG_DGRB_SM3_RAD 35


/* Bitmaps dialog events */

VoidT ProcBitmapsDesignerEvent(F_ObjHandleT dlgId, IntT itemNum)
{
	IntT bmp_idx = 0;
	StringT bmp_val = NULL;
	StringT bmc_val = NULL;

	switch (itemNum) {

#ifdef BMC_USED
		case DLG_DGRB_BMC_CK:
			// check is add or modify, uncheck is delete
			EnableItems(dlgId, DLG_DGRB_BMP_BOX, DLG_DGRB_BMP_POP,
													 GetDlgState(dlgId, itemNum));
			BitmapCharChanged = True;
			DesignerChanged = True;
			break;

		case DLG_DGRB_BMP_BOX:
			// typed in box, add or modify
			BitmapCharChanged = True;
			DesignerChanged = True;
			break;

		case DLG_DGRB_BMP_POP:
			// list selection made, add or modify
			bmp_idx = GetDlgState(dlgId, itemNum);
			bmp_val = BmcBitmapStrings[bmc_idx];
			SetDlgText(dlgId, DLG_DGRB_BMP_BOX, bmp_val);
			BitmapCharChanged = True;
			DesignerChanged = True;
			break;

		case DLG_DGRB_BMC_RAD:
		case DLG_DGRB_BMN_RAD:
			// switch char string between char and numeric forms
			bmc_val = GetDlgText(dlgId, DLG_DGRB_BMC_BOX);
			if (SetBitmapChar(&bmc_val, (itemNum == DLG_DGRB_BMN_RAD) ? True : False)) {
				SetDlgText(dlgId, DLG_DGRB_BMC_BOX, bmc_val);
				BitmapCharChanged = True;
			}
			F_StrFree(bmc_val);
			DesignerChanged = True;
			break;
#endif

		case DLG_DGRB_DPI_CK:
			EnableDlgItem(dlgId, DLG_DGRB_DPI_BOX,
													 GetDlgState(dlgId, itemNum));
			DesignerChanged = True;
			break;

		case DLG_DGRB_DPI_BOX:
			DesignerChanged = True;
			break;

		case DLG_DGRB_FLIP_CK:
		case DLG_DGRB_TRAN_CK:
		case DLG_DGRB_CMPR_CK:
			DesignerChanged = True;
			break;

		case DLG_DGRB_SM0_RAD:
		case DLG_DGRB_SM1_RAD:
		case DLG_DGRB_SM2_RAD:
		case DLG_DGRB_SM3_RAD:
			DesignerChanged = True;
			break;

		default:
			break;
	}
}


VoidT SetBitmapsDesigner(F_ObjHandleT dlgId)
{
#ifdef BMC_USED
	EnableItems(dlgId, DLG_DGRB_BMC_CK, DLG_DGRB_BMN_RAD, (CurrPropSetNum == DES_PROPSET_HELP));
	if (CurrPropSetNum == DES_PROPSET_HELP) {
		SetDlgState(dlgId, DLG_DGRB_BMC_CK, CurrBitmapChar->used);
		SetLabels(dlgId, DLG_DGRB_BMP_POP, &BmcBitmapStrings);
		SetDlgText(dlgId, DLG_DGRB_BMP_BOX, CurrBitmapChar->graphic);
		SetDlgState(dlgId, DLG_DGRB_BMC_RAD + CurrBitmapChar->num, True);
		EnableItems(dlgId, DLG_DGRB_BMP_BOX, DLG_DGRB_BMP_POP, CurrBitmapChar->used);
	}
#else
	EnableItems(dlgId, DLG_DGRB_BMC_CK, DLG_DGRB_BMN_RAD, False);
#endif

	EnableDlgItem(dlgId, DLG_DGRB_TRAN_CK, (CurrPropSetNum == DES_PROPSET_HELP));

	SetDlgState(dlgId, DLG_DGRB_DPI_CK, CurrDesGraph.bitmap_rescale);
	SetDlgText(dlgId, DLG_DGRB_DPI_BOX, CurrDesGraph.bitmap_dpi);
	EnableDlgItem(dlgId, DLG_DGRB_DPI_BOX, CurrDesGraph.bitmap_rescale);

	SetDlgState(dlgId, DLG_DGRB_FLIP_CK, CurrDesGraph.bitmap_flip);

	SetDlgState(dlgId, DLG_DGRB_TRAN_CK, CurrDesGraph.bitmap_trans);

	SetDlgState(dlgId, DLG_DGRB_CMPR_CK, CurrDesGraph.compress_bmps);
	SetDlgState(dlgId, DLG_DGRB_SM0_RAD + CurrDesGraph.stretch_mode, True);

	BitmapCharChanged = False;
	DesignerChanged = False;
}


VoidT GetBitmapsDesigner(F_ObjHandleT dlgId)
{
#ifdef BMC_USED
	StringT bmc_char = NULL;
	IntT bmc_num = 0;

	bmc_num = GetRadValue(dlgId, DLG_DGRB_BMC_RAD, DLG_DGRB_BMN_RAD);
	if (bmc_num != CurrBitmapChar->num) {
		CurrBitmapChar->num = bmc_num;
		BitmapCharChanged = True;
		BitmapListChanged = True;
	}

	bmc_char = GetDlgText(dlgId, DLG_DGRB_BMC_BOX);
	if (F_StrICmp(bmc_char, CurrBitmapChar->name)) {
		CurrBitmapChar->name = bmc_char;
		BitmapCharChanged = True;
		BitmapListChanged = True;
	}

	if (BitmapCharChanged) {
		CurrBitmapChar->used = GetDlgState(dlgId, DLG_DGRB_BMC_CK);
		CurrBitmapChar->graphic = GetDlgText(dlgId, DLG_DGRB_BMP_BOX);
	}
#endif

	CurrDesGraph.bitmap_rescale = GetDlgState(dlgId, DLG_DGRB_DPI_CK);
	CurrDesGraph.bitmap_dpi = GetDlgText(dlgId, DLG_DGRB_DPI_BOX);

	CurrDesGraph.bitmap_flip = GetDlgState(dlgId, DLG_DGRB_FLIP_CK);

	CurrDesGraph.bitmap_trans = GetDlgState(dlgId, DLG_DGRB_TRAN_CK);

	CurrDesGraph.compress_bmps = GetDlgState(dlgId, DLG_DGRB_CMPR_CK);
	CurrDesGraph.stretch_mode = GetRadValue(dlgId, DLG_DGRB_SM0_RAD,
																								 DLG_DGRB_SM3_RAD);

	GraphicsChanged = True;
	DesignerChanged = False;
}


VoidT FreeBitmapsDesigner(F_ObjHandleT dlgId)
{
	/* no resources to release */
}


/* end of file m2rdgrb.c */

