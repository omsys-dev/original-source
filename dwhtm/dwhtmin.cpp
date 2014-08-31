// dwhtmin.cpp is the code for dcl HTM ini processing, JHG, 3/20/05
// Copyright (c) 2005 by Omni Systems, Inc.  All rights reserved.

#include "dcappl.h"
#include "dcctlg.h"
#include "dcfili.h"
#include "dctxfx.h"
#include "dcfmtr.h"
#include "dcini.h"
#include "dcmacr.h"
#include "dcmxl.h"
#include "dwhtm.h"
#include "dwhtmfl.h"


// list of all mif2htm.ini sections (except for macros)
// add: non-fixed-set sections like HtmlStyles

secinfo HTMwr::HTMIniSects[] = {

 // global settings always checked:
 { "Options", set, "OptionsSet", ISecOptions, true },
 { "Automation", set, "OptionsSet", ISecAuto, true },
 { "MapOptions", set, NULL, ISecMapOptions, true },
 { "CSS", set, "CssSet", ISecCss, true },
 { "HTMLOptions", set, "OptionsSet", ISecCss, true },
 { "HTMLOptions", set, "OptionsSet", ISecOptions, true },
 { "uDocOptions", set, "OptionsSet", ISecUdocOptions, true },
 { "FDK", set, "FDKSet", ISecFDK, true },
 { "Setup", set, "FDKSet", ISecFDK, true },
// { "Templates", set, "TemplateSet", ISecTemplates, true },
 { "Table", set, "TableSet", ISecTable, true },
 { "Tables", set, "TableSet", ISecTable, true },
 { "Trails", set, "TrailsSet", ISecTrails, true },
 { "LocalTOC", set, "LocalTOCSet", ISecLocalTOC, true },
 { "DynamicFiltering", set, "FilterConditionsSet", ISecFilterConditions, true },
 { "DropDowns", set, "DropDownsSet", ISecDropDowns, true },
 { "Base", set, "BaseFontSet", ISecBase, true },
 { "Inserts", set, "MacroSet", ISecInserts, true },
 { "NavigationMacros", set, "MacroSet", ISecNavMacs, true },
 { "Typographics", set, "TypoSet", ISecTypo, true },
 { "ConditionOptions", set, "ElementSet", ISecCondOptions, true },
 { "Logging", set, "LoggingSet", ILogOptions, true },
 { "Index", set, NULL, ISecIndex, true },
 { "IndexText", set, NULL, ISecIndexText, true },
 { "Attributes", set, NULL, ISecAttr, true },  // body and table defaults

 // global settings checked in other modules:
 { "Macros", set, NULL, NULL, false },
 { "GraphExport", set, NULL, NULL, false },

 // output-format dependent settings:
 { "DocBookOptions", set, "DocBookSet", ISecDocBook, false },
 { "DITAOptions", set, NULL, ISecDITA, false },
 { "DITABookmapOptions", set, NULL, ISecDITAbookmap, false },

 // Help-format settings checked conditionally:
 { "OmniHelpOptions", set, "OHSet", ISecOH, false },
 { "HTMLOptions", set, "JavaSet", ISecJava, false },
 { "HTMLOptions", set, "JHSet", ISecJH, false },
 { "HTMLOptions", set, "OHJSet", ISecOHJ, false },
 { "JavaHelpOptions", set, "JavaSet", ISecJava, false },
 { "JavaHelpOptions", set, "JHSet", ISecJH, false },
 { "JavaHelpOptions", set, "OHJSet", ISecOHJ, false },
 { "JavaHelpOptions", set, "JHXSet", ISecJava, false },
 { "JavaHelpOptions", set, "JHXSet", ISecJH, false },
 { "OracleHelpOptions", set, "JavaSet", ISecJava, false },
 { "OracleHelpOptions", set, "OHJSet", ISecOHJ, false },
 { "OracleHelpOptions", set, "OHJXSet", ISecJava, false },
 { "OracleHelpOptions", set, "OHJXSet", ISecOHJ, false },
 { "HTMLOptions", set, "HHSet", ISecHH, false },
 { "MSHtmlHelpOptions", set, "HHSet", ISecHH, false },
 { "EclipseHelpOptions", set, "EHSet", ISecEH, false },

 // Graphics must be checked after JavaHelpOptions
 { "Graphics", set, "GraphicsSet", ISecGraphics, false },

 // style settings
 { "HtmlStyles", pcstyle, "Style1Set", ISecHTMLStyles, false },
 { "HtmlStyles", pstyle, "Style1Set", ISecHTMLPStyles, false },
 { "HtmlStyles", cstyle, "Style1Set", ISecHTMLCStyles, false },
 { "HtmlParaStyles", pstyle, "P1StyleSet", ISecHTMLStyles, false },
 { "HtmlParaStyles", pstyle, "P1StyleSet", ISecHTMLPStyles, false },
 { "HtmlCharStyles", cstyle, "C1StyleSet", ISecHTMLStyles, false },
 { "HtmlCharStyles", cstyle, "C1StyleSet", ISecHTMLCStyles, false },
 { "HtmlStyleCodeStore", pstyle, "PStyleSet", NULL, false, str, &Curr.cstore },
 { "HtmlStyleTextStore", pstyle, "PStyleSet", NULL, false, str, &Curr.tstore },
 { "HtmlStyleTitlePrefix", pstyle, "PStyleSet", NULL, false, str, &Curr.ttlpx },
 { "HtmlStyleTitleSuffix", pstyle, "PStyleSet", NULL, false, str, &Curr.ttlsx },
 { "HtmlStyleTrailPrefix", pstyle, "PStyleSet", NULL, false, str, &Curr.trailpx },
 { "HtmlStyleTrailSuffix", pstyle, "PStyleSet", NULL, false, str, &Curr.trailsx },
 { "HtmlStyleFilePrefix", pstyle, "PStyleSet", NULL, false, str, &Curr.flnpx },
 { "HtmlStyleFileSuffix", pstyle, "PStyleSet", NULL, false, str, &Curr.flnsx },
 { "HtmlStyleMetaName", pstyle, "PStyleSet", NULL, false, str, &Curr.meta },
 { "StyleCodeStore", pstyle, "PStyleSet", NULL, false, str, &Curr.cstore },
 { "StyleTextStore", pstyle, "PStyleSet", NULL, false, str, &Curr.tstore },
 { "StyleTitlePrefix", pstyle, "PStyleSet", NULL, false, str, &Curr.ttlpx },
 { "StyleTitleSuffix", pstyle, "PStyleSet", NULL, false, str, &Curr.ttlsx },
 { "StyleTrailPrefix", pstyle, "PStyleSet", NULL, false, str, &Curr.trailpx },
 { "StyleTrailSuffix", pstyle, "PStyleSet", NULL, false, str, &Curr.trailsx },
 { "StyleFilePrefix", pstyle, "PStyleSet", NULL, false, str, &Curr.flnpx },
 { "StyleFileSuffix", pstyle, "PStyleSet", NULL, false, str, &Curr.flnsx },
 { "StyleMetaName", pstyle, "PStyleSet", NULL, false, str, &Curr.meta },
#if 0
 { "ParaStyleCodeStore", pstyle, "PStyleSet", NULL, false, str, &Curr.cstore },
 { "ParaStyleTextStore", pstyle, "PStyleSet", NULL, false, str, &Curr.tstore },
 { "ParaStyleTitlePrefix", pstyle, "PStyleSet", NULL, false, str, &Curr.ttlpx },
 { "ParaStyleTitleSuffix", pstyle, "PStyleSet", NULL, false, str, &Curr.ttlsx },
 { "ParaStyleTrailPrefix", pstyle, "PStyleSet", NULL, false, str, &Curr.trailpx },
 { "ParaStyleTrailSuffix", pstyle, "PStyleSet", NULL, false, str, &Curr.trailsx },
 { "ParaStyleFilePrefix", pstyle, "PStyleSet", NULL, false, str, &Curr.flnpx },
 { "ParaStyleFileSuffix", pstyle, "PStyleSet", NULL, false, str, &Curr.flnsx },
 { "ParaStyleMetaName", pstyle, "PStyleSet", NULL, false, str, &Curr.meta },
 { "CharStyleCodeStore", cstyle, "CStyleSet", NULL, false, str, &Curr.cstore },
 { "CharStyleTextStore", cstyle, "CStyleSet", NULL, false, str, &Curr.tstore },
 { "CharStyleTitlePrefix", cstyle, "CStyleSet", NULL, false, str, &Curr.ttlpx },
 { "CharStyleTitleSuffix", cstyle, "CStyleSet", NULL, false, str, &Curr.ttlsx },
 { "CharStyleTrailPrefix", cstyle, "CStyleSet", NULL, false, str, &Curr.trailpx },
 { "CharStyleTrailSuffix", cstyle, "CStyleSet", NULL, false, str, &Curr.trailsx },
 { "CharStyleFilePrefix", cstyle, "CStyleSet", NULL, false, str, &Curr.flnpx },
 { "CharStyleFileSuffix", cstyle, "CStyleSet", NULL, false, str, &Curr.flnsx },
 { "CharStyleMetaName", cstyle, "CStyleSet", NULL, false, str, &Curr.meta },
#endif

 //{ "XMLNestLevels", pstyle, "StyleSet", NULL, false, usnum, &Curr.xnlev },
 { "LocalTOCLevels", pstyle, "PStyleSet", NULL, false, usnum, &Curr.loctoclevel, &LocTocSet },
 { "TrailLevels", pstyle, "PStyleSet", NULL, false, usnum, &Curr.traillevel, &TrailSet },
 { "HelpContentsLevels", pstyle, "PStyleSet", NULL, false, usnum, &Curr.contents },

 { "DITAAliases", pstyle, NULL, NULL, false, str, &Curr.dstag },
 { "DITAAlias", pstyle, NULL, NULL, false, str, &Curr.dstag },
 { "DITALevels", pstyle, "DStyleSet", NULL, false, usnum, &Curr.dtlev },
 { "DITAParents", pstyle, "DStyleSet", NULL, false, sarr, &Curr.dtpar },
 { "DITAFirst", pstyle, "DStyleSet", NULL, false, sarr, &Curr.dtfirst },
 { "DITAOpenBefore", pstyle, "DStyleSet", NULL, false, sarr, &Curr.dtopb },
 { "DITAOpenAfter", pstyle, "DStyleSet", NULL, false, sarr, &Curr.dtopa },
 { "DITACloseBefore", pstyle, "DStyleSet", NULL, false, sarr, &Curr.dtclb },
 { "DITACloseAfter", pstyle, "DStyleSet", NULL, false, sarr, &Curr.dtcla },
 { "DITAMapLevels", pstyle, "DStyleSet", NULL, false, lnum, &Curr.dtmap },
 { "DITAMapUsage", pstyle, "DStyleSet", NULL, false, cap, &Curr.dtmhd },
 { "DITATopics", pstyle, "DStyleSet", NULL, false, str, &Curr.dttop },

 { "DocBookAliases", pstyle, NULL, NULL, false, str, &Curr.dstag },
 { "DocBookAlias", pstyle, NULL, NULL, false, str, &Curr.dstag },
 { "DocBookLevels", pstyle, "DBStyleSet", NULL, false, usnum, &Curr.dtlev },
 { "DocBookParents", pstyle, "DBStyleSet", NULL, false, sarr, &Curr.dtpar },
 { "DocBookFirst", pstyle, "DBStyleSet", NULL, false, sarr, &Curr.dtfirst },
 { "DocBookOpenBefore", pstyle, "DBStyleSet", NULL, false, sarr, &Curr.dtopb },
 { "DocBookOpenAfter", pstyle, "DBStyleSet", NULL, false, sarr, &Curr.dtopa },
 { "DocBookCloseBefore", pstyle, "DBStyleSet", NULL, false, sarr, &Curr.dtclb },
 { "DocBookCloseAfter", pstyle, "DBStyleSet", NULL, false, sarr, &Curr.dtcla },

 { "Targets", pcstyle, "StyleSet", NULL, false, str, &Curr.target },
 { "ParaTargets", pstyle, "PStyleSet", NULL, false, str, &Curr.target },
 { "CharTargets", cstyle, "CStyleSet", NULL, false, str, &Curr.target },
 { "SecWindows", cstyle, "CStyleSet", NULL, false, str, &Curr.secwin },

 // style settings checked conditionally
 { "ParaStyles", pstyle, NULL, NULL, false, str, &Curr.hpname },
 { "CharStyles", cstyle, NULL, NULL, false, str, &Curr.hsname },
 { "ParaTags", pstyle, NULL, NULL, false, str, &Curr.hpname },
 { "CharTags", cstyle, NULL, NULL, false, str, &Curr.hsname },
 { "Classes", pstyle, NULL, NULL, false, str, &Curr.hcname },
 { "Classes", cstyle, NULL, NULL, false, str, &Curr.hscname },
 { "ParaClasses", pstyle, NULL, NULL, false, str, &Curr.hcname },
 { "CharClasses", cstyle, NULL, NULL, false, str, &Curr.hscname },

 { "HtmlStyleParaLinkClass", pcstyle, NULL, NULL, false, str, &Curr.plclass },
 { "StyleParaLinkClass", pcstyle, NULL, NULL, false, str, &Curr.plclass },
 { "ParaStyleParaLinkClass", pstyle, NULL, NULL, false, str, &Curr.plclass },
 { "CharStyleParaLinkClass", cstyle, NULL, NULL, false, str, &Curr.plclass },

 { "DITATags", pstyle, NULL, NULL, false, str, &Curr.dtcname },
 { "DITATags", cstyle, NULL, NULL, false, str, &Curr.dtscname },
 { "DITAParaTags", pstyle, NULL, NULL, false, str, &Curr.dtcname },
 { "DITACharTags", cstyle, NULL, NULL, false, str, &Curr.dtscname },
 { "DITAParaTypographics", pstyle, NULL, NULL, false, sarr, &Curr.dtptyp },
 { "DITACharTypographics", cstyle, NULL, NULL, false, sarr, &Curr.dtctyp },

 { "DocBookTags", pstyle, NULL, NULL, false, str, &Curr.dtcname },
 { "DocBookTags", cstyle, NULL, NULL, false, str, &Curr.dtscname },
 { "DocBookParaTags", pstyle, NULL, NULL, false, str, &Curr.dtcname },
 { "DocBookCharTags", cstyle, NULL, NULL, false, str, &Curr.dtscname },

 // string sections read during HtmlStyles processing
 { "HtmlStyleCodeBefore", pstyle, NULL, NULL, false, str, &Curr.maccb },
 { "HtmlStyleCodeStart", pstyle, NULL, NULL, false, str, &Curr.maccs },
 { "HtmlStyleCodeEnd", pstyle, NULL, NULL, false, str, &Curr.macce },
 { "HtmlStyleCodeAfter", pstyle, NULL, NULL, false, str, &Curr.macca },
 { "HtmlStyleCodeReplace", pstyle, NULL, NULL, false, str, &Curr.maccr },
 { "HtmlAnumCodeBefore", pstyle, NULL, NULL, false, str, &Curr.macacb },
 { "HtmlAnumCodeAfter", pstyle, NULL, NULL, false, str, &Curr.macaca },
 { "StyleCodeBefore", pstyle, NULL, NULL, false, str, &Curr.maccb },
 { "StyleCodeStart", pstyle, NULL, NULL, false, str, &Curr.maccs },
 { "StyleCodeEnd", pstyle, NULL, NULL, false, str, &Curr.macce },
 { "StyleCodeAfter", pstyle, NULL, NULL, false, str, &Curr.macca },
 { "StyleCodeReplace", pstyle, NULL, NULL, false, str, &Curr.maccr },
 { "ParaStyleCodeBefore", pstyle, NULL, NULL, false, str, &Curr.maccb },
 { "ParaStyleCodeStart", pstyle, NULL, NULL, false, str, &Curr.maccs },
 { "ParaStyleCodeEnd", pstyle, NULL, NULL, false, str, &Curr.macce },
 { "ParaStyleCodeAfter", pstyle, NULL, NULL, false, str, &Curr.macca },
 { "ParaStyleCodeReplace", pstyle, NULL, NULL, false, str, &Curr.maccr },
 { "AnumCodeBefore", pstyle, NULL, NULL, false, str, &Curr.macacb },
 { "AnumCodeAfter", pstyle, NULL, NULL, false, str, &Curr.macaca },

 { "HtmlStyleLinkSrc", pcstyle, NULL, NULL, false, str, &Curr.macls },
 { "StyleLinkSrc", pcstyle, NULL, NULL, false, str, &Curr.macls },
 { "ParaStyleLinkSrc", pstyle, NULL, NULL, false, str, &Curr.macls },
 { "CharStyleLinkSrc", cstyle, NULL, NULL, false, str, &Curr.macls },

 { "HtmlStyleCSS", pcstyle, NULL, NULL, false, str, &Curr.maccss },
 { "StyleCSS", pcstyle, NULL, NULL, false, str, &Curr.maccss },
 { "ParaStyleCSS", pstyle, NULL, NULL, false, str, &Curr.maccss },
 { "CharStyleCSS", cstyle, NULL, NULL, false, str, &Curr.maccss },

 { "StyleWindow", pstyle, NULL, NULL, false, str, &Curr.cntwin },

 { "HtmlStyleCodeBefore", cstyle, NULL, NULL, false, str, &Curr.cmaccb },
 { "HtmlStyleCodeStart", cstyle, NULL, NULL, false, str, &Curr.cmaccs },
 { "HtmlStyleCodeEnd", cstyle, NULL, NULL, false, str, &Curr.cmacce },
 { "HtmlStyleCodeAfter", cstyle, NULL, NULL, false, str, &Curr.cmacca },
 { "HtmlStyleCodeReplace", cstyle, NULL, NULL, false, str, &Curr.cmaccr },
 { "StyleCodeBefore", cstyle, NULL, NULL, false, str, &Curr.cmaccb },
 { "StyleCodeStart", cstyle, NULL, NULL, false, str, &Curr.cmaccs },
 { "StyleCodeEnd", cstyle, NULL, NULL, false, str, &Curr.cmacce },
 { "StyleCodeAfter", cstyle, NULL, NULL, false, str, &Curr.cmacca },
 { "StyleCodeReplace", cstyle, NULL, NULL, false, str, &Curr.cmaccr },
 { "CharStyleCodeBefore", cstyle, NULL, NULL, false, str, &Curr.cmaccb },
 { "CharStyleCodeStart", cstyle, NULL, NULL, false, str, &Curr.cmaccs },
 { "CharStyleCodeEnd", cstyle, NULL, NULL, false, str, &Curr.cmacce },
 { "CharStyleCodeAfter", cstyle, NULL, NULL, false, str, &Curr.cmacca },
 { "CharStyleCodeReplace", cstyle, NULL, NULL, false, str, &Curr.cmaccr },

 { "HtmlStyleRowAttribute", pstyle, NULL, NULL, false, str, &Curr.macrat },
 { "HtmlStyleCellAttribute", pstyle, NULL, NULL, false, str, &Curr.maccat },
 { "HtmlStyleCellAbbr", pstyle, NULL, NULL, false, str, &Curr.maccab },
 { "HtmlStyleCellAxis", pstyle, NULL, NULL, false, str, &Curr.maccax },
 { "HtmlStyleCellScope", pstyle, NULL, NULL, false, str, &Curr.maccsc },
 { "StyleRowAttribute", pstyle, NULL, NULL, false, str, &Curr.macrat },
 { "StyleCellAttribute", pstyle, NULL, NULL, false, str, &Curr.maccat },
 { "StyleCellAbbr", pstyle, NULL, NULL, false, str, &Curr.maccab },
 { "StyleCellAxis", pstyle, NULL, NULL, false, str, &Curr.maccax },
 { "StyleCellScope", pstyle, NULL, NULL, false, str, &Curr.maccsc },

 { "StyleTabReplace", pstyle, "PStyleSet", NULL, false, str, &Curr.tabrep },

 { "ExtrReplace", pstyle, "ExtrSet", NULL, false, str, &Curr.macer },
 { "ExtrTitle", pstyle, "ExtrSet", NULL, false, str, &Curr.maceti },
 { "ExtrHead", pstyle, "ExtrSet", NULL, false, str, &Curr.maceh },
 { "ExtrTop", pstyle, "ExtrSet", NULL, false, str, &Curr.macet },
 { "ExtrBottom", pstyle, "ExtrSet", NULL, false, str, &Curr.maceb },

 // Tables settings for specific tables, table formats, and groups

 { "TableGroup", tabl, NULL, NULL, false, str, &CTbl.tbgroup },
 { "TableClasses", tabl, NULL, NULL, false, str, &CTbl.cssclass },

 // [Table]AccessMethod +++
 { "TableAccess", tabl, NULL, ISecTableAccess, false },

 // [Table]TableIndents, TableSizing, UseRowColor
 { "TableIndents", tabl, "TblPropSet", NULL, false, snum, &CTbl.tbind },
 { "TableSizing", tabl, "TblPropSet", NULL, false, cap, &CTbl.tbsiz },
 { "TableUseRowColor", tabl, "TblPropSet", NULL, false, yesno, &CTbl.tburc },

 // [Table]Border, Spacing, Padding, Attributes, [Attributes] table
 { "TableAttributes", tabl, "TblAttrSet", NULL, false, str, &CTbl.att },
 // [Attributes] tr, td, thead, tfoot tbody
 { "TableRowAttributes", tabl, "TblAttrSet", NULL, false, str, &CTbl.attr },
 { "TableCellAttributes", tabl, "TblAttrSet", NULL, false, str, &CTbl.attc },
 { "TableHeaderAttributes", tabl, "TblAttrSet", NULL, false, str, &CTbl.atth },
 { "TableFooterAttributes", tabl, "TblAttrSet", NULL, false, str, &CTbl.attf },
 { "TableBodyAttributes", tabl, "TblAttrSet", NULL, false, str, &CTbl.attb },

 { "TableBeforeMacros", tabl, "TblMacroSet", NULL, false, str, &CTbl.macb },
 { "TableStartMacros", tabl, "TblMacroSet", NULL, false, str, &CTbl.macs },
 { "TableReplaceMacros", tabl, "TblMacroSet", NULL, false, str, &CTbl.macr },
 { "TableEndMacros", tabl, "TblMacroSet", NULL, false, str, &CTbl.mace },
 { "TableAfterMacros", tabl, "TblMacroSet", NULL, false, str, &CTbl.maca },
 { "TableRowStartMacros", tabl, "TblMacroSet", NULL, false, str, &CTbl.macrs },
 { "TableRowEndMacros", tabl, "TblMacroSet", NULL, false, str, &CTbl.macre },
 { "TableCellStartMacros", tabl, "TblMacroSet", NULL, false, str, &CTbl.maccs },
 { "TableCellEndMacros", tabl, "TblMacroSet", NULL, false, str, &CTbl.macce },

 { "DITATables", tabl, NULL, NULL, false, str, &CTbl.dtname },
 { "DITATableParents", tabl, "TblParentSet", NULL, false, sarr, &CTbl.dtpar },
 { "DocBookTableParents", tabl, "TblParentSet", NULL, false, sarr, &CTbl.dtpar },

 // Graphics settings for specific graphics and groups
 { "GraphGroup", graphic, NULL, NULL, false, str, &GraphGroup },

 { "GraphFiles", graphic, "GraphAttrSet", NULL, false, str, &GraphFile },
 { "GraphAttr", graphic, "GraphAttrSet", NULL, false, str, &GraphAttr },
 { "GraphALT", graphic, "GraphAttrSet", NULL, false, str, &GraphALT },

 { "GraphAlign", graphic, "GraphAttrSet", NULL, false, str, &GraphAlign },
 { "GraphParaAlign", graphic, "GraphAttrSet", NULL, false, str, &GraphParaAlign },

 { "GraphDpi", graphic, "GraphAttrSet", NULL, false, str, &GraphDpi },
 { "GraphScale", graphic, "GraphAttrSet", NULL, false, snum, &GraphScaleVal },
 { "GraphWide", graphic, "GraphAttrSet", NULL, false, snum, &GraphWide },
 { "GraphHigh", graphic, "GraphAttrSet", NULL, false, snum, &GraphHigh },
 { "GraphIndents", graphic, "GraphAttrSet", NULL, false, snum, &GraphIndent },
 { "GraphRightSpacers", graphic, "GraphAttrSet", NULL, false, snum, &GraphRightSpacer },

 { "GraphStartMacros", graphic, "GraphMacroSet", NULL, false, str, &GraphStartMacro },
 { "GraphReplaceMacros", graphic, "GraphMacroSet", NULL, false, str, &GraphReplaceMacro },
 { "GraphEndMacros", graphic, "GraphMacroSet", NULL, false, str, &GraphEndMacro },

 { "DITAImageParents", graphic, "GraphParentSet", NULL, false, sarr, &GraphParents },
 { "DocBookImageParents", graphic, "GraphParentSet", NULL, false, sarr, &GraphParents },

 // Xref formats and instance settings
 { "XrefStyles", xrstyle, NULL, ISecXrefStyles, false },  // NOT used for .ini
 { "XrefStyleLinkSrc", xrstyle, "XrefMacroSet", NULL, false, str, &XLSMacro },


#if 0
 // The rest will never need to be changed with markers
 // Either there's another way (like for macvars), or they
 // affect the whole file rather than one part of it

 { "GraphSuffix", suf, NULL, NULL, false },

 // Text-related
 { "TextFlows", flow, NULL, NULL, false },
 { "CharConvert", ch, NULL, NULL, false },
 { "Fonts", fnt, NULL, NULL, false },
 { "FontSizes", fsize, NULL, NULL, false },
 { "Colors", colr, NULL, NULL, false },

 // Marker-related
 { "Markers", mark, NULL, NULL, false },
 { "MarkerTypes", mark, NULL, NULL, false },
 { "MarkerTypeCodeBefore", mark, NULL, NULL, false },
 { "MarkerTypeCodeAfter", mark, NULL, NULL, false },

 // Macros
 { "CSSStartMacro", macr, NULL, NULL, false },
 { "CSSEndMacro", macr, NULL, NULL, false },
 { "UserVars", macvar, NULL, NULL, false },
 { "MacroVariables", macvar, NULL, NULL, false },

 // File-related
 { "Titles", file, NULL, NULL, false },
 { "HtmlFiles", file, NULL, NULL, false },
 { "FileIDs", file, NULL, NULL, false },
 { "XrefFiles", file, NULL, NULL, false },
 { "RefFiles", file, NULL, NULL, false },
 { "FileSequence", file, NULL, NULL, false },

 // All Help formats
 { "TargetFiles", file, NULL, NULL, false },

 // JH only
 { "JH window name", set, NULL, NULL, false },
 { "JHImages", jhimg, NULL, NULL, false },
 { "JH2_HelpsetAddition", macr, NULL, NULL, false },
 { "TocLevelExpand", toclev, NULL, NULL, false },
 { "TocLevelImage", toclev, NULL, NULL, false },

 // Java only
 { "HelpMergePaths", proj, NULL, NULL, false },

 // OHJ only
 { "OracleHelpWindows", wind, NULL, NULL, false },

 // HH only
 { "ChmFiles", file, NULL, NULL, false },

 // HH and OH only
 { "HelpMerge", proj, NULL, NULL, false },

 // OH only
 { "OHTopLeftNav", macr, NULL, NULL, false },
 { "OHTopRightNav", macr, NULL, NULL, false },
 { "OHMergeFiles", file, NULL, NULL, false },
 { "StopWords", macr, NULL, NULL, false },

 // { "", , NULL, NULL, false },
#endif
 { NULL, unknown, NULL, NULL, false }
};


