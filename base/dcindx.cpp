// dcindx.cpp is for dcl index classes, JHG
// Copyright (c) 2013 by Omni Systems, Inc.  All rights reserved.

#include "dcglob.h"
#include "dcbase.h"
#include "dclist.h"
#include "dcctlg.h"
#include "dcnode.h"
#include "dcfile.h"
#include "dcfilr.h"
#include "dcfilw.h"
#include "dclog.h"
#include "dcfili.h"
#include "dcindx.h"



/* three starting situations for list creation:
  1. list is in [IndexLists], create it at start with name
     and selectors, but missing elemname, outclass, navtitle
     until booklist elem is seen.
	2. ixref with a class is seen, but class is not in selectors
     for any list. If no [IndexClasses], no list gen use, else
     if list in [IC] that wasn't in [IL], create it.
  3. booklist elem is seen but its list does not exist yet,
     create it with name and all props
*/

// DCixset functions

/* to init in drxml.cpp:
	DCixset::LogEventFunc = (logfunc) LogEvent;
	char *basename = NewName(strlen(CurrFile));
	GetBaseName(CurrFile, NULL, basename);
	DCixset::listset = new DCixset(IniFile, LangFile, basename);
*/


logfunc DCixset::LogEventFunc = NULL;

void
DCixset::LogEvent(logtype lt, unc sev, const char *desc,
	const char *d2, const char *d3, const char *d4, const char *d5)
{
	if (!LogEventFunc)
		return;

	LogEventFunc(lt, sev, desc, d2, d3, d4, d5);
}


DCixset *DCixset::listset = NULL;

int
DCixset::cmpstrs(const char *s1, const char *s2)
{
	return (CompareString(LOCALE_SYSTEM_DEFAULT, 
	                       NORM_IGNORECASE | NORM_IGNOREKANATYPE | NORM_IGNOREWIDTH,
                         s1, -1, s2, -1) - 2);
}


void
DCixset::makelists(bool makedef)
{
	// called from ixset constructor at startup
	char *name = NULL;
	DCixlist *list = NULL;
	DCvlist *firstlist = NULL;

	if (makedef) {
		deflist = new DCixlist("IDX");
		addlist(deflist);
		firstlist = lists.next;
		setuplist(deflist);
	}

	if (!ifile->Section("IndexLists"))
		return;

	// listname = list of selectors
	while (ifile->NextItem()) {
		name = ifile->StrLeft();
		if (listexists(name))
			continue;
		list = new DCixlist(name);
		addlist(list);
		if (ifile->ListRight())
			procsellist(list);
	}

	if (firstlist) // skip def list, already set up
		firstlist=firstlist->next;
	else
		firstlist = lists.next;

	while (firstlist) {
		list = (DCixlist *) firstlist->vp;
		firstlist = firstlist->next;
		setuplist(list);
	}
}


void
DCixset::procsellist(DCixlist *list)
{
	char *sel = NULL;
	char *lastsel = NULL;

	while ((sel = ifile->ListItem()) != NULL) {
		lastsel = sel;
		list->itemsels.add(sel, ++list->itemselcnt);
	}
	if (!list->selclass
	 && (list->itemselcnt == 1))
		list->selclass = lastsel;
}


ixuse
DCixset::checklist(char *cl)
{
	DCixlist *list = NULL;
	bool doent = false;
	bool dogen = false;
	char *opt = NULL;
	DCnlist optlist;
	long opnum = 0;

	// called before DCixref constructor to ensure list exists
	if (!cl
	 || !ifile->Section("IndexClasses"))
		return ixent;  // default if no classes listed
	if (!ifile->Find(cl))
		return ixent;  // this class not listed
	if (ifile->ListRight()) {
		while ((opt = ifile->ListItem()) != NULL) {
			if (!stricmp(opt, "NoIDX"))
				return ixuseno;
			if (!stricmp(opt, "IDX"))
				doent = true;
			else { // must be list name
				dogen = true;
				optlist.add(opt, ++opnum);
			}
		}
	}
	if (dogen) {  // second loop required because it changes Section
		while ((opt = optlist.dequeue()) != NULL) {
			if (!listexists(opt)) {
				list = new DCixlist(opt);
				addlist(list);
				if (ifile->Section("IndexLists")
				 && ifile->Find(opt)
				 && ifile->ListRight())
					procsellist(list);
				setuplist(list);
			}
		}
	}
	if (doent && dogen)
		return ixall;
	if (dogen)
		return ixgen;
	return ixent;
}


