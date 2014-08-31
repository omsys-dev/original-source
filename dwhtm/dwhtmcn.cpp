 
// dwhtmcn.cpp is the code for dcl Microsoft HTM condition processing
// Copyright (c) 2009 by Omni Systems, Inc.  All rights reserved.

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




// conditional text

long HTMwr::CurrCond = 0;
long HTMwr::ConflictCond = 0;
DClist HTMwr::HiddenConds;
DCnlist HTMwr::CondNames;
DCnlist HTMwr::CondAttrs;
char *HTMwr::CurrCondName;
bool HTMwr::ShowHiddenCond = false;
bool HTMwr::ShowTbRow = false;
bool HTMwr::HideTbRow = false;
bool HTMwr::EmptyTable = false;
DClist HTMwr::EmptyTableList;


void
HTMwr::ProcTbCondUse(HTMctl *cp)
{
	if ((ShowHiddenCond == false)
	 && (Row != NULL)) { // only used to suppress table rows
		if (HiddenConds.find(cp->dat()))
			HideTbRow = true;
		else               // handle multiple conditions properly
			ShowTbRow = true;
	}
}



// character range marking

#if 0

[CharacterRangeClasses]
; starting U+ code point (four or five hex digits) = class name,
; - (exclude from all classes), or * (allow in any class).  Class
; applies to the character specified and all following to the next
; setting. Any text after the first word or symbol is a comment.
; The initial state is * (for allow in any); the last should be -.
;
; for English and Euro languages in Japanese:
0021=latin  common symbols
0030=*      digits
003A=latin  alpha, some symbols
00A5=*      Yen sign
00A6=latin  latin-1, diacritics
0342=greek  greek diacritics
0346=latin  latin diacritics
0374=greek  greek letters
03E2=-      ethiopic and many more
1E00=latin  latin extended
1F00=greek  greek extended
2000=*      lots of punctuation
2E80=-      rest of the world
;
; or the same without Greek:
0021=latin  common symbols
0030=*      digits
003A=latin  alpha, some symbols
00A5=*      Yen sign
00A6=latin  latin-1, diacritics, more non-asian
2000=*      lots of punctuation
2E80=-      rest of the world
;
; or for Russian in English:
21=-
400=russian
514=-
2000=*
3000=-


#endif

// create a condition for each class and a class attribute
// within scan *and* write, check each char against the list:
//  if in class group, and that is not the current class,
//    start the class; if it is the current class, continue
//  if - and in class group, end class group, else continue
//  if * continue, whether in class group or not
// so to start, call ProcCondStart in scan, CondStart in write,
// and to end, call ProcCondEnd in scan, CondEnd in write.
// Would be nice to get compare done in scan without doing it
// again in write, which suggests maybe this should be in drmif,
// where we could insert cond ctls and perhaps divide up text ctls..

bool HTMwr::UseCharRangeClasses = false;  // in [CSS] section
unl *HTMwr::RangeStarts = NULL;
char **HTMwr::RangeClasses = NULL;
rangetype *HTMwr::RangeTypes = NULL;
unl *HTMwr::RangeConds = NULL;
long HTMwr::CurrCharRange = 0;
long HTMwr::RangeCount = 0;
long HTMwr::FirstRangeCond = 0;
long HTMwr::LastRangeCond = 0;


void
HTMwr::SetCharRangeConds(void)
{
	// called at end of resources, after all conditions in DCL set
	// add new conds after CurrCond, with class name as cond name

	if (!UseCharRangeClasses
	 || !IniFile->Section("CharacterRangeClasses")) {
		UseCharRangeClasses = false;
		return;
	}

	RangeCount = IniFile->GetItemCount();
	if (!RangeCount) {
		UseCharRangeClasses = false;
		return;
	}

	RangeStarts = (unl *) new long[RangeCount];
	RangeClasses = (char **) new char[RangeCount * sizeof(char *)];
	RangeTypes = (rangetype *) new char[RangeCount * sizeof(rangetype)];
	RangeConds = (unl *) new long[RangeCount];
	char *num = NULL;
	char *str = NULL;
	char *spc = NULL;
	long cval = 0;

	for (long i = 0; i < RangeCount; i++) {
		if (!IniFile->NextItem())
			break;
		num = IniFile->StrLeft();
		RangeStarts[i] = strtoul(num, NULL, 16);
		str = IniFile->StrRight();
		if ((spc = strchr(str, ' ')) != NULL)
			*spc = '\0';  // remove comments
		if (*str == '-')
			RangeTypes[i] = RTnone;
		else if (*str == '*')
			RangeTypes[i] = RTany;
		else if (*str) {
			RangeTypes[i] = RTclass;
			RangeClasses[i] = NewName(str);
			// set cond name and attrs
			if ((cval = CondNames.getitem(str)) < FirstRangeCond) {  // already used for cond
				LogEvent(logwarn, 1, "Re-use of condition name for CharacterRangeClasses: ",
					                    str);
				if (!UseConditionalFlagging) {  // use it anyway
					StoreRangeCond(str);
					RangeConds[i] = LastRangeCond;
				}
			}
			else if (cval == 0) {
				StoreRangeCond(str);
				RangeConds[i] = LastRangeCond;
			}
			else // already stored, mark usage
				RangeConds[i] = cval;
		}
		else {  // error, empty
			RangeTypes[i] = RTnone;
			LogEvent(logwarn, 1, "Undefined range in CharacterRangeClasses for ",
				                   num);
		}
		DeleteName(str);
		DeleteName(num);
	}

	if (RangeTypes[RangeCount - 1] != RTnone)
		LogEvent(logwarn, 2, "Open top char range in CharacterRangeClasses");
}


void 
HTMwr::StoreRangeCond(char *name)
{
	char *cname = NULL;

	if (!FirstRangeCond)
		FirstRangeCond = LastRangeCond = CurrCond + 1;
	else
		LastRangeCond++;

	CondNames.add(name, LastRangeCond);

	cname = NewName("class=\"", strlen(name) + 9);
	strcat(cname, name);
	strcat(cname,"\"");
	CondAttrs.add(cname, LastRangeCond);
}


