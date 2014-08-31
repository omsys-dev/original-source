// drxmlud.cpp is the code for uDoc functionality, JHG
// Copyright (c) 2013 by Omni Systems, Inc.  All rights reserved.

#include "dcappl.h"
#include "dcctlg.h"
#include "dcfili.h"
#include "dcfilp.h"
#include "dctxfx.h"
#include "dcfmtr.h"
#include "dcini.h"
#include "dcelem.h"
#include "drxml.h"
#include "dcindx.h"
#ifdef DITA
#include "expat.h"
#endif
#include "mxlparser.h"
#include "drxmlud.h"
#include "drxmlfl.h"

#ifdef UDOC
#endif
#ifdef DITA
#endif

// utilities for UTF-32 strings


bool
XMLud::CmpUnlStr(unl *u, long uc, char *s)
{
	char *str = (char *) MxlParser::U32to8Str(u, uc);
	for (long i = 0; str[i] != '\0'; i++) {
		if (s[i] != str[i])
			return false;
	}
	return (s[i] == 0);
}


bool
XMLud::BoolUnlStr(unl *u, long uc)
{
	char *str = (char *) MxlParser::U32to8Str(u, uc);
	if (!stricmp(str, "yes"))
		return true;
	return false;
}


char *
XMLud::GetUnlStr(unl *u, long uc)
{
	return NewName((char *) MxlParser::U32to8Str(u, uc));

#if 0
	char *str = NewName((uns) uc);

	for (long i = 0; i < uc; i++)
		str[i] = (char) (u[i] & 0x7F);

	str[uc] = '\0';
	return str;  // use DeleteName(str) when done
#endif
}


// element utilities


char *
XMLud::GetElemAttr(element *el, char *aname)
{
	long i = 0;
	pair *attr = NULL;
	char *aval = NULL;

	if (el->attrs
	 && el->attrcnt) {
		for (i = 0; i < el->attrcnt; i++) {
			attr = el->attrs[i];
			if (CmpUnlStr(attr->name, attr->namelen, aname)) {
				aval = GetUnlStr(attr->val, attr->vallen);
				break;
			}
		}
	}
	return aval;
}


char *
XMLud::GetElemText(element *el)
{
	long i = 0;
	cont *ci = NULL;
	char *txt = NULL;
	char *etxt = NULL;
	char *ntxt = NULL;
	uns len = 0;
	char *name = NULL;
	element *nel = NULL;

	for (i = 0; i < el->contcnt; i++) {
		ci = el->content[i];
		if (ci->cnt) {  // text
			txt = GetUnlStr((unl *) ci->it, ci->cnt);
			len += (uns) strlen(txt);;
		}
		else { // element
			nel = (element *) ci->it;
			name = GetUnlStr(nel->name, nel->namelen);
			if (!CheckUMXLElemName(name, epData)
			 && (!CheckUMXLElemName(name, epDesc)
			  || UseDescInText)) {
				txt = GetElemText(nel);
				len += strlen(txt);
			}
			DeleteName(name);
		}
		if (txt) {
			if (etxt) {
				ntxt = NewName(etxt, len);
				strcat(ntxt, txt);
				DeleteName(etxt);
				DeleteName(txt);
				etxt = ntxt;
			}
			else {
				ntxt = NewName(txt, len);
				DeleteName(txt);
				etxt = ntxt;
			}
			txt = ntxt = NULL;
		}
	}
	return etxt;
}


bool 
XMLud::PutElemContent(element *el)
{
	long i = 0;
	cont *ci = NULL;

	if (el->contcnt) {
		for (i = 0; i < el->contcnt; i++) {
			ci = el->content[i];
			if (!ci->cnt)  // element
				ProcElemContent((element *) ci->it);
			else  // text
				ProcTextContent(GetUnlStr((unl *) ci->it, ci->cnt));
		}
		return true;
	}

	return false;
}


element * 
XMLud::GetElemChild(element *el, char *name)
{
	long i = 0;
	cont *ci = NULL;
	element *nel = NULL;

	if (el->contcnt) {
		for (i = 0; i < el->contcnt; i++) {
			ci = el->content[i];
			if (!ci->cnt) {
				nel = (element *) (ci->it);
				if (CmpUnlStr(nel->name, nel->namelen, name))
					return nel;
			}
		}
	}

	return NULL;
}


void
XMLud::GetNamedElems(element *el, char *name, DClist *dlp)
{
	// go through elem content, and add any children 
	// with matching name to dlp
	if (CmpUnlStr(el->name, el->namelen, name)) {
		dlp->add((long) el);
		return;
	}

	// check children
	for (long i = 0; i < el->contcnt; i++) {
		cont *ci = el->content[i];
		if (!ci->cnt)  // element
			GetNamedElems((element *) ci->it, name, dlp);
	}
}



// other utilities


char *
XMLud::MakeUMXLHref(char *hr, const char *ref)
{
	uns len = (hr ? strlen(hr) : 0) + (ref ? strlen(ref) : 0) + 1;
	char *nhr = NewName(len);
	if (hr)
		strcpy(nhr, hr);
	if (ref) {
		strcat(nhr, "#");
		strcat(nhr, ref);
	}
	return nhr;
}


char *
XMLud::GetSystemTime(const char *dform)
{
	char timebuf[1024];
	time_t ltime = 0;
	struct tm *today = NULL;

	_tzset();
	time(&ltime);
	today = localtime(&ltime);
	strftime(timebuf, 1024, dform, today);
	return NewName(timebuf);
}



// uDoc element properties

npair XMLud::EpMxlNamesInit[] = {
 { epRoot, "Root" },
 { epTopic, "Doc" }, // Topic
 { epMap, "Map" },
 { epLib, "Lib" },  // not in dita

 { epGroup, "Group" },
 { epNolev, "NoLevel" },
 { epStart, "Start" },  // TabStart
 { epShort, "Short" },  // not in dita
 { epNumbered, "Num" },
 { epSub, "Sub" },
 { epSeq, "Seq" },  // Sequence
 { epSect, "Sect" },  // Section

 { epTable, "Table" },
 { epTabCol, "Col" },  // TabCol
 { epTabRow, "Row" },  // TabRow
 { epTabCell, "Cell" }, // TabCell

 { epFig, "Fig" },
 { epImage, "Image" },
 { epObj, "Object" },
 { epParam, "Param" },

 { epList, "List" },
 { epParmList, "PList" },
 { epListItem, "Item" },  // ListItem
 { epNote, "Note" },
 { epDraft, "Comment" },  // Draft

 { epText, "Text" },
 { epPara, "Para" },  // not in dita
 { epPre, "Pre" },
 { epTitle, "Title" },
 { epUsage, "Usage" },  // not in dita
 { epQuote, "Quote" },  // not in dita
 { epAlt, "Alt" },
 { epDesc, "Desc" },

 { epInline, "Inline" },
 { epGloss, "Gloss" },  // Glossary
 { epAbbr, "Abbr" },  // Abbrev
 { epTmark, "Tmark" },  // Trademark
 { epMarker, "Marker" },  // not in dita
 { epIndex, "Index" },
 { epVar, "Var" },
 { epFtn, "Fnote" },  // Footnote
 { epBreak, "Break" },  // not in dita
 { epTab, "Tab" },  // not in dita
 { epTypo, "Typo" },

 { epData, "Data" },  // not in dita
 { epDate, "Date" },  // not in dita
 { epCSH, "CSH" },  // not in dita
 { epCode, "Code" },  // not in dita

 { epRef, "Ref" },
 { epFile, "File" },  // not in dita
 { epDir, "Dir" },  // not in dita
 { epDB, "DB" },  // not in dita
 { epWeb, "Web" },  // not in dita
 { epElem, "Elem" },  // not in dita
 { epRel, "Rel" },
 { epExt, "Ext" },  // not in dita

 { epDef, "Def" },  // not in dita
 { epCond, "Cond" },  // not in dita
 { epOut, "Output" },  // not in dita
 { epAttr, "Attr" },  // not in dita
 { epSyn, "Syn" },  // not in dita
 { epKey, "Key" },

#if 0
// not used by uDoc

 //{ epPern, "Pern" }, **  // Pernicious
 //{ epIndexStart, "Start" }, ** //IxStart
 //{ epIndexSort, "IxSort" }, **
 //{ epIndexSee, "IxSee" }, **
 //{ epIndexSeeAlso, "IxSeeAlso" }, **
 //{ epMeta, "Meta" }, **
 //{ epBkTitle, "BookTitle" }, **
 //{ epLink, "Link" }, **
 //{ epTask, "Task" }, **
 //{ epRefer, "Reference" }, **
 //{ epXRsrc, "XRSource" }, **
 //{ epTabHead, "Head" }, **  // TabHead
 //{ epTabBody, "Body" }, **  // TabBody
 //{ epDefList, "DList" }, **
 //{ epDefEntry, "DLEntry" }, **
 //{ epDefTerm, "DLTerm" }, **
 //{ epDefDef, "DLDef" }, **
 //{ epParmEntry, "PLEntry" }, **
 //{ epParmTerm, "PLTerm" }, **
 //{ epParmDef, "PLDef" }, **
 //{ epNavtitle, "Navtitle" }, **
 //{ epSDesc, "SDesc" }, **
 //{ epAbstr, "Abstr" }, **
 //{ epCascadeItem, "CascadeItem" }, **
 //{ epCascadeSet, "CascadeSet" }, **
 //{ epNonum, "NoNumber" }, **  // *
#endif
 NULL
};

DCnlist XMLud::Elements;
DCnlist *XMLud::CommonAttrs;
DCnlist XMLud::ElementDefs;
DCvlist XMLud::ElementAttrDefs;
bool XMLud::ElementsChanged = false;
bool XMLud::UseDescInText = false;

// these two are the only path-specific refs in drxml:
char *XMLud::LogIniPrefix = "_x2";
char *XMLud::StdLibPath = "%omsyshome%\\x2g\\lib";

char *XMLud::StdElemLib = "stdelems.mxl";
char *XMLud::StdAttrLib = "stdattrs.mxl";
char *XMLud::StdVarLib = "stdvars.mxl";
char *XMLud::StdGlossLib = "stdgloss.mxl";
char *XMLud::StdAbbrLib = "stdabbrs.mxl";
char *XMLud::StdTmarkLib = "stdtmarks.mxl";

char *XMLud::LocalElemLib = "localelems.mxl";
char *XMLud::LocalAttrLib = "localattrs.mxl";


char *
XMLud::GetLibPath(char *libname)
{
	char *path = GetEnvVars(StdLibPath);
	//char *home = getenv("omsyshome");
	//char *fname = NewName(home, strlen(home) + strlen(StdLibPath) + strlen(libname));
	char *fname = NewName(path, strlen(path) + strlen(libname) + 2);
	//strcat(fname, StdLibPath);
	if (fname[strlen(fname) - 1] != '\\')
		strcat(fname, "\\");
	strcat(fname, libname);
	return fname;
}


