
// drmif.h is the header for dcl FrameMaker MIF reading, JHG, 4/21/97
// Copyright (c) 1993-1997 by Omni Systems, Inc.  All rights reserved.

#ifndef DRMIF_H
#define DRMIF_H


class MIFrd;
class MIFrfile;


enum RmifConstants {
  NumStyleMax = 7, StdArrowMax = 8, LevelMax = 16, TSLeadMax = 20,
  FStringMax = 80, LineMax = 4096, AnsiTabMax = 128,	ColorsMax = 4,
  RStringMax = 256, MathFullMax = 32767,
	// TextBufMax = 1024,
  LineSegsMax = 32, ElemIdLen = 8, NumLenMax = 10,
  MarkTypes = 11, MarkHyperTypes = 15,
	CharSymsMax = 21,  RefBufLen = 1024, MaxIniLen = 65500
};


// mapping of mif to dcl

enum dc_mtyp {  // argument type in mif statement
  empty, newgroup,
  stringval, fileval, nameval, unameval, vdefval, mathval, noteval,
  numval, fillval, separ, colorval, emval,
  sizeval, ptval, arectval, brectval, srectval, // all in page_units
  leadval, leadrect,       // in lead_units
  fontval,    // in font_units
  glineval,   // in line_units
  yorn,
  listword,                // marks start of wordlist identifiers
  l_units, l_cstate, l_cstyle,
  l_palign, l_pplace, l_plstyl, l_shplc, l_pspace, l_plang, l_tstype,
  l_fline, l_fcase, l_fposit,
  l_numstyle, l_pgparity, l_cbarpos, l_pgtype, l_pgorient,
	l_tipgspc, l_tiform, l_ticode, l_chname,
  l_fnstart, l_fnbase,
  l_tbplace, l_tcalign, l_ttplace,
  l_frtype, l_gralign, l_grcap, l_grarrow, l_mathsiz,
  list_end                 // marks end of wordlist identifiers
};

enum dc_styp {  // special processing required for statement
  ignore, none, pseudogroup, dclgroup, new_file, unit_set,
  mif_id, mif_rect,	mif_uniq,
	color_def, color_tag, color_val,

  styp_tx,  // listed in ProcTx()
  cond_def, cond_tag, cond_style, cond_color,
	cond_start, cond_use, cond_end,
  pform_def, pform_tag, palign, pplace, plstyl, prunin,
	pspbef, pspaft, pkprev, pknext, pblock, ptsep, pbsep,
	phyuse, phyline, phybef, phyaft, phyword,
  next_utag, next_tag, plind, pfind, prind,
  pspc_mode, pspc_val, lspace, wspmin, wspopt, wspmax, plang,
  tscount, tstop, tspos, tstype, tslead, tsalign,
  cform_def, cform_tag, pform_font, f_plat, f_size,
  f_string, f_line, f_case, f_posit, f_bool, f_enc,
  fkern, fsepar, fcolor, fspread, fvpos, fhpos,
  docgroup, psided, pparity,
  new_page, pg_type, pg_name, pg_back, pg_num, pg_size,
  fr_def, fr_next, fr_side,
  tflow, tauto, tpost, tftag,
  hidend, hidfull,
  tinset, tiname, tifile, tiimph, tiaupd, tilupd, tilock, tinend,
  ticlnt, ticlname, ticlfile, ticltype, ticldata,
  tiflow, timflw, tipgsp, tiftag, tiform, tinovr, tinpgb,
  titext, titeol, titcod,
  tittbl, titbfm, titbrow, titbcols, titbcsep, titbnsep, titbnhdr,
  titbehdr, titbcod,
  fnotes, fnote, para, pline, tframe, text, tchar,

  styp_an,  // listed in ProcAn()
  panum, panform, panfont, panpos, panval,
  var_def, var_tag, var_form,
  xref_def, xref_tag, xref_form, xref_use, xref_text, xref_file, xref_val,
  xref_elem, elem_beg, elem_tag, elem_end, elem_attr, elem_all,
  attr_beg, attr_name, attr_val,
  marker, markcat, marktyp, markname, marktxt, markpg,
  pfirst, pnstyle, bkchars,
	volfirst, volstyle, voltext, chfirst, chstyle, chtext,
	secfirst, secstyle, sectext, ssecfirst, ssecstyle, ssectext,
  fnfirst, fnstyle, fnname, fnstart, fnhigh, fnchars, tfnname,
  fnbase, fnpre, fnsuf, fntbase, fntpre, fntsuf,

