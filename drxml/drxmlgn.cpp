// drxmlgn.cpp is the code for list file generation, JHG
// Copyright (c) 2008 by Omni Systems, Inc.  All rights reserved.

#include "dcappl.h"
#include "dcctlg.h"
#include "dcfili.h"
#include "dctxfx.h"
#include "dcfmtr.h"
#include "dcini.h"
#include "dcindx.h"
#include "dcelem.h"
#include "drxml.h"
#ifdef DITA
#include "expat.h"
#endif
#include "mxlparser.h"
#include "drxmlud.h"
#include "drxmlfl.h"


DCvlist XMLrd::GenLists;
DCnlist XMLrd::GenListNames;
npair XMLrd::InitGenListNames[] = {
	{ gentoc, "toc"},
	{ genidx, "indexlist"},
	{ genlof, "figurelist"},
	{ genlot, "tablelist"},
	{ gengloss, "glossarylist"},
	{ genbib, "bibliolist"},
	{ genabbr, "abbrevlist"},
	{ gentmk, "trademarklist"},
	{ genamend, "amendments"},
	{ genlist, "booklist"},
	NULL
};
npair XMLrd::InitUMXLGenListNames[] = {
	{ gentoc, "contents"},
	{ genidx, "index"},
	{ genlof, "figures"},
	{ genlot, "tables"},
	{ gengloss, "glossary"},
	{ genbib, "bibliography"},
	{ genabbr, "abbreviations"},
	{ gentmk, "trademarks"},
	{ genamend, "amendments"},
	{ genlist, "doclist"},
	NULL
};

char *XMLrd::TitlePageFile = NULL;
char *XMLrd::TitlePageTemplate = NULL;

bool XMLrd::InGenerateFile = false;
bool XMLrd::GenerateTOC = true;
char *XMLrd::TOCFile = NULL;
char *XMLrd::TOCSuffix = "TOC";
char *XMLrd::TOCFormat = "TOCItem";
char *XMLrd::TOCTitle = "Table of Contents";
char *XMLrd::TOCTitleFormat = "ContentsTitle";
char *XMLrd::TOCXrefFormat = "TOCXref";
char *XMLrd::TOCLinkFormat = "TOCLink";
DCgroup *XMLrd::TOCTextGroup = NULL;
bool XMLrd::InTOCTOC = false;
bool XMLrd::KeepTOCWithTitlePage = false;
DCgroup *XMLrd::PreTOCTOCGroup = NULL;
DCgroup *XMLrd::TOCTOCGroup = NULL;
DCgroup *XMLrd::PreTOCTOCIgnore = NULL;
DCgroup *XMLrd::TOCTOCIgnore = NULL;
DCgroup *XMLrd::TOCWrapper = NULL;
bool XMLrd::UseContents = true;

bool XMLrd::GenerateLOF = false;
char *XMLrd::LOFFile = NULL;
char *XMLrd::LOFSuffix = "LOF";
char *XMLrd::LOFFormat = "FigureListItem";
char *XMLrd::LOFTitle = "List of Figures";
char *XMLrd::LOFTOCFormat = "LOF";
char *XMLrd::LOFTitleFormat = "FigureListTitle";
char *XMLrd::LOFXrefFormat = "FigureTitleXref";
DCgroup *XMLrd::LOFTextGroup = NULL;

bool XMLrd::GenerateLOT = false;
char *XMLrd::LOTFile = NULL;
char *XMLrd::LOTSuffix = "LOT";
char *XMLrd::LOTFormat = "TableListItem";
char *XMLrd::LOTTitle = "List of Tables";
char *XMLrd::LOTTOCFormat = "LOT";
char *XMLrd::LOTTitleFormat = "TableListTitle";
char *XMLrd::LOTXrefFormat = "TableTitleXref";
DCgroup *XMLrd::LOTTextGroup = NULL;

bool XMLrd::GenerateIDX = false;
bool XMLrd::MakeDefaultIDX = true;
char *XMLrd::IDXFile = NULL;
char *XMLrd::IDXSuffix = "IDX";
char *XMLrd::IDXFormat = "IndexItem";
char *XMLrd::IDXTitle = "Index";
char *XMLrd::IDXTOCFormat = "IndexTOC";
char *XMLrd::IDXTitleFormat = "IndexTitle";
char *XMLrd::IDXXrefFormat = "IndexXref";
DCgroup *XMLrd::IDXTextGroup = NULL;
bool XMLrd::IndexInTOC = false;

bool XMLrd::GenerateGLS = false;
char *XMLrd::GLSFile = NULL;
char *XMLrd::GLSSuffix = "GLS";
char *XMLrd::GLSFormat = "GlossaryItem";
char *XMLrd::GLSTitle = "Glossary";
char *XMLrd::GLSTOCFormat = "GLS";
char *XMLrd::GLSTitleFormat = "GlossaryTitle";
char *XMLrd::GLSXrefFormat = "GlossaryXref";
DCgroup *XMLrd::GLSTextGroup = NULL;
bool XMLrd::InGlossEntry = false;
bool XMLrd::InGlossTerm = false;
bool XMLrd::InGlossAbbr = false;
char *XMLrd::GlossaryFileName = NULL;

bool XMLrd::GenerateABR = false;
char *XMLrd::ABRFile = NULL;
char *XMLrd::ABRSuffix = "ABR";
char *XMLrd::ABRFormat = "AbbrevItem";
char *XMLrd::ABRTitle = "Abbreviations";
char *XMLrd::ABRTOCFormat = "ABR";
char *XMLrd::ABRTitleFormat = "AbbrevTitle";
char *XMLrd::ABRXrefFormat = "AbbrevXref";
DCgroup *XMLrd::ABRTextGroup = NULL;
char *XMLrd::AbbrFileName = NULL;

bool XMLrd::GenerateTMK = false;
char *XMLrd::TMKFile = NULL;
char *XMLrd::TMKSuffix = "TMK";
char *XMLrd::TMKFormat = "TmarkItem";
char *XMLrd::TMKTitle = "Trademarks";
char *XMLrd::TMKTOCFormat = "TMK";
char *XMLrd::TMKTitleFormat = "TmarkTitle";
char *XMLrd::TMKXrefFormat = "TmarkXref";
DCgroup *XMLrd::TMKTextGroup = NULL;
char *XMLrd::TmarkFileName = NULL;

DCnlist XMLrd::GenFileNames;
DCilist XMLrd::GenFileElems;
long XMLrd::GenFileCount = 0;

char *XMLrd::GenListXrefFormat = "TitleXref";
char *XMLrd::ListXrefFormat = "TitleXref";


void
XMLrd::SetGenFileRef(DCmapelem *elem)
{
	// called from ProcTopicref, first info on it

	gentyp gt = gennone;

	if ((gt = (gentyp) GenListNames(elem->elemname)) == gennone) {
		char *classatt = elem->classatt;
		char *slash = NULL;
		char *space = NULL;
		char *classend = classatt + strlen(classatt) - 1;
		while (classend > classatt) {
			while (*classend == ' ')
				classend--;
			slash = classend;
			while ((slash > classatt)
			 && (*slash != '/'))
				slash--;
			if (slash > classatt) {  // use classes right to left
				char *echeck = NewName(slash + 1, classend - slash);
				gt = (gentyp) GenListNames(echeck);
				DeleteName(echeck);
			}
			if (gt != gennone)
				break;
			while ((classend > classatt)
			 && (*classend != ' '))
				classend--;
		}
	}
	elem->gentype = gt;

	if (gt == gentoc) {
		//InTOCTOC = true;
		elem->toc = false;  // toc doesn't contain itself
	}

	// elem->href specifies file name to use for list,
	// if no doc to process, reserve a page for it
	if (elem->href)
		return;

	//if (!elem->toc)
	//	InTOCTOC = true;

	elem->gendoc = true;

	if (gt == genidx) {
		DCixlist *list = NULL;
		char *name = DCixset::listset->getname(elem->elemname, elem->outclass);
		if ((list = DCixset::listset->listexists(name)) == NULL) {
			list = new DCixlist(name);
			DCixset::listset->addlist(list);
			DCixset::listset->setuplist(list);
		}
		list->setlistprops(elem->outclass, elem->navtitle ? elem->navtitle : IDXTitle);
	}
	else {
		SetUpGenFile(elem);
		DCgenlist *gl = (DCgenlist *) GenLists.find(GenFileCount);

		// set TOC format in elem->elform
		if (gl->tocform)
			elem->elform = gl->tocform;
	}
}


