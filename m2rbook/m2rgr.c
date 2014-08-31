/* m2rgr.c is the graphics FDK module for mif2rtf for FM 5.5
 * it is used by both the m2r and the m2rbook modules.
 * Last edited on 11/28/98 by Jeremy H. Griffith.
 * Copyright 1998 Omni Systems, Inc.  All Rights Reserved.
 */


#include "m2rbook.h"


/* globals */

BoolT GraphicsChanged = False;

GraphPropT CurrDesGraph;

GraphPropT WDefDesGraph = {  // Word
	False, False, "100", "WMF",
	True, "120", "125", False, "8", "4", False, "5", "3",
	"4", "4", False, False, True, True, True, 0,
	0, 0, "20", "100", "0", "0", 0, False, False,
	1, "", False, NULL, 0, 0,
	False, "0", False, False, True, 0
};

GraphPropT HDefDesGraph = {  // WinHelp
	True, False, "100", "WMF",
	True, "120", "125", False, "0", "0", False, "5", "3",
	"4", "4", True, True, True, True, True, 0,
	0, 0, "20", "100", "0", "0", 0, False, False,
	1, "", False, NULL, 1, 0,
	False, "0", False, False, True, 0
};

GraphPropT DefDesGraph;
GraphPropT ClipboardGraph;

StringT FDK = { "FDK" };
StringT Graphics = { "Graphics" };
StringT GraphExport = { "GraphExport" };
StringT GraphFiles = { "GraphFiles" };

StringT Metafiles[3] = { "Internal", "External", NULL };
StringT FilePaths[3] = { "Retain", "None", NULL };
StringT FileNames[3] = { "Retain", "Map", NULL };

StringT RefFrames[4] = { "None", "Graphics", "Text", NULL };
StringT EmptyFrames[4] = { "Standard", "Remove", "Identify", NULL };

StringT EpsiUsage[4] = { "Preview", "EPS", "Retain", NULL };
StringT RasterBorders[5] = { "None", "Inside", "Outside", "Frame", NULL };
StringT StretchMode[5] = { "0", "1", "2", "3", NULL };

StringT GraphText[3] = { "Embed", "Frame", NULL };
StringT ClipType[4] = { "Move", "Show", "Delete", NULL };
StringT BackMode[3] = { "1", "2", NULL };


/* graph format list */

StringT DesGrFormList[DES_GRX_FORM_COUNT + 2] = {
 "Format",
 "BMP", "CGM", "EPS", "IGS", "JPG",
 "PIC", "PNG", "TIF", "WMF", "GIF",
 "SVG", "PDF", NULL
};

F_StringsT GrFormStrings = { DES_GRX_FORM_COUNT + 1, DesGrFormList };

/* value to use for matching pos in GrFormList */
IntT DesGrFormVal[DES_GRX_FORM_COUNT + 1] = {
 0,
 GF_BMP, GF_CGM, GF_EPS, GF_IGES, GF_JPEG,
 GF_PICT, GF_PNG, GF_TIFF, GF_WMF, GF_GIF,
 GF_SVG, GF_PDF
 };
/* index to GrFormList name for each type */
IntT DesGrFormMap[DES_GRX_FORM_COUNT + 1] = { 
 9,   /* default to WMF */
 1,  8,  9,  5,  7,
 3,  6,  2,  4, 10,
11, 12
 };


IntT FindGrFormInList(StringT fname)
{
	IntT fnum = 1;

	if (fname && *fname) {
		while (fnum <= DES_GRX_FORM_COUNT) {
			if (!F_StrICmpN(fname, DesGrFormList[fnum], 3))
				return fnum;  /* don't map, used for display set only */
			fnum++;
		}
	}

	return -1;
}


/* imported graphics format list */

StringT ImportDesNames[DES_GRI_FORM_COUNT + 1] = {
 "Formats", "BMP", "CDR", "EPS",
 "GIF", "JPG", "OLE", "PCT", "PCX",
 "RF", "TIF", "WMF", "WPG"
};

