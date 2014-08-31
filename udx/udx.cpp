// udx.cpp is the main file for udx.exe, which converts uDoc
// files between the full tagged and tag minimized forms
// Copyright 2013 by Jeremy H. Griffith, under Apache license

#include <time.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <errno.h>
#include "mxlparser.h"
#include "udx.h"


// Project debug settings:
// c:\test\udoc\udx\initial
// -m -o ..\min ud_minimize.mxd
// -o ..\full ud_minimize.mxd

// c:\test\udoc\udx\full
// -m -o ..\min2 ud_minimize.mxd


bool DoFull = true;
bool DoMin = false;
bool NoFull = false;
bool NoMin = false;

bool DoPara = true;
bool DoBlank = true;
bool DoListShort = true;
bool DoPairShort = true;
bool DoTableShort = true;
bool DoWiki = true;

bool DoParaM = true;
bool DoBlankM = true;
bool DoListShortM = true;
bool DoPairShortM = true;
bool DoTableShortM = true;
bool DoWikiM = true;

char *OutArg = NULL;
char *ErrArg = NULL;

bool RenameOfile = false;

UDini *IniFile = NULL;

FILE *InFile = NULL;
FILE *OutFile = NULL;

MxlParser *Parser = NULL;

char *SwitchHelp = {
"Usage: udx [switches] sourcefile.mxd [switches] another.mxd ...\n"
" Switches are cumulative and apply to all following source files.\n"
" Source file names can contain wildcards.\n"
" Primary processing:\n"
"  -f Make full-tagged form (default)\n"
"  -m Make minimized form\n"
" Output (default replaces source file):\n"
"  -o filename.mxd  Write to different filename in same dir\n"
"  -o path\\to\\dir   Write to same filename in different dir\n"
" Options (override udx.ini settings):\n"
"  -s options determine which short tags to process:\n"
"   -sa All short tags\n"
"   -st Short tags in tables\n"
"   -sp Short tags in pair lists, <pl>\n"
"   -sl Short tags in other lists: <ul>, <ol>, and <sl>\n"
"  -w  Convert to or from wiki symbols\n"
"  -b  Convert to or from blank lines in <p> elements\n"
"  -p  Wrap or unwrap text in block elements in <p> elements\n"
"  -n switches reverse preceding -s, -w, -p, and -b switches\n"
"     For example, -nst prevents short-tag conversion in tables\n"
" Help options:\n"
"  -h and -? both show this text.\n"
};



int main(int argc, char **argv)
{
	// read options for all files from udx.ini, 
	// in source file dir, in lib dir, or in prog file dir
	ProcessIni();
	
	bool processed = false;
	char **nextarg = NULL;

	for (int arg = 1; arg < argc; arg++) {
		if (*argv[arg] != '-') {
			// args are file(s) to process, or stdin
			if (nextarg) {
				*nextarg = argv[arg];
				nextarg = NULL;
			}
			else {
				ProcessFile(argv[arg]);
				processed = true;
			}
		}
		else switch (*(argv[arg] + 1)) {
			// switches apply to following args until changed
			// options are -f (full), -m (minimize),
			// -o output, -e errorfile, -h or -? (help)
			case 'f':
				DoFull = true;
				DoMin = false;
				break;

			case 'm':
				DoFull = false;
				DoMin = true;
				break;

			case 'o':
				OutArg = argv[arg] + 2;
				if (*OutArg == '\0')
					nextarg = &OutArg;
				break;

			case 'e':
				ErrArg = argv[arg] + 2;
				if (*ErrArg == '\0')
					nextarg = &ErrArg;
				break;

			case 'h':
			case '?':
				ShowHelp();
				return 0;

			// use -n + letter for No, just -letter for Yes:
			case 'n':
				switch (*(argv[arg] + 2)) {
					// -p para tags in block elems
					case 'p':
						DoPara = false;
						break;

					// -b blank lines for para breaks
					case 'b':
						DoBlank = false;
						break;

					// -s short tags + a (all), l (list), p (pair), t (table)
					case 's':
						switch (*(argv[arg] + 3)) {
							case 'a':
								DoListShort = false;
								DoPairShort = false;
								DoTableShort = false;
								break;

							case 'l':
								DoListShort = false;
								break;

							case 'p':
								DoPairShort = false;
								break;

							case 't':
								DoTableShort = false;
								break;

							default:
								break;
						}
						break;

					// -w wiki symbols
					case 'w':
						DoWiki = false;
						break;

					default:
						break;
				}
				break;

			// -p para tags in block elems
			case 'p':
				DoPara = true;
				break;

			// -b blank lines for para breaks
			case 'b':
				DoBlank = true;
				break;

			// -s short tags + a (all), l (list), p (pair), t (table)
			case 's':
				switch (*(argv[arg] + 3)) {
					case 'a':
						DoListShort = true;
						DoPairShort = true;
						DoTableShort = true;
						break;

					case 'l':
						DoListShort = true;
						break;

					case 'p':
						DoPairShort = true;
						break;

					case 't':
						DoTableShort = true;
						break;

					default:
						break;
				}
				break;

			// -w wiki symbols
			case 'w':
				DoWiki = true;
				break;

			default:
				break;
		}
	}
	if (!processed)
		ProcessFile(NULL);


	return 0;
}


