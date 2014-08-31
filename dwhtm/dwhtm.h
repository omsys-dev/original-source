
// dwhtm.h is the header for dcl HTM writing, JHG
// Copyright (c) 1996-1998 by Omni Systems, Inc.  All rights reserved.

#ifndef DWHTM_H
#define DWHTM_H


class HTMwr;
class HTMwfile;
class HTMctl;
class HTMitem;
class HTMlink;
class HTMgroup;
class DCfrfile;
class MXLwr;

typedef void (*vfphtm)(HTMwfile *);

enum WhtmConstants {
  RowAlgnMax = 3, TabTypeMax = 3, CharSMax = 4, ParAlgnsMax = 4,
	FontSizesMax = 7,	FontColorMax = 9, HtmStyleMax = 10, HRefLen = 6,
  FrameHRMax = 3, FrameHAMax = 6, FrameVRMax = 2, FrameVAMax = 5, 
  UnitBasesMax = 4, PgnFormsMax = 5, FrLangMax = 14, SavedStateMax = 20,
  CellFillMax = 12, CellFillEmpty = 13, CellFillWhite = 14, CellCountMax = 36,
  ColorTableMax = 9,  Invisible = 1, SpacerImageSize = 43, EuroCharMax = 64,
  GrFWeightMax = 7, HtmlKeysMax = 82, TitleBufMax = 16000, NumStrMax = 17,
	ListInitMax = 12, ListTypeMax = 8, OrderedListMax = 6, ListLevMax = 32,
  FTypesMax = 7, FNamesMax = 17, FillMapMax = 16, CPStackMax = 16,
  FrameStackMax = 16, GrFillMax = 16, GrGroupMax = 32, WmfFNameMax = 32,
  StringMax = 80, CharMetricMax = 128, LineMax = 255,
  HULevMax = 128,	LevelMax = 128, ANameLevMax = 128, AnumPxSxMax = 32,
	AlphaSortCharMax = 128, AnsiLen = 128, ElementFormsMax = 7,
	CssClassNameMax = 128, RefBufLen = 1024, MaxIniLen = 65500,
  MinorMax = 256, NameBufMax = 256, HtmListMax = 1024,
  WaiAttrPropsMax = 18, MarkerPropsMax = 43, CellMargMax = 30000, 
	TrailLevMin = 1, TrailLevMax = 9, TrailLenMax = 1024, RomanMax = 1024,
	CssUnitsMax = 9, IndexLevMark = 31, DITAMarkerPropsMax = 36,
	GlossTitleBufLen = 1024, ESetBase = 65535, UserESetBase = 98303,
	FilterSelectBarMax = 4096
};

enum cptype {  // used to access CPTypes, keep in sync
  CPBold = 0, CPItalic, CPUline, CPOline, CPStrike, CPChbar,
  CPSub, CPSup, CPFont, CPTypesMax
};

enum mactype { // used for style macpos values
  MacNone = 0, MacBefore = 1, MacAfter = 2, MacReplace = 4, MacStart = 8,
	MacExtrReplace = 10, MacExtrHead = 11, MacExtrTop = 12, MacExtrBottom = 13,
  MacEnd = 16, MacAnumBefore = 32, MacAnumAfter = 64, MacLinkSrc = 128
};

enum cbtype {  // used for CharBlockState
	CbNone = 0, CbStart = 1, CbStarted = 2, CbEnd = 3, CbEnded = 4, CbChanged = 5
};

enum csstyp {  // used for CSS size unit identification
	fnth = 0, pspc = 1, hpos = 2
};

enum tdtype {  // used to identify table divs
	tFootType = 1, tBodyType = 2, tHeadType = 3
};

enum modtype { // where processing happens
  none = 0, omit,
  top, tx, an, tb, gr, rfm,
  modlast
};

enum scantype {  // what to do during scan phase
  nothing = 0, pass, dbgscan,
  unitset, srcver, drname, ispart,        // in dwhtm.cc
  docdefs, docres, txfile,                // in dwhtmtx.cc
  fontlist, language, charset, flowlist, flowend, flowname, flowused,
	setrunin, endrunin, sideflow, idxchk,
	ukeyset, ukeyeset, ukeyout, ukeylib, ukeyid, ukeyiref, ukeyitm, ukeyeitm, ukeykeys, ukeysrc,
  formdef, formend, formname, formtype, formtag, formchk, formscan,
	formscch, formpfx, formpfe, formsfx, formsfe, formspc,
	mapref, mapdsc, relref, reldsc, 
	conddef, condproc, condnam, condind, condvis, condfcol, condbcol,
	condchcol, condcchr, condstim, condstalt, condenim, condenalt,
  condset, condres, conddis, condrset,
  ftnstart, ftnend, ftnform, 
  tinset, tiname, tifile, tiimph, tiaupd, tilupd, tilock, tinend,
  ticlnt, ticlname, ticlfile, ticltype, ticldata,
  tiflow, timflw, tipgsp, tiftag, tiform, tinovr, tinpgb,
	xrfname, xrffnum,
	anumdef, anumtype, endanum, anumtxt, anumset, anumform,
	anform, anfend, anrform, anrfend, ansyms, anesyms, anvtype,
  ruleprop, tformck, tmargck, colrdef, colrname,   // in dwhtmtb.cc
	tbstart, tbend, tblchk, tblform, trstart, trend, tcstart, tcend,
  tbltits, tbltite,
  pgdef, pgend, pgattr, pgsize,           // in dwhtmgr.cc
  frdef, fredef, franch, frattr, frsize, frline, frpen, frfill,
	frprop, frpname, frpval, freprop, frdpi,
  frprev, frtext, setanch, frpost, eqform, eqfile, grfile,
  grline, grpen, grfill, grshape,
  grgroup, gregroup, grobj, greobj, grtext,
  rasprop, rasbmap, rascmap,
	imagemap, endimap, imapprop, 
	maparea, endmapa, mapaprop, mapshape,
	varnum, vardef, varname, varform, varsys,     // in dwhtman.cc
	varanum, varacnt, varaelem, varaidx, varaform,
  setmark, scanlast
};

enum htmtype {  // what to do during write phase
  ignore = 0, skip, dbgwrite,
  puttx, puttbr, putfile,             // in dwhtmtx.cc
  putfont, putfsize, putbold, putital,
  putbase, putcaps, putstyl, putrevb, putline, putlmod, putkern,
  putspan, putside, putrun, putrunin, eptrunin,
  putlspc, putpspc, putalgn, putindt,
  putlang, putdown, putup, putright, putleft, putcset, putcolr,
  parstart, parend, linend, putbreak, putkeep, putkeepn,
  puttdef, endtdef, puttab, tabfill, tabfstr,
  flnum, flend,
  formcall, formch, ftaguse, formpx, formsx, formrest, ftagspc,
	mapuse,
  putftn, endftn, setftn, tistart, tiend,
  putidx, idxgrp, idxend, idxtyp, markuse,
  ansgrp, anegrp, anstart, anend,
	begxref, putxref, setxref, endxref, xrfform, sethref, setfile, settitle,
	hypopt, hyploc, hypunq, hypfile, hyplnk, hypobj, hypcsh,
	hypall, hypurl, hypmac, hyppag, hyppgn,  hyptarg, alink, idxref, idxprp,
  formtbl,                     // in dwhtmtb.cc
  puttbl, endtbl, tbplace, tbeplac, tbldflt, tbedflt,
  tblrult, tblrulb, tblrull, tblrulr,
	tblalrc, tblncnt, tblacnt, tblaclr, tblafll,
  tbcol, tbcend, tbcols, tbcwid, tbcwids, tbcwtyp,
  tbccolr, tbcfill, tbcrull, tbcrulr,
  tbrow, tbrend, tbrhigh, tbrhfix, tbrhmin, tbrhmax, tbrcolr, tbrfill,
  tbrrult, tbrrulb, tbrruls,
  tbcell, tbclend, tbclstrd, tbclrstr, tbclcolr, tbclfill,
  tbclrult, tbclrulb, tbclrull, tbclrulr,
  tbclmart, tbclmarb, tbclmarl, tbclmarr,
  tbclmrtt, tbclmrtb, tbclmrtl, tbclmrtr,
	tbclvalg,
  tbtplace, tbtgap, tbltitl, tbetitl,
	conduse, condend, condruse, condclass,
  lncolr, lnthck, putfr,       // in dwhtmgr.cc
  object, putobj, objiprop, objtprop, 
  objparam, objeparm, objnparm, objvparm, objtparm,
	putvar, putavar, putvsys,            // in dwhtman.cc
  htmlast
};

enum dc_hier {
 ParentError = 1, LevelError = 2, InvalidTag = 3,
 RootTagNum = 1, MapLevMax = 128, TagLevMax = 256
};

enum dc_eltype {  // bits for element type in content models
 MixedType = 1, InLineType = 2, PreformType = 4
};

enum rangetype {  // types for CharacterRangeClasses
 RTnone = 0, RTany = 1, RTclass = 2
};


struct htm {
  dc_dtyp ctldt;
  uns ctlmaj;
  uns ctlmin;
  uns ctldat;
  scantype scan_op;
  modtype module;
  htmtype write_op;
};

struct fmtvals;


class HTMctl : public DCctl {
 public:
  htm *rp;
  HTMctl() { set(ilong); rp = NULL; }
  HTMctl(dc_dtyp dtp, uns mj = 0, uns mn = 0, uns d = 0, unl s = 0)
    { dtype = dtp; major = (unc) mj; minor = (unc) mn; data = d; size = s; ext = NULL;
      rp = NULL; }
  HTMctl(HTMctl& c)
    { dtype = c.dtype; major = c.major; minor = c.minor; data = c.data;
      size = c.ext ? 0 : c.size; ext = NULL; rp = NULL; }
  HTMctl(DCLrfile& rf) { *this = rf; }

  virtual void write(DCfile& wf);
  HTMctl& operator= (DCLrfile& rf)
   { return (HTMctl &) rf.getCtl(*this); }
  HTMctl& operator= (DCblock& b)
    { if (ext) delete [] ext; ext = b.p(); size = b.l(); b.clear();
      return (HTMctl&) *this; }
};


class HTMitem {
 public:
  HTMctl *ctl;
  HTMitem *next;

  HTMitem() : ctl(NULL), next(NULL) { }
  HTMitem(HTMctl *cp) : ctl(cp), next(NULL) { }
  HTMitem(HTMctl *cp, HTMitem *n) : ctl(cp), next(n) { }
  virtual ~HTMitem() { }
  virtual void insert(HTMctl *nc, HTMctl *oc)
    { if (!next) return;
      if (next->ctl == oc) next = new HTMitem(nc, next);
      else next->insert(nc, oc); }
  virtual void replace(HTMctl *nc, HTMctl *oc)
    { if (!next) return;
      if (next->ctl == oc) next->ctl = nc;
      else next->replace(nc, oc); }
};


class HTMlink {
 protected:
  HTMitem *first, *last;
 public:
  HTMlink() : first(NULL), last(NULL) { }
  virtual ~HTMlink()
		{ HTMitem *that = NULL; 
			while (first) {
				that = first->next;
				delete first;
				first = that;
			}
		}
};


class HTMgroup : public HTMctl, public HTMlink {
 public:
  HTMgroup() : HTMctl(group), HTMlink() { }
  HTMgroup(HTMctl& c) : HTMctl(c), HTMlink() { }
  virtual ~HTMgroup() { }
  void add(HTMctl& c)
    { if (!first) last = first = new HTMitem(&c);
      else last = (last->next = new HTMitem(&c)); }
  void add(HTMgroup& g)
    { if (!first) last = first = new HTMitem(&g);
      else last = (last->next = new HTMitem(&g)); }
  void merge(HTMgroup& g)
    { if (&g == NULL) return;
      if (!first) first = g.first; else last->next = g.first;
      if (g.last) last = g.last;
      g.first = g.last = NULL; delete &g; }
  void prune(void)
    { if (first) delete first; first = NULL; }
  virtual void insert(HTMctl& nc, HTMctl& oc)
    { if (!first) return;
      else if (first->ctl == &oc) first = new HTMitem(&nc, first);
      else first->HTMitem::insert(&nc, &oc); }
  virtual void replace(HTMctl& nc, HTMctl& oc)
    { if (!first) return;
      else if (first->ctl == &oc) first->ctl = &nc;
      else first->HTMitem::replace(&nc, &oc); }
  virtual void replace(HTMgroup& gc, HTMctl& oc)
    { if (!first) return;
      else if (first->ctl == &oc) first->ctl = &gc;
      else first->HTMitem::replace(&gc, &oc); }
  virtual void write(DCfile& wf)
    { this->HTMctl::write(wf); HTMitem *itm = first;
      while (itm) { if (itm->ctl) itm->ctl->write(wf); itm = itm->next; } }
  virtual void execu(void (*e)(HTMctl *c))
    { (*e)((HTMctl *)this); HTMitem *itm = first;
      while (itm) { if (itm->ctl) (*e)(itm->ctl); itm = itm->next; } }
};


enum unittype {
 pg_unit, ld_unit, ft_unit, gl_unit, UnitsMax
};


struct finfo {
  char *fname;
	char *fmap;
  uns fnum;
  unc ftype;
	unc ftsym;
};


struct tinfo {
  short tpos;
  unc ttype;
  unc tlead;
};

struct chprop {
	char *pstr;
	void (*efunc)(HTMwfile *);
};

struct pprop {
  char *stag;
  uns stype;     // if stype is 1 (para),
	char *cstag;   //  this is the current char format (from its stag) or NULL
	uns snum;
	unc list;
	unc llev;
	bool lfirst;
	bool llast;
	bool lnest;
	bool lend;
	bool listdd;
	bool anuse;
	bool anskip;
	bool hpclass;
	bool hsclass;
	bool hsnclass;
	bool keeplink;
	bool nosplit;
	unc hyper;
	unc macpos;
	unc cmacpos;
	char *maccb;
	char *maccs;
	char *macce;
	char *macca;
	char *maccr;
	char *macacb;
	char *macaca;
	char *macls;
	char *maccss;
	shdinfo *cshd;
	brdrinfo *cbrdrt;
	brdrinfo *cbrdrb;
	brdrinfo *cbrdrl;
	brdrinfo *cbrdrr;
	char *cssadd;
	char *cmaccb;
	char *cmaccs;
	char *cmacce;
	char *cmacca;
	char *cmaccr;
	char *cmaccep;
	char *cmaccap;
	char *macrat;
	char *maccat;
	char *maccab;
	char *maccax;
	char *maccsc;
	char *macer;
	char *maceti;
	char *maceh;
	char *macet;
	char *maceb;
	char *hsname;
	char *hpname;
	char *hcname;
	char *hscname;
	char *dtcname;
	char *dtscname;
	char **dtptyp;
	char *dtptyps;
	char *dtptype;
	char **dtctyp;
	char *dtctyps;
	char *dtctype;
	char *plclass;
	char *cstore;
	char *tstore;
	char *fmap;
	char *tabrep;
  uns fnum;
	uns psize;
  uns fsize;
	uns osize;
	unc fsym;
  unc bold;
	unc obold;
	unc fweight;
  unc ital;
	unc oital;
  unc base;
  unc caps;
  unc scaps;
  unc strike;
	unc ostrike;
  unc line;
	unc oline;
  unc algn;
  unc valgn;
  unc lsmode;
	unc cpuse;
	unc sideh;
	unc runin;
	unc dtmhd;
  short kern;
  short lspace;
  short aspace;
  short bspace;
  short lind;
  short find;
  short rind;
  short pbreak;
  long colr;
	long bcolr;
	uns cntform;
	uns split;
	uns extract;
	uns title;
	uns contents;
	uns splitlevel;
  uns cellmargl;
  uns cellmargr;
  uns cellmargt;
  uns cellmargb;
  unc cellmartl;
  unc cellmartr;
  unc cellmartt;
  unc cellmartb;
  uns tcount;
	uns pfx;
	uns sfx;
	uns traillevel;
	uns loctoclevel;
	bool trailset;
	bool loctocset;
	bool ddlpar;
	bool ddlcst;
	bool ddlcend;
	bool ddlpst;
	bool ddlpend;
	bool ddbstb;
	bool ddbsta;
	bool ddbend;
	uns dtlev;
	long dtmap;
	char **dtpar;
	char **dtfirst;
	char **dtopb;
	char **dtopa;
	char **dtclb;
	char **dtcla;
	//char *dtepar;
	char *dttop;
	char *dstag;
	char *tstag;
	char *runpunct;
	HTMgroup *rungroup;
	char *secwin;
	char *cntwin;
	char *target;
	char *meta;
	char *flnpx;
	char *flnsx;
	char *ttlpx;
	char *ttlsx;
	char *trailpx;
	char *trailsx;
	bool config;
	bool preform;
	bool comment;
	bool notags;
	bool nopara;
	bool nofig;
	bool fig;
	bool raw;
	bool hcwtoc;
	bool rowattr;
	bool cellattr;
	bool cellnocolid;
	bool cellspan;
	bool cellcolgroup;
	bool cellrowgroup;
	bool cellscope;
	bool cellabbr;
	bool cellaxis;
	bool nobreak;
	bool hrpara;
	bool ignorewrap;
	bool nowrap;
	bool flname;
	bool finame;
	bool ipname;
	bool alink;
	bool plink;
	bool nocolor;
	bool nosize;
	bool txframes;
	bool tbhead;
	bool tbbody;
	bool keep;
	bool keepn;
	bool nohref;
	bool nocss;
	bool cssrep;
	bool trail;
	bool attr;
	bool noref;
	bool noclink;
	bool gloss;
	bool glosstitle;
	char *attrname;
  DCvlist *tabs;
};


struct condprop {
	long fcolor;
	long bcolor;
	bool bold;
	bool ital;
	bool underline;
	bool overline;
	bool strike;
	bool chbar;
	long chcolor;
	char *chchars;
	char *startimage;
	char *startalt;
	char *endimage;
	char *endalt;
};


struct tbl {
	char *tblfname;  //  format
	char *tblid;     //  FDK id
	char *tbgroup;   //  group
	uns tblncnt;     // names for ini
  uns tbccount;
  uns tbrcount;
  uns tbhcount;
  uns tbbcount;
  uns tbfcount;
  uns tblalgn;
	unc tbaccess;
	unc tbsiz;
	bool tburc;
	short tbind;
  short tbllind;
  short tblbspc;
  short tblaspc;
  short tblbrk;
	long tblpref;
  uns tblkeep;
	brdrinfo *brdrl;
	brdrinfo *brdrr;
	brdrinfo *brdrt;
	brdrinfo *brdrb;
	brdrinfo *brdra;
	brdrinfo *brdrcl;
	brdrinfo *brdrcr;
  uns tblmargl;
  uns tblmargr;
  uns tblmargt;
  uns tblmargb;
  uns titleplace;
  uns titlegap;
	uns altrowcol;
	uns normcount;
	uns altcount;
  uns altfill;
  uns altcolr;
	shdinfo *tblaltshd;
	shdinfo *tblbdyshd;
	long titleprn;
  HTMgroup *titlecont;
  DCvlist *collist;
  DCvlist *rowlist;
  DCvlist *rowtlist;
	DCnlist *attrlist;
	char *macb;      // macro before
	char *macs;      //  start
	char *macr;      //  replace
	char *mace;      //  end
	char *maca;      //  after
	char *macrs;     //  row start
	char *macre;     //   end
	char *maccs;     //  cell start
	char *macce;     //   end
	char *att;       // tbl attributes
	char *attr;      //  row
	char *attc;      //  cell
	char *atth;      //  header
	char *attf;      //  footer
	char *attb;      //  body
	char *dtname;  // DITA top-level element
	char **dtpar;  // DITA parents
	struct cell **vstrad;
	tbfmtvals* fv;
	DCvlist *hrfmts;
	long hrcount;
	DCvlist *brfmts;
	long brcount;
	DCvlist *frfmts;
	long frcount;
	char *cssclass;
};

struct col {
  uns colwid;
	uns colpct;
  uns colfill;
  uns colcolr;
	shdinfo *colshd;
	brdrinfo *brdrl;
	brdrinfo *brdrr;
	bool alt;
	char *dtname;
};

struct rule {
  uns rwid;
  uns rlines;
  uns rulcolr;
};

struct row {
  uns rtype;
  short rhigh;
  short rhfix;
  short rhmin;
  short rhmax;
  short cmargx;
	long rowref;
  DCvlist *celllist;
  uns rowfill;
  uns rowcolr;
	shdinfo *rowshd;
	brdrinfo *brdrt;
	brdrinfo *brdrb;
	brdrinfo *brdrx;
	rowfmtvals* fv;
	long rccount;
	long rhcount;
	long rbcount;
	DCvlist *hcfmts;
	long hccount;
	DCvlist *bcfmts;
	long bccount;
	uns colstrad;
	bool startgroup;
	bool alt;
	char *styleattr;
	DCnlist *attrlist;
	char *cssclass;
};

struct cell {
  short cwid;
  unc algn;        // cell content hor alignment, 0..3
  unc valgn;       // cell content vert alignment, 0..3
  uns straddling;
  bool straddled;
	bool nocolid;
	bool span;
	bool colgroup;
	bool rowgroup;
	bool tbhead;
	bool tbbody;
	uns rowstrad;
  uns cellmargl;
  uns cellmargr;
  uns cellmargt;
  uns cellmargb;
  unc cellmartl;
  unc cellmartr;
  unc cellmartt;
  unc cellmartb;
	//unc cellvalgn;
  uns cellfill;    // fill or shading, 0 = none, 1..12 = pattern, else %*100
  uns cellcolr;
	shdinfo *cellshd;
	brdrinfo *brdrl;
	brdrinfo *brdrr;
	brdrinfo *brdrt;
	brdrinfo *brdrb;
	char *styleattr;
	char *scope;
	char *abbr;
	char *axis;
	char *id;
	DCnlist *attrlist;
  HTMgroup *cont;
	cellfmtvals* fv;
	char *cssclass;
};

struct tblprop {
	char *ttyp;
	bool ttitle;
	char *tdesc;
	char *tgroup;

	char *tcspec;
	char *csnatt;
	char *csiatt;
	char *cswatt;
	long ccmax;
	long hrmax;
	long frmax;

	char *hgroup;
	char *hrow;
	char *hcell;
	DCnlist *hclist;

	char *bgroup;
	char *brow;
	char *bcell;
	DCnlist *bclist;

	char *fgroup;
	char *frow;
	char *fcell;
	DCnlist *fclist;

	char *rsatt;

	bool csnmuse;
	char *cscatt;
	char *csnsatt;
	char *csneatt;
	char *cnmatt;
	char *csnpx;
	bool calgnatt;
};


struct page {
  short pnum;
  uns ptype;
  uns pusage;
  uns porient;
  uns phand;
  uns pback;
  short pwide;
  short phigh;
  short plmrg;
  short prmrg;
  short ptmrg;
  short pbmrg;
  short pcols;
  short gutter;
};

struct cntprop {
	char *title;
	char *ref;
	char *win;
	uns level;
	bool noref;
};

struct ixprop {
	char *sort;
	char *name;
	char *title;
	char *ref;
	char *win;
	long refid;
};

struct ixitem {
  long level;
	char *sort;
	char *title;
	char *self;
	DClist *reflist;
};

struct glprop {
	char *term;
	char *id;
	long fn;
};

struct mapprop {
	char *name;
	char *ref;
	char *win;
};

struct dmapprop {
	char *ntitle; // put in navtitle attr
	char *ltext; // put in topicref's topicmeta
	char *stitle; // put in topicref's topicmeta
	char *sdesc;  // put in topicref's topicmeta
	char *ref;    // href attr, filename
	char *tid;    // href attr, topicid
	char *typ;    // type attr, root element
	long level;   // map nesting level [DITAMapLevel]
	bool noref;   // true if head only, in topichead
	bool navref;  // like Eclipse link
	bool anchor;  // like Eclipse anchor
	bool mapref;  // for ditamap references
	bool locktitle;
	bool notoc;
	DCnlist *attrs;
	DCnlist *attrnames;
	long attrcount;
};


struct dcframe {
  short fnum;
  short fpage;
  uns fusage;
  uns fmtype;
  uns fshare;
  uns fanchor;
  short fapage;
  uns ftext;
  uns ftprev;
  uns ftnext;
  uns halgn;
  uns valgn;
  uns fpos;
  uns fzpos;
  uns flim;
  uns ftbl;
  uns frot;
  uns fcrop;
  uns fauto;
  uns fshape;
  uns frvis;
  uns fcolr;
  uns fthck;
  uns ffill;
  uns refpt;
  uns irefpt;
  short xoff;
  short yoff;
  short xpos;
  short ypos;
  short wide;
  short high;
  short iwide;
  short ihigh;
  uns rang;
  uns vcount;
	unl fmdpi;
  short *vertx;
};

struct frame {
	char *grfile;
	char *grname;
 	char *grgroup;
  uns fpos;
  short fpage;
  uns frul[2];
  uns frulsp;
  uns ffill;
  uns irefpt;
  uns href;
  uns vref;
  uns halgn;
  short xpos;
  uns valgn;
  short ypos;
  short wide;
  short high;
  short iwide;
  short ihigh;
  short xspace;
  uns ftext;
	uns prtype;
	unl fmdpi;
	long id;
	bool mathobj;
	bool presuf;
	uns grcount;
  uns frcount;
  DCvlist *grlist;
  DCvlist *frlist;
	DCnlist *attrlist;
	DClist *gslist;
	frame *parent;
	char *math;
	char *alt;
	char *longdesc;
	char *gclass;
	DClist *proplist;
	DClist *cfglist;
};

struct area {
	long left;
	long top;
	long right;
	long bottom;
	long *circ;
	long *poly;
	long pcnt;
	char *shape;
	char *alt;
	char *ref;
	char *base;
	char *file;
	char *target;
	bool url;
	bool popup;
};

struct link {
	char *lid;
	char *ref;
	char *nfile;
	size_t nlen;
	char *nlink;
	size_t nllen;
	char *llink;
	char *lpath;
	char *lfile;
	char *rfile;
	size_t lloc;
	size_t llen;
	size_t lpad;
	unl props;
};