BoolT ExportGraphsChanged = False;

GrExPropT ExportGraphs[DES_GRI_FORM_COUNT + 1];

GrExPropT ExportGraphRead[DES_GRI_FORM_COUNT + 1];

GrExPropT ExportGraphDef[DES_GRI_FORM_COUNT + 1] = {
	{ NULL, False, NULL },  // Dummy
	{ "bmp", True, NULL },
	{ "cdr", False, NULL },
	{ "eps", False, NULL },
	{ "gif", False, NULL },
	{ "jpg", False, NULL },
	{ "ole", True, NULL },
	{ "pct", False, NULL },
	{ "pcx", False, NULL },
	{ "rf", True, NULL },
	{ "tif", False, NULL },
	{ "wmf", True, NULL },
	{ "wpg", False, NULL }
};

GrExPropT *CurrExportGraph = &ExportGraphs[1];
IntT CurrExportGraphNum = 1;

GrExPropT ClipboardExportGraph = { NULL, False, NULL };
IntT ClipboardExGrNum = 0; // index of item to affect


IntT FindGrExFormInList(StringT fname)
{
	IntT fnum = 1;

	if (fname && *fname) {
		while (fnum <= DES_GRX_FORM_COUNT) {
			if (!F_StrICmp(fname, DesGrFormList[fnum]))
				return fnum;
			fnum++;
		}
	}

	return -1;
}


VoidT SetNewGrForm(IntT formnum)
{
	CurrExportGraphNum = formnum;
	CurrExportGraph = &ExportGraphs[formnum];
}


VoidT ReadExportGraphPropsFromIni(StringT ininame, BoolT confirm)
{
	IntT i = 0;
	UCharT pname[16];
	BoolT expval = False;


	for (i = 1; i <= DES_GRI_FORM_COUNT; i++) {
		ExportGraphs[i] = ExportGraphDef[i];
		F_Sprintf(pname, "Export%c%sFiles", F_CharToUpper(ExportGraphs[i].suf[0]),
																				&ExportGraphs[i].suf[1]);
		expval = ExportGraphs[i].retain ? False : True;
		TrueFalseIni(ininame, GraphExport, pname, &expval);
		ExportGraphs[i].retain = expval ? False : True;
		GetIniString(ininame, GraphFiles, ExportGraphs[i].suf, &(ExportGraphs[i].remap));
		ExportGraphRead[i] = ExportGraphs[i];
	}

	ExportGraphsChanged = False;
}


VoidT WriteExportGraphPropsToIni(StringT ininame, BoolT confirm)
{
	IntT i = 0;
	UCharT pname[16];

	if (!ExportGraphsChanged)
		return;

	for (i = 1; i <= DES_GRI_FORM_COUNT; i++) {
		if (ExportGraphs[i].retain != ExportGraphRead[i].retain) {
			F_Sprintf(pname, "Export%c%sFiles", F_CharToUpper(ExportGraphs[i].suf[0]),
																					&ExportGraphs[i].suf[1]);
			SetIniTF(ininame, GraphExport, pname, !(ExportGraphs[i].retain));
		}
		if ((ExportGraphs[i].remap != ExportGraphRead[i].remap)
		 && ((ExportGraphs[i].remap == NULL)
		  || (ExportGraphRead[i].remap == NULL)
			|| F_StrICmp(ExportGraphs[i].remap, ExportGraphRead[i].remap)))
			SetIniStr(ininame, GraphFiles, ExportGraphs[i].suf, ExportGraphs[i].remap);
		ExportGraphRead[i] = ExportGraphs[i];
	}

	ExportGraphsChanged = False;
}



/* graphics .ini updating */


