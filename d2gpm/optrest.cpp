/////////////////////////////////////////////////////////////////////////////
// Name:        optrest.cpp
// Purpose:     
// Author:      Jeremy H. Griffith
// Modified by: 
// Created:     08/12/2009 22:57:11
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

#include "optrest.h"
#include "d2gmain.h"

////@begin XPM images
////@end XPM images


/*
 * OptRest type definition
 */

IMPLEMENT_DYNAMIC_CLASS( OptRest, wxDialog )


/*
 * OptRest event table definition
 */

BEGIN_EVENT_TABLE( OptRest, wxDialog )

////@begin OptRest event table entries
	EVT_INIT_DIALOG( OptRest::OnInitDialog )

	EVT_BUTTON( wxID_OK, OptRest::OnOkClick )

	EVT_BUTTON( wxID_CANCEL, OptRest::OnCancelClick )

////@end OptRest event table entries

END_EVENT_TABLE()


/*
 * OptRest constructors
 */

OptRest::OptRest()
{
	Init();
}

OptRest::OptRest( wxWindow* parent, wxWindowID id, const wxString& caption, const wxPoint& pos, const wxSize& size, long style )
{
	Init();
	Create(parent, id, caption, pos, size, style);
}


/*
 * OptRest creator
 */

bool OptRest::Create( wxWindow* parent, wxWindowID id, const wxString& caption, const wxPoint& pos, const wxSize& size, long style )
{
////@begin OptRest creation
	SetExtraStyle(wxWS_EX_BLOCK_EVENTS);
	wxDialog::Create( parent, id, caption, pos, size, style );

	CreateControls();
	if (GetSizer())
	{
		GetSizer()->SetSizeHints(this);
	}
	Centre();
////@end OptRest creation
	return true;
}


/*
 * OptRest destructor
 */

OptRest::~OptRest()
{
////@begin OptRest destruction
////@end OptRest destruction
}


/*
 * Member initialisation
 */

void OptRest::Init()
{
////@begin OptRest member initialisation
	m_optrestdlg = NULL;
	m_optrestset = NULL;
	m_optreststart = NULL;
	m_optrestdef = NULL;
	m_optrestok = NULL;
	m_optrestcan = NULL;
////@end OptRest member initialisation
}


/*
 * Control creation for OptRest
 */

void OptRest::CreateControls()
{    
////@begin OptRest content construction
	OptRest* itemDialog1 = this;

	wxBoxSizer* itemBoxSizer2 = new wxBoxSizer(wxVERTICAL);
	itemDialog1->SetSizer(itemBoxSizer2);

	m_optrestdlg = new wxStaticText( itemDialog1, ID_OPTRESTDLG, _("Restore options to:"), wxDefaultPosition, wxDefaultSize, 0 );
	itemBoxSizer2->Add(m_optrestdlg, 0, wxALIGN_LEFT|wxLEFT|wxTOP, 15);

	wxBoxSizer* itemBoxSizer4 = new wxBoxSizer(wxHORIZONTAL);
	itemBoxSizer2->Add(itemBoxSizer4, 0, wxGROW|wxALL, 5);

	itemBoxSizer4->Add(5, 5, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

	m_optrestset = new wxRadioButton( itemDialog1, ID_OPTRESTSET, _("Last options set"), wxDefaultPosition, wxDefaultSize, wxRB_GROUP );
	m_optrestset->SetValue(true);
	itemBoxSizer4->Add(m_optrestset, 0, wxALIGN_CENTER_VERTICAL|wxLEFT|wxTOP, 5);

	wxBoxSizer* itemBoxSizer7 = new wxBoxSizer(wxHORIZONTAL);
	itemBoxSizer2->Add(itemBoxSizer7, 0, wxGROW|wxALL, 5);

	itemBoxSizer7->Add(5, 5, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

	m_optreststart = new wxRadioButton( itemDialog1, ID_OPTRESTSTART, _("Start of session"), wxDefaultPosition, wxDefaultSize, 0 );
	m_optreststart->SetValue(false);
	itemBoxSizer7->Add(m_optreststart, 0, wxALIGN_CENTER_VERTICAL|wxLEFT|wxTOP, 5);

	wxBoxSizer* itemBoxSizer10 = new wxBoxSizer(wxHORIZONTAL);
	itemBoxSizer2->Add(itemBoxSizer10, 0, wxGROW|wxALL, 5);

	itemBoxSizer10->Add(5, 5, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

	m_optrestdef = new wxRadioButton( itemDialog1, ID_OPTRESTDEF, _("Initial defaults"), wxDefaultPosition, wxDefaultSize, 0 );
	m_optrestdef->SetValue(false);
	itemBoxSizer10->Add(m_optrestdef, 0, wxALIGN_CENTER_VERTICAL|wxLEFT|wxTOP, 5);

	wxBoxSizer* itemBoxSizer13 = new wxBoxSizer(wxHORIZONTAL);
	itemBoxSizer2->Add(itemBoxSizer13, 0, wxGROW|wxALL, 5);

	wxStdDialogButtonSizer* itemStdDialogButtonSizer14 = new wxStdDialogButtonSizer;

	itemBoxSizer2->Add(itemStdDialogButtonSizer14, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);
	m_optrestok = new wxButton( itemDialog1, wxID_OK, _("&OK"), wxDefaultPosition, wxDefaultSize, 0 );
	m_optrestok->SetDefault();
	itemStdDialogButtonSizer14->AddButton(m_optrestok);

	m_optrestcan = new wxButton( itemDialog1, wxID_CANCEL, _("&Cancel"), wxDefaultPosition, wxDefaultSize, 0 );
	itemStdDialogButtonSizer14->AddButton(m_optrestcan);

	itemStdDialogButtonSizer14->Realize();

////@end OptRest content construction

	SetAllLabels();
	SetAllToolTips();
}


/*
 * Should we show tooltips?
 */

bool OptRest::ShowToolTips()
{
	return true;
}

/*
 * Get bitmap resources
 */

wxBitmap OptRest::GetBitmapResource( const wxString& name )
{
	// Bitmap retrieval
////@begin OptRest bitmap retrieval
	wxUnusedVar(name);
	return wxNullBitmap;
////@end OptRest bitmap retrieval
}

/*
 * Get icon resources
 */

wxIcon OptRest::GetIconResource( const wxString& name )
{
	// Icon retrieval
////@begin OptRest icon retrieval
	wxUnusedVar(name);
	return wxNullIcon;
////@end OptRest icon retrieval
}



/*
 * wxEVT_INIT_DIALOG event handler for ID_OPTREST
 */

void OptRest::OnInitDialog( wxInitDialogEvent& event )
{
	//m_optrestset->SetValue(true);
	event.Skip();
}


/*
 * wxEVT_COMMAND_BUTTON_CLICKED event handler for wxID_OK
 */

void OptRest::OnOkClick( wxCommandEvent& event )
{
	int ret = 0;

	if (m_optrestset->GetValue())
		ret = 1;
	else if (m_optreststart->GetValue())
		ret = 2;
	else if (m_optrestdef->GetValue())
		ret = 3;

	EndModal(ret);
	wxUnusedVar(event);
}


/*
 * wxEVT_COMMAND_BUTTON_CLICKED event handler for wxID_CANCEL
 */

void OptRest::OnCancelClick( wxCommandEvent& event )
{
	EndModal(0);
	wxUnusedVar(event);
}

