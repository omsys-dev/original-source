// drxmlfl.cc is the code for dcl XML file reading, JHG
// Copyright (c) 2008 by Omni Systems, Inc.  All rights reserved.

#include "dcappl.h"
#include "dcctlg.h"
#include "dcfili.h"
#include "dcini.h"
#include "dcelem.h"
#ifdef DITA
#include "expat.h"
#endif
#ifdef UDOC
#include "mxlparser.h"
#include "..\drxml\drxmlud.h"
#endif
#include "drxmlfl.h"
#include <windows.h>
#include <iostream.h>
#include "Shlwapi.h"


char *XMLrfile::ErrMsg[] = {
  "opening", "not XML format", "reading"
};


// public statics

int XMLrfile::CurrDepth = 0;
unc XMLrfile::XMLver = 0;
unc XMLrfile::XMLminver = 0;
unc XMLrfile::DITAver = 0;
char *XMLrfile::CurrRoot = NULL;
logfunc XMLrfile::LogEventFunc = NULL;

// callback pointers

elemstfunc XMLrfile::elemStart = NULL;
elemfunc XMLrfile::elemEnd = NULL;
charfunc XMLrfile::charProc = NULL;
pifunc XMLrfile::piProc = NULL;


// file opening


XMLrfile::XMLrfile(char *fname) : DCrfile(fname)
{
  Err();
  if (Ferr == fok)
		LogEvent(loginfo, 1, "Opened XML file ", fname);
}


void
XMLrfile::SetHandlers(elemstfunc es, elemfunc ee, charfunc ch, pifunc pi,
		                  bool usemxl)
{
	// call this before calling ParseDoc()
	elemStart = es;
	elemEnd = ee;
	charProc = ch;
	piProc = pi;

#ifdef UDOC
	UseMxlParser = usemxl;
#endif
}


void
XMLrfile::Err(void)
{
  if (Ferr == fok)
    return;
	LogEvent(logwarn, 1, "Error reading file ", fname, ",",
	         ErrMsg[((int) Ferr) - 1]);
  fprintf(stderr, "Readfile %s: error %s.\n",
    fname ? fname : "stdin", ErrMsg[((int) Ferr) - 1]);

	CloseFile();
}


void
XMLrfile::LogEvent(logtype lt, unc sev, const char *desc,
	const char *d2, const char *d3, const char *d4, const char *d5)
{
	if (!LogEventFunc)
		return;

	LogEventFunc(lt, sev, desc, d2, d3, d4, d5);
}


XMLrfile::~XMLrfile(void)
{
}


void
XMLrfile::ReopenFile(const char *fn, const char *dn, bool close)
{
	if (close)
		CloseFile();

	OpenFile(fn);
	LogEvent(loginfo, 1, "Opened XML file ", dn);
}


#ifdef DITA
XML_Parser XMLrfile::CurrDocParser = NULL;
bool XMLrfile::AbortingParse = false;

void
XMLrfile::AbortParse(void)
{
	AbortingParse = true;
	XML_StopParser(CurrDocParser, XML_FALSE);
}
#endif

#ifdef UDOC
// MicroXML parsing

bool XMLrfile::UseMxlParser = false;
bool XMLrfile::UseMxlModel = true;

void
XMLrfile::ParseMxlDoc(void)
{
	CurrDepth = 0;
	CurrRoot = NULL;

	if (UseMxlModel)
		XMLud::GetUMXLElement(fname, NULL, true);
	else {
		MxlParser *Parser = new MxlParser();
		Parser->SetOptions(true, false, false);
		Parser->SetExpatCallbacks(NULL, procError,
			startElement, endElement, charData,
			startCData, endCData,
			procInstr, xmlDecl,
			startDT, endDT, procComment, &CurrDepth);
		Parser->ParseFile(fname, NULL);
	}
}
#endif


void
XMLrfile::procError(long line, char *warning, char *cpt, bool fatal)
{
	char buf[10];

	LogEvent(fatal ? logerr : logwarn, 1,
		       "On line ", ultoa(line, buf, 10), ": ", warning, cpt);
}


