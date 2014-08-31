// drxmlan.cpp is the code for dcl XML ref processing, JHG
// Copyright (c) 2008 by Omni Systems, Inc.  All rights reserved.

#include "dcappl.h"
#include "dcctlg.h"
#include "dcfili.h"
#include "dcfilp.h"
#include "dctxfx.h"
#include "dcfmtr.h"
#include "dcini.h"
#include "dcindx.h"
#include "dcelem.h"
#include "drxml.h"
#ifdef DITA
#include "expat.h"
#endif
#ifdef UDOC
#include "mxlparser.h"
#include "drxmlud.h"
#endif
#include "drxmlfl.h"
#include <windows.h>
#include <iostream.h>
#include "Shlwapi.h"




// process xrefs that need content, conrefs, and idrefs (1.2)


void
XMLrd::ProcIntRefs(void)
{
	// process refs in the current file set
	// that have sources in the current file

}

#if 0
void
XMLrd::ProcExtRefs(XMLrfile *rf)
{
	// process all refs (or maybe external to the current file set)

	// DocNamesProcessed has the names to check against
	// XrefRefs, LinkRefs, and ConrefRefs have the items to check

	// for both links and xrefs, adjust the file and anchor ctls

	while ((LinkCount > LinksProcessed)
	 || (XrefCount > XrefsProcessed)
	 || (ConrefCount > ConrefsProcessed)
	 || AnumElems.count()
	 || GenLists.count()) {

		if (ProcConrefs)
			ProcessConrefs(rf);

		ProcessAnums();

		ProcessGeneratedLists();

		ProcessLinks();
		ProcessXrefs();
	}
}
#endif


char *XMLrd::IDPathSep = "P";
char *XMLrd::IDUpDir = "U";
char *XMLrd::IDTopSep = "T";
char *XMLrd::IDElemSep = "E";
bool XMLrd::IDPath = true;
bool XMLrd::IDFile = true;
bool XMLrd::IDTopic = true;


char *
XMLrd::MakeAnchorName(char *href, DCelem *einfo)
{
	// Anchor names must be guaranteed unique in the output file,
	// therefore unique in the DCL file.  To do this, we make up
	// the name using the canonical relative path from the DCL
	// file including the DITA file name and the topic ID, with
	// nonalnum chars replaced: D for path / or \, U for .., nothing
	// for ./, and T for #, and E for the / before the element ID.
	// So ../concepts/method.dita#c_method/fig1 becomes the anchor:
	//     UDconceptsDmethodTc_methodEfig1   treated like a newlink.

	static char nref[RefBufLen];
	char *np = nref;
	char *path = NULL;
	char *topic = NULL;
	char *elem = NULL;
	char *cpt = NULL;
	char *slash = NULL;
	*np = '\0';

	if (!href)
		return NULL;

	ParseHref(href, &path, &topic, &elem);

	if (IDPath || IDFile) {
		if (IDPath && IDFile)
			cpt = path;
		else {  // one but not the other
			if ((slash = strrchr(path, '/')) != NULL) {
				*slash = '\0'; 
				cpt = IDPath ? path : (slash + 1);
			}
			else
				cpt = IDFile ? path : "";
		}

		while (*cpt) {
			if (!strncmp(cpt, "../", 3)) {
				cpt += 3;
				strcpy(np, IDUpDir);
				np += strlen(IDUpDir);
			}
			else if (!strncmp(cpt, "./", 2))
				cpt += 2;
			else if (!strncmp(cpt, ".dita", 5))
				cpt += 5;
			else if (!strncmp(cpt, ".xml", 4))
				cpt += 4;
			else if (*cpt == '/') {
				cpt++;
				strcpy(np, IDPathSep);
				np += strlen(IDPathSep);
			}
			else if (isalnum(*cpt))
				*np++ = *cpt++;
			else
				cpt++;
		}
		*np = '\0';
		char *nm = MakeNameUnique(nref, einfo, true);
		strcpy(nref, nm);
		np = nref + strlen(nref);
	}
	if (IDTopic && topic) {
		if (IDPath || IDFile) {
			strcpy(np, IDTopSep); 
			np += strlen(IDTopSep);
		}
		cpt = topic;
		while (*cpt) {
			if (isalnum(*cpt))
				*np++ = *cpt++;
			else
				cpt++;
		}
	}
	if (elem) {
		if (IDPath || IDFile || IDTopic) {
			strcpy(np, IDElemSep);
			np += strlen(IDElemSep);
		}
		cpt = elem;
		while (*cpt) {
			if (isalnum(*cpt))
				*np++ = *cpt++;
			else
				cpt++;
		}
	}

	*np = '\0';

	return NewName(nref);
}


void 
XMLrd::ProcessLinks(void)
{
	// for links, adjust anchors

	long i = 0;

	char *hfdoc = NULL;
	char *hftopic = NULL;
	char *hfelem = NULL;

	long docnum = 0;
	DCtopdoc *dinfo = NULL;
	long topnum = 0;
	DCtopic *tinfo = NULL;
	DCtopic *origtinfo = NULL;
	long elnum = 0;
	DCtopelem *einfo = NULL;

	char *branch = CurrBranch;
	conditions *conds = CurrCondSet;
	linkinfo *linfo = NULL;

	for (i = LinksProcessed + 1; i <= LinkCount; i++) {
		linfo = (linkinfo *) Links.find(i);
		if (!linfo) {
			LogEvent(logwarn, 1, "Missing info for internal link ",
			                     itoa(i, NewName(10), 10));
			continue;
		}
		if (linfo->url
		 || linfo->peer
		 || !linfo->href)
			continue;  // nothing to do here
		CurrBranch = linfo->branch;
		CurrCondSet = linfo->conds;
		hfdoc = hftopic = hfelem = NULL;
		ParseHref(linfo->href, &hfdoc, &hftopic, &hfelem);
		if ((docnum = GetProcessedDocInfo(hfdoc)) != 0) {
			// replace with anchor used for title or other item
			if (!UsePartFiles)
				linfo->fnctl->ex(MainDocName, strlen(MainDocName));
			if (IsRtf)
				linfo->fnctl->maj((uns) 0);  // remove it
			if ((dinfo = (DCtopdoc *) DocInfos.find(docnum)) != NULL) {
#if 0
(linfo->mref  // reference from map to own doc
		 && ((docnum = linfo->mref->docnum) != 0))
		 || ()
#endif
				topnum = hftopic ? dinfo->topicids->getitem(hftopic) : 1;
				origtinfo = (DCtopic *) dinfo->topics->find(topnum);
				if (GetFinalConrefTopic(&docnum, &topnum)) {
					dinfo = (DCtopdoc *) DocInfos.find(docnum);
					if (!dinfo)
						continue; // ****
				}
				if ((tinfo = (DCtopic *) dinfo->topics->find(topnum)) != NULL) {
					if (UsePartFiles) {
						char *dtname = DCLPartFileNames.find(tinfo->part);
						if (dtname)
							linfo->fnctl->ex(dtname, strlen(dtname));
						else
							linfo->fnctl->ex(MainDocName, strlen(MainDocName));
					}
					if (hfelem              // ref is to a specific element
					 && ((elnum = tinfo->elemids->getitem(hfelem)) != 0)) {
						if ((einfo = (DCtopelem *) tinfo->elems->find(elnum)) != NULL) {
							if (GenerateUIDs)
								hfelem = einfo->elemuid;
							linfo->factl->ex(hfelem, strlen(hfelem));     // use elemid
						}
						else
							LogEvent(logwarn, 1, "No id for link to element ", linfo->href, " in: ", linfo->xrname);
					}
					else if (hftopic
					 || ((hftopic = origtinfo->id) != NULL)) {
						if ((einfo = (DCtopelem *) origtinfo->elems->find(1)) != NULL) {
							if (GenerateUIDs)
								hftopic = einfo->elemuid;
							linfo->factl->ex(hftopic, strlen(hftopic));   // use topicid
						}
						else
							LogEvent(logwarn, 1, "No id for link to topic ", linfo->href, " in: ", linfo->xrname);
					}
					else if (GenerateUIDs) {  // topic has no id, create anchor now
						if ((einfo = (DCtopelem *) tinfo->elems->find(1)) != NULL) {
							if (!einfo->elemuid)
								einfo->elemuid = MakeAnchorName(linfo->href, einfo);
							hftopic = einfo->elemuid;
						}
						else
							hftopic = MakeAnchorName(linfo->href, einfo);
						linfo->factl->ex(hftopic, strlen(hftopic));
					}
					// otherwise already set with empty string
				}
				else
					LogEvent(logwarn, 1, "Missing topic for internal link ", linfo->href, " in: ", linfo->xrname);
			}
			else
				LogEvent(logwarn, 1, "Missing anchor for internal link ", linfo->href, " in: ", linfo->xrname);
		}
#ifdef DITA
		else  // else not in processed doc, leave as is
			LogEvent(logwarn, 1, "Missing text for external link ", linfo->href, " in: ", linfo->xrname);
#endif
	}
	LinksProcessed = LinkCount;
	CurrBranch = branch;
	CurrCondSet = conds;
}


void 
XMLrd::ProcessXrefs(void)
{
	// for xrefs, insert (part of) the element content in group

	long i = 0;

	char *hfdoc = NULL;
	char *hftopic = NULL;
	char *hfelem = NULL;

	long docnum = 0;
	DCtopdoc *dinfo = NULL;
	long topnum = 0;
	DCtopic *tinfo = NULL;
	DCtopic *origtinfo = NULL;
	long elnum = 0;
	DCtopelem *einfo = NULL;
	DCtopelem *ginfo = NULL;

	char *branch = CurrBranch;
	conditions *conds = CurrCondSet;
	xrefinfo *xinfo = NULL;

	for (i = XrefsProcessed + 1; i <= XrefCount; i++) {
		xinfo = (xrefinfo *) Xrefs.find(i);
		if (!xinfo) {
			LogEvent(logwarn, 1, "Missing info for internal xref ",
			                     itoa(i, NewName(10), 10));
			continue;
		}
		if (!xinfo->href)
			continue;
		CurrBranch = xinfo->branch;
		CurrCondSet = xinfo->conds;

		hfdoc = hftopic = hfelem = NULL;
		ParseHref(xinfo->href, &hfdoc, &hftopic, &hfelem);
		if (!hfdoc)
			hfdoc = xinfo->xrname;
		if ((docnum = GetProcessedDocInfo(hfdoc)) != 0) {
			// replace with anchor used for title or other item
			if (!UsePartFiles) {
				xinfo->fnctl->ex(MainDocName, strlen(MainDocName));
				xinfo->fnctl->maj((uns) 0);  // remove it
			}
			if ((dinfo = (DCtopdoc *) DocInfos.find(docnum)) != NULL) {
				topnum = hftopic ? dinfo->topicids->getitem(hftopic) : 1;
				origtinfo = (DCtopic *) dinfo->topics->find(topnum);
				if (GetFinalConrefTopic(&docnum, &topnum)) {
					dinfo = (DCtopdoc *) DocInfos.find(docnum);
					if (!dinfo)
						continue; // ****
				}
				if ((tinfo = (DCtopic *) dinfo->topics->find(topnum)) != NULL) {
					if (UsePartFiles) {
						char *dtname = DCLPartFileNames.find(tinfo->part);
						if (dtname)
							xinfo->fnctl->ex(dtname, strlen(dtname));
						else
							xinfo->fnctl->ex(MainDocName, strlen(MainDocName));
					}
					if (hfelem              // ref is to a specific element
					 && ((elnum = tinfo->elemids->getitem(hfelem)) != 0)) {
						if ((einfo = (DCtopelem *) tinfo->elems->find(elnum)) != NULL) {
							if (GenerateUIDs)
								hfelem = einfo->elemuid;
							xinfo->factl->ex(hfelem, strlen(hfelem));     // use elemid
							// get element content, or part of it like its title, into group
							// for types topic, fig, table, and section, use the title
							// for types li and fn, use the number assigned to the element
							// since xform may depend on the type of the referenced elem,
							// determine its value now, and override the default as needed
							xinfo->xsrc = einfo;
							xinfo->xtsrc = (DCtopelem *) tinfo->elems->find(elnum + 1);
							if (!strcmp(xinfo->xtsrc->elemname, "title"))
								xinfo->gldef = xinfo->xtsrc;
							SetXrefGroup(xinfo);
						}
						else
							LogEvent(logwarn, 1, "No content for xref to element ", xinfo->href, " in: ", xinfo->xrname);
					}
					else if (hftopic
					 || ((hftopic = origtinfo->id) != NULL)) {
						if ((einfo = (DCtopelem *) origtinfo->elems->find(1)) != NULL) {
							if (GenerateUIDs)
								hftopic = einfo->elemuid;
							xinfo->factl->ex(hftopic, strlen(hftopic));
						}
						else
							LogEvent(logwarn, 1, "No id for xref to topic ", xinfo->href, " in: ", xinfo->xrname);
						if (einfo && xinfo->abbr) {  // special proc for abbeviated->form
							if (!xinfo->form)
								xinfo->form = "TextXref";
							if (xinfo->abbr == abfirst) {
								// get glossBody/glossSurfaceForm
								if ((ginfo = GetGlossSurfaceForm(einfo)) != NULL)
									xinfo->xsrc = ginfo;
							}
							else if (xinfo->abbr == abacron) {
								// get glossBody/glossAlt/glossAcronym, may be many glossAlts
								if ((ginfo = GetGlossAcronymForm(einfo)) != NULL)
									xinfo->xsrc = ginfo;
							}
							if (!ginfo)
								xinfo->xsrc = einfo = (DCtopelem *) tinfo->elems->find(2);
							else if ((ginfo = (DCtopelem *) tinfo->elems->find(2)) != NULL)
								xinfo->gldef = ginfo;  // use glossTerm for hovertext
							SetXrefGroup(xinfo);
						}
						else if ((einfo = GetTopicTitle(tinfo)) != NULL) {
							xinfo->xsrc = einfo;
							if ((ginfo = GetTopicDesc(tinfo)) != NULL)
								xinfo->gldef = ginfo;
							SetXrefGroup(xinfo);
						}
#if 0
						else if ((einfo = (DCtopelem *) tinfo->elems->find(2)) != NULL) {
							// get topic title content into group per xref format
							xinfo->xsrc = einfo;
							if (((ginfo = (DCtopelem *) tinfo->elems->find(3)) != NULL)
							 && (strstr(ginfo->classatt, " glossentry/glossdef ")
							  || strstr(ginfo->classatt, " topic/shortdesc ")
							  || strstr(ginfo->classatt, " topic/abstract ")))
								xinfo->gldef = ginfo;
							SetXrefGroup(xinfo);
						}
#endif
						else
							LogEvent(logwarn, 1, "No content for xref to topic ", xinfo->href, " in: ", xinfo->xrname);
					}
					else if (GenerateUIDs) {  // topic has no id, create anchor now
						if ((einfo = (DCtopelem *) tinfo->elems->find(1)) != NULL) {
							if (!einfo->elemuid)
								einfo->elemuid = MakeAnchorName(xinfo->href, einfo);
							hftopic = einfo->elemuid;
						}
						else
							hftopic = MakeAnchorName(xinfo->href, einfo);
						xinfo->factl->ex(hftopic, strlen(hftopic));
						if ((einfo = GetTopicTitle(tinfo)) != NULL) {
							xinfo->xsrc = einfo;
							if ((ginfo = GetTopicDesc(tinfo)) != NULL)
								xinfo->gldef = ginfo;
							SetXrefGroup(xinfo);
						}
#if 0
						if ((einfo = (DCtopelem *) tinfo->elems->find(2)) != NULL) {
							// get topic title content into group per xref format
							xinfo->xsrc = einfo;
							if (((ginfo = (DCtopelem *) tinfo->elems->find(3)) != NULL)
							 && (strstr(ginfo->classatt, " glossentry/glossdef ")
							  || strstr(ginfo->classatt, " topic/shortdesc ")
							  || strstr(ginfo->classatt, " topic/abstract ")))
								xinfo->gldef = ginfo;
							SetXrefGroup(xinfo);
						}
#endif
						else
							LogEvent(logwarn, 1, "No content for xref to topic ", xinfo->href, " in: ", xinfo->xrname);
					}
#if 0
					else if (hftopic
					 || ((hftopic = tinfo->id) != NULL)) {
						if ((einfo = (DCtopelem *) tinfo->elems->find(1)) != NULL) {
							if (GenerateUIDs)
								hftopic = einfo->elemuid;
							linfo->factl->ex(hftopic, strlen(hftopic));   // use topicid
						}
						else
							LogEvent(logwarn, 1, "No id for link to topic ", linfo->href, " in: ", xinfo->xrname);
					}
					else if (GenerateUIDs) {  // topic has no id, create anchor now
						if ((einfo = (DCtopelem *) tinfo->elems->find(1)) != NULL) {
							if (!einfo->elemuid)
								einfo->elemuid = MakeAnchorName(linfo->href, einfo);
							hftopic = einfo->elemuid;
						}
						else
							hftopic = MakeAnchorName(linfo->href, einfo);
						linfo->factl->ex(hftopic, strlen(hftopic));
					}
#endif
				}
				else
					LogEvent(logwarn, 1, "Missing topic for internal xref ", xinfo->href, " in: ", xinfo->xrname);
			}
			else  // else in processed doc, but no anchor
				LogEvent(logwarn, 1, "Missing anchor for internal xref ", xinfo->href, " in: ", xinfo->xrname);
		}
		else  // else not in processed doc, leave as is
			LogEvent(logwarn, 2, "Missing text for external xref ", xinfo->href, " in: ", xinfo->xrname);
	}
	XrefsProcessed = XrefCount;
	CurrBranch = branch;
	CurrCondSet = conds;
}