VoidT ReadGraphPropsFromIni(StringT ininame, BoolT confirm)
{
	BoolT val = False;
	BoolT val2 = False;
	IntT ival = 0;

	CurrDesGraph = DefDesGraph;

	// Basic Graphics
	ival = 0;
	GetIniMatch(ininame, SubOptions, "Metafiles", Metafiles, &ival);
	if (ival) 
		CurrDesGraph.external = (ival - 1) ? True : False;

	TrueFalseIni(ininame, Options, "UseGraphicPreviews", &val);
	TrueFalseIni(ininame, FDK, "WriteAllGraphics", &val2);
	CurrDesGraph.fdkprv_used = (val || val2);

	GetIniString(ininame, FDK, "GraphicExportDPI", &CurrDesGraph.fdkprv_dpi);
	GetIniString(ininame, FDK, "GraphicExportFormat", &CurrDesGraph.fdkgr_form);

	TrueFalseIni(ininame, FDK, "WriteEquations", &CurrDesGraph.fdkeqn_used);
	GetIniString(ininame, FDK, "EquationExportDPI", &CurrDesGraph.fdkeqn_dpi);
	GetIniString(ininame, FDK, "EquationFrameExpand", &CurrDesGraph.fdkeqn_frm);

	TrueFalseIni(ininame, Graphics, "FrameExactHeight", &CurrDesGraph.fexact);
	GetIniString(ininame, Options, "EqVertAdjust", &CurrDesGraph.eqn_vert);
	GetIniString(ininame, Graphics, "GrVertAdjust", &CurrDesGraph.gr_vert);

	TrueFalseIni(ininame, Graphics, "NameWMFsAsBMPs", &CurrDesGraph.name_bmp);
	GetIniString(ininame, Graphics, "MetaNameChars", &CurrDesGraph.wmfname_chars);
	GetIniString(ininame, Graphics, "MetaNumDigits", &CurrDesGraph.wmfname_digits);

	// Imported Graphics
	ReadExportGraphPropsFromIni(ininame, confirm);

	GetIniString(ininame, GraphExport, "ExportNameChars", &CurrDesGraph.exname_chars);
	GetIniString(ininame, GraphExport, "ExportNumDigits", &CurrDesGraph.exname_digits);

	ival = 0;
	GetIniMatch(ininame, SubOptions, "FilePaths", FilePaths, &ival);
	if (ival) 
		CurrDesGraph.strip_path = (ival - 1) ? True : False;

	ival = 0;
	GetIniMatch(ininame, SubOptions, "FileNames", FileNames, &ival);
	if (ival) 
		CurrDesGraph.remap_name = (ival - 1) ? True : False;

	TrueFalseIni(ininame, Graphics, "EmbedBMPsInWMFs", &CurrDesGraph.wrap_bmp);
	TrueFalseIni(ininame, Graphics, "EmbedWMFsInWMFs", &CurrDesGraph.wrap_wmf);
	TrueFalseIni(ininame, Options, "EmbedEqsInWMFs", &CurrDesGraph.wrap_eqn);

	ival = 0;
	GetIniMatch(ininame, Graphics, "EpsiUsage", EpsiUsage, &ival);
	if (ival) 
		CurrDesGraph.epsi_usage = ival - 1;

	// Graphic Text
	ival = 0;
	GetIniMatch(ininame, SubOptions, "GraphText", GraphText, &ival);
	if (ival) 
		CurrDesGraph.graph_text = ival - 1;

	ival = 0;
	GetIniMatch(ininame, Graphics, "ClipType", ClipType, &ival);
	if (ival) 
		CurrDesGraph.clip_type = ival - 1;

	GetIniString(ininame, Graphics, "ClipLimit", &CurrDesGraph.clip_limit);

	GetIniString(ininame, Graphics, "TextScale", &CurrDesGraph.text_scale);
	GetIniString(ininame, Graphics, "TextWidth", &CurrDesGraph.text_width);
	GetIniString(ininame, Graphics, "TextVertAdjust", &CurrDesGraph.text_vert);

	ival = 0;
	GetIniMatch(ininame, Graphics, "BackMode", BackMode, &ival);
	if (ival) 
		CurrDesGraph.back_mode = ival - 1;

	TrueFalseIni(ininame, Graphics, "UseTopSpaceAbove", &CurrDesGraph.sp_above);

	val = CurrDesGraph.blank_first ? False : True;
	TrueFalseIni(ininame, Graphics, "NoBlankFirstGTLine", &val);
	CurrDesGraph.blank_first = val ? False : True;


	// Graphic Frames

	ival = 0;
	GetIniMatch(ininame, SubOptions, "RefFrames", RefFrames, &ival);
	if (ival) {
		CurrDesGraph.refframe_typ = ival - 1;
		GetIniString(ininame, SubOptions, "RefFrameDefFormat", &CurrDesGraph.refframe_form);
	}
	else {
		GetIniString(ininame, SubOptions, "RefFrameFormat", &CurrDesGraph.refframe_form);
		CurrDesGraph.refframe_typ = IniBufEmpty() ? 1 :
                             ((CurrDesGraph.refframe_form[0] == '\0') ? 1 : 2 );
	}

	GetIniString(ininame, SubOptions, "FrameStyle", &CurrDesGraph.frstyle_name);
	CurrDesGraph.frstyle_used = (CurrDesGraph.frstyle_name && *CurrDesGraph.frstyle_name) ?
															True : False;

	ival = 0;
	GetIniMatch(ininame, SubOptions, "EmptyFrames", EmptyFrames, &ival);
	if (ival) 
		CurrDesGraph.empty_frames = ival - 1;

	ival = 0;
	GetIniMatch(ininame, Graphics, "RasterBorders", RasterBorders, &ival);
	if (ival) 
		CurrDesGraph.ras_borders = ival - 1;


	// Bitmaps

	GetIniString(ininame, Graphics, "BitmapDPI", &CurrDesGraph.bitmap_dpi);
	CurrDesGraph.bitmap_rescale = (CurrDesGraph.bitmap_dpi
															&& (CurrDesGraph.bitmap_dpi[0] != '0'));

	TrueFalseIni(ininame, Graphics, "BitmapFlip", &CurrDesGraph.bitmap_flip);
	TrueFalseIni(ininame, SubOptions, "Transparent", &CurrDesGraph.bitmap_trans);
	TrueFalseIni(ininame, Graphics, "CompressRasters", &CurrDesGraph.compress_bmps);

	ival = 0;
	GetIniMatch(ininame, Graphics, "StretchMode", StretchMode, &ival);
	if (ival) 
		CurrDesGraph.stretch_mode = ival - 1;


	GraphicsChanged = False;
}


