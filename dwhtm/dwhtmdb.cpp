
// dwhtmdb.cpp is the data for dcl HTM DocBook 4.5 support, JHG
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


// preloaded DocBook topic type info

char *DBtag::PrologDTypeDocBook = {"\"-//OASIS//DTD DocBook XML V4.5//EN\""};
char *DBtag::PrologDTDDocBook = {"  \"http://www.oasis-open.org/docbook/xml/4.5/docbookx.dtd\""};


// tagsets for DocBook 4.5 DTD for book root

npair DBtag::DocBookBNamesSet[] = {
 { 1, "book"},
 { 2, "abbrev"},
 { 3, "abstract"},
 { 4, "accel"},
 { 5, "ackno"},
 { 6, "acronym"},
 { 7, "action"},
 { 8, "address"},
 { 9, "affiliation"},
 { 10, "alt"},
 { 11, "anchor"},
 { 12, "answer"},
 { 13, "appendix"},
 { 14, "appendixinfo"},
 { 15, "application"},
 { 16, "area"},
 { 17, "areaset"},
 { 18, "areaspec"},
 { 19, "arg"},
 { 20, "article"},
 { 21, "articleinfo"},
 { 22, "artpagenums"},
 { 23, "attribution"},
 { 24, "audiodata"},
 { 25, "audioobject"},
 { 26, "author"},
 { 27, "authorblurb"},
 { 28, "authorgroup"},
 { 29, "authorinitials"},
 { 30, "beginpage"},
 { 31, "bibliocoverage"},
 { 32, "bibliodiv"},
 { 33, "biblioentry"},
 { 34, "bibliography"},
 { 35, "bibliographyinfo"},
 { 36, "biblioid"},
 { 37, "bibliolist"},
 { 38, "bibliomisc"},
 { 39, "bibliomixed"},
 { 40, "bibliomset"},
 { 41, "biblioref"},
 { 42, "bibliorelation"},
 { 43, "biblioset"},
 { 44, "bibliosource"},
 { 45, "blockinfo"},
 { 46, "blockquote"},
 { 47, "bookinfo"},
 { 48, "bridgehead"},
 { 49, "callout"},
 { 50, "calloutlist"},
 { 51, "caption"},
 { 52, "caution"},
 { 53, "chapter"},
 { 54, "chapterinfo"},
 { 55, "citation"},
 { 56, "citebiblioid"},
 { 57, "citerefentry"},
 { 58, "citetitle"},
 { 59, "city"},
 { 60, "classname"},
 { 61, "classsynopsis"},
 { 62, "classsynopsisinfo"},
 { 63, "cmdsynopsis"},
 { 64, "co"},
 { 65, "code"},
 { 66, "col"},
 { 67, "colgroup"},
 { 68, "collab"},
 { 69, "collabname"},
 { 70, "colophon"},
 { 71, "colspec"},
 { 72, "command"},
 { 73, "computeroutput"},
 { 74, "confdates"},
 { 75, "confgroup"},
 { 76, "confnum"},
 { 77, "confsponsor"},
 { 78, "conftitle"},
 { 79, "constant"},
 { 80, "constructorsynopsis"},
 { 81, "contractnum"},
 { 82, "contractsponsor"},
 { 83, "contrib"},
 { 84, "copyright"},
 { 85, "coref"},
 { 86, "corpauthor"},
 { 87, "corpcredit"},
 { 88, "corpname"},
 { 89, "country"},
 { 90, "database"},
 { 91, "date"},
 { 92, "dedication"},
 { 93, "destructorsynopsis"},
 { 94, "edition"},
 { 95, "editor"},
 { 96, "email"},
 { 97, "emphasis"},
 { 98, "entry"},
 { 99, "entrytbl"},
 { 100, "envar"},
 { 101, "epigraph"},
 { 102, "equation"},
 { 103, "errorcode"},
 { 104, "errorname"},
 { 105, "errortext"},
 { 106, "errortype"},
 { 107, "example"},
 { 108, "exceptionname"},
 { 109, "fax"},
 { 110, "fieldsynopsis"},
 { 111, "figure"},
 { 112, "filename"},
 { 113, "firstname"},
 { 114, "firstterm"},
 { 115, "footnote"},
 { 116, "footnoteref"},
 { 117, "foreignphrase"},
 { 118, "formalpara"},
 { 119, "funcdef"},
 { 120, "funcparams"},
 { 121, "funcprototype"},
 { 122, "funcsynopsis"},
 { 123, "funcsynopsisinfo"},
 { 124, "function"},
 { 125, "glossary"},
 { 126, "glossaryinfo"},
 { 127, "glossdef"},
 { 128, "glossdiv"},
 { 129, "glossentry"},
 { 130, "glosslist"},
 { 131, "glosssee"},
 { 132, "glossseealso"},
 { 133, "glossterm"},
 { 134, "graphic"},
 { 135, "graphicco"},
 { 136, "group"},
 { 137, "guibutton"},
 { 138, "guiicon"},
 { 139, "guilabel"},
 { 140, "guimenu"},
 { 141, "guimenuitem"},
 { 142, "guisubmenu"},
 { 143, "hardware"},
 { 144, "highlights"},
 { 145, "holder"},
 { 146, "honorific"},
 { 147, "imagedata"},
 { 148, "imageobject"},
 { 149, "imageobjectco"},
 { 150, "important"},
 { 151, "index"},
 { 152, "indexdiv"},
 { 153, "indexentry"},
 { 154, "indexinfo"},
 { 155, "indexterm"},
 { 156, "informalequation"},
 { 157, "informalexample"},
 { 158, "informalfigure"},
 { 159, "informaltable"},
 { 160, "initializer"},
 { 161, "inlineequation"},
 { 162, "inlinegraphic"},
 { 163, "inlinemediaobject"},
 { 164, "interface"},
 { 165, "interfacename"},
 { 166, "invpartnumber"},
 { 167, "isbn"},
 { 168, "issn"},
 { 169, "issuenum"},
 { 170, "itemizedlist"},
 { 171, "itermset"},
 { 172, "jobtitle"},
 { 173, "keycap"},
 { 174, "keycode"},
 { 175, "keycombo"},
 { 176, "keysym"},
 { 177, "keyword"},
 { 178, "keywordset"},
 { 179, "label"},
 { 180, "legalnotice"},
 { 181, "lineage"},
 { 182, "lineannotation"},
 { 183, "link"},
 { 184, "listitem"},
 { 185, "literal"},
 { 186, "literallayout"},
 { 187, "lot"},
 { 188, "lotentry"},
 { 189, "manvolnum"},
 { 190, "markup"},
 { 191, "mathphrase"},
 { 192, "medialabel"},
 { 193, "mediaobject"},
 { 194, "mediaobjectco"},
 { 195, "member"},
 { 196, "menuchoice"},
 { 197, "methodname"},
 { 198, "methodparam"},
 { 199, "methodsynopsis"},
 { 200, "modespec"},
 { 201, "modifier"},
 { 202, "mousebutton"},
 { 203, "msg"},
 { 204, "msgaud"},
 { 205, "msgentry"},
 { 206, "msgexplan"},
 { 207, "msginfo"},
 { 208, "msglevel"},
 { 209, "msgmain"},
 { 210, "msgorig"},
 { 211, "msgrel"},
 { 212, "msgset"},
 { 213, "msgsub"},
 { 214, "msgtext"},
 { 215, "note"},
 { 216, "objectinfo"},
 { 217, "olink"},
 { 218, "ooclass"},
 { 219, "ooexception"},
 { 220, "oointerface"},
 { 221, "option"},
 { 222, "optional"},
 { 223, "orderedlist"},
 { 224, "orgdiv"},
 { 225, "orgname"},
 { 226, "otheraddr"},
 { 227, "othercredit"},
 { 228, "othername"},
 { 229, "package"},
 { 230, "pagenums"},
 { 231, "para"},
 { 232, "paramdef"},
 { 233, "parameter"},
 { 234, "part"},
 { 235, "partinfo"},
 { 236, "partintro"},
 { 237, "personblurb"},
 { 238, "personname"},
 { 239, "phone"},
 { 240, "phrase"},
 { 241, "pob"},
 { 242, "postcode"},
 { 243, "preface"},
 { 244, "prefaceinfo"},
 { 245, "primary"},
 { 246, "primaryie"},
 { 247, "printhistory"},
 { 248, "procedure"},
 { 249, "productname"},
 { 250, "productnumber"},
 { 251, "programlisting"},
 { 252, "programlistingco"},
 { 253, "prompt"},
 { 254, "property"},
 { 255, "pubdate"},
 { 256, "publisher"},
 { 257, "publishername"},
 { 258, "pubsnumber"},
 { 259, "qandadiv"},
 { 260, "qandaentry"},
 { 261, "qandaset"},
 { 262, "question"},
 { 263, "quote"},
 { 264, "refclass"},
 { 265, "refdescriptor"},
 { 266, "refentry"},
 { 267, "refentryinfo"},
 { 268, "refentrytitle"},
 { 269, "reference"},
 { 270, "referenceinfo"},
 { 271, "refmeta"},
 { 272, "refmiscinfo"},
 { 273, "refname"},
 { 274, "refnamediv"},
 { 275, "refpurpose"},
 { 276, "refsect1"},
 { 277, "refsect1info"},
 { 278, "refsect2"},
 { 279, "refsect2info"},
 { 280, "refsect3"},
 { 281, "refsect3info"},
 { 282, "refsection"},
 { 283, "refsectioninfo"},
 { 284, "refsynopsisdiv"},
 { 285, "refsynopsisdivinfo"},
 { 286, "releaseinfo"},
 { 287, "remark"},
 { 288, "replaceable"},
 { 289, "returnvalue"},
 { 290, "revdescription"},
 { 291, "revhistory"},
 { 292, "revision"},
 { 293, "revnumber"},
 { 294, "revremark"},
 { 295, "row"},
 { 296, "sbr"},
 { 297, "screen"},
 { 298, "screenco"},
 { 299, "screeninfo"},
 { 300, "screenshot"},
 { 301, "secondary"},
 { 302, "secondaryie"},
 { 303, "sect1"},
 { 304, "sect1info"},
 { 305, "sect2"},
 { 306, "sect2info"},
 { 307, "sect3"},
 { 308, "sect3info"},
 { 309, "sect4"},
 { 310, "sect4info"},
 { 311, "sect5"},
 { 312, "sect5info"},
 { 313, "section"},
 { 314, "sectioninfo"},
 { 315, "see"},
 { 316, "seealso"},
 { 317, "seealsoie"},
 { 318, "seeie"},
 { 319, "seg"},
 { 320, "seglistitem"},
 { 321, "segmentedlist"},
 { 322, "segtitle"},
 { 323, "seriesvolnums"},
 { 324, "setindex"},
 { 325, "setindexinfo"},
 { 326, "sgmltag"},
 { 327, "shortaffil"},
 { 328, "shortcut"},
 { 329, "sidebar"},
 { 330, "sidebarinfo"},
 { 331, "simpara"},
 { 332, "simplelist"},
 { 333, "simplemsgentry"},
 { 334, "simplesect"},
 { 335, "spanspec"},
 { 336, "state"},
 { 337, "step"},
 { 338, "stepalternatives"},
 { 339, "street"},
 { 340, "structfield"},
 { 341, "structname"},
 { 342, "subject"},
 { 343, "subjectset"},
 { 344, "subjectterm"},
 { 345, "subscript"},
 { 346, "substeps"},
 { 347, "subtitle"},
 { 348, "superscript"},
 { 349, "surname"},
 { 350, "symbol"},
 { 351, "synopfragment"},
 { 352, "synopfragmentref"},
 { 353, "synopsis"},
 { 354, "systemitem"},
 { 355, "table"},
 { 356, "task"},
 { 357, "taskprerequisites"},
 { 358, "taskrelated"},
 { 359, "tasksummary"},
 { 360, "tbody"},
 { 361, "td"},
 { 362, "term"},
 { 363, "termdef"},
 { 364, "tertiary"},
 { 365, "tertiaryie"},
 { 366, "textdata"},
 { 367, "textobject"},
 { 368, "tfoot"},
 { 369, "tgroup"},
 { 370, "th"},
 { 371, "thead"},
 { 372, "tip"},
 { 373, "title"},
 { 374, "titleabbrev"},
 { 375, "toc"},
 { 376, "tocback"},
 { 377, "tocchap"},
 { 378, "tocentry"},
 { 379, "tocfront"},
 { 380, "toclevel1"},
 { 381, "toclevel2"},
 { 382, "toclevel3"},
 { 383, "toclevel4"},
 { 384, "toclevel5"},
 { 385, "tocpart"},
 { 386, "token"},
 { 387, "tr"},
 { 388, "trademark"},
 { 389, "type"},
 { 390, "ulink"},
 { 391, "uri"},
 { 392, "userinput"},
 { 393, "varargs"},
 { 394, "variablelist"},
 { 395, "varlistentry"},
 { 396, "varname"},
 { 397, "videodata"},
 { 398, "videoobject"},
 { 399, "void"},
 { 400, "volumenum"},
 { 401, "warning"},
 { 402, "wordasword"},
 { 403, "xref"},
 { 404, "year"},
 { 0, NULL }
 };

ipair DBtag::DocBookBInlineSet[] = {
 { 2, 1 },
 { 4, 3 },
 { 5, 1 },
 { 6, 3 },
 { 7, 3 },
 { 8, 1 },
 { 10, 1 },
 { 15, 3 },
 { 19, 1 },
 { 22, 1 },
 { 23, 1 },
 { 29, 1 },
 { 31, 1 },
 { 36, 1 },
 { 38, 1 },
 { 39, 1 },
 { 40, 3 },
 { 41, 2 },
 { 42, 1 },
 { 44, 1 },
 { 48, 1 },
 { 51, 1 },
 { 55, 3 },
 { 56, 1 },
 { 57, 2 },
 { 58, 1 },
 { 59, 3 },
 { 60, 3 },
 { 62, 1 },
 { 64, 2 },
 { 65, 3 },
 { 69, 1 },
 { 72, 3 },
 { 73, 3 },
 { 74, 1 },
 { 76, 1 },
 { 77, 1 },
 { 78, 1 },
 { 79, 3 },
 { 81, 1 },
 { 82, 1 },
 { 83, 1 },
 { 85, 2 },
 { 86, 1 },
 { 87, 1 },
 { 88, 1 },
 { 89, 3 },
 { 90, 3 },
 { 91, 1 },
 { 94, 1 },
 { 96, 3 },
 { 97, 3 },
 { 98, 1 },
 { 100, 3 },
 { 103, 3 },
 { 104, 3 },
 { 105, 3 },
 { 106, 3 },
 { 108, 3 },
 { 109, 3 },
 { 112, 3 },
 { 113, 1 },
 { 114, 3 },
 { 115, 2 },
 { 116, 2 },
 { 117, 3 },
 { 119, 1 },
 { 120, 1 },
 { 123, 1 },
 { 124, 3 },
 { 131, 1 },
 { 132, 1 },
 { 133, 3 },
 { 137, 3 },
 { 138, 3 },
 { 139, 3 },
 { 140, 3 },
 { 141, 3 },
 { 142, 3 },
 { 143, 3 },
 { 145, 1 },
 { 146, 1 },
 { 160, 1 },
 { 161, 2 },
 { 162, 2 },
 { 163, 2 },
 { 164, 3 },
 { 165, 3 },
 { 166, 1 },
 { 167, 1 },
 { 168, 1 },
 { 169, 1 },
 { 172, 1 },
 { 173, 3 },
 { 174, 3 },
 { 175, 2 },
 { 176, 3 },
 { 177, 1 },
 { 179, 1 },
 { 181, 1 },
 { 182, 3 },
 { 183, 3 },
 { 185, 3 },
 { 186, 5 },
 { 188, 1 },
 { 189, 1 },
 { 190, 3 },
 { 191, 1 },
 { 192, 3 },
 { 195, 1 },
 { 196, 2 },
 { 197, 3 },
 { 200, 1 },
 { 201, 1 },
 { 202, 3 },
 { 204, 1 },
 { 208, 1 },
 { 210, 1 },
 { 217, 3 },
 { 218, 2 },
 { 219, 2 },
 { 220, 2 },
 { 221, 3 },
 { 222, 3 },
 { 224, 1 },
 { 225, 1 },
 { 226, 3 },
 { 228, 1 },
 { 229, 3 },
 { 230, 1 },
 { 231, 1 },
 { 232, 1 },
 { 233, 3 },
 { 239, 3 },
 { 240, 3 },
 { 241, 3 },
 { 242, 3 },
 { 245, 1 },
 { 246, 1 },
 { 249, 1 },
 { 250, 1 },
 { 251, 5 },
 { 253, 3 },
 { 254, 3 },
 { 255, 1 },
 { 257, 1 },
 { 258, 1 },
 { 263, 3 },
 { 264, 1 },
 { 265, 1 },
 { 268, 1 },
 { 272, 1 },
 { 273, 1 },
 { 275, 1 },
 { 286, 1 },
 { 287, 1 },
 { 288, 3 },
 { 289, 3 },
 { 293, 1 },
 { 294, 1 },
 { 297, 5 },
 { 299, 1 },
 { 301, 1 },
 { 302, 1 },
 { 315, 1 },
 { 316, 1 },
 { 317, 1 },
 { 318, 1 },
 { 319, 1 },
 { 322, 1 },
 { 323, 1 },
 { 326, 3 },
 { 327, 1 },
 { 331, 1 },
 { 336, 3 },
 { 339, 3 },
 { 340, 3 },
 { 341, 3 },
 { 344, 1 },
 { 345, 3 },
 { 347, 1 },
 { 348, 3 },
 { 349, 1 },
 { 350, 3 },
 { 352, 1 },
 { 353, 5 },
 { 354, 3 },
 { 361, 1 },
 { 362, 1 },
 { 363, 3 },
 { 364, 1 },
 { 365, 1 },
 { 370, 1 },
 { 373, 1 },
 { 374, 1 },
 { 376, 1 },
 { 378, 1 },
 { 379, 1 },
 { 386, 3 },
 { 388, 3 },
 { 389, 3 },
 { 390, 3 },
 { 391, 3 },
 { 392, 3 },
 { 396, 3 },
 { 400, 1 },
 { 402, 3 },
 { 403, 2 },
 { 404, 1 },
 { 0, 0 }
 };

