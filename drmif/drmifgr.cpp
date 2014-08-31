 
// drmifgr.cc is the code for dcl FrameMaker graphics reading, JHG
// Copyright (c) 1993-1997 by Omni Systems, Inc.  All rights reserved.

// allow PICT for export as .pct files only
#define PICTOK

#include "dcappl.h"
#include "dcctlg.h"
#include "dclist.h"
#include "dcfili.h"
#include "dcini.h"
#include "drmif.h"
#include "drmiffl.h"


mif MIFrd::mifgrpile[] = {
  { "AFrames",           newgroup,   group, 0, 11,  0, pseudogroup },

  { "Group",             newgroup,   group, 4,  1,  1, ggroup },
  { "GroupID",           numval,     ilong, 0,  0,  0, mif_gid },

  { "Frame",             newgroup,   group, 3, 20,  0, gframe },
  { "FrameType",         l_frtype,   imnem, 3, 21,  7, frtype },
//  { "Tag",               nameval,    ename, 3, 21,  1, frname },
  { "Float",             yorn,       imnem, 3, 21,  9, frfloat },
  { "NSOffset",          sizeval,    ilong, 0,  0,  0, frnsoff },
  { "BLOffset",          sizeval,    ilong, 0,  0,  0, frbloff },
  { "RunaroundGap",      sizeval,    ilong, 0,  0,  0, frrungap },
  { "AnchorAlign",       l_gralign,  imnem, 3, 21,  6, fralign },
  { "Cropped",           yorn,       imnem, 3, 21, 11, frcrop },

  { "ObjectAttribute",   newgroup,   group, 3, 35,  1, objprop },
  { "Tag",               nameval,    ename, 3, 36,  1, propname },
  { "Value",             nameval,    ename, 3, 36,  2, propval },

  { "AFrame",            numval,     imnem, 2,  5,  0, aframe },

  { "PolyLine",          newgroup,   elong, 4, 30,  1, gobj },
  { "Polygon",           newgroup,   elong, 4, 30,  2, gobj },
  { "Smoothed",          yorn,       imnem, 0,  0,  0, smooth },
  { "NumPoints",         numval,     elong, 4, 30,  0, gpoints },
  { "Point",             ptval,      elong, 0,  0,  0, grpoint },

  { "Rectangle",         newgroup,   elong, 4, 30, 10, gobj },
  { "RoundRect",         newgroup,   elong, 4, 30, 11, gobj },
  { "Radius",            sizeval,    ilong, 0,  0,  0, radius },

  { "Ellipse",           newgroup,   elong, 4, 30, 20, gobj },

  { "Arc",               newgroup,   elong, 4, 30, 30, gobj },
  { "ArcRect",           arectval,   ilong, 0,  0,  0, mif_rect },
  { "ArcTheta",          numval,     ilong, 0,  0,  0, arctheta },
  { "ArcDTheta",         numval,     ilong, 0,  0,  0, arcdtheta },

  { "ArrowStyle",        newgroup,   ilong, 4, 24,  0, arstyle },
  { "TipAngle",          numval,     ilong, 0,  0,  0, artang },
  { "BaseAngle",         numval,     ilong, 0,  0,  0, arbang },
  { "Length",            glineval,   ilong, 0,  0,  0, arlen },
  { "HeadType",          l_grarrow,  imnem, 0,  0,  0, arhead },
  { "ScaleHead",         yorn,       imnem, 0,  0,  0, arscale },
  { "ScaleFactor",       glineval,   ilong, 0,  0,  0, arfactor },

  { "TextLine",          newgroup,   group, 4, 10,  2, gtext },
  { "TLOrigin",          ptval,      elong, 4, 40,  0, tlpoint },
  { "TLAlignment",       l_gralign,  elong, 4, 40,  0, tlalign },
  { "Angle",             numval,     ilong, 0,  0,  0, tlrot },

  { "Math",              newgroup,   group, 4, 10,  3, mathobj },
  { "MathOrigin",        ptval,      elong, 4, 50,  4, mathorg },
  { "MathAlignment",     l_palign,   elong, 4, 51,  2, mathalgn },
  { "MathSize",          l_mathsiz,  imnem, 4, 51,  3, mathsiz },
  { "MathLineBreak",     sizeval,    ilong, 4, 51,  4, mathbk },
  { "MathFullForm",      mathval,    ename, 4, 52,  1, mathff },

  { "ImportObject",      newgroup,   group, 4, 10,  4, impobj },
  { "ImportObFile",      fileval,    ename, 0,  0,  0, impname },
  { "BitMapDpi",         numval,     ilong, 0,  0,  0, gres },
  { "FlipLR",            yorn,       imnem, 0,  0,  0, gflip },

  { "Pen",               fillval,    imnem, 4, 21,  1, gpen },
  { "PenWidth",          glineval,   ilong, 4, 20,  3, gpwid },
  { "Fill",              fillval,    imnem, 4, 22,  1, gfill },
  { "Separation",        separ,      imnem, 4, 20,  2, gsepar },
  { "ObColor",           nameval,    imnem, 4, 20,  2, gcolor },

  { "DashedPattern",     newgroup,   imnem, 4, 20,  1, lsegpat },
  { "NumSegments",       numval,     ilong, 0,  0,  0, lsegcount },
  { "DashSegment",       glineval,   ilong, 0,  0,  0, lineseg },

  { "HeadCap",           l_grcap,    imnem, 4, 24,  1, headcap },
  { "TailCap",           l_grcap,    imnem, 4, 24,  2, tailcap }

};

size_t MIFrd::mifgr_size = (sizeof(mifgrpile) / sizeof(mif));

char *MIFrd::wordgrpile[] = {
  (char *) (int) l_frtype, "NotAnchored", "Inline", "Top", "Below",
	  "Bottom", "Left", "Right", "Near", "Far", "RunIntoParagraph",
	  "Inside", "Outside", NULL,
  (char *) (int) l_gralign, "Left", "Center", "Right",
	  "Inside", "Outside", NULL,
  (char *) (int) l_grcap, "Square", "Butt", "Round", "ArrowHead", NULL,
  (char *) (int) l_grarrow, "Filled", "Hollow", "Stick", NULL,
  (char *) (int) l_mathsiz, "MathSmall", "MathMedium", "MathLarge", NULL,
  NULL
};


// start of code


void
MIFrd::ProcGr(mif *mp)
{
  switch (mp->spec) {

    case mif_gid:
      CurrGID = (uns) MIFval;
      break;
    case ggroup:
      GraphicGroup(mp);
      break;

    case gframe:
      StartGraphic();
      ProcGFrame(mp);
      break;
    case frtype:
      if (MIFval < 0)
        break;
      FrType = (uns) MIFval;
      break;
    case frname:
		frametag:
      if (*MIFstring)
        strncpy(FrName, MIFstring, FStringMax);
      break;
    case frfloat:
      FrFloat = (uns) MIFval;
      break;
    case frnsoff:
      FrNSoff = MIFval;
      break;
    case frbloff:
      FrBLoff = MIFval;
      break;
    case frrungap:
      FrRunaroundGap = MIFval;
      break;
    case fralign:
      if (MIFval < 0)
        break;
      FrAlign = (uns) MIFval;
			if (FrAlign > 2)
				FrAlign++;  // for inner and outer
      break;
    case frcrop:
      FrCrop = (uns) MIFval;
      break;

		case objprop:
      ProcFrameProps(mp);
			FrameProp = true;
			break;
		case propname:
			if (!FrameProp)
				goto frametag;
		case propval:
			ProcFrameProp(mp);
			FrameProp = false;
			break;

    case aframe:
      ProcAFrame(mp);
      break;

    case gobj:
      StartGraphic();
      ProcGrObj(mp);
      break;
    case gpoints:
      PointList = new point[(PointCount = MIFval) + 1];
      PLPos = 0;
      break;
    case grpoint:
      assert(PLPos < PointCount);
      PointList[PLPos++] = MIFpt;
      break;
    case smooth:
      Smooth = MIFval;
      break;
    case radius:
      Radius = MIFval;
      break;
    case arctheta:
      ArcTheta = MIFval;
      break;
    case arcdtheta:
      ArcDTheta = MIFval;
      break;

    case gtext:
      StartGraphic();
      ProcTextLine(mp);
      break;
    case tlpoint:
      TLPos = MIFpt;
      break;
    case tlrot:
			if (MIFval == 360)
				TAngle = 0;
      TAngle = MIFval * 100;
      break;
    case tlalign:
      if (MIFval < 0)
        break;
      TAlign = MIFval;
      break;

    case mathobj:
      StartGraphic();
      ProcMath(mp);
      break;
    case mathff:
      ProcMathFullForm(mp);
      break;
    case mathbk:
			MathLBreak = MIFval;
      break;
    case mathorg:
      MathPos = MIFpt;
      break;
    case mathalgn:
      if (MIFval < 0)
        break;
			MathAlign = MIFval;
      break;
    case mathsiz:
      if (MIFval < 0)
        break;
			MathTSize = MIFval;
      break;

    case impobj:
      StartGraphic();
      ProcImport(mp);
      break;
    case impname:
      ProcImpFile();
      break;
    case gres:
      BitMapDPI = (uns) MIFval;
      break;
    case gflip:
      if (MIFval < 0)
        break;
      FlipLR = (uns) MIFval;
      break;

    case arstyle:
      StartGraphic();
      ProcArrow(mp);
      break;
    case artang:
      TipAngle = MIFval;
      break;
    case arbang:
      BaseAngle = MIFval;
      break;
    case arlen:
      ArLength = MIFval;
      break;
    case arhead:
      if (MIFval < 0)
        break;
      ArHead = MIFval;
      break;
    case arscale:
      if (MIFval < 0)
        break;
      ArScale = (uns) MIFval;
      break;
    case arfactor:
      ArFactor = MIFval;
      break;

    case gpen:
      CurrPen = (uns) MIFval;
      break;
    case gpwid:
      CurrPenWid = (uns) MIFval;
      break;
    case gfill:
      CurrFill = (uns) MIFval;
      break;
    case gsepar:
      CurrColor = (uns) MIFval;
      break;
    case gcolor:
			ProcColor();
			if (MIFval)
	      CurrColor = (uns) MIFval;
      break;

    case lsegpat:
      CurrLineSegCount = 0;
			LineSegNum = 0;
		  EndProc[MIFlevel] = EndDashProc;
      break;
		case lineseg:
      CurrLineSeg[LineSegNum++] = (uns) MIFval;
			if (LineSegNum >= LineSegsMax)
				LineSegNum--;
      break;
    case lsegcount:
      CurrLineSegCount = (uns) MIFval;
      break;

    case headcap:
      if (MIFval < 0)
        break;
      HeadCap = MIFval;
      break;
    case tailcap:
      if (MIFval < 0)
        break;
      TailCap = MIFval;
      break;

    default:
      assert(0);
      break;
  }
}
 
 
void
MIFrd::StartGraphic(void)
{
  if (EndProc[MIFlevel - 1] == EndGFrame) { // nested in graphic frame, 
    EndGFrame();  // call its end proc before generic globals are gone
    EndProc[MIFlevel - 1] = NULL; // make sure nobody else calls it again
  }

  CurrID = 0;   // ensure ID used later is really for this frame
  CurrGID = 0;  // assume not part of graphic group, check at EndGFrame
}

 
void
MIFrd::StartGraphText(void)
{
  if ((EndProc[MIFlevel - 1] == EndTextLine)  // in graphic text line, 
   || (EndProc[MIFlevel] == EndTextLine)) // either at Font or String,
    StartTextLine();  // call start proc before attaching text object
}

// graphic frame attribute processing

bool MIFrd::FrameProp = false;

void
MIFrd::ProcFrameProps(mif *mp)
{
  DCgroup *gp = new DCgroup(DCctl(mp->dtype, mp->maj, mp->min, mp->dat));
  GroupStack[MIFlevel - 1]->add(*gp);
  GroupStack[MIFlevel] = gp;
}

void
MIFrd::ProcFrameProp(mif *mp)
{
  DCgroup *gp = GroupStack[MIFlevel];
	DCctl *cp = new DCctl(mp->dtype, mp->maj, mp->min, mp->dat);
	cp->ex(MIFstring, strlen(MIFstring) + 1);
	gp->add(*cp);
}


// group processing


uns MIFrd::CurrGID;
DCclist MIFrd::GIDList;  // graphic group ctls indexed by MIF id (CurrGID)
DCclist MIFrd::GGroupList;  // group ctls where graphic groups were added
 
 
void
MIFrd::GraphicGroup(mif *mp)
{
  DCctl *cp = new DCctl(endgroup, mp->maj, mp->min, mp->dat);
  CtlStack[MIFlevel] = cp;

  CurrGID = 0;
  EndProc[MIFlevel] = EndGGroup;
}


void
MIFrd::EndGGroup()
{
  DCctl *cp = CtlStack[MIFlevel];
  DCgroup *gp = (DCgroup *) GGroupList.find(CurrID);

  // add method for starting new group if first mention is in this ctl,
  //   which means that new group includes the one ending here
	//   in which case gp == NULL 

  if (CurrGID  // part of a graphics group
   && (GIDList.find(CurrGID) == NULL)) {  // starting it
    DCctl *ncp = new DCctl(group, 4, 1, 1); // graphic group ctl
    GIDList.add(ncp, CurrGID);
		if (gp == NULL)  // at least trap the above situation to avoid *NULL
			return;
    GGroupList.add(gp, CurrGID);
    // add to enclosing page or frame ahead of start of this group
    DCctl *ocp = GIDList.find(CurrID);
    gp->insert(*ncp, *ocp);
  }
    
  // put group end into same group as group start
  if (gp)
    gp->add(*cp);
}
 


