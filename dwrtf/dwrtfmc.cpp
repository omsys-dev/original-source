// dwrtfmc.cpp is the code for dcl RTF macros, JHG
// Copyright (c) 2005 by Omni Systems, Inc.  All rights reserved.

#include "dcappl.h"
#include "dcctlg.h"
#include "dcfili.h"
#include "dctxfx.h"
#include "dcfmtr.h"
#include "dcini.h"
#include "dcmacr.h"
#include "dwrtf.h"
#include "dwrtffl.h"


// macro variables

char *RTFwr::TopInsert = NULL;
char *RTFwr::BottomInsert = NULL;
char *RTFwr::HeaderInsert = NULL;
char *RTFwr::FooterInsert = NULL;
char *RTFwr::FirstHeaderInsert = NULL;
char *RTFwr::FirstFooterInsert = NULL;
char *RTFwr::LeftHeaderInsert = NULL;
char *RTFwr::LeftFooterInsert = NULL;
char *RTFwr::RightHeaderInsert = NULL;
char *RTFwr::RightFooterInsert = NULL;

char *RTFwr::TopicStartInsert = NULL;
char *RTFwr::TopicEndInsert = NULL;
char *RTFwr::SlideStartInsert = NULL;
char *RTFwr::SlideEndInsert = NULL;
char *RTFwr::CurrPrjName = NULL;
char *RTFwr::CurrPrjPath = NULL;


// macromod callbacks

bool
RTFwr::WriteMacroChar(DCwfile *df, unc uch)
{
	RTFwfile *wf = (RTFwfile *) df;
	wf->putSpec(uch);
	return true;
}


bool
RTFwr::WritePredefinedMacro(DCwfile *df, char *name)
{
	RTFwfile *wf = (RTFwfile *) df;

	// <$_styleref(style)> = a STYLEREF field for header/footer
	if (!stricmp(name, "styleref")) {
		WriteStyleref(wf, DCmacr::MacroParam);
		return true;
	}

	// <$_style(style)> = style coding, complete
	if (!stricmp(name, "style")) {
		WriteStyle(wf, DCmacr::MacroParam);
		return true;
	}

	// <$_stylenum(style)> = style number
	if (!stricmp(name, "stylenum")) {
		WriteStylenum(wf, DCmacr::MacroParam);
		return true;
	}

	// <$_fontnum(fontname)> = font number
	if (!stricmp(name, "fontnum")) {
		WriteFontnum(wf, DCmacr::MacroParam);
		return true;
	}

	// <$_colornum(colorref)> = color number
	if (!stricmp(name, "colornum")) {
		WriteColornum(wf, DCmacr::MacroParam);
		return true;
	}

	// <$_pageref> = PAGEREF field referencing last bookmark used
	if (!stricmp(name, "pageref")) {
		WritePageRef(wf);
		return true;
	}

	if (!stricmp(name, "macroparam")) {
		DCmacr::WriteMacro(wf, NewName(DCmacr::MacroParam));
		return true;
	}

	return false;  // no predefined macros yet
}


void
RTFwr::WriteStyleref(RTFwfile *wf, char *style)
{
	wf->putCr();
	wf->putStr("{\\field{\\*\\fldinst styleref \"\\f1 ");
	wf->putStr(style);
	wf->putStr("\" \\\\* MERGEFORMAT }{\\fldrslt { }}}");
}


void
RTFwr::WriteStyle(RTFwfile *wf, char *stylename)
{
	if (!stylename)
		return;

	long fnum = FormNameList(stylename);
	pprop *style = (pprop *) FormList.find(fnum);

	if (!style) {
		LogEvent(logwarn, 1, "Style not found: ", stylename);
		return;
	}

	if (style->stype == 2) {
		wf->putCW("plain");
	  wf->putCWN("cs", (short) fnum);
	}
	else {
		wf->putCW("pard");
		wf->putCW("plain");
	  wf->putCWN("s", (short) fnum);
	}
	if (style->stype != 2)
		WriteParaStyles(wf, style);
	if (CharStyles)
		WriteCharStyles(wf, style);
	if (style->stype == 2)
		wf->putCW("additive");
	else if (style->tcount)
		WriteTabDefs(wf, style);
	if (style->stype != 2) {
		WriteBorders(wf, style);
		WriteShading(wf, style->shading);
	}
	if (style->styadd)
		DCmacr::WriteMacro(wf, style->styadd);
}


void
RTFwr::WriteStylenum(RTFwfile *wf, char *style)
{
	long fnum = FormNameList(style);
	wf->putLong(fnum);
}


void
RTFwr::WriteFontnum(RTFwfile *wf, char *font)
{
	uns fnum = GetFontNum(font);
	wf->putShort((short) fnum);
}


