 
// dwrtfgr.cc is the code for dcl Microsoft RTF graphic writing, JHG
// Copyright (c) 1993-1998 by Omni Systems, Inc.  All rights reserved.

#include "dcappl.h"
#include "dcctlg.h"
#include "dcfili.h"
#include "dctxfx.h"
#include "dcfmtr.h"
#include "dcfilp.h"
#include "dcini.h"
#include "dcmacr.h"
#include "dcauto.h"
#include "dwrtf.h"
#include "dwrtffl.h"

#include <windows.h>

#pragma warning ( disable : 4237 )


// processing switch


bool
RTFwr::ProcGr(RTFctl *cp)
{
  switch((cp->rp)->scan_op) {
    case nothing:   // just put with its group for later
      return true;

    case pass:      // ignore, don't save
      break;

    case pgdef:
      ProcPage(cp);
      break;
    case pgend:
      ProcPgEnd(cp);
      break;

    case pgattr:
      ProcPgAttr(cp);
      break;
    case pgsize:
      ProcPgSize(cp);
      break;

    case frdef:
      ProcFrame(cp);
      break;
		case frname:
			ProcFrameName(cp);
			break;
    case fredef:
      EndFrame(cp);
      break;

    case franch:
      Frame->fanchor = cp->dat();
      Frame->fapage = (uns) cp->siz();
      break;

    case frattr:
			return ProcFrAttr(cp);
      break;
    case frsize:
      ProcFrSize(cp);
      break;

    case frline:
      switch (cp->dat()) {
        case 2: // color
          GetColor(cp->siz(), &Frame->fcolr);
          break;
        case 3: // thickness
          Frame->fthck = GetTwips(cp->siz(), gl_unit);
          break;
      }
			// if (FrameLevel > 1)
			ProcGrLine(cp);
      break;
    case frpen:
      if (cp->dat() == 1)
        Frame->frvis = cp->siz() ? 1 : 0;
			//if (FrameLevel > 1) 
			ProcGrPen(cp);
      break;
    case frfill:
      if (cp->dat() == 1)
        GetFill(cp, &Frame->ffill);
			// if (FrameLevel > 1)
			ProcGrFill(cp);
      break;

    case frtext:
      Frame->ftext = cp->dat();
      Frame->ftnext = (uns) cp->siz();
      break;

    case ruleprop:  // line color and weight, but not in rule def
    case grline:
      ProcGrLine(cp);
      break;
    case grpen:
      ProcGrPen(cp);
      break;
    case grfill:
      ProcGrFill(cp);
      break;
		case grarrhd:
			Graphic->grhead = (uns) cp->siz();
			break;
		case grarrtl:
			Graphic->grtail = (uns) cp->siz();
			break;

    case setanch:  // anchor in text for frame
      ProcFrAnchor(cp);
      return true;

    case grgroup:
      if (GrGroupLevel < GrGroupMax)
        GrGroupStack[GrGroupLevel] = cp->dat();
      GrGroupLevel++;
      break;
    case gregroup:
      if (GrGroupLevel)
        GrGroupLevel--;
      break;

    case grobj:
      ProcGraphic(cp);
      break;
    case greobj:
      EndGraphic(cp);
      break;

    case grshape:
      ProcGrShape(cp);
      break;

    case grtext:
      ProcGrText(cp);
      break;

		case formscan:
			ProcGrFormat(cp);
			break;

		case fontlist:
    case formchk:
      ProcGrTProp(cp);
      break;

    case flowused:
      ProcGrTStr(cp);
      break;

		case eqtype:   // includes starting pt and angle of equation
      break;
		case eqsize:   // size of frame for equation itself
      break;
		case eqalgn:   // alignment of equation in its frame
      break;
		case eqtxsiz:  // relative size of equation
      break;
		case eqlbrk:   // line break value (constant, not used)
      break;
		case eqtext:   // MathFullForm description
      break;
		case eqgfile:  // graphic of frame with math obj in it
			RFrame->grfile = (char *) cp->ex();
			RFrame->eqtype = 2;
      break;

		case frprev:  // graphic of full frame
			RFrame->grfile = (char *) cp->ex();
			RFrame->prtype = cp->dat();
      break;

		case colordef:
			ProcColorDef(cp->siz(), cp->dat());
			break;

		case arrdef:
			ProcArrowDef(cp);
			break;

		case grfile:
			RFrame->grfile = (char *) cp->ex();
			RFrame->dpi = BitmapDPI;
			break;

    default:
      break;
  }
  return false;  // default is to discard ctl after scan
}



// page properties


uns RTFwr::PaperWide = 12240;
uns RTFwr::PaperHigh = 15840;
uns RTFwr::LMarg = 1440;
uns RTFwr::RMarg = 1440;
uns RTFwr::TMarg = 1440;
uns RTFwr::BMarg = 1440;
uns RTFwr::Gutter = 0;
uns RTFwr::Margins = 2880;
uns RTFwr::BodyWide = 9360;
uns RTFwr::BodyHigh = 12960;

page *RTFwr::Page;
DCvlist RTFwr::PageList;
uns RTFwr::LastPage = 0;
uns RTFwr::LastMasterPage = 0;

uns RTFwr::LeftMaster = 0;
uns RTFwr::RightMaster = 0;
uns RTFwr::FirstMaster = 0;
uns RTFwr::NextFirstMaster = 0;

bool RTFwr::StartingPage = true;
bool RTFwr::StartingPageBody = true;
bool RTFwr::TwoSided = false;
bool RTFwr::FirstRight = true;
bool RTFwr::DifferentFirst = false;
uns RTFwr::FirstPageSpaceAbove = 0;
bool RTFwr::TitlePage = false;
bool RTFwr::MargSet = false;
bool RTFwr::MirrorMarg = false;

bool RTFwr::NewBPage;
uns RTFwr::LastBPage;
uns RTFwr::PFrameCount;
bool RTFwr::BodyFound = false;


void
RTFwr::ProcPage(RTFctl *cp)
{
  Page = new page;
  Page->ptype = 0;
  Page->plmrg = Page->prmrg = Page->ptmrg = Page->pbmrg = 1440;
  LastPage = Page->pnum = cp->dat();
  PageList.add(Page, Page->pnum);
  StartingPage = true;
  StartingPageBody = true;
	ParaSpace = BodyParaSpace = 0;
  NewBPage = false;
  PFrameCount = 0;
}


void
RTFwr::ProcPgEnd(RTFctl *cp)
{
	BodyFound = false;
  if (PFrameCount == 1) {
    Page->pcols = 1;
    //ProcPgBody(BodyFrameList.find(LastPage));
    BodyFrameList.execu(LastPage, ProcPgBody);  // first column
  }
  else if (PFrameCount >= 2) {
    Page->pcols = 0;
    //ProcPgBody(BodyFrameList.find(LastPage));   // first column
    BodyFrameList.execu(LastPage, ProcPgBody);  // first column
    BodyFrameList.execu(LastPage, ProcPgCols);  // other columns
  }

	if (TwoSided) {
		if (LeftMaster && RightMaster && !MargSet) {
			page *lp = (page *) PageList.find(LeftMaster);
			page *rp = (page *) PageList.find(RightMaster);
			TMarg = rp->ptmrg;
			BMarg = rp->pbmrg;
			Gutter = 0;
			if (NearEqual(lp->plmrg, rp->plmrg)) {
				LMarg = rp->plmrg;
				RMarg = rp->prmrg;
			}
			else if ((NearEqual(lp->plmrg, rp->prmrg))
			 && (NearEqual(lp->prmrg, rp->plmrg))
			 && ((SideHType != 'F')
			  || SideHFlip)) {
				MirrorMarg = true;
				LMarg = rp->plmrg;
				RMarg = rp->prmrg;
			}
			else if (lp->plmrg < rp->plmrg) {
				LMarg = lp->plmrg;
				RMarg = rp->prmrg;
				Gutter = rp->plmrg - lp->plmrg;
			}
			else {
				LMarg = rp->plmrg;
				RMarg = rp->prmrg;
			}
			Margins = LMarg + RMarg + Gutter;
			MargSet = true;
			if (!DefCols)
				DefCols = rp->pcols;
			if (!ColGap)
				ColGap = rp->pcgap;
		}
	}
	else if (RightMaster == LastPage) {
		LMarg = Page->plmrg;
		RMarg = Page->prmrg;
		TMarg = Page->ptmrg;
		BMarg = Page->pbmrg;
		Gutter = 0;
		Margins = LMarg + RMarg;
		if (!DefCols)
			DefCols = Page->pcols;
		if (!ColGap)
			ColGap = Page->pcgap;
	}
	BodyWide = PaperWide - Margins;
	BodyHigh = PaperHigh - (TMarg + BMarg);

	// handle any graphics on master page
	frame *fr = NULL;

	if (HeadGraphFrame.grcount > 0) {
		if ((fr = (frame *) HeaderList.find(LastPage)) == NULL) {
			fr = new frame;	 // need header to anchor graphics, make one
			frame *hfr = NULL;
			if ((hfr = (frame *) HeaderList.find(RightMaster)) != NULL)
				*fr = *hfr;	 // borrow right header if present
			else {
				fr->xpos = LMarg;
				fr->ypos = TMarg - 200;
				fr->high = 200;
				fr->wide = BodyWide;
			}
			fr->ftext = 0;
			HeaderList.add(fr, LastPage);
		}
		fr->grcount = HeadGraphFrame.grcount;
		fr->grlist = HeadGraphFrame.grlist;
		HeadGraphFrame.grcount = 0;
		HeadGraphFrame.grlist = NULL;
	}

	if (FootGraphFrame.grcount > 0) {
		if ((fr = (frame *) FooterList.find(LastPage)) == NULL) {
			fr = new frame;
			frame *ffr = NULL;
			if ((ffr = (frame *) FooterList.find(RightMaster)) != NULL)
				*fr = *ffr;  // borrow right footer if present
			else {
				fr->xpos = LMarg;
				fr->ypos = PaperHigh - BMarg;
				fr->high = 200;
				fr->wide = BodyWide;
			}
			fr->ftext = 0;
			FooterList.add(fr, LastPage);
		}
		fr->grcount = FootGraphFrame.grcount;
		fr->grlist = FootGraphFrame.grlist;
		FootGraphFrame.grcount = 0;
		FootGraphFrame.grlist = NULL;
	}

  while (FrameLevel) {
    FrameLevel--;
    FrameStack[FrameLevel] = NULL;
  }
}


bool
RTFwr::NearEqual(uns a, uns b, uns diff)
{
	if (a == b)
		return true;

	if (a > b)
		return ((a - b) < diff) ? true : false;

	return ((b - a) < diff) ? true : false;
}


void
RTFwr::ProcPgBody(void *vp)
{
	if (BodyFound)
		return;
  frame *fr = (frame *) vp;
	if (FrameSkipList.find(FrameList(fr)))
		return;

	BodyFound = true;
  Page->plmrg = fr->xpos;
  Page->prmrg = Page->pwide - (fr->xpos + fr->wide);
  Page->ptmrg = fr->ypos;
  Page->pbmrg = Page->phigh - (fr->ypos + fr->high);
	if (fr->cols > 1) {
		Page->pcols = fr->cols;
		Page->pcgap = fr->colgap;
	}
}


void
RTFwr::ProcPgCols(void *vp)
{
  frame *fr = (frame *) vp;
	if (FrameSkipList.find(FrameList(fr)))
		return;

  Page->pcols++;

  if (fr->xpos < Page->plmrg) {  // more left
    if ((fr->xpos + fr->wide) < Page->plmrg)
      Page->gutter = Page->plmrg - (fr->xpos + fr->wide);
    Page->plmrg = fr->xpos;
  }
  else if ((fr->xpos + fr->wide) > (Page->pwide - Page->prmrg)) { // more right
    if (fr->xpos > (Page->pwide - Page->prmrg))
      Page->gutter = (Page->pwide - Page->prmrg) - fr->xpos;
    Page->prmrg = Page->pwide - (fr->xpos + fr->wide);
  }

  if (fr->ypos < Page->ptmrg) {  // more above
    Page->ptmrg = fr->ypos;
  }
  else if ((fr->ypos + fr->high) > (Page->phigh - Page->pbmrg)) { // more below
    Page->pbmrg = Page->phigh - (fr->ypos + fr->high);
  }
}


void
RTFwr::ProcPgAttr(RTFctl *cp)
{
  switch (cp->dat()) {
    case 2: // page usage
      Page->pusage = (uns) cp->siz();
      break;
    case 3: // master type
      if ((Page->ptype = (uns) cp->siz()) > 0)
        LastMasterPage = LastPage;
      break;
    case 4: // master ID
      if (LastBPage != (Page->pback = (uns) cp->siz())) // every pg if doublesided
        NewBPage = true;  // implies new section, different headers/footers
      LastBPage = Page->pback;
			if ((Page->ptype == 0)
			 && (Page->pback != LeftMaster)
			 && (Page->pback != RightMaster)) {
				if ((FirstMaster == 0)
				 && (LastPage == LastMasterPage + 1)) { // really first body page
					FirstMaster = Page->pback;
					DifferentFirst = true;
				}
				else if (NewBPage)
					NextFirstMaster = Page->pback;
			}
      break;
    case 5: // orient
      Page->porient = (uns) cp->siz();
      break;
    case 6: // hand
      Page->phand = (uns) cp->siz();
			if (Page->ptype == 1) {
				if (Page->phand == 1)
					RightMaster = Page->pnum;
				else if (Page->phand == 2)
					LeftMaster = Page->pnum;
			}
      break;
		case 9: // sided (for doc)
			TwoSided = (cp->siz() == 0) ? false : true;
			break;
		case 10: // starting side (for doc)
			FirstRight = (cp->siz() == 0) ? false : true;
			break;
    default:
      break;
  }
}


void
RTFwr::ProcPgSize(RTFctl *cp)
{
  long *lptr = (long *) cp->ex();
  if (Page) {
    Page->pwide = GetTwips(lptr[0], pg_unit);
    Page->phigh = GetTwips(lptr[1], pg_unit);
  }
  else {
    PaperWide = GetTwips(lptr[0], pg_unit);
    PaperHigh = GetTwips(lptr[1], pg_unit);
  }
}



