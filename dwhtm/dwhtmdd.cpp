// dwhtmdd.cpp is the data for dcl HTM DITA specialization, JHG
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


// tagsets for standard DTDs

npair *DTtag::DTopicTypesSet = DTopicTypesSet1;

npair *DTtag::DTopicTypesSets[] = {
 DTopicTypesSet0, DTopicTypesSet1
};


npair *DTtag::DTopicStartsSet = DTopicStartsSet1;

npair *DTtag::DTopicStartsSets[] = {
 DTopicStartsSet0, DTopicStartsSet1
};

npair *DTtag::DRelTopicTypesSet = DRelTopicTypesSet1;

npair *DTtag::DRelTopicTypesSets[] = {
 DRelTopicTypesSet0, DRelTopicTypesSet1
};


npair *DTtag::CommonNamesSet = CommonNamesSet1;

npair *DTtag::CommonNamesSets[] = {
 CommonNamesSet0, CommonNamesSet1
};


// preloaded DITA topic type info

char *DTtag::PrologDTypeDita = PrologDTypeDita1;
char *DTtag::PrologDTypeDitas[] = {
 PrologDTypeDita0, PrologDTypeDita1 };

char *DTtag::PrologDTDDita = PrologDTDDita1;
char *DTtag::PrologDTDDitas[] = {
 PrologDTDDita0, PrologDTDDita1 };

char *DTtag::PrologDTypeTopic = PrologDTypeTopic1;
char *DTtag::PrologDTypeTopics[] = {
 PrologDTypeTopic0, PrologDTypeTopic1 };

char *DTtag::PrologDTDTopic = PrologDTDTopic1;
char *DTtag::PrologDTDTopics[] = {
 PrologDTDTopic0, PrologDTDTopic1 };

char *DTtag::PrologDTypeConcept = PrologDTypeConcept1;
char *DTtag::PrologDTypeConcepts[] = {
 PrologDTypeConcept0, PrologDTypeConcept1 };

char *DTtag::PrologDTDConcept = PrologDTDConcept1;
char *DTtag::PrologDTDConcepts[] = {
 PrologDTDConcept0, PrologDTDConcept1 };

char *DTtag::PrologDTypeTask = PrologDTypeTask1;
char *DTtag::PrologDTypeTasks[] = {
 PrologDTypeTask0, PrologDTypeTask1 };

char *DTtag::PrologDTDTask = PrologDTDTask1;
char *DTtag::PrologDTDTasks[] = {
 PrologDTDTask0, PrologDTDTask1 };

char *DTtag::PrologDTypeReference = PrologDTypeReference1;
char *DTtag::PrologDTypeReferences[] = {
 PrologDTypeReference0, PrologDTypeReference1 };

char *DTtag::PrologDTDReference = PrologDTDReference1;
char *DTtag::PrologDTDReferences[] = {
 PrologDTDReference0, PrologDTDReference1 };

char *DTtag::PrologDTypeGlossentry = PrologDTypeGlossentry1;
char *DTtag::PrologDTypeGlossentrys[] = {
 NULL, PrologDTypeGlossentry1 };

char *DTtag::PrologDTDGlossentry = PrologDTDGlossentry1;
char *DTtag::PrologDTDGlossentrys[] = {
 NULL, PrologDTDGlossentry1 };

char *DTtag::PrologDTypeMap = PrologDTypeMap1;
char *DTtag::PrologDTypeMaps[] = {
 PrologDTypeMap0, PrologDTypeMap1 };

char *DTtag::PrologDTDMap = PrologDTDMap1;
char *DTtag::PrologDTDMaps[] = {
 PrologDTDMap0, PrologDTDMap1 };

char *DTtag::PrologDTypeBookmap = PrologDTypeBookmap1;
char *DTtag::PrologDTypeBookmaps[] = {
 NULL, PrologDTypeBookmap1 };

char *DTtag::PrologDTDBookmap = PrologDTDBookmap1;
char *DTtag::PrologDTDBookmaps[] = {
 NULL, PrologDTDBookmap1 };


