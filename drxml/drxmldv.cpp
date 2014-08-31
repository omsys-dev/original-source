// drxmldv.cpp is the code for dcl ditaval conditional processing, JHG
// Copyright (c) 2009 by Omni Systems, Inc.  All rights reserved.

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
#include "drxdvfl.h"



// each topicref has a pointer to a set of ditaval properties
// that are in scope for it.  Each new ditaval file creates a
// new set, without inheritance from any previous file in effect.

// Consider allowing overrides to individual props in PIs.

// ditaval files can be used two ways:
// 1. Outside of the map and referenced in the .ini, global scope.
//    This is also the scope of the .ini condition settings.
// 2. In any map (but not topic) as either a topicref with @format=
//    ditaval, or in a PI with @ditaval=filename.
// In the second case, the scope begins where the topicref or PI
// appears, and persists until the end of the map or the end of
// the level (for nested topicrefs), and includes all children of
// the topicrefs in that scope, and all conrefs to topics in scope.


// list of sets, used to avoid repeated processing of ditavals
// when the same ditaval file is used in more than one place
#ifdef DITA
DCvlist XMLrd::ConditionSets;
DCnlist XMLrd::ConditionSetFiles;
DClist XMLrd::CondSetPendList;
long XMLrd::ConditionSetCount = 0;


// current set
// in maps, changed by a new ditaval and stored in each topicref
// in docs, changed by each topicref to its stored value 
DCvlist XMLrd::CondSetLevels;  // stack for restores at end of scope
long XMLrd::CurrCondSetLevel = 0;
long XMLrd::CurrCondRefLevel = 1;
#endif

conditions *XMLrd::CurrCondSet = NULL;
char *XMLrd::CurrBranch = NULL;
DCvlist XMLrd::BranchLevels;  // stack for restores at end of scope
long XMLrd::CurrBranchLevel = 0;

#ifdef DITA

void XMLrd::SetNewConditionSet(const char *dvname, bool pi)
{
	long cond = 0;
	if (dvname && !*dvname)
		dvname = NULL;

	CurrCondSetLevel = CurrCondRefLevel;

	if (dvname) {
		DitavalFile = FixReference(dvname);
		if ((cond = ConditionSetFiles(DitavalFile)) != 0) {
			CurrCondSet = (conditions *) ConditionSets.find(cond);
			CondSetLevels.replace(CurrCondSet, CurrCondSetLevel);
			return;
		}
		ConditionSetFiles.add(DitavalFile, ++ConditionSetCount);
	}
	else  // no name, globals from .ini file
		ConditionSetCount++; 

	CurrCondSet = new conditions;
	ConditionSets.add(CurrCondSet, ConditionSetCount);
	CondSetLevels.replace(CurrCondSet, CurrCondSetLevel);

	if (dvname) {
#if 0
		if (pi) {
			CondSetPendList.add(ConditionSetCount);
			return;
		}
#endif
		if (ProcessDitavalFile(DitavalFile))
			return;
	}
	if (!CondAttrsProcessed)
		SetCondAttrs();
}


void XMLrd::SetNewBranch(char *bname)
{
	CurrBranch = bname;
	CurrBranchLevel = CurrCondRefLevel;
	BranchLevels.replace(CurrBranch, CurrBranchLevel);
}

// call when topicref, topicgroup, topichead, or map ends

void XMLrd::RestoreConditionSet(void)
{
	long lev = CurrCondSetLevel;
	long blev = CurrBranchLevel;
	CurrCondRefLevel--;

	if (CurrCondRefLevel < lev) {  // not in scope
		for (lev = CurrCondRefLevel; lev > 0; lev--) {
			if ((CurrCondSet = (conditions *) CondSetLevels.find(lev)) != NULL)
				break;
		}
	}
	if (CurrCondRefLevel < blev) {  // not in scope
		for (blev = CurrCondRefLevel; blev > 0; blev--) {
			if ((CurrBranch = (char *) BranchLevels.find(lev)) != NULL)
				break;
		}
	}
	CurrCondSetLevel = lev;
	CurrBranchLevel = blev;
}



// ditaval file parsing


char *XMLrd::DitavalFile = NULL;
bool XMLrd::UseMatchingDitaval = false;
bool XMLrd::ComplexOtherprops = false;
bool XMLrd::CondListsInitialized = false;
XDVrfile *XMLrd::DVFile = NULL;


bool XMLrd::ProcessDitavalFile(char *dvname)
{
	if (!dvname
	 || !*dvname)
		return false;

	if (!CondListsInitialized) {
		DVFile = new XDVrfile(dvname);
		if (DVFile->FErr() != fok)
			return false;

		DVElemNames.init(DVelems);
		DVActions.init(DVacts);
		DVStyles.init(DVstyls);
		DVColors.init(DVcolrs);

		if (UseLog)
			XDVrfile::LogEventFunc = LogEvent;

		XDVrfile::LocalDTDPath = DTDPath;
		SetDVCatalogs();
		DVFile->SetHandlers(StartDVElement, EndDVElement, ProcDVChar, ProcDVPI);

		CondListsInitialized = true;
	}
	else {
		DVFile->ReopenFile(dvname);
		if (DVFile->FErr() != fok)
			return false;
	}

	CurrCondSet->DVname = NewName(DVFile->getBaseName());

	DVFile->ParseDoc();  // parses entire doc

	return true;
}


void
XMLrd::SetDVCatalogs(void)
{
	// set XDVrfile::Catalogs and CatalogPathUsed
	if (!IniFile->Section("Catalogs")
	 || (IniFile->Find("UseCatalogs")
	  && (IniFile->BoolRight() == false))) {
		XDVrfile::CatalogPathUsed = false;
		return;
	}

	char *cat = NULL;
	long cnum = 0;

	if (IniFile->Find("CatalogKeys")) {
		char **cats = IniFile->StrArrRight();
		while (*cats) {
			if (IniFile->Find(*cats)
			 && ((cat = IniFile->StrRight()) != NULL)) {
				XDVrfile::Catalogs.add(cat, ++cnum);
			}
			else
				LogEvent(logwarn,1,"Catalog key not found: ", *cats);
			cats++;
		}
	}
	else if (IniFile->Find("BaseCatalog")
	 && ((cat = IniFile->StrRight()) != NULL)) {
		XDVrfile::Catalogs.add(cat, cnum = 1);
	}

	if (!cnum) {
		LogEvent(logwarn,1,"No XML Catalogs found");
		XDVrfile::CatalogPathUsed = false;
	}
}



