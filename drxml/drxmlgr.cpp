// drxmlgr.cpp is the code for dcl XML graphics processing, JHG
// Copyright (c) 2008 by Omni Systems, Inc.  All rights reserved.

#include "dcappl.h"
#include "dcctlg.h"
#include "dcfili.h"
#include "dcfilp.h"
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


// process fig, image, alt, and imagemaps


bool XMLrd::InFig = false;
bool XMLrd::InAlt = false;
bool XMLrd::InDesc = false;
char *XMLrd::TopicDesc = NULL;
bool XMLrd::TreatTableFigAsTable = true;
char *XMLrd::GraphHref = NULL;
bool XMLrd::GraphInline = true;
long XMLrd::GraphWide = 0;
long XMLrd::GraphHigh = 0;
long XMLrd::GraphDepth = 0;
long XMLrd::GraphTop = 0;
long XMLrd::GraphLeft = 0;
char *XMLrd::GraphAlt = NULL;
char *XMLrd::GraphDesc = NULL;
char *XMLrd::GraphicAlignment = NULL;
long XMLrd::GraphPos = 1;  // 1=left, 2=center, 3=right

graphinfo *XMLrd::CurrGraphInfo = NULL;
DCvlist XMLrd::Graphics;
long XMLrd::GraphNum = 0;

bool XMLrd::FigTitleAboveImage = false;
bool XMLrd::FigTitleAboveTable = true;
bool XMLrd::UseFigureAnchor = true;
char *XMLrd::FigureAnchorFormat = "FigureAnchor";
DCgroup *XMLrd::FigTextGroup = NULL;
DCgroup *XMLrd::FigTextIDGroup = NULL;
DCgroup *XMLrd::FigTitleIDGroup = NULL;


void XMLrd::ProcFigStart(void)
{
	InFig = true;
	if (FigTitleAboveImage
	 && FigTitleAboveTable) {
		FigTextGroup = NULL;
		FigTextIDGroup = NULL;
	}
	else {  // create anchor para
		FigTextGroup = new DCgroup;
		TextGroup->add(*FigTextGroup);
		FigTextIDGroup = new DCgroup;
		FigTitleIDGroup = new DCgroup;
	}
}


void XMLrd::ProcFigEnd(void)
{
	InFig = false;
	InFigTitle = false;
}


void XMLrd::ProcImageStart(void)
{
	// write out tag and attrs
	GraphInline = true;
	GraphWide = 0;
	GraphHigh = 0;
	GraphDepth = 0;
	GraphTop = 0;
	GraphLeft = 0;
	GraphAlt = NULL;
	GraphDesc = NULL;
	SetInlineElemTags();
	SetGraphAttrs();
	if (InFig)
		GraphInline = false;
	GraphDepth = CurrDepth;
}


void XMLrd::ProcImageEnd(void)
{
	AddImageFrame(GraphHref, GraphInline, GraphWide, GraphHigh);
}


void XMLrd::SetGraphAttrs(void)
{

	char *name = NULL;
	char *val = NULL;

	for (long i = 1; i <= CurrElemInfo->attrcnt; i++) {
		name = CurrElemInfo->attrnames->find(i);
		val = CurrElemInfo->attrvals->find(i);

		if (!strcmp(name, "placement"))  // always set by parser
			GraphInline = (!strcmp(val, "break")) ? false : true;
		else if (!strcmp(name, "width"))
			GraphWide = GetNumericAttr(val);
		else if (!strcmp(name, "height"))
			GraphHigh = GetNumericAttr(val);
		else if (!strcmp(name, "depth"))
			GraphDepth = GetNumericAttr(val);
		else if (!strcmp(name, "top"))
			GraphTop = GetNumericAttr(val);
		else if (!strcmp(name, "left"))
			GraphLeft = GetNumericAttr(val);
		else if (!strcmp(name, "alt"))
			GraphAlt = val;
	}
}



