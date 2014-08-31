// dcelem.h is the header for dcl element classes, JHG
// Copyright (c) 2009 by Omni Systems, Inc.  All rights reserved.

#ifndef DCELEM_H
#define DCELEM_H


class DCtopdoc;
class DCpage;
class DCgenlist;
class DCelemset;
class DCtopic;
class DCmap;
class DCelem;
class DCtopelem;
class DCmapelem;

struct conditions;


class DCtopdoc {

 public:
	long docnum;
	char *docname;
	DCnlist *topicids; // tid used to get topicnum
	DCvlist *topics;   // pointers to topicinfo structs by topicnum
	long topiccnt;     // last topicnum (1-based)
	DCmapelem *mref;   // map topicref pointing to this doc

	// member functions
	DCtopdoc(long num) : docnum(num), topiccnt(0),
	 topicids(new DCnlist), topics(new DCvlist) { }
	virtual ~DCtopdoc() { }

};


class DCpage {

 public:
	long pagenum;      // page number (not page sequence number)
	DCgroup *pgroup;   // page group
	DClist *graphics;  // list of graphnums used on page
	DCilist *groupmap; // map from old group to new group for conrefs
	DCilist *childmap; // map from old children to new children for conrefs

	DCgroup *layout;   // layout frame group, layout framenum in dat()
	long firstframe;   // layout frame for page
	long lastframe;    // last frame used on page, layout or graphic
	DCctl *textframe;  // text layout framenum in dat(), pagenum in siz()
	DCctl *nextframe;  // next layout textnum in dat(), framenum in siz()

	uns textnum;       // text stream num
	DCgroup *pagetext; // add graphic frames before this one, textnum in dat()
	DCctl *flowtag;    // start of flow, remove if continuing prev flow
	DCctl *textprev;   // last text framenum in dat(), last page in siz()
	DCctl *textnext;   // next text framenum in dat(), next page in siz()
	DCgroup *relend;   // group for rellinks at end of text group

	DCtopic *topic;    // topic for this page

	// member functions
	DCpage(long num) : pagenum(num)
	{
		groupmap = new DCilist;
		childmap = new DCilist;
	}
	virtual ~DCpage() { }

	void setgroup(DCgroup **gp)
	{
		long ng = groupmap->find((long) *gp);
		if (ng)
			*gp = (DCgroup *) ng;
	}
	void setchild(DCelem **cp)
	{
		long ng = childmap->find((long) *cp);
		if (ng)
			*cp = (DCelem *) ng;
	}
};


enum gentyp {
	gennone = 0, gentoc, genidx, genlof, genlot, gengloss,
	genbib, genabbr, gentmk, genamend, genlist
};


class DCgenlist {

 public:
	DCelem *elem;
	char *gname;
	bool used;
	gentyp typ;
	char *fname;
	char *href;
	char *id;
	char *suf;
	char *title;
	char *titleform;
	char *itemform;
	char *xrefform;
	char *tocform;
	DClist *items;
	long itemcnt;
	DClist *conrefitems;
	long conrefitemcnt;
	bool keepprev;
	DCmapelem *docref;
	DCgroup *gp;
	DCgroup *wrapper;
	DCilist itemlevs;
	DCnlist itemforms;
	long maxlevel;
	DCnlist itemsels;
	long itemselcnt;
	DCctl *frefcp;

	DCgenlist(DCelem *el, gentyp gt) :
	 elem(el), typ(gt)  { }
	virtual ~DCgenlist() { }
};


enum linktyp {
	linknone = 1, linksource, linktarget, linknormal
};

enum colltyp {
	colunord = 1, colseqence, colchoice, colfamily
};

enum formtyp {
	ftnone = 0, ftdita, ftditamap, fthtml, ftpdf, fttxt, ftother
};

enum roletyp {
  rlnone = 0, rlprev, rlnext, rlparent,
  rlchild, rlsibling, rlancestor, rldescendant,
  rlcousin, rlfriend, rlother, rltopic,
  rlconcept, rltask, rlreference, rlrelated
};

