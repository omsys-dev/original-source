// dtmaptx.cpp processes dita topic file text content, JHG
// Copyright (c) 2009 by Jeremy H. Griffith.  All rights reserved.

#include "dcappl.h"
#include "dcctlg.h"
#include "dcfili.h"
#include "dcfilp.h"
#include "dcini.h"
#include "dcelem.h"
#include "expat.h"
#include "d2m.h"
#include "drxmlfl.h"
#include "dwhtmfl.h"



char *D2M::PendingText = NULL;
long D2M::PendingLen = 0;



void
D2M::ProcChar(const char *str, int len, long depth)
{
	// collect topic title and shortdesc, ignore all else
	if (!InTopicShortdesc
	 && !InTopicTitle)
		return;

	char *txt = NULL;
	strln *sl = ProcStr(str, len, depth);
	str = sl->str;
	len = sl->len;
	delete sl;
	if (!len)
		return;

	if (PendingText) {
		txt = NewName(PendingText, PendingLen + len);
		memcpy(txt + PendingLen, str, len);
		DeleteName(PendingText);
		PendingText = txt;
		PendingLen += len;
	}
	else  // either starting, or resuming after inline
		PendingText = NewName(str, PendingLen = len);

	if (InTopicTitle)
		CurrTopicRef->navtitle = PendingText;
	if (InTopicShortdesc)
		CurrTopicRef->shdesc = PendingText;
}


strln *
D2M::ProcStr(const char *str, int len, long depth)
{
	static bool InWhite = false;
	static bool InElemText = false;
	static DCilist InText;
	static long lastdepth = 0;

	strln *sl = new strln;
	sl->str = (char *) str;
	sl->len = len;

	int i = 0;
	int j = 0;

	if (depth > lastdepth) {
		InText.replace(InElemText, lastdepth);
		InElemText = false;
	}
	else if (depth < lastdepth)
		InElemText = (bool) InText.find(depth);
	lastdepth = depth;

	char *s = (char *) str;
	int olen = len;

	for (i = 0; i < len; i++) {
		if (!isspace(s[i]))
			break;
	}

	for (j = len - 1; j >= 0; j--) {
		if (!isspace(s[j]))
			break;
	}

	if (i == len) { // all whitespace, ignore it
		if (!InElemText
		 || InWhite) {
			sl->len = 0;
			return sl;
		}
		s = " ";
		len = 1;
		InWhite = true;
	}
	else if (InWhite
	 && (i > 0)) {
		s += i;
		len -= i;
		InWhite = false;
	}
	else
		InWhite = false;

	if (j < (olen - 1)) {  // trailing whitespace
		InWhite = true;
		len -= ((olen - 1) - j) - 1;
	}

	InElemText = true;
	sl->str = s;
	sl->len = len;

	return sl;
}




// end of dtmaptx.cpp

