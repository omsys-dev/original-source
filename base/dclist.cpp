
// dclist.cc is for dcl list classes, JHG
// Copyright (c) 1993-97 by Omni Systems, Inc.  All rights reserved.

#include "dcglob.h"
#include "dcbase.h"
#include "dcfile.h"
#include "dclist.h"

#pragma warning( disable : 4244 4514 )


// DClist functions

DClist::DClist(DClist *lp)
{
	this->id = lp->id;
	while (lp->next) {
		lp = lp->next;
		this->add(lp->id);
	}
}


void DClist::init(long *lpt)
{
	for (; *lpt; lpt++)
		add(*lpt);
}

void DClist::add(long i)
{
  for (DClist *that = this; that; that = that->next) {
		if (that->id == i)
			break;
		if (!that->next) {
			that->next = new DClist(i);
			break;
		}
	}
}

DClist *DClist::append(long i)
{
  DClist *that = this;
	while (that->next)
		that = that->next;
	return (that->next = new DClist(i));
}

void DClist::add(DClist *lst)
{
  for (DClist *that = lst; that; that = that->next)
		this->add(that->id);
}

void DClist::insert(long i)
{
	DClist* that = next;
	next = new DClist(i);
	next->next = that;
}

void DClist::insert(DClist *lst)
{
	DClist *that = next;
	DClist *ln = next = lst->next;
	while (ln->next)
		ln = ln->next;
	ln->next = that;
	lst->next = NULL;
}

void DClist::insertafter(long val, long nitem)
{
	DClist *ni = new DClist(nitem);
  for (DClist *that = this; that; that = that->next) {
		if (that->id == val) {
			ni->next = that->next;
			break;
		}
		if (!that->next)
			break;
	}
	that->next = ni;
}

void DClist::insertbefore(long val, long nitem)
{
	DClist *ni = new DClist(nitem);
	if (this->id == val) {
		ni->next = this->next;
		this->next = ni;
		this->id = ni->id;
		ni->id = val;
		return;
	}
  for (DClist *that = this; that; that = that->next) {
		if (that->next
		 && (that->next->id == val)) {
			ni->next = that->next;
			break;
		}
		if (!that->next)
			break;
	}
	that->next = ni;
}

void DClist::remove(long i)
{
  for (DClist *that = this; that; that = that->next) {
		if (that->next
		 && (i == that->next->id)) {
			DClist *del = that->next;
			that->next = del->next;
			del->next = NULL;
			delete del;
			break;
		}
	}
}

bool DClist::replace(long n, long i)
{
  for (DClist *that = this; that; that = that->next) {
		if (that->id == i) {
			that->id = n;
			return true;
		}
		if (!that->next
		 && (n != 0)) {
			that->next = new DClist(n);
			return false;
		}
	}
	return false;
}

bool DClist::substitute(long start, long end, DClist *rep)
{
	DClist *that = this;
	DClist *stitem = NULL;
	DClist *enditem = NULL;

	for ( ; that; that = that->next) {
		if (that->id == start) {
			stitem = that;
			break;
		}
	}

	// if no start item, skip
	if (!stitem)
		return false;

	for ( ; that; that = that->next) {
		if (that->id == end) {
			enditem = that;
			break;
		}
	}

	// if no end item, replace rest of list

	if (!rep
	 || !rep->next) {  // no replacement, delete range
		stitem->next = enditem;
		return true;
	}

	stitem->next = rep = rep->next;

	for ( ; rep; rep = rep->next) {
		if (rep->next == NULL) { // at end
			rep->next = enditem;
			break;
		}
	}
	return true;
}

long DClist::dequeue(void)
{
	if (!next)
		return 0;
	DClist* that = next;
	next = that->next;
	that->next = NULL;
	long rid = that->id;
	delete that;
	return rid;
}

long DClist::pop(void)
{
	long rid;

  for (DClist *that = this; !(that->next); that = that->next) {
		if (!(that->next->next)) {
			rid = that->next->id;
			delete that->next;
			that->next = NULL;
			return rid;
		}
	}
	return 0;
}

void DClist::all(void (*e)(long i))
{
  for (DClist *that = this; that; that = that->next)
    (*e)(that->id);
}

void DClist::writeall(DCwfile *wf, void (*e)(DCwfile *w, long i))
{
  for (DClist *that = this; that; that = that->next)
    (*e)(wf, that->id);
}

DClist *DClist::find(long i)
{
	if (!i)
		return NULL;
  for (DClist *that = this; that; that = that->next) {
    if (that->id == i)
			return that;
	}
	return NULL;
//  return ((i == id) ? true : ((!next) ? false : next->find(i)));
}

long DClist::number(long i)
{
  DClist *that = this;

  while (i-- > 0) {
    if (!that->next)
      return 0;
    else that = that->next;
  }
  return that->id;
}

long DClist::position(long idval)
{
  DClist *that = this;
	long pos = 0;

  while (that) {
    if (that->id == idval)
      return pos;
    that = that->next;
		pos++;
  }
  return 0;
}

bool DClist::fullmatch(DClist *lst)
{
	if (count() != lst->count())
		return false;
  for (DClist *that = this->next; that; that = that->next) {
		if (!lst->find(that->id))
			return false;
	}
	return true;
}

long DClist::count(void)
{
	long val = -1;

  for (DClist *that = this; that; that = that->next)
    val++;
	return val;
}

void DClist::empty(void)
{
	DClist* nextone;

  for (DClist *that = this->next; that; that = nextone) {
		nextone = that->next;
		that->next = NULL;
		delete that;
	}
	this->next = NULL;
}