  styp_tb,  // listed in ProcTb()
  rugroup, runame, rucolor,
  tbform, tbname, tbcol, tbcoln, tbwcol, tbncol,
  tcmarg, tbplg, tbalign, tbplace, tbblock,
  tbhprop, tbfprop, tbhfprop, tbhfcolor, tbbprop, tbbcolor,
  tbrul, tbxcnum, tbxcrul, tbxcolor, tbbrul, tbhfrul,
  tbtplace, tbanum,
  tbggroup, tbid, tbrtyp, tbrow, tbcell, tbcang, tbccon, tbccolor,
  atbl, tcalign, tclmt, tcrmt, tctmt, tcbmt,

  styp_gr,  // listed in ProcGr()
  mif_gid, ggroup,
  gframe, frtype, frname, frfloat, frnsoff, frbloff, frrungap,
	fralign, frcrop, aframe,
  objprop, propname, propval,
  gobj, gpoints, grpoint, smooth, radius, arctheta, arcdtheta,
  gtext, tlpoint, tlalign, tlrot,
	mathobj, mathff, mathbk, mathorg, mathalgn, mathsiz,
  impobj, impname, gres, gflip,
  arstyle, artang, arbang, arlen, arhead, arscale, arfactor,
  gpen, gpwid, gfill, gsepar, gcolor, lsegpat, lineseg, lsegcount,
  headcap, tailcap
};

enum psg_id {  // identifiers for pseudogroups
  psg_base = 0, psg_doc, psg_cond, psg_pgf, psg_font,
  psg_rule, psg_tbl, psg_var, psg_xref,
  psg_book, psg_dict, psg_aframe, psg_atbl,
  psg_ref, psg_pgdef, psg_page, psg_tflow,
  psg_anum, psg_fnote, psg_exfile, psg_color, psg_last
};

struct mif {
  char *name;     // mif name, such as "TextFlow"
  dc_mtyp mtype;  // type of operation needed for it
  dc_dtyp dtype;  // type of dcl ctl to produce
  uns maj;        // majortype for dcl ctl, 0 for pseudogroup
  uns min;        // minortype for ctl; if maj is 0, min is index in pg list
  uns dat;        // 0 or subtype
  dc_styp spec;   // special operation
};

enum mifunit {
  inch, cm, mm, pc, pt, dd, cc, umax, percent
};

enum dc_ucat {
  page_u = 10, lead_u, font_u, gline_u
};

struct nvlist {
  char *n;
  long v;
};

struct vdata {
  char *p;
  size_t len;
  char pre;
  char val;
};

struct tab {
  uns ttype, talign, tfill;
  char *tfstr;
  long tpos;
};

struct alevel {
  uns lnum;
	uns lval;
  bool incr;
  bool set;
  bool show;
  uns setval;
  uns incval;
  alevel *next;
};

struct anum {
  char name;
  char *fnt;
  uns levels;
  uns seq;
  alevel *next;
};

struct cform {
	long fsize;
	uns kern;
	uns color;
	long spread;
	long fnum;
	char *fname;
	uns compr;
	uns weight;
	uns angle;
	uns basel;
	uns caps;
	uns style;
	uns rev;
	uns lining;
	bool sym;
};

struct pform {
	uns pnum;
	uns align;
	char *fnext;
	long find;
	long lind;
	long rind;
	uns pplace;
	uns pstyle;
	char *runin;
	long sbval;
	long saval;
	unc kprev;
	unc knext;
	uns kwid;
	cform *fnt;
	uns tcount;
	DCvlist *tabs;
	long slval;
	unc sltype;
	unc hyuse;
	uns hymline;
	uns hymbef;
	uns hymaft;
	uns hymword;
	uns cspls;
	uns wsmin;
	uns wsopt;
	uns wsmax;
	uns lang;
	long cplmval;
	long cprmval;
	long cptmval;
	long cpbmval;
	unc cplmtyp;
	unc cprmtyp;
	unc cptmtyp;
	unc cpbmtyp;
	unc tcalgn;
	DCgroup *px;
	DCgroup *sx;
};

struct point {
  long x, y;
};

struct rect {
  long l, t, w, h;
};

struct arrow {
  long tip, base, len, head;
};

struct gtprop {
	char *fname;
	long fsize;
	uns fbold;
	uns fital;
	uns fundl;
	uns fstrike;
	uns fnum;
	uns color;
	bool fsym;
};

struct grcform {
	char *fname;
	long fsize;
	uns fbold;
	uns fital;
	uns fundl;
	uns fstrike;
	uns fnum;
	uns color;
	bool fbset, fiset, fuset, fsset;
	bool fsym, fsymset;
};

enum rastyp {
  norast, rf, bmp, tif, gif, pcx, jpg, png
};

