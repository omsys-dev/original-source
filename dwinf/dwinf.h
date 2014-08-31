
// dwinf.h is the header for dcl INF writing, JHG
// Copyright (c) 1998 by Omni Systems, Inc.  All rights reserved.

#ifndef DWINF_H
#define DWINF_H


class INFwr;
class INFwfile;
class INFctl;
class INFitem;
class INFlink;
class INFgroup;


enum WinfConstants {
  FontNamesMax = 18,
	FileNameMax = 256,
  MinorMax = 256,
	InfListMax = 1024
};

enum scantype {  // what to do during scan phase
  nothing = 0, 
	fontlist, styletype, stylelist, framelist, flowlist,
	grfile, xrfile
};

struct inf {
  dc_dtyp ctldt;
  uns ctlmaj;
  uns ctlmin;
  uns ctldat;
  scantype scan_op;
};


class INFctl : public DCctl {
 public:
  inf *rp;
  INFctl() { set(ilong); rp = NULL; }
  INFctl(dc_dtyp dtp, uns mj = 0, uns mn = 0, uns d = 0, unl s = 0)
    { dtype = dtp; major = (unc) mj; minor = (unc) mn; data = d; size = s; ext = NULL;
      rp = NULL; }
  INFctl(INFctl& c)
    { dtype = c.dtype; major = c.major; minor = c.minor; data = c.data;
      size = c.ext ? 0 : c.size; ext = NULL; rp = NULL; }
  INFctl(DCLrfile& rf) { *this = rf; }

  INFctl& operator= (DCLrfile& rf)
   { return (INFctl &) rf.getCtl(*this); }
  INFctl& operator= (DCblock& b)
    { if (ext) delete [] ext; ext = b.p(); size = b.l(); b.clear();
      return (INFctl&) *this; }
};


class INFitem {
 public:
  INFctl *ctl;
  INFitem *next;

  INFitem() : ctl(NULL), next(NULL) { }
  INFitem(INFctl *cp) : ctl(cp), next(NULL) { }
  INFitem(INFctl *cp, INFitem *n) : ctl(cp), next(n) { }
  virtual ~INFitem() { if (next) delete next; delete ctl; }
  virtual void insert(INFctl *nc, INFctl *oc)
    { if (!next) return;
      if (next->ctl == oc) next = new INFitem(nc, next);
      else next->insert(nc, oc); }
  virtual void replace(INFctl *nc, INFctl *oc)
    { if (!next) return;
      if (next->ctl == oc) next->ctl = nc;
      else next->replace(nc, oc); }
};


class INFlink {
 protected:
  INFitem *first, *last;
 public:
  INFlink() : first(NULL), last(NULL) { }
  virtual ~INFlink() { if (first) delete first; }
};


class INFgroup : public INFctl, public INFlink {
 public:
  INFgroup() : INFctl(group), INFlink() { }
  INFgroup(INFctl& c) : INFctl(c), INFlink() { }
  virtual ~INFgroup() { }
  void add(INFctl& c)
    { if (!first) last = first = new INFitem(&c);
      else last = (last->next = new INFitem(&c)); }
  void add(INFgroup& g)
    { if (!first) last = first = new INFitem(&g);
      else last = (last->next = new INFitem(&g)); }
  void merge(INFgroup& g)
    { if (&g == NULL) return;
      if (!first) first = g.first; else last->next = g.first;
      if (g.last) last = g.last;
      g.first = g.last = NULL; delete &g; }
  void prune(void)
    { if (first) delete first; first = NULL; }
  virtual void insert(INFctl& nc, INFctl& oc)
    { if (!first) return;
      else if (first->ctl == &oc) first = new INFitem(&nc, first);
      else first->INFitem::insert(&nc, &oc); }
  virtual void replace(INFctl& nc, INFctl& oc)
    { if (!first) return;
      else if (first->ctl == &oc) first->ctl = &nc;
      else first->INFitem::replace(&nc, &oc); }
  virtual void replace(INFgroup& gc, INFctl& oc)
    { if (!first) return;
      else if (first->ctl == &oc) first->ctl = &gc;
      else first->INFitem::replace(&gc, &oc); }
};


class INFwr {

  friend class INFctl;
  friend class INFgroup;

  // in dwinf.cc

  static inf INFpile[];
  static size_t INFsize;
  static DCvlist *INFlist[InfListMax];

  static void SetUpINF(void);
  static void FindINF(INFctl *cp);

	static long ObjectID;
	static long StyleType;
	static INFctl *StyleName;

  static DCnlist FontList;
  static DCnlist StyleList;
  static DCnlist PStyleList;
  static DCnlist CStyleList;
  static DCnlist FrameList;
  static DCnlist FlowList;
  static DCnlist GrFileList;
  static DCnlist XrFileList;

	static char *FontNames[FontNamesMax];

	static void AddToList(DCnlist *nl, INFctl *cp, char *str = NULL);
	static char *StripPath(INFctl *cp);

	static void WriteListSection(INFwfile *wf, char *str, DCnlist *nl);
	static void WriteKeys(DCwfile *dwf, char *str, long i);

 public:

  // in dwinf.cc, called from main()

  static short Verbose;
	static DCirfile *IniFile;
	static char BaseName[FileNameMax];

  static void ReadDCL(DCLrfile *rf);
  static void WriteINF(INFwfile *wf);
};


#endif

