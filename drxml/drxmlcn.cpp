// drxmlcn.cpp is the code for dcl XML conref processing, JHG
// Copyright (c) 2011 by Omni Systems, Inc.  All rights reserved.

#include "dcappl.h"
#include "dcctlg.h"
#include "dcfili.h"
#include "dcfilp.h"
#include "dctxfx.h"
#include "dcfmtr.h"
#include "dcini.h"
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



// common conref functions

char *XMLrd::ConrefBranch = NULL;
DCelem *XMLrd::LastConactElem = NULL;
bool XMLrd::ProcessingConrefs = false;
bool XMLrd::LastConactUsed = false;
bool XMLrd::ConrefPull = true;
bool XMLrd::ConrefsDone = false;


DCelem *
XMLrd::CheckElemConact(DCelem *elem)
{
	bool setbymark = false;

	if (elem->conact) {
		if (!strcmp(elem->conact, "mark")) {  // don't set this one
			if (LastConactElem
			 && !strcmp(elem->elemname, LastConactElem->elemname)
			 && !strcmp(LastConactElem->conact, "pushbefore")) {  // this has conref for last one
				LastConactElem->conref = elem->conref;
				elem = LastConactElem;  // set the pushbefore now
				LastConactUsed = true;
				//LastConactElem = NULL;  // leave for possible pushafter
				return elem;
			}
			else { // else this is for next one, pushafter
				LastConactElem = elem;
				LastConactUsed = false;
				return NULL;
			}
		}
		else if (!strcmp(elem->conact, "pushbefore")) {
			LastConactElem = elem;
			LastConactUsed = false;
			if (elem->conref) {
				LogEvent(loginfo, 2, "Conref pushbefore has its own @conref: ", elem->conref);
				return elem;
			}
			else
				return NULL;  // don't set it yet, wait for mark
		}
		else if (!strcmp(elem->conact, "pushafter")) {
			if (LastConactElem
			 && !strcmp(elem->elemname, LastConactElem->elemname)
			 && !strcmp(LastConactElem->conact, "mark")) {
				elem->conref = LastConactElem->conref;
				setbymark = true;
			}
			else if (!elem->conref)
				LogEvent(loginfo, 2, "Conref pushafter missing preceding mark");
			LastConactElem = NULL;
			LastConactUsed = false;
			if (elem->conref) {
				if (!setbymark)
					LogEvent(loginfo, 2, "Conref pushafter has its own @conref: ", elem->conref);
				return elem;
			}
			else
				return NULL; 
		}
		else
			return elem;
	}
	else {  // no conact here
		if (LastConactElem
		 && !strcmp(elem->elemname, LastConactElem->elemname)) {
			if (!strcmp(LastConactElem->conact, "pushbefore")
			 && !LastConactElem->conref)
				LogEvent(logerr, 1, "Conref pushbefore missing following mark");
			else if (!strcmp(LastConactElem->conact, "mark")
			 && !LastConactUsed)
				LogEvent(logerr, 1, "Conref mark missing following pushafter");
		}
		if (LastConactElem
		 && !strcmp(elem->elemname, LastConactElem->elemname)) {
			LastConactElem = NULL;
			LastConactUsed = false;
		}
		return elem;
	}
}


void 
XMLrd::SetConrefConds(DCcnref *cinfo)
{
	if (ConrefBranch) {
		cinfo->branch = ConrefBranch;
		ConrefBranch = NULL;
	}
	else
		cinfo->branch = CurrBranch;
	cinfo->conds = CurrCondSet;
}


void 
XMLrd::SetConrefPath(DCelem *elem, bool isMap)
{
	// continue to parse current content, but wrap it in group
	SetElemText(true, isMap);
	if (!elem->elpath)
		elem->elpath = GetElemPath(elem->elemdepth);
}


bool
XMLrd::FileIsMap(char *href)
{
	if (!href)
		return false;
	char *hash = strrchr(href, '#');
	if (!hash)
		hash = href + strlen(href);
	if (!strncmp(hash - 3, "map", 3))
		return true;
	return false;	
}


char *
XMLrd::GetGroupText(DCgroup *gp)
{
	if (!gp
	 || !gp->first)
		return NULL;

	char cbuf[1024];
	char *txt = cbuf;
	*txt = '\0';
	long sz = 0;
	DCctl *cp = NULL;

	for (DCitem *ci = gp->first; ci; ci = ci->next) {
		// get all text obj lit content into txt
		// ignore anything else
		cp = ci->ctl;
		if (!cp)
			continue;
		if ((cp->dt() == group)
		 && (cp->maj() == 0)
		 && (cp->min() == 0)
		 && (cp->dat() == 0)) {
			char *t2 = GetGroupText((DCgroup *) cp);
			if (t2) {
				strcpy(txt, t2);
				txt += strlen(t2);
				DeleteName(t2);
			}
		}
		else if ((cp->maj() == 2)
		 && (cp->min() == 10)
		 && (cp->dat() == 1)) {
			sz = cp->siz();
			if (cp->dt() == etext) {
				strncpy(txt, (const char*) cp->ex(), sz);
				txt += sz;
			}
			else if (cp->dt() == imnem) {
				if ((sz > 0)
				 && (sz <= 0x7F))
					*txt++ = (char) (sz & 0x7F);
				else if ((sz > 0x7F)
				 && (sz < 0x1FFFFF)) {
					// write in UTF-8
					txt = (char *) ::UTF8((unl) sz, (unc *) txt);
				}
			}
		}
	}

	if (txt == cbuf)
		return NULL;

	*txt = '\0';
	return NewName(cbuf);
}


void
XMLrd::ReplaceUniqueIDs(DCelem *elem)
{
	// replace all unique IDs, DCctl(ilong, 2, 112, 2, num)
	// with new siz(++ItemID)
	// recurse into DCgroups

	if (!elem)
		return;
	DCgroup *et = elem->elemtext;
	if (et)
		ReplaceIDsInGroup(et);
}


void
XMLrd::ReplaceIDsInGroup(DCgroup *gp)
{
	DCctl *cp = NULL;
	DCitem *ip = NULL;

	for (ip = gp->first; ip; ip = ip->next) {
		cp = ip->ctl;
		if ((cp->dt() == ilong)
		 && (cp->maj() == 2)
		 && (cp->min() == 112)
		 && (cp->dat() == 2))
			ip->ctl = new DCctl(ilong, 2, 112, 2, ++ItemID);
		else if (cp->dt() == group)
			ReplaceIDsInGroup((DCgroup *) cp);
	}
}


void
XMLrd::ReplaceIndexRefs(DCelem *elem)
{
	// inside index groups, DCctl(group, 2, 110, 1)
	// replace all refs, DCctl(etext, 2, 112, 2, str)
	// and xref DCctl(etext, 2, 110, 4, str)
	// with new rstr which is guid of dest topic
	// recurse into DCgroups

	if (!elem
	 || (elem->eltyp != estopic))
		return;

	DCpage *page = ((DCtopelem *) elem)->topic->page;
	page->setchild(&elem);

	char *rstr = ((DCtopelem *) elem)->topic->id;


	// check elem and children for elem->idxrefs ****
	ReplaceIndexRefsInElem(page, elem);

	DCgroup *et = elem->elemtext;
	page->setgroup(&et);
	if (et)
		ReplaceIndexRefsInGroup(page, et, rstr);
}


void
XMLrd::ReplaceIndexRefsInElem(DCpage *page, DCelem *elem)
{
	if (elem->idxref)  // at indexterm element
		CloneIndexRef(elem, (DCixref *) elem->idxref);


	//if (elem->idxrefs) {  // at indexterm element
	//	DClist *ir = elem->idxrefs;
	//	while ((ir = ir->next) != NULL)
	//		CloneIndexRef(elem, (DCixref *) ir->id);
	//}

	if (elem->children) {
		DClist *child = elem->children;
		DCelem *celem = NULL;
		while ((child = child->next) != NULL) {
			celem = (DCelem *) child->id;
			page->setchild(&celem);
			ReplaceIndexRefsInElem(page, celem);
		}
	}
}


void
XMLrd::ReplaceIndexRefsInGroup(DCpage *page, DCgroup *gp, char *rstr)
{
	DCctl *cp = NULL;
	DCitem *ip = NULL;


	for (ip = gp->first; ip; ip = ip->next) {
		cp = ip->ctl;
		if ((cp->dt() == ilong)
		 && (cp->maj() == 2)
		 && (cp->min() == 110)
		 && (cp->dat() == 1)) {
			// make clone of index ref and add to its lists ***
			DCgroup *ncp = new DCgroup(*((DCgroup *) cp), page->groupmap);
			gp->replace(*ncp, *cp);
			ReplaceIndexRefIDs((DCgroup *) ncp, rstr);
		}
		else if (cp->dt() == group)
			ReplaceIndexRefsInGroup(page, (DCgroup *) cp, rstr);
	}
}


void
XMLrd::ReplaceIndexRefIDs(DCgroup *gp, char *rstr)
{
	DCctl *cp = NULL;
	DCitem *ip = NULL;

	for (ip = gp->first; ip; ip = ip->next) {
		cp = ip->ctl;
		if ((cp->dt() == etext)
		 && (cp->maj() == 2)
		 && (cp->min() == 110)
		 && (cp->dat() == 4))
			cp->ex(rstr);
		else if ((cp->dt() == etext)
		 && (cp->maj() == 2)
		 && (cp->min() == 112)
		 && (cp->dat() == 2))
			cp->ex(rstr);
	}
}



void
XMLrd::ProcConrefFailure(DCcnref *cinfo, cnerrtyp err)
{
	if (cinfo->destelem) {
		if (InIgnoreGroup)
			InIgnoreGroup = false;
		else if (cinfo->destelem->eignore)
			cinfo->destelem->eignore->maj((uns) 0);

		if (cinfo->destelem->einset)
			cinfo->destelem->einset->maj((uns) 0);
	}

	switch (err) {

		case noelem:
			LogEvent(logwarn, 1, "Could not find conref element ", *cinfo->href);
			LogEvent(logcont, 1, "in: ", cinfo->xrname);
			break;

		case notopic:
			LogEvent(logwarn, 1, "Could not find conref topic ", *cinfo->href);
			LogEvent(logcont, 1, "in: ", cinfo->xrname);
			break;

		case nodocinfo:
			LogEvent(logwarn, 1, "Missing docinfo for conref ", *cinfo->href);
			LogEvent(logcont, 1, "in: ", cinfo->xrname);
			break;

		case nodoc:
			LogEvent(logwarn, 1, "Could not find doc for conref ", *cinfo->href);
			LogEvent(logcont, 1, "in: ", cinfo->xrname);
			break;

		case nomap:
			LogEvent(logwarn, 1, "Could not find conref map ", *cinfo->href);
			LogEvent(logcont, 1, "in: ", cinfo->xrname);
			break;

		case nomapelem:
			LogEvent(logwarn, 1, "Could not find map conref element ", *cinfo->href);
			LogEvent(logcont, 1, "in: ", cinfo->xrname);
			break;

		case nomapinfo:
			LogEvent(logwarn, 1, "Missing mapinfo for conref ", *cinfo->href);
			LogEvent(logcont, 1, "in: ", cinfo->xrname);
			break;

		case noact:
			LogEvent(logwarn, 1, "Unsupported action ", cinfo->actatt, " for conref ", *cinfo->href);
			LogEvent(logcont, 1, "in: ", cinfo->xrname);
			break;

		case not2m:
			LogEvent(logwarn, 1, "Cannot conref topic elem into map ", *cinfo->href);
			LogEvent(logcont, 1, "in: ", cinfo->xrname);
			break;

		case nom2mpush:
			LogEvent(logwarn, 1, "Cannot push map elem to map ", *cinfo->href);
			LogEvent(logcont, 1, "in: ", cinfo->xrname);
			break;
		default:
		case noerr:
			break;
	}
}



