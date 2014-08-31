// dwhtmds.cpp is the code for dcl HTM DITA specialization, JHG
// Copyright (c) 2007 by Omni Systems, Inc.  All rights reserved.

#include "dcappl.h"
#include "dcctlg.h"
#include "dclist.h"
#include "dcfili.h"
#include "dctxfx.h"
#include "dcfmtr.h"
#include "dcini.h"
#include "dcmacr.h"
#include "dwhtm.h"
#include "dwhtmfl.h"


// preloaded DITA topic type info

DCnlist *HXMwr::DTopicTypes = NULL;
DCnlist *HXMwr::DTopicStarts = NULL;
DCnlist *HXMwr::DTopicRoots = NULL;
DCnlist *HXMwr::CommonNames = NULL;
DCvlist *HXMwr::DTagSets = NULL;
bool HXMwr::UseCommonNames = true;

tagset *HXMwr::DITAtags = NULL;
tagset *HXMwr::DTopicTags = NULL;
tagset *HXMwr::DConceptTags = NULL;
tagset *HXMwr::DTaskTags = NULL;
tagset *HXMwr::DReferenceTags = NULL;
tagset *HXMwr::DGlossentryTags = NULL;
tagset *HXMwr::DMapTags = NULL;
tagset *HXMwr::DBookmapTags = NULL;


// DocBook components

tagset *HXMwr::DocBookTags = NULL;

DCnlist *HXMwr::DocBookBNames = NULL;
DCilist *HXMwr::DocBookBSingle = NULL;
DCnlist *HXMwr::DocBookBElemSetNames = NULL;
DCvlist *HXMwr::DocBookBElemSets = NULL;
DCilist *HXMwr::DocBookBInline = NULL;
DCvlist *HXMwr::DocBookBParents = NULL;
DCilist *HXMwr::DocBookBLevels = NULL;
DCilist *HXMwr::DocBookBAtFirst = NULL;

DCnlist *HXMwr::DocBookANames = NULL;
DCilist *HXMwr::DocBookASingle = NULL;
DCnlist *HXMwr::DocBookAElemSetNames = NULL;
DCvlist *HXMwr::DocBookAElemSets = NULL;
DCilist *HXMwr::DocBookAInline = NULL;
DCvlist *HXMwr::DocBookAParents = NULL;
DCilist *HXMwr::DocBookALevels = NULL;
DCilist *HXMwr::DocBookAAtFirst = NULL;

// topic lists

DCnlist *HXMwr::TopicNames = NULL;
DCilist *HXMwr::TopicSingle = NULL;
DCnlist *HXMwr::TopicElemSetNames = NULL;
DCvlist *HXMwr::TopicElemSets = NULL;
DCilist *HXMwr::TopicInline = NULL;
DCvlist *HXMwr::TopicParents = NULL;
DCilist *HXMwr::TopicLevels = NULL;
DCilist *HXMwr::TopicAtFirst = NULL;

DCnlist *HXMwr::ConceptNames = NULL;
DCilist *HXMwr::ConceptSingle = NULL;
DCnlist *HXMwr::ConceptElemSetNames = NULL;
DCvlist *HXMwr::ConceptElemSets = NULL;
DCilist *HXMwr::ConceptInline = NULL;
DCvlist *HXMwr::ConceptParents = NULL;
DCilist *HXMwr::ConceptLevels = NULL;
DCilist *HXMwr::ConceptAtFirst = NULL;

DCnlist *HXMwr::TaskNames = NULL;
DCilist *HXMwr::TaskSingle = NULL;
DCnlist *HXMwr::TaskElemSetNames = NULL;
DCvlist *HXMwr::TaskElemSets = NULL;
DCilist *HXMwr::TaskInline = NULL;
DCvlist *HXMwr::TaskParents = NULL;
DCilist *HXMwr::TaskLevels = NULL;
DCilist *HXMwr::TaskAtFirst = NULL;

DCnlist *HXMwr::ReferenceNames = NULL;
DCilist *HXMwr::ReferenceSingle = NULL;
DCnlist *HXMwr::ReferenceElemSetNames = NULL;
DCvlist *HXMwr::ReferenceElemSets = NULL;
DCilist *HXMwr::ReferenceInline = NULL;
DCvlist *HXMwr::ReferenceParents = NULL;
DCilist *HXMwr::ReferenceLevels = NULL;
DCilist *HXMwr::ReferenceAtFirst = NULL;

DCnlist *HXMwr::GlossentryNames = NULL;
DCilist *HXMwr::GlossentrySingle = NULL;
DCnlist *HXMwr::GlossentryElemSetNames = NULL;
DCvlist *HXMwr::GlossentryElemSets = NULL;
DCilist *HXMwr::GlossentryInline = NULL;
DCvlist *HXMwr::GlossentryParents = NULL;
DCilist *HXMwr::GlossentryLevels = NULL;
DCilist *HXMwr::GlossentryAtFirst = NULL;

DCnlist *HXMwr::MapNames = NULL;
DCilist *HXMwr::MapSingle = NULL;
DCnlist *HXMwr::MapElemSetNames = NULL;
DCvlist *HXMwr::MapElemSets = NULL;
DCilist *HXMwr::MapInline = NULL;
DCvlist *HXMwr::MapParents = NULL;
DCilist *HXMwr::MapLevels = NULL;
DCilist *HXMwr::MapAtFirst = NULL;

DCnlist *HXMwr::BookmapNames = NULL;
DCilist *HXMwr::BookmapSingle = NULL;
DCnlist *HXMwr::BookmapElemSetNames = NULL;
DCvlist *HXMwr::BookmapElemSets = NULL;
DCilist *HXMwr::BookmapInline = NULL;
DCvlist *HXMwr::BookmapParents = NULL;
DCilist *HXMwr::BookmapLevels = NULL;
DCilist *HXMwr::BookmapAtFirst = NULL;


// DocBook functions

void HXMwr::InitDocBook(void)
{
	// set defaults for .ini settings
	HTMwr::XMLRoot = "book";
	HTMwr::XHLangAttr = "lang";
	HTMwr::RemEmptyParas = true;
	HTMwr::RemEmptyTbParas = true;
	IDattr = "xml:id";
	DefDITAPar = "para";
	DefDITAChar = "phrase";
	DefDITAImg = "imagedata";
	HTMwr::ImgSrcAttr = "fileref";
	HTMwr::UseAnums = false;
	HTMwr::NumericCharRefs = false;
	HTMwr::AllowNobr = false;

	HTMwr::ATagName = "xref";
	HTMwr::HrefAttribute = "linkend";
	HTMwr::NoANames = true;
	HTMwr::UseHash = false;
	HTMwr::UseUlink = true;
	HTMwr::RemoveXrefHotspots = true;
	HTMwr::UseListedXrefFilesOnly = true;

	HTMwr::FootType = 'I';
	HTMwr::FootInlineTag = "footnote";
	HTMwr::FootInlineParaTag = "para";
	HTMwr::FootInlineIDPrefix = "foot";
	HTMwr::UseFootXrefTag = true;
	HTMwr::FootInlineXrefTag = "footnoteref";

	HTMwr::ProhibitPrefix = true;
	HTMwr::ProhibitSuffix = true;
	HTMwr::AnchorFrames = false;

	HTMwr::UseTypographicElements = false;
	HTMwr::UseInformaltableTag = true;
	HTMwr::CALS = true;
	HXref = true;

	HTMwr::ClassIsTag = false;
	HTMwr::SelectorIncludesTag = false;
	HTMwr::WriteClassAttributes = false;
	HTMwr::WriteCssStylesheet = 'N';
	HTMwr::WriteCssLink = false;

	// these apply if running book as one file
	if (IniFile
	 && (IniFile->Section("FDK")
	  || IniFile->Section("Setup"))
	 && IniFile->Find("XMLSuffix")
	 && !strcmp(".ent", IniFile->StrRight()))
		DocBookBook = true;

	if (DocBookBook) {  // reverse defaults
		HTMwr::UseDOCTYPE = false;
		HTMwr::UseXMLRoot = false;
		WriteBookFile = true;
	}


	// set up DTopicTypes
	DTopicTypes = new DCnlist;
	DTopicStarts = new DCnlist;

	CommonNames = new DCnlist;
	CommonNames->init(DTtag::CommonNamesSet);

	// set up spec ini section names
	DSpecSects = new DCnlist;
	DSpecSects->init(DSpecSectsSet);

	// create standard tagsets
	DTagSets = new DCvlist;
	DTopicRoots = new DCnlist;


	// set up content model
	long tagid = 0;
	SetDocBookTypeB(++tagid);
	SetDocBookTypeA(++tagid);

	if (IniFile
	 && IniFile->Section("DocBookOptions")
	 && IniFile->Find("ContentModel"))   // content model to load now
		DITALoadSpecial(IniFile->StrRight());

	SetDocBookType(1);  // initialize to book set
}


void HXMwr::SetDocBookType(long tagid)
{
	DocBookTags = (tagset *) DTagSets->find(tagid);
	HTMwr::PrologDType = DocBookTags->dtype;
	HTMwr::PrologDTypeSet = true;
	HTMwr::PrologDTD = DocBookTags->dtd;
	HTMwr::PrologDTDSet = true;

	InitTags(DocBookTags, PutDITAStart, PutDITAStartFull,
		                    PutDITAEnd, PutDITAEndFull);
}

void HXMwr::SetDocBookTypeB(long tagid)
{
	// set with book as root
	DocBookBNames = new DCnlist;
	DocBookBNames->init(DBtag::DocBookBNamesSet);

	DocBookBElemSetNames = new DCnlist;
	DocBookBElemSetNames->init(DBtag::DocBookBElemSetNamesSet);

	DocBookBElemSets = new DCvlist;
	DocBookBElemSets->initdl(DBtag::DocBookBElemSetsSet);

	DocBookBSingle = new DCilist;
	DocBookBSingle->init(DBtag::DocBookBSingleSet);

	DocBookBInline = new DCilist;
	DocBookBInline->init(DBtag::DocBookBInlineSet);

	DocBookBLevels = new DCilist;
	DocBookBLevels->init(DBtag::DocBookBLevelsSet);

	DocBookBAtFirst = new DCilist;
	DocBookBAtFirst->init(DBtag::DocBookBAtFirstSet);

	DocBookBParents = new DCvlist;
	DCvlist *rescan = MergeElemLists(DocBookBParents, DocBookBElemSets);
	assert(rescan==NULL);

	DocBookTags = new tagset;
	DocBookTags->dtype = DBtag::PrologDTypeDocBook;
	DocBookTags->dtd = DBtag::PrologDTDDocBook;
	DocBookTags->names = DocBookBNames;
	DocBookTags->single = DocBookBSingle;
	DocBookTags->esetnames = DocBookBElemSetNames;
	DocBookTags->esetlists = DocBookBElemSets;
	DocBookTags->levels = DocBookBLevels;
	DocBookTags->parents = DocBookBParents;
	DocBookTags->inlin = DocBookBInline;
	DocBookTags->atfirst = DocBookBAtFirst;

	DTagSets->add(DocBookTags, tagid);
	DTopicRoots->add(DocBookBNames->find(1), tagid);
	DTopicTypes->add(DocBookBNames->find(1), tagid);
}

void HXMwr::SetDocBookTypeA(long tagid)
{
	// set with article as root
	DocBookANames = new DCnlist;
	DocBookANames->init(DBtag::DocBookANamesSet);

	DocBookAElemSetNames = new DCnlist;
	DocBookAElemSetNames->init(DBtag::DocBookAElemSetNamesSet);

	DocBookAElemSets = new DCvlist;
	DocBookAElemSets->initdl(DBtag::DocBookAElemSetsSet);

	DocBookASingle = new DCilist;
	DocBookASingle->init(DBtag::DocBookASingleSet);

	DocBookAInline = new DCilist;
	DocBookAInline->init(DBtag::DocBookAInlineSet);

	DocBookALevels = new DCilist;
	DocBookALevels->init(DBtag::DocBookALevelsSet);

	DocBookAAtFirst = new DCilist;
	DocBookAAtFirst->init(DBtag::DocBookAAtFirstSet);

	DocBookAParents = new DCvlist;
	DCvlist *rescan = MergeElemLists(DocBookAParents, DocBookAElemSets);
	assert(rescan==NULL);

	DocBookTags = new tagset;
	DocBookTags->dtype = DBtag::PrologDTypeDocBook;
	DocBookTags->dtd = DBtag::PrologDTDDocBook;
	DocBookTags->names = DocBookANames;
	DocBookTags->single = DocBookASingle;
	DocBookTags->esetnames = DocBookAElemSetNames;
	DocBookTags->esetlists = DocBookAElemSets;
	DocBookTags->levels = DocBookALevels;
	DocBookTags->parents = DocBookAParents;
	DocBookTags->inlin = DocBookAInline;
	DocBookTags->atfirst = DocBookAAtFirst;

	DTagSets->add(DocBookTags, tagid);
	DTopicRoots->add(DocBookANames->find(1), tagid);
	DTopicTypes->add(DocBookANames->find(1), tagid);
}


// DITA functions

