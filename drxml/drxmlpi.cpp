// drxmlpi.cpp is the code for dcl XML processing instructions, JHG
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




DCctl *XMLrd::TopicAlias = NULL;



void
XMLrd::ProcPI(const char *target, const char *data)
{
	// DocBook uses dbfo and dbhtml as targets
	// DITA can use dtpdf, dthtm, dtrtf, and dtall
	// the data is a set of pseudo-attributes, attr="val"

#ifdef DITA
	if (RecordingElements)
		RecPI(target, data);
#endif

	if (!strncmp(target, "debug", 5)) { // debug break
#ifdef _DEBUG
			__asm int 3
#endif
		return;
	}

	// <?FM MARKER [TopicAlias] IDH_about?>
	if (!stricmp(target, "FM")
	 && !strnicmp(data, "MARKER [TopicAlias]", 19)) {
		SetTopicAlias(NewName(data + 20));
		return;
	}

	if (strncmp(target, "dt", 2))  // not DITA
		return;

	if (!strcmp(target, "dthtm")) {
		if (OutType != 'N')
			return;
	}
	else if (!strcmp(target, "dtrtf")) {
		if ((OutType != 'S')
		 && (OutType != 'H'))
			return;
	}
	else if (strcmp(target, "dtall"))
		return;

	if (CurrElemInfo) {
		if ((PendingLen == 1)
		 && (*PendingText == ' ')) {
			PendingText = NULL;
			PendingLen = 0;
		}
		else
			FlushText();
	}

	char **piattrs = ProcPIAttrs(data);
	// attrs are in same format as for regular elements now

	for (long i = 0; piattrs[i] != NULL; i += 2)
		ProcOnePIAttr(piattrs[i], piattrs[i + 1]);
}


char **
XMLrd::ProcPIAttrs(const char *data)
{
	char **atts = NULL;
	const char *start = NULL;
	const char *end = NULL;
	long cnt = 0;
	DCnlist alist;

	while (*data) {
		while (*data
		 && isspace(*data))
			data++;
		start = data;
		while (*data
		 && (*data != '='))
			data++;
		end = data - 1;
		while ((end > start)
		 && isspace(*end))
			end--;
		if (end > start)
			alist.add(NewName(start, (end - start) + 1), ++cnt);
		if (*data == '=') {
			data++;
			while (*data
			 && isspace(*data))
				data++;
			if (*data == '"') {
				start = ++data;
				while (*data) {
					if (*data == '\\')
						data++;
					else if (*data == '"')
						break;
					data++;
				}
				end = data - 1;
				if (end >= start)
					alist.add(NewName(start, (end - start) + 1), ++cnt);
			}
			else if (*data == '\'') {
				start = ++data;
				while (*data) {
					if (*data == '\\')
						data++;
					else if (*data == '\'')
						break;
					data++;
				}
				end = data - 1;
				if (end >= start)
					alist.add(NewName(start, (end - start) + 1), ++cnt);
			}
			if (*data)
				data++;
		}
	}

	if (cnt) {
		if ((cnt & 1) == 1) // odd number, so no value
			cnt++;            // make it even, with NULL value

		atts = (char **) new char[sizeof(char *) * (cnt + 1)];

		for (long i = 0; i < cnt; i++)
			atts[i] = alist.dequeue();
		atts[i] = NULL;  // terminator
	}

	return atts;
}


char *XMLrd::NextOutputclass = NULL;

DCnlist XMLrd::HyperTypes;
npair XMLrd::InitHyperTypes[] = {
	{ 2, "Anchor"},
	{ 4, "Jump"},
	{ 5, "Popup"},
	{ 6, "Alert"},
	{ 7, "Link"},
	{ 15, "Target"},
	NULL
};


DCctl *XMLrd::PendingBreak = NULL;

