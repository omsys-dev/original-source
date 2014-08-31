// drxml.h is the header for dcl XML reading, JHG
// Copyright (c) 2008 by Omni Systems, Inc.  All rights reserved.

#ifndef DRXML_H
#define DRXML_H


class XMLrd;
class XMLud;
class XMLrfile;
class XDVrfile;
class DCixset;
class DCixlist;
class DCixitem;
class DCixref;


enum RxmlConstants {
	MaxFmtClassType = 2,
  NumStyleMax = 7, LevelMax = 16,
  LineMax = 4096, AnsiTabMax = 128,
  ChunkTokMax = 7,
	FormNameMax = 128,
  ElemPathLen = 1024, NumLenMax = 10,
	RomanMax = 1024, AlphaMax = 6,
  RefBufLen = 1024, MaxIniLen = 65500,
	GraphPosMax = 128,
	TopicElemTypesCount = 287,
	TabMax = 20
};

enum units {
	px = 0, in, pc, pt, cm, mm, em, ex, mu
};

enum ep {
 epRoot = 1, epMap, epTopic, epTask, epMeta,
 epText, epInline, epPern, epPre, epTypo,
 epFig, epImage, epObj, epAlt, epParam,
 epTable, epTabRow, epTabCol, epTabCell,
 epTabHead, epTabBody, epStart, epShort,
 epLink, epKey, epRef, epRel, epXRsrc, epGroup, epSeq,
 epList, epListItem, epNumbered,
 epDefList, epDefEntry, epDefTerm, epDefDef,
 epParmList, epParmEntry, epParmTerm, epParmDef, epNote,
 epIndex, epIndexStart, epIndexSee, epIndexSeeAlso, epIndexSort,
 epTitle, epBkTitle, epSect, epFtn, epDesc,
 epNavtitle, epSDesc, epAbstr, epVar,
 epRefer, epGloss, epData, epCascadeItem, epCascadeSet, epSub,
 epTmark, epDraft, epAbbr, epNonum, epNolev,
 // uDoc props
 epLib, epPara, epUsage, epQuote, epMarker, epBreak, epDate, epCSH,
 epCode, epFile, epDir, epDB, epWeb,
 epElem, epDef, epCond, epOut, epAttr, epTab, epExt, epSyn
};

struct attrp {
	const char *name;
	const char **var;
};

struct rect {
  long l, t, w, h;
};

enum tunit {
 tuNone = 0, tuCol, tuIn, tuCm, tuEm
};

struct tset {
	unc type;
	long pos;
	unc leader;
};

typedef void (*attcb)(char *name, char *val);

struct strln {
	char *str;
	long len;
};

struct flagprop {
	unl fcolor;     // foreground color in hex RGB (none = 0, black = 1)
	unl bcolor;     // background color in hex RGB
	unl chcolor;    // changebar color
	unl style;      // bitmapped flags from flagstyles
	char *chchars;  // changebar characters
	char *startimg; // image to mark start of condition
	char *startalt; // alt text for start of condition
	char *endimg;   // image to mark end of condition
	char *endalt;   // alt text for end of condition
};

enum flagstyles {
	FSunder = 1, FSdouble = 2, FSover = 4, FSnumunder = 8,
	FSstrike = 16, FSchange = 32,
	FSital = 64, FSbold = 128
};

struct dvflag {
	char *image;    // href to image to use
	char *text;     // alt text for image
};

struct dvprop {
	char *att;      // attribute for prop, =rev for revprop
	char *val;      // value for prop/revprop to act on
	char *action;   // include (default), exclude, passthrough, flag
	char *chbar;    // changebar property
	char *fgcolor;  // foreground color
	char *bgcolor;  // background color
	char *style;    // (under-, double-, over-) line, bold, italic
	dvflag *startflag;   // image for start of flagged content
	dvflag *endflag;     // image for end of flagged content
};

enum trtyp {
 trnone = 0, trtext, trelem, trcond, trvar, trkey
};

enum qutyp {
 qunone = 0, qudir, qudb, quweb
};

enum keytyp {
 ktnone = 0, ktlink, ktterm
};

enum notetyp {
 ntnone = 0, ntnote, ntattention, ntcaution, ntwarning, ntdanger,
 ntfastpath, ntimportant, ntremember, ntrestriction, nttip, ntother
};

enum thtyp {
	thnone = 0, thsteps, thprereq, thcontext, thexample,
	thresult, thpostreq, thstepxmp, thstepresult, thlast
};

enum imshape {
  imnone = 0, imrect, impoly, imcirc, imdefault
};

enum sfmt {
 sfmt_none = 0, sfmt_tb, sfmt_tr, sfmt_tc, sfmt_br, sfmt_sh
};


#if 0
struct pageinfo {
	long pagenum;      // page number (not page sequence number)
	DCgroup *pgroup;   // page group
	DClist *graphics;  // list of graphnums used on page

	DCgroup *layout;   // layout frame group, layout framenum in dat()
	long firstframe;   // layout frame for page
	long lastframe;    // last frame used on page, layout or graphic
	DCctl *textframe;  // text layout framenum in dat(), pagenum in siz()
	DCctl *nextframe;  // next layout textnum in dat(), framenum in siz()

	uns textnum;      // text stream num
	DCctl *pagetext;   // add graphic frames before this one, textnum in dat()
	DCctl *flowtag;    // start of flow, remove if continuing prev flow
	DCctl *textprev;   // last text framenum in dat(), last page in siz()
	DCctl *textnext;   // next text framenum in dat(), next page in siz()
	DCctl *textend;    // end of text group, textnum in dat()
};

struct docinfo {
	long docnum;
	char *docname;
	DCnlist *topicids; // tid used to get topicnum
	DCvlist *topics;   // pointers to topicinfo structs by topicnum
	long topiccnt;     // last topicnum (1-based)
};

struct topicinfo {
	long topicnum;

	long docnum;
	docinfo *doc;
	long pagenum;      // output page used for topic content
	pageinfo *page;

	//long elemcount;    // index in BlockElems
	char *topicid;     // id for links and xrefs
	//DCctl *anchor;     // id anchor for links
	//DCctl *xanchor;    // id anchor for xrefs
	//elemtyp topictype;

	DCnlist *elemids;  // elemid used to get elemnum
	DCvlist *elems;    // pointers to eleminfo structs by elemnum
	long elemcnt;      // last elemnum (1-based)
	//long conrefnum;
};

struct eleminfo {
	long elemnum;      // starts with 1 for root
	long topicnum;
	long graphnum;     // if nonzero, has graphic frame

	topicinfo *topic;
	char *elemname;    // used to find matching end for conrefs
	long elemdepth;    // end must be at same depth

	long elemcount;    // index in BlockElems or InlineElems
	char *elemid;      // id for links and xrefs
	char *classatt;    // class attribute
	char *outclass;    // outputclass attribute
	DCclist *attrctls; // list of attribute ctls in output
	long attrnum;      // count of attribute ctls
	DCgroup *attrgrp;  // attribute ctls in output
	DCgroup *attrrep;  // replacement group of attrs for conref
	DCgroup *attrtext; // text group containing attrgrp in output

	unl eprop;         // properties flags
	bool epset;        // true once prop flags set by CheckElem()
	bool elinlin;      // true if inline, false if block
	bool xrsrc;        // true if possible xreference source
	char *elemuid;     // unique ID made from path, topic id, elemid
	DCctl *anchor;     // id anchor for links
	DCctl *xanchor;    // id anchor for xrefs
	char *elform;      // format used for element
	char *elpath;      // element path to elem
	DCctl *formctl;    // format ctl, if set by this elem
	elemtyp elemtype;

	DCgroup *elemnref; // contains full anum to be used in xref to elem
	DCgroup *elemaref; // contains num-only anum to be used in xref to elem
	DCgroup *elemtref; // contains text to be used in xref to elem
	DCgroup *elemtext; // contains full original elem text content for conref to elem
	DCgroup *elemgraph; // contains element's graphic frame if any
	DCgroup *lasttext; // parent TextGroup for restoral at end of orig
	DCgroup *fntext;   // contains full footnote content
	long conrefnum;
};
#endif

struct graphinfo;

struct imapinfo {
	char *name;         // name made up for HTML ref from image to map
	DCgroup *grmap;     // group containing imagemap areas
	DCgroup *grframe;   // group in which image map group is placed
	graphinfo *graph;   // graphic for which map is set
	DClist areas;       // imaparea *s for all map areas
};

struct imaparea {
	imapinfo *imap;     // parent imagemap
	char *shname;       // name of shape
	imshape shape;      // enum of shape
	char *coords;       // coords string
	long *carray;       // array of coord values
	long ccnt;          // count of coords
	char *href;         // xref href
	scopetyp scope;     // xref scope enum
	formtyp form;       // xref format enum
	char *alt;          // xref text content
	DCgroup *grarea;    // group in which this area is placed
};

struct graphinfo {
	long graphnum;
	long framenum;
	long pagenum;
	bool replaced;      // replaced by conref
	DCgroup *graphframe; // frame group, framenum in dat()
	char *srcpath;      // graphic source file path name
	char *srcbase;      // graphic source file base name
	DCctl *granchor;    // graph anchor, textnum, pagenum
	DCctl *txanchor;    // text anchor, framenum, pagenum
	DCtopelem *einfo;   // containing element
	imapinfo *imagemap; // imagemap
};

struct linkinfo {
	char *href;         // corrected link href attribute
	bool url;           // true if link is to URL
	bool peer;          // true if peer link
	DCctl *fnctl;       // link file name ctl
	DCctl *factl;       // link anchor ctl
	char *form;         // link format name from outputclass
	char *elpath;       // element path to link 
	DCctl *formctl;     // link format number ctl
	DCmapelem *mref;    // topicref at link src for links from map
	char *branch;       // target branch info
	conditions *conds;  // current cond info
	char *xrname;
};

struct xrefinfo {
	char *href;         // corrected xref href attribute
	DCctl *fnctl;       // xref file name ctl
	DCctl *factl;       // xref anchor ctl
	char *form;         // xref format name from outputclass
	char *elpath;       // element path to xref 
	DCctl *formctl;     // xref format number ctl
	DCgroup *xgrp;      // xref content group ctl
	DCgroup *tgrp;      // xref title group ctl
	DCctl *tctrl;       // xref title string ctl
	DCgroup *xrgrp;     // full xref group ctl
	abbrtyp abbr;				// type of abbreviation to use for GlossAbbr
	elemtyp elemtype;   // type attribute for elem to which xref points
	DCelem *xsrc;       // element to which xref points, title for topic
	DCelem *xtsrc;      // element with title to use, NULL for topic, li, fn
	DCelem *gldef;      // glossdef element, NULL if not glossentry
	char *branch;       // target branch info
	conditions *conds;  // current cond info
	char *xrname;
	bool ingloss;
};

