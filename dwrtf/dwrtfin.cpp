// dwrtfin.cpp is the code for dcl RTF ini processing, JHG, 3/22/05
// Copyright (c) 2005 by Omni Systems, Inc.  All rights reserved.

#include "dcappl.h"
#include "dcctlg.h"
#include "dcfili.h"
#include "dctxfx.h"
#include "dcfmtr.h"
#include "dcini.h"
#include "dcmacr.h"
#include "dcmxl.h"
#include "dwrtf.h"
#include "dwrtffl.h"


// list of all mif2rtf.ini sections (except for macros)

/***********
The following sections in mif2rtf.ini are settable and 
readable by macros, and are settable by markers.

Section for both Word and WinHelp:
Options, Defaults, FDK, Graphics, Table, Inserts, XrefStyles,
StyleCodeBefore, StyleCodeStart, StyleCodeEnd, StyleCodeAfter, 
StyleCodeReplace, AnumCodeBefore, AnumCodeAfter, StyleCodeStore,
StyleTextStore 

Sections for Word only:
WordOptions, WordStyles, WordCntStyles, WordReplacements

Sections for WinHelp only:
HelpOptions, HelpBrowse, HelpContents, HelpStyles, HelpRefStyles, 
HelpBrowsePrefixStyles, HelpKeywordStyles, HelpJumpFileStyles, 
HelpWindowStyles, HelpMacroStyles, HelpTopicBuildStyles, 
HelpSuffixStyles, HelpTitleSufStyles, HelpInfoTypeStyles, 
HelpCntStyles, HelpReplacements
**************/

secinfo RTFwr::RTFIniSects[] = {

 // global settings always checked:
 { "Options", set, "InitSet", ISecInitOptions, true },
 { "uDocOptions", set, "OptionsSet", ISecUdocOptions, true },
 { "Automation", set, "OptionsSet", ISecAuto, true },
 { "Defaults", set, "DefaultsSet", ISecDefaults, true },
 { "Defaults", set, "DefaultsSet", ISecRefFrames, true },
 { "FDK", set, "FDKSet", ISecFDK, true },
 { "Setup", set, "FDKSet", ISecFDK, true },
 { "Logging", set, "LoggingSet", ILogOptions, true },

 { "Options", set, "OptionsSet", ISecOptions, false },
 { "Options", set, "OptionsSet", ISecRefFrames, false },

 { "WordOptions", set, "WordSet", ISecOptions, false },
 { "WordOptions", set, "WordSet", ISecRefFrames, false },
 { "WordOptions", set, "WordSet", ISecWordOptions, false },
 { "WordOptions", set, "WordSet", ISecTable, false },

 { "HelpOptions", set, "HelpSet", ISecOptions, false },
 { "HelpOptions", set, "HelpSet", ISecRefFrames, false },
 { "HelpOptions", set, "HelpSet", ISecHelpOptions, false },
 { "HelpOptions", set, "HelpSet", ISecTable, false },
 { "HelpBrowse", set, "HelpSet", ISecHelpBrowse, false },
 { "HelpContents", set, "HelpSet", ISecHelpContents, false },

 { "Graphics", set, "GraphicsSet", ISecGraphics, false },
 { "Options", set, "GraphicsSet", ISecGraphOpts, false },
 { "WordOptions", set, "GraphicsSet", ISecGraphOpts, false },
 { "HelpOptions", set, "GraphicsSet", ISecGraphOpts, false },
 { "Graphics", set, "GraphicsSet", ISecGraphOpts, false },

 { "Table", set, "TableSet", ISecTable, false },

 { "Index", set, "WordSet", ISecIndex, false },

 { "Inserts", set, "HelpSet", ISecHInserts, false },
 { "Inserts", set, "WordSet", ISecWInserts, false },

 // string sections read during style processing
 { "StyleCodeBefore", pcstyle, NULL, NULL, false, str, &Curr.maccb },
 { "StyleCodeStart", pcstyle, NULL, NULL, false, str, &Curr.maccs },
 { "StyleCodeEnd", pcstyle, NULL, NULL, false, str, &Curr.macce },
 { "StyleCodeAfter", pcstyle, NULL, NULL, false, str, &Curr.macca },
 { "StyleCodeReplace", pcstyle, NULL, NULL, false, str, &Curr.maccr, StyleCodeReplaceSet },
 { "AnumCodeBefore", pcstyle, NULL, NULL, false, str, &Curr.macacb },
 { "AnumCodeAfter", pcstyle, NULL, NULL, false, str, &Curr.macaca },
 { "StyleCodeStore", pcstyle, NULL, NULL, false, str, &Curr.cstore },
 { "StyleTextStore", pcstyle, NULL, NULL, false, str, &Curr.tstore },

 // help style settings
 { "HelpStyles", pcstyle, "HStyleSet", ISecHelpStyles, false },
 { "HelpStyles", pcstyle, "HStyleSet", ISecCommonStyles, false },
 { "HelpRefStyles", pcstyle, "HStyleSet", NULL, false, str, &Curr.href, HelpRefStylesSet },
 { "HelpBrowsePrefixStyles", pcstyle, "HStyleSet", NULL, false, str, &Curr.hbrowse },
 { "HelpKeywordStyles", pcstyle, "HStyleSet", NULL, false, spec, &Curr.hkey, HelpKeywordStylesSet },
 { "HelpJumpFileStyles", pcstyle, "HStyleSet", NULL, false, str, &Curr.hjfile },
 { "HelpWindowStyles", pcstyle, "HStyleSet", NULL, false, str, &Curr.hwindow },
 { "HelpMacroStyles", pcstyle, "HStyleSet", NULL, false, str, &Curr.hmacro },
 { "HelpTopicBuildStyles", pcstyle, "HStyleSet", NULL, false, str, &Curr.hbuild },
 { "HelpSuffixStyles", pcstyle, "HStyleSet", NULL, false, str, &Curr.hsuf },
 { "HelpTitleSufStyles", pcstyle, "HStyleSet", NULL, false, str, &Curr.htsuf },
 { "HelpInfoTypeStyles", pcstyle, "HStyleSet", NULL, false, str, &Curr.infot },
 { "HelpCntStyles", pcstyle, "HStyleSet", NULL, false, spec, &Curr.cntform, CntStylesSet },
 { "HelpReplacements", pcstyle, "HStyleSet", NULL, false, spec, &Curr.replace, ReplacementsSet },

 // word style settings
 { "WordStyles", pcstyle, "WStyleSet", ISecWordStyles, false },
 { "HelpStyles", pcstyle, "WStyleSet", ISecWordStyles, false },
 { "WordStyles", pcstyle, "WStyleSet", ISecCommonStyles, false },
 { "WordCntStyles", pcstyle, "WStyleSet", NULL, false, spec, &Curr.cntform, CntStylesSet },
 { "WordReplacements", pcstyle, "WStyleSet", NULL, false, spec, &Curr.replace, ReplacementsSet },

 // xref style settings
 { "XrefStyles", xrstyle, NULL, ISecXrefStyles, false },  // NOT used for .ini

 { NULL, unknown, NULL, NULL, false }
};