// during scan, add ctls for cond start and end, before
// current ctl gets added, with GroupStack[DCLlevel]->add(*cp);
// can also split etext ctls then, putting out first chunk,
// then cond ctl, repeat as needed, adjust the original ctl
// to contain only the last bit.

bool 
HTMwr::ProcCharRangeText(HTMctl *cp)
{
	if (!UseCharRangeClasses)
		return false;

	uns range = 0;
	char *str = NULL;
	char *cpt = NULL;
	char *cpn = NULL;
	bool retval = false;

	if (cp->dt() == etext) {
		str = cpt = cpn = NewName((char *) cp->ex(), cp->siz());
		for ( ; *cpt ; cpt++) {
			if ((range = GetCharRange(*cpt)) != CurrCharRange) {
				if (cpt > cpn) {
					AddTextCtl(cpn, cpt - cpn);
					cpn = cpt;
				}
				AddCharRangeCond(range);
			}
		}
		if (cpn > str) {
			AddTextCtl(cpn, cpt - cpn);
			retval = true;  // text ctl replaced
		}
		DeleteName(str);
	}
	else {  // imnem
		unl ch = cp->siz();
		if ((range = GetCharRange(ch)) != CurrCharRange)
			AddCharRangeCond(range);
	}
	return retval;  // text ctl not replaced, use it
}


uns 
HTMwr::GetCharRange(unl ch)
{
	// return 0 if RTnone (range should end),
	// CurrCharRange if RTany,
	// or range cond if RTclass
	rangetype rt = RTany;
	uns cond = 0;

	for (long i = 0; i < RangeCount; i++) {
		if (ch < RangeStarts[i])
			break;  // range identified
		rt = RangeTypes[i];
		if (rt == RTclass)
			cond = (uns) RangeConds[i];
	}

	switch (rt){
		case RTnone:
			return 0;

		case RTany:
			return (uns) CurrCharRange;

		case RTclass:
			return cond;

		default:
			break;
	}
	return (uns) CurrCharRange;
}


void 
HTMwr::AddTextCtl(char *str, long len)
{
	if (!len)
		return;

	HTMctl *cp = new HTMctl((len > 1) ? etext : imnem, 2, 10, 1);
	if (len > 1)
		cp->ex(str, len);
	else
		cp->siz(*str);
	FindHTM(cp);
	GroupStack[DCLlevel]->add(*cp);
}


void 
HTMwr::AddCharRangeCond(uns cond)
{
	HTMctl *cp = NULL;

	if (CurrCharRange) {  // turn off CurrCharRange
		cp = new HTMctl(imnem, 2, 63, (uns) CurrCharRange);
		FindHTM(cp);
		GroupStack[DCLlevel]->add(*cp);
		ProcCondEnd(CurrCharRange);
	}
	if (cond) {
		cp = new HTMctl(imnem, 2, 62, cond);
		FindHTM(cp);
		GroupStack[DCLlevel]->add(*cp);
		ProcCondStart(cond);
	}
	CurrCharRange = cond;
}



// condition indicator properties

bool HTMwr::InCondDef = false;
condprop *HTMwr::CurrCondProps = NULL;
DCvlist HTMwr::CondProps;


void
HTMwr::StartCondDef(uns cond)
{
	InCondDef = true;
	CurrCond = cond;
	CurrCondProps = new condprop;

#if CONDSPANS
	if (MaxCond < CurrCond)
		MaxCond = CurrCond;
#endif
}


void
HTMwr::ProcCondName(HTMctl *cp)
{
	char *cname = NULL;

	CurrCondName = NewName((char *) cp->ex(), cp->siz());
	CondNames.add(CurrCondName, CurrCond);

	cname = ProcCondAttr(CurrCondName);

	if (cname)
		CondAttrs.add(cname, CurrCond);
	if (UseConditionalFlagging
	 && strstr(cname, "conflict"))
		ConflictCond = CurrCond;
}


char *
HTMwr::ProcCondAttr(char *name)
{
	char *cname = NULL;

	if (UseConditionalFlagging) {
		cname = NewName("class=\"", strlen(name) + 9);
		strcat(cname, name);
		strcat(cname,"\"");
	}
	else if (IniFile
	 && IniFile->Section("ConditionAttributes")
	 && IniFile->Find(name))
		cname = IniFile->StrRight();

	return cname;
}


void
HTMwr::SetCondProps(HTMctl *cp)
{
	unc bar = 0;
	unc line = 0;

  if ((cp->rp == NULL)
	 || !CurrCondProps)
    return;

  switch ((cp->rp)->write_op) {
    case ignore:
      break;
    case skip:
      break;

    case putbold:
      CurrCondProps->bold = (cp->siz() > 2) ? true : false;
      break;
    case putital:
      CurrCondProps->ital = cp->siz() ? true : false;
      break;
    case putrevb:
			if ((bar = (unc) cp->siz()) == 3)
				CurrCondProps->chbar = true;
			else if (bar == 2)
	      CurrCondProps->strike = true;
      break;
    case putline:
      if ((line = (unc) cp->siz()) == 3)
				CurrCondProps->overline = true;
			else if (line > 0)
				CurrCondProps->underline = true;
      break;

    default:
      break;
	}
}


char *HTMwr::CondFlagClass = "CondFlag";

char *
HTMwr::StoreCondFlag(char *image, char *alt)
{
	short len = 0;
	if (image) {
		CorrectGraphFilePath(&image);
		image = FixGrFileName(image);
		len = strlen(image) + 13;
		if (alt)
			len += strlen(alt) + 7;
	}
	else if (alt)
		len = strlen(alt) + strlen(CondFlagClass) + 23;

	char *buf = NewName(len);

	if (image) {
		strcpy(buf, "<img src=\"");
		strcat(buf, image);
		strcat(buf, "\"");
		if (alt) {
			strcat(buf, " alt=\"");
			strcat(buf, alt);
			strcat(buf, "\"");
		}
		strcat(buf, ">");
	}
	else if (alt) {
		strcpy(buf, "<span class=\"");
		strcat(buf, CondFlagClass);
		strcat(buf, "\">");
		strcat(buf, alt);
		strcat(buf, "</span>");
	}
	return buf;
}


