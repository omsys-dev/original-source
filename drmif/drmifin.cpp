// drmifin.cpp is the code for dcl MIF ini processing, JHG, 4/21/05
// Copyright (c) 2005 by Omni Systems, Inc.  All rights reserved.

#include "dcappl.h"
#include "dcctlg.h"
#include "dcfili.h"
#include "dcini.h"
#include "drmif.h"
#include "drmiffl.h"


// list of all mif2*.ini sections processed in drmif.dll

secinfo MIFrd::MIFIniSects[] = {
 { "Options", set, "OptionsSet", ISecOptions, true },
 { "FDK", set, "FDKSet", ISecFDK, true },
 { "Setup", set, "FDKSet", ISecFDK, true },
 { "Graphics", set, "GraphicsSet", ISecGraphics, true },
 { "GraphExport", set, "GraphicsSet", ISecGraphExport, true },
 { "Logging", set, "LoggingSet", ILogOptions, true },

 { "WordOptions", set, "OptionsSet", ISecOptions, false },
 { "HelpOptions", set, "OptionsSet", ISecOptions, false },
 { "HTMLOptions", set, "OptionsSet", ISecOptions, false },
 { NULL, unknown, NULL, NULL, false }
};


// OptionsSet

setinfo MIFrd::ISecOptions[] = {
 { "CaselessMatch", yesno, &CaselessMatch, CaselessMatchSet },
 { "SpacelessMatch", yesno, &SpacelessMatch, SpacelessMatchSet },
 { "WildcardMatch", yesno, &WildcardMatch, WildcardMatchSet },

 { "XrefType", cap, &XrefType, NULL },
 { "RemoveWordTocMarkers", yesno, &RemoveWordTocMarkers, NULL },
 { "MarkerType11", cap, &Mark11, NULL },
 { "ALinkMarkerType", usnum, &ALinkMarkerType, NULL },
 { "DirectiveMarker", usnum, &DirectiveMark, NULL },
 { "MathType", usnum, &MathForm, NULL },
 { "MathFullForm", yesno, &MathFullForm, NULL },
 { "HTSuffix", str, &HTSuffix, NULL },
 { "EqSuffix", str, &EqnSuffix, NULL },

 { "IDAttrName", str, &IDAttrName, NULL },
 { "IDRefAttrName", str, &IDRefAttrName, NULL },

 { "UseGraphicPreviews", yesno, &UseGrPreviews, NULL },

 // for cd-europe type 18 markers
 { "ExtraIXMarkerType", usnum, &ExtraIXMarkerType, NULL },
 { "ExtraIXMarkerName", str, &ExtraIXMarkerName, NULL },
 { "ExtraIXPrefix", str, &ExtraIXPrefix, NULL },

 // undocumented and untested
 { "UseAllGifs", yesno, &UseAllGifs, NULL },

 { NULL, spec, NULL, NULL }
};


void
MIFrd::CaselessMatchSet(char *)
{
	IniFile->SetMatch(1, CaselessMatch);
}

void
MIFrd::SpacelessMatchSet(char *)
{
	IniFile->SetMatch(2, SpacelessMatch);
}

void
MIFrd::WildcardMatchSet(char *)
{
	IniFile->SetMatch(3, WildcardMatch);
}


// FDKSet

setinfo MIFrd::ISecFDK[] = {
 { "UseLocalFileID", yesno, &UseLocalFileID, NULL },
 { "UseGraphicFileID", yesno, &UseGrFileID, NULL },
 { "GraphicNameDigits", lnum, &GrFileNumLen, GraphicNameDigitsSet },
 { "GraphicExportFormat", cap, &GrExFormat, GrExFormatSet },

 { "IDFileName", path, &IDFileName, NULL },
 { "PrjFileName", path, &PrjFileName, NULL },

 { NULL, spec, NULL, NULL }
};

void
MIFrd::GraphicNameDigitsSet(char *)
{
	if (GrFileNumLen > 7) {
		GrFileNumLen = 8;
		GrFileNumMask = 0x7FFFFFFF;
	}
	else {
		if (GrFileNumLen < 4)
			GrFileNumLen = 4;
		GrFileNumMask = 0;
		for (long i = 0; i < GrFileNumLen; i++) {
			GrFileNumMask <<= 4;
			GrFileNumMask += 0x0F;
		}
	}
}