// topic conrefs

DCcnref *XMLrd::CurrConrefT2TInfo = NULL;
DCcnref *XMLrd::CurrConrefT2TReplacement = NULL;
bool XMLrd::ProcessingT2TConrefs = false;
long XMLrd::ConrefT2TCount = 0;
long XMLrd::ConrefsT2TProcessed = 0;
long XMLrd::ConrefsT2TNum = 0;
DCvlist XMLrd::ConrefsT2T;
bool XMLrd::MakeConrefLists = false;
DClist XMLrd::ConrefAnumList;
DClist XMLrd::ConrefLOFList;
DClist XMLrd::ConrefLOTList;


void
XMLrd::CheckTopicConref(DCelem *elem, bool isRoot)
{
	// if element has conref, save its attrs and set up group
	if ((elem = CheckElemConact(elem)) == NULL)
		return;

	if (elem->conref) {
		if (FileIsMap(elem->conref)) {
			if (elem->conact)
				// push, error because map processing is already done
				SetT2MConrefInfo(elem, true);
			else
				// pull, process with T2T
				SetM2TConrefInfo(elem, false);
		}
		else
			SetT2TConrefInfo(elem, isRoot, (elem->conact != NULL));
	}
}


void 
XMLrd::SetT2TConrefInfo(DCelem *elem, bool isRoot, bool push)
{
	CurrConrefT2TInfo = new DCcnref(++ConrefT2TCount, ctt2t,
	                             ConrefsT2T, elem);
	if (push) {
		CurrConrefT2TInfo->srcset = CurrTopicInfo;
		CurrConrefT2TInfo->srcelem = elem;
	}
	else {
		CurrConrefT2TInfo->destset = CurrTopicInfo;
		CurrConrefT2TInfo->destelem = elem;
	}
	SetConrefConds(CurrConrefT2TInfo);

	AnumElemList->append(ConrefT2TCount);
	if (GenerateLOF)
		LOFItemList->append(ConrefT2TCount);
	if (GenerateLOT)
		LOTItemList->append(ConrefT2TCount);

	MarkConrefGenList(ConrefT2TCount);

	if (ReadTopicTitle)  // flag that title ctl is invalid
		CurrTopicInfo->titlechanged = true;

	if (!isRoot // if full topic, pagegroup is its container
	 && !push)
		SetConrefPath(elem, false);
}


void 
XMLrd::ProcessT2TConrefs(XMLrfile *rf)
{
		// for conrefs, insert entire element content in group
		//  or treat conref as a variable and put it in a variable def
	long i = 0;

	DCcnref *cinfo = NULL;

	DCelemset **targetset = NULL;
	DCelem **targetelem = NULL;
	cnerrtyp err = noerr;

	char *branch = CurrBranch;
	conditions *conds = CurrCondSet;

	ProcessingT2TConrefs = ProcessingConrefs = true;
	AnumElemList = &ConrefAnumList;
	LOFItemList = &ConrefLOFList;
	LOTItemList = &ConrefLOTList;

	for (i = ConrefsT2TProcessed + 1; i <= ConrefT2TCount; i++) {
		ConrefsT2TNum = i;
		cinfo = (DCcnref *) ConrefsT2T.find(i);
		CurrConrefT2TReplacement = cinfo;
		CurrBranch = cinfo->branch;
		CurrCondSet = cinfo->conds;

		if (cinfo->action) { // push
			ConrefPull = false;
			targetset = &(cinfo->destset);
			targetelem = &(cinfo->destelem);
			if (GetTopicConrefTarget(rf, cinfo, targetset, targetelem, err)) {
				ResolvedT2TConrefs.add(cinfo, ++ResolvedT2TConrefsCount);
#if 0
				switch (cinfo->action) {
					case ca_replace:
						if ((*targetelem)->elemnum == 1)
							ReplaceTopic(cinfo, i);
						else
							ReplaceT2TElement(cinfo);
						break;

					case ca_before:
						PushTopicElemBefore(cinfo);
						break;

					case ca_after:
						PushTopicElemAfter(cinfo);
						break;

					case ca_mark:
						break;

					case ca_start:
						PushTopicElemAtStart(cinfo);
						break;

					case ca_end:
						PushTopicElemAtEnd(cinfo);
						break;

					default:
						err = noact;
						ProcConrefFailure(cinfo, err);
						break;
				}
#endif
			}
			else
				ProcConrefFailure(cinfo, err);
		}
		else {
			ConrefPull = true;
			targetset = &(cinfo->srcset);
			targetelem = &(cinfo->srcelem);
			if (GetTopicConrefTarget(rf, cinfo, targetset, targetelem, err)) {
				ResolvedT2TConrefs.add(cinfo, ++ResolvedT2TConrefsCount);
#if 0
				if ((*targetelem)->elemnum == 1)
					ReplaceTopic(cinfo, i);
				else
					ReplaceT2TElement(cinfo);
#endif
			}
			else
				ProcConrefFailure(cinfo, err);
		}
#if 0
		if (cinfo->destset
		 && cinfo->destset->titlechanged)
			UpdateTopicTitle((DCtopic *) (cinfo->destset));
#endif
	}
	ConrefsT2TProcessed = ConrefT2TCount;
	SetT2TConrefAction();
	ProcessingT2TConrefs = ProcessingConrefs = false;
	AnumElemList = &AnumElems;
	LOFItemList = &LOFItems;
	LOTItemList = &LOTItems;
	CurrBranch = branch;
	CurrCondSet = conds;
}


DCvlist XMLrd::ResolvedT2TConrefs;
DCvlist XMLrd::SortedT2TConrefs;
long XMLrd::ResolvedT2TConrefsCount = 0;

void
XMLrd::SetT2TConrefAction(void)
{
	DCcnref *cinfo = NULL;
	DCcnref *tinfo = NULL;
	long scount = 0;
	DClist *moved = new DClist;
	long i = 0;
	long j = 0;

	// sort resolved list so use as dest precedes use as src
	for (i = 1; i <= ResolvedT2TConrefsCount; i++) {
#if 0
		if (moved->find(i))
			continue;
#endif
		cinfo = (DCcnref *) ResolvedT2TConrefs.find(i);
#if 0
		for (j = i + 1; j <= ResolvedT2TConrefsCount; j++) {
			tinfo = (DCcnref *) ResolvedT2TConrefs.find(j);
			if (cinfo->srcset == tinfo->destset) {
				moved->add(j);
				SortedT2TConrefs.add(tinfo, ++scount);	
			}
		}
#endif
		SortedT2TConrefs.add(cinfo, ++scount);	
	}

	for (i = 1; i <= ResolvedT2TConrefsCount; i++) {
		cinfo = (DCcnref *) SortedT2TConrefs.find(i);
		CurrConrefT2TReplacement = cinfo;
		CurrBranch = cinfo->branch;
		CurrCondSet = cinfo->conds;
		DoT2TConrefAction(cinfo, i);
	}

	delete moved;
}


void
XMLrd::DoT2TConrefAction(DCcnref *cinfo, long i)
{
	switch (cinfo->action) {
		case ca_pull:
			if (cinfo->srcelem->elemnum == 1)
				ReplaceTopic(cinfo, i);
			else
				ReplaceT2TElement(cinfo);
			break;

		case ca_replace:
			if (cinfo->destelem->elemnum == 1)
				ReplaceTopic(cinfo, i);
			else
				ReplaceT2TElement(cinfo);
			break;

		case ca_before:
			PushTopicElemBefore(cinfo);
			break;

		case ca_after:
			PushTopicElemAfter(cinfo);
			break;

		case ca_mark:
			break;

		case ca_start:
			PushTopicElemAtStart(cinfo);
			break;

		case ca_end:
			PushTopicElemAtEnd(cinfo);
			break;

		default:
			ProcConrefFailure(cinfo, noact);
			break;
	}

	if (cinfo->destset
	 && cinfo->destset->titlechanged)
		UpdateTopicTitle((DCtopic *) (cinfo->destset));
}


bool
XMLrd::GetTopicConrefTarget(XMLrfile *rf, DCcnref *cinfo,
 DCelemset **targetset, DCelem **targetelem, cnerrtyp &err)
{
	char *hfdoc = NULL;
	char *hftopic = NULL;
	char *hfelem = NULL;
	char *endid = NULL;

	long docnum = 0;
	long topnum = 0;
	long elnum = 0;

	DCtopdoc *dinfo = NULL;
	DCtopdoc *dinfo1 = NULL;

	DCelemset *tinfo = NULL;
	DCelemset *tinfo1 = NULL;
	DCelem *einfo = NULL;

	// return true if target found, false with err set if not
	err = noerr;

	// see if in doc already read
	hfdoc = hftopic = hfelem = NULL;
	ParseHref(*cinfo->href, &hfdoc, &hftopic, &hfelem);
	if ((docnum = GetProcessedDocInfo(hfdoc)) != 0) {
		if ((dinfo = (DCtopdoc *) DocInfos.find(docnum)) != NULL) {
			dinfo1 = dinfo;
			topnum = hftopic ? dinfo->topicids->getitem(hftopic) : 1;
			if (!topnum) {
				CurrDocRef = ((DCtopic *) (cinfo->destset))->doc->mref;
				ParseDocument(rf, cinfo->href, true);
				if (rf->FErr() != fok) {
					err = nodoc;
					return false;
				}
				else if (!*targetset) {
					err = notopic;
					return false;
				}
				else if (!*targetelem) {
					err = noelem;
					return false;
				}
				else   // found it, now replace
					return true;
			}
			tinfo1 = tinfo = (DCtopic *) dinfo->topics->find(topnum);

			if (GetFinalConrefTopic(&docnum, &topnum)) {
				dinfo = (DCtopdoc *) DocInfos.find(docnum);
				if (!dinfo) {  // revert to original
					dinfo = dinfo1;
					tinfo = tinfo1;
				}
			}
			tinfo = (DCtopic *) dinfo->topics->find(topnum);
			if (hfelem) {           // ref is to a specific element
				if (((elnum = tinfo->elemids->getitem(hfelem)) != 0)
				 && ((einfo = (DCtopelem *) tinfo->elems->find(elnum)) != NULL)) {
					*targetset = tinfo;
					*targetelem = einfo;
					return true;
				}
				else if (tinfo != tinfo1) {  // revert to original
					tinfo = tinfo1;			
					if (((elnum = tinfo->elemids->getitem(hfelem)) != 0)
					 && ((einfo = (DCtopelem *) tinfo->elems->find(elnum)) != NULL)) {
						*targetset = tinfo;
						*targetelem = einfo;
						return true;
					}
					else {
						err = noelem;
						return false;
					}
				}
				else {
					err = noelem;
					return false;
				}
			}
			else {  // ref is to entire topic
				if ((einfo = (DCtopelem *) tinfo->elems->find(1)) != NULL) {
					*targetset = tinfo;
					*targetelem = einfo;
					return true;
				}
				else if (tinfo != tinfo1) {  // revert to original
					tinfo = tinfo1;			
					if ((einfo = (DCtopelem *) tinfo->elems->find(1)) != NULL) {
						*targetset = tinfo;
						*targetelem = einfo;
						return true;
					}
					else {
						err = notopic;
						return false;
					}
				}
				else {
					err = notopic;
					return false;
				}
			}
		}
		else { // else in processed doc, but docinfo missing
			err = nodocinfo;
			return false;
		}
	}
	else {   // not in processed doc, get it and parse it all
		if (ConrefPull) {
			if (cinfo->destset->estyp == estopic)
				CurrDocRef = ((DCtopic *) (cinfo->destset))->doc->mref;
			else
				CurrDocRef = (DCmapelem *) cinfo->destelem;
		}
		else { // push
			if (cinfo->srcset->estyp == estopic)
				CurrDocRef = ((DCtopic *) (cinfo->srcset))->doc->mref;
			else
				CurrDocRef = (DCmapelem *) cinfo->srcelem;
		}
		ParseDocument(rf, cinfo->href, true);
		if (rf->FErr() != fok) {
			err = nodoc;
			return false;
		}
		else if (!*targetset) {
			err = notopic;
			return false;
		}
		else if (!*targetelem) {
			err = noelem;
			return false;
		}
		else   // found it, now replace
			return true;
	}
}


