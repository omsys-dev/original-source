
// dwrtf.h is the header for dcl Microsoft RTF writing, JHG
// Copyright (c) 1993-1998 by Omni Systems, Inc.  All rights reserved.

#ifndef DWRTF_H
#define DWRTF_H


class RTFwr;
class RTFwfile;
class RTFctl;
class RTFitem;
class RTFlink;
class RTFgroup;


enum WrtfConstants {
  RowAlgnMax = 3, TabTypeMax = 3, CharSMax = 4, ParAlgnsMax = 4,
  FrameHRMax = 3, FrameHAMax = 6, FrameVRMax = 3, FrameVAMax = 5, 
  UnitBasesMax = 4, PgnFormsMax = 5, FrLangMax = 17, SavedStateMax = 10,
	HelpKeysMax = 38, HelpArgsMax = 14, HelpOptionalArgs = 3,
	MarkerPropsMax = 12,
  CellFillMax = 12, CellFillEmpty = 13, CellFillWhite = 14, CellCountMax = 36,
  ColorTableMax = 256, ColorMapMax = 257, RTFColorMax = 255, Invisible = 256,
  GrFWeightMax = 7,	GrArrStyleMax = 11,	HULevMax = 12, BmStrMax = 6,
  FTypesMax = 7, FNamesMax = 17, FNamesXtra = 4, FDataMax = 25,
	UnicodeFDataMax = 12,
  LevelMax = 128, FillMapMax = 16, WMFObjCodesMax = 14,
  FrameStackMax = 128, GrStyleMax = 16, GrFillMax = 16, GrGroupMax = 128,
	WmfFNameMax = 32,
  StringMax = 80, CharMetricMax = 128, LineMax = 128, NumStrMax = 12,
	HelpCloneBufMax = 256, BrowseDigitsMax = 10,
	CntHeadMax = 256, CntTopicMax = 256,
  MinorMax = 256, RtfListMax = 1024,
  RefBufLen = 1024, RomanMax = 1024, MaxIniLen = 65500,
  CellMargMax = 30000, TextStoreBufMax = 16000
};

enum modtype { // where processing happens
  none = 0, omit,
  top, tx, an, tb, gr, rfm,
  modlast
};

enum scantype {  // what to do during scan phase
  nothing = 0, pass,
  unitset, drname, srcver,                // in dwrtf.cc
	dirapp, dirsec, dirprop, dirval,
  dbgscan,
  docdefs, docres, txfile,                // in dwrtftx.cc
  fontlist, language, charset,
	flowlist, flowframe, flowprev, flowname, flowused,
	ukeyset, ukeyeset, ukeyout, ukeylib, ukeyid, ukeyiref, ukeyitm, ukeyeitm, ukeykeys, ukeysrc,
  formdef, formend, formname, formtype, formtag, formspc,
	fmpxdef, fmpxend, fmsxdef, fmsxend, fmtxbeg, fmtxend, anumscan,
	setrunin, endrunin, sideflow, sidepos, sidewid, sidegap,
	formchk, formscan, vardef, varname, varform, varend,
	supsiz, supoff, subsiz, suboff, scapsiz,
	xrfmdef, xrfname, xrform, xrftok, xrfval,
	getxref, numxrfm, usexrfm, setxloc, gtxfile, setmark,
	conddef, condedef, condnam, condsind, condeind, condvis, 
	condcol, condbcol, condccol,
	condcch, condsimg, condsalt, condeimg, condealt, 
  condset, condres, conddis, condrset,
  ftnstart, ftnend, ftnform,
  anumdef, anumtype, endanum, anumid, anumstrm, anumlvl,
  anumset, anumtxt, anumfmt, anumefmt, anumrfmt, anumform, anumdtok,
	idxchk, idxchke, varnum,
  ruleprop, tblstart, tblend, tbcstart, endtbc, // in dwrtftb.cc
  tbcstrd, tformck,
	pgdef, pgend, pgattr, pgsize,           // in dwrtfgr.cc
  frdef, fredef, franch, frattr, frsize, frline, frpen, frfill,
  frname, frprev, frtext, setanch, frpost,
  grline, grpen, grfill, grshape, grarrhd, grarrtl,
  grgroup, gregroup, grobj, greobj, grtext,
	eqtype, eqsize, eqalgn, eqtxsiz, eqlbrk, eqtext, eqgfile,
  rasprop, rasbmap, rascmap, epsprop, epshead,
	colordef, arrdef, grfile,
  scanlast
};

enum rtftype {  // what to do during write phase
  ignore = 0, skip,
	exapp, exsec, exprop, exval,
  dbgwrite,
  puttx, gpstart, gpend, frnum, flnum, putfile,  // in dwrtftx.cc
  putfont, putfsize, putcomp, putbold, putital,
  putbase, putcaps, putstyl, putrevb, putline, putlmod, putkern,
  putspan, putside, putrun, putrunin, eptrunin,
  putlspc, putpspc, putalgn, putindt,
  putlang, putdown, putup, putright, putleft, putcset, putcolr,
  parstart, parend, colend, pagend, linend, putbreak, putkeep, putkeepn,
  puttdef, endtdef, puttab, tabfill, tabfstr,
	fmpxset, fmpxres, fmsxset, fmsxres,	frmval,
  formcall, ftaguse, formsave, formrest, ftagspc,
  putvar,	putvtok, putanum, putatok, putdtok,
	putftn, endftn, setftn,
  putidx, idxgrp, idxend, idxtyp, idxfld, idxprp,
	putxrtok, makxref, putxref, setxref, endxref,
	sethref, setfile, markuse,
	xrfform, xrfcont, xrfecont,
	hypopt, hyploc, hypunq, hypfile, hyplnk, hypalt, hyppgn, hypobj,
  setsubj,
  formtbl,                     // in dwrtftb.cc
  puttbl, endtbl, tbplace, tbeplac, tbldflt, tbedflt,
	tbatype, tbapern, tbapera, tbacolr, tbafill,
  tblrult, tblrulb, tblrull, tblrulr,
  tbcol, tbcend, tbcwid, tbcwids, tbcwtyp,
	tbccolr, tbcfill, tbcrull, tbcrulr,
  tbrow, tbrend, tbrhigh, tbrhfix, tbrhmin, tbrhmax, tbrcolr, tbrfill,
	conduse, condend, condruse,
  tbrrult, tbrrulb, tbrruls, tbrrper, tbrralt,
  tbcell, tbclend, tbclstrd, tbclrstr, tbclrotr, tbclcolr, tbclfill,
  tbclrult, tbclrulb, tbclrull, tbclrulr,
  tbclmart, tbclmarb, tbclmarl, tbclmarr,
  tbclmrtt, tbclmrtb, tbclmrtl, tbclmrtr,
  tbclvalg, tbtplace, tbtgap, tbltitl, tbetitl,
  lncolr, lnthck, putfr,       // in dwrtfgr.cc
  rtflast
};

struct rtf {
  dc_dtyp ctldt;
  uns ctlmaj;
  uns ctlmin;
  uns ctldat;
  scantype scan_op;
  modtype module;
  rtftype write_op;
};


class RTFctl : public DCctl {
 public:
  rtf *rp;
  RTFctl() { set(ilong); rp = NULL; }
  RTFctl(dc_dtyp dtp, uns mj = 0, uns mn = 0, uns d = 0, unl s = 0)
    { dtype = dtp; major = (unc) mj; minor = (unc) mn; data = d; size = s; ext = NULL;
      rp = NULL; }
  RTFctl(RTFctl& c)
    { dtype = c.dtype; major = c.major; minor = c.minor; data = c.data;
      size = c.ext ? 0 : c.size; ext = NULL; rp = NULL; }
  RTFctl(DCLrfile& rf) { *this = rf; }

  virtual void write(DCfile& wf);
  RTFctl& operator= (DCLrfile& rf)
   { return (RTFctl &) rf.getCtl(*this); }
  RTFctl& operator= (DCblock& b)
    { if (ext) delete [] ext; ext = b.p(); size = b.l(); b.clear();
      return (RTFctl&) *this; }
};


class RTFitem {
 public:
  RTFctl *ctl;
  RTFitem *next;

  RTFitem() : ctl(NULL), next(NULL) { }
  RTFitem(RTFctl *cp) : ctl(cp), next(NULL) { }
  RTFitem(RTFctl *cp, RTFitem *n) : ctl(cp), next(n) { }
  virtual ~RTFitem() { if (next) delete next; delete ctl; }
#if 0
  virtual void insert(RTFctl *nc, RTFctl *oc)
    { if (!next) return;
      if (next->ctl == oc) next = new RTFitem(nc, next);
      else next->insert(nc, oc); }
  virtual void replace(RTFctl *nc, RTFctl *oc)
    { if (!next) return;
      if (next->ctl == oc) next->ctl = nc;
      else next->replace(nc, oc); }
#endif
};


class RTFlink {
 public:
  RTFitem *first, *last;
  RTFlink() : first(NULL), last(NULL) { }
  virtual ~RTFlink() { if (first) delete first; }
};


class RTFgroup : public RTFctl, public RTFlink {
 public:
  RTFgroup() : RTFctl(group), RTFlink() { }
  RTFgroup(RTFctl& c) : RTFctl(c), RTFlink() { }
  virtual ~RTFgroup() { }
  void add(RTFctl& c)
    { if (!first) last = first = new RTFitem(&c);
      else last = (last->next = new RTFitem(&c)); }
  void add(RTFgroup& g)
    { if (!first) last = first = new RTFitem(&g);
      else last = (last->next = new RTFitem(&g)); }
  void merge(RTFgroup& g)
    { if (&g == NULL) return;
      if (!first) first = g.first; else last->next = g.first;
      if (g.last) last = g.last;
      g.first = g.last = NULL; delete &g; }
  void prune(void)
    { if (first) delete first; first = NULL; }

#if 0
  virtual void insert(RTFctl& nc, RTFctl& oc)
    { if (!first) return;
      else if (first->ctl == &oc) first = new RTFitem(&nc, first);
      else first->RTFitem::insert(&nc, &oc); }
  virtual void replace(RTFctl& nc, RTFctl& oc)
    { if (!first) return;
      else if (first->ctl == &oc) first->ctl = &nc;
      else first->RTFitem::replace(&nc, &oc); }
  virtual void replace(RTFgroup& gc, RTFctl& oc)
    { if (!first) return;
      else if (first->ctl == &oc) first->ctl = &gc;
      else first->RTFitem::replace(&gc, &oc); }
#endif

  virtual void insert(RTFctl& nc, RTFctl& oc);
  virtual void append(RTFctl& nc, RTFctl& oc);
  virtual void replace(RTFctl& nc, RTFctl& oc);

