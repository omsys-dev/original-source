// dwrtfpp.cpp is the code for dcl RTF private profiles, JHG
// Copyright (c) 2004 by Omni Systems, Inc.  All rights reserved.

#include "dcappl.h"
#include "dcctlg.h"
#include "dcfili.h"
#include "dctxfx.h"
#include "dcfmtr.h"
#include "dcfilp.h"
#include "dcini.h"
#include "dcmacr.h"
#include "dwrtf.h"
#include "dwrtffl.h"


char RTFwr::RefBuf[RefBufLen];


char *
RTFwr::GetFileID(char *fn)
{
	if (!fn || (*fn == '\0'))
		return "";

	char *name = BaseName(fn);
	char *px = NULL;

	DCpfile::getval("FileIDs", name, "", RefBuf, RefBufLen, IDFileName);
	if (*RefBuf == '\0') {
		DCpfile::getval("FDK", "NextFileID", "", RefBuf, RefBufLen, IDFileName);
		if (*RefBuf == '\0')
			strcpy(RefBuf, "aa");

		// check to see if next ID is already in use, ugly reverse lookup
		while (FileIDUsed(RefBuf)) {
			if (!IncAlpha(RefBuf))
				strcat(RefBuf, "a");
		}
		px = NewName(RefBuf);
		DCpfile::putval("FileIDs", name, RefBuf, IDFileName);
		if (!IncAlpha(RefBuf))
			strcat(RefBuf, "a");
		DCpfile::putval("FDK", "NextFileID", RefBuf, IDFileName);
	}
	else
		px = NewName(RefBuf);

	DeleteName(name);
	return px;
}


char *
RTFwr::BaseName(char *fn, size_t len)
{
	char *name = NewName(fn, len);

  char *cpt = strrchr(name, '/');	// remove any path
	char *bpt = strrchr(name, '>'); // and any Frame path codes
	if (bpt > cpt)
		cpt = bpt++;
	if (!cpt)
		cpt = name;
	else
		cpt++;

	StripSuf(name);

	char *base = NewName(cpt);
	DeleteName(name);
	return base;
}


bool
RTFwr::IncAlpha(char *str)
{
	if (!str || (*str == '\0'))
		return false;

	bool done = false;

	for (char *cpt = str + strlen(str) - 1; (!done) && (cpt >= str); cpt--) {
		switch (*cpt) {
			case 'z':
				*cpt = 'a';
				break;
			case 'Z':
				*cpt = 'A';
				break;
			case '9':
				*cpt = '0';
				break;
			case 'k':  // skip lower-case L
				*cpt = 'm';
				done = true;
				break;
			case 'N':  // and upper-case O
				*cpt = 'P';
				done = true;
				break;

			default:
				if (isalnum(*cpt)) {
					(*cpt)++;
					done = true;
				}
				break;			
		}
	}

	return done;  // false for overflow
}


bool
RTFwr::FileIDUsed(char *id)
{
	char *fid = NewName(id);
	char *buf = new char[MaxIniLen];
	char *bpt = buf;
	bool rval = false;

	DCpfile::getval("FileIDs", NULL, "", buf, MaxIniLen, IDFileName);
	while (*bpt) {
		DCpfile::getval("FileIDs", bpt, "", RefBuf, RefBufLen, IDFileName);
		if (!stricmp(fid, RefBuf)) {
			rval = true;
			break;
		}
		while (*bpt)
			bpt++;
		bpt++;
	}

	strcpy(id, fid);  // restore original
	delete [] buf;
	return rval;
}

// end of dwrtfpp.cpp

