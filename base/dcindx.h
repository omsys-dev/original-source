// dcindx.h is the header for dcl index classes, JHG
// Copyright (c) 2013 by Omni Systems, Inc.  All rights reserved.

#ifndef DCINDX_H
#define DCINDX_H

#include <windows.h>
#include <winnls.h>


class DCixset;
class DCixlist;
class DCixitem;
class DCixref;


enum ixtyp {
 ixno = 0, ixstd, ixsee, ixalso
};

enum ixuse {
 ixuseno = 0, ixent, ixgen, ixall
};

enum rngtyp {
 rngno = 0, rngstart, rngend
};

enum sortyp {
 sortno = 0, sortnum, sortlex
};

enum mrgtyp {
 mrgno = 0, mrgall
};

typedef int (*cmpfunc)(const char *s1, const char *s2);


class DCixset {
	// all the index lists to which refs may be added
 public:
	static DCixset *listset;

	static void LogEvent(logtype lt, unc sev, const char *desc,
	              const char *d2 = NULL, const char *d3 = NULL,
	              const char *d4 = NULL, const char *d5 = NULL);
	static logfunc LogEventFunc;

	static int cmpstrs(const char *s1, const char *s2);

	DCvlist lists;
	DCnlist listnames;
	long listcnt;
	DCvlist *nextlist;

	char *basename;
	char *fname;
	char *fsuf;
	DCirfile *ifile;
	DCirfile *lfile;

	bool compact;
	bool rtf;
	char *xrform;  // for compact, else use IndexTitleXref
	DCnlist *entforms;
	DCnlist *refforms;
	char *tocform;
	char *titleform;

	bool usehead;
	bool uselead;
	bool uselets;
	bool usetlets;
	bool makedef;

	bool seealsofirst;
	char *ignorefirst;
	char *ignore;

	DCixlist *deflist;

	DCixset(DCirfile *ifl, DCirfile *lfl, char *bn,
	 bool cf, bool rt, DCnlist *efs, DCnlist *rfs,
	 char *itf, char *tcf, char *fn, char *fs,
	 bool uh, bool ul, bool ulet, bool utlet, bool md,
	 bool saf, char *igf, char *ig) :
	 ifile(ifl), lfile(lfl), basename(bn), compact(cf), rtf(rt),
	 entforms(efs), refforms(rfs), titleform(itf), tocform(tcf),
	 listcnt(0), nextlist(&lists), fname(fn), fsuf(fs),
	 usehead(uh), uselead(ul), uselets(ulet), usetlets(utlet),
	 makedef(md), seealsofirst(saf), ignorefirst(igf), ignore(ig)
	{
		xrform = rtf ? "IndexPageXref" : "IndexIconXref";
		makelists(makedef);
	}
	virtual ~DCixset() { }

	void makelists(bool makedef);
	void procsellist(DCixlist *list);
	ixuse checklist(char *cl);
	DCixlist *listexists(char *name);

	char *getname(char *ename, char *oclass);
	void setuplist(DCixlist *list);
	void addlist(DCixlist *list);
	DCixlist *findlist(char *sel, bool first = false);

};


class DCixlist {
	// a single index list
 public:

	char *name;          // key used in ifile sections
	char *ename;         // element name of booklist elem
	char *oclass;        // outputclass
	char *title;         // navtitle or title from lang file
	sortyp sortby;
	mrgtyp merge;

	char *fname;         // basename without ext
	char *suf;           // suffix for mainname if no fname
	char *ext;           // extension, default .hym
	char *href;          // full path name with ext
	char *id;            // internal id for xref to body of list

	DCvlist items;
	DCnlist itemtitles;
	DCnlist itemsorts;
	long itemcnt;

	char *selclass;
	DCnlist itemsels;
	long itemselcnt;

	bool compact;
	bool ixrefpara;
	bool ixrefentry;
	bool ixrngstart;

	char *titleform;
	char *tocform;
	char *xrefform;

	char *itemform;
	DCnlist itemforms;
	char *refform;
	DCnlist refforms;
	long maxlevel;

	bool usehead;
	bool uselead;
	bool uselets;
	bool usetlets;

	cmpfunc sortcmp;

	DCgroup *gp;
	DCctl *frefcp;

	DCvlist seeitems;
	DCnlist seeitemtitles;
	long seecnt;
	DCvlist alsoitems;
	DCnlist alsoitemtitles;
	long alsocnt;

	DCixlist(char *e) : name(e) { }
	virtual ~DCixlist() { }

