// dwhtmd1.cpp is the data for dcl HTM DITA 1.1 specialization, JHG
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


// tagsets for version 1.1 DTDs

npair DTtag::DTopicTypesSet1[] = {
 { 1, "topic"},
 { 2, "concept"},
 { 3, "task"},
 { 4, "reference"},
 { 5, "glossary"},
 { 6, "map"},
 { 7, "bookmap"},
 { 0, NULL }
 };

npair DTtag::DTopicStartsSet1[] = {
 { 1, "title"},
 { 2, "title"},
 { 3, "title"},
 { 4, "title"},
 { 5, "glossterm"},
 { 6, "title"},
 { 7, "booktitle"},
 { 0, NULL }
 };

npair DTtag::DRelTopicTypesSet1[] = {
 { 1, "concept"},
 { 2, "task"},
 { 3, "reference"},
 { 0, NULL }
 };

npair DTtag::CommonNamesSet1[] = {
 { 1, "topic"},
 { 2, "body"},
 { 0, NULL }
 };


// preloaded DITA topic type info

char *DTtag::PrologDTypeDita1 = {"\"-//OASIS//DTD DITA 1.1 Composite//EN\""};
char *DTtag::PrologDTDDita1 = {"  \"http://docs.oasis-open.org/dita/v1.1/OS/dtd/ditabase.dtd\""};
char *DTtag::PrologDTypeTopic1 = {"\"-//OASIS//DTD DITA 1.1 Topic//EN\""};
char *DTtag::PrologDTDTopic1 = {"  \"http://docs.oasis-open.org/dita/v1.1/OS/dtd/topic.dtd\""};
char *DTtag::PrologDTypeConcept1 = {"\"-//OASIS//DTD DITA 1.1 Concept//EN\""};
char *DTtag::PrologDTDConcept1 = {"  \"http://docs.oasis-open.org/dita/v1.1/OS/dtd/concept.dtd\""};
char *DTtag::PrologDTypeTask1 = {"\"-//OASIS//DTD DITA 1.1 Task//EN\""};
char *DTtag::PrologDTDTask1 = {"  \"http://docs.oasis-open.org/dita/v1.1/OS/dtd/task.dtd\""};
char *DTtag::PrologDTypeReference1 = {"\"-//OASIS//DTD DITA 1.1 Reference//EN\""};
char *DTtag::PrologDTDReference1 = {"  \"http://docs.oasis-open.org/dita/v1.1/OS/dtd/reference.dtd\""};
char *DTtag::PrologDTypeGlossentry1 = {"\"-//OASIS//DTD DITA 1.1 Glossary//EN\""};
char *DTtag::PrologDTDGlossentry1 = {"  \"http://docs.oasis-open.org/dita/v1.1/OS/dtd/glossary.dtd\""};

char *DTtag::PrologDTypeMap1 = {"\"-//OASIS//DTD DITA 1.1 Map//EN\""};
char *DTtag::PrologDTDMap1 = {"  \"http://docs.oasis-open.org/dita/v1.1/OS/dtd/map.dtd\""};
char *DTtag::PrologDTypeBookmap1 = {"\"-//OASIS//DTD DITA 1.1 BookMap//EN\""};
char *DTtag::PrologDTDBookmap1 = {"  \"http://docs.oasis-open.org/dita/v1.1/OS/dtd/bookmap.dtd\""};


// topic

npair DTtag::TopicNamesSet1[] = {
 { 1, "topic"},
 { 2, "body"},
 { 3, "abstract"},
 { 4, "alt"},
 { 5, "apiname"},
 { 6, "area"},
 { 7, "audience"},
 { 8, "author"},
 { 9, "b"},
 { 10, "boolean"},
 { 11, "brand"},
 { 12, "category"},
 { 13, "cite"},
 { 14, "cmdname"},
 { 15, "codeblock"},
 { 16, "codeph"},
 { 17, "colspec"},
 { 18, "component"},
 { 19, "coords"},
 { 20, "copyrholder"},
 { 21, "copyright"},
 { 22, "copyryear"},
 { 23, "created"},
 { 24, "critdates"},
 { 25, "data"},
 { 26, "data-about"},
 { 27, "dd"},
 { 28, "ddhd"},
 { 29, "delim"},
 { 30, "desc"},
 { 31, "dl"},
 { 32, "dlentry"},
 { 33, "dlhead"},
 { 34, "draft-comment"},
 { 35, "dt"},
 { 36, "dthd"},
 { 37, "entry"},
 { 38, "example"},
 { 39, "featnum"},
 { 40, "fig"},
 { 41, "figgroup"},
 { 42, "filepath"},
 { 43, "fn"},
 { 44, "foreign"},
 { 45, "fragment"},
 { 46, "fragref"},
 { 47, "groupchoice"},
 { 48, "groupcomp"},
 { 49, "groupseq"},
 { 50, "i"},
 { 51, "image"},
 { 52, "imagemap"},
 { 53, "index-base"},
 { 54, "index-see"},
 { 55, "index-see-also"},
 { 56, "index-sort-as"},
 { 57, "indexterm"},
 { 58, "indextermref"},
 { 59, "itemgroup"},
 { 60, "keyword"},
 { 61, "keywords"},
 { 62, "kwd"},
 { 63, "li"},
 { 64, "lines"},
 { 65, "link"},
 { 66, "linkinfo"},
 { 67, "linklist"},
 { 68, "linkpool"},
 { 69, "linktext"},
 { 70, "lq"},
 { 71, "menucascade"},
 { 72, "metadata"},
 { 73, "msgblock"},
 { 74, "msgnum"},
 { 75, "msgph"},
 { 76, "navtitle"},
 { 77, "note"},
 { 78, "object"},
 { 79, "ol"},
 { 80, "oper"},
 { 81, "option"},
 { 82, "othermeta"},
 { 83, "p"},
 { 84, "param"},
 { 85, "parml"},
 { 86, "parmname"},
 { 87, "pd"},
 { 88, "permissions"},
 { 89, "ph"},
 { 90, "platform"},
 { 91, "plentry"},
 { 92, "pre"},
 { 93, "prodinfo"},
 { 94, "prodname"},
 { 95, "prognum"},
 { 96, "prolog"},
 { 97, "pt"},
 { 98, "publisher"},
 { 99, "q"},
 { 100, "related-links"},
 { 101, "repsep"},
 { 102, "required-cleanup"},
 { 103, "resourceid"},
 { 104, "revised"},
 { 105, "row"},
 { 106, "screen"},
 { 107, "searchtitle"},
 { 108, "section"},
 { 109, "sep"},
 { 110, "series"},
 { 111, "shape"},
 { 112, "shortcut"},
 { 113, "shortdesc"},
 { 114, "simpletable"},
 { 115, "sl"},
 { 116, "sli"},
 { 117, "source"},
 { 118, "state"},
 { 119, "stentry"},
 { 120, "sthead"},
 { 121, "strow"},
 { 122, "sub"},
 { 123, "sup"},
 { 124, "synblk"},
 { 125, "synnote"},
 { 126, "synnoteref"},
 { 127, "synph"},
 { 128, "syntaxdiagram"},
 { 129, "systemoutput"},
 { 130, "table"},
 { 131, "tbody"},
 { 132, "term"},
 { 133, "tgroup"},
 { 134, "thead"},
 { 135, "title"},
 { 136, "titlealts"},
 { 137, "tm"},
 { 138, "tt"},
 { 139, "u"},
 { 140, "uicontrol"},
 { 141, "ul"},
 { 142, "unknown"},
 { 143, "userinput"},
 { 144, "var"},
 { 145, "varname"},
 { 146, "vrm"},
 { 147, "vrmlist"},
 { 148, "wintitle"},
 { 149, "xref"},
 { 0, NULL }
 };

ipair DTtag::TopicInlineSet1[] = {
 { 3, 1 },
 { 4, 1 },
 { 5, 3 },
 { 8, 1 },
 { 9, 3 },
 { 10, 2 },
 { 11, 1 },
 { 12, 1 },
 { 13, 3 },
 { 14, 3 },
 { 15, 5 },
 { 16, 3 },
 { 18, 1 },
 { 19, 1 },
 { 20, 1 },
 { 25, 3 },
 { 26, 2 },
 { 27, 1 },
 { 28, 1 },
 { 29, 1 },
 { 30, 1 },
 { 34, 3 },
 { 35, 1 },
 { 36, 1 },
 { 37, 1 },
 { 38, 1 },
 { 39, 1 },
 { 40, 2 },
 { 42, 3 },
 { 43, 1 },
 { 44, 2 },
 { 46, 1 },
 { 50, 3 },
 { 52, 2 },
 { 53, 3 },
 { 54, 3 },
 { 55, 3 },
 { 56, 3 },
 { 57, 3 },
 { 58, 2 },
 { 59, 3 },
 { 60, 3 },
 { 62, 1 },
 { 63, 1 },
 { 64, 5 },
 { 66, 1 },
 { 69, 1 },
 { 70, 1 },
 { 71, 2 },
 { 73, 5 },
 { 74, 3 },
 { 75, 3 },
 { 76, 1 },
 { 77, 1 },
 { 80, 1 },
 { 81, 3 },
 { 83, 1 },
 { 86, 3 },
 { 87, 1 },
 { 89, 3 },
 { 90, 1 },
 { 92, 5 },
 { 94, 1 },
 { 95, 1 },
 { 97, 1 },
 { 98, 1 },
 { 99, 3 },
 { 101, 1 },
 { 102, 2 },
 { 106, 5 },
 { 107, 1 },
 { 108, 1 },
 { 109, 1 },
 { 110, 1 },
 { 111, 1 },
 { 112, 3 },
 { 113, 1 },
 { 116, 1 },
 { 117, 1 },
 { 118, 2 },
 { 119, 1 },
 { 122, 3 },
 { 123, 3 },
 { 125, 1 },
 { 127, 3 },
 { 128, 2 },
 { 129, 3 },
 { 130, 2 },
 { 132, 3 },
 { 135, 1 },
 { 137, 3 },
 { 138, 3 },
 { 139, 3 },
 { 140, 3 },
 { 142, 2 },
 { 143, 3 },
 { 144, 1 },
 { 145, 3 },
 { 148, 3 },
 { 149, 3 },
 { 0, 0 }
 };

ipair DTtag::TopicSingleSet1[] = {
 { 2, 1 },
 { 3, 1 },
 { 4, 51 },
 { 5, 65568 },
 { 6, 52 },
 { 7, 72 },
 { 8, 96 },
 { 9, 65578 },
 { 10, 65579 },
 { 11, 93 },
 { 12, 72 },
 { 13, 65553 },
 { 14, 65568 },
 { 15, 65557 },
 { 16, 65554 },
 { 17, 133 },
 { 18, 93 },
 { 19, 6 },
 { 20, 21 },
 { 21, 96 },
 { 22, 21 },
 { 23, 24 },
 { 24, 96 },
 { 25, 65555 },
 { 26, 65555 },
 { 27, 32 },
 { 28, 33 },
 { 29, 65591 },
 { 30, 65556 },
 { 31, 65557 },
 { 32, 31 },
 { 33, 31 },
 { 34, 65558 },
 { 35, 32 },
 { 36, 33 },
 { 37, 105 },
 { 38, 2 },
 { 39, 93 },
 { 40, 65559 },
 { 41, 65560 },
 { 42, 65578 },
 { 43, 65561 },
 { 44, 65562 },
 { 45, 65563 },
 { 46, 65564 },
 { 47, 65564 },
 { 48, 65564 },
 { 49, 65564 },
 { 50, 65578 },
 { 51, 65565 },
 { 52, 65559 },
 { 53, 57 },
 { 54, 57 },
 { 55, 57 },
 { 56, 57 },
 { 57, 65566 },
 { 58, 65558 },
 { 59, 65567 },
 { 60, 65568 },
 { 61, 72 },
 { 62, 65591 },
 { 63, 65569 },
 { 64, 65557 },
 { 65, 65570 },
 { 66, 67 },
 { 67, 65571 },
 { 68, 65572 },
 { 69, 65 },
 { 70, 65573 },
 { 71, 65578 },
 { 72, 96 },
 { 73, 65557 },
 { 74, 65568 },
 { 75, 65578 },
 { 76, 136 },
 { 77, 65574 },
 { 78, 65575 },
 { 79, 65557 },
 { 80, 65591 },
 { 81, 65576 },
 { 82, 72 },
 { 83, 65577 },
 { 84, 78 },
 { 85, 65557 },
 { 86, 65576 },
 { 87, 91 },
 { 88, 96 },
 { 89, 65578 },
 { 90, 93 },
 { 91, 85 },
 { 92, 65557 },
 { 93, 72 },
 { 94, 93 },
 { 95, 93 },
 { 96, 1 },
 { 97, 91 },
 { 98, 96 },
 { 99, 65579 },
 { 100, 1 },
 { 101, 65580 },
 { 102, 65581 },
 { 103, 96 },
 { 104, 24 },
 { 105, 65582 },
 { 106, 65557 },
 { 107, 136 },
 { 108, 2 },
 { 109, 65591 },
 { 110, 93 },
 { 111, 6 },
 { 112, 140 },
 { 113, 65583 },
 { 114, 65584 },
 { 115, 65557 },
 { 116, 115 },
 { 117, 96 },
 { 118, 65579 },
 { 119, 65585 },
 { 120, 114 },
 { 121, 114 },
 { 122, 65578 },
 { 123, 65578 },
 { 124, 128 },
 { 125, 65564 },
 { 126, 65564 },
 { 127, 65554 },
 { 128, 65559 },
 { 129, 65578 },
 { 130, 65586 },
 { 131, 133 },
 { 132, 65587 },
 { 133, 130 },
 { 134, 133 },
 { 135, 65588 },
 { 136, 1 },
 { 137, 65589 },
 { 138, 65578 },
 { 139, 65578 },
 { 140, 65590 },
 { 141, 65557 },
 { 142, 65562 },
 { 143, 65578 },
 { 144, 65591 },
 { 145, 65568 },
 { 146, 147 },
 { 147, 93 },
 { 148, 65568 },
 { 149, 65592 },
 { 0, 0 }
 };

long DTtag::Part1TopicPar1[] = { 
45, 47, 48, 49, 124, 128, 0 };

long DTtag::Part2TopicPar1[] = { 
9, 13, 15, 16, 25, 50, 70, 77, 
83, 89, 92, 99, 106, 113, 122, 123, 
135, 138, 139, 149, 0 };

long DTtag::Part3TopicPar1[] = { 
3, 27, 28, 30, 34, 35, 36, 37, 
38, 41, 43, 59, 63, 64, 66, 87, 
97, 108, 116, 119, 0 };

long DTtag::Part4TopicPar1[] = { 
4, 8, 11, 12, 20, 42, 53, 54, 
55, 56, 57, 61, 73, 75, 94, 98, 
117, 129, 140, 143, 0 };

long DTtag::Part5TopicPar1[] = { 
18, 19, 29, 39, 46, 69, 76, 80, 
90, 95, 101, 107, 109, 110, 125, 144, 
0 };

long DTtag::Part6TopicPar1[] = { 
3, 27, 28, 30, 34, 35, 36, 37, 
38, 43, 59, 63, 64, 66, 87, 97, 
108, 116, 119, 0 };

long DTtag::Part7TopicPar1[] = { 
70, 77, 83, 89, 113, 135, 149, 0 };

long DTtag::Part8TopicPar1[] = { 
9, 50, 92, 99, 122, 123, 125, 138, 
139, 0 };

long DTtag::Part9TopicPar1[] = { 
4, 8, 11, 12, 20, 42, 53, 54, 
55, 56, 57, 73, 75, 94, 98, 117, 
129, 140, 143, 0 };

long DTtag::Part10TopicPar1[] = { 
2, 18, 19, 29, 39, 40, 46, 69, 
72, 76, 80, 90, 95, 96, 101, 107, 
109, 110, 144, 0 };

long DTtag::Part11TopicPar1[] = { 
3, 2, 25, 27, 34, 37, 38, 40, 
43, 59, 63, 87, 108, 119, 0 };

long DTtag::Part12TopicPar1[] = { 
30, 35, 66, 97, 116, 0 };

long DTtag::Part13TopicPar1[] = { 
3, 15, 16, 27, 34, 37, 38, 43, 
59, 63, 64, 70, 77, 83, 87, 89, 
106, 108, 119, 0 };

long DTtag::Part14TopicPar1[] = { 
3, 2, 27, 30, 34, 37, 38, 40, 
43, 59, 63, 66, 77, 87, 108, 119, 
0 };

long DTtag::Part15TopicPar1[] = { 
3, 27, 37, 38, 59, 63, 70, 83, 
87, 108, 119, 0 };

long DTtag::Part16TopicPar1[] = { 
15, 64, 77, 89, 92, 106, 116, 0 };

long DTtag::Part17TopicPar1[] = { 
3, 2, 27, 34, 38, 59, 63, 70, 
77, 83, 87, 108, 0 };

long DTtag::citeTopicPar1[] = { 
65543, 65547, 65548, 0 };

long DTtag::codephTopicPar1[] = { 
46, 125, 127, 65537, 65538, 0 };

long DTtag::dataTopicPar1[] = { 
26, 65537, 65541, 65544, 65545, 0 };

long DTtag::descTopicPar1[] = { 
40, 65, 67, 78, 130, 149, 0 };

long DTtag::dlTopicPar1[] = { 
70, 83, 65549, 0 };

long DTtag::draftcommentTopicPar1[] = { 
65550, 65551, 0 };

long DTtag::figTopicPar1[] = { 
2, 34, 43, 77, 65550, 0 };

long DTtag::figgroupTopicPar1[] = { 
40, 41, 0 };

long DTtag::fnTopicPar1[] = { 
40, 41, 65550, 65551, 0 };

long DTtag::foreignTopicPar1[] = { 
78, 65537, 65541, 65544, 65545, 0 };

long DTtag::fragmentTopicPar1[] = { 
124, 128, 0 };

long DTtag::groupseqTopicPar1[] = { 
65536, 0 };

long DTtag::imageTopicPar1[] = { 
28, 36, 52, 140, 65542, 65546, 65547, 0 };

long DTtag::indextermTopicPar1[] = { 
53, 54, 55, 57, 61, 65550, 65551, 0 };

long DTtag::itemgroupTopicPar1[] = { 
27, 63, 87, 0 };

long DTtag::keywordTopicPar1[] = { 
65537, 65538, 65539, 65540, 0 };

long DTtag::liTopicPar1[] = { 
79, 141, 0 };

long DTtag::linkTopicPar1[] = { 
67, 68, 100, 0 };

long DTtag::linklistTopicPar1[] = { 
67, 100, 0 };

long DTtag::linkpoolTopicPar1[] = { 
68, 100, 0 };

long DTtag::lqTopicPar1[] = { 
83, 65549, 0 };

long DTtag::noteTopicPar1[] = { 
2, 30, 34, 40, 43, 66, 65550, 0 };

long DTtag::objectTopicPar1[] = { 
70, 77, 83, 65546, 0 };

long DTtag::optionTopicPar1[] = { 
127, 65537, 65538, 65539, 65540, 0 };

long DTtag::pTopicPar1[] = { 
70, 65549, 0 };

long DTtag::phTopicPar1[] = { 
46, 125, 65537, 65538, 0 };

long DTtag::qTopicPar1[] = { 
13, 15, 16, 46, 106, 65541, 65542, 65543, 
0 };

long DTtag::repsepTopicPar1[] = { 
47, 48, 49, 0 };

long DTtag::requiredcleanupTopicPar1[] = { 
2, 65550, 65551, 0 };

long DTtag::rowTopicPar1[] = { 
131, 134, 0 };

long DTtag::shortdescTopicPar1[] = { 
3, 1, 0 };

long DTtag::simpletableTopicPar1[] = { 
40, 65552, 0 };