void
DCixset::addlist(DCixlist *list)
{
	lists.add(list, ++listcnt);
	listnames.add(list->name, listcnt);
}


DCixlist *
DCixset::listexists(char *name)
{
	long pos = listnames.getitem(name);
	return (DCixlist *) lists.find(pos);
}


char *
DCixset::getname(char *ename, char *oclass)
{
	// make key name from ename and oclass
	char *gkey = NULL;
	char *name = NULL;

	if (makedef
	 && !oclass
	 && !stricmp(ename, "indexlist"))
		name = deflist->name;
	else if (ifile->Section("BookLists")) {
		if (oclass) {
			gkey = NewName(ename, strlen(ename)
															  + strlen(oclass) + 2);
			strcat(gkey, "/");
			strcat(gkey, oclass);
			if (ifile->Find(gkey))
				name = ifile->StrRight();
			DeleteName(gkey);
		}

		if (!name  // no match to name/outputclass
		 && ifile->Find(ename))
			name = ifile->StrRight();
	}

	if (!name) 
		name = NewName(ename);

	return name;
}

void
DCixset::setuplist(DCixlist *list)
{
	long len = strlen(list->name);
	char *selsect = NewName(list->name, len + 8);

	// always added to lists first
	// if (!listexists(list->name))  // not added yet
	//	addlist(list);
	list->setlistsort();

	strcat(selsect, "BList");
	list->setfname(ifile, selsect, basename, fname, fsuf);
	list->setlisttypes(ifile, selsect, compact, rtf,
		                 usehead, uselead, uselets, usetlets);
	list->setlistforms(ifile, selsect, xrform, titleform, tocform);

	//strcpy(selsect + len, "BLItems");
	//list->setselectors(ifile, selsect);

	strcpy(selsect + len, "BLForms");
	list->setitemforms(ifile, selsect, entforms);
;
	strcpy(selsect + len, "BLRefForms");
	list->setrefforms(ifile, selsect, refforms);

	strcpy(selsect + len, "BLText");
	list->settitle(lfile, selsect);
}


DCixlist *
DCixset::findlist(char *sel, bool first)
{
	DCixlist *clist = NULL;
	if (first
	 || !nextlist)
		nextlist = &lists;

	while (nextlist->next) {
		nextlist = nextlist->next;
		clist = (DCixlist *) nextlist->vp;

		if (!sel) {
			if (!clist->selclass
			 && (clist->itemselcnt == 0))
				return clist;  // not selective, use default
		}
		else if (clist->itemselcnt == 0) {  // only one class
			if (clist->selclass
			 && !stricmp(sel, clist->selclass))
				return clist;
		}
		else if (clist->itemsels.getitem(sel))
			return clist;
	}

	nextlist = &lists;
	return NULL;
}



// DCixlist functions

/* to init when indexlist elem without href found::
		DCixlist *list = NULL;
		char *name = DCixset::listset->getname(elem->elemname, elem->outclass);
		if ((list = DCixset::listset->listexists(name)) == NULL) {
			list = new DCixlist(name);
			DCixset::listset->setuplist(list);
		}
		list->setlistprops(elem->outclass, elem->navtitle);
*/

void
DCixlist::setlistsort(sortyp st, mrgtyp mt, void *scmp)
{
	sortby = st;
	merge = mt;

	if (scmp)
		sortcmp = (int (*)(const char *, const char *)) scmp;
	else if (sortby == sortlex)
		sortcmp = &DCixset::cmpstrs;
	else 
		sortcmp = &stricmp;
}


void
DCixlist::setlistprops(char *oc, char *tt)
{
	// can set only when booklist elem found
	oclass = oc;
	title = tt;
}