enum vectyp {
  novect, fmv, wmf, pct, wpg, cdr, ole
};


// class for mif parsing vars and funcs


class MIFrd {
	static char ObjectIDs;
	static long CurrObjectID;
	static char XrefType;
	static char Mark11;
	static bool RemoveWordTocMarkers;
	static char ImpGraph;
	static bool ExFiles;
	static char EpsiUsage;
	static bool KeepEpsi;
	static bool EpsiPreview;
	static uns DirectiveMark;
	static uns ALinkMarkerType;
	static uns ExtraIXMarkerType;
	static char *ExtraIXMarkerName;
	static char *ExtraIXPrefix;

	static bool CaselessMatch;
	static bool SpacelessMatch;
	static bool WildcardMatch;
	static char *IDFileName;
	static char *PrjFileName;
	static char GrExFormat;

	static bool UseLog;
	static bool ShowLog;
	static char *LogFileName;
	static char *HistoryFileName;
	static char *EditorFileName;
	static DClog *Log;
	static unc LogLevels[logmax];
	static bool LogIniChains;
	static void LogEvent(logtype lt, unc sev, const char *desc,
	              const char *d2 = NULL, const char *d3 = NULL,
	              const char *d4 = NULL, const char *d5 = NULL);

	static char *MacroFile;

  static mif mifpile[];
  static size_t mifpile_size;
  static size_t miflist_size;
  static mif **miflist;
  static char *wordpile[];
  static size_t wordlist_size;
  static cpp *wordlist;
  static char *YesNoList[];

  static unc MIFversion;
  static char *FMversion;
  static char *MIFstring;
  static mifunit MIFunit;
  static long MIFval;
  static point MIFpt;
  static rect MIFrect;
	static bool ConvertToEms;
  static DCblock *MIFraster;
  static DCblock *MIFvector;
  static DCblock *MIFepsi;
  static rastyp MIFrastype;
  static vectyp MIFvectype;
  static long MIFlines;
	static bool MultipleOLE;
	static DClist OLEimages;
	static long OLEimageCount;

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

  static void (*EndProc[LevelMax])(void);

  static DCgroup *GroupStack[LevelMax];
  static DCctl *CtlStack[LevelMax];
  static int MIFlevel;
  static DCctl *MakeCtl(mif *mp, bool attach = true);

  static uns CurrPG;
  static DCgroup *GroupList[psg_last];
  static void MakePseudo(psg_id pgnum);
  static void MakeDclGroup(mif *mp);
  static void ProcUnique(mif *mp);
  static void ConvertStringName(bool vdef);
  static void ConvertFileString(void);

  static uns M2Dunits[umax];
  static uns Upage;
  static uns Ulead;
  static uns Ufont;
  static uns Ugline;
  static void ProcUnits(void);
  static void MakeUnits(dc_ucat umin, uns dat);
  static void ConvUnits(dc_ucat utype);
  static void ConvPt(dc_ucat utype);
  static void ConvRect(dc_ucat utype);

  static uns CurrID;
  static rect CurrGrRect;
  static void ProcGrID(void);
  static void ProcGrRect(void);

	static double ColorVal;
	static char *ColorTag;
	static long ColorVals[ColorsMax];
	static DCnlist ColorDefs;
	static long LastColor;
  static void ProcColorDef(void);
  static void EndColorDef(void);
  static void ProcColorTag(void);
  static void ProcColorVal(short num);
  static void ProcColor(void);

  static long LookUp(char *str, cpp slist);

  static void CheckLogErrors(void);

	static char RefBuf[RefBufLen];
	static char *GetFileID(char *name);
	static bool IncAlpha(char *str);
	static bool FileIDUsed(char *id);
	static bool FindIDFile(void);


	// in drmifin.cpp
	static secinfo MIFIniSects[];

	static setinfo ISecOptions[];
	static void CaselessMatchSet(char *);
	static void SpacelessMatchSet(char *);
	static void WildcardMatchSet(char *);

	static setinfo ISecFDK[];
	static void GraphicNameDigitsSet(char *);
	static void GrExFormatSet(char *fname);

	static setinfo ISecGraphics[];
	static setinfo ISecGraphExport[];
	static void ImpGraphSet(char *);
	static void MultipleOLESet(char *);

	static setinfo ILogOptions[];


  // in drmiftx.cc

  static mif miftxpile[];
  static size_t miftx_size;
  static char *wordtxpile[];

	static long TextInsetNum;
	static long NextInsetNum;
	static long TextInsetLev;
	static DCilist TextInsetStack;

  static void ProcTx(mif *mp);