char *DClist::strlist(void)
{
	long cnt = 0;
	if ((cnt = this->count()) == 0)
		return NULL;

	long val = 0;
	char *valstr = NULL;
	char *vptr = NULL;

	vptr = valstr = NewName((short) cnt * 18);
	while ((val = this->dequeue()) != 0) {
		_ltoa(val, vptr, 10);
		vptr += strlen(vptr);
		*vptr++ = ' ';
		*vptr = '\0';
	}
	*--vptr = '\0';

	return valstr;
}


// DCilist functions


DCilist::DCilist() : id(0), next(NULL)
{
	val = (long) this;
}


DCilist::DCilist(DCilist *lp)
{
	this->id = 0;
	this->val = (long) this;
	if (lp->id)  // not at start, use first item
		this->append(lp->val, lp->id);
	while (lp->next) {
		lp = lp->next;
		this->append(lp->val, lp->id);
	}
}


void DCilist::init(ipair *ip)
{
	for ( ; ip->id; ip++)
		append(ip->val, ip->id);
}

void DCilist::add(long v, long i)
{
  for (DCilist *that = this; that; that = that->next) {
		if (that->next
		 && (v == that->next->val)) { // shadow any with same val
			if (i == that->next->id)    // prevent duplicates
				break;
			DCilist *tmp = that->next;
			that->next = new DCilist(v, i);
			that->next->next = tmp;
			break;
		}
		if (!that->next) {
			that->next = new DCilist(v, i);
			if (!this->id)
				this->val = (long) that->next;
			break;
		}
	}
}

void DCilist::append(long v, long i)
{
	if (!this->id) {
		DCilist *last = (DCilist *) this->val;
		while (last->next)
			last = last->next;
		last->next = new DCilist(v, i);
		this->val = (long) last->next;
	}
	else {  // not starting at beginning
		for (DCilist *that = this; that; that = that->next) {
			// allow duplicates
			if (!that->next) {
				that->next = new DCilist(v, i);
				break;
			}
		}
	}
}

void DCilist::insert(long v, long i)
{
	DCilist *that = this->next;
	this->next = new DCilist(v, i);
	this->next->next = that;
	if (!this->id && !that)
		this->val = (long) this->next;
}

void DCilist::insertat(long v, long i, long at)
{
	DCilist *prev = this;
  for (DCilist *that = this; that; that = that->next) {
		if (that->next) {
			if (at == that->next->id)
				break;
			prev = that->next;
		}
	}
	if (!that) {  // not found, append to end
		that = prev;
		that->next = new DCilist(v, i);
		that->next->next = NULL;
		if (!this->id)
			this->val = (long) that->next;
	}
	else {  // insert before item found
		prev = that->next;
		that->next = new DCilist(v, i);
		that->next->next = prev;
	}
}

void DCilist::replace(long v, long i)
{
  for (DCilist *that = this; that; that = that->next) {
		if (i == that->id) {
			that->val = v;
			break;
		}
		if (!that->next) {
			that->next = new DCilist(v, i);
			if (!this->id)
				this->val = (long) that->next;
			break;
		}
	}
}

void DCilist::remove(long i)
{
  for (DCilist *that = this; that; that = that->next) {
		if (that->next
		 && (i == that->next->id)) {
			DCilist *del = that->next;
			that->next = del->next;
			if (!del->next) {
				if (!this->id)
					this->val = (long) that;
			}
			else
				del->next = NULL;
			delete del;
			break;
		}
	}
}

long DCilist::dequeue(long *ip)
{
	if (!next)
		return NULL;
	DCilist *that = this->next;
	this->next = that->next;
	that->next = NULL;
	long v = that->val;
	if (ip)
		*ip = that->id;
	delete that;
	if (this->val == (long) that)
		this->val = (long) this;
	return v;
}

long DCilist::count(void)
{
	long cnt = 0;

  for (DCilist *that = this->next; that; that = that->next)
    cnt++;
	return cnt;
}

void DCilist::empty(void)
{
	DCilist* nextone;

  for (DCilist *that = this->next; that; that = nextone) {
		nextone = that->next;
		that->next = NULL;
		delete that;
	}
	this->next = NULL;
	this->val = (long) this;
}

long DCilist::number(long i, long *vp)
{
  DCilist *that = this;

  while (i--) {
    if (!that->next)
      return 0;
    else that = that->next;
  }
	*vp = that->val;
  return that->id;
}

void DCilist::all(void (*e)(long v, long i))
{
  for (DCilist *that = this->next; that; that = that->next)
    (*e)(that->val, that->id);
}

void DCilist::write(long i, DCwfile *wf, void (*e)(DCwfile *w, long v))
{
  for (DCilist *that = this->next; that; that = that->next)
    if (that->id == i)
      (*e)(wf, that->val);
}

void DCilist::writeall(DCwfile *wf, void (*e)(DCwfile *w, long v, long i))
{
  for (DCilist *that = this->next; that; that = that->next)
    (*e)(wf, that->val, that->id);
}

long DCilist::find(long i)
{
	if (i) {
		for (DCilist *that = this->next; that; that = that->next) {
			if (that->id == i)
				return that->val;
		}
	}
	return 0;
}

long DCilist::operator()(long v)
{
  for (DCilist *that = this->next; that; that = that->next) {
    if (that->val == v)
			return that->id;
	}
	return 0;
}

long DCilist::getitem(long v)
{
  for (DCilist *that = this->next; that; that = that->next) {
    if (that->val == v)
			return that->id;
	}
	return 0;
}

bool DCilist::match(long i, long v)
{
  for (DCilist *that = this->next; that; that = that->next) {
    if (((that->val == v) || (that->val == -1))
		 && (that->id == i))
			return true;
	}
	return false;
}


// DCvlist functions

long DCvlist::VLset = 128;
long DCvlist::VLnumhash = 512;
long DCvlist::VLnummask = 0x1FF;