void
XMLrd::ProcOnePIAttr(char *name, char *val)
{
	if (!name || !*name)
		return;

	DCctl *cp = NULL;

#if 0
	if (!stricmp(name, "LineBreak")) {
		cp = new DCctl(imnem, 2, 11, 10, 1);
		if (TextGroup)
			TextGroup->add(*cp);
		return;
	}

	if (!stricmp(name, "ColumnBreak")) {
		PendingBreak = new DCctl(imnem, 2, 11, 3, 1);
		return;
	}

	if (!stricmp(name, "PageBreak")) {
		PendingBreak = new DCctl(imnem, 2, 11, 4, 1);
		return;
	}
#endif


	// if name is known, set value for it
	if (!val || !*val)
		return;

	if (!stricmp(name, "Break")) {
		cp = new DCctl(imnem, 2, 11, 10, 1);

		if (!stricmp(val, "line")) {
			if (TextGroup
			 && (!InCodeblock
			  || UseCodeblockPI))
				TextGroup->add(*cp);
		}
		else if (!stricmp(val, "column")) {
			cp->dat(3);
			PendingBreak = cp;
		}
		else if (!stricmp(val, "page")) {
			cp->dat(4);
			PendingBreak = cp;
		}
		return;
	}
#ifdef DITA
	if (!stricmp(name, "ditaval")) {
		SetNewConditionSet(val, true);
		return;
	}
	if (!stricmp(name, "branch")) {
		SetNewBranch(NewName(val));
		return;
	}
#endif

  // affects target of following xref only
	if (!stricmp(name, "xrefbranch")) {
		XrefBranch = NewName(val);
		return;
	}

  // affects target of following conref only
	if (!stricmp(name, "conrefbranch")) {
		ConrefBranch = NewName(val);
		return;
	}

  // affects target of following keyref only
	if (!stricmp(name, "keyrefbranch")) {
		KeyrefBranch = NewName(val);
		return;
	}

	if (!stricmp(name, "topic-alias")
	 || !stricmp(name, "topicalias")) {  // put before topic title
		SetTopicAlias(val);
		return;
	}

#if 0
	if (!stricmp(name, "list-presentation")) {
		// values are "list" or "table"
		return;
	}
#endif


	if (!stricmp(name, "BorderFormat")) {
		BorderFmt = val;
		return;
	}

	if (BorderType
	 && !stricmp(name, BorderType)) {
		BorderFmt = val;
		return;
	}

	if (!stricmp(name, "ShadeFormat")) {
		ShadeFmt = val;
		return;
	}

	if (ShadeType
	 && !stricmp(name, ShadeType)) {
		ShadeFmt = val;
		return;
	}


	if (!stricmp(name, "SimpleTableWidth")) {
		SimpleTableWidth = val;
		return;
	}

	if (!stricmp(name, "SimpleTableRelCol")) {
		SimpleTableRelCol = val;
		return;
	}


	if (!strnicmp(name, "Hyper", 5)) {
		uns num = 0;
		num = (uns) HyperTypes(name + 5);
		cp =  new DCctl(etext, 2, 112, num);
		cp->ex(val, strlen(val));
		if (TextGroup)
			TextGroup->add(*cp);
		return;
	}

	if (!stricmp(name, "alinkjump")
	 || !stricmp(name, "klinkjump")) {
		cp = new DCctl(etext, 2, 112, 7);
		char *txt = NewName(strlen(val) + 7);
		strncat(txt, name, 5);
		strcat(txt, ":");
		strcat(txt, val);
		cp->ex(txt, strlen(txt));
		if (TextGroup)
			TextGroup->add(*cp);
		return;
	}

	if (!stricmp(name, "outputclass")) {
		NextOutputclass = val;
		return;
	}

	// any others become the equivalent of Frame markers
	cp = new DCctl(etext, 2, 110, 3);
	uns len = strlen(name) + strlen(val) + 2;
	char *txt = NewName(name, len);
	strcat(txt, ":");
	strcat(txt, val);
	cp->ex(txt, len);
	if (TextGroup)
		TextGroup->add(*cp);
}


void
XMLrd::SetTopicAlias(char *val)
{
	DCctl *cp = NULL;

	if (!TopicAlias)
		TopicAlias = new DCgroup();
	cp = new DCctl(etext, 2, 112, 16);
	cp->ex(val, strlen(val));
	((DCgroup *) TopicAlias)->add(*cp);
}



// Variables

long XMLrd::CurrVar = 0;
bool XMLrd::InVar = false;
DCnlist XMLrd::VarNames;
DCclist XMLrd::VarFormats;

DCilist XMLrd::VarArraySize;  // count of array elems indexed by var
DCvlist XMLrd::VarArrayList;  // elem format group DClists indexed by var

// each elem format group DClist contains a DCgroup (2,92,4)
// for each element in the array in the order added to the array
DClist *XMLrd::CurrVarArrayElems = NULL;
long XMLrd::CurrVarArrayBase = 0;
long XMLrd::CurrVarArrayItem = 0;


char *
XMLrd::IsVariable(DCelem *elem)
{
	if (elem->elpath
	 && IniFile->Section("VariableMaps")
	 && IniFile->FindElemPath(elem->elpath)) {
		char *name = IniFile->StrRight();
		if (!stricmp(name, "no")
		 || !stricmp(name, "none"))
			return NULL;
		return name;
	}

#ifdef DITA
	if (elem->checkType(epVar))
#endif
#ifdef UDOC
	if (elem->checkType(epData | (epVar << 8)))
#endif
		return elem->elemname;

	return NULL;
}


DCgroup *
XMLrd::SetVariable(const char *name)
{
	if (!name || !*name)
		return NULL;

	char *nstr = NULL;
	DCgroup *vf = NULL;
	long vnum = 0;

	if ((vnum = VarNames(name)) != 0) { // exists, add to array
		CurrVarArrayItem = VarArraySize.find(vnum);
		if (CurrVarArrayItem)
			CurrVarArrayElems = (DClist *) VarArrayList.find(vnum);
		else {
			CurrVarArrayElems = new DClist;
			VarArrayList.add(CurrVarArrayElems, vnum);
		}
		VarArraySize.replace(++CurrVarArrayItem, vnum);
		vf = new DCgroup(DCctl(group, 2, 92, 4));
		CurrVarArrayElems->add((long) vf);
		CurrVarArrayBase = vnum;
	}
	else {
		CurrVarArrayItem = 0;
		VarNames.addnew(name, ++CurrVar);
		vf = new DCgroup(DCctl(group, 2, 91, 2));
		VarFormats.add(vf, CurrVar);
	}
	return vf;
}