  static uns CondID;
  static DCnlist CondNameList;
  static DClist *OldConds;
  static DClist *NewConds;
  static DClist *RowConds;

  static void ProcCondDef(mif *mp);
  static void ProcCondTag(mif *mp);
  static void ProcCondStyle(mif *mp);
  static void ProcCondStart(void);
  static void ProcCondUse(mif *mp);
  static void EndCondStart(void);
  static void ProcCondEnd(uns lev);

  static uns FormatID;
  static uns CurrPForm;
  static DCnlist FormNameList;
  static DCnlist FormNSPNameList;
  static DCnlist FormTagNameList;
  static DCnlist CFormNameList;
  static DCnlist CFormNSPNameList;
  static DClist CFTagList;
  static DCilist FormTypeList;
  static DCilist FormSymList;
  static bool InPgf;
  static bool PgfTagged;
  static bool PgfTagSeen;
  static bool TagPF;
  static bool TagCF;
  static bool CFTagUsed;
  static bool AFFontDef;
  static bool Inheriting;

	static pform CurrPgf;
	static cform CurrFnt;
	static cform DefPgfFnt;
	static DCvlist PFormList;
	static DCctl *CurrPTagCtl;
  static DCnlist GenFileParaFmts;
	static npair GenFileParaFmtsInit[];

  static void ProcPForm(mif *mp);
  static void EndPForm(void);
  static void ProcPTag(mif *mp);
	static DCnlist UndefParaForms;
	static long UndefParaFormsCount;
  static char *NoSpace(char *nm);
  static void CheckParaFormName(char *pname);
  static void PutPFProps(void);
  static void PutPgfOverrides(pform *pf);
  static void PutTabStop(void *vp, long);
  static void ProcPPlace(mif *mp);
  static void ProcPStyle(mif *mp);
  static void ProcPRunIn(char *punct);
  static void ProcPBlock(mif *mp);

  static uns PSpMode;
  static void ProcPSpVal(mif *mp);

  static uns TSCount;
  static uns TSLeft;
  static uns TSType;
  static uns TSFill;
  static uns TSAlign;
  static long TSPos;
  static uns LineTabCount;
  static uns CurrTabFill;
  static uns CurrTabAlign;
  static bool GenericTab;
  static char *CurrTFString;
  static tab *CurrTab;
  static DCvlist *CurrTabList;
  static DCvlist TabFormatList;

  static void ProcTSCount(mif *mp);
  static void EndTStop(void);
  static void ProcTSLead(mif *mp);
  static void MakeTabCtl(void);

  static void ProcCForm(mif *mp);
  static void ProcCTag(mif *mp);
  static DCnlist UndefCharForms;
  static long UndefCharFormsCount;
  static void CheckCharFormName(char *pname);
  static void ProcFForm(void);
  static void SetEFont(void);
  static void PutFontProps(void);
  static void PutFntOverrides(cform *cf);
  static void ProcVPos(mif *mp);
  static void ProcHPos(mif *mp);

  static char FFamily[FStringMax];
  static char FVar[FStringMax];
  static char FWeight[FStringMax];
  static char FAngle[FStringMax];
  static char FName[FStringMax];
  static char FEncode[FStringMax];
  static uns FFNum;
  static uns FVNum;
  static uns FWNum;
  static uns FANum;
  static uns FENum;
  static nvlist FontFamilies[];
  static nvlist FontVars[];
  static nvlist FontWeights[];
  static nvlist FontAngles[];
  static nvlist FontEncoding[];
  static void ProcFPlat(void);
  static void ProcFString(mif *mp);

  static uns FBase;
  static uns FStyle;
  static uns FCaps;
  static uns FRev;
  static uns FLine;
  static bool FBaseSet;
  static bool FStyleSet;
  static bool FCapsSet;
  static bool FRevSet;
  static bool FLineSet;
  static bool SymFont;
  static bool NoSymMap;
  static bool Asian;
  static void ProcFBool(mif *mp);
  static void EndFont(void);
  static void SetSymFont(long fval);
  static void PutFntProp(void *vp, long);

  static uns PageID;
  static uns LastMasterPage;
  static uns FrameID;
  static uns FirstPageFrame;
  static uns PageHand;
  static uns MasterType;
  static uns LeftMaster;
  static uns RightMaster;
  static bool DoubleSided;
  static bool FirstRight;
	static long PageWide;
	static long PageHigh;

  static DCilist FrameIDList;
  static DCnlist FrameNameList;
  static DCnlist PageNameList;
  static DCilist PageNumList;
	static DClist PageDoneList;
  static DCilist MasterTypeList;
  static DCclist FrameTextList;
  static DCclist FrameNextList;

