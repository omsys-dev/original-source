/* m2rdlg.c has the FDK dialog utilities for mif2rtf FM 5.5  
 * Last edited on 12/25/98 by Jeremy H. Griffith.
 * Copyright 1998 Omni Systems, Inc.  All Rights Reserved.
 */

#include "m2rbook.h"


/* general purpose dialog utilities */

VoidT EnableItems(F_ObjHandleT dlgId, IntT first, IntT last, BoolT state)
{
	IntT i = 0;
	F_ObjHandleT itemId = 0;

	for (i = first; i <= last; i++) {
		itemId = F_ApiDialogItemId(dlgId, i);
		if (itemId)
			F_ApiSetInt(dlgId, itemId, FP_Sensitivity, state);
	}
}


VoidT EnableDlgItem(F_ObjHandleT dlgId, IntT item, BoolT state)
{
	F_ApiSetInt(dlgId, F_ApiDialogItemId(dlgId, item), FP_Sensitivity, state);
}



IntT GetDlgState(F_ObjHandleT dlgId, IntT item)
{
	return F_ApiGetInt(dlgId, F_ApiDialogItemId(dlgId, item), FP_State);
}


VoidT SetDlgState(F_ObjHandleT dlgId, IntT item, IntT state)
{
	F_ApiSetInt(dlgId, F_ApiDialogItemId(dlgId, item), FP_State, state);
}



StringT GetDlgText(F_ObjHandleT dlgId, IntT item)
{
	return F_ApiGetString(dlgId, F_ApiDialogItemId(dlgId, item), FP_Text);
}


VoidT SetDlgText(F_ObjHandleT dlgId, IntT item, StringT text)
{
	F_ApiSetString(dlgId, F_ApiDialogItemId(dlgId, item), FP_Text, text);
}



VoidT SetDlgLabel(F_ObjHandleT dlgId, IntT item, StringT text)
{
	F_ApiSetString(dlgId, F_ApiDialogItemId(dlgId, item), FP_Label, text);
}


VoidT SetLabels(F_ObjHandleT dlgId, IntT item, F_StringsT *psList)
{
	F_ApiSetStrings(dlgId, F_ApiDialogItemId(dlgId, item), FP_Labels, psList);
}



VoidT SetStyleListState(F_ObjHandleT dlgId, IntT item, StringT text)
{
	IntT val = 0;

	val = (text && *text) ? FindStyleInList(text) : -1;
	SetDlgState(dlgId, item, (val == -1) ? 1 : val);
}


StringT GetStyleListText(F_ObjHandleT dlgId, IntT item)
{
	IntT val = 0;

	val = GetDlgState(dlgId, item);
	if ((val < 1)
	 || (val > DesListCount))
		return NULL;

	return F_StrCopyString(DesStyleList[val]);
}



VoidT SetGrFormListState(F_ObjHandleT dlgId, IntT item, StringT text)
{
	IntT val = 0;

	val = (text && *text) ? FindGrFormInList(text) : -1;
	SetDlgState(dlgId, item, (val == -1) ? 1 : val);
}


StringT GetGrFormListText(F_ObjHandleT dlgId, IntT item)
{
	IntT val = 0;

	val = GetDlgState(dlgId, item);
	if ((val < 1)
	 || (val > DES_GRX_FORM_COUNT))
		return NULL;

	return F_StrCopyString(GrFormList[GrFormMap[DesGrFormVal[val]]]);
}


VoidT SetGrExFormListState(F_ObjHandleT dlgId, IntT item, StringT text)
{
	IntT val = 0;

	val = (text && *text) ? FindGrExFormInList(text) : -1;
	SetDlgState(dlgId, item, (val == -1) ? 1 : val);
}


StringT GetGrExFormListText(F_ObjHandleT dlgId, IntT item)
{
	IntT val = 0;

	val = GetDlgState(dlgId, item);
	if ((val < 1)
	 || (val > DES_GRX_FORM_COUNT))
		return NULL;

	return F_StrCopyString(DesGrFormList[val]);
}


VoidT SetFontListState(F_ObjHandleT dlgId, IntT item, StringT text)
{
	IntT val = 0;
	if (!text || !*text)
		text = "Arial";
	if (FontListChanged)
		SetLabels(dlgId, item, &FontStrings);
	val = (text && *text) ? FindFontInList(text) : -1;
	if (val == -1)
		val = 1;
	if (FontListChanged
	 || (GetDlgState(dlgId, item) != val))
		SetDlgState(dlgId, item, val);
}


StringT GetFontListText(F_ObjHandleT dlgId, IntT item)
{
	IntT val = 0;

	val = GetDlgState(dlgId, item);
	if ((val < 1)
	 || (val > DesFontCount))
		return NULL;

	return F_StrCopyString(DesFontList[val]);
}



IntT GetRadValue(F_ObjHandleT dlgId, IntT first, IntT last)
{
	IntT i = 0;

	for (i = first; i < last; i++) {
		if (GetDlgState(dlgId, i))
			return (i - first);
	}
	return (last - first);
}



VoidT SetDlgProject(F_ObjHandleT dlgId, IntT item)
{
	F_ObjHandleT itemId = 0;
	StringT projdesc = NULL;

	itemId = F_ApiDialogItemId(dlgId, item);
	projdesc = F_StrNew(F_StrLen(CurrProject) + F_StrLen(CurrProjPath) + 3);
	sprintf(projdesc, "%s: %s", CurrProject, CurrProjPath);
	F_ApiSetString(dlgId, itemId, FP_Label, projdesc);
	F_StrFree(projdesc);
}


/* end of file m2rdlg.c */


