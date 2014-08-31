// dcctlg.cc is the dcl ctl group class for DCL readers, JHG, 12/14/96
// Copyright (c) 1993-1996 by Omni Systems, Inc.  All rights reserved.

#include "dcglob.h"
#include "dcbase.h"
#include "dclist.h"
#include "dcctlg.h"


// display list item class
#if 0

void
DCitem::insert(DCctl *nc, DCctl *oc)
{
  if (!next)
    return;
  if (next->ctl == oc)
    next = new DCitem(nc, next);
}

void
DCitem::replace(DCctl *nc, DCctl *oc)
{
  if (!next)
    return;
  if (next->ctl == oc)
    next->ctl = nc;
}

void
DCitem::write(DCfile& wf)
{
  if (ctl)
    ctl->write(wf);
}
#endif

// group class

DCgroup::~DCgroup()
{
  DCitem* nextip = NULL;
  for (DCitem* ip = first; ip != NULL; ip = nextip) {
    nextip = ip->next;
    delete ip;
  }
}


void 
DCgroup::empty(void)
{
	DCitem* nextone;

  for (DCitem *that = first; that; that = nextone) {
		nextone = that->next;
		that->ctl = NULL;
		that->next = NULL;
		delete that;
	}
	first = last = NULL;
}


void
DCgroup::add(DCctl& c)
{
  if (!first)
    last = (first = new DCitem(&c));
  else if (last)
    last = (last->next = new DCitem(&c));
	else
		last = (first->next = new DCitem(&c));
}


void
DCgroup::merge(DCgroup& g, bool del)
{
  if (&g == NULL)
    return;

  if (!first)
    first = g.first;
  else
    last->next = g.first;
  if (g.last)
    last = g.last;

	if (del) {
		g.first = g.last = NULL;
	  delete &g;
	}
}

#if 0
DCgroup::DCgroup(DCgroup& g)
{
  if (&g != NULL) {
		dt(g.dt());
		maj(g.maj());
		min(g.min());
		dat(g.dat());
		siz(g.siz());
	}

  if ((&g != NULL)
	 && g.first) {
		DCitem *ni = NULL;
		last = first = new DCitem((ni = g.first)->ctl);
		if ((last->ctl->dt() == group)
		 && !((last->ctl->maj() == 2)
		   && (last->ctl->min() == 98)
		   && (last->ctl->dat() == 4)))
			last->ctl = new DCgroup(* (DCgroup *) last->ctl);

		while (ni->next && ni->next->ctl) {
			last = last->next = new DCitem((ni = ni->next)->ctl);
			if ((last->ctl->dt() == group)
			 && !((last->ctl->maj() == 2)
				 && (last->ctl->min() == 98)
				 && (last->ctl->dat() == 4)))
				last->ctl = new DCgroup(* (DCgroup *) last->ctl);
		}
	}
	else
		last = first = NULL;
}
#endif


DCgroup::DCgroup(DCgroup& g, DCilist *gmap)
{
	// gmap maps the previous group to the new one for conrefs

  if (&g != NULL) {
		dt(g.dt());
		maj(g.maj());
		min(g.min());
		dat(g.dat());
		siz(g.siz());
	}

  if ((&g != NULL)
	 && g.first) {
		DCitem *ni = NULL;
		last = first = new DCitem((ni = g.first)->ctl);
		if ((last->ctl->dt() == group)
		 && !((last->ctl->maj() == 2)
		   && (last->ctl->min() == 98)
		   && (last->ctl->dat() == 4))) {
			last->ctl = new DCgroup(* (DCgroup *) last->ctl, gmap);
			gmap->add((long) last->ctl, (long) ni->ctl);
		}

		while (ni->next && ni->next->ctl) {
			last = last->next = new DCitem((ni = ni->next)->ctl);
			if ((last->ctl->dt() == group)
			 && !((last->ctl->maj() == 2)
				 && (last->ctl->min() == 98)
				 && (last->ctl->dat() == 4))) {
				last->ctl = new DCgroup(* (DCgroup *) last->ctl, gmap);
				gmap->add((long) last->ctl, (long) ni->ctl);
			}
		}
	}
	else
		last = first = NULL;
}

//		 && !((last->ctl->maj() == 2)
//			 && (last->ctl->min() == 96)
//			 && (last->ctl->dat() == 3))
//		 && !((last->ctl->maj() == 2)
//			 && (last->ctl->min() == 71)
//			 && (last->ctl->dat() == 1))
//		 && !((last->ctl->maj() == 2)
//			 && (last->ctl->min() == 71)
//			 && (last->ctl->dat() == 2))
//		 && !((last->ctl->maj() == 2)
//			 && (last->ctl->min() == 1)
//			 && (last->ctl->dat() == 1))
//		 && !((last->ctl->maj() == 2)
//			 && (last->ctl->min() == 80))
//		 && !((last->ctl->maj() == 1)
//			 && (last->ctl->min() == 30)))

