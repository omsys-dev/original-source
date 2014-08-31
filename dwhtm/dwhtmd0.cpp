// dwhtmd0.cpp is the data for dcl HTM DITA 1.0.1 specialization, JHG
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


// tagsets for standard 1.0 DTDs

npair DTtag::DTopicTypesSet0[] = {
 { 1, "topic"},
 { 2, "concept"},
 { 3, "task"},
 { 4, "reference"},
 { 5, "map"},
 { 0, NULL }
 };

npair DTtag::DTopicStartsSet0[] = {
 { 1, "title"},
 { 2, "title"},
 { 3, "title"},
 { 4, "title"},
 { 5, "topicmeta"},
 { 0, NULL }
 };

npair DTtag::DRelTopicTypesSet0[] = {
 { 1, "concept"},
 { 2, "task"},
 { 3, "reference"},
 { 0, NULL }
 };

npair DTtag::CommonNamesSet0[] = {
 { 1, "topic"},
 { 2, "body"},
 { 0, NULL }
 };


// preloaded DITA topic type info

char *DTtag::PrologDTypeDita0 = {"\"-//OASIS//DTD DITA Composite//EN\""};
char *DTtag::PrologDTDDita0 = {"  \"http://docs.oasis-open.org/dita/v1.0.1/dtd/ditabase.dtd\""};
char *DTtag::PrologDTypeTopic0 = {"\"-//OASIS//DTD DITA Topic//EN\""};
char *DTtag::PrologDTDTopic0 = {"  \"http://docs.oasis-open.org/dita/v1.0.1/dtd/topic.dtd\""};
char *DTtag::PrologDTypeConcept0 = {"\"-//OASIS//DTD DITA Concept//EN\""};
char *DTtag::PrologDTDConcept0 = {"  \"http://docs.oasis-open.org/dita/v1.0.1/dtd/concept.dtd\""};
char *DTtag::PrologDTypeTask0 = {"\"-//OASIS//DTD DITA Task//EN\""};
char *DTtag::PrologDTDTask0 = {"  \"http://docs.oasis-open.org/dita/v1.0.1/dtd/task.dtd\""};
char *DTtag::PrologDTypeReference0 = {"\"-//OASIS//DTD DITA Reference//EN\""};
char *DTtag::PrologDTDReference0 = {"  \"http://docs.oasis-open.org/dita/v1.0.1/dtd/reference.dtd\""};

char *DTtag::PrologDTypeMap0 = {"\"-//OASIS//DTD DITA Map//EN\""};
char *DTtag::PrologDTDMap0 = {"  \"http://docs.oasis-open.org/dita/v1.0.1/dtd/map.dtd\""};



// topic


npair DTtag::TopicNamesSet0[] = {
 { 1, "topic"},
 { 2, "body"},
 { 3, "alt"},
 { 4, "apiname"},
 { 5, "area"},
 { 6, "audience"},
 { 7, "author"},
 { 8, "b"},
 { 9, "boolean"},
 { 10, "brand"},
 { 11, "category"},
 { 12, "cite"},
 { 13, "cmdname"},
 { 14, "codeblock"},
 { 15, "codeph"},
 { 16, "colspec"},
 { 17, "component"},
 { 18, "coords"},
 { 19, "copyrholder"},
 { 20, "copyright"},
 { 21, "copyryear"},
 { 22, "created"},
 { 23, "critdates"},
 { 24, "dd"},
 { 25, "ddhd"},
 { 26, "delim"},
 { 27, "desc"},
 { 28, "dl"},
 { 29, "dlentry"},
 { 30, "dlhead"},
 { 31, "draft-comment"},
 { 32, "dt"},
 { 33, "dthd"},
 { 34, "entry"},
 { 35, "example"},
 { 36, "featnum"},
 { 37, "fig"},
 { 38, "figgroup"},
 { 39, "filepath"},
 { 40, "fn"},
 { 41, "fragment"},
 { 42, "fragref"},
 { 43, "groupchoice"},
 { 44, "groupcomp"},
 { 45, "groupseq"},
 { 46, "i"},
 { 47, "image"},
 { 48, "imagemap"},
 { 49, "indexterm"},
 { 50, "indextermref"},
 { 51, "itemgroup"},
 { 52, "keyword"},
 { 53, "keywords"},
 { 54, "kwd"},
 { 55, "li"},
 { 56, "lines"},
 { 57, "link"},
 { 58, "linkinfo"},
 { 59, "linklist"},
 { 60, "linkpool"},
 { 61, "linktext"},
 { 62, "lq"},
 { 63, "menucascade"},
 { 64, "metadata"},
 { 65, "msgblock"},
 { 66, "msgnum"},
 { 67, "msgph"},
 { 68, "navtitle"},
 { 69, "note"},
 { 70, "object"},
 { 71, "ol"},
 { 72, "oper"},
 { 73, "option"},
 { 74, "othermeta"},
 { 75, "p"},
 { 76, "param"},
 { 77, "parml"},
 { 78, "parmname"},
 { 79, "pd"},
 { 80, "permissions"},
 { 81, "ph"},
 { 82, "platform"},
 { 83, "plentry"},
 { 84, "pre"},
 { 85, "prodinfo"},
 { 86, "prodname"},
 { 87, "prognum"},
 { 88, "prolog"},
 { 89, "pt"},
 { 90, "publisher"},
 { 91, "q"},
 { 92, "related-links"},
 { 93, "repsep"},
 { 94, "required-cleanup"},
 { 95, "resourceid"},
 { 96, "revised"},
 { 97, "row"},
 { 98, "screen"},
 { 99, "searchtitle"},
 { 100, "section"},
 { 101, "sep"},
 { 102, "series"},
 { 103, "shape"},
 { 104, "shortcut"},
 { 105, "shortdesc"},
 { 106, "simpletable"},
 { 107, "sl"},
 { 108, "sli"},
 { 109, "source"},
 { 110, "state"},
 { 111, "stentry"},
 { 112, "sthead"},
 { 113, "strow"},
 { 114, "sub"},
 { 115, "sup"},
 { 116, "synblk"},
 { 117, "synnote"},
 { 118, "synnoteref"},
 { 119, "synph"},
 { 120, "syntaxdiagram"},
 { 121, "systemoutput"},
 { 122, "table"},
 { 123, "tbody"},
 { 124, "term"},
 { 125, "tgroup"},
 { 126, "thead"},
 { 127, "title"},
 { 128, "titlealts"},
 { 129, "tm"},
 { 130, "tt"},
 { 131, "u"},
 { 132, "uicontrol"},
 { 133, "ul"},
 { 134, "userinput"},
 { 135, "var"},
 { 136, "varname"},
 { 137, "vrm"},
 { 138, "vrmlist"},
 { 139, "wintitle"},
 { 140, "xref"},
 { 0, NULL }
 };

ipair DTtag::TopicInlineSet0[] = {
 { 3, 1 },
 { 4, 3 },
 { 7, 1 },
 { 8, 3 },
 { 9, 2 },
 { 10, 1 },
 { 11, 1 },
 { 12, 3 },
 { 13, 3 },
 { 14, 5 },
 { 15, 3 },
 { 17, 1 },
 { 18, 1 },
 { 19, 1 },
 { 24, 1 },
 { 25, 1 },
 { 26, 1 },
 { 27, 1 },
 { 31, 3 },
 { 32, 1 },
 { 33, 1 },
 { 34, 1 },
 { 35, 1 },
 { 36, 1 },
 { 37, 2 },
 { 39, 3 },
 { 40, 3 },
 { 42, 1 },
 { 46, 3 },
 { 48, 2 },
 { 49, 3 },
 { 50, 2 },
 { 51, 3 },
 { 52, 3 },
 { 54, 1 },
 { 55, 1 },
 { 56, 5 },
 { 58, 1 },
 { 61, 1 },
 { 62, 1 },
 { 63, 2 },
 { 65, 5 },
 { 66, 3 },
 { 67, 3 },
 { 68, 1 },
 { 69, 1 },
 { 72, 1 },
 { 73, 3 },
 { 75, 1 },
 { 78, 3 },
 { 79, 1 },
 { 81, 3 },
 { 82, 1 },
 { 84, 5 },
 { 86, 1 },
 { 87, 1 },
 { 89, 1 },
 { 90, 1 },
 { 91, 3 },
 { 93, 1 },
 { 94, 2 },
 { 98, 5 },
 { 99, 1 },
 { 100, 1 },
 { 101, 1 },
 { 102, 1 },
 { 103, 1 },
 { 104, 3 },
 { 105, 1 },
 { 108, 1 },
 { 109, 1 },
 { 110, 2 },
 { 111, 1 },
 { 114, 3 },
 { 115, 3 },
 { 117, 1 },
 { 119, 3 },
 { 120, 2 },
 { 121, 3 },
 { 124, 3 },
 { 127, 1 },
 { 129, 3 },
 { 130, 3 },
 { 131, 3 },
 { 132, 3 },
 { 134, 3 },
 { 135, 1 },
 { 136, 3 },
 { 139, 3 },
 { 140, 3 },
 { 0, 0 }
 };

ipair DTtag::TopicSingleSet0[] = {
 { 2, 1 },
 { 3, 47 },
 { 4, 65563 },
 { 5, 48 },
 { 6, 64 },
 { 7, 88 },
 { 8, 65573 },
 { 9, 65574 },
 { 10, 85 },
 { 11, 64 },
 { 12, 65550 },
 { 13, 65563 },
 { 14, 65553 },
 { 15, 65551 },
 { 16, 125 },
 { 17, 85 },
 { 18, 5 },
 { 19, 20 },
 { 20, 88 },
 { 21, 20 },
 { 22, 23 },
 { 23, 88 },
 { 24, 29 },
 { 25, 30 },
 { 26, 65585 },
 { 27, 65552 },
 { 28, 65553 },
 { 29, 28 },
 { 30, 28 },
 { 31, 65554 },
 { 32, 29 },
 { 33, 30 },
 { 34, 97 },
 { 35, 2 },
 { 36, 85 },
 { 37, 65555 },
 { 38, 65556 },
 { 39, 65573 },
 { 40, 65557 },
 { 41, 65558 },
 { 42, 65559 },
 { 43, 65559 },
 { 44, 65559 },
 { 45, 65559 },
 { 46, 65573 },
 { 47, 65560 },
 { 48, 65555 },
 { 49, 65561 },
 { 50, 65554 },
 { 51, 65562 },
 { 52, 65563 },
 { 53, 64 },
 { 54, 65585 },
 { 55, 65564 },
 { 56, 65553 },
 { 57, 65565 },
 { 58, 59 },
 { 59, 65566 },
 { 60, 65567 },
 { 61, 57 },
 { 62, 65568 },
 { 63, 65573 },
 { 64, 88 },
 { 65, 65553 },
 { 66, 65563 },
 { 67, 65573 },
 { 68, 128 },
 { 69, 65569 },
 { 70, 65570 },
 { 71, 65553 },
 { 72, 65585 },
 { 73, 65571 },
 { 74, 64 },
 { 75, 65572 },
 { 76, 70 },
 { 77, 65553 },
 { 78, 65571 },
 { 79, 83 },
 { 80, 88 },
 { 81, 65573 },
 { 82, 85 },
 { 83, 77 },
 { 84, 65553 },
 { 85, 64 },
 { 86, 85 },
 { 87, 85 },
 { 88, 1 },
 { 89, 83 },
 { 90, 88 },
 { 91, 65574 },
 { 92, 1 },
 { 93, 65575 },
 { 94, 65576 },
 { 95, 88 },
 { 96, 23 },
 { 97, 65577 },
 { 98, 65553 },
 { 99, 128 },
 { 100, 2 },
 { 101, 65585 },
 { 102, 85 },
 { 103, 5 },
 { 104, 132 },
 { 105, 1 },
 { 106, 65578 },
 { 107, 65553 },
 { 108, 107 },
 { 109, 88 },
 { 110, 65574 },
 { 111, 65579 },
 { 112, 106 },
 { 113, 106 },
 { 114, 65573 },
 { 115, 65573 },
 { 116, 120 },
 { 117, 65559 },
 { 118, 65559 },
 { 119, 65551 },
 { 120, 65555 },
 { 121, 65573 },
 { 122, 65580 },
 { 123, 125 },
 { 124, 65581 },
 { 125, 122 },
 { 126, 125 },
 { 127, 65582 },
 { 128, 1 },
 { 129, 65583 },
 { 130, 65573 },
 { 131, 65573 },
 { 132, 65584 },
 { 133, 65553 },
 { 134, 65573 },
 { 135, 65585 },
 { 136, 65563 },
 { 137, 138 },
 { 138, 85 },
 { 139, 65563 },
 { 140, 65586 },
 { 0, 0 }
 };