  static DCilist FramePageList;
  static DCclist PageFrameIndex;
  static DCclist PageUsageIndex;
  static DCclist FrameUsageIndex;
  static DCclist PageTextIndex;
  static DCclist PagelessAFTextIndex;
  static DCclist PagelessAFTextStart;
  static DCclist PagelessTextIndex;
  static DCclist NestedAFIndex;
  static DCclist NestedAFTextID;

  static DClist FrameHeaderList;
  static DClist FrameFooterList;
  static DClist PageHeaderList;
  static DClist PageFooterList;

  static DCclist MPFrameMTList;
  static DCilist *MPFrameTList;
  static DCilist *MPFrameHList;

  static DCilist FrameAFList;
  static DCilist PageAFList;

  static void ProcPage(mif *mp);
  static void EndPage(void);
  static void ProcPageType(mif *mp);
  static void ProcPageName(mif *mp);
  static void ProcTFrame(mif *mp);
  static void EndTFrame(void);
  static void ProcTFNext(mif *mp);

  static uns TextID;
  static uns CurrTextID;
  static uns CurrTFrame;
  static uns CurrPage;
  static bool ParaStarting;
  static bool TFAuto;
  static bool TFPost;
  static bool FlowTagged;
  static char TFTag[FStringMax];

  static bool HiddenPage;
  static bool HiddenFlow;
  static bool HiddenEnd;
  static bool HiddenFull;

  static DCctl *FirstTFCtl;
  static DCctl *FirstPGCtl;

  static void ProcTextFlow(mif *mp);
  static void EndFlow(void);
  static void ProcPara(mif *mp);
  static void EndPara(void);
  static void ProcTLine(void);
  static void EndTLine(void);
  static void ProcTFRef(mif *mp);
  static void ProcHiddenMarker(void);

  static void ProcNotes(void);
  static void EndNotes(void);
  static void ProcFNote(mif *mp);
  static void EndFNote(void);

  static unc ANSItab[AnsiTabMax];
  static uns CharSymList[CharSymsMax];
  static uns CharList[CharSymsMax];
  static void ProcTextObj(void);
  static void ProcHex(char **cpp);
	static unl C1Ctrls[32];
	static unl CheckC1CtrlSyms(unl uch);
  static void ProcChar(void);

  static size_t TextPos;
  static size_t LastLinePos;
  static DCctl *TextObjLit;
  static DCblock *TextBuf;
  static void AddText(unl ch = 0);
  static void FlushText(void);


  // in drmifan.cc

  static mif mifanpile[];
  static size_t mifan_size;
  static char *wordanpile[];

  static void ProcAn(mif *mp);

  static uns VarID;
  static DCnlist VarNameList;
  static vdata VarTime[];
  static vdata VarBlocks[];
  static void ProcVarTag(mif *mp);
  static void ProcVarForm(mif *mp);
  static void ProcVXForm(DCgroup *gp, bool isVar = false);
  static int FindVarBlock(char *vn);

  static uns XrefID;
  static DCnlist XrefNameList;
  static DCvlist XrefFormList;
  static bool XrefEmpty;
  static bool InXrefText;
  static DCgroup *XrefParentGroup;
  static DCitem *XrefParentLast;
	static DCctl *XrefFileCtl;
	static DCctl *XrefTextCtl;
  static void ProcXrefTag(mif *mp);
  static void EndXrefDef(void);
  static void EndXrefStart(void);
  static void ProcXrefInstance(DCgroup *instg, DCgroup *formg);
  static void FixXrefFileName(DCctl *xfname);
	static char *FixReference(const char *hrefattr);

	static bool BeginElement;
	static bool ElemAttr;
	static bool IDAttr;
	static bool IDRefAttr;
	static char *IDAttrName;
	static char *IDRefAttrName;

  static void EndElement(void);
  static void EndElementBegin(void);
  static void EndAttribute(void);

	static bool InMarker;
	static bool InHiddenMarker;
  static uns MarkType;
  static uns MarkHyperType;
	static bool InMarkCat;
	static char *MarkName;
	static DCgroup *IndexGroup;
	static DCnlist AttrMarkers;
	static char *AttrMark;
	static char *AttrMarkVal;
  static uns MarkTList[MarkTypes];
  static char *MarkNames[MarkTypes];
  static char *MarkHyperNames[MarkHyperTypes];
	static void StartMarkCat(void);
  static void EndMarkCat(void);
	static void StartMarker(void);
  static void EndMarker(void);
  static void ProcAttrMarker(void);
  static void GetMarkType(void);
  static void ProcMarker(mif *mp);
  static void ProcMarkText(mif *mp);
  static void CheckMarkerSetting(char *txt);
  static void ParseHyperFileName(char* &str, bool open = false);