void
XMLrd::ProcessVariableStart(DCelem *einfo)
{
	char *vname = NULL;
	if ((vname = IsVariable(einfo)) != NULL) {
		DCgroup *vargp = SetVariable(vname);
		vargp->add(*(einfo->vartext = new DCgroup));
		if (!einfo->lasttext
		 && !einfo->elemtext
		 && (TextGroup != PageTextGroup))
			einfo->lasttext = TextGroup;
		//if (!UMXL) {
#ifdef DITA
			if (!CurrVarArrayItem)  // normal var
				UseVariable(CurrVar);
			else {                  // array instance
				long vn = (CurrVarArrayBase << 16) + CurrVarArrayItem;
				TextGroup->add(*(new DCctl(ilong, 2, 10, 8, vn)));
			}
#endif
		//}
		TextGroup = einfo->vartext;
		InVar = true;
	}
}


void
XMLrd::ProcessVariableEnd(DCelem *einfo)
{
	if (einfo->vartext) {  // end the var group
		if (einfo->elemtext)
			TextGroup = einfo->elemtext;
		else if (einfo->lasttext)
			TextGroup = einfo->lasttext;
		else
			TextGroup = PageTextGroup;
		InVar = false;
	}
}


void
XMLrd::UseVariable(char *vname)
{
	if (!vname
	 || !*vname)
		return;

	long vnum = VarNames(vname);
	if (!vnum)
		LogEvent(logwarn, 1, "Reference to undefined variable: ", vname);
	TextGroup->add(*(new DCctl(ilong, 2, 10, 2, vnum)));
}


void
XMLrd::UseVariable(long vnum)
{
	if (!vnum)
		return;

	TextGroup->add(*(new DCctl(ilong, 2, 10, 2, vnum)));
}




void XMLrd::WriteVariableDef(DCgroup *gp, long i)
{
	DCgroup *varp = new DCgroup(DCctl(group, 2, 90, (uns) i));
	gp->add(*varp);
	DCctl *cp = new DCctl(etext, 2, 91, 1);
	char *txt = VarNames.find(i);
	cp->ex(txt, strlen(txt));
	varp->add(*cp);
	varp->add(*((DCgroup *) VarFormats.find(i)));

	// handle variable arrays
	long count = 0;
	long idx = 0;
	DClist *vitems = NULL;
	DCgroup* vi = NULL;
	if (((count = VarArraySize.find(i)) != 0)  // it's an array
	 && ((vitems = (DClist *) VarArrayList.find(i)) != NULL)) {
		cp = new DCctl(ilong, 2, 92, 1, count);  // array elem count
		varp->add(*cp);
		for (idx = 1; idx <= count; idx++) {
			vi = new DCgroup(DCctl(group, 2, 92, 2)); // elem group
			varp->add(*vi);
			cp = new DCctl(ilong, 2, 92, 3, idx);      // elem index
			vi->add(*cp);
			vi->add(*((DCgroup *) vitems->dequeue())); // elem format
		}
	}
}


#if 0

// importance handling

DCnlist XMLrd::ImpAttrs;
DCvlist XMLrd::ImpAttrVals;
long XMLrd::ImpAttrCount = 0;

bool XMLrd::UseStepImportance = true;
bool XMLrd::UseImpRequired = false;
bool XMLrd::UseImpOptional = false;
bool XMLrd::UseImportanceAttribute = false;
bool XMLrd::UseImpAttr = false;
char *XMLrd::ImportanceRequired = "<b>[Required] </b>";
char *XMLrd::ImportanceOptional = "<i>[Optional] </i>";
DCgroup *XMLrd::CurrImpAttr = NULL;
long XMLrd::CurrImpVar = 0;


void
XMLrd::PredefineVariable(const char *vname, char *vdef)
{
	if (!vname
	 || !*vname
	 || !vdef
	 || !*vdef)
		return;

	TextGroup = SetVariable(vname);
	SetVarGroupContent(vdef, "predefined variable: ", vname);
	TextGroup = NULL;
}