void HXMwr::InitDITA(void)
{
	if (IniFile
	 && IniFile->Section("DITAOptions")) {
		if (IniFile->Find("DITAVer"))
			DITAVer = (unc) IniFile->IntRight();
		//if (DITAVer > 1)
		//	DITAVer = 1;
		if (IniFile->Find("UseDTDPath"))
			UseDTDPath = IniFile->BoolRight();
		if ((DITAVer != 1)
		 || !UseDTDPath)
			DTtag::SetDITAVer();
	}

	// set defaults for .ini settings
	HTMwr::WriteClassAttributes = false;
	HTMwr::ClassIsTag = false;
	HTMwr::SelectorIncludesTag = false;
	HTMwr::StartingSplit = false;
	HTMwr::AllowNobr = false;
	HTMwr::RuninType = 'N';
	HTMwr::NoANames = true;
	HTMwr::ATagName = "xref";
	HTMwr::NoEnc = false;
	HTMwr::NumericCharRefs = false;
	HTMwr::RemEmptyParas = true;
	HTMwr::RemEmptyTbParas = true;
	HTMwr::EmptyParaContent = "";
	HTMwr::EmptyTbCellContent = "";
	HTMwr::UseSortString = true;
	HTMwr::UseTypographicElements = false;
	HTMwr::UseAnums = false;
	HTMwr::FootType = 'I';
	HTMwr::FootInlineTag = "fn";
	HTMwr::FootInlineParaTag = NULL;
	HTMwr::UseFootXrefTag = false;
	HTMwr::ConditionCharTag = "ph";
	HTMwr::CALS = true;
	HXref = true;
	HTMwr::ProhibitPrefix = true;
	HTMwr::ProhibitSuffix = true;
	HTMwr::AnchorFrames = false;

	//if (IniFile
	// && IniFile->Section("FileSequence")
	// && IniFile->Find(CurrFileName))
	//	DITAMapTitle = IniFile->StrRight();

	InitDITATableTypes();

	// set up DTopicTypes
	DTopicTypes = new DCnlist;
	DTopicTypes->init(DTtag::DTopicTypesSet);
	DTopicStarts = new DCnlist;
	DTopicStarts->init(DTtag::DTopicStartsSet);
	DITATopicTypes.init(DTtag::DRelTopicTypesSet);
	DITATypeNameCount = 3;

	CommonNames = new DCnlist;
	CommonNames->init(DTtag::CommonNamesSet);

	// set up spec ini section names
	DSpecSects = new DCnlist;
	DSpecSects->init(DSpecSectsSet);

	// create and populate standard tagsets
	DTagSets = new DCvlist;
	DTopicRoots = new DCnlist;
	long tagid = 1;

	SetDITATopicType(tagid++);
	SetDITAConceptType(tagid++);
	SetDITATaskType(tagid++);
	SetDITAReferenceType(tagid++);
	if (DITAVer > 0)
		SetDITAGlossentryType(tagid++);
	SetDITAMapType(tagid++);
	if (DITAVer > 0)
		SetDITABookmapType(tagid++);

	if (IniFile
	 && IniFile->Section("DITAOptions")
	 && IniFile->Find("SpecIniDir"))
		SpecIniDir = IniFile->StrRight();

	if (IniFile->Section("DITAContentModels")
	 || IniFile->Section("DITASpec")) {  // content models to load now
		while (IniFile->NextItem())
			DITALoadSpecial(IniFile->StrLeft());
	}

	SetDITATable(DefDITATbl);

	if (IniFile->Section("DITATopicFileNamePrefix")) {
		long tn = 0;
		while (IniFile->NextItem()) {
			if ((tn = DTopicTypes->getitem(IniFile->StrLeft())) != 0) {
				DITATypeNamePrefixList.add(IniFile->StrRight(), tn);
				DITATypeNamePrefixUsed = true;
			}
		}
	}

#if 0
	if (IniFile->Section("DITAElementSets")) {  // content models to load now
		while (IniFile->NextItem()) {
			DElemSetNames.add(++DElemSetCount, IniFile->StrLeft());
			DElemSets.add(DElemSetCount, IniFile->StrArrRight());
		}
	}
#endif
}

void DTtag::SetDITAVer(void)
{
	unc dval = HXMwr::DITAVer;

	if (HXMwr::DITAVer > 1)
		dval = 1;

	// set init ptrs to correct version
	DTopicTypesSet = DTopicTypesSets[dval];
	DTopicStartsSet = DTopicStartsSets[dval];
	DRelTopicTypesSet = DRelTopicTypesSets[dval];
	CommonNamesSet = CommonNamesSets[dval];

	PrologDTypeDita = PrologDTypeDitas[dval];
	PrologDTypeTopic = PrologDTypeTopics[dval];
	PrologDTypeConcept = PrologDTypeConcepts[dval];
	PrologDTypeTask = PrologDTypeTasks[dval];
	PrologDTypeReference = PrologDTypeReferences[dval];
	if (dval > 0)
		PrologDTypeGlossentry = PrologDTypeGlossentrys[dval];
	PrologDTypeMap = PrologDTypeMaps[dval];
	PrologDTypeBookmap = PrologDTypeBookmaps[dval];

	if (HXMwr::UseDTDPath) {
		PrologDTDDita = PrologDTDDitas[dval];
		PrologDTDTopic = PrologDTDTopics[dval];
		PrologDTDConcept = PrologDTDConcepts[dval];
		PrologDTDTask = PrologDTDTasks[dval];
		PrologDTDReference = PrologDTDReferences[dval];
		PrologDTDMap = PrologDTDMaps[dval];
		if (dval > 0) {
			PrologDTDGlossentry = PrologDTDGlossentrys[dval];
			PrologDTDBookmap = PrologDTDBookmaps[dval];
		}
	}
	else {
		PrologDTDDita = "  \"ditabase.dtd\"";
		PrologDTDTopic = "  \"topic.dtd\"";
		PrologDTDConcept = "  \"concept.dtd\"";
		PrologDTDTask = "  \"task.dtd\"";
		PrologDTDReference = "  \"reference.dtd\"";
		PrologDTDMap = "  \"map.dtd\"";
		if (dval > 0) {
			PrologDTDGlossentry = "  \"glossary.dtd\"";
			PrologDTDBookmap = "  \"bookmap.dtd\"";
		}
	}

	DTtag::SetDITAVerSets(dval);
}


void DTtag::SetDITAVerSets(unc dval)
{
	TopicNamesSet = TopicNamesSets[dval];
	TopicInlineSet = TopicInlineSets[dval];
	TopicSingleSet = TopicSingleSets[dval];
	TopicElemSetsSet = TopicElemSetsSets[dval];
	TopicElemSetNamesSet = TopicElemSetNamesSets[dval];
	TopicLevelsSet = TopicLevelsSets[dval];
	TopicAtFirstSet = TopicAtFirstSets[dval];

	ConceptNamesSet = ConceptNamesSets[dval];
	ConceptInlineSet = ConceptInlineSets[dval];
	ConceptSingleSet = ConceptSingleSets[dval];
	ConceptElemSetsSet = ConceptElemSetsSets[dval];
	ConceptElemSetNamesSet = ConceptElemSetNamesSets[dval];
	ConceptLevelsSet = ConceptLevelsSets[dval];
	ConceptAtFirstSet = ConceptAtFirstSets[dval];

	TaskNamesSet = TaskNamesSets[dval];
	TaskInlineSet = TaskInlineSets[dval];
	TaskSingleSet = TaskSingleSets[dval];
	TaskElemSetsSet = TaskElemSetsSets[dval];
	TaskElemSetNamesSet = TaskElemSetNamesSets[dval];
	TaskLevelsSet = TaskLevelsSets[dval];
	TaskAtFirstSet = TaskAtFirstSets[dval];

	ReferenceNamesSet = ReferenceNamesSets[dval];
	ReferenceInlineSet = ReferenceInlineSets[dval];
	ReferenceSingleSet = ReferenceSingleSets[dval];
	ReferenceElemSetsSet = ReferenceElemSetsSets[dval];
	ReferenceElemSetNamesSet = ReferenceElemSetNamesSets[dval];
	ReferenceLevelsSet = ReferenceLevelsSets[dval];
	ReferenceAtFirstSet = ReferenceAtFirstSets[dval];

	if (dval > 0) {
		GlossentryNamesSet = GlossentryNamesSets[dval];
		GlossentryInlineSet = GlossentryInlineSets[dval];
		GlossentrySingleSet = GlossentrySingleSets[dval];
		GlossentryElemSetsSet = GlossentryElemSetsSets[dval];
		GlossentryElemSetNamesSet = GlossentryElemSetNamesSets[dval];
		GlossentryLevelsSet = GlossentryLevelsSets[dval];
		GlossentryAtFirstSet = GlossentryAtFirstSets[dval];
	}

	MapNamesSet = MapNamesSets[dval];
	MapInlineSet = MapInlineSets[dval];
	MapSingleSet = MapSingleSets[dval];
	MapElemSetsSet = MapElemSetsSets[dval];
	MapElemSetNamesSet = MapElemSetNamesSets[dval];
	MapLevelsSet = MapLevelsSets[dval];
	MapAtFirstSet = MapAtFirstSets[dval];

	if (dval > 0) {
		BookmapNamesSet = BookmapNamesSets[dval];
		BookmapInlineSet = BookmapInlineSets[dval];
		BookmapSingleSet = BookmapSingleSets[dval];
		BookmapElemSetsSet = BookmapElemSetsSets[dval];
		BookmapElemSetNamesSet = BookmapElemSetNamesSets[dval];
		BookmapLevelsSet = BookmapLevelsSets[dval];
		BookmapAtFirstSet = BookmapAtFirstSets[dval];
	}
}


void HXMwr::SetDITATopicType(long tagid)
{
	TopicNames = new DCnlist;
	TopicNames->init(DTtag::TopicNamesSet);

	TopicElemSetNames = new DCnlist;
	TopicElemSetNames->init(DTtag::TopicElemSetNamesSet);

	TopicElemSets = new DCvlist;
	TopicElemSets->initdl(DTtag::TopicElemSetsSet);

	TopicSingle = new DCilist;
	TopicSingle->init(DTtag::TopicSingleSet);

	TopicInline = new DCilist;
	TopicInline->init(DTtag::TopicInlineSet);

	TopicLevels = new DCilist;
	TopicLevels->init(DTtag::TopicLevelsSet);

	TopicAtFirst = new DCilist;
	TopicAtFirst->init(DTtag::TopicAtFirstSet);

	TopicParents = new DCvlist;
	DCvlist *rescan = MergeElemLists(TopicParents, TopicElemSets);
	assert(rescan==NULL);

	DTopicTags = new tagset;
	DTopicTags->setname = DTopicTypes->find(tagid);
	DTopicTags->start = DTopicStarts->find(tagid);
	DTopicTags->dtype = DTtag::PrologDTypeTopic;
	DTopicTags->dtd = DTtag::PrologDTDTopic;
	DTopicTags->names = TopicNames;
	DTopicTags->single = TopicSingle;
	DTopicTags->esetnames = TopicElemSetNames;
	DTopicTags->esetlists = TopicElemSets;
	DTopicTags->levels = TopicLevels;
	DTopicTags->parents = TopicParents;
	DTopicTags->inlin = TopicInline;
	DTopicTags->atfirst = TopicAtFirst;
	DTopicTags->tblnames = new DCnlist;
	DTopicTags->tblnames->add("table", 1);
	DTopicTags->tblnames->add("simple", 2);
	DTopicTags->tbllists = new DCvlist;
	DTopicTags->tbllists->add(&DITAComplexTable, 1);
	DTopicTags->tbllists->add(&DITASimpleTable, 2);
	DTagSets->add(DTopicTags, tagid);

	DTopicRoots->add(TopicNames->find(1), tagid);
}


void HXMwr::SetDITAConceptType(long tagid)
{
	ConceptNames = new DCnlist;
	ConceptNames->init(DTtag::ConceptNamesSet);

	ConceptElemSetNames = new DCnlist;
	ConceptElemSetNames->init(DTtag::ConceptElemSetNamesSet);

	ConceptElemSets = new DCvlist;
	ConceptElemSets->initdl(DTtag::ConceptElemSetsSet);

	ConceptSingle = new DCilist;
	ConceptSingle->init(DTtag::ConceptSingleSet);

	ConceptInline = new DCilist;
	ConceptInline->init(DTtag::ConceptInlineSet);

	ConceptLevels = new DCilist;
	ConceptLevels->init(DTtag::ConceptLevelsSet);

	ConceptAtFirst = new DCilist;
	ConceptAtFirst->init(DTtag::ConceptAtFirstSet);

	ConceptParents = new DCvlist;
	DCvlist *rescan = MergeElemLists(ConceptParents, ConceptElemSets);
	assert(rescan==NULL);

	DConceptTags = new tagset;
	DConceptTags->setname = DTopicTypes->find(tagid);
	DConceptTags->start = DTopicStarts->find(tagid);
	DConceptTags->dtype = DTtag::PrologDTypeConcept;
	DConceptTags->dtd = DTtag::PrologDTDConcept;
	DConceptTags->names = ConceptNames;
	DConceptTags->single = ConceptSingle;
	DConceptTags->esetnames = ConceptElemSetNames;
	DConceptTags->esetlists = ConceptElemSets;
	DConceptTags->levels = ConceptLevels;
	DConceptTags->parents = ConceptParents;
	DConceptTags->inlin = ConceptInline;
	DConceptTags->atfirst = ConceptAtFirst;
	DConceptTags->tblnames = DTopicTags->tblnames;
	DConceptTags->tbllists = DTopicTags->tbllists;
	DTagSets->add(DConceptTags, tagid);

	DTopicRoots->add(ConceptNames->find(1), tagid);
}


