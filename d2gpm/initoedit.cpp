/////////////////////////////////////////////////////////////////////////////
// Name:        initoedit.cpp
// Purpose:     
// Author:      Jeremy H. Griffith
// Modified by: 
// Created:     23/01/2010 23:02:03
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

#include "initoedit.h"
#include "d2gmain.h"

////@begin XPM images
////@end XPM images


/*
 * IniToEdit type definition
 */

IMPLEMENT_DYNAMIC_CLASS( IniToEdit, wxDialog )


/*
 * IniToEdit event table definition
 */

BEGIN_EVENT_TABLE( IniToEdit, wxDialog )

////@begin IniToEdit event table entries
	EVT_INIT_DIALOG( IniToEdit::OnInitDialog )

	EVT_BUTTON( wxID_OK, IniToEdit::OnOkClick )

	EVT_BUTTON( wxID_CANCEL, IniToEdit::OnCancelClick )

////@end IniToEdit event table entries

END_EVENT_TABLE()


/*
 * IniToEdit constructors
 */

IniToEdit::IniToEdit()
{
	Init();
}

IniToEdit::IniToEdit( wxWindow* parent, wxWindowID id, const wxString& caption, const wxPoint& pos, const wxSize& size, long style )
{
	Init();
	Create(parent, id, caption, pos, size, style);
}


/*
 * IniToEdit creator
 */

bool IniToEdit::Create( wxWindow* parent, wxWindowID id, const wxString& caption, const wxPoint& pos, const wxSize& size, long style )
{
////@begin IniToEdit creation
	SetExtraStyle(wxWS_EX_BLOCK_EVENTS);
	wxDialog::Create( parent, id, caption, pos, size, style );

	CreateControls();
	if (GetSizer())
	{
		GetSizer()->SetSizeHints(this);
	}
	Centre();
////@end IniToEdit creation
	return true;
}


/*
 * IniToEdit destructor
 */

IniToEdit::~IniToEdit()
{
////@begin IniToEdit destruction
////@end IniToEdit destruction
}


/*
 * Member initialisation
 */

void IniToEdit::Init()
{
////@begin IniToEdit member initialisation
	m_i2edit = NULL;
	m_i2estart = NULL;
	m_i2estartname = NULL;
	m_i2edoc = NULL;
	m_i2edocname = NULL;
	m_i2etok = NULL;
	m_i2ecan = NULL;
////@end IniToEdit member initialisation
}


/*
 * Control creation for IniToEdit
 */

void IniToEdit::CreateControls()
{    
////@begin IniToEdit content construction
	IniToEdit* itemDialog1 = this;

	wxBoxSizer* itemBoxSizer2 = new wxBoxSizer(wxVERTICAL);
	itemDialog1->SetSizer(itemBoxSizer2);

	wxBoxSizer* itemBoxSizer3 = new wxBoxSizer(wxHORIZONTAL);
	itemBoxSizer2->Add(itemBoxSizer3, 0, wxGROW|wxALL, 5);

	m_i2edit = new wxStaticText( itemDialog1, ID_I2EDIT, _("Choose the .ini file to edit:"), wxDefaultPosition, wxDefaultSize, 0 );
	itemBoxSizer3->Add(m_i2edit, 0, wxALIGN_CENTER_VERTICAL|wxLEFT|wxRIGHT|wxTOP, 5);

	wxBoxSizer* itemBoxSizer5 = new wxBoxSizer(wxHORIZONTAL);
	itemBoxSizer2->Add(itemBoxSizer5, 0, wxGROW|wxALL, 5);

	m_i2estart = new wxRadioButton( itemDialog1, ID_I2ESTART, _("Starting .ini: "), wxDefaultPosition, wxDefaultSize, wxRB_GROUP );
	m_i2estart->SetValue(true);
	itemBoxSizer5->Add(m_i2estart, 0, wxALIGN_CENTER_VERTICAL|wxLEFT|wxRIGHT, 5);

	m_i2estartname = new wxStaticText( itemDialog1, ID_I2ESTARTNAME, _("_d2omnihelp.ini"), wxDefaultPosition, wxDefaultSize, wxFULL_REPAINT_ON_RESIZE );
	itemBoxSizer5->Add(m_i2estartname, 0, wxALIGN_CENTER_VERTICAL|wxLEFT|wxRIGHT, 5);

	wxBoxSizer* itemBoxSizer8 = new wxBoxSizer(wxHORIZONTAL);
	itemBoxSizer2->Add(itemBoxSizer8, 0, wxGROW|wxALL, 5);

	m_i2edoc = new wxRadioButton( itemDialog1, ID_I2EDOC, _("Document .ini: "), wxDefaultPosition, wxDefaultSize, 0 );
	m_i2edoc->SetValue(false);
	itemBoxSizer8->Add(m_i2edoc, 0, wxALIGN_CENTER_VERTICAL|wxLEFT|wxRIGHT|wxBOTTOM, 5);

	m_i2edocname = new wxStaticText( itemDialog1, ID_I2EDOCNAME, _("the-document-specific.ini"), wxDefaultPosition, wxDefaultSize, wxFULL_REPAINT_ON_RESIZE );
	itemBoxSizer8->Add(m_i2edocname, 0, wxALIGN_CENTER_VERTICAL|wxLEFT|wxRIGHT|wxBOTTOM, 5);

	wxStdDialogButtonSizer* itemStdDialogButtonSizer11 = new wxStdDialogButtonSizer;

	itemBoxSizer2->Add(itemStdDialogButtonSizer11, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);
	m_i2etok = new wxButton( itemDialog1, wxID_OK, _("&OK"), wxDefaultPosition, wxDefaultSize, 0 );
	m_i2etok->SetDefault();
	itemStdDialogButtonSizer11->AddButton(m_i2etok);

	m_i2ecan = new wxButton( itemDialog1, wxID_CANCEL, _("&Cancel"), wxDefaultPosition, wxDefaultSize, 0 );
	itemStdDialogButtonSizer11->AddButton(m_i2ecan);

	itemStdDialogButtonSizer11->Realize();

////@end IniToEdit content construction

	SetAllLabels();
	SetAllToolTips();
}


/*
 * Should we show tooltips?
 */

bool IniToEdit::ShowToolTips()
{
	return true;
}

/*
 * Get bitmap resources
 */

wxBitmap IniToEdit::GetBitmapResource( const wxString& name )
{
	// Bitmap retrieval
////@begin IniToEdit bitmap retrieval
	wxUnusedVar(name);
	return wxNullBitmap;
////@end IniToEdit bitmap retrieval
}

/*
 * Get icon resources
 */

wxIcon IniToEdit::GetIconResource( const wxString& name )
{
	// Icon retrieval
////@begin IniToEdit icon retrieval
	wxUnusedVar(name);
	return wxNullIcon;
////@end IniToEdit icon retrieval
}


/*
 * wxEVT_INIT_DIALOG event handler for ID_INITOEDIT
 */

void IniToEdit::OnInitDialog( wxInitDialogEvent& event )
{
	event.Skip();
}


/*
 * wxEVT_COMMAND_BUTTON_CLICKED event handler for wxID_OK
 */

void IniToEdit::OnOkClick( wxCommandEvent& event )
{
	int ret = 0;

	if (m_i2estart->GetValue())
		ret = 1;
	else
		ret = 2;

	EndModal(ret);
	wxUnusedVar(event);
}


/*
 * wxEVT_COMMAND_BUTTON_CLICKED event handler for wxID_CANCEL
 */

void IniToEdit::OnCancelClick( wxCommandEvent& event )
{
	EndModal(0);
	wxUnusedVar(event);
}