void
XMLrd::SetVarGroupContent(char *str, char *vtyp, const char *vname)
{
	// parse for <format>
	char *ntok = NULL;
	char formname[FormNameMax] = "";
	bool typo = false;
	DCnlist *nestlist = NULL;
	DCilist *typolist = NULL;
	long fnest = 0;

	while ((ntok = strchr(str, '<')) != NULL) {
		if (ntok > str) {
			if ((ntok - str) > 1)
				PutTextStrCtl(str, ntok - str);
			else
				PutTextCharCtl(*str);
		}
		str = ntok;
		while (*str
		 && (*str != '>'))
			str++;
		if (!*str)
			LogEvent(logwarn, 1, "<format> tag missing ending '>': ", ntok,
			                     " in ", vtyp, vname);
		ntok++;
		if (*ntok != '/') {  // start the named format
			if (fnest) { // nest format
				if (!nestlist)
					nestlist = new DCnlist();
				nestlist->addnew(formname, fnest);
				if (!typolist)
					typolist = new DCilist();
				typolist->add(typo, fnest);
			}
			fnest++;
			strncpy(formname, ntok, str - ntok);
			if (SetTypographic(formname, true))
				typo = true;
			else
				SetFormat(formname, true); 
		}
		else if (!strnicmp(ntok + 1, formname, (str - ntok) - 1)) {
			// end any current format
			if (typo) {
				SetTypographic(formname, false);
				typo = false;
			}
			else
				SetFormat(NULL, true);
			*formname = '\0';
			if (--fnest) {
				strcpy(formname, nestlist->find(fnest));
				typo = typolist->find(fnest);
			}
		}
		else
			LogEvent(logwarn, 1, "<format> token error: ", ntok,
			                     " in ", vtyp, vname);
		if (*str)  // move after ending bracket
			ntok = ++str;
	}
	if (*str) { // more left, finish it
		if (strlen(str) > 1)
			PutTextStrCtl(str, strlen(str));
		else
			PutTextCharCtl(*str);
	}
	while (fnest) {
		if (typo)
			SetTypographic(formname, false);
		else if (*formname != '\0')
			SetFormat(NULL, true);
		if (--fnest) {
			strcpy(formname, nestlist->find(fnest));
			typo = typolist->find(fnest);
		}
	}

#if 0
	char *cpt = vdef;
	char *clast = vdef;
	char *typo = NULL;
	bool start = true;

	while (*cpt) {
		if (*cpt == '<') {  // start of typographic
			if (cpt > clast)
				FlushTextCtrls(clast, cpt - clast);
			if (*(typo = ++cpt) == '/') {
				start = false;
				typo++;
			}
			while (*cpt && (*cpt != '>'))
				cpt++;
			typo = NewName(typo, cpt - typo);
			if (!SetTypographic(typo, start))
				LogEvent(logwarn, 1, "Invalid typographic element: <", typo,
					     "> in ", vtyp, vname);
			if (!start)
				start = true;
			DeleteName(typo);
			typo = NULL;
			clast = cpt + 1;
			if (!*cpt)
				break;
		}
		cpt++;			
	}
	if (cpt > clast)
		FlushTextCtrls(clast, cpt - clast);
#endif
}


void
XMLrd::SetImportance(const char *impatt)
{
	char *adef = NULL;

	if (LangFile
	 && LangFile->Section("ImportanceText")
	 && LangFile->Find(impatt))
		adef = LangFile->StrRight();

	if (!adef
	 || !*adef) {
		UseImpAttr = false;
		CurrImpVar = 0;
		return;
	}

	char *name = NewName("importance", strlen(impatt) + 11);
	strcat(name, impatt);

	long vnum = 0;
	if ((vnum = VarNames(name)) != 0) {  // variable already defined
		DeleteName(name);
		CurrImpVar = vnum;
	}
	else {
		VarNames.add(name, ++CurrVar);
		CurrImpVar = CurrVar;

		// set new definition
		DCgroup *prev = TextGroup;
		TextGroup = new DCgroup(DCctl(group, 2, 91, 2));
		SetVarGroupContent(adef, "importance attribute: ", name);
		PutTextCharCtl(' ');
		VarFormats.add(TextGroup, CurrVar);
		TextGroup = prev;
	}

	UseImpAttr = true;
}



// note handling


DCnlist XMLrd::NoteTypes;
npair XMLrd::NoteTypesInit[] = {
 { ntnote, "note" },
 { ntattention, "attention" },
 { ntcaution, "caution" },
 { ntwarning, "warning" },
 { ntdanger, "danger" },
 { ntfastpath, "fastpath" },
 { ntimportant, "important" },
 { ntremember, "remember" },
 { ntrestriction, "restriction" },
 { nttip, "tip" },
 { ntother, "other" },
 NULL
};

DCvlist XMLrd::NoteHeads;
vpair XMLrd::NoteHeadsInit[] = {
 { ntnote, &NoteHead },
 { ntattention, &AttentionHead },
 { ntcaution, &CautionHead },
 { ntwarning, &WarningHead },
 { ntdanger, &DangerHead },
 { ntfastpath, &FastpathHead },
 { ntimportant, &ImportantHead },
 { ntremember, &RememberHead },
 { ntrestriction, &RestrictionHead },
 { nttip, &TipHead },
 { ntother, &OtherHead },
 NULL
};

DCvlist XMLrd::NoteGroups;
vpair XMLrd::NoteGroupsInit[] = {
 { ntnote, &NoteGroup },
 { ntattention, &AttentionGroup },
 { ntcaution, &CautionGroup },
 { ntwarning, &WarningGroup },
 { ntdanger, &DangerGroup },
 { ntfastpath, &FastpathGroup },
 { ntimportant, &ImportantGroup },
 { ntremember, &RememberGroup },
 { ntrestriction, &RestrictionGroup },
 { nttip, &TipGroup },
 { ntother, &OtherGroup },
 NULL
};

DCvlist XMLrd::NoteFormats;
vpair XMLrd::NoteFormatsInit[] = {
 { ntnote, &NoteFormat },
 { ntattention, &AttentionFormat },
 { ntcaution, &CautionFormat },
 { ntwarning, &WarningFormat },
 { ntdanger, &DangerFormat },
 { ntfastpath, &FastpathFormat },
 { ntimportant, &ImportantFormat },
 { ntremember, &RememberFormat },
 { ntrestriction, &RestrictionFormat },
 { nttip, &TipFormat },
 { ntother, &OtherFormat },
 NULL
};

notetyp XMLrd::CurrNoteType;

bool XMLrd::UseNoteHeads = true;