void
HTMwr::ProcCondDef(void)
{
	char *ftxt = NULL;

	if (CurrCondProps->startalt || CurrCondProps->startimage) {
		ftxt = StoreCondFlag(CurrCondProps->startimage, CurrCondProps->startalt);
		StartCondFlags.add(ftxt, CurrCond);
	}
	if (CurrCondProps->endalt || CurrCondProps->endimage) {
		ftxt = StoreCondFlag(CurrCondProps->endimage, CurrCondProps->endalt);
		EndCondFlags.add(ftxt, CurrCond);
	}
	CondProps.add(CurrCondProps, CurrCond);
	CurrCondProps = NULL;
	CurrCondName = NULL;
	InCondDef = false;
}


// conditional flagging

bool HTMwr::UseConditionalFlagging = false;
char *HTMwr::CSSFlagsFile = NULL;
char *HTMwr::FlagsFullPath = NULL;
bool HTMwr::WriteFlagsFile = true;
bool HTMwr::ReferenceFlagsFile = true;
bool HTMwr::AppendFlagsFile = false;
DCwfile *HTMwr::FlagsFile = NULL;


void
HTMwr::WriteFlagsCSSFile(void)
{
	if (!UseConditionalFlagging
	 || !WriteFlagsFile)
		return;

	if (!AppendFlagsFile
	 && !CSSFlagsFile)
		return;

	if (AppendFlagsFile
	 && CssFile)
		FlagsFile = CssFile;
	else {
		FlagsFile = new DCwfile(CSSFlagsFile, ftxt);
		if (FlagsFile->FErr() != fok) {
			WriteFlagsFile = false;  // failed write, just use
			return;
		}
	}

	if (GeneratorTag == 'C') {
		FlagsFile->putStr("/* ");
		FlagsFile->putStr(CSSFlagsFile);
		FlagsFile->putStr(" produced by Mif2Go ");
		FlagsFile->putStr(FilterVer);
		FlagsFile->putStr(" */");
		FlagsFile->putCr();
	}

	CondProps.writeallval(FlagsFile, WriteOneFlagRule);

	if (ConflictCond) {
		long cc = ConflictCond;
		ConflictCond = 0;
		WriteOneFlagRule(FlagsFile, CondProps.find(cc), cc);
	}

	if (FlagsFile != CssFile)
		FlagsFile->CloseFile();
}


void
HTMwr::WriteOneFlagRule(DCfile *df, void *vp, long cond)
{
	DCwfile *ff = (DCwfile *) df;
	char *cname = CondNames.find(cond);
	condprop *curr = (condprop *) vp;

	if (!ff || !cname || !curr)
		return;
	if (ConflictCond
	 && (cond == ConflictCond))
		return;

	ff->putCr();
	ff->putChar('.');
	ff->putStr(cname);
	ff->putChar(' ');
	ff->putChar('{');
	ff->putCr();

	if (curr->ital) {
		ff->putStr(" font-style: ");
		ff->putStr("italic");
		ff->putChar(';');
		ff->putCr();
	}

	if (curr->bold) {
		ff->putStr(" font-weight: ");
		ff->putStr("bold");
		ff->putChar(';');
		ff->putCr();
	}

	if (curr->underline
	 || curr->overline
	 || curr->strike) {
		ff->putStr(" text-decoration:");
		if (curr->underline)
			ff->putStr(" underline");
		if (curr->overline)
			ff->putStr(" overline");
		if (curr->strike)
			ff->putStr(" line-through");
		ff->putChar(';');
		ff->putCr();
	}

	if (curr->fcolor > 2) {
		ff->putStr(" color: ");
		WriteCssColor(ff, (curr->fcolor) < 9 ? ColorValue((uns) curr->fcolor + 1) :
                                           curr->fcolor );
		ff->putChar(';');
		ff->putCr();
	}

	if (curr->bcolor > 2) {
		ff->putStr(" background-color: ");
		WriteCssColor(ff, (curr->bcolor) < 9 ? ColorValue((uns) curr->bcolor + 1) :
                                           curr->bcolor );
		ff->putChar(';');
		ff->putCr();
	}

	ff->putStr(" }");
	ff->putCr();
	ff->putCr();
}


void
HTMwr::CheckCondConflict(HTMwfile *wf, long cnt, DClist *conds)
{
	if (!UseConditionalFlagging
	 || (cnt < 2))
		return;

	bool colcon = false;
	long fcol = 0;
	long bcol = 0;

	bool lincon = false;
	bool under = false;
	bool nounder = false;
	bool over = false;
	bool noover = false;
	bool strike = false;
	bool nostrike = false;

	long i = 0;

	// compare all cnt conds in list for conflicts
	// ignore ital/bold conflicts
	condprop **cparr = (condprop **) new char[cnt * sizeof(condprop *)];
	for (i = 0; i < cnt; i++)
		cparr[i] = (condprop *) CondProps.find(conds->number(i + 1));

	for (i = 0; i < cnt; i++) {
		if (cparr[i]->fcolor) {
			if (!fcol)
				fcol = cparr[i]->fcolor;
			else if (fcol != cparr[i]->fcolor)
				colcon = true;
		}
		if (cparr[i]->bcolor) {
			if (!bcol)
				bcol = cparr[i]->bcolor;
			else if (bcol != cparr[i]->bcolor)
				colcon = true;
		}
		if (cparr[i]->underline)
			under = true;
		else
			nounder = true;
		if (cparr[i]->overline)
			over = true;
		else
			noover = true;
		if (cparr[i]->strike)
			strike = true;
		else
			nostrike = true;
	}
	if ((fcol != 0)
	 && (fcol == bcol))
		colcon = true;
	if ((under && nounder)
	 || (over && noover)
	 || (strike && nostrike))
		lincon = true;


	// for color conflicts, add "conflict" class
	if (colcon)
		wf->putAttrStr(CondAttrs.find(CondNames("conflict")));

	// for lining conflicts, create new class for combo
	//  and add it to CondNames, CondProps, and CondAttrs
	short len = 0;
	char *nm = NULL;
	char *nnm = NULL;
	long val = 0;
	condprop *prop = NULL;

	if (lincon) {
		for (i = 0; i < cnt; i++) {
			if ((nm = CondNames.find(conds->number(i + 1))) != NULL)
				len += strlen(nm);
		}
		nnm = NewName(len);
		for (i = 0; i < cnt; i++) {
			if ((nm = CondNames.find(conds->number(i + 1))) != NULL)
				strcat(nnm, nm);
		}
		if ((val = CondNames(nnm)) == 0) {  // create new cond
			val = CondNames.count() + 1;
			CondNames.add(nnm, val);
			prop = new condprop;
			prop->underline = under;
			prop->overline = over;
			prop->strike = strike;
			CondProps.add(prop, val);
			CondAttrs.add(ProcCondAttr(nnm), val);
		}
		wf->putAttrStr(CondAttrs.find(val));
	}

	delete [] cparr;
}