// Image maps

bool XMLrd::InImageMap = false;
bool XMLrd::StartingImageMap = false;
bool XMLrd::InImageArea = false;
bool XMLrd::InImageShape = false;
bool XMLrd::InImageCoords = false;
bool XMLrd::InImageXref = false;

imapinfo *XMLrd::CurrImageMap = NULL;
imaparea *XMLrd::CurrImageMapArea = NULL;

DClist XMLrd::ImageMaps;

uns XMLrd::ImageMapCount = 0;
uns XMLrd::ImageAreaCount = 0;

DCnlist XMLrd::ImageMapShapes;
npair XMLrd::ImageMapShapesInit[] = {
 { imrect, "rect" },
 { impoly, "poly" },
 { imcirc, "circle" },
 { imdefault, "default" },
 NULL
};


void
XMLrd::StartImageMap(void)
{
	InImageMap = StartingImageMap = true;
	CurrImageMap = new imapinfo;
	ImageMaps.add((long) CurrImageMap);
	CurrImageMap->grmap = new DCgroup(DCctl(group, 4, 95, ++ImageMapCount));
}


void
XMLrd::EndImageMap(void)
{
	InImageMap = false;
}


void
XMLrd::StartImageArea(const char *shapeatt, const char *coordsatt)
{
	if (StartingImageMap) {
		CurrImageMap->graph = CurrGraphInfo;
		CurrGraphInfo->imagemap = CurrImageMap;
		CurrGraphInfo->graphframe->add(*(CurrImageMap->grmap));
		CurrImageMap->grframe = CurrGraphInfo->graphframe;
		CurrImageMap->name = CurrGraphInfo->srcbase;
		DCctl *cp = new DCctl(ename, 4, 96, 1);
		cp->ex(CurrImageMap->name);
		CurrImageMap->grmap->add(*cp);
		StartingImageMap = false;
	}

	InImageArea = true;
	CurrImageMapArea = new imaparea;
	CurrImageMapArea->imap = CurrImageMap;
	CurrImageMap->areas.add((long) CurrImageMapArea);

	DCgroup *gp = new DCgroup(DCctl(group, 4, 97, ++ImageAreaCount));
	CurrImageMapArea->grarea = gp;
	CurrImageMap->grmap->add(*gp);

	InImageShape = InImageCoords = InImageXref = false;

	//if (UMXL) {
#ifdef UDOC
		ProcImageAreaShape(NewName(shapeatt));
		ProcImageAreaCoords(NewName(coordsatt));

		if (CurrImageMapArea->ccnt == 0)
			LogEvent(logwarn, 1, "Area coords are missing");
		else if (CurrImageMapArea->ccnt == 3) {
			if (CurrImageMapArea->shape != imcirc)
				LogEvent(logwarn, 1, "Area shape should be circle but is ",
							               CurrImageMapArea->shname);
		}
		else if (CurrImageMapArea->ccnt == 4) {
			if (CurrImageMapArea->shape != imrect)
				LogEvent(logwarn, 1, "Area shape should be rect but is ",
							               CurrImageMapArea->shname);
		}
		else if ((CurrImageMapArea->ccnt & 1) == 1) {
			LogEvent(logwarn, 1, "Area coord count is odd, should be even");
		}
		else if (CurrImageMapArea->ccnt > 4) {
			if (CurrImageMapArea->shape != impoly)
				LogEvent(logwarn, 1, "Area shape should be poly but is ",
							               CurrImageMapArea->shname);
		}
#endif
	//}
}


void 
XMLrd::ProcImageAreaShape(char *shname)
{
	CurrImageMapArea->shname = shname;
	CurrImageMapArea->shape =  (imshape) ImageMapShapes(shname);
}