  virtual void write(DCfile& wf)
    { this->RTFctl::write(wf); RTFitem *itm = first;
      while (itm) { if (itm->ctl) itm->ctl->write(wf); itm = itm->next; } }
  virtual void execu(void (*e)(RTFctl *c))
    { (*e)((RTFctl *)this); RTFitem *itm = first;
      while (itm) { if (itm->ctl) (*e)(itm->ctl); itm = itm->next; } }
	RTFctl *getnext(RTFitem** it);
};


enum unittype {
 pg_unit, ld_unit, ft_unit, gl_unit, UnitsMax
};


struct finfo {
  char *fname;
  uns fnum;
  uns ftype;
	uns fwid;
	uns fenc;
	unl cpg;
};


struct tinfo {
  short tpos;
  unc ttype;
  unc tlead;
};

struct pprop {
  char *stag;
  uns stype;     // if stype is 1 (para),
	char *cstag;   //  this is the current char format (from its stag) or NULL
  uns snum;
  uns snext;
	unl cpg;
	uns cfnum;
  uns fnum;
  uns ftype;
	uns fwid;
	uns fenc;
  uns fsize;
  unc bold;
  unc ital;
  unc base;
  unc caps;
  unc scaps;
  unc outl;
  unc shad;
  unc strike;
  unc line;
  unc lmode;
  unc algn;
  unc keep;
  unc keepn;
  unc lsmode;
	unc help;
	unc hkey;
	unc hvis;
	unc htvis;
	unc fnote;
	unc hide;
	unc clone;
	unc slide;
	unc hyper;
	unc cntform;
	unc cntlev;
	unc ding;
	unc anumpos;
	unc sideh;
	unc runin;
	unc hscroll;
	unc xscroll;
	bool del;
	bool config;
	uns paralink;
	bool mref;
	bool pref;
	bool nref;
  short kern;
  short lspace;
  short aspace;
  short bspace;
  short lind;
  short find;
  short rind;
  short pbreak;
  short pxbreak;
  short lang;
	char *section;
	bool sectstart;
  uns colr;
  uns bcolr;
	shdinfo *shading;
  uns cellmargl;
  uns cellmargr;
  uns cellmargt;
  uns cellmargb;
  unc cellmartl;
  unc cellmartr;
  unc cellmartt;
  unc cellmartb;
 	unc cellvalgn;
	uns tcount;
	uns pxframe;
	uns sxframe;
	brdrinfo *brdrt;
	brdrinfo *brdrb;
	brdrinfo *brdrl;
	brdrinfo *brdrr;
	brdrinfo *brdrbar;
	brdrinfo *brdrbox;
	brdrinfo *brdrbtw;
	char *runpunct;
	RTFgroup *rungroup;
	char *hsuf;
	char *href;
	char *htsuf;
	char *infot;
	char *hbrowse;
	char *hbuild;
	char *hmacro;
	char *hwindow;
	char *hjfile;
	char *replace;
	char *maccb;
	char *maccs;
	char *macce;
	char *macca;
	char *maccr;
	char *macacb;
	char *macaca;
	char *cstore;
	char *tstore;
	char *styadd;
	//pprop *cform;
  DCvlist *tabs;
};

struct shprop {
	short shwid;
	short shgap;
	unc shpos;
	bool shflip;
};

struct tbl {
	char *tblfname;  // format name
	char *tblid;     // FDK id name
	char *tbgroup;   // group name
	uns tblncnt;     // count of names used for ini (2 or 3)
	bool tblstrip;
  uns tbccount;
  uns tbrcount;
  uns tbhcount;
  uns tbbcount;
  uns tbfcount;
  uns tblalgn;
  short tbllind;
  short tblbspc;
  short tblaspc;
  short tblbrk;
  uns tblkeep;
  //uns tblrl[2];
  //uns tblrr[2];
  //uns tblrt[2];
  //uns tblrb[2];
	//uns tblrra[2];
  //uns tblcrl[2];
  //uns tblcrr[2];
	brdrinfo *brdrl;
	brdrinfo *brdrr;
	brdrinfo *brdrt;
	brdrinfo *brdrb;
	brdrinfo *brdra;
	brdrinfo *brdrcl;
	brdrinfo *brdrcr;
	uns tblrrper;
	uns tblshtyp;
	uns tblshper;
	uns tblaltper;
  uns tblaltfill;
  uns tblaltcolr;
	shdinfo *tblaltshd;
  uns tblbdyfill;
  uns tblbdycolr;
	shdinfo *tblbdyshd;
  uns tblmargl;
  uns tblmargr;
  uns tblmargt;
  uns tblmargb;
  uns titleplace;
  uns titlegap;
	long tblhlp;
  RTFgroup *titlecont;
  DCvlist *collist;
  DCvlist *rowlist;
  DCvlist *rowtlist;
	tbfmtvals* fv;
	DCvlist *hrfmts;
	long hrcount;
	DCvlist *brfmts;
	long brcount;
	DCvlist *frfmts;
	long frcount;
};

struct col {
  uns colwid;
  uns colfill;
  uns colcolr;
	shdinfo *colshd;
  //uns colrl[2];
  //uns colrr[2];
	brdrinfo *brdrl;
	brdrinfo *brdrr;
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
  //uns rowrt[2];
  //uns rowrb[2];
  //uns rowrx[2];
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
};

struct cell {
  short rpos;      // distance of right edge of cell from start of row
  uns algn;        // cell content alignment, 0..3
  bool straddling;
  bool straddled;
	bool laststrad;
	uns vstrad;
  uns cellmargl;
  uns cellmargr;
  uns cellmargt;
  uns cellmargb;
  unc cellmartl;
  unc cellmartr;
  unc cellmartt;
  unc cellmartb;
	unc cellvalgn;
  uns cellrotr;
  uns cellfill;    // fill or shading, 0 = none, 1..12 = pattern, else %*100
  uns cellcolr;
	shdinfo *cellshd;
  uns cellrl[2];
  uns cellrr[2];
  uns cellrt[2];
  uns cellrb[2];
	brdrinfo *brdrl;
	brdrinfo *brdrr;
	brdrinfo *brdrt;
	brdrinfo *brdrb;
	bool cellrlov;
	bool cellrrov;
	bool cellrtov;
	bool cellrbov;
  RTFgroup *cont;
	cellfmtvals* fv;
};

struct page {
  short pnum;
  uns ptype;
  uns pusage;

	bool margmirror;
	uns pgnform;
	uns pgnstart;
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
	short pcgap;
	bool linebetcol;
  short gutter;
	uns margl;
	uns margr;
	uns margt;
	uns margb;
	brdrinfo *brdrl;
	brdrinfo *brdrr;
	brdrinfo *brdrt;
	brdrinfo *brdrb;
	bool brwraphd;
	bool brwrapft;
	bool titlepg;
	long titlespa;
	long headery;
	long footery;
	hfinfo *hdb;
	hfinfo *ftb;
	hfinfo *hdf;
	hfinfo *ftf;
	hfinfo *hdl;
	hfinfo *ftl;
	hfinfo *hdr;
	hfinfo *ftr;
};

struct wmf_pen {
  uns pstyle;
  uns pwidth;
  unl pcolor;
  uns pshade;
	uns lstyle;
};

struct wmf_brush {
  uns bstyle;
  uns bhatch;
  unl bcolor;
  uns bshade;
};

struct wmf_font {
  char fname[WmfFNameMax];
  uns fcolr;
  short fhigh;
  short bhigh;
	short bshift;
	short lhigh;
	short fangle;
  uns fnum;
	uns fwid;
  uns fweight;
	uns fenc;
	unl cpg;
  unc fital;
  unc fundl;
  unc fstrik;
  unc fpitch;
  unc fntype;
	unc fbase;
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
  short xoff;
  short yoff;
  short xpos;
  short ypos;
  short wide;
  short high;
  short xspace;
	short cols;
	short colgap;
  uns rang;
  uns vcount;
  short *vertx;
};

struct frame {
  uns fpos;
  uns frul[2];
  uns frulsp;
  uns ffill;
  uns href;
  uns vref;
  uns halgn;
  uns valgn;
	uns fwrap;
  short xpos;
  short ypos;
	short pxpos;
	short pypos;
  short wide;
  short high;
  short xspace;
	short stretch;
	short cols;
	short colgap;
  uns ftext;
	uns frot;
  uns grcount;
	short dpi;
	short clip;
	uns eqtype;
	uns prtype;
	long id;
	char *grfile;
	char *ngrfile;
  wmf_pen *frpen;
  wmf_brush *frbrush;
  DCvlist *grlist;
};

struct graph {
  uns gtype;
	uns gypos;
	uns grhead;
	uns grtail;
  unl gsize;
  short *gparm;
  wmf_pen *gpen;
  wmf_brush *gbrush;
  wmf_font *gfont;
};

struct grarc {
	short cx;
	short cy;
	short lenx;
	short leny;
	double hang;
	double tang;
};

struct gtext {
  unc gform;
	unc sideh;
  short axpos;
  short aypos;
  short hpos;
  short vpos;
  short hinc;
  short vinc;
	short fwide;
	short fhigh;
  short twide;
  short thigh;
	short find;
	short lind;
	uns lsmode;
	short lspace;
	short bspace;
  uns tcount;
	uns acount;
  char *tptr;
};

struct arrow {
	short alen;
	short afill;
	short atip;
	short abase;
};

struct rast {
  short iwide;
  short ihigh;
  short ixpos;
  short iypos;
  long pwide;
  long phigh;
  short plcrop;
  short prcrop;
  short ptcrop;
  short pbcrop;
  short pdeep;
  short comp;
  short pbpl;
  short cmap;
	short dpi;
  unc *pcolor;
  DCblock *pbits;
};

struct wmeta {
	uns mtype;
  short iwide;
  short ihigh;
  short ixpos;
  short iypos;
  DCblock *mrecs;
};


struct condinfo {
	long cond;
	char *name;
	char *chchs;
	char *simage;
	char *salt;
	char *eimage;
	char *ealt;
	uns colr;
	uns bcolr;
	uns chcolr;
  unc bold;
  unc ital;
	unc line;
	bool hidden;
};



class RTFwr {

  friend class RTFctl;
  friend class RTFgroup;

  // in dwrtf.cc

  static void WriteHPJFile(void);

  static rtf RTFpile[];
  static size_t RTFsize;
  static DCvlist *RTFlist[RtfListMax];

  static RTFgroup BaseGroup;
  static RTFgroup *GroupStack[LevelMax];
  static uns DCLlevel;
  static bool SkipCurrGroup;
  static uns SkipLevel;

  static void SetUpRTF(void);
  static void FindRTF(RTFctl *cp);

  static bool ProcTop(RTFctl *cp);

	static char *DirAppName;
	static bool DirLocal;
	static char *DirSecName;
	static short DirSecNum;
	static char *DirSecNames[];
	static char *DirPropName;
	static short DirPropNum;
	static char **DirPropNames[];
	static char *DirDefaultsProps[];
	static char *DirOptionsProps[];
	static char *DirGraphicsProps[];
	static char *DirTableProps[];
	static char *DirHelpBrowseProps[];
	static char *DirValName;
	static long DirValNum;
 	static bool DirValBool;