void 
XMLrd::SetTOCTOC(void)
{
	//DCctl *cp = NULL;

	//cp = new DCctl(imnem, 2, 117, 1, 0);
	//TextGroup->add(*cp);
	if (!TOCWrapper)
		TOCWrapper = new DCgroup();
	PageTextGroup = TextGroup = TOCWrapper;
	//cp = new DCctl(imnem, 2, 117, 1, 2);
	//TextGroup->add(*cp);
	//InTOCTOC = false;
}


DCgroup *XMLrd::TOCIgnore = NULL;
DCgroup *XMLrd::PreTOCIgnore = NULL;

void
XMLrd::SetGenFileTOC(DCmapelem *elem, int depth, long level)
{
	// called from ProcTopicrefText when no href
	// after resolving keyref if any
	// when elem->gentype != gennone
	char *title = NULL;
	char *id = NULL;
	char *form = NULL;
	DCixlist *list = NULL;
	DCgenlist *gl = NULL;

	elem->gendoc = true;

	long gnum = GenFileElems.getitem((long) elem);
	if (!gnum) { // see if index
		char *name = DCixset::listset->getname(elem->elemname, elem->outclass);
		if ((list = DCixset::listset->listexists(name)) == NULL)
			return;  // not an index either

		title = list->title;
		id = list->id;
		form = list->tocform;
	}
	else {
		gl = (DCgenlist *) GenLists.find(gnum);
		if (gl) {
			title = gl->title;
			id = gl->id;
			form = gl->tocform;
		}
	}

	PreTOCIgnore = TextGroup;
	if (!elem->toc) {
		TOCIgnore = new DCgroup(DCctl(group, 1, 30, 1));
		TextGroup->add(* (DCctl *) TOCIgnore);
		TextGroup = TOCIgnore;
	}
	StartPara();
	if (form) {
		elem->elform = form;
		SetFormat(elem->elform);
		FormatUsed.add(elem->elform, depth);
	}
	else
		SetMapFormat(elem, depth, level);

	SetFlags(depth);
	SetMapElemTags();

	DCctl *cp = WriteGenListTOC(elem, title, id);

	if (!gnum)
		list->frefcp = cp;
	else
		gl->frefcp = cp;
}


DCctl *
XMLrd::WriteGenListTOC(DCmapelem *elem, char *title, char *id)
{
	DCctl *cp = new DCctl(etext, 2, 112, 3);
	cp->ex(MainDocName, strlen(MainDocName));

	if (IsRtf) {
		//char *did = MainBaseName + strlen(MainBaseName) - 3;
		//char *srcid = NewName("R", strlen(id) + 4);
		//strcat(srcid, did);
		//strcat(srcid, id);
		//id = srcid;
	}
	else
		TextGroup->add(*cp);

	DCctl *cp2 = new DCctl(etext, 2, 112, 4);
	cp2->ex(id, strlen(id));
	TextGroup->add(*cp2);
	PutTextStrCtl(title);
	EndPara();
	TextGroup = PreTOCIgnore;
	TOCIgnore = NULL;
	return cp;
}


void
XMLrd::SetUpGenFile(DCmapelem *elem)
{
	// definitely to be generated
	DCgenlist *gl = new DCgenlist(elem, elem->gentype);

	// set defaults based on type from old sections

	switch (gl->typ) {
		case gentoc:
			UseContents = true;
			gl->title = TOCTitle;
			gl->href = TOCFile;
			//gl->id = "Rtoc";
			gl->suf = TOCSuffix;
			gl->titleform = TOCTitleFormat;
			gl->itemform = TOCFormat;
			gl->xrefform = TOCXrefFormat;
			gl->tocform = NULL;
			break;

		case genlof:
			GenerateLOF = true;
			gl->title = LOFTitle;
			gl->href = LOFFile;
			//gl->id = "Rlof";
			gl->suf = LOFSuffix;
			gl->titleform = LOFTitleFormat;
			gl->itemform = LOFFormat;
			gl->xrefform = LOFXrefFormat;
			gl->tocform = LOFTOCFormat;
			break;

		case genlot:
			GenerateLOT = true;
			gl->title = LOTTitle;
			gl->href = LOTFile;
			//gl->id = "Rlot";
			gl->suf = LOTSuffix;
			gl->titleform = LOTTitleFormat;
			gl->itemform = LOTFormat;
			gl->xrefform = LOTXrefFormat;
			gl->tocform = LOTTOCFormat;
			break;

		case genidx:
			// this function isn't called in this case ****
			gl->title = IDXTitle;
			gl->href = IDXFile;
			//gl->id = "Ridx";
			gl->suf = IDXSuffix;
			gl->titleform = IDXTitleFormat;
			gl->itemform = IDXFormat;
			gl->tocform = IDXTOCFormat;
			IndexInTOC = true;
			break;

		case gengloss:
			gl->title = GLSTitle;
			gl->href = GLSFile;
			//gl->id = "Rgls";
			gl->suf = GLSSuffix;
			gl->titleform = GLSTitleFormat;
			gl->xrefform = GLSXrefFormat;
			gl->tocform = GLSTOCFormat;
			break;

		case genabbr:
			gl->title = ABRTitle;
			gl->href = ABRFile;
			//gl->id = "Rabr";
			gl->suf = ABRSuffix;
			gl->titleform = ABRTitleFormat;
			gl->xrefform = ABRXrefFormat;
			gl->tocform = ABRTOCFormat;
			break;

		case gentmk:
			gl->title = TMKTitle;
			gl->href = TMKFile;
			//gl->id = "Rtmk";
			gl->suf = TMKSuffix;
			gl->titleform = TMKTitleFormat;
			gl->xrefform = TMKXrefFormat;
			gl->tocform = TMKTOCFormat;
			break;

		default:
			gl->tocform = LOFTOCFormat;
			gl->titleform = TOCTitleFormat;
			gl->xrefform = TOCXrefFormat;
			gl->tocform = LOFTOCFormat;
			break;  // type not in old sections
	}

	// set individual overrides to the standard values ****

	char *gkey = NULL;
	char *gname = NULL;

	if (IniFile->Section("BookLists")) {
		if (elem->outclass) {
			gkey = NewName(elem->elemname, strlen(elem->elemname)
																	 + strlen(elem->outclass) + 2);
			strcat(gkey, "/");
			strcat(gkey, elem->outclass);
			if (IniFile->Find(gkey))
				gname = IniFile->StrRight();
			DeleteName(gkey);
		}

		if (!gname) {  // no match to name/outputclass
			if (IniFile->Find(elem->elemname))
				gname = IniFile->StrRight();
			else
				gname = NewName(elem->elemname);
		}
	}

	if (gname) {  // key to rest of sections
		long len = strlen(gname);
		char *secname = NewName(gname, len + 8);
		strcat(secname, "BList");
		if (IniFile->Section(secname)) {
			if (IniFile->Find("FileName"))
				gl->fname = IniFile->StrRight();
			if (IniFile->Find("Suffix"))
				gl->suf = IniFile->StrRight();
			if (IniFile->Find("RefID"))
				gl->id = IniFile->StrRight();
			if (IniFile->Find("TOCFormat"))
				gl->tocform = IniFile->StrRight();
			if (IniFile->Find("TitleFormat"))
				gl->titleform = IniFile->StrRight();
			if (IniFile->Find("ItemFormat"))
				gl->itemform = IniFile->StrRight();
			if (IniFile->Find("XrefFormat"))
				gl->xrefform = IniFile->StrRight();
			if (IniFile->Find("KeepWithPrev"))
				gl->keepprev = IniFile->BoolRight();
		}

		// mark each format for list inclusion
		strcpy(secname + len, "BLItems");
		if (IniFile->Section(secname)) {
			while (IniFile->NextItem()) {
				char *sel = IniFile->StrLeft();
				long level = IniFile->IntRight();
				if (!stricmp(sel, "end")) {
					gl->maxlevel = level - 1;
					break;
				}
				gl->itemsels.add(sel, ++gl->itemselcnt);
				gl->itemlevs.add(level, gl->itemselcnt);
				if (level > gl->maxlevel)
					gl->maxlevel = level;
			}
		}

		// store the format for each level
		if (gl->maxlevel > 1) {
			strcpy(secname + len, "BLForms");
			if (IniFile->Section(secname)) {
				while (IniFile->NextItem())
					gl->itemforms.add(IniFile->StrRight(), atoi(IniFile->StrLeft()));
			}
		}
		
		strcpy(secname + len, "BLText");
		if (LangFile->Section(secname)) {
			if (LangFile->Find("ListTitle"))
				gl->title = LangFile->StrRight();
		}

		if (elem->navtitle)
			gl->title = elem->navtitle;
		else
			elem->navtitle = gl->title;

		DeleteName(secname);
	}
	else
		gname = NewName(elem->elemname);

	// handle any members still needed but not set, like id
	if (!gl->id) {
		gl->id = NewName("R", strlen(gname) + 2);
		strcat(gl->id, gname);
	}

	if (!gl->href) {
		if (gl->fname)
			gl->href = gl->fname;
		else if (gl->suf) {
			gl->href = NewName(MainBaseName, strlen(MainBaseName)
																		 + strlen(gl->suf) + 5);
			strcat(gl->href, gl->suf);
			strcat(gl->href, ".htm");
		}
		else {
			gl->href = NewName(MainBaseName, strlen(MainBaseName)
																		 + strlen(gl->id) + 5);
			strcat(gl->href, gl->id);
			strcat(gl->href, ".htm");
		}
	}

	gl->gname = gname;

	GenFileNames.addnew(gl->href, ++GenFileCount);
	GenLists.add(gl, GenFileCount);
	GenFileElems.add((long) elem, GenFileCount);

	switch (gl->typ) {
		case gengloss:
			GlossaryFileName = gl->href;
			break;

		case genabbr:
			AbbrFileName = gl->href;
			break;

		case gentmk:
			TmarkFileName = gl->href;
			break;

		default:
			break;
	}
}


