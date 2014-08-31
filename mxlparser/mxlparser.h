// mxlparser.h is for the DLL for parsing MicroXML files.
// Created May 26, 2013, by Jeremy H. Griffith
// Copyright (c) 2013 Jeremy H. Griffith.  All rights reserved.


// The following ifdef block is the standard way of creating macros which make exporting 
// from a DLL simpler. All files within this DLL are compiled with the MXLPARSER_EXPORTS
// symbol defined on the command line. this symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see 
// MXLPARSER_API functions as being imported from a DLL, wheras this DLL sees symbols
// defined with this macro as being exported.
#ifdef MXLPARSER_EXPORTS
#define MXLPARSER_API __declspec(dllexport)
#else
#define MXLPARSER_API __declspec(dllimport)
#endif


class MxlParser;

typedef unsigned char unc;
typedef unsigned long unl;

struct pair {
 unl *name;       // attribute name, UTF-32
 long namelen;
 unl *val;        // attribute value, UTF-32
 long vallen;
};

struct cont {
 void *it;        // ptr to array of UTF-32 chars or element
 long cnt;        // count if chars, 0 if element
};

struct element {  // object model uses one top element per doc
 unl *name;       // array of UTF-32 chars
 long namelen;
 pair **attrs;    // array of attribute pairs
 long attrcnt;
 cont **content;  // arrays of element ptrs or UTF-32 chars
 long contcnt;
 element *parent;
};

enum mxl_const {
 MAX_CHAR_COUNT = 2048, MAX_ELEM_NEST = 129, MAX_ATTR_COUNT = 128
};

class MXLPARSER_API MxlParser {
 protected:
  char *fname;
  char *lang;
	FILE *ifile;

	bool InStartTag;
	bool InEndTag;
	bool IsEmptyElement;
	bool InComment;
	bool StartPI;
	bool InPI;
	bool InDoctype;
	bool InCDATA;
	bool EndDoc;
	bool InDoc;
	bool EndFile;

	long Line;

	pair *CurrAttr;
	bool AttrName;
	long AttrNameCnt;
	bool AttrVal;
	long AttrValCnt;
	long AttrQuote;
  
  element *rootelem;              // for building data model
	element *currelem;
	long elemlevel;
	element *elevels[MAX_ELEM_NEST];
	long attrcnt;
	pair *attrbuf[MAX_ATTR_COUNT];

  static unl txt32buf[MAX_CHAR_COUNT];  // for writing data model
  unc txt8buf[MAX_CHAR_COUNT];   // for reading from file
  unc errbuf[MAX_CHAR_COUNT];    // for writng error reports
  unc cmtbuf[MAX_CHAR_COUNT];    // for writng comment reports
  static unc out8buf[MAX_CHAR_COUNT];   // for writing UTF-8

	void GetStartTag(unl u32, unc **cpt);
	void EndStartTag(void);
	void GetAttributes(unc **cpt, unc *end = NULL);
	void SkipSpace(unc **cpt);
	void SetAttrName(void);
	void SetAttrVal(void);
	void AddAttributes(void);
	void DeleteAttributes(void);
	void GetEndTag(unc **cpt);
	unl GetCharReference(unc **cpt);
	void AddTextContent(long cnt);
  
  bool IsValidUChar(unl u32);
  bool IsValidUName(unl u32);
  bool IsValidUNameStart(unl u32);

  char *MakeStr(unc *start, unc* end);
	char *GetAttrVal(char *str, char **end = NULL);
	char *GetQuotedVal(char *str, char **end = NULL);

	// typedefs for callbacks, normal and expat
	typedef void (*starttag)(unl *, long, pair **, long);
	typedef void (*endtag)(unl *, long);
	typedef void (*txtcont)(unl *, long);
	typedef void (*reperr)(long, char *, char *, bool);
	typedef void (*repcmt)(char *);

	typedef void (*expatstart)(void *, const char *, const char **);
	typedef void (*expatend)(void *, const char *);
	typedef void (*expattxt)(void *, const char *, int);
	typedef void (*expatpi)(void *, const char *, const char *);
	typedef void (*expxmldec)(void *, const char *, const char *, int);
	typedef void (*expsdtdec)(void *, const char *, const char *, const char *, int);
	typedef void (*expdata)(void *);
	typedef void (*expcmt)(void *, char *);

	void ReportXMLCData(unc *cp);
	void ReportXMLDoctype(unc *cp);
	void ReportXMLPI(unc *cp);

	// layer for changing report formats to expat

	bool ExpatMode;
	char ExPIBuf[MAX_CHAR_COUNT];
	long ExPICnt;
	void *ExpatUserData;

  void SetStart(unl *name, long namecnt, pair **attrs, long attrcnt);
  void SetEnd(unl *name, long namecnt);
  void SetText(unl *text, long textcnt);
  void SetPI(char *pi);
  void SetDoctype(char *pi);
  void SetCData(char *pi);

