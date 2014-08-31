 
// dwhtman.cpp is the code for dcl HTM xref and link writing, JHG
// Copyright (c) 1996-1998 by Omni Systems, Inc.  All rights reserved.

#include "dcappl.h"
#include "dcctlg.h"
#include "dclist.h"
#include "dcfili.h"
#include "dctxfx.h"
#include "dcfmtr.h"
#include "dcini.h"
#include "dcmacr.h"
#include "dcmxl.h"
#include "dwhtm.h"
#include "dwhtmfl.h"
#include <windows.h>
#undef hyper
#include "resource.h"

#pragma warning ( disable : 4237 )


// selection switch


bool
HTMwr::ProcAn(HTMctl *cp)
{
	uns num = 0;

  switch((cp->rp)->scan_op) {
    case nothing:   // just put with its group for later
      break;

    case pass:      // ignore, don't save 
      return false;

	  case formchk:
      if (InGraphic)
        return false;
			if (InGraphicText) {
				switch ((cp->rp)->write_op) {
					case hyplnk:
						ProcGrTextHyperLink(cp);
						break;
					case hypobj:
						ProcGrTextHyperObj(cp);
						break;
					case hyptarg:
						ProcGrTextHyperTarget(cp);
						break;
					case hyppag:
						ProcGrTextHyperObj(cp, true);
						break;
					case hypurl:
						ProcGrTextHyperURL(cp);
						break;
					case hypall:
						ProcGrTextHyperAlert(cp);
						break;
					case hypfile:
						ProcGrTextHyperFile(cp);
						break;
					default:
						break;
				}
				return false;
			}
			if (InFormDef) {
        SetCurrProps(NULL, cp);
        return false;
      }  
			else switch ((cp->rp)->write_op) {
				case sethref:
					ProcHyperMark(cp);
					break;
				case hyplnk:
					ProcHyperLink(cp);
					break;
				case hypobj:
					ProcHyperObj(cp);
					break;
				case hypall:
					ProcHyperAlert(cp);
					break;
				case hypurl:
					ProcHyperURL(cp);
					break;
				case hypfile:
					ProcHyperFile(cp);
					break;
				case hypunq:
					ProcHyperUnique(cp);
					break;
				case hyploc:
					ProcHyperLoc(cp);
					break;
				case hypcsh:
					ProcTopicAlias(cp);
					break;
				case hyppag:
					ProcHyperObj(cp, true);
					break;
				case hyppgn:
					ProcHyperUnique(cp, true);
					break;

				case alink:
					ALinkList.add((char *)cp->ex(), TxtParaRefNum);
					return false;

				case begxref:
					XrefFile = NULL;
					XrefInstID = (long) cp;
					break;
				case setxref:
					ProcXrefAName(cp);
					break;
				case putxref:
					ProcStartAnchor(cp);
					break;
				case endxref:
					ProcEndAnchor(cp);
					break;

				case anstart:
					InAnumVal = true;
					break;
				case anend:
					InAnumVal = false;
					break;

				case ansgrp:
					InAnumGroup = true;
					break;
				case anegrp:
					InAnumGroup = false;
					break;

				case idxgrp:
					InIdxGroup = true;
					IdxSeeAlso = false;
					ProcCondIndex((long) cp);
					break;
				case idxend:
					InIdxGroup = false;
					break;

				default:
					break;
			}
      break;


    case ftnstart:
      if (FootType == 'N') {
        SkipCurrGroup = true;
        SkipLevel = DCLlevel;
        return false;
      }
			InFootnote = true;
			StartingFootnote = true;
			FootnoteNum++;
			ProcCondFtn(FootnoteNum);
			SaveCondStateFtn(true);
			SaveParaVarsFtn();
			// save para ref info for later graphics
			PreFtnParaStartCtl = ParaStartCtl;
			PreFtnParaRefNum = ParaRefNum;
			PreFtnTxtParaStartCtl = TxtParaStartCtl;
			PreFtnTxtParaRefNum = TxtParaRefNum;
      break;
		case ftnend:
			ParaStartCtl = PreFtnParaStartCtl;
			ParaRefNum = PreFtnParaRefNum;
			TxtParaStartCtl = PreFtnTxtParaStartCtl;
			TxtParaRefNum = PreFtnTxtParaRefNum;
			RestoreParaVarsFtn();
			InFootnote = false;
			StartingFootnote = false;
			FtnConds = NULL;
			RestoreCondStateFtn();
			break;

    case ftnform:
      FtnForm = (uns) cp->siz();
      return false;

    case xrffnum:
      XrefID = (long) cp->dat();
      return false;
		case xrfname:
			ProcXrefFormatName(cp);
			return false;

		case varnum:
			NonWhiteText = true;
			if (StoreText
			 && (!InAnumVal || !SkipAnums)
			 && (!InFormPx || !SkipAnums)
			 && (!InTbl || AllowTbTitle)
			 && (!HideTbRow)
			 && (!WhiteText))
				ProcTitleVar((uns) cp->siz(), true);
			if (HXMwr::DITA
			 && (DITAVarType != 'T'))
				VarUsedList.append(cp->siz(), SplitID ? SplitID : 1);
			break;
		case vardef:
			CurrVar = cp->dat();
			break;
		case varname:
			CheckVarName(cp);
			break;
		case varform:
			SetVarForm(cp); // add to var list
			break;

		case varanum:
			NonWhiteText = true;
			if (StoreText
			 && (!InAnumVal || !SkipAnums)
			 && (!InFormPx || !SkipAnums)
			 && (!InTbl || AllowTbTitle)
			 && (!HideTbRow)
			 && (!WhiteText))
				ProcTitleArrVar(cp);
			break;
		case varacnt:
			SetVarArray(cp);
			break;
		case varaelem:
			break;
		case varaidx:
			CurrVarArrayItem = cp->siz();
			break;
		case varaform:
			SetVarArrItemForm(cp);
			break;

		case varsys:
			break;

		case setmark:
			ProcMarker(cp);
			return false;

		case anumdef:
			return false;
		case anumtype:
			switch (cp->siz()) {
				case 1:
					//InPgnAnum = true;
					break;
				case 2:
					InFtnAnum = true;
					InFtPx = true;
					break;
				case 8:
					InChapNum = true;
					break;
				case 9:
					InVolNum = true;
					break;
				case 10:
					InSectNum = true;
					break;
				case 11:
					InSSectNum = true;
					break;
				case 12:
					InTbFtnAnum = true;
					InTbFtPx = true;
					break;
				default:
					break;
			}
			return false;

		case anform:
			InAnFormat = true;
	    return false;
		case anfend:
			InAnFormat = false;
	    return false;
		case anrform:
			InAnRefFormat = true;
	    return false;
		case anrfend:
			InAnRefFormat = false;
	    return false;
		case ansyms:
			InAnSyms = true;
	    return false;
		case anesyms:
			InAnSyms = false;
	    return false;
		case anvtype:
			if (InFtnAnum) {
				if (InAnFormat || InAnRefFormat) {
					FootVType = cp->dat();
					if ((FootVType > 1) && (FootVType < 7))
						*FootTypeAttr = FootTypeAttrs[FootVType - 2];
					else {
						*FootTypeAttr = '\0';
						UseFootnoteLists = false;
					}
					FootItemTag = UseFootnoteLists ? "li" : "p";
					FootBlockTag = UseFootnoteLists ? "ol" : "div";
					InFtPx = false;
				}
			}
			else if (InTbFtnAnum) {
				if (InAnFormat || InAnRefFormat) {
					TbFootVType = cp->dat();
					if ((TbFootVType > 1) && (TbFootVType < 7))
						*TbFootTypeAttr = FootTypeAttrs[TbFootVType - 2];
					else {
						*TbFootTypeAttr = '\0';
						UseTbFootnoteLists = false;
					}
					if (!TbFootPos) {
						TbFootItemTag = FootItemTag;
						TbFootBlockTag = FootBlockTag;
					}
					else {
						TbFootItemTag = UseTbFootnoteLists ? "li" : "p";
						TbFootBlockTag = UseTbFootnoteLists ? "ol" : "div";
					}
					InTbFtPx = false;
				}
			}
			else if (InChapNum)
				ChapNumVType = cp->dat();
			else if (InVolNum)
				VolNumVType = cp->dat();
			else if (InSectNum)
				SectNumVType = cp->dat();
			else if (InSSectNum)
				SSectNumVType = cp->dat();
	    return false;

		case endanum:
      InFtnAnum = false;
      InTbFtnAnum = false;
			InChapNum = false;
 			InVolNum = false;
 			InSectNum = false;
 			InSSectNum = false;
	    return false;

    case anumset:
			if (InChapNum == true) {
				if ((ChapNumVType < 8) && (ChapNumVType > 1))
					ChapNum = SetNumVal(ChapNumVType, cp->dat(), FootSyms);
				//ChapNum = NewName(NumStrMax);
				//sprintf(ChapNum, "%ld", cp->dat());
			}
			else if (InVolNum == true) {
				if ((VolNumVType < 8) && (VolNumVType > 1))
					VolNum = SetNumVal(VolNumVType, cp->dat(), FootSyms);
				//VolNum = NewName(NumStrMax);
				//sprintf(VolNum, "%ld", cp->dat());
			}
			else if (InSectNum == true) {
				if ((SectNumVType < 8) && (SectNumVType > 1))
					SectionNum = SetNumVal(SectNumVType, cp->dat(), FootSyms);
				//SectionNum = NewName(NumStrMax);
				//sprintf(SectionNum, "%ld", cp->dat());
			}
			else if (InSSectNum == true) {
				if ((SSectNumVType < 8) && (SSectNumVType > 1))
					SSectNum = SetNumVal(SSectNumVType, cp->dat(), FootSyms);
				//SSectNum = NewName(NumStrMax);
				//sprintf(SSectNum, "%ld", cp->dat());
			}
      return false;

		case anumtxt:
			if (InChapNum == true)
				ChapNum = NewName((char *)cp->ex(), cp->siz());
			else if (InVolNum == true)
				VolNum = NewName((char *)cp->ex(), cp->siz());
			else if (InSectNum == true)
				SectionNum = NewName((char *)cp->ex(), cp->siz());
			else if (InSSectNum == true)
				SSectNum = NewName((char *)cp->ex(), cp->siz());
			return false;


    default:
#ifdef UNUSED
      assert(0);
#endif
			LogEvent(logdebug, 1, "Invalid case in ProcAn()");
      return false;
  }
  return true;
}


// HTML macros


bool
HTMwr::PutStyleMacro(HTMwfile *wf, unc pos, char *tag, pprop *cform)
{
	char *mactxt = NULL;
	char *mac = NULL;
	bool para = true;

	if (!cform && tag)
		cform = (pprop *) FormList.find(FormNameList(tag));
	if (!cform)
		cform = &Curr;

	switch (cform->stype) {
		case 1: // para
			para = true;
			break;
		case 2: // char
			if (cform->macpos & pos)
				break;
			para = false;
			break;
		default: // anything else
			return false;
	}

	switch (pos) {
		case MacBefore:  // before
			mac = (para ? cform->maccb : cform->cmaccb);
			break;

		case MacAfter:  // after
			mac = (para ? cform->macca : cform->cmacca);
			break;

		case MacReplace:  // instead
			mac = (para ? cform->maccr : cform->cmaccr);
			break;

		case MacStart:  // begin
			mac = (para ? cform->maccs : cform->cmaccs);
			break;

		case MacExtrReplace:  // extract macros
			mac = cform->macer;
			break;

		case MacExtrHead:
			mac = cform->maceh;
			break;

		case MacExtrTop:
			mac = cform->macet;
			break;

		case MacExtrBottom:
			mac = cform->maceb;
			break;

		case MacEnd:  // end
			mac = (para ? cform->macce : cform->cmacce);
			break;

		case MacAnumBefore:  // before
			mac = cform->macacb;
			break;

		case MacAnumAfter:  // after
			mac = cform->macaca;
			break;

		case MacLinkSrc: // link
			mac = cform->macls;
			break;

		default:
			return false;
	}

	mactxt = NewName(mac);

	if (!mactxt)
		return false;

	DCmacr::WriteMacro(wf, mactxt);

	DeleteName(mactxt);
	return true;
}


void
HTMwr::PutAnumMacro(HTMwfile *wf, bool start)
{
	InAnumVal = start;

	if (Curr.macpos & MacReplace) //  if not replacing
		return;

	if (start) {
		if (!InPara)
			WriteText(wf);
		if (Curr.macpos & MacAnumBefore)  // put before autonum
			PutStyleMacro(wf, MacAnumBefore);
		else if (Curr.macacb)
			DCmacr::WriteMacro(wf, Curr.macacb);
	}
	else {
		if (Curr.macpos & MacAnumAfter)  // put after autonum
			PutStyleMacro(wf, MacAnumAfter);
		else if (Curr.macaca)
			DCmacr::WriteMacro(wf, Curr.macaca);
	}
}


char *HTMwr::BeginFileInsert = NULL;
char *HTMwr::EntitiesInsert = NULL;

char *HTMwr::HeadInsert = NULL;
char *HTMwr::HeadEndInsert = NULL;
char *HTMwr::FramesInsert = NULL;
char *HTMwr::TopInsert = NULL;
char *HTMwr::BottomInsert = NULL;
char *HTMwr::EndInsert = NULL;
char *HTMwr::TopicBreak = NULL;

char *HTMwr::FirstHeadInsert = NULL;
char *HTMwr::FirstHeadEndInsert = NULL;
char *HTMwr::FirstFramesInsert = NULL;
char *HTMwr::FirstTopInsert = NULL;
char *HTMwr::FirstBottomInsert = NULL;
char *HTMwr::FirstEndInsert = NULL;

char *HTMwr::LastHeadInsert = NULL;
char *HTMwr::LastHeadEndInsert = NULL;
char *HTMwr::LastFramesInsert = NULL;
char *HTMwr::LastTopInsert = NULL;
char *HTMwr::LastBottomInsert = NULL;
char *HTMwr::LastEndInsert = NULL;

char *HTMwr::SplitHeadInsert = NULL;
char *HTMwr::SplitHeadEndInsert = NULL;
char *HTMwr::SplitFramesInsert = NULL;
char *HTMwr::SplitTopInsert = NULL;
char *HTMwr::SplitBottomInsert = NULL;
char *HTMwr::SplitEndInsert = NULL;

char *HTMwr::NonsplitHeadInsert = NULL;
char *HTMwr::NonsplitHeadEndInsert = NULL;
char *HTMwr::NonsplitFramesInsert = NULL;
char *HTMwr::NonsplitTopInsert = NULL;
char *HTMwr::NonsplitBottomInsert = NULL;
char *HTMwr::NonsplitEndInsert = NULL;

char *HTMwr::ExtrHeadInsert = NULL;
char *HTMwr::ExtrHeadEndInsert = NULL;
char *HTMwr::ExtrTopInsert = NULL;
char *HTMwr::ExtrBottomInsert = NULL;


void
HTMwr::PutInsertMacro(HTMwfile *wf, char *place)
{
	char *mactxt = NULL;

	bool first = false;
	bool last = false;
	bool split = false;
	bool nonsplit = false;
	bool extr = false;
	bool index = false;
	bool topic = false;

	bool bfile = false;
	bool ent = false;

	bool head = false;
	bool headend = false;
	bool frames = false;
	bool top = false;
	bool bottom = false;
	bool end = false;
	bool tbreak = false;

	if (!_strnicmp(place, "First", 5)) {
		place += 5;
		first = true;
	}
	else if (!_strnicmp(place, "Last", 4)) {
		place += 4;
		last = true;
	}
	else if (!_strnicmp(place, "Split", 5)) {
		place += 5;
		split = true;
	}
	else if (!_strnicmp(place, "Nonsplit", 8)) {
		place += 8;
		nonsplit = true;
	}
	else if (!_strnicmp(place, "Extr", 4)) {
		place += 4;
		extr = true;
	}
	else if (!_strnicmp(place, "Index", 5)) {
		place += 5;
		index = true;
	}
	else if (!_strnicmp(place, "Topic", 5)) {
		place += 5;
		topic = true;
	}

	if (!_stricmp(place, "BeginFile"))
		bfile = true;
	else if (!_stricmp(place, "Entities"))
		ent = true;
	else if (!_stricmp(place, "HeadEnd"))
		headend = true;
	else if (!_stricmp(place, "Head"))
		head = true;
	else if (!_stricmp(place, "Top"))
		top = true;
	else if (!_stricmp(place, "Bottom"))
		bottom = true;
	else if (!_stricmp(place, "Frames"))
		frames = true;
	else if (!_stricmp(place, "End"))
		end = true;
	else if (!_stricmp(place, "Break"))
		tbreak = true;

	if (ent)
		mactxt = EntitiesInsert;
	else if (bfile)
		mactxt = BeginFileInsert;
	else if (index) {
		if (bottom)
			mactxt = IndexBottomInsert;
		else if (top)
			mactxt = IndexTopInsert;
		else if (head)
			mactxt = IndexHeadInsert;
		else if (!_stricmp(place, "TOC"))
			mactxt = IndexTOCInsert;
	}
	else if (first) {
		if (bottom) {
			if (FirstBottomInsert)
				mactxt = FirstBottomInsert;
			else if (SplitBottomInsert)
				mactxt = SplitBottomInsert;
			else
				mactxt = BottomInsert;
		}
		else if (top) {
			if (FirstTopInsert)
				mactxt = FirstTopInsert;
			else
				mactxt = TopInsert;
		}
		else if (head) {
			if (FirstHeadInsert)
				mactxt = FirstHeadInsert;
			else
				mactxt = HeadInsert;
		}
		else if (headend) {
			if (FirstHeadEndInsert)
				mactxt = FirstHeadEndInsert;
			else
				mactxt = HeadEndInsert;
		}
		else if (frames) {
			if (FirstFramesInsert)
				mactxt = FirstFramesInsert;
			else
				mactxt = FramesInsert;
		}
		else if (end) {
			if (FirstEndInsert)
				mactxt = FirstEndInsert;
			else
				mactxt = EndInsert;
		}
	}
	else if (last) {
		if (bottom) {
			if (LastBottomInsert)
				mactxt = LastBottomInsert;
			else
				mactxt = BottomInsert;
		}
		else if (top) {
			if (LastTopInsert)
				mactxt = LastTopInsert;
			else if (SplitTopInsert)
				mactxt = SplitTopInsert;
			else
				mactxt = TopInsert;
		}
		else if (head) {
			if (LastHeadInsert)
				mactxt = LastHeadInsert;
			else if (SplitHeadInsert)
				mactxt = SplitHeadInsert;
			else
				mactxt = HeadInsert;
		}
		else if (headend) {
			if (LastHeadEndInsert)
				mactxt = LastHeadEndInsert;
			else if (SplitHeadEndInsert)
				mactxt = SplitHeadEndInsert;
			else
				mactxt = HeadEndInsert;
		}
		else if (frames) {
			if (LastFramesInsert)
				mactxt = LastFramesInsert;
			else if (SplitFramesInsert)
				mactxt = SplitFramesInsert;
			else
				mactxt = FramesInsert;
		}
		else if (end) {
			if (LastEndInsert)
				mactxt = LastEndInsert;
			else
				mactxt = EndInsert;
		}
	}
	else if (split) {
		if (bottom) {
			if (SplitBottomInsert)
				mactxt = SplitBottomInsert;
			else
				mactxt = BottomInsert;
		}
		else if (top) {
			if (SplitTopInsert)
				mactxt = SplitTopInsert;
			else
				mactxt = TopInsert;
		}
		else if (head) {
			if (SplitHeadInsert)
				mactxt = SplitHeadInsert;
			else
				mactxt = HeadInsert;
		}
		else if (headend) {
			if (SplitHeadEndInsert)
				mactxt = SplitHeadEndInsert;
			else
				mactxt = HeadEndInsert;
		}
		else if (frames) {
			if (SplitFramesInsert)
				mactxt = SplitFramesInsert;
			else
				mactxt = FramesInsert;
		}
		else if (end) {
			if (SplitEndInsert)
				mactxt = SplitEndInsert;
			else
				mactxt = EndInsert;
		}
	}
	else if (nonsplit) {
		if (bottom) {
			if (NonsplitBottomInsert)
				mactxt = NonsplitBottomInsert;
			else if (FirstBottomInsert)
				mactxt = FirstBottomInsert;
			else if (LastBottomInsert)
				mactxt = LastBottomInsert;
			else
				mactxt = BottomInsert;
		}
		else if (top) {
			if (NonsplitTopInsert)
				mactxt = NonsplitTopInsert;
			else if (FirstTopInsert)
				mactxt = FirstTopInsert;
			else
				mactxt = TopInsert;
		}
		else if (head) {
			if (NonsplitHeadInsert)
				mactxt = NonsplitHeadInsert;
			else
				mactxt = HeadInsert;
		}
		else if (headend) {
			if (NonsplitHeadEndInsert)
				mactxt = NonsplitHeadEndInsert;
			else
				mactxt = HeadEndInsert;
		}
		else if (frames) {
			if (NonsplitFramesInsert)
				mactxt = NonsplitFramesInsert;
			else
				mactxt = FramesInsert;
		}
		else if (end) {
			if (NonsplitEndInsert)
				mactxt = NonsplitEndInsert;
			else
				mactxt = EndInsert;
		}
	}
	else if (extr) {
		if (bottom) {
			if (ExtrBottomInsert)
				mactxt = ExtrBottomInsert;
			else
				mactxt = BottomInsert;
		}
		else if (top) {
			if (ExtrTopInsert)
				mactxt = ExtrTopInsert;
			else
				mactxt = TopInsert;
		}
		else if (head) {
			if (ExtrHeadInsert)
				mactxt = ExtrHeadInsert;
			else
				mactxt = HeadInsert;
		}
		else if (headend) {
			if (ExtrHeadEndInsert)
				mactxt = ExtrHeadEndInsert;
			else
				mactxt = HeadEndInsert;
		}
	}
	else if (topic) {
		if (tbreak)
			mactxt = TopicBreak;
	}
	else {
		if (bottom)
			mactxt = BottomInsert;
		else if (top)
			mactxt = TopInsert;
		else if (head)
			mactxt = HeadInsert;
		else if (headend)
			mactxt = HeadEndInsert;
		else if (frames)
			mactxt = FramesInsert;
		else if (end)
			mactxt = EndInsert;
	}

	if (!mactxt)
		return;

	if (!MinimizeLines)
		wf->putEnd();
	DCmacr::WriteMacro(wf, mactxt);
	if (!MinimizeLines
	 || !end)
		wf->putEnd();
}


bool HTMwr::FixMacroQuotes = false;
short HTMwr::MacroNestMax = MacroNestLevMax;
short HTMwr::RepeatMax = MacroNestLevMax;
short HTMwr::WhileMax = MacroNestLevMax;
char *HTMwr::MacroFile = "m2hmacro.ini";
bool HTMwr::OmitMacroReturns = false;
bool HTMwr::MacroVarNesting = true;
char *HTMwr::CurrPrjName = NULL;
char *HTMwr::CurrPrjPath = NULL;

DCnlist HTMwr::TextStoreNames;
DCnlist HTMwr::TextStoreVals;
DCilist HTMwr::TextStoreIDs;
long HTMwr::TextStoreCount = 0;


char *
HTMwr::GetTextStore(char *vp)
{
	long pos = 0;
	long lastpos = 0;
	long matchpos = 0;
	long prevpos = 0;
	long extrpos = 0;
	long sid = SplitID ? SplitID : 1;
	long lastsid = 0;
	
	DCnlist *lp = &TextStoreNames;

	while ((pos = lp->getitem(vp, &lp)) != 0) {
		lastpos = pos;
		lastsid = TextStoreIDs.find(pos);
		if (Extracting && (lastsid == ExtrID))
			extrpos = pos;
		if (lastsid == sid)
			matchpos = pos;
		if (SplitNumList.find(sid) > SplitNumList.find(lastsid))
			prevpos = pos;
		if (lp->next == NULL)
			break;
	}
	if (lastpos)
		return NewName(TextStoreVals.find(extrpos ? extrpos :
		                                 (matchpos ? matchpos :
		                                 (prevpos ? prevpos : lastpos))));
	return NULL;
}