DCvlist::DCvlist() : id(0), next(NULL)
{
	vp = new vlinfo;
	((vlinfo *) vp)->last = this;
}


void DCvlist::hashadd(DCvlist *that) {
	vlinfo *vf = (vlinfo *) this->vp;
	if (!vf)
		return;
	if (vf->set)  // add to hash index
		vf->idnx[that->id & VLnummask] = vf->idnx[that->id & VLnummask]->append((long) that);
	else if (++(vf->cnt) > VLset) { // set up hash index
		int i = 0;
		vf->ids = new DClist *[VLnumhash];
		vf->idnx = new DClist *[VLnumhash];
		for (i = 0; i < VLnumhash; i++)
			vf->idnx[i] = vf->ids[i] = new DClist();
		for (DCvlist *vi = this->next; vi; vi = vi->next) {
			vf->last = vi;
			vf->idnx[vi->id & VLnummask] = vf->idnx[vi->id & VLnummask]->append((long) vi);
		}
		vf->set = true;
	}
}


void DCvlist::init(vpair *vpr)
{
	for ( ; vpr->id; vpr++)
		add(vpr->vp, vpr->id);
}

void DCvlist::initdl(vpair *vpr)
{
	DClist *dl = NULL;

	for ( ; vpr->id; vpr++) {
		dl = new DClist;
		dl->init((long *) vpr->vp);
		add(dl, vpr->id);
	}
}

void DCvlist::add(void *v, long i)
{
	if (!this->id) {
		if (this->find(i) == v)
			return;
		DCvlist *last = ((vlinfo *) this->vp)->last;
		while (last->next)
			last = last->next;
		last->next = new DCvlist(v, i);
		((vlinfo *) this->vp)->last = last->next;
		hashadd(last->next);
	}
	else {  // not at beginning		
		for (DCvlist *that = this; that; that = that->next) {
			if (that->next
			 && (i == that->next->id)
			 && (v == that->next->vp))
				break;
			if (!that->next) {
				that->next = new DCvlist(v, i);
				break;
			}
		}
	}
}

void DCvlist::append(void *v, long i)
{
	if (!this->id) {
		DCvlist *last = ((vlinfo *) this->vp)->last;
		while (last->next)
			last = last->next;
		last->next = new DCvlist(v, i);
		((vlinfo *) this->vp)->last = last->next;
		hashadd(last->next);
	}
	else {  // not at beginning		
		for (DCvlist *that = this; that; that = that->next) {
			// allow duplicates
			if (!that->next) {
				that->next = new DCvlist(v, i);
				break;
			}
		}
	}
}


void DCvlist::insert(void *v, long i)
{
	DCvlist* that = this->next;
	this->next = new DCvlist(v, i);
	this->next->next = that;
	hashadd(this->next);
	if (!this->id && !that)
		((vlinfo *) this->vp)->last = this->next;
}

void DCvlist::insertat(void *v, long i, long at)
{
	DCvlist *prev = this;

  for (DCvlist *that = this; that; that = that->next) {
		if (that->next) {
			if (at == that->next->id)
				break;
			prev = that->next;
		}
	}
	if (!that) {  // not found, append to end
		that = prev;
		that->next = new DCvlist(v, i);
		hashadd(that->next);
		that->next->next = NULL;
		if (!this->id
		 && this->vp)
			((vlinfo *) this->vp)->last = that->next;
	}
	else {  // insert before item found
		prev = that->next;
		that->next = new DCvlist(v, i);
		hashadd(that->next);
		that->next->next = prev;
	}
}

void DCvlist::remove(long i)
{
  for (DCvlist *that = this; that; that = that->next) {
		if (that->next
		 && (i == that->next->id)) {
			DCvlist *del = that->next;
			that->next = del->next;
			vlinfo *vf = (vlinfo *) this->vp;
			if (!del->next) {
				if (!this->id)
					vf->last = that;
			}
			vf->cnt--;
			if (vf->set) {
				vf->ids[del->id & VLnummask]->remove((long) del);
				vf->idnx[del->id & VLnummask] = vf->ids[del->id & VLnummask];
			}
			del->next = NULL;
			delete del;
			break;
		}
	}
}

void DCvlist::clear(bool arr)
{
	DCvlist *del = NULL;

  for (DCvlist *that = this->next; that; that = that->next) {
		if (del)
			delete del;
		del = that;
	  if (that->vp) {
			if (arr)
				delete [] that->vp;
			else
				delete that->vp;
			that->vp = NULL;
			that->id = 0;
		}
	}
	this->next = NULL;

	if (!this->id) {
		vlinfo *vf = (vlinfo *) this->vp;
		vf->last = this;
		vf->cnt = 0;
		if (vf->set) { // used hashes
			for (int i = 0; i < VLnumhash; i++)
				(vf->idnx[i] = vf->ids[i])->empty();
		}
	}
}

long DCvlist::count(void)
{
	if (!this->id)
		return ((vlinfo *) this->vp)->cnt;

	long cnt = 0;

  for (DCvlist *that = this->next; that; that = that->next)
    cnt++;
	return cnt;
}

void DCvlist::empty(void)
{
	DCvlist* nextone;

  for (DCvlist *that = this->next; that; that = nextone) {
		if (!that->vp
		 || (that->vp == (void *) 0xdddddddd))
			break;
		nextone = that->next;
		that->next = NULL;
		delete that;
	}
	this->next = NULL;

	if (!this->id) {
		vlinfo *vf = (vlinfo *) this->vp;
		vf->last = this;
		vf->cnt = 0;
		if (vf->set) { // used hashes
			for (int i = 0; i < VLnumhash; i++)
				(vf->idnx[i] = vf->ids[i])->empty();
		}
	}
}

