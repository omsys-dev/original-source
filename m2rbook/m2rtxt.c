/* m2rtxt.c is for FDK text props for mif2rtf FM 5.5 export 
 * Last edited on 1/10/99 by Jeremy H. Griffith.
 * Copyright 1998, 1999 Omni Systems, Inc.  All Rights Reserved.
 */


#include "m2rbook.h"


/* globals */

TextPropT CurrDesText;
TextPropT WDefDesText = {  // Word
	False, 
	"144", "240", "Times New Roman", "220",
	"Arial", "180", False, "", False,
	False, True, False, False, True, False, True, 1,
	3, 2, True, "0", 0,
	False, True, True, "0", 0,
	0, "100", True, True, "0",
	0, "100", "0", 0, 0, False, 
	False, "Table Continuation", False, "Table Sheet",
	0, 0, 1, 0, 0, True, True, False, True,
	False, False, False, False, False,
	"", "", "", "", False, "", False, "", "",
	0, False, False, ".rtf", ".rtf", 1, False, False, False,
	2, False, "", False, "", False, "", False, "", False, "", False, "",
	False, False, "",
	False, False, "", "", "", "", "", False,
	False, "wordstys.fm", 0, False, "normal.dot", True,
	0, "12", 0, 1, "13", False,
	True, True, True, False, 1, "", "rtf"
};

TextPropT HDefDesText = {  // WinHelp
	True, 
	"144", "240", "Times New Roman", "220",
	"Arial", "180", False, "Algerian", True,
	False, False, False, False, False, False, True, 0,
	3, 0, False, "0", 3, 
	False, False, False, "0", 2,
	2, "0", False, False, "0",
	1, "100", "0", 1, 0, False, 
	False, "Table Continuation", False, "Table Sheet",
	1, 1, 1, 1, 0, True, True, True, False,
	False, False, False, False, True,
	"3", "1", "1", "HLP", True, "bullet.bmp", False, "64", "32",
	1, False, False, ".rtf", ".rtf", 1, False, False, False,
	0, False, "myfile.cnt", False, "myfile.hlp", False, "Project Name",
	False, "myfile.bct", False, "1 Book Title", True, "Overview",
	False, False, "main",
	True, False, "myfile.hpj", "myfile.hlp", "Project Name", "myfile",
	"Copyright (c) 1999 Omni Systems, Inc.  All rights reserved.", True,
	False, "helpstys.fm", 0, False, "winhelp.dot", False,
	0, "12", 1, 1, "13", False,
	True, True, True, False, 1, "unknown", "hlp"
};

TextPropT DefDesText;
TextPropT ClipboardText;
BoolT TextChanged = False;

StringT WordOptions = "WordOptions";
StringT HelpOptions = "HelpOptions";

StringT SubOptions = NULL;

StringT Options = "Options";
StringT Defaults = "Defaults";
StringT BitmapChars = "BitmapChars";
StringT HelpBrowse = "HelpBrowse";
StringT HelpContents = "HelpContents";

StringT HPJOptions = "OPTIONS";

StringT RunInHeads[3] = { "Normal", "Runin", NULL };
StringT SmallCaps[3] = { "None", "Standard", NULL };
StringT RMargTabs[5] = { "None", "Left", "Right", "Both", NULL };
StringT SideHeads[5] = { "Left", "Indent", "Frame", "Normal", NULL };
StringT FootNotes[7] = {
	"Standard", "Embed", "None", "Jump", "Popup", "Variable", NULL 
};
StringT PageBreaks[4] = { "Normal", "Format", "Remove", NULL };
StringT HeadFoot[4] = { "Standard", "TextOnly", "None", NULL };
StringT TableTitles[4] = { "0", "1", "2", NULL };
StringT TableRules[9] = { "Cell", "None", "Variable", 
  "Box", "Double", "Thick", "Shadow", "Para", NULL };
StringT TableGraphics[4] = { "Standard", "None", "Outside", NULL };

StringT IdxUsage[4] = { "Standard", "Help", "None", NULL };
StringT XrefUsage[4] = { "Standard", "Help", "None", NULL };
StringT XrefType[3] = { "Full", "Numeric", NULL };
StringT QuoteUsage[4] = { "Standard", "Help", "None", NULL };
StringT OccludedTabs[4] = { "Remove", "Space", "Tab", NULL };
StringT ParaSpace[3] = { "Normal", "Frame", NULL };
StringT Bullets[3] = { "Standard", "Help", NULL };
StringT Output[3] = { "Standard", "Help", NULL };
StringT HelpVersion[3] = { "3", "4", NULL };
StringT CntTypes[4] = { "Full", "Body", "None", NULL };
StringT ALinkMarkerType[5] = { "0", "4", "5", "6", NULL };
StringT MarkerType11[4] = { "None", "Midtopic", "Full", NULL };
StringT DirectiveMarker[3] = { "0", "3", NULL };
StringT ObjectIDs[4] = { "None", "Referenced", "All", NULL };


/* text prop ini read/write */

