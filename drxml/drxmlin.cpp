// drxmlin.cpp is the code for dcl XML ini processing, JHG
// Copyright (c) 2008 by Omni Systems, Inc.  All rights reserved.

#include "dcappl.h"
#include "dcctlg.h"
#include "dcfili.h"
#include "dctxfx.h"
#include "dcfmtr.h"
#include "dcini.h"
#include "dcelem.h"
#include "drxml.h"
#ifdef DITA
#include "expat.h"
#endif
#ifdef UDOC
#include "mxlparser.h"
#include "drxmlud.h"
#endif
#include "drxmlfl.h"


// list of all xml2*.ini sections processed in drxml.dll

secinfo XMLrd::XMLIniSects[] = {
 { "Options", set, "OptionsSet", ISecOptions, true },
 { "Chunking", set, NULL, ISecChunking, true },
 { "MapOptions", set, NULL, ISecMapOptions, true },
 { "MapGeneration", set, NULL, ISecMapGen, true },
 { "Setup", set, NULL, ISecSetup, true },
 { "Logging", set, "LoggingSet", ILogOptions, true },
 { "Footnotes", set, NULL, ISecFootnotes, true },
 { "IDOptions", set, "ElementSet", ISecIDOptions, true },
 { "ConditionOptions", set, "ElementSet", ISecCondOptions, true },
 { "DynamicFiltering", set, "FilterConditionsSet", ISecFilterConditions, true },
 { "RelatedLinks", set, NULL, ISecRelTopics, true },
 { "ElementOptions", set, "ElementSet", ISecElements, true },
 { "FigureOptions", set, "FigureSet", ISecFigure, true },
 { "TableOptions", set, "TableSet", ISecTable, true },
 { "KeyOptions", set, "KeySet", ISecKey, true },

 { "Contents", set, "GenListSet", ISecTOC, true },
 { "ListOfFigures", set, "GenListSet", ISecLOF, true },
 { "ListOfTables", set, "GenListSet", ISecLOT, true },
 { "Index", set, "GenListSet", ISecIDX, true },
 { "Glossary", set, "GenListSet", ISecGLS, true },
 { "Abbreviations", set, "GenListSet", ISecABR, true },
 { "Trademarks", set, "GenListSet", ISecTMK, true },
 { "TopicHeads", set, "GenListSet", ISecTopicHeads, true },

 { "MapText", set, NULL, ISecMapText, true },
 { "TopicHeadText", set, "GenListSet", ISecTopicHeadText, true },
 { "TitlePsgeText", set, "GenListSet", ISecTitlePgText, true },
 { "ContentsText", set, "GenListSet", ISecTOCText, true },
 { "ListOfFiguresText", set, "GenListSet", ISecLOFText, true },
 { "ListOfTablesText", set, "GenListSet", ISecLOTText, true },
 { "IndexText", set, "GenListSet", ISecIDXText, true },
 { "GlossaryText", set, "GenListSet", ISecGLSText, true },

 { "IndexSeeFormats", set, NULL, ISecIndexSee, true },
 { "RelatedLinksText", set, NULL, ISecRelTopicsText, true },
 { "ElementText", set, "ElementSet", ISecElementText, true },

 { "ElementTypes", pcstyle, NULL, NULL, false, sarr, &CurrElemType },

 { "WordOptions", set, "OptionsSet", ISecWordOptions, false },
 { "WordOptions", set, "OptionsSet", ISecOptions, false },
 { "HTMLOptions", set, "OptionsSet", ISecHTMLOptions, false },
 { "HTMLOptions", set, "OptionsSet", ISecOptions, false },
 { "HelpOptions", set, "OptionsSet", ISecOptions, false },

 { NULL, unknown, NULL, NULL, false }
};


// OptionsSet