struct extlink {
	char *lid;
	char *reftxt;
	char *lnktxt;
	char *rfile;
	size_t lloc;
	size_t llen;
	unl props;
};

struct ecrefs { // Eclipse context refs
	char *ctitle;
	char *cfile;
	char *cref;
};

struct list {
	bool used;
	unc typeval;
	uns level;
	uns last;
	uns pos[ListLevMax];
	unc type[ListLevMax];
	char *iname[ListLevMax];
};

struct xnest {
	char *tag;
	char *mactxt;
	uns lev;
};


struct pvars {
	bool storeText;
	bool nonWhiteText;
	bool configHere;
	bool alinkHere;
	bool nameHere;
	bool splitHere;
	bool fileNameHere;
	bool finalIniNameHere;
	bool iniPartNameHere;
	bool titleHere;
	bool titleSet;
	bool inRaw;
	bool attrHere;
	bool tStoreHere;
	bool glossHere;
	uns contents;
	uns titleBufCount;
	uns nameBufCount;
	uns currFormNum;
	long contentsId;
	long localTOCLevel;
	uns trailLevel;
	char *contentsRef;
	char *contentsWin;
	char *metaName;
	char *attrMarkerName;
	unc *titleBuf;
	unc *nameBuf;

	char *dITATag;
	char *dITATagID;
	char *dITAAlias;
	long dITALevel;
	char *dITAParent;
	char *dITAOpenBefore;
	char *dITAOpenAfter;
	char **dITAOpenBeforeArr;
	char **dITAOpenAfterArr;
	char *dITACloseBefore;
	char *dITACloseAfter;
	char **dITACloseBeforeArr;
	char **dITACloseAfterArr;
	char *dITAFirst;
	long paraRefNum;
	DClist *intRefPend;
};

struct tagset {
	char *setname;
	char *start;
	char *dtype;
	char *dtd;
	DCnlist *names;
	DCnlist *esetnames;
	DCvlist *esetlists;
	DCilist *levels;
  DCilist *single;
	DCvlist *parents;
  DCilist *inlin;
  DCilist *atfirst;
	DCnlist *tblnames;
	DCvlist *tbllists;
};

typedef void (*tagfunc)(HTMwfile *, char *, long, bool);

class DTtag {
	friend class HTMwr;
	friend class HXMwr;

	// in dwhtmds.cpp
	static void SetDITAVer(void);
	static void DTtag::SetDITAVerSets(unc dval);

	// in dwhtmdd.cpp
	static npair *DTopicTypesSet;
	static npair *DTopicTypesSets[];

	static npair *DTopicStartsSet;
	static npair *DTopicStartsSets[];

	static npair *DRelTopicTypesSet;
	static npair *DRelTopicTypesSets[];

	static npair *CommonNamesSet;
	static npair *CommonNamesSets[];

	static char *PrologDTypeDita;
	static char *PrologDTypeDitas[];

	static char *PrologDTDDita;
	static char *PrologDTDDitas[];

	static char *PrologDTypeTopic;
	static char *PrologDTypeTopics[];

	static char *PrologDTDTopic;
	static char *PrologDTDTopics[];

	static char *PrologDTypeConcept;
	static char *PrologDTypeConcepts[];

	static char *PrologDTDConcept;
	static char *PrologDTDConcepts[];

	static char *PrologDTypeTask;
	static char *PrologDTypeTasks[];

	static char *PrologDTDTask;
	static char *PrologDTDTasks[];

	static char *PrologDTypeReference;
	static char *PrologDTypeReferences[];

	static char *PrologDTDReference;
	static char *PrologDTDReferences[];

	static char *PrologDTypeGlossentry;
	static char *PrologDTypeGlossentrys[];

	static char *PrologDTDGlossentry;
	static char *PrologDTDGlossentrys[];

	static char *PrologDTypeMap;
	static char *PrologDTypeMaps[];

	static char *PrologDTDMap;
	static char *PrologDTDMaps[];

	static char *PrologDTypeBookmap;
	static char *PrologDTypeBookmaps[];

	static char *PrologDTDBookmap;
	static char *PrologDTDBookmaps[];

	static npair *TopicNamesSet;
	static npair *TopicNamesSets[];
	static ipair *TopicInlineSet;
	static ipair *TopicInlineSets[];
	static ipair *TopicSingleSet;
	static ipair *TopicSingleSets[];
	static vpair *TopicElemSetsSet;
	static vpair *TopicElemSetsSets[];
	static npair *TopicElemSetNamesSet;
	static npair *TopicElemSetNamesSets[];
	static ipair *TopicLevelsSet;
	static ipair *TopicLevelsSets[];
	static ipair *TopicAtFirstSet;
	static ipair *TopicAtFirstSets[];

	static npair *ConceptNamesSet;
	static npair *ConceptNamesSets[];
	static ipair *ConceptInlineSet;
	static ipair *ConceptInlineSets[];
	static ipair *ConceptSingleSet;
	static ipair *ConceptSingleSets[];
	static vpair *ConceptElemSetsSet;
	static vpair *ConceptElemSetsSets[];
	static npair *ConceptElemSetNamesSet;
	static npair *ConceptElemSetNamesSets[];
	static ipair *ConceptLevelsSet;
	static ipair *ConceptLevelsSets[];
	static ipair *ConceptAtFirstSet;
	static ipair *ConceptAtFirstSets[];

	static npair *TaskNamesSet;
	static npair *TaskNamesSets[];
	static ipair *TaskInlineSet;
	static ipair *TaskInlineSets[];
	static ipair *TaskSingleSet;
	static ipair *TaskSingleSets[];
	static vpair *TaskElemSetsSet;
	static vpair *TaskElemSetsSets[];
	static npair *TaskElemSetNamesSet;
	static npair *TaskElemSetNamesSets[];
	static ipair *TaskLevelsSet;
	static ipair *TaskLevelsSets[];
	static ipair *TaskAtFirstSet;
	static ipair *TaskAtFirstSets[];

	static npair *ReferenceNamesSet;
	static npair *ReferenceNamesSets[];
	static ipair *ReferenceInlineSet;
	static ipair *ReferenceInlineSets[];
	static ipair *ReferenceSingleSet;
	static ipair *ReferenceSingleSets[];
	static vpair *ReferenceElemSetsSet;
	static vpair *ReferenceElemSetsSets[];
	static npair *ReferenceElemSetNamesSet;
	static npair *ReferenceElemSetNamesSets[];
	static ipair *ReferenceLevelsSet;
	static ipair *ReferenceLevelsSets[];
	static ipair *ReferenceAtFirstSet;
	static ipair *ReferenceAtFirstSets[];

	static npair *GlossentryNamesSet;
	static npair *GlossentryNamesSets[];
	static ipair *GlossentryInlineSet;
	static ipair *GlossentryInlineSets[];
	static ipair *GlossentrySingleSet;
	static ipair *GlossentrySingleSets[];
	static vpair *GlossentryElemSetsSet;
	static vpair *GlossentryElemSetsSets[];
	static npair *GlossentryElemSetNamesSet;
	static npair *GlossentryElemSetNamesSets[];
	static ipair *GlossentryLevelsSet;
	static ipair *GlossentryLevelsSets[];
	static ipair *GlossentryAtFirstSet;
	static ipair *GlossentryAtFirstSets[];

	static npair *MapNamesSet;
	static npair *MapNamesSets[];
	static ipair *MapInlineSet;
	static ipair *MapInlineSets[];
	static ipair *MapSingleSet;
	static ipair *MapSingleSets[];
	static vpair *MapElemSetsSet;
	static vpair *MapElemSetsSets[];
	static npair *MapElemSetNamesSet;
	static npair *MapElemSetNamesSets[];
	static ipair *MapLevelsSet;
	static ipair *MapLevelsSets[];
	static ipair *MapAtFirstSet;
	static ipair *MapAtFirstSets[];

	static npair *BookmapNamesSet;
	static npair *BookmapNamesSets[];
	static ipair *BookmapInlineSet;
	static ipair *BookmapInlineSets[];
	static ipair *BookmapSingleSet;
	static ipair *BookmapSingleSets[];
	static vpair *BookmapElemSetsSet;
	static vpair *BookmapElemSetsSets[];
	static npair *BookmapElemSetNamesSet;
	static npair *BookmapElemSetNamesSets[];
	static ipair *BookmapLevelsSet;
	static ipair *BookmapLevelsSets[];
	static ipair *BookmapAtFirstSet;
	static ipair *BookmapAtFirstSets[];


	// in dwhtmd0.cpp
	static npair DTopicTypesSet0[];
	static npair DTopicStartsSet0[];
	static npair DRelTopicTypesSet0[];
	static npair CommonNamesSet0[];

	static char *PrologDTypeDita0;
	static char *PrologDTDDita0;

	static char *PrologDTypeTopic0;
	static char *PrologDTDTopic0;

	static char *PrologDTypeConcept0;
	static char *PrologDTDConcept0;

	static char *PrologDTypeTask0;
	static char *PrologDTDTask0;

	static char *PrologDTypeReference0;
	static char *PrologDTDReference0;

	static char *PrologDTypeMap0;
	static char *PrologDTDMap0;

	static npair TopicNamesSet0[];
	static ipair TopicInlineSet0[];
	static ipair TopicSingleSet0[];
	static long Part1TopicPar0[];
	static long Part2TopicPar0[];
	static long Part3TopicPar0[];
	static long Part4TopicPar0[];
	static long Part5TopicPar0[];
	static long Part6TopicPar0[];
	static long Part7TopicPar0[];
	static long Part8TopicPar0[];
	static long Part9TopicPar0[];
	static long Part10TopicPar0[];
	static long Part11TopicPar0[];
	static long Part12TopicPar0[];
	static long Part13TopicPar0[];
	static long Part14TopicPar0[];
	static long citeTopicPar0[];
	static long codephTopicPar0[];
	static long descTopicPar0[];
	static long dlTopicPar0[];
	static long draftcommentTopicPar0[];
	static long figTopicPar0[];
	static long figgroupTopicPar0[];
	static long fnTopicPar0[];
	static long fragmentTopicPar0[];
	static long groupseqTopicPar0[];
	static long imageTopicPar0[];
	static long indextermTopicPar0[];
	static long itemgroupTopicPar0[];
	static long keywordTopicPar0[];
	static long liTopicPar0[];
	static long linkTopicPar0[];
	static long linklistTopicPar0[];
	static long linkpoolTopicPar0[];
	static long lqTopicPar0[];
	static long noteTopicPar0[];
	static long objectTopicPar0[];
	static long optionTopicPar0[];
	static long pTopicPar0[];
	static long phTopicPar0[];
	static long qTopicPar0[];
	static long repsepTopicPar0[];
	static long requiredcleanupTopicPar0[];
	static long rowTopicPar0[];
	static long simpletableTopicPar0[];
	static long stentryTopicPar0[];
	static long tableTopicPar0[];
	static long termTopicPar0[];
	static long titleTopicPar0[];
	static long tmTopicPar0[];
	static long uicontrolTopicPar0[];
	static long varTopicPar0[];
	static long xrefTopicPar0[];
	static long titlefirstTopicPar0[];
	static vpair TopicElemSetsSet0[];
	static npair TopicElemSetNamesSet0[];
	static ipair TopicLevelsSet0[];
	static ipair TopicAtFirstSet0[];


	static npair ConceptNamesSet0[];
	static ipair ConceptInlineSet0[];
	static ipair ConceptSingleSet0[];
	static long Part1ConceptPar0[];
	static long Part2ConceptPar0[];
	static long Part3ConceptPar0[];
	static long Part4ConceptPar0[];
	static long Part5ConceptPar0[];
	static long Part6ConceptPar0[];
	static long Part7ConceptPar0[];
	static long Part8ConceptPar0[];
	static long Part9ConceptPar0[];
	static long Part10ConceptPar0[];
	static long Part11ConceptPar0[];
	static long Part12ConceptPar0[];
	static long Part13ConceptPar0[];
	static long Part14ConceptPar0[];
	static long citeConceptPar0[];
	static long codephConceptPar0[];
	static long descConceptPar0[];
	static long dlConceptPar0[];
	static long draftcommentConceptPar0[];
	static long figConceptPar0[];
	static long figgroupConceptPar0[];
	static long fnConceptPar0[];
	static long fragmentConceptPar0[];
	static long groupseqConceptPar0[];
	static long imageConceptPar0[];
	static long indextermConceptPar0[];
	static long itemgroupConceptPar0[];
	static long keywordConceptPar0[];
	static long liConceptPar0[];
	static long linkConceptPar0[];
	static long linklistConceptPar0[];
	static long linkpoolConceptPar0[];
	static long lqConceptPar0[];
	static long noteConceptPar0[];
	static long objectConceptPar0[];
	static long optionConceptPar0[];
	static long pConceptPar0[];
	static long phConceptPar0[];
	static long qConceptPar0[];
	static long repsepConceptPar0[];
	static long requiredcleanupConceptPar0[];
	static long rowConceptPar0[];
	static long simpletableConceptPar0[];
	static long stentryConceptPar0[];
	static long tableConceptPar0[];
	static long termConceptPar0[];
	static long titleConceptPar0[];
	static long tmConceptPar0[];
	static long uicontrolConceptPar0[];
	static long varConceptPar0[];
	static long xrefConceptPar0[];
	static long titlefirstConceptPar0[];
	static vpair ConceptElemSetsSet0[];
	static npair ConceptElemSetNamesSet0[];
	static ipair ConceptLevelsSet0[];
	static ipair ConceptAtFirstSet0[];


	static npair TaskNamesSet0[];
	static ipair TaskInlineSet0[];
	static ipair TaskSingleSet0[];
	static long Part1TaskPar0[];
	static long Part2TaskPar0[];
	static long Part3TaskPar0[];
	static long Part4TaskPar0[];
	static long Part5TaskPar0[];
	static long Part6TaskPar0[];
	static long Part7TaskPar0[];
	static long Part8TaskPar0[];
	static long Part9TaskPar0[];
	static long Part10TaskPar0[];
	static long Part11TaskPar0[];
	static long Part12TaskPar0[];
	static long Part13TaskPar0[];
	static long Part14TaskPar0[];
	static long Part15TaskPar0[];
	static long Part16TaskPar0[];
	static long Part17TaskPar0[];
	static long citeTaskPar0[];
	static long cmdTaskPar0[];
	static long codephTaskPar0[];
	static long descTaskPar0[];
	static long dlTaskPar0[];
	static long draftcommentTaskPar0[];
	static long figTaskPar0[];
	static long figgroupTaskPar0[];
	static long fnTaskPar0[];
	static long fragmentTaskPar0[];
	static long groupseqTaskPar0[];
	static long imageTaskPar0[];
	static long indextermTaskPar0[];
	static long itemgroupTaskPar0[];
	static long keywordTaskPar0[];
	static long liTaskPar0[];
	static long linkTaskPar0[];
	static long linklistTaskPar0[];
	static long linkpoolTaskPar0[];
	static long lqTaskPar0[];
	static long noteTaskPar0[];
	static long objectTaskPar0[];
	static long optionTaskPar0[];
	static long pTaskPar0[];
	static long phTaskPar0[];
	static long qTaskPar0[];
	static long repsepTaskPar0[];
	static long rowTaskPar0[];
	static long simpletableTaskPar0[];
	static long stentryTaskPar0[];
	static long stepTaskPar0[];
	static long tableTaskPar0[];
	static long termTaskPar0[];
	static long titleTaskPar0[];
	static long tmTaskPar0[];
	static long uicontrolTaskPar0[];
	static long varTaskPar0[];
	static long xrefTaskPar0[];
	static long titlefirstTaskPar0[];
	static vpair TaskElemSetsSet0[];
	static npair TaskElemSetNamesSet0[];
	static ipair TaskLevelsSet0[];
	static ipair TaskAtFirstSet0[];


	static npair ReferenceNamesSet0[];
	static ipair ReferenceInlineSet0[];
	static ipair ReferenceSingleSet0[];
	static long Part1ReferencePar0[];
	static long Part2ReferencePar0[];
	static long Part3ReferencePar0[];
	static long Part4ReferencePar0[];
	static long Part5ReferencePar0[];
	static long Part6ReferencePar0[];
	static long Part7ReferencePar0[];
	static long Part8ReferencePar0[];
	static long Part9ReferencePar0[];
	static long Part10ReferencePar0[];
	static long Part11ReferencePar0[];
	static long Part12ReferencePar0[];
	static long Part13ReferencePar0[];
	static long Part14ReferencePar0[];
	static long Part15ReferencePar0[];
	static long citeReferencePar0[];
	static long codephReferencePar0[];
	static long descReferencePar0[];
	static long dlReferencePar0[];
	static long draftcommentReferencePar0[];
	static long figReferencePar0[];
	static long figgroupReferencePar0[];
	static long fnReferencePar0[];
	static long fragmentReferencePar0[];
	static long groupseqReferencePar0[];
	static long imageReferencePar0[];
	static long indextermReferencePar0[];
	static long itemgroupReferencePar0[];
	static long keywordReferencePar0[];
	static long liReferencePar0[];
	static long linkReferencePar0[];
	static long linklistReferencePar0[];
	static long linkpoolReferencePar0[];
	static long lqReferencePar0[];
	static long noteReferencePar0[];
	static long objectReferencePar0[];
	static long optionReferencePar0[];
	static long pReferencePar0[];
	static long phReferencePar0[];
	static long qReferencePar0[];
	static long repsepReferencePar0[];
	static long rowReferencePar0[];
	static long simpletableReferencePar0[];
	static long stentryReferencePar0[];
	static long tableReferencePar0[];
	static long termReferencePar0[];
	static long titleReferencePar0[];
	static long tmReferencePar0[];
	static long uicontrolReferencePar0[];
	static long varReferencePar0[];
	static long xrefReferencePar0[];
	static long titlefirstReferencePar0[];
	static vpair ReferenceElemSetsSet0[];
	static npair ReferenceElemSetNamesSet0[];
	static ipair ReferenceLevelsSet0[];
	static ipair ReferenceAtFirstSet0[];


	static npair MapNamesSet0[];
	static ipair MapInlineSet0[];
	static ipair MapSingleSet0[];
	static long indextermMapPar0[];
	static long keywordMapPar0[];
	static long navrefMapPar0[];
	static long topicmetaMapPar0[];
	static long topicrefMapPar0[];
	static long topicmetafirstMapPar0[];
	static vpair MapElemSetsSet0[];
	static npair MapElemSetNamesSet0[];
	static ipair MapLevelsSet0[];
	static ipair MapAtFirstSet0[];


	// in dwhtmd1.cpp
	static npair DTopicTypesSet1[];
	static npair DTopicStartsSet1[];
	static npair DRelTopicTypesSet1[];
	static npair CommonNamesSet1[];

	static char *PrologDTypeDita1;
	static char *PrologDTDDita1;

	static char *PrologDTypeTopic1;
	static char *PrologDTDTopic1;

	static char *PrologDTypeConcept1;
	static char *PrologDTDConcept1;

	static char *PrologDTypeTask1;
	static char *PrologDTDTask1;

	static char *PrologDTypeReference1;
	static char *PrologDTDReference1;

	static char *PrologDTypeGlossentry1;
	static char *PrologDTDGlossentry1;

	static char *PrologDTypeMap1;
	static char *PrologDTDMap1;

	static char *PrologDTypeBookmap1;
	static char *PrologDTDBookmap1;

	static npair TopicNamesSet1[];
	static ipair TopicInlineSet1[];
	static ipair TopicSingleSet1[];
	static long Part1TopicPar1[];
	static long Part2TopicPar1[];
	static long Part3TopicPar1[];
	static long Part4TopicPar1[];
	static long Part5TopicPar1[];
	static long Part6TopicPar1[];
	static long Part7TopicPar1[];
	static long Part8TopicPar1[];
	static long Part9TopicPar1[];
	static long Part10TopicPar1[];
	static long Part11TopicPar1[];
	static long Part12TopicPar1[];
	static long Part13TopicPar1[];
	static long Part14TopicPar1[];
	static long Part15TopicPar1[];
	static long Part16TopicPar1[];
	static long Part17TopicPar1[];
	static long citeTopicPar1[];
	static long codephTopicPar1[];
	static long dataTopicPar1[];
	static long descTopicPar1[];
	static long dlTopicPar1[];
	static long draftcommentTopicPar1[];
	static long figTopicPar1[];
	static long figgroupTopicPar1[];
	static long fnTopicPar1[];
	static long foreignTopicPar1[];
	static long fragmentTopicPar1[];
	static long groupseqTopicPar1[];
	static long imageTopicPar1[];
	static long indextermTopicPar1[];
	static long itemgroupTopicPar1[];
	static long keywordTopicPar1[];
	static long liTopicPar1[];
	static long linkTopicPar1[];
	static long linklistTopicPar1[];
	static long linkpoolTopicPar1[];
	static long lqTopicPar1[];
	static long noteTopicPar1[];
	static long objectTopicPar1[];
	static long optionTopicPar1[];
	static long pTopicPar1[];
	static long phTopicPar1[];
	static long qTopicPar1[];
	static long repsepTopicPar1[];
	static long requiredcleanupTopicPar1[];
	static long rowTopicPar1[];
	static long shortdescTopicPar1[];
	static long simpletableTopicPar1[];
	static long stentryTopicPar1[];
	static long tableTopicPar1[];
	static long termTopicPar1[];
	static long titleTopicPar1[];
	static long tmTopicPar1[];
	static long uicontrolTopicPar1[];
	static long varTopicPar1[];
	static long xrefTopicPar1[];
	static long titlefirstTopicPar1[];
	static vpair TopicElemSetsSet1[];
	static npair TopicElemSetNamesSet1[];
	static ipair TopicLevelsSet1[];
	static ipair TopicAtFirstSet1[];


	static npair ConceptNamesSet1[];
	static ipair ConceptInlineSet1[];
	static ipair ConceptSingleSet1[];
	static long Part1ConceptPar1[];
	static long Part2ConceptPar1[];
	static long Part3ConceptPar1[];
	static long Part4ConceptPar1[];
	static long Part5ConceptPar1[];
	static long Part6ConceptPar1[];
	static long Part7ConceptPar1[];
	static long Part8ConceptPar1[];
	static long Part9ConceptPar1[];
	static long Part10ConceptPar1[];
	static long Part11ConceptPar1[];
	static long Part12ConceptPar1[];
	static long Part13ConceptPar1[];
	static long Part14ConceptPar1[];
	static long Part15ConceptPar1[];
	static long Part16ConceptPar1[];
	static long Part17ConceptPar1[];
	static long citeConceptPar1[];
	static long codephConceptPar1[];
	static long dataConceptPar1[];
	static long descConceptPar1[];
	static long dlConceptPar1[];
	static long draftcommentConceptPar1[];
	static long figConceptPar1[];
	static long figgroupConceptPar1[];
	static long fnConceptPar1[];
	static long foreignConceptPar1[];
	static long fragmentConceptPar1[];
	static long groupseqConceptPar1[];
	static long imageConceptPar1[];
	static long indextermConceptPar1[];
	static long itemgroupConceptPar1[];
	static long keywordConceptPar1[];
	static long liConceptPar1[];
	static long linkConceptPar1[];
	static long linklistConceptPar1[];
	static long linkpoolConceptPar1[];
	static long lqConceptPar1[];
	static long noteConceptPar1[];
	static long objectConceptPar1[];
	static long optionConceptPar1[];
	static long pConceptPar1[];
	static long phConceptPar1[];
	static long qConceptPar1[];
	static long repsepConceptPar1[];
	static long requiredcleanupConceptPar1[];
	static long rowConceptPar1[];
	static long shortdescConceptPar1[];
	static long simpletableConceptPar1[];
	static long stentryConceptPar1[];
	static long tableConceptPar1[];
	static long termConceptPar1[];
	static long titleConceptPar1[];
	static long tmConceptPar1[];
	static long uicontrolConceptPar1[];
	static long varConceptPar1[];
	static long xrefConceptPar1[];
	static long titlefirstConceptPar1[];
	static vpair ConceptElemSetsSet1[];
	static npair ConceptElemSetNamesSet1[];
	static ipair ConceptLevelsSet1[];
	static ipair ConceptAtFirstSet1[];


	static npair TaskNamesSet1[];
	static ipair TaskInlineSet1[];
	static ipair TaskSingleSet1[];
	static long Part1TaskPar1[];
	static long Part2TaskPar1[];
	static long Part3TaskPar1[];
	static long Part4TaskPar1[];
	static long Part5TaskPar1[];
	static long Part6TaskPar1[];
	static long Part7TaskPar1[];
	static long Part8TaskPar1[];
	static long Part9TaskPar1[];
	static long Part10TaskPar1[];
	static long Part11TaskPar1[];
	static long Part12TaskPar1[];
	static long Part13TaskPar1[];
	static long Part14TaskPar1[];
	static long Part15TaskPar1[];
	static long Part16TaskPar1[];
	static long Part17TaskPar1[];
	static long Part18TaskPar1[];
	static long Part19TaskPar1[];
	static long Part20TaskPar1[];
	static long citeTaskPar1[];
	static long cmdTaskPar1[];
	static long codephTaskPar1[];
	static long dataTaskPar1[];
	static long descTaskPar1[];
	static long dlTaskPar1[];
	static long draftcommentTaskPar1[];
	static long figTaskPar1[];
	static long figgroupTaskPar1[];
	static long fnTaskPar1[];
	static long foreignTaskPar1[];
	static long fragmentTaskPar1[];
	static long groupseqTaskPar1[];
	static long imageTaskPar1[];
	static long indextermTaskPar1[];
	static long itemgroupTaskPar1[];
	static long keywordTaskPar1[];
	static long liTaskPar1[];
	static long linkTaskPar1[];
	static long linklistTaskPar1[];
	static long linkpoolTaskPar1[];
	static long lqTaskPar1[];
	static long noteTaskPar1[];
	static long objectTaskPar1[];
	static long optionTaskPar1[];
	static long pTaskPar1[];
	static long phTaskPar1[];
	static long qTaskPar1[];
	static long repsepTaskPar1[];
	static long rowTaskPar1[];
	static long shortdescTaskPar1[];
	static long simpletableTaskPar1[];
	static long stentryTaskPar1[];
	static long stepTaskPar1[];
	static long tableTaskPar1[];
	static long termTaskPar1[];
	static long titleTaskPar1[];
	static long tmTaskPar1[];
	static long uicontrolTaskPar1[];
	static long varTaskPar1[];
	static long xrefTaskPar1[];
	static long titlefirstTaskPar1[];
	static vpair TaskElemSetsSet1[];
	static npair TaskElemSetNamesSet1[];
	static ipair TaskLevelsSet1[];
	static ipair TaskAtFirstSet1[];