VoidT ReadTextPropsFromIni(StringT ininame, BoolT confirm)
{
	IntT val = 0;
	StringT hpjname;

	CurrDesText = DefDesText;
	hpjname = GetHpjName(ininame);

	// Basic Styles
	TrueFalseIni(ininame, SubOptions, "RemoveUnusedStyles", &CurrDesText.rem_unused_sty);

	// Fonts
	GetIniString(ininame, Defaults, "FontWidth", &CurrDesText.def_fwid);
	GetIniString(ininame, Defaults, "LineSpacing", &CurrDesText.def_lspace);
	GetIniString(ininame, Defaults, "FontName", &CurrDesText.def_font);
	GetIniString(ininame, Defaults, "FontSize", &CurrDesText.def_fsize);

	GetIniString(ininame, Graphics, "DefFont", &CurrDesText.wmf_font);
	GetIniString(ininame, Graphics, "DefFSize", &CurrDesText.wmf_fsize);

	GetIniString(ininame, BitmapChars, "BitmapFont", &CurrDesText.bmc_font);
	CurrDesText.bmc_used = (CurrDesText.bmc_font && *CurrDesText.bmc_font) ? True : False;

	TrueFalseIni(ininame, SubOptions, "RemoveUnusedFonts", &CurrDesText.rem_unused_fonts);

	// Basic Text
	TrueFalseIni(ininame, Options, "HideWhiteText", &CurrDesText.hide_white);
	TrueFalseIni(ininame, SubOptions, "TextColor", &CurrDesText.keep_color);
	TrueFalseIni(ininame, SubOptions, "UseHyperColor", &CurrDesText.hyper_color);
	TrueFalseIni(ininame, Options, "KeepAnchorParagraphs", &CurrDesText.keep_anchor);
	TrueFalseIni(ininame, SubOptions, "CharStylesUsedInText", &CurrDesText.char_styles_used);
	TrueFalseIni(ininame, Options, "ExactLineSpace", &CurrDesText.exact_space);
	
	val = 0;
	GetIniMatch(ininame, SubOptions, "SmallCaps", SmallCaps, &val);
	if (val) 
		CurrDesText.small_caps = val - 1;

	val = 0;
	GetIniMatch(ininame, SubOptions, "RunInHeads", RunInHeads, &val);
	if (val) 
		CurrDesText.runin_heads = val - 1;

	val = 0;
	GetIniMatch(ininame, Options, "RMarginTabs", RMargTabs, &val);
	if (val) 
		CurrDesText.rm_tabs = val - 1;

	val = 0;
	GetIniMatch(ininame, SubOptions, "Sideheads", SideHeads, &val);
	if (val) 
		CurrDesText.sidehds = val - 1;
	TrueFalseIni(ininame, SubOptions, "SHSpannerAnchors", &CurrDesText.anchor_spanners);
	GetIniString(ininame, SubOptions, "SHVertAdjust", &CurrDesText.sidehd_vs);

	val = 0;
	GetIniMatch(ininame, SubOptions, "Footnotes", FootNotes, &val);
	if (val) 
		CurrDesText.fnotes = val - 1;

	// Text Layout
	TrueFalseIni(ininame, SubOptions, "SingleFlow", &CurrDesText.single_flow);
	TrueFalseIni(ininame, SubOptions, "UseTextFrames", &CurrDesText.text_frames);
	TrueFalseIni(ininame, SubOptions, "WrapAroundTextFrames", &CurrDesText.tframe_wrap);
	GetIniString(ininame, SubOptions, "PageColumns", &CurrDesText.tcols);

	val = 0;
	GetIniMatch(ininame, SubOptions, "PageBreaks", PageBreaks, &val);
	if (val) 
		CurrDesText.pg_break = val - 1;

	val = 0;
	GetIniMatch(ininame, SubOptions, "HeadFoot", HeadFoot, &val);
	if (val) 
		CurrDesText.hd_ft = val - 1;

	GetIniString(ininame, SubOptions, "HFVertAdjust", &CurrDesText.hdft_vs);
	TrueFalseIni(ininame, SubOptions, "HFFramed", &CurrDesText.hf_framed);
	TrueFalseIni(ininame, SubOptions, "WrapAroundHFFrames", &CurrDesText.hfframe_wrap);
	GetIniString(ininame, SubOptions, "HFGap", &CurrDesText.hfframe_sp);

	// Tables
	val = 0;
	GetIniMatch(ininame, SubOptions, "TableTitles", TableTitles, &val);
	if (val) 
		CurrDesText.tbl_titles = val - 1;

	GetIniString(ininame, SubOptions, "TblColWidPct", &CurrDesText.tbcol_scale);
	GetIniString(ininame, SubOptions, "TblColWidAdd", &CurrDesText.tbcol_add);

	val = 0;
	GetIniMatch(ininame, SubOptions, "TableRules", TableRules, &val);
	if (val) 
		CurrDesText.tbl_rules = val - 1;

	val = 0;
	GetIniMatch(ininame, SubOptions, "TableGraphics", TableGraphics, &val);
	if (val) 
		CurrDesText.tbl_graphics = val - 1;

	TrueFalseIni(ininame, SubOptions, "StripTables", &CurrDesText.tbl_strip);

	TrueFalseIni(ininame, SubOptions, "TableContinued", &CurrDesText.tbl_use_cont);
	GetIniString(ininame, SubOptions, "TableContVar", &CurrDesText.tbl_cont_var);

	TrueFalseIni(ininame, SubOptions, "TableSheet", &CurrDesText.tbl_use_sheet);
	GetIniString(ininame, SubOptions, "TableSheetVar", &CurrDesText.tbl_sheet_var);

	// Advanced Text
	val = 0;
	GetIniMatch(ininame, SubOptions, "Index", IdxUsage, &val);
	if (val) 
		CurrDesText.idx_usage = val - 1;

	val = 0;
	GetIniMatch(ininame, SubOptions, "Xrefs", XrefUsage, &val);
	if (val) 
		CurrDesText.xref_usage = val - 1;

	val = 0;
	GetIniMatch(ininame, SubOptions, "XrefType", XrefType, &val);
	if (val) 
		CurrDesText.xref_type = val - 1;

	val = 0;
	GetIniMatch(ininame, SubOptions, "Quotes", QuoteUsage, &val);
	if (val) 
		CurrDesText.quote_usage = val - 1;

	val = 0;
	GetIniMatch(ininame, Options, "OccludedTabs", OccludedTabs, &val);
	if (val) 
		CurrDesText.occluded_tabs = val - 1;

	val = 0;
	GetIniMatch(ininame, Options, "ParaSpace", ParaSpace, &val);
	if (val) 
		CurrDesText.frame_spacing = (val - 1) ? True : False;

	TrueFalseIni(ininame, Options, "CharStyles", &CurrDesText.char_styles);
	TrueFalseIni(ininame, SubOptions, "HelpSectionBreaks", &CurrDesText.secbk_help);
	TrueFalseIni(ininame, SubOptions, "KeepSectBreaks", &CurrDesText.secbk_word);

	// HelpText
	TrueFalseIni(ininame, SubOptions, "TitleScroll", &CurrDesText.scroll_title);
	TrueFalseIni(ininame, SubOptions, "ExtendHelpNoScroll", &CurrDesText.xnscroll_title);
	TrueFalseIni(ininame, SubOptions, "TitleIndent", &CurrDesText.ind_title);
	TrueFalseIni(ininame, SubOptions, "TitleSpace", &CurrDesText.space_title);
	TrueFalseIni(ininame, SubOptions, "HelpLineBreak", &CurrDesText.linebk_title);

	GetIniString(ininame, HelpBrowse, "Digits", &CurrDesText.browse_digits);
	GetIniString(ininame, HelpBrowse, "Step", &CurrDesText.browse_step);
	GetIniString(ininame, HelpBrowse, "Start", &CurrDesText.browse_start);
	GetIniString(ininame, HelpBrowse, "Prefix", &CurrDesText.browse_prefix);

	val = 0;
	GetIniMatch(ininame, SubOptions, "Bullets", Bullets, &val);
	if (val) 
		CurrDesText.bullet_bmc = (val - 1) ? True : False;

	GetIniString(ininame, Graphics, "BulletFile", &CurrDesText.bullet_file);
	TrueFalseIni(ininame, SubOptions, "BMPsForDingbats", &CurrDesText.dingbat_bmp);

	GetIniString(ininame, SubOptions, "KeywordLimit", &CurrDesText.keyword_limit);
	GetIniString(ininame, SubOptions, "HelpTabLimit", &CurrDesText.help_tab_limit);

	// Version
	val = 0;
	GetIniMatch(ininame, Options, "Output", Output, &val);
	if (val) 
		CurrDesText.output = val - 1;

	TrueFalseIni(ininame, WordOptions, "Word8", &CurrDesText.word8);
	TrueFalseIni(ininame, WordOptions, "WordPerfect", &CurrDesText.wordperf);

	GetIniString(ininame, FDK, "WordSuffix", &CurrDesText.word_suf);
	GetIniString(ininame, FDK, "WordPerfectSuffix", &CurrDesText.wp_suf);

	val = 0;
	GetIniMatch(ininame, HelpOptions, "HelpVersion", HelpVersion, &val);
	if (val) 
		CurrDesText.helpver = val - 1;
	TrueFalseIni(ininame, HelpOptions, "Altura", &CurrDesText.altura);
	TrueFalseIni(ininame, HelpOptions, "HyperHelp", &CurrDesText.hyperhelp);
	TrueFalseIni(ininame, HelpOptions, "ForceBmc", &CurrDesText.force_bmc);

	// Help Contents
	val = 0;
	GetIniMatch(ininame, HelpContents, "CntType", CntTypes, &val);
	if (val) 
		CurrDesText.cnt_type = val - 1;

	GetIniString(ininame, HelpContents, "CntName", &CurrDesText.cnt_name);
	CurrDesText.cnt_name_used = !IniBufEmpty();

	GetIniString(ininame, HelpContents, "CntBase", &CurrDesText.cnt_base);
	CurrDesText.cnt_base_used = !IniBufEmpty();

	GetIniString(ininame, HelpContents, "CntTitle", &CurrDesText.cnt_title);
	CurrDesText.cnt_title_used = !IniBufEmpty();

	GetIniString(ininame, HelpContents, "CntStartFile", &CurrDesText.cnt_start_file);
	CurrDesText.cnt_start_file_used = !IniBufEmpty();

	GetIniString(ininame, HelpContents, "CntTopHead", &CurrDesText.cnt_top_head);
	CurrDesText.cnt_top_head_used = !IniBufEmpty();

	GetIniString(ininame, HelpContents, "CntBStyleText", &CurrDesText.cnt_b_topic);
	CurrDesText.cnt_b_topic_used = !IniBufEmpty();

	TrueFalseIni(ininame, HelpContents, "AddCntFileName", &CurrDesText.cnt_files);
	TrueFalseIni(ininame, HelpContents, "AddCntWindowName", &CurrDesText.cnt_wins);
	GetIniString(ininame, HelpContents, "CntMainWindow", &CurrDesText.cnt_main_win);

	// Help Compile
	TrueFalseIni(ininame, FDK, "MakeCombinedCnt", &CurrDesText.comb_cnt);
	TrueFalseIni(ininame, FDK, "CompileHelp", &CurrDesText.comp_hlp);
	GetIniString(ininame, FDK, "HPJFileName", &CurrDesText.hpj_name);

	GetIniString(hpjname, HPJOptions, "HLP", &CurrDesText.hlp_name);
	GetIniString(hpjname, HPJOptions, "TITLE", &CurrDesText.hlp_title);
	GetIniString(hpjname, HPJOptions, "CONTENTS", &CurrDesText.contents_ref);

	GetIniString(hpjname, HPJOptions, "COPYRIGHT", &CurrDesText.copyr_txt);
	CurrDesText.copyr_date = 
			(F_StrSubString(CurrDesText.copyr_txt, "%date") == -1) ? False : True;

	// Templates
	TrueFalseIni(ininame, FDK, "ApplyTemplateFile", &CurrDesText.fm_tpl_apply);
	GetIniString(ininame, FDK, "TemplateFileName", &CurrDesText.fm_tpl_name);
	GetIniInt(ininame, FDK, "AppliedTemplateFlags", &CurrDesText.fm_tpl_flags);

	TrueFalseIni(ininame, SubOptions, "TemplateAutoUpdate", &CurrDesText.wd_tpl_auto);
	GetIniString(ininame, SubOptions, "Template", &CurrDesText.wd_tpl_name);
	CurrDesText.wd_tpl_apply = IniBufEmpty() ? False : True;

	// Help Markers
	val = 0;
	GetIniMatch(ininame, SubOptions, "ALinkMarkerType", ALinkMarkerType, &val);
	if (val) 
		CurrDesText.alink_mark = val - 1;
	else if (!IniBufEmpty()) {
		GetIniString(ininame, SubOptions, "ALinkMarkerType", &CurrDesText.alink_cust);
		CurrDesText.alink_mark = 4;
	}

	val = 0;
	GetIniMatch(ininame, SubOptions, "MarkerType11", MarkerType11, &val);
	if (val) 
		CurrDesText.typ11_mark = val - 1;

	val = 0;
	GetIniMatch(ininame, Options, "DirectiveMarker", DirectiveMarker, &val);
	if (val) 
		CurrDesText.directive_mark = val - 1;
	else if (!IniBufEmpty()) {
		GetIniString(ininame, Options, "DirectiveMarker", &CurrDesText.directive_cust);
		CurrDesText.directive_mark = 2;
	}

	TrueFalseIni(ininame, SubOptions, "NoXrefPopups", &CurrDesText.no_xref_popup);

	// MultiFile
	TrueFalseIni(ininame, SubOptions, "UseFileIDs", &CurrDesText.use_file_ids);
	TrueFalseIni(ininame, FDK, "UseGraphicFileID", &CurrDesText.use_graph_fids);
	TrueFalseIni(ininame, FDK, "UseFrameTOC", &CurrDesText.use_fm_toc);
	TrueFalseIni(ininame, FDK, "UseFrameIX", &CurrDesText.use_fm_ix);

	val = 0;
	GetIniMatch(ininame, HelpOptions, "ObjectIDs", ObjectIDs, &val);
	if (val) 
		CurrDesText.object_ids = val - 1;

	GetIniString(ininame, HelpOptions, "XrefFileDefault", &CurrDesText.hlp_xref_default);
	GetIniString(ininame, HelpOptions, "XrefFileSuffix", &CurrDesText.hlp_xref_suf);


	TextChanged = False;
}


