 
// dwrtfgt.cc is the code for dcl Microsoft RTF graphic text, JHG
// Copyright (c) 1993-1998 by Omni Systems, Inc.  All rights reserved.

#include "dcappl.h"
#include "dcctlg.h"
#include "dcfili.h"
#include "dctxfx.h"
#include "dcfmtr.h"
#include "dcini.h"
#include "dcmacr.h"
#include "dwrtf.h"
#include "dwrtffl.h"
#include "dcicu.h"



// graphic text functions


gtext *RTFwr::GText;
wmf_font ParaFont;
uns RTFwr::GTAlignChange = 0;
uns RTFwr::GTWidth = 0;
DClist RTFwr::GTFixList;


void
RTFwr::ProcGrText(RTFctl *cp)
{
  if (Verbose)
    printf("callout text ");
  GText = new gtext;
  Graphic->gtype = 0x521;
  Graphic->gparm = (short *) GText;
  Graphic->gsize = 6;
	Graphic->gfont = NULL;

  GText->gform = (cp->dat() - 1);
	// if center or right align, watch for need to fix as left
	GTAlignChange = ((GText->gform == 1) || (GText->gform == 2)) ? 1 : 0;
	GTWidth = 0;
	GText->hinc = 0;
  long *lptr = (long *) cp->ex();
  GText->hpos = GetTwips(lptr[0], pg_unit);
  GText->vpos = GetTwips(lptr[1], pg_unit);
	GText->fwide = GText->fhigh = 0;
	CurrFont.fangle = (lptr[2] + 5) / 10;  // rotation angle
	CurrFont.fbase = 0;
	ParaFont = CurrFont;
	Graphic->gypos = GText->vpos;
	GText->axpos = 0;
	GText->aypos = 0;
	switch (CurrFont.fangle) {
	 case 0:
	 case 3600:
		GTxmult = 0.0;
		GTymult = 1.0;
		//GTxpos = 0;
		//GTypos = 0;
		break;
	 case 900:
		GTxmult = 1.0;
		GTymult = 0.0;
		//GTypos = txt_fr->high;
		break;
	 case 1800:
		GTxmult = 0.0;
		GTymult = -1.0;
		//GTxpos = txt_fr->wide;
		//GTypos = txt_fr->high;
		break;
	 case 2700:
		GTxmult = -1.0;
		GTymult = 0.0;
		//GTxpos = txt_fr->wide;
		break;
	 default:
		double ang = (((double) CurrFont.fangle) * PI) / 1800.0;
		GTxmult = sin(ang);
		GTymult = cos(ang);
		// compute change in origin ****
		break;
	}
}



// text frame conversion to graphic text


bool RTFwr::InGrTextFrame = false;
bool RTFwr::NewTextGrNeeded = false;
bool RTFwr::NewTextGrLine = false;
bool RTFwr::NewTextGrPara = false;
bool RTFwr::NewTextGrBase = false;
bool RTFwr::GTDiscHyphen = false;
bool RTFwr::NoGrText = false;
bool RTFwr::NoGrTab = false;
bool RTFwr::FirstGTPara = true;
bool RTFwr::FirstGTLine = true;
double RTFwr::GTxmult = 0.0;
double RTFwr::GTymult = 1.0;
short RTFwr::GTxpos = 0;
short RTFwr::GTypos = 0;
short RTFwr::GTylim = 0;
short RTFwr::GTTabPos = 0;
short RTFwr::GTPSpace = 0;
short RTFwr::GTFSpace = 0;
RTFgroup *RTFwr::GTAnum = NULL; 
bool RTFwr::GTAnumProc = false;
RTFgroup *RTFwr::GTVar = NULL;
short RTFwr::GTShLeft = 0; 
short RTFwr::GTShRight = 0; 