ipair DBtag::DocBookBSingleSet[] = {
 { 3, 65587 },
 { 4, 65588 },
 { 5, 20 },
 { 6, 65589 },
 { 7, 65590 },
 { 8, 65591 },
 { 9, 65592 },
 { 10, 65593 },
 { 11, 65594 },
 { 12, 260 },
 { 13, 65595 },
 { 14, 13 },
 { 15, 65596 },
 { 16, 65597 },
 { 17, 18 },
 { 18, 65598 },
 { 19, 65599 },
 { 20, 65688 },
 { 21, 65600 },
 { 22, 65601 },
 { 23, 65602 },
 { 24, 25 },
 { 25, 65730 },
 { 26, 65603 },
 { 27, 65604 },
 { 28, 65601 },
 { 29, 65605 },
 { 30, 65606 },
 { 31, 65601 },
 { 32, 34 },
 { 33, 65607 },
 { 34, 65608 },
 { 35, 34 },
 { 36, 65601 },
 { 37, 65640 },
 { 38, 65601 },
 { 39, 65607 },
 { 40, 65609 },
 { 41, 65635 },
 { 42, 65601 },
 { 43, 65601 },
 { 44, 65601 },
 { 45, 65610 },
 { 46, 65611 },
 { 47, 1 },
 { 48, 65612 },
 { 49, 50 },
 { 50, 65613 },
 { 51, 65614 },
 { 52, 65615 },
 { 53, 65688 },
 { 54, 53 },
 { 55, 65635 },
 { 56, 65616 },
 { 57, 65635 },
 { 58, 65616 },
 { 59, 8 },
 { 60, 65617 },
 { 61, 65715 },
 { 62, 61 },
 { 63, 65715 },
 { 64, 65618 },
 { 65, 65590 },
 { 66, 65619 },
 { 67, 65719 },
 { 68, 65627 },
 { 69, 68 },
 { 70, 65620 },
 { 71, 65621 },
 { 72, 65622 },
 { 73, 65590 },
 { 74, 75 },
 { 75, 65601 },
 { 76, 75 },
 { 77, 75 },
 { 78, 75 },
 { 79, 65590 },
 { 80, 65634 },
 { 81, 65601 },
 { 82, 65601 },
 { 83, 65623 },
 { 84, 65601 },
 { 85, 65624 },
 { 86, 65625 },
 { 87, 65625 },
 { 88, 65601 },
 { 89, 8 },
 { 90, 65590 },
 { 91, 65626 },
 { 92, 1 },
 { 93, 65634 },
 { 94, 65601 },
 { 95, 65627 },
 { 96, 65628 },
 { 97, 65629 },
 { 98, 295 },
 { 99, 295 },
 { 100, 65590 },
 { 101, 65630 },
 { 102, 65631 },
 { 103, 65590 },
 { 104, 65590 },
 { 105, 65590 },
 { 106, 65590 },
 { 107, 65632 },
 { 108, 65633 },
 { 109, 8 },
 { 110, 65634 },
 { 111, 65631 },
 { 112, 65590 },
 { 113, 65646 },
 { 114, 65635 },
 { 115, 65635 },
 { 116, 65635 },
 { 117, 65635 },
 { 118, 65636 },
 { 119, 121 },
 { 120, 65637 },
 { 121, 122 },
 { 122, 65715 },
 { 123, 122 },
 { 124, 65638 },
 { 125, 65723 },
 { 126, 125 },
 { 127, 129 },
 { 128, 125 },
 { 129, 65639 },
 { 130, 65640 },
 { 131, 129 },
 { 132, 127 },
 { 133, 65641 },
 { 134, 65642 },
 { 135, 65643 },
 { 136, 65599 },
 { 137, 65644 },
 { 138, 65644 },
 { 139, 65644 },
 { 140, 65644 },
 { 141, 65644 },
 { 142, 65644 },
 { 143, 65590 },
 { 144, 65645 },
 { 145, 84 },
 { 146, 65646 },
 { 147, 148 },
 { 148, 65647 },
 { 149, 65648 },
 { 150, 65615 },
 { 151, 65723 },
 { 152, 65649 },
 { 153, 65650 },
 { 154, 151 },
 { 155, 65651 },
 { 156, 65652 },
 { 157, 65653 },
 { 158, 65653 },
 { 159, 65654 },
 { 160, 65655 },
 { 161, 65656 },
 { 162, 65657 },
 { 163, 65658 },
 { 164, 65644 },
 { 165, 65659 },
 { 166, 65601 },
 { 167, 65601 },
 { 168, 65601 },
 { 169, 65601 },
 { 170, 65660 },
 { 171, 65663 },
 { 172, 9 },
 { 173, 65661 },
 { 174, 65590 },
 { 175, 65661 },
 { 176, 65661 },
 { 177, 178 },
 { 178, 65663 },
 { 179, 65662 },
 { 180, 65663 },
 { 181, 65646 },
 { 182, 65664 },
 { 183, 65665 },
 { 184, 65666 },
 { 185, 65590 },
 { 186, 65667 },
 { 187, 65723 },
 { 188, 187 },
 { 189, 65696 },
 { 190, 65590 },
 { 191, 65593 },
 { 192, 65590 },
 { 193, 65668 },
 { 194, 65643 },
 { 195, 332 },
 { 196, 65590 },
 { 197, 65669 },
 { 198, 65670 },
 { 199, 65634 },
 { 200, 65671 },
 { 201, 65672 },
 { 202, 65661 },
 { 203, 205 },
 { 204, 207 },
 { 205, 212 },
 { 206, 65673 },
 { 207, 205 },
 { 208, 207 },
 { 209, 203 },
 { 210, 207 },
 { 211, 203 },
 { 212, 65674 },
 { 213, 203 },
 { 214, 65675 },
 { 215, 65615 },
 { 216, 65676 },
 { 217, 65665 },
 { 218, 65677 },
 { 219, 65677 },
 { 220, 65677 },
 { 221, 65678 },
 { 222, 65679 },
 { 223, 65660 },
 { 224, 9 },
 { 225, 65680 },
 { 226, 8 },
 { 227, 65625 },
 { 228, 65646 },
 { 229, 65681 },
 { 230, 65601 },
 { 231, 65682 },
 { 232, 121 },
 { 233, 65683 },
 { 234, 1 },
 { 235, 234 },
 { 236, 65684 },
 { 237, 65685 },
 { 238, 65686 },
 { 239, 8 },
 { 240, 65687 },
 { 241, 8 },
 { 242, 8 },
 { 243, 65688 },
 { 244, 243 },
 { 245, 155 },
 { 246, 153 },
 { 247, 65601 },
 { 248, 65689 },
 { 249, 65616 },
 { 250, 65616 },
 { 251, 65690 },
 { 252, 65691 },
 { 253, 65590 },
 { 254, 65590 },
 { 255, 65601 },
 { 256, 65601 },
 { 257, 65692 },
 { 258, 65601 },
 { 259, 65693 },
 { 260, 65694 },
 { 261, 65674 },
 { 262, 260 },
 { 263, 65635 },
 { 264, 274 },
 { 265, 274 },
 { 266, 65695 },
 { 267, 266 },
 { 268, 65696 },
 { 269, 65688 },
 { 270, 269 },
 { 271, 266 },
 { 272, 271 },
 { 273, 274 },
 { 274, 266 },
 { 275, 274 },
 { 276, 266 },
 { 277, 276 },
 { 278, 65697 },
 { 279, 278 },
 { 280, 278 },
 { 281, 280 },
 { 282, 65698 },
 { 283, 282 },
 { 284, 266 },
 { 285, 284 },
 { 286, 65601 },
 { 287, 65699 },
 { 288, 65700 },
 { 289, 65590 },
 { 290, 292 },
 { 291, 65701 },
 { 292, 291 },
 { 293, 292 },
 { 294, 292 },
 { 295, 65702 },
 { 296, 65703 },
 { 297, 65704 },
 { 298, 65691 },
 { 299, 300 },
 { 300, 65691 },
 { 301, 155 },
 { 302, 153 },
 { 303, 65705 },
 { 304, 303 },
 { 305, 303 },
 { 306, 305 },
 { 307, 305 },
 { 308, 307 },
 { 309, 307 },
 { 310, 309 },
 { 311, 309 },
 { 312, 311 },
 { 313, 65706 },
 { 314, 313 },
 { 315, 155 },
 { 316, 155 },
 { 317, 153 },
 { 318, 153 },
 { 319, 320 },
 { 320, 321 },
 { 321, 65660 },
 { 322, 321 },
 { 323, 65601 },
 { 324, 1 },
 { 325, 324 },
 { 326, 65590 },
 { 327, 9 },
 { 328, 196 },
 { 329, 65707 },
 { 330, 329 },
 { 331, 65636 },
 { 332, 65660 },
 { 333, 212 },
 { 334, 65708 },
 { 335, 65709 },
 { 336, 8 },
 { 337, 65710 },
 { 338, 337 },
 { 339, 8 },
 { 340, 65590 },
 { 341, 65590 },
 { 342, 343 },
 { 343, 65663 },
 { 344, 342 },
 { 345, 65711 },
 { 346, 337 },
 { 347, 65712 },
 { 348, 65711 },
 { 349, 65646 },
 { 350, 65713 },
 { 351, 63 },
 { 352, 65714 },
 { 353, 65715 },
 { 354, 65590 },
 { 355, 65716 },
 { 356, 65674 },
 { 357, 356 },
 { 358, 356 },
 { 359, 356 },
 { 360, 65720 },
 { 361, 387 },
 { 362, 395 },
 { 363, 65635 },
 { 364, 155 },
 { 365, 153 },
 { 366, 367 },
 { 367, 65717 },
 { 368, 65718 },
 { 369, 65719 },
 { 370, 387 },
 { 371, 65720 },
 { 372, 65615 },
 { 373, 65721 },
 { 374, 65722 },
 { 375, 65723 },
 { 376, 375 },
 { 377, 65724 },
 { 378, 65725 },
 { 379, 375 },
 { 380, 377 },
 { 381, 380 },
 { 382, 381 },
 { 383, 382 },
 { 384, 383 },
 { 385, 375 },
 { 386, 65590 },
 { 387, 65726 },
 { 388, 65727 },
 { 389, 65728 },
 { 390, 65665 },
 { 391, 65590 },
 { 392, 65590 },
 { 393, 121 },
 { 394, 65660 },
 { 395, 394 },
 { 396, 65729 },
 { 397, 398 },
 { 398, 65730 },
 { 399, 65731 },
 { 400, 65601 },
 { 401, 65615 },
 { 402, 65635 },
 { 403, 65635 },
 { 404, 84 },
 { 0, 0 }
 };

long DBtag::Part1DocBookBPar[] = { 
14, 21, 33, 35, 39, 40, 43, 45, 
47, 54, 126, 154, 216, 235, 244, 267, 
270, 283, 325, 330, 0 };

long DBtag::Part2DocBookBPar[] = { 
277, 279, 281, 285, 304, 306, 308, 310, 
312, 314, 0 };

long DBtag::Part3DocBookBPar[] = { 
46, 52, 92, 107, 125, 150, 180, 206, 
215, 248, 259, 261, 324, 329, 337, 357, 
358, 359, 372, 401, 0 };

long DBtag::Part4DocBookBPar[] = { 
13, 20, 32, 34, 53, 70, 128, 151, 
236, 243, 276, 282, 284, 303, 305, 307, 
309, 311, 313, 334, 0 };

long DBtag::Part5DocBookBPar[] = { 
37, 1, 50, 102, 111, 130, 152, 170, 
187, 212, 223, 234, 269, 278, 280, 321, 
355, 356, 375, 394, 0 };

long DBtag::Part6DocBookBPar[] = { 
15, 38, 55, 58, 97, 114, 117, 133, 
183, 217, 240, 249, 263, 287, 347, 353, 
363, 373, 374, 390, 0 };

long DBtag::Part7DocBookBPar[] = { 
23, 48, 98, 131, 132, 182, 186, 195, 
204, 231, 251, 268, 297, 299, 319, 322, 
331, 361, 362, 370, 0 };

long DBtag::Part8DocBookBPar[] = { 
188, 245, 246, 275, 301, 302, 315, 316, 
317, 318, 364, 365, 376, 378, 379, 0 };

long DBtag::Part9DocBookBPar[] = { 
2, 6, 179, 189, 402, 0 };

long DBtag::Part10DocBookBPar[] = { 
22, 29, 56, 81, 82, 86, 87, 88, 
91, 94, 96, 166, 167, 168, 200, 225, 
250, 345, 348, 388, 0 };

long DBtag::Part11DocBookBPar[] = { 
5, 31, 36, 42, 44, 83, 113, 146, 
169, 181, 228, 230, 255, 257, 258, 286, 
323, 339, 349, 400, 0 };

long DBtag::Part12DocBookBPar[] = { 
59, 69, 74, 76, 77, 78, 89, 109, 
145, 172, 224, 226, 239, 241, 242, 293, 
294, 327, 336, 404, 0 };

long DBtag::Part13DocBookBPar[] = { 
7, 65, 72, 73, 90, 112, 120, 123, 
124, 143, 165, 173, 185, 221, 222, 233, 
254, 288, 354, 392, 0 };

long DBtag::Part14DocBookBPar[] = { 
62, 152, 266, 272, 274, 0 };

long DBtag::Part15DocBookBPar[] = { 
12, 49, 184, 214, 262, 278, 280, 290, 
0 };

long DBtag::Part16DocBookBPar[] = { 
98, 115, 127, 135, 157, 231, 361, 370, 
0 };

long DBtag::Part17DocBookBPar[] = { 
125, 248, 259, 261, 324, 329, 357, 358, 
359, 0 };

long DBtag::Part18DocBookBPar[] = { 
22, 29, 56, 81, 82, 86, 87, 88, 
91, 94, 96, 166, 167, 168, 200, 225, 
250, 272, 0 };

long DBtag::Part19DocBookBPar[] = { 
13, 20, 34, 46, 52, 53, 150, 151, 
170, 206, 215, 223, 243, 303, 313, 334, 
337, 372, 394, 401, 0 };

long DBtag::Part20DocBookBPar[] = { 
32, 128, 236, 276, 282, 284, 305, 307, 
309, 311, 0 };

long DBtag::Part21DocBookBPar[] = { 
7, 62, 65, 72, 73, 90, 112, 120, 
123, 124, 143, 165, 173, 185, 221, 222, 
233, 254, 354, 392, 0 };

long DBtag::Part22DocBookBPar[] = { 
60, 79, 100, 103, 104, 105, 106, 108, 
137, 138, 139, 140, 141, 142, 164, 174, 
176, 190, 197, 229, 0 };

long DBtag::Part23DocBookBPar[] = { 
4, 160, 192, 201, 202, 208, 210, 253, 
272, 289, 326, 340, 341, 350, 386, 389, 
391, 396, 0 };

long DBtag::Part24DocBookBPar[] = { 
62, 188, 245, 246, 301, 302, 315, 316, 
317, 318, 364, 365, 376, 378, 379, 0 };

long DBtag::Part25DocBookBPar[] = { 
14, 21, 35, 45, 47, 54, 102, 107, 
111, 156, 158, 159, 216, 235, 244, 270, 
300, 330, 353, 355, 0 };

long DBtag::Part26DocBookBPar[] = { 
126, 152, 154, 267, 283, 325, 0 };

long DBtag::Part27DocBookBPar[] = { 
98, 115, 127, 157, 231, 361, 370, 0 };

long DBtag::Part28DocBookBPar[] = { 
62, 123, 186, 251, 297, 353, 0 };

long DBtag::Part29DocBookBPar[] = { 
111, 157, 158, 170, 223, 355, 356, 394, 
0 };

long DBtag::Part30DocBookBPar[] = { 
107, 180, 248, 357, 358, 359, 0 };

long DBtag::Part31DocBookBPar[] = { 
14, 21, 35, 45, 47, 54, 126, 154, 
216, 235, 244, 267, 270, 283, 325, 330, 
0 };

long DBtag::Part32DocBookBPar[] = { 
137, 138, 139, 140, 141, 142, 0 };

long DBtag::Part33DocBookBPar[] = { 
4, 127, 160, 192, 201, 202, 208, 210, 
253, 289, 326, 340, 341, 350, 376, 379, 
386, 389, 391, 396, 0 };

long DBtag::Part34DocBookBPar[] = { 
13, 20, 34, 53, 151, 243, 303, 313, 
334, 0 };

long DBtag::Part35DocBookBPar[] = { 
80, 93, 199, 265, 273, 388, 0 };

long DBtag::Part36DocBookBPar[] = { 
111, 152, 157, 158, 231, 0 };

long DBtag::Part37DocBookBPar[] = { 
49, 125, 184, 214, 278, 280, 324, 0 };