void 
XMLrd::ProcImageAreaCoords(char *coords)
{
	CurrImageMapArea->coords = coords;
	CurrImageMapArea->ccnt = 0;

	if (!coords)
		return;

	char *cptr = coords;
	long vals = 1;

	while (*cptr) {
		if (*cptr == ',')
			vals++;
		cptr++;
	}
	CurrImageMapArea->ccnt = vals;

	cptr = coords;
	long *ldat = new long[vals];
	CurrImageMapArea->carray = ldat;

	for (long i = 0; i < vals; i++) {
		ldat[i] = strtol(cptr, &cptr, 10);
		while (*cptr && ((*cptr == ',') || (*cptr == ' ')))
			cptr++;
	}

	InImageCoords = false;
}


void 
XMLrd::ProcImageAreaXref(char *href, const char *format, const char *scope)
{
	CurrImageMapArea->href = href;
	CurrImageMapArea->form = format ? (formtyp) LinkForms(format) : ftnone;
	CurrImageMapArea->scope = scope ? (scopetyp) LinkScopes(scope) : scnone;
	if (CurrImageMapArea->scope == scnone) {
		if (CurrImageMapArea->form == ftdita)
			CurrImageMapArea->scope = sclocal;
		else if (CurrImageMapArea->form != ftnone)
			CurrImageMapArea->scope = scexternal;
	}

	DCgroup *lasttext = TextGroup;
	TextGroup = CurrImageMapArea->grarea;
	CurrLinkInfo = new linkinfo;
	CurrLinkInfo->href = href;
	SetHyperlink(CurrLinkInfo, CurrImageMapArea->scope);
	Links.add(CurrLinkInfo, ++LinkCount);
	TextGroup = lasttext;
}


void
XMLrd::EndImageArea(void)
{
	char nbuf[NumLenMax];

	DCgroup *gp = CurrImageMapArea->grarea;
	DCctl *cp = NULL;

	if (CurrImageMapArea->alt && *(CurrImageMapArea->alt)) {
		cp = new DCctl(etext, 4, 98, 2);         // alt text
		cp->ex(CurrImageMapArea->alt, strlen(CurrImageMapArea->alt));
		gp->add(*cp);
	}

	long *ldat = NULL;
	long ccnt = CurrImageMapArea->ccnt;

	cp = new DCctl(elong, 4, 99, (uns) CurrImageMapArea->shape);

	switch (CurrImageMapArea->shape) {
		case imrect:  // xypos top left, bottom right
			ldat = CurrImageMapArea->carray;
			if (ccnt != 4)
				LogEvent(logwarn, 1, "Incorrect count of imagemap rect values: ",
				 ltoa(ccnt, nbuf, 10), " for ", CurrImageMapArea->imap->name);
			cp->ex(ldat, sizeof(long) * ccnt);
			gp->add(*cp);
			break;

		case impoly:  // xypos for each vertex, last == first
			ldat = CurrImageMapArea->carray;
			if ((ccnt % 2) != 0)
				LogEvent(logwarn, 1, "Incorrect count of imagemap poly values: ",
				 ltoa(ccnt, nbuf, 10), " for ", CurrImageMapArea->imap->name);
			cp->ex(ldat, sizeof(long) * ccnt);
			gp->add(*cp);
			break;

		case imcirc:  // xypos center, radius
			ldat = CurrImageMapArea->carray;
			if (ccnt != 3)
				LogEvent(logwarn, 1, "Incorrect count of imagemap circle values: ",
				 ltoa(ccnt, nbuf, 10), " for ", CurrImageMapArea->imap->name);
			cp->ex(ldat, sizeof(long) * ccnt);
			gp->add(*cp);
			break;

		case imdefault:  // otherwise
			if (ccnt > 0)
				LogEvent(logwarn, 1, "Incorrect count of imagemap default values: ",
				 ltoa(ccnt, nbuf, 10), " for ", CurrImageMapArea->imap->name);
			gp->add(*cp);
			break;

		default:
			break;
	}

	InImageArea = InImageShape = InImageCoords = InImageXref = false;
}