	static char *DMAppName;
	static char *DMSectionName;
	static char *DMPropertyName;
	static char *DMPropertyValue;
	static bool LocalApp;
  static DCgroup *GraphicsDirectives;
  static void ProcDirectiveMarker(void);

  static void EndDocGroup(void);

  static bool PNumUsed;
  static bool PNumEnd;
	static bool PTSepUsed;
	static bool PBSepUsed;
  static char PTSep[FStringMax];
  static char PBSep[FStringMax];
  static char PAForm[FStringMax];
  static char PAFont[FStringMax];
  static char PAVal[FStringMax];
  static bool PAFontSet;
  static DCclist RFrameRefList; 
  static DCnlist RFrameNameList;
  static uns RFRefNum; 
  static uns PARefNum;
  static uns AnumDefID;
  static DCclist AnumRefList; 
  static DCclist AnumFNRefList; 
  static DCvlist AnumStructList; 
  static DCilist AnumPFormList; 
  static DCilist AnumDefList; 
  static DCclist AnumDefCList; 
  static DCclist AnumLevCList; 
  static DCilist AnumNameList; 
  static bool AnumsDefined;
  static DClist ChapnumRefList; 
  static DClist ChapnumStructList; 
  static DClist VolnumRefList; 
  static DClist VolnumStructList; 
  static DClist SectnumRefList; 
  static DClist SectnumStructList; 
  static DClist SSectnumRefList; 
  static DClist SSectnumStructList; 

  static void ProcPFCont(void);
  static void MakePFrame(DCgroup *gp, char *fname);
  static void MakePADef(DCgroup *gp);
  static void MakeANDefs(void);
	static DCnlist AnumCharNotInCatalog;
	static long AnumCharNotInCatalogCount;
  static void ProcPAVal(void);

  static long FirstPage;
  static uns PNumStyle;
  static char NumStyles[NumStyleMax];
  static long FirstVol;
  static uns VolStyle;
	static char *VolText;
  static long FirstChap;
  static uns ChapStyle;
	static char *ChapText;
  static long FirstSect;
  static uns SectStyle;
	static char *SectText;
  static long FirstSSect;
  static uns SSectStyle;
	static char *SSectText;
  static void MakePNDef(void);
  static void SetChap(long i);
  static void SetChapStruct(long i);
  static void SetVol(long i);
  static void SetVolStruct(long i);
  static void SetSect(long i);
  static void SetSectStruct(long i);
  static void SetSSect(long i);
  static void SetSSectStruct(long i);
  static void MakeNumDef(long val, long typ, uns sty, char *txt = NULL);

  static uns FirstFN;
  static uns FNStyle;
  static char FNName[FStringMax];
  static char TFNName[FStringMax];
  static uns FNReset;
  static uns FNHigh;
  static char FNChars[FStringMax];
  static DCgroup *FNCharGroup;
  static uns FNBase;
  static char FNPre[FStringMax];
  static char FNSuf[FStringMax];
  static uns FNTBase;
  static char FNTPre[FStringMax];
  static char FNTSuf[FStringMax];
  static bool InNotes;
  static DCclist *FNoteList;
  static uns FNType;

  static void MakeFNProp(void);
  static void MakeFNDef(long typ = 2);
  static void MakeFNFormat(DCgroup *fgp, uns Base, char *Pre, char *Suf);
  static void ConvertFString(char *p);
  static DCgroup *ProcCharList(mif *mp);


  // in drmiftb.cc

  static mif miftbpile[];
  static size_t miftb_size;
  static char *wordtbpile[];

  static void ProcTb(mif *mp);

  static DCnlist RuleNameList;

  static void ProcRForm(mif *mp);
  static void ProcRName(mif *mp);

  static DCnlist TblNameList;

  static void ProcTForm(mif *mp);
  static void EndTForm(void);
  static void ProcTName(mif *mp);

  static DCgroup *TPlaceGroup;
  static bool TPUsed;

  static void ProcTPlaceGroup(mif *mp);
  static void ProcTBAlign(mif *mp);
  static void ProcTBPlace(mif *mp);
  static void ProcTBBlock(mif *mp);

  static DCclist *ColPropList;
  static uns TbCols;
  static uns TbXCol;
  static uns TbXColRule;

  static void ProcTCol(mif *mp);
  static void ProcTCNum(void);
  static void ProcTXColRule(void);

  static void ProcTCMarg(mif *mp);

  static uns TbColWidCount;
  static DCblock *ColWids;
  static void ProcTColWid(mif *mp);