long DTtag::stentryTopicPar1[] = { 
120, 121, 0 };

long DTtag::tableTopicPar1[] = { 
65552, 0 };

long DTtag::termTopicPar1[] = { 
65537, 65540, 65541, 65544, 0 };

long DTtag::titleTopicPar1[] = { 
25, 38, 40, 41, 67, 108, 130, 1, 
65536, 0 };

long DTtag::tmTopicPar1[] = { 
13, 46, 60, 132, 137, 65541, 65542, 65543, 
0 };

long DTtag::uicontrolTopicPar1[] = { 
46, 71, 125, 65537, 65538, 0 };

long DTtag::varTopicPar1[] = { 
47, 48, 49, 127, 0 };

long DTtag::xrefTopicPar1[] = { 
6, 40, 41, 65543, 65547, 65548, 0 };

long DTtag::titlefirstTopicPar1[] = { 
40, 41, 67, 130, 1, 65536, 0 };

vpair DTtag::TopicElemSetsSet1[] = {
 { 65536, Part1TopicPar1 },
 { 65537, Part2TopicPar1 },
 { 65538, Part3TopicPar1 },
 { 65539, Part4TopicPar1 },
 { 65540, Part5TopicPar1 },
 { 65541, Part6TopicPar1 },
 { 65542, Part7TopicPar1 },
 { 65543, Part8TopicPar1 },
 { 65544, Part9TopicPar1 },
 { 65545, Part10TopicPar1 },
 { 65546, Part11TopicPar1 },
 { 65547, Part12TopicPar1 },
 { 65548, Part13TopicPar1 },
 { 65549, Part14TopicPar1 },
 { 65550, Part15TopicPar1 },
 { 65551, Part16TopicPar1 },
 { 65552, Part17TopicPar1 },
 { 65553, citeTopicPar1 },
 { 65554, codephTopicPar1 },
 { 65555, dataTopicPar1 },
 { 65556, descTopicPar1 },
 { 65557, dlTopicPar1 },
 { 65558, draftcommentTopicPar1 },
 { 65559, figTopicPar1 },
 { 65560, figgroupTopicPar1 },
 { 65561, fnTopicPar1 },
 { 65562, foreignTopicPar1 },
 { 65563, fragmentTopicPar1 },
 { 65564, groupseqTopicPar1 },
 { 65565, imageTopicPar1 },
 { 65566, indextermTopicPar1 },
 { 65567, itemgroupTopicPar1 },
 { 65568, keywordTopicPar1 },
 { 65569, liTopicPar1 },
 { 65570, linkTopicPar1 },
 { 65571, linklistTopicPar1 },
 { 65572, linkpoolTopicPar1 },
 { 65573, lqTopicPar1 },
 { 65574, noteTopicPar1 },
 { 65575, objectTopicPar1 },
 { 65576, optionTopicPar1 },
 { 65577, pTopicPar1 },
 { 65578, phTopicPar1 },
 { 65579, qTopicPar1 },
 { 65580, repsepTopicPar1 },
 { 65581, requiredcleanupTopicPar1 },
 { 65582, rowTopicPar1 },
 { 65583, shortdescTopicPar1 },
 { 65584, simpletableTopicPar1 },
 { 65585, stentryTopicPar1 },
 { 65586, tableTopicPar1 },
 { 65587, termTopicPar1 },
 { 65588, titleTopicPar1 },
 { 65589, tmTopicPar1 },
 { 65590, uicontrolTopicPar1 },
 { 65591, varTopicPar1 },
 { 65592, xrefTopicPar1 },
 { 65593, titlefirstTopicPar1 },
 { 0, NULL }
 };

npair DTtag::TopicElemSetNamesSet1[] = {
 { 65536, "*Part1"},
 { 65537, "*Part2"},
 { 65538, "*Part3"},
 { 65539, "*Part4"},
 { 65540, "*Part5"},
 { 65541, "*Part6"},
 { 65542, "*Part7"},
 { 65543, "*Part8"},
 { 65544, "*Part9"},
 { 65545, "*Part10"},
 { 65546, "*Part11"},
 { 65547, "*Part12"},
 { 65548, "*Part13"},
 { 65549, "*Part14"},
 { 65550, "*Part15"},
 { 65551, "*Part16"},
 { 65552, "*Part17"},
 { 65553, "*cite"},
 { 65554, "*codeph"},
 { 65555, "*data"},
 { 65556, "*desc"},
 { 65557, "*dl"},
 { 65558, "*draft-comment"},
 { 65559, "*fig"},
 { 65560, "*figgroup"},
 { 65561, "*fn"},
 { 65562, "*foreign"},
 { 65563, "*fragment"},
 { 65564, "*groupseq"},
 { 65565, "*image"},
 { 65566, "*indexterm"},
 { 65567, "*itemgroup"},
 { 65568, "*keyword"},
 { 65569, "*li"},
 { 65570, "*link"},
 { 65571, "*linklist"},
 { 65572, "*linkpool"},
 { 65573, "*lq"},
 { 65574, "*note"},
 { 65575, "*object"},
 { 65576, "*option"},
 { 65577, "*p"},
 { 65578, "*ph"},
 { 65579, "*q"},
 { 65580, "*repsep"},
 { 65581, "*required-cleanup"},
 { 65582, "*row"},
 { 65583, "*shortdesc"},
 { 65584, "*simpletable"},
 { 65585, "*stentry"},
 { 65586, "*table"},
 { 65587, "*term"},
 { 65588, "*title"},
 { 65589, "*tm"},
 { 65590, "*uicontrol"},
 { 65591, "*var"},
 { 65592, "*xref"},
 { 65593, "*titlefirst"},
 { 0, NULL }
 };

ipair DTtag::TopicLevelsSet1[] = {
 { 1, 1 },
 { 2, 1 },
 { 3, 1 },
 { 96, 1 },
 { 100, 1 },
 { 136, 1 },
 { 0, 0 }
 };

ipair DTtag::TopicAtFirstSet1[] = {
 { 23, -1 },
 { 25, 26 },
 { 30, 78 },
 { 33, -1 },
 { 36, -1 },
 { 51, 52 },
 { 69, -1 },
 { 76, -1 },
 { 94, -1 },
 { 111, -1 },
 { 120, -1 },
 { 135, 65593 },
 { 0, 0 }
 };



// concept

npair DTtag::ConceptNamesSet1[] = {
 { 1, "concept"},
 { 2, "conbody"},
 { 3, "abstract"},
 { 4, "alt"},
 { 5, "apiname"},
 { 6, "area"},
 { 7, "audience"},
 { 8, "author"},
 { 9, "b"},
 { 10, "boolean"},
 { 11, "brand"},
 { 12, "category"},
 { 13, "cite"},
 { 14, "cmdname"},
 { 15, "codeblock"},
 { 16, "codeph"},
 { 17, "colspec"},
 { 18, "component"},
 { 19, "coords"},
 { 20, "copyrholder"},
 { 21, "copyright"},
 { 22, "copyryear"},
 { 23, "created"},
 { 24, "critdates"},
 { 25, "data"},
 { 26, "data-about"},
 { 27, "dd"},
 { 28, "ddhd"},
 { 29, "delim"},
 { 30, "desc"},
 { 31, "dl"},
 { 32, "dlentry"},
 { 33, "dlhead"},
 { 34, "draft-comment"},
 { 35, "dt"},
 { 36, "dthd"},
 { 37, "entry"},
 { 38, "example"},
 { 39, "featnum"},
 { 40, "fig"},
 { 41, "figgroup"},
 { 42, "filepath"},
 { 43, "fn"},
 { 44, "foreign"},
 { 45, "fragment"},
 { 46, "fragref"},
 { 47, "groupchoice"},
 { 48, "groupcomp"},
 { 49, "groupseq"},
 { 50, "i"},
 { 51, "image"},
 { 52, "imagemap"},
 { 53, "index-base"},
 { 54, "index-see"},
 { 55, "index-see-also"},
 { 56, "index-sort-as"},
 { 57, "indexterm"},
 { 58, "indextermref"},
 { 59, "itemgroup"},
 { 60, "keyword"},
 { 61, "keywords"},
 { 62, "kwd"},
 { 63, "li"},
 { 64, "lines"},
 { 65, "link"},
 { 66, "linkinfo"},
 { 67, "linklist"},
 { 68, "linkpool"},
 { 69, "linktext"},
 { 70, "lq"},
 { 71, "menucascade"},
 { 72, "metadata"},
 { 73, "msgblock"},
 { 74, "msgnum"},
 { 75, "msgph"},
 { 76, "navtitle"},
 { 77, "note"},
 { 78, "object"},
 { 79, "ol"},
 { 80, "oper"},
 { 81, "option"},
 { 82, "othermeta"},
 { 83, "p"},
 { 84, "param"},
 { 85, "parml"},
 { 86, "parmname"},
 { 87, "pd"},
 { 88, "permissions"},
 { 89, "ph"},
 { 90, "platform"},
 { 91, "plentry"},
 { 92, "pre"},
 { 93, "prodinfo"},
 { 94, "prodname"},
 { 95, "prognum"},
 { 96, "prolog"},
 { 97, "pt"},
 { 98, "publisher"},
 { 99, "q"},
 { 100, "related-links"},
 { 101, "repsep"},
 { 102, "required-cleanup"},
 { 103, "resourceid"},
 { 104, "revised"},
 { 105, "row"},
 { 106, "screen"},
 { 107, "searchtitle"},
 { 108, "section"},
 { 109, "sep"},
 { 110, "series"},
 { 111, "shape"},
 { 112, "shortcut"},
 { 113, "shortdesc"},
 { 114, "simpletable"},
 { 115, "sl"},
 { 116, "sli"},
 { 117, "source"},
 { 118, "state"},
 { 119, "stentry"},
 { 120, "sthead"},
 { 121, "strow"},
 { 122, "sub"},
 { 123, "sup"},
 { 124, "synblk"},
 { 125, "synnote"},
 { 126, "synnoteref"},
 { 127, "synph"},
 { 128, "syntaxdiagram"},
 { 129, "systemoutput"},
 { 130, "table"},
 { 131, "tbody"},
 { 132, "term"},
 { 133, "tgroup"},
 { 134, "thead"},
 { 135, "title"},
 { 136, "titlealts"},
 { 137, "tm"},
 { 138, "tt"},
 { 139, "u"},
 { 140, "uicontrol"},
 { 141, "ul"},
 { 142, "unknown"},
 { 143, "userinput"},
 { 144, "var"},
 { 145, "varname"},
 { 146, "vrm"},
 { 147, "vrmlist"},
 { 148, "wintitle"},
 { 149, "xref"},
 { 0, NULL }
 };

ipair DTtag::ConceptInlineSet1[] = {
 { 3, 1 },
 { 4, 1 },
 { 5, 3 },
 { 8, 1 },
 { 9, 3 },
 { 10, 2 },
 { 11, 1 },
 { 12, 1 },
 { 13, 3 },
 { 14, 3 },
 { 15, 5 },
 { 16, 3 },
 { 18, 1 },
 { 19, 1 },
 { 20, 1 },
 { 25, 3 },
 { 26, 2 },
 { 27, 1 },
 { 28, 1 },
 { 29, 1 },
 { 30, 1 },
 { 34, 3 },
 { 35, 1 },
 { 36, 1 },
 { 37, 1 },
 { 38, 1 },
 { 39, 1 },
 { 42, 3 },
 { 43, 1 },
 { 44, 2 },
 { 46, 1 },
 { 50, 3 },
 { 53, 3 },
 { 54, 3 },
 { 55, 3 },
 { 56, 3 },
 { 57, 3 },
 { 58, 2 },
 { 59, 3 },
 { 60, 3 },
 { 62, 1 },
 { 63, 1 },
 { 64, 5 },
 { 66, 1 },
 { 69, 1 },
 { 70, 1 },
 { 71, 2 },
 { 73, 5 },
 { 74, 3 },
 { 75, 3 },
 { 76, 1 },
 { 77, 1 },
 { 80, 1 },
 { 81, 3 },
 { 83, 1 },
 { 86, 3 },
 { 87, 1 },
 { 89, 3 },
 { 90, 1 },
 { 92, 5 },
 { 94, 1 },
 { 95, 1 },
 { 97, 1 },
 { 98, 1 },
 { 99, 3 },
 { 101, 1 },
 { 106, 5 },
 { 107, 1 },
 { 108, 1 },
 { 109, 1 },
 { 110, 1 },
 { 111, 1 },
 { 112, 3 },
 { 113, 1 },
 { 116, 1 },
 { 117, 1 },
 { 118, 2 },
 { 119, 1 },
 { 122, 3 },
 { 123, 3 },
 { 125, 1 },
 { 127, 3 },
 { 129, 3 },
 { 132, 3 },
 { 135, 1 },
 { 137, 3 },
 { 138, 3 },
 { 139, 3 },
 { 140, 3 },
 { 142, 2 },
 { 143, 3 },
 { 144, 1 },
 { 145, 3 },
 { 148, 3 },
 { 149, 3 },
 { 0, 0 }
 };

ipair DTtag::ConceptSingleSet1[] = {
 { 2, 1 },
 { 3, 1 },
 { 4, 51 },
 { 5, 65568 },
 { 6, 52 },
 { 7, 72 },
 { 8, 96 },
 { 9, 65578 },
 { 10, 65579 },
 { 11, 93 },
 { 12, 72 },
 { 13, 65553 },
 { 14, 65568 },
 { 15, 65557 },
 { 16, 65554 },
 { 17, 133 },
 { 18, 93 },
 { 19, 6 },
 { 20, 21 },
 { 21, 96 },
 { 22, 21 },
 { 23, 24 },
 { 24, 96 },
 { 25, 65555 },
 { 26, 65555 },
 { 27, 32 },
 { 28, 33 },
 { 29, 65591 },
 { 30, 65556 },
 { 31, 65557 },
 { 32, 31 },
 { 33, 31 },
 { 34, 65558 },
 { 35, 32 },
 { 36, 33 },
 { 37, 105 },
 { 38, 2 },
 { 39, 93 },
 { 40, 65559 },
 { 41, 65560 },
 { 42, 65578 },
 { 43, 65561 },
 { 44, 65562 },
 { 45, 65563 },
 { 46, 65564 },
 { 47, 65564 },
 { 48, 65564 },
 { 49, 65564 },
 { 50, 65578 },
 { 51, 65565 },
 { 52, 65559 },
 { 53, 57 },
 { 54, 57 },
 { 55, 57 },
 { 56, 57 },
 { 57, 65566 },
 { 58, 65558 },
 { 59, 65567 },
 { 60, 65568 },
 { 61, 72 },
 { 62, 65591 },
 { 63, 65569 },
 { 64, 65557 },
 { 65, 65570 },
 { 66, 67 },
 { 67, 65571 },
 { 68, 65572 },
 { 69, 65 },
 { 70, 65573 },
 { 71, 65578 },
 { 72, 96 },
 { 73, 65557 },
 { 74, 65568 },
 { 75, 65578 },
 { 76, 136 },
 { 77, 65574 },
 { 78, 65575 },
 { 79, 65557 },
 { 80, 65591 },
 { 81, 65576 },
 { 82, 72 },
 { 83, 65577 },
 { 84, 78 },
 { 85, 65557 },
 { 86, 65576 },
 { 87, 91 },
 { 88, 96 },
 { 89, 65578 },
 { 90, 93 },
 { 91, 85 },
 { 92, 65557 },
 { 93, 72 },
 { 94, 93 },
 { 95, 93 },
 { 96, 1 },
 { 97, 91 },
 { 98, 96 },
 { 99, 65579 },
 { 100, 1 },
 { 101, 65580 },
 { 102, 65581 },
 { 103, 96 },
 { 104, 24 },
 { 105, 65582 },
 { 106, 65557 },
 { 107, 136 },
 { 108, 2 },
 { 109, 65591 },
 { 110, 93 },
 { 111, 6 },
 { 112, 140 },
 { 113, 65583 },
 { 114, 65584 },
 { 115, 65557 },
 { 116, 115 },
 { 117, 96 },
 { 118, 65579 },
 { 119, 65585 },
 { 120, 114 },
 { 121, 114 },
 { 122, 65578 },
 { 123, 65578 },
 { 124, 128 },
 { 125, 65564 },
 { 126, 65564 },
 { 127, 65554 },
 { 128, 65559 },
 { 129, 65578 },
 { 130, 65586 },
 { 131, 133 },
 { 132, 65587 },
 { 133, 130 },
 { 134, 133 },
 { 135, 65588 },
 { 136, 1 },
 { 137, 65589 },
 { 138, 65578 },
 { 139, 65578 },
 { 140, 65590 },
 { 141, 65557 },
 { 142, 65562 },
 { 143, 65578 },
 { 144, 65591 },
 { 145, 65568 },
 { 146, 147 },
 { 147, 93 },
 { 148, 65568 },
 { 149, 65592 },
 { 0, 0 }
 };

long DTtag::Part1ConceptPar1[] = { 
45, 47, 48, 49, 124, 128, 0 };

long DTtag::Part2ConceptPar1[] = { 
9, 13, 15, 16, 25, 50, 70, 77, 
83, 89, 92, 99, 106, 113, 122, 123, 
135, 138, 139, 149, 0 };

long DTtag::Part3ConceptPar1[] = { 
3, 27, 28, 30, 34, 35, 36, 37, 
38, 41, 43, 59, 63, 64, 66, 87, 
97, 108, 116, 119, 0 };

long DTtag::Part4ConceptPar1[] = { 
4, 8, 11, 12, 20, 42, 53, 54, 
55, 56, 57, 61, 73, 75, 94, 98, 
117, 129, 140, 143, 0 };

long DTtag::Part5ConceptPar1[] = { 
18, 19, 29, 39, 46, 69, 76, 80, 
90, 95, 101, 107, 109, 110, 125, 144, 
0 };

long DTtag::Part6ConceptPar1[] = { 
3, 27, 28, 30, 34, 35, 36, 37, 
38, 43, 59, 63, 64, 66, 87, 97, 
108, 116, 119, 0 };

long DTtag::Part7ConceptPar1[] = { 
70, 77, 83, 89, 113, 135, 149, 0 };

long DTtag::Part8ConceptPar1[] = { 
9, 50, 92, 99, 122, 123, 125, 138, 
139, 0 };

long DTtag::Part9ConceptPar1[] = { 
4, 8, 11, 12, 20, 42, 53, 54, 
55, 56, 57, 73, 75, 94, 98, 117, 
129, 140, 143, 0 };

long DTtag::Part10ConceptPar1[] = { 
18, 2, 19, 29, 39, 40, 46, 69, 
72, 76, 80, 90, 95, 96, 101, 107, 
109, 110, 144, 0 };

long DTtag::Part11ConceptPar1[] = { 
3, 2, 25, 27, 34, 37, 38, 40, 
43, 59, 63, 87, 108, 119, 0 };

long DTtag::Part12ConceptPar1[] = { 
30, 35, 66, 97, 116, 0 };

long DTtag::Part13ConceptPar1[] = { 
3, 15, 16, 27, 34, 37, 38, 43, 
59, 63, 64, 70, 77, 83, 87, 89, 
106, 108, 119, 0 };

long DTtag::Part14ConceptPar1[] = { 
3, 2, 27, 30, 34, 37, 38, 40, 
43, 59, 63, 66, 77, 87, 108, 119, 
0 };

long DTtag::Part15ConceptPar1[] = { 
3, 27, 37, 38, 59, 63, 70, 83, 
87, 108, 119, 0 };

long DTtag::Part16ConceptPar1[] = { 
15, 64, 77, 89, 92, 106, 116, 0 };

long DTtag::Part17ConceptPar1[] = { 
3, 2, 27, 34, 38, 59, 63, 70, 
77, 83, 87, 108, 0 };

long DTtag::citeConceptPar1[] = { 
65543, 65547, 65548, 0 };

