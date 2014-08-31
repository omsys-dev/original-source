//dcfmtr.h is the header for format ini file reading, JHG
// Copyright (c) 2009 by Omni Systems, Inc.  All rights reserved.

#ifndef DCFMTR_H
#define DCFMTR_H


enum fmtouttyp {
 fmoutnone = 0, fmouthtm, fmoutxml, fmoutrtf
};

enum lenunits {
	lennone = 0, lenin, lencm, lenmm, lenpt, lenpc,
	lenpx, lenem, lenex, lenpct
};

enum brtyp {
	brnone = 0, brs, brdb, brdot, brdash,           // all
	brth, brsh, brhair,                             // rtf
  brridge, broutset, brgroove, brinset, brhidden  // css
};

struct brinfo {
	brtyp type;
	long thick;
	long color;
	long space;
};

enum shtyp {
	shnone = 0, shhoriz, shvert, shfdiag, shbdiag, shcross, shdcross,
	shdkhoriz, shdkvert, shdkfdiag, shdkbdiag, shdkcross, shdkdcross,
};

struct shinfo {
	shtyp type;
	long color;
	long bcolor;
	long tint;
};

struct tabinfo {
  short tpos;
  unc ttype;
  unc tlead;
};


struct fmtinfo {  // strings collected from .ini
	char *display;
	char *based;
	char *selector;

	char *color;
	char *bcolor;
	char *fntname;
	char *fntsize;
	char *fntstyle;
	char *fntvar;
	char *fntweight;
	char *fntkern;
	char *textdecor;
	char *texttrans;

	char *section;
	bool bsectstart;
	char *sectstart;
	char *sectbreak;
	char *pbreak;

	char *lineheight;
	char *textalign;
	char *vertalign;
	char *keep;

	char *margleft;     // margins
	char *margright;
	char *margtop;
	char *margbottom;

	char *padleft;      // padding
	char *padright;
	char *padtop;
	char *padbottom;

	char *txindent;

	char *brdrt;
	char *brdrb;
	char *brdrl;
	char *brdrr;
	char *brdrbar;
	char *brdrbox;
	char *brdrbtw;

	char *shade;

	char *tabs;

	char *liststyle;
	long listlevel;
	char *xref;
	char *runin;
	char *number;
	char *numref;

	char *css;
	char *rtf;

	char *before;
	char *start;
	char *end;
	char *after;
};

struct fmtvals {  // values converted for use in Curr
	uns stype;
	char *cdisp;
	char *based;
	char *selector;

	long color;
	char *ccolor;
	long bcolor;
	char *cbcolor;

	char *font;     // single font name
	char *cfont;    // font name for CSS
	long fsize;     // font size in twips
	char *cfsize;   // font size for CSS
	unc fstyle;     // 0 = none, 1 = normal, 2 = italic
	char *cfstyle;
	unc fvar;       // 0 = none, 1 = normal, 2 = small-caps
	char *cfvar;
	unc fweight;    // 0 = none, 1 = normal, 2 = bold
	char *cfweight;
	long fkern;     // positive or negative kerning amount (RTF only)
	char *cfkern;
	unc tdecor;     // 0 = none, 1 = normal, 2 = under, 3 = over
	unc tstrike;    // 0 = none, 1 = normal, 2 = strikeout
	char *ctdecor;
	unc ttrans;     // 0 = none, 1 = normal, 2 = uppercase,
                  // 3 = lowercase, 4 = capitalize
	char *cttrans;

	char *section;
	bool sectstart;
	unc sectbreak;
	unc pbreak;

	long lhigh;     // line height
	unc lmode;
	char *clhigh;
	unc talign;     // text align
	char *ctalign;
	unc valign;
	char *cvalign;
	uns keep;

	long margl;     // margins
	long margr;
	long margt;
	long margb;
	char *cmargl;   // margins for CSS
	char *cmargr;
	char *cmargt;
	char *cmargb;

	long padl;      // padding
	long padr;
	long padt;
	long padb;
	char *cpadl;    // padding for CSS
	char *cpadr;
	char *cpadt;
	char *cpadb;

	long tind;      // text indent in twips
	char *ctind;    // text indent for CSS

	brinfo *brdrt;
	brinfo *brdrb;
	brinfo *brdrl;
	brinfo *brdrr;
	brinfo *brdrbar;
	brinfo *brdrbox;
	brinfo *brdrbtw;

	shinfo *shade;

	DCvlist *tabs;
	long tcount;

	long listnum;
	bool listfirst;
	char *listelem;
	char *liststyle;
	long listlevel;
	char *listurl;
	char *listpos;