long DBtag::Part38DocBookBPar[] = { 
15, 38, 55, 58, 97, 114, 117, 133, 
183, 186, 204, 217, 240, 249, 251, 263, 
287, 353, 363, 390, 0 };

long DBtag::Part39DocBookBPar[] = { 
23, 131, 132, 182, 188, 195, 268, 297, 
299, 319, 331, 362, 378, 0 };

long DBtag::Part40DocBookBPar[] = { 
92, 187, 234, 269, 375, 0 };

long DBtag::Part41DocBookBPar[] = { 
198, 232, 265, 273, 388, 0 };

long DBtag::Part42DocBookBPar[] = { 
46, 170, 206, 223, 337, 394, 0 };

long DBtag::Part43DocBookBPar[] = { 
3, 27, 51, 98, 101, 115, 127, 144, 
152, 157, 170, 223, 237, 247, 361, 367, 
370, 394, 0 };

long DBtag::Part44DocBookBPar[] = { 
107, 111, 152, 158, 376, 379, 0 };

long DBtag::Part45DocBookBPar[] = { 
51, 111, 152, 158, 170, 223, 367, 394, 
0 };

long DBtag::Part46DocBookBPar[] = { 
46, 52, 127, 150, 206, 215, 231, 337, 
372, 401, 0 };

long DBtag::Part47DocBookBPar[] = { 
46, 52, 150, 206, 215, 337, 372, 401, 
0 };

long DBtag::Part48DocBookBPar[] = { 
13, 20, 1, 53, 234, 243, 303, 305, 
307, 309, 311, 313, 0 };

long DBtag::Part49DocBookBPar[] = { 
13, 20, 53, 236, 243, 0 };

long DBtag::Part50DocBookBPar[] = { 
303, 305, 307, 309, 311, 313, 0 };

long DBtag::abbrevDocBookBPar[] = { 
129, 65536, 65537, 65541, 65542, 65543, 0 };

long DBtag::abstractDocBookBPar[] = { 
248, 357, 358, 359, 65536, 65537, 65555, 65569, 
65572, 65577, 0 };

long DBtag::accelDocBookBPar[] = { 
164, 65567, 0 };

long DBtag::acronymDocBookBPar[] = { 
129, 354, 65541, 65542, 65543, 65544, 0 };

long DBtag::actionDocBookBPar[] = { 
265, 273, 388, 65541, 65542, 65543, 65556, 0 };

long DBtag::addressDocBookBPar[] = { 
9, 26, 75, 95, 107, 115, 127, 227, 
256, 65536, 65537, 65550, 65552, 65554, 65555, 65571, 
0 };

long DBtag::affiliationDocBookBPar[] = { 
8, 26, 68, 95, 227, 65536, 65537, 0 };

long DBtag::altDocBookBPar[] = { 
102, 156, 161, 0 };

long DBtag::anchorDocBookBPar[] = { 
62, 152, 345, 348, 388, 65541, 65542, 65543, 
65544, 65548, 65550, 65552, 65554, 65555, 0 };

long DBtag::appendixDocBookBPar[] = { 
20, 1, 234, 0 };

long DBtag::applicationDocBookBPar[] = { 
264, 265, 273, 388, 65541, 65542, 65543, 65556, 
0 };

long DBtag::areaDocBookBPar[] = { 
17, 18, 0 };

long DBtag::areaspecDocBookBPar[] = { 
135, 149, 252, 298, 0 };

long DBtag::argDocBookBPar[] = { 
19, 63, 136, 351, 0 };

long DBtag::articleinfoDocBookBPar[] = { 
20, 33, 0 };

long DBtag::artpagenumsDocBookBPar[] = { 
65536, 65537, 0 };

long DBtag::attributionDocBookBPar[] = { 
46, 101, 0 };

long DBtag::authorDocBookBPar[] = { 
28, 292, 65536, 65537, 65541, 65542, 65543, 0 };

long DBtag::authorblurbDocBookBPar[] = { 
8, 26, 95, 227, 248, 357, 358, 359, 
65536, 65537, 65555, 65569, 65572, 65577, 0 };

long DBtag::authorinitialsDocBookBPar[] = { 
292, 65536, 65537, 65541, 65542, 65543, 0 };

long DBtag::beginpageDocBookBPar[] = { 
98, 266, 275, 329, 361, 370, 65544, 65554, 
65555, 65556, 65557, 65565, 65568, 65571, 65572, 65573, 
65574, 65575, 0 };

long DBtag::biblioentryDocBookBPar[] = { 
32, 34, 37, 0 };

long DBtag::bibliographyDocBookBPar[] = { 
125, 65583, 0 };

long DBtag::bibliomsetDocBookBPar[] = { 
39, 40, 0 };

long DBtag::blockinfoDocBookBPar[] = { 
37, 46, 102, 130, 156, 159, 212, 259, 
260, 261, 65564, 65565, 0 };

long DBtag::blockquoteDocBookBPar[] = { 
51, 115, 127, 170, 223, 367, 394, 65538, 
65539, 65550, 65571, 0 };

long DBtag::bridgeheadDocBookBPar[] = { 
65550, 65552, 65554, 65555, 0 };

long DBtag::calloutlistDocBookBPar[] = { 
51, 144, 149, 252, 298, 367, 65538, 65539, 
65550, 65551, 0 };

long DBtag::captionDocBookBPar[] = { 
193, 355, 0 };

long DBtag::cautionDocBookBPar[] = { 
51, 92, 98, 144, 180, 231, 361, 367, 
370, 65539, 65550, 65552, 65577, 0 };

long DBtag::citetitleDocBookBPar[] = { 
65536, 65537, 65541, 65542, 65543, 0 };

long DBtag::classnameDocBookBPar[] = { 
218, 265, 273, 388, 65541, 65542, 65543, 65556, 
0 };

long DBtag::coDocBookBPar[] = { 
73, 186, 251, 253, 288, 297, 353, 354, 
392, 0 };

long DBtag::colDocBookBPar[] = { 
67, 159, 355, 0 };

long DBtag::colophonDocBookBPar[] = { 
20, 1, 0 };

long DBtag::colspecDocBookBPar[] = { 
99, 368, 369, 371, 0 };

long DBtag::commandDocBookBPar[] = { 
63, 265, 273, 388, 65541, 65542, 65543, 65556, 
0 };

long DBtag::contribDocBookBPar[] = { 
8, 26, 95, 227, 65536, 65537, 0 };

long DBtag::corefDocBookBPar[] = { 
186, 251, 297, 353, 0 };

long DBtag::corpauthorDocBookBPar[] = { 
28, 65536, 65537, 65541, 65542, 65543, 0 };

long DBtag::dateDocBookBPar[] = { 
292, 65536, 65537, 0 };

long DBtag::editorDocBookBPar[] = { 
28, 65536, 65537, 0 };

long DBtag::emailDocBookBPar[] = { 
8, 26, 95, 227, 265, 273, 388, 65541, 
65542, 65543, 65556, 0 };

long DBtag::emphasisDocBookBPar[] = { 
191, 272, 65541, 65542, 65543, 65544, 65545, 65546, 
65547, 0 };

long DBtag::epigraphDocBookBPar[] = { 
248, 357, 358, 359, 65555, 65569, 65572, 65577, 
0 };

long DBtag::equationDocBookBPar[] = { 
65550, 65552, 65555, 65569, 65581, 0 };

long DBtag::exampleDocBookBPar[] = { 
356, 65550, 65552, 65555, 65569, 65581, 0 };

long DBtag::exceptionnameDocBookBPar[] = { 
219, 65541, 65542, 65543, 65556, 65570, 0 };

long DBtag::fieldsynopsisDocBookBPar[] = { 
61, 65550, 65552, 65554, 65555, 65562, 65573, 65574, 
65579, 0 };

long DBtag::footnoterefDocBookBPar[] = { 
65541, 65542, 65543, 0 };

long DBtag::formalparaDocBookBPar[] = { 
65538, 65539, 65550, 65578, 0 };

long DBtag::funcparamsDocBookBPar[] = { 
198, 232, 0 };

long DBtag::functionDocBookBPar[] = { 
119, 265, 273, 388, 65541, 65542, 65543, 65556, 
0 };

long DBtag::glossentryDocBookBPar[] = { 
125, 128, 130, 0 };

long DBtag::glosslistDocBookBPar[] = { 
51, 144, 367, 65538, 65539, 65550, 65562, 0 };

long DBtag::glosstermDocBookBPar[] = { 
129, 65541, 65542, 65543, 0 };

long DBtag::graphicDocBookBPar[] = { 
161, 65537, 65550, 65551, 65552, 65554, 65555, 65560, 
65561, 0 };

long DBtag::graphiccoDocBookBPar[] = { 
107, 115, 127, 300, 65550, 65552, 65554, 65555, 
65571, 0 };

long DBtag::guibuttonDocBookBPar[] = { 
196, 265, 273, 388, 65541, 65542, 65543, 65556, 
0 };

long DBtag::highlightsDocBookBPar[] = { 
65550, 65552, 65555, 65569, 65577, 0 };

long DBtag::honorificDocBookBPar[] = { 
8, 26, 95, 227, 238, 65536, 65537, 0 };

long DBtag::imageobjectDocBookBPar[] = { 
149, 163, 193, 0 };

long DBtag::imageobjectcoDocBookBPar[] = { 
163, 193, 194, 0 };

long DBtag::indexdivDocBookBPar[] = { 
151, 324, 0 };

long DBtag::indexentryDocBookBPar[] = { 
151, 152, 324, 0 };

long DBtag::indextermDocBookBPar[] = { 
118, 129, 144, 171, 188, 266, 271, 275, 
378, 65536, 65537, 65538, 65541, 65542, 65544, 65546, 
65547, 65550, 65553, 65555, 65556, 65557, 65564, 65568, 
65569, 0 };

long DBtag::informalequationDocBookBPar[] = { 
102, 107, 115, 127, 65550, 65552, 65554, 65555, 
65571, 0 };

long DBtag::informalexampleDocBookBPar[] = { 
107, 115, 127, 65550, 65552, 65554, 65555, 65571, 
0 };

long DBtag::informaltableDocBookBPar[] = { 
107, 115, 127, 361, 370, 65550, 65552, 65554, 
65555, 65571, 0 };

long DBtag::initializerDocBookBPar[] = { 
110, 198, 232, 0 };

long DBtag::inlineequationDocBookBPar[] = { 
188, 376, 378, 379, 65541, 65542, 0 };

long DBtag::inlinegraphicDocBookBPar[] = { 
65541, 65542, 65544, 65545, 65546, 65547, 65548, 65557, 
65558, 65559, 0 };

long DBtag::inlinemediaobjectDocBookBPar[] = { 
161, 65541, 65542, 65544, 65545, 65546, 65547, 65548, 
65557, 65558, 65559, 0 };

long DBtag::interfacenameDocBookBPar[] = { 
220, 265, 273, 388, 65541, 65542, 65543, 65556, 
0 };

long DBtag::itemizedlistDocBookBPar[] = { 
51, 144, 152, 367, 65538, 65539, 65550, 65562, 
0 };

long DBtag::keycapDocBookBPar[] = { 
175, 265, 273, 328, 388, 65541, 65542, 65543, 
65556, 0 };

long DBtag::labelDocBookBPar[] = { 
12, 262, 0 };

long DBtag::legalnoticeDocBookBPar[] = { 
65537, 65566, 0 };

long DBtag::lineannotationDocBookBPar[] = { 
65563, 0 };

long DBtag::linkDocBookBPar[] = { 
111, 158, 65541, 65542, 65543, 65544, 65545, 65546, 
65547, 65548, 65549, 0 };

long DBtag::listitemDocBookBPar[] = { 
170, 223, 395, 0 };

long DBtag::literallayoutDocBookBPar[] = { 
101, 65538, 65539, 65550, 65562, 65580, 0 };

long DBtag::mediaobjectDocBookBPar[] = { 
65537, 65550, 65552, 65554, 65555, 65560, 65561, 65562, 
0 };

long DBtag::methodnameDocBookBPar[] = { 
65541, 65542, 65543, 65556, 65570, 0 };

long DBtag::methodparamDocBookBPar[] = { 
80, 93, 199, 0 };

long DBtag::modespecDocBookBPar[] = { 
65537, 65541, 65542, 65543, 65566, 0 };

long DBtag::modifierDocBookBPar[] = { 
80, 93, 110, 121, 198, 199, 218, 219, 
220, 0 };

long DBtag::msgexplanDocBookBPar[] = { 
205, 333, 0 };

long DBtag::msgsetDocBookBPar[] = { 
46, 206, 248, 337, 357, 358, 359, 65555, 
65569, 65572, 0 };

long DBtag::msgtextDocBookBPar[] = { 
209, 211, 213, 333, 0 };

long DBtag::objectinfoDocBookBPar[] = { 
25, 148, 163, 193, 194, 367, 398, 0 };

long DBtag::ooclassDocBookBPar[] = { 
61, 265, 273, 388, 65541, 65542, 65543, 65556, 
0 };

long DBtag::optionDocBookBPar[] = { 
19, 136, 265, 273, 388, 65541, 65542, 65543, 
65556, 0 };

long DBtag::optionalDocBookBPar[] = { 
62, 265, 273, 388, 65541, 65542, 65543, 65548, 
0 };

long DBtag::orgnameDocBookBPar[] = { 
9, 65536, 65537, 65541, 65542, 65543, 0 };

long DBtag::packageDocBookBPar[] = { 
218, 219, 220, 265, 273, 388, 65541, 65542, 
65543, 65556, 0 };

long DBtag::paraDocBookBPar[] = { 
118, 65538, 65539, 65550, 65578, 0 };

long DBtag::parameterDocBookBPar[] = { 
65541, 65542, 65543, 65556, 65576, 0 };

long DBtag::partintroDocBookBPar[] = { 
234, 269, 0 };

long DBtag::personblurbDocBookBPar[] = { 
26, 95, 227, 0 };

long DBtag::personnameDocBookBPar[] = { 
8, 26, 95, 227, 65536, 65537, 65541, 65542, 
65543, 0 };

long DBtag::phraseDocBookBPar[] = { 
367, 65541, 65542, 65543, 0 };

long DBtag::prefaceDocBookBPar[] = { 
1, 234, 0 };

long DBtag::procedureDocBookBPar[] = { 
107, 157, 356, 65550, 65552, 65555, 65569, 65582, 
0 };

long DBtag::programlistingDocBookBPar[] = { 
252, 65538, 65539, 65550, 65562, 65580, 0 };

long DBtag::programlistingcoDocBookBPar[] = { 
65538, 65539, 65550, 65562, 65580, 0 };

long DBtag::publishernameDocBookBPar[] = { 
256, 65536, 65537, 0 };

long DBtag::qandadivDocBookBPar[] = { 
259, 261, 0 };

long DBtag::qandaentryDocBookBPar[] = { 
12, 259, 261, 0 };

long DBtag::refentryDocBookBPar[] = { 
234, 269, 65584, 65585, 0 };

long DBtag::refentrytitleDocBookBPar[] = { 
57, 271, 0 };

long DBtag::refsect2DocBookBPar[] = { 
276, 284, 0 };

long DBtag::refsectionDocBookBPar[] = { 
266, 282, 0 };

long DBtag::remarkDocBookBPar[] = { 
127, 65541, 65542, 65543, 65544, 65545, 65546, 65547, 
65548, 65549, 65550, 65552, 65554, 65555, 0 };

long DBtag::replaceableDocBookBPar[] = { 
19, 119, 136, 232, 265, 273, 65541, 65542, 
65543, 65545, 65546, 65547, 65556, 65557, 65558, 0 };

long DBtag::revhistoryDocBookBPar[] = { 
129, 260, 65536, 65537, 65541, 65542, 65543, 0 };

long DBtag::rowDocBookBPar[] = { 
360, 368, 371, 0 };

long DBtag::sbrDocBookBPar[] = { 
19, 63, 136, 0 };

long DBtag::screenDocBookBPar[] = { 
298, 65538, 65539, 65550, 65562, 65580, 0 };

long DBtag::sect1DocBookBPar[] = { 
65584, 0 };

long DBtag::sectionDocBookBPar[] = { 
313, 65584, 0 };

long DBtag::sidebarDocBookBPar[] = { 
248, 357, 358, 359, 65555, 65569, 65572, 65582, 
0 };

long DBtag::simplesectDocBookBPar[] = { 
65584, 65585, 0 };

long DBtag::spanspecDocBookBPar[] = { 
99, 369, 0 };

long DBtag::stepDocBookBPar[] = { 
248, 338, 346, 0 };

long DBtag::subscriptDocBookBPar[] = { 
62, 191, 272, 65541, 65542, 65543, 65544, 65545, 
65546, 65547, 65548, 65567, 0 };

long DBtag::subtitleDocBookBPar[] = { 
1, 125, 152, 278, 280, 324, 65536, 65537, 
65539, 65575, 0 };

long DBtag::symbolDocBookBPar[] = { 
265, 273, 345, 348, 388, 65541, 65542, 65543, 
65556, 0 };

long DBtag::synopfragmentrefDocBookBPar[] = { 
19, 136, 0 };

long DBtag::synopsisDocBookBPar[] = { 
65550, 65552, 65554, 65555, 65562, 65573, 65574, 65579, 
0 };

long DBtag::tableDocBookBPar[] = { 
361, 370, 65550, 65552, 65555, 65569, 65581, 0 };

long DBtag::textobjectDocBookBPar[] = { 
159, 163, 193, 194, 355, 65563, 0 };

