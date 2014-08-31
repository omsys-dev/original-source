// dwhtmhr.cpp is for dcl hierarchical tag classes, JHG
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



// reference info
DCnlist *HXMwr::TagNames = NULL;	// all tag names; other lists use this number (string pool)
                                     // these lists are indexed by tag number (root == 1):
DCilist *HXMwr::TagLevels = NULL;   // required level, like 1; if 0 (missing), valid at any level 
DCilist *HXMwr::TagSingle = NULL;   // single parent; if -1, check TagParents, if 0, valid in any
DCvlist *HXMwr::TagParents = NULL;  // DClist* of parent nums for tags that can have two or more
DCilist *HXMwr::TagInline = NULL;   // 1 if inline (use no returns), 0 (missing) if block (indent)
DCilist *HXMwr::AtFirstList = NULL; // list of child/parent pairs where child must be first

tagfunc HXMwr::PutStartFunc = &PutStart;
tagfunc HXMwr::PutStartFullFunc = &PutStartFull;
tagfunc HXMwr::PutEndFunc = &PutEnd;
tagfunc HXMwr::PutEndFullFunc = &PutEnd;

// current state info
long HXMwr::CurrLevel = 0;    // set to 1 after root tag start
long HXMwr::LastBlockLev = 0; // used for ending inlines
long HXMwr::CurrTag = 0;      // tag number of CurrLevel
bool HXMwr::CurrInline = false;   // true if CurrTag is inline
bool HXMwr::CurrPreform = false;   // true if CurrTag is preformatted
bool HXMwr::InCurrPreform = false;   // true if within preformatted area
long HXMwr::CurrTags[TagLevMax];  // tag number indexed by level
char *HXMwr::CurrTagNames[TagLevMax];  // tag name indexed by level
bool HXMwr::LevUsed[TagLevMax];		// true if ever used by child
long HXMwr::TempTags = 0;     // count of adds to TagNames
long HXMwr::ErrorCount = 0;   // count of errors reported in XML
bool HXMwr::FirstOK = true;   // true if passed CheckFirst

DCirfile *HXMwr::IniFile = NULL;


// DChier functions

void HXMwr::InitTags(tagset *tset, tagfunc sf = NULL, tagfunc sff = NULL,
                                   tagfunc ef = NULL, tagfunc eff = NULL)
{
	TagNames = tset->names;
	TagLevels = tset->levels;
	TagSingle = tset->single;
	TagParents = tset->parents;
	TagInline = tset->inlin;
	AtFirstList = tset->atfirst;
	DITATableNames = tset->tblnames;
	DITATableLists = tset->tbllists;
	DITAStartElem = tset->start;

	if (sf)
		PutStartFunc = sf;
	if (sff)
		PutStartFullFunc = sff;
	if (ef)
		PutEndFunc = ef;
	if (eff)
		PutEndFullFunc = eff;

	CurrLevel = 1;
	LastBlockLev = 1;
	CurrTag = RootTagNum;
	CurrTags[0] = 0;  // guard
	CurrTags[1] = RootTagNum;
	CurrTagNames[0] = NULL;
	CurrTagNames[1] = TagNames->find(RootTagNum);
	LevUsed[0] = true;
	LevUsed[1] = false;
	CurrInline = false;
	CurrPreform = false;
	InCurrPreform = false;
	TempTags = 0;
}