// graphics frames


uns MIFrd::AFNum;
uns MIFrd::FrType;
uns MIFrd::FrFloat;
uns MIFrd::FrAlign;
uns MIFrd::FrCrop;
long MIFrd::FrNSoff;
long MIFrd::FrBLoff;
long MIFrd::FrRunaroundGap;
char MIFrd::FrName[FStringMax];

DCclist MIFrd::AFrameList;
DCclist MIFrd::AnchorList;

DCilist MIFrd::PrIDList; // objectID for frame indexed by frame number
char *MIFrd::GrSuffix = "wmf";
char *MIFrd::GrFileID = NULL;
bool MIFrd::UseGrFileID = true;
bool MIFrd::UseLocalFileID = false;
bool MIFrd::UseGrPreviews = false;
uns MIFrd::GrType = 4;
bool MIFrd::StartingFrame = false;

void
MIFrd::ProcGFrame(mif *mp)
{
  DCgroup *gp = new DCgroup(DCctl(mp->dtype, mp->maj, mp->min, ++FrameID));
  GroupStack[MIFlevel] = gp;       // make graphic frame the current group
	GraphTextSeq = false;
	StartingFrame = true;

  EndProc[MIFlevel] = EndGFrame;
}
 
 
void
MIFrd::EndGFrame(void)
{
  DCctl *cp = NULL;

  // we may be at end of frame, or we may be at start of first nested obj
  int rel_level = 1;  // assume end of frame
	StartingFrame = false;

  DCgroup *gp = GroupStack[MIFlevel];
  if (gp == GroupStack[MIFlevel - 1])
    rel_level = 2;  // we are duplicated on stack, so must be in middle
	DCgroup *ngp = GroupStack[MIFlevel - rel_level];
 
  FrameIDList.add(CurrID, FrameID);  // frame mif ID indexed by dcl FrameID
 
  if ((CurrGID)  // part of a graphics group
   && (GIDList.find(CurrGID) == NULL)) {  // starting it
    cp = new DCctl(group, 4, 1, 1); // graphic group ctl
    GIDList.add(cp, CurrGID);
    // add to enclosing page or frame ahead of text frame
    GGroupList.add(ngp, CurrGID);
    ngp->add(*cp);
  }
    
  // add graphic frame to enclosing page or frame
  ngp->add(*gp);

  // process anchored frame information
  if ((CurrPG == psg_aframe)        // frame is in aframe area
   && (MIFlevel == (rel_level + 1)) // at top level
   && FrType                        // not NotAnchored
   && CurrID)                       // with an ID, so it must be an AFrame
    PutAnchorProps(gp);
  else if (*FrName) {  // process reference page named frame info
		if ((MasterType == 3) // on reference page
		 && (ngp->maj() == 0) // top level below page frame itself
		 && (ngp->min() == 10)) {
			size_t len = strlen(FrName) + 1;
			char *chp = strcpy(NewName(len), FrName);
			FrameNameList.add(chp, FrameID);
			for (uns i = 1; i <= RFRefNum; i++) {
				if (!strcmp(chp, RFrameNameList.find(i)))
					(RFrameRefList.find(i))->siz(FrameID);
			}
			cp = new DCctl(ename, 3, 21, 1);
			cp->ex(chp, len);
			gp->add(*cp);
			if (UseGrPreviews)
				ProcPreviewName(gp, CurrObjectID);
		}
    *FrName = '\0';
  }
	else if (UseGrPreviews
	 && ((MasterType == 1)  // and Master Page
	  || (MasterType == 2))
	 && (ngp->maj() == 0)   // top level graphics
	 && (ngp->min() == 10))
		ProcPreviewName(gp, CurrObjectID);

  // add the rest of the frame properties
  if (FrCrop) {
    cp = new DCctl(imnem, 3, 21, 11, 1);
    gp->add(*cp);
    FrCrop = 0;
  }
  // frame size ctl, ref to enclosing frame or page
  cp = new DCctl(elong, 3, 22, (MIFlevel == (rel_level + 1)) ? 3 : 4);
  cp->ex(&CurrGrRect, sizeof(CurrGrRect)); // written from last BRect
  gp->add(*cp);

  // frame line, pen, fill, and color for frame itself
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
};


void
MIFrd::PutAnchorProps(DCgroup *gp)
{
  DCctl *cp = NULL;
  long frpos = 0;
  long frref = 0;
  long frcons = 0;
  long frcount = 0;
  long frhor = 0;
  long frvert = 0;
  point frpt;

  frhor = FrAlign + 1;
  if (FrFloat)
    frpos = 3;
  switch (FrType) {
    case 1: // inline uses FrBLoff but not FrNSoff or frhor
      frpos = 1;
      frhor = 0;
      frref = 1;
      frcons = 1;
      frcount = 1;
      break;
    case 2: // top uses align's frhor but not offsets
      frvert = 1;
      frref = 3;
      frcons = 3;
      frcount = 0;
      break;
    case 3: // below uses align's frhor but not offsets
      frpos = 2;
      frref = 1;
      frcons = 3;
      frcount = 0;
      break;
    case 4: // bottom uses align's frhor but not offsets
      frvert = 3;
      frref = 3;
      frcons = 3;
      frcount = 0;
      break;
		case 10: // inside
    case 5: // left uses offsets but not frhor
      frhor = 1;
      frref = 5;
      frcons = 2;
      frcount = 2;
      break;
		case 11: // outside
    case 6: // right uses offsets but not frhor
      frhor = 3;
      frref = 5;
      frcons = 2;
      frcount = 2;
      break;
    case 7: // near uses offsets but not frhor
      frhor = 6;
      frref = 5;
      frcons = 2;
      frcount = 2;
      break;
    case 8: // far uses offsets but not frhor
      frhor = 5;
      frref = 5;
      frcons = 2;
      frcount = 2;
      break;
		case 9: // runin uses align's frhor but not offsets
      frpos = 4;
      frref = 2;
      frcons = 3;
      frcount = 0;
      break;
    default:
      assert(0);
      break;
  }

  AFNum = CurrID;
  AFrameList.add(gp, CurrID);
  cp = new DCctl(imnem, 3, 23, 0, 0); // layout anchor ctl
  AnchorList.add(cp, CurrID); // will have data and size fields set later
  gp->add(*cp);
	if (UseGrPreviews)
		PrIDList.add(CurrObjectID, AFNum);

  if (frpos) {
    cp = new DCctl(imnem, 3, 21, 9, frpos);
    gp->add(*cp);

		if (frpos == 4) {    // frame gap
			cp = new DCctl(ilong, 3, 21, 14, FrRunaroundGap);
			gp->add(*cp);
		}
  }

  if (frcons) {
    cp = new DCctl(imnem, 3, 21, 12, frcons);
    gp->add(*cp);
  }

  if (frhor) {
    cp = new DCctl(imnem, 3, 21, 6, frhor);
    gp->add(*cp);
  }

  if (frvert) {
    cp = new DCctl(imnem, 3, 21, 7, frvert);
    gp->add(*cp);
  }

  switch (frcount) {
    case 0:
      // none needed, go by regular size ctl put out later
      break;
    case 1:
      if (FrBLoff) {  // frame size ctl with offset
        cp = new DCctl(ilong, 3, 22, (uns) frref, FrBLoff);
        gp->add(*cp);
      }
      break;
    case 2:
      cp = new DCctl(elong, 3, 22, (uns) frref);  // frame size ctl, ref to col
      frpt.x = FrNSoff;
      frpt.y = FrBLoff;
      cp->ex(&frpt, sizeof(frpt));
      gp->add(*cp);
      break;
    default:
      assert(0);
      break;
  }

  FrAlign = FrType = FrFloat = 0;
  FrNSoff = FrBLoff = 0;
}

 
void
MIFrd::ProcAFrame(mif *mp)
{
  // put out text anchor ctl
  uns frameid = (uns) FrameIDList(MIFval);
  assert(frameid);
  DCctl *cp = new DCctl(mp->dtype, mp->maj, mp->min, frameid, CurrPage);

	if (CurrPage)
    ProcAFLinks(cp);
	else if (CurrAFrame) {
		NestedAFIndex.add(cp, CurrAFrame);
		NestedAFTextID.add(cp, TextID);
	}
	else if (TbID)
		TblAFIndex.add(cp, TbID);
	else if (InNotes)
		FNoteAFIndex.add(cp, CurrID);
	else {  // must be on hidden page
		delete cp;
		return;
	}

  AddText(0);
  GroupStack[MIFlevel]->add(*cp);
}


DCvlist MIFrd::TblAFIndex;
DCvlist MIFrd::FNoteAFIndex;
long MIFrd::GrFileNumLen = 8;
long MIFrd::GrFileNumMask = 0x7FFFFFFF;

void
MIFrd::ProcAFLinks(DCctl *cp)
{
	char *chp = NULL;
	DCctl *fcp = NULL;
	long prID = 0;

	cp->siz(CurrPage);  // update text anchor ctl

  uns frameid = (uns) cp->dat();
	if (frameid == 0)
		return;
	uns mifid = (uns) FrameIDList.find(frameid);

  // update layout anchor ctl in frame
  DCctl *acp = AnchorList.find(mifid);
  acp->dat(CurrTextID);
  acp->siz(CurrPage);

  // add aframe to index, indexed by page ID
  DCgroup *gp = (DCgroup *) AFrameList.find(mifid);
  PageFrameIndex.add(gp, CurrPage);
  PageAFList.add(CurrPage, mifid);

	if (UseGrPreviews
	 && ((prID = PrIDList.find(mifid)) != 0))
		ProcPreviewName(gp, prID, mifid);
	else // if mathobj in frame, add name of .gif file
		ProcMathGifs(gp, mifid);

	// add graphics directives to graphic group
	if (GraphicsDirectives != NULL) {
		gp->insert(*GraphicsDirectives);
		GraphicsDirectives = NULL;
	}

	// if text streams already seen, set their page location
	PagelessAFTextIndex.allval(ProcAFTextIndex, mifid);
	PagelessAFTextStart.allval(ProcAFTextStart, mifid);

	// process any nested frames now
	NestedAFIndex.allval(ProcNestedAFIndex, mifid);
}


void
MIFrd::ProcPreviewName(DCgroup *gp, long prid, uns mifid)
{
	char *chp = NULL;
	DCctl *fcp = NULL;
	char *suf = GrSuffix;
	if (mifid
	 && (MathIDList.find(mifid) != 0))
		suf = EqnSuffix;

	prid &= GrFileNumMask;
	if (UseGrFileID
	 && GrFileID) {
		chp = NewName(strlen(GrFileID) + strlen(suf) + GrFileNumLen + 2);
		sprintf(chp, "%s%0.*lx.%s", GrFileID, GrFileNumLen, prid, suf);
	}
	else {
		chp = NewName(strlen(suf) + GrFileNumLen + 2);
		sprintf(chp, "%0.*lx.%s", GrFileNumLen, prid, suf);
	}
	fcp = new DCctl(ename, 3, 30, GrType);
	fcp->ex(chp);
	gp->insert(*fcp); // put at front of anchored frame
}


void
MIFrd::ProcMathGifs(DCgroup *gp, uns mifid)
{
	char *chp = NULL;
	DCctl *fcp = NULL;
	bool math = false;
	long eqnID = 0;

	// if mathobj in frame, add name of .gif file
	if (MathFrameList.find(mifid))  // MathForm must be 2
		math = true;
	else if ((eqnID = MathIDList.find(mifid)) != 0) { // MF is 3
		eqnID &= GrFileNumMask;
		if (UseGrFileID
		 && GrFileID) {
			chp = NewName(strlen(GrFileID) + strlen(EqnSuffix) + GrFileNumLen + 2);
			sprintf(chp, "%s%0.*lx.%s", GrFileID, GrFileNumLen, eqnID, EqnSuffix);
		}
		else if (EqnSuffix) {
			chp = NewName(strlen(EqnSuffix) + GrFileNumLen + 2);
			sprintf(chp, "%0.*lx.%s", GrFileNumLen, eqnID, EqnSuffix);
		}
		else {
			chp = NewName(GrFileNumLen + 5);
			sprintf(chp, "%0.*lx.gif", GrFileNumLen, eqnID);
		}
		fcp = new DCctl(ename, 4, 52, 2);
		fcp->ex(chp);
		gp->insert(*fcp); // put at front of anchored frame
		return;
	}		
	else if (!UseAllGifs)
		return;

	if ((GifID = MathParaList.find(MathParaID)) != 0) {
		GifID += MathParaSequence++;
		if ((chp = (char *) MathFileList.find(GifID)) != NULL) {
			if (math)
				fcp = new DCctl(ename, 4, 52, 2);
			else
				fcp = new DCctl(ename, 3, 21, 15);
			fcp->ex(chp);
			gp->insert(*fcp); // put at front of anchored frame
		}
	}
}

 
long MIFrd::CurrAFrame;

void
MIFrd::ProcAFTextIndex(DCctl *cp, long i, long v)
{
	if (i == v)
		PageTextIndex.add(cp, CurrPage);
}

