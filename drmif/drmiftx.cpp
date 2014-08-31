 
// drmiftx.cc is the code for dcl FrameMaker text reading, JHG, 4/21/97
// Copyright (c) 1993-1997 by Omni Systems, Inc.  All rights reserved.

#include "dcappl.h"
#include "dcctlg.h"
#include "dcfili.h"
#include "dcini.h"
#include "drmif.h"
#include "drmiffl.h"



// list of MIF statements processed in this module


mif MIFrd::miftxpile[] = {
  { "ConditionCatalog",  newgroup,   group, 0,  2,  0, pseudogroup },
  { "Condition",         newgroup,   group, 2, 60,  0, cond_def },
  { "CTag",              nameval,    ename, 2, 61,  1, cond_tag },
  { "CState",            l_cstate,   imnem, 2, 61,  3, none },
  { "CStyle",            l_cstyle,   group, 2, 61,  2, cond_style },
  { "CSeparation",       separ,      imnem, 2, 61,  4, none },
  { "CColor",            nameval,    imnem, 2, 61,  4, cond_color },
  { "Conditional",       newgroup,   group, 0,  0,  0, cond_start },
  { "InCondition",       nameval,    imnem, 2, 62,  0, cond_use },
  { "Unconditional",     empty,      imnem, 2, 63,  0, cond_end },

  { "PgfCatalog",        newgroup,   group, 0,  3,  0, pseudogroup },
  { "Pgf",               newgroup,   group, 2, 80,  0, pform_def },
  { "PgfTag",            nameval,    ename, 2, 81,  1, pform_tag },
  { "PgfUseNextTag",     yorn,       imnem, 2,209,  3, next_utag },
  { "PgfNextTag",        nameval,    ename, 2,209,  4, next_tag },
  { "PgfLIndent",        sizeval,    ilong, 2, 32,  2, plind },
  { "PgfFIndent",        sizeval,    ilong, 2, 32,  1, pfind },
  { "PgfRIndent",        sizeval,    ilong, 2, 32,  3, prind },
  { "PgfAlignment",      l_palign,   imnem, 2, 31,  0, palign },
  { "PgfPlacement",      l_pplace,   imnem, 2, 11,  0, pplace },
	{ "PgfPlacementStyle", l_plstyl,   imnem, 2, 35,  1, plstyl },
	{ "PgfRunInDefaultPunct",stringval,group, 2, 35,  3, prunin },
  { "PgfSpBefore",       leadval,    ilong, 2, 22,  1, pspbef },
  { "PgfSpAfter",        leadval,    ilong, 2, 22,  3, pspaft },
  { "PgfWithPrev",       yorn,       imnem, 2, 12,  1, pkprev },
  { "PgfWithNext",       yorn,       imnem, 2, 12,  2, pknext },
  { "PgfBlockSize",      numval,     ilong, 2, 12,  0, pblock },
  { "PgfLineSpacing",    l_pspace,   ilong, 2, 21,  0, pspc_mode },
  { "PgfLeading",        leadval,    ilong, 2, 21,  0, pspc_val },
  { "PgfTopSeparator",   nameval,    etext, 0,  0,  0, ptsep },
  { "PgfBotSeparator",   nameval,    etext, 0,  0,  0, pbsep },
  { "PgfHyphenate",      yorn,       imnem, 2, 13,  1, phyuse },
  { "HyphenMaxLines",    numval,     ilong, 2, 13,  2, phyline },
  { "HyphenMinPrefix",   numval,     ilong, 2, 13,  4, phybef },
  { "HyphenMinSuffix",   numval,     ilong, 2, 13,  5, phyaft },
  { "HyphenMinWord",     numval,     ilong, 2, 13,  3, phyword },
  { "PgfLetterSpace",    yorn,       ilong, 2, 34,  2, lspace },
  { "PgfMinWordSpace",   numval,     ilong, 2, 33,  2, wspmin },
  { "PgfOptWordSpace",   numval,     ilong, 2, 33,  1, wspopt },
  { "PgfMaxWordSpace",   numval,     ilong, 2, 33,  3, wspmax },
  { "PgfLanguage",       l_plang,    imnem, 2, 17,  2, plang },

  { "PgfNumTabs",        numval,     group, 2, 40,  0, tscount },
  { "TabStop",           newgroup,   ilong, 2, 40,  0, tstop },
  { "TSX",               sizeval,    ilong, 2, 41,  0, tspos },
  { "TSType",            l_tstype,   ilong, 2, 41,  0, tstype },
  { "TSLeaderStr",       stringval,  etext, 2, 42,  1, tslead },
  { "TSDecimalChar",     numval,     ilong, 2, 42,  2, tsalign },

  { "FontCatalog",       newgroup,   group, 0,  4,  0, pseudogroup },
  { "PgfFont",           newgroup,   group, 2, 80,  0, pform_font },
  { "Font",              newgroup,   group, 2, 80,  0, cform_def },
  { "FTag",              nameval,    ename, 2, 81,  1, cform_tag },
  { "FSize",             fontval,    ilong, 2, 15,  2, f_size },
  { "FEncoding",         nameval,    ilong, 2, 14,  0, f_enc },
  { "FLanguage",         l_plang,    imnem, 2, 17,  2, none },
  { "FPlatformName",     nameval,    ename, 2, 15,  0, f_plat },
  { "FWesternPlatformName", nameval, ename, 2, 15, 15, f_plat },
 // { "FCombinedFontName", nameval,    ename, 2, 15, 16, f_string },
  { "FFamily",           nameval,    ename, 2, 15,  1, f_string },
  { "FVar",              nameval,    imnem, 2, 15,  3, f_string },
  { "FWeight",           nameval,    imnem, 2, 15,  4, f_string },
  { "FAngle",            nameval,    imnem, 2, 15,  5, f_string },
  { "FUnderline",        yorn,       imnem, 2, 15,111, f_bool },
  { "FDoubleUnderline",  yorn,       imnem, 2, 15,112, f_bool },
  { "FOverline",         yorn,       imnem, 2, 15,113, f_bool },
  { "FStrike",           yorn,       imnem, 2, 15, 92, f_bool },
  { "FUnderlining",      l_fline,    imnem, 2, 15,110, f_line },
  { "FCase",             l_fcase,    imnem, 2, 15, 70, f_case },
  { "FPosition",         l_fposit,   imnem, 2, 15, 60, f_posit },
  { "FSupScript",        yorn,       imnem, 2, 15, 62, f_bool },
  { "FSubScript",        yorn,       imnem, 2, 15, 61, f_bool },
  { "FChangeBar",        yorn,       imnem, 2, 15, 91, f_bool },
  { "FOutline",          yorn,       imnem, 2, 15, 82, f_bool },
  { "FShadow",           yorn,       imnem, 2, 15, 81, f_bool },
  { "FPairKern",         yorn,       imnem, 2, 34,  1, fkern },
  { "FSeparation",       separ,      imnem, 2, 15, 13, fsepar },
  { "FColor",            nameval,    imnem, 2, 15, 13, fcolor },
  { "FDX",               emval,      imnem, 2, 30,  0, fhpos },
  { "FDY",               emval,      imnem, 2, 20,  0, fvpos },
  { "FDW",               emval,      imnem, 2, 34,  3, fspread },

  { "Document",          newgroup,   group, 0,  1,  0, docgroup },
  { "DPageSize",         ptval,      elong, 3, 12,  1, none },
  { "DLanguage",         l_plang,    imnem, 2, 17,  2, none },
  { "DTwoSides",         yorn,       imnem, 3, 11,  9, psided },
  { "DParity",           l_pgparity, imnem, 3, 11, 10, pparity },
  { "DMaxInterLine",     leadval,    ilong, 2, 23,  2, none },
  { "DMaxInterPgf",      leadval,    ilong, 2, 23,  3, none },
  { "DShowAllConditions", yorn,      imnem, 2, 64,  1, none },
  { "DDisplayOverrides", yorn,       imnem, 2, 64,  2, none },
  { "DAutoChBars",       yorn,       imnem, 2, 18,  1, none },
  { "DChBarGap",         leadval,    ilong, 2, 18,  4, none },
  { "DChBarWidth",       leadval,    ilong, 2, 18,  5, none },
  { "DChBarPosition",    l_cbarpos,  imnem, 2, 18,  2, none },
  { "DSuperscriptSize",  sizeval,    ilong, 2, 15, 21, none },
  { "DSuperscriptShift", sizeval,    ilong, 2, 15, 22, none },
  { "DSubscriptSize",    sizeval,    ilong, 2, 15, 23, none },
  { "DSubscriptShift",   sizeval,    ilong, 2, 15, 24, none },
  { "DSmallCapsSize",    sizeval,    ilong, 2, 15, 25, none },
  { "DSuperscriptStretch", sizeval,  ilong, 2, 15, 26, none },
  { "DSubscriptStretch", sizeval,    ilong, 2, 15, 27, none },
  { "DSmallCapsStretch", sizeval,    ilong, 2, 15, 28, none },

  { "Page",              newgroup,   group, 3, 10,  0, new_page },
  { "PageType",          l_pgtype,   imnem, 3, 11,  6, pg_type },
  { "PageNum",           stringval,  etext, 3, 11,  8, pg_num },
  { "PageTag",           nameval,    ename, 3, 11,  1, pg_name },
  { "PageBackground",    nameval,    imnem, 3, 11,  4, pg_back },
  { "PageSize",          ptval,      elong, 3, 12,  1, pg_size },
  { "PageOrientation",   l_pgorient, imnem, 3, 11,  5, none },
  { "TextRect",          newgroup,   group, 3, 20,  0, fr_def },
  { "TRNumColumns",      numval,     imnem, 3, 21, 15, none },
  { "TRColumnGap",       sizeval,    ilong, 3, 21, 16, none },
  { "TRColumnBalance",   yorn,       imnem, 2, 24,  4, none },
  { "TRNext",            numval,     imnem, 3, 40,  0, fr_next },
  { "TRSideheadPlacement",l_shplc,   imnem, 2, 36,  2, fr_side },
  { "TRSideheadWidth",   sizeval,    ilong, 2, 36,  3, none },
  { "TRSideheadGap",     sizeval,    ilong, 2, 36,  4, none },

  { "TextFlow",          newgroup,   group, 2,  1,  0, tflow },
  { "TFTag",             nameval,    ename, 2,  6,  1, tftag },
  { "TFAutoConnect",     yorn,       imnem, 2,  6,  2, tauto },
  { "TFPostScript",      yorn,       imnem, 2,  6,  3, tpost },
  { "TFFeather",         yorn,       imnem, 2, 23,  1, none },
  { "TFSynchronized",    yorn,       imnem, 2, 24,  1, none },
  { "TFLineSpacing",     leadval,    ilong, 2, 24,  2, none },
  { "TFMinHangHeight",   leadval,    ilong, 2, 24,  3, none },
	{ "TFSideheads",       yorn,       imnem, 2, 36,  1, none },
  { "TFSideheadPlacement",l_shplc,   imnem, 2, 36,  2, fr_side },
  { "TFSideheadWidth",   sizeval,    ilong, 2, 36,  3, none },
  { "TFSideheadGap",     sizeval,    ilong, 2, 36,  4, none },
  { "Notes",             newgroup,   imnem, 0,  0,  0, fnotes },
  { "FNote",             noteval,    group, 2, 71,  0, fnote },
  { "Para",              newgroup,   imnem, 2, 11,  1, para },
  { "ParaLine",          newgroup,   imnem, 2, 11,  7, pline },
  { "TextRectID",        numval,     imnem, 2,  2,  0, tframe },

  { "PgfEndCond",        yorn,       imnem, 0,  0,  0, hidend },
  { "PgfCondFullPgf",    yorn,       imnem, 0,  0,  0, hidfull },

  { "TextInset",         newgroup,   imnem, 2, 75,  1, tinset },
  { "TiName",            stringval,  ename, 2, 75,  2, tiname },
  { "TiSrcFile",         fileval,    ename, 2, 75,  3, tifile },
  //{ "TiImportHint",      stringval,  ename, 2, 75,  4, tiimph },
  //{ "TiAutoUpdate",      yorn,       imnem, 2, 75,  5, tiaupd },
  //{ "TiLastUpdate",      numval,     stamp, 2, 75,  6, tilupd },
  //{ "TextInsetLocked",   yorn,       imnem, 2, 75,  7, tilock },
  { "TextInsetEnd",      empty,      imnem, 2, 75,  8, tinend },

  { "TiApiClient",       newgroup,   imnem, 2, 76,  1, ticlnt },
  { "TiClientName",      stringval,  ename, 2, 76,  2, ticlname },
  { "TiClientSource",    fileval,    ename, 2, 76,  3, ticlfile },
  { "TiClientType",      stringval,  ename, 2, 76,  4, ticltype },
  { "TiClientData",      stringval,  etext, 2, 76,  5, ticldata },

  { "TiFlow",            newgroup,   imnem, 2, 77,  1, tiflow },
  { "TiMainFlow",        yorn,       imnem, 2, 77,  2, timflw },
  { "TiPageSpace",       l_tipgspc,  imnem, 2, 77,  3, tipgsp },
  { "TiFlowName",        stringval,  ename, 2, 77,  4, tiftag },
  //{ "TiFormatting",      l_tiform,   imnem, 2, 77,  5, tiform },
  //{ "TiFormatRemoveOverrides", yorn, imnem, 2, 77,  6, tinovr },
  //{ "TiFormatRemovePageBreaks", yorn, imnem, 2, 77, 7, tinpgb },

  { "TiText",            newgroup,   imnem, 2, 78,  1, titext },
  { "TiEOLisEOP",        yorn,       imnem, 2, 78,  2, titeol },
  { "TiTxtEncoding",     l_ticode,   imnem, 2, 78,  3, titcod },

  { "TiTextTable",       newgroup,   imnem, 2, 79,  1, tittbl },
  { "TiTblTag",          stringval,  ename, 2, 79,  2, titbfm },
  { "TiTblIsByRow",      yorn,       imnem, 2, 79,  3, titbrow },
  { "TiTblNumCols",      numval,     ilong, 2, 79,  4, titbcols },
  { "TiTblSep",          stringval,  ename, 2, 79,  5, titbcsep },
  { "TiTblNumSep",       numval,     ilong, 2, 79,  5, titbnsep },
  { "TiTblNumHdrRows",   numval,     ilong, 2, 79,  6, titbnhdr },
  { "TiTblHeadersEmpty", yorn,       imnem, 2, 79,  6, titbehdr },
  { "TiTblTxtEncoding",  l_ticode,   imnem, 2, 79,  7, titbcod },

  { "SpclHyphenation",   yorn,       imnem, 2, 13,  6, none },
  { "String",            stringval,  etext, 2, 10,  1, text },
  { "Char",              l_chname,   etext, 2, 10,  1, tchar }
};

size_t MIFrd::miftx_size = (sizeof(miftxpile) / sizeof(mif));