bool HXMwr::CheckFirst(long tnum, long par, long lev, long *first, long fmax)
{
	// true if OK to put tnum here under par, because 
	// either not on AtFirstList, or level not used yet
	long val = 0;
	DClist *pl = NULL;
	long lv = 0;
	FirstOK = true;

	if (!LevUsed[lev])
		return true;   // is first, OK in any case

	if (!tnum)  // PCDATA valid under par
		return (par && (TagInline->find(par) & MixedType));

	if (fmax == 0)
		fmax = CurrLevel;

	if (first) {  // list of parents for which curr format must be first
		while (*first) {
			if (*first > UserESetBase) {
				long *esnums = (long *) UserElemSetNums->find(*first);
				while (*esnums) {
					if (par == *esnums++)
						return (FirstOK = false);
				}
			}
			else if (par == *first) {
				for (lv = lev + 1; lv <= fmax; lv++) {
					if (CurrTags[lv] == *first)  // match at lower level
						break;
				}
				if (lv > fmax)  // no lower match
					return (FirstOK = false);
			}
			first++;
		}
	}

	if (((val = AtFirstList->find(tnum)) == par)
	 || (val == -1))  // -1 means for Any
		return (FirstOK = false);

	if ((val > ESetBase)
	 && ((pl = (DClist *) TagParents->find(val)) != NULL)
	 && pl->find(par))  // listed in the set
		return (FirstOK = false);

	return true;  // does not have to be first
	// was (!AtFirstList->match(tnum, par) || !LevUsed[lev]);
}


bool HXMwr::FirstFixed(HTMwfile *wf, long tnum, long *parnum, long *first)
{
	// called if failed for First at initial check
	// see if replacing current stack item with same fixes that
	// if so, do it and return true
	long par = 0;
	long currlev = CurrLevel;
	long currpar = CurrTags[currlev];
	long *esnums = NULL;
	long clev = 0;

	if (!tnum)
		return false;

	while (!currpar && (currlev > 1))
		currpar = CurrTags[--currlev];

	if (parnum) {  // is it legal for all parents?
		for (clev = 0; clev < currlev; clev++) {
			par = parnum[clev];
			if (!par)
				break;
			if (par > UserESetBase) {
				esnums = (long *) UserElemSetNums->find(par);
				while (*esnums) {
					if (*esnums == CurrTags[currlev - clev])
						break;
					esnums++;
				}
				if (*esnums == 0)
					return false;
			}
			else if (par != CurrTags[currlev - clev])
				return false;
		}
	}
	// bottom parent matches, see if First problem is at CurrLevel
	if (!LevUsed[currlev])
		return false;
	if (!first         // no first requirement
	 || *(first + 1))  // or more than one
		return false;

	if (*first > UserESetBase) {
		long *esnums = (long *) UserElemSetNums->find(*first);
		while (*esnums) {
			if (currpar == *esnums)
				break;
			esnums++;
		}
		if (!*esnums)
			return false;
	}
	else if (currpar != *first)
		return false;

	// current stack item is the only problem, replace it
	WriteEnd(wf, currpar);
	WriteStart(wf, currpar, true);
	return true;
}

bool HXMwr::IsLegal(long tnum, long lev, long *parnum, long *first, long fmax)
{
	long par = 0;
	long currpar = CurrTags[lev];
	long *esnums = NULL;
	FirstOK = true;  // in case CheckFirst not called

	while (!currpar && (lev > 1))
		currpar = CurrTags[--lev];

	if (fmax == 0)
		fmax = lev;

	if (!tnum  //  see if PCDATA valid under currpar
	 && !(TagInline->find(currpar) & MixedType))
		return false;

	if (parnum) {  // at least one required parent
		for (long i = 0, ln = lev; parnum[i] && (ln > 0); i++, ln--) {
			par = parnum[i];
			if (par > UserESetBase) {
				esnums = (long *) UserElemSetNums->find(par);
				while (*esnums) {
					if (CurrTags[ln] == *esnums)
						break;
					esnums++;
				}
				if ((*esnums == 0)
				 || !CheckFirst(tnum, *esnums, ln, first, fmax))
					return false;
			}
			else if ((par != CurrTags[ln])
			 || !CheckFirst(tnum, par, ln, first, fmax))
				return false;
		}
		return true;
	}
	if (tnum == 1)  // root is not legal within doc
		return false;
	if ((par = TagSingle->find(tnum)) == 0)
		return true;
	if (!CheckFirst(tnum, currpar, lev, first, fmax))
		return false;
	if (par < ESetBase)
		return (currpar == par);

	// multiple parents
	if (par > UserESetBase) {
		esnums = (long *) UserElemSetNums->find(par);
		while (*esnums) {
			if (currpar == *esnums++)
				return true;
		}
		return false;
	}

	DClist *plist = (DClist *) TagParents->find(par);
	long cnt = plist->count();
	for (long i = 1; i <= cnt; i++) {
		if (currpar == plist->number(i))
			return true;
	}
	return false;
}