long DTtag::Part1TopicPar0[] = { 
41, 43, 44, 45, 116, 120, 0 };

long DTtag::Part2TopicPar0[] = { 
8, 14, 15, 35, 46, 56, 62, 69, 
75, 81, 84, 91, 98, 100, 105, 114, 
115, 127, 130, 131, 0 };

long DTtag::Part3TopicPar0[] = { 
12, 24, 25, 27, 31, 32, 33, 34, 
38, 40, 42, 51, 55, 58, 79, 89, 
108, 111, 117, 140, 0 };

long DTtag::Part4TopicPar0[] = { 
3, 7, 10, 11, 19, 39, 49, 53, 
61, 65, 67, 68, 86, 90, 99, 102, 
109, 121, 132, 134, 0 };

long DTtag::Part5TopicPar0[] = { 
17, 18, 26, 36, 72, 82, 87, 93, 
101, 135, 0 };

long DTtag::Part6TopicPar0[] = { 
12, 24, 25, 27, 31, 32, 33, 34, 
40, 42, 51, 55, 58, 79, 89, 108, 
111, 117, 140, 0 };

long DTtag::Part7TopicPar0[] = { 
35, 62, 69, 75, 81, 100, 105, 127, 
0 };

long DTtag::Part8TopicPar0[] = { 
8, 46, 56, 84, 91, 114, 115, 130, 
131, 0 };

long DTtag::Part9TopicPar0[] = { 
24, 27, 31, 32, 34, 40, 51, 55, 
58, 79, 89, 108, 111, 0 };

long DTtag::Part10TopicPar0[] = { 
14, 15, 35, 62, 69, 75, 81, 98, 
100, 117, 0 };

long DTtag::Part11TopicPar0[] = { 
2, 24, 27, 31, 34, 35, 37, 40, 
51, 55, 58, 69, 79, 100, 111, 0 };

long DTtag::Part12TopicPar0[] = { 
24, 34, 35, 51, 55, 62, 75, 79, 
100, 111, 0 };

long DTtag::Part13TopicPar0[] = { 
14, 56, 69, 81, 84, 98, 108, 0 };

long DTtag::Part14TopicPar0[] = { 
2, 24, 31, 35, 51, 55, 62, 69, 
75, 79, 100, 0 };

long DTtag::citeTopicPar0[] = { 
65543, 65544, 65545, 0 };

long DTtag::codephTopicPar0[] = { 
119, 65537, 65538, 0 };

long DTtag::descTopicPar0[] = { 
37, 57, 59, 70, 122, 140, 0 };

long DTtag::dlTopicPar0[] = { 
62, 75, 65546, 0 };

long DTtag::draftcommentTopicPar0[] = { 
65547, 65548, 0 };

long DTtag::figTopicPar0[] = { 
2, 31, 40, 69, 65547, 0 };

long DTtag::figgroupTopicPar0[] = { 
37, 38, 0 };

long DTtag::fnTopicPar0[] = { 
38, 65547, 65548, 0 };

long DTtag::fragmentTopicPar0[] = { 
116, 120, 0 };

long DTtag::groupseqTopicPar0[] = { 
65536, 0 };

long DTtag::imageTopicPar0[] = { 
2, 25, 33, 37, 48, 132, 140, 65542, 
65544, 0 };

long DTtag::indextermTopicPar0[] = { 
49, 53, 65547, 65548, 0 };

long DTtag::itemgroupTopicPar0[] = { 
24, 55, 79, 0 };

long DTtag::keywordTopicPar0[] = { 
65537, 65538, 65539, 65540, 0 };

long DTtag::liTopicPar0[] = { 
71, 133, 0 };

long DTtag::linkTopicPar0[] = { 
59, 60, 92, 0 };

long DTtag::linklistTopicPar0[] = { 
59, 92, 0 };

long DTtag::linkpoolTopicPar0[] = { 
60, 92, 0 };

long DTtag::lqTopicPar0[] = { 
75, 65546, 0 };

long DTtag::noteTopicPar0[] = { 
2, 27, 31, 37, 40, 58, 65547, 0 };

long DTtag::objectTopicPar0[] = { 
2, 31, 37, 40, 69, 65547, 0 };

long DTtag::optionTopicPar0[] = { 
119, 65537, 65538, 65539, 65540, 0 };

long DTtag::pTopicPar0[] = { 
62, 65546, 0 };

long DTtag::phTopicPar0[] = { 
65537, 65538, 0 };

long DTtag::qTopicPar0[] = { 
65537, 65541, 0 };

long DTtag::repsepTopicPar0[] = { 
43, 44, 45, 0 };

long DTtag::requiredcleanupTopicPar0[] = { 
2, 65547, 65548, 0 };

long DTtag::rowTopicPar0[] = { 
123, 126, 0 };

long DTtag::simpletableTopicPar0[] = { 
37, 65549, 0 };

long DTtag::stentryTopicPar0[] = { 
112, 113, 0 };

long DTtag::tableTopicPar0[] = { 
65549, 0 };

long DTtag::termTopicPar0[] = { 
3, 7, 10, 11, 19, 39, 49, 61, 
65, 67, 68, 86, 90, 99, 102, 109, 
121, 132, 134, 65537, 65540, 65541, 0 };

long DTtag::titleTopicPar0[] = { 
35, 37, 38, 59, 100, 122, 1, 65536, 
0 };

long DTtag::tmTopicPar0[] = { 
52, 124, 129, 65541, 65542, 65543, 0 };

long DTtag::uicontrolTopicPar0[] = { 
63, 65537, 65538, 0 };

long DTtag::varTopicPar0[] = { 
43, 44, 45, 119, 0 };

long DTtag::xrefTopicPar0[] = { 
5, 38, 65543, 65544, 65545, 0 };

long DTtag::titlefirstTopicPar0[] = { 
37, 38, 59, 122, 1, 65536, 0 };

vpair DTtag::TopicElemSetsSet0[] = {
 { 65536, Part1TopicPar0 },
 { 65537, Part2TopicPar0 },
 { 65538, Part3TopicPar0 },
 { 65539, Part4TopicPar0 },
 { 65540, Part5TopicPar0 },
 { 65541, Part6TopicPar0 },
 { 65542, Part7TopicPar0 },
 { 65543, Part8TopicPar0 },
 { 65544, Part9TopicPar0 },
 { 65545, Part10TopicPar0 },
 { 65546, Part11TopicPar0 },
 { 65547, Part12TopicPar0 },
 { 65548, Part13TopicPar0 },
 { 65549, Part14TopicPar0 },
 { 65550, citeTopicPar0 },
 { 65551, codephTopicPar0 },
 { 65552, descTopicPar0 },
 { 65553, dlTopicPar0 },
 { 65554, draftcommentTopicPar0 },
 { 65555, figTopicPar0 },
 { 65556, figgroupTopicPar0 },
 { 65557, fnTopicPar0 },
 { 65558, fragmentTopicPar0 },
 { 65559, groupseqTopicPar0 },
 { 65560, imageTopicPar0 },
 { 65561, indextermTopicPar0 },
 { 65562, itemgroupTopicPar0 },
 { 65563, keywordTopicPar0 },
 { 65564, liTopicPar0 },
 { 65565, linkTopicPar0 },
 { 65566, linklistTopicPar0 },
 { 65567, linkpoolTopicPar0 },
 { 65568, lqTopicPar0 },
 { 65569, noteTopicPar0 },
 { 65570, objectTopicPar0 },
 { 65571, optionTopicPar0 },
 { 65572, pTopicPar0 },
 { 65573, phTopicPar0 },
 { 65574, qTopicPar0 },
 { 65575, repsepTopicPar0 },
 { 65576, requiredcleanupTopicPar0 },
 { 65577, rowTopicPar0 },
 { 65578, simpletableTopicPar0 },
 { 65579, stentryTopicPar0 },
 { 65580, tableTopicPar0 },
 { 65581, termTopicPar0 },
 { 65582, titleTopicPar0 },
 { 65583, tmTopicPar0 },
 { 65584, uicontrolTopicPar0 },
 { 65585, varTopicPar0 },
 { 65586, xrefTopicPar0 },
 { 65587, titlefirstTopicPar0 },
 { 0, NULL }
 };

npair DTtag::TopicElemSetNamesSet0[] = {
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
 { 65550, "*cite"},
 { 65551, "*codeph"},
 { 65552, "*desc"},
 { 65553, "*dl"},
 { 65554, "*draft-comment"},
 { 65555, "*fig"},
 { 65556, "*figgroup"},
 { 65557, "*fn"},
 { 65558, "*fragment"},
 { 65559, "*groupseq"},
 { 65560, "*image"},
 { 65561, "*indexterm"},
 { 65562, "*itemgroup"},
 { 65563, "*keyword"},
 { 65564, "*li"},
 { 65565, "*link"},
 { 65566, "*linklist"},
 { 65567, "*linkpool"},
 { 65568, "*lq"},
 { 65569, "*note"},
 { 65570, "*object"},
 { 65571, "*option"},
 { 65572, "*p"},
 { 65573, "*ph"},
 { 65574, "*q"},
 { 65575, "*repsep"},
 { 65576, "*required-cleanup"},
 { 65577, "*row"},
 { 65578, "*simpletable"},
 { 65579, "*stentry"},
 { 65580, "*table"},
 { 65581, "*term"},
 { 65582, "*title"},
 { 65583, "*tm"},
 { 65584, "*uicontrol"},
 { 65585, "*var"},
 { 65586, "*xref"},
 { 65587, "*titlefirst"},
 { 0, NULL }
 };

ipair DTtag::TopicLevelsSet0[] = {
 { 1, 1 },
 { 2, 1 },
 { 88, 1 },
 { 92, 1 },
 { 105, 1 },
 { 128, 1 },
 { 0, 0 }
 };

ipair DTtag::TopicAtFirstSet0[] = {
 { 22, -1 },
 { 27, 70 },
 { 30, -1 },
 { 33, -1 },
 { 47, 48 },
 { 61, -1 },
 { 68, -1 },
 { 86, -1 },
 { 103, -1 },
 { 112, -1 },
 { 127, 65587 },
 { 0, 0 }
 };


// concept