setinfo XMLrd::ISecOptions[] = {
 { "CaselessMatch", yesno, &CaselessMatch, CaselessMatchSet },
 { "SpacelessMatch", yesno, &SpacelessMatch, SpacelessMatchSet },
 { "WildcardMatch", yesno, &WildcardMatch, WildcardMatchSet },

 { "WriteAnums", yesno, &WriteAnums, NULL },
 { "SetAnumTabs", yesno, &SetAnumTabs, NULL },
 { "ProcessConrefs", yesno, &ProcConrefs, NULL },
 { "UseCodeblockPI", yesno, &UseCodeblockPI, NULL },
 { "UseFullPath", yesno, &UseFullPath, NULL },

 { "UsePartFiles", yesno, &UsePartFiles, NULL },
 { "PartFileLevel", str, &PartFileLevel, NULL },

#ifdef UDOC
 { "StdLibPath", str, &XMLud::StdLibPath, NULL },
 { "UseDescInText", str, &XMLud::UseDescInText, NULL },
#endif

 { NULL, spec, NULL, NULL }
};


void
XMLrd::CaselessMatchSet(char *)
{
	IniFile->SetMatch(1, CaselessMatch);
}

void
XMLrd::SpacelessMatchSet(char *)
{
	IniFile->SetMatch(2, SpacelessMatch);
}

void
XMLrd::WildcardMatchSet(char *)
{
	IniFile->SetMatch(3, WildcardMatch);
}


// ChunkingSet

setinfo XMLrd::ISecChunking[] = {
 { "SplitTopicFiles", yesno, &SplitTopicFiles, NULL },
 { "DisableChunking", yesno, &DisableChunking, NULL },
 { "ChunkBy", str, &ChunkBy, NULL },
 { "ChunkSel", str, &ChunkSel, NULL },

 { NULL, spec, NULL, NULL }
};


// FigureSet

setinfo XMLrd::ISecFigure[] = {
 { "FigTitleAboveImage", yesno, &FigTitleAboveImage, NULL },
 { "FigTitleAboveTable", yesno, &FigTitleAboveTable, NULL },
 { "TreatTableFigAsTable", yesno, &TreatTableFigAsTable, NULL },
 { "UseFigureAnchor", yesno, &UseFigureAnchor, NULL },
 { "FigureAnchorFormat", str, &FigureAnchorFormat, NULL },
 { "GraphicAlignment", str, &GraphicAlignment, NULL },

 { NULL, spec, NULL, NULL }
};


// TableSet

setinfo XMLrd::ISecTable[] = {
 { "NormalTableFormat", str, &NormalTableFormat, NULL },
 { "SimpleTableFormat", str, &SimpleTableFormat, NULL },
 { "ChoiceTableFormat", str, &ChoiceTableFormat, NULL },
 { "PropertiesTableFormat", str, &PropertiesTableFormat, NULL },
 { "TableFooterClass", str, &TableFooterClass, NULL },

 { "UseTableAnchor", yesno, &UseTableAnchor, NULL },
 { "TableAnchorFormat", str, &TableAnchorFormat, NULL },

 { "DefinitionListTables", yesno, &DefinitionListTables, NULL },
 { "DefListTableFormat", str, &DefListTableFormat, NULL },
 { "DefListTableColWidths", str, &DefListTableColWidths, NULL },
 { "ParameterListTables", yesno, &ParameterListTables, NULL },
 { "ParamListTableFormat", str, &ParamListTableFormat, NULL },
 { "ParamListTableColWidths", str, &ParamListTableColWidths, NULL },

 { NULL, spec, NULL, NULL }
};


// KeySets

setinfo XMLrd::ISecKey[] = {
 { "UseBranchKeydefs", yesno, &UseBranchKeys, NULL },
 { "KeydefsOnlyWithinBranch", yesno, &KeydefsOnlyWithinBranch, NULL },
 { NULL, spec, NULL, NULL }
};


// GenListSets