void
DCixlist::setfname(DCirfile *ifile, char *selsect, char *basename,
	                 char *filename, char *filesuf)
{
	fname = filename;
	suf = filesuf;

	if (ifile->Section(selsect)) {
		if (ifile->Find("FilePath"))
			href = ifile->StrRight();
		if (ifile->Find("FileName"))
			fname = ifile->StrRight();
		if (ifile->Find("Suffix"))
			suf = ifile->StrRight();
		if (ifile->Find("Extension"))
			ext = ifile->StrRight();
		if (ifile->Find("RefID"))
			id = ifile->StrRight();
	}

	if (!id) {
		id = NewName("R", strlen(name) + 2);
		strcat(id, name);
	}

	if (!ext)
		ext = ".htm";

	if (!href) {
		if (fname)
			href = fname;
		else if (suf) {
			href = NewName(basename, strlen(basename)
																		 + strlen(suf) + 5);
			strcat(href, suf);
			strcat(href, ".htm");
		}
		else {
			href = NewName(basename, strlen(basename) 
																		 + strlen(id) + 5);
			strcat(href, id);
			strcat(href, ".htm");
		}
	}
	else {
		fname = NewName(strlen(href));
		GetBaseName(href, NULL, fname);
	}
}


void
DCixlist::setselectors(DCirfile *ifile)
{
	char *name = NULL;
	char *sel = NULL;

	if (itemselcnt
	 || !ifile->Section("IndexLists"))
		return;

	// listname = list of selectors
	if (ifile->Find(name)) {
		if (ifile->ListRight()) {
			while ((sel = ifile->ListItem()) != NULL)
				itemsels.add(sel, ++itemselcnt);
			if (!selclass
			 && (itemselcnt == 1))
				selclass = sel;
		}
	}
}


void
DCixlist::setlisttypes(DCirfile *ifile, char *selsect, bool cf, bool rtf,
	                     bool uh, bool ul, bool ulet, bool utlet)
{
	if (ifile->Section(selsect)
	 && ifile->Find("CompactForm"))
		compact = ifile->BoolRight();
	else
		compact = cf;

	usehead = uh;
	uselead = ul;
	uselets = ulet;
	usetlets = utlet;

	if (ifile->Section(selsect)) {
		if (ifile->Find("UseHeading"))
			usehead = ifile->BoolRight();
		if (ifile->Find("UseLeader"))
			uselead = ifile->BoolRight();
		if (ifile->Find("UseLetters"))
			uselets = ifile->BoolRight();
		if (ifile->Find("UseTopLetters"))
			usetlets = ifile->BoolRight();
	}

	if (compact) {
		ixrefpara = false;
		ixrefentry = true;
		ixrngstart = rtf ? false : true;
	}
	else {
		ixrefpara = true;
		ixrefentry = false;
		ixrngstart = true;
	}
}


void
DCixlist::setitemforms(DCirfile *ifile, char *selsect, DCnlist *defiforms)
{
	long lev = 0;
	char *form = NULL;

	if (ifile->Section(selsect)) {
		// level = format
		while (ifile->NextItem()) {
			lev = atoi(ifile->StrLeft());
			form = ifile->StrRight();
			itemforms.add(form, lev);
			if (lev > maxlevel)
				maxlevel = lev;
		}
	}
	else
		maxlevel = 9;

	for (lev = 1; lev <= 9; lev++) {
		if (!itemforms.find(lev)
		 && ((form = defiforms->find(lev)) != NULL))
			itemforms.add(form, lev);
	}
;
	if (!itemform
	 && (maxlevel < 2))
		itemform = form;
}


void
DCixlist::setrefforms(DCirfile *ifile, char *selsect, DCnlist *defrforms)
{
	long lev = 0;
	char *form = NULL;

	if (ifile->Section(selsect)) {
		// level = format
		while (ifile->NextItem()) {
			lev = atoi(ifile->StrLeft());
			form = ifile->StrRight();
			refforms.add(form, lev);
			if (lev > maxlevel)
				maxlevel = lev;
		}
	}
	else
		maxlevel = 9;

	for (lev = 1; lev <= 9; lev++) {
		if (!refforms.find(lev)
		 && ((form = defrforms->find(lev)) != NULL))
			refforms.add(form, lev);
	}
;
	if (!refform
	 && (maxlevel < 2))
		refform = form;
}