void
XMLrd::SetGeneratedFile(DCmapelem *elem)
{
	// called from ParseDocument
	char *href = NULL;
	char *title = NULL;
	char *titleform = NULL;
	char *id = NULL;
	long typ = 0;
	DCgroup **gpp = NULL;
	bool usehead = true;

	InGenerateFile = true;
	long gnum = GenFileElems.getitem((long) elem);
	char *pname = DCLPartFileNames.find(DCLPartFile);

	if (!gnum) { // see if index
		DCixlist *list = NULL;
		char *name = DCixset::listset->getname(elem->elemname, elem->outclass);
		if ((list = DCixset::listset->listexists(name)) == NULL)
			return;  // not an index either

		href = list->href;
		title = list->title;
		titleform = list->titleform;
		id = list->id;
		typ = (long) genidx;
		gpp = &(list->gp);
		usehead = list->usehead;;
		if (pname && DCLPartFile) {
			IndexListPartName = NewName(pname);
			list->frefcp->ex(pname, strlen(pname));
		}
		else
			IndexListPartName = NewName(MainDocName);
		if (IsRtf)
			list->frefcp->maj((uns) 0);  // remove it
	}
	else {
		DCgenlist *gl = (DCgenlist *) GenLists.find(gnum);
		gl->docref = elem;
		href = gl->href;
		title = gl->title;
		titleform = gl->titleform;
		id = gl->id;
		typ = (long) gl->typ;
		gpp = &(gl->gp);
		if (pname)
			gl->frefcp->ex(pname, strlen(pname));
		//if ((gl->typ == gentoc)
		// && !elem->toc)
		//if (!elem->toc)
		//	InTOCTOC = true;
	}

	WriteGenListHead(elem, href, title, titleform, id, typ, gpp, usehead);

	InGenerateFile = false;
}


void
XMLrd::WriteGenListHead(DCmapelem *elem, char *href, char *title,
		 char *titleform, char *id, long typ, DCgroup **gpp, bool usehead)
{
	char *chtxt = NewName("chunkfile:", strlen(href) + 11);
	strcat(chtxt, href);

	StartPage();

	// add ctrls for gen type, name, and href from CurrDocRef
	DCctl *cp = new DCctl(imnem, 2, 115, (uns) typ);
	TextGroup->add(*cp);
	cp = new DCctl(etext, 2, 116, 1);
	cp->ex(title);
	TextGroup->add(*cp);
	cp = new DCctl(etext, 2, 116, 2);
	cp->ex(href);
	TextGroup->add(*cp);

	// write title
	if (usehead) {
		StartPara();
		SetFormat(CurrFormat = titleform);
		WriteTextNumPrefix(elem, titleform);
	}

	if ((SplitTopicFiles
	  || (!DisableChunking
	   && (elem->chunkby == cbtopic))
		|| (UMXL
	   && elem->chunkby == cbdoc))
	 && ((typ != gentoc)
	  || !KeepTOCWithTitlePage)) {
		cp = new DCctl(etext, 2, 110, 3);
		cp->ex("chunk", 6);
		TextGroup->add(*cp);
		cp = new DCctl(etext, 2, 110, 3);
		cp->ex(chtxt, strlen(chtxt));
		TextGroup->add(*cp);
	}

	cp = new DCctl(etext, 2, 112, 2);
	cp->ex(id, strlen(id));
	TextGroup->add(*cp);

	if (usehead) {
		PutTextStrCtl(title);
		EndPara();
	}

#if 0
	if (InTOCTOC) {
		//PreTOCTOCIgnore = TextGroup;
		//TOCTOCIgnore = new DCgroup(DCctl(group, 1, 30, 1));
		//TextGroup->add(*TOCTOCIgnore);
		//TextGroup = TOCTOCIgnore;
		DCctl *cp2 = new DCctl(group, 1, 30, 1);
		TextGroup->add(*cp2);
		InTOCTOC = false;
	}
#endif

	// set group for later addition of list items
	*gpp = new DCgroup;
	TextGroup->add(**gpp);
}


bool XMLrd::InFigTitle = false;
DClist XMLrd::LOFItems;  // list of IDs of figure titles to add as xrefs
DClist *XMLrd::LOFItemList = &LOFItems;  // pointer to above list
bool XMLrd::InTblTitle = false;
DClist XMLrd::LOTItems;  // list of IDs of table titles to add as xrefs
DClist *XMLrd::LOTItemList = &LOTItems;  // pointer to above list

bool XMLrd::InGlossary = false;

void
XMLrd::AddGenListItem(DCelem *elem, char *form)
{
	// see if format is included in any list

	DCgenlist *gl = NULL;

	for (long i = 1; i <= GenFileCount; i++) {
		gl = (DCgenlist *) GenLists.find(i);
		if (!gl)
			continue;
		if (gl->itemselcnt
		 && gl->itemsels.getitem(form)) {
			if (ProcessingConrefs) {
				if (!gl->conrefitems)
					gl->conrefitems = new DClist();
				gl->conrefitems->add((long) elem);
				++gl->conrefitemcnt;
			}
			else {
				if (!gl->items)
					gl->items = new DClist();
				gl->items->add((long) elem);
				++gl->itemcnt;
			}
			elem->ingenlist = true;
		}
	}
}