  static DCgroup *BRowProps;
  static DCgroup *HRowProps;
  static DCgroup *FRowProps;
  static bool BPUsed;
  static bool HPUsed;
  static bool FPUsed;

  static void ProcBRowProp(mif *mp);
  static void ProcHRowProp(mif *mp);
  static void ProcFRowProp(mif *mp);
  static void ProcHFRowProp(mif *mp);

  static DCclist TblList;
  static uns TbID;
  static uns TbSect;
  static uns TbRows;
	static bool InTbRow;
	static bool InTbCell;

  static void ProcTbl(mif *mp);
  static void EndTbl(void);
  static void ProcTbRow(mif *mp);
  static void EndTbRow(void);
  static void ProcTbCell(mif *mp);
  static void EndTbCell(void);
  static void ProcATbl(void);



  // in drmifgr.cc

  static mif mifgrpile[];
  static size_t mifgr_size;
  static char *wordgrpile[];

  static void ProcGr(mif *mp);
  static void StartGraphic(void);
  static void StartGraphText(void);

	static bool FrameProp;
  static void ProcFrameProps(mif *mp);
  static void ProcFrameProp(mif *mp);

  static uns CurrGID;
  static DCclist GIDList;
  static DCclist GGroupList;
  static void GraphicGroup(mif *mp);
  static void EndGGroup(void);

  static uns AFNum;
  static uns FrType;
  static uns FrFloat;
  static uns FrAlign;
  static uns FrCrop;
  static long FrNSoff;
  static long FrBLoff;
  static long FrRunaroundGap;
  static char FrName[FStringMax];

  static DCclist AFrameList;
  static DCclist AnchorList;

	static DCilist PrIDList;
	static char *GrSuffix;
	static char *GrFileID;
	static bool UseGrFileID;
	static bool UseLocalFileID;
	static bool UseGrPreviews;
	static uns GrType;
	static bool StartingFrame;

  static void ProcGFrame(mif *mp);
  static void EndGFrame(void);
  static void PutAnchorProps(DCgroup *gp);
  static void ProcAFrame(mif *mp);
	static DCvlist TblAFIndex;
	static DCvlist FNoteAFIndex;
	static long GrFileNumLen;
	static long GrFileNumMask;
	static void ProcAFLinks(DCctl *cp);
	static void ProcPreviewName(DCgroup *gp, long prid, uns mifid = 0);
	static void ProcMathGifs(DCgroup *gp, uns mifid);
	static long CurrAFrame;
	static void ProcAFTextIndex(DCctl *cp, long i, long v);
	static void ProcAFTextStart(DCctl *cp, long i, long v);
	static void ProcNestedAFIndex(DCctl *cp, long i, long v);

  static point *PointList;
  static long PointCount;
  static long PLPos;
	static bool ClosedPoly;
	static bool ShapeRect;

  static long Smooth;
  static long Radius;
  static long RRectExt[6];
  static long EllipseExt[5];
  static long ArcTheta;
  static long ArcDTheta;
  static long ArcExt[7];

  static void ProcGrObj(mif *mp);
  static void EndGrObj(void);
	static void RotatePoints(bool IsBezier = false);

  static long BezFlatness;
  static long BezDistMax;
  static long BezLevel;

#if 0
  static long BezXMin;
  static long BezXMax;
  static long BezYMin;
  static long BezYMax;
#endif

  static long BezLX;
  static long BezLY;
	static double BezTX;
	static double BezTY;
	static double BezTD;


  static void	GetBezCenter(long& xcent, long& ycent);
  static void	CheckBezBounds(long cx, long cy);
  static void	ProcBezArc(long *bezpt);
  static bool	BezFlat(long *bezpt);

  static void SetArcValues(long *lptr);
  static long BoundAngle(long ang);

  static bool GraphicText;
  static bool GraphTextStart;
  static bool GraphTextFontStart;
	static bool GraphTextForm;
  static bool GraphTextSeq;
  static point TLPos;
  static long TAngle;
  static long TAlign;
  static long TLExt[3];
	static gtprop CurrGrTextProp;
	static grcform *CurrGrCForm;
  static DCvlist GCFormList;

  static void ProcTextLine(mif *mp);
  static void StartTextLine(void);
	static void PutGrTextProps(DCgroup *gp);
	static bool GetGrCForm(long val);
	static void UseGrCForm(grcform *gcf);
	static char *DefGrTextName;
	static long DefGrTextSize;
	static grcform *DefGrCForm;
	static bool UseDefGrCForm;
	static void SetDefGrTextProps(void);
  static void EndTextLine(void);