char *MIFrd::wordtxpile[] = {
  (char *) (int) l_cstate, "CShown", "CHidden", NULL,
  (char *) (int) l_cstyle, "CAsIs", "CUnderline", "CNumericUnderline",
		"CDoubleUnderline", "CStrike", "COverline", "CChangeBar", 
		"CNumericUnderlineAndChangeBar", NULL,
  (char *) (int) l_fline,  "FNoUnderlining", "FSingle", "FDouble",
    "FNumeric", NULL,
  (char *) (int) l_fcase,  "FAsTyped", "FLowercase", "FUppercase",
    "FInitialCaps", "FSmallCaps", NULL,
  (char *) (int) l_fposit, "FNormal", "FSubscript", "FSuperscript", NULL,
  (char *) (int) l_palign, "Left", "Center", "Right", "LeftRight", NULL,
  (char *) (int) l_pplace, "Anywhere", "ColumnTop", "PageTop",
    "LPageTop", "RPageTop", NULL,
  (char *) (int) l_plstyl, "Normal", "StraddleNormalOnly", "Straddle",
    "SideheadFirstBaseline", "SideheadTop", "SideheadLastBaseline",
		"RunIn", NULL,
  (char *) (int) l_shplc, "Left", "Right", "Inside", "Outside", NULL,
  (char *) (int) l_pspace, "Fixed", "Proportional", "Floating", NULL,
  (char *) (int) l_plang, "NoLanguage", "USEnglish", "UKEnglish", "French",
    "Dutch", "German", "Italian", "Spanish", "Norwegian", "Portuguese",
    "Brazilian", "Danish", "Finnish", "Japanese", "SimplifiedChinese", 
		"TraditionalChinese", "Korean", NULL,
  (char *) (int) l_pgparity, "FirstLeft", "FirstRight", NULL,
  (char *) (int) l_cbarpos, "LeftOfCol", "RightOfCol",
    "FurthestEdge", "NearestEdge", NULL,
  (char *) (int) l_tstype, "Left", "Center", "Right", "Decimal", NULL,
  (char *) (int) l_pgtype, "BodyPage", "HiddenPage", "ReferencePage",
    "OtherMasterPage", "RightMasterPage", "LeftMasterPage", NULL,
  (char *) (int) l_pgorient, "Portrait", "Landscape", NULL,
  (char *) (int) l_tipgspc, "BodyPage", "ReferencePage", NULL,
  (char *) (int) l_tiform, "TiSource", "TiEnclosing", "TiPlainText", NULL,
  (char *) (int) l_ticode, "TiIsoLatin", "TiASCII", "TiANSI", "TiJIS",
		"TiShiftJIS", "TiEUC", "TiBig5", "TiEUCCNS", "TiGB", "TiHZ",
		"TiKorean", "TiUTF8", "TiUTF16BE", "TiUTF16LE", "TiUTF32BE", 
		"TiUTF32LE",  NULL,
  (char *) (int) l_chname, "Tab", "HardSpace", "SoftHyphen", "DiscHyphen",
    "NoHyphen", "Cent", "Pound", "Yen", "EnDash", "EmDash", "Dagger",
    "DoubleDagger", "Bullet", "HardReturn", "EndOfPara", "EndOfFlow",
    "NumberSpace", "ThinSpace", "EnSpace", "EmSpace", "HardHyphen", NULL,
  NULL
};



// start of code

long MIFrd::TextInsetNum = 0;
long MIFrd::NextInsetNum = 0;
long MIFrd::TextInsetLev = 0;
DCilist MIFrd::TextInsetStack;

void
MIFrd::ProcTx(mif *mp)
{
  switch (mp->spec) {
    case cond_def:
      ProcCondDef(mp);
      break;
    case cond_tag:
      ProcCondTag(mp);
      break;
    case cond_style:
      ProcCondStyle(mp);
      break;
    case cond_color:
      ProcColor();
			if (MIFval)
				MakeCtl(mp);
      break;
    case cond_start:
      ProcCondStart();
      break;
    case cond_use:
      ProcCondUse(mp);
      break;
    case cond_end:
      ProcCondEnd(0);
      break;


    case pform_def:
      ProcPForm(mp);
      break;
    case pform_tag:
      ProcPTag(mp);
      break;
    case next_utag:
      if (!MIFval)
				CurrPgf.fnext = NULL;
			else if (!TagPF)
        MakeCtl(mp);
      break;
    case next_tag:
      if (*MIFstring) {
				CurrPgf.fnext = NewName(MIFstring);
				if (!TagPF)
		      MakeCtl(mp);
			}
      break;
		case plind:
			CurrPgf.lind = MIFval;
			if (!TagPF)
				MakeCtl(mp);
			break;
		case pfind:
			CurrPgf.find = MIFval;
			if (!TagPF)
				MakeCtl(mp);
			break;
		case prind:
			CurrPgf.rind = MIFval;
			if (!TagPF)
				MakeCtl(mp);
			break;
    case palign:
      if (MIFval < 0)
        break;
      MIFval++;
			CurrPgf.align = (uns) MIFval;
			if (!TagPF)
				MakeCtl(mp);
      break;
    case pplace:
      if (MIFval < 0)
        break;
			CurrPgf.pplace = (uns) MIFval;
			if (!TagPF)
	      ProcPPlace(mp);
      break;
    case plstyl:
      if (MIFval < 0)
        break;
			CurrPgf.pstyle = (uns) MIFval;
			if (!TagPF)
		    ProcPStyle(mp);
      break;
    case prunin:
			CurrPgf.runin = NewName(MIFstring);
      break;
		case pspbef:
			CurrPgf.sbval = MIFval;
			if (!TagPF)
				MakeCtl(mp);
			break;
		case pspaft:
			CurrPgf.saval = MIFval;
			if (!TagPF)
				MakeCtl(mp);
			break;
		case pkprev:
			CurrPgf.kprev = (unc) MIFval;
			if (!TagPF)
				MakeCtl(mp);
			break;
		case pknext:
			CurrPgf.knext = (unc) MIFval;
			if (!TagPF)
				MakeCtl(mp);
			break;
    case pblock:
			CurrPgf.kwid = (uns) MIFval;
			if (!TagPF)
	      ProcPBlock(mp);
      break;
    case pspc_mode:
      PSpMode = (MIFval > 0) ? 4 : 3;
			CurrPgf.sltype = (unc) PSpMode;
      break;
    case pspc_val:
			CurrPgf.slval = MIFval;
			if (!TagPF)
	      ProcPSpVal(mp);
      break;
    case ptsep:
      strncpy(PTSep, MIFstring, FStringMax);
			PTSepUsed = true;
      break;
    case pbsep:
      strncpy(PBSep, MIFstring, FStringMax);
			PBSepUsed = true;
      break;
		case phyuse:
			CurrPgf.hyuse = (unc) MIFval;
			if (!TagPF)
				MakeCtl(mp);
			break;
		case phyline:
			CurrPgf.hymline = (uns) MIFval;
			if (!TagPF)
				MakeCtl(mp);
			break;
		case phybef:
			CurrPgf.hymbef = (uns) MIFval;
			if (!TagPF)
				MakeCtl(mp);
			break;
		case phyaft:
			CurrPgf.hymaft = (uns) MIFval;
			if (!TagPF)
				MakeCtl(mp);
			break;
		case phyword:
			CurrPgf.hymword = (uns) MIFval;
			if (!TagPF)
				MakeCtl(mp);
			break;
    case lspace:
      if (MIFval)
        MIFval = 1000;
			CurrPgf.cspls = (uns) MIFval;
			if (!TagPF)
				MakeCtl(mp);
      break;
    case wspmin:
      MIFval *= 20;
			CurrPgf.wsmin = (uns) MIFval;
			if (!TagPF)
				MakeCtl(mp);
      break;
    case wspopt:
      MIFval *= 20;
			CurrPgf.wsopt = (uns) MIFval;
			if (!TagPF)
				MakeCtl(mp);
      break;
    case wspmax:
      MIFval *= 20;
			CurrPgf.wsmax = (uns) MIFval;
			if (!TagPF)
				MakeCtl(mp);
      break;
    case plang:
      if (MIFval < 0)
        break;
			CurrPgf.lang = (uns) MIFval;
			if (!TagPF)
				MakeCtl(mp);
      break;

    case tscount:
      ProcTSCount(mp);
      break;
    case tstop:
      EndProc[MIFlevel] = EndTStop;
      break;
    case tspos:
      TSPos = MIFval;
      break;
    case tstype:
      if (MIFval < 0)
        break;
      if ((TSType = MIFval + 1) == 4)
        TSType = 7;
      break;
    case tslead:
      ProcTSLead(mp);
      break;
    case tsalign:
      TSAlign = (uns) MIFval;
			if (!TagPF)
	      MakeCtl(mp);
      break;

    case cform_def:
      ProcCForm(mp);
      break;
    case cform_tag:
      ProcCTag(mp);
      break;
    case pform_font:
      ProcFForm();
      break;
		case f_plat:
			ProcFPlat();
			break;
    case f_string:
      ProcFString(mp);
      break;
    case f_enc:
      ProcFString(mp);
			switch (FENum) {
				case 1:   // ansi
					MIFval = 64;
					break;
				case 2:   // shiftJIS
					MIFval = 192;
					Asian = true;
					break;
				case 3:   // simp chinese
					MIFval = 193;
					Asian = true;
					break;
				case 4:   // big5 chinese
					MIFval = 194;
					Asian = true;
					break;
				case 5:   // korean
					MIFval = 195;
					Asian = true;
					break;

				default:
					LogEvent(logdebug, 3, "Unknown encoding ", MIFstring);
					MIFval = 0;
					break;
			}
      MakeCtl(mp);
      break;
    case f_size:
			if (GraphTextForm)
				CurrGrCForm->fsize = (uns) MIFval;
			else if (GraphicText)      
				CurrGrTextProp.fsize = (uns) MIFval;
			CurrFnt.fsize = MIFval;
			if (TagCF || (!TagPF) || GraphTextFontStart)
	      MakeCtl(mp);
      break;
    case f_line:
			if (MIFval < 0)
				break;
      if (MIFval == 3) // numeric underline
        MIFval = 1;    // becomes single
			mp->dat = 110 + (uns) MIFval; // set the shortval
			MIFval = 1;				 // mark it true
      ProcFBool(mp);		 // treat it like a bool
			mp->dat = 110;
      break;
    case f_case:
			if (MIFval < 0)
				break;
			mp->dat = 70 + (uns) MIFval; // set the shortval
			MIFval = 1;				 // mark it true
      ProcFBool(mp);		 // treat it like a bool
			mp->dat = 70;
      break;
    case f_posit:
			if (MIFval < 0)
				break;
			mp->dat = 60 + (uns) MIFval; // set the shortval
			MIFval = 1;				 // mark it true
      ProcFBool(mp);		 // treat it like a bool
			mp->dat = 60;
      break;
		case f_bool:
      ProcFBool(mp);
      break;

		case fkern:
			CurrFnt.kern = (uns) MIFval;
			if (!TagPF)
				MakeCtl(mp);
			break;
		case fsepar:
			if (GraphTextForm)
				CurrGrCForm->color = (uns) MIFval;
			else if (GraphicText)      
				CurrGrTextProp.color = (uns) MIFval;
			CurrFnt.color = (uns) MIFval;
			if (!TagPF)
				MakeCtl(mp);
			break;
		case fcolor:
			ProcColor();
			if (MIFval) {
				if (GraphTextForm)
					CurrGrCForm->color = (uns) MIFval;
				else if (GraphicText)      
					CurrGrTextProp.color = (uns) MIFval;
				CurrFnt.color = (uns) MIFval;
				if (!TagPF)
					MakeCtl(mp);
			}
			break;
    case fhpos:
      ProcHPos(mp);
      break;
    case fvpos:
      ProcVPos(mp);
      break;
    case fspread:
      if (MIFval)	 // given as percent * 100
        MIFval = 1000 + (MIFval / 10);
			CurrFnt.spread = MIFval;
			if (!TagPF)
	      MakeCtl(mp);
      break;

    case docgroup:
      MakePseudo( (psg_id) mp->min);
      EndProc[MIFlevel] = EndDocGroup;
      break;
    case psided:
      if (MIFval < 0)
        break;
      DoubleSided = (unc) MIFval;
      MakeCtl(mp);
      break;
    case pparity:
      if (MIFval < 0)
        break;
      FirstRight = (unc) MIFval;
      MakeCtl(mp);
      break;

    case new_page:
      ProcPage(mp);
      break;
    case pg_type:
      ProcPageType(mp);
      break;
		case pg_num:
			MakeCtl(mp);
			PageNumList.add(atol(MIFstring), PageID);
			break;
    case pg_name:
      ProcPageName(mp);
      break;
		case pg_size:
			MakeCtl(mp);
			PageWide = MIFpt.x;
			PageHigh = MIFpt.y;
			break;
    case pg_back:
      if ((MIFval = PageNameList(MIFstring)) != 0) {
        MakeCtl(mp);
        if (MasterTypeList.find(MIFval) == 2)
          PageUsageIndex.find(PageID)->siz(2);
      }
      else if (!strcmp(MIFstring, "Default")) {
        MIFval = (PageHand == 2) ? LeftMaster : RightMaster;
        MakeCtl(mp);
      }
      break;
    case fr_def:
      ProcTFrame(mp);
      break;
    case fr_next:
      ProcTFNext(mp);
      break;
    case fr_side:
      if (MIFval < 0)
        break;
      MIFval++;
      MakeCtl(mp);
      break;

    case tflow:
      ProcTextFlow(mp);
      break;
    case tauto:
      if (MIFval < 0)
        break;
      TFAuto = (unc) MIFval;
      MakeCtl(mp);
      break;
    case tpost:
      if (MIFval < 0)
        break;
      if ((TFPost = (bool) MIFval) != false)
        MakeCtl(mp);
      break;
    case tftag:
      if (!strcmp(MIFstring, "HIDDEN"))
        HiddenFlow = true;
      strncpy(TFTag, MIFstring, FStringMax);
      MakeCtl(mp);
      FlowTagged = true;
      break;
    case fnotes:
      ProcNotes();
      break;
    case fnote:
      ProcFNote(mp);
      break;
    case para:
      ProcPara(mp);
      break;
    case pline:
      ProcTLine();
      break;
    case tframe:
      ProcTFRef(mp);
      break;

    case hidend:
      HiddenEnd = (unc) MIFval;
      break;
    case hidfull:
      HiddenFull = (unc) MIFval;
      break;

		case tinset:
		  AddText(0);
			MIFval = TextInsetNum = ++NextInsetNum;
			TextInsetStack.add(TextInsetNum, ++TextInsetLev);
      MakeCtl(mp);
			break;
		case tinend:
			MIFval = TextInsetNum = TextInsetStack.find(TextInsetLev);
			TextInsetStack.remove(TextInsetLev--);
      MakeCtl(mp);
			break;
		case tiname:
		case tifile:
		case tiimph:
		case tiaupd:
		case tilupd:
		case tilock:
      MakeCtl(mp);
			break;

		case ticlnt:
			MIFval = 0;
		case ticlname:
		case ticlfile:
		case ticltype:
		case ticldata:
      MakeCtl(mp);
			break;

		case tiflow:
			MIFval = 0;
		case timflw:
		case tipgsp:
		case tiftag:
		case tiform:
		case tinovr:
		case tinpgb:
      MakeCtl(mp);
			break;

		case titext:
			MIFval = 0;
		case titeol:
		case titcod:
      MakeCtl(mp);
			break;

		case tittbl:
			MIFval = 0;
		case titbfm:
		case titbrow:
		case titbcols:
		case titbcsep:
		case titbnsep:
		case titbnhdr:
		case titbehdr:
		case titbcod:
      MakeCtl(mp);
			break;


    case text:
      ProcTextObj();
      break;
    case tchar:
      ProcChar();
      break;

    default:
      assert(0);
			LogEvent(logdebug, 2, "Unknown text CTL");
      break;
  }
}



// condition processing


uns MIFrd::CondID;
DCnlist MIFrd::CondNameList;
DClist *MIFrd::OldConds;
DClist *MIFrd::NewConds;
DClist *MIFrd::RowConds;


void
MIFrd::ProcCondDef(mif *mp)
{
  DCgroup *gp = new DCgroup(DCctl(mp->dtype, mp->maj, mp->min, ++CondID));
  GroupList[psg_cond]->add(*gp);
  GroupStack[MIFlevel] = gp;
}


void
MIFrd::ProcCondTag(mif *mp)
{
  CondNameList.addnew(MIFstring, CondID);
  MakeCtl(mp);
}