 	void setlistsort(sortyp st = sortlex, mrgtyp mt = mrgall,
	                 void *scmp = NULL);
	void setlistprops(char *oc, char *tt = NULL);
	void setfname(DCirfile *ifile, char *selsect, char *basename,
	              char *filename, char *filesuf);
	void setlisttypes(DCirfile *ifile, char *selsect, bool cf, bool rtf,
		                bool uh, bool ul, bool ulet, bool utlet);
	void setselectors(DCirfile *ifile);
	void setitemforms(DCirfile *ifile, char *selsect, DCnlist *difs);
	void setrefforms(DCirfile *ifile, char *selsect, DCnlist *drfs);
	void setlistforms(DCirfile *ifile, char *selsect,
	                  char *xf, char *tf, char *cf);
	void settitle(DCirfile *lfile, char *selsect);

	void additem(DCixitem *item);
	DCixitem *getitem(char *title);

	//void writeTOC(DCLwfile *df, DCgroup *cgp);
	//void writeDCL(DCLwfile *df, DCgroup *gp);
	void procseeids(void);
	long findidxitemtitle(const char *name);
	char *makeitemid(const char *title);
};



class DCixitem {
	// a single index item in one list
	// for use in multiple lists, use multiple items
 public:
	char *title;       // text to display
	DCgroup *gtitle;
	char *stitle;      // text without see/also
	DCgroup *gstitle;
	char *sort;        // text to sort by
	bool sortset;
	ixtyp itype;       // normal, see, see also
	char *id;          // id for targets of see/also items

  long level;        // level, top == 1

	DCixlist *list;    // list parent
	DCixitem *parent;  // item parent if level > 1
	long parentcnt;    // index in list or parent item

	DCvlist subs;     // subitems
	DCnlist subtitles;
	DCnlist subsorts;
	long subcnt;

	DCvlist refs;
	long refcnt;

	DCixitem(char *t, long l, DCixlist *lst, DCixitem *par = NULL, ixtyp ixt = ixno) :
	 title(t), level(l), list(lst), parent(par), sortset(false), itype(ixt)
	{
		if (!list
		 && parent)
			list = parent->list;

		if (level == 1) {
			list->additem(this);
			parentcnt = list->itemcnt;
		}
		else if (parent) {
			parent->addsub(this);
			parentcnt = parent->subcnt;
		}
	}

	virtual ~DCixitem() { }

	void addsub(DCixitem *sub);
	DCixitem *getsub(char *title);

	void addref(DCixref *ref);
	char *setsort(char *s);
	//void writeitemDCL(DCixlist *list, DCgroup *gpp, long lev);
	//void writerefsep(DCgroup *gpp);
	//void writerefrangesep(DCgroup *gpp);
};


class DCixref {
	// a single reference to one index item
	// if item in multiple lists, ref points to all
	// and all point to the same ref

 public:
	char *id;          // id of index entry
	char *title;       // text to show for entry
	DCgroup *gtitle;
	char *stitle;      // text without see/also
	DCgroup *gstitle;
	char *selclass;    // outclass to select lists to put in
	char *sort;        // sort string specified in ref
	long level;
	DCixref *parent;   // parent ref if level > 1

	rngtyp range;      // start/end range indicator
	char *rname;       // range name
	DCixref *eref;

	ixtyp itype;       // normal, see, see also
	char *seeref;      // idref for see/also entry

	DCixitem *curritem;
	DCvlist items;     // pointers to each item using ref
	long itemcnt;      // count of items using ref

	DCvlist subs;      // subrefs
	long subcnt;

	void *elem;        // starting indexterm element
	void *refelem;     // referenced element with title
	bool map;
	char *topid;       // id of containing topic
	char *refid;       // id of containing element
	char *reftitle;    // text to show for HTML refs
	DCgroup **reftgroup;  // ptr to ptr to text group containing title

	static DCixref *CurrIXRef;
	static DCnlist IXRefRanges;

	DCixref(void *el, char *sc, long l = 1, DCixref *par = NULL) :
	 elem(el), selclass(sc), level(l), parent(par),
	 itype(ixstd), range(rngno)
	{ if (parent) parent->subs.add(this, ++parent->subcnt);}

	virtual ~DCixref() { }

	void settitle(DCgroup *t, bool conref);
	void setstitle(DCgroup *t);
	void setlistitem(void);
	void setsortstr(char *s);
	void setids(void *rel, DCgroup **rt, char *tid,
	            char *rid = NULL, bool inmap = false);
	void setsee(ixtyp i);
	void setrange(rngtyp r, char *rn);
};


#endif