void
XMLrd::FixConrefGenLists(DCelem *srcelem, DCelem* newelem)
{
	DClist *lp = NULL;
	DCgenlist *gl = NULL;

	for (long i = 1; i <= GenFileCount; i++) {
		gl = (DCgenlist *) GenLists.find(i);
		if (MakeConrefLists) {
			if (newelem->ingenlist) {
				gl->conrefitems->append((long) newelem);
				gl->conrefitemcnt++;
			}
		}
		else {
			if (newelem->anumtxt
			 && ((lp = gl->conrefitems->find((long) srcelem)) != NULL))
				lp->id = (long) newelem;
		}
	}
}


void
XMLrd::MarkConrefGenList(long refnum)
{
	DCgenlist *gl = NULL;

	for (long i = 1; i <= GenFileCount; i++) {
		gl = (DCgenlist *) GenLists.find(i);
		if (!gl->items)
			gl->items = new DClist();
		gl->items->append(refnum);
	}
}


void
XMLrd::EndConrefGenLists(DCcnref *cinfo)
{
	DCgenlist *gl = NULL;

	for (long i = 1; i <= GenFileCount; i++) {
		gl = (DCgenlist *) GenLists.find(i);
		if (gl->conrefitemcnt) {
			gl->items->substitute(cinfo->cnum, -cinfo->cnum, gl->conrefitems);
			gl->conrefitemcnt = 0;
		}
	}
}


void
XMLrd::ProcessGeneratedLists(void)
{
#ifdef UDOC
		XMLud::IndexSeeIDrefs.all(XMLud::ProcOneUMXLIndexSee);
#endif

	//DCmapelem *elem = NULL;
	//while ((elem = (DCmapelem *) GenLists.dequeue()) != NULL)
	//	WriteGenListItems(elem->gentype);
	DCgenlist *gl = NULL;
	while ((gl = (DCgenlist *) GenLists.dequeue()) != NULL)
		WriteGenListItems(gl);
	
	if (DCixset::listset
	 && DCixset::listset->listcnt) {
		DCvlist *lstp = &(DCixset::listset->lists);
		DCixlist *list = NULL;
		while (((lstp = lstp->next) != NULL)
		 && ((list = (DCixlist *) (lstp->vp)) != NULL))
			WriteIndexListItems(list);
	}
}


void
XMLrd::WriteGenListItems(DCgenlist *gl)
{
	char *form = NULL;
	DClist *dl = gl->items;
	DCctl *cp = NULL;
	bool gloss =  false;
	listdefinfo *ldp = NULL;
	if (!gl->gp)
		gl->gp = new DCgroup();

	switch (gl->typ) {
		case gentoc:
			if (TOCWrapper)
				gl->gp->add(*TOCWrapper);
			else {
				cp = new DCctl(imnem, 2, 117, 1, 1);
				gl->gp->add(*cp);
			}
			if (!strcmp(gl->gname, "toc"))  // main toc
				return;
			break;
		case genlof:
			if (!GenerateLOF)
				return;
			if (!gl->itemcnt)
				dl = LOFItemList;
			break;
		case genlot:
			if (!GenerateLOT)
				return;
			if (!gl->itemcnt)
				dl = LOTItemList;
			break;
		case genidx:
			// write ctl to mark place for index 
			cp = new DCctl(imnem, 2, 117, 2, 1);
			gl->gp->add(*cp);
			if (!strcmp(gl->gname, "indexlist"))  // main index
				return;
			break;
		case gengloss:
			gloss = true;
			DocRoot = "glossary";
#ifdef UDOC
			ldp = &XMLud::GlossDefs;
#endif
#ifdef DITA
			//if (!UMXL)
				return;
#endif
			break;
		case genabbr:
			gloss = true;
			DocRoot = "abbreviations";
#ifdef UDOC
			ldp = &XMLud::AbbrDefs;
#endif
#ifdef DITA
			//if (!UMXL)
				return;
#endif
			break;
		case gentmk:
			gloss = true;
			DocRoot = "trademarks";
#ifdef UDOC
			ldp = &XMLud::TmarkDefs;
#endif
#ifdef DITA
			//if (!UMXL)
				return;
#endif
			break;
		default:
			break;  // another type
	}

	if (!dl
	 && !gloss)
		return;

	// just process LOF, LOT, and dictionary lists here

	DCelem *einfo = NULL;

	DCgroup *lastgp = TextGroup;
	TextGroup = gl->gp;
	PageStarted = true;
	ListXrefFormat = gl->xrefform ? gl->xrefform : GenListXrefFormat;
	form = gl->itemform;
	CurrDocRef = gl->docref;

	if (gloss) {

#ifdef UDOC
		if (CurrDocRef)
			XMLud::CurrConds = (condset *) CurrDocRef->conds;
		else
			XMLud::CurrConds = NULL;
#endif
#ifdef DITA
			CurrCondSet = (conditions *) CurrDocRef->conds;
		CurrCondRefLevel = CurrDocRef->condlevel;
#endif
		ChunkDoc = false;
		ChunkDocSet = false;
		ChunkTopic = false;
		CurrBranchDepth = 0;
		CurrRootDepth = 2;
		CurrDocLevel = 2;
		CurrDepth = 3;
		InGlossary = true;
		XMLud::PutAllListDefs(ldp, gl->typ);
		InGlossary = false;
	}
	else {
		while ((einfo = (DCelem *) dl->dequeue()) != NULL) {
			if (((long) einfo > ConrefT2TCount)
			 || ((long) einfo < -ConrefT2TCount)) {
				if (gl->maxlevel) {
					long it = gl->itemsels.getitem(einfo->elform);
					long lev = gl->itemlevs.find(it);
					form = gl->itemforms.find(lev);
					if (!form)
						form = gl->itemform;
				}
				WriteOneGenListItem(form, einfo);
			}
		}
	}

	PageStarted = false;
	TextGroup = lastgp;
}


void
XMLrd::WriteOneGenListItem(char *form, DCelem *einfo)
{
	StartPara();
	SetFormat(form);

	DCgroup *gp = new DCgroup(DCctl(group, 2, 10, 7));
	TextGroup->add(*gp);
	DCgroup *lasttext = TextGroup;
	TextGroup = gp;

	long fnum = XrefFormNameDefs.getitem(ListXrefFormat);
	if (!fnum) {
		char *format = NULL;
		XrefFormNameDefs.add(ListXrefFormat, (fnum = ++XrefFormDefCount));
		if ((format = SubFormatsFile->GetXrefFmtForm(ListXrefFormat)) == NULL)
			format = GetDefaultXrefFormatDef(ListXrefFormat);
		XrefFormDefs.add(format, fnum);
	}

	DCctl *cp = new DCctl(imnem, 2, 96, 3, fnum);
	TextGroup->add(*cp);

	cp = new DCctl(ename, 2, 96, 5);
	DCtopic *top = ((DCtopelem *) einfo)->topic;
	char *fname = NULL;
	if (top
	 && top->part
	 && ((fname = DCLPartFileNames.find(top->part)) != NULL))
		cp->ex(fname, strlen(fname));
	else
		cp->ex(MainDocName, strlen(MainDocName));
	if (IsRtf)
		cp->maj((uns) 0);  // remove it
	TextGroup->add(*cp);

	cp = new DCctl(ilong, 2, 112, 2, ++ItemID);
	TextGroup->add(*cp);

	char *id = einfo->elemuid;
	if (!id)
		id = einfo->id;
	cp = new DCctl(etext, 2, 96, 2);
	cp->ex(id);
	TextGroup->add(*cp);

	// insert content into this group
	DCgroup *cgp = new DCgroup(DCctl(group, 2, 98, 4));
	TextGroup->add(*cgp);

	char *fcp = GetXrefFormat(ListXrefFormat);
	TextGroup = cgp;

	DCelem *etinfo = einfo;
	if (!einfo->children) { // title has ID but parent doesn't
		einfo = einfo->parent;
	}
	else {
		etinfo = (DCelem *) einfo->children->next->id;
	}
	SetXrefContent(fcp, einfo, etinfo);

	TextGroup = lasttext;

	EndPara();
}