long DTtag::codephConceptPar1[] = { 
46, 125, 127, 65537, 65538, 0 };

long DTtag::dataConceptPar1[] = { 
26, 65537, 65541, 65544, 65545, 0 };

long DTtag::descConceptPar1[] = { 
40, 65, 67, 78, 130, 149, 0 };

long DTtag::dlConceptPar1[] = { 
70, 83, 65549, 0 };

long DTtag::draftcommentConceptPar1[] = { 
65550, 65551, 0 };

long DTtag::figConceptPar1[] = { 
2, 34, 43, 77, 65550, 0 };

long DTtag::figgroupConceptPar1[] = { 
40, 41, 0 };

long DTtag::fnConceptPar1[] = { 
40, 41, 65550, 65551, 0 };

long DTtag::foreignConceptPar1[] = { 
78, 65537, 65541, 65544, 65545, 0 };

long DTtag::fragmentConceptPar1[] = { 
124, 128, 0 };

long DTtag::groupseqConceptPar1[] = { 
65536, 0 };

long DTtag::imageConceptPar1[] = { 
28, 36, 52, 140, 65542, 65546, 65547, 0 };

long DTtag::indextermConceptPar1[] = { 
53, 54, 55, 57, 61, 65550, 65551, 0 };

long DTtag::itemgroupConceptPar1[] = { 
27, 63, 87, 0 };

long DTtag::keywordConceptPar1[] = { 
65537, 65538, 65539, 65540, 0 };

long DTtag::liConceptPar1[] = { 
79, 141, 0 };

long DTtag::linkConceptPar1[] = { 
67, 68, 100, 0 };

long DTtag::linklistConceptPar1[] = { 
67, 100, 0 };

long DTtag::linkpoolConceptPar1[] = { 
68, 100, 0 };

long DTtag::lqConceptPar1[] = { 
83, 65549, 0 };

long DTtag::noteConceptPar1[] = { 
2, 30, 34, 40, 43, 66, 65550, 0 };

long DTtag::objectConceptPar1[] = { 
70, 77, 83, 65546, 0 };

long DTtag::optionConceptPar1[] = { 
127, 65537, 65538, 65539, 65540, 0 };

long DTtag::pConceptPar1[] = { 
70, 65549, 0 };

long DTtag::phConceptPar1[] = { 
46, 125, 65537, 65538, 0 };

long DTtag::qConceptPar1[] = { 
13, 15, 16, 46, 106, 65541, 65542, 65543, 
0 };

long DTtag::repsepConceptPar1[] = { 
47, 48, 49, 0 };

long DTtag::requiredcleanupConceptPar1[] = { 
2, 65550, 65551, 0 };

long DTtag::rowConceptPar1[] = { 
131, 134, 0 };

long DTtag::shortdescConceptPar1[] = { 
3, 1, 0 };

long DTtag::simpletableConceptPar1[] = { 
40, 65552, 0 };

long DTtag::stentryConceptPar1[] = { 
120, 121, 0 };

long DTtag::tableConceptPar1[] = { 
65552, 0 };

long DTtag::termConceptPar1[] = { 
65537, 65540, 65541, 65544, 0 };

long DTtag::titleConceptPar1[] = { 
1, 25, 38, 40, 41, 67, 108, 130, 
65536, 0 };

long DTtag::tmConceptPar1[] = { 
13, 46, 60, 132, 137, 65541, 65542, 65543, 
0 };

long DTtag::uicontrolConceptPar1[] = { 
46, 71, 125, 65537, 65538, 0 };

long DTtag::varConceptPar1[] = { 
47, 48, 49, 127, 0 };

long DTtag::xrefConceptPar1[] = { 
6, 40, 41, 65543, 65547, 65548, 0 };

long DTtag::titlefirstConceptPar1[] = { 
1, 40, 41, 67, 130, 65536, 0 };

vpair DTtag::ConceptElemSetsSet1[] = {
 { 65536, Part1ConceptPar1 },
 { 65537, Part2ConceptPar1 },
 { 65538, Part3ConceptPar1 },
 { 65539, Part4ConceptPar1 },
 { 65540, Part5ConceptPar1 },
 { 65541, Part6ConceptPar1 },
 { 65542, Part7ConceptPar1 },
 { 65543, Part8ConceptPar1 },
 { 65544, Part9ConceptPar1 },
 { 65545, Part10ConceptPar1 },
 { 65546, Part11ConceptPar1 },
 { 65547, Part12ConceptPar1 },
 { 65548, Part13ConceptPar1 },
 { 65549, Part14ConceptPar1 },
 { 65550, Part15ConceptPar1 },
 { 65551, Part16ConceptPar1 },
 { 65552, Part17ConceptPar1 },
 { 65553, citeConceptPar1 },
 { 65554, codephConceptPar1 },
 { 65555, dataConceptPar1 },
 { 65556, descConceptPar1 },
 { 65557, dlConceptPar1 },
 { 65558, draftcommentConceptPar1 },
 { 65559, figConceptPar1 },
 { 65560, figgroupConceptPar1 },
 { 65561, fnConceptPar1 },
 { 65562, foreignConceptPar1 },
 { 65563, fragmentConceptPar1 },
 { 65564, groupseqConceptPar1 },
 { 65565, imageConceptPar1 },
 { 65566, indextermConceptPar1 },
 { 65567, itemgroupConceptPar1 },
 { 65568, keywordConceptPar1 },
 { 65569, liConceptPar1 },
 { 65570, linkConceptPar1 },
 { 65571, linklistConceptPar1 },
 { 65572, linkpoolConceptPar1 },
 { 65573, lqConceptPar1 },
 { 65574, noteConceptPar1 },
 { 65575, objectConceptPar1 },
 { 65576, optionConceptPar1 },
 { 65577, pConceptPar1 },
 { 65578, phConceptPar1 },
 { 65579, qConceptPar1 },
 { 65580, repsepConceptPar1 },
 { 65581, requiredcleanupConceptPar1 },
 { 65582, rowConceptPar1 },
 { 65583, shortdescConceptPar1 },
 { 65584, simpletableConceptPar1 },
 { 65585, stentryConceptPar1 },
 { 65586, tableConceptPar1 },
 { 65587, termConceptPar1 },
 { 65588, titleConceptPar1 },
 { 65589, tmConceptPar1 },
 { 65590, uicontrolConceptPar1 },
 { 65591, varConceptPar1 },
 { 65592, xrefConceptPar1 },
 { 65593, titlefirstConceptPar1 },
 { 0, NULL }
 };

npair DTtag::ConceptElemSetNamesSet1[] = {
 { 65536, "*Part1"},
 { 65537, "*Part2"},
 { 65538, "*Part3"},
 { 65539, "*Part4"},
 { 65540, "*Part5"},
 { 65541, "*Part6"},
 { 65542, "*Part7"},
 { 65543, "*Part8"},
 { 65544, "*Part9"},
 { 65545, "*Part10"},
 { 65546, "*Part11"},
 { 65547, "*Part12"},
 { 65548, "*Part13"},
 { 65549, "*Part14"},
 { 65550, "*Part15"},
 { 65551, "*Part16"},
 { 65552, "*Part17"},
 { 65553, "*cite"},
 { 65554, "*codeph"},
 { 65555, "*data"},
 { 65556, "*desc"},
 { 65557, "*dl"},
 { 65558, "*draft-comment"},
 { 65559, "*fig"},
 { 65560, "*figgroup"},
 { 65561, "*fn"},
 { 65562, "*foreign"},
 { 65563, "*fragment"},
 { 65564, "*groupseq"},
 { 65565, "*image"},
 { 65566, "*indexterm"},
 { 65567, "*itemgroup"},
 { 65568, "*keyword"},
 { 65569, "*li"},
 { 65570, "*link"},
 { 65571, "*linklist"},
 { 65572, "*linkpool"},
 { 65573, "*lq"},
 { 65574, "*note"},
 { 65575, "*object"},
 { 65576, "*option"},
 { 65577, "*p"},
 { 65578, "*ph"},
 { 65579, "*q"},
 { 65580, "*repsep"},
 { 65581, "*required-cleanup"},
 { 65582, "*row"},
 { 65583, "*shortdesc"},
 { 65584, "*simpletable"},
 { 65585, "*stentry"},
 { 65586, "*table"},
 { 65587, "*term"},
 { 65588, "*title"},
 { 65589, "*tm"},
 { 65590, "*uicontrol"},
 { 65591, "*var"},
 { 65592, "*xref"},
 { 65593, "*titlefirst"},
 { 0, NULL }
 };

ipair DTtag::ConceptLevelsSet1[] = {
 { 1, 1 },
 { 2, 1 },
 { 3, 1 },
 { 96, 1 },
 { 100, 1 },
 { 136, 1 },
 { 0, 0 }
 };

ipair DTtag::ConceptAtFirstSet1[] = {
 { 23, -1 },
 { 25, 26 },
 { 30, 78 },
 { 33, -1 },
 { 36, -1 },
 { 51, 52 },
 { 69, -1 },
 { 76, -1 },
 { 94, -1 },
 { 111, -1 },
 { 120, -1 },
 { 135, 65593 },
 { 0, 0 }
 };


// task

npair DTtag::TaskNamesSet1[] = {
 { 1, "task"},
 { 2, "taskbody"},
 { 3, "abstract"},
 { 4, "alt"},
 { 5, "apiname"},
 { 6, "area"},
 { 7, "audience"},
 { 8, "author"},
 { 9, "b"},
 { 10, "boolean"},
 { 11, "brand"},
 { 12, "category"},
 { 13, "chdesc"},
 { 14, "chdeschd"},
 { 15, "chhead"},
 { 16, "choice"},
 { 17, "choices"},
 { 18, "choicetable"},
 { 19, "choption"},
 { 20, "choptionhd"},
 { 21, "chrow"},
 { 22, "cite"},
 { 23, "cmd"},
 { 24, "cmdname"},
 { 25, "codeblock"},
 { 26, "codeph"},
 { 27, "colspec"},
 { 28, "component"},
 { 29, "context"},
 { 30, "coords"},
 { 31, "copyrholder"},
 { 32, "copyright"},
 { 33, "copyryear"},
 { 34, "created"},
 { 35, "critdates"},
 { 36, "data"},
 { 37, "data-about"},
 { 38, "dd"},
 { 39, "ddhd"},
 { 40, "delim"},
 { 41, "desc"},
 { 42, "dl"},
 { 43, "dlentry"},
 { 44, "dlhead"},
 { 45, "draft-comment"},
 { 46, "dt"},
 { 47, "dthd"},
 { 48, "entry"},
 { 49, "example"},
 { 50, "featnum"},
 { 51, "fig"},
 { 52, "figgroup"},
 { 53, "filepath"},
 { 54, "fn"},
 { 55, "foreign"},
 { 56, "fragment"},
 { 57, "fragref"},
 { 58, "groupchoice"},
 { 59, "groupcomp"},
 { 60, "groupseq"},
 { 61, "i"},
 { 62, "image"},
 { 63, "imagemap"},
 { 64, "index-base"},
 { 65, "index-see"},
 { 66, "index-see-also"},
 { 67, "index-sort-as"},
 { 68, "indexterm"},
 { 69, "indextermref"},
 { 70, "info"},
 { 71, "itemgroup"},
 { 72, "keyword"},
 { 73, "keywords"},
 { 74, "kwd"},
 { 75, "li"},
 { 76, "lines"},
 { 77, "link"},
 { 78, "linkinfo"},
 { 79, "linklist"},
 { 80, "linkpool"},
 { 81, "linktext"},
 { 82, "lq"},
 { 83, "menucascade"},
 { 84, "metadata"},
 { 85, "msgblock"},
 { 86, "msgnum"},
 { 87, "msgph"},
 { 88, "navtitle"},
 { 89, "note"},
 { 90, "object"},
 { 91, "ol"},
 { 92, "oper"},
 { 93, "option"},
 { 94, "othermeta"},
 { 95, "p"},
 { 96, "param"},
 { 97, "parml"},
 { 98, "parmname"},
 { 99, "pd"},
 { 100, "permissions"},
 { 101, "ph"},
 { 102, "platform"},
 { 103, "plentry"},
 { 104, "postreq"},
 { 105, "pre"},
 { 106, "prereq"},
 { 107, "prodinfo"},
 { 108, "prodname"},
 { 109, "prognum"},
 { 110, "prolog"},
 { 111, "pt"},
 { 112, "publisher"},
 { 113, "q"},
 { 114, "related-links"},
 { 115, "repsep"},
 { 116, "required-cleanup"},
 { 117, "resourceid"},
 { 118, "result"},
 { 119, "revised"},
 { 120, "row"},
 { 121, "screen"},
 { 122, "searchtitle"},
 { 123, "sep"},
 { 124, "series"},
 { 125, "shape"},
 { 126, "shortcut"},
 { 127, "shortdesc"},
 { 128, "simpletable"},
 { 129, "sl"},
 { 130, "sli"},
 { 131, "source"},
 { 132, "state"},
 { 133, "stentry"},
 { 134, "step"},
 { 135, "stepresult"},
 { 136, "steps"},
 { 137, "steps-unordered"},
 { 138, "stepxmp"},
 { 139, "sthead"},
 { 140, "strow"},
 { 141, "sub"},
 { 142, "substep"},
 { 143, "substeps"},
 { 144, "sup"},
 { 145, "synblk"},
 { 146, "synnote"},
 { 147, "synnoteref"},
 { 148, "synph"},
 { 149, "syntaxdiagram"},
 { 150, "systemoutput"},
 { 151, "table"},
 { 152, "tbody"},
 { 153, "term"},
 { 154, "tgroup"},
 { 155, "thead"},
 { 156, "title"},
 { 157, "titlealts"},
 { 158, "tm"},
 { 159, "tt"},
 { 160, "tutorialinfo"},
 { 161, "u"},
 { 162, "uicontrol"},
 { 163, "ul"},
 { 164, "unknown"},
 { 165, "userinput"},
 { 166, "var"},
 { 167, "varname"},
 { 168, "vrm"},
 { 169, "vrmlist"},
 { 170, "wintitle"},
 { 171, "xref"},
 { 0, NULL }
 };

ipair DTtag::TaskInlineSet1[] = {
 { 3, 1 },
 { 4, 1 },
 { 5, 3 },
 { 8, 1 },
 { 9, 3 },
 { 10, 2 },
 { 11, 1 },
 { 12, 1 },
 { 13, 1 },
 { 14, 1 },
 { 16, 1 },
 { 19, 1 },
 { 20, 1 },
 { 22, 3 },
 { 23, 1 },
 { 24, 3 },
 { 25, 7 },
 { 26, 3 },
 { 28, 1 },
 { 29, 1 },
 { 30, 1 },
 { 31, 1 },
 { 36, 3 },
 { 37, 2 },
 { 38, 1 },
 { 39, 1 },
 { 40, 1 },
 { 41, 1 },
 { 42, 2 },
 { 45, 3 },
 { 46, 1 },
 { 47, 1 },
 { 48, 1 },
 { 49, 1 },
 { 50, 1 },
 { 51, 2 },
 { 53, 3 },
 { 54, 3 },
 { 55, 2 },
 { 57, 1 },
 { 61, 3 },
 { 62, 2 },
 { 63, 2 },
 { 64, 3 },
 { 65, 3 },
 { 66, 3 },
 { 67, 3 },
 { 68, 3 },
 { 69, 2 },
 { 70, 1 },
 { 71, 3 },
 { 72, 3 },
 { 74, 1 },
 { 75, 1 },
 { 76, 7 },
 { 78, 1 },
 { 81, 1 },
 { 82, 3 },
 { 83, 2 },
 { 85, 7 },
 { 86, 3 },
 { 87, 3 },
 { 88, 1 },
 { 89, 3 },
 { 90, 2 },
 { 91, 2 },
 { 92, 1 },
 { 93, 3 },
 { 95, 3 },
 { 97, 2 },
 { 98, 3 },
 { 99, 1 },
 { 101, 3 },
 { 102, 1 },
 { 104, 1 },
 { 105, 7 },
 { 106, 1 },
 { 108, 1 },
 { 109, 1 },
 { 111, 1 },
 { 112, 1 },
 { 113, 3 },
 { 115, 1 },
 { 116, 2 },
 { 118, 1 },
 { 121, 7 },
 { 122, 1 },
 { 123, 1 },
 { 124, 1 },
 { 125, 1 },
 { 126, 3 },
 { 127, 1 },
 { 129, 2 },
 { 130, 1 },
 { 131, 1 },
 { 132, 2 },
 { 133, 1 },
 { 135, 1 },
 { 138, 1 },
 { 141, 3 },
 { 144, 3 },
 { 146, 1 },
 { 148, 3 },
 { 149, 2 },
 { 150, 3 },
 { 151, 2 },
 { 153, 3 },
 { 156, 1 },
 { 158, 3 },
 { 159, 3 },
 { 160, 1 },
 { 161, 3 },
 { 162, 3 },
 { 163, 2 },
 { 164, 2 },
 { 165, 3 },
 { 166, 1 },
 { 167, 3 },
 { 170, 3 },
 { 171, 3 },
 { 0, 0 }
 };

ipair DTtag::TaskSingleSet1[] = {
 { 2, 1 },
 { 3, 1 },
 { 4, 62 },
 { 5, 65572 },
 { 6, 63 },
 { 7, 84 },
 { 8, 110 },
 { 9, 65582 },
 { 10, 65583 },
 { 11, 107 },
 { 12, 84 },
 { 13, 21 },
 { 14, 15 },
 { 15, 18 },
 { 16, 17 },
 { 17, 134 },
 { 18, 134 },
 { 19, 21 },
 { 20, 15 },
 { 21, 18 },
 { 22, 65556 },
 { 23, 65557 },
 { 24, 65572 },
 { 25, 65561 },
 { 26, 65558 },
 { 27, 154 },
 { 28, 107 },
 { 29, 2 },
 { 30, 6 },
 { 31, 32 },
 { 32, 110 },
 { 33, 32 },
 { 34, 35 },
 { 35, 110 },
 { 36, 65559 },
 { 37, 65559 },
 { 38, 43 },
 { 39, 44 },
 { 40, 65595 },
 { 41, 65560 },
 { 42, 65561 },
 { 43, 42 },
 { 44, 42 },
 { 45, 65562 },
 { 46, 43 },
 { 47, 44 },
 { 48, 120 },
 { 49, 2 },
 { 50, 107 },
 { 51, 65563 },
 { 52, 65564 },
 { 53, 65582 },
 { 54, 65565 },
 { 55, 65566 },
 { 56, 65567 },
 { 57, 65568 },
 { 58, 65568 },
 { 59, 65568 },
 { 60, 65568 },
 { 61, 65582 },
 { 62, 65569 },
 { 63, 65563 },
 { 64, 68 },
 { 65, 68 },
 { 66, 68 },
 { 67, 68 },
 { 68, 65570 },
 { 69, 65562 },
 { 70, 65557 },
 { 71, 65571 },
 { 72, 65572 },
 { 73, 84 },
 { 74, 65595 },
 { 75, 65573 },
 { 76, 65561 },
 { 77, 65574 },
 { 78, 79 },
 { 79, 65575 },
 { 80, 65576 },
 { 81, 77 },
 { 82, 65577 },
 { 83, 65582 },
 { 84, 110 },
 { 85, 65561 },
 { 86, 65572 },
 { 87, 65582 },
 { 88, 157 },
 { 89, 65578 },
 { 90, 65579 },
 { 91, 65561 },
 { 92, 65595 },
 { 93, 65580 },
 { 94, 84 },
 { 95, 65581 },
 { 96, 90 },
 { 97, 65561 },
 { 98, 65580 },
 { 99, 103 },
 { 100, 110 },
 { 101, 65582 },
 { 102, 107 },
 { 103, 97 },
 { 104, 2 },
 { 105, 65561 },
 { 106, 2 },
 { 107, 84 },
 { 108, 107 },
 { 109, 107 },
 { 110, 1 },
 { 111, 103 },
 { 112, 110 },
 { 113, 65583 },
 { 114, 1 },
 { 115, 65584 },
 { 116, 65562 },
 { 117, 110 },
 { 118, 2 },
 { 119, 35 },
 { 120, 65585 },
 { 121, 65561 },
 { 122, 157 },
 { 123, 65595 },
 { 124, 107 },
 { 125, 6 },
 { 126, 162 },
 { 127, 65586 },
 { 128, 65587 },
 { 129, 65561 },
 { 130, 129 },
 { 131, 110 },
 { 132, 65583 },
 { 133, 65588 },
 { 134, 65589 },
 { 135, 65557 },
 { 136, 2 },
 { 137, 2 },
 { 138, 65557 },
 { 139, 128 },
 { 140, 128 },
 { 141, 65582 },
 { 142, 143 },
 { 143, 134 },
 { 144, 65582 },
 { 145, 149 },
 { 146, 65568 },
 { 147, 65568 },
 { 148, 65558 },
 { 149, 65563 },
 { 150, 65582 },
 { 151, 65590 },
 { 152, 154 },
 { 153, 65591 },
 { 154, 151 },
 { 155, 154 },
 { 156, 65592 },
 { 157, 1 },
 { 158, 65593 },
 { 159, 65582 },
 { 160, 65557 },
 { 161, 65582 },
 { 162, 65594 },
 { 163, 65561 },
 { 164, 65566 },
 { 165, 65582 },
 { 166, 65595 },
 { 167, 65572 },
 { 168, 169 },
 { 169, 107 },
 { 170, 65572 },
 { 171, 65596 },
 { 0, 0 }
 };

