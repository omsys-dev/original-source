// drxmlud.h is the header for dcl uDoc processing, JHG
// Copyright (c) 2013 by Omni Systems, Inc.  All rights reserved.

#ifndef DRXMLUD_H
#define DRXMLUD_H


class XMLrd;
class XMLud;
class XMLrfile;
class XDVrfile;
class DCixset;
class DCixlist;
class DCixitem;
class DCixref;


struct aprop {
	char *name;
	char *type;
	DCnlist *evals;
	char *deflt;
};

struct wikisym {
	char *symbol;
	char *tag;
	char *sch;
	char *ech;
	bool space;
	unc code;
};

struct condset {
	char *id;           // name of set
	char *srcfile;      // filename to resolve naming conflicts

	long attrcnt;       // count of attributes in set
	DCnlist attrs;      // attribute names to process with attrnum
	DCnlist exprs;      // attribute expressions by attrnum

	DClist incl;        // attrnum if value is include
	DClist excl;        // attrnum if value is exclude

	DClist flag;        // attrnum if flagged
	DCnlist fclass;     // class for flagging (maps to char format)
};


struct listrefinfo {
	DCnlist files;
	DCnlist fileIDs;
	DCvlist models;
	DCvlist modelIDs;
	DCvlist modelParents;
	long count;
};


struct listdefinfo {
	DCvlist elems;
	DCnlist sorts;
	DCnlist IDs;
	DCnlist used;
	long count;
};


class XMLud {
  friend class XMLrd;

 protected:
	// in drxmlud.cpp
	static bool CmpUnlStr(unl *u, long uc, char *s);
	static bool BoolUnlStr(unl *u, long uc);
	static char *GetUnlStr(unl *u, long uc);

	static char *GetElemAttr(element *el, char *aname);
	static char *GetElemText(element *el);
	static bool PutElemContent(element *elem);
	static element *GetElemChild(element *el, char *name);
	static void GetNamedElems(element *el, char *name, DClist *dlp);

	static char *GetSystemTime(const char *dform);

	static npair EpMxlNamesInit[];
	static DCnlist Elements;
	static DCnlist *CommonAttrs;
	static DCnlist ElementDefs;
	static DCvlist ElementAttrDefs;
	static bool ElementsChanged;
	static bool UseDescInText;

	static element *GetModel(const char *fname);
	static void ProcessElemDefModel(element *model, char *fname, char *idref);
	static void ProcessAttrModel(element *model, char *fname, char *idref);
	static element *GetElemByID(element *el, char *idref);
	static void GetElemProps(element *el, char *idref);

	static DCnlist *CurrWikiSyms;
	static DCvlist WikiSymsAttrs;
	static long WikiSymsCount;

	static void GetOneWikiAttrs(element *el);

	static void GetOneElemProps(element *el);
	static void GetElemPropContent(char *ename, char *propptr);
	static void SetElemAttrDefs(element *el);

	static bool GettingElem;
	static char *XMLProjectRoot;
	static element *XMLProjectRootElem;
	static element *CurrModel;
	static DCnlist ParsedFiles;
	static DCvlist ParsedModels;
	static DCvlist ParsedIDs;
	static DCvlist ParsedParents;
	static long ParsedCount;

	static bool GetNewElem(char *fname, const char *elemids = NULL, bool nomsg = false);
	static bool ProcRange(char *start, char *end, DCnlist *elemIDs,
		                    DCvlist *elemParents, bool nomsg);
	static bool ProcElem(char *currID, DCnlist *elemIDs, bool nomsg);
	static void GetCurrElemIDs(element *el, DCnlist *lp, element *ep, DCvlist *lep);
	static void ProcElemContent(element *el);
	static void ProcTextContent(char *txt);
	static char **CheckAttrs(const char *tag, char **attrs, long idx);

	static condset *PendConds;
	static DCvlist CondSets;
	static DCnlist CondSetNames;
	static DCnlist CondSetFiles;
	static DClist CondSetsPend;
	static long CondSetCount;

	static DCvlist CondSetLevs;  // stack for restores at end of scope
	static long CurrCondSetLev;
	static long CurrCondRefLev;
	static char *CurrCondRefFile;

	static void SetNewCondSet(DCelem *elem, char *refsrc);
	static void EndNewCondSet(DCelem *elem);
	static void SetNewCond(DCelem *elem);
	static void EndNewCond(DCelem *elem);
	static void RestoreCondSet(void);
	static bool CheckCondType(const char **atts, condset *conds);
	static bool CheckCondExpr(char **expr, DCnlist *dcnp, DClist *dcp = NULL);
	static bool GetCondArg(char **expr, DCnlist *dcnp, DClist *dcp);
	static char *GetCondWord(char **expr);


	static bool InOutput;
	static bool UseOutput;
	static bool SourceKeys;
	static bool OutputKeys;
	static char *OutputKeyref;
	static char *OutputKeyID;
	static DCvlist OutputKeyList;
	static long OutputKeyCount;
	static DCgroup *OutputKeyDefs;