void ShowHelp(void)
{
	fputs(SwitchHelp, stdout);
}


void ProcessIni(void)
{
	// open ini, read sections
	// update options variables
	IniFile = new UDini();

}


void ProcessFile(char *fname)
{
	// read each file using mxlparser, in SAX mode
	// process start tags, end tags, text nodes,.and comments
	// write to -o or stdout, errs to -e or stderr

	char *ofname = NULL;
	RenameOfile = false;
	int ftyp = OutArg ? FileInfo(OutArg) : 0;
	if (ftyp == 2) {  // dir, prefix to fname
		ofname = NewName(OutArg, strlen(OutArg) + strlen(fname) + 1);
		strcat(ofname, "\\");
		strcat(ofname, fname);
	}
	else if (ftyp == 1) {  // file, use for output
		ofname = OutArg;
	}
	else {  // doesn't exist, write temp and rename to orig
		ofname = tmpnam(NewName(L_tmpnam));
		RenameOfile = true;
	}

	if ((OutFile = fopen(ofname, "wt")) == NULL)
		return;

	Nesting = new UDilist();
	WikiSymsUsed = new UDilist();
	CurrStype = new UDlist();
	CurrShort = new UDlist();
	CurrShortEnd = new UDlist();
	CurrFull = new UDlist();
	CurrW2T = IniFile->GetSect("WikiSymbolReplacement");
	CurrT2W = IniFile->GetSect("WikiTagReplacement");
	CurrElems = new UDlist();
	CurrEtypes = new UDilist();

	char *wval = NULL;
	if ((wval = IniFile->GetVal("WikiTagReplacement", "<")) != NULL)
		WikiLT = wval;
	if ((wval = IniFile->GetVal("WikiTagReplacement", ">")) != NULL)
		WikiGT = wval;

	Parser = new MxlParser();
	Parser->SetOptions(true, false, false);
	Parser->SetCallbacks(ErrArg, NULL,
    StartTag, EndTag, ProcChar, NULL, 
    NULL, NULL, ProcComment);
	Parser->ParseFile(fname);

	if (PendingText) {
		WriteText(PendingText);
		PendingText = NULL;
	}

	delete Nesting;
	delete CurrStype;
	delete CurrShort;
	delete CurrShortEnd;
	delete CurrFull;
	fclose(OutFile);
	delete Parser;

	if (RenameOfile) { // rename ofname to fname
		int ret = 0;
		if ((ret = remove(fname)) == 0) {
			if ((ret = rename(ofname, fname)) == 0)
				return;
			fprintf(stderr, "Error renaming output to original file: &s.\n",
				              (errno == EACCES) ? "Original still exists" :
				              ((errno == ENOENT) ? "Cannot find output file" : ""));
		}
		fprintf(stderr, "Error removing original file before rename: &s.\n",
				            (errno == EACCES) ? "Original is read-only" :
				            ((errno == ENOENT) ? "Cannot find original file" : ""));
		fprintf(stderr, "Output is in: %s\n", ofname);
	}
}


long CurrDepth = 0;
long LastFullEndDepth = 0;
long NestingLevel = 0;
bool EmptyTag = false;
bool ShortTag = false;
char *PendingText = NULL;
unc InCode = false;
char *WikiLT = NULL;
char *WikiGT = NULL;
char *StartTextPend = NULL;
char *EndTextPend = NULL;
bool WrappedText = false;
bool IsPTag = false;
bool EndingPTag = false;
bool SkippedPTag = false;