// Image frames

// inl is true if placement=inline, false if placement=break
// calc height and width in .01pt units for DCL using ConvertVal
// real nums, units in DITA are px, in, pc, pt, cm, mm, em

void
XMLrd::AddImageFrame(char *href, bool inl, long width, long height)
{
	CurrGraphInfo = new graphinfo;
	Graphics.add(CurrGraphInfo, ++GraphNum);
	CurrGraphInfo->graphnum = GraphNum;
	CurrGraphInfo->replaced = false;
	if (!CurrPageInfo->graphics)
		CurrPageInfo->graphics = new DClist;
	CurrPageInfo->graphics->add(GraphNum);

	DCgroup *gp = new DCgroup(DCctl(group, 3, 20, ++FrameNum));
	CurrGraphInfo->graphframe = gp;
	CurrGraphInfo->framenum = FrameNum;
	CurrGraphInfo->pagenum = PageNum;

	DCtopelem *topelem = (DCtopelem *) CurrElemInfo;
	topelem->graphnum = GraphNum;
	topelem->elemgraph = gp;
	CurrGraphInfo->einfo = topelem;

	FramePageList.add(PageNum, FrameNum);

	DCctl *cp = new DCctl(ilong, 2, 112, 2, ++ItemID);  // loc
	gp->add(*cp);

	if (GraphAlt) {  // layout props frame
		DCgroup *gprop = new DCgroup(DCctl(group, 3, 35, 1));
		gp->add(*gprop);

		cp = new DCctl(ename, 3, 36, 1);  // name
		cp->ex("GraphAlt");
		gprop->add(*cp);

		cp = new DCctl(ename, 3, 36, 2);  // val
		cp->ex(GraphAlt);
		gprop->add(*cp);
	}

	if (GraphDesc) {  // layout props frame
		DCgroup *gprop = new DCgroup(DCctl(group, 3, 35, 1));
		gp->add(*gprop);

		cp = new DCctl(ename, 3, 36, 1);  // name
		cp->ex("ActualText");
		gprop->add(*cp);

		cp = new DCctl(ename, 3, 36, 2);  // val
		cp->ex(GraphDesc);
		gprop->add(*cp);
	}
	if (GraphTop) {  // layout props frame
		DCgroup *gprop = new DCgroup(DCctl(group, 3, 35, 1));
		gp->add(*gprop);

		cp = new DCctl(ename, 3, 36, 1);  // name
		cp->ex("Graphstyle");
		gprop->add(*cp);

		cp = new DCctl(ename, 3, 36, 2);  // val
		gprop->add(*cp);

		char *gtop = NewName("vertical-align: ", GraphPosMax);
		char *gval = NewName(NumLenMax);
		ltoa(-GraphTop/100, gval, 10);
		strcat(gtop, gval);
		strcat(gtop, "pt;");
		cp->ex(gtop);
	}

	cp = new DCctl(imnem, 3, 23, TextNum, PageNum); // anchor
	CurrGraphInfo->granchor = cp;
	gp->add(*cp);
	
	// add frame positioning
	cp = new DCctl(imnem, 3, 21, 9, inl ? 1 : 2); // inline or break
	gp->add(*cp);

	if (!inl) {
		cp = new DCctl(imnem, 3, 21, 12, 3);  // within column
		gp->add(*cp);

		cp = new DCctl(imnem, 3, 21, 6, GraphPos);  // horizontal pos
		gp->add(*cp);
	}

  // graph obj external
	DCgroup *grobj = new DCgroup(DCctl(group, 4, 10, 6));
	gp->add(*grobj);

	// add graphic size
	rect gpos;
	gpos.l = gpos.t = 0;  // xypos not known
	gpos.l = GraphLeft;
	gpos.t = GraphTop;
	gpos.h = height;
	gpos.w = width;
	cp = new DCctl(elong, 3, 22, 1); // at anchor, xypos whsize
	cp->ex((void *) &gpos, sizeof(rect));
	grobj->add(*cp);

	cp = new DCctl(ename, 1, 50, 3);  // dcl include graphic
	cp->ex(href);
	grobj->add(*cp);

	CurrGraphInfo->srcpath = href;
	char buf[PATHMAX];
	GetBaseName(href, NULL, buf);
	if (*buf)
		CurrGraphInfo->srcbase = NewName(buf);

	// reference in text flow
	DCgroup *lasttext = TextGroup;
	if (!inl) {
		if (InFig
		 && !FigTitleAboveImage
		 && FigTextGroup)
			TextGroup = FigTextGroup;
		if (CurrTaskHeadType)
			SetStepsPara();
		if (UseFigureAnchor) {
			StartPara();
			SetFormat(FigureAnchorFormat);
		}
		if (InFig) {
			if (!FigTitleAboveImage) {
				if (FigTextGroup
				 && FigTextIDGroup)
				FigTextGroup->add(*FigTextIDGroup);
			}
			else {
				if (FigTitleIDGroup
				 && FigTextIDGroup)
				FigTitleIDGroup->add(*FigTextIDGroup);
			}
		}
	}
	SetFlags(GraphDepth);
	cp = new DCctl(imnem, 2, 5, FrameNum, PageNum); // anchor in text
	CurrGraphInfo->txanchor = cp;
	TextGroup->add(*cp);
	SetFlags(GraphDepth - 1);
	TextGroup = lasttext;
}