//			 && !((last->ctl->maj() == 2)
//				 && (last->ctl->min() == 96)
//				 && (last->ctl->dat() == 3))
//			 && !((last->ctl->maj() == 2)
//				 && (last->ctl->min() == 71)
//				 && (last->ctl->dat() == 1))
//			 && !((last->ctl->maj() == 2)
//				 && (last->ctl->min() == 71)
//				 && (last->ctl->dat() == 2))
//			 && !((last->ctl->maj() == 2)
//				 && (last->ctl->min() == 1)
//				 && (last->ctl->dat() == 1))
//			 && !((last->ctl->maj() == 2)
//				 && (last->ctl->min() == 80))
//			 && !((last->ctl->maj() == 1)
//				 && (last->ctl->min() == 30)))

DCgroup& 
DCgroup::operator= (DCgroup& g)
{ 
  if ((&g == NULL)
	 || !g.first)
    return *this;

	if (first)
		empty();

	DCitem *ni = NULL;
	DCctl *c = NULL;
	last = first = new DCitem(&(*(c = new DCctl()) = *((ni = g.first)->ctl)));

	while (ni->next && ni->next->ctl)
		last = last->next = new DCitem(&(*(c = new DCctl()) = *((ni = ni->next)->ctl)));

	return *this;
}


void
DCgroup::insert(DCctl& nc)
{
  first = new DCitem(&nc, first);
}


bool
DCgroup::insert(DCctl& nc, DCctl& oc)
{
  if (!first
	 || (first->ctl == &oc)) {
    first = new DCitem(&nc, first);
    return true;
  }

  for (DCitem* ip = first; ip->next != NULL; ip = ip->next) {
    if (ip->next->ctl == &oc) {
      ip->next = new DCitem(&nc, ip->next);
      return true;
    }
		if ((ip->ctl->dt() == group)
		 && (ip->ctl->maj() == 0)
		 && (ip->ctl->min() == 0)
		 && (ip->ctl->dat() == 0)
		 && (((DCgroup *) ip->ctl)->first != 0)) {
			if (((DCgroup *) ip->ctl)->insert(nc, oc))
				return true;
		}
  }
	return false;
}

void
DCgroup::insert(DCgroup& g)
{
  if (&g == NULL)
    return;

	g.last->next = first;
	first = g.first;

  g.first = g.last = NULL;
  delete &g;
}

bool
DCgroup::replace(DCctl& nc, DCctl& oc, DCitem **nxt)
{
	if (nxt)
		*nxt = NULL;

  if (!first) {
    first = new DCitem(&nc);
    return true;
  }
  
  for (DCitem* ip = first; ip != NULL; ip = ip->next) {
    if (ip->ctl == &oc) {
      ip->ctl = &nc;
			if (nxt)
				*nxt = ip->next;
      return true;
    }
		if ((ip->ctl->dt() == group)
		 && (ip->ctl->maj() == 0)
		 && (ip->ctl->min() == 0)
		 && (ip->ctl->dat() == 0)
		 && (((DCgroup *) ip->ctl)->first != 0)) {
			if (((DCgroup *) ip->ctl)->replace(nc, oc, nxt))
				return true;
		}
#if 0
		if (!ip->next) {
			last = ip->next = new DCitem(&nc);
			break;
		}
#endif
  }

	return false;
}

void
DCgroup::write(DCLwfile& wf, long lev)
{
	if (lev > 32)
		return;

  this->DCctl::write(wf, lev + 1);

  for (DCitem* ip = first; ip != NULL; ip = ip->next) {
    if (ip->ctl != NULL)
      (ip->ctl)->write(wf, lev + 1);
  }

  if (this->dt() == group) {
    this->dt(endgroup);
    this->DCctl::write(wf, lev + 1);
    this->dt(group);
  }
}


void 
DCgroup::execu(void (*e)(DCctl*))
{ 
	(*e)((DCctl *)this); 
	for (DCitem *ip = first; ip != NULL; ip = ip->next) {
		if (ip->ctl != NULL) 
			(*e)(ip->ctl); 
	} 
}


void 
DCgroup::execwf(void (*e)(DCctl*, DCLwfile&), DCLwfile& wf)
{ 
	(*e)((DCctl *)this, wf); 
	for (DCitem *ip = first; ip != NULL; ip = ip->next) {
		if (ip->ctl != NULL) 
			(*e)(ip->ctl, wf); 
	} 
}


char *
DCgroup::getText(void)
{
	// extract plain text from ctls in group
	if (!this
	 || ((maj() == 2)
	  && (min() == 71))
	 || !first
	 || !first->ctl)
		return NULL;

	short textlen = getTextLen(0);
	char *tbuf = NewName(textlen + 1);
	getTextStrs(tbuf, 0);
	long last = strlen(tbuf) - 1;
	if (tbuf[last] == ' ')
		tbuf[last] = '\0';
	if (*tbuf == ' ')
		tbuf++;
	return tbuf;
}