	static npair ReferenceNamesSet1[];
	static ipair ReferenceInlineSet1[];
	static ipair ReferenceSingleSet1[];
	static long Part1ReferencePar1[];
	static long Part2ReferencePar1[];
	static long Part3ReferencePar1[];
	static long Part4ReferencePar1[];
	static long Part5ReferencePar1[];
	static long Part6ReferencePar1[];
	static long Part7ReferencePar1[];
	static long Part8ReferencePar1[];
	static long Part9ReferencePar1[];
	static long Part10ReferencePar1[];
	static long Part11ReferencePar1[];
	static long Part12ReferencePar1[];
	static long Part13ReferencePar1[];
	static long Part14ReferencePar1[];
	static long Part15ReferencePar1[];
	static long Part16ReferencePar1[];
	static long Part17ReferencePar1[];
	static long Part18ReferencePar1[];
	static long Part19ReferencePar1[];
	static long Part20ReferencePar1[];
	static long citeReferencePar1[];
	static long codephReferencePar1[];
	static long dataReferencePar1[];
	static long descReferencePar1[];
	static long dlReferencePar1[];
	static long draftcommentReferencePar1[];
	static long figReferencePar1[];
	static long figgroupReferencePar1[];
	static long fnReferencePar1[];
	static long foreignReferencePar1[];
	static long fragmentReferencePar1[];
	static long groupseqReferencePar1[];
	static long imageReferencePar1[];
	static long indextermReferencePar1[];
	static long itemgroupReferencePar1[];
	static long keywordReferencePar1[];
	static long liReferencePar1[];
	static long linkReferencePar1[];
	static long linklistReferencePar1[];
	static long linkpoolReferencePar1[];
	static long lqReferencePar1[];
	static long noteReferencePar1[];
	static long objectReferencePar1[];
	static long optionReferencePar1[];
	static long pReferencePar1[];
	static long phReferencePar1[];
	static long qReferencePar1[];
	static long repsepReferencePar1[];
	static long rowReferencePar1[];
	static long shortdescReferencePar1[];
	static long simpletableReferencePar1[];
	static long stentryReferencePar1[];
	static long tableReferencePar1[];
	static long termReferencePar1[];
	static long titleReferencePar1[];
	static long tmReferencePar1[];
	static long uicontrolReferencePar1[];
	static long varReferencePar1[];
	static long xrefReferencePar1[];
	static long titlefirstReferencePar1[];
	static vpair ReferenceElemSetsSet1[];
	static npair ReferenceElemSetNamesSet1[];
	static ipair ReferenceLevelsSet1[];
	static ipair ReferenceAtFirstSet1[];


	static npair GlossentryNamesSet1[];
	static ipair GlossentryInlineSet1[];
	static ipair GlossentrySingleSet1[];
	static long Part1GlossentryPar1[];
	static long Part2GlossentryPar1[];
	static long Part3GlossentryPar1[];
	static long Part4GlossentryPar1[];
	static long Part5GlossentryPar1[];
	static long Part6GlossentryPar1[];
	static long Part7GlossentryPar1[];
	static long Part8GlossentryPar1[];
	static long Part9GlossentryPar1[];
	static long Part10GlossentryPar1[];
	static long Part11GlossentryPar1[];
	static long Part12GlossentryPar1[];
	static long Part13GlossentryPar1[];
	static long Part14GlossentryPar1[];
	static long Part15GlossentryPar1[];
	static long citeGlossentryPar1[];
	static long codephGlossentryPar1[];
	static long dataGlossentryPar1[];
	static long descGlossentryPar1[];
	static long dlGlossentryPar1[];
	static long draftcommentGlossentryPar1[];
	static long figGlossentryPar1[];
	static long figgroupGlossentryPar1[];
	static long fnGlossentryPar1[];
	static long foreignGlossentryPar1[];
	static long fragmentGlossentryPar1[];
	static long groupseqGlossentryPar1[];
	static long imageGlossentryPar1[];
	static long indextermGlossentryPar1[];
	static long itemgroupGlossentryPar1[];
	static long keywordGlossentryPar1[];
	static long liGlossentryPar1[];
	static long linkGlossentryPar1[];
	static long linklistGlossentryPar1[];
	static long linkpoolGlossentryPar1[];
	static long lqGlossentryPar1[];
	static long noteGlossentryPar1[];
	static long objectGlossentryPar1[];
	static long optionGlossentryPar1[];
	static long pGlossentryPar1[];
	static long phGlossentryPar1[];
	static long qGlossentryPar1[];
	static long repsepGlossentryPar1[];
	static long rowGlossentryPar1[];
	static long simpletableGlossentryPar1[];
	static long stentryGlossentryPar1[];
	static long tableGlossentryPar1[];
	static long termGlossentryPar1[];
	static long titleGlossentryPar1[];
	static long tmGlossentryPar1[];
	static long uicontrolGlossentryPar1[];
	static long varGlossentryPar1[];
	static long xrefGlossentryPar1[];
	static long titlefirstGlossentryPar1[];
	static vpair GlossentryElemSetsSet1[];
	static npair GlossentryElemSetNamesSet1[];
	static ipair GlossentryLevelsSet1[];
	static ipair GlossentryAtFirstSet1[];


	static npair MapNamesSet1[];
	static ipair MapInlineSet1[];
	static ipair MapSingleSet1[];
	static long Part1MapPar1[];
	static long Part2MapPar1[];
	static long Part3MapPar1[];
	static long Part4MapPar1[];
	static long Part5MapPar1[];
	static long Part6MapPar1[];
	static long Part7MapPar1[];
	static long Part8MapPar1[];
	static long Part9MapPar1[];
	static long Part10MapPar1[];
	static long Part11MapPar1[];
	static long Part12MapPar1[];
	static long citeMapPar1[];
	static long dataMapPar1[];
	static long descMapPar1[];
	static long dlMapPar1[];
	static long draftcommentMapPar1[];
	static long figMapPar1[];
	static long figgroupMapPar1[];
	static long fnMapPar1[];
	static long foreignMapPar1[];
	static long imageMapPar1[];
	static long indextermMapPar1[];
	static long itemgroupMapPar1[];
	static long keywordMapPar1[];
	static long liMapPar1[];
	static long lqMapPar1[];
	static long navrefMapPar1[];
	static long noteMapPar1[];
	static long objectMapPar1[];
	static long pMapPar1[];
	static long phMapPar1[];
	static long qMapPar1[];
	static long rowMapPar1[];
	static long simpletableMapPar1[];
	static long stentryMapPar1[];
	static long tableMapPar1[];
	static long termMapPar1[];
	static long titleMapPar1[];
	static long tmMapPar1[];
	static long topicmetaMapPar1[];
	static long topicrefMapPar1[];
	static long xrefMapPar1[];
	static long titlefirstMapPar1[];
	static long topicmetafirstMapPar1[];
	static vpair MapElemSetsSet1[];
	static npair MapElemSetNamesSet1[];
	static ipair MapLevelsSet1[];
	static ipair MapAtFirstSet1[];


	static npair BookmapNamesSet1[];
	static ipair BookmapInlineSet1[];
	static ipair BookmapSingleSet1[];
	static long Part1BookmapPar1[];
	static long Part2BookmapPar1[];
	static long Part3BookmapPar1[];
	static long Part4BookmapPar1[];
	static long Part5BookmapPar1[];
	static long Part6BookmapPar1[];
	static long Part7BookmapPar1[];
	static long Part8BookmapPar1[];
	static long Part9BookmapPar1[];
	static long Part10BookmapPar1[];
	static long Part11BookmapPar1[];
	static long Part12BookmapPar1[];
	static long Part13BookmapPar1[];
	static long Part14BookmapPar1[];
	static long Part15BookmapPar1[];
	static long Part16BookmapPar1[];
	static long booklistsBookmapPar1[];
	static long chapterBookmapPar1[];
	static long citeBookmapPar1[];
	static long dataBookmapPar1[];
	static long dataaboutBookmapPar1[];
	static long descBookmapPar1[];
	static long dlBookmapPar1[];
	static long draftcommentBookmapPar1[];
	static long figBookmapPar1[];
	static long figgroupBookmapPar1[];
	static long fnBookmapPar1[];
	static long foreignBookmapPar1[];
	static long imageBookmapPar1[];
	static long indextermBookmapPar1[];
	static long itemgroupBookmapPar1[];
	static long keywordBookmapPar1[];
	static long liBookmapPar1[];
	static long linktextBookmapPar1[];
	static long lqBookmapPar1[];
	static long monthBookmapPar1[];
	static long namedetailsBookmapPar1[];
	static long navrefBookmapPar1[];
	static long noteBookmapPar1[];
	static long objectBookmapPar1[];
	static long otherinfoBookmapPar1[];
	static long pBookmapPar1[];
	static long personBookmapPar1[];
	static long phBookmapPar1[];
	static long qBookmapPar1[];
	static long revisionidBookmapPar1[];
	static long rowBookmapPar1[];
	static long simpletableBookmapPar1[];
	static long stentryBookmapPar1[];
	static long summaryBookmapPar1[];
	static long tableBookmapPar1[];
	static long termBookmapPar1[];
	static long titleBookmapPar1[];
	static long tmBookmapPar1[];
	static long topicmetaBookmapPar1[];
	static long topicrefBookmapPar1[];
	static long xrefBookmapPar1[];
	static long yearBookmapPar1[];
	static long titlefirstBookmapPar1[];
	static long topicmetafirstBookmapPar1[];
	static vpair BookmapElemSetsSet1[];
	static npair BookmapElemSetNamesSet1[];
	static ipair BookmapLevelsSet1[];
	static ipair BookmapAtFirstSet1[];
};


class DBtag {
	friend class HTMwr;
	friend class HXMwr;

	// in dwhtmdb.cpp
	static char *PrologDTypeDocBook;
	static char *PrologDTDDocBook;

	static npair DocBookBNamesSet[];
	static ipair DocBookBInlineSet[];
	static ipair DocBookBSingleSet[];
	static long Part1DocBookBPar[];
	static long Part2DocBookBPar[];
	static long Part3DocBookBPar[];
	static long Part4DocBookBPar[];
	static long Part5DocBookBPar[];
	static long Part6DocBookBPar[];
	static long Part7DocBookBPar[];
	static long Part8DocBookBPar[];
	static long Part9DocBookBPar[];
	static long Part10DocBookBPar[];
	static long Part11DocBookBPar[];
	static long Part12DocBookBPar[];
	static long Part13DocBookBPar[];
	static long Part14DocBookBPar[];
	static long Part15DocBookBPar[];
	static long Part16DocBookBPar[];
	static long Part17DocBookBPar[];
	static long Part18DocBookBPar[];
	static long Part19DocBookBPar[];
	static long Part20DocBookBPar[];
	static long Part21DocBookBPar[];
	static long Part22DocBookBPar[];
	static long Part23DocBookBPar[];
	static long Part24DocBookBPar[];
	static long Part25DocBookBPar[];
	static long Part26DocBookBPar[];
	static long Part27DocBookBPar[];
	static long Part28DocBookBPar[];
	static long Part29DocBookBPar[];
	static long Part30DocBookBPar[];
	static long Part31DocBookBPar[];
	static long Part32DocBookBPar[];
	static long Part33DocBookBPar[];
	static long Part34DocBookBPar[];
	static long Part35DocBookBPar[];
	static long Part36DocBookBPar[];
	static long Part37DocBookBPar[];
	static long Part38DocBookBPar[];
	static long Part39DocBookBPar[];
	static long Part40DocBookBPar[];
	static long Part41DocBookBPar[];
	static long Part42DocBookBPar[];
	static long Part43DocBookBPar[];
	static long Part44DocBookBPar[];
	static long Part45DocBookBPar[];
	static long Part46DocBookBPar[];
	static long Part47DocBookBPar[];
	static long Part48DocBookBPar[];
	static long Part49DocBookBPar[];
	static long Part50DocBookBPar[];
	static long abbrevDocBookBPar[];
	static long abstractDocBookBPar[];
	static long accelDocBookBPar[];
	static long acronymDocBookBPar[];
	static long actionDocBookBPar[];
	static long addressDocBookBPar[];
	static long affiliationDocBookBPar[];
	static long altDocBookBPar[];
	static long anchorDocBookBPar[];
	static long appendixDocBookBPar[];
	static long applicationDocBookBPar[];
	static long areaDocBookBPar[];
	static long areaspecDocBookBPar[];
	static long argDocBookBPar[];
	static long articleinfoDocBookBPar[];
	static long artpagenumsDocBookBPar[];
	static long attributionDocBookBPar[];
	static long authorDocBookBPar[];
	static long authorblurbDocBookBPar[];
	static long authorinitialsDocBookBPar[];
	static long beginpageDocBookBPar[];
	static long biblioentryDocBookBPar[];
	static long bibliographyDocBookBPar[];
	static long bibliomsetDocBookBPar[];
	static long blockinfoDocBookBPar[];
	static long blockquoteDocBookBPar[];
	static long bridgeheadDocBookBPar[];
	static long calloutlistDocBookBPar[];
	static long captionDocBookBPar[];
	static long cautionDocBookBPar[];
	static long citetitleDocBookBPar[];
	static long classnameDocBookBPar[];
	static long coDocBookBPar[];
	static long colDocBookBPar[];
	static long colophonDocBookBPar[];
	static long colspecDocBookBPar[];
	static long commandDocBookBPar[];
	static long contribDocBookBPar[];
	static long corefDocBookBPar[];
	static long corpauthorDocBookBPar[];
	static long dateDocBookBPar[];
	static long editorDocBookBPar[];
	static long emailDocBookBPar[];
	static long emphasisDocBookBPar[];
	static long epigraphDocBookBPar[];
	static long equationDocBookBPar[];
	static long exampleDocBookBPar[];
	static long exceptionnameDocBookBPar[];
	static long fieldsynopsisDocBookBPar[];
	static long footnoterefDocBookBPar[];
	static long formalparaDocBookBPar[];
	static long funcparamsDocBookBPar[];
	static long functionDocBookBPar[];
	static long glossentryDocBookBPar[];
	static long glosslistDocBookBPar[];
	static long glosstermDocBookBPar[];
	static long graphicDocBookBPar[];
	static long graphiccoDocBookBPar[];
	static long guibuttonDocBookBPar[];
	static long highlightsDocBookBPar[];
	static long honorificDocBookBPar[];
	static long imageobjectDocBookBPar[];
	static long imageobjectcoDocBookBPar[];
	static long indexdivDocBookBPar[];
	static long indexentryDocBookBPar[];
	static long indextermDocBookBPar[];
	static long informalequationDocBookBPar[];
	static long informalexampleDocBookBPar[];
	static long informaltableDocBookBPar[];
	static long initializerDocBookBPar[];
	static long inlineequationDocBookBPar[];
	static long inlinegraphicDocBookBPar[];
	static long inlinemediaobjectDocBookBPar[];
	static long interfacenameDocBookBPar[];
	static long itemizedlistDocBookBPar[];
	static long keycapDocBookBPar[];
	static long labelDocBookBPar[];
	static long legalnoticeDocBookBPar[];
	static long lineannotationDocBookBPar[];
	static long linkDocBookBPar[];
	static long listitemDocBookBPar[];
	static long literallayoutDocBookBPar[];
	static long mediaobjectDocBookBPar[];
	static long methodnameDocBookBPar[];
	static long methodparamDocBookBPar[];
	static long modespecDocBookBPar[];
	static long modifierDocBookBPar[];
	static long msgexplanDocBookBPar[];
	static long msgsetDocBookBPar[];
	static long msgtextDocBookBPar[];
	static long objectinfoDocBookBPar[];
	static long ooclassDocBookBPar[];
	static long optionDocBookBPar[];
	static long optionalDocBookBPar[];
	static long orgnameDocBookBPar[];
	static long packageDocBookBPar[];
	static long paraDocBookBPar[];
	static long parameterDocBookBPar[];
	static long partintroDocBookBPar[];
	static long personblurbDocBookBPar[];
	static long personnameDocBookBPar[];
	static long phraseDocBookBPar[];
	static long prefaceDocBookBPar[];
	static long procedureDocBookBPar[];
	static long programlistingDocBookBPar[];
	static long programlistingcoDocBookBPar[];
	static long publishernameDocBookBPar[];
	static long qandadivDocBookBPar[];
	static long qandaentryDocBookBPar[];
	static long refentryDocBookBPar[];
	static long refentrytitleDocBookBPar[];
	static long refsect2DocBookBPar[];
	static long refsectionDocBookBPar[];
	static long remarkDocBookBPar[];
	static long replaceableDocBookBPar[];
	static long revhistoryDocBookBPar[];
	static long rowDocBookBPar[];
	static long sbrDocBookBPar[];
	static long screenDocBookBPar[];
	static long sect1DocBookBPar[];
	static long sectionDocBookBPar[];
	static long sidebarDocBookBPar[];
	static long simplesectDocBookBPar[];
	static long spanspecDocBookBPar[];
	static long stepDocBookBPar[];
	static long subscriptDocBookBPar[];
	static long subtitleDocBookBPar[];
	static long symbolDocBookBPar[];
	static long synopfragmentrefDocBookBPar[];
	static long synopsisDocBookBPar[];
	static long tableDocBookBPar[];
	static long textobjectDocBookBPar[];
	static long tfootDocBookBPar[];
	static long tgroupDocBookBPar[];
	static long theadDocBookBPar[];
	static long titleDocBookBPar[];
	static long titleabbrevDocBookBPar[];
	static long tocDocBookBPar[];
	static long tocchapDocBookBPar[];
	static long tocentryDocBookBPar[];
	static long trDocBookBPar[];
	static long trademarkDocBookBPar[];
	static long typeDocBookBPar[];
	static long varnameDocBookBPar[];
	static long videoobjectDocBookBPar[];
	static long voidDocBookBPar[];
	static long beginpagefirstDocBookBPar[];
	static long blockinfofirstDocBookBPar[];
	static long titlefirstDocBookBPar[];
	static vpair DocBookBElemSetsSet[];
	static npair DocBookBElemSetNamesSet[];
	static ipair DocBookBLevelsSet[];
	static ipair DocBookBAtFirstSet[];


	static npair DocBookANamesSet[];
	static ipair DocBookAInlineSet[];
	static ipair DocBookASingleSet[];
	static long Part1DocBookAPar[];
	static long Part2DocBookAPar[];
	static long Part3DocBookAPar[];
	static long Part4DocBookAPar[];
	static long Part5DocBookAPar[];
	static long Part6DocBookAPar[];
	static long Part7DocBookAPar[];
	static long Part8DocBookAPar[];
	static long Part9DocBookAPar[];
	static long Part10DocBookAPar[];
	static long Part11DocBookAPar[];
	static long Part12DocBookAPar[];
	static long Part13DocBookAPar[];
	static long Part14DocBookAPar[];
	static long Part15DocBookAPar[];
	static long Part16DocBookAPar[];
	static long Part17DocBookAPar[];
	static long Part18DocBookAPar[];
	static long Part19DocBookAPar[];
	static long Part20DocBookAPar[];
	static long Part21DocBookAPar[];
	static long Part22DocBookAPar[];
	static long Part23DocBookAPar[];
	static long Part24DocBookAPar[];
	static long Part25DocBookAPar[];
	static long Part26DocBookAPar[];
	static long Part27DocBookAPar[];
	static long Part28DocBookAPar[];
	static long Part29DocBookAPar[];
	static long Part30DocBookAPar[];
	static long Part31DocBookAPar[];
	static long Part32DocBookAPar[];
	static long Part33DocBookAPar[];
	static long Part34DocBookAPar[];
	static long Part35DocBookAPar[];
	static long Part36DocBookAPar[];
	static long Part37DocBookAPar[];
	static long Part38DocBookAPar[];
	static long Part39DocBookAPar[];
	static long Part40DocBookAPar[];
	static long Part41DocBookAPar[];
	static long Part42DocBookAPar[];
	static long Part43DocBookAPar[];
	static long Part44DocBookAPar[];
	static long Part45DocBookAPar[];
	static long Part46DocBookAPar[];
	static long Part47DocBookAPar[];
	static long Part48DocBookAPar[];
	static long Part49DocBookAPar[];
	static long Part50DocBookAPar[];
	static long Part51DocBookAPar[];
	static long abbrevDocBookAPar[];
	static long abstractDocBookAPar[];
	static long accelDocBookAPar[];
	static long acronymDocBookAPar[];
	static long actionDocBookAPar[];
	static long addressDocBookAPar[];
	static long affiliationDocBookAPar[];
	static long altDocBookAPar[];
	static long anchorDocBookAPar[];
	static long applicationDocBookAPar[];
	static long areaDocBookAPar[];
	static long areaspecDocBookAPar[];
	static long argDocBookAPar[];
	static long articleinfoDocBookAPar[];
	static long artpagenumsDocBookAPar[];
	static long attributionDocBookAPar[];
	static long authorDocBookAPar[];
	static long authorblurbDocBookAPar[];
	static long authorinitialsDocBookAPar[];
	static long beginpageDocBookAPar[];
	static long biblioentryDocBookAPar[];
	static long bibliographyDocBookAPar[];
	static long bibliomsetDocBookAPar[];
	static long blockinfoDocBookAPar[];
	static long blockquoteDocBookAPar[];
	static long bridgeheadDocBookAPar[];
	static long calloutlistDocBookAPar[];
	static long captionDocBookAPar[];
	static long cautionDocBookAPar[];
	static long citetitleDocBookAPar[];
	static long classnameDocBookAPar[];
	static long coDocBookAPar[];
	static long colDocBookAPar[];
	static long colspecDocBookAPar[];
	static long commandDocBookAPar[];
	static long contribDocBookAPar[];
	static long corefDocBookAPar[];
	static long corpauthorDocBookAPar[];
	static long dateDocBookAPar[];
	static long editorDocBookAPar[];
	static long emailDocBookAPar[];
	static long emphasisDocBookAPar[];
	static long epigraphDocBookAPar[];
	static long equationDocBookAPar[];
	static long exampleDocBookAPar[];
	static long exceptionnameDocBookAPar[];
	static long fieldsynopsisDocBookAPar[];
	static long footnoterefDocBookAPar[];
	static long formalparaDocBookAPar[];
	static long funcparamsDocBookAPar[];
	static long functionDocBookAPar[];
	static long glossentryDocBookAPar[];
	static long glosslistDocBookAPar[];
	static long glosstermDocBookAPar[];
	static long graphicDocBookAPar[];
	static long graphiccoDocBookAPar[];
	static long guibuttonDocBookAPar[];
	static long highlightsDocBookAPar[];
	static long honorificDocBookAPar[];
	static long imageobjectDocBookAPar[];
	static long imageobjectcoDocBookAPar[];
	static long indexentryDocBookAPar[];
	static long indextermDocBookAPar[];
	static long informalequationDocBookAPar[];
	static long informalexampleDocBookAPar[];
	static long informaltableDocBookAPar[];
	static long initializerDocBookAPar[];
	static long inlineequationDocBookAPar[];
	static long inlinegraphicDocBookAPar[];
	static long inlinemediaobjectDocBookAPar[];
	static long interfacenameDocBookAPar[];
	static long itemizedlistDocBookAPar[];
	static long keycapDocBookAPar[];
	static long labelDocBookAPar[];
	static long legalnoticeDocBookAPar[];
	static long lineannotationDocBookAPar[];
	static long linkDocBookAPar[];
	static long listitemDocBookAPar[];
	static long literallayoutDocBookAPar[];
	static long lotDocBookAPar[];
	static long mediaobjectDocBookAPar[];
	static long methodnameDocBookAPar[];
	static long methodparamDocBookAPar[];
	static long modespecDocBookAPar[];
	static long modifierDocBookAPar[];
	static long msgexplanDocBookAPar[];
	static long msgsetDocBookAPar[];
	static long msgtextDocBookAPar[];
	static long objectinfoDocBookAPar[];
	static long ooclassDocBookAPar[];
	static long optionDocBookAPar[];
	static long optionalDocBookAPar[];
	static long orgnameDocBookAPar[];
	static long packageDocBookAPar[];
	static long paraDocBookAPar[];
	static long parameterDocBookAPar[];
	static long personblurbDocBookAPar[];
	static long personnameDocBookAPar[];
	static long phraseDocBookAPar[];
	static long procedureDocBookAPar[];
	static long programlistingDocBookAPar[];
	static long programlistingcoDocBookAPar[];
	static long publishernameDocBookAPar[];
	static long qandadivDocBookAPar[];
	static long qandaentryDocBookAPar[];
	static long refentrytitleDocBookAPar[];
	static long refsect2DocBookAPar[];
	static long refsectionDocBookAPar[];
	static long remarkDocBookAPar[];
	static long replaceableDocBookAPar[];
	static long revhistoryDocBookAPar[];
	static long rowDocBookAPar[];
	static long sbrDocBookAPar[];
	static long screenDocBookAPar[];
	static long sect1DocBookAPar[];
	static long sectionDocBookAPar[];
	static long sidebarDocBookAPar[];
	static long spanspecDocBookAPar[];
	static long stepDocBookAPar[];
	static long subscriptDocBookAPar[];
	static long subtitleDocBookAPar[];
	static long symbolDocBookAPar[];
	static long synopfragmentrefDocBookAPar[];
	static long synopsisDocBookAPar[];
	static long tableDocBookAPar[];
	static long textobjectDocBookAPar[];
	static long tfootDocBookAPar[];
	static long tgroupDocBookAPar[];
	static long theadDocBookAPar[];
	static long titleDocBookAPar[];
	static long titleabbrevDocBookAPar[];
	static long tocchapDocBookAPar[];
	static long tocentryDocBookAPar[];
	static long trDocBookAPar[];
	static long trademarkDocBookAPar[];
	static long typeDocBookAPar[];
	static long varnameDocBookAPar[];
	static long videoobjectDocBookAPar[];
	static long voidDocBookAPar[];
	static long beginpagefirstDocBookAPar[];
	static long blockinfofirstDocBookAPar[];
	static long titlefirstDocBookAPar[];
	static vpair DocBookAElemSetsSet[];
	static npair DocBookAElemSetNamesSet[];
	static ipair DocBookALevelsSet[];
	static ipair DocBookAAtFirstSet[];

};


