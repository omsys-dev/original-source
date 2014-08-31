// dclog.cpp contains code for the dcl logging classes, JHG
// Copyright (c) 1995-2008 by Omni Systems, Inc.  All rights reserved.

#include "dcglob.h"
#include "dcbase.h"
#include "dclist.h"
#include "dcfile.h"
#include "dcfilw.h"
#include "dclog.h"

#pragma warning( disable : 4244 4514 )


char DClog::lType[logmax] = { ' ', 'E', 'W', 'Q', 'I', 'D' };
char *DClog::lStart = "Starting log for ";
bool DClog::ErrWarn = false;


DClog::DClog(const char *lname, const char *mname,
 const char *ver, unc *llev, bool spc) : DCwfile(lname, ftxt, mwa)
{
	tStamp = time(NULL);
	tStr = NewName(26);

	if (llev) {
		lLevels = llev;
		for (unc i = logerr; i < logmax; i++) {
			if (lLevels[i] == 0)
				lLevels[i] = 1;
		}
	}
	else {
		lLevels = new unc[logmax];
		for (unc i = logerr; i < logmax; i++)
			lLevels[i] = 1;
	}


	if (spc) {
		putCr();
		putCr();
	}
	logTime(true);

	if (mname && ver)
		logEvent(loginfo, 1, lStart, mname, ", ", ver);
}

DClog::~DClog()
{
	DeleteName(tStr);
}


void DClog::logTime(bool nl)
{
	tStamp = time(NULL);
	strncpy(tStr, ctime(&tStamp), 25);
	putStr(tStr, 24);
	if (nl)
		putCr();
}


void DClog::logEvent(logtype lt, unc sev, const char *desc,
 const char *d2, const char *d3, const char *d4, const char *d5)
{
	static bool nocont = false;

	if (lt == logcont) { // continuation of last entry, indent
		if (nocont)
			return;
		putStr("    ");
	}
	else {
		if ((sev == 0)
		 || (sev > lLevels[lt])) {  // not severe enough to log
			nocont = true;
			return;
		}
		nocont = false;
		if (tStamp < time(NULL))  // record time if not repeat
			logTime();
		putChar(lType[lt]);  // E, W, Q, I, D
		putChar(sev + '0');  // 1 (worst) to 9
		putStr(": ");
	}

	if ((lt == logwarn)
	 || (lt == logerr))
		ErrWarn = true;

	putStr(desc);  // structure this more later
	putStr(d2);
	putStr(d3);
	putStr(d4);
	putStr(d5);
	putCr();
	fflush(fptr);
}


// log entries consist of:
//   timestamp, flush left on its own line, not repeated
//    until it is changed
//   event type, indented one space, single-char for Error,
//    Warning, Query, Info, Debug
//   event code, severity, 1=greatest, 9=least
//   code is followed by a short fixed description on same line
//   any more comments are indented two spaces, as many lines
//    as are needed to identify the problem location, with links
//    to the Frame file and output file if possible
//   file start events are logged iff an error occurs




// end of dclog.cpp