VoidT WriteTextPropsToIni(StringT ininame, BoolT confirm)
{
	IntT pos;
	StringT tstr;
	StringT hpjname;

	if (!TextChanged)
		return;
	hpjname = GetHpjName(ininame);

	// Basic Styles
	SetIniTF(ininame, SubOptions, "RemoveUnusedStyles", CurrDesText.rem_unused_sty);

	// Fonts
	SetIniStr(ininame, Defaults, "FontWidth", CurrDesText.def_fwid);
	SetIniStr(ininame, Defaults, "LineSpacing", CurrDesText.def_lspace);
	SetIniStr(ininame, Defaults, "FontName", CurrDesText.def_font);
	SetIniStr(ininame, Defaults, "FontSize", CurrDesText.def_fsize);

	SetIniStr(ininame, Graphics, "DefFont", CurrDesText.wmf_font);
	SetIniStr(ininame, Graphics, "DefFSize", CurrDesText.wmf_fsize);

	if (CurrPropSetNum == DES_PROPSET_HELP)
		SetIniStr(ininame, BitmapChars, "BitmapFont", CurrDesText.bmc_used ?
																		 CurrDesText.bmc_font : NULL);

	SetIniTF(ininame, SubOptions, "RemoveUnusedFonts", CurrDesText.rem_unused_fonts);

	// Basic Text
	SetIniTF(ininame, Options, "HideWhiteText", CurrDesText.hide_white);
	SetIniTF(ininame, SubOptions, "TextColor", CurrDesText.keep_color);

	if (CurrPropSetNum == DES_PROPSET_HELP)
		SetIniTF(ininame, SubOptions, "UseHyperColor", CurrDesText.hyper_color);

	SetIniTF(ininame, Options, "KeepAnchorParagraphs", CurrDesText.keep_anchor);

	if (CurrPropSetNum == DES_PROPSET_WORD)
		SetIniTF(ininame, SubOptions, "CharStylesUsedInText", CurrDesText.char_styles_used);

	SetIniTF(ininame, Options, "ExactLineSpace", CurrDesText.exact_space);
	SetIniStr(ininame, SubOptions, "SmallCaps", SmallCaps[CurrDesText.small_caps]);

	SetIniStr(ininame, SubOptions, "RunInHeads", RunInHeads[CurrDesText.runin_heads]);
	SetIniStr(ininame, Options, "RMarginTabs", RMargTabs[CurrDesText.rm_tabs]);
	SetIniStr(ininame, SubOptions, "Sideheads", SideHeads[CurrDesText.sidehds]);

	if (CurrPropSetNum == DES_PROPSET_WORD) {
		SetIniTF(ininame, SubOptions, "SHSpannerAnchors", CurrDesText.anchor_spanners);
		SetIniStr(ininame, SubOptions, "SHVertAdjust", CurrDesText.sidehd_vs);
	}
	SetIniStr(ininame, SubOptions, "Footnotes", FootNotes[CurrDesText.fnotes]);

	// Text Layout
	SetIniTF(ininame, SubOptions, "SingleFlow", CurrDesText.single_flow);

	if (CurrPropSetNum == DES_PROPSET_WORD) {
		SetIniTF(ininame, SubOptions, "UseTextFrames", CurrDesText.text_frames);
		SetIniTF(ininame, SubOptions, "WrapAroundTextFrames", CurrDesText.tframe_wrap);
		SetIniStr(ininame, SubOptions, "PageColumns", CurrDesText.tcols);

		SetIniStr(ininame, SubOptions, "PageBreaks", PageBreaks[CurrDesText.pg_break]);

		SetIniStr(ininame, SubOptions, "HeadFoot", HeadFoot[CurrDesText.hd_ft]);
		SetIniStr(ininame, SubOptions, "HFVertAdjust", CurrDesText.hdft_vs);
		SetIniTF(ininame, SubOptions, "HFFramed", CurrDesText.hf_framed);
		SetIniTF(ininame, SubOptions, "WrapAroundHFFrames", CurrDesText.hfframe_wrap);
		SetIniStr(ininame, SubOptions, "HFGap", CurrDesText.hfframe_sp);
	}

	// Tables
	SetIniStr(ininame, SubOptions, "TableTitles", TableTitles[CurrDesText.tbl_titles]);

	SetIniStr(ininame, SubOptions, "TblColWidPct", CurrDesText.tbcol_scale);
	SetIniStr(ininame, SubOptions, "TblColWidAdd", CurrDesText.tbcol_add);

	SetIniStr(ininame, SubOptions, "TableRules", TableRules[CurrDesText.tbl_rules]);

	SetIniStr(ininame, SubOptions, "TableGraphics", TableGraphics[CurrDesText.tbl_graphics]);

	if (CurrPropSetNum == DES_PROPSET_HELP)
		SetIniTF(ininame, SubOptions, "StripTables", CurrDesText.tbl_strip);

	if (CurrPropSetNum == DES_PROPSET_WORD) {
		SetIniTF(ininame, SubOptions, "TableContinued", CurrDesText.tbl_use_cont);
		SetIniStr(ininame, SubOptions, "TableContVar", CurrDesText.tbl_cont_var);

		SetIniTF(ininame, SubOptions, "TableSheet", CurrDesText.tbl_use_sheet);
		SetIniStr(ininame, SubOptions, "TableSheetVar", CurrDesText.tbl_sheet_var);
	}

	// Advanced Text
	SetIniStr(ininame, SubOptions, "Index", IdxUsage[CurrDesText.idx_usage]);
	SetIniStr(ininame, SubOptions, "Xrefs", XrefUsage[CurrDesText.xref_usage]);
	SetIniStr(ininame, SubOptions, "XrefType", XrefType[CurrDesText.xref_type]);
	SetIniStr(ininame, SubOptions, "Quotes", QuoteUsage[CurrDesText.quote_usage]);
	SetIniStr(ininame, Options, "OccludedTabs", OccludedTabs[CurrDesText.occluded_tabs]);
	SetIniStr(ininame, Options, "ParaSpace", ParaSpace[CurrDesText.frame_spacing]);
	SetIniTF(ininame, Options, "CharStyles", CurrDesText.char_styles);
	if (CurrPropSetNum == DES_PROPSET_HELP)
		SetIniTF(ininame, SubOptions, "HelpSectionBreaks", CurrDesText.secbk_help);
	if (CurrPropSetNum == DES_PROPSET_WORD)
		SetIniTF(ininame, SubOptions, "KeepSectBreaks", CurrDesText.secbk_word);

	// Help Text
	if (CurrPropSetNum == DES_PROPSET_HELP) {
		SetIniTF(ininame, SubOptions, "TitleScroll", CurrDesText.scroll_title);
		SetIniTF(ininame, SubOptions, "ExtendHelpNoScroll", CurrDesText.xnscroll_title);
		SetIniTF(ininame, SubOptions, "TitleIndent", CurrDesText.ind_title);
		SetIniTF(ininame, SubOptions, "TitleSpace", CurrDesText.space_title);
		SetIniTF(ininame, SubOptions, "HelpLineBreak", CurrDesText.linebk_title);

		SetIniStr(ininame, HelpBrowse, "Digits", CurrDesText.browse_digits);
		SetIniStr(ininame, HelpBrowse, "Step", CurrDesText.browse_step);
		SetIniStr(ininame, HelpBrowse, "Start", CurrDesText.browse_start);
		SetIniStr(ininame, HelpBrowse, "Prefix", CurrDesText.browse_prefix);

		SetIniStr(ininame, SubOptions, "Bullets", Bullets[CurrDesText.bullet_bmc ? 1 : 0]);
		SetIniStr(ininame, Graphics, "BulletFile", CurrDesText.bullet_file);
		SetIniTF(ininame, SubOptions, "BMPsForDingbats", CurrDesText.dingbat_bmp);

		SetIniStr(ininame, SubOptions, "KeywordLimit", CurrDesText.keyword_limit);
		SetIniStr(ininame, SubOptions, "HelpTabLimit", CurrDesText.help_tab_limit);
	}

	// Version -- do not qualify by prop set
	SetIniStr(ininame, Options, "Output", Output[CurrDesText.output]);
	SetIniTF(ininame, WordOptions, "Word8", CurrDesText.word8);
	SetIniTF(ininame, WordOptions, "WordPerfect", CurrDesText.wordperf);

	SetIniStr(ininame, FDK, "WordSuffix", CurrDesText.word_suf);
	SetIniStr(ininame, FDK, "WordPerfectSuffix", CurrDesText.wp_suf);

	SetIniStr(ininame, HelpOptions, "HelpVersion", HelpVersion[CurrDesText.helpver]);
	SetIniTF(ininame, HelpOptions, "Altura", CurrDesText.altura);
	SetIniTF(ininame, HelpOptions, "HyperHelp", CurrDesText.hyperhelp);
	SetIniTF(ininame, HelpOptions, "ForceBmc", CurrDesText.force_bmc);

	// Help Contents
	if (CurrPropSetNum == DES_PROPSET_HELP) {
		SetIniStr(ininame, HelpContents, "CntType", CntTypes[CurrDesText.cnt_type]);

		SetIniStr(ininame, HelpContents, "CntName", CurrDesText.cnt_name_used ?
																			CurrDesText.cnt_name : NULL);
		SetIniStr(ininame, HelpContents, "CntBase", CurrDesText.cnt_base_used ?
																			CurrDesText.cnt_base : NULL);
		SetIniStr(ininame, HelpContents, "CntTitle", CurrDesText.cnt_title_used ?
																			CurrDesText.cnt_title : NULL);
		SetIniStr(ininame, HelpContents, "CntStartFile", CurrDesText.cnt_start_file_used ?
																			CurrDesText.cnt_start_file : NULL);
		SetIniStr(ininame, HelpContents, "CntTopHead", CurrDesText.cnt_top_head_used ?
																			CurrDesText.cnt_top_head : NULL);
		SetIniStr(ininame, HelpContents, "CntBStyleText", CurrDesText.cnt_b_topic_used ?
																			CurrDesText.cnt_b_topic : NULL);

		SetIniTF(ininame, HelpContents, "AddCntFileName", CurrDesText.cnt_files);
		SetIniTF(ininame, HelpContents, "AddCntWindowName", CurrDesText.cnt_wins);
		SetIniStr(ininame, HelpContents, "CntMainWindow", CurrDesText.cnt_main_win);
	}

	// Help Compile
	if (CurrPropSetNum == DES_PROPSET_HELP) {
		SetIniTF(ininame, FDK, "MakeCombinedCnt", CurrDesText.comb_cnt);
		SetIniTF(ininame, FDK, "CompileHelp", CurrDesText.comp_hlp);
		SetIniStr(ininame, FDK, "HPJFileName", CurrDesText.hpj_name);

		SetIniStr(hpjname, HPJOptions, "HLP", CurrDesText.hlp_name);
		SetIniStr(hpjname, HPJOptions, "TITLE", CurrDesText.hlp_title);
		SetIniStr(hpjname, HPJOptions, "CONTENTS", CurrDesText.contents_ref);

		if (CurrDesText.copyr_date) {
			if (!CurrDesText.copyr_txt || (*CurrDesText.copyr_txt == '\0'))
				CurrDesText.copyr_txt = F_StrCopyString("%date");
			else if (F_StrSubString(CurrDesText.copyr_txt, "%date") == -1) {
				// add %date to end
				tstr = F_StrNew(F_StrLen(CurrDesText.copyr_txt) + 7);
				F_StrCpy(tstr, CurrDesText.copyr_txt);
				F_StrCat(tstr, " %date");
			}
		}
		else {
			if (CurrDesText.copyr_txt && *CurrDesText.copyr_txt
			 && ((pos = F_StrSubString(CurrDesText.copyr_txt, "%date")) != -1)) {
				// remove %date from end
				if ((pos == 0)
				 && (F_StrLen(CurrDesText.copyr_txt) == 5)) {
					F_StrFree(CurrDesText.copyr_txt);
					CurrDesText.copyr_txt = NULL;
				}
				else if (pos == (F_StrLen(CurrDesText.copyr_txt) - 5))
					CurrDesText.copyr_txt[pos - 1] = '\0';
				// else not at end, leave it
			}
		}
		SetIniStr(hpjname, HPJOptions, "COPYRIGHT", CurrDesText.copyr_txt);
	}

	// Templates
	SetIniTF(ininame, FDK, "ApplyTemplateFile", CurrDesText.fm_tpl_apply);
	SetIniStr(ininame, FDK, "TemplateFileName", CurrDesText.fm_tpl_name);
	SetIniInt(ininame, FDK, "AppliedTemplateFlags", CurrDesText.fm_tpl_flags);

	SetIniStr(ininame, SubOptions, "TemplateFileName", CurrDesText.wd_tpl_apply ?
													 CurrDesText.wd_tpl_name : NULL);
	if (CurrDesText.wd_tpl_apply)
		SetIniTF(ininame, SubOptions, "TemplateAutoUpdate", CurrDesText.wd_tpl_auto);

	// Help Markers
	if (CurrPropSetNum == DES_PROPSET_HELP) {
		SetIniStr(ininame, SubOptions, "ALinkMarkerType", (CurrDesText.alink_mark < 4) ?
		 ALinkMarkerType[CurrDesText.alink_mark] : CurrDesText.alink_cust);

		SetIniStr(ininame, SubOptions, "MarkerType11", 
																		MarkerType11[CurrDesText.typ11_mark]);

		SetIniTF(ininame, SubOptions, "NoXrefPopups", CurrDesText.no_xref_popup);
	}

	SetIniStr(ininame, Options, "DirectiveMarker", (CurrDesText.directive_mark < 2) ?
	 DirectiveMarker[CurrDesText.directive_mark] : CurrDesText.directive_cust);

	// MultiFile
	SetIniTF(ininame, SubOptions, "UseFileIDs", CurrDesText.use_file_ids);
	SetIniTF(ininame, FDK, "UseGraphicFileID", CurrDesText.use_graph_fids);
	SetIniTF(ininame, FDK, "UseFrameTOC", CurrDesText.use_fm_toc);
	SetIniTF(ininame, FDK, "UseFrameIX", CurrDesText.use_fm_ix);

	if (CurrPropSetNum == DES_PROPSET_HELP) {
		SetIniStr(ininame, HelpOptions, "ObjectIDs", 
																		 ObjectIDs[CurrDesText.object_ids]);
		SetIniStr(ininame, HelpOptions, "XrefFileDefault", CurrDesText.hlp_xref_default);
		SetIniStr(ininame, HelpOptions, "XrefFileSuffix", CurrDesText.hlp_xref_suf);
	}


	TextChanged = False;
}