char *XMLrd::NoteHead = "<b>Note:</b>";
char *XMLrd::AttentionHead = "<i>Attention:</i>";
char *XMLrd::CautionHead = "<b>Caution:</b>";
char *XMLrd::WarningHead = "<b>Warning:</b>";
char *XMLrd::DangerHead = "<b>Danger:</b>";
char *XMLrd::FastpathHead = "<i>Fastpath:</i>";
char *XMLrd::ImportantHead = "<b>Important:</b>";
char *XMLrd::RememberHead = "<i>Remember:</i>";
char *XMLrd::RestrictionHead = "<i>Restriction:</i>";
char *XMLrd::TipHead = "<b>Tip:</b>";
char *XMLrd::OtherHead = "<i>Note:</i>";

DCgroup *XMLrd::NoteGroup = NULL;
DCgroup *XMLrd::AttentionGroup = NULL;
DCgroup *XMLrd::CautionGroup = NULL;
DCgroup *XMLrd::WarningGroup = NULL;
DCgroup *XMLrd::DangerGroup = NULL;
DCgroup *XMLrd::FastpathGroup = NULL;
DCgroup *XMLrd::ImportantGroup = NULL;
DCgroup *XMLrd::RememberGroup = NULL;
DCgroup *XMLrd::RestrictionGroup = NULL;
DCgroup *XMLrd::TipGroup = NULL;
DCgroup *XMLrd::OtherGroup = NULL;

bool XMLrd::UseNoteFormats = true;

char *XMLrd::NoteFormat = "NoteFormat";
char *XMLrd::AttentionFormat = "AttentionFormat";
char *XMLrd::CautionFormat = "CautionFormat";
char *XMLrd::WarningFormat = "WarningFormat";
char *XMLrd::DangerFormat = "DangerFormat";
char *XMLrd::FastpathFormat = "FastpathFormat";
char *XMLrd::ImportantFormat = "ImportantFormat";
char *XMLrd::RememberFormat = "RememberFormat";
char *XMLrd::RestrictionFormat = "RestrictionFormat";
char *XMLrd::TipFormat = "TipFormat";
char *XMLrd::OtherFormat = "OtherNoteFormat";


void 
XMLrd::SetNoteGroup(void *ph, long typ)
{
	// called during init for each item in NoteHeads
	if (!ph || !typ)
		return;

	char *head = *((char **) ph);
	if (!head || !*head)
		return;

	TextGroup = new DCgroup;
	SetVarGroupContent(head, "head for note type ", NoteTypes.find(typ));
	PutTextCharCtl(' ');
	*((DCgroup **) NoteGroups.find(typ)) = TextGroup;
	TextGroup = NULL;
}


void 
XMLrd::SetNoteHead(void)
{
	if ((CurrNoteType == ntnone)
	 || !TextGroup)
		return;

	DCgroup *phead = *((DCgroup **) NoteGroups.find(CurrNoteType));
	if (phead)
		TextGroup->add(*phead);
	CurrNoteType = ntnone;
}


char * 
XMLrd::SetNoteFormat(void)
{
	char **pform = (char **) NoteFormats.find(CurrNoteType);
	if (pform)
		return *pform;
	return NULL;
}



// TaskHeads

bool XMLrd::InTaskTopic = false;
bool XMLrd::UseTaskHeads = false;

DCvlist XMLrd::TaskHeads;
vpair XMLrd::TaskHeadsInit[] = {
 { thsteps, &StepsHead },
 { thprereq, &PrereqHead },
 { thcontext, &ContextHead },
 { thexample, &ExampleHead },
 { thresult, &ResultHead },
 { thpostreq, &PostreqHead },
 { thstepxmp, &StepxmpHead },
 { thstepresult, &StepresultHead },
 NULL
};

DCvlist XMLrd::TaskHeadGroups;
vpair XMLrd::TaskHeadGroupsInit[] = {
 { thsteps, &StepsGroup },
 { thprereq, &PrereqGroup },
 { thcontext, &ContextGroup },
 { thexample, &ExampleGroup },
 { thresult, &ResultGroup },
 { thpostreq, &PostreqGroup },
 { thstepxmp, &StepxmpGroup },
 { thstepresult, &StepresultGroup },
 NULL
};



char *XMLrd::StepsHead = "<b>Procedure:</b>";
char *XMLrd::PrereqHead = "Before you start:";
char *XMLrd::ContextHead = "<b>Purpose:</b>";
char *XMLrd::ExampleHead = "<i>For example:</i>";
char *XMLrd::ResultHead = "Results:";
char *XMLrd::PostreqHead = "After you finish:";
char *XMLrd::StepxmpHead = "<i>Example:</i>";
char *XMLrd::StepresultHead = "Result:";

DCgroup *XMLrd::StepsGroup = NULL;
DCgroup *XMLrd::PrereqGroup = NULL;
DCgroup *XMLrd::ContextGroup = NULL;
DCgroup *XMLrd::ResultGroup = NULL;
DCgroup *XMLrd::ExampleGroup = NULL;
DCgroup *XMLrd::PostreqGroup = NULL;
DCgroup *XMLrd::StepxmpGroup = NULL;
DCgroup *XMLrd::StepresultGroup = NULL;


void 
XMLrd::SetTaskHeadGroup(void *ph, long typ)
{
	// called during init for each item in TaskHeads
	if (!ph || !typ)
		return;

	char *head = *((char **) ph);
	if (!head || !*head)
		return;

	TextGroup = new DCgroup;
	SetVarGroupContent(head, "head for task type ", TaskHeadTypes.find(typ));
	if (typ != thsteps)
		PutTextCharCtl(' ');
	*((DCgroup **) TaskHeadGroups.find(typ)) = TextGroup;
	TextGroup = NULL;
}