void
HTMwr::LocTocSet(char *)
{
	Curr.loctocset = true;
}


void
HTMwr::TrailSet(char *)
{
	Curr.trailset = true;
}




// AutoSet

setinfo HTMwr::ISecAuto[] = {
 { "WrapAndShip", yesno, &DoAutomation, NULL },
 { "CopyOriginalGraphics", yesno, &CopyOriginalGraphics, NULL },


 { NULL, spec, NULL, NULL }
};



// UdocOptionsSet

setinfo HTMwr::ISecUdocOptions[] = {
 { "WriteKeydefLib", yesno, &MXLwr::WriteKeydefLib, NULL },
 { "UseKeydefAliases", yesno, &MXLwr::UseKeydefAliases, NULL },
 { "KeydefLibSrc", str, &MXLwr::OutputKeydefLibSrc, NULL },


 { NULL, spec, NULL, NULL }
};


// OptionsSet

setinfo HTMwr::ISecOptions[] = {
 { "CaselessMatch", yesno, &CaselessMatch, CaselessMatchSet },
 { "SpacelessMatch", yesno, &SpacelessMatch, SpacelessMatchSet },
 { "WildcardMatch", yesno, &WildcardMatch, WildcardMatchSet },

 { "MathFullForm", yesno, &MathFullForm, NULL },
 { "UsePartFiles", yesno, &UsePartFiles, NULL },
 { "AllowPadding", yesno, &AllowPadding, NULL },

 { "ICUdll", str, &ICUdll, NULL },

 { "Confluence", yesno, &Confluence, ConfluenceSet },
 { "ConfluenceLinks", yesno, &ConfluenceLinks, NULL },
 { "ConfluenceLinkStart", str, &ConfluenceLinkStart, NULL },
 { "ConfluenceLinkPage", str, &ConfluenceLinkPage, NULL },
 { "ConfluenceLinkPageEnd", str, &ConfluenceLinkPageEnd, NULL },
 { "ConfluenceLinkText", str, &ConfluenceLinkText, NULL },
 { "ConfluenceLinkTextEnd", str, &ConfluenceLinkTextEnd, NULL },
 { "ConfluenceLinkEnd", str, &ConfluenceLinkEnd, NULL },

 //{ "IncludeMapsInOutput", yesno, &IncludeMaps, NULL },

 { "PageBreaks", cap, &PgBreaks, NULL },
 { "PageBreak", cap, &PgBreaks, NULL },
 { "UseFileIDs", yesno, &UseXrefFilePrefix, NULL },
 { "ReAnchorFrames", yesno, &AnchorFrames, NULL },
 { "UseGraphicPreviews", yesno, &UseGraphicPreviews, NULL },

 { "ObjectIDs", cap, &HUType, NULL },
 { "Title", str, &Title, NULL },
 { "FileSuffix", str, &XrefFileSuffix, XrefFileSuffixSet },

 { "RemoveFilePaths", yesno, &RemoveFilePaths, NULL },
 { "ListMissingRefs", yesno, &ListMissingRefs, NULL },
 { "CheckAllRefs", yesno, &CheckAllRefs, NULL },

 { "ExtractEnable", yesno, &ExtractEnable, NULL },

 { "HideWhiteText", yesno, &HideWhite, NULL },
 { "RemoveEmptyParagraphs", yesno, &RemEmptyParas, NULL },
 { "EmptyParaContent", str, &EmptyParaContent, NULL },
 { "GlossTitlePath", str, &GlossTitlePath, NULL },

 { "ConditionCharTag", str, &ConditionCharTag, NULL },

 { "RunInHeads", cap, &RuninType, NULL },
 { "UseRunInTag", yesno, &UseRunInTag, NULL },
 { "NoParaClose", yesno, &NoParaClose, NULL },
 { "NoWrap", yesno, &NoWrap, NULL },

 { "UseHyperColor", yesno, &UseHyperColor, NULL },
 { "AllowOverrides", yesno, &AllowOver, NULL },

 { "NoFont", yesno, &NoFont, NULL },
 { "NoFonts", yesno, &NoFont, NULL },
 { "NoSymbolFont", yesno, &NoSymbolFont, NULL },
 { "UseFontFace", yesno, &UseFontFace, NULL },
 { "UseFontSize", yesno, &UseFontSize, NULL },
 { "Basefont", yesno, &Basefont, NULL },
 { "AlignAttributes", yesno, &AlignAttributes, NULL },
 { "MinimizeLines", yesno, &MinimizeLines, NULL },
 { "XMLBreakPara", yesno, &HardRetPara, NULL },
 { "UseXMLbr", yesno, &UseXMLbr, NULL },
 { "MergePre", yesno, &MergePre, NULL },

 { "DefaultTarget", str, &DefaultTarget, DefaultTargetSet },
 { "URLTarget", str, &URLTarget, NULL },
 { "RemoveFramesAbove", yesno, &ProhibitPrefix, NULL },
 { "RemoveFramesBelow", yesno, &ProhibitSuffix, NULL },

 { "ClassSpaceChar", chr, &ClassSpaceChar, ClassSpaceCharSet },
 { "HyperSpaceChar", chr, &HyperSpaceChar, HyperSpaceCharSet },
 { "XrefSpaceChar", chr, &XrefSpaceChar, XrefSpaceCharSet },
 { "CharConvertSpaces", yesno, &CharConvertSpaces, NULL },

 { "Footnotes", cap, &FootType, NULL },
 { "UseFootnoteParaCode", yesno, &UseFootnoteParaCode, NULL },
 { "NoFootnoteLinks", yesno, &NoFootnoteLinks, NULL },
 { "FootnoteSeparator", str, &FootSep, NULL },
 { "FootnoteStartCode", str, &FootnoteStartCode, NULL },
 { "FootnoteEndCode", str, &FootnoteEndCode, NULL },
 { "UseFootnoteLists", yesno, &UseFootnoteLists, NULL },
 { "UseTbFootnoteLists", yesno, &UseTbFootnoteLists, NULL },
 { "FootClass", str, &FootClass, NULL },
 { "TbFootClass", str, &TbFootClass, NULL },
 { "FootInlineTag", str, &FootInlineTag, NULL },
 { "FootInlineParaTag", str, &FootInlineParaTag, NULL },
 { "FootInlineIDPrefix", str, &FootInlineIDPrefix, NULL },
 { "FootInlineXrefTag", str, &FootInlineXrefTag, NULL },
 { "UseFootXrefTag", yesno, &UseFootXrefTag, NULL },

 { "ConversionDPI", snum, &ConversionDPI, NULL },

 { "UseSpacers", yesno, &UseSpacers, NULL },
 { "PixelSpacerImage", str, &PixelSpacerImage, NULL },
 { "SpacerAlt", str, &SpacerAlt, NULL },
 { "WriteSpacerFile", yesno, &WriteSpacerFile, NULL },
 { "WriteSpacer", yesno, &WriteSpacerFile, NULL },

 { "MadeWithImageFile", str, &MadeWithImageFile, NULL },
 { "WriteMadeWithGraphic", cap, &WriteMadeWithGraphic, WriteMadeWithGraphicSet },
 { "WriteMadeWithFile", yesno, &WriteMadeWithFile, NULL },
 { "MadeWithUsed", yesno, &MadeWithUsed, NULL },
 { "MadeWithPara", yesno, &MadeWithPara, NULL },
 { "MadeWithLink", yesno, &MadeWithLink, NULL },
 { "MadeWithAttributes", str, &MadeWithAttributes, NULL },

 { "AnumTabs", yesno, &AnumTabs, NULL },
 { "UseAnums", yesno, &UseAnums, NULL },
 { "TabCharsPerInch", usnum, &TabCharsPerInch, TabCharsPerInchSet },

 { "UnwrapPRE", yesno, &UnwrapPRE, NULL },
 { "ScriptType", str, &ScriptType, NULL },

 { "UseDOCTYPE", yesno, &UseDOCTYPE, NULL },
 { "HTMLVersion", usnum, &HTMLVersion, HTMLVersionSet },
 { "HTMLDocType", str, &PrologDType, HTMLDocTypeSet },
 { "HTMLDTD", str, &PrologDTD, HTMLDTDSet },
 { "ValidOnly", yesno, &ValidOnly, NULL },
 { "AllowNobr", yesno, &AllowNobr, NULL },

 { "GeneratorTag", cap, &GeneratorTag, NULL },

 { "UseFrameSet", yesno, &UseFrameSet, NULL },
 { "XHNamespace", str, &XhtmlNS, NULL },
 { "XHLanguage", str, &XhtmlLang, NULL },
 { "XHLangAttr", str, &XHLangAttr, NULL },

 { "UseXMLDeclaration", yesno, &UseXMLDeclaration, NULL },
 { "XMLVersion", str, &XMLVer, NULL },
 { "UseXMLRoot", yesno, &UseXMLRoot, NULL },
 { "XMLRoot", str, &XMLRoot, NULL },
 { "UseHeadAndBody", yesno, &UseHeadAndBody, NULL },
 { "BodyContentOnly", yesno, &BodyContentOnly, NULL },
 //{ "NestXML", yesno, &NestXML, NULL },
 { "QuotedEncoding", yesno, &QuotedEncoding, NULL },
 { "Encoding", cap, &Encoding, EncodingSet },

 { "XMLEncoding", str, &XMLEncoding, NULL },
 { "NumericCharRefs", yesno, &NumericCharRefs, NULL },
 { "Ansi", str, &Ansi, AnsiSet },
 { "ContentType", str, &ContentType, NULL },

 { "UseRawName", yesno, &UseRawName, NULL },
 { "KeepFileNameUnderscores", yesno, &KeepFileNameUnderscores, NULL },
 { "KeepFileNameSpaces", yesno, &KeepFileNameSpaces, NULL },
 { "ChangeFileNameSpaces", yesno, &ChangeFileNameSpaces, NULL },
 { "FileNameSpaceChar", chr, &FileNameSpaceChar, NULL },
 { "UseFilePxSxInBaseName", yesno, &UseFilePxSxInBaseName, NULL },
 { "UseRawNewlinks", yesno, &UseRawNewlinks, NULL },
 { "MakeFileHrefsLower", yesno, &MakeFileHrefsLower, NULL },
 { "ATagLineBreak", yesno, &ATagLineBreak, NULL },

 { "MakeAliasFile", yesno, &MakeAliasFile, NULL },
 { "AliasPrefix", str, &AliasPrefix, NULL },

 { "RemoveANames", yesno, &NoANames, NULL },
 { "RemoveATags", yesno, &NoATags, NULL },
 { "RemoveAHrefAttrs", yesno, &NoAHrefs, NULL },
 { "XMLLinkAttrs", yesno, &XMLLinkAttrs, NULL },
 { "ATagElement", str, &ATagName, NULL },
 { "HrefAttribute", str, &HrefAttribute, NULL },
 { "UseHash", yesno, &UseHash, NULL },
 { "UseUlink", yesno, &UseUlink, NULL },
 { "RemoveXrefHotspots", yesno, &RemoveXrefHotspots, NULL },

 { "StartingSplit", yesno, &StartingSplit, NULL },
 { "SmartSplit", yesno, &SmartSplit, NULL },
 { "UseChunking", yesno, &UseChunking, NULL },

 { "KeepReplacedCharLinks", yesno, &KeepReplacedCharLinks, NULL },

 { "NoLocations", yesno, &NoLocations, NULL },

 { "SplitNumStart", str, &SplitNumStart, SplitNumStartSet },

 { "SplitNumIncrement", lnum, &SplitNumIncrement, NULL },

 { "IndexSortType", cap, &IndexSortType, NULL },
 { "IndexSortLocale", str, &IndexSortLocale, NULL },
 { "IndexRanges", yesno, &IndexRanges, NULL },

 { "DefXrefToCurr", yesno, &DefXrefToCurr, NULL },
 { "UseListedXrefFilesOnly", yesno, &UseListedXrefFilesOnly, NULL },

 { "PrintProject", yesno, &PrintProject, NULL },

 { NULL, spec, NULL, NULL }
};