/* text flow handling */

TextFlowT *CurrTextFlow = NULL;
TextFlowT *DesTextFlows = NULL;
TextFlowT DefTextFlow = { "", 0, NULL };
TextFlowT FirstTextFlow = { "", 0, NULL };
TextFlowT *LastTextFlow = &FirstTextFlow;
TextFlowT ClipboardTextFlow;
BoolT TextFlowChanged = False;

StringT *DesTextFlowList = NULL;
IntT CurrTextFlowNum = 0;
IntT DesTextFlowCount = 0;
IntT DesTextFlowAlloc = 0;
BoolT TextFlowListChanged = False;


VoidT InitDesignerFlows(VoidT)
{
	ReadFlowsFromIni(CurrIni, True);	
	InitFlowList();
}


VoidT GetDocFlows(F_ObjHandleT docId)
{
	F_ObjHandleT compId = 0;
	StringT sname = NULL;
	TextFlowT *curr = NULL;

	compId = F_ApiGetId(FV_SessionId, docId, FP_FirstFlowInDoc);

	while (compId) {
		sname = F_ApiGetString(docId, compId, FP_Name);
		if (sname && *sname) {  // skip untagged flows
			if (InFlowList(sname))
				F_StrFree(sname);
			else {
				curr = (TextFlowT *) F_Alloc(sizeof(TextFlowT), NO_DSE);
				if (curr) {
					F_ClearPtr(curr, sizeof(TextFlowT));
					curr->name = sname;
				}
				LastTextFlow->next = curr;
				LastTextFlow = curr;
				DesTextFlowCount++;
			}
		}
		compId = F_ApiGetId(docId, compId, FP_NextFlowInDoc);
	}
}


