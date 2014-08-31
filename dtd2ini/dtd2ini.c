// dtd2ini.c reads a DTD and writes a content-model .ini
// Copyright )c) 2007 Jeremy H. Griffith. All rights reserved.
// Licensed under the General Public License (GPL), version 2.

#include "dtd2ini.h"

// globals

unc UseIni = 0;
char *IniName = "dtd2ini.ini";

char *OutTypes[] = { "DITA", "XML" };
int OutType = 2;      // default to XML
unc AlphaSort = 1;    // 1 to sort all outputs alphabetically

unc TopicParents = 1;
unc TopicLevels = 1;
unc TopicFirst = 1;
unc TopicChildren = 0;

unc ElementTypes = 1;
unc ShowVotes = 0;    // show ElementType votes for inline/block
int BlockWeight = 10; // multiplier for votes for block type

unc ElementSets = 1;
int MinPart = 4;      // minimum partial set size
int MaxPart = 20;     // maximum partial set size

char *ModelName = NULL;
char *TopicRoot = NULL;
char *TopicStart = NULL;
char *TopicBody = NULL;
char *PrologDType = NULL;
char *PrologDTD = NULL;

uns RootNum = 0;

const char *CurrElemName = NULL;
uns CurrElemNum = 0;
Elem *CurrElem = NULL;

unc FirstChild = 0;
unc InSeq = 0;
unc InChoice = 0;

Elem *ElemList = NULL;
uns NextElemNum = 0;
uns ElemCount = 0;

uns *UsedList = NULL;
uns UsedCount = 0;

uns *SetList = NULL;
uns SetCount = 0;

uns *CSetList = NULL;
uns CSetCount = 0;

uns **PartsList = NULL;
uns *PartsListCount = NULL;
uns PartsCount = 0;

uns *MatchedList = NULL;
uns MatchCount = 0;

uns *FirstList = NULL;
uns FirstCount = 0;
uns SetFirstCount = 0;
uns SetChildCount = 0;
uns LevelCount = 0;


// functions

int main(int argc, char* argv[])
{
	Entity dtd = NULL;
	Parser p = NULL;
	XBit x = NULL;
	ElementDefinition *edefs;
	ElementDefinition elem;
	FILE *logfile = NULL;
	FILE *inifile = NULL;
	char *uri = NULL;
	uns i;

	if (argc < 3) {
		printf("usage: %s DTDfile.dtd Output.ini [Config.ini]\n", argv[0]);
		return 1;
	}

	// make DTD location into URI
	uri = MakeURI(argv[1]);

	// RXP puts out its own error messages for these
	dtd = NewExternalEntity(NULL, NULL, uri, NULL, NULL);
	p = NewParser();
	ParserSetFlag(p, WarnOnRedefinitions, 0);
	ParserSetFlag(p, ErrorOnBadCharacterEntities, 0);
	x =	ParseDtd(p, dtd);

	if ((ElemCount = p->dtd->nelements) == 0) {
		printf("\nNo elements found.\n");
		return 2;
	}

	ElemList = (Elem *) calloc(ElemCount, sizeof(Elem));
	UsedList = (uns *) calloc(ElemCount, sizeof(uns));
	SetList = (uns *) calloc(ElemCount, sizeof(uns));
	CSetList = (uns *) calloc(ElemCount, sizeof(uns));
	FirstList = (uns *) calloc(ElemCount, sizeof(uns));
	MatchedList = (uns *) calloc(ElemCount, sizeof(uns));

	edefs = p->dtd->elements;
	for (i = 0; i < ElemCount; i++)  // set all names first, so as to
		SetElem(edefs[i]->name);       // use ElemList as a string pool

	if ((logfile = fopen(argv[2], "w")) == NULL) {
		printf("\nCannot open %s for writing.\n", argv[3]);
		return 3;
	}

	SetIniName(argv[0], (argc > 3) ? argv[3] : NULL);

	if ((inifile = fopen(IniName, "r")) == NULL) {
		printf("\nConfiguration file %s missing.\n", IniName);
		return 4;
	}
	else {
		UseIni = 1;
		fclose(inifile);
	}

	if (UseIni) {
		GetIniString("Topic", "TopicRoot", &TopicRoot);
		if (TopicRoot == NULL) {
			printf("\nTopicRoot missing from configuration .ini.\n");
			return 5;
		}
	}

	if ((RootNum = GetElemNum(TopicRoot)) == INVALID) {
		printf("\nRoot element %s not found.\n", TopicRoot);
		return 5;
	}

	if (UseIni) {
		GetIniMatch("Options", "OutputType", OutTypes, &OutType);
		TrueFalseIni("Options", "AlphAlphaSort", &AlphaSort);

		TrueFalseIni("Options", "TopicParents", &TopicParents);
		TrueFalseIni("Options", "TopicLevels", &TopicLevels);
		TrueFalseIni("Options", "TopicFirst", &TopicFirst);
		TrueFalseIni("Options", "TopicChildren", &TopicChildren);

		TrueFalseIni("Options", "ElementTypes", &ElementTypes);
		TrueFalseIni("Options", "ShowVotes", &ShowVotes);
		GetIniInt("Options", "BlockWeighting", &BlockWeight);

		TrueFalseIni("Options", "ElementSets", &ElementSets);
		GetIniInt("Options", "MinimumPartSize", &MinPart);
		GetIniInt("Options", "MaximumPartSize", &MaxPart);

		GetIniString("Topic", "TopicStart", &TopicStart);
		GetIniString("Topic", "TopicBody", &TopicBody);
		GetIniString("Topic", "ModelName", &ModelName);
		GetIniString("Topic", "PrologDType", &PrologDType);
		GetIniString("Topic", "PrologDTD", &PrologDTD);
	}

	for (i = 0; i < ElemCount; i++) {  // proc content models
		elem = edefs[i];
		CurrElemNum = GetElemNum(CurrElemName = elem->name);
		if (CurrElemNum == INVALID) {
			i--, ElemCount--;
			continue;
		}
		CurrElem = &ElemList[CurrElemNum];
		FirstChild = 1;
		InSeq = InChoice = 0;
		ProcParticle(elem->particle);
	}

	ProcElemNum(RootNum);   // recursive, builds UsedList
	ProcElemSets();         // remove unused elems from parents
	if (ElementSets)
		GeneratePartialSets();  // find common subsets

	WriteElemList(logfile, uri); // write results
	fclose(logfile);

	return 0;
}