// topic

npair *DTtag::TopicNamesSet = DTtag::TopicNamesSet1;

npair *DTtag::TopicNamesSets[] = {
 DTtag::TopicNamesSet0, DTtag::TopicNamesSet1
};

ipair *DTtag::TopicInlineSet = DTtag::TopicInlineSet1;

ipair *DTtag::TopicInlineSets[] = {
 DTtag::TopicInlineSet0, DTtag::TopicInlineSet1
};

ipair *DTtag::TopicSingleSet = DTtag::TopicSingleSet1;

ipair *DTtag::TopicSingleSets[] = {
 DTtag::TopicSingleSet0, DTtag::TopicSingleSet1
};

vpair *DTtag::TopicElemSetsSet = DTtag::TopicElemSetsSet1;

vpair *DTtag::TopicElemSetsSets[] = {
 DTtag::TopicElemSetsSet0, DTtag::TopicElemSetsSet1
};

npair *DTtag::TopicElemSetNamesSet = DTtag::TopicElemSetNamesSet1;

npair *DTtag::TopicElemSetNamesSets[] = {
 DTtag::TopicElemSetNamesSet0, DTtag::TopicElemSetNamesSet1
};

ipair *DTtag::TopicLevelsSet = DTtag::TopicLevelsSet1;

ipair *DTtag::TopicLevelsSets[] = {
 DTtag::TopicLevelsSet0, DTtag::TopicLevelsSet1
};

ipair *DTtag::TopicAtFirstSet = DTtag::TopicAtFirstSet1;

ipair *DTtag::TopicAtFirstSets[] = {
 DTtag::TopicAtFirstSet0, DTtag::TopicAtFirstSet1
};


// concept


npair *DTtag::ConceptNamesSet = DTtag::ConceptNamesSet1;

npair *DTtag::ConceptNamesSets[] = {
 DTtag::ConceptNamesSet0, DTtag::ConceptNamesSet1
};

ipair *DTtag::ConceptInlineSet = DTtag::ConceptInlineSet1;

ipair *DTtag::ConceptInlineSets[] = {
 DTtag::ConceptInlineSet0, DTtag::ConceptInlineSet1
};

ipair *DTtag::ConceptSingleSet = DTtag::ConceptSingleSet1;

ipair *DTtag::ConceptSingleSets[] = {
 DTtag::ConceptSingleSet0, DTtag::ConceptSingleSet1
};

vpair *DTtag::ConceptElemSetsSet = DTtag::ConceptElemSetsSet1;

vpair *DTtag::ConceptElemSetsSets[] = {
 DTtag::ConceptElemSetsSet0, DTtag::ConceptElemSetsSet1
};

npair *DTtag::ConceptElemSetNamesSet = DTtag::ConceptElemSetNamesSet1;

npair *DTtag::ConceptElemSetNamesSets[] = {
 DTtag::ConceptElemSetNamesSet0, DTtag::ConceptElemSetNamesSet1
};

ipair *DTtag::ConceptLevelsSet = DTtag::ConceptLevelsSet1;

ipair *DTtag::ConceptLevelsSets[] = {
 DTtag::ConceptLevelsSet0, DTtag::ConceptLevelsSet1
};

ipair *DTtag::ConceptAtFirstSet = DTtag::ConceptAtFirstSet1;

ipair *DTtag::ConceptAtFirstSets[] = {
 DTtag::ConceptAtFirstSet0, DTtag::ConceptAtFirstSet1
};


// task


npair *DTtag::TaskNamesSet = DTtag::TaskNamesSet1;

npair *DTtag::TaskNamesSets[] = {
 DTtag::TaskNamesSet0, DTtag::TaskNamesSet1
};

ipair *DTtag::TaskInlineSet = DTtag::TaskInlineSet1;

ipair *DTtag::TaskInlineSets[] = {
 DTtag::TaskInlineSet0, DTtag::TaskInlineSet1
};