// index lists


bool XMLrd::UseEntry = true;
bool XMLrd::UseList = false;
bool XMLrd::UseDefList = true;
DCgroup *XMLrd::CurrListIndexGroup = NULL;

bool XMLrd::SortSeeAlsoFirst = false;
char *XMLrd::IgnoreLeadingCharsIX = ".$*";
char *XMLrd::IgnoreCharsIX = "-[]()<>_";


void 
XMLrd::CheckIndexListItem(void)
{
	ixuse ixlistuse = DCixset::listset->checklist(CurrElemInfo->outclass);
	switch (ixlistuse) {   // select process for lists for this ref
		case ixuseno:  // skip this entry entirely
			UseEntry = false;
			UseList = false;
			UseDefList = false;
			break;

		default:
		case ixent:    // just make ix marker, no lists
			UseEntry = true;
			UseList = false;
			if (MakeDefaultIDX)
				UseDefList = true;
			break;

		case ixgen:    // just lists, not index marker
			UseEntry = false;
			UseList = true;
			UseDefList = false;
			break;

		case ixall:    // lists and index marker
			UseEntry = true;
			UseList = true;
			if (MakeDefaultIDX)
				UseDefList = true;
			break;
	}
}


void 
XMLrd::StartIndexListItem(void)
{
	DCixref::CurrIXRef = new DCixref(CurrElemInfo,
			                             CurrElemInfo->outclass, 1);
	CurrListIndexGroup = CurrIndexGroup;

	//if (!CurrElemInfo->idxrefs)
	//	CurrElemInfo->idxrefs = new DClist();
	//CurrElemInfo->idxrefs->add((long) DCixref::CurrIXRef);

	CurrElemInfo->idxref = DCixref::CurrIXRef;
	DCixref::CurrIXRef->id = CurrElemInfo->id;
}


void 
XMLrd::AddIndexListItemLevel(void)
{
	DCixref::CurrIXRef->settitle(CurrListIndexGroup, ProcessingConrefs);
	DCixref* parent = DCixref::CurrIXRef;
	//DCixref::CurrIXRef = new DCixref(parent->elem,
	//	 parent->selclass, parent->level + 1, parent);
	//((DCelem *) (parent->elem))->idxrefs->add((long) DCixref::CurrIXRef);

	DCixref::CurrIXRef = new DCixref(CurrElemInfo,
		 parent->selclass, parent->level + 1, parent);

	//if (!CurrElemInfo->idxrefs)
	//	CurrElemInfo->idxrefs = new DClist();
	//CurrElemInfo->idxrefs->add((long) DCixref::CurrIXRef);

#ifdef DITA
		CurrElemInfo->idxref = DCixref::CurrIXRef;
#endif
}


void 
XMLrd::AddIndexListSeeItemLevel(void)
{
	DCixref* parent = DCixref::CurrIXRef;
	//DCixref::CurrIXRef = new DCixref(parent->elem,
	//	parent->selclass, parent->level + 1, parent);

	DCixref::CurrIXRef = new DCixref(CurrElemInfo,
		 parent->selclass, parent->level + 1, parent);

#ifdef DITA
		CurrElemInfo->idxref = DCixref::CurrIXRef;
#endif
}


void 
XMLrd::RestartIndexListItem(void)
{
	DCixref* parent = DCixref::CurrIXRef->parent;
	if (DCixref::CurrIXRef->level < IndexLevel)
		parent = DCixref::CurrIXRef;

	//DCixref::CurrIXRef = new DCixref(parent->elem,
	//	 parent->selclass, parent->level + 1, parent);
	//((DCelem *) (parent->elem))->idxrefs->add((long) DCixref::CurrIXRef);

	DCixref::CurrIXRef = new DCixref(CurrElemInfo,
		 parent->selclass, parent->level + 1, parent);

#ifdef DITA
		CurrElemInfo->idxref = DCixref::CurrIXRef;
#endif
}


void 
XMLrd::StartIndexListSeeSortItem(void)
{
	DCixref::CurrIXRef->settitle(CurrListIndexGroup, ProcessingConrefs);
}


void 
XMLrd::EndIndexListSeeItem(void)
{
	// see or see also
	DCixref::CurrIXRef->settitle(IndexLevelSeeGroup, ProcessingConrefs);
}


void 
XMLrd::EndIndexListSortItem(void)
{
	DCixref::CurrIXRef->setsortstr(IndexLevelSortGroup->getText());
}


void 
XMLrd::EndIndexListItem(void)
{
	DCixref *ref = DCixref::CurrIXRef;
	ref->settitle(CurrListIndexGroup, ProcessingConrefs);
	DCixref::CurrIXRef = ref->parent;  // becomes either parent or NULL

	//while ((IndexLevel > 1)
	// && (DCixref::CurrIXRef->level > IndexLevel))
	//	DCixref::CurrIXRef = DCixref::CurrIXRef->parent;

	if (!ProcessingConrefs)  // don't add ID for conref source
		SetIndexRefIDs(ref);
}


void 
XMLrd::SetIndexRefIDs(DCixref *ref)
{

	// get IDs of topic title and of closest title to indexterm
	DCgroup **reftgrp = NULL;  // has &(elem->elemtref) for title
	char *topid = NULL;
	char *refid = NULL;
	DCelem *elem = (DCelem *) ref->elem;
	DCelem *xelem = NULL;
	DCelem *refelem = NULL;
	DCelem *child = NULL;
	bool inmap = false;

	if (elem->eltyp == esmap) {
		// if elem is in map, go up to topicref, for navtitle
		inmap = true;
		refelem = elem->parent->parent->parent;  // topicref
	}
	else {
		DCtopic *top = ((DCtopelem *) elem)->topic;
		DCpage * page = top->page;
		topid = top->id;
		// if elem is a title with an ID, or has a first child
		// that is a title and has an id, refid = elem->id
		// and reftgrp = title's &(elem->elemtref) since
		// we may be at start of title with no text yet
		// if elem is/has neither, check its parents up to topic root
		if (InTitleElem) { // TitleGroup will have title
			xelem = elem;
			while ((xelem = xelem->parent) != NULL) {
				page->setchild(&xelem);
				if ((TitleGroup == xelem->elemtref)
				 && xelem->checkType(epTitle)) {
					reftgrp = &(xelem->elemtref);
					refid = xelem->id;
					refelem = xelem;
					break;
				}
			}
		}
		if (!refid) {
			//elem = (DCelem *) DCixref::CurrIXRef->elem;
			xelem = elem;
			while ((xelem = xelem->parent) != NULL) {
				page->setchild(&xelem);
				if (xelem->children
				 && ((child = (DCelem *) xelem->children->next->id) != NULL)
				 && child->checkType(epTitle)) {
					page->setchild(&child);
					reftgrp = &(child->elemtref);
					refid = child->id;
					refelem = child;
					break;
				}
			}
		}
		if (!refid) {
			long i = 0;
			long cnt = top->elems->count();
			for (i = 2; i <= cnt; i++) {
			child = (DCelem *) top->elems->find(i);  // topic title elem
				if (child
				 && child->checkType(epTitle)) {
					page->setchild(&elem);
					reftgrp = &(child->elemtref);
					refelem = child;
					break;
				}
			}
		}
	}

	ref->setids(refelem, reftgrp, topid, refid, inmap);
}


DCixref *XMLrd::LastRefParent = NULL;