#if 0

struct conrefinfo {
	long cnum;           // id number for conref
	char **href;         // corrected conref attribute
	char **endref;       // end conref range attribute
	char *act;           // conaction attribute
	conact action;       // none, push*, but not mark
	DCtopic *toporig;    // original topic in which conref is done
	DCtopic *topnew;     // replacement topic info
	DCtopelem *elemorig; // if elemnum == 1, orig is full topic, else element
	DCtopelem *elemnew;  // replacement element info
	bool didlists;       // true if lists below populated
	DClist anums;        // elements with anums
	DClist lofitems;     // elements in LOF
	DClist lotitems;     // elements in LOT
	char *branch;        // target branch info
	conditions *conds;   // current cond info
	char *xrname;        // name of doc containing conref
};

struct conrefmapinfo {
	long cnum;           // id number for conref
	char **href;         // corrected conref attribute
	char **endref;       // end conref range attribute
	char *act;           // conaction attribute
	conact action;       // none, push*, but not mark
	DCmap *maporig;      // original map in which conref is done
	DCmap *mapnew;       // replacement map info
	DCmapelem *elemorig; // if elemnum == 1, orig is full topic, else element
	DCmapelem *elemnew;  // replacement element info
	char *branch;        // target branch info
	conditions *conds;   // current cond info
	char *xrname;        // name of doc containing conref
};

struct conrefm2tinfo {  // map to topic
	long cnum;           // id number for conref
	char **href;         // corrected conref attribute
	char **endref;       // end conref range attribute
	char *act;           // conaction attribute
	conact action;       // none, push*, but not mark
	DCmap *maporig;      // original map in which conref is done
	DCtopic *topnew;     // replacement topic info
	DCmapelem *elemorig; // if elemnum == 1, orig is full topic, else element
	DCtopelem *elemnew;  // replacement element info
	char *branch;        // target branch info
	conditions *conds;   // current cond info
	char *xrname;        // name of doc containing conref
};

struct conreft2minfo {  // topic to map
	long cnum;           // id number for conref
	char **href;         // corrected conref attribute
	char **endref;       // end conref range attribute
	char *act;           // conaction attribute
	conact action;       // none, push*, but not mark
	DCtopic *toporig;    // original topic in which conref is done
	DCmap *mapnew;       // replacement map info
	DCtopelem *elemorig; // if elemnum == 1, orig is full topic, else element
	DCmapelem *elemnew;  // replacement element info
	char *branch;        // target branch info
	conditions *conds;   // current cond info
	char *xrname;        // name of doc containing conref
};

#endif

// element recording

struct elemdata {
	char *name;
	char **attrs;
	long depth;
};

struct enddata {
	char *name;
	long depth;
};

struct textdata {
	char *str;
	long len;
	long depth;
};

struct pidata {
	char *target;
	char *data;
};

enum parsetyp {
 parnone = 0, parelem, parend, partext, parpi
};

struct parsedata {
	parsetyp type;
	void *pdata;
};


struct conditions {
	char *DVname;

	DCvlist DVProps;
	long DVPropCount;
	char *DVDefPropAction;
	bool DVDefPropExclude;

	long CondAttDefCount;
	DCnlist CondAttDefNames;
	DCnlist CondAttDefaults;
	DCvlist CondAttDefFlags;

	long CondAttExcCount;
	DCnlist CondAttExcNames;
	DCvlist CondAttExclude;
	DClist CondAttDefExclude;

	long CondAttIncCount;
	DCnlist CondAttIncNames;
	DCvlist CondAttInclude;
	DClist CondAttDefInclude;

	long CondAttPassCount;
	DCnlist CondAttPassNames;
	DCvlist CondAttPassthrough;

	long CondAttFlagCount;
	DCnlist CondAttFlagNames;
	DCvlist CondAttFlagging;

#if 0
	long CondFlagCount;
	DCnlist CondFlagNames;
	DCvlist CondFlagProps;
#endif
};



// class for xml parsing vars and funcs


class XMLrd {
  friend class XMLud;

 protected:
	// in drxml.cpp

	static char ObjectIDs;
	static long CurrObjectID;

	static bool CaselessMatch;
	static bool SpacelessMatch;
	static bool WildcardMatch;
	static bool ProcConrefs;
	static char *IDFileName;
	static char *PrjFileName;
	static char *ConfigTemplate;

	static char *LanguageText;
	static DCnlist LangFiles;
	static DCnlist LangIDs;
	static DCirfile *GetLangFile(char **ltext, char **langid, bool first = false);
	static void RestoreLangFile(DCirfile *lfile);
	static void ResetLangFile(void);
	static char *GetParentLang(DCelem *einfo);

	static bool UseLog;
	static bool ShowLog;
	static char *LogIniPrefix;
	static char *LogFileName;
	static char *HistoryFileName;
	static char *EditorFileName;
	static DClog *Log;
	static bool LogIniChains;
	static unc LogLevels[logmax];
	static void LogEvent(logtype lt, unc sev, const char *desc,
	              const char *d2 = NULL, const char *d3 = NULL,
	              const char *d4 = NULL, const char *d5 = NULL);

	static char *MacroFile;

	static DCnlist Align;
	static npair AlignInit[];
	static DCnlist Valign;
	static npair ValignInit[];
	static DCnlist TrademarkTypes;
	static npair TrademarkTypesInit[];
	static spprop CurrElem;
	static char **CurrElemType;
	static bool SetupComplete;
#ifdef DITA
	static void SetCatalogs(void);
	static void ProcCurrElemTypes(void);
#endif
	static void ConvertSarr(char **sarr, etype *et);

	static DCvlist DocsToGo;
	static DCvlist DocRefs;
	static long CurrDocLevel;
	static long DocCount;
	static long CurrDocNum;
	static long NextDocNum;
	static DCmapelem *CurrDocRef;
	static DCmapelem *InitDocRef;
	static DCtopdoc *CurrDocInfo;
	static DCvlist DocInfos;

	static long CurrTopicNum;
	static DCtopic *CurrTopicInfo;

	static DCnlist DocRefsProcessed;
	static DCmlist DocNamesProcessed;
	static DCmlist ConrefDocNamesProcessed;
	static char *DocRef;
	static char *DocName;
	static char *DocBasePath;
	static char *DocBaseName;
	static char *MainDocName;
	static char *MainBaseName;
	static char *DocID;
	static char *DocElement;
	static char *DTDPath;
	static bool FirstTopic;
	static bool ConrefTopic;
	static bool SkipTopic;
	static long SkipNesting;

	static DCvlist MapsToGo;
	static DCvlist MapRefs;
	static long CurrMapLevel;
	static long MapCount;
	static long CurrMapNum;
	static DCmapelem *CurrMapRef;
	static long CurrMapPos;
	static DCmap *CurrMapInfo;
	static DCnlist MapNamesProcessed;
	static DCnlist ConrefMapNamesProcessed;
	static DCvlist MapInfos;

	static bool KeydefsDone;
	static bool ProcessingKeyrefMaps;
	static DCvlist KeyrefMapsToGo;
	static DCvlist KeyrefMapRefs;

	static DCnlist DocRoots;
	static char *DocRoot;
	static bool InitialParse;
	static bool GenerateMapIfMissing;
	static bool UsingBookmap;

	static void ParseMap(XMLrfile *rf, char **pref);
	static void ParseDocument(XMLrfile *rf, char **pref, bool cnref = false);
	static bool UseReplacementMap(void);

	static bool UsePartFiles;
	static char *PartFileLevel;
	static long PartFileLevelNum;
	static long DCLPartFile;
	static DCnlist DCLPartFileNames;
	static DCnlist DCLPartBaseNames;
	static char *DCLPartFileName;
	static char *DCLPartBaseName;
	static DCwfile *DCLPartListFile;
  static void CheckPartStart(DCelem *elem);
  static void MakePartFileName(char *name, long page);

  static DCilist DCLPartPages;
  static DCilist DCLPageParts;
  static void WriteDCLFile(DCLwfile *wf, long part = 0);

  static long DCLHeadDiv;
  static void WriteDCLHead(DCLwfile *wf);

  static void CheckLogErrors(void);

	static char *UnitAbbrs[];
	static long GetNumericAttr(const char *aval);
	static long ConvertVal(double val, units unit);
	static void ProcAttrs(const char **attp, attrp *alist);
	//static char *GetAttr(const char **attp, const char *name);

	// in drxmlsp.cpp

#ifdef SAMPLE
	static char *SampList[];
	static int *SampListLen;
	static uns SampListNum;
	static uns SampListCount;
	static bool SampDelayRand;
	static uns SampDelay;
	static uns SampDelayLimit;
	static uns SampDelayMin;
	static bool SampStart;
	static bool SampEnd;
	static uns SampStartPos;
	static uns SampWordMin;

	static void SetUpSample(void);
	static void SampConvert(char *str);
#endif

	// in drxmlgn.cpp