void
MIFrd::ProcCondStyle(mif *mp)
{
  DCctl *cp = NULL;
  DCctl *cp2 = NULL;

  switch (MIFval) {
    case 1: // underline
      cp = new DCctl(imnem, 2, 15, 11, 1);
      break;
    case 2: // numeric underline
      cp = new DCctl(imnem, 2, 15, 11, 4);
      break;
    case 3: // double underline
      cp = new DCctl(imnem, 2, 15, 11, 2);
      break;
    case 4: // strikethrough
      cp = new DCctl(imnem, 2, 15, 9, 2);
      break;
    case 5: // overline
      cp = new DCctl(imnem, 2, 15, 11, 3);
      break;
    case 6: // change bar
      cp = new DCctl(imnem, 2, 15, 9, 3);
      break;
    case 7: // change bar and numeric underline
      cp = new DCctl(imnem, 2, 15, 11, 4);
      cp2 = new DCctl(imnem, 2, 15, 9, 3);
      break;
    default: // nada
      return;
  }
  DCgroup *gp = new DCgroup(DCctl(mp->dtype, mp->maj, mp->min, mp->dat));
  gp->add(*cp);
	if (cp2)
		gp->add(*cp2);
  GroupStack[MIFlevel]->add(*gp);
}


void
MIFrd::ProcCondStart(void)
{
  AddText(0);
  assert(OldConds);
  NewConds = new DClist;
  EndProc[MIFlevel] = EndCondStart;
}


void
MIFrd::ProcCondUse(mif *mp)
{
	if (InTbRow && !InTbCell) {
		DCctl *cp = new DCctl(imnem, 2, 65, (uns) CondNameList(MIFstring));
		GroupStack[MIFlevel]->add(*cp);
	}
	else { // not a row cond
	  // identify cond as on, possibly still on, possibly newly on
		NewConds->add(CondNameList(MIFstring));
	}
}


void
MIFrd::EndCondStart(void)
{
  uns i;
  long cond;
  DCctl *cp = NULL;
  DCgroup *gp = GroupStack[MIFlevel];

  assert(OldConds);
  assert(NewConds);

  // now turn off any conds on before but not still on,
  for (i = 1; cond = OldConds->number(i); i++) {
    if (!NewConds->find(cond)) {
      cp = new DCctl(imnem, 2, 63, (uns) cond);
      gp->add(*cp);
    }
  }

  // and turn on any that were off before but are now on
  for (i = 1; cond = NewConds->number(i); i++) {
    if (!OldConds->find(cond)) {
      cp = new DCctl(imnem, 2, 62, (uns) cond);
      gp->add(*cp);
    }
  }

  // update status
  delete OldConds;
  OldConds = NewConds;
	NewConds = NULL;
	assert(OldConds);
}


void
MIFrd::ProcCondEnd(uns lev)
{
  uns i;
  long cond;
  DCctl *cp = NULL;

	if (OldConds == NULL)
		return;

  DCgroup *gp = GroupStack[MIFlevel - lev];

  AddText(0);
  // turn off all conds that are on
  for (i = 1; cond = OldConds->number(i); i++) {
    cp = new DCctl(imnem, 2, 63, (uns) cond);
    gp->add(*cp);
  }
  delete OldConds;
  OldConds = new DClist;
}



// format processing


uns MIFrd::FormatID = 0;
uns MIFrd::CurrPForm = 0;
DCnlist MIFrd::FormNameList;
DCnlist MIFrd::FormNSPNameList;
DCnlist MIFrd::FormTagNameList;
DCnlist MIFrd::CFormNameList;
DCnlist MIFrd::CFormNSPNameList;
DClist MIFrd::CFTagList;
DCilist MIFrd::FormTypeList;
DCilist MIFrd::FormSymList;
bool MIFrd::InPgf = false;
bool MIFrd::PgfTagged = false;
bool MIFrd::PgfTagSeen = false;
bool MIFrd::TagPF = false;
bool MIFrd::TagCF = false;
bool MIFrd::CFTagUsed = false;
bool MIFrd::AFFontDef = false;
bool MIFrd::Inheriting = false;

pform MIFrd::CurrPgf;
cform MIFrd::CurrFnt;
cform MIFrd::DefPgfFnt;
DCvlist MIFrd::PFormList;
DCctl *MIFrd::CurrPTagCtl = NULL;
DCnlist MIFrd::GenFileParaFmts;
npair MIFrd::GenFileParaFmtsInit[] = {
 { 1, "1body" },
 { 2, "Active*" },
 { 3, "IgnoreChars*" },
 { 4, "Mapping Table*" },
 { 5, "Separators*" },
 { 6, "SortOrder*" },
 NULL
};


void
MIFrd::ProcPForm(mif *mp)
{
  InPgf = true;
  *PTSep = *PBSep = *PAFont = *PAForm = '\0';
	PAFontSet = false;
  EndProc[MIFlevel] = EndPForm;

  if (CurrPG == psg_pgf) {  // defining format
    DCgroup *gp = new DCgroup(DCctl(mp->dtype, mp->maj, mp->min, ++FormatID));
    GroupList[psg_pgf]->add(*gp);
    GroupStack[MIFlevel] = gp;
    return;
  }

  // otherwise let contents percolate up to enclosing group
	//PgfTagSeen = false;
	//TagPF = true; // in case of no tag
}


void
MIFrd::EndPForm(void)
{
	DCctl *cp = NULL;
	cform *cf = NULL;
	pform *pf = NULL;

  // called at end of pgf definition in catalog and pgf overrides in text
	if ((CurrPTagCtl != NULL)
	 && (CurrPTagCtl->siz() == 0))	// ensure valid init format is called
		CurrPTagCtl->siz(CurrPgf.pnum);
	CurrPTagCtl = NULL;
	PgfTagged = true;
  InPgf = false;
	if (!TagPF) {
		ProcPRunIn(CurrPgf.runin);
		ProcPFCont();  // add px and sx
	}

	if ((CurrPG == psg_pgf)   // definition
	 || (TagPF && PgfTagSeen)) { // no init store if making tag format
		cf = new cform;
		*cf = CurrFnt;
		CurrPgf.fnt = cf;
		pf = new pform;
		*pf = CurrPgf;
		PFormList.add(pf, CurrPgf.pnum);
		if (TagPF) {
			PutPFProps();
			TagPF = false;
		}
	}
	else if (TagPF && !PgfTagSeen) { // no tag, put out overrides
		pf = (pform *) PFormList.find(CurrPForm);
		if (memcmp(pf, &CurrPgf, sizeof(pform))
		 || memcmp(pf->fnt, &CurrFnt, sizeof(cform))) {
			PutPgfOverrides(pf);
			cp = new DCctl(imnem, 2, 82, 1, 0L); // init store
	    GroupStack[MIFlevel - 1]->add(*cp);
		}
		TagPF = false;
	}
	else {       // override, not definition, so mark end of Pgf
		cp = new DCctl(imnem, 2, 82, 1, 0L);  // init store
		GroupStack[MIFlevel - 1]->add(*cp);
	}

	DefPgfFnt = CurrFnt;
}


void
MIFrd::ProcPTag(mif *mp)
{
  DCctl *cp = NULL;
	DCgroup *gp = GroupStack[MIFlevel];
  *PTSep = *PBSep = *PAFont = *PAForm = '\0';
	PAFontSet = false;

  if (CurrPG == psg_pgf) {  // defining format, get dcl ID from global
		CheckParaFormName(MIFstring);
    FormNameList.addnew(MIFstring, FormatID);
    FormNSPNameList.addnew(NoSpace(MIFstring), FormatID);
    FormTypeList.add(1, FormatID);
		CurrPgf.pnum = FormatID;
    MakeCtl(mp);
    cp = new DCctl(imnem, 2, 81, 2, 1);
    gp->add(*cp);
		return;
  }

  // using format, get dcl ID from list
	PgfTagSeen = true;
	TagPF = false;
	Inheriting = false;

  if ((MIFval = FormNameList.getnum(MIFstring)) != 0) {
    CurrPForm = (uns) MIFval;
		CurrPgf = *((pform *) PFormList.find(MIFval));
		CurrFnt = *(CurrPgf.fnt);
		DefPgfFnt = CurrFnt;
		if (CurrPTagCtl == NULL) { // outside Pgf, as in table format
			CurrPTagCtl = new DCctl(imnem, 2, 82, 1, CurrPForm);
			GroupStack[MIFlevel]->add(*CurrPTagCtl);
		}
		else 
			CurrPTagCtl->siz(CurrPForm);	// replace 0 with value
		CurrPTagCtl = NULL;
		PgfTagged = true;
    assert(FormTypeList.find(MIFval) == 1);  // must be init
    CurrTabList = (DCvlist *) TabFormatList.find(CurrPForm);
		SetSymFont(MIFval);
  }
  else if ((MIFval = FormTagNameList.getnum(MIFstring)) != 0) {
    CurrPForm = (uns) MIFval;
		CurrPgf.pnum = (uns) MIFval;
		// do not change props to match tag
		DefPgfFnt = CurrFnt;
		if (CurrPTagCtl == NULL) { // outside Pgf, as in table format
			CurrPTagCtl = new DCctl(imnem, 2, 82, 1, CurrPForm);
			GroupStack[MIFlevel]->add(*CurrPTagCtl);
		}
		else 
			CurrPTagCtl->siz(CurrPForm);	// replace 0 with value
		CurrPTagCtl = NULL;
		PgfTagged = true;
    assert(FormTypeList.find(CurrPForm) == 1);  // must be init
		// put out overrides to make props match current values
		PutPgfOverrides((pform *) PFormList.find(CurrPForm));
	}
	else {                  // make new init format on the fly
		if (UndefParaForms.getitem(MIFstring) == 0) {
			UndefParaForms.add(MIFstring, ++UndefParaFormsCount);
			if (!GenFileParaFmts.getwildrev(MIFstring))
				LogEvent(logwarn, 1, "Paragraph format not defined in catalog: ",
					MIFstring);
		}
		DCgroup *fgp = new DCgroup(DCctl(group, 2, 80, ++FormatID));
		GroupList[psg_pgf]->add(*fgp);
		TagPF = true;
		TabFormatList.add( (void *) CurrTabList, FormatID);
		CurrPForm = FormatID;
		CurrPgf.pnum = FormatID;
		if (CurrPTagCtl == NULL) { // outside Pgf, table fmt or defaults
			CurrPTagCtl = new DCctl(imnem, 2, 82, 1, FormatID);
			GroupStack[MIFlevel]->add(*CurrPTagCtl);
		}
		else 
			CurrPTagCtl->siz(FormatID);	// set value
		CurrPTagCtl = NULL;
		PgfTagged = true;
		GroupStack[MIFlevel] = fgp;	// switch to format definition group
		FormTagNameList.addnew(MIFstring, FormatID);
		FormTypeList.add(1, FormatID);
		MakeCtl(mp);								// create format name ctl
		cp = new DCctl(imnem, 2, 81, 2, 1);	// specify init format
		fgp->add(*cp);
	}
}


DCnlist MIFrd::UndefParaForms;
long MIFrd::UndefParaFormsCount = 0;


char *
MIFrd::NoSpace(char *nm)
{
	char *nsp = NewName(strlen(nm));
	char *pnm = nm;
	char *pnsp = nsp;
	while (*pnm) {
		if (*pnm != ' ')
			*pnsp++ = *pnm;
		pnm++;
	}
	*pnsp = '\0';
	return nsp;
}


void
MIFrd::CheckParaFormName(char *pname)
{
	long pos = 0;
	long spos = 0;
	DCnlist *nitem = NULL;
	const char *nname = NULL;
	char *nspname = NoSpace(pname);

	if (((pos = FormNameList.getitem(pname, &nitem, false, false)) != 0)
	 && nitem
	 && ((nname = nitem->name) != NULL)) {
		if (!strcmp(nname, pname))
			LogEvent(logwarn, 1, "Exact match of two para format names: ",
				nname, " and ", pname);
		else if (!stricmp(nname, pname))
			LogEvent(logwarn, 1, "Caseless match of two para format names: ",
				nname, " and ", pname);
	}
	if (((spos = FormNSPNameList.getitem(nspname, &nitem, false, false)) != 0)
	 && nitem
	 && ((nname = nitem->name) != NULL)
	 && (pos != spos)) {
		if (!strcmp(nname, nspname)) {
			nname = FormNameList.find(spos);
			LogEvent(logwarn, 1, "Spaceless match of two para format names: ",
				nname, " and ", pname);
		}
		else if (!stricmp(nname, nspname)) {
			nname = FormNameList.find(spos);
			LogEvent(logwarn, 1, "Spaceless and caseless match of two para format names: ",
				nname, " and ", pname);
		}
	}
}


void
MIFrd::PutPFProps(void)
{
	DCgroup *gp = GroupStack[MIFlevel];
  DCctl *cp = NULL;

	// in format def, name and format type already put out

	if (CurrPgf.fnext != NULL) {
		cp = new DCctl(imnem, 2, 209, 3, 1);
		gp->add(*cp);
		cp = new DCctl(ename, 2, 209, 4);
		cp->ex(CurrPgf.fnext);
		gp->add(*cp);
	}
	cp = new DCctl(imnem, 2, 31, CurrPgf.align, 0);
	gp->add(*cp);
	cp = new DCctl(ilong, 2, 32, 1, CurrPgf.find);
	gp->add(*cp);
	cp = new DCctl(ilong, 2, 32, 2, CurrPgf.lind);
	gp->add(*cp);
	cp = new DCctl(ilong, 2, 32, 3, CurrPgf.rind);
	gp->add(*cp);
	if (CurrPgf.pplace > 0) {
		cp = new DCctl(imnem, 2, 11, CurrPgf.pplace + 2, 1);
		gp->add(*cp);
	}
	if (CurrPgf.pstyle > 0) {
		if (CurrPgf.pstyle < 3)
			cp = new DCctl(imnem, 2, 35, 1, CurrPgf.pstyle + 1);
		else if (CurrPgf.pstyle < 6)
			cp = new DCctl(imnem, 2, 35, 2, CurrPgf.pstyle - 2);
		else if (CurrPgf.pstyle == 6)
			cp = new DCctl(imnem, 2, 35, 1, 4);  // run-in
		gp->add(*cp);
	}

	cp = new DCctl(ilong, 2, 22, 1, CurrPgf.sbval);
	gp->add(*cp);
	cp = new DCctl(ilong, 2, 22, 3, CurrPgf.saval);
	gp->add(*cp);
	cp = new DCctl(imnem, 2, 12, 1, CurrPgf.kprev);
	gp->add(*cp);
	cp = new DCctl(imnem, 2, 12, 2, CurrPgf.knext);
	gp->add(*cp);
	cp = new DCctl(ilong, 2, 12, 4, CurrPgf.kwid);
	gp->add(*cp);
	cp = new DCctl(ilong, 2, 12, 5, CurrPgf.kwid);  // orphans same as widows
	gp->add(*cp);

	// font info goes here
	PutFontProps();

	// tab info goes here
	if (CurrPgf.tcount > 0) {
		cp = new DCctl(group, 2, 40, CurrPgf.tcount);
		gp->add(*cp);
		(CurrPgf.tabs)->all(PutTabStop);
		cp = new DCctl(endgroup, 2, 40, CurrPgf.tcount);
		gp->add(*cp);
	}

	cp = new DCctl(ilong, 2, 21, CurrPgf.sltype, CurrPgf.slval);
	gp->add(*cp);
	cp = new DCctl(imnem, 2, 13, 1, CurrPgf.hyuse);
	gp->add(*cp);
	cp = new DCctl(ilong, 2, 13, 2, CurrPgf.hymline);
	gp->add(*cp);
	cp = new DCctl(ilong, 2, 13, 4, CurrPgf.hymbef);
	gp->add(*cp);
	cp = new DCctl(ilong, 2, 13, 5, CurrPgf.hymaft);
	gp->add(*cp);
	cp = new DCctl(ilong, 2, 13, 3, CurrPgf.hymword);
	gp->add(*cp);
	cp = new DCctl(ilong, 2, 34, 2, CurrPgf.cspls);
	gp->add(*cp);
	cp = new DCctl(ilong, 2, 33, 2, CurrPgf.wsmin);
	gp->add(*cp);
	cp = new DCctl(ilong, 2, 33, 1, CurrPgf.wsopt);
	gp->add(*cp);
	cp = new DCctl(ilong, 2, 33, 3, CurrPgf.wsmax);
	gp->add(*cp);
	cp = new DCctl(imnem, 2, 17, 2, CurrPgf.lang);
	gp->add(*cp);

	// table cell props here
	cp = new DCctl(ilong, 2, 57, 19, CurrPgf.tcalgn);
	gp->add(*cp);
	cp = new DCctl(ilong, 2, 57, 13, CurrPgf.cplmval);
	gp->add(*cp);
	cp = new DCctl(ilong, 2, 57, 14, CurrPgf.cprmval);
	gp->add(*cp);
	cp = new DCctl(ilong, 2, 57, 11, CurrPgf.cptmval);
	gp->add(*cp);
	cp = new DCctl(ilong, 2, 57, 12, CurrPgf.cpbmval);
	gp->add(*cp);
	cp = new DCctl(imnem, 2, 57, 17, CurrPgf.cplmtyp);
	gp->add(*cp);
	cp = new DCctl(imnem, 2, 57, 18, CurrPgf.cprmtyp);
	gp->add(*cp);
	cp = new DCctl(imnem, 2, 57, 15, CurrPgf.cptmtyp);
	gp->add(*cp);
	cp = new DCctl(imnem, 2, 57, 16, CurrPgf.cpbmtyp);
	gp->add(*cp);

	if (CurrPgf.runin != NULL)
		ProcPRunIn(CurrPgf.runin);

	// prefix/suffix info here
	ProcPFCont();  // add px and sx
}