// AutoSet

setinfo RTFwr::ISecAuto[] = {
 { "WrapAndShip", yesno, &DoAutomation, NULL },
 { "CopyOriginalGraphics", yesno, &CopyOriginalGraphics, NULL },


 { NULL, spec, NULL, NULL }
};


// UdocOptionsSet

setinfo RTFwr::ISecUdocOptions[] = {
 { "WriteKeydefLib", yesno, &MXLwr::WriteKeydefLib, NULL },
 { "KeydefLibSrc", str, &MXLwr::OutputKeydefLibSrc, NULL },


 { NULL, spec, NULL, NULL }
};


// OptionsSet

setinfo RTFwr::ISecInitOptions[] = {
 { "CaselessMatch", yesno, &CaselessMatch, CaselessMatchSet },
 { "SpacelessMatch", yesno, &SpacelessMatch, SpacelessMatchSet },
 { "WildcardMatch", yesno, &WildcardMatch, WildcardMatchSet },
 { "ICUdll", str, &ICUdll, NULL },
 { NULL, spec, NULL, NULL }
};

void
RTFwr::CaselessMatchSet(char *)
{
	IniFile->SetMatch(1, CaselessMatch);
}

void
RTFwr::SpacelessMatchSet(char *)
{
	IniFile->SetMatch(2, SpacelessMatch);
}

void
RTFwr::WildcardMatchSet(char *)
{
	IniFile->SetMatch(3, WildcardMatch);
}


setinfo RTFwr::ISecHelpOptions[] = {
 { "HPJFileName", str, &HPJName, NULL },
 { "WriteHelpProjectFile", yesno, &WriteHelpProjectFile, NULL },
 { "HelpCopyright", str, &HelpCopyright, NULL },
 { "HelpCopyDate", yesno, &HelpCopyDate, NULL },

 { "Altura", yesno, &Altura, NULL },
 { "HyperHelp", yesno, &HyperHelp, HyperHelpSet },

 { "ForceBmc", yesno, &ForceBmc, NULL },
 { "BMPsForDingbats", yesno, &NoDing, NULL },
 { "HelpSectionBreaks", yesno, &HelpSectBreaks, NULL },
 { "UseGreen", yesno, &UseGreen, NULL },

 { "IdxColon", yesno, &IdxColon, NULL },
 { "KeywordLimit", snum, &KeywordLimit, NULL },
 { "DisambiguateIndex", cap, &DisambiguateIndex, NULL },
 { "IndexRanges", yesno, &IndexRanges, NULL },
 { "IXnewlinkPrefix", str, &IXnewlinkPrefix, NULL },

 { "TitleScroll", yesno, &HelpTitleScroll, NULL },
 { "ExtendHelpNoScroll", yesno, &ExtendHelpNoScroll, NULL },
 { "HelpLineBreak", yesno, &HelpLineBreak, NULL },
 { "TitleSpace", yesno, &HelpTitleSpace, NULL },
 { "TitleIndent", yesno, &HelpTitleIndent, NULL },
 { "HelpTabLimit", usnum, &HelpTabLimit, NULL },

 { "EndFtnWithSpace", yesno, &EndFtnWithSpace, NULL },
 { "FootnoteSpace", cap, &FtnSpace, NULL },

 { "UseHyperColor", yesno, &UseHyperColor, NULL },
 { "NoXrefPopups", yesno, &NoXRPopup, NULL },
 { "AllowLiningOverrides", yesno, &ULineOver, NULL },

 { NULL, spec, NULL, NULL }
};

void
RTFwr::HyperHelpSet(char *)
{
	if (HyperHelp)
		EndFtnWithSpace = false;
}


