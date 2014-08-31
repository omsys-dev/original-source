/////////////////////////////////////////////////////////////////////////////
// Name:        d2gmanager.h
// Purpose:     
// Author:      Jeremy H. Griffith
// Modified by: 
// Created:     07/12/2009 21:02:22
// RCS-ID:      
// Copyright:   (c) Copyright 2009 Omni Sytems, Inc.
// Licence:     
/////////////////////////////////////////////////////////////////////////////

#ifndef _D2GMANAGER_H_
#define _D2GMANAGER_H_


/*!
 * Includes
 */

////@begin includes
////@end includes

#include "d2gmain.h"


/*!
 * Forward declarations
 */

////@begin forward declarations
////@end forward declarations

/*!
 * Control identifiers
 */

////@begin control identifiers
#define ID_D2GMANAGE 12000
#define ID_D2GMANAGER 12001
#define ID_MANAGEPROJECTS 12002
#define ID_MANPROJ 12003
#define ID_MANAGESETTINGS 12004
#define ID_MANSET 12005
#define ID_DONE 12006
#define SYMBOL_D2GMANAGER_STYLE wxCAPTION|wxRESIZE_BORDER|wxSYSTEM_MENU|wxCLOSE_BOX|wxMINIMIZE_BOX|wxTAB_TRAVERSAL
#ifdef DITA
#define SYMBOL_D2GMANAGER_TITLE _("DITA2Go Manager")
#endif
#ifdef UDOC
#define SYMBOL_D2GMANAGER_TITLE _("uDoc2Go Manager")
#endif
#define SYMBOL_D2GMANAGER_IDNAME ID_D2GMANAGE
#define SYMBOL_D2GMANAGER_SIZE wxSize(400, 300)
#define SYMBOL_D2GMANAGER_POSITION wxDefaultPosition
////@end control identifiers


/*!
 * D2GManager class declaration
 */

class D2GManager: public wxDialog
{    
	DECLARE_DYNAMIC_CLASS( D2GManager )
	DECLARE_EVENT_TABLE()

public:
	/// Constructors
	D2GManager();
	D2GManager( wxWindow* parent, wxWindowID id = SYMBOL_D2GMANAGER_IDNAME, const wxString& caption = SYMBOL_D2GMANAGER_TITLE, const wxPoint& pos = SYMBOL_D2GMANAGER_POSITION, const wxSize& size = SYMBOL_D2GMANAGER_SIZE, long style = SYMBOL_D2GMANAGER_STYLE );

	/// Creation
	bool Create( wxWindow* parent, wxWindowID id = SYMBOL_D2GMANAGER_IDNAME, const wxString& caption = SYMBOL_D2GMANAGER_TITLE, const wxPoint& pos = SYMBOL_D2GMANAGER_POSITION, const wxSize& size = SYMBOL_D2GMANAGER_SIZE, long style = SYMBOL_D2GMANAGER_STYLE );

	/// Destructor
	~D2GManager();

	/// Initialises member variables
	void Init();

	/// Creates the controls and sizers
	void CreateControls();

////@begin D2GManager event handler declarations

	/// wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_MANPROJ
	void OnManprojClick( wxCommandEvent& event );

	/// wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_MANSET
	void OnMansetClick( wxCommandEvent& event );

	/// wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_DONE
	void OnDoneClick( wxCommandEvent& event );

////@end D2GManager event handler declarations

////@begin D2GManager member function declarations

	/// Retrieves bitmap resources
	wxBitmap GetBitmapResource( const wxString& name );

	/// Retrieves icon resources
	wxIcon GetIconResource( const wxString& name );
////@end D2GManager member function declarations

	/// Should we show tooltips?
	static bool ShowToolTips();

////@begin D2GManager member variables
	wxStaticText* m_d2gmanager;
	wxStaticText* m_manageprojects;
	wxButton* m_manproj;
	wxStaticText* m_managesettings;
	wxButton* m_manset;
////@end D2GManager member variables

	D2gmain* ProjManagerWindow;
};

#endif
	// _D2GMANAGER_H_