char *MakeURI(const char *arg)
{
	char *uri = NULL;
	char *urp = NULL;
	char *fileprotocol = "file:///";
	static char ubuf[_MAX_PATH + 4] = { '\0'};
	int pos = 0;

	if (!strncmp(arg, "file://", 7)
	 || !strncmp(arg, "http://", 7)
	 || !strncmp(arg, "ftp://", 6))
		return (char *) arg;
	else {
		strcpy(ubuf, fileprotocol);
		pos = strlen(fileprotocol);
	}

	if ((uri = _fullpath(NULL, arg, 0)) != NULL) {
		for (urp = uri; *urp && (pos < _MAX_PATH); urp++) {
			if (*urp == '\\')     // path delimiter
				ubuf[pos++] = '/';
			else if (*urp < 0x20) // discard control characters
				continue;
			else if ((*urp < 0)   // unacceptable per IETF RFC 2396
			 || (*urp == ' ')
			 || (*urp == '"')
			 || (*urp == '<')
			 || (*urp == '>')
			 || (*urp == '[')
			 || (*urp == ']')
			 || (*urp == '{')
			 || (*urp == '}')
			 || (*urp == '|')
			 || (*urp == '`')
			 || (*urp == '^')
			 || (*urp == '%')) {
				sprintf(ubuf + pos, "%%%0.2x", ((unsigned int) *urp) & 0xFF);
				pos += 3;
			}
			else                  // allow alnum and :/?;@&=+$,-_.!~*'()#
				ubuf[pos++] = *urp;
		}
		ubuf[pos] = '\0';
		free(uri);
		uri = ubuf;
	}
	else
		uri = (char *) arg;

	return uri;
}

void SetElem(const char *name)
{
	// add elem to elemlist
	Elem *el = &ElemList[NextElemNum++];
	el->name = name;
	el->level = 0;
	el->nparents = 0;
	el->parlist = (uns *) calloc(ElemCount, sizeof(uns));
	el->nparts = 0;
	el->nchildren = 0;
	el->children = (uns *) calloc(ElemCount, sizeof(uns));
	el->cparts = 0;
	el->nfirst = 0;
	el->firstlist = (uns *) calloc(ElemCount, sizeof(uns));
	el->fparts = 0;
	el->setMatched = 0;
	el->voteInline = 0;
	el->voteBlock = 0;
	el->inLine = 0;
	el->hasData = 0;
	el->noContent = 0;
	el->used = 0;
	el->parMatch = 0;
	el->setFirst = 0;
	el->setChild = 0;
}

uns GetElemNum(const char *name)
{
	uns i = 0;
	for (i = 0; i < ElemCount; i++)
		if (!strcmp(name, ElemList[i].name))
			return i;
	return INVALID;  // name missing
}