setinfo RTFwr::ISecWordOptions[] = {
 { "Word2000", yesno, &Word2000, Word8Set },
 { "Word2K", yesno, &Word2000, Word8Set },
 { "Word2002", yesno, &Word2002, Word8Set },
 { "WordXP", yesno, &Word2002, Word8Set },
 { "Word2003", yesno, &Word2003, Word8Set },
 { "Word2007", yesno, &Word2003, Word8Set },
 { "Word2009", yesno, &Word2003, Word8Set },
 { "Word2010", yesno, &Word2003, Word8Set },
 { "PicScale", snum, &PicScale, NULL },

 { "FieldHyper", yesno, &FieldHyper, NULL },  // deprecated
 { "XrefHyper", yesno, &FieldHyper, NULL },   // replacement
 { "WrapXrefs", yesno, &WrapXrefs, NULL },
 { "LockXrefs", yesno, &LockXrefs, NULL },
 { "LockHyper", yesno, &LockHyper, NULL },
 { "ExtXrefPages", yesno, &ExtXrefPages, NULL },

 { "SeqAnums", yesno, &SeqAnums, NULL },
 { "AnumMergeformat", yesno, &AnumMergeformat, NULL },

 { "RevProt", yesno, &RevProt, RevProtSet },
 { "RevTrack", yesno, &RevTrack, NULL },
 { "WriteMissingForms", yesno, &WriteMissingForms, NULL },
 { "WriteAllVarForms", yesno, &WriteAllVarForms, NULL },

 { "DefTabWidth", snum, &DefTabWidth, NULL },
 { "Template", str, &Template, NULL },
 { "TemplateFileName", str, &Template, NULL },
 { "TemplateAutoUpdate", yesno, &TemplateAutoUpdate, NULL },

 { "HeadFoot", cap, &HFType, NULL },
 { "HFVertAdjust", snum, &HFVAdjust, NULL },
 { "HFFramed", yesno, &FrameHF, NULL },
 { "WrapAroundHFFrames", yesno, &WrapHTFs, NULL },
 { "HFGap", snum, &HFGap, NULL },

 { "UseTextFrames", yesno, &UseTextFrames, NULL },
 { "WrapAroundTextFrames", yesno, &WrapTFs, NULL },

 { "ForceSideHeadPos", cap, &FSideHPos, ForceSideHeadPosSet },
 { "SHSpannerAnchors", yesno, &SHSpannerAnchors, NULL },
 { "SHVertAdjust", snum, &SHVAdjust, NULL },
 { "SHWidth", snum, &SHWidth, NULL },
 { "SHGap", snum, &SHGap, NULL },

 { "SingleFlow", yesno, &SingleSect, NULL },
 { "PageColumns", usnum, &DefCols, NULL },
 { "PageColGap", usnum, &ColGap, NULL },

 // not implemented
 { "TextRescale", usnum, &TextRescale, NULL },
 { "TableRescale", usnum, &TableRescale, NULL },
 { "GraphicRescale", usnum, &GraphicRescale, NULL },

 { NULL, spec, NULL, NULL }
};

void
RTFwr::Word8Set(char *)
{
	if (Word2000 || Word2002 || Word2003)
		Word8 = true;
}

void
RTFwr::RevProtSet(char *)
{
	if (RevProt)
		RevTrack = true;
}

void
RTFwr::ForceSideHeadPosSet(char *)
{
	ForceSideHeadPos = true;
	switch (FSideHPos) {
		case 'L':
			FSideHFlip = false;
			FSideHPos = 1;
			break;
		case 'R':
			FSideHFlip = false;
			FSideHPos = 2;
			break;
		case 'I':
			FSideHFlip = true;
			FSideHPos = 1;
			break;
		case 'O':
			FSideHFlip = true;
			FSideHPos = 2;
			break;

		case 'N':
		default:
			ForceSideHeadPos = false;
			FSideHPos = 0;
			break;
	}
}


setinfo RTFwr::ISecOptions[] = {
 { "Index", cap, &IdxType, NULL },
 { "NoSeeAlso", yesno, &NoSeeAlso, NULL },

 { "Quotes", cap, &QuoType, NULL },
 { "Bullets", cap, &BulType, NULL },
 { "SmallCaps", cap, &SmallCaps, SmallCapsSet },

 { "WriteAnums", yesno, &WriteAnums, NULL },

 { "Xrefs", cap, &XrfType, NULL },
 { "XrefFileSuffix", str, &XrefFileSuffix, XrefFileSuffixSet },
 { "XrefFileDefault", str, &GlobalXrefFileName, NULL },
 { "XrefLenLimit", snum, &XrefLenLimit, NULL },

 { "ObjectIDs", cap, &HUType, NULL },
 { "UseHyperlinks", yesno, &UseHL, NULL },
 { "UseFileIDs", yesno, &UseXrefFilePrefix, NULL },

 { "CharStyles", yesno, &CharStyles, NULL },
 { "CharStylesUsedInText", yesno, &CharStylesUsed, NULL },

 { "RemoveUnusedFonts", yesno, &RemFonts, NULL },
 { "RemoveUnusedStyles", yesno, &RemStyles, NULL },
 { "FrameEndPara", yesno, &FrameEndPara, NULL },

 { "Footnotes", cap, &FootType, NULL },
 { "FootnoteSeparator", str, &FtnSeparator, NULL },

 { "PageBreaks", cap, &PageType, NULL },
 { "KeepSectBreaks", yesno, &KeepSectBreaks, NULL },
 { "KeepAnchorParagraphs", yesno, &KeepAnchors, NULL },
 { "KeepIXMarkerIDs", yesno, &KeepIXMarkerIDs, NULL },
 { "BookmarkIXRanges", yesno, &BookmarkIXRanges, NULL },

 { "FrameStyle", str, &FrameStyle, NULL },

 { "Sideheads", cap, &SideHType, NULL },
 { "RunInHeads", cap, &RuninType, NULL },

 { "ParaSpace", cap, &PSpcType, NULL },
 { "ExactLineSpace", yesno, &ExactLSUsed, NULL },
 { "UseParaAnchors", yesno, &UseParaAnchors, NULL },

 { "OccludedTabs", cap, &OccTabType, NULL },
 { "RMarginTabs", cap, &RMarginTabs, RMarginTabsSet },
 { "TrailingTabs", yesno, &TrailingTabs, NULL },

 { "UnderlineTabs", yesno, &UnderlineTabs, NULL },
 { "HideWhiteText", yesno, &HideWhite, NULL },
 { "TextColor", yesno, &TextColor, NULL },
 { "BackColor", yesno, &BackColor, NULL },
 { "EmptyFrames", cap, &EmpFrType, NULL },
 { "RunInFrames", cap, &RunInFrames, NULL },

 { "EmbedEqsInWMFs", yesno, &EmbedEqs, NULL },
 { "EqVertAdjust", snum, &EqVert, NULL },
 { "EqHorAdjust", snum, &EqHor, NULL },

 // undocumented
 { "Custom", str, &CustCode, NULL },
 { "ExternalXrefs", yesno, &ExtXRefs, NULL },
 { "NoBookmarks", yesno, &NoFreakingBookmarks, NULL },
 { "LogBookmarkCount", yesno, &LogBookmarkCount, NULL },
 { "PageBreakMethod", cap, &PBMethod, NULL },
 { "NegLeading", cap, &NegLead, NULL },
 { "AllowZeroFSize", yesno, &AllowZeroFSize, NULL },

 { NULL, spec, NULL, NULL }
};

