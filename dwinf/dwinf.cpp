 
// dwinf.cc is the code for dcl Microsoft INF writing, JHG
// Copyright (c) 1998 by Omni Systems, Inc.  All rights reserved.

#include "dcappl.h"
#include "dcctlg.h"
#include "dclist.h"
#include "dcfilw.h"
#include "dcfili.h"
#include "dwinf.h"
#include "dwinffl.h"



// start of code


short INFwr::Verbose;
DCirfile *INFwr::IniFile = NULL;


#ifdef _DLL

#define DllExport extern "C" __declspec( dllexport)

DllExport int
DCLmain(int argc, char* argv[])

#else

int
main(int argc, char *argv[])

#endif
{
  DCapp app(dw, ".inf");

  app.Init(argc, argv);

  if (!app.Proc()) {
    fprintf(stderr, "usage: %s [-v] [-o inf_file] dcl_file\n", argv[0]);
    exit(dc_err_arg);
  }

  INFwr::Verbose = app.verbose();

  DCLrfile rf(app.rfile());
  if (rf.FErr() != fok)
    rf.Err();

	strncpy(INFwr::BaseName, rf.getBaseName(), FileNameMax);

  INFwfile wf(app.wfile());
  if (wf.FErr() != fok)
    wf.Err();

	DCirfile inif("dwinf.ini");	// looks in current directory
	if (inif.FErr() == fok)
		INFwr::IniFile = &inif;  // make accessible everywhere

  INFwr::ReadDCL(&rf);   // read entire dcl file
  INFwr::WriteINF(&wf);   // put out the INF file

	inif.CloseFile();	// no need to check ini file for errors
  rf.CloseFile();
  wf.CloseFile();
  rf.Err();      // these two just return if no err
  wf.Err();      // otherwise they exit with the err (and a message)
  return dc_ok;  // so if we get here, it worked
}



// set up the list of ctls


inf INFwr::INFpile[] = {

	// groups to look inside
  { group,    1, 40, 1, nothing},   // defaults set
  { group,    1, 40, 2, nothing},   // resource set
  { group,    2,  1, 0, nothing},   // text segment
  { group,    2, 10, 7, nothing},   // xref group
  { group,    2, 50, 0, nothing},   // table start
  { group,    2, 52, 0, nothing},   // column start
  { group,    2, 54, 0, nothing},   // row start
  { group,    2, 56, 0, nothing},   // cell start
  { group,    2, 58, 3, nothing},   // table title start
  { group,    2, 71, 0, nothing},   // footnote start
  { group,    2, 80, 0, nothing},   // format def start
  { group,    3, 10, 0, nothing},   // page start
  { group,    3, 20, 0, nothing},   // frame start
  { group,    4,  1, 0, nothing},   // graphic group start
  { group,    4, 10, 0, nothing},   // graph obj start

	// things to look for
  { ename,    2, 15, 1, fontlist},  // named font
  { imnem,    2, 15, 1, fontlist},  // numbered font
  { imnem,    2, 81, 2, styletype}, // format type
  { ename,    2, 81, 1, stylelist}, // format name
  { ename,    3, 21, 1, framelist}, // ref frame name
  { ename,    2,  6, 1, flowlist},  // flow tag
  { ename,    1, 50, 3, grfile},    // graphic file name
  { ename,    2, 96, 5, xrfile},    // xref file name
  { etext,    2, 96, 5, xrfile},    // xref file name error
	{ etext,    2,112, 3, xrfile}     // hypertext file ref
};

size_t INFwr::INFsize;
DCvlist *INFwr::INFlist[InfListMax];


void
INFwr::SetUpINF(void)
{
  size_t i;
  inf *rtp;
  uns idx;
  DCvlist *dtlist;

  INFsize = sizeof(INFpile) / sizeof(inf);
  for (i = 0, rtp = INFpile; i < INFsize; i++, rtp++) {
    idx = ((rtp->ctlmaj - 1) * MinorMax) + rtp->ctlmin;
    if (idx >= InfListMax)
      continue;
    if (INFlist[idx] == NULL)
      INFlist[idx] = new DCvlist;
    if ((dtlist = (DCvlist *) INFlist[idx]->find(rtp->ctldat + 1)) == NULL) {
      dtlist = new DCvlist;
      INFlist[idx]->add(dtlist, rtp->ctldat + 1);
    }
    if (dtlist->find(rtp->ctldt) == NULL)
      dtlist->add(rtp, rtp->ctldt);
  }
}


void
INFwr::FindINF(INFctl *cp)
{
  uns idx;
  DCvlist *dlist, *dtlist;

  idx = ((cp->maj() - 1) * MinorMax) + cp->min();
  if ((idx < InfListMax)
   && ((dlist = (DCvlist *) INFlist[idx]) != NULL)) {
    if (((dtlist = (DCvlist *) dlist->find(cp->dat() + 1)) != NULL)
     || ((dtlist = (DCvlist *) dlist->find(1)) != NULL)) {
      if ((cp->rp = (inf *) dtlist->find(cp->dt())) != NULL)
        return;
      if (cp->dt() == imnem) { // fuzzy match
        if ((cp->rp = (inf *) dtlist->find(ilong)) != NULL)
          return;
      }
      else if (cp->dt() == ilong) {
        if ((cp->rp = (inf *) dtlist->find(imnem)) != NULL)
          return;
      }
    }
  }
  cp->rp = NULL;
}



