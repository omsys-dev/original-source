/////////////////////////////////////////////////////////////////////////////
// Name:        d2guiapp.h
// Purpose:     
// Author:      Jeremy H. Griffith
// Modified by: 
// Created:     02/12/2009 16:01:53
// RCS-ID:      
// Copyright:   (c) Copyright 2009 Omni Sytems, Inc.
// Licence:     
/////////////////////////////////////////////////////////////////////////////

#ifndef _D2GUIAPP_H_
#define _D2GUIAPP_H_


/*!
 * Includes
 */

////@begin includes
#include "wx/image.h"
#include "d2gmain.h"
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
 * D2guiApp class declaration
 */

class D2guiApp: public wxApp
{    
	DECLARE_CLASS( D2guiApp )
	DECLARE_EVENT_TABLE()

public:
	/// Constructor
	D2guiApp();

	void Init();

	/// Initialises the application
	virtual bool OnInit();

	/// Called on exit
	virtual int OnExit();

////@begin D2guiApp event handler declarations

////@end D2guiApp event handler declarations

////@begin D2guiApp member function declarations

////@end D2guiApp member function declarations

////@begin D2guiApp member variables
////@end D2guiApp member variables
};

/*!
 * Application instance declaration 
 */

////@begin declare app
DECLARE_APP(D2guiApp)
////@end declare app

#endif
	// _D2GUIAPP_H_