void
MIFrd::GrExFormatSet(char *fname)
{
	switch (GrExFormat) {
		case 'G':
			GrType = 1;
			GrSuffix = "gif";
			break;
		case 'B':
		case 'D':
			GrType = 2;
			GrSuffix = "bmp";
			break;
		case 'T':
			GrType = 3;
			GrSuffix = "tif";
			break;
		case 'J':
			GrType = 5;
			GrSuffix = "jpg";
			break;
		case 'P':
			if (!_stricmp(fname, "pict")) {
				GrType = 8;
				GrSuffix = "pct";
			}
			else if (!_stricmp(fname, "pdf")) {
				GrType = 12;
				GrSuffix = "pdf";
			}
			else {
				GrType = 6;
				GrSuffix = "png";
			}
			break;
		case 'E':
			GrType = 7;
			GrSuffix = "eps";
			break;
		case 'C':
			GrType = 9;
			GrSuffix = "cgm";
			break;
		case 'I':
			GrType = 10;
			GrSuffix = "igs";
			break;
		case 'S':
			GrType = 11;
			GrSuffix = "svg";
			break;

		default:
		case 'W':
			GrType = 4;
			GrSuffix = "wmf";
			break;
	}
}




// GraphicsSet

setinfo MIFrd::ISecGraphics[] = {
 { "EpsiUsage", cap, &EpsiUsage, NULL },
 { "FrameDefaultFontName", str, &DefGrTextName, NULL },
 { "FrameDefaultFontSize", lnum, &DefGrTextSize, NULL },

 { "UseGraphicPreviews", yesno, &UseGrPreviews, NULL },
 { "UseDefaultGraphicFormat", yesno, &UseDefGrCForm, NULL },
 { "UseFrameImage", yesno, &UseFrameImage, NULL },

 { "FixGraphicNames", yesno, &FixGraphicNames, NULL },
 { "GraphicRenameBat", str, &GraphicRenameBat, NULL },

 // undocumented
 { "BezierFlatness", lnum, &BezFlatness, NULL },

 { NULL, spec, NULL, NULL }
};


// GraphExportSet

setinfo MIFrd::ISecGraphExport[] = {
 { "ImportGraphics", cap, &ImpGraph, ImpGraphSet },
 { "ExportNameChars", lnum, &ExNameWid, NULL },
 { "ExportNumDigits", lnum, &ExNumWid, NULL },

 { "ExportWmfFiles", yesno, &ExWmfFiles, NULL },
 { "ExportBmpFiles", yesno, &ExBmpFiles, NULL },
 { "ExportRfFiles", yesno, &ExRfFiles, NULL },
 { "ExportPctFiles", yesno, &ExPctFiles, NULL },
 { "ExportTifFiles", yesno, &ExTifFiles, NULL },
 { "ExportPcxFiles", yesno, &ExPcxFiles, NULL },
 { "ExportWpgFiles", yesno, &ExWpgFiles, NULL },
 { "ExportGifFiles", yesno, &ExGifFiles, NULL },
 { "ExportJpgFiles", yesno, &ExJpgFiles, NULL },
 { "ExportPngFiles", yesno, &ExPngFiles, NULL },
 { "ExportEpsFiles", yesno, &ExEpsFiles, NULL },
 { "ExportCdrFiles", yesno, &ExCdrFiles, NULL },
 { "ExportOleFiles", yesno, &ExOleFiles, NULL },
 { "MultipleOLE", yesno, &MultipleOLE, MultipleOLESet },

 { NULL, spec, NULL, NULL }
};


void
MIFrd::ImpGraphSet(char *)
{
	if (ImpGraph == 'E') {
		ExFiles = true;
		ExWmfFiles = true;
		ExBmpFiles = true;
		ExRfFiles = true;
		ExOleFiles = true;
	}
	else if (ImpGraph == 'R') {
		ExPctFiles = false;
		ExTifFiles = false;
		ExGifFiles = false;
		ExJpgFiles = false;
		ExPngFiles = false;
		ExPcxFiles = false;
		ExWpgFiles = false;
		ExCdrFiles = false;
		ExOleFiles = false;
		ExEpsFiles = false;
	}
}

void
MIFrd::MultipleOLESet(char *)
{
	if (MultipleOLE) {  // make other required settings
		ExOleFiles = false;
		ExWmfFiles = true;
	}
}


// LoggingSet

setinfo MIFrd::ILogOptions[] = {
 { "UseLog", yesno, &UseLog, NULL },
 { "ShowLog", yesno, &ShowLog, NULL },
 { "LogFileName", path, &LogFileName, NULL },
 { "HistoryFileName", path, &HistoryFileName, NULL },
 { "EditorFileName", path, &EditorFileName, NULL },
 { "LogIniChains", yesno, &LogIniChains, NULL },
 { "LogErrors", ucnum, &LogLevels[logerr], NULL },
 { "LogWarnings", ucnum, &LogLevels[logwarn], NULL },
 { "LogQuerys", ucnum, &LogLevels[logquery], NULL },
 { "LogInfo", ucnum, &LogLevels[loginfo], NULL },
 { "LogDebug", ucnum, &LogLevels[logdebug], NULL },

 { NULL, spec, NULL, NULL }
};

// end of drmifin.cpp