void
HTMwr::CaselessMatchSet(char *)
{
	IniFile->SetMatch(1, CaselessMatch);
}

void
HTMwr::SpacelessMatchSet(char *)
{
	IniFile->SetMatch(2, SpacelessMatch);
}

void
HTMwr::WildcardMatchSet(char *)
{
	IniFile->SetMatch(3, WildcardMatch);
}

void
HTMwr::ConfluenceSet(char *)
{
	if (Confluence) {
		ConfluenceLinks = true;
		NoANames = true;
		NoLocations = true;
		UseHash = false;
		StartingSplit = true;
		UseSpanAsDefault = false;
		UseXMLDeclaration = false;
		UseDOCTYPE = false;
		UseHeadAndBody = false;
		UseCSS=false;
		UseCSSSet(NULL);
		AlignAttributes = false;
		NoFont = true;
	}
}

void
HTMwr::XrefFileSuffixSet(char *)
{
	if (*XrefFileSuffix == '.')
		XrefFileSuffix++;
}

void
HTMwr::DefaultTargetSet(char *)
{
	TargetFrame = DefaultTarget;
}

void
HTMwr::ClassSpaceCharSet(char *)
{
	ClassSpaceChar &= 0x7F;
	if (!isalnum((int) ClassSpaceChar)
	 && (ClassSpaceChar != '_')
	 && (ClassSpaceChar != '-'))
		ClassSpaceChar = 0;
}

void
HTMwr::HyperSpaceCharSet(char *)
{
	HyperSpaceChar &= 0x7F;
	if (!isalnum((int) HyperSpaceChar)
	 && (HyperSpaceChar != '_')
	 && (HyperSpaceChar != '-'))
		HyperSpaceChar = 0;
}

void
HTMwr::XrefSpaceCharSet(char *)
{
	XrefSpaceChar &= 0x7F;
	if (!isalnum((int) XrefSpaceChar)
	 && (XrefSpaceChar != '_')
	 && (XrefSpaceChar != '-'))
		XrefSpaceChar = 0;
}

void
HTMwr::WriteMadeWithGraphicSet(char *)
{
	switch (WriteMadeWithGraphic) {
		case 'A':
			MadeWithUsed = true;
			break;
		case 'N':
			WriteMadeWithFile = false;
			break;
		case 'M':
		default:
			break;
	}
}

void
HTMwr::TabCharsPerInchSet(char *)
{
	TabCharWidth = 1440 / TabCharsPerInch;
}

void
HTMwr::HTMLVersionSet(char *)
{
	switch (HTMLVersion) {
		case 2:
			PrologDType = PrologDType2;
			break;
		case 3:
			PrologDType = PrologDType3;
			break;
		case 4:
			PrologDType = PrologDType4;
			break;
		default:
			break;
	}
}

void
HTMwr::HTMLDocTypeSet(char *)
{
	PrologDTypeSet = true;
}

void
HTMwr::HTMLDTDSet(char *)
{
	PrologDTDSet = true;
}

void
HTMwr::EncodingSet(char *)
{
	switch (Encoding) {
		case 'U':
			UTF8 = true;
			NoEnc = false;
			XMLEncoding = "UTF-8";
			NumericCharRefs = false;
			break;
		case 'I':
			UTF8 = false;
			NoEnc = false;
			XMLEncoding = "ISO-8859-1";
			break;
		case 'N':
			NoEnc = true;
			XMLEncoding = "ISO-8859-1";
			break;
		default:
			break;
	}
}

void
HTMwr::AnsiSet(char *)
{
	SetAnsiMap(Ansi);
}

void
HTMwr::SplitNumStartSet(char *)
{
	NextSplitNum = atol(SplitNumStart);
}



// AttributesSet

setinfo HTMwr::ISecAttr[] = {
 { "body", str, &AttrBody, NULL },
 { "table", str, &AttrTable, NULL },
 { "row", str, &AttrRow, NULL },
 { "tr", str, &AttrRow, NULL },
 { "th", str, &AttrCellHead, NULL },
 { "td", str, &AttrCellBody, NULL },
 { "pre", str, &PreFormatClass, NULL },

 { NULL, spec, NULL, NULL }
};



// MapSet

setinfo HTMwr::ISecMapOptions[] = {
 { "IncludeMapsInOutput", yesno, &IncludeMaps, NULL },
 { "UseMapDescAsTitle", yesno, &UseMapDescAsTitle, NULL },

 { NULL, spec, NULL, NULL }
};


// CssSet

setinfo HTMwr::ISecCss[] = {
 { "Stylesheet", cap, &CssType, StylesheetSet },
 { "UseCSS", yesno, &UseCSS, UseCSSSet },
 { "WriteClassAttributes", yesno, &WriteClassAttributes, NULL },
 { "WriteCssStylesheet", cap, &WriteCssStylesheet, NULL },
 { "WriteCssLink", yesno, &WriteCssLink, NULL },
 { "CssBrowserDetect", str, &CssBrowserDetect, NULL },
 { "CssFileName", str, &CssName, NULL },
 { "CssPath", str, &CssPath, NULL },

 { "UseCharRangeClasses", yesno, &UseCharRangeClasses, NULL },
 { "UseParaCondClasses", yesno, &UseParaCondClasses, NULL },

 { "CSSLinkNS4", yesno, &CSSLinkNS4, NULL },
 { "ClassIsTag", yesno, &ClassIsTag, NULL },
 { "SelectorIncludesTag", yesno, &SelectorIncludesTag, NULL },
 { "LowerCaseCSS", yesno, &LowerCaseCSS, NULL },
 { "UseCSSLeading", yesno, &CSSLeading, NULL },
 { "NoClassLists", yesno, &NoClassLists, NULL },
 { "NoAttribLists", yesno, &NoAttribLists, NULL },
 { "UseListTypeAttribute", yesno, &UseListTypeAttribute, NULL },
 { "AlwaysNestLists", yesno, &AlwaysNestLists, NULL },
 { "LinkClassIsParaClass", yesno, &LinkClassIsParaClass, NULL },
 { "XrefFormatIsXrefClass", yesno, &XrefFormatIsXrefClass, NULL },
 { "ZeroCSSMargins", yesno, &ZeroCSSMargins, NULL },
 { "UseSpanAsDefault", yesno, &UseSpanAsDefault, NULL },

 { "FootClass", str, &FootClass, NULL },
 { "TbFootClass", str, &TbFootClass, NULL },

 { "CssFontUnits", ucnum, &CssFontUnits, NULL },
 { "CssIndentUnits", ucnum, &CssIndentUnits, NULL },
 { "CssFontUnitDec", ucnum, &CssFontUnitDec, NULL },
 { "CssIndentUnitDec", ucnum, &CssIndentUnitDec, NULL },
 { "CssBodyFontSize", lnum, &CssBodyFontSize, CssBodyFontSet },
 { "CssBodyFontUnit", ucnum, &CssBodyFontUnit, CssBodyFontSet },
 { "CssBodyFontTag", yesno, &CssBodyFontTag, NULL },
 { "CssIndentBaseSize", lnum, &CssIndentBaseSize, CssIndentBaseSet },
 { "CssIndentBaseUnit", ucnum, &CssIndentBaseUnit, CssIndentBaseSet },
 { NULL, spec, NULL, NULL }
};


void
HTMwr::StylesheetSet(char *)
{
	if (CssType == 'N') {
		UseCSS = false;
		UseCSSSet(NULL);
	}
	else {
		UseCSS = true;
		UseCSSSet(NULL);

		if (CssType == 'I') {
			WriteCssStylesheet = 'O';
			WriteCssLink = true;
		}
		else if (CssType == 'G') {
			WriteCssStylesheet = 'A';
			WriteCssLink = true;
		}
		else if (CssType == 'C') {
			WriteCssStylesheet = 'N';
			WriteCssLink = false;
		}
		else if (CssType == 'U') {
			WriteCssStylesheet = 'N';
			WriteCssLink = true;
		}
	}
}