bool
HTMwr::WriteCodeStoreVar(DCwfile *df, char *mn)
{
	HTMwfile *wf = (HTMwfile *) df;
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
HTMwr::GetCodeStoreVar(DCwfile *df, char *vp)
{
	HTMwfile *wf = (HTMwfile *) df;
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

char *
HTMwr::GetPredefinedVar(DCwfile *df, char *vp)
{
	HTMwfile *wf = (HTMwfile *) df;
	long spID = 0;
	char *ret = NULL;
	char *txt = NULL;

	if (!stricmp(vp, "paratag"))
		ret = Curr.stag;
	else if (!stricmp(vp, "parauid"))
		ret = ParaUIDRef;
	else if (!stricmp(vp, "xrefid"))
		ret = XrefIDRef;
	else if (!stricmp(vp, "class"))
		ret = Curr.hcname;
	else if (!stricmp(vp, "charclass"))
		ret = Curr.hscname;
	else if (!strnicmp(vp, "oh", 2))
		ret = OHMacroVars(vp);
	else if (!strnicmp(vp, "tbl", 3) && Tbl) {
		int val = (!strnicmp(&vp[3], "cols", 4)) ? Tbl->tbccount :
			    (!strnicmp(&vp[3], "rows", 4) ? Tbl->tbrcount : 0);
		char *str = NewName(NumStrMax);
		ret = itoa(val, str, 10);
	}
	else if (!stricmp(vp, "currfile"))
		ret = (SplitID ? SplitNameList.find(SplitID) : CurrXFileName);
	else if (!stricmp(vp, "currtitle"))
		ret = (SplitID ? SplitTitleList.find(SplitID) : CurrXFileTitle);
	else if (!stricmp(vp, "currbase"))
		ret = (SplitID ? BaseNameList.find(Extracting ? ExtrID : SplitID) : CurrFileName);
	else if (!stricmp(vp, "basename"))
		ret = CurrBaseName;
	else if (!stricmp(vp, "currfilepath"))
		ret = CurrFilePath;
	else if (!stricmp(vp, "prjname"))
		ret = CurrPrjName;
	else if (!stricmp(vp, "prjpath"))
		ret = CurrPrjPath;

	else if (!stricmp(vp, "basefile"))
		ret = CurrXFileBase;
	else if (!stricmp(vp, "basetitle"))
		ret = (CurrXFileTitle ? CurrXFileTitle : Title);

	else if (!stricmp(vp, "toptitle"))
		ret = TopTitle;

	else if (!stricmp(vp, "prevfile")) {
		if (FirstPart)
			ret = GetPrevFSName(wf);
		else {
			spID = PrevSplitList.find(SplitID);
			ret = ((spID > 1) ? SplitNameList.find(spID) : CurrXFileName);
		}
	}
	else if (!stricmp(vp, "prevtitle")) {
		if (FirstPart)
			ret = GetPrevFSTitle(wf);
		else {
			spID = PrevSplitList.find(SplitID);
			ret = ((spID > 1) ? SplitTitleList.find(spID) : CurrXFileTitle);
		}
	}
	else if (!stricmp(vp, "prevsfile")) {
		if (SplitID == FirstSplitID)  // may both be 0
			ret = GetPrevFSName(wf);
		else {
			spID = PrevSplitList.find(SplitID ? SplitID : 1);
			ret = ((spID > 1) ? SplitNameList.find(spID) : CurrXFileName);
		}
	}

	else if (!stricmp(vp, "nextfile")) {
		if (LastPart)
			ret = GetNextFSName(wf);
		else {
			spID = NextSplitList.find(SplitID ? SplitID : 1);
			ret = ((spID > 1) ? SplitNameList.find(spID) : CurrXFileName);
		}
	}
	else if (!stricmp(vp, "nexttitle")) {
		if (LastPart)
			ret = GetNextFSTitle(wf);
		else {
			spID = NextSplitList.find(SplitID ? SplitID : 1);
			ret = ((spID > 1) ? SplitTitleList.find(spID) : CurrXFileTitle);
		}
	}

	else if (!stricmp(vp, "seqcurrtitle"))
		ret = CurrFSTitle ? CurrFSTitle : Title;

	else if (!stricmp(vp, "seqprevfile"))
		ret = GetPrevFSName(wf);
	else if (!stricmp(vp, "seqprevtitle"))
		ret = GetPrevFSTitle(wf);
	else if (!stricmp(vp, "seqstarttitle"))
		ret = StartingPrevFSTitle;

	else if (!stricmp(vp, "seqnextfile"))
		ret = GetNextFSName(wf);
	else if (!stricmp(vp, "seqnexttitle"))
		ret = GetNextFSTitle(wf);
	else if (!stricmp(vp, "seqendtitle"))
		ret = EndingNextFSTitle;

	else if (!stricmp(vp, "firstfile"))
		ret = (FirstPart ? "1" : "0");
	else if (!stricmp(vp, "firstsfile"))
		ret = ((SplitID == FirstSplitID) ? "1" : "0");
	else if (!stricmp(vp, "lastfile"))
		ret = (LastPart ? "1" : "0");

	else if (!stricmp(vp, "fileid")
	 && CurrFilePrefix)
		ret = CurrFilePrefix;
	else if (!stricmp(vp, "splitnum"))
		ret = (SplitID ? SplitNumList.find(SplitID) : SplitNumStart);
	else if (!stricmp(vp, "splitid"))
		ret = (SplitID ? SplitIDRef : "1");

	else if (!stricmp(vp, "extrfile")
	 && ExtrFile)
		ret = ExtrFile;
	else if (!stricmp(vp, "extrtitle")
	 && ExtrID)
		ret = (SplitTitleList.find(ExtrID));
	else if (!stricmp(vp, "extrgraph")
	 && ExtrGraph)
		ret = ExtrGraph;
	else if (!stricmp(vp, "extrgraphid")
	 && ExtrGraphID)
		ret = ExtrGraphID;
	else if (!stricmp(vp, "extrgraphclass")
	 && ExtrGraphClass)
		ret = ExtrGraphClass;
	else if (!stricmp(vp, "extrgraphtarget")
	 && ExtrGraphTarget)
		ret = ExtrGraphTarget;
	else if (!stricmp(vp, "extrgraphhigh")
	 && ExtrGraphHigh)
		ret = ExtrGraphHigh;
	else if (!stricmp(vp, "extrgraphwide")
	 && ExtrGraphWide)
		ret = ExtrGraphWide;

	else if (!stricmp(vp, "objectid")
	 && ObjectID)
		ret = ObjectID;
	else if (!stricmp(vp, "linksrc")
	 && LinkSrc)
		ret = LinkSrc;

	else if (!stricmp(vp, "graphsrc")
	 && GraphSrc) {
		ret = GraphSrc;
		GraphSrcUsed = true;
	}
	else if (!stricmp(vp, "graphbase")
	 && GraphBase) {
		ret = GraphBase;
		GraphSrcUsed = true;
	}
	else if (!stricmp(vp, "graphorighigh")
	 && GraphOrigHigh)
		ret = GraphOrigHigh;
	else if (!stricmp(vp, "graphorigwide")
	 && GraphOrigWide)
		ret = GraphOrigWide;

	else if (!stricmp(vp, "loctoctitle")
	 && LocalTOCID
	 && (((txt = LocalTOCTitleList.find(LocalTOCID)) != NULL)
		|| ((txt = SplitTitleList.find(LocalTOCID)) != NULL)))
		ret = txt;
	else if (!stricmp(vp, "loctocfile")
	 && LocalTOCID)
		ret = SplitNameList.find(LocalTOCID);
	else if (!stricmp(vp, "loctocind")
	 && LocalTOCIndent)
		ret = _itoa(LocalTOCIndent, NewName(NumStrMax), 10);

	else if (!stricmp(vp, "dropid")) {
		if (!DropDownUsed)
			return NULL;
		long len = strlen(DropIDPrefix);
		ret = NewName(DropIDPrefix, len + NumStrMax);
		itoa(DropID, (ret + len), 10);
		return ret;
	}

	else if (!stricmp(vp, "filterbar")) {
		if (!FilterSelectBar)
			FilterSelectBar = WriteFilterSelectBar();
		WriteText(wf);
		return FilterSelectBar;
	}


	else if (!stricmp(vp, "chapnum")
	 && ChapNum && *ChapNum)
		ret = ChapNum;
	else if (!stricmp(vp, "volnum")
	 && VolNum && *VolNum)
		ret = VolNum;
	else if (!stricmp(vp, "sectionnum")
	 && SectionNum && *SectionNum)
		ret = SectionNum;
	else if (!stricmp(vp, "subsectionnum")
	 && SSectNum && *SSectNum)
		ret = SSectNum;
	else if (!stricmp(vp, "pagenum")
	 && PageNum && *PageNum)
		ret = PageNum;

	else if (!stricmp(vp, "indexfilename"))
		ret = SetIndexFileName(CurrXFileBase);

	else if (!stricmp(vp, "ditastart"))
		ret = HXMwr::CurrDITATopic;

	else if (!stricmp(vp, "macroparam"))
		ret = DCmacr::MacroParam;

	if (ret)
		return NewName(ret);

	return NULL;
}


char *
HTMwr::GetFrameUserVar(char *vp)
{
	uns vnum = 0;

	if (((vnum = (uns) VarNameList(vp)) != 0)
	 || ((vnum = (uns) UserVarNameList(vp)) != 0))
		return GetVarText(vnum, false);
	return NULL;
}


long 
HTMwr::GetFrameUserArrVar(char *vp, long idx)
{
	if (!vp)
		return 0;

	long vnum = VarNameList.getitem(vp);
	if (!vnum)
		return 0;

	if (!idx)
		return VarArraySize.find(vnum);

	CurrVarArrayList = (DCvlist *) VarArrayList.find(vnum);
	CurrVarArrayForm = (HTMgroup *) CurrVarArrayList->find(idx);
	if (!CurrVarArrayForm)
		return 0;

  // borrow TitleBuf to get var text out
	uns tcount = TitleBufCount;
	char *txt = (char *) &TitleBuf[tcount];
	CurrVarArrayForm->execu(ProcTitleVarText);
	uns ncount = TitleBufCount - tcount;
	TitleBufCount = tcount;
	return (ncount ? (long) NewName(txt, ncount) : NULL);
}


char *
HTMwr::GetVarText(uns vnum, bool macvar)
{
	char *nm = NULL;
	char *txt = NULL;

	if (macvar
	 && DCmacr::ReplacingFrameVars()
	 && ((nm = VarNameList.find(vnum)) != NULL)
	 && ((txt = DCmacr::GetUserMacVar(nm, false)) != NULL))
		return txt;

  // borrow TitleBuf to get var text out
	uns tcount = TitleBufCount;
	txt = (char *) &TitleBuf[tcount];
	ProcTitleVar(vnum, false);
	uns ncount = TitleBufCount - tcount;
	TitleBufCount = tcount;
	return (ncount ? NewName(txt, ncount) : NULL);
}


bool HTMwr::NameUndefMacroVars = false;
bool HTMwr::NameUndefMacros = false;


bool
HTMwr::WriteMacroChar(DCwfile *df, unc uch)
{
	HTMwfile *wf = (HTMwfile *) df;
	
	if (uch == 183) {  // bullet
		wf->putSpec(uch);
		return true;
	}

	return false;
}


bool
HTMwr::WritePredefinedMacro(DCwfile *df, char *name)
{
	HTMwfile *wf = (HTMwfile *) df;
	long cdiv = 0;

	if (!stricmp(name, "prev"))
		WritePrevMacro(wf);
	else if (!stricmp(name, "next"))
		WriteNextMacro(wf);
	else if (!stricmp(name, "top"))
		DCmacr::WriteMacro(wf, *TopMacro);
	else if (!stricmp(name, "macroparam"))
		DCmacr::WriteMacro(wf, NewName(DCmacr::MacroParam));

	else if (!stricmp(name, "madewith"))
		WriteMadeWithMacro(wf);
	else if (!stricmp(name, "trail")
	 && MakeTrail)
		WriteTrail(wf, true);
	else if (!stricmp(name, "localtoc")
	 && MakeLocalTOC)
		WriteLocalTOC(wf);
	else if (!stricmp(name, "lastlocaltoc")
	 && LocalTOCDiv) {
		cdiv = wf->GetDiv();
		wf->WriteDiv(LocalTOCDiv);
		wf->SelectDiv(cdiv);
	}

	else if (!stricmp(name, "IncDropID")) {
		if (DropDownUsed)
			DropID++;
	}
	else if (!stricmp(name, "DropJS")) {
		if (DropDownUsed)
			WriteDropJSCode(wf);
	}
	else if (!stricmp(name, "DropLinkStart")) {
		if (DropDownUsed) {
			WritePendingANames(wf);
			StartDropLink();
			NoHyper = true;
			DCmacr::WriteMacro(wf, DropLinkStart);
		}
	}
	else if (!stricmp(name, "DropLinkEnd")) {
		if (DropDownUsed) {
			DCmacr::WriteMacro(wf, DropLinkEnd);
			NoHyper = false;
		}
	}
	else if (!stricmp(name, "DropLinkParaStart")) {
		if (DropDownUsed)
			DCmacr::WriteMacro(wf, DropLinkParaStart);
	}
	else if (!stricmp(name, "DropLinkParaText")) {
		if (DropDownUsed)
			DCmacr::WriteMacro(wf, DropLinkParaText);
	}
	else if (!stricmp(name, "DropLinkParaEnd")) {
		if (DropDownUsed)
			DCmacr::WriteMacro(wf, DropLinkParaEnd);
	}
	else if (!stricmp(name, "DropBlockStart")) {
		if (DropDownUsed)
			DCmacr::WriteMacro(wf, DropBlockStart);
	}
	else if (!stricmp(name, "DropBlockEnd")) {
		if (DropDownUsed)
			DCmacr::WriteMacro(wf, DropBlockEnd);
	}
	else if (!stricmp(name, "DropOpenIcon")) {
		if (DropDownUsed) {
			StartDropLink();
			DCmacr::WriteMacro(wf, DropOpenIcon);
		}
	}
	else if (!stricmp(name, "DropOpenIconFile")) {
		if (DropDownUsed)
			DCmacr::WriteMacro(wf, DropOpenIconFile);
	}
	else if (!stricmp(name, "DropOpenIconAlt")) {
		if (DropDownUsed)
			DCmacr::WriteMacro(wf, DropOpenIconAlt);
	}
	else if (!stricmp(name, "DropCloseIcon")) {
		if (DropDownUsed) {
			StartDropLink();
			DCmacr::WriteMacro(wf, DropCloseIcon);
		}
	}
	else if (!stricmp(name, "DropCloseIconFile")) {
		if (DropDownUsed)
			DCmacr::WriteMacro(wf, DropCloseIconFile);
	}
	else if (!stricmp(name, "DropCloseIconAlt")) {
		if (DropDownUsed)
			DCmacr::WriteMacro(wf, DropCloseIconAlt);
	}
	else if (!stricmp(name, "DropButton")) {
		if (DropDownUsed) {
			StartDropLink();
			DCmacr::WriteMacro(wf, DropButton);
		}
	}
	else if (!stricmp(name, "DropButtonOpenLabel")) {
		if (DropDownUsed)
			DCmacr::WriteMacro(wf, DropButtonOpenLabel);
	}
	else if (!stricmp(name, "DropButtonCloseLabel")) {
		if (DropDownUsed)
			DCmacr::WriteMacro(wf, DropButtonCloseLabel);
	}
	else if (!stricmp(name, "DropClass")) {
		if (DropDownUsed)
			DCmacr::WriteMacro(wf, DropClass);
	}
	else if (!stricmp(name, "DropText")) {
		if (DropDownUsed)
			DCmacr::WriteMacro(wf, DropText);
	}
	else if (!stricmp(name, "DropIDPrefix")) {
		if (DropDownUsed)
			DCmacr::WriteMacro(wf, DropIDPrefix);
	}
	else if (!stricmp(name, "DropLinkAttr")) {
		if (DropDownUsed)
			DCmacr::WriteMacro(wf, DropLinkAttr);
	}
	else if (!stricmp(name, "DropButtonAttr")) {
		if (DropDownUsed)
			DCmacr::WriteMacro(wf, DropButtonAttr);
	}
	else if (!stricmp(name, "DropDivAttr")) {
		if (ClickBlockToClose
		 && DropDownUsed)
			DCmacr::WriteMacro(wf, DropDivAttr);
	}

	else if (!stricmp(name, "ohprojhead"))
		WriteOHProjHead(wf);
	else if (!stricmp(name, "ohmerge"))
		WriteOHMergeInfo(wf);
	else if (!stricmp(name, "extrthumb"))
		WriteExtrThumb(wf);

	else if (!stricmp(name, "topicstartcode"))
		DCmacr::WriteMacro(wf, TopicStartCode);

	else
		return false;

	return true;
}


// evaluate macros used at start of topic

char *HTMwr::TopicStartCode = NULL;
DCvlist HTMwr::TopicStartMacros;


void 
HTMwr::EvalTopicStartMacros(HTMwfile *wf, long id)
{
	TopicStartCode = NULL;
	long cdiv = wf->GetDiv();
	long tsdiv = wf->NewDiv();

	TopicStartMacros.write(id, wf, WriteOneTopicStartMacro);

	TopicStartCode = wf->ReadDiv();
	wf->DeleteDiv(tsdiv);
	wf->SelectDiv(cdiv);
}


void 
HTMwr::WriteOneTopicStartMacro(DCfile *dwf, void *vp)
{
	DCctl *cp = (DCctl *) vp;
	char *name = (char *) cp->ex();
	size_t len = strlen(name);
	char *txt = (cp->siz() > (len + 2)) ? name + len + 1 : "";
	char *code = NewName(txt);

	ObjectID = MarkerIDList.find((long) cp);
	AddMarkerCode(name, &code);
	DCmacr::WriteMacro((HTMwfile *) dwf, code);
	DeleteName(code);
	ObjectID = NULL;
}



// handle macros in file names and titles

void
HTMwr::ProcNameTitleMacros(HTMwfile *wf)
{
  BaseNameList.writeallname((DCwfile *) wf, UseNameTitleMacro);
  SplitNameList.writeallname((DCwfile *) wf, UseSplitNameMacro);
  SplitTitleList.writeallname((DCwfile *) wf, UseNameTitleMacro);
	if (Title)
		UseNameTitleMacro((DCwfile *) wf, &Title, 1);
}

void
HTMwr::UseNameTitleMacro(DCwfile *w, char **np, long i)
{
	if (!i 
	 || (*np == NULL)
	 || !strstr(*np, "<$"))
		return;

	HTMwfile *wf = (HTMwfile *) w;
	char *chp = NULL;

	SplitID = i;
	sprintf(SplitIDRef, "%ld", SplitID);

	chp = DCmacr::GetMacro(*np);
	SplitID = 0;
	if (chp)
		*np = chp;
}

void
HTMwr::UseSplitNameMacro(DCwfile *w, char **np, long i)
{
	if (!i 
	 || (*np == NULL))
		return;

	HTMwfile *wf = (HTMwfile *) w;
	char *chp = NULL;

	SplitID = i;
	sprintf(SplitIDRef, "%ld", SplitID);

	chp = DCmacr::GetMacro(*np);
	if ((HelpType == 'M')
	 && !ExcludedTopics->find(SplitID)
	 && ExcludeFromSearch.find(SplitID)) {
		char *suf = strrchr(chp, '.');
		if (suf
		 && (*(suf + 1) == 'h')) {  // MSHH only searches files with "*.h*"
			short len = (short) (suf - chp);
			char *nchp = NewName(len + 7);
			strncpy(nchp, chp, len);
			strcat(nchp, ".xhtml");
			DeleteName(chp);
			chp = nchp;
		}
	}
	SplitID = 0;
	if (chp)
		*np = chp;

	long sid = 0;
	char buf[17];
	if (((sid = SplitNameList.getitem(*np)) != 0)
	 && (sid != i)) {
		LogEvent(logwarn, 1, "Duplicate split file name \"", *np, "\"");
		LogEvent(logcont, 1, "detected at ", SplitIDRef, " and ", ltoa(sid, buf, 10));
	}
}


// update titles for contents from split title list

bool HTMwr::UseNavtitleMarkers = false;

void
HTMwr::ProcContentsTitles(void)
{
	ContentsSplitList.all(UpdateContTitle);
}


void
HTMwr::UpdateContTitle(long cid, long sid)
{
	cntprop *cpt = (cntprop *) ContentsList.find(cid);
	char *txt = SplitTitleList.find(sid);
	if (!cpt
	 || !cpt->title
	 || !txt
	 || !strcmp(cpt->title, txt))
		return;
	DeleteName(cpt->title);
	cpt->title = NewName(txt);
}



// File Sequence

char *HTMwr::CurrFSTitle = NULL;
char **HTMwr::TopMacro = &TopLink;
char *HTMwr::TopTitle = "Top";

char *HTMwr::PrevFSBase = NULL;
char *HTMwr::PrevFSName = NULL;
char *HTMwr::PrevFSTitle = NULL;
char **HTMwr::PrevMacro = &PrevLink;
char **HTMwr::PrevFSMacro = &PrevFSLink;
char **HTMwr::StartingPrevFSMacro = &StartingPrevFSLink;
char *HTMwr::StartingPrevFSTitle = "At Start";

char *HTMwr::NextFSBase = NULL;
char *HTMwr::NextFSName = NULL;
char *HTMwr::NextFSTitle = NULL;
char **HTMwr::NextMacro = &NextLink;
char **HTMwr::NextFSMacro = &NextFSLink;
char **HTMwr::EndingNextFSMacro = &EndingNextFSLink;
char *HTMwr::EndingNextFSTitle = "At End";

char *HTMwr::TopLink = "<a href=\"<$$_currfile>\"><$$_toptitle></a>";
char *HTMwr::PrevLink = "<a href=\"<$$_prevfile>\"><$$_prevtitle></a>";
char *HTMwr::PrevFSLink = "<a href=\"<$$_seqprevfile>\"><$$_seqprevtitle></a>";
char *HTMwr::StartingPrevFSLink = "<$$_seqstarttitle>";
char *HTMwr::NextLink = "<a href=\"<$$_nextfile>\"><$$_nexttitle></a>";
char *HTMwr::NextFSLink = "<a href=\"<$$_seqnextfile>\"><$$_seqnexttitle></a>";
char *HTMwr::EndingNextFSLink = "<$$_seqendtitle>";

char *HTMwr::TopButton = "<button type=\"button\" onclick=\"javascript:location.href='<$$_currfile>'\"><$$_toptitle></button>";
char *HTMwr::PrevButton = "<button type=\"button\" onclick=\"javascript:location.href='<$$_prevfile>'\"><$$_prevtitle></button>";
char *HTMwr::PrevFSButton = "<button type=\"button\" onclick=\"javascript:location.href='<$$_seqprevfile>'\"><$$_seqprevtitle></button>";
char *HTMwr::StartingPrevFSButton = "<button type=\"button\"><$$_seqstarttitle></button>";
char *HTMwr::NextButton = "<button type=\"button\" onclick=\"javascript:location.href='<$$_nextfile>'\"><$$_nexttitle></button>";
char *HTMwr::NextFSButton = "<button type=\"button\" onclick=\"javascript:location.href='<$$_seqnextfile>'\"><$$_seqnexttitle></button>";
char *HTMwr::EndingNextFSButton = "<button type=\"button\"><$$_seqendtitle></button>";

DCilist HTMwr::PrevFSNameUsage;
DCilist HTMwr::PrevFSTitleUsage;
DCilist HTMwr::NextFSNameUsage;
DCilist HTMwr::NextFSTitleUsage;

bool HTMwr::FSSet = false;
bool HTMwr::FirstFS = false;
bool HTMwr::LastFS = false;
bool HTMwr::UseNavButtons = false;

void
HTMwr::SetNavMacro(char *macname, char **mac)
{
	char *ret = NULL;

	if (IniFile->Find(macname)) {
		if ((ret = IniFile->StrRight()) == NULL)
			ret = "";  // make these empty, not null
		*mac = ret;
	}
}


DCnlist HTMwr::FSNames;   // from [FS] or .lst file
DCnlist HTMwr::FSTitles;  // from [FS] or dup of name from file
long HTMwr::CurrFSPos = 0;
long HTMwr::LastFSPos = 0;


void
HTMwr::SetFileSeqNames(void)
{
	long pos = 0;
	char lbuf[LineMax];
	char *srcname = PrjFileName;
	bool noseq = false;

	if ((HelpType == 'M')
	 && DefaultHelpProjectFile)
		srcname = DefaultHelpProjectFile;
	else if ((HelpType == 'J')
	 && DefaultHelpSetFile)
		srcname = DefaultHelpSetFile;
	// for OH and Eclipse, book name used instead
	else if ((HelpType == 'C') || (HelpType == 'E')) {
		if (FrameBook)
			srcname = FrameBook;
	}

	if (srcname) {
		char *slash = strrchr(srcname, '/');
		if (!slash)
			slash = strrchr(srcname, '\\');
		char *tmpname = NewName(slash ? slash + 1 : srcname);
		StripSuf(tmpname);  // remove ".prj"
		char *lstname = NewName(tmpname, strlen(tmpname) + 4);
		DeleteName(tmpname);
		strcat(lstname, ".lst");
		DCrfile *lfile = new DCrfile(lstname);
		if (lfile) {
			if (lfile->FErr() == fok) {
				if (lfile->getLine(lbuf, LineMax)
				 && !strncmp(lbuf, "LIST for", 8)) {
					while (lfile->getLine(lbuf, LineMax)) {
						FSNames.addnew(StripSuf(lbuf), ++pos);
						FSTitles.addnew(lbuf, pos);
						if (!CurrFSPos
						 && !stricmp(lbuf, CurrFileName))
							CurrFSPos = pos;
					}
					LastFSPos = pos;
				}
			}
			else
				noseq = true;
			delete lfile;
		}
		if (!pos
		 && !noseq
		 && !HXMwr::DITA      // doesn't matter for DITA, not used anyway
		 && !HXMwr::DocBook
		 //&& FMSource
		 && (RefFileType != 'F'))
			LogEvent(logwarn, 1, "Failed to get file sequence from ", lstname);
		DeleteName(lstname);
	}

	if (!pos  // file operation failed, try .ini
	 && IniFile->Section("FileSequence")
	 && IniFile->Find(CurrFileName)) {
		CurrFSTitle = IniFile->StrRight();
		CurrFSPos = IniFile->GetPos();
		IniFile->Section("FileSequence");
		while (IniFile->NextItem()) {
			FSNames.add(IniFile->StrLeft(), ++pos);
			FSTitles.add(IniFile->StrRight(), pos);
		}
		LastFSPos = pos;
	}

	if (!pos  // complete failure, inter-file prev/next links broken
	 && !noseq
	 && !HXMwr::DITA      // doesn't matter for DITA, not used anyway
	 && !HXMwr::DocBook
	 //&& FMSource
	 && (RefFileType != 'F'))
		LogEvent(logwarn, 1, "Failed to set file sequence");
}


void
HTMwr::SetFileSequence(void)
{
	SetFileSeqNames();

	if (!CurrFSPos)
		return;

	size_t slen = strlen(XrefFileSuffix) + 2;

	if (CurrFSPos == 1) {
		FirstFS = true;
		PrevFSName = CurrXFileName;
		PrevFSTitle = StartingPrevFSTitle;
	}
	else {
		PrevFSBase = FSNames.find(CurrFSPos - 1);
		PrevFSName = NewName(PrevFSBase, strlen(PrevFSBase) + slen);
		strcat(PrevFSName, ".");
		strcat(PrevFSName, XrefFileSuffix);
		PrevFSTitle = FSTitles.find(CurrFSPos - 1);
	}

	if (CurrFSPos == LastFSPos) {
		LastFS = true;
		NextFSName = CurrXFileName;
		NextFSTitle = EndingNextFSTitle;
	}
	else {
		NextFSBase = FSNames.find(CurrFSPos + 1);
		NextFSName = NewName(NextFSBase, strlen(NextFSBase) + slen);
		strcat(NextFSName, ".");
		strcat(NextFSName, XrefFileSuffix);
		NextFSTitle = FSTitles.find(CurrFSPos + 1);
	}

	FSSet = true;
}


char *
HTMwr::GetPrevFSName(HTMwfile *wf)
{
	if (!CurrFSPos)
		return CurrXFileName;

	if (FirstFS) 
		return StartSplit ? SplitNameList.find(FirstSplitID) : CurrXFileName;

	long fpos = wf->tell();

	if (fpos)
		PrevFSNameUsage.append(fpos, SplitID ? SplitID : 1);

	return PrevFSName;
}


char *
HTMwr::GetPrevFSTitle(HTMwfile *wf)
{
	if (!CurrFSPos)
		return CurrXFileName;

	if (FirstFS) 
		return StartingPrevFSTitle;

	long fpos = wf->tell();

	if (fpos)
		PrevFSTitleUsage.append(fpos, SplitID ? SplitID : 1);

	return PrevFSTitle;
}


char *
HTMwr::GetNextFSName(HTMwfile *wf)
{
	if (!CurrFSPos)
		return CurrXFileName;

	if (LastFS) 
		return SplitID ? SplitNameList.find(SplitID) : CurrXFileName;

	long fpos = wf->tell();

	if (fpos)
		NextFSNameUsage.append(fpos, SplitID ? SplitID : 1);

	return NextFSName;
}


char *
HTMwr::GetNextFSTitle(HTMwfile *wf)
{
	if (!CurrFSPos)
		return CurrXFileName;

	if (LastFS) 
		return EndingNextFSTitle;

	long fpos = wf->tell();

	if (fpos)
		NextFSTitleUsage.append(fpos, SplitID ? SplitID : 1);

	return NextFSTitle;
}



void
HTMwr::WritePrevMacro(HTMwfile *wf)
{
	wf->putEnd();

	if (FirstPart  // at first (or nonsplit) section
	 && FirstFS) 
		DCmacr::WriteMacro(wf, *StartingPrevFSMacro);
	else if (FirstPart)
		DCmacr::WriteMacro(wf, *PrevFSMacro);
	else
		DCmacr::WriteMacro(wf, *PrevMacro);

	wf->putEnd();
}


void
HTMwr::WriteNextMacro(HTMwfile *wf)
{
	wf->putEnd();

	if (LastPart  // at last (or nonsplit) section
	 && LastFS) 
		DCmacr::WriteMacro(wf, *EndingNextFSMacro);
	else if (LastPart)
		DCmacr::WriteMacro(wf, *NextFSMacro);
	else
		DCmacr::WriteMacro(wf, *NextMacro);

	wf->putEnd();
}


// made with mif2go image

bool HTMwr::MadeWithUsed = false;
bool HTMwr::WriteMadeWithFile = true;
char *HTMwr::MadeWithImageFile = "madewithm2g.jpg";
size_t HTMwr::MadeWithImageSize = 2271;

bool HTMwr::MadeWithPara = true;
bool HTMwr::MadeWithLink = true;

char *HTMwr::MadeWithAttributes = {
 "border=\"0\" alt=\"Made with Mif2Go\" height=\"48\" width=\"78\""
};


void
HTMwr::WriteMadeWithImage(void)
{
	if (!MadeWithUsed
	 || !WriteMadeWithFile)
		return;

	WriteResGraphic(MadeWithImageFile, IDR_GRAPHIC1, MadeWithImageSize);
}


void
HTMwr::WriteResGraphic(char *fname, const int gid, size_t imsize)
{
	DCwfile mwi(fname, fbin);
  if (mwi.FErr() != fok)
		return;

	HMODULE mwmod = GetModuleHandle((LPCTSTR) "dwhtm.dll");
	if (!mwmod) {
		mwi.CloseFile();
		return;
	}

	HRSRC mwres = FindResource(mwmod, (const char *) gid, "graphic");
	if (!mwres) {
		mwi.CloseFile();
		return;
	}

	HGLOBAL mwglob = LoadResource(mwmod, mwres);
	if (!mwglob) {
		mwi.CloseFile();
		return;
	}

	LPVOID mwptr = LockResource(mwglob);
	if (!mwptr) {
		mwi.CloseFile();
		return;
	}

	size_t mwsz = (size_t) SizeofResource(mwmod, mwres);
	if (mwsz < imsize) {  // mwsz is rounded up
		mwi.CloseFile();
		return;
	}

	mwi.putBlock(mwptr, imsize);

	mwi.CloseFile();
}


void
HTMwr::WriteMadeWithMacro(HTMwfile *wf)
{
	MadeWithUsed = true;
	if (MadeWithPara)
		wf->putStr("<p>");
	if (MadeWithLink)
		wf->putStr("<a href=\"http://www.omsys.com/\">");

	wf->putStr("<img src=\"");
	wf->putStr(MadeWithImageFile);
	wf->putStr("\" ");
	wf->putStr(MadeWithAttributes);
	wf->putStr(" />");

	if (MadeWithLink)
		wf->putStr("</a>");
	if (MadeWithPara)
		wf->putStr("</p>");
}



// local TOCs

bool HTMwr::MakeLocalTOC = false;
char *HTMwr::LocalTOCStart = { "<p class=\"localtocstart\">In this section:</p>" };
char *HTMwr::LocalTOCItem = { "<p class=\"localtocitem\"><a href=\"<$$loctocfile>\"><$$loctoctitle></a></p>" };
char *HTMwr::LocalTOCEnd = { "<br>" };
long HTMwr::LocalTOCID = 0;         // set only during write of <$_localtoc>
long HTMwr::LocalTOCLevel = 0;      // set if split para format is in [LocalTOCLevels]
bool HTMwr::LocalTOCSubs = false;
long HTMwr::LocalTOCIndent = 0;     // set if subs for indent level of each item
long HTMwr::LocalTOCDiv = 0;        // set when new <$_localtoc> created
long HTMwr::LocalTOCSeq = 0;        // set when LocalTOC item added
DCilist HTMwr::CurrLocalTOCList;    // LocalTOCLevel -> last SplitID for it
DCilist HTMwr::LocalTOCParentList;  // SplitID -> parent SplitID
DCnlist HTMwr::LocalTOCTitleList;   // SplitID -> title or LocalTOCTitle marker
DCilist HTMwr::LocalTOCSeqList;     // Sequence  -> last SplitID for it
DCilist HTMwr::LocalTOCFmtList;     // SplitID -> format number for it


void
HTMwr::AddLocalTOCItem(void)
{
	// called during scan phase at end of starting split file para if MakeLocalTOC

	if (!LocalTOCLevel)
		return;

	long parent = 0;
	long level = 0;
	long seq = ++LocalTOCSeq;
	long id = 0;

	while (--seq > 0) {
		if (((id = LocalTOCSeqList.find(seq)) != 0)
		 && ((level = CurrLocalTOCList(id)) < LocalTOCLevel)
		 && (LocalTOCFmtList(id) != CurrFormNum)) {
			parent = id;
			break;
		}
	}

#if 0
	while (--level) {  // just plain wrong
		if ((parent = CurrLocalTOCList.find(level)) != NULL)
			break;
	}	
#endif

	if (parent)
		LocalTOCParentList.append(SplitID ? SplitID : 1, parent);
	CurrLocalTOCList.append(SplitID ? SplitID : 1, LocalTOCLevel);
	LocalTOCSeqList.append(SplitID ? SplitID : 1, LocalTOCSeq);
	LocalTOCFmtList.append(SplitID ? SplitID : 1, CurrFormNum);
}


void
HTMwr::WriteLocalTOC(HTMwfile *wf)
{
	// invoked from $_localtoc macro during write phase if MakeLocalTOC

	if (!LocalTOCParentList.find(SplitID ? SplitID : 1))
		return;

	long prevdiv = wf->GetDiv();
	if (LocalTOCDiv)
		wf->DeleteDiv(LocalTOCDiv);
	LocalTOCDiv = wf->NewDiv();

	char *txt = NULL;

	wf->putEnd();
	DCmacr::WriteMacro(wf, LocalTOCStart);
	LocalTOCIndent = 0;

	// write each tocitem that has current SplitID (or 1 if at top) as parent
	LocalTOCParentList.write(SplitID ? SplitID : 1, (DCwfile *) wf, WriteLocalTOCItem);

	wf->putEnd();
	DCmacr::WriteMacro(wf, LocalTOCEnd);
	wf->putEnd();

	wf->SelectDiv(prevdiv);
	wf->WriteDiv(LocalTOCDiv);
	wf->SelectDiv(prevdiv);
}


void
HTMwr::WriteLocalTOCItem(DCwfile *pwf, long v)
{
	HTMwfile *wf = (HTMwfile *) pwf;

	if (LocalTOCID == v)
		return;  // prevent infinite recursion
	LocalTOCID = v;
	LocalTOCIndent++;
	wf->putEnd();
	DCmacr::WriteMacro(wf, LocalTOCItem);
	if (LocalTOCSubs
	 && LocalTOCParentList.find(v))
		LocalTOCParentList.write(v, pwf, WriteLocalTOCItem);
	LocalTOCIndent--;
	LocalTOCID = 0;
}


// runtime condition filtering

bool HTMwr::FilterConditions = false;
char *HTMwr::FilterJSFile = "runcond.js";

char *HTMwr::FilterDialogTitle = "Select conditions to hide:";
char *HTMwr::FilterBodyAttr = "onclick=\"dialogWin.checkModal()\" onfocus=\"return dialogWin.checkModal()\"";
char *HTMwr::FilterLinkHref = "javascript:SetCondAttrs()";
uns HTMwr::FilterDialogWide = 250;
uns HTMwr::FilterDialogHigh = 300;

char *HTMwr::FilterSelectBar = NULL;
char *HTMwr::FilterBarStart = "Hide:";
char *HTMwr::FilterBarSpan = "<span class=\"show %s\" style=\"cursor:pointer;color:blue;\" onclick='ToggleShowHide(\"%s\")'>%s</span>";
char *HTMwr::FilterBarEnd = "or Show: <span class=\"show\" style=\"cursor:pointer;color:blue;\" onclick='ShowAll()'>All</span>";
char *HTMwr::FilterBarBodyAttr = "onload=\"SetDocConditions()\"";


// in drxml, drmxl ,and drmif: if attribute is in list, ****
// keep element and write class attr for it
// in dwhtm, just write as CondAttrs
DCnlist HTMwr::FilterDialogAttributes;


void
HTMwr::SetFilterAttributes(void)
{
	if (!FilterConditions)
		return;

	// read [FilterAttributes] and populate FilterDialogAttributes
	if (!IniFile
	 || !IniFile->Section("FilterAttributes"))
		return;
	while (IniFile->NextItem()) {
		char *aname = IniFile->StrLeft();
		char *vlist = IniFile->StrRight();
		DCnlist *npt = new DCnlist();
		npt->addlist(vlist);
		FilterDialogAttributes.add(aname, (long) npt);
		while ((npt = npt->next) != NULL) {
			npt->id *= 2;
			if (*(npt->name) == '-') {
				npt->name++;
				npt->id ++;
			}
		}
	}
}


void 
HTMwr::WriteFilterDialogSettings(DCwfile *wf)
{
	if (!FilterConditions)
		return;

	DCnlist *npt = &FilterDialogAttributes;
	DCnlist *vpt = NULL;

	// write var CondAttrs to top of HTML files, or to OH file *_ohr.js
	wf->putStr("var CondAttrs = [\n");
	while ((npt = npt->next) != NULL) {
		wf->putStr("\t[\"");
		wf->putStr(npt->name);
		wf->putStr("\", [ ");
		vpt = (DCnlist *) npt->id;
		while ((vpt = vpt->next) != NULL) {
			wf->putStr("[\"");
			wf->putStr(vpt->name);
			wf->putStr("\", ");
			wf->putChar((vpt->id & 1) + '0');
			wf->putStr("]");
			if (vpt->next)
				wf->putStr(", ");
			else
				wf->putStr(" ]");
		}
		wf->putStr(" ]");
		if (npt->next)
			wf->putStr(",\n");
		else
			wf->putStr(" ];\n");
	}
}


char * 
HTMwr::WriteFilterSelectBar(void)
{
	if (!FilterConditions)
		return NULL;

	// invoke from predefined macro var <$$_FilterBar>
	// write out the contents of the selection bar
	char bar[FilterSelectBarMax];
	char *cpt = bar;
	DCnlist *attr = &FilterDialogAttributes;
	const char *aname = NULL;
	const char *aval = NULL;
	DCnlist *vlist = NULL;
	char *cstr = NULL;
	char *tstr = NULL;
	bool useini = false;

	// write start from [FilterConditions]FilterBarStart
	if (FilterBarStart) {
		strcpy(cpt, FilterBarStart);
		strcat(cpt, " \n");
	}
	// write [FilterConditions]FilterBarAll
	//if (FilterBarAll)
	//	strcat(cpt, FilterBarAll);

	// for each name/value:
	//   write span per format str in [FilterConditions]
	//   span text is in [FilterValueText] indexed by attr_value
	if (IniFile
	 && IniFile->Section("FilterValueText"))
		useini = true;

	while ((attr = attr->next) != NULL) {
		aname = attr->name;
		vlist = (DCnlist *) attr->id;
		while ((vlist = vlist->next) != NULL) {
			aval = vlist->name;
			cstr = NewName(aname, strlen(aname) + strlen(aval) + 2);
			strcat(cstr, "_");
			strcat(cstr, aval);
			if (useini && IniFile->Find(cstr))
				tstr = IniFile->StrRight();
			else
				tstr = cstr;
			cpt += strlen(cpt);
			sprintf(cpt, FilterBarSpan, cstr, cstr, tstr ? tstr : cstr);
			if (tstr != cstr)
				DeleteName(tstr);
			DeleteName(cstr);
			tstr = cstr = NULL;
			strcat(cpt, "\n");
		}
	}

	// write end from [FilterConditions]FilterBarEnd
	if (FilterBarEnd) {
		strcat(cpt, " ");
		strcat(cpt, FilterBarEnd);
	}

	return NewName(bar);
}



void 
HTMwr::WriteFilterDisplayStyle(HTMwfile *wf)
{
	// write style attribute value "display: none;"
	if (!FilterConditions)
		return;

	wf->strAttr("style", "display: none;");
}



// dropdowns

bool HTMwr::DropDownUsed = false;
unc HTMwr::DropLinkType = 'I';
char *HTMwr::DropLinkStart = "<a class=\"<$DropClass>\"\n <$DropLinkAttr>>";
char *HTMwr::DropLinkEnd = "</a>";
char *HTMwr::DropLinkParaStart = "<p class=\"<$DropClass>\">";
char *HTMwr::DropLinkParaText = "<$DropText>";
char *HTMwr::DropLinkParaEnd = "</p>\n";
char *HTMwr::DropBlockStart = "<div class=\"<$DropClass>\" id=\"<$$_DropID>\" style=\"display:none;\" <$DropDivAttr>>\n";
char *HTMwr::DropBlockEnd = "</div>\n";
bool HTMwr::ClickBlockToClose = true;
bool HTMwr::WriteDropIconFiles = false;
char *HTMwr::DropOpenIcon = "<img\n src=\"<$DropOpenIconFile>\" id=\"io<$$_DropID>\" style=\"border:0;\" alt=\"<$DropOpenIconAlt>\">";
char *HTMwr::DropOpenIconFile = "dropopen.gif";
char *HTMwr::DropOpenIconAlt = "Click to open.";
char *HTMwr::DropCloseIcon = "<img\n src=\"<$DropCloseIconFile>\" id=\"ic<$$_DropID>\" style=\"display:none;border:0;\" alt=\"<$DropCloseIconAlt>\">\n";
char *HTMwr::DropCloseIconFile = "dropclose.gif";
char *HTMwr::DropCloseIconAlt = "Click to close.";
char *HTMwr::DropButton = "\n<button type=\"button\" class=\"<$DropClass>\" id=\"bu<$$_DropID>\" <$DropButtonAttr>><$DropButtonOpenLabel></button>\n";
char *HTMwr::DropButtonOpenLabel = "More";
char *HTMwr::DropButtonCloseLabel = "Less";
char *HTMwr::DropClass = "dropdown";
char *HTMwr::DropText = "Click here.";
char *HTMwr::DropIDPrefix = "drop";
char *HTMwr::DropLinkAttr = "href=\"javascript:doSection('<$$_DropID>');void 0;\"";
char *HTMwr::DropButtonAttr = "onclick=\"doSection('<$$_DropID>')\"";
char *HTMwr::DropDivAttr = "onclick=\"noSection('<$$_DropID>')\"";
char *HTMwr::DropJSCode = "<$DropJS>";
char *HTMwr::DropJSLocation = "Head";
bool HTMwr::WriteDropJSFile = false;
bool HTMwr::UseCompositeDropJS = true;


char *HTMwr::DropJSCodeComp = {
 "function doSection(id){\n"
 " var but = document.getElementById(\"bu\" + id)\n"
 " var imop = document.getElementById(\"io\" + id)\n"
 " var imcl = document.getElementById(\"ic\" + id)\n"
 " var idiv = document.getElementById(id)\n"
 " if (idiv.style.display==\"none\") {\n"
 "  idiv.style.display=\"\"\n"
 "  if (but != null)\n"
 "	 but.innerHTML=\"<$DropButtonCloseLabel>\"\n"
 "  if (imop != null)\n"
 "   imop.style.display=\"none\"\n"
 "  if (imcl != null)\n"
 "   imcl.style.display=\"\"\n"
 " } else {\n"
 "  idiv.style.display=\"none\"\n"
 "  if (but != null)\n"
 "	 but.innerHTML=\"<$DropButtonOpenLabel>\"\n"
 "  if (imop != null)\n"
 "   imop.style.display=\"\"\n"
 "  if (imcl != null)\n"
 "   imcl.style.display=\"none\"\n"
 " }\n"
 " return false;\n"
 "}\n"
 "function noSection(id){\n"
 " var but = document.getElementById(\"bu\" + id)\n"
 " var imop = document.getElementById(\"io\" + id)\n"
 " var imcl = document.getElementById(\"ic\" + id)\n"
 " var idiv = document.getElementById(id)\n"
 " if (idiv.style.display==\"\") {\n"
 "  idiv.style.display=\"none\"\n"
 "  if (but != null)\n"
 "	 but.innerHTML=\"<$DropButtonOpenLabel>\"\n"
 "  if (imop != null)\n"
 "   imop.style.display=\"\"\n"
 "  if (imcl != null)\n"
 "   imcl.style.display=\"none\"\n"
 " }\n"
 "}\n"
};


char *HTMwr::DropJSCodeIcon = {
 "function doSection(id){\n"
 " var imop = document.getElementById(\"io\" + id)\n"
 " var imcl = document.getElementById(\"ic\" + id)\n"
 " var idiv = document.getElementById(id)\n"
 " if (idiv.style.display==\"none\") {\n"
 "  idiv.style.display=\"\"\n"
 "  imop.style.display=\"none\"\n"
 "  imcl.style.display=\"\"\n"
 " } else {\n"
 "  idiv.style.display=\"none\"\n"
 "  imop.style.display=\"\"\n"
 "  imcl.style.display=\"none\"\n"
 " }\n"
 " return false;\n"
 "}\n"
 "function noSection(id){\n"
 " var imop = document.getElementById(\"io\" + id)\n"
 " var imcl = document.getElementById(\"ic\" + id)\n"
 " var idiv = document.getElementById(id)\n"
 " if (idiv.style.display==\"\") {\n"
 "  idiv.style.display=\"none\"\n"
 "  imop.style.display=\"\"\n"
 "  imcl.style.display=\"none\"\n"
 " }\n"
 "}\n"
};

char *HTMwr::DropJSCodeText = {
 "function doSection(id){\n"
 " var idiv = document.getElementById(id)\n"
 " if (idiv.style.display==\"none\") {\n"
 "  idiv.style.display=\"\"\n"
 " } else {\n"
 "  idiv.style.display=\"none\"\n"
 " }\n"
 " return false;\n"
 "}\n"
 "function noSection(id){\n"
 " var idiv = document.getElementById(id)\n"
 " if (idiv.style.display==\"\") {\n"
 "  idiv.style.display=\"none\"\n"
 " }\n"
 "}\n"
};

char *HTMwr::DropJSCodeButton = {
 "function doSection(id){\n"
 " var but = document.getElementById(\"bu\" + id)\n"
 " var idiv = document.getElementById(id)\n"
 " if (idiv.style.display==\"none\") {\n"
 "  idiv.style.display=\"\"\n"
 "	but.innerHTML=\"<$DropButtonCloseLabel>\"\n"
 " } else {\n"
 "  idiv.style.display=\"none\"\n"
 "	but.innerHTML=\"<$DropButtonOpenLabel>\"\n"
 " }\n"
 " return false;\n"
 "}\n"
 "function noSection(id){\n"
 " var but = document.getElementById(\"bu\" + id)\n"
 " var idiv = document.getElementById(id)\n"
 " if (idiv.style.display==\"\") {\n"
 "  idiv.style.display=\"none\"\n"
 "	but.innerHTML=\"<$DropButtonOpenLabel>\"\n"
 " }\n"
 "}\n"
};

char *HTMwr::DropJSHead = {
 "<script language=\"JavaScript\" type=\"text/javascript\">\n"
 "<!--\n"
 "<$DropJS>\n"
 "//--> \n"
 "</script>\n"
};

char *HTMwr::DropJSFile = {
 "<script language=\"JavaScript\" type=\"text/javascript\"\n"
 " src=\"<$DropJSLocation>\"></script>\n"
};

long HTMwr::DropID = 0;  // <$_IncDropID> increments, <$$_DropID> gets this
bool HTMwr::DropLinkStarted = false;
bool HTMwr::DropLinkCharSet = false;


void
HTMwr::WriteDropJSCode(HTMwfile *wf)
{
	if (!DropDownUsed)
		return;

	char *jcode = DropJSCodeIcon;

	if (UseCompositeDropJS)
		jcode = DropJSCodeComp;

	else switch (DropLinkType) {
		case 'T':
			jcode = DropJSCodeText;
			break;

		case 'B':
			jcode = DropJSCodeButton;
			break;

		case 'I':
		default:
			break;
	}

	DCmacr::WriteMacro(wf, jcode);
}


void
HTMwr::WriteDropDownHead(HTMwfile *wf)
{
	if (!DropDownUsed)
		return;

	if (!stricmp(DropJSLocation, "none")) {
		WriteDropJSFile = false;
		return;
	}

	if (!stricmp(DropJSLocation, "head")) {
		DCmacr::WriteMacro(wf, DropJSHead);
		WriteDropJSFile = false;
		return;
	}

	// must be file name, reference it
	DCmacr::WriteMacro(wf, DropJSFile);
}


void
HTMwr::WriteDropDownJSFile(void)
{
	if (!DropDownUsed)
		return;

	char *dest = DropJSLocation; // strip path and write code
	char *fn = strrchr(DropJSLocation, '\\');
	if (!fn)
		fn = strrchr(DropJSLocation, '/');
	if (fn)
		dest = fn + 1;

	HTMwfile *dwf = new HTMwfile(dest);

	if (dwf->FErr() == fok) {
		DCmacr::InitMacroModule(dwf, IniFile, MacroFile,
		 WriteMacroChar, WritePredefinedMacro,
		 WriteCodeStoreVar, GetCodeStoreVar,
		 GetTextStore, GetPredefinedVar,
		 GetFrameUserVar, GetFrameUserArrVar,
		 ValidOnly);
		DCmacr::WriteMacroTextSingleChar = &WriteTextSingleChar;
		WriteDropJSCode(dwf);
		dwf->CloseFile();
	}
}


void
HTMwr::WriteDropLinkPara(HTMwfile *wf)
{
	if (!DropDownUsed)
		return;

	DCmacr::WriteMacro(wf, "<$DropLinkParaStart>");
	WriteDropLinkStart(wf);
	if (DropLinkType != 'B') {
		DCmacr::WriteMacro(wf, "<$DropLinkParaText>");
		WriteDropLinkEnd(wf);
	}
	DCmacr::WriteMacro(wf, "<$DropLinkParaEnd>");
	DropLinkStarted = false;
}


void
HTMwr::StartDropLink(void)
{
	if (!DropLinkStarted) {
		DropID++;
		DropLinkStarted = true;
	}
}


void
HTMwr::WriteDropLinkStart(HTMwfile *wf)
{
	if (!DropDownUsed)
		return;

	if (DropLinkStarted)
		return;

	WritePendingANames(wf);

	switch (DropLinkType) {
		case 'B':
			DCmacr::WriteMacro(wf, "<$DropButton>");
			break;

		case 'T':
			NoHyper = true;
			DCmacr::WriteMacro(wf, "<$DropLinkStart>");
			break;

		case 'I':
		default:
			NoHyper = true;
			DCmacr::WriteMacro(wf, "<$DropLinkStart>");
			DCmacr::WriteMacro(wf, "<$DropOpenIcon>");
			DCmacr::WriteMacro(wf, "<$DropCloseIcon>");
			break;
	}
}


void
HTMwr::WriteDropLinkEnd(HTMwfile *wf)
{
	if (!DropDownUsed)
		return;

	switch (DropLinkType) {
		case 'B':  // not used for button
			break;

		case 'T':
		case 'I':
		default:
			DCmacr::WriteMacro(wf, "<$DropLinkEnd>");
			NoHyper = false;
			break;
	}
}


void
HTMwr::WriteDropLinkIcons(void)
{
	if (!DropDownUsed)
		return;

	if (!WriteDropIconFiles)
		return;

	WriteResGraphic(DropOpenIconFile, IDR_GRAPHIC2, 856);
	WriteResGraphic(DropCloseIconFile, IDR_GRAPHIC3, 859);
}



// trails


bool HTMwr::MakeTrail = false;
bool HTMwr::SplitTrail = false;
char *HTMwr::TrailStart = { "<p><em>" };
char *HTMwr::TrailSep = { "&nbsp;&gt;&nbsp;" };
char *HTMwr::TrailEnd = { "</em></p>" };
char *HTMwr::TrailLinkClass = NULL;
char *HTMwr::TrailPrefix = NULL;
char *HTMwr::TrailSuffix = NULL;
unc HTMwr::TrailCurrent = 'Y';
unc HTMwr::TrailPosition = 'B';
uns HTMwr::TrailFirstLevel = TrailLevMin;
uns HTMwr::TrailLastLevel = TrailLevMax;
uns HTMwr::TrailCurrLevel = TrailLevMin;
uns HTMwr::TrailIndent = 0;

long HTMwr::TrailParas[TrailLevMax]; // paragraphs to use
DCilist HTMwr::TrailSplitIDs;
DCvlist HTMwr::TrailParaList;
DCilist HTMwr::TrailLevelList;
DCnlist HTMwr::TrailTitleList;


void
HTMwr::AddTrailItem(void)
{
	// called at end of para in scan phase

	if (TrailCurrLevel > TrailLevMax)
		return;

	// store para id
	TrailParas[TrailCurrLevel - 1] = ParaRefNum;
	for (uns i = TrailCurrLevel; i < TrailLevMax; i++)
		TrailParas[i] = 0;
	TrailSplitIDs.append(Extracting ? ExtrID : (SplitID ? SplitID : 1), ParaRefNum);

	long *paras = new long[TrailLevMax];
	memcpy(paras, TrailParas, TrailLevMax * sizeof(long));
	TrailParaList.add(paras, ParaRefNum);
	TrailLevelList.append(TrailCurrLevel, ParaRefNum);
}


void
HTMwr::WriteTrail(HTMwfile *wf, bool macro)
{
	// create on fly here, before or after para or at <$$trail>

	long pid = 0;
	long fid = 0;
	char *nm = NULL;
	char *title = NULL;
	char *trail = NULL;
	bool started = false;
	long pnum = ParaRefNum;

	uns trailend = (uns) TrailLevelList.find(pnum);
	if (macro) {  // scan forward to first 
		while ((trailend == 0) && (pnum <= ItemRefNum))
			trailend = (uns) TrailLevelList.find(++pnum);
	}
	if (trailend == 0)
		return;
	uns last = trailend;
	if (TrailCurrent == 'N')
		last--;

	// generate trail ending at current para
	long *paras = (long *) TrailParaList.find(pnum);
	if (!paras)
		return;
	memcpy(TrailParas, paras, TrailLevMax * sizeof(long));
	uns step = 1;
	if (TrailIndent > 4)
		TrailIndent = 4;

	for (uns i = TrailFirstLevel; i <= last; i++) {
		if (((pid = TrailParas[i - 1]) == 0)  // missing level
		 || ((fid = TrailSplitIDs.find(pid)) == 0))  // unknown part ID
			continue;

		if (!started) {
			if ((i == trailend)         // nothing before current para
			 && (TrailCurrent == 'Y'))  // not set to "Always" (or "None")
				break;
			trail = NewName(TrailLenMax);
			strcpy(trail, TrailStart);
			started = true;
		}

		if (i < trailend) {  // link only for ones before current para
			strcat(trail, "<a ");
			if (TrailLinkClass) {
				strcat(trail, "class=\"");
				strcat(trail, TrailLinkClass);
				strcat(trail, "\" ");
			}
			strcat(trail, "href=\"");
			nm = (fid == 1) ? CurrXFileName : SplitNameList.find(fid);
			strcat(trail, nm ? nm : CurrXFileName);
			strcat(trail, "\">");
		}

		title = TrailTitleList.find(fid);
		strcat(trail, title ? title : CurrXFileTitle);

		if (i < trailend)
			strcat(trail, "</a>");

		if (i < last)
			strcat(trail, TrailSep);

		if (TrailIndent) {
			for (uns ic = 1 ; ic < (step * TrailIndent) ; ic++)
				strcat(trail, "&nbsp;");
			step++;
		}
	}

	if (started) {
		wf->putEnd();
		strcat(trail, TrailEnd);
		DCmacr::WriteMacro(wf, trail);
		wf->putEnd();
		DeleteName(trail);
	}
}



// macro template pages

void 
HTMwr::WriteTemplatePages(void)
{
	// go through [Templates] and write a file for each line

	if (!IniFile->Section("MacroTemplates"))
		return;

	char *fname = NULL;
	char *macro = NULL;
	DCnlist fnames;
	DCnlist macros;
	long cnt = 0;

	while (IniFile->NextItem()) {
		fname = IniFile->StrLeft();
		macro = IniFile->StrRight();
		if (!fname || !*fname
		 || !macro || !*macro)
			continue;
		fnames.add(fname, ++cnt);
		macros.add(macro, cnt);
	}

	HTMwfile *mwf = NULL;

	for (long i = 1; i <= cnt; i++) {
		fname = fnames.find(i);
		if (*fname == '<') // name is a macro
			fname = DCmacr::GetMacro(fname);
		macro = macros.find(i);
		mwf = new HTMwfile(fname);
		if (!mwf
		 || (mwf->FErr() != fok)) {
			LogEvent(logwarn, 1, "Cannot write template file ", fname);
			continue;
		}
		DCmacr::WriteMacroName(mwf, &macro);
		mwf->CloseFile();
	}
}



// list markers

DCnlist HTMwr::MarkList;
DCnlist HTMwr::MarkTypeList;
long HTMwr::MarkTypes = 0;

DCvlist HTMwr::CellAttrLists; // associate CellCpID with a DCnlist
DClist HTMwr::CellColGroupList;
DClist HTMwr::CellRowGroupList;
DClist HTMwr::CellSpanList;
DCnlist HTMwr::CellScopeList;
DCnlist HTMwr::CellIDList;

DCvlist HTMwr::RowAttrLists;  // associate RowCpID with a DCnlist
DCvlist HTMwr::TblAttrLists;  // associate TblCpID with a DCnlist
DCvlist HTMwr::ParaAttrLists;  // associate ParaRefNum with a DCnlist
DCvlist HTMwr::CharAttrLists;  // associate CharRefNum with a DCnlist
DCvlist HTMwr::LinkAttrLists;  // associate link id with a DCnlist
DCvlist HTMwr::JH2PopAttrLists;  // associate JH2Pop id with a DCnlist
DCvlist HTMwr::JH2SecAttrLists;  // associate JH2Sec id with a DCnlist
DCnlist *HTMwr::CellAttrLp = NULL;
DCnlist *HTMwr::RowAttrLp = NULL;
DCnlist *HTMwr::TblAttrLp = NULL;
DCnlist *HTMwr::GraphAttrLp = NULL;
DCnlist *HTMwr::LinkAttrLp = NULL;
DCnlist *HTMwr::JH2PopAttrLp = NULL;
DCnlist *HTMwr::JH2SecAttrLp = NULL;
DCnlist *HTMwr::ParaAttrLp = NULL;
DCnlist *HTMwr::CharAttrLp = NULL;

bool HTMwr::AttrHere = false;
char *HTMwr::AttrMarkerName = NULL;
char *HTMwr::NoProp = { "No" };


void
HTMwr::ProcMarkerGroup(HTMctl *cp)
{
}


void
HTMwr::ProcMarker(HTMctl *cp)
{
	MarkName = (char *) cp->ex();
	MarkerSetSplit = false;
	MarkerSetExtract = false;
	char *txt = strchr(MarkName, ':');

	if (txt) {
		*txt = '\0';  // terminate type name
		txt++;        // point at marker text
		txt = NewName(txt);
	}

	if (IniFile
	 && IniFile->Section("MarkerTypes")
	 && IniFile->Find(MarkName)
	 && IniFile->ListRight()) {
		char *item = NULL;
		while ((item = IniFile->ListItem()) != NULL)
			ProcMarkerType(cp, item, txt);
	}
	else
		ProcMarkerType(cp, MarkName, txt);

	if (MarkerSetSplit)
		ProcSplit();
	if (MarkerSetExtract)
		ProcStartExtract();
}


char *HTMwr::MarkerProps[MarkerPropsMax] = {
 "Delete", "Code", "HTMLComment", "Split",
 "ExtrStart",  "ExtrFinish", "ExtrEnd","ExtrEnable",
 "ExtrDisable", "ExtrReplace", "ExtrHead", "ExtrTop",
 "ExtrBottom", "Title", "FileName", "ALink",
 "LocalTOCTitle", "HelpMerge", "Ansi", "HTMConfig",
 "Config", "RTFConfig", "EclipseLink", "EclipseAnchor",
 "EclipseContext", "Window", "DITA", "DocBook",
 "ChunkFile", "Chunk", "TopicStartCode", "Text",
 "Search", "Print", "HVIndex", "ExtCodeStartLine",
 "ExtCodeEndLine", "ExtCodeFileLen",
 "ExtCodeStartChar", "ExtCodeEndChar", "ExtCodeFileEnc",
 "IndexCode", "HHTextPopup"
};

DCilist HTMwr::TextMarkerList;
DCnlist HTMwr::ExtrReplaceMark;
DCnlist HTMwr::ExtrHeadMark;
DCnlist HTMwr::ExtrTopMark;
DCnlist HTMwr::ExtrBottomMark;
DCnlist HTMwr::MarkerIDList;

char *HTMwr::MarkName = NULL;
bool HTMwr::MarkerIDNext = false;
bool HTMwr::MarkerSetSplit = false;
bool HTMwr::MarkerSetExtract = false;
HTMctl *HTMwr::MarkerCtl = NULL;
char *HTMwr::ObjectID = NULL;

DClist HTMwr::ALinkMarkerList;
DCnlist HTMwr::ALinkMarkerRefList;
DCnlist HTMwr::ALinkParaRefList;
bool HTMwr::ALinkPara = false;


void
HTMwr::ProcMarkerType(HTMctl *cp, char *kp, char *txt)
{
	short key;
	size_t len = 0;
	unc arg = 0;
	bool val = true;
	bool link = false;
	long sid = Extracting ? ExtrID : (SplitID ? SplitID : 1);
	long mtext = 0;
	char *period = NULL;

	if (!_strnicmp(kp, NoProp, len = strlen(NoProp))) {
		val = false;
		kp += len;
	}

	// identify key
	for (key = 0; key < MarkerPropsMax; key++) {
		len = strlen(MarkerProps[key]);
		if (!_strnicmp(kp, MarkerProps[key], len))
			break;
	}
	//if (key == MarkerPropsMax)
	//	return;

	if (isdigit(*(kp + len)))
		arg = (unc) atoi(kp + len);

	switch (key) {
		case 0:  // Delete, do nothing
			break;

		case 1:  // Code
			MarkerCtl = cp;
			MarkerIDNext = true;
			mtext = 1;
			break;

		case 2:  // HTMLComment
			mtext = 2;
			break;

		case 3:  // Split
			if (!SplitHere) {
				SplittingFile = true;
				SplitHere = true;
				MarkerSetSplit = true;
			}
			break;

		case 4:  // ExtrStart
			if (!SplitHere
			 && ExtractEnable) {
				ExtrStarting = true;
				ExtrStartTag = MarkName; // or FormName;
				ExtrStartForm = &Curr;
				MarkerSetExtract = true;
			}
			break;

		case 5:  // ExtrFinish
			if (Extracting)
				ExtrFinishing = true;
			break;

		case 6:  // ExtrEnd
			if (Extracting) {
				ExtrEnding = true;
				ProcEndExtract();
			}
			break;

		case 7:  // ExtrEnable
			ExtractEnable = true;
			break;

		case 8:  // ExtrDisable
			ExtractEnable = false;
			break;

		case 9:  // ExtrReplace
			ExtrReplaceMark.add(txt, ExtrID);
			break;

		case 10: // ExtrHead
			ExtrHeadMark.add(txt, ExtrID);
			break;

		case 11: // ExtrTop
			ExtrTopMark.add(txt, ExtrID);
			break;

		case 12: // ExtrBottom
			ExtrBottomMark.add(txt, ExtrID);
			break;

		case 13: // Title
			if (sid > 1)
				SplitTitleList.add(txt, sid);
			else if (!TitleSet) {
				TitleSet = true;
				Title = txt;
				SplitTitleList.replace(txt, 1);
			}
			break;

		case 14: // FileName
			if (sid > 1) {
				period = strrchr(txt, '.');
				if (period
				 && !stricmp(period + 1, XrefFileSuffix))  // has ext
					*period = '\0';
				BaseNameList.replace(txt, sid);
				size_t len = strlen(txt) + strlen(XrefFileSuffix) + 2;
				char *nmf = NewName(len);
				sprintf(nmf, "%s.%s", txt, XrefFileSuffix);
				SplitNameList.replace(nmf, sid);
			}
			break;

		case 15: // ALink
			ALinkMarkerList.add(ParaRefNum);
			ALinkMarkerRefList.add(SetXIntRef(), (long) cp);
			mtext = 3;
			break;

		case 16: // LocalTOCTitle
			if ((sid > 1) && LocalTOCLevel)
				LocalTOCTitleList.add(txt, sid);
			break;

		case 17: // HelpMerge
			link = false;
		hmerge:
			if (Contents) {  // at start; use current ContentsID, then inc it
				ProcMergeMarker(txt, link);
				ContentsID++;
			}
			else {  // at end; inc ContentsID, then use it
				ContentsID++;
				ProcMergeMarker(txt, link);
			}
			ContentsUsed = true;
			break;

		case 18: // Ansi
			SetAnsiMap(txt);
			mtext = 4;
			break;

		case 19: // HTMConfig
		case 20: // Config
			if (DCini::SetDirective(txt, (long) cp))
				mtext = 5;
			Contents = Curr.contents;
			break;

		case 21: // RTFConfig, ignore it
			break;

		case 22: // EclipseLink, merge call for TOC
			link = true;
			if (Eclipse)
				goto hmerge;
			break;

		case 23: // EclipseAnchor, merge dest for TOC
			link = false;
			if (Eclipse)
				goto hmerge;
			break;

		case 24: // EclipseContext, in topic to ref in infopop
			mtext = 6;
			break;

		case 25:  // Window, sec window to use for HH
			if (!ContentsWindowList.find(sid))
				ContentsWindowList.add(NewName(txt), sid);
			ContentsWin = ContentsWindowList.find(sid);
			break;

		case 26:  // DITA
			if (HXMwr::DITA) // returns 1, 7 through 15, or 0
				mtext = HXMwr::ProcDITAMarker(kp + 4, txt);
			break;

		case 27:  // DocBook
			if (HXMwr::DocBook) // returns 1, 7 through 15, or 0
				mtext = HXMwr::ProcDITAMarker(kp + 7, txt);
			break;

		case 28:  // ChunkFile
			if (UseChunking) {
				if (txt
				 && *txt
				 && (sid > 1)) {
					char *dot = strrchr(txt, '.');
					if (dot
					 && !stricmp(dot + 1, XrefFileSuffix)) {  // has ext
						SplitNameList.replace(txt, sid);
						char *nmf = NewName(txt, dot - txt);
						BaseNameList.replace(nmf, sid);
					}
					else { // no ext
						BaseNameList.replace(txt, sid);
						size_t len = strlen(txt) + strlen(XrefFileSuffix) + 2;
						char *nmf = NewName(len);
						sprintf(nmf, "%s.%s", txt, XrefFileSuffix);
						SplitNameList.replace(nmf, sid);
					}
				}
			}
			break;

		case 29:  //  Chunk
			if (!SplitHere
			 && UseChunking) {
				SplittingFile = true;
				SplitHere = true;
				MarkerSetSplit = true;
			}
			break;

		case 30: // TopicStartCode
			if (txt && *txt)
				TopicStartMacros.add(cp, sid);
			MarkerCtl = cp;
			MarkerIDNext = true;
			break;

		case 31:  // Text
			MarkerCtl = cp;
			MarkerIDNext = true;
			mtext = 16;
			break;

		case 32: // Search (Yes or No)
			SearchHere = (!stricmp(txt, "No")) ? false : true;
			break;

		case 33: // Print (Only, Yes, or No)
			if (!stricmp(txt, "Only"))
				PrintOnly = PrintHere = true;
			else if (!stricmp(txt, "No"))
				PrintHere = PrintOnly = false;
			else {
				PrintOnly = false;
				PrintHere = true;
			}
			break;

		case 34:  // HVIndex for Help Viewer 1
			if (UseHVIndex)
				ProcHVIndex(txt, sid);
			return;  // all processing done here, not later

		case 35:  // ExtCodeStartLine
			mtext = 17;
			break;

		case 36:  // ExtCodeEndLine
			mtext = 18;
			break;

		case 37:  // ExtCodeFileLen
			mtext = 19;
			break;

		case 38:  // ExtCodeStartChar
			mtext = 20;
			break;

		case 39:  // ExtCodeEndChar
			mtext = 21;
			break;

		case 40:  // ExtCodeFileEnc
			mtext = 22;
			break;

		case 41:  // IndexCode
			MarkerCtl = cp;
			MarkerIDNext = true;
			mtext = 23;
			break;

		case 42:  // HHTextPopup
			if ((HelpType == 'M')
			 || JHVersion2
			 || Eclipse
			 || OracleHelp)
				SetHyperLink(cp, NewName(txt), 2);
			break;


		default:  // not found in list
			AddMarkerCode(kp, &txt);
			AddAttrMarker(kp, txt);
			break;
	}

	if (mtext) {
		// for text-containing or referencing markers,
		TextMarkerList.append(mtext, (long) cp); // id is ptr value
		// put in the ctl as a flag for later processing in write phase
		if (DCLlevel < LevelMax)
			GroupStack[DCLlevel]->add(*cp);
	}

}

#undef min

char *
HTMwr::GetMarkerGroupText(DCgroup *gp)
{
	if (!gp
	 || !gp->first)
		return NULL;

	static char cbuf[1024];
	char *txt = cbuf;
	long sz = 0;
	DCctl *cp = NULL;

	for (DCitem *ci = gp->first; ci; ci = ci->next) {
		// get all text obj lit content into txt
		// ignore anything else
		cp = ci->ctl;
		if (!cp)
			continue;
		if ((cp->maj() == 2)
		 && (cp->min() == 10)
		 && (cp->dat() == 1)) {
			sz = cp->siz();
			if (cp->dt() == etext) {
				strncpy(txt, (const char*) cp->ex(), sz);
				txt += sz;
			}
			else if (cp->dt() == imnem) {
				if ((sz > 0)
				 && (sz <= 0x7F))
					*txt++ = (char) (sz & 0x7F);
				else if ((sz > 0x7F)
				 && (sz < 0x1FFFFF)) {
					// write in UTF-8
					txt = (char *) ::UTF8((unl) sz, (unc *) txt);
				}
			}
		}
	}
	*txt = '\0';
	return NewName(txt);
}


void
HTMwr::ProcMergeMarker(char *txt, bool linked)
{
	if (!isdigit(*txt) || (*(txt + 1) != ' '))
		return;

	uns lev = (uns) *txt - '0';
	while (*++txt == ' ')
		;
	if (!*txt)
		return;

	cntprop *cpt = new cntprop;
	cpt->level = lev;
	cpt->noref = false;

	cpt->ref = NewName((linked ? "=" : "*"), strlen(txt) + 1);
	strcat(cpt->ref, txt);

	if (OmniHelp && IniFile
	 && IniFile->Section("HelpMerge")
	 && IniFile->Find(txt))
		cpt->title = IniFile->StrRight();
	else
		cpt->title = NULL;

	ContentsList.add(cpt, ContentsID);
}


char *
HTMwr::SetXIntRef(void)
{
		char *ref = NewName(strlen(CurrFilePrefix) + NumStrMax + 3);
		sprintf(ref, "#X%s%ld", CurrFilePrefix, ParaUID);
		if (!IntRefList(ref + 1))
			SetIntRef(ref + 1);
		return ref;
}


void
HTMwr::WriteMarker(HTMwfile *wf, HTMctl *cp)
{
	long kind = TextMarkerList.find((long) cp);
	char *name = (char *) cp->ex();
	size_t len = strlen(name);
	char *txt = (cp->siz() > (len + 2)) ? name + len + 1 : "";
	char *code = NULL;
	setting *s = NULL;
	int i = 0;
	ecrefs *ep = NULL;
	dmapprop *dm = NULL;
	long prediv = wf->GetDiv();


	switch (kind) {
		case 1: // code
			WriteText(wf);  // set props
			ObjectID = MarkerIDList.find((long) cp);
			code = NewName(txt);
			AddMarkerCode(name, &code);
			DCmacr::WriteMacro(wf, code);
			DeleteName(code);
			ObjectID = NULL;
			break;

		case 16: // text
			WriteText(wf);  // set props
			ObjectID = MarkerIDList.find((long) cp);
			code = GetEncodedText(wf, txt); // re-encode and escape text content
			AddMarkerCode(name, &code);
			DCmacr::WriteMacro(wf, code);
			DeleteName(code);
			ObjectID = NULL;
			break;

		case 2:  // comment, on its own line
			wf->putEnd();
			wf->putComment(txt);
			wf->putEnd();
			break;

		case 3:  // alink
			ALinkRef = ALinkMarkerRefList.find((long) cp);
			WriteALinkObject(wf, txt);
			break;

		case 4: // Ansi setting
			SetAnsiMap(txt);
			break;

		case 5: // directive
			s = (setting *) DCini::SetList.find((long) cp);
			if (s != NULL)
				DCini::ProcMarkSect((char *)s->sect, (char *)s->key, (char *)s->val);
			break;

		case 6:  // Eclipse context
			if ((i = EclipseContextNames(txt)) == 0)
				EclipseContextNames.add(txt, (i = ++EclipseContextCount));
			ep = new ecrefs;
			ep->ctitle = ((Extracting || SplitID) ?
									 SplitTitleList.find(Extracting ? ExtrID : SplitID) :
									 CurrXFileTitle);
			ep->cfile = ((Extracting || SplitID) ?
									 SplitNameList.find(Extracting ? ExtrID : SplitID) : 
									 CurrXFileName);
			ep->cref = EclipseContextAnchors ? SetXIntRef() : NULL;
			EclipseContextRefs.add(ep, i);
			break;

		case 7:  // DITAStartElem
			HXMwr::SetDITAStart(wf);
			HXMwr::WriteDITAStartCTag(wf, txt);
			wf->endElem(true);
			HXMwr::SetDITADivOff(wf);
			break;

		case 8:  // DITAEndElem
			HXMwr::SetDITAEnd(wf);
			HXMwr::WriteDITAEndCTag(wf, txt);
			HXMwr::SetDITADivOff(wf);
			break;

		case 9: // DITAAnchor
			dm = new dmapprop;
			dm->anchor = true;
			dm->tid = txt;
			dm->level = HXMwr::DITAMapLevel;
			HXMwr::DITAMapVals.add(dm, HXMwr::DITATopicNum);
			break;

		case 10: //DITANavref
			dm = new dmapprop;
			dm->navref = true;
			dm->tid = txt;
			dm->level = HXMwr::DITAMapLevel;
			HXMwr::DITAMapVals.add(dm, HXMwr::DITATopicNum);
			break;

		case 11: //DITAMapref
			dm = new dmapprop;
			dm->mapref = true;
			dm->ref = txt;
			dm->level = HXMwr::DITAMapLevel;
			HXMwr::DITAMapVals.add(dm, HXMwr::DITATopicNum);
			break;

		case 12: //DITALinkScope
			HXMwr::DITALinkScope = txt;
			break;

		case 13: //DITALinkFormat
			HXMwr::DITALinkFormat = txt;
			break;

		case 14: //DITALinkType
			HXMwr::DITALinkType = txt;
			break;

		case 15: //DITALinkElemID
			HXMwr::DITALinkElemID = txt;
			break;

		case 17:  //ExtCodeStartLine
			ExtFileStartLine = strtoul(txt, NULL, 10);
			break;

		case 18:  //ExtCodeEndLine
			ExtFileEndLine = strtoul(txt, NULL, 10);
			break;

		case 19:  //ExtCodeFileLen
			ExtCodeFileLen = strtoul(txt, NULL, 10);
			break;

		case 20:  //ExtCodeStartChar
			ExtFileStartChar = strtoul(txt, NULL, 10);
			break;

		case 21:  //ExtCodeEndChar
			ExtFileEndChar = strtoul(txt, NULL, 10);
			break;

		case 22:  //ExtCodeFileEnc
			ExtCodeFileEnc = NewName(txt);
			break;

		case 23: // index code (for DITA only)
			ObjectID = MarkerIDList.find((long) cp);
			code = NewName(txt);
			AddMarkerCode(name, &code);
			if (IndexEntry)
				WriteIndexEntry(wf);
			if (!HXMwr::IndexDiv)
				HXMwr::IndexDiv = wf->NewDiv();
			else
				wf->SelectDiv(HXMwr::IndexDiv);
			DCmacr::WriteMacro(wf, code);
			HXMwr::IndexDivLen = wf->LenDiv();
			wf->SelectDiv(prediv);
			DeleteName(code);
			ObjectID = NULL;
			break;

		default:
			break;
	}
}


void
HTMwr::AddMarkerCode(char *name, char **ptxt)
{
	char *codeb = NULL;
	char *codea = NULL;
	char *coder = NULL;

	if (IniFile
	 && IniFile->Section("MarkerTypeCodeBefore")
	 && IniFile->Find(name))
		codeb = IniFile->StrRight();
	if (IniFile
	 && IniFile->Section("MarkerTypeCodeAfter")
	 && IniFile->Find(name))
		codea = IniFile->StrRight();
	if (IniFile
	 && IniFile->Section("MarkerTypeCodeReplace")
	 && IniFile->Find(name))
		coder = IniFile->StrRight();

	if (!codeb
	 && !codea
	 && !coder)
		return;

	char *code = NewName((codeb ? strlen(codeb) : 0) +
		                   (coder ? strlen(coder) : strlen(*ptxt)) +
		                   (codea ? strlen(codea) : 0) + 1);
	sprintf(code, "%s%s%s", (codeb ? codeb : ""),
		                      (coder ? coder : *ptxt),
		                      (codea ? codea : ""));
	DeleteName(codeb);
	DeleteName(codea);
	DeleteName(coder);
	DeleteName(*ptxt);
	*ptxt = code;
}


// can also be used to add attrib text from other sources
// during scan phase before end of obj needing attribute

void
HTMwr::AddAttrMarker(char *nm, char *txt)
{
	long mt = 0;
	if ((mt = MarkTypeList(nm)) == 0)
		MarkTypeList.add(nm, mt = ++MarkTypes);

	// concatenate same type items until consumed by setting first byte 0
	char *oldtxt = NULL;
	char *newtxt = NULL;

	if ((oldtxt = MarkList.find(mt)) == NULL)
		MarkList.add(txt, mt);
	else {
		if (*oldtxt == '\0')
			MarkList.replace(txt, mt);
		else {
			newtxt = NewName(oldtxt, strlen(oldtxt) + strlen(txt) + 1);
			strcat(newtxt, txt);
			MarkList.replace(newtxt, mt);
			DeleteName(txt);
			txt = newtxt;
		}
		//DeleteName(oldtxt);
	}
}


// at end of cell scan, go through MarkList and add Cell attrs (if any)
//  to CellAttrLp using MarkList.all(GetCellAttrs);

void
HTMwr::GetCellAttrs(char *txt, long mt)
{
	if (!txt || !mt
	 || (*txt == '\0'))
		return;

	char *nm = NULL;
	if (((nm = MarkTypeList.find(mt)) == NULL)
	 || _strnicmp(nm, "Cell", 4))
		return;
	if (*(nm + 4) == '-')
		nm++;

	if (!_strnicmp(nm + 4, "group", 5)) {  // special case
		if (!_strnicmp(txt, "col", 3))
			CellColGroupList.add(CellCpID);
		else if (!_strnicmp(txt, "row", 3))
			CellRowGroupList.add(CellCpID);
		*txt = '\0';
		return;
	}

	if (!_strnicmp(nm + 4, "span", 4)) {  // another special case
		CellSpanList.add(CellCpID);
		*txt = '\0';
		return;
	}

	if (!_strnicmp(nm + 4, "scope", 5)) {  // prevent multiple scopes
		CellScopeList.add(NewName(txt), CellCpID);
		*txt = '\0';
		return;
	}

	if (!_strnicmp(nm + 4, "id", 2)) {  // prevent multiple IDs
		CellIDList.add(NewName(txt), CellCpID);
		*txt = '\0';
		return;
	}

	nm = NewName(nm);
	size_t len = 0;
	char *astr = NewName(strlwr(nm + 4), (len = strlen(nm + 4)) + strlen(txt) + 4);
	astr[len++] = '=';
	astr[len++] = '"';
	astr[len] = '\0';
	strcpy(&astr[len], txt);
	strcat(astr, "\"");
	if (!CellAttrLp) {
		CellAttrLp = new DCnlist;
		CellAttrLists.add(CellAttrLp, CellCpID);
	}
	CellAttrLp->add(astr, mt);
	*txt = '\0';  // mark finished
	DeleteName(nm);
}


// at end of Row scan, go through MarkList and add Row attrs (if any)
//  to RowAttrLp using MarkList.all(GetRowAttrs);

void
HTMwr::GetRowAttrs(char *txt, long mt)
{
	if (!txt || !mt
	 || (*txt == '\0'))
		return;

	char *nm = NULL;
	if (((nm = MarkTypeList.find(mt)) == NULL)
	 || _strnicmp(nm, "Row", 3))
		return;

	if (*(nm + 3) == '-')
		nm++;
	nm = NewName(nm);
	size_t len = 0;
	char *astr = NewName(strlwr(nm + 3), (len = strlen(nm + 3)) + strlen(txt) + 4);
	astr[len++] = '=';
	astr[len++] = '"';
	astr[len] = '\0';
	strcpy(&astr[len], txt);
	strcat(astr, "\"");
	if (!RowAttrLp) {
		RowAttrLp = new DCnlist;
		RowAttrLists.add(RowAttrLp, RowCpID);
	}
	RowAttrLp->add(astr, mt);
	*txt = '\0';  // mark finished
	DeleteName(nm);
}


// at end of Tbl scan, go through MarkList and add Tbl attrs (if any)
//  to TblAttrLp using MarkList.all(GetTblAttrs);

void
HTMwr::GetTblAttrs(char *txt, long mt)
{
	if (!txt || !mt
	 || (*txt == '\0'))
		return;

	char *nm = NULL;
	if (((nm = MarkTypeList.find(mt)) == NULL)
	 || _strnicmp(nm, "Table", 5))
		return;

	if (*(nm + 5) == '-')
		nm++;
	nm = NewName(nm);
	size_t len = 0;
	char *astr = NewName(strlwr(nm + 5), (len = strlen(nm + 5)) + strlen(txt) + 4);
	astr[len++] = '=';
	astr[len++] = '"';
	astr[len] = '\0';
	strcpy(&astr[len], txt);
	strcat(astr, "\"");
	if (!TblAttrLp) {
		TblAttrLp = new DCnlist;
		TblAttrLists.add(TblAttrLp, TblCpID);
	}
	TblAttrLp->add(astr, mt);
	*txt = '\0';  // mark finished
	DeleteName(nm);
}


// at end of Para scan, go through MarkList and add Para attrs (if any)
//  to ParaAttrLp using MarkList.all(GetParaAttrs);

void
HTMwr::GetParaAttrs(char *txt, long mt)
{
	if (!txt || !mt
	 || (*txt == '\0'))
		return;

	char *nm = NULL;
	if (((nm = MarkTypeList.find(mt)) == NULL)
	 || _strnicmp(nm, "Para", 4))
		return;

	if (*(nm + 4) == '-')
		nm++;
	nm = NewName(nm);
	size_t len = 0;
	char *astr = NewName(strlwr(nm + 4), (len = strlen(nm + 4)) + strlen(txt) + 4);
	astr[len++] = '=';
	astr[len++] = '"';
	astr[len] = '\0';
	strcpy(&astr[len], txt);
	strcat(astr, "\"");
	ParaAttrLp = (DCnlist *) ParaAttrLists.find(ParaRefNum);
	if (!ParaAttrLp) {
		ParaAttrLp = new DCnlist;
		ParaAttrLists.add(ParaAttrLp, ParaRefNum);
	}
	ParaAttrLp->add(astr, mt);
	*txt = '\0';  // mark finished
	DeleteName(nm);
}


// at end of Char scan, go through MarkList and add Char attrs (if any)
//  to CharAttrLp using MarkList.all(GetCharAttrs);

void
HTMwr::GetCharAttrs(char *txt, long mt)
{
	if (!txt || !mt
	 || (*txt == '\0'))
		return;

	char *nm = NULL;
	if (((nm = MarkTypeList.find(mt)) == NULL)
	 || _strnicmp(nm, "Char", 4))
		return;

	if (*(nm + 4) == '-')
		nm++;
	nm = NewName(nm);
	size_t len = 0;
	char *astr = NewName(strlwr(nm + 4), (len = strlen(nm + 4)) + strlen(txt) + 4);
	astr[len++] = '=';
	astr[len++] = '"';
	astr[len] = '\0';
	strcpy(&astr[len], txt);
	strcat(astr, "\"");
	if (!CharAttrLp) {
		CharAttrLp = new DCnlist;
		CharAttrLists.add(CharAttrLp, CharRefNum);
	}
	CharAttrLp->add(astr, mt);
	*txt = '\0';  // mark finished
	//CharAttrLp = NULL;
	DeleteName(nm);
}


// next graphic picks up GraphAlt markers

void
HTMwr::GetGraphAttrs(char *txt, long mt)
{
	if (!txt || !mt
	 || (*txt == '\0'))
		return;

	char *nm = NULL;
	if (((nm = MarkTypeList.find(mt)) == NULL)
	 || _strnicmp(nm, "Graph", 5))
		return;
	if (*(nm + 5) == '-')
		nm++;

	if (!_strnicmp(nm + 5, "dpi", 3)) {  // special case
		GrDpiList.addnew(txt, CurrGraphFrameID);
		*txt = '\0';
		return;
	}

	if (!_strnicmp(nm + 5, "alt", 3))  // another special case
		GrAltList.add(CurrGraphFrameID);
	if (!_strnicmp(nm + 5, "longdesc", 8))  // another special case
		GrLDescList.add(CurrGraphFrameID);
	if (!_strnicmp(nm + 5, "class", 5))  // another special case
		GrClassList.add(CurrGraphFrameID);

	nm = NewName(nm);
	size_t len = 0;
	char *astr = NewName(strlwr(nm + 5), (len = strlen(nm + 5)) + strlen(txt) + 4);
	astr[len++] = '=';
	astr[len++] = '"';
	astr[len] = '\0';
	strcpy(&astr[len], txt);
	strcat(astr, "\"");
	if (!GraphAttrLp)
		GraphAttrLp = new DCnlist;
	GraphAttrLp->add(astr, mt);
	*txt = '\0';  // mark finished
	DeleteName(nm);
}


// next link picks up LinkTitle markers

void
HTMwr::GetLinkAttrs(char *txt, long mt)
{
	if (!txt || !mt
	 || (*txt == '\0'))
		return;

	char *nm = NULL;
	if ((nm = MarkTypeList.find(mt)) == NULL)
		return;

	nm = NewName(nm);
	size_t len = 0;
	size_t px = 6;
	char *astr = NULL;

	if (!_strnicmp(nm, "Link", 4)) {
		if (*(nm + 4) == '-')
			nm++;
		astr = NewName(strlwr(nm + 4), (len = strlen(nm + 4)) + strlen(txt) + 4);
		astr[len++] = '=';
		astr[len++] = '"';
		astr[len] = '\0';
		strcpy(&astr[len], txt);
		strcat(astr, "\"");
		if (!LinkAttrLp)
			LinkAttrLp = new DCnlist;
		LinkAttrLp->add(astr, mt);
		*txt = '\0';  // mark finished
	}
	else if (!_strnicmp(nm, JH2PopMarkerPrefix, (px = strlen(JH2PopMarkerPrefix)))) {
		astr = NewName(strlwr(nm + px), (len = strlen(nm + px)) + strlen(txt) + 4);
		astr[len++] = '=';
		astr[len++] = '"';
		astr[len] = '\0';
		strcpy(&astr[len], txt);
		strcat(astr, "\"");
		if (!JH2PopAttrLp)
			JH2PopAttrLp = new DCnlist;
		JH2PopAttrLp->add(astr, mt);
		*txt = '\0';  // mark finished
	}
	else if (!_strnicmp(nm, JH2SecMarkerPrefix, (px = strlen(JH2SecMarkerPrefix)))) {
		astr = NewName(strlwr(nm + px), (len = strlen(nm + px)) + strlen(txt) + 4);
		astr[len++] = '=';
		astr[len++] = '"';
		astr[len] = '\0';
		strcpy(&astr[len], txt);
		strcat(astr, "\"");
		if (!JH2SecAttrLp)
			JH2SecAttrLp = new DCnlist;
		JH2SecAttrLp->add(astr, mt);
		*txt = '\0';  // mark finished
	}
	DeleteName(nm);
}


// at end of part scan, go through MarkList and add Meta attrs (if any)
//  to MetaList using MarkList.all(GetMetaAttrs);

void
HTMwr::GetMetaAttrs(char *txt, long mt)
{
	if (!txt || !mt
	 || (*txt == '\0'))
		return;

	char *nm = NULL;
	if (((nm = MarkTypeList.find(mt)) == NULL)
	 || _strnicmp(nm, "Meta", 4))
		return;
	if (*(nm + 4) == '-')
		nm++;

	long sid = Extracting ? ExtrID : SplitID;
	if (!sid)
		sid = 1;

	MetaList.addnew(txt, ++MetaCount);
	MetaNameList.addnew(nm + 4, MetaCount);
	MetaPartList.add((void *) sid, MetaCount);

	*txt = '\0';  // mark finished
}


void
HTMwr::GetRefAttrs(char *txt, long mt)
{
	if (!txt || !mt
	 || (*txt == '\0'))
		return;

	char *nm = NULL;
	if (((nm = MarkTypeList.find(mt)) == NULL)
	 || _strnicmp(nm, "Ref", 3))
		return;
	if (*(nm + 3) == '-')
		nm++;

	if (!HXMwr::DITARefAttrs) {
		HXMwr::DITARefAttrs = new DCnlist();
		HXMwr::DITARefAttrNames = new DCnlist();
		HXMwr::DITARefCount = 0;
	}
	HXMwr::DITARefAttrs->addnew(txt, ++HXMwr::DITARefCount);
	HXMwr::DITARefAttrNames->addnew(nm + 3, HXMwr::DITARefCount);

	*txt = '\0';  // mark finished
}



void
HTMwr::WriteAttrListItem(DCwfile *w, char *n, long i)
{
	if (!w || !n || !i
	 || (*n == '\0'))
		return;

	HTMwfile *wf = (HTMwfile *) w;

	//wf->putChar(' ');
	//DCmacr::WriteMacro(wf, n);
	wf->putAttrStr(DCmacr::GetMacro(n));

	if (!HXMwr::HXML)
		wf->putEnd();
}


void
HTMwr::WritePropListItem(DCwfile *w, long i)
{
	if (!w || !i)
		return;

	HTMwfile *wf = (HTMwfile *) w;

	//wf->putChar(' ');
	//DCmacr::WriteMacro(wf, (char *) i);
	wf->putAttrStr(DCmacr::GetMacro((char *) i));
	wf->putEnd();
}



// hypertext markers


char HTMwr::HyperSpaceChar = 0;
bool HTMwr::HyperJump = false;
bool HTMwr::HyperJumpStarting = false;
bool HTMwr::ParaFullLink = false;
uns HTMwr::HyperVal = 0;
long HTMwr::HyperJumpID = 0;
char *HTMwr::HyperJumpStr = NULL;
DCnlist HTMwr::HyperJumpStrList;
DCilist HTMwr::HyperJumpTypeList;

bool HTMwr::HyperJumpFile = false;
long HTMwr::HyperJumpFileID = 0;
DCnlist HTMwr::HyperJumpFileList;
char *HTMwr::HyperJumpFileName = NULL;

bool HTMwr::HyperLinkIDNext = false;
DCnlist HTMwr::HyperLinkRefList;
char *HTMwr::HyperLinkID = NULL;


bool HTMwr::HyperPara = false;
long HTMwr::HyperParaID = 0;
DCilist HTMwr::HyperParaList;

bool HTMwr::HyperForm = false;
bool HTMwr::HyperFormEnding = false;
long HTMwr::HyperFormNumber = 0;
long HTMwr::HyperFormStartID = 0;
long HTMwr::HyperFormEndID = 0;
DCilist HTMwr::HyperFormStartList;
DClist HTMwr::HyperFormEndList;


void
HTMwr::SetHyperStart(long id)
{
	if (InHyper
	 && ((Curr.hyper & 2) == 2))
		return;

	HyperJumpID = id;
	if (Curr.secwin
	 && ((HelpType == 'M')
	  || JHVersion2
	  || Eclipse
	  || OracleHelp))
		SecWin = true;
	Curr.hyper |= 1;  // hypertext marker start needed
	HyperVal = (unc) HyperJumpTypeList.find(id);  // jump or popup
	HyperJumpStr = HyperJumpStrList.find(id);
}


void
HTMwr::SetHyperEnd(void)
{
	if (!InHyper)
		return;

	Curr.hyper |= 2;  // need to write ending anchor now
	HyperVal = 1;
}


void
HTMwr::CheckHyper(HTMctl *cp)
{
	long id;

	if (HyperPara) {
		if (Curr.hyper == 0) {
			SetHyperStart(HyperJumpID);
			HyperPara = false;
		}
	}
	else {
		CheckHyperEnd(cp);
		//if (HyperFormEndList.find((long) cp))
		//	SetHyperEnd();
		if ((id = HyperFormStartList.find((long) cp)) != 0)
			SetHyperStart(id);
	}
}


void
HTMwr::CheckHyperEnd(HTMctl *cp)
{
	if (HyperFormEndList.find((long) cp)
	 || HyperColorEndList.find((long) cp))
		SetHyperEnd();
}


// hyper color interpretation


bool HTMwr::UseHyperColor = false;
bool HTMwr::HyperColor = false;
bool HTMwr::HyperColorEnding = false;
bool HTMwr::ParaHyperColor = false;
long HTMwr::HyperColorStartID;
long HTMwr::HyperColorEndID;
DCilist HTMwr::HyperColorStartList;
DClist HTMwr::HyperColorEndList;


void
HTMwr::ProcHyperColor(HTMctl *cp)
{
	// processing during scan phase
	if ((UseHyperColor == false)
	 || HyperForm)
		return;

	if (cp->siz() > 1)  // color
		ProcHyperColorStart(cp);
	else if ((cp->siz() == 1)  // black
	 && HyperColor)     // was in color
		ProcHyperColorEnd(cp);
}


void
HTMwr::ProcHyperColorStart(HTMctl *cp)
{
	HyperColor = true;
	HyperColorEnding = false;
	HyperColorStartID = (long) cp;
	if (HyperJumpStarting) {  // link was just before start
		HyperColorStartList.append(HyperJumpID, HyperColorStartID);
		HyperJumpStarting = false;
	}
	else if (HyperJump) {  // link started earlier, end it here
		HyperParaList.append(HyperJumpID, HyperParaID);
		HyperColorEndList.add(HyperColorStartID);
		HyperJump = false;
	}
}


void
HTMwr::ProcHyperColorEnd(HTMctl *cp)
{
	HyperColorEndID = (long) cp;
	if (HyperJump && HyperColor) {
		HyperColorEndList.add(HyperColorEndID);
		HyperJump = false;
		HyperJumpStarting = false;
	}
	// leave IDs in case HyperJump is right after
	HyperColor = false;
	HyperColorEnding = true;  // turn off after real text
}


void
HTMwr::ProcTextColor(HTMctl *cp)
{
	// processing during write phase
	long id;

	GetColor(cp, &Curr.colr);
	if (HyperColorEndList.find((long) cp))
		SetHyperEnd();

	if ((id = HyperColorStartList.find((long) cp)) != NULL)
		SetHyperStart(id);
}



// variable formats

uns HTMwr::CurrVar;
DCvlist HTMwr::VarList;
DCnlist HTMwr::VarNameList;
DCnlist HTMwr::UserVarNameList;
uns HTMwr::TbContVar;
uns HTMwr::TbSheetVar;
bool HTMwr::TbContUse = false;
bool HTMwr::TbSheetUse = false;
char *HTMwr::TbContVarName = "Table Continuation"; 
char *HTMwr::TbSheetVarName = "Table Sheet"; 

DCnlist HTMwr::SystemVarNames;
npair HTMwr::SystemVarNamesSet[] = {
 { 1, "Page Count"},
 { 2, "Current Date (Long)"},
 { 3, "Current Date (Short)"},
 { 4, "Modification Date (Long)"},
 { 5, "Modification Date (Short)"},
 { 6, "Creation Date (Long)"},
 { 7, "Creation Date (Short)"},
 { 8, "Filename (Long)"},
 { 9, "Filename (Short)"},
 { 10, "Current Page #"},
 { 11, "Table Continuation"},
 { 12, "Table Sheet"},
 { 13, "Running H/F 1"},
 { 14, "Running H/F 2"},
 { 15, "Running H/F 3"},
 { 16, "Running H/F 4"},
 { 17, "Running H/F 5"},
 { 18, "Running H/F 6"},
 { 19, "Running H/F 7"},
 { 20, "Running H/F 8"},
 { 0, NULL }
 };


unc HTMwr::DITAVarType = 'T';
char *HTMwr::DITAVarFile = "ditavars.dita";
char *HTMwr::DITAVarID = "varset";
char *HTMwr::DITAVarElem = "ph";
bool HTMwr::WriteVarFile = false;
DCilist HTMwr::VarUsedList;

#ifdef UNUSED
bool HTMwr::InDateToken = false;
long HTMwr::DateDiv = 0;
long HTMwr::PreDateDiv = 0;
uns HTMwr::ParaVarType = 0;
#endif

void 
HTMwr::SetVarEntities(HTMwfile *wf, long sid)
{
	if (DITAVarType != 'E')
		return;

	// for all if sid == 0, else for split part
	wf->putChar('[');
	if (sid)
		VarUsedList.write(sid, wf, ProcVarEnt);
	else {
		ProcVarEnt(wf, 0);
		VarUsedList.writeall(wf, ProcVarEnts);
	}

	// end the header
	wf->putChar(']');
	wf->endElem(true);
	wf->putEnd();
}

void
HTMwr::ProcVarEnts(DCwfile *dwf, long val, long ix)
{
	ProcVarEnt(dwf, val);
}

void
HTMwr::ProcVarEnt(DCwfile *dwf, long val)
{
	static DClist written;

	if (val == 0) {
		written.empty();
		return;
	}
	if (written.find(val))
		return;
	written.add(val);

	char *nm = UserVarNameList.find(val);
	if (!nm)
		return;

	char *txt = GetVarText((uns) val);
	if (!txt)
		return;

	HTMwfile *wf = (HTMwfile *) dwf;
	// \n<!ENTITY vname "vdef"> 
	// with any < inside vdef escaped as &#60;
	wf->putEnd();
	wf->putStr(" <!ENTITY ");
	wf->putStr(nm);
	wf->putStr(" \"");
	wf->putStr(txt);
	wf->putStr("\">");
} 

void
HTMwr::WriteVarElementsFile(HTMwfile *wf)
{
	// write DITA file containing DITAVarElems to conref

	long tnum = HXMwr::DTopicTypes->getitem("topic");

	tagset *ttags = (tagset *) HXMwr::DTagSets->find(tnum);
	if (!ttags)
		ttags = HXMwr::DTopicTags;

	char *troot = ttags->names->find(1);
	if (!troot)
		troot = "topic";

	HTMwfile *vf = new HTMwfile(DITAVarFile);
	vf->putXML(XMLEncoding, XMLVer);
	vf->putDT(troot, ttags->dtype, ttags->dtd, false);  
	vf->startBlock(troot);
	vf->strAttr(HXMwr::IDattr, DITAVarID);
	vf->strAttr(XHLangAttr, XhtmlLang);
	vf->endElem(true);
	vf->putEnd();
	vf->startSBlock("title");
	vf->putStr("Variable Definitions");
	vf->endBlock("title");
	vf->putEnd();
	vf->startSBlock("body");
	vf->startSBlock("dl");
	vf->putEnd();
	vf->startSBlock("dlhead");
	vf->writeBlock("dthd", "Name");
	vf->writeBlock("ddhd", "Definition");
	vf->endBlock("dlhead");
	UserVarNameList.writeall(vf, ProcVarElems);
	vf->putEnd();
	vf->putEnd();
	vf->endBlock("dl");
	vf->endBlock("body");
	vf->endBlock(troot);
	vf->CloseFile();
}

void
HTMwr::ProcVarElems(DCwfile *dwf, char *nm, long val)
{
	if (!val || !nm)
		return;

	char *txt = GetVarText((uns) val);
	if (!txt)
		return;

	HTMwfile *wf = (HTMwfile *) dwf;

	wf->putEnd();
	wf->putEnd();
	wf->startSBlock("dlentry");
	wf->writeBlock("dt", nm);
	wf->startSBlock("dd");
	wf->startBlock(DITAVarElem);
	wf->strAttr(HXMwr::IDattr, nm);
	wf->endElem(true);
	wf->putStr(txt);
	wf->endBlock(DITAVarElem);
	wf->endBlock("dd");
	wf->endBlock("dlentry");
}


void
HTMwr::SetVarForm(HTMctl* cp)
{
	HTMgroup* gp = new HTMgroup;
	VarList.add(gp, CurrVar);
  if (DCLlevel < LevelMax)
    GroupStack[DCLlevel] = gp;
}


void
HTMwr::CheckVarName(HTMctl* cp)
{
	char *nm = (char *) cp->ex();
	if (!strcmp(nm, TbContVarName))
		TbContVar = CurrVar;
	if (!strcmp(nm, TbSheetVarName))
		TbSheetVar = CurrVar;

	if (SystemVarNames(nm) == 0) {
		uns len = (uns) strlen(nm) + 2;
		char *nnm = NewName(len);
		MakeID(nm, nnm, len, '_', true, true, false);
		UserVarNameList.addnew(nnm, CurrVar);
	}

	VarNameList.addnew(nm, CurrVar);
}


bool
HTMwr::CheckVarForm(void)
{
	if (ParaForm.macpos & MacReplace)  // replacing para
		return false;

	if (Curr.macpos & MacReplace)  // replacing para
		return false;

	if ((CharBlockState == CbStarted)
	 && ((CharBlockMacro & MacReplace)
	  || (Curr.cmaccr)))  // still in replaced char text
		return false;

	if (HideWhite
	 && (Curr.colr == 3)
	 && !ParaEnding) {
		WhiteText = true;
		return false;
	}

	return true;
}


void
HTMwr::PutVarForm(HTMwfile* wf, HTMctl* cp)
{
	if (!CheckVarForm())
		return;

	CurrVar = (uns) cp->siz();

	if ((CurrVar == TbContVar)
	 && (TbContUse == false))
		return;
	if ((CurrVar == TbSheetVar)
	 && (TbSheetUse == false))
		return;

	char *vname = NULL;

	if (HXMwr::DITA
	 && (DITAVarType != 'T')
	 && ((vname = UserVarNameList.find(CurrVar)) != NULL)) {
		char *vref = NULL;
		switch (DITAVarType) {
			case 'E':  // entities
				WriteText(wf);  // set props
				wf->putEntity(vname);
				return;
			case 'C':  // conrefs
				WriteText(wf);  // set props
				vref = NewName(strlen(DITAVarFile) + strlen(DITAVarID) + strlen(vname) + 3);
				sprintf(vref, "%s#%s/%s", DITAVarFile, DITAVarID, vname);
				wf->startBlock(DITAVarElem);
				wf->strAttr("conref", vref);
				wf->endElem(true);
				wf->endBlock(DITAVarElem);
				DeleteName(vref);
				return;
			case 'T':  // text
			default:
				break;
		}
	}

	char *txt = NULL;
	if (((vname = VarNameList.find(CurrVar)) != NULL)
	 && DCmacr::ReplacingFrameVars()
	 && ((txt = DCmacr::GetUserMacVar(vname, false)) != NULL)) {
		WriteText(wf);  // set props
		wf->putTextStr(txt);
		return;
	}

	HTMgroup* gp = (HTMgroup*) VarList.find(CurrVar);
	if (gp) {
		SaveState();
		gp->write(*wf);
		WriteEndCharProps(wf);
		RestoreState();
	}
}


// variable arrays

DCilist HTMwr::VarArraySize;  // count of arr elems indexed by var
DCvlist HTMwr::VarArrayList;  // DCvlists of arr elem forms by var 
long HTMwr::CurrVarArrayCount = 0;
DCvlist *HTMwr::CurrVarArrayList; // DCgroups of arr elem items by item
long HTMwr::CurrVarArrayItem = 0;
HTMgroup *HTMwr::CurrVarArrayForm = NULL;


void
HTMwr::SetVarArray(HTMctl* cp)
{
	CurrVarArrayCount = cp->siz();
	VarArraySize.append(CurrVarArrayCount, CurrVar);
	CurrVarArrayList = new DCvlist;
	VarArrayList.add(CurrVarArrayList, CurrVar);
}


void
HTMwr::SetVarArrItemForm(HTMctl* cp)
{
	HTMgroup* gp = new HTMgroup;
	CurrVarArrayList->add(gp, CurrVarArrayItem);
  if (DCLlevel < LevelMax)
    GroupStack[DCLlevel] = gp;
}


void
HTMwr::SetVarArrItem(unl var)
{
	CurrVarArrayItem = var & 0xFFFF;
	CurrVar = (uns) (var >> 16);
	CurrVarArrayCount = VarArraySize.find(CurrVar);
	CurrVarArrayList = (DCvlist *) VarArrayList.find(CurrVar);
	CurrVarArrayForm = (HTMgroup *) CurrVarArrayList->find(CurrVarArrayItem);
}


void
HTMwr::ProcTitleArrVar(HTMctl* cp)
{
	SetVarArrItem(cp->siz());

	if (CurrVarArrayForm)
		CurrVarArrayForm->execu(ProcTitleVarText);
}


void
HTMwr::PutVarArrItemForm(HTMwfile* wf, HTMctl* cp)
{
	if (!CheckVarForm())
		return;

	SetVarArrItem(cp->siz());

	if (CurrVarArrayForm) {
		SaveState();
		CurrVarArrayForm->write(*wf);
		RestoreState();
	}
}


// document numbers

bool HTMwr::InChapNum = false;
bool HTMwr::InVolNum = false;
bool HTMwr::InSectNum = false;
bool HTMwr::InSSectNum = false;
char *HTMwr::ChapNum = NULL;
char *HTMwr::VolNum = NULL;
char *HTMwr::SectionNum = NULL;
char *HTMwr::SSectNum = NULL;
uns HTMwr::ChapNumVType = 0;
uns HTMwr::VolNumVType = 0;
uns HTMwr::SectNumVType = 0;
uns HTMwr::SSectNumVType = 0;
char *HTMwr::PageNum = NULL;


void
HTMwr::PutSystemVar(HTMwfile* wf, HTMctl* cp)
{
	switch (cp->siz()) {
		case 1: // chapnum
			WriteText(wf);
			wf->putTextStr(ChapNum);
			break;
		case 2:  // volnum
			WriteText(wf);
			wf->putTextStr(VolNum);
			break;
		case 3:  // sectionnum
			WriteText(wf);
			wf->putTextStr(SectionNum);
			break;
		case 4:  // subsectionnum
			WriteText(wf);
			wf->putTextStr(SSectNum);
			break;
		default:
			break;
	}			
}




// xref file and file ID mapping

char *HTMwr::XrefFile = NULL;
//char *HTMwr::XrefRawFile = NULL;
bool HTMwr::XrefMapped = false;
bool HTMwr::XrefListed = false;
bool HTMwr::XrefFound = false;
bool HTMwr::MakeFileHrefsLower = false;
bool HTMwr::ATagLineBreak = true;

DCnlist HTMwr::XRFNames;
DCnlist HTMwr::XRFNewNames;
long HTMwr::XRFCount = 0;

bool HTMwr::RemoveFilePaths = true;
bool HTMwr::UseXrefFilePrefix = true;
bool HTMwr::DefXrefToCurr = false;
bool HTMwr::UseListedXrefFilesOnly = false;
bool HTMwr::XrefInCurrDBFile = false;
char *HTMwr::CurrFilePrefix = NULL;
long HTMwr::CurrPxLen = 0;
char *HTMwr::XrefFilePrefix = NULL;
char *HTMwr::XrefFileSuffix = "htm";

DCnlist HTMwr::FIDNames;
DCnlist HTMwr::FIDPrefixes;
long HTMwr::FIDCount = 0;

bool HTMwr::XrefLinkIDNext = false;
char *HTMwr::XrefLinkID = NULL;


char *
HTMwr::StripSuf(char *name)
{
	char *suf;

	if (((suf = strrchr(name, '.')) != NULL)
	 && (suf > name))
		*suf = '\0';
	return name;
}


void
HTMwr::SetFIDLists(void)
{
	if (!UseLocalFileID)
		return;

	if (IniFile
	 && IniFile->Section("FileIDs")) {
		while (IniFile->NextItem()) {
			FIDNames.add(StripSuf(IniFile->StrLeft()), ++FIDCount);
			FIDPrefixes.add(IniFile->StrRight(), FIDCount);
		}
	}	
}


char *
HTMwr::GetFIDPrefix(char *fname)
{
	long i;

	if (!UseLocalFileID)
		return GetFileID(fname);

	if ((i = FIDNames(fname)) != 0)
		return FIDPrefixes.find(i);

	// no prefix, so make one up
	char *nn = NewName(fname);
	char *px = nn + (strlen(nn) - 3);
	while (FIDPrefixes(px)  // already in use
	 && (px > nn))       // more of name available
		px--;              // use more of name
	FIDNames.add(nn, ++FIDCount);
	FIDPrefixes.add(px, FIDCount);
	return px;
}


char *HTMwr::CurrXFileName = NULL;
char *HTMwr::CurrXFilePath = NULL;
char *HTMwr::CurrXFileTitle = NULL;
char *HTMwr::CurrXFileBase = NULL;

extern "C" char *_getcwd(char *buffer, int maxlen);

void
HTMwr::SetXRFLists(void)
{
	char *str;
	char *nstr;

	CurrXFileName = NewName(strlen(CurrFileName) + strlen(XrefFileSuffix) + 2);
	strcpy(CurrXFileName, CurrFileName);
	strcat(CurrXFileName, ".");
	strcat(CurrXFileName, XrefFileSuffix);
	CurrXFileBase = NewName(CurrFileName);
	BaseNameList.add(CurrXFileBase, 1);

	char *pbuf = NewName(_MAX_PATH);
	_getcwd(pbuf, _MAX_PATH);
	char *pb = pbuf;
	while (*pb) {
		if (*pb == '\\')
			*pb = '/';
		pb++;
	}
	if (*(pb - 1) != '/')
		*pb++ = '/';
	*pb = '\0';
	CurrXFilePath = NewName(pbuf);
	DeleteName(pbuf);

	if (IniFile
	 && IniFile->Section("XrefFiles")) {
		while (IniFile->NextItem()) {
			str = IniFile->StrRight();
			if (*str == '\0') {  // empty new name
				delete str;
				continue;
			}
			StripSuf(str);
			nstr = NewName(strlen(str) + strlen(XrefFileSuffix) + 2);
			strcpy(nstr, str);
			delete str;
			strcat(nstr, ".");
			strcat(nstr, XrefFileSuffix);
			XRFNames.add(StripSuf(IniFile->StrLeft()), ++XRFCount);
			XRFNewNames.add(nstr, XRFCount);
		}
	}	
}


char *
HTMwr::GetXrefName(char *fname)
{
	long i;
	char *dname;

	if ((i = XRFNames(fname)) != 0) {
		XrefFound = true;
		return NewName(XRFNewNames.find(i));
	}

	// no xref name listed
	XrefFound = false;

	if ((*fname == '.')
	 && (*(fname + 1) == '/'))
		fname += 2;

	if (!DefXrefToCurr) {
		i = strlen(fname);
		if ((i > 4)
		 && !strchr((fname + i - 5), '.')) {
			dname = NewName(fname, i + strlen(XrefFileSuffix) + 2);
			strcat(dname, ".");
			strcat(dname, XrefFileSuffix);
			return dname;
		}
		else
			return fname;
	}
	return CurrXFileName;
}


void
HTMwr::SetXrefFileName(char *name, size_t len)
{
	// for xrefs, used at write time only
	// for hyperfiles, used at scan time only

  //DeleteName(XrefRawFile);
	//XrefRawFile = NULL;

  char *xfname = NewName(name, len);
	if (len == 0)
		len = strlen(xfname);
  char *cpt = strrchr(xfname, '/');	// remove any path
	char *bpt = strrchr(xfname, '>'); // and any Frame path codes
	if (bpt > cpt)
		cpt = bpt++;
	if (!cpt)
		cpt = xfname;
	else
		cpt++;

  //if (cpt)	 // shorten but no need to reallocate str
  //  memmove(xfname, cpt + 1, len - (cpt - xfname));
	StripSuf(xfname);

	XrefFile = GetXrefName(cpt);
	if (XrefFound) {
	  //XrefRawFile = xfname;
		XrefMapped = true;
		if (UseXrefFilePrefix) {
			cpt = NewName(XrefFile);
			XrefFilePrefix = GetFIDPrefix(StripSuf(cpt));
			DeleteName(cpt);
		}
	}
	else {  // leave path on, add correct suffix
		XrefMapped = false;
		if (cpt
		 && (cpt == xfname + 2)
		 && (xfname[0] == '.')
		 && (xfname[1] == '/')) // strip path if curr dir
			xfname = cpt;
		if (bpt > xfname)  // must get rid of FM path codes
			xfname = bpt;
		XrefFile = NewName(strlen(xfname) + strlen(XrefFileSuffix) + 2);
		if (MakeFileHrefsLower) {
			uns i;
			for (i = 0; i < strlen(xfname); i++)
				XrefFile[i] = tolower(xfname[i]);
			XrefFile[i] = '.';
		}
		else {
			strcpy(XrefFile, xfname);
			strcat(XrefFile, ".");
		}
		strcat(XrefFile, XrefFileSuffix);

		if (UseXrefFilePrefix)
			XrefFilePrefix = GetFIDPrefix(cpt);
	}


	if (!stricmp(XrefFile, CurrXFileName)) {
		XrefFile = NULL;
		XrefFilePrefix = CurrFilePrefix;
		HyperLinkIDNext = false;
	}

	if (UseListedXrefFilesOnly
	 && !XrefFound)
		XrefListed = false;  // but leave prefix alone
}


void 
HTMwr::ProcHyperFile(HTMctl *cp)
{
	// used at scan time only
	HyperJumpFile = true;
	HyperJumpFileID = (long) cp;
	HyperLinkIDNext = true;
	SetXrefFileName((char*) cp->ex(), cp->siz());
}


// hyperlink jump scan processing


void 
HTMwr::ProcHyperMark(HTMctl *cp)
{
	// process FrameMaker gotolink, newlink, and openlink
	char *txt = (char *) cp->ex();
	char *colon = NULL;
	char *period = NULL;
	static char str[128];		 // make value an enum ****

	// ending may be "path/filename.ext:ref"
	if (!_strnicmp(txt, "gotolink", 8)    // FrameMaker specific
	 || !_strnicmp(txt, "openlink", 8)) {	// anchor href
		txt += 9; // skip tag and following space
		char *fname = NULL;
		if ((colon = strrchr(txt, ':')) != NULL) { // handle filename
			strncpy(str, txt, colon - txt);  // put filename into str
			if ((period = strrchr(str, '.')) != NULL) {
				fname = NewName(str, period - str);  // keep base name
				strcpy(period + 1, XrefFileSuffix);  // change file suffix
			}
			else {  // or append file suffix if there is none
				fname = NewName(str);
				strcat(str, ".");
				strcat(str, XrefFileSuffix);
			}
			if (!strncmp(colon + 1, "firstpage", 9)		// nonunique names,
			 || !strncmp(colon + 1, "lastpage", 8)) { // use filename as ref
				txt = str + strlen(str); // terminate at end, may add ref later
			}																
			else {  // use ref with filename
				txt = colon + 1;
				strcat(str, "#");
				strcat(str, txt);
			}
		}
		else {  // no filename, must be local ref
			str[0] = '#';
			strncpy(&str[1], txt, 128);
		}
		//ProcExtRef(txt, true);
		char *refstr = strcpy(NewName(strlen(str) + 1), str);
		SetHyperLink(cp, refstr, 3);
	}
	else if (!_strnicmp(txt, "alert", 5)) {
		if (strlen(txt) > 6)
			ProcHyperAlert(cp, txt + 6);
	}
	else if (!_strnicmp(txt, "newlink", 7)) {	 // anchor name
		// ignore now, handle during write phase
	}
	// ignore anything else, like nextpage
}


bool HTMwr::OpenlinkIsFile = false;


void 
HTMwr::ProcHyperLink(HTMctl *cp, char *txt)
{
	// hyperlink jump to text, "gotolink" already stripped

	if (txt == NULL)
		txt = (char *) cp->ex();

	char *ptr = NULL;
	short len = (short) cp->siz();
	bool isfile = false;

#if 0
	if ((cp->dat() == 5)  // openlink not gotolink
	 && OpenlinkIsFile
	 && (!HyperJumpFile  // no filename
	  || (((HTMctl*) HyperJumpFileID)->siz() < 3))) {  // or just one for the drive letter
		HTMctl *ncp = new HTMctl(*cp);
		ncp->dat(3);
		if (HyperJumpFile)
			len += (short) (((HTMctl*) HyperJumpFileID)->siz() + 1);
		ptr = NewName(len);
		if (HyperJumpFile) {
			strcpy(ptr, (char *) ((HTMctl*) HyperJumpFileID)->ex());
			strcat(ptr, ":");
		}
		strcat(ptr, txt);
		if (RemoveFilePaths) {  // get rid of any pathname components
		  char *cpt = strrchr(ptr, '/');	// remove any path now, fails later
			if (!cpt)
				cpt = strrchr(ptr, '\\');
			if (cpt)
				ptr = NewName(cpt + 1);
		}
		ncp->ex(ptr);
		ProcHyperFile(ncp);  // use ctl as the filename
		isfile = true;
	}
#endif

	static char str[LineMax + 1];

	if ((*txt == '\0')
	 || !_strnicmp(txt, "firstpage", 9)		// nonunique names,
	 || !_strnicmp(txt, "lastpage", 8)) { // use filename as ref

#if 0
		str[0] = '#';
		if (XrefFile) {
			if (XrefMapped) {
				len = strlen(XrefRawFile);
				if (len > 127)
					len = 127;
				strncpy(&str[1], XrefRawFile, len); // terminate there
			}
			else {
				len = strlen(XrefFile);
				if (len > 127)
					len = 127;
				strncpy(&str[1], XrefFile, len); // terminate there
			}
			txt = str;
			str[len + 1] = '\0';
			if ((ptr = strrchr(str, '/')) != NULL) // always uses / as sep
				*(txt = ptr) = '#'; // strip leading path
			XrefFile = NULL;
		}
		else { // use our own base name
			len = strlen(CurrFileName);
			if (len > 127)
				len = 127;
			strncpy(&str[1], CurrFileName, len);
			str[len + 1] = '\0';
			txt = str;
		}
#endif

		// pass empty string as link, filename is sufficient
		// str[0] = '\0';
		// but keep trailing # to distinguish from URL...
		str[0] = '#';
		str[1] = '\0';

		txt = str;
		//ProcExtRef(txt, true);
	}
	else {  // relative in current file
		str[0] = '#';
		str[1] = '\0';

		MakeID(txt, &str[1], LineMax, HyperSpaceChar, true, true, MakeFileHrefsLower);
		//strcat(str, txt);
		//ProcExtRef(txt, true);
#if 0
		if (XMLSource) {
			txt = NewName("#R", strlen(CurrFilePrefix) + strlen(str + 1) + 2);
			strcat(txt, CurrFilePrefix);
			strcat(txt, str + 1);
		}
		else
#endif
			txt = str;
	}																

	XrefFilePrefix = CurrFilePrefix;  // not used in this case

	SetHyperLink(cp, NewName(txt), 3);
}


void
HTMwr::SetHyperLink(HTMctl *cp, char *ahref, uns val)
{
	if (HyperJump)  // first one wins if two in same span
		return;

	// put ahref text in HyperJumpStrList under cp
	HyperJumpStrList.add(ahref, (long) cp);

	HyperJump = HyperJumpStarting = true;
	HyperVal = val;
	HyperJumpID = (long) cp;
	HyperJumpTypeList.append(val, HyperJumpID);
	if (HyperJumpFile
	 && (val == 3)) {
		HyperJumpFileList.add(XrefFile, HyperJumpID);
		HyperJumpFile = false;
		HyperLinkIDNext = true;
	}

	ProcCondLink(HyperJumpID);

	MarkList.all(GetLinkAttrs);
	if (LinkAttrLp) {
		LinkAttrLists.add(LinkAttrLp, HyperJumpID);
		LinkAttrLp = NULL;
	}
	if (JH2PopAttrLp) {
		JH2PopAttrLists.add(JH2PopAttrLp, HyperJumpID);
		JH2PopAttrLp = NULL;
	}
	if (JH2SecAttrLp) {
		JH2SecAttrLists.add(JH2SecAttrLp, HyperJumpID);
		JH2SecAttrLp = NULL;
	}

	// within, or just after, char format or color
	// (just before handled at beginning of format or color)

	if (HyperForm) {
		HyperFormStartList.append(HyperJumpID, HyperFormStartID);
		HyperJumpStarting = false;
	}
	else if (HyperFormEnding) {
		HyperFormStartList.append(HyperJumpID, HyperFormStartID);
		HyperFormEndList.add(HyperFormEndID);
		HyperJump = false;
	}
	else if (HyperColor) {
		HyperColorStartList.append(HyperJumpID, HyperColorStartID);
		HyperJumpStarting = false;
	}
	else if (HyperColorEnding) {
		HyperColorStartList.append(HyperJumpID, HyperColorStartID);
		HyperColorEndList.add(HyperColorEndID);
		HyperJump = false;
	}

	// may be just before, or may continue to end of para
}


void 
HTMwr::ProcHyperObj(HTMctl *cp, bool page)
{
	// hyperlink jump to numeric object, "X"
	char *refstr = NULL;
	char *fstr = page ? "#XPG" : "#X";
	long pgnum = (long) cp->siz();

	if (pgnum == 0) { // lastpage ref, need to point to top of last page
		pgnum = 1;      //  until we decide how to do that, point at start
	}

	if (UseXrefFilePrefix) {
		refstr = NewName(strlen(fstr) + strlen(XrefFilePrefix) + NumStrMax);
		sprintf(refstr, "%s%s%ld", fstr, XrefFilePrefix, pgnum);
		XrefFilePrefix = CurrFilePrefix;
	}
	else {
		refstr = NewName(strlen(fstr) + NumStrMax);
		sprintf(refstr, "%s%ld", fstr, pgnum);
	}

	// add ID value to database of required values later
	SetHyperLink(cp, refstr, 3);
}


void
HTMwr::ProcHyperAlert(HTMctl *cp, char *txt)
{
	if (LastMasterPage == LastPage)
		return;

	if (txt == NULL)
		txt = (char *) cp->ex();

	if (!_strnicmp(txt, "alert ", 6)) {
		txt += 6;
		while (*txt == ' ')
			txt++;
		if (*txt == '~')
			return;
	}

	if (!_strnicmp(txt, "split", 5)
	 && (ParaUID != 0)
	 && (!InTbl || AllowTbSplit)
	 && !SplitHere
	 && (SplitID != ParaUID)) {
		SplittingFile = true;
		SplitHere = true;
		MarkerSetSplit = true;
		ProcSplit();
		return;
	}

	if ((HelpType != 'M')
	 && !Eclipse
	 && !OracleHelp
	 && !JHVersion2)
		return;

	// process alert popup text in seperate frame
	
	SetHyperLink(cp, NewName(txt), 2);
}


void
HTMwr::ProcHyperURL(HTMctl *cp, char *txt)
{
	if (txt == NULL)
		txt = (char *) cp->ex();

	// text following "message url"
	SetHyperLink(cp, NewName(txt), 6);
}


void
HTMwr::PutHyperMacro(HTMwfile *wf, HTMctl *cp, char *txt)
{
	if (txt == NULL)
		txt = (char *) cp->ex();

	// text of HTML Macro is macro name
	if (InPara == false)
		WriteText(wf, NULL);

	if (strchr(txt, '<')     // chars not legal in macro name
	 || strchr(txt, '>'))
		DCmacr::WriteMacro(wf, txt);       // new form, macro content
	else
		DCmacr::WriteMacroName(wf, &txt);  // old form, name only
}


// hyperlink aname write processing


DCnlist HTMwr::HyperLocList;

void 
HTMwr::ProcHyperLoc(HTMctl *cp)
{
	// newlink text, no prefixing
	char *txt = (char *) cp->ex();
	if (!UseRawNewlinks)
		txt = MakeID(txt, NULL, 0, HyperSpaceChar, true, true, MakeFileHrefsLower);
	HyperLocList.add(txt, (long) cp);
	if (HXMwr::HXML
	 && !HXMwr::DITAParaTagIDSet.find(ParaRefNum))
		HXMwr::DITAParaTagIDList.replace(txt, ParaRefNum);

	SetIntRef(txt);
}

void 
HTMwr::PutHyperLoc(HTMwfile *wf, HTMctl *cp)
{
	// newlink text, no prefixing
	// char *txt = (char *) cp->ex();

	char *txt = HyperLocList.find((long) cp);
	WriteAName(wf, txt);
	if (HelpType == 'J') {
		mapprop *mp = new mapprop;
		mp->name = txt;
		if (JHVersion2
		 || OracleHelp
		 || Eclipse) {
			char *nam = Extracting ? wf->getAuxName() : wf->getFileName();
			mp->ref = NewName(nam, strlen(nam) + strlen(txt) + 1);
			strcat(mp->ref, "#");
			strcat(mp->ref, txt);
		}
		else
			mp->ref = NewName(Extracting ? wf->getAuxName() : wf->getFileName());
		MapNewlinkList.add(mp, ++NewlinkCount);
		MapNewlinkNameList.add(txt, NewlinkCount);
	}
	if ((OmniHelp || MakeAliasFile)
	 && CheckAlias(txt)) {
		mapprop *mp = new mapprop;
		mp->name = NewName(txt);
		if (UseAliasAName) {
			char *nm = Extracting ? wf->getAuxName() : wf->getFileName();
			mp->ref = NewName(nm, strlen(nm) + strlen(txt) + 1);
			strcat(mp->ref, "#");
			strcat(mp->ref, txt);
		}
		else
			mp->ref = NewName(Extracting ? wf->getAuxName() : wf->getFileName());
		if (AliasTitle) {
			long sid = Extracting ? ExtrID : (SplitID ? SplitID : 1);
			char *cpt = SplitTitleList.find(sid);
			if (cpt)
				mp->win = NewName(cpt);
		}
		AliasList.add(mp, ++AliasCount);
	}
}


DCnlist HTMwr::TopicAliasList;

void 
HTMwr::ProcTopicAlias(HTMctl *cp)
{
	char *txt = (char *) cp->ex();
	char *val = (char *) (cp->ex());
	char *colon = strchr(val, ':');
	if (colon
	 && !strnicmp(val, "topicalias", colon - val))
		txt = colon + 1;
	TopicAliasList.add(txt, (long) cp);
}

void 
HTMwr::PutTopicAlias(HTMwfile *wf, HTMctl *cp)
{
	char *txt = TopicAliasList.find((long) cp);
	if (HelpType == 'J') {
		mapprop *mp = new mapprop;
		mp->name = txt;
		if (JHVersion2
		 || OracleHelp
		 || Eclipse) {
			char *nam = Extracting ? wf->getAuxName() : wf->getFileName();
			mp->ref = NewName(nam, strlen(nam) + strlen(txt) + 1);
			strcat(mp->ref, "#");
			strcat(mp->ref, txt);
		}
		else
			mp->ref = NewName(Extracting ? wf->getAuxName() : wf->getFileName());
		MapNewlinkList.add(mp, ++NewlinkCount);
		MapNewlinkNameList.add(txt, NewlinkCount);
	}
	if ((OmniHelp || MakeAliasFile)
	 && CheckAlias(txt)) {
		mapprop *mp = new mapprop;
		mp->name = NewName(txt);
		if (UseAliasAName) {
			char *nm = Extracting ? wf->getAuxName() : wf->getFileName();
			mp->ref = NewName(nm, strlen(nm) + strlen(txt) + 1);
			strcat(mp->ref, "#");
			strcat(mp->ref, txt);
		}
		else
			mp->ref = NewName(Extracting ? wf->getAuxName() : wf->getFileName());
		if (AliasTitle) {
			long sid = Extracting ? ExtrID : (SplitID ? SplitID : 1);
			char *cpt = SplitTitleList.find(sid);
			if (cpt)
				mp->win = NewName(cpt);
		}
		AliasList.add(mp, ++AliasCount);
	}
	if (HXMwr::DITA
	 && UseDITAAlias
	 && CheckAlias(txt))
		HXMwr::PendingAliasList.add(txt, ++HXMwr::PendingAlias);

	if (UMXL
	 && MXLwr::UseKeydefAliases
	 && !MXLwr::KeydefAliases.find(SplitID))
		MXLwr::KeydefAliases.add(txt, SplitID);
}

bool HTMwr::MakeAliasFile = false;
bool HTMwr::UseAliasAName = false;
bool HTMwr::AliasTitle = false;
bool HTMwr::UseDITAAlias = true;
char *HTMwr::AliasPrefix = NULL;
char **HTMwr::AliasPxs = NULL;
long HTMwr::AliasPxCount = 0;
DCvlist HTMwr::AliasList;
long HTMwr::AliasCount = 0;


void
HTMwr::SetAliasPxs(void)
{
	if (!AliasPrefix)
		return;

	char *chp = AliasPrefix;
	bool inPx = false;
	while (*chp != '\0') {
		if ((*chp == ' ')
		 || (*chp == '\t')
		 || (*chp == ',')) {
			if (inPx) {
				AliasPxCount++;
				inPx = false;
			}
		}
		else
			inPx = true;
		chp++;
	}
	if (inPx) {
		AliasPxCount++;
		inPx = false;
	}
	AliasPxs = (char **) new char[AliasPxCount * sizeof(char*)];
	AliasPxCount = 0;
	for (chp = AliasPrefix; *chp; chp++) {
		if ((*chp == ' ')
		 || (*chp == '\t')
		 || (*chp == ',')) {
			if (inPx) {
				AliasPxCount++;
				inPx = false;
			}
			*chp = '\0';
		}
		else {
			if (!inPx) {
				inPx = true;
				AliasPxs[AliasPxCount] = chp;
			}
		}
	}
	if (inPx)
		AliasPxCount++;
}

bool
HTMwr::CheckAlias(char *chp)
{
	if (AliasPxCount) {
		long i = 0;
		char *prp = NULL;
		for (; i < AliasPxCount; i++) {
			prp = AliasPxs[i];
			if (!_strnicmp(chp, prp, strlen(prp)))
				break;
		}
		if (i == AliasPxCount)
			return false; // no match
	}
	return true;
}


DCnlist HTMwr::HyperUniRefList;
DCilist HTMwr::HyperUniIdList;
long HTMwr::HyperUnique[HULevMax];
short HTMwr::HULevel = 0;
char HTMwr::HUType = 'A';


void 
HTMwr::ProcHyperUnique(HTMctl *cp, bool page)
{
	bool ditaid = false;

	if (page) {
		if (!PageNum)
			PageNum = NewName(10);
		sprintf(PageNum, "%ld", cp->siz());
	}

	if (!page
	 && StartingFrame) {
		if (!RFrame->id)        // get frame ID
			RFrame->id = (long) cp->siz();
		if (!RFrame->grname) {  // get frame name
			char *ref = NewName(strlen(CurrFilePrefix) + FrIDLen + 1);
			sprintf(ref, "%s%.*lx", CurrFilePrefix, FrIDLen, (long) cp->siz());
			RFrame->grname = ref;
		}
		StartingFrame = false;
	}

	if (LastPage <= LastMasterPage)
		return;

	if (ParaRefNum == 0)
		return;

	if (TableIDPend) { // ID is for full table
		TableIDPend = false;
		if (StripTable)
			return;
		HXMwr::CurrDITATblID = Tbl->tblid = NewName(strlen(CurrFilePrefix) + 11);
		sprintf(Tbl->tblid, "%s%ld", CurrFilePrefix, (long) cp->siz());
		if (HXMwr::HXML) {
			Tbl->tblpref = ParaRefNum;
			HXMwr::DITAElemTypeList.append(3, Tbl->tblpref);
			HXMwr::DITATableIDList.add(Tbl->tblid, Tbl->tblpref);
			ditaid = true;
		}
		if (TableConds) {
			CondTables.add(TableConds, (long) HXMwr::CurrDITATblID);
			TableConds = NULL;
		}
	}

	if (!page
	 && !ParaUID
	 && InPara) { // scan phase
		ParaUID = (long) cp->siz();  // first UID in para
		sprintf(ParaUIDRef, "%ld", ParaUID);
		if (HXMwr::HXML
		 && !ditaid
		 && !HXMwr::DITAParaTagIDSet.find(ParaRefNum)) {
			char *txt = NULL;
			if (InTbTitle
			 && HXMwr::CurrDITATblID
			 && !InFootnote) {
				txt = HXMwr::CurrDITATblID;
				HXMwr::DITAParaTagIDUsed.add(ParaRefNum); // prevent double use
				//Tbl->tblpref = ParaRefNum;
				//HXMwr::DITAParaTagIDSet.add(ParaRefNum);
			}
			else {
				txt = NewName(strlen(CurrFilePrefix) + NumStrMax);
				sprintf(txt, "%s%ld", CurrFilePrefix, (long) cp->siz());
			}
			HXMwr::DITAParaTagIDList.add(txt, ParaRefNum);
			if (InFootnote) {
				FootnoteParaRefList.append(FootnoteNum, ParaRefNum);
				HXMwr::DITAElemTypeList.append(5, ParaRefNum);
			}
			else {
				HXMwr::DITASetElemType(Curr.dtcname, Curr.dtpar);
#if 0
				long val = 0;
				if (!strcmp(Curr.dtcname, "li")
				 || FindPar(Curr.dtpar,"li"))
					val = 6;
				else if (!strcmp(Curr.dtcname, "title")) {
					if (FindPar(Curr.dtpar,"section")
					 || FindPar(Curr.dtpar,"example")
					 || FindPar(Curr.dtpar,"refsyn"))
						val = 2;
					else if (FindPar(Curr.dtpar,"fig"))
						val = 4;
				}
				if (val)
					HXMwr::DITAElemTypeList.add(val, ParaRefNum);
#endif
			}
		}
		if (SplitHere  // set file name and title if possible
		 && (!InTbl || AllowTbSplit))
			ProcSplit();
		else if (ExtrStarting)
			ProcStartExtract();
		else if (ExtrEnding)
			ProcEndExtract();
	}

	if (MarkerIDNext) {  // for list_tok list markers
		char *mref = NULL;
		mref = NewName(strlen(CurrFilePrefix) + NumStrMax + 2);
		sprintf(mref, "X%s%ld", CurrFilePrefix, (long) cp->siz());
		MarkerIDList.add(mref, (long) MarkerCtl);
		MarkerIDNext = false;
	}

	if (XrefLocNext) {  // never need ID of xref marker
		XrefLocNext = false;
		//return;
	}

	if (HyperLinkIDNext) {  // ID for ref to ext file, used in ProcExtRef
		char *lref = NULL;
		lref = NewName(strlen(CurrFilePrefix) + NumStrMax);
		sprintf(lref, "%s%ld", CurrFilePrefix, (long) cp->siz());
		HyperLinkRefList.add(lref, HyperJumpID);
		HyperLinkIDNext = false;
	}

	if ((page && !HXMwr::HXML)
	 || Contents
	 || EclipseContextAnchors
	 || (HUType == 'A')
	 || ((HUType == 'R')
	  && (KeepParaUID
	   || InIdxGroup))) {

		char *ref = NULL;
		char *fstr = page ? "#XPG" : "#X";
		long refnum = (long) cp->siz();
		if (InIdxGroup
		 && (KeywordRefs == 'P')
		 && ParaUID
		 && UseIndex)
			refnum = ParaUID;

		if (UseXrefFilePrefix) {
			ref = NewName(strlen(fstr) + strlen(CurrFilePrefix) + NumStrMax);
			sprintf(ref, "%s%s%ld", fstr, CurrFilePrefix, refnum);
		}
		else {
			ref = NewName(strlen(fstr) + NumStrMax);
			sprintf(ref, "%s%ld", fstr, refnum);
		}

		if (!IntRefPend)
			IntRefPend = new DClist();
		IntRefPend->append((long) NewName(ref + 1));
		//SetIntRef(ref + 1);

		HyperUniRefList.add(ref, (long) cp);
		HyperUniIdList.append(Extracting ? ExtrID : SplitID, (long) cp);
		if (Contents)
			ContentsRef = NoContLink ? "" : NewName(ref);
	}
}

void 
HTMwr::PutHyperUnique(HTMwfile *wf, HTMctl *cp)
{
	if (cp->dat() == 12) {
		if (!PageNum)
			PageNum = NewName(10);
		sprintf(PageNum, "%ld", cp->siz());
	}

	if (!ParaUID) {
		ParaUID = cp->siz();
		GetHyperUniqueRef(ParaUIDRef, (long) cp);
		if (!*ParaUIDRef)
			sprintf(ParaUIDRef, "%ld", ParaUID);
	}

	if (HUType == 'N')
		return;

	// check .ref [IntRefs] to see if this one is needed

	if (InIdxGroup) {
		IndexRefCtl = cp;
		if ((KeywordRefs == 'P')
		 && ParaUID
		 && UseIndex)
			return;  // don't put in ParaUID twice
	}
	else if (XrefLinkIDNext) {
		if (!XrefTextOnly) {
			if (XrefLinkID)
				DeleteName(XrefLinkID);
			XrefLinkID = NewName(strlen(CurrFilePrefix) + NumStrMax);
			sprintf(XrefLinkID, "%s%ld", CurrFilePrefix, (long) cp->siz());
		}
		XrefLinkIDNext = false;
		return;
	}
	else if ((HUType == 'R')   // referenced
	 && !EclipseContextAnchors
	 && !ALinkPara
	 && (Curr.cntform == 0))
		return;

	if (NoANames)
		return;

	HyperUnique[HULevel++] = (long) cp;
	if (HULevel == HULevMax)
		HULevel--;

	if (InPara
	 && !InHyper)
		WriteHyperUnique(wf);
}


void 
HTMwr::WriteHyperUnique(HTMwfile *wf)
{
	char ref[NameBufMax];

	if (HULevel == 0)
		return;

	if (NoANames) {
		HULevel = 0;
		return;
	}

	for (short i = 0; i < HULevel; i++) {
		GetHyperUniqueRef(ref, HyperUnique[i]);
		if (*ref) {
			wf->putAName(ref, NULL, ATagLineBreak);
			*ref = '\0';
		}
	}
	HULevel = 0;
}

void
HTMwr::GetHyperUniqueRef(char *str, long cp)
{
	char *ref = HyperUniRefList.find(cp);

	if (!ref)
			*str = '\0';
	else {
		if (*ref == '#')
			ref++;
		strcpy(str, ref);
	}
}

// hyperlink jump write processing


void 
HTMwr::PutHypertext(HTMwfile *wf, HTMctl *cp)
{
	// process full FrameMaker hypertext marker
	char *txt = (char *) cp->ex();

	if (!_strnicmp(txt, "newlink", 7)) {	 // anchor name
		txt += 8; // skip tag and following space
		WriteAName(wf, txt);	// newlink text, no prefixing
	}
	else if (!_strnicmp(txt, "gotolink", 8)  // FrameMaker
	 || !_strnicmp(txt, "openlink", 8)) {    // anchor href
		// already handled at scan time, ignore now
		if (UseXrefFilePrefix)
			XrefFilePrefix = CurrFilePrefix;
		if (HyperJumpFile)
			HyperJumpFile = false;
	}
	// ignore anything else, like nextpage
}


void 
HTMwr::PutHyperTarget(HTMctl *cp)
{
	// used to set target="_blank"
	TargetFrame = (char *) cp->ex();
}


void
HTMwr::WriteHyperAHref(HTMwfile *wf)
{
	if (Curr.nohref && !Curr.keeplink)
		return;

	if (CurrTab->tpos
	 && (CurrTab->tpos > LinePos)) {
		WriteEndCharProps(wf);
		WriteTabs(wf);
    CurrTab->tpos = 0;  // turn off
  }

	char *ahref = NewName(HyperJumpStr);
	char *targ = NULL;
	size_t len = ahref ? strlen(ahref) + 1 : 1;
	bool frset = false;

	HyperJumpFileName = HyperJumpFileList.find(HyperJumpID);
	HyperLinkID = HyperLinkRefList.find(HyperJumpID);
	char *oname = NewName(HyperJumpFileName);

	if (ahref) {
		if (*ahref == '#') {
			ahref = ProcExtRef(ahref, true, SecWin ? false : true, true);
			len = ahref ? strlen(ahref) + 1 : 1;
		}
		else  { // url
			ExtRefNum++;   // skip number to prevent update of prev ref
			SetTargetFrame(ahref);
			frset = true;
		}
	}

	if (!frset
	 && (oname || HyperJumpFileName))
		SetTargetFrame(oname, HyperJumpFileName);

	if (HyperJumpFileName != NULL) {
		if (NoLocations || XrefNoRef || Curr.noref || !ahref)
			ahref = NewName(HyperJumpFileName);
		else {
			len += strlen(HyperJumpFileName);
			char *xstr = NewName(HyperJumpFileName, len);
			strcat(xstr, ahref);
			ahref = xstr;
		}
		if (HelpType == 'M')
			SetDestChm(NewName(HyperJumpFileName), oname);
		HyperJumpFileName = NULL;
	}
	else if (SecWin && (HelpType == 'M'))
		SetDestChm();
	else
		DestChm = NULL;

	if (oname)
		DeleteName(oname);

	size_t alen = strlen(ahref);

	if (alen
	 && (ahref[alen - 1] == '#'))
		ahref[alen - 1] = '\0';

	if (SecWin
	 && ((HelpType == 'M')
	  || JHVersion2 || OracleHelp))
		WriteSecWinJump(wf, ahref, frset, false, HyperJumpID);
	else
		StartAHref(wf, ahref, (HyperVal == 6) ? 6 : 1, true, HyperJumpID);
}



// ALinks for HTML and Oracle Help

bool HTMwr::ALinkHere = false;
DCnlist HTMwr::ALinkList;
bool HTMwr::MakeALinkFile = true;
long HTMwr::ALinkCount = 0;
char *HTMwr::ALinkRef = NULL;
DCvlist HTMwr::OracleALinkList;
DCnlist HTMwr::OHALinkList;


void
HTMwr::WriteALinkObject(HTMwfile *wf, char *aname)
{
	if ((HelpType != 'M')
	 && (HelpType != 'C')
	 && !(OracleHelp && ALinkRef))
		return;

	CurrentDiv = wf->GetDiv();

	// chop up at semicolons into multiple entries
	char *semi = NULL;
	while ((semi = strchr(aname, ';')) != NULL) {
		*semi = '\0';
		WriteOneALink(wf, aname);
		aname = semi + 1;
	}
	WriteOneALink(wf, aname);  // last one
}


void
HTMwr::WriteOneALink(HTMwfile *wf, char *aname)
{
	while (isspace(*aname))
		aname++;
	if (*aname == '\0')
		return;
	char *colon = strchr(aname, ':');
	if (colon) {
		*colon++ = '\0';
		while (isspace(*colon))
			colon++;
		if (*colon == '\0')
			colon = NULL;
	}

	if (HelpType == 'M') {
		// <Object type="application/x-oleobject"
		// classid="clsid:1e2a7bd0-dab9-11d0-b93a-00c04fc99f9e">
		//  <param name="ALink Name" value="Testing">
		// </OBJECT>

		wf->SelectDiv(Extracting ? ExtrObjectDiv : ObjectDiv);

		wf->startBlock("object");
		wf->strAttr("type", "application/x-oleobject");
		wf->strAttr("classid", "clsid:1e2a7bd0-dab9-11d0-b93a-00c04fc99f9e");
		wf->endElem(true);
		wf->putEnd();
		WriteParam(wf, "ALink Name", aname);
		wf->putBlockEnd("object");
		wf->putEnd();

		wf->SelectDiv(CurrentDiv);
	}
	else if (OracleHelp) {
		ixprop *ap = new ixprop;
		ap->name = NewName(aname);
		ap->ref = ALinkRef;
		ap->title = colon ? colon :
                 ((Extracting || SplitID) ? 
		              SplitTitleList.find(Extracting ? ExtrID : SplitID) :
		              CurrXFileTitle);
		ap->sort = NewName(aname, strlen(aname) + strlen(ap->title));
		strcat(ap->sort, ap->title);
		OracleALinkList.add(ap, ++ALinkCount);
	}
	else if (HelpType == 'C') {
		DClist *pt = NULL;
		if ((pt = (DClist *) OHALinkList(aname)) == NULL) {
			if (UnicodeFTS)
				OHALinkList.addsortlex(NewName(aname), (long) (pt = new DClist), cmplexstrs);
			else
				OHALinkList.addsorted(NewName(aname), (long) (pt = new DClist));
		}
		pt->add(ContentsID);
		ALinkCount++;
	}
}


// popups and secondary windows for HTML Help


short HTMwr::PopTextNum = 0;
char *HTMwr::PopFont = "Helvetica,10,,PLAIN";
char *HTMwr::PopText = "Poptext";
char *HTMwr::PopMargins = "9,9";
char *HTMwr::PopColors = "-1,-1";
char *HTMwr::PopFormat = "JavaScript:popup.TextPopup(%s,Popfont,%s,%s)";
long HTMwr::ScriptDiv = 0;
long HTMwr::ExtrScriptDiv = 0;


void
HTMwr::WriteHyperPopup(HTMwfile *wf, char *href, bool map)
{
	if (JHVersion2) {
		WriteJHPopup(wf, href, map);
		return;
	}

	if (OracleHelp) {
		WriteOHJPopup(wf, href, map);
		return;
	}

	// rest of this code is for MSHH
	// JavaScript:popup.TextPopup(PoptextN,Popfont,9,9,-1,-1)
	if (HelpType != 'M')
		return;

	CurrentDiv = wf->GetDiv();

	// HyperJumpStr becomes text var in scriptdiv, needs name
	size_t len = strlen(PopText);
	char *txtname = NewName(PopText, len + NumStrMax);
	sprintf(txtname + len, "%d", ++PopTextNum);

	// popup margins and colors should be settable too
	char *ref = NewName((strlen(PopFormat) - 5) + strlen(txtname)
                      + strlen(PopMargins) + strlen(PopColors));
	sprintf(ref, PopFormat, txtname, PopMargins, PopColors);

	ExtRefNum++;   // skip number to prevent update of prev ref

	if (map)
		wf->strAttr("href", ref);
	else
		StartAHref(wf, ref, 2, false, NULL);  // do *not* add chm here, not filename

	wf->SaveAttrs();
	// if first item, scriptdiv needs settable font var too
	//   and popup object should be produced
	if (Extracting && (ExtrScriptDiv == 0)) {
		WritePopupObject(wf);
		ExtrScriptDiv = wf->NewDiv();
		wf->putStr("Popfont=\"");
		wf->putStr(PopFont);
		wf->putChar('"');
		wf->putEnd();
	}
	else if (!Extracting && (ScriptDiv == 0)) {
		WritePopupObject(wf);
		ScriptDiv = wf->NewDiv();
		wf->putStr("Popfont=\"");
		wf->putStr(PopFont);
		wf->putChar('"');
		wf->putEnd();
	}
	else
		wf->SelectDiv(Extracting ? ExtrScriptDiv : ScriptDiv);

	WriteRefFileInfo(wf, strlen(txtname) + 3, Extracting ? ExtrScriptDiv : ScriptDiv);
	// store the offset of the filename in the script so that when
  // script div is written final pos can be recorded in .ref
	wf->putStr(txtname);
	wf->putStr("=\"");
	wf->putStr(href);
	wf->putChar('"');
	wf->putEnd();
	wf->SelectDiv(CurrentDiv);
	wf->RestoreAttrs();
	DeleteName(txtname);
}


void
HTMwr::WritePopupObject(HTMwfile *wf)
{
	wf->SelectDiv(Extracting ? ExtrObjectDiv : ObjectDiv);

	wf->startBlock("object");
	wf->strAttr(HXMwr::IDattr, "popup");
	wf->putEnd();
	wf->strAttr("type", "application/x-oleobject");
	wf->strAttr("classid", MSHHctrl);
	wf->endElem(true);
	wf->putEnd();
	wf->putBlockEnd("object");
	wf->putEnd();

	wf->SelectDiv(CurrentDiv);
}


bool HTMwr::SecWin = false;
short HTMwr::SecWinNum = 0;
char *HTMwr::DefaultChmFile = NULL;
char *HTMwr::DestChm = NULL;
char *HTMwr::ChmFormat = "mk:@MSITStore:%s.chm::/%s";
char *HTMwr::SecWinParamFormat = ";%s.chm::/%s";
char *HTMwr::SecWinJumpFormat = "JavaScript:%s.Click()";
bool HTMwr::RemoveChmFilePaths = true;
bool HTMwr::BinaryIndex = false;
bool HTMwr::BinaryTOC = false;



void
HTMwr::SetDestChm(char *fname, char *oname)
{
	// must set DestChm based on filename after or before splitting
	DestChm = NULL;

	char *per = NULL;
	if (!oname)
		oname = CurrFileName;
	else if ((per = strrchr(oname, '.')) != NULL)
		*per = '\0';  // truncate extension of orig name

	if (fname
	 && ((per = strrchr(fname, '.')) != NULL))
		*per = '\0';  // truncate extension of new name

	char *cpt = NULL;
	char *oxname = NULL;
	char *fxname = NULL;

	if (RemoveChmFilePaths) {  // get rid of any pathname components
		char *cpt = strrchr(oname, '/');	// remove any path now, fails later
		if (!cpt)
			cpt = strrchr(oname, '\\');
		if (cpt)
			oxname = cpt + 1;
		if (fname) {
			cpt = strrchr(fname, '/');	// remove any path now, fails later
			if (!cpt)
				cpt = strrchr(fname, '\\');
			if (cpt)
				fxname = cpt + 1;
		}
	}

	if (IniFile
	 && IniFile->Section("ChmFiles")
	 && ((fname && IniFile->Find(fname))    // try new name first
	  || IniFile->Find(oname)    // then orig name (always present)
	  || (fxname && IniFile->Find(fxname))  // then new name pathless
	  || (oxname && IniFile->Find(oxname))))   // then orig name pathless
		DestChm = IniFile->StrRight();

	if (DestChm == NULL)
		DestChm = DefaultChmFile;
}


void
HTMwr::WriteSecWinJump(HTMwfile *wf, char *ahref, bool url, bool map, long attrid)
{
	// must work with both xrefs and hyperlinks, using [SecWindows]
	// property of char format used for xref or for spanning
	if (Curr.secwin
	 && !XrefSecWinName)
		XrefSecWinName = Curr.secwin;

	if (JHVersion2) {
		if (!_stricmp(XrefSecWinName, "popup"))
			WriteJHPopup(wf, ahref, map, attrid);
		else
			WriteJHSecWinJump(wf, ahref, url, map, attrid);
		return;
	}

	if (OracleHelp) {
		if (!_stricmp(XrefSecWinName, "popup"))
			WriteOHJPopup(wf, ahref, map);
		else
			WriteOHJSecWinJump(wf, ahref, url, map);
		return;
	}

	if (HelpType != 'M')
		return;

	CurrentDiv = wf->GetDiv();

	// create a new ID
	char *id = NewName("SecWin", NumStrMax + 7);
	sprintf(id + 6, "%d", ++SecWinNum);

	// write a new object with the win, CHM and HREF embedded in it
	ExtRefNum++;   // skip number to prevent update of prev ref

	if (url)
		WriteSecWinObject(wf, id, XrefSecWinName, ahref, 0);
	else {
		size_t fnpos = strlen(DestChm) + 8;
		char *param = NewName((strlen(SecWinParamFormat) - 3) + strlen(DestChm)
		 + strlen(ahref) + 1);
		sprintf(param, SecWinParamFormat, DestChm, ahref);
		WriteSecWinObject(wf, id, XrefSecWinName, param, fnpos);
	}

	// make an href that references the object
	char *ref = NewName((strlen(SecWinJumpFormat) - 1) + strlen(id) + 1);
	sprintf(ref, SecWinJumpFormat, id);

	if (map)
		wf->strAttr("href", ref);
	else
		StartAHref(wf, ref, 3, false, NULL);  // no .chm should be added to object

	DeleteName(id);
	SecWin = false;

	XrefSecWinName = NULL;
}


void
HTMwr::WriteSecWinObject(HTMwfile *wf, char *id, char *win, char *ref, size_t fnpos)
{
	wf->SelectDiv(Extracting ? ExtrObjectDiv : ObjectDiv);

	wf->startBlock("object");
	wf->strAttr(HXMwr::IDattr, id);
	wf->putEnd();
	wf->strAttr("type", "application/x-oleobject");
	wf->strAttr("classid", MSHHctrl);
	wf->putEnd();
	wf->strAttr("codebase", "hhctrl.ocx#Version=4,74,8702,0");
	wf->endElem(true);
	wf->putEnd();

	// <PARAM name="Command" value="Related Topics">
	// <PARAM name="Window" value="TestWindow">
	WriteParam(wf, "Command", "Related Topics");
	WriteParam(wf, "Window", win);


	// <PARAM name="Item1" value=";Test.chm::/11998437.htm#681">
	if (fnpos)  // zero for arbitrary URL, no .ref info wanted
		WriteRefFileInfo(wf, fnpos + 27, Extracting ? ExtrObjectDiv : ObjectDiv);
  // param: pos of filename in ref + 27 (pos of ref in param)
	// needed to compute loc in file for .ref
	WriteParam(wf, "Item1", ref);

	wf->putBlockEnd("object");
	wf->putEnd();

	wf->SelectDiv(CurrentDiv);
}



// HTML Help specific OBJECT and SCRIPT handling


void
HTMwr::PutObjects(HTMwfile *wf, bool final)
{
	if (!(Extracting ? ExtrObjectDiv : ObjectDiv))
		return;

	CurrentDiv = wf->GetDiv();

	ExtObjectRefList.write(Extracting ? ExtrID : (SplitID ? SplitID : 1), wf, FixOneDivRef);
	//ExtObjectRefList.empty();

	wf->WriteDiv(Extracting ? ExtrObjectDiv : ObjectDiv);
	wf->ClearDiv();
	SecWinNum = 0;

	wf->SelectDiv(CurrentDiv);
	wf->putEnd();

	if (final) {
		wf->DeleteDiv(ObjectDiv);
		wf->DeleteDiv(ExtrObjectDiv);
		ObjectDiv = ExtrObjectDiv = 0;
		wf->SelectDiv(CurrentDiv);
	}
}


void
HTMwr::WriteParam(HTMwfile *wf, char *name, char *val)
{
	wf->startElem("param");
	wf->strAttr("name", name);
	wf->strAttr("value", val);
	wf->endElem();
	wf->putEnd();
}


void
HTMwr::PutScript(HTMwfile *wf)
{
	if ((Extracting ? ExtrScriptDiv : ScriptDiv) == 0)
		return;

	CurrentDiv = wf->GetDiv();
	wf->startBlock("script");
	wf->strAttr("language", "JavaScript");
	wf->strAttr("type", "text/javascript");

	wf->endElem(true);
	wf->putEnd();

	ExtScriptRefList.write(Extracting ? ExtrID : (SplitID ? SplitID : 1), wf, FixOneDivRef);
	//ExtScriptRefList.empty();

	wf->WriteDiv(Extracting ? ExtrScriptDiv : ScriptDiv);
	wf->DeleteDiv(Extracting ? ExtrScriptDiv : ScriptDiv);
	(Extracting ? ExtrScriptDiv : ScriptDiv) = 0;
	PopTextNum = 0;
	wf->SelectDiv(CurrentDiv);

	wf->endBlock("script");
	wf->putEnd();
}


// popups and sec windows for JavaHelp 2

char *HTMwr::PopMarkerPrefix = NULL;
char *HTMwr::JH2PopMarkerPrefix = "JH2Pop";
char *HTMwr::SecMarkerPrefix = NULL;
char *HTMwr::JH2SecMarkerPrefix = "JH2Sec";

char HTMwr::JH2PopType = 'B';
char *HTMwr::JH2PopText = "&gt;";
char *HTMwr::JH2PopSize = "250,300";
char *HTMwr::JH2PopLocation = "600,200";
char *HTMwr::JH2PopFontFamily = "SansSerif";
char *HTMwr::JH2PopFontSize = "small";
char *HTMwr::JH2PopFontWeight = "plain";
char *HTMwr::JH2PopFontStyle = "plain";
char *HTMwr::JH2PopFontColor = "blue";
char *HTMwr::JH2PopGraphic = "../graphics/1p.gif";

char HTMwr::JH2SecType = 'B';
char *HTMwr::JH2SecText = "&gt;";
char *HTMwr::JH2SecSize = "250,300";
char *HTMwr::JH2SecLocation = "600,200";
char *HTMwr::JH2SecName = NULL;
char *HTMwr::JH2SecFontFamily = "Serif";
char *HTMwr::JH2SecFontSize = "small";
char *HTMwr::JH2SecFontWeight = "plain";
char *HTMwr::JH2SecFontStyle = "plain";
char *HTMwr::JH2SecFontColor = "blue";
char *HTMwr::JH2SecGraphic = "../graphics/1p.gif";

char HTMwr::JH2TmpType = '\0';
char *HTMwr::JH2TmpText = NULL;
char *HTMwr::JH2TmpSize = NULL;
char *HTMwr::JH2TmpLocation = NULL;
char *HTMwr::JH2TmpName = NULL;
char *HTMwr::JH2TmpFontFamily = NULL;
char *HTMwr::JH2TmpFontSize = NULL;
char *HTMwr::JH2TmpFontWeight = NULL;
char *HTMwr::JH2TmpFontStyle = NULL;
char *HTMwr::JH2TmpFontColor = NULL;
char *HTMwr::JH2TmpGraphic = NULL;

void
HTMwr::WriteJHPopup(HTMwfile *wf, char *href, bool map, long attrid)
{
	// ugly use of object in place of href

	if (map) { // impossible, make into jump
		StartAHref(wf, href, 2);
		SecWin = false;
		XrefSecWinName = NULL;
		return;
	}

	DCnlist *attrlist = NULL;
	// set temps to override globals as specified by JH2Pop markers
	if (attrid
	 && ((attrlist = (DCnlist *) JH2PopAttrLists.find(attrid)) != NULL))
		attrlist->all(SetAttrJH2Item);

	wf->startBlock("object");
	wf->strAttr("classid", "java:com.sun.java.help.impl.JHSecondaryViewer");
	wf->endElem(true);
	wf->putEnd();

	char jh2type = (JH2TmpType ? JH2TmpType : JH2PopType);

	WriteParam(wf, "content", href);
	WriteParam(wf, "viewerStyle", "javax.help.Popup");
	WriteParam(wf, "viewerActivator", (jh2type == 'B') ?
	                 "javax.help.LinkButton" : "javax.help.LinkLabel");
	WriteParam(wf, "viewerSize", JH2TmpSize ? JH2TmpSize : JH2PopSize);
	// for popups, location is automatically close to link, so this value is not used
	//WriteParam(wf, "viewerLocation", JH2TmpLocation ? JH2TmpLocation : JH2PopLocation);

	if (jh2type != 'G') {
		WriteParam(wf, "text", JH2TmpText ? JH2TmpText : JH2PopText);

		WriteParam(wf, "textFontFamily", JH2TmpFontFamily ? JH2TmpFontFamily : JH2PopFontFamily);
		WriteParam(wf, "textFontSize", JH2TmpFontSize ? JH2TmpFontSize : JH2PopFontSize);
		WriteParam(wf, "textFontWeight", JH2TmpFontWeight ? JH2TmpFontWeight : JH2PopFontWeight);
		WriteParam(wf, "textFontStyle", JH2TmpFontStyle ? JH2TmpFontStyle : JH2PopFontStyle);
		WriteParam(wf, "textColor", JH2TmpFontColor ? JH2TmpFontColor : JH2PopFontColor);
	}
	else {
		WriteParam(wf, "iconByName", JH2TmpGraphic ? JH2TmpGraphic : JH2PopGraphic); // URL for image used without button
		//WriteParam(wf, "iconByID", "");  // not apparent what id is meant, mapfile?
	}

	//if (fnpos)  // zero for arbitrary URL, no .ref info wanted
	//	WriteRefFileInfo(wf, fnpos + 27, Extracting ? ExtrObjectDiv : ObjectDiv);
  // param: pos of filename in ref + 27 (pos of ref in param)
	// needed to compute loc in file for .ref
	//WriteParam(wf, "Item1", ref);

	wf->putBlockEnd("object");
	wf->putEnd();
	SecWin = false;
	XrefSecWinName = NULL;
	ClearJH2Tmps();
}

void
HTMwr::WriteJHSecWinJump(HTMwfile *wf, char *ahref, bool url, bool map, long attrid)
{
	if (map) { // impossible, make into jump
		StartAHref(wf, ahref, 3);
		SecWin = false;
		XrefSecWinName = NULL;
		return;
	}

	DCnlist *attrlist = NULL;
	// set temps to override globals as specified by JH2Sec markers
	if (attrid
	 && ((attrlist = (DCnlist *) JH2SecAttrLists.find(attrid)) != NULL))
		attrlist->all(SetAttrJH2Item);

	// write JH2 object for jump, nasty business because of embedding hotspot text
	wf->startBlock("object");
	wf->strAttr("classid", "java:com.sun.java.help.impl.JHSecondaryViewer");
	wf->endElem(true);
	wf->putEnd();

	char jh2type = (JH2TmpType ? JH2TmpType : JH2SecType);

	WriteParam(wf, "content", ahref);  // can be URL or mapid

	WriteParam(wf, "viewerStyle", "javax.help.SecondaryWindow");
	WriteParam(wf, "viewerActivator", (jh2type == 'B') ?
	                 "javax.help.LinkButton" : "javax.help.LinkLabel");

	if (XrefSecWinName // get props for window
	 && !JH2SecName)
		JH2SecName = XrefSecWinName;

	WriteParam(wf, "viewerSize", JH2TmpSize ? JH2TmpSize : JH2SecSize);
	WriteParam(wf, "viewerLocation", JH2TmpLocation ? JH2TmpLocation : JH2SecLocation);
	WriteParam(wf, "viewerName", JH2TmpName ? JH2TmpName :
	                            (JH2SecName ? JH2SecName : "sec"));
	
	if (jh2type != 'G') {
		WriteParam(wf, "text", JH2TmpText ? JH2TmpText : JH2SecText);

		WriteParam(wf, "textFontFamily", JH2TmpFontFamily ? JH2TmpFontFamily : JH2SecFontFamily);
		WriteParam(wf, "textFontSize", JH2TmpFontSize ? JH2TmpFontSize : JH2SecFontSize);
		WriteParam(wf, "textFontWeight", JH2TmpFontWeight ? JH2TmpFontWeight : JH2SecFontWeight);
		WriteParam(wf, "textFontStyle", JH2TmpFontStyle ? JH2TmpFontStyle : JH2SecFontStyle);
		WriteParam(wf, "textColor", JH2TmpFontColor ? JH2TmpFontColor : JH2SecFontColor);
	}
	else {
		WriteParam(wf, "iconByName", JH2TmpGraphic ? JH2TmpGraphic : JH2SecGraphic);
		//WriteParam(wf, "iconByID", "");  // not apparent what id is meant
	}

	//if (fnpos)  // zero for arbitrary URL, no .ref info wanted
	//	WriteRefFileInfo(wf, fnpos + 27, Extracting ? ExtrObjectDiv : ObjectDiv);
  // param: pos of filename in ref + 27 (pos of ref in param)
	// needed to compute loc in file for .ref
	//WriteParam(wf, "Item1", ref);

	wf->putBlockEnd("object");
	wf->putEnd();
	SecWin = false;
	XrefSecWinName = NULL;
	ClearJH2Tmps();
}

void
HTMwr::ClearJH2Tmps(void)
{
	JH2TmpType = '\0';
	JH2TmpText = NULL;
	JH2TmpSize = NULL;
	JH2TmpLocation = NULL;
	JH2TmpName = NULL;
	JH2TmpFontFamily = NULL;
	JH2TmpFontSize = NULL;
	JH2TmpFontWeight = NULL;
	JH2TmpFontStyle = NULL;
	JH2TmpFontColor = NULL;
	JH2TmpGraphic = NULL;
}

void
HTMwr::SetAttrJH2Item(char *n, long i)
{
	if (!n || !i
	 || (*n == '\0'))
		return;

	char *equal = strchr(n, '=');
	if (!equal)
		return;
	*equal = '\0';
	char *val = equal + 2;
	char *quote = strchr(val, '"');
	if (!quote) {
		*equal = '=';
		return;
	}
	*quote = '\0';

	if (!strcmp(n, "type"))
		JH2TmpType = toupper(*val);
	else if (!strcmp(n, "text"))
		JH2TmpText = NewName(val);
	else if (!strcmp(n, "size"))
		JH2TmpSize = NewName(val);
	else if (!strcmp(n, "location"))
		JH2TmpLocation = NewName(val);
	else if (!strcmp(n, "name"))
		JH2TmpName = NewName(val);
	else if (!strcmp(n, "graphic"))
		JH2TmpGraphic = NewName(val);
	else if (!strcmp(n, "fontfamily"))
		JH2TmpFontFamily = NewName(val);
	else if (!strcmp(n, "fontsize"))
		JH2TmpFontSize = NewName(val);
	else if (!strcmp(n, "fontweight"))
		JH2TmpFontWeight = NewName(val);
	else if (!strcmp(n, "fontstyle"))
		JH2TmpFontStyle = NewName(val);
	else if (!strcmp(n, "fontcolor"))
		JH2TmpFontColor = NewName(val);

	*equal = '=';
	*quote = '"';
}


// popups and sec windows for Oracle Help

void
HTMwr::WriteOHJPopup(HTMwfile *wf, char *href, bool map)
{
	// set popID for map file entry, use href="popup:FIDpopID", don't use target
	mapprop *mp = new mapprop;
	mp->name = NewName(strlen(CurrFilePrefix) + NumStrMax + 7); 
	sprintf(mp->name, "%spopupid%ld", CurrFilePrefix, ++PopupCount);
	mp->ref = NewName(href);  // but when do/did we call ProcExtRef on it?? ****
	MapPopupList.add(mp, PopupCount);

	char *nhref = NewName("popup:", strlen(mp->name) + 6);
	strcat(nhref, mp->name);
	StartAHref(wf, nhref, 2);
	DeleteName(nhref);

	SecWin = false;
	XrefSecWinName = NULL;
}

void
HTMwr::WriteOHJSecWinJump(HTMwfile *wf, char *ahref, bool url, bool map)
{
	// set topicID for map file entry, use href="topicid:topicID"
	// in map file, use target with a window name defined in the .hs
	mapprop *mp = new mapprop;
	mp->name = NewName(strlen(CurrFilePrefix) + NumStrMax + 7); 
	sprintf(mp->name, "%stopicid%ld", CurrFilePrefix, ++TopicCount);
	mp->ref = NewName(ahref);  // but when do/did we call ProcExtRef on it?? ****
	mp->win = NewName(TargetFrame ? TargetFrame : XrefSecWinName);
	MapTopicList.add(mp, TopicCount);

	char *nhref = NewName("topicid:", strlen(mp->name) + 8);
	strcat(nhref, mp->name);
	StartAHref(wf, nhref, 3);
	DeleteName(nhref);

	SecWin = false;
	XrefSecWinName = NULL;
	TargetFrame = NULL;
}


// xref format processing

DCnlist HTMwr::XrefNameList;
DCnlist HTMwr::XrefLinkSrcList;
long HTMwr::XrefID = 0;
long HTMwr::XrefInstID = 0;
char *HTMwr::XrefInstStr = NULL;
anyini HTMwr::XrefIni;
DCilist HTMwr::XrefUseList;
bool HTMwr::XrefTextOnly = false;
bool HTMwr::XrefDelete = false;
bool HTMwr::XrefNoRef = false;
bool HTMwr::XrefLinkSrc = false;


void 
HTMwr::ProcXrefFormatName(HTMctl *cp)
{
	XrefName = NewName((char *) cp->ex());
	XrefNameList.add(XrefName, XrefID);

	long usage = 1;

	XrefTextOnly = XrefDelete = XrefNoRef = XrefLinkSrc = false;
	DCini::ProcIniSect("XrefStyles");
	if (XrefTextOnly)
		usage |= 2;
	if (XrefDelete)
		usage |= 4;
	if (XrefNoRef)
		usage |= 8;
	if (XrefLinkSrc)
		usage |= 16;

	XrefUseList.append(usage, XrefID);
	XrefID = 0;
}


void
HTMwr::ProcXrefFormatProp(char *propval, long *valp)
{
	if (!_strnicmp(propval, "Text", 4))
		*valp |= 2;
	else if (!_strnicmp(propval, "Delete", 6))
		*valp |= 4;
	else if (!_strnicmp(propval, "NoRef", 5))
		*valp |= 8;
	else if (!_strnicmp(propval, "LinkSrc", 7))
		*valp |= 16;
}


void 
HTMwr::ProcXrefFormatUse(HTMctl *cp)
{
	//DClist *xlist = (DClist *)(XrefSetIDList.find(XrefInstID));

	XrefTextOnly = XrefDelete = XrefNoRef = XrefLinkSrc = false;

	//if (xlist != NULL)  // marker overrides replace format props
	//	xlist->all(GetPropSettings);
	//else {              // use regular xref format props in .ini
		XrefID = cp->siz();
		long usage = XrefUseList.find(XrefID);
		XrefTextOnly = ((usage & 2) == 2) ? true : false;
		XrefDelete = ((usage & 4) == 4) ? true : false;
		XrefNoRef = ((usage & 8) == 8) ? true : false;
		XrefLinkSrc = ((usage & 16) == 16) ? true : false;
	//}
	XrefIni.nm[1] = XrefName = XrefNameList.find(XrefID);
	XrefIni.nm[0] = XrefInstStr = itoa(XrefInstID, NewName(NumStrMax), 10);
	XrefIni.cnt = 2;
	DCini::XrefSetList.all(DCini::GetXrefSettings);  // marker overrides replace format props

	XrefLinkIDNext = true;
}



// xref aname writing

DCnlist HTMwr::XrefRefList;
DCnlist HTMwr::XrefParaList;
char *HTMwr::XrefIDRef = "0";
char HTMwr::XrefSpaceChar = 0;
bool HTMwr::XrefLocNext = false;


void 
HTMwr::ProcXrefAName(HTMctl *cp)
{
	size_t len = 2;
	char *refstr = NULL;
	char *tstr = NULL;

	tstr = MakeXrefName(cp);

	len += strlen(tstr);

	if (UseXrefFilePrefix) {
		refstr = NewName(strlen(XrefFilePrefix) + len);
		*refstr = 'R';
		strcat(refstr, XrefFilePrefix);
		XrefFilePrefix = CurrFilePrefix;
	}
	else {
		refstr = NewName(len);
		*refstr = 'R';
	}

	strcat(refstr, tstr);
	if (HXMwr::HXML) {
		HXMwr::DITAParaTagIDList.add(refstr, ParaRefNum);
		if (!StripTable
		 && (InTbTitle
			|| (InTbl 
		   && (!HXMwr::CurrDITATblID
		    || !HXMwr::DITAParaTagIDList(HXMwr::CurrDITATblID))))
		 && !HXMwr::DITAElemTypeList.find(ParaRefNum))
			HXMwr::DITAElemTypeList.append(3, ParaRefNum);
	}
	SetIntRef(refstr);
	XrefRefList.add(refstr, (long) cp);
	if (!XrefParaList.find(ParaRefNum))
		XrefParaList.add(refstr, ParaRefNum);
	if (InTbTitle)
		Tbl->titleprn = ParaRefNum;
	XrefLocNext = true;
}


char *
HTMwr::MakeXrefName(HTMctl *cp)
{
	char *tstr = NULL;
	
	if (cp->dt() == ilong) {
		tstr = NewName(NumStrMax);
		sprintf(tstr, "%ld", cp->siz());
	}
	else {
		tstr = NewName((char *) cp->ex(), cp->siz());
		char *colon = strchr(tstr, ':');
		if (colon && ((colon - tstr) > 4)) {
			for (char *chp = tstr; chp < colon; chp++) {
				if (!isdigit(*chp))
					break;
			}
			if (chp == colon)  // all digits, at least five of them
				*colon = '\0';
		}
		char *buf = MakeID(tstr, NULL, 0, XrefSpaceChar, true);
		DeleteName(tstr);
		tstr = buf;
	}

	return tstr;
}


void 
HTMwr::PutXrefAName(HTMwfile *wf, HTMctl *cp)
{
	char *refstr = XrefRefList.find( (long) cp);
	if (refstr)
		WriteAName(wf, refstr);
}



// xref and hypertext aname writing

void 
HTMwr::WriteAName(HTMwfile *wf, char *str)
{
	if (InHyper
	 || NoHyper
	 || !InPara)
		StorePendingAName(str);
	else if (!NoANames) {
		WriteText(wf);
		wf->putAName(str, ATagName, ATagLineBreak);
	}
}


char *HTMwr::PendingANames[ANameLevMax];
uns HTMwr::ANameLevel = 0;


void 
HTMwr::StorePendingAName(char *str)
{
	PendingANames[ANameLevel++] = str;
	if (ANameLevel == ANameLevMax)
		ANameLevel--;
}


void 
HTMwr::WritePendingANames(HTMwfile *wf)
{
	if (NoHyper
	 || InRaw)
		return;

	WriteHyperUnique(wf); // ensure it comes first, for NS bug

	if (ANameLevel == 0)
		return;

	if (!NoANames) {
		for (short i = 0; i < ANameLevel; i++)
			wf->putAName(PendingANames[i], ATagName, ATagLineBreak);
	}

	ANameLevel = 0;
}


// xref jump scanning


bool HTMwr::InXref = false;
bool HTMwr::XrefSecWin = false;
HTMctl *HTMwr::XrefStart = NULL;
char *HTMwr::XrefSecWinName = NULL;
DCnlist HTMwr::XrefSecWinUse;


void 
HTMwr::ProcStartAnchor(HTMctl *cp)
{
	InXref = true;
	XrefStart = cp;
	XrefSecWin = false;
	XrefLinkSrc = false;
	HyperLinkIDNext = true;
	ProcCondLink( (long) cp);
}


void 
HTMwr::ProcEndAnchor(HTMctl *cp)
{
	InXref = false;

	//if (XrefSetList) {
	//	XrefSetIDList.add((void *) &XrefSetList, XrefInstID);  // fix this
	//	XrefSetList = NULL;
	//}
	XrefIni.nm[0] = XrefInstStr = itoa(XrefInstID, NewName(NumStrMax), 10);
	DCini::XrefSetList.all(DCini::SetXrefID);

	if (XLSMacro) {  // set by marker
		XrefLinkSrcList.add(XLSMacro, XrefInstID);
		XLSMacro = NULL;
	}

	XrefInstID = 0;

	MarkList.all(GetLinkAttrs);
	if (LinkAttrLp) {
		LinkAttrLists.add(LinkAttrLp, (long) XrefStart);
		LinkAttrLp = NULL;
	}
	if (JH2PopAttrLp) {
		JH2PopAttrLists.add(JH2PopAttrLp, (long) XrefStart);
		JH2PopAttrLp = NULL;
	}
	if (JH2SecAttrLp) {
		JH2SecAttrLists.add(JH2SecAttrLp, (long) XrefStart);
		JH2SecAttrLp = NULL;
	}

	if (XrefSecWin)
		XrefSecWinUse.add(XrefSecWinName, (long) XrefStart);

	XrefStart = NULL;
	XrefSecWin = false;
	XrefLinkSrc = false;
	XrefSecWinName = NULL;
}


// xref jump writing


void 
HTMwr::StartAnchor(HTMwfile *wf, HTMctl *cp)
{
	size_t len = 3;
	char *tstr = NULL;

	if (XrefTextOnly
	 || XrefDelete)
		return;

	tstr = MakeXrefName(cp);

	len += strlen(tstr);

	if (UseXrefFilePrefix)
		len += strlen(XrefFilePrefix);

	char *refstr = NewName("#R", len);
	//strcpy(refstr, "#R");

	if (UseXrefFilePrefix) {
		strcat(refstr, XrefFilePrefix);
		XrefFilePrefix = CurrFilePrefix;
	}
	
	strcat(refstr, tstr);

	XrefSecWinName = XrefSecWinUse.find((long) cp);
	if ((XrefSecWinName || Curr.secwin)
	 && ((HelpType == 'M') || JHVersion2 || OracleHelp))
		XrefSecWin = SecWin = true;
	char *oname = NewName(XrefFile);
	char *xbase = NULL;

	if ((UseListedXrefFilesOnly
	  && !XrefListed)
	 || (HXMwr::DocBook
		&& XrefFile
	  && (xbase = NewName(XrefFile, strrchr(XrefFile, '.') - XrefFile))
		&& FSNames.getitem(xbase))) {
		if (xbase)
			DeleteName(xbase);
		XrefInCurrDBFile = true;
	}
	else
		XrefInCurrDBFile = false;

	refstr = ProcExtRef(refstr, false, SecWin ? false : true, true);

	if (XrefInCurrDBFile)
		XrefFile = NULL;  // but leave prefix alone

	if (oname || XrefFile)
		SetTargetFrame(oname, XrefFile);

	if (XrefFile) {
		char *ahref = NULL;
		if (NoLocations || XrefNoRef || Curr.noref)
			ahref = NewName(XrefFile);
		else {
			len = strlen(refstr) + strlen(XrefFile) + 1;
			ahref = NewName(XrefFile, len);
			strcat(ahref, refstr);
		}
		if (HelpType == 'M')
			SetDestChm(NewName(XrefFile), oname);
		XrefFile = NULL;
		DeleteName(refstr);
		refstr = ahref;
	}
	else if (SecWin && (HelpType == 'M'))
		SetDestChm();
	else
		DestChm = NULL;

	if (oname)
		DeleteName(oname);

	if (!InHyper) {
		if (SecWin && ((HelpType == 'M') || JHVersion2 || OracleHelp))
			WriteSecWinJump(wf, refstr, false, false, (long) cp);
		else
			StartAHref(wf, refstr, 4, true, (long) cp);
	}

	XrefNoRef = false;
	XrefLinkSrc = false;
	XrefSecWin = false;
}


void
HTMwr::StartXrefText(HTMwfile *wf, HTMctl *cp)
{
	XrefFile = NULL;
	XrefInstID = (long) cp;
	XrefFilePrefix = HTMwr::CurrFilePrefix;
	WriteText(wf);

	if (CurrTab->tpos
	 && (CurrTab->tpos > LinePos)) {
		WriteEndCharProps(wf);
		WriteTabs(wf);
    CurrTab->tpos = 0;  // turn off
  }
}


void 
HTMwr::EndAnchor(HTMwfile *wf)
{
	if (XrefTextOnly
	 || XrefDelete) {
		XrefTextOnly = XrefDelete = false;
		return;
	}

	EndAHref(wf);
	XrefInstID = 0;
}


void
HTMwr::SetXrefTitle(DCctl *cp)
{
	// set title attribute for next ahref jump
	XrefTitle = (char *) cp->ex();
}



// hyperlink and xref final writing

bool HTMwr::InHyper = false;
bool HTMwr::NoHyper = false;
char *HTMwr::DefaultTarget = NULL;
char *HTMwr::TargetFrame = NULL;
char *HTMwr::URLTarget = NULL;
char *HTMwr::XrefTitle = NULL;
bool HTMwr::NoATags = false;
bool HTMwr::NoAHrefs = false;
bool HTMwr::NoANames = false;
bool HTMwr::XMLLinkAttrs = false;
bool HTMwr::NoLocations = false;
bool HTMwr::KeepReplacedCharLinks = false;
bool HTMwr::UseChmInLinks = false;
bool HTMwr::CharFormInLink = false;

long HTMwr::LinkDiv = 0;
char *HTMwr::LinkSrc = NULL;
char *HTMwr::ATagName = "a";
char *HTMwr::HrefAttribute = "href";
bool HTMwr::UseHash = true; 
bool HTMwr::UseUlink = false; 
bool HTMwr::UsingUlink = false; 
bool HTMwr::RemoveXrefHotspots = false;
bool HTMwr::UsingXrefDiv = false;
long HTMwr::XrefDiv = 0;
long HTMwr::PreXrefDiv = 0;
long HTMwr::XrefLevel = 0;

bool HTMwr::LinkTypeA = true; 
char HTMwr::LinkType = '\0';
char *HTMwr::LinkFlags = NULL;
char *HTMwr::LinkEmptyTopic = NULL;
char *HTMwr::LinkButtonWidth = NULL;
char *HTMwr::LinkButtonHeight = NULL;
char *HTMwr::LinkButtonText = NULL;
char *HTMwr::LinkButtonGraphic = NULL;
char *HTMwr::LinkButtonIcon = NULL;
char *HTMwr::LinkTextFont = NULL;
char *HTMwr::LinkText = NULL;
char HTMwr::ALinkType = '\0';
char *HTMwr::ALinkFlags = NULL;
char *HTMwr::ALinkEmptyTopic = NULL;
char *HTMwr::ALinkButtonWidth = NULL;
char *HTMwr::ALinkButtonHeight = NULL;
char *HTMwr::ALinkButtonText = NULL;
char *HTMwr::ALinkButtonGraphic = NULL;
char *HTMwr::ALinkButtonIcon = NULL;
char *HTMwr::ALinkTextFont = NULL;
char *HTMwr::ALinkText = NULL;
char HTMwr::KLinkType = '\0';
char *HTMwr::KLinkFlags = NULL;
char *HTMwr::KLinkEmptyTopic = NULL;
char *HTMwr::KLinkButtonWidth = NULL;
char *HTMwr::KLinkButtonHeight = NULL;
char *HTMwr::KLinkButtonText = NULL;
char *HTMwr::KLinkButtonGraphic = NULL;
char *HTMwr::KLinkButtonIcon = NULL;
char *HTMwr::KLinkTextFont = NULL;
char *HTMwr::KLinkText = NULL;

char *HTMwr::MSHHctrl = "clsid:adb880a6-d8ff-11cf-9377-00aa003b7a11";

bool HTMwr::Confluence = false;
bool HTMwr::ConfluenceLinks = false;
bool HTMwr::InConfluenceLink = false;
char *HTMwr::ConfluenceLinkStart = "<ac:link>";
char *HTMwr::ConfluenceLinkPage = "<ri:page ri:content-title=\"";
char *HTMwr::ConfluenceLinkPageEnd = "\"/>";
char *HTMwr::ConfluenceLinkText = "<ac:link-body>";
char *HTMwr::ConfluenceLinkTextEnd = "</ac:link-body>";
char *HTMwr::ConfluenceLinkEnd = "</ac:link>";


void 
HTMwr::StartAHref(HTMwfile *wf, char *str, unc typ,  bool addChm, long attrid)
{
	// typ for DITA, 1=hyperlink to FM file, 2=popup, 3=secwin, 4=xref, 5=ftn, 6=url

	if ((ParaForm.macpos & MacReplace)
	 || NoATags)
		return;

	char *hratt = HrefAttribute;
	char *atag = ATagName;
	if (UseUlink
	 && (typ == 6)) {
		UsingUlink = true;
		atag = "ulink";
		hratt = "url";
	}
	else
		UsingUlink = false;

	if (UseFootXrefTag) {
		long ftnum = 0;
		long pid = 0;
		long flen = strlen(FootInlineIDPrefix);
		if ((typ == 5)
		 || (HXMwr::DocBook
		  && ((pid = HXMwr::DITAParaTagIDList((*str == '#') ? str + 1 : str)) != 0)
		  && ((ftnum = FootnoteParaRefList.find(pid)) != 0))) {
			if (ftnum) {  // replace str
				str = NewName(FootInlineIDPrefix, flen + NumStrMax);
				_itoa(ftnum, str + flen, 10);
			}
			UsingFootXrefTag = true;
			atag = FootInlineXrefTag;
			hratt = "linkend";
		}
	}
	else
		UsingFootXrefTag = false;

	if (RemoveXrefHotspots
	 && (typ != 6))
		UsingXrefDiv = true;
	else
		UsingXrefDiv = false;

	char *chp = str;
	char ch = '\0';
	size_t fnpos = strlen(hratt) + 3; // length of ' href="'
	bool refinfo = false;

	if (InHyper)
		return;

	if (InPara == false) {
		WriteParaProps(wf);
		InPara = true;
	}
	CPEndAll(wf);

	if (XMLSource
	 && (InlineHyperLevel <= InlineStackLevel))
		;
	else if (Last.hsname
	 || (CharBlockMacro & (MacEnd | MacAfter)))
		EndCharBlock(wf);

	WritePendingANames(wf);

	if (CondChanged)
		CondTextChange(wf);

	if (ConfluenceLinks
	 && (typ != 6)) {
		InConfluenceLink = true;
		wf->putStr(ConfluenceLinkStart);
		wf->putEnd();
		CurrentDiv = wf->GetDiv();
		wf->putStr(ConfluenceLinkPage);
		WriteRefFileInfo(wf, 0, CurrentDiv); 
		char *chpp = chp;
		if (*chp == '#')
			chpp++;
		wf->putStr(chpp);
		wf->putStr(ConfluenceLinkPageEnd);
		wf->putEnd();
		wf->putStr(ConfluenceLinkText);
		return;
	}

	DCnlist *attrlist = NULL;
	if (attrid)
		attrlist = (DCnlist *) LinkAttrLists.find(attrid);

	if (!strnicmp(chp + 1, "link:", 5)) {  // alink or klink
		ch = tolower(*chp);
		if (OmniHelp) {
			size_t colons = 0;
			if (ch == 'k') { // fix up klinks
				for (chp = &str[6]; *chp; chp++)
					if (*chp == ':')
						colons++;
			}
			chp = NewName("javascript:", strlen(str) + colons + 26);
			str[5] = '\0';
			strcat(chp, _strlwr(str));
			str[5] = ':';
			strcat(chp, "('");
			char *cs = str + 6;
			char *cd = chp + strlen(chp);
			if (ch == 'k') { // fix up klinks
				while (*cs) {
					if (*cs == ':') {
						*cd++ = ',';
						*cd++ = ' ';
					}
					else
						*cd++ = *cs;
					cs++;
				}
			}
			else
				strcat(chp, str + 6);
			strcat(chp, "')");
			chp = CheckLinkSrc(wf, chp);
			InHyper = true;
			TargetFrame = NULL;  // keep in frameset
		}
		else if ((HelpType == 'J')
		 && (ch == 'a')) {  // alink only
			if (OracleHelp) {
				// just pass it through as is; may have to deal with multiples later
				chp = CheckLinkSrc(wf, chp);
				InHyper = true;
			}
			else  // not implemented in JavaHelp
				InHyper = false;
		}
		else if ((HelpType == 'M')
		 && ((ch == 'a')
			|| (ch == 'k'))) {  // alink or klink in HH, use object
			wf->startBlock("object");
			wf->strAttr(HXMwr::IDattr, "hhctrl");
			wf->strAttr("type", "application/x-oleobject");
			wf->strAttr("classid", MSHHctrl);
			if (ch == 'a') {
				LinkTypeA = true;
				if (attrlist)
					attrlist->all(SetAttrLinkItem);
				if (!ALinkType)
					ALinkType = 'B';
				if (ALinkType != 'T') {
					wf->strAttr("width", ALinkButtonWidth ? ALinkButtonWidth : "100");
					wf->strAttr("height", ALinkButtonHeight ? ALinkButtonHeight : "100");
				}
				wf->endElem(true);
				wf->putEnd();
				WriteParam(wf, "Command", "ALink");
				if (ALinkType == 'B')
					WriteParam(wf, "Button", ALinkButtonText ? ALinkButtonText : "Text:ALink");
				else if (ALinkType == 'C')
					WriteParam(wf, "Button", " ");
				else if (ALinkType == 'G')
					WriteParam(wf, "Button", ALinkButtonGraphic ? ALinkButtonGraphic : " ");
				else if (ALinkType == 'I')
					WriteParam(wf, "Button", ALinkButtonIcon ? ALinkButtonIcon : " ");
				else if (ALinkType == 'S')
					WriteParam(wf, "Button", "Bitmap:shortcut");
				else if (ALinkType == 'T') {
					WriteParam(wf, "Font", ALinkTextFont ? ALinkTextFont : "Helvetica,10,,PLAIN");
					WriteParam(wf, "Text", ALinkText ? ALinkText : "Text:Related Topics");
				}
				WriteParam(wf, "Flags", ALinkFlags ? ALinkFlags : "1");
				if (ALinkEmptyTopic)
					WriteParam(wf, "DefaultTopic", ALinkEmptyTopic);
			}
			else if (ch == 'k') {
				LinkTypeA = false;
				if (attrlist)
					attrlist->all(SetAttrLinkItem);
				if (!KLinkType)
					KLinkType = 'B';
				if (KLinkType != 'T') {
					wf->strAttr("width", KLinkButtonWidth ? KLinkButtonWidth : "100");
					wf->strAttr("height", KLinkButtonHeight ? KLinkButtonHeight : "100");
				}
				wf->endElem(true);
				wf->putEnd();
				WriteParam(wf, "Command", "KLink");
				if (KLinkType == 'B')
					WriteParam(wf, "Button", KLinkButtonText ? KLinkButtonText : "Text:KLink");
				else if (KLinkType == 'C')
					WriteParam(wf, "Button", " ");
				else if (KLinkType == 'G')
					WriteParam(wf, "Button", KLinkButtonGraphic ? KLinkButtonGraphic : " ");
				else if (KLinkType == 'I')
					WriteParam(wf, "Button", KLinkButtonIcon ? KLinkButtonIcon : " ");
				else if (KLinkType == 'S')
					WriteParam(wf, "Button", "Bitmap:shortcut");
				else if (KLinkType == 'T') {
					WriteParam(wf, "Font", KLinkTextFont ? KLinkTextFont : "Helvetica,10,,PLAIN");
					WriteParam(wf, "Text", KLinkText ? KLinkText : "Text:Index Items");
				}
				WriteParam(wf, "Flags", KLinkFlags ? KLinkFlags : "1");
				if (KLinkEmptyTopic)
					WriteParam(wf, "DefaultTopic", KLinkEmptyTopic);
			}
			WriteParam(wf, "Item1", "");      // path to .chm, not needed
			WriteLinkItems(wf, chp + 6);
			wf->putBlockEnd("object");
			wf->putEnd();
			InHyper = false;  // the in-line object does the whole job
		}
		else  // alink or klink not supported, ignore it)
			InHyper = false;
	}
	else {  // not alink or klink
		if ((HelpType == 'M')
		 && addChm
		 && DestChm
		 && (UseChmInLinks || stricmp(DestChm, DefaultChmFile))) {
			fnpos += strlen(DestChm) + (strlen(ChmFormat) - 4);
			chp = NewName(strlen(DestChm) + strlen(str) + (strlen(ChmFormat) - 4) + 1);
			sprintf(chp, ChmFormat, DestChm, str);
		}

		chp = CheckLinkSrc(wf, chp);
		InHyper = true;
#if 0
		if (strncmp(chp, "http:", 5)
		 && strncmp(chp, "ftp:", 4)
		 && strncmp(chp, "mailto:", 7))
#endif
		refinfo = true;
	}

	if (InHyper) {
		//InlineHyperLevel = InlineStackLevel;

		if (!HXMwr::InRelTopics) {
			if (InTextCond) { // existing ph or span, may end it
				if (CheckCondLink(attrid)) {  // end if not present for entire link
					CondTextEnd(wf);
					LinkInTextCond = false;
				}
				else
					LinkInTextCond = true;
			}
			if (HXMwr::DITA 
			 && !HXMwr::IsLegal(HXMwr::DITAtags->names->operator()(atag), HXMwr::CurrLevel)) {
				wf->startBlock("ph");
				wf->setClass(HXMwr::XrefWrapClass);
				wf->endElem(true);
				//wf->startSBlock("ph");
			}
			wf->startBlock(atag);
		}
		if (ATagLineBreak
		 && !InPreform
		 && !HXMwr::InRelTopics
		 && !HXMwr::HXML)
			wf->putEnd();  // put inside element to avoid unwanted spacing
		else if (refinfo)
			wf->putEnd();  // essential to get right loc for ref

		CurrentDiv = wf->GetDiv();
		if (refinfo)
			WriteRefFileInfo(wf, fnpos, CurrentDiv); // fnpos is pos of filename in chp 
		if (HXMwr::DITA)
			HXMwr::WriteDITALinkAttrs(wf, chp, typ);
		else if (!NoAHrefs) {
			char *chpp = chp;
			if (!UseHash
			 && (*chp == '#'))
				chpp++;
			wf->strAttr(hratt, chpp);
		}
		if (!HXMwr::InRelTopics) {
			StartCondLink(wf, attrid);
		}
		if (WriteClassAttributes) {
			if (Curr.plclass) {
				wf->setClass(Curr.plclass);
				// fnpos += (HXMwr::DITA ? 11 : 5) + strlen(Curr.plclass) + 4;
			}
			else if (XrefFormatIsXrefClass
			 && (typ == 4))
				wf->setClass(MakeID(XrefName, NULL, 0, ClassSpaceChar, false, true, LowerCaseCSS));
			else if (LinkClassIsParaClass) {
				wf->setClass(Curr.hcname);
				// fnpos += (HXMwr::DITA ? 11 : 5) + strlen(Curr.hcname) + 4;
			}
			else if (!HXMwr::HXML
			 && (typ == 5))
				wf->setClass((InTbTitle || InTbCell) ? TbFootClass : FootClass);
		}
		else if (HXMwr::DITA
		 && XrefFormatIsXrefClass
		 && (typ == 4))
			wf->setClass(MakeID(XrefName, NULL, 0, ClassSpaceChar, false, true, LowerCaseCSS));
		if (!TargetFrame
		 && (typ == 6)
		 && !UsingUlink
		 && URLTarget)
			TargetFrame = URLTarget;
		if (TargetFrame) {
			if (OmniHelp) {
				char *comma = strchr(TargetFrame, ',');
				if (comma) {
					char *fname = NewName(TargetFrame, comma - TargetFrame);
					char *next = comma + 1;
					unl wide = strtoul(next, &comma, 10);
					unl high = 0;
					char *prop = "";
					if (*comma == ',') {
						next = comma + 1;
						high = strtoul(next, &comma, 10);
					}
					if (*comma == ',') {
						next = comma + 1;
						if (*next == ' ')
							next++;
						prop = NewName(next);
					}
					wf->strAttr("target", fname);
					char *pt1 = "sec(this";
					char *pt2 = "); return false";
					char *attr = NewName((NumStrMax * 2) + strlen(pt1) + strlen(pt2) + strlen(prop) + 6);
					if (*prop)
						sprintf(attr, "%s, %u, %u, '%s'%s", pt1, wide, high, prop, pt2);
					else
						sprintf(attr, "%s, %u, %u%s", pt1, wide, high, pt2);
					wf->strAttr("onclick", attr);
				}
				else {
					wf->strAttr("target", TargetFrame);
					if (strcmp(TargetFrame, "main")
					 && strcmp(TargetFrame, "_blank"))
						wf->strAttr("onclick", "sec(this); return false");
				}
			}
			else if (!OracleHelp)
				wf->strAttr("target", TargetFrame);
			//TargetFrame = DefaultTarget;
			TargetFrame = NULL;
		}
		if (XrefTitle) {
			wf->strAttr("title", XrefTitle);
			XrefTitle = NULL;
		}
		if (attrlist)
			attrlist->writeall(wf, WriteAttrListItem);

		if (Xml && XMLLinkAttrs) { 
		//xml:link="simple" show="replace" actuate="user" class="url"
			wf->strAttr("xml:link", "simple");
			wf->strAttr("show", "replace");
			wf->strAttr("actuate", "user");
			wf->strAttr("class", "url");
		}
		if (!HXMwr::InRelTopics) {
			wf->endElem(true);
			if (HXMwr::DITA)
				HXMwr::StartDITALinkDiv(wf);
			else if (UsingXrefDiv) {
				PreXrefDiv = wf->GetDiv();
				XrefDiv = wf->NewDiv();
				XrefLevel = HXMwr::CurrLevel;
			}
		}
		else if (HXMwr::DITARelLinkDiv)
			HXMwr::EndDITARelLinkDiv(wf);
	}

	if (chp != str)
		DeleteName(chp);
}


void
HTMwr::SetAttrLinkItem(char *n, long i)
{
	if (!n || !i
	 || (*n == '\0'))
		return;

	char *equal = strchr(n, '=');
	if (!equal)
		return;
	*equal = '\0';
	char *val = equal + 2;
	char *quote = strchr(val, '"');
	if (!quote) {
		*equal = '=';
		return;
	}
	*quote = '\0';

	if (!stricmp(n, "type")) {
		if (LinkTypeA)
			ALinkType = toupper(*val);
		else
			KLinkType = toupper(*val);
	}
	else if (!stricmp(n, "emptytopic")) {
		if (LinkTypeA)
			ALinkEmptyTopic = NewName(val);
		else
			KLinkEmptyTopic = NewName(val);
	}
	else if (!stricmp(n, "buttonwidth")) {
		if (LinkTypeA)
			ALinkButtonWidth = NewName(val);
		else
			KLinkButtonWidth = NewName(val);
	}
	else if (!stricmp(n, "buttonheight")) {
		if (LinkTypeA)
			ALinkButtonHeight = NewName(val);
		else
			KLinkButtonHeight = NewName(val);
	}
	else if (!stricmp(n, "buttontext")) {
		if (LinkTypeA)
			ALinkButtonText = NewName(val);
		else
			KLinkButtonText = NewName(val);
	}
	else if (!stricmp(n, "buttongraphic")) {
		if (LinkTypeA)
			ALinkButtonGraphic = NewName(val);
		else
			KLinkButtonGraphic = NewName(val);
	}
	else if (!stricmp(n, "buttonicon")) {
		if (LinkTypeA)
			ALinkButtonIcon = NewName(val);
		else
			KLinkButtonIcon = NewName(val);
	}
	else if (!stricmp(n, "textfont")) {
		if (LinkTypeA)
			ALinkTextFont = NewName(val);
		else
			KLinkTextFont = NewName(val);
	}
	else if (!stricmp(n, "text")) {
		if (LinkTypeA)
			ALinkText = NewName(val);
		else
			KLinkText = NewName(val);
	}

	*equal = '=';
	*quote = '"';
}


char *HTMwr::XLSMacro = NULL;
char *HTMwr::XrefName = NULL;

char *
HTMwr::CheckLinkSrc(HTMwfile *wf, char *chp)
{
	LinkSrc = chp;
	if (XrefLinkSrc
	 || ((Curr.macpos & MacLinkSrc) == MacLinkSrc)
	 || Curr.macls) {
		CurrentDiv = wf->GetDiv();
		LinkDiv = wf->NewDiv();
		XLSMacro = NULL;
		if (XrefLinkSrc) {
			if ((XLSMacro = XrefLinkSrcList.find(XrefInstID)) == NULL) {
				XrefName = XrefNameList.find(XrefID);
				DCini::ProcIniSect("XrefStyleLinkSrc");
			}
			DCmacr::WriteMacro(wf, XLSMacro);
		}
		else {
			if (Curr.macls)
				DCmacr::WriteMacro(wf, Curr.macls);
			else
				PutStyleMacro(wf, MacLinkSrc, CurrCSName, CurrCSForm);
		}
		if (wf->LenDiv())
			chp = NewName(wf->ReadDiv());
		wf->DeleteDiv(LinkDiv);
		wf->SelectDiv(CurrentDiv);
	}
	return chp;
}

void
HTMwr::WriteLinkItems(HTMwfile *wf, char *chp)
{
	if (!chp)
		return;

	char itstr[8] = "Item"; // up to 9998 items
	int inum = 2;
	char *semi = NULL;

	while ((*chp != '\0')
	 && ((semi = strchr(chp, ';')) != NULL)) {
		sprintf(itstr + 4, "%d", inum++);
		*semi = '\0';
		WriteParam(wf, itstr, chp);
		*semi = ';';
		chp = semi + 1;
	}
	if (*chp) {
		sprintf(itstr + 4, "%d", inum++);
		WriteParam(wf, itstr, chp);
	}
}


void 
HTMwr::SetTargetFrame(char *ref, char *ref2)
{
	char *targ = NULL;

	if (TargetFrame)  // set by DCL
		return;

	if (DefaultTarget)
		TargetFrame = DefaultTarget;

	if (Curr.target) {
		TargetFrame = Curr.target;
		return;
	}

	if (Curr.secwin && OmniHelp) {
		TargetFrame = Curr.secwin;
		return;
	}

	if (ref) {
		if (!GetBaseName(ref, targ))
			targ = NewName(ref);

		if (IniFile
		 && IniFile->Section("TargetFiles")
		 && IniFile->Find(targ))
			TargetFrame = IniFile->StrRight();

		DeleteName(targ);
	}

	if (ref2) {
		if (!GetBaseName(ref2, targ))
			targ = NewName(ref2);

		if (IniFile
		 && IniFile->Section("TargetFiles")
		 && IniFile->Find(targ))
			TargetFrame = IniFile->StrRight();

		DeleteName(targ);
	}
}


void 
HTMwr::EndAHref(HTMwfile *wf)
{
	if ((Curr.macpos & MacReplace)
	 || NoATags)
		return;

	CPEndAll(wf);
	if (XMLSource
	 && (InlineHyperLevel <= InlineStackLevel))
		;
	else if (Last.hsname
	 || (CharBlockMacro & (MacEnd | MacAfter))
	 || (CharBlockState == CbEnd))
		EndCharBlock(wf, !HXMwr::DITA);

	if (InConfluenceLink) {
		wf->putStr(ConfluenceLinkTextEnd);
		wf->putEnd();
		wf->putStr(ConfluenceLinkEnd);
		InConfluenceLink = false;
		return;
	}

	if (InHyper
	 && !HXMwr::InRelTopics) {
		if (CharFormInLink)
			EndCharBlock(wf);
		CharFormInLink = false;
		if (InTextCond
		 && !LinkInTextCond)   // nested ph or span, end it
			CondTextEnd(wf);
		LinkInTextCond = false;
		if (HXMwr::DITA)
			HXMwr::EndDITALinkDiv(wf);
		else if (UsingXrefDiv) {
			HXMwr::EndLevels(wf, XrefLevel);
			wf->DeleteDiv(XrefDiv);
			XrefDiv = 0;
			wf->SelectDiv(PreXrefDiv);
		}
		char *atag = UsingFootXrefTag ? FootInlineXrefTag :
		             (UsingUlink ? "ulink" : ATagName);

		wf->endBlock(atag);
		if (HXMwr::DITA) {
			if (!HXMwr::IsLegal(HXMwr::DITAtags->names->operator()(atag), HXMwr::CurrLevel))
				wf->endBlock("ph");
			//if (CondChanged)
			//	CondTextEnd(wf);
		}
		UsingXrefDiv = UsingUlink = UsingFootXrefTag = false;
	}
	InHyper = false;
}


// autonumber processing


bool HTMwr::InAnumVal = false;
bool HTMwr::InAnumGroup = false;
bool HTMwr::EndingAnum = false;



// footnote writing


uns HTMwr::FtnStart = 1;
uns HTMwr::FtnForm = 1;
uns HTMwr::FtnMarker = 0;
bool HTMwr::InFootnote = false;
bool HTMwr::StartingFootnote = false;
bool HTMwr::InFtnAnum = false;
bool HTMwr::InTbFtnAnum = false;

char HTMwr::FootType = 'J';
long HTMwr::FootDiv = 0;
long HTMwr::PreFootDiv = 0;
char *HTMwr::FootSep = NULL;
char *HTMwr::FootnoteStartCode = NULL;
char *HTMwr::FootnoteEndCode = NULL;
long HTMwr::TbFootDiv = 0;
char *HTMwr::TbFootSep = NULL;
char *HTMwr::FootSepH = "<br><br><hr>";
char *HTMwr::FootSepX = "<br /><br /><hr />";
char *HTMwr::FootClass = "footnote";
char *HTMwr::TbFootClass = "tblfootnote";
long HTMwr::FootVType = 2;
long HTMwr::TbFootVType = 5;
char *HTMwr::FootTypeAttrs = "1iIaA";
char HTMwr::FootTypeAttr[2] = { '1', '\0' };
char HTMwr::TbFootTypeAttr[2] = { 'a', '\0' };
bool HTMwr::InFtPx = false;
bool HTMwr::InTbFtPx = false;
bool HTMwr::InAnFormat = false;
bool HTMwr::InAnRefFormat = false;
bool HTMwr::InAnSyms = false;
char HTMwr::FootPx[AnumPxSxMax] = "";
char HTMwr::TbFootPx[AnumPxSxMax] = "";
char HTMwr::FootSx[AnumPxSxMax] = "";
char HTMwr::TbFootSx[AnumPxSxMax] = "";
char HTMwr::FootRPx[AnumPxSxMax] = "";
char HTMwr::TbFootRPx[AnumPxSxMax] = "";
char HTMwr::FootRSx[AnumPxSxMax] = "";
char HTMwr::TbFootRSx[AnumPxSxMax] = "";
char HTMwr::FootSyms[AnumPxSxMax] = "";
char HTMwr::TbFootSyms[AnumPxSxMax] = "";
char *HTMwr::FootElem = "sup";
char *HTMwr::TbFootElem = "sup";
char *HTMwr::FootRElem = "";
char *HTMwr::TbFootRElem = "";
char *HTMwr::FootItemTag = "li";
char *HTMwr::TbFootItemTag = "li";
char *HTMwr::FtItemTag = "li";
char *HTMwr::FootBlockTag = "ol";
char *HTMwr::TbFootBlockTag = "ol";
uns HTMwr::FtnPosition = 0;
bool HTMwr::StartingFtn = false;
bool HTMwr::EndingFtn = false;
bool HTMwr::TbFootPos = false;
bool HTMwr::UseFootnoteParaCode = false;
bool HTMwr::UseFootnoteLists = true;
bool HTMwr::UseTbFootnoteLists = true;
bool HTMwr::NoFootnoteLinks = false;
bool HTMwr::InFootPara = false;

char *HTMwr::FootInlineTag = "footnote";
char *HTMwr::FootInlineParaTag = "para";
char *HTMwr::FootInlineIDPrefix = "foot";
char *HTMwr::FootInlineXrefTag = "footnoteref";
bool HTMwr::UseFootXrefTag = false;
bool HTMwr::UsingFootXrefTag = false;
char *HTMwr::FootID = NULL;
bool HTMwr::FootnoteXref = true;

uns HTMwr::FootnoteNum = 0;
uns HTMwr::TbFootNum = 0;
uns HTMwr::TxFootNum = 0;
DCilist HTMwr::FootnoteParaRefList;

void
HTMwr::WriteFtnStart(HTMwfile *wf)
{
	WriteText(wf);
	SaveState();
	FtnPosition = LinePos;
	bool table = false;

	if (FtnMarker == 0) {
		FootnoteNum++;
		if ((InTbCell
		  || (InTbTitle
		   && ((Tbl->titleplace == 1)
		    || IntTbCaption))
		  || InTbl)
		 && TbFootPos) {
			table = true;
			TbFootNum++;
		}
		else
			TxFootNum++;
	}

	if (FootType == 'I') {
		//if (CheckCondFtn(FootnoteNum)) {
			if (InTextCond)  // existing ph or span, may end it
				CondTextEnd(wf);
			ParaConds = CharConds = TextConds = NULL;
		//}
		CPEndAll(wf);
		if (Last.hsname
		 || (CharBlockMacro & (MacEnd | MacAfter)))
			EndCharBlock(wf);
		if (HXMwr::DITA 
		 && (InTbTitle
		  || !HXMwr::IsLegal(HXMwr::DITAtags->names->operator()(FootInlineTag), HXMwr::CurrLevel))) {
			wf->startBlock("ph");
			wf->setClass(HXMwr::FootnoteWrapClass);
			wf->endElem(true);
			//wf->startSBlock("ph");
		}

		wf->startBlock(FootInlineTag);
		if (HXMwr::DITA)
			wf->strAttr(HXMwr::IDattr, FootID = HXMwr::DITAParaTagIDList.find(FootnoteParaRefList(FootnoteNum)));
		else if (FootInlineIDPrefix
		 && *FootInlineIDPrefix) {
			long flen = strlen(FootInlineIDPrefix);
			char *ftid = NewName(FootInlineIDPrefix, flen + NumStrMax);
			_itoa(FootnoteNum, ftid + flen, 10);
			wf->strAttr(HXMwr::IDattr, ftid);
			DeleteName(ftid);
		}
		StartCondFtn(wf);
		wf->endElem(true);
		wf->SetAttrAdd(false);
	  InFootnote = true;
		//if (FootInlineParaTag)
		//	wf->startSBlock(FootInlineParaTag);
	}
	else if (FootType == 'E') {
		wf->putStr(" [");
	  InFootnote = true;
	}
	else if (FootType == 'J') {
		uns num = TbFootPos ? (table ? TbFootNum : TxFootNum) : FootnoteNum;
		PreFootDiv = wf->GetDiv();
		if (table) {
			if (!TbFootDiv)
				SetFootDiv(wf, true);
		}
		else {
			if (!FootDiv)
				SetFootDiv(wf, false);
		}
		// write ref string	using local ID
		char *ref = NewName(NumStrMax + 5);
		if (!NoFootnoteLinks) {
			sprintf(ref, "#ftn%d", FootnoteNum);
			StartAHref(wf, ref, 5);
		}
		char *fnum = NULL;
		if (table) {
			if (TbFootElem && *TbFootElem)
				wf->startSBlock(TbFootElem);
			wf->putStr(TbFootPx);
			fnum = SetNumVal(TbFootVType, num, TbFootSyms);
			wf->putStr(fnum);
			wf->putStr(TbFootSx);
			if (TbFootElem && *TbFootElem)
				wf->endBlock(TbFootElem);
		}
		else {
			if (FootElem && *FootElem)
				wf->startSBlock(FootElem);
			wf->putStr(FootPx);
			fnum = SetNumVal(FootVType, num, FootSyms);
			wf->putStr(fnum);
			wf->putStr(FootSx);
			if (FootElem && *FootElem)
				wf->endBlock(FootElem);
		}
		if (NoFootnoteLinks)
			CPEndAll(wf);
		else
			EndAHref(wf);

		// switch to footnote div
		if (table) {
			wf->SelectDiv(TbFootDiv);
			// if start list iten in <OL>, no need for number
			wf->startElem(FtItemTag = TbFootItemTag);
			if (WriteClassAttributes)
				wf->setClass(TbFootClass);
			wf->endElem(true);
			if (!UseTbFootnoteLists) {  // write number
				wf->putStr(TbFootRPx);
				wf->putStr(fnum);
				wf->putStr(TbFootRSx);
			}
		}
		else {
			wf->SelectDiv(FootDiv);
			// if start list iten in <OL>, no need for number
			wf->startElem(FtItemTag = FootItemTag);
			if (WriteClassAttributes)
				wf->setClass(FootClass);
			wf->endElem(true);
			if (!UseFootnoteLists) {  // write number
				wf->putStr(FootRPx);
				wf->putStr(fnum);
				wf->putStr(FootRSx);
			}
		}
		DeleteName(fnum);
		// write matching target
		if (!NoFootnoteLinks)
			wf->putAName(ref + 1, NULL, ATagLineBreak);
		DeleteName(ref);
		Curr = DefForm;
		DCmacr::WriteMacro(wf, FootnoteStartCode);
		if (UseFootnoteParaCode) {
			InFootPara = true;
			StartingFtn = true;
		}
		else
		  InFootnote = true;
	}

	SetCondFlags(wf);
  InPara = true;
}


char *
HTMwr::SetNumVal(long ntyp, uns nval, char *syms)
{
	char *nstr = NULL;
	char *tstr = NULL;
	char *tp = NULL;
	uns len = 0;
	uns cnt = 0;
	uns i = 0;

	switch (ntyp) {
		case 2:  //arabic
		default:
			nstr = NewName(NumStrMax);
			sprintf(nstr, "%ld", nval);
			break;
		case 3:  // lc Roman
			tstr = NewName(RomanMax);
			tp = tstr;
			while (nval >= 1000) {
				*tp++ = 'm';
				nval -= 1000;
			}
			if (nval >= 900) {
				*tp++ = 'c';
				*tp++ = 'm';
				nval -= 900;
			}
			while (nval >= 100) {
				*tp++ = 'c';
				nval -= 100;
			}
			if (nval >= 50) {
				*tp++ = 'l';
				nval -= 50;
			}
			if (nval >= 40) {
				*tp++ = 'x';
				*tp++ = 'l';
				nval -= 40;
			}
			while (nval >= 10) {
				*tp++ = 'x';
				nval -= 10;
			}
			if (nval >= 9) {
				*tp++ = 'i';
				*tp++ = 'x';
				nval -= 9;
			}
			if (nval >= 5) {
				*tp++ = 'v';
				nval -= 5;
			}
			if (nval >= 4) {
				*tp++ = 'i';
				*tp++ = 'v';
				nval -= 4;
			}
			while (nval >= 1) {
				*tp++ = 'i';
				nval--;
			}
			*tp = '\0';
			nstr = NewName(tstr);
			DeleteName(tstr);
			break;
		case 4:  // uc Roman
			tstr = NewName(RomanMax);
			tp = tstr;
			while (nval >= 1000) {
				*tp++ = 'M';
				nval -= 1000;
			}
			if (nval >= 900) {
				*tp++ = 'C';
				*tp++ = 'M';
				nval -= 900;
			}
			while (nval >= 100) {
				*tp++ = 'C';
				nval -= 100;
			}
			if (nval >= 50) {
				*tp++ = 'L';
				nval -= 50;
			}
			if (nval >= 40) {
				*tp++ = 'X';
				*tp++ = 'L';
				nval -= 40;
			}
			while (nval >= 10) {
				*tp++ = 'X';
				nval -= 10;
			}
			if (nval >= 9) {
				*tp++ = 'I';
				*tp++ = 'X';
				nval -= 9;
			}
			if (nval >= 5) {
				*tp++ = 'V';
				nval -= 5;
			}
			if (nval >= 4) {
				*tp++ = 'I';
				*tp++ = 'V';
				nval -= 4;
			}
			while (nval >= 1) {
				*tp++ = 'I';
				nval--;
			}
			*tp = '\0';
			nstr = NewName(tstr);
			DeleteName(tstr);
			break;
		case 5:  // lc alpha
			if (nval) {
				len = 26;
				cnt = (nval - 1) / len;
				nstr = NewName(cnt + 2);
				*nstr = 'a' + (unc) ((nval - 1) % len);
				for (i = 1; i <= cnt; i++)
					nstr[i] = *nstr;
			}
			break;
		case 6:  // uc alpha
			if (nval) {
				len = 26;
				cnt = (nval - 1) / len;
				nstr = NewName(cnt + 2);
				*nstr = 'A' + (unc) ((nval - 1) % len);
				for (i = 1; i <= cnt; i++)
					nstr[i] = *nstr;
			}
			break;
		case 7:  // custom
			if (nval
			 && syms
			 && *syms) {
				len = strlen(syms);
				cnt = (nval - 1) / len;
				nstr = NewName(cnt + 2);
				*nstr = syms[(nval - 1) % len];
				for (i = 1; i <= cnt; i++)
					nstr[i] = *nstr;
			}
			else
				nstr = NewName("*");
			break;
	}
	if (!nstr)  // guarantee a string
		nstr = NewName(2);
	return nstr;
}


void
HTMwr::WriteFtnEnd(HTMwfile *wf)
{
	SetCondFlags(wf);

	if (FootType == 'I') {
		//if (FootInlineParaTag)
		//	wf->endBlock(FootInlineParaTag);
		wf->endBlock(FootInlineTag);
		if (HXMwr::DITA) { 
			if (HXMwr::DITATopicID
			 && FootID
			 && FootnoteXref) {  // put xref immediately after footnote to get callout
				wf->startBlock("xref");
				char *xattr = NewName(strlen(HXMwr::DITATopicID) + strlen(FootID) + 3);
				sprintf(xattr, "#%s/%s", HXMwr::DITATopicID, FootID);
				wf->strAttr("href", xattr);
				wf->strAttr("scope", "local");
				wf->strAttr("type", "fn");
				wf->strAttr("format", "dita");
				StartCondFtn(wf);
				wf->endElem(true);
				wf->SetAttrAdd(false);
				wf->endBlock("xref");
				DeleteName(xattr);
			}
			if (InTbTitle
			 || !HXMwr::IsLegal(HXMwr::DITAtags->names->operator()(FootInlineTag), HXMwr::CurrLevel))
				wf->endBlock("ph");
		}
	}
	else if (FootType == 'E')
		wf->putStr("] ");
	else if (FootType == 'J') {
		if (StartingFtn)
			StartingFtn = false;
		else {  // multi-para footnote
			EndingFtn = true;
			Curr.list = 0;
			WriteParaStartProps(wf);
			EndingFtn = false;
		}
		if (Xhtml || Xml)
			wf->putBlockEnd(FtItemTag);
		CPEndAll(wf);
		DCmacr::WriteMacro(wf, FootnoteEndCode);
		//wf->putEnd();
		// switch back from footnote div
		wf->SelectDiv(PreFootDiv);
		WriteCharProps(wf);
		WriteText(wf);
	}

  InFootnote = false;
  FtnMarker = 0;
  InPara = true;

	if (!HXMwr::DITA
	 && (FootType != 'I')
	 && (FootType != 'E'))
		LinePos = FtnPosition;
	RestoreState();
}


void
HTMwr::FinishFtns(HTMwfile *wf, bool final, bool table)
{
	if (!(table ? TbFootDiv : FootDiv)
	 || !(table ? TbFootNum : TxFootNum))
		return;

	CurrentDiv = wf->GetDiv();
	DCmacr::WriteMacro(wf, table ? TbFootSep : FootSep);
	wf->WriteDiv(table ? TbFootDiv : FootDiv);
	wf->ClearDiv();
	wf->SelectDiv(CurrentDiv);
	wf->endBlock(table ? TbFootBlockTag : FootBlockTag);
	wf->putEnd();
	if (table)
		TbFootNum = 0;
	else {
		TxFootNum = 0;
		FootnoteNum = 0;
	}

	if (final) {
		wf->DeleteDiv(FootDiv);
		if (TbFootDiv != FootDiv)
			wf->DeleteDiv(TbFootDiv);
		FootDiv = TbFootDiv = 0;
		wf->SelectDiv(CurrentDiv);
	}
	else {
		wf->SelectDiv(table ? TbFootDiv : FootDiv);
		wf->startElem(table ? TbFootBlockTag : FootBlockTag);
		if (WriteClassAttributes)
			wf->setClass(table ? TbFootClass : FootClass);
		if (table ? TbFootTypeAttr : FootTypeAttr)
			wf->strAttr("type", table ? TbFootTypeAttr : FootTypeAttr);
		wf->endElem(true);
		wf->putEnd();
		wf->SelectDiv(CurrentDiv);
	}
}

void HTMwr::SetFootDiv(HTMwfile *wf, bool table)
{
	if (table) {
		if (!TbFootPos) {
			if (!FootDiv)
				SetFootDiv(wf, false);
			TbFootDiv = FootDiv;
		}
		else {
			TbFootDiv = wf->NewDiv();
			wf->startElem(TbFootBlockTag);
			if (WriteClassAttributes)
				wf->setClass(TbFootClass);
			if (TbFootTypeAttr)
				wf->strAttr("type", TbFootTypeAttr);
			wf->endElem(true);
			wf->putEnd();
		}
	}
	else {
		FootDiv = wf->NewDiv();
		if (!TbFootPos)
			TbFootDiv = FootDiv;
		wf->startElem(FootBlockTag);
		if (WriteClassAttributes)
			wf->setClass(FootClass);
		if (FootTypeAttr)
			wf->strAttr("type", FootTypeAttr);
		wf->endElem(true);
		wf->putEnd();
	}
	wf->SelectDiv(PreFootDiv);  // but don't use it yet
}


// index entries

bool HTMwr::UseIndex = false;
bool HTMwr::InIdxGroup = false;
bool HTMwr::StartingIdxGroup = false;
bool HTMwr::IdxSeeAlso = false;
bool HTMwr::IdxEnded = false;

bool HTMwr::IdxStartRange = false;
bool HTMwr::IdxEndRange = false;
bool HTMwr::IdxInRange = false;
DClist HTMwr::StartRangeList;
DClist HTMwr::EndRangeList;
DClist HTMwr::StartRangeIDList;
DCvlist HTMwr::IndexRangeList;
char *HTMwr::CurrIndexEntry = NULL;

bool HTMwr::IndexEntry = false;
bool HTMwr::IndexCounted = false;
bool HTMwr::InIndexSort = false;
bool HTMwr::AfterIndexColon = false;
short HTMwr::IndexCount = 0;
short HTMwr::IndexObjCount = 0;
char *HTMwr::IndexBuf = NULL;
short HTMwr::IndexPos = 0;
short HTMwr::IndexSPos = 0;
short HTMwr::IndexAllocUnit = 512;
short HTMwr::IndexAlloc = 0;
short HTMwr::KeywordLimit = 488;
unc HTMwr::KeywordRefs = 'K';
bool HTMwr::CombineIndexLevels = true;
bool HTMwr::UseCommaAsSeparator = true;
bool HTMwr::LevelBreakForSee = true;
bool HTMwr::CaseSensitiveIndexCompare = false;
char *HTMwr::IgnoreCharsIX = "-[]()<>_";
char *HTMwr::IgnoreLeadingCharsIX = ".$*";


unc HTMwr::RefFileType = 'B';
HTMctl *HTMwr::IndexRefCtl = NULL;
DCvlist HTMwr::IndexRefList;
DClist HTMwr::SeeAlsoList;

bool HTMwr::UseHVIndex = false;
DCvlist HTMwr::HVIndex;
DClist *HTMwr::CurrHVIndex = NULL;

void
HTMwr::ProcHVIndex(char *txt, long sid)
{
	// special processing for Help Viewer 1 index entries
	if (!CurrHVIndex) {
		CurrHVIndex = new DClist;
		HVIndex.add(CurrHVIndex, sid);  // one dclist per split part
	}

	// each index entry gets its own list item
	// txt may have multiple entries sep with ';'
	// may have [sort string]s, ignore them
	// may have ranges, ignore those
	// remove anything inside unescaped <>s
	// has ':' for level seps, replace with','
	// replace real commas, or more than one sep, with "%2C"
	// use UTF-8 for non-ANSI, or numeric ents &#nn; &#xhh;
  // <meta name="Microsoft.Help.Keywords" content="..."/>
	// attribute, so ents for < lt, > gt, & amp, " quot, and ' apos.
	// don't know: max length, handling of %HH other than %2C
	long len = strlen(txt);
	char *cpt = txt;
	char *ntxt = NewName(len * 5);  // allow for all ents
	char *npt = ntxt;
	char *tpt = NULL;
	unc level = 0;

	for (cpt = txt; *cpt; cpt++) {
		switch (*cpt) {
			case '\\':
				*npt = *++cpt;
				switch (*npt) {
					case '<':
						strcpy(npt, "&lt;");
						npt += 3;
						break;

					case '>':
						strcpy(npt, "&gt;");
						npt += 3;
						break;

					case '\'':
						strcpy(npt, "&apos;");
						npt += 5;
						break;

					case '"':
						strcpy(npt, "&quot;");
						npt += 5;
						break;

					case '&':
						strcpy(npt, "&amp;");
						npt += 4;
						break;

					default:
						break;
				}
				npt++;
				break;

			case '>':
				strcpy(npt, "&gt;");
				npt += 4;
				break;

			case '\'':
				strcpy(npt, "&apos;");
				npt += 6;
				break;

			case '"':
				strcpy(npt, "&quot;");
				npt += 6;
				break;

			case '&':
				strcpy(npt, "&amp;");
				npt += 5;
				break;

			case ':':
				if (level) {
					*npt++ = '%';
					*npt++ = '2';
					*npt++ = 'C';
					LogEvent(logwarn, 2, "Index level over two in: ", txt);
				}
				else {
					level++;
					*npt++ = ',';
				}
				if (*(cpt + 1) != ' ')
					*npt++ = ' ';
				break;

			case '[':
				while (*cpt && (*cpt != ']'))
					*cpt++;
				break;

			case ';':
				*npt = '\0';
				tpt = NewName(ntxt);     // completed part
				CurrHVIndex->append( (long) tpt);
				npt = ntxt;   // back to start
				level = 0;
				break;

			case ',':
				*npt++ = '%';
				*npt++ = '2';
				*npt++ = 'C';
				break;

			case '<':
				while (*cpt && (*cpt != '>'))
					*cpt++;
				break;

			default:
				*npt++ = *cpt;
				break;
		}
	}
	if (npt > ntxt) {
		*npt = '\0';
		tpt = NewName(ntxt);     // completed part
		CurrHVIndex->append( (long) tpt);
		DeleteName(ntxt);		
	}
}


void
HTMwr::WriteHVIndexItems(HTMwfile *wf, long sid)
{
	if (((CurrHVIndex = (DClist *) HVIndex.find(sid)) == NULL)
	 || !CurrHVIndex->next)
		return;

	while ((CurrHVIndex = CurrHVIndex->next) != NULL) {
		char *hv = (char *) (CurrHVIndex->id);
		if (hv) {
			wf->startElem("meta");
			wf->strAttr("name", "Microsoft.Help.Keywords");
			wf->strAttr("content", hv);
			wf->endElem(false);
			wf->putEnd();
			DeleteName(hv);
		}
	}
}



void
HTMwr::ProcIdxType(HTMctl *cp)
{
	switch (cp->siz()) {  // write phase
		case 1:  // no_page marks See Also entries
			IdxSeeAlso = true;
			//if (IndexBuf)
			//	SeeAlsoList.add((long) (IndexBuf + IndexSPos));
			break;
		case 2:  // one_page (default) returns to normal
			IdxSeeAlso = false;
			break;
		case 3:  // start range, add to CurrRanges
			if (IndexRanges)
				IdxStartRange = true;
			break;
		case 4:  // end range, delete from CurrRanges
			if (IndexRanges)
				IdxEndRange = true;
			break;
		default:
			break;
	}
}


void
HTMwr::PutIndexEntry(HTMctl *cp)
{
  char *sptr = NULL;
  char *eptr = NULL;
  char *iptr = NULL;
  char *istr = NULL;
	short len = 0;
	short i = 0;
	short count = 0;
	bool nosemi = false;
	bool ansi = true;

  if (cp->dt() == imnem) {  // single char form
		unl uch = cp->siz();
		if (uch <= 0x7F) {
			ansi = true;
			iptr = NewName((uns) 2);
			*iptr = (unc) (uch & 0x7F);
			if (*iptr != ';')
				nosemi = true;
		}
		else {
			ansi = false;
			iptr = ProcOutputText(cp);
		}
		if (!iptr)
			return;
  }
	else {	// string form, all legal chars
		ansi = true;
    if ((cp->siz() <= 1) // may be only a nul
     || (cp->ex() == NULL))
      return;
		iptr = NewName((char *) cp->ex(), cp->siz());
  }
	istr = iptr;  // save for later deletion
	len = strlen(iptr);

  if (StartingIdxGroup) {	// start of grouped entry
    IndexCounted = false;
    StartingIdxGroup = false;
		IndexSPos = IndexPos;
  }
  else if (InIdxGroup) {	// continuation of entry
    if (IndexPos  // something already written
     && IndexCounted) {// and not finished
			IndexPos--;	 // so overwrite last terminator
			if (IndexBuf[IndexPos - 1] == '\0') // and possible sort string term
				IndexPos--;
		}
	}
  else {  // start of single string entry
    IndexCounted = false;
  }


	// add up chars that will result in a size increase
	count = 0;
	for (i = 0; i < len; i++) {
		switch (iptr[i]) {
			case ':':
			case ',':
			case '\\':
				count++;
				break;
			default:
				break;
		}
	}
	len += (count * 2);

  if (IndexPos) { // adding on
    if (IndexAlloc < (IndexPos + len + 3)) {
			while (IndexAlloc < (IndexPos + len + 3))
				IndexAlloc += IndexAllocUnit;
			IndexBuf = (char *) ResizeMem(IndexBuf, IndexAlloc);
		}
  }
  else {
		while (IndexAlloc < (len + 3))
			IndexAlloc += IndexAllocUnit;
    IndexBuf = (char *) NewMem(IndexAlloc);
  }

  sptr = &IndexBuf[IndexSPos];
  eptr = &IndexBuf[IndexPos];

  while (*iptr) {  // strings are always null-terminated
		if (!ansi) {  // not ansi, pass through
      *eptr++ = *iptr++;
		}
		else if (AfterIndexColon) {
			AfterIndexColon = false;  // just for one char
			if (*iptr == ' ')         // skip it if space
				iptr++;
		}
		else if (InIndexSort) {
			if (*iptr == ']') {
				InIndexSort = false;
				*eptr++ = '\0';
        iptr++;
			}
			else
				*eptr++ = *iptr++;
		}
    else switch (*iptr) {
      case '[':  // remove synonyms **** sort strings, process later
				InIndexSort = true;
				*eptr++ = '\0';
				iptr++;
        //while (*iptr && (*iptr != ']'))
        //  *eptr++ = *iptr++;
				//*eptr++ = '\0';
        //if (*iptr)
        //  iptr++;
        break;
			case ':':  // colons are subentries, change to commas
				//if (*(eptr - 1) != ',')
				//	*eptr++ = ',';
				*eptr++ = IndexLevMark;
				*eptr++ = *iptr++; // add colon
				if (*iptr != ' ') {
					*eptr++ = ' ';
					if (!*iptr)
						AfterIndexColon = true;
				}
				break;
			case ',':
				if (UseCommaAsSeparator
				 && !isdigit(*(iptr + 1))) {
					*eptr++ = IndexLevMark;
					*eptr++ = ':';  // normalize to colon
					iptr++;         // skip comma
					if (*iptr != ' ') {
						*eptr++ = ' ';
						if (!*iptr)
							AfterIndexColon = true;
					}
				}
				else
	        *eptr++ = *iptr++;
				break;
      case '\\': // pass existing escapes through
				if (!ansi)
					goto normal;
				if (*++iptr == '\\') // keep doubled backslash
					*eptr++ = '\\';    // but don't escape other chars
        *eptr++ = *iptr++;
        break;
      case ';':	 // split up multiple entries
				if (nosemi)  // semi must be end of entity code
					goto normal;
				if (eptr > sptr) { // not empty
					if (*(eptr - 1) != '\0') { // no sort string at end
						*eptr++ = '\0';  // make empty sort string
						*eptr++ = '\0';
					}
					ProcIdxFlags(sptr);
					sptr = eptr;
					IndexPos = IndexSPos = eptr - IndexBuf;
					if (!IndexCounted)
						IndexCount++;
					else IndexCounted = false;
				}
        iptr++;
        break;
			case 130:  // single quotes
			case 145:
			case 146:
				*iptr = '\'';
				goto normal;
			case 132:  // double quote
			case 147:
			case 148:
				*iptr = '"';
				goto normal;
				break;
			case 0x99: // things to omit (TM)
				iptr++;
				break;
			case ' ':  // space
				if (eptr > sptr)
					goto normal;
				iptr++;
				break;

			normal:
      default:	 // write text
        *eptr++ = *iptr++;
        break;
    }
  }
	DeleteName(istr);

	if (eptr > sptr) {  // wrote something, not terminated
		if (InIndexSort  // sort string in progress
		 || (*(eptr - 1) != '\0')) {  // no sort string at end
			*eptr++ = '\0'; // make empty sort string
			*eptr++ = '\0';	// terminate it
		}
		ProcIdxFlags(sptr);
		IndexPos = eptr - IndexBuf;
		if (!IndexCounted) {  // first time
			IndexCounted = true; // indicate entry in progress
			IndexCount++;		// update entry count
		}
  }

	IndexSPos = sptr - IndexBuf;

  if (IndexPos)	// not empty
    IndexEntry = true;
  else {
    IndexEntry = false;
		IndexCounted = false;
    IndexCount = 0;
		IndexSPos = 0;
    DeleteMem(IndexBuf);
		SeeAlsoList.empty();
		StartRangeList.empty();
		EndRangeList.empty();
    IndexAlloc = 0;
		IndexBuf = NULL;
		InIndexSort = false;
		AfterIndexColon = false;
  }
}


void
HTMwr::ProcIdxFlags(char *ent)
{
	if (IdxSeeAlso) {
		SeeAlsoList.add((long) ent);
		IdxSeeAlso = false;
	}
	if (IdxStartRange) {
		StartRangeList.add((long) ent);
		IdxStartRange = false;
	}
	if (IdxEndRange) {
		EndRangeList.add((long) ent);
		IdxEndRange = false;
	}
}


void
HTMwr::WriteIndexEntry(HTMwfile *wf)
{
	if ((IndexBuf == NULL)
	 || (IndexPos == 0))
		return;	

  char *iptr = IndexBuf;
	short len = 0;

  for (short i = 0; i < IndexCount; i++) {
		if (((len = strlen(iptr)) == 1)
		 && (*iptr == ' ')) {   // empty
			iptr += 2;
			continue;
		}
		if (HXMwr::uDoc)
			HUDwr::PutuDocIndexEntry(wf, iptr, iptr + len + 1);
		else if (HXMwr::HXML)
			HXMwr::PutDITAIndexEntry(wf, iptr, iptr + len + 1);
		else if ((HelpType == 'M')
		 && ((RefFileType == 'H')
		 || ((ListType != 'B')
		  && (ListType != 'I'))))
			PutKeyword(wf, iptr); // write entire keyword now
		else
			SetKeyword(iptr);
    iptr += (len + 1);
		if (*iptr)  // sort string
			iptr += strlen(iptr);
		iptr++; // skip sort string terminator
  }

  IndexEntry = false;
	IndexCounted = false;
  IndexCount = 0;
	IndexPos = 0;
	IndexSPos = 0;
  DeleteMem(IndexBuf);
	SeeAlsoList.empty();
	StartRangeList.empty();
	EndRangeList.empty();
  IndexAlloc = 0;
	IndexBuf = NULL;
	//IdxEnded = true;
}


void
HTMwr::SetKeyword(char *key)
{
	// write entry for subsequent use in .hhk, preferred method
	if ((*key == '\0')
	 || !IndexRefCtl)
		return;

	if (EndRangeList.find((long)key)) {
		CurrIndexEntry = key;
		IndexRangeList.all(FindStartSetRangeKeyword);
		StartRangeIDList.all(RemoveRangeKeyword);
		StartRangeIDList.empty();
		return;  // already put out for this topic, don't duplicate
	}

	ixprop *ip = new ixprop;
	IndexRefList.add(ip, ++IndexRefCount);
	if (StartRangeList.find((long)key))
		IndexRangeList.add(ip, IndexRefCount);

	bool seealso = false;
	if (SeeAlsoList.find((long)key)) {
		seealso = true;
		ip->ref = ip->title = NULL;
	}
	else {
		ip->ref = HyperUniRefList.find((long)IndexRefCtl);
		ip->win = ContentsWindowList.find(Extracting ? ExtrID : (SplitID ? SplitID : 1));
		ip->title = SplitTitleList.find(HyperUniIdList.find((long)IndexRefCtl));
		ip->refid = ContentsID;
		if (!ip->title
		 || (*ip->title == '\0'))
			ip->title = NewName(CurrXFileTitle);
	}
	ip->name = NewName(key);
	char *sp = key + strlen(key) + 1;
	if (*sp && UseSortString) {
		char *bp = NULL;  // check for multiple placement
		while ((bp = strchr(sp, ';')) != NULL) {
			ixprop *np = new ixprop;
			IndexRefList.add(np, ++IndexRefCount);
			*np = *ip;  // shallow copy is fine
			*bp = '\0';
			np->sort = ProcIXSortString(sp, seealso, true);
			*bp = ';';
			sp = bp + 1;
		}
		ip->sort = ProcIXSortString(sp, seealso, true);
	}
	else
		ip->sort = ProcIXSortString(sp = NewName(ip->name), seealso);
}


char *
HTMwr::ProcIXSortString(char *sort, bool seealso, bool sepsort)
{
	if (!sort)
		return NULL;

	char *cvt = NewName(strlen(sort) * 2);
	char *sret = NULL;
	int pos = 0;
	int i = 0;
	int j = 0;
	bool started = false;
	long stlen = SeeTerm ? strlen(SeeTerm) : 0;
	long satlen = SeeAlsoTerm ? strlen(SeeAlsoTerm) : 0;

	if (!satlen && !stlen)  // no term, can't check for it
		seealso = false;

	for (i = 0; sort[i] != '\0'; i++) {
		if ((!started
		  && IgnoreLeadingCharsIX
		  && strchr(IgnoreLeadingCharsIX, sort[i]))
		 || (IgnoreCharsIX
		  && strchr(IgnoreCharsIX, sort[i])))
			continue;
		started = true;
		if (sepsort) {
			switch (sort[i]) {
				case ':':  // colons are subentries
					cvt[pos++] = IndexLevMark;
					cvt[pos++] = ':';
					if (sort[i + 1] != ' ')
						cvt[pos++] = ' ';
					break;
				case ',':
					if (UseCommaAsSeparator
					 && !isdigit(sort[i + 1])) {
						cvt[pos++] = IndexLevMark;
						cvt[pos++] = ',';
						if (sort[i + 1] != ' ')
							cvt[pos++] = ' ';
					}
					else
						cvt[pos++] = ',';
					break;
				case '\\': // pass existing escapes through
					if (sort[++i] == '\\') // keep doubled backslash
						cvt[pos++] = '\\';    // but don't escape other chars
					cvt[pos++] = sort[i];
					break;
				default:
					break;
			}
		}
		if (seealso
		 && ((satlen && !_strnicmp(SeeAlsoTerm, &sort[i], satlen)) // See also
		  || (stlen && !_strnicmp(SeeTerm, &sort[i], stlen)))) {  // or See
			seealso = false;  // just do once

			// put lev mark and punct before See
			for (j = pos - 1; j > 0; j--) { // start before insert point
				if (isspace(cvt[j])) // remove any spaces
					pos--;
				else if (ispunct(cvt[j])) {  // normally period or comma
					if (cvt[j - 1] != IndexLevMark) {  // ensure lev sep here
						cvt[j + 1] = cvt[j];
						cvt[j++] = IndexLevMark;
						pos++;
					}
					if (cvt[j] == ',')  // change comma to semi for see
						cvt[j] = ';';
					break;
				}
				else if (isgraph(cvt[j])) {  // found word, add lev sep
					j++;  // move to char after word
					cvt[j++] = IndexLevMark;
					cvt[j] = ' ';  // indicates no punct
					pos += 2;
					break;
				}
			}

			if (SortSeeAlsoFirst)  // ensure "See also" sorts first or last
				cvt[pos++] = ' ';  // space sorts first
			else
				cvt[pos++] = '~';  // tilde sorts last

			cvt[pos++] = ' '; // insert space before See
		}
		cvt[pos++] = sort[i];
	}
	if (cvt[pos - 2] != IndexLevMark) {
		cvt[pos++] = IndexLevMark;
		cvt[pos++] = ' ';
	}
	cvt[pos] = '\0';

	sret = NewName(cvt);
	DeleteName(cvt);
	return sret;
}


void
HTMwr::ResetRangeKeyword(void *vip, long i)
{
	// called from SplitFile() as IndexRangeList.all(ResetRangeKeywordSp);
	if ((i == 0)
	 || (vip == NULL))
		return;

	ixprop *ip = (ixprop *) vip;
	ixprop *nip = new ixprop;

	*nip = *ip;  // start with shallow copy

	// replace ref and title with those for current split file
	nip->ref = NewName(SplitRefList.find(SplitID));
	nip->title = NewName(SplitTitleList.find(SplitID));
	nip->refid = ContentsID;

	ProcIntRef(nip->ref + 1, SplitID);

	IndexRefList.add(nip, ++IndexRefCount);  // add back to list
}


void
HTMwr::FindStartSetRangeKeyword(void *vip, long i)
{
	if ((i == 0)
	 || (vip == NULL))
		return;

	if (!strcmp(CurrIndexEntry, ((ixprop *) vip)->name))  // exact name match
		StartRangeIDList.add(i);
}


void
HTMwr::PutRangeKeyword(DCfile *wf, void *vip)
{
	// called from SplitFile() and StartFileExtract() as IndexRangeList.writeall(PutRangeKeyword);
	if (vip == NULL)
		return;

	PutKeyword((HTMwfile *) wf, (char *) vip, true);
}


void
HTMwr::FindStartPutRangeKeyword(void *vip, long i)
{
	if ((i == 0)
	 || (vip == NULL))
		return;

	if (!strcmp(CurrIndexEntry, (char *) vip))  // exact name match
		StartRangeIDList.add(i);
}


void
HTMwr::RemoveRangeKeyword(long i)
{
	if (i == 0)
		return;

	IndexRangeList.remove(i);
}


void
HTMwr::PutKeyword(HTMwfile *wf, char *key, bool range)
{
	char *subptr = NULL;
	char *topptr = NULL;
	long toplen = 0;

	if (!range) { // skip when placed from SplitFile()
		if (EndRangeList.find((long)key)) {
			CurrIndexEntry = key;
			IndexRangeList.all(FindStartPutRangeKeyword);
			StartRangeIDList.all(RemoveRangeKeyword);
			StartRangeIDList.empty();
			return;  // already put out for this topic, don't duplicate
		}

		if (StartRangeList.find((long)key))
			IndexRangeList.add(NewName(key), ++IndexObjCount);
	}

	// **** needs See Also processing (though this branch is deprecated)
	// if (SeeAlsoList.find((long)key))

	if (((short) strlen(key)) > KeywordLimit)
		key[KeywordLimit] = '\0';	// trim it

// <Object type="application/x-oleobject"
// classid="clsid:1e2a7bd0-dab9-11d0-b93a-00c04fc99f9e">
//	<param name="Keyword" value="drives">
//	<param name="Keyword" value="drives, checking compatibility of tape
// drives">
//	<param name="Keyword" value="compatibility of tape drive, checking">
//	<param name="Keyword" value="tape drives">
//	<param name="Keyword" value="tape drives, checking compatibility of">
// </OBJECT>

	CurrentDiv = wf->GetDiv();
	wf->SelectDiv(Extracting ? ExtrObjectDiv : ObjectDiv);

	wf->startBlock("object");
	wf->putEnd();
	wf->strAttr("type", "application/x-oleobject");
	wf->strAttr("classid", "clsid:1e2a7bd0-dab9-11d0-b93a-00c04fc99f9e");
	wf->endElem();
	wf->putEnd();

	// we should coalesce these by writing params to a keydiv
	// then writing the object above, the keydiv, and the ending
  // when we PutObjects() at the end of each file
	// check on limit to number of params... and of objects

	if ((subptr = strchr(key, ',')) != NULL) { // always write level 1
		toplen = (subptr - key);   // has a level 1 key
		topptr = NewName(key, toplen); // exclude key terminator
		WriteParam(wf, "keyword", topptr);
		DeleteName(topptr);
	}

	WriteParam(wf, "keyword", key);

	wf->putBlockEnd("object");

	wf->SelectDiv(CurrentDiv);
}


// end of dwhtman.cpp