void
RTFwr::ConvertTextFrame(frame *txt_fr, frame *par_fr)
{
	shprop *shp = NULL;

	FrameLevel++;
	FrameStack[FrameLevel - 1] = txt_fr; // set containing frame
	InGrTextFrame = true;
	NewTextGrNeeded = false;
	NewTextGrLine = false;
	NewTextGrPara = false;
	NewTextGrBase = false;
	NoGrText = false;
	NoGrTab = false;
	FirstGTPara = true;
	FirstGTLine = true;
	InGraphic = true;
	ClipText = par_fr->clip ? true : false;

	GTShLeft = 0;
	GTShRight = 0;
	if (SideHFlowList.find(txt_fr->ftext)) { // sideheads on for flow
		long fnum = 0;
		if ((fnum = FrameList(txt_fr)) != 0)
			shp = (shprop *) SideHFrameList.find(fnum);
		if (!shp)  // last chance, if FM 4.x
			shp = (shprop *) SideHTFlowList.find(txt_fr->ftext);
	}
	if (shp) {
		if (shp->shpos == 1)      // on left
			GTShLeft = shp->shwid + shp->shgap;
		else if (shp->shpos == 2) // on right
			GTShRight = txt_fr->wide - shp->shwid;
	}

	// may be filled or ruled frame, draw rect
	Graphic = new graph;
	Graphic->gtype = 0x41B;
  Graphic->gsize = 7;
  Graphic->gparm = new short[4];
  Graphic->gparm[0] = txt_fr->high;
  Graphic->gparm[1] = txt_fr->wide;
  Graphic->gparm[2] = 0;
  Graphic->gparm[3] = 0;
	EndGraphic(NULL); // insert into grlist

	CurrPen = LastPen;
	CurrBrush = LastBrush;
	CurrFont = LastFont;

	Graphic = new graph;
	GText = new gtext;
	Graphic->gtype = 0x521;
	Graphic->gparm = (short *) GText;
	Graphic->gsize = 6;
	Graphic->gpen = NULL;
	Graphic->gbrush = NULL;
	Graphic->gfont = NULL;
	GText->gform = 0; // default to flush left

	// set graph info from txt_fr frame
	GText->axpos = 0; // will be added at WriteGrText
	GText->aypos = 0;
	GText->hpos = 0;
	GText->vpos = GrTxVAdj;

	GTxmult = 0.0;    // to correct for rotation
	GTymult = 1.0;
	GTxpos = 0;
	GTypos = 0;
	CurrFont.fangle = 0;

	GText->fwide = txt_fr->wide;
	GText->fhigh = txt_fr->high;
	GTTabPos = 0;
	GTPSpace = 0;
	GTFSpace = 0;

	if (txt_fr->frot) {  // rotated frame
		CurrFont.fangle = (txt_fr->frot + 5) / 10;  // rotation angle
		// must adjust origin, top left isn't it
		// also set x and y multipliers by which origin
		//  must be adjusted for added lines
		switch (CurrFont.fangle) {
		 case 0:
		 case 3600:
			GTxmult = 0.0;
			GTymult = 1.0;
			GTxpos = 0;
			GTypos = 0;
			break;
		 case 900:
			GTxmult = 1.0;
			GTymult = 0.0;
			GTypos = txt_fr->high;
			break;
		 case 1800:
			GTxmult = 0.0;
			GTymult = -1.0;
			GTxpos = txt_fr->wide;
			GTypos = txt_fr->high;
			break;
		 case 2700:
			GTxmult = -1.0;
			GTymult = 0.0;
			GTxpos = txt_fr->wide;
			break;
		 default:
			double ang = (((double) CurrFont.fangle) * PI) / 1800.0;
			GTxmult = sin(ang);
			GTymult = cos(ang);
			// compute change in origin ****
			break;
		}
	}

	if ((txt_fr->ypos + txt_fr->high) > par_fr->high)
		GTylim = (par_fr->high - txt_fr->ypos) + ClipLimit;
	else
		GTylim = txt_fr->high + ClipLimit;

	// process contained text ctls (formats, props, text)
	// may change Graphic and GText and GrListPos
  RTFgroup *gp = (RTFgroup *) FlowList.find(txt_fr->ftext);
	if (gp)
		gp->execu(ProcGrTx);

	EndGTextLine();

	EndGraphic(NULL); // insert into grlist

	InGrTextFrame = false;
	FrameLevel--;
}


void
RTFwr::EndGTextLine()
{
	// adjust vpos to move from top to baseline
	GText->vpos += (GText->thigh * 3) / 4;

	if (GText->vpos > GTylim) {	 // clipped
		if (ClipType == 'D')       // delete entirely
 			Graphic->gsize = 0;
		else if ((ClipType == 'M') // move up
		 || (ClipText == false))   // old option
			GText->vpos = GTylim - ClipLimit;
	}

	// translate to (possibly rotated) position	in frame
	double len = (double) GText->vpos;
	GText->axpos = GTxpos + (short) (GTxmult * len);
	GText->aypos = GTypos + (short) (GTymult * len);

	// adjust hpos for alignment
	if (GTAlignChange < 2) { // can adjust now
		if (GText->gform == 1)  // center
			GText->hpos += (GText->fwide / 2);
		else if (GText->gform == 2)  // right
			GText->hpos += GText->fwide;

		if (GText->hpos > 0) {  // correct for horizontal offset
			double len = (double) GText->hpos;
			GText->axpos += (short) (GTymult * len);
			GText->aypos -= (short) (GTxmult * len);
		}
	}
}


void
RTFwr::FixGTHorPosition(gtext *gt, short wid)
{
	if (!gt
	 || (gt->gform == 0))
		return;

	short fix = 0;

	if (gt->gform == 1)  // center
		fix = (wid / 2);
	else if (gt->gform == 2)  // right
		fix = wid;

	if (gt->hinc)
		fix -= gt->hinc;

	gt->gform = 0;

	gt->axpos = gt->hpos;
	gt->aypos = gt->vpos;

	if (fix != 0) {  // correct for horizontal offset
		double len = (double) -fix;
		gt->axpos += (short) (GTymult * len);
		gt->aypos -= (short) (GTxmult * len);
	}
}


void
RTFwr::GTFixup(long gtl)
{
	gtext *gt = (gtext *) gtl;
	FixGTHorPosition(gt, GTWidth);
}