void 
XMLrd::SetTaskHead(void)
{
	if ((CurrTaskHeadType == thnone)
	 || !TextGroup)
		return;

	DCgroup *phead = *((DCgroup **) TaskHeadGroups.find(CurrTaskHeadType));
	if (phead)
		TextGroup->add(*phead);
	CurrTaskHeadType = thnone;
}

#endif


// runin heads and prefixes


DCnlist *XMLrd::CurrFormatPrefixes = NULL;
char *XMLrd::DefaultNoteType = NULL;

void 
XMLrd::GetElementAttrPrefixes(DCelem *elem, char **form)
{
	// find any attribute format prefixes for this elem
	// concatenate them, and prefix them to the original format

	// later: for parent prefixes, go up extended elem path and check
	// each elem recursively until at top elem orstopped by elem prop

	char *attrsect = NULL;
	char *valsect = NULL;
	char *attrname = NULL;
	char *attrval = NULL;
	long attrcount = 0;
	DCnlist attrlist;
	DCnlist vallist;
	long attrnum = 0;
	char *pxval = NULL;
	long pxlen = 0;
	long pxcount = 0;
	DCnlist pxs;
	long i = 0;
	long n = 0;
	
	//CurrFormatPrefixes = NULL;

	// see if elem has any pxs
	if (IniFile->Section("ElementAttrPrefixes")
	 && IniFile->Find(elem->elemname)) {
		attrsect = IniFile->StrRight();
		if (IniFile->Section(attrsect)) {  // check each attr depth-first
			while (IniFile->NextItem()) {    // get all the attrs now
				attrlist.add(IniFile->StrLeft(), ++attrcount);
				vallist.add(IniFile->StrRight(), attrcount);
			}
			for (i = 1; i <= attrcount; i++) { // check each attr in list
				attrname = attrlist.dequeue();   // name cannot be wildcard
				if ((attrnum = elem->attrnames->getnum(attrname)) != 0) { // match
					valsect = vallist.dequeue();
					attrval = elem->attrvals->find(attrnum);
					if (*valsect == '*')  // attr value is prefix
						pxval = attrval;
					else if (IniFile->Section(valsect)) {
						if (strchr(attrval, ' ')) { // handle multiple values
							char **valarr = NULL;
							long valcnt = MakeSarr(attrval, &valarr);
							if (valcnt
							 && IniFile->FindAny((const char **) valarr, (short) valcnt))
								pxval = IniFile->StrRight();
						}
						else if (IniFile->Find(attrval))
							pxval = IniFile->StrRight();
					}
					if (pxval) {
						if (*pxval == '*')
							pxval = attrval;
						else if (strchr(pxval, ' ')) // handle multiple values
							StripSpaces(pxval);
						pxs.add(pxval, ++pxcount);
						pxlen += strlen(pxval);
					}
				}
				else
					vallist.dequeue();  // keep in sync
			}

			// all attributes listed for elem checked, build prefix
			if (!pxlen
			 && (DefaultNoteType != NULL)
			 && !strcmp(elem->elemname, "note")) {
				pxval = DefaultNoteType;
				pxs.add(pxval, ++pxcount);
				pxlen += strlen(pxval);
			}

			if (pxlen) {  // not empty
				pxval = NewName(pxlen + strlen(*form) + 1);
				for (i = 1; i <= pxcount; i++)
					strcat(pxval, pxs.find(i));
				strcat(pxval, *form);

				// see if pxval exists as a format
				i = 1;
				while ((i <= pxcount)
				 && !FmtFile->CheckFormat(pxval)) {  // not there
					*pxval = '\0';
					for (n = i + 1; n <= pxcount; n++)
						strcat(pxval, pxs.find(n));
					strcat(pxval, *form);
					i++;
				}
				if (i > 1) {  // prefixes trimmed off, add default if any
					CurrFormatPrefixes = new DCnlist();
					for (n = 1; n < i; n++)
						CurrFormatPrefixes->add(pxs.find(n), n);
				}
				if (i <= pxcount)  // still at least one added
					*form = pxval;
			}
		}
	}
}


DCnlist XMLrd::PrefixRuninNames;
DCvlist XMLrd::PrefixRuninGroups;
long XMLrd::PrefixRuninCount = 0;


void 
XMLrd::AddDefFormatPrefixHeads(DCelem *elem)
{
	if (!CurrFormatPrefixes)
		return;

	// add heads for CurrFormatPrefixes to TextGroup
	if (!elem->pxheads)
		elem->pxheads = new DCgroup;

	long i = 0;
	long hdcnt = CurrFormatPrefixes->count();
	char *px = NULL;
	char *hfmtname = NULL;
	char *hfmt = NULL;
	DCgroup *hfgrp = NULL;
	long hfnum = 0;

	for (i = 1; i <= hdcnt; i++) {
		if ((px = CurrFormatPrefixes->find(i)) == NULL)
			break;
		if (IniFile->Section(elem->elinlin ? "InlineFormatPrefixRunins"
		                                   : "BlockFormatPrefixRunins")
		 && IniFile->Find(px)
		 && ((hfmtname = IniFile->StrRight()) != NULL)
		 && ((hfnum = PrefixRuninNames.getitem(hfmtname)) != 0)
		 && ((hfgrp = (DCgroup *) PrefixRuninGroups.find(hfnum)) != NULL))
			elem->pxheads->add(*hfgrp);
	}
}


