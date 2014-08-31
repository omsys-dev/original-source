// dcmxl.cpp contains code for outout file automation, JHG
// Copyright (c) 2009 by Omni Systems, Inc.  All rights reserved.

#include "dcglob.h"
#include "dcbase.h"
#include "dclist.h"
#include "dcnode.h"
#include "dcfile.h"
#include "dcfilr.h"
#include "dcfilw.h"
#include "dclog.h"
#include "dcfili.h"
#include "dcfilp.h"
#include "dcini.h"
#include "dcmacr.h"
#include "dcauto.h"
#include "dcmxl.h"

/* system headers */
#include <stdio.h>
#include <time.h>
#include <direct.h>
#include <io.h>
#include <sys\stat.h>

// Windows specific
#include <windows.h>
#include <iostream.h>
#include "Shlwapi.h"



bool MXLwr::WriteKeydefLib = false;
bool MXLwr::UseKeydefAliases = false;

DCnlist MXLwr::OutputKeydefs;
long MXLwr::OutputKeydefCount = 0;
DCnlist MXLwr::SourceKeydefs;
long MXLwr::SourceKeydefCount = 0;
DCnlist MXLwr::KeydefAliases;
char *MXLwr::OutputKeydefLibPath = NULL;
char *MXLwr::OutputKeydefLibID = NULL;
char *MXLwr::OutputKeydefLibSrc = NULL;
char *MXLwr::KeylibOutput;
char *MXLwr::OutputKey;
char *MXLwr::OutputKeySrc;


// if WriteKeydefLib, in dwhtm.cpp:
// for each source keydef, use same keys for output keydef
// change the src from src file name to output file name
// call SetOutputKeydef for each keydef corrected
// if UseKeydefAliases, use CSH alias if any for file
// make sure lib src (KeydefLibSrc) ends with # for OH
// call WriteOutputKeydefLib at end


void 
MXLwr::SetOutputKeydef(char *keys, char *src)
{
	if (!OutputKeydefs.getitem(keys)) {
		OutputKeydefs.add(keys, (long) src);
		OutputKeydefCount++;
	}
}


void 
MXLwr::SetOutputKeydefLibProps(char *libsrc)
{
	OutputKeydefLibSrc = libsrc;
}


void 
MXLwr::WriteOutputKeydefLib(void)
{
	if (!OutputKeydefLibPath
	 || !OutputKeydefCount)
		return;

	DCwfile *wf = new DCwfile(OutputKeydefLibPath,fxml);
	if (wf->FErr() != fok)
		return;

	// write lib root with @id and #src

	wf->putStr("<lib ");
	if (OutputKeydefLibID) {
		wf->putStr("id=\"");
		wf->putStr(OutputKeydefLibID);
		wf->putStr("\" ");
	}
	if (OutputKeydefLibSrc) {
		wf->putStr("src=\"");
		wf->putStr(OutputKeydefLibSrc);
		wf->putStr("\" ");
	}
	wf->putStr(">");
;	wf->putCr();

	// write comment with filename and output
	wf->putStr("<!-- ");
	wf->putStr(OutputKeydefLibPath);
	wf->putStr(" is the output keydef library for ");
	wf->putStr(KeylibOutput);
	wf->putStr(" -->");
	wf->putCr();
	wf->putCr();

	// write all key elements with @keys, and @src
	// for OH, make src the topicalias;
	// for others, filename to append to root @src
	// keys has items with name=key, id=src
	DCnlist *dnl = &OutputKeydefs;
	while ((dnl = dnl->next) != NULL) {
		wf->putStr("<key ");
		wf->putStr("keys=\"");
		wf->putStr(dnl->name);
		wf->putStr("\" ");
		wf->putStr("src=\"");
		wf->putStr((char *) dnl->id);
		wf->putStr("\" />");
		wf->putCr();
	}

	wf->putCr();
	wf->putStr("</lib>");
	wf->putCr();
	wf->putCr();
	wf->CloseFile();
	delete wf;
}


// end of dcmxl.cpp