DCnlist XMLrd::DVElemNames;
npair XMLrd::DVelems[8] = {
 { 1, "val" },
 { 2, "style-conflict" },
 { 3, "prop" },
 { 4, "revprop" },
 { 5, "startflag" },
 { 6, "endflag" },
 { 7, "alt-text" },
 NULL
};

DCnlist XMLrd::DVActions;
npair XMLrd::DVacts[5] = {
 { 1, "include" },
 { 2, "exclude" },
 { 3, "passthrough" },
 { 4, "flag" },
 NULL
};


DCnlist XMLrd::DVStyles;
npair XMLrd::DVstyls[6] = {
 { FSunder, "underline" },
 { FSdouble, "double-underline" },
 { FSover, "overline" },
 { FSital, "italics" },
 { FSbold, "bold" },
 NULL
};

DCnlist XMLrd::DVColors;
npair XMLrd::DVcolrs[17] = {
 { 0x000001, "black" },  // really 0

 { 0xffffff, "white" },
 { 0xff0000, "red" },
 { 0x00ff00, "lime" },
 { 0x0000ff, "blue" },
 { 0xff00ff, "fuchsia" },
 { 0x00ffff, "aqua" },
 { 0xffff00, "yellow" },

 { 0x808080, "gray" },
 { 0x800000, "maroon" },
 { 0x008000, "green" },
 { 0x000080, "navy" },
 { 0x800080, "purple" },
 { 0x008080, "teal" },
 { 0x808000, "olive" },

 { 0xcccccc, "silver" },
 NULL
};



dvprop *XMLrd::CurrDVProp = NULL;
dvflag *XMLrd::CurrDVFlag = NULL;
char *XMLrd::CurrDVText = NULL;
long XMLrd::CurrDVTextLen = 0;

#if 0
DCvlist XMLrd::DVProps;
long XMLrd::DVPropCount = 0;

char *XMLrd::DVDefPropAction = NULL;
long XMLrd::CondAttDefCount = 0;
DCnlist XMLrd::CondAttDefNames;
DCnlist XMLrd::CondAttDefaults;
DCvlist XMLrd::CondAttDefFlags;
#endif


void
XMLrd::StartDVElement(const char *name, const char **atts, long depth)
{
	// root is val, contains style-conflict, then prop or revprop
	// style-conflict has @foreground-conflict-color and @background--
	// prop contains startflag and endflag, has @att, @val, @action,
	//  @color, @backcolor, @style
	// revprop contains startflag and endflag, has @val, @action,
	//  @changebar, @color, @backcolor, @style
	// startflag and endflag contain alt-text, have @imageref
	// alt-text contains text (the only one that does)

	const char *att = NULL;
	const char *val = NULL;
	const char *action = NULL;
	const char *color = NULL;
	const char *backcolor = NULL;
	const char *style = NULL;
	const char *changebar = NULL;
	const char *imageref = NULL;
	const char *fgccolor = NULL;
	const char *bgccolor = NULL;
	flagprop *fp = NULL;

	attrp alist[] = {
	 {"att", &att },
	 {"val", &val },
	 {"action", &action },
	 {"color", &color },
	 {"backcolor", &backcolor },
	 {"style", &style },
	 {"changebar", &changebar },
	 {"imageref", &imageref },
	 {"foreground-conflict-color", &fgccolor },
	 {"background-conflict-color", &bgccolor },
	 {NULL, NULL }
	};

	ProcAttrs(atts, alist);

	switch (DVElemNames(name)) {
		case 1:  // val, root
			CurrDVProp = NULL;
			CurrDVFlag = NULL;
			break;

		case 2:  // style-conflict
			if (CondFlagNames("conflict") == 0) {
				CondFlagNames.add("conflict", ++CondFlagCount);
				fp = new flagprop;
				fp->fcolor = GetDVColor((char *) fgccolor, "style-conflict");
				fp->bcolor = GetDVColor((char *) bgccolor, "style-conflict");
				CondFlagProps.add(fp, CondFlagCount);
			}
			break;

		case 3:  // prop
			CurrDVProp = new dvprop;
			CurrDVProp->att = NewName(att);
			CurrDVProp->val = NewName(val);
			CurrDVProp->action = NewName(action);
			CurrDVProp->fgcolor = NewName(color);
			CurrDVProp->bgcolor = NewName(backcolor);
			CurrDVProp->style = NewName(style);
			CurrDVFlag = NULL;
			break;

		case 4:  // revprop
			CurrDVProp = new dvprop;
			CurrDVProp->att = "rev";
			CurrDVProp->val = NewName(val);
			CurrDVProp->action = NewName(action);
			CurrDVProp->chbar = NewName(changebar);
			CurrDVProp->fgcolor = NewName(color);
			CurrDVProp->bgcolor = NewName(backcolor);
			CurrDVProp->style = NewName(style);
			CurrDVFlag = NULL;
			break;

		case 5:  // startflag
			CurrDVProp->startflag = CurrDVFlag = new dvflag;
			CurrDVProp->startflag->image = FixReference(imageref);
			break;

		case 6:  // endflag
			CurrDVProp->endflag = CurrDVFlag = new dvflag;
			CurrDVProp->endflag->image = FixReference(imageref);
			break;

		case 7:  // alt-text
			break;

		default:
			LogEvent(logwarn, 1, "Unknown ditaval element \"",
			               name, "\" in file ", DitavalFile);
			break;
	}

}


void
XMLrd::EndDVElement(const char *name, long depth)
{
	switch (DVElemNames(name)) {
		case 1:  // val, root
		case 2:  // style-conflict
			break;

		case 3:  // prop
		case 4:  // revprop
			CurrCondSet->DVProps.add(CurrDVProp, ++(CurrCondSet->DVPropCount));
			ProcessDVPropElement();
			CurrDVProp = NULL;
			break;

		case 5:  // startflag
		case 6:  // endflag
			if (CurrDVFlag && CurrDVText) {
				CurrDVFlag->text = CurrDVText;
				CurrDVText = NULL;
				CurrDVTextLen = 0;
			}
			CurrDVFlag = NULL;
			break;

		case 7:  // alt-text
		default:
			break;
	}
}