long DBtag::tfootDocBookBPar[] = { 
159, 355, 369, 0 };

long DBtag::tgroupDocBookBPar[] = { 
159, 355, 0 };

long DBtag::theadDocBookBPar[] = { 
99, 159, 355, 369, 0 };

long DBtag::titleDocBookBPar[] = { 
3, 27, 118, 203, 209, 211, 213, 237, 
65536, 65537, 65538, 65539, 65540, 0 };

long DBtag::titleabbrevDocBookBPar[] = { 
92, 107, 65536, 65537, 65539, 65540, 65552, 0 };

long DBtag::tocDocBookBPar[] = { 
65583, 0 };

long DBtag::tocchapDocBookBPar[] = { 
13, 20, 53, 243, 375, 385, 0 };

long DBtag::tocentryDocBookBPar[] = { 
377, 380, 381, 382, 383, 384, 385, 0 };

long DBtag::trDocBookBPar[] = { 
159, 355, 360, 368, 371, 0 };

long DBtag::trademarkDocBookBPar[] = { 
65541, 65542, 65543, 65544, 65546, 65547, 65553, 0 };

long DBtag::typeDocBookBPar[] = { 
110, 119, 199, 65541, 65542, 65543, 65556, 65576, 
0 };

long DBtag::varnameDocBookBPar[] = { 
110, 265, 273, 388, 65541, 65542, 65543, 65556, 
0 };

long DBtag::videoobjectDocBookBPar[] = { 
163, 193, 0 };

long DBtag::voidDocBookBPar[] = { 
80, 93, 121, 199, 0 };

long DBtag::beginpagefirstDocBookBPar[] = { 
13, 53, 187, 234, 243, 266, 269, 375, 
0 };

long DBtag::blockinfofirstDocBookBPar[] = { 
37, 46, 102, 111, 130, 156, 157, 158, 
159, 170, 212, 223, 259, 260, 261, 356, 
394, 65565, 0 };

long DBtag::titlefirstDocBookBPar[] = { 
3, 20, 27, 32, 1, 50, 52, 70, 
92, 118, 128, 150, 152, 203, 206, 209, 
211, 213, 215, 236, 237, 321, 334, 337, 
372, 401, 0 };

vpair DBtag::DocBookBElemSetsSet[] = {
 { 65536, Part1DocBookBPar },
 { 65537, Part2DocBookBPar },
 { 65538, Part3DocBookBPar },
 { 65539, Part4DocBookBPar },
 { 65540, Part5DocBookBPar },
 { 65541, Part6DocBookBPar },
 { 65542, Part7DocBookBPar },
 { 65543, Part8DocBookBPar },
 { 65544, Part9DocBookBPar },
 { 65545, Part10DocBookBPar },
 { 65546, Part11DocBookBPar },
 { 65547, Part12DocBookBPar },
 { 65548, Part13DocBookBPar },
 { 65549, Part14DocBookBPar },
 { 65550, Part15DocBookBPar },
 { 65551, Part16DocBookBPar },
 { 65552, Part17DocBookBPar },
 { 65553, Part18DocBookBPar },
 { 65554, Part19DocBookBPar },
 { 65555, Part20DocBookBPar },
 { 65556, Part21DocBookBPar },
 { 65557, Part22DocBookBPar },
 { 65558, Part23DocBookBPar },
 { 65559, Part24DocBookBPar },
 { 65560, Part25DocBookBPar },
 { 65561, Part26DocBookBPar },
 { 65562, Part27DocBookBPar },
 { 65563, Part28DocBookBPar },
 { 65564, Part29DocBookBPar },
 { 65565, Part30DocBookBPar },
 { 65566, Part31DocBookBPar },
 { 65567, Part32DocBookBPar },
 { 65568, Part33DocBookBPar },
 { 65569, Part34DocBookBPar },
 { 65570, Part35DocBookBPar },
 { 65571, Part36DocBookBPar },
 { 65572, Part37DocBookBPar },
 { 65573, Part38DocBookBPar },
 { 65574, Part39DocBookBPar },
 { 65575, Part40DocBookBPar },
 { 65576, Part41DocBookBPar },
 { 65577, Part42DocBookBPar },
 { 65578, Part43DocBookBPar },
 { 65579, Part44DocBookBPar },
 { 65580, Part45DocBookBPar },
 { 65581, Part46DocBookBPar },
 { 65582, Part47DocBookBPar },
 { 65583, Part48DocBookBPar },
 { 65584, Part49DocBookBPar },
 { 65585, Part50DocBookBPar },
 { 65586, abbrevDocBookBPar },
 { 65587, abstractDocBookBPar },
 { 65588, accelDocBookBPar },
 { 65589, acronymDocBookBPar },
 { 65590, actionDocBookBPar },
 { 65591, addressDocBookBPar },
 { 65592, affiliationDocBookBPar },
 { 65593, altDocBookBPar },
 { 65594, anchorDocBookBPar },
 { 65595, appendixDocBookBPar },
 { 65596, applicationDocBookBPar },
 { 65597, areaDocBookBPar },
 { 65598, areaspecDocBookBPar },
 { 65599, argDocBookBPar },
 { 65600, articleinfoDocBookBPar },
 { 65601, artpagenumsDocBookBPar },
 { 65602, attributionDocBookBPar },
 { 65603, authorDocBookBPar },
 { 65604, authorblurbDocBookBPar },
 { 65605, authorinitialsDocBookBPar },
 { 65606, beginpageDocBookBPar },
 { 65607, biblioentryDocBookBPar },
 { 65608, bibliographyDocBookBPar },
 { 65609, bibliomsetDocBookBPar },
 { 65610, blockinfoDocBookBPar },
 { 65611, blockquoteDocBookBPar },
 { 65612, bridgeheadDocBookBPar },
 { 65613, calloutlistDocBookBPar },
 { 65614, captionDocBookBPar },
 { 65615, cautionDocBookBPar },
 { 65616, citetitleDocBookBPar },
 { 65617, classnameDocBookBPar },
 { 65618, coDocBookBPar },
 { 65619, colDocBookBPar },
 { 65620, colophonDocBookBPar },
 { 65621, colspecDocBookBPar },
 { 65622, commandDocBookBPar },
 { 65623, contribDocBookBPar },
 { 65624, corefDocBookBPar },
 { 65625, corpauthorDocBookBPar },
 { 65626, dateDocBookBPar },
 { 65627, editorDocBookBPar },
 { 65628, emailDocBookBPar },
 { 65629, emphasisDocBookBPar },
 { 65630, epigraphDocBookBPar },
 { 65631, equationDocBookBPar },
 { 65632, exampleDocBookBPar },
 { 65633, exceptionnameDocBookBPar },
 { 65634, fieldsynopsisDocBookBPar },
 { 65635, footnoterefDocBookBPar },
 { 65636, formalparaDocBookBPar },
 { 65637, funcparamsDocBookBPar },
 { 65638, functionDocBookBPar },
 { 65639, glossentryDocBookBPar },
 { 65640, glosslistDocBookBPar },
 { 65641, glosstermDocBookBPar },
 { 65642, graphicDocBookBPar },
 { 65643, graphiccoDocBookBPar },
 { 65644, guibuttonDocBookBPar },
 { 65645, highlightsDocBookBPar },
 { 65646, honorificDocBookBPar },
 { 65647, imageobjectDocBookBPar },
 { 65648, imageobjectcoDocBookBPar },
 { 65649, indexdivDocBookBPar },
 { 65650, indexentryDocBookBPar },
 { 65651, indextermDocBookBPar },
 { 65652, informalequationDocBookBPar },
 { 65653, informalexampleDocBookBPar },
 { 65654, informaltableDocBookBPar },
 { 65655, initializerDocBookBPar },
 { 65656, inlineequationDocBookBPar },
 { 65657, inlinegraphicDocBookBPar },
 { 65658, inlinemediaobjectDocBookBPar },
 { 65659, interfacenameDocBookBPar },
 { 65660, itemizedlistDocBookBPar },
 { 65661, keycapDocBookBPar },
 { 65662, labelDocBookBPar },
 { 65663, legalnoticeDocBookBPar },
 { 65664, lineannotationDocBookBPar },
 { 65665, linkDocBookBPar },
 { 65666, listitemDocBookBPar },
 { 65667, literallayoutDocBookBPar },
 { 65668, mediaobjectDocBookBPar },
 { 65669, methodnameDocBookBPar },
 { 65670, methodparamDocBookBPar },
 { 65671, modespecDocBookBPar },
 { 65672, modifierDocBookBPar },
 { 65673, msgexplanDocBookBPar },
 { 65674, msgsetDocBookBPar },
 { 65675, msgtextDocBookBPar },
 { 65676, objectinfoDocBookBPar },
 { 65677, ooclassDocBookBPar },
 { 65678, optionDocBookBPar },
 { 65679, optionalDocBookBPar },
 { 65680, orgnameDocBookBPar },
 { 65681, packageDocBookBPar },
 { 65682, paraDocBookBPar },
 { 65683, parameterDocBookBPar },
 { 65684, partintroDocBookBPar },
 { 65685, personblurbDocBookBPar },
 { 65686, personnameDocBookBPar },
 { 65687, phraseDocBookBPar },
 { 65688, prefaceDocBookBPar },
 { 65689, procedureDocBookBPar },
 { 65690, programlistingDocBookBPar },
 { 65691, programlistingcoDocBookBPar },
 { 65692, publishernameDocBookBPar },
 { 65693, qandadivDocBookBPar },
 { 65694, qandaentryDocBookBPar },
 { 65695, refentryDocBookBPar },
 { 65696, refentrytitleDocBookBPar },
 { 65697, refsect2DocBookBPar },
 { 65698, refsectionDocBookBPar },
 { 65699, remarkDocBookBPar },
 { 65700, replaceableDocBookBPar },
 { 65701, revhistoryDocBookBPar },
 { 65702, rowDocBookBPar },
 { 65703, sbrDocBookBPar },
 { 65704, screenDocBookBPar },
 { 65705, sect1DocBookBPar },
 { 65706, sectionDocBookBPar },
 { 65707, sidebarDocBookBPar },
 { 65708, simplesectDocBookBPar },
 { 65709, spanspecDocBookBPar },
 { 65710, stepDocBookBPar },
 { 65711, subscriptDocBookBPar },
 { 65712, subtitleDocBookBPar },
 { 65713, symbolDocBookBPar },
 { 65714, synopfragmentrefDocBookBPar },
 { 65715, synopsisDocBookBPar },
 { 65716, tableDocBookBPar },
 { 65717, textobjectDocBookBPar },
 { 65718, tfootDocBookBPar },
 { 65719, tgroupDocBookBPar },
 { 65720, theadDocBookBPar },
 { 65721, titleDocBookBPar },
 { 65722, titleabbrevDocBookBPar },
 { 65723, tocDocBookBPar },
 { 65724, tocchapDocBookBPar },
 { 65725, tocentryDocBookBPar },
 { 65726, trDocBookBPar },
 { 65727, trademarkDocBookBPar },
 { 65728, typeDocBookBPar },
 { 65729, varnameDocBookBPar },
 { 65730, videoobjectDocBookBPar },
 { 65731, voidDocBookBPar },
 { 65732, beginpagefirstDocBookBPar },
 { 65733, blockinfofirstDocBookBPar },
 { 65734, titlefirstDocBookBPar },
 { 0, NULL }
 };

npair DBtag::DocBookBElemSetNamesSet[] = {
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
 { 65556, "*Part21"},
 { 65557, "*Part22"},
 { 65558, "*Part23"},
 { 65559, "*Part24"},
 { 65560, "*Part25"},
 { 65561, "*Part26"},
 { 65562, "*Part27"},
 { 65563, "*Part28"},
 { 65564, "*Part29"},
 { 65565, "*Part30"},
 { 65566, "*Part31"},
 { 65567, "*Part32"},
 { 65568, "*Part33"},
 { 65569, "*Part34"},
 { 65570, "*Part35"},
 { 65571, "*Part36"},
 { 65572, "*Part37"},
 { 65573, "*Part38"},
 { 65574, "*Part39"},
 { 65575, "*Part40"},
 { 65576, "*Part41"},
 { 65577, "*Part42"},
 { 65578, "*Part43"},
 { 65579, "*Part44"},
 { 65580, "*Part45"},
 { 65581, "*Part46"},
 { 65582, "*Part47"},
 { 65583, "*Part48"},
 { 65584, "*Part49"},
 { 65585, "*Part50"},
 { 65586, "*abbrev"},
 { 65587, "*abstract"},
 { 65588, "*accel"},
 { 65589, "*acronym"},
 { 65590, "*action"},
 { 65591, "*address"},
 { 65592, "*affiliation"},
 { 65593, "*alt"},
 { 65594, "*anchor"},
 { 65595, "*appendix"},
 { 65596, "*application"},
 { 65597, "*area"},
 { 65598, "*areaspec"},
 { 65599, "*arg"},
 { 65600, "*articleinfo"},
 { 65601, "*artpagenums"},
 { 65602, "*attribution"},
 { 65603, "*author"},
 { 65604, "*authorblurb"},
 { 65605, "*authorinitials"},
 { 65606, "*beginpage"},
 { 65607, "*biblioentry"},
 { 65608, "*bibliography"},
 { 65609, "*bibliomset"},
 { 65610, "*blockinfo"},
 { 65611, "*blockquote"},
 { 65612, "*bridgehead"},
 { 65613, "*calloutlist"},
 { 65614, "*caption"},
 { 65615, "*caution"},
 { 65616, "*citetitle"},
 { 65617, "*classname"},
 { 65618, "*co"},
 { 65619, "*col"},
 { 65620, "*colophon"},
 { 65621, "*colspec"},
 { 65622, "*command"},
 { 65623, "*contrib"},
 { 65624, "*coref"},
 { 65625, "*corpauthor"},
 { 65626, "*date"},
 { 65627, "*editor"},
 { 65628, "*email"},
 { 65629, "*emphasis"},
 { 65630, "*epigraph"},
 { 65631, "*equation"},
 { 65632, "*example"},
 { 65633, "*exceptionname"},
 { 65634, "*fieldsynopsis"},
 { 65635, "*footnoteref"},
 { 65636, "*formalpara"},
 { 65637, "*funcparams"},
 { 65638, "*function"},
 { 65639, "*glossentry"},
 { 65640, "*glosslist"},
 { 65641, "*glossterm"},
 { 65642, "*graphic"},
 { 65643, "*graphicco"},
 { 65644, "*guibutton"},
 { 65645, "*highlights"},
 { 65646, "*honorific"},
 { 65647, "*imageobject"},
 { 65648, "*imageobjectco"},
 { 65649, "*indexdiv"},
 { 65650, "*indexentry"},
 { 65651, "*indexterm"},
 { 65652, "*informalequation"},
 { 65653, "*informalexample"},
 { 65654, "*informaltable"},
 { 65655, "*initializer"},
 { 65656, "*inlineequation"},
 { 65657, "*inlinegraphic"},
 { 65658, "*inlinemediaobject"},
 { 65659, "*interfacename"},
 { 65660, "*itemizedlist"},
 { 65661, "*keycap"},
 { 65662, "*label"},
 { 65663, "*legalnotice"},
 { 65664, "*lineannotation"},
 { 65665, "*link"},
 { 65666, "*listitem"},
 { 65667, "*literallayout"},
 { 65668, "*mediaobject"},
 { 65669, "*methodname"},
 { 65670, "*methodparam"},
 { 65671, "*modespec"},
 { 65672, "*modifier"},
 { 65673, "*msgexplan"},
 { 65674, "*msgset"},
 { 65675, "*msgtext"},
 { 65676, "*objectinfo"},
 { 65677, "*ooclass"},
 { 65678, "*option"},
 { 65679, "*optional"},
 { 65680, "*orgname"},
 { 65681, "*package"},
 { 65682, "*para"},
 { 65683, "*parameter"},
 { 65684, "*partintro"},
 { 65685, "*personblurb"},
 { 65686, "*personname"},
 { 65687, "*phrase"},
 { 65688, "*preface"},
 { 65689, "*procedure"},
 { 65690, "*programlisting"},
 { 65691, "*programlistingco"},
 { 65692, "*publishername"},
 { 65693, "*qandadiv"},
 { 65694, "*qandaentry"},
 { 65695, "*refentry"},
 { 65696, "*refentrytitle"},
 { 65697, "*refsect2"},
 { 65698, "*refsection"},
 { 65699, "*remark"},
 { 65700, "*replaceable"},
 { 65701, "*revhistory"},
 { 65702, "*row"},
 { 65703, "*sbr"},
 { 65704, "*screen"},
 { 65705, "*sect1"},
 { 65706, "*section"},
 { 65707, "*sidebar"},
 { 65708, "*simplesect"},
 { 65709, "*spanspec"},
 { 65710, "*step"},
 { 65711, "*subscript"},
 { 65712, "*subtitle"},
 { 65713, "*symbol"},
 { 65714, "*synopfragmentref"},
 { 65715, "*synopsis"},
 { 65716, "*table"},
 { 65717, "*textobject"},
 { 65718, "*tfoot"},
 { 65719, "*tgroup"},
 { 65720, "*thead"},
 { 65721, "*title"},
 { 65722, "*titleabbrev"},
 { 65723, "*toc"},
 { 65724, "*tocchap"},
 { 65725, "*tocentry"},
 { 65726, "*tr"},
 { 65727, "*trademark"},
 { 65728, "*type"},
 { 65729, "*varname"},
 { 65730, "*videoobject"},
 { 65731, "*void"},
 { 65732, "*beginpagefirst"},
 { 65733, "*blockinfofirst"},
 { 65734, "*titlefirst"},
 { 0, NULL }
 };