void
RTFwr::GenNewTextGraphic()
{
	graph *ngr = new graph;	 // allocate new objects
	gtext *ngt = new gtext;

	// add any para space before line if not at top
	if (FirstGTPara
	 && !UseTopSpaceAbove)
		GTPSpace = 0;
	else if (NewTextGrPara
	 || (FirstGTPara && UseTopSpaceAbove)) {
		GText->vpos += GTPSpace;
		GTPSpace = 0; // only once
	}

	if (GTFSpace) {  // add any frame spacing
		GText->vpos += GTFSpace;
		GTFSpace = 0; // just once
	}

	if (GText->hpos == 0) {  // at start, use indent
		if (FirstGTLine == true) {
			GText->hpos = GText->find;
			if (NewTextGrLine == true)
				FirstGTLine = false;
		}
		else GText->hpos = GText->lind;
	}

	*ngr = *Graphic; // save gtype, re-init others below
	*ngt = *GText;	 // save gform, hpos, vpos, fwide, fhigh, thigh

	if (GTAlignChange == 1)
		GTAlignChange = 2;  // must correct hpos later

	if (GTAlignChange) {
		if (!GTFixList.find((long)GText))
			GTFixList.add((long)GText);
		if (!GTFixList.find((long)ngt))
			GTFixList.add((long)ngt);
	}

	// point at origin for text string
	if (InGrTextFrame)
		EndGTextLine();

	EndGraphic(NULL); // insert old into grlist

	InGraphic = true;
	Graphic = ngr;							 // set globals to new objects
	GText = ngt;
	Graphic->gparm = (short *) GText;	// re-init Graphic
	Graphic->gsize = 6;
	Graphic->gpen = NULL;
	Graphic->gbrush = NULL;
	Graphic->gfont = NULL;

	if (NewTextGrLine == true) { 	// point at top of next line
		GText->vpos += GText->thigh;
		GText->hpos = NewTextGrPara ? GText->find : GText->lind;
	}
	else { // continuation of line, either tab or property change
		if (GTAlignChange == 2) {
			GTWidth += GText->twide;
			GText->hinc += GText->twide;
			if (GTTabPos > 0) {
				if (GTTabPos > (GText->hpos + GText->hinc))
					GText->hinc = GTTabPos - GText->hpos;
				else
					GText->hinc += (CurrFont.fwid * CurrFont.fhigh) / 240;
				GTTabPos = 0;
			}
		}
		else {
			GText->hpos += GText->twide;
			if (GTTabPos > 0) {
				if (GTTabPos > GText->hpos)
					GText->hpos = GTTabPos;
				else
					GText->hpos += (CurrFont.fwid * CurrFont.fhigh) / 240;
				GTTabPos = 0;
			}
		}
	}

	// correct vpos for base shift here ****
	if (NewTextGrBase) {
		if (CurrFont.fbase < 2) {  // return to normal
			if (CurrFont.bhigh) {
				CurrFont.fhigh = CurrFont.bhigh;
				CurrFont.bhigh = 0;
			}
			if (CurrFont.bshift) {
				GText->vpos -= CurrFont.bshift;
				CurrFont.bshift = 0;
			}
		}
		else {  // super or sub
			CurrFont.bhigh = CurrFont.fhigh;
			CurrFont.fhigh = (CurrFont.bhigh * 7) / 10;
			if (CurrFont.fbase == 2)        // sub
				CurrFont.bshift = (CurrFont.bhigh * 1) / 10;
			else if (CurrFont.fbase == 3)  // super
				CurrFont.bshift = - (CurrFont.bhigh * 3) / 10;
			GText->vpos += CurrFont.bshift;
		}
		NewTextGrBase = false;
	}

	GText->twide = 0;            // empty string info from GText
	GText->tcount = 0;
	GText->tptr = NULL;

	NewTextGrNeeded = false;
	NewTextGrLine = false;
	NewTextGrPara = false;
}