setinfo XMLrd::ISecTOC[] = {
// { "UseContents", yesno, &UseContents, NULL },
// { "GenerateTOC", yesno, &UseContents, NULL },
 { "TOCFile", str, &TOCFile, NULL },
 { "TOCSuffix", str, &TOCSuffix, NULL },
 { "TOCFormat", str, &TOCFormat, NULL },
 { "TOCXrefFormat", str, &TOCXrefFormat, NULL },
// { "TOCLinkFormat", str, &TOCLinkFormat, NULL },
 { "TOCTitleFormat", str, &TOCTitleFormat, NULL },
 { "KeepTOCWithTitlePage", yesno, &KeepTOCWithTitlePage, NULL },
 { "UseTOCDescriptions", yesno, &UseMapDescriptions, NULL },
 { "UseNestedTopicsInTOC", yesno, &UseNestedTopicsInTOC, NULL },

 { NULL, spec, NULL, NULL }
};


setinfo XMLrd::ISecLOF[] = {
// { "GenerateLOF", yesno, &GenerateLOF, NULL },
 { "LOFFile", str, &LOFFile, NULL },
 { "LOFSuffix", str, &LOFSuffix, NULL },
 { "LOFFormat", str, &LOFFormat, NULL },
 { "LOFXrefFormat", str, &LOFXrefFormat, NULL },
 { "LOFTitleFormat", str, &LOFTitleFormat, NULL },
 { "LOFTOCFormat", str, &LOFTOCFormat, NULL },

 { NULL, spec, NULL, NULL }
};


setinfo XMLrd::ISecLOT[] = {
// { "GenerateLOT", yesno, &GenerateLOT, NULL },
 { "LOTFile", str, &LOTFile, NULL },
 { "LOTSuffix", str, &LOTSuffix, NULL },
 { "LOTFormat", str, &LOTFormat, NULL },
 { "LOTXrefFormat", str, &LOTXrefFormat, NULL },
 { "LOTTitleFormat", str, &LOTTitleFormat, NULL },
 { "LOTTOCFormat", str, &LOTTOCFormat, NULL },

 { NULL, spec, NULL, NULL }
};


setinfo XMLrd::ISecIDX[] = {
 { "MakeDefaultIDX", yesno, &MakeDefaultIDX, NULL },
 { "IDXFile", str, &IDXFile, NULL },
 { "IDXSuffix", str, &IDXSuffix, NULL },
 { "IDXFormat", str, &IDXFormat, NULL },
 { "IDXXrefFormat", str, &IDXXrefFormat, NULL },
 { "IDXTOCClass", str, &IDXTOCFormat, NULL },
 { "IDXTOCFormat", str, &IDXTOCFormat, NULL },
 { "IDXTitleFormat", str, &IDXTitleFormat, NULL },
 { "UseCompactForm ", yesno, &UseCompactForm , UseCompactFormSet  },
 { "IndexRefIsPara", yesno, &IndexRefIsPara, NULL },
 { "IndexRefsWithEntry", yesno, &IndexRefsWithEntry, NULL },
 { "FullIndexRanges", yesno, &IndexRangeStartOnly, FullIndexRangesSet },
 { "IndexRangeStartOnly", yesno, &IndexRangeStartOnly, NULL },
 { "UseIndexHeading", yesno, &UseIndexHeading, NULL },
 { "UseIndexLeader", yesno, &UseIndexLeader, NULL },
 { "UseIndexLetters", yesno, &UseIndexLetters, NULL },
 { "UseIndexTopLetters", yesno, &UseIndexTopLetters, NULL },
 { "IgnoreCharsIX", str, &IgnoreCharsIX, NULL },
 { "IgnoreLeadingCharsIX", str, &IgnoreLeadingCharsIX, NULL },
 { "SortSeeAlsoFirst", yesno, &SortSeeAlsoFirst, NULL },

 { NULL, spec, NULL, NULL }
};