void
RTFwr::WriteColornum(RTFwfile *wf, char *color)
{
	long cval = DCfrfile::GetColorValue(color);
	long cv2 = CheckColorValue(cval);
	wf->putLong(cv2);
}


char *RTFwr::LastPageRefBookmark = NULL;

void
RTFwr::WritePageRef(RTFwfile *wf)
{
	if (!LastPageRefBookmark)
		return;

	bool lastlock = LockXrefs;
	LockXrefs = false;
	WriteRefFieldStart(wf, LastPageRefBookmark, true, false, NULL, false);
	WriteRefFieldEnd(wf);
	LastPageRefBookmark = NULL;
	LockXrefs = lastlock;
}



char *
RTFwr::GetPredefinedVar(DCwfile *df, char *vp)
{
	char *ret = NULL;

	if (!stricmp(vp, "paratag"))
		ret = Curr.stag;
	else if (!strnicmp(vp, "tbl", 3) && Tbl) {
		int val = (!strnicmp(&vp[3], "cols", 4)) ? Tbl->tbccount :
			    (!strnicmp(&vp[3], "rows", 4) ? Tbl->tbrcount : 0);
		char *str = NewName(NumStrMax);
		ret = itoa(val, str, 10);
	}
	else if (!stricmp(vp, "currfile"))
		ret = CurrFileFull;
	else if (!stricmp(vp, "currbase"))
		ret = CurrFileName;
	else if (!stricmp(vp, "basename"))
		ret = CurrFileName;
	else if (!stricmp(vp, "currfilepath"))
		ret = CurrFilePath;
	else if (!stricmp(vp, "currpath"))
		ret = CurrPath;
	else if (!stricmp(vp, "prjname"))
		ret = CurrPrjName;
	else if (!stricmp(vp, "prjpath"))
		ret = CurrPrjPath;
	else if (!stricmp(vp, "fileid")
	 && CurrFilePrefix)
		ret = CurrFilePrefix;
	else if (!stricmp(vp, "macroparam"))
		ret = DCmacr::MacroParam;


	if (ret)
		return NewName(ret);

	return NULL;
}


char *
RTFwr::GetFrameUserVar(char *vp)
{
	char *txt = NULL;
	uns vnum = 0;
	long tcount = 0;

	if ((vnum = (uns) VarNameList(vp)) != 0) {
	  // borrow TextStoreBuf to get var text out
		tcount = TextStoreBufCnt;
		txt = (char *) &TextStoreBuf[tcount];
		ProcTextStoreVar(vnum, false);
		txt = NewName(txt, TextStoreBufCnt - tcount);
		TextStoreBufCnt = tcount;
		return txt;
	}
	return NULL;
}



// CodeStore

DCnlist RTFwr::CodeStoreParaList;
DCnlist RTFwr::CodeStoreDivList;
char *RTFwr::CodeStoreVar = NULL;
long RTFwr::CodeStoreDiv = 0;
long RTFwr::PreCodeStoreDiv = 0;


bool
RTFwr::WriteCodeStoreVar(DCwfile *df, char *mn)
{
	RTFwfile *wf = (RTFwfile *) df;
	long div = 0;
	long cdiv = 0;

	if ((div = CodeStoreDivList.getitem(mn)) != 0) {
		cdiv = wf->GetDiv();
		wf->WriteDiv(div);
		wf->SelectDiv(cdiv);
		return true;
	}
	return false;
}


char *
RTFwr::GetCodeStoreVar(DCwfile *df, char *vp)
{
	RTFwfile *wf = (RTFwfile *) df;
	long div = 0;
	long cdiv = 0;
	char *var = NULL;

	if ((div = CodeStoreDivList.getitem(vp)) != 0) {
		cdiv = wf->GetDiv();
		wf->SelectDiv(div);
		var = wf->ReadDiv();
		wf->SelectDiv(cdiv);
		return NewName(var);
	}
	return NULL;
}


void
RTFwr::StartCodeStore(RTFwfile *wf)
{
	if (CodeStoreVar) {  // start diversion for macro var
		DCnlist *dcn = NULL;
		PreCodeStoreDiv = wf->GetDiv();
		if ((CodeStoreDiv = CodeStoreDivList.getitem(CodeStoreVar, &dcn)) != 0) {  // existing div
			wf->DeleteDiv(CodeStoreDiv);
			dcn->id = CodeStoreDiv = wf->NewDiv();
		}
		else {
			CodeStoreDiv = wf->NewDiv();
			CodeStoreDivList.add(CodeStoreVar, CodeStoreDiv);
		}
		CodeStoreVar = NULL;
	}
}


void
RTFwr::EndCodeStore(RTFwfile *wf)
{
	if (CodeStoreDiv) {  // end any pending div
		wf->SelectDiv(PreCodeStoreDiv);
		CodeStoreDiv = 0;
	}
}



// end of dwrtfmc.cpp
