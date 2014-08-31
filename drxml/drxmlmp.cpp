// drxmlmp.cpp is the code for dcl XML map reading, JHG
// Copyright (c) 2008 by Omni Systems, Inc.  All rights reserved.

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
#include <windows.h>
#include <iostream.h>
#include "Shlwapi.h"


DCnlist XMLrd::MapElems;
DCvlist XMLrd::MapElemInfo;
DCmapelem *XMLrd::DefMapElemInfo = NULL;
long XMLrd::MapElemCount = 0;
long XMLrd::MapElemsStarted = 0;
DCilist XMLrd::MapAttrNumStartList;
DCilist XMLrd::MapAttrNumEndList;
DCnlist XMLrd::MapAttrNameList;
DCnlist XMLrd::MapAttrValList;
long XMLrd::MapAttrNum;
DCmapelem *XMLrd::CurrTopicref = NULL;
DCmapelem *XMLrd::CurrDocref = NULL;

bool XMLrd::IsBookmap = false;
bool XMLrd::BookFrontMatter = false;
bool XMLrd::BookFrontList = false;
bool XMLrd::BookChapter = false;
bool XMLrd::BookAppendix = false;
bool XMLrd::InReltable = false;
bool XMLrd::InMapTopicmeta = false;
bool XMLrd::InCodedocref = false;
bool XMLrd::InExternDoc = false;
bool XMLrd::TopicheadsHaveNavtitles = false;
bool XMLrd::LockAllNavtitles = false;
bool XMLrd::UseAllInTOC = false;
bool XMLrd::SkipMapRef = false;
long XMLrd::SkipMapRefDepth = 0;

//bool XMLrd::IncludeMaps = false;
bool XMLrd::UseMapDescriptions = false;
long XMLrd::MapDescDepth = 0;
long XMLrd::MapLevelAdjust = 0;
bool XMLrd::SplitTopicFiles = true;
bool XMLrd::DisableChunking = false;
bool XMLrd::LastChunkToCont = false;
bool XMLrd::SplitThisTopic = false;
//char *XMLrd::TOCFileName = NULL;
DCgroup *XMLrd::PreTopicmeta = NULL;
DCgroup *XMLrd::PreNavtitle = NULL;
DCgroup *XMLrd::MapNavtitle = NULL;
DCgroup *XMLrd::PreDocText = NULL;

char *XMLrd::BaseMapFormat = "Map";
char *XMLrd::LinkFormat = "Link";
//char *XMLrd::MapXrefFormat = NULL;
//char *XMLrd::MapLinkFormat = "MapLink";
bool XMLrd::InResTopic = false;

bool XMLrd::ChunkDoc = false;
bool XMLrd::ChunkDocSet = false;
bool XMLrd::ChunkTopic = false;

char *XMLrd::ChunkBy = NULL;
char *XMLrd::ChunkSel = NULL;
DCnlist XMLrd::ChunkByTypes;
npair XMLrd::InitChunkByTypes[] = {
	{ cbtopic, "topic"},
	{ cbdoc, "document"},
	NULL
};
;
DCnlist XMLrd::ChunkSelTypes;
npair XMLrd::InitChunkSelTypes[] = {
	{ cstopic, "topic"},
	{ csdoc, "document"},
	{ csbranch, "branch"},
	NULL
};