// frame properties


uns RTFwr::DefCols = 0;
uns RTFwr::ColGap = 0;
bool RTFwr::ColLine = false;
bool RTFwr::NoLineNumbers = true;

dcframe *RTFwr::Frame;
frame *RTFwr::RFrame;
DCvlist RTFwr::FrameList;
DCvlist RTFwr::FramePageList;
DCnlist RTFwr::FrameNameList;
uns RTFwr::LastFrame;
DCvlist RTFwr::HeaderList;
DCvlist RTFwr::FooterList;
DCvlist RTFwr::MPTextList;
frame RTFwr::HeadGraphFrame;
frame RTFwr::FootGraphFrame;
DClist RTFwr::DocBodyList;
DCvlist RTFwr::BodyFrameList;
frame *RTFwr::FrameStack[FrameStackMax];
uns RTFwr::FrameLevel;
bool RTFwr::FrameStarting = false;


void
RTFwr::ProcFrame(RTFctl *cp)
{
  if (FrameLevel
   && (FrameStack[FrameLevel - 1] == NULL))
    ProcEndFrame();
  FrameLevel++;
  Frame = new dcframe;
  RFrame = new frame;
  LastFrame = Frame->fnum = cp->dat();
  Frame->fpage = LastPage;

	if (FrameLevel == 1)
		InitFrame(RFrame);
	FrameStarting = true;

	SideHWide = 0;
	SideHGap = 0;
	SideHPos = 0;
	SideHFlip = false;

  FrameList.add(RFrame, Frame->fnum);
  //InitFrame(RFrame);
}


void
RTFwr::ProcFrameName(RTFctl *cp)
{
	FrameNameList.add((char *) cp->ex(), LastFrame);
}


void
RTFwr::InitFrame(frame *fr)
{
  CurrPen = DefaultPen;
  CurrBrush = DefaultBrush;
	CurrFont = DefaultFont;
}


void
RTFwr::EndFrame(RTFctl *cp)
{
  if (FrameStack[FrameLevel - 1] == NULL)
    ProcEndFrame();
  FrameLevel--;
  FrameStack[FrameLevel] = NULL;
	if (FrameLevel == 0) {
		InGraphic = false;
		GrGroupLevel = 0;
	}
}


void
RTFwr::ProcEndFrame(void)
{
  frame *fr = NULL;
	bool mp_graph = false;

  FrameStack[FrameLevel - 1] = RFrame;
  if (!Frame->fapage) {
		if (FrameLevel > 1) {  // nested
	    fr = FrameStack[FrameLevel - 2];
			ProcNestedFrame(fr, RFrame);
		}
		else if ((LastPage == LastMasterPage)
		 && (Page->ptype <= 2)
		 && (HFType != 'N')
		 && (HFType != 'T')
		 && (Frame->fusage == 0)
		 && (Frame->ftext == 0)) { // MP frame with graphics
			// choose frame to fasten to, header or footer
			if (RFrame->pypos <= (PaperHigh / 2))
				fr = &HeadGraphFrame;
			else
				fr = &FootGraphFrame;
			ProcNestedFrame(fr, RFrame);
			fr = NULL;  // not nested
			mp_graph = true;
		}
	}
	
  if (Frame->frvis) {
    RFrame->frul[0] = (Frame->fthck > 52) ? 4 : 2;
    if ((Frame->fthck > 52) && (RFrame->frul[0] == 4))
      RFrame->frul[1] = 4;
    else if (Frame->fthck > 37)
      RFrame->frul[1] = 3;
    else if ((Frame->fthck > 22) || (RFrame->frul[0] == 4))
      RFrame->frul[1] = 2;
    else RFrame->frul[1] = 1;
    if ((Frame->fcolr > 1) && (Frame->fcolr != Invisible))
      RFrame->frul[1] += Frame->fcolr << 4;
    RFrame->frulsp = 20;  // use 1-pt default for margins within border 
  }

  *(RFrame->frpen = new wmf_pen) = CurrPen;
  *(RFrame->frbrush = new wmf_brush) = CurrBrush;

  RFrame->ftext = Frame->ftext;
  RFrame->wide = Frame->wide;
  RFrame->high = Frame->high;
  RFrame->xpos = Frame->xpos;
  RFrame->ypos = Frame->ypos;
	RFrame->frot = Frame->frot;
	RFrame->cols = Frame->cols;
	RFrame->colgap = Frame->colgap;
	RFrame->ffill = Frame->ffill;

  if (fr && (fr->fpos != 100)) { // nested frame
    RFrame->fpos = 0;
    RFrame->halgn = 0;
    RFrame->valgn = fr->valgn;
    RFrame->href = 1;
    RFrame->vref = fr->vref;

		if (RFrame->pxpos == 0)
			RFrame->pxpos = RFrame->xpos + fr->pxpos;
		if (RFrame->pypos == 0)
			RFrame->pypos = RFrame->ypos + fr->pypos;

		// problematic with new way of processing nested frames
		//if (FrameLevel > 2) {
		//	RFrame->xpos += fr->xpos;
		//	RFrame->ypos += fr->ypos;
    //}

    RFrame->xspace = fr->xspace;

	  if (Frame->ftext
     && (Frame->fusage == 0)
		 && SideHPos)
			SetFrameSideheadProps();

    return;
  }

  RFrame->xspace = Frame->xspace;

  if (Frame->ftext) {
    switch (Frame->fusage) {
			case 6:  // dita map
      case 1:  // body text frames
        DocBodyList.add(Frame->ftext);
				if (!SectList(CurrFlow))
					SectList.add(CurrFlow, ++SectNum);

        BodyFrameList.add(RFrame, LastPage);
        PFrameCount++;
				SetFrameSideheadProps();
        break;
      case 2:
				if (HFType == 'N')
					break;
        HeaderList.add(RFrame, Page->pnum);
        break;
      case 3:
				if (HFType == 'N')
					break;
        FooterList.add(RFrame, Page->pnum);
        break;
			case 0: // graphic text frames need sidehead info too
				SetFrameSideheadProps();
				// fall through
      default:  // check for master-page text
				if ((HFType == 'N')
				 || (LastPage != LastMasterPage)
				 || (Page->ptype > 2))
					break;
				MPTextList.add(RFrame, Page->pnum);
        break;
    }
  }

  RFrame->halgn = (Frame->halgn < 4) ? Frame->halgn :
                 ((Frame->halgn > 4) ? (Frame->halgn - 1) : 0);
  RFrame->valgn = (Frame->valgn < 4) ? Frame->valgn : 0;

  switch (Frame->flim) {
    case 0: // no constraints
      RFrame->href = RFrame->vref = 1;
      break;
    case 1: // in hor
      RFrame->href = 3;
      RFrame->vref = 1;
      break;
    case 2: // in vert
      RFrame->href = 1;
      RFrame->vref = 2;
      break;
    case 3: // in both
      RFrame->href = 3;
			RFrame->vref = 2;
      break;
    default:
      break;
  }

  switch (Frame->refpt) {
    case 3:  // page
      RFrame->fpos = (RFrame->valgn == 1) ? 2 : 3;
      break;
    case 4:  // frame or col
      RFrame->fpos = 3;
      RFrame->href = 3;
			RFrame->vref = 2;
      break;
    case 2:  // para start
      RFrame->fpos = 2;
      RFrame->href = 1;
      RFrame->valgn = 4;
      if (Frame->xoff > 0)
        RFrame->xspace = Frame->xoff;
      break;
    case 5:  // col edge
      RFrame->fpos = 2;
			if (RFrame->href != 1) {
				RFrame->href = 3;
				RFrame->xpos = -Frame->xpos;
			}
			else {
				RFrame->xspace = Frame->xoff;
				RFrame->xpos = RFrame->pxpos;
			}
			if (Frame->fpos != 4)
				RFrame->halgn = 0;
			RFrame->vref = 3;
			RFrame->valgn = 0;
			RFrame->ypos = -(Frame->yoff + Frame->high);
      break;
    case 1:  // text anchor
      RFrame->fpos = 0;
      break;
    default:
      break;
  }

  switch (Frame->fpos) {
    case 1:  // at anchor (inline)
      RFrame->fpos = 0;
      break;
		case 4:  // run-in
			RFrame->fwrap = 1;  // prevent nowrap
			if (RunInFrames == 'E') {  // end, treat as next
				RFrame->fpos = 1;
	      RFrame->vref = 0;
		    RFrame->valgn = 4;
			}
			else if ((RunInFrames == 'N')  // at anchor
			 && HelpForm)     // use in help files only
				RFrame->fpos = 0;
			else {              // otherwise put at start
				RFrame->fpos = 2;
				RFrame->href = 3;
				RFrame->vref = 3;
				RFrame->valgn = 4;
			}
      break;
    case 2:  // following anchor (next)
      RFrame->fpos = 1;    // place after para in frame
      RFrame->vref = 0;
      RFrame->valgn = 4;
      break;
    case 3:  // float
    default:
      break;
  }

  if (!Frame->fapage   // unanchored, process at start of first para on page
   && !Frame->ftext
	 && !mp_graph) {
    RFrame->fpos = 100;
    FramePageList.add(RFrame, LastPage);
  }
  // what if no para start on a page with unanchored frames???

#if 0
  delete Frame;
#endif
}


void
RTFwr::SetFrameSideheadProps()
{
	shprop *shp = new shprop;
	shp->shwid = SideHWide;
	shp->shgap = SideHGap;
	shp->shpos = SideHPos;
	shp->shflip = SideHFlip;
	SideHFrameList.add(shp, Frame->fnum);
	if (SideHTFlowList.find(Frame->ftext) == NULL)
		SideHTFlowList.add(shp, Frame->ftext);
}


void
RTFwr::ProcNestedFrame(frame *fr, frame *nf)
{
	graph *gr = new graph;
	gr->gtype = 0;
	gr->gsize = 1; // to prevent removal as empty
  gr->gparm	= (short *) nf;

	fr->grcount++;
  if (!fr->grlist)
    fr->grlist = new DCvlist;
  (fr->grlist)->add(gr, fr->grcount);

	fr->clip = (ClipText == false) ? 0 : 1;
	*(gr->gpen = new wmf_pen) = CurrPen;
	*(gr->gbrush = new wmf_brush) = CurrBrush;
}


bool
RTFwr::ProcFrAttr(RTFctl *cp)
{
  switch (cp->dat()) {
    case 2: // body, header, footer, border, label
      Frame->fusage = (uns) cp->siz();
      break;
    case 3: // master type, 1 = content, 2 = frame only
      Frame->fmtype = (uns) cp->siz();
      break;
    case 4:  // shared master ID
			if (InFormPx)	{
				Curr.pxframe = (uns) cp->siz();
				return (InFormDef) ? false : true;
			}
			else if (InFormSx) {
				Curr.sxframe = (uns) cp->siz();
				return (InFormDef) ? false : true;
			}
			else
	      Frame->fshare = (uns) cp->siz();
      break;
    case 5:  // table frame usage
      Frame->ftbl = (uns) cp->siz();
      break;
    case 6:  // horizontal alignment, l, c, r, full, inner, outer
      Frame->halgn = (uns) cp->siz();
      break;
    case 7:  // vertical alignment, t, c, b, full
      Frame->valgn = (uns) cp->siz();
      break;
    case 8:  // foreground or background usage
      Frame->fzpos = (uns) cp->siz();
      break;
    case 9:  // pos = inline, next (following anchor), or float
      if ((Frame->fpos = (uns) cp->siz()) > 4)
        Frame->fpos = 0;
      break;
    case 10:  // autosizing
      Frame->fauto = (uns) cp->siz();
      break;
    case 11:  // cropping
      Frame->fcrop = (uns) cp->siz();
      break;
    case 12: // within col_hor, within col_vert, within col (both)
      Frame->flim = (uns) cp->siz();
      break;
    case 13: // rotation
      Frame->frot = (uns) cp->siz();
      break;
    case 14: // gap
      Frame->xspace = (uns) GetTwips(cp->siz(), pg_unit);
      break;
    case 15: // columns
      Frame->cols = (uns) cp->siz();
      break;
    case 16: // column gap
      Frame->colgap = (uns) GetTwips(cp->siz(), pg_unit);
      break;
    default:
      break;
  }
	return false;  // do not save
}


void
RTFwr::ProcFrSize(RTFctl *cp)
{
  if (Frame->refpt == 0)
    Frame->refpt = cp->dat();
  long *lptr = (long *) cp->ex();
  uns lcount = (uns) (cp->siz() / sizeof(long));
  Frame->fshape = (lcount > 5) ? 2 : ((lcount == 5) ? 1 : 0);
  switch (lcount) {
    case 5:  // ellipse, xy of center, xy of major/minor diameters, rot angle
      Frame->rang = (uns) lptr[4];
    case 4:  // rect, xy of top left, xy of width and height
      Frame->high = GetTwips(lptr[3], pg_unit);
    case 3:  // size unknown, xy of top left, x of wid (0 if unknown)
      Frame->wide = GetTwips(lptr[2], pg_unit);
      Frame->ypos = GetTwips(lptr[1], pg_unit);
      Frame->xpos = GetTwips(lptr[0], pg_unit);
      break;
    case 2:  // column offset (x to nearest, y to bottom)
      Frame->yoff = GetTwips(lptr[1], pg_unit);
    case 1:  // anchor to bottom offset
      Frame->xoff = GetTwips(lptr[0], pg_unit);
    case 0:  // at reference
      break;
    default: // polygon, xy of vertices
			Frame->vcount = lcount / 2;
			Frame->vertx = new short[lcount];
			for (uns i = 0; i < lcount; i++)
					Frame->vertx[i] = GetTwips(lptr[i], pg_unit);
      break;
  }
	if (cp->dat() == 3) { // page reference
		RFrame->pxpos = Frame->xpos;
		RFrame->pypos = Frame->ypos;
	}
}



// frame writing