void
XMLrd::UseCompactFormSet(char *)
{
	if (UseCompactForm) {
		IndexRefIsPara = false;
		IndexRefsWithEntry = true;
		IndexRangeStartOnly = false;
	}
	else {
		IndexRefIsPara = true;
		IndexRefsWithEntry = false;
		IndexRangeStartOnly = true;
	}
}

void
XMLrd::FullIndexRangesSet(char *)
{
	// reverse meaning
	IndexRangeStartOnly = !IndexRangeStartOnly;
}


setinfo XMLrd::ISecGLS[] = {
// { "GenerateGLS", yesno, &GenerateGLS, NULL },
 { "GLSFile", str, &GLSFile, NULL },
 { "GLSSuffix", str, &GLSSuffix, NULL },
 { "GLSFormat", str, &GLSFormat, NULL },
 { "GLSXrefFormat", str, &GLSXrefFormat, NULL },
 { "GLSTitleFormat", str, &GLSTitleFormat, NULL },
 { "GLSTOCFormat", str, &GLSTOCFormat, NULL },

 { "ResetAbbrevAt", str, &GlossResetElem, NULL },
 { "UseAbbrevInTitles", yesno, &AcronymInTitle, NULL },

 { NULL, spec, NULL, NULL }
};

setinfo XMLrd::ISecABR[] = {
// { "GenerateABR", yesno, &GenerateABR, NULL },
 { "ABRFile", str, &ABRFile, NULL },
 { "ABRSuffix", str, &ABRSuffix, NULL },
 { "ABRFormat", str, &ABRFormat, NULL },
 { "ABRXrefFormat", str, &ABRXrefFormat, NULL },
 { "ABRTitleFormat", str, &ABRTitleFormat, NULL },
 { "ABRTOCFormat", str, &ABRTOCFormat, NULL },

 { "ResetAbbrevAt", str, &GlossResetElem, NULL },
 { "UseAbbrevInTitles", yesno, &AcronymInTitle, NULL },

 { NULL, spec, NULL, NULL }
};

setinfo XMLrd::ISecTMK[] = {
// { "GenerateTMK", yesno, &GenerateTMK, NULL },
 { "TMKFile", str, &TMKFile, NULL },
 { "TMKSuffix", str, &TMKSuffix, NULL },
 { "TMKFormat", str, &TMKFormat, NULL },
 { "TMKXrefFormat", str, &TMKXrefFormat, NULL },
 { "TMKTitleFormat", str, &TMKTitleFormat, NULL },
 { "TMKTOCFormat", str, &TMKTOCFormat, NULL },
 { "ResetTmarkAt", str, &TmarkResetElem, NULL },

 { NULL, spec, NULL, NULL }
};


// TopicHeadsSet

setinfo XMLrd::ISecTopicHeads[] = {
 { "TreatTopicheadsAsTopics", yesno, &TreatTopicheadsAsTopics, NULL },
 { "TopicheadsHaveNavtitles", yesno, &TopicheadsHaveNavtitles, NULL },
 { "LockAllNavtitles", yesno, &LockAllNavtitles, NULL },
 { "UseAllInTOC", yesno, &UseAllInTOC, NULL },
 { "TitleOnlyTopicType", str, &TitleOnlyTopicType, NULL },
 { "TitleOnlyTopicID", str, &TitleOnlyTopicID, NULL },
 { "AddTopicHeadChildren", yesno, &AddTopicHeadChildren, NULL },
 { "TopicHeadChildHeadFormat", str, &TopicHeadChildHeadFormat, NULL },

 { NULL, spec, NULL, NULL }
};


// MapOptionsSet