DCilist XMLrd::ConrefTopics; // conref doctopnum indexed by original doctopnum


bool 
XMLrd::GetFinalConrefTopic(long *docnum, long *topicnum)
{
	// for original docnum and topicnum, set final values, return true
	// if not found, return false
	long dtnum = (*docnum << 16) + *topicnum;
	if (!ConrefTopics.find(dtnum))
		return false;

	long lastdt = 0;
	while (dtnum) {
		lastdt = dtnum;
		dtnum = ConrefTopics.find(dtnum);
	}
	*docnum = (lastdt >> 16) & 0xFFFF;
	*topicnum = lastdt & 0xFFFF;
	return true;
}


void
XMLrd::UpdateTopicTitle(DCtopic *top)
{
	// title altered by conref or numbering, rewrite ctl from elemtext
	LogEvent(loginfo, 2, "Topic title \"", top->title, "\" in ",
		                   top->doc->docname, " updated");
	top->titlechanged = false;  // just fix once
	DCelem *elem = (DCelem *) top->elems->find(2);
	DCgroup *gp = NULL;
	char *txt = NULL;
	if (elem)
		gp = elem->elemtext;
	if (gp)
		txt = GetGroupText(gp);
	if (txt && *txt) {
		top->title = txt;
		LogEvent(logcont, 2, "New title is: ", top->title);
	}
	else
		LogEvent(logwarn, 1, "Topic title of ", top->doc->docname,
			                   " not replaced after update.");
}


void 
XMLrd::ReplaceTopic(DCcnref *cinfo, long citem)
{
	LogEvent(loginfo, 2, "Replacing conref topic ", *cinfo->href);
	LogEvent(logcont, 2, "in: ", cinfo->xrname);

	DCtopic *srctop = (DCtopic *) cinfo->srcset;
	DCtopic *desttop = (DCtopic *) cinfo->destset;
	DCtopelem *srcelem = (DCtopelem *) cinfo->srcelem;
	DCtopelem *destelem = (DCtopelem *) cinfo->destelem;

	// clone topic and its page, with new DCtopic and DCelem root element,
	// but retain pointers to the rest of the elements
	// with default copy constructor
	DCtopic *newtop = new DCtopic(*srctop);
	newtop->topicnum = ++CurrTopicNum;
	CurrTopicInfo = newtop;

	long newdtnum = CurrTopicNum + (newtop->docnum << 16);
	long olddtnum = desttop->topicnum + (desttop->docnum << 16);
	ConrefTopics.add(newdtnum, olddtnum);
	newtop->doc->topics->add(newtop, CurrTopicNum);

	DCtopelem *newroot = new DCtopelem(*srcelem);
	DCvlist *newelems = new DCvlist();
	newelems->add(newroot, 1);
	newelems->next->next = newtop->elems->next->next;
	newtop->elems = newelems;
	CurrElemInfo = newroot;
	newroot->topicnum = CurrTopicNum;
	newroot->topic = CurrTopicInfo;

	DCpage *newpage = new DCpage(*(srctop->page));
	newtop->page = CurrPageInfo = newpage;
	newtop->pagenum = newpage->pagenum = ++PageNum;
	Pages.add(CurrPageInfo, PageNum);

	PageGroup = new DCgroup(*(CurrPageInfo->pgroup), CurrPageInfo->groupmap);
	PageTextGroup = new DCgroup(*(CurrPageInfo->pagetext), CurrPageInfo->groupmap);
	newpage->setgroup(&PageTextGroup);
	newpage->setgroup(&newroot->elemtext);
	if (!newroot->groupmap->next)
		newroot->groupmap->next = newpage->groupmap->next;

	CurrPageInfo->pgroup = PageGroup;
	CurrPageInfo->pagetext = PageTextGroup;
	PageList.add(PageGroup, PageNum);

	if (desttop->part) {
		newtop->part = desttop->part;
		DCLPartPages.add(PageNum, desttop->part);
		DCLPageParts.add(desttop->part, PageNum);
	}

	// replace page used by toporig with page used by topnew in PageSequence
	uns origpage = (uns) desttop->pagenum;
	long seqpage = PageSequence(origpage);
	PageSequence.replace(PageNum, seqpage);
	RemovedPages.add(origpage);

	// replace conref src id with orig id
	DCgroup *origidgrp = destelem->idgrp;
	if (!origidgrp)  // if no orig id, replace with empty group
		origidgrp = new DCgroup();
	DCgroup *newidgrp = newroot->idgrp;
	newpage->setgroup(&newidgrp);
	if (origidgrp && newidgrp) {
		if (!PageTextGroup->replace(*origidgrp, *newidgrp))
			LogEvent(logwarn, 2, "Page idgrp not replaced.");
	}

	// adjust attrs for copy of conref'd topic's root element
	if (IncludeElementTags) {
		DCgroup *origagrp = srcelem->attrgrp;
		newpage->setgroup(&origagrp);
		DCgroup *newagrp = new DCgroup();
		newroot->attrtext = PageTextGroup;
		if (origagrp && newagrp) {
			if (!PageTextGroup->replace(*newagrp, *origagrp))
				LogEvent(logwarn, 2, "Page attrgrp not replaced.");
		}
		AdjustConrefAttrs(destelem, newroot, newagrp);
	}

	if (newroot->children) {
		newroot->children = new DClist(*(srcelem->children));
		// recursively create new children from the old ones
		ReplaceChildren(newroot);
	}

	// correct any conrefs in topnew to point at newtop
	DCcnref *ci = NULL;
	while ((ci = (DCcnref *) SortedT2TConrefs.find(++citem)) != NULL) {
		if (ci->destset == cinfo->srcset) {
			ci->destset = newtop;
			newpage->setchild(&(ci->destelem));
		}
	}

	MakeConrefLists = !cinfo->didlists;

	// replace autonum and LOF/LOT list pointers
	ReplaceAnumListItems(cinfo);
#if 0
	AnumElems.substitute(cinfo->cnum, -cinfo->cnum, &cinfo->anums);
	LOFItems.substitute(cinfo->cnum, -cinfo->cnum, &cinfo->lofitems);
	LOTItems.substitute(cinfo->cnum, -cinfo->cnum, &cinfo->lotitems);
#endif

	EndConrefGenLists(cinfo);
}

void 
XMLrd::ReplaceAnumListItems(DCcnref *cinfo)
{
	bool found = false;
	DCcnref *ncinfo = NULL;

	if (AnumElems.find(cinfo->cnum))
		AnumElems.substitute(cinfo->cnum, -cinfo->cnum, &cinfo->anums);
	else if (AnumElemList->find(cinfo->cnum))
		AnumElemList->substitute(cinfo->cnum, -cinfo->cnum, &cinfo->anums);
	else {
		for (long i = 1; i <= ResolvedT2TConrefsCount; i++) {
			ncinfo = (DCcnref *) SortedT2TConrefs.find(i);
			if (ncinfo->anums.find(cinfo->cnum)) {
				ncinfo->anums.substitute(cinfo->cnum, -cinfo->cnum, &cinfo->anums);
				found = true;
				break;
			}
		}
		if (!found)
			LogEvent(logwarn, 2, "Can't find list for conreffed anum.");
	}
	//AnumElems.substitute(cinfo->cnum, -cinfo->cnum, &cinfo->anums);
	LOFItems.substitute(cinfo->cnum, -cinfo->cnum, &cinfo->lofitems);
	LOTItems.substitute(cinfo->cnum, -cinfo->cnum, &cinfo->lotitems);
}


void 
XMLrd::ReplaceT2TElement(DCcnref *cinfo)
{
	char *endid = NULL;
	DCtopelem *endelem = NULL;
	DCtopelem *addelem = NULL;
	DCtopelem *nsrcelem = NULL;
	long startpos = 0;
	long endpos = 0;
	long currpos = 0;
	bool getRange = false;
	DCitem *nxttxt = NULL;

	DCtopic *srctop = (DCtopic *) cinfo->srcset;
	DCtopic *desttop = (DCtopic *) cinfo->destset;
	DCtopelem *srcelem = (DCtopelem *) cinfo->srcelem;
	DCtopelem *destelem = (DCtopelem *) cinfo->destelem;

	if (cinfo->action)
		LogEvent(loginfo, 2, "Push replacing conref topic element ", *cinfo->href);
	else
		LogEvent(loginfo, 2, "Replacing conref topic element ", *cinfo->href);
	LogEvent(logcont, 2, "in: ", desttop->doc->docname);

	// may be conref range
	// endid is the element ID of the end of the range to get
	// must have same parent as *href, and be later sibling
	// fix ids of elems after first to ensure unique in doc
	// even if same range already conreffed in same doc
	if (cinfo->endref
	 && *(cinfo->endref)) {
		if (cinfo->action) {  // push takes precedence
			LogEvent(logwarn, 1, "Conref range end element ignored for ",
													 cinfo->actatt, "in ", *cinfo->href);
		}
		else {
			if ((endid = strrchr(*(cinfo->endref), '/')) == NULL)
				LogEvent(logwarn, 1, "Could not find conref range end element id.");
			else {
				endid++;
				long endnum = srctop->elemids->getitem(endid);
				if (!endnum)
					LogEvent(logwarn, 1, "Conref range end element ",
															 endid, "not in topic.");
				else {
					endelem = (DCtopelem *) srctop->elems->find(endnum);
					endpos = srcelem->parent->children->position((long) endelem);
					startpos = srcelem->parent->children->position((long) srcelem);
					if (!endpos)
						LogEvent(logwarn, 1, "Conref range end element ",
																 endid, " has different parent.");
					else if (endpos < startpos)
						LogEvent(logwarn, 1, "Conref range end element ",
																 endid, " precedes start element.");
					else
						getRange = true;
				}
			}
		}
	}

	// replace elem struct with new one
	DCtopelem *newelem = new DCtopelem(*srcelem);
	ReplaceTopicElement(srctop, srcelem, desttop, destelem, newelem);
	AdjustConreffedElement(cinfo, newelem);

	// for range, rest must be done for each elem
	// in addition, newelems must be inserted in toporig
	DCtopelem *lastelem = newelem;
	DCgroup *textgrp = CurrTopicConrefTextGrp;
	nxttxt = CurrTopicConrefNxtTxt;

	if (getRange) {
		for (currpos = startpos + 1; currpos <= endpos; currpos++) {
			nsrcelem = (DCtopelem *) srcelem->parent->children->number(currpos);
			addelem = new DCtopelem(*nsrcelem);
			addelem->parent = newelem->parent;
			newelem->parent->children->insertafter((long) lastelem, (long) addelem);
			addelem->topic = newelem->topic;
			addelem->topicnum = newelem->topicnum;
			addelem->elemnum = ++(addelem->topic->elemcnt);
			addelem->topic->elems->add(addelem, addelem->elemnum);
			if (addelem->id  // src had an id not already in this topic
			 && !addelem->topic->elemids->getitem(addelem->id))
				addelem->topic->elemids->add(addelem->id, addelem->elemnum);
			addelem->elemtext = new DCgroup(*(nsrcelem->elemtext));
			if (!nxttxt)
				textgrp->add(*(addelem->elemtext));
			else
				textgrp->insert(*(addelem->elemtext), *(nxttxt->ctl)); 
			AddConrefRangeElement(cinfo, addelem, nsrcelem);
			ReplaceGraphic(desttop, nsrcelem, addelem);
			lastelem = addelem;
		}
	}
}