long HXMwr::LookUp(long tnum, long lev, long *parnum, long *first)
{
	// locate tag on stach which is valid as parent for tnum
	for (long clev = (lev && (lev < CurrLevel)) ? (parnum ? lev - 1 : lev) : CurrLevel;
	     clev > 0; clev--) {

		if (IsLegal(tnum, clev, parnum, first))
			return clev;

		if (!IsEndable(clev))
			return 0;
	}
	return 0;
}


DClist *HXMwr::LookDown(HTMwfile *wf, long tnum, long lev, long *parnum, long *first)
{
	DClist pq;  // queue for parents for breadth-first check
	long clev, nlev, maxdepth;
	long plev = 0;
	long flev = 0;
	long xnum, pnum, lxnum;
	bool found = false;

	// generate list of tags to add to stack so that tnum is valid
	DClist *tlist = NULL;
	DCilist hist;  // list of child for each parent checked
	DCilist depth; // depth of parentage for item in queue

	// if not addable at current level, go up and try again
	for (clev = (lev && (lev < CurrLevel)) ? (parnum ? lev - 1 : lev) : CurrLevel;
	     clev > 0; clev--) {

		pnum = plev = flev = 0;
		pq.empty();
		pq.add(tnum + 1); // start with current item each time
		hist.empty(); // clear parent/child list
		if (lev) {
			depth.empty();
			depth.add(1, tnum);
			maxdepth = lev - clev;
		}

		while ((xnum = pq.dequeue()) != 0) {
			xnum--;

			while (parnum
			 && parnum[plev]
			 && parnum[plev + 1]) {
				pnum = parnum[plev++];  // required parent and grandparent(s)
				if ((pnum == 1) && (clev > 1))
					break; // at top of parents, go up stack one
				if (first && (first[flev] == pnum))
					flev++;
				if (IsLegal(pnum, clev, parnum + plev, first + flev)) {
					found = true;
					break;
				}
				else {
					if (lev) {
						nlev = depth.find(xnum);
						if (nlev < maxdepth) {
							depth.append(nlev + 1, pnum);
							hist.append(xnum, pnum);
						}
					}
					else
						hist.append(xnum, pnum);
					xnum = pnum;
				}
			}
			if ((pnum == 1) && (clev > 1))
				break; // at top of parents, go up stack one

			if (!found) {
				if (parnum
				 && parnum[plev]
				 && ((xnum == tnum)
				  || (plev > 0)))
					pnum = parnum[plev++];         // last required parent
				else
					pnum = TagSingle->find(xnum);  // consider possible parents

				if (pnum == 0) {  // any parent is legal for xnum
					if (xnum == 1)  // unless the child is topic itself
						break;
					found = true;
				}
				else if (pnum < ESetBase) {  // single parent
					if ((pnum == 1) && (clev > 1))
						break; // at top of parents, go up stack one
					if (!TagNames->find(pnum))
						break;  // not in this set
					if (first && (first[flev] == pnum))
						flev++;
					if (IsLegal(pnum, clev, NULL, first + flev)) {
						found = true;
					}
					else {  // in recognized set
						if (lev) {
							nlev = depth.find(xnum);
							if (nlev < maxdepth) {
								depth.append(nlev + 1, pnum);
								pq.add(pnum + 1);
								hist.append(xnum, pnum);
								if (first && (first[flev] == xnum))
									flev++;
							}
						}
						else {
							pq.add(pnum + 1); // not legal, add to queue
							hist.append(xnum, pnum);
						}
					}
				}
				else if (pnum > UserESetBase) {  // multiple parents
					long *esnums = (long *) UserElemSetNums->find(pnum);

					while (*esnums) {
						pnum = *esnums++;
						if (!TagNames->find(pnum))  // not in recognized set
							continue;                 // shouldn't happen  ;-)  ****
						if (first && (first[flev] == pnum))
							flev++;
						if (IsLegal(pnum, clev, NULL, first + flev)) {
							found = true;
							break;
						}
						if (lev) {
							nlev = depth.find(xnum);
							if (nlev < maxdepth) {
								depth.append(nlev + 1, pnum);
								pq.add(pnum + 1);
								hist.append(xnum, pnum);
							}
						}
						else {
							pq.add(pnum + 1); // not legal, add to queue
							hist.append(xnum, pnum);
						}
					}
				}
				else {  // pnum > ESetBase, multiple parents
					DClist *plist = (DClist *) TagParents->find(pnum);
					long cnt = plist->count();
					long i = 0;

					for (i = 1; i <= cnt; i++) {
						pnum = plist->number(i);
						if (!TagNames->find(pnum))  // not in recognized set
							continue;                 // shouldn't happen  ;-)  ****
						if (first && (first[flev] == pnum))
							flev++;
						if (IsLegal(pnum, clev, NULL, first + flev)) {
							found = true;
							break;
						}
						if (lev) {
							nlev = depth.find(xnum);
							if (nlev < maxdepth) {
								depth.append(nlev + 1, pnum);
								pq.add(pnum + 1);
								hist.append(xnum, pnum);
							}
						}
						else {
							pq.add(pnum + 1); // not legal, add to queue
							hist.append(xnum, pnum);
						}
					}
				}
			}
			if (found) {
				if (!pnum
				 && (xnum == tnum))
					return NULL;          // don't produce empty list
				tlist = new DClist;
				if (clev < CurrLevel)
					EndLevels(wf, clev);
				if (pnum)               // don't include if "any"
					tlist->add(pnum);
				while (xnum != tnum) {  // get chidren down to tnum
					tlist->add(xnum);
					xnum = hist.find(lxnum = xnum);
					if (xnum == lxnum)
						break;
				}
				return tlist;
			}
		}

		// nothing matched at current level, go up one and try again
		if (!IsEndable(clev))   // unless we can't end current level
			return NULL;
	}

	return NULL;
}


