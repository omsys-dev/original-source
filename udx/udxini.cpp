// udxini.cpp is the ini-file reader for udx.exe
// Copyright 2013 by Jeremy H. Griffith, under Apache license

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "mxlparser.h"
#include "udx.h"


char *UDini::DefaultIniFileName = "udx.ini";

char *UDini::SectNames[] = {
	"udxFullTaggingOptions",
	"ShortTagContainers",
	"ListShortTagReplacement",
	"PairListShortTagReplacement",
	"TableShortTagReplacement",
	"ListShortTagEnds",
	"PairListShortTagEnds",
	"TableShortTagEnds",
	"WikiSymbolReplacement",
	"udxTagMinimizingOptions",
	"ListLongTagReplacement",
	"PairListLongTagReplacement",
	"TableLongTagReplacement",
	"WikiTagReplacement",
	"CodeElements",
	"ElementTypes",
	NULL };

vpair UDini::InitNames[] = {
	{ &udxFullTaggingOptions, "udxFullTaggingOptions" },
	{ &ShortTagContainers, "ShortTagContainers" },
	{ &ListShortTagReplacement, "ListShortTagReplacement" },
	{ &PairListShortTagReplacement, "PairListShortTagReplacement" },
	{ &TableShortTagReplacement, "TableShortTagReplacement" },
	{ &ListShortTagEnds, "ListShortTagEnds" },
	{ &PairListShortTagEnds, "PairListShortTagEnds" },
	{ &TableShortTagEnds, "TableShortTagEnds" },
	{ &WikiSymbolReplacement, "WikiSymbolReplacement" },
	{ &udxTagMinimizingOptions, "udxTagMinimizingOptions" },
	{ &ListLongTagReplacement, "ListLongTagReplacement" },
	{ &PairListLongTagReplacement, "PairListLongTagReplacement" },
	{ &TableLongTagReplacement, "TableLongTagReplacement" },
	{ &WikiTagReplacement, "WikiTagReplacement" },
	{ &CodeElements, "CodeElements" },
	{ &ElementTypes, "ElementTypes" },
	NULL };

vpair UDini::udxFullTaggingOptions[] = {
	{ &DoPara, "AddPTagsInBlocks" },
	{ &DoBlank, "BlankLinesInTextConvert" },
	{ &DoListShort, "ListsConvert" },
	{ &DoPairShort, "PairListsConvert" },
	{ &DoTableShort, "TablesConvert" },
	{ &DoWiki, "WikiSymbolsConvert" },
	NULL };


npair UDini::ShortTagContainers[] = {
	{ "pairlist", "pl" },
	{ "list", "ol" },
	{ "list", "ul" },
	{ "list", "sl" },
	{ "table", "table" },
	NULL };

npair UDini::ListShortTagReplacement[] = {
	{ &("li"), "l" },
	NULL };

npair UDini::PairListShortTagReplacement[] = {
	{ &("li"), "l" },
	{ &("title"), "t" },
	{ &("p"), "d" },
	NULL };

npair UDini::TableShortTagReplacement[] = {
	{ &("title"), "t" },
	{ &("row"), "r" },
	{ &("cell"), "c" },
	NULL };

npair UDini::ListShortTagEnds[] = {
	{ &("l li"), "l" },
	NULL };

npair UDini::PairListShortTagEnds[] = {
	{ &("l li"), "l" },
	{ &("t title d p l li"), "t" },
	{ &("d p l li"), "d" },
	NULL };

npair UDini::TableShortTagEnds[] = {
	{ &("r row col"), "t" },
	{ &("r row col"), "r" },
	{ &("c cell r row col"), "c" },
	NULL };

npair UDini::WikiSymbolReplacement[] = {
	{ &("b"), "*" },
	{ &("i"), "_" },
	{ &("q"), "\"" },
	{ &("tt"), "`" },
	{ &("tt &lt; &gt;"), "^" },
	NULL };

vpair UDini::udxTagMinimizingOptions[] = {
	{ &DoParaM, "RemovePTagsFromBlocks" },
	{ &DoBlankM, "TextEndStartTagsToBlanklinesConvert" },
	{ &DoListShortM, "ListsToShortTagsConvert" },
	{ &DoPairShortM, "PairListsToShortTagsConvert" },
	{ &DoTableShortM, "TablesToShortTagsConvert" },
	{ &DoWikiM, "InlineTagsToWikiSymbolsConvert" },
	NULL };

npair UDini::ListLongTagReplacement[] = {
	{ &("l"), "li" },
	NULL };

npair UDini::PairListLongTagReplacement[] = {
	{ &("l"), "li" },
	{ &("t"), "title" },
	{ &("d"), "p" },
	NULL };

npair UDini::TableLongTagReplacement[] = {
	{ &("t"), "title" },
	{ &("r"), "row" },
	{ &("c"), "cell" },
	NULL };

npair UDini::WikiTagReplacement[] = {
	{ &("*"), "b" },
	{ &("_"), "i" },
	{ &("\""), "q" },
	{ &("`"), "tt" },
	{ &("^"), "<" },
	{ &("^"), ">" },
	NULL };

npair UDini::CodeElements[] = {
	{ &("text"), "pre" },
	{ &("block"), "code" },
	{ &("inline"), "tt" },
	NULL };