char *RTFwr::FrameHRef[FrameHRMax] = {
  "phpg", "phmrg", "phcol"
};

char *RTFwr::FrameHAlgn[FrameHAMax] = {
  "posnegx", "posxl", "posxc", "posxr", "posxi", "posxo"
};

char *RTFwr::FrameVRef[FrameVRMax] = {
  "pvpg", "pvmrg", "pvpara"
};

char *RTFwr::FrameVAlgn[FrameVAMax] = {
  "posnegy", "posyt", "posyc", "posyb", "posyil"
};

DClist *RTFwr::FramePendList;
bool RTFwr::WritingFrame = false;
bool RTFwr::InTextFrame = false;
bool RTFwr::FrameStyleUsed = false;
bool RTFwr::WritingFrameAsText = false;


void
RTFwr::WriteFrameCtl(RTFwfile *wf, RTFctl *cp)
{
	frame *fr = (frame *) FrameList.find(cp->dat());

	if (fr != NULL)
		WriteFrame(wf, fr);
}


void
RTFwr::WriteFrame(RTFwfile *wf, frame *fr)
{
	if (!fr)
		return;

	if ((EmpFrType == 'U')  // underline, hack for empty frames
   && !fr->ftext          //  used as underlines in textbook
	 && !fr->eqtype
	 && !fr->prtype
   && !fr->grcount) {
		WriteText(wf);
		wf->putEnd();
		wf->putStr("____________");
		wf->putEnd();
		return;
	}

	if (RemoveGraphics
	 || (WritingFrame
	  && !InTextFrame))
		return;

  if ((fr->fpos >= 1)
	 && (fr->fpos != 2)
	 && (InPara || ParaStarting || ParaStartFrames)
	 && !WritingHFGraph
	 // && !InTbCell
	 ) {  // postpone to end of para
		if (!InPara)
			WriteText(wf);
    if (FramePendList == NULL)
      FramePendList = new DClist;
    FramePendList->add( (long) fr);
    return;
  }

	if ((fr->fpos == 2)
	 && (ParaStartFrames == false))
		return; // already written, this is original ref

  if (!fr->ftext
	 && !fr->eqtype
	 && !fr->prtype
   && !fr->grcount
	 && (EmpFrType == 'R'))  // remove empty frames
		 return;

	if (!InHelpTitlePara)
		TopicEmpty = false;

	graph *gr = NULL;
	frame *gfr = NULL;
	RTFgroup *gp = NULL;

	if ((fr->grcount == 1)   // single item in frame
	 && ((gr = (graph *) ((fr->grlist)->find(1))) != NULL)
	 && (gr->gtype == 0)     // the item is a frame itself
	 && ((gfr = (frame *) gr->gparm) != NULL)
	 && (gfr->ftext != 0)    // the only frame is a text frame
	 && (GrTxType == 'T')    // and it's to be handled as text
	 && ((gp = (RTFgroup *) FlowList.find(gfr->ftext)) != NULL)) {
		WritingFrameAsText = true;
		WriteFrameText(wf, fr, gp);
		WritingFrameAsText = false;
		return;
	}

	if (RemoveNontextFrames)
		return;

	if (ParaStarting
	 && (fr->fpos != 2)
	 && !InPara)
		WriteText(wf);

	if (fr->fpos != 0)
		WritingFrame = true;

  wf->putEnd();

	if (InTbCell) {
		if (fr->fpos) {
			long pnum = 0;
			pprop *pp = NULL;
			wf->putCW("par");
			wf->putEnd();
			if (FrameStyle
			 && *FrameStyle
			 && ((pnum = FormNameList(FrameStyle)) != 0)
			 && ((pp = (pprop *) FormList.find(pnum)) != NULL)) {
				FrameStyleUsed = true;
				Curr = *pp;
				wf->putCWN("s", GetFinalForm(Curr.snum));
				wf->putCW(ParAlgns[Last.algn = Curr.algn]);
				wf->putCWN("li", Last.lind = Curr.lind + Cell->cellmargl);
				wf->putCWN("fi", Last.find = Curr.find, true);
				wf->putCWN("ri", Last.rind = Curr.rind + Cell->cellmargr);
				CalcLineSpace();
				wf->putCWN("sl", LastLineSpace = CurrLineSpace, true);
				wf->putCWN("sb", Last.bspace = Curr.bspace + Cell->cellmargb);
				//wf->putCWN("sa", Last.aspace = Curr.aspace);
			}
			else {
				if (fr->halgn > 0) {
					if ((Last.algn = fr->halgn - 1) > 2)
						Last.algn = (fr->halgn == 5) ? 2 : 0;
					wf->putCW(ParAlgns[Last.algn]);
					wf->putCWN("li", Last.lind = Cell->cellmargl);
					wf->putCWN("fi", Last.find = 0);
					wf->putCWN("ri", Last.rind = Cell->cellmargr);
				}
				if (GrTbSpace || ExactLSUsed)
					wf->putCWN("sb", Last.bspace = fr->high - LastLineSpace);
				else
					wf->putCWN("sb", Last.bspace = Cell->cellmargb);
			}
		}
		else {  // inline
			WriteText(wf);
			if (FrameBorders)
				WriteFrameBox(wf, fr);
		}
	}
	else if (fr->fpos) {
	  SaveState();
	  wf->putGS();
		WriteFrameStart(wf, fr);
	}
	else {  // inline
		WriteText(wf);
	  SaveState();
		if (FrameBorders)
			WriteFrameBox(wf, fr);
	  wf->putGS();
	}

  // put text or graphic contents
  WriteFrameCont(wf, fr);

  // put ending
	if (!InTbCell) {
		if (fr->fpos
		 && ((fr->fwrap != 1)
		  || (OutType != 'H'))) {
			//if (SideHUsed
			// && ((Curr.sideh > 0) || WritingHF)
			// && (SideHType == 'F'))  // already framed the para
			if (ParaFrameProps
			 || (SideHSpaceList.find(ParaRefNum) > 0))
				WriteAnchorPara(wf, -20);
			wf->putCW("par");
		}
		wf->putGE();
	  RestoreState(FullState);
	}
  wf->putEnd();
  WritingFrame = false;
	ParaFrameProps = true;
}


void
RTFwr::WriteFrameText(RTFwfile *wf, frame *fr, RTFgroup *gp)
{
	WritingFrame = true;
	InTextFrame = true;
	FramePendList->replace(0, (long) fr);
	SaveState();
	DCvlist *tlist = TblList;
	TblList = NULL;
	wf->putGS();
	gp->write(*wf);
	wf->putGE();
	wf->putEnd();
	TblList = tlist;
	RestoreState(FullState);
	InTextFrame = false;
	WriteAnchorPara(wf, -20);
	WritingFrame = false;
	ParaFrameProps = false;
	CurrTextFrame = NULL;
}


void
RTFwr::WriteFrameStart(RTFwfile *wf, frame *fr)
{
	long pnum = 0;
	short left = 0;
	pprop *pp = NULL;

	if (FrameStyle
	 && *FrameStyle
	 && ((pnum = FormNameList(FrameStyle)) != 0)
	 && ((pp = (pprop *) FormList.find(pnum)) != NULL)) {
		FrameStyleUsed = true;
		Curr = *pp;
	}
	else
		FrameStyleUsed = false;

	SetPardProps(wf);

	if (FrameStyleUsed)
		wf->putCWN("s", GetFinalForm(Curr.snum));

	WriteParaProps(wf, fr);

  if (FrExHigh
	 && fr->high
	 && !WordPerf)
    wf->putCWN("absh", -(fr->high), true);

	if (SideHUsed
	 && ((Curr.sideh > 0) || WritingHF)
	 && (SideHType == 'F'))  // already framed the para
		return;

	if (HelpForm)
		return;

	//SetPardProps();
  // Last = DefForm;

  // put box
	WriteFrameBox(wf, fr);

	if (WordPerf)
		return;

  // put positioning
  if ((fr->fwrap != 1)
	 && !WritingHFGraph
	 && !WrapAllFrames)
		wf->putCW("nowrap");

  if ((fr->vref) && (fr->vref <= FrameVRMax))
    wf->putCW(FrameVRef[fr->vref - 1]);
  if ((fr->href) && (fr->href <= FrameHRMax))
    wf->putCW(FrameHRef[fr->href - 1]);

  if (fr->halgn < FrameHAMax) {
		if ((fr->href > 1) // in colunn
		 && ((fr->wide > BodyWide) // but overflows it
		  || (SideHUsed && (fr->wide > (BodyWide - (SideHWide + SideHGap))))))
			fr->halgn = 1;  // ensure left alignment
		if (SideHUsed  // may need to move frame left to emulate Frame's fixup
		 && (SideHType == 'F')
		 && (SideHPos == 1)   // sideheads on left
		 && (fr->href > 1)    // in column
		 && ((fr->halgn == 1)  // left aligned
		  || (fr->halgn == 2)) // or center
		 && (fr->pxpos > 0)
		 && ((left = fr->pxpos - (LMarg + SideHWide + SideHGap)) < 0))
      wf->putCWN(FrameHAlgn[0], left, true);  // any pos move handled by indent
    else if (fr->halgn)
      wf->putCW(FrameHAlgn[fr->halgn]);
    else if (fr->xspace)
      wf->putCW(FrameHAlgn[1]);
    else
      wf->putCWN(FrameHAlgn[0], fr->xpos, true);
  }

  if (fr->valgn < FrameVAMax) {
    if (fr->valgn)
      wf->putCW(FrameVAlgn[fr->valgn]);
    else
      wf->putCWN(FrameVAlgn[0], fr->ypos, true);
  }

  //if (fr->wide)
  //  wf->putCWN("absw", fr->wide);
  if (fr->xspace) {
		left = fr->xspace;
		if (SideHUsed  // may need to move frame left to emulate Frame's fixup
		 && (SideHType == 'F')
		 && (SideHPos == 1)   // sideheads on left
		 && (left < 0))  // Frame seems to behave this terrible way...
			left += SideHWide + SideHGap;  // make relative to body column
		if (left > 0)
			wf->putCWN("dxfrtext", left);
	}
  wf->putEnd();
}


void
RTFwr::WriteFrameBox(RTFwfile *wf, frame *fr)
{
  if (!HelpForm
	 && fr->frul[0]) {
    wf->putCW("box");
    WriteRule(wf, fr->frul);
    wf->putCWN("brsp", fr->frulsp);
    wf->putEnd();
  }
}