BoolT InFlowList(StringT sname)
{
	TextFlowT *item = FirstTextFlow.next;

	while (item) {
		if (!F_StrICmp(sname, item->name))
			return True;
		item = item->next;
	}
	return False;
}


VoidT InitFlowList(VoidT)
{
	IntT i = 0;
	TextFlowT *curr = NULL;

	if (DesTextFlowList
	 && (DesTextFlowAlloc < DesTextFlowCount)) {
		F_Free(DesTextFlowList);
		DesTextFlowList = NULL;
	}

	if (DesTextFlowList == NULL) {
		DesTextFlowList = (StringT *) F_Alloc((DesTextFlowCount + 1) * sizeof(StringT), NO_DSE);
		DesTextFlowList[0] = "Flows";
		DesTextFlowAlloc = DesTextFlowCount;
	}

	/* make list from flows */
	curr = DesTextFlows;
	i = 0;
	while (curr) {
		DesTextFlowList[++i] = curr->name;
		curr = (TextFlowT *) curr->next;
	}

	/* sort the DesTextFlowList entries 1..DesTextFlowCount alphabetically */
	if (DesTextFlowCount > 1)
		qsort(&DesTextFlowList[1], DesTextFlowCount, sizeof(StringT), CmpStrings);

	SetNewTextFlow(1);  /* put at start of list, sets CurrTextFlow and CurrTextFlowNum */
	TextFlowListChanged = False;

	/* set the global namelist entries to the new info */
	DesNameLists[DES_NAMES_FLOWS].nameList = DesTextFlowList;
	DesNameLists[DES_NAMES_FLOWS].nameCount = DesTextFlowCount;
	DesNameLists[DES_NAMES_FLOWS].nameSel = CurrTextFlowNum;
}