long DTtag::Part1TaskPar1[] = { 
56, 58, 59, 60, 145, 149, 0 };

long DTtag::Part2TaskPar1[] = { 
9, 22, 25, 26, 36, 61, 82, 89, 
95, 101, 105, 113, 121, 127, 141, 144, 
156, 159, 161, 171, 0 };

long DTtag::Part3TaskPar1[] = { 
3, 29, 38, 39, 41, 45, 46, 47, 
48, 49, 52, 54, 71, 75, 76, 78, 
106, 118, 130, 133, 0 };

long DTtag::Part4TaskPar1[] = { 
13, 14, 16, 19, 20, 23, 57, 70, 
99, 104, 111, 135, 138, 146, 160, 0 };

long DTtag::Part5TaskPar1[] = { 
4, 8, 11, 12, 31, 53, 64, 65, 
66, 67, 68, 73, 85, 87, 108, 112, 
131, 150, 162, 165, 0 };

long DTtag::Part6TaskPar1[] = { 
28, 30, 40, 50, 81, 88, 92, 102, 
109, 115, 122, 123, 124, 166, 0 };

long DTtag::Part7TaskPar1[] = { 
3, 29, 38, 39, 41, 45, 46, 47, 
48, 49, 54, 71, 75, 76, 78, 106, 
118, 130, 133, 0 };

long DTtag::Part8TaskPar1[] = { 
82, 89, 95, 101, 127, 156, 171, 0 };

long DTtag::Part9TaskPar1[] = { 
9, 61, 105, 113, 141, 144, 159, 161, 
0 };

long DTtag::Part10TaskPar1[] = { 
4, 8, 11, 12, 31, 53, 64, 65, 
66, 67, 68, 85, 87, 108, 112, 131, 
150, 162, 165, 0 };

long DTtag::Part11TaskPar1[] = { 
13, 14, 16, 19, 20, 23, 51, 57, 
70, 84, 99, 104, 110, 111, 135, 138, 
160, 0 };

long DTtag::Part12TaskPar1[] = { 
3, 16, 29, 36, 38, 45, 48, 49, 
51, 54, 70, 71, 75, 104, 106, 118, 
133, 135, 138, 160, 0 };

long DTtag::Part13TaskPar1[] = { 
13, 14, 19, 20, 99, 0 };

long DTtag::Part14TaskPar1[] = { 
23, 41, 46, 78, 111, 130, 0 };

long DTtag::Part15TaskPar1[] = { 
3, 25, 26, 29, 38, 45, 48, 49, 
54, 71, 75, 76, 82, 89, 95, 101, 
106, 118, 121, 133, 0 };

long DTtag::Part16TaskPar1[] = { 
16, 70, 104, 135, 138, 146, 160, 0 };

long DTtag::Part17TaskPar1[] = { 
3, 29, 38, 41, 45, 48, 49, 51, 
54, 70, 71, 75, 78, 89, 104, 106, 
118, 133, 138, 160, 0 };

long DTtag::Part18TaskPar1[] = { 
3, 16, 29, 38, 48, 49, 70, 71, 
75, 82, 95, 104, 106, 118, 133, 135, 
138, 160, 0 };

long DTtag::Part19TaskPar1[] = { 
23, 25, 76, 89, 101, 105, 121, 130, 
0 };

long DTtag::Part20TaskPar1[] = { 
3, 16, 29, 38, 45, 49, 70, 71, 
75, 82, 89, 95, 99, 104, 106, 118, 
135, 138, 160, 0 };

long DTtag::citeTaskPar1[] = { 
65544, 65548, 65549, 65550, 65551, 0 };

long DTtag::cmdTaskPar1[] = { 
134, 142, 0 };

long DTtag::codephTaskPar1[] = { 
148, 65537, 65538, 65539, 0 };

long DTtag::dataTaskPar1[] = { 
37, 65537, 65541, 65542, 65545, 65546, 0 };

long DTtag::descTaskPar1[] = { 
51, 77, 79, 90, 151, 171, 0 };

long DTtag::dlTaskPar1[] = { 
16, 82, 95, 135, 65548, 65552, 0 };

long DTtag::draftcommentTaskPar1[] = { 
65548, 65553, 65554, 0 };

long DTtag::figTaskPar1[] = { 
45, 54, 89, 65548, 65553, 0 };

long DTtag::figgroupTaskPar1[] = { 
51, 52, 0 };

long DTtag::fnTaskPar1[] = { 
51, 52, 65548, 65553, 65554, 0 };

long DTtag::foreignTaskPar1[] = { 
90, 65537, 65541, 65542, 65545, 65546, 0 };

long DTtag::fragmentTaskPar1[] = { 
145, 149, 0 };

long DTtag::groupseqTaskPar1[] = { 
65536, 0 };

long DTtag::imageTaskPar1[] = { 
39, 47, 63, 162, 65543, 65547, 65548, 65549, 
0 };

long DTtag::indextermTaskPar1[] = { 
64, 65, 66, 68, 73, 65548, 65553, 65554, 
0 };

long DTtag::itemgroupTaskPar1[] = { 
16, 38, 75, 99, 0 };

long DTtag::keywordTaskPar1[] = { 
65537, 65538, 65539, 65540, 65541, 0 };

long DTtag::liTaskPar1[] = { 
91, 163, 0 };

long DTtag::linkTaskPar1[] = { 
79, 80, 114, 0 };

long DTtag::linklistTaskPar1[] = { 
79, 114, 0 };

long DTtag::linkpoolTaskPar1[] = { 
80, 114, 0 };

long DTtag::lqTaskPar1[] = { 
16, 95, 135, 65548, 65552, 0 };

long DTtag::noteTaskPar1[] = { 
41, 45, 51, 54, 78, 65548, 65553, 0 };

long DTtag::objectTaskPar1[] = { 
82, 89, 95, 65547, 65548, 0 };

long DTtag::optionTaskPar1[] = { 
148, 65537, 65538, 65539, 65540, 65541, 0 };

long DTtag::pTaskPar1[] = { 
16, 82, 135, 65548, 65552, 0 };

long DTtag::phTaskPar1[] = { 
65537, 65538, 65539, 0 };

long DTtag::qTaskPar1[] = { 
22, 25, 26, 121, 65539, 65542, 65543, 65544, 
0 };

long DTtag::repsepTaskPar1[] = { 
58, 59, 60, 0 };

long DTtag::rowTaskPar1[] = { 
152, 155, 0 };

long DTtag::shortdescTaskPar1[] = { 
3, 1, 0 };

long DTtag::simpletableTaskPar1[] = { 
51, 65555, 0 };

long DTtag::stentryTaskPar1[] = { 
139, 140, 0 };

long DTtag::stepTaskPar1[] = { 
136, 137, 0 };

long DTtag::tableTaskPar1[] = { 
65555, 0 };

long DTtag::termTaskPar1[] = { 
65537, 65539, 65541, 65542, 65545, 0 };

long DTtag::titleTaskPar1[] = { 
36, 49, 51, 52, 79, 151, 1, 65536, 
0 };

long DTtag::tmTaskPar1[] = { 
22, 72, 153, 158, 65539, 65542, 65543, 65544, 
0 };

long DTtag::uicontrolTaskPar1[] = { 
83, 65537, 65538, 65539, 0 };

long DTtag::varTaskPar1[] = { 
58, 59, 60, 148, 0 };

long DTtag::xrefTaskPar1[] = { 
6, 51, 52, 65544, 65548, 65549, 65550, 65551, 
0 };

long DTtag::titlefirstTaskPar1[] = { 
51, 52, 79, 151, 1, 65536, 0 };

vpair DTtag::TaskElemSetsSet1[] = {
 { 65536, Part1TaskPar1 },
 { 65537, Part2TaskPar1 },
 { 65538, Part3TaskPar1 },
 { 65539, Part4TaskPar1 },
 { 65540, Part5TaskPar1 },
 { 65541, Part6TaskPar1 },
 { 65542, Part7TaskPar1 },
 { 65543, Part8TaskPar1 },
 { 65544, Part9TaskPar1 },
 { 65545, Part10TaskPar1 },
 { 65546, Part11TaskPar1 },
 { 65547, Part12TaskPar1 },
 { 65548, Part13TaskPar1 },
 { 65549, Part14TaskPar1 },
 { 65550, Part15TaskPar1 },
 { 65551, Part16TaskPar1 },
 { 65552, Part17TaskPar1 },
 { 65553, Part18TaskPar1 },
 { 65554, Part19TaskPar1 },
 { 65555, Part20TaskPar1 },
 { 65556, citeTaskPar1 },
 { 65557, cmdTaskPar1 },
 { 65558, codephTaskPar1 },
 { 65559, dataTaskPar1 },
 { 65560, descTaskPar1 },
 { 65561, dlTaskPar1 },
 { 65562, draftcommentTaskPar1 },
 { 65563, figTaskPar1 },
 { 65564, figgroupTaskPar1 },
 { 65565, fnTaskPar1 },
 { 65566, foreignTaskPar1 },
 { 65567, fragmentTaskPar1 },
 { 65568, groupseqTaskPar1 },
 { 65569, imageTaskPar1 },
 { 65570, indextermTaskPar1 },
 { 65571, itemgroupTaskPar1 },
 { 65572, keywordTaskPar1 },
 { 65573, liTaskPar1 },
 { 65574, linkTaskPar1 },
 { 65575, linklistTaskPar1 },
 { 65576, linkpoolTaskPar1 },
 { 65577, lqTaskPar1 },
 { 65578, noteTaskPar1 },
 { 65579, objectTaskPar1 },
 { 65580, optionTaskPar1 },
 { 65581, pTaskPar1 },
 { 65582, phTaskPar1 },
 { 65583, qTaskPar1 },
 { 65584, repsepTaskPar1 },
 { 65585, rowTaskPar1 },
 { 65586, shortdescTaskPar1 },
 { 65587, simpletableTaskPar1 },
 { 65588, stentryTaskPar1 },
 { 65589, stepTaskPar1 },
 { 65590, tableTaskPar1 },
 { 65591, termTaskPar1 },
 { 65592, titleTaskPar1 },
 { 65593, tmTaskPar1 },
 { 65594, uicontrolTaskPar1 },
 { 65595, varTaskPar1 },
 { 65596, xrefTaskPar1 },
 { 65597, titlefirstTaskPar1 },
 { 0, NULL }
 };

npair DTtag::TaskElemSetNamesSet1[] = {
 { 65536, "*Part1"},
 { 65537, "*Part2"},
 { 65538, "*Part3"},
 { 65539, "*Part4"},
 { 65540, "*Part5"},
 { 65541, "*Part6"},
 { 65542, "*Part7"},
 { 65543, "*Part8"},
 { 65544, "*Part9"},
 { 65545, "*Part10"},
 { 65546, "*Part11"},
 { 65547, "*Part12"},
 { 65548, "*Part13"},
 { 65549, "*Part14"},
 { 65550, "*Part15"},
 { 65551, "*Part16"},
 { 65552, "*Part17"},
 { 65553, "*Part18"},
 { 65554, "*Part19"},
 { 65555, "*Part20"},
 { 65556, "*cite"},
 { 65557, "*cmd"},
 { 65558, "*codeph"},
 { 65559, "*data"},
 { 65560, "*desc"},
 { 65561, "*dl"},
 { 65562, "*draft-comment"},
 { 65563, "*fig"},
 { 65564, "*figgroup"},
 { 65565, "*fn"},
 { 65566, "*foreign"},
 { 65567, "*fragment"},
 { 65568, "*groupseq"},
 { 65569, "*image"},
 { 65570, "*indexterm"},
 { 65571, "*itemgroup"},
 { 65572, "*keyword"},
 { 65573, "*li"},
 { 65574, "*link"},
 { 65575, "*linklist"},
 { 65576, "*linkpool"},
 { 65577, "*lq"},
 { 65578, "*note"},
 { 65579, "*object"},
 { 65580, "*option"},
 { 65581, "*p"},
 { 65582, "*ph"},
 { 65583, "*q"},
 { 65584, "*repsep"},
 { 65585, "*row"},
 { 65586, "*shortdesc"},
 { 65587, "*simpletable"},
 { 65588, "*stentry"},
 { 65589, "*step"},
 { 65590, "*table"},
 { 65591, "*term"},
 { 65592, "*title"},
 { 65593, "*tm"},
 { 65594, "*uicontrol"},
 { 65595, "*var"},
 { 65596, "*xref"},
 { 65597, "*titlefirst"},
 { 0, NULL }
 };

ipair DTtag::TaskLevelsSet1[] = {
 { 1, 1 },
 { 2, 1 },
 { 3, 1 },
 { 110, 1 },
 { 114, 1 },
 { 157, 1 },
 { 0, 0 }
 };

ipair DTtag::TaskAtFirstSet1[] = {
 { 15, -1 },
 { 19, -1 },
 { 20, -1 },
 { 23, -1 },
 { 34, -1 },
 { 36, 37 },
 { 41, 90 },
 { 44, -1 },
 { 47, -1 },
 { 62, 63 },
 { 81, -1 },
 { 88, -1 },
 { 106, -1 },
 { 108, -1 },
 { 125, -1 },
 { 139, -1 },
 { 156, 65597 },
 { 0, 0 }
 };


// reference

npair DTtag::ReferenceNamesSet1[] = {
 { 1, "reference"},
 { 2, "refbody"},
 { 3, "abstract"},
 { 4, "alt"},
 { 5, "apiname"},
 { 6, "area"},
 { 7, "audience"},
 { 8, "author"},
 { 9, "b"},
 { 10, "boolean"},
 { 11, "brand"},
 { 12, "category"},
 { 13, "cite"},
 { 14, "cmdname"},
 { 15, "codeblock"},
 { 16, "codeph"},
 { 17, "colspec"},
 { 18, "component"},
 { 19, "coords"},
 { 20, "copyrholder"},
 { 21, "copyright"},
 { 22, "copyryear"},
 { 23, "created"},
 { 24, "critdates"},
 { 25, "data"},
 { 26, "data-about"},
 { 27, "dd"},
 { 28, "ddhd"},
 { 29, "delim"},
 { 30, "desc"},
 { 31, "dl"},
 { 32, "dlentry"},
 { 33, "dlhead"},
 { 34, "draft-comment"},
 { 35, "dt"},
 { 36, "dthd"},
 { 37, "entry"},
 { 38, "example"},
 { 39, "featnum"},
 { 40, "fig"},
 { 41, "figgroup"},
 { 42, "filepath"},
 { 43, "fn"},
 { 44, "foreign"},
 { 45, "fragment"},
 { 46, "fragref"},
 { 47, "groupchoice"},
 { 48, "groupcomp"},
 { 49, "groupseq"},
 { 50, "i"},
 { 51, "image"},
 { 52, "imagemap"},
 { 53, "index-base"},
 { 54, "index-see"},
 { 55, "index-see-also"},
 { 56, "index-sort-as"},
 { 57, "indexterm"},
 { 58, "indextermref"},
 { 59, "itemgroup"},
 { 60, "keyword"},
 { 61, "keywords"},
 { 62, "kwd"},
 { 63, "li"},
 { 64, "lines"},
 { 65, "link"},
 { 66, "linkinfo"},
 { 67, "linklist"},
 { 68, "linkpool"},
 { 69, "linktext"},
 { 70, "lq"},
 { 71, "menucascade"},
 { 72, "metadata"},
 { 73, "msgblock"},
 { 74, "msgnum"},
 { 75, "msgph"},
 { 76, "navtitle"},
 { 77, "note"},
 { 78, "object"},
 { 79, "ol"},
 { 80, "oper"},
 { 81, "option"},
 { 82, "othermeta"},
 { 83, "p"},
 { 84, "param"},
 { 85, "parml"},
 { 86, "parmname"},
 { 87, "pd"},
 { 88, "permissions"},
 { 89, "ph"},
 { 90, "platform"},
 { 91, "plentry"},
 { 92, "pre"},
 { 93, "prodinfo"},
 { 94, "prodname"},
 { 95, "prognum"},
 { 96, "prolog"},
 { 97, "propdesc"},
 { 98, "propdeschd"},
 { 99, "properties"},
 { 100, "property"},
 { 101, "prophead"},
 { 102, "proptype"},
 { 103, "proptypehd"},
 { 104, "propvalue"},
 { 105, "propvaluehd"},
 { 106, "pt"},
 { 107, "publisher"},
 { 108, "q"},
 { 109, "refsyn"},
 { 110, "related-links"},
 { 111, "repsep"},
 { 112, "required-cleanup"},
 { 113, "resourceid"},
 { 114, "revised"},
 { 115, "row"},
 { 116, "screen"},
 { 117, "searchtitle"},
 { 118, "section"},
 { 119, "sep"},
 { 120, "series"},
 { 121, "shape"},
 { 122, "shortcut"},
 { 123, "shortdesc"},
 { 124, "simpletable"},
 { 125, "sl"},
 { 126, "sli"},
 { 127, "source"},
 { 128, "state"},
 { 129, "stentry"},
 { 130, "sthead"},
 { 131, "strow"},
 { 132, "sub"},
 { 133, "sup"},
 { 134, "synblk"},
 { 135, "synnote"},
 { 136, "synnoteref"},
 { 137, "synph"},
 { 138, "syntaxdiagram"},
 { 139, "systemoutput"},
 { 140, "table"},
 { 141, "tbody"},
 { 142, "term"},
 { 143, "tgroup"},
 { 144, "thead"},
 { 145, "title"},
 { 146, "titlealts"},
 { 147, "tm"},
 { 148, "tt"},
 { 149, "u"},
 { 150, "uicontrol"},
 { 151, "ul"},
 { 152, "unknown"},
 { 153, "userinput"},
 { 154, "var"},
 { 155, "varname"},
 { 156, "vrm"},
 { 157, "vrmlist"},
 { 158, "wintitle"},
 { 159, "xref"},
 { 0, NULL }
 };