void
RTFwr::ProcGrFormat(RTFctl *cp)
{
	// handle font properties and alignment	per format
	pprop* gform;
	bool changed = false;
	uns tsh = 0;
	unc tch = 0;

  uns formnum = (uns) cp->siz();

	if (formnum == 0) {	
		if (cp->dat() == 1) // for "Init Store" after para overrides
			ParaFont = CurrFont;
		else if (cp->dat() == 2) { // for "Inherit Restore"
			if ((CurrFont.fnum != ParaFont.fnum)
			 || (CurrFont.fhigh != ParaFont.fhigh)
			 || (CurrFont.fbase != ParaFont.fbase)
			 || (CurrFont.fweight != ParaFont.fweight)
			 || (CurrFont.fital != ParaFont.fital)
			 || ((CurrFont.fundl != ParaFont.fundl) && !NoGTULine)
			 || (CurrFont.fstrik != ParaFont.fstrik)
			 || (CurrFont.fcolr != ParaFont.fcolr)) {
				if ((GText->tcount > 0)) {
					NewTextGrNeeded = true;
					if (CurrFont.fbase != ParaFont.fbase) {
						tsh = CurrFont.bshift;
						NewTextGrBase = true;
					}
				}
			}
			CurrFont = ParaFont;
			CurrFont.bshift = tsh;
		}
		return;
	}

	gform = (pprop *) FormList.find(FormRefList(formnum));
	if (gform == NULL)
		return;

	finfo *inf = NULL;

  if (cp->dat() == 1) { // init format
		CurrFont.fnum = gform->fnum;
		strncpy(CurrFont.fname, (char *) FontNameList.find(gform->fnum), WmfFNameMax);
		if ((inf = (finfo *) FontList.find(CurrFont.fnum)) != NULL) {
			CurrFont.fntype = (unc) inf->ftype;
			CurrFont.fenc = inf->fenc;
			CurrFont.cpg = inf->cpg;
		}
		else {
			if (IniFile
			 && IniFile->Section("FontEncoding")
			 && IniFile->Find(CurrFont.fname)) {
				CurrFont.fenc = (uns) IniFile->IntRight();
				CurrFont.cpg = GetCodePageFromEncoding(CurrFont.fenc);
			}
		}
    if ((CurrFont.fwid = (uns) FontWidthList.find(CurrFont.fnum)) == 0)
			CurrFont.fwid = DefFontWidth;
		CurrFont.fcolr = gform->colr;  
		CurrFont.fhigh = gform->fsize * 10;  // use twips here  
		CurrFont.fbase = gform->base;  
		CurrFont.fweight = (gform->bold == 2) ? 700 : 400;  
		CurrFont.fital = gform->ital - 1;
		if (!NoGTULine)  
			CurrFont.fundl = (gform->line > 1) ? 1 : 0;  
		CurrFont.fstrik = gform->strike - 1;  

		if ((GText->gform = gform->algn) > 2)
			GText->gform = 0;
		
		GText->find = gform->find; // use first-line indent for flush left
		GText->lind = gform->lind;
		GText->sideh = gform->sideh;
		if (GTShLeft             // left sideheads set
		 && (gform->sideh == 0)) { // in-col is affected
			GText->find += GTShLeft;
			GText->lind += GTShLeft;
		}
		else if (GTShRight       // right sideheads set
		 && (gform->sideh > 1)) {  // sideheads are affected
			GText->find += GTShRight;
			GText->lind += GTShRight;
		}

		GText->lsmode = (uns) gform->lsmode;
		GText->lspace = gform->lspace;
		GText->bspace = gform->bspace;
		GTPSpace = GText->bspace;
		SetGTextLineHeight();

		ParaFont = CurrFont;
	}
  else if (cp->dat() == 2) { // inherit format
		if (gform->fnum) {
			tsh = CurrFont.fnum;
			CurrFont.fnum = gform->fnum;
			strncpy(CurrFont.fname, (char *) FontNameList.find(gform->fnum), WmfFNameMax);
			if ((inf = (finfo *) FontList.find(CurrFont.fnum)) != NULL) {
				CurrFont.fntype = (unc) inf->ftype;
				CurrFont.fenc = inf->fenc;
				CurrFont.cpg = inf->cpg;
			}
			else {
				if (IniFile
				 && IniFile->Section("FontEncoding")
				 && IniFile->Find(CurrFont.fname)) {
					CurrFont.fenc = (uns) IniFile->IntRight();
					CurrFont.cpg = GetCodePageFromEncoding(CurrFont.fenc);
				}
			}
      if ((CurrFont.fwid = (uns) FontWidthList.find(CurrFont.fnum)) == 0)
				CurrFont.fwid = DefFontWidth;
			if (tsh != CurrFont.fnum)
				changed = true;
		}
		if (gform->fsize) {
			tsh = CurrFont.fhigh;
			CurrFont.fhigh = gform->fsize * 10;  // use twips here  
			SetGTextLineHeight();
			if (tsh != CurrFont.fhigh)
				changed = true;
		}
		if (gform->base) {
			tch = CurrFont.fbase;
			CurrFont.fbase = gform->base;
			if (tch != CurrFont.fbase) {
				NewTextGrBase = true;
				changed = true;
			}
		}
		if (gform->bold) {
			tsh = CurrFont.fweight;
			CurrFont.fweight = (gform->bold == 2) ? 700 : 400;
			if (tsh != CurrFont.fweight)
				changed = true;
		}
		if (gform->ital) {
			tch = CurrFont.fital;
			CurrFont.fital = gform->ital - 1;
			if (tch != CurrFont.fital)
				changed = true;
		}
		if (gform->line
		 && !NoGTULine) {
			tch = CurrFont.fundl;
			CurrFont.fundl = (gform->line > 1) ? 1 : 0;
			if (tch != CurrFont.fundl)
				changed = true;
		}
		if (gform->strike) {
			tch = CurrFont.fstrik;
			CurrFont.fstrik = gform->strike - 1;
			if (tch != CurrFont.fstrik)
				changed = true;
		}
	  if (gform->colr) {
			tsh = CurrFont.fcolr;
		  CurrFont.fcolr = gform->colr;
			if (tsh != CurrFont.fcolr)
				changed = true;
		}
		if (changed
		 && (GText->tcount > 0))
			NewTextGrNeeded = true;
	}
}


uns RTFwr::GrFWeight[GrFWeightMax] = {
  400, 100, 300, 600, 700, 800, 900
};