	static void SetDirApplication(RTFctl *cp);
	static void SetDirSection(RTFctl *cp);
	static void SetDirProperty(RTFctl *cp);
	static void SetDirValue(RTFctl *cp);

	static bool FlowWritten;

  static pprop SavedCurrStack[SavedStateMax];
  static pprop SavedLastStack[SavedStateMax];
  static pprop SavedParaStack[SavedStateMax];
  static uns SavedLevel;
  static void SaveState(void);
	enum StateType { CurrState, LastState, FullState };
  static void RestoreState(StateType stype);

  static uns UnitMul[UnitsMax];
  static uns UnitDiv[UnitsMax];
  static uns UBaseMul[UnitsMax];
  static uns UBaseDiv[UnitsMax];
  static void SetUnits(RTFctl *cp);
  static short GetTwips(long val, unittype ut);
  static long GetOrig(long val, unittype ut);


	// in dwrtfin.cpp
	static secinfo RTFIniSects[];

	static setinfo ISecAuto[];
	static setinfo ISecUdocOptions[];
	static setinfo ISecInitOptions[];
	static void CaselessMatchSet(char *);
	static void SpacelessMatchSet(char *);
	static void WildcardMatchSet(char *);

	static setinfo ISecHelpOptions[];
	static void HyperHelpSet(char *);

	static setinfo ISecWordOptions[];
	static void Word8Set(char *);
	static void RevProtSet(char *);
	static void ForceSideHeadPosSet(char *);

	static setinfo ISecOptions[];
	static void SmallCapsSet(char *);
	static void RMarginTabsSet(char *);

	static setinfo ISecRefFrames[];
	static void RefFramesSet(char *);
	static void RefFrameFormatSet(char *);

	static setinfo ISecDefaults[];
	static void FontSizeSet(char *);
	static void DefLangSet(char *);

	static setinfo ISecFDK[];
	static void IDFileNameSet(char *);
	static void XrefFileSuffixSet(char *);

	static setinfo ISecTable[];
	static void TableFillSet(char *);
	static void TblFullWidthSet(char *);

	static setinfo ISecGraphics[];
	static void TextScaleSet(char *);

	static setinfo ISecGraphOpts[];

	static setinfo ISecIndex[];

	static setinfo ISecHelpBrowse[];
	static void BrowseDigitsSet(char *);

	static setinfo ISecHelpContents[];

	static setinfo ISecCommonStyles[];
	static void ContentsSet(char *);
	static void CntStylesSet(char *karg);
	static void ReplaceSet(char *);
	static void ReplacementsSet(char *karg);
	static void MacBeforeSet(char *);
	static void MacStartSet(char *);
	static void MacEndSet(char *);
	static void MacAfterSet(char *);
	static void MacReplaceSet(char *);
	static void StyleCodeReplaceSet(char *);
	static void MacAnumBeforeSet(char *);
	static void MacAnumAfterSet(char *);
	static void CodeStoreSet(char *);
	static void TextStoreSet(char *);

	static setinfo ISecWordStyles[];

	static setinfo ISecHelpStyles[];
	static void JumpHotSet(char *);
	static void PopHotSet(char *);
	static void HelpRefStylesSet(char *);
	static void BrowseSet(char *);
	static void SpKeySet(char *);
	static void HelpKeywordStylesSet(char *karg);
	static void FileSet(char *);
	static void WindowSet(char *);
	static void MacroHotSet(char *);
	static void MacroSet(char *);
	static void BuildSet(char *);
	static void SuffixSet(char *);
	static void TitleSufSet(char *);
	static void InfoTypeSet(char *);
	static void ContextFnSet(char *);
	static void PopContentSet(char *);
	static void TopicSet(char *);
	static void ULineSet(char *);
	static void SlideSet(char *);
	static void ResumeSet(char *);
	static void KeysSet(char *);
	static void PopOverSet(char *);
	static void ParaLinkSet(char *);
	static void JumpOverSet(char *);

	static setinfo ISecXrefStyles[];
	static setinfo ISecWInserts[];
	static setinfo ISecHInserts[];

	static setinfo ILogOptions[];


	// in dwrtfmc.cpp

	static char *TopInsert;
	static char *BottomInsert;
	static char *HeaderInsert;
	static char *FooterInsert;
	static char *FirstHeaderInsert;
	static char *FirstFooterInsert;
	static char *LeftHeaderInsert;
	static char *LeftFooterInsert;
	static char *RightHeaderInsert;
	static char *RightFooterInsert;

	static char *TopicStartInsert;
	static char *TopicEndInsert;
	static char *SlideStartInsert;
	static char *SlideEndInsert;
	static char *CurrPrjName;
	static char *CurrPrjPath;

	static bool WriteMacroChar(DCwfile *df, unc uch);
	static bool WritePredefinedMacro(DCwfile *df, char *name);
	static void WriteStyleref(RTFwfile *wf, char *style);
	static void WriteStyle(RTFwfile *wf, char *stylename);
	static void WriteStylenum(RTFwfile *wf, char *style);
	static void WriteFontnum(RTFwfile *wf, char *font);
	static void WriteColornum(RTFwfile *wf, char *color);

	static char *LastPageRefBookmark;
	static void WritePageRef(RTFwfile *wf);

	static char *GetPredefinedVar(DCwfile *df, char *vp);
	static char *GetFrameUserVar(char *vp);

	static DCnlist CodeStoreParaList;
	static DCnlist CodeStoreDivList;
	static char *CodeStoreVar;
	static long CodeStoreDiv;
	static long PreCodeStoreDiv;

	static bool WriteCodeStoreVar(DCwfile *df, char *mn);
	static char *GetCodeStoreVar(DCwfile *df, char *vp);
	static void StartCodeStore(RTFwfile *wf);
	static void EndCodeStore(RTFwfile *wf);

  // in dwrtftx.cc

  static bool ProcTx(RTFctl *cp);

  static pprop Curr;
  static pprop Last;
  static pprop DefForm;
  static pprop ParaForm;
  static pprop CForm;
  static pprop LastCF;
  static pprop NoForm;
  static pprop FtnCurr;
  static pprop FtnLast;
  static pprop FtnLastCF;

  static bool CaselessMatch;
  static bool SpacelessMatch;
  static bool WildcardMatch;

  static char OutType;
  static char HelpType;
  static char QuoType;
  static char BulType;
  static bool NoDing;
  static char SmallCaps;
  static char IdxType;
	static short KeywordLimit;
  static char DisambiguateIndex;
  static char XrfType;
	static bool FieldHyper;
	static bool WrapXrefs;
	static bool ExtXrefPages;
	static bool WordPerf;
	static bool Word8;
	static bool Word2000;
	static bool Word2002;
	static bool Word2003;
	static bool WordVerSet;
	static char *IDFileName;
	static char *PrjFileName;
	static bool UseFrameIX;
	static bool UseLocalFileID;
  static char HUType;
  static bool UseHL;
  static bool UseGreen;
	static char FootType;
	static char *FtnSeparator;
	static bool EndFtnWithSpace;
	static char HFType;
	static char *SectionPrefix;
	static bool UseTextFrames;
	static bool WrapTFs;
	static bool WrapHTFs;
	static short HFGap;
	static char SideHType;
	static short SHVAdjust;
	static char PSpcType;
	static char NegLead;
	static bool KeepAnchors;
	static char PageType;
	static char PBMethod;
	static char EmpFrType;
	static char RunInFrames;
	static bool EmbedEqs;
	static short EqVert;
	static short EqHor;
	static char OccTabType;
	static char RMarginTabs;
	static bool LTabRMarg;
	static bool RTabRMarg;
	static bool UnderlineTabs;
	static char TbTitleType;
	static char TbRuleType;
	static bool TbRuleAdjust;
	static char TbGraphics;
	static bool StripTables;
	static bool StrippedCellPar;
	static bool TbFixed;
	static bool ForceTbLineBreaks;
	static bool FrameTbTitle;
	static bool ForcedBreak;
	static bool ShiftWideTablesLeft;
	static bool HideWhite;
	static bool TextColor;
	static bool BackColor;
	static bool AllowZeroFSize;
	static char *RefFrameFormat;
	static char *RefFrameDefFormat;
	static char RefFrames;
	static char *DefFontName;
	static long DefFontSize;
	static uns DefFontWidth;
	static uns DefLineSpace;
	static uns DefTabWidth;
	static bool ExtXRefs;

	static bool XML;
	static char *Template;
	static bool TemplateAutoUpdate;
	static uns TextRescale;
	static uns TableRescale;
	static uns GraphicRescale;

	static bool HelpForm;
	static char HlpVer;
	static char *HPJName;
	static bool WriteHelpProjectFile;
	static char *HelpCopyright;
	static bool HelpCopyDate;
	static bool HlpVerSet;
	static bool Altura;
	static bool HyperHelp;
	static bool ForceBmc;
	static bool HelpSectBreaks;
	static bool BareFnSym;
	static char FtnSpace;
	static bool HelpLineBreak;
	static bool HelpTitlePending;
	static bool HelpTitleScroll;
	static bool ExtendHelpNoScroll;
	static bool NonscrollZone;
	static bool HelpTitleSpace;
	static bool HelpTitleIndent;
	static uns HelpTabLimit;
	static bool NoXRPopup;
	static bool ULineOver;
	static char *FrameStyle;

	static bool TitleBrowse;
	static bool AutoBrowse;
	static bool BrowseStart;
	static int  BrowseCurr;
	static int  BrowseStep;
	static int  BrowseDigits;
	static char *BrowsePrefix;
	static char *CurrBrowseID;

	static char CntType;
	static char *CntBase;
	static char *CntTitle;
	static char *CntTopic;
	static char *CntTopHead;
	static char *CntName;
	static char *CntStartFile;

	static char BmcStr[BmStrMax];
	static char BmlStr[BmStrMax];
	static char BmrStr[BmStrMax];
	static char *BulletStr;
	static bool TransparentBitmaps;

  static char GrTxType;
	static bool ClipText;
  static char ClipType;
  static short ClipLimit;
	static bool UseGrPreviews;
  static char GrPaths;
  static char GrNames;
	static bool RemoveGraphics;
	static bool RemoveNontextFrames;
	static bool FrameBorders;
	static short GrVert;
	static bool GrTbSpace;
  static char MetaType;
	static bool MetaMetric;
	static short PicScale;
	static int MetaNameWid;
	static int MetaNumWid;
  static char DashLType;
	static bool RectIsPoly;
  static short GrBkMode;
  static short GrStretchMode;
  static char *GrDefFont;
  static short GrDefFSize;
  static short GrTxScale;
  static short GrTxWidth;
  static short GrTxVAdj;
  static short BitmapDPI;
  static short EqExDPI;
  static short EqExpand;
  static short Ras24Mode;
	static bool CompRasters;
	static char RasBorders;
	static char FrBorders;
	static bool BitmapFlip;
	static bool UseTopSpaceAbove;
	static bool NoBlankFirstGTLine;
	static bool NoGTULine;
	static bool FrExHigh;
	static bool WrapAllFrames;
	static bool EmbedBMPs;
	static bool EmbedWMFs;
	static bool NameWMFsAsBMPs;
	static bool NameGraphics;