DCtopelem *
XMLrd::GetTopicTitle(DCtopic *tinfo)
{
	DCtopelem *ginfo = NULL;
	long elnum = 2;

	for ( ; elnum <= tinfo->elemcnt; elnum++) {
		if ((ginfo = (DCtopelem *) tinfo->elems->find(elnum)) != NULL) {
#ifdef UDOC
			//if (UMXL) {
				if (ginfo->checkType(epTitle))
					return ginfo;
			//}
#endif
#ifdef DITA
			//else {
				if (strstr(ginfo->classatt, " glossentry/glossterm ")
				 || strstr(ginfo->classatt, " topic/title "))
					return ginfo;
			//}
#endif
		}
	}

	return NULL;
}


DCtopelem *
XMLrd::GetTopicDesc(DCtopic *tinfo)
{
	DCtopelem *ginfo = NULL;
	long elnum = 3;

	for ( ; elnum <= tinfo->elemcnt; elnum++) {
		if ((ginfo = (DCtopelem *) tinfo->elems->find(elnum)) != NULL) {
#ifdef UDOC
			//if (UMXL) {
				if (ginfo->checkType(epDesc))
					return ginfo;
			//}
#endif
#ifdef DITA
			//else {
				if (strstr(ginfo->classatt, " glossentry/glossdef ")
				 || strstr(ginfo->classatt, " topic/shortdesc ")
				 || strstr(ginfo->classatt, " topic/abstract "))
					return ginfo;
			//}
#endif
		}
	}

	return NULL;
}



// Xref processing


bool XMLrd::InXref = false;
bool XMLrd::PendXref = false;
bool XMLrd::EmptyXref = false;
bool XMLrd::TitleXref = true;
scopetyp XMLrd::XrefExt = scnone;
bool XMLrd::XrefSet = false;
bool XMLrd::XrefDone = false;
DCgroup *XMLrd::XrefTitleGroup = NULL;
DCctl *XMLrd::XrefTitleStr = NULL;
DCnlist XMLrd::XrefFormNameDefs;
DCnlist XMLrd::XrefFormDefs;
long XMLrd::XrefFormDefCount = 0;

char *XMLrd::XrefFile = NULL;
char *XMLrd::XrefID = NULL;
char *XMLrd::XrefHref = NULL;
char *XMLrd::XrefForm = NULL;
char *XMLrd::XrefBranch = NULL;
long XMLrd::XrefDepth = 0;

char *XMLrd::XrefTextFormat = "TextXref";
char *XMLrd::XrefTitleFormat = "TitleXref";
char *XMLrd::XrefNumFormat = "NumXref";
char *XMLrd::XrefFtnFormat = "FtnXref";
abbrtyp XMLrd::XrefAbbr = abnone;

long XMLrd::XrefCount = 0;
long XMLrd::XrefsProcessed = 0;
xrefinfo *XMLrd::CurrXrefInfo = NULL;
DCvlist XMLrd::Xrefs;

DCnlist XMLrd::XrefRefs;
DCnlist XMLrd::XrefFormatNames;
DCnlist XMLrd::XrefElemPaths;
DCclist XMLrd::XrefFormatCtls;
DCclist XMLrd::XrefFileCtls;
DCclist XMLrd::XrefAnchorCtls;
DCclist XMLrd::XrefGroups;

long XMLrd::LinkCount = 0;
long XMLrd::LinksProcessed = 0;
linkinfo *XMLrd::CurrLinkInfo = NULL;
DCvlist XMLrd::Links;

DCnlist XMLrd::LinkRefs;
DCclist XMLrd::LinkFileCtls;
DCclist XMLrd::LinkAnchorCtls;


void
XMLrd::StartXref(long depth)
{
	InXref = true;
	EmptyXref = true;
	PendXref = false;
	//XrefDepth = depth;
	XrefSet = false;
	XrefDone = false;
	InWhite = false;
}

scopetyp
XMLrd::SetXrefScope(const char *scope, char *href)
{
	if (scope)
		return (scopetyp) LinkScopes.getitem(scope);
	else if (href
	 && (*href != '#')
	 && PathIsURL(href))
		return scexternal;
	else
		return sclocal;
}

void
XMLrd::StartXrefContent(void)
{
	if (XrefSet)
		return;

	InXref = true;
	EmptyXref = false;
	PendXref = false;
	XrefSet = true;
	XrefDone = false;

	SetInlineElemTags();

	// put starting link char format and link info
	StartLinkText(XrefHref);
}


void
XMLrd::StartLinkText(char *dest, bool useform)
{
	LinkCount++;
	CurrLinkInfo = new linkinfo;
	CurrLinkInfo->xrname = CurrElemInfo->GetDocName();
	CurrLinkInfo->peer = (XrefExt == scpeer);
	Links.add(CurrLinkInfo, LinkCount);
	CurrLinkInfo->href = dest;
	if (XrefBranch)
		CurrLinkInfo->branch = XrefBranch;
	else
		CurrLinkInfo->branch = CurrBranch;
	CurrLinkInfo->conds = CurrCondSet;

	if (useform)
		SetLinkFormat();
	SetHyperlink(CurrLinkInfo, XrefExt);
	XrefDone = false;
}


void 
XMLrd::SetLinkFormat(void)
{
	if (CurrElemInfo->formctl)
		return;
	CurrLinkInfo->form = XrefForm ? XrefForm : LinkFormat;
	CurrLinkInfo->elpath = GetElemPath(XrefDepth);
	CurrLinkInfo->formctl = SetFormat(LinkFormat, true);
	SetFlags(XrefDepth);
	FormatUsed.add(LinkFormat, XrefDepth);
}


char *XMLrd::XrefFileSuffix = NULL;

char * 
XMLrd::SetPeerLink(char *href)
{
	if (IniFile->Section("PeerLinks")
	 && IniFile->Find(href))
		return IniFile->StrRight();

	// replace suffix, fix up path ****
	char *dot = strrchr(href, '.');
	char *nref = NULL;

	if (XrefFileSuffix) {
		if (*XrefFileSuffix == '.')
			XrefFileSuffix++;
		if (dot) {  // replace suffix
			nref = NewName((dot - href) + strlen(XrefFileSuffix) + 1);
			strncpy(nref, href, dot - href);
		}
		else        // add suffix
			nref = NewName(href, strlen(href) + strlen(XrefFileSuffix) + 1);
		strcat(nref, ".");
		strcat(nref, XrefFileSuffix);
		href = nref;
	}

	return href;
}


void
XMLrd::SetHyperlink(linkinfo *linfo, scopetyp ext, DCtopic *tinfo)
{
	DCctl *cp = NULL;
	char *fname = NULL;
	char *fanchor = NULL;

	if (linfo->href
	 && (ext != sclocal)) {
		if (*linfo->href == '#')
			ext = sclocal;
		else if (PathIsURL(linfo->href))
			ext = scexternal;
	}

	if (ext == scexternal) {  // scope = external
		cp = new DCctl(etext, 2, 112, 15);
		cp->ex("_blank", 7);
		TextGroup->add(*cp);
	}
	else if (ext == scpeer)
		linfo->href = SetPeerLink(linfo->href);

	if (linfo->href
	 && ((ext == scexternal)
	  || PathIsURL(linfo->href))) {
		cp = new DCctl(etext, 2, 112, 7);
		cp->ex(linfo->href, strlen(linfo->href));
		TextGroup->add(*cp);
		linfo->url = true;
	}
	else {
		if (linfo->href)
			ParseHref(linfo->href, &fname, &fanchor);
		if (tinfo) {  // topic already processed
			if (!UsePartFiles
			 || ((fname = DCLPartFileNames.find(tinfo->part)) == NULL))
				fname = MainDocName;
			if (fanchor
			 || ((fanchor = tinfo->id) != NULL)) {
				DCtopelem *einfo = (DCtopelem *) tinfo->elems->find(1);
				if (einfo && GenerateUIDs)
					fanchor = einfo->elemuid;
			}
		}
		if (fname
		 && !InGlossTerm) {
			cp = new DCctl(etext, 2, 112, 3);
			cp->ex(fname, strlen(fname));
			TextGroup->add(*cp);
			linfo->fnctl = cp;
		}

		cp = new DCctl(ilong, 2, 112, 2, ++ItemID);
		TextGroup->add(*cp);

		if (InXref
		 && TitleXref) {
			DCgroup *cgp = new DCgroup(DCctl(group, 2, 96, 6));
			//TextGroup->add(*cgp);
			XrefTitleGroup = cgp;
			cp = new DCctl(etext, 2, 96, 6);
			XrefTitleStr = cp;
			TextGroup->add(*cp);
		}

		if (!fanchor)
			fanchor = "";
		cp = new DCctl(etext, 2, 112, 4);
		cp->ex(fanchor, strlen(fanchor));
		TextGroup->add(*cp);
		linfo->factl = cp;
	}
}