void
DCixlist::setlistforms(DCirfile *ifile, char *selsect,
                       char *xf, char *tf, char *cf)
{
	xrefform = compact ? xf : "IndexTitleXref";
	titleform = tf;
	tocform = cf;

	if (ifile->Section(selsect)) {
		if (ifile->Find("TOCFormat"))
			tocform = ifile->StrRight();
		if (ifile->Find("TitleFormat"))
			titleform = ifile->StrRight();
		if (ifile->Find("ItemFormat"))
			itemform = ifile->StrRight();
		if (ifile->Find("RefFormat"))
			refform = ifile->StrRight();
		if (ifile->Find("XrefFormat"))
			xrefform = ifile->StrRight();
	}
}


void
DCixlist::settitle(DCirfile *lfile, char *selsect)
{
	if (!title  // not set from navtitle
	 && lfile->Section(selsect)) {
		if (lfile->Find("ListTitle"))
			title = lfile->StrRight();
	}
}



void
DCixlist::additem(DCixitem *item)
{
	items.add(item, ++itemcnt);
	itemtitles.add(item->title, itemcnt);
	char *nsort = item->setsort(item->title);
	if (!nsort)
		nsort = item->title;
	if (!item->sortset) {
		item->sort = nsort;
		itemsorts.addsortlex(nsort, itemcnt, sortcmp);
	}
}


DCixitem *
DCixlist::getitem(char *title)
{
	long i = 0;

	if ((i = itemtitles.getitem(title)) != 0)
		return (DCixitem *) items.find(i);

	return NULL;
}

#if 0
void
DCixlist::writeTOC(DCLwfile *df, DCgroup *cgp)
{
	// write TOC entry at cgp
	DCctl *cp = new DCctl(etext, 2, 112, 3);
	cp->ex(DCixset::listset->basename);
	cgp->add(*cp);
	cp = new DCctl(etext, 2, 112, 4);
	cp->ex(id, strlen(id));
	cgp->add(*cp);
	// add title in calling function
}


void
DCixlist::writeDCL(DCLwfile *df, DCgroup *gp)
{
	// write entire list as DCL
	procseeids();


	// write title with title format
	

	// write items
	DCgroup *gpp = new DCgroup();
	gp->add(*gpp);

	if (itemcnt) {
		DCnlist *ip = &itemsorts;
		long pos = 0;
		DCixitem *it = NULL;
		while ((ip = ip->next) != NULL) {
			pos = ip->id;
			it = (DCixitem *) (items.find(pos));
			if (it)
				it->writeitemDCL(this, gpp, 1);
		}
	}
}
#endif


void
DCixlist::procseeids(void)
{
	// level 1 items with titles that match those listed
	// in seeitemtitles and alsoitemtitless need ids
	// to use as targets for the see/also links

	DCixitem *item = NULL;
	DCixitem *sitem = NULL;
	DCixref *ref = NULL;
	DCnlist *see = &seeitemtitles;
	long pos = 0;

	while ((see = see->next) != NULL) {
		if ((pos = findidxitemtitle(see->name)) == 0) {
			DCixset::LogEvent(logwarn, 1, "No index entry found for \"See ", see->name, "\"");
			continue;
		}
		item = (DCixitem *) items.find(pos);
		sitem = (DCixitem *) seeitems.find(see->id);
		// add the id to the seeitem id to use as ref
		if (!item->id) // if item already had ID, use it
			item->id = makeitemid(item->title);
		sitem->id = item->id;
	}

	see = &alsoitemtitles;
	while ((see = see->next) != NULL) {
		if ((pos = findidxitemtitle(see->name)) == 0) {
			DCixset::LogEvent(logwarn, 1, "No index entry found for \"See also ", see->name, "\"");
			continue;
		}
		item = (DCixitem *) items.find(pos);
		sitem = (DCixitem *) alsoitems.find(see->id);
		// add the id to the alsoitem id to use as ref
		if (!item->id) // if item already had ID, use it
			item->id = makeitemid(item->title);
		sitem->id = item->id;
	}
}



