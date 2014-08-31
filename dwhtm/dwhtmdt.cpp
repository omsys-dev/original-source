// dwhtmdt.cpp is the code for dcl HTM DITA support, JHG
// Copyright (c) 2007 by Omni Systems, Inc.  All rights reserved.

#include "dcappl.h"
#include "dcctlg.h"
#include "dclist.h"
#include "dcfili.h"
#include "dctxfx.h"
#include "dcfmtr.h"
#include "dcfilp.h"
#include "dcini.h"
#include "dcmacr.h"
#include "dwhtm.h"
#include "dwhtmfl.h"


// variables

bool HXMwr::DocBook = false;
bool HXMwr::DocBookBook = false;
bool HXMwr::uDoc = false;
bool HXMwr::HXML = false;
bool HXMwr::HXref = false;
char *HXMwr::IDattr = "id";

bool HXMwr::DITA = false;
bool HXMwr::DITAStarting = true;
bool HXMwr::CustomTagset = false;
bool HXMwr::DITATopicSet = false;
bool HXMwr::DITAFileStart = true;
bool HXMwr::DITAComposite = false;
bool HXMwr::KeepXrefText = false;
bool HXMwr::FM8Import = false;
bool HXMwr::UseDTDPath = true;
unc HXMwr::DITAVer = 1;
char *HXMwr::LastXMLRoot = NULL;
long HXMwr::LastXMLLevel = 0;
long HXMwr::XMLStartLevel = 0;
DCnlist HXMwr::XMLCloseTags;
char *HXMwr::DITATopic = NULL;
char *HXMwr::DITAStartElem = "title";
char *HXMwr::CurrDITATopic = NULL;
char *HXMwr::DITATopicID = NULL;
char *HXMwr::DITATopicRootAttr = NULL;
long HXMwr::DITATopicCount = 0;
char HXMwr::DITATopicIDSpaceChar = 0;
bool HXMwr::DITATopicIDUnderscore = true;
bool HXMwr::DITATopicIDLowerCase = true;

char *HXMwr::DITANavTitle = NULL;
char *HXMwr::DITASearchTitle = NULL;
char *HXMwr::DITAShortDesc = NULL;
char *HXMwr::DITALinkText = NULL;
bool HXMwr::DITANoToc = false;
bool HXMwr::DITANavTitlePar = false;
bool HXMwr::DITASearchTitlePar = false;
bool HXMwr::DITAShortDescPar = false;

char *HXMwr::DefDITATopic = "concept";
bool HXMwr::UseFormatAsTag = false;
bool HXMwr::DropInvalidParaTag = false;
char *HXMwr::DefDITAPar = "p";
char *HXMwr::DefDITAChar = "ph";
char *HXMwr::DefDITAImg = "image";
char *HXMwr::DefDITATbl = "table";
bool HXMwr::UseTableFooterClass = false;
char *HXMwr::DITATableFooterClass = "footer";
char **HXMwr::DITANoCondAttrs = NULL;

char **HXMwr::DITATableParents = NULL;

bool HXMwr::DITATopicEntities = false;
bool HXMwr::DITACompEntities = false;

char *HXMwr::DITAMenuSep = "|";

DCilist HXMwr::DITATopicNums;  // DITATopicNum indexed by sid
DCilist HXMwr::DITAParaTopicNums;  // DITATopicNum indexed by ParaRefNum
long HXMwr::DITATopicNum = 0;
long HXMwr::DITATopicTypeNum = 2;
long HXMwr::LastTopicTypeNum = 0;
char *HXMwr::DITATopicStartFormat = NULL;

bool HXMwr::DITATitlePar = false;
DCnlist HXMwr::DITATitles;

long HXMwr::DITAEndDiv = 0;
long HXMwr::PreDITADiv = 0;
long HXMwr::PreDITARuninDiv = 0;
long HXMwr::DITAStartDiv = 0;
long HXMwr::DITAErrorDiv = 0;
DCilist HXMwr::DITAEndDivs;

char *HXMwr::DITATag = NULL;
char *HXMwr::DITATagID = NULL;
char *HXMwr::DITAAlias = NULL;
long HXMwr::DITALevel = NULL;
char *HXMwr::DITAParent = NULL;
char *HXMwr::DITAOpenBefore = NULL;
char *HXMwr::DITAOpenAfter = NULL;
char **HXMwr::DITAOpenBeforeArr = NULL;
char **HXMwr::DITAOpenAfterArr = NULL;
char *HXMwr::DITACloseBefore = NULL;
char *HXMwr::DITACloseAfter = NULL;
char **HXMwr::DITACloseBeforeArr = NULL;
char **HXMwr::DITACloseAfterArr = NULL;
char *HXMwr::DITAFirst = NULL;

bool HXMwr::ForceStartTopic = true;
bool HXMwr::SplitTopicFiles = true;
bool HXMwr::NestTopicFiles = false;
bool HXMwr::PartialNesting = false;
bool HXMwr::MixedTopicNesting = false;
bool HXMwr::WrapTopicFiles = false;
bool HXMwr::DITATitleStart = true;

bool HXMwr::DITANoBlock = false;


// for DITA, make sure init of topic info is complete 
// before completing the prolog; wait until first para

void HXMwr::InitDITATopicFile(HTMwfile *wf, long sid)
{
	assert(sid);

	//DITATopicNums.add(DITATopicNum, sid);

	if (LastXMLRoot          // not starting output
	 && !SplitTopicFiles)    // not split so close last topic
		EndAllDITATags(wf);

	StartDITATopic(wf);      // sets DITAMapLevel

	if (LastXMLRoot
	 && !SplitTopicFiles) {
		if (NestTopicFiles) {  // close open levels as needed
			while (LastXMLLevel >= (DITAMapLevel - XMLStartLevel)) {
				wf->endBlock((char *) XMLCloseTags.find(LastXMLLevel--));
				wf->putEnd();
			}
			LastXMLLevel++;      // ensures that levels are not skipped
		}
		else {                 // not nesting
			wf->endBlock(LastXMLRoot);
			wf->putEnd();
		}
		wf->putEnd();
	}

	if (DITAMapVal) {
		DITAMapVal->ref = NewName(wf->getFileName());
		if (!DITAMapBase)
			DITAMapBase = NewName(wf->getBaseName());
	}

	if (WrapTopicFiles
	 || (DITAFileStart
	  && !SplitTopicFiles
	  && (DITATopicCount > 1)
	  && (!NestTopicFiles
	   || PartialNesting
	   || MixedTopicNesting))) {
		DITAComposite = true;
		DITACompEntities = (HTMwr::DITAVarType == 'E') && HTMwr::VarUsedList.count();
		wf->putDT("dita", DTtag::PrologDTypeDita, DTtag::PrologDTDDita, !DITA, DITACompEntities);
		if (DITACompEntities)
			HTMwr::SetVarEntities(wf);
		wf->startBlock("dita");
		wf->endElem(true);
		wf->putEnd();
		HTMwr::FinishProlog(wf);
		FinishInitDITATopicFile(wf, sid);
		wf->putEnd();
	}

	if (NestTopicFiles
	 && !SplitTopicFiles
	 && !LastXMLRoot) { //starting
		XMLStartLevel = DITAMapLevel - 1;
		LastXMLLevel = 1;
	}

	if (!DITAComposite
	 && (SplitTopicFiles
	  || (NestTopicFiles && !LastXMLRoot))) {
		DITATopicEntities = (HTMwr::DITAVarType == 'E')
		 && (SplitTopicFiles ? HTMwr::VarUsedList.find(sid) : HTMwr::VarUsedList.count());
		wf->putDT(HTMwr::XMLRoot, HTMwr::PrologDType, HTMwr::PrologDTD, !DITA, DITATopicEntities);
		if (DITATopicEntities)
			HTMwr::SetVarEntities(wf, SplitTopicFiles ? sid : 0);
	}

	DITAFileStart = false;

	wf->startBlock(LastXMLRoot = HTMwr::XMLRoot);
	if (NestTopicFiles
	 && !SplitTopicFiles)
		XMLCloseTags.replace(LastXMLRoot, LastXMLLevel);

	wf->strAttr(IDattr, DITATopicID);
	if (DITAMapVal)
		DITAMapVal->tid = DITATopicID;

	if (DITATopicRootAttr) {
		wf->putAttrStr(DITATopicRootAttr);
		DITATopicRootAttr = NULL;
	}

	if (DITAComposite) {
		wf->strAttr(HTMwr::XHLangAttr, HTMwr::XhtmlLang);
		wf->endElem(true);
		wf->putEnd();
	}
	else {  // already done for composite
		HTMwr::FinishProlog(wf);
		FinishInitDITATopicFile(wf, sid);
	}
}


void HXMwr::FinishInitDITATopicFile(HTMwfile *wf, long sid)
{
	HTMwr::PutInsertMacro(wf, "Entities");

	if (sid == 1)
		HTMwr::PutInsertMacro(wf, HTMwr::SplittingFile ? "FirstTop" : "NonsplitTop");
	else if (HTMwr::Extracting) {
		char *txt = HTMwr::ExtrTopMark.find(HTMwr::ExtrID);
		if (txt)
			DCmacr::WriteMacro(wf, txt);
		else if (!HTMwr::PutStyleMacro(wf, MacExtrTop, HTMwr::ExtrStartTag, HTMwr::ExtrStartForm))
			HTMwr::PutInsertMacro(wf, "ExtrTop");
		wf->putEnd();
	}
	else
		HTMwr::PutInsertMacro(wf, HTMwr::LastPart ? "LastTop" : "SplitTop");
}


