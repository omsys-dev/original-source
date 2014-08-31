
// dcelem.cpp is for dcl element classes, JHG
// Copyright (c) 2009 by Omni Systems, Inc.  All rights reserved.

#include "dcglob.h"
#include "dcbase.h"
#include "dclist.h"
#include "dcctlg.h"
#include "dcfile.h"
#include "dcelem.h"


// static variables

etype **DCelem::ElemTypes = NULL;
long DCelem::ElemTypesCount = 0;
char *DCelem::LastClass = NULL;
etype **DCelem::LastEP = NULL;




// static functions


void 
DCelem::SetElemTypes(etype **et, long etc)
{
	ElemTypes = et;
	ElemTypesCount = etc;
}


int __cdecl
DCelem::CompElemTypes(const void *elem1, const void *elem2)
{
	const char *name1 = (*((etype **) elem1))->ename;
	const char *name2 = (*((etype **) elem2))->ename;

	return strcmp(name1, name2);
}



// member functions



bool
DCelem::setType(void)
{
	if (epset)
		return true;

	if (!ElemTypes
	 || !classatt)
		return false;

	// normalize the class attribute

	char *eclass = classatt;
	if ((*eclass == '-')
	 || (*eclass == '+'))
		eclass++;

	while (isspace(*eclass))
		eclass++;

	char *ecend = eclass + strlen(eclass) - 1;

	while (isspace(*ecend))
		ecend--;

	char *nclass = NewName((ecend - eclass) + 1);
	char *nptr = nclass;
	char *eptr = eclass;
	while (eptr <= ecend) {
		if (isspace(*eptr)) {
			*nptr++ = *eptr++;
		  while (isspace(*eptr))  // make spaces single
				eptr++;
		}
		*nptr++ = *eptr++;
	}
	*nptr = '\0';

	etype **ep = NULL;
	bool last = false;

	if (LastClass
	 && !strcmp(LastClass, nclass)) {  // same class as last
		ep = LastEP;                     // use same properties
		last = true;
		DeleteName(nclass);
	}
	else {
		if (LastClass)
			DeleteName(LastClass);
		LastClass = nclass;

		etype *key = new etype;
		key->ename = nclass;

		etype **arr = ElemTypes;
		int cnt = ElemTypesCount;

		ep = (etype **) bsearch(&key, arr, cnt, sizeof(etype *),
																	CompElemTypes);

		if (!ep) {  // not in list, trim class attrs from right
			char *space = NULL;
			while (nptr > nclass) {
				while ((*nptr != ' ')
				 && (nptr > nclass))
					nptr--;             // nptr is at space between parts
				if (nptr > nclass) {  // use classes right to left
					char *elem = NewName(nclass, nptr - nclass);
					key->ename = elem;
					ep = (etype **) bsearch(&key, arr, cnt, sizeof(etype *),
															 CompElemTypes);
					DeleteName(elem);
				}
				if (ep)  // found base class of current class
					break;
				nptr--;  // move off the space
			}
		}
		delete key;
		LastEP = ep;
	}

	if (ep) {  // found, store it
		epcnt = (*ep)->cnt;
		eprop = (*ep)->eprop;
		epset = true;
	}
	else {
		epset = true;  // avoid another failed lookup
		epcnt = 0;
		if (!last)
			return false;
	}
	return true;
}


bool
DCelem::checkType(unl bit, unl neg, bool all)
{
	// the unls are packed with up to 4 bytes

	if (!epset)
		return false;

	unc val = 0;
	unc i = 0;

	while ((val = (unc) (neg & 0xFF)) != 0) {
		for (i = 0; i < epcnt; i++) {
			if (val == eprop[i])
				return false;  // exclusion beats inclusion
		}
		neg >>= 8;
	}

	bool match = false;
	bool matched = false;
	bool mismatched = false;

	while ((val = (unc) (bit & 0xFF)) != 0) {
		match = false;
		for (i = 0; i < epcnt; i++) {
			if (val == eprop[i]) {
				match = true;
				break;
			}
		}
		if (match)
			matched = true;
		else
			mismatched = true;
		bit >>= 8;
	}

	if (all & !mismatched)
		return true;

	if (!all & matched)
		return true;

	return false;
}


void
DCelem::addProp(unc prop)
{
	eprop[epcnt++] = prop;
}


void 
DCelem::setAttrs(const char **atts)
{
	attrnames = new DCnlist;
	attrvals = new DCnlist;
	attrcnt = 0;

	if (!atts)
		return;

	while (*atts) {
		attrnames->addnew(*atts++, ++attrcnt);
		attrvals->addnew(*atts++, attrcnt);
	}
}