void HXMwr::SetDITATaskType(long tagid)
{
	TaskNames = new DCnlist;
	TaskNames->init(DTtag::TaskNamesSet);

	TaskElemSetNames = new DCnlist;
	TaskElemSetNames->init(DTtag::TaskElemSetNamesSet);

	TaskElemSets = new DCvlist;
	TaskElemSets->initdl(DTtag::TaskElemSetsSet);

	TaskSingle = new DCilist;
	TaskSingle->init(DTtag::TaskSingleSet);

	TaskInline = new DCilist;
	TaskInline->init(DTtag::TaskInlineSet);

	TaskLevels = new DCilist;
	TaskLevels->init(DTtag::TaskLevelsSet);

	TaskAtFirst = new DCilist;
	TaskAtFirst->init(DTtag::TaskAtFirstSet);

	TaskParents = new DCvlist;
	DCvlist *rescan = MergeElemLists(TaskParents, TaskElemSets);
	assert(rescan==NULL);

	DTaskTags = new tagset;
	DTaskTags->setname = DTopicTypes->find(tagid);
	DTaskTags->start = DTopicStarts->find(tagid);
	DTaskTags->dtype = DTtag::PrologDTypeTask;
	DTaskTags->dtd = DTtag::PrologDTDTask;
	DTaskTags->names = TaskNames;
	DTaskTags->single = TaskSingle;
	DTaskTags->esetnames = TaskElemSetNames;
	DTaskTags->esetlists = TaskElemSets;
	DTaskTags->levels = TaskLevels;
	DTaskTags->parents = TaskParents;
	DTaskTags->inlin = TaskInline;
	DTaskTags->atfirst = TaskAtFirst;
	DTaskTags->tblnames = new DCnlist;
	DTaskTags->tblnames->add("table", 1);
	DTaskTags->tblnames->add("simple", 2);
	DTaskTags->tblnames->add("choice", 3);
	DTaskTags->tbllists = new DCvlist;
	DTaskTags->tbllists->add(&DITAComplexTable, 1);
	DTaskTags->tbllists->add(&DITASimpleTable, 2);
	DTaskTags->tbllists->add(&DITAChoiceTable, 3);
	DTagSets->add(DTaskTags, tagid);

	DTopicRoots->add(TaskNames->find(1), tagid);
}


void HXMwr::SetDITAReferenceType(long tagid)
{
	ReferenceNames = new DCnlist;
	ReferenceNames->init(DTtag::ReferenceNamesSet);

	ReferenceElemSetNames = new DCnlist;
	ReferenceElemSetNames->init(DTtag::ReferenceElemSetNamesSet);

	ReferenceElemSets = new DCvlist;
	ReferenceElemSets->initdl(DTtag::ReferenceElemSetsSet);

	ReferenceSingle = new DCilist;
	ReferenceSingle->init(DTtag::ReferenceSingleSet);

	ReferenceInline = new DCilist;
	ReferenceInline->init(DTtag::ReferenceInlineSet);

	ReferenceLevels = new DCilist;
	ReferenceLevels->init(DTtag::ReferenceLevelsSet);

	ReferenceAtFirst = new DCilist;
	ReferenceAtFirst->init(DTtag::ReferenceAtFirstSet);

	ReferenceParents = new DCvlist;
	DCvlist *rescan = MergeElemLists(ReferenceParents, ReferenceElemSets);
	assert(rescan==NULL);

	DReferenceTags = new tagset;
	DReferenceTags->setname = DTopicTypes->find(tagid);
	DReferenceTags->start = DTopicStarts->find(tagid);
	DReferenceTags->dtype = DTtag::PrologDTypeReference;
	DReferenceTags->dtd = DTtag::PrologDTDReference;
	DReferenceTags->names = ReferenceNames;
	DReferenceTags->single = ReferenceSingle;
	DReferenceTags->esetnames = ReferenceElemSetNames;
	DReferenceTags->esetlists = ReferenceElemSets;
	DReferenceTags->levels = ReferenceLevels;
	DReferenceTags->parents = ReferenceParents;
	DReferenceTags->inlin = ReferenceInline;
	DReferenceTags->atfirst = ReferenceAtFirst;
	DReferenceTags->tblnames = new DCnlist;
	DReferenceTags->tblnames->add("table", 1);
	DReferenceTags->tblnames->add("simple", 2);
	DReferenceTags->tblnames->add("property", 3);
	DReferenceTags->tbllists = new DCvlist;
	DReferenceTags->tbllists->add(&DITAComplexTable, 1);
	DReferenceTags->tbllists->add(&DITASimpleTable, 2);
	DReferenceTags->tbllists->add(&DITAPropertiesTable, 3);
	DTagSets->add(DReferenceTags, tagid);

	DTopicRoots->add(ReferenceNames->find(1), tagid);
}

void HXMwr::SetDITAGlossentryType(long tagid)
{
	GlossentryNames = new DCnlist;
	GlossentryNames->init(DTtag::GlossentryNamesSet);

	GlossentryElemSetNames = new DCnlist;
	GlossentryElemSetNames->init(DTtag::GlossentryElemSetNamesSet);

	GlossentryElemSets = new DCvlist;
	GlossentryElemSets->initdl(DTtag::GlossentryElemSetsSet);

	GlossentrySingle = new DCilist;
	GlossentrySingle->init(DTtag::GlossentrySingleSet);

	GlossentryInline = new DCilist;
	GlossentryInline->init(DTtag::GlossentryInlineSet);

	GlossentryLevels = new DCilist;
	GlossentryLevels->init(DTtag::GlossentryLevelsSet);

	GlossentryAtFirst = new DCilist;
	GlossentryAtFirst->init(DTtag::GlossentryAtFirstSet);

	GlossentryParents = new DCvlist;
	DCvlist *rescan = MergeElemLists(GlossentryParents, GlossentryElemSets);
	assert(rescan==NULL);

	DGlossentryTags = new tagset;
	DGlossentryTags->setname = DTopicTypes->find(tagid);
	DGlossentryTags->start = DTopicStarts->find(tagid);
	DGlossentryTags->dtype = DTtag::PrologDTypeGlossentry;
	DGlossentryTags->dtd = DTtag::PrologDTDGlossentry;
	DGlossentryTags->names = GlossentryNames;
	DGlossentryTags->single = GlossentrySingle;
	DGlossentryTags->esetnames = GlossentryElemSetNames;
	DGlossentryTags->esetlists = GlossentryElemSets;
	DGlossentryTags->levels = GlossentryLevels;
	DGlossentryTags->parents = GlossentryParents;
	DGlossentryTags->inlin = GlossentryInline;
	DGlossentryTags->atfirst = GlossentryAtFirst;

	DGlossentryTags->tblnames = new DCnlist;
	DGlossentryTags->tblnames->add("table", 1);
	DGlossentryTags->tblnames->add("simple", 2);

	DGlossentryTags->tbllists = new DCvlist;
	DGlossentryTags->tbllists->add(&DITAComplexTable, 1);
	DGlossentryTags->tbllists->add(&DITASimpleTable, 2);

	DTagSets->add(DGlossentryTags, tagid);

	DTopicRoots->add(GlossentryNames->find(1), tagid);
}

void HXMwr::SetDITAMapType(long tagid)
{
	MapNames = new DCnlist;
	MapNames->init(DTtag::MapNamesSet);

	MapElemSetNames = new DCnlist;
	MapElemSetNames->init(DTtag::MapElemSetNamesSet);

	MapElemSets = new DCvlist;
	MapElemSets->initdl(DTtag::MapElemSetsSet);

	MapSingle = new DCilist;
	MapSingle->init(DTtag::MapSingleSet);

	MapInline = new DCilist;
	MapInline->init(DTtag::MapInlineSet);

	MapLevels = new DCilist;
	MapLevels->init(DTtag::MapLevelsSet);

	MapAtFirst = new DCilist;
	MapAtFirst->init(DTtag::MapAtFirstSet);

	MapParents = new DCvlist;
	DCvlist *rescan = MergeElemLists(MapParents, MapElemSets);
	assert(rescan==NULL);

	DMapTags = new tagset;
	DMapTags->setname = DTopicTypes->find(tagid);
	DMapTags->start = DTopicStarts->find(tagid);
	DMapTags->dtype = DTtag::PrologDTypeMap;
	DMapTags->dtd = DTtag::PrologDTDMap;
	DMapTags->names = MapNames;
	DMapTags->single = MapSingle;
	DMapTags->esetnames = MapElemSetNames;
	DMapTags->esetlists = MapElemSets;
	DMapTags->levels = MapLevels;
	DMapTags->parents = MapParents;
	DMapTags->inlin = MapInline;
	DMapTags->atfirst = MapAtFirst;
	DMapTags->tblnames = new DCnlist;
	DMapTags->tbllists = new DCvlist;

	DTagSets->add(DMapTags, tagid);

	DTopicRoots->add(MapNames->find(1), tagid);
}

void HXMwr::SetDITABookmapType(long tagid)
{
	BookmapNames = new DCnlist;
	BookmapNames->init(DTtag::BookmapNamesSet);

	BookmapElemSetNames = new DCnlist;
	BookmapElemSetNames->init(DTtag::BookmapElemSetNamesSet);

	BookmapElemSets = new DCvlist;
	BookmapElemSets->initdl(DTtag::BookmapElemSetsSet);

	BookmapSingle = new DCilist;
	BookmapSingle->init(DTtag::BookmapSingleSet);

	BookmapInline = new DCilist;
	BookmapInline->init(DTtag::BookmapInlineSet);

	BookmapLevels = new DCilist;
	BookmapLevels->init(DTtag::BookmapLevelsSet);

	BookmapAtFirst = new DCilist;
	BookmapAtFirst->init(DTtag::BookmapAtFirstSet);

	BookmapParents = new DCvlist;
	DCvlist *rescan = MergeElemLists(BookmapParents, BookmapElemSets);
	assert(rescan==NULL);

	DBookmapTags = new tagset;
	DBookmapTags->setname = DTopicTypes->find(tagid);
	DBookmapTags->start = DTopicStarts->find(tagid);
	DBookmapTags->dtype = DTtag::PrologDTypeBookmap;
	DBookmapTags->dtd = DTtag::PrologDTDBookmap;
	DBookmapTags->names = BookmapNames;
	DBookmapTags->single = BookmapSingle;
	DBookmapTags->esetnames = BookmapElemSetNames;
	DBookmapTags->esetlists = BookmapElemSets;
	DBookmapTags->levels = BookmapLevels;
	DBookmapTags->parents = BookmapParents;
	DBookmapTags->inlin = BookmapInline;
	DBookmapTags->atfirst = BookmapAtFirst;

	DTagSets->add(DBookmapTags, tagid);

	DTopicRoots->add(BookmapNames->find(1), tagid);
}


DCvlist *HXMwr::MergeElemLists(DCvlist *parents, DCvlist *esetlists)
{
	// for each elemset in esetlists, write a new one
	// in parents with any elem set refs expanded

	long sid = 0;
	DClist *esl = NULL;
	DClist *mpl = NULL;
	DClist *epl = NULL;
	long esval = 0;

	DCvlist *rescan = new DCvlist;

	while ((esetlists = esetlists->next) != NULL) {
		sid = esetlists->id;
		esl = (DClist *) esetlists->vp;
		mpl = new DClist;
		parents->add(mpl, sid);
		while ((esl = esl->next) != NULL) {
			esval = esl->id;
			if (esval > ESetBase)  { // eset ref
				// insert copy of referenced es list
				epl = (DClist *) parents->find(esval); 
				if (epl == NULL) { // forward reference
					rescan->add(mpl, esval);
					mpl->add(esval);
				}
				else {
					while ((epl = epl->next) != NULL) {
						mpl->add(epl->id);
						if (epl->id > ESetBase)
							rescan->add(mpl, epl->id);
					}
				}
			}
			else {  // element ref
				mpl->add(esval);
			}
		}	
	}

	DClist *tmpl = NULL;
	DClist *mpls = NULL;
	long lcount = 1000;

	while ((rescan = rescan->next) != NULL) {
		if (--lcount <= 0)
			break;  // circular logic
		esval = rescan->id;
		mpl = mpls = (DClist *) rescan->vp;
		if ((epl = (DClist *) parents->find(esval)) == NULL) {
			if (!rescan)
				rescan = new DCvlist;
			rescan->add(mpl, esval);  // queue up again
		}
		else {
			while (mpl->next != NULL) {
				mpl = mpl->next;
				if (mpl->id == esval) {
					epl = epl->next;  // first real item
					mpl->id = epl->id;
					if (epl->id > ESetBase) {
						if (!rescan)
							rescan = new DCvlist;
						rescan->add(mpls, epl->id);
					}
					break;
				}
			}
			tmpl = mpl->next;
			mpl->next = NULL;
			while ((epl = epl->next) != NULL) {
				mpl->next = new DClist;
				mpl = mpl->next;
				mpl->id = epl->id;
				if (epl->id > ESetBase) {
					if (!rescan)
						rescan = new DCvlist;
					rescan->add(mpls, epl->id);
				}
			}
			mpl->next = tmpl;
		}
	}
	return rescan;  // will be NULL if all resolved
}