void SetChild (const char* child)
{
	uns cnum = GetElemNum(child);
	Elem *el = &ElemList[cnum];
	uns i;
	unc newchild = 1;

	if (cnum == INVALID)
		return;

	// store child in CurrElem list
	for (i = 0; i < CurrElem->nchildren; i++) {
		if (CurrElem->children[i] == cnum)
			newchild = 0;
	}
	if (newchild)
		CurrElem->children[CurrElem->nchildren++] = cnum;

	// store CurrElem as a parent of child
	for (i = 0; i < el->nparents; i++) {
		if (el->parlist[i] == CurrElemNum)
			return;  // already there
	}
	el->parlist[el->nparents++] = CurrElemNum;

	if (FirstChild && InSeq && !InChoice)
		el->firstlist[el->nfirst++] = CurrElemNum;

	if (CurrElem->hasData)  // if the parent can have mixed content,
		el->voteInline++;     //  the child is inline rather than block
	else                    // if the parent cannot have mixed content,
		el->voteBlock++;      //  the child is block rather than inline
}

void ProcParticle(ContentParticle part)
{
	uns i;

	if (part == NULL)  // no content allowed
		ElemList[CurrElemNum].noContent = 1;

	else switch (part->type) {
		case CP_pcdata: // elem can contain text
			ElemList[CurrElemNum].hasData = 1;
			break;

		case CP_name:   // name is a child
			if ((part->repetition == '*')
			 || (part->repetition == '+'))
				FirstChild = 0;
			SetChild(part->name);
			FirstChild = 0;
			break;

		case CP_seq:
			InSeq = 1;
			for (i = 0; i < part->nchildren; i++)
				ProcParticle(part->children[i]);
			break;
			
		case CP_choice: // nchildren is count
			if ((part->repetition == '*')
			 || (part->repetition == '+'))
				FirstChild = 0;
			if (part->nchildren > 1)
				InChoice = 1;
			for (i = 0; i < part->nchildren; i++)
				ProcParticle(part->children[i]);
			break;

		default:
			break;
	}
}

char *GetSetName(const char *name, const char *suf)
{
	size_t len = strlen(name) + 2;
	char *sname = NULL;

	if (suf)
		len += strlen(suf);
	sname = (char *) calloc(len, 1);
	*sname = '*';
	strcat(sname, name);
	if (suf)
		strcat(sname, suf);
	return sname;
}

unc MatchList(uns *prev, uns *curr, uns num)
{
	uns i;
	for (i = 0; i < num; i++) {
		if (prev[i] != curr[i])
			return 0;
	}
	return 1;
}

unc ParentsMatch(uns k, uns *pnum)
{
	uns i;
	Elem *curr = &ElemList[UsedList[k]];
	Elem *prev;
	for (i = 0; i < SetCount; i++) {
		prev = &ElemList[SetList[i]];
		if ((prev->nparents == curr->nparents)
		 && MatchList(prev->parlist, curr->parlist, prev->nparents)) {
			*pnum = SetList[i];
			return 1;
		}
	}
	return 0;
}

unc ChildrenMatch(uns k, uns *pnum)
{
	uns i;
	Elem *curr = &ElemList[UsedList[k]];
	Elem *prev;
	for (i = 0; i < CSetCount; i++) {
		prev = &ElemList[CSetList[i]];
		if ((prev->nchildren == curr->nchildren)
		 && MatchList(prev->children, curr->children, prev->nchildren)) {
			*pnum = CSetList[i];
			return 1;
		}
	}
	return 0;
}

void ProcElemNum(uns i)
{
	uns j;

	if (ElemList[i].used)
		return;
	ElemList[i].used = 1;
	UsedList[UsedCount++] = i;
	if (ElemList[i].nchildren)
		for (j = 0; j < ElemList[i].nchildren; j++)
			ProcElemNum(ElemList[i].children[j]);
}

void RemoveFirst(uns i, uns j)
{
	uns n = --ElemList[i].nfirst;
	n += ElemList[i].fparts; // parts are at end of firstlist

	if (n > j)  // some to move
		memmove(&ElemList[i].firstlist[j],
						&ElemList[i].firstlist[j + 1],
						(n - j) * sizeof(uns));
}

void RemoveChild(uns i, uns j)
{
	uns n = --ElemList[i].nchildren;
	n += ElemList[i].cparts; // parts are at end of children

	if (n > j)  // some to move
		memmove(&ElemList[i].children[j],
						&ElemList[i].children[j + 1],
						(n - j) * sizeof(uns));
}