long 
XMLrd::SetGraphicFrames(DCpage *pg, long pgseq)
{
	// renumber frames and reset text anchor numbers
	// add graphics to pagegroup before pg->pagetext
	long cnt = 0;
	graphinfo *gr = NULL;
	uns startframe = (uns) pg->firstframe;

	if (!pg
	 || !pg->graphics
	 || ((cnt = pg->graphics->count()) == 0))
		return startframe;

	for (long i = 1; i <= cnt; i++) {
		gr = (graphinfo *) Graphics.find(pg->graphics->number(i));
		if (!gr
		 || gr->replaced)   // if replaced by conref, skip
			continue;

		gr->graphframe->dat(++startframe);
		gr->granchor->dat(pg->textnum);
		gr->granchor->siz(pgseq);
		gr->txanchor->dat(startframe);
		gr->txanchor->siz(pgseq);

		pg->pgroup->insert(*((DCctl *) gr->graphframe), *(pg->pagetext));
	}

	return startframe;
}


void 
XMLrd::RemoveGraphic(DCtopic *toporig, DCtopelem *elorig)
{
	graphinfo *ginfo = NULL;
	long gorig = elorig ? elorig->graphnum : 0;

	if (gorig
	 && ((ginfo = (graphinfo *) Graphics.find(gorig)) != NULL))
		ginfo->replaced = true;
}


void 
XMLrd::ReplaceGraphic(DCtopic *toporig, DCtopelem *elorig, DCtopelem *elnew)
{
	long gorig = elorig->graphnum;
	long gnew = elnew->graphnum = 0;

	if (!gorig)
		return;   // no graphic in this elem

	graphinfo *ginfo = (graphinfo *) Graphics.find(gorig);
	if (!ginfo)
		return;  // nothing to replace

	// copy graph group and insert on original page
	DCpage *pg = toporig->page;
	long pgn = pg->pagenum;
	graphinfo *ngr = new graphinfo;
	memcpy(ngr, ginfo, sizeof(graphinfo));

	Graphics.add(ngr, ++GraphNum);
	ngr->graphnum = GraphNum;
	ngr->replaced = false;
	if (!pg->graphics)
		pg->graphics = new DClist;
	pg->graphics->add(GraphNum);

	DCgroup *gp = new DCgroup(*(ngr->graphframe));
	gp->siz(++FrameNum);
	ngr->graphframe = gp;
	ngr->framenum = FrameNum;
	ngr->pagenum = pgn;

	elnew->graphnum = GraphNum;
	elnew->elemgraph = gp;
	ngr->einfo = elnew;

	FramePageList.add(pgn, FrameNum);

	DCctl *cp = new DCctl(ilong, 2, 112, 2, ++ItemID);  // loc
	DCctl *ocp = gp->first->ctl;
	bool val = gp->replace(*cp, *ocp);

	// correct text and graphic anchors
	ocp = ngr->granchor;
	cp = new DCctl(imnem, 3, 23, pg->textnum, pgn); // anchor
	ngr->granchor = cp;
	val = gp->replace(*cp, *ocp);

	ocp = ngr->txanchor;
	cp = new DCctl(imnem, 2, 5, FrameNum, pgn); // anchor in text
	ngr->txanchor = cp;
	val = elnew->elemtext->replace(*cp, *ocp);
}