npair DTtag::ConceptNamesSet0[] = {
 { 1, "concept"},
 { 2, "conbody"},
 { 3, "alt"},
 { 4, "apiname"},
 { 5, "area"},
 { 6, "audience"},
 { 7, "author"},
 { 8, "b"},
 { 9, "boolean"},
 { 10, "brand"},
 { 11, "category"},
 { 12, "cite"},
 { 13, "cmdname"},
 { 14, "codeblock"},
 { 15, "codeph"},
 { 16, "colspec"},
 { 17, "component"},
 { 18, "coords"},
 { 19, "copyrholder"},
 { 20, "copyright"},
 { 21, "copyryear"},
 { 22, "created"},
 { 23, "critdates"},
 { 24, "dd"},
 { 25, "ddhd"},
 { 26, "delim"},
 { 27, "desc"},
 { 28, "dl"},
 { 29, "dlentry"},
 { 30, "dlhead"},
 { 31, "draft-comment"},
 { 32, "dt"},
 { 33, "dthd"},
 { 34, "entry"},
 { 35, "example"},
 { 36, "featnum"},
 { 37, "fig"},
 { 38, "figgroup"},
 { 39, "filepath"},
 { 40, "fn"},
 { 41, "fragment"},
 { 42, "fragref"},
 { 43, "groupchoice"},
 { 44, "groupcomp"},
 { 45, "groupseq"},
 { 46, "i"},
 { 47, "image"},
 { 48, "imagemap"},
 { 49, "indexterm"},
 { 50, "indextermref"},
 { 51, "itemgroup"},
 { 52, "keyword"},
 { 53, "keywords"},
 { 54, "kwd"},
 { 55, "li"},
 { 56, "lines"},
 { 57, "link"},
 { 58, "linkinfo"},
 { 59, "linklist"},
 { 60, "linkpool"},
 { 61, "linktext"},
 { 62, "lq"},
 { 63, "menucascade"},
 { 64, "metadata"},
 { 65, "msgblock"},
 { 66, "msgnum"},
 { 67, "msgph"},
 { 68, "navtitle"},
 { 69, "note"},
 { 70, "object"},
 { 71, "ol"},
 { 72, "oper"},
 { 73, "option"},
 { 74, "othermeta"},
 { 75, "p"},
 { 76, "param"},
 { 77, "parml"},
 { 78, "parmname"},
 { 79, "pd"},
 { 80, "permissions"},
 { 81, "ph"},
 { 82, "platform"},
 { 83, "plentry"},
 { 84, "pre"},
 { 85, "prodinfo"},
 { 86, "prodname"},
 { 87, "prognum"},
 { 88, "prolog"},
 { 89, "pt"},
 { 90, "publisher"},
 { 91, "q"},
 { 92, "related-links"},
 { 93, "repsep"},
 { 94, "required-cleanup"},
 { 95, "resourceid"},
 { 96, "revised"},
 { 97, "row"},
 { 98, "screen"},
 { 99, "searchtitle"},
 { 100, "section"},
 { 101, "sep"},
 { 102, "series"},
 { 103, "shape"},
 { 104, "shortcut"},
 { 105, "shortdesc"},
 { 106, "simpletable"},
 { 107, "sl"},
 { 108, "sli"},
 { 109, "source"},
 { 110, "state"},
 { 111, "stentry"},
 { 112, "sthead"},
 { 113, "strow"},
 { 114, "sub"},
 { 115, "sup"},
 { 116, "synblk"},
 { 117, "synnote"},
 { 118, "synnoteref"},
 { 119, "synph"},
 { 120, "syntaxdiagram"},
 { 121, "systemoutput"},
 { 122, "table"},
 { 123, "tbody"},
 { 124, "term"},
 { 125, "tgroup"},
 { 126, "thead"},
 { 127, "title"},
 { 128, "titlealts"},
 { 129, "tm"},
 { 130, "tt"},
 { 131, "u"},
 { 132, "uicontrol"},
 { 133, "ul"},
 { 134, "userinput"},
 { 135, "var"},
 { 136, "varname"},
 { 137, "vrm"},
 { 138, "vrmlist"},
 { 139, "wintitle"},
 { 140, "xref"},
 { 0, NULL }
 };

ipair DTtag::ConceptInlineSet0[] = {
 { 3, 1 },
 { 4, 3 },
 { 7, 1 },
 { 8, 3 },
 { 9, 2 },
 { 10, 1 },
 { 11, 1 },
 { 12, 3 },
 { 13, 3 },
 { 14, 5 },
 { 15, 3 },
 { 17, 1 },
 { 18, 1 },
 { 19, 1 },
 { 24, 1 },
 { 25, 1 },
 { 26, 1 },
 { 27, 1 },
 { 31, 3 },
 { 32, 1 },
 { 33, 1 },
 { 34, 1 },
 { 35, 1 },
 { 36, 1 },
 { 39, 3 },
 { 40, 3 },
 { 42, 1 },
 { 46, 3 },
 { 49, 3 },
 { 50, 2 },
 { 51, 3 },
 { 52, 3 },
 { 54, 1 },
 { 55, 1 },
 { 56, 5 },
 { 58, 1 },
 { 61, 1 },
 { 62, 1 },
 { 63, 2 },
 { 65, 5 },
 { 66, 3 },
 { 67, 3 },
 { 68, 1 },
 { 69, 1 },
 { 72, 1 },
 { 73, 3 },
 { 75, 1 },
 { 78, 3 },
 { 79, 1 },
 { 81, 3 },
 { 82, 1 },
 { 84, 5 },
 { 86, 1 },
 { 87, 1 },
 { 89, 1 },
 { 90, 1 },
 { 91, 3 },
 { 93, 1 },
 { 98, 5 },
 { 99, 1 },
 { 100, 1 },
 { 101, 1 },
 { 102, 1 },
 { 103, 1 },
 { 104, 3 },
 { 105, 1 },
 { 108, 1 },
 { 109, 1 },
 { 110, 2 },
 { 111, 1 },
 { 114, 3 },
 { 115, 3 },
 { 117, 1 },
 { 119, 3 },
 { 121, 3 },
 { 124, 3 },
 { 127, 1 },
 { 129, 3 },
 { 130, 3 },
 { 131, 3 },
 { 132, 3 },
 { 134, 3 },
 { 135, 1 },
 { 136, 3 },
 { 139, 3 },
 { 140, 3 },
 { 0, 0 }
 };

ipair DTtag::ConceptSingleSet0[] = {
 { 2, 1 },
 { 3, 47 },
 { 4, 65563 },
 { 5, 48 },
 { 6, 64 },
 { 7, 88 },
 { 8, 65573 },
 { 9, 65574 },
 { 10, 85 },
 { 11, 64 },
 { 12, 65550 },
 { 13, 65563 },
 { 14, 65553 },
 { 15, 65551 },
 { 16, 125 },
 { 17, 85 },
 { 18, 5 },
 { 19, 20 },
 { 20, 88 },
 { 21, 20 },
 { 22, 23 },
 { 23, 88 },
 { 24, 29 },
 { 25, 30 },
 { 26, 65585 },
 { 27, 65552 },
 { 28, 65553 },
 { 29, 28 },
 { 30, 28 },
 { 31, 65554 },
 { 32, 29 },
 { 33, 30 },
 { 34, 97 },
 { 35, 2 },
 { 36, 85 },
 { 37, 65555 },
 { 38, 65556 },
 { 39, 65573 },
 { 40, 65557 },
 { 41, 65558 },
 { 42, 65559 },
 { 43, 65559 },
 { 44, 65559 },
 { 45, 65559 },
 { 46, 65573 },
 { 47, 65560 },
 { 48, 65555 },
 { 49, 65561 },
 { 50, 65554 },
 { 51, 65562 },
 { 52, 65563 },
 { 53, 64 },
 { 54, 65585 },
 { 55, 65564 },
 { 56, 65553 },
 { 57, 65565 },
 { 58, 59 },
 { 59, 65566 },
 { 60, 65567 },
 { 61, 57 },
 { 62, 65568 },
 { 63, 65573 },
 { 64, 88 },
 { 65, 65553 },
 { 66, 65563 },
 { 67, 65573 },
 { 68, 128 },
 { 69, 65569 },
 { 70, 65570 },
 { 71, 65553 },
 { 72, 65585 },
 { 73, 65571 },
 { 74, 64 },
 { 75, 65572 },
 { 76, 70 },
 { 77, 65553 },
 { 78, 65571 },
 { 79, 83 },
 { 80, 88 },
 { 81, 65573 },
 { 82, 85 },
 { 83, 77 },
 { 84, 65553 },
 { 85, 64 },
 { 86, 85 },
 { 87, 85 },
 { 88, 1 },
 { 89, 83 },
 { 90, 88 },
 { 91, 65574 },
 { 92, 1 },
 { 93, 65575 },
 { 94, 65576 },
 { 95, 88 },
 { 96, 23 },
 { 97, 65577 },
 { 98, 65553 },
 { 99, 128 },
 { 100, 2 },
 { 101, 65585 },
 { 102, 85 },
 { 103, 5 },
 { 104, 132 },
 { 105, 1 },
 { 106, 65578 },
 { 107, 65553 },
 { 108, 107 },
 { 109, 88 },
 { 110, 65574 },
 { 111, 65579 },
 { 112, 106 },
 { 113, 106 },
 { 114, 65573 },
 { 115, 65573 },
 { 116, 120 },
 { 117, 65559 },
 { 118, 65559 },
 { 119, 65551 },
 { 120, 65555 },
 { 121, 65573 },
 { 122, 65580 },
 { 123, 125 },
 { 124, 65581 },
 { 125, 122 },
 { 126, 125 },
 { 127, 65582 },
 { 128, 1 },
 { 129, 65583 },
 { 130, 65573 },
 { 131, 65573 },
 { 132, 65584 },
 { 133, 65553 },
 { 134, 65573 },
 { 135, 65585 },
 { 136, 65563 },
 { 137, 138 },
 { 138, 85 },
 { 139, 65563 },
 { 140, 65586 },
 { 0, 0 }
 };

long DTtag::Part1ConceptPar0[] = { 
41, 43, 44, 45, 116, 120, 0 };

long DTtag::Part2ConceptPar0[] = { 
8, 14, 15, 35, 46, 56, 62, 69, 
75, 81, 84, 91, 98, 100, 105, 114, 
115, 127, 130, 131, 0 };

long DTtag::Part3ConceptPar0[] = { 
12, 24, 25, 27, 31, 32, 33, 34, 
38, 40, 42, 51, 55, 58, 79, 89, 
108, 111, 117, 140, 0 };

long DTtag::Part4ConceptPar0[] = { 
3, 7, 10, 11, 19, 39, 49, 53, 
61, 65, 67, 68, 86, 90, 99, 102, 
109, 121, 132, 134, 0 };

long DTtag::Part5ConceptPar0[] = { 
17, 18, 26, 36, 72, 82, 87, 93, 
101, 135, 0 };

long DTtag::Part6ConceptPar0[] = { 
12, 24, 25, 27, 31, 32, 33, 34, 
40, 42, 51, 55, 58, 79, 89, 108, 
111, 117, 140, 0 };

long DTtag::Part7ConceptPar0[] = { 
35, 62, 69, 75, 81, 100, 105, 127, 
0 };

long DTtag::Part8ConceptPar0[] = { 
8, 46, 56, 84, 91, 114, 115, 130, 
131, 0 };

long DTtag::Part9ConceptPar0[] = { 
24, 27, 31, 32, 34, 40, 51, 55, 
58, 79, 89, 108, 111, 0 };

long DTtag::Part10ConceptPar0[] = { 
14, 15, 35, 62, 69, 75, 81, 98, 
100, 117, 0 };