ipair *DTtag::TaskSingleSet = DTtag::TaskSingleSet1;

ipair *DTtag::TaskSingleSets[] = {
 DTtag::TaskSingleSet0, DTtag::TaskSingleSet1
};

vpair *DTtag::TaskElemSetsSet = DTtag::TaskElemSetsSet1;

vpair *DTtag::TaskElemSetsSets[] = {
 DTtag::TaskElemSetsSet0, DTtag::TaskElemSetsSet1
};

npair *DTtag::TaskElemSetNamesSet = DTtag::TaskElemSetNamesSet1;

npair *DTtag::TaskElemSetNamesSets[] = {
 DTtag::TaskElemSetNamesSet0, DTtag::TaskElemSetNamesSet1
};

ipair *DTtag::TaskLevelsSet = DTtag::TaskLevelsSet1;

ipair *DTtag::TaskLevelsSets[] = {
 DTtag::TaskLevelsSet0, DTtag::TaskLevelsSet1
};

ipair *DTtag::TaskAtFirstSet = DTtag::TaskAtFirstSet1;

ipair *DTtag::TaskAtFirstSets[] = {
 DTtag::TaskAtFirstSet0, DTtag::TaskAtFirstSet1
};


// reference

npair *DTtag::ReferenceNamesSet = DTtag::ReferenceNamesSet1;

npair *DTtag::ReferenceNamesSets[] = {
 DTtag::ReferenceNamesSet0, DTtag::ReferenceNamesSet1
};

ipair *DTtag::ReferenceInlineSet = DTtag::ReferenceInlineSet1;

ipair *DTtag::ReferenceInlineSets[] = {
 DTtag::ReferenceInlineSet0, DTtag::ReferenceInlineSet1
};

ipair *DTtag::ReferenceSingleSet = DTtag::ReferenceSingleSet1;

ipair *DTtag::ReferenceSingleSets[] = {
 DTtag::ReferenceSingleSet0, DTtag::ReferenceSingleSet1
};

vpair *DTtag::ReferenceElemSetsSet = DTtag::ReferenceElemSetsSet1;

vpair *DTtag::ReferenceElemSetsSets[] = {
 DTtag::ReferenceElemSetsSet0, DTtag::ReferenceElemSetsSet1
};

npair *DTtag::ReferenceElemSetNamesSet = DTtag::ReferenceElemSetNamesSet1;

npair *DTtag::ReferenceElemSetNamesSets[] = {
 DTtag::ReferenceElemSetNamesSet0, DTtag::ReferenceElemSetNamesSet1
};

ipair *DTtag::ReferenceLevelsSet = DTtag::ReferenceLevelsSet1;

ipair *DTtag::ReferenceLevelsSets[] = {
 DTtag::ReferenceLevelsSet0, DTtag::ReferenceLevelsSet1
};

ipair *DTtag::ReferenceAtFirstSet = DTtag::ReferenceAtFirstSet1;

ipair *DTtag::ReferenceAtFirstSets[] = {
 DTtag::ReferenceAtFirstSet0, DTtag::ReferenceAtFirstSet1
};


// glossary

npair *DTtag::GlossentryNamesSet = DTtag::GlossentryNamesSet1;

npair *DTtag::GlossentryNamesSets[] = {
 NULL, DTtag::GlossentryNamesSet1
};

ipair *DTtag::GlossentryInlineSet = DTtag::GlossentryInlineSet1;

ipair *DTtag::GlossentryInlineSets[] = {
 NULL, DTtag::GlossentryInlineSet1
};

ipair *DTtag::GlossentrySingleSet = DTtag::GlossentrySingleSet1;

ipair *DTtag::GlossentrySingleSets[] = {
 NULL, DTtag::GlossentrySingleSet1
};

vpair *DTtag::GlossentryElemSetsSet = DTtag::GlossentryElemSetsSet1;

vpair *DTtag::GlossentryElemSetsSets[] = {
 NULL, DTtag::GlossentryElemSetsSet1
};