void DCvlist::all(void (*e)(void *v, long i))
{
	(*e)(NULL, 0);
  for (DCvlist *that = this->next; that; that = that->next)
    (*e)(that->vp, that->id);
}

void DCvlist::replace(void *v, long i)
{
	//vlinfo *vf = (!this->id) ? (vlinfo *) this->vp : NULL;
	if (!i)
		return;

  for (DCvlist *that = this; that; that = that->next) {
		if (i == that->id) {
			that->vp = v;
			break;
		}
		if (!that->next) {
			that->next = new DCvlist(v, i);
			hashadd(that->next);
			if (!this->id)
				((vlinfo *) this->vp)->last = that->next;
			break;
		}
	}
}

void *DCvlist::dequeue(void)
{
	if (!this->next)
		return NULL;
	DCvlist *that = this->next;
	this->next = that->next;
	void *v = that->vp;
	if (!this->id
	 && this->vp) {
		vlinfo *vf = (vlinfo *) this->vp;
		vf->cnt--;
		if (vf->last == that)
			vf->last = this;
		if (vf->set) {
			vf->ids[that->id & VLnummask]->remove((long) that);
			vf->idnx[that->id & VLnummask] = vf->ids[that->id & VLnummask];
		}
	}
	that->next = NULL;
	that->vp = NULL;
	delete that;
	return v;
}

void *DCvlist::find(long i, DCvlist **dvl)
{
	if (!i)
		return NULL;

	vlinfo *vf = (vlinfo *) this->vp;
	if (!this->id
	 && vf->set) { // use hashes
		for (DClist *idn = vf->ids[i & VLnummask]->next; idn; idn = idn->next) {
			DCvlist *vi = (DCvlist *) idn->id;
			if (vi->id == i) {
				if (dvl)
					*dvl = vi;
				return vi->vp;
			}
		}
		if (dvl)
			*dvl = NULL;
		return NULL;
	}

  for (DCvlist *that = this; that; that = that->next) {
		if (i == that->id) {
			if (dvl)
				*dvl = that;
			return that->vp;
		}
	}
	if (dvl)
		*dvl = NULL;
	return NULL;
}

long DCvlist::operator()(void *v)
{
  for (DCvlist *that = this; that; that = that->next) {
		if (v == that->vp)
			return that->id;
	}
	return 0;
}

long DCvlist::getitem(void *v)
{
  for (DCvlist *that = this; that; that = that->next) {
		if (v == that->vp)
			return that->id;
	}
	return 0;
}

void DCvlist::execu(long i, void (*e)(void *v))
{
  for (DCvlist *that = this->next; that; that = that->next)
    if ((i == that->id) && that->vp)
      (*e)(that->vp);
}

void DCvlist::write(long i, DCfile *wf, void (*e)(DCfile *w, void *v))
{
  for (DCvlist *that = this->next; that; that = that->next)
    if ((i == that->id) && that->vp)
      (*e)(wf, that->vp);
}

void DCvlist::writeall(DCfile *wf, void (*e)(DCfile *w, void *v))
{
  for (DCvlist *that = this->next; that; that = that->next)
    if (that->vp)
      (*e)(wf, that->vp);
}

void DCvlist::writeallval(DCfile *wf, void (*e)(DCfile *w, void *v, long i))
{
  for (DCvlist *that = this->next; that; that = that->next)
    if (that->vp)
      (*e)(wf, that->vp, that->id);
}


// DCnlist functions

long DCnlist::NLset = 128;
long DCnlist::NLstrhash = 251;
long DCnlist::NLstrmask = 0x5F;
long DCnlist::NLnumhash = 512;
long DCnlist::NLnummask = 0x1FF;


DCnlist::DCnlist() : id(0), next(NULL)
{
	name = (char *) new nlinfo;
	val = (long) this;
}


long DCnlist::hash(const char *nm)
{
	long val = 0;
	if (nm) {
		while (*nm)
			val = ((val * 32) + (*nm++ & NLstrmask)) % NLstrhash;
	}
	return val;
}

void DCnlist::hashadd(DCnlist *that) {
	nlinfo *nf = (nlinfo *) this->name;
	nf->cnt++;
	if (nf->set) { // add to hash index
		nf->hashnx[that->val] = nf->hashnx[that->val]->append((long) that);
		nf->idnx[that->id & NLnummask] = nf->idnx[that->id & NLnummask]->append((long) that);
	}
	else if (nf->cnt > NLset) { // set up hash index
		int i = 0;
		nf->hashes = new DClist *[NLstrhash];
		nf->hashnx = new DClist *[NLstrhash];
		for (i = 0; i < NLstrhash; i++)
			nf->hashnx[i] = nf->hashes[i] = new DClist();
		nf->ids = new DClist *[NLnumhash];
		nf->idnx = new DClist *[NLnumhash];
		for (i = 0; i < NLnumhash; i++)
			nf->idnx[i] = nf->ids[i] = new DClist();
		for (DCnlist *ni = this->next; ni; ni = ni->next) {
			nf->hashnx[ni->val] = nf->hashnx[ni->val]->append((long) ni);
			nf->idnx[ni->id & NLnummask] = nf->idnx[ni->id & NLnummask]->append((long) ni);
		}
		nf->set = true;
	}
}


void DCnlist::init(npair *np)
{
	for ( ; np->id; np++)
		add(np->name, np->id);
}