setinfo XMLrd::ISecMapOptions[] = {
 { "IncludeMapsInOutput", yesno, &UseContents, NULL },
 { "UseMapDescriptions", yesno, &UseMapDescriptions, NULL },
 { "TreatTopicheadsAsTopics", yesno, &TreatTopicheadsAsTopics, NULL },
 { "TopicheadsHaveNavtitles", yesno, &TopicheadsHaveNavtitles, NULL },
 { "LockAllNavtitles", yesno, &LockAllNavtitles, NULL },
 { "UseAllInTOC", yesno, &UseAllInTOC, NULL },
 { "TitleOnlyTopicType", str, &TitleOnlyTopicType, NULL },
 { "TitleOnlyTopicID", str, &TitleOnlyTopicID, NULL },
 { "AddTopicHeadChildren", yesno, &AddTopicHeadChildren, NULL },
 { "TopicHeadChildHeadFormat", str, &TopicHeadChildHeadFormat, NULL },

 { "MapXrefFormat", str, &TOCXrefFormat, NULL },
// { "MapLinkFormat", str, &TOCLinkFormat, NULL },

 { "SplitAllDocFiles", yesno, &SplitTopicFiles, NULL },

 { NULL, spec, NULL, NULL }
};


// MapGenerationSet

setinfo XMLrd::ISecMapGen[] = {
 { "GenerateMapIfMissing", yesno, &GenerateMapIfMissing, NULL },

 { NULL, spec, NULL, NULL }
};



// WordOptionsSet

setinfo XMLrd::ISecWordOptions[] = {
 { "Template", str, &WordTemplate, NULL },
 { "XrefFileSuffix", str, &XrefFileSuffix, XrefFileSuffixSet },
 { "WriteAnums", yesno, &WriteAnums, NULL },
 { "SeqAnums", yesno, &SeqAnums, NULL },
 { "AnumTabWidth", lnum, &AnumTabWidth, NULL },

 { NULL, spec, NULL, NULL }
};



// HTMLOptionsSet

setinfo XMLrd::ISecHTMLOptions[] = {
 { "FileSuffix", str, &XrefFileSuffix, XrefFileSuffixSet },
 { "UseAnums", yesno, &WriteAnums, NULL },

 { NULL, spec, NULL, NULL }
};


// SetupSet

setinfo XMLrd::ISecSetup[] = {
 { "IDFileName", path, &IDFileName, NULL },
 { "PrjFileName", path, &PrjFileName, NULL },
 { "MapFileName", path, &PrjFileName, NULL },
 { "ConfigTemplate", path, &ConfigTemplate, NULL },
 { "LanguageText", path, &LanguageText, NULL },
 { "DTDPath", path, &DTDPath, NULL },
 { "FileSuffix", str, &XrefFileSuffix, XrefFileSuffixSet },
 { "PrintProject", yesno, &PrintProject, NULL },

 { NULL, spec, NULL, NULL }
};


void
XMLrd::XrefFileSuffixSet(char *)
{
	if (*XrefFileSuffix == '.')
		XrefFileSuffix++;
}


// LoggingSet

setinfo XMLrd::ILogOptions[] = {
 { "UseLog", yesno, &UseLog, NULL },
 { "ShowLog", yesno, &ShowLog, NULL },
 { "LogIniChains", yesno, &LogIniChains, NULL },
 { "LogFileName", path, &LogFileName, NULL },
 { "HistoryFileName", path, &HistoryFileName, NULL },
 { "EditorFileName", path, &EditorFileName, NULL },
 { "LogErrors", ucnum, &LogLevels[logerr], NULL },
 { "LogWarnings", ucnum, &LogLevels[logwarn], NULL },
 { "LogQuerys", ucnum, &LogLevels[logquery], NULL },
 { "LogInfo", ucnum, &LogLevels[loginfo], NULL },
 { "LogDebug", ucnum, &LogLevels[logdebug], NULL },

 { NULL, spec, NULL, NULL }
};


// IndexSeeSet

setinfo XMLrd::ISecIndexSee[] = {
 { "IndexSeeStart", str, &IndexSeeStart, NULL },
 { "IndexSeeAlsoStart", str, &IndexSeeAlsoStart, NULL },
 { "IndexSeeEnd", str, &IndexSeeEnd, NULL },
 { "IndexSeeAlsoEnd", str, &IndexSeeAlsoEnd, NULL },

 { NULL, spec, NULL, NULL }
};