	static DCvlist GenLists;
	static DCnlist GenListNames;
	static npair InitGenListNames[];
	static npair InitUMXLGenListNames[];
	static char *TitlePageFile;
	static char *TitlePageTemplate;
	static bool InGenerateFile;
	static bool GenerateTOC;
	static char *TOCFile;
	static char *TOCSuffix;
	static char *TOCFormat;
	static char *TOCTitle;
	static char *TOCTitleFormat;
	static char *TOCXrefFormat;
	static char *TOCLinkFormat;
	static DCgroup *TOCTextGroup;
	static bool InTOCTOC;
	static bool KeepTOCWithTitlePage;
	static DCgroup *PreTOCTOCGroup;
	static DCgroup *TOCTOCGroup;
	static DCgroup *PreTOCTOCIgnore;
	static DCgroup *TOCTOCIgnore;
	static DCgroup *TOCWrapper;
	static bool UseContents;
	static bool GenerateLOF;
	static char *LOFFile;
	static char *LOFSuffix;
	static char *LOFFormat;
	static char *LOFTitle;
	static char *LOFTOCFormat;
	static char *LOFTitleFormat;
	static char *LOFXrefFormat;
	static DCgroup *LOFTextGroup;
	static bool GenerateLOT;
	static char *LOTFile;
	static char *LOTSuffix;
	static char *LOTFormat;
	static char *LOTTitle;
	static char *LOTTOCFormat;
	static char *LOTTitleFormat;
	static char *LOTXrefFormat;
	static DCgroup *LOTTextGroup;
	static bool GenerateIDX;
	static bool MakeDefaultIDX;
	static char *IDXFile;
	static char *IDXSuffix;
	static char *IDXFormat;
	static char *IDXTitle;
	static char *IDXTOCFormat;
	static char *IDXTitleFormat;
	static char *IDXXrefFormat;
	static DCgroup *IDXTextGroup;
	static bool IndexInTOC;
	static bool GenerateGLS;
	static char *GLSFile;
	static char *GLSSuffix;
	static char *GLSFormat;
	static char *GLSTitle;
	static char *GLSTOCFormat;
	static char *GLSTitleFormat;
	static char *GLSXrefFormat;
	static DCgroup *GLSTextGroup;
	static bool InGlossEntry;
	static bool InGlossTerm;
	static bool InGlossAbbr;
	static char *GlossaryFileName;
	static bool GenerateABR;
	static char *ABRFile;
	static char *ABRSuffix;
	static char *ABRFormat;
	static char *ABRTitle;
	static char *ABRTOCFormat;
	static char *ABRTitleFormat;
	static char *ABRXrefFormat;
	static DCgroup *ABRTextGroup;
	static char *AbbrFileName;
	static bool GenerateTMK;
	static char *TMKFile;
	static char *TMKSuffix;
	static char *TMKFormat;
	static char *TMKTitle;
	static char *TMKTOCFormat;
	static char *TMKTitleFormat;
	static char *TMKXrefFormat;
	static DCgroup *TMKTextGroup;
	static char *TmarkFileName;
	static DCnlist GenFileNames;
	static DCilist GenFileElems;
	static long GenFileCount;
	static char *GenListXrefFormat;
	static char *ListXrefFormat;

	static void SetGenFileRef(DCmapelem *elem);
	static void SetTOCTOC(void);
	static DCgroup *TOCIgnore;
	static DCgroup *PreTOCIgnore;
	static void SetGenFileTOC(DCmapelem *elem, int depth, long level);
	static DCctl *WriteGenListTOC(DCmapelem *elem, char *title, char *id);
	static void SetUpGenFile(DCmapelem *elem);
	static void SetGeneratedFile(DCmapelem *elem);
	static void WriteGenListHead(DCmapelem *elem, char *href, char *title,
		    char *titleform, char *id, long typ, DCgroup **gpp, bool usehead);

	static bool InFigTitle;
	static DClist LOFItems;
	static DClist *LOFItemList;
	static bool InTblTitle;
	static DClist LOTItems;
	static DClist *LOTItemList;
	static bool InGlossary;

	static void AddGenListItem(DCelem *elem, char *form);
	static void FixConrefGenLists(DCelem *srcelem, DCelem* newelem);
	static void MarkConrefGenList(long refnum);
	static void EndConrefGenLists(DCcnref *cinfo);
	static void ProcessGeneratedLists(void);
	static void WriteGenListItems(DCgenlist *gl);
	static void WriteOneGenListItem(char *form, DCelem *einfo);

	static bool UseEntry;
	static bool UseList;
	static bool UseDefList;
	static DCgroup *CurrListIndexGroup;
	static bool SortSeeAlsoFirst;
	static char *IgnoreLeadingCharsIX;
	static char *IgnoreCharsIX;

	static void CheckIndexListItem(void);
	static void StartIndexListItem(void);
	static void AddIndexListItemLevel(void);
	static void AddIndexListSeeItemLevel(void);
	static void RestartIndexListItem(void);
	static void StartIndexListSeeSortItem(void);
	static void EndIndexListSeeItem(void);
	static void EndIndexListSortItem(void);
	static void EndIndexListItem(void);
	static void SetIndexRefIDs(DCixref *ref);

	static char *IndexListPartName;
	static DCixref *LastRefParent;
	static void CloneIndexRef(DCelem *elem, DCixref *ref);

	static void WriteIndexListItems(DCixlist *list);
	static bool UseCompactForm ;
	static bool IndexRefIsPara;
	static bool IndexRefsWithEntry;
	static bool IndexRangeStartOnly;
	static bool UseIndexHeading;
	static bool UseIndexLeader;
	static bool UseIndexLetters;
	static bool UseIndexTopLetters;
	static void WriteOneIndexListItem(DCixitem *item, DCixlist *list,
					                          DCgroup *gp, long lev);
	static void WriteOneIndexRefItem(DCixref *ref, DCixlist *list,
			                              DCgroup *gp, long lev);
	static void WriteOneIndexRefRange(DCixref *ref, DCixref *eref,
		                     DCixlist *list, DCgroup *gp, long lev);
	static void WriteOneIndexRefContent(DCixref *ref, DCixlist *list,
			                              DCgroup *gp, long lev);
	static void WriteOneIndexSeeItem(DCixitem *item, DCixlist *list,
			                              DCgroup *gp, long lev);
	static char *IndexRefSep;
	static char *IndexRefStartSep;
	static char *IndexRangeSep;
	static void WriteIndexEntryRefSep(DCixlist *list);
	static void WriteIndexRefSep(void);
	static void WriteIndexRangeSep(void);


	static DCnlist GlossAbbrList;
	static long GlossAbbrCount;
	static char *GlossResetElem;
	static DCnlist GlossResetElems;
	static npair GlossResetElemsInit[];
	static long GlossResetLevel;
	static bool AcronymInTitle;
	static char *TmarkResetElem;
	static long TmarkResetLevel;

	static void InitGlossAbbr(void);
	static void ProcGlossAbbr(void);
	static void ResetGlossAbbr(long level);
	static void FindGlossAbbrReset(DCelem *el);
	//static bool IsFirstChild(DCelem *el);
	static DCtopelem *GetGlossSurfaceForm(DCtopelem *einfo);
	static DCtopelem *GetGlossAcronymForm(DCtopelem *einfo);
	static DCtopelem *GetGlossBody(DCtopelem *einfo);


	// in drxmlpi.cpp

	static DCctl *TopicAlias;

	static void ProcPI(const char *target, const char *data);
	static char **ProcPIAttrs(const char *data);

	static char *NextOutputclass;

	static DCnlist HyperTypes;
	static npair InitHyperTypes[];

	static DCctl *PendingBreak;
	static void ProcOnePIAttr(char *name, char *val);
	static void SetTopicAlias(char *val);

	static long CurrVar;
	static bool InVar;
	static DCnlist VarNames;
	static DCclist VarFormats;
	static DCilist VarArraySize;
	static DCvlist VarArrayList;
	static DClist *CurrVarArrayElems;
	static long CurrVarArrayBase;
	static long CurrVarArrayItem;

	static char *IsVariable(DCelem *elem);
	static DCgroup *SetVariable(const char *name);
	static void ProcessVariableStart(DCelem *einfo);
	static void ProcessVariableEnd(DCelem *einfo);
	static void UseVariable(char *vname);
	static void UseVariable(long vnum);
	static void WriteVariableDef(DCgroup *gp, long i);

#if 0
	static DCnlist ImpAttrs;
	static DCvlist ImpAttrVals;
	static long ImpAttrCount;

	static bool UseStepImportance;
	static bool UseImpRequired;
	static bool UseImpOptional;
	static bool UseImportanceAttribute;
	static bool UseImpAttr;
	static char *ImportanceRequired;
	static char *ImportanceOptional;
	static DCgroup *CurrImpAttr;
	static long CurrImpVar;

	static void PredefineVariable(const char *vname, char *vdef);
	static void SetVarGroupContent(char *str, char *vtyp,
	                               const char *vname);
	static void SetImportance(const char *impatt);

	static DCnlist NoteTypes;
	static npair NoteTypesInit[];
	static DCvlist NoteHeads;
	static vpair NoteHeadsInit[];
	static DCvlist NoteGroups;
	static vpair NoteGroupsInit[];
	static DCvlist NoteFormats;
	static vpair NoteFormatsInit[];
	static notetyp CurrNoteType;

	static bool UseNoteHeads;
	static char *NoteHead;
	static char *AttentionHead;
	static char *CautionHead;
	static char *WarningHead;
	static char *DangerHead;
	static char *FastpathHead;
	static char *ImportantHead;
	static char *RememberHead;
	static char *RestrictionHead;
	static char *TipHead;
	static char *OtherHead;

	static DCgroup *NoteGroup;
	static DCgroup *AttentionGroup;
	static DCgroup *CautionGroup;
	static DCgroup *WarningGroup;
	static DCgroup *DangerGroup;
	static DCgroup *FastpathGroup;
	static DCgroup *ImportantGroup;
	static DCgroup *RememberGroup;
	static DCgroup *RestrictionGroup;
	static DCgroup *TipGroup;
	static DCgroup *OtherGroup;

	static bool UseNoteFormats;
	static char *NoteFormat;
	static char *AttentionFormat;
	static char *CautionFormat;
	static char *WarningFormat;
	static char *DangerFormat;
	static char *FastpathFormat;
	static char *ImportantFormat;
	static char *RememberFormat;
	static char *RestrictionFormat;
	static char *TipFormat;
	static char *OtherFormat;

	static void SetNoteGroup(void *ph, long t);
	static void SetNoteHead(void);
	static char *SetNoteFormat(void);

	static bool InTaskTopic;
	static bool UseTaskHeads;

	static DCvlist TaskHeads;
	static vpair TaskHeadsInit[];
	static DCvlist TaskHeadGroups;
	static vpair TaskHeadGroupsInit[];

	static char *StepsHead;
	static char *PrereqHead;
	static char *ContextHead;
	static char *ExampleHead;
	static char *ResultHead;
	static char *PostreqHead;
	static char *StepxmpHead;
	static char *StepresultHead;

	static DCgroup *StepsGroup;
	static DCgroup *PrereqGroup;
	static DCgroup *ContextGroup;
	static DCgroup *ExampleGroup;
	static DCgroup *ResultGroup;
	static DCgroup *PostreqGroup;
	static DCgroup *StepxmpGroup;
	static DCgroup *StepresultGroup;