void HXMwr::StartDITATopic(HTMwfile *wf)
{
	long tsnum = DITATopicTypeList.find(DITATopicNum);
	if (tsnum == 0) { // not a defined topic type
		assert(0);
		char *tname = DITATopicNameList.find(DITATopicNum);
		HTMwr::LogEvent(logwarn, 1, "Undefined DITA topic type ", tname);
		DITALoadSpecial(tname);  // adds type to list
		if (((tsnum = DTopicTypes->getitem(tname)) == 0)
		 && ((tsnum = DTopicTypes->getitem(DefDITATopic)) == 0))
			tsnum = 2;  // concept
	}
	if ((DITAtags = (tagset *) DTagSets->find(tsnum)) == NULL)
		DITAtags = DConceptTags;

	DITAStartElem = DITAtags->start;
	HTMwr::PrologDType = DITAtags->dtype;
	HTMwr::PrologDTypeSet = true;
	HTMwr::PrologDTD = DITAtags->dtd;
	HTMwr::PrologDTDSet = true;
	CurrDITATopic = DITAtags->setname;
	HTMwr::XMLRoot = DITAtags->names->find(1);
	InitTags(DITAtags, PutDITAStart, PutDITAStartFull,
		                 PutDITAEnd, PutDITAEndFull);

	DITAStartDiv = 0;
	DITAEndDivs.empty();
	UserElemSetNames->empty();
	UserElemSetNums->empty();
	UserElemSetID = UserESetBase;

	if ((DITATypeNum = DITATopicTypes(HTMwr::XMLRoot)) == 0)
		DITATopicTypes.add(HTMwr::XMLRoot, DITATypeNum = ++DITATypeNameCount);
	if (!DITATypeUsage.match(DITATypeNum, DITATopicNum))
		DITATypeUsage.add(DITATypeNum, DITATopicNum);

	DITATopicID = DITATopicIDList.find(DITATopicNum);
	DITATopicRootAttr = DITATopicRootAttrList.find(DITATopicNum);
	DITANoToc = DITANoTocList.find(DITATopicNum) ? true : false;
	DITANavTitle = DITANavTitleList.find(DITATopicNum);
	DITASearchTitle = DITASearchTitleList.find(DITATopicNum);
	DITAShortDesc = DITAShortDescList.find(DITATopicNum);
	DITALinkText = DITALinkTextList.find(DITATopicNum);

	DITAMapLevel = DITAMapLevels.find(HTMwr::ParaRefNum);
	if (!DITAMapLevel)
		DITAMapLevel = 1;
	DITAMapUsage = (unc) DITAMapUsages.find(HTMwr::ParaRefNum);

	if (DITAMapUsage != 'N') {
		DITAMapVal = new dmapprop;
		DITAMapVal->notoc = DITANoToc;
		if (DITANavTitle) {
			DITAMapVal->locktitle = true;
			DITAMapVal->ntitle = DITANavTitle;
		}
		else {
			DITAMapVal->locktitle = false;
			DITAMapVal->ntitle = DITATitles.find(DITATopicNum);
		}
		if (!DITAMapTitleAlt)
			DITAMapTitleAlt = DITAMapVal->ntitle;
		if (DITASearchTitle)
			DITAMapVal->stitle = DITASearchTitle;
		if (DITAShortDesc)
			DITAMapVal->sdesc = DITAShortDesc;
		if (DITALinkText)
			DITAMapVal->ltext = DITALinkText;
		DITAMapVal->tid = DITATopicID;
		DITAMapVal->typ = HTMwr::XMLRoot;
		DITAMapVal->level = DITAMapLevel;
		DITAMapVal->noref = (DITAMapUsage == 'H');
		if (DITARefAttrs) {
			DITAMapVal->attrs = DITARefAttrs;
			DITARefAttrs = NULL;
			DITAMapVal->attrnames = DITARefAttrNames;
			DITARefAttrNames = NULL;
			DITAMapVal->attrcount = DITARefCount;
			DITARefCount = 0;
		}
		DITAMapVals.add(DITAMapVal, DITATopicNum);
		DITARelVals.add(DITAMapVal, DITATopicNum);
	}
	//DITATitleStart = true;
	InRelTopics = false;
	RelTopicTag = DITAtags->names->operator()("related-links");
	IndexTagNum = DITAtags->names->operator()("indexterm");
	PhTagNum = DITAtags->names->operator()("ph");
}


// variables and markers

DClist HXMwr::DITANoTocList;
DCnlist HXMwr::DITATopicNameList;
DCnlist HXMwr::DITATopicIDList;
DCnlist HXMwr::DITATopicRootAttrList;
DCnlist HXMwr::DITANavTitleList;
DCnlist HXMwr::DITASearchTitleList;
DCnlist HXMwr::DITAShortDescList;
DCnlist HXMwr::DITALinkTextList;

DCilist HXMwr::DITATopicTypeLevels;    // TopicTypeNums indexed by XMLLevel
DCnlist HXMwr::DITATypeNamePrefixList; // file prefix indexed by TopicTypeNum
bool HXMwr::DITATypeNamePrefixUsed = false;


void
HXMwr::ProcDITATopicStart(pprop *cform)
{
	char *lastTopic = DITATopic;
	char *nextTopic = NULL;

	if (!DITATopicSet && cform->dttop) {
		nextTopic = DITATopic = cform->dttop;
		char *sname = NULL;
		long tnum = 0;
		tagset *dtag = NULL;
		if (((tnum = DTopicTypes->getitem(DITATopic)) != 0)
		 && ((dtag = (tagset *) DTagSets->find(tnum)) != NULL)
		 && ((sname = dtag->start) != NULL)
		 && ((cform->dtlev == 1) || DITAStarting)
		 && (cform->dtcname != NULL)
		 && !stricmp(cform->dtcname, sname))
			DITAStartElem = sname;
	}

	if ((DITAStarting
		&& ForceStartTopic)
	 || ((cform->dtlev == 1)
		&& (cform->dtcname != NULL)
		&& !stricmp(cform->dtcname, DITAStartElem))) {
		if (DITAStarting) {
			if (ForceStartTopic) {
				cform->dtlev = 1;
				//if (cform->dtpar)
				//	delete [] cform->dtpar;
				cform->dtpar = DCini::GetStrArr("topic");
				cform->dtcname = DITAStartElem;
			}
			DITAStarting = false;
		}
		if (DITATopicNum) { // process vars for previous topic
			DITATopic = lastTopic;
			ProcDITATopicVars();
			if (nextTopic)
				DITATopic = nextTopic;
		}
		DITATopicNum++;
		DITATopicStartFormat = cform->stag;
		DITATitlePar = true;
		HTMwr::StoreText = true;
		DITATopicSet = false;
		DITAMapLevel = (long) cform->dtmap;
		DITAMapUsage = cform->dtmhd;
		if (SplitTopicFiles
		 && !HTMwr::UseChunking
		 && !HTMwr::SplitHere
		 && HTMwr::SplittingOK()) {
			HTMwr::SplittingFile = true;
			HTMwr::SplitHere = true;
		}
	}
	else if (cform->dtcname != NULL) {
		if (!stricmp(cform->dtcname, "shortdesc")) {
			DITAShortDescPar = true;
			HTMwr::StoreText = true;
		}
		else if (!stricmp(cform->dtcname, "navtitle")) {
			DITANavTitlePar = true;
			HTMwr::StoreText = true;
		}
		else if (!stricmp(cform->dtcname, "searchtitle")) {
			DITASearchTitlePar = true;
			HTMwr::StoreText = true;
		}
	}
}


void 
HXMwr::ProcDITAParaVars(char *chp)
{
	if (DITATitlePar) {
		DITATitles.add(NewName(chp), DITATopicNum); 
		DITAParaTopicNums.append(DITATopicNum, HTMwr::ParaRefNum);
		DITAMapLevels.append(DITAMapLevel, HTMwr::ParaRefNum);
		DITAOrigMapLevels.append(DITAMapLevel, HTMwr::ParaRefNum);
		DITAMapSeqs.append(++DITAMapSeq, HTMwr::ParaRefNum);
		DITAMapUsages.append(DITAMapUsage, HTMwr::ParaRefNum);
		DITATitlePar = false;
	}

	if (DITAShortDescPar) {
		DITAShortDescList.add(NewName(chp), DITATopicNum); 
		DITAShortDescPar = false;
	}
	if (DITANavTitlePar) {
		DITANavTitleList.add(NewName(chp), DITATopicNum); 
		DITANavTitlePar = false;
	}
	if (DITASearchTitlePar) {
		DITASearchTitleList.add(NewName(chp), DITATopicNum); 
		DITASearchTitlePar = false;
	}
}


void 
HXMwr::ProcDITATopicVars(void)
{
	// set vars for topic at end of section in scan phase
	if (DITATopic) {
		DITATopicNameList.replace(NewName(DITATopic), DITATopicNum);
		DITATopic = NULL;
	}
	else if (!DITATopicNameList.find(DITATopicNum))
		DITATopicNameList.add(DefDITATopic, DITATopicNum);

	if (DITATopicID) {
		DITATopicIDList.replace(NewName(DITATopicID), DITATopicNum);
		DITATopicID = NULL;
	}

	if (DITATopicRootAttr) {
		DITATopicRootAttrList.replace(NewName(DITATopicRootAttr), DITATopicNum);
		DITATopicRootAttr = NULL;
	}
	else if (!DITATopicRootAttrList.find(DITATopicNum)
	 && IniFile->Section("DITATopicRootAttrs")
	 && IniFile->Find(DITATopicStartFormat ?
	                  DITATopicStartFormat : HTMwr::Curr.stag)) {
		char *txt = IniFile->StrRight();
		if (txt)
			DITATopicRootAttrList.add(txt, DITATopicNum);
		DITATopicStartFormat = NULL;
	}

	if (DITANoToc)
		DITANoTocList.add(DITATopicNum);

	if (DITANavTitle) {
		DITANavTitleList.replace(NewName(DITANavTitle), DITATopicNum);
		DITANavTitle = NULL;
	}

	if (DITASearchTitle) {
		DITASearchTitleList.replace(NewName(DITASearchTitle), DITATopicNum);
		DITASearchTitle = NULL;
	}

	if (DITAShortDesc) {
		DITAShortDescList.replace(NewName(DITAShortDesc), DITATopicNum);
		DITAShortDesc = NULL;
	}

	if (DITALinkText) {
		DITALinkTextList.replace(NewName(DITALinkText), DITATopicNum);
		DITALinkText = NULL;
	}

	// verify topic type
	DITATopic = DITATopicNameList.find(DITATopicNum);
	DITATopicTypeNum = DTopicTypes->getitem(DITATopic);
	if (DITATopicTypeNum == 0) { // not a defined topic type
		DITALoadSpecial(DITATopic);  // adds type to list
		if (((DITATopicTypeNum = DTopicTypes->getitem(DITATopic)) == 0)
		 && ((DITATopicTypeNum = DTopicTypes->getitem(DefDITATopic)) == 0))
			DITATopicTypeNum = 2;  // concept
	}
	DITATopicTypeList.append(DITATopicTypeNum, DITATopicNum);
	DITATopic = DTopicTypes->find(DITATopicTypeNum);

	if (NestTopicFiles)
		CheckNesting();

	DITATopic = NULL;
	DITATopicTypeNum = 0;
}