// parsing loop

char *XMLrfile::LastStart = NULL;
char *XMLrfile::LastEnd = NULL;
void *XMLrfile::LastText = NULL;
long XMLrfile::LastTextCount = 0;
char *XMLrfile::CurrSysID = NULL;

#ifdef DITA

bool XMLrfile::ParserFail = false;

void
XMLrfile::ParseDoc(void)
{
#if 0
	if (UseMxlParser) {
		ParseMxlDoc();
		return;
	}
#endif
	XML_Parser parser = NULL;
	enum XML_Status status;
	void *buff = NULL;
	long btotal = getSize();
	long bsize = 0;
	bool done = false;

	CurrDepth = 0;
	CurrDocParser = parser = XML_ParserCreate(NULL);
  XML_SetUserData(parser, &CurrDepth);
  XML_SetElementHandler(parser, startElement, endElement);
	XML_SetCharacterDataHandler(parser, charData);
	XML_SetProcessingInstructionHandler(parser, procInstr);
	XML_SetXmlDeclHandler(parser, xmlDecl);
  XML_SetDoctypeDeclHandler(parser, startDT, endDT);
	XML_SetExternalEntityRefHandler(parser, extEntRef);
	XML_SetParamEntityParsing(parser, XML_PARAM_ENTITY_PARSING_ALWAYS);
	LocalPathUsed = false;
	LogEvent(loginfo, 3, "Parsing XML file ", fname);
	LastStart = LastEnd = NULL;

  while (!done) {
		if (btotal < TextBufMax) {
			bsize = btotal;
			done = true;
		}
		else {
			bsize = TextBufMax;
			btotal -= TextBufMax;
		}
    buff = XML_GetBuffer(parser, bsize);
    if (buff == NULL) {
      LogEvent(logwarn, 1, "Error with XML_GetBuffer call");
			XML_ParserFree(parser);
      return;
    }
		getBlock(buff, bsize);
		if (Ferr) {
      LogEvent(logwarn, 1, "Error with getting block from file");
			XML_ParserFree(parser);
      return;
    }

    status = XML_ParseBuffer(parser, bsize, done);
    switch (status) {
      case XML_STATUS_ERROR:
				if (!AbortingParse) {
					LogEvent(logwarn, 1, "Error from parser, XML_STATUS_ERROR");
					LogEvent(logcont, 1, "Last started: ", (LastStart ? LastStart : "///"),
					                     ", last ended: ", (LastEnd ? LastEnd : "???"));
				}
				XML_ParserFree(parser);
				AbortingParse = false;
        return;
      case XML_STATUS_SUSPENDED:
				XML_ParserFree(parser);
        return;
			case XML_STATUS_OK:
			default:
				break;
    }
  }
	XML_ParserFree(parser);
}
#endif

// expat callbacks


void XMLCALL
XMLrfile::startElement(void *userData, const char *name,
 const char **atts)
{
  int *dp = (int *)userData;
  *dp += 1;
	
	if (LastStart)
		DeleteName(LastStart);
	LastStart = NewName(name);

	if (elemStart)
		elemStart(name, atts, *dp);
}


void XMLCALL
XMLrfile::endElement(void *userData, const char *name)
{
  int *dp = (int *)userData;

	if (LastEnd)
		DeleteName(LastEnd);
	LastEnd = NewName(name);

	if (elemEnd)
		elemEnd(name, *dp);

  *dp -= 1;
}


void XMLCALL
XMLrfile::charData(void *userData, const XML_Char *s, int len)
{
  int *dp = (int *)userData;
	LastText = (void *) s;
	LastTextCount = len;

	if (charProc)
		charProc(s, len, *dp);
}


void XMLCALL 
XMLrfile::procInstr(void *userData, const XML_Char *target,
 const XML_Char *data)
{
	if (piProc)
		piProc(target, data);
}


