// exwmf.cpp is the RTF Metafile Extractor used with DCL filters, JHG
// Copyright (c) 1997 Omni Systems, Inc.  All rights reserved.

#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <assert.h>

#include "exwmf.h"


// globals

bool AllowBmp = false; // produce WMFs for imported BMPs
bool AllowWmf = false; // produce WMFs for imported WMFs

FILE *rtfp = NULL;     // current RTF file

int rtfnest = 0;       // nesting level in file {}s

bool CtrlSym = false;  // ctrl read is a one-char symbol
uns CtrlErr = 0;       // ctrl read does not comply with spec

char wmfname[PATHMAX]; // current metafile name

char *picfields[] = {  // fields that may contain metafiles
 "IMPORT",
 "INCLUDEPICTURE",
 NULL
};

char *metactrls[] = {  // significant fields in metafile group
 "pich",
 "picw",
 "pichgoal",
 "picwgoal",
 NULL
};



// start of code

int main(int argc, char *argv[])
{
	// args are names of RTF files to process
	// switch -A includes all graphics, including .bmp and .wmf
	//  otherwise just extract those that were not originally usable

	if (argc < 2) {
		printf("usage: %s [-a] filename.rtf ...", argv[0]);
		return 1;
	}

	for (int i = 1; i < argc; i++) {
		if (*(argv[i]) == '-') {
			switch (*(argv[i] + 1)) {
				case 'a':
				case 'A':
					AllowBmp = true;
					AllowWmf = true;
					break;
				default:
					break;
			}
		}
		else
			ProcRtfFile(argv[i]);
	}

	return 0;
}


void ProcRtfFile(char *fname)
{
	// open RTF file, ensure it starts with "{\rtf1"
	if ((rtfp = fopen(fname, "r")) == NULL)
		return;

	const int rtfid_size = 6;
	const char *rtfid = "{\\rtf1";
	char idbuf[rtfid_size + 1];

	if (fread(idbuf, 1, rtfid_size, rtfp) != rtfid_size)
		return;
	idbuf[rtfid_size] = '\0';

	if (strncmp(idbuf, rtfid, rtfid_size))
		return;

	rtfnest = 1;  // for the opening brace

	// scan all fields until EOF
	while (!(feof(rtfp) || ferror(rtfp))) {
		if (ScanForCtrl("field")
		 && ProcField(picfields))
			GetMetafile();
	}

	fclose(rtfp);
}


bool ProcField(char **fnames)
{
	int startnest = rtfnest;

	// at "\field", get into instruction part
	if (!ScanForCtrl("fldinst", startnest))
		return false;

	// at "\fldinst", scan for metafile-containing field names
	//  if '}' first, return (different field type)
	char *field = ScanForName(rtfnest);
	if (CheckName(field, fnames) == 0)
		return false;  // different field type

	// collect filename as first token after field name, quoted
	char *filename = ScanForName(rtfnest);
	strncpy(wmfname, filename, PATHMAX);

	//  if already .wmf or .bmp, and not -A, return
  //  make .wmf name
	char *dot = strrchr(wmfname, '.');
	assert(dot != NULL);
	if (dot == NULL)
		return false;  // no ext on name

	// check for .bmp and .wmf imports
	if ((!AllowBmp)
	 && !strnicmp(dot, ".bmp", 4))
		return false;

	if ((!AllowWmf)
	 && !strnicmp(dot, ".wmf", 4))
		return false;

	// set new file extension
	strcpy(dot, ".wmf");

	// scan for "\fldrslt"
	// if rtfnest becomes < startnest field has ended
	if (!ScanForCtrl("fldrslt", startnest))
		return false;

	// collect only metafile results
	// if rtfnest decreases group has ended
	if (!ScanForCtrl("wmetafile8", rtfnest))
		return false;

	return true;
}