void
MIFrd::PutPgfOverrides(pform *pf)
{
	DCgroup *gp = GroupStack[MIFlevel];
  DCctl *cp = NULL;
	bool skipit = false;

	// in format def, name and format type already put out

	if (CurrPgf.fnext != pf->fnext) {
		if (CurrPgf.fnext) {
			cp = new DCctl(imnem, 2, 209, 3, 1);
			gp->add(*cp);
			cp = new DCctl(ename, 2, 209, 4);
			cp->ex(CurrPgf.fnext);
		}
		else
			cp = new DCctl(imnem, 2, 209, 3, 0);
		gp->add(*cp);
	}

	if (CurrPgf.align != pf->align) {
		cp = new DCctl(imnem, 2, 31, CurrPgf.align, 0);
		gp->add(*cp);
	}

	if (CurrPgf.find != pf->find) {
		cp = new DCctl(ilong, 2, 32, 1, CurrPgf.find);
		gp->add(*cp);
	}

	if (CurrPgf.lind != pf->lind) {
		cp = new DCctl(ilong, 2, 32, 2, CurrPgf.lind);
		gp->add(*cp);
	}

	if (CurrPgf.rind != pf->rind) {
		cp = new DCctl(ilong, 2, 32, 3, CurrPgf.rind);
		gp->add(*cp);
	}

	if (CurrPgf.pplace != pf->pplace) {
		if (CurrPgf.pplace > 0)
			cp = new DCctl(imnem, 2, 11, CurrPgf.pplace + 2, 1);
		else
			cp = new DCctl(imnem, 2, 11, pf->pplace + 2, 0);
		gp->add(*cp);
	}

	if (CurrPgf.pstyle != pf->pstyle) {
		if (CurrPgf.pstyle > 0) {
			if (CurrPgf.pstyle < 3)
				cp = new DCctl(imnem, 2, 35, 1, CurrPgf.pstyle + 1);
			else if (CurrPgf.pstyle < 6)
				cp = new DCctl(imnem, 2, 35, 2, CurrPgf.pstyle - 2);
			else if (CurrPgf.pstyle == 6)
				cp = new DCctl(imnem, 2, 35, 1, 4);  // run-in
			else skipit = true;
		}
		else {
			if (pf->pstyle < 3)
				cp = new DCctl(imnem, 2, 35, 1, 0);
			else if (pf->pstyle < 6)
				cp = new DCctl(imnem, 2, 35, 2, 0);
			else if (pf->pstyle == 6)
				cp = new DCctl(imnem, 2, 35, 1, 0);
			else skipit = true;
		}
		if (skipit)
			skipit = false;
		else
			gp->add(*cp);
	}

	if (CurrPgf.sbval != pf->sbval) {
		cp = new DCctl(ilong, 2, 22, 1, CurrPgf.sbval);
		gp->add(*cp);
	}

	if (CurrPgf.saval != pf->saval) {
		cp = new DCctl(ilong, 2, 22, 3, CurrPgf.saval);
		gp->add(*cp);
	}

	if (CurrPgf.kprev != pf->kprev) {
		cp = new DCctl(imnem, 2, 12, 1, CurrPgf.kprev);
		gp->add(*cp);
	}

	if (CurrPgf.knext != pf->knext) {
		cp = new DCctl(imnem, 2, 12, 2, CurrPgf.knext);
		gp->add(*cp);
	}

	if (CurrPgf.kwid != pf->kwid) {
		cp = new DCctl(ilong, 2, 12, 4, CurrPgf.kwid);
		gp->add(*cp);
		cp = new DCctl(ilong, 2, 12, 5, CurrPgf.kwid);  // orphans same as widows
		gp->add(*cp);
	}

	// font info goes here
	PutFntOverrides(pf->fnt);

	// tab info goes here

	if ((CurrPgf.tcount != pf->tcount)
	 || (CurrPgf.tabs != pf->tabs)) {
		if (CurrPgf.tcount > 0) {
			cp = new DCctl(group, 2, 40, CurrPgf.tcount);
			gp->add(*cp);
			(CurrPgf.tabs)->all(PutTabStop);
			cp = new DCctl(endgroup, 2, 40, CurrPgf.tcount);
			gp->add(*cp);
		}
		else {
			cp = new DCctl(group, 2, 40, 0);
			gp->add(*cp);
			cp = new DCctl(endgroup, 2, 40, 0);
			gp->add(*cp);
		}
	}

	if (CurrPgf.sltype != pf->sltype) {
		cp = new DCctl(ilong, 2, 21, CurrPgf.sltype, CurrPgf.slval);
		gp->add(*cp);
	}

	if (CurrPgf.hyuse != pf->hyuse) {
		cp = new DCctl(imnem, 2, 13, 1, CurrPgf.hyuse);
		gp->add(*cp);
	}

	if (CurrPgf.hymline != pf->hymline) {
		cp = new DCctl(ilong, 2, 13, 2, CurrPgf.hymline);
		gp->add(*cp);
	}

	if (CurrPgf.hymbef != pf->hymbef) {
		cp = new DCctl(ilong, 2, 13, 4, CurrPgf.hymbef);
		gp->add(*cp);
	}

	if (CurrPgf.hymaft != pf->hymaft) {
		cp = new DCctl(ilong, 2, 13, 5, CurrPgf.hymaft);
		gp->add(*cp);
	}

	if (CurrPgf.hymword != pf->hymword) {
		cp = new DCctl(ilong, 2, 13, 3, CurrPgf.hymword);
		gp->add(*cp);
	}

	if (CurrPgf.cspls != pf->cspls) {
		cp = new DCctl(ilong, 2, 34, 2, CurrPgf.cspls);
		gp->add(*cp);
	}

	if (CurrPgf.wsmin != pf->wsmin) {
		cp = new DCctl(ilong, 2, 33, 2, CurrPgf.wsmin);
		gp->add(*cp);
	}

	if (CurrPgf.wsopt != pf->wsopt) {
		cp = new DCctl(ilong, 2, 33, 1, CurrPgf.wsopt);
		gp->add(*cp);
	}

	if (CurrPgf.wsmax != pf->wsmax) {
		cp = new DCctl(ilong, 2, 33, 3, CurrPgf.wsmax);
		gp->add(*cp);
	}

	if (CurrPgf.lang != pf->lang) {
		cp = new DCctl(imnem, 2, 17, 2, CurrPgf.lang);
		gp->add(*cp);
	}


	// table cell props here

	if (CurrPgf.tcalgn != pf->tcalgn) {
		cp = new DCctl(ilong, 2, 57, 19, CurrPgf.tcalgn);
		gp->add(*cp);
	}

	if (CurrPgf.cplmval != pf->cplmval) {
		cp = new DCctl(ilong, 2, 57, 13, CurrPgf.cplmval);
		gp->add(*cp);
	}

	if (CurrPgf.cprmval != pf->cprmval) {
		cp = new DCctl(ilong, 2, 57, 14, CurrPgf.cprmval);
		gp->add(*cp);
	}

	if (CurrPgf.cptmval != pf->cptmval) {
		cp = new DCctl(ilong, 2, 57, 11, CurrPgf.cptmval);
		gp->add(*cp);
	}

	if (CurrPgf.cpbmval != pf->cpbmval) {
		cp = new DCctl(ilong, 2, 57, 12, CurrPgf.cpbmval);
		gp->add(*cp);
	}

	if (CurrPgf.cplmtyp != pf->cplmtyp) {
		cp = new DCctl(imnem, 2, 57, 17, CurrPgf.cplmtyp);
		gp->add(*cp);
	}

	if (CurrPgf.cprmtyp != pf->cprmtyp) {
		cp = new DCctl(imnem, 2, 57, 18, CurrPgf.cprmtyp);
		gp->add(*cp);
	}

	if (CurrPgf.cptmtyp != pf->cptmtyp) {
		cp = new DCctl(imnem, 2, 57, 15, CurrPgf.cptmtyp);
		gp->add(*cp);
	}

	if (CurrPgf.cpbmtyp != pf->cpbmtyp) {
		cp = new DCctl(imnem, 2, 57, 16, CurrPgf.cpbmtyp);
		gp->add(*cp);
	}

	// run-in punct
	if ((CurrPgf.runin != pf->runin)
	 && (CurrPgf.runin != NULL)
	 && (*(CurrPgf.runin) != '\0'))
     ProcPRunIn(CurrPgf.runin);

	// prefix/suffix info here
	//if (CurrPgf.align != pf->align)
	ProcPFCont();  // add px and sx	again
}


void
MIFrd::PutTabStop(void *vp, long)
{
	if (vp == NULL)
		return;

	tab *tb = (tab *) vp;
	DCgroup *gp = GroupStack[MIFlevel];
	DCctl *cp = NULL;

	if (tb->tfill == 0) {
		cp = new DCctl(etext, 2, 42,  1);
		cp->ex(tb->tfstr);
	}
	else
		cp = new DCctl(ilong, 2, 42,  1, tb->tfill);
	gp->add(*cp);

	if (tb->talign != 0) {
		cp = new DCctl(ilong, 2, 42,  2, tb->talign);
		gp->add(*cp);
	}

	cp = new DCctl(ilong, 2, 41, tb->ttype, tb->tpos);
	gp->add(*cp);
}


void
MIFrd::ProcPPlace(mif *mp)
{
  DCctl *cp = NULL;

  if (MIFval == 0) {                                    // 0 means anywhere
    if (CurrPG == psg_pgf)                              // default for format
      return;
    cp = new DCctl(mp->dtype, mp->maj, mp->min, 8, 1);  // override of format
  }
  else cp = new DCctl(mp->dtype, mp->maj, mp->min, MIFval + 2, 1);
  GroupStack[MIFlevel]->add(*cp);
}


void
MIFrd::ProcPStyle(mif *mp)
{
  DCctl *cp = NULL;

  if (MIFval == 0) {                                    // 0 means normal
    if (CurrPG == psg_pgf)                              // default for format
      return;
    cp = new DCctl(mp->dtype, mp->maj, mp->min, 1, 1);  // override of format
  }
  else if (MIFval < 3)
		cp = new DCctl(mp->dtype, mp->maj, mp->min, 1, MIFval + 1);
  else if (MIFval < 6)
		cp = new DCctl(mp->dtype, mp->maj, mp->min, 2, MIFval - 2);
  else if (MIFval == 6)
		cp = new DCctl(mp->dtype, mp->maj, mp->min, 1, 4);  // run-in

	else return;
  GroupStack[MIFlevel]->add(*cp);
}


void
MIFrd::ProcPRunIn(char *punct)
{
	if ((punct == NULL)
	 || (*punct == '\0'))
		return;

  DCgroup *gp = new DCgroup(DCctl(group, 2, 35, 3));

  AddText(0);  // purge buffer
  GroupStack[++MIFlevel] = gp;

  for ( ; *punct; punct++) {
    if (*punct == '\\') {
      char ch = *++punct;
	    if (ch == '\\')  // make double backslash into single
				ch = *++punct;
      if (ch == 't')
        AddText('\t');
      else if (ch == 'x')
        ProcHex(&punct);
    }
		else if ((((unc) *punct) > 0x7F)
		 && (MIFversion > 6)
		 && ::IsUTF8((unc *) punct)) {
				AddText(::UTF8((unc **)&punct));
				punct--;
		}
	  else AddText(*punct);
  }

  AddText(0);
  GroupStack[--MIFlevel]->add(*gp);
}


void
MIFrd::ProcPBlock(mif *mp)
{
  DCctl *cp = new DCctl(mp->dtype, mp->maj, mp->min, 4, MIFval);
  GroupStack[MIFlevel]->add(*cp);
  cp = new DCctl(mp->dtype, mp->maj, mp->min, 5, MIFval);
  GroupStack[MIFlevel]->add(*cp);
}


uns MIFrd::PSpMode = 1;

void
MIFrd::ProcPSpVal(mif *mp)
{
  DCctl *cp = new DCctl(mp->dtype, mp->maj, mp->min, PSpMode, MIFval);
  GroupStack[MIFlevel]->add(*cp);
}



// tab processing


uns MIFrd::TSCount;
uns MIFrd::TSLeft;
uns MIFrd::TSType;
uns MIFrd::TSFill;
uns MIFrd::TSAlign;
long MIFrd::TSPos;

uns MIFrd::LineTabCount;
uns MIFrd::CurrTabFill;
uns MIFrd::CurrTabAlign;
bool MIFrd::GenericTab;
char *MIFrd::CurrTFString;

tab *MIFrd::CurrTab;
DCvlist *MIFrd::CurrTabList;
DCvlist MIFrd::TabFormatList;


void
MIFrd::ProcTSCount(mif *mp)
{
  if ((TSCount = (uns) MIFval) > 0) {
		if (!TagPF)
	    MakeCtl(mp);
	}
	CurrPgf.tcount = TSCount;
  TSLeft = TSCount;
  TSFill = 0;
  CurrTabFill = ' ';
  CurrTabAlign = '.';
  if (CurrTabList && (TabFormatList( (void *) CurrTabList) == 0))
    delete CurrTabList;
  CurrTabList = new DCvlist;
	CurrPgf.tabs = CurrTabList;
  if ((CurrPG == psg_pgf) 
	 || TagPF)
    TabFormatList.add( (void *) CurrTabList, FormatID);
  LineTabCount = 0;
}


void
MIFrd::EndTStop(void)
{
  assert(TSLeft);
  CurrTab = new tab;
  CurrTab->ttype = TSType;
  CurrTab->tpos = TSPos;
  CurrTab->talign = TSAlign;
  if ((CurrTab->tfill = CurrTabFill) == 0)
    CurrTab->tfstr = NewName(CurrTFString);
  CurrTabList->add( (void *) CurrTab, ++LineTabCount);

  if (--TSLeft == 0)
    LineTabCount = 0;

	if (!TagPF) {
		DCctl *cp = new DCctl(ilong, 2, 41, TSType, TSPos);
		GroupStack[MIFlevel]->add(*cp);
		if (TSLeft == 0) {
			cp = new DCctl(endgroup, 2, 40, TSCount);
			GroupStack[MIFlevel]->add(*cp);
		}
	}
}