void
HXMwr::SetTopicTypePrefixes(HTMwfile *wf)
{
	// if prefix specified for topic type, add it
	// to SplitFileName and BaseFileName lists
  HTMwr::BaseNameList.writeallname((DCwfile *) wf, SetTopicTypePrefix);
  HTMwr::SplitNameList.writeallname((DCwfile *) wf, SetTopicTypePrefix);
}

void
HXMwr::SetTopicTypePrefix(DCwfile *w, char **np, long i)
{
	if (!i 
	 || (*np == NULL))
		return;

	HTMwfile *wf = (HTMwfile *) w;
	char *chp = NULL;
	long tn = DITATopicNums.find(i);
	long typn = DITATopicTypeList.find(tn);
	char *px = DITATypeNamePrefixList.find(typn);
	if (px) {
		chp = NewName(px, strlen(px) + strlen(*np));
		strcat(chp, *np);
	}
	if (chp)
		*np = chp;
}



void
HXMwr::CheckNesting(void)
{
	// check at end of topic during scan
	if (DITAFileStart) {
		XMLStartLevel = DITAMapLevel - 1;
		LastXMLLevel = 1;
		DITAFileStart = false;
		DITATopicTypeLevels.append(DITATopicTypeNum, LastXMLLevel);
	}
	else {
		// identify desired parent
		long cref = DITAMapSeqs(DITAMapSeq);
		long cseq = DITAMapSeq - 1;
		long pref = 0;
		long plev = 0;
		long rlev = DITAMapLevel - XMLStartLevel;
		long parentlev = 1;

		for ( ; cseq > 0; cseq--) {
			pref = DITAMapSeqs(cseq);
			plev = DITAMapLevels.find(pref);
			if ((plev < DITAMapLevel)
			 && (DITAOrigMapLevels.find(pref) < DITAOrigMapLevels.find(cref))) {
				parentlev = plev;
				break;
			}
		}

		if (DITAVer
		 && (LastTopicTypeNum == 5)
		 && ((DITAMapLevel - XMLStartLevel) > LastXMLLevel))
			// correct map level for glossary nest attempt if necessary
			DITAMapLevel = LastXMLLevel + XMLStartLevel;
		else if (DITAMapLevel > (parentlev + 1))
			DITAMapLevel = parentlev + 1;

		LastXMLLevel = DITAMapLevel - XMLStartLevel;

#if 0
		//long parentlev = (DITAMapLevel - XMLStartLevel) - 1;
		if (parentlev > LastXMLLevel)  // prevent skipping levels
			parentlev = LastXMLLevel;
		LastTopicTypeNum = parentlev ? DITATopicTypeLevels.find(parentlev) : 0;

		if (DITAVer
		 && (LastTopicTypeNum == 5)
		 && ((DITAMapLevel - XMLStartLevel) > LastXMLLevel))
			// correct map level for glossary nest attempt if necessary
			DITAMapLevel = LastXMLLevel + XMLStartLevel;
		else if (LastXMLLevel >= (DITAMapLevel - XMLStartLevel))
			// correct map level for skipped levels if any
			LastXMLLevel = (DITAMapLevel - XMLStartLevel);
		else   // prevent skipping going up
			DITAMapLevel = ++LastXMLLevel + XMLStartLevel;
		// record the results
		DITAMapLevels.replace(DITAMapLevel, DITAParaTopicNums(DITATopicNum));
#endif

		// record the results
		DITAMapLevels.replace(DITAMapLevel, cref);
		DITATopicTypeLevels.replace(DITATopicTypeNum, LastXMLLevel);
		// now see the consequences
		if (LastTopicTypeNum              // not at top level
		 && (LastTopicTypeNum < 5)        // if specialized, allow any nesting
		 && (DITATopicTypeNum < (DITAVer ? 6 : 5))
		 && (LastTopicTypeNum != DITATopicTypeNum))  // different basic types
			MixedTopicNesting = true;
		if (DITAMapLevel == (XMLStartLevel + 1))
			PartialNesting = true;  // back to top before end
	}
}


// DITA markers

char *HXMwr::DITAMarkerProps[DITAMarkerPropsMax] = {
"TopicID", "Topic", "ElemID", "ParentID",
"Tag", "Level", "Parent", "NavTitle",
"SearchTitle", "Code", "StartElem", "EndElem",
"Anchor", "NavRef", "MapRef", "ShortDesc",
"LinkText", "MapHead", "RelRow", "RelLinking",
"MapID", "MapName", "MapTitle", "CloseBefore",
"CloseAfter", "LinkScope", "LinkFormat", "LinkType",
"LinkElemID", "First", "Attribute", "CloseBefore",
"CloseAfter", "Alias", "TopicRootAttr", "NoToc"
};

long HXMwr::ProcDITAMarker(char *kp, char *txt)
{
	if (!txt
	 || (*txt == '\0'))
		return 0;

	short key;
	size_t len = 0;
	long sid = HTMwr::Extracting ? HTMwr::ExtrID : (HTMwr::SplitID ? HTMwr::SplitID : 1);
	long pref = HTMwr::StripTable ? HTMwr::ParaRefNum : HTMwr::TxtParaRefNum;
	char *id = NULL;

	// identify key
	for (key = 0; key < DITAMarkerPropsMax; key++) {
		len = strlen(DITAMarkerProps[key]);
		if (!_strnicmp(kp, DITAMarkerProps[key], len))
			break;
	}

	switch (key) {
		case 0:  // TopicID for current file part
			DITATopicID = txt;
			break;

		case 1:  // Topic type for current file part
			if (DTopicTypes->getitem(txt) != 0) {
				DITATopic = txt;
				DITATopicSet = true;
			}
			break;

		case 2:  // ElemID for current element
			DITATagID = txt;
			break;

		case 3:  // ParentID, parentname=parentid
			if ((id = strchr(txt, '=')) == NULL)
				break;
			*id++ = '\0';
			if (!DITAParentIDLp) {
				DITAParentIDIx = 1;
				DITAParentIDLp = new DCnlist;
				DITAParentIDList.add(DITAParentIDLp, pref);
				DITAParentTagLp = new DCnlist;
				DITAParentTagList.add(DITAParentTagLp, pref);
			}
			DITAParentIDLp->add(id, DITAParentIDIx);
			DITAParentTagLp->add(txt, DITAParentIDIx++);
			if (DITA)
				SetParentElemID(txt, id, pref);
			break;

		case 4:  // Tag for current element
			DITATag = txt;
			break;

		case 5:  // Level for current element
			DITALevel = atoi(txt);
			break;

		case 6:  // Parents for current element
			DITAParent = txt;
			break;

		case 7:  // NavTitle for topic
			DITANavTitle = txt;
			break;

		case 8:  // SearchTitle for topic
			DITASearchTitle = txt;
			break;

		case 9:  // Code to insert for inline element
			return 1;

		case 10:  // StartElem inline tag name
			return 7;

		case 11: // EndElem inline tag name
			return 8;

		case 12: // Anchor map marker
			return 9;

		case 13: // Navref map marker
			return 10;

		case 14: // Mapref map marker
			return 11;

		case 15: // ShortDesc marker
			DITAShortDesc = txt;
			break;

		case 16: // LinkText marker
			DITALinkText = txt;
			break;

		case 17: // MapHead marker
			DITAMapHead = txt;
			break;

		case 18: // DITARelRow
			if ((DITARelNum = DITARelNames(txt)) == 0)
				DITARelNames.add(txt, DITARelNum = ++DITARelNameCount);
			if (!DITARelTopics.match(DITARelNum, DITATopicNum))
				DITARelTopics.add(DITARelNum, DITATopicNum);
			break;

		case 19: // DITARelLinking
			DITALinkingAttr.add(txt, DITATopicNum);
			break;

		case 20: // DITAMapID
			DITAMapID = txt;
			break;

		case 21: // DITAMapName
			DITAMapBase = txt;
			break;

		case 22: // DITAMapTitle
			DITAMapTitle = txt;
			break;

		case 23: // DITACloseBefore
			DITACloseBefore = txt;
			break;

		case 24: // DITACloseAfter
			DITACloseAfter = txt;
			break;

		case 25: // DITALinkScope
			return 12;

		case 26: // DITALinkFormat
			return 13;

		case 27: // DITALinkType
			return 14;

		case 28: // DITALinkElemID
			return 15;

		case 29:  // Parents under which current element is first
			DITAFirst = txt;
			break;

		case 30:  // Attribute, tagname: attr=val attr2=val2 ...
			if ((id = strchr(txt, ':')) == NULL)
				break;
			*id++ = '\0';
			if (!DITAAttrLp) {
				DITAAttrIx = 1;
				DITAAttrLp = new DCnlist;
				DITAAttrList.add(DITAAttrLp, pref);
				DITAAttrTagLp = new DCnlist;
				DITAAttrTagList.add(DITAAttrTagLp, pref);
			}
			DITAAttrLp->add(id, DITAAttrIx);
			DITAAttrTagLp->add(txt, DITAAttrIx++);
			break;

		case 31: // DITAOpenBefore
			DITAOpenBefore = txt;
			break;

		case 32: // DITAOpenAfter
			DITAOpenAfter = txt;
			break;

		case 33: // DITAAlias
			DITAAlias = txt;
			break;

		case 34: // DITATopicRootAttr
			DITATopicRootAttr = txt;
			break;

		case 35: //DITANoToc
			DITANoToc = true;
			break;

		default: // unknown type, complain
			break;
	}
	return 0;
}


// DITA index entries

long HXMwr::IndexDiv = 0;
long HXMwr::IndexDivLen = 0;
long HXMwr::IndexTagNum = 0;
long HXMwr::PhTagNum = 0;