void
RTFwr::ProcGrTProp(RTFctl *cp)
{
  char *fntname = NULL;
	bool delfp = false;
	uns num;
	uns colr;
	short high;
	uns weight;
	unc base;
	unc ital;
	unc undl;
	unc strik;
	finfo *inf = NULL;

  switch ((cp->rp)->write_op) {  // processing during scan phase
    case putfont:
      fntname = GetFontName(cp);
      if ((fntname == NULL) || (*fntname == '\0'))
        break;
			if (IniFile
			 && IniFile->Section("Fonts")
			 && IniFile->Find(fntname)) {  // mapped
				fntname = IniFile->StrRight();
				delfp = true;
			}
      strncpy(CurrFont.fname, fntname, WmfFNameMax);
			num = CurrFont.fnum;
      CurrFont.fnum = (uns) FontNameList(fntname);
			if ((inf = (finfo *) FontList.find(CurrFont.fnum)) != NULL) {
				CurrFont.fntype = (unc) inf->ftype;
				CurrFont.fenc = inf->fenc;
				CurrFont.cpg = inf->cpg;
			}
			else {
				if (IniFile
				 && IniFile->Section("FontEncoding")
				 && IniFile->Find(fntname)) {
					CurrFont.fenc = (uns) IniFile->IntRight();
					CurrFont.cpg = GetCodePageFromEncoding(CurrFont.fenc);
				}
			}
      if ((CurrFont.fwid = (uns) FontWidthList.find(CurrFont.fnum)) == 0)
				CurrFont.fwid = DefFontWidth;
			if (delfp)
				DeleteName(fntname);
			if ((CurrFont.fnum != num)
			 && (GText->tcount > 0))
				NewTextGrNeeded = true;
      break;
    case putcomp:  // compressed or expanded, modify name ****
      break;
    case putfsize:
			high = CurrFont.fhigh;
      CurrFont.fhigh = GetFontSize(cp) * 10;
			SetGTextLineHeight();
			if ((CurrFont.fhigh != high)
			 && (GText->tcount > 0))
				NewTextGrNeeded = true;
      break;
    case putbase:
			base = CurrFont.fbase;
      CurrFont.fbase = (unc) cp->siz() + 1;
			if ((CurrFont.fbase != base)
			 && (GText->tcount > 0)) {
				NewTextGrNeeded = true;
				NewTextGrBase = true;
			}
      break;
    case putbold:
			weight = CurrFont.fweight;
      if (cp->siz() < GrFWeightMax)
        CurrFont.fweight = GrFWeight[cp->siz()];
			if ((CurrFont.fweight != weight)
			 && (GText->tcount > 0))
				NewTextGrNeeded = true;
      break;
    case putital:
			ital = CurrFont.fital;
      CurrFont.fital = cp->siz() ? 1 : 0;
			if ((CurrFont.fital != ital)
			 && (GText->tcount > 0))
				NewTextGrNeeded = true;
      break;
    case putrevb:
			strik = CurrFont.fstrik;
      CurrFont.fstrik = (cp->siz() == 2) ? 1 : 0;
			if ((CurrFont.fstrik != strik)
			 && (GText->tcount > 0))
				NewTextGrNeeded = true;
      break;
    case putline:
			if (NoGTULine)
				break;
			undl = CurrFont.fundl;
      CurrFont.fundl = cp->siz() ? 1 : 0; // 1 single, 2 double, 3 over
			if ((CurrFont.fundl != undl)
			 && (GText->tcount > 0))
				NewTextGrNeeded = true;
      break;
    case putcolr:
			colr = CurrFont.fcolr;
      GetColor(cp->siz(), &CurrFont.fcolr);
			if ((CurrFont.fcolr != colr)
			 && (GText->tcount > 0))
				NewTextGrNeeded = true;
      break;

    case putdown:
			GText->vpos += (short) ((cp->siz() * CurrFont.fhigh) / 10000L);
      break;
    case putup:
			GText->vpos -= (short) ((cp->siz() * CurrFont.fhigh) / 10000L);
      break;
    case putright:
			GText->hpos += (short) ((cp->siz() * CurrFont.fhigh) / 10000L);
      break;
    case putleft:
			GText->hpos -= (short) ((cp->siz() * CurrFont.fhigh) / 10000L);
      break;

		case puttdef:
			NoGrTab = true;
			break;
		case endtdef:
			NoGrTab = false;
			break;

		case idxgrp:
		case fmpxset:
		case fmsxset:
			NoGrText = true;
			break;
		case idxend:
		case fmpxres:
		case fmsxres:
			NoGrText = false;
			break;

		case hypunq:
			ProcHyperUnique(cp);
			break;

    default:
      break;
  }
}


void
RTFwr::SetGTextLineHeight()
{
	short val;
	short high = CurrFont.bhigh ? CurrFont.bhigh : CurrFont.fhigh;

	switch (GText->lsmode) {
		case 1:  // fixed
		case 2:  // fixed, allow
		default:
			val = GText->lspace;
			break;
		case 3:  // add
		case 4:  // add, allow
			val = GText->lspace + high;
			break;
		case 5:  // prop
			val = (short) (((long) GText->lspace * high) / 1000L);
			break;
	}
	GText->thigh = CurrFont.lhigh = val;
}


void
RTFwr::ProcGrTStr(RTFctl *cp)
{
	char *sptr;
	char ch;
	unc mult = 1;
	bool asia = false;
	unl uch = 0;

  size_t i = cp->siz();
  if (i == 0)
    return;

	if ((HideWhite == true)
	 && (CurrFont.fcolr == 9))
		return;

	GTDiscHyphen = false;
	if (cp->dt() == etext) { 
		sptr = (char *) cp->ex();
		if (sptr == NULL)
			return;
	}
	else if (cp->dt() == imnem) {
		if ((CurrFont.fenc >= 128)
		 && (CurrFont.fenc <= 136))
			asia = true;
#if 0
		else if ((i > 0x10000) // sign extended char
		 || (((i >> 8) & 0xFF) == 0xFF))
			i &= 0xFF;
#endif

		if ((i < 0x7F)
		 || ((CurrFont.fntype > 4)
		  && (i < 0xFF)
			&& !asia))
			ch = (char) i;
		else switch (i) {
      case 0xA0:
				if (asia)
					goto normal;
      case 0x2002:
      case 0x2007:
      case 0x2009:
        ch = ' ';
        break;
      case 0xAD:
				if (asia)
					goto normal;
				GTDiscHyphen = true;
        return;
      case 0x2010:
      case 0x2011:
      case 0x2012:
      case 0x2013:
        ch = '-';
        break;
      case 0x2014:
      case 0x2015:
				mult = 2;
        ch = '-';
        break;
      case 0x2044:
        ch = '/';
        break;

			case 145:
			case 146:
				if (asia)
					goto normal;
				ch = '\'';
				break;
			case 147:
			case 148:
				if (asia)
					goto normal;
				ch = '"';
				break;
      case 0x2001:
      case 0x2003: // em space
				mult = 2;
				ch = ' ';
				break;
			default:
			normal:
				ch = (char) i;
				break;
		}
		if (asia) {
			mult = 2;
			uch = i;
		}
		i = 1;
		sptr = &ch;
	}
	else return;

	// really something to write, so do it
	if (NewTextGrNeeded)
		GenNewTextGraphic();
	
  if (GText->tptr == NULL)
		GText->tptr = (char *) NewMem(GText->acount = ((i * mult) + 32));
  else if (GText->acount < (GText->tcount + (i * mult)))
    GText->tptr = (char *) ResizeMem(GText->tptr,
		                        GText->acount = (GText->tcount + (i * mult) + 32));

  char *dptr = GText->tptr + GText->tcount;

  // write chars into string, update twide
  uns chwid = (uns)(((long)GrTxScale * CurrFont.fwid * CurrFont.fhigh) / 24000L);
  bool chfix = (asia || (CurrFont.fntype > 2)) ? true : false;
  uns strsize = 0;
	if (asia && uch) {
		if (MIFVer < 8) {  // already using Windows code page
			if (uch <= 0xFF) {  // ignore other syms for now
				*dptr++ = (unc) (uch & 0xFF);
				strsize++;
			}
		}
		else if (uch > 0x7F) {  // Unicode, map to code page
			unl nch = 0;
			if ((nch = DCicu::GetUnicodeCpg(uch, CurrFont.cpg)) != 0) {
				*dptr++ = (unc) (nch & 0xFF);
				strsize++;
				if (nch > 0xFF) {
					*dptr++ = (unc) ((nch >> 8) & 0xFF);
					strsize++;
				}
			}
		}
		else {
			*dptr++ = (unc) (uch & 0xFF);
			strsize++;
		}
		GText->twide += chwid;
	}
	else {
		for (; i && *sptr; i--, sptr++) {
			for (unc j = 0; j < mult; j++) {
				*dptr++ = *sptr;
				strsize++;
			}
			unc uch = *((unc *) sptr);
			if ((chfix == true) || (uch > CharMetricMax))
				GText->twide += (chwid * mult);
			else GText->twide += ((chwid * mult) * CharMetrics[uch]) / 56;
		}
	}
  *dptr = '\0';

	if (Graphic->gfont == NULL) {
		*(Graphic->gfont = new wmf_font) = CurrFont;
		GText->thigh = CurrFont.lhigh;
	}

  // update counters

  GText->tcount += strsize;
	if (GText->acount < (GText->tcount + 1))
		GText->tptr = (char *) ResizeMem(GText->tptr,
		                         GText->acount = (GText->tcount + 33));
  Graphic->gsize = 6 + ((GText->tcount + 1) / 2);
	assert(GText->tcount < 256);
}