void
MIFrd::ProcTSLead(mif *mp)
{
  if (strlen(MIFstring) == 1) {
    if (TSFill != *MIFstring) {
      TSFill = *MIFstring;
			if (!TagPF) {
	      DCctl *cp = new DCctl(ilong, mp->maj, mp->min, mp->dat, TSFill);
		    GroupStack[MIFlevel]->add(*cp);
			}
    }
  }
  else {
    if (CurrTFString)
      DeleteName(CurrTFString);
    CurrTFString = NewName(MIFstring);
		if (!TagPF)
	    MakeCtl(mp);
    TSFill = 0;
  }
  CurrTabFill = TSFill;
}


void
MIFrd::MakeTabCtl(void)
{
  DCctl *cp = NULL;
  tab *ct;

  DCgroup *gp = GroupStack[MIFlevel];

  if ((GenericTab == true) // in a format spec or
   || !CurrTabList) {      // no tabs set
    cp = new DCctl(ilong, 2, 41, 8);
    gp->add(*cp);
    return;
  }

  while ((ct = (tab *) CurrTabList->find(LineTabCount)) == NULL) {
		if (LineTabCount < 2) {  // first tab not found
			cp = new DCctl(ilong, 2, 41, 8); // mark as unknown
			gp->add(*cp);
			return;
		}
		else LineTabCount--;     // back up to last set tab
	}

  if (ct->tfill) {
    if (ct->tfill != CurrTabFill) {
      cp = new DCctl(ilong, 2, 42, 1, ct->tfill);
      gp->add(*cp);
      CurrTabFill = ct->tfill;
    }
  }
  else {
    if (!CurrTFString || strcmp(ct->tfstr, CurrTFString)) {
      cp = new DCctl(etext, 2, 42, 1);
      cp->ex(ct->tfstr);
      gp->add(*cp);
      if (CurrTFString)
        DeleteName(CurrTFString);
      CurrTFString = NewName(ct->tfstr);
    }
  }
  cp = new DCctl(ilong, 2, 41, ct->ttype, ct->tpos);
  gp->add(*cp);
}



// font processing


void
MIFrd::ProcCForm(mif *mp)
{
  if (CurrPG == psg_font) {  // defining format
	  DCgroup *gp = new DCgroup(DCctl(mp->dtype, mp->maj, mp->min, ++FormatID));
    GroupList[psg_font]->add(*gp);
    GroupStack[MIFlevel] = gp;
		CurrGrCForm = new grcform;
		GraphTextForm = true;
  }
  else {
  // otherwise let contents percolate up to enclosing group
    FlushText();
    if (GraphTextStart) {
			GraphTextFontStart = true;
      StartGraphText();
		}
  }

  SetEFont();
}


void
MIFrd::ProcCTag(mif *mp)
{
  DCctl *cp = NULL;
	DCgroup *gp = GroupStack[MIFlevel];

  if (CurrPG == psg_font) {  // defining format, get dcl ID from global
		CheckCharFormName(MIFstring);
    CFormNameList.addnew(MIFstring, FormatID);
    CFormNSPNameList.addnew(NoSpace(MIFstring), FormatID);
    FormTypeList.add(2, FormatID);
    MakeCtl(mp);
    cp = new DCctl(imnem, 2, 81, 2, 2);
    gp->add(*cp);
  }
  else {                    // using format, get dcl ID from list
    if (*MIFstring == 0) {
			if (GraphicText) {   // update props from default format
				//SetDefGrTextProps();
				if (GraphTextFontStart) {
					PutGrTextProps(gp);
					GraphTextFontStart = false;
				}
				return;
			}
      else if (!InPgf) {
				CurrFnt = DefPgfFnt;
        cp = new DCctl(imnem, 2, 82, 2, 0);
        gp->add(*cp);
				Inheriting = false;
      }
			SymFont = CurrFnt.sym;
      return;
    }
		if (Inheriting) {
      cp = new DCctl(imnem, 2, 82, 2, 0);
      gp->add(*cp);
			Inheriting = false;
		}
    MIFval = CFormNameList.getnum(MIFstring);
		//Inheriting = true;
    if (MIFval) {
      cp = new DCctl(imnem, 2, 82, (uns) FormTypeList.find(MIFval), MIFval);
      gp->add(*cp);
			if (CFTagList.find(MIFval))
				CFTagUsed = true;
			if (GraphicText) {       // update props from this format
				if (GetGrCForm(MIFval)) {
					PutGrTextProps(gp);
					GraphTextFontStart = false;
				}
				return;
			}
			SetSymFont(MIFval);
    }
    else {  // add tag on fly
			if (UndefCharForms.getitem(MIFstring) == 0) {
				UndefCharForms.add(MIFstring, ++UndefCharFormsCount);
				LogEvent(logwarn, 1, "Character format not defined in catalog: ",
					MIFstring);
			}
	    DCgroup *fgp = new DCgroup(DCctl(group, 2, 80, ++FormatID));
		  GroupList[psg_font]->add(*fgp);
      TagCF = true;
      cp = new DCctl(imnem, 2, 82, 2, FormatID);
	    GroupStack[MIFlevel]->add(*cp);	// put in call to format in text
			GroupStack[MIFlevel] = fgp;	// switch to format definition group
	    CFormNameList.addnew(MIFstring, FormatID);
			CFTagList.add(FormatID);
	    FormTypeList.add(2, FormatID);
		  MakeCtl(mp);								// create format name ctl
			cp = new DCctl(imnem, 2, 81, 2, 2);	// specify inherit format
      fgp->add(*cp);  // remain in format def group for properties
			GroupStack[MIFlevel] = GroupStack[MIFlevel - 1];  // no, don't
			TagCF = false;  // make format all as-is so tag retained
    }
  }
}


DCnlist MIFrd::UndefCharForms;
long MIFrd::UndefCharFormsCount = 0;


void
MIFrd::CheckCharFormName(char *pname)
{
	long pos = 0;
	long spos = 0;
	long ppos = 0;
	long sppos = 0;
	DCnlist *nitem = NULL;
	const char *nname = NULL;
	char *nspname = NoSpace(pname);

	if (((pos = CFormNameList.getitem(pname, &nitem, false, false)) != 0)
	 && nitem
	 && ((nname = nitem->name) != NULL)) {
		if (!strcmp(nname, pname))
			LogEvent(logwarn, 1, "Exact match of two char format names: ",
				nname, " and ", pname);
		else if (!stricmp(nname, pname))
			LogEvent(logwarn, 1, "Caseless match of two char format names: ",
				nname, " and ", pname);
	}
	if (((spos = CFormNSPNameList.getitem(nspname, &nitem, false, false)) != 0)
	 && nitem
	 && ((nname = nitem->name) != NULL)
	 && (pos != spos)) {
		if (!strcmp(nname, nspname)) {
			nname = CFormNameList.find(spos);
			LogEvent(logwarn, 1, "Spaceless match of two char format names: ",
				nname, " and ", pname);
		}
		else if (!stricmp(nname, nspname)) {
			nname = CFormNameList.find(spos);
			LogEvent(logwarn, 1, "Spaceless and caseless match of two char format names: ",
				nname, " and ", pname);
		}
	}
	if (((ppos = FormNameList.getitem(pname, &nitem, false, false)) != 0)
	 && nitem
	 && ((nname = nitem->name) != NULL)) {
		if (!strcmp(nname, pname))
			LogEvent(logwarn, 1, "Exact match of char format name ",
				pname, " and para format name ", nname);
		else if (!stricmp(nname, pname))
			LogEvent(logwarn, 1, "Caseless match of char format name ",
				pname, " and para format name ", nname);
	}
	if (((sppos = FormNSPNameList.getitem(nspname, &nitem, false, false)) != 0)
	 && nitem
	 && ((nname = nitem->name) != NULL)
	 && (ppos != sppos)) {
		if (!strcmp(nname, nspname)) {
			nname = FormNameList.find(sppos);
			LogEvent(logwarn, 1, "Spaceless match of char format name ",
				pname, " and para format name ", nname);
		}
		else if (!stricmp(nname, nspname)) {
			nname = FormNameList.find(sppos);
			LogEvent(logwarn, 1, "Spaceless and caseless match of char format name ",
				pname, " and para format name ", nname);
		}
	}
}


void
MIFrd::ProcFForm()
{
	// PgfFont start, if before AFrames is candidate for aframe default
#if 0
  if (AFFontDef == false)	{
		if (CurrGrCForm == NULL)
			CurrGrCForm = new grcform;
		else memset(CurrGrCForm, 0, sizeof(grcform));
		GraphTextForm = true;
	}
#endif
  // let contents connect to enclosing group
  SetEFont();
}


void
MIFrd::SetEFont()
{
  // set up for font ctls
  FFamily[0] = FVar[0] = FWeight[0] = FAngle[0] = '\0';
  FFNum = FVNum = FWNum = FANum = 0;
  FBaseSet = FStyleSet = FCapsSet = FRevSet = FLineSet = false;
	FBase = FStyle = FCaps = FRev = FLine = 0;
  EndProc[MIFlevel] = EndFont;
}


void
MIFrd::PutFontProps(void)
{
	DCgroup *gp = GroupStack[MIFlevel];
  DCctl *cp = NULL;

  cp = new DCctl(ilong, 2, 15, 2, CurrFnt.fsize); // font size ctl
	gp->add(*cp);
  cp = new DCctl(imnem, 2, 34, 1, CurrFnt.kern);  // font pair kern ctl
	gp->add(*cp);
  cp = new DCctl(imnem, 2, 34, 3, CurrFnt.spread);  // font track kern ctl
	gp->add(*cp);
  cp = new DCctl(imnem, 2, 15, 13, CurrFnt.color);  // font color ctl
	gp->add(*cp);

	if (CurrFnt.fnum)   // font name ctl for number
		cp = new DCctl(imnem, 2, 15, 1, CurrFnt.fnum);
	else if (CurrFnt.fname) {	    // ctl for string
		cp = new DCctl(ename, 2, 15, 1);
		cp->ex(CurrFnt.fname);
	}
	gp->add(*cp);
  cp = new DCctl(imnem, 2, 15, 3, CurrFnt.compr);  // font compression ctl
	gp->add(*cp);
  cp = new DCctl(imnem, 2, 15, 4, CurrFnt.weight);  // font weight ctl
	gp->add(*cp);
  cp = new DCctl(imnem, 2, 15, 5, CurrFnt.angle);  // font angle ctl
	gp->add(*cp);
  cp = new DCctl(imnem, 2, 15, 6, CurrFnt.basel);  // font baseline ctl
	gp->add(*cp);
  cp = new DCctl(imnem, 2, 15, 7, CurrFnt.caps);  // font capitalization ctl
	gp->add(*cp);
  cp = new DCctl(imnem, 2, 15, 8, CurrFnt.style); // font style ctl
	gp->add(*cp);
  cp = new DCctl(imnem, 2, 15, 9, CurrFnt.rev);   // font revision ctl
	gp->add(*cp);
  cp = new DCctl(imnem, 2, 15, 11, CurrFnt.lining); // font underscore ctl
	gp->add(*cp);
}


void
MIFrd::PutFntOverrides(cform *cf)
{
	DCgroup *gp = GroupStack[MIFlevel];
  DCctl *cp = NULL;

	if (CurrFnt.fsize != cf->fsize) {
	  cp = new DCctl(ilong, 2, 15, 2, CurrFnt.fsize); // font size ctl
		gp->add(*cp);
	}

	if (CurrFnt.kern != cf->kern) {
		cp = new DCctl(imnem, 2, 34, 1, CurrFnt.kern);  // font pair kern ctl
		gp->add(*cp);
	}

	if (CurrFnt.spread != cf->spread) {
		cp = new DCctl(imnem, 2, 34, 3, CurrFnt.spread);  // font track kern ctl
		gp->add(*cp);
	}

	if (GraphicText
	 || (CurrFnt.color != cf->color)) {
		cp = new DCctl(imnem, 2, 15, 13, CurrFnt.color);  // font color ctl
		gp->add(*cp);
	}

	if ((CurrFnt.fnum != cf->fnum)
	 || (CurrFnt.fname != cf->fname)) {
		if (CurrFnt.fnum)   // font name ctl for number
			cp = new DCctl(imnem, 2, 15, 1, CurrFnt.fnum);
		else if (CurrFnt.fname   // ctl for string
		 && ((cf->fname == NULL)
		  || stricmp(CurrFnt.fname, cf->fname))) {
			cp = new DCctl(ename, 2, 15, 1);
			cp->ex(CurrFnt.fname);
		}
		gp->add(*cp);
	}

	if (CurrFnt.compr != cf->compr) {
		cp = new DCctl(imnem, 2, 15, 3, CurrFnt.compr);  // font compression ctl
		gp->add(*cp);
	}

	if (CurrFnt.weight != cf->weight) {
		cp = new DCctl(imnem, 2, 15, 4, CurrFnt.weight);  // font weight ctl
		gp->add(*cp);
	}

	if (CurrFnt.angle != cf->angle) {
		cp = new DCctl(imnem, 2, 15, 5, CurrFnt.angle);  // font angle ctl
		gp->add(*cp);
	}

	if (CurrFnt.basel != cf->basel) {
		cp = new DCctl(imnem, 2, 15, 6, CurrFnt.basel);  // font baseline ctl
		gp->add(*cp);
	}

	if (CurrFnt.caps != cf->caps) {
		cp = new DCctl(imnem, 2, 15, 7, CurrFnt.caps);  // font capitalization ctl
		gp->add(*cp);
	}

	if (CurrFnt.style != cf->style) {
		cp = new DCctl(imnem, 2, 15, 8, CurrFnt.style); // font style ctl
		gp->add(*cp);
	}

	if (CurrFnt.rev != cf->rev) {
		cp = new DCctl(imnem, 2, 15, 9, CurrFnt.rev);   // font revision ctl
		gp->add(*cp);
	}

	if (CurrFnt.lining != cf->lining) {
		cp = new DCctl(imnem, 2, 15, 11, CurrFnt.lining); // font underscore ctl
		gp->add(*cp);
	}
}


void
MIFrd::ProcVPos(mif *mp)
{
  if (MIFval) {
		if (ConvertToEms) {  // val is in points * 100
			// convert to percent * 100 of current point size
			MIFval = (MIFval * 10000) / CurrFnt.fsize;
		}
    DCctl *cp = new DCctl(mp->dtype, mp->maj, mp->min,
                          (MIFval > 0) ? 1 : 2, abs(MIFval));
    GroupStack[MIFlevel]->add(*cp);
  }
}


void
MIFrd::ProcHPos(mif *mp)
{
  if (MIFval) {
		if (ConvertToEms) {  // val is in points * 100
			// convert to percent * 100 of current point size
			MIFval = (MIFval * 10000) / CurrFnt.fsize;
		}
    DCctl *cp = new DCctl(mp->dtype, mp->maj, mp->min,
                          (MIFval > 0) ? 1 : 2, abs(MIFval));
    GroupStack[MIFlevel]->add(*cp);
  }
}


char MIFrd::FFamily[FStringMax];
char MIFrd::FVar[FStringMax];
char MIFrd::FWeight[FStringMax];
char MIFrd::FAngle[FStringMax];
char MIFrd::FEncode[FStringMax];

char MIFrd::FName[FStringMax];
uns MIFrd::FFNum;
uns MIFrd::FVNum;
uns MIFrd::FWNum;
uns MIFrd::FANum;
uns MIFrd::FENum;

