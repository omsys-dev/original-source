// DITA2map.cpp derives ditamaps from dita topic files, JHG
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



// start of code

char *D2M::D2MVer = { "d001c" };


swlist D2M::SwitchList[] = {
  { "T", NULL, D2M::SetMapTitle, true },
  { "t", NULL, D2M::SetMapTitle, true },
  { "D", NULL, D2M::SetDTDPath, true },
  { "d", NULL, D2M::SetDTDPath, true },
  { "I", NULL, D2M::SetIniName, true },
  { "i", NULL, D2M::SetIniName, true },
  { NULL, NULL }
};

short D2M::Verbose;

char *D2M::TopicFileName = NULL;
char *D2M::MapFileName = NULL;
char *D2M::IniFileName = "dita2map.ini";
char *D2M::MapTitle = NULL;
char *D2M::DTDPath = NULL;
bool D2M::MapTitleSet = false;
bool D2M::DTDPathSet = false;
bool D2M::UseTopicShortdesc = true;

XMLrfile *D2M::TopicFile = NULL;
HTMwfile *D2M::MapFile = NULL;
DCirfile *D2M::IniFile = NULL;


int main(int argc, char* argv[])
{

  DCapp app(dn, ".dita", ".ditamap");

  app.Init(argc, argv);

  if (!app.Proc(D2M::SwitchList)) {
    fprintf(stderr, "usage: %s [-v] [-t \"Map Title\"] [-d \"D:\\path\\to\\DTDs\"]"
			"\n        [-i inifile.ini] [-o mapfile.ditamap] topicfile.dita\n", argv[0]);
    exit(dc_err_arg);
  }

  D2M::Verbose = app.verbose();

  XMLrfile rf(D2M::TopicFileName = app.rfile());
  if (rf.FErr() != fok)
    rf.Err();
	else
		D2M::TopicFile = &rf;

  HTMwfile wf(D2M::MapFileName = app.wfile());
  if (wf.FErr() != fok)
    wf.Err();
	else
		D2M::MapFile = &wf;

	// look in current directory for correct ini file
	DCirfile *inif = new DCirfile(D2M::IniFileName);

	if (inif->FErr() != fok) {
		delete inif;
		inif = new DCirfile("xml2htm.ini");
	}

	if (inif->FErr() != fok) {
		delete inif;
		inif = new DCirfile("xml2rtf.ini");
	}

	if ((inif->FErr() != fok)
	 && strcmp(D2M::IniFileName, "dita2map.ini")) {
		delete inif;
		inif = new DCirfile("dita2map.ini");
	}

	if (inif->FErr() == fok)
		D2M::IniFile = inif;  // make accessible everywhere

	D2M::InitCurrProps(); // set up vars per ini file


	D2M::ProcessTopic();

  rf.CloseFile();
  wf.CloseFile();
  wf.Err();      // if error, exits with the err (and a message)
  return dc_ok;  // so if we get here, it worked
}


bool
D2M::FileExists(char *fname)
{
	if (!fname || (*fname == '\0'))
		return false;

	DCrfile *rf = new DCrfile(fname);
	if (rf->FErr() != fok)
		return false;

  rf->CloseFile();
	delete rf;

	return true;
}


void
D2M::SetMapTitle(char *name)
{
	MapTitle = NewName(name);
	MapTitleSet = true;
}


void
D2M::SetDTDPath(char *name)
{
	DTDPath = NewName(name);
	DTDPathSet = true;
}


void
D2M::SetIniName(char *name)
{
	IniFileName = NewName(name);
}



// ini processing and logging


bool D2M::CaselessMatch = true;
bool D2M::SpacelessMatch = true;
bool D2M::WildcardMatch = true;
char *D2M::ConfigTemplate = NULL;

bool D2M::UseLog = true;
char *D2M::LogFileName = "DITA2map_log.txt";
DClog *D2M::Log = NULL;
unc D2M::LogLevels[logmax] = { 0, 1, 1, 1, 1, 0 };



void
D2M::InitCurrProps(void)
{
	if (IniFile) {
		// set IniFile to point to top of config chain
		IniFile = IniFile->SetConfigTemplate("Setup", "ConfigTemplate");

		DCini::InitIniModule(IniFile, D2MIniSects);

		if (IniFile->Section("Options")) {	// read ini-interpretation info
			if (IniFile->Find("CaselessMatch"))
				IniFile->SetMatch(1, IniFile->BoolRight());
			if (IniFile->Find("SpacelessMatch"))
				IniFile->SetMatch(2, IniFile->BoolRight());
			if (IniFile->Find("WildcardMatch"))
				IniFile->SetMatch(3, IniFile->BoolRight());
			if (IniFile->Find("NoNameDel"))
				::NoNameDel = IniFile->BoolRight();
			if (IniFile->Find("NoMemDel"))
				::NoMemDel = IniFile->BoolRight();
		}

		// command-linr options override .ini settings

		char *mtitle = NULL;
		char *dpath = NULL;

		if (MapTitleSet)
			mtitle = MapTitle;
		if (DTDPathSet)
			dpath = DTDPath;

		DCini::ProcAllSects();

		if (mtitle)
			MapTitle = mtitle;
		if (dpath)
			DTDPath = dpath;
	}

	if (UseLog) {
		Log = new DClog(LogFileName, "DITA2Map", D2MVer, LogLevels);
		if (Log->FErr() == fok)
			Log->logEvent(loginfo, 1, "Opened DITA file \"", TopicFileName, "\"");
		else
			UseLog = false;
		XMLrfile::LogEventFunc = LogEvent;
	}
}



void
D2M::LogEvent(logtype lt, unc sev, const char *desc,
 const char *d2, const char *d3, const char *d4, const char *d5)
{
	if (!UseLog)
		return;

	Log->logEvent(lt, sev, desc, d2, d3, d4, d5);
}



// topic processing


void
D2M::ProcessTopic(void)
{
	XMLrfile::LocalDTDPath = DTDPath;
	TopicFile->SetHandlers(StartElement, EndElement, ProcChar, ProcPI);

	WriteMapHeader();
	TopicFile->ParseDoc();  // parses entire doc
	WriteMapEnd();
}




// end of DITA2map.cpp