void
XMLrd::EndXref(const char *name)
{
	if (XrefDone)
		return;

	bool genx = false;
	char *xtitle = NULL;

	// if no content before ending elem, it's a real xref
	//  unless scope is ext or peer
	// if content found, it's a hyperlink
	if (XrefHref
	 && (XrefExt != sclocal)
	 && EmptyXref) {
		StartLinkText(XrefHref);
		PutTextCharCtl(' ');
		PutTextCharCtl('<');
		PutTextStrCtl(XrefHref);
		PutTextCharCtl('>');
		SetFormat(NULL, true);
	}
	else if (XrefHref
	 && (XrefExt == sclocal)
	 && (EmptyXref
	  || TitleXref)) {
		XrefCount++;
		CurrXrefInfo = new xrefinfo;
		if (CurrElemInfo) {
			CurrXrefInfo->abbr = CurrElemInfo->abbr;
			CurrXrefInfo->xrname = CurrElemInfo->GetDocName();
		}
		if (!CurrXrefInfo->xrname)
			CurrXrefInfo->xrname = "unknown";

		// add to list for later ref processing
		Xrefs.add(CurrXrefInfo, XrefCount);
		if (XrefBranch)
			CurrXrefInfo->branch = XrefBranch;
		else
			CurrXrefInfo->branch = CurrBranch;
		CurrXrefInfo->conds = CurrCondSet;

		SetFlags(XrefDepth);

		DCgroup *gp = new DCgroup(DCctl(group, 2, 10, 7));
		if (EmptyXref) 
			TextGroup->add(*gp);
		DCgroup *lasttext = TextGroup;
		TextGroup = CurrXrefInfo->xrgrp = gp;

		if (XrefForm)  // from xref outputclass
			CurrXrefInfo->form = XrefForm;

		DCctl *cp = new DCctl(imnem, 2, 96, 3, 0);  // fill in later
		if (CurrElemInfo)
			CurrXrefInfo->elpath = GetElemPath(XrefDepth);

		if (EmptyXref) {
			TextGroup->add(*cp);
			CurrXrefInfo->formctl = cp;

			cp = new DCctl(ilong, 2, 112, 2, ++ItemID);
			TextGroup->add(*cp);

			SetInlineElemTags();
		}

		if (XrefHref)
			ParseHref(XrefHref, &XrefFile, &XrefID);

		// store unparsed HrefXref, ctls, and the content group ptr,
		//  flag as xref (rather than conref)
		CurrXrefInfo->href = XrefHref;
		CurrXrefInfo->ingloss = InGlossary;
		if (!XrefFile)
			XrefFile = CurrXrefInfo->xrname;

		// if name is for generated file, and local,
		// file is current and add generated location as id
		if (GenFileCount) {
			char *xbase = NewName(XrefFile);
			char *dot = strrchr(xbase, '.');
			if (dot)
				*dot = '\0';
			char *slash = strrchr(xbase, '/');
			if (!slash)
				slash = strrchr(xbase, '\\');
			if (!slash)
				slash = xbase;
			else
				slash++;
			long gnum = GenFileNames.getitem(slash);
			if (gnum) {
				genx = true;
				XrefFile = NULL;
				DCgenlist *gl = (DCgenlist *) GenLists.find(gnum);
				XrefID = gl->id;
				xtitle = gl->title;
				SetFinalXrefForm(CurrXrefInfo);
			}
		}

		if (!genx) {
			cp = new DCctl(ename, 2, 96, 5);
			if (XrefFile) 
				cp->ex(XrefFile);
			TextGroup->add(*cp);
			CurrXrefInfo->fnctl = cp;
		}

		if (EmptyXref) {
			cp = new DCctl(etext, 2, 96, 6);
			TextGroup->add(*cp);
			CurrXrefInfo->tctrl = cp;  // for gldef
		}

		cp = new DCctl(etext, 2, 96, 2);
		if (XrefID)  // otherwise add later
			cp->ex(XrefID);
		TextGroup->add(*cp);
		CurrXrefInfo->factl = cp;

		// if ref found, insert appropriate content from it in group
		// if not found, log error, leave group empty or add error msg
		// insert content into this group
		if (EmptyXref) {
			DCgroup *cgp = new DCgroup(DCctl(group, 2, 98, 4));
			TextGroup->add(*cgp);
			CurrXrefInfo->xgrp = cgp;
			if (xtitle) {
				cp = new DCctl(etext, 2, 10, 1);
				cp->ex(xtitle, strlen(xtitle) + 1);
				cgp->add(*cp);
			}
			if (name
			 && IncludeElementTags) {
				cp = new DCctl(ename, 2, 121, 3);
				cp->ex(name);
				TextGroup->add(*cp);
			}
		}
		else if (TitleXref
		 && XrefTitleGroup) {
			CurrXrefInfo->tgrp = XrefTitleGroup;
			CurrXrefInfo->tctrl = XrefTitleStr;
			XrefTitleGroup = NULL;
			XrefTitleStr = NULL;
		}
		

		TextGroup = lasttext;
	}

	if (!EmptyXref) // put ending link format
		SetFormat(NULL, true);

	SetFlags(XrefDepth - 1);
	InXref = false;
	EmptyXref = true;
	XrefSet = false;
	XrefDone = true;
	XrefForm = NULL;
	XrefBranch = NULL;
}


void 
XMLrd::SetXrefGroup(xrefinfo *xinfo)
{
	bool title = false;
	DCelem *einfo = xinfo->xsrc;
	DCelem *etinfo = xinfo->xtsrc;

	if (!etinfo)
			etinfo = einfo;
	else if (!einfo->elform)  // for benefit of SetXrefFormat
		einfo->elform = etinfo->elform;

	// set group content based on format in xform and data in einfo
	DCgroup *xrgp = xinfo->xgrp;
	if (!xrgp) {
		xrgp = xinfo->tgrp;
		title = true;
	}

	if (einfo->elemtype == fn) {  // referenced footnote, must have id
		DCgroup *fngrp = ((DCtopelem *) einfo)->fntext;
		if (fngrp) {     // not used to replace xref yet
			((DCtopelem *) einfo)->topic->page->setgroup(&fngrp);
			xinfo->xrgrp->empty();                 // remove xref content
			xinfo->xrgrp->maj((uns) 0);            // make group a placeholder
			xinfo->xrgrp->merge(*fngrp);           // put fn in its place
			((DCtopelem *) einfo)->fntext = NULL;  // mark fn as inserted
			return;
		}
	}

	DCgroup *lasttext = TextGroup;
	TextGroup = xrgp;  // to catch format setting

	char *fcp = SetFinalXrefForm(xinfo, einfo, title);

	SetXrefContent(fcp, einfo, etinfo);

	char *estr = NULL;
	char *nstr = NULL;
	char *tstr = NULL;

	if (einfo->tdesc) {
		estr = einfo->tdesc;
		xinfo->tctrl->ex(estr);
	}
	else if (einfo->parent
	 && einfo->parent->tdesc) {
		estr = einfo->parent->tdesc;
		xinfo->tctrl->ex(estr);
	}
	else if (xinfo->gldef
	 && (xinfo->gldef->elemtext	
	  || xinfo->gldef->elemtref
	  || xinfo->gldef->elemnref)) {
		if (xinfo->gldef->elemnref)
			nstr = (xinfo->gldef->elemnref)->getText();
		if (xinfo->gldef->elemtref)
			tstr = (xinfo->gldef->elemtref)->getText();
		if (nstr && tstr) {
			estr = NewName(nstr, strlen(nstr) + strlen(tstr) + 2);
			strcat(estr, " ");
			strcat(estr, tstr);
		}
		else if (tstr)
			estr = tstr;
		else if (xinfo->gldef->elemtext)
			estr = (xinfo->gldef->elemtext)->getText();
		xinfo->tctrl->ex(estr);
	}
	else if (!xinfo->tctrl->siz()) {
		if (xinfo->ingloss)
			xinfo->tctrl->ex(DocRoot);
		else if (title)
			xinfo->tctrl->ex((xrgp)->getText());
		else // tctrl not used, disable
			xinfo->tctrl->maj((uns) 0);
	}

	TextGroup = lasttext;
}


char *
XMLrd::SetFinalXrefForm(xrefinfo *xinfo, DCelem *einfo, bool title)
{
	char *xform = xinfo->form;  // from outputclass
	char *epath = xinfo->elpath;
	xform = SetXrefFormat(xform, einfo, epath);  // set final format

	char *fcp = GetXrefFormat(xform);

	if (!title) {
		DCctl *cp = xinfo->formctl;
		cp->siz(XrefFormNameDefs.getitem(xform));  // set format ctl
	}

	return fcp;
}


void 
XMLrd::SetXrefContent(char *fcp, DCelem *einfo, DCelem *etinfo)
{
	DCctl *cp = NULL;
	DCgroup *gp = NULL;
	long tlnum = 1;  // tab level num
	long atabw = AnumTabWidth * 100;
	char *ntok = NULL;
	char formname[FormNameMax] = "";
	bool typo = false;
	DCnlist *nestlist = NULL;
	DCilist *typolist = NULL;
	long fnest = 0;

	while ((ntok = strchr(fcp, '<')) != NULL) {
		if (ntok > fcp) {
			if ((ntok - fcp) > 1) {
				cp = new DCctl(etext, 2, 10, 1);
				cp->ex(fcp, ntok - fcp);
			}
			else
				cp = new DCctl(imnem, 2, 10, 1, (unc) *fcp);
			TextGroup->add(*cp);
		}
		fcp = ntok;
		while (*fcp
		 && (*fcp != '>'))
			fcp++;
		if (!*fcp)
			LogEvent(logwarn, 1, "Xref format tag missing ending '>': ", ntok);
		ntok++;
		if (MatchToken(ntok, fcp, "tab", 3)) {  // tab token
		//if (((fcp - ntok) == 4)
		// && !strnicmp(ntok, "tab/", 4))
			cp = new DCctl(ilong, 2, 41, 1, (atabw * tlnum++));
			TextGroup->add(*cp);
		}
		else if (MatchToken(ntok, fcp, "spc", 3)) {
			cp = new DCctl(imnem, 2, 10, 1, (unl) 160);
			TextGroup->add(*cp);
		}
		else if (MatchToken(ntok, fcp, "paratext", 8)) {  // text content
			if (einfo->elemtref != NULL)
				TextGroup->add(*(einfo->elemtref));
			else if (etinfo->elemtref != NULL)
				TextGroup->add(*(etinfo->elemtref));
			else if (einfo->elemtext != NULL)
				PutTextStrCtl((einfo->elemtext)->getText());
			else if (etinfo->elemtext != NULL)
				PutTextStrCtl((etinfo->elemtext)->getText());
		}
		else if (MatchToken(ntok, fcp, "numonly", 7)) {  // number content
			if (einfo->elemnref != NULL)
				TextGroup->add(*(einfo->elemnref));
			else if (etinfo->elemnref != NULL)
				TextGroup->add(*(etinfo->elemnref));
		}
		else if (MatchToken(ntok, fcp, "autonum", 7)) {  // full autonumber content
			if (einfo->elemaref != NULL)
				TextGroup->add(*(einfo->elemaref));
			else if (etinfo->elemaref != NULL)
				TextGroup->add(*(etinfo->elemaref));
		}
		else if (MatchToken(ntok, fcp, "page", 4))    {  // page number
			gp = new DCgroup(DCctl(group, 2, 98, 5));
			TextGroup->add(*gp);
			cp = new DCctl(imnem, 2, 10, 1, '0');
			gp->add(*cp);
		}
		else if (MatchToken(ntok, fcp, "tag", 3))     {  // tag name
			gp = new DCgroup(DCctl(group, 2, 98, 3));
			TextGroup->add(*gp);
			if (einfo->elemname != NULL) {
				cp = new DCctl(etext, 2, 10, 1);
				cp->ex(einfo->elemname, strlen(einfo->elemname));
			}
			else
				cp = new DCctl(imnem, 2, 10, 1, 'X');
			gp->add(*cp);
		}
		else if ((tolower(*ntok) == 'u')
		 && (*(ntok + 1) == '+')) {  // Unicode code point
			char *nend = NULL;
			unl uch = strtoul(ntok + 2, &nend, 16);
			if ((nend != (fcp - 1))
			 || (*(fcp - 1) != '/'))
				LogEvent(logwarn, 1, "Unicode char error in xref form: ", ntok - 1);
			else {
				cp = new DCctl(imnem, 2, 10, 1, uch);
				TextGroup->add(*cp);
			}
		}
		else {  // treat anything else as an inline format
			if (*ntok != '/') {  // start the named format
				if (fnest) { // nest format
					if (!nestlist)
						nestlist = new DCnlist();
					nestlist->addnew(formname, fnest);
					if (!typolist)
						typolist = new DCilist();
					typolist->add(typo, fnest);
				}
				fnest++;
				strncpy(formname, ntok, fcp - ntok);
				formname[fcp - ntok] = '\0';
				if (SetTypographic(formname, true))
					typo = true;
				else
					SetFormat(formname, true); 
			}
			else if (!strnicmp(ntok + 1, formname, (fcp - ntok) - 1)) {
				// end any current format
				if (typo) {
					SetTypographic(formname, false);
					typo = false;
				}
				else
					SetFormat(NULL, true);
				*formname = '\0';
				if (--fnest) {
					strcpy(formname, nestlist->find(fnest));
					typo = typolist->find(fnest);
				}
			}
			else
				LogEvent(logwarn, 1, "Xref <format> token error: ", ntok);
		}
		if (*fcp)  // move after ending bracket
			ntok = ++fcp;
	}
	if (*fcp) { // more left, finish it
		if (strlen(fcp) > 1) {
			cp = new DCctl(etext, 2, 10, 1);
			cp->ex(fcp, strlen(fcp));
		}
		else
			cp = new DCctl(imnem, 2, 10, 1, (unc) *fcp);
		TextGroup->add(*cp);
	}
	while (fnest) {
		if (typo) {
			SetTypographic(formname, false);
			typo = false;
		}
		else if (*formname != '\0')
			SetFormat(NULL, true);
		if (--fnest) {
			strcpy(formname, nestlist->find(fnest));
			typo = typolist->find(fnest);
		}
	}
}


void 
XMLrd::WriteXrefFormatDef(DCgroup *fp, char *fcp, char *xform)
{
	char *ntok = NULL;
	DCctl *cp = NULL;
	long fnum = 0;
	char formname[FormNameMax] = "";
	bool typo = false;
	DCnlist *nestlist = NULL;
	DCilist *typolist = NULL;
	long fnest = 0;

	if (!fcp)
		fcp = GetDefaultXrefFormatDef(xform);

	while ((ntok = strchr(fcp, '<')) != NULL) {
		if (ntok > fcp) {
			if ((ntok - fcp) > 1) {
				cp = new DCctl(etext, 2, 10, 1);
				cp->ex(fcp, ntok - fcp);
			}
			else
				cp = new DCctl(imnem, 2, 10, 1, (unc) *fcp);
			fp->add(*cp);
		}
		fcp = ntok;
		while (*fcp
		 && (*fcp != '>'))
			fcp++;
		if (!*fcp)
			LogEvent(logwarn, 1, "Xref format tag missing ending '>': ", ntok);
		ntok++;
		if (MatchToken(ntok, fcp, "tab", 3)) {  // tab token
			cp = new DCctl(ilong, 2, 41, 8);
			fp->add(*cp);
		}
		else if (MatchToken(ntok, fcp, "spc", 3)) {
			cp = new DCctl(imnem, 2, 10, 1, (unl) 160);
			fp->add(*cp);
		}
		else if (MatchToken(ntok, fcp, "paratext", 8)) {  // text content
			cp = new DCctl(imnem, 2, 97, 35, 4);
			fp->add(*cp);
		}
		else if (MatchToken(ntok, fcp, "numonly", 7)) {  // number content
			cp = new DCctl(imnem, 2, 97, 35, 1);
			fp->add(*cp);
		}
		else if (MatchToken(ntok, fcp, "autonum", 7)) {  // full autonumber content
			cp = new DCctl(imnem, 2, 97, 35, 2);
			fp->add(*cp);
		}
		else if (MatchToken(ntok, fcp, "page", 4)) {     // page number
			cp = new DCctl(imnem, 2, 97, 35, 5);
			fp->add(*cp);
		}
		else if (MatchToken(ntok, fcp, "tag", 3)) {      // tag name
			cp = new DCctl(imnem, 2, 97, 35, 3);
			fp->add(*cp);
		}
		else if ((tolower(*ntok) == 'u')
		 && (*(ntok + 1) == '+')) {  // Unicode code point
			char *nend = NULL;
			unl uch = strtoul(ntok + 2, &nend, 16);
			if ((nend != (fcp - 1))
			 || (*(fcp - 1) != '/'))
				LogEvent(logwarn, 1, "Unicode char error in xref form: ", ntok - 1);
			else {
				cp = new DCctl(imnem, 2, 10, 1, uch);
				fp->add(*cp);
			}
		}
		else {  // treat anything else as an inline format
			if (*ntok != '/') {  // start the named format
				if (fnest) { // nest format
					if (!nestlist)
						nestlist = new DCnlist();
					nestlist->addnew(formname, fnest);
					if (!typolist)
						typolist = new DCilist();
					typolist->add(typo, fnest);
				}
				fnest++;
				strncpy(formname, ntok, fcp - ntok);
				formname[fcp - ntok] = '\0';
				if ((cp = SetTypographic(formname, true)) != NULL)
					typo = true;
				else {
					if ((fnum = InlineFormatList.getitem(formname)) == 0)
						fnum = StoreFormat(formname, true);
					cp = new DCctl(imnem, 2, 82, 2, fnum);
					InlineFormats.add(cp, ++InlineFormUseCount);
				}
				fp->add(*cp);
			}
			else if (!strnicmp(ntok + 1, formname, (fcp - ntok) - 1)) {
				// end any current format
				if (typo) {
					cp = SetTypographic(formname, false);
					typo = false;
				}
				else
					cp = new DCctl(imnem, 2, 82, 2, 0);
				fp->add(*cp);
				*formname = '\0';
				if (--fnest) {
					strcpy(formname, nestlist->find(fnest));
					typo = typolist->find(fnest);
				}
			}
			else
				LogEvent(logwarn, 1, "Xref <format> token error: ", ntok);
		}
		if (*fcp)  // move after ending bracket
			ntok = ++fcp;
	}
	if (*fcp) { // more left, finish it
		if (strlen(fcp) > 1) {
			cp = new DCctl(etext, 2, 10, 1);
			cp->ex(fcp, strlen(fcp));
		}
		else
			cp = new DCctl(imnem, 2, 10, 1, (unc) *fcp);
		fp->add(*cp);
	}

	while (fnest) {
		if (typo) {
			cp = SetTypographic(formname, false);
			fp->add(*cp);
			typo = false;
		}
		else if (*formname != '\0') {
			cp = new DCctl(imnem, 2, 82, 2, 0);
			fp->add(*cp);
		}
		if (--fnest) {
			strcpy(formname, nestlist->find(fnest));
			typo = typolist->find(fnest);
		}
	}
}