void
HTMwr::UseCSSSet(char *)
{
	if (UseCSS) {
		WriteClassAttributes = ClassIsTag ? false : true;
		WriteCssStylesheet = 'O';
		WriteCssLink = true;
		NoFont = true;
		Basefont = false;
		AlignAttributes = false;
		GraphAlignAttributes = false;
		NoClassLists = false;
		LinkClassIsParaClass = true;
		XrefFormatIsXrefClass = true;
	}
	else {
		WriteClassAttributes = false;
		WriteCssStylesheet = 'N';
		WriteCssLink = false;
		NoFont = false;
		Basefont = true;
		AlignAttributes = true;
		GraphAlignAttributes = true;
		NoClassLists = true;
		LinkClassIsParaClass = false;
		XrefFormatIsXrefClass = false;
	}
}

void
HTMwr::CssBodyFontSet(char *)
{
	SetCssFontTwips();
}

void
HTMwr::CssIndentBaseSet(char *)
{
	SetCssIndentTwips();
}


// FDKSet

setinfo HTMwr::ISecFDK[] = {
 { "PrjFileName", path, &PrjFileName, NULL },
 { "IDFileName", path, &IDFileName, NULL },
 { "FrameBook", path, &FrameBook, NULL },
 { "UseLocalFileID", yesno, &UseLocalFileID, NULL },
 { "EquationFrameExpand", snum, &EqFrExpand, NULL },

 { "FileSuffix", str, &XrefFileSuffix, XrefFileSuffixSet },

// { "UseExistingDCL", yesno, &UseExistingDCL, NULL },
// { "DeleteExistingDCL", yesno, &DeleteExistingDCL, NULL },

 { "HSFileName", str, &DefaultHelpSetFile, NULL },
 { "HHPFileName", str, &DefaultHelpProjectFile, NULL },

 { "GraphicNameDigits", lnum, &FrIDLen, GraphicNameDigitsSet },

 { NULL, spec, NULL, NULL }
};

void
HTMwr::GraphicNameDigitsSet(char *)
{
	if (FrIDLen < 6)
		FrIDLen = 6;
	else if (FrIDLen > NumStrMax)
		FrIDLen = NumStrMax;
}


// TableSet

setinfo HTMwr::ISecTable[] = {

 { "UseCALSModel", yesno, &CALS, NULL },

 // These four are overridden by [TableAttributes] settings
 { "Border", usnum, &TblBorderThick, NULL },
 { "Spacing", usnum, &TblCellSpacing, NULL },
 { "Padding", usnum, &TblCellPadding, NULL },
 { "TableAttributes", yesno, &TableAttributes, NULL },

 { "CellAlignAttributes", yesno, &CellAlignAttributes, NULL },
 { "CellColorAttributes", yesno, &CellColorAttributes, NULL },

 { "TableSizing", cap, &TTblWidType, NULL, 0, NULL, "TableSizing" },
 { "TableIndents", snum, &TblIndents, NULL, 0, NULL, "TableIndents" },
 { "UseAltShading", yesno, &UseAltShading, NULL },
 { "KeepTableClassSpace", yesno, &KeepTableClassSpace, NULL },
 { "StripTable", yesno, &StripTable, StripTableSet },
 { "StripTables", yesno, &StripTable, StripTableSet },
 { "CloseStrippedTables", yesno, &CloseStrippedTables, NULL },
 { "DefColWidth", usnum, &DefColWidth, NULL },

 { "TableDPI", usnum, &TableDPI, TableDPISet },
 { "InternalTableCaption", yesno, &IntTbCaption, NULL },
 { "TableTitles", usnum, &TableTitles, NULL },
 { "TableWordBreak", yesno, &TableWordBreak, NULL },
 { "TableFootnoteSeparator", str, &TbFootSep, NULL },
 { "TableFootnotesWithTable", yesno, &TbFootPos, NULL },

 { "TableContinued", yesno, &TbContUse, NULL },
 { "TableContVar", str, &TbContVarName, NULL },
 { "TableSheet", yesno, &TbSheetUse, NULL },
 { "TableSheetVar", str, &TbSheetVarName, NULL },
 { "AllowTbSplit", yesno, &AllowTbSplit, NULL },
 { "AllowTbTitle", yesno, &AllowTbTitle, NULL },
 { "UseInformaltableTag", yesno, &UseInformaltableTag, NULL },
 { "TableCaptionTag", str, &TblCaptionTag, NULL },
 { "UseTbHeaderCode", yesno, &UseTbHeaderCode, NULL },
 { "UseRowColor", yesno, &TUseRowColor, NULL, 0, NULL, "TableUseRowColor" },
 { "RemoveEmptyTableParagraphs", yesno, &RemEmptyTbParas, NULL },
 { "EmptyTbCellContent", str, &EmptyTbCellContent, NULL },

 // All of these can be overridden by props in [TableAccess]
 { "AccessMethod", cap, &AccessMethod, AccessMethodSet, 0, NULL, "TableAccess" },
 { "TableHeaderCols", usnum, &THeaderCols, NULL },
 { "TableHeaderRows", usnum, &THeaderRows, NULL },
 { "TableFooterRows", usnum, &TFooterRows, NULL },
 { "ColIDs", yesno, &TColIDs, NULL },
 { "RowIDs", yesno, &TRowIDs, NULL },
 { "ColSpanIDs", yesno, &TColSpanIDs, NULL },
 { "RowSpanIDs", yesno, &TRowSpanIDs, NULL },
 { "ScopeCol", yesno, &TScopeCol, NULL },
 { "ScopeColGroup", yesno, &TScopeColGroup, TScopeColGroupSet },
 { "ScopeRow", yesno, &TScopeRow, NULL },
 { "ScopeRowGroup", yesno, &TScopeRowGroup, TScopeRowGroupSet },
 { "ColGroupIDs", yesno, &TColGroupIDs, NULL },
 { "RowGroupIDs", yesno, &TRowGroupIDs, NULL },
 { "HeadFootBodyTags", yesno, &THeadFootBodyTags, NULL },
 { "ColGroupElements", yesno, &TColGroupElements, NULL },

 { "ColHead", str, &ColHead, NULL },
 { "RowHead", str, &RowHead, NULL },
 { "ColSpanHead", str, &ColSpanHead, NULL },
 { "RowSpanHead", str, &RowSpanHead, NULL },
 { "ColGroupHead", str, &ColGroupHead, NULL },
 { "RowGroupHead", str, &RowGroupHead, NULL },
 { "FootTagLast", yesno, &FootTagLast, NULL },

 { NULL, spec, NULL, NULL }
};

void
HTMwr::StripTableSet(char *)
{
	if (StripTable == true) {
		AllowTbSplit = true;
		AllowTbTitle = true;
	}
}

void
HTMwr::TableDPISet(char *)
{
	if (TblDPI == 0)
		TblDPI = 96;
}

void
HTMwr::AccessMethodSet(char *)
{
	switch (AccessMethod) {
		case 'N':
			TScopeColGroup = false;
			TScopeRowGroup = false;
			TScopeCol = false;
			TScopeRow = false;
			TColGroupIDs = false;
			TRowGroupIDs = false;
			TColIDs = false;
			TRowIDs = false;
			TColSpanIDs = false;
			TRowSpanIDs = false;
			break;
		case 'S':
			TScopeColGroup = true;
			TScopeRowGroup = true;
			TScopeCol = true;
			TScopeRow = true;
			TColGroupIDs = false;
			TRowGroupIDs = false;
			TColIDs = false;
			TRowIDs = false;
			TColSpanIDs = true;
			TRowSpanIDs = true;
			THeadFootBodyTags = true;
			TColGroupElements = true;
			break;
		case 'I':
			TScopeColGroup = false;
			TScopeRowGroup = false;
			TScopeCol = false;
			TScopeRow = false;
			TColGroupIDs = true;
			TRowGroupIDs = true;
			TColIDs = true;
			TRowIDs = true;
			TColSpanIDs = true;
			TRowSpanIDs = true;
			break;
		default:
			break;
	}
}

void
HTMwr::TScopeColGroupSet(char *)
{
	if (TScopeColGroup)
		TColGroupElements = true;
}

void
HTMwr::TScopeRowGroupSet(char *)
{
	if (TScopeRowGroup)
		THeadFootBodyTags = true;
}


// GraphicsSet

setinfo HTMwr::ISecGraphics[] = {
 { "StripGraphPath", yesno, &StripGraphPath, NULL },
 { "GraphPath", str, &GraphPath, GraphPathSet },
 { "GraphSuffix", str, &GraphSuffix, NULL },
 { "GraphClass", str, &GraphClass, NULL },
 { "GraphScale", yesno, &GraphScale, NULL },
 { "UsePxSuffix", yesno, &UsePxSuffix, NULL },
 { "UsePtSuffix", yesno, &UsePtSuffix, NULL },
 { "RefPageGraphIndent", yesno, &RefPageGraphIndent, NULL },
 { "GraphPathOverrides", yesno, &GraphPathOverrides, NULL },
 { "GraphAlignAttributes", yesno, &GraphAlignAttributes, NULL },
 { "GraphWrapPara", yesno, &GraphWrapPara, NULL },
 { "KeepGraphicsInPara", yesno, &KeepGraphicsInPara, NULL },
 { "UseGraphicPreviews", yesno, &UseGraphicPreviews, NULL },
 { "UseOriginalGraphicNames", yesno, &UseOrigGrName, NULL },
 { "UseImageIDs", yesno, &ImgNames, NULL },
 { "FixGraphSpaces", yesno, &FixGraphSpaces, NULL },
 { "ANameTagsFirst", yesno, &GraphANamesFirst, NULL },
 { "UseTitleForAlt", yesno, &UseTitleForAlt, NULL },
 { "AllowEmptyAlt", yesno, &AllowEmptyAlt, NULL },

 { "XMLGraphAttrs", yesno, &XMLGraphAttrs, NULL },
 { "ImgTagElement", str, &ImgTagElement, NULL },
 { "ImgSrcAttr", str, &ImgSrcAttr, NULL },

 { "ExtrGraphThumbnail", cap, &ExtrGraphThumbnail, NULL },
 { "OrigSizedThumbnail", yesno, &OrigSizedThumbnail, NULL },
 { "ExtrGraphSuffix", str, &ExtrGraphSuffix, NULL },
 { "ExtrGraphClass", str, &ExtrGraphClass, NULL },
 { "ExtrGraphTarget", str, &ExtrGraphTarget, NULL },
 { "ExtrGraphHigh", str, &ExtrGraphHigh, NULL },
 { "ExtrGraphWide", str, &ExtrGraphWide, NULL },

 { "RetainRuninImagesForEmptyParagraphs", yesno,
   &RetainRuninImagesForEmptyParagraphs, NULL },

 { NULL, spec, NULL, NULL }
};

void
HTMwr::GraphPathSet(char *)
{
	char *cpt = GraphPath;

	for ( ; *cpt; cpt++) {
		if (*cpt == '\\')
			*cpt = '/';
	}
	if (*(cpt - 1) == '/')
		*(cpt - 1) = '\0';
}


// TrailsSet

setinfo HTMwr::ISecTrails[] = {
 { "MakeTrail", yesno, &MakeTrail, NULL },
 { "SplitTrail", yesno, &SplitTrail, NULL },
 { "TrailStart", str, &TrailStart, NULL },
 { "TrailSep", str, &TrailSep, NULL },
 { "TrailEnd", str, &TrailEnd, NULL },
 { "TrailLinkClass", str, &TrailLinkClass, NULL },
 { "TrailCurrent", cap, &TrailCurrent, NULL },
 { "TrailPosition", cap, &TrailPosition, NULL },
 { "TrailFirstLevel", usnum, &TrailFirstLevel,  TrailFirstLevelSet},
 { "TrailLastLevel", usnum, &TrailLastLevel, TrailLastLevelSet },
 { "TrailIndent", usnum, &TrailIndent, TrailIndentSet },
 { NULL, spec, NULL, NULL }
};

void
HTMwr::TrailFirstLevelSet(char *)
{
	if ((TrailFirstLevel < TrailLevMin)
	 || (TrailFirstLevel > (TrailLevMax - 1)))
		TrailFirstLevel = TrailLevMin;
}

void
HTMwr::TrailLastLevelSet(char *)
{
	if ((TrailLastLevel > TrailLevMax)
	 || (TrailLastLevel < TrailFirstLevel))
		TrailLastLevel = TrailLevMax;
}

void
HTMwr::TrailIndentSet(char *)
{
	if (TrailIndent > 4)
		TrailIndent = 4;
}


// LocalTOCSet

setinfo HTMwr::ISecLocalTOC[] = {
 { "MakeLocalTOC", yesno, &MakeLocalTOC, NULL },
 { "LocalTOCSubs", yesno, &LocalTOCSubs, LocalTOCSubsSet },
 { "LocalTOCItem", str, &LocalTOCItem, NULL },
 { "LocalTOCStart", str, &LocalTOCStart, NULL },
 { "LocalTOCEnd", str, &LocalTOCEnd, NULL },
 { NULL, spec, NULL, NULL }
};

void
HTMwr::LocalTOCSubsSet(char *)
{
	if (LocalTOCSubs)
		LocalTOCItem = "<p class=\"loctocind<$$loctocind>\"><a href=\"<$$loctocfile>\"><$$loctoctitle></a></p>";
	else
		LocalTOCItem = "<p class=\"localtocitem\"><a href=\"<$$_loctocfile>\"><$$_loctoctitle></a></p>";
}


// FilterConditionsSet

setinfo HTMwr::ISecFilterConditions[] = {
 { "FilterConditions", yesno, &FilterConditions, NULL },
 { "FilterJSFile", str, &FilterJSFile, NULL },

 { "FilterDialogTitle", str, &FilterDialogTitle, NULL },
 { "FilterBodyAttr", str, &FilterBodyAttr, NULL },
 { "FilterLinkHref", str, &FilterLinkHref, NULL },
 { "FilterDialogWide", usnum, &FilterDialogWide,  NULL},
 { "FilterDialogHigh", usnum, &FilterDialogHigh,  NULL},

 { "FilterBarStart", str, &FilterBarStart, NULL },
 { "FilterBarSpan", str, &FilterBarSpan, NULL },
 { "FilterBarEnd", str, &FilterBarEnd, NULL },
 { "FilterBarBodyAttr", str, &FilterBarBodyAttr, NULL },

 { NULL, spec, NULL, NULL }
};



// DropDownsSet

setinfo HTMwr::ISecDropDowns[] = {
 { "UseDropDowns", yesno, &DropDownUsed, NULL },
 { "DropLinkType", cap, &DropLinkType, NULL },
 { "DropLinkStart", str, &DropLinkStart, NULL },
 { "DropLinkEnd", str, &DropLinkEnd, NULL },
 { "DropLinkParaStart", str, &DropLinkParaStart, NULL },
 { "DropLinkParaText", str, &DropLinkParaText, NULL },
 { "DropLinkParaEnd", str, &DropLinkParaEnd, NULL },
 { "DropBlockStart", str, &DropBlockStart, NULL },
 { "DropBlockEnd", str, &DropBlockEnd, NULL },
 { "ClickBlockToClose", yesno, &ClickBlockToClose, NULL },
 { "WriteDropIconFiles", yesno, &WriteDropIconFiles, NULL },
 { "DropOpenIcon", str, &DropOpenIcon, NULL },
 { "DropOpenIconFile", str, &DropOpenIconFile, NULL },
 { "DropOpenIconAlt", str, &DropOpenIconAlt, NULL },
 { "DropCloseIcon", str, &DropCloseIcon, NULL },
 { "DropCloseIconFile", str, &DropCloseIconFile, NULL },
 { "DropCloseIconAlt", str, &DropCloseIconAlt, NULL },
 { "DropButton", str, &DropButton, NULL },
 { "DropButtonOpenLabel", str, &DropButtonOpenLabel, NULL },
 { "DropButtonCloseLabel", str, &DropButtonCloseLabel, NULL },
 { "DropClass", str, &DropClass, NULL },
 { "DropText", str, &DropIDPrefix, NULL },
 { "DropLinkAttr", str, &DropLinkAttr, NULL },
 { "DropDivAttr", str, &DropDivAttr, NULL },
 { "DropJSCode", str, &DropJSCode, NULL },
 { "DropJSLocation", str, &DropJSLocation, NULL },
 { "WriteDropJSFile", yesno, &WriteDropJSFile, NULL },
 { NULL, spec, NULL, NULL }
};