void
MIFrd::ProcAFTextStart(DCctl *cp, long i, long v)
{
	if (i == v)
		cp->siz(CurrPage);
}

void
MIFrd::ProcNestedAFIndex(DCctl *cp, long i, long v)
{
	uns tid = CurrTextID;
	if (i == v)	{
		CurrTextID = (uns) NestedAFTextID(cp);
		ProcAFLinks(cp);  // recurse for nested frames
		CurrTextID = tid;
	}
}



// graphics objects


point *MIFrd::PointList;
long MIFrd::PointCount;
long MIFrd::PLPos;
bool MIFrd::ClosedPoly;
bool MIFrd::ShapeRect = false;

long MIFrd::Smooth;
long MIFrd::Radius;
long MIFrd::RRectExt[6];
long MIFrd::EllipseExt[5];
long MIFrd::ArcTheta;
long MIFrd::ArcDTheta;
long MIFrd::ArcExt[7];


void
MIFrd::ProcGrObj(mif *mp)
{
  DCctl *cp = new DCctl(mp->dtype, mp->maj, mp->min, mp->dat);
  CtlStack[MIFlevel] = cp;   // make object the current item
  TAngle = Smooth = PointCount = 0;
	ShapeRect = false;
  EndProc[MIFlevel] = EndGrObj;
}
 
 
void
MIFrd::EndGrObj(void)
{
  DCctl *cp = NULL;
  long startang = 0;
	ClosedPoly = true;

  if ((CurrGID)  // part of a graphics group
   && (GIDList.find(CurrGID) == NULL)) {  // starting it
    cp = new DCctl(group, 4, 1, 1); // graphic group ctl
    GIDList.add(cp, CurrGID);
    // add to enclosing page or frame ahead of object
    DCgroup *ngp = GroupStack[MIFlevel - 1];
    GGroupList.add(ngp, CurrGID);
    ngp->add(*cp);
  }   

  // add graphic obj ctl to enclosing page or frame
  DCgroup *gp = new DCgroup(DCctl(group, 4, 10, 1));
  GroupStack[MIFlevel - 1]->add(*gp);

  // modify shape ctl fields as required, add other ctls
  cp = CtlStack[MIFlevel];
  uns datval = cp->dat();
  if (Smooth) {
    switch (datval) {
      case 1:  // polyline
				ClosedPoly = false;
				// fall through
      case 2:  // polygon
        datval += 49;  // Bezier
        break;
      case 10: // rect
        datval = 11; // rounded
        break;
      case 11: // round rect
        datval = 20; // ellipse
        break;
      default:
        assert(0);
        break;
    }
    cp->dat(datval);
  }
  if ((datval == 30) && ((ArcDTheta % 360) == 0))
    cp->dat(datval = 20);  // reduce full arc to simple ellipse

  switch (datval) {
    case 10:  // rectangle
			if (TAngle == 0) {
				cp->ex(&CurrGrRect, sizeof(CurrGrRect)); // written from last BRect
				break;
			}
			cp->dat(datval = 2); // rotated, make it polygon
			PointList = new point[(PointCount = 4) + 1];
			PointList[0].x = CurrGrRect.l;
			PointList[0].y = CurrGrRect.t;
			PointList[1].x = CurrGrRect.l + CurrGrRect.w;
			PointList[1].y = CurrGrRect.t;
			PointList[2].x = CurrGrRect.l + CurrGrRect.w;
			PointList[2].y = CurrGrRect.t + CurrGrRect.h;
			PointList[3].x = CurrGrRect.l;
			PointList[3].y = CurrGrRect.t + CurrGrRect.h;
			RotatePoints(false);
      cp->ex(PointList, PointCount * sizeof(point));
      delete [] PointList;
			break;

    case 1:  // polyline
			ClosedPoly = false;
			// fall through
    case 2:  // polygon
      if (PointCount) {
				if (TAngle)
					RotatePoints(false);
        cp->ex(PointList, PointCount * sizeof(point));
        delete [] PointList;
      }
      break;

    case 50: // open Bezier
			ClosedPoly = false;
			// fall through
    case 51: // closed Bezier
      if (PointCount) {
				if (TAngle)
					RotatePoints(true);
        cp->ex(PointList, PointCount * sizeof(point));
        delete [] PointList;
      }
      break;
    case 11:  // rounded rectangle
      RRectExt[0] = CurrGrRect.l;
      RRectExt[1] = CurrGrRect.t;
      RRectExt[2] = CurrGrRect.w;
      RRectExt[3] = CurrGrRect.h;
      RRectExt[4] = Radius;
      RRectExt[5] = Radius;
      cp->ex(RRectExt, sizeof(RRectExt));
      break;
    case 20:  // ellipse
      SetArcValues(EllipseExt);
      cp->ex(EllipseExt, sizeof(EllipseExt));
      break;
    case 30:  // arc
      SetArcValues(ArcExt);
      startang = (360 - ArcTheta) + 90 + ArcExt[4];
      if (ArcDTheta < 0) { // counterclockwise
        datval = 32; // for PutGProps, not for the ctl
        ArcExt[5] = BoundAngle(startang);
        ArcExt[6] = BoundAngle(startang - ArcDTheta);
      }
      else {
        ArcExt[5] = BoundAngle(startang - ArcDTheta);
        ArcExt[6] = BoundAngle(startang);
      }
      cp->ex(ArcExt, sizeof(ArcExt));
      break;
    default:
      assert(0);
      break;
  }

  // put out object properties
  PutGProps(gp, datval);

  // add the shape ctl to the group
  gp->add(*cp);
	ShapeRect = false;
}

#define PI 3.1415926535

void
MIFrd::RotatePoints(bool IsBezier)
{
	if (!PointCount)
		return;

	long i;
	long cx, xcent;
	long cy, ycent;

	if (IsBezier == true) {
		// compute bounding box of actual curve, not of control points
		GetBezCenter(xcent, ycent);
	}
#if 0
	else {  // compute bounding box of points listed
		long xmax, xmin;
		long ymax, ymin;
		xmax = xmin = PointList[0].x;
		ymax = ymin = PointList[0].y;

		for (i = 1; i < PointCount; i++) {
			cx = PointList[i].x;
			if (cx < xmin)
				xmin = cx;
			else if (cx > xmax)
				xmax = cx;
			cy = PointList[i].y;
			if (cy < ymin)
				ymin = cy;
			else if (cy > ymax)
				ymax = cy;
		}

		xcent = ((xmax - xmin) / 2) + xmin;
		ycent = ((ymax - ymin) / 2) + ymin;
	}
	else {  // compute center of gravity of points listed
		double xtot = 0.0;
		double ytot = 0.0;

		for (i = 0; i < PointCount; i++) {
			xtot += (double) PointList[i].x;
			ytot += (double) PointList[i].y;
		}

		xcent = (long) (xtot / PointCount);
		ycent = (long) (ytot / PointCount);
	}
#endif
	else {  // compute avg of midpoints of line segments
	        // weighted by segment lengths
		double dd = 0.0;
		double td = 0.0;
		double tx = 0.0;
		double ty = 0.0;
		long dx = 0;
		long dy = 0;

		long lx = PointList[0].x;
		long ly = PointList[0].y;
		long count = PointCount;

		if (ClosedPoly) {
			PointList[count] = PointList[0];
			count++;
		}

		for (i = 1; i < count; i++) {
			dx = PointList[i].x - lx;
			dy = PointList[i].y - ly;
			dd = sqrt(((double) dx * dx) + ((double) dy * dy));
			td += dd;
			tx += (lx + (dx / 2)) * dd;
			ty += (ly + (dy / 2)) * dd;
			lx = PointList[i].x;
			ly = PointList[i].y;
		}

		xcent = (long) (tx / td);
		ycent = (long) (ty / td);
	}

	for (i = 0; i < PointCount; i++) {
		cx = PointList[i].x - xcent;
		cy = PointList[i].y - ycent;
		switch (TAngle) {
			case 9000:
				PointList[i].x = xcent + cy;
				PointList[i].y = ycent - cx;
				break;
			case 18000:
				PointList[i].x = xcent - cx;
				PointList[i].y = ycent - cy;
				break;
			case 27000:
				PointList[i].x = xcent - cy;
				PointList[i].y = ycent + cx;
				break;
			case 0:
			case 36000:
				break;
			default:  // odd angle, compute new point ****
				// use atan2 to get the angle to the point
				if ((cy == 0) && (cx == 0)) // avoid domain error at origin
					break;
				double da = atan2((double) cy, (double) cx);
				// subtract the angle of rotation
				da -= (((double) TAngle) * PI) / 18000.0;
				if (da > PI)
					da -= PI * 2;
				else if (da < -PI)
					da += PI * 2;
				// use sin and cos to compute x and y offsets
				double len = sqrt(((double) cx * cx) + ((double) cy * cy));
				cx = (long) (len * cos(da));
				cy = (long) (len * sin(da));
				// add offsets to center position
				PointList[i].x = xcent + cx;
				PointList[i].y = ycent + cy;
				break;
		}
	}
}


// Bezier computation

long MIFrd::BezFlatness = 3;
long MIFrd::BezDistMax;
long MIFrd::BezLevel;

#if 0
long MIFrd::BezXMin;
long MIFrd::BezXMax;
long MIFrd::BezYMin;
long MIFrd::BezYMax;
#endif

long MIFrd::BezLX;
long MIFrd::BezLY;
double MIFrd::BezTX;
double MIFrd::BezTY;
double MIFrd::BezTD;


void
MIFrd::GetBezCenter(long& xcent, long& ycent)
{
  //BezFlatness = 3L; // flatness measure for 1 pixel at 300dpi in twips

  //BezFlatness is in twips (pts * 20)
  switch (Upage) {  // convert twips to current page units
    case 1:  // inches
      BezDistMax = (BezFlatness * 5000) / 72;
      break;
    case 2:  // microns
      BezDistMax = (BezFlatness * 1270) / 72;
      break;
    case 3:  // pts * 100
			BezDistMax = BezFlatness * 5;
      break;
    case 4:  // Didot
      BezDistMax = (BezFlatness * 336) / 72;
      break;
    default:  // unknown, should never happen
			BezDistMax = 15L;
      break;
  }

#if 0
	BezXMax = BezXMin = PointList[0].x;
	BezYMax = BezYMin = PointList[0].y;
#endif

	BezLX = PointList[0].x;
	BezLY = PointList[0].y;
	BezTX = BezTY = BezTD = 0.0;

	BezLevel = 0;

	uns bezct = (uns) ((PointCount - 1) / 3);

  long bezpt[8];

  for (uns bez = 0; bez < bezct; bez++) {
    for (uns i = 0; i < 4; i++)	{
      bezpt[i * 2] = PointList[i + (bez * 3)].x;
      bezpt[(i * 2) + 1] = PointList[i + (bez * 3)].y;
		}
    ProcBezArc(bezpt);
  }

	CheckBezBounds(bezpt[6], bezpt[7]);  // end point

	if (ClosedPoly
	 && ((bezpt[6] != PointList[0].x)
	  || (bezpt[7] != PointList[0].y)))
		CheckBezBounds(PointList[0].x, PointList[0].y);

#if 0
	xcent = ((BezXMax - BezXMin) / 2) + BezXMin;
	ycent = ((BezYMax - BezYMin) / 2) + BezYMin;
#endif

	xcent = (long) (BezTX / BezTD);
	ycent = (long) (BezTY / BezTD);
}


void
MIFrd::CheckBezBounds(long cx, long cy)
{
#if 0
	if (cx < BezXMin)
		BezXMin = cx;
	else if (cx > BezXMax)
		BezXMax = cx;

	if (cy < BezYMin)
		BezYMin = cy;
	else if (cy > BezYMax)
		BezYMax = cy;
#endif

	long dx = cx - BezLX;
	long dy = cy - BezLY;
	double dd = sqrt(((double) dx * dx) + ((double) dy * dy));
	BezTD += dd;
	BezTX += (BezLX + (dx / 2)) * dd;
	BezTY += (BezLY + (dy / 2)) * dd;
	BezLX = cx;
	BezLY = cy;
}