enum uDocConstants {
  ElemPathMax = 50
};

class HUDwr {  // for uDoc output

  friend class HTMctl;
  friend class HTMgroup;
	friend class HTMwr;
	friend class HXMwr;
	friend class MXLwr;

	// in dwhtmud.cpp

	static DCnlist CurrElemPathNames;
	static DCilist CurrElemPathTerms;
	static long CurrElemPathLen;

	static void CloseElem(HTMwfile *wf, char *tag);
	static void OpenElem(HTMwfile *wf, char *tag, char *attrs);


 public:
  static void WriteElemPath(HTMwfile *wf, char *mpath, char *mform);
  static void PutuDocIndexEntry(HTMwfile *wf, char *iptr, char *sort);

};



class HXMwr {

  friend class HTMctl;
  friend class HTMgroup;
	friend class DBtag;
	friend class DTtag;
	friend class HTMwr;
	friend class HUDwr;

	// in dwhtmhr.cpp

	// reference info
	static DCnlist *TagNames;	// all valid tags; other lists use this number (string pool)
	// these lists are indexed by tag number (root == 1):
	static DCilist *TagLevels;   // required level, like 1; if 0 (missing), valid at any level 
	static DCilist *TagSingle;   // single parent; if -1, check TagParents, if 0, valid in any
	static DCvlist *TagParents;  // DClist* of parent nums for tags that can have two or more
	static DCilist *TagInline;   // 1 if inline (use no returns), 0 (missing) if block (indent)
	static DCilist *AtFirstList;  // list of child/parent pairs where child must be first

	// the lists above use ptrs because we need to swap in the lists for the
	// specific DTD in use; for DITA: Topic, Concept, Task, Reference, Map.
	// for specializations, we generate those at init time from .ini sections

	static tagfunc PutStartFunc;
	static tagfunc PutStartFullFunc;
	static tagfunc PutEndFunc;
	static tagfunc PutEndFullFunc;

	// current state info
  static long LastBlockLev;
	static long CurrTag;
	static bool CurrInline;
	static bool CurrPreform;
	static bool InCurrPreform;
	static long CurrTags[TagLevMax];
	static char *CurrTagNames[TagLevMax];
	static bool LevUsed[TagLevMax];
	static long TempTags;
	static long ErrorCount;
	static bool FirstOK;

	static bool CheckFirst(long tnum, long par, long lev,
	                    long *first = NULL, long fmax = 0);
	static bool FirstFixed(HTMwfile *wf, long tnum, long *parnum,
										  long *first);

	static bool IsLegal(long tnum, long lev = 0, long *parnum = NULL,
		                  long *first = NULL, long fmax = 0);
	static long LookUp(long tnum, long lev = 0, long *parnum = NULL,
		                 long *first = NULL);
	static DClist *LookDown(HTMwfile *wf, long tnum, long lev = 0,
		                      long *parnum = NULL, long *first = NULL);

	static void StartLevels(HTMwfile *wf, DClist *tlist);
	static void EndLevels(HTMwfile *wf, long lev);

	static void WriteStart(HTMwfile *wf, long tnum, bool complete = false);
	static void PutStart(HTMwfile *wf, char *tag, long lev, bool inlin);
	static void PutStartFull(HTMwfile *wf, char *tag, long lev, bool inlin);
	static void WriteEnd(HTMwfile *wf, long tnum, bool complete = false);
	static void PutEnd(HTMwfile *wf, char *tag, long lev, bool inlin);

	static void SetWarning(HTMwfile *wf, long warn);
	static long *GetNameNums(char **sar, bool rev = false);
	static long GetNameNum(char *chp);

	static DCnlist *UserElemSetNames;
	static DCvlist *UserElemSetNums;
	static long UserElemSetID;
	static long GetElemSetNum(char *name);
	static char **CombineSars(char **sar1, char **sar2);

  static long CurrLevel;    // set to 1 after root tag start

	static void InitTags(tagset *tset,
		tagfunc sf, tagfunc sff, tagfunc ef, tagfunc eff); // called for each new output file
  static bool WriteStartTag(HTMwfile *wf, char *tag, char **par = NULL,
	 char **pfirst = NULL, long lev = 0, bool inl = false);  // for each start tag
  static bool WriteEndTag(HTMwfile *wf, char *tag);    // only needed to end inline tags
  static bool WriteFinishTag(HTMwfile *wf, char *tag); // only needed to finish inline elems


	// in dwhtmdt.cpp
	static bool DocBook;
	static bool DocBookBook;
	static bool uDoc;
	static bool HXML;
	static bool HXref;
	static char *IDattr;

	static bool DITA;
	static bool DITAStarting;
	static bool CustomTagset;
	static bool DITATopicSet;
	static bool DITAFileStart;
	static bool DITAComposite;
	static bool KeepXrefText;
	static bool FM8Import;
	static bool UseDTDPath;
	static unc DITAVer;
	static char *LastXMLRoot;
	static long LastXMLLevel;
	static long XMLStartLevel;
	static DCnlist XMLCloseTags;
	static char *DITATopic;
	static char *DITAStartElem;
	static char *CurrDITATopic;
	static char *DITATopicID;
	static char *DITATopicRootAttr;
	static long DITATopicCount;
	static char DITATopicIDSpaceChar;
	static bool DITATopicIDUnderscore;
	static bool DITATopicIDLowerCase;

	static char *DITANavTitle;
	static char *DITASearchTitle;
	static char *DITAShortDesc;
	static char *DITALinkText;
	static bool DITANoToc;
	static bool DITANavTitlePar;
	static bool DITASearchTitlePar;
	static bool DITAShortDescPar;

	static char *DefDITATopic;
	static bool UseFormatAsTag;
	static bool DropInvalidParaTag;
	static char *DefDITAPar;
	static char *DefDITAChar;
	static char *DefDITAImg;
	static char *DefDITATbl;
	static bool UseTableFooterClass;
	static char *DITATableFooterClass;
	static char **DITANoCondAttrs;

	static char **DITATableParents;

	static bool DITATopicEntities;
	static bool DITACompEntities;

	static char *DITAMenuSep;

	static DCilist DITATopicNums;
	static DCilist DITAParaTopicNums;
	static long DITATopicNum;
	static long DITATopicTypeNum;
	static long LastTopicTypeNum;
	static char *DITATopicStartFormat;

	static bool DITATitlePar;
	static DCnlist DITATitles;

	static long DITAEndDiv;
	static long PreDITADiv;
	static long PreDITARuninDiv;
	static long DITAStartDiv;
	static long DITAErrorDiv;
	static DCilist DITAEndDivs;

	static char *DITATag;
	static char *DITATagID;
	static char *DITAAlias;
	static long DITALevel;
	static char *DITAParent;
	static char *DITAOpenBefore;
	static char *DITAOpenAfter;
	static char **DITAOpenBeforeArr;
	static char **DITAOpenAfterArr;
	static char *DITACloseBefore;
	static char *DITACloseAfter;
	static char **DITACloseBeforeArr;
	static char **DITACloseAfterArr;
	static char *DITAFirst;

	static bool ForceStartTopic;
	static bool SplitTopicFiles;
	static bool NestTopicFiles;
	static bool PartialNesting;
	static bool MixedTopicNesting;
	static bool WrapTopicFiles;
	static bool DITANoBlock;

	static void InitDITATopicFile(HTMwfile *wf, long sid);
	static void FinishInitDITATopicFile(HTMwfile *wf, long sid);
	static void StartDITATopic(HTMwfile *wf);

	static DClist DITANoTocList;
	static DCnlist DITATopicNameList;
	static DCnlist DITATopicIDList;
	static DCnlist DITATopicRootAttrList;
	static DCnlist DITANavTitleList;
	static DCnlist DITASearchTitleList;
	static DCnlist DITAShortDescList;
	static DCnlist DITALinkTextList;
	static DCilist DITATopicTypeLevels;
	static DCnlist DITATypeNamePrefixList;
	static bool DITATypeNamePrefixUsed;
	static void ProcDITATopicStart(pprop *cform);
	static void ProcDITAParaVars(char *chp);
	static void ProcDITATopicVars(void);
	static void SetTopicTypePrefixes(HTMwfile *wf);
	static void SetTopicTypePrefix(DCwfile *w, char **np, long i);
	static void CheckNesting(void);

	static char *DITAMarkerProps[DITAMarkerPropsMax];
	static long ProcDITAMarker(char *kp, char *txt);

	static long IndexDiv;
	static long IndexDivLen;
	static long IndexTagNum;
	static long PhTagNum;
	static void PutDITAIndexEntry(HTMwfile *wf, char *iptr, char *sort);

	static char *XrefWrapClass;
	static char *IndexWrapClass;
	static char *FootnoteWrapClass;
	static void WriteDITAIndexEntries(HTMwfile *wf);

	static DCnlist DITAParaTagList;
	static DCnlist DITAParaTagIDList;
	static DClist DITAParaTagIDSet;
	static DClist DITAParaTagIDUsed;
	static DCnlist DITAParaAliasList;
	static DCilist DITAParaLevelList;
	static DCvlist DITAParaParentList;
	static DCvlist DITAParaFirstList;
	static DCvlist DITAParaOpenBeforeList;
	static DCvlist DITAParaOpenAfterList;
	static DCvlist DITAParaCloseBeforeList;
	static DCvlist DITAParaCloseAfterList;

	static long DITAParentIDIx;
	static DCvlist DITAParentIDList;
	static DCnlist *DITAParentIDLp;
	static DCvlist DITAParentTagList;
	static DCnlist *DITAParentTagLp;
	static long DITAParaTagLevel;

	static long DITAAttrIx;
	static DCvlist DITAAttrList;
	static DCnlist *DITAAttrLp;
	static DCvlist DITAAttrTagList;
	static DCnlist *DITAAttrTagLp;
	static char *CurrFMStartTag;

	static void ProcDITAElemVars(long pid);
	static void DITASetElemType(char *tag, char **par);
	static void DITASetTypographics(bool para);
	static char *DITASetTypo(char **b, char *s, char *e);
	static void WriteDITAStartPTag(HTMwfile *wf, long pid, char *tag,
		         char **par = NULL, char **pfirst = NULL, long lev = 0,
						 char *ftag = NULL);

	static long HXMwr::PendingAlias;
	static DCnlist HXMwr::PendingAliasList;
	static void HXMwr::WritePendingAliases(HTMwfile *wf);

	static bool FindPar(char **par, char *tag);
	static void WriteDITAEndPTag(HTMwfile *wf, char *endtag);
	static void WriteDITAOpenBlocks(HTMwfile *wf, char **sar);
	static void WriteDITACloseBlocks(HTMwfile *wf, char **sar);
	static void WriteDITAStartCTag(HTMwfile *wf, char *tag, char *ftag = NULL,
	           bool runin = false, char **par = NULL, char **pfirst = NULL,
	           long lev = 0);
	static void WriteDITAEndCTag(HTMwfile *wf, char *tag, bool runin = false);
	static void WriteDITAFinishTag(HTMwfile *wf, char *tag, bool endit = true);

	static bool CloseFigAfterImage;
	static char *FigSar[2];
	static char *FigureSar[2];
	static void CloseFigureDiv(HTMwfile *wf, bool part = false);

	static void EndAllDITATags(HTMwfile *wf);
	static void WriteDITAElem(HTMwfile *wf, char *tag, char *txt);

	static tblprop *CurrDITATbl;
	static bool InDITATblHeader;
	static void SetDITATable(char *tname);
	static void WriteDITAStartTbl(HTMwfile *wf, char *tag, char **par = NULL);
	static void WriteDITAEndTbl(HTMwfile *wf, long lev);

	static void SetDITAStart(HTMwfile *wf);
	static void SetDITAEnd(HTMwfile *wf);
	static void SetDITADivOff(HTMwfile *wf);

	static void PutDITAStart(HTMwfile *wf, char *tag, long lev, bool inlin);
	static void PutDITAStartFull(HTMwfile *wf, char *tag, long lev, bool inlin);
	static bool IsEndable(long lev);
	static void PutDITAEnd(HTMwfile *wf, char *tag, long lev, bool inlin);
	static void PutDITAEndFull(HTMwfile *wf, char *tag, long lev, bool inlin);

	static long DITALinkDiv;
	static bool IsFrXref;
	static bool SetElementIDs;
	static bool InRelTopics;
	static bool UseLocalScope;
	static long RelTopicTag;
	static long DITARelLinkDiv;
	static long PreRelLinkDiv;
	static DCilist DITATopicTypeList;
	static DCnlist DITATopicLinkList;
	static DCnlist DITAPendElemLinks;
	static DCilist DITAElemTypeList;
	static char *CurrDITATblID;
	static DCnlist DITATableIDList;
	static long DITAElemType;
	static long DITATopicType;
	static char *DITALinkScope;
	static char *DITALinkFormat;
	static char *DITALinkType;
	static char *DITALinkElemID;
	static uns IsDITARefType(char *tag);
	static void DITASetIntRef(char *txt);
	static void DITASetTopicInfo(void);
	static bool DITAProcIntRef(char *txt, bool isnew);
	static char *DITASetIntLink(char *txt, char *fname);
	static void SetParentElemID(char *tag, char *id, long pid);
	static char *DITAProcExtRef(char *txt = NULL, char *rfn = NULL);
	static void WriteDITALinkAttrs(HTMwfile *wf, char *chp, unc typ = 0);
	static void StartDITALinkDiv(HTMwfile *wf);
	static void EndDITALinkDiv(HTMwfile *wf);
	static void StartDITARelLinkDiv(HTMwfile *wf);
	static void EndDITARelLinkDiv(HTMwfile *wf);


	// in dwhtmdm.cpp
	static bool UseDITAMapID;
	static char *DITAMapID;
	static char *DITAMapTitle;
	static char *DITAMapTitleAlt;
	static char *DITAMapHead;
	static char *DITAMapHeadAlt;
	static char *DITAMapBase;
	static char *DITAMapFileName;
	static bool MapTopicmeta;
	static bool MapTopicID;
	static bool UseAltMapTitle;

	static DCvlist DITAMapVals;
	static DCnlist *DITARefAttrs;
	static DCnlist *DITARefAttrNames;
	static long DITARefCount;
	static DCvlist DITARelVals;
	static dmapprop *DITAMapVal;
	static bool WriteDitamaps;
	static bool MapBookTopics;
	static bool MapBookRelTable;
	static bool DITATitleStart;

	static unc DITABookmapType;
	static char *DITABookTitle;
	static char *DITABookSubtitle;
	static char *DITABookLibrary;
	static char *DITABookMeta;

	static void WriteDITAMap(HTMwfile *wf);
	static long DITAMapLevel;
	static DCilist DITAMapLevels;
	static DCilist DITAOrigMapLevels;
	static long DITAMapSeq;
	static DCilist DITAMapSeqs;
	static unc DITAMapUsage;
	static DCilist DITAMapUsages;
	static char *CurrMapTags[MapLevMax];
	static bool FirstTopicRef;
	static long MapLevelOffset;
	static void WriteDITATopicrefs(DCfile *w, void *v, long i);

	static long DITARelNameCount;
	static long DITARelNum;
	static DCnlist DITARelNames;
	static DCilist DITARelTopics;
	static long DITATypeNameCount;
	static long DITATypeNum;
	static DCnlist DITATopicTypes;
	static DCilist DITATypeUsage;
	static DCnlist DITALinkingAttr;
	static bool DITAUseRelNameCol;

	static void WriteDITARelTable(HTMwfile *wf);
	static void WriteRelTableRef(DCwfile *w, long v, long i);

	static long DITAMapDiv;
	static long DITAFullMapDiv;
	static DCvlist DITARelRowList;
	static DCilist *DITARelCellList;

	static void WriteDITABookInfo(HTMwfile *wf);

	static bool AllowPartAppendix;
	static char *DITABookMapName;
	static char *DITABookMapID;
	static char *DITABookMapTitle;
	static char *DITABookMapHead;

	static DCnlist FileExtTypes;
	static npair FileExtTypesSet[];
	static DCnlist SegmentTypes;
	static npair SegmentTypesSet[];
	static DCnlist BookFileTypes;
	static npair BookFileTypesSet[];
	static DCnlist PropertyTypes;
	static npair PropertyTypesSet[];

	static void WriteDITABookMap(DCrfile *rf, char *destname, char *buf);
	static char *SetBookMapListTag(long fpltyp);
	static char *ExtractTopicref(char *pstr, char *tid);
	static char *ChangeTags(char *pstr, char *otag, char *ntag);
	static void PutBookmapAddedElement(HTMwfile *wf, long etype, char *key);
	static void PutBookmapTopicrefAttrs(HTMwfile *wf, char *key);

	static char *DocBookRoot;
	static bool WriteBookFile;
	static char *BookFileName;
	static char *BookFileTitle;

	static void WriteDocBookFile(DCrfile *rf, char *destname, char *buf);
	static void ProcDocBookEnt(char *buf, HTMwfile *wf, long erdiv);


	// in dwhtmds.cpp
	static DCnlist *DTopicTypes;
	static DCnlist *DTopicStarts;
	static DCnlist *DTopicRoots;
	static DCnlist *CommonNames;
	static DCvlist *DTagSets;
	static bool UseCommonNames;

	static tagset *DITAtags;
	static tagset *DTopicTags;
	static tagset *DConceptTags;
	static tagset *DTaskTags;
	static tagset *DReferenceTags;
	static tagset *DGlossentryTags;
	static tagset *DMapTags;
	static tagset *DBookmapTags;

	static tagset *DocBookTags;

	static DCnlist *DocBookBNames;
	static DCilist *DocBookBSingle;
	static DCnlist *DocBookBElemSetNames;
	static DCvlist *DocBookBElemSets;
	static DCilist *DocBookBInline;
	static DCvlist *DocBookBParents;
	static DCilist *DocBookBLevels;
	static DCilist *DocBookBAtFirst;

	static DCnlist *DocBookANames;
	static DCilist *DocBookASingle;
	static DCnlist *DocBookAElemSetNames;
	static DCvlist *DocBookAElemSets;
	static DCilist *DocBookAInline;
	static DCvlist *DocBookAParents;
	static DCilist *DocBookALevels;
	static DCilist *DocBookAAtFirst;


	static DCnlist *TopicNames;
	static DCilist *TopicSingle;
	static DCnlist *TopicElemSetNames;
	static DCvlist *TopicElemSets;
	static DCilist *TopicInline;
	static DCvlist *TopicParents;
	static DCilist *TopicLevels;
	static DCilist *TopicAtFirst;

	static DCnlist *ConceptNames;
	static DCilist *ConceptSingle;
	static DCnlist *ConceptElemSetNames;
	static DCvlist *ConceptElemSets;
	static DCilist *ConceptInline;
	static DCvlist *ConceptParents;
	static DCilist *ConceptLevels;
	static DCilist *ConceptAtFirst;

	static DCnlist *TaskNames;
	static DCilist *TaskSingle;
	static DCnlist *TaskElemSetNames;
	static DCvlist *TaskElemSets;
	static DCilist *TaskInline;
	static DCvlist *TaskParents;
	static DCilist *TaskLevels;
	static DCilist *TaskAtFirst;

	static DCnlist *ReferenceNames;
	static DCilist *ReferenceSingle;
	static DCnlist *ReferenceElemSetNames;
	static DCvlist *ReferenceElemSets;
	static DCilist *ReferenceInline;
	static DCvlist *ReferenceParents;
	static DCilist *ReferenceLevels;
	static DCilist *ReferenceAtFirst;

	static DCnlist *GlossentryNames;
	static DCilist *GlossentrySingle;
	static DCnlist *GlossentryElemSetNames;
	static DCvlist *GlossentryElemSets;
	static DCilist *GlossentryInline;
	static DCvlist *GlossentryParents;
	static DCilist *GlossentryLevels;
	static DCilist *GlossentryAtFirst;

	static DCnlist *MapNames;
	static DCilist *MapSingle;
	static DCnlist *MapElemSetNames;
	static DCvlist *MapElemSets;
	static DCilist *MapInline;
	static DCvlist *MapParents;
	static DCilist *MapLevels;
	static DCilist *MapAtFirst;

	static DCnlist *BookmapNames;
	static DCilist *BookmapSingle;
	static DCnlist *BookmapElemSetNames;
	static DCvlist *BookmapElemSets;
	static DCilist *BookmapInline;
	static DCvlist *BookmapParents;
	static DCilist *BookmapLevels;
	static DCilist *BookmapAtFirst;

	static void InitDocBook(void);
	static void SetDocBookType(long tagid);
	static void SetDocBookTypeB(long tagid);
	static void SetDocBookTypeA(long tagid);

	static void InitDITA(void);
	static void SetDITATopicType(long tagid);
	static void SetDITAConceptType(long tagid);
	static void SetDITATaskType(long tagid);
	static void SetDITAReferenceType(long tagid);
	static void SetDITAGlossentryType(long tagid);
	static void SetDITAMapType(long tagid);
	static void SetDITABookmapType(long tagid);
	static DCvlist *MergeElemLists(DCvlist *parents, DCvlist *esetlists);

	static char *SpecIniDir;
	static DCnlist *DSpecSects;
	static npair DSpecSectsSet[];
	static DCirfile *DITASpecIni(char *sname);
	static void DITALoadSpecial(char *sname);
	static void DITAModSpecial(DCirfile *sf, char *deriv,
		                         char *sname, char *repname);
	static void ReportDITALoadErrors(DCwfile *wf, char *sname, tagset *ts,
	  DClist *undef, DClist *unseq, DCvlist *rescan, DCnlist *dupnames);

	static DCnlist *DITATableNames;
	static DCvlist *DITATableLists;
	static npair DITATableTypeList[];
	static tblprop DITASimpleTable;
	static npair DITAPropTblHead[];
	static npair DITAPropTblBody[];
	static tblprop DITAPropertiesTable;
	static npair DITAChoiceTblHead[];
	static npair DITAChoiceTblBody[];
	static tblprop DITAChoiceTable;
	static tblprop DITAComplexTable;

	static void InitDITATableTypes(void);
	static void GetDITATableProps(DCirfile *sf, char *tname, tblprop *tst);
	static void DumpDITATables(DCwfile *wf, tagset *ts);
	static void DumpDITATableStruct(DCwfile *wf, char *secname, tblprop* tp);
	static void DumpDITATablesCpp(DCwfile *wf, tagset *ts, char *Base, long typ);
	static void DumpDITATableStructCpp(DCwfile *wf, char *secname,
		                                 tblprop *tp, long typ);

	static bool CppForm;

	static DCnlist *DSPnames;
	static DCnlist *DSPbasenames;
	static DCilist *DSPsparents;
	static DCilist *DSPbasespars;
	static DCvlist *DSPmparents;
	static DCvlist *DSPelemsets;
	static DCnlist *DSPsetnames;
	static DCilist *DSPinlins;
	static DCilist *DSPlevels;
	static DCilist *DSPatfirst;

	static void DumpTagSet(char *dumpfile, char *tname, char *deriv,
		DClist *undef, DClist *unseq, DCvlist *rescan, DCnlist *dupnames);

	static char *FixSetName(char *n);
	static void DumpNameList(DCwfile *w, char *n, long i);
	static void DumpSetList(DCfile *w, void *v, long i);
	static void DumpElemTypeList(DCwfile *w, long v, long i);
	static void DumpPropList(DCwfile *w, long v, long i);
	static void DumpFirstList(DCwfile *w, long v, long i);

 public:
	static DCirfile *IniFile;

};


class HTMwr {

  friend class HTMctl;
  friend class HTMgroup;
	friend class DBtag;
	friend class DTtag;
	friend class HXMwr;
	friend class MXLwr;
	friend class HUDwr;

  // in dwhtm.cc

	static char *FilterVer;
	static char *DWhtmVer;

  static char *CurrFileName;
  static char *CurrBaseName;
  static char *CurrFilePath;
  static char *CurrFileFull;
	static DCwfile *CssFile;
	static long CurrentDiv;
	static long ObjectDiv;
	static long ExtrObjectDiv;
  static unc ClassSpaceChar;
  static bool ClassIsTag;
  static bool SelectorIncludesTag;
  static bool LowerCaseCSS;
  static bool UseElements;
  static bool ValidOnly;
  static bool AllowNobr;

	static bool UseLog;
	static bool ShowLog;
	static bool ShowUndefinedFormats;
	static char *LogIniPrefix;
	static char *LogFileName;
	static char *EditorFileName;
	static DClog *Log;
	static unc LogLevels[logmax];
	static bool LogIniChains;
	static void LogEvent(logtype lt, unc sev, char *desc, char *d2 = NULL,
                     char *d3 = NULL, char *d4 = NULL, char *d5 = NULL);
	static void SetNamePath(const char *pathname, char **oname, char **opath);
	static void StartCSSFile(void);

	static bool CaselessMatch;
	static bool SpacelessMatch;
	static bool WildcardMatch;
	static unc WriteMadeWithGraphic;
	static unc Encoding;
	static char *Ansi;

	static htm HTMpile[];
  static size_t HTMsize;
  static DCvlist *HTMlist[HtmListMax];