long DTtag::Part11ConceptPar0[] = { 
2, 24, 27, 31, 34, 35, 37, 40, 
51, 55, 58, 69, 79, 100, 111, 0 };

long DTtag::Part12ConceptPar0[] = { 
24, 34, 35, 51, 55, 62, 75, 79, 
100, 111, 0 };

long DTtag::Part13ConceptPar0[] = { 
14, 56, 69, 81, 84, 98, 108, 0 };

long DTtag::Part14ConceptPar0[] = { 
2, 24, 31, 35, 51, 55, 62, 69, 
75, 79, 100, 0 };

long DTtag::citeConceptPar0[] = { 
65543, 65544, 65545, 0 };

long DTtag::codephConceptPar0[] = { 
119, 65537, 65538, 0 };

long DTtag::descConceptPar0[] = { 
37, 57, 59, 70, 122, 140, 0 };

long DTtag::dlConceptPar0[] = { 
62, 75, 65546, 0 };

long DTtag::draftcommentConceptPar0[] = { 
65547, 65548, 0 };

long DTtag::figConceptPar0[] = { 
2, 31, 40, 69, 65547, 0 };

long DTtag::figgroupConceptPar0[] = { 
37, 38, 0 };

long DTtag::fnConceptPar0[] = { 
38, 65547, 65548, 0 };

long DTtag::fragmentConceptPar0[] = { 
116, 120, 0 };

long DTtag::groupseqConceptPar0[] = { 
65536, 0 };

long DTtag::imageConceptPar0[] = { 
2, 25, 33, 37, 48, 132, 140, 65542, 
65544, 0 };

long DTtag::indextermConceptPar0[] = { 
49, 53, 65547, 65548, 0 };

long DTtag::itemgroupConceptPar0[] = { 
24, 55, 79, 0 };

long DTtag::keywordConceptPar0[] = { 
65537, 65538, 65539, 65540, 0 };

long DTtag::liConceptPar0[] = { 
71, 133, 0 };

long DTtag::linkConceptPar0[] = { 
59, 60, 92, 0 };

long DTtag::linklistConceptPar0[] = { 
59, 92, 0 };

long DTtag::linkpoolConceptPar0[] = { 
60, 92, 0 };

long DTtag::lqConceptPar0[] = { 
75, 65546, 0 };

long DTtag::noteConceptPar0[] = { 
2, 27, 31, 37, 40, 58, 65547, 0 };

long DTtag::objectConceptPar0[] = { 
2, 31, 37, 40, 69, 65547, 0 };

long DTtag::optionConceptPar0[] = { 
119, 65537, 65538, 65539, 65540, 0 };

long DTtag::pConceptPar0[] = { 
62, 65546, 0 };

long DTtag::phConceptPar0[] = { 
65537, 65538, 0 };

long DTtag::qConceptPar0[] = { 
65537, 65541, 0 };

long DTtag::repsepConceptPar0[] = { 
43, 44, 45, 0 };

long DTtag::requiredcleanupConceptPar0[] = { 
2, 65547, 65548, 0 };

long DTtag::rowConceptPar0[] = { 
123, 126, 0 };

long DTtag::simpletableConceptPar0[] = { 
37, 65549, 0 };

long DTtag::stentryConceptPar0[] = { 
112, 113, 0 };

long DTtag::tableConceptPar0[] = { 
65549, 0 };

long DTtag::termConceptPar0[] = { 
3, 7, 10, 11, 19, 39, 49, 61, 
65, 67, 68, 86, 90, 99, 102, 109, 
121, 132, 134, 65537, 65540, 65541, 0 };

long DTtag::titleConceptPar0[] = { 
1, 35, 37, 38, 59, 100, 122, 65536, 
0 };

long DTtag::tmConceptPar0[] = { 
52, 124, 129, 65541, 65542, 65543, 0 };

long DTtag::uicontrolConceptPar0[] = { 
63, 65537, 65538, 0 };

long DTtag::varConceptPar0[] = { 
43, 44, 45, 119, 0 };

long DTtag::xrefConceptPar0[] = { 
5, 38, 65543, 65544, 65545, 0 };

long DTtag::titlefirstConceptPar0[] = { 
1, 37, 38, 59, 122, 65536, 0 };

vpair DTtag::ConceptElemSetsSet0[] = {
 { 65536, Part1ConceptPar0 },
 { 65537, Part2ConceptPar0 },
 { 65538, Part3ConceptPar0 },
 { 65539, Part4ConceptPar0 },
 { 65540, Part5ConceptPar0 },
 { 65541, Part6ConceptPar0 },
 { 65542, Part7ConceptPar0 },
 { 65543, Part8ConceptPar0 },
 { 65544, Part9ConceptPar0 },
 { 65545, Part10ConceptPar0 },
 { 65546, Part11ConceptPar0 },
 { 65547, Part12ConceptPar0 },
 { 65548, Part13ConceptPar0 },
 { 65549, Part14ConceptPar0 },
 { 65550, citeConceptPar0 },
 { 65551, codephConceptPar0 },
 { 65552, descConceptPar0 },
 { 65553, dlConceptPar0 },
 { 65554, draftcommentConceptPar0 },
 { 65555, figConceptPar0 },
 { 65556, figgroupConceptPar0 },
 { 65557, fnConceptPar0 },
 { 65558, fragmentConceptPar0 },
 { 65559, groupseqConceptPar0 },
 { 65560, imageConceptPar0 },
 { 65561, indextermConceptPar0 },
 { 65562, itemgroupConceptPar0 },
 { 65563, keywordConceptPar0 },
 { 65564, liConceptPar0 },
 { 65565, linkConceptPar0 },
 { 65566, linklistConceptPar0 },
 { 65567, linkpoolConceptPar0 },
 { 65568, lqConceptPar0 },
 { 65569, noteConceptPar0 },
 { 65570, objectConceptPar0 },
 { 65571, optionConceptPar0 },
 { 65572, pConceptPar0 },
 { 65573, phConceptPar0 },
 { 65574, qConceptPar0 },
 { 65575, repsepConceptPar0 },
 { 65576, requiredcleanupConceptPar0 },
 { 65577, rowConceptPar0 },
 { 65578, simpletableConceptPar0 },
 { 65579, stentryConceptPar0 },
 { 65580, tableConceptPar0 },
 { 65581, termConceptPar0 },
 { 65582, titleConceptPar0 },
 { 65583, tmConceptPar0 },
 { 65584, uicontrolConceptPar0 },
 { 65585, varConceptPar0 },
 { 65586, xrefConceptPar0 },
 { 65587, titlefirstConceptPar0 },
 { 0, NULL }
 };

npair DTtag::ConceptElemSetNamesSet0[] = {
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
 { 65550, "*cite"},
 { 65551, "*codeph"},
 { 65552, "*desc"},
 { 65553, "*dl"},
 { 65554, "*draft-comment"},
 { 65555, "*fig"},
 { 65556, "*figgroup"},
 { 65557, "*fn"},
 { 65558, "*fragment"},
 { 65559, "*groupseq"},
 { 65560, "*image"},
 { 65561, "*indexterm"},
 { 65562, "*itemgroup"},
 { 65563, "*keyword"},
 { 65564, "*li"},
 { 65565, "*link"},
 { 65566, "*linklist"},
 { 65567, "*linkpool"},
 { 65568, "*lq"},
 { 65569, "*note"},
 { 65570, "*object"},
 { 65571, "*option"},
 { 65572, "*p"},
 { 65573, "*ph"},
 { 65574, "*q"},
 { 65575, "*repsep"},
 { 65576, "*required-cleanup"},
 { 65577, "*row"},
 { 65578, "*simpletable"},
 { 65579, "*stentry"},
 { 65580, "*table"},
 { 65581, "*term"},
 { 65582, "*title"},
 { 65583, "*tm"},
 { 65584, "*uicontrol"},
 { 65585, "*var"},
 { 65586, "*xref"},
 { 65587, "*titlefirst"},
 { 0, NULL }
 };

ipair DTtag::ConceptLevelsSet0[] = {
 { 1, 1 },
 { 2, 1 },
 { 88, 1 },
 { 92, 1 },
 { 105, 1 },
 { 128, 1 },
 { 0, 0 }
 };

ipair DTtag::ConceptAtFirstSet0[] = {
 { 22, -1 },
 { 27, 70 },
 { 30, -1 },
 { 33, -1 },
 { 47, 48 },
 { 61, -1 },
 { 68, -1 },
 { 86, -1 },
 { 103, -1 },
 { 112, -1 },
 { 127, 65587 },
 { 0, 0 }
 };


// task

npair DTtag::TaskNamesSet0[] = {
 { 1, "task"},
 { 2, "taskbody"},
 { 3, "alt"},
 { 4, "apiname"},
 { 5, "area"},
 { 6, "audience"},
 { 7, "author"},
 { 8, "b"},
 { 9, "boolean"},
 { 10, "brand"},
 { 11, "category"},
 { 12, "chdesc"},
 { 13, "chdeschd"},
 { 14, "chhead"},
 { 15, "choice"},
 { 16, "choices"},
 { 17, "choicetable"},
 { 18, "choption"},
 { 19, "choptionhd"},
 { 20, "chrow"},
 { 21, "cite"},
 { 22, "cmd"},
 { 23, "cmdname"},
 { 24, "codeblock"},
 { 25, "codeph"},
 { 26, "colspec"},
 { 27, "component"},
 { 28, "context"},
 { 29, "coords"},
 { 30, "copyrholder"},
 { 31, "copyright"},
 { 32, "copyryear"},
 { 33, "created"},
 { 34, "critdates"},
 { 35, "dd"},
 { 36, "ddhd"},
 { 37, "delim"},
 { 38, "desc"},
 { 39, "dl"},
 { 40, "dlentry"},
 { 41, "dlhead"},
 { 42, "draft-comment"},
 { 43, "dt"},
 { 44, "dthd"},
 { 45, "entry"},
 { 46, "example"},
 { 47, "featnum"},
 { 48, "fig"},
 { 49, "figgroup"},
 { 50, "filepath"},
 { 51, "fn"},
 { 52, "fragment"},
 { 53, "fragref"},
 { 54, "groupchoice"},
 { 55, "groupcomp"},
 { 56, "groupseq"},
 { 57, "i"},
 { 58, "image"},
 { 59, "imagemap"},
 { 60, "indexterm"},
 { 61, "indextermref"},
 { 62, "info"},
 { 63, "itemgroup"},
 { 64, "keyword"},
 { 65, "keywords"},
 { 66, "kwd"},
 { 67, "li"},
 { 68, "lines"},
 { 69, "link"},
 { 70, "linkinfo"},
 { 71, "linklist"},
 { 72, "linkpool"},
 { 73, "linktext"},
 { 74, "lq"},
 { 75, "menucascade"},
 { 76, "metadata"},
 { 77, "msgblock"},
 { 78, "msgnum"},
 { 79, "msgph"},
 { 80, "navtitle"},
 { 81, "note"},
 { 82, "object"},
 { 83, "ol"},
 { 84, "oper"},
 { 85, "option"},
 { 86, "othermeta"},
 { 87, "p"},
 { 88, "param"},
 { 89, "parml"},
 { 90, "parmname"},
 { 91, "pd"},
 { 92, "permissions"},
 { 93, "ph"},
 { 94, "platform"},
 { 95, "plentry"},
 { 96, "postreq"},
 { 97, "pre"},
 { 98, "prereq"},
 { 99, "prodinfo"},
 { 100, "prodname"},
 { 101, "prognum"},
 { 102, "prolog"},
 { 103, "pt"},
 { 104, "publisher"},
 { 105, "q"},
 { 106, "related-links"},
 { 107, "repsep"},
 { 108, "required-cleanup"},
 { 109, "resourceid"},
 { 110, "result"},
 { 111, "revised"},
 { 112, "row"},
 { 113, "screen"},
 { 114, "searchtitle"},
 { 115, "sep"},
 { 116, "series"},
 { 117, "shape"},
 { 118, "shortcut"},
 { 119, "shortdesc"},
 { 120, "simpletable"},
 { 121, "sl"},
 { 122, "sli"},
 { 123, "source"},
 { 124, "state"},
 { 125, "stentry"},
 { 126, "step"},
 { 127, "stepresult"},
 { 128, "steps"},
 { 129, "steps-unordered"},
 { 130, "stepxmp"},
 { 131, "sthead"},
 { 132, "strow"},
 { 133, "sub"},
 { 134, "substep"},
 { 135, "substeps"},
 { 136, "sup"},
 { 137, "synblk"},
 { 138, "synnote"},
 { 139, "synnoteref"},
 { 140, "synph"},
 { 141, "syntaxdiagram"},
 { 142, "systemoutput"},
 { 143, "table"},
 { 144, "tbody"},
 { 145, "term"},
 { 146, "tgroup"},
 { 147, "thead"},
 { 148, "title"},
 { 149, "titlealts"},
 { 150, "tm"},
 { 151, "tt"},
 { 152, "tutorialinfo"},
 { 153, "u"},
 { 154, "uicontrol"},
 { 155, "ul"},
 { 156, "userinput"},
 { 157, "var"},
 { 158, "varname"},
 { 159, "vrm"},
 { 160, "vrmlist"},
 { 161, "wintitle"},
 { 162, "xref"},
 { 0, NULL }
 };