ipair DTtag::ReferenceInlineSet1[] = {
 { 3, 1 },
 { 4, 1 },
 { 5, 3 },
 { 8, 1 },
 { 9, 3 },
 { 10, 2 },
 { 11, 1 },
 { 12, 1 },
 { 13, 3 },
 { 14, 3 },
 { 15, 7 },
 { 16, 3 },
 { 18, 1 },
 { 19, 1 },
 { 20, 1 },
 { 25, 3 },
 { 26, 2 },
 { 27, 1 },
 { 28, 1 },
 { 29, 1 },
 { 30, 1 },
 { 31, 2 },
 { 34, 3 },
 { 35, 1 },
 { 36, 1 },
 { 37, 1 },
 { 38, 1 },
 { 39, 1 },
 { 40, 2 },
 { 42, 3 },
 { 43, 3 },
 { 44, 2 },
 { 46, 1 },
 { 50, 3 },
 { 51, 2 },
 { 52, 2 },
 { 53, 3 },
 { 54, 3 },
 { 55, 3 },
 { 56, 3 },
 { 57, 3 },
 { 58, 2 },
 { 59, 3 },
 { 60, 3 },
 { 62, 1 },
 { 63, 1 },
 { 64, 7 },
 { 66, 1 },
 { 69, 1 },
 { 70, 1 },
 { 71, 2 },
 { 73, 7 },
 { 74, 3 },
 { 75, 3 },
 { 76, 1 },
 { 77, 1 },
 { 79, 2 },
 { 80, 1 },
 { 81, 3 },
 { 83, 1 },
 { 85, 2 },
 { 86, 3 },
 { 87, 1 },
 { 89, 3 },
 { 90, 1 },
 { 92, 7 },
 { 94, 1 },
 { 95, 1 },
 { 97, 1 },
 { 98, 1 },
 { 102, 1 },
 { 103, 1 },
 { 104, 1 },
 { 105, 1 },
 { 106, 1 },
 { 107, 1 },
 { 108, 3 },
 { 109, 1 },
 { 111, 1 },
 { 112, 2 },
 { 116, 7 },
 { 117, 1 },
 { 118, 1 },
 { 119, 1 },
 { 120, 1 },
 { 121, 1 },
 { 122, 3 },
 { 123, 1 },
 { 125, 2 },
 { 126, 1 },
 { 127, 1 },
 { 128, 2 },
 { 129, 1 },
 { 132, 3 },
 { 133, 3 },
 { 135, 1 },
 { 137, 3 },
 { 138, 2 },
 { 139, 3 },
 { 142, 3 },
 { 145, 1 },
 { 147, 3 },
 { 148, 3 },
 { 149, 3 },
 { 150, 3 },
 { 151, 2 },
 { 152, 2 },
 { 153, 3 },
 { 154, 1 },
 { 155, 3 },
 { 158, 3 },
 { 159, 3 },
 { 0, 0 }
 };

ipair DTtag::ReferenceSingleSet1[] = {
 { 2, 1 },
 { 3, 1 },
 { 4, 51 },
 { 5, 65571 },
 { 6, 52 },
 { 7, 72 },
 { 8, 96 },
 { 9, 65581 },
 { 10, 65582 },
 { 11, 93 },
 { 12, 72 },
 { 13, 65556 },
 { 14, 65571 },
 { 15, 65560 },
 { 16, 65557 },
 { 17, 143 },
 { 18, 93 },
 { 19, 6 },
 { 20, 21 },
 { 21, 96 },
 { 22, 21 },
 { 23, 24 },
 { 24, 96 },
 { 25, 65558 },
 { 26, 65558 },
 { 27, 32 },
 { 28, 33 },
 { 29, 65593 },
 { 30, 65559 },
 { 31, 65560 },
 { 32, 31 },
 { 33, 31 },
 { 34, 65561 },
 { 35, 32 },
 { 36, 33 },
 { 37, 115 },
 { 38, 2 },
 { 39, 93 },
 { 40, 65562 },
 { 41, 65563 },
 { 42, 65581 },
 { 43, 65564 },
 { 44, 65565 },
 { 45, 65566 },
 { 46, 65567 },
 { 47, 65567 },
 { 48, 65567 },
 { 49, 65567 },
 { 50, 65581 },
 { 51, 65568 },
 { 52, 65562 },
 { 53, 57 },
 { 54, 57 },
 { 55, 57 },
 { 56, 57 },
 { 57, 65569 },
 { 58, 65561 },
 { 59, 65570 },
 { 60, 65571 },
 { 61, 72 },
 { 62, 65593 },
 { 63, 65572 },
 { 64, 65560 },
 { 65, 65573 },
 { 66, 67 },
 { 67, 65574 },
 { 68, 65575 },
 { 69, 65 },
 { 70, 65576 },
 { 71, 65581 },
 { 72, 96 },
 { 73, 65560 },
 { 74, 65571 },
 { 75, 65581 },
 { 76, 146 },
 { 77, 65577 },
 { 78, 65578 },
 { 79, 65560 },
 { 80, 65593 },
 { 81, 65579 },
 { 82, 72 },
 { 83, 65580 },
 { 84, 78 },
 { 85, 65560 },
 { 86, 65579 },
 { 87, 91 },
 { 88, 96 },
 { 89, 65581 },
 { 90, 93 },
 { 91, 85 },
 { 92, 65560 },
 { 93, 72 },
 { 94, 93 },
 { 95, 93 },
 { 96, 1 },
 { 97, 100 },
 { 98, 101 },
 { 99, 2 },
 { 100, 99 },
 { 101, 99 },
 { 102, 100 },
 { 103, 101 },
 { 104, 100 },
 { 105, 101 },
 { 106, 91 },
 { 107, 96 },
 { 108, 65582 },
 { 109, 2 },
 { 110, 1 },
 { 111, 65583 },
 { 112, 65561 },
 { 113, 96 },
 { 114, 24 },
 { 115, 65584 },
 { 116, 65560 },
 { 117, 146 },
 { 118, 2 },
 { 119, 65593 },
 { 120, 93 },
 { 121, 6 },
 { 122, 150 },
 { 123, 65585 },
 { 124, 65586 },
 { 125, 65560 },
 { 126, 125 },
 { 127, 96 },
 { 128, 65582 },
 { 129, 65587 },
 { 130, 124 },
 { 131, 124 },
 { 132, 65581 },
 { 133, 65581 },
 { 134, 138 },
 { 135, 65567 },
 { 136, 65567 },
 { 137, 65557 },
 { 138, 65562 },
 { 139, 65581 },
 { 140, 65588 },
 { 141, 143 },
 { 142, 65589 },
 { 143, 140 },
 { 144, 143 },
 { 145, 65590 },
 { 146, 1 },
 { 147, 65591 },
 { 148, 65581 },
 { 149, 65581 },
 { 150, 65592 },
 { 151, 65560 },
 { 152, 65565 },
 { 153, 65581 },
 { 154, 65593 },
 { 155, 65571 },
 { 156, 157 },
 { 157, 93 },
 { 158, 65571 },
 { 159, 65594 },
 { 0, 0 }
 };

long DTtag::Part1ReferencePar1[] = { 
25, 38, 41, 109, 118, 0 };

long DTtag::Part2ReferencePar1[] = { 
45, 47, 48, 49, 134, 138, 0 };

long DTtag::Part3ReferencePar1[] = { 
9, 13, 15, 16, 50, 64, 70, 77, 
83, 89, 92, 108, 116, 123, 132, 133, 
145, 148, 149, 159, 0 };

long DTtag::Part4ReferencePar1[] = { 
3, 27, 28, 30, 34, 35, 36, 37, 
43, 59, 63, 66, 97, 98, 102, 103, 
104, 105, 126, 129, 0 };

long DTtag::Part5ReferencePar1[] = { 
46, 87, 106, 135, 137, 0 };

long DTtag::Part6ReferencePar1[] = { 
4, 8, 11, 12, 20, 42, 53, 54, 
55, 56, 57, 61, 73, 75, 94, 107, 
127, 139, 150, 153, 0 };

long DTtag::Part7ReferencePar1[] = { 
18, 19, 29, 39, 69, 76, 80, 90, 
95, 111, 117, 119, 120, 154, 0 };

long DTtag::Part8ReferencePar1[] = { 
38, 46, 87, 106, 109, 118, 135, 0 };

long DTtag::Part9ReferencePar1[] = { 
70, 77, 83, 89, 123, 145, 159, 0 };

long DTtag::Part10ReferencePar1[] = { 
9, 50, 64, 92, 108, 132, 133, 148, 
149, 0 };

long DTtag::Part11ReferencePar1[] = { 
4, 8, 11, 12, 20, 25, 42, 53, 
54, 55, 56, 57, 73, 75, 94, 107, 
127, 139, 150, 153, 0 };

long DTtag::Part12ReferencePar1[] = { 
38, 40, 46, 72, 87, 96, 106, 2, 
109, 118, 0 };

long DTtag::Part13ReferencePar1[] = { 
25, 38, 40, 87, 109, 118, 0 };

long DTtag::Part14ReferencePar1[] = { 
3, 27, 34, 37, 43, 59, 63, 70, 
77, 83, 98, 103, 105, 129, 0 };

long DTtag::Part15ReferencePar1[] = { 
15, 16, 30, 35, 38, 66, 87, 89, 
97, 102, 104, 106, 109, 116, 118, 126, 
135, 0 };

long DTtag::Part16ReferencePar1[] = { 
3, 27, 30, 34, 37, 38, 40, 43, 
59, 63, 66, 77, 87, 97, 98, 103, 
105, 109, 118, 129, 0 };

long DTtag::Part17ReferencePar1[] = { 
30, 38, 40, 66, 87, 97, 109, 118, 
0 };

long DTtag::Part18ReferencePar1[] = { 
3, 27, 37, 59, 63, 70, 83, 98, 
103, 105, 129, 0 };

long DTtag::Part19ReferencePar1[] = { 
15, 38, 64, 77, 87, 89, 92, 102, 
104, 109, 116, 118, 126, 0 };

long DTtag::Part20ReferencePar1[] = { 
3, 27, 34, 38, 59, 63, 70, 77, 
83, 87, 2, 109, 118, 0 };

long DTtag::citeReferencePar1[] = { 
65545, 65549, 65550, 0 };

long DTtag::codephReferencePar1[] = { 
65536, 65538, 65539, 65540, 0 };

long DTtag::dataReferencePar1[] = { 
26, 65538, 65539, 65542, 65546, 65547, 0 };

long DTtag::descReferencePar1[] = { 
40, 65, 67, 78, 140, 159, 0 };

long DTtag::dlReferencePar1[] = { 
65549, 65552, 0 };

long DTtag::draftcommentReferencePar1[] = { 
65553, 65554, 0 };

long DTtag::figReferencePar1[] = { 
38, 87, 109, 118, 65549, 0 };

long DTtag::figgroupReferencePar1[] = { 
40, 41, 0 };

long DTtag::fnReferencePar1[] = { 
40, 41, 65553, 65554, 0 };

long DTtag::foreignReferencePar1[] = { 
78, 65538, 65539, 65542, 65546, 65547, 0 };

long DTtag::fragmentReferencePar1[] = { 
134, 138, 0 };

long DTtag::groupseqReferencePar1[] = { 
65537, 0 };

long DTtag::imageReferencePar1[] = { 
52, 106, 150, 65539, 65544, 65548, 0 };

long DTtag::indextermReferencePar1[] = { 
53, 54, 55, 57, 61, 65553, 65554, 0 };

long DTtag::itemgroupReferencePar1[] = { 
27, 63, 87, 0 };

long DTtag::keywordReferencePar1[] = { 
46, 87, 106, 135, 65536, 65538, 65539, 65541, 
65542, 0 };

long DTtag::liReferencePar1[] = { 
79, 151, 0 };

long DTtag::linkReferencePar1[] = { 
67, 68, 110, 0 };

long DTtag::linklistReferencePar1[] = { 
67, 110, 0 };

long DTtag::linkpoolReferencePar1[] = { 
68, 110, 0 };

long DTtag::lqReferencePar1[] = { 
83, 65551, 0 };

long DTtag::noteReferencePar1[] = { 
34, 43, 65552, 65553, 0 };

long DTtag::objectReferencePar1[] = { 
65548, 65549, 0 };

long DTtag::optionReferencePar1[] = { 
65536, 65538, 65539, 65540, 65541, 65542, 0 };

long DTtag::pReferencePar1[] = { 
70, 65551, 0 };

long DTtag::phReferencePar1[] = { 
46, 87, 106, 135, 65536, 65538, 65539, 0 };

long DTtag::qReferencePar1[] = { 
65538, 65539, 65543, 0 };

long DTtag::repsepReferencePar1[] = { 
47, 48, 49, 0 };

long DTtag::rowReferencePar1[] = { 
141, 144, 0 };

long DTtag::shortdescReferencePar1[] = { 
3, 1, 0 };

long DTtag::simpletableReferencePar1[] = { 
40, 65555, 0 };

long DTtag::stentryReferencePar1[] = { 
130, 131, 0 };

long DTtag::tableReferencePar1[] = { 
65555, 0 };

long DTtag::termReferencePar1[] = { 
65538, 65539, 65542, 65543, 65546, 0 };

long DTtag::titleReferencePar1[] = { 
40, 67, 1, 140, 65536, 65537, 0 };

long DTtag::tmReferencePar1[] = { 
13, 60, 142, 147, 65539, 65543, 65544, 65545, 
0 };

long DTtag::uicontrolReferencePar1[] = { 
46, 71, 87, 106, 135, 65536, 65538, 65539, 
0 };

long DTtag::varReferencePar1[] = { 
47, 48, 49, 137, 0 };

long DTtag::xrefReferencePar1[] = { 
6, 40, 41, 65545, 65549, 65550, 0 };

long DTtag::titlefirstReferencePar1[] = { 
40, 41, 67, 1, 140, 65537, 0 };

vpair DTtag::ReferenceElemSetsSet1[] = {
 { 65536, Part1ReferencePar1 },
 { 65537, Part2ReferencePar1 },
 { 65538, Part3ReferencePar1 },
 { 65539, Part4ReferencePar1 },
 { 65540, Part5ReferencePar1 },
 { 65541, Part6ReferencePar1 },
 { 65542, Part7ReferencePar1 },
 { 65543, Part8ReferencePar1 },
 { 65544, Part9ReferencePar1 },
 { 65545, Part10ReferencePar1 },
 { 65546, Part11ReferencePar1 },
 { 65547, Part12ReferencePar1 },
 { 65548, Part13ReferencePar1 },
 { 65549, Part14ReferencePar1 },
 { 65550, Part15ReferencePar1 },
 { 65551, Part16ReferencePar1 },
 { 65552, Part17ReferencePar1 },
 { 65553, Part18ReferencePar1 },
 { 65554, Part19ReferencePar1 },
 { 65555, Part20ReferencePar1 },
 { 65556, citeReferencePar1 },
 { 65557, codephReferencePar1 },
 { 65558, dataReferencePar1 },
 { 65559, descReferencePar1 },
 { 65560, dlReferencePar1 },
 { 65561, draftcommentReferencePar1 },
 { 65562, figReferencePar1 },
 { 65563, figgroupReferencePar1 },
 { 65564, fnReferencePar1 },
 { 65565, foreignReferencePar1 },
 { 65566, fragmentReferencePar1 },
 { 65567, groupseqReferencePar1 },
 { 65568, imageReferencePar1 },
 { 65569, indextermReferencePar1 },
 { 65570, itemgroupReferencePar1 },
 { 65571, keywordReferencePar1 },
 { 65572, liReferencePar1 },
 { 65573, linkReferencePar1 },
 { 65574, linklistReferencePar1 },
 { 65575, linkpoolReferencePar1 },
 { 65576, lqReferencePar1 },
 { 65577, noteReferencePar1 },
 { 65578, objectReferencePar1 },
 { 65579, optionReferencePar1 },
 { 65580, pReferencePar1 },
 { 65581, phReferencePar1 },
 { 65582, qReferencePar1 },
 { 65583, repsepReferencePar1 },
 { 65584, rowReferencePar1 },
 { 65585, shortdescReferencePar1 },
 { 65586, simpletableReferencePar1 },
 { 65587, stentryReferencePar1 },
 { 65588, tableReferencePar1 },
 { 65589, termReferencePar1 },
 { 65590, titleReferencePar1 },
 { 65591, tmReferencePar1 },
 { 65592, uicontrolReferencePar1 },
 { 65593, varReferencePar1 },
 { 65594, xrefReferencePar1 },
 { 65595, titlefirstReferencePar1 },
 { 0, NULL }
 };

npair DTtag::ReferenceElemSetNamesSet1[] = {
 { 65536, "*Part1"},
 { 65537, "*Part2"},
 { 65538, "*Part3"},
 { 65539, "*Part4"},
 { 65540, "*Part5"},
 { 65541, "*Part6"},
 { 65542, "*Part7"},
 { 65543, "*Part8"},
 { 65544, "*Part9"},
 { 65545, "*Part10"},
 { 65546, "*Part11"},
 { 65547, "*Part12"},
 { 65548, "*Part13"},
 { 65549, "*Part14"},
 { 65550, "*Part15"},
 { 65551, "*Part16"},
 { 65552, "*Part17"},
 { 65553, "*Part18"},
 { 65554, "*Part19"},
 { 65555, "*Part20"},
 { 65556, "*cite"},
 { 65557, "*codeph"},
 { 65558, "*data"},
 { 65559, "*desc"},
 { 65560, "*dl"},
 { 65561, "*draft-comment"},
 { 65562, "*fig"},
 { 65563, "*figgroup"},
 { 65564, "*fn"},
 { 65565, "*foreign"},
 { 65566, "*fragment"},
 { 65567, "*groupseq"},
 { 65568, "*image"},
 { 65569, "*indexterm"},
 { 65570, "*itemgroup"},
 { 65571, "*keyword"},
 { 65572, "*li"},
 { 65573, "*link"},
 { 65574, "*linklist"},
 { 65575, "*linkpool"},
 { 65576, "*lq"},
 { 65577, "*note"},
 { 65578, "*object"},
 { 65579, "*option"},
 { 65580, "*p"},
 { 65581, "*ph"},
 { 65582, "*q"},
 { 65583, "*repsep"},
 { 65584, "*row"},
 { 65585, "*shortdesc"},
 { 65586, "*simpletable"},
 { 65587, "*stentry"},
 { 65588, "*table"},
 { 65589, "*term"},
 { 65590, "*title"},
 { 65591, "*tm"},
 { 65592, "*uicontrol"},
 { 65593, "*var"},
 { 65594, "*xref"},
 { 65595, "*titlefirst"},
 { 0, NULL }
 };

ipair DTtag::ReferenceLevelsSet1[] = {
 { 1, 1 },
 { 2, 1 },
 { 3, 1 },
 { 96, 1 },
 { 110, 1 },
 { 146, 1 },
 { 0, 0 }
 };

ipair DTtag::ReferenceAtFirstSet1[] = {
 { 23, -1 },
 { 25, 26 },
 { 30, 78 },
 { 33, -1 },
 { 36, -1 },
 { 51, 52 },
 { 69, -1 },
 { 76, -1 },
 { 94, -1 },
 { 101, -1 },
 { 102, -1 },
 { 103, -1 },
 { 121, -1 },
 { 130, -1 },
 { 145, 65595 },
 { 0, 0 }
 };


// glossary