void
RTFwr::WriteFrameCont(RTFwfile *wf, frame *fr, short rcrop)
{
	if (!fr)
		return;

	RTFgroup* gp = NULL;
	bool isEq = false;
	char *gfn = fr->grfile;;

  //if (fr->ftext) { // top level frame is text only
	//	if ((gp = (RTFgroup *) FlowList.find(fr->ftext)) != NULL)
  //    gp->write(*wf);
  //  return;
  //}

	graph *gr = NULL;
	frame *gfr = NULL;
	char *chp = NULL;
	char *bcp = NULL;
	char *pos_str = NULL;

	if (((fr->fpos == 0)  // at anchor 
	  || FrameStyleUsed   // or named style override
	  || ForceBmc)        // or HyperHelp preference
	 && (fr->fwrap != 1)) // and not a run-in
		pos_str = BmcStr;   // use char alignment
	else if ((fr->halgn == 2)  // center
	 && (fr->fpos > 0)
	 && (fr->fwrap != 1)  // and not a run-in
	 && (Curr.sideh < 2)) {    // or sidehead
		if (Curr.algn != 1) {
			wf->putCW("qc");
			Last.algn = 1;
		}
		pos_str = BmcStr;
	}
	else if (((fr->halgn == 3)  // right
	  || (fr->halgn == 5))      // or outer
	 && (Curr.sideh < 2))
		pos_str = BmrStr;   // use right alignment
	else                  // left or inner (or center)
		pos_str = BmlStr;   // use left alignment

	// use preview image in place of the rest
	if (UseGrPreviews 
	 && !NameGraphics
	 && fr->prtype
	 && (fr->eqtype != 2)
	 && ((chp = GraphFileName(gfn)) != NULL)) {
		LogEvent(loginfo, 2, "Using graphics file ", chp);
		if (OutType == 'H') {
			ProcFrameRefs(fr);
			WriteWMFRefs(wf);
			wf->putCS('{');
			wf->putStr(pos_str);
			bcp = BmxFileName(chp);
			wf->putStr(bcp);
			delete [] bcp;
			wf->putCS('}');
			SetGraphFDK(fr->id, chp);
			delete [] chp;
			return;
		}
		else if (!CheckGraphFile(chp, gfn, fr, true)) { // not embeddable
			if (GrVert > 0)
				wf->putCWN("dn", GrVert);
			else if (GrVert < 0)
				wf->putCWN("up", -GrVert);
			LogEvent(loginfo, 2, "Set graphics file ", chp, " for inclusion");
			if ((MetaType == 'E')   // the "\\d" means reference
			 || Word2003)
				wf->putField("INCLUDEPICTURE", chp, true, "\\\\d ");
			else if (Word8)
				wf->putField("INCLUDEPICTURE", chp, false);
			else                   // import into document for internal
				wf->putField("IMPORT", chp);
			GraphFileCopyProc(gfn);
			if (GrVert > 0)
				wf->putCWN("dn", 0);
			else if (GrVert < 0)
				wf->putCWN("up", 0);
			SetGraphFDK(fr->id, chp);
			delete [] chp;
			return;
		}
		else {  // replace frame contents
			gfr = new frame;
			gfr->grfile = NewName(fr->grfile);
			gfr->wide = fr->wide;
			gfr->high = fr->high;
			gr = new graph;
			gr->gtype = 0;
			gr->gsize = 1; // to prevent removal as empty
			gr->gparm	= (short *) gfr;

			fr->grcount = 1;
			if (fr->grlist)
				delete fr->grlist;
			fr->grlist = new DCvlist;
			(fr->grlist)->add(gr, 1);
			LogEvent(loginfo, 2, "Set graphics file ", chp, " for generation");
			SetGraphFDK(fr->id, chp);
			delete [] chp;
			chp = NULL;
		}
	}

	if (fr->eqtype == 2) {  // equation graphic in referenced file
		chp = GraphFileName(gfn);
		LogEvent(loginfo, 2, "Set equation ", chp, " for generation");
		SetGraphFDK(fr->id, chp);
		if (EmbedEqs) {  // embed converted .bmp in .wmf
			isEq = true;
			gfr = new frame;
			gfr->grfile = NewName(chp);
			gfr->eqtype = 2;
			gfr->xpos = (OutType == 'H') ? (EqHor * 10) : 0;
			gfr->ypos = (OutType == 'H') ? (EqVert * 10) : 0;
			fr->wide = (short) (((long) EqExpand * (long)(fr->wide)) / 100L);
			fr->high = (short) (((long) EqExpand * (long)(fr->high)) / 100L);
			gfr->wide = fr->wide;
			gfr->high = fr->high;
			//fr->high += (gfr->ypos * 2);

			gr = new graph;
			gr->gtype = 0;
			gr->gsize = 1; // to prevent removal as empty
			gr->gparm	= (short *) gfr;

			fr->grcount++;
			if (!fr->grlist)
				fr->grlist = new DCvlist;
			(fr->grlist)->add(gr, fr->grcount);
		}
		else if (OutType == 'H') {
			wf->putCS('{');
			wf->putStr(pos_str);
			bcp = BmxFileName(chp);
			wf->putStr(bcp);
			delete [] bcp;
			wf->putCS('}');
			delete [] chp;
			return;
		}
		else {
			if (EqVert > 0)
				wf->putCWN("dn", EqVert);
			else if (EqVert < 0)
				wf->putCWN("up", -EqVert);
			LogEvent(loginfo, 2, "Set equation ", chp, " for inclusion");
			if (Word2003)
				wf->putField("INCLUDEPICTURE", chp, true, "\\\\d ");
			else if (Word8)
				wf->putField("INCLUDEPICTURE", chp, false);
			else
				wf->putField("IMPORT", chp);
			GraphFileCopyProc(gfn);
			if (EqVert > 0)
				wf->putCWN("dn", 0);
			else if (EqVert < 0)
				wf->putCWN("up", 0);
			delete [] chp;
			return;
		}
		delete [] chp;
	}

	if ((fr->grcount == 1)   // single item in frame
	 && ((gr = (graph *) ((fr->grlist)->find(1))) != NULL)
	 && (gr->gtype == 0)     // the item is a frame itself
	 && ((gfr = (frame *) gr->gparm) != NULL)) {
		if (gfr->ftext != 0) { // the only frame is a text frame
			if (GrTxType != 'E') {
				if ((gp = (RTFgroup *) FlowList.find(gfr->ftext)) != NULL) {
					WriteFrameText(wf, fr, gp);
				  WriteAnchorPara(wf, 240);
				}
				return;
			}
		}
		else if (gfr->grfile != NULL) {  // frame references external file
			bool identified = false;
			chp = GraphFileName(gfr->grfile);
			// don't use SetGraphFDK(fr->id, chp);

			if (isEq || EmbedBMPs || EmbedWMFs) // see if file will go into wmf
				identified = CheckGraphFile(chp, gfr->grfile, gfr, isEq);

			if (identified) { // will go into wmf
				//gfr->grfile = fr->grfile;
				//gfr->ypos += GrVert * 10;
				//fr->grfile = chp;	// allow normal .bmp processing
				LogEvent(loginfo, 2, "Embed graphics file ", chp);
			}
			else {  // not a bmp or wmf that is to be wrapped
				char *suf = NULL;
				if (NameWMFsAsBMPs
				 && ((suf = strrchr(chp, '.')) != NULL)
				 && !stricmp(suf + 1, "wmf"))
					strcpy(suf + 1, "bmp");
				if (OutType == 'H') {  // allow use of .shg (or any other) files
					wf->putCS('{');
					wf->putStr(pos_str);
					bcp = BmxFileName(chp);
					wf->putStr(bcp);
					delete [] bcp;
					wf->putCS('}');
				}
				else if (EmpFrType == 'I')
					wf->putStr(gfr->grfile);  // always use original path to identify
				else {                // put field in for graphic
					if (GrVert > 0)
						wf->putCWN("dn", GrVert);
					else if (GrVert < 0)
						wf->putCWN("up", -GrVert);
					LogEvent(loginfo, 2, "Set graphics file ", chp, " for inclusion");
					if ((MetaType == 'E')   // the "\\d" means reference
					 || Word2003)
						wf->putField("INCLUDEPICTURE", chp, true, "\\\\d ");
					else if (Word8)
						wf->putField("INCLUDEPICTURE", chp, false);
					else                  // import into document for internal
						wf->putField("IMPORT", chp);
					GraphFileCopyProc(gfr->grfile);
					if (GrVert > 0)
						wf->putCWN("dn", 0);
					else if (GrVert < 0)
						wf->putCWN("up", 0);
					SetGraphFDK(gfr->id, chp);
				}
				delete [] chp;
				return;
			}
		}
	}

  if (fr->grcount) { // frame contains graphics
		bool named = false;
		if (NameGraphics && (OutType != 'H')) {
			if (!fr->grfile && (!gfr || !gfr->grfile)) {
				for (uns i = 1; i <= fr->grcount; i++) {
					if (((gr = (graph *) ((fr->grlist)->find(i))) != NULL)
					 && (gr->gtype == 0)     // the item is a frame itself
					 && ((gfr = (frame *) gr->gparm) != NULL)
					 && gfr->grfile) {
						named = true;
						break;  // stop on first frame
					}
				}
				// and what about fr->grfile?  ****
			}
			else named = true;
		}
		// bool named = (NameGraphics && (OutType != 'H')
		//               && fr->grfile && gfr->grfile);
		short vert = isEq ? EqVert : GrVert;
		if (fr->fpos == 0) {
			if (OutType == 'H') {
				if (!isEq)
					fr->ypos += GrVert * 10;
			}
			else {
				if (vert > 0)
					wf->putCWN("dn", vert);
				else if (vert < 0)
					wf->putCWN("up", -vert);
			}
		}

		if (named) {  // put in locked field with filename
			//char *fn = gfr->grfile;
			//char *slash = strrchr(fn, '/');
			//if (GrPaths == 'N')
			//	fn = NewName(slash + 1);
			chp = GraphFileName((gfr && gfr->grfile) ? gfr->grfile : gfn);
			wf->putField("INCLUDEPICTURE", chp, true,
			             NULL, NULL, false, true);
			GraphFileCopyProc(chp);
		}
		FrameLevel = 1;
		RFrame = fr;  // top level frame
		if (gfr) {
			if (gfr->high
			 && !fr->high)
				fr->high = gfr->high;
			if (gfr->wide
			 && !fr->wide)
				fr->wide = gfr->wide;
		}
		SetWMFStart(wf, fr, rcrop); // start diversion
		WriteWMFFrame(wf, fr, 0, 0); // will recurse if nested frames
    WriteWMFEnd(wf);	// end diversion
    WriteWMFStart(wf, fr); // write diversion
    SetWMFEnd(wf, fr, pos_str);
		assert(FrameLevel == 1);
		if (named) {
			wf->putGE();
			wf->putGE();
		}

		if ((fr->fpos == 0) 
		 && (OutType != 'H')) {
			if (vert > 0)
				wf->putCWN("dn", 0);
			else if (vert < 0)
				wf->putCWN("up", 0);
		}
		return;
  }
	if (EmpFrType == 'I')  // no content, no name, no math, give up
		wf->putStr("**Empty**");
}


bool RTFwr::CopyOriginalGraphics = false;
DCnlist RTFwr::GraphFileCopyList;
DCnlist RTFwr::GraphFileFixedCopyList;
long RTFwr::GraphCopyCount = 0;

void
RTFwr::GraphFileCopyProc(char *cpt)
{
	if (!CopyOriginalGraphics)
		return;

	char *np = new char[PATHMAX];
	strcpy(np, cpt);
	MapGraphFile(np);
	if (!GraphFileCopyList.getnum(np))
		GraphFileCopyList.addnew(np, ++GraphCopyCount);
	delete [] np;
}


char*
RTFwr::GraphFileName(const char* ocp)
{
	char *npath = new char[PATHMAX];
	char *np = NULL;

	if ((np = strrchr(ocp, '/')) != NULL) { // UNIX pathname
		if (GrPaths == 'N')        // no path	wanted
			strcpy(npath, ++np); // point to char after last slash
		else if (GrPaths == 'R')   // retain the existing path	(default)
			strcpy(npath, ocp);
	}
	else if ((np = strrchr(ocp, '\\')) != NULL) { // backslash pathname
		if (GrPaths == 'N')         // no path	wanted
			strcpy(npath, ++np); // point to char after last backslash
		else if (GrPaths == 'R') {  // retain the existing path	(default)
			np = npath;
			while (*ocp) {
				if (*ocp == '\\') { // change the backslashes to slashes
					*np++ = '/';
					ocp++;
				}
				else *np++ = *ocp++;
			}
			*np = '\0';
		}
	}
	else strcpy(npath, ocp); // no path, just use filename

	return MapGraphFile(npath);
}


char*
RTFwr::MapGraphFile(char *npath)
{
	char *np = NULL;
	char *op = NULL;
	char *oorig = NULL;

	if ((GrNames == 'M') && IniFile
	 && IniFile->Section("GraphFiles")) {	// read graphics mapping
		if (IniFile->Find(npath))	{
			op = oorig = IniFile->StrRight();
			np = npath;
			while (*op) {
				if (*op == '\\') { // change the backslashes to slashes
					*np++ = '/';
					op++;
				}
				else *np++ = *op++;
			}
			*np = '\0';
			DeleteName(oorig);
		}
		else if (((np = strrchr(npath, '.')) != NULL)
		 && IniFile->Find(++np)) {
			*np = '\0';
			strcat(npath, (np = IniFile->StrRight()));
			DeleteName(np);
		}				
	}
	return npath;	// caller must delete [] when done
}


char*
RTFwr::BmxFileName(const char *ocp)
{
	char *npath = NULL;
	char *np;

	if (((np = strrchr(ocp, '/')) != NULL)  
	 || ((np = strrchr(ocp, '\\')) != NULL)
	 || ((np = strrchr(ocp, ':')) != NULL)) {
		npath = new char[strlen(np)];
		strcpy(npath, ++np); // point to char after path or drive
	}
	else {
		npath = new char[strlen(ocp) + 1];
		strcpy(npath, ocp);
	}
	return npath;	// caller must delete [] when done
}


bool 
RTFwr::CheckGraphFile(char *fname, char *oname, frame *fr, bool fdk)
{
	if (!fname
	 || (*fname == '\0'))
		return false;

	bool identified = false;
	char *slash = NULL;
	char *nname = NULL;
	char *suf = strrchr(fname, '.');
	if (suf != NULL)
		suf++;

	fr->ngrfile = NULL;

	DCrfile *bmf = new DCrfile(fname);

	if (bmf->Ferr == fok)
		fr->ngrfile = fname;

	if ((bmf->Ferr != fok)
	 && ((slash = strrchr(fname, '/')) == NULL)    // new name has no path
	 && ((slash = strrchr(oname, '/')) != NULL)) {  // but old one did
		nname = NewName(strlen(fname) + (slash - oname) + 1);
		strncpy(nname, oname, (slash - oname) + 1);
		strcat(nname, fname);
		bmf = new DCrfile(nname);
		if (bmf->Ferr == fok)
			fr->ngrfile = nname;
	}

	if (bmf->Ferr != fok) {
		bmf = new DCrfile(oname);
		if (bmf->Ferr != fok) {
			LogEvent(logerr, 1, "Cannot read \"", fname, "\" or \"", oname, "\"");
			return false;  // bail out
		}
		fr->ngrfile = oname;
	}

	if (suf) {
		if ((fdk || EmbedBMPs)
		 && !_stricmp(suf, "bmp")) {
			if ((bmf->FErr() == fok)
			 &&	(bmf->getChar() == 'B')
			 && (bmf->getChar() == 'M'))  // bmp, will go into wmf
				identified = true;
		}
		else if ((fdk || EmbedWMFs)
		 && !_stricmp(suf, "wmf")) {
			if (bmf->FErr() == fok) {
				unc ch = bmf->getChar();
				unc nch = bmf->getChar();
				if (((ch == 0xD7) && (nch == 0xCD))
				 || ((ch == 1) && (nch == 0)))
					identified = true;
			}
		}
	}
	bmf->CloseFile();
	return identified;
}


void 
RTFwr::SetGraphFDK(long id, char* fname)
{
	if (!FMSource)
		return;

	char *idstr = NewName(10);
	sprintf(idstr, "%ld", id);

	DCpfile::putval("GrGen", idstr, fname, CurrGrxFile);

	DeleteName(idstr);
}


void 
RTFwr::ClearGraphFDK(void)
{
	FILE *fp = NULL;

	// make new graphics file each time
	_unlink(CurrGrxFile);
	if ((fp = fopen(CurrGrxFile, "wt")) != NULL)  { // create file
		fputs("; graphics file for ", fp);
		fputs(CurrFileName, fp);
		fputs("\n\n", fp);
		fputs("[GrGen]\n\n", fp);
		fputs("[GrDpi]\n\n", fp);
		fputs("[GrUse]\n\n", fp);
		fputs("[End]\n;end of grx file\n\n", fp);
		fclose(fp);
	}
	DCpfile::psetup(CurrGrxFile);
}





// graphic processing


const double RTFwr::PI = 3.1415926535;

graph *RTFwr::Graphic;
anyini RTFwr::GraphIni;
RTFctl *RTFwr::GrShapeCtl;
grarc RTFwr::CurrGrArc;
bool RTFwr::InGraphic;
uns RTFwr::GrGroupStack[GrGroupMax];
uns RTFwr::GrGroupLevel;
short RTFwr::GrStretch = 3;

