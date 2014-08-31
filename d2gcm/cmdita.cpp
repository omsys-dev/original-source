/////////////////////////////////////////////////////////////////////////////
// Name:        cmdita.cpp
// Purpose:     CM strings specific to DITA
// Author:      Jeremy H. Griffith
// Modified by: 
// Created:     04/21/2013
// RCS-ID:      
// Copyright:   (c) Copyright 2013 Omni Systems CIA. LTDA.
// Licence:     
/////////////////////////////////////////////////////////////////////////////

// For compilers that support precompilation, includes "wx/wx.h".
#include "wx/wxprec.h"

#ifdef __BORLANDC__
#pragma hdrstop
#endif

#ifndef WX_PRECOMP
#include "wx/wx.h"
#endif

#include "d2gcmapp.h"
#include "wx/bookctrl.h"
#include "wx/msgdlg.h"
#include "wx/filedlg.h"

#include "configmgr.h"
#include "d2gcmapp.h"

#pragma warning( disable : 4089 4189 4710)

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
#include "dctxfx.h"
#include "dcfmtr.h"

#include "cmdcl.h"


#ifndef _M2GCM

char *CMgr::CMIniName = "%omsyshome%\\d2g\\local\\manager\\local_d2gcm.ini";
char *CMgr::SysIniName = "%omsyshome%\\d2g\\local\\manager\\local_d2g_settings.ini";
char *CMgr::ProjIniName = "%omsyshome%\\d2g\\system\\projects.ini";
// note inclusion of trailing backslash below
char *CMgr::LocalStartDir = "%omsyshome%\\d2g\\local\\starts\\";
char *CMgr::SystemStartDir = "%omsyshome%\\d2g\\system\\starts\\";
char *CMgr::HelpCommand = "file://%omsyshome%/d2g/usersguide/_ugdita2go.htm#";
char *CMgr::CHMPathSet = "%omsyshome%\\d2g\\usersguide\\ugdita2go.chm";

char *CMgr::CMexePath = "%omsyshome%\\common\\bin\\d2gcm.exe";
char *CMgr::CMResPath = "%omsyshome%\\d2g\\local\\manager\\cmresult.txt";
char *CMgr::CMProgName = "d2gcm";
char *CMgr::CMVersion = "cmd000b";
char *CMgr::AltHelpCommand = "file://%omsyshome%/d2g/usersguide/_ugdita2go.htm#";

#else

char *CMgr::CMIniName = "%omsyshome%\\m2g\\local\\manager\\local_m2gcm.ini";
char *CMgr::SysIniName = "%omsyshome%\\m2g\\local\\manager\\local_m2g_settings.ini";
char *CMgr::ProjIniName = "%omsyshome%\\m2g\\system\\projects.ini";
// note inclusion of trailing backslash below
char *CMgr::LocalStartDir = "%omsyshome%\\m2g\\local\\starts\\";
char *CMgr::SystemStartDir = "%omsyshome%\\m2g\\system\\starts\\";
char *CMgr::HelpCommand = "file://%omsyshome%/m2g/usersguide/_ugmif2go.htm#";
char *CMgr::CHMPathSet = "%omsyshome%\\m2g\\usersguide\\ugmif2go.chm";

char *CMgr::CMexePath = "%omsyshome%\\common\\bin\\m2gcm.exe";
char *CMgr::CMResPath = "%omsyshome%\\m2g\\local\\manager\\cmresult.txt";
char *CMgr::CMProgName = "m2gcm";
char *CMgr::CMVersion = "cmm000b";
char *CMgr::AltHelpCommand = "file://%omsyshome%/m2g/usersguide/_ugmif2go.htm#";

#endif

// end of cmdita.cpp