// conditional attributes

// For DITA, if a cond is in effect for:
//  an entire para, it sets attrs for that para.
//  an entire char element, it sets attrs for that element.
//  part of a para, it is put in a <ph> element with attrs.
//  more than a para, the attrs go with the parents too
//   if the entire parent has the condition(s).
// So, at start of para set list of all conds in effect.
// Also at start of each parent, including interpolated ones.
// At start of each char elem create a similar list.
// If any cond ends before the last text, remove it from that
//  para and/or char list and set ph elems for it that do not
//  overlap other char elements.
// For parent conds to be known, parent start and end must be
//  known before start is written.  But these are not known until
//  write phase.  They cannot be known in scan because markers
//  that affect parent start/end may appear anywhere in the para.
//  However, by end of para they should be known, and can be
//  stored in the same way as the attr lists.

DClist *HTMwr::CurrConds = NULL;
DClist *HTMwr::CondCharChanged = NULL;
DClist *HTMwr::ImageConds = NULL;
DClist *HTMwr::TableConds = NULL;
DClist *HTMwr::TableStartConds = NULL;
DClist *HTMwr::TableEndConds = NULL;
DClist *HTMwr::FtnConds = NULL;
DClist *HTMwr::LinkConds = NULL;
DClist *HTMwr::IndexConds = NULL;
DClist *HTMwr::ParaConds = NULL;
DClist *HTMwr::CharConds = NULL;
DClist *HTMwr::RowConds = NULL;
DClist *HTMwr::TextConds = NULL;
DCvlist HTMwr::CondImages;
DCvlist HTMwr::CondTables;
DCvlist HTMwr::CondFtns;
DCvlist HTMwr::CondLinks;
DCvlist HTMwr::CondIndexes;
DCvlist HTMwr::CondParas;
DCvlist HTMwr::CondRows;
DCvlist HTMwr::CondChars;
bool HTMwr::CondChanged = false;
bool HTMwr::CondCharPending = false;
bool HTMwr::ChangedCharCond = false;
bool HTMwr::InCharCond = false;
bool HTMwr::InTextCond = false;
bool HTMwr::UseParaCondClasses = true;
bool HTMwr::CharFormInTextCond = false;
bool HTMwr::LinkInTextCond = false;
char *HTMwr::ConditionCharTag = "span";
char *HTMwr::CurrCondClass = NULL;


void
HTMwr::ProcCondStart(long cnum)
{
	// add to CurrConds; recompute FullCondProp 
	if (CondAttrs.find(cnum)) { // only care if attrs set
		if (!CurrConds)
			CurrConds = new DClist;
		CurrConds->add(cnum);
	}
	CondChanged = true;
	if (InCharCond || CharRefNum) {
		if (!CondCharChanged)
			CondCharChanged = new DClist;
		CondCharChanged->add(CharRefNum);
	}
}

void
HTMwr::ProcCondEnd(long cnum)
{
	if (!CondAttrs.find(cnum))  // only care if attrs set
		return;

	// delete from CurrConds; recompute FullCondProp 
	if (CurrConds)
		CurrConds->remove(cnum);
	if (ParaConds)  // not in effect for full para
		ParaConds->remove(cnum);
	if (FtnConds)   // not in effect for full footnote
		FtnConds->remove(cnum);
	if (CharConds)   // not in effect for full char format
		CharConds->remove(cnum);
	if (TextConds)   // not in effect for full char format
		TextConds->remove(cnum);
	if (InCharCond || CharRefNum)
		CondCharChanged->add(CharRefNum);

	CondChanged = true;
}


void
HTMwr::ProcCondImage(void)
{
	// called at image anchor
	if (!CurrConds)
		return;

	long ccnt = CurrConds->count();
	long i = 1;

	if (ccnt == 0)
		return;

	ImageConds = new DClist;

	for (i = 1; i <= ccnt; i++)
		ImageConds->add(CurrConds->number(i));

	CondImages.add(ImageConds, ImageRefNum);

	ImageConds = NULL;
}


void
HTMwr::ProcCondTable(void)
{
	// called at table anchor
	if (!CurrConds)
		return;

	long ccnt = CurrConds->count();
	long i = 1;

	if (ccnt == 0)
		return;

	TableConds = new DClist;

	for (i = 1; i <= ccnt; i++)
		TableConds->add(CurrConds->number(i));

	ParaConds = CharConds = CondCharChanged = NULL;

	// add to CondTables when ID is known
}


void
HTMwr::ProcCondFtn(long ftnnum)
{
	// called at footnote start
	if (!CurrConds)
		return;

	long ccnt = CurrConds->count();
	long i = 1;

	if (ccnt == 0)
		return;

	FtnConds = new DClist;

	for (i = 1; i <= ccnt; i++)
		FtnConds->add(CurrConds->number(i));

	CondFtns.add(FtnConds, ftnnum);
}


void
HTMwr::ProcCondLink(long linknum)
{
	// called at Link anchor
	if (!CurrConds)
		return;

	long ccnt = CurrConds->count();
	long i = 1;

	if (ccnt == 0)
		return;

	LinkConds = new DClist;

	for (i = 1; i <= ccnt; i++)
		LinkConds->add(CurrConds->number(i));

	CondLinks.add(LinkConds, linknum);

	LinkConds = NULL;
}


void
HTMwr::ProcCondIndex(long indexnum)
{
	// called at Index entry start
	if (!CurrConds)
		return;

	long ccnt = CurrConds->count();
	long i = 1;

	if (ccnt == 0)
		return;

	IndexConds = new DClist;

	for (i = 1; i <= ccnt; i++)
		IndexConds->add(CurrConds->number(i));

	CondIndexes.add(IndexConds, indexnum);

	IndexConds = NULL;
}