void HXMwr::StartLevels(HTMwfile *wf, DClist *tlist)
{
	long cnt = tlist->count();
	for (long i = 1; i <= cnt; i++) {
		// write '>' and a return to finish tags
		WriteStart(wf, tlist->number(i), true);
	}
}


void HXMwr::EndLevels(HTMwfile *wf, long lev)
{
	while (lev < CurrLevel)
		WriteEnd(wf, CurrTag);
}


void HXMwr::WriteStart(HTMwfile *wf, long tnum, bool complete)
{
	if (tnum >= UserESetBase) {  // interpolating from a user set
		long *lpt = (long *) UserElemSetNums->find(tnum);
		if (lpt)
			tnum = lpt[0];  // use the first element of the set
		else {
			assert(0);
			HTMwr::LogEvent(logdebug, 1, "Missing ElemSet in WriteStart()");
			return;
		}
	}
	LevUsed[CurrLevel] = true;  // level above is now used
	CurrLevel++;
	LevUsed[CurrLevel] = false;  // current level is just started
	CurrTags[CurrLevel] = tnum;
	CurrTag = tnum;
	if (tnum == RelTopicTag)
		InRelTopics = true;
	char *tag = TagNames->find(tnum);
	CurrTagNames[CurrLevel] = tag;
	CurrInline = (TagInline->find(tnum) & InLineType);
	CurrPreform = (TagInline->find(tnum) & PreformType);
	if (CurrPreform)
		wf->SetPreForm(InCurrPreform = true);
	if (!CurrInline)
		LastBlockLev = CurrLevel;
	if (complete)
	// || !strcmp(tag, "link"))
		(*PutStartFullFunc)(wf, tag, CurrLevel, CurrInline);
	else
		(*PutStartFunc)(wf, tag, CurrLevel, CurrInline);
}