void
XMLrd::ProcDVChar(const char *str, int len, long depth)
{
	// must be alt-text content
	char *txt = NULL;

	strln *sl = ProcStr(str, len, depth);
	str = sl->str;
	len = sl->len;
	delete sl;
	if (!len)
		return;

	if (CurrDVText) {
		txt = NewName(CurrDVText, CurrDVTextLen + len);
		memcpy(txt + CurrDVTextLen, str, len);
		DeleteName(CurrDVText);
		CurrDVText = txt;
		CurrDVTextLen += len;
	}
	else
		CurrDVText = NewName(str, CurrDVTextLen = len);
}


void
XMLrd::ProcDVPI(const char *target, const char *data)
{
}


void
XMLrd::ProcessDVPropElement(void)
{
	long attnum = 0;
	DCnlist *attlist = NULL;
	long flagnum = 0;
	char *flagname = NULL;
	flagprop *fprop = NULL;
	flagprop *dfprop = NULL;

	// handle special cases
	if (!CurrDVProp->att) { // sets default action for all props
		if (!CurrCondSet->DVDefPropAction) {
			CurrCondSet->DVDefPropAction = CurrDVProp->action;
			if (!strcmp(CurrDVProp->action, "exclude"))
				CurrCondSet->DVDefPropExclude = true;
		}
		else
			LogEvent(logwarn, 1, "Duplicate prop default action in ", DitavalFile);
		return;
	}
	if (!CurrDVProp->val) { // sets default action for this attr
		if ((attnum = CurrCondSet->CondAttDefNames(CurrDVProp->att)) == 0) {
			attnum = ++(CurrCondSet->CondAttDefCount);
			CurrCondSet->CondAttDefNames.add(CurrDVProp->att, attnum);
			CurrCondSet->CondAttDefaults.add(CurrDVProp->action, attnum);
			if (!strcmp(CurrDVProp->action, "flag")) {
				CurrCondSet->CondAttDefFlags.add((fprop = new flagprop), attnum);
				SetDVFlagProps(fprop, CurrDVProp);
			}
			else if (!strcmp(CurrDVProp->action, "exclude"))
				CurrCondSet->CondAttDefExclude.add(attnum);
			else if (!strcmp(CurrDVProp->action, "include"))
				CurrCondSet->CondAttDefInclude.add(attnum);
		}
		else
			LogEvent(logwarn, 1, "Duplicate default action for ",
			                      CurrDVProp->att, " in ", DitavalFile);
		return;
	}
	if (!CurrDVProp->action) {  // use the default if any
		if ((attnum = CurrCondSet->CondAttDefNames(CurrDVProp->att)) != 0)
			CurrDVProp->action = CurrCondSet->CondAttDefaults.find(attnum);
		else if (CurrCondSet->DVDefPropAction)
			CurrDVProp->action = CurrCondSet->DVDefPropAction;
		else {
			LogEvent(logwarn, 1, "No action for ", CurrDVProp->att,
			                      CurrDVProp->val, " in ", DitavalFile);
			return;
		}
		if (!strcmp(CurrDVProp->action, "flag"))
			dfprop = (flagprop *) (CurrCondSet->CondAttDefFlags.find(attnum));
	}

	// using CurrDVProp, set exclude, passthrough, and flagging lists
	switch (DVActions(CurrDVProp->action)) {
		case 1:  // include
			if ((attnum = CurrCondSet->CondAttIncNames(CurrDVProp->att)) == 0) {
				attnum = ++(CurrCondSet->CondAttIncCount);
				CurrCondSet->CondAttIncNames.add(CurrDVProp->att, attnum);
			}
			if ((attlist = (DCnlist *) CurrCondSet->CondAttInclude.find(attnum)) == NULL)
				CurrCondSet->CondAttInclude.add(attlist = new DCnlist, attnum);
			attlist->add(CurrDVProp->val, attlist->count() + 1);
			DoInclude = true;
			break;

		case 2:  // exclude
			//if (!strcmp(CurrDVProp->att, "rev"))
			//	return;   // rev can't exclude
			if ((attnum = CurrCondSet->CondAttExcNames(CurrDVProp->att)) == 0) {
				attnum = ++(CurrCondSet->CondAttExcCount);
				CurrCondSet->CondAttExcNames.add(CurrDVProp->att, attnum);
			}
			if ((attlist = (DCnlist *) CurrCondSet->CondAttExclude.find(attnum)) == NULL)
				CurrCondSet->CondAttExclude.add(attlist = new DCnlist, attnum);
			attlist->add(CurrDVProp->val, attlist->count() + 1);
			DoExclude = true;
			break;

		case 3:  // passthrough
			if ((attnum = CurrCondSet->CondAttPassNames(CurrDVProp->att)) == 0) {
				attnum = ++(CurrCondSet->CondAttPassCount);
				CurrCondSet->CondAttPassNames.add(CurrDVProp->att, attnum);
			}
			if ((attlist = (DCnlist *) CurrCondSet->CondAttPassthrough.find(attnum)) == NULL)
				CurrCondSet->CondAttPassthrough.add(attlist = new DCnlist, attnum);
			attlist->add(CurrDVProp->val, attlist->count() + 1);
			AttrPassthrough = true;
			break;

		case 4:  // flag
			if ((attnum = CurrCondSet->CondAttFlagNames(CurrDVProp->att)) == 0) {
				attnum = ++(CurrCondSet->CondAttFlagCount);
				CurrCondSet->CondAttFlagNames.add(CurrDVProp->att, attnum);
			}
			if ((attlist = (DCnlist *) CurrCondSet->CondAttFlagging.find(attnum)) == NULL)
				CurrCondSet->CondAttFlagging.add(attlist = new DCnlist, attnum);

			// make name for flag (condition)
			flagname = NewName(CurrCondSet->DVname, strlen(CurrCondSet->DVname) + 
			           strlen(CurrDVProp->att) + strlen(CurrDVProp->val) + 1);
			strcat(flagname, CurrDVProp->att);
			strcat(flagname, CurrDVProp->val);
			if ((flagnum = CondFlagNames(flagname)) == 0)
				CondFlagNames.add(flagname, flagnum = ++CondFlagCount);
			else  // can't have more than one attrval instance in ditaval file
				LogEvent(logwarn, 1, "Duplicate prop for ", flagname,
								             " in ", DitavalFile);
			if ((fprop = (flagprop *) CondFlagProps.find(flagnum)) == NULL)
				CondFlagProps.add(fprop = new flagprop, flagnum);

			// associate attr/val with flag
			if (!attlist->getitem(CurrDVProp->val))  // ignore duplicates
				attlist->add(CurrDVProp->val, flagnum);

			// define flag props
			if (dfprop  // action was set from default
			 && !CurrDVProp->fgcolor
			 && !CurrDVProp->bgcolor
			 && !CurrDVProp->style)
				*fprop = *dfprop;
			else
				SetDVFlagProps(fprop, CurrDVProp);

			if (CurrDVProp->chbar)
				SetDVChangebarProps(fprop, CurrDVProp);

			DoFlag = true;
			break;

		default:
			break;
	}
}


