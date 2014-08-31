// dtmapmp.cpp produces dita2map ditamap output, JHG
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



DClist D2M::TopicRefList;
topref *D2M::CurrTopicRef = NULL;

long D2M::MapDepth = 0;
char *D2M::MapRootElem = "map";
char *D2M::MapTitleElem = "title";
char *D2M::MapLanguage = "en-us";
bool D2M::InTopicrefStart = false;
char *D2M::PublicID = "\"-//OASIS//DTD DITA 1.1 Map//EN\"";
char *D2M::SystemID = " \"http://docs.oasis-open.org/dita/v1.1/OS/dtd/map.dtd\"";



// map writing functions


void
D2M::WriteMapHeader(void)
{
	MapFile->SetXml(true);
	MapFile->putXML("UTF-8");
	MapFile->putDT(MapRootElem, PublicID, SystemID, false, false);
	MapFile->startBlock(MapRootElem);
	MapFile->strAttr("xml:lang", MapLanguage);
	MapFile->endElem(true);
	MapFile->putEnd();
	if (MapTitle) {
		MapFile->startSBlock(MapTitleElem);
		MapFile->putTextStr(MapTitle);
		MapFile->endBlock(MapTitleElem);
		MapFile->putEnd();
	}		
	MapDepth = 0;
}



void
D2M::StartTopicref(const char *type, const char *id)
{
	if (InTopicrefStart)
		EndTopicrefStart(TopicDepth > MapDepth);

	while (MapDepth && (TopicDepth <= MapDepth))
		EndTopicrefBlock();

	CurrTopicRef = new topref;
	TopicRefList.add((long) CurrTopicRef);

	CurrTopicRef->type = NewName(type);
	CurrTopicRef->depth = TopicDepth;

	CurrTopicRef->href = NewName(TopicFileName,
	                             strlen(TopicFileName) + strlen(id) + 2);
	strcat(CurrTopicRef->href, "#");
	strcat(CurrTopicRef->href, id);

	MapFile->startBlock("topicref");
	MapFile->strAttr("href", CurrTopicRef->href);
	MapFile->strAttr("type", CurrTopicRef->type);
	InTopicrefStart = true;
	MapDepth++;
}


void
D2M::EndTopicrefStart(bool block)
{
	MapFile->strAttr("navtitle", CurrTopicRef->navtitle);
	MapFile->endElem(block);
	MapFile->putEnd();
	if (!block)
		MapDepth--;
	InTopicrefStart = false;
}


void
D2M::EndTopicrefBlock(void)
{
	MapFile->endBlock("topicref");
	MapDepth--;
	MapFile->putEnd();
}


void
D2M::WriteShortdesc(void)
{
	if (InTopicrefStart)
		EndTopicrefStart(true);
	MapFile->startSBlock("topicmeta");
	MapFile->startSBlock("shortdesc");
	MapFile->putTextStr(CurrTopicRef->shdesc);
	MapFile->endBlock("shortdesc");
	MapFile->endBlock("topicmeta");
	MapFile->putEnd();
}


void
D2M::WriteMapEnd(void)
{
	if (InTopicrefStart)
		EndTopicrefStart(false);
	while (MapDepth)
		EndTopicrefBlock();
	MapFile->endBlock(MapRootElem);
	MapFile->putEnd();
	LogEvent(loginfo, 1, "Wrote map file \"", MapFileName, "\"");
}



// end of dtmapmp.cpp