char *CurrElem;
long CurrEtype;
UDlist *CurrElems = NULL;
UDilist *CurrEtypes = NULL;

UDnlist *CurrSTR = NULL;
UDnlist *CurrSTE = NULL;
UDnlist *CurrLTR = NULL;
UDnlist *CurrW2T = NULL;
UDnlist *CurrT2W = NULL;

UDilist *Nesting = NULL;
UDlist *CurrStype = NULL;
UDlist *CurrShort = NULL;
UDlist *CurrShortEnd = NULL;
UDlist *CurrFull = NULL;


void StartTag(unl *tag, long len, pair **attrs, long attrcnt)
{
	if (EmptyTag) { // end of previous start tag
		WriteChar('>');
		EmptyTag = false;
	}

	char *name = (char *) Parser->U32to8Str(tag, len);
	if (!stricmp(name, "udx")) {
		if (PendingText) {
			WriteText(PendingText);
			PendingText = NULL;
		}
		WriteStartTag(name, attrs, attrcnt);
		WriteChar('>');
		CheckUdxTag(attrs, attrcnt);
		return;
	}

	IsPTag = !stricmp(name, "p");
	long lastetype = CurrEtype;

	CurrElem = name;
	CurrEtype = GetCurrEtype(name);

	bool skiptag = false;

	if (DoMin) {
		if (DoBlank
		 && EndingPTag
		 && PendingText) {
			if (!IsPTag
			 || attrs) {  // mismatch
				WriteCode("</p>");
				WriteText(PendingText);  // double return
			}
			else { // suppress this one
				WriteText(PendingText);  // double return
				skiptag = true;
			}
			PendingText = NULL;
			EndingPTag = false;
		}
		else if (IsPTag
		 && !EndingPTag
		 && !PendingText) {
			SkippedPTag = true;
			skiptag = true;
		}
	}

	char *shtag = NULL;
	char *futag = NULL;
	char *shend = NULL;
	char *tmptxt = NULL;

	if (PendingText) {
		tmptxt = PendingText + strlen(PendingText);
		while ((*(tmptxt - 1) == '\n')
		 || (*(tmptxt - 1) == '\t')) {
			if (--tmptxt <= PendingText)
				break;
		}
		if (tmptxt > PendingText) {
			char ch = *tmptxt;
			*tmptxt = '\0';
			WriteText(PendingText);
			PendingText = tmptxt;
			*tmptxt = ch;
		}
		// else write it later, after added end tags
	}

	if (DoFull && DoPara) {
		if (!WrappedText
		 && (lastetype == 1)
		 && (CurrEtype == 3)) {  //block to inline
			WriteCode("<p>");
			WrappedText = true;
		}
		else if (WrappedText
		 && (CurrEtype == 1)) {
			WriteCode("</p>");
			WrappedText = false;
		}
	}

	if (PendingText) // if writing end tag for short, keep \ns till after
		PendingText = WriteTextWithoutTrailingSpace(PendingText);

	while (DoFull
	 && NestingLevel
	 && ((shend = CurrShortEnd->find(CurrDepth)) != NULL)
	 && CheckList(shend, name))  // end of CurrFull goes here
		WriteEndFullTag();  // close curr open tag if any

	if (PendingText) {  // write \ns now
		WriteText(PendingText);
		PendingText = NULL;
	}

	CurrElems->add(name, CurrDepth + 1);
	CurrEtypes->add(CurrEtype, CurrDepth + 1);

	CurrDepth++;

// NestingLevel of containers to check = depth of start tag
// for each container type, list of sections to use
	char *stype = IniFile->GetVal("ShortTagContainers", name);
	if (stype) { // end tag at which to end last open short tag
		if (DoFull) {
			CurrStype->add(name, CurrDepth);
			if (IniFile->GetBool("udxFullTaggingOptions",
													 "sConvert", stype)) {
				CurrSTR = IniFile->GetSect("ShortTagReplacement", stype);
				CurrSTE = IniFile->GetSect("ShortTagEnds", stype);
				Nesting->add(NestingLevel, CurrDepth);
				NestingLevel = CurrDepth;  // use list as stack ***
			}
		}
		else if (DoMin) {
			CurrStype->add(name, CurrDepth);
			if (IniFile->GetBool("udxTagMinimizingOptions",
													 "sToShortTagsConvert", stype)) {
				CurrLTR = IniFile->GetSect("LongTagReplacement", stype);
				Nesting->add(NestingLevel, CurrDepth);
				NestingLevel = CurrDepth;  // use list as stack ***
			}
		}
	}


	if (NestingLevel) {
		if (DoFull) {
			shtag = (char *) CurrSTR->getitem(name);
			shend = (char *) CurrSTE->getitem(name);
			if (shtag) {  // replace with full tag
				CurrShort->add(name, CurrDepth);
				// start tag before which to add end tag
				CurrShortEnd->add(shend, CurrDepth);
				CurrFull->add(shtag, CurrDepth);
				LastFullEndDepth = CurrDepth;
				name = shtag;
			}
		}
		else if (DoMin) {
			futag = (char *) CurrLTR->getitem(name);
			if (futag) {  // replace with short tag
				CurrFull->add(name, CurrDepth);
				CurrShort->add(futag, CurrDepth);
				name = futag;
				ShortTag = true;
			}
		}
	}

	// check for wiki symbol tags
	if (DoMin && DoWiki
	 && !attrs) {
		if (WriteWikiSym(name, true))
			return;
	}

	if (skiptag)
		return;

	WriteStartTag(name, attrs, attrcnt);
	if (IsCodeTag(name))
		InCode++;
	if (ShortTag) {  // write end now
		WriteChar('/');
		WriteChar('>');
		ShortTag = false;
	}
	else
		EmptyTag = true;

}