// object processing


bool XMLrd::InObject = false;
bool XMLrd::InObjectDesc = false;
DCgroup *XMLrd::ObjectGroup = NULL;
uns XMLrd::ObjectNum = 0;
char *XMLrd::ObjectDesc = NULL;
long XMLrd::ObjectDepth = 0;

//<object classid="clsid:D27CDB6E-AE6D-11cf-96B8-444553540000"
//  codebase="http://download.macromedia.com/pub/shockwave/cabs/
//   flash/swflash.cab#version=6,0,0,0"
//  data="cutkey370.swf"
//  type="application/x-shockwave-flash"
//  height="280"
//  width="370"
//  id="cutkey370">
// <desc>A description of the task</desc>
// <param name="movie" value="cutkey370.swf"/>
// <param name="quality" value="high"/>
// <param name="bgcolor" value="#FFFFFF"/>
//</object>


void 
XMLrd::StartObject(const char **atts)
{
	if (InObject)
		EndObject();

	if (ParaStarted)
		EndPara();

	const char *data = NULL;
	const char *classid = NULL;
	const char *type = NULL;
	const char *codebase = NULL;
	const char *codetype = NULL;
	const char *id = NULL;
	const char *height = NULL;
	const char *width = NULL;
	const char *declare = NULL;
	const char *standby = NULL;
	const char *archive = NULL;
	const char *usemap = NULL;
	const char *name = NULL;

	const char *tabindex = NULL;
	const char *longdescref = NULL;

	attrp alist[] = {
	 {"data", &data },
	 {"classid", &classid },
	 {"type", &type },
	 {"codebase", &codebase },
	 {"codetype", &codetype },
	 {"id", &id },
	 {"height", &height },
	 {"width", &width },
	 {"declare", &declare },
	 {"standby", &standby },
	 {"archive", &archive },
	 {"usemap", &usemap },
	 {"name", &name },
	 {"tabindex", &tabindex },
	 {"longdescref", &longdescref },
	 {NULL, NULL }
	};

	ProcAttrs(atts, alist);

	SetFlags(CurrDepth);
	ObjectGroup = new DCgroup(DCctl(group, 4, 90, ++ObjectNum));
	TextGroup->add(*ObjectGroup);
	SetFlags(CurrDepth - 1);

	DCctl *cp = NULL;

	if (data) {
		cp = new DCctl(etext, 4, 91, 1);
		char *ndata = FixReference(data);
		cp->ex(ndata, strlen(ndata));
		ObjectGroup->add(*cp);
	}
	if (classid) {
		cp = new DCctl(etext, 4, 91, 2);
		cp->ex(classid, strlen(classid));
		ObjectGroup->add(*cp);
	}
	if (type) {
		cp = new DCctl(etext, 4, 91, 3);
		cp->ex(type, strlen(type));
		ObjectGroup->add(*cp);
	}
	if (codebase) {
		cp = new DCctl(etext, 4, 91, 4);
		cp->ex(codebase, strlen(codebase));
		ObjectGroup->add(*cp);
	}
	if (codetype) {
		cp = new DCctl(etext, 4, 91, 5);
		cp->ex(codetype, strlen(codetype));
		ObjectGroup->add(*cp);
	}
	if (id) {
		cp = new DCctl(etext, 4, 91, 6);
		cp->ex(id, strlen(id));
		ObjectGroup->add(*cp);
	}
	if (height) {
		cp = new DCctl(ilong, 4, 91, 8, GetNumericAttr(height));
		ObjectGroup->add(*cp);
	}
	if (width) {
		cp = new DCctl(ilong, 4, 91, 9, GetNumericAttr(width));
		ObjectGroup->add(*cp);
	}
	if (declare) {
		cp = new DCctl(imnem, 4, 91, 13, 1);
		ObjectGroup->add(*cp);
	}
	if (standby) {
		cp = new DCctl(etext, 4, 91, 14);
		cp->ex(standby, strlen(standby));
		ObjectGroup->add(*cp);
	}
	if (archive) {
		cp = new DCctl(etext, 4, 91, 15);
		cp->ex(archive, strlen(archive));
		ObjectGroup->add(*cp);
	}
	if (usemap) {
		cp = new DCctl(etext, 4, 91, 16);
		cp->ex(usemap, strlen(usemap));
		ObjectGroup->add(*cp);
	}
	if (name) {
		cp = new DCctl(etext, 4, 91, 17);
		cp->ex(name, strlen(name));
		ObjectGroup->add(*cp);
	}
	if (tabindex) {
		cp = new DCctl(ilong, 4, 91, 18, atol(tabindex));
		ObjectGroup->add(*cp);
	}
	if (longdescref) {
		cp = new DCctl(etext, 4, 91, 19);
		cp->ex(longdescref, strlen(longdescref));
		ObjectGroup->add(*cp);
	}

	InObject = true;
}