VoidT SetNewTextFlow(IntT flownum)
{
	TextFlowT *curr = NULL;
	StringT sname = NULL;

	CurrTextFlowNum = flownum;
	sname = DesTextFlowList[flownum];
	curr = DesTextFlows;
	while (curr
	 && F_StrICmp(sname, curr->name))
		curr = (TextFlowT *) curr->next;
	if (curr)
		CurrTextFlow = curr;
}


VoidT FreeDesignerFlows(VoidT)
{
	TextFlowT *last = DesTextFlows;
	TextFlowT *curr = NULL;

	if (TextFlowChanged)
		WriteFlowsToIni(CurrIni, True);

	while (last) {
		F_StrFree(last->name);

		// free all the other 10 strings too

		curr = (TextFlowT *) (last->next);
		F_Free(last);
		last = curr;
	}

	DesTextFlows = NULL;
	DesTextFlowCount = 0;
	CurrTextFlow = NULL;
	FirstTextFlow.next = NULL;

	F_Free(DesTextFlowList);
	DesTextFlowList = NULL;
	CurrTextFlowNum = 0;
}


StringT FlowProps[5] = { "Normal", "Skip", NULL };


VoidT ReadFlowsFromIni(StringT ininame, BoolT confirm)
{
	TextFlowT *curr = NULL;
	StringT section = "TextFlows";
	StringT sname = NULL;

	/* scan DesTextFlows list for match, or add to end, and set props */
	GetIniSect(ininame, section);
	while ((sname = GetIniSectKey()) != NULL) {
		for (curr = DesTextFlows; curr; curr = (TextFlowT *) curr->next) {
			if (!F_StrICmp(sname, curr->name))
				break;
			if (!curr->next) {
				curr->next = F_Alloc(sizeof(TextFlowT), NO_DSE);
				curr = (TextFlowT *) curr->next;
				F_ClearPtr(curr, sizeof(TextFlowT));
				curr->name = F_StrCopyString(sname);
				curr->next = NULL;
				DesTextFlowCount++;
				break;
			}
		}
		GetIniMatch(ininame, section, sname, FlowProps, &curr->status);
	}

	TextFlowChanged = False;
}