void 
XMLrd::CloneIndexRef(DCelem *elem, DCixref *ref)
{
	if ((ref->itype > ixstd)  // don't clone see/also
	 || (ref->range > rngno)) // or ranges
		return;

	// clone ref and re-add it to its items
	DCixref *nref = NULL;
	DCixitem *item = NULL;

	if (ref->parent
	 && LastRefParent)
	// && (ref->parent == LastRefParent))
		nref = new DCixref(elem,
				 ref->parent->selclass, ref->parent->level + 1, LastRefParent);
	else
		LastRefParent = nref = new DCixref(elem, elem->outclass, 1);


	// set title and sort
	nref->title = ref->title;
	nref->sort = ref->sort;

	// set map, refelem, topid, refid, reftgroup
	nref->map = ref->map;
	nref->topid = NULL;
	nref->refid = NULL;
	nref->refelem = NULL;
	nref->reftgroup = NULL;
	nref->reftitle = NULL;

#if 0
	if (nref->map) {
		if (((DCmapelem *) elem)->reftop)
			nref->topid = ((DCmapelem *) elem)->reftop->id;
		nref->refelem = elem->parent->parent->parent;  // topicref
	}
	else {
		nref->topid = ((DCtopelem *) elem)->topic->id;
		DCelem *xelem = elem;
		DCelem *child = NULL;
		while ((xelem = xelem->parent) != NULL) {
			if (xelem->children
			 && ((child = (DCelem *) xelem->children->next->id) != NULL)
			 && child->checkType(epTitle)) {
				nref->reftgroup = &(child->elemtref);
				nref->refid = child->id;
				nref->refelem = child;
				break;
			}
		}
		if (!nref->refid) {
			child = (DCelem *) ((DCtopelem *) elem)->topic->elems->find(2);  // topic title elem
			if (child
			 && child->checkType(epTitle)) {
				nref->reftgroup = &(child->elemtref);
				nref->refelem = child;
			}
		}
	}
#endif

	if (ref->itemcnt == 0)
		nref->setlistitem();

	else if (ref->subcnt == 0) {
		// add to items for itemcnt
		for (long i = 1; i <= ref->itemcnt; i++) {
			item = (DCixitem *) ref->items.find(i);
			item->addref(nref);
		}
	}

	SetIndexRefIDs(nref);

	// replace in elem->idxrefs
	//elem->idxrefs->replace((long) nref, (long) ref);
	elem->idxref = nref;
}


char *XMLrd::IndexListPartName = NULL;


void 
XMLrd::WriteIndexListItems(DCixlist *list)
{
	if (!list->gp)
		return;

	DCgroup *lastgp = TextGroup;
	TextGroup = list->gp;

	PageStarted = true;

	DCgroup *gpp = TextGroup;

	// set list-specific props
	DCctl *cp = new DCctl(imnem, 2, 118, 2, list->uselets);
	gpp->add(*cp);
	cp = new DCctl(imnem, 2, 118, 3, list->usetlets);
	gpp->add(*cp);

	// signal start to dwhtm
	cp = new DCctl(imnem, 2, 117, 2, 2);
	gpp->add(*cp);

	char *pname = (char *) list->frefcp->ex();
	if (pname && DCLPartFile)
		IndexListPartName = NewName(pname);
	else
		IndexListPartName = NewName(MainDocName);
	if (IsRtf)
		list->frefcp->maj((uns) 0);  // remove it

	if (list->itemcnt) {
#ifdef DITA
			list->procseeids();
#endif
		DCnlist *ip = &list->itemsorts;
		long pos = 0;
		DCixitem *it = NULL;
		while ((ip = ip->next) != NULL) {
			pos = ip->id;
			it = (DCixitem *) (list->items.find(pos));
			if (it)
				WriteOneIndexListItem(it, list, gpp, 1);
		}
	}

	// signal end to dwhtm
	cp = new DCctl(imnem, 2, 117, 2, 3);
	gpp->add(*cp);

	PageStarted = false;
	TextGroup = lastgp;
}


bool XMLrd::UseCompactForm  = true;
bool XMLrd::IndexRefIsPara = false;
bool XMLrd::IndexRefsWithEntry = true;
bool XMLrd::IndexRangeStartOnly = false;
bool XMLrd::UseIndexHeading = true;
bool XMLrd::UseIndexLeader = true;
bool XMLrd::UseIndexLetters = true;
bool XMLrd::UseIndexTopLetters = true;


void 
XMLrd::WriteOneIndexListItem(DCixitem *item, DCixlist *list, DCgroup *gpp, long lev)
{
	// write one item and its subs as DCL

	long pos = 0;
	DCixref *ref = NULL;
	DCctl *cp = NULL;

	DCgroup *lastgp = TextGroup;
	TextGroup = gpp;

	// if using index letters, add next one if needed here
	if (list->uselets
	 && (item->level == 1)) {
		long let = (long) (item->sort ? *(item->sort) : *(item->title));
		cp = new DCctl(ilong, 2, 117, 2, (let & 0xFF));
		gpp->add(*cp);
	}

	// signal start to dwhtm
	cp = new DCctl(imnem, 2, 117, 2, 4);
	gpp->add(*cp);

	// handle see, see also
	if ((item->itype > ixstd)
	 && item->id
	 && *(item->id)) {
		WriteOneIndexSeeItem(item, list, gpp, lev);
		cp = new DCctl(imnem, 2, 117, 2, 5);
		gpp->add(*cp);
		TextGroup = lastgp;
		return;
	}

	if (!item->refcnt
	 && !item->subcnt) {
		cp = new DCctl(imnem, 2, 117, 2, 5);
		gpp->add(*cp);
		return;
	}

	// write title in correct format for level
	char *form = list->itemforms.find(lev);
	if (!form)
		form = list->itemform;

	StartPara();
	SetFormat(form);

	// if id, and not see/also, set id (if see/also, id is ref)
	if (item->id) {
		cp = new DCctl(etext, 2, 112, 2);
		cp->ex(item->id, strlen(item->id));
		TextGroup->add(*cp);
	}

	if (item->gtitle)
		item->gtitle->writeIdxTitle(TextGroup, true);
	else 
		PutTextStrCtl(item->title);

	if (!list->ixrefentry)
		EndPara();

	// write refs, unless ref has subs  (ref->subcnt > 0)
	// handle ranges; start gets next end
	// if no end before next start, or before end of list, warning

	bool refstart = true;
	long epos = 0;

	if (!list->ixrefpara
	 && !list->ixrefentry) {
		char *form = list->refforms.find(lev);
		if (!form)
			form = list->refform;
		StartPara();
		SetFormat(form);
	}

	if (item->refcnt) {
		if (list->ixrefpara) {
			cp = new DCctl(imnem, 2, 117, 2, 6);
			gpp->add(*cp);
		}
		// else for RTF add tab here, refstart = true; ****

		for (pos = 1; pos <= item->refcnt; pos++) {
			ref = (DCixref *) item->refs.find(pos);
			if (ref->subcnt)
				continue;        // ref is meant for subitems
			if (ref->range == rngstart) {
				if (!list->ixrefpara) {
					if (refstart)
						WriteIndexEntryRefSep(list);
					else
						WriteIndexRefSep();
				}
				if (ref->eref)  // matching end ref
					WriteOneIndexRefRange(ref, ref->eref, list, gpp, lev);
				else {
					DCixset::LogEvent(logwarn, 1, "Index range start has no end: ",
																				 ref->rname);
					WriteOneIndexRefItem(ref, list, gpp, lev);
				}
				refstart = false;
			}
			else if (ref->range == rngend)
				continue;
			else {             // normal single entry
				if (!list->ixrefpara) {
					if (refstart)
						WriteIndexEntryRefSep(list);
					else
						WriteIndexRefSep();
				}
				WriteOneIndexRefItem(ref, list, gpp, lev);
			}
			refstart = false;
		}
		if (list->ixrefpara) {
			cp = new DCctl(imnem, 2, 117, 2, 7);
			gpp->add(*cp);
		}
	}

	if (!list->ixrefpara)
		EndPara();


	// write the real subs
	if (item->subcnt) {
		cp = new DCctl(imnem, 2, 117, 2, 10);
		gpp->add(*cp);
		DCnlist *sp = &item->subsorts;
		long pos = 0;
		DCixitem *sub = NULL;
		while ((sp = sp->next) != NULL) {
			pos = sp->id;
			sub = (DCixitem *) (item->subs.find(pos));
			if (sub)
				WriteOneIndexListItem(sub, list, gpp, lev + 1);
		}
		cp = new DCctl(imnem, 2, 117, 2, 11);
		gpp->add(*cp);
	}

	cp = new DCctl(imnem, 2, 117, 2, 5);
	gpp->add(*cp);
	TextGroup = lastgp;
}