void
HTMwr::ProcCondParaStart(void)
{
	// called at first para text, after cond markers
	if (!CurrConds)
		return;

	long ccnt = CurrConds->count();
	long i = 1;

	if (ccnt == 0)
		return;

	ParaConds = new DClist;

	for (i = 1; i <= ccnt; i++)
		ParaConds->add(CurrConds->number(i));

	if (RowConds  // don't duplicate row conds in paras
	 && (ccnt == RowConds->count())
	 && ParaConds->fullmatch(RowConds)) {
		for (i = 1; i <= ccnt; i++)
			ParaConds->remove(RowConds->number(i));
	}

	if (!ParaConds->count())
		ParaConds = NULL;

	CondChanged = false;
}


void
HTMwr::ProcCondParaEnd(void)
{
	// called at end of para text
	if (!ParaConds)
		return;

	// ParaConds contains all in effect for full para
	if (ParaConds->count())
		CondParas.add(ParaConds, ParaRefNum);
	else  // empty
		delete ParaConds;
	ParaConds = NULL;

	// end of para ends char formats too
	if (InCharCond)
		ProcCondCharEnd();
}


void
HTMwr::ProcCondRowUse(long cnum)
{
	if (!RowConds)
		RowConds = new DClist;

	RowConds->add(cnum);
}


#if 0
void
HTMwr::ProcCondRowStart(void)
{
	// called at Row props, after cond ctls
	if (!CurrConds)
		return;

	long ccnt = CurrConds->count();

	if (ccnt == 0)
		return;

	RowConds = new DClist;

	for (long i = 1; i <= ccnt; i++)
		RowConds->add(CurrConds->number(i));

	CondChanged = false;
}
#endif


void
HTMwr::ProcCondRowEnd(void)
{
	// called at end of Row
	if (!RowConds)
		return;

	// RowConds contains all in effect for full Row
	if (RowConds->count())
		CondRows.add(RowConds, RowCpID);
	else  // empty
		delete RowConds;
	RowConds = NULL;

	// end of Row ends char formats too
	if (InCharCond)
		ProcCondCharEnd();
}


void
HTMwr::ProcCondCharStart(void)
{
	// called at start of first text after char format
	if (!CurrConds)
		return;

	long ccnt = CurrConds->count();
	if (!ccnt)
		return;

	CharConds = new DClist;

	for (long i = 1; i <= ccnt; i++)
		CharConds->add(CurrConds->number(i));

	// need to remove ParaConds during write phase
	// because ones in effect now may not be later

	InCharCond = true;
}


void
HTMwr::ProcCondCharEnd(void)
{
	// called at end of char format, or start of next one
	// before setting CharRefNum for the next one

	// CharConds contains all in effect for full char format
	if (!CharConds)
		return;

	if (CharConds->count())
		CondChars.add(CharConds, CharRefNum);
	else  // empty
		delete CharConds;

	CharConds = NULL;
	InCharCond = false;
}


// write phase


void
HTMwr::CondStart(long cnum)
{
	if (!CondAttrs.find(cnum))  // only care if attrs set
		return;

	if (!CurrConds)
		CurrConds = new DClist;

	CurrConds->add(cnum);
	CondChanged = true;

	char *flag = NULL;
	if (((flag = StartCondFlags.find(cnum)) != NULL)
	 && !CurrCondFlags.find(cnum)) {
		CurrCondFlags.add(cnum);
		PendStartCondFlags.add(flag, cnum);
		CondsEffective = true;
	}
}


void
HTMwr::CondEnd(long cnum)
{
	if (!CondAttrs.find(cnum))  // only care if attrs set
		return;

	CurrConds->remove(cnum);
	CondChanged = true;

	char *flag = NULL;
	if (((flag = EndCondFlags.find(cnum)) != NULL)
	 && CurrCondFlags.find(cnum)) {
		CurrCondFlags.remove(cnum);
		PendEndCondFlags.add(flag, cnum);
	}
}


void
HTMwr::StartCondImage(HTMwfile *wf)
{
	ImageConds = (DClist *) CondImages.find(ImageRefNum);
	if (!ImageConds)
		return;

	long ccnt = ImageConds->count();
	wf->SetAttrAdd(true);
	for (long i = 1; i <= ccnt; i++)
		wf->putAttrStr(CondAttrs.find(ImageConds->number(i)));
	if (ccnt > 1)
		CheckCondConflict(wf, ccnt, ImageConds);
}


void
HTMwr::SetCondTable(HTMwfile *wf, long tablenum)
{
	TableConds = (DClist *) CondTables.find(tablenum ? tablenum : (long) Tbl->tblid);
	//SaveCondState();
	ParaConds = CharConds = NULL;

	if (!TableEndConds)
		TableEndConds = CurrConds ? new DClist(CurrConds) : NULL;
	CurrConds = TableStartConds;

	if (!TableConds) {
		if (!StripTable)
			CurrConds = TextConds = NULL;
		return;
	}

	long ccnt = TableConds->count();
	for (long i = 1; i <= ccnt; i++) {
		CurrConds->add(TableConds->number(i));
		TextConds->add(TableConds->number(i));
	}

	CondChanged = true;  // to trigger recheck
}


void
HTMwr::StartCondTable(HTMwfile *wf, long tablenum)
{
	SetCondTable(wf, tablenum);

	if (!TableConds)
		return;

	long ccnt = TableConds->count();
	wf->SetAttrAdd(true);
	for (long i = 1; i <= ccnt; i++)
		wf->putAttrStr(CondAttrs.find(TableConds->number(i)));
	if (ccnt > 1)
		CheckCondConflict(wf, ccnt, TableConds);
}

#if 0
bool
HTMwr::CheckCondFtn(long ftnnum)
{
	FtnConds = (DClist *) CondFtns.find(ftnnum);
	if (!FtnConds)
		return true;  // end any text condition

	long ccnt = FtnConds->count();
	if (TextConds->count() != ccnt)
		return true;  // not identical, end text conds

	for (long i = 1; i <= ccnt; i++)
		if (!TextConds->find(FtnConds->number(i)))
			return true;  // cond not already set

	FtnConds = NULL;  // identical, so continue text conds
	return false;
}
#endif