char *
XMLud::GetEnvVars(char *fn)
{
	char *pct = NULL;
	char *epct = NULL;
	char *enname = NULL;
	char *enval = NULL;
	char *nfn = NULL;
	uns len = 0;
	while ((pct = strchr(fn, '%')) != NULL) {  // environment variables present
		if ((epct = strchr(pct + 1, '%')) > (pct + 1)) {
			len = (epct - pct) - 1;
			enname = NewName(pct + 1, len);
			if ((enval = ::getenv(enname)) != NULL) {
				nfn = NewName(strlen(fn) + strlen(enval) - (len + 2));
				if (fn > pct)
					strncpy(nfn, fn, pct - fn);
				strcat(nfn, enval);
				strcat(nfn, epct + 1);
				fn = nfn;
				DeleteName(enname);
			}
			else {
				DeleteName(enname);
				break;
			}
		}
		else
			break;
	}
	return fn;
}


void
XMLud::ProcUMXLElemTypes(void)
{
	XMLrd::EpNames.init(EpMxlNamesInit);
	char *fname = NULL;

	// read common attribute defs from stdattrs.mxl
	fname = GetLibPath(StdAttrLib);
	ReadCommonAttrDefinitions(fname);
	DeleteName(fname);
	fname = GetLibPath(LocalAttrLib);
	ReadCommonAttrDefinitions(fname);
	DeleteName(fname);

	// read predefined element defs from stdelems.mxl
	fname = GetLibPath(StdElemLib);
	ReadElementDefinitions(fname);
	DeleteName(fname);
	fname = GetLibPath(LocalElemLib);
	ReadElementDefinitions(fname);
	DeleteName(fname);
}


void
XMLud::ReadElementDefinitions(char *fname, char *idref)
{
	XMLrd::LogEvent(loginfo, 1, "Reading element definitions from: ", fname);

	element *model = GetModel(fname);

	if (!model)
		XMLrd::LogEvent(logerr, 1, "Cannot read file of uDoc elements: ", fname);
	else
		ProcessElemDefModel(model, fname, idref);
}


void
XMLud::ReadCommonAttrDefinitions(char *fname)
{
	XMLrd::LogEvent(loginfo, 1, "Reading common attribute definitions from: ", fname);

	element *model = GetModel(fname);

	if (!model)
		XMLrd::LogEvent(logerr, 1, "Cannot read file of uDoc attributes: ", fname);
	else
		ProcessAttrModel(model, fname, "CommonAttrs");
}


element *
XMLud::GetModel(const char *fname)
{
	MxlParser *parser = new MxlParser();
	parser->SetOptions(false, true, false);  // model, not SAX
	parser->SetExpatCallbacks(NULL, XMLrfile::procError);

	element *model = parser->ParseFile(fname, NULL);
	delete parser;
	return model;
}


void
XMLud::ProcessAttrModel(element *model, char *fname, char *idref)
{
	// model is stdattrs.mxl, get wrapper elem with idref
	element *el = GetElemByID(model, idref);
	if (!el)
		XMLrd::LogEvent(loginfo, 1, "File of uDoc attributes: ", fname, " is missing \"", idref, "\" div.");
	else {
		SetElemAttrDefs(el);
		DCnlist *dcn = (DCnlist *) ElementAttrDefs.find((long) el);
		if (!CommonAttrs)
			CommonAttrs = dcn;
		else if (dcn
		 && dcn->next)
			CommonAttrs->addlist(dcn);
		ElementAttrDefs.remove((long) el);
	}
}


element * 
XMLud::GetElemByID(element *el, char *idref)
{
	long i = 0;
	element *nel = NULL;

	char *id = GetElemAttr(el, "id");
	if (id
	 && !stricmp(id, idref))
		return el;

	if (el->contcnt) {
		for (i = 0; i < el->contcnt; i++) {
			cont *ci = el->content[i];
			if (!ci->cnt) {
				if ((nel = GetElemByID((element *) (ci->it), idref)) != NULL)
					return nel;
			}
		}
	}

	return NULL;
}


void
XMLud::ProcessElemDefModel(element *model, char *fname, char *idref)
{
	long ocount = Elements.count();
	ElementsChanged = false;
	GetElemProps(model, idref);

	long ecount = Elements.count();
	if ((ecount == ocount)
	 && !ElementsChanged) {
		//long m = Elements.getitem("keyword");
		XMLrd::LogEvent(logerr, 2, "No uDoc elements changed or added in ", fname,
			                     idref? " with id = " : NULL, idref ? idref : NULL);
		return;
	}

	if (XMLrd::ElemTypes
	 && XMLrd::ElemTypesCount) {  // updating
		delete [] XMLrd::ElemTypes;
		XMLrd::ElemTypesCount = 0;
		DCelem::SetElemTypes(NULL, 0);
	}

	etype **et = (etype **) new char[sizeof(etype *) * ecount];
	DCnlist *enext = &Elements;
	long pos = 0;
	while ((enext = enext->next) != NULL)
		et[pos++] = (etype *) (enext->id);
	qsort(et, ecount, sizeof(etype *), DCelem::CompElemTypes);

	DCelem::SetElemTypes(et, ecount);
	XMLrd::ElemTypes = et;
	XMLrd::ElemTypesCount = ecount;
}


void
XMLud::GetElemProps(element *elp, char *idref)
{
	long i = 0;
	element *el = NULL;

	if (elp->content) {  // this is a parent element, like lib or div
		for (i = 0; i < elp->contcnt; i++) {
			if (elp->content[i]
			 && (elp->content[i]->cnt == 0)  // item is an element
			 && ((el = (element *) elp->content[i]->it) != NULL)) {
				if (el->namelen
				 && el->name) {
					if (CmpUnlStr(el->name, el->namelen, "element")
					 && !idref)
						GetOneElemProps(el);
					else if (CmpUnlStr(el->name, el->namelen, "wiki")
					 && !idref)
						GetOneWikiAttrs(el);
					else if (CmpUnlStr(el->name, el->namelen, "div")) {
						if (idref) {
							char *id = GetElemAttr(el, "id");
							if (id
							 && !stricmp(id, idref))
								idref = NULL;
							DeleteName(id);
						}
						GetElemProps(el, idref); // recurse
					}
					// otherwise it's not something that contains <element>, skip
				}
			}
		}
	}
}


DCnlist *XMLud::CurrWikiSyms = new DCnlist();
DCvlist XMLud::WikiSymsAttrs;
long XMLud::WikiSymsCount = 0;


void
XMLud::GetOneWikiAttrs(element *el)
{
	long i = 0;
	pair *attr = NULL;
	wikisym *sym = new wikisym;

	sym->symbol = 0;
	sym->tag = NULL;
	sym->sch = NULL;
	sym->ech = NULL;
	sym->space = true;
	sym->code = 0;

	if (el->attrs
	 && el->attrcnt) {
		for (i = 0; i < el->attrcnt; i++) {
			attr = el->attrs[i];
			if (CmpUnlStr(attr->name, attr->namelen, "symbol"))
				sym->symbol = GetUnlStr(attr->val, attr->vallen);
			else if (CmpUnlStr(attr->name, attr->namelen, "tag"))
				sym->tag = GetUnlStr(attr->val, attr->vallen);
			else if (CmpUnlStr(attr->name, attr->namelen, "ech"))
				sym->ech = GetUnlStr(attr->val, attr->vallen);
			else if (CmpUnlStr(attr->name, attr->namelen, "sch"))
				sym->sch = GetUnlStr(attr->val, attr->vallen);
			else if (CmpUnlStr(attr->name, attr->namelen, "space"))
				sym->space = BoolUnlStr(attr->val, attr->vallen);
			else if (CmpUnlStr(attr->name, attr->namelen, "code")) {
				if (CmpUnlStr(attr->val, attr->vallen, "set"))
					sym->code = 2;
				else if (BoolUnlStr(attr->val, attr->vallen))
					sym->code = 1;
			}
		}
	}

	if (sym->symbol) {
		long pos = 0;
		// add to list or replace in lists
		WikiSymsAttrs.add(sym, ++WikiSymsCount);
		if ((pos = CurrWikiSyms->getitem(sym->symbol)) == 0)
			CurrWikiSyms->add(sym->symbol, WikiSymsCount);
		else
			CurrWikiSyms->changeid(WikiSymsCount, pos);
	}
	else
		delete sym;
}



void
XMLud::GetOneElemProps(element *el)
{
	long i = 0;
	pair *attr = NULL;
	char *ename = NULL;
	bool named = false;
	char *proparr = NULL;
	bool props = false;

	if (el->attrs
	 && el->attrcnt) {
		for (i = 0; i < el->attrcnt; i++) {
			attr = el->attrs[i];
			if (CmpUnlStr(attr->name, attr->namelen, "name")) {
				ename = GetUnlStr(attr->val, attr->vallen);
				named = true;
			}
			else if (CmpUnlStr(attr->name, attr->namelen, "props")) {
				proparr = GetUnlStr(attr->val, attr->vallen);
				props = true;
			}
		}
	}
	if (named) {
		if ((i = ElementDefs.getitem(ename)) != 0)  // exists, replace
			ElementDefs.changeid((long) el, i);
		else
			ElementDefs.add(ename, (long) el);
		if (i)
			ElementAttrDefs.remove(i);
		SetElemAttrDefs(el);
		if (props) {
			GetElemPropContent(ename, proparr);
			DeleteName(proparr);
		}
	}
}


void
XMLud::GetElemPropContent(char *ename, char *propptr)
{
	long i = 0;
	cont *ccnt = NULL;
	char *spc = NULL;
	long pval = 0;

	if (!propptr)
		return;

	etype *etp = (etype *) new char[sizeof(etype)];
	etp->cnt = 0;
	etp->ename = ename;

	// convert arr to prop values
	while ((spc = strchr(propptr, ' ')) != NULL) {
		*spc = '\0';
		if ((pval = XMLrd::EpNames.getitem(propptr)) != 0)
			etp->eprop[etp->cnt++] = (unc) pval;
		else
			XMLrd::LogEvent(logwarn, 1, "Undefined element prop: ", propptr);
		*spc = ' ';
		propptr = spc + 1;
	}
	if (*propptr) { // last prop
		if ((pval = XMLrd::EpNames.getitem(propptr)) != 0)
			etp->eprop[etp->cnt++] = (unc) pval;
		else
			XMLrd::LogEvent(logwarn, 1, "Undefined element prop: ", propptr);
	}

	long pos = 0;

	if ((pos = Elements.getitem(ename)) != 0) {  // exists, replace
		Elements.changeid((long) etp, pos);
		ElementsChanged = true;
		delete [] (etype *) pos;
	}
	else
		Elements.add(ename, (long) etp);
}