void
RTFwr::ProcGrTx(RTFctl *cp)
{
	if (cp->rp == NULL)
		return;

  if (Verbose)
    printf("graphic text ");

  switch ((cp->rp)->write_op) {  // rescan graphic text frame content
		case puttx:
			if (NoGrText == false)
				ProcGrTStr(cp);
			break;
		case puttab:
			if (NoGrTab == false) {
				PutGTSpace();
				if (GText->gform == 0) {  // left only
					GTTabPos = GetTwips(cp->siz(), pg_unit);
					if (GTShLeft            // left sideheads set
					 && (GText->sideh == 0))  // in-col is affected
						GTTabPos += GTShLeft;
					NewTextGrLine = false;
					GenNewTextGraphic();
				}
			}
			break;
		case linend:	  // line break, move down
			if (cp->siz() == 1)			 // hard break always followed by soft break
				break;
			if ((GText->tcount == 0)	 // after empty line
			 && (FirstGTPara == true)	 // at the very beginning
			 && (FirstGTLine == true)
			 && (NoBlankFirstGTLine == true))
				break;									 // skip dummy para
			if (GTDiscHyphen == true)
				PutGTHyphen();
			NewTextGrLine = true;
			GenNewTextGraphic();
			FirstGTLine = false;
			break;
		case parend:
			GText->hpos = 0;
			FirstGTPara = false;
			FirstGTLine = true;
			NewTextGrPara = true;
			//GTPSpace = GText->bspace;
			break;
		case formcall:
			if (!InIndexGroup)
				ProcGrFormat(cp);
			break;
		case putalgn:	// override
		  if ((GText->gform = (cp->dat() - 1)) > 2)
				GText->gform = 0;	// make justified into left
			break;
    case putlspc:
      switch (GText->lsmode = (uns) cp->dat()) {
        case 1:  // fixed
        case 2:  // fixed, allow
        case 3:  // add
        case 4:  // add, allow
          GText->lspace = GetTwips(cp->siz(), ld_unit);
          break;
        case 5:  // prop
          GText->lspace = (short) cp->siz();
          break;
        default:
          break;
      }
			SetGTextLineHeight();
      break;
    case putpspc:
      switch (cp->dat()) {
        case 1: // fixed before
        case 2: // min before
          if ((GText->bspace = GetTwips(cp->siz(), ld_unit)) < 0)
            GText->bspace = 0;
					GTPSpace = GText->bspace;
          break;
        default:
          break;
      }
      break;
    case putindt:
      switch (cp->dat()) {
        case 1: // first left
          GText->find = GetTwips(cp->siz(), pg_unit);
					if (GTShLeft            // left sideheads set
					 && (GText->sideh == 0))  // in-col is affected
						GText->find += GTShLeft;
					else if (GTShRight      // right sideheads set
					 && (GText->sideh > 1))   // sideheads are affected
						GText->find += GTShRight;
          break;
        case 2: // body left
          GText->lind = GetTwips(cp->siz(), pg_unit);
					if (GTShLeft
					 && (GText->sideh == 0))
						GText->lind += GTShLeft;
					else if (GTShRight
					 && (GText->sideh > 1))
						GText->lind += GTShRight;
          break;
        default:
          break;
      }
      break;

		case parstart:
			if (cp->siz() != 0)  // see if autonum present
				GTAnum = (RTFgroup *) AnumValList.find(cp->siz());
			break;
		case putanum:
			if (GTAnum && (GTAnumProc == false)) {
				GTAnumProc = true;
				GTAnum->execu(ProcGrTx);
				GTAnumProc = false;
				GTAnum = NULL;
			}
			break;
		case putvar:   // variable reference
			if (!GTVar) {
				GTVar = (RTFgroup*) VarList->find(cp->siz());
				GTVar->execu(ProcGrTx);  // does not nest
				GTVar = NULL;
			}
			break;
		case putvtok:  // variable token, date, etc., ignored ****
			break;
		case setxref:  // xref target
			if (XrfType == 'H') {
				WMFRefList.add(GetXRStr(cp), 1L);
				WMFRefUsed = true;
			}
			break;
		case hyploc:  // newlink target
			if (UseHL == false)
				break;
		case hypalt:
			if (XrfType == 'H') {
				WMFRefList.add((char *)(cp->ex()), 2L);
				WMFRefUsed = true;
			}
			break;

		case putfr:  // anchored frame in text frame in anchored frame...
			PutGTFrame(cp); // make it a graphic frame within the current frame
			break;

		default:
			ProcGrTProp(cp);
			break;
	}
}