uns RTFwr::GrStyleMap[GrStyleMax] = {
  5, 0, 0, 0, 0, 0, 0, 0,
  255, 1, 1, 2, 2, 4, 3, 3
//  5, 0, 1, 1, 1, 2, 2, 2,
//  255, 3, 3, 3, 3, 4, 4, 4
};

uns RTFwr::GrFillMap[GrFillMax] = {
#if 0
  0, 255, 230, 180, 128, 76, 26, 8, 0,
#endif
  0, 0, 26, 76, 128, 180, 230, 248, 255, // multiplier for color byte
  3, 2, 1, 0, 5, 3, 2
};


void
RTFwr::ProcGraphic(RTFctl *cp)
{
#if 0
  frame *fr;
#endif

  InGraphic = true;
  if (FrameLevel) {
    if (FrameStack[FrameLevel - 1] == NULL)
      ProcEndFrame();
  }
#if 0
  else { // not in frame, create one as page-size underlay or overlay
    fr = new frame;
    FrameLevel = 1;
    FrameStack[0] = fr;
    fr->href = fr->vref = 1;
    fr->halgn = fr->valgn = 1;
    fr->wide = Page->pwide;
    fr->high = Page->phigh;
    InitFrame(fr);
    fr->fpos = 4;
    FramePageList.add(fr, LastPage);
  }
#endif
  Graphic = new graph;
	Graphic->gypos = 0;
  if (Verbose)
    printf("Graphic ");
	if (cp->dat() == 6) {  // external graphic object, make nested frame
	  Frame = new dcframe;
		RFrame = new frame;
		Graphic->gtype = 0;
		Graphic->gsize = 1; // to prevent removal as empty
    Graphic->gparm	= (short *) RFrame;
	}
}


void
RTFwr::EndGraphic(RTFctl *cp)
{
  InGraphic = false;
	if (Graphic == NULL)  // already deleted
		return;
  if ((Graphic->gsize == 0) // ignore empty graphics
   || ((Graphic->gsize == 6) && (Graphic->gtype == 0x521))) {
    delete Graphic;
    if (Verbose)
      printf("empty\n");
    return;
  }
  if (Verbose)
    printf("\n");

	if ((Graphic->gtype == 0x521)
	 && (InGrTextFrame == false)  // for callouts only
	 && (GText != NULL)) {
		if (cp && (GTAlignChange == 2)) {
			GTWidth += GText->twide;
			GTFixList.all(GTFixup);
			GTFixList.empty();
			GTAlignChange = GTWidth = 0;
		}
		else if (GTAlignChange < 2) {
			if (GText->axpos == 0)
				GText->axpos = GText->hpos;
			if (GText->aypos == 0)
				GText->aypos = GText->vpos;
		}
	}

  frame *fr = NULL;

	if (FrameLevel == 0) {
		if ((LastPage != LastMasterPage)
		 || (Page->ptype > 2)) {
			delete Graphic;
			return;
		}
		// choose frame to fasten to, header or footer
		if (((Graphic->gtype == 0) ? Frame->ypos : Graphic->gypos) <= (PaperHigh / 2))
			fr = &HeadGraphFrame;
		else
			fr = &FootGraphFrame;
	}
	else
		fr = FrameStack[FrameLevel - 1];

  if (!fr->grlist)
    fr->grlist = new DCvlist;
	fr->grcount++;
	(fr->grlist)->add(Graphic, fr->grcount);

  *(Graphic->gpen = new wmf_pen) = CurrPen;
  *(Graphic->gbrush = new wmf_brush) = CurrBrush;

	if ((cp != NULL)         // not a graphic text conversion
	 && (cp->dat() == 6)) {  // end of import object, update frame
		RFrame->ftext = Frame->ftext;
		RFrame->wide = Frame->wide;
		RFrame->high = Frame->high;
		RFrame->xpos = Frame->xpos;
		RFrame->ypos = Frame->ypos;
		RFrame->frot = Frame->frot;
    RFrame->fpos = 0;
    RFrame->halgn = 0;
    RFrame->valgn = fr->valgn;
    RFrame->href = 1;
    RFrame->vref = fr->vref;
		if (FrameLevel > 2) {
			RFrame->xpos += fr->xpos;
			RFrame->ypos += fr->ypos;
    }
    RFrame->xspace = fr->xspace;
		Frame = NULL;
		RFrame = NULL;
    return;
  }

	if (Graphic->gtype == 0xF43) // stretch mode for rasters
		fr->stretch = GrStretch;

	// handle arrowheads where appropriate

	if (cp == NULL)  // arrowhead or graphic text
		return;

	if ((Graphic->gtype != 0x325)  // polylines (including Beziers)
	 && (Graphic->gtype != 0x817)) // elliptic arcs
		return;

	graph *gr = Graphic; // functions below recurse, save on stack

	if (gr->grhead > 2)
		ProcGrArHead(gr);
	if (gr->grtail > 2)
		ProcGrArTail(gr);
}


// arrow processing


void
RTFwr::ProcArrowDef(RTFctl *cp)
{
	// custom arrowhead definition
	short anum = cp->dat();
	long *lptr = (long *) cp->ex();

  if (lptr[0] != 3)  // only FrameMaker for now
		return;

	arrow *ar = new arrow;
	ar->alen = (GetTwips(lptr[1], gl_unit) + 10) / 20;
	ar->afill = (short) lptr[2];
	ar->atip = (short) lptr[3];
	ar->abase = (short) lptr[4];

	arrdefs.add(ar, (long) anum); 
}


void
RTFwr::ProcGrArHead(graph *gr)
{
	short *tipx;
	short *tipy;
	short *vect;
	double ang;
	short ellarc = 0;
	short polyln = 0;
	short *lim = &(gr->gparm[0]);
	unl *gsiz = &(gr->gsize);
	int parms;

	if (gr->gtype == 0x325) {  // polyline
		if ((parms = gr->gsize - 3) < 5)
			return;

		tipx = &(gr->gparm[parms - 2]);
		tipy = &(gr->gparm[parms - 1]);
		vect = &(gr->gparm[parms - 4]);

		if ((*tipx == vect[0])
		 && (*tipy == vect[1]))
			return;  // avoid atan2 domain error

		ang = atan2((double) (vect[1] - *tipy),
								(double) (vect[0] - *tipx));
		polyln = -2;
	}
	else if (gr->gtype == 0x817) {  // elliptic arc
		tipy = &(gr->gparm[2]);  // y then x
		tipx = &(gr->gparm[3]);
		ang = CurrGrArc.hang;
		ang -= PI / 2;
		if (ang < PI)
			ang += (PI * 2);
		ellarc = 1;
	}

	ProcGrArrow(gr->grhead, tipx, tipy, ang, ellarc, polyln, lim, gsiz);
}


void
RTFwr::ProcGrArTail(graph *gr)
{
	short *tipx;
	short *tipy;
	short *vect;
	double ang;
	short ellarc = 0;
	short polyln = 0;
	short *lim = &(gr->gparm[0]);
	unl *gsiz = &(gr->gsize);

	if (gr->gtype == 0x325) {  // polyline
		if (gr->gsize < 8)
			return;

		tipx = &(gr->gparm[1]);
		tipy = &(gr->gparm[2]);
		vect = &(gr->gparm[3]);

		if ((*tipx == vect[0])
		 && (*tipy == vect[1]))
			return;  // avoid atan2 domain error

		ang = atan2((double) (vect[1] - *tipy),
								(double) (vect[0] - *tipx));
		polyln = 2;
	}
	else if (gr->gtype == 0x817) {  // elliptic arc
		tipy = &(gr->gparm[0]);  // y then x
		tipx = &(gr->gparm[1]);
		ang = CurrGrArc.tang;
		ang += PI / 2;
		if (ang > PI)
			ang -= (PI * 2);
		ellarc = -1;
	}

	ProcGrArrow(gr->grtail, tipx, tipy, ang, ellarc, polyln, lim, gsiz);
}

short RTFwr::arrtips[GrArrStyleMax] = { 0, 0, 0,
 30, 30, 16, 16, 16, 30, 16, 16 }; 
short RTFwr::arrbases[GrArrStyleMax] = { 0, 0, 0,
 90, 90, 90, 70, 120, 90, 90, 90 };
short RTFwr::arrlens[GrArrStyleMax] = { 0, 0, 0,
 6, 8, 12, 12, 12, 6, 12, 12 };
short RTFwr::arrfills[GrArrStyleMax] = { 0, 0, 0,
 1, 1, 1, 1, 1, 3, 3, 2 };
DCvlist RTFwr::arrdefs;

void
RTFwr::ProcGrArrow(uns astyle, short *tipx, short *tipy, double ang,
                   short ellarc, short polyln, short *verts, unl *gsiz)
{
	if (astyle < 3)  // line end, not arrow, not used in WMF
		return;

	// process custom arrowhead styles
	if (astyle >= GrArrStyleMax) {
		arrow *ar;
		if ((ar = (arrow *) arrdefs.find((long) astyle)) != NULL) {
			astyle = 0;	 // use for temp custom style
			arrlens[0] = ar->alen;
			arrfills[0] = ar->afill;
			arrtips[0] = ar->atip;
			arrbases[0] = ar->abase;
		}
		else astyle = 3;	// use small head if unknown
	}

	// produce arrow with point at tip and base in direction of vect
	short tip[2];   // local copy to use for end of arc
	tip[0] = *tipx;
	tip[1] = *tipy;

	short top[2];
	short ctr[2];
	short bot[2];

	short tipval = arrtips[astyle];
	short baseval = arrbases[astyle];
	short pts = arrlens[astyle];
	short fill = arrfills[astyle];

	double tipang = (PI * tipval)	/ 180.0;

	short len = ((pts * CurrPen.pwidth) / 4) + (pts * 20);

	if ((tipval > 25) && (fill == 1))    // small head
		len -= pts * 5;      // reduce by 1pt of pen width
	else if (fill == 3)    // scale pen width down for stick form
		len -= (CurrPen.pwidth * 3) / 4;
	if (len > 1280)	       // limit of 64 pts
		len = 1280;


	// compute polylines for arrowhead
	if (ellarc) {
		top[0] = tip[0] - (short) (cos(ang - tipang) * len);
		bot[0] = tip[0] - (short) (cos(ang + tipang) * len);
	}
	else {
		top[0] = tip[0] + (short) (cos(ang - tipang) * len);
		bot[0] = tip[0] + (short) (cos(ang + tipang) * len);
	}

	top[1] = tip[1] + (short) (sin(ang - tipang) * len);
	bot[1] = tip[1] + (short) (sin(ang + tipang) * len);

	ctr[0] = (top[0] + bot[0]) / 2;
	ctr[1] = (top[1] + bot[1]) / 2;

	// compute displacement for base angle != 90
	double baseang;	// base angle in radians
	double bclen;   // back to ctr length

	if (baseval != 90) {
		baseang = (PI * baseval) / 180.0;
		double tclen = sqrt(((double) (ctr[0] - top[0]) * (ctr[0] - top[0]))
		                  + ((double) (ctr[1] - top[1]) * (ctr[1] - top[1])));
		bclen = tclen / tan(baseang);
		if (ellarc < 1)
			bclen = -bclen;

		ctr[0] += (short) (cos(ang) * bclen); 
		ctr[1] += (short) (sin(ang) * bclen); 
	}
	else {
		baseang = PI / 2;
		bclen = 0.0;
	}

	bool trim = true;

	// move arrowhead to visually correct angle at end of arc
	if (ellarc) {	// 1 at head, -1 at tail

		// find angle from center of ellipse to back of arrowhead
		double backang = atan2((double) (ctr[1] - CurrGrArc.cy) * CurrGrArc.lenx,
		                       (double) (ctr[0] - CurrGrArc.cx) * CurrGrArc.leny);

		// compute new pos for back of arrowhead, on the ellipse
		ctr[0] = (((short) (cos(backang) * CurrGrArc.lenx)) + CurrGrArc.cx);
		ctr[1] = (((short) (sin(backang) * CurrGrArc.leny)) + CurrGrArc.cy);

		if (fill == 3) {  // stick form, trim arc to back of head
			*tipx = ctr[0];
			*tipy = ctr[1];
			trim = false;
		}

		// find new ang from tip to back 
		ang = atan2((double) (ctr[1] - tip[1]),
								(double) (ctr[0] - tip[0]));

		// compute new top and bot
		top[0] = tip[0] + (short) (cos(ang - tipang) * len);
		top[1] = tip[1] + (short) (sin(ang - tipang) * len);

		bot[0] = tip[0] + (short) (cos(ang + tipang) * len);
		bot[1] = tip[1] + (short) (sin(ang + tipang) * len);

		// readjust ctr again to get correct base angle
		ctr[0] = (top[0] + bot[0]) / 2;
		ctr[1] = (top[1] + bot[1]) / 2;

		if (baseval != 90) {
			ctr[0] += (short) (cos(ang) * bclen) * -ellarc; 
			ctr[1] += (short) (sin(ang) * bclen) * -ellarc; 
		}
	}
	else if ((polyln != 0) && (*verts > 2)) {  // more than one line seg
		// rotate arrowhead so center of back is on a line seg
		double arlen = sqrt(((double) (ctr[0] - tip[0]) * (ctr[0] - tip[0]))
		                  + ((double) (ctr[1] - tip[1]) * (ctr[1] - tip[1])));
		double dist = 0.0;
		double newang = ang;
		bool found = false;
		short segs = *verts - 1;
		short *vecx = tipx + polyln;  // next vertex
		short *vecy = tipy + polyln;
		short sx = *tipx;
		short sy = *tipy;
		short ex = *vecx;
		short ey = *vecy;

		// if end of line seg is closer to tip than arrow len, keep going
		for (;;) {
			dist = sqrt(((double) (ex - tip[0]) * (ex - tip[0]))
			          + ((double) (ey - tip[1]) * (ey - tip[1])));
			if (dist > arlen) {
				found = true;
				break;
			}
			if (--segs < 1)
				break;
			sx = ex;
			sy = ey;
			ex = *(vecx += polyln);
			ey = *(vecy += polyln);
		}

		// end of line past arrow len, put ctr onto last line seg
		if (found) {
			short mx = 0;
			short my = 0;
			double ldist = arlen;
			while (dist != arlen) {
				mx = ((sx - ex) / 2) + ex;
				my = ((sy - ey) / 2) + ey;
				dist = sqrt(((double) (mx - tip[0]) * (mx - tip[0]))
					        + ((double) (my - tip[1]) * (my - tip[1])));
				if (dist > arlen) {
					ex = mx;
					ey = my;
				}
				else {
					sx = mx;
					sy = my;
				}
				if ((ex == sx)
				 && (ey == sy))
					break;
				if (dist == ldist)
					break;
				ldist = dist;
			}
			ctr[0] = mx;
			ctr[1] = my;
			newang = atan2((double) (ctr[1] - tip[1]),
										 (double) (ctr[0] - tip[0]));
		}
		else { // last seg still inside arrowhead
			// use vector from tip to ex/ey to determine angle
			newang = atan2((double) (tip[1] - ey),
										 (double) (tip[0] - ex));
		}

		if (newang != ang) {  // required adjustment
			ang = newang;

			// compute new top and bot
			top[0] = tip[0] + (short) (cos(ang - tipang) * len);
			top[1] = tip[1] + (short) (sin(ang - tipang) * len);

			bot[0] = tip[0] + (short) (cos(ang + tipang) * len);
			bot[1] = tip[1] + (short) (sin(ang + tipang) * len);

			// readjust ctr again to get correct base angle
			ctr[0] = (top[0] + bot[0]) / 2;
			ctr[1] = (top[1] + bot[1]) / 2;

			if (baseval != 90) {
				ctr[0] += (short) (cos(ang) * bclen); 
				ctr[1] += (short) (sin(ang) * bclen); 
			}
		}

		// see if trimming line requires removal of line segs
		if (segs < (*verts - 1)) {
			short remove = (*verts - 1) - segs;
			if (polyln > 0)   // removing from front, move up
				memmove(verts + 1, verts + 1 + (remove * 2), (segs + 1) * 4);
			*verts -= remove;
			*gsiz -= (remove * 2);
			trim = false;  //  segs removed
		}
	}

	if (trim) {
		// trim lineseg back along its own axis so that it ends within head
		short xmov = tip[0] - ctr[0];
		short ymov = tip[1] - ctr[1];

		if (fill == 3) {  // stick form on line, trim slightly
			xmov /= 6;
			ymov /= 6;
		}

		*tipx -= xmov;
		*tipy -= ymov;
	}

	// construct graph object from computed positions
	Graphic = new graph;

	if (fill == 3) {
		Graphic->gtype = 0x325;  // polyline
		Graphic->gsize = 10;     // (3 points * 2) + 4
		Graphic->gparm = new short[7];
		Graphic->gparm[0] = 3;

		Graphic->gparm[1] = top[0]; // start at top corner at base
		Graphic->gparm[2] = top[1];

		Graphic->gparm[3] = tip[0]; // tip
		Graphic->gparm[4] = tip[1];

		Graphic->gparm[5] = bot[0]; // bottom corner of base
		Graphic->gparm[6] = bot[1];
	}
	else {
		Graphic->gtype = 0x324;  // polygon
		Graphic->gsize = 14;     // (5 points * 2) + 4
		Graphic->gparm = new short[11];
		Graphic->gparm[0] = 5;
		Graphic->gparm[1] = tip[0]; // start at tip
		Graphic->gparm[2] = tip[1];

		Graphic->gparm[3] = top[0]; // top corner at base
		Graphic->gparm[4] = top[1];

		Graphic->gparm[5] = ctr[0]; // center of base
		Graphic->gparm[6] = ctr[1];

		Graphic->gparm[7] = bot[0]; // bottom corner of base
		Graphic->gparm[8] = bot[1];

		Graphic->gparm[9] = tip[0];	// finish at tip
		Graphic->gparm[10] = tip[1];
	}

	// set pen and brush for drawing arrowhead
	wmf_brush SavedBrush = CurrBrush;
	wmf_pen SavedPen = CurrPen;

	if (fill == 1) {  // solid
		CurrPen.pwidth = 15;  // keep lines sharp
		CurrPen.pstyle = 6;   // and within polygon
		CurrBrush.bstyle = 0;
		CurrBrush.bcolor = CurrPen.pcolor;
		CurrBrush.bhatch = 0;
		CurrBrush.bshade = 0;
	}
	else if (fill == 2) {  // hollow = white
		if (CurrPen.pwidth > 40)	// scale down above 2pt
			CurrPen.pwidth = ((CurrPen.pwidth - 40) / 8) + 40;
		CurrPen.pstyle = 6;   // rule within polygon
		// may need to use smaller pen and draw twice
		CurrBrush.bstyle = 0;
		CurrBrush.bcolor = 0xFFFFFF;
		CurrBrush.bhatch = 0;
		CurrBrush.bshade = 0;
	}
	else if (fill == 3) {  // stick
		if (CurrPen.pwidth > 40)	// scale down above 2pt
			CurrPen.pwidth = (CurrPen.pwidth * 3) / 4;
		// may need to use smaller pen, and draw outline and fill
	}

	EndGraphic(NULL);
	CurrBrush = SavedBrush;
	CurrPen = SavedPen;
}