void
XMLrd::SetDVFlagProps(flagprop *fprop, dvprop *dprop)
{
	fprop->fcolor = GetDVColor(dprop->fgcolor, DitavalFile);
	fprop->bcolor = GetDVColor(dprop->bgcolor, DitavalFile);
	if (dprop->style)
		fprop->style = (unl) DVStyles(dprop->style);
	if (dprop->startflag) {
		fprop->startimg = dprop->startflag->image;
		fprop->startalt = dprop->startflag->text;
	}
	if (dprop->endflag) {
		fprop->endimg = dprop->endflag->image;
		fprop->endalt = dprop->endflag->text;
	}
}


void
XMLrd::SetDVChangebarProps(flagprop *fprop, dvprop *dprop)
{
	fprop->style |= FSchange;

	// process changebar string (color, style?): underspecified
	char *chp = NULL;
	char *spc = NULL;
	unl color = 0;
	char *str = chp = NewName(dprop->chbar);
	while (*chp) {
		spc = strchr(chp, ' ');
		if (spc) {
			*spc++ = '\0';
			while (*spc && (*spc == ' '))
				spc++;
		}
		else
			spc = chp + strlen(chp);

		// see if chp is a color
		if ((color = GetDVColor(chp)) != 0)
			fprop->chcolor = color;
		// or is it punct chars to use in margin
		else if (ispunct(*chp)) {
			char *ppt = chp;
			while ((ppt < spc)
			 && ispunct(*ppt))
				ppt++;
			if (ppt < spc)
				*ppt = '\0';
			fprop->chchars = NewName(chp);
		}
		else
			LogEvent(logwarn, 1, "Unknown changebar property ", chp,
													 " in ", DitavalFile);
		chp = spc;
	}
	DeleteName(str);

}



unl 
XMLrd::GetDVColor(char *str, char *src)
{
	if (!str || !*str)
		return 0;

	unl color = 0;

	if (*str == '#') {
		color = strtoul(str + 1, NULL, 16);
		if (color == 0)
			color = 1;  // distinguish black from nothing
	}
	else if (src
	 && ((color = (unl) DVColors(str)) == 0))  // not in list
		LogEvent(logwarn, 1, "Unknown color ", str,
								         " in ", src);

	return color;
}



// ini setting reading

bool XMLrd::CondAttrsProcessed = false;


void
XMLrd::SetCondAttrs(void)
{
	long attnum = 0;
	DCnlist *attlist = NULL;
	char *attname = NULL;
	char *equ = NULL;
	char *val = NULL;
	long vnum = 0;
	flagprop *fprop = NULL;
	DCnlist *flagnames = new DCnlist;

	CondAttrsProcessed = true;
	CurrCondSet->DVname = "D2Gini";

	if (IniFile->Section("ConditionalOptions")
	 && IniFile->Find("ConditionalDefaults")
	 && ((val = IniFile->StrRight()) != NULL)) {
		if (!stricmp(val, "include"))
			CurrCondSet->DVDefPropExclude = false;
		else if (!stricmp(val, "exclude"))
			CurrCondSet->DVDefPropExclude = true;
	}

	if (IniFile->Section("ConditionalDefaults")) {
		attnum = 0;
		while (IniFile->NextItem()) {
			CurrCondSet->CondAttDefNames.add((attname = IniFile->StrLeft()), ++attnum);
			if (IniFile->ListRight()) {
				while ((val = IniFile->ListItem()) != NULL) {
					if (!strcmp(val, "include")) {
						CurrCondSet->CondAttDefaults.add(val, attnum);
						CurrCondSet->CondAttDefInclude.add(attnum);
					}
					else if (!strcmp(val, "exclude")) {
						CurrCondSet->CondAttDefaults.add(val, attnum);
						CurrCondSet->CondAttDefExclude.add(attnum);
					}
#if 0
					else {  // flag name
						if ((vnum = CondFlagNames(val)) == 0)
							CondFlagNames.add(val, (vnum = ++CondFlagCount));
						flagnames->add(val, vnum);
						CurrCondSet->CondAttDefFlags.add(fprop = new flagprop, attnum);
						// something missing in here like below in DV processing ****
						// SetDVFlagProps(fprop, CurrDVProp);
					}
#endif
				}						
			}
		}
		CurrCondSet->CondAttDefCount = attnum;
	}

	if (IniFile->Section("ConditionalInclude")) {
		attnum = 0;
		while (IniFile->NextItem()) {
			CurrCondSet->CondAttIncNames.add((attname = IniFile->StrLeft()), ++attnum);
			if (IniFile->ListRight()) {
				vnum = 0;
				DoInclude = true;
				CurrCondSet->CondAttInclude.add(attlist = new DCnlist, attnum);
				while ((val = IniFile->ListItem()) != NULL)
					attlist->add(val, ++vnum);
			}
		}
		CurrCondSet->CondAttIncCount = attnum;
	}

	if (IniFile->Section("ConditionalExclude")) {
		attnum = 0;
		while (IniFile->NextItem()) {
			CurrCondSet->CondAttExcNames.add((attname = IniFile->StrLeft()), ++attnum);
			if (IniFile->ListRight()) {
				vnum = 0;
				DoExclude = true;
				CurrCondSet->CondAttExclude.add(attlist = new DCnlist, attnum);
				while ((val = IniFile->ListItem()) != NULL)
					attlist->add(val, ++vnum);
			}
		}
		CurrCondSet->CondAttExcCount = attnum;
	}

	if (IniFile->Section("ConditionalPassthrough")) {
		attnum = 0;
		while (IniFile->NextItem()) {
			CurrCondSet->CondAttPassNames.add((attname = IniFile->StrLeft()), ++attnum);
			if (IniFile->ListRight()) {
				vnum = 0;
				CurrCondSet->CondAttPassthrough.add(attlist = new DCnlist, attnum);
				while ((val = IniFile->ListItem()) != NULL)
					attlist->add(val, ++vnum);
			}
		}
		CurrCondSet->CondAttPassCount = attnum;
		if (attnum)
			AttrPassthrough = true;
	}

	if (IniFile->Section("ConditionalFlagging")) {
		attnum = 0;
		while (IniFile->NextItem()) {
			CurrCondSet->CondAttFlagNames.add((attname = IniFile->StrLeft()), ++attnum);
			if (IniFile->ListRight()) {   // pairs are value=flag
				vnum = 0;
				DoFlag = true;
				CurrCondSet->CondAttFlagging.add(attlist = new DCnlist, attnum);
				while ((val = IniFile->ListItem()) != NULL) {
					if ((equ = strchr(val, '=')) != NULL) {
						*equ++ = '\0';
						if ((vnum = CondFlagNames(equ)) == 0)
							CondFlagNames.add(equ, (vnum = ++CondFlagCount));

						// associate attr/val with flag
						if (!attlist->getitem(val))  // ignore duplicates
							attlist->add(val, vnum);

						flagnames->add(equ, vnum);
					}
					else
						LogEvent(logwarn, 1, "Bad flag=value pair ", val,
						" for attribute ", attname, " in [ConditionalFlagging]");
				}
			}
		}
		CurrCondSet->CondAttFlagCount = attnum;
	}

	if (flagnames->next) {
		DCnlist *nxt = flagnames->next;
		while (nxt) {
			vnum = nxt->id;
			equ = (char *) nxt->name;

			// get flag properties if any
			if ((CondFlagProps.find(vnum) == NULL)
			 && IniFile->Section("ConditionalFlags")
			 && IniFile->Find(equ))
				SetIniFlagProps(vnum, IniFile->StrArrRight());

			nxt = nxt->next;
		}
	}

	if (IniFile->Section("ConditionalFlags")) {
		// pick up any flags like "conflict" not associated already
		while (IniFile->NextItem()) {
			if (!CondFlagNames.getitem(attname = IniFile->StrLeft())) {
				CondFlagNames.add(attname, vnum = ++CondFlagCount);
				SetIniFlagProps(vnum, IniFile->StrArrRight());
			}
		}
	}
}