void
RTFwr::PutGTFrame(RTFctl *cp)
{
	if ((cp->rp)->write_op != putfr)
		return;

	frame *fr = (frame *) FrameList.find(cp->dat());
	if (fr == NULL)
		return;

	// make wrapper for frame
	graph *gr = new graph;
	gr->gtype = 0;
	gr->gsize = 1; // to prevent removal as empty
  gr->gparm	= (short *) fr;

	// add wrapper to text frame
	frame *tfr = FrameStack[FrameLevel - 1]; // containing frame
	tfr->grcount++;
  if (!tfr->grlist)
    tfr->grlist = new DCvlist;
  (tfr->grlist)->add(gr, tfr->grcount);

	// adjust frame position
	fr->xpos = fr->pxpos - tfr->xpos;
	fr->ypos = fr->pypos - tfr->ypos;

	if (fr->fpos == 0) { // in-line
		fr->ypos += GrVert * 10;
		NewTextGrLine = false;
		GTTabPos = fr->xpos + fr->wide;
		GenNewTextGraphic();
	}
	else if (fr->fwrap) { // run-in
		GText->hpos += (fr->xpos + fr->wide);
		GText->vpos += fr->high;
	}
	else if (fr->fpos == 2)
		GTFSpace += fr->high;
}


void
RTFwr::PutGTSpace()
{
	RTFctl *tcp = new RTFctl(imnem, 2, 10, 1, 32);
	ProcGrTStr(tcp);
	delete tcp;
}


void
RTFwr::PutGTHyphen()
{
	RTFctl *tcp = new RTFctl(imnem, 2, 10, 1, '-');
	ProcGrTStr(tcp);
	delete tcp;
}



// handle xrefs in frames that are replaced by previews

void
RTFwr::ProcFrameRefs(frame *fr)
{
  frame *nf;
  graph *gr;

	for (uns i = 1; i <= fr->grcount; i++) {
		if ((gr = (graph *) ((fr->grlist)->find(i))) != NULL) {
			if (gr->gtype == 0)	{ // recurse for embedded frames
				nf = (frame *) gr->gparm;
				if (nf->ftext) {
					RTFgroup *gp = (RTFgroup *) FlowList.find(nf->ftext);
					if (gp)
						gp->execu(ProcGrTxRef);
					nf->ftext = 0;
        }
				ProcFrameRefs(nf);
			}
		}
	}
}


void
RTFwr::ProcGrTxRef(RTFctl *cp)
{
	if (cp->rp == NULL)
		return;

  switch ((cp->rp)->write_op) {  // rescan graphic text frame content
		case setxref:  // xref target
			if (XrfType == 'H') {
				WMFRefList.add(GetXRStr(cp), 1L);
				WMFRefUsed = true;
			}
			break;

		case hyploc:  // newlink target
			if (UseHL == false)
				break;
		case hypalt:
			if (XrfType == 'H') {
				WMFRefList.add((char *)(cp->ex()), 2L);
				WMFRefUsed = true;
			}
			break;

		default:
			break;
	}
}



// metafile text for WinHelp superscripts and subscripts


bool RTFwr::InTextMetafile = false;
frame *RTFwr::TMFrame = NULL;


void
RTFwr::StartTextMetafile()
{
	if ((OutType != 'H')
	 || (MetaType != 'E')
	 || InGraphic)
		return;

	short baseline = Curr.fsize * 10;
	short scale = Curr.fsize * 7;  // 70% height for subs and supers

	InTextMetafile = true;

	TMFrame = new frame;
	InitFrame(TMFrame); // sets whead
	TMFrame->high = baseline;

	// create first graphic text element
	Graphic = new graph;
	Graphic->gtype = 0x521;
	Graphic->gsize = 6;
	Graphic->gparm = (short *) (GText = new gtext);

	// set up font and vert positioning
	CurrFont = LastFont = DefaultFont;
	CurrFont.fnum = Curr.fnum;
	finfo *inf = NULL;
	if ((inf = (finfo *) FontList.find(CurrFont.fnum)) != NULL)
		CurrFont.fntype = (unc) inf->ftype;
	CurrFont.fwid = Curr.fwid;
	strncpy(CurrFont.fname, (char *) FontNameList.find(CurrFont.fnum), WmfFNameMax);
	CurrFont.fweight = (Curr.bold == 2) ? 700 : 400;
	CurrFont.fital = (Curr.ital == 2) ? 1 : 0;
	CurrFont.fundl = HelpForm ? 0 : ((Curr.line > 1) ? 1 : 0);
	CurrFont.fstrik = (Curr.strike > 1) ? 1 : 0;

	if (Curr.base == 2) {       // subscript
		CurrFont.fhigh = scale;   // scaled to 70%
		GText->aypos = baseline;  // base on baseline
	}
	else if (Curr.base == 3) {  // superscript
		CurrFont.fhigh = scale;   // 70% again
		GText->aypos = scale;     // top at the top
	}
	else {                      // normal, just in case...
		CurrFont.fhigh = baseline; 
		GText->aypos = baseline;
	}
}