void 
DCgroup::getTextStrs(char *tbuf, long lev)
{
	unc ubuf[5] = "";
	DCitem *ip = NULL;

	if (!this
	 || ((maj() == 2)
	  && ((min() == 71)
	   //|| (min() == 110)
	   || (min() == 112)))
	 || !first
	 || !first->ctl
	 || (lev > 128))
		return;

	// append text to buffer, using UTF-8 for imnem as needed
	for (ip = first; ip && ip->ctl; ip = ip->next) {
		if (ip->ctl->dt() == group)
			((DCgroup *) ip->ctl)->getTextStrs(tbuf, lev + 1);
		else if ((ip->ctl->maj() == 2)
		 && (ip->ctl->min() == 11)
		 && (ip->ctl->dat() == 2)) 
			strcat(tbuf, " ");
		else if ((ip->ctl->maj() == 2)
		 && (ip->ctl->min() == 10)) {
			switch (ip->ctl->dt()) {
				case etext:
					strncat(tbuf, (char *) ip->ctl->ex(), ip->ctl->siz());
					break;

				case imnem:
					if (ip->ctl->siz() == 0xA0)
						strcat(tbuf, " ");
					else {
						::UTF8(ip->ctl->siz(), ubuf);
						strcat(tbuf, (char *) ubuf);
						ubuf[0] = '\0';
					}
					break;

				default:
					break;		
			}
		}
	}
}


short 
DCgroup::getTextLen(long lev)
{
	short textlen = 0;
	DCitem *ip = NULL;

	if (!this
	 || ((maj() == 2)
	  && ((min() == 71)
	   //|| (min() == 110)
	   || (min() == 112)))
	 || !first
	 || !first->ctl
	 || (lev > 128))
		return 0;

	// for each text ctl, get length; for imnem, allow 4 bytes
	for (ip = first; ip && ip->ctl; ip = ip->next) {
		if (ip->ctl->dt() == group)
			textlen += ((DCgroup *) ip->ctl)->getTextLen(lev + 1);
		else if ((ip->ctl->maj() == 2)
		 && (ip->ctl->min() == 11)
		 && (ip->ctl->dat() == 2)) 
			textlen++;
		else if ((ip->ctl->maj() == 2)
		 && (ip->ctl->min() == 10)) {
			switch (ip->ctl->dt()) {
				case etext:
					textlen += (short) ip->ctl->siz();
					break;

				case imnem:
					if (ip->ctl->siz() == 0xA0)
						textlen++;
					else
						textlen += 4;
					break;

				default:
					break;		
			}
		}
	}
	return textlen;
}


bool
DCgroup::writeIdxTitle(DCgroup *dest, bool firstonly, long lev)
{
	DCitem *ip = NULL;
	char *txt = NULL;
	char *colon = NULL;
	char *start = NULL;
	DCctl *cp = NULL;
	bool skip = false;

	if (!this
	 || ((maj() == 2)
	  && (min() == 71))
	 || !first
	 || !first->ctl
	 || (lev > 16))
		return false;

	// add each ctl of desired ttpes to dest
	for (ip = first; ip && ip->ctl; ip = ip->next) {
		if (ip->ctl->dt() == group) {
			if (!skip
			 && ((DCgroup *) ip->ctl)->writeIdxTitle(dest,  firstonly, lev + 1))
				skip = true;
		}
		else if (ip->ctl->maj() == 2) {
			if (ip->ctl->min() == 10) {  // look for colons
				if (skip)
					continue;;
				switch (ip->ctl->dt()) {
					case etext:
						txt = NewName((char *) ip->ctl->ex(), (short) ip->ctl->siz());
						start = txt;
						while (((colon = strchr(txt, ':')) != NULL)
						 && ((colon == txt)
						  || (*(colon - 1) != '\\'))
						 && (*(colon + 1) != ' ')) {
							if (colon > txt) {
								cp = new DCctl(etext, 2, 10, 1);
								cp->ex(txt, colon - txt);
								dest->add(*cp);
							}
							if (firstonly) {
								skip = true;
								break;
							}
							cp = new DCctl(etext, 2, 10, 1);
							cp->ex(", ", 3);
							dest->add(*cp);
							txt = colon + 1;
						}
						if (skip)
							break;
						if (txt == start)
							dest->add(*(ip->ctl));
						else if (*txt) {
							cp = new DCctl(etext, 2, 10, 1);
							cp->ex(txt);
							dest->add(*cp);
						}
						
						DeleteName(txt);
						break;

					case imnem:
						if (ip->ctl->siz() == ':') {
							if (firstonly) {
								skip = true;
								break;
							}
							cp = new DCctl(etext, 2, 10, 1);
							cp->ex(", ", 3);
							dest->add(*cp);
						}
						else
							dest->add(*(ip->ctl));
						break;

					default:
						break;		
				}
			}
			else if ((ip->ctl->min() == 10) 
			 || (ip->ctl->min() == 15)
			 || (ip->ctl->min() == 82))
				dest->add(*(ip->ctl));
		}
	}
	return skip;
}



// end of dcctlg.cc