	static char *CustCode;
  static bool CharStyles;
  static bool RepStyles;
  static bool RemStyles;
  static bool RemFonts;
  static bool UseLang;

	static bool UseLog;
	static bool ShowLog;
	static char *LogIniPrefix;
	static char *LogFileName;
	static char *EditorFileName;
	static DClog *Log;
	static unc LogLevels[logmax];
	static bool LogIniChains;
	static void LogEvent(logtype lt, unc sev, char *desc, char *d2 = NULL,
                     char *d3 = NULL, char *d4 = NULL, char *d5 = NULL);
	static bool MergeCurrFormatProps;
	static bool FormatBaseSet;

	static void SetNamePath(const char *pathname, char **oname, char **opath);

  static void SetUpCnt(void);
  static void SetDefForm(void);

  static DCvlist FormList;
  static DCnlist FormNameList;
  static DCnlist FormRenameList;
  static DCnlist FormNewNameList;
  static DCilist FormRefList;
  static DCilist FormTypeList;
  static DCvlist FormTabList;
  static DCvlist CharFormStack;
  static uns FormNum;
  static uns DCLFormNum;
  static uns FormsStored;
  static uns CharFormLevel;
  static char *FormName;
  static char *FinalFormName;
  static bool InFormDef;
  static bool InFormTagDef;
  static bool InFormPx;
  static bool InFormSx;
  static bool InFormText;

  static bool CharStylesUsed;
  static bool StartCharForm;
  static bool InCharForm;
  static bool EndCharForm;
  static bool CharFormSaved;
  static bool CharFormsNest;
  static bool StartFtnCharForm;
  static bool InFtnCharForm;
  static bool EndFtnCharForm;
  static bool FtnCharFormSaved;

  static void ProcForm(RTFctl *cp);
  static void ProcFormType(RTFctl *cp);
  static void ProcFormName(RTFctl *cp);
  static void StoreForm(void);

	static char *HelpStyleElems[HelpKeysMax];
	static char *HelpStyleSects[HelpArgsMax];
	static void ProcHelpStyle(void);
	static void ProcHelpKey(char *kp, bool first);
  static void ProcHelpCntForm(char *hform = NULL);
  static void ProcHelpForm(void);

  static void TagForm(RTFctl *cp);
  static void UseForm(RTFctl *cp);

	static DClist FormsUsed;
	static DCilist FinalForms;
	static long FinalFormNum;
	static DClist FormsReplaced;
	static DCnlist RepFormName;

  static void MarkFormUsed(RTFctl *cp);
  static void SetFinalForms(void);
  static uns GetFinalForm(uns snum);

  static void CheckHelpForm(RTFctl *cp);
  static void CheckSideHeads(RTFctl *cp);
  static void CheckFormat(RTFctl *cp);
  static void UseFTag(RTFctl *cp);
  static void UseSpecFmtTag(RTFctl *cp);
  static void InitForm(pprop *cform);
  static void RestoreInheritForm(void);

	static bool InheritPending;
	static pprop InheritPendForm;
  static void InheritForm(pprop *cform);
  static void UseInheritForm(pprop *cform);
  static void RestoreInheritParaForm(pprop *cform);

  static uns CurrCharSet;
  static uns DefCharSet;
  static char *CharSet;
  static char *CharSNames[CharSMax];
  static uns CharSNums[CharSMax];
  static void SetCharSet(void);

  static uns Fr2MsLang[FrLangMax];
  static uns Fr2Cpg[FrLangMax];
  static char *DefLang;
  static short DefaultLanguage;
  static unl DefaultCodePage;
  static bool SpaceAfterUnicode;
  static void ProcLang(RTFctl *cp);

  static char *ParAlgns[ParAlgnsMax];
  static bool InTabDef;
  static bool TabsChanged;
  static uns TabCount;
  static uns TabFillChar;
  static unc TabAlign;
  static unc TabLine;
  static tinfo *CurrTab;
  static DCvlist *TabList;
	static bool InRunGroupDef;
  static void SetCurrProps(RTFctl *cp);

  static uns FontNum;
  static uns DefFont;
  static DCvlist FontList;
  static DCnlist FontNameList;
  static DCilist FontWidthList;
  static char *FontTypes[FTypesMax];
  static uns FTypes[FNamesMax + FNamesXtra];
  static char *FontNames[FNamesMax + FNamesXtra];
  static uns FCharWids[FNamesMax];
  static bool FixedWid;
  static uns CharMetrics[CharMetricMax];
	static uns SuperSize;
	static uns SubSize;
	static uns SCapSize;
	static long SuperShift;
	static long SubShift;
	static long DefShift;
	static char *LastShiftDir;

	static DClist FontsUsed;
	static DCilist FinalFonts;
	static long FinalFontNum;
	static finfo FontData[FDataMax];
	static finfo UnicodeFontData[UnicodeFDataMax];

  static void MarkFontUsed(uns fnum);
  static void SetFinalFonts(void);
  static uns GetFinalFont(uns fnum);
  static void SetDefaultFont(void);
  static void SetUnicodeFonts(void);
  static void SetUnicodeFont(uns ufnum);
	static unl GetCodePageFromEncoding(uns enc);
	static uns GetEncodingFromCodePage(unl cpg);

  static void ProcFont(RTFctl *cp);
  static char *GetFontName(RTFctl *cp);
  static uns GetFontSize(RTFctl *cp);
  static void SetKern(RTFctl *cp);

  static DCvlist FlowList;
  static DCilist SectList;
  static DClist SectUsedList;
  static DClist SectSkipList;
  static long SectNum;
  static DClist FrameSkipList;
  static uns FlowFrameNum;
  static RTFctl *FlowFrameCtl;
  static bool InDocBody;
  static bool WritingStyles;
  static bool InPara;
  static bool InParaGroup;
  static unc PGStarted;
  static bool ParaStarting;
  static bool ParaEnding;
  static bool FtnParaEnding;
	static char *FtnEndMacro;
	static long PreFtnParaRefNum;
	static short ParaSpace;
	static short BodyParaSpace;
  static RTFgroup *FlowGroup;
	static uns CurrFlow;
  static RTFgroup *NormalFlowGroup;
	static uns NormalFlow;
	static DClist SideHFlowList;
  static void ProcFlow(RTFctl *cp);
  static void ProcFlowName(RTFctl *cp);
  static void MarkSectUsed(RTFctl *cp);

  static bool RevTrack;
  static bool RevProt;
  static void WriteDocProps(RTFwfile *wf);

  static void WriteFonts(RTFwfile *wf);
  static void WriteStyles(RTFwfile *wf);
  static char *TmpStyleName(char *name);

  static bool BordersShadingDone;
  static void WriteBorders(RTFwfile *wf, pprop *style);
  static void WriteOneBorder(RTFwfile *wf, brdrinfo *br,
	                           char *cw = NULL, bool hlp = false);
  static void WriteShading(RTFwfile *wf, shdinfo *sh);
  static void WriteCharStyles(RTFwfile *wf, pprop *style);
  static void WriteParaStyles(RTFwfile *wf, pprop *style);

	static DCvlist SideHTFlowList;
	static DCvlist SideHFrameList;
	static DCilist SideHSpaceList;
  static short SideHWide;
  static short SideHGap;
  static short SHWidth;
  static short SHGap;
  static unc SideHPos;
  static bool SideHFlip;
  static bool ForceSideHeadPos;
  static unc FSideHPos;
  static bool FSideHFlip;
  static bool SideHUsed;
  static bool SideHAnchor;
  static bool SHSpannerAnchors;
	static bool SideHPropsInFlow;
  static short SideHRind;
  static short SideHLind;
  static short TextRind;
  static short TextLind;
  static short TextLMarg;
  static short TextRMarg;
	static bool FirstTop;
  static short FirstHBottom;

	static bool SingleSect;
	static RTFgroup *TextFlow;
	static RTFgroup *SkippedFlow;
	static bool SectStarting;
	static bool KeepSectBreaks;
	static short HeaderYPos;
	static short FooterYPos;
	static char *SectBreakType;
	static char *SectBreakTypes[9];

  static void WriteFlow(RTFwfile *wf, uns flownum);

	static DCilist SectBreakList;
	static short CurrCols;
	static void WriteSectBreak(RTFwfile *wf, uns first);
	static void PutSectPageProps(RTFwfile *wf, uns first);

	static char *IndexStyle;
	static char *IndexLeader;
	static DCnlist IndexStyles;
	static npair IndexStylesInit[];
	static DCnlist IndexLeaders;
	static npair IndexLeadersInit[];
	static uns IndexColumns;
	static char CurrIdxLetter;
	static bool UseIndexLetters;
	static bool UseIndexTopLetters;
	static bool UseListProps;
	static bool UseIXLets;
	static bool UseIXTopLets;
	static char *IndexLetterSymbol;
	static char *IndexLetterNumber;
	static char *IndexLetPrefix;
	static char *IndexTopLettersForm;
	static char *IndexLettersForm;

	static void InsertIndexTOCItem(RTFwfile *wf, RTFctl *cp);
	static void SetIXListProps(RTFctl *cp);
	static void PutIndexField(RTFwfile *wf, RTFctl *cp);
	static void SetIndexTopLetters(RTFwfile *wf);
	static void SetIndexLetter(RTFwfile *wf, char ch);
	static void PutIXBookmark(RTFwfile *wf, char ch);
	static void PutIndexForm(RTFwfile *wf, char *fname);

	static bool WriteMissingForms;
	static bool WriteAllVarForms;
	static DCilist MissingFormList;
	static long MissingFormNum;
	static DClist FormsFound;
	static void AddMissingForm(long snum);
	static void AddFormsFound(RTFctl *cp);
  static void WriteMissingFormParas(RTFwfile *wf);
  static void WriteHiddenPara(DCwfile *wf, long fnum, long item);

  static void WriteAllHFFmts(RTFwfile *wf, page *mpp);
  static void WriteFmtHF(RTFwfile *wf, char *cw, hfinfo *hf, bool head = false);
  static void DefineRequiredFormats(DCfrfile *fmts);
  static uns DefineFormat(char *fname, uns ftype);