ipair DBtag::DocBookBLevelsSet[] = {
 { 47, 1 },
 { 92, 1 },
 { 234, 1 },
 { 324, 1 },
 { 0, 0 }
 };

ipair DBtag::DocBookBAtFirstSet[] = {
 { 10, 161 },
 { 18, -1 },
 { 23, 101 },
 { 30, 65732 },
 { 35, -1 },
 { 45, 65733 },
 { 69, -1 },
 { 126, -1 },
 { 133, 129 },
 { 154, -1 },
 { 179, -1 },
 { 214, 333 },
 { 216, -1 },
 { 245, -1 },
 { 246, -1 },
 { 257, 256 },
 { 265, -1 },
 { 268, 57 },
 { 277, -1 },
 { 279, -1 },
 { 281, -1 },
 { 283, -1 },
 { 285, -1 },
 { 293, -1 },
 { 299, -1 },
 { 304, -1 },
 { 306, -1 },
 { 308, -1 },
 { 310, -1 },
 { 312, -1 },
 { 314, -1 },
 { 325, -1 },
 { 327, -1 },
 { 328, -1 },
 { 330, -1 },
 { 373, 65734 },
 { 0, 0 }
 };


// tagsets for DocBook 4.5 DTD for article root

npair DBtag::DocBookANamesSet[] = {
 { 1, "article"},
 { 2, "abbrev"},
 { 3, "abstract"},
 { 4, "accel"},
 { 5, "ackno"},
 { 6, "acronym"},
 { 7, "action"},
 { 8, "address"},
 { 9, "affiliation"},
 { 10, "alt"},
 { 11, "anchor"},
 { 12, "answer"},
 { 13, "appendix"},
 { 14, "appendixinfo"},
 { 15, "application"},
 { 16, "area"},
 { 17, "areaset"},
 { 18, "areaspec"},
 { 19, "arg"},
 { 20, "articleinfo"},
 { 21, "artpagenums"},
 { 22, "attribution"},
 { 23, "audiodata"},
 { 24, "audioobject"},
 { 25, "author"},
 { 26, "authorblurb"},
 { 27, "authorgroup"},
 { 28, "authorinitials"},
 { 29, "beginpage"},
 { 30, "bibliocoverage"},
 { 31, "bibliodiv"},
 { 32, "biblioentry"},
 { 33, "bibliography"},
 { 34, "bibliographyinfo"},
 { 35, "biblioid"},
 { 36, "bibliolist"},
 { 37, "bibliomisc"},
 { 38, "bibliomixed"},
 { 39, "bibliomset"},
 { 40, "biblioref"},
 { 41, "bibliorelation"},
 { 42, "biblioset"},
 { 43, "bibliosource"},
 { 44, "blockinfo"},
 { 45, "blockquote"},
 { 46, "bridgehead"},
 { 47, "callout"},
 { 48, "calloutlist"},
 { 49, "caption"},
 { 50, "caution"},
 { 51, "citation"},
 { 52, "citebiblioid"},
 { 53, "citerefentry"},
 { 54, "citetitle"},
 { 55, "city"},
 { 56, "classname"},
 { 57, "classsynopsis"},
 { 58, "classsynopsisinfo"},
 { 59, "cmdsynopsis"},
 { 60, "co"},
 { 61, "code"},
 { 62, "col"},
 { 63, "colgroup"},
 { 64, "collab"},
 { 65, "collabname"},
 { 66, "colophon"},
 { 67, "colspec"},
 { 68, "command"},
 { 69, "computeroutput"},
 { 70, "confdates"},
 { 71, "confgroup"},
 { 72, "confnum"},
 { 73, "confsponsor"},
 { 74, "conftitle"},
 { 75, "constant"},
 { 76, "constructorsynopsis"},
 { 77, "contractnum"},
 { 78, "contractsponsor"},
 { 79, "contrib"},
 { 80, "copyright"},
 { 81, "coref"},
 { 82, "corpauthor"},
 { 83, "corpcredit"},
 { 84, "corpname"},
 { 85, "country"},
 { 86, "database"},
 { 87, "date"},
 { 88, "destructorsynopsis"},
 { 89, "edition"},
 { 90, "editor"},
 { 91, "email"},
 { 92, "emphasis"},
 { 93, "entry"},
 { 94, "entrytbl"},
 { 95, "envar"},
 { 96, "epigraph"},
 { 97, "equation"},
 { 98, "errorcode"},
 { 99, "errorname"},
 { 100, "errortext"},
 { 101, "errortype"},
 { 102, "example"},
 { 103, "exceptionname"},
 { 104, "fax"},
 { 105, "fieldsynopsis"},
 { 106, "figure"},
 { 107, "filename"},
 { 108, "firstname"},
 { 109, "firstterm"},
 { 110, "footnote"},
 { 111, "footnoteref"},
 { 112, "foreignphrase"},
 { 113, "formalpara"},
 { 114, "funcdef"},
 { 115, "funcparams"},
 { 116, "funcprototype"},
 { 117, "funcsynopsis"},
 { 118, "funcsynopsisinfo"},
 { 119, "function"},
 { 120, "glossary"},
 { 121, "glossaryinfo"},
 { 122, "glossdef"},
 { 123, "glossdiv"},
 { 124, "glossentry"},
 { 125, "glosslist"},
 { 126, "glosssee"},
 { 127, "glossseealso"},
 { 128, "glossterm"},
 { 129, "graphic"},
 { 130, "graphicco"},
 { 131, "group"},
 { 132, "guibutton"},
 { 133, "guiicon"},
 { 134, "guilabel"},
 { 135, "guimenu"},
 { 136, "guimenuitem"},
 { 137, "guisubmenu"},
 { 138, "hardware"},
 { 139, "highlights"},
 { 140, "holder"},
 { 141, "honorific"},
 { 142, "imagedata"},
 { 143, "imageobject"},
 { 144, "imageobjectco"},
 { 145, "important"},
 { 146, "index"},
 { 147, "indexdiv"},
 { 148, "indexentry"},
 { 149, "indexinfo"},
 { 150, "indexterm"},
 { 151, "informalequation"},
 { 152, "informalexample"},
 { 153, "informalfigure"},
 { 154, "informaltable"},
 { 155, "initializer"},
 { 156, "inlineequation"},
 { 157, "inlinegraphic"},
 { 158, "inlinemediaobject"},
 { 159, "interface"},
 { 160, "interfacename"},
 { 161, "invpartnumber"},
 { 162, "isbn"},
 { 163, "issn"},
 { 164, "issuenum"},
 { 165, "itemizedlist"},
 { 166, "itermset"},
 { 167, "jobtitle"},
 { 168, "keycap"},
 { 169, "keycode"},
 { 170, "keycombo"},
 { 171, "keysym"},
 { 172, "keyword"},
 { 173, "keywordset"},
 { 174, "label"},
 { 175, "legalnotice"},
 { 176, "lineage"},
 { 177, "lineannotation"},
 { 178, "link"},
 { 179, "listitem"},
 { 180, "literal"},
 { 181, "literallayout"},
 { 182, "lot"},
 { 183, "lotentry"},
 { 184, "manvolnum"},
 { 185, "markup"},
 { 186, "mathphrase"},
 { 187, "medialabel"},
 { 188, "mediaobject"},
 { 189, "mediaobjectco"},
 { 190, "member"},
 { 191, "menuchoice"},
 { 192, "methodname"},
 { 193, "methodparam"},
 { 194, "methodsynopsis"},
 { 195, "modespec"},
 { 196, "modifier"},
 { 197, "mousebutton"},
 { 198, "msg"},
 { 199, "msgaud"},
 { 200, "msgentry"},
 { 201, "msgexplan"},
 { 202, "msginfo"},
 { 203, "msglevel"},
 { 204, "msgmain"},
 { 205, "msgorig"},
 { 206, "msgrel"},
 { 207, "msgset"},
 { 208, "msgsub"},
 { 209, "msgtext"},
 { 210, "note"},
 { 211, "objectinfo"},
 { 212, "olink"},
 { 213, "ooclass"},
 { 214, "ooexception"},
 { 215, "oointerface"},
 { 216, "option"},
 { 217, "optional"},
 { 218, "orderedlist"},
 { 219, "orgdiv"},
 { 220, "orgname"},
 { 221, "otheraddr"},
 { 222, "othercredit"},
 { 223, "othername"},
 { 224, "package"},
 { 225, "pagenums"},
 { 226, "para"},
 { 227, "paramdef"},
 { 228, "parameter"},
 { 229, "personblurb"},
 { 230, "personname"},
 { 231, "phone"},
 { 232, "phrase"},
 { 233, "pob"},
 { 234, "postcode"},
 { 235, "primary"},
 { 236, "primaryie"},
 { 237, "printhistory"},
 { 238, "procedure"},
 { 239, "productname"},
 { 240, "productnumber"},
 { 241, "programlisting"},
 { 242, "programlistingco"},
 { 243, "prompt"},
 { 244, "property"},
 { 245, "pubdate"},
 { 246, "publisher"},
 { 247, "publishername"},
 { 248, "pubsnumber"},
 { 249, "qandadiv"},
 { 250, "qandaentry"},
 { 251, "qandaset"},
 { 252, "question"},
 { 253, "quote"},
 { 254, "refclass"},
 { 255, "refdescriptor"},
 { 256, "refentry"},
 { 257, "refentryinfo"},
 { 258, "refentrytitle"},
 { 259, "refmeta"},
 { 260, "refmiscinfo"},
 { 261, "refname"},
 { 262, "refnamediv"},
 { 263, "refpurpose"},
 { 264, "refsect1"},
 { 265, "refsect1info"},
 { 266, "refsect2"},
 { 267, "refsect2info"},
 { 268, "refsect3"},
 { 269, "refsect3info"},
 { 270, "refsection"},
 { 271, "refsectioninfo"},
 { 272, "refsynopsisdiv"},
 { 273, "refsynopsisdivinfo"},
 { 274, "releaseinfo"},
 { 275, "remark"},
 { 276, "replaceable"},
 { 277, "returnvalue"},
 { 278, "revdescription"},
 { 279, "revhistory"},
 { 280, "revision"},
 { 281, "revnumber"},
 { 282, "revremark"},
 { 283, "row"},
 { 284, "sbr"},
 { 285, "screen"},
 { 286, "screenco"},
 { 287, "screeninfo"},
 { 288, "screenshot"},
 { 289, "secondary"},
 { 290, "secondaryie"},
 { 291, "sect1"},
 { 292, "sect1info"},
 { 293, "sect2"},
 { 294, "sect2info"},
 { 295, "sect3"},
 { 296, "sect3info"},
 { 297, "sect4"},
 { 298, "sect4info"},
 { 299, "sect5"},
 { 300, "sect5info"},
 { 301, "section"},
 { 302, "sectioninfo"},
 { 303, "see"},
 { 304, "seealso"},
 { 305, "seealsoie"},
 { 306, "seeie"},
 { 307, "seg"},
 { 308, "seglistitem"},
 { 309, "segmentedlist"},
 { 310, "segtitle"},
 { 311, "seriesvolnums"},
 { 312, "sgmltag"},
 { 313, "shortaffil"},
 { 314, "shortcut"},
 { 315, "sidebar"},
 { 316, "sidebarinfo"},
 { 317, "simpara"},
 { 318, "simplelist"},
 { 319, "simplemsgentry"},
 { 320, "simplesect"},
 { 321, "spanspec"},
 { 322, "state"},
 { 323, "step"},
 { 324, "stepalternatives"},
 { 325, "street"},
 { 326, "structfield"},
 { 327, "structname"},
 { 328, "subject"},
 { 329, "subjectset"},
 { 330, "subjectterm"},
 { 331, "subscript"},
 { 332, "substeps"},
 { 333, "subtitle"},
 { 334, "superscript"},
 { 335, "surname"},
 { 336, "symbol"},
 { 337, "synopfragment"},
 { 338, "synopfragmentref"},
 { 339, "synopsis"},
 { 340, "systemitem"},
 { 341, "table"},
 { 342, "task"},
 { 343, "taskprerequisites"},
 { 344, "taskrelated"},
 { 345, "tasksummary"},
 { 346, "tbody"},
 { 347, "td"},
 { 348, "term"},
 { 349, "termdef"},
 { 350, "tertiary"},
 { 351, "tertiaryie"},
 { 352, "textdata"},
 { 353, "textobject"},
 { 354, "tfoot"},
 { 355, "tgroup"},
 { 356, "th"},
 { 357, "thead"},
 { 358, "tip"},
 { 359, "title"},
 { 360, "titleabbrev"},
 { 361, "toc"},
 { 362, "tocback"},
 { 363, "tocchap"},
 { 364, "tocentry"},
 { 365, "tocfront"},
 { 366, "toclevel1"},
 { 367, "toclevel2"},
 { 368, "toclevel3"},
 { 369, "toclevel4"},
 { 370, "toclevel5"},
 { 371, "tocpart"},
 { 372, "token"},
 { 373, "tr"},
 { 374, "trademark"},
 { 375, "type"},
 { 376, "ulink"},
 { 377, "uri"},
 { 378, "userinput"},
 { 379, "varargs"},
 { 380, "variablelist"},
 { 381, "varlistentry"},
 { 382, "varname"},
 { 383, "videodata"},
 { 384, "videoobject"},
 { 385, "void"},
 { 386, "volumenum"},
 { 387, "warning"},
 { 388, "wordasword"},
 { 389, "xref"},
 { 390, "year"},
 { 0, NULL }
 };

ipair DBtag::DocBookAInlineSet[] = {
 { 2, 1 },
 { 4, 3 },
 { 5, 1 },
 { 6, 3 },
 { 7, 3 },
 { 8, 1 },
 { 10, 1 },
 { 15, 3 },
 { 19, 1 },
 { 21, 1 },
 { 22, 1 },
 { 28, 1 },
 { 30, 1 },
 { 35, 1 },
 { 37, 1 },
 { 38, 1 },
 { 39, 3 },
 { 40, 2 },
 { 41, 1 },
 { 43, 1 },
 { 46, 1 },
 { 49, 1 },
 { 51, 3 },
 { 52, 1 },
 { 53, 2 },
 { 54, 1 },
 { 55, 3 },
 { 56, 3 },
 { 58, 1 },
 { 60, 2 },
 { 61, 3 },
 { 65, 1 },
 { 68, 3 },
 { 69, 3 },
 { 70, 1 },
 { 72, 1 },
 { 73, 1 },
 { 74, 1 },
 { 75, 3 },
 { 77, 1 },
 { 78, 1 },
 { 79, 1 },
 { 81, 2 },
 { 82, 1 },
 { 83, 1 },
 { 84, 1 },
 { 85, 3 },
 { 86, 3 },
 { 87, 1 },
 { 89, 1 },
 { 91, 3 },
 { 92, 3 },
 { 93, 1 },
 { 95, 3 },
 { 98, 3 },
 { 99, 3 },
 { 100, 3 },
 { 101, 3 },
 { 103, 3 },
 { 104, 3 },
 { 107, 3 },
 { 108, 1 },
 { 109, 3 },
 { 110, 2 },
 { 111, 2 },
 { 112, 3 },
 { 114, 1 },
 { 115, 1 },
 { 118, 1 },
 { 119, 3 },
 { 126, 1 },
 { 127, 1 },
 { 128, 3 },
 { 132, 3 },
 { 133, 3 },
 { 134, 3 },
 { 135, 3 },
 { 136, 3 },
 { 137, 3 },
 { 138, 3 },
 { 140, 1 },
 { 141, 1 },
 { 155, 1 },
 { 156, 2 },
 { 157, 2 },
 { 158, 2 },
 { 159, 3 },
 { 160, 3 },
 { 161, 1 },
 { 162, 1 },
 { 163, 1 },
 { 164, 1 },
 { 167, 1 },
 { 168, 3 },
 { 169, 3 },
 { 170, 2 },
 { 171, 3 },
 { 172, 1 },
 { 174, 1 },
 { 176, 1 },
 { 177, 3 },
 { 178, 3 },
 { 180, 3 },
 { 181, 5 },
 { 183, 1 },
 { 184, 1 },
 { 185, 3 },
 { 186, 1 },
 { 187, 3 },
 { 190, 1 },
 { 191, 2 },
 { 192, 3 },
 { 195, 1 },
 { 196, 1 },
 { 197, 3 },
 { 199, 1 },
 { 203, 1 },
 { 205, 1 },
 { 212, 3 },
 { 213, 2 },
 { 214, 2 },
 { 215, 2 },
 { 216, 3 },
 { 217, 3 },
 { 219, 1 },
 { 220, 1 },
 { 221, 3 },
 { 223, 1 },
 { 224, 3 },
 { 225, 1 },
 { 226, 1 },
 { 227, 1 },
 { 228, 3 },
 { 231, 3 },
 { 232, 3 },
 { 233, 3 },
 { 234, 3 },
 { 235, 1 },
 { 236, 1 },
 { 239, 1 },
 { 240, 1 },
 { 241, 5 },
 { 243, 3 },
 { 244, 3 },
 { 245, 1 },
 { 247, 1 },
 { 248, 1 },
 { 253, 3 },
 { 254, 1 },
 { 255, 1 },
 { 258, 1 },
 { 260, 1 },
 { 261, 1 },
 { 263, 1 },
 { 274, 1 },
 { 275, 1 },
 { 276, 3 },
 { 277, 3 },
 { 281, 1 },
 { 282, 1 },
 { 285, 5 },
 { 287, 1 },
 { 289, 1 },
 { 290, 1 },
 { 303, 1 },
 { 304, 1 },
 { 305, 1 },
 { 306, 1 },
 { 307, 1 },
 { 310, 1 },
 { 311, 1 },
 { 312, 3 },
 { 313, 1 },
 { 317, 1 },
 { 322, 3 },
 { 325, 3 },
 { 326, 3 },
 { 327, 3 },
 { 330, 1 },
 { 331, 3 },
 { 333, 1 },
 { 334, 3 },
 { 335, 1 },
 { 336, 3 },
 { 338, 1 },
 { 339, 5 },
 { 340, 3 },
 { 347, 1 },
 { 348, 1 },
 { 349, 3 },
 { 350, 1 },
 { 351, 1 },
 { 356, 1 },
 { 359, 1 },
 { 360, 1 },
 { 362, 1 },
 { 364, 1 },
 { 365, 1 },
 { 372, 3 },
 { 374, 3 },
 { 375, 3 },
 { 376, 3 },
 { 377, 3 },
 { 378, 3 },
 { 382, 3 },
 { 386, 1 },
 { 388, 3 },
 { 389, 2 },
 { 390, 1 },
 { 0, 0 }
 };