// specialization
/*

// These sections are in files named DITAtopic.ini, etc., where "topic"
//  is replaced by the actual TopicType, located in the SpecIniDir.
//  They are processed the first time the TopicType is encountered.
//  Then we do a fast binary dump of the resulting lists for use with
//  subsequent files, until the .ini is modified.  This allows us to
//  use a DITAbase.ini, etc., for the predefined types too.

secinfo HXMwr::TopicIniSects[] = {
 { "Topic", set, NULL, ISecDITASpec, true },
 { "TopicParents", pcstyle, "StyleSet", NULL, false, str, &Topic.dtpar },
 { "TopicInline", pcstyle, "StyleSet", NULL, false, usnum, &Topic.dtinl },
 { "TopicLevels", pcstyle, "StyleSet", NULL, false, usnum, &Topic.dtlev },
 { "TopicFirst", pcstyle, "StyleSet", NULL, false, str, &Topic.dtfirst },
 { NULL, unknown, NULL, NULL, false }
};

setinfo HXMwr::ISecDITASpec[] = {
 { "TopicType", str, &Topic.typ, NULL },
 { "TopicBody", str, &Topic.body, NULL },
 { "PrologDType", str, &Topic.pdt, NULL },
 { "PrologDTD", str, &Topic.dtd, NULL },
 { "TopicDerivation", str, &Topic.deriv, NULL },
 { NULL, spec, NULL, NULL }
};

*/



// reading DITA*.ini files


char *HXMwr::SpecIniDir = "./";  // path for specialization .inis

DCnlist *HXMwr::DSpecSects;

npair HXMwr::DSpecSectsSet[] = {
 { 1, "TopicParents"},
 { 2, "ElementSets"},
 { 3, "ElementTypes"},
 { 4, "TopicLevels"},
 { 5, "TopicFirst"},
 { 6, "TopicTables"},

 { 0, NULL }
 };


DCirfile *HXMwr::DITASpecIni(char *sname)
{
	char *fname = NewName(strlen(SpecIniDir) + strlen(sname) + 10);
	strcpy(fname, SpecIniDir);  // may just be ./
	char lc = fname[strlen(fname) - 1];
	if ((lc != '/')
	 && (lc != '\\'))
		strcat(fname, "/");
	if (DITA
	 && _strnicmp("dita", sname, 4)) // allow for doc error
		strcat(fname, "DITA");
	if (!_stricmp(sname, "glossentry"))
		sname = "glossary";
	strcat(fname, sname);
	strcat(fname, ".ini");
	DCirfile *sf = new DCirfile(fname);
	if (sf->FErr())
		return NULL;
	return sf;
}


void HXMwr::DITALoadSpecial(char *sname)
{
	if (!sname || !*sname)
		sname = "topic";

	DCirfile *sf = DITASpecIni(sname);

	if (!_strnicmp("dita", sname, 4))  // allow for doc error
		sname = NewName(sname + 4);

	if (!sf)
		return;

	char *repname = NULL;
	char *derivtop = NULL;
	char *root = NULL;

	if (sf->Section("Topic")) {
		if (sf->Find("TopicRoot")) {
			root = sf->StrRight();
			if (DocBook) {
				if (!_stricmp(root, "book")
				 || !_stricmp(root, "article"))
					repname = root;
				sname = root;
			}
		}
		else
			root = "topic";
		if (sf->Find("ModelName")
		 || sf->Find("Replace")) {
			repname = sf->StrRight();
			if (!_stricmp(repname, "glossentry"))
				repname = "glossary";
		}
		if (sf->Find("TopicDerivation")) {
			derivtop = sf->StrRight();
			if (!_stricmp(derivtop, "glossentry"))
				derivtop = "glossary";
			// if not in types list, call ourselves to create it
			if (!DTopicTypes->getitem(derivtop))
				DITALoadSpecial(derivtop);
			// then clone it and modify the copy
			if (DTopicTypes->getitem(derivtop))
				DITAModSpecial(sf, derivtop, sname, repname);
			return;
		}
	}

	DCnlist *topnames = new DCnlist;
	DCnlist *dupnames = new DCnlist;
	long sectnum = 1;
	char *start = NULL;
	char *pdtype = NULL;
	char *pdtd = NULL;
	char *body = NULL;
	char *dumpfile = NULL;
	char *cppdumpfile = NULL;

	topnames->add(root, 1);

	if (sf->Section("Topic")) {
		if (sf->Find("TopicStart"))
			start = sf->StrRight();
		if (sf->Find("TopicBody")
		 && ((body = sf->StrRight()) != NULL))
			topnames->add(sf->StrRight(), 2);
		if (sf->Find("PrologDType"))
			pdtype = sf->StrRight();
		if (sf->Find("PrologDTD"))
			pdtd = sf->StrRight();
		if (sf->Find("DumpToFile"))
			dumpfile = sf->StrRight();
#ifdef _DEBUG
		if (sf->Find("DumpToCpp"))
			cppdumpfile = sf->StrRight();
#endif
	}

	// get element names and assign ids
	DCnlist *toppars = new DCnlist;
	long id = 2;  // root=1 and body=2 are already set
	long topcount = 0;  // but they don't have toppars
	char *tn = NULL;
	char *tp = NULL;

	if (DocBook) {
		id = 1;  // reserve only the root
	}

	if (sf->Section(DSpecSects->find(sectnum))) {
		while (sf->NextItem()) {
			tn = sf->StrLeft();
			tp = sf->StrRight();
			if (!topnames->operator()(tn)) {
				topnames->add(tn, ++id);
				toppars->add(tp, id);
			}
			else
				dupnames->add(tn, sectnum);
		}
		topcount = id;
	}

	// pass 1, get set names and assign ids
	DCnlist *esnames = new DCnlist;
	char *sn = NULL;

	if (sf->Section(DSpecSects->find(sectnum = 2))) {
		id = ESetBase;  // set bit to identify sets
		while (sf->NextItem()) {
			sn = sf->StrLeft();
			if (!esnames->operator()(sn))
				esnames->add(sn, ++id);
			else
				dupnames->add(sn, sectnum);
		}
	}

	// Make list of unresolved names for error message.
	DClist *undefnames = new DClist;
	DClist *unseqnames = new DClist;
	DCvlist *rescan = new DCvlist;

	// pass 2, parse element sets
	DCvlist *espars = new DCvlist;
	DClist *esl = NULL;
	long escount = 0;
	long esval = 0;
	char *esp = NULL;

	if (sf->Section(DSpecSects->find(sectnum))) {
		id = ESetBase;  // restart id at first item
		while (sf->NextItem()) {
			espars->add(esl = new DClist, ++id);
			if (sf->ListRight()) {  // make content the ID number now
				while ((esp = sf->ListItem()) != 0) {
					if (*esp == '*')  { // eset ref
						if ((esval = esnames->getitem(esp)) == 0)
							undefnames->add((long) esp);  // name missing, err
						else if (esval > id)
							unseqnames->add((long) esp);  // not resolved yet, err
						else  // add to list
							esl->add(esval);
					}
					else {  // element ref
						if ((esval = topnames->getitem(esp)) == 0)
							undefnames->add((long) esp);  // name missing, err
						else   // add parent to list
							esl->add(esval);
					}
				}
			}
		}
		escount = id;
	}

	DCvlist *parsets = new DCvlist;
	DClist *mpl = NULL;
	DClist *epl = NULL;
	long inum = 0;
	char *chp = NULL;

	// Resolve sets; in each esl, replace *es refs with
	//  a copy of their esl.  Since an es entry can
	//  refer only to those before it, we prevent
	//  circularity and can do one-pass resolution.
	if (escount > ESetBase)
		rescan = MergeElemLists(parsets, espars);

	DCilist *sparents = new DCilist;
	if (body)
		sparents->add(1, 2);  // body under topic

	// Resolve elem parents, building single and multi
	//  parent lists.  The multis can set pointers to
	//  the resolved es lists, no need to copy them.

	if (topcount) {
		for (id = 3; id <= topcount; id++) {
			chp = toppars->find(id);
			if (*chp == '*')  { // eset ref
				if ((inum = esnames->getitem(chp)) == 0)
					undefnames->add((long) chp);  // name missing, err
				else
					sparents->append(inum, id);
			}
			else if (stricmp(chp, "any")) { // not zero, add
				inum = topnames->getitem(chp);
				sparents->append(inum, id);
			}
		}
	}

	// read other lists: [ElementTypes], [TopicLevels], [TopicFirst]
	DCilist *elinline = new DCilist;
	if (sf->Section(DSpecSects->find(sectnum = 3))) {
		while (sf->NextItem()) {
			chp = sf->StrLeft();
			if ((id = topnames->getitem(chp)) == 0)
				undefnames->add((long) chp);  // name missing, err
			else if (elinline->find(id))
				dupnames->add(chp, sectnum);
			else {
				DeleteName(chp);
				if ((inum = sf->IntRight()) != 0)  // old style, number
					elinline->append(inum, id);
				else if (sf->ListRight()) {  // new style, Block, Inline, Text, Preform
					while ((chp = sf->ListItem()) != NULL) {
						if (!_stricmp(chp, "text"))
							inum |= 1;
						else if (!_stricmp(chp, "inline"))
							inum |= 2;
						else if (!_stricmp(chp, "preform"))
							inum |= 4;
						DeleteName(chp);
					}
					if (inum > 0)
						elinline->append(inum, id);
				}
			}
		}
	}

	DCilist *toplevel = new DCilist;
	if (sf->Section(DSpecSects->find(sectnum = 4))) {
		while (sf->NextItem()) {
			chp = sf->StrLeft();
			if ((id = topnames->getitem(chp)) == 0)
				undefnames->add((long) chp);  // name missing, err
			else if (toplevel->find(id))
				dupnames->add(chp, sectnum);
			else if ((inum = atoi(sf->StrRight())) != 0)
				toplevel->append(inum, id);
		}
	}

	DCilist *topfirst = new DCilist;
	DClist *fidl = NULL;

	if (sf->Section(DSpecSects->find(sectnum = 5))) {
		while (sf->NextItem()) {
			chp = sf->StrLeft();
			if ((id = topnames->getitem(chp)) == 0)
				undefnames->add((long) chp);  // name missing, err
			else if (topfirst->find(id))
				dupnames->add(chp, sectnum);
			else {
				chp = sf->StrRight();
				if (chp && (*chp == '*'))  { // eset ref
					if ((inum = esnames->getitem(chp)) == 0)
						undefnames->add((long) chp);  // name missing, err
					else
						topfirst->append(inum, id);
				}
				else {
					if (!chp || !stricmp(chp, "any"))
						topfirst->add( -1, id);
					else if ((inum = topnames->getitem(chp)) == 0)
						undefnames->add((long) chp);  // name missing, err
					else
						topfirst->append(inum, id);
				}
			}
		}
	}

	// read table definitions in [TopicTables]
	DCnlist *tbnames = new DCnlist;
	DCvlist *tblists = new DCvlist;
	DCnlist *tsnames = new DCnlist;
	tblprop *tbstruct = NULL;
	long tblid = 0;
	long tbcount = 0;

	if (sf->Section(DSpecSects->find(sectnum = 6))) {
		while (sf->NextItem()) {
			chp = sf->StrLeft();
			if (!tbnames->operator()(chp))
				tbnames->add(chp, ++tblid);
			else
				dupnames->add(chp, sectnum);
			chp = sf->StrRight();
			tsnames->add(chp, tblid);
		}
		tbcount = tblid;
	}

	for (tblid = 1; tblid <= tbcount; tblid++) {
		if (sf->Section(chp = tsnames->find(tblid))) {
			tbstruct = new tblprop;
			tblists->add(tbstruct, tblid);
			GetDITATableProps(sf, chp, tbstruct);
		}
	}


	// assemble struct of topic-type info and add to types list
	tagset *topictags = new tagset;
	long tagid = 0;

	if (repname
	 && ((tagid = DTopicTypes->getitem(repname)) != 0)) {
		topictags->setname = repname;
		DTagSets->replace(topictags, tagid);
		DTopicRoots->replace(topnames->find(1), tagid);
	}
	else {
		tagid = DTopicTypes->count() + 1;
		DTopicTypes->add(sname, tagid);
		DTagSets->add(topictags, tagid);
		DTopicRoots->add(topnames->find(1), tagid);
	}

	topictags->setname = DTopicTypes->find(tagid);
	topictags->start = start ? start : "title";
	topictags->dtype = pdtype ? pdtype : (DITA ? DTtag::PrologDTypeDita : DBtag::PrologDTypeDocBook);
	topictags->dtd = pdtd ? pdtd : (DITA ? DTtag::PrologDTDDita : DBtag::PrologDTDDocBook);

	// note that all lists must be initialized
	// they may be empty, but not NULL
	topictags->names = topnames;
	topictags->esetnames = esnames;
	topictags->esetlists = espars;  // was [mult]parsets 
	topictags->levels = toplevel;
	topictags->single = sparents;
	topictags->parents = parsets;   // was mparents
	topictags->inlin = elinline;
	topictags->atfirst = topfirst;
	topictags->tblnames = tbnames;
	topictags->tbllists = tblists;

	// report on errors in undefnames and unseqnames

	if (dumpfile) {
		CppForm = false;
		DumpTagSet(dumpfile, sname, derivtop, undefnames, unseqnames, rescan, dupnames);
	}
#ifdef _DEBUG
	if (cppdumpfile) {
		CppForm = true;
		DumpTagSet(cppdumpfile, sname, derivtop, undefnames, unseqnames, rescan, dupnames);
	}
#endif
	if (!dumpfile && !cppdumpfile)
		ReportDITALoadErrors(NULL, sname, topictags, undefnames, unseqnames, rescan, dupnames);
}