ipair DTtag::TaskInlineSet0[] = {
 { 3, 1 },
 { 4, 3 },
 { 7, 1 },
 { 8, 3 },
 { 9, 2 },
 { 10, 1 },
 { 11, 1 },
 { 12, 1 },
 { 13, 1 },
 { 15, 1 },
 { 18, 1 },
 { 19, 1 },
 { 21, 3 },
 { 22, 1 },
 { 23, 3 },
 { 24, 7 },
 { 25, 3 },
 { 27, 1 },
 { 28, 1 },
 { 29, 1 },
 { 30, 1 },
 { 35, 1 },
 { 36, 1 },
 { 37, 1 },
 { 38, 1 },
 { 39, 2 },
 { 42, 3 },
 { 43, 1 },
 { 44, 1 },
 { 45, 1 },
 { 46, 1 },
 { 47, 1 },
 { 48, 2 },
 { 50, 3 },
 { 51, 3 },
 { 53, 1 },
 { 57, 3 },
 { 58, 2 },
 { 59, 2 },
 { 60, 3 },
 { 61, 2 },
 { 62, 1 },
 { 63, 3 },
 { 64, 3 },
 { 66, 1 },
 { 67, 1 },
 { 68, 7 },
 { 70, 1 },
 { 73, 1 },
 { 74, 3 },
 { 75, 2 },
 { 77, 7 },
 { 78, 3 },
 { 79, 3 },
 { 80, 1 },
 { 81, 3 },
 { 82, 2 },
 { 83, 2 },
 { 84, 1 },
 { 85, 3 },
 { 87, 3 },
 { 89, 2 },
 { 90, 3 },
 { 91, 1 },
 { 93, 3 },
 { 94, 1 },
 { 96, 1 },
 { 97, 7 },
 { 98, 1 },
 { 100, 1 },
 { 101, 1 },
 { 103, 1 },
 { 104, 1 },
 { 105, 3 },
 { 107, 1 },
 { 108, 2 },
 { 110, 1 },
 { 113, 7 },
 { 114, 1 },
 { 115, 1 },
 { 116, 1 },
 { 117, 1 },
 { 118, 3 },
 { 119, 1 },
 { 121, 2 },
 { 122, 1 },
 { 123, 1 },
 { 124, 2 },
 { 125, 1 },
 { 127, 1 },
 { 130, 1 },
 { 133, 3 },
 { 136, 3 },
 { 138, 1 },
 { 140, 3 },
 { 141, 2 },
 { 142, 3 },
 { 143, 2 },
 { 145, 3 },
 { 148, 1 },
 { 150, 3 },
 { 151, 3 },
 { 152, 1 },
 { 153, 3 },
 { 154, 3 },
 { 155, 2 },
 { 156, 3 },
 { 157, 1 },
 { 158, 3 },
 { 161, 3 },
 { 162, 3 },
 { 0, 0 }
 };

ipair DTtag::TaskSingleSet0[] = {
 { 2, 1 },
 { 3, 58 },
 { 4, 65567 },
 { 5, 59 },
 { 6, 76 },
 { 7, 102 },
 { 8, 65577 },
 { 9, 65578 },
 { 10, 99 },
 { 11, 76 },
 { 12, 20 },
 { 13, 14 },
 { 14, 17 },
 { 15, 16 },
 { 16, 126 },
 { 17, 126 },
 { 18, 20 },
 { 19, 14 },
 { 20, 17 },
 { 21, 65553 },
 { 22, 65554 },
 { 23, 65567 },
 { 24, 65557 },
 { 25, 65555 },
 { 26, 146 },
 { 27, 99 },
 { 28, 2 },
 { 29, 5 },
 { 30, 31 },
 { 31, 102 },
 { 32, 31 },
 { 33, 34 },
 { 34, 102 },
 { 35, 40 },
 { 36, 41 },
 { 37, 65589 },
 { 38, 65556 },
 { 39, 65557 },
 { 40, 39 },
 { 41, 39 },
 { 42, 65558 },
 { 43, 40 },
 { 44, 41 },
 { 45, 112 },
 { 46, 2 },
 { 47, 99 },
 { 48, 65559 },
 { 49, 65560 },
 { 50, 65577 },
 { 51, 65561 },
 { 52, 65562 },
 { 53, 65563 },
 { 54, 65563 },
 { 55, 65563 },
 { 56, 65563 },
 { 57, 65577 },
 { 58, 65564 },
 { 59, 65559 },
 { 60, 65565 },
 { 61, 65558 },
 { 62, 65554 },
 { 63, 65566 },
 { 64, 65567 },
 { 65, 76 },
 { 66, 65589 },
 { 67, 65568 },
 { 68, 65557 },
 { 69, 65569 },
 { 70, 71 },
 { 71, 65570 },
 { 72, 65571 },
 { 73, 69 },
 { 74, 65572 },
 { 75, 65577 },
 { 76, 102 },
 { 77, 65557 },
 { 78, 65567 },
 { 79, 65577 },
 { 80, 149 },
 { 81, 65573 },
 { 82, 65574 },
 { 83, 65557 },
 { 84, 65589 },
 { 85, 65575 },
 { 86, 76 },
 { 87, 65576 },
 { 88, 82 },
 { 89, 65557 },
 { 90, 65575 },
 { 91, 95 },
 { 92, 102 },
 { 93, 65577 },
 { 94, 99 },
 { 95, 89 },
 { 96, 2 },
 { 97, 65557 },
 { 98, 2 },
 { 99, 76 },
 { 100, 99 },
 { 101, 99 },
 { 102, 1 },
 { 103, 95 },
 { 104, 102 },
 { 105, 65578 },
 { 106, 1 },
 { 107, 65579 },
 { 108, 65558 },
 { 109, 102 },
 { 110, 2 },
 { 111, 34 },
 { 112, 65580 },
 { 113, 65557 },
 { 114, 149 },
 { 115, 65589 },
 { 116, 99 },
 { 117, 5 },
 { 118, 154 },
 { 119, 1 },
 { 120, 65581 },
 { 121, 65557 },
 { 122, 121 },
 { 123, 102 },
 { 124, 65578 },
 { 125, 65582 },
 { 126, 65583 },
 { 127, 65554 },
 { 128, 2 },
 { 129, 2 },
 { 130, 65554 },
 { 131, 120 },
 { 132, 120 },
 { 133, 65577 },
 { 134, 135 },
 { 135, 126 },
 { 136, 65577 },
 { 137, 141 },
 { 138, 65563 },
 { 139, 65563 },
 { 140, 65555 },
 { 141, 65559 },
 { 142, 65577 },
 { 143, 65584 },
 { 144, 146 },
 { 145, 65585 },
 { 146, 143 },
 { 147, 146 },
 { 148, 65586 },
 { 149, 1 },
 { 150, 65587 },
 { 151, 65577 },
 { 152, 65554 },
 { 153, 65577 },
 { 154, 65588 },
 { 155, 65557 },
 { 156, 65577 },
 { 157, 65589 },
 { 158, 65567 },
 { 159, 160 },
 { 160, 99 },
 { 161, 65567 },
 { 162, 65590 },
 { 0, 0 }
 };

long DTtag::Part1TaskPar0[] = { 
52, 54, 55, 56, 137, 141, 0 };

long DTtag::Part2TaskPar0[] = { 
8, 24, 25, 46, 57, 68, 74, 81, 
87, 93, 97, 105, 113, 119, 133, 136, 
148, 151, 153, 162, 0 };

long DTtag::Part3TaskPar0[] = { 
21, 22, 28, 35, 36, 38, 42, 43, 
44, 45, 49, 51, 63, 67, 70, 96, 
98, 110, 122, 125, 0 };

long DTtag::Part4TaskPar0[] = { 
12, 13, 15, 18, 19, 53, 62, 91, 
103, 127, 130, 138, 152, 0 };

long DTtag::Part5TaskPar0[] = { 
3, 7, 10, 11, 30, 50, 60, 65, 
73, 77, 79, 80, 100, 104, 114, 116, 
123, 142, 154, 156, 0 };

long DTtag::Part6TaskPar0[] = { 
27, 29, 37, 47, 84, 94, 101, 107, 
115, 157, 0 };

long DTtag::Part7TaskPar0[] = { 
21, 22, 28, 35, 36, 38, 42, 43, 
44, 45, 51, 63, 67, 70, 96, 98, 
110, 122, 125, 0 };

long DTtag::Part8TaskPar0[] = { 
46, 74, 81, 87, 93, 119, 148, 162, 
0 };

long DTtag::Part9TaskPar0[] = { 
8, 57, 68, 97, 105, 133, 136, 151, 
153, 0 };

long DTtag::Part10TaskPar0[] = { 
22, 28, 35, 38, 42, 43, 45, 51, 
62, 63, 67, 70, 96, 98, 110, 122, 
125, 127, 130, 152, 0 };

long DTtag::Part11TaskPar0[] = { 
12, 13, 15, 18, 19, 91, 103, 0 };

long DTtag::Part12TaskPar0[] = { 
24, 25, 46, 74, 81, 87, 93, 113, 
138, 0 };

long DTtag::Part13TaskPar0[] = { 
28, 35, 38, 42, 45, 46, 48, 51, 
62, 63, 67, 70, 81, 96, 98, 110, 
125, 127, 130, 152, 0 };

long DTtag::Part14TaskPar0[] = { 
12, 13, 15, 18, 19, 91, 0 };

long DTtag::Part15TaskPar0[] = { 
28, 35, 45, 46, 62, 63, 67, 74, 
87, 96, 98, 110, 125, 127, 130, 152, 
0 };

long DTtag::Part16TaskPar0[] = { 
22, 24, 68, 81, 93, 97, 113, 122, 
0 };