void
HTMwr::StartCondFtn(HTMwfile *wf)
{
	if (!FtnConds)
		return;

	FilterConds(FtnConds, 2);

	long ccnt = FtnConds->count();
	wf->SetAttrAdd(true);
	for (long i = 1; i <= ccnt; i++)
		wf->putAttrStr(CondAttrs.find(FtnConds->number(i)));
	if (ccnt > 1)
		CheckCondConflict(wf, ccnt, FtnConds);
}


bool
HTMwr::CheckCondLink(long linknum)
{
	LinkConds = (DClist *) CondLinks.find(linknum);
	if (!LinkConds)
		return true;  // end any text condition

	long ccnt = LinkConds->count();
	if (TextConds->count() != ccnt)
		return true;  // not identical, end text conds

	for (long i = 1; i <= ccnt; i++)
		if (!TextConds->find(LinkConds->number(i)))
			return true;  // cond not already set

	LinkConds = NULL;  // identical, so continue text conds
	CondLinks.replace(LinkConds, linknum);
	return false;
}


void
HTMwr::StartCondLink(HTMwfile *wf, long linknum)
{
	LinkConds = (DClist *) CondLinks.find(linknum);
	if (!LinkConds)
		return;

	FilterConds(LinkConds, 4);

	long ccnt = LinkConds->count();
	wf->SetAttrAdd(true);
	for (long i = 1; i <= ccnt; i++)
		wf->putAttrStr(CondAttrs.find(LinkConds->number(i)));
	if (ccnt > 1)
		CheckCondConflict(wf, ccnt, LinkConds);
}


void
HTMwr::SetCondIndex(long indexnum)
{
	IndexConds = (DClist *) CondIndexes.find(indexnum);

	if (IndexConds)
		FilterConds(IndexConds, 4);
}


void
HTMwr::StartCondIndex(HTMwfile *wf)
{
	if (!IndexConds)
		return;

	long ccnt = IndexConds->count();
	wf->SetAttrAdd(true);
	for (long i = 1; i <= ccnt; i++)
		wf->putAttrStr(CondAttrs.find(IndexConds->number(i)));
	if (ccnt > 1)
		CheckCondConflict(wf, ccnt, IndexConds);
}


void
HTMwr::StartCondParaBlock(HTMwfile *wf)
{
	if (!UseParaCondClasses) {
		ParaConds = NULL;
		return;
	}

	ParaConds = (DClist *) CondParas.find(ParaRefNum);

#if 0
	// set PendEndCondFlags for any just ending ****
	long cfcnt = CurrCondFlags.count();
	long cf = 0;
	if (cfcnt) {
		for (long n = 1; n <= cfcnt; n++) {
			cf = CurrCondFlags.number(n);
			if (!ParaConds
			 || !ParaConds->find(cf)) {
				PendEndCondFlags.add(EndCondFlags.find(cf), cf);
				CurrCondFlags.remove(cf);
				cfcnt--;
				n--;
			}
		}
	}
#endif

	if (!ParaConds)
		return;

	if (HXMwr::DITA  // give in to stupidity (over-restricted titles)
	 && (HXMwr::DITANoBlock
	  || !strcmp(Curr.dtcname, "title")
	  || !strcmp(Curr.dtcname, "glossterm")
	  || HXMwr::FindPar(HXMwr::DITANoCondAttrs, Curr.dtcname))) {
		ParaConds = NULL;
		return;
	}

	FilterConds(ParaConds, 1);

	long ccnt = ParaConds->count();
	wf->SetAttrAdd(true);
	long cnum = 0;
	char *ftxt = NULL;
	for (long i = 1; i <= ccnt; i++) {
		cnum = ParaConds->number(i);
		wf->putAttrStr(CondAttrs.find(cnum));
#if 0
		// set PendStartCondFlags for any just starting ****
		if (!CurrCondFlags.find(cnum)) {
			if ((ftxt = StartCondFlags.find(cnum)) != NULL) {
				PendStartCondFlags.add(ftxt, cnum);
				CurrCondFlags.add(cnum);
			}
		}
#endif
	}
	if (ccnt > 1)
		CheckCondConflict(wf, ccnt, ParaConds);

}


void
HTMwr::StartCondRow(HTMwfile *wf)
{
	RowConds = (DClist *) CondRows.find(Row->rowref);
	//if (CurrConds)
	//	CurrConds->empty();

	if (!RowConds)
		return;

	FilterConds(RowConds, 0);

	long ccnt = RowConds->count();
	wf->SetAttrAdd(true);
	if (CurrCondClass) {
		wf->strAttr("class", CurrCondClass);
		CurrCondClass = NULL;
	}
	for (long i = 1; i <= ccnt; i++)
		wf->putAttrStr(CondAttrs.find(RowConds->number(i)));
	if (ccnt > 1)
		CheckCondConflict(wf, ccnt, RowConds);
}


bool
HTMwr::SetCondCharBlock(void)
{
	CharConds = (DClist *) CondChars.find(CharRefNum);
	if (!CharConds)
		return false;

	FilterConds(CharConds, 2);

	long ccnt = CharConds->count();
	if (!ccnt)
		return false;
	return true;
}


bool
HTMwr::CheckCondCharBlock(void)
{
	ChangedCharCond = CondCharChanged ? (bool) CondCharChanged->find(CharRefNum) : false;

	if (!SetCondCharBlock()
	 || ChangedCharCond)
		return true;  // end text conds before char format

	long ccnt = CharConds->count();
	if (TextConds->count() != ccnt)
		return true;  // not identical, end text conds

	for (long i = 1; i <= ccnt; i++)
		if (!TextConds->find(CharConds->number(i)))
			return true;  // cond not already set

	CharConds = NULL;  // identical, so continue text conds
	return false;
}


void
HTMwr::StartCondCharBlock(HTMwfile *wf)
{
	if (!CharConds)
		return;

	long ccnt = CharConds->count();
	wf->SetAttrAdd(true);
	for (long i = 1; i <= ccnt; i++)
		wf->putAttrStr(CondAttrs.find(CharConds->number(i)));
	if (ccnt > 1)
		CheckCondConflict(wf, ccnt, CharConds);
}