  static HTMgroup BaseGroup;
  static HTMgroup *GroupStack[LevelMax];
  static uns DCLlevel;
  static bool SkipCurrGroup;
  static uns SkipLevel;

  static void SetUpHTM(void);
  static void FindHTM(HTMctl *cp);

  static uns HTMLVersion;
	static bool UseFrameSet;
	static unc GeneratorTag;

	static char *PrologDType2;
	static char *PrologDType3;
	static char *PrologDType4;
	static char *PrologDType4F;
	static char *PrologDType;
	static bool PrologDTypeSet;
	static char *PrologDTD4;
	static char *PrologDTD4F;
	static char *PrologDTD;
	static bool PrologDTDSet;

	static bool Xhtml;
	static bool Xml;
	static bool UseDOCTYPE;
	static char *PrologDTypeX;
	static char *PrologDTDX;
	static char *XhtmlDefNS;
	static char *XhtmlNS;
	static char *XhtmlLang;
	static char *XHLangAttr;
	static bool UseXMLDeclaration;
	static char *XMLVer;
	static char *XMLEncoding;
	static bool QuotedEncoding;
	static bool UseXMLRoot;
	static char *XMLRoot;
	static char *ContentType;
	static bool UseHeadAndBody;
	static bool BodyContentOnly;
	static bool UTF8;
	static bool NoEnc;
	static bool NumericCharRefs;

  static bool ProcTop(HTMctl *cp);

  static char *GetFinalFileName(char *src, bool alias);
  static char *MakeOutID(char *out, char *id = NULL);
  static char *GetSplitFileName(char *sid);

	static void CheckLogErrors(void);

	static void WriteProlog(HTMwfile *wf, long sid);
	static void FinishProlog(HTMwfile *wf);

	static char *AttrBody;
	static char *AttrTable;
	static char *AttrRow;
	static char *AttrCellHead;
	static char *AttrCellBody;
	static void WriteAttrib(HTMwfile *wf, char *aname);
  static bool InTextRun;
  static bool CharStyles;
  static bool UseLang;

  static pprop SavedStateStack[SavedStateMax];
  static pprop PFStateStack[SavedStateMax];
  static tinfo SavedTabStack[SavedStateMax];
  static uns SavedLevel;
  static void SaveState(void);
  static void RestoreState(void);

	static bool GetBaseName(char *name, char *&base, bool ext = false);

  static uns UnitMul[UnitsMax];
  static uns UnitDiv[UnitsMax];
  static uns UBaseMul[UnitsMax];
  static uns UBaseDiv[UnitsMax];
  static void SetUnits(HTMctl *cp);
  static short GetTwips(long val, unittype ut);
  static long GetOrig(long val, unittype ut);

	static short ConversionDPI;
	static short PixelsFromTwips(short twips);


	// in dwhtmin.cpp
	static secinfo HTMIniSects[];
	static void LocTocSet(char *);
	static void TrailSet(char *);

	static setinfo ISecAuto[];
	static setinfo ISecUdocOptions[];
	static setinfo ISecOptions[];
	static void CaselessMatchSet(char *);
	static void SpacelessMatchSet(char *);
	static void WildcardMatchSet(char *);
	static void ConfluenceSet(char *);
	static void XrefFileSuffixSet(char *);
	static void DefaultTargetSet(char *);
	static void ClassSpaceCharSet(char *);
	static void HyperSpaceCharSet(char *);
	static void XrefSpaceCharSet(char *);
	static void WriteMadeWithGraphicSet(char *);
	static void TabCharsPerInchSet(char *);
	static void HTMLVersionSet(char *);
	static void HTMLDocTypeSet(char *);
	static void HTMLDTDSet(char *);
	static void EncodingSet(char *);
	static void AnsiSet(char *);
	static void SplitNumStartSet(char *);

	static setinfo ISecAttr[];

	static setinfo ISecMapOptions[];

	static setinfo ISecCss[];
	static void StylesheetSet(char *);
	static void UseCSSSet(char *);
	static void CssBodyFontSet(char *);
	static void CssIndentBaseSet(char *);

	static setinfo ISecFDK[];
	static void HTMLSubdirSet(char *);
	static void GraphSubdirSet(char *);
	static void GraphicNameDigitsSet(char *);

	static setinfo ISecTable[];
	static void StripTableSet(char *);
	static void TableDPISet(char *);
	static void AccessMethodSet(char *);
	static void TScopeColGroupSet(char *);
	static void TScopeRowGroupSet(char *);

	static setinfo ISecGraphics[];
	static void GraphPathSet(char *);

	static setinfo ISecTrails[];
	static void TrailFirstLevelSet(char *);
	static void TrailLastLevelSet(char *);
	static void TrailIndentSet(char *);

	static setinfo ISecLocalTOC[];
	static void LocalTOCSubsSet(char *);

	static setinfo ISecFilterConditions[];

	static setinfo ISecDropDowns[];

	static setinfo ISecIndex[];
	static setinfo ISecIndexText[];
	static void IndexNavTypeSet(char *);

	static setinfo ISecInserts[];
	static setinfo ISecNavMacs[];
	static void UseNavButtonsSet(char *);

	static setinfo ISecBase[];
	static void BaseSizeSet(char *);

	static setinfo ISecTypo[];
	static setinfo ISecCondOptions[];

	static setinfo ISecDocBook[];
	static void DocBookRootSet(char *);

	static setinfo ISecDITA[];
	static void FM8ImportSet(char *);
	static setinfo ISecDITAbookmap[];

	static setinfo ISecOH[];
	static void OHTitleSet(char *);
	static void OHPxSet(char *);
	static void OHSxSet(char *);
	static void NavElemsSet(char *);

	static setinfo ISecJava[];

	static setinfo ISecJH[];
	static void PMPxSet(char *);
	static void SMPxSet(char *);

	static setinfo ISecOHJ[];

	static setinfo ISecEH[];

	static setinfo ISecHH[];
	static void UseCodePageSet(char *);
	static void WHPSet(char *);
	static void DefaultChmFileSet(char *);
	static void LinkTypeSet(char *);
	static void LinkFlagsSet(char *);
	static void LinkEmptyTopicSet(char *);
	static void LinkButtonWidthSet(char *);
	static void LinkButtonHeightSet(char *);
	static void LinkButtonTextSet(char *);
	static void LinkButtonGraphicSet(char *);
	static void LinkButtonIconSet(char *);
	static void LinkTextFontSet(char *);
	static void LinkTextSet(char *);

	static setinfo ISecHTMLStyles[];
	static setinfo ISecHTMLPStyles[];
	static setinfo ISecHTMLCStyles[];
	static void PlainSet(char *);
	static void ColorSet(char *);
	static void MacBeforeSet(char *);
	static void MacAfterSet(char *);
	static void MacReplaceSet(char *);
	static void MacStartSet(char *);
	static void MacEndSet(char *);
	static void MacAnumBeforeSet(char *);
	static void MacAnumAfterSet(char *);
	static void MacLinkSrcSet(char *);
	static void ContentsSet(char *);
	static void SplitSet(char *);
	static void TitleStyleSet(char *);
	static void WindowStyleSet(char *);
	static void RawSet(char *);
	static void ExtrSet(char *);
	static void ALinkSet(char *);
	static void ParaLinkClassSet(char *);
	static void RowAttributeSet(char *);
	static void CellAttributeSet(char *);
	static void CellScopeValSet(char *);
	static void CellAbbrSet(char *);
	static void CellAxisSet(char *);
	static void AttrSet(char *);
	static void CodeStoreSet(char *);
	static void TextStoreSet(char *);
	static void CSSReplacePSet(char *);
	static void CSSReplaceCSet(char *);
	static void DropDownCLinkSet(char *);
	static void DropDownPLinkSet(char *);
	static void DropDownBlockSet(char *);

	static setinfo ISecTableAccess[];
	static void ScopeColGroupSet(char *);
	static void ScopeRowGroupSet(char *);
	static void TblNoAccessSet(char *);
	static void TblScopeSet(char *);
	static void TblIDsSet(char *);

	static setinfo ISecXrefStyles[];

	static setinfo ILogOptions[];


	// in dwhtmfm.cpp

  static bool MergeCurrFormatProps;
	static bool FormatBaseSet;
  static fmtvals *GetFormatFileInfo(char *fmtname, pprop *prop);
  static char *GetCSSBorderStyle(brdrtyp btyp);

  static tbfmtvals *GetTblFormatFileInfo(char *tbfmtname);
  static rowfmtvals *GetRowFormatFileInfo(char *rowfmtname, long rnum = 0);
  static cellfmtvals *GetCellFormatFileInfo(char *cellfmtname,
                                            long rnum = 0, long cnum = 0);
  static void GetShading(HTMctl *cp, shdinfo **fptr);
  static void GetShadeColor(HTMctl *cp, shdinfo **fptr);
  static void WriteShadingFmt(HTMwfile *wf, shdinfo *sh);
  static brdrinfo *GetTableFileBorder(char *bfmt);

  static DCnlist ElementFormNames;
  static npair ElementFormNamesInit[ElementFormsMax];
  static DCnlist ElementSelectors;
  static npair ElementSelectorsInit[ElementFormsMax];
  static DCnlist ElementCssDefaults;
  static npair ElementCssDefaultsInit[ElementFormsMax];
  static DCvlist ElementCSSProps;
  static DCvlist ElementCSSVals;
  static void InitElementForms(void);
  static void DefineRequiredFormats(DCfrfile *fmts);
  static void WriteOneCSSFormat(char *fname, uns ftype = 1);

  static DCnlist CSSDone;
  static void WriteCssStyle(fmtvals *fv, pprop *prop, char *selector = NULL);

  static DCnlist CssTableStylesUsed;
  static DCvlist CssTableStyles;
  static long CssTableStyleCount;
  static DCnlist CssRowStylesUsed;
  static DCvlist CssRowStyles;
  static long CssRowStyleCount;
  static DCnlist CssCellStylesUsed;
  static DCvlist CssCellStyles;
  static long CssCellStyleCount;

  static void WriteCssTableFormats(void);
  static void WriteCssTableStyle(tbfmtvals *fv);
  static void WriteCssRowStyle(rowfmtvals *fv);
  static void WriteCssCellStyle(cellfmtvals *fv);

  static void WriteCssBorder(DCwfile *wf, char *pos, brdrinfo *br);
  static void WriteCssShading(DCwfile *wf, shinfo *sh);
  static void WriteCssColor(DCwfile *wf, long color);

  static unc CssFontUnits;
  static unc CssIndentUnits;
  static unc CssFontUnitDec;
  static unc CssIndentUnitDec;
  static long CssBodyFontSize;
  static long CssBodyFontTwips;
  static unc CssBodyFontUnit;
	static bool CssBodyFontTag;
  static long CssIndentBaseSize;
  static long CssIndentBaseTwips;
  static unc CssIndentBaseUnit;
  static char *CssUnits[CssUnitsMax];

  static void SetCssFontTwips(void);
  static void SetCssIndentTwips(void);
  static void WriteCssPts(long val, csstyp use);


  // in dwhtmtx.cpp

  static bool ProcTx(HTMctl *cp);

  static pprop Curr;
  static pprop Last;
  static pprop DefForm;
  static pprop ParaForm;
  static bool InResources;

  static void SetDefForm(void);

  static DCvlist FormList;
  static DCnlist FormNameList;
  static DCilist FormRefList;
  static DCvlist FormTabList;
  static uns FormNum;
  static uns DCLFormNum;
  static uns FormsStored;
  static char *FormName;
  static bool InFormDef;
  static bool InFormDefPfx;
  static bool InFormDefSfx;

  static void ProcForm(HTMctl *cp);
  static void ProcFormType(HTMctl *cp);

	static char *HTStyles[HtmStyleMax];
	static char *HTMLListInit[ListInitMax];
	static char *HTMLListType[ListTypeMax];

	//static uns CurrListPos[ListLevMax];
	//static unc CurrListType[ListLevMax];
	//static uns CurrListLevel;
	//static uns LastListLevel;
	static list CurrList;

	static bool InFormPx;
	static bool CSSLeading;
	static bool LinkClassIsParaClass;
	static bool XrefFormatIsXrefClass;
	static bool NoClassLists;
	static bool NoAttribLists;
	static bool UseListTypeAttribute;
	static bool ProhibitPrefix;
	static bool AllowPrefix;
	static bool ProhibitSuffix;
	static bool AllowSuffix;
	static bool ZeroCSSMargins;

  static void ProcFormName(HTMctl *cp);
  static void StoreForm(void);
  static void SetDITATags(void);
  static void SetDocBookTags(void);
	static char *GetStyleClassName(char *nm);
	static char EuroChars[EuroCharMax];
	static char *MakeID(char *txt, char *buf = NULL,
	 uns buflen = 0, char space = 0,
	 bool undscr = false, bool nodigit = true, bool lowercase = true);
  static char *NameCssClass(char *name);

	static uns CharFormLevel;
	static uns HyperFormLevel;
	static uns CurrFormNum;
  static void CheckForm(HTMctl *cp);

  static bool InGlossTitle;
  static unc GlossTitleBuf[GlossTitleBufLen];
	static uns GlossTitleCount;
	static char *GlossTitlePath;
  static void AddToGlossTitle(DCctl *cp);
  static void SetGlossTitle(void);

  static void UseForm(HTMwfile *wf, HTMctl *cp);

  static DCvlist InlineStack;
  static DCvlist InlineSrcStack;
  static DCilist InlineRefStack;
  static long InlineStackLevel;
  static long InlineHyperLevel;
  static pprop *LastCharForm;
  static void SetInline(pprop *cform);
  static void RestoreInline(HTMwfile *wf);

	static void RestoreForm(void);
  static void UseFTag(HTMctl *cp);
  static void UseSpecFmtTag(HTMwfile *wf, HTMctl *cp);
  static void InitForm(HTMwfile *wf, pprop *cform);
  static void InheritForm(pprop *cform);
	static void RestoreCurrForm(void);

  static bool InTabDef;
  static bool TabsChanged;
  static uns TabCount;
  static uns TabFillChar;
  static tinfo *CurrTab;
  static DCvlist *TabList;

	static DClist SideHFlowList;
	static bool InRunGroupDef;
  static bool SideHUsed;

  static void SetCurrProps(HTMwfile *wf, HTMctl *cp);

  static uns FontNum;
  static uns DefFont;
  static DCvlist FontList;
  static DCnlist FontNameList;
  static DCnlist FontMapList;
  static char MCDec[10];
  static char MCHexU[10];
  static char MCHex4[10];
  static char MCHex2[10];
  static char MCChr[3];
	static anyini MapChar;
  static bool CharConvertSpaces;
  static char *FontTypes[FTypesMax];
  static unc FTypes[FNamesMax];
  static char *FontNames[FNamesMax];
  static uns FCharWids[FNamesMax];
  static bool FixedWid;
  static uns CharMetrics[CharMetricMax];
  static void ProcFont(HTMctl *cp);
  static uns GetFontNum(HTMctl *cp);

	static uns FontSizeVector[FontSizesMax - 1];
  static uns GetFontSize(HTMctl *cp);

	static long FontColorVector[FontColorMax];
  static long GetFontColor(HTMctl *cp);

  static DCvlist FlowList;
  static DCilist SectList;
  static DClist SectUsedList;
  static DClist SectSkipList;
  static long SectNum;
  static long NextSectNum;
  static uns CurrFlow;
  static bool InDocBody;
  static bool InPara;
  static bool InParaGroup;
  static unc PGStarted;
  static bool ParaStartScan;
  static bool ParaStarting;
  static bool ParaEnding;
  static HTMgroup *FlowGroup;
  static HTMgroup *NormalFlowGroup;
  static uns NormalFlow;
  static void ProcFlow(HTMctl *cp);
  static void ProcFlowName(HTMctl *cp);
  static void MarkSectUsed(HTMctl *cp);

	static char *Title;
	static char *DefTitle;
	static char *HeadMacroName;
	static char *HeadEndMacroName;
	static char CssType;
	static char *CssName;
	static char *CssPath;
	static char *CssFullPath;
	static bool UseCSS;
	static bool WriteClassAttributes;
  static unc WriteCssStylesheet;
	static bool WriteCssLink;
	static char *CssBrowserDetect;

	static bool CSSLinkNS4;
	static bool UseSpanAsDefault;
	static bool Helen;
  static void WriteHead(HTMwfile *wf, vfphtm cbfunc = NULL);
	static void WriteOneCSSLink(HTMwfile *wf, char *fname);
  static void PutMeta(HTMwfile *wf);

	static uns BaseSize;
	static char *BaseFontName;
	static bool Basefont;
  static void WriteBaseProps(HTMwfile *wf);

  static void WriteFlow(HTMwfile *wf, uns flownum);
	static void WriteFlowNum(HTMwfile *wf, HTMctl *cp);

	static unl ExtFileStartLine;
	static unl ExtFileEndLine;
	static unl ExtFileStartChar;
	static unl ExtFileEndChar;
	static unl ExtCodeFileLen;
	static char *ExtCodeFileEnc;
	static char *PreFormatClass;

	static void WriteFromExtFile(HTMwfile *wf, HTMctl *cp);

  static bool HideWhite;
  static bool WhiteText;
  static bool ParaWhiteText;
  static bool RemEmptyParas;
  static bool RemEmptyTbParas;
  static bool NonWhiteText;
  static bool WordBreak;
  static bool TableWordBreak;
	static uns TableTitles;
	static uns TableDPI;
	static bool UseCodePage;
	static void WriteText(HTMwfile *wf, HTMctl *cp = NULL);
	static char *GetEncodedText(DCwfile *dwf, char *txt);
	static void WriteTextSingleChar(DCwfile *dwf, unl wch);
	static void WriteTextStringChars(HTMwfile *wf, unc *cpt, size_t len);
  static void AddChar(unc ch);
	static void WriteTextChar(HTMwfile *wf, unl wch, unl uch = 0);

	static unl SymbolUniMap[0xE0];
	static unl WingdingsUniMap[0xE0];
	static unl CheckWingSym(unc sym, unl wch);
	static bool CSSSymFontClass;
	static bool CSSWingFontClass;
	static bool WriteWingSym(HTMwfile *wf, unl uch);
	static void WriteWingSymFormats(void);
	static unl GetWingSym(unl uch, char **classatt);

	static char RuninType;
	static bool UseRunInTag;
	static long RuninDiv;
	static long PreRuninDiv;
	static pprop RuninProps;
	static DCvlist RuninOverrides;
	static bool InPreform;
	static bool MergePre;
	static char *PreClass;
	static bool UseXMLbr;
	static bool InComment;
	static bool InRaw;
	static bool MinimizeLines;
	static bool HardRetPara;
	static bool ParaStartProps;
	static bool AlignAttributes;
	static bool CellAlignAttributes;
	static bool CellColorAttributes;
	static char *ScriptTag;
	static char *ScriptType;

  static void WriteProps(HTMwfile *wf);
  static void WriteParaStartProps(HTMwfile *wf);
  static void EndSpecialBlocks(HTMwfile *wf, bool endall = false, bool line = false);
  static void WriteParaProps(HTMwfile *wf);

	//static bool InList;
	//static unc ListTypeVal;
	static bool AlwaysNestLists;
	static char *LastItemName;
  static void PutListStart(HTMwfile *wf, unc ltype, bool lstart);
  static void PutListEnd(HTMwfile *wf, bool lend);
  static void ResetListLevel(HTMwfile *wf);
  static bool ListTypesDiffer(unc t1, unc t2);

	static chprop CPTypes[CPTypesMax];
	static cptype CPStack[CPStackMax];
	static short CPStackPos;
	static bool AllowOver;
	static bool NoFont;
	static bool UseFontFace;
	static bool UseFontSize;
	static bool SymbolFont;
	static bool NoSymbolFont;
	static bool UseTypographicElements;
	static bool UseFormatTypo;
	static bool UsePFormatTypo;
	static bool UseCFormatTypo;

	static bool UseTypographicStyles;
	static char **CPStyleTypes[CPTypesMax];
	static char *TypoB;
	static char *TypoI;
	static char *TypoU;
	static char *TypoOline;
	static char *TypoStrike;
	static char *TypoChbar;
	static char *TypoSub;
	static char *TypoSup;

	static void CPSet(HTMwfile *wf, cptype CPt);
	static void CPSetBold(HTMwfile *wf);
	static void CPSetItalic(HTMwfile *wf);
	static void CPSetUline(HTMwfile *wf);
	static void CPSetOline(HTMwfile *wf);
	static void CPSetStrike(HTMwfile *wf);
	static void CPSetChbar(HTMwfile *wf);
	static void CPSetSub(HTMwfile *wf);
	static void CPSetSup(HTMwfile *wf);
	static void CPSetFont(HTMwfile *wf);

	static void CPEnd(HTMwfile *wf, cptype CPt);
	static void CPEndAll(HTMwfile *wf);
	static void CPEndLevel(HTMwfile *wf, short lev);
	static void CPEndBold(HTMwfile *wf);
	static void CPEndItalic(HTMwfile *wf);
	static void CPEndUline(HTMwfile *wf);
	static void CPEndOline(HTMwfile *wf);
	static void CPEndStrike(HTMwfile *wf);
	static void CPEndChbar(HTMwfile *wf);
	static void CPEndBase(HTMwfile *wf);
	static void CPEndFont(HTMwfile *wf);

  static unc CharBlockMacro;
  static unc CharBlockState;
  static char *CurrCSName;
  static char *LastCSName;
  static char *LastCTagName;
  static char *LastDTypoEnd;
  static pprop *CurrCSForm;
  static pprop *LastCSForm;

  static void StartCharBlock(HTMwfile *wf);
  static void EndCharBlock(HTMwfile *wf, bool endcond = true);
  static void EndOneCharBlock(HTMwfile *wf, bool endcond = true);

  static void WriteCharProps(HTMwfile *wf);
  static void WriteEndCharProps(HTMwfile *wf, bool ending = false);

  static bool UseMapDescAsTitle;
  static long CurrMapTopicCtl;
  static bool GettingMapDescText;
  static char *CurrMapDescription;
  static DCnlist MapDescList;
  static DClist MapSkipList;
  static void ProcMapTopicHead(HTMctl *cp);
  static void ProcMapDesc(HTMctl *cp);

  static uns LinePos;
  static uns CharWid;
  static char *TabTypes[TabTypeMax];
  static uns TabCharWidth;
  static uns TabCharsPerInch;
	static bool AnumTabs;
	static bool UseAnums;

  static void WriteTabDefs(HTMwfile *wf, pprop *style);
  static void WriteTabs(HTMwfile *wf);

	static char PgBreaks;
	static bool UnwrapPRE;
  static void EndLine(HTMwfile *wf, HTMctl *cp);
  static void WriteBreak(HTMwfile *wf);

  static HTMctl *ParaStartCtl;
  static HTMctl *TxtParaStartCtl;
  static HTMctl *TbCellParaStartCtl;
  static HTMctl *PreFtnParaStartCtl;
  static HTMctl *PreFtnTxtParaStartCtl;
  static HTMctl *PrevStartCtl;
	static bool PrevParaIsTb;
  static long ItemRefNum;
  static long ParaRefNum;
  static long PrevParaRefNum;
  static long TxtParaRefNum;
  static long CharRefNum;
  static long StartingParaRef;
  static long StartSplitParaRef;
  static long PreFtnParaRefNum;
  static long PreFtnTxtParaRefNum;
	static bool PrevParaKeepNext;
	static bool ParaKeepNext;
	static frame *PrevParaFrameAfter;
	static frame *ParaFrameAfter;
	static frame *FrameLinks;
  static DClist EmptyParaList;

  static void ProcParaStart(HTMctl *cp);
	static pvars SavedParaVars;
	static tinfo SavedTab;
  static void SaveParaVars(void);
  static void RestoreParaVars(void);
	static pvars SavedParaVarsFtn;
  static void SaveParaVarsFtn(void);
  static void RestoreParaVarsFtn(void);
  static void ProcParaEnd(HTMctl *cp);
	static char *FixObjectID(char *mac, char *orig);

  static DCvlist PRefFrameList;
  static DCvlist PRuninFrameList;
  static long RuninRefNum;
	static bool AnchorFrames;
	static bool GraphANamesFirst;
  static long CurrGraphFrameID;
  static void ReAnchorFrame(void *vp);
  static void ProcFrAnchor(HTMctl *cp);
  static void ExecFrame(DCfile *wf, void *vp);

	static DCnlist CodeStoreParaList;
	static DCnlist CodeStoreDivList;
	static char *CodeStoreVar;
	static long CodeStoreDiv;
	static long PreCodeStoreDiv;
	static bool NoParaClose;
	static bool NoWrap;

  static void StartPara(HTMwfile *wf, HTMctl *cp);
  static void EndPara(HTMwfile *wf);
  static void WriteParaFramesAndTables(HTMwfile *wf,
		            DClist **fmlist, DClist **tblist, tbl **ftbl = NULL);

#if 0
	static bool NestXML;
	static long XMLNestLevel;
	static DCvlist XMLNestList;
  static void StartNestedXmlBlock(HTMwfile *wf, char *tag);
  static bool EndNestedXmlBlock(HTMwfile *wf, char *tag);
  static void EndAllNestedXmlBlocks(HTMwfile *wf);
#endif

	// in dwhtmxf.cpp

	static uns Contents;
	static bool MakeContents;
	static bool NoContLink;
	static bool NoContRef;
	static char *ContentsRef;
	static char *ContentsWin;
	static uns ContentsLevel;
	static uns ContentsTabLev;
	static bool ContentsLevEmpty;
	static DClist ContentsLevSkip;

	static char *TocIdxFilePrefix;

