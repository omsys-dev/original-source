// dtmapel.cpp processes dita topic file elements, JHG
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



long D2M::TopicDepth = 0;
char *D2M::TopicRootElem = NULL;
char *D2M::TopicTitleElem = NULL;
char *D2M::TopicShortdescElem = NULL;
DCnlist D2M::TopicRoots;
bool D2M::InTopicTitle = false;
bool D2M::InTopicShortdesc = false;



// callbacks from expat


void
D2M::StartElement(const char *name, const char **atts, long depth)
{
	const char *classatt = NULL;
	const char *idatt = NULL;
	attrp alist[] = {
	 {"class", &classatt },
	 {"id", &idatt },
	 {NULL, NULL }
	};

	ProcAttrs(atts, alist);

	// detect topic start and increment TopicDepth
	// get topic type (root name) and id, and StartTopicref
	if (strstr(classatt, " topic/topic ")) {
		TopicDepth++;
		TopicRootElem = NewName(name);
		TopicRoots.add(TopicRootElem, TopicDepth);
		StartTopicref(TopicRootElem, idatt);
		return;
	}

	// at start of topic title, set InTopicTitle
	if (strstr(classatt, " topic/title ")) {
		if (CurrTopicRef
		 && !CurrTopicRef->navtitle) {
			InTopicTitle = true;
			TopicTitleElem = NewName(name);
		}
		return;
	}

	// at start of shortdesc, set InTopicShortdesc
	if (UseTopicShortdesc
	 && strstr(classatt, " topic/shortdesc ")) {
		InTopicShortdesc = true;
		TopicShortdescElem = NewName(name);
		return;
	}
}


void 
D2M::ProcAttrs(const char **attp, attrp *alist)
{
	long i = 0;

	while (*attp) {
		for (i = 0; alist[i].name != NULL; i++) {
			if (!strcmp(*attp, alist[i].name)) {
				*(alist[i].var) = *(attp + 1);
				break;
			}
		}
		attp += 2;
	}
}


void
D2M::EndElement(const char *name, long depth)
{
	// detect topic end based on name and depth
	// and decrement TopicDepth
	// do not end topicref block, may have nested content
	if (TopicRootElem
	 && !strcmp(name, TopicRootElem)) {
		TopicRoots.remove(TopicDepth);
		DeleteName(TopicRootElem);
		if (--TopicDepth)
			TopicRootElem = (char *) TopicRoots.find(TopicDepth);
		else
			TopicRootElem = NULL;
		return;
	}

	// at end of topic title, EndTopicrefStart
	if (InTopicTitle
	 && TopicTitleElem
	 && !strcmp(name, TopicTitleElem)) {
		InTopicTitle = false;
		DeleteName(TopicTitleElem);
		TopicTitleElem = NULL;
		PendingText = NULL;
		PendingLen = 0;
		//EndTopicrefStart();
		return;
	}

	// at end of shortdesc, WriteShortdesc
	if (InTopicShortdesc
	 && TopicShortdescElem
	 && !strcmp(name, TopicShortdescElem)) {
		InTopicShortdesc = false;
		DeleteName(TopicShortdescElem);
		TopicShortdescElem = NULL;
		PendingText = NULL;
		PendingLen = 0;
		WriteShortdesc();
		return;
	}
}


void
D2M::ProcPI(const char *target, const char *data)
{
	// ignore PIs for now
}




// end of dtmapel.cpp