void XMLCALL 
XMLrfile::xmlDecl(void *userData, const XML_Char *version,
	 const XML_Char *encoding, int standalone)
{
	if (version) {
		XMLver = *version - '0';
		if (version[1] == '.')
			XMLminver = version[2] - '0';
	}
}


void XMLCALL 
XMLrfile::startDT(void *userData, const XML_Char *doctypeName,
	 const XML_Char *sysid, const XML_Char *pubid, int int_subset)
{
	CurrRoot = NewName(doctypeName);
	CurrSysID = NewName(sysid);
	if (strstr(CurrSysID, "1.1"))
		DITAver = 1;
	else
		DITAver = 0;
}


void XMLCALL 
XMLrfile::endDT(void *userData)
{
	// dummy
	void *local = userData;
}


void XMLCALL 
XMLrfile::startCData(void *userData)
{
	// dummy
	void *local = userData;
}


void XMLCALL 
XMLrfile::endCData(void *userData)
{
	// dummy
	void *local = userData;
}


void
XMLrfile::procComment(void *userData, char *comment)
{
	// dummy
	void *local = userData;
	char *cmt = comment;
}

#ifdef DITA


DCnlist XMLrfile::FailingFiles;
long XMLrfile::Fails = 0;
char *XMLrfile::SubPath = NULL;
long XMLrfile::SubLevel = 0;
DCnlist XMLrfile::SubPaths;


int XMLCALL 
XMLrfile::extEntRef(XML_Parser p,
	 const XML_Char *context, const XML_Char *base,
	 const XML_Char *systemId, const XML_Char *publicId)
{
	XML_Parser pp = NULL;
	enum XML_Status status = XML_STATUS_OK;
	char *fname = NULL;
	char *pathend = NULL;
	DCrfile *rf = NULL;
	void *buff = NULL;
	long btotal = 0;
	long bsize = 0;
	bool done = false;

	fname = ResolveEntity(systemId, publicId);

	if (FailingFiles(fname))
		return ParserFail ? XML_STATUS_ERROR : XML_STATUS_OK;

	// open DCrfile and get file length
	rf = new DCrfile(fname);
	if (rf->FErr() != fok) {
		char *nn = NULL;  // alternative attempt
		DCrfile *xf = NULL;
		if (systemId
		 && ((SubPath = SubPaths.find(SubLevel)) != NULL)
		 && strcmp((nn = AddRelPath(SubPath, NewName(systemId))), fname)
		 && ((xf = new DCrfile(nn))->FErr() == fok)) {
			delete rf;
			rf = xf;
			fname = nn;
			SystemValList.addnew(systemId, ++SysidVal);
			SystemIdList.addnew(fname, SysidVal);
		}
		else {
			FailingFiles.addnew(fname, ++Fails);
			LogEvent(logwarn, 1, "Error reading file ", rf->getFilePath(), ", ",
							 ErrMsg[((int) rf->FErr()) - 1]);
			delete rf;
			return ParserFail ? XML_STATUS_ERROR : XML_STATUS_OK;
		}
	}

	SubLevel++;
	if ((pathend = strrchr(fname, '/')) != NULL)
		SubPath = NewName(fname, (pathend - fname) + 1);  // include slash
	SubPaths.replace(SubPath, SubLevel);

	btotal = rf->getSize();
	LogEvent(loginfo, 3, "Parsing XML subfile ", rf->getFilePath());

	pp = XML_ExternalEntityParserCreate(p, context, NULL);
	XML_SetParamEntityParsing(pp, XML_PARAM_ENTITY_PARSING_ALWAYS);

	while (!done
	 && (status == XML_STATUS_OK)) {
		if (btotal < TextBufMax) {
			bsize = btotal;
			done = true;
		}
		else {
			bsize = TextBufMax;
			btotal -= TextBufMax;
		}
    buff = XML_GetBuffer(pp, bsize);
    if (buff == NULL) {
      LogEvent(logwarn, 1, "Error with subparser XML_GetBuffer call");
			XML_ParserFree(pp);
			delete rf;
			SubLevel--;
      return ParserFail ? XML_STATUS_ERROR : XML_STATUS_OK;
    }
		rf->getBlock(buff, bsize);
		if (rf->FErr() != fok) {
      LogEvent(logwarn, 1, "Error with getting block from subparse file");
			XML_ParserFree(pp);
			delete rf;
			SubLevel--;
      return ParserFail ? XML_STATUS_ERROR : XML_STATUS_OK;
    }

    status = XML_ParseBuffer(pp, bsize, done);
    switch (status) {
      case XML_STATUS_ERROR:
				FailingFiles.addnew(fname, ++Fails);
				LogEvent(logwarn, 1, "Error from subparser for ", fname);
				XML_ParserFree(pp);
				delete rf;
				SubLevel--;
        return ParserFail ? XML_STATUS_ERROR : XML_STATUS_OK;
      case XML_STATUS_SUSPENDED:
				XML_ParserFree(pp);
				delete rf;
				SubLevel--;
        return XML_STATUS_SUSPENDED;
			case XML_STATUS_OK:
			default:
				break;
    }

	}
	XML_ParserFree(pp);
	SubLevel--;
	delete rf;
	DeleteName(fname);
	return ParserFail ? status : XML_STATUS_OK;
}