npair DTtag::GlossentryNamesSet1[] = {
 { 1, "glossentry"},
 { 2, "glossdef"},
 { 3, "alt"},
 { 4, "apiname"},
 { 5, "area"},
 { 6, "b"},
 { 7, "boolean"},
 { 8, "cite"},
 { 9, "cmdname"},
 { 10, "codeblock"},
 { 11, "codeph"},
 { 12, "colspec"},
 { 13, "coords"},
 { 14, "data"},
 { 15, "data-about"},
 { 16, "dd"},
 { 17, "ddhd"},
 { 18, "delim"},
 { 19, "desc"},
 { 20, "dl"},
 { 21, "dlentry"},
 { 22, "dlhead"},
 { 23, "draft-comment"},
 { 24, "dt"},
 { 25, "dthd"},
 { 26, "entry"},
 { 27, "fig"},
 { 28, "figgroup"},
 { 29, "filepath"},
 { 30, "fn"},
 { 31, "foreign"},
 { 32, "fragment"},
 { 33, "fragref"},
 { 34, "glossterm"},
 { 35, "groupchoice"},
 { 36, "groupcomp"},
 { 37, "groupseq"},
 { 38, "i"},
 { 39, "image"},
 { 40, "imagemap"},
 { 41, "index-base"},
 { 42, "index-see"},
 { 43, "index-see-also"},
 { 44, "index-sort-as"},
 { 45, "indexterm"},
 { 46, "indextermref"},
 { 47, "itemgroup"},
 { 48, "keyword"},
 { 49, "kwd"},
 { 50, "li"},
 { 51, "lines"},
 { 52, "link"},
 { 53, "linkinfo"},
 { 54, "linklist"},
 { 55, "linkpool"},
 { 56, "linktext"},
 { 57, "lq"},
 { 58, "menucascade"},
 { 59, "msgblock"},
 { 60, "msgnum"},
 { 61, "msgph"},
 { 62, "no-topic-nesting"},
 { 63, "note"},
 { 64, "object"},
 { 65, "ol"},
 { 66, "oper"},
 { 67, "option"},
 { 68, "p"},
 { 69, "param"},
 { 70, "parml"},
 { 71, "parmname"},
 { 72, "pd"},
 { 73, "ph"},
 { 74, "plentry"},
 { 75, "pre"},
 { 76, "pt"},
 { 77, "q"},
 { 78, "related-links"},
 { 79, "repsep"},
 { 80, "required-cleanup"},
 { 81, "row"},
 { 82, "screen"},
 { 83, "sep"},
 { 84, "shape"},
 { 85, "shortcut"},
 { 86, "shortdesc"},
 { 87, "simpletable"},
 { 88, "sl"},
 { 89, "sli"},
 { 90, "state"},
 { 91, "stentry"},
 { 92, "sthead"},
 { 93, "strow"},
 { 94, "sub"},
 { 95, "sup"},
 { 96, "synblk"},
 { 97, "synnote"},
 { 98, "synnoteref"},
 { 99, "synph"},
 { 100, "syntaxdiagram"},
 { 101, "systemoutput"},
 { 102, "table"},
 { 103, "tbody"},
 { 104, "term"},
 { 105, "tgroup"},
 { 106, "thead"},
 { 107, "title"},
 { 108, "tm"},
 { 109, "tt"},
 { 110, "u"},
 { 111, "uicontrol"},
 { 112, "ul"},
 { 113, "unknown"},
 { 114, "userinput"},
 { 115, "var"},
 { 116, "varname"},
 { 117, "wintitle"},
 { 118, "xref"},
 { 0, NULL }
 };

ipair DTtag::GlossentryInlineSet1[] = {
 { 2, 1 },
 { 3, 1 },
 { 4, 3 },
 { 6, 3 },
 { 7, 2 },
 { 8, 3 },
 { 9, 3 },
 { 10, 5 },
 { 11, 3 },
 { 13, 1 },
 { 14, 3 },
 { 15, 2 },
 { 16, 1 },
 { 17, 1 },
 { 18, 1 },
 { 19, 1 },
 { 23, 3 },
 { 24, 1 },
 { 25, 1 },
 { 26, 1 },
 { 29, 3 },
 { 30, 1 },
 { 31, 2 },
 { 33, 1 },
 { 34, 1 },
 { 38, 3 },
 { 41, 3 },
 { 42, 3 },
 { 43, 3 },
 { 44, 3 },
 { 45, 3 },
 { 46, 2 },
 { 47, 3 },
 { 48, 3 },
 { 49, 1 },
 { 50, 1 },
 { 51, 5 },
 { 53, 1 },
 { 56, 1 },
 { 57, 1 },
 { 58, 2 },
 { 59, 5 },
 { 60, 3 },
 { 61, 3 },
 { 63, 1 },
 { 66, 1 },
 { 67, 3 },
 { 68, 1 },
 { 71, 3 },
 { 72, 1 },
 { 73, 3 },
 { 75, 5 },
 { 76, 1 },
 { 77, 3 },
 { 79, 1 },
 { 82, 5 },
 { 83, 1 },
 { 84, 1 },
 { 85, 3 },
 { 86, 1 },
 { 89, 1 },
 { 90, 2 },
 { 91, 1 },
 { 94, 3 },
 { 95, 3 },
 { 97, 1 },
 { 99, 3 },
 { 101, 3 },
 { 104, 3 },
 { 107, 1 },
 { 108, 3 },
 { 109, 3 },
 { 110, 3 },
 { 111, 3 },
 { 113, 2 },
 { 114, 3 },
 { 115, 1 },
 { 116, 3 },
 { 117, 3 },
 { 118, 3 },
 { 0, 0 }
 };

ipair DTtag::GlossentrySingleSet1[] = {
 { 2, 1 },
 { 3, 39 },
 { 4, 65566 },
 { 5, 40 },
 { 6, 65576 },
 { 7, 65577 },
 { 8, 65551 },
 { 9, 65566 },
 { 10, 65555 },
 { 11, 65552 },
 { 12, 105 },
 { 13, 5 },
 { 14, 65553 },
 { 15, 65553 },
 { 16, 21 },
 { 17, 22 },
 { 18, 65587 },
 { 19, 65554 },
 { 20, 65555 },
 { 21, 20 },
 { 22, 20 },
 { 23, 65556 },
 { 24, 21 },
 { 25, 22 },
 { 26, 81 },
 { 27, 65557 },
 { 28, 65558 },
 { 29, 65576 },
 { 30, 65559 },
 { 31, 65560 },
 { 32, 65561 },
 { 33, 65562 },
 { 34, 1 },
 { 35, 65562 },
 { 36, 65562 },
 { 37, 65562 },
 { 38, 65576 },
 { 39, 65563 },
 { 40, 65557 },
 { 41, 45 },
 { 42, 45 },
 { 43, 45 },
 { 44, 45 },
 { 45, 65564 },
 { 46, 65556 },
 { 47, 65565 },
 { 48, 65566 },
 { 49, 65587 },
 { 50, 65567 },
 { 51, 65555 },
 { 52, 65568 },
 { 53, 54 },
 { 54, 65569 },
 { 55, 65570 },
 { 56, 52 },
 { 57, 65571 },
 { 58, 65576 },
 { 59, 65555 },
 { 60, 65566 },
 { 61, 65576 },
 { 62, 1 },
 { 63, 65572 },
 { 64, 65573 },
 { 65, 65555 },
 { 66, 65587 },
 { 67, 65574 },
 { 68, 65575 },
 { 69, 64 },
 { 70, 65555 },
 { 71, 65574 },
 { 72, 74 },
 { 73, 65576 },
 { 74, 70 },
 { 75, 65555 },
 { 76, 74 },
 { 77, 65577 },
 { 78, 1 },
 { 79, 65578 },
 { 80, 65556 },
 { 81, 65579 },
 { 82, 65555 },
 { 83, 65587 },
 { 84, 5 },
 { 85, 111 },
 { 86, 2 },
 { 87, 65580 },
 { 88, 65555 },
 { 89, 88 },
 { 90, 65577 },
 { 91, 65581 },
 { 92, 87 },
 { 93, 87 },
 { 94, 65576 },
 { 95, 65576 },
 { 96, 100 },
 { 97, 65562 },
 { 98, 65562 },
 { 99, 65552 },
 { 100, 65557 },
 { 101, 65576 },
 { 102, 65582 },
 { 103, 105 },
 { 104, 65583 },
 { 105, 102 },
 { 106, 105 },
 { 107, 65584 },
 { 108, 65585 },
 { 109, 65576 },
 { 110, 65576 },
 { 111, 65586 },
 { 112, 65555 },
 { 113, 65560 },
 { 114, 65576 },
 { 115, 65587 },
 { 116, 65566 },
 { 117, 65566 },
 { 118, 65588 },
 { 0, 0 }
 };

long DTtag::Part1GlossentryPar1[] = { 
6, 8, 10, 11, 14, 38, 57, 63, 
68, 73, 75, 77, 82, 86, 94, 95, 
107, 109, 110, 118, 0 };

long DTtag::Part2GlossentryPar1[] = { 
16, 17, 19, 23, 24, 25, 26, 28, 
30, 33, 2, 34, 47, 50, 51, 53, 
72, 76, 89, 91, 0 };

long DTtag::Part3GlossentryPar1[] = { 
3, 13, 18, 29, 41, 42, 43, 44, 
45, 56, 59, 61, 66, 79, 83, 97, 
101, 111, 114, 115, 0 };

long DTtag::Part4GlossentryPar1[] = { 
16, 17, 19, 23, 24, 25, 26, 30, 
33, 2, 34, 47, 50, 51, 53, 72, 
76, 89, 91, 0 };

long DTtag::Part5GlossentryPar1[] = { 
57, 63, 68, 73, 86, 107, 118, 0 };

long DTtag::Part6GlossentryPar1[] = { 
6, 38, 75, 77, 94, 95, 97, 109, 
110, 0 };

long DTtag::Part7GlossentryPar1[] = { 
3, 13, 18, 27, 29, 41, 42, 43, 
44, 45, 56, 59, 61, 66, 79, 83, 
101, 111, 114, 115, 0 };

long DTtag::Part8GlossentryPar1[] = { 
14, 16, 23, 26, 27, 30, 2, 47, 
50, 72, 91, 0 };

long DTtag::Part9GlossentryPar1[] = { 
19, 24, 53, 76, 89, 0 };

long DTtag::Part10GlossentryPar1[] = { 
10, 11, 16, 23, 26, 30, 2, 47, 
50, 51, 57, 63, 68, 72, 73, 82, 
91, 0 };

long DTtag::Part11GlossentryPar1[] = { 
16, 19, 23, 26, 27, 30, 2, 47, 
50, 53, 63, 72, 91, 0 };

long DTtag::Part12GlossentryPar1[] = { 
16, 26, 2, 47, 50, 57, 68, 72, 
91, 0 };

long DTtag::Part13GlossentryPar1[] = { 
10, 51, 63, 73, 75, 82, 89, 0 };

long DTtag::Part14GlossentryPar1[] = { 
32, 35, 36, 37, 96, 100, 0 };

long DTtag::Part15GlossentryPar1[] = { 
16, 23, 2, 47, 50, 57, 63, 68, 
72, 0 };

long DTtag::citeGlossentryPar1[] = { 
65541, 65544, 65545, 0 };

long DTtag::codephGlossentryPar1[] = { 
97, 99, 65536, 65537, 0 };

long DTtag::dataGlossentryPar1[] = { 
15, 65536, 65539, 65542, 0 };

long DTtag::descGlossentryPar1[] = { 
27, 52, 54, 64, 102, 118, 0 };

long DTtag::dlGlossentryPar1[] = { 
57, 68, 65546, 0 };

long DTtag::draftcommentGlossentryPar1[] = { 
65547, 65548, 0 };

long DTtag::figGlossentryPar1[] = { 
23, 30, 63, 65547, 0 };

long DTtag::figgroupGlossentryPar1[] = { 
27, 28, 0 };

long DTtag::fnGlossentryPar1[] = { 
27, 28, 65547, 65548, 0 };

long DTtag::foreignGlossentryPar1[] = { 
64, 65536, 65539, 65542, 0 };

long DTtag::fragmentGlossentryPar1[] = { 
96, 100, 0 };

long DTtag::groupseqGlossentryPar1[] = { 
65549, 0 };

long DTtag::imageGlossentryPar1[] = { 
17, 25, 34, 40, 111, 65540, 65543, 65544, 
0 };

long DTtag::indextermGlossentryPar1[] = { 
41, 42, 43, 45, 65547, 65548, 0 };

long DTtag::itemgroupGlossentryPar1[] = { 
16, 50, 72, 0 };

long DTtag::keywordGlossentryPar1[] = { 
65536, 65537, 65538, 0 };

long DTtag::liGlossentryPar1[] = { 
65, 112, 0 };

long DTtag::linkGlossentryPar1[] = { 
54, 55, 78, 0 };

long DTtag::linklistGlossentryPar1[] = { 
54, 78, 0 };

long DTtag::linkpoolGlossentryPar1[] = { 
55, 78, 0 };

long DTtag::lqGlossentryPar1[] = { 
68, 65546, 0 };

long DTtag::noteGlossentryPar1[] = { 
19, 23, 27, 30, 53, 65547, 0 };

long DTtag::objectGlossentryPar1[] = { 
57, 63, 68, 65543, 0 };

long DTtag::optionGlossentryPar1[] = { 
99, 65536, 65537, 65538, 0 };

long DTtag::pGlossentryPar1[] = { 
57, 65546, 0 };

long DTtag::phGlossentryPar1[] = { 
97, 65536, 65537, 0 };

long DTtag::qGlossentryPar1[] = { 
8, 10, 11, 82, 65539, 65540, 65541, 0 };

long DTtag::repsepGlossentryPar1[] = { 
35, 36, 37, 0 };

long DTtag::rowGlossentryPar1[] = { 
103, 106, 0 };

long DTtag::simpletableGlossentryPar1[] = { 
27, 65550, 0 };

long DTtag::stentryGlossentryPar1[] = { 
92, 93, 0 };

long DTtag::tableGlossentryPar1[] = { 
65550, 0 };

long DTtag::termGlossentryPar1[] = { 
65536, 65538, 65539, 0 };

long DTtag::titleGlossentryPar1[] = { 
14, 27, 28, 54, 102, 65549, 0 };

long DTtag::tmGlossentryPar1[] = { 
8, 48, 104, 108, 65539, 65540, 65541, 0 };

long DTtag::uicontrolGlossentryPar1[] = { 
58, 97, 65536, 65537, 0 };

long DTtag::varGlossentryPar1[] = { 
35, 36, 37, 99, 0 };

long DTtag::xrefGlossentryPar1[] = { 
5, 27, 28, 65541, 65544, 65545, 0 };

long DTtag::titlefirstGlossentryPar1[] = { 
27, 28, 54, 102, 65549, 0 };

vpair DTtag::GlossentryElemSetsSet1[] = {
 { 65536, Part1GlossentryPar1 },
 { 65537, Part2GlossentryPar1 },
 { 65538, Part3GlossentryPar1 },
 { 65539, Part4GlossentryPar1 },
 { 65540, Part5GlossentryPar1 },
 { 65541, Part6GlossentryPar1 },
 { 65542, Part7GlossentryPar1 },
 { 65543, Part8GlossentryPar1 },
 { 65544, Part9GlossentryPar1 },
 { 65545, Part10GlossentryPar1 },
 { 65546, Part11GlossentryPar1 },
 { 65547, Part12GlossentryPar1 },
 { 65548, Part13GlossentryPar1 },
 { 65549, Part14GlossentryPar1 },
 { 65550, Part15GlossentryPar1 },
 { 65551, citeGlossentryPar1 },
 { 65552, codephGlossentryPar1 },
 { 65553, dataGlossentryPar1 },
 { 65554, descGlossentryPar1 },
 { 65555, dlGlossentryPar1 },
 { 65556, draftcommentGlossentryPar1 },
 { 65557, figGlossentryPar1 },
 { 65558, figgroupGlossentryPar1 },
 { 65559, fnGlossentryPar1 },
 { 65560, foreignGlossentryPar1 },
 { 65561, fragmentGlossentryPar1 },
 { 65562, groupseqGlossentryPar1 },
 { 65563, imageGlossentryPar1 },
 { 65564, indextermGlossentryPar1 },
 { 65565, itemgroupGlossentryPar1 },
 { 65566, keywordGlossentryPar1 },
 { 65567, liGlossentryPar1 },
 { 65568, linkGlossentryPar1 },
 { 65569, linklistGlossentryPar1 },
 { 65570, linkpoolGlossentryPar1 },
 { 65571, lqGlossentryPar1 },
 { 65572, noteGlossentryPar1 },
 { 65573, objectGlossentryPar1 },
 { 65574, optionGlossentryPar1 },
 { 65575, pGlossentryPar1 },
 { 65576, phGlossentryPar1 },
 { 65577, qGlossentryPar1 },
 { 65578, repsepGlossentryPar1 },
 { 65579, rowGlossentryPar1 },
 { 65580, simpletableGlossentryPar1 },
 { 65581, stentryGlossentryPar1 },
 { 65582, tableGlossentryPar1 },
 { 65583, termGlossentryPar1 },
 { 65584, titleGlossentryPar1 },
 { 65585, tmGlossentryPar1 },
 { 65586, uicontrolGlossentryPar1 },
 { 65587, varGlossentryPar1 },
 { 65588, xrefGlossentryPar1 },
 { 65589, titlefirstGlossentryPar1 },
 { 0, NULL }
 };

npair DTtag::GlossentryElemSetNamesSet1[] = {
 { 65536, "*Part1"},
 { 65537, "*Part2"},
 { 65538, "*Part3"},
 { 65539, "*Part4"},
 { 65540, "*Part5"},
 { 65541, "*Part6"},
 { 65542, "*Part7"},
 { 65543, "*Part8"},
 { 65544, "*Part9"},
 { 65545, "*Part10"},
 { 65546, "*Part11"},
 { 65547, "*Part12"},
 { 65548, "*Part13"},
 { 65549, "*Part14"},
 { 65550, "*Part15"},
 { 65551, "*cite"},
 { 65552, "*codeph"},
 { 65553, "*data"},
 { 65554, "*desc"},
 { 65555, "*dl"},
 { 65556, "*draft-comment"},
 { 65557, "*fig"},
 { 65558, "*figgroup"},
 { 65559, "*fn"},
 { 65560, "*foreign"},
 { 65561, "*fragment"},
 { 65562, "*groupseq"},
 { 65563, "*image"},
 { 65564, "*indexterm"},
 { 65565, "*itemgroup"},
 { 65566, "*keyword"},
 { 65567, "*li"},
 { 65568, "*link"},
 { 65569, "*linklist"},
 { 65570, "*linkpool"},
 { 65571, "*lq"},
 { 65572, "*note"},
 { 65573, "*object"},
 { 65574, "*option"},
 { 65575, "*p"},
 { 65576, "*ph"},
 { 65577, "*q"},
 { 65578, "*repsep"},
 { 65579, "*row"},
 { 65580, "*simpletable"},
 { 65581, "*stentry"},
 { 65582, "*table"},
 { 65583, "*term"},
 { 65584, "*title"},
 { 65585, "*tm"},
 { 65586, "*uicontrol"},
 { 65587, "*var"},
 { 65588, "*xref"},
 { 65589, "*titlefirst"},
 { 0, NULL }
 };

ipair DTtag::GlossentryLevelsSet1[] = {
 { 2, 1 },
 { 34, 1 },
 { 62, 1 },
 { 78, 1 },
 { 0, 0 }
 };

ipair DTtag::GlossentryAtFirstSet1[] = {
 { 14, 15 },
 { 19, 64 },
 { 22, -1 },
 { 25, -1 },
 { 34, -1 },
 { 39, 40 },
 { 56, -1 },
 { 84, -1 },
 { 92, -1 },
 { 107, 65589 },
 { 0, 0 }
 };


// maps