long
DCixlist::findidxitemtitle(const char *name)
{
	if (!name
	 || (*name == '\0'))
		return 0;

	long pos = itemtitles.getitem(name);
	if (pos)
		return pos;

	char *nm = NewName(name);
	char *semi = strchr(nm, ';');
	if (semi) {
		*semi = '\0';
		if ((pos = itemtitles.getitem(nm)) != 0) {
			DeleteName(nm);
			return pos;
		}
	}

	char *comma = strchr(nm, ',');
	if (comma) {
		*comma = '\0';
		if ((pos = itemtitles.getitem(nm)) != 0) {
			DeleteName(nm);
			return pos;
		}
	}

	DeleteName(nm);
	return 0; 
}


#if 0

long
DCixlist::findidxitemtitle(const char *name)
{
	long pos = itemtitles.getitem(name);
	if (pos)
		return pos;

	char *nm = NULL;
	char *semi = strchr(name, ';');
	char *comma = strchr(name, ',');
	if (!semi
	 && !comma)
		return 0;

	if (semi
	 && (!comma
	  || (comma
	   && (semi < comma)))) {
		nm = NewName(name, semi - name);
		if ((pos = itemtitles.getitem(nm)) != 0) {
			DeleteName(nm);
			return pos;
		}
		DeleteName(nm);
		return 0; 
	}

	if (!comma)
		return 0;

	nm = NewName(name, comma - name);
	if ((pos = itemtitles.getitem(nm)) != 0) {
		DeleteName(nm);
		return pos;
	}

	while ((comma = strchr(comma + 1, ',')) != NULL) {
		DeleteName(nm);
		nm = NewName(name, comma - name);
		if ((pos = itemtitles.getitem(nm)) != 0) {
			DeleteName(nm);
			return pos;
		}
	}

	DeleteName(nm);
	return 0; 
}

#endif


char *
DCixlist::makeitemid(const char *title)
{
	const char *tp = title;
	char *tid = NewName(strlen(title));

	char *np = tid;
	*np++ = '_';
	while (*tp) {
		if (isalnum(*tp) || (*tp == '_'))
			*np++ = *tp;
		tp++;
	}
	*np = '\0';

	char *nid = NewName(id, strlen(id) + strlen(tid));
	strcat(nid, tid);
	return nid;
}



// DCixitem functions

// items are added automatically when their first reference is seen


void
DCixitem::addsub(DCixitem *sub)
{
	subs.add(sub, ++subcnt);
	subtitles.add(sub->title, subcnt);
	char *nsort = sub->setsort(sub->title);
	if (!nsort)
		nsort = sub->title;
	if (!sub->sortset) {
		sub->sort = nsort;
		subsorts.addsortlex(nsort, subcnt, list->sortcmp);
	}
}


DCixitem *
DCixitem::getsub(char *title)
{
	long i = 0;

	if ((i = subtitles.getitem(title)) != 0)
		return (DCixitem *) subs.find(i);

	return NULL;
}


void
DCixitem::addref(DCixref *ref)
{
	refs.add(ref, ++refcnt);
}


char *
DCixitem::setsort(char *s)
{
	if (!s
	 || !*s
	 || sortset) // only set once
		return NULL; 

	char *nsort = NewName(strlen(s) + 1);
	char *dest = nsort;

	if (itype == ixalso)
		*dest++ = (DCixset::listset->seealsofirst) ? ' ' : '~';
	else if (itype == ixsee)
		*dest++ = ' ';

	if (DCixset::listset->ignorefirst
	 && DCixset::listset->ignore) {
		while (*s
		 && (strchr(DCixset::listset->ignorefirst, *s)
			|| strchr(DCixset::listset->ignore, *s)))
			s++;
	}
	else if (DCixset::listset->ignorefirst) {
		while (*s
		 && strchr(DCixset::listset->ignorefirst, *s))
			s++;
	}

	if (DCixset::listset->ignore) {
		while (*s) {
			if (!strchr(DCixset::listset->ignore, *s))
				*dest++ = *s;
			s++;
		}
	}

	*dest = '\0';
	return nsort;
}


