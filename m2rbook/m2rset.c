/* m2rset.c is the FDK setup common features module, JHG  
 * Copyright 1998 Omni Systems, Inc.  All Rights Reserved.
 */

#include "m2rbook.h"

/* defines for common template dialog part */
#define DLG_TF_TPL_CK    0
#define DLG_TF_TPLNAME   1
#define DLG_TF_TPLBTN    2

#define DLG_TF_FLAGS     4
#define DLG_TF_LAST     18

/* import formats flags (in Fapidefs.h at line 257)
	FF_UFF_PGF     Import Paragraph Catalog formats
	FF_UFF_FONT    Import Character Catalog formats
	FF_UFF_PAGE    Import page layouts
	FF_UFF_TABLE   Import Table Catalog formats
	FF_UFF_COND    Import conditions
	FF_UFF_REFPAGE Import reference pages
	FF_UFF_VAR     Import variable formats
	FF_UFF_XREF    Import cross-reference formats
	FF_UFF_COLOR   Import colors
	FF_UFF_MATH    Import equation settings
	FF_UFF_DOCUMENT_PROPS   Import document settings
	FF_UFF_COMBINED_FONTS   Import combined fonts (Japanese)
	FF_UFF_REMOVE_PAGE_BREAKS   Remove all forced page breaks
	FF_UFF_REMOVE_EXCEPTIONS    Remove exceptions to formats 
 */

#define DLG_TF_PSTYLES   4
#define DLG_TF_CSTYLES   5
#define DLG_TF_TABLES    6
#define DLG_TF_XREFS     7
#define DLG_TF_CONDS     8
#define DLG_TF_CFONTS    9
#define DLG_TF_PAGES    10
#define DLG_TF_REFPAGES 11
#define DLG_TF_VARS     12
#define DLG_TF_COLORS   13
#define DLG_TF_EQUS     14
#define DLG_TF_DOCP     15

#define DLG_TF_OVERS    17
#define DLG_TF_PBRKS    18

#ifndef CFGTPL
#define DLG_TF_CNVRT    20
#define DLG_TF_GENUP    21
#endif

#ifdef CFGTPL
#define DLG_TF_GENUP    20
#define DLG_TF_CFG_CK   22
#define DLG_TF_CFGNAME  23
#define DLG_TF_CFGBTN   24
#endif

BoolT SetupCancelled = False;

BoolT ConfigTemplate = False;
StringT ConfigName = NULL;


/* Frame template functions */

VoidT SetFrameTemplateDlg(F_ObjHandleT dlgId, IntT first)
{
	UseTemplate = ConvertVars = GenUpBook = ConfigTemplate = False;
	SetDlgState(dlgId, first + DLG_TF_TPL_CK, UseTemplate);
	SetDlgText(dlgId, first + DLG_TF_TPLNAME, TemplateName);
	TemplateFlags = 0;
	EnableItems(dlgId, first + DLG_TF_TPLNAME, first + DLG_TF_LAST, False);

#ifndef CFGTPL
	SetDlgState(dlgId, first + DLG_TF_CNVRT, ConvertVars);
#endif
	SetDlgState(dlgId, first + DLG_TF_GENUP, GenUpBook);

#ifdef CFGTPL
	SetDlgState(dlgId, first + DLG_TF_CFG_CK, ConfigTemplate);
	EnableItems(dlgId, first + DLG_TF_CFGNAME, first + DLG_TF_CFGBTN, False);
#endif
}


VoidT GetFrameTemplateDlg(F_ObjHandleT dlgId, IntT first)
{
	IntT i = 0;

	if ((UseTemplate = GetDlgState(dlgId, first + DLG_TF_TPL_CK)) != 0) {
		TemplateName = GetDlgText(dlgId, first + DLG_TF_TPLNAME);
		TemplateFlags = 0;

		for (i = DLG_TF_FLAGS; i <= DLG_TF_LAST; i++) {
			if (GetDlgState(dlgId, first + i)) {
				switch (i) {
					case DLG_TF_PSTYLES:
						TemplateFlags += FF_UFF_PGF;
						break;
					case DLG_TF_CSTYLES:
						TemplateFlags += FF_UFF_FONT;
						break;
					case DLG_TF_TABLES:
						TemplateFlags += FF_UFF_TABLE;
						break;
					case DLG_TF_XREFS:
						TemplateFlags += FF_UFF_XREF;
						break;
					case DLG_TF_CONDS:
						TemplateFlags += FF_UFF_COND;
						break;
					case DLG_TF_CFONTS:
						TemplateFlags += FF_UFF_COMBINED_FONTS;
						break;
					case DLG_TF_PAGES:
						TemplateFlags += FF_UFF_PAGE;
						break;
					case DLG_TF_REFPAGES:
						TemplateFlags += FF_UFF_REFPAGE;
						break;
					case DLG_TF_VARS:
						TemplateFlags += FF_UFF_VAR;
						break;
					case DLG_TF_COLORS:
						TemplateFlags += FF_UFF_COLOR;
						break;
					case DLG_TF_EQUS:
						TemplateFlags += FF_UFF_MATH;
						break;
					case DLG_TF_DOCP:
						TemplateFlags += FF_UFF_DOCUMENT_PROPS;
						break;
					case DLG_TF_OVERS:
						TemplateFlags += FF_UFF_REMOVE_EXCEPTIONS;
						break;
					case DLG_TF_PBRKS:
						TemplateFlags += FF_UFF_REMOVE_PAGE_BREAKS;
						break;
					default:
						break;
				}
			}
		}
	}

#ifndef CFGTPL
	ConvertVars = GetDlgState(dlgId, first + DLG_TF_CNVRT);
#endif
	GenUpBook = GetDlgState(dlgId, first + DLG_TF_GENUP);
#ifdef CFGTPL
	if ((ConfigTemplate = GetDlgState(dlgId, first + DLG_TF_CFG_CK)) != 0)
		ConfigName = GetDlgText(dlgId, first + DLG_TF_CFGNAME);
#endif
}