// FootnoteSet

setinfo XMLrd::ISecFootnotes[] = {
 { "UseFtnCalloutAttribute", yesno, &UseFtnCalloutAttribute, NULL },
 { "FootnoteFormat", str, &FootnoteFormat, NULL },
 { "TblFootFormat", str, &TblFootFormat, NULL },

 { NULL, spec, NULL, NULL }
};


// ConditionalOptionsSet

setinfo XMLrd::ISecCondOptions[] = {
#ifdef DITA
 { "DitavalFile", path, &DitavalFile, NULL },
 { "UseMatchingDitaval", yesno, &UseMatchingDitaval, NULL },
 { "ComplexOtherprops", yesno, &ComplexOtherprops, NULL },
 { "UseConditionalFlagging", yesno, &UseConditionalFlagging, NULL },
 { "CSSFlagsFile", str, &CSSFlagsFile, NULL },
 { "WriteFlagsFile", yesno, &WriteFlagsFile, NULL },
 { "ReferenceFlagsFile", yesno, &ReferenceFlagsFile, NULL },
#endif

 { NULL, spec, NULL, NULL }
};


// FilterConditionsSet

setinfo XMLrd::ISecFilterConditions[] = {
 { "FilterConditions", yesno, &FilterConditions, NULL },

 { NULL, spec, NULL, NULL }
};



// RelatedTopicsSet


setinfo XMLrd::ISecRelTopics[] = {
 { "AppendLinksToTopics", yesno, &AppendLinksToTopics, NULL },
 { "GenerateALinks", yesno, &GenerateALinks, NULL },
 { "AddContentsLink", yesno, &AddContentsLink, NULL },
 { "AddLOFLink", yesno, &AddLOFLink, NULL },
 { "AddLOTLink", yesno, &AddLOTLink, NULL },
 { "AddGlossaryLink", yesno, &AddGlossaryLink, NULL },
 { "AddIndexLink", yesno, &AddIndexLink, NULL },
 { "AddedLinksFormat", str, &AddedLinksFormat, NULL },
 { "AddedLinksSpacer", str, &AddedLinksSpacer, NULL },
 { "AddedLinksStart", str, &AddedLinksStart, NULL },
 { "AddedLinksEnd", str, &AddedLinksEnd, NULL },
 { "UseAddedDivider", yesno, &UseAddedDivider, NULL },
 { "AddedDividerFormat", str, &AddedDividerFormat, NULL },
 { "GeneratePrevNext", yesno, &GeneratePrevNext, NULL },
 { "GenerateParentChild", yesno, &GenerateParentChild, NULL },
 { "GenerateSiblings", yesno, &GenerateSiblings, NULL },
 { "UsePrevNext", yesno, &UsePrevNext, NULL },
 { "UseParent", yesno, &UseParent, NULL },
 { "UseChildren", yesno, &UseChildren, NULL },
 { "UseSiblings", yesno, &UseSiblings, NULL },
 { "UseAncestors", yesno, &UseAncestors, NULL },
 { "UseDescendants", yesno, &UseDescendants, NULL },
 { "UseCousins", yesno, &UseCousins, NULL },
 { "UseFriends", yesno, &UseFriends, NULL },
 { "UseTopicTypes", yesno, &UseTopicTypes, NULL },
 { "UseRelDescription", yesno, &UseRelDescription, NULL },
 { "UseRelDescAsTitle", yesno, &UseRelDescAsTitle, NULL },
 { "HeadInlineFormat", str, &HeadInlineFormat, NULL },
 { "HeadBlockFormat", str, &HeadBlockFormat, NULL },
 { "TopicTitleFormat", str, &TopicTitleFormat, NULL },
 { "ShortdescFormat", str, &ShortdescFormat, NULL },
 { "UseRelatedDivider", yesno, &UseRelatedDivider, NULL },
 { "SearchRelatedLinks", yesno, &SearchRelatedLinks, NULL },
 { "RelatedDividerFormat", str, &RelatedDividerFormat, NULL },

 { NULL, spec, NULL, NULL }
};


