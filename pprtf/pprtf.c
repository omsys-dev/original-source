// pprtf.c pretty-prints .rtf file as .txt file
// Copyright 2003 Omni Systems, Inc. All rights reserved.
// Licensed to all under the General Public License (GPL).
// usage: pprtf rtffile.rtf [newone.txt] (default rtffile.txt)

#include <stdio.h>
#include <windows.h>

#define MAXCW 128
#define MAXLN  65

FILE *rtf = NULL;
FILE *pp = NULL;
int pos = 0;
int lbrace = 0;
int lbopt = 0;
int color = 0;


void PutCR(void)
{
	if (pos == 0)
		return;

	fputc('\r', pp);
	fputc('\n', pp);
	pos = 0;
}

void PutLB(void)
{
	while (lbrace > 0) {
		fputc('{', pp);
		pos++;
		lbrace--;
	}
	if (lbopt) {
		fputc('\\', pp);
		fputc('*', pp);
		lbopt = 0;
		pos += 2;
	}
}

void ProcCW(void)
{
	static char cw[MAXCW + 1];
	char *cwp = cw;
	int ch = 0;

	ch = fgetc(rtf);
	if (!isalnum(ch)) {
		if (pos > MAXLN)
			PutCR();
		if (lbrace)
			PutLB();
		fputc('\\', pp);
		fputc(ch, pp);
		pos += 2;
		if (ch == '\'') {
			while (isxdigit(ch = fgetc(rtf))) {
				fputc(ch, pp);
				pos++;
			}
			ungetc(ch, rtf);
		}
		return;
	}
	*cwp++ = ch;

	while (((ch = fgetc(rtf)) != EOF)
	 && isalnum(ch)
	 && (cwp < &cw[MAXCW]))
		*cwp++ = ch;

	if (ch != ' ')
		ungetc(ch, rtf);
	*cwp = '\0';

	if (!strcmp(cw, "pard")
	 || !strcmp(cw, "fonttbl")
	 || !strcmp(cw, "field"))
		PutCR();

	if (!strcmp(cw, "colortbl"))
		color = 1;
	else if (!strcmp(cw, "stylesheet"))
		color = 0;

	if (pos > MAXLN)
		PutCR();
	if (lbrace)
		PutLB();
	fputc('\\', pp);
	fputs(cw, pp);
	pos += strlen(cw) + 1;
	if (ch == ' ') {
		fputc(' ', pp);
		pos++;
	}

	if (!strcmp(cw, "par"))
		PutCR();
}

void ProcFile(void)
{
	int ch = 0;
	int lastch = 0;

	pos = 0;

	while ((ch = fgetc(rtf)) != EOF) {
		switch (ch) {
			case '{':
				if (lastch == '}')
					PutCR();
				lbrace++;
				if ((ch = fgetc(rtf)) == '\\') {
					if ((ch = fgetc(rtf)) == '*') {
						lbopt = 1;
						break;
					}
					else
						ungetc(ch, rtf);
					goto codew;
				}
				else
					ungetc(ch, rtf);
				break;

			case '\\':
			codew:
				ProcCW();
				lastch = 0;
				break;

			case '}':
			case ' ':
				if (lbrace)
					PutLB();
				if (pos > MAXLN)
					PutCR();
				goto normal;

			case '\r':
			case '\n':
				break;

			case ';':
				if (color) {
					fputc(';', pp);
					PutCR();
					break;
				}
				goto normal;

			normal:
			default:
				if (lbrace)
					PutLB();
				fputc(ch, pp);
				lastch = ch;
				pos++;
				break;
		}
	}
	
	PutCR();
}


int main(int argc, char *argv[])
{
	char *rpath = NULL;
	char *ppath = NULL;
	char *newpath = NULL;
	char *cpos = NULL;

	//notify user of deleted files or usage
	switch (argc){
		case 3:
			ppath = argv[2];
			break;
		case 2:
			break;
		default:
			fprintf(stderr, "usage: pprtf rtffile.rtf [newone.txt]\n");
			return 4;
	}

	rpath = argv[1];

	// ensure full path is present
	if (strchr(rpath, '\\') == NULL) {
		newpath = (char *) calloc(strlen(rpath) + 3, sizeof(char));
		if (rpath[1] == ':') {
			newpath[0] = rpath[0];
			newpath[1] = rpath[1];
			newpath[2] = '.';
			newpath[3] = '\\';
			strcpy(newpath + 4, rpath + 2);
		}
		else {
			newpath[0] = '.';
			newpath[1] = '\\';
			strcpy(newpath + 2, rpath);
		}
		rpath = newpath;
	}

	// ensure file exists
	if ((rtf = fopen(rpath, "rb")) == NULL){
		fprintf(stderr, "Error 1: %s does not exist.\n", rpath);
		if (newpath)		
			free(newpath);
		return 1;
	}

	// create temp file path
	if (!ppath) {
		ppath = strdup(rpath);
		if ((cpos = strstr(ppath, ".rtf")) != NULL)
			strcpy(cpos + 1, "txt");
		else {
			fprintf(stderr, "Error 3: file %s must have .rtf ext.\n", ppath);
			if (newpath)		
				free(newpath);
			return 3;
		}
	}

	if ((pp = fopen(ppath, "wb")) == NULL){
		fprintf(stderr, "Error 2: cannot write file %s.\n", ppath);
		if (newpath)		
			free(newpath);
		return 2;
	}

	ProcFile();
	fclose(rtf);
	fclose(pp);
	if (newpath)		
		free(newpath);
	return 0;
}


//end of pprtf.c