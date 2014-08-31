/////////////////////////////////////////////////////////////////////////////
// Name:        d2guiapp.cpp
// Purpose:     
// Author:      Jeremy H. Griffith
// Modified by: 
// Created:     02/12/2009 16:01:53
// RCS-ID:      
// Copyright:   (c) Copyright 2009 Omni Sytems, Inc.
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

#include "d2guiapp.h"

////@begin XPM images
////@end XPM images


/*
 * Application instance implementation
 */

////@begin implement app
IMPLEMENT_APP( D2guiApp )
////@end implement app


/*
 * D2guiApp type definition
 */

IMPLEMENT_CLASS( D2guiApp, wxApp )


/*
 * D2guiApp event table definition
 */

BEGIN_EVENT_TABLE( D2guiApp, wxApp )

////@begin D2guiApp event table entries
////@end D2guiApp event table entries

END_EVENT_TABLE()


/*
 * Constructor for D2guiApp
 */

D2guiApp::D2guiApp()
{
	Init();
}


/*
 * Member initialisation
 */

void D2guiApp::Init()
{
////@begin D2guiApp member initialisation
////@end D2guiApp member initialisation
}

/*
 * Initialisation for D2guiApp
 */

bool D2guiApp::OnInit()
{    
////@begin D2guiApp initialisation
	// Remove the comment markers above and below this block
	// to make permanent changes to the code.

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
	D2gmain* mainWindow = new D2gmain(NULL);
	/* int returnValue = */ mainWindow->ShowModal();

	mainWindow->Destroy();
	// A modal dialog application should return false to terminate the app.
	return false;
////@end D2guiApp initialisation

	return true;
}


/*
 * Cleanup for D2guiApp
 */

int D2guiApp::OnExit()
{    
////@begin D2guiApp cleanup
	return wxApp::OnExit();
////@end D2guiApp cleanup
}