	static bool FrameTop;
	static bool HFDone;
	static bool FrameHF;
	static bool WritingHF;
	static bool PosHFText;
	static bool WritingHFGraph;
	static char *HFHRef;
	static short HFHPos;
	static short HFWide;
	static short HFVPos;
  static short HFBottom;
	static short HFVAdjust;
  static void WriteHF(RTFwfile *wf, char *chp, uns mpg, bool head, bool right = true);
  static void WriteHFText(DCfile *wf, void *vp);
  static void WriteHFGraphic(DCfile *wf, void *vp);
  static void WriteHFGrFrame(DCfile *wf, frame *gfr);
  static void GetGraphicBounds(graph *gr, frame *gfr);
  static void SetGraphicPos(graph *gr, short xp, short yp);

	static bool FirstFlowFrame;
	static unl FlowFramePage;
	static DClist PageStartList;
  static frame *CurrTextFrame;
	static bool FrameEndPara;
	static bool EndingTextFrame;

	static void CheckTextFrame(RTFwfile *wf, RTFctl *cp);

	static unl ExtFileStartLine;
	static unl ExtFileEndLine;
	static unl ExtFileStartChar;
	static unl ExtFileEndChar;
	static unl ExtCodeFileLen;
	static char *ExtCodeFileEnc;

	static void WriteFromExtFile(RTFwfile *wf, RTFctl *cp);
	static void WriteFlowNum(RTFwfile *wf, RTFctl *cp);
	static void SetSideHProps(RTFwfile *wf, shprop *shp, page *fp = NULL, bool sec = false);
	static void ResetSideHProps(void);

	static bool EndUL;
	static bool DiscHyphen;
	static bool HardHyphen;
	static bool HardRetSpace;

	static void WriteText(RTFwfile *wf, RTFctl *cp = NULL);
	static bool InUnicode;
	static bool InAnsi;

#if 0
	static void PutUText(RTFwfile *wf, unc ch);

	static unc CyrLatinSet[64];
	static uns CyrLowSet[16];
	static uns CyrHighSet[16];
	static unc GreekLatinSet[64];
	static unc EasternEuroLatinSet[128];
	static uns EasternEuroSet[128];
	static uns EncodingVals[5];
	static char *EncodingSuffixes[5];
	static unl EncodingCpgs[5];

	static uns ProcUnicode(RTFwfile *wf, uns wch);
	static uns GetUnicodeEquiv(uns wch, uns enc);
#endif

  static void CheckDing(RTFwfile *wf, uns& wch);
  static void PutBullet(RTFwfile *wf);

  static RTFctl *LastLineEnd;
  static DClist CancelLineEnd;
  static void ProcLineEnd(RTFctl *cp);
  static void EndLine(RTFwfile *wf, RTFctl *cp);
  static void EndColumn(RTFwfile *wf, RTFctl *cp);
  static void EndPage(RTFwfile *wf, RTFctl *cp);
  static void AddChar(unc ch);

	static bool PageBroke;
	static char RuninType;
	static long RuninDiv;
	static long PreRuninDiv;
	static long RuninPara;
	static DClist NoRuninPara;
	static pprop RuninProps;
	static DCvlist RuninOverrides;
	static uns RuninLinePos;
	static tinfo *RuninTab;
	static short TabDiff;

  static void WriteProps(RTFwfile *wf);
  static void WriteRunin(RTFwfile *wf, bool cont);
	static uns MergeTabs(DCvlist **ctab, DCvlist **rtab);
  static void WriteOutTabs(RTFwfile *wf);
	static void SetPardProps(RTFwfile *wf = NULL);

	static bool UseParaAnchors;
  static void CheckAnchorPara(RTFwfile *wf);
//static void WriteCharProps(RTFwfile *wf);
  static void WriteCharForm(RTFwfile *wf);
  static void WriteFontProps(RTFwfile *wf);
  static void WriteSimpleCharProps(RTFwfile *wf);
  static void WriteParaProps(RTFwfile *wf, frame *fr = NULL);
  static void WriteParaFrameProps(RTFwfile *wf);

	static bool ExactLSUsed;
	static bool ExactLSpace;
	static short CurrLineSpace;
	static short LastLineSpace;
	static short NegLineSpace;
  static void CalcLineSpace(void);
  static void PutLineSpace(RTFwfile *wf);
  static void PutParaSpace(RTFwfile *wf);
  static void PutParaIndent(RTFwfile *wf, frame *fr);

  static uns LinePos;
  static uns CharWid;
  static char *TabTypes[TabTypeMax];
	static bool SuppressSpace;
	static bool FirstTabSet;
	static bool TrailingTabs;

  static void WriteTabDefs(RTFwfile *wf, pprop *style);
  static void WriteTabs(RTFwfile *wf);

	static bool PageBreakPending;
  static void WriteBreak(RTFwfile *wf);

  static bool InPxSxFrame;
	static DCnlist ExtMetaList;
  static void WritePreSufFrame(RTFwfile* wf, uns framenum, bool start);

  static bool StoreText;
	static bool TextStoreHere;
	static bool ConfigHere;
	static char *TextStoreName;
	static unc TextStoreBuf[TextStoreBufMax];
	static long TextStoreBufCnt;
	static long TextStoreCount;
	static DCnlist TextStoreNames;
	static DCnlist TextStoreVals;

	static void PutTextStoreBuf(unc ch);
	static void PutTextStoreCS(unc ch);
	static void PutTextStoreHex(unc ch);
	static void PutTextStoreSpec(unc ch);
	static void ProcTextStoreText(RTFctl *cp);
	static char *GetTextStoreBuf(bool cfg = false);
	static char *GetTextStore(char *vp);

	static RTFctl *ParaStartCtl;
  static RTFctl *TxtParaStartCtl;
  static RTFctl *TbCellParaStartCtl;
  static RTFctl *PreFtnParaStartCtl;
  static RTFctl *PrevStartCtl;
	static bool PrevParaIsTb;
  static long ItemRefNum;
  static long ParaRefNum;
  static long TxtParaRefNum;
  static long PreFtnTxtParaRefNum;
  static long JustPara;
  static long JustLineEndCount;
	static uns JustParaEndSpace;
  static DCilist JustLineEndList;

  static DCvlist PRefFrameList;
	static DClist FrPendList;
	static DClist TbCellParaEndList;

  static void ProcParaStart(RTFctl *cp);
  static void ReAnchorFrame(void *vp);
  static void ProcFrAnchor(RTFctl *cp);
  static void ExecFrame(DCfile *wf, void *vp);

  static DCilist HelpTitleParaList;
  static bool InHelpTitlePara;
  static bool StartingHelpTopic;
  static bool AnchorPara;
	static long CurrSlider;
	static long PreSlideDiv;
	static long PHtype;
	static bool WhiteText;
	static bool NonWhiteText;
	static bool DeletingText;
	static bool TopicEmpty;
	static bool LastTopicEmpty;
	static bool ParaStartFrames;
	static bool ParaStartFrPend;
	static bool ParaPropsWritten;
	static bool ParaFrameProps;
	static bool PStartSeen;

	static pprop AnchorProp;

  static void StartPara(RTFwfile *wf, RTFctl *cp);
  static void WriteParaStartFrames(RTFwfile *wf);
  static void SetParaHelp(RTFwfile *wf);
	static void WriteTopicEnding(RTFwfile *wf);
  static void ProcParaEnd(RTFctl *cp);
  static void EndPara(RTFwfile *wf);
  static void WriteParaEndCode(RTFwfile *wf);
  static void WriteFramesAndTables(RTFwfile *wf);
  static void WriteAnchorPara(RTFwfile *wf, short lsize, bool atend = true);


	// in dwrtffm.cpp

  static fmtvals *GetFormatFileInfo(char *fmtname);
  static uns GetFontNum(char *fname, pprop *form = NULL);

  static tbfmtvals *DefTable;
  static rowfmtvals *DefRow;
  static cellfmtvals *DefCell;
  static bool DefTblSettings;
  static void GetDefTblFormats(void);
  static tbfmtvals *GetTblFormatFileInfo(char *tbfmtname);
  static rowfmtvals *GetRowFormatFileInfo(char *rowfmtname, long rnum = 0);
  static cellfmtvals *GetCellFormatFileInfo(char *cellfmtname,
                                            long rnum = 0, long cnum = 0);
  static char *CurrSection;
  static DCnlist SectionStart;
  static bool StartingSection;
  static DCvlist SectProps;
  static DCnlist SectNames;
  static long SectCount;
  static page *DocFormatProps;
  static void GetPageFormatFileInfo(void);
  static page *GetSectFormatFileInfo(char *sectfmtname);


  // in dwrtfan.cpp

  static bool ProcAn(RTFctl *cp);

  static DCnlist MarkList;
  static DCnlist MarkTypeList;
	static long MarkTypes;

  static void ProcMarker(RTFctl *cp);
	static char *MarkerProps[MarkerPropsMax];
	static char *MarkName;
	static DCilist TextMarkerList;

	static void ProcMarkerType(RTFctl *cp, char *kp, char *txt);
	static void ProcMergeMarker(char *txt);

	static void WriteMarker(RTFwfile *wf, RTFctl *cp);
  static void AddMarkerCode(char *name, char **ptxt);

  static bool HyperJump;
  static bool PopOver;
  static uns HyperVal;
  static long HotDiv;
  static long PreHotDiv;
  static bool HyperStart;
  static bool HyperEnd;
  static bool LockHyper;
  static uns HyperPara;
  static DCilist HyperParaList;

  static bool HyperForm;
  static bool HyperFormEnding;
  static long HyperFormStartID;
  static long HyperFormEndID;
  static DCilist HyperFormStartList;
  static DClist HyperFormEndList;

  static void SetHyperStart(uns val);
  static void SetHyperEnd(void);
  static void CheckHyper(RTFctl* cp);
  static void CheckHyperEnd(RTFctl* cp);

  static bool UseHyperColor;
  static bool HyperColor;
  static bool HyperColorEnding;
  static bool ParaHyperColor;
  static long HyperColorStartID;
  static long HyperColorEndID;
  static DCilist HyperColorStartList;
  static DClist HyperColorEndList;

  static void ProcHyperColor(RTFctl *cp);
  static void ProcHyperColorStart(RTFctl *cp);
  static void ProcHyperColorEnd(RTFctl *cp);
  static void ProcTextColor(RTFctl *cp);

	static bool InVarDef;
	static bool InVar;
	static uns CurrVar;
	static DCvlist* VarList;
	static DCnlist VarNameList;
	static uns TbContVar;
	static bool TbContUse;
	static char *TbContVarName;
	static uns TbSheetVar;
	static bool TbSheetUse;
	static char *TbSheetVarName;
	static bool InDateToken;
	static long PreDateDiv;
	static long DateDiv;
	static uns ParaVarType;
  static void SetVarForm(RTFctl *cp);
  static void CheckVarName(RTFctl *cp);
  static void ProcTextStoreVarText(RTFctl *cp);
  static void ProcTextStoreVar(uns vnum, bool macvar = true);
  static void PutVarForm(RTFwfile* wf, RTFctl *cp);
  static void PutVarToken(RTFwfile* wf, RTFctl *cp);
	static char *GetVarStyle(char *sname);
	static char *SetNumVal(uns ntyp, uns nval);