char *XMLrfile::LocalDTDPath = NULL;
bool XMLrfile::LocalPathUsed = false;
bool XMLrfile::CatalogPathUsed = true;
DCnlist XMLrfile::SystemIdList;
DCnlist XMLrfile::PublicIdList;
DCnlist XMLrfile::SystemValList;
DCnlist XMLrfile::PublicValList;
long XMLrfile::SysidVal = 0;
long XMLrfile::PubidVal = 0;

DCnlist XMLrfile::Catalogs;
char *XMLrfile::CurrSysId;
char *XMLrfile::CurrPubId;
bool XMLrfile::CatalogItemFound;
char *XMLrfile::CurrCatItem;
char *XMLrfile::CurrCatPath;
DClist XMLrfile::CurrCatalogs;
DClist XMLrfile::NewCatalogs;


char *
XMLrfile::ResolveEntity(const XML_Char *systemId, const XML_Char *publicId)
{
	char *fname = NULL;
	char *fptr = NULL;
	char *nptr = NULL;
	char *pathend = NULL;
	bool found = false;
	long val = 0;
	long num = 0;

	// lookup systemId then publicId, first in cache
	if ((((val = SystemValList(systemId)) != 0)
	  && ((fname = SystemIdList.find(val)) != NULL))
	 || (((val = PublicValList(publicId)) != 0)
	  && ((fname = PublicIdList.find(val)) != NULL)))
		return NewName(fname);

	if (CatalogPathUsed) {		// not in cache, try to find it
		CurrCatalogs.empty();
		for (num = 1; num <= Catalogs.count(); num++)
			CurrCatalogs.add((long) Catalogs.find(num));
		CurrSysId = NewName(systemId);
		CurrPubId = NewName(publicId);
		CatalogItemFound = false;
		CurrCatItem = NULL;
		NextCatalog = NULL;
		for (;;) {
			if (NextCatalog) { // delegated
				fname = NextCatalog;
				NextCatalog = NULL;
				NewCatalogs.empty();
			}
			else
				fname = (char *) CurrCatalogs.dequeue();
			if (!fname)
				break;
			if ((pathend = strrchr(fname, '/')) != NULL)
				CurrCatPath = NewName(fname, (pathend - fname) + 1);  // include slash
			SearchCatalog(fname);
			if (AbortingCatParse) {
				// delegating, discard other nextCatalogs
				AbortingCatParse = false;
				continue;
			}
			if (CatalogItemFound
			 && CurrCatItem) {
				fname = CurrCatItem;
				CurrCatItem = NULL;
				found = true;
				break;
			}
			if (NewCatalogs.count()) {
				if (CurrCatPath) {  // put current path on all relative paths returned
					DClist *nlst = new DClist;;
					char *nn = NULL;
					while ((nn = (char *) NewCatalogs.dequeue()) != NULL) {;
						char *nx = AddRelPath(CurrCatPath, nn);
						nlst->append((long) nx);
					}
					CurrCatalogs.insert(nlst);
					NewCatalogs.empty();
					delete nlst;
				}
				else
					CurrCatalogs.insert(&NewCatalogs);
			}
		}

		if (CurrSysId) {
			DeleteName(CurrSysId);
			CurrSysId = NULL;
		}
		if (CurrPubId) {
			DeleteName(CurrPubId);
			CurrPubId = NULL;
		}
		CatalogItemFound = false;

		//CurrBase = NewName("base/dtd/");

		if (found && CurrBase) 	// put base on front of relative uri
			fname = AddRelPath(CurrBase, fname);

		if (found && CurrCatPath) 	// put path on too
			fname = AddRelPath(CurrCatPath, fname);
	}

	if (!found) {
		// if systemId is not local "file:", make path to local version 
		fptr = fname = NewName(systemId);
		if ((nptr = strstr(fname, "file:")) != NULL)
			fname = nptr + 5;  // trim off file prefix, leave UNC path
		else if ((SubPath = SubPaths.find(SubLevel)) != NULL)
			fname = AddRelPath(SubPath, NewName(systemId));
		else if (LocalPathUsed) {  // use same path for recursive calls
			fname = NewName(LocalDTDPath, strlen(LocalDTDPath) + strlen(fptr) + 1);
			strcat(fname, "/");
			strcat(fname, fptr);
		}
		else if (LocalDTDPath) {
			if ((nptr = strrchr(fname, '/')) != NULL) {
				fname = NewName(LocalDTDPath, strlen(LocalDTDPath) + strlen(nptr));
				strcat(fname, nptr);
			}
			else {
				fname = NewName(LocalDTDPath, strlen(LocalDTDPath) + strlen(fptr) + 1);
				strcat(fname, "/");
				strcat(fname, fptr);
			}
			LocalPathUsed = true;
		}
		// else no local path, so assume simple filename and use it
		if (fname != fptr)
			DeleteName(fptr);
	}

	// fname is best guess, cache it
	if (publicId && FoundPub) {
		PublicValList.addnew(publicId, ++PubidVal);
		PublicIdList.addnew(fname, PubidVal);
	}
	else if (systemId) {
		SystemValList.addnew(systemId, ++SysidVal);
		SystemIdList.addnew(fname, SysidVal);
	}

	return fname;
}