void HXMwr::PutDITAIndexEntry(HTMwfile *wf, char *iptr, char *sort)
{
	if (DocBook)  // add correct processing later ****
		return;

	// treat as inline elements, with sort applying only to top level
	// include index-see and index-see-also as needed
	// use indexterm start attribute for ranges only at bottom level
	// end ranges should have no content at all, just <it end="..." />

	// range-containing entries cam be placed in prolog or map  ****
	// prolog is already past, but map is ahead and does the same thing

	long lev = 0;
	long maxlev = 0;
	char *first = iptr;
	bool start = false;
	bool end = false;
	bool see = false;
	char *buf = NULL;
	char *bptr = NULL;
	long stlen = 0;
	long satlen = 0;
	bool seealso = false;
	bool seefound = false;
	bool issee = false;
	long seelev = 0;
	long len = 0;

	if (HTMwr::StartRangeList.find((long)first))
		start = true;
	if (HTMwr::EndRangeList.find((long)first))
		end = true;
	if (HTMwr::SeeAlsoList.find((long)first))
		see = true;

	if (see) {
		stlen = HTMwr::SeeTerm ? strlen(HTMwr::SeeTerm) : 0;
		satlen = HTMwr::SeeAlsoTerm ? strlen(HTMwr::SeeAlsoTerm) : 0;
		if (!satlen && !stlen)  // no term, can't check for it
			see = false;
	}

	if ((DITAVer > 0)
	 && (start || end)) {
		// make up attr from marker contents
		bptr = buf = NewName(strlen(iptr) + 1);
		while (*iptr) {
			if (*iptr == IndexLevMark)
				maxlev++;  // to identify lev for start attr
			else
				*bptr++ = *iptr; // keep char after ILMark
			iptr++;
		}
		*bptr = '\0';
		iptr = first;
	}

	long prediv = wf->GetDiv();

	if (!IndexDiv)
		IndexDiv = wf->NewDiv();
	else
		wf->SelectDiv(IndexDiv);

	wf->startBlock("indexterm");
	if (DITAVer > 0) {
		if (end) {
			wf->strAttr("end", buf);
			DeleteName(buf);
			wf->endElem();
			IndexDivLen = wf->LenDiv();
			wf->SelectDiv(prediv);
			return;
		}
		if (start
		 && !maxlev) {
			wf->strAttr("start", buf);
			DeleteName(buf);
			start = false;
		}
	}

	HTMwr::StartCondIndex(wf);

	wf->endElem(true);
	while (*iptr) {
		if (*iptr == IndexLevMark) {
			if (iptr > first)
				wf->putEscStr(first, iptr - first);
			if (*sort   // put the first part of the sort string here
			 && (DITAVer > 0)
			 && HTMwr::UseSortString
			 && !seefound) {
				wf->putEnd();
				wf->startBlock("index-sort-as");
				wf->endElem(true);
				first = sort;
				len = 0;
				while (*sort) {
					if (*sort == ':') {
						len = sort - first;
						sort++;  // point at next part
						break;
					}
					sort++;
				}
				if (sort > first)
					wf->putEscStr(first, len);
				wf->endBlock("index-sort-as");
			}
			first = iptr = iptr + 2;
			issee = false;
			if (*first == ' ') {
				if (see
				 && (*(first + 1) != '\0')
				 && ((satlen && !_strnicmp(HTMwr::SeeAlsoTerm, first + 1, satlen))
				  || (stlen && !_strnicmp(HTMwr::SeeTerm, first + 1, stlen))))
					issee = true;
				else if (!seefound)
					first++;  // discard space after ILMark if any
			}
			if (!issee) {
				if (!seefound) {
					lev++;
					wf->putEnd();
					wf->startBlock("indexterm");
					if (start
					 && (lev == maxlev)) {
						wf->strAttr("start", buf);
						DeleteName(buf);
						start = false;
					}
					wf->endElem(true);
				}
			}
		}
		if (issee
		 || (see
		  && (iptr > first)
		  && (*iptr == ' ')
		  && (*(iptr + 1) != '\0'))) {
			iptr++; // check string after space
			if ((seealso = (satlen && !_strnicmp(HTMwr::SeeAlsoTerm, iptr, satlen))) // See also
			 || (stlen && !_strnicmp(HTMwr::SeeTerm, iptr, stlen))) {  // or See
				see = false;  // just do once
				if (!issee) {
					len = (iptr - first) - 1;  // discard space before see
					if ((first[len - 1] == '.')
					 || (first[len - 1] == ','))
						len--;
					wf->putEscStr(first, len);  // rest of str is see ref, which may have ILMarks
					if (DITAVer > 0)
						wf->putEnd();
				}
#if 0
				if (*sort
				 && (DITAVer > 0)
				 && HTMwr::UseSortString) {
					wf->putEnd();
					wf->startBlock("index-sort-as");
					wf->endElem(true);
					first = sort;
					len = 0;
					while (*sort) {
						if (*sort == ':') {
							len = sort - first;
							sort++;  // point at next part
							break;
						}
						sort++;
					}
					if (sort > first)
						wf->putEscStr(first, len);
					wf->endBlock("index-sort-as");
					wf->putEnd();
				}
#endif
				if (DITAVer > 0) {
					wf->startSBlock(seealso ? "index-see-also" : "index-see");
					iptr += (seealso ? satlen : stlen) + 1;  // discard space after see
				}
				else
					wf->startSBlock("indexterm");
				first = iptr;
				seefound = true;
				seelev = lev;
			}
			issee = false;
		}
		else
			iptr++;
	}
	if (iptr > first) { // put out last of entry
		len = iptr - first;
		if (seefound      // remove any trailing period
		 && ((first[len - 1] == '.')
		  || (first[len - 1] == ',')))
			len--;
		wf->putEscStr(first, len);
	}
	if (seefound) {
#if 0
		while (lev > seelev) { // close levels for see
			wf->endBlock("indexterm");
			lev--;
		}
#endif
		if (DITAVer > 0)
			wf->endBlock(seealso ? "index-see-also" : "index-see");
		else
			wf->endBlock("indexterm");
		if (*sort && HTMwr::UseSortString)
			wf->putEnd();
	}
	// write sort string for last level
	if (*sort
	 && (DITAVer > 0)
	 && HTMwr::UseSortString
	 && !seefound) {
		wf->putEnd();
		wf->startBlock("index-sort-as");
		wf->endElem(true);
		first = sort;
		while (*sort) {
			if (*sort == ':')
				break;
			sort++;
		}
		*sort = 0;
		if (sort > first)
			wf->putEscStr(first);
		wf->endBlock("index-sort-as");
		if (lev)
			wf->putEnd();
	}
	while (lev) {  // close lower levels
		wf->endBlock("indexterm");
		lev--;
	}
	wf->putEnd();
	wf->endBlock("indexterm");

	IndexDivLen = wf->LenDiv();
	wf->SelectDiv(prediv);
}


char *HXMwr::XrefWrapClass = "phxref";
char *HXMwr::IndexWrapClass = "phindex";
char *HXMwr::FootnoteWrapClass = "phfoot";


void HXMwr::WriteDITAIndexEntries(HTMwfile *wf)
{
	if (!IndexDiv
	 || !IndexDivLen)
		return;

	bool useph = false;
	long prediv = wf->GetDiv();

	if (!IsLegal(IndexTagNum, CurrLevel)) {
		useph = true;
		wf->startBlock("ph");
		wf->setClass(IndexWrapClass);
		wf->endElem(true);
		//wf->startSBlock("ph");
	}

	wf->WriteDiv(IndexDiv);
	wf->SelectDiv(IndexDiv);
	wf->ClearDiv();
	IndexDivLen = 0;
	wf->SelectDiv(prediv);

	if (useph)
		wf->endBlock("ph");
}


// DITA element tags

DCnlist HXMwr::DITAParaTagList;
DCnlist HXMwr::DITAParaTagIDList;
DClist HXMwr::DITAParaTagIDSet;
DClist HXMwr::DITAParaTagIDUsed;
DCnlist HXMwr::DITAParaAliasList;
DCilist HXMwr::DITAParaLevelList;
DCvlist HXMwr::DITAParaParentList;
DCvlist HXMwr::DITAParaFirstList;
DCvlist HXMwr::DITAParaOpenBeforeList;
DCvlist HXMwr::DITAParaOpenAfterList;
DCvlist HXMwr::DITAParaCloseBeforeList;
DCvlist HXMwr::DITAParaCloseAfterList;

long HXMwr::DITAParentIDIx = 0;
DCvlist HXMwr::DITAParentIDList;
DCnlist *HXMwr::DITAParentIDLp = NULL;
DCvlist HXMwr::DITAParentTagList;
DCnlist *HXMwr::DITAParentTagLp = NULL;
long HXMwr::DITAParaTagLevel = 0;

long HXMwr::DITAAttrIx = 0;
DCvlist HXMwr::DITAAttrList;
DCnlist *HXMwr::DITAAttrLp = NULL;
DCvlist HXMwr::DITAAttrTagList;
DCnlist *HXMwr::DITAAttrTagLp = NULL;

char *HXMwr::CurrFMStartTag = NULL;


void HXMwr::ProcDITAElemVars(long pid)
{
	char **sar = NULL;

	// set vars for block element at end of para in scan phase
	if (DITATag) {
		DITAParaTagList.addnew(DITATag, pid);
		DITASetElemType(DITATag, NULL);
		DITATag = NULL;
	}

	if (DITATagID) {
		DITAParaTagIDList.replace(NewName(DITATagID), pid);
		DITAParaTagIDSet.add(pid);
		DITATagID = NULL;
	}

	if (DITAAlias) {
		DITAParaAliasList.addnew(DITAAlias, pid);
		DITAAlias = NULL;
	}

	if (DITALevel) {
		DITAParaLevelList.append(DITALevel, pid);
		DITALevel = 0;
	}

	if (DITAParent) {
		sar = DCini::GetStrArr(DITAParent);
		DITASetElemType(NULL, sar);
		DITAParaParentList.add(sar, pid);
		if (DITA) {
			while (*sar)
				SetParentElemID(*sar++, NULL, pid);
		}
		DITAParent = NULL;
	}
	else if (HTMwr::Curr.dtpar) {
		sar = HTMwr::Curr.dtpar;
		if (DITA) {
			while (*sar)
				SetParentElemID(*sar++, NULL, pid);
		}
	}

	if (DITAOpenBefore) {
		sar = DCini::GetStrArr(DITAOpenBefore);
		DITAParaOpenBeforeList.add(sar, pid);
		DITAOpenBefore = NULL;
	}
	else if (DITAOpenBeforeArr)
		DITAParaOpenBeforeList.add(DITAOpenBeforeArr, pid);

	if (DITAOpenAfter) {
		sar = DCini::GetStrArr(DITAOpenAfter);
		DITAParaOpenAfterList.add(sar, pid);
		DITAOpenAfter = NULL;
	}
	else if (DITAOpenAfterArr)
		DITAParaOpenAfterList.add(DITAOpenAfterArr, pid);

	if (DITACloseBefore) {
		sar = DCini::GetStrArr(DITACloseBefore);
		DITAParaCloseBeforeList.add(sar, pid);
		DITACloseBefore = NULL;
	}
	else if (DITACloseBeforeArr)
		DITAParaCloseBeforeList.add(DITACloseBeforeArr, pid);

	if (DITACloseAfter) {
		sar = DCini::GetStrArr(DITACloseAfter);
		DITAParaCloseAfterList.add(sar, pid);
		DITACloseAfter = NULL;
	}
	else if (DITACloseAfterArr)
		DITAParaCloseAfterList.add(DITACloseAfterArr, pid);

	if (DITAFirst) {
		sar = DCini::GetStrArr(DITAFirst);
		DITAParaFirstList.add(sar, pid);
		DITAFirst = NULL;
	}

	if (DITAParentIDLp) {
		DITAParentIDList.add(DITAParentIDLp, pid);
		DITAParentIDLp = NULL;
		DITAParentTagList.add(DITAParentTagLp, pid);
		DITAParentTagLp = NULL;
	}

	if (DITAAttrLp) {
		DITAAttrList.add(DITAAttrLp, pid);
		DITAAttrLp = NULL;
		DITAAttrTagList.add(DITAAttrTagLp, pid);
		DITAAttrTagLp = NULL;
	}
}


