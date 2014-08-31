/////////////////////////////////////////////////////////////////////////////
// Name:        d2gcmapp.cpp
// Purpose:     
// Author:      Jeremy H Griffith
// Modified by: 
// Created:     12/03/2012 14:03:16
// RCS-ID:      
// Copyright:   Copyright (c) 2012 Omni Systems
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

////@begin includes
////@end includes

#include "d2gcmapp.h"

#pragma warning( disable : 4089 )


////@begin XPM images
////@end XPM images


/*
 * Application instance implementation
 */

////@begin implement app
IMPLEMENT_APP( D2gcmApp )
////@end implement app


/*
 * D2gcmApp type definition
 */

IMPLEMENT_CLASS( D2gcmApp, wxApp )


/*
 * D2gcmApp event table definition
 */

BEGIN_EVENT_TABLE( D2gcmApp, wxApp )

////@begin D2gcmApp event table entries
////@end D2gcmApp event table entries

END_EVENT_TABLE()


/*
 * Constructor for D2gcmApp
 */

D2gcmApp::D2gcmApp()
{
    Init();
}


/*
 * Member initialisation
 */

void D2gcmApp::Init()
{
////@begin D2gcmApp member initialisation
////@end D2gcmApp member initialisation
}

const wxCmdLineEntryDesc D2gcmApp::g_cmdLineDesc[] =
{
	{wxCMD_LINE_SWITCH, wxT("h"), wxT("help"),
	 wxT("displays command-line help") },
	{wxCMD_LINE_OPTION, wxT("s"), wxT("start"),
	 wxT("specify the starting file") },
	{wxCMD_LINE_OPTION, wxT("c"), wxT("category"),
	 wxT("specify the category to use") },
	{wxCMD_LINE_OPTION, wxT("msect"), wxT("matchsect"),
	 wxT("specify the section match if any") },
	{wxCMD_LINE_OPTION, wxT("mset"), wxT("matchset"),
	 wxT("specify the setting match if any") },
	{wxCMD_LINE_OPTION, wxT("a"), wxT("action"),
	 wxT("specify the action to initiate") },
	{wxCMD_LINE_OPTION, wxT("sect"), wxT("section"),
	 wxT("specify the section to select") },
	{wxCMD_LINE_OPTION, wxT("set"), wxT("setting"),
	 wxT("specify the setting to select") },
	{wxCMD_LINE_OPTION, wxT("is"), wxT("inisource"),
	 wxT("specify the ini for move/copy source") },
	{wxCMD_LINE_OPTION, wxT("id"), wxT("inidest"),
	 wxT("specify the ini to select as destination") },
	{wxCMD_LINE_OPTION, wxT("r"), wxT("result"),
	 wxT("specify a file for results of selection"
	     " (for internal use only)") },

	{wxCMD_LINE_NONE }
};

wxString D2gcmApp::StartFile;
wxString D2gcmApp::Category;
wxString D2gcmApp::SectMatch;
wxString D2gcmApp::SetMatch;
wxString D2gcmApp::Action;
wxString D2gcmApp::Section;
wxString D2gcmApp::Setting;
wxString D2gcmApp::IniFileSrc;
wxString D2gcmApp::IniFileDest;
wxString D2gcmApp::ResultFile;


/*
 * Initialisation for D2gcmApp
 */

bool D2gcmApp::OnInit()
{    

#if wxUSE_XPM
	wxImage::AddHandler(new wxXPMHandler);
#endif
#if wxUSE_LIBPNG
	wxImage::AddHandler(new wxPNGHandler);
#endif
#if wxUSE_LIBJPEG
	wxImage::AddHandler(new wxJPEGHandler);
#endif
#if wxUSE_GIF
	wxImage::AddHandler(new wxGIFHandler);
#endif

	// process command line

	wxCmdLineParser cmdParser(g_cmdLineDesc, argc, argv);
	int res = cmdParser.Parse(false);

	if ((res == -1)
	 || (res > 0)
	 || cmdParser.Found(wxT("h"))) {
		cmdParser.Usage();
		return false;
	}

	cmdParser.Found(wxT("s"), &StartFile);
	cmdParser.Found(wxT("c"), &Category);
	cmdParser.Found(wxT("msect"), &SectMatch);
	cmdParser.Found(wxT("mset"), &SetMatch);
	cmdParser.Found(wxT("a"), &Action);
	cmdParser.Found(wxT("sect"), &Section);
	cmdParser.Found(wxT("set"), &Setting);
	cmdParser.Found(wxT("is"), &IniFileSrc);
	cmdParser.Found(wxT("id"), &IniFileDest);
	cmdParser.Found(wxT("r"), &ResultFile);

	ConfigMgr* mainWindow = new ConfigMgr(NULL);
	/* int returnValue = */ mainWindow->ShowModal();

	mainWindow->Destroy();
	// A modal dialog application should return false to terminate the app.
	return false;
}


/*
 * Cleanup for D2gcmApp
 */

int D2gcmApp::OnExit()
{    
////@begin D2gcmApp cleanup
	return wxApp::OnExit();
////@end D2gcmApp cleanup
}