void WriteStartTag(char *name, pair **attrs, long attrcnt)
{
	WriteChar('<');
	WriteText(name);
	if (attrs) {
		pair *attr = NULL;
		char *aname = NULL;
		char *aval = NULL;
		for (long i = 0; i < attrcnt; i++) {
			attr = attrs[i];
			if (!attr)
				break;
			aname = (char *) Parser->U32to8Str(attr->name, attr->namelen);
			aval = (char *) Parser->U32to8Str(attr->val, attr->vallen);
			WriteAttr(aname, aval);
		}
	}
}


void EndTag(unl *tag, long len)
{
	char *name = (char *) Parser->U32to8Str(tag, len);
	long etype = CurrEtype;

	if (!stricmp(name, "udx")) {
		if (PendingText) {
			WriteText(PendingText);
			PendingText = NULL;
		}
		WriteCode("</udx>");
		CheckUdxEnd();
		return;
	}

	if (DoMin
	 && DoBlank
	 && EndingPTag) {
		if (!SkippedPTag)
			WriteCode("</p>");
		else
			SkippedPTag = false;
		EndingPTag = false;
	}

	if (PendingText) {
		EmptyTag = false;
		PendingText = WriteTextWithoutTrailingSpace(PendingText);
	}

	char *etag = NULL;
	char *shend = NULL;
	bool removed = false;

	if (DoFull) {
		if (((etag = CurrShort->find(CurrDepth)) != NULL)
		 && !stricmp(etag, name)) { // close of replaced short tag
			//removed = true;
			return;
		}
	}
	else if (DoMin) {
		if (((etag = CurrFull->find(CurrDepth)) != NULL)
		 && !stricmp(etag, name)) { // close of replaced full tag
			removed = true;
			CurrFull->remove(CurrDepth);
		}
	}

	bool skiptag = false;

	if (EmptyTag) {
		WriteChar('/');
		WriteChar('>');
		EmptyTag = false;
	}
	else if (!removed) {
		if (DoFull) {
			// see if end tag at same level as pending full end tag
			if (NestingLevel) {
				while (((((shend = CurrShortEnd->find(CurrDepth)) != NULL)
				  && CheckList(shend, name))  // end of CurrFull goes here
				 || (((etag = CurrStype->find(NestingLevel)) != NULL)
				  && !stricmp(name, etag)))
				 && (CurrDepth > NestingLevel))
					WriteEndFullTag();  // close curr open tag if any
				if (((etag = CurrStype->find(CurrDepth)) != NULL)
				 && !stricmp(name, etag)) { // see if closing CurrStype
					long depth = CurrDepth;
					while (LastFullEndDepth > depth) {
						CurrDepth = LastFullEndDepth;
						WriteEndFullTag();  // close last open tag in container if any
					}
					CurrDepth = depth;
					CurrStype->remove(CurrDepth);
					NestingLevel = Nesting->find(CurrDepth);
					Nesting->remove(CurrDepth);
				}
			}
		}
		else if (DoMin) {
			if (ShortTag) {
				skiptag = true;
				ShortTag = false;
			}

			if (NestingLevel) {
				if (((etag = CurrFull->find(CurrDepth)) != NULL)
				 && !stricmp(name, etag)) {  // see if end for removed full tag
					skiptag = true;
					CurrFull->remove(CurrDepth);
					CurrShort->remove(CurrDepth);
				}
				else if (((etag = CurrStype->find(CurrDepth)) != NULL)
				 && !stricmp(name, etag)) { // see if closing CurrStype
					CurrStype->remove(CurrDepth);
					NestingLevel = Nesting->find(CurrDepth);
					Nesting->remove(CurrDepth);
				}
			}
		}

		if (PendingText) {
			WriteText(PendingText);
			PendingText = NULL;
		}

		if (DoMin) {
			if (DoWiki) {  // check for wiki symbol tags
				if (WriteWikiSym(name, false))
					skiptag = true;;
			}

			if (DoBlank) {
				if (!stricmp(CurrElem, "p")
				 && !stricmp(name, "p")) {
					EndingPTag = true;
					skiptag = true;;
				}
			}

			if (DoPara) {
				if (SkippedPTag
				 && !stricmp(name, "p"))
					skiptag = true;;
			}
		}

		if (DoFull && DoPara) {
			// if ending block, and in wrapped text, close the p
			if ((etype == 1)
			 && WrappedText) {
				WriteCode("</p>");
				WrappedText = false;
			}
		}

		if (!skiptag) {
			WriteChar('<');
			WriteChar('/');
			WriteText(name);
			if (IsCodeTag(name))
				InCode--;
			WriteChar('>');
		}
	}

	CurrElems->remove(CurrDepth);
	CurrEtypes->remove(CurrDepth);
	CurrElem = CurrElems->find(CurrDepth - 1);
	CurrEtype = CurrEtypes->find(CurrDepth - 1);

	CurrDepth--;
}


