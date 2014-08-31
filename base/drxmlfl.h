
// drxmlfl.h is the header for dcl XML file reading, JHG
// Copyright (c) 2008 by Omni Systems, Inc.  All rights reserved.

#ifndef DRXMLFL_H
#define DRXMLFL_H

#ifndef XMLCALL
#if defined(_MSC_EXTENSIONS) && !defined(__BEOS__) && !defined(__CYGWIN__)
#define XMLCALL __cdecl
#elif defined(__GNUC__)
#define XMLCALL __attribute__((cdecl))
#else
#define XMLCALL
#endif
#endif

class XMLrfile;

typedef void (*elemstfunc)(const char *name, const char **atts, long depth);
typedef void (*elemfunc)(const char *name, long depth);
typedef void (*charfunc)(const char *str, int len, long depth);
typedef void (*pifunc)(const char *target, const char *data);
typedef void (*logfunc)(logtype lt, unc sev, const char *desc,
 const char *d2, const char *d3, const char *d4, const char *d5);

#ifdef UDOC
#define XML_Char char
#define XML_Parser void *
#endif

// classes

class XMLrfile: public DCrfile {
  friend class XMLrd;

	// callbacks are from C, so must be statics

	static void XMLCALL startElement(void *userData, const char *name,
	 const char **atts);
	static void XMLCALL endElement(void *userData, const char *name);

	static void XMLCALL charData(void *userData, const XML_Char *s,
	 int len);
	static void XMLCALL procInstr(void *userData, const XML_Char *target,
   const XML_Char *data);

	static void XMLCALL xmlDecl(void *userData, const XML_Char *version,
	 const XML_Char *encoding, int standalone);
	static void XMLCALL startDT(void *userData, const XML_Char *doctypeName,
	 const XML_Char *sysid, const XML_Char *pubid, int int_subset);
	static void XMLCALL endDT(void *userData);

	// for MxlPparser
	static void XMLCALL startCData(void *userData);
	static void XMLCALL endCData(void *userData);
	static void XMLCALL procComment(void *userData, char *name);

	static DCnlist FailingFiles;
	static long Fails;
	static char *SubPath;
	static long SubLevel;
	static DCnlist SubPaths;

	static int XMLCALL extEntRef(XML_Parser p,
	 const XML_Char *context, const XML_Char *base,
	 const XML_Char *systemId, const XML_Char *publicId);

	static DCnlist SystemIdList;
	static DCnlist PublicIdList;
	static DCnlist SystemValList;
	static DCnlist PublicValList;
	static long SysidVal;
	static long PubidVal;

	static char *ResolveEntity(const XML_Char *systemId,
	 const XML_Char *publicId);

	static char *CurrSysId;
	static char *CurrPubId;
	static bool CatalogItemFound;
	static char *CurrCatItem;
	static char *CurrCatPath;
	static DClist CurrCatalogs;
	static DClist NewCatalogs;
	static void SearchCatalog(char *fname);

	static DCnlist CatElements;
	static npair CatElementsInit[];
	static char *NextCatalog;
	static char *CurrBase;
	static bool PrefPub;
	static bool PrefCat;
	static bool FoundPub;

	static void XMLCALL startCatElement(void *userData, const char *name,
	 const char **atts);
	static void XMLCALL endCatElement(void *userData, const char *name);
	static char *GetAttr(const char **attp, const char *name);

	static XML_Parser CurrCatParser;
	static bool AbortingCatParse;
	static void AbortCatParse(void);
	static char *AddRelPath(char *path, char *nm);

	static elemstfunc elemStart;
	static elemfunc elemEnd;
	static charfunc charProc;
	static pifunc piProc;


 public:
  static char *ErrMsg[];
	static int CurrDepth;
  static unc XMLver;         // XML version major number, 1... (not '1'...)
	static unc XMLminver;
	static unc DITAver;
	static char *CurrRoot;
	static logfunc LogEventFunc;

  XMLrfile(char *fname = NULL);

	void SetHandlers(elemstfunc es, elemfunc ee, charfunc ch, pifunc pi,
		               bool usemxl = false);

  virtual void Err();

	static void LogEvent(logtype lt, unc sev, const char *desc,
	              const char *d2 = NULL, const char *d3 = NULL,
	              const char *d4 = NULL, const char *d5 = NULL);

	~XMLrfile();

	virtual void ReopenFile(const char *fn, const char *dn, bool close = true);

#ifdef DITA
	static XML_Parser CurrDocParser;
	static bool AbortingParse;
	static void AbortParse(void);
#endif

#ifdef UDOC
	static bool UseMxlParser;
	static bool UseMxlModel;
	void ParseMxlDoc(void);
#endif

	static void procError(long line, char *warning, char *cpt, bool fatal);

	static char *LastStart;
	static char *LastEnd;
	static void *LastText;
	static long LastTextCount;
	static char *CurrSysID;

#ifdef DITA
	static bool ParserFail;

	void ParseDoc(void);

	// set during init
	static char *LocalDTDPath;
	static bool LocalPathUsed;
	static bool CatalogPathUsed;
	static DCnlist Catalogs;
#endif

};



#endif