void
XMLud::SetElemAttrDefs(element *el)
{
	// content of el is a usage elem and a set of attr elems
	cont *cnt = NULL;
	aprop *atprop = NULL;
	long i = 0;
	element *ec = NULL;
	long j = 0;
	pair *attr = NULL;
	DCnlist *lpt = NULL;
	char *estr = NULL;

	if (!el->contcnt)
		return;

	for (i = 0; i < el->contcnt; i++) {
		cnt = el->content[i];
		if (cnt->cnt == 0) {  // element *
			ec = (element *) cnt->it;
			if (CmpUnlStr(ec->name, ec->namelen, "attr")) {
				if (!lpt)
					ElementAttrDefs.add((lpt = new DCnlist), (long) el);
				if (!atprop)
					atprop = new aprop;
				for (j = 0; j < ec->attrcnt; j++) {
					attr = ec->attrs[j];
					if (CmpUnlStr(attr->name, attr->namelen, "name")) {
						atprop->name = GetUnlStr(attr->val, attr->vallen);
						lpt->add(atprop->name, (long) atprop);
					}
					else if (CmpUnlStr(attr->name, attr->namelen, "type")) {
						if (CmpUnlStr(attr->val, 5, "enum:")) {
							atprop->type = "enum";
							estr = GetUnlStr(attr->val + 6, attr->vallen - 6);
							atprop->evals = new DCnlist;
							atprop->evals->addlist(estr);
;							DeleteName(estr);
						}
						else  // a simple type
							atprop->type = GetUnlStr(attr->val, attr->vallen);
					}
					else if (CmpUnlStr(attr->name, attr->namelen, "default")) {
						atprop->deflt = GetUnlStr(attr->val, attr->vallen);
					}
				}
			}
			atprop = NULL;
		}
	}
}



// uDoc element identification

DCnlist XMLud::MissingElemDefs;
long XMLud::MissingElemCount = 0;