// IndexSet

setinfo HTMwr::ISecIndex[] = {
 { "IndexNavType", cap, &IndexNavType, IndexNavTypeSet },
 //{ "UseIndex", yesno, &UseIndex, NULL },
 //{ "GenerateIDX", yesno, &UseIndex, NULL },
 { "UseIndexLevelNum", yesno, &UseIndexLevelNum, NULL },
 { "UseIndexLetters", yesno, &UseIndexLetters, NULL },
 { "UseIndexTopLetters", yesno, &UseIndexTopLetters, NULL },
 { "UseIndexHeading", yesno, &UseIndexHeading, NULL },
 { "IndexLevelClass", str, &IndexLevelClass, NULL },
 { "IndexTopLettersClass", str, &IndexTopLettersClass, NULL },
 { "IndexTopLettersFormat", str, &IndexTopLettersClass, NULL },
 { "IndexLetterClass", str, &IndexLetterClass, NULL },
 { "IndexLettersFormat", str, &IndexLetterClass, NULL },
 { "IndexRefClass", str, &IndexRefClass, NULL },
 { "IndexTOCClass", str, &IndexTOCClass, NULL },
 { "IDXTOCFormat", str, &IndexTOCClass, NULL },
 { "IndexHeadingClass", str, &IndexHeadingClass, NULL },
 { "IDXTitleFormat", str, &IndexHeadingClass, NULL },
 { "IndexLetterPrefix", str, &IndexLetPrefix, NULL },
 { "IndexLetterSymbol", str, &IndexLetterSymbol, NULL },
 { "IndexLetterNumber", str, &IndexLetterNumber, NULL },
 { "IndexNumPrefix", str, &IndexNumPrefix, NULL },
 { "IndexFile", str, &IndexFileName, NULL },
 { "IDXFile", str, &IndexFileName, NULL },
 { "IndexFileSuffix", str, &IndexFileSuffix, NULL },
 { "IDXSuffix", str, &IDXSuffix, NULL },
 { "IndexFileTitle", str, &IndexFileTitle, NULL },
 { "WriteIndexCssLink", yesno, &WriteIndexCssLink, NULL },

 { "KeywordRefs", cap, &KeywordRefs, NULL },
 { "CaseSensitiveIndexCompare", yesno, &CaseSensitiveIndexCompare, NULL },
 { "UseSortString", yesno, &UseSortString, NULL },
 { "IgnoreCharsIX", str, &IgnoreCharsIX, NULL },
 { "IgnoreLeadingCharsIX", str, &IgnoreLeadingCharsIX, NULL },
 { "SeeTerm", str, &SeeTerm, NULL },
 { "SeeAlsoTerm", str, &SeeAlsoTerm, NULL },
 { "SortSeeAlsoFirst", yesno, &SortSeeAlsoFirst, NULL },
 { "CombineIndexLevels", yesno, &CombineIndexLevels, NULL },
 { "UseCommaAsSeparator", yesno, &UseCommaAsSeparator, NULL },
 { "LevelBreakForSee", yesno, &LevelBreakForSee, NULL },

 { "UseHVIndex", yesno, &UseHVIndex, NULL },

 { NULL, spec, NULL, NULL }
};

setinfo HTMwr::ISecIndexText[] = {
 { "IDXTitle", str, &IndexFileTitle, NULL },
 { "IndexRefStartSep", str, &IndexRefStartSep, NULL },
 { "IndexRefSep", str, &IndexRefSep, NULL },

 { NULL, spec, NULL, NULL }
};


void
HTMwr::IndexNavTypeSet(char *)
{
	if (IndexNavType == 'H') {
		UseIndexTopLetters = true;
	}
	else if (IndexNavType == 'J') {
		UseIndexTopLetters = false;
		UseIndexLetters = true;
	}
	else {  // CSS
		UseIndexTopLetters = false;
	}
}

// MacroSet

setinfo HTMwr::ISecInserts[] = {
 { "BeginFile", str, &BeginFileInsert, NULL, true },
 { "Entities", str, &EntitiesInsert, NULL, true },

 { "Head", str, &HeadInsert, NULL, true },
 { "HeadEnd", str, &HeadEndInsert, NULL, true },
 { "Frames", str, &FramesInsert, NULL, true },
 { "Top", str, &TopInsert, NULL, true },
 { "Bottom", str, &BottomInsert, NULL, true },
 { "End", str, &EndInsert, NULL, true },
 { "TopicBreak", str, &TopicBreak, NULL, true },

 { "IndexTop", str, &IndexTopInsert, NULL, true },
 { "IndexHead", str, &IndexHeadInsert, NULL, true },
 { "IndexBottom", str, &IndexBottomInsert, NULL, true },
 { "IndexTOC", str, &IndexTOCInsert, NULL, true },

 { "FirstHead", str, &FirstHeadInsert, NULL, true },
 { "FirstHeadEnd", str, &FirstHeadEndInsert, NULL, true },
 { "FirstFrames", str, &FirstFramesInsert, NULL, true },
 { "FirstTop", str, &FirstTopInsert, NULL, true },
 { "FirstBottom", str, &FirstBottomInsert, NULL, true },
 { "FirstEnd", str, &FirstEndInsert, NULL, true },

 { "LastHead", str, &LastHeadInsert, NULL, true },
 { "LastHeadEnd", str, &LastHeadEndInsert, NULL, true },
 { "LastFrames", str, &LastFramesInsert, NULL, true },
 { "LastTop", str, &LastTopInsert, NULL, true },
 { "LastBottom", str, &LastBottomInsert, NULL, true },
 { "LastEnd", str, &LastEndInsert, NULL, true },

 { "SplitHead", str, &SplitHeadInsert, NULL, true },
 { "SplitHeadEnd", str, &SplitHeadEndInsert, NULL, true },
 { "SplitFrames", str, &SplitFramesInsert, NULL, true },
 { "SplitTop", str, &SplitTopInsert, NULL, true },
 { "SplitBottom", str, &SplitBottomInsert, NULL, true },
 { "SplitEnd", str, &SplitEndInsert, NULL, true },

 { "NonsplitTop", str, &NonsplitTopInsert, NULL, true },
 { "NonsplitBottom", str, &NonsplitBottomInsert, NULL, true },

 { "ExtrHead", str, &ExtrHeadInsert, NULL, true },
 { "ExtrHeadEnd", str, &ExtrHeadEndInsert, NULL, true },
 { "ExtrTop", str, &ExtrTopInsert, NULL, true },
 { "ExtrBottom", str, &ExtrBottomInsert, NULL, true },

 { NULL, spec, NULL, NULL }
};


setinfo HTMwr::ISecNavMacs[] = {
 { "UseNavButtons", yesno, &UseNavButtons, UseNavButtonsSet },

 { "TopMacro", str, &TopLink, NULL },
 { "PrevMacro", str, &PrevLink, NULL },
 { "NextMacro", str, &NextLink, NULL },
 { "PrevFSMacro", str, &PrevFSLink, NULL },
 { "NextFSMacro", str, &NextFSLink, NULL },
 { "StartingPrevFSMacro", str, &StartingPrevFSLink, NULL },
 { "EndingNextFSMacro", str, &EndingNextFSLink, NULL },

 { "TopButton", str, &TopButton, NULL },
 { "PrevButton", str, &PrevButton, NULL },
 { "NextButton", str, &NextButton, NULL },
 { "PrevFSButton", str, &PrevFSButton, NULL },
 { "NextFSButton", str, &NextFSButton, NULL },
 { "StartingPrevFSButton", str, &StartingPrevFSButton, NULL },
 { "EndingNextFSButton", str, &EndingNextFSButton, NULL },

 { "StartingFSButton", str, &StartingPrevFSTitle, NULL },
 { "StartingPrevFSTitle", str, &StartingPrevFSTitle, NULL },
 { "EndingFSButton", str, &EndingNextFSTitle, NULL },
 { "EndingNextFSTitle", str, &EndingNextFSTitle, NULL },
 { "TopTitle", str, &TopTitle, NULL },
 { NULL, spec, NULL, NULL }
};

void
HTMwr::UseNavButtonsSet(char *)
{
	if (UseNavButtons) {
		TopMacro = &TopButton;
		PrevMacro = &PrevButton;
		NextMacro = &NextButton;
		PrevFSMacro = &PrevFSButton;
		NextFSMacro = &NextFSButton;
		StartingPrevFSMacro = &StartingPrevFSButton;
		EndingNextFSMacro = &EndingNextFSButton;
	}
	else {
		TopMacro = &TopLink;
    PrevMacro = &PrevLink;
    NextMacro = &NextLink;
    PrevFSMacro = &PrevFSLink;
    NextFSMacro = &NextFSLink;
    StartingPrevFSMacro = &StartingPrevFSLink;
    EndingNextFSMacro = &EndingNextFSLink;
	}
}



// BaseFontSet

setinfo HTMwr::ISecBase[] = {
 { "Font", str, &BaseFontName, NULL },
 { "Size", usnum, &BaseSize, BaseSizeSet },
 { NULL, spec, NULL, NULL }
};

void
HTMwr::BaseSizeSet(char *)
{
	if (BaseSize > 7)
		BaseSize = 7;
	else if (BaseSize < 1)
		BaseSize = 1;
}


// TypoSet

setinfo HTMwr::ISecTypo[] = {
 { "UseTypographicElements", yesno, &UseTypographicElements, NULL },
 { "UseFormatTypographics", yesno, &UseFormatTypo, NULL },
 { "UseParagraphTypographics", yesno, &UsePFormatTypo, NULL },
 { "UseCharacterTypographics", yesno, &UseCFormatTypo, NULL },

 { "UseTypographicStyles", yesno, &UseTypographicStyles, NULL },
 { "b", str, &TypoB, NULL },
 { "i", str, &TypoI, NULL },
 { "u", str, &TypoU, NULL },
 { "over", str, &TypoOline, NULL },
 { "strike", str, &TypoStrike, NULL },
 { "chbar", str, &TypoChbar, NULL },
 { "sub", str, &TypoSub, NULL },
 { "sup", str, &TypoSup, NULL },

 { NULL, spec, NULL, NULL }
};


// ConditionalOptionsSet

setinfo HTMwr::ISecCondOptions[] = {
 { "UseConditionalFlagging", yesno, &UseConditionalFlagging, NULL },
 { "CSSFlagsFile", str, &CSSFlagsFile, NULL },
 { "WriteFlagsFile", yesno, &WriteFlagsFile, NULL },
 { "ReferenceFlagsFile", yesno, &ReferenceFlagsFile, NULL },
 { "AppendFlagsFile", yesno, &AppendFlagsFile, NULL },

 { NULL, spec, NULL, NULL }
};


// DocBookSet

setinfo HTMwr::ISecDocBook[] = {
 { "DocBookRoot", str, &HXMwr::DocBookRoot, DocBookRootSet },
 { "WriteBookFile", yesno, &HXMwr::WriteBookFile, NULL },
 { "BookFileName", str, &HXMwr::BookFileName, NULL },
 { "BookFileTitle", str, &HXMwr::BookFileTitle, NULL },
 { "UseFormatAsTag", yesno, &HXMwr::UseFormatAsTag, NULL },
 { "DropInvalidParaTag", yesno, &HXMwr::DropInvalidParaTag, NULL },
 { "DefParaElem", str, &HXMwr::DefDITAPar, NULL },
 { "DefCharElem", str, &HXMwr::DefDITAChar, NULL },

 { "ImageParents", sarr, &DITAImageParents, NULL },
 { "TableParents", sarr, &HXMwr::DITATableParents, NULL },

 { "CloseFigAfterImage", yesno, &HXMwr::CloseFigAfterImage, NULL },
 { "MultiImageFigures", yesno, &MultiImageFigures, NULL },
 { "FigureTitleStartsFigure", yesno, &FigureTitleStartsFigure, NULL },

 { NULL, spec, NULL, NULL }
};

void
HTMwr::DocBookRootSet(char *)
{
	long id = HXMwr::DTopicRoots->getnum(HXMwr::DocBookRoot);
	if (id) {
		HXMwr::SetDocBookType(id);
		XMLRoot = HXMwr::DocBookRoot;
	}
	else
		HXMwr::SetDocBookType(1);
}



// DITASet