	static void SetTaskHeadGroup(void *ph, long t);
	static void SetTaskHead(void);
#endif

	static DCnlist *CurrFormatPrefixes;
	static char *DefaultNoteType;
	static void GetElementAttrPrefixes(DCelem *elem, char **form);
	static DCnlist PrefixRuninNames;
	static DCvlist PrefixRuninGroups;
	static long PrefixRuninCount;
	static void AddDefFormatPrefixHeads(DCelem *elem);
	static void SetDefFormatPrefixHeads(bool inl);
	static void WriteFormatRunin(DCelem *elem, char *fmname);
	static void WriteRuninHead(DCgroup *grp, char *nm);
	static void WriteIndexSeeItem(DCgroup *grp, char *nm);
	static void WriteMarkItem(DCgroup *grp, char *nm);
	static bool WritingSubformatHead;
	static void WriteSubformatHead(DCgroup *grp, char *nm, char *fcp,
                                 char *langsect, char *formtype);
	static long MakeSarr(char *str, char ***psarr);
	static void StripSpaces(char *str);

	static DCnlist TaskHeadTypes;
	static npair TaskHeadTypesInit[];
	static thtyp CurrTaskHeadType;
	static char *StepsHeadFormat;
	static DCelem *CurrTaskHeadElem;
	static long CurrTaskHeadDepth;
	static bool CurrTaskHeadUsed;
	static void SetStepsPara(DCelem *einfo = NULL);

	// in drxmlel.cpp

	static DCnlist CurrElems;
	static DCvlist CurrEInfo;
	static long CurrDepth;
	static long CurrRootDepth;
	static long CurrBranchDepth;
	static long CurrElemDepth;
	static char *CurrElemName;
	static DCelem *CurrElemInfo;
	static DCelem *PrevElemInfo;
	static long CurrElemNum;
	static bool InTitleElem;
	static DCgroup *TitleGroup;
	static bool ReadTopicTitle;
	static bool TopicTitleStarting;
	static bool InAnumElem;
	static long InList;
	static long InListItem;
	static long InPairList;
	static long InPairListTerm;
	static long InPairListDef;
	static DClist PairListLevels;
	static bool InCodeblock;
	static bool UseCodeblockPI;
	static DCgroup *AnumGroup;
	static DCgroup *PrePrologText;
	static bool KeepPrologText;

	static bool SkipToElement;
	static bool EndShortTag;
	static bool EndAtElement;
	static char *EndingElemName;
	static long EndingElemDepth;

	//static bool SkipThisElement;
	//static char *SkippingElemName;
	//static long SkippingElemDepth;

	static DCnlist BlockElems;
	static long BlockElemCount;
	static long BlockElemsStarted;
	static DCilist BlockAttrNumStartList;
	static DCilist BlockAttrNumEndList;
	static DCnlist BlockAttrNameList;
	static DCnlist BlockAttrValList;
	static long BlockAttrNum;
	static DCvlist BlockElemInfo;
	//static DCvlist RootElemInfo;

	static DCnlist InlineElems;
	static long InlineElemCount;
	static long InlineElemsStarted;
	static DCilist InlineAttrNumStartList;
	static DCilist InlineAttrNumEndList;
	static DCnlist InlineAttrNameList;
	static DCnlist InlineAttrValList;
	static long InlineAttrNum;
	static DCvlist InlineElemInfo;

	static bool IsFirst;
	static char *CurrLang;
	static DCvlist LangStack;
	static long LangLevel;
	static DCilist LangDepthStack;
	static DCnlist LangAttrStack;
	static DCnlist ElemClasses;

	static bool InTextref;
	static DCvlist PreTextRefGroups;
	static DCgroup *TextRefGroup;
	static char *TextRefIDs;
	static trtyp TextRefType;
	static DCnlist VarLibPaths;
	static long VarLibCount;

	static qutyp QueryRefType;
	static DCvlist PreQueryRefGroups;
	static DCgroup *QueryRefGroup;
	static DCelem *QueryParent;
	static char *Query;
	static char *QueryResultsPath;
	static DCnlist QueryResult;

	static void StartElement(const char *name, const char **atts,
		long depth);

	static bool OutputclassHasBorderShadeFormats;
	static char *BorderType;
	static char *ShadeType;
	static char *BorderFmt;
	static char *ShadeFmt;
	static char **FmtClassTypes[MaxFmtClassType];
	static char **FmtClassNames[MaxFmtClassType];
	static DCnlist OutClassParts;
	static DCilist OutClassTypes;
	static char *CheckOutputClass(const char *outputclass);
	static bool CheckClassType(char *oclass, long cnt = 0);

	static long GetProcessedDocInfo(const char *dname, bool strict = false);
	static void SetDocInfo(char *ref);
	static void SetTopicInfo(const char *name, const char *classatt);
	static void SetTopicElemInfo(const char *name, const char *classatt,
	  const char *idatt, const char *outclass, const char *hrefatt,
		const char *conrefatt, const char *conendatt,
		const char *conactatt, const char *keyrefatt,
		const char *conkeyrefatt, const char *idrefatt, const char *idrefsatt,
		long depth, bool isRoot, bool first);

	static DCvlist ElemTextGroups;
	static void SetElemText(bool conref = false, bool map = false);
	static void EndElement(const char *name, long depth);

#ifdef UDOC
	static void SetTextRef(DCelem *celem, trtyp typ);
	static void GetTextRef(trtyp typ);

	static void SetQueryRef(DCelem *elem, qutyp typ, const char *query, const char *results);
	static void RunQuery(char *src, char *query, DCnlist *dcn);
	static void GetQueryRef(qutyp typ);
#endif

	static void PutMarkFormat(char *mark);
	static bool CheckElemName(const char *name, const char *classatt,
	                       unl bit, unl neg = 0, bool all = true);
	static char *NormalizeClass(const char *eclass);

	static int __cdecl CompElemTypes(const void *elem1, const void *elem2);

	static char *CascadeSeparator;
	static bool InCascade;
	static bool CascadeStarted;

	static char *Quotes;
	static uns QuoteLevel;
	static unl QuoteChars[4];
	static void SetQuotes(void);
	static DCctl *SetTypographic(const char *name, bool start);
	static elemtyp GetTopicNameType(const char *name);
	static elemtyp GetTopicType(const char *name, const char *classatt);

#ifdef DITA
	static bool RecordingElements;
	static long RecElemDepth;
	static DClist *RecList;
	static void RecElemStart(const char *name, const char **attp, long depth);
	static void RecElemEnd(const char *name, long depth);
	static void RecText(const char *str, long len, long depth);
	static void RecPI(const char *target, const char *data);
	static void Playback(DClist *rec, long currdepth);
#endif


	// in drxmldv.cpp

	static DCvlist ConditionSets;
	static DCnlist ConditionSetFiles;
	static DClist CondSetPendList;
	static long ConditionSetCount;
	static conditions *CurrCondSet;
	static DCvlist CondSetLevels;
	static long CurrCondSetLevel;
	static long CurrCondRefLevel;
	static char *CurrBranch;
	static DCvlist BranchLevels;
	static long CurrBranchLevel;

	static void SetNewConditionSet(const char *dvname, bool pi = false);
	static void SetNewBranch(char *bname);
	static void XMLrd::RestoreConditionSet(void);

	static char *DitavalFile;
	static bool UseMatchingDitaval;
	static bool ComplexOtherprops;
	static bool CondListsInitialized;
	static XDVrfile *DVFile;
	static bool ProcessDitavalFile(char *dvname);
	static void SetDVCatalogs(void);

	static DCnlist DVElemNames;
	static npair DVelems[8];
	static DCnlist DVActions;
	static npair DVacts[5];
	static DCnlist DVStyles;
	static npair DVstyls[6];
	static DCnlist DVColors;
	static npair DVcolrs[17];

	static dvprop *CurrDVProp;
	static dvflag *CurrDVFlag;
	static char *CurrDVText;
	static long CurrDVTextLen;
;
#if 0
	static DCvlist DVProps;
	static long DVPropCount;

	static char *DVDefPropAction;
	static long CondAttDefCount;
	static DCnlist CondAttDefNames;
	static DCnlist CondAttDefaults;
	static DCvlist CondAttDefFlags;
#endif

	static void StartDVElement(const char *name, const char **atts, long depth);
	static void EndDVElement(const char *name, long depth);
	static void ProcDVChar(const char *str, int len, long depth);
	static void ProcDVPI(const char *target, const char *data);
	static void ProcessDVPropElement(void);
	static void SetDVFlagProps(flagprop *fprop, dvprop *dprop);
	static void SetDVChangebarProps(flagprop *fprop, dvprop *dprop);
	static unl GetDVColor(char *str, char *src = NULL);

	static bool CondAttrsProcessed;
	static void SetCondAttrs(void);
	static void SetIniFlagProps(long flagnum, char **sarr);
	static void GetIniFlagInfo(flagprop *fprop, char *fname, bool start);

	static bool DoExclude;
	static bool DoInclude;
	static long ExcludeDepth;
#if 0
	static long CondAttExcCount;
	static DCnlist CondAttExcNames;
	static DCvlist CondAttExclude;
#endif

	static bool CheckFilterConds(const char **atts);
	static bool CondAttrsExclude(const char **atts);
	static bool CheckOneCondAttr(const char *attname, const char *attval);
	static bool CheckCondExcVals(const char *attr, DCnlist *attlist);
	static bool CheckCondIncVals(const char *attr, DCnlist *attlist);

	static bool AttrPassthrough;
#if 0
	static long CondAttPassCount;
	static DCnlist CondAttPassNames;
	static DCvlist CondAttPassthrough;
#endif
	static void SetPassthroughAttrs(DCelem *einfo, DCgroup *tgrp);

	static bool DoFlag;
	static bool UseConditionalFlagging;
	static char *CSSFlagsFile;
	static bool WriteFlagsFile;
	static bool ReferenceFlagsFile;

#if 0
	static long CondAttFlagCount;
	static DCnlist CondAttFlagNames;
	static DCvlist CondAttFlagging;
#endif

	static long CondFlagCount;
	static DCnlist CondFlagNames;
	static DCvlist CondFlagProps;

	static DCvlist CondFlagLevels;
	static DClist *CurrCondFlags;
	static DClist *NextCondFlags;