	char *css;
	char *rtf;

	char *before;
	char *start;
	char *end;
	char *after;
};


struct tbfmtinfo {  // strings collected from .ini
	char *cssclass;
	char *based;

	char *brdrmodel;
	char *brdrspacing;
	char *tbllayout;
	char *emptycells;

	char *tbalign;
	char *tbbreak;
	char *keep;
	char *tbwide;

	char *tbcwide;

	char *margleft;    // table margins
	char *margtop;
	char *margbottom;
	char *margcell;

	char *cellmargleft;     // cell margins
	char *cellmargright;
	char *cellmargtop;
	char *cellmargbottom;

	char *cellpadleft;      // cell padding
	char *cellpadright;
	char *cellpadtop;
	char *cellpadbottom;

	char *hrows;
	char *brows;
	char *frows;

	char *brdrt;
	char *brdrb;
	char *brdrl;
	char *brdrr;

	char *colbrdrl;
	char *colbrdrr;

	char *shade;

	char *css;
	char *rtf;

	char *before;
	char *after;
};

struct tbfmtvals {  // values converted for use in Tbl
	char *cssclass;
	char *based;

	char *brdrmodel;
	char *brdrspacing;
	char *emptycells;
	char *tbllayout;

	unc tbalign;     // table align
	char *ctbalign;

	short tbbreak;   // col/page break
	uns keep;        // keep together

	long tbwide;     // table width
	char *ctbwide;

	DCilist *cwids;  // column widths
	long colcount;

	long margl;      // table margin left
	long margt;
	long margb;
	char *cmargl;    // table margins for CSS
	char *cmargt;
	char *cmargb;

	long cellmarg;   // cell margins horizontal
	char *ccellmarg;

	long cellmargl;  // cell margins
	long cellmargr;
	long cellmargt;
	long cellmargb;
	char *ccellmargl; // cell margins for CSS
	char *ccellmargr;
	char *ccellmargt;
	char *ccellmargb;

	long cellpadl;    // cell padding
	long cellpadr;
	long cellpadt;
	long cellpadb;
	char *ccellpadl;  // cell padding for CSS
	char *ccellpadr;
	char *ccellpadt;
	char *ccellpadb;

	DCnlist *hrows;  // header row formats
	long hrcount;

	DCnlist *brows;  // body row formats
	long brcount;

	DCnlist *frows;  // footer row formats
	long frcount;

	brinfo *brdrt;   // table borders
	brinfo *brdrb;
	brinfo *brdrl;
	brinfo *brdrr;

	brinfo *colbrdrl; // column borders
	brinfo *colbrdrr;

	shinfo *shade;

	char *css;
	char *rtf;

	char *before;
	char *after;
};

struct rowfmtinfo {  // strings collected from .ini
	char *cssclass;
	char *based;

	char *rowtype;
	char *rowhigh;
	char *keep;

	char *hcells;
	char *bcells;

	char *brdrt;
	char *brdrb;
	char *brdrx;
	char *brdrl;
	char *brdrr;

	char *shade;

	char *css;
	char *rtf;
};

struct rowfmtvals {  // values converted for use in Tbl
	char *cssclass;
	char *based;

	unc rowtype;    // 1 = body, 2 = header, 3 = footer
	long rowhigh;
	unc rhtype;     // 1 = adapt, 2 = minimum, 3 = fixed

	uns keep;

	DCnlist *hcells; // header cell formats
	long hccount;

	DCnlist *bcells; // body cell formats
	long bccount;

	brinfo *brdrt;
	brinfo *brdrb;
	brinfo *brdrx;
	brinfo *brdrl;
	brinfo *brdrr;

	shinfo *shade;

	char *css;
	char *rtf;
};


struct cellfmtinfo {  // strings collected from .ini
	char *cssclass;
	char *based;

	char *margleft;
	char *margright;
	char *margtop;
	char *margbottom;

	char *padleft;
	char *padright;
	char *padtop;
	char *padbottom;

	char *vertalign;

	char *brdrt;
	char *brdrb;
	char *brdrl;
	char *brdrr;

	char *shade;

	char *css;
	char *rtf;

	char *start;
	char *end;
};

struct cellfmtvals {  // values converted for use in Tbl
	char *cssclass;
	char *based;

	long margl;     // margins
	long margr;
	long margt;
	long margb;
	char *cmargl;   // margins for CSS
	char *cmargr;
	char *cmargt;
	char *cmargb;
	long padl;      // padding
	long padr;
	long padt;
	long padb;
	char *cpadl;    // padding for CSS
	char *cpadr;
	char *cpadt;
	char *cpadb;