setinfo HTMwr::ISecDITA[] = {
 { "FM8Import", yesno, &HXMwr::FM8Import, FM8ImportSet },
 { "UseDTDPath", yesno, &HXMwr::UseDTDPath, NULL },
 { "DITAVer", ucnum, &HXMwr::DITAVer, NULL },
 { "DefTopic", str, &HXMwr::DefDITATopic, NULL },
 { "SpecIniDir", str, &HXMwr::SpecIniDir, NULL },
 { "TopicID", str, &HXMwr::DITATopicID, NULL },
 { "SetElementIDs", yesno, &HXMwr::SetElementIDs, NULL },
 { "UseCommonNames", yesno, &HXMwr::UseCommonNames, NULL },

 { "DITATopicIDSpaceChar", chr, &HXMwr::DITATopicIDSpaceChar, NULL },
 { "DITATopicIDUnderscore", yesno, &HXMwr::DITATopicIDUnderscore, NULL },
 { "DITATopicIDLowerCase", yesno, &HXMwr::DITATopicIDLowerCase, NULL },
 { "UseFilePxSxInBaseName", yesno, &UseFilePxSxInBaseName, NULL },

 { "MapName", str, &HXMwr::DITAMapBase, NULL },
 { "MapID", str, &HXMwr::DITAMapID, NULL },
 { "UseMapID", yesno, &HXMwr::UseDITAMapID, NULL },
 { "MapTitle", str, &HXMwr::DITAMapTitle, NULL },
 { "MapHead", str, &HXMwr::DITAMapHead, NULL },
 { "MapTopicmeta", yesno, &HXMwr::MapTopicmeta, NULL },
 { "MapTopicID", yesno, &HXMwr::MapTopicID, NULL },
 { "UseAltMapTitle", yesno, &HXMwr::UseAltMapTitle, NULL },

 { "ForceStartTopic", yesno, &HXMwr::ForceStartTopic, NULL },
 { "SplitTopicFiles", yesno, &HXMwr::SplitTopicFiles, NULL },
 { "NestTopicFiles", yesno, &HXMwr::NestTopicFiles, NULL },
 { "WrapTopicFiles", yesno, &HXMwr::WrapTopicFiles, NULL },
 { "WriteDitamaps", yesno, &HXMwr::WriteDitamaps, NULL },
 { "MapBookTopics", yesno, &HXMwr::MapBookTopics, NULL },
 { "MapBookRelTable", yesno, &HXMwr::MapBookRelTable, NULL },

 { "BookMapName", str, &HXMwr::DITABookMapName, NULL },
 { "BookMapID", str, &HXMwr::DITABookMapID, NULL },
 { "BookMapTitle", str, &HXMwr::DITABookMapTitle, NULL },
 { "BookMapHead", str, &HXMwr::DITABookMapHead, NULL },

 { "UseFormatAsTag", yesno, &HXMwr::UseFormatAsTag, NULL },
 { "DropInvalidParaTag", yesno, &HXMwr::DropInvalidParaTag, NULL },
 { "DefParaElem", str, &HXMwr::DefDITAPar, NULL },
 { "DefCharElem", str, &HXMwr::DefDITAChar, NULL },
 { "DefTableElem", str, &HXMwr::DefDITATbl, NULL },
 { "UseTableFooterClass", yesno, &HXMwr::UseTableFooterClass, NULL },
 { "TableFooterClass", str, &HXMwr::DITATableFooterClass, NULL },
 { "TableColsRelative", yesno, &TableWidthType, NULL },

 { "ImageParents", sarr, &DITAImageParents, NULL },
 { "TableParents", sarr, &HXMwr::DITATableParents, NULL },

 { "NoCondAttrs", sarr, &HXMwr::DITANoCondAttrs, NULL },

 { "CloseFigAfterImage", yesno, &HXMwr::CloseFigAfterImage, NULL },
 { "MultiImageFigures", yesno, &MultiImageFigures, NULL },
 { "FigureTitleStartsFigure", yesno, &FigureTitleStartsFigure, NULL },
 { "UseOtherpropsDpi", yesno, &UseOtherpropsDpi, NULL },

 { "MenuSep", str, &HXMwr::DITAMenuSep, NULL },

 { "UseOutputClass", yesno, &WriteClassAttributes, NULL },
 { "UseRelNameColumn", yesno, &HXMwr::DITAUseRelNameCol, NULL },
 { "KeepXrefText", yesno, &HXMwr::KeepXrefText, NULL },
 { "FootnoteXref", yesno, &FootnoteXref, NULL },
 { "UseLocalScope", yesno, &HXMwr::UseLocalScope, NULL },

 { "VariableType", cap, &DITAVarType, NULL },
 { "VariableFile", str, &DITAVarFile, NULL },
 { "VariableTopicID", str, &DITAVarID, NULL },
 { "VariableElement", str, &DITAVarElem, NULL },
 { "WriteVariableFile", yesno, &WriteVarFile, NULL },

 { "RemoveTextInsets", yesno, &RemoveTextInsets, NULL },
 { "TextInsetMark", yesno, &TextInsetMark, NULL },
 { "TextInsetNest", yesno, &TextInsetNest, NULL },

 { "XrefWrapClass", str, &HXMwr::XrefWrapClass, NULL },
 { "IndexWrapClass", str, &HXMwr::IndexWrapClass, NULL },
 { "FootnoteWrapClass", str, &HXMwr::FootnoteWrapClass, NULL },

 { "AliasPrefix", str, &AliasPrefix, NULL },
 { "UseTopicAlias", yesno, &UseDITAAlias, NULL },

 { NULL, spec, NULL, NULL }
};

void
HTMwr::FM8ImportSet(char *)
{
	if (HXMwr::FM8Import) {
		HXMwr::MapTopicmeta = false;
		HXMwr::MapTopicID = false;
		HXMwr::WrapTopicFiles = true;
		UsePtSuffix = true;
		HXMwr::DITAVer = 0;
	}
}


setinfo HTMwr::ISecDITAbookmap[] = {
 { "BookmapType", cap, &HXMwr::DITABookmapType, NULL },
 { "BookTitle", str, &HXMwr::DITABookTitle, NULL },
 { "BookSubtitle", str, &HXMwr::DITABookSubtitle, NULL },
 { "BookLibrary", str, &HXMwr::DITABookLibrary, NULL },
 { "BookMeta", str, &HXMwr::DITABookMeta, NULL },
 { "AllowPartAppendix", yesno, &HXMwr::AllowPartAppendix, NULL },

 { NULL, spec, NULL, NULL }
};


// OHSet

setinfo HTMwr::ISecOH[] = {
 { "ProjectName", str, &OHProjectName, NULL },
 { "OHRunMsg", str, &OHRunMsg, NULL },
 { "HelpFileTitle", str, &OHTitle, OHTitleSet },
 { "HelpFileLanguage", str, &HelpFileLanguage, NULL },
 { "OHProjFilePrefix", str, &OHProjFilePrefix, OHPxSet },
 { "OHProjFileSuffix", str, &OHProjFileSuffix, OHSxSet },
 { "OHProjFileXhtml", yesno, &OHProjFileXhtml, NULL },
 { "DefaultTopicFile", str, &DefaultTopicFile, NULL },

 { "ProjectTemplate", path, &ProjectTemplate, NULL },
 { "NavElems", str, &NavElems, NavElemsSet },
 { "UseNavtitleMarkers", yesno, &UseNavtitleMarkers, NULL },

 { "FrameLeft", str, &FrameLeft, NULL },
 { "FrameTop", str, &FrameTop, NULL },
 { "FrameHigh", str, &FrameHigh, NULL },
 { "FrameWide", str, &FrameWide, NULL },
 { "FrameOptions", str, &FrameOptions, NULL },
 { "TopHigh", str, &TopHigh, NULL },
 { "LeftWide", str, &LeftWide, NULL },
 { "MidHigh", str, &MidHigh, NULL },
 { "TopFirst", yesno, &TopFirst, NULL },

 { "UseTopButtons", yesno, &UseTopButtons, NULL },
 { "UseStart", yesno, &UseStart, NULL },
 { "UsePrevNext", yesno, &UsePrevNext, NULL },
 { "UseBackForward", yesno, &UseBackForward, NULL },
 { "UseHideShow", yesno, &UseHideShow, NULL },
 { "ShowNavLeft", yesno, &ShowNavLeft, NULL },
 { "NewWindow", yesno, &NewWindow, NULL },
 { "CloseOldWindow", yesno, &CloseOldWindow, NULL },
 { "PersistSettings", yesno, &PersistSettings, NULL },
 { "LowMem", yesno, &OHLowMem, NULL },

 { "CtrlCssName", str, &CtrlCssName, NULL },
 { "MainCssName", str, &MainCssName, NULL },
 { "IECtrlCssName", str, &IECtrlCssName, NULL },
 { "N6CtrlCssName", str, &N6CtrlCssName, NULL },
 { "N4CtrlCssName", str, &N4CtrlCssName, NULL },
 { "IECssName", str, &IECssName, NULL },
 { "N6CssName", str, &N6CssName, NULL },
 { "N4CssName", str, &N4CssName, NULL },

 { "TocGroupsOpen", yesno, &OHTocGroupsOpen, NULL },
 { "TocOpenLevel", str, &OHTocOpenLevel, NULL },
 { "TocIcoBase", str, &OHTocIcoBase, NULL },
 { "TocExpand", yesno, &OHTocExpand, NULL },
 { "TocButtons", yesno, &OHTocButtons, NULL },

 { "IdxGroupsOpen", yesno, &OHIdxGroupsOpen, NULL },
 { "IdxOpenLevel", str, &OHIdxOpenLevel, NULL },
 { "IdxIcoBase", str, &OHIdxIcoBase, NULL },
 { "IdxExpand", yesno, &OHIdxExpand, NULL },
 { "IdxButtons", yesno, &OHIdxButtons, NULL },

 { "KeywordRefs", cap, &KeywordRefs, NULL },
 { "CaseSensitiveIndexCompare", yesno, &CaseSensitiveIndexCompare, NULL },
 { "UseSortString", yesno, &UseSortString, NULL },
 { "IgnoreCharsIX", str, &IgnoreCharsIX, NULL },
 { "IgnoreLeadingCharsIX", str, &IgnoreLeadingCharsIX, NULL },
 { "SeeTerm", str, &SeeTerm, NULL },
 { "SeeAlsoTerm", str, &SeeAlsoTerm, NULL },
 { "SortSeeAlsoFirst", yesno, &SortSeeAlsoFirst, NULL },
 { "CombineIndexLevels", yesno, &CombineIndexLevels, NULL },
 { "UseCommaAsSeparator", yesno, &UseCommaAsSeparator, NULL },

 { "RefFileType", cap, &RefFileType, NULL },

 { "UseFTS", yesno, &UseFTS, NULL },
 { "SearchWordMin", lnum, &SearchWordMin, NULL },
 { "UseDefaultStopWords", yesno, &UseDefaultStopWords, NULL },
 { "UseListButton", yesno, &UseListButton, NULL },
 { "UseSearchHighlight", yesno, &UseSearchHighlight, NULL },
 { "SearchHighlightStyle", str, &SearchHighlightStyle, NULL },
 { "UnicodeFTS", yesno, &UnicodeFTS, NULL },
 { "UnicodeLocale", str, &UnicodeLocale, NULL },
 { "CompoundWordChars", str, &CompoundWordChars, NULL },

 // { "CshMapFile", str, &CshMapFile, NULL },
 { "AliasPrefix", str, &AliasPrefix, NULL },
 { "MergeFirst", yesno, &MergeFirst, NULL },

 { "ShowSubjects", yesno, &OHRelShowSubjects, NULL },
 { "ALinkRefs", cap, &ALinkFileRefs, NULL },
 { NULL, spec, NULL, NULL }
};

void
HTMwr::OHTitleSet(char *)
{
	if (*OHTitle == '\0')
		OHTitle = "OmniHelp";
	else
		OHTitle = FixOHString((unc *) OHTitle);
}

void
HTMwr::OHPxSet(char *)
{
	if (OHProjFilePrefix == NULL)
		OHProjFilePrefix = "";
}

void
HTMwr::OHSxSet(char *)
{
	if (OHProjFileSuffix == NULL)
		OHProjFileSuffix = "";
}

void
HTMwr::NavElemsSet(char *)
{
	if (strstr(NavElems, "Fts") != NULL)
		UseFTS = true;
}


// JavaSet

setinfo HTMwr::ISecJava[] = {
 { "HSFileName", str, &DefaultHelpSetFile, NULL },
 { "HelpFileTitle", str, &HelpFileTitle, NULL },
 { "WriteHelpSetFile", yesno, &WriteHelpSetFile, NULL },
 { "DefaultHelpSetFile", str, &DefaultHelpSetFile, NULL },
 { "HSPathNames", yesno, &HSPathNames, NULL },
 { "HTMLSubdir", str, &MapFilePrefix, HTMLSubdirSet },
 { "GraphSubdir", str, &GraphPath, GraphSubdirSet },
 { "DefaultTopic", str, &DefaultTopicFile, NULL },
 { "DefaultTopicFile", str, &DefaultTopicFile, NULL },
 { "MapFilePrefix", str, &MapFilePrefix, NULL },
 { "UseFTS", yesno, &UseFTS, NULL },
 { "RemoveInternalAnchors", yesno, &NoANames, NULL },

 { "RefFileType", cap, &RefFileType, NULL },
 { "ListType", cap, &ListType, NULL },
 { "UseIndexentryTag", yesno, &UseIndexentryTag, NULL },
 { "UseNavtitleMarkers", yesno, &UseNavtitleMarkers, NULL },

 { "KeywordRefs", cap, &KeywordRefs, NULL },
 { "CaseSensitiveIndexCompare", yesno, &CaseSensitiveIndexCompare, NULL },
 { "UseSortString", yesno, &UseSortString, NULL },
 { "IgnoreCharsIX", str, &IgnoreCharsIX, NULL },
 { "IgnoreLeadingCharsIX", str, &IgnoreLeadingCharsIX, NULL },
 { "SeeTerm", str, &SeeTerm, NULL },
 { "SeeAlsoTerm", str, &SeeAlsoTerm, NULL },
 { "SortSeeAlsoFirst", yesno, &SortSeeAlsoFirst, NULL },
 { "CombineIndexLevels", yesno, &CombineIndexLevels, NULL },
 { "UseCommaAsSeparator", yesno, &UseCommaAsSeparator, NULL },

 { "UseSubHelpSets", yesno, &UseSubHelpSets, NULL },
 { NULL, spec, NULL, NULL }
};

void
HTMwr::HTMLSubdirSet(char *)
{
	char *cpt = MapFilePrefix;
	char *opt = cpt;
	
	for ( ; *cpt; cpt++) {
		if (*cpt == '\\')
			*cpt = '/';
	}
	if (*(cpt - 1) != '/') { // add trailing slash
		MapFilePrefix = NewName(opt, strlen(opt) + 2);
		strcat(MapFilePrefix, "/");
		DeleteName(opt);
	}
}

void
HTMwr::GraphSubdirSet(char *)
{
	char *cpt = GraphPath;
	char *opt = cpt;

	for ( ; *cpt; cpt++) {
		if (*cpt == '\\')
			*cpt = '/';
	}
	if (*(cpt - 1) == '/')  // remove trailing slash
		*(cpt - 1) = '\0';

	GraphPath = NewName("../", strlen(opt) + 4);
	strcat(GraphPath, opt);  // prepend rel path
	DeleteName(opt);
}


// JHSet

setinfo HTMwr::ISecJH[] = {
 { "JHVersion2", yesno, &JHVersion2, NULL },

 { "UseGlossary", yesno, &UseGlossary, NULL },
 { "UseFavorites", yesno, &UseFavorites, NULL },
 { "GlossPrefix", str, &GlossPrefix, NULL },
 { "GlossSuffix", str, &GlossSuffix, NULL },
 { "GlossSpace", str, &GlossSpace, NULL },

 { "Helen", yesno, &Helen, NULL },

 { "PopMarkerPrefix", str, &PopMarkerPrefix, PMPxSet },
 { "SecMarkerPrefix", str, &SecMarkerPrefix, SMPxSet },

 { "PopType", cap, &JH2PopType, NULL },

 { "PopText", str, &JH2PopText, NULL },
 { "PopSize", str, &JH2PopSize, NULL },
 { "PopLocation", str, &JH2PopLocation, NULL },
 { "PopFontFamily", str, &JH2PopFontFamily, NULL },
 { "PopFontSize", str, &JH2PopFontSize, NULL },
 { "PopFontWeight", str, &JH2PopFontWeight, NULL },
 { "PopFontStyle", str, &JH2PopFontStyle, NULL },
 { "PopFontColor", str, &JH2PopFontColor, NULL },
 { "PopGraphic", str, &JH2PopGraphic, NULL },

 { "SecType", cap, &JH2SecType, NULL },

 { "SecText", str, &JH2SecText, NULL },
 { "SecSize", str, &JH2SecSize, NULL },
 { "SecLocation", str, &JH2SecLocation, NULL },
 { "SecName", str, &JH2SecName, NULL },
 { "SecFontFamily", str, &JH2SecFontFamily, NULL },
 { "SecFontSize", str, &JH2SecFontSize, NULL },
 { "SecFontWeight", str, &JH2SecFontWeight, NULL },
 { "SecFontStyle", str, &JH2SecFontStyle, NULL },
 { "SecFontColor", str, &JH2SecFontColor, NULL },
 { "SecGraphic", str, &JH2SecGraphic, NULL },
 { NULL, spec, NULL, NULL }
};

void
HTMwr::PMPxSet(char *)
{
	if (PopMarkerPrefix)
		JH2PopMarkerPrefix = PopMarkerPrefix;
}

void
HTMwr::SMPxSet(char *)
{
	if (SecMarkerPrefix)
		JH2SecMarkerPrefix = SecMarkerPrefix;
}


// OHJSet

setinfo HTMwr::ISecOHJ[] = {
 { "MakeALinkFile", yesno, &MakeALinkFile, NULL },
 { "ALinkRefs", cap, &ALinkFileRefs, NULL },
 { NULL, spec, NULL, NULL }
};