npair DTtag::MapNamesSet1[] = {
 { 1, "map"},
 { 3, "alt"},
 { 4, "anchor"},
 { 5, "audience"},
 { 6, "author"},
 { 7, "boolean"},
 { 8, "brand"},
 { 9, "category"},
 { 10, "cite"},
 { 11, "colspec"},
 { 12, "component"},
 { 13, "copyrholder"},
 { 14, "copyright"},
 { 15, "copyryear"},
 { 16, "created"},
 { 17, "critdates"},
 { 18, "data"},
 { 19, "data-about"},
 { 20, "dd"},
 { 21, "ddhd"},
 { 22, "desc"},
 { 23, "dl"},
 { 24, "dlentry"},
 { 25, "dlhead"},
 { 26, "draft-comment"},
 { 27, "dt"},
 { 28, "dthd"},
 { 29, "entry"},
 { 30, "featnum"},
 { 31, "fig"},
 { 32, "figgroup"},
 { 33, "fn"},
 { 34, "foreign"},
 { 35, "image"},
 { 36, "index-base"},
 { 37, "index-see"},
 { 38, "index-see-also"},
 { 39, "index-sort-as"},
 { 40, "indexterm"},
 { 41, "indextermref"},
 { 42, "itemgroup"},
 { 43, "keyword"},
 { 44, "keywords"},
 { 45, "li"},
 { 46, "lines"},
 { 47, "linktext"},
 { 48, "lq"},
 { 49, "navref"},
 { 50, "note"},
 { 51, "object"},
 { 52, "ol"},
 { 53, "othermeta"},
 { 54, "p"},
 { 55, "param"},
 { 56, "permissions"},
 { 57, "ph"},
 { 58, "platform"},
 { 59, "pre"},
 { 60, "prodinfo"},
 { 61, "prodname"},
 { 62, "prognum"},
 { 63, "publisher"},
 { 64, "q"},
 { 65, "relcell"},
 { 66, "relcolspec"},
 { 67, "relheader"},
 { 68, "relrow"},
 { 69, "reltable"},
 { 70, "required-cleanup"},
 { 71, "resourceid"},
 { 72, "revised"},
 { 73, "row"},
 { 74, "searchtitle"},
 { 75, "series"},
 { 76, "shortdesc"},
 { 77, "simpletable"},
 { 78, "sl"},
 { 79, "sli"},
 { 80, "source"},
 { 81, "state"},
 { 82, "stentry"},
 { 83, "sthead"},
 { 84, "strow"},
 { 85, "table"},
 { 86, "tbody"},
 { 87, "term"},
 { 88, "tgroup"},
 { 89, "thead"},
 { 90, "title"},
 { 91, "tm"},
 { 92, "topicgroup"},
 { 93, "topichead"},
 { 94, "topicmeta"},
 { 95, "topicref"},
 { 96, "ul"},
 { 97, "unknown"},
 { 98, "vrm"},
 { 99, "vrmlist"},
 { 100, "xref"},
 { 0, NULL }
 };

ipair DTtag::MapInlineSet1[] = {
 { 3, 1 },
 { 6, 1 },
 { 7, 2 },
 { 8, 1 },
 { 9, 1 },
 { 10, 3 },
 { 12, 1 },
 { 13, 1 },
 { 18, 1 },
 { 20, 1 },
 { 21, 1 },
 { 22, 1 },
 { 23, 2 },
 { 26, 3 },
 { 27, 1 },
 { 28, 1 },
 { 29, 1 },
 { 30, 1 },
 { 31, 2 },
 { 33, 1 },
 { 34, 2 },
 { 35, 2 },
 { 36, 3 },
 { 37, 3 },
 { 38, 3 },
 { 39, 3 },
 { 40, 3 },
 { 41, 2 },
 { 42, 3 },
 { 43, 3 },
 { 45, 1 },
 { 46, 7 },
 { 47, 1 },
 { 48, 1 },
 { 50, 1 },
 { 51, 2 },
 { 52, 2 },
 { 54, 1 },
 { 57, 3 },
 { 58, 1 },
 { 59, 7 },
 { 61, 1 },
 { 62, 1 },
 { 63, 1 },
 { 64, 3 },
 { 70, 2 },
 { 74, 1 },
 { 75, 1 },
 { 76, 1 },
 { 78, 2 },
 { 79, 1 },
 { 80, 1 },
 { 81, 2 },
 { 82, 1 },
 { 85, 2 },
 { 87, 3 },
 { 90, 1 },
 { 91, 3 },
 { 96, 2 },
 { 97, 2 },
 { 100, 1 },
 { 0, 0 }
 };

ipair DTtag::MapSingleSet1[] = {
 { 3, 35 },
 { 4, 65563 },
 { 5, 94 },
 { 6, 94 },
 { 7, 65568 },
 { 8, 60 },
 { 9, 94 },
 { 10, 65548 },
 { 11, 88 },
 { 12, 60 },
 { 13, 14 },
 { 14, 94 },
 { 15, 14 },
 { 16, 17 },
 { 17, 94 },
 { 18, 65549 },
 { 19, 65549 },
 { 20, 24 },
 { 21, 25 },
 { 22, 65550 },
 { 23, 65551 },
 { 24, 23 },
 { 25, 23 },
 { 26, 65552 },
 { 27, 24 },
 { 28, 25 },
 { 29, 73 },
 { 30, 60 },
 { 31, 65553 },
 { 32, 65554 },
 { 33, 65555 },
 { 34, 65556 },
 { 35, 65557 },
 { 36, 40 },
 { 37, 40 },
 { 38, 40 },
 { 39, 40 },
 { 40, 65558 },
 { 41, 65552 },
 { 42, 65559 },
 { 43, 65560 },
 { 44, 94 },
 { 45, 65561 },
 { 46, 65551 },
 { 47, 94 },
 { 48, 65562 },
 { 49, 65563 },
 { 50, 65564 },
 { 51, 65565 },
 { 52, 65551 },
 { 53, 94 },
 { 54, 65566 },
 { 55, 51 },
 { 56, 94 },
 { 57, 65567 },
 { 58, 60 },
 { 59, 65551 },
 { 60, 94 },
 { 61, 60 },
 { 62, 60 },
 { 63, 94 },
 { 64, 65568 },
 { 65, 68 },
 { 66, 67 },
 { 67, 69 },
 { 68, 69 },
 { 69, 1 },
 { 70, 65552 },
 { 71, 94 },
 { 72, 17 },
 { 73, 65569 },
 { 74, 94 },
 { 75, 60 },
 { 76, 94 },
 { 77, 65570 },
 { 78, 65551 },
 { 79, 78 },
 { 80, 94 },
 { 81, 65568 },
 { 82, 65571 },
 { 83, 77 },
 { 84, 77 },
 { 85, 65572 },
 { 86, 88 },
 { 87, 65573 },
 { 88, 85 },
 { 89, 88 },
 { 90, 65574 },
 { 91, 65575 },
 { 92, 65577 },
 { 93, 65577 },
 { 94, 65576 },
 { 95, 65577 },
 { 96, 65551 },
 { 97, 65556 },
 { 98, 99 },
 { 99, 60 },
 { 100, 65578 },
 { 0, 0 }
 };

long DTtag::Part1MapPar1[] = { 
10, 18, 21, 22, 26, 28, 33, 42, 
45, 46, 48, 50, 54, 57, 59, 64, 
76, 79, 90, 100, 0 };

long DTtag::Part2MapPar1[] = { 
20, 27, 29, 32, 82, 0 };

long DTtag::Part3MapPar1[] = { 
3, 6, 8, 9, 12, 13, 30, 36, 
37, 38, 39, 40, 47, 58, 61, 62, 
63, 74, 75, 80, 0 };

long DTtag::Part4MapPar1[] = { 
22, 26, 33, 42, 45, 46, 48, 50, 
54, 57, 59, 64, 79, 0 };

long DTtag::Part5MapPar1[] = { 
10, 20, 21, 27, 28, 29, 76, 82, 
90, 100, 0 };

long DTtag::Part6MapPar1[] = { 
20, 27, 29, 31, 82, 94, 0 };

long DTtag::Part7MapPar1[] = { 
1, 65, 92, 93, 95, 0 };

long DTtag::Part8MapPar1[] = { 
18, 20, 26, 29, 31, 33, 42, 45, 
48, 50, 54, 82, 0 };

long DTtag::Part9MapPar1[] = { 
20, 22, 26, 29, 31, 33, 42, 45, 
50, 82, 0 };

long DTtag::Part10MapPar1[] = { 
20, 29, 42, 45, 48, 54, 82, 0 };

long DTtag::Part11MapPar1[] = { 
46, 50, 57, 59, 79, 0 };

long DTtag::Part12MapPar1[] = { 
20, 26, 42, 45, 48, 50, 54, 0 };

long DTtag::citeMapPar1[] = { 
20, 27, 29, 82, 65539, 0 };

long DTtag::dataMapPar1[] = { 
19, 65536, 65538, 65541, 65542, 0 };

long DTtag::descMapPar1[] = { 
31, 51, 85, 100, 0 };

long DTtag::dlMapPar1[] = { 
48, 54, 65544, 0 };

long DTtag::draftcommentMapPar1[] = { 
65545, 65546, 0 };

long DTtag::figMapPar1[] = { 
26, 33, 50, 65545, 0 };

long DTtag::figgroupMapPar1[] = { 
31, 32, 0 };

long DTtag::fnMapPar1[] = { 
31, 32, 65545, 65546, 0 };

long DTtag::foreignMapPar1[] = { 
51, 65536, 65538, 65541, 0 };

long DTtag::imageMapPar1[] = { 
21, 22, 27, 28, 57, 76, 79, 90, 
100, 65543, 0 };

long DTtag::indextermMapPar1[] = { 
36, 37, 38, 40, 44, 65545, 65546, 0 };

long DTtag::itemgroupMapPar1[] = { 
20, 45, 0 };

long DTtag::keywordMapPar1[] = { 
44, 65536, 65537, 65538, 0 };

long DTtag::liMapPar1[] = { 
52, 96, 0 };

long DTtag::lqMapPar1[] = { 
54, 65544, 0 };

long DTtag::navrefMapPar1[] = { 
1, 92, 93, 95, 0 };

long DTtag::noteMapPar1[] = { 
22, 26, 31, 33, 65545, 0 };

long DTtag::objectMapPar1[] = { 
65543, 0 };

long DTtag::pMapPar1[] = { 
48, 65544, 0 };

long DTtag::phMapPar1[] = { 
65536, 65537, 0 };

long DTtag::qMapPar1[] = { 
65539, 65540, 0 };

long DTtag::rowMapPar1[] = { 
86, 89, 0 };

long DTtag::simpletableMapPar1[] = { 
31, 65547, 0 };

long DTtag::stentryMapPar1[] = { 
83, 84, 0 };

long DTtag::tableMapPar1[] = { 
65547, 0 };

long DTtag::termMapPar1[] = { 
20, 27, 29, 82, 65536, 65538, 0 };

long DTtag::titleMapPar1[] = { 
18, 31, 32, 1, 85, 0 };

long DTtag::tmMapPar1[] = { 
43, 87, 91, 65539, 65540, 0 };

long DTtag::topicmetaMapPar1[] = { 
1, 66, 69, 92, 93, 95, 0 };

long DTtag::topicrefMapPar1[] = { 
65542, 0 };

long DTtag::xrefMapPar1[] = { 
31, 65537, 65539, 0 };

long DTtag::titlefirstMapPar1[] = { 
31, 32, 1, 85, 0 };

long DTtag::topicmetafirstMapPar1[] = { 
69, 92, 93, 95, 0 };

vpair DTtag::MapElemSetsSet1[] = {
 { 65536, Part1MapPar1 },
 { 65537, Part2MapPar1 },
 { 65538, Part3MapPar1 },
 { 65539, Part4MapPar1 },
 { 65540, Part5MapPar1 },
 { 65541, Part6MapPar1 },
 { 65542, Part7MapPar1 },
 { 65543, Part8MapPar1 },
 { 65544, Part9MapPar1 },
 { 65545, Part10MapPar1 },
 { 65546, Part11MapPar1 },
 { 65547, Part12MapPar1 },
 { 65548, citeMapPar1 },
 { 65549, dataMapPar1 },
 { 65550, descMapPar1 },
 { 65551, dlMapPar1 },
 { 65552, draftcommentMapPar1 },
 { 65553, figMapPar1 },
 { 65554, figgroupMapPar1 },
 { 65555, fnMapPar1 },
 { 65556, foreignMapPar1 },
 { 65557, imageMapPar1 },
 { 65558, indextermMapPar1 },
 { 65559, itemgroupMapPar1 },
 { 65560, keywordMapPar1 },
 { 65561, liMapPar1 },
 { 65562, lqMapPar1 },
 { 65563, navrefMapPar1 },
 { 65564, noteMapPar1 },
 { 65565, objectMapPar1 },
 { 65566, pMapPar1 },
 { 65567, phMapPar1 },
 { 65568, qMapPar1 },
 { 65569, rowMapPar1 },
 { 65570, simpletableMapPar1 },
 { 65571, stentryMapPar1 },
 { 65572, tableMapPar1 },
 { 65573, termMapPar1 },
 { 65574, titleMapPar1 },
 { 65575, tmMapPar1 },
 { 65576, topicmetaMapPar1 },
 { 65577, topicrefMapPar1 },
 { 65578, xrefMapPar1 },
 { 65579, titlefirstMapPar1 },
 { 65580, topicmetafirstMapPar1 },
 { 0, NULL }
 };

npair DTtag::MapElemSetNamesSet1[] = {
 { 65536, "*Part1"},
 { 65537, "*Part2"},
 { 65538, "*Part3"},
 { 65539, "*Part4"},
 { 65540, "*Part5"},
 { 65541, "*Part6"},
 { 65542, "*Part7"},
 { 65543, "*Part8"},
 { 65544, "*Part9"},
 { 65545, "*Part10"},
 { 65546, "*Part11"},
 { 65547, "*Part12"},
 { 65548, "*cite"},
 { 65549, "*data"},
 { 65550, "*desc"},
 { 65551, "*dl"},
 { 65552, "*draft-comment"},
 { 65553, "*fig"},
 { 65554, "*figgroup"},
 { 65555, "*fn"},
 { 65556, "*foreign"},
 { 65557, "*image"},
 { 65558, "*indexterm"},
 { 65559, "*itemgroup"},
 { 65560, "*keyword"},
 { 65561, "*li"},
 { 65562, "*lq"},
 { 65563, "*navref"},
 { 65564, "*note"},
 { 65565, "*object"},
 { 65566, "*p"},
 { 65567, "*ph"},
 { 65568, "*q"},
 { 65569, "*row"},
 { 65570, "*simpletable"},
 { 65571, "*stentry"},
 { 65572, "*table"},
 { 65573, "*term"},
 { 65574, "*title"},
 { 65575, "*tm"},
 { 65576, "*topicmeta"},
 { 65577, "*topicref"},
 { 65578, "*xref"},
 { 65579, "*titlefirst"},
 { 65580, "*topicmetafirst"},
 { 0, NULL }
 };

ipair DTtag::MapLevelsSet1[] = {
 { 69, 1 },
 { 0, 0 }
 };

ipair DTtag::MapAtFirstSet1[] = {
 { 16, -1 },
 { 18, 19 },
 { 22, 51 },
 { 25, -1 },
 { 28, -1 },
 { 47, -1 },
 { 61, -1 },
 { 83, -1 },
 { 90, 65579 },
 { 94, 65580 },
 { 0, 0 }
 };


// bookmap

npair DTtag::BookmapNamesSet1[] = {
 { 1, "bookmap"},
 { 3, "abbrevlist"},
 { 4, "addressdetails"},
 { 5, "administrativearea"},
 { 6, "alt"},
 { 7, "amendments"},
 { 8, "anchor"},
 { 9, "appendix"},
 { 10, "approved"},
 { 11, "audience"},
 { 12, "author"},
 { 13, "authorinformation"},
 { 14, "backmatter"},
 { 15, "bibliolist"},
 { 16, "bookabstract"},
 { 17, "bookchangehistory"},
 { 18, "bookevent"},
 { 19, "bookeventtype"},
 { 20, "bookid"},
 { 21, "booklibrary"},
 { 22, "booklist"},
 { 23, "booklists"},
 { 24, "bookmeta"},
 { 25, "booknumber"},
 { 26, "bookowner"},
 { 27, "bookpartno"},
 { 28, "bookrestriction"},
 { 29, "bookrights"},
 { 30, "booktitle"},
 { 31, "booktitlealt"},
 { 32, "boolean"},
 { 33, "brand"},
 { 34, "category"},
 { 35, "chapter"},
 { 36, "cite"},
 { 37, "colophon"},
 { 38, "colspec"},
 { 39, "completed"},
 { 40, "component"},
 { 41, "contactnumber"},
 { 42, "contactnumbers"},
 { 43, "copyrfirst"},
 { 44, "copyrholder"},
 { 45, "copyright"},
 { 46, "copyrlast"},
 { 47, "copyryear"},
 { 48, "country"},
 { 49, "created"},
 { 50, "critdates"},
 { 51, "data"},
 { 52, "data-about"},
 { 53, "day"},
 { 54, "dd"},
 { 55, "ddhd"},
 { 56, "dedication"},
 { 57, "desc"},
 { 58, "dl"},
 { 59, "dlentry"},
 { 60, "dlhead"},
 { 61, "draft-comment"},
 { 62, "draftintro"},
 { 63, "dt"},
 { 64, "dthd"},
 { 65, "edited"},
 { 66, "edition"},
 { 67, "emailaddress"},
 { 68, "emailaddresses"},
 { 69, "entry"},
 { 70, "featnum"},
 { 71, "fig"},
 { 72, "figgroup"},
 { 73, "figurelist"},
 { 74, "firstname"},
 { 75, "fn"},
 { 76, "foreign"},
 { 77, "frontmatter"},
 { 78, "generationidentifier"},
 { 79, "glossarylist"},
 { 80, "honorific"},
 { 81, "image"},
 { 82, "index-base"},
 { 83, "index-see"},
 { 84, "index-see-also"},
 { 85, "index-sort-as"},
 { 86, "indexlist"},
 { 87, "indexterm"},
 { 88, "indextermref"},
 { 89, "isbn"},
 { 90, "itemgroup"},
 { 91, "keyword"},
 { 92, "keywords"},
 { 93, "lastname"},
 { 94, "li"},
 { 95, "lines"},
 { 96, "linktext"},
 { 97, "locality"},
 { 98, "localityname"},
 { 99, "lq"},
 { 100, "mainbooktitle"},
 { 101, "maintainer"},
 { 102, "middlename"},
 { 103, "month"},
 { 104, "namedetails"},
 { 105, "navref"},
 { 106, "note"},
 { 107, "notices"},
 { 108, "object"},
 { 109, "ol"},
 { 110, "organization"},
 { 111, "organizationinfo"},
 { 112, "organizationname"},
 { 113, "organizationnamedetails"},
 { 114, "otherinfo"},
 { 115, "othermeta"},
 { 116, "p"},
 { 117, "param"},
 { 118, "part"},
 { 119, "permissions"},
 { 120, "person"},
 { 121, "personinfo"},
 { 122, "personname"},
 { 123, "ph"},
 { 124, "platform"},
 { 125, "postalcode"},
 { 126, "pre"},
 { 127, "preface"},
 { 128, "printlocation"},
 { 129, "prodinfo"},
 { 130, "prodname"},
 { 131, "prognum"},
 { 132, "published"},
 { 133, "publisher"},
 { 134, "publisherinformation"},
 { 135, "publishtype"},
 { 136, "q"},
 { 137, "relcell"},
 { 138, "relcolspec"},
 { 139, "relheader"},
 { 140, "relrow"},
 { 141, "reltable"},
 { 142, "required-cleanup"},
 { 143, "resourceid"},
 { 144, "reviewed"},
 { 145, "revised"},
 { 146, "revisionid"},
 { 147, "row"},
 { 148, "searchtitle"},
 { 149, "series"},
 { 150, "shortdesc"},
 { 151, "simpletable"},
 { 152, "sl"},
 { 153, "sli"},
 { 154, "source"},
 { 155, "started"},
 { 156, "state"},
 { 157, "stentry"},
 { 158, "sthead"},
 { 159, "strow"},
 { 160, "summary"},
 { 161, "table"},
 { 162, "tablelist"},
 { 163, "tbody"},
 { 164, "term"},
 { 165, "tested"},
 { 166, "tgroup"},
 { 167, "thead"},
 { 168, "thoroughfare"},
 { 169, "title"},
 { 170, "tm"},
 { 171, "toc"},
 { 172, "topicgroup"},
 { 173, "topichead"},
 { 174, "topicmeta"},
 { 175, "topicref"},
 { 176, "trademarklist"},
 { 177, "ul"},
 { 178, "unknown"},
 { 179, "url"},
 { 180, "urls"},
 { 181, "volume"},
 { 182, "vrm"},
 { 183, "vrmlist"},
 { 184, "xref"},
 { 185, "year"},
 { 0, NULL }
 };