void GetMetafile(void)
{
	int ch;
	int startnest = rtfnest;

	const int num_size = 12;
	char buf[num_size + 1];
	int pos = 0;
	bool token = false;

	char *ctrl = NULL;
	uns prop = 0;
	long argval = 0;
	uns hgoal = 0;
	uns wgoal = 0;
	uns pich = 0;
	uns picw = 0;

	// scan for start of metafile hex header, "010009000003"
	//  if nesting decreases, not in group, return

	while ((ch = fgetc(rtfp)) != EOF) {
		if ((ch == '\n')
		 || (ch == '\r'))
			continue;
		if (ch == 0x7D) {
			rtfnest--;
			if (rtfnest < startnest)
				return;  // group ended without meta header
		}
		else if (ch == 0x7B) 
			rtfnest++;
		else if (ch == '\\') {
			argval = 0;
			ctrl = GetRtfCtrl(&argval);
			prop = CheckName(ctrl, metactrls);
			switch(prop) {
				case 1: // pich
					pich = (uns) argval;
					break;
				case 2: // picw
					picw = (uns) argval;
					break;
				case 3: // pichgoal
					hgoal = (uns) argval;
					break;
				case 4: // picwgoal
					wgoal = (uns) argval;
					break;
				default:
					break;
			}
			token = false;
		}
		else if (ch == ' ')
			token = false;
		else if (token) {
			if (isdigit(ch)) {
				buf[pos++] = ch;
				if (pos == num_size) {
					buf[pos] = '\0';
					if (!strcmp(buf, "010009000003"))
						break;
					return;    // not a metafile header
				}
			}
			else if (!isspace(ch)) // allow only spaces
				token = false;
		}
		else if (isdigit(ch)) {  // possible header start
			token = true;
			buf[0] = ch;
			pos = 1;
		}
	}



	FILE *mfp = fopen(wmfname, "wb");
	if (mfp == NULL)
		return;

	// write placeable metafile header
	fputc(0xD7, mfp); // ID
	fputc(0xCD, mfp);
	fputc(0xC6, mfp);
	fputc(0x9A, mfp);
	fputc(0, mfp);  // unused, 0
	fputc(0, mfp);
	fputc(0, mfp);  // left x
	fputc(0, mfp);
	fputc(0, mfp);  // top y
	fputc(0, mfp);
	fputc(wgoal & 0xFF, mfp);  // right x
	fputc((wgoal >> 8) & 0xFF, mfp);
	fputc(hgoal & 0xFF, mfp);  // bottom y
	fputc((hgoal >> 8) & 0xFF, mfp);
	fputc(0xA0, mfp);  // scale, 1440
	fputc(0x05, mfp);
	fputc(0, mfp);  // unused, 0
	fputc(0, mfp);
	fputc(0, mfp);
	fputc(0, mfp);

	uns check = 0xCDD7 ^ 0x9AC6 ^ wgoal ^ hgoal ^ 0x05A0 ^ 0;
	fputc(check & 0xFF, mfp);  // checksum
	fputc((check >> 8) & 0xFF, mfp);

	// write start of wmf header
	fputc(1, mfp);
	fputc(0, mfp);
	fputc(9, mfp);
	fputc(0, mfp);
	fputc(0, mfp);
	fputc(3, mfp);

	// write all hex data to .wmf file until '}'
	//  skip white space
	bool first = true;
	bool getlen = true;
	unl lcount = 6;  // in bytes
	unl length = 0;

	while ((ch = fgetc(rtfp)) != EOF) {
		if ((ch == '\n')
		 || (ch == '\r'))
			continue;
		if (ch == 0x7D) {
			rtfnest--;
			break;
		}
		else if (ch == 0x7B) { // should never happen
			rtfnest++;
			break;
		}
		if (isxdigit(ch)) {  // uses lowercase only
			if (first) {
				argval = (ch > '9') ? (ch - 'a') + 10 : ch - '0';
				first = false;
			}
			else {
				argval <<= 4;
				argval += (ch > '9') ? (ch - 'a') + 10 : ch - '0';
				fputc(argval & 0xFF, mfp);
				lcount++;
				if (getlen) {
					length >>= 8;
					length += (unl) (argval << 24);
					if (lcount >= 10) {
						length *= 2;
						getlen = false;
					}
				}
				first = true;
			}
		}
		// ignore any other chars such as spaces
	}
	assert(lcount == length);

	fclose(mfp);
}


// utility functions


bool ScanForCtrl(char *cname, int minnest, long *argval)
{
	int ch;
	char *ctrl;
	uns len = 0;

	if ((cname == NULL)
	 || ((len = strlen(cname)) == 0))
		return false;

	// scan for named ctrl until end of specified group
	while ((ch = fgetc(rtfp)) != EOF) {
		if ((ch == '\n')
		 || (ch == '\r'))
			continue;
		if (ch == 0x7D) {
			rtfnest--;
			if (rtfnest < minnest)
				return false; // out of specified group
		}
		else if (ch == 0x7B)
			rtfnest++;
		else if (ch == '\\') {
			ctrl = GetRtfCtrl(argval);
			if ((len == strlen(ctrl))
			 && !_stricmp(ctrl, cname))
				return true;
		}
	}

	return false;  // at EOF
}


char *GetNextCtrl(int minnest)
{
	int ch;
	
	// scan for ctrl until end of specified group
	while ((ch = fgetc(rtfp)) != EOF) {
		if ((ch == '\n')
		 || (ch == '\r'))
			continue;
		if (ch == 0x7D) {
			rtfnest--;
			if (rtfnest < minnest)
				return NULL; // out of specified group
		}
		else if (ch == 0x7B)
			rtfnest++;
		else if (ch == '\\')
			return GetRtfCtrl(NULL);
	}

	return NULL;  // at EOF
}