void HXMwr::DITASetElemType(char *tag, char **par)
{
	long val = 0;

	if (!DITAParaTagIDList.find(HTMwr::ParaRefNum)  // no ref to it
	 || DITAElemTypeList.find(HTMwr::ParaRefNum))   // or already marked
		return;

	if (!tag)
		tag = HTMwr::Curr.dtcname;
	if (!par)
		par = HTMwr::Curr.dtpar;

	if (!strcmp(tag, "li")
	 || FindPar(par,"li"))
		val = 6;
	else if (!strcmp(tag, "title")) {
		if (FindPar(par,"section")
		 || FindPar(par,"example")
		 || FindPar(par,"refsyn"))
			val = 2;
		else if (FindPar(par, DocBook ? "figure" : "fig"))
			val = 4;
	}
	if (val)
		DITAElemTypeList.append(val, HTMwr::ParaRefNum);
}



void HXMwr::DITASetTypographics(bool para)
{
	char **base = para ? HTMwr::Curr.dtptyp : HTMwr::Curr.dtctyp;
	char **start = para ? &HTMwr::Curr.dtptyps : &HTMwr::Curr.dtctyps;
	char **end = para ? &HTMwr::Curr.dtptype : &HTMwr::Curr.dtctype;

	if (!base)
		return;

	char *st = NewName(64);
	char *en = NewName(64);
	char *term = DITASetTypo(base, st, en);
	*term = '\0';  // terminate end string
	*start = st;
	*end = en;
}

char *HXMwr::DITASetTypo(char **b, char *s, char *e)
{
	char elem[32] = "";
	char *ept = elem;

	if (*b && **b) {
		strncpy(elem, *b++, 32);
		ept = elem;      // add elem to start string
		*s++ = '<';
		while (*ept)
			*s++ = *ept++;
		*s++ = '>';

		e = DITASetTypo(b, s, e); // process rest of elems

		ept = elem;      // add elem to end string
		*e++ = '<';
		*e++ = '/';
		while (*ept)
			*e++ = *ept++;
		*e++ = '>';
	}
	else
		*s = '\0';  // no more, terminate start string

	return e;
}


void HXMwr::WriteDITAStartPTag(HTMwfile *wf, long pid, char *tag,
	                char **par, char **pfirst, long lev, char *ftag)
{
	char *ntag = NULL;
	char *nid = NULL;
	char **npar = NULL;
	char **nfirst = NULL;
	long nlev = 0;
	char *attr = NULL;
	long aix = 0;

	HTMwr::FigureOpening = false;
	CurrFMStartTag = ftag;

	if (pid) {
		if ((ntag = DITAParaTagList.find(pid)) != NULL)
			HTMwr::Curr.dtcname = tag = ntag;

		if ((npar = (char **) DITAParaParentList.find(pid)) != NULL)
			par = npar;

		if ((nfirst = (char **) DITAParaFirstList.find(pid)) != NULL)
			pfirst = nfirst;

		if ((nlev = DITAParaLevelList.find(pid)) != 0)
			lev = nlev;

		DITAParentIDLp = (DCnlist *) DITAParentIDList.find(pid);
		DITAParentTagLp = (DCnlist *) DITAParentTagList.find(pid);

		DITAAttrLp = (DCnlist *) DITAAttrList.find(pid);
		DITAAttrTagLp = (DCnlist *) DITAAttrTagList.find(pid);

		long cdiv = wf->GetDiv();
		if (cdiv == DITAStartDiv)
			SetDITADivOff(wf);

		if ((DocBook || DITA)          // has pid, so not figure itself
		 && !DITANoBlock) {            // or empty anchor para
			if (!strcmp(tag, "title")
			 && FindPar(par, DocBook ? "figure" : "fig")) { // starting figure title
				if (HTMwr::FigureTitleStartsFigure
				 && HTMwr::FigureDiv)
					CloseFigureDiv(wf);         // write graphic and ending figure tag
				if (HTMwr::FigureDiv) {
					HTMwr::FigureOpening = true;
					HTMwr::FigureOpen = false;
				}
				else
					HTMwr::FigureOpen = true;
			}
			else if (HTMwr::FigureDiv     // starting something else
			 && !HTMwr::FigTable) {
				if (HTMwr::FigsInStart
				 && (cdiv == DITAStartDiv))
					wf->SelectDiv(DITAStartDiv);
				CloseFigureDiv(wf);         // write graphic and ending figure tag
				if (HTMwr::FigsInStart
				 && (cdiv == DITAStartDiv)) {
					HTMwr::FigsInStart = false;
					SetDITADivOff(wf);
				}
			}
			else if (HTMwr::FigureOpen)
				HTMwr::FigureOpen = false;
		}

		WriteDITACloseBlocks(wf, (char **) DITAParaCloseBeforeList.find(pid));
		WriteDITAOpenBlocks(wf, (char **) DITAParaOpenBeforeList.find(pid));
		if (cdiv == DITAStartDiv)
			wf->SelectDiv(DITAStartDiv);
	}

	if (DITANoBlock) {
		if (pid)
			tag = NULL;
	}
	else {
		if (!TagNames->getnum(tag)
		 && DefDITAPar
		 && TagNames->getnum(DefDITAPar))
			HTMwr::Curr.dtcname = tag = DefDITAPar;

		if (pid && !DITAParaTagList.find(pid))
			DITAParaTagList.add(tag, pid);
	}

	if (!DITANoBlock || !pid) {
		if (HTMwr::CodeStoreDiv)
			wf->SelectDiv(HTMwr::CodeStoreDiv);
		if (HTMwr::FigureOpen && HTMwr::FigParentDiv) {
			wf->SelectDiv(PreDITADiv);
			wf->WriteDiv(HTMwr::FigParentDiv);
			wf->DeleteDiv(HTMwr::FigParentDiv);
			HTMwr::FigParentDiv = 0;
			wf->SelectDiv(HTMwr::CodeStoreDiv ? HTMwr::CodeStoreDiv : DITAStartDiv);
		}
		if (!HTMwr::FigureOpening
		 && (!HTMwr::Curr.nopara || !pid))
			wf->startBlock(tag);
		if (HTMwr::CodeStoreDiv)
			wf->SelectDiv(DITAStartDiv);
	}
	WriteStartTag(wf, tag, par, pfirst, lev);  // still writing to start div

	if (pid) {
		if (HTMwr::FigureOpening) {
			wf->startBlock(tag);
			HTMwr::FigureLev = CurrLevel;
			HTMwr::FigureOpen = true;
			HTMwr::FigureOpening = false;
		}
		DITAParaTagLevel = CurrLevel;
		DITACloseAfterArr = (char **) DITAParaCloseAfterList.find(pid);
		DITAOpenAfterArr = (char **) DITAParaOpenAfterList.find(pid);

		if (!DITANoBlock) {
			if (!DITAParaTagIDUsed.find(pid)
			 && ((nid = DITAParaTagIDList.find(pid)) != NULL)) {
				if ((DITAParaTagIDSet.find(pid)
				 || (DITA
				  && (SetElementIDs
				   || DITAPendElemLinks.find(pid)))
				   // && IsDITARefType(tag))))
				 || (DocBook
				  && IniFile
				  && IniFile->Section("DocBookParaIDs")
				  && IniFile->Find(CurrFMStartTag)
				  && IniFile->BoolRight()))) {
					wf->strAttr(IDattr, nid);
					DITAParaTagIDUsed.add(pid);
					if (DITA)
						SetParentElemID(tag, nid, pid);
				}
			}
			if (IniFile
			 && IniFile->Section(DITA ? "DITAParaAttributes" : "DocBookParaAttributes")
			 && IniFile->Find(CurrFMStartTag))
				wf->putAttrStr(DCmacr::GetMacro(IniFile->StrRight()));

			if (DITAAttrLp
			 && ((aix = DITAAttrTagLp->getitem(tag)) != 0)
			 && ((attr = DITAAttrLp->find(aix)) != NULL)) {
				wf->putAttrStr(DCmacr::GetMacro(attr));
			}
		}

		DITAParentIDLp = NULL;
		DITAAttrLp = NULL;
	}

	SetDITADivOff(wf);
	CurrFMStartTag = NULL;
	if (HTMwr::CodeStoreDiv)
		wf->SelectDiv(HTMwr::CodeStoreDiv);
}


long HXMwr::PendingAlias = 0;
DCnlist HXMwr::PendingAliasList;

void HXMwr::WritePendingAliases(HTMwfile *wf)
{
	if (!PendingAlias)
		return;

	char *txt = NULL;
	for (long i = 1; i <= PendingAlias; i++) {
		if (!(txt = PendingAliasList.find(i)))
			continue;
		wf->startElem("data");
		wf->strAttr("name", "topicalias");
		wf->putEnd();
		wf->strAttr("value", txt);
		wf->endElem(false);
	}
	//wf->putEnd();
	PendingAliasList.empty();
	PendingAlias = 0;
}


bool HXMwr::FindPar(char **par, char *tag)
{
	if (!par
	 || !*par)
		return false;

	while (*par) {
		if (!stricmp(*par, tag))
			return true;
		par++;
	}

	return false;
}