void DCnlist::clear(void)
{
	DCnlist *del = NULL;

  for (DCnlist *that = this->next; that; that = that->next) {
		if (del)
			delete del;
		del = that;
	  if (that->name && *(that->name))
	    DeleteName((char *) that->name);
	}
	this->next = NULL;

	if (!this->id) {
		nlinfo *nf = (nlinfo *) this->name;
		this->val = (long) this;
		nf->cnt = 0;
		if (nf->set) { // used hashes
			for (int i = 0; i < NLstrhash; i++)
				(nf->hashnx[i] = nf->hashes[i])->empty();
			for (i = 0; i < NLnumhash; i++)
				(nf->idnx[i] = nf->ids[i])->empty();
		}
	}
}

void DCnlist::addnew(const char *n, long i)
{
	if (!n || !*n)
		return;

  add(NewName(n), i);
}

void DCnlist::addlist(DCnlist *nl)
{
	if (!nl || !nl->next)
		return;

	long i = count();
	while ((nl = nl->next) != NULL)
		if (!getitem(nl->name))
			addnew(nl->name, ++i);
}

void DCnlist::addlist(const char *n)
{
	if (!n || !*n)
		return;

	long i = count();
	char *np = (char *) n;
	char *nm = NULL;
	char *end = np;

	while (*end) {
		while ((*end == ' ')
		 || (*end == ',')
		 || (*end == '\t')
		 || (*end == '\n')
		 || (*end == '\r'))
			end++;

		np = end;

		while (!((*end == '\0')
		 || (*end == ' ')
		 || (*end == ',')
		 || (*end == '\t')
		 || (*end == '\n')
		 || (*end == '\r')))
			end++;

		if (end == np)
			break;
		nm = NewName(np, end - np);
		nm[end - np] = '\0';
		add(nm, ++i);
	}
}

void DCnlist::add(const char *n, long i)
{
	if ((!n || !*n) && !i)
		return;

	if (!this->id) {
		DCnlist *last = (DCnlist *) this->val;
		while (last->next)
			last = last->next;
		last->next = new DCnlist(n, i);
		this->val = (long) last->next;
		hashadd(last->next);
	}
	else {  // not at beginning		
		for (DCnlist *that = this; that; that = that->next) {
			if (that->next == NULL) {
				that->next = new DCnlist(n, i);
				break;
			}
		}
	}
}

void DCnlist::addsorted(const char *n, long i)
{
	if (!n || !*n)
		return;

	DCnlist *x = new DCnlist(n, i);

  for (DCnlist *that = this; that; that = that->next) {
		if ((that->next == NULL)
		 || (_stricmp(n, that->next->name) < 0)) {
			x->next = that->next;
			that->next = x;
			hashadd(x);
			if (!this->id && !x->next)
				this->val = (long) x;
			break;
		}
	}
}


void DCnlist::addsortlex(const char *n, long i, void *cmp)
{
	if (!n || !*n)
		return;
	int (*cf)(const char *, const char *) = 
	 (int (*)(const char *, const char *)) cmp;
	if (!cf)
		cf = *_stricmp;

	DCnlist *x = new DCnlist(n, i);

  for (DCnlist *that = this; that; that = that->next) {
		if ((that->next == NULL)
		 || ((*cf)(n, that->next->name) < 0)) {
			x->next = that->next;
			that->next = x;
			hashadd(x);
			if (!this->id && !x->next)
				this->val = (long) x;
			break;
		}
	}
}

void DCnlist::insert(const char *n, long i)
{
	if ((!n || !*n) && !i)
		return;

	DCnlist *that = this->next;
	this->next = new DCnlist(n, i);
	this->next->next = that;
	hashadd(this->next);
	if (!this->id && !that)
		this->val = (long) this->next;
}

void DCnlist::insertat(const char *n, long i, long at)
{
	if ((!n || !*n) && !i)
		return;

	DCnlist *prev = this;
  for (DCnlist *that = this; that; that = that->next) {
		if (that->next) {
			if (at == that->next->id)
				break;
			prev = that->next;
		}
	}
	if (!that) {  // not found, append to end
		that = prev;
		that->next = new DCnlist(n, i);
		hashadd(that->next);
		that->next->next = NULL;
		if (!this->id)
			this->val = (long) that->next;
	}
	else {  // insert before item found
		prev = that->next;
		that->next = new DCnlist(n, i);
		hashadd(that->next);
		that->next->next = prev;
	}
}

void DCnlist::remove(long i)
{
  for (DCnlist *that = this; that; that = that->next) {
		if (that->next
		 && (i == that->next->id)) {
			DCnlist *del = that->next;
			that->next = del->next;
			if (!del->next) {
				if (!this->id)
					this->val = (long) that;
			}
			nlinfo *nf = (nlinfo *) this->name;
			nf->cnt--;
			if (nf->set) {
				nf->hashes[del->val]->remove((long) del);
				nf->hashnx[del->val] = nf->hashes[del->val];
				nf->ids[del->id & NLnummask]->remove((long) del);
				nf->idnx[del->id & NLnummask] = nf->ids[del->id & NLnummask];
			}
			del->next = NULL;
			delete del;
			break;
		}
	}
}

void DCnlist::replace(const char *n, long i)
{
	if (!n || !*n)
		return;

	long h = hash(n);

  for (DCnlist *that = this; that; that = that->next) {
		if (i == that->id) {
			if (!this->id) {
				nlinfo *nf = (nlinfo *) this->name;
				if (nf && nf->set) {
					nf->hashes[that->val]->remove((long) that);
					nf->hashnx[that->val] = nf->hashes[that->val];
					nf->hashnx[h] = nf->hashes[h]->append((long) that);
				}
			}
			else
				assert(0);
			that->name = (char *) n;
			that->val = h;
			break;
		}
		if (!that->next) {
			that->next = new DCnlist(n, i);
			this->hashadd(that->next);
			if (!this->id)
				this->val = (long) that->next;
			break;
		}
	}
}