void RemoveParent(uns i, uns j)
{
	uns n = --ElemList[i].nparents;
	n += ElemList[i].nparts; // parts are at end of parlist

	if (n > j)  // some to move
		memmove(&ElemList[i].parlist[j],
						&ElemList[i].parlist[j + 1],
						(n - j) * sizeof(uns));
}

int UnsComp(const void *p1, const void *p2)
{
	return ((int) *((const uns *) p1)) - ((int) *((const uns *) p2));
}

int NameComp(const void *p1, const void *p2)
{
	const char *n1 = ElemList[*((const uns *) p1)].name;
	const char *n2 = ElemList[*((const uns *) p2)].name;

	return strcmp(n1, n2);
}

void ProcElemSets(void)
{
	uns i, j, k, m;

	// remove parents not in rooted tree, match sets
	for (k = 0; k < UsedCount; k++) {
		i = UsedList[k];
		if (ElemList[i].nparents) {
			for (j = 0; j < ElemList[i].nparents; j++) {
				m = ElemList[i].parlist[j];
				if (!ElemList[m].used)  // remove
					RemoveParent(i, j--);
			}
			if (ElemList[i].nparents == 0)  // nobody left, is root
				assert(i == RootNum);
			else if (ElemList[i].nparents == 1) {
				m = ElemList[i].parlist[0];
				if (m == RootNum) {
					ElemList[i].level = 1;
					LevelCount++;
				}
			}
			else if (ElementSets) {
				qsort(ElemList[i].parlist, ElemList[i].nparents,
				      sizeof(uns), &UnsComp);
				if (ParentsMatch(k, &m)) {  // matched one in SetList
					ElemList[i].parMatch = 1;
					ElemList[i].setMatched = m;
				}
				else    // set is unique, add it to SetList
					SetList[SetCount++] = i;
			}
		}

		if (TopicFirst
		 && ElemList[i].nfirst) {
			for (j = 0; j < ElemList[i].nfirst; j++) {
				m = ElemList[i].firstlist[j];
				if (!ElemList[m].used)  // remove
					RemoveFirst(i, j--);
			}
			FirstList[FirstCount++] = i;
			if (ElementSets) {
				if ((ElemList[i].nfirst > 1)
				 && (ElemList[i].nfirst != ElemList[i].nparents)) {
					ElemList[i].setFirst = 1;
					SetFirstCount++;
				}
			}
		}

		if (TopicChildren
		 && ElemList[i].nchildren) {
			for (j = 0; j < ElemList[i].nchildren; j++) {
				m = ElemList[i].children[j];
				if (!ElemList[m].used)  // remove
					RemoveChild(i, j--);
			}
			if (ElementSets) {
				if (ElemList[i].nchildren > 1) {
					qsort(ElemList[i].children, ElemList[i].nchildren,
								sizeof(uns), &UnsComp);
					if (ChildrenMatch(k, &m)) {  // matched one in CSetList
						ElemList[i].childMatch = 1;
						ElemList[i].csetMatched = m;
					}
					else   // set is unique, add it to CSetList
						CSetList[CSetCount++] = i;
					ElemList[i].setChild = 1;
					SetChildCount++;
				}
			}
		}
		if (ElementTypes)
			ElemList[i].inLine = (ElemList[i].nparents == 0) ? 0 :
			 ((ElemList[i].voteInline > (ElemList[i].voteBlock * BlockWeight)) ? 1 : 0);
	}
}

void ReplaceFirstMatch(uns i, uns p)
{
	uns *pp = PartsList[p];
	uns np = PartsListCount[p];
	uns *pi = ElemList[FirstList[i]].firstlist;
	uns ni = ElemList[FirstList[i]].nfirst;
	uns *pni = &ElemList[FirstList[i]].fparts;
	uns j, k;

	// first make sure that all members of pp are in pi
	for (j = 0, k = 0; (j < np) && (k < ni); j++, k++) {
		if (pp[j] == pi[k])
			continue;
		if (pp[j] > pi[k])
			j--;
		else  // missed one
			break;
	}
	if (j < np)  // did not get to end of partsList
		return;

	// remove parents in FirstList[i] that are in PartsList[p]
	for (j = 0, k = 0; (j < np) && (k < ni); j++, k++) {
		if (pp[j] == pi[k]) {
			RemoveFirst(FirstList[i], k);
			ni--; // removal reduced count by 1
			k--;  // index is now that of next parent
			continue;
		}
		else // skip in parlist
			j--;
	}

	// add reference to partsList to firstlist, inc nparts
	pi[ni + (*pni)++] = p;
}