void
XMLrd::PushTopicElemAfter(DCcnref *cinfo)
{
	// put srcelem after destelem, and also after any other elems
	// already pushed after destelem
	LogEvent(loginfo, 2, "Pushing after conref topic element ", *cinfo->href);
	LogEvent(logcont, 2, "in: ", ((DCtopic *) cinfo->destset)->doc->docname);
	AddOneTopicElem(cinfo, false);
}


void
XMLrd::PushTopicElemBefore(DCcnref *cinfo)
{
	// put srcelem before destelem, after any others pushed before
	LogEvent(loginfo, 2, "Pushing before conref topic element ", *cinfo->href);
	LogEvent(logcont, 2, "in: ", ((DCtopic *) cinfo->destset)->doc->docname);
	AddOneTopicElem(cinfo, true);
}


void
XMLrd::PushTopicElemAtStart(DCcnref *cinfo)
{
	// put srcelem inside destelem at start, but after any other elems
	// already inserted into destelem
	LogEvent(loginfo, 2, "Pushing into start of conref topic element ", *cinfo->href);
	LogEvent(logcont, 2, "in: ", ((DCtopic *) cinfo->destset)->doc->docname);
	InsertTopicElemText(cinfo, false);
}


void
XMLrd::PushTopicElemAtEnd(DCcnref *cinfo)
{
	// put srcelem inside destelem at end, sfter any others added at end
	LogEvent(loginfo, 2, "Pushing into end of conref topic element ", *cinfo->href);
	LogEvent(logcont, 2, "in: ", ((DCtopic *) cinfo->destset)->doc->docname);
	InsertTopicElemText(cinfo, true);
}


void
XMLrd::InsertTopicElemText(DCcnref *cinfo, bool atend)
{
	// extract source elemtext into string
	// add a space before (for end) or after (for start)
	// add to elem startgroup or endgroup (empty groups
	//  put in after StartPara and before EndPara ctls)
	if (!cinfo->srcelem
	 || !cinfo->destelem)
		return;

	char *txt = GetGroupText(cinfo->srcelem->elemtext);
	if (!txt
	 || !*txt)
		return;

	DCctl *cp = new DCctl(etext, 2, 10, 1);
	cp->ex(txt, strlen(txt) + 1);
	DCctl *spc = new DCctl(imnem, 2, 10, 1, 32);
	DCgroup *destgp = (atend ? cinfo->destelem->endtext : cinfo->destelem->starttext);
	
	if (atend) {
		destgp = cinfo->destelem->endtext;
		if (destgp) {
			if (*txt != ' ')
				destgp->add(*spc);
			destgp->add(*cp);
		}
		else if ((destgp = (DCgroup *) ElemTextGroups(cinfo->destelem)) != NULL) {
			// probably after para end ****
			if (*txt != ' ')
				destgp->add(*spc);
			destgp->add(*cp);
		}
	}
	else {
		destgp = cinfo->destelem->starttext;
		if (destgp) {
			destgp->add(*cp);
			if (*(txt + strlen(txt) - 1) != ' ')
				destgp->add(*spc);
		}
		else if ((destgp = (DCgroup *) ElemTextGroups(cinfo->destelem)) != NULL) {
			// probably before para start ****
			if (*txt != ' ')
				destgp->insert(*spc);
			destgp->insert(*cp);
		}
	}
}


void
XMLrd::AddOneTopicElem(DCcnref *cinfo, bool before)
{
	DCtopelem *srcelem = (DCtopelem *) cinfo->srcelem;
	DCtopelem *newelem = (DCtopelem *) cinfo->destelem;

	DCtopelem *addelem = new DCtopelem(*srcelem);
	DCtopelem *lastelem = newelem;

	if (!before) {
		while (lastelem->addedafter)
			lastelem = (DCtopelem *) lastelem->addedafter;
	}

	addelem->parent = newelem->parent;

	if (before) {
		newelem->parent->children->replace((long) addelem, (long) lastelem);
		newelem->parent->children->insertafter((long) addelem, (long) lastelem);
	}
	else
		newelem->parent->children->insertafter((long) lastelem, (long) addelem);

	addelem->topic = newelem->topic;
	addelem->topicnum = newelem->topicnum;
	addelem->elemnum = ++(addelem->topic->elemcnt);
	addelem->topic->elems->add(addelem, addelem->elemnum);
	if (addelem->id  // src had an id not already in this topic
	 && !addelem->topic->elemids->getitem(addelem->id))
		addelem->topic->elemids->add(addelem->id, addelem->elemnum);

	addelem->elemtext = new DCgroup(*(srcelem->elemtext), addelem->groupmap);
	DCgroup *origgrp = lastelem->elemtext;
	DCgroup *textgrp = newelem->lasttext;
	if (!textgrp)  // NULL means elem's parent is the PageGroup
		textgrp = newelem->topic->page->pgroup;
	if (origgrp && addelem->elemtext && textgrp) {
		if (before) {
			if (!textgrp->insert(*(addelem->elemtext), *origgrp))
				LogEvent(logwarn, 1, "Addelem elemtext not inserted.");
		}
		else {
			if (!textgrp->replace(*(addelem->elemtext), *origgrp)
			 || !textgrp->insert(*origgrp, *(addelem->elemtext)))
				LogEvent(logwarn, 1, "Addelem elemtext not appended.");
		}
	}

	AddConrefRangeElement(cinfo, addelem, srcelem);
	ReplaceGraphic(newelem->topic, srcelem, addelem);
}




DCgroup *XMLrd::CurrTopicConrefTextGrp = NULL;
DCitem *XMLrd::CurrTopicConrefNxtTxt = NULL;


void 
XMLrd::ReplaceTopicElement(DCelemset *srctop, DCelem *srcelem,
	 DCtopic *desttop, DCtopelem *destelem, DCtopelem *newelem)
{
	DCitem *nxttxt = NULL;

	newelem->parent = destelem->parent;
	newelem->parent->children->replace((long) newelem, (long) destelem);
	newelem->topic = destelem->topic;
	newelem->topicnum = destelem->topicnum;
	newelem->elemnum = destelem->elemnum;
	newelem->topic->elems->replace(newelem, newelem->elemnum); // id is same

	if (newelem->isAncestor((DCelem *) newelem->topic->elems->find(2)))
		newelem->topic->titlechanged = true;

	// replace group containing elemorig with group containing elemnew
	// both have an elemtext group set, lasttext is parent group
	DCgroup *origgrp = destelem->elemtext;
	DCgroup *newgrp = srcelem->elemtext;
	newelem->topic->page->setgroup(&newgrp);
	newelem->elemtext = new DCgroup(*newgrp, newelem->groupmap);

	DCgroup *textgrp = newelem->lasttext = destelem->lasttext;
	if (!textgrp)  // NULL means elem's parent is the PageGroup
		textgrp = newelem->lasttext = desttop->page->pgroup;
	if (origgrp && newelem->elemtext && textgrp) {
		if (!textgrp->replace(*(newelem->elemtext), *origgrp, &nxttxt))
			LogEvent(logwarn, 1, "Newelem elemtext not replaced.");
	}
	if (InIgnoreGroup)
		InIgnoreGroup = false;
	else if (destelem->eignore)
		destelem->eignore->maj((uns) 0);
	CurrTopicConrefTextGrp = textgrp;
	CurrTopicConrefNxtTxt = nxttxt;

	// replace conref src id with orig id
	DCgroup *newidgrp = srcelem->idgrp;
	newelem->setgroup(&newidgrp);
	newelem->topic->page->setgroup(&newidgrp);
	if ((newelem->idgrp = destelem->idgrp) == NULL)
		newelem->idgrp = new DCgroup(); // if no orig id, use empty group
	if (newelem->idgrp && newidgrp && newelem->elemtext) {
		if (!newelem->elemtext->replace(*(newelem->idgrp), *newidgrp))
			LogEvent(logwarn, 2, "Newelem idgrp not replaced.");
	}

	// adjust attrs for copy of conref'd element
	if (IncludeElementTags) {
		DCgroup *origagrp = destelem->attrgrp;
		newelem->attrgrp = new DCgroup();
		newelem->attrtext = destelem->attrtext;
		if (origagrp && newelem->attrgrp && newelem->attrtext) {
			if (!newelem->attrtext->replace(*(newelem->attrgrp), *origagrp))
				LogEvent(logwarn, 2, "Newelem attrgrp not replaced.");
		}
		AdjustConrefAttrs(destelem, srcelem, newelem->attrgrp);
	}
}