void HXMwr::PutStart(HTMwfile *wf, char *tag, long lev, bool inlin)
{
	// write start (tag), with indent if !CurrInline, leaving open for attrs
	wf->putEnd();
	if (!inlin) {
		for (long i = 0; i < lev; i++) {
			wf->putChar(' ');
			wf->putChar(' ');
		}
	}
	wf->startBlock(tag);
}

void HXMwr::PutStartFull(HTMwfile *wf, char *tag, long lev, bool inlin)
{
	// write start (tag), with indent if !CurrInline, leaving open for attrs
	wf->putEnd();
	if (!inlin) {
		for (long i = 0; i < lev; i++) {
			wf->putChar(' ');
			wf->putChar(' ');
		}
	}
	wf->startSBlock(tag);
}

void HXMwr::WriteEnd(HTMwfile *wf, long tnum, bool complete)
{
	bool inl = CurrInline;
	assert (CurrTags[CurrLevel] == tnum);
	if (tnum == RelTopicTag)
		InRelTopics = false;
	CurrTag = CurrTags[--CurrLevel];
	CurrInline = (TagInline->find(CurrTag) & InLineType);
	CurrPreform = (TagInline->find(CurrTag) & PreformType);
	if (!CurrPreform                            // enclosing tag is not preform
	 && !(CurrInline || inl)                    // neither tag is inline
	 && (TagInline->find(tnum) & PreformType))  // ending one was preform
		wf->SetPreForm(InCurrPreform = false);
	char *tag = TagNames->find(tnum);
	if (!inl)
		LastBlockLev = CurrLevel;
	(*PutEndFunc)(wf, tag, CurrLevel + 1, inl);
}

void HXMwr::PutEnd(HTMwfile *wf, char *tag, long lev, bool inlin)
{
	// write end (tag), after return if !inl
	if (!inlin) {
		wf->putEnd();
		for (long i = 0; i < lev; i++) {
			wf->putChar(' ');
			wf->putChar(' ');
		}
	}
	wf->endBlock(tag);
}

void HXMwr::SetWarning(HTMwfile *wf, long warn)
{
	ErrorCount++;
	long div = wf->GetDiv();

	if (!DITAErrorDiv)
		DITAErrorDiv = wf->NewDiv();
	wf->SelectDiv(DITAErrorDiv);

	wf->putEnd();
	wf->putStr("<!-- ");
	switch (warn) {
		case ParentError:
			wf->putStr("Parent Error");
			break;

		case LevelError:
			wf->putStr("Level Error");
			break;

		case InvalidTag:
			wf->putStr("Invalid Tag");
			break;

		default:
			wf->putStr("Unknown Error");
			break;
	}
	if (CurrFMStartTag) {
		wf->putStr(" for ");
		wf->putStr(CurrFMStartTag);
	}
	wf->putStr(" -->");
	wf->putEnd();
	wf->SelectDiv(div);
}


long *HXMwr::GetNameNums(char **sar, bool rev)
{
	char **sp = sar;
	long count = 0;
	long tn = 0;

	while (*sp) {
		if (GetNameNum(*sp++) != 0)
			count++;
	}

	if (count == 0)
		return NULL;

	long cmax = count + 1;
	long *par = new long[cmax];
	sp = sar;

	if (rev) {
		par[count--] = 0;
		while (*sp
		 && (count >= 0)) {
			if ((par[count] = GetNameNum(*sp++)) != 0)
				count--;
		}
	}
	else {
		count = 0;
		while (*sp
		 && (count < cmax)) {
			if ((par[count] = GetNameNum(*sp++)) != 0)
				count++;
		}
		par[count] = 0;
	}
	return par;
}