char *
XMLrd::GetDefaultXrefFormatDef(char *xform)
{
	if (!xform || !*xform)
		return "<paratext/>";

	// called when no definition found for format
	if (!strcmp(xform, XrefNumFormat))
		return "<numonly/>";
	if (!strcmp(xform, XrefTextFormat))
		return "<paratext/>";
	if (!strcmp(xform, XrefTitleFormat))
		return "<autonum/><paratext/>";
	if (!strcmp(xform, XrefFtnFormat))
		return "<sup><numonly/>";

	LogEvent(logwarn, 1, "Undefined xref format: ", xform);
	return "<paratext/>";
}


char *
XMLrd::GetDefaultXrefFormatName(DCelem *einfo)
{
	// go by the type attribute in the xref
	switch (einfo->elemtype) {
		case li:
			return XrefNumFormat;

		case fn:
			return XrefFtnFormat;

		case other:
			break;

		default:  // all the rest are likely to be numbered
			return XrefTitleFormat;
	}
	return XrefTextFormat;
}


char *
XMLrd::GetXrefFormat(char *form)
{
	char *format = NULL;

	if (!form)
		return NULL;

	long fnum = XrefFormNameDefs.getitem(form);
	if (!fnum) 
		return NULL;

	format = XrefFormDefs.find(fnum);

	return format;
}