	static void CheckFlags(const char **atts);
	static void CheckOneFlagAttr(const char *attname, const char *attval,
		                           DClist *flaglist);
	static bool SetFlags(long depth);
	static void EndFlags(void);
	static void SetNextCondFlags(long flag);
	static void SetRowFlags(long depth, DCgroup *gp);

	static void ProcCondFlagDef(DCgroup *gp, long num);

	static bool FilterConditions;
	static DCnlist FilterDialogAttributes;
	static DCnlist *FilterClasses;
	static long FilterClassCount;
	static void SetFilterAttributes(void);
	static void WriteFilterClass(char *aname, char *aval);
	static void SetFilterClasses(bool tbl = true, bool inl = false);


	// in drxmldt.cpp

	static etype **ElemTypes;
	static long ElemTypesCount;
	static etype XMLrd::TopicElemTypes[TopicElemTypesCount];
	//static etype XMLrd::MapElemTypes[MapElemTypesCount];
	static DCnlist EpNames;
	static npair EpNameInit[];


	// in drxmltx.cpp

	static long ItemID;
	static char *PendingText;
	static long PendingLen;
	static long InterpolatedP;
	static char *InterpolatedPName;
	static bool CloseInterP;
	static long TextBlockElemLevel;
	static char *TextBlockElemName;
	static DCgroup *TextGroup;
	static DCnlist FormatUsed;
	static DClist FormatInline;
	static DCnlist ForcedClose;
	static char *ContinuedFormatSuffix;
	static bool AllowText;
	static DClist AllowTextLevels;
	static bool InMap;
	static bool IsInline;
	static bool IsPreform;
	static bool PreInline;
	static bool StartingPre;
	static long PreformDepth;
	static bool UseElemNameForFormat;
	static bool UseOutclassForFormat;
	static bool KeepDraftComments;
	static bool IncludeElementTags;
	static bool ShowElementPath;
	static bool DisplayElementPath;
	static char *CurrFormat;
	static char *DefInlineFormat;
	static char *DefBlockFormat;
	static char *ElementPathFormat;

	static bool InWhite;
	static bool InElemText;
	static DCilist InText;
	static DCilist InLine;
	static long LastDepth;
	static void SetStringProps(long depth);

	static strln *ProcStr(const char *str, int len, long depth);
	static void ProcChar(const char *str, int len, long depth);

#ifdef UDOC
	static DCilist CurrShortPairs;
	static DCnlist CurrShortEcode;
	static DCilist CurrShortDepth;
	static DCnlist CurrShortEtext;
	static long CurrShortPairLevel;
	static void ProcParaTextShorthand(char *cpt);
	static void EndParaTextShorthand(void);
#endif

	static void RemoveTrailingSpace(void);
	static void FlushText(void);
	static long PutTextCtrls(char *text, long len = 0,
	                         bool brspc = false, bool brctl = true);
	static void PutOneTextCtl(char *text, size_t len);

	static void SetElemFormat(bool flush = false);
	static void SetParentPrefix(DCelem *einfo, DCelem *epinfo, long depth);
	static void SetMetadata(void);
	static char *GetElemPath(long lev, long num = 0);
	static void GetExtendedElemPath(DCelem *elem, long lev = 0);
	static char *GetElemForm(DCelem *elem, const char *outclass,
		        const char *ename, bool inl = false, long lev = 0);

	static DCnlist BlockFormatList;
	static DCnlist InlineFormatList;
	static long BlockFormCount;
	static long InlineFormCount;

	static DCclist BlockFormats;
	static DCclist InlineFormats;
	static long BlockFormUseCount;
	static long InlineFormUseCount;

	static long StoreFormat(const char *form, bool inl = false);
	static DCctl *SetFormat(const char *form, bool inl = false,
		                      bool setgroup = true);
	static void SetSpecialFormat(sfmt sftype, const char *fname);

	static tset *Tabs[TabMax];
	static tset *CurrTab;
	static bool TabPending;
	static long TabCount;
	static long PendTabLinePos;
	static unc PendAlignChar;
	static long PendAlignCharPos;
	static DCgroup *PendTabStart;
	static tunit TabUnits;
	static bool ColTabSet;
	static bool PreTabPreform;
	static bool IsTabPreform;

	static void SetTabs(DCelem *elem);
	static void ClearTabs(void);
	static void UseTab(char *tnum);
	static void StartPendingTab(tset *tbset);
	static void SetPendingTab(void);

	static uns PageNum;
	static long PageSeqNum;
	static long LastPageSeq;
	static DCpage *CurrPageInfo;
	static DCvlist Pages;
	static DCvlist MapPages;
	static DCilist PageSequence;
	static DClist RemovedPages;
	static DClist ConrefPages;
	static uns FirstTopicPage;
	static DCgroup *PageGroup;
	static DCgroup *PageTextGroup;
	static DCclist PageList;
	static bool PageStarted;
	static DCclist PageTextCtls;

	static void StartPage(void);
	static bool SetMapName;
	static void SetChunkMarker(char *fname);
	static void EndPage(void);
	static void SetFirstPage(void);
	static void ConnectPage(long pgseq, long lpgseq);

	static uns FrameNum;
	static DCilist FramePageList;
	static uns TextNum;

	static bool ParaStarted;
	static long LinePos;
	static bool StartPern;
	static long PernDepth;
	static bool StartIgnoreGroup;
	static bool EndIgnoreGroup;
	static bool InIgnoreGroup;
	static DCgroup *PreIgnoreGroup;
	static DCgroup *IgnoreGroup;
	static DCgroup *LastParaStartGroup;
	static void StartPara(void);
	static void EndPara(DCgroup *txt = NULL);

	static void SetUniqueID(void);
	static void SetStartTextGroup(void);
	static void SetEndTextGroup(void);
	static void SetBlockElemTags(void);
	static void SetInlineElemTags(void);
	static void SetMapElemTags(void);

	static bool GenerateUIDs;
	static bool UseLinkID;

	static void SetAnchorIDs(DCelem *einfo);

	static bool DuplicateNameCheck;
	static char *UniqueNameSuffixFormat;
	static long UniqueNameSuffixLength;
	static DCnlist InputFileNames;
	static DCnlist OutputFileNames;
	static char *MakeNameUnique(char *nm, DCelem *einfo, bool inp = false);

	static DCctl *PutTextStrCtl(const char *str, size_t len = 0);
	static DCctl *PutTextCharCtl(unl uch);


	// in drxmltb.cpp

	static bool InTable;
	static bool InTableTitle;
	static bool InTableRow;
	static bool InTableCell;
	static bool TableHead;
	static bool TableFoot;
	static bool TableColMajor;
	static bool TableRowMajor;
	static long TableDepth;
	static long TableCol;
	static long TableColCount;
	static long ColStraddle;
	static long TableRow;
	static long TableRowCount;
	static long RowStraddle;
	static long *CurrRowStrad;
	static DCilist CurrColStrad;

	static DCgroup *TableGroup;
	static DCgroup *PreTableGroup;
	static DCgroup *TableTitleGroup;
	static DCgroup *TableRowGroup;
	static DCgroup *TableCellGroup;
	static DCvlist TableRows;
	static DCvlist TableColElems;
	static DClist TableHeadCols;
	static long TableColTypeDef;
	static DCilist TableRowTypes;
	static long TableRowTypeDef;
	static DCilist TableRowHeights;
	static long TableRowHeightDef;
	static DCctl *TableColCtl;
	static DCctl *TableWtypeCtl;
	static DCctl *TableColWidCtl;
	static DCnlist ColNameList;
	static DCilist TableColWidths;
	static DCvlist TableColWidthsD;
	static long TableColWidthLDef;
	static double TableColWidthDDef;
	//static long DefColWidth;
	static bool TableColWidthsPercent;
	static double TableColWidthSum;
	static char *TableWidth;
	static char *SimpleTableWidth;
	static char *SimpleTableRelCol;
	static char *TableFooterClass;

	static bool DefinitionListTables;
	static bool InDefinitionList;
	static bool InDefinitionEntry;
	static bool InDefinitionTerms;
	static bool InDefinitionDefs;
	static long DefinitionListTermCount;
	static long DefinitionListDefCount;

	static bool ParameterListTables;
	static bool InParameterList;
	static bool InParameterEntry;
	static bool InParameterTerms;
	static bool InParameterDefs;
	static long ParameterListTermCount;
	static long ParameterListDefCount;

	static void StartTable(const char *outclass, const char *classatt, const char **atts);
	static void EndTable(void);
	static void SetTableDesc(char *title, DCgroup *gp);
	static void ProcTableCol(const char **atts);
	static void ProcPosList(const char *aval, bool row, long ttyp, const char *tsize);
	static void ProcTableColWidth(const char *tsize);
	static void EndTableCol(void);
	static void StartTableTitle(void);
	static void EndTableTitle(void);
	static void StartTableRow(const char **atts);
	static void EndTableRow(void);
	static void GetTableRow(void);
	static void StartTableCell(const char **atts);
	static void ProcCellCol(const char *name);
	static void AddTableCells(long count);
	static void EndTableCell(void);

	static DCnlist TableFormatList;
	static long TableFormCount;
	static long TableFormUseCount;
	//static char *BaseTableFormat;
	static char *NormalTableFormat;
	static char *SimpleTableFormat;
	static char *ChoiceTableFormat;
	static char *PropertiesTableFormat;
	static char *DefListTableFormat;
	static char *DefListTableColWidths;
	static char *ParamListTableFormat;
	static char *ParamListTableColWidths;
	static bool UseTableAnchor;
	static char *TableAnchorFormat;
	static DCclist TableFormats;

	static char *GetTableFormat(const char *outclass, const char *classatt);
	static void SetTableFormat(char *form);
	static long StoreTableFormat(const char *form);


	// in drxmlcn.cpp

	static char *ConrefBranch;
	static DCelem *LastConactElem;
	static bool ProcessingConrefs;
	static bool LastConactUsed;
	static bool ConrefPull;
	static bool ConrefsDone;

	static DCelem *CheckElemConact(DCelem *elem);
	static void SetConrefConds(DCcnref *cinfo);
	static void SetConrefPath(DCelem *elem, bool isMap);
	static bool FileIsMap(char *href);
	static char *GetGroupText(DCgroup *gp);
	static void ReplaceUniqueIDs(DCelem *elem);
	static void ReplaceIDsInGroup(DCgroup *gp);
	static void ReplaceIndexRefs(DCelem *elem);
	static void ReplaceIndexRefsInElem(DCpage *page, DCelem *elem);
	static void ReplaceIndexRefsInGroup(DCpage *page, DCgroup *gp, char *rstr);
	static void ReplaceIndexRefIDs(DCgroup *gp, char *rstr);
	static void ProcConrefFailure(DCcnref *cinfo, cnerrtyp err = noerr);