void
XMLrd::SetIniFlagProps(long flagnum, char **sarr)
{
	if (!sarr || !*sarr)
		return;

	flagprop *fprop = new flagprop;
	char *str = NULL;
	unl pnum = 0;
	char *src = "[ConditionalFlags]";

	while ((str = *sarr++) != NULL) {
		if (!_strnicmp(str, "bgcolor=", 8))
			fprop->bcolor = GetDVColor(str + 8, src);
		else if (!_strnicmp(str, "fgcolor=", 8))
			fprop->fcolor = GetDVColor(str + 8, src);
		else if (!_strnicmp(str, "chcolor=", 8))
			fprop->chcolor = GetDVColor(str + 8, src);
		else if (!_strnicmp(str, "chchars=", 8))
			fprop->chchars = NewName(str + 8);
		else if (!stricmp(str, "startflag"))
			GetIniFlagInfo(fprop, CondFlagNames.find(flagnum), true);
		else if (!stricmp(str, "endflag"))
			GetIniFlagInfo(fprop, CondFlagNames.find(flagnum), false);
		else if (!stricmp(str, "bold"))
			fprop->style |= FSbold;
		else if (!stricmp(str, "italics"))
			fprop->style |= FSital;
		else if (!stricmp(str, "underline"))
			fprop->style |= FSunder;
		else if (!stricmp(str, "doubleline"))
			fprop->style |= FSdouble;
		else if (!stricmp(str, "overline"))
			fprop->style |= FSover;
		else if (!stricmp(str, "numunderline"))
			fprop->style |= FSnumunder;
		else if (!stricmp(str, "strike"))
			fprop->style |= FSstrike;
		else if (!stricmp(str, "change"))
			fprop->style |= FSchange;
		else if ((pnum = (unl) DVStyles(str)) != 0)
			fprop->style |= pnum;
		else
			LogEvent(logwarn, 1, "Unknown property ", str,
													 " in ", src);
	}
	CondFlagProps.add(fprop, flagnum);
}



void
XMLrd::GetIniFlagInfo(flagprop *fprop, char *fname, bool start)
{
	if (start) {
		if (IniFile->Section("CondStartFlagImages")
		 && IniFile->Find(fname))
			fprop->startimg = IniFile->StrRight();
		if (IniFile->Section("CondStartFlagAltText")
		 && IniFile->Find(fname))
			fprop->startalt = IniFile->StrRight();
	}
	else {
		if (IniFile->Section("CondEndFlagImages")
		 && IniFile->Find(fname))
			fprop->endimg = IniFile->StrRight();
		if (IniFile->Section("CondEndFlagAltText")
		 && IniFile->Find(fname))
			fprop->endalt = IniFile->StrRight();
	}
}




// exclusion processing

bool XMLrd::DoExclude = false;;
bool XMLrd::DoInclude = false;;

#if 0
long XMLrd::CondAttExcCount = 0;
DCnlist XMLrd::CondAttExcNames;
DCvlist XMLrd::CondAttExclude;
#endif

#endif


long XMLrd::ExcludeDepth = 0;


bool
XMLrd::CheckFilterConds(const char **atts)
{
	bool condset = false;

	for (long i = 0; atts[i] != NULL; i += 2) {
		if (FilterConditions
		 && (FilterDialogAttributes.getitem(atts[i]) != 0)) {
			// reserve for dynamic filtering, generate classes
			const char *val = atts[i + 1];  // one or more values
			if (val) {
				condset = true;
				WriteFilterClass((char *) atts[i], (char *) val);
			}
		}
	}

	return condset;
}

/*
			if (XMLrd::TextGroup) {
				DCctl *cp = new DCctl(etext, 2, 110, 3);
				char *str = NewName("Code:", 128);
				strcat(str, atts[i]);
				strcat(str, atts[i + 1]);
				cp->ex(str);
				XMLrd::TextGroup->add(*cp);
			}
*/
#ifdef DITA