nvlist MIFrd::FontFamilies[] = {
  { "Times", 2 },
  { "Helvetica", 3 },
  { "Courier", 4 },
  { "AvantGarde", 5 },
  { "Bookman", 6 },
  { "Garamond", 7 },
  { "Korinna", 11 },
  { "NewCenturySchlbk", 12 },
  { "Palatino", 13 },
  { "ZapfChancery", 14 },
  { "ZapfDingbats", 15 },
  { "Symbol", 16 },
	{ "Webdings", 17 },
  { NULL, 0 }
};

nvlist MIFrd::FontVars[] = {
  { "Regular", 0 },
  { "UltraCompressed", 1 },
  { "ExtraCompressed", 1 },
  { "Compressed", 1 },
  { "Condensed", 2 },
  { "Narrow", 3 },
  { "Wide", 4 },
  { "Poster", 5 },
  { "Expanded", 5 },
  { NULL, 0 }
};

nvlist MIFrd::FontWeights[] = {
  { "Book", 0 },
  { "Regular", 0 },
  { "Thin", 1 },
  { "ExtraLight", 1 },
  { "SemiLight", 2 },
  { "Light", 2 },
  { "SemiBold", 3 },
  { "DemiBold", 3 },
  { "Bold", 4 },
  { "ExtraBold", 5 },
  { "Heavy", 6 },
  { "Black", 6 },
  { NULL, 0 }
};

nvlist MIFrd::FontAngles[] = {
  { "Regular", 0 },
  { "Kursiv", 1 },
  { "Slanted", 1 },
  { "Oblique", 1 },
  { "Italic", 1 },
  { "Obliqued", 1 },
  { NULL, 0 }
};

nvlist MIFrd::FontEncoding[] = {
  { "FrameRoman", 1 },
	{ "JISX0208.ShiftJIS", 2 },
	{ "GB2312-80.EUC", 3 },
	{ "BIG5", 4 },
	{ "KSC5601-1992", 5 },
  { NULL, 0 }
};



void
MIFrd::ProcFPlat()
{
  char *per = ".";

  char *cp = strtok(MIFstring, per); // platform ID
  if ((*cp != 'W') && (*cp != 'M'))
    return; // windows or mac, otherwise junk

  cp = strtok(NULL, per); // font family name
  if (!cp)
		return;
	strcpy(FFamily, cp);
  FFNum = 0;
  nvlist *f = FontFamilies;
  for ( ; f->n != NULL; f++) {
    if (!strcmp(f->n, cp)) {
      FFNum = (uns) f->v;
      break;
    }
  }

  cp = strtok(NULL, per); // font italic flag
  if (!cp)
    return;
	strcpy(FAngle, cp);
	FANum = 0;   // regular
  if (*cp == 'I')
		FANum = 1; // unless italic

  cp = strtok(NULL, per); // font weight value
  if (!cp)
    return;
	strcpy(FWeight, cp);
	FWNum = 0;
	if (isdigit(*cp)) {  // windows weight number
    if ((FWNum = atoi(cp)) < 200)
      FWNum = 1;  // thin
    else if (FWNum < 300)
			FWNum = 2;  // light
		else if (FWNum < 500)
      FWNum = 0;  // regular
    else if (FWNum < 600)
      FWNum = 3;  // demi
    else if (FWNum < 800)
      FWNum = 4;  // bold
    else if (FWNum < 900)
      FWNum = 5;  // extra
    else
      FWNum = 6;  // black
	}
	else {
		f = FontWeights;
		for ( ; f->n != NULL; f++) {
			if (!strcmp(f->n, cp)) {
				FWNum = (uns) f->v;
				break;
			}
		}
  }

  cp = strtok(NULL, per); // font variation name
  if (!cp)
		return;
	strcpy(FVar, cp);
  FVNum = 0;
  f = FontVars;
  for ( ; f->n != NULL; f++) {
    if (!strcmp(f->n, cp)) {
      FVNum = (uns) f->v;
      break;
    }
  }
}


void
MIFrd::ProcFString(mif *mp)
{
  nvlist *f;
  char *cp;
  uns *vp;

  switch (mp->dat) {
    case 1:
		case 15:
      f = FontFamilies;
      cp = FFamily;
      vp = &FFNum;
      break;
    case 3:
      f = FontVars;
      cp = FVar;
      vp = &FVNum;
      break;
    case 4:
      f = FontWeights;
      cp = FWeight;
      vp = &FWNum;
      break;
    case 5:
      f = FontAngles;
      cp = FAngle;
      vp = &FANum;
      break;
		case 0:
			f = FontEncoding;
			cp = FEncode;
			vp = &FENum;
			break;
    default:
      //assert(0); triggered by CombinedFontName, case 16
      return;
  }
  strncpy(cp, MIFstring, FStringMax);

  for ( ; f->n != NULL; f++) {
    if (!strcmp(f->n, cp)) {
      *vp = (uns) f->v;
      return;
    }
  }
  *vp = 0;
}


uns MIFrd::FBase;
uns MIFrd::FStyle;
uns MIFrd::FCaps;
uns MIFrd::FRev;
uns MIFrd::FLine;

bool MIFrd::FBaseSet;
bool MIFrd::FStyleSet;
bool MIFrd::FCapsSet;
bool MIFrd::FRevSet;
bool MIFrd::FLineSet;

bool MIFrd::SymFont;
bool MIFrd::NoSymMap = false;
bool MIFrd::Asian = false;


void
MIFrd::ProcFBool(mif *mp)
{
  uns val = mp->dat % 10;
  uns type = mp->dat / 10;
  uns *sp;

  switch (type) {
    case 6:  // sub or sup
      sp = &FBase;
      FBaseSet = true;
      break;
    case 7:  // upper, lower, or small
      sp = &FCaps;
      FCapsSet = true;
      break;
    case 8:  // shadow or outline
      sp = &FStyle;
      FStyleSet = true;
      break;
    case 9:  // change or strike
      sp = &FRev;
      FRevSet = true;
      break;
    case 11: // unl or ovl
      sp = &FLine;
      FLineSet = true;
      break;
    default:
      assert(0);
      break;
  }
  if (MIFval)
    *sp = val;
  else if (*sp == val)
    *sp = 0;
}


void
MIFrd::EndFont()
{
  DCctl *cp = NULL;
  DCgroup *gp = GroupStack[MIFlevel];

	bool writing = (TagCF || (!TagPF) || GraphTextFontStart || CFTagUsed);

  // put out the combined properties, name first
  if (*FFamily) {
    if (FFNum) {
      SymFont = ((FFNum == 15) || (FFNum == 16) || (FFNum == 17));
      if (FFNum == 3) {  // helvetica variations
        if (FVNum == 3)
          FFNum = 10;
        else if (FWNum == 2)
          FFNum = 9;
        else if (FWNum == 6)
          FFNum = 8;
      }
			if (writing)
	      cp = new DCctl(imnem, 2, 15, 1, FFNum);  // font name ctl for number
			CurrFnt.fnum = FFNum;
			CurrFnt.fname = NULL;
			CurrFnt.sym = SymFont;
			if (GraphTextForm) {
				CurrGrCForm->fnum = FFNum;
				CurrGrCForm->fname = NULL;
				CurrGrCForm->fsym = SymFont;
				CurrGrCForm->fsymset = true;
			}
			else if (GraphicText) {
				CurrGrTextProp.fnum = FFNum;
				CurrGrTextProp.fname = NULL;
			}
    }
    else {
      strcpy(FName, FFamily);
      if (*FVar
       && strcmp(FVar, "Regular"))
        strcat(FName, FVar);
			if (writing) {
	      cp = new DCctl(ename, 2, 15, 1);  // font name ctl for string
			  cp->ex(FName);
			}
			CurrFnt.fnum = 0;
			CurrFnt.fname = NewName(FName);
			if (GraphTextForm)
				CurrGrCForm->fname = NewName(FName);
			else if (GraphicText) {
				if (CurrGrTextProp.fname != NULL)
					DeleteName(CurrGrTextProp.fname);
				CurrGrTextProp.fname = NewName(FName);
			}
			if (!_strnicmp(FFamily, "Wingdings", 9))
				SymFont = true;
			else if (IniFile) {
				char *fnm = FFamily;
				if (IniFile->Section("Fonts")
				 && IniFile->Find(FFamily))
					fnm = IniFile->StrRight();
				if (IniFile->Section("FontTypes")
				 && IniFile->Find(fnm)
				 && (IniFile->IntRight() > 4))
					SymFont = true;
				else SymFont = false;
			}
			CurrFnt.sym = SymFont;
			if (GraphTextForm) {
				CurrGrCForm->fsym = SymFont;
				CurrGrCForm->fsymset = true;
			}
	  }
		if (writing)
	    gp->add(*cp);
  }

  if (*FVar) {
		if (writing)
	    cp = new DCctl(imnem, 2, 15, 3, FVNum);  // font compression ctl
		CurrFnt.compr = (uns) FVNum;
		if (writing)
	    gp->add(*cp);
  }
  if (*FWeight) {
		if (writing)
	    cp = new DCctl(imnem, 2, 15, 4, FWNum);  // font weight ctl
		CurrFnt.weight = (uns) FWNum;
		if (writing)
	    gp->add(*cp);
		if (GraphTextForm) {
			CurrGrCForm->fbold = FWNum;
			CurrGrCForm->fbset = true;
		}
		else if (GraphicText)
			CurrGrTextProp.fbold = FWNum;
  }
  if (*FAngle) {
		if (writing)
	    cp = new DCctl(imnem, 2, 15, 5, FANum);  // font angle ctl
		CurrFnt.angle = (uns) FANum;
		if (writing)
	    gp->add(*cp);
		if (GraphTextForm) {
			CurrGrCForm->fital = FANum;
			CurrGrCForm->fiset = true;
		}
		else if (GraphicText)
			CurrGrTextProp.fital = FANum;
  }

	if (writing) {
		long fs = 0;
		if (GraphTextForm)
			fs = CurrGrCForm->fsize;
		else if (GraphicText)
			fs = CurrGrTextProp.fsize;
		else if (CFTagUsed)
			fs = CurrFnt.fsize;
		if (fs != 0) {
			cp = new DCctl(ilong, 2, 15, 2, fs); // font size ctl
			gp->add(*cp);
		}
	}

  if (FBaseSet) {
		if (writing)
	    cp = new DCctl(imnem, 2, 15, 6, FBase);  // font baseline ctl
		CurrFnt.basel = (uns) FBase;
		if (writing)
	    gp->add(*cp);
  }
  if (FCapsSet) {
		if (writing)
	    cp = new DCctl(imnem, 2, 15, 7, FCaps);  // font capitalization ctl
		CurrFnt.caps = (uns) FCaps;
		if (writing)
	    gp->add(*cp);
  }
  if (FStyleSet) {
		if (writing)
	    cp = new DCctl(imnem, 2, 15, 8, FStyle); // font style ctl
		CurrFnt.style = (uns) FStyle;
		if (writing)
	    gp->add(*cp);
  }
  if (FRevSet) {
		if (writing)
	    cp = new DCctl(imnem, 2, 15, 9, FRev);   // font revision ctl
		CurrFnt.rev = (uns) FRev;
		if (writing)
	    gp->add(*cp);
		if (GraphTextForm) {
			CurrGrCForm->fstrike = FRev;
			CurrGrCForm->fsset = true;
		}
		else if (GraphicText)
			CurrGrTextProp.fstrike = FRev;
  }
  if (FLineSet) {
		if (writing)
	    cp = new DCctl(imnem, 2, 15, 11, FLine); // font underscore ctl
		CurrFnt.lining = (uns) FLine;
		if (writing)
	    gp->add(*cp);
		if (GraphTextForm) {
			CurrGrCForm->fundl = FLine;
			CurrGrCForm->fuset = true;
		}
		else if (GraphicText)
			CurrGrTextProp.fundl = FLine;
  }

	if (writing) {
		long color = 0;
		if (GraphicText)
			color = CurrGrTextProp.color;
		else if (CFTagUsed)
			color = CurrFnt.color;
		if (color) {
			cp = new DCctl(imnem, 2, 15, 13, color); // font color
			gp->add(*cp);
		}
	}

	if (CFTagUsed) {
		cp = new DCctl(imnem, 2, 34, 1, CurrFnt.kern);  // font pair kern ctl
		gp->add(*cp);
		cp = new DCctl(imnem, 2, 34, 3, CurrFnt.spread);  // font track kern ctl
		gp->add(*cp);
	}

	if (GraphTextForm) {
		if (CurrPG == psg_font)
			GCFormList.add(CurrGrCForm, FormatID);
#if 0
		else if (CurrPG == psg_aframe) {	// use first one as default
			if (AFFontDef == false) {
				grcform *gcf = new grcform;
				*gcf = *CurrGrCForm;
				GCFormList.add(gcf, -1);
				AFFontDef = true;
			}
			GetGrCForm(-1);
		}
#endif
		GraphTextForm = false;
	}

  if (((CurrPG == psg_pgf)
	  || (CurrPG == psg_font)
	  || TagCF) 
   && !FormSymList.find(FormatID))
		FormSymList.add((*FFamily ? (SymFont ? 3 : 2) : 1), FormatID);

	TagCF = false;
	CFTagUsed = false;
}


void
MIFrd::SetSymFont(long fval)
{
	long val = FormSymList.find(fval);
	if (val > 1)
		SymFont = (val == 3) ? true : false;
}


void
MIFrd::PutFntProp(void *vp, long)
{
	if (vp == NULL)
		return;

	DCctl *cp = (DCctl *) vp;
	GroupStack[MIFlevel]->add(*cp);
}




// page processing

uns MIFrd::PageID = 0;
uns MIFrd::LastMasterPage = 0;
uns MIFrd::FrameID = 0;
uns MIFrd::FirstPageFrame;

bool MIFrd::DoubleSided;
bool MIFrd::FirstRight;

long MIFrd::PageWide = 0;
long MIFrd::PageHigh = 0;

uns MIFrd::PageHand = 0;
uns MIFrd::MasterType = 0;
uns MIFrd::LeftMaster = 0;
uns MIFrd::RightMaster = 0;

DCilist MIFrd::FrameIDList;  // MIF id (CurrID) indexed by DCL id (FrameID)

DCnlist MIFrd::FrameNameList; // reference frame names indexed by FrameID
DCnlist MIFrd::PageNameList;  // master and ref page names indexed by PageID
DCilist MIFrd::PageNumList;   // body page numbers indexed by PageID
DClist MIFrd::PageDoneList;   // body page numbers put out already
DCilist MIFrd::MasterTypeList;  // master type indexed by PageID

// layout text ctls of TextRects indexed by FrameID, for fixup by ProcTRect
DCclist MIFrd::FrameTextList;

// layout text ctls of TextRects indexed by MIF id of next TextRect,
DCclist MIFrd::FrameNextList;  // for fixup (to DCL id) by EndTFrame

// the List is used by ProcTRect to find what page the current text is on
// the Indexes are used when writing DCL page by page at end
DCilist MIFrd::FramePageList;  // PageID indexed by FrameID of frame on page
DCclist MIFrd::PageFrameIndex; // page psg DCgroup indexed by PageID
DCclist MIFrd::PageUsageIndex; // page usage ctl indexed by PageID
DCclist MIFrd::FrameUsageIndex; // frame usage ctl indexed by FrameID
DCclist MIFrd::PageTextIndex;  // text stream group indexed by PageID
DCclist MIFrd::PagelessAFTextIndex;  // AF text groups indexed by AFrameID
DCclist MIFrd::PagelessAFTextStart;  // AF text start ctls indexed by AFrameID
DCclist MIFrd::PagelessTextIndex;  // loose text groups indexed by FrameID
DCclist MIFrd::NestedAFIndex;  // AF anchors in text indexed by containing AF
DCclist MIFrd::NestedAFTextID;  // AF anchors in text indexed by their TextID