void HXMwr::DITAModSpecial(DCirfile *sf, char *deriv, char *sname, char *repname)
{
	// check the .ini for each section; if present,
	//  copy the lists from deriv and modify them.
	//  if missing, use the lists from deriv as is.

	long tagid = DTopicTypes->getitem(deriv);
	tagset *ts = new tagset(*((tagset *) DTagSets->find(tagid)));
	// shallow copy, so ptrs are correct for reused lists
	ts->setname = sname;

	// replace names list if names changed
	DCnlist *topnames = new DCnlist;
	DCnlist *dupnames = new DCnlist;
	long sectnum = 1;
	long id = 2;
	long nid = 0;

	if (sf->Section(DSpecSects->find(sectnum))
	 ||(sf->Section("Topic")
	  && (sf->Find("TopicType")
	   || sf->Find("TopicBody")))) {
		while ((ts->names = ts->names->next) != NULL) {
			topnames->add(ts->names->name, nid = ts->names->id);
			if (nid > id)  // numbers may be skipped if deleted
				id = nid;
		}
		ts->names = topnames;
	}

	char *dumpfile = NULL;
	char *cppdumpfile = NULL;
	char *body = NULL;

	if (sf->Section("Topic")) {
		if (sf->Find("TopicRoot"))
			topnames->replace(sf->StrRight(), 1);
		if (sf->Find("TopicStart"))
			ts->start = sf->StrRight();
		if (sf->Find("TopicBody")) {
			if ((body = sf->StrRight()) != NULL)
				topnames->replace(body, 2);
		}
		if (sf->Find("PrologDType"))
			ts->dtype = sf->StrRight();
		if (sf->Find("PrologDTD"))
			ts->dtd = sf->StrRight();
		if (sf->Find("DumpToFile"))
			dumpfile = sf->StrRight();
#ifdef _DEBUG
		if (sf->Find("DumpToCpp"))
			cppdumpfile = sf->StrRight();
#endif
	}

	DCnlist *toppars = new DCnlist;
	DClist *remnames = new DClist;
	char *chp = NULL;
	long lid = id;  // higher name IDs are new
	bool tpchanged = false;

	if (sf->Section(DSpecSects->find(sectnum))) {
		tpchanged = true;
		while (sf->NextItem()) {
			chp = sf->StrLeft();
			if ((nid = topnames->getitem(chp)) != 0) {  // name is in list
				if (nid > lid)  // new duplicate
					dupnames->add(chp, sectnum);
				else if (!stricmp(chp = sf->StrRight(), "no")) { // remove from list
					topnames->remove(nid);
					remnames->add(nid);
				}
				else  // store new parent or set
					toppars->add(chp, nid);
			}
			else {  // new name, add to list
				topnames->add(chp, ++id);
				toppars->add(sf->StrRight(), id);
			}
		}
	}

	// Make list of unresolved names for error message.
	DClist *undefnames = new DClist;
	DClist *unseqnames = new DClist;
	DCvlist *rescan = new DCvlist;

	// Process modified set lists
	bool eschanged = false;
	DCnlist *esnames = new DCnlist;
	long sid = 0;
	long lsid = 0;

	DCvlist *espars = new DCvlist;
	DClist *esl = NULL;
	long esval = 0;

	DCvlist *parsets = new DCvlist;

	if (sf->Section(DSpecSects->find(sectnum = 2))) {
		eschanged = true;
		// replace set lists if altered
		if (ts->esetnames) {
			while ((ts->esetnames = ts->esetnames->next) != NULL) {
				esnames->add(ts->esetnames->name, nid = ts->esetnames->id);
				if (nid > sid)  // numbers may be skipped if deleted
					sid = nid;
			}
		}
		if (sid == 0)  // empty list
			sid = ESetBase;  // set flag
		lsid = sid;  // last id used before
		ts->esetnames = esnames;

		// make new setlist with existing setlists in it
		if (ts->esetlists) {
			while ((ts->esetlists = ts->esetlists->next) != NULL)
				espars->add(ts->esetlists->vp, ts->esetlists->id);
		}
		ts->esetlists = espars;

		// always need new resolved sets list, ignore old one
		ts->parents = parsets;

		// gemerate resolved parsets
		DClist *mpl = NULL;
		DClist *epl = NULL;
		long inum = 0;
		while (sf->NextItem()) {  // may be either modified or new
			chp = sf->StrLeft();
			if ((nid = esnames->getitem(chp)) == 0) { // new
				esnames->add(chp, ++sid);
				espars->add(epl = new DClist, sid);
			}
			else if (nid > lsid) // new dup
				dupnames->add(chp, sectnum);
			else  // modified, replace list
				espars->replace(epl = new DClist, nid);

			if (sf->ListRight()) {
				while ((chp = sf->ListItem()) != NULL) {
					if (*chp == '*')  { // eset ref
						if ((inum = esnames->getitem(chp)) == 0)
							undefnames->add((long) chp);  // name missing, err
						else if (inum > sid)
							unseqnames->add((long) chp);  // not resolved yet, err
						else  // add to espars list
							epl->add(inum);
					}
					else {  // element ref
						if ((inum = topnames->getitem(chp)) == 0)
							undefnames->add((long) chp);  // name missing, err
						else   // add parent to list
							epl->add(inum);
					}
				}
			}
		}
		// list items processed, espars set, gen new resolved parsets list
		// old parsets list doesn't help, ignore it and rebuild from espars
		if (sid > ESetBase)
			rescan = MergeElemLists(parsets, espars);
	}
	else  // no change to element sets
		parsets = ts->parents;

	DCilist *sparents = new DCilist;
	DCvlist *mppl = NULL;
	long inum = 0;

	// Resolve elem parents, building sparent list
	if (tpchanged || eschanged) {  // parentage may have changed
		if (ts->single) {
			while ((ts->single = ts->single->next) != NULL)
				sparents->add(ts->single->val, ts->single->id);
		}
		ts->single = sparents;
		while ((toppars = toppars->next) != NULL) {
			id = toppars->id;
			chp = (char *) toppars->name;
			if (*chp == '*')  { // eset ref
				if ((inum = esnames->getitem(chp)) == 0)
					undefnames->add((long) chp);  // name missing, err
				else {
					sparents->replace(inum, id);
					//mparents->add(multparsets->find(inum), id);
				}
			}
			else if (!stricmp(chp, "any"))  // zero, remove
				sparents->remove(id);
			else if ((inum = topnames->getitem(chp)) == 0)
				undefnames->add((long) chp);  // name missing, err
			else
				sparents->replace(inum, id);
		}
		if (remnames->count() > 0) {  // process removed names
			while ((inum = remnames->dequeue()) != 0) {
				// remove orphans (children of removed parents)
				while ((id = sparents->operator()(inum)) != 0) {
					sparents->remove(id);
					topnames->remove(id);
					remnames->add(id);  // add removed child to queue
				}
				// scan espars lists
				for (mppl = espars->next; mppl; mppl = mppl->next)
					((DClist *) (mppl->vp))->remove(inum);
				// scan parsets lists too
				for (mppl = parsets->next; mppl; mppl = mppl->next)
					((DClist *) (mppl->vp))->remove(inum);
			}
		}
	}

	// read other lists: [ElementTypes], [TopicLevels], [TopicFirst]
	DCilist *elinline = new DCilist;
	DClist *newinlin = new DClist;
	if (sf->Section(DSpecSects->find(sectnum = 3))) {
		if (ts->inlin) {
			while ((ts->inlin = ts->inlin->next) != NULL)
				if (topnames->find(id = ts->inlin->id))
					elinline->append(ts->inlin->val, id);
		}
		ts->inlin = elinline;
		while (sf->NextItem()) {
			chp = sf->StrLeft();
			if ((id = topnames->getitem(chp)) == 0)
				undefnames->add((long) chp);  // name missing, err
			else if (newinlin->find(id))
				dupnames->add(chp, sectnum);
			else {
				DeleteName(chp);
				newinlin->add(id);
				if (!stricmp((chp = sf->StrRight()), "no"))
					elinline->remove(id);
				else if ((inum = atoi(chp)) != 0)  // old style, number
					elinline->replace(inum, id);
				else if (sf->ListRight()) {  // new style, Block, Inline, Text, Preform
					while ((chp = sf->ListItem()) != NULL) {
						if (!_stricmp(chp, "text"))
							inum |= 1;
						else if (!_stricmp(chp, "inline"))
							inum |= 2;
						else if (!_stricmp(chp, "preform"))
							inum |= 4;
						DeleteName(chp);
					}
					if (inum > 0)
						elinline->replace(inum, id);
				}
			}
		}
	}

	DCilist *toplevel = new DCilist;
	DClist *newlevel = new DClist;
	if (sf->Section(DSpecSects->find(sectnum = 4))) {
		if (ts->levels) {
			while ((ts->levels = ts->levels->next) != NULL)
				if (topnames->find(id = ts->levels->id))
					toplevel->append(ts->levels->val, id);
		}
		ts->levels = toplevel;
		while (sf->NextItem()) {
			chp = sf->StrLeft();
			if ((id = topnames->getitem(chp)) == 0)
				undefnames->add((long) chp);  // name missing, err
			else if (newlevel->find(id))
				dupnames->add(chp, sectnum);
			else {
				newlevel->add(id);
				if (!stricmp((chp = sf->StrRight()), "no"))
					toplevel->remove(id);
				else if ((inum = atoi(chp)) != 0)
					toplevel->replace(inum, id);
			}
		}
	}

	DCilist *topfirst = new DCilist;
	DClist *newfirst = new DClist;
	DClist *fidl = NULL;

	if (sf->Section(DSpecSects->find(sectnum = 5))) {
		if (ts->atfirst) {
			while ((ts->atfirst = ts->atfirst->next) != NULL)
				if (topnames->find(id = ts->atfirst->id))
					topfirst->append(ts->atfirst->val, id);
		}
		ts->atfirst = topfirst;
		while (sf->NextItem()) {
			chp = sf->StrLeft();
			if ((id = topnames->getitem(chp)) == 0)
				undefnames->add((long) chp);  // name missing, err
			else if (newfirst->find(id))
				dupnames->add(chp, sectnum);
			else {
				newfirst->add(id);
				chp = sf->StrRight();
				if (*chp == '*') {   // eset ref
					if ((inum = esnames->getitem(chp)) == 0)
						undefnames->add((long) chp);  // name missing, err
					else
						topfirst->append(inum, id);
				}
				else {
					if (!stricmp(chp, "any"))
						topfirst->add( -1, id); 
					else if (!stricmp(chp, "no")) {
						while (topfirst->find(id))
							topfirst->remove(id);  // may be expanded set to remove
					}
					else if ((inum = topnames->getitem(chp)) == 0)
						undefnames->add((long) chp);  // name missing, err
					else
						topfirst->append(inum, id);
				}
			}
		}
	}

	// read table definitions in [TopicTables]
	DCnlist *tbnames = new DCnlist;
	DClist *newtbnames = new DClist;
	DCvlist *tblists = new DCvlist;
	DCnlist *tsnames = new DCnlist;
	tblprop *tbstruct = NULL;
	long tblid = 0;
	long tblast = 0;
	long tbcount = 0;
	char *tsect = NULL;

	if (sf->Section(DSpecSects->find(sectnum = 6))) {
		// copy tblnames and tbllists, advancing tblid
		if (ts->tblnames) {
			while ((ts->tblnames = ts->tblnames->next) != NULL) {
				tbnames->add(ts->tblnames->name, tblid = ts->tblnames->id);
				if (tblid > tblast)  // numbers may be skipped if deleted
					tblast = tblid;
			}
		}
		ts->tblnames = tbnames;

		if (ts->tbllists) {
			while ((ts->tbllists = ts->tbllists->next) != NULL)
				tblists->add(ts->tbllists->vp, ts->tbllists->id);
		}
		ts->tbllists = tblists;

		while (sf->NextItem()) {
			chp = sf->StrLeft();
			tsect = sf->StrRight();
			if ((id = tbnames->getitem(chp)) == 0) {  // new table
				tbnames->add(chp, ++tblid);
				tsnames->add(tsect, tblid);
			}
			else if (newtbnames->find(id))  // dup of new name
				dupnames->add(chp, sectnum);
			else {
				newtbnames->add(id);
				tsnames->replace(tsect, id);
			}
		}
		tbcount = tblid;
	}

	while ((tsnames = tsnames->next) != NULL) {
		tsect = (char *) tsnames->name;
		tblid = tsnames->id;
		if (sf->Section(tsect)) {
			tbstruct = new tblprop;
			tblists->replace(tbstruct, tblid);
			GetDITATableProps(sf, tbnames->find(tblid), tbstruct);
		}
	}


	// add topic-type info to types lists
	if (repname
	 && ((tagid = DTopicTypes->getitem(repname)) != 0)) {
		ts->setname = repname;
		DTagSets->replace(ts, tagid);
		DTopicRoots->replace(topnames->find(1), tagid);
	}
	else {
		tagid = DTopicTypes->count() + 1;
		DTopicTypes->add(sname, tagid);
		DTagSets->add(ts, tagid);
		DTopicRoots->add(topnames->find(1), tagid);
	}

	// report on errors
	if (dumpfile) {
		CppForm = false;
		DumpTagSet(dumpfile, sname, deriv, undefnames, unseqnames, rescan, dupnames);
	}
#ifdef _DEBUG
	if (cppdumpfile) {
		CppForm = true;
		DumpTagSet(cppdumpfile, sname, deriv, undefnames, unseqnames, rescan, dupnames);
	}
#endif
	if (!dumpfile && !cppdumpfile)
		ReportDITALoadErrors(NULL, sname, ts, undefnames, unseqnames, rescan, dupnames);
}