	static DCcnref *CurrConrefT2TInfo;
	static DCcnref *CurrConrefT2TReplacement;
	static bool ProcessingT2TConrefs;
	static long ConrefT2TCount;
	static long ConrefsT2TProcessed;
	static long ConrefsT2TNum;
	static DCvlist ConrefsT2T;
	static bool MakeConrefLists;
	static DClist ConrefAnumList;
	static DClist ConrefLOFList;
	static DClist ConrefLOTList;

	static void CheckTopicConref(DCelem *elem, bool isRoot);
	static void SetT2TConrefInfo(DCelem *elem, bool isRoot, bool push = false);
	static void ProcessT2TConrefs(XMLrfile *rf);

	static DCvlist ResolvedT2TConrefs;
	static DCvlist SortedT2TConrefs;
	static long ResolvedT2TConrefsCount;
	static void SetT2TConrefAction(void);
	static void DoT2TConrefAction(DCcnref *cinfo, long i);

	static bool GetTopicConrefTarget(XMLrfile *rf, DCcnref *cinfo,
	 DCelemset **targetset, DCelem **targetelem, cnerrtyp &err);
	static DCilist ConrefTopics;
	static bool GetFinalConrefTopic(long *docnum, long *topicnum);
	static void UpdateTopicTitle(DCtopic *top);
	static void ReplaceTopic(DCcnref *cinfo, long citem);
	static void ReplaceAnumListItems(DCcnref *cinfo);
	static void ReplaceT2TElement(DCcnref *cinfo);
	static void PushTopicElemAfter(DCcnref *cinfo);
	static void PushTopicElemBefore(DCcnref *cinfo);
	static void PushTopicElemAtStart(DCcnref *cinfo);
	static void PushTopicElemAtEnd(DCcnref *cinfo);
	static void InsertTopicElemText(DCcnref *cinfo, bool before);
	static void AddOneTopicElem(DCcnref *cinfo, bool before);

	static DCgroup *CurrTopicConrefTextGrp;
	static DCitem *CurrTopicConrefNxtTxt;
	static void ReplaceTopicElement(DCelemset *srctop, DCelem *srcelem,
	 DCtopic *desttop, DCtopelem *destelem, DCtopelem *newelem);
	static void AdjustConreffedElement(DCcnref *cinfo, DCtopelem *newelem);
	static void AddOneConrefAnum(DCcnref *cinfo, DCtopelem *newelem, DCelem *srcelem);
	static void AddConrefRangeElement(DCcnref *cinfo,
         DCtopelem *addelem, DCtopelem *srcelem);
	static void RecalcFormat(DCelem *elemorig, DCelem *elemnew, long lev = 0);
	static void ReplaceChildren(DCelem *elem);
	static void ReplaceChild(DCelem *elem, DCelem *parent, bool first = false);
	static DCgroup *GetAnumTextGroup(DCgroup *pgp);
	static void AdjustConrefAttrs(DCelem *eorig, DCelem *enew,
	                              DCgroup *newgrp);


	static DCcnref *CurrConrefM2TInfo;
	static DCcnref *CurrConrefM2TReplacement;
	static bool ProcessingM2TConrefs;
	static long ConrefM2TCount;
	static long ConrefsM2TProcessed;
	static DCvlist ConrefsM2T;

	static void SetM2TConrefInfo(DCelem *elem, bool push = false);
	static void ProcessM2TConrefs(XMLrfile *rf);
	static void ReplaceM2TElement(DCcnref *cinfo);


	static DCcnref *CurrMapConrefInfo;
	static DCcnref *CurrMapConrefReplacement;
	static bool ProcessingMapConrefs;
	static long MapConrefCount;
	static long MapConrefsProcessed;
	static DCvlist MapConrefs;

	static void CheckMapConref(DCelem *elem);
	static void SetMapConrefInfo(DCelem *elem, bool push = false);
	static void ProcessMapConrefs(XMLrfile *rf);
	static bool GetMapConrefTarget(XMLrfile *rf, DCcnref *cinfo,
	 DCelemset **targetset, DCelem **targetelem, cnerrtyp &err);
	static void CheckMapConrefReplacement(DCmapelem *elem, bool isRoot,
	 const char **pnavattr, int depth);
	static void ProcessTopicrefConref(DCmapelem *elem, bool mapref, bool replace);
	static void ReplaceMap(DCcnref *cinfo, long citem);
	static void ReplaceM2MElement(DCcnref *cinfo);
	static void ReplaceMapElement(DCelemset *srcmap, DCelem *srcelem,
	 DCmap *destmap, DCmapelem *destelem, DCmapelem *newelem);
	static void ReplaceChildren(DCmapelem *elem);
	static void ReplaceChild(DCmapelem *elem, DCmapelem *parent);


	static DCcnref *CurrConrefT2MInfo;
	static DCcnref *CurrConrefT2MReplacement;
	static bool ProcessingT2MConrefs;
	static long ConrefT2MCount;
	static long ConrefsT2MProcessed;
	static DCvlist ConrefsT2M;

	static void SetT2MConrefInfo(DCelem *elem, bool push = false);
	static void ProcessT2MConrefs(XMLrfile *rf);
	static bool GetT2MConrefTarget(XMLrfile *rf, DCcnref *cinfo,
	 DCelemset **targetset, DCelem **targetelem, cnerrtyp &err);
	static void ReplaceT2MElement(DCcnref *cinfo);


	// in drxmlan.cpp

	static void ProcIntRefs(void);
	//static void ProcExtRefs(XMLrfile *rf);

	static char *IDPathSep;
	static char *IDUpDir;
	static char *IDTopSep;
	static char *IDElemSep;
	static bool IDPath;
	static bool IDFile;
	static bool IDTopic;

	static char *MakeAnchorName(char *href, DCelem *einfo);

	static void ProcessLinks(void);
	static void ProcessXrefs(void);
	static DCtopelem *GetTopicTitle(DCtopic *tinfo);
	static DCtopelem *GetTopicDesc(DCtopic *tinfo);


	static bool InXref;
	static bool PendXref;
	static bool EmptyXref;
	static bool TitleXref;
	static bool XrefSet;
	static scopetyp XrefExt;
	static bool XrefDone;
	static DCgroup *XrefTitleGroup;
	static DCctl *XrefTitleStr;
	static DCnlist XrefFormNameDefs;
	static DCnlist XrefFormDefs;
	static long XrefFormDefCount;
	static char *XrefFile;
	static char *XrefID;
	static char *XrefHref;
	static char *XrefForm;
	static char *XrefBranch;
	static long XrefDepth;
	static char *XrefTextFormat;
	static char *XrefTitleFormat;
	static char *XrefNumFormat;
	static char *XrefFtnFormat;
	static abbrtyp XrefAbbr;


	static long XrefCount;
	static long XrefsProcessed;
	static xrefinfo *CurrXrefInfo;
	static DCvlist Xrefs;

	static DCnlist XrefRefs;
	static DCnlist XrefFormatNames;
	static DCnlist XrefElemPaths;
	static DCclist XrefFormatCtls;
	static DCclist XrefFileCtls;
	static DCclist XrefAnchorCtls;
	static DCclist XrefGroups;

	static long LinkCount;
	static long LinksProcessed;
	static linkinfo *CurrLinkInfo;
	static DCvlist Links;

	static DCnlist LinkRefs;
	static DCclist LinkFileCtls;
	static DCclist LinkAnchorCtls;

	static void StartXref(long depth);
	static scopetyp SetXrefScope(const char *scope, char *href);
	static void StartXrefContent(void);
	static void StartLinkText(char *dest, bool useform = true);
	static void SetLinkFormat(void);

	static char *XrefFileSuffix;
	static char *SetPeerLink(char *href);
	static void SetHyperlink(linkinfo *linfo, scopetyp ext, DCtopic *tinfo = NULL);
	static void EndXref(const char *name = NULL);
	static void SetXrefGroup(xrefinfo *xinfo);
	static char *SetFinalXrefForm(xrefinfo *xinfo, DCelem *einfo = NULL,
		                            bool title = false);
	static void SetXrefContent(char *fcp, DCelem *einfo, DCelem *etinfo);

	static void WriteXrefFormatDef(DCgroup *xgp, char *fcp, char *xform);
	static char *GetDefaultXrefFormatDef(char *xform);
	static char *GetDefaultXrefFormatName(DCelem *einfo);
	static char *GetXrefFormat(char *form);
	static char *SetXrefFormat(char *form, DCelem *einfo, char *epath);


	static bool InIndex;
	static bool InSee;
	static bool InSeeAlso;
	static bool InSortAs;
	static bool SortAsUsed;
	static bool NopageEntry;
	static char *IndexSeeStart;
	static char *IndexSeeAlsoStart;
	static char *IndexSeeEnd;
	static char *IndexSeeAlsoEnd;
	static uns IndexLevel;
	static uns IndexTopLevel;
	static DCgroup *CurrIndexGroup;
	static DCgroup *PreIndexGroup;
	static DCgroup *IndexLevelGroup;
	static DCclist IndexLevels;
	static DCgroup *IndexLevelSortGroup;
	static DCgroup *IndexLevelSeeGroup;
	static DCclist IndexSortLevels;
	static DCnlist IndexRangeList;
	static DCnlist IndexEndRangeList;
	static DCgroup *EndingForCurrIndexGroup;
	static DCgroup *PendingIndexItems;

	static void StartIndexItem(void);
	static void ProcIndexRange(char *start, char *end);
	static void ProcIndexItemText(void);
	static void CopyIndexEntry(DCgroup *gp, DCgroup *dp);
	static void RestartIndexItem(void);
	static void ProcIndexElemStart(void);
	static void ProcIndexSeeStart(char *start, char first);
	static void ProcIndexElemString(char *str);
	static void ProcIndexElemEnd(void);
	static void EndIndexItem(void);
	static void WriteIndexSort(void);


	static void ParseHref(const char *href, char **pfile,
	              char **ptopicid, char **pelemid = NULL);

	static bool UseFullPath;
	static char *FixReference(const char *hrefattr);
	static bool MatchToken(char *ntok, char *nend, char *elem, long elen);