void 
XMLrfile::SearchCatalog(char *fname)
{
	if (!fname
	 || (CatalogItemFound && CurrCatItem))
		return;

	// call recursively to parse catalog at catpath
	// looking for CurrSysId or CurrPubId
	// if found, set CatalogItemFound and CurrCatItem
	// end at first instance found

	// set up new parser for catalog
	// catalogs have no element content, so processing
	// start elements is sufficient, plus end for group

	XML_Parser parser = NULL;
	enum XML_Status status;
	void *buff = NULL;
	long bsize = 0;
	bool done = false;

	if (FailingFiles(fname))
		return;

	DCrfile *cf = new DCrfile(fname);
	if (cf->FErr() != fok) {
		FailingFiles.addnew(fname, ++Fails);
		LogEvent(logwarn, 1, "Error reading catalog file ", cf->getFilePath(), ", ",
	           ErrMsg[((int) cf->FErr()) - 1]);
		return;
	}
	long btotal = cf->getSize();

	CurrCatParser = parser = XML_ParserCreate(NULL);
  XML_SetElementHandler(parser, startCatElement, endCatElement);
	XML_SetExternalEntityRefHandler(parser, extEntRef);
	XML_SetParamEntityParsing(parser, XML_PARAM_ENTITY_PARSING_ALWAYS);
	LogEvent(loginfo, 3, "Parsing catalog file ", fname);

	CurrBase = NULL;
	NextCatalog = NULL;
	PrefPub = PrefCat = true;
	FoundPub = false;
	if (CatElements.count() == 0)
		CatElements.init(CatElementsInit);

  while (!done) {
		if (btotal < TextBufMax) {
			bsize = btotal;
			done = true;
		}
		else {
			bsize = TextBufMax;
			btotal -= TextBufMax;
		}
    buff = XML_GetBuffer(parser, bsize);
    if (buff == NULL) {
      LogEvent(logwarn, 1, "Error with XML_GetBuffer call for catalog");
			XML_ParserFree(parser);
			delete cf;
      return;
    }
		cf->getBlock(buff, bsize);
		if (cf->Ferr) {
      LogEvent(logwarn, 1, "Error with getting block from catalog");
			XML_ParserFree(parser);
			delete cf;
      return;
    }

    status = XML_ParseBuffer(parser, bsize, done);
    switch (status) {
      case XML_STATUS_ERROR:
				if (!AbortingCatParse)
					LogEvent(logwarn, 1, "Error from catalog parser, XML_STATUS_ERROR");
				XML_ParserFree(parser);
				delete cf;
        return;
      case XML_STATUS_SUSPENDED:
				XML_ParserFree(parser);
				delete cf;
        return;
			case XML_STATUS_OK:
			default:
				break;
    }
  }
	XML_ParserFree(parser);
	delete cf;
}