void
RTFwr::ProcTextMetafile(RTFctl *cp)
{
	if (TMFrame == NULL)
		return;
  // start new gtext if font changes ****

	// add to the graphic text
	NewTextGrNeeded = false;
	ProcGrTStr(cp);
}


void
RTFwr::EndTextMetafile(RTFwfile *wf)
{
	if (TMFrame == NULL)
		return;
	// complete last graphic text element, update whead
	if (GText->tcount > 0) {
		TMFrame->wide += GText->twide;
		TMFrame->grcount++;
	  if (!TMFrame->grlist)
		  TMFrame->grlist = new DCvlist;
		TMFrame->grlist->add(Graphic, TMFrame->grcount);
		//Graphic->gfont = new wmf_font;
		//*(Graphic->gfont) = CurrFont;
	}

	if (!InHelpTitlePara)
		TopicEmpty = false;

	// write entire metafile
	if (TMFrame->grcount) {
		SetWMFStart(wf, TMFrame); // start diversion
		if (TMFrame->grlist) {
			for (int i = 1; i <= TMFrame->grcount; i++) {
				if ((Graphic = (graph *) ((TMFrame->grlist)->find(i))) != NULL) {
					WriteGraphic(wf, Graphic);
					delete Graphic->gparm;
					delete Graphic;
				}
			}
		}
    WriteWMFEnd(wf);	// end diversion
		WriteWMFStart(wf, TMFrame, 1);  // write diversion
		// write name into rtf file
		SetWMFEnd(wf, TMFrame, BmcStr);
	}
	else {
		delete GText;
		delete Graphic;
	}
	GText = NULL;
	Graphic = NULL;

	delete TMFrame;
	TMFrame = NULL;
	InTextMetafile = false;
}



// graphic writing functions


void
RTFwr::WriteGrTextAlign(RTFwfile *wf, uns align)
{
  wf->putMEnd();
  wf->putML(4);      // record length
	UpdateWMFHead(4);
  wf->putMS(0x12E);  // record function
	wf->putMS((align == 2) ? 0x1A :
						(align == 1) ? 0x1E : 0x18);
}


void
RTFwr::WriteGrTextColor(RTFwfile *wf, uns color)
{
  wf->putMEnd();
  wf->putML(5);      // record length
	UpdateWMFHead(5);
  wf->putMS(0x209);  // record function
  wf->putML(((((long) BlueTable[color]) << 16) & 0xFF0000)
         + ((((long) GreenTable[color]) << 8) & 0xFF00)
         + (RedTable[color] & 0xFF));
}


void
RTFwr::WriteGrText(RTFwfile *wf, gtext *gt)
{
  wf->putMS(gt->tcount);
  for (uns i = 0; i < gt->tcount; i++)
    wf->putMB(gt->tptr[i]);
  if (i & 1)
    wf->putMB(0);
  wf->putMChar(' ');
	// default vert text alignment is top
	// but align ctl sets it to baseline
  wf->putMS(gt->aypos + WMFyoff);
  // default hor text alignment is left
  // but align ctl sets to ctr or right if needed
  wf->putMS(gt->axpos + WMFxoff);
}



wmf_font RTFwr::DefaultFont = { "Arial", 0L, 200, 0, 1, 400, 0, 0, 0, 0 } ;
wmf_font RTFwr::CurrFont;
wmf_font RTFwr::LastFont;


void
RTFwr::WriteWMFFont(RTFwfile *wf, wmf_font *fnt)
{
	short fontht = -(fnt->fhigh);

  wf->putMEnd();
  wf->putML(28);      // record length
	UpdateWMFHead(28);
  wf->putMS(0x2FB);  // record function: LOGFONT structure

	if (GrTxScale != 100)    // use GrTxScale for size
		wf->putMS((short) ((((long) fontht * GrTxScale) + 50L) / 100L));
  else wf->putMS(fontht);

	if ((GrTxWidth != 0)    // use GrTxWidth for width
	 && (GrTxWidth != 100))
		wf->putMS((short) ((((long) fontht * GrTxWidth) + 50L) / 100L));
  else wf->putMS(0);

  if (fnt->fname[0] == '\0')
    strncpy(fnt->fname, (char *) FontNameList.find(1), WmfFNameMax);

#if 0
	long num = fnt->fnum;
	finfo *inf = NULL;
	unc cset = 0;
	if (num == 0)
		num = FontNameList(fnt->fname);
	if (num)
		inf = (finfo *) FontList.find(num);
	if (inf != NULL)
		cset = (inf->ftype == 5) ? 2 : 0;
#endif

  wf->putMS(fnt->fangle);	// repeated is necessary
  wf->putMS(fnt->fangle);
  wf->putMS(fnt->fweight);
  wf->putMB(fnt->fital);
  wf->putMB(fnt->fundl);
  wf->putMB(fnt->fstrik);
  wf->putMB((fnt->fntype == 5) ? 2 : (unc) fnt->fenc);
  wf->putMB(0);
  wf->putMB(0);
  wf->putMB(1);
  wf->putMB((fnt->fntype < 7) ? ((fnt->fntype << 4) & 0xF0) : 0);
  wf->putMEnd();

  wf->putMChar(' ');
  for (uns i = 0; i < WmfFNameMax; i++)
    wf->putMB(fnt->fname[i]);

  WriteWMFSel(wf, 2); // select as object 2
  CurrFont = *fnt;
}



// end of dwrtfgt.cpp