void ReplaceChildMatch(uns i, uns p)
{
	uns *pp = PartsList[p];
	uns np = PartsListCount[p];
	uns *pi = ElemList[CSetList[i]].children;
	uns ni = ElemList[CSetList[i]].nchildren;
	uns *pni = &ElemList[CSetList[i]].cparts;
	uns j, k;

	// first make sure that all members of pp are in pi
	for (j = 0, k = 0; (j < np) && (k < ni); j++, k++) {
		if (pp[j] == pi[k])
			continue;
		if (pp[j] > pi[k])
			j--;
		else  // missed one
			break;
	}
	if (j < np)  // did not get to end of partsList
		return;

	// remove children in UsedList[i] that are in PartsList[p]
	for (j = 0, k = 0; (j < np) && (k < ni); j++, k++) {
		if (pp[j] == pi[k]) {
			RemoveChild(CSetList[i], k);
			ni--; // removal reduced count by 1
			k--;  // index is now that of next child
			continue;
		}
		else // skip in nchildren
			j--;
	}

	// add reference to partsList to children, inc cparts
	pi[ni + (*pni)++] = p;
}

void ReplaceParentMatch(uns i, uns p)
{
	uns *pp = PartsList[p];
	uns np = PartsListCount[p];
	uns *pi = ElemList[SetList[i]].parlist;
	uns ni = ElemList[SetList[i]].nparents;
	uns *pni = &ElemList[SetList[i]].nparts;
	uns j, k;

	// first make sure that all members of pp are in pi
	for (j = 0, k = 0; (j < np) && (k < ni); j++, k++) {
		if (pp[j] == pi[k])
			continue;
		if (pp[j] > pi[k])
			j--;
		else  // missed one
			break;
	}
	if (j < np)  // did not get to end of partsList
		return;

	// remove parents in SetList[i] that are in PartsList[p]
	for (j = 0, k = 0; (j < np) && (k < ni); j++, k++) {
		if (pp[j] == pi[k]) {
			RemoveParent(SetList[i], k);
			ni--; // removal reduced count by 1
			k--;  // index is now that of next parent
			continue;
		}
		else // skip in parlist
			j--;
	}

	// add reference to partsList to parlist, inc nparts
	pi[ni + (*pni)++] = p;
}

void CheckChildMatches(uns i, uns j)
{
	// compare children for two members of CSetList and
	// put matching parents into MatchList, set MatchCount
	uns *pi = ElemList[CSetList[i]].children;
	uns ni = ElemList[CSetList[i]].nchildren;
	uns *pj = ElemList[CSetList[j]].children;
	uns nj = ElemList[CSetList[j]].nchildren;
	uns k, l;

	MatchCount = 0;
	for (k = 0, l = 0; (k < ni) && (l < nj); k++, l++) {
		if (pi[k] == pj[l]) {
			MatchedList[MatchCount++] = pi[k];
			if (MatchCount == MaxPart)
				break;
		}
		else if (pi[k] < pj[l])
			l--;
		else
			k--;
	}
}

void CheckMatches(uns i, uns j)
{
	// compare parents for two members of SetList and
	// put matching parents into MatchList, set MatchCount
	uns *pi = ElemList[SetList[i]].parlist;
	uns ni = ElemList[SetList[i]].nparents;
	uns *pj = ElemList[SetList[j]].parlist;
	uns nj = ElemList[SetList[j]].nparents;
	uns k, l;

	MatchCount = 0;
	for (k = 0, l = 0; (k < ni) && (l < nj); k++, l++) {
		if (pi[k] == pj[l]) {
			MatchedList[MatchCount++] = pi[k];
			if (MatchCount == MaxPart)
				break;
		}
		else if (pi[k] < pj[l])
			l--;
		else
			k--;
	}
}

void MakeChildPartsList(uns i, uns j)
{
	// copy MatchedList to new partsList, add to PartsList
	uns *partsList = NULL;
	uns k;

	partsList = (uns *) calloc(MatchCount, sizeof(uns));
	PartsList[PartsCount] = partsList;
	PartsListCount[PartsCount] = MatchCount;
	memcpy(partsList, MatchedList, MatchCount * sizeof(uns));

	// remove matching cnildren from CSetList[i], and from
	// CSetList[j] to end of CSetList, adding ref to PartsList
	// at the end of each elem.parlist and incrementing nparts
	ReplaceChildMatch(i, PartsCount);
	for (k = j; k < SetCount; k++)
		ReplaceChildMatch(k, PartsCount);
	PartsCount++;
}