long DTtag::Part17TaskPar0[] = { 
15, 28, 35, 42, 46, 62, 63, 67, 
74, 81, 87, 91, 96, 98, 110, 127, 
130, 152, 0 };

long DTtag::citeTaskPar0[] = { 
65544, 65545, 65546, 65547, 0 };

long DTtag::cmdTaskPar0[] = { 
126, 134, 0 };

long DTtag::codephTaskPar0[] = { 
140, 65537, 65538, 65539, 0 };

long DTtag::descTaskPar0[] = { 
48, 69, 71, 82, 143, 162, 0 };

long DTtag::dlTaskPar0[] = { 
74, 87, 65548, 65549, 0 };

long DTtag::draftcommentTaskPar0[] = { 
65549, 65550, 65551, 0 };

long DTtag::figTaskPar0[] = { 
42, 51, 81, 65549, 65550, 0 };

long DTtag::figgroupTaskPar0[] = { 
48, 49, 0 };

long DTtag::fnTaskPar0[] = { 
49, 65549, 65550, 65551, 0 };

long DTtag::fragmentTaskPar0[] = { 
137, 141, 0 };

long DTtag::groupseqTaskPar0[] = { 
65536, 0 };

long DTtag::imageTaskPar0[] = { 
36, 44, 48, 59, 154, 65543, 65545, 65546, 
0 };

long DTtag::indextermTaskPar0[] = { 
60, 65, 65549, 65550, 65551, 0 };

long DTtag::itemgroupTaskPar0[] = { 
15, 35, 67, 91, 0 };

long DTtag::keywordTaskPar0[] = { 
65537, 65538, 65539, 65540, 65541, 0 };

long DTtag::liTaskPar0[] = { 
83, 155, 0 };

long DTtag::linkTaskPar0[] = { 
71, 72, 106, 0 };

long DTtag::linklistTaskPar0[] = { 
71, 106, 0 };

long DTtag::linkpoolTaskPar0[] = { 
72, 106, 0 };

long DTtag::lqTaskPar0[] = { 
87, 65548, 65549, 0 };

long DTtag::noteTaskPar0[] = { 
38, 42, 48, 51, 70, 65549, 65550, 0 };

long DTtag::objectTaskPar0[] = { 
42, 48, 51, 81, 65549, 65550, 0 };

long DTtag::optionTaskPar0[] = { 
140, 65537, 65538, 65539, 65540, 65541, 0 };

long DTtag::pTaskPar0[] = { 
74, 65548, 65549, 0 };

long DTtag::phTaskPar0[] = { 
65537, 65538, 65539, 0 };

long DTtag::qTaskPar0[] = { 
65537, 65539, 65542, 0 };

long DTtag::repsepTaskPar0[] = { 
54, 55, 56, 0 };

long DTtag::rowTaskPar0[] = { 
144, 147, 0 };

long DTtag::simpletableTaskPar0[] = { 
48, 65552, 0 };

long DTtag::stentryTaskPar0[] = { 
131, 132, 0 };

long DTtag::stepTaskPar0[] = { 
128, 129, 0 };

long DTtag::tableTaskPar0[] = { 
65552, 0 };

long DTtag::termTaskPar0[] = { 
3, 7, 10, 11, 30, 50, 60, 73, 
77, 79, 80, 100, 104, 114, 116, 123, 
142, 154, 156, 65537, 65539, 65541, 65542, 0 };

long DTtag::titleTaskPar0[] = { 
46, 48, 49, 71, 143, 1, 65536, 0 };

long DTtag::tmTaskPar0[] = { 
64, 145, 150, 65539, 65542, 65543, 65544, 0 };

long DTtag::uicontrolTaskPar0[] = { 
75, 65537, 65538, 65539, 0 };

long DTtag::varTaskPar0[] = { 
54, 55, 56, 140, 0 };

long DTtag::xrefTaskPar0[] = { 
5, 49, 65544, 65545, 65546, 65547, 0 };

long DTtag::titlefirstTaskPar0[] = { 
48, 49, 71, 143, 1, 65536, 0 };

vpair DTtag::TaskElemSetsSet0[] = {
 { 65536, Part1TaskPar0 },
 { 65537, Part2TaskPar0 },
 { 65538, Part3TaskPar0 },
 { 65539, Part4TaskPar0 },
 { 65540, Part5TaskPar0 },
 { 65541, Part6TaskPar0 },
 { 65542, Part7TaskPar0 },
 { 65543, Part8TaskPar0 },
 { 65544, Part9TaskPar0 },
 { 65545, Part10TaskPar0 },
 { 65546, Part11TaskPar0 },
 { 65547, Part12TaskPar0 },
 { 65548, Part13TaskPar0 },
 { 65549, Part14TaskPar0 },
 { 65550, Part15TaskPar0 },
 { 65551, Part16TaskPar0 },
 { 65552, Part17TaskPar0 },
 { 65553, citeTaskPar0 },
 { 65554, cmdTaskPar0 },
 { 65555, codephTaskPar0 },
 { 65556, descTaskPar0 },
 { 65557, dlTaskPar0 },
 { 65558, draftcommentTaskPar0 },
 { 65559, figTaskPar0 },
 { 65560, figgroupTaskPar0 },
 { 65561, fnTaskPar0 },
 { 65562, fragmentTaskPar0 },
 { 65563, groupseqTaskPar0 },
 { 65564, imageTaskPar0 },
 { 65565, indextermTaskPar0 },
 { 65566, itemgroupTaskPar0 },
 { 65567, keywordTaskPar0 },
 { 65568, liTaskPar0 },
 { 65569, linkTaskPar0 },
 { 65570, linklistTaskPar0 },
 { 65571, linkpoolTaskPar0 },
 { 65572, lqTaskPar0 },
 { 65573, noteTaskPar0 },
 { 65574, objectTaskPar0 },
 { 65575, optionTaskPar0 },
 { 65576, pTaskPar0 },
 { 65577, phTaskPar0 },
 { 65578, qTaskPar0 },
 { 65579, repsepTaskPar0 },
 { 65580, rowTaskPar0 },
 { 65581, simpletableTaskPar0 },
 { 65582, stentryTaskPar0 },
 { 65583, stepTaskPar0 },
 { 65584, tableTaskPar0 },
 { 65585, termTaskPar0 },
 { 65586, titleTaskPar0 },
 { 65587, tmTaskPar0 },
 { 65588, uicontrolTaskPar0 },
 { 65589, varTaskPar0 },
 { 65590, xrefTaskPar0 },
 { 65591, titlefirstTaskPar0 },
 { 0, NULL }
 };

npair DTtag::TaskElemSetNamesSet0[] = {
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
 { 65554, "*cmd"},
 { 65555, "*codeph"},
 { 65556, "*desc"},
 { 65557, "*dl"},
 { 65558, "*draft-comment"},
 { 65559, "*fig"},
 { 65560, "*figgroup"},
 { 65561, "*fn"},
 { 65562, "*fragment"},
 { 65563, "*groupseq"},
 { 65564, "*image"},
 { 65565, "*indexterm"},
 { 65566, "*itemgroup"},
 { 65567, "*keyword"},
 { 65568, "*li"},
 { 65569, "*link"},
 { 65570, "*linklist"},
 { 65571, "*linkpool"},
 { 65572, "*lq"},
 { 65573, "*note"},
 { 65574, "*object"},
 { 65575, "*option"},
 { 65576, "*p"},
 { 65577, "*ph"},
 { 65578, "*q"},
 { 65579, "*repsep"},
 { 65580, "*row"},
 { 65581, "*simpletable"},
 { 65582, "*stentry"},
 { 65583, "*step"},
 { 65584, "*table"},
 { 65585, "*term"},
 { 65586, "*title"},
 { 65587, "*tm"},
 { 65588, "*uicontrol"},
 { 65589, "*var"},
 { 65590, "*xref"},
 { 65591, "*titlefirst"},
 { 0, NULL }
 };

ipair DTtag::TaskLevelsSet0[] = {
 { 1, 1 },
 { 2, 1 },
 { 102, 1 },
 { 106, 1 },
 { 119, 1 },
 { 149, 1 },
 { 0, 0 }
 };

ipair DTtag::TaskAtFirstSet0[] = {
 { 14, -1 },
 { 18, -1 },
 { 19, -1 },
 { 22, -1 },
 { 33, -1 },
 { 38, 82 },
 { 41, -1 },
 { 44, -1 },
 { 58, 59 },
 { 73, -1 },
 { 80, -1 },
 { 98, -1 },
 { 100, -1 },
 { 117, -1 },
 { 131, -1 },
 { 148, 65591 },
 { 0, 0 }
 };


// reference

npair DTtag::ReferenceNamesSet0[] = {
 { 1, "reference"},
 { 2, "refbody"},
 { 3, "alt"},
 { 4, "apiname"},
 { 5, "area"},
 { 6, "audience"},
 { 7, "author"},
 { 8, "b"},
 { 9, "boolean"},
 { 10, "brand"},
 { 11, "category"},
 { 12, "cite"},
 { 13, "cmdname"},
 { 14, "codeblock"},
 { 15, "codeph"},
 { 16, "colspec"},
 { 17, "component"},
 { 18, "coords"},
 { 19, "copyrholder"},
 { 20, "copyright"},
 { 21, "copyryear"},
 { 22, "created"},
 { 23, "critdates"},
 { 24, "dd"},
 { 25, "ddhd"},
 { 26, "delim"},
 { 27, "desc"},
 { 28, "dl"},
 { 29, "dlentry"},
 { 30, "dlhead"},
 { 31, "draft-comment"},
 { 32, "dt"},
 { 33, "dthd"},
 { 34, "entry"},
 { 35, "example"},
 { 36, "featnum"},
 { 37, "fig"},
 { 38, "figgroup"},
 { 39, "filepath"},
 { 40, "fn"},
 { 41, "fragment"},
 { 42, "fragref"},
 { 43, "groupchoice"},
 { 44, "groupcomp"},
 { 45, "groupseq"},
 { 46, "i"},
 { 47, "image"},
 { 48, "imagemap"},
 { 49, "indexterm"},
 { 50, "indextermref"},
 { 51, "itemgroup"},
 { 52, "keyword"},
 { 53, "keywords"},
 { 54, "kwd"},
 { 55, "li"},
 { 56, "lines"},
 { 57, "link"},
 { 58, "linkinfo"},
 { 59, "linklist"},
 { 60, "linkpool"},
 { 61, "linktext"},
 { 62, "lq"},
 { 63, "menucascade"},
 { 64, "metadata"},
 { 65, "msgblock"},
 { 66, "msgnum"},
 { 67, "msgph"},
 { 68, "navtitle"},
 { 69, "note"},
 { 70, "object"},
 { 71, "ol"},
 { 72, "oper"},
 { 73, "option"},
 { 74, "othermeta"},
 { 75, "p"},
 { 76, "param"},
 { 77, "parml"},
 { 78, "parmname"},
 { 79, "pd"},
 { 80, "permissions"},
 { 81, "ph"},
 { 82, "platform"},
 { 83, "plentry"},
 { 84, "pre"},
 { 85, "prodinfo"},
 { 86, "prodname"},
 { 87, "prognum"},
 { 88, "prolog"},
 { 89, "propdesc"},
 { 90, "propdeschd"},
 { 91, "properties"},
 { 92, "property"},
 { 93, "prophead"},
 { 94, "proptype"},
 { 95, "proptypehd"},
 { 96, "propvalue"},
 { 97, "propvaluehd"},
 { 98, "pt"},
 { 99, "publisher"},
 { 100, "q"},
 { 101, "refsyn"},
 { 102, "related-links"},
 { 103, "repsep"},
 { 104, "required-cleanup"},
 { 105, "resourceid"},
 { 106, "revised"},
 { 107, "row"},
 { 108, "screen"},
 { 109, "searchtitle"},
 { 110, "section"},
 { 111, "sep"},
 { 112, "series"},
 { 113, "shape"},
 { 114, "shortcut"},
 { 115, "shortdesc"},
 { 116, "simpletable"},
 { 117, "sl"},
 { 118, "sli"},
 { 119, "source"},
 { 120, "state"},
 { 121, "stentry"},
 { 122, "sthead"},
 { 123, "strow"},
 { 124, "sub"},
 { 125, "sup"},
 { 126, "synblk"},
 { 127, "synnote"},
 { 128, "synnoteref"},
 { 129, "synph"},
 { 130, "syntaxdiagram"},
 { 131, "systemoutput"},
 { 132, "table"},
 { 133, "tbody"},
 { 134, "term"},
 { 135, "tgroup"},
 { 136, "thead"},
 { 137, "title"},
 { 138, "titlealts"},
 { 139, "tm"},
 { 140, "tt"},
 { 141, "u"},
 { 142, "uicontrol"},
 { 143, "ul"},
 { 144, "userinput"},
 { 145, "var"},
 { 146, "varname"},
 { 147, "vrm"},
 { 148, "vrmlist"},
 { 149, "wintitle"},
 { 150, "xref"},
 { 0, NULL }
 };