void HXMwr::ReportDITALoadErrors(DCwfile *wf, char *sname, tagset *ts,
	                      DClist *undef, DClist *unseq, DCvlist *rescan, DCnlist *dupnames)
{
	bool embedded = true;

	if (!dupnames->next
	 && !undef->next
	 && !unseq->next
	 && !(rescan
	   && (rescan->next != NULL)))
		return;  // no errors to report

	if (!wf) {
		wf = new DCwfile(DITA ? "DITAErrors.txt" : "XMLErrors.txt", ftxt);
		wf->putStr(DITA ? "\nDITA" : "\nXML");
		wf->putStr(" error file for ");
		wf->putStr(sname);
		wf->putCr();
		embedded = false;
	}

	// report duplicate names
	if (dupnames->next) {
		wf->putStr("\nNames duplicated in same section:\n");
		while ((dupnames = dupnames->next) != NULL) {
			wf->putStr(dupnames->name);
			wf->putStr(" in section [");
			wf->putStr(DSpecSects->find(dupnames->id));
			wf->putStr("]\n");
		}
	}

	// dump lists of undef and unseq names
	if (undef->next) {
		wf->putStr("\nNames not defined:\n");
		while ((undef = undef->next) != NULL) {
			wf->putStr((char *) undef->id);
			wf->putCr();
		}
	}
	if (unseq->next) {
		wf->putStr("\nNames referenced before definition:\n");
		while ((unseq = unseq->next) != NULL) {
			wf->putStr((char *) unseq->id);
			wf->putCr();
		}
	}
	if (rescan && (rescan->next != NULL)) {
		wf->putStr("\nSets in circular references:");
		while ((rescan = rescan->next) != NULL) {
			// Set (id) is referenced in set (vp)
			wf->putStr("\nSet ");
			wf->putStr(ts->esetnames->find(rescan->id));
			wf->putStr(" is referenced in set ");
			wf->putStr(ts->esetnames->find(ts->parents->operator()(rescan->vp)));
		}
		wf->putCr();
	}

	if (!embedded)
		wf->CloseFile();
}



// table properties

DCnlist *HXMwr::DITATableNames = NULL; // relates our name to tblprop *
DCvlist *HXMwr::DITATableLists = NULL; // relates our name to tblprop *

tblprop HXMwr::DITASimpleTable = {
 "simpletable", false, NULL, NULL,
 NULL, NULL, NULL, NULL, 0, 1, 1,
 NULL, "sthead", NULL, NULL,
 NULL, "strow", "stentry", NULL,
 NULL, "strow", "stentry", NULL,  // footer rows are body
 NULL, true, NULL, NULL,
 NULL, NULL, NULL, false
};

npair HXMwr::DITAPropTblHead[] = {
 { 1, "proptypehd" },
 { 2, "propvaluehd" },
 { 3, "propdeschd" },
 { 0, NULL }
 };

npair HXMwr::DITAPropTblBody[] = {
 { 1, "proptype" },
 { 2, "propvalue" },
 { 3, "propdesc" },
 { 0, NULL }
 };

tblprop HXMwr::DITAPropertiesTable = {
 "properties", false, NULL, NULL,
 NULL, NULL, NULL, NULL, 3, 1, 1,
 NULL, "prophead", NULL, NULL,
 NULL, "property", NULL, NULL,
 NULL, "property", NULL, NULL,
 NULL, true, NULL, NULL,
 NULL, NULL, NULL, false
};

npair HXMwr::DITAChoiceTblHead[] = {
 { 1, "choptionhd" },
 { 2, "chdeschd" },
 { 0, NULL }
 };

npair HXMwr::DITAChoiceTblBody[] = {
 { 1, "choption" },
 { 2, "chdesc" },
 { 0, NULL }
 };

tblprop HXMwr::DITAChoiceTable = {
 "choicetable", false, NULL, NULL,
 NULL, NULL, NULL, NULL, 2, 1, 1,
 NULL, "chhead", NULL, NULL,
 NULL, "chrow", NULL, NULL,
 NULL, "chrow", NULL, NULL,
 NULL, true, NULL, NULL,
 NULL, NULL, NULL, false
};

tblprop HXMwr::DITAComplexTable = {
 "table", true, "desc", "tgroup",
 "colspec", "colname", "colnum", "colwidth", 0, 0, 0,
 "thead", "row", "entry", NULL,
 "tbody", "row", "entry", NULL,
 "tbody", "row", "entry", NULL,
 "morerows", true, NULL, "namest",
 "nameend", "colname", "col", true
};

void HXMwr::InitDITATableTypes(void)
{
	DCnlist *nl = NULL;

	nl = new DCnlist();
	nl->init(DITAPropTblHead);
	DITAPropertiesTable.hclist = nl;
	nl = new DCnlist();
	nl->init(DITAPropTblBody);
	DITAPropertiesTable.bclist = nl;

	nl = new DCnlist();
	nl->init(DITAChoiceTblHead);
	DITAChoiceTable.hclist = nl;
	nl = new DCnlist();
	nl->init(DITAChoiceTblBody);
	DITAChoiceTable.bclist = nl;
}


// loads info in table section into tblprop struct
void HXMwr::GetDITATableProps(DCirfile *sf, char *tname, tblprop *tst)
{
	long i = 0;
	char *chp = NULL;

	if (sf->Find("TableType"))
		tst->ttyp = sf->StrRight();
	else
		tst->ttyp = tname;
	if (sf->Find("TableTitle"))
		tst->ttitle = sf->BoolRight();
	else
		tst->ttitle = false;
	if (sf->Find("TableDesc"))
		tst->tdesc = sf->StrRight();
	else
		tst->tdesc = NULL;
	if (sf->Find("TableGroup"))
		tst->tgroup = sf->StrRight();
	else
		tst->tgroup = NULL;
	if (sf->Find("ColSpec"))
		tst->tcspec = sf->StrRight();
	else
		tst->tcspec = NULL;
	if (sf->Find("ColNum"))
		tst->csiatt = sf->StrRight();
	else
		tst->csiatt = NULL;
	if (sf->Find("ColSpecName"))
		tst->csnatt = sf->StrRight();
	else
		tst->csnatt = NULL;
	if (sf->Find("ColWidth"))
		tst->cswatt = sf->StrRight();
	else
		tst->cswatt = NULL;

	// long ccmax, hrmax
	if (sf->Find("ColCountMax"))
		tst->ccmax = sf->IntRight();
	else
		tst->ccmax = 0;
	if (sf->Find("HeadRowMax"))
		tst->hrmax = sf->IntRight();
	else
		tst->hrmax = 0;
	if (sf->Find("FootRowMax"))
		tst->frmax = sf->IntRight();
	else
		tst->frmax = 0;

	if (sf->Find("HeadGroup"))
		tst->hgroup = sf->StrRight();
	else
		tst->hgroup = NULL;
	if (sf->Find("HeadRow"))
		tst->hrow = sf->StrRight();
	else
		tst->hrow = NULL;
	if (sf->Find("HeadCell"))
		tst->hcell = sf->StrRight();
	else
		tst->hcell = NULL;

	if (sf->Find("FootGroup"))
		tst->fgroup = sf->StrRight();
	else
		tst->fgroup = NULL;
	if (sf->Find("FootRow"))
		tst->frow = sf->StrRight();
	else
		tst->frow = NULL;
	if (sf->Find("FootCell"))
		tst->fcell = sf->StrRight();
	else
		tst->fcell = NULL;

	// DCnlist *hclist
	if (tst->ccmax) {
		tst->hclist = new DCnlist;
		for (i = 1; i <= tst->ccmax; i++) {
			chp = NewName("HeadCell", NumStrMax + 9);
			_ltoa(i, chp + 8, 10);
			if (sf->Find(chp))
				tst->hclist->add(sf->StrRight(), i);
		}
	}
	else
		tst->hclist = NULL;

	// DCnlist *fclist
	if (tst->ccmax) {
		tst->fclist = new DCnlist;
		for (i = 1; i <= tst->ccmax; i++) {
			chp = NewName("FootCell", NumStrMax + 9);
			_ltoa(i, chp + 8, 10);
			if (sf->Find(chp))
				tst->fclist->add(sf->StrRight(), i);
		}
	}
	else
		tst->fclist = NULL;

	if (sf->Find("BodyGroup"))
		tst->bgroup = sf->StrRight();
	else
		tst->bgroup = NULL;
	if (sf->Find("Row"))
		tst->brow = sf->StrRight();
	else
		tst->brow = "row";
	if (sf->Find("Cell"))
		tst->bcell = sf->StrRight();
	else
		tst->bcell = "entry";

	// DCnlist *bclist
	if (tst->ccmax) {
		tst->bcell = NULL;
		tst->bclist = new DCnlist;
		for (i = 1; i <= tst->ccmax; i++) {
			chp = NewName("Cell", NumStrMax + 5);
			_ltoa(i, chp + 4, 10);
			if (sf->Find(chp))
				tst->bclist->add(sf->StrRight(), i);
		}
	}
	else
		tst->bclist = NULL;

	if (sf->Find("RowSpan"))
		tst->rsatt = sf->StrRight();
	else
		tst->rsatt = NULL;

	if (sf->Find("ColSpanNames"))
		tst->csnmuse = sf->BoolRight();
	else
		tst->csnmuse = false;

	if (sf->Find("ColSpanCount"))
		tst->cscatt = sf->StrRight();
	else
		tst->cscatt = NULL;
	if (sf->Find("ColSpanStart"))
		tst->csnsatt = sf->StrRight();
	else
		tst->csnsatt = NULL;
	if (sf->Find("ColSpanEnd"))
		tst->csneatt = sf->StrRight();
	else
		tst->csneatt = NULL;

	if (sf->Find("ColName"))
		tst->cnmatt = sf->StrRight();
	else
		tst->cnmatt = NULL;
	if (sf->Find("ColNamePrefix"))
		tst->csnpx = sf->StrRight();
	else
		tst->csnpx = NULL;

	if (sf->Find("CellAlign"))
		tst->calgnatt = sf->BoolRight();
	else
		tst->calgnatt = false;
}


void HXMwr::DumpDITATables(DCwfile *wf, tagset *ts)
{
	long tid = 0;
	DCnlist *tbsects = new DCnlist;
	DCnlist *tbnames = ts->tblnames;
	DCvlist *tblists = ts->tbllists;
	long tc = tbnames->count();
	char *chp = NULL;
	char *tbl = NULL;


	if (!tc)  // no tables here
		return;

	wf->putStr("\n[TopicTables]");

	for (tid = 1; tid <= tc; tid++) {
		wf->putCr();
		wf->putStr(chp = tbnames->find(tid));
		wf->putChar('=');
		tbl = NewName(chp, strlen(chp) + 6);
		*tbl = _toupper(*tbl);
		strcat(tbl, "Table");
		tbsects->add(tbl, tid);
		wf->putStr(tbl);
	}
	wf->putCr();
	for (tid = 1; tid <= tc; tid++)
		DumpDITATableStruct(wf, tbsects->find(tid), (tblprop *) tblists->find(tid));
}