bool
XMLud::CheckUMXLElemName(const char *name, unl bit, unl neg, bool all)
{
	bool ret = false;

	DCelem *einfo = new DCelem(0);
	einfo->epset = false;
	einfo->epcnt = 0;
	einfo->eprop = NULL;

	etype **ep = NULL;

	etype *key = new etype;
	key->ename = name;

	etype **arr = XMLrd::ElemTypes;
	int cnt = XMLrd::ElemTypesCount;

	ep = (etype **) bsearch(&key, arr, cnt, sizeof(etype *),
																XMLrd::CompElemTypes);

	if (ep) {  // found
		einfo->epcnt = (*ep)->cnt;
		einfo->eprop = (*ep)->eprop;
		einfo->epset = true;
		ret = einfo->checkType(bit, neg, all);
	}
#if 0
	else {
		for (long i = 0; i < cnt; i++) {
			if (!stricmp(arr[i]->ename, name)) {
				ep = &arr[i];
				break;
			}
		}
		if (ep) {
			einfo->epcnt = (*ep)->cnt;
			einfo->eprop = (*ep)->eprop;
			einfo->epset = true;
			ret = einfo->checkType(bit, neg, all);
		}
#endif
		else if (!MissingElemDefs.getitem(name)) {
			MissingElemDefs.add(name, ++MissingElemCount);
			XMLrd::LogEvent(logwarn, 1, "Properties not found for ", name);
		}
	//}

	delete key;
	delete einfo;
	return ret;
}



// udoc transclusion

bool XMLud::GettingElem = false;
char *XMLud::XMLProjectRoot = NULL;
element *XMLud::XMLProjectRootElem = NULL;


bool 
XMLud::GetUMXLElement(char *fname, const char *elemids, bool nomsg)
{
	bool ret = false;

	if (GettingElem) { // recurse
		ret = GetNewElem(fname, elemids, nomsg);
		return ret;
	}
	GettingElem = true;
	ret = GetNewElem(fname, elemids, nomsg);
	GettingElem = false;
	if (XMLProjectRootElem == NULL) {
		XMLProjectRoot = NewName(fname);
		XMLProjectRootElem = (element *) ParsedModels.find(1);
	}
	return ret;
}


element *XMLud::CurrModel = NULL;
element *XMLud::CurrElement = NULL;
DCnlist XMLud::ParsedFiles;   // name of file
DCvlist XMLud::ParsedModels;  // element * of model
DCvlist XMLud::ParsedIDs;     // DCnlist * of IDs with element * as index
DCvlist XMLud::ParsedParents; // DCvlist * of parents with element * as index
long XMLud::ParsedCount = 0;


bool 
XMLud::GetNewElem(char *fname, const char *elemids, bool nomsg)
{
	long modelPos = 0;
	element *model = NULL;
	element *lastmodel = CurrModel;
	DCnlist *elemIDs = NULL;
	DCvlist *elemParents = NULL;

	if (!fname) {  // use current model
		if (!elemids) {
			XMLrd::LogEvent(logerr, 1, "Cannot recurse entire surrent file");
			return false;  // prevent infinite recursion
		}
		model = CurrModel;
		modelPos = ParsedModels.getitem(model);
		elemIDs = (DCnlist *) ParsedIDs.find(modelPos);
		elemParents = (DCvlist *) ParsedParents.find(modelPos);
	}
	// see if file already read; if so, use stored model
	else if ((modelPos = ParsedFiles.getitem(fname)) != 0) {
		CurrModel = model = (element *) ParsedModels.find(modelPos);
		elemIDs = (DCnlist *) ParsedIDs.find(modelPos);
		elemParents = (DCvlist *) ParsedParents.find(modelPos);
	}
	// else open fname with new parser, get model, and index IDs
	else {
		MxlParser *parser = new MxlParser();
		parser->SetOptions(false, true, false);  // model, not SAX
		parser->SetExpatCallbacks(NULL, XMLrfile::procError);
		if (!nomsg)
			XMLrd::LogEvent(loginfo, 1, "Reading included content from: ", fname);
		if ((model = parser->ParseFile(fname, NULL)) == NULL) {
			XMLrd::LogEvent(logerr, 1, "Cannot read file: ", fname);
			delete parser;
			return false;
		}
		CurrModel = model;
		modelPos = ++ParsedCount;
		ParsedFiles.addnew(fname, modelPos);
		ParsedModels.add(model, modelPos);
		elemIDs = new DCnlist();
		elemParents = new DCvlist();
		GetCurrElemIDs(model, elemIDs, NULL, elemParents);
		ParsedIDs.add(elemIDs, modelPos);
		ParsedParents.add(elemParents, modelPos);
		delete parser;
	}

	// parse series of IDs; if range, ensure same parent elem
	// go to first ID and call our Start, End, and Content funcs
  // if range of two IDs, continue in doc order to end of the second one
  // if more IDs, read each in order given; may not be doc order


	char *currID = (char *) elemids;
	char *spc = NULL;
	DCnlist *idlist = new DCnlist();
	long idcnt = 0;

	if (currID) {
		idlist->addlist(currID);
		idcnt = idlist->count();
#if 0
		while (((spc = strchr(currID, ' ')) != NULL)
		 || ((spc = strchr(currID, ',')) != NULL)) {
			char sp = *spc;
			*spc = '\0';
			idlist->addnew(currID, ++idcnt);
			*spc = sp;
			currID = spc + 1;
		}
		if (*currID) // last prop
			idlist->addnew(currID, ++idcnt);
#endif
	}

	if (idcnt == 0)  // process entire file
		ProcElemContent(model);
	else {  // read each in order
		bool ret = true;
		char *sep = NULL;
		for (long i = 1; i <= idcnt; i++) {
			currID = idlist->find(i);
			if ((sep = strchr(currID, '-')) != NULL) { // range
				char *start = NewName(currID, sep - currID);
				char *end = NewName(sep + 1);
				ret = ProcRange(start, end, elemIDs, elemParents, nomsg);
				DeleteName(end);
				DeleteName(start);
			}
			else
				ret = ProcElem(currID, elemIDs, nomsg);
		}
		CurrModel = lastmodel;
		return ret;  // false if any fail
	}
	CurrModel = lastmodel;
	return true;
}


bool
XMLud::ProcRange(char *start, char *end, DCnlist *elemIDs,
	               DCvlist *elemParents, bool nomsg)
{
	element *startelem = (element *) elemIDs->getitem(start);
	if (!startelem) {
		if (!nomsg)
			XMLrd::LogEvent(logerr, 1, "Cannot find starting element: ", start);
		return false;
	}
	element *endelem = (element *) elemIDs->getitem(end);
	if (!endelem) {
		if (!nomsg)
			XMLrd::LogEvent(logerr, 1, "Cannot find ending element: ", end);
		return false;
	}
	element *parent = (element *) elemParents->find((long) startelem);
	if (!parent) {  // must be root element
		ProcElemContent(startelem);
		return true;
	}
	if (parent != elemParents->find((long) endelem)) {
		if (!nomsg)
			XMLrd::LogEvent(logerr, 1, "Starting and ending elements have different parents: ",
															 start, ", ", end);
		return false;
	}
	bool started = false;
	bool ending = false;
	for (long i = 0; i < parent->contcnt; i++) {
		cont *ci = parent->content[i];
		if (!ci->cnt) {  // element
			if (startelem == (element *) ci->it)
				started = true;
			if (endelem == (element *) ci->it)
				ending = true;
			if (started)
				ProcElemContent((element *) ci->it);
			if (ending)
				break;
		}
		else if (started)  // text content
			ProcTextContent(GetUnlStr((unl *) ci->it, ci->cnt));
	}
	return true;
}


bool
XMLud::ProcElem(char *currID, DCnlist *elemIDs, bool nomsg)
{
	element *IDelem = (element *) elemIDs->getitem(currID);
	if (!IDelem) {
		if (!nomsg)
			XMLrd::LogEvent(logerr, 1, "Cannot find element: ", currID);
		return false;
	}
	else
		ProcElemContent(IDelem);
	return true;
}


void 
XMLud::GetCurrElemIDs(element *el, DCnlist *lp, element *ep, DCvlist *lep)
{
	long i = 0;

	char *id = GetElemAttr(el, "id");
	if (id) {
		lp->add(id, (long) el);
		lep->add(ep, (long) el);
	}

	if (el->contcnt) {
		for (i = 0; i < el->contcnt; i++) {
			cont *ci = el->content[i];
			if (!ci->cnt)
				GetCurrElemIDs((element *) (ci->it), lp, el, lep);
		}
	}
}


void  
XMLud::ProcElemContent(element *el)
{
	// call normal start, text, and end callbacks
	char *tag = GetUnlStr(el->name, el->namelen);
	long i = 0;
	char **attrs8 = NULL;
	pair *attr = NULL;
	char *attstr = NULL;

	if (XMLrd::InGlossary
	 && (CheckUMXLElemName(tag, epData)
		|| (CheckUMXLElemName(tag, epDesc)
	   && !UseDescInText))) {
		DeleteName(tag);
		return;
	}

	CurrElement = el;

	if (el->attrcnt) {
		attrs8 = new char*[(el->attrcnt * 2) + 1];
		for (i = 0; i < el->attrcnt; i++) {
			attr = el->attrs[i];
			if (!attr)
				break;
			attstr = GetUnlStr(attr->name, attr->namelen);
			attrs8[i * 2] = attstr;
			attstr = GetUnlStr(attr->val, attr->vallen);
			attrs8[(i * 2) + 1] = attstr;
		}
		attrs8[i * 2] = NULL;
	}

	attrs8 = CheckAttrs(tag, attrs8, i * 2);

	XMLrd::StartElement(tag, (const char **) attrs8, XMLrd::CurrDepth + 1);

	if (el->attrcnt) {
		for (i = 0; i < (el->attrcnt * 2); i++)
			DeleteName(attrs8[i]);
		delete [] attrs8;
		attrs8 = NULL;
	}

	for (i = 0; i < el->contcnt; i++) {
		cont *ci = el->content[i];
		if (!ci->cnt)  // element
			ProcElemContent((element *) ci->it);
		else  // text
			ProcTextContent(GetUnlStr((unl *) ci->it, ci->cnt));
	}

	CurrElement = el;
	XMLrd::EndElement(tag, XMLrd::CurrDepth);

	//CurrElement = CurrElement->parent;
}


void  
XMLud::ProcTextContent(char *txt)
{
	// call text callback
	XMLrd::ProcChar(txt, strlen(txt), XMLrd::CurrDepth + 1);
}


char **  
XMLud::CheckAttrs(const char *tag, char **attrs, long idx)
{
	long nidx = idx;
	element *el = (element *) ElementDefs.getitem(tag);
	if (!el)
		return attrs;

	DCnlist *elatts = (DCnlist *) ElementAttrDefs.find((long) el);
	if (!elatts)
		return attrs;

	long i = 0;
	aprop *atprop = NULL;

	for (i = 0; i < idx; i += 2) { // check all present
		if (((atprop = (aprop *) elatts->getitem(attrs[i])) != NULL)
		 || ((atprop = (aprop *) CommonAttrs->getitem(attrs[i])) != NULL)) {
			 // check atprop->type against attrs[i + 1];
			if (atprop->evals) {  // enum
				if (!atprop->evals->getitem(attrs[i + 1]))
					XMLrd::LogEvent(logwarn, 1, tag, " attribute ", attrs[i],
					" has invalid enum value: ", attrs[i + 1]);
			}
		}
		else  // not found
			XMLrd::LogEvent(logwarn, 1, "Element ", tag, " has undefined attribute: ", attrs[i]);
	}

	// provide defaults for any missing enums
	DCnlist nattrs;

	while ((elatts = elatts->next) != NULL) {
		atprop = (aprop *) elatts->id;
		if (atprop->deflt) {
			for (i = 0; i < idx; i += 2) { // check all present
				if (!stricmp(atprop->name, attrs[i]))
					break;
			}
			if (i == idx)  // not there
				nattrs.add(atprop->name, (long) atprop->deflt);
		}
	}

	DCnlist *nptr = &nattrs;

	if (nattrs.next) {
		long tidx = nidx + (nattrs.count() * 2);
		char **newattrs = new char*[tidx + 1];
		memcpy(newattrs, attrs, nidx * sizeof(char *));
		delete [] attrs;
		attrs = newattrs;
		while ((nptr = nptr->next) != NULL) {
			attrs[nidx++] = (char *) nptr->name;
			attrs[nidx++] = (char *) nptr->id;
		}
		attrs[nidx] = NULL;
	}

	return attrs;
}



// conditional processing

condset *XMLud::CurrConds;
condset *XMLud::OutputConds;
condset *XMLud::PendConds;
DCvlist XMLud::CondSets;
DCnlist XMLud::CondSetNames;
DCnlist XMLud::CondSetFiles;
DClist XMLud::CondSetsPend;
long XMLud::CondSetCount = 0;

DCvlist XMLud::CondSetLevs;  // stack for restores at end of scope
long XMLud::CurrCondSetLev = 0;
long XMLud::CurrCondRefLev = 1;
char *XMLud::CurrCondRefFile = NULL;

#ifdef UDOC

void
XMLud::ProcCondition(DCelem *elem)
{
	if (elem->checkType(epRef)) {  // reference to condition settings
		XMLrd::SetTextRef(elem, trcond);                      // condref
		CurrCondRefFile = elem->href;
	}
	else if (elem->checkType(epGroup)) {  // starting condition set
		SetNewCondSet(elem, CurrCondRefFile);
	}
	else if (elem->checkType(epDef)) {  // single condition setting
		SetNewCond(elem);
	}
}


void
XMLud::EndCondition(DCelem *elem)
{
	if (elem->checkType(epRef)) {
		XMLrd::GetTextRef(trcond);
	}
	else if (elem->checkType(epGroup)) {  // ending condition set
		EndNewCondSet(elem);
	}
	else if (elem->checkType(epDef)) { // ending one condition
		EndNewCond(elem);
	}
}
#endif


void
XMLud::SetNewCondSet(DCelem *elem, char *refsrc)
{
	if (!elem)
		return;

	if (InOutput
	 && !UseOutput)
		return;

	PendConds = NULL;

	// set up set named by elem->id in elem->href
	// if already read, don't process conds again
	long cond = 0;
	condset *currcond = NULL;

	if ((cond = CondSetNames.getitem(elem->id)) == 0) {
		currcond = new condset;
		CondSetNames.add(elem->id, ++CondSetCount);
		CondSets.add(currcond, CondSetCount);
		currcond->id = elem->id;
		currcond->srcfile = refsrc;
		currcond->attrcnt = 0;
		cond = CondSetCount;
	}
	else if (((currcond = (condset *) CondSets.find(cond)) == NULL)
	 || stricmp(currcond->srcfile, refsrc)) {  // errors
		XMLrd::LogEvent(logwarn, 1, "condition set ", elem->id, " missing or duplicated.");
		return;
	}

	PendConds = currcond;  // ready to add cond elem info
}


void
XMLud::EndNewCondSet(DCelem *elem)
{
	if (!PendConds
	 ||!elem)
		return;

	if (InOutput) {
		if (UseOutput)
			OutputConds = PendConds;
		return;
	}

	// store prev one if any on CondSetLevs stack by CurrCondSetLev
	if (CurrConds
	 && (CurrCondSetLev < XMLrd::CurrDepth - 1))
		CondSetLevs.add(CurrConds, CurrCondSetLev);

	// make new one current
	CurrCondSetLev = XMLrd::CurrDepth - 1;  // depth of conref
	CurrConds = PendConds;
}


void
XMLud::SetNewCond(DCelem *elem)
{
	if (!PendConds
	 || !elem
	 || !elem->attrcnt)
		return;

	if (InOutput
	 && !UseOutput)
		return;

	char *att = NULL;
	bool incl = false;
	bool excl = false;
	bool flag = false;
	char *expr = NULL;
	char *fclass = NULL;
	long pos = 0;

	if ((pos = elem->attrnames->getitem("attr")) != 0)
		att = elem->attrvals->find(pos);
	if ((pos = elem->attrnames->getitem("include")) != 0) {
		if (excl) {
			XMLrd::LogEvent(logwarn, 1, "cond has both include and exclude, ignored");
			return;
		}
		expr = elem->attrvals->find(pos);
		incl = true;
	}
	if ((pos = elem->attrnames->getitem("exclude")) != 0) {
		if (incl) {
			XMLrd::LogEvent(logwarn, 1, "cond has both include and exclude, ignored");
			return;
		}
		expr = elem->attrvals->find(pos);
		excl = true;
	}
	if ((pos = elem->attrnames->getitem("flag")) != 0) {
		if (incl || excl) {
			XMLrd::LogEvent(logwarn, 1, "cond has flag expression with include or exclude, ignored");
			return;
		}
		expr = elem->attrvals->find(pos);
		flag = true;
	}
	if ((pos = elem->attrnames->getitem("class")) != 0) {
		flag = true;
		fclass = elem->attrvals->find(pos);
	}

	if (flag && !fclass) {
		XMLrd::LogEvent(logwarn, 1, "cond has flag with no class, ignored");
		return;
	}
	if (!att) {
		XMLrd::LogEvent(logwarn, 1, "cond is missing attribute name, ignored");
		return;
	}
	if (!expr) {
		XMLrd::LogEvent(logwarn, 1, "cond is missing value expression, ignored");
		return;
	}

	// all looks good, add it
	pos = ++(PendConds->attrcnt);
	PendConds->attrs.add(att, pos);
	PendConds->exprs.add(expr, pos);
	if (incl)
		PendConds->incl.add(pos);
	if (excl)
		PendConds->excl.add(pos);
	if (flag) {
		PendConds->flag.add(pos);
		PendConds->fclass.add(fclass, pos);
	}
}


void
XMLud::EndNewCond(DCelem *elem)
{
	if (!PendConds
	 ||!elem)
		return;

	if (InOutput
	 && !UseOutput)
		return;

	// empty element, not needed
}


void
XMLud::RestoreCondSet(void)
{
	// restore previous set if any at end of curr set scope
	if (CurrCondSetLev <= XMLrd::CurrDepth)
		return;

	condset *cset = NULL;

	for (long i = XMLrd::CurrDepth; i > 0; i--) {
		if ((cset = (condset *) CondSetLevs.find(i)) != NULL) {
			CurrConds = cset;
			CurrCondSetLev = i;
			CondSetLevs.remove(i);
			return;
		}
	}
}



bool
XMLud::CheckConds(const char **atts)
{
	if (!atts)
		return true;

	bool ret = true;

	if (XMLrd::CheckFilterConds(atts))
		return true;

	if (OutputConds
	 && OutputConds->attrcnt)
		ret = CheckCondType(atts, OutputConds);

	if (!ret)
		return false;

	if (CurrConds
	 && CurrConds->attrcnt)
		ret = CheckCondType(atts, CurrConds);

	return ret;
}


bool
XMLud::CheckCondType(const char **atts, condset *conds)
{
	// return true to include, false to exclude
	// if no attr of elem is in CurrConds->attrs, include
	long cond = 0;
	const char *val = NULL;
	char *expr = NULL;
	DCnlist *varr = NULL;
	bool exptrue = false;
	DClist *vused = NULL;
	bool ret = true;
	char *fcl = NULL;

	for (long i = 0; atts[i] != NULL; i += 2) {
		if ((cond = conds->attrs.getitem(atts[i])) != 0) {
			val = atts[i + 1];  // one or more values
			if (val) {
				varr = new DCnlist;
				vused = new DClist;
				varr->addlist(val);
				expr = conds->exprs.find(cond);
				exptrue = CheckCondExpr(&expr, varr, vused);
				// if an attribute value is not mentioned in the expr,
				//  include regardless of expr result (as in DITA)
				if (vused->count() == varr->count()) { // all values used
					if (conds->incl.find(cond)) // an include
						ret = exptrue;
					else if (conds->excl.find(cond)) // an exclude
						ret = !exptrue;
					if (ret  // will include
					 && (conds->flag.find(cond))
					 && ((fcl = conds->fclass.find(cond)) != NULL))
						CheckCondFlags(fcl);
				}
				delete varr;
				delete vused;
				// if any attr that has an expr excludes, then exclude
				if (!ret)
					break;
			}
		}
	}

	return ret;
}


bool
XMLud::CheckCondExpr(char **expr, DCnlist *dcnp, DClist *dcp)
{
	// expr has leftarg, op, rightarg
	// if arg is in dcnp, it is true, else false
	// if arg is preceded by not or !, reverse that
	// add index of each arg found to dcp
	// ops are and, or, &&, ||
	// if left paren, recurse and treat result as arg
	// if right paren, return

	bool ret = false;
	bool lset = false;
	bool lval = false;
	bool rset = false;
	bool rval = false;
	bool op = false;
	bool neg = false;
	bool opand = false;
	bool opor = false;
	char *exp = *expr;


	while (*exp) {
		if (*exp == ')') {
			exp++;
			break;
		}

		if (lset && op && rset)
			break;

		if (isascii(*exp)
		 && isspace(*exp)) {
			exp++;
			continue;
		}

		if (*exp == '!') {
			neg = true;
			exp++;
			continue;
		}

		if (!strnicmp(exp, "not", 3)
		 && !isalnum(*(exp + 3))) {
			neg = true;
			exp += 3;
			continue;
		}

		// get left arg
		if (!lset) {
			lval = GetCondArg(&exp, dcnp, dcp);
			if (neg) {
				lval = !lval;
				neg = false;
			}
			lset = true;
			continue;
		}

		// get op
		if (!op) {
			if (!strncmp(exp, "&&", 2)) {
				opand = op = true;
				exp += 2;
			}
			else if (!strncmp(exp, "||", 2)) {
				opor = op = true;
				exp += 2;
			}
			else if (!strnicmp(exp, "and", 3)
			 && !isalnum(*(exp + 3))) {
				opand = op = true;
				exp += 3;
			}
			else if (!strnicmp(exp, "or", 2)
			 && !isalnum(*(exp + 2))) {
				opor = op = true;
				exp += 2;
			}
			continue;
		}
		
		// get right arg
		if (!rset) {
			rval = GetCondArg(&exp, dcnp, dcp);
			if (neg) {
				rval = !rval;
				neg = false;
			}
			rset = true;
			continue;
		}
	}

	if (!op && !rset)  // just one arg
		ret = neg ? !lval : lval;
	else if (opand)
		ret = lval && rval;
	else if (opor)
		ret = lval || rval;
	else
		XMLrd::LogEvent(logwarn, 1, "Error in condition expression: ", *expr);

	*expr = exp;
	return ret;
}


bool
XMLud::GetCondArg(char **expr, DCnlist *dcnp, DClist *dcp) 
{
	bool neg = false;
	bool val = false;
	char *exp = *expr;
	char *word = NULL;
	long pos = 0;

	if (*exp == '(') {
		exp++;
		val = CheckCondExpr(&exp, dcnp, dcp);
	}
	else {
		if (*exp == '!') {
			neg = true;
			exp++;
		}
		if (!strnicmp(exp, "not", 3)
		 && !isalnum(*(exp + 3))) {
			neg = true;
			exp += 3;
		}

		word = GetCondWord(&exp);
		if ((pos = dcnp->getitem(word)) != 0) {
			val = true;  // was present
			if (dcp)
				dcp->add(pos);
		}
		else
			val = false;	
	}

	*expr = exp;
	return val;
}


char *
XMLud::GetCondWord(char **expr)
{
	// skip leading spaces, return ptr to word
	// set expr after any trailing spaces
	char *cp = *expr;
	while (isascii(*cp)
	 && isspace(*cp))
		cp++;

	char *ep = cp;
	while (*ep
	 && isascii(*ep)
	 && isalnum(*ep))
		ep++;

	uns len = ep - cp;
	cp = NewName(cp, len);
	cp[len] = '\0';

	while (isascii(*ep)
	 && isspace(*ep))
		ep++;
	*expr = ep;

	return cp;
}


void
XMLud::CheckCondFlags(const char *fcl)
{
	long fval = 0;
	DClist *flist = NULL;

	if ((fval = XMLrd::CondFlagNames.getitem(fcl)) == 0)
		XMLrd::CondFlagNames.add(fcl, (fval = ++XMLrd::CondFlagCount));
	if ((flist = (DClist *) XMLrd::CondFlagLevels.find(XMLrd::CurrDepth)) == NULL)
		XMLrd::CondFlagLevels.add(flist = new DClist(), XMLrd::CurrDepth);
	flist->add(fval);
	XMLrd::DoFlag = true;
}


bool
XMLud::SetCondFlags(long depth)
{
	return XMLrd::SetFlags(depth);
}


void
XMLud::EndCondFlags(void)
{
	XMLrd::EndFlags();
}


void
XMLud::ProcCondFlagDef(DCgroup *gp, long num)
{
  DCgroup *cgp = new DCgroup(DCctl(group, 2, 60, (uns) num));
	gp->add(*cgp);

	char *txt = XMLrd::CondFlagNames.find(num);
	if (txt) {
		DCctl *cp = new DCctl(ename, 2, 61, 1);
		cp->ex(txt, strlen(txt));
		cgp->add(*cp);
	}
	else
		assert(0);  // nameless flag
}



// output processing

bool XMLud::InOutput = false;
bool XMLud::UseOutput = false;
bool XMLud::SourceKeys = false;
bool XMLud::OutputKeys = false;
char *XMLud::OutputKeyref = NULL;
char *XMLud::OutputKeyID = NULL;
DCvlist XMLud::OutputKeyList;
long XMLud::OutputKeyCount = 0;
DCgroup *XMLud::OutputKeyDefs = NULL;

DCnlist XMLud::ExtProjPaths;
DCvlist XMLud::ExtProjModels;
long XMLud::ExtProjCount = 1;
DCvlist XMLud::ExtProjOutputs;
DCvlist XMLud::ExtProjKeysets;
long XMLud::ExtProjOutCount = 0;


void
XMLud::SetOutputType(char *type)
{
	InOutput = true;
	UseOutput = false;
	SourceKeys = false;
	OutputKeys = false;
	OutputKeyref = NULL;

	if (!type)
		return;

	if (!stricmp("udoc", type)) {
		UseOutput = true;
		SourceKeys = true;
	}
	else if (XMLrd::OutTypeName
	 && CheckOutputExpr(type, XMLrd::OutTypeName)) {
		UseOutput = true;
		OutputKeys = true;
	}
}


bool 
XMLud::CheckOutputExpr(char *expr, const char *type)
{
	DCnlist *varr = new DCnlist;
	varr->add(type, 1);
	return CheckCondExpr(&expr, varr);
}


void
XMLud::ProcOutputKeys(void)
{
	// create group for gen_key defs, set output type XMLrd::OutTypeName
	// and lib to write, XMLud::OutputKeyref, with output dir @src
	if (!OutputKeyCount
	 || !XMLrd::OutTypeName
	 || !OutputKeyref)
		return;  // no keys are public

	OutputKeyDefs = new DCgroup(DCctl(group, 2, 113, 1));
	DCctl *cp = new DCctl(etext, 2, 113, 2);
	cp->ex(XMLrd::OutTypeName);
	OutputKeyDefs->add(*cp);

	cp = new DCctl(etext, 2, 113, 3);
	cp->ex(OutputKeyref);
	OutputKeyDefs->add(*cp);

	cp = new DCctl(etext, 2, 113, 7);
	cp->ex(OutputKeyID);
	OutputKeyDefs->add(*cp);

	// for all items in the OutputKeyList, write ctls for id and ref
	// the ref needs to be to a file, the idref is separate
	DCgroup *kgp = NULL;
	DCvlist *nlp = &OutputKeyList;
	DCelem *el = NULL;

	while ((nlp = nlp->next) != NULL) {
		kgp = new DCgroup(DCctl(group, 2, 113, 4));
		OutputKeyDefs->add(* (DCctl *) kgp);
		el = (DCelem *) nlp->vp;
		if (!el)
			continue;
		cp = new DCctl(etext, 2, 113, 5);
		cp->ex(el->keys);
		kgp->add(*cp);

		cp = new DCctl(etext, 2, 113, 6);
		cp->ex(el->href);
		kgp->add(*cp);

		cp = new DCctl(etext, 2, 113, 8);
		cp->ex(el->idref);
		kgp->add(*cp);
	}

	// add group to resources in WriteDCLHead, for processing by dw*
}


void
XMLud::GetOutputKeys(const char *project, const char *output)
{
	// when any key specifies a different project, get the keys
	// for the current output XMLrd::OutTypeName from it
	// log warning if not available

	if (!output)
		output = XMLrd::OutTypeName;

	if (!output)
		return;

	long currprojnum = 0;
	element *model = NULL;

	if (!project) {
		model = XMLProjectRootElem;
		project = XMLProjectRoot;
		currprojnum = 1;
	}
	else if ((currprojnum = ExtProjPaths.getitem(project)) == 0) {
		ExtProjPaths.add(project, (currprojnum = ++ExtProjCount));
		model = GetModel(project);
		ExtProjModels.add(model, currprojnum);
	}
	else
		model = (element *) ExtProjModels.find(currprojnum);

	if (!model)
		return;

	long curroutnum = 0;
	DCnlist *kdouts = NULL;

	if ((kdouts = (DCnlist *) ExtProjOutputs.find(currprojnum)) == NULL) {
		kdouts = new DCnlist();
		ExtProjOutputs.add(kdouts, currprojnum);
	}

	if ((curroutnum = kdouts->getitem(output)) != 0) // already have this keyset
		return;

	kdouts->add(output, (curroutnum = ++ExtProjOutCount));

	DClist *outputs = new DClist();
	GetNamedElems(model, "output", outputs);
	if (!outputs->next)
		return;

	DClist *dlp = outputs;
	element *elp = NULL;
	element *kr = NULL;
	bool found = false;
	while ((dlp = dlp->next) != NULL) {
		elp = (element *) dlp->id;
		if (!elp)
			break;
		if (CheckOutputExpr(GetElemAttr(elp, "type"), output)) {
			if ((kr = GetElemChild(elp, "keyref")) != NULL) {
				found = true;
				break;
			}
		}
	}
	if (!found)
		return;


	bool isrtf = (!stricmp(output, "rtf")) ? true : false;

	// kr is the correct keyref, src is lib to read
	char *klib = GetElemAttr(kr, "src");
	if (!klib)
		return;
	char *bs = strrchr(project, '\\');
	if (bs)
		*bs = '\0';
	char *klpath = NewName(project, strlen(project) + strlen(klib) + 1);
	strcat(klpath, "\\");
	strcat(klpath, klib);
	if (bs)
		*bs = '\\';
	element *kmodel = GetModel(klpath);
	if (!kmodel)
		return;
	char *krpath = GetElemAttr(kmodel, "src");
	if (!isrtf) {
		char kend = krpath[strlen(krpath) - 1];
		if ((kend != '/')
		 && (kend != '#')) {
			krpath = NewName(krpath, strlen(krpath) + 2);
			strcat(krpath, "/");
		}
	}
	else if (stricmp("rtf", XMLrd::OutTypeName)) {
		if (krpath[4] != ':') {
			char *npath = NewName("file:///", strlen(krpath) + 8);
			strcat(npath, krpath);
			DeleteName(krpath);
			krpath = npath;
		}
	}
	DClist *keydefs = new DClist();
	GetNamedElems(kmodel, "key", keydefs);
	if (!keydefs->next)
		return;

	// make kist of keys and src pairs, indexed by keys
	DCnlist *kdprops = new DCnlist();
	DClist *kdp = keydefs;
	element *klp = NULL;
	char *kpath = NULL;
	char *kfpath = NULL;
	found = false;
	while ((kdp = kdp->next) != NULL) {
		klp = (element *) kdp->id;
		if (!klp)
			break;
		kpath = GetElemAttr(klp, "src");
		kfpath = NewName(krpath, strlen(krpath) + strlen(kpath) + 1);
		strcat(kfpath, kpath);
		kdprops->add(GetElemAttr(klp, "keys"), (long) kfpath);
	}

	// add the results to the lookup lists
	ExtProjKeysets.add(kdprops, curroutnum);
}


char *
XMLud::ResolveOutputKey(const char *project, const char *key, const char *output)
{
	// resolve a key that points to another project
	// return src, or NULL (and warn) if not available


	long kset = project ? ExtProjPaths.getitem(project) : 1;
	if (!kset) {
		GetOutputKeys(project, output);
		if ((kset = ExtProjPaths.getitem(project)) == 0) {
			XMLrd::LogEvent(logwarn, 1, "Cannot find referenced project: ", project);
			XMLrd::LogEvent(logcont, 1, "so failed to get reference for key ", key);
			return NULL;
		}
	}

	if (!output)
		output = XMLrd::OutTypeName;
	DCnlist *ko = (DCnlist *) ExtProjOutputs.find(kset);
	long onm = 0;
	DCnlist *ksl = NULL;

	if ((kset == 1)
	 && (!ko
	  || ((onm = ko->getitem(output)) == 0)
	  || ((ksl = (DCnlist *) ExtProjKeysets.find(onm)) == NULL))) { // this output not present
		GetOutputKeys(NULL, output);
		ko = (DCnlist *) ExtProjOutputs.find(kset);
	}

	if ((kset > 1)
	 && (!ko
	  || ((onm = ko->getitem(output)) == 0)
	  || ((ksl = (DCnlist *) ExtProjKeysets.find(onm)) == NULL))) { // this output not present
		GetOutputKeys(project, output);
		ko = (DCnlist *) ExtProjOutputs.find(kset);
	}

	if (!ko
	 || ((onm = ko->getitem(output)) == 0)
	 || ((ksl = (DCnlist *) ExtProjKeysets.find(onm)) == NULL)) { // this output not present
		if (project)
			XMLrd::LogEvent(logwarn, 1, "Cannot find keydefs for ", output,
				               " in project: ", project);
		else
			XMLrd::LogEvent(logwarn, 1, "Cannot find output keys for current project: ");
		XMLrd::LogEvent(logcont, 1, "so failed to get reference for key ", key);
		return NULL;
	}

	long knum = ksl->getitem(key);
	if (!knum) {
		XMLrd::LogEvent(logwarn, 1, "Cannot find keydef for key ", key,
			               " in project: ", project);
		XMLrd::LogEvent(logcont, 1, "for current output ", XMLrd::OutTypeName);
		return NULL;
	}

	return (char *) knum;;
}




// glossary functions

listrefinfo XMLud::GlossRefs;
listdefinfo XMLud::GlossDefs;


void 
XMLud::PutAllGlossDefs(void)
{
	PutAllListDefs(&GlossDefs, gengloss);
}


void 
XMLud::SetGlossRefFile(char *srcfile, char *srcid)
{
	XMLrd::LogEvent(loginfo, 1, "Reading glossary defs from: ", srcfile);
	SetListRefFile(srcfile, srcid, &GlossRefs);
}



// abbreviation functions

listrefinfo XMLud::AbbrRefs;
listdefinfo XMLud::AbbrDefs;
bool  XMLud::AbbrsInGloss = true;


void
XMLud::AbbrResetUse(void)
{
	ListDefResetUse(&AbbrDefs);
}


void 
XMLud::PutAllAbbrDefs(void)
{
	PutAllListDefs(&AbbrDefs, genabbr);
}


void 
XMLud::SetAbbrRefFile(char *srcfile, char *srcid)
{
	XMLrd::LogEvent(loginfo, 1, "Reading abbrdefs from: ", srcfile);
	SetListRefFile(srcfile, srcid, &AbbrRefs);
}



// trademark functions

listrefinfo XMLud::TmarkRefs;
listdefinfo XMLud::TmarkDefs;


void
XMLud::TmarkResetUse(void)
{
	ListDefResetUse(&TmarkDefs);
}


void 
XMLud::PutAllTmarkDefs(void)
{
	PutAllListDefs(&TmarkDefs, gentmk);
}


void 
XMLud::SetTmarkRefFile(char *srcfile, char *srcid)
{
	XMLrd::LogEvent(loginfo, 1, "Reading tmarkdefs from: ", srcfile);
	SetListRefFile(srcfile, srcid, &TmarkRefs);
}




// list processing for gloss, abbr, tmark


void
XMLud::SetUpLists(void)
{
	SetListRefFile(GetLibPath(StdGlossLib), "stdgloss", &GlossRefs);
	SetListRefFile(GetLibPath(StdAbbrLib), "stdabbrs", &AbbrRefs);
	SetListRefFile(GetLibPath(StdTmarkLib), "stdtmarks", &TmarkRefs);
}


void
XMLud::ListElemRefStart(DCelem *elem, char *fname, long depth, gentyp ltyp)
{
	char *ref = elem->idrefs ? elem->idrefs : elem->idref;

	XMLrd::InGlossTerm = true;
	XMLrd::FlushText();
	elem->isEmpty = true;
	if (!elem->href) {
		if (!fname
		 || (ltyp == genabbr)
		 || (ltyp == gentmk))
		 //|| !stricmp(fname, XMLrd::TmarkFileName)
		 //|| !stricmp(fname, XMLrd::AbbrFileName))
			fname = XMLrd::GlossaryFileName;
		if (GetListDefElem(elem->idref, &GlossRefs) == NULL) {
			XMLrd::LogEvent(logwarn, 2, "Glossdef not found for ", elem->idref);
			char *nref = NewName(ref, strlen(ref) + 4);
			if (ltyp == genabbr)
				strcat(nref, "_ab");
			if (ltyp == gentmk)
				strcat(nref, "_tm");
			ref = nref;
		}
		elem->href = MakeUMXLHref(fname, ref);
	}
	if (elem->href) {
		XMLrd::PendXref = true;
		XMLrd::XrefDepth = depth;
		XMLrd::XrefHref = elem->href;
		XMLrd::XrefForm = "TextXref";
		XMLrd::XrefExt = sclocal;
	}
	else {
		// point XMLrd::XrefTitleStr at ctrl for title-only span
		DCctl *cp = new DCctl(etext, 2, 85, 6, 0);
		XMLrd::XrefTitleStr = cp;
		XMLrd::TextGroup->add(*cp);
	}
}


void
XMLud::ListElemRefEnd(DCelem *elem, gentyp ltyp)
{
	char *txt = NULL;
	bool first = true;
	bool found = false;
	element *glosselem = NULL;
	listrefinfo *lrp = NULL;
	listdefinfo *ldp = NULL;
	char *tname = NULL;

	switch (ltyp) {
		case gengloss:
			lrp = &GlossRefs;
			ldp = &GlossDefs;
			tname = "glossary";
			first = false;
			break;

		case genabbr:
			lrp = &AbbrRefs;
			ldp = &AbbrDefs;
			tname = "abbreviation";
			break;

		case gentmk:
			lrp = &TmarkRefs;
			ldp = &TmarkDefs;
			tname = "trademark";
			break;

		default:
			XMLrd::LogEvent(logwarn, 1, "Undefined dictionary list type");
			return;
	}

	if (elem->idref) {
		// if idref and making abbreviations list, mark to include def ****
		if ((glosselem = GetListDefElem(elem->idref, &GlossRefs)) != NULL)
			SetListDefUsed(elem->idref, &GlossRefs, &GlossDefs);
		if (!GetListDefElem(elem->idref, lrp)) {
			if (ltyp == genabbr)
				XMLrd::LogEvent(logwarn, 1, "Abbreviation not found for ", elem->idref);
			else if (ltyp == gentmk)
				XMLrd::LogEvent(logwarn, 1, "Trademark not found for ", elem->idref);
			if (glosselem) {
				ltyp = gengloss;
				lrp = &GlossRefs;
				ldp = &GlossDefs;
			}
		}
		else
			SetListDefUsed(elem->idref, lrp, ldp);
		if (ltyp != gengloss) {
			if (XMLrd::InTitleElem
			 && XMLrd::AcronymInTitle)
				first = false;
			else 
				first = ListDefFirstUse(elem->idref, ldp);
		}
	}
	if (elem->isEmpty) {
		// first instance of abbr, get gloss title; later, get own title
		if (ltyp == genabbr) {
 			element *aelem =	NULL;
			found = GetListDefTitle(elem->idref, first ? &GlossRefs : lrp);
			if (!found
			 && first
			 && ((aelem = GetListDefElem(elem->idref, lrp)) != NULL)
			 && ((aelem = GetElemChild(aelem, "p")) != NULL))
				found = PutElemContent(aelem);
		}

		// for tm, just the opposite
		else if (ltyp == gentmk)
			found = GetListDefTitle(elem->idref, first ? lrp : &GlossRefs);
		else if (ltyp == gengloss)
			found = GetListDefTitle(elem->idref, &GlossRefs);
		if (found)
			XMLrd::EmptyXref = elem->isEmpty = false;
		else if ((!first
		  && (ltyp == genabbr))
		 || (first
		  && (ltyp == gentmk)))
			XMLrd::LogEvent(logwarn, 1, "Cannot find ", tname, " definition for: ",
					                  elem->idref);
	}
	XMLrd::FlushText();
	if (XMLrd::XrefTitleStr) {
		txt = GetListDefDesc(elem->idref, &GlossRefs);
		if (txt)
			XMLrd::XrefTitleStr->ex(txt);
	}
	if (elem->href)
		XMLrd::EndXref();
	else {
		// else end special title group ****
		DCctl *cp = new DCctl(etext, 2, 85, 6, 0);
		XMLrd::TextGroup->add(*cp);
	}
	XMLrd::InGlossTerm = false;
}


element *
XMLud::GetListDefElem(char *ref, listrefinfo *lrp)
{
	// search listref models for id ref
	// return element if found
	long i = 0;
	DCnlist *ids = NULL;
	element *elem = NULL;

	for (i = 1; i <= lrp->count; i++) {
		if ((ids = (DCnlist *) lrp->modelIDs.find(i)) != NULL) {
			if ((elem = (element *) ids->getitem(ref)) != NULL)
				return elem;
		}
	}
	return NULL;
}


bool
XMLud::GetListDefTitle(char *ref, listrefinfo *lrp)
{
	// search listref models for id ref
	// return true if title found

	element *elem = GetListDefElem(ref, lrp);;

	if (elem != NULL)
		elem = GetElemChild(elem, "title");

	if (elem != NULL)
		return PutElemContent(elem);

	return false;
}


char *
XMLud::GetListDefTitleText(char *ref, listrefinfo *lrp)
{
	// search listref models for id ref
	// return text if title found

	element *elem = GetListDefElem(ref, lrp);;
	char *txt = NULL;

	if (elem != NULL)
		elem = GetElemChild(elem, "title");

	if ((elem != NULL)
	 && ((txt = GetElemText(elem)) != NULL))
		return txt;

	return NULL;
}


char *
XMLud::GetListDefDesc(char *ref, listrefinfo *lrp)
{
	// search listref models for id ref
	// return text if desc found

	char *txt = NULL;
	element *elem = GetListDefElem(ref, lrp);;
	element *nel = NULL;

	if (elem != NULL) {
		if (((nel = GetElemChild(elem, "desc")) != NULL)
		 && ((txt = GetElemText(nel)) != NULL))
			return txt;
		if (((nel = GetElemChild(elem, "p")) != NULL)
		 && ((txt = GetElemText(nel)) != NULL))
			return txt;  // fallback, use first para for mouseover
	}

	return NULL;
}


char *
XMLud::GetListDefText(char *ref, listrefinfo *lrp)
{
	// search listref models for id ref
	// return true if p found

	char *txt = NULL;
	element *elem = GetListDefElem(ref, lrp);;

	if (elem != NULL)
		elem = GetElemChild(elem, "p");

	if (elem != NULL) {
		if ((txt = GetElemText(elem)) != NULL)
			return txt;
	}

	return NULL;
}


bool 
XMLud::ListDefFirstUse(const char *ref, listdefinfo *ldp)
{
	long val = 0;

	// if not referenced in  def, must be first use
	if ((val = ldp->IDs.getitem(ref)) == 0)
		return true;

	if (ldp->used.getitem(ref) != 0)
		return false;

;	ldp->used.add(ref, val);
	return true;
}


void 
XMLud::ListDefResetUse(listdefinfo *ldp)
{
	ldp->used.empty();
}


bool 
XMLud::SetListDefUsed(const char *ref, listrefinfo *lrp, listdefinfo *ldp)
{

	if (ldp->IDs.getitem(ref))
		return true;

	long i = 0;
	DCnlist *ids = NULL;
	element *elem = NULL;
	char *sort = NULL;
	char *xrefs = NULL;
	DCnlist *xrf = NULL;
	DCnlist *xrfp = NULL;
	bool ret = true;

	for (i = 1; i <= lrp->count; i++) {
		if ((ids = (DCnlist *) lrp->modelIDs.find(i)) != NULL) {
			if ((elem = (element *) ids->getitem(ref)) != NULL) {
				// sdd referenced elem to listDefElems
				ldp->elems.add(elem, ++ldp->count);
				// add @id to listDefIDs
				ldp->IDs.add(ref, ldp->count);

				// addsorted @sort or title to listDefSorts
				if ((sort = GetElemAttr(elem, "sort")) == NULL)
					sort = GetElemText(GetElemChild(elem, "title"));
				if (sort) {
					if (AbbrsInGloss // for abbr, add to gloss list too
					 && (ldp == &AbbrDefs))
						GlossDefs.sorts.addsorted(sort, -(ldp->count));
					ldp->sorts.addsorted(sort,  ldp->count);
				}
				else
					ret = false;

				if (((xrefs = GetElemAttr(elem, "idrefs")) != NULL)
				 || ((xrefs = GetElemAttr(elem, "idref")) != NULL)) {
					xrfp = xrf = new DCnlist;
					xrf->addlist(xrefs);
					while ((xrfp = xrfp->next) != NULL)
						SetListDefUsed(xrfp->name, lrp, ldp);
					delete xrf;
				}
				return ret;
			}
		}
	}
	return false;
}


void 
XMLud::PutAllListDefs(listdefinfo *ldp, gentyp ltyp)
{
	// write list of defs used
	long i = 0;
	long pos = 0;
	DCnlist *dcn = &(ldp->sorts);
	element *el = NULL;
	char *id = NULL;
	char *nid = NULL;
	char *txt = NULL;

	while ((dcn = dcn->next) != NULL) {  // get in sorted order
		pos = dcn->id;
		if (AbbrsInGloss
		 && (pos < 0)
		 && (ltyp == gengloss)
		 && ((el = (element *) AbbrDefs.elems.find(-pos)) != NULL)) {
			XMLrd::CurrDepth--;
			nid = NULL;
			if ((id = AbbrDefs.IDs.find(-pos)) != NULL) {
				nid = NewName(id, strlen(id) + 4);
				strcat(nid, "_ab");
				DCctl *cp = new DCctl(etext, 2, 112, 2);
				cp->ex(nid, strlen(nid));
				XMLrd::TextGroup->add(*cp);
			}
			PutElemContent(el);
			txt = GetListDefTitleText(id, &GlossRefs);
			if (txt) {
				char **attrs = new char *[4];
				attrs[0] = "idref";
				attrs[1] = nid;
				attrs[2] = NULL;
				attrs[3] = NULL;
				XMLrd::StartElement("p", NULL, XMLrd::CurrDepth + 1);
				XMLrd::FlushText();
				XMLrd::WriteIndexSeeItem(XMLrd::TextGroup, XMLrd::IndexSeeStart);
				//XMLrd::ProcChar(XMLrd::IndexSeeStart, strlen(XMLrd::IndexSeeStart), XMLrd::CurrDepth + 1);
				//XMLrd::ProcChar(": ", 2, XMLrd::CurrDepth + 1);
				XMLrd::StartElement("term", (const char **) attrs, XMLrd::CurrDepth + 1);
				XMLrd::ProcChar(txt, strlen(txt), XMLrd::CurrDepth + 1);
				XMLrd::EndElement("term", XMLrd::CurrDepth);
				XMLrd::EndElement("p", XMLrd::CurrDepth);
				delete [] attrs;
			}
			if (nid)
				DeleteName(nid);
			XMLrd::CurrDepth++;
		}
		else if ((el = (element *) ldp->elems.find(pos)) != NULL) {
			XMLrd::CurrDepth--;
			nid = NULL;
			if ((id = ldp->IDs.find(pos)) != NULL) {
				nid = NewName(id, strlen(id) + 4);
				if (ltyp == genabbr)
					strcat(nid, "_ab");
				else if (ltyp == gentmk)
					strcat(nid, "_tm");
				DCctl *cp = new DCctl(etext, 2, 112, 2);
				cp->ex(nid, strlen(nid));
				XMLrd::TextGroup->add(*cp);
			}
			PutElemContent(el);
			XMLrd::CurrDepth++;
			if (nid)
				DeleteName(nid);
		}
	}
}


void 
XMLud::SetListRefFile(char *srcfile, char *srcid, listrefinfo *lrp)
{
	if (lrp->files.getitem(srcfile))
		return;

	// add href filename to listRefFiles
	lrp->files.add(srcfile, ++lrp->count);

	// add idref if any to listRefFileIDs
	if (srcid)
		lrp->fileIDs.add(srcid, lrp->count);

	// add file model to GlossRefModels
	element *model = NULL;
	MxlParser *parser = new MxlParser();
	parser->SetOptions(false, true, false);  // model, not SAX
	parser->SetExpatCallbacks(NULL, XMLrfile::procError);
	if ((model = parser->ParseFile(srcfile, NULL)) == NULL) {
		XMLrd::LogEvent(logerr, 1, "Cannot read file: ", srcfile);
		delete parser;
		return;
	}
	lrp->models.add(model, lrp->count);
	DCnlist *elemIDs = new DCnlist();
	DCvlist *elemParents = new DCvlist();
	GetCurrElemIDs(model, elemIDs, NULL, elemParents);
	lrp->modelIDs.add(elemIDs, lrp->count);
	lrp->modelParents.add(elemParents, lrp->count);
	delete parser;
}



// index lists

bool XMLud::InMXMLIndexEntry = false;
bool XMLud::InIndexSort = false;
DCvlist XMLud::IndexItems;
DCnlist XMLud::IndexItemIDs;
DCclist XMLud::IndexItemGroups;
long XMLud::IndexItemCount = 0;
DCvlist XMLud::IndexSeeItems;
DClist XMLud::IndexSeeTypes;
DCnlist XMLud::IndexSeeIDrefs;
DCclist XMLud::IndexSeeGroups;
long XMLud::IndexSeeCount = 0;
char *XMLud::IXSee;
char *XMLud::IXSeeAlso;

void 
XMLud::StartUMXLIndexItem(void)
{
	if (XMLrd::IsPreform)
		return;
	XMLrd::InIndex = true;
	InMXMLIndexEntry = true;
	XMLrd::IndexLevel = 1;
	if (XMLrd::CurrElemInfo->id) {
		IndexItems.add(XMLrd::CurrElemInfo, ++IndexItemCount);
		IndexItemIDs.add(XMLrd::CurrElemInfo->id, IndexItemCount);
	}
	// CurrElemInfo has outclass, need to determine if this elem
	// should make an index entry, or just be used to gen an index
	XMLrd::CheckIndexListItem();

	XMLrd::PreIndexGroup = XMLrd::TextGroup;
	XMLrd::CurrIndexGroup = new DCgroup(DCctl(group, 2, 110, 1));
	if (XMLrd::CurrElemInfo->id)
		IndexItemGroups.add(XMLrd::CurrIndexGroup, IndexItemCount);
	if (XMLrd::UseEntry) {
		if (XMLrd::ParaStarted)
			XMLrd::TextGroup->add(*XMLrd::CurrIndexGroup);
		else {
			if (!XMLrd::PendingIndexItems)
				XMLrd::PendingIndexItems = new DCgroup();
			XMLrd::PendingIndexItems->add(*XMLrd::CurrIndexGroup);
		}
	}
	XMLrd::TextGroup = XMLrd::CurrIndexGroup;

	if (XMLrd::UseList || XMLrd::UseDefList)
		XMLrd::StartIndexListItem();  // **** will need corrections for UMXL
	else
		DCixref::CurrIXRef = NULL;

	char *start = XMLrd::CurrElemInfo->getAttr("start");
	char *end = XMLrd::CurrElemInfo->getAttr("end");
	ProcUMXLIndexRange(start, end);

	if (XMLrd::UseList || XMLrd::UseDefList) {
		XMLrd::IndexLevels.empty();
		XMLrd::IndexSortLevels.empty();
		InIndexSort = false;
		XMLrd::IndexLevelGroup = new DCgroup;
		XMLrd::IndexLevels.add(XMLrd::IndexLevelGroup, XMLrd::IndexLevel);
		XMLrd::CurrIndexGroup->add(*XMLrd::IndexLevelGroup);
		XMLrd::TextGroup = XMLrd::IndexLevelGroup;
		XMLrd::IndexTopLevel = XMLrd::IndexLevel;
		//XMLrd::CurrListIndexGroup = XMLrd::IndexLevelGroup;
}

	XMLrd::NopageEntry = false;
	IXSee = XMLrd::CurrElemInfo->getAttr("see");
	IXSeeAlso = XMLrd::CurrElemInfo->getAttr("seealso");
	if (IXSee || IXSeeAlso)
		ProcUMXLSee();
}


void 
XMLud::StartNewUMXLIndexLevel(char *pto)
{
	if (InIndexSort)
		XMLrd::EndIndexListSortItem();
	else
		XMLrd::AddIndexListItemLevel();

	if (XMLrd::UseEntry
	 && pto
	 && *pto) {
		DCctl *cp = new DCctl(imnem, 2, 10, 1, *pto);
		XMLrd::CurrIndexGroup->add(*cp);
	}

	XMLrd::IndexLevel++;
	if (XMLrd::IndexTopLevel < XMLrd::IndexLevel)
		XMLrd::IndexTopLevel = XMLrd::IndexLevel;

	XMLrd::IndexLevelGroup = new DCgroup;
	if (InIndexSort)
		XMLrd::IndexSortLevels.add(XMLrd::IndexLevelGroup, XMLrd::IndexLevel);
	else
		XMLrd::IndexLevels.add(XMLrd::IndexLevelGroup, XMLrd::IndexLevel);
	XMLrd::CurrIndexGroup->add(*XMLrd::IndexLevelGroup);
	XMLrd::TextGroup = XMLrd::IndexLevelGroup;
	XMLrd::CurrListIndexGroup = XMLrd::IndexLevelGroup;
}


void 
XMLud::EndUMXLIndexItem(void)
{
	XMLrd::IndexLevel = 0;

	if (XMLrd::NopageEntry) {
		DCctl *cp = new DCctl(imnem, 2, 97, 40, 1);
		XMLrd::CurrIndexGroup->insert(*cp);
		// add see/also in Frame syntax here ****
		// may be forward ref to ID, so add empty group for later
		XMLrd::CurrIndexGroup->add(*XMLrd::IndexLevelSeeGroup);
	}

	if (XMLrd::UseList || XMLrd::UseDefList) {
		if (XMLrd::NopageEntry) {
			DCixref::CurrIXRef->settitle(XMLrd::CurrListIndexGroup, false);
			XMLrd::AddIndexListSeeItemLevel();
			DCixref::CurrIXRef->itype = (XMLrd::InSee ? ixsee : ixalso);
			DCixref::CurrIXRef->seeref = (XMLrd::InSee ? IXSee : IXSeeAlso);
		}
		else // add all groups in IndexLevels and IndexSortLevels
			XMLrd::EndIndexListItem();  // **** will need corrections for UMXL
	}

	if (XMLrd::EndingForCurrIndexGroup) {
		XMLrd::CopyIndexEntry(XMLrd::CurrIndexGroup, XMLrd::EndingForCurrIndexGroup);
		XMLrd::EndingForCurrIndexGroup = NULL;
	}
	XMLrd::SetUniqueID();
	XMLrd::TextGroup = XMLrd::PreIndexGroup;
	XMLrd::InIndex = false;
	XMLrd::InSee = false;
	XMLrd::InSeeAlso = false;
	InMXMLIndexEntry = false;
}


void 
XMLud::ProcUMXLIndexRange(char *start, char *end)
{
	// start and end are range IDs; for end, elem is empty
	XMLrd::ProcIndexRange(start, end);
}


void 
XMLud::ProcUMXLSee(void)
{
	// see and seealso are IDs of other idx elements
	// see is only ref, except maybe for other sees
	// seealso appears at start or end of list
	// both are ordered by title of referenced idx item
	XMLrd::NopageEntry = true;
	if (IXSee)
		XMLrd::InSee = true;
	if (IXSeeAlso)
		XMLrd::InSeeAlso = true;
	XMLrd::IndexLevelSeeGroup = new DCgroup;
	IndexSeeItems.add(XMLrd::CurrElemInfo, ++IndexSeeCount);
	IndexSeeIDrefs.add(IXSee ? IXSee : IXSeeAlso, IndexSeeCount);
	IndexSeeGroups.add(XMLrd::IndexLevelSeeGroup, IndexSeeCount);
	if (IXSee)
		IndexSeeTypes.add(IndexSeeCount);
}


void
XMLud::ProcOneUMXLIndexSee(char *idref, long pos)
{
	// called from IndexSeeIDrefs.all() before writing DCL out
	DCgroup *gp = (DCgroup *) IndexSeeGroups.find(pos);
	DCelem *elem = (DCelem *) IndexSeeItems.find(pos);
	bool see = IndexSeeTypes.find(pos);
	if (!gp || !elem)
		return;

	// find entry with idref as id, copy its IndexGroup to gp
	long src = IndexItemIDs.getitem(idref);
	if (!src)
		return;
	DCgroup *sgp = (DCgroup *) IndexItemGroups.find(src);
	DCelem *selem = (DCelem *) IndexItems.find(src);
	if (!sgp || !selem)
		return;

	DCixref *ref = NULL;

  if (elem->idxref) {
		ref = (DCixref *) elem->idxref;
		while (ref->subcnt)
			ref = (DCixref *) ref->subs.find(1);
		see = (ref->itype == ixsee);
	}

	// add see/also start text ****
	DCctl *cp = new DCctl(imnem, 2, 10, 1, ':');
	gp->add(*cp);

	XMLrd::WriteIndexSeeItem(gp, see ? XMLrd::IndexSeeStart : XMLrd::IndexSeeAlsoStart);

	// add group itself
	gp->merge(*sgp, false);

	// add see/also end text ****
	XMLrd::WriteIndexSeeItem(gp, see ? XMLrd::IndexSeeEnd : XMLrd::IndexSeeAlsoEnd);

	if (ref) {
		DCixitem *item = NULL;
		long i = 0;
		ref->setstitle(sgp);
		ref->settitle(gp, false);
		for (i = 1; i <= ref->itemcnt; i++) {
			item = (DCixitem *) ref->items.find(i);
			if (item) {
				item->itype = see ? ixsee : ixalso;
				item->id = idref;
			}
		}
	}
}


void
XMLud::ProcUMXLIndexText(char *txt, long len)
{
	// may be typographics in the index entry, so process in FlushText

	if (!XMLrd::UseList
	 && !XMLrd::UseDefList) {    // no need to group by level, just add
		XMLrd::PutTextCtrls(txt, len, true, false);  // for our own entry
		return;
	}

	// otherwise group by level, determine level break and sorts

	char *pto = txt;
	DCctl *cp = NULL;

	while (*pto) {
		switch (*pto) {
			case '\\':
				pto++;  // skip next char
				break;

			case ':':
				if (pto > txt)
					XMLrd::PutTextCtrls(txt, (pto - txt), true, false);
				StartNewUMXLIndexLevel(pto);
				txt = pto + 1;
				break;

			case '[':
				XMLrd::PutTextCtrls(txt, (pto - txt), true, false);
				InIndexSort = true;
				XMLrd::IndexLevel = 0;
				StartNewUMXLIndexLevel(pto);
				txt = pto + 1;
				break;

			case ']':
				XMLrd::PutTextCtrls(txt, (pto - txt), true, false);
				XMLrd::EndIndexListSortItem();
				if (XMLrd::UseEntry) {
					cp = new DCctl(imnem, 2, 10, 1, *pto);
					XMLrd::CurrIndexGroup->add(*cp);
				}
				InIndexSort = false;
				txt = pto + 1;
				if (*txt)
					XMLrd::LogEvent(logwarn, 1, "Index entry has text after sort: ", txt);
				return;;

			default:
				break;
		}
		pto++;
	}
	XMLrd::PutTextCtrls(txt, (pto - txt), true, false);
}



// write file referenced from map as plain text
// with the filename minus path as title

DCnlist XMLud::CodedocrefIDs;    // id referenced to DCctl * of ID in TOC
DCnlist XMLud::CodedocrefFiles;  // id referenced to DCctl * of fileref in TOC
DCnlist XMLud::CodedocrefPrefixes;  // id to DCgroup * before title in TOC
DCnlist XMLud::CodedocrefTitles;  // id referenced to title elem in doc


void 
XMLud::WriteCodedocrefTOC(DCelem *elem)
{
	char *fpath = elem->href;
	char *fname = elem->href;
	char *slash = strrchr(fpath, '/');
	if (!slash)
		slash = strrchr(fpath, '\\');
	if (slash)
		fname = slash + 1;

	char *per = strrchr(fname, '.');
	char *fbase = NewName(fname, per ? (per - fname) : 0);

	DCctl *cp = new DCctl(etext, 2, 112, 3);
	CodedocrefFiles.add(fbase, (long) cp);
	//cp->ex(XMLrd::MainDocName, strlen(XMLrd::MainDocName));
	//cp->ex(elem->elfile, strlen(elem->elfile));

	if (!XMLrd::IsRtf)
		XMLrd::TextGroup->add(*cp);

	cp = new DCctl(etext, 2, 112, 4);
	CodedocrefIDs.add(fbase, (long) cp);
	//cp->ex(fbase, strlen(fbase));
	//cp->ex(elem->elemuid, strlen(elem->elemuid));
	XMLrd::TextGroup->add(*cp);
	DCgroup *gp = new DCgroup();
	XMLrd::TextGroup->add(*((DCctl *) gp));
	CodedocrefPrefixes.add(fbase, (long) gp);
	XMLrd::PutTextStrCtl(fname);
}


void 
XMLud::WriteCodedocrefFile(DCmapelem *ref)
{
	char *fpath = ref->href;
	char *fname = ref->href;
	char *slash = strrchr(fpath, '/');
	if (!slash)
		slash = strrchr(fpath, '\\');
	if (slash)
		fname = slash + 1;

	char *per = strrchr(fname, '.');
	char *fbase = NewName(fname, per ? (per - fname) : 0);

	char *attrs[4];
	attrs[0] = "id";
	attrs[1] = fbase;
	attrs[2] = NULL;
	attrs[3] = NULL;

	XMLrd::StartElement("doc", (const char **) attrs, 1);
	DCelem *elem = XMLrd::CurrElemInfo;

	XMLrd::StartElement("title", NULL, 2);

	// map to new title in [CodeDocTitles]  ****
	if (XMLrd::IniFile
	 && XMLrd::IniFile->Section("CodeDocTitles")
	 && XMLrd::IniFile->Find(fname))
		fname = XMLrd::IniFile->StrRight();

	XMLrd::ProcChar(fname, strlen(fname), 3);

	// elemuid and elfile are set after title text is processed
	DCctl *cp = (DCctl *) CodedocrefIDs.getitem(fbase);
	if (cp
	 && elem->elemuid)
		cp->ex(elem->elemuid, strlen(elem->elemuid));

	// the filename gets created based on the docref parent
	cp = (DCctl *) CodedocrefFiles.getitem(fbase);
	if (cp
	 && elem->elfile)
		cp->ex(elem->elfile, strlen(elem->elfile));

	DCgroup *gp = (DCgroup *) CodedocrefPrefixes.getitem(fbase);
	elem = XMLrd::CurrElemInfo;  // get title elem iyself
	CodedocrefTitles.add(fbase, (long) elem);

	XMLrd::EndElement("title", 2);

	char sstr[2] = { 0x0A, 0};
	// in addition to spacing, this prevents the coderef
	// from inheriting the title format
	XMLrd::StartElement("pre", NULL, 2);
	XMLrd::ProcChar(sstr, 1, 3);
	XMLrd::EndElement("pre", 2);

	attrs[0] = "src";
	attrs[1] = ref->href;

	XMLrd::StartElement("coderef", (const char **) attrs, 2);
	XMLrd::EndElement("coderef", 2);

	XMLrd::EndElement("doc", 1);
}


void 
XMLud::WriteCodedocrefAnums(void)
{
	DCnlist *elt = &CodedocrefTitles;
	DCnlist *elp = &CodedocrefPrefixes;
	DCelem *elem = NULL;
	DCgroup *pxgp = NULL;

	while ((elt = elt->next) != NULL) {
		elp = elp->next;
		elem = (DCelem *) elt->id;
		pxgp = (DCgroup *) elp->id;
		if (elem->elemaref)
			pxgp->add(*((DCctl *) elem->elemaref));
	}
}




// end of drxmlud.cpp