// ElementsSet

setinfo XMLrd::ISecElements[] = {
 { "UseOutputClassForFormat", yesno, &UseOutclassForFormat, NULL },
 { "UseClassForFormat", yesno, &UseOutclassForFormat, NULL },
 { "UseElementNameForFormat", yesno, &UseElemNameForFormat, NULL },
 { "DefaultInlineFormat", str, &DefInlineFormat, NULL },
 { "DefaultBlockFormat", str, &DefBlockFormat, NULL },
 { "BaseMapFormat", str, &BaseMapFormat, NULL },

 { "LinkFormat", str, &LinkFormat, NULL },
 { "XrefTextFormat", str, &XrefTextFormat, NULL },
 { "XrefTitleFormat", str, &XrefTitleFormat, NULL },
 { "XrefNumFormat", str, &XrefNumFormat, NULL },
 { "XrefFtnFormat", str, &XrefFtnFormat, NULL },
 { "GenListXrefFormat", str, &GenListXrefFormat, NULL },
 { "UseTitleForXrefsWithSuppliedText", yesno, &TitleXref, NULL },

 { "IncludeElementTags", yesno, &IncludeElementTags, NULL },
 { "ShowElementPath", yesno, &ShowElementPath, NULL },
 { "DisplayElementPath", yesno, &DisplayElementPath, NULL },
 { "ElementPathFormat", str, &ElementPathFormat, NULL },

 { "KeepPrologText", yesno, &KeepPrologText, NULL },
 { "KeepDraftComments", yesno, &KeepDraftComments, NULL },
 { "KeepComments", yesno, &KeepDraftComments, NULL },
 { "ContinuedFormatSuffix", str, &ContinuedFormatSuffix, NULL },
 { "DefaultNoteType", str, &DefaultNoteType, NULL },

 { "StepsHeadFormat", str, &StepsHeadFormat, NULL },

 { "OutputclassHasBorderShadeFormats", yesno, &OutputclassHasBorderShadeFormats, NULL },
 { "ClassHasBorderShadeFormats", yesno, &OutputclassHasBorderShadeFormats, NULL },

 { NULL, spec, NULL, NULL }
};



// IDSet

setinfo XMLrd::ISecIDOptions[] = {
 { "GenerateUIDs", yesno, &GenerateUIDs, NULL },
 { "UseLinkID", yesno, &UseLinkID, NULL },
 { "IDPathSep", str, &IDPathSep, NULL },
 { "IDUpDir", str, &IDUpDir, NULL },
 { "IDTopSep", str, &IDTopSep, NULL },
 { "IDElemSep", str, &IDElemSep, NULL },
 { "IDPath", yesno, &IDPath, NULL },
 { "IDFile", yesno, &IDFile, NULL },
 { "IDTopic", yesno, &IDTopic, NULL },
 { "DuplicateNameCheck", yesno, &DuplicateNameCheck, NULL },
 { "UniqueNameSuffixFormat", str, &UniqueNameSuffixFormat, NULL },
 { "UniqueNameSuffixLength", lnum, &UniqueNameSuffixLength, NULL },

 { NULL, spec, NULL, NULL }
};



// LanguageText Ini