void MakePartsList(uns i, uns j)
{
	// copy MatchedList to new partsList, add to PartsList
	uns *partsList = NULL;
	uns k;

	partsList = (uns *) calloc(MatchCount, sizeof(uns));
	PartsList[PartsCount] = partsList;
	PartsListCount[PartsCount] = MatchCount;
	memcpy(partsList, MatchedList, MatchCount * sizeof(uns));

	// remove matching parents from SetList[i], and from
	// SetList[j] to end of SetList, adding ref to PartsList
	// at the end of each elem.parlist and incrementing nparts
	ReplaceParentMatch(i, PartsCount);
	for (k = j; k < SetCount; k++)
		ReplaceParentMatch(k, PartsCount);
	PartsCount++;
}


void GeneratePartialSets(void)
{
	// reduce and combine remaining elem sets
	uns partsListAlloc = 20;
	uns i, j;

	PartsList = (uns **) calloc(partsListAlloc, sizeof(uns *));
	PartsListCount = (uns *) calloc(partsListAlloc, sizeof(uns));

	for (i = 0; i < SetCount; i++) {
		for (j = i + 1; j < SetCount; j++) {
			CheckMatches(i, j);
			if (MatchCount > MinPart) {
				if (PartsCount >= partsListAlloc) {
					partsListAlloc *= 2;
					PartsList = (uns **) realloc(PartsList,
					 partsListAlloc * sizeof(uns *));
					PartsListCount = (uns *) realloc(PartsListCount,
					 partsListAlloc * sizeof(uns));
				}
				MakePartsList(i, j);
				if (MatchCount == MaxPart)
					j--;  // look for more
			}
		}
	}

	if (TopicChildren) {
		for (i = 0; i < CSetCount; i++) {
			for (j = 0; j < PartsCount; j++)  // look for existing parts
				ReplaceChildMatch(i, j);
			for (j = i + 1; j < CSetCount; j++) {
				CheckChildMatches(i, j);
				if (MatchCount > MinPart) {
					if (PartsCount >= partsListAlloc) {
						partsListAlloc *= 2;
						PartsList = (uns **) realloc(PartsList,
						 partsListAlloc * sizeof(uns *));
						PartsListCount = (uns *) realloc(PartsListCount,
						 partsListAlloc * sizeof(uns));
					}
					MakeChildPartsList(i, j);
					if (MatchCount == MaxPart)
						j--;  // look for more
				}
			}
		}
	}
	if (TopicFirst) {
		for (i = 0; i < FirstCount; i++) {
			if (ElemList[FirstList[i]].setFirst) { // has a set to check
				for (j = 0; j < PartsCount; j++)  // look for existing parts
					ReplaceFirstMatch(i, j);
			}
		}
	}
}