long GetCurrEtype(char *name)
{
	char *etype = IniFile->GetVal("ElementTypes", name);
	if (!etype)
		return 1; // default to block
	else if (!strcmp(etype, "block"))
		return 1;
	else if (!strcmp(etype, "text"))
		return 2;
	else if (!strcmp(etype, "inline"))
		return 3;
	else
		return 1; // default to block
}
	

void WriteEndFullTag(void)
{
	char *etag = NULL;

	if ((etag = CurrFull->find(CurrDepth)) == NULL)
		return;

	long etype = GetCurrEtype(etag);

	// if pending text, strip trailing space until after end tag
	if (PendingText)
		PendingText = WriteTextWithoutTrailingSpace(PendingText);

	if (DoFull && DoPara) {
		// if ending block, and in wrapped text, close the p
		if ((etype == 1)
		 && WrappedText) {
			WriteCode("</p>");
			WrappedText = false;
		}
	}

	bool skiptag = false;

	if (DoMin && DoPara) {
		if (SkippedPTag
		 && !stricmp(etag, "p")) {
			skiptag = true;
			SkippedPTag = false;
		}
	}

	if (!skiptag) {
		WriteChar('<');
		WriteChar('/');
		WriteText(etag);
		WriteChar('>');
	}

	if (((etag = CurrShort->find(CurrDepth)) != NULL)
	 && !stricmp(etag, CurrElems->find(CurrDepth))) {
		CurrElems->remove(CurrDepth);
		CurrEtypes->remove(CurrDepth);
	}
	CurrFull->remove(CurrDepth);
	CurrShortEnd->remove(CurrDepth);
	CurrShort->remove(CurrDepth);
	LastFullEndDepth = --CurrDepth;
}


// char proc uses wiki sections

UDilist *WikiSymsUsed = NULL;