	static uns IndexLevel;
	static char *IndexLevelClass;
	static char *IndexLetterClass;
	static char *IndexTopLettersClass;
	static char *IndexHeadingClass;
	static char *IndexTOCClass;
	static char *IndexRefClass;
	static char *IndexLetPrefix;
	static char *IndexNumPrefix;
	static unc IndexNavType;
	static bool WriteIndexCssLink;
	static bool UseIndexLevelNum;
	static bool UseIndexLetters;
	static bool UseIndexTopLetters;
	static bool UseListProps;
	static bool UseIXLets;
	static bool UseIXTopLets;
	static char *IndexLetterSymbol;
	static char *IndexLetterNumber;
	static bool UseIndexHeading;
	static char *IndexFileName;
	static char *IndexFileSuffix;
	static char *IDXSuffix;
	static char *IndexFileTitle;
	static char *IndexRefStartSep;
	static char *IndexRefSep;
	static void **IndexRefs;
	static long IndexRefCount;
	static long IndexRefAlloc;
	static bool UseIndexentryTag;

	static void **ALinkRefs;
	static long ALinkAlloc;
	static unc ALinkFileRefs;
	static void **GlossRefs;
	static long ContentsID;
	static bool ContentsUsed;
	static DCvlist ContentsList;
	static DCilist ParaContentsList;
	static DCilist ContentsSplitList;
	static DCnlist ContentsWindowList;
	static long NewlinkCount;
	static bool UseRawNewlinks;
	static DCilist ContentsPartList;
	static long ContentsPartCount;

	static bool Eclipse;
	static bool IncludeVersionPI;
	static char *EclipseVer;
	static char *EclipsePluginSchemaVersion;
	static char *EclipseTocLabel;
	static char *EclipseTocTopic;
	static char *EclipseTocFilename;
	static char *EclipseTocLink;
	static bool EclipseTocPrimary;
	static bool EclipseTocFileOnly;
	static char *EclipseTocExtradir;
	static char *EclipseIdxFilename;
	static bool EclipseUseIndex;
	static bool EclipseUsePlugin;
	static bool EclipseWritePlugin;
	static bool EclipseUseManifest;
	static bool EclipseWriteManifest;
	static char *EclipseManifestName;
	static bool EclipseUseSingleton;
	static bool EclipseUseRuntime;
	static char *EclipsePlugName;
	static char *EclipsePlugID;
	static char *EclipsePlugVer;
	static char *EclipsePlugProvider;
	static bool EclipseUseContext;
	static bool EclipseWriteContext;
	static char *EclipseContextName;
	static char *EclipseContextPlugName;
	static bool EclipseDescription;
	static bool DescriptionIsFirstLabel;
	static bool EclipseContextAnchors;
	static long EclipseContextCount;
	static DCnlist EclipseContextNames;
	static DCvlist EclipseContextRefs;
	static DCvlist EclipseContextTopics;

	static DCvlist MapNewlinkList;
	static DCnlist MapNewlinkNameList;
	static DCvlist MapPopupList;
	static long PopupCount;
	static DCvlist MapTopicList;
	static long TopicCount;

	static DCwfile *ContentsFile;
	static bool ContentsFileStarting;
	static DCwfile *IndexFile;
	static bool IndexFileStarting;
	static DCwfile *MapFile;
	static DCwfile *SearchFile;
	static DCwfile *AliasFile;
	static DCwfile *AliasTitleFile;
	static bool AliasFileStarting;
	static DCwfile *ALinkFile;
	static DCwfile *GlossFile;
	static long GlossCount;
	static char *CshMapFile;
	static bool MakeCshMapFile;
	static unl CshMapFileNumStart;
	static unl CshMapFileNumIncrement;
	static char *HelpFileTitle;
	static char *HelpFileLanguage;
	static char *HomeID;
	static char *HomeURL;
	static bool WriteHelpProjFile;
	static bool StoreCshFiles;
	static bool WriteHelpProjFileSet;
	static bool WriteHelpSetFile;
	static char *DefaultHelpProjectFile;
	static char *DefaultHelpSetFile;
	static char *DefaultTopicFile;
	static char *HHContValPx;
	static bool HHContFileOnly;
	static bool HSPathNames;
	static bool UseFTS;
	static bool UseSubHelpSets;
	static bool UseGlossary;
	static bool UseFavorites;
	static bool JHVersion2;
	static bool JHImages;
	static DCnlist JHImagesName;
	static DCnlist JHImagesLoc;
	static DCilist ExpandLevel;
	static DCnlist ImageLevel;

	static char *GlossPrefix;
	static char *GlossSuffix;
	static char *GlossSpace;
	static char *JHDtdVer;
	static char *JHFileVer;
	static char *HHCProperties;
	static char *HHKProperties;

	static char *OHProjectName;
	static char *OHProjFileSuffix;
	static char *OHProjFilePrefix;
	static bool OHProjFileXhtml;
	static char *ProjectTemplate;
	static DCirfile *OHTemplate;
	static char *OHTitle;
	static char *MainName;
	static char *NavElems;
	static char *FrameLeft;
	static char *FrameTop;
	static char *FrameHigh;
	static char *FrameWide;
	static char *FrameOptions;
	static char *TopHigh;
	static char *LeftWide;
	static char *MidHigh;
	static bool TopFirst;
	static bool UseTopButtons;
	static bool UseStart;
	static bool UsePrevNext;
	static bool UseBackForward;
	static bool UseHideShow;
	static bool ShowNavLeft;
	static bool NewWindow;
	static bool CloseOldWindow;
	static bool PersistSettings;
	static bool OHLowMem;
	static char *CtrlCssName;
	static char *MainCssName;
	static char *IECtrlCssName;
	static char *N6CtrlCssName;
	static char *N4CtrlCssName;
	static char *IECssName;
	static char *N6CssName;
	static char *N4CssName;
	static bool OHTocGroupsOpen;
	static char *OHTocOpenLevel;
	static char *OHTocIcoBase;
	static bool OHTocExpand;
	static bool OHTocButtons;
	static bool OHIdxGroupsOpen;
	static char *OHIdxOpenLevel;
	static char *OHIdxIcoBase;
	static bool OHIdxExpand;
	static bool OHIdxButtons;
	static bool OHRelShowSubjects;
	static DClist MergeList;
	static DCnlist MergeLocList;
	static bool MergeFirst;

	static void WriteReferenceFiles(DCwfile *wf);
	static void SetJHLists(void);

	static void WriteHelpProjectFile(char *fname);
	static void WriteEclipsePlugin(void);
	static void WriteEclipseManifest(void);
	static void WriteEclipseContext(void);
	static void WriteContext(DCwfile *mwf, char *chp, long i);
	static void WriteContextTopic(DCfile *mwf, void *vp);
	static void WriteHelpSet(char *fname);
	static void PutJHPresentation(DCwfile *wf, char *item, long id);
	static void PutJHHelpAction(DCwfile *wf, char *action);
	static void MapJHImage(char *image);

	static void WriteOHProjectFile(char *fname);
	static char *OHProjHeadHtml;
	static char *OHProjHeadXhtml;
	static char *OHRunMsg;
	static char *OHPFileDef;
	static char *OHXFileDef;
	static void WriteOHProjFile(char *tpl, char *name, char *ext, char *cp);
	static char *FixOHString(unc *chp);
	static size_t IsEntity(char *str);
	static char *OHMacroVars(char *name);
	static void WriteOHProjHead(HTMwfile *wf);
	static void WriteOHMergeInfo(HTMwfile *wf);
	static void WriteOHMergeItem(DCwfile *dwf, long id);

	static void WriteContentsStart(void);
	static void WriteSearchStart(void);

	static void WriteIndexStart(void);
	static void WriteSpecialIndexStart(void);
	static char *IndexTopInsert;
	static char *IndexHeadInsert;
	static char *IndexBottomInsert;
	static char *IndexTOCInsert;
	static bool InsertIndexTOC;
	static DClist IndexSplitFile;
	static bool UseDITAIndex;

	static void CheckIndexTOCItem(HTMctl *cp);
	static void InsertIndexTOCItem(HTMwfile *wf, HTMctl *cp);
	static void SetIXListProps(HTMctl *cp);

	static char *SetIndexFileName(char *fname);
	static void WriteIndexCSS(HTMwfile *wf);
	static void WriteIndexJS(HTMwfile *wf);
	static void WriteIndexFileFormats(void);

	static void WriteALinkStart(void);
	static void WriteGlossStart(void);
	static void WriteMapFileStart(void);

	static void WriteContentsEnd(void);
	static void WriteSearchEnd(void);
	static void WriteIndexEnd(void);
	static void WriteSpecialIndexEnd(void);
	static void WriteALinkEnd(void);
	static void WriteGlossEnd(void);
	static void WriteMapFileEnd(void);

	static void WriteContentsItem(DCfile *cwf, void *ip);
	static void WriteContentsAttr(DCwfile *wf, char *txt);

	static void WriteSearchItem(DCwfile *cwf, char *cp, long id);
	static void WriteSearchVal(DCwfile *cwf, long id);

	static char IndexSortType;
	static char *IndexSortLocale;
	static bool IndexRanges;
	static unc AlphaSortChars[AlphaSortCharMax];
	static void SortIndexRefs(void);
	static void SetIndexArray(void *vp, long id);
	static int CompIdxNames(const void *p1, const void *p2);
	static void WriteIndexRefs(void);
	static long ScanIndexRefs(long start, long end, long levoff, long levnum);
	static char *SkipLevMark(char *first, long *levpt);
	static long GetLevLen(char *first);

	static char *CurrIdxTitle;
	static char *SeeTerm;
	static char *SeeAlsoTerm;
	static bool SortSeeAlsoFirst;
	static bool UseSortString;
	static char CurrOHLetter;
	static void WriteIndexTitle(DCwfile *wf, char *txt);
	static void WriteIndexItem(ixitem *ip, long pos);
	static void SetIndexLetter(char ch);
	static void PutIxId(char ch);
	static void ChangeIndexLevel(long nlev);
	static void StartIndexSubs(void);
	static void EndIndexSubs(void);
	static void ZeroIndexLevel(void);
	static void StartIndexEnt(void);
	static void EndIndexEnt(void);
	static void StartIndexRefs(void);
	static void StartIndexRef(void);
	static void EndIndexRef(void);
	static void EndIndexRefs(void);

	static void WriteIndexItemParam(char *name, char *val, bool lit = false);
	static long IndexItemRefCounter;
	static void WriteIndexItemRef(long pos);
	static bool IdxNameMatch(char *ixname, char *rname, long len);
	static void WriteSeeAlsoRef(char *rname, long refpos);

	static void WriteDCLIndexRefs(void);

	static void SortALinkRefs(void);
	static int CompALinkNames(const void *p1, const void *p2);
	static void SetALinkArray(void *vp, long id);
	static void WriteALinkRefs(void);
	static void WriteDCLALinkRefs(void);
	static void WriteALinkItem(DCwfile *cwf, char *cp, long id);
	static void WriteALinkVal(DCwfile *cwf, long id);

	static bool GlossHere;
	static DCvlist GlossList;
	static DCnlist GlossParaList;
	static DCnlist GlossIDList;
	static long GlossAlloc;
	static void SetGlossTerm(HTMwfile *wf, char *term, char *id);
	static char *MakeGlossID(char *term);
	static void SortGlossRefs(void);
	static int CompGlossNames(const void *p1, const void *p2);
	static void SetGlossArray(void *vp, long id);
	static void WriteGlossRefs(void);
	static void WriteDCLGlossRefs(void);

	static DCnlist *MapItemList;
	static long MapItemCount;
	static char *MapFilePrefix;
	static void WriteMapFileItem(char *id, char *url, char *window = NULL);
	static void WriteMapRef(void *vp, long i);

	static void IncContentsLevel(void);
	static void DecContentsLevel(void);
	static void WriteContentsTabs(void);
	static void WriteContentsSpaces(void);
	static void WriteIndexTabs(void);
	static void WriteIndexSpaces(void);

	static void WriteAliasFullFile(void);
	static void WriteAlias(DCfile *mwf, void *vp);

	static void WriteCSHMapFile(char *projname);

	static void WriteContentsDCL(DCfile *cwf, void *ip);
	static void WriteSearchDCL(DCwfile *cwf, char *chp, long id);
	static void WriteIndexDCL(DCLwfile *wf, ixprop *ip);
	static void WriteMapDCL(DCfile *mwf, void *vp);
	static void WriteContextDCL(DCwfile *mwf, char *chp, long i);
	static void WriteContextTopicDCL(DCfile *mwf, void *vp);

	static void ProcGraphCopyFiles(DCrfile *rf, char *buf);

	static void WriteContentsFile(DCrfile *rf, char *dest, char *buf);
	static void ProcContentsPartFile(char *name);

	static void WriteSearchFile(DCrfile *rf, char *dest, char *buf);
	static void ProcSearchPartFile(char *name);

	static void WriteIndexFile(DCrfile *rf, char *dest, char *buf);
	static void ProcIndexPartFile(char *name);

	static void WriteALinkFile(DCrfile *rf, char *dest, char *buf);
	static void ProcALinkPartFile(char *name);
	static void ProcOHALinkPartFile(char *name);

	static void WriteGlossFile(DCrfile *rf, char *dest, char *buf);
	static void ProcGlossPartFile(char *name);

	static void ProcContextPartFile(char *name);

	static void ProcMapPartFile(char *name);

	static void WriteAliasFile(DCrfile *rf, char *dest, char *buf);
	static void ProcAliasPartFile(char *name);

	static DCnlist Suffixes;
	static npair SuffixesInit[];
	static char *ReplaceExt(char *name, char *ext = NULL);



  // in dwhtman.cc

  static bool ProcAn(HTMctl *cp);

  static bool PutStyleMacro(HTMwfile *wf, unc pos,
	             char *tag = NULL, pprop *cform = NULL);
  static void PutAnumMacro(HTMwfile *wf, bool start);

	static char *BeginFileInsert;
	static char *EntitiesInsert;
	static char *HeadInsert;
	static char *HeadEndInsert;
	static char *FramesInsert;
	static char *TopInsert;
	static char *BottomInsert;
	static char *EndInsert;
	static char *TopicBreak;
	static char *FirstHeadInsert;
	static char *FirstHeadEndInsert;
	static char *FirstFramesInsert;
	static char *FirstTopInsert;
	static char *FirstBottomInsert;
	static char *FirstEndInsert;
	static char *LastHeadInsert;
	static char *LastHeadEndInsert;
	static char *LastFramesInsert;
	static char *LastTopInsert;
	static char *LastBottomInsert;
	static char *LastEndInsert;
	static char *SplitHeadInsert;
	static char *SplitHeadEndInsert;
	static char *SplitFramesInsert;
	static char *SplitTopInsert;
	static char *SplitBottomInsert;
	static char *SplitEndInsert;
	static char *NonsplitHeadInsert;
	static char *NonsplitHeadEndInsert;
	static char *NonsplitFramesInsert;
	static char *NonsplitTopInsert;
	static char *NonsplitBottomInsert;
	static char *NonsplitEndInsert;
	static char *ExtrHeadInsert;
	static char *ExtrHeadEndInsert;
	static char *ExtrTopInsert;
	static char *ExtrBottomInsert;

  static void PutInsertMacro(HTMwfile *wf, char *place);

	static bool FixMacroQuotes;
  static short MacroNestMax;
  static short RepeatMax;
  static short WhileMax;
	static char *MacroFile;
	static bool OmitMacroReturns;
	static bool MacroVarNesting;
	static char *CurrPrjName;
	static char *CurrPrjPath;

	static DCnlist TextStoreNames;
	static DCnlist TextStoreVals;
	static DCilist TextStoreIDs;
	static long TextStoreCount;
  static char *GetTextStore(char *vp);
	static bool WriteCodeStoreVar(DCwfile *df, char *mn);
  static char *GetCodeStoreVar(DCwfile *df, char *vp);
  static char *GetPredefinedVar(DCwfile *df, char *vp);
  static char *GetFrameUserVar(char *vp);
  static long GetFrameUserArrVar(char *vp, long idx);
  static char *GetVarText(uns vnum, bool macvar = true);

	static bool NameUndefMacroVars;
	static bool NameUndefMacros;

	static bool WriteMacroChar(DCwfile *df, unc uch);
	static bool WritePredefinedMacro(DCwfile *df, char *name);

  static char *TopicStartCode;
	static DCvlist TopicStartMacros;
  static void EvalTopicStartMacros(HTMwfile *wf, long id);
  static void WriteOneTopicStartMacro(DCfile *dwf, void *vp);

  static void ProcNameTitleMacros(HTMwfile *wf);
  static void UseNameTitleMacro(DCwfile *w, char **n, long i);
  static void UseSplitNameMacro(DCwfile *w, char **n, long i);

	static bool UseNavtitleMarkers;
  static void ProcContentsTitles(void);
  static void UpdateContTitle(long cid, long sid);

  static char *CurrFSTitle;
  static char **TopMacro;
  static char *TopTitle;
  static char *PrevFSBase;
  static char *PrevFSName;
  static char *PrevFSTitle;
  static char **PrevMacro;
  static char **PrevFSMacro;
  static char **StartingPrevFSMacro;
  static char *StartingPrevFSTitle;
  static char *NextFSBase;
  static char *NextFSName;
  static char *NextFSTitle;
  static char **NextMacro;
  static char **NextFSMacro;
  static char **EndingNextFSMacro;
  static char *EndingNextFSTitle;
  static DCilist PrevFSNameUsage;
  static DCilist PrevFSTitleUsage;
  static DCilist NextFSNameUsage;
  static DCilist NextFSTitleUsage;
  static char *TopLink;
  static char *PrevLink;
  static char *PrevFSLink;
  static char *StartingPrevFSLink;
  static char *NextLink;
  static char *NextFSLink;
  static char *EndingNextFSLink;
  static char *TopButton;
  static char *PrevButton;
  static char *PrevFSButton;
  static char *StartingPrevFSButton;
  static char *NextButton;
  static char *NextFSButton;
  static char *EndingNextFSButton;
	static bool FSSet;
	static bool FirstFS;
	static bool LastFS;
	static bool UseNavButtons;
	static void SetNavMacro(char *macname, char **mac);
	static DCnlist FSNames;
	static DCnlist FSTitles;
	static long CurrFSPos;
	static long LastFSPos;
	static void SetFileSeqNames(void);
  static void SetFileSequence(void);
  static char *GetPrevFSName(HTMwfile *wf);
  static char *GetPrevFSTitle(HTMwfile *wf);
  static char *GetNextFSName(HTMwfile *wf);
  static char *GetNextFSTitle(HTMwfile *wf);

	static void WritePrevMacro(HTMwfile *wf);
	static void WriteNextMacro(HTMwfile *wf);

	static bool MadeWithUsed;
	static bool WriteMadeWithFile;
	static char *MadeWithImageFile;
	static size_t MadeWithImageSize;
	static bool MadeWithPara;
	static bool MadeWithLink;
	static char *MadeWithAttributes;
	static void WriteMadeWithImage(void);
	static void WriteResGraphic(char *fname, const int gid, size_t imsize);
	static void WriteMadeWithMacro(HTMwfile *wf);

	static bool MakeLocalTOC;
	static char *LocalTOCStart;
	static char *LocalTOCItem;
	static char *LocalTOCEnd;
	static long LocalTOCID;
	static long LocalTOCLevel;
	static bool LocalTOCSubs;
	static long LocalTOCIndent;
	static long LocalTOCDiv;
	static long LocalTOCSeq;
	static DCilist CurrLocalTOCList;
  static DCilist LocalTOCParentList;
  static DCnlist LocalTOCTitleList;
  static DCilist LocalTOCSeqList;
  static DCilist LocalTOCFmtList;

	static void AddLocalTOCItem(void);
	static void WriteLocalTOC(HTMwfile *wf);
	static void WriteLocalTOCItem(DCwfile *pwf, long v);

	static bool FilterConditions;
	static char *FilterJSFile;
	static char *FilterDialogTitle;
	static char *FilterBodyAttr;
	static char *FilterLinkHref;
	static uns FilterDialogWide;
	static uns FilterDialogHigh;
	static char *FilterSelectBar;
	static char *FilterBarStart;
	static char *FilterBarSpan;
	static char *FilterBarEnd;
	static char *FilterBarBodyAttr;
	static DCnlist FilterDialogAttributes;
	static void SetFilterAttributes(void);
	static void WriteFilterDialogSettings(DCwfile *wf);
	static char *WriteFilterSelectBar(void);
	static void WriteFilterDisplayStyle(HTMwfile *wf);


	static bool DropDownUsed;
	static unc DropLinkType;
	static char *DropLinkStart;
	static char *DropLinkEnd;
	static char *DropLinkParaStart;
	static char *DropLinkParaText;
	static char *DropLinkParaEnd;
	static char *DropBlockStart;
	static char *DropBlockEnd;
	static bool ClickBlockToClose;
	static bool WriteDropIconFiles;
	static char *DropOpenIcon;
	static char *DropOpenIconFile;
	static char *DropOpenIconAlt;
	static char *DropCloseIcon;
	static char *DropCloseIconFile;
	static char *DropCloseIconAlt;
	static char *DropButton;
	static char *DropButtonOpenLabel;
	static char *DropButtonCloseLabel;
	static char *DropClass;
	static char *DropText;
	static char *DropIDPrefix;
	static char *DropLinkAttr;
	static char *DropButtonAttr;
	static char *DropDivAttr;
	static char *DropJSCode;
	static char *DropJSLocation;
	static bool WriteDropJSFile;
	static bool UseCompositeDropJS;
	static char *DropJSCodeComp;
	static char *DropJSCodeIcon;
	static char *DropJSCodeText;
	static char *DropJSCodeButton;
	static char *DropJSHead;
	static char *DropJSFile;
	static long DropID;
	static bool DropLinkStarted;
	static bool DropLinkCharSet;
	static void WriteDropJSCode(HTMwfile *wf);
	static void WriteDropDownHead(HTMwfile *wf);
	static void WriteDropDownJSFile(void);
	static void WriteDropLinkPara(HTMwfile *wf);
	static void StartDropLink(void);
	static void WriteDropLinkStart(HTMwfile *wf);
	static void WriteDropLinkEnd(HTMwfile *wf);
	static void WriteDropLinkIcons(void);

	static bool MakeTrail;
	static bool SplitTrail;
	static char *TrailStart;
	static char *TrailSep;
	static char *TrailEnd;
	static char *TrailLinkClass;
	static char *TrailPrefix;
	static char *TrailSuffix;
	static unc TrailCurrent;
	static unc TrailPosition;
	static uns TrailFirstLevel;
	static uns TrailLastLevel;
	static uns TrailCurrLevel;
	static uns TrailIndent;
	static long TrailParas[TrailLevMax];
  static DCilist TrailSplitIDs;
  static DCvlist TrailParaList;
  static DCilist TrailLevelList;
  static DCnlist TrailTitleList;

	static void AddTrailItem(void);
	static void WriteTrail(HTMwfile *wf, bool macro = false);

	static void WriteTemplatePages(void);

  static DCnlist MarkList;
  static DCnlist MarkTypeList;
	static long MarkTypes;
	static DCvlist CellAttrLists;
	static DClist CellColGroupList;
	static DClist CellRowGroupList;
	static DClist CellSpanList;
	static DCnlist CellScopeList;
	static DCnlist CellIDList;
	static DCvlist RowAttrLists;
	static DCvlist TblAttrLists;
	static DCvlist ParaAttrLists;
	static DCvlist CharAttrLists;
	static DCvlist LinkAttrLists;
	static DCvlist JH2PopAttrLists;
	static DCvlist JH2SecAttrLists;
	static DCnlist *CellAttrLp;
	static DCnlist *RowAttrLp;
	static DCnlist *TblAttrLp;
	static DCnlist *GraphAttrLp;
	static DCnlist *LinkAttrLp;
	static DCnlist *JH2PopAttrLp;
	static DCnlist *JH2SecAttrLp;
	static DCnlist *ParaAttrLp;
	static DCnlist *CharAttrLp;
	static bool AttrHere;
	static char *AttrMarkerName;
	static char *NoProp;

  static void ProcMarkerGroup(HTMctl *cp);
  static void ProcMarker(HTMctl *cp);
	static char *MarkerProps[MarkerPropsMax];
	static DCilist TextMarkerList;
	static DCnlist ExtrReplaceMark;
	static DCnlist ExtrHeadMark;
	static DCnlist ExtrTopMark;
	static DCnlist ExtrBottomMark;
	static DCnlist HTMwr::MarkerIDList;

	static char *MarkName;
	static bool MarkerIDNext;
	static bool MarkerSetSplit;
	static bool MarkerSetExtract;
	static HTMctl *MarkerCtl;
	static char *ObjectID;

	static DClist ALinkMarkerList;
	static DCnlist ALinkMarkerRefList;
	static DCnlist ALinkParaRefList;
	static bool ALinkPara;

	static void ProcMarkerType(HTMctl *cp, char *kp, char *txt);
	static char *GetMarkerGroupText(DCgroup *gp);
	static void ProcMergeMarker(char *txt, bool linked = false);

	static char *SetXIntRef(void);
	static void WriteMarker(HTMwfile *wf, HTMctl *cp);
  static void AddMarkerCode(char *name, char **ptxt);

  static void AddAttrMarker(char *nm, char *txt);
  static void GetCellAttrs(char *txt, long mt);
  static void GetRowAttrs(char *txt, long mt);
  static void GetTblAttrs(char *txt, long mt);
  static void GetParaAttrs(char *txt, long mt);
  static void GetCharAttrs(char *txt, long mt);
  static void GetGraphAttrs(char *txt, long mt);
  static void GetLinkAttrs(char *txt, long mt);
  static void GetMetaAttrs(char *txt, long mt);
  static void GetRefAttrs(char *txt, long mt);
  static void WriteAttrListItem(DCwfile *w, char *n, long i);
  static void WritePropListItem(DCwfile *w, long i);

  static char HyperSpaceChar;
	static bool HyperJump;
	static bool HyperJumpStarting;
	static bool ParaFullLink;
  static uns HyperVal;
  static long HyperJumpID;
  static char *HyperJumpStr;
  static DCnlist HyperJumpStrList;
  static DCilist HyperJumpTypeList;

  static bool HyperJumpFile;
  static long HyperJumpFileID;
  static DCnlist HyperJumpFileList;
  static char *HyperJumpFileName;

  static bool HyperLinkIDNext;
  static DCnlist HyperLinkRefList;
  static char *HyperLinkID;

