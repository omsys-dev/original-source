
// dcfile.cc contains code for the dcl file classes, JHG
// Copyright (c) 1993-2007 by Omni Systems, Inc.  All rights reserved.

#include "dcglob.h"
#include "dcfile.h"

#pragma warning( disable : 4244 4514 )

DCfile::DCfile(const char *fn, dc_fmod fm)
{
	fmode = fm;
  fname = NULL;

	OpenFile(fn, fm);
}


void
DCfile::OpenFile(const char *fn, dc_fmod fm)
{
  Ferr = fok;

	if (fm == mno)
		fm = fmode;

  if (fn == NULL) {
    fptr = (fm == mwr) ? stdout : stdin;
		return;
	}

  fname = NewName(fn);

  char *fmstr = NULL;
	switch (fm) {
		case mwr:
			fmstr = "wb";
			break;

		case mrd:
			fmstr = "rb";
			break;

		case mwa:
			fmstr = "a+b";
			break;

		case mno:
		default:
			fmstr = "rwb";
			break;
	}

  if (*fn == '\0') {
    Ferr = fno;
		return;
	}

	char *pct = NULL;
	char *epct = NULL;
	char *enname = NULL;
	char *enval = NULL;
	char *nfn = NULL;
	uns len = 0;
	while ((pct = strchr(fn, '%')) != NULL) {  // environment variables present
		if ((epct = strchr(pct + 1, '%')) > (pct + 1)) {
			len = (epct - pct) - 1;
			enname = NewName(pct + 1, len);
			if ((enval = ::getenv(enname)) != NULL) {
				nfn = NewName(strlen(fn) + strlen(enval) - (len + 2));
				if (fn > pct)
					strncpy(nfn, fn, pct - fn);
				strcat(nfn, enval);
				strcat(nfn, epct + 1);
				fn = fname = nfn;
				DeleteName(enname);
			}
			else {
				DeleteName(enname);
				break;
			}
		}
		else
			break;
	}

	GetBaseName(fname, basepath, basename);

	if ((fptr = fopen(fn, fmstr)) == NULL) {
    Ferr = fno;
		return;
	}
}


void
DCfile::ReopenFile(const char *fn, bool close)
{
	if (close)
		CloseFile();
	OpenFile(fn);
}

void
DCfile::CloseFile()
{
	if (fptr) {
		fclose(fptr);
		fptr = NULL;
	}
  if (fname) {
    DeleteName(fname);
		fname = NULL;
	}
}


DCfile::~DCfile()
{
  if (fptr)
    fclose(fptr);
  if (fname)
    DeleteName(fname);
}


int
DCfile::seek(long pos, int mode)
{
	if (Ferr)
		return -1;
	return fseek(fptr, pos, mode);
}

long 
DCfile::tell()
{
	if (Ferr)
		return -1;
	return ftell(fptr);
}

long
DCfile::getSize(void)
{
	if (Ferr)
		return 0L;

	long pos = ftell(fptr);
	if (pos == -1)
		return 0L;

	if (fseek(fptr, 0L, SEEK_END) != 0)
		return 0L;

	long epos = ftell(fptr);
	if (epos != pos)
		fseek(fptr, pos, SEEK_SET);

	return epos;
}
 

/* end of dcfile.cc */