#if 0
void
DCixitem::writeitemDCL(DCixlist *list, DCgroup *gpp, long lev)
{
	// write one item and its subs as DCL

	long pos = 0;
	DCixref *ref = NULL;

	// if id, and not see/also, set id; if see/also, id is ref

	// handle see, see also
	if (itype > ixstd) {

		return;
	}

	// write title in correct format for level
	char *form = list->itemforms.find(lev);
	if (!form)
		form = list->itemform;



	// write refs, unless ref has subs  (ref->subcnt > 0)
	// handle ranges; start gets next end
	// if no end before next start, or before end of list, warning
	// use IndexXref format, so Word gets pages and HTML gets titles
	// use IndexRangeSep for ranges; in HTML just use start

	bool refstart = true;
	long epos = 0;

	for (pos = 1; pos <= refcnt; pos++) {
		ref = (DCixref *) refs.find(pos);
		if (ref->subcnt)
			continue;        // ref is meant for subitems
		if (ref->range == rngstart) {
			bool match = false;
			for (epos = pos + 1; epos <= refcnt; epos++) {
				DCixref *eref = (DCixref *) refs.find(epos);
				if (eref->range == rngend) { // write range
					ref->writerefDCL(gpp);
					writerefrangesep(gpp);
					eref->writerefDCL(gpp);
					match = true;
					break;
				}
				else if (eref->range == rngstart) {  // warn, not ended
					ref->writerefDCL(gpp);
					match = true;
					break;
				}
			}
			if (!match) // no match, warn, write single ref
				ref->writerefDCL(gpp);
		}
		else if (ref->range == rngend)
			continue;
		else {             // normal single entry
			if (!refstart)
				writerefsep(gpp);
			ref->writerefDCL(gpp);
		}
		refstart = false;
	}

	// write the real subs
	if (subcnt) {
		DCnlist *sp = &subsorts;
		long pos = 0;
		DCixitem *sub = NULL;
		while ((sp = sp->next) != NULL) {
			pos = sp->id;
			sub = (DCixitem *) (subs.find(pos));
			if (sub)
				sub->writeitemDCL(list, gpp, lev + 1);
		}
	}
}


void
DCixitem::writerefsep(DCgroup *gpp)
{
	// use [IndexText]IndexRefSeparator between refs

}


void
DCixitem::writerefrangesep(DCgroup *gpp)
{
	// use [IndexText]IndexRangeSeparator between start/end refs

}
#endif


// DCixref functions

/* to init at first ref based on [IndexClasses] if ref outclass is there
	 if outclass isn't, or is mapped to IDX, just make regular index entry
	 Before constructor for ref:
		ixuse ixlistuse = DCixset::listset->checklist(CurrElemInfo->outclass);
		switch (ixlistuse) {
	    // select process for lists for this ref
			case ixuseno:  // skip this entry entirely
			case ixent:    // just make ix marker, no lists (default)
			case ixgen:    // just lists, not index marker
			case ixall:    // lists and index marker
		}
*/

/* For lists, then add references (indexterms) one level at a time
	 DCixref *ref = new DCixref(outclass, level, parentref)
	   outclass determines which lists it goes to for all levels
	   level is 1 up
	   parentref is  preceding level ref, needed if level > 1
	 then ref->settext(title, sortstr)
	   title is text to show
	   sortstr is optional
	 then ref->setprops() for topic and para ids and types
*/

DCixref *DCixref::CurrIXRef;
DCnlist DCixref::IXRefRanges;


void
DCixref::settitle(DCgroup *g, bool conref)
{
	if (title)  // already set
		return;

	gtitle = g;

	char *t = g->getText();
	if (!t)
		return;

	char *tp = title = NewName(strlen(t));
	while (*t) {
		if (*t != '\\')
			*tp++ = *t;
		t++;
	}
	*tp = '\0';

	if (!conref)
		setlistitem();

	if (itype > ixstd)
		setsee(itype);
}


void
DCixref::setstitle(DCgroup *g)
{
	char *t = g->getText();
	if (t) {
		stitle = NewName(t);
		gstitle = g;
	}
}