enum scopetyp {
  scnone = 0, sclocal, scpeer, scexternal
};

enum tseqtyp {
	sqnone = 0, sqhead, sqtopic, sqdesc
};

struct seqtopic {
	long level;
	tseqtyp type;
	DCtopic *topic;   // NULL for topicheads
	DCgroup *title;   // contains link or xref for topics, text for heads
	DCgroup *desc;    // contains shortdesc from map
};

struct relattr {
	scopetyp lscope;  // nonzero if scope was set

	char *lrole;      // role attribute, enum
	char *lorole;     // otherrole attribute, text
	roletyp lrval;    // role value

	char *lform;      // format attribute, enum/text
	formtyp lftype;

	char *ltype;      // type attribute, enum/text
	roletyp ltoptype;

	char *lcoll;      // collection attribute, enum
	colltyp lcollect; // type of collection

	char *linking;    // linking attribute, enum
	linktyp links;    // directionality for linking
};

struct rellink {
	// info for a single related link
	DCtopic *ltopic;  // topic pointed to for link
	seqtopic *seq;    // sequence info
	DCelem *lsrc;     // element containing the link
	char *lhref;      // fixed href from link or topicref
	char *loclass;    // outputclass attribute

	relattr *at;      // attributes affected by inheritance

	roletyp lrtype;   // category to put link in

	DCgroup *lshort;  // shortdesc from reltable topicref
	DCgroup *ltext;   // linktext from related-links
	DCgroup *ldesc;   // desc from related-links
};

struct rellinkinfo {
	// links from topic to other topics, not always bidirectional
	rellink *lparent;   // single parent
	rellink *lprev;     // previous in sequence
	rellink *lnext;     // next in sequence
	DClist *lchildren;  // children are sequential
	DClist *lsiblings;  // siblings are unordered
	DClist *lancestors;
	DClist *ldescendants;
	DClist *lcousins;
	DClist *lfriends;

	DClist *lrelated;   // related are unordered
	DClist *lconcepts;
	DClist *ltasks;
	DClist *lreferences;

#if 0
	// not used
	DClist *linklist;   // topics in linklist
	DClist *linkpool;   // topics in linkpool

	DCvlist *lrelated;  // list of DClist *s, containing rellink *s,
	long relrows;       //   each for one row of a reltable, 
	DCnlist *lrelnames; // names for those rows for ALink use
#endif
};

enum esettyp {
	esnone = 0, esmap, estopic
};


class DCelemset {

 public:
	char *id;          // id for links, xrefs, and conrefs
	esettyp estyp;     // type of elemset, map or topic

	long pagenum;      // output page used for topic content
	DCpage *page;

	DCmapelem *mref;

	DCnlist *elemids;  // elemid used to get elemnum
	DCvlist *elems;    // pointers to DCelems by elemnum
	long elemcnt;      // last elemnum (1-based)

	char *title;
	bool titlechanged;   // set when conref in child of title alters title
	bool userel;


	// member functions
	DCelemset(long pnum, DCpage *pg, esettyp est) :
	 pagenum(pnum), page(pg), id(NULL), estyp(est),
	 elemcnt(0), elemids(new DCnlist), elems(new DCvlist),
	 userel(true) { }
	virtual ~DCelemset() { }

};

enum topictyp {
	tttopic = 0, ttconcept, tttask, ttreference
};

class DCtopic : public DCelemset {

 public:
	long topicnum;
	topictyp ttype;

	long docnum;
	DCtopdoc *doc;
	char *branch;        // name of current branch
	rellinkinfo *rlinks; // related-links and reltable info
	char *alinkid;       // id used for alinks from related-links
	DClist *alinks;      // list of other topic alinkids to reference
	DCgroup *metatext;   // index entries, keywords, from prolog
	DCgroup *headchildren; // group for adding topic head children
	DCgroup *desc;       // contains shortdesc from topic
	long tlevel;
	bool chunked;
	long part;