  static bool InPgnAnum;
  static bool InChapNum;
  static bool InVolNum;
  static bool InSectionNum;
  static bool InSSectNum;
	static char *ChapNum;
	static char *VolNum;
	static char *SectionNum;
	static char *SSectNum;
	static long CurrPgn;
  static uns PgnStart;
  static uns PgnForm;
  static uns ChapNumType;
  static uns ChapNumVal;
  static uns VolNumType;
  static uns VolNumVal;
  static uns SectionNumType;
  static uns SectionNumVal;
  static uns SSectNumType;
  static uns SSectNumVal;
  static char *PgnForms[PgnFormsMax];
  static void SetPgnProps(RTFctl *cp);
  static void SetCurrPgn(RTFctl *cp);

  static DCvlist AnumDefList;
  static DCvlist AnumValList;
  static bool InAnumDef;
  static bool InAnumStreamDef;
  static uns AnumDefID;
  static DCnlist AnumNameList;
  static DCilist AnumLimitList;
  static DCilist AnumSetList;
  static bool AnumPending;
  static bool WriteAnums;
  static bool SeqAnums;
  static bool AnumMergeformat;
  static bool WritingAnum;
  static bool WritingAnumVal;
  static bool WhiteAnumVal;
  static bool AnumUsedCS;
  static uns AnumTokenCount;
  static uns AnumTokenLimit;
  static char *AnumTokenName;
  static char *CurrAnumValStr;
	static size_t CurrAnumSepLen;
  static uns CurrAnumToken;
  static uns CurrAnumLevel;
  static uns CurrAnumChange;
  static uns CurrAnumSetLevel;
  static uns CurrAnumVal;
  static bool InTextPx;

  static void ProcAnumDef(RTFctl* cp);
  static void ProcAnumVal(RTFctl* cp);
  static void PutAnumVal(RTFwfile *wf, bool atstart);
  static void WriteAnumBkmks(RTFwfile *wf, RTFgroup *avp);
  static size_t GetNonTextLength(char *ivp);
  static void WriteAnumToken(RTFwfile *wf, RTFctl *cp);
  static void WriteAnumField(RTFwfile *wf, uns lev, uns typ=0,
	       uns aform=0, bool keep=false, bool hide=false,
	       bool zero=false, bool set=false, uns val=0);
  static void PutAnumTokVal(RTFwfile *wf, uns aform);
  static void WriteChapVolFields(RTFwfile *wf);

//  static void EndAnumXR(RTFwfile *wf, RTFctl* cp, char *ncp);

	static char *XrefFile;
	static char *XrefRawFile;
	static char *XrefWindow;
	static char XrefHvis;
	static bool XrefMapped;
	static char *RefStr;
	static DCnlist RefStrList;
	static short RefLevel;
	static bool IdxEnded;
	static long HyperUnique[HULevMax];
	static short HULevel;
	static short AlertNum;
	static char *IXnewlinkPrefix;

	static char *CurrFileName;
	static char *CurrFilePath;
	static char *CurrFileFull;
	static char *CurrPath;
	static char *GlobalXrefFileName;
	static DCnlist XRFNames;
	static DCnlist XRFNewNames;
	static long XRFCount;

	static bool UseXrefFilePrefix;
	static char *CurrFilePrefix;
	static char *XrefFilePrefix;
	static char *XrefFileSuffix;
	static short XrefLenLimit;
	static DCnlist FIDNames;
	static DCnlist FIDPrefixes;
	static long FIDCount;

	static char *StripSuf(char *name);
	static void SetFIDLists(void);
	static char *GetFIDPrefix(char *fname);
	static void SetXRFLists(char *secname);
	static char *GetXrefName(char *fname);

	static void ProcHyperMark(RTFctl *cp);
	static void ProcHyperLink(uns val = 3);
	static void ProcHyperObj(RTFctl *cp);

	static bool MakeRef;
	static long CurrRef;
	static DCilist MakeRefList;

	static void ProcHyperUnique(RTFctl *cp);
	static void PutHypertext(RTFwfile *wf, RTFctl *cp);
	static void PutHyperLoc(RTFwfile *wf, RTFctl *cp);
	static void PutHyperAlt(RTFwfile *wf, RTFctl *cp);
	static void PutHyperUnique(RTFwfile *wf, RTFctl *cp);
	static void WriteHyperUnique(RTFwfile *wf);
	static void PutHyperFile(RTFwfile *wf, RTFctl *cp);
	static void PutHyperLnk(RTFwfile *wf, RTFctl *cp);
	static void PutHyperObj(RTFwfile *wf, RTFctl *cp);
	static void PrepRefStr(long num);
	static void PutHyperSubject(RTFwfile *wf, RTFctl *cp);
	static void PutRefStr(RTFwfile *wf);
	static void PutWordHyper(RTFwfile *wf);
	static void PutHyperFieldStart(RTFwfile *wf, char *ref,
	                               bool pop = true);
	static void PutHyperFieldEnd(RTFwfile *wf);

	static char *XrefName;
	static DCnlist XrefNameList;
	static long XrefID;
	static long XrefUsage;
	static DCilist XrefUseList;
	static long XrefInstID;
	static char *XrefInstStr;
	static DCilist XrefIDUseList;
	static anyini XrefIni;
	static bool XrefTextOnly;
	static bool XrefDelete;
	static void ProcXrefFormatName(RTFctl *cp);
	static void ProcXrefFormatUse(long id);

	static short CurrXRForm;
	static char *CurrXRStr;
	static char *CurrXRBkmkStr;
	static long XRValNum;
	static bool WritingXRVal;
	static bool XRValSpace;
	static bool LockXrefs;

	static DCvlist XRFormList;
	static DCvlist XRValList;
	static DCnlist XRStrList;
	static DCnlist XRLocList;

	static DCilist XRVFormList;
	static DCnlist XRVStrList;
	static DCnlist XRVFileList;

	static DClist XRFPnList;
	static DClist XRFPsList;
	static DClist XRFPtList;

	static unc XRNused;
	static unc XRSused;
	static unc XRTused;

	static DCnlist XRNStrList;
	static DCnlist XRSStrList;
	static DCnlist XRTStrList;
	static DCnlist XRNEndStrList;
	static DCnlist XRSEndStrList;
	static DCnlist XRTEndStrList;

	static bool LogBookmarkCount;
	static long LastBkmkCount;
	static long BkmkstartCount;
	static long BkmkendCount;
	static long BkmkErrCount;

	static void SetXRForm(RTFctl *cp);
	static bool SetXRToken(RTFctl *cp);
	static void SetXRVal(RTFctl *cp);
	static char *GetXRStr(RTFctl *cp, bool jump = false);
	static void SetXRRef(void);
	static void SetXRSrc(void);
	static bool NoFreakingBookmarks;
	static void SetXRBookmarks(void);
	static void XRStartAnumNum(RTFwfile *wf);
	static void XREndAnumNum(RTFwfile *wf);
	static void XRStartAnumStr(RTFwfile *wf);
	static void XREndAnumStr(RTFwfile *wf);
	static void XRStartPara(RTFwfile *wf);
	static void XREndPara(RTFwfile *wf);
	static void ResetXRBookmarks(void);
	static void PutBkmk(DCwfile *w, char *bkstr, long val);
	static void WriteXRVal(RTFwfile *wf, RTFctl *cp);
	static void WriteXRCont(RTFwfile *wf, RTFctl *cp, bool start);
	static void WriteXRToken(RTFwfile *wf, RTFctl *cp);
	static void WriteRefFieldStart(RTFwfile *wf, char *rstr,
	 bool pg = false, bool extr = false, char *fmt = NULL,
	 bool lock = false);
	static void WriteRefFieldEnd(RTFwfile *wf);

  static void SetXrefFileName(char *name, size_t len = 0);

	static bool InHelpXref;
	static bool XrefMarker;
	static void StartAnchor(RTFwfile *wf, RTFctl *cp, bool href);
	static void SetRefStr(char *chp);
	static void WriteRefStrs(RTFwfile *wf);
	static void WriteRefStrListItem(DCwfile *pf, char *ref, long rnum);
	static void PutWordBookmarks(RTFwfile *wf, char *ref);
	static void ScanBookmark(char *chp);
	static bool EndingAnchor;
	static void EndAnchor(RTFwfile *wf);

  static uns FtnStart;
  static uns FtnForm;
  static uns FtnMarker;
  static uns FtnPosition;
  static bool InFootnote;
  static bool InFtnAnum;
  static bool InTbFtnAnum;
	static RTFgroup *FtnFormGroup;
	static RTFgroup *FtnRFormGroup;
	static RTFgroup *TbFtnFormGroup;
	static RTFgroup *TbFtnRFormGroup;
  static bool WritingFtnGroup;

  static bool StartingFtnCont;
	static long FootDiv;
	static short FootnoteNum;
	static short TxFootNum;
	static short TbFootNum;

  static void WriteFtnProps(RTFwfile *wf);
  static void WriteFtnGroup(RTFwfile *wf, bool inftn);
  static void WriteFtnAnumToken(RTFwfile *wf, RTFctl *cp);
  static void WriteTextFtnStart(RTFwfile *wf);
  static void WriteFtnStart(RTFwfile *wf);
  static void WriteTextFtnNum(RTFwfile *wf);
  static void WriteFtnNum(RTFwfile *wf);
  static void WriteTextFtnEnd(RTFwfile *wf);
  static void WriteFtnEnd(RTFwfile *wf);

  static bool IndexEntry;
  static bool InIndexGroup;
	static bool IndexCounted;
  static bool StartingIndexGroup;
  static bool KeepIXMarkerIDs;
  static short IndexCount;
  static char *IndexBuf;
  static short IndexPos;
  static short IndexSPos;
  static short IndexAllocUnit;
  static short IndexAlloc;
  static bool IdxColon;
  static short IndexColon;
  static bool InIdxSort;
  static bool NoIdxSort;
  static bool IdxSeeAlso;
  static bool NoSeeAlso;
	static DClist SeeAlsoList;
  static bool IdxStartRange;
	static DClist StartRangeList;
  static bool IdxEndRange;
	static DClist EndRangeList;
  static bool IndexRanges;
  static bool InIdxGroup;
  static bool BookmarkIXRanges;
	static void ProcIdxType(RTFctl *cp);
  static void PutIndexEntry(RTFctl *cp);
  static void ProcIdxFlags(long ent);
	static unc IdxBold;
	static unc IdxItal;
	static unc IdxLine;
	static unc LastIdxBold;
	static unc LastIdxItal;
	static unc LastIdxLine;
  static void ResetIdxCharForm(void);
  static void ProcIdxCharForm(pprop *cf);
	static char *WriteIdxCharForm(void);