void
RTFwr::SmallCapsSet(char *)
{
	if (SmallCaps == 'Y') // error in m2rbook
		SmallCaps = 'S';
}

void
RTFwr::RMarginTabsSet(char *)
{
	switch (RMarginTabs) {
		case 'L':
			LTabRMarg = true;
			RTabRMarg = false;
			break;
		case 'R':
			LTabRMarg = false;
			RTabRMarg = true;
			break;
		case 'N':
			LTabRMarg = false;
			RTabRMarg = false;
			break;

		default:
		case 'B':
			LTabRMarg = true;
			RTabRMarg = true;
			break;
	}
}


// RefFramesSet

setinfo RTFwr::ISecRefFrames[] = {
 { "RefFrameDefFormat", str, &RefFrameDefFormat, NULL },
 { "RefFrameFormat", str, &RefFrameFormat, RefFrameFormatSet },
 { "RefFrames", cap, &RefFrames, RefFramesSet },
 { NULL, spec, NULL, NULL }
};

void
RTFwr::RefFramesSet(char *)
{
	switch (RefFrames) {
		case 'N':
			RefFrameFormat = NULL;
			break;
		case 'T':
			if (RefFrameDefFormat)
				RefFrameFormat = RefFrameDefFormat;
			break;
		case 'G':
		default:
			RefFrameFormat = "";
			break;
	}
}

void
RTFwr::RefFrameFormatSet(char *)
{
	if (!RefFrameFormat)
		RefFrameFormat = "";
}


// DefaultsSet

setinfo RTFwr::ISecDefaults[] = {
 { "FontName", str, &DefFontName, NULL },
 { "FontSize", lnum, &DefFontSize, FontSizeSet },
 { "FontWidth", snum, &DefFontWidth, NULL },
 { "LineSpacing", snum, &DefLineSpace, NULL },
 { "Language", str, &DefLang, DefLangSet },
 { "CodePage", ulnum, &DefaultCodePage, NULL },
 { "SpaceAfterUnicode", yesno, &SpaceAfterUnicode, NULL },
 { "TblColumnWidth", lnum, &DefColWidth, NULL },
 { NULL, spec, NULL, NULL }
};

void
RTFwr::FontSizeSet(char *)
{
	DefFontSize = (DefFontSize + 5) / 10;  // in half-points
}

void
RTFwr::DefLangSet(char *)
{
	DefaultLanguage = (short) strtoul(DefLang, NULL, 0);
}


// FDKSet

setinfo RTFwr::ISecFDK[] = {
 { "EquationExportDPI", snum, &EqExDPI, NULL },
 { "EquationFrameExpand", snum, &EqExpand, NULL },
 { "UseFrameIX", yesno, &UseFrameIX, NULL },
 { "UseLocalFileID", yesno, &UseLocalFileID, NULL },
 { "IDFileName", path, &IDFileName, IDFileNameSet },
 { "PrjFileName", path, &PrjFileName, NULL },
 { "FileSuffix", str, &XrefFileSuffix, XrefFileSuffixSet },
 { NULL, spec, NULL, NULL }
};

void
RTFwr::IDFileNameSet(char *)
{
	if (!UseLocalFileID
	 && IDFileName) {
		DCirfile id(IDFileName);
		if (id.FErr() != fok) {
			DeleteName(IDFileName);
			IDFileName = NULL;
		}
		else if (!(id.Section("FileIDs"))) {
			DeleteName(IDFileName);
			IDFileName = NULL;
			id.CloseFile();
		}
		else
			id.CloseFile();
	}
}


void
RTFwr::XrefFileSuffixSet(char *)
{
	if (*XrefFileSuffix == '.')
		XrefFileSuffix++;
}



// TableSet