void 
XMLrd::SetDefFormatPrefixHeads(bool inl)
{
	// called twice at init, false then true
	char *sect = inl ? "InlineFormatPrefixRunins" : "BlockFormatPrefixRunins";
	char *nm = NULL;
	DCgroup *grp = NULL;
	long i = PrefixRuninCount + 1;

	if (IniFile->Section(sect)) {
		while (IniFile->NextItem()
		 && !PrefixRuninNames.getitem(nm = IniFile->StrRight())) {
			PrefixRuninNames.add(nm, ++PrefixRuninCount);
			PrefixRuninGroups.add(grp = new DCgroup, PrefixRuninCount);
		}
		for (; i <= PrefixRuninCount; i++) {
			nm = PrefixRuninNames.find(i);
			grp = (DCgroup *) PrefixRuninGroups.find(i);
			WriteRuninHead(grp, nm);
		}
	}
}


void 
XMLrd::WriteFormatRunin(DCelem *elem, char *fmname)
{
	if (!FmtFile || !fmname)
		return;

	if (!elem->pxheads)
		elem->pxheads = new DCgroup;
	AddDefFormatPrefixHeads(elem);
	CurrFormatPrefixes = NULL;

	char *nm = FmtFile->GetRuninFmtName(fmname);
	if (nm)
		WriteRuninHead(elem->pxheads, nm);

	TextGroup->add(*(elem->pxheads));
}


void 
XMLrd::WriteRuninHead(DCgroup *grp, char *nm)
{
	if (!grp || !nm)
		return;

	char *fcp = SubFormatsFile->GetRuninFmtForm(nm);
	if (!fcp)
		return;

	WriteSubformatHead(grp, nm, fcp, "RuninHeadText", "Head");
}


void 
XMLrd::WriteIndexSeeItem(DCgroup *grp, char *nm)
{
	if (!grp || !nm)
		return;

	char *fcp = SubFormatsFile->GetIndexFmtForm(nm);
	if (!fcp)
		return;

	WriteSubformatHead(grp, nm, fcp, "IndexSeeText", "Index");
}


void 
XMLrd::WriteMarkItem(DCgroup *grp, char *nm)
{
	if (!grp || !nm)
		return;

	char *fcp = SubFormatsFile->GetMarkFmtForm(nm);
	if (!fcp)
		return;

	WriteSubformatHead(grp, nm, fcp, "MarkText", "Mark");
}


bool XMLrd::WritingSubformatHead = false;

void 
XMLrd::WriteSubformatHead(DCgroup *grp, char *nm, char *fcp,
                          char *langsect, char *formtype)
{

	DCgroup *lasttext = TextGroup;
	TextGroup = grp;

	char *name = NULL;
	char *ntok = NULL;
	DCctl *cp = NULL;
	char formname[FormNameMax] = "";
	char *fnend = NULL;
	bool typo = false;
	DCnlist *nestlist = NULL;
	DCilist *typolist = NULL;
	long fnest = 0;
	WritingSubformatHead = true;

	while ((ntok = strchr(fcp, '<')) != NULL) {
		if (ntok > fcp) {
			if ((ntok - fcp) > 1) {
				cp = new DCctl(etext, 2, 10, 1);
				cp->ex(fcp, ntok - fcp);
			}
			else
				cp = new DCctl(imnem, 2, 10, 1, (unc) *fcp);
			TextGroup->add(*cp);
		}
		fcp = ntok;
		while (*fcp
		 && (*fcp != '>'))
			fcp++;
		if (!*fcp)
			LogEvent(logwarn, 1, formtype, " format tag missing ending '>': ", ntok);
		ntok++;
		if (MatchToken(ntok, fcp, "tab", 3)) {  // tab token
			cp = new DCctl(ilong, 2, 41, 1, AnumTabWidth * 100);
			TextGroup->add(*cp);
		}
		else if (MatchToken(ntok, fcp, "spc", 3)) {
			cp = new DCctl(imnem, 2, 10, 1, (unl) 160);
			TextGroup->add(*cp);
		}
		else if (MatchToken(ntok, fcp, "br", 2)) {
			cp = new DCctl(imnem, 2, 11, 10, 1);
			TextGroup->add(*cp);
		}
		else if (MatchToken(ntok, fcp, "name", 4)) {  // get name from lang
			if (IniFile->Section(langsect)
			 && IniFile->Find(nm))
				name = IniFile->StrRight();
			else if (LangFile->Section(langsect)
			 && LangFile->Find(nm))
				name = LangFile->StrRight();
			if (name) {
				cp = new DCctl(etext, 2, 10, 1);
				cp->ex(name, strlen(name));
				TextGroup->add(*cp);
			}
			else
				LogEvent(logwarn, 1, formtype, " format <name/> undefined for ", nm);
		}
		else if ((tolower(*ntok) == 'u')
		 && (*(ntok + 1) == '+')) {  // Unicode code point
			char *nend = NULL;
			unl uch = strtoul(ntok + 2, &nend, 16);
			if ((nend != (fcp - 1))
			 || (*(fcp - 1) != '/'))
				LogEvent(logwarn, 1, "Unicode char error in form for ", nm);
			else {
				cp = new DCctl(imnem, 2, 10, 1, uch);
				TextGroup->add(*cp);
			}
		}
		else {  // treat anything else as an inline format
			if (*ntok != '/') {  // start the named format
				if (fnest) { // nest format
					if (!nestlist)
						nestlist = new DCnlist();
					nestlist->addnew(formname, fnest);
					if (!typolist)
						typolist = new DCilist();
					typolist->add(typo, fnest);
				}
				fnest++;
				strncpy(formname, ntok, fcp - ntok);
				formname[fcp - ntok] = '\0';
				if (SetTypographic(formname, true))
					typo = true;
				else
					SetFormat(formname, true); 
			}
			else if (!strnicmp(ntok + 1, formname, (fcp - ntok) - 1)) {
				// end any current format
				if (typo) {
					SetTypographic(formname, false);
					typo = false;
				}
				else
					SetFormat(NULL, true);
				*formname = '\0';
				if (--fnest) {
					strcpy(formname, nestlist->find(fnest));
					typo = typolist->find(fnest);
				}
			}
			else
				LogEvent(logwarn, 1, formtype, " <format> token error: ", ntok);
		}
		if (*fcp)  // move after ending bracket
			ntok = ++fcp;
	}
	if (*fcp) { // more left, finish it
		if (strlen(fcp) > 1) {
			cp = new DCctl(etext, 2, 10, 1);
			cp->ex(fcp, strlen(fcp));
		}
		else
			cp = new DCctl(imnem, 2, 10, 1, (unc) *fcp);
		TextGroup->add(*cp);
	}
	while (fnest) {
		if (typo) {
			SetTypographic(formname, false);
			typo = false;
		}
		else if (*formname != '\0')
			SetFormat(NULL, true);
		if (--fnest) {
			strcpy(formname, nestlist->find(fnest));
			typo = typolist->find(fnest);
		}
	}

	TextGroup = lasttext;
	WritingSubformatHead = false;
}