bool
XMLrd::CondAttrsExclude(const char **atts)
{
	// for each attribute in [ConditionalExclude] that is in atts,
	// return true to exclude it if all values in it are excluded

	if (CheckFilterConds(atts))
		return false;

	if (!DoExclude
	 && !DoInclude)
		return false;

	const char *attname = NULL;
	const char *attval = NULL;

	for (long i = 0; atts[i] != NULL; i += 2) {
		attname = atts[i];
		attval = atts[i + 1];

		// process complex otherprops, as in: 
		// otherprops="proglang(java  cpp)  commentformat(javadoc  html)"
		if (!strcmp(attname, "otherprops")
		 && strchr(attval, '(')) {
			const char *apt = attval;
			const char *oparen = NULL;
			const char *cparen = NULL;
			while (*apt) {
				while (*apt == ' ')
					apt++;
				if (*apt
				 && ((oparen = strchr(apt, '(')) != NULL)
				 && ((cparen = strchr(oparen, ')')) != NULL)) {
					char *name = NewName(apt, oparen - apt);
					char *val = NewName(++oparen, cparen - oparen);
					bool ret = CheckOneCondAttr(name, val);
					DeleteName(name);
					DeleteName(val);
					if (ret)
						return true;
				}
			}
		}
		else if (CheckOneCondAttr(attname, attval))
			return true;;
	}
	return false;
}


bool
XMLrd::CheckOneCondAttr(const char *attname, const char *attval)
{
	long attnum = 0;
	DCnlist *attlist = NULL;
	bool exclude = CurrCondSet->DVDefPropExclude;  // default is include

	if ((attnum = CurrCondSet->CondAttDefNames(attname)) != 0) {
		if (CurrCondSet->CondAttDefExclude.find(attnum))
			exclude = true;
		else if (CurrCondSet->CondAttDefInclude.find(attnum))
			exclude = false;
	}
	if (!exclude) {  // if default is include, return true if all vals excluded
		if (((attnum = CurrCondSet->CondAttExcNames(attname)) != 0)
		 && ((attlist = (DCnlist *) CurrCondSet->CondAttExclude.find(attnum)) != NULL)) {
			// some values for this attr are excluded, check current ones
			if (CheckCondExcVals(attval, attlist))
				return true;  // all vals excluded for this attr
		}
	}
	else {  // if default is exclude, return true if no vals are included
		if (((attnum = CurrCondSet->CondAttIncNames(attname)) != 0)
		 && ((attlist = (DCnlist *) CurrCondSet->CondAttInclude.find(attnum)) != NULL)) {
			// some values for this attr are included, check current ones
			if (!CheckCondIncVals(attval, attlist))
				return true;  // no vals included for this attr
		}
	}
	return false;
}


bool
XMLrd::CheckCondExcVals(const char *attr, DCnlist *attlist)
{
	char *att = NULL;
	char *spc = NULL;

	char *attp = att = NewName(attr);
	bool match = false;
	while (*attp) {
		spc = strchr(attp, ' ');
		if (spc) {
			*spc++ = '\0';
			while (*spc && (*spc == ' '))
				spc++;
		}
		else
			spc = attp + strlen(attp);

		// use wildcard match where the attlist item is the wildcard
		if (attlist->getwild(attp))
			match = true;
		else {
			match = false;
			break;  // value not excluded
		}
		attp = spc;
	}
	DeleteName(att);
		
	return match;
}


bool
XMLrd::CheckCondIncVals(const char *attr, DCnlist *attlist)
{
	char *att = NULL;
	char *spc = NULL;

	char *attp = att = NewName(attr);
	bool match = false;
	while (*attp) {
		spc = strchr(attp, ' ');
		if (spc) {
			*spc++ = '\0';
			while (*spc && (*spc == ' '))
				spc++;
		}
		else
			spc = attp + strlen(attp);

		// use wildcard match where the attlist item is the wildcard
		if (attlist->getwild(attp)) {
			match = true;
			break;  // value included
		}
		else
			match = false;
		attp = spc;
	}
	DeleteName(att);
		
	return match;
}



// passthrough processing

bool XMLrd::AttrPassthrough = false;

#if 0
long XMLrd::CondAttPassCount = 0;
DCnlist XMLrd::CondAttPassNames;
DCvlist XMLrd::CondAttPassthrough;
#endif

void
XMLrd::SetPassthroughAttrs(DCelem *einfo, DCgroup *tgrp)
{
	char *nm = NULL;
	DCnlist *attlist = NULL;
	long vnum = 0;
	char *att = NULL;
	char *spc = NULL;
	char *attp = NULL;
	DCctl *cp = NULL;

	for (long i = 1; i < einfo->attrcnt; i++) {
		nm = einfo->attrnames->find(i);
		if ((vnum = CurrCondSet->CondAttPassNames(nm)) != 0) { // attr name is in list
			attlist = (DCnlist *) CurrCondSet->CondAttPassthrough.find(vnum);
			attp = att = NewName(einfo->attrvals->find(i));
			while (*attp) {
				spc = strchr(attp, ' ');
				if (spc) {
					*spc++ = '\0';
					while (*spc && (*spc == ' '))
						spc++;
				}
				else
					spc = attp + strlen(attp);
				if (attlist->getwild(attp)) {  // attr val is in list
					if (!einfo->attrgrp) {
						einfo->attrgrp = new DCgroup;
						tgrp->add(*((DCctl *) einfo->attrgrp));
					}
					cp = new DCctl(etext, 2, 121, 8);
					cp->ex(nm, strlen(nm));
					einfo->attrgrp->add(*cp);
					cp = new DCctl(etext, 2, 121, 9);
					cp->ex(attp, strlen(attp));
					einfo->attrgrp->add(*cp);
				}
			}
			DeleteName(att);
		}
	}
}
#endif


// flagging processing

bool XMLrd::DoFlag = false;
bool XMLrd::UseConditionalFlagging = true;
char *XMLrd::CSSFlagsFile = NULL;
bool XMLrd::WriteFlagsFile = true;
bool XMLrd::ReferenceFlagsFile = true;

#if 0
long XMLrd::CondAttFlagCount = 0;
DCnlist XMLrd::CondAttFlagNames;
DCvlist XMLrd::CondAttFlagging;
#endif

long XMLrd::CondFlagCount = 0;
DCnlist XMLrd::CondFlagNames;
DCvlist XMLrd::CondFlagProps;