void 
XMLrd::AdjustConreffedElement(DCcnref *cinfo, DCtopelem *newelem)
{
	DCtopic *srctop = (DCtopic *) cinfo->srcset;
	DCtopic *desttop = (DCtopic *) cinfo->destset;
	DCtopelem *srcelem = (DCtopelem *) cinfo->srcelem;
	DCtopelem *destelem = (DCtopelem *) cinfo->destelem;

	// fix up any graphics removed or added
	RemoveGraphic(desttop, destelem);
	ReplaceGraphic(desttop, srcelem, newelem);

	// adjust formats used within conref'd element
	newelem->elpath = destelem->elpath;
	if (srcelem->epctl && destelem->epctl) {
		if (!newelem->elemtext->replace(*(destelem->epctl),
		                           *(srcelem->epctl)))
			LogEvent(logwarn, 2, "Newelem epctl not replaced.");
	}
	newelem->elfpath = destelem->elfpath;
	if (srcelem->epfctl && destelem->epfctl) {
		if (!newelem->elemtext->replace(*(destelem->epfctl),
		                           *(srcelem->epfctl)))
			LogEvent(logwarn, 2, "Newelem epfctl not replaced.");
	}

	newelem->elform = destelem->elform;
	newelem->outclass = destelem->outclass;
	newelem->formctl = destelem->formctl;
	if (srcelem->formctl
	 && destelem->formctl
	 && (srcelem->formctl->siz() != destelem->formctl->siz())) {
		if (!newelem->elemtext->replace(*(destelem->formctl),
		                           *(srcelem->formctl)))
			LogEvent(logwarn, 2, "Newelem formctl not replaced.");
	}

	if (destelem->anumtxt) {
		newelem->anumtxt = new DCgroup(DCctl(group, 2, 10, 3));
		DCgroup *oldanum = srcelem->anumtxt;
		newelem->setgroup(&oldanum);
		if (!newelem->elemtext->replace(*(newelem->anumtxt), *oldanum))
			LogEvent(logwarn, 2, "Newelem anumtxt not replaced.");

		char *nform = FmtFile->GetNumFmtName(newelem->elform);
		newelem->elemnf = GetNumFormat(nform);
	}

	ReplaceUniqueIDs(newelem);

	MakeConrefLists = !cinfo->didlists;

	AddOneConrefAnum(cinfo, newelem, srcelem);

	FixConrefGenLists(srcelem, newelem);

	if (srcelem->children) {
		newelem->children = new DClist(*(srcelem->children));
		// recursively create new children from the old ones
		ReplaceChildren(newelem);
	}

	ReplaceIndexRefs(newelem);

	// replace autonum and LOF/LOT list pointers
	ReplaceAnumListItems(cinfo);
#if 0
	if (AnumElems.find(cinfo->cnum))
		AnumElems.substitute(cinfo->cnum, -cinfo->cnum, &cinfo->anums);
	else if (AnumElemList->find(cinfo->cnum))
		AnumElemList->substitute(cinfo->cnum, -cinfo->cnum, &cinfo->anums);
	else {
		bool found = false;
		for (long i = 1; i <= ResolvedT2TConrefsCount; i++) {
			DCcnref *ncinfo = (DCcnref *) SortedT2TConrefs.find(i);
			if (ncinfo->anums.find(cinfo->cnum)) {
				ncinfo->anums.substitute(cinfo->cnum, -cinfo->cnum, &cinfo->anums);
				found = true;
				break;
			}
		}
		if (!found)
			LogEvent(logwarn, 2, "Can't find list for conreffed anum.");
	}
	LOFItems.substitute(cinfo->cnum, -cinfo->cnum, &cinfo->lofitems);
	LOTItems.substitute(cinfo->cnum, -cinfo->cnum, &cinfo->lotitems);
#endif

	EndConrefGenLists(cinfo);
}


void 
XMLrd::AddOneConrefAnum(DCcnref *cinfo, DCtopelem *newelem, DCelem *srcelem)
{;
	if (MakeConrefLists) {
		if (newelem->anumtxt) {
			bool found = false;
			DCtopelem *elem = NULL;
			for (DClist *dl = cinfo->anums.next; dl; dl = dl->next) {
				elem = (DCtopelem *) dl->id;
				newelem->topic->page->setchild((DCelem **) &elem);
				if (elem == newelem) {
					found = true;
					dl->id = (long) newelem;
					break;
				}
			}
			if (!found)
				cinfo->anums.append((long) newelem);
		}

		if (newelem->inLOF)
			cinfo->lofitems.append((long) newelem);
		if (newelem->inLOT)
			cinfo->lotitems.append((long) newelem);
	}
	else {
		DClist *lp = NULL;
		if (newelem->anumtxt
		 && ((lp = cinfo->anums.find((long) srcelem)) != NULL))
			lp->id = (long) newelem;
		if (newelem->inLOF
		 && ((lp = cinfo->lofitems.find((long) srcelem)) != NULL))
			lp->id = (long) newelem;
		if (newelem->inLOT
		 && ((lp = cinfo->lotitems.find((long) srcelem)) != NULL))
			lp->id = (long) newelem;
	}
}


void 
XMLrd::AddConrefRangeElement(DCcnref *cinfo,
         DCtopelem *newelem, DCtopelem *srcelem)
{
	// addelem is in place in dest as sibling of newelem
	// make addelem have content of srcelem, but in new context
	// adjust id attribute as needed to make unique in file
	// adjust other attrs if elem same as elemorig
	//  and attr was given in elemorig
	DCtopelem *parent = (DCtopelem *) newelem->parent;
	DCctl *cp = NULL;
	char *sep = "/";

	if (parent->elpath) {
		newelem->elpath = NewName(strlen(parent->elpath) +
		                          strlen(newelem->elemname) + 2);
		strcpy(newelem->elpath, newelem->elemname);
		strcat(newelem->elpath, sep);
		strcat(newelem->elpath, parent->elpath);
	}
	if (newelem->epctl) {
		cp = new DCctl(ename, 2, 121, 6);
		cp->ex(newelem->elpath);
		if (!newelem->elemtext->replace(*cp, *(newelem->epctl)))
			LogEvent(logwarn, 2, "Range elpath not replaced.");
		newelem->epctl = cp;
	}
	if (parent->elfpath) {
		newelem->elfpath = NewName(strlen(parent->elfpath) +
		                          strlen(newelem->elemname) + 2);
		strcpy(newelem->elfpath, newelem->elemname);
		strcat(newelem->elfpath, sep);
		strcat(newelem->elfpath, parent->elfpath);
	}
	if (newelem->epfctl) {
		cp = new DCctl(ename, 2, 121, 6);
		cp->ex(newelem->elfpath);
		if (!newelem->elemtext->replace(*cp, *(newelem->epfctl)))
			LogEvent(logwarn, 2, "Range elfpath not replaced.");
		newelem->epfctl = cp;
	}

	if (newelem->formctl) {
		newelem->elform = GetElemForm(newelem, newelem->outclass,
			newelem->elemname, newelem->elinlin, newelem->topic->tlevel);
		cp = SetFormat(newelem->elform, newelem->elinlin, false);
		if (!newelem->elemtext->replace(*cp, *(newelem->formctl)))
			LogEvent(logwarn, 2, "Range formctl not replaced.");
		newelem->formctl = cp;
	}

	if (newelem->anumtxt) {
		newelem->anumtxt = new DCgroup(DCctl(group, 2, 10, 3));
		DCgroup *oldanum = srcelem->anumtxt;
		newelem->setgroup(&oldanum);
		if (!newelem->elemtext->replace(*(newelem->anumtxt), *oldanum)
		 && (((oldanum = GetAnumTextGroup(newelem->elemtext)) == NULL)
		  || !newelem->elemtext->replace(*(newelem->anumtxt), *oldanum)))
			LogEvent(logwarn, 2, "Range anumtxt not replaced.");
		char *nform = FmtFile->GetNumFmtName(newelem->elform);
		newelem->elemnf = GetNumFormat(nform);
	}

	ReplaceUniqueIDs(newelem);

	AddOneConrefAnum(CurrConrefT2TReplacement, newelem, parent);

#if 0
	if (MakeConrefLists) {
		if (newelem->anumtxt)
			CurrConrefT2TReplacement->anums.append((long) newelem);
		if (newelem->inLOF)
			CurrConrefT2TReplacement->lofitems.append((long) newelem);
		if (newelem->inLOT)
			CurrConrefT2TReplacement->lotitems.append((long) newelem);
	}
	else {
		DClist *lp = NULL;
		if (newelem->anumtxt
		 && ((lp = CurrConrefT2TReplacement->anums.find((long) parent)) != NULL))
			lp->id = (long) newelem;
		if (newelem->inLOF
		 && ((lp = CurrConrefT2TReplacement->lofitems.find((long) parent)) != NULL))
			lp->id = (long) newelem;
		if (newelem->inLOT
		 && ((lp = CurrConrefT2TReplacement->lotitems.find((long) parent)) != NULL))
			lp->id = (long) newelem;
	}
#endif

	FixConrefGenLists(srcelem, newelem);

	ReplaceChildren(newelem);
}


void
XMLrd::RecalcFormat(DCelem *elemorig, DCelem *elemnew, long lev)
{
	// recalculate format based on elempath and outclass of orig
	// recalc any formats used within it, such as title in section

	if (elemnew->formctl) {

		// not same format, see if orig path and outclass works for new
		char *ocl = elemorig->outclass;
		char *form = NULL;
		if (ocl
		 && strcmp(ocl, "-dita-use-conref-target")
		 && elemnew->outclass)
			ocl = elemnew->outclass;
		form = GetElemForm(elemorig, ocl,
			                 elemorig->elemname, elemorig->elinlin, lev);
		if (elemnew->elform
		 && !strcmp(form, elemnew->elform))
			return;  // matched, leave it

		elemnew->elform = form;
		elemnew->formctl = new DCctl();
		if (elemorig->elform
		 && elemorig->formctl
		 && !strcmp(form, elemorig->elform))
			*(elemnew->formctl) = *(elemorig->formctl);  // clone
		else {  // something altogether new
			DCctl *cp = SetFormat(form, elemorig->elinlin, false);
			*(elemnew->formctl) = *cp;
		}
	}
}


void
XMLrd::ReplaceChildren(DCelem *elem)
{
	if (!elem->children)
		return;
	long cnt = elem->children->count();
	for (long i = 1; i <= cnt; i++)
		ReplaceChild( (DCelem *) elem->children->number(i), elem);
}