void HXMwr::WriteDITAEndPTag(HTMwfile *wf, char *endtag)
{
	SetDITADivOff(wf);
	EndLevels(wf, DITAParaTagLevel);
	WriteDITAIndexEntries(wf);
	EndLevels(wf, DITAParaTagLevel - 1);

	if (HTMwr::FigureOpen
	 && HTMwr::FigTable
	 && !HTMwr::Tbl
	 && endtag
	 && !stricmp(endtag, "title")) {
		HTMwr::Tbl = HTMwr::FigTable;
		HTMwr::WriteTbl(wf);
		if (HTMwr::FigureDiv)
			CloseFigureDiv(wf);
		EndLevels(wf, CurrLevel - 1);  // close the fig
	}

	if (DITACloseAfterArr) {
		WriteDITACloseBlocks(wf, DITACloseAfterArr);
		DITACloseAfterArr = NULL;
	}
	if (DITAOpenAfterArr) {
		WriteDITAOpenBlocks(wf, DITAOpenAfterArr);
		DITAOpenAfterArr = NULL;
	}
}

void HXMwr::WriteDITAOpenBlocks(HTMwfile *wf, char **sar)
{
	if (!sar)
		return;

	long *pars = GetNameNums(sar);
	if (!pars)
		return;

	long *par = pars;

	while (*par)
		WriteStart(wf, *par++, true);

	delete [] pars;
}

void HXMwr::WriteDITACloseBlocks(HTMwfile *wf, char **sar)
{
	if (!sar)
		return;

	long *pars = GetNameNums(sar);
	if (!pars)
		return;

	long *par = pars;
	long lev = 0;
	bool skip = false;
	long levmin = HTMwr::CellStartLevel ? HTMwr::CellStartLevel : 1;

	while (*par) {
		skip = false;
		for (lev = CurrLevel; lev > levmin; lev--) {
			if (DITAEndDivs.find(lev) == 0) {
				skip = true;
				break;
			}
			if (CurrTags[lev] == *par)
				break;
		}
		if (!skip && (lev > levmin)) {
			//wf->putEnd();
			EndLevels(wf, lev - 1);
		}
		par++;
	}
	delete [] pars;
}

void HXMwr::WriteDITAStartCTag(HTMwfile *wf, char *tag, char *ftag, bool runin,
	char **par, char **pfirst, long lev)  // last three params not used yet for CTags
{
	CurrFMStartTag = ftag;

	if (!TagNames->getnum(tag)
	 && DefDITAChar
	 && TagNames->getnum(DefDITAChar))
		tag = DefDITAChar;

	wf->startBlock(tag); // still writing to start div
	if (!runin)
		WriteStartTag(wf, tag, par, pfirst, lev, true);

	if (CurrFMStartTag
	 && IniFile
	 && IniFile->Section(DITA ? "DITACharAttributes" : "DocBookCharAttributes")
	 && IniFile->Find(CurrFMStartTag))
		wf->putAttrStr(DCmacr::GetMacro(IniFile->StrRight()));

	CurrFMStartTag = NULL;
}

void HXMwr::WriteDITAEndCTag(HTMwfile *wf, char *tag, bool runin)
{
	if (!TagNames->getnum(tag)
	 && DefDITAChar
	 && TagNames->getnum(DefDITAChar))
		tag = DefDITAChar;

	wf->endBlock(tag);
	if (!runin)
		WriteEndTag(wf, tag);
}


void HXMwr::WriteDITAFinishTag(HTMwfile *wf, char *tag, bool endit)
{
	// for self-contained elements
	if (endit)
		wf->endElem();
	WriteFinishTag(wf, tag);
}


bool HXMwr::CloseFigAfterImage = true;
char *HXMwr::FigSar[2] = { "fig", NULL };
char *HXMwr::FigureSar[2] = { "figure", NULL };


void HXMwr::CloseFigureDiv(HTMwfile *wf, bool part)
{
	long cdiv = wf->GetDiv();
	HTMwr::FigureDivRefList.writeall(wf, HTMwr::FixOneDivRef);
	if (HTMwr::FigParentDiv) {
		wf->WriteDiv(HTMwr::FigParentDiv);
		wf->DeleteDiv(HTMwr::FigParentDiv);
		HTMwr::FigParentDiv = 0;
		wf->SelectDiv(cdiv);
	}
	wf->WriteDiv(HTMwr::FigureDiv);
	wf->DeleteDiv(HTMwr::FigureDiv);
	HTMwr::FigureDiv = 0;
	wf->SelectDiv(cdiv);
	HTMwr::FigureDivRefList.empty();
	if (HTMwr::MapCalloutDiv) {
		HTMwr::MapCalloutDivRefList.writeall(wf, HTMwr::FixOneDivRef);
		wf->WriteDiv(HTMwr::MapCalloutDiv);
		wf->DeleteDiv(HTMwr::MapCalloutDiv);
		wf->SelectDiv(cdiv);
		HTMwr::MapCalloutDiv = 0;
		HTMwr::MapCalloutDivRefList.empty();
	}
	if (!part) {
		if (HTMwr::FigureOpen) {  // close figure tag too
			if (HTMwr::FigureLev)
				EndLevels(wf, HTMwr::FigureLev);
			HTMwr::FigureLev = 0;
			HTMwr::FigureOpen = false;
		}
		if (CloseFigAfterImage)
			WriteDITACloseBlocks(wf, DocBook ? FigureSar : FigSar);
	}
}

void HXMwr::EndAllDITATags(HTMwfile *wf)
{
	long div = 0;

	if (HTMwr::FigureDiv)
		CloseFigureDiv(wf);
	EndLevels(wf, 1);
	if ((div = wf->GetDiv()) != 0) {  // uh-oh ****
		if (PreDITADiv) {
			wf->SelectDiv(0);
			wf->WriteDiv(PreDITADiv);
			wf->SelectDiv(0);
		}
		if (div != PreDITADiv) {
			wf->SelectDiv(0);
			wf->WriteDiv(div);
			wf->SelectDiv(0);
		}
	}
}

void HXMwr::WriteDITAElem(HTMwfile *wf, char *tag, char *txt)
{
	HTMwfile *hwf = (HTMwfile *) wf;
	hwf->putEnd();
	hwf->startBlock(tag);
	hwf->endElem(true);
	hwf->putStr(txt);
	hwf->endBlock(tag);
}


//  DITA tables

tblprop *HXMwr::CurrDITATbl = NULL;
bool HXMwr::InDITATblHeader = false;

void HXMwr::SetDITATable(char *tname)
{
	long id = 0;

	if (!DITATableNames
	 || ((id = DITATableNames->getitem(tname)) == 0)
	 || ((CurrDITATbl = (tblprop *) DITATableLists->find(id)) == NULL))
		CurrDITATbl = &DITAComplexTable;
}

void HXMwr::WriteDITAStartTbl(HTMwfile *wf, char *tag, char **par)
{
	if (HTMwr::FigureDiv)
		CloseFigureDiv(wf);

	wf->startBlock(tag);
	WriteStartTag(wf, tag, par);

	if (IniFile
	 && IniFile->Section(DITA ? "DITATableAttributes" : "DocBookTableAttributes")
	 && IniFile->Find(tag))
		wf->putAttrStr(DCmacr::GetMacro(IniFile->StrRight()));

	SetDITADivOff(wf);
}

void HXMwr::WriteDITAEndTbl(HTMwfile *wf, long lev)
{
	EndLevels(wf, lev);
}


// calls to set divs

void HXMwr::SetDITAStart(HTMwfile *wf)
{
	if (!HTMwr::RuninDiv)
		PreDITADiv = wf->GetDiv();
	DITAStartDiv = wf->NewDiv();
	wf->SelectDiv(DITAStartDiv);
}

void HXMwr::SetDITAEnd(HTMwfile *wf)
{
	long lev = 0;

	if (!HTMwr::RuninDiv)
		PreDITADiv = wf->GetDiv();
	DITAEndDiv = wf->NewDiv();
	wf->SelectDiv(DITAEndDiv);
	for (lev = CurrLevel; lev > 1; lev--) {
		if (DITAEndDivs.find(lev) == 0) {  // does not already exist
			DITAEndDivs.append(DITAEndDiv, lev);
			break;
		}
	}
	if (lev == 0)  // full up, overwrite bottom one
		DITAEndDivs.replace(DITAEndDiv, CurrLevel);
}

void HXMwr::SetDITADivOff(HTMwfile *wf)
{
	if (InRelTopics
	 && DITARelLinkDiv)
		wf->SelectDiv(DITARelLinkDiv);
	else
		wf->SelectDiv(PreDITADiv);
}


// callbacks from DChier

void HXMwr::PutDITAStart(HTMwfile *wf, char *tag, long lev, bool inlin)
{
	// write stored div for element start, DITAStartDiv
	long pre = InRelTopics ? DITARelLinkDiv : PreDITADiv ;
	wf->SelectDiv(pre);
	if (DITAErrorDiv) {
		wf->WriteDiv(DITAErrorDiv);
		wf->DeleteDiv(DITAErrorDiv);
		DITAErrorDiv = 0;
		wf->SelectDiv(pre);
	}
	if (DITAStartDiv) {
		wf->WriteDiv(DITAStartDiv);
		wf->DeleteDiv(DITAStartDiv);
		DITAStartDiv = 0;
		wf->SelectDiv(pre);
	}
	if (HTMwr::CodeStoreDiv)
		wf->SelectDiv(HTMwr::CodeStoreDiv);
}

