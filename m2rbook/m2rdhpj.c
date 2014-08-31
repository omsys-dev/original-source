/* m2rdhpj.c is the FDK designer panel for Help Compile
 * Last edited on 1/4/99 by Jeremy H. Griffith.
 * Copyright 1998, 1999 Omni Systems, Inc.  All Rights Reserved.
 */


#include "m2rbook.h"


/**************************************************************/
/* Help Compile designer page */


/* m2rdhpj items */

#define DLG_DHPJ_COMB_CK  18

#define DLG_DHPJ_COMPH_CK 20

#define DLG_DHPJ_HPJ_NM   22
#define DLG_DHPJ_HLP_NM   24
#define DLG_DHPJ_HLP_TIT  26
#define DLG_DHPJ_CONT_REF 28

#define DLG_DHPJ_COPYR_TX 30
#define DLG_DHPJ_CPY_DATE 31


/* Help Compile dialog events */

VoidT ProcHelpCompileDesignerEvent(F_ObjHandleT dlgId, IntT itemNum)
{
	switch (itemNum) {

		case DLG_DHPJ_COMB_CK:

		case DLG_DHPJ_COMPH_CK:

		case DLG_DHPJ_HPJ_NM:
		case DLG_DHPJ_HLP_NM:
		case DLG_DHPJ_HLP_TIT:
		case DLG_DHPJ_CONT_REF:

		case DLG_DHPJ_COPYR_TX:
		case DLG_DHPJ_CPY_DATE:
			DesignerChanged = True;
			break;

		default:
			break;
	}
}


VoidT SetHelpCompileDesigner(F_ObjHandleT dlgId)
{
	EnableDlgItem(dlgId, DLG_DHPJ_COMB_CK, (CurrDesText.cnt_type == 1));

	SetDlgState(dlgId, DLG_DHPJ_COMB_CK, CurrDesText.comb_cnt);

	SetDlgState(dlgId, DLG_DHPJ_COMPH_CK, CurrDesText.comp_hlp);

	SetDlgText(dlgId, DLG_DHPJ_HPJ_NM, CurrDesText.hpj_name);
	SetDlgText(dlgId, DLG_DHPJ_HLP_NM, CurrDesText.hlp_name);
	SetDlgText(dlgId, DLG_DHPJ_HLP_TIT, CurrDesText.hlp_title);
	SetDlgText(dlgId, DLG_DHPJ_CONT_REF, CurrDesText.contents_ref);

	SetDlgText(dlgId, DLG_DHPJ_COPYR_TX, CurrDesText.copyr_txt);
	SetDlgState(dlgId, DLG_DHPJ_CPY_DATE, CurrDesText.copyr_date);

	DesignerChanged = False;
}


VoidT GetHelpCompileDesigner(F_ObjHandleT dlgId)
{
	CurrDesText.comb_cnt = GetDlgState(dlgId, DLG_DHPJ_COMB_CK);

	CurrDesText.comp_hlp = GetDlgState(dlgId, DLG_DHPJ_COMPH_CK);

	CurrDesText.hpj_name = GetDlgText(dlgId, DLG_DHPJ_HPJ_NM);
	CurrDesText.hlp_name = GetDlgText(dlgId, DLG_DHPJ_HLP_NM);
	CurrDesText.hlp_title = GetDlgText(dlgId, DLG_DHPJ_HLP_TIT);
	CurrDesText.contents_ref = GetDlgText(dlgId, DLG_DHPJ_CONT_REF);

	CurrDesText.copyr_txt = GetDlgText(dlgId, DLG_DHPJ_COPYR_TX);
	CurrDesText.copyr_date = GetDlgState(dlgId, DLG_DHPJ_CPY_DATE);

	TextChanged = True;
	DesignerChanged = False;
}


VoidT FreeHelpCompileDesigner(F_ObjHandleT dlgId)
{
	/* no resources to release */
}


/* end of file m2rdhpj.c */

