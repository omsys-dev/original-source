/////////////////////////////////////////////////////////////////////////////
// Name:        d2gmanager.cpp
// Purpose:     
// Author:      Jeremy H. Griffith
// Modified by: 
// Created:     07/12/2009 21:02:22
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

#include "d2gmanager.h"

////@begin XPM images
////@end XPM images


/*
 * D2GManager type definition
 */

IMPLEMENT_DYNAMIC_CLASS( D2GManager, wxDialog )


/*
 * D2GManager event table definition
 */

BEGIN_EVENT_TABLE( D2GManager, wxDialog )

////@begin D2GManager event table entries
	EVT_BUTTON( ID_MANPROJ, D2GManager::OnManprojClick )

	EVT_BUTTON( ID_MANSET, D2GManager::OnMansetClick )

	EVT_BUTTON( ID_DONE, D2GManager::OnDoneClick )

////@end D2GManager event table entries

END_EVENT_TABLE()


/*
 * D2GManager constructors
 */

D2GManager::D2GManager()
{
	Init();
}

D2GManager::D2GManager( wxWindow* parent, wxWindowID id, const wxString& caption, const wxPoint& pos, const wxSize& size, long style )
{
	Init();
	Create(parent, id, caption, pos, size, style);
}


/*
 * D2GManager creator
 */

bool D2GManager::Create( wxWindow* parent, wxWindowID id, const wxString& caption, const wxPoint& pos, const wxSize& size, long style )
{
////@begin D2GManager creation
	SetExtraStyle(wxWS_EX_BLOCK_EVENTS);
	wxDialog::Create( parent, id, caption, pos, size, style );

	CreateControls();
	if (GetSizer())
	{
		GetSizer()->SetSizeHints(this);
	}
	Centre();
////@end D2GManager creation
	return true;
}


/*
 * D2GManager destructor
 */

D2GManager::~D2GManager()
{
	ProjManagerWindow->Destroy();

////@begin D2GManager destruction
////@end D2GManager destruction
}


/*
 * Member initialisation
 */

void D2GManager::Init()
{
////@begin D2GManager member initialisation
	m_d2gmanager = NULL;
	m_manageprojects = NULL;
	m_manproj = NULL;
	m_managesettings = NULL;
	m_manset = NULL;
////@end D2GManager member initialisation
}


/*
 * Control creation for D2GManager
 */

void D2GManager::CreateControls()
{    
////@begin D2GManager content construction
	D2GManager* itemDialog1 = this;

	wxBoxSizer* itemBoxSizer2 = new wxBoxSizer(wxVERTICAL);
	itemDialog1->SetSizer(itemBoxSizer2);

	wxBoxSizer* itemBoxSizer3 = new wxBoxSizer(wxHORIZONTAL);
	itemBoxSizer2->Add(itemBoxSizer3, 100, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);

#ifdef DITA
	m_d2gmanager = new wxStaticText( itemDialog1, ID_D2GMANAGER, _("DITA2Go Manager"), wxDefaultPosition, wxDefaultSize, wxALIGN_CENTRE );
#endif
#ifdef UDOC
	m_d2gmanager = new wxStaticText( itemDialog1, ID_D2GMANAGER, _("uDoc2Go Manager"), wxDefaultPosition, wxDefaultSize, wxALIGN_CENTRE );
#endif
	itemBoxSizer3->Add(m_d2gmanager, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

	wxBoxSizer* itemBoxSizer5 = new wxBoxSizer(wxHORIZONTAL);
	itemBoxSizer2->Add(itemBoxSizer5, 0, wxGROW|wxALL, 5);

	m_manageprojects = new wxStaticText( itemDialog1, ID_MANAGEPROJECTS, _("Manage projects:  "), wxDefaultPosition, wxDefaultSize, 0 );
	itemBoxSizer5->Add(m_manageprojects, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

	itemBoxSizer5->Add(5, 5, 100, wxALIGN_CENTER_VERTICAL|wxALL, 5);

	m_manproj = new wxButton( itemDialog1, ID_MANPROJ, _("Open Project Manager"), wxDefaultPosition, wxSize(125, -1), 0 );
	itemBoxSizer5->Add(m_manproj, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

	wxBoxSizer* itemBoxSizer9 = new wxBoxSizer(wxHORIZONTAL);
	itemBoxSizer2->Add(itemBoxSizer9, 0, wxGROW|wxALL, 5);

	m_managesettings = new wxStaticText( itemDialog1, ID_MANAGESETTINGS, _("Change project settings:  "), wxDefaultPosition, wxDefaultSize, 0 );
	itemBoxSizer9->Add(m_managesettings, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

	itemBoxSizer9->Add(5, 5, 100, wxALIGN_CENTER_VERTICAL|wxALL, 5);

	m_manset = new wxButton( itemDialog1, ID_MANSET, _("Open Settings Manager"), wxDefaultPosition, wxSize(125, -1), 0 );
	itemBoxSizer9->Add(m_manset, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

	wxBoxSizer* itemBoxSizer13 = new wxBoxSizer(wxHORIZONTAL);
	itemBoxSizer2->Add(itemBoxSizer13, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);

	wxButton* itemButton14 = new wxButton( itemDialog1, ID_DONE, _("Done"), wxDefaultPosition, wxDefaultSize, 0 );
	itemButton14->SetDefault();
	itemButton14->Show(false);
	itemBoxSizer13->Add(itemButton14, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

////@end D2GManager content construction

	ProjManagerWindow = new D2gmain(NULL);

}


/*
 * Should we show tooltips?
 */

bool D2GManager::ShowToolTips()
{
	return true;
}

/*
 * Get bitmap resources
 */

wxBitmap D2GManager::GetBitmapResource( const wxString& name )
{
	// Bitmap retrieval
////@begin D2GManager bitmap retrieval
	wxUnusedVar(name);
	return wxNullBitmap;
////@end D2GManager bitmap retrieval
}

/*
 * Get icon resources
 */

wxIcon D2GManager::GetIconResource( const wxString& name )
{
	// Icon retrieval
////@begin D2GManager icon retrieval
	wxUnusedVar(name);
	return wxNullIcon;
////@end D2GManager icon retrieval
}


/*
 * wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_MANPROJ
 */

void D2GManager::OnManprojClick( wxCommandEvent& event )
{
	ProjManagerWindow->Show();

	event.Skip();
}


/*
 * wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_MANSET
 */

void D2GManager::OnMansetClick( wxCommandEvent& event )
{
////@begin wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_MANSET in D2GManager.
	// Before editing this code, remove the block markers.
	event.Skip();
////@end wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_MANSET in D2GManager. 
}


/*
 * wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_DONE
 */

void D2GManager::OnDoneClick( wxCommandEvent& event )
{
////@begin wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_DONE in D2GManager.
	// Before editing this code, remove the block markers.
	event.Skip();
////@end wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_DONE in D2GManager. 
}