void
XMLrd::ReplaceChild(DCelem *elem, DCelem *parent, bool first)
{
	DCctl *cp = NULL;
	char *sep = first ? "^" : "/";
	bool ret = false;

	DCtopelem *newelem = new DCtopelem(* (DCtopelem *) elem);
	newelem->topic = ((DCtopelem *) parent)->topic;
	newelem->topic->page->childmap->add((long) newelem, (long) elem);

	newelem->elemtext = new DCgroup(*(elem->elemtext), newelem->groupmap);
	if (!parent->elemtext)
		LogEvent(logwarn, 1, "Parent elemtext not set.");
	//else 
	//	parent->elemtext->replace(*(newelem->elemtext), *(elem->elemtext));
	else {
		DCgroup *oldtext = elem->elemtext;
		parent->setgroup(&oldtext);
		((DCtopelem *) parent)->topic->page->setgroup(&oldtext);
		if (!parent->elemtext->replace(*(newelem->elemtext), *oldtext))
			LogEvent(logwarn, 2, "Child elemtxt not replaced.");
	}
		//LogEvent(logwarn, 1, "Child elemtext not replaced in parent elemtext.");
		// because it doesn't belong there  parent->elemtext->add(*(newelem->elemtext));
		//if (!newelem->lasttext->replace(*(newelem->elemtext), *(elem->elemtext)))
		//	LogEvent(logwarn, 1, "Child elemtext not replaced in lasttext.");

	newelem->lasttext = parent->elemtext;

	newelem->parent = parent;
	if (!parent->children->replace((long) newelem, (long) elem))
		LogEvent(logwarn, 1, "Child not replaced by parent.");

	newelem->topic = ((DCtopelem *) parent)->topic;
	newelem->topicnum = ((DCtopelem *) parent)->topicnum;

	if (parent->elpath) {
		newelem->elpath = NewName(strlen(parent->elpath) +
		                          strlen(newelem->elemname) + 2);
		strcpy(newelem->elpath, newelem->elemname);
		strcat(newelem->elpath, sep);
		strcat(newelem->elpath, parent->elpath);
	}
	if (newelem->epctl) {
		cp = new DCctl(ename, 2, 121, 6);
		cp->ex(newelem->elpath);
		if (!newelem->elemtext->replace(*cp, *(newelem->epctl)))
			LogEvent(logwarn, 2, "Child elpath not replaced.");
		newelem->epctl = cp;
	}
	if (parent->elfpath) {
		newelem->elfpath = NewName(strlen(parent->elfpath) +
		                          strlen(newelem->elemname) + 2);
		strcpy(newelem->elfpath, newelem->elemname);
		strcat(newelem->elfpath, sep);
		strcat(newelem->elfpath, parent->elfpath);
	}
	if (newelem->epfctl) {
		cp = new DCctl(ename, 2, 121, 6);
		cp->ex(newelem->elfpath);
		if (!newelem->elemtext->replace(*cp, *(newelem->epfctl)))
			LogEvent(logwarn, 2, "Child elfpath not replaced.");
		newelem->epfctl = cp;
	}

	if (newelem->formctl) {
		newelem->elform = GetElemForm(newelem, newelem->outclass,
			newelem->elemname, newelem->elinlin, newelem->topic->tlevel);
		cp = SetFormat(newelem->elform, newelem->elinlin, false);
		if (cp->siz() != newelem->formctl->siz()) {
			if (!newelem->elemtext->replace(*cp, *(newelem->formctl)))
				LogEvent(logwarn, 2, "Child formctl not replaced.");
			newelem->formctl = cp;
		}
	}

	if (newelem->anumtxt) {
		newelem->anumtxt = new DCgroup(DCctl(group, 2, 10, 3));
		DCgroup *oldanum = elem->anumtxt;
		newelem->setgroup(&oldanum);
		if (!newelem->elemtext->replace(*(newelem->anumtxt), *oldanum)
		 && (((oldanum = GetAnumTextGroup(newelem->elemtext)) == NULL)
		  || !newelem->elemtext->replace(*(newelem->anumtxt), *oldanum)))
			LogEvent(logwarn, 2, "Child anumtxt not replaced.");
		char *nform = FmtFile->GetNumFmtName(newelem->elform);
		newelem->elemnf = GetNumFormat(nform);
	}

	ReplaceGraphic(newelem->topic, (DCtopelem *) elem, newelem);

	ReplaceUniqueIDs(newelem);

	AddOneConrefAnum(CurrConrefT2TReplacement, newelem, elem);

#if 0
	if (MakeConrefLists) {
		if (newelem->anumtxt)
			CurrConrefT2TReplacement->anums.append((long) newelem);
		if (newelem->inLOF)
			CurrConrefT2TReplacement->lofitems.append((long) newelem);
		if (newelem->inLOT)
			CurrConrefT2TReplacement->lotitems.append((long) newelem);
	}
	else {
		DClist *lp = NULL;
		if (newelem->anumtxt
		 && ((lp = CurrConrefT2TReplacement->anums.find((long) elem)) != NULL))
			lp->id = (long) newelem;
		if (newelem->inLOF
		 && ((lp = CurrConrefT2TReplacement->lofitems.find((long) elem)) != NULL))
			lp->id = (long) newelem;
		if (newelem->inLOT
		 && ((lp = CurrConrefT2TReplacement->lotitems.find((long) elem)) != NULL))
			lp->id = (long) newelem;
	}
#endif

	FixConrefGenLists(elem, newelem);

	ReplaceChildren(newelem);
}


DCgroup *
XMLrd::GetAnumTextGroup(DCgroup *pgp)
{
	DCgroup *gp = NULL;
	DCitem *ig = NULL;

	if (!pgp
	 || !pgp->first)
		return NULL;

	for (ig = pgp->first; ig; ig = ig->next) {
		if (ig->ctl->dt() == group) {
			if ((ig->ctl->maj() == 2)
			 && (ig->ctl->min() == 10)
			 && (ig->ctl->dat() == 3))
				return (DCgroup *) ig->ctl;
			if ((ig->ctl->maj() == 0)
			 && (ig->ctl->min() == 0)
			 && (ig->ctl->dat() == 0)
			 && ((gp = GetAnumTextGroup((DCgroup *) ig->ctl)) != NULL))
				return gp;
		}
	}

	return NULL;
}


void
XMLrd::AdjustConrefAttrs(DCelem *eorig, DCelem *enew, DCgroup *newgrp)
{
	long oi = 1;
	long ni = 1;
	long ocnt = eorig->attrcnt;
	long ncnt = enew->attrcnt;

	// make up new attrgrp with merged values, using rules below,
	// newgrp is empty and in place for merged attrs
	// leave previous enew->attrgrp alone in case used in another conref

	// rules for merge are:  orig replaces new,
	//  unless orig is "-dita-use-conref-target", then use new
	// if both orig and new are "-dita-use-conref-target", remove attr
	// if attr is in one set but not the other, use it

	// make indexes of names and values for orig and new
	DCnlist *onames = eorig->attrnames;
	DCnlist *nnames = enew->attrnames;
	DCnlist *ovals = eorig->attrvals;
	DCnlist *nvals = enew->attrvals;
	DCctl *cp = NULL;
	char *name = NULL;
	DCctl *val = NULL;
	char *oval = NULL;
	char *nval = NULL;

	// step though orig, if matching new found resolve value, else use orig
	for (oi = 1; oi <= ocnt; oi++) {
		cp = eorig->attrnctls->find(oi);
		name = onames->find(oi);
		if (!strcmp(name, "conref"))  // drop original conref attr
			continue;
		val = eorig->attrvctls->find(oi);  // default is to use original
		if ((ni = nnames->getitem(name)) != 0) {  // matching name
			oval = ovals->find(oi);
			nval = nvals->find(ni);
			if (!strcmp(oval, "-dita-use-conref-target")) {
				if (!strcmp(nval, "-dita-use-conref-target"))
					continue;
				val = enew->attrvctls->find(ni);  // override with new value
			}
		}
		newgrp->add(*cp);
		newgrp->add(*val);
	}

	// step through new, if matching orig skip, else add new
	for (ni = 1; ni <= ncnt; ni++) {
		cp = enew->attrnctls->find(ni);
		name = nnames->find(ni);
		if (onames->getitem(name) != 0)  // matching, already processed
			continue;
		val = enew->attrvctls->find(ni);
		newgrp->add(*cp);
		newgrp->add(*val);
	}
}



// map to topic conrefs


DCcnref *XMLrd::CurrConrefM2TInfo = NULL;
DCcnref *XMLrd::CurrConrefM2TReplacement = NULL;
bool XMLrd::ProcessingM2TConrefs = false;
long XMLrd::ConrefM2TCount = 0;
long XMLrd::ConrefsM2TProcessed = 0;
DCvlist XMLrd::ConrefsM2T;


void 
XMLrd::SetM2TConrefInfo(DCelem *elem, bool push)
{
	// pull: topic referencing elem in map
	// push: map putting content into topic
	// either way, process with T2T conrefs
	CurrConrefM2TInfo = new DCcnref(++ConrefM2TCount,
	 ctm2t, ConrefsM2T, elem);

	if (push) {
		CurrConrefM2TInfo->srcset = CurrMapInfo;
		CurrConrefM2TInfo->srcelem = elem;
	}
	else {
		CurrConrefM2TInfo->destset = CurrTopicInfo;
		CurrConrefM2TInfo->destelem = elem;
	}

	SetConrefConds(CurrConrefM2TInfo);
	if (!push)
		SetConrefPath(elem, false);
}


void 
XMLrd::ProcessM2TConrefs(XMLrfile *rf)
{
		// for conrefs, insert entire element content in group
		//  or treat conref as a variable and put it in a variable def
	long i = 0;

	DCcnref *cinfo = NULL;

	DCelemset **targetset = NULL;
	DCelem **targetelem = NULL;
	cnerrtyp err = noerr;

	char *branch = CurrBranch;
	conditions *conds = CurrCondSet;

	ProcessingM2TConrefs = ProcessingConrefs = true;
	AnumElemList = &ConrefAnumList;
	LOFItemList = &ConrefLOFList;
	LOTItemList = &ConrefLOTList;

	for (i = ConrefsM2TProcessed + 1; i <= ConrefM2TCount; i++) {
		cinfo = (DCcnref *) ConrefsM2T.find(i);
		CurrConrefM2TReplacement = cinfo;
		CurrBranch = cinfo->branch;
		CurrCondSet = cinfo->conds;

		if (cinfo->action) { // push from map tp topic, find topic
			ConrefPull = false;
			targetset = &(cinfo->destset);
			targetelem = &(cinfo->destelem);
			if (GetTopicConrefTarget(rf, cinfo, targetset, targetelem, err)) {
				switch (cinfo->action) {
					case ca_replace:
						ReplaceM2TElement(cinfo);
						break;

					default:
						err = noact;
						ProcConrefFailure(cinfo, err);
						break;
				}
			}
			else
				ProcConrefFailure(cinfo, err);
		}
		else {  // pull from map to topic, find map
			ConrefPull = true;
			targetset = &(cinfo->srcset);
			targetelem = &(cinfo->srcelem);
			if (GetMapConrefTarget(rf, cinfo, targetset, targetelem, err))
				ReplaceM2TElement(cinfo);
			else
				ProcConrefFailure(cinfo, err);
		}

		if (cinfo->destset->titlechanged)
			UpdateTopicTitle((DCtopic *) (cinfo->destset));
	}
	ConrefsM2TProcessed = ConrefM2TCount;
	ProcessingM2TConrefs = ProcessingConrefs = false;
	AnumElemList = &AnumElems;
	LOFItemList = &LOFItems;
	LOTItemList = &LOTItems;
	CurrBranch = branch;
	CurrCondSet = conds;
}


void 
XMLrd::ReplaceM2TElement(DCcnref *cinfo)
{
	// replace elem in topic with elem from map
	if (cinfo->action)
		LogEvent(loginfo, 2, "Push replacing topic element with map conref ", *cinfo->href);
	else
		LogEvent(loginfo, 2, "Replacing topic element with map conref ", *cinfo->href);
	LogEvent(logcont, 2, "in: ", cinfo->xrname);

	DCelemset *srctop = cinfo->srcset;
	DCtopic *desttop = (DCtopic *) cinfo->destset;
	DCelem *srcelem = cinfo->srcelem;
	DCtopelem *destelem = (DCtopelem *) cinfo->destelem;
	DCtopelem *newelem = new DCtopelem(*srcelem);

	ReplaceTopicElement(srctop, srcelem, desttop, destelem, newelem);
	AdjustConreffedElement(cinfo, newelem);
}




// map to map conrefs

DCcnref *XMLrd::CurrMapConrefInfo = NULL;
DCcnref *XMLrd::CurrMapConrefReplacement = NULL;
bool XMLrd::ProcessingMapConrefs = false;
long XMLrd::MapConrefCount = 0;
long XMLrd::MapConrefsProcessed = 0;
DCvlist XMLrd::MapConrefs;


void
XMLrd::CheckMapConref(DCelem *elem)
{
	if ((elem = CheckElemConact(elem)) == NULL)
		return;

	if (elem->conref || elem->conkeyref) {
		if (!elem->checkType((epTopic << 16) | (epKey << 8) | epRef)) {
			// not for keydefs
			if (elem->conref
			 && !FileIsMap(elem->conref)
			 && !elem->conkeyref) {
				if (elem->conact)
					// push, process with T2T
					SetM2TConrefInfo(elem, true);
				else
					// pull, process with M2M from resource topic
					SetT2MConrefInfo(elem, false);
			}
			else
				SetMapConrefInfo(elem, (elem->conact != NULL));
		}
	}
}