// EHSet
setinfo HTMwr::ISecEH[] = {
 { "EclipseVer", str, &EclipseVer, NULL },
 { "IncludeVersionPI", yesno, &IncludeVersionPI, NULL },
 { "TocLabel", str, &EclipseTocLabel, NULL },
 { "TocTopic", str, &EclipseTocTopic, NULL },
 { "TocFilename", str, &EclipseTocFilename, NULL },
 { "TocLinkTo", str, &EclipseTocLink, NULL },
 { "TocPrimary", yesno, &EclipseTocPrimary, NULL },
 { "TocNamesFileOnly", yesno, &EclipseTocFileOnly, NULL },
 { "TocExtradir", str, &EclipseTocExtradir, NULL },
 { "TocIdxFilePrefix", str, &TocIdxFilePrefix, NULL },
 { "IdxFilename", str, &EclipseIdxFilename, NULL },
 { "UseNavtitleMarkers", yesno, &UseNavtitleMarkers, NULL },
 { "UseIndex", yesno, &EclipseUseIndex, NULL },
 { "UseFTS", yesno, &UseFTS, NULL },
 { "UsePlugin", yesno, &EclipseUsePlugin, NULL },
 { "WritePlugin", yesno, &EclipseWritePlugin, NULL },
 { "UseManifest", yesno, &EclipseUseManifest, NULL },
 { "WriteManifest", yesno, &EclipseWriteManifest, NULL },
 { "ManifestName", str, &EclipseManifestName, NULL },
 { "UseSingleton", yesno, &EclipseUseSingleton, NULL },
 { "UseRuntime", yesno, &EclipseUseRuntime, NULL },
 { "PluginSchemaVersion", str, &EclipsePluginSchemaVersion, NULL },
 { "PluginName", str, &EclipsePlugName, NULL },
 { "PluginID", str, &EclipsePlugID, NULL },
 { "PluginVer", str, &EclipsePlugVer, NULL },
 { "PluginProvider", str, &EclipsePlugProvider, NULL },
 { "UseContext", yesno, &EclipseUseContext, NULL },
 { "WriteContext", yesno, &EclipseWriteContext, NULL },
 { "ContextFileName", str, &EclipseContextName, NULL },
 { "ContextPluginName", str, &EclipseContextPlugName, NULL },
 { "ContextDescription", yesno, &EclipseDescription, NULL },
 { "DescriptionIsFirstLabel", yesno, &DescriptionIsFirstLabel, NULL }, 
 { "ContextAnchors", yesno, &EclipseContextAnchors, NULL },
 { "RefFileType", cap, &RefFileType, NULL },
 { "ListType", cap, &ListType, NULL },
 { NULL, spec, NULL, NULL }
};


// HHSet

setinfo HTMwr::ISecHH[] = {
 { "PopFont", str, &PopFont, NULL },
 { "PopMargins", str, &PopMargins, NULL },
 { "PopColors", str, &PopColors, NULL },

 { "HHPFileName", str, &DefaultHelpProjectFile, NULL },
 { "DefaultTopicFile", str, &DefaultTopicFile, NULL },
 { "HelpFileTitle", str, &HelpFileTitle, NULL },
 { "HelpFileLanguage", str, &HelpFileLanguage, NULL },
 { "UseCodePage", yesno, &UseCodePage, UseCodePageSet },
 { "WriteHelpProjectFile", yesno, &WriteHelpProjFile, WHPSet },
 { "UseFTS", yesno, &UseFTS, NULL },
 { "ContentsNamesFileOnly", yesno, &HHContFileOnly, NULL },
 { "ContentsLocalValuePrefix", str, &HHContValPx, NULL },

 { "CshMapFile", str, &CshMapFile, NULL },
 { "MakeCshMapFile", yesno, &MakeCshMapFile, NULL },
 { "CshMapFileNumStart", ulnum, &CshMapFileNumStart, NULL },
 { "CshMapFileNumIncrement", ulnum, &CshMapFileNumIncrement, NULL },
 { "MakeAliasFile", yesno, &MakeAliasFile, NULL },
 { "UseAliasAName", yesno, &UseAliasAName, NULL },
 { "AliasTitle", yesno, &AliasTitle, NULL },
 { "AliasPrefix", str, &AliasPrefix, NULL },
 { "StoreCshFiles", yesno, &StoreCshFiles, NULL },

 { "DefaultChmFile", str, &DefaultChmFile, DefaultChmFileSet },
 { "RemoveChmFilePaths", yesno, &RemoveChmFilePaths, NULL },
 { "BinaryIndex", yesno, &BinaryIndex, NULL },
 { "BinaryTOC", yesno, &BinaryTOC, NULL },
 { "UseNavtitleMarkers", yesno, &UseNavtitleMarkers, NULL },

 { "KeywordLimit", snum, &KeywordLimit, NULL },
 { "KeywordRefs", cap, &KeywordRefs, NULL },
 { "CaseSensitiveIndexCompare", yesno, &CaseSensitiveIndexCompare, NULL },
 { "UseSortString", yesno, &UseSortString, NULL },
 { "IgnoreCharsIX", str, &IgnoreCharsIX, NULL },
 { "IgnoreLeadingCharsIX", str, &IgnoreLeadingCharsIX, NULL },
 { "SeeTerm", str, &SeeTerm, NULL },
 { "SeeAlsoTerm", str, &SeeAlsoTerm, NULL },
 { "SortSeeAlsoFirst", yesno, &SortSeeAlsoFirst, NULL },
 { "CombineIndexLevels", yesno, &CombineIndexLevels, NULL },
 { "UseCommaAsSeparator", yesno, &UseCommaAsSeparator, NULL },

 { "RefFileType", cap, &RefFileType, NULL },
 { "ListType", cap, &ListType, NULL },

 { "UseChmInLinks", yesno, &UseChmInLinks, NULL },
 { "ChmFormat", str, &ChmFormat, NULL },

 { "HHCProperties", str, &HHCProperties, NULL },
 { "HHKProperties", str, &HHKProperties, NULL },

 { "ALinkType", cap, &ALinkType, NULL },
 { "ALinkFlags", str, &ALinkFlags, NULL },
 { "ALinkEmptyTopic", str, &ALinkEmptyTopic, NULL },
 { "ALinkButtonWidth", str, &ALinkButtonWidth, NULL },
 { "ALinkButtonHeight", str, &ALinkButtonHeight, NULL },
 { "ALinkButtonText", str, &ALinkButtonText, NULL },
 { "ALinkButtonGraphic", str, &ALinkButtonGraphic, NULL },
 { "ALinkButtonIcon", str, &ALinkButtonIcon, NULL },
 { "ALinkTextFont", str, &ALinkTextFont, NULL },
 { "ALinkText", str, &ALinkText, NULL },

 { "KLinkType", cap, &KLinkType, NULL },
 { "KLinkFlags", str, &KLinkFlags, NULL },
 { "KLinkEmptyTopic", str, &KLinkEmptyTopic, NULL },
 { "KLinkButtonWidth", str, &KLinkButtonWidth, NULL },
 { "KLinkButtonHeight", str, &KLinkButtonHeight, NULL },
 { "KLinkButtonText", str, &KLinkButtonText, NULL },
 { "KLinkButtonGraphic", str, &KLinkButtonGraphic, NULL },
 { "KLinkButtonIcon", str, &KLinkButtonIcon, NULL },
 { "KLinkTextFont", str, &KLinkTextFont, NULL },
 { "KLinkText", str, &KLinkText, NULL },

 { "MSHHctrl", str, &MSHHctrl, NULL },

 { "LinkType", cap, &LinkType, LinkTypeSet },
 { "LinkFlags", str, &LinkFlags, LinkFlagsSet },
 { "LinkEmptyTopic", str, &LinkEmptyTopic, LinkEmptyTopicSet },
 { "LinkButtonWidth", str, &LinkButtonWidth, LinkButtonWidthSet },
 { "LinkButtonHeight", str, &LinkButtonHeight, LinkButtonHeightSet },
 { "LinkButtonText", str, &LinkButtonText, LinkButtonTextSet },
 { "LinkButtonGraphic", str, &LinkButtonGraphic, LinkButtonGraphicSet },
 { "LinkButtonIcon", str, &LinkButtonIcon, LinkButtonIconSet },
 { "LinkTextFont", str, &LinkTextFont, LinkTextFontSet },
 { "LinkText", str, &LinkText, LinkTextSet },
 { NULL, spec, NULL, NULL }
};

void
HTMwr::UseCodePageSet(char *)
{
	if (!UseCodePage) {
		HelpFileLanguage = NULL;
		NoEnc = false;
	}
}

void
HTMwr::WHPSet(char *)
{
	WriteHelpProjFileSet = true;
}

void
HTMwr::DefaultChmFileSet(char *)
{
	char *per = NULL;

	if (DefaultChmFile
	 && ((per = strrchr(DefaultChmFile, '.')) != NULL))
		*per = '\0';  // truncate any extension
}

void
HTMwr::LinkTypeSet(char *)
{
	if (!ALinkType)
		ALinkType = LinkType;
	if (!KLinkType)
		KLinkType = LinkType;
}

void
HTMwr::LinkFlagsSet(char *)
{
	if (!ALinkFlags)
		ALinkFlags = LinkFlags;
	if (!KLinkFlags)
		KLinkFlags = LinkFlags;
}

void
HTMwr::LinkEmptyTopicSet(char *)
{
	if (!ALinkEmptyTopic)
		ALinkEmptyTopic = LinkEmptyTopic;
	if (!KLinkEmptyTopic)
		KLinkEmptyTopic = LinkEmptyTopic;
}

void
HTMwr::LinkButtonWidthSet(char *)
{
	if (!ALinkButtonWidth)
		ALinkButtonWidth = LinkButtonWidth;
	if (!KLinkButtonWidth)
		KLinkButtonWidth = LinkButtonWidth;
}

void
HTMwr::LinkButtonHeightSet(char *)
{
	if (!ALinkButtonHeight)
		ALinkButtonHeight = LinkButtonHeight;
	if (!KLinkButtonHeight)
		KLinkButtonHeight = LinkButtonHeight;
}

void
HTMwr::LinkButtonTextSet(char *)
{
	if (!ALinkButtonText)
		ALinkButtonText = LinkButtonText;
	if (!KLinkButtonText)
		KLinkButtonText = LinkButtonText;
}

void
HTMwr::LinkButtonGraphicSet(char *)
{
	if (!ALinkButtonGraphic)
		ALinkButtonGraphic = LinkButtonGraphic;
	if (!KLinkButtonGraphic)
		KLinkButtonGraphic = LinkButtonGraphic;
}

void
HTMwr::LinkButtonIconSet(char *)
{
	if (!ALinkButtonIcon)
		ALinkButtonIcon = LinkButtonIcon;
	if (!KLinkButtonIcon)
		KLinkButtonIcon = LinkButtonIcon;
}

void
HTMwr::LinkTextFontSet(char *)
{
	if (!ALinkTextFont)
		ALinkTextFont = LinkTextFont;
	if (!KLinkTextFont)
		KLinkTextFont = LinkTextFont;
}

void
HTMwr::LinkTextSet(char *)
{
	if (!ALinkText)
		ALinkText = LinkText;
	if (!KLinkText)
		KLinkText = LinkText;
}


// HTML Styles properties

setinfo HTMwr::ISecHTMLStyles[] = {
 { "Override", val, &Curr.cpuse, NULL, 1 },
 { "Size", usnum, &Curr.fsize, NULL, 7 }, // max value
 { "NoSize", bval, &Curr.nosize, NULL, 1 },
 { "Bold", val, &Curr.bold, NULL, 2 },
 { "Ital", val, &Curr.ital, NULL, 2 },
 { "ULine", val, &Curr.line, NULL, 2 },
 { "Strike", val, &Curr.strike, NULL, 2 },
 { "Plain", val, &Curr.bold, PlainSet, 1 },
 { "Color", lnum, &Curr.colr, ColorSet },
 { "NoColor", bval, &Curr.nocolor, NULL, 1 },

 { "NoTags", bval, &Curr.notags, NULL, 1 },
 { "Comment", bval, &Curr.comment, NULL, 1 },
 { "Raw", bval, &Curr.raw, RawSet, 1 },
 { "NoPara", bval, &Curr.nopara, NULL, 1 },

 { "NoBreak", bval, &Curr.nobreak, NULL, 1 },
 { "XMLBreak", bval, &Curr.hrpara, NULL, 1 },
 { "XMLNoBreak", bval, &Curr.hrpara, NULL, 0 },

 { "NoFig", bval, &Curr.nofig, NULL, 1 },
 { "Fig", bval, &Curr.fig, NULL, 1 },
 { "Figure", bval, &Curr.fig, NULL, 1 },
 
 { "ParaLinkClass", cval, &Curr.plclass, ParaLinkClassSet, 1 },

 { "LinkSrc", bit, &Curr.macpos, MacLinkSrcSet, MacLinkSrc },
 { "KeepLink", bval, &Curr.keeplink, NULL, 1 },
 { "NoRef", bval, &Curr.noref, NULL, 1 },
 { "NoHref", bval, &Curr.nohref, NULL, 1 },
 { "NoCSS", bval, &Curr.nocss, NULL, 1 },
 { "GlossTerm", bval, &Curr.gloss, NULL, 1 },

 { NULL, spec, NULL, NULL, 0, NULL }
};

