/* m2rdtxa.c is the FDK designer panel for Advanced Text
 * Last edited on 1/2/98 by Jeremy H. Griffith.
 * Copyright 1998, 1999 Omni Systems, Inc.  All Rights Reserved.
 */


#include "m2rbook.h"


/**************************************************************/
/* Advanced Text designer page */


/* m2rdtxa items */

#define DLG_DTXA_IXS_RAD 19
#define DLG_DTXA_IXH_RAD 20
#define DLG_DTXA_IXN_RAD 21

#define DLG_DTXA_XRS_RAD 24
#define DLG_DTXA_XRH_RAD 25
#define DLG_DTXA_XRN_RAD 26

#define DLG_DTXA_XTF_RAD 28
#define DLG_DTXA_XTN_RAD 29

#define DLG_DTXA_QTS_RAD 32
#define DLG_DTXA_QTH_RAD 33
#define DLG_DTXA_QTN_RAD 34

#define DLG_DTXA_TBR_RAD 37
#define DLG_DTXA_TBS_RAD 38
#define DLG_DTXA_TBT_RAD 39

#define DLG_DTXA_FMSP_CK 41
#define DLG_DTXA_CHST_CK 42
#define DLG_DTXA_SBHT_CK 43
#define DLG_DTXA_SBWF_CK 44



/* Advanced Text dialog events */

VoidT ProcAdvancedTextDesignerEvent(F_ObjHandleT dlgId, IntT itemNum)
{
	switch (itemNum) {

		case DLG_DTXA_IXS_RAD:
		case DLG_DTXA_IXH_RAD:
		case DLG_DTXA_IXN_RAD:
			DesignerChanged = True;
			break;

		case DLG_DTXA_XRS_RAD:
		case DLG_DTXA_XRH_RAD:
			EnableItems(dlgId, DLG_DTXA_XTF_RAD, DLG_DTXA_XTN_RAD, True);
			DesignerChanged = True;
			break;

		case DLG_DTXA_XRN_RAD:
			EnableItems(dlgId, DLG_DTXA_XTF_RAD, DLG_DTXA_XTN_RAD, False);
			DesignerChanged = True;
			break;

		case DLG_DTXA_XTF_RAD:
		case DLG_DTXA_XTN_RAD:
			DesignerChanged = True;
			break;

		case DLG_DTXA_QTS_RAD:
		case DLG_DTXA_QTH_RAD:
		case DLG_DTXA_QTN_RAD:
			DesignerChanged = True;
			break;

		case DLG_DTXA_TBR_RAD:
		case DLG_DTXA_TBS_RAD:
		case DLG_DTXA_TBT_RAD:
			DesignerChanged = True;
			break;

		case DLG_DTXA_FMSP_CK:
		case DLG_DTXA_CHST_CK:
		case DLG_DTXA_SBHT_CK:
		case DLG_DTXA_SBWF_CK:
			DesignerChanged = True;
			break;

		default:
			break;
	}
}


VoidT SetAdvancedTextDesigner(F_ObjHandleT dlgId)
{
	EnableDlgItem(dlgId, DLG_DTXA_IXS_RAD, (CurrPropSetNum == DES_PROPSET_WORD));
	EnableDlgItem(dlgId, DLG_DTXA_IXH_RAD, (CurrPropSetNum == DES_PROPSET_HELP));
	EnableDlgItem(dlgId, DLG_DTXA_XRS_RAD, (CurrPropSetNum == DES_PROPSET_WORD));
	EnableDlgItem(dlgId, DLG_DTXA_XRH_RAD, (CurrPropSetNum == DES_PROPSET_HELP));
	EnableDlgItem(dlgId, DLG_DTXA_QTS_RAD, (CurrPropSetNum != DES_PROPSET_HELP));
	EnableDlgItem(dlgId, DLG_DTXA_SBHT_CK, (CurrPropSetNum == DES_PROPSET_HELP));
	EnableDlgItem(dlgId, DLG_DTXA_SBWF_CK, (CurrPropSetNum == DES_PROPSET_WORD));

	SetDlgState(dlgId, DLG_DTXA_IXS_RAD + CurrDesText.idx_usage, True);
	SetDlgState(dlgId, DLG_DTXA_XRS_RAD + CurrDesText.xref_usage, True);
	SetDlgState(dlgId, DLG_DTXA_XTF_RAD + CurrDesText.xref_type, True);
	SetDlgState(dlgId, DLG_DTXA_QTS_RAD + CurrDesText.quote_usage, True);
	SetDlgState(dlgId, DLG_DTXA_TBR_RAD + CurrDesText.occluded_tabs, True);

	SetDlgState(dlgId, DLG_DTXA_FMSP_CK, CurrDesText.frame_spacing);
	SetDlgState(dlgId, DLG_DTXA_CHST_CK, CurrDesText.char_styles);
	SetDlgState(dlgId, DLG_DTXA_SBHT_CK, CurrDesText.secbk_help);
	SetDlgState(dlgId, DLG_DTXA_SBWF_CK, CurrDesText.secbk_word);

	DesignerChanged = False;
}

VoidT GetAdvancedTextDesigner(F_ObjHandleT dlgId)
{
	CurrDesText.idx_usage = GetRadValue(dlgId, DLG_DTXA_IXS_RAD,
																					DLG_DTXA_IXN_RAD);

	CurrDesText.xref_usage = GetRadValue(dlgId, DLG_DTXA_XRS_RAD,
																					DLG_DTXA_XRS_RAD);
	CurrDesText.xref_type = GetRadValue(dlgId, DLG_DTXA_XTF_RAD,
																					DLG_DTXA_XTN_RAD);

	CurrDesText.quote_usage = GetRadValue(dlgId, DLG_DTXA_QTS_RAD,
																					DLG_DTXA_QTN_RAD);

	CurrDesText.occluded_tabs = GetRadValue(dlgId, DLG_DTXA_TBR_RAD,
																					DLG_DTXA_TBT_RAD);

	CurrDesText.frame_spacing = GetDlgState(dlgId, DLG_DTXA_FMSP_CK);
	CurrDesText.char_styles = GetDlgState(dlgId, DLG_DTXA_CHST_CK);
	CurrDesText.secbk_help = GetDlgState(dlgId, DLG_DTXA_SBHT_CK);
	CurrDesText.secbk_word = GetDlgState(dlgId, DLG_DTXA_SBWF_CK);

	TextChanged = True;
	DesignerChanged = False;
}

VoidT FreeAdvancedTextDesigner(F_ObjHandleT dlgId)
{
	/* no resources to release */
}


/* end of file m2rdtxa.c */