ipair DBtag::DocBookASingleSet[] = {
 { 3, 65588 },
 { 4, 65589 },
 { 5, 1 },
 { 6, 65590 },
 { 7, 65591 },
 { 8, 65592 },
 { 9, 65593 },
 { 10, 65594 },
 { 11, 65595 },
 { 12, 250 },
 { 13, 1 },
 { 14, 13 },
 { 15, 65596 },
 { 16, 65597 },
 { 17, 18 },
 { 18, 65598 },
 { 19, 65599 },
 { 20, 65600 },
 { 21, 65601 },
 { 22, 65602 },
 { 23, 24 },
 { 24, 65724 },
 { 25, 65603 },
 { 26, 65604 },
 { 27, 65601 },
 { 28, 65605 },
 { 29, 65606 },
 { 30, 65601 },
 { 31, 33 },
 { 32, 65607 },
 { 33, 65608 },
 { 34, 33 },
 { 35, 65601 },
 { 36, 65639 },
 { 37, 65601 },
 { 38, 65607 },
 { 39, 65609 },
 { 40, 65634 },
 { 41, 65601 },
 { 42, 65601 },
 { 43, 65601 },
 { 44, 65610 },
 { 45, 65611 },
 { 46, 65612 },
 { 47, 48 },
 { 48, 65613 },
 { 49, 65614 },
 { 50, 65615 },
 { 51, 65634 },
 { 52, 65616 },
 { 53, 65634 },
 { 54, 65616 },
 { 55, 8 },
 { 56, 65617 },
 { 57, 65710 },
 { 58, 57 },
 { 59, 65710 },
 { 60, 65618 },
 { 61, 65591 },
 { 62, 65619 },
 { 63, 65714 },
 { 64, 65626 },
 { 65, 64 },
 { 66, 1 },
 { 67, 65620 },
 { 68, 65621 },
 { 69, 65591 },
 { 70, 71 },
 { 71, 65601 },
 { 72, 71 },
 { 73, 71 },
 { 74, 71 },
 { 75, 65591 },
 { 76, 65633 },
 { 77, 65601 },
 { 78, 65601 },
 { 79, 65622 },
 { 80, 65601 },
 { 81, 65623 },
 { 82, 65624 },
 { 83, 65624 },
 { 84, 65601 },
 { 85, 8 },
 { 86, 65591 },
 { 87, 65625 },
 { 88, 65633 },
 { 89, 65601 },
 { 90, 65626 },
 { 91, 65627 },
 { 92, 65628 },
 { 93, 283 },
 { 94, 283 },
 { 95, 65591 },
 { 96, 65629 },
 { 97, 65630 },
 { 98, 65591 },
 { 99, 65591 },
 { 100, 65591 },
 { 101, 65591 },
 { 102, 65631 },
 { 103, 65632 },
 { 104, 8 },
 { 105, 65633 },
 { 106, 65630 },
 { 107, 65591 },
 { 108, 65645 },
 { 109, 65634 },
 { 110, 65634 },
 { 111, 65634 },
 { 112, 65634 },
 { 113, 65635 },
 { 114, 116 },
 { 115, 65636 },
 { 116, 117 },
 { 117, 65710 },
 { 118, 117 },
 { 119, 65637 },
 { 120, 65666 },
 { 121, 120 },
 { 122, 124 },
 { 123, 120 },
 { 124, 65638 },
 { 125, 65639 },
 { 126, 124 },
 { 127, 122 },
 { 128, 65640 },
 { 129, 65641 },
 { 130, 65642 },
 { 131, 65599 },
 { 132, 65643 },
 { 133, 65643 },
 { 134, 65643 },
 { 135, 65643 },
 { 136, 65643 },
 { 137, 65643 },
 { 138, 65591 },
 { 139, 65644 },
 { 140, 80 },
 { 141, 65645 },
 { 142, 143 },
 { 143, 65646 },
 { 144, 65647 },
 { 145, 65615 },
 { 146, 65666 },
 { 147, 146 },
 { 148, 65648 },
 { 149, 146 },
 { 150, 65649 },
 { 151, 65650 },
 { 152, 65651 },
 { 153, 65651 },
 { 154, 65652 },
 { 155, 65653 },
 { 156, 65654 },
 { 157, 65655 },
 { 158, 65656 },
 { 159, 65643 },
 { 160, 65657 },
 { 161, 65601 },
 { 162, 65601 },
 { 163, 65601 },
 { 164, 65601 },
 { 165, 65658 },
 { 166, 65661 },
 { 167, 9 },
 { 168, 65659 },
 { 169, 65591 },
 { 170, 65659 },
 { 171, 65659 },
 { 172, 173 },
 { 173, 65661 },
 { 174, 65660 },
 { 175, 65661 },
 { 176, 65645 },
 { 177, 65662 },
 { 178, 65663 },
 { 179, 65664 },
 { 180, 65591 },
 { 181, 65665 },
 { 182, 65666 },
 { 183, 182 },
 { 184, 65692 },
 { 185, 65591 },
 { 186, 65594 },
 { 187, 65591 },
 { 188, 65667 },
 { 189, 65642 },
 { 190, 318 },
 { 191, 65591 },
 { 192, 65668 },
 { 193, 65669 },
 { 194, 65633 },
 { 195, 65670 },
 { 196, 65671 },
 { 197, 65659 },
 { 198, 200 },
 { 199, 202 },
 { 200, 207 },
 { 201, 65672 },
 { 202, 200 },
 { 203, 202 },
 { 204, 198 },
 { 205, 202 },
 { 206, 198 },
 { 207, 65673 },
 { 208, 198 },
 { 209, 65674 },
 { 210, 65615 },
 { 211, 65675 },
 { 212, 65663 },
 { 213, 65676 },
 { 214, 65676 },
 { 215, 65676 },
 { 216, 65677 },
 { 217, 65678 },
 { 218, 65658 },
 { 219, 9 },
 { 220, 65679 },
 { 221, 8 },
 { 222, 65624 },
 { 223, 65645 },
 { 224, 65680 },
 { 225, 65601 },
 { 226, 65681 },
 { 227, 116 },
 { 228, 65682 },
 { 229, 65683 },
 { 230, 65684 },
 { 231, 8 },
 { 232, 65685 },
 { 233, 8 },
 { 234, 8 },
 { 235, 150 },
 { 236, 148 },
 { 237, 65601 },
 { 238, 65686 },
 { 239, 65616 },
 { 240, 65616 },
 { 241, 65687 },
 { 242, 65688 },
 { 243, 65591 },
 { 244, 65591 },
 { 245, 65601 },
 { 246, 65601 },
 { 247, 65689 },
 { 248, 65601 },
 { 249, 65690 },
 { 250, 65691 },
 { 251, 65673 },
 { 252, 250 },
 { 253, 65634 },
 { 254, 262 },
 { 255, 262 },
 { 256, 65666 },
 { 257, 256 },
 { 258, 65692 },
 { 259, 256 },
 { 260, 259 },
 { 261, 262 },
 { 262, 256 },
 { 263, 262 },
 { 264, 256 },
 { 265, 264 },
 { 266, 65693 },
 { 267, 266 },
 { 268, 266 },
 { 269, 268 },
 { 270, 65694 },
 { 271, 270 },
 { 272, 256 },
 { 273, 272 },
 { 274, 65601 },
 { 275, 65695 },
 { 276, 65696 },
 { 277, 65591 },
 { 278, 280 },
 { 279, 65697 },
 { 280, 279 },
 { 281, 280 },
 { 282, 280 },
 { 283, 65698 },
 { 284, 65699 },
 { 285, 65700 },
 { 286, 65688 },
 { 287, 288 },
 { 288, 65688 },
 { 289, 150 },
 { 290, 148 },
 { 291, 65701 },
 { 292, 291 },
 { 293, 291 },
 { 294, 293 },
 { 295, 293 },
 { 296, 295 },
 { 297, 295 },
 { 298, 297 },
 { 299, 297 },
 { 300, 299 },
 { 301, 65702 },
 { 302, 301 },
 { 303, 150 },
 { 304, 150 },
 { 305, 148 },
 { 306, 148 },
 { 307, 308 },
 { 308, 309 },
 { 309, 65658 },
 { 310, 309 },
 { 311, 65601 },
 { 312, 65591 },
 { 313, 9 },
 { 314, 191 },
 { 315, 65703 },
 { 316, 315 },
 { 317, 65635 },
 { 318, 65658 },
 { 319, 207 },
 { 320, 65666 },
 { 321, 65704 },
 { 322, 8 },
 { 323, 65705 },
 { 324, 323 },
 { 325, 8 },
 { 326, 65591 },
 { 327, 65591 },
 { 328, 329 },
 { 329, 65661 },
 { 330, 328 },
 { 331, 65706 },
 { 332, 323 },
 { 333, 65707 },
 { 334, 65706 },
 { 335, 65645 },
 { 336, 65708 },
 { 337, 59 },
 { 338, 65709 },
 { 339, 65710 },
 { 340, 65591 },
 { 341, 65711 },
 { 342, 65673 },
 { 343, 342 },
 { 344, 342 },
 { 345, 342 },
 { 346, 65715 },
 { 347, 373 },
 { 348, 381 },
 { 349, 65634 },
 { 350, 150 },
 { 351, 148 },
 { 352, 353 },
 { 353, 65712 },
 { 354, 65713 },
 { 355, 65714 },
 { 356, 373 },
 { 357, 65715 },
 { 358, 65615 },
 { 359, 65716 },
 { 360, 65717 },
 { 361, 65666 },
 { 362, 361 },
 { 363, 65718 },
 { 364, 65719 },
 { 365, 361 },
 { 366, 363 },
 { 367, 366 },
 { 368, 367 },
 { 369, 368 },
 { 370, 369 },
 { 371, 361 },
 { 372, 65591 },
 { 373, 65720 },
 { 374, 65721 },
 { 375, 65722 },
 { 376, 65663 },
 { 377, 65591 },
 { 378, 65591 },
 { 379, 116 },
 { 380, 65658 },
 { 381, 380 },
 { 382, 65723 },
 { 383, 384 },
 { 384, 65724 },
 { 385, 65725 },
 { 386, 65601 },
 { 387, 65615 },
 { 388, 65634 },
 { 389, 65634 },
 { 390, 80 },
 { 0, 0 }
 };

long DBtag::Part1DocBookAPar[] = { 
14, 20, 32, 34, 38, 39, 42, 44, 
121, 149, 211, 257, 265, 267, 269, 271, 
273, 292, 294, 316, 0 };

long DBtag::Part2DocBookAPar[] = { 
1, 33, 45, 50, 102, 120, 145, 175, 
201, 210, 238, 249, 251, 315, 323, 343, 
344, 345, 358, 387, 0 };

long DBtag::Part3DocBookAPar[] = { 
13, 31, 66, 123, 146, 264, 266, 268, 
270, 272, 291, 293, 295, 297, 299, 301, 
320, 0 };

long DBtag::Part4DocBookAPar[] = { 
36, 48, 97, 106, 125, 147, 165, 182, 
207, 218, 296, 298, 300, 302, 309, 341, 
342, 361, 380, 0 };

long DBtag::Part5DocBookAPar[] = { 
15, 37, 51, 54, 92, 109, 112, 128, 
178, 212, 232, 239, 253, 275, 333, 339, 
349, 359, 360, 376, 0 };

long DBtag::Part6DocBookAPar[] = { 
22, 46, 93, 126, 127, 177, 181, 190, 
199, 226, 241, 258, 285, 287, 307, 310, 
317, 347, 348, 356, 0 };

long DBtag::Part7DocBookAPar[] = { 
183, 235, 236, 263, 289, 290, 303, 304, 
305, 306, 350, 351, 362, 364, 365, 0 };

long DBtag::Part8DocBookAPar[] = { 
124, 296, 298, 300, 302, 0 };

long DBtag::Part9DocBookAPar[] = { 
2, 6, 174, 184, 388, 0 };

long DBtag::Part10DocBookAPar[] = { 
21, 28, 52, 77, 78, 82, 83, 84, 
87, 89, 91, 161, 162, 163, 195, 220, 
240, 331, 334, 374, 0 };

long DBtag::Part11DocBookAPar[] = { 
5, 30, 35, 41, 43, 79, 108, 141, 
164, 176, 223, 225, 245, 247, 248, 274, 
311, 325, 335, 386, 0 };

long DBtag::Part12DocBookAPar[] = { 
55, 65, 70, 72, 73, 74, 85, 104, 
140, 167, 219, 221, 231, 233, 234, 281, 
282, 313, 322, 390, 0 };

long DBtag::Part13DocBookAPar[] = { 
7, 61, 68, 69, 86, 107, 115, 118, 
119, 138, 160, 168, 180, 216, 217, 228, 
244, 276, 340, 378, 0 };

long DBtag::Part14DocBookAPar[] = { 
58, 147, 256, 260, 262, 0 };

long DBtag::Part15DocBookAPar[] = { 
12, 47, 179, 209, 252, 278, 0 };

long DBtag::Part16DocBookAPar[] = { 
93, 110, 122, 130, 152, 226, 347, 356, 
0 };

long DBtag::Part17DocBookAPar[] = { 
1, 33, 120, 238, 249, 251, 315, 343, 
344, 345, 0 };

long DBtag::Part18DocBookAPar[] = { 
9, 296, 298, 300, 302, 0 };

long DBtag::Part19DocBookAPar[] = { 
21, 28, 52, 77, 78, 82, 83, 84, 
87, 89, 91, 161, 162, 163, 195, 220, 
240, 260, 0 };

long DBtag::Part20DocBookAPar[] = { 
13, 45, 50, 145, 146, 165, 201, 210, 
218, 291, 293, 295, 297, 299, 301, 320, 
323, 358, 380, 387, 0 };

long DBtag::Part21DocBookAPar[] = { 
31, 123, 264, 266, 268, 270, 272, 0 };

long DBtag::Part22DocBookAPar[] = { 
8, 25, 90, 222, 296, 298, 300, 302, 
0 };

long DBtag::Part23DocBookAPar[] = { 
7, 58, 61, 68, 69, 86, 107, 115, 
118, 119, 138, 160, 168, 180, 216, 217, 
228, 244, 340, 378, 0 };

long DBtag::Part24DocBookAPar[] = { 
56, 75, 95, 98, 99, 100, 101, 103, 
132, 133, 134, 135, 136, 137, 159, 169, 
171, 185, 192, 224, 0 };

long DBtag::Part25DocBookAPar[] = { 
4, 155, 187, 196, 197, 203, 205, 243, 
260, 277, 312, 326, 327, 336, 372, 375, 
377, 382, 0 };

long DBtag::Part26DocBookAPar[] = { 
58, 183, 235, 236, 289, 290, 303, 304, 
305, 306, 350, 351, 362, 364, 365, 0 };

long DBtag::Part27DocBookAPar[] = { 
14, 20, 34, 44, 97, 102, 106, 121, 
149, 151, 153, 154, 211, 257, 265, 271, 
288, 316, 339, 341, 0 };

long DBtag::Part28DocBookAPar[] = { 
147, 267, 269, 273, 292, 294, 296, 298, 
300, 302, 0 };

long DBtag::Part29DocBookAPar[] = { 
93, 110, 122, 152, 226, 347, 356, 0 };

long DBtag::Part30DocBookAPar[] = { 
58, 118, 181, 241, 285, 339, 0 };

long DBtag::Part31DocBookAPar[] = { 
106, 152, 153, 165, 218, 341, 342, 380, 
0 };

long DBtag::Part32DocBookAPar[] = { 
102, 175, 238, 343, 344, 345, 0 };

long DBtag::Part33DocBookAPar[] = { 
14, 20, 34, 44, 121, 149, 211, 257, 
265, 267, 269, 271, 273, 292, 294, 296, 
298, 300, 302, 316, 0 };

long DBtag::Part34DocBookAPar[] = { 
132, 133, 134, 135, 136, 137, 0 };

long DBtag::Part35DocBookAPar[] = { 
4, 122, 155, 187, 196, 197, 203, 205, 
243, 277, 312, 326, 327, 336, 362, 365, 
372, 375, 377, 382, 0 };

long DBtag::Part36DocBookAPar[] = { 
13, 146, 291, 293, 295, 297, 299, 301, 
320, 0 };

long DBtag::Part37DocBookAPar[] = { 
76, 88, 194, 255, 261, 374, 0 };