setinfo HTMwr::ISecHTMLPStyles[] = {
 { "Left", val, &Curr.algn, NULL, 1 },
 { "Center", val, &Curr.algn, NULL, 2 },
 { "Right", val, &Curr.algn, NULL, 3 },

 { "Trail", bval, &Curr.trail, NULL, 1 },
 { "CodeStore", spec, &Curr.cstore, CodeStoreSet, 1 },
 { "TextStore", spec, &Curr.tstore, TextStoreSet, 1 },
 { "FileName", bval, &Curr.flname, NULL, 1 },
 { "Meta", cval, &Curr.meta, NULL, 1, (char *) 1 },
 { "CSSReplace", bval, &Curr.cssrep, CSSReplacePSet, 1 },

 { "CodeBefore", bit, &Curr.macpos, MacBeforeSet, MacBefore },
 { "CodeAfter", bit, &Curr.macpos, MacAfterSet, MacAfter },
 { "CodeReplace", bit, &Curr.macpos, MacReplaceSet, MacReplace },
 { "Delete", bit, &Curr.macpos, NULL, MacReplace },
 { "CodeStart", bit, &Curr.macpos, MacStartSet, MacStart },
 { "CodeEnd", bit, &Curr.macpos, MacEndSet, MacEnd },
 { "CodeBeforeAnum", bit, &Curr.macpos, MacAnumBeforeSet, MacAnumBefore },
 { "CodeAfterAnum", bit, &Curr.macpos, MacAnumAfterSet, MacAnumAfter },
 { "Window", cval, &Curr.cntwin, WindowStyleSet, 1 },
 { "DropDown", bval, &Curr.ddlpar, DropDownBlockSet, 1 },
 { "DropDownLink", bval, &Curr.ddlpst, DropDownPLinkSet, 1 },
 { "DropDownBlock", bval, &Curr.ddbstb, DropDownBlockSet, 1 },
 { "DropDownStart", bval, &Curr.ddbsta, DropDownPLinkSet, 1 },
 { "DropDownEnd", bval, &Curr.ddbend, NULL, 1 },

 { "ExtrStart", bit, &Curr.extract, ExtrSet, 1 },
 { "ExtrFinish", bit, &Curr.extract, NULL, 2 },
 { "ExtrEnd", bit, &Curr.extract, NULL, 4 },
 { "ExtrEnable", bit, &Curr.extract, NULL, 8 },
 { "ExtrDisable", bit, &Curr.extract, NULL, 16 },

 { "List", ucnum, &Curr.list, NULL, ListInitMax },
 { "LFirst", bval, &Curr.lfirst, NULL, 1 },
 { "LEnd", bval, &Curr.lend, NULL, 1 },
 { "LNest", bval, &Curr.lnest, NULL, 1 },
 { "LLevel", ucnum, &Curr.llev, NULL, ListLevMax },
 { "Anum", bval, &Curr.anuse, NULL, 1 },
 { "NoAnum", bval, &Curr.anskip, NULL, 1 },
 { "ListDD", bval, &Curr.listdd, NULL, 1 },
 { "DListDD", bval, &Curr.listdd, NULL, 1 },

 { "Contents", val, &Curr.cntform, ContentsSet, 1 },
 { "Split", val, &Curr.split, SplitSet, 1 },
 { "NoSplit", bval, &Curr.nosplit, NULL, 1 },
 { "Title", val, &Curr.title, TitleStyleSet, 1 },
 { "NoContLink", bval, &Curr.noclink, NULL, 1 },

 { "Prefix", bval, &AllowPrefix, NULL, 1 },
 { "NoPrefix", cval, &Curr.pfx, NULL, 0, NULL },
 { "Suffix", bval, &AllowSuffix, NULL, 1 },
 { "NoSuffix", cval, &Curr.sfx, NULL, 0, NULL },
 { "FrameAbove", bval, &AllowPrefix, NULL, 1 },
 { "NoFrameAbove", cval, &Curr.pfx, NULL, 0, NULL },
 { "FrameBelow", bval, &AllowSuffix, NULL, 1 },
 { "NoFrameBelow", cval, &Curr.sfx, NULL, 0, NULL },

 { "Config", bval, &Curr.config, NULL, 1 },
 { "HTMConfig", bval, &Curr.config, NULL, 1 },
 { "IniPartName", bval, &Curr.ipname, NULL, 1 },
 { "FinalIniName", bval, &Curr.finame, NULL, 1 },
 { "TextFrameIsText", bval, &Curr.txframes, NULL, 1 },

 { "IgnoreWrap", bval, &Curr.ignorewrap, NULL, 1 },
 { "NoWrap", bval, &Curr.nowrap, NULL, 1 },

 { "ALink", bval, &Curr.alink, ALinkSet, 0 },
 { "ParaLink", bval, &Curr.plink, NULL, 1 },

 { "AbbrVal", cval, &Curr.attrname, AttrSet, 1, "CellAbbr" },
 { "Abbr", bval, &Curr.cellabbr, CellAbbrSet, 1 },
 { "AxisVal", cval, &Curr.attrname, AttrSet, 1, "CellAxis" },
 { "Axis", bval, &Curr.cellaxis, CellAxisSet, 1 },
 { "Summary", cval, &Curr.attrname, AttrSet, 1, "TableSummary" },
 { "TableTitle", cval, &Curr.attrname, AttrSet, 1, "TableTitle" },
 { "Alt", cval, &Curr.attrname, AttrSet, 1, "GraphAlt" },
 { "TableHead", bval, &Curr.tbhead, NULL, 1 },
 { "TableBody", bval, &Curr.tbbody, NULL, 1 },
 { "Longdesc", cval, &Curr.attrname, AttrSet, 1, "GraphLongdesc" },
 { "LinkTitle", cval, &Curr.attrname, AttrSet, 1, "LinkTitle" },
 { "LinkClass", cval, &Curr.attrname, AttrSet, 1, "LinkClass" },
 { "ColGroup", bval, &Curr.cellcolgroup, NULL, 1 },
 { "RowGroup", bval, &Curr.cellrowgroup, NULL, 1 },

 { "CellAttribute", bval, &Curr.cellattr, CellAttributeSet, 1 },
 { "RowAttribute", bval, &Curr.rowattr, RowAttributeSet, 1 },
 { "NoColID", bval, &Curr.cellnocolid, NULL, 1 },
 { "Span", bval, &Curr.cellspan, NULL, 1 },
 { "Scope", bval, &Curr.cellscope, CellScopeValSet, 1 },

 { NULL, spec, NULL, NULL, 0, NULL }
};

setinfo HTMwr::ISecHTMLCStyles[] = {
 { "CSSReplace", bval, &Curr.cssrep, CSSReplaceCSet, 1 },
 { "CodeBefore", bit, &Curr.cmacpos, MacBeforeSet, MacBefore },
 { "CodeAfter", bit, &Curr.cmacpos, MacAfterSet, MacAfter },
 { "CodeReplace", bit, &Curr.cmacpos, MacReplaceSet, MacReplace },
 { "Delete", bit, &Curr.cmacpos, NULL, MacReplace },
 { "CodeStart", bit, &Curr.cmacpos, MacStartSet, MacStart },
 { "CodeEnd", bit, &Curr.cmacpos, MacEndSet, MacEnd },
 { "DropDownLink", bval, &Curr.ddlcst, DropDownCLinkSet, 1 },
 { "GlossTitle", bval, &Curr.glosstitle, NULL, 1 },

 { NULL, spec, NULL, NULL, 0, NULL }
};

void
HTMwr::PlainSet(char *)
{
	Curr.strike = Curr.ital = Curr.line = Curr.bold;
}

void
HTMwr::ColorSet(char *)
{
	Curr.colr++;
}

void 
HTMwr::MacBeforeSet(char *)
{
	DCini::ProcIniSect("HtmlStyleCodeBefore");
	DCini::ProcIniSect("StyleCodeBefore");
	DCini::ProcIniSect("ParaStyleCodeBefore");
	DCini::ProcIniSect("CharStyleCodeBefore");
}

void 
HTMwr::MacAfterSet(char *)
{
	DCini::ProcIniSect("HtmlStyleCodeAfter");
	DCini::ProcIniSect("StyleCodeAfter");
	DCini::ProcIniSect("ParaStyleCodeAfter");
	DCini::ProcIniSect("CharStyleCodeAfter");
}

void 
HTMwr::MacReplaceSet(char *)
{
	DCini::ProcIniSect("HtmlStyleCodeReplace");
	DCini::ProcIniSect("StyleCodeReplace");
	DCini::ProcIniSect("ParaStyleCodeReplace");
	DCini::ProcIniSect("CharStyleCodeReplace");
}

void 
HTMwr::MacStartSet(char *)
{
	DCini::ProcIniSect("HtmlStyleCodeStart");
	DCini::ProcIniSect("StyleCodeStart");
	DCini::ProcIniSect("ParaStyleCodeStart");
	DCini::ProcIniSect("CharStyleCodeStart");
}

void 
HTMwr::MacEndSet(char *)
{
	DCini::ProcIniSect("HtmlStyleCodeEnd");
	DCini::ProcIniSect("StyleCodeEnd");
	DCini::ProcIniSect("ParaStyleCodeEnd");
	DCini::ProcIniSect("CharStyleCodeEnd");
}

void 
HTMwr::MacAnumBeforeSet(char *)
{
	DCini::ProcIniSect("HtmlAnumCodeBefore");
	DCini::ProcIniSect("AnumCodeBefore");
}

void 
HTMwr::MacAnumAfterSet(char *)
{
	DCini::ProcIniSect("HtmlAnumCodeAfter");
	DCini::ProcIniSect("AnumCodeAfter");
}

void 
HTMwr::MacLinkSrcSet(char *)
{
	DCini::ProcIniSect("HtmlStyleLinkSrc");
	DCini::ProcIniSect("StyleLinkSrc");
	DCini::ProcIniSect("ParaStyleLinkSrc");
	DCini::ProcIniSect("CharStyleLinkSrc");
}

void
HTMwr::ContentsSet(char *)
{
	if (MakeContents)
		Curr.contents = 1;
}

void
HTMwr::SplitSet(char *)
{
	if (!UseChunking) {
		Curr.cntform = Curr.split;
		if (MakeTrail && SplitTrail)
			Curr.trail = true;
	}
}

void
HTMwr::TitleStyleSet(char *)
{
	Curr.cntform = Curr.title;
}

void
HTMwr::WindowStyleSet(char *)
{
	DCini::ProcIniSect("StyleWindow");
}

void
HTMwr::RawSet(char *)
{
	Curr.comment = Curr.notags = Curr.raw;
}

void
HTMwr::ExtrSet(char *)
{
	Curr.cntform = 1;
	DCini::ProcIniGroup("ExtrSet");
}

void
HTMwr::ALinkSet(char *)
{
	if ((HelpType == 'M')
	 || OracleHelp
	 || OmniHelp)
		Curr.alink = true;
}

void
HTMwr::ParaLinkClassSet(char *)
{
	DCini::ProcIniSect("HtmlStyleParaLinkClass");
	DCini::ProcIniSect("StyleParaLinkClass");
	DCini::ProcIniSect("ParaStyleParaLinkClass");
	DCini::ProcIniSect("CharStyleParaLinkClass");
}

void
HTMwr::RowAttributeSet(char *)
{
	DCini::ProcIniSect("HtmlStyleRowAttribute");
	DCini::ProcIniSect("StyleRowAttribute");
}

void
HTMwr::CellAttributeSet(char *)
{
	DCini::ProcIniSect("HtmlStyleCellAttribute");
	DCini::ProcIniSect("StyleCellAttribute");
}

void
HTMwr::CellScopeValSet(char *)
{
	DCini::ProcIniSect("HtmlStyleCellScope");
	DCini::ProcIniSect("StyleCellScope");
}

void
HTMwr::CellAbbrSet(char *)
{
	DCini::ProcIniSect("HtmlStyleCellAbbr");
	DCini::ProcIniSect("StyleCellAbbr");
}

void
HTMwr::CellAxisSet(char *)
{
	DCini::ProcIniSect("HtmlStyleCellAxis");
	DCini::ProcIniSect("StyleCellAxis");
}

void
HTMwr::AttrSet(char *)
{
	Curr.attr = true;
}

void
HTMwr::CodeStoreSet(char *)
{
	Curr.cstore = NewName(Curr.stag);
}

void
HTMwr::TextStoreSet(char *)
{
	Curr.tstore = NewName(Curr.stag);
}

void
HTMwr::CSSReplacePSet(char *)
{
	DCini::ProcIniSect("HtmlStyleCSS");
	DCini::ProcIniSect("StyleCSS");
	DCini::ProcIniSect("ParaStyleCSS");
}

void
HTMwr::CSSReplaceCSet(char *)
{
	DCini::ProcIniSect("HtmlStyleCSS");
	DCini::ProcIniSect("StyleCSS");
	DCini::ProcIniSect("CharStyleCSS");
}

void
HTMwr::DropDownCLinkSet(char *)
{
	Curr.ddlcst = true;
	Curr.ddlcend = true;
}

void
HTMwr::DropDownPLinkSet(char *)
{
	Curr.ddlpst = true;
	Curr.ddlpend = true;
}

void
HTMwr::DropDownBlockSet(char *)
{
	Curr.ddbstb = true;
	Curr.ddbend = true;
}


// Table Access properties, overrides [Table]AccessMethod props

setinfo HTMwr::ISecTableAccess[] = {
 { "HCols", usnum, &HeaderCols, NULL },
 { "NoHCols", ushval, &HeaderCols, NULL, 0 },
 { "HRows", usnum, &HeaderRows, NULL },
 { "NoHRows", ushval, &HeaderRows, NULL, 0 },
 { "FRows", usnum, &FooterRows, NULL },
 { "NoFRows", ushval, &FooterRows, NULL, 0 },
 { "ColIDs", bval, &ColIDs, NULL, 1 },
 { "NoColIDs", bval, &ColIDs, NULL, 0 },
 { "RowIDs", bval, &RowIDs, NULL, 1 },
 { "NoRowIDs", bval, &RowIDs, NULL, 0 },
 { "ColSpanIDs", bval, &ColSpanIDs, NULL, 1 },
 { "NoColSpanIDs", bval, &ColSpanIDs, NULL, 0 },
 { "RowSpanIDs", bval, &RowSpanIDs, NULL, 1 },
 { "NoRowSpanIDs", bval, &RowSpanIDs, NULL, 0 },
 { "ScopeCol", bval, &ScopeCol, NULL, 1 },
 { "NoScopeCol", bval, &ScopeCol, NULL, 0 },
 { "ScopeColGroup", bval, &ScopeColGroup, ScopeColGroupSet, 1 },
 { "NoScopeColGroup", bval, &ScopeColGroup, ScopeColGroupSet, 0 },
 { "ScopeRow", bval, &ScopeRow, NULL, 1 },
 { "NoScopeRow", bval, &ScopeRow, NULL, 0 },
 { "ScopeRowGroup", bval, &ScopeRowGroup, ScopeRowGroupSet, 1 },
 { "NoScopeRowGroup", bval, &ScopeRowGroup, ScopeRowGroupSet, 0 },
 { "ColGroupIDs", bval, &ColGroupIDs, NULL, 1 },
 { "NoColGroupIDs", bval, &ColGroupIDs, NULL, 0 },
 { "RowGroupIDs", bval, &RowGroupIDs, NULL, 1 },
 { "NoRowGroupIDs", bval, &RowGroupIDs, NULL, 0 },
 { "HFBTags", bval, &HeadFootBodyTags, NULL, 1 },
 { "NoHFBTags", bval, &HeadFootBodyTags, NULL, 0 },
 { "CGElements", bval, &ColGroupElements, NULL, 1 },
 { "NoCGElements", bval, &ColGroupElements, NULL, 0 },
 { "NoAccess", spec, NULL, TblNoAccessSet },
 { "Scope", spec, NULL, TblScopeSet },
 { "IDs", spec, NULL, TblIDsSet },
 { NULL, spec, NULL, NULL }
};

void
HTMwr::ScopeColGroupSet(char *)
{
	if (ScopeColGroup)
		ColGroupElements = true;
}

void
HTMwr::ScopeRowGroupSet(char *)
{
	if (ScopeRowGroup)
		HeadFootBodyTags = true;
}

void
HTMwr::TblNoAccessSet(char *)
{
	ScopeColGroup = false;
	ScopeRowGroup = false;
	ScopeCol = false;
	ScopeRow = false;
	ColGroupIDs = false;
	RowGroupIDs = false;
	ColIDs = false;
	RowIDs = false;
	ColSpanIDs = false;
	RowSpanIDs = false;
}

void
HTMwr::TblScopeSet(char *)
{
	ScopeColGroup = true;
	ScopeRowGroup = true;
	ScopeCol = true;
	ScopeRow = true;
	ColGroupIDs = false;
	RowGroupIDs = false;
	ColIDs = false;
	RowIDs = false;
	ColSpanIDs = true;
	RowSpanIDs = true;
	HeadFootBodyTags = true;
	ColGroupElements = true;
}

void
HTMwr::TblIDsSet(char *)
{
	ScopeColGroup = false;
	ScopeRowGroup = false;
	ScopeCol = false;
	ScopeRow = false;
	ColGroupIDs = true;
	RowGroupIDs = true;
	ColIDs = true;
	RowIDs = true;
	ColSpanIDs = true;
	RowSpanIDs = true;
}


// Xref formats

setinfo HTMwr::ISecXrefStyles[] = {
 { "Text", bval, &XrefTextOnly, NULL, true },
 { "Delete", bval, &XrefDelete, NULL, true },
 { "NoRef", bval, &XrefNoRef, NULL, true },
 { "LinkSrc", bval, &XrefLinkSrc, NULL, true },
 { NULL, spec, NULL, NULL }
};


// LoggingSet

setinfo HTMwr::ILogOptions[] = {
 { "UseLog", yesno, &UseLog, NULL },
 { "ShowLog", yesno, &ShowLog, NULL },
 { "ShowUndefinedFormats", yesno, &ShowUndefinedFormats, NULL },
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

setinfo HTMwr::ISec[] = {
 { "", yesno, &, NULL },
 { "", cap, &, NULL },
 { "", str, &, NULL },
 { "", chr, &, NULL },
 { "", ucnum, &, NULL },
 { "", snum, &, NULL },
 { "", usnum, &, NULL },
 { "", lnum, &, NULL },
 { NULL, spec, NULL, NULL }
};

void
HTMwr::Set(char *)
{
}

*/


// end of dwhtmin.cpp