	static DCnlist ExtProjPaths;
	static DCvlist ExtProjModels;
	static long ExtProjCount;
	static DCvlist ExtProjOutputs;
	static DCvlist ExtProjKeysets;
	static long ExtProjOutCount;

	static void SetOutputType(char *type);
	static bool CheckOutputExpr(char *expr, const char *type);
	static void ProcOutputKeys(void);
	static void GetOutputKeys(const char *project, const char *output = NULL);
	static char *ResolveOutputKey(const char *project, const char *key,
		           const char *output = NULL);


	static listrefinfo GlossRefs;
	static listdefinfo GlossDefs;
	static listrefinfo AbbrRefs;
	static listdefinfo AbbrDefs;
	static listrefinfo TmarkRefs;
	static listdefinfo TmarkDefs;
	static bool AbbrsInGloss;

	static element *GetListDefElem(char *ref, listrefinfo *lrp);
	static bool GetListDefTitle(char *ref, listrefinfo *lrp);
	static char *GetListDefTitleText(char *ref, listrefinfo *lrp);
	static char *GetListDefDesc(char *ref, listrefinfo *lrp);
	static char *GetListDefText(char *ref, listrefinfo *lrp);
	static bool SetListDefUsed(const char *ref, listrefinfo *lrp, listdefinfo *ldp);
	static bool ListDefFirstUse(const char *ref, listdefinfo *ldp);
	static void ListDefResetUse(listdefinfo *ldp);
	static void PutAllListDefs(listdefinfo *ldp, gentyp ltyp);
	static void SetListRefFile(char *srcfile, char *srcid, listrefinfo *lrp);


 public:

	// in drxmlud.cpp
	static char *MakeUMXLHref(char *hr, const char *ref);

	static char *LogIniPrefix;
	static char *StdLibPath;
	static char *StdElemLib;
	static char *StdAttrLib;
	static char *StdVarLib;
	static char *StdGlossLib;
	static char *StdAbbrLib;
	static char *StdTmarkLib;
	static char *LocalElemLib;
	static char *LocalAttrLib;

	static char *GetLibPath(char *libname);
	static char *GetEnvVars(char *fn);
	static void ProcUMXLElemTypes(void);
	static void ReadElementDefinitions(char *fname, char *idref = NULL);
	static void ReadCommonAttrDefinitions(char *fname);

	static DCnlist MissingElemDefs;
	static long MissingElemCount;
	static bool CheckUMXLElemName(const char *name, unl bit, unl neg = 0, bool all = true);
	static element *CurrElement;

	static bool GetUMXLElement(char *fname, const char *elemids = NULL, bool nomsg = false);

	static void ProcCondition(DCelem *elem);
	static void EndCondition(DCelem *elem);
	static bool CheckConds(const char **atts);

	static condset *CurrConds;
	static condset *OutputConds;
	static void CheckCondFlags(const char *fcl);
	static bool SetCondFlags(long depth);
	static void EndCondFlags(void);
	static void ProcCondFlagDef(DCgroup *gp, long num);

	static void SetUpLists(void);
	static void ListElemRefStart(DCelem *elem, char *fname, long depth, gentyp ltyp);
	static void ListElemRefEnd(DCelem *elem, gentyp ltyp);

	static void SetGlossRefFile(char *srcfile, char *srcid);
	static void PutAllGlossDefs(void);

	static void AbbrResetUse(void);
	static void SetAbbrRefFile(char *srcfile, char *srcid);
	static void PutAllAbbrDefs(void);

	static void TmarkResetUse(void);
	static void SetTmarkRefFile(char *srcfile, char *srcid);
	static void PutAllTmarkDefs(void);

	static bool InMXMLIndexEntry;
	static bool InIndexSort;
	static DCvlist IndexItems;
	static DCnlist IndexItemIDs;
	static DCclist IndexItemGroups;
	static long IndexItemCount;
	static DCvlist IndexSeeItems;
	static DClist IndexSeeTypes;
	static DCnlist IndexSeeIDrefs;
	static DCclist IndexSeeGroups;
	static long IndexSeeCount;
	static char *IXSee;
	static char *IXSeeAlso;

	static void StartUMXLIndexItem(void);
	static void EndUMXLIndexItem(void);
	static void StartNewUMXLIndexLevel(char *pto);
	static void ProcUMXLIndexRange(char *start, char *end);
	static void ProcUMXLSee(void);
	static void ProcOneUMXLIndexSee(char *idref, long pos);
	static void ProcUMXLIndexText(char *txt, long len);

	static DCnlist CodedocrefIDs;
	static DCnlist CodedocrefFiles;
	static DCnlist CodedocrefPrefixes;
	static DCnlist CodedocrefTitles;
	static void WriteCodedocrefTOC(DCelem *elem);
	static void WriteCodedocrefFile(DCmapelem *ref);
	static void WriteCodedocrefAnums(void);
};


#endif