VoidT ProcFrameTemplateDlgEvent(F_ObjHandleT dlgId, IntT first, IntT itemNum)
{
	StringT newname = NULL;
	IntT err = 0;

	switch (itemNum - first) {

		case DLG_TF_TPL_CK:
			if (GetDlgState(dlgId, itemNum)) {
				if (!UseTemplate) { /* enable all items */
					EnableItems(dlgId, first + DLG_TF_TPLNAME,
					                   first + DLG_TF_LAST, True);
					UseTemplate = True;
				}
			}
			else {
				if (UseTemplate) { /* disable all items */
					EnableItems(dlgId, first + DLG_TF_TPLNAME,
					                   first + DLG_TF_LAST, False);
					UseTemplate = False;
				}
			}
			break;

		case DLG_TF_TPLNAME:
			/* prohibit direct changes from typing in box */
			SetDlgText(dlgId, first + DLG_TF_TPLNAME, TemplateName);
			break;

		case DLG_TF_TPLBTN:
#if 0
			err = F_ApiChooseFile(&newname, ChooseTemplateFileStr,
								CurrProjPath, TemplateName, FV_ChooseSelect, (StringT) "");
			if (!err
			 && (newname != NULL)) {
				SetDlgText(dlgId, first + DLG_TF_TPLNAME, TemplateName = newname);
				newname = NULL;
			}
#else
			M2GGetOpenFile(ChooseTemplateFileStr, CurrProjPath, &newname);
			if (newname != NULL) {
				SetDlgText(dlgId, first + DLG_TF_TPLNAME, TemplateName = newname);
				newname = NULL;
			}
#endif
			break;

#ifdef CFGTPL
		case DLG_TF_CFG_CK:
			if (GetDlgState(dlgId, itemNum)) {
				if (!ConfigTemplate) { /* enable all items */
					EnableItems(dlgId, first + DLG_TF_CFGNAME,
					                   first + DLG_TF_CFGBTN, True);
					ConfigTemplate = True;
				}
			}
			else {
				if (ConfigTemplate) { /* disable all items */
					EnableItems(dlgId, first + DLG_TF_CFGNAME,
					                   first + DLG_TF_CFGBTN, False);
					ConfigTemplate = False;
				}
			}
			break;

		case DLG_TF_CFGNAME:
			/* prohibit direct changes from typing in box */
			SetDlgText(dlgId, first + DLG_TF_CFGNAME, ConfigName);
			break;

		case DLG_TF_CFGBTN:
			M2GGetOpenFile(ChooseTemplateFileStr, CurrProjPath, &newname);
			if (newname != NULL) {
				SetDlgText(dlgId, first + DLG_TF_TPLNAME, ConfigName = newname);
				newname = NULL;
			}
			break;
#endif

		default:
			break;
	}
}


VoidT WriteIniCommonVars(BoolT fmfiles, BoolT idpath)
{
	StringT temp1 = NULL;

	// from m2rset.c

	if (UseTemplate) {
		M2GIniPutBool(CurrIniName, Setup, "ApplyTemplateFile", UseTemplate);
		M2GIniPutSetting(CurrIniName, Setup, "TemplateFileName", temp1 = Fm2a(TemplateName));
		F_Free(temp1);
		M2GIniPutInt(CurrIniName, Setup, "AppliedTemplateFlags", TemplateFlags);
	}

	if (ConvertVars)
		M2GIniPutBool(CurrIniName, Setup, "ConvertVariables", ConvertVars);

	if (GenUpBook)
		M2GIniPutBool(CurrIniName, Setup, "GenerateBook", GenUpBook);

	// from several m2rsets
	if (fmfiles) {
		M2GIniPutBool(CurrIniName, Setup, "UseFrameTOC", UseFmTOC);
		M2GIniPutBool(CurrIniName, Setup, "UseFrameIX", UseFmIX);
		M2GIniPutBool(CurrIniName, Setup, "UseFrameGenFiles", UseFmGenFiles);
	}

	M2GIniPutSetting(CurrIniName, Setup, "PrjFileName", temp1 = Fm2a(CurrPrjFilePath));
	F_Free(temp1);

	if (idpath
	 && CurrFileIDPath) {
		M2GIniPutSetting(CurrIniName, Setup, "IDFileName", CurrFileIDPath);
		_chmod(CurrFileIDPath, _S_IWRITE);
		SetFileID(NULL); // sets NextFileID
	}
}




/* end of m2rset.c */