setinfo RTFwr::ISecTable[] = {
 { "TableTitles", ucnum, &TbTitleType, NULL },
 { "TitleInRow", yesno, &TitleInRow, NULL },

 { "TableContinued", yesno, &TbContUse, NULL },
 { "TableContVar", str, &TbContVarName, NULL },
 { "TableSheet", yesno, &TbSheetUse, NULL },
 { "TableSheetVar", str, &TbSheetVarName, NULL },

 { "TableRules", cap, &TbRuleType, NULL },
 { "TableFill", cap, &TableFill, TableFillSet },
 { "TableRuleAdjust", yesno, &TbRuleAdjust, NULL },
 { "TableGraphics", cap, &TbGraphics, NULL },

 { "TblColWidPct", snum, &TblColWidPct, NULL },
 { "TblColWidAdd", snum, &TblColWidAdd, NULL },
 { "TblFullWidth", ulnum, &TblFullWidth, TblFullWidthSet },

 { "StripTable", yesno, &StripTables, NULL },
 { "StripTables", yesno, &StripTables, NULL },
 { "StrippedCellPar", yesno, &StrippedCellPar, NULL },

 { "TableWidthsFixed", yesno, &TbFixed, NULL },
 { "ForceTableLineBreaks", yesno, &ForceTbLineBreaks, NULL },
 { "FrameTableTitles", yesno, &FrameTbTitle, NULL },
 { "ShiftWideTablesLeft", yesno, &ShiftWideTablesLeft, NULL },
 { "CellStradAdjust", yesno, &CellStradAdjust, NULL },
 { "MergeStradCells", yesno, &MergeStradCells, NULL },

 { NULL, spec, NULL, NULL }
};

void
RTFwr::TableFillSet(char *)
{
	switch (TableFill) {
		case 'N':
			TblFillColor = false;
			TblFillShade = false;
			break;
		case 'C':
			TblFillColor = true;
			TblFillShade = false;
			break;
		case 'S':
			TblFillColor = false;
			TblFillShade = true;
			break;
		case 'A':
		default:
			TblFillColor = true;
			TblFillShade = true;
			break;
	}
}


void
RTFwr::TblFullWidthSet(char *)
{
	TblFullWidthWasSet = true;
}


// GraphicsSet

setinfo RTFwr::ISecGraphics[] = {
 { "RemoveGraphics", yesno, &RemoveGraphics, NULL },
 { "RemoveNontextFrames", yesno, &RemoveNontextFrames, NULL },
 { "ClipText", yesno, &ClipText, NULL },
 { "ClipType", cap, &ClipType, NULL },
 { "ClipLimit", snum, &ClipLimit, NULL },
 { "GrVertAdjust", snum, &GrVert, NULL },
 { "BackMode", snum, &GrBkMode, NULL },
 { "StretchMode", snum, &GrStretchMode, NULL },
 { "DefBrushType", cap, &DefBrushType, NULL },
 { "EmbedBMPsInWMFs", yesno, &EmbedBMPs, NULL },
 { "EmbedWMFsInWMFs", yesno, &EmbedWMFs, NULL },
 { "BitmapDPI", snum, &BitmapDPI, NULL },
 { "CompressRasters", yesno, &CompRasters, NULL },
 { "RasterBorders", cap, &RasBorders, NULL },
 { "FrBorders", cap, &FrBorders, NULL },
 { "BitmapFlip", yesno, &BitmapFlip, NULL },
 { "FrameBorders", yesno, &FrameBorders, NULL },
 { "DefFont", str, &GrDefFont, NULL },
 { "DefFSize", snum, &GrDefFSize, NULL },
 { "TextScale", snum, &GrTxScale, TextScaleSet },
 { "TextWidth", snum, &GrTxWidth, NULL },
 { "TextVertAdjust", snum, &GrTxVAdj, NULL },
 { "UseTopSpaceAbove", yesno, &UseTopSpaceAbove, NULL },
 { "NoBlankFirstGTLine", yesno, &NoBlankFirstGTLine, NULL },
 { "SuppressGTUnderlines", yesno, &NoGTULine, NULL },
 { "FrameExactHeight", yesno, &FrExHigh, NULL },
 { "WrapAllFrames", yesno, &WrapAllFrames, NULL },
 { "MetaNameChars", lnum, &MetaNameWid, NULL },
 { "MetaNumDigits", lnum, &MetaNumWid, NULL },
 { "BulletFile", str, &BulletStr, NULL },
 { "NameWMFsAsBMPs", yesno, &NameWMFsAsBMPs, NULL },
 { "NameGraphics", yesno, &NameGraphics, NULL },

 // not documented
 { "DashLines", cap, &DashLType, NULL },
 { "RectIsPoly", yesno, &RectIsPoly, NULL },
 { "GrTableSpace", yesno, &GrTbSpace, NULL },
 { "Raster24BitMode", snum, &Ras24Mode, NULL },
 { "ImportedWMFs", cap, &ImpWMFType, NULL },

 { NULL, spec, NULL, NULL }
};

void
RTFwr::TextScaleSet(char *)
{
	if (GrTxScale == 0)
		GrTxScale = 100;
}


setinfo RTFwr::ISecGraphOpts[] = {
 { "UseGraphicPreviews", yesno, &UseGrPreviews, NULL },
 { "GraphText", cap, &GrTxType, NULL },
 { "FilePaths", cap, &GrPaths, NULL },
 { "FileNames", cap, &GrNames, NULL },
 { "Metafiles", cap, &MetaType, NULL },
 { "Transparent", yesno, &TransparentBitmaps, NULL },

 { NULL, spec, NULL, NULL }
};


// Index Set