VoidT WriteFlowsToIni(StringT ininame, BoolT confirm)
{
	TextFlowT *curr = NULL;
	StringT section = "TextFlows";

	if (TextFlowChanged == False)
		return;

	for (curr = DesTextFlows; curr; curr = (TextFlowT *) curr->next) {
		if (curr->status == 0)
			SetIniStr(ininame, section, curr->name, NULL);
		else
			SetIniStr(ininame, section, curr->name, FlowProps[curr->status - 1]);
	}

	TextFlowChanged = False;
}



/* font handling */

FontT *DesFonts;
FontT *CurrFont;
FontT DefFont = { "", 0, NULL, False, NULL, NULL };
FontT FirstFont = { "", 0, NULL, False, NULL, NULL };
FontT *LastFont = &FirstFont;
FontT ClipboardFont;
BoolT FontChanged = False;

StringT *DesFontList = NULL;
IntT CurrFontNum = 0;
IntT DesFontCount = 0;
IntT DesFontAlloc = 0;
BoolT FontListChanged = False;

F_StringsT FontFamilies = { 0, NULL };
F_IntsT FontAttributes = { 0, NULL };

F_StringsT FontStrings;

DFPropT DefFontProps[] = {
 { "Times", 1, "134", "Times New Roman" },
 { "Times New Roman", 1, "120", NULL },
 { "Helvetica", 2, "138", "Arial" },
 { "Arial", 2, "138", NULL },
 { "Arial-Narrow", 2, "115", NULL },
 { "Courier", 3, "144", "Courier New" },
 { "Courier New", 3, "144", NULL },
 { "AvantGarde", 2, "144", NULL },
 { "Bookman", 1, "150", NULL },
 { "Garamond", 1, "145", NULL },
 { "Helvetica-Black", 2, "155", NULL },
 { "Helvetica-Light", 2, "130", NULL },
 { "HelveticaNarrow", 2, "115", "Arial Narrow" },
 { "Helvetica-Narrow", 2, "115", "Arial Narrow" },
 { "Arial Narrow", 2, "115", NULL },
 { "Korinna", 1, "140", NULL },
 { "Century Schoolbook", 1, "135", "NewCenturySchlbk" },
 { "NewCenturySchlbk", 1, "135", NULL },
 { "MS Serif", 1, "144", NULL },
 { "Palatino", 1, "141", NULL },
 { "ZapfChancery", 4, "121", NULL },
 { "ZapfDingbats", 5, "180", NULL },
 { "Dingbats", 5, "180", NULL },
 { "Wingdings", 5, "180", NULL },
 { "Bullets", 5, "180", NULL },
 { "Common Bullets", 5, "180", "CommonBullets" },
 { "CommonBullets", 5, "180", NULL },
 { "Symbol", 6, "130", NULL },
 { "Algerian", 4, "140", NULL },
 { NULL, 0, NULL, NULL}
};

FontT *LastDefFont = NULL;


VoidT InitDesignerFonts(VoidT)
{
	ReadFontsFromIni(CurrIni, True);	
	InitFontList();
}


VoidT GetSessionFonts(VoidT)
{
	UIntT i;

	//if (LastDefFont)
	//	LastFont = LastDefFont;
	//else
		GetDefFontProps();

	FontFamilies = F_ApiGetStrings(0, FV_SessionId, FP_FontFamilyNames);
#if 1
// was ifndef FM50VER
	FontAttributes = F_ApiGetInts(0, FV_SessionId, FP_FontFamilyAttributes);
#endif

	for (i = 0; i < FontFamilies.len; i++)
#if 1
// was ifndef FM50VER
		if (FontAttributes.val[i] & FV_FAMILY_VISIBLE)
#endif
			AddDesFont(FontFamilies.val[i]);

	// name used is FP_FontFamily + FP_FontVariation if not "Regular"
}


VoidT GetDefFontProps(VoidT)
{
	DFPropT *fnt = DefFontProps;

	while (fnt->name) {
		AddDesFont(fnt->name);
		LastFont->ftyp = fnt->typ;
		LastFont->fwid = fnt->wid;
		if (fnt->rmap) {
			LastFont->remap = True;
			LastFont->rem_name = fnt->rmap;
		}
		fnt++;
	}
	LastDefFont = LastFont;
}