void 
XMLrd::SetObjectParam(const char **atts)
{
	if (!ObjectGroup)
		return;

	const char *name = NULL;
	const char *value = NULL;
	const char *valuetype = NULL;
	const char *type = NULL;

	attrp alist[] = {
	 {"name", &name },
	 {"value", &value },
	 {"valuetype", &valuetype },
	 {"type", &type },
	 {NULL, NULL }
	};

	ProcAttrs(atts, alist);

	DCgroup *gp = new DCgroup(DCctl(group, 4, 92, 1));
	ObjectGroup->add(*gp);

	DCctl *cp = new DCctl(etext, 4, 92, 2);
	cp->ex(name, strlen(name));
	gp->add(*cp);

	long vt = 0;
	if (valuetype) {
	if (!strcmp(valuetype, "data"))
		vt = 1;
	else if (!strcmp(valuetype, "ref"))
		vt = 2;
	else if (!strcmp(valuetype, "object"))
		vt = 3;
	}
	else
		vt = 1;

	if (vt) {
		cp = new DCctl(imnem, 4, 92, 4, vt - 1);
		gp->add(*cp);

		if (vt == 2) {
			cp = new DCctl(etext, 4, 92, 5);
			cp->ex(type, strlen(type));
			gp->add(*cp);
		}
	}

	cp = new DCctl(etext, 4, 92, 3);
	const char *nval = (vt == 2) ? FixReference(value) : value;
	cp->ex(nval, strlen(nval));
	gp->add(*cp);
}


void 
XMLrd::EndObject(void)
{
	if (!InObject)
		return;

	DCctl *cp = NULL;

	// collect string from desc elem

	if (ObjectDesc) {
		cp = new DCctl(etext, 4, 91, 20);
		cp->ex(ObjectDesc, strlen(ObjectDesc));
		ObjectGroup->add(*cp);
	}

	ObjectGroup = NULL;
	ObjectDesc = NULL;
	InObject = false;
	InObjectDesc = false;
}



// end of drxmlgr.cpp