	// member functions
	DCtopic(long num, long pnum, DCpage *pg, long dnum, DCtopdoc *doc,
	 long lev, char *br = NULL) : DCelemset(pnum, pg, estopic),
	 topicnum(num), docnum(dnum), doc(doc), tlevel(lev), branch(br),
	 part(0) {
		doc->topics->add(this, num); doc->topiccnt = num; }
	virtual ~DCtopic() { }
};


class DCmap : public DCelemset {

 public:
	long mapnum;
	char *mapname;

	// member functions
	DCmap(long num, char *name, long pnum, DCpage *pg) : 
		DCelemset(pnum, pg, esmap), mapnum(num), mapname(name) { }
	virtual ~DCmap() { }
};



enum elemtyp {
 topic = 1, concept, task, reference, glossentry,
 section, fig, table, li, fn, other = 0
};

enum cnrftyp {
 ctnot = 0, ctt2t, ctm2m, ctt2m, ctm2t, ctlast
};

enum conacts {
 ca_pull = 0, ca_replace, ca_before, ca_after,
 ca_start, ca_end, ca_mark
};

enum elemconst {
 epCntMax = 11
};

enum tmtyp {
	tmnone = 0, tmtm, tmreg, tmsm
};

enum abbrtyp {
	abnone = 0, abfirst, abacron, ababbr
};

enum roltyp {  // processing-role, inherited
	rolenone = 0, roleuse, roleres
};


struct etype {
	const char *ename;
	unc cnt;
	unc eprop[epCntMax];
};


class DCelem {

 public:
	long elemnum;      // starts with 1 for root
	char *elemname;    // used to find matching end for conrefs
	long elemdepth;    // end must be at same depth
	esettyp eltyp;     // type of elemset, map or topic
	elemtyp elemtype;
	roltyp role;

	DCelem *parent;    // parent element of this element
	DClist *children;  // child elements of this element
	DCilist *childmap;

	long elemcount;    // index in BlockElems, InlineElems, or MapElems

	long conrefnum;     // index in list of its type
	cnrftyp conreftype; // indicates which list it is in
	DCelem *addedafter; // records elem pushed after current one

	DCctl *eignore;    // after last para end for ending ignore
	DCctl *einset;     // at end of conref content
	char *id;          // id for links, xrefs, and conrefs
	char *lang;        // xml:lang attribute
	char *classatt;    // class attribute
	char *outclass;    // outputclass attribute
	char *href;        // normalized href attribute
  char *tdesc;
	char *idref;       // UMXL @idref
	char *idrefs;      // UMXL @idrefs
	char *project;     // UMXL @project
	char *type;        // UMXL @type
	char *output;      // UMXL @output
	char *conref;
	char *endref;      // end conref range attribute
	char *conact;         // conaction attribute
	char *keyref;      // keyref attribute
	char *keyeid;
	char *conkeyref;   // keyref attribute for conrefs
	char *conkeyeid;
	char *keyhref;
	char *keyconref;
	DCgroup *keytext;
	DCctl *conrefinset;
	bool isEmpty;
	tmtyp tm;          // trademark type

	long attrcnt;      // count of attributes
	DCnlist *attrnames; // list of attribute names in order
	DCnlist *attrvals;  // list of attribute values in same order
	DCclist *attrnctls; // list of attribute name ctls in output
	DCclist *attrvctls; // list of attribute value ctls in output
	DCgroup *attrgrp;  // attribute ctls in output
	DCgroup *attrtext; // text group containing attrgrp in output
	DCgroup *pxheads;  // group for added prefix head content
	DClist *parselist; // set to catch all child elems for replay
	DClist *idxrefs;   // indexrefs within the current elem
	void *idxref;      // indexref for the current elem

	unc epcnt;         // count of properties
	unc *eprop;        // properties flags
	bool epset;        // true once prop flags set by CheckElem()
	bool elinlin;      // true if inline, false if block
	bool elskip;       // true if tags for it should be skipped