bool DCnlist::changeid(long n, long i)
{
	if (!n || !i)
		return false;

  for (DCnlist *that = this; that; that = that->next) {
		if (i == that->id)  {
			that->id = n;
			return true;;
		}
	}
	return false;  // id not found
}

char *DCnlist::dequeue(long *ip)
{
	if (!this->next)
		return NULL;
	DCnlist *that = this->next;
	this->next = that->next;
	that->next = NULL;
	const char *nm = that->name;
	if (ip)
		*ip = that->id;
	that->name = NULL;
	nlinfo *nf = (nlinfo *) this->name;
	nf->cnt--;
	if (nf->set) {
		nf->hashes[that->val]->remove((long) that);
		nf->hashnx[that->val] = nf->hashes[that->val];
		nf->ids[that->id & NLnummask]->remove((long) that);
		nf->idnx[that->id & NLnummask] = nf->ids[that->id & NLnummask];
	}
	if (this->val == (long) that)
		this->val = (long) this;
	delete that;
	return (char *) nm;
}

char *DCnlist::concat(void)
{
	long cnt = 0;
	DCnlist *that = NULL;

	for (that = this->next; that; that = that->next) {
		if (that->name)
			cnt += strlen(that->name) + 1;
	}

	if (!cnt)
		return NULL;

	char *str = NewName(cnt + 1);
	char *ptr = str;

  for (that = this->next; that; that = that->next) {
		if (that->name) {
			strcpy(ptr, that->name);
			ptr += strlen(that->name);
			*ptr++ = '\0';
		}
	}
	*ptr++ = '\0';
	return str;
}

long DCnlist::count(void)
{
	if (!this->id)
		return ((nlinfo *) this->name)->cnt;

	long cnt = 0;

  for (DCnlist *that = this->next; that; that = that->next)
    cnt++;
	return cnt;
}

void DCnlist::empty(void)
{
#if 0
	DCnlist* nextone;

  for (DCnlist *that = this->next; that; that = nextone) {
		nextone = that->next;
		that->next = NULL;
		// delete that;  // find the goddam leak someday ****
	}
#endif

	this->next = NULL;
	this->val = (long) this;

	if (!this->id) {
		nlinfo *nf = (nlinfo *) this->name;
		nf->cnt = 0;
		if (nf->set) { // used hashes
			for (int i = 0; i < NLstrhash; i++)
				(nf->hashnx[i] = nf->hashes[i])->empty();
			for (i = 0; i < NLnumhash; i++)
				(nf->idnx[i] = nf->ids[i])->empty();
		}
	}
}

void DCnlist::all(void (*e)(char *n, long i))
{
  for (DCnlist *that = this->next; that; that = that->next)
    (*e)((char *) that->name, that->id);
}

void DCnlist::all(void (*e)(const char *n, long i))
{
  for (DCnlist *that = this->next; that; that = that->next)
    (*e)((char *) that->name, that->id);
}

void DCnlist::writeall(DCwfile *wf, void (*e)(DCwfile *w, char *n, long i))
{
  for (DCnlist *that = this->next; that; that = that->next)
    (*e)(wf, (char *) that->name, that->id);
}

void DCnlist::writeallname(DCwfile *wf, void (*e)(DCwfile *w, char **n, long i))
{
  for (DCnlist *that = this->next; that; that = that->next)
    (*e)(wf, (char **) &(that->name), that->id);
}

char *DCnlist::find(long i, DCnlist **dnl)
{
	if (!i)
		return NULL;
	nlinfo *nf = (nlinfo *) this->name;
	if (!this->id
	 && nf->set) { // use hashes
		for (DClist *idn = nf->ids[i & NLnummask]->next; idn; idn = idn->next) {
			DCnlist *ni = (DCnlist *) idn->id;
			if (ni->id == i) {
				if (dnl)
					*dnl = ni;
				return (char *) ni->name;
			}
		}
		if (dnl)
			*dnl = NULL;
		return NULL;
	}
  for (DCnlist *that = this; that; that = that->next) {
		if (i == that->id) {
			if (dnl)
				*dnl = that->next;
			return (char *) that->name;
		}
	}
	if (dnl)
		*dnl = NULL;
	return NULL;
}

long DCnlist::getnum(const char *n, DCnlist **dnl)
{
	if (!n || !*n)
		return 0;
	long v = hash(n);

  for (DCnlist *that = this->next; that; that = that->next) {
		if ((v == that->val)
		 && (that->name != NULL)
		 && !strcmp(that->name, n)) {
			if (dnl)
				*dnl = that->next;
			return that->id;
		}
	}
	if (dnl)
		*dnl = NULL;
	return 0;
}

long DCnlist::operator()(const char *n, DCnlist **dnl)
{
	if (!n || !*n)
		return 0;
	long v = hash(n);

  for (DCnlist *that = this->next; that; that = that->next) {
		if ((v == that->val)
		 && (that->name != NULL)
		 && !_stricmp(that->name, n)) {
			if (dnl)
				*dnl = that->next;
			return that->id;
		}
	}
	if (dnl)
		*dnl = NULL;
	return 0;
}

long DCnlist::checksuffix(const char *n, DCnlist **dnl)
{
	if (!n || !*n)
		return 0;

	// check in list of suffixes for match to name n
	long ln = strlen(n);
	long li = 0;

  for (DCnlist *that = this->next; that; that = that->next) {
		if (that->name != NULL) {
			li = strlen(that->name);
			if ((li <= ln)
			 && !_stricmp(that->name, n + (ln - li))) {
				if (dnl)
					*dnl = that->next;
				return that->id;
			}
		}
	}
	if (dnl)
		*dnl = NULL;
	return 0;
}