void ProcChar(unl *str, long len)
{
	if (DoMin
	 && DoBlank
	 && EndingPTag) {
		if (str
		 && (*str == '\n')
		 && (*(str + 1) == '\n')) {
			unl *cp = str;
			while (*cp == '\n')
				cp++;
			if (!*cp) { // only returns
				PendingText = (char *) Parser->U32to8Str(str, len);
				return;  // wait for start tag
			}
		}
		WriteCode("</p>");
		EndingPTag = false;
	}

	if (PendingText) {
		WriteText(PendingText);
		PendingText = NULL;
	}

	char *txt = (char *) Parser->U32to8Str(str, len);
	if (!CurrElem) {  // after end of root elem
		WriteText(txt);
		return;
	}

	if (EmptyTag) {
		WriteChar('>');
		EmptyTag = false;
	}

	if (DoFull && DoPara) {
		if (!WrappedText
		 && (CurrEtype == 1)) {  //block
			char *cp = txt;
			while (*cp && isspace(*cp))
				cp++;
			if (*cp) {
				for (char *fcp = txt; fcp < cp; fcp++)
					WriteChar(*fcp);
				txt = cp;
				WriteCode("<p>");
				WrappedText = true;
			}
		}
	}

	// check for wiki chars, double returns, and missing p tags

	if (DoFull) {  // this is all done here
		// for \n\n in a p, insert </p> before and <p> after, keep \n's
		bool blanks = DoBlank && !stricmp(CurrElem, "p");
		char *cp = txt;

		// replace wiki chars with tags
		if (DoWiki) {
			char *wval = NULL;
			char *wtag = NULL;
			char *sch = NULL;
			char *ech = NULL;

			char buf[2];
			buf[1] = '\0';
			char sym = 0;

			while (*cp) {
				if (blanks
				 && (*cp == '\n')
				 && (*(cp + 1) == '\n')
				 && (cp > txt)) {
					char *fret = cp;
					while (*cp == '\n')
						cp++;
					if (!*cp) // trailing returns
						break;
					*fret = '\0';
					WriteText(txt);
					*fret = '\n';
					WriteCode("</p>");
					while (fret < cp) {
						WriteChar('\n');
						fret++;
					}
					WriteCode("<p>");
					txt = cp;
					continue;
				}
				if ((*cp < 0x21) // UTF-8, space, or control
				 || isalnum(*cp)
				 || ((cp != txt)
				  && (*(cp - 1) == '\\'))
				 || (*cp == '\\')) {
					cp++;
					continue;
				}
				// must be symbol, check list
				buf[0] = *cp;  // single char
				if ((wval = CurrW2T->getitem(buf)) != NULL) {
					wtag = NULL;
					sch = NULL;
					ech = NULL;
					ParseWikiTag(wval, &wtag, &sch, &ech);

					sym = *cp;
					if (IsCodeTag(wtag)) { // code tag
						// if already in pre or code don't use tag
						if (InCode > 1) {
							wtag = NULL;
							if (!sch && !ech) {
								cp++;;
								continue;
							}
						}
						else
							InCode++;
					}
					else { //  inline format, not used in code
						// need to know if enclosing tag is pre or what
						// if in pre or code, it is literal ****
						if (InCode) {
							cp++;
							continue;
						}
						// see if preceded or followed by space or punct
						if ((cp != txt)
						 && (*(cp - 1) != '\\')
						 && !isspace(*(cp - 1))
						 && !ispunct(*(cp - 1))
						 && (*(cp + 1) != '\0')
						 && !isspace(*(cp + 1))
						 && !ispunct(*(cp + 1))) {
							cp++;
							continue;
						}
					}
					// is this first or second instance of sym at this level?
					bool start = true;
					UDilist *ip = WikiSymsUsed;
					UDilist *last = ip;
					while ((ip = ip->next) != NULL) {
						if ((ip->id == CurrDepth)
						 && (ip->val == (long) sym)) {
							start = false;
							if (IsCodeTag(wtag))
								InCode -= 2;
							last->next = ip->next;
							delete ip;
							break;
						}
						last = ip;	
					}
					if (start)
						WikiSymsUsed->add((long) sym, CurrDepth);

					// put out preceding text, then start or end tsg and string
					*cp = '\0';
					WriteText(txt);
					if (start) {
						if (wtag) {  // put out tag if needed
							WriteChar('<');
							WriteText(wtag);
							WriteChar('>');
						}
						if (sch && *sch)
							WriteCode(sch);
					}
					else {
						if (ech && *ech)
							WriteCode(ech);
						if (wtag) {  // put out tag if needed
							WriteChar('<');
							WriteChar('/');
							WriteText(wtag);
							WriteChar('>');
						}
					}
					txt = cp + 1;
				}
				cp++;
			}
			// process last text
			PendingText = txt;

		}
		else if (blanks) {
			while (*cp) {
				if ((*cp == '\n')
				 && (*(cp + 1) == '\n')
				 && (cp > txt)) {
					char *fret = cp;
					while (*cp == '\n')
						cp++;
					if (!*cp) // trailing returns
						break;
					*fret = '\0';
					WriteText(txt);
					*fret = '\n';
					WriteCode("</p>");
					while (fret < cp) {
						WriteChar('\n');
						fret++;
					}
					WriteCode("<p>");
					txt = cp;
					continue;
				}
				cp++;
			}
			PendingText = txt;
		}
	}
	else if (DoMin) {  // this is mainly done in StartTag and EndTag, not here
		// replace </p>...<p> with \n...\n.  
		//  If EndingP and then starting p w/o attrs,
		//  replace both tags but don't change line count.
		// replace inline tags with wiki chars in start/end procs
		if (StartTextPend) {  // from start tag
			char *gt = NULL;
			if ((*txt == '<')
			 && ((gt = strchr(txt, '>')) != NULL)
			 && (gt == (txt + strlen(txt) - 1))) {
				WriteCode(WikiLT);
				txt++;
				EndTextPend = ">";
				*gt = '\0';
			}
			else
				WriteCode(StartTextPend);
			StartTextPend = NULL;
		}
	}

	PendingText = txt;
}