	static DCnlist TopIdxList;
	static long TopIdxCount;
	static long IdxDiv;
  static void WriteIndexEntry(RTFwfile *wf);
	static char *PutIXBook(RTFwfile *wf, char *iptr, bool start);
	static void PutHelpKeyword(RTFwfile *wf, char *iptr);

	static char HelpCloneBuf[HelpCloneBufMax];
	static short HelpCloneCount;
	static bool InitFileFtn;
	static long HelpCloneDiv;
	static long CurrentDiv;
	static void StartHelpClone(RTFwfile *wf);
	static void EndHelpClone(RTFwfile *wf, char *dvch);
	static void PutHelpCloneBuf(RTFwfile *wf, uns ch);
	static void WriteHelpClone(RTFwfile *wf, pprop *fmt);
	static void WriteCntHead(unc level);
	static long CntRefNum;
	static char *CntRefStr;
	static char *CntBTextStr;
	static void WriteCntTopic(unc level, char *px = NULL, char *cfile = NULL, 
   char *wind = NULL, char *suf = NULL, bool cntref = false, bool bt = false);

	static char CntHeadStr[CntHeadMax];
	static char CntTopicStr[CntTopicMax];
	static char *CntTopicRefStr;
	static unc CntLastLevel;
	static unc CntLastOrigLevel;
	static unc CntPrevOrigLevel;
	static unc CntLastForm;
	static char *CurrHelpFile;
	static char *CurrHelpBase;
	static bool AddCntFileName;
	static bool AddCntWindowName;
	static char *CntMainWindow;

	static void WriteOutCntEntry(unc& newlevel);
	static void WriteOutCntHead(void);
	static void WriteOutCntTopic(void);
	static void WriteOutCntMerged(unc level = 0);

	static void WriteHelpFtn(RTFwfile *wf, unc fntype, char *suf = NULL);
	static void WriteExHelpFtn(RTFwfile *wf, unc fntype, char *str);
	static void WriteContextFtn(RTFwfile *wf, char *prefix = NULL, char *suf = NULL);
	static void PutContext(RTFwfile *wf, char *txt, bool min = false);
	static void PutFtnContext(RTFwfile *wf, char *txt);
	static void WriteBrowseID(char *hbstr);
	static void WriteBrowseFtn(RTFwfile *wf);
	static void ProcHelpBreak(RTFwfile *wf);
	static void WriteFileFtn(RTFwfile *wf);

	// in dwrtfpp.cpp

	static char RefBuf[RefBufLen];

	static char *GetFileID(char *fn);
	static char *BaseName(char *fn, size_t len = 0);
	static bool IncAlpha(char *str);
	static bool FileIDUsed(char *id);

  // in dwrtftb.cc

	static RTFctl *TableStartCtl;
  static bool ProcTb(RTFctl *cp);
  static void ProcTblProp(RTFctl *cp);

  static DCvlist TblFormList;
  static DCnlist TblFormNameList;
  static DCvlist RowFormList;
  static DCnlist RowFormNameList;
  static DCvlist CellFormList;
  static DCnlist CellFormNameList;
	static long RowFormNum;
	static long CellFormNum;
  static tbl *TblForm;
  static bool InTblDef;
  static bool TbColWidPercent;
  static col *ColForm;
  static uns TbCol;
  static long DefColWidth;
  static void ProcTblForm(void);
  static void StoreTblForm(void);

  static void UseTblForm(RTFctl *cp);

  static DCvlist *TblList;
  static bool TblListStarted;
	static uns TblCount;
  static tbl *Tbl;
  static tbl CTbl;
	static anyini TblIni;
  static bool InTbl;
  static bool InTbTitle;
	static bool TblTitleParaStarted;
	static bool TblTitleTextAdded;
	static bool TblTitleParaEnded;
  static bool InTDefaults;
	static bool StripTb;
	static DClist StripTableList;

  static col *Col;
  static uns ColNum;
  static uns LastColwid;
  static bool TitleInRow;
  static bool InTopTitleRow;
  static bool InTblTitleRow;
  static uns TitleRowAlign;
  static short TitleRowLind;
  static long TitleRowEnd;

  static void ProcTbl(RTFwfile *wf, RTFctl *cp);
  static void ProcTblCol(RTFctl *cp);
	static short TblColWidPct;
	static short TblColWidAdd;
	static unl TblFullWidth;
	static bool TblFullWidthWasSet;
	static uns AdjColWid(unl cwid);

  static void WriteTbl(RTFwfile *wf);
  static void PutTbSpacer(RTFwfile *wf, short spc);
  static void PutTbTitleSpacer(RTFwfile *wf, short spc);
  static void WriteTbTitle(RTFwfile *wf);
  static void StartTitleRow(RTFwfile *wf, bool top);
  static void EndTitleRow(RTFwfile *wf);

  static row *Row;
  static row *RowForm;
  static row *LastRow;
  static row *NextRow;
  static uns TbRowType;
  static uns RowNum;
  static uns BodyRowNum;
 	static long RowCpID;
  static char *RowAlgns[RowAlgnMax];
  static void ProcRow(RTFctl *cp);
  static void ProcRowEnd(void);
  static void WriteTbRow(RTFwfile *wf);

  static cell *Cell;
  static bool InTbCell;
  static bool InTblCell;
  static bool StartingTbCell;
  static bool EndingTbCell;
  static char *CellFillPats[CellFillMax];
  static char *CellBordStyles[];
  static uns CellStradCount;
  static uns *CellRowStradCount;
  static uns *CellRowColStrad;
  static cell **CellRowStradCell;
  static bool InStradTbCell;
  static bool FirstStradTbCell;
  static bool LastStradTbCell;
  static bool MergeStradCells;
  static bool RowStrad;
  static bool CellStradAdjust;
  static short StradRpos;
  static short LastRpos;
  static char TableFill;
  static bool TblFillColor;
  static bool TblFillShade;

  static void SetCellMarg(RTFctl *cp);
  static void ProcCell(RTFctl *cp);
  static void ProcCellEnd(void);
  static void ProcCellStrad(RTFctl *cp);
  static void ProcCellRowStrad(RTFctl *cp);
  static void StartCellText(RTFctl *cp);
  static void AddCellCont(RTFctl *cp);
  static void WriteCellProps(RTFwfile *wf);
	static void SetCellBorder(void);
  static void WriteCellBorder(RTFwfile *wf);
  static void WriteRule(RTFwfile *wf, uns *rul);
  static void WriteHelpCellBorder(RTFwfile *wf);
  //static void WriteHelpRule(RTFwfile *wf, uns *rul);
  static void WriteCell(RTFwfile *wf);


  static DCvlist RuleFormList;
  static DCnlist RuleNameList;
  //static uns Rule[2];
  //static rule DCLrule;
  static brdrinfo *RuleDef;
  static brdrinfo NoBrdr;
  static bool InRuleDef;
  static void ProcRuleForm(void);
  static void ProcRuleProp(RTFctl *cp);
  static void StoreRuleForm(void);
  static brdrinfo *GetTableFileBorder(char *bfmt);
  static void GetRule(RTFctl *cp, brdrinfo **rptr);

  static uns FillMap[FillMapMax];
  static void GetFill(RTFctl *cp, uns *fptr);
  static void GetShading(RTFctl *cp, shdinfo **fptr);
  static void GetShadeColor(RTFctl *cp, shdinfo **fptr);
  static void WriteCellShadingFmt(RTFwfile *wf);
  static void WriteCellShading(RTFwfile *wf);

  static uns ColorsUsed;
  static uns LastColor;
  static uns ColorMap[ColorMapMax];
  static unc RedTable[ColorTableMax];
  static unc GreenTable[ColorTableMax];
  static unc BlueTable[ColorTableMax];
  static void GetColor(unl cpsiz, uns *cptr);
  static void WriteColorTable(RTFwfile *wf);
	static unl CheckColorNum(uns cnum);
	static uns CheckColorValue(unl cval, uns def = 2);
	static uns ProcColorDef(unl cval, uns cnum = 0);


	// in dwrtfcn.cpp

  static long CurrCond;
  static DCnlist CondNames;
  static void StartCondDef(RTFctl *cp);
  static void SetCondName(RTFctl *cp);

  static bool InCondInds;
  static DCvlist CondInfos;
	static condinfo *CurrCondInfo;
	static condinfo *ConflictCondInfo;

	static void SetCondInd(RTFctl *cp);
	static void SetCondTypo(RTFctl *cp);
	static void SetCondColor(RTFctl *cp);

	static DClist CondsStarting;
	static DClist CondsEnding;
	static DClist CondsInEffect;
	static bool CondStartPend;
	static bool CondEndPend;
	static bool CondsEffective;
	static long CondFlagDiv;
	static long PreCondFlagDiv;

	static void SetCond(RTFctl *cp);
	static void EndCond(RTFctl *cp);

	static condinfo *PendCondInfo;
	static condinfo *NonCondInfo;
	static DCnlist EndFlags;
	static DCnlist EndAlts;
	static DCnlist StartFlags;
	static DCnlist StartAlts;

	static void WriteCondProps(RTFwfile *wf);
	static condinfo *ResetCondInfo(void);
	static void AddCondProps(long cond);
	static void WriteFlags(RTFwfile *wf, DCnlist *flg, DCnlist *alt);

  static DClist HiddenConds;
  static bool ShowHiddenCond;
  static bool ShowTbRow;
  static bool HideTbRow;

	static void ProcTbCondUse(RTFctl *cp);


  // in dwrtfgr.cpp

  static bool ProcGr(RTFctl *cp);

  static uns PaperWide;
  static uns PaperHigh;
  static uns LMarg;
  static uns RMarg;
  static uns TMarg;
  static uns BMarg;
  static uns Gutter;
	static uns Margins;
	static uns BodyWide;
	static uns BodyHigh;
  static page *Page;
  static DCvlist PageList;
  static uns LastPage;
  static uns LastMasterPage;

  static uns LeftMaster;
  static uns RightMaster;
  static uns FirstMaster;
  static uns NextFirstMaster;

  static bool StartingPage;
  static bool StartingPageBody;
	static bool TwoSided;
	static bool FirstRight;
	static bool DifferentFirst;
	static uns FirstPageSpaceAbove;
	static bool TitlePage;
	static bool MargSet;
	static bool MirrorMarg;

  static bool NewBPage;
  static uns LastBPage;
  static uns PFrameCount;
  static bool BodyFound;

  static void ProcPage(RTFctl *cp);
  static void ProcPgEnd(RTFctl *cp);
	static bool NearEqual(uns a, uns b, uns diff = 20);
  static void ProcPgBody(void *vp);
  static void ProcPgCols(void *vp);
  static void ProcPgAttr(RTFctl *cp);
  static void ProcPgSize(RTFctl *cp);

  static uns DefCols;
  static uns ColGap;
  static bool ColLine;
  static bool NoLineNumbers;