void
MIFrd::ProcBezArc(long *bezpt)
{
  if ((BezDistMax == 0)
	 || BezFlat(bezpt)) {
		CheckBezBounds(bezpt[0], bezpt[1]);
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
  ProcBezArc(lbez);
  ProcBezArc(rbez);
}


bool
MIFrd::BezFlat(long *bezpt)
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



// Arc computation


void
MIFrd::SetArcValues(long *lptr)
{
	// determine centerpoint xy
  lptr[0] = CurrGrRect.l + (CurrGrRect.w / 2);
  lptr[1] = CurrGrRect.t + (CurrGrRect.h / 2);

	// major axis, minor axis, rotation
  if (CurrGrRect.h <= CurrGrRect.w) { // normalized
	  lptr[2] = CurrGrRect.w / 2;
		lptr[3] = CurrGrRect.h / 2;
		lptr[4] = 0L;
  }
	else {	// rotate 90 degrees ccw
		lptr[2] = CurrGrRect.h / 2;
		lptr[3] = CurrGrRect.w / 2;
		lptr[4] = 9000L;
	}
}


long
MIFrd::BoundAngle(long ang)
{
  while (ang < 0)
    ang += 360;
  while (ang >= 360)
    ang -= 360;
  return (ang * 100);
}



// graphics text processing


bool MIFrd::GraphicText = false;
bool MIFrd::GraphTextStart = false;
bool MIFrd::GraphTextFontStart = false;
bool MIFrd::GraphTextForm = false;
bool MIFrd::GraphTextSeq = false;
point MIFrd::TLPos;
long MIFrd::TAngle;
long MIFrd::TAlign;
long MIFrd::TLExt[3];
gtprop MIFrd::CurrGrTextProp;

grcform *MIFrd::CurrGrCForm;
DCvlist MIFrd::GCFormList;


void
MIFrd::ProcTextLine(mif *mp)
{
  DCgroup *gp = new DCgroup(DCctl(mp->dtype, mp->maj, mp->min, mp->dat));
  GroupStack[MIFlevel] = gp;   // make object the current group
  TAngle = TAlign = 0;
  EndProc[MIFlevel] = EndTextLine;
	GraphTextStart = true;
	GraphTextFontStart = false;
  GraphicText = true;
}

 
void
MIFrd::StartTextLine(void)
{
  DCctl *cp = NULL;

  // we may be at end of frame, or we may be at start of first nested obj
  int rel_level = 1;  // assume end of frame
  DCgroup *gp = GroupStack[MIFlevel];
  if (gp == GroupStack[MIFlevel - 1])
    rel_level = 2;  // we are duplicated on stack, so must be in middle
 
  if ((CurrGID)  // part of a graphics group
   && (GIDList.find(CurrGID) == NULL)) {  // starting it
    cp = new DCctl(group, 4, 1, 1); // graphic group ctl
    GIDList.add(cp, CurrGID);
    // add to enclosing page or frame ahead of object
    DCgroup *ngp = GroupStack[MIFlevel - rel_level];
    GGroupList.add(ngp, CurrGID);
    ngp->add(*cp);
  }   

  // add graphic text group to enclosing page or frame
  GroupStack[MIFlevel - rel_level]->add(*gp);

  // add text attr ctl
  cp = new DCctl(elong, 4, 40, TAlign + 1);
  TLExt[0] = TLPos.x;
  TLExt[1] = TLPos.y;
  TLExt[2] = TAngle;
  cp->ex(TLExt, sizeof(TLExt));
  gp->add(*cp);
  //cp = new DCctl(imnem, 2, 15, 13, CurrFnt.color);
  //gp->add(*cp);
  GraphTextStart = false;

	if (!GraphTextSeq) {  // need properties now
		if (!GraphTextFontStart)
			PutGrTextProps(gp);
		GraphTextSeq = true;
	}
}


void
MIFrd::PutGrTextProps(DCgroup *gp)
{
	DCctl *cp = NULL;

	// put out curr graph text props
	if (CurrGrTextProp.fname
	 && (CurrGrTextProp.fname[0] != '\0')) {
		cp = new DCctl(ename, 2, 15, 1);
		cp->ex(CurrGrTextProp.fname, strlen(CurrGrTextProp.fname) + 1);
	}
	else if (CurrGrTextProp.fnum)
		cp = new DCctl(imnem, 2, 15, 1, CurrGrTextProp.fnum);
	else
		cp = new DCctl(imnem, 2, 15, 1, 3);  // Helvetica
	gp->add(*cp);
	if (CurrGrTextProp.fsize)
		cp = new DCctl(ilong, 2, 15, 2, CurrGrTextProp.fsize);
	else
		cp = new DCctl(ilong, 2, 15, 2, 900); // 9 pt
	gp->add(*cp);
	cp = new DCctl(imnem, 2, 15, 4, CurrGrTextProp.fbold);
	gp->add(*cp);
	cp = new DCctl(imnem, 2, 15, 5, CurrGrTextProp.fital);
	gp->add(*cp);
	cp = new DCctl(imnem, 2, 15, 11, CurrGrTextProp.fundl);
	gp->add(*cp);
	cp = new DCctl(imnem, 2, 15, 9, CurrGrTextProp.fstrike);
	gp->add(*cp);
	cp = new DCctl(imnem, 2, 15, 13, CurrGrTextProp.color);
	gp->add(*cp);
	SymFont = CurrGrTextProp.fsym;

}


bool
MIFrd::GetGrCForm(long val)
{
	grcform *gcf = (grcform *) GCFormList.find(val);
	if (gcf == NULL)
		return false;

	UseGrCForm(gcf);
	DefGrCForm = gcf;

	return true;
}


void
MIFrd::UseGrCForm(grcform *gcf)
{
	if (gcf->fname != NULL) {
		if (CurrGrTextProp.fname != NULL)
			DeleteName(CurrGrTextProp.fname);
		CurrGrTextProp.fname = NewName(gcf->fname);
	}
	if (gcf->fnum != 0)
		CurrGrTextProp.fnum = gcf->fnum;
	if (gcf->fsize != 0)
		CurrGrTextProp.fsize = gcf->fsize;
	if (gcf->fbset == true)
		CurrGrTextProp.fbold = gcf->fbold;
	if (gcf->fiset == true)
		CurrGrTextProp.fital = gcf->fital;
	if (gcf->fuset == true)
		CurrGrTextProp.fundl = gcf->fundl;
	if (gcf->fsset == true)
		CurrGrTextProp.fstrike = gcf->fstrike;
	if (gcf->fsymset == true)
		SymFont = CurrGrTextProp.fsym = gcf->fsym;
	if (gcf->color != 0)
		CurrGrTextProp.color = gcf->color;
}


char *MIFrd::DefGrTextName = "Times New Roman";
long MIFrd::DefGrTextSize = 1200;
grcform *MIFrd::DefGrCForm = NULL;
bool MIFrd::UseDefGrCForm = false;

void
MIFrd::SetDefGrTextProps()
{
	if (CurrGrTextProp.fname != NULL)
		DeleteName(CurrGrTextProp.fname);
	CurrGrTextProp.fname = NewName(DefGrTextName);

	CurrGrTextProp.fnum = 0;
	CurrGrTextProp.fsym = SymFont = false;

	if (DefGrTextSize == 0) {
    switch (Ufont) {
      case 1:
        //val = (val * 1000) / 72;
				DefGrTextSize = 16667;
        break;
      case 2:
        //val = (val * 254) / 72;
				DefGrTextSize = 4233;
        break;
      case 3:
				DefGrTextSize = 1200;
        break;
      case 4:
        //val = (val * 672) / 720;
				DefGrTextSize = 1120;
        break;
      default:
				DefGrTextSize = 1200;
        break;
    }
	}
	CurrGrTextProp.fsize = DefGrTextSize;

	CurrGrTextProp.fbold = 0;
	CurrGrTextProp.fital = 0;
	CurrGrTextProp.fundl = 0;
	CurrGrTextProp.fstrike = 0;
	CurrGrTextProp.color = 1;

	if (UseDefGrCForm
	 && DefGrCForm)
		UseGrCForm(DefGrCForm);
}

 
void
MIFrd::EndTextLine(void)
{
  if (GraphTextStart)  // empty line
    StartTextLine();
  else FlushText();
	GraphicText = false;
}



// math object processing

uns MIFrd::MathNum = 0;
uns MIFrd::MathForm = 3;
bool MIFrd::MathFullForm = false;
long MIFrd::MathAlign;
point MIFrd::MathPos;
long MIFrd::MathTSize;
long MIFrd::MathLBreak;
long MIFrd::MathExt[4];
DCctl *MIFrd::MathFFCtl = NULL;

long MIFrd::MathParaID = 0;
long MIFrd::MathParaSequence = 0;
long MIFrd::GifID = 0;
DClist MIFrd::MathFrameList; // identifies frames containing math objs
DCilist MIFrd::MathIDList; // objectID for eqn indexed by frame number
DCilist MIFrd::MathParaList; // first GifID for para indexed by MathParaID
DCnlist MIFrd::MathFileList; // indexed by GifID
char *MIFrd::HTSuffix = "htm";
char *MIFrd::EqnSuffix = NULL;
bool MIFrd::UseAllGifs = false;


void
MIFrd::ScanHTMath(MIFrfile *rf)
{
	// scan filename.htm file for pgfIDs and graphic SRCs
	char HTstr[PATHMAX];
	sprintf(HTstr, ".\\%s.%s", rf->getBaseName(), HTSuffix);
	DCrfile hf(HTstr);
  if (hf.FErr() != fok) {
		MathForm = 0;
		UseAllGifs = false;
		return;
	}

	sprintf(HTstr, "<IMG SRC=\"%s-", rf->getBaseName());
	size_t len = strlen(HTstr);
	char *hb = new char[TextBufMax];
	char *hp = NULL;
	char *hend = NULL;

	GifID = 0;

	while (hf.getLine(hb, TextBufMax)) {
		for (hp = hb; hp != NULL; hp = strchr(hp, '<')) {
			if (!strncmp(hp, "<A NAME=\"pgfId=", 15)) {
				MathParaSequence = 0;
				MathParaID = atol(hp += 15);
			}
			else if (!strncmp(hp, HTstr, len)) {
				GifID++;   // found another IMG
				hp += 10;  // first char of name after quote
				hend = strchr(hp, '"'); // quote after last char
				if (hend > hp)  // something there
					MathFileList.add(NewName(hp, hend - hp), GifID);
				if (MathParaSequence++ == 0)  // only add once per MathParaID
					MathParaList.add(GifID, MathParaID); // first GifID in para
			}
			else hp++;
		}
	}
	delete [] hb;
}


void
MIFrd::ProcMath(mif *mp)
{
  DCgroup *gp = new DCgroup(DCctl(mp->dtype, mp->maj, mp->min, mp->dat));
  GroupStack[MIFlevel] = gp;   // make object the current group
  MathAlign = MathTSize = MathLBreak = 0;
	MathPos.x = MathPos.y = 0;
  TAngle = 0L;
  MIFrect.l = MIFrect.t = MIFrect.w = MIFrect.h = 0;
	MathNum++;
  EndProc[MIFlevel] = EndMath;
}


void
MIFrd::ProcMathFullForm(mif *mp)
{
	if ((*MIFstring != '\0')
	 && ((MathForm == 1) || MathFullForm)) {
		MathFFCtl = new DCctl(mp->dtype, mp->maj, mp->min, mp->dat);
		MathFFCtl->ex(MIFstring);
	}
	delete [] MIFstring;  // 32K buffer, release it
	MIFstring = NULL;
}


void
MIFrd::EndMath(void)
{
	if (MathForm == 0)
		return;

  DCgroup *gp = GroupStack[MIFlevel];
  DCctl *cp = NULL;

	// put out equ type
	cp = new DCctl(elong, 4, 50, 4);
	MathExt[0] = MathPos.x;
	MathExt[1] = MathPos.y;
	MathExt[2] = TAngle;
	cp->ex(MathExt, 3 * sizeof(long));
  gp->add(*cp);

	// put out size control
	cp = new DCctl(elong, 4, 51, 1);
  MathExt[0] = MIFrect.l;
  MathExt[1] = MIFrect.t;
  MathExt[2] = MIFrect.w;
  MathExt[3] = MIFrect.h;
	cp->ex(MathExt, 4 * sizeof(long));
  gp->add(*cp);

	// set reference for adding .gif later
	if (MathForm == 2)
		MathFrameList.add(AFNum);  // applies to anchored frame
	else if (MathForm == 3)
		MathIDList.add(CurrObjectID, AFNum);
	
	// put out other props
	cp = new DCctl(imnem, 4, 51, 2, MathAlign);
  gp->add(*cp);

	cp = new DCctl(imnem, 4, 51, 3, MathTSize);
  gp->add(*cp);

	//if (MathLBreak) {
	//	cp = new DCctl(ilong, 4, 51, 4, MathLBreak);
	//  gp->add(*cp);
	//}

  cp = new DCctl(imnem, 4, 20, 2, CurrColor);
  gp->add(*cp);

	// put out fullform content
	if (MathFFCtl) {
	  gp->add(*MathFFCtl);
		MathFFCtl = NULL;
	}

  GroupStack[MIFlevel - 1]->add(*gp);
}



// import object processing


DCctl *MIFrd::ImpFileC;
uns MIFrd::BitMapDPI; 
uns MIFrd::FlipLR;
long MIFrd::ImpExt[9];
uns MIFrd::ExNum = 0;
uns MIFrd::SubNum = 0;
bool MIFrd::ExWmfFiles = false;
bool MIFrd::ExBmpFiles = false;
bool MIFrd::ExRfFiles =  false;
bool MIFrd::ExPctFiles = true;
bool MIFrd::ExTifFiles = true;
bool MIFrd::ExPcxFiles = true;
bool MIFrd::ExWpgFiles = true;
bool MIFrd::ExGifFiles = true;
bool MIFrd::ExJpgFiles = true;
bool MIFrd::ExPngFiles = true;
bool MIFrd::ExEpsFiles = true;
bool MIFrd::ExCdrFiles = true;
bool MIFrd::ExOleFiles = false;
bool MIFrd::UseFrameImage = false;
bool MIFrd::FixGraphicNames = false;
char *MIFrd::GraphicRenameBat = "rengraphics.bat";
FILE *MIFrd::GraphicRenBatFile = NULL;
 

void
MIFrd::ProcImport(mif *mp)
{
  DCgroup *gp = new DCgroup(DCctl(mp->dtype, mp->maj, mp->min, mp->dat));
  GroupStack[MIFlevel] = gp;   // make object the current group
  BitMapDPI = FlipLR = 0;
  TAngle = 0L;
  MIFrect.l = MIFrect.t = MIFrect.w = MIFrect.h = 0;
  MIFvector = MIFraster = MIFepsi = NULL;
	ExNum++;
	SubNum = 0;
  EndProc[MIFlevel] = EndImport;
}


void
MIFrd::ProcImpFile(void)
{
  ImpFileC = new DCctl(ename, 1, 50, 3);
	if (*MIFstring == '\0')
		strcpy(MIFstring, "Unknown.obj");
	if (FixGraphicNames
	 && strcmp(MIFstring, "2.0 internal inset")) {
		char *slash = strrchr(MIFstring, '/');
		char *bslash = strrchr(MIFstring, '\\');
		char *name = ((slash && bslash) ? ((slash > bslash) ? slash : bslash) : 
		             (slash ? slash + 1 : 
		             (bslash ? bslash + 1 : 
		              MIFstring)));
		char *dot = strrchr(name, '.');
		char *oname = NewName(name);
		char *path = (name > MIFstring) ? NewName(MIFstring, (name - MIFstring)) : NULL;
		char *ptr = name;
		while (*ptr) {
			if (((*ptr == '.')
			  && (ptr != dot))
			 || (*ptr == ' '))
				*ptr = '_';
			if (*ptr == '{') {
				while (*ptr && (*ptr != '}'))
					ptr++;
				name = ptr + 1;
			}
			ptr++;
		}
		char *nname = NewName(name);
		if (!strcmp(nname, oname))
			ImpFileC->ex(MIFstring, strlen(MIFstring) + 1);
		else if (path) {
			char *dpath = NewName(path);
			ptr = dpath;
			while (*ptr) {
				if (*ptr == '/')
					*ptr = '\\';
				ptr++;
			}
			fprintf(GraphicRenBatFile, "ren \"%s%s\" \"%s\"\n", dpath, oname, nname);
			DeleteName(oname);
			DeleteName(dpath);
			oname = NewName(path, strlen(path) + strlen(nname));
			strcat(oname, nname);
			ImpFileC->ex(oname, strlen(oname) + 1);
			DeleteName(nname);
		}
		else {
			fprintf(GraphicRenBatFile, "ren \"%s\" \"%s\"\n", oname, nname);
			DeleteName(oname);
			ImpFileC->ex(nname, strlen(nname) + 1);
		}
	}
	else
		ImpFileC->ex(MIFstring, strlen(MIFstring) + 1);
}


void
MIFrd::EndImport(void)
{
  DCgroup *gp = GroupStack[MIFlevel];

  // put out line, pen, and fill CTLs
	PutGProps(gp, 0);

  ImpExt[0] = FlipLR ? (MIFrect.l + MIFrect.w) : MIFrect.l;
  ImpExt[1] = MIFrect.t;
  ImpExt[2] = FlipLR ? -MIFrect.w : MIFrect.w;
  ImpExt[3] = MIFrect.h;

	// process in order of priority, with externally-referenced last:
	// WMF, DIB, SunRaster (= FrameImage), EPSI, TIFF, [FrVect], [PICT]
  
  if (MIFepsi && KeepEpsi)  // keep epsi object even if it has raster
		ProcEpsi(gp);
  else if (MIFraster && (MIFrastype == bmp))  // DIB format raster
		ProcDIB(gp);
  else if (MIFraster && (MIFrastype == rf)    // Sun (FrameImage) raster
	 && UseFrameImage)
		ProcSunRF(gp);
	else if (MIFvector && (MIFvectype == wmf))  // WMF after raster
		ProcWMF(gp);
	else if (MIFraster && (MIFrastype == tif)   // TIFF raster for export
	 && ExTifFiles)
		ProcTiff(gp);
	else if (MIFraster && (MIFrastype == gif)   // Compuserve GIF raster
	 && ExGifFiles) {
		ProcExport(gp, "gif", MIFraster);
		MIFraster = NULL;
	}
	else if (MIFraster && (MIFrastype == jpg)   // JPEG raster
	 && ExJpgFiles) {
		ProcExport(gp, "jpg", MIFraster);
		MIFraster = NULL;
	}
	else if (MIFraster && (MIFrastype == png)   // PNG raster
	 && ExPngFiles) {
		ProcExport(gp, "png", MIFraster);
		MIFraster = NULL;
	}
	else if (MIFraster && (MIFrastype == pcx)   // PC Paintbrush PCX raster
	 && ExPcxFiles) {
		ProcExport(gp, "pcx", MIFraster);
		MIFraster = NULL;
	}
	else if (MIFvector && (MIFvectype == pct)   // Mac PICT for export
	 && ExPctFiles) {
		ProcExport(gp, "pct", MIFvector);
		MIFvector = NULL;
	}
	else if (MIFvector && (MIFvectype == wpg)   // WordPerfect graphic
	 && ExWpgFiles) {
		ProcExport(gp, "wpg", MIFvector);
		MIFvector = NULL;
	}
	else if (MIFvector && (MIFvectype == cdr)   // Corel Draw graphic
	 && ExCdrFiles) {
		ProcExport(gp, "cdr", MIFvector);
		MIFvector = NULL;
	}
	else if (MIFvector && (MIFvectype == ole)) {  // OLE2 object
		if (ExOleFiles) {
			ProcExport(gp, "ole", MIFvector);
			MIFvector = NULL;
		}
		else if (FindOleWMF())
			ProcWMF(gp);
	}
  else if (MIFraster && (MIFrastype == tif))  // TIFF format raster
		ProcTiff(gp);
  else if (MIFepsi)  // internal epsi object, without a raster
		ProcEpsi(gp);

#ifdef VECINSET
  else if (MIFvector && (MIFvectype == fmv))  // FrameVector format object
		ProcFrVect(gp);
#endif

#ifdef PICTOK
  else if (MIFvector && (MIFvectype == pct))  // Mac PICT format object
		ProcPICT(gp);
#endif

  else if (MIFraster && (MIFrastype == rf))   // Sun (FrameImage) raster
		ProcSunRF(gp);
  else if (ImpFileC  // something external
	 && (ImpFileC->ex() != NULL)
	 && strcmp((char *) ImpFileC->ex(), "2.0 internal inset"))
		ProcExtern(gp);

  if (ImpFileC) // left over after internal inset was used
    delete ImpFileC;

	delete MIFraster;
	delete MIFvector;
	delete MIFepsi;

  GroupStack[MIFlevel - 1]->add(*gp);
}


DCctl *
MIFrd::ProcExport(DCgroup* gp, char *ext, DCblock *content)
{
	DCctl *cp = NULL;

	if (ImpFileC)
		delete ImpFileC;
	ImpFileC = new DCctl(ename, 1, 50, 3);

	cp = new DCctl(ename, 0, 0, ExNum);
	cp->ex(ext);
	GroupList[psg_exfile]->add(*cp);

	if (SubNum) {
		cp = new DCctl(ishort, 0, 0, SubNum);
		GroupList[psg_exfile]->add(*cp);
	}

	cp = new DCctl(ilong, 0, 0, ExNum, (unl) ImpFileC);
	GroupList[psg_exfile]->add(*cp); // pointer to get name
	if (gp)
		ProcExtern(gp);

	cp = new DCctl(ebyte, 0, 0, ExNum);
	if (content)
		*cp = *content;
	GroupList[psg_exfile]->add(*cp);

	return cp;
}


bool
MIFrd::ProcExtern(DCgroup* gp)
{
  gp->dat(6);  // change type to unknown

  // put out size as frame size CTL
  DCctl *cp = new DCctl(elong, 3, 22, 4);
  cp->ex(ImpExt, 4 * sizeof(long));
  gp->add(*cp);

	// angle is frame attribute angle CTL
  cp = new DCctl(ilong, 3, 21, 13, TAngle);
  gp->add(*cp);

	if (BitMapDPI) {
		cp = new DCctl(ilong, 4, 63, 1, BitMapDPI);
		gp->add(*cp);
	}

  gp->add(*ImpFileC);
	ImpFileC = NULL;
	return true;
}

#ifdef PICTOK

bool
MIFrd::ProcPICT(DCgroup* gp)
{
	DCctl *cp = NULL;
	unc* ucp = (unc*) MIFvector->p();

	// always export
	if (ExPctFiles) {
		ProcExport(gp, "pct", MIFvector);

#if 0
		if (ImpFileC)
			delete ImpFileC;
	  ImpFileC = new DCctl(ename, 1, 50, 3);

		cp = new DCctl(ename, 0, 0, ExNum);
		cp->ex("pct", 4);
		GroupList[psg_exfile]->add(*cp);

		cp = new DCctl(ilong, 0, 0, ExNum, (unl) ImpFileC);
		GroupList[psg_exfile]->add(*cp); // pointer to get name
		ProcExtern(gp);

		cp = new DCctl(ebyte, 0, 0, ExNum);
		*cp = *MIFvector;
		GroupList[psg_exfile]->add(*cp);
#endif

		MIFvector = NULL;
		return true;
	}

	MIFvector = NULL;
	return false;
}

#endif

bool
MIFrd::FindOleWMF(void)
{
	const long bbsize = 0x200;  // big block size
	const long sbsize = 0x40;   // small block size
	const long ppssize = 0x80;  // property storage size

	const long bl_unused = -1;
	const long bl_endchain = -2;

	// conduct sanity check of header
	long *chkp = (long*) MIFvector->p();
	if ((chkp[2] || chkp[3] || chkp[4] || chkp[5] || chkp[9] || chkp[10])
	 || (chkp[11] < 1)  // no bbd blocks
	 || (chkp[12] < 1)  // no root blocks
	 // || (chkp[11] > 12) // too many bbd blocks
	 || (*(chkp + 19 + chkp[11]) != -1)) { // no unused bbd block at end
		delete MIFvector;  // and original data
		MIFvector = NULL;
		return false;
	}

	// normalize OLE file first
	unc *headp = (unc*) MIFvector->p();
	DCblock *tp = new DCblock(MIFvector->l() - bbsize); // omit header
	unc *endp = (unc *) tp->p();  // next place to write normalized data
	unc *lastp = endp + tp->l();  // sentinel at end
	unc *blockp = NULL;  // block to be written to normalized area
	unc *basep = headp + bbsize;  // base of block 0 in original data

	// gather big block depot parts
	long *bbdp = (long *) endp;  // start of normalized big block depot
	unl ctr = *((unl *)(headp + 0x2C));  // count of bbd blocks
	long *hbdp = (long *) (headp + 0x4C);  // first header bbd entry
	long bpos = *hbdp; // position of first bbd block in file, usually 0
	while ((bpos >= 0) && (ctr > 0)) { // step through to "unused", -1
		blockp = basep + (bpos * bbsize);  // bbd block
		if ((endp + bbsize) <= lastp)
			memmove(endp, blockp, bbsize);
		endp += bbsize;
		ctr--;
		hbdp++;
		bpos = *hbdp;  // next array pos contains next bbd block number
	}
	assert((bpos == bl_unused) && (ctr == 0));

	// gather small block depot parts via bbd
	long *sbdp = (long *) endp;  // start of normalized small block depot
	bpos = *((long *)(headp + 0x3C));  // position of first sbd block
	while (bpos >= 0) {  // length not stored, follow chain until end, -2
		blockp = basep + (bpos * bbsize);  // sbd block
		if ((endp + bbsize) <= lastp)
			memmove(endp, blockp, bbsize);
		endp += bbsize;
		bpos = bbdp[bpos];  // bbd contains position of next block
	}
	assert(bpos == bl_endchain);

	// gather root chain of pps's
	long *rchp = (long *) endp;  // start of normalized root chain
	bpos = *((long *)(headp + 0x30));  // position of first pps block
	while (bpos >= 0) {  // length not stored, follow chain
		blockp = basep + (bpos * bbsize);  // root chain block
		if ((endp + bbsize) <= lastp)
			memmove(endp, blockp, bbsize);
		endp += bbsize;
		bpos = bbdp[bpos];
	}
	assert(bpos == bl_endchain);

	unc *ssp = (unc *) endp;  // scan starting point, data begins here

	// gather chains from big pps's
	unc *ppsp = (unc *) rchp;  // scan ptr for root chain
	unc *rootpps = NULL;       // ptr for Root Entry pps 
	long pplen = 0;       // length of data for this pps
	for ( ; ppsp < (unc *) ssp; ppsp += ppssize) { // scan normalized chain
		if ((ppsp[0] == 0)  // no name, unused entry
		 && (ppsp[1] == 0))
			continue;
		if (ppsp[0x42] == 5) { // ID for root entry
			rootpps = ppsp;
			continue;
		}
		if (ppsp[0x42] != 2) // require stream, not storage
			continue;
		if (((pplen = *((long *) (ppsp + 0x78))) == 0)  // unused
		 || (pplen < 0x1000)) // and long
			continue;
		bpos = *((long *) (ppsp + 0x74));  // starting block for pps
		while ((bpos >= 0) && (pplen > 0)) {
			blockp = basep + (bpos * bbsize);  // pps big block
			if ((endp + bbsize) <= lastp)
				memmove(endp, blockp, bbsize);
			endp += bbsize;
			pplen -= bbsize;
			bpos = bbdp[bpos];
		}
		assert((bpos == bl_endchain) && (pplen <= 0));
	}

	// gather small block file, in bbd sequence, in temp storage
	long sbflen = *((long *) (rootpps + 0x78)); // from Root Entry
	DCblock *sp = new DCblock((((sbflen + (bbsize - 1)) / bbsize) * bbsize));
	unc *usp = (unc *) sp->p();  // place to write in temp sbf
	unc *lusp = usp + sp->l();   // sentinel
	pplen = *((long *) (rootpps + 0x78)); // length of sbf
	bpos = *((long *) (rootpps + 0x74));  // starting block of sbf
	while ((bpos >= 0) && (pplen > 0)) {
		blockp = basep + (bpos * bbsize);  // sbf bbd block
		if ((usp + bbsize) <= lusp)
			memmove(usp, blockp, bbsize);
		usp += bbsize;
		pplen -= bbsize;
		bpos = bbdp[bpos];
	}
	assert((bpos == bl_endchain) && (pplen <= 0));

	// gather chains from small pps's
	usp = (unc *) sp->p();  // base of temp (semi-normalized) sbf
	for (ppsp = (unc *) rchp; ppsp < (unc *) ssp; ppsp += ppssize) {
		if (ppsp[0x42] != 2) // require stream, not storage or root
			continue;
		if (((pplen = *((long *) (ppsp + 0x78))) == 0)  // unused
		 || (pplen >= 0x1000)) // or long, already processed
			continue;
		bpos = *((long *) (ppsp + 0x74));  // starting block in sbf
		while ((bpos >= 0) && (pplen > 0)) {
			blockp = usp + (bpos * sbsize);  // pps small block
			if ((endp + sbsize) <= lastp)
				memmove(endp, blockp, sbsize);
			endp += sbsize;
			pplen -= sbsize;
			bpos = sbdp[bpos];
		}
		assert((bpos == bl_endchain) && (pplen <= 0));
	}

	// endp is now at end of area used for short pps data
	// clean up temp storages
	delete sp;   // done with small block file
	delete MIFvector;  // and original data
	MIFvector = NULL;

	// scan normalized data starting with big pps files
	unc *ucp = ssp;   // scan ptr

	uns *ecp = NULL;  // ending record ptr
	unl ulen = 0;     // WMF length in bytes

	unc *fcp = NULL;  // last WMF found
	unl flen = 0;     // length of last WMF found

	while (ucp < endp) {
		if ((*ucp == 1)  // metafile signature
		 && (*++ucp == 0)
		 && (*++ucp == 9)
		 && (*++ucp == 0)
		 && (*++ucp == 0)
		 && (*++ucp == 3)) {
			ulen = *((unl *) (ucp + 1)) * 2;
			if (((ucp - 5) + ulen) > endp)  // past end, invalid
				continue;
			ecp = (uns *) (((ucp - 5) + ulen) - 6); // check ending record
			if ((*ecp == 3)
			 && (*++ecp == 0)
			 && (*++ecp == 0)) {
				if (MultipleOLE
				 && (fcp != NULL)) {
					OLEimageCount++;
					DCblock *nb = new DCblock(flen);
					memmove((unc *) nb->p(), fcp, flen);
					OLEimages.add((long) nb);
				}
				fcp = ucp - 5;  // found, record pos and len
				flen = ulen;
			}
		}
		else if (*ucp != 1)
			ucp++;
	}

	if (fcp != NULL) {  // last valid WMF found, use it
		MIFvectype = wmf;
		MIFvector = new DCblock(flen);
		memmove((unc *) MIFvector->p(), fcp, flen);
		delete tp;
		return true;
	}

	delete tp;
	return false;
}


void
MIFrd::ProcOLEextraWMF(long ptr)
{
	if (ptr == 0)
		return;

	DCblock *bp = (DCblock *) ptr;
	SubNum++;
	ProcExport(NULL, "wmf", bp);
}


unl
MIFrd::VerifyWMFSection(unc *sp, unl len, unc *ep)
{
	// see if last three words is correct ending code
	uns *fp = (uns *) (sp + len - 6);
	if ((*fp == 3)
	 && (*++fp == 0)
	 && (*++fp == 0))
		return len;  // correct ending code found, good enough

	// uh-oh. we need to walk the records until we find the end
	// or else run out of data to look at (and return 0)
	unl ret = 0;
	unl *lp = NULL;
	for (fp = (uns *) (sp + 18); fp < (uns *) ep; fp += *lp) {
		lp = (unl *) fp;
		if ((*lp == 3)
		 && (*(fp + 2) == 0)) { // we have a winner
			ret = (unl) (((unc *) (fp + 3)) - sp);
			break;
		}
	}

	LogEvent(logwarn, 2, ret ? "Delayed" : "Missing", "WMF ending code");
	return ret;  // no ending record before end of data, bail
}


bool
MIFrd::ProcWMF(DCgroup* gp)
{
	DCctl *cp = NULL;
	unc* ucp = (unc*) MIFvector->p();
	long id = GetILong(ucp);

	if (id == 0x90001) { // prepare placeable metafile header
		DCblock *bp = new DCblock(22L + MIFvector->l());
		short *sp = (short *) bp->p();
		sp[0] = -12841;  //0xCDD7
		short check = sp[0];
		MakeIShort(&sp[0]);
		sp[1] = -25914;  //0x9AC6
		check ^= sp[1];
		MakeIShort(&sp[1]);
		sp[2] = 0;
		sp[3] = 0;
		sp[4] = 0;
		short twide = 0;
		short thigh = 0;
		switch (Upage) {  // convert current page units to twips
			case 1:  // inches
				twide = (short) ((ImpExt[2] * 72L) / 5000);
				thigh = (short) ((ImpExt[3] * 72L) / 5000);
				break;
			case 2:  // microns
				twide = (short) ((ImpExt[2] * 72L) / 1270);
				thigh = (short) ((ImpExt[3] * 72L) / 1270);
				break;
			case 3:  // pts * 100
				twide = (short) (ImpExt[2] / 5L);
				thigh = (short) (ImpExt[3] / 5L);
				break;
			case 4:  // Didot
				twide = (short) ((ImpExt[2] * 72L) / 336);
				thigh = (short) ((ImpExt[3] * 72L) / 336);
				break;
			default:  // unknown, should never happen
				LogEvent(logwarn, 2, "Unknown unit size in WMF");
				break;
		}
		sp[5] = twide;  // width in twips
		check ^= sp[5];
		MakeIShort(&sp[5]);
		sp[6] = thigh;  // height in twips
		check ^= sp[6];
		MakeIShort(&sp[6]);
		sp[7] = 1440;
		check ^= sp[7];
		MakeIShort(&sp[7]);
		sp[8] = 0;
		sp[9] = 0;
		sp[10] = check ^ 0;
		MakeIShort(&sp[10]);
		char *chp = ((char *) (bp->p())) + 22;
		memmove(chp, MIFvector->p(), MIFvector->l());
		delete MIFvector;
		MIFvector = bp;
	}
	else if (id != 0x9AC6CDD7) {
		assert(0);
		LogEvent(logwarn, 2, "Incorrect header ID in WMF");
		return false;
	}

	if (ExWmfFiles) {
		ProcExport(gp, "wmf", MIFvector);
		if (OLEimageCount) {  // additional images to export
			if (UseLog) {
				char cnt[3];
				unc idx = 0;
				if (OLEimageCount > 10)
					cnt[idx++] = (OLEimageCount / 10) + '0';
				cnt[idx++] = (OLEimageCount % 10) + '0';
				cnt[idx] = '\0';
				LogEvent(loginfo, 2, "Multiple WMF OLE images, ",
				 (OLEimageCount < 100) ? cnt : "over 99");
			}
			OLEimages.all(ProcOLEextraWMF);
			OLEimages.empty();
			OLEimageCount = 0;
		}

#if 0
		if (ImpFileC)
			delete ImpFileC;
	  ImpFileC = new DCctl(ename, 1, 50, 3);

		cp = new DCctl(ename, 0, 0, ExNum);
		cp->ex("wmf", 4);
		GroupList[psg_exfile]->add(*cp);

		cp = new DCctl(ilong, 0, 0, ExNum, (unl) ImpFileC);
		GroupList[psg_exfile]->add(*cp); // pointer to get name
		ProcExtern(gp);

		cp = new DCctl(ebyte, 0, 0, ExNum);
		*cp = *MIFvector;
		GroupList[psg_exfile]->add(*cp);
#endif

		MIFvector = NULL;
		return true;
	}

	ImpExt[4] = ImpExt[5] = 0;
	ImpExt[6] = TAngle;

	cp = new DCctl(elong, 4, 61, 0); // no pixel depth
	cp->ex(ImpExt, 7 * sizeof(long));
	gp->add(*cp);
	cp = new DCctl(ebyte, 4, 60, 9);
	*cp = *MIFvector;  // package entire WMF, clears MIFvector
	gp->add(*cp);

	MIFvector = NULL;
	return true;
}


bool
MIFrd::ProcDIB(DCgroup* gp)
{
	DCctl *cp = NULL;
  long *lp = (long *) (((char *) MIFraster->p()) + 2);
  char *chp = (char *) MIFraster->p();
  size_t isize = MIFraster->l();
  if ((chp[0] != 'B')
   || (chp[1] != 'M')
   || (chp[14] != 40)) { // start of BITMAPINFO header
		assert(0);
		LogEvent(logwarn, 2, "Incorrect header ID in DIB");
		return false;
	}

	if (ExBmpFiles) {
		ProcExport(gp, "bmp", MIFraster);

#if 0
		if (ImpFileC)
			delete ImpFileC;
	  ImpFileC = new DCctl(ename, 1, 50, 3);

		cp = new DCctl(ename, 0, 0, ExNum);
		cp->ex("bmp", 4);
		GroupList[psg_exfile]->add(*cp);

		cp = new DCctl(ilong, 0, 0, ExNum, (unl) ImpFileC);
		GroupList[psg_exfile]->add(*cp); // pointer to get name
		ProcExtern(gp);

		cp = new DCctl(ebyte, 0, 0, ExNum);
		*cp = *MIFraster;
		GroupList[psg_exfile]->add(*cp);
#endif

		MIFraster = NULL;
		return true;
	}

	ImpExt[4] = ImpILong(&lp[4]);
	ImpExt[5] = ImpILong(&lp[5]);
	if (BitMapDPI) { // rescale by pixel size
		long inchsize = 100000L; // only in America
		switch (Upage) {	// page units type
			case 2:	// microns
				inchsize = 25400L;
				break;
			case 3:	// pts
				inchsize = 7200L;
				break;
			case 4:	// didot
				inchsize = 6720L;
				break;
			default: // leave as inches
				break;
		}
		ImpExt[2] = (ImpExt[4] * inchsize) / BitMapDPI;
		ImpExt[3] = (ImpExt[5] * inchsize) / BitMapDPI;
		if (FlipLR)
			ImpExt[2] = -ImpExt[2];
	}
	ImpExt[6] = TAngle;
	uns idat = (uns) ((ImpILong(&lp[6]) >> 16) & 0xFF);
	uns comptype = (chp[30] == 0) ? 6 : ((chp[30] == 1) ? 8 : 7);
	cp = new DCctl(elong, 4, 61, idat);
	cp->ex(ImpExt, 7 * sizeof(long));
	gp->add(*cp);

	size_t msize = ImpILong(&lp[11]) * 4; // map size, 0 = full
	lp = &lp[13];
	isize -= 54;
	if (idat <= 8) { // colormap
		if (msize == 0)
			msize = (idat == 1) ? 8 : ((idat == 4) ? 64 : 1024);
		cp = new DCctl(ebyte, 4, 62, 2);
		cp->ex(lp, msize);
		gp->add(*cp);
		lp = (long *) (((char *) lp) + msize);
		isize -= msize;
	}
	if (BitMapDPI) {
		cp = new DCctl(ilong, 4, 63, 1, BitMapDPI);
		gp->add(*cp);
	}
	cp = new DCctl(ebyte, 4, 60, comptype);
	cp->ex(lp, isize);
	gp->add(*cp);

	MIFraster = NULL;
	return true;
}


bool
MIFrd::ProcSunRF(DCgroup* gp)
{
	DCctl *cp = NULL;
  long* lp = (long *) MIFraster->p();
  size_t isize = MIFraster->l();
  if (ImpMLong(lp) != 0x59A66A95) {
		assert(0);
		return false;
	}

	if (ExRfFiles) {
		ProcExport(gp, "rf", MIFraster);

#if 0
		if (ImpFileC)
			delete ImpFileC;
	  ImpFileC = new DCctl(ename, 1, 50, 3);

		cp = new DCctl(ename, 0, 0, ExNum);
		cp->ex("rf", 3);
		GroupList[psg_exfile]->add(*cp);

		cp = new DCctl(ilong, 0, 0, ExNum, (unl) ImpFileC);
		GroupList[psg_exfile]->add(*cp); // pointer to get name
		ProcExtern(gp);

		cp = new DCctl(ebyte, 0, 0, ExNum);
		*cp = *MIFraster;
		GroupList[psg_exfile]->add(*cp);
#endif

		MIFraster = NULL;
		return true;
	}

	ImpExt[4] = ImpMLong(&lp[1]);
	ImpExt[5] = ImpMLong(&lp[2]);
	ImpExt[6] = TAngle;
	uns idat = (uns) ImpMLong(&lp[3]);
	uns suntype = (ImpMLong(&lp[5]) == 2) ? 4 : 1; // 4 = compressed

	if (BitMapDPI) { // rescale by pixel size
		long inchsize = 100000L; // only in America
		switch (Upage) {	// page units type
			case 2:	// microns
				inchsize = 25400L;
				break;
			case 3:	// pts
				inchsize = 7200L;
				break;
			case 4:	// didot
				inchsize = 6720L;
				break;
			default: // leave as inches
				break;
		}
		ImpExt[2] = (ImpExt[4] * inchsize) / BitMapDPI;
		ImpExt[3] = (ImpExt[5] * inchsize) / BitMapDPI;
		if (FlipLR)
			ImpExt[2] = -ImpExt[2];
	}
	cp = new DCctl(elong, 4, 61, idat);
	cp->ex(ImpExt, 7 * sizeof(long));
	gp->add(*cp);

	if (lp[6]) { // colormap
		size_t msize = ImpMLong(&lp[7]);
		isize -= msize;
		cp = new DCctl(ebyte, 4, 62, 1);
		cp->ex(&lp[8], msize);
		gp->add(*cp);
		lp = (long *) (((char *) &lp[8]) + msize);
	}
	else lp = &lp[8];
	isize -= 32;

	if (BitMapDPI) {
		cp = new DCctl(ilong, 4, 63, 1, BitMapDPI);
		gp->add(*cp);
	}
	cp = new DCctl(ebyte, 4, 60, suntype);
	cp->ex(lp, isize);
	gp->add(*cp);

	MIFraster = NULL;
	return true;
}


bool
MIFrd::ProcEpsi(DCgroup* gp)
{
	DCctl *cp = NULL;
	char* chp;
	DCblock* bp;
	long* lp;

	if (ExEpsFiles) {
		cp = ProcExport(gp, "eps", NULL);

#if 0
		if (ImpFileC)
			delete ImpFileC;
	  ImpFileC = new DCctl(ename, 1, 50, 3);

		cp = new DCctl(ename, 0, 0, ExNum);
		cp->ex("eps", 4);
		GroupList[psg_exfile]->add(*cp);

		cp = new DCctl(ilong, 0, 0, ExNum, (unl) ImpFileC);
		GroupList[psg_exfile]->add(*cp); // pointer to get name
		ProcExtern(gp);

		cp = new DCctl(ebyte, 0, 0, ExNum);
		GroupList[psg_exfile]->add(*cp);
#endif

		if (MIFraster && (MIFrastype == tif) && EpsiPreview) {
			bp = new DCblock(32L + MIFepsi->l() + MIFraster->l());
			lp = (long*) bp->p();
			lp[0] = 0xC6D3D0C5;	// always Intel order
			MakeILong(&lp[0]);
			lp[1] = 32;
			MakeILong(&lp[1]);
			lp[2] = MIFepsi->l();
			MakeILong(&lp[2]);
			lp[3] = 0L;
			lp[4] = 0L;
			lp[5] = MIFepsi->l() + 32;
			MakeILong(&lp[5]);
			lp[6] = MIFraster->l();
			MakeILong(&lp[6]);
			lp[7] = 0xFFFF;
			MakeILong(&lp[7]);
			chp = (char *) &lp[8];
			memmove(chp, MIFepsi->p(), MIFepsi->l());
			chp += MIFepsi->l();
			memmove(chp, MIFraster->p(), MIFraster->l());
			*cp = *bp;
		}
		else if (MIFraster && EpsiPreview) {  // non-TIFF
			*cp = *MIFepsi;	 // complete the EPSI without preview
			MIFepsi = NULL;

			ImpFileC = new DCctl(ename, 1, 50, 3); // add raster file

			cp = new DCctl(ename, 0, 0, ExNum);	// using same number
			if (MIFrastype == rf)
				cp->ex("rf", 3);
			else if (MIFrastype == bmp)
				cp->ex("bmp", 4);
			else assert(0);
			GroupList[psg_exfile]->add(*cp);

			cp = new DCctl(ilong, 0, 0, ExNum, (unl) ImpFileC);
			GroupList[psg_exfile]->add(*cp); // pointer to get name
		  gp->add(*ImpFileC);	 // instead of calling ProcExtern
			ImpFileC = NULL;

			cp = new DCctl(ebyte, 0, 0, ExNum);
			GroupList[psg_exfile]->add(*cp);
			*cp = *MIFraster;
			MIFraster = NULL;
		}
		else if (MIFvector && (MIFvectype == wmf) && EpsiPreview) {
			bp = new DCblock(32L + MIFepsi->l() + MIFvector->l());
			lp = (long*) bp->p();
			lp[0] = 0xC6D3D0C5;	// always Intel order
			MakeILong(&lp[0]);
			lp[1] = 32;
			MakeILong(&lp[1]);
			lp[2] = MIFepsi->l();
			MakeILong(&lp[2]);
			lp[3] = MIFepsi->l() + 32;
			MakeILong(&lp[3]);
			lp[4] = MIFvector->l();
			MakeILong(&lp[4]);
			lp[5] = 0L;
			lp[6] = 0L;
			lp[7] = 0xFFFF;
			MakeILong(&lp[7]);
			chp = (char *) &lp[8];
			memmove(chp, MIFepsi->p(), MIFepsi->l());
			chp += MIFepsi->l();
			memmove(chp, MIFvector->p(), MIFvector->l());
			*cp = *bp;
		}
		else {
			*cp = *MIFepsi;
			MIFepsi = NULL;
		}
		return true;
	}

	gp->dat(5);  // correct type in group
	chp = (char *) MIFepsi->p();
	uns idat = 0;
	if (chp = strstr(chp, "BoundingBox:")) {
		chp += 12;
		for (uns i = 0; i < 4; i++)
			ImpExt[4 + i] = strtol(chp, &chp, 10);
		// determine kind of preview
		chp = (char *) MIFepsi->p();
		idat = strstr(chp, "BeginPreview:") ? 1 : 2;
	}
	else {
		ImpExt[4] = ImpExt[5] = ImpExt[6] = ImpExt[7] = 0;
		idat = 5; // non-EPS compliant
	}
	ImpExt[8] = TAngle;
	cp = new DCctl(elong, 4, 71, idat);
	cp->ex(ImpExt, 9 * sizeof(long));
	gp->add(*cp);

	cp = new DCctl(ecode, 4, 70, (uns) MIFlines);
	*cp = *MIFepsi;  // transfers block data to CTL
	gp->add(*cp);
	MIFepsi = NULL;

	if (MIFraster) {  // TIFF preview
		cp = new DCctl(ebyte, 4, 72, 4);
		*cp = *MIFraster;
		gp->add(*cp);
		MIFraster = NULL;
	}
	if (MIFvector) {  // WMF preview
		cp = new DCctl(ebyte, 4, 72, 4);
		*cp = *MIFvector;
		gp->add(*cp);
		MIFvector = NULL;
	}
	return true;
}


bool
MIFrd::ProcTiff(DCgroup* gp)
{
	DCctl *cp = NULL;
  unc* ncp = (unc *) MIFraster->p();
  size_t isize = MIFraster->l();
	if ((ncp[0] == 'M')
	 && (ncp[1] == 'M')) {     // Motorola, MSB first
		GetShort = GetMShort;
		GetLong = GetMLong;
	}
	else if ((ncp[0] == 'I')
	 && (ncp[1] == 'I')) {    // Intel, LSB first
		GetShort = GetIShort;
		GetLong = GetILong;
	}
	else {                    // who knows?
		GetShort = NULL;
		GetLong = NULL;
	}
	ncp += 2;
	if ((GetShort == NULL)    // validate TIFF header
	 || (GetShort(ncp) != 0x2A)) {
		assert(0);
		LogEvent(logwarn, 2, "Incorrect header ID in TIFF");
		return false;
	}

	if (ExTifFiles) {
		ProcExport(gp, "tif", MIFraster);

#if 0
		if (ImpFileC)
			delete ImpFileC;
	  ImpFileC = new DCctl(ename, 1, 50, 3);

		cp = new DCctl(ename, 0, 0, ExNum);
		cp->ex("tif", 4);
		GroupList[psg_exfile]->add(*cp);

		cp = new DCctl(ilong, 0, 0, ExNum, (unl) ImpFileC);
		GroupList[psg_exfile]->add(*cp); // pointer to get name
		ProcExtern(gp);

		cp = new DCctl(ebyte, 0, 0, ExNum);
		*cp = *MIFraster;
		GroupList[psg_exfile]->add(*cp);
#endif

		MIFraster = NULL;
		return true;
	}

	// look up pixel dimensions
	long lpos = GetLong(ncp);  // position of first index
	ncp += lpos - 8;
	short scount = GetShort(ncp); // count of 12-byte entries
	short scode;
	short stype;
	long len;
	uns idat = 1;  // default for TIFF
	for (;;) {
		if (scount == 0)
			break;
		scode = GetShort(ncp); 
		stype = GetShort(ncp);
		len = GetLong(ncp);
		switch (scode) {
			case 0x100:  // pixel width
				assert(len == 1);
				if (stype == 3) {
					ImpExt[4] = (long) GetShort(ncp);
					ncp += 2;
				}
				else if (stype == 4)
				  ImpExt[4] = GetLong(ncp);
				else assert(0);
				break;
			case 0x101:  // pixel height
				assert(len == 1);
				if (stype == 3) {
					ImpExt[5] = (long) GetShort(ncp);
					ncp += 2;
				}
				else if (stype == 4)
				  ImpExt[5] = GetLong(ncp);
				else assert(0);
				break;
			case 0x102:  // pixel bit depth
				if (len == 1) {
					if (stype == 3) {
						idat = (uns) GetShort(ncp);
						ncp += 2;
					}
					else if (stype == 4)
						idat = (uns) GetLong(ncp);
					else assert(0);
				}
				else if (len == 3)
					idat = 24;
				else assert(0);
				break;
			default:
				ncp += 4;
				break;
		}
		if (--scount == 0) { // end of last entry
			if ((lpos = GetLong(ncp)) == 0L)
				break; // last IFD
			ncp = ((unc *) MIFraster->p()) + lpos;
			scount = GetShort(ncp); // next IFD
		}
	}
	if ((ImpExt[4] == 0)
	 || (ImpExt[5] == 0)) {
		assert(0);
		return false;
	}

  if (BitMapDPI) { // rescale by pixel size
    long inchsize = 100000L; // inch units
		switch (Upage) {	// page units type
      case 2:	// microns
        inchsize = 25400L;
        break;
      case 3:	// pts
        inchsize = 7200L;
        break;
      case 4:	// didot
        inchsize = 6720L;
        break;
      default: // leave as inches
        break;
    }
    ImpExt[2] = (ImpExt[4] * inchsize) / BitMapDPI;
    ImpExt[3] = (ImpExt[5] * inchsize) / BitMapDPI;
    if (FlipLR)
      ImpExt[2] = -ImpExt[2];
  }
  ImpExt[6] = TAngle;

  cp = new DCctl(elong, 4, 61, idat);
  cp->ex(ImpExt, 7 * sizeof(long));
  gp->add(*cp);

	if (BitMapDPI) {
		cp = new DCctl(ilong, 4, 63, 1, BitMapDPI);
		gp->add(*cp);
	}
  cp = new DCctl(ebyte, 4, 60, 5);
  *cp = *MIFraster;  // package entire TIFF file, clears MIFraster
  gp->add(*cp);
	MIFraster = NULL;
	return true;
}


#ifdef VECINSET

bool
MIFrd::ProcFrVect(DCgroup* gp)
{
  gp->maj(3);  // correct majortype to graphic group
  gp->min(20);  // correct minortype to graphic group
  gp->dat(++FrameID);  // correct shortval to normal
  unc* cpt = (unc *) MIFvector->p();
  long lsize = MIFvector->l();
	unc opcode = 0;
	long dlen = 0;
	// process all the graphic records: 1 byte opcode, MLong datalen, data
	// final opcode is 0xFF with 0 datalen
	while ((*cpt != 0xFF) && (lsize > 0)) {
		opcode = *cpt++;
		dlen = GetMLong(cpt);
		lsize -= (dlen + 5); // safety net
		switch (opcode) {	 // proc the record
			case 0:
			default:	// ignore all others
				cpt += dlen; // skip any data
				break;
		}
	}
  delete MIFvector;
	MIFvector = NULL;
	return true;
}


bool
MIFrd::ProcPICT(DCgroup* gp)
{
	static bool FirstPict = true;

  gp->maj(3);  // correct majortype to graphic group
  gp->min(20);  // correct minortype to graphic group
  gp->dat(++FrameID);  // correct shortval to normal
  unc* cpt = (unc *) MIFvector->p();
  long lsize = MIFvector->l();
	short psize = GetMShort(cpt);
	assert(psize == (lsize & 0xFFFF));

	// read frame in 72-dpi pixels
	short left = GetMShort(cpt);
	short top = GetMShort(cpt);
	short right = GetMShort(cpt);
	short bottom = GetMShort(cpt);
	short high = bottom - top;
	short wide = right - left;

	// read first (version) record
	unc opcode = *cpt++;
	unc version = opcode ? 1 : 2;
	if (version == 2)
		opcode = *cpt++;
	if ((opcode != 0x11)
	 || (*cpt != version)) {
		assert(0);
		if (UseLog)
			Log->logEvent(logwarn, 2, "Incorrect header ID in PICT");
		return false;
	}

	cpt++;
	if (version == 2) {
		assert(*cpt == 0xFF);
		cpt++;
	}
	lsize -= (version == 2) ? 14 : 12;
	long dlen = 0;
	unc blen = 0;
	short slen = 0;

	if (FirstPict) {
		for (short i = 0; i < 256; i++)
			PictUsage[i] = 0;
		FirstPict = false;
	}
	unc lastop = 0;
	short lastlen = 0;

	// process all the records
	while (lsize > 0) {
		if (version == 2) {
			blen = *cpt++;
			lsize--;
		}
		lastop = opcode;
		opcode = *cpt++;
		lsize--;
		if (opcode == 0xFF) { // end code
			assert(lsize == 0);
			break;
		}

		if (blen) {	// ignore 0C00 and unassigned codes
			assert(blen == 0x0C);
			cpt += blen * 2;
			lsize -= blen * 2;
			blen = 0;
			continue;
		}
		// look up opcode length
		lastlen = slen;
		if (opcode > 0xCF) {
			slen = (short) GetMLong(cpt);
			cpt -= 4;
		}
		else if ((slen = PictSize[opcode]) < 0) {
			switch (slen) {
				case -1:
					slen = GetMShort(cpt) + 2;
					cpt -= 2;
					break;
				case -2:
					cpt += 2;
					slen = GetMShort(cpt) + 4;
					cpt -= 4;
					break;
				case -3:
					slen = (short) cpt[1] + 2;
					break;
				case -4:
					slen = (short) cpt[2] + 3;
					break;
				case -5:
					slen = (short) cpt[4] + 5;
					break;
				case -7:
					slen = GetMShort(cpt); // regions
					cpt -= 2;
					break;
				default: // compute for specific case
					break;
			}
		}
		if ((version == 2) && (slen & 1))
			slen++;

		PictUsage[opcode]++;

		switch (opcode) {
			case 0x12: // background pattern
			case 0x13: // pen pattern
			case 0x14: // fill pattern
			case 0x90: // bits rect
			case 0x91: // bits region
			case 0x98: // packbits rect
			case 0x99: // packbits region
				// must at least determine length to skip
				lsize = 0; // until then, bail out
				break;

			default:
				cpt += slen;
				lsize -= slen;
				break;
		}
	}
  delete MIFvector;
	MIFvector = NULL;
	return true;
}


short MIFrd::PictSize[208] = {
  0, -7,  8,  2,  1,  2,  4,  4,  2,  8,  8,	4,  4,  2,  4,  4,
  8,  1, -6, -6, -6,  2,  2,  0,  0,  0,  6,  6,  0,  6,  0,  6,
  8,  4,  6,  2, -1, -1, -1, -1, -5, -3, -3, -4, -1, -1, -1, -1,
  8,  8,  8,  8,  8,  8,  8,  8,  0,  0,  0,  0,  0,  0,  0,  0,

  8,  8,  8,  8,  8,  8,  8,  8,  0,  0,  0,  0,  0,  0,  0,  0,
  8,  8,  8,  8,  8,  8,  8,  8,  0,  0,  0,  0,  0,  0,  0,  0,
 12, 12, 12, 12, 12, 12, 12, 12,  4,  4,  4,  4,  4,  4,  4,  4,
 -1, -1, -1, -1, -1, -1, -1, -1,  0,  0,  0,  0,  0,  0,  0,  0,

 -7, -7, -7, -7, -7, -7, -7, -7,  0,  0,  0,  0,  0,  0,  0,  0,
 -6, -6, -1, -1, -1, -1, -1, -1, -6, -6, -1, -1, -1, -1, -1, -1,
  2, -2, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,

  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0
};

short MIFrd::PictUsage[256];

#endif

short
MIFrd::GetIShort(unc* &chp)
{
  short val = (((short) chp[1] << 8) & 0xFF00) | chp[0];
	chp += 2;
	return val;
}

short
MIFrd::GetMShort(unc* &chp)
{
  short val = (((short) chp[0] << 8) & 0xFF00) | chp[1];
	chp += 2;
	return val;
}

short (*MIFrd::GetShort)(unc*&);

long
MIFrd::GetILong(unc* &chp)
{
  long val = (( (long) chp[3] << 24) & 0xFF000000)
           | (( (long) chp[2] << 16) & 0x00FF0000)
           | (( (long) chp[1] << 8) & 0x0000FF00)
           | (chp[0] & 0xFF);
	chp += 4;
	return val;
}

long
MIFrd::GetMLong(unc* &chp)
{
  long val = (( (long) chp[0] << 24) & 0xFF000000)
           | (( (long) chp[1] << 16) & 0x00FF0000)
           | (( (long) chp[2] << 8) & 0x0000FF00)
           | (chp[3] & 0xFF);
	chp += 4;
	return val;
}

long (*MIFrd::GetLong)(unc*&);

long
MIFrd::ImpMLong(long *lp)
{
  unc *chp = (unc *) lp;
  long val = (( (long) chp[0] << 24) & 0xFF000000)
           | (( (long) chp[1] << 16) & 0x00FF0000)
           | (( (long) chp[2] << 8) & 0x0000FF00)
           | (chp[3] & 0xFF);
  return val;
}


long
MIFrd::ImpILong(long *lp)
{
  unc *chp = (unc *) lp;
  long val = (( (long) chp[3] << 24) & 0xFF000000)
           | (( (long) chp[2] << 16) & 0x00FF0000)
           | (( (long) chp[1] << 8) & 0x0000FF00)
           | (chp[0] & 0xFF);
  return val;
}


void
MIFrd::MakeILong(long *lp)
{
  unc *chp = (unc *) lp;
	long val = *lp;

  chp[0] = val & 0xFF;
  chp[1] = (val >> 8) & 0xFF;
  chp[2] = (val >> 16) & 0xFF;
  chp[3] = (val >> 24) & 0xFF;
}


void
MIFrd::MakeIShort(short *sp)
{
  unc *chp = (unc *) sp;
	short val = *sp;

  chp[0] = val & 0xFF;
  chp[1] = (val >> 8) & 0xFF;
}


int MIFrd::ExNameWid = 4;
int MIFrd::ExNumWid = 4;


void
MIFrd::WriteGrFiles(DCctl* cp, DCLwfile& wf)
{
	static short exnum = 0;
	static char* exsuf = NULL;
	static short snum = 0;
	DCctl* ncp = NULL;
  char* nptr = NULL;
	DCblock* bp = NULL;

	if ((cp->maj() != 0)
	 || (cp->min() != 0)) {
		assert(0);
		return;
	}

	switch (cp->dt()) {
		case ename:  
			exnum = cp->dat();         // number for file
			exsuf = (char*) cp->ex();  // suffix for file
			break;
		case ishort:
			snum = cp->dat();  // subnumber for extra OLE files
			break;
		case ilong:  // open file and writeback name
			assert(cp->dat() == exnum);
			wf.StartAux(exsuf, exnum, ExNameWid, ExNumWid, (long) snum); 
			if (snum == 0) {  // no writeback for extras
				ncp = (DCctl*) (cp->siz()); // pointer to name ctl
				nptr = wf.getAuxName();
				ncp->ex(nptr, strlen(nptr) + 1);
				LogEvent(loginfo, 2, "Writing graphic file ", nptr);
			}
			break;
		case ebyte:  // content of file
			assert(cp->dat() == exnum);
			if (!_strnicmp(exsuf, "pct", 3)) {
				bp = new DCblock(512);  // empty header
				wf = *bp;
				delete bp;
			}
			bp = new DCblock;
			*bp = *cp;
			wf = *bp;
			wf.EndAux();
			exnum = 0;
			exsuf = NULL;
			delete bp;
			break;
		case group:	 // put out once, at start of sequence
			break;
		default:     // blunder
			assert(0);
			break;
	}
}



// arrow style processing


long MIFrd::TipAngle;
long MIFrd::BaseAngle;
long MIFrd::ArLength;
long MIFrd::ArHead;

long MIFrd::ArrowStyle;

arrow MIFrd::StdArrows[StdArrowMax] = {
  { 30,  90,  600, 0 },
  { 30,  90,  800, 0 },
  { 16,  90, 1200, 0 },
  { 16,  70, 1200, 0 },
  { 16, 120, 1200, 0 },
  { 30,  90,  600, 2 },
  { 16,  90, 1200, 2 },
  { 16,  90, 1200, 1 }
};
uns MIFrd::ArrowUnit = pt;  // used for .len in StdArrows

DCclist MIFrd::CustomArrows;
long MIFrd::ArExt[5];

// these never seem to change in MIF, but are here for completeness
uns MIFrd::ArScale;  // always 1, for "Yes"
long MIFrd::ArFactor;  // always 0.25 pt
 

void
MIFrd::ProcArrow(mif *mp)
{
  TipAngle = BaseAngle = ArLength = ArHead = 0;
  EndProc[MIFlevel] = EndArrow;
}


void
MIFrd::EndArrow(void)
{
  int i;

  if ((Ugline != umax) && (Ugline != ArrowUnit)) { // reset std arrow vals
    mifunit tmp = MIFunit;
    MIFunit = (mifunit) ArrowUnit;
    for (i = 0; i < StdArrowMax; i++) {
      MIFval = StdArrows[i].len;
      ConvUnits(gline_u);
      StdArrows[i].len = MIFval;
    }
    MIFunit = tmp;
    ArrowUnit = Ugline;
  }

  for (i = 0; i < StdArrowMax; i++) {
    if ((StdArrows[i].tip == TipAngle)
     && (StdArrows[i].base == BaseAngle)
     && (StdArrows[i].len == ArLength)
     && (StdArrows[i].head == ArHead)) {
      ArrowStyle = i + 3;
      return;
    }
  }

  // check custom entries
  arrow *Arrow;
  for ( ; Arrow = (arrow *) CustomArrows.find(i); i++) {
    if ((Arrow->tip == TipAngle)
     && (Arrow->base == BaseAngle)
     && (Arrow->len == ArLength)
     && (Arrow->head == ArHead)) {
      ArrowStyle = i + 3;
      return;
    }
  }

  // make a custom entry if necessary
  Arrow = new arrow;
  Arrow->tip = TipAngle;
  Arrow->base = BaseAngle;
  Arrow->len = ArLength;
  Arrow->head = ArHead;

  // the horrible cast avoids creating an arrow list class for this one use
  CustomArrows.add( (DCctl *) Arrow, i);

  // add the new style to the doc resource list
  DCctl *cp = new DCctl(elong, 4, 84, i + 3);
  ArExt[0] = 3;  // FrameMaker definition type
  ArExt[1] = ArLength;
  ArExt[2] = ArHead + 1;
  ArExt[3] = TipAngle;
  ArExt[4] = BaseAngle;
  cp->ex(ArExt, 5 * sizeof(long));
  GroupList[psg_base]->add(*cp);

  ArrowStyle = i + 3;
}


 
// graphics common properties processing
 

uns MIFrd::CurrPen;
uns MIFrd::CurrPenWid;
uns MIFrd::CurrFill;
uns MIFrd::CurrColor;
long MIFrd::HeadCap;
long MIFrd::TailCap;

uns MIFrd::CurrDash = 1;
uns MIFrd::CurrLineSeg[LineSegsMax];
uns MIFrd::CurrLineSegCount = 0;
uns MIFrd::LineSegNum = 0;

void
MIFrd::EndDashProc(void)
{
	if (CurrLineSegCount == 0)
		CurrDash = 1; // solid
	else if (CurrLineSegCount > 4)
		CurrDash = 5; // dash-dot-dot
	else if (CurrLineSegCount > 2)
		CurrDash = 4; // dash-dot
	else if (CurrLineSeg[0] <= 400)  // <= 4 pts
		CurrDash = 3; // dot
	else
		CurrDash = 2; // dash
}


void
MIFrd::PutGProps(DCgroup *gp, uns gdat)
{
  DCctl *cp = NULL;

  // common for all graphic objects
  cp = new DCctl(imnem, 4, 20, 1, CurrDash);
  gp->add(*cp);
  cp = new DCctl(imnem, 4, 20, 2, CurrColor);
  gp->add(*cp);
  cp = new DCctl(imnem, 4, 20, 3, CurrPenWid);
  gp->add(*cp);

  // Pen uses fill styles
  cp = new DCctl(imnem, 4, 21, 1, CurrPen);
  gp->add(*cp);

  if (gdat == 32) {  // arc with reversed ends
    cp = new DCctl(imnem, 4, 24, 1, (TailCap == 3) ? ArrowStyle : TailCap);
    gp->add(*cp);
    cp = new DCctl(imnem, 4, 24, 2, (HeadCap == 3) ? ArrowStyle : HeadCap);
    gp->add(*cp);
  }
  else if ((gdat == 1) || (gdat == 30) || (gdat == 50)) {  // things with ends
    cp = new DCctl(imnem, 4, 24, 1, (HeadCap == 3) ? ArrowStyle : HeadCap);
    gp->add(*cp);
    cp = new DCctl(imnem, 4, 24, 2, (TailCap == 3) ? ArrowStyle : TailCap);
    gp->add(*cp);
  }
  else {  // things that can be filled
    cp = new DCctl(imnem, 4, 22, 1, CurrFill);
    gp->add(*cp);
    cp = new DCctl(imnem, 4, 22, 2, CurrColor);
		gp->add(*cp);
	}
}
 


// end of drmifgr.cc