	bool xrsrc;        // true if possible xreference source
	char *elemuid;     // unique ID made from path, topic id, elemid
	char *elfile;      // name used for chunkfile
	DCctl *anchor;     // id anchor for links
	DCctl *xanchor;    // id anchor for xrefs
	DCgroup *idgrp;    // id ctls in output
	char *elform;      // format used for element
	char *elpath;      // element path to elem
	char *elfpath;     // element full path to top of highest map
	DCctl *epctl;      // element path ctl, if set by this elem
	DCctl *epfctl;     // element full path ctl, if set by this elem
	DCctl *formctl;    // format ctl, if set by this elem
	bool isfirst;      // true if first under its parent

	DCgroup *anumtxt;  // contains full anum put in TextGroup for elem
	DCelem *firstanelem; // first contained elem with anum, may be self
	DCelem *lastanelem;  // last contained elem with anum, may be same
	void *elemnf;        // numbering format to use
	bool inLOF;        // true if added to LOF list
	bool inLOT;        // true if added to LOT list
	bool ingenlist;    // true if added to any generated booklist

	abbrtyp abbr;      // type of abbreviation to use for GlossAbbr
	long abreset;      // lowest level of reset to use for this topic/mapelem

	DCgroup *elemnref; // contains full anum to be used in xref to elem
	DCgroup *elemaref; // contains num-only anum to be used in xref to elem
	DCgroup *elemtref; // contains text to be used in xref to elem
	DCgroup *elemtext; // contains full original elem text content for conref to elem
	DCgroup *lasttext; // parent TextGroup for restoral at end of orig
	DCgroup *vartext;  // contains variable content if elem is var source

	DCgroup *starttext; // placeholder for pushatstart text
	DCgroup *endtext;  // placeholder for pushatend text
	DCilist *groupmap; // old group to new group for conrefs

	char *branch;       // set by PI, uses condlevel
	char *keys;         // set for keydefs

	// member functions
	DCelem() {  groupmap = new DCilist;}
	DCelem(long num) : elemnum(num) {  groupmap = new DCilist;}
	DCelem(long num, char *name, long depth, char *clat, char *oclat,
	 esettyp eltyp) :
	 elemnum(num), elemname(name), classatt(clat), outclass(oclat),
	 href(NULL), elemdepth(depth), epcnt(0), eprop(NULL), epset(false),
	 abbr(abnone), abreset(0) {
		groupmap = new DCilist;
		childmap = new DCilist;
	}
	virtual ~DCelem() { }
	virtual long GetDocCount(void) { return 1; }

	bool setType(void);
	bool checkType(unl bit, unl neg = 0, bool all = true);
	void addProp(unc prop);
	void setAttrs(const char **atts);
	void setAttrCtls(DCgroup *txtgp);
	char *getAttr(const char *name);
	void addAttr(const char *name, const char *val);
	bool isDescendant(DCelem *child);
	bool isAncestor(DCelem *par);
	DCelem *getChildName(char *cname);
	DCelem *getDerivedChild(char *cl, bool special);
	void setKeyrefs(const char *kref, const char *ckref);
	virtual DCmapelem *GetMapParent(void) { return NULL; }
	virtual char *GetDocName(void) { return NULL; }
	void setgroup(DCgroup **gp)
	{
		long ng = groupmap->find((long) *gp);
		if (ng)
			*gp = (DCgroup *) ng;
	}
	void setchild(DCelem **cp)
	{
		long ng = childmap->find((long) *cp);
		if (ng)
			*cp = (DCelem *) ng;
	}

	// statics
	static etype **ElemTypes;
	static long ElemTypesCount;
	static char *LastClass;
	static etype **LastEP;

	static void SetElemTypes(etype **et, long etc);
	static int __cdecl CompElemTypes(const void *elem1, const void *elem2);
};


enum outputtyp {
	outnone = 0, outprint, outweb, outall
};