void 
XMLrd::SetMapConrefInfo(DCelem *elem, bool push)
{
	CurrMapConrefInfo = new DCcnref(++MapConrefCount, ctm2m,
	                             MapConrefs, elem);
	if (push) {
		CurrMapConrefInfo->srcset = CurrMapInfo;
		CurrMapConrefInfo->srcelem = elem;
	}
	else {
		CurrMapConrefInfo->destset = CurrMapInfo;
		CurrMapConrefInfo->destelem = elem;
	}
	SetConrefConds(CurrMapConrefInfo);
	if (!push)
		SetConrefPath(elem, true);
}


void 
XMLrd::ProcessMapConrefs(XMLrfile *rf)
{
	// conrefs from a map element to a map element
	long i = 0;

	DCcnref *cinfo = NULL;

	DCelemset **targetset = NULL;
	DCelem **targetelem = NULL;
	cnerrtyp err = noerr;

	ProcessingMapConrefs = ProcessingConrefs = true;

	for (i = MapConrefsProcessed + 1; i <= MapConrefCount; i++) {
		ParaStarted = false;
		cinfo = (DCcnref *) MapConrefs.find(i);
		CurrMapConrefReplacement = cinfo;

		if (cinfo->action) { // push
			err = nom2mpush;
			ProcConrefFailure(cinfo, err);
#if 0
			ConrefPull = false;
			targetset = &(cinfo->destset);
			targetelem = &(cinfo->destelem);
			if (GetMapConrefTarget(rf, cinfo, targetset, targetelem, err)) {
				CurrMapRef = (DCmapelem *) cinfo->destelem;
				CurrMapLevel = CurrMapRef->mlevel;
				CurrMapPos = CurrMapRef->mcount;
				switch (cinfo->action) {
					case ca_replace:
						if ((*targetelem)->elemnum == 1)
							ReplaceMap(cinfo, i);
						else
							ReplaceM2MElement(cinfo);
						break;

					default:
						err = noact;
						ProcConrefFailure(cinfo, err);
						break;
				}
			}
			else
				ProcConrefFailure(cinfo, err);
#endif
		}
		else {
			ConrefPull = true;
			targetset = &(cinfo->srcset);
			targetelem = &(cinfo->srcelem);
			if (GetMapConrefTarget(rf, cinfo, targetset, targetelem, err)) {
				CurrMapRef = (DCmapelem *) cinfo->destelem;
				CurrMapLevel = CurrMapRef->mlevel;
				CurrMapPos = CurrMapRef->mcount;
				if ((*targetelem)->elemnum == 1)
					ReplaceMap(cinfo, i);
				else
					ReplaceM2MElement(cinfo);
			}
			else
				ProcConrefFailure(cinfo, err);
		}

		while (MapsToGo.next) { // process new maps immediately
			ParaStarted = false;
			ProcessingMapConrefs = ProcessingConrefs = false;
			ParseMap(rf, (char **) MapsToGo.dequeue());
			ProcessingMapConrefs = ProcessingConrefs = true;
		}
	}
	MapConrefsProcessed = MapConrefCount;
	ProcessingMapConrefs = ProcessingConrefs = false;
	ParaStarted = false;
}