	static char *WordTemplate;
	static bool WriteAnums;
	static bool SeqAnums;
	static long AnumTabWidth;
	//static char **CurrNumProps;
	//static char *CurrNumForm;
	static char *CurrNumName;
	static char *CurrNumRefName;
	static DCgroup *CurrAnumGroup;
	static DCnlist NumStreamNames;
	static DCvlist NumStreams;
	static long NumStreamIDs;
	static DCnlist NumFormatNames;
	static DCvlist NumFormInfos;
	static long NumberIDs;
	static DCnlist NumFormatText;

	static DClist AnumElems;
	static DClist *AnumElemList;
	static bool SetAnumTabs;

	static void ProcessAnums(void);

	static bool IsNumbered(const char *name);
	static void WriteTextNumPrefix(DCelem *elem, const char *oname,
		                             bool ftn = false);
	static void WriteANum(DCelem *elem);
	static numforminfo *GetNumFormat(char *name);
	static void GetNumFormatNameText(numforminfo *nf, char *name, long id);

	static char **DefNumSyms;
	static long DefNumSymCount;
	static numstrinfo *GetNumStream(char *strname);
	static void ResetStreams(resettyp reset);

	static void WriteTextNumPrefixForm(DCgroup *np, DCgroup *ap,
	                              numforminfo *nf, numstrinfo *sf);
	static void WriteFormatNumPrefix(const char *name, DCgroup *gp);
	static long AddNumFormDef(DCgroup *fp, char *fcp, numstrinfo *sf,
		                        long numid, bool ftn = false);
	static void WriteFormatStreamDefs(DCgroup *gp);

	static DCgroup *SetNumVal(nlevtyp ntyp, long nval,
	                          char **syms, long scount);
	static DCgroup *PutUniTextCtls(char *str);

	static bool InFootnote;
	static bool CurrFtnHasID;
	static bool UseFtnCalloutAttribute;
	static DCgroup *PreFootGroup;
	static DCgroup *PreFootTitleGroup;
	static char *CurrFtnForm;
	static char *CurrFtnCallout;
	//static char **FootnoteSymbols;
	//static long FootnoteSymbolCount;

	static char *FootnoteFormat;
	//static char *FootnoteNums[];
	//static char **FootnoteNumbering;
	//static char *FootnoteNumForm;
	//static char *FootnoteRefForm;
	//static nlevtyp FootnoteNtype;
	//static uns FootnoteReset;
	//static long CurrFootnoteSeq;

	static char *TblFootFormat;
	//static char **TblFootNumbering;
	//static char *TblFootNumForm;
	//static char *TblFootRefForm;
	//static nlevtyp TblFootNtype;
	//static uns TblFootReset;
	//static long CurrTblFootSeq;

	static void StartFootnote(DCtopelem *einfo, long depth);
	static void EndFootnote(void);
	//static void SetFootnoteInfo(bool tbl);
	static void WriteFootnoteStreamDefs(DCgroup *gp, uns id, bool tbl);


	// in drxmlrl.cpp

	static bool InRelLinks;
	static bool InLinkPool;
	static bool InLinkList;

	static bool AppendLinksToTopics;
	static bool GenerateALinks;

	static bool AddContentsLink;
	static bool AddLOFLink;
	static bool AddLOTLink;
	static bool AddGlossaryLink;
	static bool AddIndexLink;

	static char *AddedLinksFormat;
	static char *AddedLinksSpacer;
	static char *AddedLinksStart;
	static char *AddedLinksEnd;

	static bool UseAddedDivider;
	static char *AddedDividerFormat;
	static char *AddedDividerText;

	static bool GeneratePrevNext;
	static bool GenerateParentChild;
	static bool GenerateSiblings;

	static bool UsePrevNext;
	static bool UseParent;
	static bool UseChildren;
	static bool UseSiblings;
	static bool UseAncestors;
	static bool UseDescendants;
	static bool UseCousins;
	static bool UseFriends;
	static bool UseTopicTypes;

	static bool UseRelDescription;
	static bool UseRelDescAsTitle;

	static char *PrevHead;
	static char *NextHead;
	static char *ParentHead;
	static char *ChildHead;
	static char *SiblingHead;
	static char *AncestorHead;
	static char *DescendantHead;
	static char *CousinHead;
	static char *FriendHead;
	static char *RelatedHead;
	static char *ConceptsHead;
	static char *TasksHead;
	static char *ReferencesHead;

	static char *HeadInlineFormat;
	static char *HeadBlockFormat;
	static char *TopicTitleFormat;
	static char *ShortdescFormat;
	static bool UseRelatedDivider;
	static bool SearchRelatedLinks;
	static bool RelDividerSet;
	static char *RelatedDividerFormat;
	static char *RelatedDividerText;

	static DCvlist PrevLinks;
	static DCvlist NextLinks;
	static DCvlist ParentLinks;
	static DCvlist ChildLinks;
	static DCvlist SiblingLinks;
	static DCvlist AncestorLinks;
	static DCvlist DescendantLinks;
	static DCvlist CousinLinks;
	static DCvlist FriendLinks;
	static DCvlist ConceptLinks;
	static DCvlist TaskLinks;
	static DCvlist ReferenceLinks;
	static DCvlist RelatedLinks;

	static DClist *CurrChildLinks;
	static DClist *CurrSiblingLinks;
	static DClist *CurrAncestorLinks;
	static DClist *CurrDescendantLinks;
	static DClist *CurrCousinLinks;
	static DClist *CurrFriendLinks;
	static DClist *CurrConceptLinks;
	static DClist *CurrTaskLinks;
	static DClist *CurrReferenceLinks;
	static DClist *CurrRelatedLinks;

	static DCvlist TopicSequence;
	static DCnlist TopicSeqHrefs;
	static long TSeqCount;

	static void ProcessRelatedTopics(void);
	static long GetTopicSequence(void);
	static void ProcMapPageGroup(void);
	static DCgroup *ProcMapGroup(DCitem *item, DCgroup *mgp);
	static void SetSingleRelLink(rellink **prl, rellink *nl);
	static void SetRelLinkGroup(DClist **prlist, DClist *nlist);
	static void SetRelLinkGroupItem(DClist **prlist, rellink *rl);
	static seqtopic *FindSeqTopic(rellink *rl);
	static bool SeqInList(seqtopic *seq, DClist *lst);
	static void SetRelTopicAlinks(DClist *al, DCtopic *ctopic);
	static void SetReltableAlinks(void);
	static void SetOneTopicAlink(char *alid, DCtopic *topic);
	static DCtopic *FindSeqTopicHref(char *href);

	static void WriteRelatedLinks(DCtopic *topic, DCtopic *nexttopic);
	static void CombineRelLinks(rellinkinfo *rlinks, rellinkinfo *tlinks,
		                          DClist *topset, DCtopic *topic);
	static void RemoveRelLink(DClist *links, DCtopic *topic);
	static void MergeRelLink(DClist *links, DClist *merge, DClist *topset);

	static void WriteAlink(char *alname);

	static bool AddedLinksSet;
	static bool AddedLinksUsed;
	static DCgroup *AddedLinksGroup;
	static DCgroup *ContentsLinkGroup;
	static DCgroup *LOFLinkGroup;
	static DCgroup *LOTLinkGroup;
	static DCgroup *GlossaryLinkGroup;
	static DCgroup *IndexLinkGroup;
	static DCgroup *AddSpacerGroup;
	static DCgroup *AddStartGroup;
	static DCgroup *AddEndGroup;

	static void WriteAddedLinks(void);
	static void SetUpAddedLinks(void);
	static void SetOneAddedLink(DCgroup *gp, char *fanchor, char *txt);

	static void WriteRelLinkGroup(char *title, DClist *linklist);
	static void WriteTopicHeadChildren(DCgroup *txtgrp, DClist *linklist);
	static void WriteRelLinkGroupBody(DClist *linklist);
	static void WriteSingleRelLink(char *title, rellink *link);
	static void WriteRelLinkTitle(rellink *link);
	static void WriteRelLinkDesc(rellink *link);
	static void WriteRelDivider(void);

	static DCgroup *PreRelTableTextGroup;
	static DCvlist RelColAttrs;
	static DCvlist RelCellAttrs;
	static DCvlist RelRowTopicrefs;
	static long RelColCount;

	static DCnlist ReltableAlinkNames;
	static DCvlist ReltableAlinkRefs;
	static DClist *RelRowAlinkRefs;
	static long ReltableAlinkNameCount;

	static void ProcReltable(DCmapelem *elem, const char **atts, int depth);
	static void ProcRelRowEnd(void);
	static bool CheckRelLinking(DCmapelem *tref, bool source);
	static DCnlist RelTableLinkList;
	static void MakeRelLink(DCmapelem *tsrc, DCmapelem *targ);

	static DCgroup *PreLinkTextGroup;
	static rellink *CurrRelLink;
	static DCmapelem *CurrRelLinkTopicref;
	static DCnlist LinkRoles;
	static npair LinkRolesInit[];
	static DCnlist LinkScopes;
	static npair LinkScopesInit[];
	static DCnlist LinkTopicTypes;
	static npair LinkTopicTypesInit[];
	static DCnlist LinkingTypes;
	static npair LinkingTypesInit[];
	static DCnlist LinkForms;
	static npair LinkFormsInit[];
	static DCnlist LinkCollTypes;
	static npair LinkCollTypesInit[];

	static DCnlist RelAlinkNames;
	static DCvlist RelAlinkRefs;
	static DClist *RelTopicAlinkRefs;
	static long RelAlinkNameCount;


	static bool ProcRelatedLinks(DCtopelem *elem, const char **atts);
	static void SetElemRelLinkAlink(DCtopic *top, const char *idattr);
	static void InitRelatedLinks(void);
	static void AddSingleRelLink(rellink *rl, DCvlist *list);
	static void AddRelLink(rellink *rl, DClist **currlist, DCvlist *list);

	static roletyp GetRoleType(roletyp ltype, roletyp ttype);
	static bool CheckRole(roletyp role);

	static void AdjustRelAttrs(relattr *ra, relattr *rx);
	static void AdjustCellAttrs(relattr *ra, relattr *rcell, int depth);


	// in drxmlgr.cpp