// process the DCL file


long INFwr::ObjectID = 0;
long INFwr::StyleType = 0;
INFctl *INFwr::StyleName = NULL;

DCnlist INFwr::FontList;
DCnlist INFwr::StyleList;
DCnlist INFwr::PStyleList;
DCnlist INFwr::CStyleList;
DCnlist INFwr::FrameList;
DCnlist INFwr::FlowList;
DCnlist INFwr::GrFileList;
DCnlist INFwr::XrFileList;

char *INFwr::FontNames[FontNamesMax] = {
  "undefined", "unknown", "Times", "Helvetica", 
  "Courier", "AvantGarde", "Bookman", "Garamond",
  "Helvetica-Black", "Helvetica-Light", "Helvetica-Narrow", "Korinna",
  "NewCenturySchlbk", "Palatino", "ZapfChancery", "ZapfDingbats",
  "Symbol", "Webdings"
};

void
INFwr::ReadDCL(DCLrfile *rf)
{
  INFctl *cp;

  SetUpINF();  // prepare the search array

  while (!rf->eof()) {
    cp = new INFctl;
    *cp = *rf;

    FindINF(cp);            // sets cp->rp to point at matching INF struct
    if (cp->rp == NULL) {   // or to NULL if no match
      if (cp->dt() == group)
        rf->skipGroup();
      delete cp;
      continue;
    }

    switch((cp->rp)->scan_op) {
      case nothing:      // do not process, but check inside if group
				delete cp;
        break;

      case fontlist:
				if (cp->dt() == ename)
					AddToList(&FontList, cp);
				else if (cp->siz() < FontNamesMax)
					AddToList(&FontList, cp, FontNames[cp->siz()]);
				else
					delete cp;
				break;

			case styletype:
				StyleType = cp->siz();
				if (StyleName) {   // if style type follows name
					if (StyleType < 3) {
						AddToList(&StyleList, StyleName);
						AddToList((StyleType == 1) ? &PStyleList : &CStyleList, StyleName);
					}
					else
						delete StyleName;
					StyleType = 0;
					StyleName = NULL;
				}
				delete cp;
				break;

      case stylelist:
				if (StyleName)
					delete StyleName;
				StyleName = cp;
				if (StyleType) {   // if style name follows type
					if (StyleType < 3) {
						AddToList(&StyleList, StyleName);
						AddToList((StyleType == 1) ? &PStyleList : &CStyleList, StyleName);
					}
					else
						delete StyleName;
					StyleType = 0;
					StyleName = NULL;
				}
        break;

      case framelist:
				AddToList(&FrameList, cp);
        break;

      case flowlist:
				AddToList(&FlowList, cp);
        break;

      case grfile:
				AddToList(&GrFileList, cp, StripPath(cp));
        break;

      case xrfile:
				AddToList(&XrFileList, cp, StripPath(cp));
        break;

      default:
        assert(0);
        return;
    }
  }

	// clean up leftovers
	if (StyleName)
		delete StyleName;
}


void
INFwr::AddToList(DCnlist *nl, INFctl *cp, char *str)
{
	if (str == NULL)
		str = (char *) cp->ex();

	if ((*nl)(str) == 0)   // not in list already, add it
		nl->addsorted(str, ++ObjectID);
	//else
	//	delete cp;
}


char *
INFwr::StripPath(INFctl *cp)
{
	if ((cp->dt() != ename)
	 && (cp->dt() != etext))
		return NULL;

	char *fn = strrchr((char *) cp->ex(), '>');
	if (fn == NULL)
		fn = strrchr((char *) cp->ex(), '/');
	if (fn == NULL)
		fn = strrchr((char *) cp->ex(), '\\');
	if (fn == NULL)
		return (char *) cp->ex();
	return fn + 1;
}


// write the INF file


char INFwr::BaseName[FileNameMax];


void
INFwr::WriteINF(INFwfile *wf)
{
	// write .inf comment with name of file embedded
	wf->putComment("DCL .inf file for: ");
	wf->putStr(BaseName);
	wf->putEnd();

	// write each .inf section:
	//  fonts, styles, frames (from ref page), flows
	//  graph files, xref files
	WriteListSection(wf, "Fonts",	&FontList);
	WriteListSection(wf, "Styles",	&StyleList);
	WriteListSection(wf, "CharStyles",	&CStyleList);
	WriteListSection(wf, "ParaStyles",	&PStyleList);
	WriteListSection(wf, "RefFrames",	&FrameList);
	WriteListSection(wf, "TextFlows",	&FlowList);
	WriteListSection(wf, "GraphFiles",	&GrFileList);
	WriteListSection(wf, "XrefFiles",	&XrFileList);

	// xref and hyper targets accessible from other files

	// problems: tags not in catalogs, unknown facet types
	//  but that is really drmif's job; we can't see them here
}


void
INFwr::WriteListSection(INFwfile *wf, char *str, DCnlist *nl)
{
	wf->putSection(str);
	nl->writeall(wf, WriteKeys);
	wf->putEnd();
}


void
INFwr::WriteKeys(DCwfile *dwf, char *str, long i)
{
	if (i == 0)
		return;

  INFwfile *wf = (INFwfile *) dwf;
	wf->putKey(str);           // including the '='
	wf->putShort((short) i);   // our ObjectID
}



// end of dwinf.cpp