setinfo RTFwr::ISecIndex[] = {
 { "IndexStyle", str, &IndexStyle, NULL },
 { "IndexLeader", str, &IndexLeader, NULL },
 { "IndexColumns", snum, &IndexColumns, NULL },
 { "UseIndexLetters", yesno, &UseIndexLetters, NULL },
 { "UseIndexTopLetters", yesno, &UseIndexTopLetters, NULL },
 { "IndexLetterPrefix", str, &IndexLetPrefix, NULL },
 { "IndexLetterSymbol", str, &IndexLetterSymbol, NULL },
 { "IndexLetterNumber", str, &IndexLetterNumber, NULL },
 { "IndexTopLettersClass", str, &IndexTopLettersForm, NULL },
 { "IndexTopLettersFormat", str, &IndexTopLettersForm, NULL },
 { "IndexLetterClass", str, &IndexLettersForm, NULL },
 { "IndexLettersFormat", str, &IndexLettersForm, NULL },

 { NULL, spec, NULL, NULL }
};


// Help-specific Sets

setinfo RTFwr::ISecHelpBrowse[] = {
 { "TitleBrowse", yesno, &TitleBrowse, NULL },
 { "Start", lnum, &BrowseCurr, NULL },
 { "Step", lnum, &BrowseStep, NULL },
 { "Digits", lnum, &BrowseDigits, BrowseDigitsSet },
 { "Prefix", str, &BrowsePrefix, NULL },
 { "AutoBrowse", yesno, &AutoBrowse, NULL },
 { NULL, spec, NULL, NULL }
};

void
RTFwr::BrowseDigitsSet(char *)
{
	if (BrowseDigits > BrowseDigitsMax)
		BrowseDigits = BrowseDigitsMax;
}


setinfo RTFwr::ISecHelpContents[] = {
 { "CntType", cap, &CntType, NULL },
 { "CntBase", str, &CntBase, NULL },
 { "CntTitle", str, &CntTitle, NULL },
 { "CntTopic", str, &CntTopic, NULL },
 { "CntTopHead", str, &CntTopHead, NULL },
 { "CntName", str, &CntName, NULL },
 { "CntStartFile", str, &CntStartFile, NULL },
 { "CntBStyleText", str, &CntBTextStr, NULL },
 { "AddCntFileName", yesno, &AddCntFileName, NULL },
 { "AddCntWindowName", yesno, &AddCntWindowName, NULL },
 { "CntMainWindow", str, &CntMainWindow, NULL },
 { NULL, spec, NULL, NULL }
};


// StylesSets

setinfo RTFwr::ISecCommonStyles[] = {
 { "Delete", bval, &Curr.del, NULL, 1 },
 { "Replace", spec, &Curr.replace, ReplaceSet },

 { "CodeBefore", spec, &Curr.maccb, MacBeforeSet },
 { "CodeStart", spec, &Curr.maccs, MacStartSet },
 { "CodeEnd", spec, &Curr.macce, MacEndSet },
 { "CodeAfter", spec, &Curr.macca, MacAfterSet },
 { "CodeReplace", spec, &Curr.maccr, MacReplaceSet },
 { "CodeBeforeAnum", spec, &Curr.macacb, MacAnumBeforeSet },
 { "CodeAfterAnum", spec, &Curr.macaca, MacAnumAfterSet },
 { "CodeStore", spec, &Curr.cstore, CodeStoreSet },
 { "TextStore", spec, &Curr.tstore, TextStoreSet },

 { "Config", bval, &Curr.config, NULL, 1 },
 { "RTFConfig", bval, &Curr.config, NULL, 1 },
 { NULL, spec, NULL, NULL }
};

void
RTFwr::ReplaceSet(char *)
{
	Curr.del = true;
	if (DCini::IsIni)
		DCini::ProcIniSect(HelpForm ? "HelpReplacements" : "WordReplacements");
}

void
RTFwr::ReplacementsSet(char *karg)
{
	Curr.del = true;
	Curr.replace = karg;
}

void
RTFwr::MacBeforeSet(char *)
{
	if (DCini::IsIni)
		DCini::ProcIniSect("StyleCodeBefore");
}

void
RTFwr::MacStartSet(char *)
{
	if (DCini::IsIni)
		DCini::ProcIniSect("StyleCodeStart");
}

void
RTFwr::MacEndSet(char *)
{
	if (DCini::IsIni)
		DCini::ProcIniSect("StyleCodeEnd");
}

void
RTFwr::MacAfterSet(char *)
{
	if (DCini::IsIni)
		DCini::ProcIniSect("StyleCodeAfter");
}

void
RTFwr::MacReplaceSet(char *)
{
	Curr.del = true;
	if (DCini::IsIni)
		DCini::ProcIniSect("StyleCodeReplace");
}

void
RTFwr::StyleCodeReplaceSet(char *)
{
	Curr.del = true;
}

void
RTFwr::MacAnumBeforeSet(char *)
{
	if (DCini::IsIni)
		DCini::ProcIniSect("AnumCodeBefore");
}

void
RTFwr::MacAnumAfterSet(char *)
{
	if (DCini::IsIni)
		DCini::ProcIniSect("AnumCodeAfter");
}

void
RTFwr::CodeStoreSet(char *)
{
	Curr.cstore = NewName(Curr.stag);
	if (DCini::IsIni)
		DCini::ProcIniSect("StyleCodeStore");
}

void
RTFwr::TextStoreSet(char *)
{
	Curr.tstore = NewName(Curr.stag);
	if (DCini::IsIni)
		DCini::ProcIniSect("StyleTextStore");
}