  void ReportExpatXMLDecl(char *cp);
	void ReportExpatDoctypeDecl(char *cp);
	void ReportExpatCDataSection(char *cp);


  static void StubExpatStart(void *user, const char *name, const char **attrs);
  static void StubExpatEnd(void *user, const char *name);
  static void StubExpatText(void *user, const char *text, int textcnt);
  static void StubExpatStartCdataSection(void *user);
  static void StubExpatEndCdataSection(void *user);
  static void StubExpatPI(void *user, const char *target, const char *data);
  static void StubExpatXMLDecl(void *user, const char *ver, const char *enc, int stand);
  static void StubExpatStartDoctypeDecl(void *user, const char *name,
	                  const char *sys, const char *pub, int intsub);
  static void StubExpatEndDoctypeDecl(void *user);
  static void StubExpatComment(void *user, char *comment);

  expatstart ExpatStartTag;
  expatend ExpatEndTag;
  expattxt ExpatTextContent;
	expdata ExpatStartCdataSection;
	expdata ExpatEndCdataSection;
  expatpi ExpatPI;
	expxmldec ExpatXMLDecl;
	expsdtdec ExpatStartDoctypeDecl;
	expdata ExpatEndDoctypeDecl;
	expcmt ExpatComment;

	// normal callbacks
  static void StubStart(unl *name, long namecnt, pair **attrs, long attrcnt);
  static void StubEnd(unl *name, long namecnt);
  static void StubText(unl *text, long textcnt);
  static void StubError(long line, char *warning, char *cpt = NULL, bool fatal = false);
  static void StubComment(char *comment);
  static void StubPI(char *pi);
  static void StubDoctype(char *doctype);
  static void StubCData(char *cdata);

  starttag StartTag;
  endtag EndTag;
  txtcont TextContent;
  reperr ReportError;
  repcmt ReportComment;
  repcmt ReportPI;
  repcmt ReportDoctype;
  repcmt ReportCData;
  
 public:
  MxlParser(void)
	{
		UseSAX = false;
		UseModel = true;
		FullXML = false;
		StartTag = StubStart;
		EndTag = StubEnd;
		TextContent = StubText;
		ReportError = StubError;
		ReportComment = StubComment;
		ReportPI = StubPI;
		ReportDoctype = StubDoctype;
		ReportCData = StubCData;

		ExpatMode = false;
		ExPIBuf[0] = '\0';
		ExPICnt = 0;
		ExpatUserData = NULL;
		ExpatStartTag = StubExpatStart;
		ExpatEndTag = StubExpatEnd;
		ExpatTextContent = StubExpatText;
		ExpatStartCdataSection = StubExpatStartCdataSection;
		ExpatEndCdataSection = StubExpatEndCdataSection;
		ExpatPI = StubExpatPI;
		ExpatXMLDecl = StubExpatXMLDecl;
		ExpatStartDoctypeDecl = StubExpatStartDoctypeDecl;
		ExpatEndDoctypeDecl = StubExpatEndDoctypeDecl;
		ExpatComment = StubExpatComment;
;
		InComment = false;
		InStartTag = false;
		InEndTag = false;
		IsEmptyElement = false;
		StartPI = false;
		InPI = false;
		InDoctype = false;
		InCDATA = false;
		currelem = rootelem = NULL;
		elemlevel = 0;
		EndDoc = false;
		CurrAttr = NULL;
		AttrName = true;
		AttrNameCnt = 0;
		AttrVal = false;
		AttrValCnt = 0;
		AttrQuote = 0;
	}
  virtual ~MxlParser(void) { }
  
  element *ParseFile(const char *filename = NULL, char *lang = NULL);
  void SetOptions(bool us = false, bool um = true, bool fx = false);
	void SetCallbacks(char *ofile = NULL, reperr re = NULL,
    starttag st = NULL, endtag et = NULL, txtcont tc = NULL,
		repcmt rcd = NULL, repcmt rp = NULL,
		repcmt rdt = NULL, repcmt rc = NULL);

	void SetExpatCallbacks(char *ofile = NULL, reperr re = NULL,
		expatstart st = NULL, expatend et = NULL, expattxt tc = NULL, 
		expdata scd = NULL, expdata ecd = NULL, 
		expatpi rp = NULL, expxmldec rxd = NULL,
		expsdtdec sdt = NULL, expdata edt = NULL,
		expcmt rc = NULL, void *ud = NULL);

	bool UseSAX;
	bool UseModel;
	bool FullXML;
	static FILE *OutFile;

  static unl U8to32Chr(unc **u8);
  static unc *U32to8Chr(unl u32, unc *cpt = NULL);
  static unl *U8to32Str(unc **u8);
  static unc *U32to8Str(unl *u32, long u32cnt);

	static void Put32JSON(unl *u32arr, long cnt);
	static void Put8JSON(char *u8);
};


// end of mxlparser.h