DClist MIFrd::PageHeaderList;  // PageIDs for which headers have been found
DClist MIFrd::PageFooterList;  // PageIDs for which footers have been found
DClist MIFrd::FrameHeaderList;  // FrameIDs identified as possible headers
DClist MIFrd::FrameFooterList;  // FrameIDs identified as possible footers

DCclist MIFrd::MPFrameMTList;  // master frame type ctl indexed by FrameID
DCilist *MIFrd::MPFrameTList;   // master frame top pos indexed by FrameID
DCilist *MIFrd::MPFrameHList;   // master frame height indexed by FrameID

DCilist MIFrd::FrameAFList;  // AFrameID indexed by FrameID of frame in AFrame
DCilist MIFrd::PageAFList;   // PageID indexed by AFrameID of AFrame on page


void
MIFrd::ProcPage(mif *mp)
{
  DCgroup *gp = new DCgroup(DCctl(mp->dtype, 0, mp->min, 0)); // pseudogroup
  //assert(MIFlevel == 1);
  GroupStack[MIFlevel] = gp;
  CurrPG = psg_page;
  GroupList[psg_page]->add(*gp);
  HiddenPage = false;
	MasterType = 0;
	PageWide = 0;
	PageHigh = 0;
  FirstPageFrame = FrameID + 1;
  EndProc[MIFlevel] = EndPage;
}


void
MIFrd::EndPage(void)
{
	// use only for page layout masters
	if ((MasterType == 0)
	 || (MasterType > 2))
		return;

  uns i;
  long ht;
	long top;
  long fhigh = 0;
  long ftop = 0;
  uns highest = 0;

  for (i = FirstPageFrame; i <= FrameID; i++) {	 // find the highest frame
    if ((ht = MPFrameHList->find(i)) > fhigh) {	 // assuming it is the body
      highest = i;
			fhigh = ht;
      ftop = MPFrameTList->find(i);							 // and note its top pos
    }
  }
  for (i = FirstPageFrame; i <= FrameID; i++) {	 // scan the other frames
    if (i == highest)														 // skipping the body one
      continue;
    ht = MPFrameHList->find(i);
    top = MPFrameTList->find(i);
    if ((top < ftop)                 // if starts above body
     && ((top + (ht / 2)) <= ftop))  // and it is half above top of body
	    FrameHeaderList.add(i);        // must be a header
    else if ((top >= (ftop + fhigh)) // but if its top is below body
		 && ((top + ht) < PageHigh))     // and it fits on the page
      FrameFooterList.add(i);        // must be a footer
  }
  delete MPFrameHList;
  delete MPFrameTList;
}


void
MIFrd::ProcPageType(mif *mp)
{
  DCctl *cp = NULL;
  DCgroup *gp = GroupStack[MIFlevel];  // collect from stack

  switch (MIFval) {
    case 0:  // body
      gp->dat(++PageID);
      PageHand = DoubleSided ?
                 ((FirstRight ^ ((PageID - LastMasterPage) & 1)) ? 2 : 1) : 0;
      MasterType = 0;
      cp = new DCctl(imnem, 3, 11, 2, 0);
      PageUsageIndex.add(cp, PageID);
      gp->add(*cp);
      break;
    case 1:  // hidden
      // leave dat() ID 0 to identify
      HiddenPage = true;
      return;
    case 2:  // reference
      gp->dat(++PageID);
      PageHand = 0;
      MasterType = 3;
      break;
    case 3:  // other master
      gp->dat(++PageID);
      PageHand = 0;
      MasterType = 2;
      break;
    case 4:  // right master
      gp->dat(++PageID);
      PageHand = 1;
      if (!RightMaster)
        RightMaster = PageID;
      MasterType = 1;
      break;
    case 5:  // left master
      gp->dat(++PageID);
      PageHand = 2;
      if (!LeftMaster)
        LeftMaster = PageID;
      MasterType = 1;
      break;
    default: // wrong
      assert(0);
			LogEvent(logwarn, 2, "Invalid page type");
      return;
  }

  PageFrameIndex.add(gp, PageID);
  if (MasterType) {
    LastMasterPage = PageID;
    MasterTypeList.add(MasterType, PageID);
		if (MasterType < 3) { // page layout master page
		  MPFrameHList = new DCilist;
			MPFrameTList = new DCilist;
		}
  }
  cp = new DCctl(imnem, 3, 11, 3, MasterType);
  gp->add(*cp);
  MIFval = PageHand;
  MakeCtl(mp);  // handedness
}


void
MIFrd::ProcPageName(mif *mp)
{
  if (HiddenPage)
    return;

  MakeCtl(mp);
  if (PageID != LastMasterPage)
    return;
  PageNameList.addnew(MIFstring, PageID);
}


void
MIFrd::ProcTFrame(mif *mp)
{
  if (HiddenPage)
    return;

  StartGraphic();  // ensures correct operation if nested in graphic frame
  DCgroup *gp = new DCgroup(DCctl(mp->dtype, mp->maj, mp->min, ++FrameID));
  GroupStack[MIFlevel] = gp;       // make text frame the current group

  TAngle = 0;
	GraphTextSeq = false;
  EndProc[MIFlevel] = EndTFrame;
}


void
MIFrd::EndTFrame(void)
{
  DCctl *cp = NULL;

  FrameIDList.add(CurrID, FrameID);  // frame mif ID indexed by dcl FrameID

  if (CurrGID) { // part of a graphics group
    if (GIDList.find(CurrGID) == NULL) {  // starting it
      cp = new DCctl(group, 4, 1, 1); // graphic group ctl
      GIDList.add(cp, CurrGID);
      // add to enclosing page or frame ahead of text frame
      DCgroup *ngp = GroupStack[MIFlevel - 1];
      GGroupList.add(ngp, CurrGID);
      ngp->add(*cp);
    }
  }

  // add text frame to enclosing page or frame
  DCgroup *gp = GroupStack[MIFlevel];
  GroupStack[MIFlevel - 1]->add(*gp);

  // assume frame is relative to page top left corner if at right level
  uns fref = (MIFlevel == 2) ? 3 : 4;  // else relative to enclosing frame
  // store page ID indexed by FrameID
  if (CurrPG == psg_page) {
    FramePageList.add(PageID, FrameID);
    if (PageID == LastMasterPage) {
      cp = new DCctl(imnem, 3, 21, 3);  // master type ctl
      gp->add(*cp);
      MPFrameMTList.add(cp, FrameID);
			if ((MasterType == 1)
			 || (MasterType == 2)) {
				MPFrameTList->add(CurrGrRect.t, FrameID);
				MPFrameHList->add(CurrGrRect.h, FrameID);
			}
    }
  }
  else {  // otherwise we are in an AFrame
    FrameAFList.add(AFNum, FrameID);
    fref = 4;  // also frame is relative to enclosing frame, not page
		cp = new DCctl(imnem, 3, 24, 1, CurrDash);
		gp->add(*cp);
		cp = new DCctl(imnem, 3, 24, 2, CurrColor);
		gp->add(*cp);
		cp = new DCctl(imnem, 3, 24, 3, CurrPenWid);
		gp->add(*cp);
		cp = new DCctl(imnem, 3, 25, 1, CurrPen);
		gp->add(*cp);
		cp = new DCctl(imnem, 3, 26, 1, CurrFill);
		gp->add(*cp);
		cp = new DCctl(imnem, 3, 26, 2, CurrColor);
		gp->add(*cp);
  }

  cp = new DCctl(elong, 3, 22, fref);  // frame size ctl
  cp->ex(&CurrGrRect, sizeof(CurrGrRect)); // written from last BRect
  gp->add(*cp);

  if (TAngle) {  // specify frame rotation if any
    cp = new DCctl(ilong, 3, 21, 13, TAngle);
    gp->add(*cp);
  }

  // if there was no layout text ctl for this frame yet, make a 0 one
  if (FrameTextList.find(FrameID) == NULL) {
    cp = new DCctl(elong, 3, 40, 0);  // layout text 0 ctl
    gp->add(*cp);
    FrameTextList.add(cp, FrameID); // indexed by dcl ID of frame
  }

  // check for layout text ctls in previous frames that reference this frame
  if (cp = FrameNextList.find(CurrID))  // if prev frame in flow has been seen
    cp->siz(FrameID);  // update the ID in its layout text ctl for this frame

  cp = new DCctl(imnem, 3, 21, 2);  // frame usage ctl, usage unknown so far
  gp->add(*cp);
  FrameUsageIndex.add(cp, FrameID); // indexed by dcl ID of frame
}


void
MIFrd::ProcTFNext(mif *mp)
{
  if (HiddenPage)
    return;

  // layout text ctl, needs dat=TextID and size=FrameID of next text frame
  // neither of these IDs is known now, so they must be fixed up later
  DCctl *cp = new DCctl(mp->dtype, mp->maj, mp->min, mp->dat, MIFval);
  GroupStack[MIFlevel]->add(*cp);
  // store ptr in list, for later change of dat() from 0 to TextID
  FrameTextList.add(cp, FrameID); // indexed by dcl ID of frame

  uns nframe = (uns) FrameIDList(MIFval); // try to look up dcl ID of next frame
  if (nframe)         // if next frame (TextRect) already seen
    cp->siz(nframe);  // put in its dcl ID now
  else FrameNextList.add(cp, MIFval); // save for fixup when frame is seen
}



// text flow processing


uns MIFrd::TextID = 0;
uns MIFrd::CurrTextID = 0;
uns MIFrd::CurrTFrame = 0;
uns MIFrd::CurrPage = 0;
bool MIFrd::ParaStarting = false;
bool MIFrd::TFAuto = false;
bool MIFrd::TFPost = false;

bool MIFrd::FlowTagged = false;
char MIFrd::TFTag[FStringMax];

bool MIFrd::HiddenPage = false;
bool MIFrd::HiddenFlow = false;
bool MIFrd::HiddenEnd = false;
bool MIFrd::HiddenFull = false;

DCctl *MIFrd::FirstTFCtl;
DCctl *MIFrd::FirstPGCtl;


void
MIFrd::ProcTextFlow(mif *mp)
{
  CurrPG = psg_tflow;
  FlowTagged = false;
  HiddenFlow = false;
  HiddenEnd = true;
  FNType = 0;
  FNoteList = new DCclist;

  // start first text seg of text stream
  DCgroup *gp = new DCgroup(DCctl(mp->dtype, mp->maj, mp->min, ++TextID));
  GroupStack[MIFlevel] = gp;
	CurrTextID = TextID;

  // put out text prev ctl for start of flow
  DCctl *cp = new DCctl(imnem, 2, 3, 0, 0);
  gp->add(*cp);

  // put out text frame ctl for dcl ID of new frame and its page
  FirstTFCtl = new DCctl(imnem, 2, 2, 0, 0);
  gp->add(*FirstTFCtl);

  // put out hyperpage ctl with FM page number
	FirstPGCtl = new DCctl(ilong, 2, 112, 12, 0);
	gp->add(*FirstPGCtl);


  CurrTFrame = CurrPage = 0; // frame and page not yet known
	CurrAFrame = 0;
  //CurrPForm = 0;
  TFAuto = TFPost = false;
  *TFTag = 0;
  OldConds = new DClist;
  EndProc[MIFlevel] = EndFlow;
}


void
MIFrd::EndFlow()
{
  delete FNoteList;
  DCgroup *gp = GroupStack[MIFlevel];

	CurrTextID = 0;
  if (HiddenFlow) {
    TextID--;
    //delete gp;  // yet another cleanup attempt fails
    return;
  }

  // turn off any leftover conditionals
  assert(OldConds);
  ProcCondEnd(0);
  delete OldConds;
	OldConds = NULL;

  // mark end of stream with text next 0 ctl (text already flushed)
  DCctl *cp = new DCctl(imnem, 2, 4, 0);
  gp->add(*cp);

  // add last segment to tflow psg
  GroupList[psg_tflow]->add(*gp);
}


void
MIFrd::ProcPara(mif *mp)
{
  if (!InXrefText && (!HiddenFlow || HiddenEnd)) {
    DCgroup *gp = new DCgroup;
    GroupStack[MIFlevel] = gp;
  }
  ParaStarting = true;
  LineTabCount = 0;
  HiddenEnd = HiddenFull = false;
  MIFval = 0;
  MakeCtl(mp);  // text break para start ctl
  EndProc[MIFlevel] = EndPara;

	// put in call to init format in text, using 0 as value to set later
	CurrPTagCtl = new DCctl(imnem, 2, 82, 1, 0);
	GroupStack[MIFlevel]->add(*CurrPTagCtl);
	PgfTagged = false;
	PgfTagSeen = false;
	TagPF = true; // in case of no tag

	// for tracking math-containing anchored frames
	MathParaID = 0;
	MathParaSequence = 0;
}


void
MIFrd::EndPara()
{
  FlushText();
  if (!HiddenFlow || HiddenFull || !HiddenEnd) {
    DCctl *cp = new DCctl(imnem, 2, 11, 2, 0);  // text break para end ctl
    GroupStack[MIFlevel]->add(*cp);
  }

	if (CurrPTagCtl != NULL)  //  no ParaTag or Pgf seen
		CurrPTagCtl->siz(CurrPForm);	// replace 0 with value
	CurrPTagCtl = NULL;
	PgfTagged = false;

  if (InXrefText)  // keep xref content encapsulated 
    GroupStack[MIFlevel - 1] = GroupStack[MIFlevel];  // still in xref dcl group
  else if (!HiddenFlow)
    GroupStack[MIFlevel - 1]->merge(*GroupStack[MIFlevel]);
  else if (HiddenEnd)
    ProcCondEnd(0);
	CurrFnt = DefPgfFnt;
}


void
MIFrd::ProcTLine()
{
  // LineTabCount = 0;	 // tab may be in preceding format prefix
	if (InXrefText) {
		MIFlevel--;
		EndProc[MIFlevel] = EndTLine;
		return;
	}

  EndProc[MIFlevel] = EndTLine;
	if (PgfTagged == false) {  // no ParaTag or Pgf, must put one now
		CurrFnt = DefPgfFnt;
		CurrPTagCtl->siz(CurrPForm);	// replace 0 with value
		CurrPTagCtl = NULL;
		pform *pf = (pform *) PFormList.find(CurrPForm);
		if (memcmp(pf, &CurrPgf, sizeof(pform))) {
			PutPgfOverrides(pf);
			DCctl *cp = new DCctl(imnem, 2, 82, 1, 0);  // init store
	    GroupStack[MIFlevel]->add(*cp);
		}
		//DefPgfFnt = CurrFnt;
		PgfTagged = true;	
	}
}


void
MIFrd::EndTLine()
{
  if (InXrefText) {  // skip in xref content
		MIFlevel++;
		return;
	}

  AddText(0);
  DCctl *cp = new DCctl(imnem, 2, 11, 10, 0);  // text break soft ret ctl
	LineTabCount = 0;
  GroupStack[MIFlevel]->add(*cp);
  //if (InXrefText)  // keep xref content encapsulated
  //  GroupStack[MIFlevel - 1] = GroupStack[MIFlevel];  // still in xref dcl group
}