char *
XMLrd::SetXrefFormat(char *form, DCelem *einfo, char *epath)
{
	long fnum = 0;
	char *format = NULL;

#if 0
	if (OldXrefs) {
		if (!form) {  // not in outputclass of xref element
			if (einfo->elform
			 && IniFile->Section("XrefProperties")
			 && IniFile->Find(einfo->elform))
				form = IniFile->StrRight();
			else if (epath
			 && IniFile->Section("XrefFormatMaps")
			 && IniFile->FindElemPath(epath))
				form = IniFile->StrRight();
			else
				form = GetDefaultXrefFormatName(einfo);
		}
		else {  // see if outputclass remapped
			if (IniFile->Section("XrefOutclassMaps")
			 && IniFile->Find(form))
				form = IniFile->StrRight();
		}

		fnum = XrefFormNameDefs.getitem(form);
		if (!fnum) {
			XrefFormNameDefs.add(form, (fnum = ++XrefFormDefCount));
			if (IniFile->Section("XrefFormats")
			 && IniFile->Find(form))
				format = IniFile->StrRight();
			else
				format = GetDefaultXrefFormatDef(form);
			XrefFormDefs.add(format, fnum);
		}
	}
	else {
#endif

		if (form) {  // has outputclass
			if (IniFile->Section("XrefOutclassMaps")
			 && IniFile->Find(form))
				form = IniFile->StrRight();
		}
		else if (einfo
		 && einfo->elform)
			form = FmtFile->GetXrefFmtName(einfo->elform);

		if (!form
		 && einfo)
			form = GetDefaultXrefFormatName(einfo);

		fnum = XrefFormNameDefs.getitem(form);
		if (!fnum) {
			XrefFormNameDefs.add(form, (fnum = ++XrefFormDefCount));
			if ((format = SubFormatsFile->GetXrefFmtForm(form)) == NULL)
				format = GetDefaultXrefFormatDef(form);
			XrefFormDefs.add(format, fnum);
		}
	//}

	return form;
}



// index markers


bool XMLrd::InIndex = false;
bool XMLrd::InSee = false;
bool XMLrd::InSeeAlso = false;
bool XMLrd::InSortAs = false;
bool XMLrd::SortAsUsed = false;
bool XMLrd::NopageEntry = false;
char *XMLrd::IndexSeeStart = "SeeStartIndex";
char *XMLrd::IndexSeeAlsoStart = "SeeAlsoStartIndex";
char *XMLrd::IndexSeeEnd = "SeeEndIndex";
char *XMLrd::IndexSeeAlsoEnd = "SeeAlsoEndIndex";

uns XMLrd::IndexLevel = 0;
uns XMLrd::IndexTopLevel = 0;
DCgroup *XMLrd::CurrIndexGroup = NULL;
DCgroup *XMLrd::PreIndexGroup = NULL;
DCgroup *XMLrd::IndexLevelGroup = NULL;
DCclist XMLrd::IndexLevels;
DCgroup *XMLrd::IndexLevelSortGroup = NULL;
DCgroup *XMLrd::IndexLevelSeeGroup = NULL;
DCclist XMLrd::IndexSortLevels;
DCnlist XMLrd::IndexRangeList;
DCnlist XMLrd::IndexEndRangeList;
DCgroup *XMLrd::EndingForCurrIndexGroup = NULL;
DCgroup *XMLrd::PendingIndexItems = NULL;


void
XMLrd::StartIndexItem(void)
{

#if 0
	if (UMXL) {
		XMLud::StartUMXLIndexItem();
		return;
	}
#endif

	InIndex = true;
	
	if (++IndexLevel == 1) {  // always the case for UMXL
		// CurrElemInfo has outclass, need to determine if this elem
		// should make an index entry, or just be used to gen an index
		CheckIndexListItem();

		CurrIndexGroup = new DCgroup(DCctl(group, 2, 110, 1));
		if (UseEntry
		 && !InVar)
			TextGroup->add(*CurrIndexGroup);
		if (UseList || UseDefList)
			StartIndexListItem();
		else
			DCixref::CurrIXRef = NULL;
		PreIndexGroup = TextGroup;
		IndexLevels.empty();
		IndexSortLevels.empty();
		SortAsUsed = NopageEntry = false;
	}
	else {  // start of new level for DITA, not UMXL
		FlushText();
		if (IndexLevels.find(IndexLevel) != NULL)  // level already used
			RestartIndexItem();
		else {
			TextGroup = CurrIndexGroup;
			if (UseList || UseDefList)
				AddIndexListItemLevel();
		}
		PutTextCharCtl(':');
	}

	IndexLevelGroup = new DCgroup;
	IndexLevels.add(IndexLevelGroup, IndexLevel);
	CurrIndexGroup->add(*IndexLevelGroup);
	TextGroup = IndexLevelGroup;
	IndexTopLevel = IndexLevel;
	if ((UseList || UseDefList)
	 && (IndexLevel > 1))
			CurrListIndexGroup = IndexLevelGroup;

	char *start = CurrElemInfo->getAttr("start");
	char *end = CurrElemInfo->getAttr("end");
	ProcIndexRange(start, end);
}


void
XMLrd::ProcIndexRange(char *start, char *end)
{
	// process any start and end attrs for DITA
	DCctl *cp = NULL;
	DCgroup *gp = NULL;

	// (imnem, 2, 97, 40, 3) for start range,
	if (start != NULL) {
		if (UseList || UseDefList)
			DCixref::CurrIXRef->setrange(rngstart, start);
		cp = new DCctl(imnem, 2, 97, 40, 3);
		CurrIndexGroup->insert(*cp);
		// see if end group was read first in a different topic
		gp = (DCgroup *) IndexEndRangeList(start);
		if (gp)  // copy start group to end group when done
			EndingForCurrIndexGroup = gp;
		else
			IndexRangeList.add(start, (long) CurrIndexGroup);
	}

	// (imnem, 2, 97, 40, 4) for end range
	if (end != NULL) {
		if (UseList || UseDefList)
			DCixref::CurrIXRef->setrange(rngend, end);
		cp = new DCctl(imnem, 2, 97, 40, 4);
		CurrIndexGroup->insert(*cp);
		gp = (DCgroup *) IndexRangeList(end);
		if (gp)   // copy start group to end group
			CopyIndexEntry(gp, CurrIndexGroup);
		else      // no start gpoup processed yet
			IndexEndRangeList.add(end, (long) CurrIndexGroup);
	}
}


void
XMLrd::ProcIndexItemText(void)
{
	// add escapes for DITA, not needed for UMXL

	if (!PendingText)
		return;

	char *txt = NewName((PendingLen * 2) + 1);
	char *pto = PendingText;
	char *ptn = txt;

	for (int i = 0; i < PendingLen; i++) {
		switch (*pto) {
			case '\\': // already escaped, skip
				*ptn++ = *pto++;
				break;

			case ':':
				if (!UMXL)
					*ptn++ = '\\';
				break;

			case '[':
				if (!UMXL)
					*ptn++ = '\\';
				break;

			case ']':
				if (!UMXL)
					*ptn++ = '\\';
				break;

			case ';':
			case '<':
			case '>':
				*ptn++ = '\\';
				break;

			default:
				break;
		}
		*ptn++ = *pto++;
	}
	*ptn = '\0';
	PendingLen = (ptn - txt);
	DeleteName(PendingText);

	PendingText = NewName(txt, PendingLen + 1);
	DeleteName(txt);
}


void
XMLrd::CopyIndexEntry(DCgroup *gp, DCgroup *dp)
{
	DCctl *cp = NULL;
	DCitem *ip = NULL;
	DCctl *op = NULL;

	ip = gp->first;
	while (ip) {
		op = ip->ctl;
		if (op->dt() == group)  // level group
			CopyIndexEntry((DCgroup *) op, dp);
		if (op->match(2, 97, 40, 1)
		 || op->match(2, 10, 1)
		 || op->match(2, 15)
		 || op->match(2, 82, 2)) {
			cp = new DCctl();
			*cp = *op;
			dp->add(*cp);
		}
		ip = ip->next;
	}
}


void
XMLrd::RestartIndexItem(void)
{
	uns i = 0;

	// make new index entry starting with same higher levels
	if (SortAsUsed) {
		WriteIndexSort();
		SortAsUsed = false;
	}
	if (NopageEntry) {
		TextGroup->add(*(new DCctl(ilong, 2, 97, 40, 1)));
		NopageEntry = false;
	}
	if (UseList || UseDefList)
		RestartIndexListItem();
	CurrIndexGroup = new DCgroup(DCctl(group, 2, 110, 1));
	PreIndexGroup->add(*CurrIndexGroup);
	TextGroup = CurrIndexGroup;
	SortAsUsed = false;
	for (i = 1; i < IndexLevel; i++) {  // retain higher-level info
		if (i > 1)
			PutTextCharCtl(':');
		TextGroup->add(*(IndexLevels.find(i)));
		if (IndexSortLevels.find(i) != NULL)
			SortAsUsed = true;
	}
	for ( ; i <= IndexTopLevel; i++) {  // discard lower-level info
		IndexLevels.remove(i);
		IndexSortLevels.remove(i);
	}
}


void
XMLrd::ProcIndexElemStart(void)
{
	if (!InIndex)
		return;

	FlushText();

	if (UseList || UseDefList)
		StartIndexListSeeSortItem();

	// determine elem type and process it
	// may be see, see-also, sort-as (DITA), or range (UMXL)
	if (CurrElemInfo->checkType(epIndexSeeAlso)) {
		InSeeAlso = NopageEntry = true;
		// add colon
		ProcIndexSeeStart(IndexSeeAlsoStart, ':');
	}
	else if (CurrElemInfo->checkType(epIndexSee)) {
		InSee = NopageEntry = true;
		// add comma
		ProcIndexSeeStart(IndexSeeStart, ',');
	}
	else if (CurrElemInfo->checkType(epIndex, epText)) {
		// range element, empty
		char *start = CurrElemInfo->getAttr("id");
		char *end = CurrElemInfo->getAttr("idref");
		ProcIndexRange(start, end);
	}
	else if (CurrElemInfo->checkType(epIndexSort)) {
		InSortAs = true;
		// allow for sort-as on each level, assemble at end of entry
		if (IndexSortLevels.find(IndexLevel)) {
			LogEvent(logwarn, 2, "More than one index-sort-as in indexitem");
			IndexSortLevels.remove(IndexLevel);  // keep only the last one
		}
		IndexLevelSortGroup = new DCgroup;
		TextGroup = IndexLevelSortGroup;
		IndexSortLevels.add(IndexLevelSortGroup, IndexLevel);
	}
}


void
XMLrd::ProcIndexSeeStart(char *start, char first)
{
	IndexLevel++;  // treat as added level
	if (IndexLevels.find(IndexLevel) != NULL)  // level already used
		RestartIndexItem();
	else {
		TextGroup = CurrIndexGroup;
		if (UseList || UseDefList)
			AddIndexListSeeItemLevel();
	}
	if (UseList || UseDefList)
		DCixref::CurrIXRef->itype = (InSee ? ixsee : ixalso);
	if (first) {  // comma or colon
		DCctl *cp = new DCctl(imnem, 2, 10, 1, first & 0x7F);
		TextGroup->add(*cp);
	}
	ProcIndexElemString(start);
	IndexLevelSeeGroup = new DCgroup;
	IndexLevels.add(IndexLevelSeeGroup, IndexLevel);
	CurrIndexGroup->add(*IndexLevelSeeGroup);
	TextGroup = IndexLevelSeeGroup;
}


void
XMLrd::ProcIndexElemString(char *str)
{
	WriteIndexSeeItem(TextGroup, str);
}


void
XMLrd::ProcIndexElemEnd(void)
{
	if (!InIndex
	 || !CurrElemInfo)
		return;

	if (CurrElemInfo->checkType(epIndexSeeAlso)) {
		if (UseList || UseDefList)
			EndIndexListSeeItem();
		if (IndexSeeAlsoEnd)
			ProcIndexElemString(IndexSeeAlsoEnd);
		IndexLevel--;
		TextGroup = CurrIndexGroup;
		InSeeAlso = false;
	}
	else if (CurrElemInfo->checkType(epIndexSee)) {
		if (UseList || UseDefList)
			EndIndexListSeeItem();
		if (IndexSeeEnd)
			ProcIndexElemString(IndexSeeEnd);
		IndexLevel--;
		TextGroup = CurrIndexGroup;
		InSee = false;
	}
	else if (CurrElemInfo->checkType(epIndexSort)) {
		if (UseList || UseDefList)
			EndIndexListSortItem();
		InSortAs = false;
		SortAsUsed = true;
		TextGroup = CurrIndexGroup;
	}
}


void
XMLrd::EndIndexItem(void)
{
	if (IndexLevel == 0)
		return;

#if 0
	if (UMXL) {
		XMLud::EndUMXLIndexItem();
		return;
	}
#endif

	if (UseList || UseDefList)
		EndIndexListItem();

	if (--IndexLevel == 0) {
		if (SortAsUsed) {
			WriteIndexSort();
			IndexSortLevels.empty();
		}
		if (NopageEntry) {
			DCctl *cp = new DCctl(imnem, 2, 97, 40, 1);
			CurrIndexGroup->insert(*cp);
		}
		if (EndingForCurrIndexGroup) {
			CopyIndexEntry(CurrIndexGroup, EndingForCurrIndexGroup);
			EndingForCurrIndexGroup = NULL;
		}
		SetUniqueID();
		TextGroup = PreIndexGroup;
		InIndex = false;
	}
	else {  // end of a level
		TextGroup = CurrIndexGroup;
	}
}


void 
XMLrd::WriteIndexSort(void)
{
	DCctl *cp = NULL;

	TextGroup = CurrIndexGroup;
	PutTextCharCtl('[');
	for (long i = 1; i <= IndexTopLevel; i++) {
		if (i > 1)
			PutTextCharCtl(':');
		if ((cp = IndexSortLevels.find(i)) == NULL)      // if no sort-as,
			cp = IndexLevels.find(i);  // use regular content for that level
		TextGroup->add(*cp);
	}
	PutTextCharCtl(']');
}






// ref utilities


void 
XMLrd::ParseHref(const char *href, char **pfile, char **ptopicid, char **pelemid)
{
	if (!href
	 || !*href
	 || !pfile
	 || !ptopicid)
		return;

	char *pound = strchr(href, '#');
	char *slash = pound ? strchr(pound, '/') : NULL;
	char *dot = NULL;
	bool map = false;

	if (pound >= href) { // has a topic id
		*ptopicid = NewName(pound + 1, slash ? ((slash - pound) - 1) : 0);
		if (pound > href) { // has a file name
			*pfile = NewName(href, pound - href);
			if (((dot = strrchr(*pfile, '.')) != NULL)
			 && strstr(dot, "map"))  // map or bookmap
				map = true;
		}
		else { // with leading pound, is in current file
			map = InMap;
			*pfile = NULL;
		}
		if (pelemid) {
			if (UMXL || map) {  // no topic id used, must be elem id
				*pelemid = *ptopicid;
				*ptopicid = NULL;
			}
			else if (slash)   // only in topic files
				*pelemid = slash + 1;
		}
	}
	else {  // first topic in named file
		*pfile = NewName(href);
		*ptopicid = NULL;
		if (pelemid)
			*pelemid = NULL;
	}
}


// determine relative path for href

// PathRelativePathTo() in Shlwapi.dll, defined in Shlwapi.h and Shlwapi.lib
// PathCommonPrefix()        ""
// _fullpath() is abs path for rel path from curr dir
bool XMLrd::UseFullPath = false;


char *XMLrd::FixReference(const char *hrefattr)
{
	if (!hrefattr)
		return NULL;

	char nref[MAX_PATH];

	// determine if reference is an absolute path or URI
	// if so, use it as it is.
	if (!PathIsRelative(hrefattr)
	 || PathIsURL(hrefattr))
		return NewName(hrefattr);


	char *dclpath = DCLFile->getBasePath();
	if (!*dclpath)
		dclpath = "./";
	if (_fullpath(nref, dclpath, MAX_PATH))
		dclpath = NewName(nref);
	else
		return NewName(hrefattr);

	// dclpath is the full path to the file that will contain the ref

	char *xmlpath = XMLFile->getBasePath();
	if (!*xmlpath)
		xmlpath = "./";
	if (_fullpath(nref, xmlpath, MAX_PATH))
		xmlpath = NewName(nref);
	else
		return NewName(hrefattr);

	// xmlpath is the full path to the file we are reading the ref from
	char *hrefpath = NULL;
	if (*hrefattr == '#') {
		PathCombine(nref, xmlpath, XMLFile->getFileName());
		hrefpath = NewName(nref, strlen(nref) + strlen(hrefattr));
		strcat(hrefpath, hrefattr);
	}
	else {
		PathCombine(nref, xmlpath, hrefattr);
		hrefpath = NewName(nref);
	}

	DeleteName(xmlpath);

	if (_fullpath(nref, hrefpath, MAX_PATH)) {
		DeleteName(hrefpath);
		hrefpath = NewName(nref);
	}
	else
		return NewName(hrefattr);

	// hrefpath is now the full path to the referenced item

	char *hrp = NULL;

	if (UseFullPath) {
		for (hrp = hrefpath; *hrp; hrp++) {
			if (*hrp == '\\')
				*hrp = '/';
		}

		return hrefpath;
	}

	if (PathRelativePathTo(nref, dclpath, FILE_ATTRIBUTE_DIRECTORY,
	     hrefpath, FILE_ATTRIBUTE_NORMAL)) {
		DeleteName(dclpath);
		DeleteName(hrefpath);
	}
	else
		return NewName(hrefattr);

	// nref is now the path to the referenced item from the DCL file

	for (hrp = nref; *hrp; hrp++) {
		if (*hrp == '\\')
			*hrp = '/';
	}

	// see if this is a path to another doc we are processing
	// if so, we need to replace it with an internal reference ****

	return NewName(nref);
}


bool 
XMLrd::MatchToken(char *ntok, char *nend, char *elem, long elen)
{
	long len = nend - ntok;  // count of chars between <>s

	if (strnicmp(ntok, elem, elen))
		return false;

	if (len == elen)
		return true;

	if ((len == (elen + 1))
	 && (ntok[elen] == '/'))
		return true;

	return false;
}



// numbering

char *XMLrd::WordTemplate;       //  required to furnish RTF formatting info
bool XMLrd::WriteAnums = true;   // write anum prefixes to text paras
bool XMLrd::SeqAnums = false;    // write the format prefix info too.
long XMLrd::AnumTabWidth = 18;   // tab spacing in pts, default 1/4"
//char **XMLrd::CurrNumProps = NULL;
//char *XMLrd::CurrNumForm = NULL;
char *XMLrd::CurrNumName = NULL;
char *XMLrd::CurrNumRefName = NULL;
DCgroup *XMLrd::CurrAnumGroup = NULL;

DCnlist XMLrd::NumStreamNames;  // stream name indexed by stream num
DCvlist XMLrd::NumStreams;      // numstrinfo struct indexed by stream num
long XMLrd::NumStreamIDs = 2;   // count of numstreams, after footnote streams

DCnlist XMLrd::NumFormatNames;  // format name indexed by numberID (not streamnum)
DCvlist XMLrd::NumFormInfos;    // numforminfo struct indexed by numberID
long XMLrd::NumberIDs;          // count of numberIDs (basically format numbers)
DCnlist XMLrd::NumFormatText;  // format text indexed by numberID (not streamnum)


// process all autonumbers after conrefs

DClist XMLrd::AnumElems;     // list of elems with anums in processing order
DClist *XMLrd::AnumElemList = &AnumElems; // pointer to above list
//DCelem *CurrAnumElem = NULL; // last element in curr topic to have anum
bool XMLrd::SetAnumTabs = false;


void 
XMLrd::ProcessAnums(void)
{
	DCelem *elem = NULL;

	while ((elem = (DCelem *) AnumElems.dequeue()) != NULL) {
		// write content to anumtxt group, already in text stream
		if ((((long) elem > ConrefT2TCount)
		  || ((long) elem < -ConrefT2TCount))
		 && ((elem->eltyp != estopic)
		  || !RemovedPages.find(((DCtopelem *) elem)->topic->pagenum)))
			WriteANum(elem);
	}
#ifdef UDOC
	//if (UMXL)
		XMLud::WriteCodedocrefAnums();
#endif
}



// process [NumberProperties] and [NumberFormats] for formats mentioned

bool 
XMLrd::IsNumbered(const char *name)
{
	if (!WriteAnums)
		return false;

#if 0
	bool found = false;
	CurrNumProps = NULL;
	CurrNumForm = NULL;

	if (OldNumbering) {
		if (IniFile->Section("NumberProperties")
		 && IniFile->Find(name)) {
			CurrNumProps = IniFile->StrArrRight();
			found = true;
		}
		if (IniFile->Section("NumberFormats")
		 && IniFile->Find(name)) {
			CurrNumForm = IniFile->StrRight();
			found = true;
		}
		if (found) {
			if (!CurrNumProps) {
				LogEvent(logwarn, 1, "No [NumberProperties] for ", name);
				return false;
			}
			else if (!CurrNumForm) {
				LogEvent(logwarn, 1, "No [NumberFormats] for ", name);
				return false;
			}
		}
		return found;
	}

	if (FmtFile->GetSection(name)) {
		if (FmtFile->Find("number"))
			CurrNumName = FmtFile->StrRight();
		if (FmtFile->Find("numref"))
			CurrNumRefName = FmtFile->StrRight();
	}
#endif

	CurrNumName = FmtFile->GetNumFmtName(name);
	CurrNumRefName = FmtFile->GetNumRefFmtName(name);

	if (!SubFormatsFile
	 || !CurrNumName
	 || !stricmp(CurrNumName, "none"))
		return false;

	if (SubFormatsFile->GetSection(CurrNumName))
		return true;

	return false;
}


void 
XMLrd::WriteTextNumPrefix(DCelem *elem, const char *oname, bool ftn)
{
	if (!WriteAnums)
		return;

	DCctl *cp = NULL;
	char *name = NULL;
	numforminfo *nf = NULL;
	long numform = 0;

#if 0
	if (OldNumbering) {
		if (IniFile->Section("NumberUsage")
		 && IniFile->Find(oname))
			name = IniFile->StrRight();
		else
			name = NewName(oname);
	}
	else 
#endif

	// for each elem, determine number format from einfo->elform
	if (IsNumbered(oname))
		name = CurrNumName;

	// write immediately after format call and ID:
  //((text obj anumber)
  // (text anum stream = 6)    from numprops
  // (text anum sequence = 1)    "      "
  // (text anum_set 0 = 1)  or   "      "
  // (text anum level = 1)       "      "
  // ((text anum value)
  //  (text obj lit "1.")      generate based on numform
  //  (text tab_pos left = 25000) or more likely
  //  (text tab_pos unknown = 0)
  // )
  //)

	if (!name
	 || ((nf = GetNumFormat(name)) == NULL))
		return;  // not numbered

	DCgroup *gp = new DCgroup(DCctl(group, 2, 10, 3));
	elem->anumtxt = gp;
	elem->elemnf = nf;
	if (nf->atend)
		CurrAnumGroup = gp;
	else if (!ftn)  // suppress text output for footnotes
		TextGroup->add(*gp);

	AnumElemList->append((long) elem);
}


void 
XMLrd::WriteANum(DCelem *elem)
{
	DCctl *cp = NULL;
	DCgroup *gp = elem->anumtxt;
	numforminfo *nf = (numforminfo *) elem->elemnf;
	numstrinfo *sf = NULL;
	long numform = 0;
	long i = 0;

	if (!nf) {
		char *form = FmtFile->GetNumFmtName(elem->elform);
		if (!strcmp(form, "none"))
			return;
		elem->elemnf = nf = GetNumFormat(form);
		if (!nf) {
			LogEvent(logwarn, 1, "Missing number format for ", elem->elform);
			return;
		}
	}

	if (nf->streamid) {
		cp = new DCctl(ilong, 2, 101, 3, nf->streamid);
		gp->add(*cp);

		sf = (numstrinfo *) NumStreams.find(nf->streamid);
		if (nf->level > sf->levels)
			sf->levels = nf->level;
		if (!sf->levtyps->find(nf->level))
			sf->levtyps->add(nf->typ, nf->level);
		if (!sf->levseqs->find(nf->level))
			sf->levseqs->add((nf->start == -1) ? 0 : nf->start, nf->level);
		if (!sf->levstrts->find(nf->level))
			sf->levstrts->add(nf->start, nf->level);

		long lseq = sf->levseqs->find(nf->level);
		if (nf->start != -1)
			sf->levseqs->replace(lseq = nf->start, nf->level);
		else if (lseq == 0)
			sf->levseqs->replace(lseq = 1, nf->level);
		else // increment seq for this level
			sf->levseqs->replace(++lseq, nf->level);

		if (nf->level < sf->levels) {
			for (i = nf->level + 1; i <= sf->levels; i++) {
				if (!nf->keep.find(i))
					sf->levseqs->replace(0, i);
			}
		}

		//elem->levseqs = new DCilist(sf->levseqs);
		//elem->anumlev = nf->level;

		cp = new DCctl(ilong, 2, 101, 6, lseq);
		gp->add(*cp);

		if (nf->start == -1)
			cp = new DCctl(ilong, 2, 101, 7, nf->level);
		else
			cp = new DCctl(ilong, 2, 103, (uns) nf->start, nf->level);
		gp->add(*cp);
		for (i = nf->level + 1; i <= sf->levels; i++) {
			if (!nf->keep.find(i)) {
				cp = new DCctl(ilong, 2, 103, 0, i);
				gp->add(*cp);
			}
		}
	}

	// parse format string and write text with values for tokens
	DCgroup *tp = new DCgroup(DCctl(group, 2, 101, 8));
	gp->add(*tp);
	DCgroup *lasttext = TextGroup;
	TextGroup = tp;
	DCgroup *lasttitle = TitleGroup;
	TitleGroup = NULL; // avoid doubling typo settings

	DCgroup *np = NULL;
	DCgroup *ap = NULL;
	if (UMXL
	 || elem->xrsrc
	 || (elem->parent
	  && elem->parent->xrsrc)) {  // store for potential use in an xref
		elem->elemnref = np = new DCgroup;
		elem->elemaref = ap = new DCgroup;
	}

	WriteTextNumPrefixForm(np, ap, nf, sf);
	
	TextGroup = lasttext;
	TitleGroup = lasttitle;
}


numforminfo *
XMLrd::GetNumFormat(char *name)
{
	long id = 0;
	numforminfo *nf = NULL;
	numstrinfo *sf = NULL;

	if (!name)
		return NULL;

	if (!strcmp(name, "none"))
		return NULL;

	if (((id = NumFormatNames.getitem(name)) != 0)
	 && ((nf = (numforminfo *) NumFormInfos.find(id)) != NULL))
		return nf;

#if 0
	if (OldNumbering) {
		if (!IsNumbered(name))
			return NULL;

		// set up structures
		nf = new numforminfo;
		nf->name = name;
		if (!id)
			id = ++NumberIDs;
		nf->formid = id;

		NumFormatNames.add(name, id);
		NumFormInfos.add(nf, id);

		// get text name to use for this format if any
		if (LangFile
		 && LangFile->Section("NumberFormatsText")
		 && LangFile->Find(name))
			nf->lname = LangFile->StrRight();

		nf->start = -1;
		char *strnum = NULL;

		nf->form = CurrNumForm;

		if (CurrNumProps) {
			while (*CurrNumProps) {
				if (!_strnicmp(*CurrNumProps, "stream", 6))
					nf->streamid = atol(strnum = (*CurrNumProps + 6)) + 2;
				else if (!_strnicmp(*CurrNumProps, "counter", 7))
					nf->level = atol(*CurrNumProps + 7);
				else if (!_strnicmp(*CurrNumProps, "start", 5))
					nf->start = atol(*CurrNumProps + 5);
				else if (!_strnicmp(*CurrNumProps, "keep", 4))
					nf->keep.add(atol(*CurrNumProps + 4));
				else if (!stricmp(*CurrNumProps, "xrefprefix"))
					nf->xrefpx = true;
				else if (!stricmp(*CurrNumProps, "xreftabs"))
					nf->xreftab = true;
				else if (!stricmp(*CurrNumProps, "xrefsuffix"))
					nf->xrefsx = true;
				else if (!stricmp(*CurrNumProps, "atend"))
					nf->atend = true;
				else if (!stricmp(*CurrNumProps, "num"))
					nf->typ = ntNum;
				else if (!stricmp(*CurrNumProps, "ucalpha"))
					nf->typ = ntUCAlpha;
				else if (!stricmp(*CurrNumProps, "lcalpha"))
					nf->typ = ntLCAlpha;
				else if (!stricmp(*CurrNumProps, "ucrom"))
					nf->typ = ntUCRom;
				else if (!stricmp(*CurrNumProps, "lcrom"))
					nf->typ = ntLCRom;
				CurrNumProps++;
			}
		}

		if ((sf = (numstrinfo *) NumStreams.find(nf->streamid)) == NULL) {
			sf = new numstrinfo;
			NumStreamIDs++;
			NumStreams.add(sf, nf->streamid);
			sf->streamid = nf->streamid;
			sf->levels = nf->level;
			sf->levtyps = new DCilist;
			sf->levtyps->add(nf->typ, nf->level);
			sf->levseqs = new DCilist;
			sf->levseqs->add((nf->start == -1) ? 0 : nf->start, nf->level);
			if (IniFile->Section("NumberStreams")
			 && IniFile->Find(strnum))
				sf->seqid = IniFile->StrRight();
		}
		else {
			if (nf->level > sf->levels)
				sf->levels = nf->level;
			if (!sf->levtyps->find(nf->level))
				sf->levtyps->add(nf->typ, nf->level);
			if (!sf->levseqs->find(nf->level))
				sf->levseqs->add((nf->start == -1) ? 0 : nf->start, nf->level);
		}

		return nf;
	}
#endif

	// new method

	DCfrfile::NumBasedUsed.empty();

	if (!strcmp(name, "none"))
		return NULL;

	if ((nf = SubFormatsFile->GetNumFormat(name)) == NULL) {
		LogEvent(logwarn, 1, "Undefined number format: ", name);
		return NULL;
	}

	if (!id)
		id = ++NumberIDs;
	nf->formid = id;
	NumFormatNames.add(name, id);
	NumFormInfos.add(nf, id);

	//CurrNumForm = nf->form;
	GetNumFormatNameText(nf, name, id);

	if (!stricmp(nf->sname, "none"))  // bulleted format
		return nf;

	// get stream for number format
	if ((sf = GetNumStream(nf->sname)) != NULL) { // always succeeds
		nf->streamid = sf->streamid;
		if (nf->level > sf->levels) {
			LogEvent(logwarn, 1, "Counter for ", name,
													 " too high for ", nf->sname);
			sf->levels = nf->level;
		}
		if ((nf->typ = (nlevtyp) sf->levtyps->find(nf->level)) == 0) {
			nlevtyp typ = (nlevtyp) ((sf->rep && (sf->tcount > 1))
							? sf->levtyps->find(((nf->level - 1) % sf->tcount) + 1)
							: sf->levtyps->find(sf->tcount));
			sf->levtyps->add(nf->typ = typ, nf->level);
		}
		if (nf->start != -1)
			sf->levseqs->replace(nf->start, nf->level);
	}

	return nf;
}


void 
XMLrd::GetNumFormatNameText(numforminfo *nf, char *name, long id)
{
	numforminfo *nfmt = nf;

	// override text name to use in this format
	while (name) {
		if (IniFile
		 && IniFile->Section("NumberFormatsText")
		 && IniFile->Find(name)) {
			nf->lname = IniFile->StrRight();
			break;
		}
		else if (LangFile
		 && LangFile->Section("NumberFormatsText")
		 && LangFile->Find(name)) {
			nf->lname = LangFile->StrRight();
			break;
		}
		if (nfmt->based) {
			name = nfmt->based;
			nfmt = SubFormatsFile->GetNumFormat(name);
			if (!nf->lname
			 && nfmt->lname)
				nf->lname = nfmt->lname;
		}
		else
			break;
	}

	// store result
	if (nf->lname)
		NumFormatText.add(nf->lname, id);
}



char **XMLrd::DefNumSyms = NULL;
long XMLrd::DefNumSymCount = 0;


numstrinfo *
XMLrd::GetNumStream(char *strname)
{
	long id = 0;
	numstrinfo *sf = NULL;
	nlevtyp typ = ntNum;

	if (((id = NumStreamNames(strname)) != 0)
	 && ((sf = (numstrinfo *) NumStreams.find(id)) != NULL))
		return sf;

	DCfrfile::NumStrBasedUsed.empty();

	if ((sf = SubFormatsFile->GetNumStream(strname)) == NULL) {
		LogEvent(logwarn, 1, "Undefined number stream: ", strname);
		sf = new numstrinfo;
		sf->name = strname;
		sf->seqid = "X";
		sf->levels = 1;
		sf->levseqs = new DCilist;
		sf->levstrts = new DCilist;
		sf->levtyps = new DCilist;
		sf->levtyps->add(ntNum, 1);
	}

	if (!id)
		id = ++NumStreamIDs;
	sf->streamid = id;
	NumStreams.add(sf, id);
	NumStreamNames.add(strname, id);

	if (!sf->syms) {
		if (!DefNumSyms) {
			DefNumSyms = (char **) new char[sizeof(char *) * 2];
			DefNumSyms[0] = NewName("*");
			DefNumSyms[1] = NULL;
			DefNumSymCount = 1;
		}
		sf->syms = DefNumSyms;
		sf->scount = DefNumSymCount;
	}

	return sf;
}


void 
XMLrd::ResetStreams(resettyp reset)
{
	if (reset == rtNone)
		return;

	// if a stream's sf->reset prop matches reset,
	// set sf->levseqs for each level to start value

	// iterate list of sf's for those with matching reset,
	// and reset all their levels based on start set by num

	long start = 0;
	numstrinfo *sf = NULL;

	for (long id = 1; id <= NumStreamIDs; id++) {
		sf = (numstrinfo *) NumStreams.find(id);
		if (sf
		 && (sf->reset == reset)) {
			for (long lev = 1; lev <= sf->levels; lev++) {
				start = sf->levstrts->find(lev);
				sf->levseqs->replace((start == -1) ? 0 : start, lev);
			}
		}
	}

#if 0
	// if a footnote sf->reset prop matches reset,
	// set sf->levseqs for nf->level to nf->start
	// note this only works for default ftn formats ****
	char *form = FmtFile->GetNumFmtName(FootnoteFormat);
	numforminfo *nf = GetNumFormat(form);
	numstrinfo *sf = nf ? (numstrinfo *) NumStreams.find(nf->streamid) : NULL;
	if (sf
	 && (sf->reset == reset))
		sf->levseqs->replace(nf->start, nf->level);

	form = FmtFile->GetNumFmtName(TblFootFormat);
	nf = GetNumFormat(form);
	sf = nf ? (numstrinfo *) NumStreams.find(nf->streamid) : NULL;
	if (sf
	 && (sf->reset == reset))
		sf->levseqs->replace(nf->start, nf->level);
#endif
}


void 
XMLrd::WriteTextNumPrefixForm(DCgroup *np, DCgroup *ap,
                              numforminfo *nf, numstrinfo *sf)
{
	long tlnum = 0;  // token level num
	long tlseq = 1;  // current seq for that token
	nlevtyp tltyp = ntNum;
	long atabw = AnumTabWidth * 100;
	char *fcp = nf->form;
	char *ntok = NULL;
	DCctl *cp = NULL;
	char formname[FormNameMax] = "";
	char *fnend = NULL;
	bool typo = false;
	DCnlist *nestlist = NULL;
	DCilist *typolist = NULL;
	long fnest = 0;
	bool keepap = nf->xrefpx;
	bool keepnp = false;

	// get count of counters
	long ccnt = 0;
	char *fpt = fcp;
	while ((fpt = strstr(fpt, "<counter")) != NULL)
		ccnt++, fpt++;
	long cnum = 0;

	while ((ntok = strchr(fcp, '<')) != NULL) {
		if (ntok > fcp) {
			if ((ntok - fcp) > 1) {
				cp = new DCctl(etext, 2, 10, 1);
				cp->ex(fcp, ntok - fcp);
			}
			else
				cp = new DCctl(imnem, 2, 10, 1, (unc) *fcp);
			TextGroup->add(*cp);
			if (ap && keepap)
				ap->add(*cp);
			if (np && keepnp)
				np->add(*cp);
		}
		fcp = ntok;
		while (*fcp
		 && (*fcp != '>'))
			fcp++;
		if (!*fcp)
			LogEvent(logwarn, 1, "Number format tag missing ending '>': ", ntok);
		ntok++;
		if (MatchToken(ntok, fcp, "tab", 3)) {  // tab token
			cp = new DCctl(ilong, 2, 41, 1, tlnum ? (atabw * tlnum) : atabw);
			TextGroup->add(*cp);
			if (ap && nf->xreftab) {
				cp = new DCctl(imnem, 2, 10, 1, (unl) ' ');
				ap->add(*cp);
			}
		}
		else if (MatchToken(ntok, fcp, "spc", 3)) {
			cp = new DCctl(imnem, 2, 10, 1, (unl) 160);
			TextGroup->add(*cp);
			if (ap && keepap)
				ap->add(*cp);
			if (np && keepnp)
				np->add(*cp);
		}
		else if (MatchToken(ntok, fcp, "br", 2)) {
			cp = new DCctl(imnem, 2, 11, 10, 1);
			TextGroup->add(*cp);
			if (ap && keepap)
				ap->add(*cp);
			if (np && keepnp)
				np->add(*cp);
		}
		else if (!strnicmp(ntok, "counter", 7)
		 && ((fcp == ntok + 7)
		  || (*(fcp - 1) == '/')
		  || isdigit(*(fcp - 1)))) {
			if (ccnt
			 && (++cnum == ccnt)) {
				keepap = nf->xrefsx;  // after last counter is suffix
				keepnp = false;
			}
			else {
				keepap = true;
				keepnp = true;        // keep text between counters for numonly
			}
			tlnum = 0;
			if ((fcp == (ntok + 7))
			 || ((fcp == (ntok + 8))
			  && (*(fcp - 1) == '/'))) // no number, treat as 1
				tlnum = 1;
			else if (isdigit(*(ntok + 7)))  // get level number
				tlnum = (long) strtoul(ntok + 7, &fnend, 10);
			if (sf
			 && tlnum) {
				tlseq = sf->levseqs->find(tlnum);
				tltyp = (nlevtyp) sf->levtyps->find(tlnum);
				cp = SetNumVal(tltyp, tlseq, sf->syms, sf->scount);
				if (cp) {
					TextGroup->add(*cp);
					if (ap)
						ap->add(*cp);
					if (np)
						np->add(*cp);
				}
			}
			else
				LogEvent(logwarn, 1, "Number format <counter/> error: ", ntok);
		}
		else if (MatchToken(ntok, fcp, "name", 4)) {  // get name from lang
			if (nf->lname) {
				cp = new DCctl(etext, 2, 10, 1);
				cp->ex(nf->lname, strlen(nf->lname));
				TextGroup->add(*cp);
				if (ap && keepap)
					ap->add(*cp);
			}
			else
				LogEvent(logwarn, 1, "Number format <name/> undefined for ", nf->name);
		}
		else if ((tolower(*ntok) == 'u')
		 && (*(ntok + 1) == '+')) {  // Unicode code point
			char *nend = NULL;
			unl uch = strtoul(ntok + 2, &nend, 16);
			if ((nend != (fcp - 1))
			 || (*(fcp - 1) != '/'))
				LogEvent(logwarn, 1, "Unicode char error in form for ", nf->name);
			else {
				cp = new DCctl(imnem, 2, 10, 1, uch);
				TextGroup->add(*cp);
				if (ap && keepap)
					ap->add(*cp);
			}
		}
		else {  // treat anything else as an inline format
			if (*ntok != '/') {  // start the named format
				if (fnest) { // nest format
					if (!nestlist)
						nestlist = new DCnlist();
					nestlist->addnew(formname, fnest);
					if (!typolist)
						typolist = new DCilist();
					typolist->add(typo, fnest);
				}
				fnest++;
				strncpy(formname, ntok, fcp - ntok);
				formname[fcp - ntok] = '\0';
				if (SetTypographic(formname, true))
					typo = true;
				else
					SetFormat(formname, true); 
			}
			else if (!strnicmp(ntok + 1, formname, (fcp - ntok) - 1)) {
				// end any current format
				if (typo) {
					SetTypographic(formname, false);
					typo = false;
				}
				else
					SetFormat(NULL, true);
				*formname = '\0';
				if (--fnest) {
					strcpy(formname, nestlist->find(fnest));
					typo = typolist->find(fnest);
				}
			}
			else
				LogEvent(logwarn, 1, "Number <format> token error: ", ntok);
		}
		if (*fcp)  // move after ending bracket
			ntok = ++fcp;
	}
	if (*fcp) { // more left, finish it
		if (strlen(fcp) > 1) {
			cp = new DCctl(etext, 2, 10, 1);
			cp->ex(fcp, strlen(fcp));
		}
		else
			cp = new DCctl(imnem, 2, 10, 1, (unc) *fcp);
		TextGroup->add(*cp);
		if (ap && keepap)
			ap->add(*cp);
	}
	while (fnest) {
		if (typo) {
			SetTypographic(formname, false);
			typo = false;
		}
		else if (*formname != '\0')
			SetFormat(NULL, true);
		if (--fnest) {
			strcpy(formname, nestlist->find(fnest));
			typo = typolist->find(fnest);
		}
	}
}


void 
XMLrd::WriteFormatNumPrefix(const char *name, DCgroup *gp)
{
	if (!WriteAnums)
		return;

	numforminfo *nf = NULL;
	numstrinfo *sf = NULL;
	long numid = NumFormatNames.getitem(name);
	long i = 0;
	DCctl *cp = NULL;

	nf = (numforminfo *) NumFormInfos.find(numid);
	if (nf->streamid)
		sf = (numstrinfo *) NumStreams.find(nf->streamid);

	// write tab definitions for anum tabs

	if (SetAnumTabs) {
		DCgroup *tgp = new DCgroup(DCctl(group, 2, 40, 1));
		gp->add(*tgp);

		cp = new DCctl(ilong, 2, 42, 1, 32);
		tgp->add(*cp);

		long atabw = AnumTabWidth * 100;

		for (i = 1; i <= (nf->level ? nf->level : 1); i++) {
			cp = new DCctl(ilong, 2, 41, 1, (i * atabw));
			tgp->add(*cp);
		}
	}

	// write at end of formdef group; for Numbered:
  //((text form prefix)
  // ((text obj anumber)
  //  (text anum stream = 6)
  //  ((text anum form)      from numform
  //   (text anum_tok arabic 1)
  //   (text obj lit period)
  //   (text tab_pos unknown = 0)
  //  )
  //  (text anum level = 1)  or
  //  (text anum_set 1 = 1)  for Numbered1
  // )
  //)

	DCgroup *fgp = new DCgroup(DCctl(group, 2, 81, nf->atend ? 4 : 3));
	gp->add(*fgp);

	DCgroup *agp = new DCgroup(DCctl(group, 2, 10, 3));
	fgp->add(*agp);

	if (nf->streamid) {
		cp = new DCctl(ilong, 2, 101, 3, nf->streamid);
		agp->add(*cp);
	}

	DCgroup *fp = new DCgroup(DCctl(group, 2, 101, 4));
	agp->add(*fp);

	DCgroup *lasttext = TextGroup;
	TextGroup = fp;

	long tlnum = 0;  // token level num
	tlnum = AddNumFormDef(fp, nf->form, sf, numid);

	TextGroup = lasttext;

	if (sf) {
		for (i = tlnum + 1; i <= sf->levels; i++) {
			//if (nf->keep.find(i)) {
				cp = new DCctl(ilong, 2, 102, (uns) ntHide, i);
				fp->add(*cp);
			//}
		}

		if (nf->start == -1)
			cp = new DCctl(ilong, 2, 101, 7, nf->level);
		else
			cp = new DCctl(ilong, 2, 103, (uns) nf->start, nf->level);
		agp->add(*cp);
		for (i = tlnum + 1; i <= sf->levels; i++) {
			if (!nf->keep.find(i)) {
				cp = new DCctl(ilong, 2, 103, 0, i);
				agp->add(*cp);
			}
		}
	}
}


long 
XMLrd::AddNumFormDef(DCgroup *fp, char *fcp, numstrinfo *sf,
	                   long numid, bool ftn)
{
	long tlnum = 0;  // token level num
	DCctl *cp = NULL;
	char *ntok = NULL;
	nlevtyp ntype = ntNum;
	char formname[FormNameMax] = "";
	char *fnend = NULL;
	long fnum = 0;
	bool typo = false;
	DCnlist *nestlist = NULL;
	DCilist *typolist = NULL;
	long fnest = 0;

	while ((ntok = strchr(fcp, '<')) != NULL) {
		if (ntok > fcp) {
			if ((ntok - fcp) > 1) {
				cp = new DCctl(etext, 2, 10, 1);
				cp->ex(fcp, ntok - fcp);
			}
			else
				cp = new DCctl(imnem, 2, 10, 1, (unc) *fcp);
			fp->add(*cp);
		}
		fcp = ntok;
		while (*fcp
		 && (*fcp != '>'))
			fcp++;
		if (!*fcp)
			LogEvent(logwarn, 1, "Number format tag missing ending '>': ", ntok);
		ntok++;
		if (MatchToken(ntok, fcp, "tab", 3)) {  // tab token
			cp = new DCctl(ilong, 2, 41, 8);
			fp->add(*cp);
		}
		else if (MatchToken(ntok, fcp, "spc", 3)) {
			cp = new DCctl(imnem, 2, 10, 1, (unl) 160);
			fp->add(*cp);
		}
		else if (MatchToken(ntok, fcp, "br", 2)) {
			cp = new DCctl(imnem, 2, 11, 10, 1);
			fp->add(*cp);
		}
		else if (!strnicmp(ntok, "counter", 7)
		 && ((fcp == ntok + 7)
		  || (*(fcp - 1) == '/')
		  || isdigit(*(fcp - 1)))) {
			tlnum = 0;
			if ((fcp == (ntok + 7))
			 || ((fcp == (ntok + 8))
			  && (*(fcp - 1) == '/'))) // no number, treat as 1
				tlnum = 1;
			else if (isdigit(*(ntok + 7)))  // get level number
				tlnum = (long) strtoul(ntok + 7, &fnend, 10);
			if (sf
			 && tlnum) {
				ntype = (nlevtyp) sf->levtyps->find(tlnum);
				cp = new DCctl(ilong, 2, 102, (uns) ntype, tlnum);
				fp->add(*cp);
			}
			else
				LogEvent(logwarn, 1, "Number format <counter/> error: ", ntok);
		}
		else if (MatchToken(ntok, fcp, "name", 4)) {  // get name from lang
			char *txt = NumFormatText.find(numid);
			if (txt) {
				cp = new DCctl(etext, 2, 10, 1);
				cp->ex(txt, strlen(txt));
				fp->add(*cp);
			}
			else {
				char *name = NumFormatNames.find(numid);
				LogEvent(logwarn, 1, "Number format <name/> undefined for ", name);
			}
		}
		else if ((tolower(*ntok) == 'u')
		 && (*(ntok + 1) == '+')) {  // Unicode code point
			char *nend = NULL;
			unl uch = strtoul(ntok + 2, &nend, 16);
			if ((nend != (fcp - 1))
			 || (*(fcp - 1) != '/'))
				LogEvent(logwarn, 1, "Unicode char error in form for ",
				                     NumFormatNames.find(numid));
			else {
				cp = new DCctl(imnem, 2, 10, 1, uch);
				fp->add(*cp);
			}
		}
		else {  // treat anything else as an inline format
			if (*ntok != '/') {  // start the named format
				if (fnest) { // nest format
					if (!nestlist)
						nestlist = new DCnlist();
					nestlist->addnew(formname, fnest);
					if (!typolist)
						typolist = new DCilist();
					typolist->add(typo, fnest);
				}
				fnest++;
				strncpy(formname, ntok, fcp - ntok);
				formname[fcp - ntok] = '\0';
				if ((cp = SetTypographic(formname, true)) != NULL)
					typo = true;
				else {
					if ((fnum = InlineFormatList.getitem(formname)) == 0)
						fnum = StoreFormat(formname, true);
					cp = new DCctl(imnem, 2, 82, 2, fnum);
					InlineFormats.add(cp, ++InlineFormUseCount);
					fp->add(*cp);
				}
			}
			else if (!strnicmp(ntok + 1, formname, (fcp - ntok) - 1)) {
				// end any current format
				if (typo) {
					cp = SetTypographic(formname, false);
					typo = false;
				}
				else {
					cp = new DCctl(imnem, 2, 82, 2, 0);
					fp->add(*cp);
				}
				*formname = '\0';
				if (--fnest) {
					strcpy(formname, nestlist->find(fnest));
					typo = typolist->find(fnest);
				}
			}
			else
				LogEvent(logwarn, 1, "Number <format> token error: ", ntok);
		}
		if (*fcp)  // move after ending bracket
			ntok = ++fcp;
	}
	if (*fcp) { // more left, finish it
		if (strlen(fcp) > 1) {
			cp = new DCctl(etext, 2, 10, 1);
			cp->ex(fcp, strlen(fcp));
		}
		else
			cp = new DCctl(imnem, 2, 10, 1, (unc) *fcp);
		fp->add(*cp);
	}

	if (!ftn) {  // leave footnote format open
		while (fnest) {
			if (typo) {
				cp = SetTypographic(formname, false);
				typo = false;
			}
			else if (*formname != '\0') {
				cp = new DCctl(imnem, 2, 82, 2, 0);
				fp->add(*cp);
			}
			if (--fnest) {
				strcpy(formname, nestlist->find(fnest));
				typo = typolist->find(fnest);
			}
		}
	}

	return tlnum;
}


void 
XMLrd::WriteFormatStreamDefs(DCgroup *gp)
{
	if (!WriteAnums)
		return;

  // include streams at start of resources:
  //((text anum_def 6)
  // (text anum name 'N')  from numprops
  // (text anum level = 1)   "      "
  //)

	numstrinfo *sf = NULL;
	DCgroup *fp = NULL;
	DCctl *cp = NULL;
	long cnt = NumStreamIDs - 2;
	long stream = 3;  // 1 and 2 reserved for footnotes

	while (cnt) {
		sf = (numstrinfo *) NumStreams.find(stream++);
		if (!sf)
			continue;
		cnt--;
		fp = new DCgroup(DCctl(group, 2, 100, (uns) sf->streamid));
		gp->add(*fp);

		cp = new DCctl(ename, 2, 101, 1);
		cp->ex(sf->seqid, strlen(sf->seqid));
		fp->add(*cp);

		cp = new DCctl(ilong, 2, 101, 7, sf->levels);
		fp->add(*cp);
	}
}



// numbering utilities


DCgroup *
XMLrd::SetNumVal(nlevtyp ntyp, long nval, char **syms, long scount)
{
	char *nstr = NULL;
	char *tstr = NULL;
	char *tp = NULL;

	switch (ntyp) {
		case ntNum:  //arabic
		default:
			nstr = NewName(NumLenMax);
			sprintf(nstr, "%ld", nval);
			break;

		case ntLCRom:  // lc Roman
			tstr = NewName(RomanMax);
			tp = tstr;
			while (nval >= 1000) {
				*tp++ = 'm';
				nval -= 1000;
			}
			if (nval >= 900) {
				*tp++ = 'c';
				*tp++ = 'm';
				nval -= 900;
			}
			while (nval >= 100) {
				*tp++ = 'c';
				nval -= 100;
			}
			if (nval >= 50) {
				*tp++ = 'l';
				nval -= 50;
			}
			if (nval >= 40) {
				*tp++ = 'x';
				*tp++ = 'l';
				nval -= 40;
			}
			while (nval >= 10) {
				*tp++ = 'x';
				nval -= 10;
			}
			if (nval >= 9) {
				*tp++ = 'i';
				*tp++ = 'x';
				nval -= 9;
			}
			if (nval >= 5) {
				*tp++ = 'v';
				nval -= 5;
			}
			if (nval >= 4) {
				*tp++ = 'i';
				*tp++ = 'v';
				nval -= 4;
			}
			while (nval >= 1) {
				*tp++ = 'i';
				nval--;
			}
			*tp = '\0';
			nstr = NewName(tstr);
			DeleteName(tstr);
			break;

		case ntUCRom:  // uc Roman
			tstr = NewName(RomanMax);
			tp = tstr;
			while (nval >= 1000) {
				*tp++ = 'M';
				nval -= 1000;
			}
			if (nval >= 900) {
				*tp++ = 'C';
				*tp++ = 'M';
				nval -= 900;
			}
			while (nval >= 100) {
				*tp++ = 'C';
				nval -= 100;
			}
			if (nval >= 50) {
				*tp++ = 'L';
				nval -= 50;
			}
			if (nval >= 40) {
				*tp++ = 'X';
				*tp++ = 'L';
				nval -= 40;
			}
			while (nval >= 10) {
				*tp++ = 'X';
				nval -= 10;
			}
			if (nval >= 9) {
				*tp++ = 'I';
				*tp++ = 'X';
				nval -= 9;
			}
			if (nval >= 5) {
				*tp++ = 'V';
				nval -= 5;
			}
			if (nval >= 4) {
				*tp++ = 'I';
				*tp++ = 'V';
				nval -= 4;
			}
			while (nval >= 1) {
				*tp++ = 'I';
				nval--;
			}
			*tp = '\0';
			nstr = NewName(tstr);
			DeleteName(tstr);
			break;

		case ntLCAlpha:  // lc alpha
			if (nval) {
				nstr = NewName(AlphaMax);
				if (nval < 27)
					*nstr = 'a' + (unc) ((nval - 1) % 26);
				else if (nval < 676) {
					*nstr++ = 'a' + (unc) ((nval / 26) - 1);
					*nstr = 'a' + (unc) ((nval - 1) % 26);
				}
				else if (nval < (676 * 26)) {
					*nstr++ = 'a' + (unc) ((nval / 676) - 1);
					*nstr++ = 'a' + (unc) (((nval / 26) - 1) % 26);
					*nstr = 'a' + (unc) ((nval - 1) % 26);
				}
			}
			break;

		case ntUCAlpha:  // uc alpha
			if (nval) {
				nstr = NewName(AlphaMax);
				if (nval < 27)
					*nstr = 'A' + (unc) ((nval - 1) % 26);
				else if (nval < 676) {
					*nstr++ = 'A' + (unc) ((nval / 26) - 1);
					*nstr = 'A' + (unc) ((nval - 1) % 26);
				}
				else if (nval < (676 * 26)) {
					*nstr++ = 'A' + (unc) ((nval / 676) - 1);
					*nstr++ = 'A' + (unc) (((nval / 26) - 1) % 26);
					*nstr = 'A' + (unc) ((nval - 1) % 26);
				}
			}
			break;

		case ntSym:  // symbols
			if (!nval)
				break;
			if (!syms || !*syms) {
				syms = DefNumSyms;
				scount = DefNumSymCount;
			}
			long cnt = ((nval - 1) / scount) + 1;
			char *str = syms[(nval - 1) % scount];
			long slen = strlen(str);
			nstr = NewName((short) (cnt * slen));
			for (long i = 0; i < cnt; i++)
				strcat(nstr, str);
			break;
	}
	if (!nstr)
		return NULL;

	DCgroup *gp = PutUniTextCtls(nstr);
	return gp;
}


DCgroup *
XMLrd::PutUniTextCtls(char *str)
{
	if (!str || !*str)
		return NULL;
	DCgroup *gp = new DCgroup;
	DCgroup *lasttext = TextGroup;
	DCgroup *lasttitle = TitleGroup;
	TextGroup = gp;
	TitleGroup = NULL;
	char *chp = str;
	unl val = 0;
	while (*chp) {
		if ((*chp == '<')
		 && (tolower(*(chp + 1)) == 'u')
		 && (*(chp + 2) == '+')) {
			char *ep = NULL;
			val = strtoul(chp + 3, &ep, 16);
			if (val) {
				if (chp > str)
					PutTextCtrls(str, chp - str);
				PutTextCharCtl(val);
				if (ep) {
					if (*ep == '/')
						ep++;
					if (*ep == '>')
						ep++;
					chp = str = ep;
				}
			}
		}
		chp++;
	}
	if (chp > str)
		PutTextCtrls(str, chp - str);
	TextGroup = lasttext;
	TitleGroup = lasttitle;
	return gp;
}


// footnote processing


bool XMLrd::InFootnote = false;
bool XMLrd::CurrFtnHasID = false;
bool XMLrd::UseFtnCalloutAttribute = false;
DCgroup *XMLrd::PreFootGroup = NULL;
DCgroup *XMLrd::PreFootTitleGroup = NULL;
char *XMLrd::CurrFtnForm = NULL;
char *XMLrd::CurrFtnCallout = NULL;
//char **XMLrd::FootnoteSymbols = NULL;
//long XMLrd::FootnoteSymbolCount = 0;

char *XMLrd::FootnoteFormat = "Footnote";
//char *XMLrd::FootnoteNums[] = { "Num", "Topic", NULL };
//char **XMLrd::FootnoteNumbering = (char **) &FootnoteNums;
//char *XMLrd::FootnoteNumForm = "<counter/>.<tab/>";
//char *XMLrd::FootnoteRefForm = "<sup><counter/>";
//nlevtyp XMLrd::FootnoteNtype = ntNum;
//uns XMLrd::FootnoteReset = 1;
//long XMLrd::CurrFootnoteSeq = 0;

char *XMLrd::TblFootFormat = "TblFootnote";
//char **XMLrd::TblFootNumbering = NULL;
//char *XMLrd::TblFootNumForm = "<counter/>.<tab/>";
//char *XMLrd::TblFootRefForm = "<sup><counter/>";
//nlevtyp XMLrd::TblFootNtype = ntLCAlpha;
//uns XMLrd::TblFootReset = 3;
//long XMLrd::CurrTblFootSeq = 0;


void
XMLrd::StartFootnote(DCtopelem *einfo, long depth)
{
	DCctl *cp = NULL;
	char *epath = NULL;

	InFootnote = true;
	PreFootGroup = TextGroup;
	DCgroup *gp = new DCgroup(DCctl(group, 2, 71, InTable ? 2 : 1));
#ifdef DITA
	if (einfo->id) {
		// suppress footnote, put out at first of its xrefs
		einfo->fntext = new DCgroup(); // wrap for later merge
		einfo->fntext->add(* ((DCctl *) gp));
	}
	else
#endif
		TextGroup->add(* ((DCctl *) gp));

	TextGroup = gp;
	//cp = new DCctl(group, 2, 71, InTable ? 2 : 1);
	//TextGroup->add(*cp);
	cp = new DCctl(imnem, 2, 11, 1);
	TextGroup->add(*cp);

	if (!CurrFtnForm) { // no outclass
		if (!einfo->elpath)
			einfo->elpath = GetElemPath(depth);
		if (einfo->elpath
		 && IniFile->Section("InlineFormatMaps")
		 && IniFile->FindElemPath(einfo->elpath))
			CurrFtnForm = IniFile->StrRight();
		else // or use default:
			CurrFtnForm =  InTable ? TblFootFormat : FootnoteFormat;
	}
	einfo->elform = CurrFtnForm;
	einfo->formctl = SetFormat(CurrFtnForm);  // treat as block here
	if (ShowElementPath) {
		einfo->epctl = cp = new DCctl(ename, 2, 121, 6);
		cp->ex(einfo->elpath);
		TextGroup->add(*cp);
	}
	if (UseFtnCalloutAttribute
	 && CurrFtnCallout) {
		cp = PutUniTextCtls(CurrFtnCallout);
		if (cp) {
			einfo->elemaref = new DCgroup;
			einfo->elemnref = new DCgroup;
			einfo->elemaref->add(*cp);
			einfo->elemnref->add(*cp);
		}
		CurrFtnCallout = NULL;
	}
	else
		WriteTextNumPrefix(einfo, CurrFtnForm, true);
	SetInlineElemTags();

#if 0
	// populate einfo->elemnref and elemaref with footnote number
	long *ftseq = InTable ? &CurrTblFootSeq : &CurrFootnoteSeq;
	nlevtyp ntyp = InTable ? TblFootNtype : FootnoteNtype;

	DCgroup *np = new DCgroup;
	DCgroup *ap = new DCgroup;
	einfo->elemnref = np;
	einfo->elemaref = ap;
	(*ftseq)++;
	char *nv = CurrFtnCallout;
	if (!nv)
		cp = SetNumVal(ntyp, *ftseq, FootnoteSymbols, FootnoteSymbolCount);
	else
		cp = PutUniTextCtls(nv);
	if (cp) {
		ap->add(*cp);
		np->add(*cp);
	}
	CurrFtnCallout = NULL;
#endif

	PreFootTitleGroup = TitleGroup;
	TitleGroup = einfo->elemtref = new DCgroup;
}


void
XMLrd::EndFootnote(void)
{
	DCctl *cp = NULL;

	cp = new DCctl(imnem, 2, 11, 2);
	TextGroup->add(*cp);
	//cp = new DCctl(endgroup, 2, 71, InTable ? 2 : 1);
	//TextGroup->add(*cp);
	if (PreFootGroup) {
		TextGroup = PreFootGroup;
		PreFootGroup = NULL;
	}
	TitleGroup = PreFootTitleGroup;
	InFootnote = false;
}


#if 0
; this section will be mostly gone soon
[Footnotes]
FootnoteFormat=Footnote
FootnoteNumbering=Num Topic
FootnoteNumForm= <counter/>. <tab/>
FootnoteRefForm= <sup><counter/>
FootnoteSymbols= *
TblFootFormat=Table Footnote
TblFootNumbering=LCAlpha Table
TblFootNumForm= <counter/>. <tab/>
TblFootRefForm= <sup><counter/>

void 
XMLrd::SetFootnoteInfo(bool tbl)
{
	char **numbering = tbl ? TblFootNumbering : FootnoteNumbering;
	uns nres = tbl ? 3 : 1;
	nlevtyp ntyp = tbl ? ntLCAlpha : ntNum;

	if (numbering) {
		while (*numbering) {
			if (!stricmp(*numbering, "topic"))
				nres = 1;
			else if (!stricmp(*numbering, "table"))
				nres = 3;
			else if (!stricmp(*numbering, "none"))
				nres = 0;

			else if (!stricmp(*numbering, "num"))
				ntyp = ntNum;
			else if (!stricmp(*numbering, "ucalpha"))
				ntyp = ntUCAlpha;
			else if (!stricmp(*numbering, "lcalpha"))
				ntyp = ntLCAlpha;
			else if (!stricmp(*numbering, "ucrom"))
				ntyp = ntUCRom;
			else if (!stricmp(*numbering, "lcrom"))
				ntyp = ntLCRom;
			else if (!stricmp(*numbering, "sym"))
				ntyp = ntSym;
			numbering++;
		}
	}
	if (tbl) {
		TblFootNtype = ntyp;
		TblFootReset = nres;
	}
	else {
		FootnoteNtype = ntyp;
		FootnoteReset = nres;
	}

	if (!FootnoteSymbols) {
		FootnoteSymbols = (char **) new char[sizeof(char *) * 2];
		FootnoteSymbols[0] = NewName("*");
		FootnoteSymbols[1] = NULL;
		FootnoteSymbolCount = 1;
	}
	else {
		char **syms = FootnoteSymbols;
		FootnoteSymbolCount = 0;
		while (*syms++)
			FootnoteSymbolCount++;
	}
}
#endif


void 
XMLrd::WriteFootnoteStreamDefs(DCgroup *gp, uns id, bool tbl)
{
	DCgroup *fp = NULL;
	DCgroup *fmt = NULL;
	DCctl *cp = NULL;
	numforminfo *nf = NULL;
	numstrinfo *sf = NULL;
	long numid = 0;

	char *fname = tbl ? TblFootFormat : FootnoteFormat;
	if (!fname) {
		LogEvent(logwarn, 1, "Footnote format not defined for ",
		                     (tbl ? "table" : "text"), " footnotes");
		return;
	}
	char *form = FmtFile->GetNumFmtName(fname);
	if (!form) {
		LogEvent(logwarn, 1, "Footnote number format not defined for ", fname);
		return;
	}
	char *rform = FmtFile->GetNumRefFmtName(fname);
	if (!rform) {
		LogEvent(logwarn, 1, "Footnote refnum format not defined for ", fname);
		return;
	}

	//uns nres = tbl ? TblFootReset : FootnoteReset;
	//nlevtyp ntyp = tbl ? TblFootNtype : FootnoteNtype;

	fp = new DCgroup(DCctl(group, 2, 100, id));
	gp->add(*fp);

	cp = new DCctl(imnem, 2, 101, 2, tbl ? 12 : 2);  // type
	fp->add(*cp);

	DCgroup *lasttext = TextGroup;

#if 0
	// add form used at ref to footnote to group
	form = tbl ? TblFootRefForm : FootnoteRefForm;
	if (form) {
		fmt = new DCgroup(DCctl(group, 2, 101, 4));
		fp->add(*fmt);
		TextGroup = fmt;
		if (AddNumFormDef(fmt, form, NULL, 0, ntyp) != 1)
			LogEvent(logwarn, 1, "Must have one level for footnote numbering.");
	}

	// add form used at footnote itself to group
	form = tbl ? TblFootNumForm : FootnoteNumForm;
	if (form) {
		fmt = new DCgroup(DCctl(group, 2, 101, 5));
		fp->add(*fmt);
		TextGroup = fmt;
		if (AddNumFormDef(fmt, form, NULL, 0, ntyp) != 1)
			LogEvent(logwarn, 1, "Must have one level for footnote numbering.");
	}
#endif

	// add form used at ref to footnote to group
	if (rform) {
		fmt = new DCgroup(DCctl(group, 2, 101, 4));
		fp->add(*fmt);
		TextGroup = fmt;

		if (!strcmp(form, "none"))
			return;
		nf = GetNumFormat(rform);
		if (!nf) {
			LogEvent(logwarn, 1, "Footnote refnum format ", rform, " not defined");
			return;
		}
		sf = (numstrinfo *) NumStreams.find(nf->streamid);
		numid = NumFormatNames.getitem(rform);
		AddNumFormDef(fmt, nf->form, sf, numid, true);
	}

	// add form used at footnote itself to group
	if (form) {
		fmt = new DCgroup(DCctl(group, 2, 101, 5));
		fp->add(*fmt);
		TextGroup = fmt;

		nf = GetNumFormat(form);
		if (!nf) {
			LogEvent(logwarn, 1, "Footnote number format ", form, " not defined");
			return;
		}
		sf = (numstrinfo *) NumStreams.find(nf->streamid);
		numid = NumFormatNames.getitem(form);
		AddNumFormDef(fmt, nf->form, sf, numid, true);
	}

	TextGroup = lasttext;

	cp = new DCctl(ilong, 2, 103, 1, 1);  // set level 1 to 1
	fp->add(*cp);

	cp = new DCctl(ilong, 2, 101, 10, (uns) sf->reset);  // reset rule
	fp->add(*cp);

#if 0
	if (FootnoteSymbols
	 && *FootnoteSymbols) {
		DCgroup *sym = new DCgroup(DCctl(group, 2, 101, 9));  // symbol list
		fp->add(*sym);
		char **syp = FootnoteSymbols;
		long len = 0;
		while (*syp) {
			if ((len = strlen(*syp)) > 1) {
				cp = new DCctl(etext, 2, 10, 1);
				cp->ex(*syp, len);
			}
			else
				cp = new DCctl(imnem, 2, 10, 1, (unc) **syp);
			sym->add(*cp);
			syp++;
		}
	}

	cp = new DCctl(ilong, 2, 101, 10, nres);  // reset rule
	fp->add(*cp);
#endif
}



// end of drxmlan.cpp