// graphic line, pen, fill, and shape processing


void
RTFwr::ProcGrLine(RTFctl *cp)
{
	uns cind;

  switch(cp->dat()) {
    case 1:  // line pattern
			CurrPen.lstyle = (short) cp->siz() - 1;
			if (CurrPen.lstyle < 0)
				CurrPen.lstyle = 0;
      break;
    case 2:  // line color
			GetColor(cp->siz(), &cind);
      if (cind == Invisible)
        CurrPen.pstyle = 5;
      else {
        cind -= 2;
        CurrPen.pcolor = ((((long) BlueTable[cind]) << 16) & 0xFF0000)
                       + ((((long) GreenTable[cind]) << 8) & 0xFF00)
                       + (RedTable[cind] & 0xFF);
      }
      break;
    case 3:  // line thickness
      CurrPen.pwidth = GetTwips(cp->siz(), gl_unit);
      break;
    default:
      break;
  }
}


void
RTFwr::ProcGrPen(RTFctl *cp)
{
  switch(cp->dat()) {
    case 1:  // pen fill pattern
			if ((cp->siz() >= 0) && (cp->siz() < GrStyleMax))
				CurrPen.pstyle = GrStyleMap[cp->siz()];
			else CurrPen.pstyle = 0; // solid
      break;
    case 2:  // fg color
      break;
    case 3:  // bg color
      break;
    case 4:  // shading
      CurrPen.pshade = (uns) cp->siz();
      break;
    default:
      break;
  }
}


void
RTFwr::ProcGrFill(RTFctl *cp)
{
	uns bfill = 0;
	uns cind;

  switch(cp->dat()) {
    case 1:  // brush fill pattern
      CurrBrush.bstyle = 0;
      CurrBrush.bshade = 0;
      if (cp->siz() == 0)
        CurrBrush.bstyle = 1;
      else if ((cp->siz() > 1)
            && (cp->siz() < GrFillMax)) {
				bfill = GrFillMap[cp->siz()];
        if (bfill < 6) {
          CurrBrush.bhatch = bfill;
          CurrBrush.bstyle = 2;
        }
        else CurrBrush.bshade = bfill;
      }
      break;
    case 2:  // fg color
			GetColor(cp->siz(), &cind);
      if (cind == Invisible)
        CurrBrush.bstyle = 1;
      else {
        cind -= 2;
        CurrBrush.bcolor = ((((long) BlueTable[cind]) << 16) & 0xFF0000)
                         + ((((long) GreenTable[cind]) << 8) & 0xFF00)
                         + (RedTable[cind] & 0xFF);
      }
      break;
    case 3:  // bg color
      break;
    case 4:  // shading
      CurrBrush.bshade = (uns) cp->siz();
      break;
    default:
      break;
  }
}


void
RTFwr::ProcGrShape(RTFctl *cp)
{
  if (Verbose)
    printf("shape %d ", cp->dat());

	GrShapeCtl = cp;

  switch (cp->dat()) {
    case 1:  // polylines
      Graphic->gtype = 0x325;
      ProcGrPoly(cp);
      break;
    case 2:  // polygons
      Graphic->gtype = 0x324;
      ProcGrPoly(cp);
      break;
    case 3:  // polymarkers
      break;

    case 10:  // rects
      Graphic->gtype = 0x41B;
      ProcGrRect(cp, 1);
      break;
    case 11:  // rounded rects
      Graphic->gtype = 0x61C;
      ProcGrRect(cp, 2);
      break;

    case 20:  // ellipses
      Graphic->gtype = 0x418;
      ProcGrEllipse(cp);
      break;
    case 21:  // IL ellipses
      break;

    case 30:  // arcs
      Graphic->gtype = 0x817;
      ProcGrArc(cp);
      break;
    case 31:  // pie arcs
      Graphic->gtype = 0x81A;
      ProcGrArc(cp);
      break;
    case 32:  // chord arcs
      Graphic->gtype = 0x830;
      ProcGrArc(cp);
      break;

    case 33:  // hermite (IL) arcs
      break;
    case 34:  // hermite (IL) arcs, extended
      break;

    case 40:  // open splines
      break;
    case 41:  // closed splines
      break;

    case 50:  // open Beziers
      Graphic->gtype = 0x325;
      ProcGrBezier(cp);
      break;
    case 51:  // closed Beziers
      Graphic->gtype = 0x324;
      ProcGrBezier(cp);
      break;

    default:
      break;
  }
}


void
RTFwr::ProcGrPoly(RTFctl *cp)
{
  uns len = (uns) (cp->siz() / sizeof(long));
  long *lptr = (long *) cp->ex();
  Graphic->gsize = 4 + len;
  Graphic->gparm = new short[len + 1];
  Graphic->gparm[0] = (short) len / 2;
  for (uns i = 0; i < len; i++) {
    Graphic->gparm[i + 1] = GetTwips(lptr[i], pg_unit);
		if (((i & 1) == 0)
		 && (Graphic->gparm[i] > Graphic->gypos))
			Graphic->gypos = Graphic->gparm[i];
	}
}


void
RTFwr::ProcGrRect(RTFctl *cp, uns rtyp)
{
  uns len = (uns) (cp->siz() / sizeof(long));
  long *lptr = (long *) cp->ex();
	long wide = lptr[2] ? lptr[2] : 10;
	long high = lptr[3] ? lptr[3] : 10;

  if (rtyp == 1) {
		if (RectIsPoly) {
      Graphic->gtype = 0x324;
			Graphic->gsize = 14;
			Graphic->gparm = new short[11];
		  Graphic->gparm[0] = 5;
			Graphic->gparm[1] = GetTwips(lptr[0], pg_unit);
			Graphic->gparm[2] = GetTwips(lptr[1], pg_unit);
			Graphic->gparm[3] = GetTwips(lptr[0] + wide, pg_unit);
			Graphic->gparm[4] = Graphic->gparm[2];
			Graphic->gparm[5] = Graphic->gparm[3];
			Graphic->gparm[6] = GetTwips(lptr[1] + high, pg_unit);
			Graphic->gparm[7] = Graphic->gparm[1];
			Graphic->gparm[8] = Graphic->gparm[6];
			Graphic->gparm[9] = Graphic->gparm[1];
			Graphic->gparm[10] = Graphic->gparm[2];
			Graphic->gypos = Graphic->gparm[6];
		}
		else {
			Graphic->gsize = 7;
			Graphic->gparm = new short[4];
			Graphic->gparm[0] = GetTwips(lptr[1] + high, pg_unit);
			Graphic->gparm[1] = GetTwips(lptr[0] + wide, pg_unit);
			Graphic->gparm[2] = GetTwips(lptr[1], pg_unit);
			Graphic->gparm[3] = GetTwips(lptr[0], pg_unit);
			Graphic->gypos = Graphic->gparm[0];
		}
  }
  else if (rtyp == 2) {
    Graphic->gsize = 9;
    Graphic->gparm = new short[6];
    Graphic->gparm[0] = GetTwips(lptr[5] * 2, pg_unit);
    Graphic->gparm[1] = GetTwips(lptr[4] * 2, pg_unit);
    Graphic->gparm[2] = GetTwips(lptr[1] + high, pg_unit);
    Graphic->gparm[3] = GetTwips(lptr[0] + wide, pg_unit);
    Graphic->gparm[4] = GetTwips(lptr[1], pg_unit);
    Graphic->gparm[5] = GetTwips(lptr[0], pg_unit);
		Graphic->gypos = Graphic->gparm[2];
  }
}


void
RTFwr::ProcGrEllipse(RTFctl *cp)
{
  uns len = (uns) (cp->siz() / sizeof(long));
  long *lptr = (long *) cp->ex();
  Graphic->gsize = 7;
  Graphic->gparm = new short[4];
  if ((lptr[4] == 0) || (lptr[4] == 18000)) {
    Graphic->gparm[0] = GetTwips(lptr[1] + lptr[3], pg_unit);
    Graphic->gparm[1] = GetTwips(lptr[0] + lptr[2], pg_unit);
    Graphic->gparm[2] = GetTwips(lptr[1] - lptr[3], pg_unit);
    Graphic->gparm[3] = GetTwips(lptr[0] - lptr[2], pg_unit);
  }
  else if ((lptr[4] == 9000) || (lptr[4] == 27000)) {
    Graphic->gparm[0] = GetTwips(lptr[1] + lptr[2], pg_unit);
    Graphic->gparm[1] = GetTwips(lptr[0] + lptr[3], pg_unit);
    Graphic->gparm[2] = GetTwips(lptr[1] - lptr[2], pg_unit);
    Graphic->gparm[3] = GetTwips(lptr[0] - lptr[3], pg_unit);
  }
	Graphic->gypos = Graphic->gparm[0];
}