void 
XMLrd::WriteOneIndexRefItem(DCixref *ref, DCixlist *list,
	                          DCgroup *gp, long lev)
{
	DCctl *cp = NULL;

	if (list->ixrefpara) {
		cp = new DCctl(imnem, 2, 117, 2, 8);
		gp->add(*cp);
		char *form = list->refforms.find(lev);
		if (!form)
			form = list->refform;
		StartPara();
		SetFormat(form);
	}

	WriteOneIndexRefContent(ref, list, gp, lev);

	if (list->ixrefpara) {
		EndPara();
		cp = new DCctl(imnem, 2, 117, 2, 9);
		gp->add(*cp);
	}
}


void 
XMLrd::WriteOneIndexRefRange(DCixref *ref, DCixref *eref,
	                           DCixlist *list, DCgroup *gp, long lev)
{
	DCctl *cp = NULL;

	if (list->ixrefpara) {
		cp = new DCctl(imnem, 2, 117, 2, 8);
		gp->add(*cp);
		char *form = list->refforms.find(lev);
		if (!form)
			form = list->refform;
		StartPara();
		SetFormat(form);
	}

	WriteOneIndexRefContent(ref, list, gp, lev);

	// in HTML just use start; in RTF, use full range
	if (!list->ixrngstart) {
		WriteIndexRangeSep();
		WriteOneIndexRefContent(eref, list, gp, lev);
	}

	if (list->ixrefpara) {
		EndPara();
		cp = new DCctl(imnem, 2, 117, 2, 9);
		gp->add(*cp);
	}
}


void 
XMLrd::WriteOneIndexRefContent(DCixref *ref, DCixlist *list,
	                             DCgroup *gpp, long lev)
{
	DCgroup *lasttext = TextGroup;

	// use IndexXref format, so Word gets pages and HTML gets titles
	DCgroup *gp = new DCgroup(DCctl(group, 2, 10, 7));
	gpp->add(*gp);
	TextGroup = gp;

	long fnum = XrefFormNameDefs.getitem(list->xrefform);
	if (!fnum) {
		char *format = NULL;
		XrefFormNameDefs.add(list->xrefform, (fnum = ++XrefFormDefCount));
		if ((format = SubFormatsFile->GetXrefFmtForm(list->xrefform)) == NULL)
			format = GetDefaultXrefFormatDef(list->xrefform);
		XrefFormDefs.add(format, fnum);
	}

	DCctl *cp = new DCctl(imnem, 2, 96, 3, fnum);
	TextGroup->add(*cp);

	cp = new DCctl(ilong, 2, 112, 2, ++ItemID);
	TextGroup->add(*cp);

	DCelem *etinfo = NULL;

	if (ref->map) {
		DCmapelem *rel = (DCmapelem *) ref->refelem;
		DCtopic *top = NULL;
		if (rel) {
			top = rel->reftop;
			while (!top
			 && rel->crefnew) {
				rel = rel->crefnew;
				top = rel->reftop;
			}
			while (!top
			 && rel->parent) {
				rel = (DCmapelem *) rel->parent;
				top = rel->reftop;
			}
		}
		if (top)
			etinfo = (DCelem *) top->elems->next->next->vp;
		else
			etinfo = NULL;
	}
	else
		etinfo = (DCelem *) ref->refelem;

	char *id = ref->refid;

	if (etinfo) {
		if (etinfo->elemuid)
			id = etinfo->elemuid;
		else if (etinfo->parent
		 && etinfo->parent->elemuid)
			id = etinfo->parent->elemuid;
	}

	char *txt = NULL;
	if (etinfo
	 && etinfo->elemtref
	 && ((txt = (etinfo->elemtref)->getText()) != NULL)) {
		cp = new DCctl(etext, 2, 96, 6);
		cp->ex(txt, strlen(txt));
		TextGroup->add(*cp);
	}

	cp = new DCctl(ename, 2, 96, 5);
	DCtopic *top = NULL;
	if (ref->refelem
	 && (((DCelem *) (ref->refelem))->eltyp == esmap))
		top = ((DCmapelem *) (ref->refelem))->reftop;
	else
		top = ((DCtopelem *) (ref->elem))->topic;
	char *fname = NULL;
	if (top
	 && top->part
	 && ((fname = DCLPartFileNames.find(top->part)) != NULL))
		cp->ex(fname, strlen(fname));
	else
		cp->ex(MainDocName, strlen(MainDocName));
	if (IsRtf)
		cp->maj((uns) 0);  // remove it
	TextGroup->add(*cp);

	if (!id)
		id = ref->topid;
	cp = new DCctl(etext, 2, 96, 2);
	cp->ex(id);
	TextGroup->add(*cp);

	// insert content into this group
	DCgroup *cgp = new DCgroup(DCctl(group, 2, 98, 4));
	TextGroup->add(*cgp);

	char *fcp = GetXrefFormat(list->xrefform);
	TextGroup = cgp;

	if (etinfo)
		SetXrefContent(fcp, etinfo, etinfo);
	else
		PutTextStrCtl("reference", 10);

	TextGroup = lasttext;
}


void 
XMLrd::WriteOneIndexSeeItem(DCixitem *item, DCixlist *list,
	                          DCgroup *gp, long lev)
{
	char *form = list->itemforms.find(lev);
	if (!form)
		form = list->itemform;
	StartPara();
	SetFormat(form);

	// item->id is ref for link
	DCctl *cp = new DCctl(etext, 2, 112, 3);
	if (DCLPartFile)
		cp->ex(IndexListPartName, strlen(IndexListPartName));
	else
		cp->ex(MainDocName, strlen(MainDocName));
	TextGroup->add(*cp);
	cp = new DCctl(etext, 2, 112, 4);
	cp->ex(item->id, strlen(item->id));

	TextGroup->add(*cp);

	//if (UMXL)
	//	PutTextStrCtl(item->title);
	//else {
		if (item->itype == ixsee)
			WriteIndexSeeItem(TextGroup, IndexSeeStart);
		else if (item->itype == ixalso)
			WriteIndexSeeItem(TextGroup, IndexSeeAlsoStart);

#ifdef UDOC
		//if (UMXL) {
			if (item->gstitle)
				item->gstitle->writeIdxTitle(TextGroup);
			else if (item->stitle) {
				char *colon = NULL;
				char *txt = item->stitle;
				while (((colon = strchr(txt, ':')) != NULL)
				 && ((colon == txt)
					|| (*(colon - 1) != '\\'))
				 && (*(colon + 1) != ' ')) {
					if (colon > txt)
						PutTextStrCtl(txt, colon - txt);
					PutTextStrCtl(", ");
					txt = colon + 1;
				}
				if (*txt)
					PutTextStrCtl(txt);
			}
			else if (item->gtitle)
				item->gtitle->writeIdxTitle(TextGroup, true);
			else 
				PutTextStrCtl(item->title);
		//}
#endif
#ifdef DITA
		//else {
			if (item->gtitle)
				item->gtitle->writeIdxTitle(TextGroup);
			else 
				PutTextStrCtl(item->title);
		//}
#endif
		//PutTextStrCtl((UMXL && item->stitle) ? item->stitle : item->title);

		if (item->itype == ixsee)
			WriteIndexSeeItem(TextGroup, IndexSeeEnd);
		else if (item->itype == ixalso)
			WriteIndexSeeItem(TextGroup, IndexSeeAlsoEnd);
	//}

	EndPara();
}