long DCnlist::getitem(const char *n, DCnlist **dnl, bool usehash, bool getnext)
{
	if (!n || !*n)
		return 0;
	long v = hash(n);
	nlinfo *nf = (nlinfo *) this->name;
	DCnlist *last = this;

	if (usehash
	 && !this->id
	 && nf->set) { // use hashes
		for (DClist *hashn = nf->hashes[v]->next; hashn; hashn = hashn->next) {
			DCnlist *ni = (DCnlist *) hashn->id;
			if (ni->name
			 && !_stricmp(ni->name, n)) {
				if (dnl)
					*dnl = ni;
				return ni->id;
			}
		}
		if (dnl)
			*dnl = NULL;
		return 0;
	}
  for (DCnlist *that = this->next; that; that = that->next) {
		last = that;
		if ((v == that->val)
		 && (that->name != NULL)
		 && !_stricmp(that->name, n)) {
			if (dnl) {
				if (getnext)
					*dnl = that->next;
				else
					*dnl = that;
			}
			return that->id;
		}
	}
	if (dnl)
		*dnl = last;
	return 0;
}

long DCnlist::getallwild(const char *n, DCnlist *results)
{
	if (!n
	 || !*n
	 || !results)
		return 0;

	long startidx = results->count();
	long idx = startidx;
	DCnlist *lastmatch = this;
	while (lastmatch->getwild(n, &lastmatch))
		results->addnew(lastmatch->name, ++idx);
	return (idx - startidx);
}


long DCnlist::getallwildval(const char *n, DCnlist *results)
{
	if (!n
	 || !*n
	 || !results)
		return 0;

	DCnlist *lastmatch = this;
	while (lastmatch->getwild(n, &lastmatch))
		results->addnew(lastmatch->name, lastmatch->id);
	return results->count();
}

long DCnlist::getwild(const char *n, DCnlist **dnl)
{
	if (!n || !*n)
		return 0;
  for (DCnlist *that = this->next; that; that = that->next) {
		if ((that->name != NULL)
		 && wildmatch(n, that->name)) {
			if (dnl)
				*dnl = that;
			return that->id;
		}
	}
	if (dnl)
		*dnl = NULL;
	return 0;
}

long DCnlist::getwildrev(const char *n, DCnlist **dnl)
{
	// check for match of n to list containing wildcards
	if (!n || !*n)
		return 0;
  for (DCnlist *that = this->next; that; that = that->next) {
		if ((that->name != NULL)
		 && wildmatch(that->name, n)) {
			if (dnl)
				*dnl = that;
			return that->id;
		}
	}
	if (dnl)
		*dnl = NULL;
	return 0;
}

bool DCnlist::wildmatch(const char *cl, const char *name)
{
	// if wildcard cl matches name, return true
	int nmc, clc;

	if (!name)
		return false;

	while (*name) {
		nmc = (int) (*name & 0xFF);
		clc = (int) (*cl & 0xFF);
		if ((nmc == clc) 
		 || (tolower(nmc) == tolower(clc))) {
			name++;
			cl++;
		}
		else if ((clc == '*') || (clc == '?')) {
			if (clc == '*') {
				clc = (int) (*++cl & 0xFF);
				if (clc == '\0')
					return true;
				while (*++name) {
					if (wildmatch(cl, name))
						return true;
				}
				return false;
			}
			else { // single-char wildcard
				name++;
				cl++;
			}
		}
		else
			return false;
	}

	clc = (int) (*cl & 0xFF);
	if (*name == 0) { // matched full name
		if ((clc == '*')
		 || (clc == '?'))
			clc = (int) (*++cl & 0xFF);
		if (clc == '\0')  // matched full setting
			return true;
	}

	return false;
}


long DCnlist::getfile(const char *n, DCnlist **dnl)
{
	static char nbuf[_MAX_PATH];
	static char abuf[_MAX_PATH];
	nbuf[0] = '\0';

	if (!n || !*n)
		return 0;
	long v = hash(n);

  for (DCnlist *that = this->next; that; that = that->next) {
		if (dnl)
			*dnl = that;
		if (that->name) {
			if ((v == that->val)
			 && (that->name != NULL)
			 && !_stricmp(that->name, n))
				return that->id;
			_fullpath(abuf, that->name, _MAX_PATH);
			if (nbuf[0] == '\0')
				_fullpath(nbuf, n, _MAX_PATH);
			if (!_stricmp(nbuf, abuf))
				return that->id;
		}
	}
	return 0;
}


// DCmlist functions

DClist DCmlist::multpend;
long DCmlist::multcount = 0;
long DCmlist::multpos = 0;


DCmlist::DCmlist(char *n, long i, char *b)
{
	id = i;
	next = NULL;
	name = n;
	branch = b;
	val = hash(name);
}



void DCmlist::addmult(char *n, long i, char *b)
{
	if ((!n || !*n) && !i)
		return;

	if (!this->id) {
		DCnlist *last = (DCnlist *) this->val;
		while (last->next)
			last = last->next;
		last->next = new DCmlist(n, i, b);
		this->val = (long) last->next;
		hashadd(last->next);
	}
	else {  // not at beginning		
		for (DCnlist *that = this; that; that = that->next) {
			if (that->next == NULL) {
				that->next = new DCmlist(n, i, b);
				break;
			}
		}
	}
}

char *DCmlist::findmult(long i, char **bpt)
{
	if (!i)
		return NULL;
	nlinfo *nf = (nlinfo *) this->name;
	if (!this->id
	 && nf->set) { // use hashes
		for (DClist *idn = nf->ids[i & NLnummask]->next; idn; idn = idn->next) {
			DCmlist *ni = (DCmlist *) idn->id;
			if (ni->id == i) {
				if (bpt)
					*bpt = ni->branch;
				return (char *) ni->name;
			}
		}
		if (bpt)
			*bpt = NULL;
		return NULL;
	}
  for (DCmlist *that = this; that; that = (DCmlist *) that->next) {
		if (i == that->id) {
			if (bpt)
				*bpt = that->branch;
			return (char *) that->name;
		}
	}
	if (bpt)
		*bpt = NULL;
	return NULL;
}