void
DCixref::setlistitem(void)
{
	// identify item in each containing list
	DCixlist *list = NULL;
	DCixitem *item = NULL;
	DCixitem *subitem = NULL;
	bool newitem = false;
	long pos = 0;
	long subpos = 0;

	if (level == 1) {
		bool first = true;
		while ((list = DCixset::listset->findlist(selclass, first)) != NULL) {
			// found qualifying list
			first = false;
			if ((item = list->getitem(title)) == NULL) {
				item = new DCixitem(title, level, list);
				if (gtitle)
					item->gtitle = gtitle;
			}
			item->addref(this);
			items.add(item, ++itemcnt);
			if (gstitle)
				item->gstitle = gstitle;
			if (stitle)
				item->stitle = stitle;
			if (id)
				item->id = id;
		}
	}
	else if (parent) { // ref to subitem
		for (pos = 1; pos <= parent->itemcnt; pos++) {
			item = (DCixitem *) parent->items.find(pos);
			// see if parent's item already has this subitem
			if ((subitem = item->getsub(title)) == NULL) {
				subitem = new DCixitem(title, level, list, item, this->itype);
				if (gtitle)
					subitem->gtitle = gtitle;
			}
			subitem->addref(this);
			items.add(subitem, ++itemcnt);
			if (gstitle)
				subitem->gstitle = gstitle;
			if (stitle)
				subitem->stitle = stitle;
			if (id)
				subitem->id = id;
			if (parent->itype)  // multiple see/seealso levels
				subitem->itype = parent->itype;
		}
	}
}


void
DCixref::setsortstr(char *s)
{
	if (!s
	 || !stricmp(s, "aaa"))
		return;  // ignore this bullshit

	if (sort)
		return;  // first one wins

	sort = s;

	DCixitem *item = NULL;
	DCixitem *subitem = NULL;
	long pos = 0;
	char *nsort = NULL;

	if (level == 1) {
		for (pos = 1; pos <= itemcnt; pos++) {
			item = (DCixitem *) items.find(pos);
			nsort = item->setsort(sort);
			if (nsort) {
				item->sort = nsort;
				item->sortset = true;
				item->list->itemsorts.remove(item->parentcnt);
				item->list->itemsorts.addsortlex(sort, item->parentcnt, item->list->sortcmp);
			}
		}
	}
	else if (parent) { // check parent ref's items
		for (pos = 1; pos <= parent->itemcnt; pos++) {
			item = (DCixitem *) parent->items.find(pos);
			subitem = item->getsub(title);
			nsort = subitem->setsort(sort);
			if (nsort) {
				subitem->sort = nsort;
				subitem->sortset = true;
				item->subsorts.remove(subitem->parentcnt);
				item->subsorts.addsortlex(sort, subitem->parentcnt, item->list->sortcmp);
			}
		}
	}
}


void
DCixref::setids(void *rel, DCgroup **rt, char *tid,
		            char *rid, bool inmap)
{
	if (refelem)
		return;
	refelem = rel;
	reftgroup = rt;
	topid = tid;
	refid = rid;
	map = inmap;
}


// handle ranges and ixtyps (see/see also)
void
DCixref::setsee(ixtyp i)
{
	DCixitem *item = NULL;
	//itype = i;
	for (long pos = 1; pos <= itemcnt; pos++) {
		item = (DCixitem *) items.find(pos);
		item->itype = i;
		//if (item->level == 1) {
			if (i == ixsee) {
				item->list->seeitems.add(item, ++(item->list->seecnt));
				item->list->seeitemtitles.add(item->title, item->list->seecnt);
			}
			else {
				item->list->alsoitems.add(item, ++(item->list->alsocnt));
				item->list->alsoitemtitles.add(item->title, item->list->alsocnt);
			}
		//}
	}
}


void
DCixref::setrange(rngtyp r, char *rn)
{
	range = r;
	rname = NewName(rn);

	if (r == rngstart) {
		// same range name may be used more than once in doc
		IXRefRanges.add(rname, (long) this);
	}
	else if (r == rngend) {
		DCixref *rstart = NULL;
		long lev = 0;
		if ((rstart = (DCixref *) IXRefRanges.getitem(rname)) != NULL) {
			IXRefRanges.remove((long) rstart);  // remove first instance
			// end is always at level 1, but start isn't necessarily
			rstart->eref = this;
			title = rstart->title;
		}
		else
			DCixset::LogEvent(logwarn, 1, "Index range end before start: ", rname);
	}

}



// end of dcindx.cpp

