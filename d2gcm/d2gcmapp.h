/////////////////////////////////////////////////////////////////////////////
// Name:        d2gcmapp.h
// Purpose:     
// Author:      Jeremy H Griffith
// Modified by: 
// Created:     12/03/2012 14:03:16
// RCS-ID:      
// Copyright:   Copyright (c) 2012 Omni Systems
// Licence:     
/////////////////////////////////////////////////////////////////////////////

#ifndef _D2GCMAPP_H_
#define _D2GCMAPP_H_


/*!
 * Includes
 */

#include "wx/cmdline.h"

////@begin includes
#include "wx/image.h"
#include "configmgr.h"
////@end includes

/*!
 * Forward declarations
 */

////@begin forward declarations
////@end forward declarations

/*!
 * Control identifiers
 */

////@begin control identifiers
////@end control identifiers

/*!
 * D2gcmApp class declaration
 */

class D2gcmApp: public wxApp
{    
    DECLARE_CLASS( D2gcmApp )
    DECLARE_EVENT_TABLE()

public:
    /// Constructor
    D2gcmApp();

    void Init();

    /// Initialises the application
    virtual bool OnInit();

    /// Called on exit
    virtual int OnExit();

////@begin D2gcmApp event handler declarations

////@end D2gcmApp event handler declarations

////@begin D2gcmApp member function declarations

////@end D2gcmApp member function declarations

////@begin D2gcmApp member variables
////@end D2gcmApp member variables

	static const wxCmdLineEntryDesc g_cmdLineDesc[];
	static wxString StartFile;
	static wxString Category;
	static wxString SectMatch;
	static wxString SetMatch;
	static wxString Action;
	static wxString Section;
	static wxString Setting;
	static wxString IniFileSrc;
	static wxString IniFileDest;
	static wxString ResultFile;
};

/*!
 * Application instance declaration 
 */

////@begin declare app
DECLARE_APP(D2gcmApp)
////@end declare app

#endif
    // _D2GCMAPP_H_