  static uns MathNum;
  static uns MathForm;
  static bool MathFullForm;
  static long MathAlign;
  static point MathPos;
  static long MathTSize;
  static long MathLBreak;
  static long MathExt[4];
  static DCctl *MathFFCtl;

	static long MathParaID;
	static long MathParaSequence;
	static long GifID;
	static DClist MathFrameList;
	static DCilist MathIDList;
	static DCilist MathParaList;
	static DCnlist MathFileList;
	static char *HTSuffix;
	static char *EqnSuffix;
	static bool UseAllGifs;

  static void ScanHTMath(MIFrfile *rf);
  static void ProcMath(mif *mp);
  static void ProcMathFullForm(mif *mp);
  static void EndMath(void);

  static DCctl *ImpFileC;
  static uns BitMapDPI;
  static uns FlipLR;
  static long ImpExt[9];
	static uns ExNum;
	static uns SubNum;
	static bool ExWmfFiles;
	static bool ExBmpFiles;
	static bool ExRfFiles;
	static bool ExPctFiles;
	static bool ExTifFiles;
	static bool ExPcxFiles;
	static bool ExWpgFiles;
	static bool ExGifFiles;
	static bool ExJpgFiles;
	static bool ExPngFiles;
	static bool ExEpsFiles;
	static bool ExCdrFiles;
	static bool ExOleFiles;
	static bool UseFrameImage;
	static bool FixGraphicNames;
	static char *GraphicRenameBat;
	static FILE *GraphicRenBatFile;

  static void ProcImport(mif *mp);
  static void ProcImpFile(void);
  static void EndImport(void);
	static DCctl *ProcExport(DCgroup* gp, char *ext, DCblock *content = NULL);
	static bool ProcExtern(DCgroup* gp);
#ifdef PICTOK
	static bool ProcPICT(DCgroup* gp);
#endif
	static bool FindOleWMF(void);
	static void ProcOLEextraWMF(long ptr);
	static unl VerifyWMFSection(unc *sp, unl len, unc *ep);
	static bool ProcWMF(DCgroup* gp);
	static bool ProcDIB(DCgroup* gp);
	static bool ProcSunRF(DCgroup* gp);
	static bool ProcEpsi(DCgroup* gp);
	static bool ProcTiff(DCgroup* gp);

#ifdef VECINSET
	static bool ProcFrVect(DCgroup* gp);
	static short PictSize[208];
	static short PictUsage[256];
#endif

	static short GetIShort(unc* &chp);
	static short GetMShort(unc* &chp);
	static short (*GetShort)(unc*&);

	static long GetILong(unc* &chp);
	static long GetMLong(unc* &chp);
	static long (*GetLong)(unc*&);

  static long ImpMLong(long *lp);
  static long ImpILong(long *lp);

	static void MakeILong(long *lp);
	static void MakeIShort(short *sp);

	static int ExNameWid;
	static int ExNumWid;
	static void WriteGrFiles(DCctl* cp, DCLwfile& wf);

  static long TipAngle;
  static long BaseAngle;
  static long ArLength;
  static long ArHead;
  static long ArrowStyle;
  static arrow StdArrows[StdArrowMax];
  static uns ArrowUnit;
  static DCclist CustomArrows;
  static long ArExt[5];
  static uns ArScale;
  static long ArFactor;

  static void ProcArrow(mif *mp);
  static void EndArrow(void);

  static uns CurrPen;
  static uns CurrPenWid;
  static uns CurrFill;
  static uns CurrColor;
  static long HeadCap;
  static long TailCap;

	static uns CurrDash;
	static uns CurrLineSeg[LineSegsMax];
	static uns CurrLineSegCount;
	static uns LineSegNum;

	static void EndDashProc(void);
  static void PutGProps(DCgroup *gp, uns gdat);


 public:

  // in drmif.cc, called from main()

	static char *DRmifVer;
	static swlist SwitchList[];
	static bool FormatSet;
  static short Verbose;

  static char *CurrFile;
  static char *IniFileName;
  static char *OldIniName;
  static char OldFormType;
	static DCirfile *IniFile;
	static MIFrfile *MIFFile;
	static DCLwfile *DCLFile;
	static bool StartingAuto;
	static bool BookAuto;

	static bool DoAutomation;
	static char OutType;
	static char HelpType;
	static char FormType;

  static void SetFormat(char *fmt);

	static bool InitCurrProps(MIFrfile *rf);

  static void ParseMIF(MIFrfile *rf);
  static void WriteDCL(DCLwfile *wf);
};



// externs in drmif.cc
 
extern "C" int mifcmp(const void *mp1, const void *mp2);



#endif