VoidT AddDesFont(StringT fam)
{
	FontT *curr = NULL;

	if (!InFontList(fam)) {
		curr = (FontT *) F_Alloc(sizeof(FontT), NO_DSE);
		if (curr) {
			F_ClearPtr(curr, sizeof(FontT));
			curr->family = F_StrCopyString(fam);
		}
		LastFont->next = curr;
		LastFont = curr;
		DesFontCount++;
	}
}


BoolT InFontList(StringT sname)
{
	FontT *item = FirstFont.next;

	while (item) {
		if (!F_StrICmp(sname, item->family))
			return True;
		item = item->next;
	}
	return False;
}


VoidT InitFontList(VoidT)
{
	IntT i = 0;
	FontT *curr = NULL;

	if (DesFontList
	 && (DesFontAlloc < DesFontCount)) {
		F_Free(DesFontList);
		DesFontList = NULL;
	}

	if (DesFontList == NULL) {
		DesFontList = (StringT *) F_Alloc((DesFontCount + 1) * sizeof(StringT), NO_DSE);
		DesFontList[0] = "Fonts";
		DesFontAlloc = DesFontCount;
	}

	/* make list from flows */
	curr = DesFonts;
	i = 0;
	while (curr) {
		DesFontList[++i] = curr->family;
		curr = (FontT *) curr->next;
	}

	/* sort the DesTextFlowList entries 1..DesTextFlowCount alphabetically */
	if (DesFontCount > 1)
		qsort(&DesFontList[1], DesFontCount, sizeof(StringT), CmpStrings);

	SetNewFont(FindFontInList("Arial"));  /* sets CurrFont and CurrFontNum */
	FontListChanged = True;
	FontStrings.val = DesFontList;
	FontStrings.len = DesFontCount + 1;


	/* set the global namelist entries to the new info */
	DesNameLists[DES_NAMES_FONTS].nameList = DesFontList;
	DesNameLists[DES_NAMES_FONTS].nameCount = DesFontCount;
	DesNameLists[DES_NAMES_FONTS].nameSel = CurrFontNum;
}


VoidT SetNewFont(IntT fontnum)
{
	FontT *curr = NULL;
	StringT sname = NULL;

	CurrFontNum = fontnum;
	sname = DesFontList[fontnum];
	curr = DesFonts;
	while (curr
	 && F_StrICmp(sname, curr->family))
		curr = (FontT *) curr->next;
	if (curr)
		CurrFont = curr;
}


VoidT FreeDesignerFonts(VoidT)
{
	FontT *last = DesFonts;
	FontT *curr = NULL;

	if (FontChanged)
		WriteFontsToIni(CurrIni, True);

	while (last) {
		F_StrFree(last->family);

		// free all the other 10 strings too

		curr = (FontT *) (last->next);
		F_Free(last);
		last = curr;
	}

	DesFonts = NULL;
	DesFontCount = 0;
	DesFontAlloc = 0;
	CurrFont = NULL;
	FirstFont.next = NULL;

	F_Free(DesFontList);
	DesFontList = NULL;
	CurrFontNum = 0;

	F_ApiDeallocateStrings(&FontFamilies);
	FontFamilies.len = 0;
	FontFamilies.val = NULL;
	F_ApiDeallocateInts(&FontAttributes);
	FontAttributes.len = 0;
	FontAttributes.val = NULL;
}


IntT FindFontInList(StringT sname)
{
	IntT fnum = 1;

	if (sname && *sname) {
		while (fnum <= DesFontCount) {
			if (!F_StrICmp(sname, DesFontList[fnum]))
				return fnum;
			fnum++;
		}
	}

	return -1;

}


StringT Fonts = { "Fonts" };
StringT FontWidths = { "FontWidths" };
StringT FontTypes = { "FontTypes" };
StringT FontSects[3] = { "Fonts", "FontTypes", "FontWidths" };


VoidT ReadFontsFromIni(StringT ininame, BoolT confirm)
{
	FontT *curr = NULL;
	StringT section = NULL;
	StringT sname = NULL;
	IntT sect = 0;

	for (sect = 0; sect < 3; sect++) {
		/* scan DesFonts list for match, or add to end, and set props */
		GetIniSect(ininame, section = FontSects[sect]);
		while ((sname = GetIniSectKey()) != NULL) {
			for (curr = DesFonts; curr; curr = (FontT *) curr->next) {
				if (!F_StrICmp(sname, curr->family))
					break;
				if (!curr->next) {
					curr->next = F_Alloc(sizeof(FontT), NO_DSE);
					curr = (FontT *) curr->next;
					F_ClearPtr(curr, sizeof(FontT));
					curr->family = F_StrCopyString(sname);
					curr->next = NULL;
					LastFont = curr;
					DesFontCount++;
					break;
				}
			}
			switch (sect) {
				case 0:
					GetIniString(ininame, section, sname, &curr->rem_name);
					curr->remap = (curr->rem_name && *(curr->rem_name)) ? True : False;
					if (curr->remap)
						AddDesFont(curr->rem_name);
					break;
				case 1:
					GetIniInt(ininame, section, sname, &curr->ftyp);
					break;
				case 2:
					GetIniString(ininame, section, sname, &curr->fwid);
					break;
				default:
					break;
			}
		}
	}

	FontChanged = False;
}


VoidT WriteFontsToIni(StringT ininame, BoolT confirm)
{
	FontT *curr = NULL;
	DFPropT *fnt = DefFontProps;

	if (FontChanged == False)
		return;

	for (curr = DesFonts; curr != LastDefFont->next; curr = (FontT *) curr->next) {
		SetIniStr(ininame, Fonts, curr->family, curr->remap ? curr->rem_name : NULL);
		if (curr->fwid != fnt->wid)
			SetIniStr(ininame, FontWidths, curr->family, curr->fwid ? curr->fwid : NULL);
		if (curr->ftyp != fnt->typ)
			SetIniInt(ininame, FontTypes, curr->family, curr->ftyp);
		fnt++;
	}

	for (curr = LastDefFont->next; curr; curr = (FontT *) curr->next) {
		SetIniStr(ininame, Fonts, curr->family, curr->remap ? curr->rem_name : NULL);
		SetIniStr(ininame, FontWidths, curr->family, curr->fwid ? curr->fwid : NULL);
		if (curr->ftyp)
			SetIniInt(ininame, FontTypes, curr->family, curr->ftyp);
	}

	FontChanged = False;
}




/* end of m2rtxt.c */