  static bool HyperPara;
  static long HyperParaID;
  static DCilist HyperParaList;

  static bool HyperForm;
  static bool HyperFormEnding;
	static long HyperFormNumber;
  static long HyperFormStartID;
  static long HyperFormEndID;
  static DCilist HyperFormStartList;
  static DClist HyperFormEndList;

  static void SetHyperStart(long id);
  static void SetHyperEnd(void);
  static void CheckHyper(HTMctl* cp);
  static void CheckHyperEnd(HTMctl* cp);

  static bool UseHyperColor;
  static bool HyperColor;
  static bool HyperColorEnding;
  static bool ParaHyperColor;
  static long HyperColorStartID;
  static long HyperColorEndID;
  static DCilist HyperColorStartList;
  static DClist HyperColorEndList;

  static void ProcHyperColor(HTMctl *cp);
  static void ProcHyperColorStart(HTMctl *cp);
  static void ProcHyperColorEnd(HTMctl *cp);
  static void ProcTextColor(HTMctl *cp);

	static uns CurrVar;
	static DCvlist VarList;
	static DCnlist VarNameList;
	static DCnlist UserVarNameList;
	static uns TbContVar;
	static bool TbContUse;
	static char *TbContVarName;
	static uns TbSheetVar;
	static bool TbSheetUse;
	static char *TbSheetVarName;
	static DCnlist SystemVarNames;
	static npair SystemVarNamesSet[];

	static unc DITAVarType;
	static char *DITAVarFile;
	static char *DITAVarID;
	static char *DITAVarElem;
	static bool WriteVarFile;
	static DCilist VarUsedList;

#ifdef UNUSED
	static bool InDateToken;
	static long PreDateDiv;
	static long DateDiv;
	static uns ParaVarType;
#endif

  static void SetVarEntities(HTMwfile *wf, long sid = 0);
  static void ProcVarEnts(DCwfile *dwf, long val, long ix);
  static void ProcVarEnt(DCwfile *dwf, long val);
  static void WriteVarElementsFile(HTMwfile *wf);
  static void ProcVarElems(DCwfile *dwf, char *nm, long val);

  static void SetVarForm(HTMctl *cp);
  static void CheckVarName(HTMctl *cp);
	static bool CheckVarForm(void);
  static void PutVarForm(HTMwfile* wf, HTMctl *cp);

  static DCilist VarArraySize;
  static DCvlist VarArrayList;
  static long CurrVarArrayCount;
  static DCvlist *CurrVarArrayList;
  static long CurrVarArrayItem;
  static HTMgroup *CurrVarArrayForm;

  static void SetVarArray(HTMctl* cp);
  static void SetVarArrItemForm(HTMctl* cp);
  static void SetVarArrItem(unl var);
  static void ProcTitleArrVar(HTMctl* cp);
  static void PutVarArrItemForm(HTMwfile* wf, HTMctl *cp);

  static bool InChapNum;
  static bool InVolNum;
  static bool InSectNum;
  static bool InSSectNum;
	static char *ChapNum;
	static char *VolNum;
	static char *SectionNum;
	static char *SSectNum;
	static uns ChapNumVType;
	static uns VolNumVType;
	static uns SectNumVType;
	static uns SSectNumVType;
	static char *PageNum;
  static void PutSystemVar(HTMwfile* wf, HTMctl *cp);

	static char *XrefFile;
	//static char *XrefRawFile;
	static bool XrefMapped;
	static bool XrefListed;
	static bool XrefFound;
	static bool MakeFileHrefsLower;
	static bool ATagLineBreak;

	static DCnlist XRFNames;
	static DCnlist XRFNewNames;
	static long XRFCount;

	static bool RemoveFilePaths;
	static bool UseXrefFilePrefix;
	static bool DefXrefToCurr;
	static bool UseListedXrefFilesOnly;
	static bool XrefInCurrDBFile;
	static char *CurrFilePrefix;
	static long CurrPxLen;
	static char *XrefFilePrefix;
	static char *XrefFileSuffix;
	static DCnlist FIDNames;
	static DCnlist FIDPrefixes;
	static long FIDCount;

	static bool XrefLinkIDNext;
	static char *XrefLinkID;

	static char *StripSuf(char *name);
	static void SetFIDLists(void);
	static char *GetFIDPrefix(char *fname);
	static char *CurrXFileName;
	static char *CurrXFilePath;
	static char *CurrXFileTitle;
	static char *CurrXFileBase;
	static void SetXRFLists(void);
	static char *GetXrefName(char *fname);
  static void SetXrefFileName(char *name, size_t len = 0);
	static void ProcHyperFile(HTMctl *cp);

	static void ProcHyperMark(HTMctl *cp);
	static bool OpenlinkIsFile;
	static void ProcHyperLink(HTMctl *cp, char *txt = NULL);
	static void SetHyperLink(HTMctl *cp, char *ahref, uns val = 3);
	static void ProcHyperObj(HTMctl *cp, bool page = false);
	static void ProcHyperAlert(HTMctl *cp, char *txt = NULL);
	static void ProcHyperURL(HTMctl *cp, char *txt = NULL);
	static void PutHyperMacro(HTMwfile *wf, HTMctl *cp, char *txt = NULL);

	static DCnlist HyperLocList;
	static void ProcHyperLoc(HTMctl *cp);
	static void PutHyperLoc(HTMwfile *wf, HTMctl *cp);

	static DCnlist TopicAliasList;
	static void ProcTopicAlias(HTMctl *cp);
	static void PutTopicAlias(HTMwfile *wf, HTMctl *cp);

	static bool MakeAliasFile;
	static bool UseAliasAName;
	static bool UseDITAAlias;
	static bool AliasTitle;
	static char *AliasPrefix;
	static char **AliasPxs;
	static long AliasPxCount;
	static DCvlist AliasList;
	static long AliasCount;
	static void SetAliasPxs(void);
	static bool CheckAlias(char *chp);

	static DCnlist HyperUniRefList;
	static DCilist HyperUniIdList;
	static long HyperUnique[HULevMax];
	static short HULevel;
	static char HUType;
	static void ProcHyperUnique(HTMctl *cp, bool page = false);
	static void PutHyperUnique(HTMwfile *wf, HTMctl *cp);
	static void WriteHyperUnique(HTMwfile *wf);
	static void GetHyperUniqueRef(char *str, long cp);

	static void PutHypertext(HTMwfile *wf, HTMctl *cp);
	static void PutHyperTarget(HTMctl *cp);
	static void WriteHyperAHref(HTMwfile *wf);

	static bool ALinkHere;
	static DCnlist ALinkList;
	static bool MakeALinkFile;
	static long ALinkCount;
	static char *ALinkRef;
	static DCvlist OracleALinkList;
	static DCnlist OHALinkList;

	static void WriteALinkObject(HTMwfile *wf, char *aname);
	static void WriteOneALink(HTMwfile *wf, char *aname);

	static short PopTextNum;
	static char *PopFont;
	static char *PopText;
	static char *PopMargins;
	static char *PopColors;
	static char *PopFormat;
	static long ScriptDiv;
	static long ExtrScriptDiv;

	static void WriteHyperPopup(HTMwfile *wf, char *href, bool map = false);
	static void WritePopupObject(HTMwfile *wf);

	static bool SecWin;
	static short SecWinNum;
	static char *DefaultChmFile;
	static char *DestChm;
	static char *ChmFormat;
	static char *SecWinParamFormat;
	static char *SecWinJumpFormat;
	static bool RemoveChmFilePaths;
	static bool BinaryIndex;
	static bool BinaryTOC;

	static void SetDestChm(char *fname = NULL, char *oname = NULL);
	static void WriteSecWinJump(HTMwfile *wf, char *ahref, bool url = false, bool map = false,
	                            long attrid = 0);
	static void WriteSecWinObject(HTMwfile *wf, char *id, char *win, char *ref, size_t fnpos);

  static void PutObjects(HTMwfile *wf, bool final = false);
	static void WriteParam(HTMwfile *wf, char *name, char *val);
	static void PutScript(HTMwfile *wf);

	static char *PopMarkerPrefix;
	static char *JH2PopMarkerPrefix;
	static char *SecMarkerPrefix;
	static char *JH2SecMarkerPrefix;

	static char JH2PopType;
	static char *JH2PopText;
	static char *JH2PopSize;
	static char *JH2PopLocation;
	static char *JH2PopFontFamily;
	static char *JH2PopFontSize;
	static char *JH2PopFontWeight;
	static char *JH2PopFontStyle;
	static char *JH2PopFontColor;
	static char *JH2PopGraphic;

	static char JH2SecType;
	static char *JH2SecText;
	static char *JH2SecSize;
	static char *JH2SecLocation;
	static char *JH2SecName;
	static char *JH2SecFontFamily;
	static char *JH2SecFontSize;
	static char *JH2SecFontWeight;
	static char *JH2SecFontStyle;
	static char *JH2SecFontColor;
	static char *JH2SecGraphic;

	static char JH2TmpType;
	static char *JH2TmpText;
	static char *JH2TmpSize;
	static char *JH2TmpLocation;
	static char *JH2TmpName;
	static char *JH2TmpFontFamily;
	static char *JH2TmpFontSize;
	static char *JH2TmpFontWeight;
	static char *JH2TmpFontStyle;
	static char *JH2TmpFontColor;
	static char *JH2TmpGraphic;

	static void WriteJHPopup(HTMwfile *wf, char *href, bool map = false,
	                         long attrid = 0);
	static void WriteJHSecWinJump(HTMwfile *wf, char *ahref, bool url = false,
	                              bool map = false, long attrid = 0);
	static void ClearJH2Tmps(void);
	static void SetAttrJH2Item(char *n, long i);

	static void WriteOHJPopup(HTMwfile *wf, char *href, bool map = false);
	static void WriteOHJSecWinJump(HTMwfile *wf, char *ahref, bool url = false, bool map = false);

	static DCnlist XrefNameList;
	static DCnlist XrefLinkSrcList;
	static long XrefID;
	static long XrefInstID;
	static char *XrefInstStr;
	static anyini XrefIni;
	static DCilist XrefUseList;
	static bool XrefTextOnly;
	static bool XrefDelete;
	static bool XrefNoRef;
	static bool XrefLinkSrc;
	static void ProcXrefFormatName(HTMctl *cp);
	static void ProcXrefFormatProp(char *propval, long *valp);
	static void ProcXrefFormatUse(HTMctl *cp);

	static DCnlist XrefRefList;
	static DCnlist XrefParaList;
	static char *XrefIDRef;
	static char XrefSpaceChar;
	static bool XrefLocNext;
	static void ProcXrefAName(HTMctl *cp);
	static char *MakeXrefName(HTMctl *cp);
	static void PutXrefAName(HTMwfile *wf, HTMctl *cp);

	static void WriteAName(HTMwfile *wf, char *str);
	static char *PendingANames[ANameLevMax];
	static uns ANameLevel;
	static void StorePendingAName(char *str);
	static void WritePendingANames(HTMwfile *wf);

	static bool InXref;
	static bool XrefSecWin;
	static HTMctl *XrefStart;
	static char *XrefSecWinName;
	static DCnlist XrefSecWinUse;

	static void ProcStartAnchor(HTMctl *cp);
	static void ProcEndAnchor(HTMctl *cp);

	static void StartAnchor(HTMwfile *wf, HTMctl *cp);
	static void StartXrefText(HTMwfile *wf, HTMctl *cp);
	static void EndAnchor(HTMwfile *wf);
	static void SetXrefTitle(DCctl *cp);

	static bool InHyper;
	static bool NoHyper;
	static char *DefaultTarget;
	static char *TargetFrame;
	static char *URLTarget;
	static char *XrefTitle;
	static bool NoATags;
	static bool NoAHrefs;
	static bool NoANames;
	static bool XMLLinkAttrs;
	static bool NoLocations;
	static bool KeepReplacedCharLinks;
	static bool UseChmInLinks;
	static bool CharFormInLink;
	static long LinkDiv;
	static char *LinkSrc;
	static char *ATagName;
	static char *HrefAttribute;
	static bool UseHash;
	static bool UseUlink;
	static bool UsingUlink;
	static bool RemoveXrefHotspots;
	static bool UsingXrefDiv;
	static long XrefDiv;
	static long PreXrefDiv;
	static long XrefLevel;

	static bool LinkTypeA;
	static char LinkType;
	static char *LinkFlags;
	static char *LinkEmptyTopic;
	static char *LinkButtonWidth;
	static char *LinkButtonHeight;
	static char *LinkButtonText;
	static char *LinkButtonGraphic;
	static char *LinkButtonIcon;
	static char *LinkTextFont;
	static char *LinkText;
	static char ALinkType;
	static char *ALinkFlags;
	static char *ALinkEmptyTopic;
	static char *ALinkButtonWidth;
	static char *ALinkButtonHeight;
	static char *ALinkButtonText;
	static char *ALinkButtonGraphic;
	static char *ALinkButtonIcon;
	static char *ALinkTextFont;
	static char *ALinkText;
	static char KLinkType;
	static char *KLinkFlags;
	static char *KLinkEmptyTopic;
	static char *KLinkButtonWidth;
	static char *KLinkButtonHeight;
	static char *KLinkButtonText;
	static char *KLinkButtonGraphic;
	static char *KLinkButtonIcon;
	static char *KLinkTextFont;
	static char *KLinkText;

	static char *MSHHctrl;

	static bool Confluence;
	static bool ConfluenceLinks;
	static bool InConfluenceLink;
	static char *ConfluenceLinkStart;
	static char *ConfluenceLinkPage;
	static char *ConfluenceLinkPageEnd;
	static char *ConfluenceLinkText;
	static char *ConfluenceLinkTextEnd;
	static char *ConfluenceLinkEnd;

	static void StartAHref(HTMwfile *wf, char *str, unc typ = 0, bool addChm = false, long attrid = NULL);
	static void SetAttrLinkItem(char *n, long i);

	static char *XLSMacro;
	static char *XrefName;
	static char *CheckLinkSrc(HTMwfile *wf, char *chp);
	static void WriteLinkItems(HTMwfile *wf, char *chp);
	static void SetTargetFrame(char *ref, char *ref2 = NULL);
	static void EndAHref(HTMwfile *wf);

	static bool InAnumVal;
	static bool InAnumGroup;
	static bool EndingAnum;

  static uns FtnStart;
  static uns FtnForm;
  static uns FtnMarker;
  static bool InFootnote;
  static bool StartingFootnote;
  static bool InFtnAnum;
	static bool InTbFtnAnum;

	static char FootType;
	static long FootDiv;
	static long PreFootDiv;
	static char *FootSep;
	static char *FootnoteStartCode;
	static char *FootnoteEndCode;
	static long TbFootDiv;
	static char *TbFootSep;
	static char *FootSepH;
	static char *FootSepX;
	static char *FootClass;
	static char *TbFootClass;
	static long FootVType;
	static long TbFootVType;
	static char *FootTypeAttrs;
	static char FootTypeAttr[2];
	static char TbFootTypeAttr[2];
	static bool InFtPx;
	static bool InTbFtPx;
	static bool InAnFormat;
	static bool InAnRefFormat;
	static bool InAnSyms;
	static char FootPx[];
	static char TbFootPx[];
	static char FootSx[];
	static char TbFootSx[];
	static char FootRPx[];
	static char TbFootRPx[];
	static char FootRSx[];
	static char TbFootRSx[];
	static char FootSyms[];
	static char TbFootSyms[];
	static char *FootElem;
	static char *TbFootElem;
	static char *FootRElem;
	static char *TbFootRElem;
	static char *FootItemTag;
	static char *TbFootItemTag;
	static char *FtItemTag;
	static char *FootBlockTag;
	static char *TbFootBlockTag;
	static uns FtnPosition;
	static bool StartingFtn;
  static bool EndingFtn;
  static bool TbFootPos;
  static bool UseFootnoteParaCode;
  static bool UseFootnoteLists;
  static bool UseTbFootnoteLists;
  static bool NoFootnoteLinks;
  static bool InFootPara;
  static char *FootInlineTag;
  static char *FootInlineParaTag;
  static char *FootInlineIDPrefix;
  static char *FootInlineXrefTag;
  static bool UseFootXrefTag;
  static bool UsingFootXrefTag;
  static char *FootID;
  static bool FootnoteXref;

  static uns FootnoteNum;
  static uns TbFootNum;
  static uns TxFootNum;
  static DCilist FootnoteParaRefList;

  static void WriteFtnStart(HTMwfile *wf);
	static char *SetNumVal(long ntyp, uns nval, char *syms);
  static void WriteFtnEnd(HTMwfile *wf);
  static void SetFootDiv(HTMwfile *wf, bool table);
  static void FinishFtns(HTMwfile *wf, bool final = false, bool table = false);

  static bool InIdxGroup;
  static bool StartingIdxGroup;
  static bool IdxSeeAlso;
  static bool IdxEnded;

  static bool IdxStartRange;
  static bool IdxEndRange;
  static bool IdxInRange;
	static DClist StartRangeList;
	static DClist EndRangeList;
	static DClist StartRangeIDList;
  static DCvlist IndexRangeList;
  static char *CurrIndexEntry;

  static bool IndexEntry;
	static bool IndexCounted;
	static bool InIndexSort;
	static bool AfterIndexColon;
  static short IndexCount;
  static short IndexObjCount;
  static char *IndexBuf;
  static short IndexPos;
  static short IndexSPos;
  static short IndexAllocUnit;
  static short IndexAlloc;
  static short KeywordLimit;
	static unc KeywordRefs;
	static unc RefFileType;
	static bool CombineIndexLevels;
	static bool UseCommaAsSeparator;
	static bool LevelBreakForSee;
	static bool CaseSensitiveIndexCompare;
  static char *IgnoreCharsIX;
  static char *IgnoreLeadingCharsIX;
	static HTMctl *IndexRefCtl;
	static DCvlist IndexRefList;
	static DClist SeeAlsoList;

	static bool UseHVIndex;
	static DCvlist HVIndex;
	static DClist *CurrHVIndex;
	static void ProcHVIndex(char *txt, long sid);
	static void WriteHVIndexItems(HTMwfile *wf, long sid);

  static void ProcIdxType(HTMctl *cp);
  static void PutIndexEntry(HTMctl *cp);
  static void ProcIdxFlags(char *ent);
  static void WriteIndexEntry(HTMwfile *wf);
  static void SetKeyword(char *key);
	static char *ProcIXSortString(char *sort, bool seealso, bool sepsort = false);
  static void ResetRangeKeyword(void *vip, long i);
  static void FindStartSetRangeKeyword(void *vip, long i);
  static void PutRangeKeyword(DCfile *wf, void *vip);
  static void FindStartPutRangeKeyword(void *vip, long i);
  static void RemoveRangeKeyword(long i);
  static void PutKeyword(HTMwfile *wf, char *key, bool range = false);

	// in dwhtmsp.cpp

  static DCilist ParaSplitList;
  static DCnlist SplitNameList;
  static DCnlist SplitRefList;
  static DCnlist BaseNameList;
  static DCnlist SplitTitleList;
  static DCilist PrevSplitList;
  static DCilist NextSplitList;
  static DCnlist SplitNumList;
  static DClist ExcludeFromPrint;
  static DClist ExcludeFromSearch;
  static DClist ExcludeFromView;
  static DClist *ExcludedTopics;

	static bool FileStarted;
	static bool SplittingFile;
	static bool SplitHere;
	static bool StartingSplit;
	static bool SmartSplit;
	static bool NoSplit;
	static bool UseChunking;
	static uns SplitLevel;
	static uns LastSplitLevel;
	static bool AllowTbSplit;
	static long SplitID;
	static long FirstSplitID;
	static long LastSplitID;
	static bool FirstPart;
	static bool LastPart;
	static long SplitNum;
	static long NextSplitNum;
	static long SplitNumIncrement;
	static char *SplitNumStart;
	static char SplitIDRef[NumStrMax];
	static char FirstSplitIDRef[NumStrMax];
	static char LastSplitIDRef[NumStrMax];
	static long ParaUID;
	static char ParaUIDRef[NumStrMax];
	static long TxtParaUID;
	static bool KeepParaUID;

	static void ProcSplit(void);
	static void ProcSplitName(long sid);
	static bool SplittingOK(void);
	static void CloseFileBlocks(HTMwfile *wf);
	static void SplitFile(HTMwfile *wf, long spID);

  static DCilist ExtrStartList;
  static DCilist ExtrEndList;
  static DCnlist ExtrTagList;
  static DCvlist ExtrFormList;
	static bool Extracting;
	static bool ExtractEnable;
	static bool ExtrStarting;
	static bool ExtrFinishing;
	static bool ExtrEnding;
	static char *ExtrFile;
	static long ExtrID;
	static unc ExtrGraphThumbnail;
	static bool OrigSizedThumbnail;
	static char *ExtrGraphSuffix;
	static char *ExtrGraphClass;
	static char *ExtrGraphTarget;
	static char *ExtrGraphHigh;
	static char *ExtrGraphWide;
	static char *ExtrGraphOrigHigh;
	static char *ExtrGraphOrigWide;
	static char *ExtrGraphID;
	static char *ExtrGraph;
	static char *ExtrStartTag;
	static pprop *ExtrStartForm;
	static char ExtrIDRef[NumStrMax];

	static void ProcStartExtract(void);
	static void ProcEndExtract(void);
	static void StartFileExtract(HTMwfile *wf, long extrID);
	static void EndFileExtract(HTMwfile *wf);
	static void WriteExtrThumb(HTMwfile *wf);


	static bool TitleHere;
	static bool NameHere;
	static bool FileNameHere;
	static bool UseRawName;
	static bool KeepFileNameUnderscores;
	static bool KeepFileNameSpaces;
	static bool ChangeFileNameSpaces;
	static bool UseFilePxSxInBaseName;
  static unc FileNameSpaceChar;
  static unc NameBuf[NameBufMax];
	static uns NameBufCount;
	static char *NamePrefix;
	static char *NameSuffix;
	static char *TitlePrefix;
	static char *TitleSuffix;
	static char *MetaName;
	static uns MetaCount;
  static DCnlist MetaList;
  static DCnlist MetaNameList;
  static DCvlist MetaPartList;
	static bool StoreText;
	static bool TStoreHere;
	static char *TStoreName;
	static bool ConfigHere;
	static bool AllowTbTitle;
	static bool TitleSet;
  static unc TitleBuf[TitleBufMax];
	static uns TitleBufCount;
	static bool SkipAnums;
	static bool StripTitleSpaces;

	static bool FinalIniNameHere;
	static bool IniPartNameHere;
  static DCnlist FinalIniNameList;
  static DCnlist IniPartNameList;
	static char *IniPartName;
  static char *FinalIniName;
	static DCirfile *FinalIniFile;
	static DCirfile *FinalIniParent;

	static bool SearchHere;
	static bool PrintHere;
	static bool PrintOnly;
	static bool PrintProject;
	static bool DescHyph;

	static void ProcTitleText(HTMctl *cp);
	static void ProcTitleVarText(HTMctl *cp);
	static void ProcTitleVar(uns vnum, bool macvar = true);
	static char *ProcOutputText(HTMctl *cp);
	static void PutTitleBuf(unc ch);
	static void PutTitleBufSpec(uns ch);
	static void PutTitleBufEnt(unc ch);
	static char *GetTitleBuf(bool mem = false);

	static DCnlist SearchWordList;
	static long SearchWordMin;
	static DCnlist StopWordList;
	static long StopWordCount;
	static bool UseListButton;
	static bool UseSearchHighlight;
	static char *SearchHighlightStyle;
	static bool UseDefaultStopWords;
	static char *DefStopWords[];
	static bool UnicodeFTS;
	static char *UnicodeLocale;
	static char *CompoundWordChars;

	static void InitOHSearch(void);
	static void ProcOHUnicodeSearch(char *txt, long id);
	static void ProcOHSearch(char *txt, long id);
	static void ProcUnicodeSearchWord(char *txt, long id);
	static int cmplexstrs(const char *s1, const char *s2);

	static void ProcSearchWord(char *txt, long id);


	static long TextInsetNum;
	static long TextInsetLev;
	static DCilist ParaStartsTextInset;
	static DCilist ParaEndsTextInset;
	static DCilist TextInsetStack;
	static DCilist TextInsetStartingPara;
	static DClist TextInsetStartsPara;
	static DCilist TextInsetEndingPara;
	static DCnlist TextInsetName;
	static DCnlist TextInsetFile;
	static DClist TextInsetMainFlow;
	static DCilist TextInsetFlowPg;
	static DCnlist TextInsetFlowTag;
	static bool RemoveTextInsets;
	static bool TextInsetEnding;
	static bool TextInsetMark;
	static bool TextInsetNest;

	static void ProcStartTextInset(HTMctl *cp);
	static void ProcEndTextInset(HTMctl *cp);
	static void StartTextInset(HTMwfile *wf, HTMctl *cp);
	static void EndTextInset(HTMwfile *wf, HTMctl *cp);


	static uns *AnsiMap;
	static uns EE1250Map[AnsiLen];
	static uns Cyr1251Map[AnsiLen];
	static uns Gr1253Map[AnsiLen];
	static uns Turk1254Map[AnsiLen];
	static void SetAnsiMap(char *txt);
	static uns MapAnsiChar(uns ch);


	// in dwhtmpp.cpp

	static bool ListMissingRefs;
	static bool CheckAllRefs;
	static bool ExtRefUsed;
	static char RefBuf[RefBufLen];
	static char *LinkID;
	static char *LinkRefFile;
	static DCvlist LinkFixupList;
	static long LinkFixupNum;
	static link **FixupArray;
	static DCnlist IntRefList;
	static DClist *IntRefPend;

	static long IntRefCount;
	static long ExtRefCount;
	static long FilesFixed;
	static void WriteRefCount(void);

	static void SetIntRef(char *txt);
	static void ProcIntRef(char *txt, long sid);
	static void FixupFileRefs(void);
	static void SetFileRef(void *vp, long num);
	static int CompRef(const void *p1, const void *p2);
	static void FixOneFileRef(void *vp, long num);

