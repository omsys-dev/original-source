 
// dwhtmgr.cc is the code for dcl Microsoft HTM graphic writing, JHG, 1/4/96
// Copyright (c) 1996 by Omni Systems, Inc.  All rights reserved.

#include "dcappl.h"
#include "dcctlg.h"
#include "dclist.h"
#include "dcfili.h"
#include "dctxfx.h"
#include "dcfmtr.h"
#include "dcini.h"
#include "dcmacr.h"
#include "dcauto.h"
#include "dwhtm.h"
#include "dwhtmfl.h"



// processing switch


bool
HTMwr::ProcGr(HTMctl *cp)
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
    case fredef:
      EndFrame(cp);
      break;

    case franch:
      Frame->fanchor = cp->dat();
      Frame->fapage = (uns) cp->siz();
      break;

    case frattr:
			if (InFormDefPfx && (cp->dat() == 4))
				Curr.pfx = (uns) cp->siz();
			else if (InFormDefSfx && (cp->dat() == 4))
				Curr.sfx = (uns) cp->siz();
			else
				ProcFrAttr(cp);
      break;

    case frsize:
      ProcFrSize(cp);
      break;

		case frdpi:
			if (Frame && UseOtherpropsDpi)
				Frame->fmdpi = cp->siz();
			break;

#ifdef UNUSED
    case frline:
      switch (cp->dat()) {
        case 2: // color
          GetColor(cp, &Frame->fcolr);
          break;
        case 3: // thickness
          Frame->fthck = GetTwips(cp->siz(), gl_unit);
          break;
      }
      break;
    case frpen:
      if (cp->dat() == 1)
        Frame->frvis = cp->siz() ? 1 : 0;
      break;
    case frfill:
      if (cp->dat() == 1)
        GetFill(cp, &Frame->ffill);
      break;
#endif

		case frprev:  // graphic of full frame
			if (UseLocalFileID
			 || !_strnicmp(CurrFilePrefix, (char *) cp->ex(), CurrPxLen))
				RFrame->grname = NewName((char *) cp->ex(), cp->siz() + 1);
			else {
				RFrame->grname = NewName(CurrFilePrefix, cp->siz() + CurrPxLen + 1);
				strncat(RFrame->grname, (char *) cp->ex(), cp->siz());
			}
			RFrame->prtype = cp->dat();
			//StartingFrame = false;
      break;

		case frprop:
			FramePropName = NULL;
			FramePropValue = NULL;
			break;
		case frpname:
			FramePropName = (char *) cp->ex();
			break;
		case frpval:
			if (FramePropValue) {
				char *np = NewName(FramePropValue,
				                   strlen(FramePropValue) + cp->siz() + 2);
				strcat(np, (char *) cp->ex());
				FramePropValue = np;
			}
			else
				FramePropValue = (char *) cp->ex();
			break;
		case freprop:
			if (!stricmp(FramePropName, "Alt"))
				RFrame->alt = FramePropValue;
			else if (!stricmp(FramePropName, "ActualText"))
				RFrame->longdesc = FramePropValue;
			else if (!_strnicmp(FramePropName, "Config", 6)
				|| !_strnicmp(FramePropName, "HTMConfig", 9)) {
				setting *s = DCini::ParseDirective(FramePropValue);
				if (s) {
					if (!stricmp(s->sect, "GraphGroup")
					 && s->val)
						RFrame->grgroup = NewName(s->val);
					else {
						if (!RFrame->cfglist)
							RFrame->cfglist = new DClist;
						RFrame->cfglist->add((long) s);
					}
				}
			}
			else if (!stricmp(FramePropName, "GraphGroup"))
				RFrame->grgroup = FramePropValue;
			else if (!stricmp(FramePropName, "GraphClass"))
				RFrame->gclass = FramePropValue;
			else if (!strnicmp(FramePropName, "Graph", 5)) {
				size_t len = strlen(FramePropName + 5);
				char *astr = NewName(strlwr(FramePropName + 5), 
					                   len + strlen(FramePropValue) + 4);
				astr[len++] = '=';
				astr[len++] = '"';
				astr[len] = '\0';
				strcpy(&astr[len], FramePropValue);
				strcat(astr, "\"");
				if (!RFrame->proplist)
					RFrame->proplist = new DClist;
				RFrame->proplist->add((long) astr);
			}
			break;

    case frtext:
      Frame->ftext = cp->dat();
      Frame->ftnext = (uns) cp->siz();
      break;

#ifdef UNUSED
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
#endif

    case setanch:  // anchor in text for frame
			ImageRefNum = (long) cp;
			ProcCondImage();
      ProcFrAnchor(cp);
      return true;

		case eqform:
			if (InGraphic
			 && RFrame
			 && !RFrame->math)
				RFrame->math = (char *) cp->ex();
			break;

		case grfile:
			if (InGraphic
			 && RFrame
			 && !RFrame->grfile)
				RFrame->grfile = (char *) cp->ex();
			break;

    case grgroup:
			if (RFrame)
				RFrame->grcount++;
      break;
    case gregroup:
      break;

    case grobj:
			InGraphic = true;
			InImpObj = (cp->dat() == 6) ? true : false;
			if (RFrame) {
				RFrame->grcount++;
				if (cp->dat() == 3)
					RFrame->mathobj = true;
			}
      break;
    case greobj:
			InGraphic = InImpObj = false;
      break;

    case grtext:
			if (RFrame)
				RFrame->grcount++;
      break;


		case imagemap:
			ProcImageMapStart();
      break;
		case endimap:
			ProcImageMapEnd();
      break;
		case imapprop:
			ProcImageMapProp(cp);
      break;
		case maparea:
			ProcImageMapAreaStart();
      break;
		case endmapa:
			ProcImageMapAreaEnd();
      break;
		case mapaprop:
			ProcImageMapAreaProp(cp);
      break;
		case mapshape:
			ProcImageMapAreaShape(cp);
      break;


#ifdef UNUSED
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
    case formchk:
      ProcGrTProp(cp);
      break;
    case flowused:
      ProcGrTStr(cp);
      break;
#endif

    default:
      break;
  }
  return false;  // default is to discard ctl after scan
}



// page properties


uns HTMwr::PaperWide = 12240;
uns HTMwr::PaperHigh = 15840;
uns HTMwr::LMarg = 1440;
uns HTMwr::RMarg = 1440;
uns HTMwr::TMarg = 1440;
uns HTMwr::BMarg = 1440;

page *HTMwr::Page;
DCvlist HTMwr::PageList;
uns HTMwr::LastPage;
uns HTMwr::LastMasterPage;
bool HTMwr::StartingPage;

bool HTMwr::NewBPage;
uns HTMwr::LastBPage;
uns HTMwr::PFrameCount;


void
HTMwr::ProcPage(HTMctl *cp)
{
  Page = new page;
  Page->ptype = 0;
  Page->plmrg = Page->prmrg = Page->ptmrg = Page->pbmrg = 1440;
  LastPage = Page->pnum = cp->dat();
  PageList.add(Page, Page->pnum);
  StartingPage = true;
  NewBPage = false;
	StartingFrame = false;
  PFrameCount = 0;
}


void
HTMwr::ProcPgEnd(HTMctl *cp)
{
  if (PFrameCount == 1) {
    Page->pcols = 1;
    ProcPgBody(BodyFrameList.find(LastPage));
  }
  else if (PFrameCount >= 2) {
    Page->pcols = 0;
    ProcPgBody(BodyFrameList.find(LastPage));   // first column
    BodyFrameList.execu(LastPage, ProcPgCols);  // other columns
  }

  LMarg = Page->plmrg;
  RMarg = Page->prmrg;
  TMarg = Page->ptmrg;
  BMarg = Page->pbmrg;

  while (FrameLevel) {
    FrameLevel--;
    FrameStack[FrameLevel] = NULL;
  }

	if (XMLSource) {  // end of page is end of DITA topic
		SearchHere = true;
		PrintHere = true;
		PrintOnly = false;
	}
}


void
HTMwr::ProcPgBody(void *vp)
{
	if (!vp)
		return;
  frame *fr = (frame *) vp;
  Page->plmrg = fr->xpos;
  Page->prmrg = Page->pwide - (fr->xpos + fr->wide);
  Page->ptmrg = fr->ypos;
  Page->pbmrg = Page->phigh - (fr->ypos + fr->high);
}