void HXMwr::PutDITAStartFull(HTMwfile *wf, char *tag, long lev, bool inlin)
{
	char *nid = NULL;
	long par = 0;
	char *attr = NULL;
	long aix = 0;
	bool inlink = false;
	bool infig = false;

	// write interpolated tag
	if (HTMwr::FigureDiv
	 && !HTMwr::FigureOpen) {
		infig = true;
		if (HTMwr::FigureOpening)
			wf->SelectDiv(DITAStartDiv);
		else if (!HTMwr::FigParentDiv)
			HTMwr::FigParentDiv = wf->NewDiv();
		else
			wf->SelectDiv(HTMwr::FigParentDiv);
	}
	else
		wf->SelectDiv(PreDITADiv);

	if (DITAErrorDiv) {
		wf->WriteDiv(DITAErrorDiv);
		wf->DeleteDiv(DITAErrorDiv);
		DITAErrorDiv = 0;
		wf->SelectDiv(infig ? HTMwr::FigParentDiv : PreDITADiv);
	}
	wf->startBlock(tag);
	if (DITAParentIDLp
	 && ((par = DITAParentTagLp->getitem(tag)) != 0)
	 && ((nid = DITAParentIDLp->find(par)) != NULL)) {
		wf->strAttr(IDattr, nid);
		if (DITA || DocBook)
			SetParentElemID(tag, nid, HTMwr::ParaRefNum);
	}
	else if (!DITAParaTagIDUsed.find(HTMwr::ParaRefNum)
	 && ((nid = DITAParaTagIDList.find(HTMwr::ParaRefNum)) != NULL)) {
		if (((DITA || DocBook)
		  && IsDITARefType(tag))
		 || (IniFile
		  && IniFile->Section("DocBookParentIDs")
		  && IniFile->Find(CurrFMStartTag)
		  && !stricmp(tag, IniFile->StrRight()))) {
			wf->strAttr(IDattr, nid);
			DITAParaTagIDUsed.add(HTMwr::ParaRefNum);
			if (DITA || DocBook)
				SetParentElemID(tag, nid, HTMwr::ParaRefNum);
		}
	}

	if (DITA
	 && HTMwr::FigureOpen
	 && !strcmp(tag, "fig")) {
		if (HTMwr::FigTable || HTMwr::Tbl)
			HTMwr::StartCondTable(wf, HTMwr::FigTable ? (long) HTMwr::FigTable->tblid : 0);
		HTMwr::StartCondParaBlock(wf);
	}

	if (IniFile
	 && IniFile->Section(DITA ? "DITAParentAttributes" : "DocBookParentAttributes")
	 && IniFile->Find(CurrFMStartTag)) {
		char *cstr = IniFile->StrRight();
		while (*cstr == ' ')
			cstr++;
		size_t len = strlen(tag);
		if (!_strnicmp(cstr, tag, len)
		 && (cstr[len] == ':'))
			wf->putAttrStr(DCmacr::GetMacro(cstr + len + 1));
	}

	if (DITAAttrLp
	 && ((aix = DITAAttrTagLp->getitem(tag)) != 0)
	 && ((attr = DITAAttrLp->find(aix)) != NULL)) {
		//if (*attr != ' ')
		//	wf->putChar(' ');
		wf->putAttrStr(DCmacr::GetMacro(attr));
	}

	if (DITA
	 && InRelTopics
	 && !strcmp(tag, "link")) {  // start new div for linktext
		StartDITARelLinkDiv(wf);
		wf->SaveAttrs();
		inlink = true;
	}

	if (!inlink) {
		wf->endElem(true);
		wf->putEnd();
	}
	if (infig)
		wf->SelectDiv(PreDITADiv);
	DITAEndDivs.add( -1, CurrLevel);
}

bool HXMwr::IsEndable(long lev)
{
	//if (lev == CurrLevel)
	//	return true;

	if (lev == 0)
		return false;

	if (lev == 1)
		return true;

	if (DITAEndDivs.find(lev) != 0)
		return true;

	return false;  // must nest because elem not ended yet
}

void HXMwr::PutDITAEnd(HTMwfile *wf, char *tag, long lev, bool inlin)
{
	// write stored div for element end
	long div = DITAEndDivs.find(lev);
	if (div == -1)  // interpolated
		PutDITAEndFull(wf, tag, lev, inlin);
	else if (div) {
		wf->SelectDiv(PreDITADiv);
		wf->WriteDiv(div);
		wf->DeleteDiv(div);
		wf->SelectDiv(PreDITADiv);
	}
	DITAEndDivs.remove(lev);

#if 0
	if (DITATitleStart
	 && !stricmp(tag, "title")) {
		wf->SelectDiv(PreDITADiv);
		if (DITANavTitle || DITASearchTitle) {
			wf->startBlock("titlealts");
			wf->endElem(true);
			if (DITANavTitle)
				WriteDITAElem(wf, "navtitle", DITANavTitle);
			if (DITASearchTitle)
				WriteDITAElem(wf, "searchtitle", DITASearchTitle);
			wf->putEnd();
			wf->endBlock("titlealts");
			wf->putEnd();
		}
		DITATitleStart = false;
	}
#endif
}


void HXMwr::PutDITAEndFull(HTMwfile *wf, char *tag, long lev, bool inlin)
{
	// write interpolated tags
	bool infig = false;

	if (HTMwr::FigureOpen) {
		wf->SelectDiv(HTMwr::FigureDiv);
		wf->endBlock(tag);
		wf->putEnd();
		wf->SelectDiv(PreDITADiv);
		return;
	}
	if (HTMwr::FigureDiv
	 && !HTMwr::FigureOpen) {
		infig = true;
		if (!HTMwr::FigParentDiv)
			HTMwr::FigParentDiv = wf->NewDiv();
		else
			wf->SelectDiv(HTMwr::FigParentDiv);
	}
	else
		wf->SelectDiv(PreDITADiv);

	if (DITARelLinkDiv) { // ending a <link> that had no hyperlink
		wf->SelectDiv(PreRelLinkDiv);
		wf->RestoreAttrs();
		EndDITARelLinkDiv(wf);
	}
	wf->endBlock(tag);
	wf->putEnd();
	if (infig)
		wf->SelectDiv(PreDITADiv);
}


// Links

long HXMwr::DITALinkDiv = 0;
bool HXMwr::IsFrXref = false;
bool HXMwr::SetElementIDs = false;
bool HXMwr::InRelTopics = false;
bool HXMwr::UseLocalScope = false;
long HXMwr::RelTopicTag = 0;
long HXMwr::DITARelLinkDiv = 0;
long HXMwr::PreRelLinkDiv = 0;
DCilist HXMwr::DITATopicTypeList;   // typenum in DTopicTypes indexed by topicnum
DCnlist HXMwr::DITATopicLinkList;   // link id str indexed by topicnum
DCnlist HXMwr::DITAPendElemLinks;   // link id str indexed by ParaRefNum
DCilist HXMwr::DITAElemTypeList;    // typenum for [DITAElemType] indexed by ParaRefNum
char *HXMwr::CurrDITATblID = NULL;  // ID of current table element in scan and write
DCnlist HXMwr::DITATableIDList;     // tableid indexed by para ref num of anchor
long HXMwr::DITAElemType = 0;       // set for current link in ProcExtRef
long HXMwr::DITATopicType = 0;
char *HXMwr::DITALinkScope = NULL;  // marker attributes set in write phase
char *HXMwr::DITALinkFormat = NULL;
char *HXMwr::DITALinkType = NULL;
char *HXMwr::DITALinkElemID = NULL;


uns HXMwr::IsDITARefType(char *tag)
{
	if (!strcmp(tag, "section")
	 || !strcmp(tag, "example")
	 || !strcmp(tag, "refsyn"))
		return 2;
	if (!strcmp(tag, DocBook ? "figure" : "fig"))
		return 4;
	if (!strcmp(tag, "li"))
		return 6;
	return 0;
}


void HXMwr::DITASetIntRef(char *txt)
{
	// during scan phase, at point of anchor ID
	DITATopicLinkList.add(txt, DITATopicNum ? DITATopicNum : 1);
	// this gets the TopicID from DITATopicIDList
	// and topic type info from DITATopicTypeList
	// both written at the end of scan phase by DITASetTopicInfo()
	// so #topicID is available for the links in write phase

	DITAPendElemLinks.add(txt, HTMwr::ParaRefNum);
	// the DITAParaTagIDList should have an ID for ParaRefNum;
	// the DITAParaTagList gives the DITA tag for the para itself.
	// both should be set in scan phase
}

void HXMwr::DITASetTopicInfo(void)
{
	long i = 0;
	long sid = 0;
	char *id = NULL;

	// run at end of scan phase before ProcIntRef

	for (i = 1; i <= DITATopicCount; i++) {
		if (!DITATopicIDList.find(i)) {
			if (SplitTopicFiles) {
				sid = HTMwr::ParaSplitList.find(DITAParaTopicNums(i));
				id = sid ? HTMwr::BaseNameList.find(sid) : HTMwr::CurrFileName;
				id = HTMwr::MakeID(id, NULL, 0, DITATopicIDSpaceChar,
				            DITATopicIDUnderscore, true, DITATopicIDLowerCase);
			}
			else {
				id = NewName("topic", NumStrMax + 6);
				ltoa(i, id + 5, 10);
			}
			DITATopicIDList.add(id, i);
			id = NULL;
		}
	}
}

bool HXMwr::DITAProcIntRef(char *txt, bool isnew)
{
	// at end of scan phase, checking in RefFileName
	//  txt == ID used as key in .ref file: newlink, X*, R*
	//  isnew == true if not in [IntRefs] before
	// check DITA info and set it as needed
	// info is used during write phase for internal links
	// actual fixup of ext files is at very end of write phase
	bool changed = false;
	char *tid = NULL;
	char *tpstr = NULL;
	long tp = 0;

	long tn = DITATopicLinkList(txt);
	if (tn == 0)
		return false;
	long pr = DITAPendElemLinks(txt);

	// set [DITATopicID]
	if (DITA) {
		tid = DITATopicIDList.find(tn);
		assert(tid);
		DCpfile::getval("DITATopicID", txt, "", HTMwr::RefBuf, RefBufLen, HTMwr::RefFileName);
		if ((*HTMwr::RefBuf == '\0')
		 || strcmp(HTMwr::RefBuf, tid)) {
			changed = true;
			DCpfile::putval("DITATopicID", txt, tid, HTMwr::RefFileName);
		}
	}

	// set [DITAElemID]
	tid = DITAParaTagIDList.find(pr);
	//assert(tid);
	if (tid) {
		DCpfile::getval("DITAElemID", txt, "", HTMwr::RefBuf, RefBufLen, HTMwr::RefFileName);
		if ((*HTMwr::RefBuf == '\0')
		 || strcmp(HTMwr::RefBuf, tid)) {
			changed = true;
			DCpfile::putval("DITAElemID", txt, tid, HTMwr::RefFileName);
		}
	}

	// set [DITAElemType]
	tp = DITAElemTypeList.find(pr);
	//assert(tp);
	if (tp) {
		tpstr = NewName(NumStrMax);
		_ltoa(tp, tpstr, 10);
		DCpfile::getval("DITAElemType", txt, "", HTMwr::RefBuf, RefBufLen, HTMwr::RefFileName);
		if ((*HTMwr::RefBuf == '\0')
		 || strcmp(HTMwr::RefBuf, tpstr)) {
			changed = true;
			DCpfile::putval("DITAElemType", txt, tpstr, HTMwr::RefFileName);
		}
		DeleteName(tpstr);
	}

	// set [DITATopicType]
	if (DITA) {
		tp = DITATopicTypeList.find(tn);
		assert(tp);
		tpstr = NewName(NumStrMax);
		_ltoa(tp, tpstr, 10);
		DCpfile::getval("DITATopicType", txt, "", HTMwr::RefBuf, RefBufLen, HTMwr::RefFileName);
		if ((*HTMwr::RefBuf == '\0')
		 || strcmp(HTMwr::RefBuf, tpstr)) {
			changed = true;
			DCpfile::putval("DITATopicType", txt, tpstr, HTMwr::RefFileName);
		}
		DeleteName(tpstr);
	}

	return changed;  // false == DITA info is unchanged
}