void
HTMwr::CondTextChange(HTMwfile *wf)
{
	// called when CondChanged after start of para
	if (InTextCond)   // existing ph or span, end it
		CondTextEnd(wf);

	CondTextStart(wf);

	CondChanged = false;
}



void
HTMwr::CondTextStart(HTMwfile *wf)
{
	// called to start cond ph or span
	if (!CurrConds)
		return;

	long tcnt = CurrConds->count();
	if (!tcnt)
		return;

	long i = 1;

	if (!TextConds)
		TextConds = new DClist;
	else
		TextConds->empty();

	for (i = 1; i <= tcnt; i++)
		TextConds->add(CurrConds->number(i));

	FilterConds(TextConds, 3);

	if (!TextConds
	 || ((tcnt = TextConds->count()) == 0))
		return;

	CPEndAll(wf);
	if (XMLSource
	 && (CharBlockState == CbStart))
		StartCharBlock(wf);
	SetCondFlags(wf);
	wf->startBlock(ConditionCharTag);
	wf->SetAttrAdd(true);
	for (i = 1; i <= tcnt; i++)
		wf->putAttrStr(CondAttrs.find(TextConds->number(i)));
	if (tcnt > 1)
		CheckCondConflict(wf, tcnt, TextConds);
	wf->endElem(true);
	wf->SetAttrAdd(false);
	InTextCond = true;
}


void
HTMwr::CondTextEnd(HTMwfile *wf)
{
	// called to end cond ph or span
	if (!InTextCond)
		return;

	CPEndAll(wf);
	wf->endBlock(ConditionCharTag);
	SetCondFlags(wf);

	InTextCond = false;
	CondChanged = true;  // to trigger recheck
}



DCnlist HTMwr::StartCondFlags;
DCnlist HTMwr::EndCondFlags;
DClist HTMwr::CurrCondFlags;

DCnlist HTMwr::PendStartCondFlags;
DCnlist HTMwr::PendEndCondFlags;

bool HTMwr::CondsEffective = false;
long HTMwr::CondFlagDiv = 0;
long HTMwr::PreCondFlagDiv = 0;


void
HTMwr::SetCondFlags(HTMwfile *wf)
{
	// set any pending end then start flags
	long cnum = 0;
	char *flag = NULL;

	if (CondFlagDiv)
		wf->SelectDiv(PreCondFlagDiv);

	while ((flag = PendEndCondFlags.dequeue(&cnum)) != NULL) {
		// if both set for same cond, ignore them
		if (PendStartCondFlags.find(cnum) != NULL)
			PendStartCondFlags.remove(cnum);
		else {
			wf->putEnd();
			wf->putStr(flag);
		}
	}

	if (CondFlagDiv) {
		wf->WriteDiv(CondFlagDiv);
		CondFlagDiv = 0;
		wf->SelectDiv(PreCondFlagDiv);
	}

	if (CurrCondFlags.count() == 0)
		CondsEffective = false;

	while ((flag = PendStartCondFlags.dequeue()) != NULL) {
		wf->putEnd();
		wf->putStr(flag);
	}
}


void
HTMwr::FilterConds(DClist *clist, unc filtlevel)
{
	// level = 0 table, 1 row, 2 para, 3 char, 4 text

	long tcnt = 0;
	long i = 1;

	if (!StripTable
	 && (InTbl || InTbCell || InTbTitle)) {
		if (TableConds
		 && clist->fullmatch(TableConds)) {
			tcnt = TableConds->count();
			for (i = 1; i <= tcnt; i++)
				clist->remove(TableConds->number(i));
		}

		if ((filtlevel > 0)
		 && RowConds
		 && clist->fullmatch(RowConds)) {
			tcnt = RowConds->count();
			for (i = 1; i <= tcnt; i++)
				clist->remove(RowConds->number(i));
		}
	}

	if ((filtlevel > 1)
	 && InFootnote
	 && FtnConds
	 && clist->fullmatch(FtnConds)) {
		tcnt = FtnConds->count();
		for (i = 1; i <= tcnt; i++)
			clist->remove(FtnConds->number(i));
	}

	if ((filtlevel > 1)
	 && ParaConds
	 && clist->fullmatch(ParaConds)) {
		tcnt = ParaConds->count();
		for (i = 1; i <= tcnt; i++)
			clist->remove(ParaConds->number(i));
	}

	if ((filtlevel > 2)
	 && CharConds
	 && clist->fullmatch(CharConds)) {
		tcnt = CharConds->count();
		for (i = 1; i <= tcnt; i++)
			clist->remove(CharConds->number(i));
	}

	if ((filtlevel > 3)
	 && TextConds
	 && clist->fullmatch(TextConds)) {
		tcnt = TextConds->count();
		for (i = 1; i <= tcnt; i++)
			clist->remove(TextConds->number(i));
	}
}


// save one level of cond status around table anchors

DClist *HTMwr::SavedCurrConds = NULL;
DClist *HTMwr::SavedParaConds = NULL;
DClist *HTMwr::SavedCharConds = NULL;
DClist *HTMwr::SavedTextConds = NULL;


void
HTMwr::SaveCondState(void)
{

	SavedCurrConds = CurrConds ? new DClist(CurrConds) : NULL;
	SavedParaConds = ParaConds ? new DClist(ParaConds) : NULL;
	SavedCharConds = CharConds ? new DClist(CharConds) : NULL;
	SavedTextConds = TextConds ? new DClist(TextConds) : NULL;
	ParaConds = CharConds = TextConds = NULL;
}


void
HTMwr::RestoreCondState(void)
{
	CurrConds = SavedCurrConds;
	ParaConds = SavedParaConds;
	CharConds = SavedCharConds;
	TextConds = SavedTextConds;
}


// save one level of cond status around footnotes

DClist *HTMwr::SavedCurrCondsFtn = NULL;
DClist *HTMwr::SavedParaCondsFtn = NULL;
DClist *HTMwr::SavedCharCondsFtn = NULL;
DClist *HTMwr::SavedTextCondsFtn = NULL;