long
XMLrd::MakeSarr(char *str, char ***psarr)
{
	long cnt = 0;
	long i = 0;
	char **sarr = NULL;
	char *spt = str;
	char *spc = str;
	DCnlist sar;

	while (*str) {
		while (*str == ' ')
			str++;
		spt = spc = str;
		while (*spc && (*spc != ' '))
			spc++;
		if (spc > spt)
			sar.add(NewName(spt, spc - spt), ++cnt);
		str = spt = spc;
	}
	if (cnt) {
		sarr = (char **) new char *[cnt];
		for (i = 1; i <= cnt; i++)
			sarr[i] = sar.dequeue();	
	}
	*psarr = sarr;
	return cnt;
}


void 
XMLrd::StripSpaces(char *str)
{
	// remove spaces in place
	char *dest = str;
	while (*str) {
		while (*str == ' ')
			str++;
		*dest++ = *str++;
	}
	*dest = '\0';
}


DCnlist XMLrd::TaskHeadTypes;
npair XMLrd::TaskHeadTypesInit[] = {
 { thsteps, "steps" },
 { thprereq, "prereq" },
 { thcontext, "context" },
 { thexample, "example" },
 { thresult, "result" },
 { thpostreq, "postreq" },
 { thstepxmp, "stepxmp" },
 { thstepresult, "stepresult" },
 NULL
};

thtyp XMLrd::CurrTaskHeadType = thnone;
char *XMLrd::StepsHeadFormat = "Steps";
DCelem *XMLrd::CurrTaskHeadElem = NULL;
long XMLrd::CurrTaskHeadDepth = 0;
bool XMLrd::CurrTaskHeadUsed = false;


void 
XMLrd::SetStepsPara(DCelem *einfo)
{
	char *form = NULL;

	if (CurrTaskHeadUsed)
		return;

	if (CurrTaskHeadType == thsteps)
		form = StepsHeadFormat;
	else if (CurrTaskHeadElem) {
		if (!CurrTaskHeadElem->elpath)
			CurrTaskHeadElem->elpath = GetElemPath(CurrTaskHeadDepth);
		form = GetElemForm(CurrTaskHeadElem, CurrTaskHeadElem->outclass,
			                 CurrTaskHeadElem->elemname, false);
	}

	//CurrTaskHeadType = thnone;

	if (!form)
		return;
	
	char *head = FmtFile->GetRuninFmtName(form);

	if (!head)
		return;

	if (einfo) {
		char *eform = NULL;
		char *ehead = NULL;

		if (!einfo->elpath)
			einfo->elpath = GetElemPath(einfo->elemdepth);
		eform = GetElemForm(einfo, einfo->outclass,
			                 einfo->elemname, false);
		if (eform
		 && (!stricmp(eform, form)      // uses same format
		  || (((ehead = FmtFile->GetRuninFmtName(eform)) != NULL)
		   && !stricmp(ehead, head)))) { // or uses same runin
			CurrTaskHeadUsed = true;
			return;                       // don't repeat the head
		}
	}

	StartPara();
	SetFormat(form);
	WriteRuninHead(TextGroup, head);
	EndPara();
	CurrTaskHeadUsed = true;
}




// end of drxmlpi.cpp