void HXMwr::DumpDITATableStruct(DCwfile *wf, char *secname, tblprop* tp)
{
	long i = 0;

	wf->putStr("\n[");
	wf->putStr(secname);
	wf->putChar(']');
	if (tp->ttyp) {
		wf->putStr("\nTableType=");
		wf->putStr(tp->ttyp);
	}
	if (tp->ttitle)
		wf->putStr("\nTableTitle=Yes");
	if (tp->tdesc) {
		wf->putStr("\nTableDesc=");
		wf->putStr(tp->tdesc);
	}
	if (tp->tgroup) {
		wf->putStr("\nTableGroup=");
		wf->putStr(tp->tgroup);
	}
	if (tp->tcspec) {
		wf->putStr("\nColSpec=");
		wf->putStr(tp->tcspec);
	}
	if (tp->csnatt) {
		wf->putStr("\nColNum=");
		wf->putStr(tp->csiatt);
	}
	if (tp->csiatt) {
		wf->putStr("\nColSpecName=");
		wf->putStr(tp->csnatt);
	}
	if (tp->cswatt) {
		wf->putStr("\nColWidth=");
		wf->putStr(tp->cswatt);
	}
	if (tp->ccmax) {
		wf->putStr("\nColCountMax=");
		wf->putNum(tp->ccmax);
	}
	if (tp->hrmax) {
		wf->putStr("\nHeadRowMax=");
		wf->putNum(tp->hrmax);
	}
	if (tp->hgroup) {
		wf->putStr("\nHeadGroup=");
		wf->putStr(tp->hgroup);
	}
	if (tp->hrow) {
		wf->putStr("\nHeadRow=");
		wf->putStr(tp->hrow);
	}
	if (tp->hcell) {
		wf->putStr("\nHeadCell=");
		wf->putStr(tp->hcell);
	}
	if (tp->hclist) {
		for (i = 1; i <= tp->ccmax; i++) {
			wf->putStr("\nHeadCell");
			wf->putNum(i);
			wf->putChar('=');
			wf->putStr(tp->hclist->find(i));
		}
	}
	if (tp->frmax) {
		wf->putStr("\nFootRowMax=");
		wf->putNum(tp->frmax);
	}
	if (tp->fgroup) {
		wf->putStr("\nFootGroup=");
		wf->putStr(tp->fgroup);
	}
	if (tp->frow) {
		wf->putStr("\nFootRow=");
		wf->putStr(tp->frow);
	}
	if (tp->fcell) {
		wf->putStr("\nFootCell=");
		wf->putStr(tp->fcell);
	}
	if (tp->fclist) {
		for (i = 1; i <= tp->ccmax; i++) {
			wf->putStr("\nFootCell");
			wf->putNum(i);
			wf->putChar('=');
			wf->putStr(tp->fclist->find(i));
		}
	}
	if (tp->bgroup) {
		wf->putStr("\nBodyGroup=");
		wf->putStr(tp->bgroup);
	}
	if (tp->brow) {
		wf->putStr("\nRow=");
		wf->putStr(tp->brow);
	}
	if (tp->bcell) {
		wf->putStr("\nCell=");
		wf->putStr(tp->bcell);
	}
	if (tp->bclist) {
		for (i = 1; i <= tp->ccmax; i++) {
			wf->putStr("\nCell");
			wf->putNum(i);
			wf->putChar('=');
			wf->putStr(tp->bclist->find(i));
		}
	}
	if (tp->rsatt) {
		wf->putStr("\nRowSpan=");
		wf->putStr(tp->rsatt);
	}

	if (tp->csnmuse
	 && tp->csnsatt
	 && tp->csneatt)
		wf->putStr("\nColSpanNames=Yes");
	else if (!tp->csnmuse
	 && tp->cscatt)
		wf->putStr("\nColSpanNames=No");

	if (tp->cscatt) {
		wf->putStr("\nColSpanCount=");
		wf->putStr(tp->cscatt);
	}
	if (tp->csnsatt) {
		wf->putStr("\nColSpanStart=");
		wf->putStr(tp->csnsatt);
	}
	if (tp->csneatt) {
		wf->putStr("\nColSpanEnd=");
		wf->putStr(tp->csneatt);
	}
	if (tp->cnmatt) {
		wf->putStr("\nColName=");
		wf->putStr(tp->cnmatt);
	}
	if (tp->csnpx) {
		wf->putStr("\nColNamePrefix=");
		wf->putStr(tp->csnpx);
	}
	if (tp->calgnatt)
		wf->putStr("\nCellAlign=Yes");
	wf->putCr();
}

#ifdef _DEBUG
void HXMwr::DumpDITATablesCpp(DCwfile *wf, tagset *ts, char *Base, long typ)
{
	long tid = 0;
	DCnlist *tbsects = new DCnlist;
	DCnlist *tbnames = ts->tblnames;
	DCvlist *tblists = ts->tbllists;
	long tc = tbnames->count();
	char *chp = NULL;
	char *tbl = NULL;

	if (!tc)  // no tables here
		return;

	for (tid = 1; tid <= tc; tid++) {
		chp = tbnames->find(tid);
		tbl = NewName(chp, strlen(chp) + 6);
		*tbl = _toupper(*tbl);
		strcat(tbl, "Table");
		tbsects->add(tbl, tid);
	}

	if (typ == 1) {  // c++ data for dwhtmdd.cpp
		for (tid = 1; tid <= tc; tid++)
			DumpDITATableStructCpp(wf, tbsects->find(tid), (tblprop *) tblists->find(tid), 1);
	}
	else if (typ == 2) { // write c++ init code for dwhtmds.cpp
		wf->putStr("\n\tD");
		wf->putStr(Base);
		wf->putStr("Tags->tblnames = new DCnlist;");

		for (tid = 1; tid <= tc; tid++) {
			wf->putStr("\n\tD");
			wf->putStr(Base);
			wf->putStr("Tags->tblnames->add(\"");
			wf->putStr(tbnames->find(tid));
			wf->putStr("\", ");
			wf->putNum(tid);
			wf->putStr(");");
		}
		wf->putCr();

		wf->putStr("\n\tD");
		wf->putStr(Base);
		wf->putStr("Tags->tbllists = new DCvlist;");

		for (tid = 1; tid <= tc; tid++) {
			wf->putStr("\n\tD");
			wf->putStr(Base);
			wf->putStr("Tags->tbllists->add(&DITA");
			wf->putStr(tbsects->find(tid));
			wf->putStr(", ");
			wf->putNum(tid);
			wf->putStr(");");
		}
		wf->putCr();
	}
	else if (typ == 3) {     // write .h declarations for dwhtm.h
		for (tid = 1; tid <= tc; tid++)
			DumpDITATableStructCpp(wf, tbsects->find(tid), (tblprop *) tblists->find(tid), typ);
	}
}

void HXMwr::DumpDITATableStructCpp(DCwfile *wf, char *secname, tblprop *tp, long typ)
{
	if (typ == 1) {       // c++ data struct for dwhtmdd.cpp ****

	}
	else if (typ == 3) {  // declaration for dwhtm.h ****

	}
}
#endif

// dumping tag set

bool HXMwr::CppForm = false;

DCnlist *HXMwr::DSPnames = new DCnlist;
DCnlist *HXMwr::DSPbasenames = new DCnlist;
DCilist *HXMwr::DSPsparents = new DCilist;
DCvlist *HXMwr::DSPmparents = new DCvlist;
DCilist *HXMwr::DSPbasespars = new DCilist;
DCvlist *HXMwr::DSPelemsets = new DCvlist;
DCnlist *HXMwr::DSPsetnames = new DCnlist;
DCilist *HXMwr::DSPinlins = new DCilist;
DCilist *HXMwr::DSPlevels = new DCilist;
DCilist *HXMwr::DSPatfirst = new DCilist;