void 
DCelem::setAttrCtls(DCgroup *txtgp)
{

	DCctl *cp = NULL;
	const char *name = NULL;
	const char *val = NULL;

	attrtext = txtgp;
	attrgrp = new DCgroup;
	attrnctls = new DCclist;
	attrvctls = new DCclist;
	txtgp->add(*((DCctl *) attrgrp));

	for (long i = 1; i <= attrcnt; i++) {
		cp = new DCctl(etext, 2, 121, 4);
		cp->ex(name = attrnames->find(i));
		attrgrp->add(*cp);
		attrnctls->add(cp, i);

		cp = new DCctl(etext, 2, 121, 5);
		cp->ex(val = attrvals->find(i));
		attrgrp->add(*cp);
		attrvctls->add(cp, i);
	}
}


char *
DCelem::getAttr(const char *name)
{
	long i = attrnames->getitem(name);
	if (i)
		return attrvals->find(i);
	return NULL;
}


void
DCelem::addAttr(const char *name, const char *val)
{
	attrnames->addnew(name, ++attrcnt);
	attrvals->addnew(val, attrcnt);
}


bool 
DCelem::isDescendant(DCelem *child)
{
	if (!children
	 || !children->next)
		return false;

	if (children->find((long) child))
		return true;
	DClist *nxt = children;
	while ((nxt = nxt->next) != NULL) {
		if (((DCelem *)(nxt->id))->isDescendant(child))
			return true;
	}
	return false;
}


bool
DCelem::isAncestor(DCelem *par)
{
	if (!parent)
		return false;
	if (par == parent)
		return true;
	return parent->isAncestor(par);
}


DCelem *
DCelem::getChildName(char *cname)
{
	if (!cname
	 || !*cname
	 || !children
	 || !children->next)
		return NULL;

	DClist *nxt = children;
	DCelem *ep = NULL;
	char *en = NULL;

	while ((nxt = nxt->next) != NULL) {
		ep = (DCelem *)(nxt->id);
		en = ep->elemname;
		if (en && !strcmp(cname, en))
			return (DCelem *)(nxt->id);
	}

	nxt = children;
	while ((nxt = nxt->next) != NULL) {
		ep = (DCelem *)(nxt->id);
		if ((ep = ep->getChildName(cname)) != NULL)
			return ep;
	}

	return NULL;
}



DCelem *
DCelem::getDerivedChild(char *cl, bool special)
{
	if (!cl
	 || !*cl
	 || !children
	 || !children->next)
		return NULL;

	DClist *nxt = children;
	DCelem *ep = NULL;
	char *ec = NULL;

	// look for children specialized from this

	while ((nxt = nxt->next) != NULL) {
		ep = (DCelem *)(nxt->id);
		ec = ep->classatt;
		if (ec
		 && !_strnicmp(cl, ec + 2, special ? strlen(cl) : strlen(ec + 2)))
			return (DCelem *)(nxt->id);
	}

	nxt = children;
	while ((nxt = nxt->next) != NULL) {
		ep = (DCelem *)(nxt->id);
		if ((ep = ep->getDerivedChild(cl, special)) != NULL)
			return ep;
	}

	return NULL;
}


void
DCelem::setKeyrefs(const char *kref, const char *ckref)
{
	char *slash = NULL;

	if (kref) {
		if ((slash = strchr(kref, '/')) == NULL)
			keyref = NewName(kref);
		else {
			keyref = NewName(kref, slash - kref);
			keyeid = NewName(slash + 1);
		}
	}
	if (ckref) {
		if ((slash = strchr(ckref, '/')) == NULL)
			conkeyref = NewName(ckref);
		else {
			conkeyref = NewName(ckref, slash - ckref);
			conkeyeid = NewName(slash + 1);
		}
	}
}


// conref functions

DCcnref::DCcnref(long num, cnrftyp typ, DCvlist &lst, DCelem *el) :
 cnum(num), conreftype(typ) 
{
	lst.add(this, num);
	href = &el->conref;
	endref = &el->endref;

	actatt = el->conact;
	if (actatt) {
		if (!strcmp(actatt, "pushreplace"))
			action = ca_replace;
		else if (!strcmp(actatt, "pushbefore"))
			action = ca_before;
		else if (!strcmp(actatt, "pushafter"))
			action = ca_after;
		else if (!strcmp(actatt, "pushatstart"))
			action = ca_start;
		else if (!strcmp(actatt, "pushatend"))
			action = ca_end;
		else if (!strcmp(actatt, "mark"))
			action = ca_mark;
	}

	xrname = el->GetDocName();

	el->conrefnum = num;
	el->conreftype = typ;
}




// end of dcelem.cpp