void
RTFwr::ProcGrArc(RTFctl *cp)
{
  long *lptr = (long *) cp->ex();
	long centx = lptr[0];
	long centy = lptr[1];
	long major = lptr[2];
	long minor = lptr[3];
	long roth = lptr[4];
	long begh = lptr[5];
	long endh = lptr[6];

  Graphic->gsize = 11;
  Graphic->gparm = new short[8];

  // convert start and end angles to points on arc from center
  double endang = (((double) endh) * PI) / 18000.0;
  double begang = (((double) begh) * PI) / 18000.0;

  long endy, endx, begy, begx;

  if ((roth == 0) || (roth == 18000)) {	 // major parallel to x axis
		// scale the y values to minor
		endy = (long) ceil(sin(endang) * (double) minor);
		endx = (long) ceil(cos(endang) * (double) major);
		begy = (long) ceil(sin(begang) * (double) minor);
		begx = (long) ceil(cos(begang) * (double) major);
    Graphic->gparm[4] = GetTwips(centy + minor, pg_unit);	// bottom
    Graphic->gparm[5] = GetTwips(centx - major, pg_unit);	// right
    Graphic->gparm[6] = GetTwips(centy - minor, pg_unit);	// top
    Graphic->gparm[7] = GetTwips(centx + major, pg_unit);	// left
		CurrGrArc.lenx = GetTwips(major, pg_unit);
		CurrGrArc.leny = GetTwips(minor, pg_unit);
  }
  else if ((roth == 9000) || (roth == 27000)) {	 // major parallel to y axis
		// scale the x values to minor
		endy = (long) ceil(sin(endang) * (double) major);
		endx = (long) ceil(cos(endang) * (double) minor);
		begy = (long) ceil(sin(begang) * (double) major);
		begx = (long) ceil(cos(begang) * (double) minor);
    Graphic->gparm[4] = GetTwips(centy + major, pg_unit); // bottom
    Graphic->gparm[5] = GetTwips(centx - minor, pg_unit);	// right
    Graphic->gparm[6] = GetTwips(centy - major, pg_unit);	// top
    Graphic->gparm[7] = GetTwips(centx + minor, pg_unit);	// left
		CurrGrArc.lenx = GetTwips(minor, pg_unit);
		CurrGrArc.leny = GetTwips(major, pg_unit);
  }
  Graphic->gparm[0] = GetTwips(centy - endy, pg_unit);
  Graphic->gparm[1] = GetTwips(centx + endx, pg_unit);
  Graphic->gparm[2] = GetTwips(centy - begy, pg_unit);
  Graphic->gparm[3] = GetTwips(centx + begx, pg_unit);

	CurrGrArc.cx = GetTwips(centx, pg_unit);
	CurrGrArc.cy = GetTwips(centy, pg_unit);
	CurrGrArc.hang = begang;
	CurrGrArc.tang = endang;
	Graphic->gypos = Graphic->gparm[4];
}


uns RTFwr::BezierPts;
uns RTFwr::BezPtSpace;
long RTFwr::BezDistMax;


void
RTFwr::ProcGrBezier(RTFctl *cp)
{
  BezDistMax = GetOrig(5L, pg_unit); // flatness measure for 1 pixel at 300dpi

  uns bezct = (uns) (((cp->siz() / sizeof(long)) - 2) / 6);
  long *lptr = (long *) cp->ex();
  Graphic->gparm = (short *) NewMem(((BezPtSpace = bezct * 4) * 2) + 2);
  BezierPts = 0;
  long bezpt[8];

  for (uns bez = 0; bez < bezct; bez++) {
    for (uns i = 0; i < 8; i++)
      bezpt[i] = lptr[i];
    lptr += 6;
    ProcBez(bezpt);
  }

  Graphic->gparm[++BezierPts] = GetTwips(bezpt[6], pg_unit);
  Graphic->gparm[++BezierPts] = GetTwips(bezpt[7], pg_unit);
  Graphic->gparm = (short *) ResizeMem(Graphic->gparm, (BezierPts * 2) + 2);
  Graphic->gsize = 4 + BezierPts;
  Graphic->gparm[0] = BezierPts / 2;
	Graphic->gypos = Graphic->gparm[1];
}


void
RTFwr::ProcBez(long *bezpt)
{
  if (BezFlat(bezpt)) {
    Graphic->gparm[++BezierPts] = GetTwips(bezpt[0], pg_unit);
    Graphic->gparm[++BezierPts] = GetTwips(bezpt[1], pg_unit);
    if (BezierPts > (BezPtSpace - 4)) {
      BezPtSpace *= 2;
      Graphic->gparm = (short *) ResizeMem(Graphic->gparm, (BezPtSpace * 2) + 2);
    }
    return;
  }

  // split bez into left and right
  long lbez[8], rbez[8];
  lbez[0] = bezpt[0];
  lbez[1] = bezpt[1];
  lbez[2] = (bezpt[0] + bezpt[2]) / 2;
  lbez[3] = (bezpt[1] + bezpt[3]) / 2;
  rbez[4] = (bezpt[4] + bezpt[6]) / 2;
  rbez[5] = (bezpt[5] + bezpt[7]) / 2;
  rbez[6] = bezpt[6];
  rbez[7] = bezpt[7];

  long topx = (bezpt[2] + bezpt[4]) / 2;
  long topy = (bezpt[3] + bezpt[5]) / 2;
  lbez[4] = (lbez[2] + topx) / 2;
  lbez[5] = (lbez[3] + topy) / 2;
  rbez[2] = (rbez[4] + topx) / 2;
  rbez[3] = (rbez[5] + topy) / 2;

  lbez[6] = rbez[0] = (lbez[4] + rbez[2]) / 2;
  lbez[7] = rbez[1] = (lbez[5] + rbez[3]) / 2;

  // recursively process left then right
  ProcBez(lbez);
  ProcBez(rbez);
}


bool
RTFwr::BezFlat(long *bezpt)
{
  // true if ctl pts (2,3) and (4,5) are within 5 twips (one 300 dpi pixel)
  // of line connecting endpoints (0,1) and (6,7) along either x or y axis

  long nbez[6];
  nbez[0] = bezpt[2] - bezpt[0];
  nbez[1] = bezpt[3] - bezpt[1];
  nbez[2] = bezpt[4] - bezpt[0];
  nbez[3] = bezpt[5] - bezpt[1];
  nbez[4] = bezpt[6] - bezpt[0];
  nbez[5] = bezpt[7] - bezpt[1];

  if ((abs(nbez[4]) < BezDistMax) && (abs(nbez[5]) < BezDistMax))
    return true;

  if (nbez[4] == 0) 
    nbez[4] = 1;
  if (nbez[5] == 0)
    nbez[5] = 1;

  if (((abs(nbez[0] - ((nbez[1] * nbez[4]) / nbez[5])) < BezDistMax)
    || (abs(nbez[1] - ((nbez[0] * nbez[5]) / nbez[4])) < BezDistMax))
   && ((abs(nbez[2] - ((nbez[3] * nbez[4]) / nbez[5])) < BezDistMax)
    || (abs(nbez[3] - ((nbez[2] * nbez[5]) / nbez[4])) < BezDistMax)))
    return true;
  return false;
}




// graphic writing functions

long RTFwr::WMFfsize;
long RTFwr::WMFrecmax;
short RTFwr::WMFobjmax;
long RTFwr::WMFdiv;
long RTFwr::WMFprevdiv;
short RTFwr::WMFxoff;
short RTFwr::WMFyoff;

void
RTFwr::UpdateWMFHead(long gsize)
{
   WMFfsize += gsize;
   if (gsize > WMFrecmax)
     WMFrecmax = gsize;
}


void
RTFwr::WriteWMFFrame(RTFwfile *wf, frame *fr, short xoff, short yoff)
{
  frame *nf;
  graph *gr;

	WMFxoff = xoff;
	WMFyoff = yoff;


	for (uns i = 1; i <= fr->grcount; i++) {
		if ((gr = (graph *) ((fr->grlist)->find(i))) != NULL) {
			if (gr->gtype == 0)	{ // recurse for embedded frames
				nf = (frame *) gr->gparm;
				if (nf->ftext) {
					CurrPen = *(gr->gpen);
					CurrBrush = *(gr->gbrush);
					ConvertTextFrame(nf, fr);
					nf->ftext = 0;
        }
				else if (nf->grfile)
					ConvertImportObjectFrame(nf, fr, gr);
				FrameLevel++;
				WriteWMFFrame(wf, nf, xoff + nf->xpos, yoff + nf->ypos);
				FrameLevel--;
				WMFxoff = xoff; // restore our origin
				WMFyoff = yoff;
			}
			else if (gr->gtype == 1) { // embedded WMF
#if 0
				if (memcmp(&WhitePen, &LastPen, sizeof(wmf_pen))) {
					WriteWMFDel(wf, 0);
					WriteWMFPen(wf, &WhitePen);
					LastPen = WhitePen;
				}
				if (memcmp(&WhiteBrush, &LastBrush, sizeof(wmf_brush))) {
					WriteWMFDel(wf, 1);
					WriteWMFBrush(wf, &WhiteBrush);
					LastBrush = WhiteBrush;
				}
#else
				if (gr->gpen && memcmp(gr->gpen, &LastPen, sizeof(wmf_pen))) {
					WriteWMFDel(wf, 0);
					WriteWMFPen(wf, gr->gpen);
					LastPen = *(gr->gpen);
				}
				if (gr->gbrush && memcmp(gr->gbrush, &LastBrush, sizeof(wmf_brush))) {
					WriteWMFDel(wf, 1);
					WriteWMFBrush(wf, gr->gbrush);
					LastBrush = *(gr->gbrush);
				}
#endif
		    WriteGrMetafile(wf, (wmeta *) gr->gparm, fr);
			}
			else if ((gr->gtype == 0xF43) // raster
			 && ((Raster = (rast *) gr->gparm) != NULL)) {
				if (fr->frot  // rotated raster, see if corrected externally
				 && (((Raster->phigh > Raster->pwide)
				   && (fr->high < fr->wide))
				  || ((Raster->phigh < Raster->pwide)
				   && (fr->high > fr->wide)))) {
					short diff = fr->high - fr->wide;
					Raster->ihigh = (fr->high -= diff);
					Raster->iwide = (fr->wide += diff);
					diff /= 2;
					Raster->ixpos = (fr->xpos -= diff) - WMFxoff;
					Raster->iypos = (fr->ypos += diff) - WMFyoff;
				}
				if ((FrameLevel == 1)  // top level
				 && (fr->grcount == 1) // only graphic in frame
				 && ((BitmapDPI = Raster->dpi) != 0)) {  // rescaling requested
					Raster->ihigh = fr->high = (Raster->phigh * 1440L) / Raster->dpi; 
					Raster->iwide = fr->wide = (Raster->pwide * 1440L) / Raster->dpi;
					WMFyoff = Raster->iypos = fr->ypos = 0;  // reorg to top left
					WMFxoff = Raster->ixpos = fr->xpos = 0;
				}
				WriteGraphic(wf, gr);
			}
			else	// plain graphic object
				WriteGraphic(wf, gr);
		}
	}

	if ((FrBorders != 'N')
	 && (FrameLevel == 1)
	 && fr->frpen
	 && (fr->frpen->pstyle != 5)
	 && (fr->frpen->pstyle != 255)) { // draw border after image
		short wid = 0;
		if (FrBorders == 'I')  // inside
			wid = (short) gr->gpen->pwidth / 2; // inside image bounds, to get even
		else if (FrBorders == 'O')  // outside
			wid = - (short) (gr->gpen->pwidth / 2);
		// else FrBorders == 'F', centered
		graph *frb = new graph;
		frb->gpen = fr->frpen;
		//frb->gbrush = fr->frbrush;
		frb->gbrush = &HollowBrush;
		frb->gtype = 0x41B;
		frb->gsize = 7;
		frb->gparm = new short[4];
		frb->gparm[0] = frb->gypos = yoff + wid;
		frb->gparm[1] = xoff + wid;
		frb->gparm[2] = fr->high + yoff - wid;
		frb->gparm[3] = fr->wide + xoff - wid;
		WriteGraphic(wf, frb);
		delete [] frb->gparm;
		delete frb;
	}
}


void
RTFwr::WriteGraphic(RTFwfile *wf, graph *gr)
{
  if (gr->gpen && memcmp(gr->gpen, &LastPen, sizeof(wmf_pen))) {
    WriteWMFDel(wf, 0);
    WriteWMFPen(wf, gr->gpen);
		LastPen = *(gr->gpen);
  }
  if (gr->gbrush && memcmp(gr->gbrush, &LastBrush, sizeof(wmf_brush))) {
    WriteWMFDel(wf, 1);
    WriteWMFBrush(wf, gr->gbrush);
		LastBrush = *(gr->gbrush);
  }
  if (gr->gfont && memcmp(gr->gfont, &LastFont, sizeof(wmf_font))) {
    WriteWMFDel(wf, 2);
    WriteWMFFont(wf, gr->gfont);
		LastFont = *(gr->gfont);
  }
	if (gr->gtype == 0x521) {
		if (gr->gfont) {
			uns colr = (gr->gfont)->fcolr;
			if (colr == Invisible)
				return;
			if (colr > 1)
				WriteGrTextColor(wf, colr - 2);
			else
				WriteGrTextColor(wf, 0);  // black is default
		}
		else
			WriteGrTextColor(wf, 0);  // black is default
		WriteGrTextAlign(wf, ((gtext *) gr->gparm)->gform);
	}

  wf->putMEnd();
  wf->putML(gr->gsize);  // record length
	UpdateWMFHead(gr->gsize);
  wf->putMS(gr->gtype);  // record function

	unl start = 0;
	unl finish = gr->gsize - 3;
	short wid = 0;
	rast* ras = NULL;
	graph* rasborder = NULL;

  switch (gr->gtype) {
	 default:		  // require listing of type here!
		assert(0);
		break;
	 case 0xF43:  // raster
    WriteGrRaster(wf, (rast *) gr->gparm);
		if ((RasBorders != 'N')
		 && gr->gpen
		 && (gr->gpen->pstyle != 255)) { // draw border after image
			if (RasBorders == 'I')  // inside
				wid = (short) gr->gpen->pwidth / 2; // inside image bounds, to get even
			else if (RasBorders == 'O')  // outside
				wid = - (short) (gr->gpen->pwidth / 2);
			// else RasBorders == 'F', centered
			ras = (rast *) gr->gparm;
			rasborder = new graph;
			rasborder->gbrush = &HollowBrush;
			rasborder->gtype = 0x41B;
			rasborder->gsize = 7;
			rasborder->gparm = new short[4];
			rasborder->gparm[0] = rasborder->gypos = ras->iypos + wid;
			rasborder->gparm[1] = ras->ixpos + wid;
			rasborder->gparm[2] = ras->ihigh + ras->ixpos - wid;
			rasborder->gparm[3] = ras->iwide + ras->iypos - wid;
			WriteGraphic(wf, rasborder);
			delete [] rasborder->gparm;
			delete rasborder;
		}
		break;
   case 0x521:	// text
    WriteGrText(wf, (gtext *) gr->gparm);
		break;
	 case 0x324:	// polygon
   case 0x325:  // polyline
		start = 1;  // skip count value, then X,Y xN
    wf->putMS(gr->gparm[0]);
		goto normal;
	 case 0x61C:	// round rect
		start = 2;  // skip arc radii, then Y,X	x2
    wf->putMS(gr->gparm[0]);
    wf->putMS(gr->gparm[1]);
		goto normal;
	 case 0x41B:  // rectangle, Y,X	x2
	 case 0x418:  // ellipse,   Y,X x2
	 case 0x817:  // elliptic arc, Y,X x4
   case 0x81A:  // elliptic pie
   case 0x830:  // elliptic chord
	 normal:      // all are Y,X for full length
    for (unl i = start; i < finish; i++) {
			// X offsets always at odd positions, Y at even
      gr->gparm[i] += (i & 1) ? WMFxoff : WMFyoff;
      wf->putMS(gr->gparm[i]);
		}
		break;
  }
}