DCvlist XMLrd::CondFlagLevels;
DClist *XMLrd::CurrCondFlags;
DClist *XMLrd::NextCondFlags;


// set flags specified for current level in dclist in CondFlagLevels


void
XMLrd::CheckFlags(const char **atts)
{
	if (!DoFlag)
		return;

	DClist *flaglist = new DClist;
	DClist *oldlist = NULL;

	if ((oldlist = (DClist *) CondFlagLevels.find(CurrDepth)) != NULL) {
		oldlist->empty();
		delete oldlist;
		CondFlagLevels.remove(CurrDepth);
	}

	const char *attname = NULL;
	const char *attval = NULL;

	for (long i = 0; atts[i] != NULL; i += 2) {
		attname = atts[i];
		attval = atts[i + 1];

		// process complex otherprops, as in: 
		// otherprops="proglang(java  cpp)  commentformat(javadoc  html)"
		if (!strcmp(attname, "otherprops")
		 && strchr(attval, '(')) {
			const char *apt = attval;
			const char *oparen = NULL;
			const char *cparen = NULL;
			while (*apt) {
				while (*apt == ' ')
					apt++;
				if (*apt
				 && ((oparen = strchr(apt, '(')) != NULL)
				 && ((cparen = strchr(oparen, ')')) != NULL)) {
					char *name = NewName(apt, oparen - apt);
					char *val = NewName(++oparen, cparen - oparen);
					CheckOneFlagAttr(attname, attval, flaglist);
					DeleteName(name);
					DeleteName(val);
				}
			}
		}
		else
			CheckOneFlagAttr(attname, attval, flaglist);
	}

	if (flaglist->count() == 0)
		delete flaglist;
	else
		CondFlagLevels.add(flaglist, CurrDepth);
}


void
XMLrd::CheckOneFlagAttr(const char *attname, const char *attval, DClist *flaglist)
{
	char *att = NULL;
	char *spc = NULL;
	char *attp = NULL;
	long attnum = 0;
	DCnlist *attlist = NULL;
	unl flag = 0;

	if (((attnum = CurrCondSet->CondAttFlagNames(attname)) != 0)
	 && ((attlist = (DCnlist *) CurrCondSet->CondAttFlagging.find(attnum)) != NULL)) {
		attp = att = NewName(attval);
		while (*attp) {
			spc = strchr(attp, ' ');
			if (spc) {
				*spc++ = '\0';
				while (*spc && (*spc == ' '))
					spc++;
			}
			else
				spc = attp + strlen(attp);

			// use wildcard match where the attlist item is the wildcard
			if ((flag = (unl) attlist->getwild(attp)) != 0)
				flaglist->add(flag);

			attp = spc;
		}
		DeleteName(att);
	}
}




// insert cond_use, cond_end, and cond_row where needed per flag


bool
XMLrd::SetFlags(long depth)
{
	bool flagset = false;

	if (!DoFlag
	 || !TextGroup)
		return false;

	// determine flags set in CondFlagLevels for depth and above,
	// put them in NextCondFlags
	long i = 0;
	DClist *flaglist = NULL;
	NextCondFlags = new DClist;

	for (i = 1; i <= depth; i++) {
		if ((flaglist = (DClist *) CondFlagLevels.find(i)) != NULL)
			flaglist->all(SetNextCondFlags);
	}

	// for each in CurrCondFlags but not in NextCondFlags, remove
	// imnem, 2, 63, flagnum
	long cnt = CurrCondFlags ? CurrCondFlags->count() : 0;
	long flag = 0;
	DCctl *cp = NULL;

	for (i = 1; i <= cnt; i++) {
		if (!NextCondFlags->find(flag = CurrCondFlags->number(i))) {
			cp = new DCctl(imnem, 2, 63, (uns) flag);
			TextGroup->add(*cp);
			flagset = true;
		}
	}

	// for each in NextCondFlags but not in CurrCondFlage, add
	// imnem, 2, 62, flagnum
	cnt = NextCondFlags->count();
	for (i = 1; i <= cnt; i++) {
		if (!CurrCondFlags
		 || !CurrCondFlags->find(flag = NextCondFlags->number(i))) {
			cp = new DCctl(imnem, 2, 62, (uns) flag);
			TextGroup->add(*cp);
			flagset = true;
		}
	}

	if (CurrCondFlags) {
		CurrCondFlags->empty();
		delete CurrCondFlags;
	}
	CurrCondFlags = NextCondFlags;
	NextCondFlags = NULL;
	return flagset;
}


void
XMLrd::EndFlags(void)
{
	if (!DoFlag
	 || !TextGroup)
		return;

	long i = 0;
	long cnt = CurrCondFlags ? CurrCondFlags->count() : 0;
	long flag = 0;
	DCctl *cp = NULL;

	for (i = 1; i <= cnt; i++) {
		flag = CurrCondFlags->number(i);
		cp = new DCctl(imnem, 2, 63, (uns) flag);
		TextGroup->add(*cp);
	}

	if (CurrCondFlags) {
		CurrCondFlags->empty();
		delete CurrCondFlags;
	}

	CurrCondFlags = NextCondFlags = NULL;
}


void
XMLrd::SetNextCondFlags(long flag)
{
	NextCondFlags->add(flag);
}


void
XMLrd::SetRowFlags(long depth, DCgroup *gp)
{
	if (!DoFlag)
		return;

	// set flags for current level only, do not change CurrCondFlags
	// remove from CondFlagLevels because flags self-cancel at end of row
	// imnem, 2,  65, flagnum

	DClist *flaglist = (DClist *) CondFlagLevels.find(depth);
	if (!flaglist)
		return;

	long i = 0;
	long cnt = flaglist->count();
	long flag = 0;
	DCctl *cp = NULL;

	for (i = 1; i <= cnt; i++) {
		if ((flag = flaglist->number(i)) != 0) {
			cp = new DCctl(imnem, 2, 65, (uns) flag);
			gp->add(*cp);
		}
	}

	flaglist->empty();
	delete flaglist;
	CondFlagLevels.remove(depth);
}



// write condition definitions in resources section for all flags
#ifdef DITA