void WriteElemList(FILE *fp, char *fn)
{
	uns i, j, k, m;
	int val;
	uns *pl = NULL;
	uns pn;
	char nbuf[17];
	uns bodyNum = INVALID;
	char *sect = NULL;
	char *asectname = NULL;
	char *etype = NULL;
	char *etstart = NULL;
	char *etend = NULL;
	unc preform = 0;

	fprintf(fp, "; Content model for DTD %s\n", fn);
	fprintf(fp, "; dtd2ini version 0.1 build 002 is licensed under GPL v.2.\n");
	fprintf(fp, "\n[Topic]\n");
	if (ModelName)
		fprintf(fp, "ModelName=%s\n", ModelName);
	fprintf(fp, "TopicRoot=%s\n", ElemList[RootNum].name);
	if (UseIni) {
		if (OutType == 1) { // DITA
			if (TopicStart
			 && (GetElemNum(TopicStart) != INVALID))
				fprintf(fp, "TopicStart=%s\n", TopicStart);
			if (TopicBody
			 && ((bodyNum = GetElemNum(TopicBody)) != INVALID))
				fprintf(fp, "TopicBody=%s\n", TopicBody);
		}
		if (PrologDType)
			fprintf(fp, "PrologDType=\"%s\"\n", PrologDType);
		if (PrologDTD)
			fprintf(fp, "PrologDTD=  \"%s\"\n", PrologDTD);
	}

	if (AlphaSort)
		qsort(UsedList, UsedCount, sizeof(uns), &NameComp);

	if (TopicParents) {
		fprintf(fp, "\n[TopicParents]\n");
		for (k = 0; k < UsedCount; k++) {
			i = UsedList[k];
			if ((i != RootNum)             // skip root and
			 && (i != bodyNum)             // DITA body and
			 && (ElemList[i].nparents      // any without parents
			  || ElemList[i].nparts)) {
				fprintf(fp, "%s=", ElemList[i].name);
				if (ElementSets) {
					if ((ElemList[i].nparents == 1)
					 && (!ElemList[i].nparts)) {
						m = ElemList[i].parlist[0];
						fprintf(fp, "%s", ElemList[m].name);
					}
					else if (ElemList[i].parMatch) {
						m = ElemList[i].setMatched;
						fprintf(fp, "%s", GetSetName(ElemList[m].name, NULL));
					}
					else
						fprintf(fp, "%s", GetSetName(ElemList[i].name, NULL));
				}
				else {
					if (AlphaSort)
						qsort(ElemList[i].parlist, ElemList[i].nparents, sizeof(uns), &NameComp);
					for (j = 0; j < ElemList[i].nparents; j++)
						fprintf(fp, "%s ", ElemList[ElemList[i].parlist[j]].name);
				}
				fprintf(fp, "\n");
			}
		}
	}

	if (TopicChildren) {
		fprintf(fp, "\n[TopicChildren]\n");
		for (k = 0; k < UsedCount; k++) {
			i = UsedList[k];
			if (ElemList[i].nchildren) {  // skip any without children
				fprintf(fp, "%s=", ElemList[i].name);
				if (ElementSets) {
					if ((ElemList[i].nchildren == 1)
					 && (!ElemList[i].cparts)) {
						m = ElemList[i].children[0];
						fprintf(fp, "%s", ElemList[m].name);
					}
					else if (ElemList[i].childMatch) {
						m = ElemList[i].csetMatched;
						fprintf(fp, "%s", GetSetName(ElemList[m].name, "child"));
					}
					else
						fprintf(fp, "%s", GetSetName(ElemList[i].name, "child"));
				}
				else {
					if (AlphaSort)
						qsort(ElemList[i].children, ElemList[i].nchildren, sizeof(uns), &NameComp);
					for (j = 0; j < ElemList[i].nchildren; j++)
						fprintf(fp, "%s ", ElemList[ElemList[i].children[j]].name);
				}
				fprintf(fp, "\n");
			}
		}
	}

	if (ElementSets) {
		if (PartsCount || SetCount || CSetCount || SetFirstCount)
			fprintf(fp, "\n[ElementSets]");
		if (PartsCount) {
			fprintf(fp, "\n; Element set parts for sets below\n");
			for (k = 0; k < PartsCount; k++) {  // write part lists first
				pl = PartsList[k];
				pn = PartsListCount[k];
				fprintf(fp, "%s=", GetSetName("Part", itoa(k + 1, nbuf, 10)));
				if (AlphaSort)
					qsort(pl, pn, sizeof(uns), &NameComp);
				for (j = 0; j < pn; j++)
					fprintf(fp, "%s ", ElemList[pl[j]].name);
				fprintf(fp, "\n");
			}
		}
		if (SetCount) {
			fprintf(fp, "\n; Element sets for [TopicParents]\n");
			if (AlphaSort)
				qsort(SetList, SetCount, sizeof(uns), &NameComp);
			for (k = 0; k < SetCount; k++) {
				i = SetList[k];
				fprintf(fp, "%s=", GetSetName(ElemList[i].name, NULL));
				if (AlphaSort)
					qsort(ElemList[i].parlist, ElemList[i].nparents, sizeof(uns), &NameComp);
				for (j = 0; j < ElemList[i].nparents; j++)
					fprintf(fp, "%s ", ElemList[ElemList[i].parlist[j]].name);
				for (j = 0; j < ElemList[i].nparts; j++) { // add part list refs
					pn = ElemList[i].parlist[j + ElemList[i].nparents];
					fprintf(fp, "%s ", GetSetName("Part", itoa(pn + 1, nbuf, 10)));
				}
				fprintf(fp, "\n");
			}
		}
		if (CSetCount) {
			fprintf(fp, "\n; Element sets for [TopicChildren]\n");
			if (AlphaSort)
				qsort(CSetList, SetCount, sizeof(uns), &NameComp);
			for (k = 0; k < CSetCount; k++) {
				i = CSetList[k];
				fprintf(fp, "%s=", GetSetName(ElemList[i].name, "child"));
				if (AlphaSort)
					qsort(ElemList[i].children, ElemList[i].nchildren, sizeof(uns), &NameComp);
				for (j = 0; j < ElemList[i].nchildren; j++)
					fprintf(fp, "%s ", ElemList[ElemList[i].children[j]].name);
				for (j = 0; j < ElemList[i].cparts; j++) { // add part list refs
					pn = ElemList[i].children[j + ElemList[i].nchildren];
					fprintf(fp, "%s ", GetSetName("Part", itoa(pn + 1, nbuf, 10)));
				}
				fprintf(fp, "\n");
			}
		}
		if (SetFirstCount) {
			fprintf(fp, "\n; Element sets for [TopicFirst]\n");
			if (AlphaSort)
				qsort(FirstList, FirstCount, sizeof(uns), &NameComp);
			for (k = 0; k < FirstCount; k++) {
				i = FirstList[k];
				if (ElemList[i].setFirst) {
					fprintf(fp, "%s=", GetSetName(ElemList[i].name, "first"));
					if (AlphaSort)
						qsort(ElemList[i].firstlist, ElemList[i].nfirst, sizeof(uns), &NameComp);
					for (j = 0; j < ElemList[i].nfirst; j++)
						fprintf(fp, "%s ", ElemList[ElemList[i].firstlist[j]].name);
					for (j = 0; j < ElemList[i].fparts; j++) { // add part list refs
						pn = ElemList[i].firstlist[j + ElemList[i].nfirst];
						fprintf(fp, "%s ", GetSetName("Part", itoa(pn + 1, nbuf, 10)));
					}
					fprintf(fp, "\n");
				}
			}
		}
	}

	if (ElementTypes) {
		fprintf(fp, "\n[ElementTypes]\n");
		for (k = 0; k < UsedCount; k++) {
			i = UsedList[k];
			// Block or Inline, Text, Preform
			fprintf(fp, "%s=", ElemList[i].name);
			etype = NULL;
			preform = 0;
			if (UseIni)
				GetIniString("ElementTypes", ElemList[i].name, &etype);
			if (etype != NULL) { // override
				etstart = etype;
				while (*etstart) {
					while (*etstart
					 && ((*etstart == ' ') || (*etstart == ',')))
						etstart++;
					etend = etstart;
					while (*etend && isalpha((int)*etend))
						etend++;
					if (!_strnicmp(etstart, "block", 5))
						ElemList[i].inLine = 0;
					else if (!_strnicmp(etstart, "inline", 6))
						ElemList[i].inLine = 1;
					else if (!_strnicmp(etstart, "preform", 7))
						preform = 1;
					etstart = etend;
				}
				free(etype);
			}
			fprintf(fp, "%s", ElemList[i].inLine ? "Inline" : "Block");
			if (ElemList[i].hasData)
				fprintf(fp, " Text");
			if (preform)
				fprintf(fp, " Preform");
			if (ShowVotes)
				fprintf(fp, " I:%hu B:%hu", ElemList[i].voteInline, ElemList[i].voteBlock);
			fprintf(fp, "\n");
		}
	}

	if (TopicLevels && LevelCount) {
		fprintf(fp, "\n[TopicLevels]\n");
		for (k = 0; k < UsedCount; k++) {
			i = UsedList[k];
			val = (int) ElemList[i].level;
			if (val)
				fprintf(fp, "%s=%d\n", ElemList[i].name, val);
		}
	}

	if (TopicFirst && FirstCount) {
		fprintf(fp, "\n[TopicFirst]\n");
		for (k = 0; k < FirstCount; k++) {
			i = FirstList[k];
			fprintf(fp, "%s=", ElemList[i].name);
			if ((ElemList[i].nfirst == ElemList[i].nparents)
			 && (ElemList[i].fparts == ElemList[i].nparts))
				fprintf(fp, "Any");
			else if (ElementSets) {
				if ((ElemList[i].nfirst == 1)
				 && (!ElemList[i].fparts)) {
					m = ElemList[i].firstlist[0];
					fprintf(fp, "%s", ElemList[m].name);
				}
				else if (ElemList[i].setFirst)
					fprintf(fp, "%s", GetSetName(ElemList[i].name, "first"));
			}
			else {
				if (AlphaSort)
					qsort(ElemList[i].firstlist, ElemList[i].nfirst, sizeof(uns), &NameComp);
				for (j = 0; j < ElemList[i].nfirst; j++)
					fprintf(fp, "%s ", ElemList[ElemList[i].firstlist[j]].name);
			}
			fprintf(fp, "\n");
		}
	}
	fprintf(fp, "\n\n");

	// add extra sections for DITA
	if (UseIni) {
		GetIniSect("AddedSections");
		while ((sect = GetIniSectKey()) != NULL) {
			GetIniString("AddedSections", sect, &asectname);
			if (asectname == NULL)
				continue;
			if ((sect = GetFullIniSect(asectname)) == NULL)
				continue;
			fprintf(fp, "%s", sect);
		}
	}
}

// end of dtd2ini.c