long DBtag::Part38DocBookAPar[] = { 
106, 147, 152, 153, 226, 0 };

long DBtag::Part39DocBookAPar[] = { 
1, 33, 47, 120, 179, 209, 0 };

long DBtag::Part40DocBookAPar[] = { 
15, 37, 51, 54, 92, 109, 112, 128, 
178, 181, 199, 212, 232, 239, 241, 253, 
275, 339, 349, 376, 0 };

long DBtag::Part41DocBookAPar[] = { 
22, 126, 127, 177, 183, 190, 258, 285, 
287, 307, 317, 348, 364, 0 };

long DBtag::Part42DocBookAPar[] = { 
193, 227, 255, 261, 374, 0 };

long DBtag::Part43DocBookAPar[] = { 
280, 296, 298, 300, 302, 0 };

long DBtag::Part44DocBookAPar[] = { 
45, 165, 201, 218, 323, 380, 0 };

long DBtag::Part45DocBookAPar[] = { 
3, 26, 49, 93, 96, 110, 122, 139, 
147, 152, 165, 218, 229, 237, 347, 353, 
356, 380, 0 };

long DBtag::Part46DocBookAPar[] = { 
27, 296, 298, 300, 302, 0 };

long DBtag::Part47DocBookAPar[] = { 
102, 106, 147, 153, 362, 365, 0 };

long DBtag::Part48DocBookAPar[] = { 
49, 106, 147, 153, 165, 218, 353, 380, 
0 };

long DBtag::Part49DocBookAPar[] = { 
45, 50, 122, 145, 201, 210, 226, 323, 
358, 387, 0 };

long DBtag::Part50DocBookAPar[] = { 
45, 50, 145, 201, 210, 323, 358, 387, 
0 };

long DBtag::Part51DocBookAPar[] = { 
13, 1, 291, 293, 295, 297, 299, 301, 
0 };

long DBtag::abbrevDocBookAPar[] = { 
65536, 65540, 65541, 65542, 65543, 0 };

long DBtag::abstractDocBookAPar[] = { 
238, 296, 298, 300, 302, 343, 344, 345, 
65536, 65556, 65571, 65574, 65579, 0 };

long DBtag::accelDocBookAPar[] = { 
159, 65569, 0 };

long DBtag::acronymDocBookAPar[] = { 
124, 340, 65540, 65541, 65542, 65544, 0 };

long DBtag::actionDocBookAPar[] = { 
255, 261, 374, 65540, 65541, 65542, 65558, 0 };

long DBtag::addressDocBookAPar[] = { 
25, 71, 90, 102, 110, 122, 222, 246, 
65536, 65550, 65552, 65553, 65555, 65556, 65573, 0 };

long DBtag::affiliationDocBookAPar[] = { 
64, 65536, 65557, 0 };

long DBtag::altDocBookAPar[] = { 
97, 151, 156, 0 };

long DBtag::anchorDocBookAPar[] = { 
58, 147, 331, 334, 374, 65540, 65541, 65542, 
65544, 65548, 65550, 65552, 65555, 65556, 0 };

long DBtag::applicationDocBookAPar[] = { 
254, 255, 261, 374, 65540, 65541, 65542, 65558, 
0 };

long DBtag::areaDocBookAPar[] = { 
17, 18, 0 };

long DBtag::areaspecDocBookAPar[] = { 
130, 144, 242, 286, 0 };

long DBtag::argDocBookAPar[] = { 
19, 59, 131, 337, 0 };

long DBtag::articleinfoDocBookAPar[] = { 
1, 32, 0 };

long DBtag::artpagenumsDocBookAPar[] = { 
296, 298, 300, 302, 65536, 0 };

long DBtag::attributionDocBookAPar[] = { 
45, 96, 0 };

long DBtag::authorDocBookAPar[] = { 
27, 65536, 65540, 65541, 65542, 65578, 0 };

long DBtag::authorblurbDocBookAPar[] = { 
238, 343, 344, 345, 65536, 65556, 65557, 65571, 
65574, 65579, 0 };

long DBtag::authorinitialsDocBookAPar[] = { 
65536, 65540, 65541, 65542, 65578, 0 };

long DBtag::beginpageDocBookAPar[] = { 
93, 182, 256, 263, 315, 347, 356, 361, 
65544, 65555, 65556, 65558, 65559, 65567, 65570, 65573, 
65574, 65575, 65576, 0 };

long DBtag::biblioentryDocBookAPar[] = { 
31, 33, 36, 0 };

long DBtag::bibliographyDocBookAPar[] = { 
120, 65586, 0 };

long DBtag::bibliomsetDocBookAPar[] = { 
38, 39, 0 };

long DBtag::blockinfoDocBookAPar[] = { 
36, 45, 97, 125, 151, 154, 207, 249, 
250, 251, 65566, 65567, 0 };

long DBtag::blockquoteDocBookAPar[] = { 
49, 110, 122, 165, 218, 353, 380, 65537, 
65538, 65550, 65573, 0 };

long DBtag::bridgeheadDocBookAPar[] = { 
65550, 65552, 65555, 65556, 0 };

long DBtag::calloutlistDocBookAPar[] = { 
49, 139, 144, 242, 286, 353, 65537, 65538, 
65550, 65551, 0 };

long DBtag::captionDocBookAPar[] = { 
188, 341, 0 };

long DBtag::cautionDocBookAPar[] = { 
49, 93, 139, 175, 226, 347, 353, 356, 
65538, 65550, 65552, 65579, 0 };

long DBtag::citetitleDocBookAPar[] = { 
296, 298, 300, 302, 65536, 65540, 65541, 65542, 
0 };

long DBtag::classnameDocBookAPar[] = { 
213, 255, 261, 374, 65540, 65541, 65542, 65558, 
0 };

long DBtag::coDocBookAPar[] = { 
69, 181, 241, 243, 276, 285, 339, 340, 
378, 0 };

long DBtag::colDocBookAPar[] = { 
63, 154, 341, 0 };

long DBtag::colspecDocBookAPar[] = { 
94, 354, 355, 357, 0 };

long DBtag::commandDocBookAPar[] = { 
59, 255, 261, 374, 65540, 65541, 65542, 65558, 
0 };

long DBtag::contribDocBookAPar[] = { 
65536, 65557, 0 };

long DBtag::corefDocBookAPar[] = { 
181, 241, 285, 339, 0 };

long DBtag::corpauthorDocBookAPar[] = { 
65536, 65540, 65541, 65542, 65581, 0 };

long DBtag::dateDocBookAPar[] = { 
65536, 65578, 0 };

long DBtag::editorDocBookAPar[] = { 
65536, 65581, 0 };

long DBtag::emailDocBookAPar[] = { 
8, 25, 90, 222, 255, 261, 374, 65540, 
65541, 65542, 65558, 0 };

long DBtag::emphasisDocBookAPar[] = { 
186, 260, 65540, 65541, 65542, 65544, 65545, 65546, 
65547, 0 };

long DBtag::epigraphDocBookAPar[] = { 
238, 343, 344, 345, 65556, 65571, 65574, 65579, 
0 };

long DBtag::equationDocBookAPar[] = { 
65550, 65552, 65556, 65571, 65584, 0 };

long DBtag::exampleDocBookAPar[] = { 
342, 65550, 65552, 65556, 65571, 65584, 0 };

long DBtag::exceptionnameDocBookAPar[] = { 
214, 65540, 65541, 65542, 65558, 65572, 0 };

long DBtag::fieldsynopsisDocBookAPar[] = { 
57, 65550, 65552, 65555, 65556, 65564, 65575, 65576, 
65582, 0 };

long DBtag::footnoterefDocBookAPar[] = { 
65540, 65541, 65542, 0 };

long DBtag::formalparaDocBookAPar[] = { 
65537, 65538, 65550, 65580, 0 };

long DBtag::funcparamsDocBookAPar[] = { 
193, 227, 0 };

long DBtag::functionDocBookAPar[] = { 
114, 255, 261, 374, 65540, 65541, 65542, 65558, 
0 };

long DBtag::glossentryDocBookAPar[] = { 
120, 123, 125, 0 };

long DBtag::glosslistDocBookAPar[] = { 
49, 139, 353, 65537, 65538, 65550, 65564, 0 };

long DBtag::glosstermDocBookAPar[] = { 
124, 65540, 65541, 65542, 0 };

long DBtag::graphicDocBookAPar[] = { 
156, 65550, 65551, 65552, 65555, 65556, 65562, 65563, 
0 };

long DBtag::graphiccoDocBookAPar[] = { 
102, 110, 122, 288, 65550, 65552, 65555, 65556, 
65573, 0 };

long DBtag::guibuttonDocBookAPar[] = { 
191, 255, 261, 374, 65540, 65541, 65542, 65558, 
0 };

long DBtag::highlightsDocBookAPar[] = { 
65550, 65552, 65556, 65571, 65579, 0 };

long DBtag::honorificDocBookAPar[] = { 
230, 65536, 65557, 0 };

long DBtag::imageobjectDocBookAPar[] = { 
144, 158, 188, 0 };

long DBtag::imageobjectcoDocBookAPar[] = { 
158, 188, 189, 0 };

long DBtag::indexentryDocBookAPar[] = { 
146, 147, 0 };

long DBtag::indextermDocBookAPar[] = { 
113, 139, 166, 183, 256, 259, 263, 364, 
65536, 65537, 65540, 65541, 65543, 65544, 65546, 65547, 
65550, 65554, 65556, 65558, 65559, 65566, 65570, 65571, 
0 };

long DBtag::informalequationDocBookAPar[] = { 
97, 102, 110, 122, 65550, 65552, 65555, 65556, 
65573, 0 };

long DBtag::informalexampleDocBookAPar[] = { 
102, 110, 122, 65550, 65552, 65555, 65556, 65573, 
0 };

long DBtag::informaltableDocBookAPar[] = { 
102, 110, 122, 347, 356, 65550, 65552, 65555, 
65556, 65573, 0 };

long DBtag::initializerDocBookAPar[] = { 
105, 193, 227, 0 };

long DBtag::inlineequationDocBookAPar[] = { 
183, 362, 364, 365, 65540, 65541, 0 };

long DBtag::inlinegraphicDocBookAPar[] = { 
65540, 65541, 65544, 65545, 65546, 65547, 65548, 65559, 
65560, 65561, 0 };

long DBtag::inlinemediaobjectDocBookAPar[] = { 
156, 65540, 65541, 65544, 65545, 65546, 65547, 65548, 
65559, 65560, 65561, 0 };

long DBtag::interfacenameDocBookAPar[] = { 
215, 255, 261, 374, 65540, 65541, 65542, 65558, 
0 };

long DBtag::itemizedlistDocBookAPar[] = { 
49, 139, 147, 353, 65537, 65538, 65550, 65564, 
0 };

long DBtag::keycapDocBookAPar[] = { 
170, 255, 261, 314, 374, 65540, 65541, 65542, 
65558, 0 };

long DBtag::labelDocBookAPar[] = { 
12, 252, 0 };

long DBtag::legalnoticeDocBookAPar[] = { 
65568, 0 };

long DBtag::lineannotationDocBookAPar[] = { 
65565, 0 };

long DBtag::linkDocBookAPar[] = { 
106, 153, 65540, 65541, 65542, 65544, 65545, 65546, 
65547, 65548, 65549, 0 };

long DBtag::listitemDocBookAPar[] = { 
165, 218, 381, 0 };

long DBtag::literallayoutDocBookAPar[] = { 
96, 65537, 65538, 65550, 65564, 65583, 0 };

long DBtag::lotDocBookAPar[] = { 
65586, 0 };

long DBtag::mediaobjectDocBookAPar[] = { 
65550, 65552, 65555, 65556, 65562, 65563, 65564, 0 };

long DBtag::methodnameDocBookAPar[] = { 
65540, 65541, 65542, 65558, 65572, 0 };

long DBtag::methodparamDocBookAPar[] = { 
76, 88, 194, 0 };

long DBtag::modespecDocBookAPar[] = { 
65540, 65541, 65542, 65568, 0 };

long DBtag::modifierDocBookAPar[] = { 
76, 88, 105, 116, 193, 194, 213, 214, 
215, 0 };

long DBtag::msgexplanDocBookAPar[] = { 
200, 319, 0 };

long DBtag::msgsetDocBookAPar[] = { 
45, 201, 238, 323, 343, 344, 345, 65556, 
65571, 65574, 0 };

long DBtag::msgtextDocBookAPar[] = { 
204, 206, 208, 319, 0 };

long DBtag::objectinfoDocBookAPar[] = { 
24, 143, 158, 188, 189, 353, 384, 0 };

long DBtag::ooclassDocBookAPar[] = { 
57, 255, 261, 374, 65540, 65541, 65542, 65558, 
0 };

long DBtag::optionDocBookAPar[] = { 
19, 131, 255, 261, 374, 65540, 65541, 65542, 
65558, 0 };

long DBtag::optionalDocBookAPar[] = { 
58, 255, 261, 374, 65540, 65541, 65542, 65548, 
0 };

long DBtag::orgnameDocBookAPar[] = { 
65536, 65540, 65541, 65542, 65553, 0 };

long DBtag::packageDocBookAPar[] = { 
213, 214, 215, 255, 261, 374, 65540, 65541, 
65542, 65558, 0 };

long DBtag::paraDocBookAPar[] = { 
113, 65537, 65538, 65550, 65580, 0 };

long DBtag::parameterDocBookAPar[] = { 
65540, 65541, 65542, 65558, 65577, 0 };

long DBtag::personblurbDocBookAPar[] = { 
25, 90, 222, 0 };

long DBtag::personnameDocBookAPar[] = { 
65536, 65540, 65541, 65542, 65557, 0 };

long DBtag::phraseDocBookAPar[] = { 
353, 65540, 65541, 65542, 0 };

long DBtag::procedureDocBookAPar[] = { 
102, 152, 342, 65550, 65552, 65556, 65571, 65585, 
0 };

long DBtag::programlistingDocBookAPar[] = { 
242, 65537, 65538, 65550, 65564, 65583, 0 };

long DBtag::programlistingcoDocBookAPar[] = { 
65537, 65538, 65550, 65564, 65583, 0 };

long DBtag::publishernameDocBookAPar[] = { 
246, 296, 298, 300, 302, 65536, 0 };

long DBtag::qandadivDocBookAPar[] = { 
249, 251, 0 };

long DBtag::qandaentryDocBookAPar[] = { 
12, 249, 251, 0 };

long DBtag::refentrytitleDocBookAPar[] = { 
53, 259, 0 };

long DBtag::refsect2DocBookAPar[] = { 
264, 272, 0 };

long DBtag::refsectionDocBookAPar[] = { 
256, 270, 0 };

long DBtag::remarkDocBookAPar[] = { 
122, 65540, 65541, 65542, 65544, 65545, 65546, 65547, 
65548, 65549, 65550, 65552, 65555, 65556, 0 };

long DBtag::replaceableDocBookAPar[] = { 
19, 114, 131, 227, 255, 261, 65540, 65541, 
65542, 65545, 65546, 65547, 65558, 65559, 65560, 0 };

long DBtag::revhistoryDocBookAPar[] = { 
250, 65536, 65540, 65541, 65542, 65543, 0 };

long DBtag::rowDocBookAPar[] = { 
346, 354, 357, 0 };

long DBtag::sbrDocBookAPar[] = { 
19, 59, 131, 0 };

long DBtag::screenDocBookAPar[] = { 
286, 65537, 65538, 65550, 65564, 65583, 0 };

long DBtag::sect1DocBookAPar[] = { 
13, 1, 0 };

long DBtag::sectionDocBookAPar[] = { 
13, 1, 301, 0 };

long DBtag::sidebarDocBookAPar[] = { 
238, 343, 344, 345, 65556, 65571, 65574, 65585, 
0 };

long DBtag::spanspecDocBookAPar[] = { 
94, 355, 0 };

long DBtag::stepDocBookAPar[] = { 
238, 324, 332, 0 };

long DBtag::subscriptDocBookAPar[] = { 
58, 186, 260, 65540, 65541, 65542, 65544, 65545, 
65546, 65547, 65548, 65569, 0 };

long DBtag::subtitleDocBookAPar[] = { 
1, 33, 120, 147, 182, 296, 298, 300, 
302, 361, 65536, 65538, 0 };

long DBtag::symbolDocBookAPar[] = { 
255, 261, 331, 334, 374, 65540, 65541, 65542, 
65558, 0 };

long DBtag::synopfragmentrefDocBookAPar[] = { 
19, 131, 0 };

long DBtag::synopsisDocBookAPar[] = { 
65550, 65552, 65555, 65556, 65564, 65575, 65576, 65582, 
0 };

long DBtag::tableDocBookAPar[] = { 
347, 356, 65550, 65552, 65556, 65571, 65584, 0 };

long DBtag::textobjectDocBookAPar[] = { 
154, 158, 188, 189, 341, 65565, 0 };

long DBtag::tfootDocBookAPar[] = { 
154, 341, 355, 0 };

long DBtag::tgroupDocBookAPar[] = { 
154, 341, 0 };

long DBtag::theadDocBookAPar[] = { 
94, 154, 341, 355, 0 };

long DBtag::titleDocBookAPar[] = { 
3, 26, 113, 198, 204, 206, 208, 229, 
65536, 65537, 65538, 65539, 0 };

long DBtag::titleabbrevDocBookAPar[] = { 
102, 65536, 65538, 65539, 65552, 0 };

long DBtag::tocchapDocBookAPar[] = { 
13, 1, 361, 371, 0 };