	unc valign;
	char *cvalign;

	brinfo *brdrt;
	brinfo *brdrb;
	brinfo *brdrl;
	brinfo *brdrr;

	shinfo *shade;

	char *css;
	char *rtf;

	char *start;
	char *end;
};



struct pagefmtinfo {  // strings collected from .ini
	char *cssclass;
	char *based;

	bool btwosided;
	char *twosided;
	bool bfirstright;
	char *firstright;
	char *startsection;

	bool bmargmirror;
	char *margmirror;
	char *pgnform;
	char *pgnstart;
	char *orient;
	char *paperw;
	char *paperh;

	char *margleft;
	char *margright;
	char *margtop;
	char *margbottom;

	char *gutter;
	char *cols;
	char *colsx;
	char *linebetcol;
	bool blinebetcol;

	char *vertalign;

	char *brdrt;
	char *brdrb;
	char *brdrl;
	char *brdrr;
	char *brwraphd;
	bool bbrwraphd;
	char *brwrapft;
	bool bbrwrapft;

	char *headery;
	char *footery;

	char *css;
	char *rtf;
};

struct pagefmtvals {  // values converted for use in Page
	char *cssclass;
	char *based;

	bool twosided;
	bool firstright;
	char *startsection;

	bool margmirror;
	uns pgnform;
	uns pgnstart;
	uns orient;
	uns paperw;
	char *cpaperw;
	uns paperh;
	char *cpaperh;

	long margl;     // margins
	long margr;
	long margt;
	long margb;
	char *cmargl;   // margins for CSS
	char *cmargr;
	char *cmargt;
	char *cmargb;

	uns gutter;
	char *cgutter;
	uns cols;
	uns colsx;
	char *ccolsx;
	bool linebetcol;

	unc valign;
	char *cvalign;

	brinfo *brdrt;
	brinfo *brdrb;
	brinfo *brdrl;
	brinfo *brdrr;

	bool brwraphd;
	bool brwrapft;

	uns headery;
	char *cheadery;
	uns footery;
	char *cfootery;

	char *css;
	char *rtf;
};


struct sectfmtinfo {  // strings collected from .ini
	char *cssclass;
	char *based;

	char *startside;
	bool bmargmirror;
	char *margmirror;
	char *pgnform;
	char *pgnstart;
	char *orient;
	char *paperw;
	char *paperh;

	char *margleft;
	char *margright;
	char *margtop;
	char *margbottom;

	char *gutter;
	char *cols;
	char *colsx;
	char *linebetcol;
	bool blinebetcol;

	char *vertalign;

	char *brdrt;
	char *brdrb;
	char *brdrl;
	char *brdrr;
	char *brwraphd;
	bool bbrwraphd;
	char *brwrapft;
	bool bbrwrapft;

	char *headery;
	char *footery;

	char *titlepg;
	bool btitlepg;
	char *titlespa;

	char *hdb;
	char *ftb;
	char *hdf;
	char *ftf;
	char *hdl;
	char *ftl;
	char *hdr;
	char *ftr;

	char *css;
	char *rtf;

	char *start;
	char *end;
};

struct sectfmtvals {  // values converted for use in Page
	char *cssclass;
	char *based;

	uns startside;
	bool margmirror;
	uns pgnform;
	uns pgnstart;
	uns lndscp;
	uns paperw;
	char *cpaperw;
	uns paperh;
	char *cpaperh;

	long margl;     // margins
	long margr;
	long margt;
	long margb;
	char *cmargl;   // margins for CSS
	char *cmargr;
	char *cmargt;
	char *cmargb;

	uns gutter;
	char *cgutter;
	uns cols;
	uns colsx;
	char *ccolsx;
	bool linebetcol;

	unc valign;
	char *cvalign;

	brinfo *brdrt;
	brinfo *brdrb;
	brinfo *brdrl;
	brinfo *brdrr;

	bool brwraphd;
	bool brwrapft;

	uns headery;
	char *cheadery;
	uns footery;
	char *cfootery;

	bool titlepg;
	long titlespa;
	char *ctitlespa;

	hfinfo *hdb;
	hfinfo *ftb;
	hfinfo *hdf;
	hfinfo *ftf;
	hfinfo *hdl;
	hfinfo *ftl;
	hfinfo *hdr;
	hfinfo *ftr;

	char *css;
	char *rtf;
};


class DCfrfile : public DCirfile {
 protected:

	static DCnlist BorderBasedUsed;

	static DCnlist ShadeBasedUsed;

	static DCnlist HFBasedUsed;
	hfinfo *GetHFForm(char *hffmt);