enum chunkbytyp {
	cbnone = 0, cbtopic, cbdoc
};
enum chunkseltyp {
	csnone = 0, cstopic, csdoc, csbranch
};
enum chunktotyp {
	ctnone = 0, ctcont, ctnav
};

enum doctexttyp {
	dtxnone = 0, dtxend, dtxstart, dtxbefore, dtxafter, dtxreplace
};
struct doctxt {
	doctexttyp dtt;
	DCgroup *dtgp;
};



class DCmapelem : public DCelem {

 public:
	long mapnum;
	DCmap *map;

	long dcount;       // DocCount for topicref
	long mcount;       // MapCount for topicref
	long docnum;       // docnum in docinfo for referenced doc
	long mlevel;       // level in toc of map item

	bool toc;          // include in toc, default true
	bool usenav;       // locktitle attr, default false
	outputtyp usage;   // use in output, default all
	bool search;       // visible to FTS, default true
	bool userel;       // add rel links, default true

	char *navtitle;    // title to use in TOC if usenav = true
	 // topic may have navtitle too; used if usenav = false
	DCctl *navtxt;     // ptr to ctl with navtitle

	DClist *flags;     // flags for this element
	DCnlist *groups;   // hashtags for this element

	char *chunk;       // the chunk attr, a space-delimited list   
	chunkbytyp chunkby;   // chunk by topic or document or none
	chunkseltyp chunksel; // select topic, doc, branch, or none
	chunktotyp chunkto;   // chunk to content or nav or none

	char *scopetxt;
	scopetyp scope;			// scexternal opens in new window
	char *copyto;       // filename to use for this instance
	char *format;       // ditamap, topic, html, pdf, ext of file

	relattr *ar;        // attributes on reltable elements
	DCgroup *metatext;  // index entries, keywords, from topicmeta
	DCgroup *mapdesc;		// place to put shortdesc from topic
	DCgroup *cmaptxt;	  // place to put text from conrefed map
	DCgroup *ldesc;		  // place to put shortdesc from reltable
	DCgroup *ltext;	    // place to put linktext from reltable
	DCgroup *docendtext;// places to put UMXL doctext from map
	DCgroup *docstarttext;
	DCnlist *doctextids;

	DCgroup *nested;    // map groups for topics nested in href topic
	long mglevel;       // level used for this topicref's map group

	DCtopic *reftop;		// topic pointed to by topicref
	DCmapelem *crefnew; // first topic this was conreffed into

	void *conds;        // condition set applied for this topicref
	long condlevel;     // level for condition set determination

	bool gendoc;        // true if must be generated, false if provided
	gentyp gentype;     // type of generated list, for topicref

	// member functions
	DCmapelem() { }
	DCmapelem(long num, char *name, long depth,
	 char *clat, char *oclat, long mnum, DCmap *map) :
	 DCelem(num, name, depth, clat, oclat, esmap),
	 mapnum(mnum), map(map), userel(true),  // conref(NULL),
	 toc(true), usenav(false), usage(outall), search(true) {
		if (map) {map->elems->add(this, num); map->elemcnt = num;} }
	DCmapelem(DCelem elem) : DCelem(elem) { }
	virtual ~DCmapelem() { }
	DCmapelem *GetMapParent(void) { return map->mref; }
	char *GetDocName(void) { return map->mapname; }
	long GetDocCount(void) { return dcount; }
};


class DCtopelem : public DCelem {

 public:
	long topicnum;
	DCtopic *topic;

	long graphnum;     // if nonzero, has graphic frame
	DCgroup *elemgraph; // contains element's graphic frame if any

	DCgroup *fntext;   // contains full footnote content