secinfo XMLrd::LangIniSects[] = {
 { "Setup", set, NULL, ISecSetup, true },
 { "MapText", set, NULL, ISecMapText, true },
 { "TopicHeadText", set, "GenListSet", ISecTopicHeadText, true },
 { "TitlePageText", set, "GenListSet", ISecTitlePgText, true },
 { "ContentsText", set, "GenListSet", ISecTOCText, true },
 { "ListOfFiguresText", set, "GenListSet", ISecLOFText, true },
 { "ListOfTablesText", set, "GenListSet", ISecLOTText, true },
 { "IndexText", set, "GenListSet", ISecIDXText, true },
 { "GlossaryText", set, "GenListSet", ISecGLSText, true },
 { "RelatedLinksText", set, NULL, ISecRelTopicsText, true },
 { "ElementText", set, "ElementSet", ISecElementText, true },

 { NULL, unknown, NULL, NULL, false }
};


// TopicHeadTextSet

setinfo XMLrd::ISecTopicHeadText[] = {
 { "TopicHeadChildHead", str, &TopicHeadChildHead, NULL },

 { NULL, spec, NULL, NULL }
};


// GenListTextSets

setinfo XMLrd::ISecTitlePgText[] = {
 { "TitlePageFile", str, &TitlePageFile, NULL },
 { "TitlePageTemplate", str, &TitlePageTemplate, NULL },

 { NULL, spec, NULL, NULL }
};


setinfo XMLrd::ISecTOCText[] = {
 { "TOCTitle", str, &TOCTitle, NULL },

 { NULL, spec, NULL, NULL }
};

setinfo XMLrd::ISecLOFText[] = {
 { "LOFTitle", str, &LOFTitle, NULL },

 { NULL, spec, NULL, NULL }
};

setinfo XMLrd::ISecLOTText[] = {
 { "LOTTitle", str, &LOTTitle, NULL },

 { NULL, spec, NULL, NULL }
};

setinfo XMLrd::ISecIDXText[] = {
 { "IDXTitle", str, &IDXTitle, NULL },
 { "IndexRefSep", str, &IndexRefSep, NULL },
 { "IndexRefStartSep", str, &IndexRefStartSep, NULL },
 { "IndexRangeSep", str, &IndexRangeSep, NULL },

 { NULL, spec, NULL, NULL }
};

setinfo XMLrd::ISecGLSText[] = {
 { "GLSTitle", str, &GLSTitle, NULL },

 { NULL, spec, NULL, NULL }
};

// RelatedTopicsText Set

setinfo XMLrd::ISecRelTopicsText[] = {
 { "PrevHead", str, &PrevHead, NULL },
 { "NextHead", str, &NextHead, NULL },
 { "ParentHead", str, &ParentHead, NULL },
 { "ChildHead", str, &ChildHead, NULL },
 { "SiblingHead", str, &SiblingHead, NULL },
 { "AncestorHead", str, &AncestorHead, NULL },
 { "DescendantHead", str, &DescendantHead, NULL },
 { "CousinHead", str, &CousinHead, NULL },
 { "FriendHead", str, &FriendHead, NULL },
 { "RelatedHead", str, &RelatedHead, NULL },
 { "ConceptsHead", str, &ConceptsHead, NULL },
 { "TasksHead", str, &TasksHead, NULL },
 { "ReferencesHead", str, &ReferencesHead, NULL },
 { "RelatedDividerText", str, &RelatedDividerText, NULL },
 { "AddedDividerText", str, &AddedDividerText, NULL },

 { NULL, spec, NULL, NULL }
};


// ElementTextSet

setinfo XMLrd::ISecElementText[] = {
 { "Quotes", str, &Quotes, NULL },
 { "CascadeSeparator", str, &CascadeSeparator, NULL },

 //{ "BorderType", str, &BorderType, NULL },
 //{ "ShadeType", str, &ShadeType, NULL },

 { NULL, spec, NULL, NULL }
};


// MapTextSet

setinfo XMLrd::ISecMapText[] = {
 { "TOCFileName", str, &TOCFile, NULL },
 { "TopicHeadChildHead", str, &TopicHeadChildHead, NULL },

 { NULL, spec, NULL, NULL }
};



// end of drxmlin.cpp