long DBtag::tocentryDocBookAPar[] = { 
363, 366, 367, 368, 369, 370, 371, 0 };

long DBtag::trDocBookAPar[] = { 
154, 341, 346, 354, 357, 0 };

long DBtag::trademarkDocBookAPar[] = { 
65540, 65541, 65542, 65544, 65546, 65547, 65554, 0 };

long DBtag::typeDocBookAPar[] = { 
105, 114, 194, 65540, 65541, 65542, 65558, 65577, 
0 };

long DBtag::varnameDocBookAPar[] = { 
105, 255, 261, 374, 65540, 65541, 65542, 65558, 
0 };

long DBtag::videoobjectDocBookAPar[] = { 
158, 188, 0 };

long DBtag::voidDocBookAPar[] = { 
76, 88, 116, 194, 0 };

long DBtag::beginpagefirstDocBookAPar[] = { 
13, 182, 256, 361, 0 };

long DBtag::blockinfofirstDocBookAPar[] = { 
36, 45, 97, 106, 125, 151, 152, 153, 
154, 165, 207, 218, 249, 250, 251, 342, 
380, 65567, 0 };

long DBtag::titlefirstDocBookAPar[] = { 
3, 1, 26, 31, 48, 50, 66, 113, 
123, 145, 147, 198, 201, 204, 206, 208, 
210, 229, 309, 320, 323, 358, 387, 0 };

vpair DBtag::DocBookAElemSetsSet[] = {
 { 65536, Part1DocBookAPar },
 { 65537, Part2DocBookAPar },
 { 65538, Part3DocBookAPar },
 { 65539, Part4DocBookAPar },
 { 65540, Part5DocBookAPar },
 { 65541, Part6DocBookAPar },
 { 65542, Part7DocBookAPar },
 { 65543, Part8DocBookAPar },
 { 65544, Part9DocBookAPar },
 { 65545, Part10DocBookAPar },
 { 65546, Part11DocBookAPar },
 { 65547, Part12DocBookAPar },
 { 65548, Part13DocBookAPar },
 { 65549, Part14DocBookAPar },
 { 65550, Part15DocBookAPar },
 { 65551, Part16DocBookAPar },
 { 65552, Part17DocBookAPar },
 { 65553, Part18DocBookAPar },
 { 65554, Part19DocBookAPar },
 { 65555, Part20DocBookAPar },
 { 65556, Part21DocBookAPar },
 { 65557, Part22DocBookAPar },
 { 65558, Part23DocBookAPar },
 { 65559, Part24DocBookAPar },
 { 65560, Part25DocBookAPar },
 { 65561, Part26DocBookAPar },
 { 65562, Part27DocBookAPar },
 { 65563, Part28DocBookAPar },
 { 65564, Part29DocBookAPar },
 { 65565, Part30DocBookAPar },
 { 65566, Part31DocBookAPar },
 { 65567, Part32DocBookAPar },
 { 65568, Part33DocBookAPar },
 { 65569, Part34DocBookAPar },
 { 65570, Part35DocBookAPar },
 { 65571, Part36DocBookAPar },
 { 65572, Part37DocBookAPar },
 { 65573, Part38DocBookAPar },
 { 65574, Part39DocBookAPar },
 { 65575, Part40DocBookAPar },
 { 65576, Part41DocBookAPar },
 { 65577, Part42DocBookAPar },
 { 65578, Part43DocBookAPar },
 { 65579, Part44DocBookAPar },
 { 65580, Part45DocBookAPar },
 { 65581, Part46DocBookAPar },
 { 65582, Part47DocBookAPar },
 { 65583, Part48DocBookAPar },
 { 65584, Part49DocBookAPar },
 { 65585, Part50DocBookAPar },
 { 65586, Part51DocBookAPar },
 { 65587, abbrevDocBookAPar },
 { 65588, abstractDocBookAPar },
 { 65589, accelDocBookAPar },
 { 65590, acronymDocBookAPar },
 { 65591, actionDocBookAPar },
 { 65592, addressDocBookAPar },
 { 65593, affiliationDocBookAPar },
 { 65594, altDocBookAPar },
 { 65595, anchorDocBookAPar },
 { 65596, applicationDocBookAPar },
 { 65597, areaDocBookAPar },
 { 65598, areaspecDocBookAPar },
 { 65599, argDocBookAPar },
 { 65600, articleinfoDocBookAPar },
 { 65601, artpagenumsDocBookAPar },
 { 65602, attributionDocBookAPar },
 { 65603, authorDocBookAPar },
 { 65604, authorblurbDocBookAPar },
 { 65605, authorinitialsDocBookAPar },
 { 65606, beginpageDocBookAPar },
 { 65607, biblioentryDocBookAPar },
 { 65608, bibliographyDocBookAPar },
 { 65609, bibliomsetDocBookAPar },
 { 65610, blockinfoDocBookAPar },
 { 65611, blockquoteDocBookAPar },
 { 65612, bridgeheadDocBookAPar },
 { 65613, calloutlistDocBookAPar },
 { 65614, captionDocBookAPar },
 { 65615, cautionDocBookAPar },
 { 65616, citetitleDocBookAPar },
 { 65617, classnameDocBookAPar },
 { 65618, coDocBookAPar },
 { 65619, colDocBookAPar },
 { 65620, colspecDocBookAPar },
 { 65621, commandDocBookAPar },
 { 65622, contribDocBookAPar },
 { 65623, corefDocBookAPar },
 { 65624, corpauthorDocBookAPar },
 { 65625, dateDocBookAPar },
 { 65626, editorDocBookAPar },
 { 65627, emailDocBookAPar },
 { 65628, emphasisDocBookAPar },
 { 65629, epigraphDocBookAPar },
 { 65630, equationDocBookAPar },
 { 65631, exampleDocBookAPar },
 { 65632, exceptionnameDocBookAPar },
 { 65633, fieldsynopsisDocBookAPar },
 { 65634, footnoterefDocBookAPar },
 { 65635, formalparaDocBookAPar },
 { 65636, funcparamsDocBookAPar },
 { 65637, functionDocBookAPar },
 { 65638, glossentryDocBookAPar },
 { 65639, glosslistDocBookAPar },
 { 65640, glosstermDocBookAPar },
 { 65641, graphicDocBookAPar },
 { 65642, graphiccoDocBookAPar },
 { 65643, guibuttonDocBookAPar },
 { 65644, highlightsDocBookAPar },
 { 65645, honorificDocBookAPar },
 { 65646, imageobjectDocBookAPar },
 { 65647, imageobjectcoDocBookAPar },
 { 65648, indexentryDocBookAPar },
 { 65649, indextermDocBookAPar },
 { 65650, informalequationDocBookAPar },
 { 65651, informalexampleDocBookAPar },
 { 65652, informaltableDocBookAPar },
 { 65653, initializerDocBookAPar },
 { 65654, inlineequationDocBookAPar },
 { 65655, inlinegraphicDocBookAPar },
 { 65656, inlinemediaobjectDocBookAPar },
 { 65657, interfacenameDocBookAPar },
 { 65658, itemizedlistDocBookAPar },
 { 65659, keycapDocBookAPar },
 { 65660, labelDocBookAPar },
 { 65661, legalnoticeDocBookAPar },
 { 65662, lineannotationDocBookAPar },
 { 65663, linkDocBookAPar },
 { 65664, listitemDocBookAPar },
 { 65665, literallayoutDocBookAPar },
 { 65666, lotDocBookAPar },
 { 65667, mediaobjectDocBookAPar },
 { 65668, methodnameDocBookAPar },
 { 65669, methodparamDocBookAPar },
 { 65670, modespecDocBookAPar },
 { 65671, modifierDocBookAPar },
 { 65672, msgexplanDocBookAPar },
 { 65673, msgsetDocBookAPar },
 { 65674, msgtextDocBookAPar },
 { 65675, objectinfoDocBookAPar },
 { 65676, ooclassDocBookAPar },
 { 65677, optionDocBookAPar },
 { 65678, optionalDocBookAPar },
 { 65679, orgnameDocBookAPar },
 { 65680, packageDocBookAPar },
 { 65681, paraDocBookAPar },
 { 65682, parameterDocBookAPar },
 { 65683, personblurbDocBookAPar },
 { 65684, personnameDocBookAPar },
 { 65685, phraseDocBookAPar },
 { 65686, procedureDocBookAPar },
 { 65687, programlistingDocBookAPar },
 { 65688, programlistingcoDocBookAPar },
 { 65689, publishernameDocBookAPar },
 { 65690, qandadivDocBookAPar },
 { 65691, qandaentryDocBookAPar },
 { 65692, refentrytitleDocBookAPar },
 { 65693, refsect2DocBookAPar },
 { 65694, refsectionDocBookAPar },
 { 65695, remarkDocBookAPar },
 { 65696, replaceableDocBookAPar },
 { 65697, revhistoryDocBookAPar },
 { 65698, rowDocBookAPar },
 { 65699, sbrDocBookAPar },
 { 65700, screenDocBookAPar },
 { 65701, sect1DocBookAPar },
 { 65702, sectionDocBookAPar },
 { 65703, sidebarDocBookAPar },
 { 65704, spanspecDocBookAPar },
 { 65705, stepDocBookAPar },
 { 65706, subscriptDocBookAPar },
 { 65707, subtitleDocBookAPar },
 { 65708, symbolDocBookAPar },
 { 65709, synopfragmentrefDocBookAPar },
 { 65710, synopsisDocBookAPar },
 { 65711, tableDocBookAPar },
 { 65712, textobjectDocBookAPar },
 { 65713, tfootDocBookAPar },
 { 65714, tgroupDocBookAPar },
 { 65715, theadDocBookAPar },
 { 65716, titleDocBookAPar },
 { 65717, titleabbrevDocBookAPar },
 { 65718, tocchapDocBookAPar },
 { 65719, tocentryDocBookAPar },
 { 65720, trDocBookAPar },
 { 65721, trademarkDocBookAPar },
 { 65722, typeDocBookAPar },
 { 65723, varnameDocBookAPar },
 { 65724, videoobjectDocBookAPar },
 { 65725, voidDocBookAPar },
 { 65726, beginpagefirstDocBookAPar },
 { 65727, blockinfofirstDocBookAPar },
 { 65728, titlefirstDocBookAPar },
 { 0, NULL }
 };

npair DBtag::DocBookAElemSetNamesSet[] = {
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
 { 65556, "*Part21"},
 { 65557, "*Part22"},
 { 65558, "*Part23"},
 { 65559, "*Part24"},
 { 65560, "*Part25"},
 { 65561, "*Part26"},
 { 65562, "*Part27"},
 { 65563, "*Part28"},
 { 65564, "*Part29"},
 { 65565, "*Part30"},
 { 65566, "*Part31"},
 { 65567, "*Part32"},
 { 65568, "*Part33"},
 { 65569, "*Part34"},
 { 65570, "*Part35"},
 { 65571, "*Part36"},
 { 65572, "*Part37"},
 { 65573, "*Part38"},
 { 65574, "*Part39"},
 { 65575, "*Part40"},
 { 65576, "*Part41"},
 { 65577, "*Part42"},
 { 65578, "*Part43"},
 { 65579, "*Part44"},
 { 65580, "*Part45"},
 { 65581, "*Part46"},
 { 65582, "*Part47"},
 { 65583, "*Part48"},
 { 65584, "*Part49"},
 { 65585, "*Part50"},
 { 65586, "*Part51"},
 { 65587, "*abbrev"},
 { 65588, "*abstract"},
 { 65589, "*accel"},
 { 65590, "*acronym"},
 { 65591, "*action"},
 { 65592, "*address"},
 { 65593, "*affiliation"},
 { 65594, "*alt"},
 { 65595, "*anchor"},
 { 65596, "*application"},
 { 65597, "*area"},
 { 65598, "*areaspec"},
 { 65599, "*arg"},
 { 65600, "*articleinfo"},
 { 65601, "*artpagenums"},
 { 65602, "*attribution"},
 { 65603, "*author"},
 { 65604, "*authorblurb"},
 { 65605, "*authorinitials"},
 { 65606, "*beginpage"},
 { 65607, "*biblioentry"},
 { 65608, "*bibliography"},
 { 65609, "*bibliomset"},
 { 65610, "*blockinfo"},
 { 65611, "*blockquote"},
 { 65612, "*bridgehead"},
 { 65613, "*calloutlist"},
 { 65614, "*caption"},
 { 65615, "*caution"},
 { 65616, "*citetitle"},
 { 65617, "*classname"},
 { 65618, "*co"},
 { 65619, "*col"},
 { 65620, "*colspec"},
 { 65621, "*command"},
 { 65622, "*contrib"},
 { 65623, "*coref"},
 { 65624, "*corpauthor"},
 { 65625, "*date"},
 { 65626, "*editor"},
 { 65627, "*email"},
 { 65628, "*emphasis"},
 { 65629, "*epigraph"},
 { 65630, "*equation"},
 { 65631, "*example"},
 { 65632, "*exceptionname"},
 { 65633, "*fieldsynopsis"},
 { 65634, "*footnoteref"},
 { 65635, "*formalpara"},
 { 65636, "*funcparams"},
 { 65637, "*function"},
 { 65638, "*glossentry"},
 { 65639, "*glosslist"},
 { 65640, "*glossterm"},
 { 65641, "*graphic"},
 { 65642, "*graphicco"},
 { 65643, "*guibutton"},
 { 65644, "*highlights"},
 { 65645, "*honorific"},
 { 65646, "*imageobject"},
 { 65647, "*imageobjectco"},
 { 65648, "*indexentry"},
 { 65649, "*indexterm"},
 { 65650, "*informalequation"},
 { 65651, "*informalexample"},
 { 65652, "*informaltable"},
 { 65653, "*initializer"},
 { 65654, "*inlineequation"},
 { 65655, "*inlinegraphic"},
 { 65656, "*inlinemediaobject"},
 { 65657, "*interfacename"},
 { 65658, "*itemizedlist"},
 { 65659, "*keycap"},
 { 65660, "*label"},
 { 65661, "*legalnotice"},
 { 65662, "*lineannotation"},
 { 65663, "*link"},
 { 65664, "*listitem"},
 { 65665, "*literallayout"},
 { 65666, "*lot"},
 { 65667, "*mediaobject"},
 { 65668, "*methodname"},
 { 65669, "*methodparam"},
 { 65670, "*modespec"},
 { 65671, "*modifier"},
 { 65672, "*msgexplan"},
 { 65673, "*msgset"},
 { 65674, "*msgtext"},
 { 65675, "*objectinfo"},
 { 65676, "*ooclass"},
 { 65677, "*option"},
 { 65678, "*optional"},
 { 65679, "*orgname"},
 { 65680, "*package"},
 { 65681, "*para"},
 { 65682, "*parameter"},
 { 65683, "*personblurb"},
 { 65684, "*personname"},
 { 65685, "*phrase"},
 { 65686, "*procedure"},
 { 65687, "*programlisting"},
 { 65688, "*programlistingco"},
 { 65689, "*publishername"},
 { 65690, "*qandadiv"},
 { 65691, "*qandaentry"},
 { 65692, "*refentrytitle"},
 { 65693, "*refsect2"},
 { 65694, "*refsection"},
 { 65695, "*remark"},
 { 65696, "*replaceable"},
 { 65697, "*revhistory"},
 { 65698, "*row"},
 { 65699, "*sbr"},
 { 65700, "*screen"},
 { 65701, "*sect1"},
 { 65702, "*section"},
 { 65703, "*sidebar"},
 { 65704, "*spanspec"},
 { 65705, "*step"},
 { 65706, "*subscript"},
 { 65707, "*subtitle"},
 { 65708, "*symbol"},
 { 65709, "*synopfragmentref"},
 { 65710, "*synopsis"},
 { 65711, "*table"},
 { 65712, "*textobject"},
 { 65713, "*tfoot"},
 { 65714, "*tgroup"},
 { 65715, "*thead"},
 { 65716, "*title"},
 { 65717, "*titleabbrev"},
 { 65718, "*tocchap"},
 { 65719, "*tocentry"},
 { 65720, "*tr"},
 { 65721, "*trademark"},
 { 65722, "*type"},
 { 65723, "*varname"},
 { 65724, "*videoobject"},
 { 65725, "*void"},
 { 65726, "*beginpagefirst"},
 { 65727, "*blockinfofirst"},
 { 65728, "*titlefirst"},
 { 0, NULL }
 };

ipair DBtag::DocBookALevelsSet[] = {
 { 5, 1 },
 { 13, 1 },
 { 66, 1 },
 { 0, 0 }
 };

ipair DBtag::DocBookAAtFirstSet[] = {
 { 10, 156 },
 { 18, -1 },
 { 22, 96 },
 { 29, 65726 },
 { 34, -1 },
 { 44, 65727 },
 { 65, -1 },
 { 121, -1 },
 { 128, 124 },
 { 149, -1 },
 { 174, -1 },
 { 209, 319 },
 { 211, -1 },
 { 235, -1 },
 { 236, -1 },
 { 247, 246 },
 { 255, -1 },
 { 258, 53 },
 { 265, -1 },
 { 267, -1 },
 { 269, -1 },
 { 271, -1 },
 { 273, -1 },
 { 281, -1 },
 { 287, -1 },
 { 292, -1 },
 { 294, -1 },
 { 296, -1 },
 { 298, -1 },
 { 300, -1 },
 { 302, -1 },
 { 313, -1 },
 { 314, -1 },
 { 316, -1 },
 { 359, 65728 },
 { 0, 0 }
 };


// end of dwhtmdb.cpp