void
XMLrd::StartMapElement(const char *name, const char **atts,
	 int depth, bool isRoot)
{
	// process topicrefs to add name to DocsToGo and level to DocLevels
	DCctl *cp = NULL;
	DCmapelem *elem = (DCmapelem *) CurrElemInfo;
	long level = CurrMapLevel + (depth - 2) - MapLevelAdjust;
	if (level < 1)
		level = 1;
	bool mapref = false;
	bool locktitle = LockAllNavtitles;

	const char *navattr = NULL;
	const char *formattr = NULL;
	const char *lockattr = NULL;
	const char *chunk = NULL;
	const char *copyto = NULL;
	const char *toc = NULL;
	const char *scope = NULL;
	const char *print = NULL;
	const char *search = NULL;
	const char *conrefatt = NULL;
	const char *conkeyref = NULL;
	const char *conactatt = NULL;
	const char *keys = NULL;
	const char *keyref = NULL;
	const char *procrole = NULL;

	const char *startatt = NULL;
	//const char *typeatt = NULL;
	const char *nameatt = NULL;
	//const char *idpxatt = NULL;
	//const char *idsxatt = NULL;
	const char *queryatt = NULL;
	const char *resultatt = NULL;
	const char *groupsatt = NULL;
	attrp alist[] = {
	 {"navtitle", &navattr },
	 {"format", &formattr },
	 {"locktitle", &lockattr },
	 {"chunk", &chunk },
	 {"copyto", &copyto },
	 {"toc", &toc },
	 {"scope", &scope },
	 {"print", &print },
	 {"search", &search },
	 {"conref", &conrefatt },
	 {"conkeyref", &conkeyref },
	 {"conaction", &conactatt },
	 {"keys", &keys },
	 {"keyref", &keyref },
	 {"processing-role", &procrole },

	 // for uDoc
	 {"start", &startatt },
	 {"key", &keyref },
	 {"name", &nameatt },
	 //{"idpx", &idpxatt },
	 //{"idsx", &idsxatt },
	 {"query", &queryatt },
	 {"results", &resultatt },
	 {"subjects", &groupsatt },
	 {NULL, NULL }
	};

	if (atts && *atts)
		ProcAttrs(atts, alist);

	elem->eltyp = esmap;

#ifdef DITA
	if (InReltable) {
		ProcReltable(elem, atts, depth);
		return;
	}

	if (elem->checkType((epRel << 8) | epMap)) {
		InReltable = true;
		ProcReltable(elem, atts, depth);
		return;
	}

	if (procrole) {
		if (!strcmp(procrole, "normal"))
			elem->role = roleuse;
		else if (!strcmp(procrole, "resource-only"))
			elem->role = roleres;
	}
	else if (elem->parent)
		elem->role = elem->parent->role;
#endif

	if (lockattr
	 && !stricmp(lockattr, "yes"))
		locktitle = true;

	if (locktitle)
		elem->usenav = true;

	if (navattr)
		elem->navtitle = NewName(navattr);

	if (!elem->map || !elem->map->userel)
		elem->userel = false;

#ifdef DITA
	if (ProcessingMapConrefs
	 && CurrMapConrefReplacement)
		CheckMapConrefReplacement(elem, isRoot, &navattr, depth);

	// if element has conref, save its attrs and set up group
	CheckMapConref(elem);
#endif

#if 0
	// is it possible to process conkeyrefatt yet?  **** No.
	if (conkeyref)
		SetMapConrefInfo(conkeyref, true);
	else if (conrefatt)
		SetMapConrefInfo(conrefatt, false);
#endif

#if 0
	if (conrefatt || conkeyref || conactatt) {
		if (conrefatt)
			elem->conref = FixReference(conrefatt);
		if (conkeyref)
			elem->conkeyref = NewName(conkeyref);
		if (!elem->checkType((epTopic << 16) | (epKey << 8) | epRef)) {
			// not for keydefs
			if (elem->conref
			 && !FileIsMap(elem->conref)
			 && !elem->conkeyref) {
				if (conactatt)  // push
					SetM2TConrefInfo(elem, true);
				else
					SetT2MConrefInfo();
			}
			else
				SetMapConrefInfo(elem, (conactatt != NULL));
		}
	}
#endif

	if (elem->map)
		DefMapElemInfo = (DCmapelem *) (elem->map->elems->find(1));
	else
		DefMapElemInfo = NULL;

	if (SplitTopicFiles)
		elem->chunkby = cbdoc;
	else if (chunk) {
		SetChunkVal(chunk);
		elem->copyto = NewName(copyto);
	}
#ifdef UDOC
	else if (startatt
	 && !stricmp(startatt, "file"))
		elem->chunkby = cbdoc;

	if (elem->checkType((epRef << 16) | (epMap << 8) | epFile,
		                  (epTopic << 8) | epExt))
		mapref = true;
#endif

	if (formattr) {
		elem->format = NewName(formattr);
#ifdef DITA
		if (!strcmp(formattr, "ditamap"))
			mapref = true;
		else if (strcmp(formattr, "dita"))
			scope = "external";
#endif
	}
	if (mapref
	 && ((elem->href
	   && strchr(elem->href, '#'))
	  || (elem->conref
	   && strchr(elem->conref, '#'))
	  || (elem->keyref
	   && strchr(elem->keyref, '/')))) {
		LogEvent(logwarn, 1, "Do not set format=\"ditamap\" for topicref to map with element id: \"",
			                    elem->href ? elem->href :
			                    (elem->conref ? elem->conref : 
				                  (elem->keyref ? elem->keyref : "")), "\".");
		mapref = false;
	}
	else if (elem->href
	 && strstr(elem->href, ".ditamap")
	 && !mapref) {
		LogEvent(logwarn, 1, "Format not set to ditamap for topicref with href \"",
			                    elem->href, "\".");
		mapref = true;
	}
	else if (elem->href
	 && !strchr(elem->href, '#')
	 && strstr(elem->href, ".xml")
	 && !mapref) {
		// if href and ext .xml, get format (map vs. topic) by reading DOCTYPE ****
		mapref = IsDocMap(elem->href);
		if (mapref)
		LogEvent(logwarn, 1, "Format not set to ditamap for topicref with href \"",
			                    elem->href, "\".");
	}

	if (toc)
		elem->toc = (!stricmp(toc, "no")) ? false : true;
	else if (!elem->parent)
		elem->toc = true;  //UseAllInTOC;
	else
		elem->toc = ((DCmapelem *) (elem->parent))->toc;

	if (print)
		elem->usage = (!stricmp(print, "yes")) ? outall :
		              ((!stricmp(print, "printonly")) ? outprint : outweb);
	else if (!elem->parent)
		elem->usage = outall;
	else
		elem->usage = ((DCmapelem *) (elem->parent))->usage;

	if (search)
		elem->search = (!stricmp(search, "yes")) ? true : false;
	else if (!elem->parent)
		elem->search = true;
	else
		elem->search = ((DCmapelem *) (elem->parent))->search;

	if (keyref || conkeyref) {
		PendingMapKeyrefs.addnew(keyref, ++PendMapKeyrefs);
		PendingMapConkeyrefs.addnew(conkeyref, PendMapKeyrefs);
		PendingMapKeyrefElems.add(elem, PendMapKeyrefs);
		SetElemText((conkeyref != NULL), true);
	}

	DClist *flaglist = NULL;
	if ((flaglist = (DClist *) CondFlagLevels.find(CurrDepth)) != NULL)
		elem->flags = new DClist(flaglist);

	// ditamap headings to process
	// write a para for each, with the format MapN where N = depth
	const char *txt = navattr ? navattr :
		               (elem->href ? elem->href : name);
	long len = strlen(txt);

	if (SkipMapRef)
		return;

	if (elem->checkType(epText)) {
		AllowText = true;
		AllowTextLevels.add(depth);
	}

	if (InMapTopicmeta) {
		if (CurrElemInfo->checkType(epNavtitle)) {
		//if (!strcmp(name, "navtitle")) {
			FlushText();
			PreNavtitle = TextGroup;
			TextGroup = MapNavtitle = new DCgroup;
		}
	}

	if (elem->checkType((epRoot << 8) | epMap)) {
		// map or bookmap

		RecordShortdesc = false;
		MapDescNeeded = false;

		if (strstr(elem->classatt, " bookmap/bookmap "))
			IsBookmap = true;

		elem->branch = CurrBranch;
#ifdef UDOC
		//if (UMXL)
			elem->conds = XMLud::CurrConds;
		//else
#endif
#ifdef DITA
			elem->conds = CurrCondSet;
		elem->condlevel = CurrCondRefLevel++;
#endif

		//DefMapElemInfo = elem;
		DefMapElemInfo = elem->map->mref;
		if (elem->map->mref) {
			elem->abreset = elem->map->mref->abreset;
			if (!elem->map->mref->userel)
				elem->userel = elem->map->userel = false;
		}

		if (DefMapElemInfo) {
			if (elem->chunkby == cbnone)
				elem->chunkby = DefMapElemInfo->chunkby;
			if (elem->chunksel == csnone)
				elem->chunksel = DefMapElemInfo->chunksel;
			if (elem->chunkto == ctnone)
				elem->chunkto = DefMapElemInfo->chunkto;
		}

#if 0
		if (DefMapElemInfo->chunkby == cbnone)
			DefMapElemInfo->chunkby = (chunkbytyp) ChunkByTypes(ChunkBy);
		if (DefMapElemInfo->chunksel == csnone)
			DefMapElemInfo->chunksel = (chunkseltyp) ChunkSelTypes(ChunkSel);
#endif

		if (!PageStarted) {  // new referenced map
			StartPage();
			if (CurrMapRef
			 && CurrMapRef->elemtext)
				TextGroup = CurrMapRef->elemtext;
		}

		FindGlossAbbrReset(elem);
	}
#ifdef UDOC
	else if (elem->checkType((epList << 8) | epDef)) {
		MapLevelAdjust++;
		elem->branch = CurrBranch;
		elem->conds = XMLud::CurrConds;
		if (elem->checkType(epRef)) {  // list to generate
			//SetGenFileRef(elem);
			if (SplitTopicFiles
			 || (startatt
			  && !stricmp(startatt, "file")))
				elem->chunkby = cbdoc;
			elem->gendoc = true;
			ProcTopicref(elem, mapref, level);
			ProcTopicrefText(elem, depth, level, scope, locktitle, txt, len);
		}
		else { // genlist or item definition , normally in lib
		}
	}
#endif
	else if (elem->checkType((epList << 8) | epMap, (epText << 8) | epRef)) {
		// booklists, frontmatter, backmatter, bookid
		bool list = true;
		if (IsBookmap
		 && strstr(elem->classatt, " bookmap/frontmatter "))
			BookFrontMatter = true;
		if (BookFrontMatter
		 && strstr(elem->classatt, " bookmap/booklists "))
			BookFrontList = true;
		MapDescNeeded = false;
		MapLevelAdjust++;
		elem->branch = CurrBranch;
#ifdef UDOC
		//if (UMXL)
			elem->conds = XMLud::CurrConds;
		//else
#endif
#ifdef DITA
			elem->conds = CurrCondSet;
		elem->condlevel = CurrCondRefLevel++;
#endif
	}
	else if (elem->checkType((epTopic << 16) | (epKey << 8) | epRef)) {
		// keydef
#ifdef UDOC
		if (XMLud::InOutput) {
			if (!XMLud::UseOutput
			 || XMLud::OutputKeys)  // don't use output keys ourselves
				return;
			// add source keydefs to list to be updated later ****
			if (XMLud::SourceKeys) {
				XMLud::OutputKeyList.add(elem, ++XMLud::OutputKeyCount);
				//elem->keys = NewName(keys);
			}
		}
#endif
		RecordShortdesc = false;
		elem->toc = false;  // keydef does not appear in toc

		elem->branch = CurrBranch;
#ifdef UDOC
		//if (UMXL)
			elem->conds = XMLud::CurrConds;
		//else
#endif
#ifdef DITA
			elem->conds = CurrCondSet;
		elem->condlevel = CurrCondRefLevel++;
#endif
		elem->scopetxt = NewName(scope);

		ProcKeydef(elem);

		//if (procrole
		// && !strcmp(procrole, "normal")) { // also process as topicref
		if (elem->role == roleuse) {
			MapDescNeeded = true;
			CurrTopicref = elem;
			ProcTopicref(elem, mapref, level);
			FindGlossAbbrReset(elem);

			if ((!mapref && !conrefatt && !conkeyref && !elem->keyref)
			 || locktitle)
				ProcTopicrefText(elem, depth, level, scope, locktitle, txt, len);
			else
				SetMapElemTags();
		}
		else  // if no role, it is roleres
			MapDescNeeded = false;
	}
	else if (elem->checkType((epTopic << 16) | (epMap << 8) | epMeta)) {
		// topicmeta, keep for start of topic title para
		// @lockmeta default yes, if no don't cascade values down  ****
		/*
All the selection attributes and topicref-configuring attributes (@toc, @type, @format, etc.) cascade
from <map> to <topicref> and from <topicref> to <topicref> within the map document.
The metadata elements <author>, <source>, <publisher>, <copyright>, <critdates>,
<permissions>, <audience>, <category>, <prodinfo>, <othermeta> all cascade from <map>
to <topicref>, from <topicref> to <topicref> within the map document, and from <topicref>
to referenced topic.
Cascading is additive for attributes and metadata elements that allow multiple values. For attributes
and elements that allow a single value, the value defined on the nearest ancestor element takes
precedence.
Notabletop attributes that do not cascade are @chunk, @copy-to, and @outputclass, along with
the addressing attributes @href, @keyref, and @keys.
		*/
		elem->metatext = new DCgroup;
		PreTopicmeta = TextGroup;
		TextGroup = elem->metatext;
		InMapTopicmeta = true;
	}
#ifdef UDOC
	else if (elem->checkType((epFile << 16) | (epRef << 8) | epMap)
#endif
#ifdef DITA
	else if (elem->checkType((epTopic << 8) | epMap, epPara)
#endif
		) {
		// topicref, topichead, topicgroup (epNolev)
		// may also be topicset or topicsetref

		if (IsBookmap) {
			if (strstr(elem->classatt, " bookmap/chapter "))
				BookChapter = true;
			if (strstr(elem->classatt, " bookmap/appendix "))
				BookAppendix = true;
			if (!BookChapter
			 && !BookFrontList)
				elem->userel = false;
			if (BookAppendix
			 && !elem->toc)
				elem->userel = false;
		}

		//if (UMXL) {
#ifdef UDOC
			if (!elem->toc)
				elem->userel = false;
			if (elem->checkType(epPre))
				InCodedocref = true;
			if (elem->checkType(epExt))
				InExternDoc = true;
			if (((DCmapelem *) elem->parent)->groups) {  // inherit groups from parent
				if (!elem->groups)
					elem->groups = new DCnlist;
				elem->groups->addlist(((DCmapelem *) elem->parent)->groups);
			}
#endif
		//}

		bool topichead = !elem->checkType(epRef) && !elem->checkType(epGroup);
		if (!elem->href
		 && !elem->keyref
		 && !elem->idref
		 && !elem->idrefs)
			topichead = true;

#if 0
		bool reftomap = elem->checkType(epRef)
			              && !topichead
#ifdef UDOC
			              && !elem->checkType(epTopic);
#endif
#ifdef DITA
										&& (formattr
										 && !strcmp(formattr, "ditamap"));
#endif
#endif

		RecordShortdesc = false;
		bool procAsTopic = false;
		CurrTopicref = elem;

		//if (!reftomap)
		if (!mapref)
			CurrDocref = elem; // used for doctext processing
		else {
			CurrDocref = NULL;
		}
		elem->branch = CurrBranch;

#ifdef UDOC
		elem->conds = XMLud::CurrConds;
		elem->scopetxt = NewName(InExternDoc ? "external" : "local");
#endif
#ifdef DITA
		elem->conds = CurrCondSet;
		elem->condlevel = CurrCondRefLevel++;
		elem->scopetxt = NewName(scope);
#endif

		if (topichead
		 && TopicheadsHaveNavtitles
		 && !elem->navtitle) {
			txt = navattr = elem->navtitle = NewName("none");
			len = 4;
		}

		if (TreatTopicheadsAsTopics
		// && !reftomap
		 && !mapref
		 && !ProcessingConrefs
		 && !(conrefatt || conkeyref)
		 && !elem->href     // has no href
		 && (elem->navtitle  // but has a navtitle
		  || TopicheadsHaveNavtitles)
		 && !elem->keyref
		 && (!procrole      // is not a keyref
		  || strcmp(procrole, "resource-only"))
		 && !elem->checkType(epList)
		 && !elem->gendoc   // not a generated file
		 && (elem->gentype == gennone)) {
			elem->href = elem->navtitle;
			procAsTopic = true;
#ifdef DITA
			RecordShortdesc = true;
			RecList = elem->parselist = new DClist;
#endif
		}

		if (elem->checkType(epRef)
		 || procAsTopic) {  // topicref

			if (keys)
				ProcKeydef(elem);

			//if (!procrole
			// || strcmp(procrole, "resource-only")
			if ((elem->role != roleres)
			 || conactatt
			 || mapref) {

				MapDescNeeded = true;
				ProcTopicref(elem, mapref, level);
				FindGlossAbbrReset(elem);

				if (procAsTopic) {
					elem->href = CorrectFileName(elem->href);
					ProcTopicrefText(elem, depth, level, scope, locktitle, txt, len);
					elem->href = elem->navtitle;
				}
				//else if (!reftomap
				// && ((!mapref && !conrefatt && !conkeyref && !elem->keyref)
				else if (!mapref
				 && ((!conrefatt && !conkeyref && !elem->keyref)
				 || locktitle))
					ProcTopicrefText(elem, depth, level, scope, locktitle, txt, len);
				else {
					SetMapElemTags();
					elem->elpath = GetElemPath(depth, CurrMapLevel);
 					if (CurrMapLevel > 1)
						GetExtendedElemPath(elem, CurrMapLevel);  // check extended path
				}
			}
			else {  // process res-only for any conref push elements ****
				MapDescNeeded = false;
				ProcTopicref(elem, mapref, level);
			}
		}

		else {  // topichead
			PreTopicmeta = NULL;
#ifdef DITA
			if (elem->toc) {
				StartPara();
				SetMapFormat(elem, depth, level);
				SetUniqueID();
				SetMapElemTags();
				StartMapGroup(level, sqhead);
				PutMapHeadID(elem);
				PutTextStrCtl(txt, len);
				EndMapGroup(sqhead);
				MapDescNeeded = false;
			}
			else
#endif
				SetMapElemTags();

			elem->mlevel = level;
			FindGlossAbbrReset(elem);
		}
	}
#ifdef UDOC
	else if (elem->checkType((epMap << 16) | (epGroup << 8) | epNolev)) {  // branch
		CurrBranch = NewName(nameatt);
		CurrBranchLevel = CurrDepth;
		BranchLevels.replace(CurrBranch, CurrBranchLevel);
	}
	else if (elem->checkType((epMap << 16) | (epTopic << 8) | epPara)) {  // doctext
		if (CurrDocref) {  // will become CurrDocRef when doc is parsed
			DCgroup **tgp = &CurrDocref->docendtext;
			if (elem->idref) {
				doctxt *dtx = new doctxt;
				dtx->dtt = dtxafter;
				tgp = &dtx->dtgp;
				if (elem->type) {
					if (!stricmp(elem->type, "replace"))
						dtx->dtt = dtxreplace;
					else if (!stricmp(elem->type, "before"))
						dtx->dtt = dtxbefore;
					else if (!stricmp(elem->type, "after"))
						dtx->dtt = dtxafter;
				}
				if (!CurrDocref->doctextids)
					CurrDocref->doctextids = new DCnlist();
				CurrDocref->doctextids->addnew(elem->idref, (long) dtx);
			}
			else if (elem->type) {
				if (!stricmp(elem->type, "start"))
					tgp = &CurrDocref->docstarttext;
				else if (!stricmp(elem->type, "end"))
					tgp = &CurrDocref->docendtext;
			}
			if (!*tgp)
				*tgp = new DCgroup();
			FlushText();
			if (!PreDocText)
				PreDocText = TextGroup;
			TextGroup = *tgp;
		}
	}
	else if (elem->checkType((epInline << 16) | (epData << 8) | epIndex)) {
		FlushText();
		if (XMLud::InMXMLIndexEntry)
			XMLud::EndUMXLIndexItem();
		XMLud::StartUMXLIndexItem();
	}
#endif
#ifdef DITA
	else if (elem->checkType((epStart << 8) | epIndex)) {
		FlushText();
		StartIndexItem();
	}
#endif
	else if (elem->checkType(epIndex))
		ProcIndexElemStart();
#ifdef UDOC
	else if (elem->checkType((epDef << 16) | (epRef << 8) | epElem)) {
		XMLud::ReadElementDefinitions(elem->href,
			           NewName(elem->idrefs ? elem->idrefs : elem->idref));
		return;
	}
	else if (elem->checkType(epOut)) {  // output-dependent settings
		XMLud::SetOutputType(elem->type);
		return;
	}
	else if (elem->checkType(epCond)) {  // condition settings
		XMLud::ProcCondition(CurrElemInfo);
		return;
	}
	else if (elem->checkType((epRef << 8) | epElem)) {
		SetTextRef(elem, trtext);  // block textref
		return;
	}
	else if (elem->checkType((epRef << 8) | epVar)) {
		VarLibPaths.add(elem->href, ++VarLibCount);
		return;
	}
	else if (elem->checkType((epRef << 8) | epGloss)) {
		XMLud::SetGlossRefFile(elem->href, elem->idref);
		return;
	}
	else if (elem->checkType((epRef << 8) | epAbbr)) {
		XMLud::SetAbbrRefFile(elem->href, elem->idref);
		return;
	}
	else if (elem->checkType((epRef << 8) | epTmark)) {
		XMLud::SetTmarkRefFile(elem->href, elem->idref);
		return;
	}
	else if (elem->checkType((epRef << 8) | epKey)) {  // keyref
		if (XMLud::InOutput
		 && XMLud::OutputKeys) {
			XMLud::OutputKeyref = elem->href;
			XMLud::OutputKeyID = elem->idref;
		}

		if (!XMLud::InOutput  // don't use keydefs for other outputs
		 || XMLud::SourceKeys)    // just for local udoc output
			SetTextRef(elem, trkey);  // get keys in keyref
		return;
	}
#endif
#ifdef UDOC
	else if (elem->checkType((epRef | (epPre << 8)), epInline)
#endif
#ifdef DITA
	else if (elem->checkType(epRef | (epPre << 8) | (epText << 16))
#endif
	) { 
	  // coderef, insert contents of file here ****
		if (elem->href) {
			FlushText();
			elem->isEmpty = false;
			DCctl *cp = new DCctl(ename, 1, 50, 4);
			TextGroup->add(*cp);
			cp->ex(CurrElemInfo->href);
			PendXref = InXref = false;
		}
	}
	else if (elem->checkType(((epInline << 8) | epRef), (epImage << 8) | epTopic)) {
		// xref, not topicref or imagemap area
		FlushText();
		if (elem->href
#ifdef UDOC
		 || elem->idref
#endif
		) {
#ifdef UDOC
			if (!elem->project)
				elem->href = XMLud::MakeUMXLHref(elem->href, elem->idref);
			if (PathIsURL(elem->href))
				scope="external";

			if (!IsInline
			 && !ParaStarted) {
				SetElemFormat(false);
				StartXref(depth);
			}
#endif
			PendXref = true;
			XrefDepth = depth;
			XrefHref = elem->href;
			XrefForm = elem->outclass;
			XrefExt = SetXrefScope(scope, XrefHref);
		}
		else
			XrefHref = NULL;
	}
#ifdef UDOC
	else if (elem->checkType((epRef | (epDir << 8)))) {
		SetQueryRef(elem, qudir, queryatt, resultatt);
		return;
	}
	else if (elem->checkType((epRef | (epDB << 8)))) {
		SetQueryRef(elem, qudb, queryatt, resultatt);
		return;
	}
	else if (elem->checkType((epRef | (epWeb << 8)))) {
		SetQueryRef(elem, quweb, queryatt, resultatt);
		return;
	}
	else if (elem->checkType((epInline << 16) | (epText << 8) | epVar)) {
		// content is a reference to a variable def for transclusion
		// if no src or key, default to x2g\lib\stdvars.mxl for idref
		SetTextRef(elem, trvar);
		return;
	}
	else if (elem->checkType((epInline << 16) | (epText << 8) | epAbbr)) {
			XMLud::ListElemRefStart(elem, AbbrFileName, depth, genabbr);
	}
	else if (elem->checkType((epInline << 16) | (epText << 8) | epGloss)) {
			XMLud::ListElemRefStart(elem, GlossaryFileName, depth, gengloss);
#if 0
		FlushText();
		InGlossTerm = true;
		elem->isEmpty = true;
		// set up link to glossentry, won't work in root map
		if (GlossaryFileName  // unless after <glossary/> so name is set
		 && !elem->href)
			elem->href = XMLud::MakeUMXLHref(GlossaryFileName,
					                elem->idrefs ? elem->idrefs : elem->idref);
		PendXref = true;
		XrefDepth = depth;
		XrefHref = elem->href;
		XrefExt = sclocal;
		XrefForm = "TextXref";
#endif
		return;
	}
	else if (elem->checkType((epDef << 8) | epVar)) {
		// content is a variable definition
		// most likely brought in by a var reference, just a wrapper
		return;
	}
	else if (elem->checkType((epFile << 16) | (epRef << 8) | epRel)) {
		// related topics, set group membership
		if (groupsatt) {
			if (!elem->groups)
				elem->groups = new DCnlist;
			elem->groups->addlist(groupsatt);
			if (elem->parent) {  // pass added groups to parent docref
				if (!((DCmapelem *) elem->parent)->groups)
					((DCmapelem *) elem->parent)->groups = new DCnlist;
				((DCmapelem *) elem->parent)->groups->addlist(elem->groups);
			}
		}
		return;
	}
#endif
	else if (elem->checkType(epImage)) {
#if 0
		if (CurrElemInfo->checkType(epRef)) // area
			StartImageArea(shapeatt, coordsatt);
		else if (CurrElemInfo->checkType(epAlt))
			InAlt = true;
		else if (!UMXL && CurrElemInfo->checkType(epText, epGroup)) // shape
			InImageShape = true;
		else if (!UMXL && CurrElemInfo->checkType(epText)) // coords
			InImageCoords = true;
		else {
#endif
			GraphHref = elem->href;
			FlushText();
			ProcImageStart();
		//}
	}
	else if (elem->checkType(epText)) {   // mainly shortdesc

		if (InKeyDef) {
			SetElemText(false, true);
			if (!elem->classatt)
				return;
			char *en = elem->elemname;
			if (!strcmp(en, "keyword")
			 || !strcmp(en, "linktext"))
				return;
		}
		else if (!UseMapDescriptions
		 && !elem->checkType(epVar) 
		 && !elem->checkType(epTitle) 
		 && !elem->checkType(epBkTitle)) {
			MapDescDepth = depth;
			return;
		}

		
		bool inl = InKeyDef ? true : elem->checkType(epInline);
		elem->elpath = GetElemPath(depth, CurrMapLevel);
 		if (CurrMapLevel > 1)
			GetExtendedElemPath(elem, CurrMapLevel);  // check extended path
		char *form = GetElemForm(elem, NULL, name, inl);

		if (elem->checkType(epSDesc)) {
		//if (elem->checkType(UMXL ? epDesc : epSDesc)) {
			if (ParaStarted)
				EndPara();
			if (CurrTopicref
			 && CurrTopicref->mapdesc)
				TextGroup = CurrTopicref->mapdesc;
			else
				TextGroup = PreTopicmeta;
		}
		else if (elem->checkType(epTmark)) { // tm
			//if (UMXL) { 
#ifdef UDOC
				if (elem->checkType((epText << 8) || epInline)) { // tm
					XMLud::ListElemRefStart(elem, TmarkFileName, depth, gentmk);
#if 0
					FlushText();
					elem->isEmpty = true;
					if (TmarkFileName
					 && !elem->href)
						elem->href = XMLud::MakeUMXLHref(TmarkFileName,
					                elem->idrefs ? elem->idrefs : elem->idref);
					PendXref = true;
					XrefDepth = depth;
					XrefHref = elem->href;
					XrefForm = "TextXref";
					XrefExt = sclocal;
#endif
				}
#endif
			//}
#ifdef DITA
			//else {  // DITA
				// tmtype is tm, reg, or service
				char *tmtype = NULL;
				if ((tmtype = elem->getAttr("tmtype")) != NULL)
					elem->tm = (tmtyp) TrademarkTypes(tmtype);
				// trademark, tmowner, and tmclass also present ****
			//}
#endif
		}

		if (elem->checkType((epInline << 8) | epTypo)) {
			FormatInline.add(depth);
			if (PendingText)
				FlushText();
			SetTypographic(name, true);  // flush and turn on prop
			SetInlineElemTags();
		}
		else if (CurrElemInfo->checkType((epInline << 8) | epBreak)) {
			if (PendingText)
				FlushText();
			TextGroup->add(*(new DCctl(imnem, 2, 11, 10, 1)));
			return;
		}
		else if (form
		 && _stricmp(form, "no")) {
			if (!inl) {
				if (InIgnoreGroup
				 && !elem->toc)
					StartIgnoreGroup = true;
				StartPara();
			}
			elem->elform = form;
			elem->formctl = SetFormat(form, inl);
			SetFlags(depth);
			if (InKeyDef)
				KeyDefForm++;
			if (ShowElementPath) {
				DCctl *cp = new DCctl(ename, 2, 121, 6);
				cp->ex(elem->elpath);
				TextGroup->add(*cp);
				if (elem->elfpath
				 && strcmp(elem->elpath, elem->elfpath)) {
					cp = new DCctl(ename, 2, 121, 6);
					cp->ex(elem->elfpath);
					TextGroup->add(*cp);
				}
			}
			WriteTextNumPrefix(elem, form);
			if (DisplayElementPath
			 && elem->elpath) {
				SetFormat(ElementPathFormat, true);
				PutTextStrCtl(elem->elpath);
				SetFormat(NULL, true);
			}
			FormatUsed.add(form, depth);
		}
		SetMapElemTags();
		ProcessVariableStart(elem);
		if (elem->checkType(epSDesc)) {
		//if (elem->checkType(UMXL ? epDesc : epSDesc)) {
			MapDescNeeded = false;
			StartMapGroup(level, sqdesc);
		}
	}
}


bool
XMLrd::IsDocMap(const char *href)
{
	char *nref = FixReference(href);
	DCrfile *nf = new DCrfile(nref);
	bool ret = false;

	if (nf->FErr() == fok) {
		char *nbuf = new char[256];
		nf->getBlock(nbuf, 255);
		if (strstr(nbuf, "<!DOCTYPE map"))
			ret = true;
		delete [] nbuf;
	}

	DeleteName(nref);
	delete nf;
	return ret;
}


void 
XMLrd::SetMapInfo(const char *name)
{
	char *mname = NewName(name);
	DCnlist *names = ((ProcessingConrefs && ConrefPull) ?
	                     &ConrefMapNamesProcessed :
	                     &MapNamesProcessed);
	CurrMapInfo = new DCmap(++CurrMapNum, mname,
	 PageNum, CurrPageInfo);

	MapInfos.add(CurrMapInfo, CurrMapNum);
	names->add(mname, CurrMapNum);
	CurrElemNum = 0;
	PrevElemInfo = CurrElemInfo = NULL;
}


void
XMLrd::SetMapElemInfo(const char *name, const char *classatt,
 const char *idatt, const char *outclass, const char *hrefatt,
 const char *conrefatt, const char *conendatt,
 const char *conactatt, const char *keyrefatt,
 const char *conkeyrefatt, const char *idrefatt, const char *idrefsatt,
 long depth, bool isRoot)
{
	PrevElemInfo = CurrElemInfo;  // NULL for map elem itself
	CurrElemInfo = new DCmapelem(++CurrElemNum, NewName(name), depth, 
		NewName(classatt), NewName(outclass), CurrMapNum, CurrMapInfo);

	CurrEInfo.replace(CurrElemInfo, depth);
	DCelem *parent = NULL;
	if ((depth > 1)
	 && ((parent = (DCelem *) CurrEInfo.find(depth - 1)) != NULL)) {
		CurrElemInfo->parent = parent;
		if (!parent->children)
			parent->children = new DClist();
		parent->children->add((long) CurrElemInfo);
	}

	if (!CurrElemInfo->setType())
		LogEvent(logwarn, 1, "Properties not found for ", CurrElemInfo->elemname);
	IsInline = CurrElemInfo->elinlin = CurrElemInfo->checkType(epInline);

	CurrElemInfo->elemcount = ++MapElemCount;
	MapElemInfo.add(CurrElemInfo, MapElemCount);
	MapElems.addnew(name, MapElemCount);

	CurrElemInfo->elemtype = isRoot ? GetTopicType(name, classatt) : other;
	CurrElemInfo->xrsrc = CurrElemInfo->checkType(epXRsrc);
	CurrElemInfo->href = FixReference(hrefatt);
	CurrElemInfo->conref = FixReference(conrefatt);
	CurrElemInfo->endref = FixReference(conendatt);
	CurrElemInfo->conact = NewName(conactatt);
	CurrElemInfo->setKeyrefs(keyrefatt, conkeyrefatt);
	CurrElemInfo->idref = NewName(idrefatt);
	CurrElemInfo->idrefs = NewName(idrefsatt);

	if (idatt) {
		if (isRoot) {  // topicid is also its elemid
			CurrMapInfo->id = NewName(idatt);
			CurrElemInfo->id = CurrMapInfo->id;
		}
		else {   // keep topicid out of elemid list
			CurrElemInfo->id = NewName(idatt);
			CurrMapInfo->elemids->add(CurrElemInfo->id, CurrElemNum);
			if ((!TextRefGroup  // leave tg alone in textrefs
			  || !TextGroup)
			 && CurrMapRef
			 && CurrMapRef->elemtext)
				TextGroup = CurrMapRef->elemtext;
		  // insurance in case it is conref'd later:
			if (TextGroup)
				SetElemText(((CurrElemInfo->conref
			            && !CurrElemInfo->conact) ? true : false), true);
		}
	}
}


void
XMLrd::SetMapFormat(DCmapelem *elem, int depth, int level)
{
	char nstr[NumLenMax];
	sprintf(nstr, "%0.2d", level);

	char *name = NewName(BaseMapFormat, strlen(BaseMapFormat) + NumLenMax);
	strcat(name, nstr);  // use as default if no [BlockFormatMaps] item

	if (!elem->elpath)
		elem->elpath = GetElemPath(depth, CurrMapLevel);
 	if (CurrMapLevel > 1)
		GetExtendedElemPath(elem, CurrMapLevel);  // check extended path
	char *form = elem->elform;
	if (!form) {
		form = GetElemForm(elem, NULL, name);
		elem->elform = form;
	}
	DeleteName(name);

	SetFormat(form);
	SetFlags(depth);
	if (ShowElementPath) {
		DCctl *cp = new DCctl(ename, 2, 121, 6);
		cp->ex(elem->elpath);
		TextGroup->add(*cp);
		if (elem->elfpath
		 && strcmp(elem->elpath, elem->elfpath)) {
			cp = new DCctl(ename, 2, 121, 6);
			cp->ex(elem->elfpath);
			TextGroup->add(*cp);
		}
	}
	WriteTextNumPrefix(elem, form);
	if (DisplayElementPath
	 && elem->elpath) {
		SetFormat(ElementPathFormat, true);
		PutTextStrCtl(elem->elpath);
		SetFormat(NULL, true);
	}
	FormatUsed.add(form, depth);
}


void 
XMLrd::ProcTopicref(DCmapelem *elem, bool mapref, long level)
{
	PreTopicmeta = NULL;
	elem->mlevel = level;
	if (elem->checkType(epList))   // generated list
		SetGenFileRef(elem);

	DefMapElemInfo = (DCmapelem *) (elem->map->elems->find(1));

	if ((elem->href
	  || elem->gendoc)
	 && !(elem->keyref
	  || elem->conref
	  || elem->conkeyref)) { 
		if (mapref) { // reference to ditamap
			SetElemText(false, true);
			elem->cmaptxt = new DCgroup();
			elem->elemtext->add(*(elem->cmaptxt));
		}

		if (elem->chunkby == cbnone)
			elem->chunkby = DefMapElemInfo->chunkby;
		if (elem->chunksel == csnone)
			elem->chunksel = DefMapElemInfo->chunksel;
		if (elem->chunkto == ctnone)
			elem->chunkto = ((DCmapelem *) elem->parent)->chunkto;
		if (elem->chunkto == ctnone)
			elem->chunkto = DefMapElemInfo->chunkto;

		if (ProcessingMapConrefs) { // for pull replacement
			ProcessTopicrefConref(elem, mapref,
			 (elem == CurrMapConrefReplacement->srcelem));
		}
		else {  // normal processing
			if (mapref) { 
				elem->mcount = ++MapCount;  // referenced map list id
				MapsToGo.append(&elem->href, MapCount);
				MapRefs.append(elem, MapCount);
			}
			else
				elem->dcount = ++DocCount;  // referenced doc list ID

			if (CurrMapPos) {  // processing submap. insert in place
				if (mapref) { 
					// insert before marker put in by this map
					DocsToGo.insertat(NULL, -MapCount, -CurrMapPos);
					DocRefs.insertat(NULL, -MapCount, -CurrMapPos);
				}
				else {
					// insert before marker put in by this map
					DocsToGo.insertat(&elem->href, DocCount, -CurrMapPos);
					DocRefs.insertat(elem, DocCount, -CurrMapPos);
				}
			}
			else {  // processing initial map, append
				if (mapref) { 
					// insert marker for position of this map
					DocsToGo.append(NULL, -MapCount);
					DocRefs.append(NULL, -MapCount);
				}
				else {
					DocsToGo.append(&elem->href, DocCount);
					DocRefs.append(elem, DocCount);
				}
			}
		}
	}
	else if (elem->conref  // no href, but conref may have one
	 || elem->keyref       // or will be provided by a keydef
	 || elem->conkeyref) {

		if (elem->chunkby == cbnone)
			elem->chunkby = DefMapElemInfo->chunkby;
		if (elem->chunksel == csnone)
			elem->chunksel = DefMapElemInfo->chunksel;
		if (elem->chunkto == ctnone)
			elem->chunkto = DefMapElemInfo->chunkto;

		if (mapref) {
			elem->mcount = ++MapCount;
			if (elem->conref || elem->conkeyref) {
				SetElemText(true, true);
				elem->cmaptxt = new DCgroup();
				elem->elemtext->add(*(elem->cmaptxt));
			}
			if (elem->keyref) {
				KeyrefMapsToGo.append(&elem->href, MapCount);
				KeyrefMapRefs.append(elem, MapCount);
			}
			if (CurrMapPos) {  // processing submap. insert in place
				DocsToGo.insertat(NULL, -MapCount, -CurrMapPos);
				DocRefs.insertat(NULL, -MapCount, -CurrMapPos);
			}
			else {
				DocsToGo.append(NULL, -MapCount);
				DocRefs.append(NULL, -MapCount);
			}
		}
		else {
			elem->dcount = ++DocCount;
			if (CurrMapPos) {  // processing submap. insert in place
				DocsToGo.insertat(&elem->href, DocCount, -CurrMapPos);
				DocRefs.insertat(elem, DocCount, -CurrMapPos);
			}
			else {
				DocsToGo.append(&elem->href, DocCount);
				DocRefs.append(elem, DocCount);
			}
		}
	}
}


void 
XMLrd::ProcTopicrefText(DCmapelem *elem, int depth, long level,
	const char *scope, bool locktitle, const char *txt, long len)
{
#if 0
	if (InTOCTOC) {
		//EndPara();
		//DCctl *cp = new DCctl(group, 1, 30, 1);
		//TextGroup->add(*cp);
		SetTOCTOC();
		InTOCTOC = false;
	}
	else if (!elem->toc)
		StartIgnoreGroup = true;
#endif

	if ((elem->gentype == gentoc)
	 && !elem->href
	 && !TOCWrapper) {
		TOCWrapper = new DCgroup();
		PageTextGroup = TextGroup = TOCWrapper;
	}
	else if ((elem->gentype == gennone)
	 && !elem->toc)
		StartIgnoreGroup = true;

	if (elem->href) {
		StartPara();
		SetMapFormat(elem, depth, level);
		SetFlags(depth);
		SetMapElemTags();
		if ((elem->gentype == gennone)
		 && elem->userel)
			StartMapGroup(level, sqtopic);
#ifdef UDOC
		if (InCodedocref)
			XMLud::WriteCodedocrefTOC(elem);
		else {
#endif
			XrefExt = SetXrefScope(scope, elem->href);
			if (InExternDoc)
				XrefExt = scexternal;
			else
				XrefExt = scope ? (scopetyp) LinkScopes.getitem(scope) : sclocal;
			if (locktitle) {
				XrefDepth = depth;
				//XrefForm = TOCLinkFormat;
				XrefForm = TOCXrefFormat;
				//if (elem->elform
				// && !IsNumbered(elem->elform))
				//	XrefForm = "TextXref";
				XrefHref = elem->href;
				InXref = true;
				EmptyXref = false;
				StartLinkText(elem->href);
				CurrLinkInfo->mref = elem;
				elem->navtxt = PutTextStrCtl(txt, len);
				//SetFormat(NULL, true);
				EndXref(NULL);
			}
			else {
				StartXref(depth);
				XrefForm = TOCXrefFormat;
				XrefDepth = depth;
				//if (elem->elform
				// && !IsNumbered(elem->elform))
				//	XrefForm = "TextXref";
				XrefHref = elem->href;
				EndXref(NULL);
			}
#ifdef UDOC
		}
#endif
		if (elem->gentype == gennone) {
			if (elem->userel)
				EndMapGroup(sqtopic);
			//if (ParaStarted && locktitle)
				EndPara();
			if (UseMapDescriptions) {
				elem->mapdesc = new DCgroup();
				TextGroup->add(*(elem->mapdesc));
			}
			elem->mglevel = level;
			elem->nested = new DCgroup();
			TextGroup->add(*(elem->nested));
		}
	}
	else if (elem->gentype != gennone)
		SetGenFileTOC(elem, depth, level);
	else {  // no href, just text maybe
		StartPara();
		SetMapFormat(elem, depth, level);
		SetFlags(depth);
		SetMapElemTags();
		if ((elem->gentype == gennone)
		 && elem->userel)
			StartMapGroup(level, sqhead);
		PutMapHeadID(elem);
		elem->navtxt = PutTextStrCtl(txt, len);
		if ((elem->gentype == gennone)
		 && elem->userel)
			EndMapGroup(sqhead);
	}
}


void 
XMLrd::ProcNestedTopicMapGroup(int depth, const char *refid)
{
	DCmapelem *elem = CurrDocRef;

	long level = elem->mglevel + (CurrDocLevel - elem->mlevel);
	char nstr[NumLenMax];
	sprintf(nstr, "%0.2d", level);

	DCgroup *txg = TextGroup;
	TextGroup = elem->nested;
	char *nref = NewName(elem->href,
	                     strlen(elem->href) + strlen(refid) + 2);
	char *hash = strchr(nref, '#');
	if (hash)  // replace original ID with nested topic ID
		strcat(hash + 1, refid);
	else {     // add nested topic ID to orig file name
		strcat(nref, "#");
		strcat(nref, refid);
	}

	DCctl *cp = NULL;
	DCgroup *ig = TextGroup;
	DCgroup *ng = NULL;

	if (UseNestedTopicsInTOC) {
		cp = new DCctl(imnem, 2, 11, 2);
		TextGroup->add(*cp);
		cp = new DCctl(imnem, 2, 11, 1);
		TextGroup->add(*cp);

		char *name = NewName(BaseMapFormat, strlen(BaseMapFormat) + NumLenMax);
		strcat(name, nstr);  // use as default if no [BlockFormatMaps] item
		char *form = NULL;
		long levs = level - elem->mglevel;
		char *tref = "topicref/";
		char *fpath = NewName(strlen(elem->elfpath) + (levs * 9));
		for (long i = 0; i < levs; i++)
			strcat(fpath, tref);
		strcat(fpath, elem->elfpath);
		if (IniFile->Section("BlockFormatMaps")
		 && IniFile->FindElemPath(fpath)) {
			form = IniFile->StrRight();
			DeleteName(name);
		}
		else
			form = name;
		DeleteName(fpath);
		SetFormat(form);
	}
	else {
		ng = new DCgroup(DCctl(group, 1, 30, 2)); // ignore but proc map group
		TextGroup->add(*ng);
		TextGroup = ng;
		//cp = new DCctl(group, 1, 30, 2); // ignore but proc map group
		//TextGroup->add(*cp);
	}
	if (elem->userel)
		StartMapGroup(level, sqtopic);
	StartXref(XrefDepth = depth);
	XrefForm = TOCXrefFormat;
	XrefHref = nref;
	EndXref(NULL);
	if (elem->userel)
		EndMapGroup(sqtopic);

	if (!UseNestedTopicsInTOC) {
		//cp = new DCctl(endgroup, 1, 30, 2);
		//TextGroup->add(*cp);
		TextGroup = ig;
	}

	TextGroup = txg;
}



bool XMLrd::TreatTopicheadsAsTopics = true;
long XMLrd::TitleOnlyNameVer = 0;
char *XMLrd::TitleOnlyTopicType = "topic";
char *XMLrd::TitleOnlyTopicID = "head";
bool XMLrd::RecordShortdesc = false;
long XMLrd::TopicHeadNum = 0;
bool XMLrd::AddTopicHeadChildren = false;
char *XMLrd::TopicHeadChildHead = "Topics:";
char *XMLrd::TopicHeadChildHeadFormat = "TopicHeadChildHead";


void
XMLrd::CreateTitleOnlyTopic(void)
{
	// CurrDocRef contains topichead elem, use it to create
	// a title-only topic with shortdesc and maybe prolog

	TitleOnlyNameVer = 0;
	uns len = strlen(TitleOnlyTopicID);
	char *id = NewName(TitleOnlyTopicID, len + 12);
	_ltoa(++TopicHeadNum, id + len, 10);

	char *docname = CorrectFileName(CurrDocRef->href);
	// see if file exists; if so, change name
	while (DCrfile::FileExists(docname)) {
		TitleOnlyNameVer++;
		docname = CorrectFileName(CurrDocRef->href);
	}
	SetDocInfo(docname);
	XMLFile->ReopenFile(DocName, docname);

	DocRoot = NULL;
	CurrDocInfo->mref = CurrDocRef;
	CurrDocRef->docnum = CurrDocNum;
	ChunkDoc = false;
	ChunkDocSet = false;
	ChunkTopic = false;
	CurrBranchDepth = 0;
	FirstTopic = true;

	// call functions as though parser read doc
	char *root = TitleOnlyTopicType;
	bool shortdesc = false;  // record this
	bool prolog = false;  // possible future use
	char *attp[5];
	attp[0] = NULL;
	long depth = 1;  // simulate top level of new file
	//bool skipmap = SkipMapRef;
	//SkipMapRef = false;
	if (CurrDocRef->parselist
	 && CurrDocRef->parselist->next)
		shortdesc = true;  // recorded shortdesc and its contents
	attp[0] = "id";
	attp[1] = id;
	attp[2] = "class";
	attp[3] = "- topic/topic ";
	attp[4] = NULL;
	XMLrfile::CurrRoot = root;
	XMLrfile::DITAver = 1;
	StartElement(root, (const char **) &attp, depth++);
	attp[0] = "class";
	attp[1] = "- topic/title ";
	attp[2] = NULL;
	DeleteName(id);
	StartElement("title", (const char **) &attp, depth);
	char *str = CurrDocRef->navtitle;
	ProcChar(str, strlen(str), depth);
	EndElement("title", depth);

#ifdef DITA
	if (shortdesc)
		Playback(CurrDocRef->parselist, depth);
#endif

	if (prolog) {
		StartElement("prolog", (const char **) &attp, depth++);
		// playback other metadata
		EndElement("prolog", --depth);
	}

	// add code to include list of child topics here ****
	// attach group to topic's TextGroup to receive children
	// when processing related topics at end.  Add heading 
	// and format for it now, as a section with a title.
	if (AddTopicHeadChildren) {
		EndPara();
		attp[0] = "class";
		attp[1] = "- topic/body ";
		attp[2] = NULL;
		StartElement("body", (const char **) &attp, depth++);
		attp[1] = "- topic/section ";
		StartElement("section", (const char **) &attp, depth++);
		CurrTopicInfo->headchildren = new DCgroup;
		TextGroup->add(*(CurrTopicInfo->headchildren));
		EndElement("section", --depth);
		EndElement("body", --depth);
	}

	EndElement(root, --depth);
	//SkipMapRef = skipmap;
}


char *
XMLrd::CorrectFileName(char *name)
{
	char *ver = NULL;
	if (TitleOnlyNameVer) {
		ver = NewName("x", 16);
		itoa(TitleOnlyNameVer, ver + 1, 10);
	}
	char *str = name;
	char *docname = NewName("./", strlen(str) + (ver ? strlen(ver) : 0) + 8);
	char *dp = docname + 2;
	while (*str) {
		if (isalnum(*str))
			*dp++ = *str;
		str++;
	}
	if (ver)
		strcat(dp, ver);
	strcat(dp, ".dita");
	return docname;
}


// keyrefs

bool XMLrd::InKeyDef = false;
long XMLrd::CurrKeyCount = 0; // count of keys in current definition
long XMLrd::LastKey = 0;      // last keynum defined
long XMLrd::KeyDefForm = 0;
DCnlist XMLrd::Keys;          // name from keys indexed by keynum
DCnlist XMLrd::KeyLinks;
DCnlist XMLrd::KeyIDRefs;
DCnlist XMLrd::KeyOutclasses;
DCnlist XMLrd::KeyConrefs;
DCvlist XMLrd::KeyTopicmeta;

DCvlist XMLrd::Keydefs;       // DCmapelem for each key
long XMLrd::PendMapKeyrefs = 0;
DCnlist XMLrd::PendingMapKeyrefs;
DCnlist XMLrd::PendingMapConkeyrefs;
DCvlist XMLrd::PendingMapKeyrefElems;


void
XMLrd::ProcKeydef(DCelem *elem)
{
	if (!elem->keys
	 || !*(elem->keys))
		return;

	if (KeydefsDone) {
		LogEvent(logwarn, 1, "Keydef for \"", elem->keys, "\" not processed.");
		return;
	}

	InKeyDef = true;
	KeyDefForm = 0;

	// add facility for using keyrefs by scope, instead of
	// by the official 1.2 rules, which totally suck.
	// Basic inheritance: top-level map ones apply until:
	//  lower-level map had same one, overrides for that map
	//  topicref contains another keydef, overrides for the
	//   rest of the topics under that topicref


	// each topicref has a keys DCnlist member which lists all keys
	// in scope for that topicref with their keyid.  The same key
	// may have different bindings (ids) in different topicrefs.
	// A global list then relates each keyid to its definition.


	// for basic 1.2 rules, first definition found in breadth-first
	// search, which is what we do with maps anyway, wins

	char *spc = NULL;
	const char *key = elem->keys;
	char *nkey = NULL;

	CurrKeyCount = 0;
	LastKey++;  // one ID per element, not per key

	while ((spc = strchr(key, ' ')) != NULL) {  // more than one key
		nkey = NewName(key, spc - key);
		AddKeydef(nkey, elem->branch);
		DeleteName(nkey);
		key = spc + 1;
	}
	AddKeydef(key, elem->branch);                      // last or only key

	if (CurrKeyCount)  // at least one added
		Keydefs.add(elem, LastKey);
	else
		LastKey--;
}


bool XMLrd::UseBranchKeys = false;
bool XMLrd::KeydefsOnlyWithinBranch = false;
DCnlist XMLrd::BranchKeys;
char *XMLrd::KeyrefBranch = NULL;


void
XMLrd::AddKeydef(const char *nkey, char *branch)
{
	DCnlist *brkeys = NULL;
	bool keydone = false;

	if (branch
	 && UseBranchKeys) {
		if ((brkeys = (DCnlist *) BranchKeys(branch)) == NULL) {
			brkeys = new DCnlist();
			BranchKeys.addnew(branch, (long) brkeys);
		}
		if (!brkeys->getnum(nkey)) {
			brkeys->addnew(nkey, LastKey);
			CurrKeyCount++;
		}
		if (KeydefsOnlyWithinBranch)
			keydone = true;
	}

	if (!keydone
	 && !Keys.getnum(nkey)) {
		Keys.addnew(nkey, LastKey);
		CurrKeyCount++;
	}
}


void 
XMLrd::ResolveKeydefs(void)
{
	if (!LastKey)
		return;

	// resolve all keydefs using index in Keys as ID
	// store resulting hrefs for links in KeyLinks (if not null)
	// and topicmeta elem structs for text in KeyTopicmeta

	DCmapelem *elem = NULL;
	long i = 0;
	long cnt = 0;
	long lastcnt = 0;
	DClist remKeydefs;

	while (((cnt = Keydefs.count()) != 0)  // more unresolved
	 && (cnt != lastcnt)) {  // last round resolved something
		lastcnt = cnt;
		for (i = 1; i <= LastKey; i++) {
			if ((elem = (DCmapelem *) Keydefs.find(i)) != NULL) {
				// if keydef elem has a keyref, need to resolve that first
				char *lnk = NULL;
				char *idr = NULL;
				char *crf = NULL;
				DCelem *tm = NULL;

#ifdef UDOC
				if (elem->project
				 || (elem->output
				  && stricmp(elem->output, OutTypeName))
				 && elem->keyref) {
					elem->keyhref = XMLud::ResolveOutputKey(elem->project, elem->keyref, elem->output);
					if (elem->keyhref)
						elem->href = elem->keyhref;
				}
#endif

				// if a keydef has a keyref, try to use it to get to the final
				// target; if it cannot be resolved, use any href as fallback
				// if no final href, no link property; may still be meta content
				// make list of pending keyrefs and resolve when keyspace done
				if (elem->keyref
				 && !elem->keyhref) {
					long id = Keys.getnum(elem->keyref);  // if undefined, ignore it
					if (id
					 && Keydefs.find(id)
					 && !remKeydefs.find(id))
						continue;  // not processed yet, skip this elem for now
					// processed, see if link found
					lnk = KeyLinks.find(id);
					idr = KeyIDRefs.find(id);
					tm = (DCelem *) KeyTopicmeta.find(id);
				}

				// a conkeyref in any element acts like a conref, but uses a
				// keydef to find the matching target topic; the elem id is
				// retained for the new topic href. any conref is used as fallback.
				if (elem->conkeyref) {
					long cid = Keys.getnum(elem->conkeyref);
					if (cid && Keydefs.find(cid))
						continue;
					crf = KeyConrefs.find(cid);
				}

				// add KeyLinks and KeyTopicmeta items, delete KeyDefs
				// if a keydef ultimately has an href, that href is used in the
				// referencing element, like topicref and related-topics/link
				// in text, it makes term or keyword into links (ie, to a gloss)
				if (lnk)
					KeyLinks.add(lnk, i);
				else if (elem->href)
					KeyLinks.addnew(elem->href, i);

				if (idr)
					KeyIDRefs.add(idr, i);
				else if (elem->idref
				 || elem->idrefs)
					KeyIDRefs.addnew((elem->idrefs ? elem->idrefs : elem->idref), i);

				if (crf)
					KeyConrefs.add(crf, i);
				else if (elem->conref)
					KeyConrefs.addnew(elem->conref, i);

				if (elem->outclass)
					KeyOutclasses.add(elem->outclass, i);

				// if the keydef has topicmeta/keywords/keyword (and no href)
				// then a keyref to it in an empty text keyword causes inclusion
				// of the keyword from the keydef; a topicref does not do this
				// this is the case for any elements that have a matching element
				// in the keydef topicmeta, not just keyword
				// get these later as needed from elem children
				if (tm
				 || ((tm = elem->getChildName("topicmeta")) != NULL))
					KeyTopicmeta.add(tm, i);

				remKeydefs.add(i);
			}
		}
	}

	for (i = LastKey; i > 0; i--) {
		if (remKeydefs.find(i))
			Keydefs.remove(i);
	}

	if (cnt) { // report failures
		for (i = 1; i <= LastKey; i++) {
			if ((elem = (DCmapelem *) Keydefs.find(i)) != NULL)
				LogEvent(logwarn, 1, "Keydef not resolved for keys ", elem->keys);
		}
	}
}


long
XMLrd::GetMapElemKeyID(DCmapelem *elem, char *key, bool conref)
{
	long id = 0;
	DCnlist *brkeys = NULL;
	char *branch = elem->branch;

	if (branch
	 && UseBranchKeys
	 && ((brkeys = (DCnlist *) BranchKeys(branch)) != NULL))
		id = brkeys->getnum(conref ? elem->conkeyref : elem->keyref);

	if (!id)
		id = Keys.getnum(conref ? elem->conkeyref : elem->keyref);

	return id;
}


void 
XMLrd::ResolveMapKeyrefs(void)
{
	// process the other topicrefs that referenced keys

	if (!PendMapKeyrefs)
		return;

	DCmapelem *elem = NULL;
	long i = 0;
	char *key = NULL;
	char *slash = NULL;
	long kval = 0;
	char *href = NULL;
	char *idref = NULL;
	char *hash = NULL;
	bool mapref = false;
	long pos = 0;

	for (i = 1; i <= PendMapKeyrefs; i++) {
		if ((elem = (DCmapelem *) PendingMapKeyrefElems.find(i)) != NULL) {
#ifdef UDOC
			if (!elem->checkType(epTopic))
#endif
#ifdef DITA
			if (elem->format
			 && !strcmp(elem->format, "ditamap"))
#endif
				mapref = true;
			else
				mapref = false;

			// replace original topicref link with href
			if ((key = PendingMapKeyrefs.find(i)) != NULL) {
				if ((slash = strchr(key, '/')) != NULL)
					*slash = '\0';
				if (((kval = GetMapElemKeyID(elem, key)) != 0)
				 && ((href = KeyLinks.find(kval)) != NULL)) {
					idref = KeyIDRefs.find(kval);
					if (strstr(href, ".ditamap"))
						mapref = true;
					if (elem->keyeid) {  // referencing elem has an element ID
						// if keydef has an element ID, remove it
						if ((hash = strchr(href, '#')) != NULL) {
							// a ref to a topic should have a hash, but no slash
							// a ref to a map should not have a hash either
							if (mapref) {
								pos = hash - href;
								href = NewName(href);
								*(href + pos) = '\0';
							}
							else if ((slash = strchr(hash, '/')) != NULL) {
								pos = slash - href;
								href = NewName(href);
								*(href + pos) = '\0';
							}
						}
#ifdef DITA
						else if (!mapref)  // missing topicid in keydef
							LogEvent(logwarn, 1, "keydef missing topicid: ", href);
#endif

						elem->href = NewName(href, strlen(href) + strlen(elem->keyeid) + 5);
						if (mapref) {  // refs to maps just have elem id
							strcat(elem->href, "#");
							strcat(elem->href, elem->keyeid);
						}
						else if (!strchr(elem->href, '#')) {  // no fragment id
							strcat(elem->href, "#");
							// if not to map, need topicid standin here ****
							if (!mapref)
								strcat(elem->href, "id/");
							strcat(elem->href, elem->keyeid);
						}
						else if (!mapref) {  // has topic id, add elem id
							strcat(elem->href, "/");
							strcat(elem->href, elem->keyeid);
						}
						// otherwise mapref with elem id, drop ours ****
					}
					else { // no keyref ID, keep keydef ID
						elem->href = href;
						if (!elem->idrefs)
							elem->idrefs = idref;
					}

					if (elem->checkType((epTopic << 8) | epMap)) { // topicref
						DCgroup *lasttg = TextGroup;
						TextGroup = elem->elemtext;
						CurrElemInfo = elem;
						ProcTopicrefText(elem, elem->elemdepth, elem->mlevel, elem->scopetxt, elem->usenav, NULL, 0);
						DCctl *cp = new DCctl(imnem, 2, 11, 2);
						TextGroup->add(*cp);
						TextGroup = lasttg;
					}
				}
			}
			// replace original topicref conref with href
			if ((key = PendingMapConkeyrefs.find(i)) != NULL) {
				if ((slash = strchr(key, '/')) != NULL)
					*slash = '\0';
				if (((kval = GetMapElemKeyID(elem, key, true)) != 0)
				 && ((href = KeyConrefs.find(kval)) != NULL)) {
					if (strstr(href, ".ditamap"))
						mapref = true;
					if (elem->conkeyeid) {
						elem->conref = NewName(href, strlen(href) + strlen(elem->conkeyeid) + 5);
						if (mapref) {  // refs to maps just have elem id
							strcat(elem->conref, "#");
							strcat(elem->conref, elem->conkeyeid);
						}
						else if (!strchr(elem->conref, '#')) {
							strcat(elem->conref, "#");
							if (!mapref)
								strcat(elem->conref, "id/");
							strcat(elem->conref, elem->conkeyeid);
						}
						else if (!mapref) {
							strcat(elem->conref, "/");
							strcat(elem->conref, elem->conkeyeid);
						}
					}
					else
						elem->conref = href;
				}
			}
		}
	}
}


void
XMLrd::ResolveKeyref(DCtopelem *elem)
{
#ifdef UDOC
	if ((elem->project
	  || (elem->output
	   && stricmp(elem->output, OutTypeName)))
	 && elem->keyref) {
		elem->keyhref = XMLud::ResolveOutputKey(elem->project, elem->keyref, elem->output);
		if (elem->keyhref) {
			if (!stricmp(OutTypeName, "rtf")) {
				char *hash = strrchr(elem->keyhref, '#');
				if (hash) {
					*hash = '\0';
					elem->href = NewName(elem->keyhref);
					elem->idref = NewName(hash + 1);
				}
				else
					elem->href = elem->keyhref;
			}
			else
				elem->href = elem->keyhref;
		}
		return;
	}
#endif

	if (!LastKey
	 || !(elem->keyref
	  || elem->conkeyref))
		return;

	if (!KeydefsDone) {
		KeydefsDone = true;
		ResolveKeydefs();
	}

	// look up keyref and add href if possible
	long id = 0;
	char *idref = NULL;
	DCnlist *brkeys = NULL;
	char *branch = KeyrefBranch ? KeyrefBranch : elem->topic->branch;
	KeyrefBranch = NULL;

	if (branch
	 && UseBranchKeys
	 && ((brkeys = (DCnlist *) BranchKeys(branch)) != NULL))
		id = brkeys->getnum(elem->keyref);
	if (!id)  // no branch ID, get main one
		id = Keys.getnum(elem->keyref);
	if (id
	 && ((elem->keyhref = KeyLinks.find(id)) != NULL))
		elem->href = elem->keyhref;
	if (id
	 && !(elem->idrefs || elem->idref)
	 && ((idref = KeyIDRefs.find(id)) != NULL))
		elem->idrefs = idref;

	long idk = id;
	id = 0;

	if (brkeys)
		id = brkeys->getnum(elem->conkeyref);
	if (!id)
		id = Keys.getnum(elem->conkeyref);
	if (id
	 && ((elem->keyconref = KeyConrefs.find(id)) != NULL))
		elem->conref = elem->keyconref;

	if (!id
	 && !idk)
		return;

	if (idk)
		id = idk;

	char *oclass = NULL;

	if (!elem->outclass
	 && ((oclass = KeyOutclasses.find(id)) != NULL))
		elem->outclass = oclass;

	// if matching elem in topicmeta set that in elem
	// then at end of elem, if no content, use it as content
	DCelem *tm = (DCelem *) KeyTopicmeta.find(id);
	if (!tm)
		return;

	DCelem *ep = NULL;
	if ((ep = tm->getChildName(elem->elemname)) != NULL)
		elem->keytext = ep->elemtext;  // name match
	else if ((ep = tm->getDerivedChild(elem->classatt + 2, true)) != NULL)
		elem->keytext = ep->elemtext;  // specialized
	//else if ((ep = tm->getDerivedChild(elem->classatt + 2, false)) != NULL)
	//	elem->keytext = ep->elemtext;  // generalized
	else if ((ep = tm->getChildName("linktext")) != NULL)
		elem->keytext = ep->elemtext;  // fallback
	else {
		LogEvent(logwarn, 1, "No text found in keydef for ",
		 elem->keyref, "to use in element ", elem->elemname);
	}
}


// map groups

bool XMLrd::InMapDescGroup = false;
bool XMLrd::UseNestedTopicsInTOC = false;
bool XMLrd::MapDescNeeded = false;


void
XMLrd::StartMapGroup(long level, tseqtyp typ)
{
	if (IsBookmap  // not for prefaces
	 && !BookFrontList
	 && !BookChapter)
		return;

	uns dat = (typ == sqhead) ? 1 :
		       ((typ == sqtopic) ? 2 :
		       ((typ == sqdesc) ? 3 : 0));
	DCctl *cp = new DCctl(imnem, 2, 122, dat, (typ == sqdesc) ? 1 : level);
	TextGroup->add(*cp);
	if (typ == sqdesc)
		InMapDescGroup = true;
	else if (!MapPages.find(PageNum))
		MapPages.add(CurrPageInfo, PageNum);
}


void
XMLrd::EndMapGroup(tseqtyp typ)
{
	if (IsBookmap  // not for prefaces
	 && !BookFrontList
	 && !BookChapter)
		return;

	uns dat = (typ == sqhead) ? 1 :
		       ((typ == sqtopic) ? 2 :
		       ((typ == sqdesc) ? 3 : 0));
	DCctl *cp = new DCctl(imnem, 2, 122, dat, 0);
	TextGroup->add(*cp);
	if (typ == sqdesc)
		InMapDescGroup = false;
}


void
XMLrd::CheckMapShortdesc(DCmapelem *elem)
{
	// check at end of topicref if shortdesc is in map
	if (!MapDescNeeded) {
		//delete(elem->mapdesc);
		//elem->mapdesc = NULL;
	}
	else
		MapDescNeeded = false;
}


void
XMLrd::SetTopicShortdescInMap(DCtopic *cti)
{
	// called at start of topic shortdesc if needed
	DCmapelem *elem = cti->mref;
	if (!elem)
		return;
	SetElemText();   // set for shortdesc in topic
	SetElemFormat(); // start para, format in topic
	MapDescNeeded = true;
	DCgroup *tg = TextGroup;  // shortdesc elemtext group

	// switch to map group
	TextGroup = elem->mapdesc;
	StartMapShortdesc(elem);
	StartMapGroup(elem->mlevel, sqdesc);
	DCgroup *sgp = new DCgroup();  // for content within para
	TextGroup->add(*sgp);
	EndMapGroup(sqdesc);
	EndPara();
	TextGroup = tg;
	TextGroup->add(*sgp);
	TextGroup = sgp;

	cti->desc = sgp;
}


void
XMLrd::StartMapShortdesc(DCmapelem *elem)
{
	// add para end, para start, map shortdesc format
	DCctl *cp = new DCctl(imnem, 2, 11, 2);
	TextGroup->add(*cp);
	cp = new DCctl(imnem, 2, 11, 1);
	TextGroup->add(*cp);
	char *epath = "shortdesc/topicmeta/topicref/*";
	char *form = NULL;
	if (IniFile->Section("BlockFormatMaps")
	 && IniFile->FindElemPath(epath))
		form = IniFile->StrRight();
	else
		form = "shortdesc";
	long fnum = BlockFormatList.getitem(form);
	if (!fnum)
		fnum = StoreFormat(form);
	cp = new DCctl(imnem, 2, 82, 1, fnum);
	TextGroup->add(*cp);
	BlockFormats.add(cp, ++BlockFormUseCount);
	SetUniqueID();
}


void
XMLrd::EndMapShortdesc(void)
{
	// at end of topic shortdesc, end cloning group
	FlushText();
	TextGroup = CurrElemInfo->elemtext;
	MapDescNeeded = false;
}



void
XMLrd::PutMapHeadID(DCmapelem *elem)
{
	if (!elem->id)
		elem->id = "maphead";

	SetElemUID(elem);

	DCctl *cp = new DCctl(etext, 2, 112, 2);  // for links
	cp->ex(elem->id, strlen(elem->id));
	TextGroup->add(*cp);
}


void 
XMLrd::SetElemUID(DCelem *elem)
{
	if (!GenerateUIDs  // don't use
	 || elem->elemuid) // or already exists
		return;

	char *ref = NewName("#", strlen(elem->id) + 2);
	strcat(ref, elem->id);
	char *nref = FixReference(ref);
	elem->elemuid = MakeAnchorName(nref, elem);
	DeleteName(ref);
	DeleteName(nref);
	elem->id = elem->elemuid;
}



char *XMLrd::ChunkTok[ChunkTokMax] = {
 "by-topic", "by-document", "select-topic", "select-document",
 "select-branch", "to-content", "to-navigation"
};


void
XMLrd::SetChunkVal(const char *chunk)
{
	if (!chunk || !*chunk
	 || DisableChunking)
		return;

	DCmapelem *el = (DCmapelem *) CurrElemInfo;

	for (unc i = 0; i < ChunkTokMax; i++) {
		if (strstr(chunk, ChunkTok[i])) {
			switch (i) {
				case 0:
					el->chunkby = cbtopic;
					break;
				case 1:
					el->chunkby = cbdoc;
					break;
				case 2:
					el->chunksel = cstopic;
					break;
				case 3:
					el->chunksel = csdoc;
					break;
				case 4:
					el->chunksel = csbranch;
					break;
				case 5:
					el->chunkto = ctcont;
					break;
				case 6:
					el->chunkto = ctnav;
					break;
				default:
					break;
			}
		}
	}
}




// end of drxmlmp.cpp



