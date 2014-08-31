/* m2rdver.c is the FDK designer panel for Version
 * Last edited on 1/5/99 by Jeremy H. Griffith.
 * Copyright 1998, 1999 Omni Systems, Inc.  All Rights Reserved.
 */


#include "m2rbook.h"


/**************************************************************/
/* Version designer page */


/* m2rdver items */

#define DLG_DVER_OUTS_RAD 19
#define DLG_DVER_OUTH_RAD 20

#define DLG_DVER_W7_RAD 23
#define DLG_DVER_W8_RAD 24
#define DLG_DVER_WP_RAD 25

#define DLG_DVER_WD_SUF 27
#define DLG_DVER_WP_SUF 29

#define DLG_DVER_H3_RAD 32
#define DLG_DVER_H4_RAD 33

#define DLG_DVER_ALTURA_CK 34
#define DLG_DVER_HYPERH_CK 35
#define DLG_DVER_FRCBMC_CK 36


/* Version dialog events */

VoidT ProcVersionDesignerEvent(F_ObjHandleT dlgId, IntT itemNum)
{
	switch (itemNum) {

		case DLG_DVER_OUTS_RAD:
		case DLG_DVER_OUTH_RAD:

		case DLG_DVER_W7_RAD:
		case DLG_DVER_W8_RAD:
		case DLG_DVER_WP_RAD:

		case DLG_DVER_WD_SUF:
		case DLG_DVER_WP_SUF:

		case DLG_DVER_H3_RAD:
		case DLG_DVER_H4_RAD:

		case DLG_DVER_ALTURA_CK:
		case DLG_DVER_HYPERH_CK:
		case DLG_DVER_FRCBMC_CK:
			DesignerChanged = True;
			break;

		default:
			break;
	}
}


VoidT SetVersionDesigner(F_ObjHandleT dlgId)
{
	SetDlgState(dlgId, DLG_DVER_OUTS_RAD + CurrDesText.output, True);

	if (CurrDesText.word8)
		SetDlgState(dlgId, DLG_DVER_W8_RAD, True);
	else if (CurrDesText.wordperf)
		SetDlgState(dlgId, DLG_DVER_WP_RAD, True);
	else
		SetDlgState(dlgId, DLG_DVER_W7_RAD, True);

	SetDlgText(dlgId, DLG_DVER_WD_SUF, CurrDesText.word_suf);
	SetDlgText(dlgId, DLG_DVER_WP_SUF, CurrDesText.wp_suf);

	SetDlgState(dlgId, DLG_DVER_H3_RAD + CurrDesText.helpver, True);

	SetDlgState(dlgId, DLG_DVER_ALTURA_CK, CurrDesText.altura);
	SetDlgState(dlgId, DLG_DVER_HYPERH_CK, CurrDesText.hyperhelp);
	SetDlgState(dlgId, DLG_DVER_FRCBMC_CK, CurrDesText.force_bmc);

	DesignerChanged = False;
}


VoidT GetVersionDesigner(F_ObjHandleT dlgId)
{
	CurrDesText.output = GetRadValue(dlgId, DLG_DVER_OUTS_RAD,
																					DLG_DVER_OUTH_RAD);

	CurrDesText.word8 = GetDlgState(dlgId, DLG_DVER_W8_RAD);
	CurrDesText.wordperf = GetDlgState(dlgId, DLG_DVER_WP_RAD);

	CurrDesText.word_suf = GetDlgText(dlgId, DLG_DVER_WD_SUF);
	CurrDesText.wp_suf = GetDlgText(dlgId, DLG_DVER_WP_SUF);

	CurrDesText.helpver = GetRadValue(dlgId, DLG_DVER_H3_RAD,
																					 DLG_DVER_H4_RAD);

	CurrDesText.altura = GetDlgState(dlgId, DLG_DVER_ALTURA_CK);
	CurrDesText.hyperhelp = GetDlgState(dlgId, DLG_DVER_HYPERH_CK);
	CurrDesText.force_bmc = GetDlgState(dlgId, DLG_DVER_FRCBMC_CK);

	TextChanged = True;
	DesignerChanged = False;
}


VoidT FreeVersionDesigner(F_ObjHandleT dlgId)
{
	/* no resources to release */
}


/* end of file m2rdver.c */