setinfo RTFwr::ISecWordStyles[] = {
 { "Contents", val, &Curr.cntform, NULL, 1 },
 { "KeepID", val, &Curr.cntform, NULL, 1 },
 { "Hide", val, &Curr.hide, NULL, 1 },
 { "ParaLink", spec, &Curr.paralink, ParaLinkSet },
 { NULL, spec, NULL, NULL }
};


setinfo RTFwr::ISecHelpStyles[] = {
 { "Contents", spec, &Curr.cntform, ContentsSet },
 { "JumpHot", val, &Curr.help, JumpHotSet, 4 },
 { "PopHot", val, &Curr.help, PopHotSet, 5 },
 { "Browse", spec, &Curr.hbrowse, BrowseSet },
 { "SpKey", spec, &Curr.hkey, SpKeySet },
 { "File", spec, &Curr.hjfile, FileSet },
 { "Window", spec, &Curr.hwindow, WindowSet },
 { "MacroHot", spec, &Curr.hmacro, MacroHotSet},
 { "Macro", spec, &Curr.hmacro, MacroSet },
 { "Build", spec, &Curr.hbuild, BuildSet },
 { "Suffix", spec, &Curr.hsuf, SuffixSet },
 { "TitleSuf", spec, &Curr.htsuf, TitleSufSet },
 { "InfoType", spec, &Curr.infot, InfoTypeSet },
 { "JumpTarget", val, &Curr.help, ContextFnSet, 6 },
 { "PopContent", val, &Curr.help, PopContentSet, 9 },
 { "Topic", val, &Curr.help, TopicSet, 8 },
 { "Refer", spec, &Curr.help, ContextFnSet },
 { "Local", bit, &Curr.clone, NULL, 16 },  // bit value
 { "Green", val, &Curr.hvis, NULL, 0 },
 { "ULine", spec, &Curr.hvis, ULineSet },
 { "Slide", val, &Curr.help, SlideSet, 16 },
 { "Resume", spec, &Curr.help, ResumeSet },
 { "Key", bit, &Curr.hkey, KeysSet, 1 },  // bit value
 { "AKey", bit, &Curr.hkey, KeysSet, 2 },
 { "PopOver", val, &Curr.help, PopOverSet, 2 },
 { "Scroll", val, &Curr.hscroll, NULL, 1 },
 { "NoScroll", val, &Curr.hscroll, NULL, 2 },
 { "XScroll", val, &Curr.xscroll, NULL, 2 },
 { "NoXScroll", val, &Curr.xscroll, NULL, 1 },
 { "NoTitle", val, &Curr.htvis, NULL, 0 },
 { "ParaLink", spec, &Curr.paralink, ParaLinkSet },
 { "MakeRef", bval, &Curr.mref, NULL, 1 },
 { "PrevRef", bval, &Curr.pref, NULL, 1 },
 { "NextRef", bval, &Curr.nref, NULL, 1 },
 { "JumpOver", val, &Curr.help, JumpOverSet, 4 },
 { NULL, spec, NULL, NULL }
};

void
RTFwr::ContentsSet(char *)
{
	if (DCini::IsIni)
		DCini::ProcIniSect(HelpForm ? "HelpCntStyles" : "WordCntStyles");
}

void
RTFwr::CntStylesSet(char *karg)
{
	if (CntType != 'N')
		ProcHelpCntForm(karg);
}

void
RTFwr::JumpHotSet(char *)
{
	Curr.line = 3;
	Curr.lmode = 1;
	Curr.hvis = '%'; // start with unmarked
	Curr.clone |= 1;
	if (DCini::IsIni)
		DCini::ProcIniSect("HelpRefStyles");
}

void
RTFwr::PopHotSet(char *)
{
	Curr.line = 2;
	Curr.lmode = 1;
	Curr.hvis = '%'; // start with unmarked
	Curr.clone |= 1;
	if (DCini::IsIni)
		DCini::ProcIniSect("HelpRefStyles");
}

void
RTFwr::HelpRefStylesSet(char *)
{
	if ((Curr.help != 4)
	 && (Curr.help != 5)) {
		Curr.help = 4;
		Curr.line = 3;
		Curr.lmode = 1;
		Curr.hvis = '%'; // start with unmarked
		Curr.clone |= 1;
	}
}

void
RTFwr::BrowseSet(char *)
{
	Curr.hbrowse = BrowsePrefix;
	if (DCini::IsIni)
		DCini::ProcIniSect("HelpBrowsePrefixStyles");
}

void
RTFwr::SpKeySet(char *)
{
	if (Curr.help == 0)
		Curr.help = 14;
	Curr.clone |= 8;
	if (DCini::IsIni)
		DCini::ProcIniSect("HelpKeywordStyles");
}

void
RTFwr::HelpKeywordStylesSet(char *karg)
{
	if (islower(*karg))
		*karg = toupper(*karg);
	if (isupper(*karg))
		Curr.hkey |= (*karg - 0x40) << 2;
}

void
RTFwr::FileSet(char *)
{
	if (DCini::IsIni)
		DCini::ProcIniSect("HelpJumpFileStyles");
}

void
RTFwr::WindowSet(char *)
{
	if (DCini::IsIni)
		DCini::ProcIniSect("HelpWindowStyles");
}

void
RTFwr::MacroHotSet(char *)
{
	Curr.help = 4;   // treat as a jump
	Curr.line = 3;
	Curr.lmode = 1;
	Curr.hvis = '%'; // start with unmarked
	if (DCini::IsIni)
		DCini::ProcIniSect("HelpMacroStyles");
}