	static bool InFig;
	static bool InAlt;
	static bool InDesc;
	static char *TopicDesc;
	static bool TreatTableFigAsTable;
	static char *GraphHref;
	static bool GraphInline;
	static long GraphWide;
	static long GraphHigh;
	static long GraphDepth;
	static long GraphTop;
	static long GraphLeft;
	static char *GraphAlt;
	static char *GraphDesc;
	static char *GraphicAlignment;
	static long GraphPos;

	static graphinfo *CurrGraphInfo;
	static DCvlist Graphics;
	static long GraphNum;

	static bool FigTitleAboveImage;
	static bool FigTitleAboveTable;
	static bool UseFigureAnchor;
	static char *FigureAnchorFormat;
	static DCgroup *FigTextGroup;
	static DCgroup *FigTextIDGroup;
	static DCgroup *FigTitleIDGroup;

	static void ProcFigStart(void);
	static void ProcFigEnd(void);
	static void ProcImageStart(void);
	static void ProcImageEnd(void);
	static void SetGraphAttrs(void);

	static bool InImageMap;
	static bool StartingImageMap;
	static bool InImageArea;
	static bool InImageShape;
	static bool InImageCoords;
	static bool InImageXref;

	static imapinfo *CurrImageMap;
	static imaparea *CurrImageMapArea;
	static DClist ImageMaps;

	static uns ImageMapCount;
	static uns ImageAreaCount;

	static DCnlist ImageMapShapes;
	static npair ImageMapShapesInit[];

	static void StartImageMap(void);
	static void EndImageMap(void);
	static void StartImageArea(const char *shapeatt, const char *coordsatt);
	static void ProcImageAreaCoords(char *coords);
	static void ProcImageAreaShape(char *shname);
	static void ProcImageAreaXref(char *href,
	             const char *format, const char *scope);
	static void EndImageArea(void);

	static void AddImageFrame(char *href, bool inl,
	                  long width, long height);
	static long SetGraphicFrames(DCpage *pg, long pgseq);
	static void RemoveGraphic(DCtopic *toporig, DCtopelem *elorig);
	static void ReplaceGraphic(DCtopic *toporig, DCtopelem *elorig, DCtopelem *elnew);

	static bool InObject;
	static bool InObjectDesc;
	static DCgroup *ObjectGroup;
	static uns ObjectNum;
	static char *ObjectDesc;
	static long ObjectDepth;

	static void StartObject(const char **atts);
	static void SetObjectParam(const char **atts);
	static void EndObject(void);



	// in drxmlmp.cpp

	static DCnlist MapElems;
	static DCvlist MapElemInfo;
	static DCmapelem *DefMapElemInfo;
	static long MapElemCount;
	static long MapElemsStarted;
	static DCilist MapAttrNumStartList;
	static DCilist MapAttrNumEndList;
	static DCnlist MapAttrNameList;
	static DCnlist MapAttrValList;
	static long MapAttrNum;
	static bool IsBookmap;
	static bool BookFrontMatter;
	static bool BookFrontList;
	static bool BookChapter;
	static bool BookAppendix;
	static bool InReltable;
	static bool InMapTopicmeta;
	static bool InCodedocref;
	static bool InExternDoc;
	static bool TopicheadsHaveNavtitles;
	static bool LockAllNavtitles;
	static bool UseAllInTOC;
	static bool SkipMapRef;
	static long SkipMapRefDepth;
	static DCmapelem *CurrTopicref;
	static DCmapelem *CurrDocref;

	//static bool IncludeMaps;
	static bool UseMapDescriptions;
	static long MapDescDepth;
	static long MapLevelAdjust;
;
	static bool SplitTopicFiles;
	static bool DisableChunking;
	static bool LastChunkToCont;
	static bool SplitThisTopic;
	//static char *TOCFileName;
	static DCgroup *PreTopicmeta;
	static DCgroup *PreNavtitle;
	static DCgroup *MapNavtitle;
	static DCgroup *PreDocText;

	static char *BaseMapFormat;
	static char *LinkFormat;
	//static char *MapXrefFormat;
	//static char *MapLinkFormat;
	static bool InResTopic;
	static bool ChunkDoc;
	static bool ChunkDocSet;
	static bool ChunkTopic;
	static char *ChunkBy;
	static char *ChunkSel;
	static DCnlist ChunkByTypes;
	static npair InitChunkByTypes[];
	static DCnlist ChunkSelTypes;
	static npair InitChunkSelTypes[];


	static void StartMapElement(const char *name, const char **atts,
	 int depth, bool isRoot);
	static bool IsDocMap(const char *href);
	static void SetMapInfo(const char *name);
	static void SetMapElemInfo(const char *name, const char *classatt,
	  const char *idatt, const char *outclass, const char *hrefatt,
		const char *conrefatt, const char *conendatt,
	  const char *conactatt, const char *keyrefatt,
		const char *conkeyrefatt, const char *idrefatt, const char *idrefsatt,
		long depth, bool isRoot);
	static void SetMapFormat(DCmapelem *elem, int depth, int level);
	static void ProcTopicref(DCmapelem *elem, bool mapref, long level);
	static void ProcTopicrefText(DCmapelem *elem, int depth, long level,
		     const char *scope, bool locktitle, const char *txt, long len);
	static void ProcNestedTopicMapGroup(int depth, const char *refid);
	static bool TreatTopicheadsAsTopics;
	static long TitleOnlyNameVer;
	static char *TitleOnlyTopicType;
	static char *TitleOnlyTopicID;
	static bool RecordShortdesc;
	static long TopicHeadNum;
	static bool AddTopicHeadChildren;
	static char *TopicHeadChildHead;
	static char *TopicHeadChildHeadFormat;

	static void CreateTitleOnlyTopic(void);
	static char *CorrectFileName(char *name);

	static bool InKeyDef;
	static long CurrKeyCount;
	static long LastKey;
	static long KeyDefForm;
	static DCnlist Keys;
	static DCnlist KeyLinks;
	static DCnlist KeyIDRefs;
	static DCnlist KeyOutclasses;
	static DCnlist KeyConrefs;
	static DCvlist KeyTopicmeta;

	static DCvlist Keydefs;
	static long PendMapKeyrefs;
	static DCnlist PendingMapKeyrefs;
	static DCnlist PendingMapConkeyrefs;
	static DCvlist PendingMapKeyrefElems;

	static void ProcKeydef(DCelem *elem);
	static bool UseBranchKeys;
	static bool KeydefsOnlyWithinBranch;
	static DCnlist BranchKeys;
	static char *KeyrefBranch;
	static void AddKeydef(const char *nkey, char *branch);

	static void ResolveKeydefs(void);
	static long GetMapElemKeyID(DCmapelem *elem, char *key, bool conref = false);
	static void ResolveMapKeyrefs(void);
	static void ResolveKeyref(DCtopelem *elem);

	static bool InMapDescGroup;
	static bool UseNestedTopicsInTOC;
	static bool MapDescNeeded;
	static void StartMapGroup(long level, tseqtyp typ = sqnone);
	static void EndMapGroup(tseqtyp typ = sqnone);
	static void CheckMapShortdesc(DCmapelem *elem);
	static void SetTopicShortdescInMap(DCtopic *cti);
	static void StartMapShortdesc(DCmapelem *elem);
	static void EndMapShortdesc(void);
	static void PutMapHeadID(DCmapelem *elem);
	static void SetElemUID(DCelem *elem);

	static char *ChunkTok[ChunkTokMax];
	static void SetChunkVal(const char *chunk);


	// in drxmlin.cpp

	static secinfo XMLIniSects[];

	static setinfo ISecOptions[];
	static void CaselessMatchSet(char *);
	static void SpacelessMatchSet(char *);
	static void WildcardMatchSet(char *);

	static setinfo ISecChunking[];
	static setinfo ISecFigure[];
	static setinfo ISecTable[];
	static setinfo ISecKey[];
	static setinfo ISecTOC[];
	static setinfo ISecLOF[];
	static setinfo ISecLOT[];
	static setinfo ISecIDX[];
	static void UseCompactFormSet(char *);
	static void FullIndexRangesSet(char *);
	static setinfo ISecGLS[];
	static setinfo ISecABR[];
	static setinfo ISecTMK[];
	static setinfo ISecTopicHeads[];
	static setinfo ISecMapOptions[];
	static setinfo ISecMapGen[];
	static setinfo ISecWordOptions[];
	static setinfo ISecHTMLOptions[];

	static setinfo ISecSetup[];
	static void XrefFileSuffixSet(char *);
	static setinfo ILogOptions[];
	static setinfo ISecFootnotes[];
	static setinfo ISecIndexSee[];
	static setinfo ISecCondOptions[];
	static setinfo ISecFilterConditions[];
	static setinfo ISecRelTopics[];
	static setinfo ISecElements[];
	static setinfo ISecIDOptions[];

	static secinfo LangIniSects[];
	static setinfo ISecNoteText[];
	static setinfo ISecTopicHeadText[];
	static setinfo ISecTitlePgText[];
	static setinfo ISecTOCText[];
	static setinfo ISecLOFText[];
	static setinfo ISecLOTText[];
	static setinfo ISecIDXText[];
	static setinfo ISecGLSText[];
	static setinfo ISecRelTopicsText[];
	static setinfo ISecTaskHeadText[];
	static setinfo ISecElementText[];
	static setinfo ISecMapText[];


 public:

  // in drxml.cpp, called from main()

	static char *DRxmlVer;
	static swlist SwitchList[];
	static bool FormatSet;
	static bool PrintProject;
  static short Verbose;
  static char *IniFileName;

	static DCirfile *IniFile;
	static XMLrfile *XMLFile;
	static DCLwfile *DCLFile;
	static DCirfile *LangFile;
	static DCfrfile *FmtFile;
	static DCfrfile *SubFormatsFile;
	static bool OutputHTML;
	//static bool OldNumbering;
	//static bool OldXrefs;
	static bool StartingAuto;
	static bool BookAuto;

	static bool DoAutomation;
	static char OutType;
	static char HelpType;
	static char FormType;
	static char OldFormType;
	static bool UMXL;
	static char *OutTypeNames[];
	static char *OutTypeName;
	static bool IsRtf;

  static void SetUMXL(char *fmt);
  static void SetFileFormat(char *fmt);

	static bool InitCurrProps(char *rfname);
  static void SetUpPartFiles(DCLwfile *wf);

	static char *CurrFile;
  static void ParseXML(XMLrfile *rf);
  static void WriteDCL(DCLwfile *wf);
};


#endif