void HXMwr::DumpTagSet(char *dumpfile, char *tname, char *deriv,
	DClist *undef, DClist *unseq, DCvlist *rescan, DCnlist *dupnames)
{
	char *Base = "Base";
	DCwfile *twf = new DCwfile(dumpfile, ftxt);

	twf->putStr(DITA ? "\nDITA" : "\nXML");
	twf->putStr(" dump file ");
	twf->putStr(dumpfile);

	// identify tagset, show DTD, etc.
	long tagid = DTopicTypes->getitem(tname);
	tagset *ts = (tagset *) DTagSets->find(tagid);
	twf->putStr("\nTagset ");
	twf->putStr(tname);
	if (!ts) {
		twf->putStr(" does not exist.\n");
		twf->CloseFile();
		return;
	}

	// make ts members accessible to list member functions
	DSPnames = ts->names;
	DSPsparents = ts->single;
	DSPmparents = ts->parents;
	DSPelemsets = ts->esetlists;
	DSPsetnames = ts->esetnames;
	DSPinlins = ts->inlin;
	DSPlevels = ts->levels;
	DSPatfirst = ts->atfirst;

	ReportDITALoadErrors(twf, tname, ts, undef, unseq, rescan, dupnames);

#ifdef _DEBUG
	if (CppForm) {
		long topid = 0;
		long i, v;

		char ver = DITAVer + '0';
		char *mod = "DTtag::";

		bool newbase = false;		// edit these if adding new DITA or DocBook type
		bool newtable = false;

		Base = NewName(DSPnames->find(1));  // for DITA, generate from root name
		*Base = toupper(*Base);

		if (DocBook) {
			Base = (*(DSPnames->find(1)) == 'a') ? "DocBookA" : "DocBookB";
			mod = "DBtag::";
			ver = '\0';
		}

		twf->putStr("\nC++ format used.\n");
		twf->putStr("\n\nData sets for dwhtmd");
		twf->putChar(DocBook ? 'b' : ver);
		twf->putStr(".cpp:\n");

		twf->putStr("\nnpair ");
		twf->putStr(mod);
		twf->putStr(Base);
		twf->putStr("NamesSet");
		twf->putChar(ver);
		twf->putStr("[] = {\n");
		while ((DSPnames = DSPnames->next) != NULL) {
			// { 1, "topic"},
			twf->putStr(" { ");
			twf->putNum(i = DSPnames->id);
			twf->putStr(", \"");
			twf->putStr(DSPnames->name);
			twf->putStr("\"},\n");
			if (i > topid)
				topid = i;
		}
		twf->putStr(" { 0, NULL }\n");
		twf->putStr(" };\n");
		DSPnames = ts->names;

		twf->putStr("\nipair ");
		twf->putStr(mod);
		twf->putStr(Base);
		twf->putStr("InlineSet");
		twf->putChar(ver);
		twf->putStr("[] = {\n");
		for (i = 1; i <= topid; i++) {
			if ((v = DSPinlins->find(i)) == 0)
				continue;
			// { 2, 1 },
			twf->putStr(" { ");
			twf->putNum(i);
			twf->putStr(", ");
			twf->putNum(v);
			twf->putStr(" },\n");
		}
		twf->putStr(" { 0, 0 }\n");
		twf->putStr(" };\n");

		twf->putStr("\nipair ");
		twf->putStr(mod);
		twf->putStr(Base);
		twf->putStr("SingleSet");
		twf->putChar(ver);
		twf->putStr("[] = {\n");
		for (i = 1; i <= topid; i++) {
			if ((v = DSPsparents->find(i)) == 0)
				continue;
			// { 2, 1 },
			twf->putStr(" { ");
			twf->putNum(i);
			twf->putStr(", ");
			twf->putNum(v);
			twf->putStr(" },\n");
		}
		twf->putStr(" { 0, 0 }\n");
		twf->putStr(" };\n");

		while ((DSPsetnames = DSPsetnames->next) != NULL) {
			long ic = 0;
			// long D?tags::liPar[] = { 42, 43, 0 };
			twf->putStr("\nlong ");
			twf->putStr(mod);
			twf->putStr(FixSetName((char *) DSPsetnames->name + 1));
			twf->putStr(Base);
			twf->putStr("Par");
			twf->putChar(ver);
			twf->putStr("[] = { \n");
			DClist *slp = (DClist *) DSPelemsets->find(DSPsetnames->id);
			while ((slp = slp->next) != NULL) {
				twf->putNum(slp->id);
				twf->putStr(", ");
				if (++ic == 8) {
					twf->putCr();
					ic = 0;
				}
			}
			twf->putStr("0 };\n");
		}
		DSPsetnames = ts->esetnames;

		twf->putStr("\nvpair ");
		twf->putStr(mod);
		twf->putStr(Base);
		twf->putStr("ElemSetsSet");
		twf->putChar(ver);
		twf->putStr("[] = {\n");
		while ((DSPelemsets = DSPelemsets->next) != NULL) {
			// { 1, titlePar },
			twf->putStr(" { ");
			twf->putNum(DSPelemsets->id);
			twf->putStr(", ");
			twf->putStr(FixSetName(DSPsetnames->find(DSPelemsets->id) + 1));
			twf->putStr(Base);
			twf->putStr("Par");
			twf->putChar(ver);
			twf->putStr(" },\n");
		}
		twf->putStr(" { 0, NULL }\n");
		twf->putStr(" };\n");
		DSPelemsets = ts->esetlists;

		twf->putStr("\nnpair ");
		twf->putStr(mod);
		twf->putStr(Base);
		twf->putStr("ElemSetNamesSet");
		twf->putChar(ver);
		twf->putStr("[] = {\n");
		while ((DSPelemsets = DSPelemsets->next) != NULL) {
			twf->putStr(" { ");
			twf->putNum(DSPelemsets->id);
			twf->putStr(", \"");
			twf->putStr(DSPsetnames->find(DSPelemsets->id));
			twf->putStr("\"},\n");
		}
		twf->putStr(" { 0, NULL }\n");
		twf->putStr(" };\n");
		DSPelemsets = ts->esetlists;

		twf->putStr("\nipair ");
		twf->putStr(mod);
		twf->putStr(Base);
		twf->putStr("LevelsSet");
		twf->putChar(ver);
		twf->putStr("[] = {\n");
		for (i = 1; i <= topid; i++) {
			if ((v = DSPlevels->find(i)) == 0)
				continue;
			// { 2, 1 },
			twf->putStr(" { ");
			twf->putNum(i);
			twf->putStr(", ");
			twf->putNum(v);
			twf->putStr(" },\n");
		}
		twf->putStr(" { 0, 0 }\n");
		twf->putStr(" };\n");

		twf->putStr("\nipair ");
		twf->putStr(mod);
		twf->putStr(Base);
		twf->putStr("AtFirstSet");
		twf->putChar(ver);
		twf->putStr("[] = {\n");
		for (i = 1; i <= topid; i++) {
			if ((v = DSPatfirst->find(i)) == 0)
				continue;
			// { 3, 1 },
			twf->putStr(" { ");
			twf->putNum(i);
			twf->putStr(", ");
			twf->putNum(v);
			twf->putStr(" },\n");
		}
		twf->putStr(" { 0, 0 }\n");
		twf->putStr(" };\n");

		if (newtable) {
			twf->putStr("\n\nCode for table structs in dwhtmdd.cpp:\n");

			// add tblprop structs here ****
			DumpDITATablesCpp(twf, ts, Base, 1);
		}

		if (newbase) {
			twf->putStr("\n\nCode for init function in dwhtmds.cpp:\n");

			twf->putStr("void HXMwr::SetDITA");
			twf->putStr(Base);
			twf->putStr("Type(long tagid)\n{");

			twf->putStr("\n\t");
			twf->putStr(Base);
			twf->putStr("Names = new DCnlist;\n\t");
			twf->putStr(Base);
			twf->putStr("Names->init(");
			twf->putStr(Base);
			twf->putStr("NamesSet);\n");

			twf->putStr("\n\t");
			twf->putStr(Base);
			twf->putStr("ElemSetNames = new DCnlist;");
			twf->putStr("\n\t");
			twf->putStr(Base);
			twf->putStr("ElemSetNames->init(");
			twf->putStr(Base);
			twf->putStr("ElemSetNamesSet);\n");

			twf->putStr("\n\t");
			twf->putStr(Base);
			twf->putStr("ElemSets = new DCvlist;");
			twf->putStr("\n\t");
			twf->putStr(Base);
			twf->putStr("ElemSets->initdl(");
			twf->putStr(Base);
			twf->putStr("ElemSetsSet);\n");

			twf->putStr("\n\t");
			twf->putStr(Base);
			twf->putStr("Single = new DCilist;");
			twf->putStr("\n\t");
			twf->putStr(Base);
			twf->putStr("Single->init(");
			twf->putStr(Base);
			twf->putStr("SingleSet);\n");

			twf->putStr("\n\t");
			twf->putStr(Base);
			twf->putStr("Inline = new DCilist;");
			twf->putStr("\n\t");
			twf->putStr(Base);
			twf->putStr("Inline->init(");
			twf->putStr(Base);
			twf->putStr("InlineSet);\n");

			twf->putStr("\n\t");
			twf->putStr(Base);
			twf->putStr("Levels = new DCilist;");
			twf->putStr("\n\t");
			twf->putStr(Base);
			twf->putStr("Levels->init(");
			twf->putStr(Base);
			twf->putStr("LevelsSet);\n");

			twf->putStr("\n\t");
			twf->putStr(Base);
			twf->putStr("AtFirst = new DCilist;");
			twf->putStr("\n\t");
			twf->putStr(Base);
			twf->putStr("AtFirst->init(");
			twf->putStr(Base);
			twf->putStr("AtFirstSet);\n");

			twf->putStr("\n\t");
			twf->putStr(Base);
			twf->putStr("Parents = new DCvlist;");
			twf->putStr("\n\tDCvlist *rescan = MergeElemLists(");
			twf->putStr(Base);
			twf->putStr("Parents, ");
			twf->putStr(Base);
			twf->putStr("ElemSets);\n");
			twf->putStr("	assert(rescan==NULL);\n");

			twf->putStr("\n\tD");
			twf->putStr(Base);
			twf->putStr("Tags = new tagset;");

			twf->putStr("\n\tD");
			twf->putStr(Base);
			twf->putStr("Tags->setname = DTopicTypes->find(tagid);");

			twf->putStr("\n\tD");
			twf->putStr(Base);
			twf->putStr("Tags->dtype = PrologDType");
			twf->putStr(Base);
			twf->putStr(";");

			twf->putStr("\n\tD");
			twf->putStr(Base);
			twf->putStr("Tags->dtd = PrologDTD");
			twf->putStr(Base);
			twf->putStr(";");

			twf->putStr("\n\tD");
			twf->putStr(Base);
			twf->putStr("Tags->names = ");
			twf->putStr(Base);
			twf->putStr("Names;");

			twf->putStr("\n\tD");
			twf->putStr(Base);
			twf->putStr("Tags->single = ");
			twf->putStr(Base);
			twf->putStr("Single;");

			twf->putStr("\n\tD");
			twf->putStr(Base);
			twf->putStr("Tags->esetnames = ");
			twf->putStr(Base);
			twf->putStr("ElemSetNames;");

			twf->putStr("\n\tD");
			twf->putStr(Base);
			twf->putStr("Tags->esetlists = ");
			twf->putStr(Base);
			twf->putStr("ElemSets;");
			twf->putStr("\n\tD");
			twf->putStr(Base);
			twf->putStr("Tags->levels = ");
			twf->putStr(Base);
			twf->putStr("Levels;");
			twf->putStr("\n\tD");
			twf->putStr(Base);
			twf->putStr("Tags->parents = ");
			twf->putStr(Base);
			twf->putStr("Parents;");
			twf->putStr("\n\tD");
			twf->putStr(Base);
			twf->putStr("Tags->inlin = ");
			twf->putStr(Base);
			twf->putStr("Inline;");

			twf->putStr("\n\tD");
			twf->putStr(Base);
			twf->putStr("Tags->atfirst = ");
			twf->putStr(Base);
			twf->putStr("AtFirst;\n");

			// add tblnames and tbllists init here ****
			DumpDITATablesCpp(twf, ts, Base, 2);

			twf->putStr("\n\tDTagSets->add(D");
			twf->putStr(Base);
			twf->putStr("Tags, tagid);\n");

			twf->putStr("\n\tDTopicRoots->add(");
			twf->putStr(Base);
			twf->putStr("Names->find(1), tagid);\n");

			twf->putStr("}\n");
		}

		twf->putStr("\n\nFor dwhtm.h:\n");

		twf->putStr("\n	static npair ");
		twf->putStr(Base);
		twf->putStr("NamesSet");
		twf->putChar(ver);
		twf->putStr("[];");

		twf->putStr("\n	static ipair ");
		twf->putStr(Base);
		twf->putStr("InlineSet");
		twf->putChar(ver);
		twf->putStr("[];");

		twf->putStr("\n	static ipair ");
		twf->putStr(Base);
		twf->putStr("SingleSet");
		twf->putChar(ver);
		twf->putStr("[];");

		while ((DSPsetnames = DSPsetnames->next) != NULL) {
			twf->putStr("\n	static long ");
			twf->putStr(FixSetName((char *) DSPsetnames->name + 1));
			twf->putStr(Base);
			twf->putStr("Par");
			twf->putChar(ver);
			twf->putStr("[];");
		}
		DSPsetnames = ts->esetnames;

		twf->putStr("\n	static vpair ");
		twf->putStr(Base);
		twf->putStr("ElemSetsSet");
		twf->putChar(ver);
		twf->putStr("[];");

		twf->putStr("\n	static npair ");
		twf->putStr(Base);
		twf->putStr("ElemSetNamesSet");
		twf->putChar(ver);
		twf->putStr("[];");

		twf->putStr("\n	static ipair ");
		twf->putStr(Base);
		twf->putStr("LevelsSet");
		twf->putChar(ver);
		twf->putStr("[];");

		twf->putStr("\n	static ipair ");
		twf->putStr(Base);
		twf->putStr("AtFirstSet");
		twf->putChar(ver);
		twf->putStr("[];");

		if (newbase) {
			twf->putStr("\n	static void SetDITA");
			twf->putStr(Base);
			twf->putStr("Type(long tagid);\n");

			// add tblnames and tbllists decl here ****
			DumpDITATablesCpp(twf, ts, Base, 3);
		}
	}

	else {
#endif
		char *body = NULL;

		twf->putStr("\n\n;DITA specialization file content ");
		twf->putStr(tname);

		twf->putStr("\n\n[Topic]");
		twf->putStr("\nTopicRoot=");
		twf->putStr(ts->names->find(1));
		twf->putStr("\nTopicStart=");
		twf->putStr(ts->start);
		if ((body = ts->names->find(2)) != NULL) {
			twf->putStr("\nTopicBody=");
			twf->putStr(body);
		}
		if (deriv) {
			twf->putStr("\nTopicDerivation=");
			twf->putStr(deriv);
		}
		twf->putStr("\nPrologDType=");
		twf->putStr(ts->dtype);
		twf->putStr("\nPrologDTD=");
		if (*ts->dtd == ' ')
			twf->putChar(' '); // to retain leading space
		twf->putStr(ts->dtd);
		twf->putCr();

		twf->putStr("\n[TopicParents]\n");
		DSPnames->writeall(twf, DumpNameList);

		twf->putStr("\n[ElementSets]\n");
		DSPelemsets->writeallval(twf, DumpSetList);

		twf->putStr("\n[ElementTypes]\n");
		DSPinlins->writeall(twf, DumpElemTypeList);

		twf->putStr("\n[TopicLevels]\n");
		DSPlevels->writeall(twf, DumpPropList);

		twf->putStr("\n[TopicFirst]\n");
		DSPatfirst->writeall(twf, DumpFirstList);

		DumpDITATables(twf, ts);
#ifdef _DEBUG
	}
#endif
	twf->CloseFile();
}


char *HXMwr::FixSetName(char *n)
{
	if (!strchr(n, '-'))
		return n;

	short len = (short) strlen(n);
	char *fx = NewName(len);
	short j = 0;
	for (short i = 0; i < len; i++) {
		if (n[i] != '-')
			fx[j++] = n[i];
	}
	fx[j] = '\0';
	return fx;		
}


void HXMwr::DumpNameList(DCwfile *w, char *n, long i)
{
	if (i == 0)
		return;

	if ((i == 1)
	 || (i == 2))
		return;  // exclude topic and body here

	long p = 0;
	char *cpt = NULL;

	w->putStr(n);
	w->putChar('=');
	if ((p = DSPsparents->find(i)) == 0)
		w->putStr("Any");
	else if (p < ESetBase) {
		if ((cpt = DSPnames->find(p)) != NULL)
			w->putStr(cpt);
		else w->putStr("Missing single");
	}
	else {
		if ((cpt = DSPsetnames->find(p)) != NULL)
			w->putStr(cpt);
		else {
			w->putStr("*set");
			w->putNum(p);
		}
	}
	w->putCr();
}

void HXMwr::DumpSetList(DCfile *wf, void *v, long i)
{
	DCwfile *w = (DCwfile *) wf;

	if (i == 0)
		return;

	DClist *lpt = (DClist *) v;
	char *cpt = NULL;

	if ((cpt = DSPsetnames->find(i)) != NULL)
		w->putStr(cpt);
	else {
		w->putStr("*set");
		w->putNum(i);
	}
	w->putChar('=');

	while ((lpt = lpt->next) != NULL) {
		if (lpt->id > ESetBase) {
			if ((cpt = DSPsetnames->find(lpt->id)) != NULL)
				w->putStr(cpt);
			else {
				w->putStr("*set");
				w->putNum(lpt->id);
			}
		}
		else {
			if ((cpt = DSPnames->find(lpt->id)) != NULL)
				w->putStr(cpt);
			else
				w->putNum(lpt->id);
		}
		if (lpt->next)
			w->putChar(' ');
	}

	w->putCr();
}

void HXMwr::DumpElemTypeList(DCwfile *w, long v, long i)
{
	if (v == NULL)
		return;

	char *cpt = NULL;

	if ((cpt = DSPnames->find(i)) != NULL)
		w->putStr(cpt);
	else
		w->putNum(i);

	w->putChar('=');
	w->putStr((v & 2) ? "Inline" : "Block");
	if (v & 1)
		w->putStr(" Text");
	if (v & 4)
		w->putStr(" Preform");
	w->putCr();
}

void HXMwr::DumpPropList(DCwfile *w, long v, long i)
{
	if (v == NULL)
		return;

	char *cpt = NULL;

	if ((cpt = DSPnames->find(i)) != NULL)
		w->putStr(cpt);
	else
		w->putNum(i);

	w->putChar('=');
	w->putNum(v);
	w->putCr();
}

void HXMwr::DumpFirstList(DCwfile *w, long v, long i)
{
	if (i == 0)
		return;

	char *cpt = NULL;

	if ((cpt = DSPnames->find(i)) != NULL)
		w->putStr(cpt);
	else
		w->putNum(i);

	w->putChar('=');
	if (v == -1)
		w->putStr("Any");
	else if (v < ESetBase) {
		if ((cpt = DSPnames->find(v)) != NULL)
			w->putStr(cpt);
		else
			w->putNum(v);
	}
	else {
		if ((cpt = DSPsetnames->find(v)) != NULL)
			w->putStr(cpt);
		else {
			w->putStr("*set");
			w->putNum(v);
		}
	}
	w->putCr();
}



// end of dwhtmds.cpp