char *XMLrd::IndexRefSep = ", ";
char *XMLrd::IndexRefStartSep = " ";
char *XMLrd::IndexRangeSep = "-";


void 
XMLrd::WriteIndexEntryRefSep(DCixlist *list)
{
	if (list->uselead) {
		DCctl *cp = NULL;
		cp = new DCctl(imnem, 2, 117, 2, 13);
		TextGroup->add(*cp);
	}
	else if (IndexRefStartSep)
		PutTextStrCtl(IndexRefStartSep, strlen(IndexRefStartSep));
}


void 
XMLrd::WriteIndexRefSep(void)
{
	if (IndexRefSep)
		PutTextStrCtl(IndexRefSep, strlen(IndexRefSep));
}


void 
XMLrd::WriteIndexRangeSep(void)
{
	if (IndexRangeSep)
		PutTextStrCtl(IndexRangeSep, strlen(IndexRangeSep));
}



// processing for <abbreviated-form>


DCnlist XMLrd::GlossAbbrList;
long XMLrd::GlossAbbrCount = 0;

char *XMLrd::GlossResetElem = NULL;
DCnlist XMLrd::GlossResetElems;
npair XMLrd::GlossResetElemsInit[] = {
	{ 1, "document"},
	{ 2, "part"},
	{ 3, "chapter"},
	{ 4, "head1"},
	{ 5, "head2"},
	{ 6, "head3"},
	{ 7, "head4"},
	{ 8, "head5"},
	{ 9, "head6"},
	{ 10, "topic"},
	NULL
};

long XMLrd::GlossResetLevel = 10;
bool XMLrd::AcronymInTitle = true;  // use acro, not surface form, in title element
	// but do not add it to the list, sso surface form is used for next instance
	// if false, use surface form if first, as usual
char *XMLrd::TmarkResetElem = NULL;
long XMLrd::TmarkResetLevel = 10;


void 
XMLrd::InitGlossAbbr(void)
{
	long lev = 0;

	GlossResetElems.init(GlossResetElemsInit);
	if (GlossResetElem
	 && ((lev = GlossResetElems(GlossResetElem)) != 0))
		GlossResetLevel = lev;
	if (TmarkResetElem
	 && ((lev = GlossResetElems(TmarkResetElem)) != 0))
		TmarkResetLevel = lev;
}


void 
XMLrd::ProcGlossAbbr(void)
{
	if (InTitleElem
	 && AcronymInTitle)
		CurrElemInfo->abbr = abacron;
	else if (!GlossAbbrList(CurrElemInfo->keyref)) {  // first instance
		GlossAbbrList.addnew(CurrElemInfo->keyref, ++GlossAbbrCount);
		// use long form, SurfaceForm or title
		CurrElemInfo->abbr = abfirst;
	}
	else   // in list, use Acronym
		CurrElemInfo->abbr = abacron;
}


void 
XMLrd::ResetGlossAbbr(long level)
{
	if (level) {
#ifdef UDOC
		//if (UMXL) {
			if (GlossResetLevel >= level)
				XMLud::AbbrResetUse();
			if (TmarkResetLevel >= level)
				XMLud::TmarkResetUse();
		//}
		//else
#endif
#ifdef DITA
		if (GlossResetLevel >= level) {
			GlossAbbrList.empty();
			GlossAbbrCount = 0;
		}
#endif
	}
}


void 
XMLrd::FindGlossAbbrReset(DCelem *el)
{
	long erst = 0;
	DCmapelem *mel = NULL;
	DCtopelem *tel = NULL;

	// called for topicrefs and topic roots
	if (el->eltyp == esmap) {  // part, chapter, appendix, topichead, topicref
#if 0
		// if not first child of parent, cannot be starting a level
		if (!el->parent
		 || !IsFirstChild(el)) {
			el->abreset = 0;
			return;
		}
#endif
		mel = (DCmapelem *) el;
		if (!mel->parent) { // root map
			if (el->abreset)
				erst = el->abreset;  // keep current setting
			else
				erst = 1;
		}
		else {
			erst = mel->parent->abreset;  // minimum value is parent's
#ifdef UDOC
			//if (UMXL) {
				if ((erst > 1)
				 && !strcmp(mel->parent->elemname, "map"))
					erst += mel->mlevel - ((DCmapelem *)(mel->parent))->mlevel;
				else if (!strcmp(el->elemname, "part")
				 || !strcmp(el->elemname, "appendices"))
					erst = 2;
				else if (!strcmp(el->elemname, "chapter")
				 || !strcmp(el->elemname, "appendix")
				 || !strcmp(el->elemname, "preface"))
					erst = 3;
			//}
#endif
#ifdef DITA
			//else {  // DITA
				if ((erst > 1)
				 && !strstr(mel->parent->classatt, " map/map "))
					erst += mel->mlevel - ((DCmapelem *)(mel->parent))->mlevel;
				else if (el->classatt) {  //look up elem class
					if (strstr(el->classatt, " bookmap/part ")
					 || strstr(el->classatt, " bookmap/appendices "))
						erst = 2;
					else if (strstr(el->classatt, " bookmap/chapter ")
					 || strstr(el->classatt, " bookmap/appendix "))
						erst = 3;
				}
			//}
#endif
		}
		el->abreset = erst;
	}
	else if (el->eltyp == estopic) {  // must be topic root
		tel = (DCtopelem *) el;
		mel = tel->topic->mref;
		erst = mel->abreset;
		if (!erst)
			erst = 10;
		el->abreset = erst;
		ResetGlossAbbr(erst);
	}
}


#if 0
bool 
XMLrd::IsFirstChild(DCelem *el)
{
	// true if first child of parent, excluding resource-only children
	// false if resource-only, or if not first non-resource-only child
	if (el->role == roleres)
		return false;

	DClist *sibs = el->parent->children;
	DCelem *sib = NULL;

	while ((sibs = sibs->next) != NULL) {
		if (sibs->id) {
			sib = (DCelem *) sibs->id;
			if (sib == el)  // we're first
				return true;
			else if (sib->role != roleres)
				return false;
		}
	}

	return false;
}
#endif


DCtopelem *
XMLrd::GetGlossSurfaceForm(DCtopelem *einfo)
{
	DCtopelem *body = GetGlossBody(einfo);
	if (!body
	 || !body->children)
		return NULL;
	DClist *kl = body->children;
	DCtopelem *kid = NULL;
	while ((kl = kl->next) != NULL) {
		kid = (DCtopelem *) kl->id;
		if (strstr(kid->classatt, " glossentry/glossSurfaceForm "))
			return kid;
	}
	return NULL;
}


DCtopelem *
XMLrd::GetGlossAcronymForm(DCtopelem *einfo)
{
	DCtopelem *body = GetGlossBody(einfo);
	if (!body
	 || !body->children)
		return NULL;

	DClist *kl = body->children;
	DCtopelem *kid = NULL;
	DClist *al = NULL;
	DCtopelem *alt = NULL;

	while ((kl = kl->next) != NULL) {
		kid = (DCtopelem *) kl->id;
		if (strstr(kid->classatt, " glossentry/glossAlt ")) {
			al = kid->children;
			while ((al = al->next) != NULL) {
				alt = (DCtopelem *) al->id;
				if (strstr(alt->classatt, " glossentry/glossAcronym "))
					return alt;
			}
		}
	}
	return NULL;
}


DCtopelem *
XMLrd::GetGlossBody(DCtopelem *einfo)
{
	if (!einfo
	 || !einfo->children)
		return NULL;
	DClist *kl = einfo->children;
	DCtopelem *kid = NULL;
	while ((kl = kl->next) != NULL) {
		kid = (DCtopelem *) kl->id;
		if (strstr(kid->classatt, " glossentry/glossBody "))
			return kid;
	}
	return NULL;
}




// also allow reset of abbr by PI for one key:
//  <?dtall ResetGlossAbbr="key" ?>
// then remove key from GlossAbbrList



// end of drxmlgn.cpp