bool
XMLrd::GetMapConrefTarget(XMLrfile *rf, DCcnref *cinfo,
	 DCelemset **targetset, DCelem **targetelem, cnerrtyp &err)
{
	char *hfdoc = NULL;
	char *hftopic = NULL;
	char *hfelem = NULL;

	long mapnum = 0;
	DCmap *minfo = NULL;
	long elnum = 0;
	DCmapelem *einfo = NULL;

	// see if in doc already read
	hfdoc = hftopic = hfelem = NULL;
	ParseHref(*cinfo->href, &hfdoc, &hftopic, &hfelem);
#if 0
	if ((mapnum = MapNamesProcessed.getitem(hfdoc)) != 0) {
		if ((minfo = (DCmap *) MapInfos.find(mapnum)) != NULL) {
			if (hfelem) {           // ref is to a specific element
				if (((elnum = minfo->elemids->getitem(hfelem)) != 0)
				 && ((einfo = (DCmapelem *) minfo->elems->find(elnum)) != NULL)) {
					*targetset = minfo;
					*targetelem = einfo;
					return true;
				}
				else {
					err = nomapelem;
					return false;
				}
			}
			else {  // ref is to entire map
				if ((einfo = (DCmapelem *) minfo->elems->find(1)) != NULL) {
					*targetset = minfo;
					*targetelem = einfo;
					return true;
				}
				else {
					err = nomap;
					return false;
				}
			}
		}
		else { // else in processed map, but mapinfo missing
			err = nomapinfo;
			return false;
		}
	}
	else {  // not in processed map, get it and parse it all
#endif
		if (ConrefPull)
			CurrMapRef = (DCmapelem *) cinfo->destelem;
		else
			CurrMapRef = (DCmapelem *) cinfo->srcelem;
		SkipMapRef = false;
		SkipMapRefDepth = 0;
		ParseMap(rf, cinfo->href);
		SkipMapRef = false;
		SkipMapRefDepth = 0;

		if (rf->FErr() != fok) {
			err = nodoc;
			return false;
		}
		else if (!*targetset) {
			err = nomap;
			return false;
		}
		else if (!*targetelem) {
			err = nomapelem;
			return false;
		}
		else {  // found it, now replace
			return true;
		}
//	}
}


void
XMLrd::CheckMapConrefReplacement(DCmapelem *elem, bool isRoot,
 const char **pnavattr, int depth)
{
	if (CurrMapConrefReplacement->action == ca_pull) {  //pull
		DCmapelem *cmdest = (DCmapelem *) (CurrMapConrefReplacement->destelem);
		if (isRoot)
			CurrMapConrefReplacement->srcset = CurrMapInfo;
		if ((!DocElement  // conref replaces map
			&& isRoot)
		 || (DocElement   // conref replaces element
			&& elem->id
			&& !strcmp(DocElement, elem->id))) {
			CurrMapConrefReplacement->srcelem = CurrElemInfo;
			if (cmdest->navtitle)
				*pnavattr = cmdest->navtitle;
			if (cmdest->chunkby)
				elem->chunkby = cmdest->chunkby;
			if (cmdest->chunksel)
				elem->chunksel = cmdest->chunksel;
			if (cmdest->chunkto)
				elem->chunkto = cmdest->chunkto;
			SkipMapRef = false;
			SkipMapRefDepth = 0;
			ParaStarted = false;
		}
		// else exclude from adding to DocsToGo and MapsToGo
		else if (!CurrMapConrefReplacement->srcelem
		 || !CurrMapConrefReplacement->srcelem->isDescendant(elem)) {
			SkipMapRef = true;
			SkipMapRefDepth = depth;
		}
	}
	else {  // push replace
		DCmapelem *cmsrc = (DCmapelem *) (CurrMapConrefReplacement->srcelem);
		if (isRoot)
			CurrMapConrefReplacement->destset = CurrMapInfo;
		if ((!DocElement  // conref replaces map
			&& isRoot)
		 || (DocElement   // conref replaces element
			&& elem->id
			&& !strcmp(DocElement, elem->id))) {
			CurrMapConrefReplacement->destelem = CurrElemInfo;
			if (cmsrc->navtitle)
				*pnavattr = cmsrc->navtitle;
			if (cmsrc->chunkby)
				elem->chunkby = cmsrc->chunkby;
			if (cmsrc->chunksel)
				elem->chunksel = cmsrc->chunksel;
			if (cmsrc->chunkto)
				elem->chunkto = cmsrc->chunkto;
			SkipMapRef = false;
			SkipMapRefDepth = 0;
			ParaStarted = false;
		}
		// else exclude from adding to DocsToGo and MapsToGo
		else if (!CurrMapConrefReplacement->destelem
		 || !CurrMapConrefReplacement->destelem->isDescendant(elem)) {
			SkipMapRef = true;
			SkipMapRefDepth = depth;
		}
	} 
}


void
XMLrd::ProcessTopicrefConref(DCmapelem *elem, bool mapref, bool replace)
{
	DCmapelem *cmdest = (DCmapelem *) (CurrMapConrefReplacement->destelem);
	long mcount = cmdest->mcount;
	long dcount = cmdest->dcount;
	if (mapref) {
		elem->mcount = ++MapCount;
		MapsToGo.append(&elem->href, MapCount);
		MapRefs.append(elem, MapCount);
	}
	else
		elem->dcount = ++DocCount;

	if (CurrMapPos
	 || MapCount) {  // processing submap. insert in place
		// make original entry into a null flag, then insert 
		// the new entries in front of it
		if (mapref) { 
			// insert before marker put in by this map
			DocsToGo.insertat(NULL, -MapCount, mcount? -mcount : dcount);
			DocRefs.insertat(NULL, -MapCount, mcount? -mcount : dcount);
		}
		else {
			// insert before marker put in by this map
			if (replace) {  // zap previous one for pushreplace
				DocsToGo.replace(NULL, dcount);
				DocRefs.replace(NULL, dcount);
			}
			DocsToGo.insertat(&elem->href, elem->dcount, dcount);
			DocRefs.insertat(elem, elem->dcount, dcount);
		}
	}
	else {  // processing initial map, append
		if (mapref) { 
			// insert marker for position of this map
			DocsToGo.append(NULL, -MapCount);
			DocRefs.append(NULL, -MapCount);
		}
		else {
			if (elem == CurrMapConrefReplacement->srcelem) {
				DocsToGo.replace(NULL, dcount);
				DocRefs.replace(NULL, dcount);
			}
			DocsToGo.insertat(&elem->href, elem->dcount, dcount);
			DocRefs.insertat(elem, elem->dcount, dcount);
		}
	}
}


void 
XMLrd::ReplaceMap(DCcnref *cinfo, long citem)
{
	LogEvent(loginfo, 2, "Replacing conref map ", *cinfo->href);
	LogEvent(logcont, 2, "in: ", cinfo->xrname);

	LogEvent(logwarn, 1, "Full map conref not available.");

#if 0
	// clone topic and its page, with new DCtopic and DCelem root element,
	// but retain pointers to the rest of the elements
	// with default copy constructor
	DCtopic *newtop = new DCtopic(*(cinfo->topnew));
	newtop->topicnum = ++CurrTopicNum;
	CurrTopicInfo = newtop;

	DCtopelem *newroot = new DCtopelem(*(cinfo->elemnew));
	DCvlist *newelems = new DCvlist();
	newelems->add(newroot, 1);
	newelems->next->next = newtop->elems->next->next;
	newtop->elems = newelems;
	CurrElemInfo = newroot;
	newroot->topicnum = CurrTopicNum;
	newroot->topic = CurrTopicInfo;

	DCpage *newpage = new DCpage(*(cinfo->topnew->page));
	newtop->page = CurrPageInfo = newpage;
	newtop->pagenum = newpage->pagenum = ++PageNum;
	Pages.add(CurrPageInfo, PageNum);
	PageGroup = new DCgroup(*(CurrPageInfo->pgroup));
	CurrPageInfo->pgroup = PageGroup;
	PageList.add(PageGroup, PageNum);

	// replace page used by toporig with page used by topnew in PageSequence
	uns origpage = (uns) cinfo->toporig->pagenum;
	long seqpage = PageSequence(origpage);
	PageSequence.replace(PageNum, seqpage);
	RemovedPages.add(origpage);

	// replace conref src id with orig id
	DCgroup *origidgrp = cinfo->elemorig->idgrp;
	if (!origidgrp)  // if no orig id, replace with empty group
		origidgrp = new DCgroup();
	DCgroup *newidgrp = newroot->idgrp;
	if (origidgrp && newidgrp)
		PageGroup->replace(*origidgrp, *newidgrp);

	// adjust attrs for copy of conref'd topic's root element
	if (IncludeElementTags) {
		DCctl *origagrp = (DCctl *) cinfo->elemnew->attrgrp;
		DCgroup *newagrp = new DCgroup();
		newroot->attrtext = PageGroup;
		if (origagrp && newagrp)
			PageGroup->replace(*newagrp, *origagrp);
		AdjustConrefAttrs(cinfo->elemorig, newroot, newagrp);
	}

	// correct any conrefs in topnew to point at newtop
	DCtopcnref *ci = NULL;
	while ((ci = (DCtopcnref *) Conrefs.find(++citem)) != NULL) {
		if (ci->toporig == cinfo->topnew)
			ci->toporig = newtop;
	}
#endif
}


void 
XMLrd::ReplaceM2MElement(DCcnref *cinfo)
{
	if (cinfo->action)
		LogEvent(loginfo, 2, "Push replacing conref map element ", *cinfo->href);
	else
		LogEvent(loginfo, 2, "Replacing conref map element ", *cinfo->href);
	LogEvent(logcont, 2, "in: ", cinfo->xrname);

	DCmap *srcmap = (DCmap *) cinfo->srcset;
	DCmap *destmap = (DCmap *) cinfo->destset;
	DCmapelem *srcelem = (DCmapelem *) cinfo->srcelem;
	DCmapelem *destelem = (DCmapelem *) cinfo->destelem;
	DCmapelem *newelem = new DCmapelem(*srcelem);

	ReplaceMapElement(srcmap, srcelem, destmap, destelem, newelem);

	if (!srcelem->crefnew)
		srcelem->crefnew = newelem;

	if ((cinfo->action == ca_replace)
	 && strstr(destelem->classatt, " map/topicref "))
		ProcessTopicrefConref(newelem, false, true);
	if (srcelem->dcount)
		DocRefs.replace(newelem, srcelem->dcount);

	if (srcelem->reftop
	 && !newelem->reftop)
		newelem->reftop = srcelem->reftop;
	if (destelem->reftop
	 && !newelem->reftop)
		newelem->reftop = destelem->reftop;
}


void 
XMLrd::ReplaceMapElement(DCelemset *srcmap, DCelem *srcelem,
 DCmap *destmap, DCmapelem *destelem, DCmapelem *newelem)
{
	// replace elem struct with new one
	newelem->parent = destelem->parent;
	newelem->parent->children->replace((long) newelem, (long) destelem);
	newelem->map = destelem->map;
	newelem->mapnum = destelem->mapnum;
	newelem->elemnum = destelem->elemnum;
	newelem->map->elems->replace(newelem, newelem->elemnum); // id is same
	newelem->abreset = destelem->abreset;

	// replace group containing elemorig with group containing elemnew
	// both have an elemtext group set, lasttext is parent group
	DCgroup *origgrp = destelem->elemtext;
	newelem->elemtext = new DCgroup(*(srcelem->elemtext), newelem->groupmap);
	DCgroup *textgrp = newelem->lasttext = destelem->lasttext;
	if (!textgrp)  // NULL means elem's parent is the PageGroup
		textgrp = newelem->lasttext = destmap->page->pgroup;
	if (origgrp && newelem->elemtext && textgrp) {
		if (!textgrp->replace(*(newelem->elemtext), *origgrp))
			LogEvent(logwarn, 1, "New map elem elemtext not replaced.");
	}
	if (InIgnoreGroup)
		InIgnoreGroup = false;
	else if (destelem->eignore)
		destelem->eignore->maj((uns) 0);


	// replace conref src id with orig id
	DCgroup *newidgrp = srcelem->idgrp;
	newelem->setgroup(&newidgrp);
	if ((newelem->idgrp = destelem->idgrp) == NULL)
		newelem->idgrp = new DCgroup(); // if no orig id, use empty group
	else if (newelem->idgrp && newidgrp && newelem->elemtext) {
		if (!newelem->elemtext->replace(*(newelem->idgrp), *newidgrp))
			LogEvent(logwarn, 2, "New map elem idgrp not replaced.");
	}

	// adjust attrs for copy of conref'd element
	if (IncludeElementTags) {
		DCgroup *origagrp = srcelem->attrgrp;
		newelem->setgroup(&origagrp);
		newelem->attrgrp = new DCgroup(*(destelem->attrgrp));
		//newelem->attrtext = destelem->attrtext;
		if (origagrp && newelem->attrgrp && newelem->elemtext) {
			if (!newelem->elemtext->replace(*(newelem->attrgrp), *origagrp))
				LogEvent(logwarn, 2, "New map elem attrgrp not replaced.");
		}
		AdjustConrefAttrs(destelem, srcelem, newelem->attrgrp);
	}

	// adjust formats used within conref'd element
	newelem->elpath = destelem->elpath;
	if (srcelem->epctl && destelem->epctl) {
		if (!newelem->elemtext->replace(*(destelem->epctl),
		                                *(srcelem->epctl)))
			LogEvent(logwarn, 2, "New map elem epctl not replaced.");
	}
	if (srcelem->formctl)
		newelem->formctl = new DCctl(*(srcelem->formctl));
	newelem->elform = srcelem->elform;
	newelem->outclass = srcelem->outclass;
	RecalcFormat(destelem, newelem);

	//RecalcFormat(destelem, srcelem);

	if (srcelem->children) {
		newelem->children = new DClist(*(srcelem->children));
		// recursively create new children from the old ones
		ReplaceChildren(newelem);
	}
}


void
XMLrd::ReplaceChildren(DCmapelem *elem)
{
	if (!elem->children)
		return;
	long cnt = elem->children->count();
	for (long i = 1; i <= cnt; i++)
		ReplaceChild( (DCmapelem *) elem->children->number(i), elem);
}


void
XMLrd::ReplaceChild(DCmapelem *elem, DCmapelem *parent)
{
	DCctl *cp = NULL;

	DCmapelem *newelem = new DCmapelem(*elem);
	if (newelem->id)
		newelem->elemtext = new DCgroup(*(elem->elemtext), newelem->groupmap);
	else
		newelem->elemtext = parent->elemtext;

	newelem->parent = parent;
	parent->children->replace((long) newelem, (long) elem);
	newelem->map = parent->map;
	newelem->mapnum = parent->mapnum;
	if (parent->elpath) {
		newelem->elpath = NewName(strlen(parent->elpath) +
		                          strlen(newelem->elemname) + 2);
		strcpy(newelem->elpath, newelem->elemname);
		strcat(newelem->elpath, "/");
		strcat(newelem->elpath, parent->elpath);
	}
	if (newelem->epctl) {
		cp = new DCctl(ename, 2, 121, 6);
		cp->ex(newelem->elpath);
		if (!newelem->elemtext->replace(*cp, *(newelem->epctl)))
			LogEvent(logwarn, 2, "Child epctl not replaced.");
		newelem->epctl = cp;
	}
	if (newelem->formctl
	 && newelem->elemtext) {
		char *form = GetElemForm(newelem, newelem->outclass,
			                   newelem->elemname, newelem->elinlin);
		cp = SetFormat(form, newelem->elinlin, false);
		if (!newelem->elemtext->replace(*cp, *(newelem->formctl)))
			LogEvent(logwarn, 2, "Child formctl not replaced.");
		newelem->formctl = cp;
	}

	ReplaceChildren(newelem);
}



// topic to map conrefs

DCcnref *XMLrd::CurrConrefT2MInfo = NULL;
DCcnref *XMLrd::CurrConrefT2MReplacement = NULL;
bool XMLrd::ProcessingT2MConrefs = false;
long XMLrd::ConrefT2MCount = 0;
long XMLrd::ConrefsT2MProcessed = 0;
DCvlist XMLrd::ConrefsT2M;


void 
XMLrd::SetT2MConrefInfo(DCelem *elem, bool push)
{
	// pull during M2M processing from resource topic
	// push is error, bacause map processing completed

	CurrConrefT2MInfo = new DCcnref(++ConrefT2MCount,
	 ctt2m, ConrefsT2M, elem);  // elem is a map dest elem

	if (push) {
		CurrConrefT2MInfo->srcset = CurrTopicInfo;
		CurrConrefT2MInfo->srcelem = elem;
	}
	else {
		CurrConrefT2MInfo->destset = CurrMapInfo;
		CurrConrefT2MInfo->destelem = elem;
	}

	SetConrefConds(CurrConrefT2MInfo);
	if (!push)
		SetConrefPath(elem, true);
}


void 
XMLrd::ProcessT2MConrefs(XMLrfile *rf)
{
	// pull conrefs to a dest map element from a src topic element
	// must be one allowed in both, like title or ph
	long i = 0;

	DCcnref *cinfo = NULL;
	DCelemset **targetset = NULL;
	DCelem **targetelem = NULL;
	cnerrtyp err = noerr;

	ProcessingT2MConrefs = ProcessingConrefs = true;

	for (i = ConrefsT2MProcessed + 1; i <= ConrefT2MCount; i++) {
		ParaStarted = false;
		cinfo = (DCcnref *) ConrefsT2M.find(i);
		CurrConrefT2MReplacement = cinfo;
		err = not2m;
		ProcConrefFailure(cinfo, err);
#if 0
		if (cinfo->action != ca_pull) { // push is error, too late
			ConrefPull = false;
			err = not2mpush;
			ProcConrefFailure(cinfo, err);
			continue;
		}

		ConrefPull = true;
		targetset = &(cinfo->srcset);
		targetelem = &(cinfo->srcelem);

		if (GetT2MConrefTarget(rf, cinfo, targetset, targetelem, err)) 
			ReplaceT2MElement(cinfo);
		else
			ProcConrefFailure(cinfo, err);
#endif
	}
	ConrefsT2MProcessed = ConrefT2MCount;
	ProcessingT2MConrefs = ProcessingConrefs = false;
	ParaStarted = false;
}


bool 
XMLrd::GetT2MConrefTarget(XMLrfile *rf, DCcnref *cinfo,
	 DCelemset **targetset, DCelem **targetelem, cnerrtyp &err)
{
	// no docs read yet

	CurrDocRef = (DCmapelem *) (cinfo->destelem); // may not be topicref
	ParseDocument(rf, cinfo->href, true);
	if (rf->FErr() != fok) {
		err = nodoc;
		return false;
	}
	else if (!*targetset) {
		err = notopic;
		return false;
	}
	else if (!*targetelem) {
		err = noelem;
		return false;
	}
	else   // found it, now replace
		return true;
}


void 
XMLrd::ReplaceT2MElement(DCcnref *cinfo)
{
	// replace elem in map with elem from topic
	if (cinfo->action)
		LogEvent(loginfo, 1, "Push replacing map element with conref ", *cinfo->href);
	else
		LogEvent(loginfo, 1, "Replacing map element with conref ", *cinfo->href);
	LogEvent(logcont, 1, "in: ", cinfo->xrname);

	DCelemset *srctop = cinfo->srcset;
	DCmap *destmap = (DCmap *) cinfo->destset;
	DCelem *srcelem = cinfo->srcelem;
	DCmapelem *destelem = (DCmapelem *) cinfo->destelem;
	DCmapelem *newelem = new DCmapelem(*srcelem);

	ReplaceMapElement(srctop, srcelem, destmap, destelem, newelem);
}




// end of drxmlcn.cpp