char *GetRtfCtrl(long *argval)
{
	int ch;
	static char buf[128];  // spec limits to 32 chars
	uns pos = 0;
	bool minus = false;
	bool hyphen = false;
	unc err = 0;

	CtrlSym = false;
	CtrlErr = 0;

	// first char after a backslash
	if ((ch = fgetc(rtfp)) == EOF) { 
		buf[0] = '\0';
		CtrlErr = 1;
		return buf;
	}

	// special handling for first char
	if (isalpha(ch)) // spec officially limits to lower case
		buf[pos++] = ch;
	else {           // single character Control Symbol (or hex value)
		switch (ch) {
			case '\n':
			case '\r':  // treat as actual para break
				strcpy(buf, "par");
				return buf;

			case '\'': // hex special character value
				buf[pos++] = ch;
				if (((ch = fgetc(rtfp)) == EOF)
				 || !isxdigit(ch)) {
					CtrlErr = 2;
					break;
				}
				if (argval)
					*argval = (ch > '9') ? (ch - 'a') + 10 : ch - '0';
				else
					buf[pos++] = ch;
				if (((ch = fgetc(rtfp)) == EOF)
				 || !isxdigit(ch)) {
					CtrlErr = 2;
					break;
				}
				if (argval)
					*argval = (*argval << 4) +
										(ch > '9') ? (ch - 'a') + 10 : ch - '0';
				else
					buf[pos++] = ch;
				buf[pos] = '\0';
				CtrlSym = true;
				return buf;

			case '*':  // optional destination next
			case '-':  // discretionary hyphen
			case ':':  // index entry subentry next
			case '\\': // literal backslash
			case '_':  // nonbreaking hyphen
			case 0x7B: // literal left brace
			case '|':  // formula char for Mac Word 5.1
			case 0x7D: // literal right brace
			case '~':  // nonbreaking space
				CtrlSym = true;
				break;

			default:
				// number, punctuation or whitespace, illegal as first char
				CtrlErr = 3;
				break;
		}

		buf[pos++] = ch;
		buf[pos] = '\0';
		return buf;
	}

	while ((ch = fgetc(rtfp)) != EOF) { // continue until terminator
		if ((ch == '\n')
		 || (ch == '\r'))
			continue;
		if (isalpha(ch))      // spec limits to lower case
			buf[pos++] = ch;
		else if ((ch == '-')
		 && !hyphen) { 
			hyphen = true;      // set so second one becomes terminator
			if (argval)
				minus = true;
			else
				buf[pos++] = ch;
		}
		else if (isdigit(ch)) {
			if (argval)         // store numeric part in arg if given
				*argval = (*argval * 10) + (ch - '0');
			else                //  in which case omit it from string
				buf[pos++] = ch;
		}
		else if (ch == ' ')   // space is terminator, remove it
			break;
		else {    // punctuation, including backslash, 0x7B, and 0x7D 
			ungetc(ch, rtfp);   // terminator symbol, put back for next scan
			break;
		}
	}

	buf[pos] = '\0';
	if (minus)  // means argval must be set
		*argval = -(*argval);
	return buf;
}


uns CheckName(char *name, char **list)
{
	uns len = 0;
	if ((name == NULL)  // empty name is not in any list, and
	 || (list == NULL)  // empty list does not have any name
	 || ((len = strlen(name)) == 0))
		return 0;

	for (uns pos = 1; *list; pos++, list++) {
		if ((len == strlen(*list))
		 && !_stricmp(name, *list))
			return pos;
	}

	return 0;
}


char *ScanForName(int minnest)
{
	int ch;
	static char buf[128];
	uns pos = 0;
	bool token = false;
	bool quoted = false;

	while ((ch = fgetc(rtfp)) != EOF) {
		if ((ch == '\n')
		 || (ch == '\r'))
			continue;

		if (quoted) {  // as in file name after field name
			if (ch == '"')
				break;
			buf[pos++] = ch;
			continue;
		}

		if (ch == 0x7D) {
			rtfnest--;
			if (token)
				break;
			if (rtfnest < minnest) // no name in group
				break;
		}
		else if (ch == 0x7B) {
			rtfnest++;
			if (token)
				break;
		}
		else if (ch == '\\') {
			GetRtfCtrl();  // discard ctrls
			if (token)
				break;
		}
		else if (ch == '"') {
			if (token)
				break;
			token = true;
			quoted = true;
			pos = 0;
		}
		else if (isalpha(ch)) {
			token = true;  // in case not quoted, like field name
			buf[pos++] = ch;
		}
		else if (token)  // punct or space not part of name, if unquoted
			break;
	}
	
	buf[pos] = '\0';
	return buf;
}


// end of exwmf.cpp