void
HTMwr::SaveCondStateFtn(bool clear)
{
	SavedCurrCondsFtn = CurrConds ? new DClist(CurrConds) : NULL;
	SavedParaCondsFtn = ParaConds ? new DClist(ParaConds) : NULL;
	SavedCharCondsFtn = CharConds ? new DClist(CharConds) : NULL;
	SavedTextCondsFtn = TextConds ? new DClist(TextConds) : NULL;
	if (clear)
		ParaConds = CharConds = TextConds = NULL;
}


void
HTMwr::RestoreCondStateFtn(void)
{
	CurrConds = SavedCurrCondsFtn;
	ParaConds = SavedParaCondsFtn;
	CharConds = SavedCharCondsFtn;
	TextConds = SavedTextCondsFtn;
}




#if 0

void HTMwr::CompFullCondProp(void)
{
	// proc CurrConds items in CondProps to set FullCondProp
}

#endif


#if CONDSPANS

uns HTMwr::MaxCond = 0;

// scan phase

// if start is after beginning of para, or end is before end of para,
// use span; else (if all text in para is included) use div.
// if a div starts inside another div, but continues when the first
// div ends, end both and restart the second one.  Same for spans.
// if two divs start at the same place, start the one that runs longer
// first, so as to avoid the previous situation.  Same for spans.
// if in a div, and the cond extends partway into the next para, end
// the div before the para and use a span for the leftover part.

DCclist HTMwr::StartingConds;
uns HTMwr::StartPoint = 0;
DCclist *HTMwr::StartCondSpans = NULL;
DCclist *HTMwr::StartCondDivs = NULL;

DCvlist HTMwr::ParaStartCondSpans;
DCvlist HTMwr::ParaStartCondDivs;

void
HTMwr::SetCond(HTMctl *cp)
{
	// add cp to StartingConds iff at start of para before any text
	if (ParaStarting)
		StartingConds.add(cp, ParaRefNum);

	// else add it to StartCondSpans and include StartPoint value, a
	// count of text items passed in para used for level optimization
	else {
		long lev = 1; // optimize later
		if (!StartCondSpans)
			ParaStartCondSpans.add(StartCondSpans = new DCclist, ParaRefNum);
		StartCondSpans->add(cp, lev);
		cp->siz(StartPoint);
	}
}

DCclist HTMwr::EndingConds;
DCclist *HTMwr::EndCondSpans = NULL;
DCclist *HTMwr::EndCondDivs = NULL;

DCvlist HTMwr::ParaEndCondSpans;
DCvlist HTMwr::ParaEndCondDivs;

void
HTMwr::ResetCond(HTMctl *cp)
{
	long lev = 1; // optimize later

	// add cp to EndingConds iff it has matching StartingConds entry
	if (StartingConds.finddat(cp->dat()))
		EndingConds.add(cp, ParaRefNum);

	// else it should have matching StartCondSpans, optimize level
	// and add it to EndCondSpans
	else if (StartCondSpans && StartCondSpans->finddat(cp->dat())) {
		if (!EndCondSpans)
			ParaEndCondSpans.add(EndCondSpans = new DCclist, ParaRefNum);
		EndCondSpans->add(cp, lev);
	}
	else
		assert(0);
}

void
HTMwr::ParaTextScanCond(void)
{
	DCctl *c;
	long lev = 1; // optimize later

	if (EndingConds.any()) {
	// when more para text, move all from EndingConds to EndCondSpans 
		EndCondSpans->add(EndingConds);
	// move the matching StartingConds to StartCondSpans
		for (long i = 1; i <= MaxConds; i++) {
			if (EndingConds.finddat(i)) {
				StartCondSpans->add(c = StartingConds.finddatcp(i). lev);
				StartingConds.remove(c);
			}
		}
		EndingConds.empty();
	}
}

void
HTMwr::EndParaScanCond(void)
{
	// add surviving StartingConds to StartCondDivs for ParaRefNum
	if (StartingConds.any()) {
		if (!StartCondDivs)
			ParaStartCondDivs.add(StartCondDivs = new DCclist, ParaRefNum);
		StartCondDivs->add(StartingConds);
	}

	// add surviving EndingConds to EndCondDivs for ParaRefNum
	// and remove matching StartingConds
	if (EndingConds.any()) {
		if (!EndCondDivs)
			ParaEndCondDivs.add(EndCondDivs = new DCclist, ParaRefNum);
		EndCondDivs->add(EndingConds);
		for (long i = 1; i <= MaxConds; i++) {
			if (EndingConds.finddat(i))
				StartingConds.remove(StartingConds.finddatcp(i));
		}
		EndingConds.empty();
	}

  // do *not* clear StartingConds, all in effect at start of next
	// para should remain on so that ending will go onto EndingConds

	// clear para-dependent lists for next para
	StartCondSpans = NULL;
	EndCondSpans = NULL;
	StartCondDivs = NULL;
	EndCondDivs = NULL;
}


// write phase

// before writing para tag, start any divs that apply to full para
// after para tag, and anywhere in text, start spans.
// before closing tag, end any spans still open, respecting nesting
// after closing tag, end any divs that do not continue, per nesting

void
HTMwr::ParaStartCond(void)
{
	// before para start, get StartCondDivs. If any are not on CurrCondDivs,
	// add them to it with nest level and write <div class="condname">
}

void
HTMwr::StartCond(HTMctl *cp)
{
	// if not on CurrCondDivs for ParaRefNum, should be in StartCondSpans
	// write <span class="condname">, add to CurrCondSpans with nest level
}

void
HTMwr::EndCond(HTMctl *cp)
{
	// if not in EndCondDivs for ParaRefNum, should have matching CurrCondSpans
	// and should also be in EndCondSpans
	// write </span> as needed to clear its level,
	// and rewrite the <span> tags for any closed prematurely
}

void
HTMwr::ParaEndCond(void)
{
	// at end of para, if CurrCondSpans not empty, write </span> to clear
	// this should not be needed unless maybe when span continues in part
	// of the next para but not all of it
}

void
HTMwr::ParaAfterCond(void)
{
	// after para end tag, check EndCondDivs for ParaID, write </div> to
	// clear level of matching CurrCondDivs, rewrite <div> tags as needed
}

void
HTMwr::FileEndCond(void)
{
	// at end of doc (or at split or extract), clear all CurrCondDivs but
	// leave items on list for restoral at start of next file part
}

#endif


// end of dwhtmcn.cpp