void
MIFrd::ProcTFRef(mif *mp)
{
  DCctl *cp = NULL;
  uns npage = 0;
	uns nframe = 0;
  uns naframe = 0;
	long pgn = 0;
  DCgroup* xgp;

  if (HiddenFlow)
    return;

  DCgroup *gp = GroupStack[MIFlevel - 2];

  // get dcl ID of next frame from mif ID
  nframe = (uns) FrameIDList(MIFval);
  assert(nframe);

  // look up new page by FrameID of new frame
  npage = (uns) FramePageList.find(nframe);
  if (npage == 0) {  // check out AFrame components
		naframe = (uns) FrameAFList.find(nframe);
    if (naframe)
			npage = (uns) PageAFList.find(naframe);
	}
	else
		pgn = PageNumList.find(npage);

  if (CurrTFrame) {  // dcl ID of current frame, 0 if starting stream
    // fill in dcl ID of next frame in old frame's layout text ctl
    FrameTextList.find(CurrTFrame)->siz(nframe);

    // add previous seg to tflow psg
    GroupList[psg_tflow]->add(*gp);

    if (!ParaStarting) {  // current info goes into current frame
      FlushText(); // finish off text of old page before adding ctls
      if (InXrefText) {
        xgp = new DCgroup(*GroupStack[MIFlevel]);  // clone xref group
        if (GroupStack[MIFlevel - 1] == GroupStack[MIFlevel])
          GroupStack[MIFlevel - 1] = XrefParentGroup;  // if line break happened
        GroupStack[MIFlevel] = XrefParentGroup;  // put back regular group
      }
      gp->merge(*GroupStack[MIFlevel]);
      GroupStack[MIFlevel - 1] = GroupStack[MIFlevel] = new DCgroup;
      if (InXrefText) {
        XrefParentGroup = GroupStack[MIFlevel];
        GroupStack[MIFlevel] = xgp;  // put new empty group in place
      }
    }

    // put out soft frame or page break
    cp = new DCctl(imnem, 2, 11, (npage == CurrPage) ? 3 : 4);
    gp->add(*cp);

    // put out text next ctl with link to next frame and its page
    cp = new DCctl(imnem, 2, 4, nframe, npage);
    gp->add(*cp);

    // start new text seg with current TextID
    gp = new DCgroup(DCctl(group, 2, 1, TextID));

    // replace previous group ref on stack with new one
    GroupStack[MIFlevel - 2] = gp;

    // put out text prev ctl with dcl ID of old frame and its page
    cp = new DCctl(imnem, 2, 3, CurrTFrame, CurrPage);
    gp->add(*cp);

    // put out text frame ctl with dcl ID of new frame and its page
    cp = new DCctl(imnem, 2, 2, nframe, npage);
    gp->add(*cp);

    // put out hyperpage ctl with FM page number
		if (pgn && !PageDoneList.find(pgn)) {
			PageDoneList.add(pgn);
			cp = new DCctl(ilong, 2, 112, 12, pgn);
			gp->add(*cp);
		}

    // add ctls to specify starting state for new seg

  }
  else {
    if (npage <= LastMasterPage) {  // starting flow on master page
      short fnum = nframe;
      if (!FlowTagged) {  // identify headers and footers
        if (!PageHeaderList.find(npage) && FrameHeaderList.find(fnum)) {
          PageHeaderList.add(npage);
          FrameUsageIndex.find(fnum)->siz(2);
        }
        else if (!PageFooterList.find(npage) && FrameFooterList.find(fnum)) {
          PageFooterList.add(npage);
          FrameUsageIndex.find(fnum)->siz(3);
        }
      }
      while (fnum) {
        if ((cp = MPFrameMTList.find(fnum)) == NULL)
          break;
        cp->siz(FlowTagged ? 2 : 1);
        if (FlowTagged)
          FrameUsageIndex.find(fnum)->siz(1);
        if ((cp = FrameTextList.find(fnum)) == NULL)
          break;
        cp->dat(TextID);
        fnum = (short) cp->siz();
      }
    }
    FirstTFCtl->dat(nframe);  // update frame and page in ctl starting seg
    FirstTFCtl->siz(npage);

    // put out hyperpage ctl with FM page number
		if (pgn && !PageDoneList.find(pgn)) {
			PageDoneList.add(pgn);
			FirstPGCtl->siz(pgn);
		}
		else
			FirstPGCtl->maj((uns) 0); // kill it
  }

  // change globals from old to new frame and page IDs
  CurrTFrame = nframe;
  CurrPage = npage;
	CurrAFrame = naframe;

  if (CurrPage)	   // add new seg to index, indexed by page ID
    PageTextIndex.add(gp, CurrPage);
	else if (naframe)	{  // look up after anchor seen
		PagelessAFTextIndex.add(gp, naframe);
		PagelessAFTextStart.add(FirstTFCtl, naframe);
	}
	else	// must be in frame we ignored on page, a bad thing
		PagelessTextIndex.add(gp, CurrTFrame);

  // fill in dcl text ID in new frame's layout text ctl
  FrameTextList.find(CurrTFrame)->dat(TextID);

  // fill in frame usage in new frame's ctl
  if (FlowTagged)
    FrameUsageIndex.find(CurrTFrame)->siz(1);
}


void
MIFrd::ProcHiddenMarker(void)
{
	InHiddenMarker = true;
  // ignore hidden text when show hidden is off; permit versioning
}



// footnote processing


bool MIFrd::InNotes = false;
DCclist *MIFrd::FNoteList;
uns MIFrd::FNType = 0;


void
MIFrd::ProcNotes()
{
  InNotes = true;
  EndProc[MIFlevel] = EndNotes;
}


void
MIFrd::EndNotes()
{
  InNotes = false;
}


void
MIFrd::ProcFNote(mif *mp)
{
  DCgroup *gp;

  if (!InNotes) { // footnote ref
    gp = (DCgroup *) (FNoteList->find(MIFval));
    assert(gp);
    FlushText();
    GroupStack[MIFlevel]->add(*gp);
		// process anchored frames in footnote
		FNoteAFIndex.execu(MIFval, (void (__cdecl *)(void *)) ProcAFLinks);
    return;
  }

  gp = new DCgroup(DCctl(mp->dtype, mp->maj, mp->min, FNType + 1));
  GroupStack[MIFlevel] = gp;
  CurrID = 0;
  if ((MIFval = FormNameList.getnum(FNType ? TFNName : FNName))
   || (MIFval = FormTagNameList.getnum(FNType ? TFNName : FNName))) {
    CurrPForm = (uns) MIFval;  // use footnote format
    CurrTabList = (DCvlist *) TabFormatList.find(CurrPForm);
  }
  EndProc[MIFlevel] = EndFNote;
}


void
MIFrd::EndFNote()
{
  DCgroup *gp = GroupStack[MIFlevel];
  assert(CurrID);
  FNoteList->add(gp, CurrID);
  FlushText();
}



// literal text processing


unc MIFrd::ANSItab[AnsiTabMax] = {
  0xC4, 0xC5, 0xC7, 0xC9, 0xD1, 0xD6, 0xDC, 0xE1,
  0xE0, 0xE2, 0xE4, 0xE3, 0xE5, 0xE7, 0xE9, 0xE8,

  0xEA, 0xEB, 0xED, 0xEC, 0xEE, 0xEF, 0xF1, 0xF3,
  0xF2, 0xF4, 0xF6, 0xF5, 0xFA, 0xF9, 0xFB, 0xFC,

  0x86, 0xB0, 0xA2, 0xA3, 0xA7, 0x95, 0xB6, 0xDF,
  0xAE, 0xA9, 0x99, 0xB4, 0xA8, 0xA6, 0xC6, 0xD8,

  0xD7, 0xB1, 0xF0, 0x8A, 0xA5, 0xB5, 0xB9, 0xB2,
  0xB3, 0xBC, 0xBD, 0xAA, 0xBA, 0xBE, 0xE6, 0xF8,

  0xBF, 0xA1, 0xAC, 0xD0, 0x83, 0xDD, 0xFD, 0xAB,
  0xBB, 0x85, 0xFE, 0xC0, 0xC3, 0xD5, 0x8C, 0x9C,

  0x96, 0x97, 0x93, 0x94, 0x91, 0x92, 0xF7, 0xDE,
  0xFF, 0x9F,    0, 0xA4, 0x8B, 0x9B,    0,    0,

  0x87, 0xB7, 0x82, 0x84, 0x89, 0xC2, 0xCA, 0xC1,
  0xCB, 0xC8, 0xCD, 0xCE, 0xCF, 0xCC, 0xD3, 0xD4,

  0x9A, 0xD2, 0xDA, 0xDB, 0xD9, 0x80, 0x88, 0x98,
  0xAF,    0,    0, 0xB0, 0xB8, 0xA8,    0, 0xFF
};

uns MIFrd::CharSymList[CharSymsMax] = {
     9, 0xA0, 0xAD, 0xAD, 0xAD, 0xA2, 0xA3, 0xB4,
  0xD0, 0xD1, 0xA0, 0xE0, 0xA5,   10, 0xA6, 0xA4,
  0x2007, 0x2009, 0x2002, 0x2003, 0x2011
};

uns MIFrd::CharList[CharSymsMax] = {
     9, 0xA0, 0xAD, 0xAD, 0xAD, 0xA2, 0xA3, 0xA5,
  0x96, 0x97, 0x86, 0x87, 0x95,   10, 0xB6, 0xA7,
  0x2007, 0x2009, 0x2002, 0x2003, 0x2011
};


void
MIFrd::ProcTextObj()
{
  unc ch = 0;
	unl uch = 0;

  if (GraphTextStart)
    StartGraphText();

  ParaStarting = false;

  // make ctls out of MIFstring, process tabs, specials, etc.
  for (unc *p = (unc *) MIFstring; *p; p++) {
		if ((*p > 0x7F)
		 && (MIFversion > 6)
		 && ::IsUTF8(p)) {
			uch = ::UTF8(&p);
			if ((uch > 127) && (SymFont || NoSymMap))
				uch = CheckC1CtrlSyms(uch);
			AddText(uch);
			p--;
		}
    else if (*p != '\\')
      AddText(*p & 0xFF);
    else switch (ch = *++p) { // Frame backslash escapes (\" etc. already done)
      case 't':  // tab, flush text and put out tab ctl
        AddText('\t');
        break;
      case 'x':  // hex
        ProcHex((char **) &p);
        break;
      default:
        AddText(ch & 0xFF);
        break;
    }
  }
}


void
MIFrd::ProcHex(char **cpp)
{
  unc ch = 0;
  uns val = 0;
	uns unic = 0;

  for (ch = *++(*cpp); isxdigit(ch); ch = *++(*cpp))
    val = (val << 4)
          + (ch - ((ch > '9') ? ((ch > 'F') ? 'a'-10 : 'A'-10) : '0'));

  switch (val) {
    case 4:  // disc hyph within word
      val = 0xAD;
      break;
    case 5:  // no hyph, before word
      val = 0xAD;
      break;
    case 6:  // soft hyphen
      val = 0xAD;
      break;
    case 21: // hard hyphen, \-
      val = 0x2011;
      break;

    case 8:  // tab, process as ctl
      val = '\t';
      break;
    case 9:  // hard return, \n
      val = '\n';
      break;

    case 10: // para break, process as symbol
      val = 0xB6;
      break;
    case 11: // end of flow, process as symbol
      val = 0xA7;
      break;

    case 17: // hard space
      val = 0xA0;
      break;
    case 18: // thin space
      val = 0x2009;
      break;
    case 16: // numeric space
      val = 0x2007;
      break;
    case 19: // en space
      val = 0x2002;
      break;
    case 20: // em space
      val = 0x2003;
      break;

    case 0xDA: // frac bar
      if (SymFont)
        break;
      val = 0x2044;
      break;

    case 0xDE: // fi ligature, make "fi"
      if (SymFont)
        break;
      AddText('f');
      val = 'i';
      break;
    case 0xDF: // fl ligature, make "fl"
      if (SymFont)
        break;
      AddText('f');
      val = 'l';
      break;

    case 0x7F: // not used
      val = 0;
      break;

    default:
      if ((val < 32) || (val > 255))
        val = 0;
      else if (val > 127) {
				if ((MIFversion < 8)
				 && !SymFont && !NoSymMap)
		      val = ANSItab[val - 128];
				else if ((MIFversion > 7)
				 && (SymFont || NoSymMap))
					val = (uns) CheckC1CtrlSyms(val);
			}
      break;
  }
  if (val)
    AddText(val);
}


// Frame 8 maps to std Unicode even for symbols, map back here

unl MIFrd::C1Ctrls[32] = {
 8364,  129, 8218,  402, 8222, 8230, 8224, 8225,
  710, 8240,  352, 8249,  338, 141,  381,  143,
  144, 8216, 8217, 8220, 8221, 8226, 8211, 8212,
  732, 8482,  353, 8250,  339, 157,  382,  376 
};


unl
MIFrd::CheckC1CtrlSyms(unl uch)
{
	unl i = 0;
	for ( ; i < 32; i++) {
		if (uch == C1Ctrls[i])
			return (i + 128);
	}
	return uch;
}


void
MIFrd::ProcChar()
{
  if (GraphTextStart)
    StartGraphText();

#if 0
  if (MIFval < 0) {  // not found, assume number
    if (val = ProcHex(atol(MIFstring)))
      AddText(val);
    return;
  }
#endif

  if (MIFval >= CharSymsMax) {
		LogEvent(logdebug, 2, "Undefined character name");
    return;
	}

	if (SymFont)
	  AddText(CharSymList[MIFval]);
	else
		AddText(CharList[MIFval]);
}



// text buffering


size_t MIFrd::TextPos = 0;
size_t MIFrd::LastLinePos = 0;
DCctl *MIFrd::TextObjLit;
DCblock *MIFrd::TextBuf;


void
MIFrd::AddText(unl ch)
{
  static size_t TextLen;
  static bool TabPending = false;
  static DCblock TextTmp(TextBufMax);
  DCctl *cp = NULL;

  if (TextPos == TextBufMax) {
    if (LastLinePos
     && ((TextLen = TextPos - LastLinePos) > 0)) {
#if 0
      void *vp = (void *) (((char *) (TextBuf->p())) + LastLinePos);
      memcpy(TextTmp.p(), vp, TextLen);
#else
      memcpy(TextTmp.p(),
             (void *) &(( (char *) (TextBuf->p()))[LastLinePos]), TextLen);
#endif
      TextPos = LastLinePos;
      FlushText();
      memcpy(TextBuf->p(), TextTmp.p(), TextLen);
      TextPos = TextLen;
    }
    else FlushText();
  }

  if (ch == '\t') {
    TabPending = true;
    LineTabCount++;
    return;
  }

  if (TabPending) {
    TabPending = false;
    FlushText();
    MakeTabCtl();
  }

  if (ch == 0) {
    FlushText();
    return;
  }

  if (ch == '\n') {
    FlushText();
    cp = new DCctl(imnem, 2, 11, 10, 1);
    GroupStack[MIFlevel]->add(*cp);
    return;
  }

  if ((ch < ' ') || (ch > 126)) {
    FlushText();
		if (ch > 0x1FFFFF)
			ch &= 0xFF;
    cp = new DCctl(imnem, 2, 10, 1, ch);
    GroupStack[MIFlevel]->add(*cp);
    return;
  }

  ((char *) (TextBuf->p()))[TextPos++] = (unc) (ch & 0xFF);

  if (ch == ' ')
    LastLinePos = TextPos;
}


void
MIFrd::FlushText()
{
  if (!TextPos)
    return;

  if (TextPos == 1) {
		unc ch = *((unc *) (TextBuf->p()));
    DCctl *cp = new DCctl(imnem, 2, 10, 1, (unl) (ch & 0xFF));
    GroupStack[MIFlevel]->add(*cp);
    TextPos = 0;
    return;
  }

  if (TextPos < TextBufMax)
    TextBuf->set(++TextPos);
#if 0
  if (InXrefText)
    TextObjLit->set(etext, 2, 96, 4);
#endif
  *TextObjLit = *TextBuf;
  GroupStack[MIFlevel]->add(*TextObjLit);
  TextObjLit = new DCctl(etext, 2, 10, 1);
  TextBuf = new DCblock(TextBufMax);
  TextPos = LastLinePos = 0;
}


// end of drmiftx.cc