npair *DTtag::GlossentryElemSetNamesSet = DTtag::GlossentryElemSetNamesSet1;

npair *DTtag::GlossentryElemSetNamesSets[] = {
 NULL, DTtag::GlossentryElemSetNamesSet1
};

ipair *DTtag::GlossentryLevelsSet = DTtag::GlossentryLevelsSet1;

ipair *DTtag::GlossentryLevelsSets[] = {
 NULL, DTtag::GlossentryLevelsSet1
};

ipair *DTtag::GlossentryAtFirstSet = DTtag::GlossentryAtFirstSet1;

ipair *DTtag::GlossentryAtFirstSets[] = {
 NULL, DTtag::GlossentryAtFirstSet1
};


// maps

npair *DTtag::MapNamesSet = DTtag::MapNamesSet1;

npair *DTtag::MapNamesSets[] = {
 DTtag::MapNamesSet0, DTtag::MapNamesSet1
};

ipair *DTtag::MapInlineSet = DTtag::MapInlineSet1;

ipair *DTtag::MapInlineSets[] = {
 DTtag::MapInlineSet0, DTtag::MapInlineSet1
};

ipair *DTtag::MapSingleSet = DTtag::MapSingleSet1;

ipair *DTtag::MapSingleSets[] = {
 DTtag::MapSingleSet0, DTtag::MapSingleSet1
};

vpair *DTtag::MapElemSetsSet = DTtag::MapElemSetsSet1;

vpair *DTtag::MapElemSetsSets[] = {
 DTtag::MapElemSetsSet0, DTtag::MapElemSetsSet1
};

npair *DTtag::MapElemSetNamesSet = DTtag::MapElemSetNamesSet1;

npair *DTtag::MapElemSetNamesSets[] = {
 DTtag::MapElemSetNamesSet0, DTtag::MapElemSetNamesSet1
};

ipair *DTtag::MapLevelsSet = DTtag::MapLevelsSet1;

ipair *DTtag::MapLevelsSets[] = {
 DTtag::MapLevelsSet0, DTtag::MapLevelsSet1
};

ipair *DTtag::MapAtFirstSet = DTtag::MapAtFirstSet1;

ipair *DTtag::MapAtFirstSets[] = {
 DTtag::MapAtFirstSet0, DTtag::MapAtFirstSet1
};

// bookmaps

npair *DTtag::BookmapNamesSet = DTtag::BookmapNamesSet1;

npair *DTtag::BookmapNamesSets[] = {
 NULL, DTtag::BookmapNamesSet1
};

ipair *DTtag::BookmapInlineSet = DTtag::BookmapInlineSet1;

ipair *DTtag::BookmapInlineSets[] = {
 NULL, DTtag::BookmapInlineSet1
};

ipair *DTtag::BookmapSingleSet = DTtag::BookmapSingleSet1;

ipair *DTtag::BookmapSingleSets[] = {
 NULL, DTtag::BookmapSingleSet1
};

vpair *DTtag::BookmapElemSetsSet = DTtag::BookmapElemSetsSet1;

vpair *DTtag::BookmapElemSetsSets[] = {
 NULL, DTtag::BookmapElemSetsSet1
};

npair *DTtag::BookmapElemSetNamesSet = DTtag::BookmapElemSetNamesSet1;

npair *DTtag::BookmapElemSetNamesSets[] = {
 NULL, DTtag::BookmapElemSetNamesSet1
};

ipair *DTtag::BookmapLevelsSet = DTtag::BookmapLevelsSet1;

ipair *DTtag::BookmapLevelsSets[] = {
 NULL, DTtag::BookmapLevelsSet1
};

ipair *DTtag::BookmapAtFirstSet = DTtag::BookmapAtFirstSet1;

ipair *DTtag::BookmapAtFirstSets[] = {
 NULL, DTtag::BookmapAtFirstSet1
};



// end of dwhtmdd.cpp