	long GetTabDefs(char *str, DCvlist *tdef);
	char *Unescape(char *chp);

	static long FmtNums;
	static DCnlist FmtName;
	static DCvlist FmtInfo;
	static DCnlist BasedUsed;
	fmtinfo *GetFmtInfo(const char *fname);

	tbfmtinfo *GetTableFmtInfo(const char *fname);
	rowfmtinfo *GetRowFmtInfo(const char *fname);
	cellfmtinfo *GetCellFmtInfo(const char *fname);

	pagefmtinfo *GetPageFmtInfo(const char *fname);
	sectfmtinfo *GetSectFmtInfo(const char *fname);

	long GetColWidths(char *cwidths, DCilist *wids);
	long GetRowDefs(char *rows, DCnlist *fmts);
	unc GetRowType(char *rtype);
	long GetRowHigh(char *rhigh, unc *rhtype);
	long GetCellDefs(char *rows, DCnlist *fmts);

	static long XrefFmtNums;
	static DCnlist XrefName;
	static DCnlist XrefForm;
	static DCnlist XrefBasedUsed;
	char *GetXrefFmtInfo(const char *xfmtname);

	static long RuninFmtNums;
	static DCnlist RuninName;
	static DCnlist RuninForm;
	static DCnlist RuninBasedUsed;
	char *GetRuninFmtInfo(const char *rfmtname);

	static long IndexFmtNums;
	static DCnlist IndexName;
	static DCnlist IndexForm;
	static DCnlist IndexBasedUsed;
	char *GetIndexFmtInfo(const char *fmtname);

	static long MarkFmtNums;
	static DCnlist MarkName;
	static DCnlist MarkForm;
	static DCnlist MarkBasedUsed;
	char *GetMarkFmtInfo(const char *fmtname);

	char *GetFontName(char *fname, char **rstr = NULL);

	static DCnlist LengthNames;
	static npair LengthNamesInit[];
	static bool LengthNamesSet;
	long GetLength(char *lstr, char **rstr = NULL);

	long GetPercent(char *lstr, char **rstr = NULL);

	static DCnlist ColorNames;
	static npair ColorNamesInit[];
	static bool ColorNamesSet;
	long GetColor(char *cstr, char **rstr = NULL);

	double CurrPercentBase;

 public:
  DCfrfile(const char *fname);
  virtual ~DCfrfile() { }


	static fmtouttyp FmtOutputType;

	static char *BodyBaseSize;
	static char *IndentBaseWidth;
	static double BodyBaseVal;
	static double IndentBaseVal;
	static bool BaseValuesSet;

	static DCfrfile *Formats;
	static DCfrfile *Tables;
	static DCfrfile *Pages;
	static DCfrfile *SubFormats;

	DCfrfile *SetConfigTemplate(const char *sect, const char *setting,
		                          bool link = true);

	long IniLevel;
	DCvlist IniFileList;
	DCnlist IniNameList;

	bool GetSection(const char *fname);
	bool GetFmtSection(const char *fname);
	bool GetTblSection(const char *fname);
	bool GetPgSection(const char *fname);
	bool GetSubSection(const char *fname);

	bool CheckFormat(const char *fname);
	bool CheckTblFormat(const char *fname);
	bool CheckPgFormat(const char *fname);

  fmtvals *Format(const char *name, fmtvals *fv = NULL);
  tbfmtvals *TableFormat(const char *name, tbfmtvals *fv = NULL);
  rowfmtvals *RowFormat(const char *name, rowfmtvals *fv = NULL);
  cellfmtvals *CellFormat(const char *name, cellfmtvals *fv = NULL);
  pagefmtvals *PageFormat(const char *name, pagefmtvals *fv = NULL);
  sectfmtvals *SectFormat(const char *name, sectfmtvals *fv = NULL);

	brinfo *GetBorder(char *brfmt);
	shinfo *GetShade(char *shfmt);

	char *GetNumFmtName(const char *fmtname);
	char *GetNumRefFmtName(const char *fmtname);

	static DCnlist NumBasedUsed;
	numforminfo *GetNumFormat(char *numfmt);

	static DCnlist NumStrBasedUsed;
	numstrinfo *GetNumStream(char *numstrfmt);

	char *GetXrefFmtName(char *fmtname);
	char *GetXrefFmtForm(char *xfmtname);

	char *GetRuninFmtName(char *fmtname);
	char *GetRuninFmtForm(char *rfmtname);

	char *GetIndexFmtForm(char *fmtname);
	char *GetMarkFmtForm(char *fmtname);

	static long GetColorValue(char *color, char **rstr = NULL);
};


#endif