char *HXMwr::DITASetIntLink(char *txt, char *fname)
{
	// called during ProcIntRef after DITAProcIntRef
	// txt is ref, fname is corrected filename
	char *flink = NULL;
	char *eid = NULL;

	DCpfile::getval("DITAElemType", txt, "", HTMwr::RefBuf, RefBufLen, HTMwr::RefFileName);
	if ((*HTMwr::RefBuf != '\0')
	 && (*HTMwr::RefBuf != '1')) {  // not ref to full topic, use elem ID
		DCpfile::getval("DITAElemID", txt, "", HTMwr::RefBuf, RefBufLen, HTMwr::RefFileName);
		if (*HTMwr::RefBuf)
			eid = NewName(HTMwr::RefBuf);
	}

	DCpfile::getval("DITATopicID", txt, "", HTMwr::RefBuf, RefBufLen, HTMwr::RefFileName);
	flink = NewName(fname, strlen(fname) + strlen(HTMwr::RefBuf) + 2 + (eid ? (strlen(eid) + 1) : 0));
	strcat(flink, "#");
	strcat(flink, HTMwr::RefBuf);
	if (eid) {
		strcat(flink, "/");
		strcat(flink, eid);
	}

	return flink;
}


void HXMwr::SetParentElemID(char *tag, char *id, long pid)
{
	// called when interpolated parent ID determined in scan phase
	// or written during write phase
	if (id
		&& !DITAParaTagIDList.find(pid))
		DITAParaTagIDList.add(id, pid);

	uns val = 0;
	if (tag
	 && !DITAElemTypeList.find(pid)
	 && ((val = IsDITARefType(tag)) != 0))
		DITAElemTypeList.append(val, pid);
}


char *HXMwr::DITAProcExtRef(char *txt, char *rfn)
{
	char *rtxt = NULL;
	char *eid = NULL;

	// called during write phase, txt is ref, returns DITA anchor
	if (!txt || !rfn) { // can't find ref
		DITAElemType = DITATopicType = 1;  // default it to generic topic
		return NULL;  // leave ref as anchor
	}

	// set DITAElemType from [DITAElemType]
	// linkid= 1=topic, 2=section, 3=table, 4=fig, 5=fn, 6=li
	DCpfile::getval("DITAElemType", txt, "", HTMwr::RefBuf, RefBufLen, rfn);
	if ((*HTMwr::RefBuf == '\0')
	 || ((DITAElemType = atol(HTMwr::RefBuf)) < 1))
		DITAElemType = 1;  // default it to generic topic

	// set element ID from [DITAElemID]
	if (DocBook
	 || (DITAElemType > 1)) {
		DCpfile::getval("DITAElemID", txt, "", HTMwr::RefBuf, RefBufLen, rfn);
		if (*HTMwr::RefBuf)
			eid = NewName(HTMwr::RefBuf);
		if (DocBook) {
			rtxt = NewName("#", strlen(eid) + 2);
			strcat(rtxt, eid);
			return rtxt;
		}
	}

	// set DITATopicType from [DITATopicType]
	// linkid= 1=topic, 2=concept, 3=task, 4=reference, 5=glossary, 6=map
	DCpfile::getval("DITATopicType", txt, "", HTMwr::RefBuf, RefBufLen, rfn);
	if ((*HTMwr::RefBuf == '\0')
	 || ((DITATopicType = atol(HTMwr::RefBuf)) < 1))
		DITATopicType = 1;  // default it to generic topic

	// replace rtxt with #[DITATopicID]
	DCpfile::getval("DITATopicID", txt, "", HTMwr::RefBuf, RefBufLen, rfn);
	if (*HTMwr::RefBuf) {
		rtxt = NewName("#", strlen(HTMwr::RefBuf) + 2 + (eid ? (strlen(eid) + 1) : 0));
		strcpy(rtxt + 1, HTMwr::RefBuf);
		if (eid
		 && stricmp(eid, rtxt + 1)) { // make sure IDs not the same
			strcat(rtxt, "/");
			strcat(rtxt, eid);
		}
		else // same or missing, correct element type to topic type
			DITAElemType = 1;
	}
	return rtxt;  // anchor will be DITA TopicID
}

void HXMwr::WriteDITALinkAttrs(HTMwfile *wf, char *chp, unc typ)
{
	// write href, type, format, and scope attrs (default=*)
	// chp is the href link, anchor already set for DITA
	// Use DITATopicType and DITAElemType to set:
	//  type: li, fn, fig, table, section, topictype, or (none)*
	// typ is 1=link to fm file, 2=popup, 3=secwin, 4=xref, 5=ftn, 6=url
	// Use along with href to determine:
	//  format: dita*, pdf, html, ditamap, (ext with no period, like txt)
	//  scope: local*, peer, external

	char *scope = DITALinkScope;
	char *fmt = DITALinkFormat;
	char *type = DITALinkType;
	char *elem = NULL;
	char *chtmp = chp;
	char *hash = strrchr(chp, '#');
	char *slash = NULL;
	bool peer = (scope && !strcmp(scope, "peer"));

	if (DITARelLinkDiv) {   // write attrs then write div
		wf->SelectDiv(PreRelLinkDiv);
		wf->RestoreAttrs();
	}

	if (SetElementIDs)
		wf->strAttr(IDattr, (typ == 4) ? HTMwr::XrefLinkID : HTMwr::HyperLinkID);


	if (!fmt) {
		char *per = NULL;
		if (typ == 6) {        // a URL
			if (hash != NULL)    // has filename, check for ext before anchor
				chtmp = NewName(chp, hash - chp);
			else { // no hash, can't tell dirname from filename
				if (*(chtmp + strlen(chtmp) - 1) == '/')  // a dirname
					fmt = "html";
				else if (((per = strrchr(chtmp, '.')) != NULL)
				 && (!strcmp(per + 1, "pdf")
					|| !strcmp(per + 1, "txt")
					|| !strcmp(per + 1, "dita")
					|| !strcmp(per + 1, "ditamap")))
					fmt = per + 1;  // most likely a file name
				else  // assume domain, dir, or HTML file
					fmt = "html";
			}
		}
		else if (hash != NULL) {  // has an anchor specified
			if ((hash == chp)   // ref to current file part
			 || (strchr(hash, '/') != NULL))  // has element ID
				fmt = "dita";
			else
				chtmp = NewName(chp, hash - chp);
		}
		if (!fmt) {  // must be file name
			if ((per = strrchr(chtmp, '.')) != NULL) {
				if (!strcmp(per + 1, "htm"))
					fmt = "html";
				else  // use ext as format
					fmt = per + 1;
			}
		}
	}

	if (!scope) {
		if (typ == 6)
			scope = "external";
		else if (UseLocalScope)
			scope = "local";
	}

	if (!type
	 && (typ != 6)) {
		if (DITAElemType == 1)
			type = DTopicRoots->find(DITATopicType);
		else switch (DITAElemType) {
			case 2:
				type = "section";
				break;

			case 3:
				type = "table";
				break;

			case 4:
				type = "fig";
				break;

			case 5:
				type = "fn";
				break;

			case 6:
				type = "li";
				break;

			default:
				break;
		}
	}

	if (DITALinkElemID) {
		if (hash
		 && ((slash = strchr(hash, '/')) != NULL))
			*slash = '\0';  // trim off existing ID
		elem = NewName(chp, strlen(DITALinkElemID) + strlen(chp) + 2);
		if (*(chp + strlen(chp) - 1) != '/')
			strcat(elem, "/");
		strcat(elem, DITALinkElemID);
		wf->strAttr("href", elem);
	}
	else if (peer) {  // remove hash part
		*hash = '\0';
		wf->strAttr("href", chp);
	}
	else
		wf->strAttr("href", chp);

	wf->strAttr("scope", scope);
	wf->strAttr("type", type);

	if (fmt) {
		//wf->putEnd();
		wf->strAttr("format", fmt);
	}

	if (chtmp
	 && (chtmp != chp))
		DeleteName(chtmp);
	if (elem)
		DeleteName(elem);

	DITALinkElemID = DITALinkScope = DITALinkFormat = DITALinkType = NULL;

	IsFrXref = ((typ == 4) && !peer) ? true : false;
}


void HXMwr::StartDITALinkDiv(HTMwfile *wf)
{
	if (!HTMwr::RuninDiv)
		PreDITADiv = wf->GetDiv();
	DITALinkDiv = wf->NewDiv();
	wf->SelectDiv(DITALinkDiv);
}


void HXMwr::EndDITALinkDiv(HTMwfile *wf)
{
	// if link is Frame xref, eliminate contents of div; otherwise keep it
	wf->SelectDiv(PreDITADiv);
	if (KeepXrefText
	 || !IsFrXref)
		wf->WriteDiv(DITALinkDiv);
	wf->DeleteDiv(DITALinkDiv);
	DITALinkDiv = 0;
	wf->SelectDiv(PreDITADiv);
}


void HXMwr::StartDITARelLinkDiv(HTMwfile *wf)
{
	PreRelLinkDiv = wf->GetDiv();
	DITARelLinkDiv = wf->NewDiv();
}


void HXMwr::EndDITARelLinkDiv(HTMwfile *wf)
{
	// write ending of elem and linktext tag
	wf->endElem(true);
	wf->putEnd();

	wf->WriteDiv(DITARelLinkDiv);
	wf->DeleteDiv(DITARelLinkDiv);
	DITARelLinkDiv = 0;
	wf->SelectDiv(PreRelLinkDiv);
}


// end of dwhtmdt.cpp

