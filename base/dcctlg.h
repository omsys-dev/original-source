
// dcctlg.h is the header for dcl ctl group classes, JHG, 6/24/93
// Copyright (c) 1993 by Omni Systems, Inc.  All rights reserved.

#ifndef DCCTLG_H
#define DCCTLG_H


class DCitem;
class DClink;
class DCgroup;


// display list item class

class DCitem {
 public:
  DCctl *ctl;
  DCitem *next;

  DCitem() : ctl(NULL), next(NULL) { }
  DCitem(DCctl *cp) : ctl(cp), next(NULL) { }
  DCitem(DCctl *cp, DCitem *n) : ctl(cp), next(n) { }
  virtual ~DCitem() { delete ctl; }

#if 0
  virtual void insert(DCctl *nc, DCctl *oc);
  virtual void replace(DCctl *nc, DCctl *oc);
  virtual void write(DCfile& wf);
#endif
};


// link class

class DClink {
 public:
  DCitem *first, *last;
  DClink() : first(NULL), last(NULL) { }
  virtual ~DClink() { }
};



// group class

class DCgroup : public DCctl, public DClink {
 public:
  DCgroup() : DCctl(group), DClink() { }
  DCgroup(DCctl& c) : DCctl(c), DClink() { }
	//DCgroup(DCgroup& g);
	DCgroup(DCgroup& g, DCilist *gmap);
  virtual ~DCgroup();

	void empty(void);
  void add(DCctl& c);
  void merge(DCgroup& g, bool del = true);
  DCgroup& operator= (DCgroup& g);
  virtual void insert(DCctl& nc);
  virtual bool insert(DCctl& nc, DCctl& oc);
  virtual void insert(DCgroup& g);
  virtual bool replace(DCctl& nc, DCctl& oc, DCitem **nxt = NULL);
  virtual void write(DCLwfile& wf, long lev = 0);
	virtual void execu(void (*e)(DCctl*));
	virtual void execwf(void (*e)(DCctl*, DCLwfile&), DCLwfile& wf);

	char *getText(void);
	void getTextStrs(char *tbuf, long lev);
	short getTextLen(long lev);
	bool writeIdxTitle(DCgroup *dest, bool first = false, long lev = 0);
};



#endif