void
HTMwr::ProcPgCols(void *vp)
{
  frame *fr = (frame *) vp;
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
HTMwr::ProcPgAttr(HTMctl *cp)
{
  switch (cp->dat()) {
    case 2: // page usage
      Page->pusage = (uns) cp->siz();
      break;
    case 3: // master type
      if ((Page->ptype = (uns) cp->siz()) > 0)
        LastMasterPage = LastPage;
      break;
    case 4: // background
      if (LastBPage != (Page->pback = (uns) cp->siz())) // every pg if doublesided
        NewBPage = true;  // implies new section, different headers/footers
      LastBPage = Page->pback;
      break;
    case 5: // orient
      Page->porient = (uns) cp->siz();
      break;
    case 6: // hand
      Page->phand = (uns) cp->siz();
      break;
    default:
      break;
  }
}


void
HTMwr::ProcPgSize(HTMctl *cp)
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


uns HTMwr::DefCols = 1;
dcframe *HTMwr::Frame;
frame *HTMwr::RFrame;
DCvlist HTMwr::FrameList;
DCvlist HTMwr::FramePageList;
uns HTMwr::LastFrame;
DClist HTMwr::HeaderList;
DClist HTMwr::FooterList;
DClist HTMwr::DocBodyList;
DCvlist HTMwr::BodyFrameList;
DCvlist HTMwr::GrTextFrameList;
bool HTMwr::CopyOriginalGraphics = false;
DCnlist HTMwr::GraphFileCopyList;
DCnlist HTMwr::GraphFileFixedCopyList;
long HTMwr::GraphCopyCount = 0;
frame *HTMwr::GrTextFrame;
frame *HTMwr::FrameStack[FrameStackMax];
uns HTMwr::FrameLevel;
bool HTMwr::IncludeMaps = false;
bool HTMwr::StartingFrame = false;
bool HTMwr::UseGraphicPreviews = false;
bool HTMwr::StripGraphPath = false;
bool HTMwr::KeepGraphicsInPara = false;
bool HTMwr::InGraphicText = false;
bool HTMwr::UseOrigGrName = false;
bool HTMwr::FixGraphSpaces = true;
char *HTMwr::GraphSuffix = NULL;
char *HTMwr::GraphPath = NULL;
int HTMwr::FrIDLen = 6;
char *HTMwr::FramePropName = NULL;
char *HTMwr::FramePropValue = NULL;


void
HTMwr::ProcFrame(HTMctl *cp)
{
  if ((((short) FrameLevel) > 0)
   && (FrameStack[FrameLevel - 1] == NULL))
    ProcEndFrame();
  FrameLevel++;
  Frame = new dcframe;
  RFrame = new frame;
  LastFrame = Frame->fnum = cp->dat();
  Frame->fpage = LastPage;
  FrameList.add(RFrame, Frame->fnum);
  InitFrame(RFrame);
	StartingFrame = true;
}


void
HTMwr::InitFrame(frame *fr)
{
#ifdef UNUSED
  CurrPen = LastPen = DefaultPen;
  CurrBrush = LastBrush = DefaultBrush;
  fr->whead.fsize = 122; // 9 + 23 + 16 + 11 + 12 + 32 + ... + 19
  fr->whead.recmax = 28;
  fr->whead.objmax = 3;
#endif
}


void
HTMwr::EndFrame(HTMctl *cp)
{
	if (((short) FrameLevel) < 1)
		return;
  if (FrameStack[FrameLevel - 1] == NULL)
    ProcEndFrame();
  FrameLevel--;
  FrameStack[FrameLevel] = NULL;
	if (FrameLevel)
		RFrame = FrameStack[FrameLevel - 1];
}


void
HTMwr::ProcEndFrame(void)
{
  frame *fr = NULL;

	if (!Frame || !RFrame)
		return;

  FrameStack[FrameLevel - 1] = RFrame;

  if (!Frame->fanchor
	 && (FrameLevel > 1)
   && ((fr = FrameStack[FrameLevel - 2]) != NULL)) {
    fr->frcount++;
    fr->grcount++;
    if (!fr->frlist)
      fr->frlist = new DCvlist;
    (fr->frlist)->add(RFrame, fr->frcount);
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

  RFrame->ftext = Frame->ftext;
  RFrame->wide = Frame->wide;
  RFrame->high = Frame->high;
  RFrame->iwide = Frame->iwide;
  RFrame->ihigh = Frame->ihigh;
	RFrame->irefpt = Frame->irefpt;
  RFrame->fmdpi = Frame->fmdpi;

  if (fr && (fr->fpos != 4)) { // nested frame
    RFrame->fpos = 0;
    RFrame->halgn = 0;
    RFrame->valgn = fr->valgn;
    RFrame->href = 1;
    RFrame->vref = fr->vref;
    RFrame->xpos = Frame->xpos + fr->xpos;
    RFrame->ypos = Frame->ypos + fr->ypos;
    RFrame->xspace = fr->xspace;
		RFrame->parent = fr;
		if (RFrame->ftext) {
			GrTextFrameList.add(RFrame, RFrame->ftext);
			if (!fr->ftext)
				fr->ftext = RFrame->ftext;
		}
    return;
  }

  if (Frame->ftext) {
    switch (Frame->fusage) {
			case 6:  // ditamap
				//if (!IncludeMaps)
				//	break;
      case 1:
        DocBodyList.add(Frame->ftext);
        BodyFrameList.add(RFrame, LastPage);
        PFrameCount++;
        break;
      case 2:
        HeaderList.add(Frame->ftext);
        break;
      case 3:
        FooterList.add(Frame->ftext);
        break;
      default:
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
      RFrame->href = 2;
      RFrame->vref = 1;
      break;
    case 2: // in vert
      RFrame->href = 1;
      RFrame->vref = 2;
      break;
    case 3: // in both
      RFrame->href = RFrame->vref = 2;
      break;
    default:
      break;
  }

  RFrame->xpos = Frame->xpos;
  RFrame->ypos = Frame->ypos;
	RFrame->fpage = Frame->fpage;

  switch (Frame->refpt) {
    case 3:  // page
      RFrame->fpos = 3;
      break;
    case 4:  // frame or col
      RFrame->fpos = 3;
      RFrame->href = RFrame->vref = 2;
			RFrame->presuf = true;
      break;
    case 2:  // para start
    case 5:  // col edge
      RFrame->fpos = 2;
      RFrame->href = 1;
      RFrame->valgn = 4;
      if (Frame->xoff > 0)
        RFrame->xspace = Frame->xoff;
      break;
    case 1:  // text anchor
      RFrame->fpos = 0;
			RFrame->presuf = true;
      break;
    default:
      break;
  }

  switch (Frame->fpos) {
    case 1:  // at anchor (inline)
      RFrame->fpos = 0;
			RFrame->presuf = true;
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

  if (!Frame->fanchor   // unanchored, process at start of first para on page
   && !Frame->ftext) {
    RFrame->fpos = 4;
    FramePageList.add(RFrame, LastPage);
  }
  // what if no para start on a page with unanchored frames???

#if 0
  delete Frame;
#endif
}


void
HTMwr::ProcFrAttr(HTMctl *cp)
{
	if (!Frame)
		return;

  switch (cp->dat()) {
    case 2: // body, header, footer, border, label
      Frame->fusage = (uns) cp->siz();
      break;
    case 3: // master type, 1 = content, 2 = frame only
      Frame->fmtype = (uns) cp->siz();
      break;
    case 4:  // shared master ID
      Frame->fshare = (uns) cp->siz();
      break;
    case 5:  // table frame usage
      Frame->ftbl = (uns) cp->siz();
      break;
    case 6:  // horizontal alignment
      Frame->halgn = (uns) cp->siz();
      break;
    case 7:  // vertical alignment
      Frame->valgn = (uns) cp->siz();
      break;
    case 8:  // foreground or background usage
      Frame->fzpos = (uns) cp->siz();
      break;
    case 9:  // inline, following anchor, or float
      if ((Frame->fpos = (uns) cp->siz()) > 3)
        Frame->fpos = 0;
      break;
    case 10:  // autosizing
      Frame->fauto = (uns) cp->siz();
      break;
    case 11:  // cropping
      Frame->fcrop = (uns) cp->siz();
      break;
    case 12: // within col hor, within col vert, within col both
      Frame->flim = (uns) cp->siz();
      break;
    case 13: // rotation
      Frame->frot = (uns) cp->siz();
      break;
    case 14: // gap
      RFrame->xspace = (uns) GetTwips(cp->siz(), pg_unit);
      break;
    default:
      break;
  }
}


void
HTMwr::ProcFrSize(HTMctl *cp)
{
	if (!Frame)
		return;
  if (Frame->refpt == 0)
    Frame->refpt = cp->dat();
	else if (InImpObj)
    Frame->irefpt = cp->dat();

  long *lptr = (long *) cp->ex();
  uns lcount = (uns) (cp->siz() / sizeof(long));
  Frame->fshape = (lcount > 5) ? 2 : ((lcount == 5) ? 1 : 0);
  switch (lcount) {
    case 5:  // ellipse, xy of center, xy of major/minor diameters, rot angle
      Frame->rang = (uns) lptr[4];
    case 4:  // rect, xy of top left, xy of width and height
			if (Frame->high == 0)
				Frame->high = GetTwips(lptr[3], pg_unit);
			else if (InImpObj)
				Frame->ihigh = GetTwips(lptr[3], pg_unit);

    case 3:  // size unknown, xy of top left, x of wid (0 if unknown)
			if (Frame->wide == 0)
				Frame->wide = GetTwips(lptr[2], pg_unit);
			else if (InImpObj)
				Frame->iwide = GetTwips(lptr[2], pg_unit);
			if (InGraphic) { // in image, make additive
	      Frame->ypos += GetTwips(lptr[1], pg_unit);
	      Frame->xpos += GetTwips(lptr[0], pg_unit);
			}
			else {
	      Frame->ypos = GetTwips(lptr[1], pg_unit);
	      Frame->xpos = GetTwips(lptr[0], pg_unit);
			}
      return;
    case 2:  // column offset (x to nearest, y to bottom)
      Frame->yoff = GetTwips(lptr[1], pg_unit);
    case 1:  // anchor to bottom offset
      Frame->xoff = GetTwips(lptr[0], pg_unit);
    case 0:  // at reference
      return;
    default: // polygon, xy of vertices
      break;
  }
  Frame->vcount = lcount / 2;
  Frame->vertx = new short[lcount];
  for (uns i = 0; i < lcount; i++)
      Frame->vertx[i] = GetTwips(lptr[i], pg_unit);
}




// frame writing

#ifdef UNUSED

char *HTMwr::FrameHRef[FrameHRMax] = {
  "phpg", "phmrg", "phcol"
};

char *HTMwr::FrameVRef[FrameVRMax] = {
  "pvpg", "pvmrg"
};

char *HTMwr::FrameVAlgn[FrameVAMax] = {
  "posy", "posyt", "posyc", "posyb", "posyil"
};

#endif

char *HTMwr::FrameHAlgn[FrameHAMax] = {
  "left", "left", "center", "right", "left", "right"
};


DClist *HTMwr::FramePendList = NULL;
DClist *HTMwr::PreSufFrames = NULL;
bool HTMwr::WritingFrame = false;
bool HTMwr::UseTitleForAlt = false;
bool HTMwr::AllowEmptyAlt = true;
bool HTMwr::MathFullForm = false;
DCvlist HTMwr::AreaList;
char *HTMwr::AreaBase = NULL;
short HTMwr::AreaScale = 100;
area *HTMwr::CurrArea = NULL;
char *HTMwr::GrTextHyperFileName = NULL;
bool HTMwr::TextFrameIsText = false;
bool HTMwr::TextFrameContent = false;
DClist HTMwr::TextFrameIsTextList;
bool HTMwr::XMLGraphAttrs = false;
char *HTMwr::ImgTagElement = "img";
char *HTMwr::ImgSrcAttr = "src";
char *HTMwr::GraphGroup = NULL;
char *HTMwr::GraphSrc = NULL;
char *HTMwr::GraphBase = NULL;
char *HTMwr::GraphOrigHigh = NULL;
char *HTMwr::GraphOrigWide = NULL;
char *HTMwr::GraphClass = "graphic";
char *HTMwr::GraphParaAlign = NULL;
char *HTMwr::GraphAlign = NULL;
char *HTMwr::GraphFile = NULL;
char *HTMwr::GraphDpi = NULL;
char *HTMwr::GraphAttr = NULL;
char *HTMwr::GraphALT = NULL;
char *HTMwr::GraphStartMacro = NULL;
char *HTMwr::GraphReplaceMacro = NULL;
char *HTMwr::GraphEndMacro = NULL;
char **HTMwr::GraphParents = NULL;
short HTMwr::GraphIndent = 0;
short HTMwr::GraphRightSpacer = -1;
short HTMwr::GraphScaleVal = 100;
short HTMwr::GraphWide = 0;
short HTMwr::GraphHigh = 0;
bool HTMwr::GraphPathOverrides = true;
bool HTMwr::GraphScale = true;
bool HTMwr::UsePxSuffix = true;
bool HTMwr::UsePtSuffix = false;
bool HTMwr::RefPageGraphIndent = true;
bool HTMwr::GraphAlignAttributes = true;
bool HTMwr::GraphWrapPara = true;
bool HTMwr::ImgNames = false;
bool HTMwr::GraphSrcUsed = false;
DCnlist HTMwr::GrGenList;
DCnlist HTMwr::GrDpiList;
DClist HTMwr::GrAltList;
DClist HTMwr::GrLDescList;
DClist HTMwr::GrClassList;
short HTMwr::EqFrExpand = 125;
anyini HTMwr::GraphIni = {
 {NULL, NULL, NULL, NULL, NULL},
 0 };

bool HTMwr::MultiImageFigures = true;
bool HTMwr::UseOtherpropsDpi = false;
bool HTMwr::FigureTitleStartsFigure = false;
bool HTMwr::FigureOpen = false;
bool HTMwr::FigureOpening = false;
bool HTMwr::FigsInStart = false;
long HTMwr::FigureLev = 0;
long HTMwr::FigureDiv = 0;
long HTMwr::PreFigureDiv = 0;
long HTMwr::FigParentDiv = 0;
DClist HTMwr::FigureDivRefList;
char *HTMwr::InlineFigParents[3] = {
 "inlinemediaobject", "imageobject", NULL };
char *HTMwr::BlockFigParents[3] = {
 "mediaobject", "imageobject", NULL };

char *HTMwr::ImageMapFigParents[3] = {
 "mediaobjectco", "imageobjectco", NULL };
char *HTMwr::SingleFigParent[2] = {
 "imageobject", NULL };
char *HTMwr::ImageMapParents[3] = {
 "p", "imagemap", NULL };
char **HTMwr::DITAImageParents = NULL;
short HTMwr::PixelWidth = 0;
short HTMwr::PixelHeight = 0;
long HTMwr::MapNumber = 0;
long HTMwr::MapAreaNumber = 0;
long HTMwr::MapCalloutDiv = 0;
long HTMwr::PreCalloutDiv = 0;
DClist HTMwr::MapCalloutDivRefList;
long HTMwr::ImageRefNum = 0;

bool HTMwr::RetainRuninImagesForEmptyParagraphs = false;


void
HTMwr::WriteFrame(HTMwfile *wf, frame *fr, bool refpage)
{
	if (WritingFrame)
		return;

	if (((frame *) PRuninFrameList.find(ParaRefNum) == fr)
	 && (RuninRefNum != ParaRefNum))  // premature
		return;

  if (fr->ftext  // top level frame is text only
	 && TextFrameIsTextList.find((long) fr))
		TextFrameContent = true;

	if ((fr->grname == NULL)
	 && (fr->grfile == NULL)
	 && !TextFrameContent)
		return;

	if (Curr.macpos & MacReplace)  // macro replace
		return;

  if (((fr->fpos >= 1) && InPara)
	 || ((fr->fpos == 1) && ParaStarting)) { // not inline, within para already
    if (fr->fpos == 1) { // goes after para, others already out before
			if (fr->presuf && Curr.sfx) {
				if (PreSufFrames == NULL)
					PreSufFrames = new DClist;
				PreSufFrames->add( (long) fr);
			}
			else {
				if (FramePendList == NULL)
					FramePendList = new DClist;
				FramePendList->add( (long) fr);
			}
    }
    return;
  }

	if (TextFrameContent
	 && fr->ftext) {
		HTMgroup *gp = (HTMgroup *) FlowList.find(fr->ftext);
		if (gp != NULL) {
		  WritingFrame = true;
			InGraphic = false;
			SaveState();
      gp->write(*wf);
			RestoreState();
		  WritingFrame = false;
		}
		TextFrameContent = false;
    return;
  }

	char *frfile = NewName(fr->grfile);
	char *frname = NewName(fr->grname);

	CorrectGraphFilePath(&frfile);

	bool nofile = false;
	bool noname = false;
	bool generate = UseGraphicPreviews || fr->mathobj;

	if (frfile == NULL) {   // no imported graphic
		nofile = true;
		frfile = frname;  // use preview image
	}
	else if (frname == NULL) {  // no preview name
		noname = true;
		generate = false;
		frname = frfile;  // use first import as name
	}

	char *grnbase = NULL;
	char *grfbase = NULL;
	char *grbase = NULL;
	bool nbase = false;
	bool fbase = false;
	bool base = false;

	// set up for .ini FindAny calls
	GraphIni.nm[0] = frname;  // FDK name with ext
	GraphIni.cnt = 1;  // always have one

	nbase = GetBaseName(frname, grnbase);
	if (nbase)
		GraphIni.nm[GraphIni.cnt++] = grnbase;  // FDK name without ext

	if (!nofile) {
		GraphIni.nm[GraphIni.cnt++] = frfile;  // file name with ext
		fbase = GetBaseName(frfile, grfbase);
		if (fbase)   // copy of name without extension is next
			GraphIni.nm[GraphIni.cnt++] = grfbase;
	}

	if (UseGraphicPreviews && !(UseOrigGrName && !nofile)) {
		base = nbase;
		grbase = grnbase;
	}
	else {
		base = fbase;
		grbase = grfbase;
	}

	if (!grbase)  // used for image maps
		grbase = NewName(frname);

	if (fr->grgroup)  // set by a marker
		GraphIni.nm[GraphIni.cnt++] = fr->grgroup;
	else if (IniFile
	 && IniFile->Section("GraphGroup")  // read group name
	 && IniFile->FindAny(&GraphIni)) {
		GraphIni.nm[GraphIni.cnt++] = fr->grgroup = IniFile->StrRight();
	}
	// finished with setup for .ini

	// set defaults for frame

	GraphGroup = NULL;  // for marker val

	GraphParaAlign = FrameHAlgn[fr->halgn];
	GraphAlign = NULL;
	GraphFile = NULL;
	GraphAttr = NULL;
	GraphALT = NULL;

	GraphIndent = -1;
	GraphDpi = 0;
	GraphScaleVal = fr->mathobj ? EqFrExpand : 100;
	GraphWide = GraphHigh = 0;
	GraphRightSpacer = -1;

	GraphStartMacro = NULL;
	GraphReplaceMacro = NULL;
	GraphEndMacro = NULL;

	if (HXMwr::HXML)
		GraphParents = DITAImageParents;

	// set per .ini
	DCini::ProcIniGroup("GraphAttrSet");
	DCini::ProcIniGroup("GraphMacroSet");

	if (HXMwr::HXML)
		DCini::ProcIniGroup("GraphParentSet");

	// set per Object Attribute configs
	(*(fr->cfglist)).all(DCini::GetPropSettings);

	// set per markers
	DCini::GraphSetList.all(DCini::GetGraphSettings);

	// start macro can affect settings too
	long prediv = wf->GetDiv();
	long gsmdiv = wf->NewDiv();
	DCmacr::WriteMacro(wf, GraphStartMacro);
	wf->SelectDiv(prediv);

	// settings finished, begin processing

	if ((SideHUsed == true)
	 && (Curr.sideh > 1))   // in side head col
		fr->halgn = 0;

	if (GraphWrapPara) {
		if ((fr->fpos > 0)        // not inline
		 && (fr->fpos != 3)      // not runin
		 && (!((fr->fpos == 2) && (fr->href == 1) && (fr->valgn == 4)))
		 && (fr->halgn > 1)) { // not left
			if (!HXMwr::HXML) {
				if (ClassIsTag)
					wf->startBlock(GraphClass);
				else {
					wf->startBlock("p");
					if (WriteClassAttributes)
						wf->setClass(GraphClass);
				}
				if (AlignAttributes)
					wf->strAttr("align", GraphParaAlign);
				wf->endElem(true);
			}
		}
		else if (fr->fpos == 0) { // start para for inline (not following)
			if (KeepGraphicsInPara)
				WriteText(wf);
			else
				WriteParaStartProps(wf);
		}
	}

	// set GraphSrc variable

	if (GraphSrc) {
		DeleteName(GraphSrc);
		DeleteName(GraphBase);
		GraphSrc = GraphBase = NULL;
	}
	GraphSrcUsed = false;

	bool mapset = AreaList.find((long) fr) ? true : false;

	bool orig = ((UseOrigGrName
	           || !UseGraphicPreviews)
	          && !nofile
	          && ((fr->grcount == 1)
						 || mapset))
	          ? true : false;

	if (!GraphFile)  // not set by .ini or marker
		GraphFile = FixGrFileName(orig ? frfile : frname);
	else if (GraphPathOverrides)
		GraphFile = FixGrFileName(GraphFile);

	GraphSrc = NewName(GraphFile);

	size_t elen = strlen(GraphFile) + 2; // room for dot and nul
	char *extp = strrchr(GraphFile, '.');
	if (extp != NULL)              // trim off any ext
		*extp = '\0';
	GraphBase = NewName(GraphFile);

	char nbuf[NumStrMax];
	short gdwide = (orig && fr->iwide) ? fr->iwide : fr->wide;
	GraphOrigWide = NewName(_itoa((int) PixelsFromTwips(gdwide), nbuf, 10));
	short gdhigh = (orig && fr->ihigh) ? fr->ihigh : fr->high;
	GraphOrigHigh = NewName(_itoa((int) PixelsFromTwips(gdhigh), nbuf, 10));

	bool extrset = false;

	if (Extracting
	 && (ExtrGraph == NULL)) {
		extrset = true;
		ExtrGraphID = NewName(frname);
		if ((ExtrGraphThumbnail == 'N')
		 && (ExtrGraphSuffix != NULL))
			elen += strlen(ExtrGraphSuffix);
		ExtrGraph = NewName(elen);
		strcpy(ExtrGraph, GraphBase);
		if ((ExtrGraphThumbnail == 'N')
		 && (ExtrGraphSuffix != NULL))
			strcat(ExtrGraph, ExtrGraphSuffix);
		if (extp != NULL) {
			*extp = '.';
			strcat(ExtrGraph, extp);  // add ext back onto end
		}
	}


	// write img tag

	// for svg in chm, use this tagging:
	// <embed type="image/svg+xml" src="some.svg" width="720" height="415">
	// </embed>
	// and include all of the SVG names in the .hhp [FILES]
	// http://www.velocityreviews.com/forums/t169618-compiled-help-chm-and-svg.html



  WritingFrame = true;
  //wf->putEnd();

	long grDITAStartDiv = HXMwr::DITAStartDiv;
	long grDITAEndDiv = HXMwr::DITAEndDiv;
	long grPreDITADiv = HXMwr::PreDITADiv;

	if (HXMwr::HXML
	 && !GraphReplaceMacro) {
		if (!TblFigureDiv
		 && (!Curr.nofig)
		 && ((fr->fpos != 0)                 // not inline
		  || EmptyParaList.find(ParaRefNum)  // or in empty anchor para
		  || FigureDiv                       // or in fig
			|| Curr.fig                        // or fig wanted
			|| HXMwr::DITANoBlock)
		 && (!FigureOpen                     // not after a figure title
		  || FigureDiv)) {                   // unless also after image
			if (!FigureOpen
			 || (FigureDiv
			  && !MultiImageFigures)) {
				HXMwr::WriteDITACloseBlocks(wf, (char **) HXMwr::DITAParaCloseBeforeList.find(ParaRefNum));
				HXMwr::WriteDITAOpenBlocks(wf, (char **) HXMwr::DITAParaOpenBeforeList.find(ParaRefNum));
			}
			if (FigureDiv
			 && !MultiImageFigures) {
				if (grDITAStartDiv)
					FigsInStart = true;
				HXMwr::CloseFigureDiv(wf);         // write graphic and ending figure tag
			}
			if (!FigureDiv) {      // not in a figure
				PreFigureDiv = wf->GetDiv();
				FigureDiv = wf->NewDiv();
			}
			else if (MultiImageFigures) {  // more than one graphic in figure
				if (grDITAStartDiv)
					FigsInStart = true;
				HXMwr::CloseFigureDiv(wf, true);    // write ending wrapper tags
				FigureDiv = wf->NewDiv();
			}
			else
				wf->SelectDiv(FigureDiv);
		}
		HXMwr::SetDITAStart(wf);
	}

	prediv = wf->GetDiv();
	wf->WriteDiv(gsmdiv);
	wf->DeleteDiv(gsmdiv);
	wf->SelectDiv(prediv);

	SetCondFlags(wf);

	if (GraphReplaceMacro)
		DCmacr::WriteMacro(wf, GraphReplaceMacro);
	else {
		bool inlin = false;

		if (refpage && !RefPageGraphIndent)
			;
		else if (GraphIndent >= 0)
			WriteSpacer(wf, GraphIndent);
		else if (fr->fpos == 0) {  // inline
			inlin = true;
			if (!NonWhiteText)       // at start of para
				WriteSpacer(wf, PixelsFromTwips(Curr.find));
		}
		else if ((fr->xpos > 0)   // positioned to right of ref
		 && ((fr->halgn < 2) || (fr->halgn == 4))) { // left aligned
			frame *bfr = (frame *) BodyFrameList.find(fr->fpage);
			short ind = fr->xpos;
			if (bfr)
				ind = fr->xpos - bfr->xpos;  // deduct margin
			if (ind)
				WriteSpacer(wf, PixelsFromTwips(ind));
		}

		if (GraphScale) { // determine scale
			short w = (orig && fr->iwide) ? fr->iwide : fr->wide;
			w = PixelsFromTwips((short) (((long) w * GraphScaleVal) / 100));
			short h = (orig && fr->ihigh) ? fr->ihigh : fr->high;
			h = PixelsFromTwips((short) (((long) h * GraphScaleVal) / 100));

			if (extrset
			 && OrigSizedThumbnail) {
				char buf[18];
				if (h > 0)
					ExtrGraphOrigHigh = NewName(itoa(h, buf, 10));
				if (w > 0)
					ExtrGraphOrigWide = NewName(itoa(w, buf, 10));
			}
			if (GraphWide)
				w = GraphWide;
			if (GraphHigh)
				h = GraphHigh;
			PixelWidth = w;
			PixelHeight = h;
		}

		char **figparents = NULL;
		if (HXMwr::DITA) {
			if (mapset)
				figparents = FigureOpen ? ImageMapParents : (ImageMapParents + 1);
			if ((!InTbCell
			  && EmptyParaList.find(ParaRefNum))  // in empty anchor para
			 || HXMwr::DITANoBlock
			 || FigureDiv                       // or in fig
			 || FigureOpen)
				inlin = false;                    // handle as block
		}
		else if (HXMwr::DocBook) {
			if (mapset) {  // writing to start div
				long cdiv = wf->GetDiv();
				if (cdiv == HXMwr::DITAStartDiv)
					HXMwr::SetDITADivOff(wf);
				PreCalloutDiv = wf->GetDiv();
				MapCalloutDiv = wf->NewDiv();
				wf->startSBlock("calloutlist");
				wf->putEnd();
				wf->SelectDiv(PreCalloutDiv);
				HXMwr::WriteDITAOpenBlocks(wf, ImageMapFigParents);
				WriteImageMap(wf, fr, grbase);
				wf->SelectDiv(PreCalloutDiv);
				figparents = SingleFigParent;
				if (cdiv == HXMwr::DITAStartDiv)
					wf->SelectDiv(HXMwr::DITAStartDiv);
			}
			else if ((fr->fpos == 0)  // inline
			 && !FigureDiv
			 && !FigureOpen)
				figparents = InlineFigParents;
			else {
				inlin = false;
				figparents = BlockFigParents;
			}
		}
		if (HXMwr::HXML
		 && GraphParents) {  // prefix any additions onto the list
			if (!figparents)
				figparents = GraphParents;
			else   // combine
				figparents = HXMwr::CombineSars(GraphParents, figparents);
		}

		// SetCondFlags(wf);

		if (HXMwr::HXML) {
			HXMwr::WriteDITAStartPTag(wf, 0, HXMwr::DefDITAImg, figparents);
			HXMwr::SetDITADivOff(wf);
		}
		else
			wf->startElem(ImgTagElement);

		if (ImgNames
		 || (HXMwr::DITA
		  && HXMwr::SetElementIDs))
			wf->strAttr(HXMwr::IDattr, frname);

		if (inlin && HXMwr::HXML)
			wf->putEnd();

		wf->strAttr(HXMwr::DITA ? "href" : ImgSrcAttr, GraphSrc);
		GraphSrcUsed = true;

		StartCondImage(wf);

		if (!GrClassList.find(fr->id)
		 && !HXMwr::DITA) {
			if (fr->gclass) {
				wf->putEnd();
				wf->setClass(fr->gclass);
			}
		}

		if (fr->attrlist)
			(fr->attrlist)->writeall(wf, WriteAttrListItem);

		if (fr->proplist)
			(fr->proplist)->writeall(wf, WritePropListItem);

		if (GraphAlignAttributes) {
			if (GraphAlign)
				wf->strAttr("align", GraphAlign);
			else if ((fr->halgn != 2)  // not centered
			 && ((fr->fpos == 3)     // runin
				|| ((fr->fpos == 2) && (fr->href == 1) && (fr->valgn == 4))))
				wf->strAttr("align", FrameHAlgn[fr->halgn]); // make float
		}

		if (HXMwr::DITA) {
			//wf->putEnd();
			wf->strAttr("placement", inlin ? "inline" : "break");
			if (fr->fmdpi > 0) {
				char *dpval = NewName("fmdpi:", 16);
				sprintf(dpval + 6, "%ld", fr->fmdpi);
				wf->strAttr("otherprops", dpval);
			}
		}

		if (GraphScale) {
			char *suf = UsePxSuffix ? "px" : NULL;
			if (HXMwr::DITA
			 && UsePtSuffix) {
				ConversionDPI = 72;
				suf = "pt";
			}
			if (PixelWidth > 0)
				wf->numAttr("width", PixelWidth, suf);
			if (PixelHeight > 0)
				wf->numAttr(HXMwr::DocBook ? "depth" : "height", PixelHeight, suf);
		}

		if (!HXMwr::HXML) {
			if ((fr->fpos == 2)
			 && (!(fr->href == 1) && (fr->valgn == 4))  // not to runin
			 && (fr->xspace != 0)) {
				wf->numAttr("hspace", PixelsFromTwips(fr->xspace));
				wf->numAttr("vspace", PixelsFromTwips(fr->xspace));
			}

			if (GraphAttr) {
				wf->putChar(' ');
				wf->putStr(GraphAttr);
				if (strstr(_strlwr(GraphAttr), "usemap") != NULL)
					mapset = false;  // doing it manually
			}

			if (mapset) {
				char *umap = NewName("#", strlen(grbase) + 2);
				strcat(umap, grbase);
				wf->strAttr("usemap", umap);
				DeleteName(umap);
			}
		}

		bool endditatag = true;
		bool dofinish = true;
		bool noend = false;

		if (FigureDiv
		 && !FigureOpen) {
			FigureOpen = true;  // in case of no title
			noend = true;
		}

		if (HXMwr::DocBook) {
			wf->endElem();  // close imagedata tag
			wf->SetAttrAdd(false);
			wf->putEnd();
			HXMwr::EndLevels(wf, HXMwr::CurrLevel - 2); // and imageobject
			if (mapset) {
				wf->SelectDiv(MapCalloutDiv);
				wf->endBlock("calloutlist");
				wf->putEnd();
				long tdiv = HXMwr::PreDITADiv;
				HXMwr::PreDITADiv = MapCalloutDiv;
				HXMwr::EndLevels(wf, HXMwr::CurrLevel - 2); // and imageobjectco
				HXMwr::PreDITADiv = tdiv;
			}
			dofinish = false;
		}

		if (!GrAltList.find(fr->id)
		 || (HXMwr::DITA && MathFullForm && fr->math)) {
			// alt not already put out from GraphAlt marker
			if (AllowEmptyAlt)
				wf->SetEmptyAttr(true);
			if (fr->alt)
				GraphALT = fr->alt;
			if (HXMwr::DITA) {
				if (MathFullForm
				 && fr->math)
					GraphALT = fr->math;
				if (GraphALT) {  // write alt element
					wf->endElem(true);  // make prev tag into start of block
					wf->SetAttrAdd(false);
					wf->putEnd();
					wf->startSBlock("alt");
					wf->putStr(GraphALT);
					wf->endBlock("alt");
					wf->endBlock(HXMwr::DefDITAImg);
					endditatag = false;
				}
			}
			else if (HXMwr::HXML) {
				if (HXMwr::DocBook
				 && GraphALT) {  // write alt element
					wf->startSBlock("textobject");
					wf->startSBlock("phrase");
					wf->putStr(GraphALT);
					wf->endBlock("phrase");
					wf->endBlock("textobject");
					wf->putEnd();
				}
			}
			else if (GraphALT) {
				wf->putEnd();
				if (UseTitleForAlt) {
					wf->strAttr("title", GraphALT);
					wf->strAttr("alt", "");
				}
				else
					wf->strAttr("alt", GraphALT);
			}
			else
				wf->strAttr("alt", "");

			if (AllowEmptyAlt)
				wf->SetEmptyAttr(false);
		}

		if (!HXMwr::DITA) {
			if (!GrLDescList.find(fr->id)) {
				if (fr->longdesc) {
					wf->putEnd();
					if (HXMwr::DocBook) {
						wf->startSBlock("textobject");
						wf->startSBlock("para");
						wf->putTextStr(fr->longdesc);
						wf->endBlock("para");
						wf->endBlock("textobject");
						wf->putEnd();
					}
					else if (!HXMwr::HXML)
						wf->strAttr((UseTitleForAlt && GraphALT) ? "longdesc" : "title", fr->longdesc);
				}
			}
		}

		if (!HXMwr::HXML
		 && XMLGraphAttrs) {
			//xml:link="simple" show="embed" actuate="auto"
			wf->strAttr("xml:link", "simple");
			wf->strAttr("show", "embed");
			wf->strAttr("actuate", "auto");
		}

		if (HXMwr::HXML) {
			if (dofinish)
				HXMwr::WriteDITAFinishTag(wf, HXMwr::DefDITAImg, endditatag);
			if (HXMwr::DITA) {
				if (!inlin
				 && !noend)
					wf->putEnd();
				if (mapset) {
					WriteImageMap(wf, fr, grbase); // write all area tags
					HXMwr::WriteDITACloseBlocks(wf, figparents);
				}
			}
			else if (HXMwr::DocBook)
				HXMwr::WriteDITACloseBlocks(wf, mapset ? ImageMapFigParents : figparents);
		}
		else
			wf->endElem();

		wf->SetAttrAdd(false);

		if (GraphRightSpacer >= 0)
			WriteSpacer(wf, GraphRightSpacer, PixelsFromTwips(fr->high));

	} // end of !GraphReplaceMacro

	SetCondFlags(wf);

	DCmacr::WriteMacro(wf, GraphEndMacro);

	if (generate && !orig
	 && GraphSrcUsed) {
		char *slash = strrchr(GraphSrc, '/');
		if (slash)
			slash++;
		else
			slash = GraphSrc;
		GrGenList.addnew(slash, fr->id);
		if ((!GrDpiList.find(fr->id))
		 && GraphDpi)
			GrDpiList.addnew(GraphDpi, fr->id);
	}

	if (MapCalloutDiv)
		wf->SelectDiv(PreCalloutDiv);
	if (FigureDiv)
		wf->SelectDiv(PreFigureDiv);

	if (GraphWrapPara) {
		if ((fr->fpos > 0)
		 && (!((fr->fpos == 2) && (fr->href == 1) && (fr->valgn == 4)))
		 && (fr->fpos != 3)    // not inline, outside col, or runin
		 && (fr->halgn > 1)) { // right or centered
			if (!NoParaClose
			 && !HXMwr::HXML) {
				if (ClassIsTag)
					wf->endBlock(GraphClass);
				else
					wf->endBlock("p");
			}	
			InPara = false;
		}
	}

	if (mapset
	 && !HXMwr::HXML
	 && !GraphReplaceMacro)
		WriteImageMap(wf, fr, grbase);

	if (HXMwr::CloseFigAfterImage
	 && FigureOpen
	 && !FigureDiv)
		HXMwr::WriteDITACloseBlocks(wf, HXMwr::DocBook ? HXMwr::FigureSar : HXMwr::FigSar);


	if (grbase)
		DeleteName(grbase);

	DeleteName(frfile);
	if (frname != frfile)
		DeleteName(frname);

	HXMwr::DITAStartDiv = grDITAStartDiv;
	HXMwr::DITAEndDiv = grDITAEndDiv;
	HXMwr::PreDITADiv = grPreDITADiv;

	ImageConds = NULL;
  WritingFrame = false;
}


void 
HTMwr::CorrectGraphFilePath(char **gfile)
{
	char *cpt = *gfile;
	if (cpt) {
		if (CopyOriginalGraphics
		 && !GraphFileCopyList.getnum(cpt))
			GraphFileCopyList.addnew(cpt, ++GraphCopyCount);

		if (StripGraphPath) {
			GetBaseName(cpt, cpt, true);
			*gfile = cpt;
		}

		char *suffix = strrchr(cpt, '.');
		char *nsuf = NULL;

		if (suffix
		 && IniFile
		 && IniFile->Section("GraphSuffix")  // read new suffix
		 && IniFile->Find(suffix + 1)
		 && ((nsuf = IniFile->StrRight()) != NULL)) {
			if (suffix > cpt)
				*suffix = '\0';
			cpt = NewName(strlen(*gfile) + strlen(nsuf) + 2);
			sprintf(cpt, "%s.%s", *gfile, nsuf);
			*gfile = cpt;
			cpt = NULL;
		}
		else if (GraphSuffix) {
			if (suffix && (suffix > cpt))
				*suffix = '\0';
			cpt = NewName(strlen(*gfile) + strlen(GraphSuffix) + 2);
			sprintf(cpt, "%s.%s", *gfile, GraphSuffix);
			*gfile = cpt;
			cpt = NULL;
		}
	}
}


char *
HTMwr::FixGrFileName(char *name)
{
	char *cpt = name;
	char *ext = strrchr(name, '.');

	if (GraphPath) {
		GetBaseName(name, cpt, true);
		name = NewName(strlen(GraphPath) + strlen(cpt) + 2);
		sprintf(name, "%s/%s", GraphPath, cpt);
	}

	if (ext == NULL) {
		char *suf = GraphSuffix ? GraphSuffix : "jpg";
		cpt = NewName(strlen(name) + strlen(suf) + 2);
		sprintf(cpt, "%s.%s", name, suf);
		name = cpt;
	}
	else
		cpt = NewName(name);

	if (FixGraphSpaces)
		for (uns i = 0; cpt[i] != '\0'; i++) {
			if (cpt[i] == ' ')
				cpt[i] = '_';
		}

	if (CopyOriginalGraphics
	 && GraphFileCopyList.find(GraphCopyCount))
		GraphFileFixedCopyList.addnew(cpt, GraphCopyCount);

	return cpt;
}



// writing client-side image map from graphic with text frames

void
HTMwr::WriteImageMap(HTMwfile *wf, frame *fr, char *grbase)
{
	if (HXMwr::DocBook) {
		MapNumber++;
		MapAreaNumber = 0;
		wf->startBlock("areaspec");
		wf->strAttr("units", "calspair");
		wf->endElem(true);
	}
	else if (!HXMwr::DITA) {
		wf->startBlock("map");
		wf->strAttr("name", grbase);
		wf->endElem(true);
	}
	wf->putEnd();
	AreaBase = grbase;
	AreaScale = GraphScaleVal ? GraphScaleVal : 100;
	AreaList.write((long) fr, (DCfile *)wf, WriteArea);
	AreaBase = NULL;
	if (HXMwr::DocBook) {
		wf->endBlock("areaspec");
		wf->putEnd();
	}
	else if (!HXMwr::DITA) {
		wf->endBlock("map");
		wf->putEnd();
	}
}


void
HTMwr::WriteArea(DCfile *dwf, void *ap)
{
	area *a = (area *) ap;
	HTMwfile *wf = (HTMwfile *) dwf;

	if (HXMwr::DocBook) {
		if (a->shape
		 && strcmp(a->shape, "rect"))
			return;  // DocBook only does rects
		++MapAreaNumber;
	}
	else if (HXMwr::DITA)
		wf->startSBlock("area");
	else {
		if (Curr.secwin && ((HelpType == 'M') || JHVersion2 || OracleHelp))
			SecWin = true;
		wf->startElem("area");
	}

	char *coord = NULL;
	short l, t, r, b;

	if (!a->shape
	 || !strcmp(a->shape, "rect")) {  // its a rect
		coord = NewName(50);
		if (FMSource) {
			l = PixelsFromTwips((short) (((long) a->left * AreaScale) / 100));
			t = PixelsFromTwips((short) (((long) a->top * AreaScale) / 100));
			r = PixelsFromTwips((short) (((long) a->right * AreaScale) / 100));
			b = PixelsFromTwips((short) (((long) a->bottom * AreaScale) / 100));
		}
		else {  // XML, already pixels
			l = ((long) a->left * AreaScale) / 100;
			t = ((long) a->top * AreaScale) / 100;
			r = ((long) a->right * AreaScale) / 100;
			b = ((long) a->bottom * AreaScale) / 100;
		}
		if (HXMwr::DocBook) { // origin at bottom left, 10K x 10K grid
			l = (short) (((long) l * 10000) / PixelWidth);
			r = (short) (((long) r * 10000) / PixelWidth);
			t = (short) (((long) (PixelHeight - t) * 10000) / PixelHeight);
			b = (short) (((long) (PixelHeight - b) * 10000) / PixelHeight);
			sprintf(coord, "%hd,%hd %hd,%hd", l, b, r, t);
		}
		else  // DITA and HTML
			sprintf(coord, "%hd,%hd,%hd,%hd", l, t, r, b);
	}

	if (!HXMwr::DocBook) {
		if (a->shape  // must be from XML
		 && strcmp(a->shape, "rect")) {  // not a rect, rewrite coords
			long i = 0;
			long val = 0;
			if (!strcmp(a->shape, "default"))
				coord = NULL;
			else if (!strcmp(a->shape, "circle")) {
				coord = NewName(36);
				val = ((long) a->circ[0] * AreaScale) / 100;
				ltoa(val, coord, 10);
				for (i = 1; i < 3; i++) {
					strcat(coord, ",");
					val = ((long) a->circ[i] * AreaScale) / 100;
					ltoa(val, coord + strlen(coord), 10);
				}
			}
			else if (!strcmp(a->shape, "poly")) {
				coord = NewName(a->pcnt * 12);
				val = ((long) a->poly[0] * AreaScale) / 100;
				ltoa(val, coord, 10);
				for (i = 1; i < a->pcnt; i++) {
					strcat(coord, ",");
					val = ((long) a->poly[i] * AreaScale) / 100;
					ltoa(val, coord + strlen(coord), 10);
				}
			}
		}
		if (HXMwr::DITA) {
			wf->startSBlock("shape");
			wf->putStr(a->shape ? a->shape : "rect");
			wf->endBlock("shape");
			if (coord) {
				wf->startSBlock("coords");
				wf->putStr(coord);
				wf->endBlock("coords");
			}
			wf->putEnd();
		}
		else {
			wf->strAttr("shape", a->shape ? a->shape : "rect");
			if (coord)
				wf->strAttr("coords", coord);
		}
	}

	// get full link into href from a->ref and a->file
	char *href = a->ref;

	if (a->url) {
		ExtRefNum++;   // skip number to prevent update of prev ref
		SetTargetFrame(a->ref);
	}
	else if (!a->popup) {  // check link file name against refs
		char *oname = NewName(a->file);
		HyperJumpFileName = a->file;

		href = a->ref = ProcExtRef(a->ref, true, SecWin ? false : true, true);
		if (oname || HyperJumpFileName)
			SetTargetFrame(oname, HyperJumpFileName);

		if (HyperJumpFileName != NULL) {
			if (NoLocations || XrefNoRef || Curr.noref)
				href = NewName(HyperJumpFileName);
			else {
				href = NewName(HyperJumpFileName,
										   strlen(HyperJumpFileName) + strlen(a->ref));
				strcat(href, a->ref);
			}
			if (HelpType == 'M')
				SetDestChm(NewName(HyperJumpFileName), oname);
			HyperJumpFileName = NULL;
		}
		if (oname)
			DeleteName(oname);
	}

	if (HXMwr::DocBook) {
		// if calloutlist is needed, write href to calloutdiv
		// and generate matching IDs here
		char *aid = NewName((NumStrMax * 2) + 8);
		sprintf(aid, "map%darea%d", MapNumber, MapAreaNumber);
		wf->SelectDiv(MapCalloutDiv);
		// <callout arearefs="...">
		//  <para><link linkend="..."></link></para>
		// </callout>
		wf->startBlock("callout");
		wf->strAttr("arearefs", aid);
		wf->endElem(true);
		wf->startSBlock("para");
		wf->startBlock("link");
		wf->putEnd();
		if (!a->url)
			WriteRefFileInfo(wf, 10, MapCalloutDiv);
		wf->strAttr("linkend", (*href == '#') ? (href + 1) : href);
		wf->endElem(true);
		wf->endBlock("link");
		wf->endBlock("para");
		wf->endBlock("callout");
		wf->putEnd();
		wf->SelectDiv(PreCalloutDiv);
		wf->startElem("area");
		wf->strAttr(HXMwr::IDattr, aid);
		wf->strAttr("coords", coord);
	}
	else if (HXMwr::DITA) {
		wf->startBlock("xref");
		wf->putEnd();
		WriteRefFileInfo(wf, 7, FigureDiv);
		wf->strAttr("href", href);
		wf->endElem(true);
		if (a->alt)
			wf->putStr(a->alt);
		wf->endBlock("xref");
	}
	else if (a->popup)
		WriteHyperPopup(wf, href, true);
	else if (SecWin && ((HelpType == 'M') || JHVersion2 || OracleHelp))
		WriteSecWinJump(wf, href, a->url, true);
	else {
		wf->putEnd();
		WriteRefFileInfo(wf, 7, FigureDiv);
		wf->strAttr("href", href);
		if (TargetFrame)
			wf->strAttr("target", TargetFrame);
		TargetFrame = NULL;
	}

	if (HXMwr::DocBook)
		wf->endElem();
	else if (HXMwr::DITA)
		wf->endBlock("area");
	else {
		// set target if any
		if (a->target)
			wf->strAttr("target", a->target);

		// get alt string using AreaBase and base href
		if (a->base) {
			char *alt = NULL;
			char *name = NewName(strlen(AreaBase) + strlen(a->base) + 2);
			sprintf(name, "%s#%s", AreaBase, a->base);

			if (IniFile
			 && IniFile->Section("GraphALT")  // read alt name
			 && (IniFile->Find(name)
				|| IniFile->Find(a->base)))
				alt = IniFile->StrRight();
			else
				alt = a->alt;
			//wf->strAttr(UseTitleForAlt ? "title" : "alt", alt);
			wf->strAttr("title", alt);
		}
		else if (a->alt)
			//wf->strAttr(UseTitleForAlt ? "title" : "alt", a->alt);
			wf->strAttr("title", a->alt);
		else
			wf->putStr(" alt=\"\"");

		wf->endElem();
	}

	wf->putEnd();

	DeleteName(coord);
	SecWin = false;
}


// image map from XML


void 
HTMwr::ProcImageMapStart(void)
{
	// set up for graphic in current frame,
	// map follows graph object external group
	InGraphicText = true;
}


void 
HTMwr::ProcImageMapProp(HTMctl *cp)
{
	// cp->dat() == 1, name (not needed here)

	// cp->dat() == 2, side: siz() 1 = client, 2 = server

}


void 
HTMwr::ProcImageMapEnd(void)
{
	InGraphicText = false;
}


void 
HTMwr::ProcImageMapAreaStart(void)
{
	// init CurrArea
	CurrArea = new area;
}


void 
HTMwr::ProcImageMapAreaProp(HTMctl *cp)
{
	if (!CurrArea)
		return;

	// cp->dat() == 1, href (not used)

	// cp->dat() == 2, alt text
	if (cp->dat() == 2)
		CurrArea->alt = (char *) cp->ex();
}


void 
HTMwr::ProcImageMapAreaShape(HTMctl *cp)
{
	if (!CurrArea)
		return;

	long *ldat = (long *) cp->ex();
	long lcnt = cp->siz() / sizeof(long);
	long i = 0;

	switch (cp->dat()) {
		case 1:  // rect, xy for top left, bottom right, lcnt == 4
			CurrArea->shape = "rect";
			CurrArea->left = ldat[0];
			CurrArea->top = ldat[1];
			CurrArea->right = ldat[2];
			CurrArea->bottom = ldat[3];
			break;
	
		case 2:  // poly, xy vertices
			CurrArea->shape = "poly";
			CurrArea->pcnt = lcnt;
			CurrArea->poly = new long[lcnt];
			for (i = 0; i < lcnt; i++)
				CurrArea->poly[i] = ldat[i];
			break;

		case 3:  // circle, xy center, radius, lcnt == 3
			CurrArea->shape = "circle";
			CurrArea->circ = new long[3];
			CurrArea->circ[0] = ldat[0];
			CurrArea->circ[1] = ldat[1];
			CurrArea->circ[2] = ldat[2];
			break;

		case 4:  // default, no values
			CurrArea->shape = "default";
			break;

		default:
			break;
	}
}


void 
HTMwr::ProcImageMapAreaEnd(void)
{
	if (!CurrArea)
		return;

	// add CurrArea to AreaList using RFrame
	AreaList.add(CurrArea, (long) RFrame);
	CurrArea = NULL;
}



// image map from Frame

void
HTMwr::ProcGrTextFlowEnd(HTMctl *cp)
{
	if (!CurrArea)
		return;

	// compute pixel values for position and size
	short x = GrTextFrame->xpos - GrTextFrame->parent->xpos;
	short y = GrTextFrame->ypos - GrTextFrame->parent->ypos;
	CurrArea->left = x;
	CurrArea->top = y;
	CurrArea->right = x + GrTextFrame->wide;
	CurrArea->bottom = y + GrTextFrame->high;

	// add area to list under its parent frame pointer
	AreaList.add(CurrArea, (long) (GrTextFrame->parent));

	CurrArea = NULL;
	GrTextHyperFileName = NULL;
	XrefFilePrefix = CurrFilePrefix;
}


void
HTMwr::ProcGrTextHyperLink(HTMctl *cp)
{
	if (FMSource) {
		if (CurrArea)  // not first
			return;
		CurrArea = new area;
	}

	CurrArea->file = GrTextHyperFileName;
	CurrArea->url = false;

	// make ref into legal link
	char *txt = (char *) cp->ex();
	static char str[LineMax + 1];

	if (!_strnicmp(txt, "firstpage", 9)		// nonunique names,
	 || !_strnicmp(txt, "lastpage", 8)) { // use filename as ref
		// pass empty string as link, filename is sufficient
		str[0] = '\0';
		CurrArea->ref = NewName(str);
		if (!GetBaseName(CurrArea->file, CurrArea->base))
			CurrArea->base = NewName(CurrArea->file);
	}
	else {  // relative in current file
		str[0] = '#';
		str[1] = '\0';
		MakeID(txt, &str[1], LineMax, HyperSpaceChar, true);
		CurrArea->ref = NewName(str);
		CurrArea->base = CurrArea->ref + 1;
	}																

	XrefFilePrefix = CurrFilePrefix;
}


void 
HTMwr::ProcGrTextHyperObj(HTMctl *cp, bool page)
{
	// hyperlink jump to numeric object, "X"
	if (FMSource) {
		if (CurrArea)  // not first
			return;
		CurrArea = new area;
	}

	CurrArea->file = GrTextHyperFileName;
	CurrArea->url = false;

	long id = cp->siz();
	char *fstr = page ? "#XPG" : "#X";

	if (id == 0) { // pass empty string as link, filename is sufficient
		CurrArea->ref = NewName("");
		if (!GetBaseName(CurrArea->file, CurrArea->base))
			CurrArea->base = NewName(CurrArea->file);
	}
	else if (UseXrefFilePrefix) {
		CurrArea->ref = NewName(strlen(fstr) + strlen(XrefFilePrefix) + NumStrMax);
		sprintf(CurrArea->ref, "%s%s%ld", fstr, XrefFilePrefix, id);
		CurrArea->base = CurrArea->ref + 1;
	}
	else {
		CurrArea->ref = NewName(strlen(fstr) + NumStrMax);
		sprintf(CurrArea->ref, "%s%ld", fstr, id);
		CurrArea->base = CurrArea->ref + 1;
	}

	XrefFilePrefix = CurrFilePrefix;
}


void
HTMwr::ProcGrTextHyperTarget(HTMctl *cp)
{
	if (!CurrArea)
		return;

	CurrArea->target = NewName((char *) cp->ex(), cp->siz());
}


void
HTMwr::ProcGrTextHyperURL(HTMctl *cp)
{
	if (FMSource) {
		if (CurrArea)  // not first
			return;
		CurrArea = new area;
	}

	CurrArea->file = NULL;
	CurrArea->url = true;
	CurrArea->ref = NewName((char *) cp->ex(), cp->siz());

	if (!GetBaseName(CurrArea->ref, CurrArea->base))
		CurrArea->base = NewName(CurrArea->ref);
}


void
HTMwr::ProcGrTextHyperAlert(HTMctl *cp)
{
	if (FMSource
	 && CurrArea)  // not first
		return;

	if (HelpType != 'M')
		return;

	if (FMSource)
		CurrArea = new area;

	CurrArea->file = NULL;
	CurrArea->url = false;
	CurrArea->popup = true;
	CurrArea->base = NULL;

	char *txt = (char *) cp->ex();

	if (!_strnicmp(txt, "alert ", 6))
		txt += 6;

	CurrArea->ref = NewName(txt);
}


void
HTMwr::ProcGrTextHyperFile(HTMctl *cp)
{
	HyperLinkIDNext = true;
	SetXrefFileName((char *) cp->ex(), cp->siz());
	GrTextHyperFileName = XrefFile;
	XrefFile = NULL;
}



#ifdef UNUSED

void
HTMwr::WriteFrameStart(HTMwfile *wf, frame *fr)
{
  wf->putCW("pard");
  wf->putCW("plain");
  Last = DefForm;

  // put box
  if (fr->frul[0]) {
    wf->putCW("box");
    WriteRule(wf, fr->frul);
    wf->putCWN("brsp", fr->frulsp);
    wf->putEnd();
  }

  // put positioning
  if ((fr->vref) && (fr->vref <= FrameVRMax))
    wf->putCW(FrameVRef[fr->vref - 1]);
  if ((fr->href) && (fr->href <= FrameHRMax))
    wf->putCW(FrameHRef[fr->href - 1]);

  if (fr->halgn < FrameHAMax) {
    if (fr->halgn)
      wf->putCW(FrameHAlgn[fr->halgn]);
    else
      wf->putCWN(FrameHAlgn[0], fr->xpos);
  }

  if (fr->valgn < FrameVAMax) {
    if (fr->valgn)
      wf->putCW(FrameVAlgn[fr->valgn]);
    else
      wf->putCWN(FrameVAlgn[0], fr->ypos);
  }

  if (fr->wide)
    wf->putCWN("absw", fr->wide);
  if (fr->xspace)
    wf->putCWN("dxfrtext", fr->xspace);
  wf->putEnd();
}


void
HTMwr::WriteFrameCont(HTMwfile *wf, frame *fr)
{
  HTMgroup *gp;

  if (fr->ftext) {
    gp = (HTMgroup *) FlowList.find(fr->ftext);
    if (gp)
      gp->write(*wf);
    return;
  }

#ifdef UNUSED
  graph *gr;
  if (fr->grcount) {
    WriteWMFStart(wf, fr);
    for (uns i = 1; i <= fr->grcount; i++) {
      gr = (graph *) ((fr->grlist)->find(i));
      WriteGraphic(wf, gr);
    }
    WriteWMFEnd(wf, fr);
    if (fr->frcount) {
      wf->putCW("par");
      wf->putGE();
      wf->putEnd();
      wf->putGS();
    }
  }

  frame *nf;
  if (fr->frcount) {
    for (uns i = 1; i <= fr->frcount; i++) {
      nf = (frame *) ((fr->frlist)->find(i));
      wf->putGS();
      WriteFrameStart(wf, nf);
      WriteFrameCont(wf, nf);
      wf->putCW("par");
      wf->putGE();
      wf->putEnd();
    }
  }
  if (!fr->frcount && !fr->grcount)
    wf->putStr("Empty ");  // temporary
#endif
}

#endif


// graphic processing

bool HTMwr::InGraphic = false;
bool HTMwr::InImpObj = false;

#ifdef UNUSED

graph *HTMwr::Graphic;
uns HTMwr::GrGroupStack[GrGroupMax];
uns HTMwr::GrGroupLevel;

uns HTMwr::GrFillMap[GrFillMax] = {
#if 0
  0, 255, 230, 180, 128, 76, 26, 8, 0,
#endif
  0, 0, 26, 76, 128, 180, 230, 248, 255, // multiplier for color byte
  2, 3, 1, 0, 5, 2, 3
};


void
HTMwr::ProcGraphic(HTMctl *cp)
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
  if (Verbose)
    printf("Graphic ");
}


void
HTMwr::EndGraphic(HTMctl *cp)
{
  InGraphic = false;
  if ((FrameLevel == 0)     // disable graphics directly on page for now
   || (Graphic->gsize == 0) // ignore empty graphics
   || ((Graphic->gsize == 6) && (Graphic->gtype == 0x521))) {
    delete Graphic;
    if (Verbose)
      printf("empty\n");
    return;
  }
  if (Verbose)
    printf("\n");

  frame *fr = FrameStack[FrameLevel - 1];
  fr->grcount++;
  if (!fr->grlist)
    fr->grlist = new DCvlist;
  (fr->grlist)->add(Graphic, fr->grcount);

  if (memcmp(&CurrPen, &LastPen, sizeof(wmf_pen))) {
    *(Graphic->gpen = new wmf_pen) = LastPen = CurrPen;
    fr->whead.fsize += 16;
  }

  if (memcmp(&CurrBrush, &LastBrush, sizeof(wmf_brush))) {
    *(Graphic->gbrush = new wmf_brush) = LastBrush = CurrBrush;
    fr->whead.fsize += 15;
  }

  if (memcmp(&CurrFont, &LastFont, sizeof(wmf_font))) {
    *(Graphic->gfont = new wmf_font) = LastFont = CurrFont;
    fr->whead.fsize += 36;
  }

  fr->whead.fsize += Graphic->gsize;
  if (Graphic->gsize > fr->whead.recmax)
    fr->whead.recmax = Graphic->gsize;
}


void
HTMwr::ProcGrLine(HTMctl *cp)
{
  switch(cp->dat()) {
    case 1:  // line pattern
      CurrPen.pstyle = cp->siz() ? 0 : 5;
      break;
    case 2:  // line color
      if (cp->siz() < ColorMapMax) {
        uns cind = ColorMap[cp->siz()];
        if (cind == Invisible)
          CurrPen.pstyle = 1;
        else {
          cind -= 2;
          CurrPen.pcolor = ((((long) RedTable[cind]) << 16) & 0xFF0000)
                         + ((((long) GreenTable[cind]) << 8) & 0xFF00)
                         + (BlueTable[cind] & 0xFF);
        }
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
HTMwr::ProcGrPen(HTMctl *cp)
{
  switch(cp->dat()) {
    case 1:  // pen fill pattern
      CurrPen.pstyle = cp->siz() ? 0 : 5;
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
HTMwr::ProcGrFill(HTMctl *cp)
{
  switch(cp->dat()) {
    case 1:  // brush fill pattern
      CurrBrush.bstyle = 0;
      CurrBrush.bshade = 0;
      if (cp->siz() == 0)
        CurrBrush.bstyle = 1;
      else if ((cp->siz() > 1)
            && (cp->siz() < GrFillMax)) {
        if (GrFillMap[cp->siz()] < 6) {
          CurrBrush.bhatch = GrFillMap[cp->siz()];
          CurrBrush.bstyle = 2;
        }
        else CurrBrush.bshade = GrFillMap[cp->siz()];
      }
      break;
    case 2:  // fg color
      if (cp->siz() < ColorMapMax) {
        uns cind = ColorMap[cp->siz()];
        if (cind == Invisible)
          CurrBrush.bstyle = 1;
        else {
          cind -= 2;
          CurrBrush.bcolor = ((((long) RedTable[cind]) << 16) & 0xFF0000)
                           + ((((long) GreenTable[cind]) << 8) & 0xFF00)
                           + (BlueTable[cind] & 0xFF);
        }
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
HTMwr::ProcGrShape(HTMctl *cp)
{
  if (Verbose)
    printf("shape %d ", cp->dat());

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
HTMwr::ProcGrPoly(HTMctl *cp)
{
  uns len = (uns) (cp->siz() / sizeof(long));
  long *lptr = (long *) cp->ex();
  Graphic->gsize = 4 + len;
  Graphic->gparm = new uns[len + 1];
  Graphic->gparm[0] = len / 2;
  for (uns i = 0; i < len; i++)
    Graphic->gparm[i + 1] = GetTwips(lptr[i], pg_unit);
}


void
HTMwr::ProcGrRect(HTMctl *cp, uns rtyp)
{
  uns len = (uns) (cp->siz() / sizeof(long));
  long *lptr = (long *) cp->ex();
  if (rtyp == 1) {
    Graphic->gsize = 7;
    Graphic->gparm = new uns[4];
    Graphic->gparm[0] = GetTwips(lptr[1] + lptr[3], pg_unit);
    Graphic->gparm[1] = GetTwips(lptr[0] + lptr[2], pg_unit);
    Graphic->gparm[2] = GetTwips(lptr[1], pg_unit);
    Graphic->gparm[3] = GetTwips(lptr[0], pg_unit);
  }
  else if (rtyp == 2) {
    Graphic->gsize = 9;
    Graphic->gparm = new uns[6];
    Graphic->gparm[0] = GetTwips(lptr[5] * 2, pg_unit);
    Graphic->gparm[1] = GetTwips(lptr[4] * 2, pg_unit);
    Graphic->gparm[2] = GetTwips(lptr[1] + lptr[3], pg_unit);
    Graphic->gparm[3] = GetTwips(lptr[0] + lptr[2], pg_unit);
    Graphic->gparm[4] = GetTwips(lptr[1], pg_unit);
    Graphic->gparm[5] = GetTwips(lptr[0], pg_unit);
  }
}


void
HTMwr::ProcGrEllipse(HTMctl *cp)
{
  uns len = (uns) (cp->siz() / sizeof(long));
  long *lptr = (long *) cp->ex();
  Graphic->gsize = 7;
  Graphic->gparm = new uns[4];
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
}


#ifndef PI
#define PI 3.14159
#endif

void
HTMwr::ProcGrArc(HTMctl *cp)
{
  uns len = (uns) (cp->siz() / sizeof(long));
  long *lptr = (long *) cp->ex();
  Graphic->gsize = 11;
  Graphic->gparm = new uns[8];

  // convert start and end angles to points at or past arc from center
  double endang = (((double) lptr[6]) * PI) / 18000.0;
  double begang = (((double) lptr[5]) * PI) / 18000.0;

  Graphic->gparm[0] =
    GetTwips(lptr[1] - (long) floor(sin(endang) * (double) lptr[2]), pg_unit);
  Graphic->gparm[1] =
    GetTwips(lptr[0] + (long) floor(cos(endang) * (double) lptr[2]), pg_unit);
  Graphic->gparm[2] =
    GetTwips(lptr[1] - (long) floor(sin(begang) * (double) lptr[2]), pg_unit);
  Graphic->gparm[3] =
    GetTwips(lptr[0] + (long) floor(cos(begang) * (double) lptr[2]), pg_unit);

  if ((lptr[4] == 0) || (lptr[4] == 18000)) {
    Graphic->gparm[4] = GetTwips(lptr[1] + lptr[3], pg_unit);
    Graphic->gparm[5] = GetTwips(lptr[0] + lptr[2], pg_unit);
    Graphic->gparm[6] = GetTwips(lptr[1] - lptr[3], pg_unit);
    Graphic->gparm[7] = GetTwips(lptr[0] - lptr[2], pg_unit);
  }
  else if ((lptr[4] == 9000) || (lptr[4] == 27000)) {
    Graphic->gparm[4] = GetTwips(lptr[1] + lptr[2], pg_unit);
    Graphic->gparm[5] = GetTwips(lptr[0] + lptr[3], pg_unit);
    Graphic->gparm[6] = GetTwips(lptr[1] - lptr[2], pg_unit);
    Graphic->gparm[7] = GetTwips(lptr[0] - lptr[3], pg_unit);
  }
}


uns HTMwr::BezierPts;
uns HTMwr::BezPtSpace;
long HTMwr::BezDistMax;


void
HTMwr::ProcGrBezier(HTMctl *cp)
{
  BezDistMax = GetOrig(5L, pg_unit); // flatness measure for 1 pixel at 300dpi

  uns bezct = (uns) (((cp->siz() / sizeof(long)) - 2) / 6);
  long *lptr = (long *) cp->ex();
  Graphic->gparm = (uns *) NewMem(((BezPtSpace = bezct * 4) * 2) + 1);
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
  Graphic->gparm = (uns *) ResizeMem(Graphic->gparm, (BezierPts * 2) + 1);
  Graphic->gsize = 4 + BezierPts;
  Graphic->gparm[0] = BezierPts / 2;
}


void
HTMwr::ProcBez(long *bezpt)
{
  if (BezFlat(bezpt)) {
    Graphic->gparm[++BezierPts] = GetTwips(bezpt[0], pg_unit);
    Graphic->gparm[++BezierPts] = GetTwips(bezpt[1], pg_unit);
    if (BezierPts > (BezPtSpace - 4)) {
      BezPtSpace *= 2;
      Graphic->gparm = (uns *) ResizeMem(Graphic->gparm, (BezPtSpace * 2) + 1);
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
HTMwr::BezFlat(long *bezpt)
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


gtext *HTMwr::GText;

void
HTMwr::ProcGrText(HTMctl *cp)
{
  if (Verbose)
    printf("text ");
  GText = new gtext;
  Graphic->gtype = 0x521;
  Graphic->gparm = (uns *) GText;
  Graphic->gsize = 6;

  GText->gform = (cp->dat() - 1);
  long *lptr = (long *) cp->ex();
  GText->axpos = GetTwips(lptr[0], pg_unit);
  GText->aypos = GetTwips(lptr[1], pg_unit);
}


uns HTMwr::GrFWeight[GrFWeightMax] = {
  400, 100, 300, 600, 700, 800, 900
};


void
HTMwr::ProcGrTProp(HTMctl *cp)
{
  char *fntname = NULL;

  switch ((cp->rp)->write_op) {  // processing during scan phase
    case putfont:
      fntname = GetFontName(cp);
      if ((fntname == NULL) || (*fntname == '\0'))
        break;
      strncpy(CurrFont.fname, fntname, WmfFNameMax);
      CurrFont.fnum = (uns) FontNameList(fntname);
      break;
    case putfsize:
      CurrFont.fhigh = GetFontSize(cp) * 10;
      break;
    case putbold:
      if (cp->siz() < GrFWeightMax)
        CurrFont.fweight = GrFWeight[cp->siz()];
      break;
    case putital:
      CurrFont.fital = cp->siz() ? 1 : 0;
      break;
    case putrevb:
      CurrFont.fstrik = (cp->siz() == 2) ? 1 : 0;
      break;
    case putline:
      CurrFont.fundl = cp->siz() ? 1 : 0;
      break;
    case putcolr:
      break;
    default:
      break;
  }
}


void
HTMwr::ProcGrTStr(HTMctl *cp)
{
  size_t i = cp->siz();
  char *sptr = (char *) cp->ex();
  if ((i == 0) || (sptr == NULL))
    return;

  if (GText->tptr)
    GText->tptr = (char *) ResizeMem(GText->tptr, GText->tcount + i);
  else GText->tptr = (char *) NewMem(i);
  char *dptr = GText->tptr + GText->tcount;
  GText->thigh = CurrFont.fhigh;
  
  // write chars into string, update twide
  uns chwid = (FCharWids[CurrFont.fnum] * CurrFont.fhigh) / 240;
  bool chfix = ((CurrFont.fnum == 3)
             || (CurrFont.fnum == 14) || (CurrFont.fnum == 15));
  uns strsize = 0;
  for (; i && *sptr; i--, sptr++) {
    switch (*sptr) {
      case '\\':
        i--, sptr++;
        switch (*sptr) {
          case 'r':
          case 'n':
            strsize++;
            *dptr++ = '\n';
            continue;
          case ' ':
          case '!':
          case '#':
          case '+':
            *sptr = ' ';
            break;
          case '-':
          case '/':
            break;
          default:
            continue;
        }
        break;
      case '\n':
        continue;
      default:
        break;
    }
    *dptr++ = *sptr;
    strsize++;
    unc ch = *((unc *) sptr);
    if ((chfix == true) || (ch > CharMetricMax))
      GText->twide += chwid;
    else GText->twide += (chwid * CharMetrics[ch]) / 56;
  }
  *dptr = '\0';

  // update counters
  GText->tcount += strsize;
  GText->tptr = (char *) ResizeMem(GText->tptr, GText->tcount + 1);
  Graphic->gsize = 6 + ((GText->tcount + 1) / 2);
}



void
HTMwr::WriteGraphic(HTMwfile *wf, graph *gr)
{
  if (gr->gpen) {
    WriteWMFDel(wf, 0);
    WriteWMFPen(wf, gr->gpen);
  }
  if (gr->gbrush) {
    WriteWMFDel(wf, 1);
    WriteWMFBrush(wf, gr->gbrush);
  }
  if (gr->gfont) {
    WriteWMFDel(wf, 2);
    WriteWMFFont(wf, gr->gfont);
  }

  wf->putEnd();
  wf->putML(gr->gsize);  // record length
  wf->putMS(gr->gtype);  // record function

  if (gr->gtype == 0xF43)
    WriteGrRaster(wf, (rast *) gr->gparm);
  else if (gr->gtype == 0x521)
    WriteGrText(wf, (gtext *) gr->gparm);
  else {
    for (unl i = 0; i < (gr->gsize - 3); i++)
      wf->putMS(gr->gparm[i]);
  }
}


void
HTMwr::WriteGrText(HTMwfile *wf, gtext *gt)
{
  wf->putMS(gt->tcount);
  for (uns i = 0; i < gt->tcount; i++)
    wf->putMB(gt->tptr[i]);
  if (i & 1)
    wf->putMB(0);
  wf->putChar(' ');
  wf->putMS(gt->aypos - gt->thigh);
  wf->putMS(gt->axpos - ((gt->gform == 2) ? gt->twide :
                        ((gt->gform == 1) ? (gt->twide / 2) : 0)));
}


void
HTMwr::WriteWMFStart(HTMwfile *wf, frame *fr)
{
  wf->putGS();
  wf->putCW("pict");
  wf->putCW("wmetafile8");  // anisotropic mapping mode
  wf->putCWN("picw", fr->wide);
  wf->putCWN("pich", fr->high);
  wf->putCWN("picwgoal", fr->wide);
  wf->putCWN("pichgoal", fr->high);
  wf->putEnd();

  // metafile header
  wf->putMS(1); // disk metafile
  wf->putMS(9); // header length
  wf->putMS(0x300);  // version for Windows 3.x
  wf->putML(fr->whead.fsize);  // metafile size
  wf->putMS(fr->whead.objmax);  // max object count
  wf->putML(fr->whead.recmax);  // max record size
  wf->putMS(0); // unused

  wf->putEnd();
  wf->putML(3);      // record length
  wf->putMS(0x01E);  // record function, save DC

  wf->putEnd();
  wf->putML(5);      // record length
  wf->putMS(0x20B);  // record function
  wf->putMS(0);      // window org y
  wf->putMS(0);      // window org x

  wf->putEnd();
  wf->putML(5);      // record length
  wf->putMS(0x20C);  // record function
  wf->putMS(fr->high);  // window ext y
  wf->putMS(fr->wide);  // window ext x

  wf->putEnd();
  wf->putML(5);      // record length
  wf->putMS(0x209);  // record function
  wf->putML(0x00);   // text color black

  wf->putEnd();
  wf->putML(5);      // record length
  wf->putMS(0x201);  // record function
  wf->putML(0x00FFFFFF);   // background color white

  wf->putEnd();
  wf->putML(4);      // record length
  wf->putMS(0x106);  // record function
  wf->putMS(1);      // poly fill mode = alternate

  wf->putEnd();
  wf->putML(4);      // record length
  wf->putMS(0x102);  // record function
  wf->putMS(2);      // background mode = fill

  wf->putEnd();
  wf->putML(4);      // record length
  wf->putMS(0x104);  // record function
  wf->putMS(13);     // ROP2 mode = overlay

  wf->putEnd();
  wf->putML(4);      // record length
  wf->putMS(0x107);  // record function
  wf->putMS(3);      // stretch blt mode = color-on-color

  WriteWMFPen(wf, &DefaultPen);
  WriteWMFBrush(wf, &DefaultBrush);
  WriteWMFFont(wf, &DefaultFont);
}


void
HTMwr::WriteWMFEnd(HTMwfile *wf, frame *fr)
{
  WriteWMFDel(wf, 0);
  WriteWMFDel(wf, 1);
  WriteWMFDel(wf, 2);

  wf->putEnd();
  wf->putML(4);      // record length
  wf->putMS(0x127);  // record function, restore DC
  wf->putMS(-1); // last DC, -1

  wf->putEnd();
  wf->putML(3);  // terminal record length
  wf->putMS(0);  // terminal record function 0
  wf->putGE();
  wf->putEnd();
}


wmf_pen HTMwr::DefaultPen = { 0, 0, 0x00 }; // solid black, min thick
wmf_pen HTMwr::CurrPen;
wmf_pen HTMwr::LastPen;

void
HTMwr::WriteWMFPen(HTMwfile *wf, wmf_pen *pen)
{
  wf->putEnd();
  wf->putML(8);      // record length
  wf->putMS(0x2FA);  // record function
  wf->putMS(pen->pstyle);
  wf->putMS(pen->pwidth);
  wf->putMS(pen->pwidth);
  wf->putML(pen->pcolor);
  WriteWMFSel(wf, 0); // select as object 0
  CurrPen = *pen;
}


wmf_brush HTMwr::DefaultBrush;  // solid black
wmf_brush HTMwr::CurrBrush;
wmf_brush HTMwr::LastBrush;

void
HTMwr::WriteWMFBrush(HTMwfile *wf, wmf_brush *br)
{
  wf->putEnd();
  wf->putML(7);      // record length
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
HTMwr::ShadeColor(long colr, uns shd)
{
  long scolr = 0;
  long ncolr;

  for (uns i = 0; i < 3; i++) {
    ncolr = ((((colr >> (8 * i)) & 0xFF) * (256 - shd)) / 256) + shd;
    scolr += (ncolr > 255 ? 255 : ncolr) << (8 * i);
  }
  return scolr;
}


wmf_font HTMwr::DefaultFont = { "Palatino", 0L, 200, 1, 400, 0, 0, 0, 0 } ;
wmf_font HTMwr::CurrFont;
wmf_font HTMwr::LastFont;

void
HTMwr::WriteWMFFont(HTMwfile *wf, wmf_font *fnt)
{
  wf->putEnd();
  wf->putML(28);      // record length
  wf->putMS(0x2FB);  // record function

  wf->putMS(-(fnt->fhigh));
  wf->putMS(0);
  wf->putMS(0);
  wf->putMS(0);
  wf->putMS(fnt->fweight);
  wf->putMB(fnt->fital);
  wf->putMB(fnt->fundl);
  wf->putMB(fnt->fstrik);
  wf->putMB(0);
  wf->putMB(0);
  wf->putMB(0);
  wf->putMB(1);
  wf->putMB(0);
  wf->putEnd();

  wf->putChar(' ');
  if (fnt->fname[0] == '\0')
    strcpy(fnt->fname, "Palatino");
  for (uns i = 0; i < WmfFNameMax; i++)
    wf->putMB(fnt->fname[i]);

  WriteWMFSel(wf, 2); // select as object 2
  CurrFont = *fnt;
}


void
HTMwr::WriteWMFSel(HTMwfile *wf, uns num)
{
  wf->putEnd();
  wf->putML(4);      // record length
  wf->putMS(0x12D);  // record function
  wf->putMS(num);    // select object
}


void
HTMwr::WriteWMFDel(HTMwfile *wf, uns num)
{
  wf->putEnd();
  wf->putML(4);      // record length
  wf->putMS(0x1F0);  // record function
  wf->putMS(num);    // delete object
}

#endif



// object processing, mainly passthrough for DITA source

bool HTMwr::InObject = false;
char *HTMwr::ObjectDesc = NULL;


void 
HTMwr::StartObject(HTMwfile *wf, DCctl *cp)
{
	InObject = true;
	ObjectDesc = NULL;

	wf->putEnd();
	wf->startBlock("object");
}


void 
HTMwr::SetObjectIntProp(HTMwfile *wf, DCctl *cp)
{
	char *aname = NULL;
	char *aval = NULL;
	char abuf[34];

	switch(cp->dat()) {
		case 7:
			aname = "align";
			switch (cp->siz()) {
				case 1:
					aval = "left";
					break;
				case 2:
					aval = "right";
					break;
				case 3:
					aval = "center";
					break;
				case 4:
					aval = "texttop";
					break;
				case 5:
					aval = "textmiddle";
					break;
				case 6:
					aval = "textbottom";
					break;
				case 7:
					aval = "middle";
					break;
				case 8:
					aval = "baseline";
					break;
				default:
					aname = NULL;
			}
			break;

		case 8:
			aname = "height";
			aval = _itoa(PixelsFromTwips((short) (cp->siz() / 5)), abuf, 10);
			break;
		case 9:
			aname = "width";
			aval = _itoa(PixelsFromTwips((short) (cp->siz() / 5)), abuf, 10);
			break;
		case 10:
			aname = "hspace";
			aval = _itoa(PixelsFromTwips((short) (cp->siz() / 5)), abuf, 10);
			break;
		case 11:
			aname = "vspace";
			aval = _itoa(PixelsFromTwips((short) (cp->siz() / 5)), abuf, 10);
			break;
		case 12:
			aname = "border";
			aval = _itoa((int) cp->siz(), abuf, 10);
			break;

		case 13:
			aname = "declare";
			aval = "declare";
			break;

		default:
			break;
	}

	if (aname) {
		wf->startAttr(aname);
		wf->putStr(aval);
		wf->endAttr();
	}
}


void 
HTMwr::SetObjectTextProp(HTMwfile *wf, DCctl *cp)
{
	char *aname = NULL;
	char *aval = (char *) cp->ex();

	switch(cp->dat()) {
		case 1:
			aname = "data";
			break;
		case 2:
			aname = "classid";
			break;
		case 3:
			aname = "type";
			break;
		case 4:
			aname = "codebase";
			break;
		case 5:
			aname = "codetype";
			break;
		case 6:
			aname = "id";
			break;
		case 14:
			aname = "standby";
			break;

		case 20:
			ObjectDesc = NewName(aval);
			break;

		default:
			break;
	}

	if (aname) {
		wf->startAttr(aname);
		wf->putStr(aval);
		wf->endAttr();
	}
}


void 
HTMwr::StartObjectParam(HTMwfile *wf, DCctl *cp)
{
	if (InObject) {
		wf->endElem(true);
		wf->putEnd();
		InObject = false;
	}
	wf->startElem("param");
}


void 
HTMwr::EndObjectParam(HTMwfile *wf, DCctl *cp)
{
	wf->endElem();
	wf->putEnd();
}


void 
HTMwr::SetObjectParamName(HTMwfile *wf, char *chp)
{
	wf->startAttr("name");
	wf->putStr(chp);
	wf->endAttr();
}


void 
HTMwr::SetObjectParamVal(HTMwfile *wf, char *chp)
{
	wf->startAttr("value");
	wf->putStr(chp);
	wf->endAttr();
}


void 
HTMwr::SetObjectParamType(HTMwfile *wf, unl typ)
{
	if ((typ < 2) || (typ > 3))
		return;

	switch (typ) {
		case 1:
		default:
			wf->startAttr("data");
			wf->putStr("data");
			break;

		case 2:
			wf->startAttr("ref");
			wf->putStr("ref");
			break;

		case 3:
			wf->startAttr("object");
			wf->putStr("object");
			break;
	}
	wf->endAttr();
}


void 
HTMwr::PutObject(HTMwfile *wf, DCctl *cp)
{
	if (InObject) {
		wf->endElem(true);
		wf->putEnd();
		InObject = false;
	}

	if (ObjectDesc)
		wf->putTextStr(ObjectDesc);

	wf->endBlock("object");
	wf->putEnd();
}





// end of dwhtmgr.cc