void
XMLrd::ProcCondFlagDef(DCgroup *gp, long num)
{
  DCctl *cp = NULL;
  DCctl *cp2 = NULL;


  DCgroup *cgp = new DCgroup(DCctl(group, 2, 60, (uns) num));
	gp->add(*cgp);

	char *txt = CondFlagNames.find(num);
	if (txt) {
		cp = new DCctl(ename, 2, 61, 1);
		cp->ex(txt, strlen(txt));
		cgp->add(*cp);
	}
	else
		assert(0);  // nameless flag


  DCgroup *fgp = new DCgroup(DCctl(group, 2, 61, 2)); // format group
  cgp->add(*fgp);

	flagprop *fp = (flagprop *) CondFlagProps.find(num);

	if (fp->fcolor) {
		cp = new DCctl((fp->fcolor < 9) ? imnem : ibyte, 2, 61, 4, fp->fcolor);
		fgp->add(*cp);
	}

	if (fp->bcolor) {
		cp = new DCctl((fp->bcolor < 9) ? imnem : ibyte, 2, 61, 5, fp->bcolor);
		fgp->add(*cp);
	}

  if (fp->style & FSbold) { // bold
		cp = new DCctl(imnem, 2, 15, 4, 4);
		fgp->add(*cp);
	}

  if (fp->style & FSital) { // italic
		cp = new DCctl(imnem, 2, 15, 5, 1);
		fgp->add(*cp);
	}

  if (fp->style & FSunder) { // underline
		cp = new DCctl(imnem, 2, 15, 11, 1);
		fgp->add(*cp);
	}

  if (fp->style & FSnumunder) { // numeric underline
		cp = new DCctl(imnem, 2, 15, 11, 4);
		fgp->add(*cp);
	}

  if (fp->style & FSdouble) { // double underline
		cp = new DCctl(imnem, 2, 15, 11, 2);
		fgp->add(*cp);
	}

  if (fp->style & FSstrike) { // strikethrough
		cp = new DCctl(imnem, 2, 15, 9, 2);
		fgp->add(*cp);
	}

  if (fp->style & FSover) { // overline
		cp = new DCctl(imnem, 2, 15, 11, 3);
		fgp->add(*cp);
	}

  if (fp->style & FSchange) { // change bar
		cp = new DCctl(imnem, 2, 15, 9, 3);
		fgp->add(*cp);

		if (fp->chcolor) {
			cp = new DCctl((fp->chcolor < 9) ? imnem : ibyte, 2, 61, 6, fp->chcolor);
			fgp->add(*cp);
		}

		if (fp->chchars) {
			cp = new DCctl(etext, 2, 61, 7);
			cp->ex(fp->chchars, strlen(fp->chchars));
			fgp->add(*cp);
		}
	}

	if (fp->startimg) {
		cp = new DCctl(etext, 2, 61, 8);
		cp->ex(fp->startimg, strlen(fp->startimg));
		fgp->add(*cp);
	}
	if (fp->startalt) {
		cp = new DCctl(etext, 2, 61, 9);
		cp->ex(fp->startalt, strlen(fp->startalt));
		fgp->add(*cp);
	}
	if (fp->endimg) {
		cp = new DCctl(etext, 2, 61, 10);
		cp->ex(fp->endimg, strlen(fp->endimg));
		fgp->add(*cp);
	}
	if (fp->endalt) {
		cp = new DCctl(etext, 2, 61, 11);
		cp->ex(fp->endalt, strlen(fp->endalt));
		fgp->add(*cp);
	}

}
#endif


// runtime conditions

bool XMLrd::FilterConditions = false;

// in drxml and drmxl, if attribute is in list,
// keep element and write class attr for it
DCnlist XMLrd::FilterDialogAttributes;
DCnlist *XMLrd::FilterClasses = NULL;
long XMLrd::FilterClassCount = 0;


void
XMLrd::SetFilterAttributes(void)
{
	if (!FilterConditions)
		return;

	// read [FilterAttributes] and populate FilterDialogAttributes
	if (!IniFile
	 || !IniFile->Section("FilterAttributes"))
		return;
	while (IniFile->NextItem()) {
		char *aname = IniFile->StrLeft();
		char *vlist = IniFile->StrRight();
		DCnlist *npt = new DCnlist();
		npt->addlist(vlist);
		FilterDialogAttributes.add(aname, (long) npt);
		while ((npt = npt->next) != NULL) {
			npt->id *= 2;
			if (*(npt->name) == '-') {
				npt->name++;
				npt->id ++;
			}
		}
	}
}


void 
XMLrd::WriteFilterClass(char *aname, char *aval)
{
	if (!FilterConditions)
		return;

	// in drxml, drmxl ,and drmif: write class attribute value ****
	DCnlist *vpt = NULL;
	long val = 0;

	if ((vpt = (DCnlist *) FilterDialogAttributes.getitem(aname)) == NULL)
		return;  // attr name not in list

	if (!FilterClasses)
		FilterClasses = new DCnlist();

	char *spc = NULL;
	DCnlist *npt = new DCnlist();
	if (((spc = strchr(aval, ' ')) != NULL)
	 && (*(spc + 1) != '\0')) {  // is a list
		npt->addlist(aval);
	}
	else
		npt->add(aval, 1);

	char *cstr = NULL;
	char *astr = NULL;
	DCctl *cp = NULL;

	while ((npt = npt->next) != NULL) {
		if (((aval = (char *) npt->name) != NULL)) {
		// && ((val = vpt->getitem(aval)) != 0)) { // val is in list
			cstr = NewName(aname, strlen(aname) + strlen(aval) + 2);
			strcat(cstr, "_");
			strcat(cstr, aval);

			FilterClasses->add(cstr, ++FilterClassCount);
		}
	}
}


void XMLrd::SetFilterClasses(bool tbl, bool inl)
{
	if (!FilterClasses
	 || !TextGroup)
		return;

	if (tbl
	 && !TableRowGroup)
		return;

	DCnlist *dnp = FilterClasses;
	while ((dnp = dnp->next) != NULL) {
		const char *cstr = dnp->name;
		DCctl *cp = new DCctl(etext, 2, 110, 3);
		char *astr = NewName((tbl && TableRowGroup) ? "rowclass" :
			                   (inl ? "charclass" : "paraclass"), strlen(cstr) + 12);
		strcat(astr, ":");
		strcat(astr, cstr);
		strcat(astr, " ");
		cp->ex(astr);
		TextGroup->add(*cp);
	}
	delete FilterClasses;
	FilterClasses = NULL;
	FilterClassCount = 0;
}



// end of drxmldv.cpp