	// member functions
	DCtopelem(long num, char *name, long depth,
	 char *clat, char *oclat, long tnum, DCtopic *top) : 
	 DCelem(num, name, depth, clat, oclat, estopic),
	 topicnum(tnum), topic(top) {
		if (topic) {topic->elems->add(this, num); topic->elemcnt = num;} }
	DCtopelem(DCelem elem) : DCelem(elem) { }
	virtual ~DCtopelem() { }
	DCmapelem *GetMapParent(void) { return topic->mref; }
	char *GetDocName(void) { return topic->doc->docname; }
	long GetDocCount(void) { return topic->mref->dcount; }
};


// conref classes

enum cnerrtyp {
	noerr = 0, noelem, notopic, nodocinfo, nodoc,
	nomapelem, nomap, nomapinfo, noact, not2m,
	nom2mpush
};

class DCcnref {

 public:
	long cnum;           // index in list of its type
	cnrftyp conreftype;  // indicates which list it is in

	DCelemset *srcset;   // pointer to source topic or map
	DCelemset *destset;  // pointer to destination topic or map
	DCelem *srcelem;     // pointer to source element
	DCelem *destelem;    // pointer to destination element

	char **href;         // corrected conref attribute
	char **endref;       // end conref range attribute
	char *actatt;        // conaction attribute
	conacts action;      // none, push*, and mark

	char *branch;        // target branch info
	conditions *conds;   // current cond info
	char *xrname;        // name of doc containing conref

	bool didlists;       // true if lists below populated
	DClist anums;        // elements with anums
	DClist lofitems;     // elements in LOF
	DClist lotitems;     // elements in LOT

	//member functions
	DCcnref(long num, cnrftyp typ, DCvlist &lst, DCelem *el);
	virtual ~DCcnref() { }
};

#if 0
class DCmapcnref : public DCcnref {

 public:
	DCmap *maporig;      // original map in which conref is done
	DCmap *mapnew;       // replacement map info
	DCmapelem *elemorig; // if elemnum == 1, orig is full topic, else element
	DCmapelem *elemnew;  // replacement element info

	//member functions
	DCmapcnref(long num, DCvlist &lst, DCmap *map, DCmapelem *el) :
	 DCcnref(num, ctm2m, lst, el),
	 maporig(map), elemorig(el), mapnew(NULL), elemnew(NULL) { }
	virtual ~DCmapcnref() { }
};


class DCtopcnref : public DCcnref {

 public:
	DCtopic *toporig;    // original topic in which conref is done
	DCtopic *topnew;     // replacement topic info
	DCtopelem *elemorig; // if elemnum == 1, orig is full topic, else element
	DCtopelem *elemnew;  // replacement element info

	//member functions
	DCtopcnref(long num, DCvlist &lst, DCtopic *top, DCtopelem *el) :
	 DCcnref(num, ctt2t, lst, el), 
	 toporig(top), elemorig(el), topnew(NULL), elemnew(NULL) { }
	virtual ~DCtopcnref() { }
};


class DCm2tcnref : public DCcnref {

 public:
	DCmap *maporig;      // original map in which conref is done
	DCtopic *topnew;     // replacement topic info
	DCmapelem *elemorig; // if elemnum == 1, orig is full topic, else element
	DCtopelem *elemnew;  // replacement element info

	//member functions
	DCm2tcnref(long num, DCvlist &lst, DCmap *map, DCmapelem *el) :
	 DCcnref(num, ctm2t, lst, el),
	 maporig(map), elemorig(el), topnew(NULL), elemnew(NULL) { }
	virtual ~DCm2tcnref() { }
};


class DCt2mcnref : public DCcnref {

 public:
	DCtopic *toporig;    // original topic in which conref is done
	DCmap *mapnew;       // replacement map info
	DCtopelem *elemorig; // if elemnum == 1, orig is full topic, else element
	DCmapelem *elemnew;  // replacement element info

	//member functions
	DCt2mcnref(long num, DCvlist &lst, DCtopic *top, DCtopelem *el) :
	 DCcnref(num, ctt2m, lst, el),
	 toporig(top), elemorig(el), mapnew(NULL), elemnew(NULL) { }
	virtual ~DCt2mcnref() { }
};
#endif

#endif