long HXMwr::GetNameNum(char *name)
{
	long num = 0;

	if (*name == '$')
		num = GetElemSetNum(name);
	else if ((num = TagNames->getnum(name)) == 0) {
		if (UseCommonNames)
			num = CommonNames->getnum(name);
	}

	return num;
}


DCnlist *HXMwr::UserElemSetNames = new DCnlist;
DCvlist *HXMwr::UserElemSetNums = new DCvlist;
long HXMwr::UserElemSetID = UserESetBase;

long HXMwr::GetElemSetNum(char *name)
{
	long num = 0;
	char **eset = NULL;
	long *esnums;

	if ((UserElemSetID > UserESetBase)
	 && ((num = UserElemSetNames->getnum(name)) != 0))
		return num;

	if (IniFile
	 && IniFile->Section(DITA ? "DITAElementSets" : "DocBookElementSets")
	 && IniFile->Find(name)
	 && ((eset = IniFile->StrArrRight()) != NULL)
	 && ((esnums = GetNameNums(eset)) != NULL)) {
		UserElemSetNums->add(esnums, ++UserElemSetID);
		UserElemSetNames->add(name, num = UserElemSetID);
	}

	return num;
}


char **HXMwr::CombineSars(char **sar1, char **sar2)
{
	int cnt = 0;

	char **spt = sar1;
	while (*spt++)
		cnt++;

	spt = sar2;
	while (*spt++)
		cnt++;

	char **strs = (char **) new char[sizeof(char *) * (cnt + 1)];

	char **npt = strs;
	spt = sar1;
	while (*spt)
		*npt++ = *spt++;
	spt = sar2;
	while (*spt)
		*npt++ = *spt++;
	*npt = NULL;

	return strs;
}


// called for each start tag
// note that lev is the DITA level, not the Frame level
// par is the required parent, from [DITAParent]