npair UDini::ElementTypes[] = {
	{ "text", "p" },
	{ "text", "pre" },
	{ "text", "title" },
	{ "text", "subtitle" },
	{ "text", "t" },
	{ "text", "d" },
	{ "text", "usage" },
	{ "text", "quote" },
	{ "text", "cite" },
	{ "text", "desc" },
	{ "text", "alt" },
	{ "text", "area" },
	{ "inline", "ph" },
	{ "inline", "term" },
	{ "inline", "abbr" },
	{ "inline", "tm" },
	{ "inline", "xref" },
	{ "inline", "seq" },
	{ "inline", "mark" },
	{ "inline", "idx" },
	{ "inline", "var" },
	{ "inline", "img" },
	{ "inline", "fn" },
	{ "inline", "br" },
	{ "inline", "b" },
	{ "inline", "i" },
	{ "inline", "u" },
	{ "inline", "du" },
	{ "inline", "o" },
	{ "inline", "s" },
	{ "inline", "sup" },
	{ "inline", "sub" },
	{ "inline", "tt" },
	{ "inline", "q" },
	{ "inline", "t1" },
	{ "inline", "t2" },
	{ "inline", "t3" },
	{ "inline", "t4" },
	{ "inline", "t5" },
	{ "inline", "t6" },
	{ "inline", "t7" },
	{ "inline", "t8" },
	{ "inline", "t9" },
	{ "inline", "t10" },
	{ "inline", "t11" },
	{ "inline", "t12" },
	{ "inline", "t13" },
	{ "inline", "t14" },
	{ "inline", "t15" },
	{ "inline", "t16" },
	{ "inline", "t17" },
	{ "inline", "t18" },
	{ "inline", "t19" },
	{ "inline", "t20" },
	NULL };



UDini::UDini()
{
	Setup();
	FILE *fini = fopen(DefaultIniFileName, "rt");
	if (fini)
		Process(fini);
}

UDini::UDini(char *fname)
{
	Setup();
	FILE *fini = fopen(fname, "rt");
	if (fini)
		Process(fini);
}



void UDini::Setup(void)
{
	sections = new UDvlist();

	UDvlist *sectinis = new UDvlist();
	sectinis->init(InitNames);

	UDvlist *sect = sectinis;
	UDnlist *sid = NULL;

	while ((sect = sect->next) != NULL) {
		sid = new UDnlist();
		sid->init((npair *) sect->id);
		sections->add(sect->name, sid);
	}

	// each sections item is a section name,
	// with a UDnlist* to an initialized list of settings.
	// the teo Options lists have bool*,
	// all the rest have char*
}



void UDini::Process(FILE *fini)
{
	char buf[LineMax];
	char *ep = NULL;
	UDnlist *slp = NULL;
	int ch = 0;
	char *equ = NULL;
	char *eqe = NULL;
	bool isbool = false;
	void *val = NULL;

	// if ini file exists:
	// when Options read, set the var at the bool*
	// for others, compare str at char* to ini,
	//  replace it with a new str if different

	while (fgets(buf, LineMax, fini) != NULL) {
		if (!*buf
		 || (*buf == ';')
		 || (*buf == '\n'))
			continue;
		if ((*buf == '[')
		 && ((ep = strchr(buf, ']')) != NULL)) {
			// found start of section
			*ep = '\0';
			if ((slp = (UDnlist *) sections->getitem(buf + 1)) != NULL) {
				// recognized section, get settings
				isbool = strstr(buf, "Options") ? true : false;
				while (fgets(buf, LineMax, fini) != NULL) {
					if (*buf
					 && (*buf != ';')
					 && (*buf != '\n')
					 && ((equ = strchr(buf, '=')) != NULL)) {
						// see if recognized setting
						eqe = equ;
						while ((eqe > buf)
						 && (((*(eqe - 1) == ' '))
						  || ((*(eqe - 1) == '\t'))))
							eqe--;
						*eqe = '\0';
						eqe = equ + 1;
						while ((*eqe == ' ')
						 || (*eqe == '\t'))
							eqe++;
						equ = eqe;
						eqe = equ + strlen(equ);
						while ((eqe > equ)
						 && ((*(eqe - 1) == ' ')
						  || (*(eqe - 1) == '\n')
						  || (*(eqe - 1) == '\t')))
							eqe--;
						*eqe = '\0';
						// name is buf, val is equ
						val = slp->getitem(buf);
						if (isbool) {  // Options setting
							if (val) {
								if (!stricmp(equ, "yes"))
									*((bool *) val) = true;
								else
									*((bool *) val) = false;
							}
						}
						else {  // string value
							if (val) {  // key found, check value
								if (equ == eqe)  // empty, remove
									slp->remove(buf);
								else if (stricmp((char *) val, equ))
									slp->replaceval(buf, NewName(equ));
								// if same, no need to replace
							}
							else   // no key, add item
								slp->add(NewName(buf), NewName(equ));
						}
					}
					ch = fgetc(fini);
					ungetc(ch, fini);
					if (ch == '[')
						break;
				}
			}
		}
	}
}


char *UDini::GetVal(char *sect, char *set)
{
	UDnlist *slp = NULL;
	char *val = NULL;

	if (((slp = (UDnlist *) sections->getitem(sect)) != NULL)
	 && ((val = (char *) slp->getitem(set)) != NULL))
		return val;

	return NULL;
}


bool UDini::GetBool(char *sect, char *set, char *stype)
{
	UDnlist *slp = NULL;
	char *fset = NULL;
	bool *val = false;

	if (stype) {
		fset = NewName(stype, strlen(set) + strlen(stype));
		strcat(fset, set);
	}
	else
		fset = set;

	if (((slp = (UDnlist *) sections->getitem(sect)) != NULL)
	 && ((val = (bool *) slp->getitem(fset)) != NULL))
		return *val;

	return false;
}


UDnlist *UDini::GetSect(char *sect, char *stype)
{
	UDnlist *slp = NULL;
	char *fsect = NULL;

	if (stype) {
		fsect = NewName(stype, strlen(sect) + strlen(stype));
		strcat(fsect, sect);
	}
	else
		fsect = sect;

	if ((slp = (UDnlist *) sections->getitem(fsect)) != NULL)
		return slp;

	return NULL;
}