  static dcframe *Frame;
  static frame *RFrame;
  static DCvlist FrameList;
  static DCvlist FramePageList;
  static DCnlist FrameNameList;
  static uns LastFrame;
  static DCvlist HeaderList;
  static DCvlist FooterList;
  static DCvlist MPTextList;
  static frame HeadGraphFrame;
  static frame FootGraphFrame;
  static DClist DocBodyList;
  static DCvlist BodyFrameList;
  static frame *FrameStack[FrameStackMax];
  static uns FrameLevel;
	static bool FrameStarting;

  static void ProcFrame(RTFctl *cp);
  static void ProcFrameName(RTFctl *cp);
  static void InitFrame(frame *fr);
  static void EndFrame(RTFctl *cp);
  static void ProcEndFrame(void);
  static void SetFrameSideheadProps(void);
  static void ProcNestedFrame(frame *fr, frame *nf);
  static bool ProcFrAttr(RTFctl *cp);
  static void ProcFrSize(RTFctl *cp);

  static char *FrameHRef[FrameHRMax];
  static char *FrameHAlgn[FrameHAMax];
  static char *FrameVRef[FrameVRMax];
  static char *FrameVAlgn[FrameVAMax];
  static DClist *FramePendList;
  static bool WritingFrame;
  static bool InTextFrame;
  static bool FrameStyleUsed;
  static bool WritingFrameAsText;

  static void WriteFrameCtl(RTFwfile *wf, RTFctl *cp);
  static void WriteFrame(RTFwfile *wf, frame *fr);
  static void WriteFrameText(RTFwfile *wf, frame *fr, RTFgroup *gp);
  static void WriteFrameStart(RTFwfile *wf, frame *fr);
  static void WriteFrameBox(RTFwfile *wf, frame *fr);
  static void WriteFrameCont(RTFwfile *wf, frame *fr, short rcrop = 0);

  static bool CopyOriginalGraphics;
  static DCnlist GraphFileCopyList;
  static DCnlist GraphFileFixedCopyList;
  static long GraphCopyCount;
  static void GraphFileCopyProc(char *cpt);
	static char* GraphFileName(const char *ocp);	
	static char* MapGraphFile(char *npath);	
	static char* BmxFileName(const char *ocp);	
	static bool CheckGraphFile(char *fname, char *oname,
		           frame *fr, bool fdk);
	static void SetGraphFDK(long id, char *fname);
	static void ClearGraphFDK(void);

	static const double PI;

  static graph *Graphic;
	static anyini GraphIni;
	static RTFctl *GrShapeCtl;
	static grarc CurrGrArc;
  static bool InGraphic;
  static uns GrGroupStack[GrGroupMax];
  static uns GrGroupLevel;
  static short GrStretch;
  static uns GrStyleMap[GrStyleMax];
  static uns GrFillMap[GrFillMax];

  static void ProcGraphic(RTFctl *cp = NULL);
  static void EndGraphic(RTFctl *cp = NULL);

	static void ProcArrowDef(RTFctl *cp);
	static void ProcGrArHead(graph *gr);
	static void ProcGrArTail(graph *gr);
	static short arrtips[GrArrStyleMax];
	static short arrbases[GrArrStyleMax];
	static short arrlens[GrArrStyleMax];
	static short arrfills[GrArrStyleMax];
	static DCvlist arrdefs;
	static void ProcGrArrow(uns astyle, short *tipx, short *tipy, double ang,
                      short ellarc, short polyln, short *verts, unl *gsiz);

  static void ProcGrLine(RTFctl *cp);
  static void ProcGrPen(RTFctl *cp);
  static void ProcGrFill(RTFctl *cp);
  static void ProcGrShape(RTFctl *cp);
  static void ProcGrPoly(RTFctl *cp);
  static void ProcGrRect(RTFctl *cp, uns rtyp);
  static void ProcGrEllipse(RTFctl *cp);
  static void ProcGrArc(RTFctl *cp);

  static uns BezierPts;
  static uns BezPtSpace;
  static long BezDistMax;
  static void ProcGrBezier(RTFctl *cp);
  static void ProcBez(long *bezpt);
  static bool BezFlat(long *bezpt);

	static long WMFfsize;
	static long WMFrecmax;
	static short WMFobjmax;
	static long WMFdiv;
	static long WMFprevdiv;
	static short WMFxoff;
	static short WMFyoff;

	static void UpdateWMFHead(long gsize);
	static void WriteWMFFrame(RTFwfile *wf, frame *fr, short xoff, short yoff);
  static void WriteGraphic(RTFwfile *wf, graph *gr);
  static void SetWMFStart(RTFwfile *wf, frame *fr, short rcrop = 0);
  static void WriteWMFStart(RTFwfile *wf, frame *fr,
	                          short bmode = 1, short smode = 3);
  static void WriteWMFEnd(RTFwfile *wf);
  static void SetWMFEnd(RTFwfile *wf, frame *fr, char *bmstr = NULL);
	static bool WMFRefUsed;
	static DCnlist WMFRefList;
	static void WriteWMFRefs(RTFwfile *wf);
	static void WriteWMFRef(DCwfile *w, char *txt, long num);

  static wmf_pen DefaultPen;
  static wmf_pen WhitePen;
  static wmf_pen CurrPen;
  static wmf_pen LastPen;
  static void WriteWMFPen(RTFwfile *wf, wmf_pen *pen);

	static char DefBrushType;
  static wmf_brush DefaultBrush;
  static wmf_brush BlackBrush;
  static wmf_brush WhiteBrush;
  static wmf_brush HollowBrush;
  static wmf_brush CurrBrush;
  static wmf_brush LastBrush;
  static void WriteWMFBrush(RTFwfile *wf, wmf_brush *br);
  static long ShadeColor(long colr, uns shd);
//  static void WriteWMFRegion(RTFwfile *wf, frame *fr);
  static void WriteWMFSel(RTFwfile *wf, uns num);
  static void WriteWMFDel(RTFwfile *wf, uns num);

	// in dwrtfgt.cpp

  static gtext *GText;
	static uns GTAlignChange;
	static uns GTWidth;
	static DClist GTFixList;
  static void ProcGrText(RTFctl *cp);

	static bool InGrTextFrame;
	static bool NewTextGrNeeded;
	static bool NewTextGrLine;
	static bool NewTextGrPara;
	static bool NewTextGrBase;
	static bool NewTextGrHigh;
	static bool GTDiscHyphen;
	static bool NoGrText;
	static bool NoGrTab;
	static bool FirstGTPara;
	static bool FirstGTLine;
	static double GTxmult;
	static double GTymult;
	static short GTxpos;
	static short GTypos;
	static short GTylim;
	static short GTTabPos;
	static short GTPSpace;
	static short GTFSpace;
	static RTFgroup *GTAnum; 
	static bool GTAnumProc;
	static RTFgroup *GTVar; 
	static short GTShLeft;
	static short GTShRight;

  static void ConvertTextFrame(frame *txt_fr, frame *par_fr);
	static void EndGTextLine(void);
	static void FixGTHorPosition(gtext *gt, short wid);
	static void GTFixup(long gtl);
	static void GenNewTextGraphic(void);
  static void ProcGrFormat(RTFctl *cp);
  static uns GrFWeight[GrFWeightMax];
  static void ProcGrTProp(RTFctl *cp);
	static void SetGTextLineHeight(void);
  static void ProcGrTStr(RTFctl *cp);
  static void ProcGrTx(RTFctl *cp);
  static void PutGTFrame(RTFctl *cp);
	static void PutGTSpace(void);
	static void PutGTHyphen(void);

  static void ProcFrameRefs(frame *fr);
  static void ProcGrTxRef(RTFctl *cp);

	static bool InTextMetafile;
	static frame *TMFrame;
	static void StartTextMetafile();
	static void ProcTextMetafile(RTFctl *cp);
	static void EndTextMetafile(RTFwfile *wf);

  static void WriteGrTextAlign(RTFwfile *wf, uns align);
  static void WriteGrTextColor(RTFwfile *wf, uns color);
  static void WriteGrText(RTFwfile *wf, gtext *gt);

  static wmf_font DefaultFont;
  static wmf_font CurrFont;
  static wmf_font LastFont;
  static void WriteWMFFont(RTFwfile *wf, wmf_font *fnt);

  // in dwrtfrf.cc

  static bool ProcRf(RTFctl *cp);

	static wmeta *Metafile;
	static void ProcGrMetafile(RTFctl *cp);
  static void ProcGrMetaRecs(RTFctl *cp);
  static void WriteGrMetafile(RTFwfile *wf, wmeta *mf, frame *fr);
	static uns WMFObjCodes[WMFObjCodesMax];
	static bool IsWMFObject(uns code);

  static rast *Raster;
  static void ProcGrRaster(RTFctl *cp);
  static void ProcGrColormap(RTFctl *cp);
  static void ProcGrBitmap(RTFctl *cp);
  static void Repack24BitRaster(void);
  static void RepackMonoRaster(void);
  static void RepackRaster(long new_pbpl);
  static void Convert24BitRaster(void);

  static void ProcGrBits(void);
  static void CheckGrSunRaster(long rsize, unc *bptr);
  static void ProcGrSunRaster(long rsize, unc *bptr);

  static void ProcGrEPSProp(RTFctl *cp);
  static void ProcGrEPSHead(RTFctl *cp);
	static long revL(long val);

	static void ConvertImportObjectFrame(frame* fr, frame* pfr, graph* gr);
	static void ConvertImportedBMP(frame* fr, frame* pfr, char* chp, graph* gr);
	static char ImpWMFType;
	static void ConvertImportedWMF(frame* fr, char* chp, graph* gr);
  static void WriteGrRaster(RTFwfile *wf, rast *rs);


 public:

  // in dwrtf.cc, called from main()
	static char *FilterVer;
	static char *DRmifVer;
	static char *DWrtfVer;
	static long MIFVer;
	static char *ICUdll;

	static swlist SwitchList[];
	static bool FormatSet;
	static bool ListFile;
	static char ListType;
  static short Verbose;
	static DCirfile *IniFile;
	static char OldFormType;
	static char *MacroFile;
	static DCfrfile *FmtFile;
	static DCfrfile *PagesFile;
	static DCfrfile *TablesFile;
	static DCfrfile *SubFormatsFile;
	static DCwfile *CntFile;
	static uns SourceType;
	static bool XMLSource;
	static bool UMXL;
	static bool FMSource;
	static char *CurrGrxFile;
	static char *OutputFormat;
	static char *IniFileName;
	static bool DoAutomation;
	static bool EndingAuto;
	static bool BookAuto;
	static DCLrfile *DCLFile;

  static void SetFormat(char *fmt);
	static bool InitCurrProps(RTFwfile *wf);
  static void ReadDCL(DCLrfile *rf);
  static void WriteRTF(RTFwfile *wf);

  static char *GetFinalFileName(char *src);
  static char *MakeOutID(char *out, char *id = NULL);

  static void CheckLogErrors(void);
  static void WriteInfo(RTFwfile *wf);
};



#endif

