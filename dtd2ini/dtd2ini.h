// dtd2ini.h is the header for writing a content-model .ini from a DTD
// Copyright )c) 2007 Jeremy H. Griffith. All rights reserved.
// Licensed under the General Public License (GPL), version 2.

#include <stdlib.h>
#include <stdio.h>
#include <malloc.h>
#include <string.h>
#include <ctype.h>
#include <assert.h>
#include "xmlparser.h"

// xmlparser.h is part of the RXP parser, by Richard Tobin,
// also licensed unfer the GPL and available on the Web at:
//  <http://www.cogsci.ed.ac.uk/~richard/rxp.html>
// This project uses the following modules from it:
//  charset ctype16 dtd hash http input namespaces stdio16
// string16 system url and xmlparser, each of which has a
// .c and a .h file, plus rxputil.h, types.h, and the set
// of files in the .\iso-8859 directory.  It is built on
// Windows 2000 Pro SP4 using Visual Studio 4.0; the .mak
// is included in the package.  The preprocessor defines
// CHAR_SIZE=8 and _BUILD_STD, and library WSOCK32.LIB, were
// added for RXD.  One change was made in the RXP source;
// in xmlparser.c, lines 3519 and 3520 were commented out to
// avoid an enormous number of warnings when processing the
// DocBook DTD ver. 4.5:
// 	if(ParserGetFlag(p, ErrorOnBadCharacterEntities))
//	    return error(p, "0x%x is not a valid 8-bit XML character", code);
//	//else
//	    //warn(p, "0x%x is not a valid 8-bit XML character; ignored", code);
 

typedef unsigned char unc;
typedef unsigned short uns;
#define INVALID 65535

struct elem_struct {
	const char *name;
	uns level;

	uns nparents;
	uns *parlist;
	uns nparts;
	unc parMatch;
	uns setMatched;

	uns nchildren;
	uns *children;
	uns cparts;
	unc childMatch;
	uns csetMatched;
	unc setChild;

	uns nfirst;
	uns *firstlist;
	uns fparts;
	unc setFirst;

	uns voteInline;
	uns voteBlock;
	unc inLine;
	unc hasData;
	unc noContent;
	unc used;
};

typedef struct elem_struct Elem;

// globals

extern unc UseIni;
extern char *IniName;

extern char *OutTypes[];
extern int OutType;
extern unc AlphaSort;

extern unc TopicParents;
extern unc TopicLevels;
extern unc TopicFirst;
extern unc TopicChildren;

extern unc ElementTypes;
extern unc ShowVotes;
extern int BlockWeight;

extern unc ElementSets;
extern int MinPart;
extern int MaxPart;

extern char *ModelName;
extern char *TopicRoot;
extern char *TopicStart;
extern char *TopicBody;
extern char *PrologDType;
extern char *PrologDTD;

extern uns RootNum;

extern const char *CurrElemName;
extern uns CurrElemNum;
extern Elem *CurrElem;

extern unc FirstChild;
extern unc InSeq;
extern unc InChoice;

extern Elem *ElemList;
extern uns NextElemNum;
extern uns ElemCount;

extern uns *UsedList;
extern uns UsedCount;

extern uns *SetList;
extern uns SetCount;

extern uns *CSetList;
extern uns CSetCount;

extern uns **PartsList;
extern uns *PartsListCount;
extern uns PartsCount;

extern uns *MatchedList;
extern uns MatchCount;

extern uns *FirstList;
extern uns FirstCount;
extern uns SetFirstCount;
extern uns SetChildCount;
extern uns LevelCount;


// functions

char *MakeURI(const char *arg);

void SetElem(const char *name);
uns GetElemNum(const char *name);
void SetChild (const char* child);
void ProcParticle(ContentParticle part);
char *GetSetName(const char *name, const char *suf);

unc MatchList(uns *prev, uns *curr, uns num);
unc ParentsMatch(uns k, uns *pnum);
unc ChildrenMatch(uns k, uns *pnum);
void ProcElemNum(uns i);
void RemoveFirst(uns i, uns j);
void RemoveChild(uns i, uns j);
void RemoveParent(uns i, uns j);

int UnsComp(const void *p1, const void *p2);
int NameComp(const void *p1, const void *p2);

void ProcElemSets(void);

void ReplaceFirstMatch(uns i, uns p);
void ReplaceChildMatch(uns i, uns p);
void ReplaceParentMatch(uns i, uns p);
void CheckChildMatches(uns i, uns j);
void CheckMatches(uns i, uns j);
void MakeChildPartsList(uns i, uns j);
void MakePartsList(uns i, uns j);
void GeneratePartialSets(void);

void WriteElemList(FILE *fp, char *fn);

// utilities in d2ird.c

extern void SetIniName(const char *argv0, const char *argv3);
extern void GetIniString(const char *section, const char *key, char **str);
extern void GetIniInt(const char *section, const char *key, int *varp);
extern void TrueFalseIni(const char *section, const char *key, unc *varp);
extern void GetIniMatch(const char *section, const char *key, char **list, int *varp);

extern void GetIniSect(const char *section);
extern unc CheckSectPart(void);
extern char *GetIniSectKey(void);
extern int GetIniSectCount(void);
extern char *GetFullIniSect(const char *name);


// end of dtd2ini.h