bool IsCodeTag(char *tag)
{
	if (IniFile->GetVal("CodeElements", tag) != NULL)
		return true;

	return false;
}


void ParseWikiTag(char *wval, char **wtag, char **sch, char **ech)
{
	if (!wval || !*wval)
		return;

	char *owval = NewName(wval);
	wval = owval;
	char *sp = NULL;

	if (*wval == '-')  // no tag
		*wtag = NULL;
	else if ((sp = strchr(wval, ' ')) == NULL)
		*wtag = NewName(wval);  // only tag
	else {
		*sp = '\0';
		*wtag = NewName(wval);  // tag plus start/end strings
		*sp = ' ';
		wval = sp + 1;

		// get start and end strings
		*sch = NULL;
		*ech = NULL;
		if ((sp = strchr(wval, ' ')) == NULL)
			*sch = NewName(wval);
		else {
			*sp = '\0';
			*sch = NewName(wval);
			*sp = ' ';
			*ech = NewName(sp + 1);
		}
	}
	delete [] owval;
}


void ProcComment(char *cmt)
{
	if (PendingText) {
		WriteText(PendingText);
		PendingText = NULL;
	}

	if (EmptyTag) {
		WriteChar('>');
		EmptyTag = false;
	}

	WriteCode("<!-- ");
	WriteText(cmt);
	WriteCode(" -->");
}


char *WriteTextWithoutTrailingSpace(char *ptext, bool lead)
{
	char *tmptxt = NULL;

	tmptxt = ptext + strlen(ptext);
	while ((*(tmptxt - 1) == '\n')
	 || (*(tmptxt - 1) == '\t')) {  // trailing returns and tabs, not spaces
		if (--tmptxt <= ptext)
			break;
	}
	if (tmptxt > ptext) {
		char ch = *tmptxt;
		*tmptxt = '\0';
		WriteText(ptext);
		*tmptxt = ch;
		return tmptxt;
	}

	// else all white, write it later
	if (lead) // no, treat as leading space
		WriteText(ptext);
	else
		return ptext;  // at least preserve it

	return NULL;
}


void WriteText(char *txt)
{
	if (OutFile) {
		txt = CheckEntities(txt);
		fputs(txt, OutFile);
	}
}


void WriteCode(char *txt)
{
	if (OutFile) {
		fputs(txt, OutFile);
	}
}