void
RTFwr::SetWMFStart(RTFwfile *wf, frame *fr, short rcrop)
{
  wf->putGS();
	if (MetaType != 'E') {
		wf->putCW("pict");
		wf->putCW("wmetafile8");  // anisotropic mapping mode
		if (MetaMetric) {  // which assumes values are himetric not twips
			wf->putCWN("picscalex", PicScale);
			wf->putCWN("picscaley", PicScale);
		}
		wf->putCWN("picw", fr->wide);
		wf->putCWN("pich", fr->high);
		wf->putCWN("picwgoal", fr->wide);
		wf->putCWN("pichgoal", fr->high);
		if (rcrop)
			wf->putCWN("piccropr", rcrop, true);
		wf->putEnd();
	}

	WMFfsize = 103; // size of normal head, 9 + 23 + 16 + 12 + 11 + 32
  WMFrecmax = 28; // font in header
	WMFobjmax = 3;  // one pen, brush, and font
  WMFxoff = 0;
  WMFyoff = 0;

	LastPen = DefaultPen;     // values set in header
  LastBrush = DefaultBrush;
  LastFont = DefaultFont;

  // now ready for use of diversion for WMF content
	WMFprevdiv = wf->GetDiv();
	wf->startMeta(MetaType, MetaNameWid, MetaNumWid);
	WMFdiv = wf->NewMDiv();
}


void
RTFwr::WriteWMFStart(RTFwfile *wf, frame *fr, short bmode, short smode)
{
	uns cksum;
	bool orgext = true;

  // diversion is completed, write out real start

	if (GrBkMode)
		bmode = GrBkMode;  // override default if specified

	if (fr->stretch)
		smode = fr->stretch;

	if (GrStretchMode)
		smode = GrStretchMode;

  // metafile header
	if (MetaType == 'E') {    // write placeable metafile header
		cksum = 0;
		wf->putML(0x9AC6CDD7);  // magic number
		cksum ^= 0x9AC6;
		cksum ^= 0xCDD7;
		wf->putMS(0);
		cksum ^= 0;
		wf->putMS(0);          // left
		cksum ^= 0;
		wf->putMS(0);          // top
		cksum ^= 0;
		wf->putMS(fr->wide);   // right
		cksum ^= fr->wide;
		wf->putMS(fr->high);   // bottom
		cksum ^= fr->high;
		wf->putMS(1440);       // twips per inch
		cksum ^= 1440;
		wf->putML(0L);
		cksum ^= 0;
		cksum ^= 0;
		wf->putMS(cksum);
		if (OutType != 'H') {  // if not for Help use,
			orgext = false;
			WMFfsize -= 10;  // remove SetWindowOrg/Ext
		}
	}

  wf->putMS(1); // disk metafile
  wf->putMS(9); // header length
  wf->putMS(0x300);  // version for Windows 3.x
  wf->putML(WMFfsize);  // metafile size
  wf->putMS(WMFobjmax); // max object count: one pen, brush, and font
//  wf->putMS(4); // max object count: one pen, brush, font, and region
  wf->putML(WMFrecmax);  // max record size
  wf->putMS(0); // unused

  wf->putMEnd();
  wf->putML(3);      // record length
  wf->putMS(0x01E);  // record function, save DC

	if (orgext) {	// not used in non-Help placeable metafile
		wf->putMEnd();
		wf->putML(5);      // record length
		wf->putMS(0x20B);  // record function
		wf->putMS(0);      // window org y
		wf->putMS(0);      // window org x

		wf->putMEnd();
		wf->putML(5);      // record length
		wf->putMS(0x20C);  // record function
		wf->putMS(fr->high);  // window ext y
		wf->putMS(fr->wide);  // window ext x
	}

  wf->putMEnd();
  wf->putML(5);      // record length
  wf->putMS(0x209);  // record function
  wf->putML(0x00);   // text color black

  wf->putMEnd();
  wf->putML(5);      // record length
  wf->putMS(0x201);  // record function
  wf->putML(0x00FFFFFF);   // background color white

  wf->putMEnd();
  wf->putML(4);      // record length
  wf->putMS(0x106);  // record function
  wf->putMS(1);      // poly fill mode = alternate

  wf->putMEnd();
  wf->putML(4);      // record length
  wf->putMS(0x102);  // record function
  wf->putMS(bmode);  // background mode, 1 = trans, 2 = fill

  wf->putMEnd();
  wf->putML(4);      // record length
  wf->putMS(0x104);  // record function
  wf->putMS(13);     // ROP2 mode = overlay

  wf->putMEnd();
  wf->putML(4);      // record length
  wf->putMS(0x107);  // record function
  wf->putMS(smode);  // stretch blt mode, 1 = black, 2 = white, 3 = color

  WriteWMFPen(wf, &DefaultPen);

	switch (DefBrushType) {
		case 'H':
			DefaultBrush = HollowBrush;
			break;
		case 'W':
			DefaultBrush = WhiteBrush;
			break;
		case 'B':
		default:
			DefaultBrush = BlackBrush;
			break;
	}

  WriteWMFBrush(wf, &DefaultBrush);
  WriteWMFFont(wf, &DefaultFont);
//	WriteWMFRegion(wf, fr);

  // write out diversion now
	wf->WriteMDiv(WMFdiv);
}


void
RTFwr::WriteWMFEnd(RTFwfile *wf)
{
  // include in diversion to get size right in head

  WriteWMFDel(wf, 0);
  WriteWMFDel(wf, 1);
  WriteWMFDel(wf, 2);
//  WriteWMFDel(wf, 3);

  wf->putMEnd();
  wf->putML(4);      // record length
	UpdateWMFHead(4);
  wf->putMS(0x127);  // record function, restore DC
  wf->putMS(-1); // last DC, -1

  wf->putMEnd();
  wf->putML(3);  // terminal record length
	UpdateWMFHead(3);
  wf->putMS(0);  // terminal record function 0

  wf->SelectMDiv(0); // end the diversion now
	if (MetaType != 'E')
		wf->SelectDiv(WMFprevdiv);  // ensure correct pos if embedded
}


void
RTFwr::SetWMFEnd(RTFwfile *wf, frame *fr, char *bmstr)
{
  // diversion has been written, ok to close file now
	wf->DeleteMDiv(WMFdiv);
	wf->endMeta();
	wf->SelectDiv(WMFprevdiv);

	if (MetaType == 'E') {
		char *fname = NewName(wf->getMetaName());
		char *suf = NULL;
		char *bcp = NULL;

		if (NameWMFsAsBMPs
		 && ((suf = strrchr(fname, '.')) != NULL)
		 && !stricmp(suf + 1, "wmf"))
			strcpy(suf + 1, "bmp");

		if (OutType == 'H') {
			WriteWMFRefs(wf);
			wf->putCS('{');
			wf->putStr(bmstr ? bmstr : BmlStr);
			//wf->putMetaName();
			bcp = BmxFileName(fname);
			wf->putStr(bcp);
			delete [] bcp;
			wf->putCS('}');
		}
		else {
			//wf->putStr("Graphic file: ");
			//wf->putMetaName();
			//wf->putField("INCLUDEPICTURE", wf->getMetaName(), true, "\\\\d ");
			wf->putField("INCLUDEPICTURE", fname, true, "\\\\d ");
		}
		DeleteName(fname);
	}
  wf->putGE();
  wf->putEnd();
}


bool RTFwr::WMFRefUsed = false;
DCnlist RTFwr::WMFRefList;


void
RTFwr::WriteWMFRefs(RTFwfile *wf)
{
	if (WMFRefUsed == true)	{
		WMFRefList.writeall(wf, WriteWMFRef);
		WMFRefList.empty();
		WMFRefUsed = false;
	}
}


void
RTFwr::WriteWMFRef(DCwfile *w, char *txt, long num)
{
	if (num == 0)
		return;

	RTFwfile *wf = (RTFwfile *) w;
	FtnMarker = '#';
	WriteFtnStart(wf);
	WriteFtnNum(wf);
	if (num == 1)
		wf->putStr(txt);
	else if (num == 2)
		PutFtnContext(wf, txt);
	WriteFtnEnd(wf);
	wf->putEnd();
}


wmf_pen RTFwr::DefaultPen = { 0, 0, 0x00000000 }; // solid black, min thick
wmf_pen RTFwr::WhitePen = { 0, 0, 0x00FFFFFF }; // solid white, min thick
wmf_pen RTFwr::CurrPen;
wmf_pen RTFwr::LastPen;

void
RTFwr::WriteWMFPen(RTFwfile *wf, wmf_pen *pen)
{
	short psty = pen->pstyle;
	short pwid = pen->pwidth;
	unl pcol = pen->pcolor;
	short lsty = pen->lstyle;

	if (psty == 255) { // background, used as whiteout
		psty = 0;
		pcol = 0xFFFFFF;
	}
	else if ((psty != 5)
	 && (lsty > 0)) // dashed spec overrides tint
		psty = lsty;

	if ((psty > 0) && (psty < 5)) // dashed or dotted
		pwid = 1; // thinner if too thick for dashed

  wf->putMEnd();
  wf->putML(8);      // record length
	UpdateWMFHead(8);
  wf->putMS(0x2FA);  // record function
	wf->putMS(psty);
  wf->putMS(pwid);
  wf->putMS(pwid);
  wf->putML(pcol);

  WriteWMFSel(wf, 0); // select as object 0
  CurrPen = *pen;
}

char RTFwr::DefBrushType = 'B';
wmf_brush RTFwr::DefaultBrush = { 0, 0, 0x00000000, 0 };  // solid black
wmf_brush RTFwr::BlackBrush = { 0, 0, 0x00000000, 0 };  // solid black
wmf_brush RTFwr::WhiteBrush = { 0, 0, 0x00FFFFFF, 0 };  // solid white
wmf_brush RTFwr::HollowBrush = { 1, 0, 0x00000000, 0 };  // hollow, empty
wmf_brush RTFwr::CurrBrush;
wmf_brush RTFwr::LastBrush;

void
RTFwr::WriteWMFBrush(RTFwfile *wf, wmf_brush *br)
{
  wf->putMEnd();
  wf->putML(7);      // record length
	UpdateWMFHead(7);
  wf->putMS(0x2FC);  // record function

  wf->putMS(br->bstyle);

  if (br->bshade == 0)         // solid
    wf->putML(br->bcolor);
  else if (br->bshade == 255)  // whited out
    wf->putML(0x00FFFFFF);
  else wf->putML(ShadeColor(br->bcolor, br->bshade));

  wf->putMS(br->bhatch);

  WriteWMFSel(wf, 1); // select as object 1
  CurrBrush = *br;
}


long
RTFwr::ShadeColor(long colr, uns shd)
{
  long scolr = 0;
  long ncolr;

  for (uns i = 0; i < 3; i++) {
    ncolr = ((((colr >> (8 * i)) & 0xFF) * (256 - shd)) / 256) + shd;
    scolr += (ncolr > 255 ? 255 : ncolr) << (8 * i);
  }
  return scolr;
}


#if 0
void
RTFwr::WriteWMFRegion(RTFwfile *wf, frame *fr)
{
  wf->putMEnd();
	wf->putML(7);      // record length
	UpdateWMFHead(7);
	wf->putMS(0x6FF);  // record function
 	wf->putMS(fr->high);  // window ext y
	wf->putMS(fr->wide);  // window ext x
	wf->putMS(0);      // window org y
	wf->putMS(0);      // window org x

	// select as clip region
  wf->putMEnd();
  wf->putML(4);      // record length
	UpdateWMFHead(4);
  wf->putMS(0x12C);  // record function
  wf->putMS(3);      // select as object 3
}
#endif


void
RTFwr::WriteWMFSel(RTFwfile *wf, uns num)
{
  wf->putMEnd();
  wf->putML(4);      // record length
	UpdateWMFHead(4);
  wf->putMS(0x12D);  // record function
  wf->putMS(num);    // select object
}


void
RTFwr::WriteWMFDel(RTFwfile *wf, uns num)
{
  wf->putMEnd();
  wf->putML(4);      // record length
	UpdateWMFHead(4);
  wf->putMS(0x1F0);  // record function
  wf->putMS(num);    // delete object
}



// end of dwrtfgr.cpp