VoidT WriteGraphPropsToIni(StringT ininame, BoolT confirm)
{
	if (!GraphicsChanged)
		return;

	// Basic Graphics
	SetIniStr(ininame, SubOptions, "Metafiles", Metafiles[CurrDesGraph.external ? 1 : 0]);

	SetIniTF(ininame, Options, "UseGraphicPreviews", CurrDesGraph.fdkprv_used);
	SetIniTF(ininame, FDK, "WriteAllGraphics", CurrDesGraph.fdkprv_used);

	SetIniStr(ininame, FDK, "GraphicExportDPI", CurrDesGraph.fdkprv_dpi);
	SetIniStr(ininame, FDK, "GraphicExportFormat", CurrDesGraph.fdkgr_form);

	SetIniTF(ininame, FDK, "WriteEquations", CurrDesGraph.fdkeqn_used);
	SetIniStr(ininame, FDK, "EquationExportDPI", CurrDesGraph.fdkeqn_dpi);
	SetIniStr(ininame, FDK, "EquationFrameExpand", CurrDesGraph.fdkeqn_frm);

	SetIniTF(ininame, Graphics, "FrameExactHeight", CurrDesGraph.fexact);
	SetIniStr(ininame, Options, "EqVertAdjust", CurrDesGraph.eqn_vert);
	SetIniStr(ininame, Graphics, "GrVertAdjust", CurrDesGraph.gr_vert);

	SetIniTF(ininame, Graphics, "NameWMFsAsBMPs", CurrDesGraph.name_bmp);
	SetIniStr(ininame, Graphics, "MetaNameChars", CurrDesGraph.wmfname_chars);
	SetIniStr(ininame, Graphics, "MetaNumDigits", CurrDesGraph.wmfname_digits);

	// Imported Graphics
	WriteExportGraphPropsToIni(ininame, confirm);

	SetIniStr(ininame, GraphExport, "ExportNameChars", CurrDesGraph.exname_chars);
	SetIniStr(ininame, GraphExport, "ExportNumDigits", CurrDesGraph.exname_digits);

	SetIniStr(ininame, SubOptions, "FilePaths", FilePaths[CurrDesGraph.strip_path ? 1 : 0]);
	SetIniStr(ininame, SubOptions, "FileNames", FileNames[CurrDesGraph.remap_name ? 1 : 0]);

	SetIniTF(ininame, Graphics, "EmbedBMPsInWMFs", CurrDesGraph.wrap_bmp);
	SetIniTF(ininame, Graphics, "EmbedWMFsInWMFs", CurrDesGraph.wrap_wmf);
	SetIniTF(ininame, Options, "EmbedEqsInWMFs", CurrDesGraph.wrap_eqn);

	SetIniStr(ininame, Graphics, "EpsiUsage", EpsiUsage[CurrDesGraph.epsi_usage]);

	// Graphic Text

	SetIniStr(ininame, SubOptions, "GraphText", GraphText[CurrDesGraph.graph_text]);

	SetIniStr(ininame, Graphics, "ClipType", ClipType[CurrDesGraph.clip_type]);

	SetIniStr(ininame, Graphics, "ClipLimit", CurrDesGraph.clip_limit);

	SetIniStr(ininame, Graphics, "TextScale", CurrDesGraph.text_scale);
	SetIniStr(ininame, Graphics, "TextWidth", CurrDesGraph.text_width);
	SetIniStr(ininame, Graphics, "TextVertAdjust", CurrDesGraph.text_vert);

	SetIniStr(ininame, Graphics, "BackMode", BackMode[CurrDesGraph.back_mode]);

	SetIniTF(ininame, Graphics, "UseTopSpaceAbove", CurrDesGraph.sp_above);
	SetIniTF(ininame, Graphics, "NoBlankFirstGTLine", CurrDesGraph.blank_first ? False : True);

	// Graphic Frames
	SetIniStr(ininame, SubOptions, "RefFrameFormat", NULL);  // remove, deprecated
	SetIniStr(ininame, SubOptions, "RefFrames", RefFrames[CurrDesGraph.refframe_typ]);
	SetIniStr(ininame, SubOptions, "RefFrameDefFormat", CurrDesGraph.refframe_form);

	SetIniStr(ininame, SubOptions, "FrameStyle", CurrDesGraph.frstyle_used ?
																							 CurrDesGraph.frstyle_name : NULL);

	SetIniStr(ininame, SubOptions, "EmptyFrames", EmptyFrames[CurrDesGraph.empty_frames]);
	SetIniStr(ininame, Graphics, "RasterBorders", RasterBorders[CurrDesGraph.ras_borders]);


	// Bitmaps
	SetIniStr(ininame, Graphics, "BitmapDPI", CurrDesGraph.bitmap_rescale ?
																						CurrDesGraph.bitmap_dpi : "0");

	SetIniTF(ininame, Graphics, "BitmapFlip", CurrDesGraph.bitmap_flip);

	if (CurrPropSetNum == DES_PROPSET_HELP)
		SetIniTF(ininame, SubOptions, "Transparent", CurrDesGraph.bitmap_trans);

	SetIniTF(ininame, Graphics, "CompressRasters", CurrDesGraph.compress_bmps);

	SetIniStr(ininame, Graphics, "StretchMode", StretchMode[CurrDesGraph.stretch_mode]);


	GraphicsChanged = False;
}



/* End of m2rgr.c */