bool HXMwr::WriteStartTag(HTMwfile *wf, char *tag, char **par, char **pfirst, long lev, bool inl)
{
	long tnum = 0;
	long elev = 0;
	DClist *tlist = NULL;
	long *parnum = NULL;
	long *first = NULL;
	char *deftag = inl ? DefDITAChar : DefDITAPar;

#if 0
	if (!tag) {  // closes up to root tag, at EOF
		EndLevels(wf, 1);
		return true;
	}
#endif

	if (par && *par)  // at least one parent, reverse list
		parnum = GetNameNums(par, true);

	if (pfirst && *pfirst)   // at least one first condition
		first = GetNameNums(pfirst, true);

	tnum = tag ? TagNames->getnum(tag) : 0;  // null tag is PCDATA

#if 0
	if (tnum == 0) {  // new
		if (deftag)
			tnum = TagNames->getnum(deftag);
		if (tnum == 0) {
			tnum = -(++TempTags);
			TagNames->add(tag, tnum);
		}
	}

	if (tnum < 0) {  // temp (invalid) tag
		if (CurrTag < 0)  // nest only if last one is valid
			WriteEnd(wf, CurrTag);
		SetWarning(wf, InvalidTag);
		WriteStart(wf, tnum);  // no point in checking
		if (parnum)
			delete [] parnum;
		return true;
	}

	inl = (TagInline->find(tnum) & InLineType);
	if (inl) {     // prevent illegal nesting of inlines
		while (CurrInline
		 && !IsLegal(tnum, CurrLevel, parnum, first))
			WriteEnd(wf, CurrTag);
	}
	else {                  // if new valid tag is block,
		while (CurrTag < 0)  // close last ones if invalid
		// || CurrInline)       // and close any inline tags
			WriteEnd(wf, CurrTag);
	}
#endif

	if (lev == 0)  // check for default level
		lev = TagLevels->find(tnum);

	if (lev == 0) {  // OK at any level, find legal parent
		if (!IsLegal(tnum, CurrLevel, parnum, first)
		 && (FirstOK  // problem with parents, not with First
		  || !FirstFixed(wf, tnum, parnum, first))) {
			if ((elev = LookUp(tnum, lev, parnum, first)) > 0)
				EndLevels(wf, elev);
			else if (DropInvalidParaTag
			 && (!tag || !strcmp(DefDITAPar, tag))
			 && !IsLegal(0, CurrLevel, parnum, first))
				tnum = 0;
			else if ((tlist = LookDown(wf, tnum, lev, parnum, first)) != NULL)
				StartLevels(wf, tlist);
			else  // no legal parent found
				SetWarning(wf, ParentError);
		}
		WriteStart(wf, tnum);  // write anyway even if invalid here
		if (parnum)
			delete [] parnum;
		return true;
	}

	// now (lev != 0)
	if (lev < CurrLevel) {  // see how much of stack to clear
		if (IsLegal(tnum, CurrLevel, parnum, first))
			;  // is legal at current level
		else if (DropInvalidParaTag
		 && !strcmp(DefDITAPar, tag)
		 && !IsLegal(0, CurrLevel, parnum, first))
			tnum = 0;
		else if ((tlist = LookDown(wf, tnum, lev, parnum, first)) > 0)
			StartLevels(wf, tlist);
		else if ((elev = LookUp(tnum, lev, parnum, first)) > 0)
			EndLevels(wf, elev);
		else
			SetWarning(wf, ParentError);
	}
	else {  // lev >= CurrLevel, interpolate parents
		if (((lev == CurrLevel)
		  || (parnum && (lev == (CurrLevel + 1))))
		 && IsLegal(tnum, CurrLevel, parnum, first))
			;
		else if (DropInvalidParaTag
		 && !strcmp(DefDITAPar, tag)
		 && !IsLegal(0, CurrLevel, parnum, first))
			tnum = 0;
		else if ((tlist = LookDown(wf, tnum, lev, parnum, first)) > 0) {
			StartLevels(wf, tlist);
			if (CurrLevel != lev)
				SetWarning(wf, LevelError);
		}
		else {  // not legal below
			SetWarning(wf, LevelError);
			if (IsLegal(tnum, CurrLevel, parnum, first))
				;  // is legal at current level
			else if (DropInvalidParaTag
			 && !strcmp(DefDITAPar, tag)
			 && !IsLegal(0, CurrLevel, parnum, first))
				tnum = 0;
			else if ((elev = LookUp(tnum, lev, parnum, first)) > 0)
				EndLevels(wf, elev);
			else
				SetWarning(wf, ParentError);
		}
	}
	if (parnum)
		delete [] parnum;
	if (first)
		delete [] first;
	if (CurrLevel == 0)
		return false;
	WriteStart(wf, tnum);
	return true;
}


// called only to end inline tags
bool HXMwr::WriteEndTag(HTMwfile *wf, char *tag)
{
	long tnum = 0;

	if (!tag) {
		EndLevels(wf, LastBlockLev);  // should end inline only ****
		return true;
	}

	//if (!CurrInline && (CurrTag > 0))
	//	return false;

	tnum = TagNames->getnum(tag);
	if ((tnum == 0)
	 && DefDITAChar)
		tnum = TagNames->getnum(DefDITAChar);
	assert (tnum == CurrTag);
	WriteEnd(wf, CurrTag);
	return true;
}


// called only to finish inline elements like <image>
bool HXMwr::WriteFinishTag(HTMwfile *wf, char *tag)
{
	long tnum = 0;

	if (tag) {
		tnum = TagNames->getnum(tag);
		if ((tnum == 0)
		 && DefDITAChar)
			tnum = TagNames->getnum(DefDITAChar);
		assert (tnum == CurrTag);
	}

	CurrTag = CurrTags[--CurrLevel];
	CurrInline = (TagInline->find(CurrTag) & InLineType);
	CurrPreform = (TagInline->find(CurrTag) & PreformType);
	return true;
}


// end of dwhtmhr.cpp