bool WriteWikiSym(char *name, bool start)
{
	bool usesym = false;
	bool code = false;
	char *wval = NULL;
	char *ttype = NULL;
	if ((wval = CurrT2W->getitem(name)) != NULL) {
		if (IsCodeTag(name)) {
			code = true;
			if (((ttype = IniFile->GetVal("CodeElements", name)) != NULL)
			 && !stricmp(ttype, "inline"))
				usesym = true;
		}
		else // just inline tag
			usesym = true;

		if (usesym) {
			if (code) {
				if (start
				 && WikiLT) {
					AddStartTextPend(wval);
					return true;
				}
				else if (!start
				 && WikiGT
				 && EndTextPend
				 && (*EndTextPend == '>')) {
					EndTextPend = NULL;
					WriteChar('^');
					return true;
				}
			}
			WriteText(wval);
			return true;
		}
	}
	return false;
}


void AddStartTextPend(char *txt)
{
	if (StartTextPend) {
		char *st = NewName(StartTextPend, strlen(StartTextPend) + strlen(txt));
		strcat(st, txt);
		StartTextPend = st;
	}
	else
		StartTextPend = txt;
}


char *CheckEntities(char *txt)
{
	if (!strchr(txt, '<')
	 && !strchr(txt, '>')
	 && !strchr(txt, '&'))
		return txt;

	char *nt = NewName(strlen(txt) * 5);
	for (unsigned int i = 0; i < strlen(txt); i++) {
		if (txt[i] == '<')
			strcat(nt, "&lt;");
		else if (txt[i] == '>')
			strcat(nt, "&gt;");
		else if (txt[i] == '&')
			strcat(nt, "&amp;");
		else
			nt[strlen(nt)] = txt[i];
	}
	return nt;
}


void WriteChar(char ch)
{
	if (OutFile)
		fputc(ch, OutFile);
}


void WriteAttr(char *name, char *val)
{
	WriteText(" ");
	WriteText(name);
	WriteText("=");

	char *quote = strchr(val, '"') ? "'" : "\"";
	WriteText(quote);
	WriteText(val);
	WriteText(quote);
}


void CheckUdxTag(pair **attrs, long attrcnt)
{
	if (!attrs)
		return;

	pair *attr = NULL;
	char *aname = NULL;
	char *aval = NULL;

	for (long i = 0; i < attrcnt; i++) {
		attr = attrs[i];
		aname = (char *) Parser->U32to8Str(attr->name, attr->namelen);
		if (!stricmp(aname, "switch")) {
			aval = (char *) Parser->U32to8Str(attr->val, attr->vallen);
			if (!stricmp(aval, "none")) {
				if (DoFull) {
					DoFull = false;
					NoFull = true;
				}
				if (DoMin) {
					DoMin = false;
					NoMin = true;
				}
			}
			else if (!stricmp(aval, "nofull")) {
				if (DoFull) {
					DoFull = false;
					NoFull = true;
				}
			}
			else if (!stricmp(aval, "nomin")) {
				if (DoMin) {
					DoMin = false;
					NoMin = true;
				}
			}
			break;
		}
	}
}


void CheckUdxEnd(void)
{
	if (NoFull) {
		NoFull = false;
		DoFull = true;
	}
	if (NoMin) {
		NoMin = false;
		DoMin = true;
	}		
}


int FileInfo(char *fname)
{
	struct _stat buf;

	if (!_stat(fname, &buf)) {
		if (buf.st_mode & _S_IFDIR)
			return 2;
		if (buf.st_mode & _S_IFREG)
			return 1;
	}
	return 0;
}


char *NewName(char *str, int len)
{
	if (!len)
		len = strlen(str);
	char *cp = new char[len + 1];
	memset(cp, '\0', len + 1);
	strcpy(cp, str);
	return cp;
}


char *NewName(int len)
{
	char *cp = new char[len + 1];
	memset(cp, '\0', len + 1);
	return cp;
}


bool CheckList(char *list, char *name)
{
	// list is space-delimited names see if name is in list
	if (!list || !*list || !name || !*name)
		return false;

	char *sp = NULL;
	char *nlist = NewName(list);

	while ((sp = strchr(nlist, ' ')) != NULL) {
		*sp = '\0';
		if (!stricmp(name, nlist)) {
			*sp = ' ';
			return true;
		}
		*sp = ' ';
		nlist = sp + 1;
		if (!*nlist)
			return false;
	}
	if (!stricmp(name, nlist))  // last item
		return true;

	return false;
}



// end of udx.cpp