long DCmlist::getmult(const char *n, const char *b)
{
	if (!n || !*n)
		return 0;
	multpend.empty();
	multcount = multpos = 0;

	long v = hash(n);
	nlinfo *nf = (nlinfo *) this->name;
	if (!this->id
	 && nf->set) { // use hashes
		for (DClist *hashn = nf->hashes[v]->next; hashn; hashn = hashn->next) {
			DCmlist *ni = (DCmlist *) hashn->id;
			if (!_stricmp(ni->name, n)
			 && ((ni->branch == b)  // may be both null
			  || !b                 // means get all
			  || (ni->branch
			   && !_stricmp(ni->branch, b)))) {
				multpend.add((long) ni);
				++multcount;
			}
		}
	}
	else {
		for (DCmlist *that = (DCmlist *) this->next; that;
		 that = (DCmlist *) that->next) {
			if ((v == that->val)
			 && (that->name != NULL)
			 && !_stricmp(that->name, n)
			 && ((that->branch == b)  // may be both null
			  || !b                 // means get all
			  || (that->branch
			   && !_stricmp(that->branch, b)))) {
				multpend.add((long) that);
				++multcount;
			}
		}
	}
	return multcount;
}

long DCmlist::multitem(char **bpt)
{
	if (!multcount
	 || (multcount <= multpos))
		return 0;

	DCmlist *that = (DCmlist *) multpend.number(++multpos);
	if (that) {
		if (bpt)
			*bpt = that->branch;
		return that->id;
	}

	return 0;
}



// DCclist functions


DCclist::DCclist() : id(0), next(NULL)
{
	cp = (DCctl *) this;
}


void DCclist::add(DCctl *c, long i)
{
	if (!this->id) {
		DCclist *last = (DCclist *) this->cp;
		while (last->next)
			last = last->next;
		last->next = new DCclist(c, i);
		this->cp = (DCctl *) last->next;
	}
	else {
		for (DCclist *that = this; that; that = that->next) {
			if (!that->next) {
				that->next = new DCclist(c, i);
				break;
			}
		}
	}
}

void DCclist::add(DCclist *cl)
{
	DCclist **endp = NULL;

  for (DCclist *that = this; that; that = that->next) {
		if (!that->next) {
			endp = &(that->next);
			break;
		}
	}
  for (that = cl->next; that; that = that->next) {
		*endp = new DCclist(that->cp, that->id);
		endp = &((*endp)->next);
	}
	if (!this->id && endp)
		this->cp = (DCctl *) *endp;
}

void DCclist::remove(long i)
{
  for (DCclist *that = this; that; that = that->next) {
		if (that->next
		 && (i == that->next->id)) {
			DCclist *del = that->next;
			that->next = del->next;
			if (!del->next) {
				if (!this->id)
					this->cp = (DCctl *) that;
			}
			else
				del->next = NULL;
			delete del;
			break;
		}
	}
}

void DCclist::remove(DCctl *c)
{
  for (DCclist *that = this; that; that = that->next) {
		if (that->next
		 && (c == that->next->cp)) {
			DCclist *del = that->next;
			that->next = del->next;
			if (!del->next) {
				if (!this->id)
					this->cp = (DCctl *) that;
			}
			else
				del->next = NULL;
			delete del;
			break;
		}
	}
}

DCctl *DCclist::dequeue(void)
{
	if (!next)
		return NULL;
	DCclist *that = this->next;
	this->next = that->next;
	that->next = NULL;
	DCctl *ctl = that->cp;
	that->cp = NULL;
	delete that;
	if (this->cp == (DCctl *) that)
		this->cp = (DCctl *) this;
	return ctl;
}

long DCclist::count(void)
{
	long val = 0;

  for (DCclist *that = this->next; that; that = that->next)
    val++;
	return val;
}

void DCclist::empty(void)
{
	DCclist* nextone;

  for (DCclist *that = this->next; that; that = nextone) {
		nextone = that->next;
		that->next = NULL;
		delete that;
	}
	this->next = NULL;
	this->cp = (DCctl *) this;
}

void DCclist::all(void (*e)(DCctl *cp, long i))
{
  for (DCclist *that = this->next; that; that = that->next)
    (*e)(that->cp, that->id);
}

void DCclist::allval(void (*e)(DCctl *cp, long i, long v), long val)
{
  for (DCclist *that = this->next; that; that = that->next)
    (*e)(that->cp, that->id, val);
}

DCctl *DCclist::find(long i)
{
	if (i) {
		for (DCclist *that = this->next; that; that = that->next) {
			if (i == that->id)
				return that->cp;
		}
	}
	return NULL;
}

long DCclist::finddat(uns i)
{
  for (DCclist *that = this->next; that; that = that->next) {
		if (i == (that->cp)->dat())
			return that->id;
	}
	return 0;
}

DCctl *DCclist::finddatcp(uns i)
{
  for (DCclist *that = this->next; that; that = that->next) {
		if (i == (that->cp)->dat())
			return that->cp;
	}
	return NULL;
}

long DCclist::operator()(DCctl *c)
{
  for (DCclist *that = this->next; that; that = that->next) {
		if (c == that->cp)
			return that->id;
	}
	return 0;
}

void DCclist::write(long i, DCLwfile& wf)
{
  for (DCclist *that = this->next; that; that = that->next)
    if ((i == that->id) && that->cp)
      (that->cp)->write(wf);
}


// end of dclist.cc