DCnlist XMLrfile::CatElements;
npair XMLrfile::CatElementsInit[] = {
 { 1, "catalog"},
 { 2, "group"},
 { 3, "public"},
 { 4, "system"},
 { 5, "uri"},
 { 6, "delegatePublic"},
 { 7, "delegateSystem"},
 { 8, "delegateURI"},
 { 9, "rewriteSystem"},
 { 10, "rewriteURI"},
 { 11, "nextCatalog"},
 NULL
};

char *XMLrfile::NextCatalog = NULL;
char *XMLrfile::CurrBase = NULL;;
bool XMLrfile::PrefPub = true;
bool XMLrfile::PrefCat = true;
bool XMLrfile::FoundPub = false;


void XMLCALL
XMLrfile::startCatElement(void *userData, const char *name,
 const char **atts)
{
	if (CatalogItemFound
	 && CurrCatItem)
		return;

	// identify element type
	long etype = CatElements(name);

	// attributes
	char *prefer = NULL;
	char *base = NULL;
	char *publicId = NULL;
	char *systemId = NULL;
	char *uname = NULL;
	char *uri = NULL;
	char *publicIdSt = NULL;
	char *systemIdSt = NULL;
	char *uriSt = NULL;
	char *rewrite = NULL;
	char *catalog = NULL;

	// get appropriate attributes
	switch (etype) {
		case 1: // catalog: prefer, xml:base
			prefer = GetAttr(atts, "prefer");
			if (prefer) {
				if (!strcmp(prefer, "public"))
					PrefPub = true;
				else if (!strcmp(prefer, "system"))
					PrefPub = false;
				PrefCat = PrefPub;
			}
			else
				PrefCat = PrefPub = true;

			base = GetAttr(atts, "base");
			if (!base)
				base = GetAttr(atts, "xml:base");
			if (base)
				CurrBase = NewName(base);
			else
				CurrBase = NULL;
			break;

		case 2: // group: prefer, xml:base
			prefer = GetAttr(atts, "prefer");
			if (prefer) {
				if (!strcmp(prefer, "public"))
					PrefPub = true;
				else if (!strcmp(prefer, "system"))
					PrefPub = false;
			}

			base = GetAttr(atts, "base");
			if (!base)
				base = GetAttr(atts, "xml:base");
			if (base)
				CurrBase = NewName(base);
			break;

		case 3: // public: publicId, uri
			publicId = GetAttr(atts, "publicId");
			uri = GetAttr(atts, "uri");
			if (CurrPubId
			 && (!CurrSysId || PrefPub)
			 && !strcmp(CurrPubId, publicId)) {
				CatalogItemFound = FoundPub = true;
				CurrCatItem = NewName(uri);
				return;
			}
			break;

		case 4: // system: systemId, uri
			systemId = GetAttr(atts, "systemId");
			uri = GetAttr(atts, "uri");
			if (CurrSysId
			 && !strcmp(CurrSysId, systemId)) {
				CatalogItemFound = true;
				CurrCatItem = NewName(uri);
				return;
			}
			break;

		case 5: // uri: name, uri
			uname = GetAttr(atts, "name");
			uri = GetAttr(atts, "uri");
			// what do we do here?  ****
			break;

		case 6: // delegatePublic: publicIdStartString, catalog
			publicIdSt = GetAttr(atts, "publicIdStartString");
			catalog = GetAttr(atts, "catalog");
			if (CurrPubId && publicIdSt && catalog
			 && (strstr(CurrPubId, publicIdSt) == CurrPubId)) {
				NextCatalog = NewName(catalog);
				AbortCatParse();
				return;
			}
			break;

		case 7: // delegateSystem: systemIdStartString, catalog
			systemIdSt = GetAttr(atts, "systemIdStartString");
			catalog = GetAttr(atts, "catalog");
			if (CurrSysId && systemIdSt && catalog
			 && (strstr(CurrSysId, systemIdSt) == CurrSysId)) {
				NextCatalog = NewName(catalog);
				AbortCatParse();
				return;
			}
			break;

		case 8: // delegateURI: uriStartString, catalog
			uriSt = GetAttr(atts, "uriStartString");
			catalog = GetAttr(atts, "catalog");
			// what do we do here?  ****
			break;

		case 9: // rewriteSystem: systemIdStartString, rewritePrefix
			systemIdSt = GetAttr(atts, "systemIdStartString");
			rewrite = GetAttr(atts, "rewritePrefix");
			if (CurrSysId && systemIdSt && rewrite
			 && (strstr(CurrSysId, systemIdSt) == CurrSysId)) {
				char *newId = NewName(rewrite,
				            (strlen(CurrSysId) - strlen(systemIdSt)) + strlen(rewrite));
				strcat(newId, (CurrSysId + strlen(systemIdSt)));
				DeleteName(CurrSysId);
				CurrSysId = newId;
				return;
			}
			break;

		case 10: // rewriteURI: uriStartString, rewritePrefix
			uriSt = GetAttr(atts, "uriStartString");
			rewrite = GetAttr(atts, "rewritePrefix");
			// what do we do here?  ****
			break;

		case 11: // nextCatalog: catalog
			catalog = GetAttr(atts, "catalog");
			if (catalog) {
				NewCatalogs.append( (long) NewName(catalog));
				return;
			}
			break;

		default:
			break;
	}

}


void XMLCALL
XMLrfile::endCatElement(void *userData, const char *name)
{
	// if group, reset base URI
	long etype = CatElements(name);
	if (etype != 2)
		return;

	PrefPub = PrefCat;
}


char *
XMLrfile::GetAttr(const char **attp, const char *name)
{
	char *val = NULL;

	while (*attp) {
		if (!strcmp(*attp, name)) {
			val = NewName(*(attp + 1));
			break;
		}
		attp += 2;
	}

	return val;
}


XML_Parser XMLrfile::CurrCatParser = NULL;
bool XMLrfile::AbortingCatParse = false;

void
XMLrfile::AbortCatParse(void)
{
	AbortingCatParse = true;
	XML_StopParser(CurrCatParser, XML_FALSE);
}


char *
XMLrfile::AddRelPath(char *path, char *name)
{
	if (!name || !path)
		return name;

	if (!PathIsRelative(name)
	 || PathIsURL(name))
		return name;

	char *nx = NewName(path, strlen(path) + strlen(name) + 1);
	if (path[strlen(path) - 1] != '/')
		strcat(nx, "/");
	strcat(nx, name);
	DeleteName(name);
	return nx;
}

#endif


// end of drxmlfl.cpp