ipair DTtag::ReferenceInlineSet0[] = {
 { 3, 1 },
 { 4, 3 },
 { 7, 1 },
 { 8, 3 },
 { 9, 2 },
 { 10, 1 },
 { 11, 1 },
 { 12, 3 },
 { 13, 3 },
 { 14, 5 },
 { 15, 3 },
 { 17, 1 },
 { 18, 1 },
 { 19, 1 },
 { 24, 1 },
 { 25, 1 },
 { 26, 1 },
 { 27, 1 },
 { 31, 3 },
 { 32, 1 },
 { 33, 1 },
 { 34, 1 },
 { 35, 1 },
 { 36, 1 },
 { 37, 2 },
 { 39, 3 },
 { 40, 3 },
 { 42, 1 },
 { 46, 3 },
 { 47, 2 },
 { 48, 2 },
 { 49, 3 },
 { 50, 2 },
 { 51, 3 },
 { 52, 3 },
 { 54, 1 },
 { 55, 1 },
 { 56, 5 },
 { 58, 1 },
 { 61, 1 },
 { 62, 1 },
 { 63, 2 },
 { 65, 5 },
 { 66, 3 },
 { 67, 3 },
 { 68, 1 },
 { 69, 1 },
 { 72, 1 },
 { 73, 3 },
 { 75, 1 },
 { 78, 3 },
 { 79, 1 },
 { 81, 3 },
 { 82, 1 },
 { 84, 5 },
 { 86, 1 },
 { 87, 1 },
 { 89, 1 },
 { 90, 1 },
 { 94, 1 },
 { 95, 1 },
 { 96, 1 },
 { 97, 1 },
 { 98, 1 },
 { 99, 1 },
 { 100, 3 },
 { 101, 1 },
 { 103, 1 },
 { 104, 2 },
 { 108, 5 },
 { 109, 1 },
 { 110, 1 },
 { 111, 1 },
 { 112, 1 },
 { 113, 1 },
 { 114, 3 },
 { 115, 1 },
 { 118, 1 },
 { 119, 1 },
 { 120, 2 },
 { 121, 1 },
 { 124, 3 },
 { 125, 3 },
 { 127, 1 },
 { 129, 3 },
 { 130, 2 },
 { 131, 3 },
 { 134, 3 },
 { 137, 1 },
 { 139, 3 },
 { 140, 3 },
 { 141, 3 },
 { 142, 3 },
 { 144, 3 },
 { 145, 1 },
 { 146, 3 },
 { 149, 3 },
 { 150, 3 },
 { 0, 0 }
 };

ipair DTtag::ReferenceSingleSet0[] = {
 { 2, 1 },
 { 3, 47 },
 { 4, 65564 },
 { 5, 48 },
 { 6, 64 },
 { 7, 88 },
 { 8, 65574 },
 { 9, 65575 },
 { 10, 85 },
 { 11, 64 },
 { 12, 65551 },
 { 13, 65564 },
 { 14, 65554 },
 { 15, 65552 },
 { 16, 135 },
 { 17, 85 },
 { 18, 5 },
 { 19, 20 },
 { 20, 88 },
 { 21, 20 },
 { 22, 23 },
 { 23, 88 },
 { 24, 29 },
 { 25, 30 },
 { 26, 65585 },
 { 27, 65553 },
 { 28, 65554 },
 { 29, 28 },
 { 30, 28 },
 { 31, 65555 },
 { 32, 29 },
 { 33, 30 },
 { 34, 107 },
 { 35, 2 },
 { 36, 85 },
 { 37, 65556 },
 { 38, 65557 },
 { 39, 65574 },
 { 40, 65558 },
 { 41, 65559 },
 { 42, 65560 },
 { 43, 65560 },
 { 44, 65560 },
 { 45, 65560 },
 { 46, 65574 },
 { 47, 65561 },
 { 48, 65556 },
 { 49, 65562 },
 { 50, 65555 },
 { 51, 65563 },
 { 52, 65564 },
 { 53, 64 },
 { 54, 65585 },
 { 55, 65565 },
 { 56, 65554 },
 { 57, 65566 },
 { 58, 59 },
 { 59, 65567 },
 { 60, 65568 },
 { 61, 57 },
 { 62, 65569 },
 { 63, 65574 },
 { 64, 88 },
 { 65, 65554 },
 { 66, 65564 },
 { 67, 65574 },
 { 68, 138 },
 { 69, 65570 },
 { 70, 65571 },
 { 71, 65554 },
 { 72, 65585 },
 { 73, 65572 },
 { 74, 64 },
 { 75, 65573 },
 { 76, 70 },
 { 77, 65554 },
 { 78, 65572 },
 { 79, 83 },
 { 80, 88 },
 { 81, 65574 },
 { 82, 85 },
 { 83, 77 },
 { 84, 65554 },
 { 85, 64 },
 { 86, 85 },
 { 87, 85 },
 { 88, 1 },
 { 89, 92 },
 { 90, 93 },
 { 91, 2 },
 { 92, 91 },
 { 93, 91 },
 { 94, 92 },
 { 95, 93 },
 { 96, 92 },
 { 97, 93 },
 { 98, 83 },
 { 99, 88 },
 { 100, 65575 },
 { 101, 2 },
 { 102, 1 },
 { 103, 65576 },
 { 104, 65555 },
 { 105, 88 },
 { 106, 23 },
 { 107, 65577 },
 { 108, 65554 },
 { 109, 138 },
 { 110, 2 },
 { 111, 65585 },
 { 112, 85 },
 { 113, 5 },
 { 114, 142 },
 { 115, 1 },
 { 116, 65578 },
 { 117, 65554 },
 { 118, 117 },
 { 119, 88 },
 { 120, 65575 },
 { 121, 65579 },
 { 122, 116 },
 { 123, 116 },
 { 124, 65574 },
 { 125, 65574 },
 { 126, 130 },
 { 127, 65560 },
 { 128, 65560 },
 { 129, 65552 },
 { 130, 65556 },
 { 131, 65574 },
 { 132, 65580 },
 { 133, 135 },
 { 134, 65581 },
 { 135, 132 },
 { 136, 135 },
 { 137, 65582 },
 { 138, 1 },
 { 139, 65583 },
 { 140, 65574 },
 { 141, 65574 },
 { 142, 65584 },
 { 143, 65554 },
 { 144, 65574 },
 { 145, 65585 },
 { 146, 65564 },
 { 147, 148 },
 { 148, 85 },
 { 149, 65564 },
 { 150, 65586 },
 { 0, 0 }
 };

long DTtag::Part1ReferencePar0[] = { 
41, 43, 44, 45, 126, 130, 0 };

long DTtag::Part2ReferencePar0[] = { 
8, 14, 15, 35, 46, 56, 62, 69, 
75, 81, 84, 100, 108, 110, 115, 124, 
125, 137, 140, 141, 0 };

long DTtag::Part3ReferencePar0[] = { 
12, 24, 25, 27, 31, 32, 33, 34, 
38, 40, 51, 55, 58, 90, 95, 97, 
101, 118, 121, 150, 0 };

long DTtag::Part4ReferencePar0[] = { 
42, 79, 89, 94, 96, 98, 127, 0 };

long DTtag::Part5ReferencePar0[] = { 
3, 7, 10, 11, 19, 39, 49, 53, 
61, 65, 67, 68, 86, 99, 109, 112, 
119, 131, 142, 144, 0 };

long DTtag::Part6ReferencePar0[] = { 
17, 18, 26, 36, 72, 82, 87, 103, 
111, 145, 0 };

long DTtag::Part7ReferencePar0[] = { 
12, 24, 25, 27, 31, 32, 33, 34, 
40, 51, 55, 58, 90, 95, 97, 101, 
118, 121, 150, 0 };

long DTtag::Part8ReferencePar0[] = { 
35, 62, 69, 75, 81, 110, 115, 137, 
0 };

long DTtag::Part9ReferencePar0[] = { 
8, 46, 56, 84, 100, 124, 125, 140, 
141, 0 };

long DTtag::Part10ReferencePar0[] = { 
24, 27, 31, 32, 34, 40, 51, 55, 
58, 79, 89, 90, 94, 95, 96, 97, 
98, 101, 118, 121, 0 };

long DTtag::Part11ReferencePar0[] = { 
14, 15, 35, 62, 69, 75, 81, 108, 
110, 127, 0 };

long DTtag::Part12ReferencePar0[] = { 
24, 27, 31, 34, 35, 37, 40, 51, 
55, 58, 69, 79, 89, 90, 95, 97, 
101, 110, 121, 0 };

long DTtag::Part13ReferencePar0[] = { 
24, 34, 35, 51, 55, 62, 75, 79, 
90, 95, 97, 101, 110, 121, 0 };

long DTtag::Part14ReferencePar0[] = { 
14, 56, 69, 81, 84, 94, 96, 108, 
118, 0 };

long DTtag::Part15ReferencePar0[] = { 
24, 31, 35, 51, 55, 62, 69, 75, 
79, 2, 101, 110, 0 };

long DTtag::citeReferencePar0[] = { 
65544, 65545, 65546, 0 };

long DTtag::codephReferencePar0[] = { 
129, 65537, 65538, 65539, 0 };

long DTtag::descReferencePar0[] = { 
37, 57, 59, 70, 132, 150, 0 };

long DTtag::dlReferencePar0[] = { 
62, 75, 65547, 0 };

long DTtag::draftcommentReferencePar0[] = { 
65548, 65549, 0 };

long DTtag::figReferencePar0[] = { 
31, 40, 69, 65548, 0 };

long DTtag::figgroupReferencePar0[] = { 
37, 38, 0 };

long DTtag::fnReferencePar0[] = { 
38, 65548, 65549, 0 };

long DTtag::fragmentReferencePar0[] = { 
126, 130, 0 };

long DTtag::groupseqReferencePar0[] = { 
65536, 0 };

long DTtag::imageReferencePar0[] = { 
25, 33, 37, 48, 142, 150, 65543, 65545, 
0 };