	static bool AllowPadding;
	static long Padded;

	static bool FixItem(char *fn, long loc, char *oname, char *name,
	                    long olen, long len, long pad, bool allowpad);
	static void FixRefLocs(char *rname, char *fn, long loc, long adjust);
	static void FixFSRefLocs(char *rname, char *locsect, char *idsect,
	 char *fn, long loc, long adjust);
	static char *ProcExtRef(char *rtxt, bool hyp = false, bool ext = false,
		bool dita = false);
	static void WriteRefFileInfo(DCfile *wf, size_t fnpos, long divused);
	static void FixOneDivRef(DCwfile *wf, long lnum);

	static DCvlist ExtRefList;
	static DCvlist ExtRefPartList;
	static DCilist ExtScriptRefList;
	static DCilist ExtObjectRefList;
	static long ExtRefNum;
	static char *ExtRefFileName;

	static void PutExtRefInfo(DCwfile *wf);
	static void PutOneExtRef(void *vp);

	static void SetMissingRef(char *fname, char *txt, int err);
	static void ProcFileNameRef(char *ref, char *fn);

	static void PutGraphGenInfo(void);
	static void PutGrGenItem(char *name, long genid);

	static bool RefFile;
	static char *RefFileName;
	static void SetUpRefFile(void);
	static bool WriteEmptyRefFile(FILE *fp, char *fn, char *fname);
	static bool GrxFile;
	static char *GrxFileName;
	static bool WriteEmptyGrxFile(FILE *fp, char *fn);

	static bool GrcFile;
	static char *GrcFileName;
	static bool WriteEmptyGrcFile(FILE *fp, char *fn);
	static void PutGraphCopyInfo(void);
	static void PutGrCopyItem(char *name, long genid);

	static DCnlist RefFileList;
	static DCnlist RefFileBaseList;
	static uns NextRefFileNum;
	static char *GetRefFileName(char **fn);

	static bool StartSplit;
	static char *PrevRefFile;
	static char *NextRefFile;
	static char *FirstTitle;
	static char *LastTitle;

	static void ProcSplitRefInfo(void);
	static void SetSplitRefInfo(void);
	static void FixFSItems(char *ref, char *idsect, char *locsect,
		                     char *oname, char *name, long olen, long len);

	static char *PrjFileName;
	static char *IDFileName;
	static char *FrameBook;
	static bool UseLocalFileID;
	static char *GetFileID(char *fn);
	static char *BaseName(char *fn, size_t len = 0);
	static bool IncAlpha(char *str);
	static bool FileIDUsed(char *id);
	static bool FindIDFile(void);



  // in dwhtmtb.cc

  static bool ProcTb(HTMctl *cp);
  static void ProcTblProp(HTMwfile *wf, HTMctl *cp);

  static DCvlist TblFormList;
  static tbl *TblForm;
  static bool InTblDef;
  static col *ColForm;
  static uns TbCol;
	static bool TableWidthType;
	static bool TableIDPend;
	static bool KeepTableClassSpace;
  static void ProcTblForm(void);
  static void StoreTblForm(void);
  static void UseTblForm(HTMctl *cp);
  static void UseTblTag(HTMctl *cp);
  static void UseSpecTblTag(HTMctl *cp);
  static void SetTblID(HTMctl *cp);

  static DCvlist TblCpList;
  static DClist *TblList;
  static DClist *PreSufTbls;
  static tbl *Tbl;
  static tbl *FigTable;
  static tbl CTbl;
	static anyini TblIni;
  static bool CALS;
  static bool InTbl;
  static bool InTbTitle;
  static bool InTDefaults;
  static bool IntTbCaption;
  static bool TableAttributes;
  static bool UseAltShading;
  static bool StripTable;
  static bool SavedStrip;
  static bool CloseStrippedTables;
 	static bool UseInformaltableTag;
  static bool UsingInformaltableTag;
	static char *TblCaptionTag;
  static col *Col;
  static uns ColNum;
  static uns LastColwid;
  static uns DefColWidth;
	static uns *CellRowStrad;
	static uns *CurrRowStrad;
	static uns TblBorderThick;
	static uns TblCellPadding;
	static uns TblCellSpacing;
	static char TblWidType;
	static char TTblWidType;
	static uns TblDPI;
	static long TblWid;
	static long TblCpID;

	static DCilist TblDivRefList;
	static long TblDivType;

	static char *PixelSpacerImage;
	static char *SpacerAlt;
  static bool UseSpacers;
  static bool SpacerUsed;
  static bool WriteSpacerFile;
	static unc SpacerImage[SpacerImageSize];
	static uns TblSpacerWide;
	static short TblIndents;

	static char *RowAttrib;
	static char *RowStartMacro;
	static char *RowEndMacro;
	static char *CellAttrib;
	static char *CellStartMacro;
	static char *CellEndMacro;

	static long TblFigureDiv;
	static long PreTblFigureDiv;

  static void ProcTbl(HTMwfile *wf, HTMctl *cp);
  static void SetTblIni(void);
  static void ProcTblCol(HTMctl *cp);
  static void WriteTbl(HTMwfile *wf);
  static void WriteTblDiv(HTMwfile *wf, long div, char *name,
	                        char *attr, long dtype);
	static void WriteSpacer(HTMwfile *wf, short width, short height = 0);
	static void WriteSpacerImage(void);
  static void WriteTbTitle(HTMwfile *wf);

  static row *Row;
  static row *RowForm;
  static row *LastRow;
  static row *NextRow;
  static uns TbRowType;
  static uns RowNum;
  static uns FirstBodyRow;
  static char *RowAlgns[RowAlgnMax];
	static uns RowColor; 
	static uns RowFill; 
 	static long RowCpID;
  static bool TUseRowColor;
  static bool UseRowColor;
  static bool FirstCol;
	static void ProcRow(HTMctl *cp);
  static void ProcRowEnd(void);
	static void WriteRowAttrListItem(DCwfile *w, char *n, long i);
  static void WriteTbRow(HTMwfile *wf);

  static cell *Cell;
  static bool InTbCell;
  static bool UseTbHeaderCode;
  static uns THeaderCols;
  static uns HeaderCols;
  static uns THeaderRows;
  static uns HeaderRows;
  static uns TFooterRows;
  static uns FooterRows;
	static char *CellFillPats[CellFillMax];
  static char *CellBordStyles[];
  static uns CellStradCount;
  static short StradCellwid;
  static short LastCellwid;
	static long CellCpID;
  static void SetCellMarg(HTMctl *cp);
  static void ProcCell(HTMctl *cp);
  static void ProcCellEnd(void);
  static void ProcCellRowStrad(HTMctl *cp);
  static void ProcCellStrad(HTMctl *cp);
  static void StartCellText(HTMctl *cp);
  static void AddCellCont(HTMctl *cp);
  static DClist *NextCellCont;
  static void ProcNextCellCont(HTMctl *cp);

  static void UseParaForm(HTMctl *cp);

	static unc AccessMethod;
  static char TableID[];
	static unl TableIDVal;

	static bool TColIDs;
	static bool TRowIDs;
	static bool TColSpanIDs;
	static bool TRowSpanIDs;

	static bool ColIDs;
	static char *ColHead;
	static bool RowIDs;
	static bool RowIDSet;
	static char *RowHead;
	static bool ColSpanIDs;
	static char *ColSpanHead;
	static bool RowSpanIDs;
	static char *RowSpanHead;
	static bool SpanHeadSpace;

	static bool TColGroupIDs;
	static bool ColGroupIDs;
	static bool TRowGroupIDs;
	static bool RowGroupIDs;
	static char *ColGroupHead;
	static char *RowGroupHead;
	static short GroupNum;
	static short ColGroupNum;
	static short RowGroupNum;
	static short *ColGrp;

	static bool THeadFootBodyTags;
	static bool HeadFootBodyTags;
	static bool FootTagLast;
	static bool TColGroupElements;
	static bool ColGroupElements;
	static bool *ColGrpStart;

	static short *ColHdr;
	static short CellSpan;
	static DCilist ColSpanList;
	static DCilist RowSpanList;
	static bool TScopeCol;
	static bool TScopeColGroup;
	static bool TScopeRow;
	static bool TScopeRowGroup;
	static bool ScopeCol;
	static bool ScopeColGroup;
	static bool ScopeRow;
	static bool ScopeRowGroup;
	static bool CellScopeSet;

  static void PutColSpanHead(DCwfile *dwf, long val);
  static void PutRowSpanHead(DCwfile *dwf, long val);
	static void WriteWaiAttributes(HTMwfile *wf, bool head, bool top);
  static void StartHeaderAttrs(HTMwfile *wf, bool *pstart);

	static bool CellAttrAlign;
	static bool CellAttrValign;
	static bool CellAttrWidth;
	static bool CellAttrBgcolor;
	static char *EmptyTbCellContent;
	static char *EmptyParaContent;
	static long CellStartLevel;
	static void WriteCellAttrListItem(DCwfile *w, char *n, long i);
	static void CheckStradCellContent(HTMctl *cp);
  static void WriteCell(HTMwfile *wf);

  static DCvlist RuleFormList;
  static DCnlist RuleNameList;
  static brdrinfo *RuleDef;
  static brdrinfo NoBrdr;
  static bool InRuleDef;
  static void ProcRuleForm(void);
  static void ProcRuleProp(HTMctl *cp);
  static void StoreRuleForm(void);
  static void GetRule(HTMctl *cp, brdrinfo **rptr);

  static uns FillMap[FillMapMax];
  static void GetFill(HTMctl *cp, uns *fptr);
	static long FillColorValue(uns fill, long colr);

  static long ColorTable[ColorTableMax];
	static DCilist ColorDefs;
	static DCnlist ColorNames;
	static long ColorsDefined;
  static void GetColor(HTMctl *cp, long *cptr);
  static void GetColorS(HTMctl *cp, uns *cptr);
	static long ColorValue(long colr);
	static void ProcColorDef(long cnum, long cval);
	static long GetColorNum(long cval);

  // in dwhtmcn.cpp

  static long CurrCond;
  static long ConflictCond;
  static DClist HiddenConds;
  static DCnlist CondNames;
  static DCnlist CondAttrs;
  static char *CurrCondName;
;
  static bool ShowHiddenCond;
  static bool ShowTbRow;
  static bool HideTbRow;
  static bool EmptyTable;
	static DClist EmptyTableList;
	static void ProcTbCondUse(HTMctl *cp);

	static bool UseCharRangeClasses;
	static unl *RangeStarts;
	static char **RangeClasses;
	static rangetype *RangeTypes;
	static unl *RangeConds;
	static long CurrCharRange;
	static long RangeCount;
	static long FirstRangeCond;
	static long LastRangeCond;
	static void SetCharRangeConds(void);
	static void StoreRangeCond(char *name);
	static bool ProcCharRangeText(HTMctl *cp);
	static uns GetCharRange(unl ch);
	static void AddTextCtl(char *str, long len);
	static void AddCharRangeCond(uns cond);

  static bool InCondDef;
  static condprop *CurrCondProps;

  static DCvlist CondProps;
	static void StartCondDef(uns cond);
	static void ProcCondName(HTMctl *cp);
	static char *ProcCondAttr(char *name);
	static void SetCondProps(HTMctl *cp);
	static char *CondFlagClass;
	static char *StoreCondFlag(char *image, char *alt);
	static void ProcCondDef(void);

	static bool UseConditionalFlagging;
	static char *CSSFlagsFile;
	static char *FlagsFullPath;
	static bool WriteFlagsFile;
	static bool ReferenceFlagsFile;
	static bool AppendFlagsFile;
	static DCwfile *FlagsFile;
	static void WriteFlagsCSSFile(void);
	static void WriteOneFlagRule(DCfile *df, void *vp, long cond);
	static void CheckCondConflict(HTMwfile *wf, long cnt, DClist *conds);

  static DClist *CurrConds;
  static DClist *CondCharChanged;
  static DClist *ImageConds;
  static DClist *TableConds;
  static DClist *TableStartConds;
  static DClist *TableEndConds;
  static DClist *FtnConds;
  static DClist *LinkConds;
  static DClist *IndexConds;
  static DClist *ParaConds;
  static DClist *CharConds;
  static DClist *RowConds;
  static DClist *TextConds;
  static DCvlist CondImages;
  static DCvlist CondTables;
  static DCvlist CondFtns;
  static DCvlist CondLinks;
  static DCvlist CondIndexes;
  static DCvlist CondParas;
  static DCvlist CondRows;
  static DCvlist CondChars;
	static bool CondChanged;
	static bool CondCharPending;
	static bool ChangedCharCond;
	static bool InCharCond;
	static bool InTextCond;
	static bool UseParaCondClasses;
	static bool CharFormInTextCond;
	static bool LinkInTextCond;
	static char *ConditionCharTag;
	static char *CurrCondClass;
	static void ProcCondStart(long cnum);
	static void ProcCondEnd(long cnum);
	static void ProcCondImage(void);
	static void ProcCondTable(void);
	static void ProcCondFtn(long ftnnum);
	static void ProcCondLink(long linknum);
	static void ProcCondIndex(long indexnum);
	static void ProcCondParaStart(void);
	static void ProcCondParaEnd(void);
	static void ProcCondRowUse(long cnum);
	//static void ProcCondRowStart(void);
	static void ProcCondRowEnd(void);
	static void ProcCondCharStart(void);
	static void ProcCondCharEnd(void);
	static void CondStart(long cnum);
	static void CondEnd(long cnum);
	static void StartCondImage(HTMwfile *wf);
	static void SetCondTable(HTMwfile *wf, long tablenum = 0);
	static void StartCondTable(HTMwfile *wf, long tablenum = 0);
	//static bool CheckCondFtn(long ftnnum);
	static void StartCondFtn(HTMwfile *wf);
	static bool CheckCondLink(long linknum);
	static void StartCondLink(HTMwfile *wf, long linknum);
	static void SetCondIndex(long indexnum);
	static void StartCondIndex(HTMwfile *wf);
	static void StartCondParaBlock(HTMwfile *wf);
	static void StartCondRow(HTMwfile *wf);
	static bool SetCondCharBlock(void);
	static bool CheckCondCharBlock(void);
	static void StartCondCharBlock(HTMwfile *wf);
	static void CondTextChange(HTMwfile *wf);
	static void CondTextStart(HTMwfile *wf);
	static void CondTextEnd(HTMwfile *wf);

	static DCnlist StartCondFlags;
	static DCnlist EndCondFlags;
	static DClist CurrCondFlags;
	static DCnlist PendStartCondFlags;
	static DCnlist PendEndCondFlags;
	static bool CondsEffective;
	static long CondFlagDiv;
	static long PreCondFlagDiv;
	static void SetCondFlags(HTMwfile *wf);
	static void FilterConds(DClist *clist, unc filtlevel);

  static DClist *SavedCurrConds;
  static DClist *SavedParaConds;
  static DClist *SavedCharConds;
  static DClist *SavedTextConds;
	static void SaveCondState(void);
	static void RestoreCondState(void);

  static DClist *SavedCurrCondsFtn;
  static DClist *SavedParaCondsFtn;
  static DClist *SavedCharCondsFtn;
  static DClist *SavedTextCondsFtn;
	static void SaveCondStateFtn(bool clear);
	static void RestoreCondStateFtn(void);


  // in dwhtmgr.cc

  static bool ProcGr(HTMctl *cp);

  static uns PaperWide;
  static uns PaperHigh;
  static uns LMarg;
  static uns RMarg;
  static uns TMarg;
  static uns BMarg;
  static page *Page;
  static DCvlist PageList;
  static uns LastPage;
  static uns LastMasterPage;
  static bool StartingPage;
  static bool NewBPage;
  static uns LastBPage;
  static uns PFrameCount;
  static void ProcPage(HTMctl *cp);
  static void ProcPgEnd(HTMctl *cp);
  static void ProcPgBody(void *vp);
  static void ProcPgCols(void *vp);
  static void ProcPgAttr(HTMctl *cp);
  static void ProcPgSize(HTMctl *cp);

  static uns DefCols;
  static dcframe *Frame;
  static frame *RFrame;
  static DCvlist FrameList;
  static DCvlist FramePageList;
  static uns LastFrame;
  static DClist HeaderList;
  static DClist FooterList;
  static DClist DocBodyList;
  static DCvlist BodyFrameList;
  static DCvlist GrTextFrameList;
  static bool CopyOriginalGraphics;
  static DCnlist GraphFileCopyList;
  static DCnlist GraphFileFixedCopyList;
  static long GraphCopyCount;
  static frame *GrTextFrame;
	static frame *FrameStack[FrameStackMax];
  static uns FrameLevel;
	static bool IncludeMaps;
	static bool StartingFrame;
	static bool UseGraphicPreviews;
	static bool StripGraphPath;
	static bool KeepGraphicsInPara;
	static bool InGraphicText;
	static bool UseOrigGrName;
	static bool FixGraphSpaces;
	static char *GraphSuffix;
	static char *GraphPath;
	static int FrIDLen;
	static char *FramePropName;
	static char *FramePropValue;

  static void ProcFrame(HTMctl *cp);
  static void InitFrame(frame *fr);
  static void EndFrame(HTMctl *cp);
  static void ProcEndFrame(void);
  static void ProcFrAttr(HTMctl *cp);
  static void ProcFrSize(HTMctl *cp);

  static char *FrameHAlgn[FrameHAMax];
  static DClist *FramePendList;
  static DClist *PreSufFrames;
  static bool WritingFrame;
  static bool UseTitleForAlt;
  static bool AllowEmptyAlt;
  static bool MathFullForm;
  static DCvlist AreaList;
	static char *AreaBase;
	static short AreaScale;
	static area *CurrArea;
	static char *GrTextHyperFileName;
  static bool TextFrameIsText;
  static bool TextFrameContent;
  static DClist TextFrameIsTextList;
  static bool XMLGraphAttrs;
	static char *ImgTagElement;
	static char *ImgSrcAttr;
	static char *GraphGroup;
	static char *GraphSrc;
	static char *GraphBase;
	static char *GraphOrigHigh;
	static char *GraphOrigWide;
	static char *GraphClass;
	static char *GraphParaAlign;
	static char *GraphAlign;
	static char *GraphFile;
	static char *GraphDpi;
	static char *GraphAttr;
	static char *GraphALT;
	static char *GraphStartMacro;
	static char *GraphReplaceMacro;
	static char *GraphEndMacro;
	static char **GraphParents;
  static short GraphIndent;
  static short GraphRightSpacer;
  static short GraphScaleVal;
  static short GraphWide;
  static short GraphHigh;
  static bool GraphPathOverrides;
  static bool GraphScale;
  static bool UsePxSuffix;
  static bool UsePtSuffix;
  static bool RefPageGraphIndent;
  static bool GraphAlignAttributes;
  static bool GraphWrapPara;
  static bool ImgNames;
  static bool GraphSrcUsed;
	static DCnlist GrGenList;
	static DCnlist GrDpiList;
	static DClist GrAltList;
	static DClist GrLDescList;
	static DClist GrClassList;
	static short EqFrExpand;
	static anyini GraphIni;

  static bool MultiImageFigures;
  static bool UseOtherpropsDpi;
  static bool FigureTitleStartsFigure;
  static bool FigureOpen;
  static bool FigureOpening;
  static bool FigsInStart;
  static long FigureLev;
  static long FigureDiv;
  static long PreFigureDiv;
  static long FigParentDiv;
	static DClist FigureDivRefList;
	static char *InlineFigParents[3];
	static char *BlockFigParents[3];

	static char *ImageMapFigParents[3];
	static char *SingleFigParent[2];
	static char *ImageMapParents[3];
	static char **DITAImageParents;
	static short PixelWidth;
	static short PixelHeight;
	static long MapNumber;
	static long MapAreaNumber;
	static long MapCalloutDiv;
	static long PreCalloutDiv;
	static DClist MapCalloutDivRefList;
	static long ImageRefNum;
	static bool RetainRuninImagesForEmptyParagraphs;

  static void WriteFrame(HTMwfile *wf, frame *fr, bool refpage = false);
  static void CorrectGraphFilePath(char **gfile);
  static char *FixGrFileName(char *name);
  static void WriteImageMap(HTMwfile *wf, frame *fr, char *grbase);
  static void WriteArea(DCfile *dwf, void *ap);

  static void ProcImageMapStart(void);
  static void ProcImageMapProp(HTMctl *cp);
  static void ProcImageMapEnd(void);
  static void ProcImageMapAreaStart(void);
  static void ProcImageMapAreaProp(HTMctl *cp);
  static void ProcImageMapAreaShape(HTMctl *cp);
  static void ProcImageMapAreaEnd(void);

	static void ProcGrTextFlowEnd(HTMctl *cp);
	static void ProcGrTextHyperLink(HTMctl *cp);
	static void ProcGrTextHyperObj(HTMctl *cp, bool page = false);
	static void ProcGrTextHyperTarget(HTMctl *cp);
	static void ProcGrTextHyperURL(HTMctl *cp);
	static void ProcGrTextHyperAlert(HTMctl *cp);
	static void ProcGrTextHyperFile(HTMctl *cp);


#ifdef UNUSED
  static char *FrameHRef[FrameHRMax];
  static char *FrameVRef[FrameVRMax];
  static char *FrameVAlgn[FrameVAMax];
  static void WriteFrameStart(HTMwfile *wf, frame *fr);
  static void WriteFrameCont(HTMwfile *wf, frame *fr);
#endif

  static bool InGraphic;
  static bool InImpObj;

#ifdef UNUSED
  static graph *Graphic;
  static uns GrGroupStack[GrGroupMax];
  static uns GrGroupLevel;
  static uns GrFillMap[GrFillMax];

  static void ProcGraphic(HTMctl *cp);
  static void EndGraphic(HTMctl *cp);
  static void ProcGrLine(HTMctl *cp);
  static void ProcGrPen(HTMctl *cp);
  static void ProcGrFill(HTMctl *cp);
  static void ProcGrShape(HTMctl *cp);
  static void ProcGrPoly(HTMctl *cp);
  static void ProcGrRect(HTMctl *cp, uns rtyp);
  static void ProcGrEllipse(HTMctl *cp);
  static void ProcGrArc(HTMctl *cp);

  static uns BezierPts;
  static uns BezPtSpace;
  static long BezDistMax;
  static void ProcGrBezier(HTMctl *cp);
  static void ProcBez(long *bezpt);
  static bool BezFlat(long *bezpt);

  static gtext *GText;
  static void ProcGrText(HTMctl *cp);
  static uns GrFWeight[GrFWeightMax];
  static void ProcGrTProp(HTMctl *cp);
  static void ProcGrTStr(HTMctl *cp);

  static void WriteGraphic(HTMwfile *wf, graph *gr);
  static void WriteGrText(HTMwfile *wf, gtext *gt);
  static void WriteWMFStart(HTMwfile *wf, frame *fr);
  static void WriteWMFEnd(HTMwfile *wf, frame *fr);

  static wmf_pen DefaultPen;
  static wmf_pen CurrPen;
  static wmf_pen LastPen;
  static void WriteWMFPen(HTMwfile *wf, wmf_pen *pen);
  static wmf_brush DefaultBrush;
  static wmf_brush CurrBrush;
  static wmf_brush LastBrush;
  static void WriteWMFBrush(HTMwfile *wf, wmf_brush *br);
  static long ShadeColor(long colr, uns shd);
  static wmf_font DefaultFont;
  static wmf_font CurrFont;
  static wmf_font LastFont;
  static void WriteWMFFont(HTMwfile *wf, wmf_font *fnt);
  static void WriteWMFSel(HTMwfile *wf, uns num);
  static void WriteWMFDel(HTMwfile *wf, uns num);
#endif

  static bool InObject;
  static char *ObjectDesc;

  static void StartObject(HTMwfile *wf, DCctl *cp);
  static void SetObjectIntProp(HTMwfile *wf, DCctl *cp);
  static void SetObjectTextProp(HTMwfile *wf, DCctl *cp);
  static void StartObjectParam(HTMwfile *wf, DCctl *cp);
  static void EndObjectParam(HTMwfile *wf, DCctl *cp);
  static void SetObjectParamName(HTMwfile *wf, char *chp);
  static void SetObjectParamVal(HTMwfile *wf, char *chp);
  static void SetObjectParamType(HTMwfile *wf, unl typ);
  static void PutObject(HTMwfile *wf, DCctl *cp);



  // in dwhtmrf.cc

#ifdef UNUSED
  static bool ProcRf(HTMctl *cp);

  static rast *Raster;
  static void ProcGrRaster(HTMctl *cp);
  static void ProcGrColormap(HTMctl *cp);
  static void ProcGrBitmap(HTMctl *cp);
  static void ProcGrSunRaster(HTMctl *cp);

  static void WriteGrRaster(HTMwfile *wf, rast *rs);
#endif


 public:

  // in dwhtm.cc, called from main()
	static swlist SwitchList[];
	static bool FormatSet;
	static bool HelpForm;
	static char HelpType;
	static bool OracleHelp;
	static bool OmniHelp;
	static bool ListFile;
	static char ListType;
	static char *DRmifVer;
	static long MIFVer;
  static bool UseIndex;
	static char *ICUdll;

  static short Verbose;
	static uns SourceType;
	static bool XMLSource;
	static bool UMXL;
	static bool UsePartFiles;
	static bool FMSource;
	static DCirfile *IniFile;
	static DCfrfile *FmtFile;
	static DCfrfile *PagesFile;
	static DCfrfile *TablesFile;
	static DCfrfile *SubFormatsFile;
	static char *OutputFormat;
	static char *IniFileName;
	static char OldFormType;
	static bool DoAutomation;
	static bool EndingAuto;
	static bool BookAuto;
	static DCLrfile *DCLFile;
	static HTMwfile *HTMFile;

  static void SetFormat(char *fmt);
  static bool InitCurrProps(char *wfname);
  static void ReadDCL(DCLrfile *rf);
  static bool WriteHTM(HTMwfile *wf);
	static void ProcListFile(DCrfile *rf, char *destname);
};


#endif