ipair DTtag::BookmapInlineSet1[] = {
 { 4, 1 },
 { 5, 3 },
 { 6, 1 },
 { 12, 1 },
 { 21, 1 },
 { 25, 1 },
 { 27, 1 },
 { 31, 1 },
 { 32, 2 },
 { 33, 1 },
 { 34, 1 },
 { 36, 3 },
 { 40, 1 },
 { 41, 1 },
 { 44, 1 },
 { 48, 3 },
 { 51, 1 },
 { 53, 1 },
 { 54, 1 },
 { 55, 1 },
 { 57, 1 },
 { 58, 2 },
 { 61, 3 },
 { 63, 1 },
 { 64, 1 },
 { 66, 1 },
 { 67, 1 },
 { 69, 1 },
 { 70, 1 },
 { 71, 2 },
 { 74, 1 },
 { 75, 1 },
 { 76, 2 },
 { 78, 1 },
 { 80, 1 },
 { 81, 2 },
 { 82, 3 },
 { 83, 3 },
 { 84, 3 },
 { 85, 3 },
 { 87, 3 },
 { 88, 2 },
 { 89, 1 },
 { 90, 3 },
 { 91, 3 },
 { 93, 1 },
 { 94, 1 },
 { 95, 7 },
 { 96, 1 },
 { 97, 3 },
 { 98, 3 },
 { 99, 1 },
 { 100, 1 },
 { 102, 1 },
 { 103, 1 },
 { 106, 1 },
 { 108, 2 },
 { 109, 2 },
 { 110, 1 },
 { 112, 1 },
 { 114, 1 },
 { 116, 1 },
 { 120, 1 },
 { 123, 3 },
 { 124, 1 },
 { 125, 3 },
 { 126, 7 },
 { 128, 1 },
 { 130, 1 },
 { 131, 1 },
 { 133, 1 },
 { 136, 3 },
 { 142, 2 },
 { 146, 1 },
 { 148, 1 },
 { 149, 1 },
 { 150, 1 },
 { 152, 2 },
 { 153, 1 },
 { 154, 1 },
 { 156, 2 },
 { 157, 1 },
 { 160, 1 },
 { 161, 2 },
 { 164, 3 },
 { 168, 3 },
 { 169, 1 },
 { 170, 3 },
 { 177, 2 },
 { 178, 2 },
 { 179, 1 },
 { 181, 1 },
 { 184, 1 },
 { 185, 1 },
 { 0, 0 }
 };

ipair DTtag::BookmapSingleSet1[] = {
 { 3, 23 },
 { 4, 65572 },
 { 5, 4 },
 { 6, 81 },
 { 7, 14 },
 { 8, 65573 },
 { 9, 1 },
 { 10, 17 },
 { 11, 65569 },
 { 12, 65569 },
 { 13, 65569 },
 { 14, 1 },
 { 15, 23 },
 { 16, 77 },
 { 17, 24 },
 { 18, 17 },
 { 19, 18 },
 { 20, 24 },
 { 21, 30 },
 { 22, 23 },
 { 23, 65552 },
 { 24, 1 },
 { 25, 20 },
 { 26, 29 },
 { 27, 20 },
 { 28, 29 },
 { 29, 24 },
 { 30, 1 },
 { 31, 30 },
 { 32, 65580 },
 { 33, 129 },
 { 34, 65569 },
 { 35, 65553 },
 { 36, 65554 },
 { 37, 65552 },
 { 38, 166 },
 { 39, 65581 },
 { 40, 129 },
 { 41, 42 },
 { 42, 65572 },
 { 43, 29 },
 { 44, 45 },
 { 45, 174 },
 { 46, 29 },
 { 47, 45 },
 { 48, 4 },
 { 49, 50 },
 { 50, 65569 },
 { 51, 65555 },
 { 52, 65556 },
 { 53, 65571 },
 { 54, 59 },
 { 55, 60 },
 { 56, 65552 },
 { 57, 65557 },
 { 58, 65558 },
 { 59, 58 },
 { 60, 58 },
 { 61, 65559 },
 { 62, 77 },
 { 63, 59 },
 { 64, 60 },
 { 65, 17 },
 { 66, 20 },
 { 67, 68 },
 { 68, 65572 },
 { 69, 147 },
 { 70, 129 },
 { 71, 65560 },
 { 72, 65561 },
 { 73, 23 },
 { 74, 122 },
 { 75, 65562 },
 { 76, 65563 },
 { 77, 1 },
 { 78, 122 },
 { 79, 23 },
 { 80, 122 },
 { 81, 65564 },
 { 82, 87 },
 { 83, 87 },
 { 84, 87 },
 { 85, 87 },
 { 86, 23 },
 { 87, 65565 },
 { 88, 65559 },
 { 89, 20 },
 { 90, 65566 },
 { 91, 65567 },
 { 92, 65569 },
 { 93, 122 },
 { 94, 65568 },
 { 95, 65558 },
 { 96, 65569 },
 { 97, 4 },
 { 98, 97 },
 { 99, 65570 },
 { 100, 30 },
 { 101, 20 },
 { 102, 122 },
 { 103, 65571 },
 { 104, 65572 },
 { 105, 65573 },
 { 106, 65574 },
 { 107, 65552 },
 { 108, 65575 },
 { 109, 65558 },
 { 110, 65578 },
 { 111, 13 },
 { 112, 113 },
 { 113, 104 },
 { 114, 65576 },
 { 115, 65569 },
 { 116, 65577 },
 { 117, 108 },
 { 118, 1 },
 { 119, 65569 },
 { 120, 65578 },
 { 121, 13 },
 { 122, 104 },
 { 123, 65579 },
 { 124, 129 },
 { 125, 97 },
 { 126, 65558 },
 { 127, 77 },
 { 128, 134 },
 { 129, 65569 },
 { 130, 129 },
 { 131, 129 },
 { 132, 134 },
 { 133, 174 },
 { 134, 24 },
 { 135, 132 },
 { 136, 65580 },
 { 137, 140 },
 { 138, 139 },
 { 139, 141 },
 { 140, 141 },
 { 141, 1 },
 { 142, 65559 },
 { 143, 65569 },
 { 144, 17 },
 { 145, 50 },
 { 146, 65581 },
 { 147, 65582 },
 { 148, 65569 },
 { 149, 129 },
 { 150, 65569 },
 { 151, 65583 },
 { 152, 65558 },
 { 153, 152 },
 { 154, 65569 },
 { 155, 65581 },
 { 156, 65580 },
 { 157, 65584 },
 { 158, 151 },
 { 159, 151 },
 { 160, 65585 },
 { 161, 65586 },
 { 162, 23 },
 { 163, 166 },
 { 164, 65587 },
 { 165, 17 },
 { 166, 161 },
 { 167, 166 },
 { 168, 4 },
 { 169, 65588 },
 { 170, 65589 },
 { 171, 23 },
 { 172, 65591 },
 { 173, 65591 },
 { 174, 65590 },
 { 175, 65591 },
 { 176, 23 },
 { 177, 65558 },
 { 178, 65563 },
 { 179, 180 },
 { 180, 111 },
 { 181, 20 },
 { 182, 183 },
 { 183, 129 },
 { 184, 65592 },
 { 185, 65593 },
 { 0, 0 }
 };

long DTtag::Part1BookmapPar1[] = { 
36, 51, 55, 57, 61, 64, 75, 90, 
94, 95, 99, 106, 116, 123, 126, 136, 
150, 153, 169, 184, 0 };

long DTtag::Part2BookmapPar1[] = { 
21, 31, 54, 63, 69, 100, 112, 157, 
0 };

long DTtag::Part3BookmapPar1[] = { 
6, 12, 33, 34, 40, 44, 70, 82, 
83, 84, 85, 87, 96, 124, 130, 131, 
133, 148, 149, 154, 0 };

long DTtag::Part4BookmapPar1[] = { 
4, 5, 25, 27, 67, 97, 98, 110, 
114, 120, 128, 160, 168, 179, 0 };

long DTtag::Part5BookmapPar1[] = { 
57, 61, 75, 90, 94, 95, 99, 106, 
116, 123, 126, 136, 153, 0 };

long DTtag::Part6BookmapPar1[] = { 
36, 55, 64, 150, 169, 184, 0 };

long DTtag::Part7BookmapPar1[] = { 
54, 63, 69, 112, 157, 0 };

long DTtag::Part8BookmapPar1[] = { 
52, 71, 137, 172, 173, 174, 175, 0 };

long DTtag::Part9BookmapPar1[] = { 
10, 18, 65, 101, 132, 134, 144, 165, 
0 };

long DTtag::Part10BookmapPar1[] = { 
51, 61, 71, 75, 90, 94, 99, 106, 
116, 0 };

long DTtag::Part11BookmapPar1[] = { 
54, 57, 61, 69, 71, 75, 90, 94, 
106, 157, 0 };

long DTtag::Part12BookmapPar1[] = { 
54, 69, 90, 94, 99, 116, 157, 0 };

long DTtag::Part13BookmapPar1[] = { 
95, 106, 112, 123, 126, 153, 0 };

long DTtag::Part14BookmapPar1[] = { 
54, 61, 90, 94, 99, 106, 116, 0 };

long DTtag::Part15BookmapPar1[] = { 
10, 18, 65, 132, 144, 165, 0 };

long DTtag::Part16BookmapPar1[] = { 
9, 35, 62, 79, 107, 118, 127, 172, 
173, 175, 0 };

long DTtag::booklistsBookmapPar1[] = { 
14, 77, 0 };

long DTtag::chapterBookmapPar1[] = { 
1, 118, 0 };

long DTtag::citeBookmapPar1[] = { 
65540, 65542, 0 };

long DTtag::dataBookmapPar1[] = { 
24, 65536, 65538, 65539, 65542, 65543, 65544, 0 };

long DTtag::dataaboutBookmapPar1[] = { 
65536, 65538, 65539, 65542, 65543, 0 };

long DTtag::descBookmapPar1[] = { 
71, 108, 161, 184, 0 };

long DTtag::dlBookmapPar1[] = { 
99, 116, 65546, 0 };

long DTtag::draftcommentBookmapPar1[] = { 
65547, 65548, 0 };

long DTtag::figBookmapPar1[] = { 
61, 75, 106, 65547, 0 };

long DTtag::figgroupBookmapPar1[] = { 
71, 72, 0 };

long DTtag::fnBookmapPar1[] = { 
71, 72, 65547, 65548, 0 };

long DTtag::foreignBookmapPar1[] = { 
71, 108, 174, 65536, 65538, 65539, 65542, 0 };

long DTtag::imageBookmapPar1[] = { 
55, 57, 64, 123, 150, 153, 169, 184, 
65537, 65545, 0 };

long DTtag::indextermBookmapPar1[] = { 
82, 83, 84, 87, 92, 65547, 65548, 0 };

long DTtag::itemgroupBookmapPar1[] = { 
54, 94, 0 };

long DTtag::keywordBookmapPar1[] = { 
72, 92, 65536, 65537, 65538, 65539, 0 };

long DTtag::liBookmapPar1[] = { 
109, 177, 0 };

long DTtag::linktextBookmapPar1[] = { 
24, 174, 0 };

long DTtag::lqBookmapPar1[] = { 
116, 65546, 0 };

long DTtag::monthBookmapPar1[] = { 
39, 155, 0 };

long DTtag::namedetailsBookmapPar1[] = { 
111, 121, 0 };

long DTtag::navrefBookmapPar1[] = { 
172, 173, 175, 0 };

long DTtag::noteBookmapPar1[] = { 
57, 61, 71, 75, 65547, 0 };

long DTtag::objectBookmapPar1[] = { 
54, 69, 157, 65545, 0 };

long DTtag::otherinfoBookmapPar1[] = { 
113, 122, 0 };

long DTtag::pBookmapPar1[] = { 
99, 65546, 0 };

long DTtag::personBookmapPar1[] = { 
26, 65544, 0 };

long DTtag::phBookmapPar1[] = { 
72, 65536, 65537, 0 };

long DTtag::qBookmapPar1[] = { 
65537, 65540, 65541, 0 };

long DTtag::revisionidBookmapPar1[] = { 
65550, 0 };

long DTtag::rowBookmapPar1[] = { 
163, 167, 0 };

long DTtag::simpletableBookmapPar1[] = { 
71, 65549, 0 };

long DTtag::stentryBookmapPar1[] = { 
158, 159, 0 };

long DTtag::summaryBookmapPar1[] = { 
29, 65550, 0 };

long DTtag::tableBookmapPar1[] = { 
65549, 0 };

long DTtag::termBookmapPar1[] = { 
65536, 65537, 65538, 65539, 0 };

long DTtag::titleBookmapPar1[] = { 
1, 51, 71, 72, 161, 0 };

long DTtag::tmBookmapPar1[] = { 
91, 164, 170, 65537, 65540, 65541, 0 };

long DTtag::topicmetaBookmapPar1[] = { 
138, 141, 65551, 0 };

long DTtag::topicrefBookmapPar1[] = { 
14, 77, 137, 65551, 0 };

long DTtag::xrefBookmapPar1[] = { 
71, 72, 65540, 65542, 0 };

long DTtag::yearBookmapPar1[] = { 
39, 43, 46, 155, 0 };

long DTtag::titlefirstBookmapPar1[] = { 
71, 72, 161, 0 };

long DTtag::topicmetafirstBookmapPar1[] = { 
141, 65551, 0 };

vpair DTtag::BookmapElemSetsSet1[] = {
 { 65536, Part1BookmapPar1 },
 { 65537, Part2BookmapPar1 },
 { 65538, Part3BookmapPar1 },
 { 65539, Part4BookmapPar1 },
 { 65540, Part5BookmapPar1 },
 { 65541, Part6BookmapPar1 },
 { 65542, Part7BookmapPar1 },
 { 65543, Part8BookmapPar1 },
 { 65544, Part9BookmapPar1 },
 { 65545, Part10BookmapPar1 },
 { 65546, Part11BookmapPar1 },
 { 65547, Part12BookmapPar1 },
 { 65548, Part13BookmapPar1 },
 { 65549, Part14BookmapPar1 },
 { 65550, Part15BookmapPar1 },
 { 65551, Part16BookmapPar1 },
 { 65552, booklistsBookmapPar1 },
 { 65553, chapterBookmapPar1 },
 { 65554, citeBookmapPar1 },
 { 65555, dataBookmapPar1 },
 { 65556, dataaboutBookmapPar1 },
 { 65557, descBookmapPar1 },
 { 65558, dlBookmapPar1 },
 { 65559, draftcommentBookmapPar1 },
 { 65560, figBookmapPar1 },
 { 65561, figgroupBookmapPar1 },
 { 65562, fnBookmapPar1 },
 { 65563, foreignBookmapPar1 },
 { 65564, imageBookmapPar1 },
 { 65565, indextermBookmapPar1 },
 { 65566, itemgroupBookmapPar1 },
 { 65567, keywordBookmapPar1 },
 { 65568, liBookmapPar1 },
 { 65569, linktextBookmapPar1 },
 { 65570, lqBookmapPar1 },
 { 65571, monthBookmapPar1 },
 { 65572, namedetailsBookmapPar1 },
 { 65573, navrefBookmapPar1 },
 { 65574, noteBookmapPar1 },
 { 65575, objectBookmapPar1 },
 { 65576, otherinfoBookmapPar1 },
 { 65577, pBookmapPar1 },
 { 65578, personBookmapPar1 },
 { 65579, phBookmapPar1 },
 { 65580, qBookmapPar1 },
 { 65581, revisionidBookmapPar1 },
 { 65582, rowBookmapPar1 },
 { 65583, simpletableBookmapPar1 },
 { 65584, stentryBookmapPar1 },
 { 65585, summaryBookmapPar1 },
 { 65586, tableBookmapPar1 },
 { 65587, termBookmapPar1 },
 { 65588, titleBookmapPar1 },
 { 65589, tmBookmapPar1 },
 { 65590, topicmetaBookmapPar1 },
 { 65591, topicrefBookmapPar1 },
 { 65592, xrefBookmapPar1 },
 { 65593, yearBookmapPar1 },
 { 65594, titlefirstBookmapPar1 },
 { 65595, topicmetafirstBookmapPar1 },
 { 0, NULL }
 };

npair DTtag::BookmapElemSetNamesSet1[] = {
 { 65536, "*Part1"},
 { 65537, "*Part2"},
 { 65538, "*Part3"},
 { 65539, "*Part4"},
 { 65540, "*Part5"},
 { 65541, "*Part6"},
 { 65542, "*Part7"},
 { 65543, "*Part8"},
 { 65544, "*Part9"},
 { 65545, "*Part10"},
 { 65546, "*Part11"},
 { 65547, "*Part12"},
 { 65548, "*Part13"},
 { 65549, "*Part14"},
 { 65550, "*Part15"},
 { 65551, "*Part16"},
 { 65552, "*booklists"},
 { 65553, "*chapter"},
 { 65554, "*cite"},
 { 65555, "*data"},
 { 65556, "*data-about"},
 { 65557, "*desc"},
 { 65558, "*dl"},
 { 65559, "*draft-comment"},
 { 65560, "*fig"},
 { 65561, "*figgroup"},
 { 65562, "*fn"},
 { 65563, "*foreign"},
 { 65564, "*image"},
 { 65565, "*indexterm"},
 { 65566, "*itemgroup"},
 { 65567, "*keyword"},
 { 65568, "*li"},
 { 65569, "*linktext"},
 { 65570, "*lq"},
 { 65571, "*month"},
 { 65572, "*namedetails"},
 { 65573, "*navref"},
 { 65574, "*note"},
 { 65575, "*object"},
 { 65576, "*otherinfo"},
 { 65577, "*p"},
 { 65578, "*person"},
 { 65579, "*ph"},
 { 65580, "*q"},
 { 65581, "*revisionid"},
 { 65582, "*row"},
 { 65583, "*simpletable"},
 { 65584, "*stentry"},
 { 65585, "*summary"},
 { 65586, "*table"},
 { 65587, "*term"},
 { 65588, "*title"},
 { 65589, "*tm"},
 { 65590, "*topicmeta"},
 { 65591, "*topicref"},
 { 65592, "*xref"},
 { 65593, "*year"},
 { 65594, "*titlefirst"},
 { 65595, "*topicmetafirst"},
 { 0, NULL }
 };

ipair DTtag::BookmapLevelsSet1[] = {
 { 9, 1 },
 { 14, 1 },
 { 24, 1 },
 { 30, 1 },
 { 77, 1 },
 { 118, 1 },
 { 141, 1 },
 { 0, 0 }
 };

ipair DTtag::BookmapAtFirstSet1[] = {
 { 19, -1 },
 { 21, -1 },
 { 43, -1 },
 { 49, -1 },
 { 51, 52 },
 { 57, 108 },
 { 60, -1 },
 { 64, -1 },
 { 80, -1 },
 { 96, -1 },
 { 104, -1 },
 { 112, -1 },
 { 130, -1 },
 { 158, -1 },
 { 169, 65594 },
 { 174, 65595 },
 { 0, 0 }
 };



// end of dwhtmd1.cpp