long DTtag::indextermReferencePar0[] = { 
49, 53, 65548, 65549, 0 };

long DTtag::itemgroupReferencePar0[] = { 
24, 55, 79, 0 };

long DTtag::keywordReferencePar0[] = { 
65537, 65538, 65539, 65540, 65541, 0 };

long DTtag::liReferencePar0[] = { 
71, 143, 0 };

long DTtag::linkReferencePar0[] = { 
59, 60, 102, 0 };

long DTtag::linklistReferencePar0[] = { 
59, 102, 0 };

long DTtag::linkpoolReferencePar0[] = { 
60, 102, 0 };

long DTtag::lqReferencePar0[] = { 
75, 65547, 0 };

long DTtag::noteReferencePar0[] = { 
27, 31, 37, 40, 58, 89, 65548, 0 };

long DTtag::objectReferencePar0[] = { 
31, 37, 40, 69, 65548, 0 };

long DTtag::optionReferencePar0[] = { 
129, 65537, 65538, 65539, 65540, 65541, 0 };

long DTtag::pReferencePar0[] = { 
62, 65547, 0 };

long DTtag::phReferencePar0[] = { 
65537, 65538, 65539, 0 };

long DTtag::qReferencePar0[] = { 
65537, 65539, 65542, 0 };

long DTtag::repsepReferencePar0[] = { 
43, 44, 45, 0 };

long DTtag::rowReferencePar0[] = { 
133, 136, 0 };

long DTtag::simpletableReferencePar0[] = { 
37, 65550, 0 };

long DTtag::stentryReferencePar0[] = { 
122, 123, 0 };

long DTtag::tableReferencePar0[] = { 
65550, 0 };

long DTtag::termReferencePar0[] = { 
3, 7, 10, 11, 19, 39, 49, 61, 
65, 67, 68, 86, 99, 109, 112, 119, 
131, 142, 144, 65537, 65539, 65541, 65542, 0 };

long DTtag::titleReferencePar0[] = { 
35, 37, 38, 59, 1, 101, 110, 132, 
65536, 0 };

long DTtag::tmReferencePar0[] = { 
52, 134, 139, 65539, 65542, 65543, 65544, 0 };

long DTtag::uicontrolReferencePar0[] = { 
63, 65537, 65538, 65539, 0 };

long DTtag::varReferencePar0[] = { 
43, 44, 45, 129, 0 };

long DTtag::xrefReferencePar0[] = { 
5, 38, 65544, 65545, 65546, 0 };

long DTtag::titlefirstReferencePar0[] = { 
37, 38, 59, 1, 132, 65536, 0 };

vpair DTtag::ReferenceElemSetsSet0[] = {
 { 65536, Part1ReferencePar0 },
 { 65537, Part2ReferencePar0 },
 { 65538, Part3ReferencePar0 },
 { 65539, Part4ReferencePar0 },
 { 65540, Part5ReferencePar0 },
 { 65541, Part6ReferencePar0 },
 { 65542, Part7ReferencePar0 },
 { 65543, Part8ReferencePar0 },
 { 65544, Part9ReferencePar0 },
 { 65545, Part10ReferencePar0 },
 { 65546, Part11ReferencePar0 },
 { 65547, Part12ReferencePar0 },
 { 65548, Part13ReferencePar0 },
 { 65549, Part14ReferencePar0 },
 { 65550, Part15ReferencePar0 },
 { 65551, citeReferencePar0 },
 { 65552, codephReferencePar0 },
 { 65553, descReferencePar0 },
 { 65554, dlReferencePar0 },
 { 65555, draftcommentReferencePar0 },
 { 65556, figReferencePar0 },
 { 65557, figgroupReferencePar0 },
 { 65558, fnReferencePar0 },
 { 65559, fragmentReferencePar0 },
 { 65560, groupseqReferencePar0 },
 { 65561, imageReferencePar0 },
 { 65562, indextermReferencePar0 },
 { 65563, itemgroupReferencePar0 },
 { 65564, keywordReferencePar0 },
 { 65565, liReferencePar0 },
 { 65566, linkReferencePar0 },
 { 65567, linklistReferencePar0 },
 { 65568, linkpoolReferencePar0 },
 { 65569, lqReferencePar0 },
 { 65570, noteReferencePar0 },
 { 65571, objectReferencePar0 },
 { 65572, optionReferencePar0 },
 { 65573, pReferencePar0 },
 { 65574, phReferencePar0 },
 { 65575, qReferencePar0 },
 { 65576, repsepReferencePar0 },
 { 65577, rowReferencePar0 },
 { 65578, simpletableReferencePar0 },
 { 65579, stentryReferencePar0 },
 { 65580, tableReferencePar0 },
 { 65581, termReferencePar0 },
 { 65582, titleReferencePar0 },
 { 65583, tmReferencePar0 },
 { 65584, uicontrolReferencePar0 },
 { 65585, varReferencePar0 },
 { 65586, xrefReferencePar0 },
 { 65587, titlefirstReferencePar0 },
 { 0, NULL }
 };

npair DTtag::ReferenceElemSetNamesSet0[] = {
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
 { 65553, "*desc"},
 { 65554, "*dl"},
 { 65555, "*draft-comment"},
 { 65556, "*fig"},
 { 65557, "*figgroup"},
 { 65558, "*fn"},
 { 65559, "*fragment"},
 { 65560, "*groupseq"},
 { 65561, "*image"},
 { 65562, "*indexterm"},
 { 65563, "*itemgroup"},
 { 65564, "*keyword"},
 { 65565, "*li"},
 { 65566, "*link"},
 { 65567, "*linklist"},
 { 65568, "*linkpool"},
 { 65569, "*lq"},
 { 65570, "*note"},
 { 65571, "*object"},
 { 65572, "*option"},
 { 65573, "*p"},
 { 65574, "*ph"},
 { 65575, "*q"},
 { 65576, "*repsep"},
 { 65577, "*row"},
 { 65578, "*simpletable"},
 { 65579, "*stentry"},
 { 65580, "*table"},
 { 65581, "*term"},
 { 65582, "*title"},
 { 65583, "*tm"},
 { 65584, "*uicontrol"},
 { 65585, "*var"},
 { 65586, "*xref"},
 { 65587, "*titlefirst"},
 { 0, NULL }
 };

ipair DTtag::ReferenceLevelsSet0[] = {
 { 1, 1 },
 { 2, 1 },
 { 88, 1 },
 { 102, 1 },
 { 115, 1 },
 { 138, 1 },
 { 0, 0 }
 };

ipair DTtag::ReferenceAtFirstSet0[] = {
 { 22, -1 },
 { 27, 70 },
 { 30, -1 },
 { 33, -1 },
 { 47, 48 },
 { 61, -1 },
 { 68, -1 },
 { 86, -1 },
 { 93, -1 },
 { 94, -1 },
 { 95, -1 },
 { 113, -1 },
 { 122, -1 },
 { 137, 65587 },
 { 0, 0 }
 };


// maps

npair DTtag::MapNamesSet0[] = {
 { 1, "map"},
 { 3, "anchor"},
 { 4, "audience"},
 { 5, "author"},
 { 6, "brand"},
 { 7, "category"},
 { 8, "component"},
 { 9, "copyrholder"},
 { 10, "copyright"},
 { 11, "copyryear"},
 { 12, "created"},
 { 13, "critdates"},
 { 14, "featnum"},
 { 15, "indexterm"},
 { 16, "keyword"},
 { 17, "keywords"},
 { 18, "linktext"},
 { 19, "navref"},
 { 20, "othermeta"},
 { 21, "permissions"},
 { 22, "platform"},
 { 23, "prodinfo"},
 { 24, "prodname"},
 { 25, "prognum"},
 { 26, "publisher"},
 { 27, "relcell"},
 { 28, "relcolspec"},
 { 29, "relheader"},
 { 30, "relrow"},
 { 31, "reltable"},
 { 32, "resourceid"},
 { 33, "revised"},
 { 34, "searchtitle"},
 { 35, "series"},
 { 36, "shortdesc"},
 { 37, "source"},
 { 38, "topicgroup"},
 { 39, "topichead"},
 { 40, "topicmeta"},
 { 41, "topicref"},
 { 42, "vrm"},
 { 43, "vrmlist"},
 { 0, NULL }
 };

ipair DTtag::MapInlineSet0[] = {
 { 5, 1 },
 { 6, 1 },
 { 7, 1 },
 { 8, 1 },
 { 9, 1 },
 { 14, 1 },
 { 15, 1 },
 { 16, 3 },
 { 18, 1 },
 { 22, 1 },
 { 24, 1 },
 { 25, 1 },
 { 26, 1 },
 { 34, 1 },
 { 35, 1 },
 { 36, 1 },
 { 37, 1 },
 { 0, 0 }
 };

ipair DTtag::MapSingleSet0[] = {
 { 3, 65538 },
 { 4, 40 },
 { 5, 40 },
 { 6, 23 },
 { 7, 40 },
 { 8, 23 },
 { 9, 10 },
 { 10, 40 },
 { 11, 10 },
 { 12, 13 },
 { 13, 40 },
 { 14, 23 },
 { 15, 65536 },
 { 16, 65537 },
 { 17, 40 },
 { 18, 40 },
 { 19, 65538 },
 { 20, 40 },
 { 21, 40 },
 { 22, 23 },
 { 23, 40 },
 { 24, 23 },
 { 25, 23 },
 { 26, 40 },
 { 27, 30 },
 { 28, 29 },
 { 29, 31 },
 { 30, 31 },
 { 31, 1 },
 { 32, 40 },
 { 33, 13 },
 { 34, 40 },
 { 35, 23 },
 { 36, 40 },
 { 37, 40 },
 { 38, 65540 },
 { 39, 65540 },
 { 40, 65539 },
 { 41, 65540 },
 { 42, 43 },
 { 43, 23 },
 { 0, 0 }
 };

long DTtag::indextermMapPar0[] = { 
15, 17, 0 };

long DTtag::keywordMapPar0[] = { 
5, 6, 7, 8, 9, 14, 15, 17, 
18, 22, 24, 25, 26, 34, 35, 36, 
37, 0 };

long DTtag::navrefMapPar0[] = { 
1, 38, 39, 41, 0 };

long DTtag::topicmetaMapPar0[] = { 
1, 28, 31, 38, 39, 41, 0 };

long DTtag::topicrefMapPar0[] = { 
1, 27, 38, 39, 41, 0 };

long DTtag::topicmetafirstMapPar0[] = { 
1, 31, 38, 39, 41, 0 };

vpair DTtag::MapElemSetsSet0[] = {
 { 65536, indextermMapPar0 },
 { 65537, keywordMapPar0 },
 { 65538, navrefMapPar0 },
 { 65539, topicmetaMapPar0 },
 { 65540, topicrefMapPar0 },
 { 65541, topicmetafirstMapPar0 },
 { 0, NULL }
 };

npair DTtag::MapElemSetNamesSet0[] = {
 { 65536, "*indexterm"},
 { 65537, "*keyword"},
 { 65538, "*navref"},
 { 65539, "*topicmeta"},
 { 65540, "*topicref"},
 { 65541, "*topicmetafirst"},
 { 0, NULL }
 };

ipair DTtag::MapLevelsSet0[] = {
 { 31, 1 },
 { 0, 0 }
 };

ipair DTtag::MapAtFirstSet0[] = {
 { 12, -1 },
 { 18, -1 },
 { 24, -1 },
 { 40, 65541 },
 { 0, 0 }
 };

// end of dwhtmd0.cpp