void
RTFwr::MacroSet(char *)
{
	if (DCini::IsIni)
		DCini::ProcIniSect("HelpMacroStyles");
}

void
RTFwr::BuildSet(char *)
{
	if (DCini::IsIni)
		DCini::ProcIniSect("HelpTopicBuildStyles");
}

void
RTFwr::SuffixSet(char *)
{
	if (DCini::IsIni)
		DCini::ProcIniSect("HelpSuffixStyles");
}

void
RTFwr::TitleSufSet(char *)
{
	if (DCini::IsIni)
		DCini::ProcIniSect("HelpTitleSufStyles");
}

void
RTFwr::InfoTypeSet(char *)
{
	if (DCini::IsIni
	 && (HelpType == 'M'))
		DCini::ProcIniSect("HelpInfoTypeStyles");
}

void
RTFwr::ContextFnSet(char *)
{
	Curr.clone |= 2;  // context fn
}

void
RTFwr::PopContentSet(char *)
{
	Curr.clone |= 2;  // context fn
	Curr.clone |= 32; // popup start
	Curr.slide = 1;
}

void
RTFwr::TopicSet(char *)
{
	Curr.clone |= 4;  // title fn
	Curr.htvis = 1;
}

void
RTFwr::ULineSet(char *)
{
	if (Curr.hvis == '%')
		Curr.hvis = '*';
	else 
		Curr.hvis = 0;
}

void
RTFwr::SlideSet(char *)
{
	Curr.slide = 1;
}

void
RTFwr::ResumeSet(char *)
{
	if (Curr.help == 0)
		Curr.help = 17;
	Curr.slide = 2;
}

void
RTFwr::KeysSet(char *)
{
	if (Curr.help == 0)
		Curr.help = 14;
	Curr.clone |= 8;
}

void
RTFwr::PopOverSet(char *)
{
	Curr.line = 2;
	Curr.lmode = 1;
	Curr.hvis = '%'; // start with unmarked
}

void
RTFwr::ParaLinkSet(char *)
{
	// may be preceded by popover
	Curr.paralink = ((Curr.help == 2) && (Curr.line == 2)) ? 2 : 3;
}

void
RTFwr::JumpOverSet(char *)
{
	Curr.line = 3;
	Curr.lmode = 1;
	Curr.hvis = '%'; // start with unmarked
}


// Xref formats

setinfo RTFwr::ISecXrefStyles[] = {
 { "Text", bval, &XrefTextOnly, NULL, true },
 { "Delete", bval, &XrefDelete, NULL, true },
 { NULL, spec, NULL, NULL }
};

// Inserts

setinfo RTFwr::ISecWInserts[] = {
 { "Top", str, &TopInsert, NULL, true },
 { "Bottom", str, &BottomInsert, NULL, true },
 { "Header", str, &HeaderInsert, NULL, true },
 { "Footer", str, &FooterInsert, NULL, true },
 { "FirstHeader", str, &FirstHeaderInsert, NULL, true },
 { "FirstFooter", str, &FirstFooterInsert, NULL, true },
 { "LeftHeader", str, &LeftHeaderInsert, NULL, true },
 { "LeftFooter", str, &LeftFooterInsert, NULL, true },
 { "RightHeader", str, &RightHeaderInsert, NULL, true },
 { "RightFooter", str, &RightFooterInsert, NULL, true },
 { NULL, spec, NULL, NULL }
};

setinfo RTFwr::ISecHInserts[] = {
 { "TopicStart", str, &TopicStartInsert, NULL, true },
 { "TopicEnd", str, &TopicEndInsert, NULL, true },
 { "SlideStart", str, &SlideStartInsert, NULL, true },
 { "SlideEnd", str, &SlideEndInsert, NULL, true },
 { NULL, spec, NULL, NULL }
};


// LoggingSet

setinfo RTFwr::ILogOptions[] = {
 { "UseLog", yesno, &UseLog, NULL },
 { "ShowLog", yesno, &ShowLog, NULL },
 { "LogIniChains", yesno, &LogIniChains, NULL },
 { "LogFileName", path, &LogFileName, NULL },
 { "EditorFileName", path, &EditorFileName, NULL },
 { "LogErrors", ucnum, &LogLevels[logerr], NULL },
 { "LogWarnings", ucnum, &LogLevels[logwarn], NULL },
 { "LogQuerys", ucnum, &LogLevels[logquery], NULL },
 { "LogInfo", ucnum, &LogLevels[loginfo], NULL },
 { "LogDebug", ucnum, &LogLevels[logdebug], NULL },

 { NULL, spec, NULL, NULL }
};



/* template for new sections

 { "", typ, "Set", ISec, false, str, &var },

// 

setinfo RTFwr::ISec[] = {
 // settings
 { "", yesno, &, NULL },
 { "", cap, &, NULL },
 { "", str, &, NULL },
 { "", chr, &, NULL },
 { "", ucnum, &, NULL },
 { "", snum, &, NULL },
 { "", usnum, &, NULL },
 { "", lnum, &, NULL },

 // styles
 { "", val, &Curr., NULL, 3 },
 { "", usnum, &Curr., NULL, 7 }, // max value
 { "", bval, &Curr., NULL, 1 },
 { "", ucnum, &Curr., NULL, ListInitMax },
 { "", bit, &Curr., NULL, 4 },  // bit value
 { "", cval, &Curr., NULL, 0, "sth" },  // str value
 { "", spec, &Curr., CodeStoreSet }, // func only

 { NULL, spec, NULL, NULL }
};

void
RTFwr::Set(char *)
{
}

*/


// end of dwrtfin.cpp

