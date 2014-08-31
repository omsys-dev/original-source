/////////////////////////////////////////////////////////////////////////////
// Name:        configstart.cpp
// Purpose:     
// Author:      Jeremy H. Griffith
// Modified by: 
// Created:     26/12/2009 14:55:00
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

#include "configstart.h"
#include "d2gmain.h"

////@begin XPM images
////@end XPM images


/*
 * Configstart type definition
 */

IMPLEMENT_DYNAMIC_CLASS( Configstart, wxWizard )


/*
 * Configstart event table definition
 */

BEGIN_EVENT_TABLE( Configstart, wxWizard )

////@begin Configstart event table entries
	EVT_WIZARD_PAGE_CHANGED( ID_CONFIGSTART, Configstart::OnConfigstartPageChanged )
	EVT_WIZARD_PAGE_CHANGING( ID_CONFIGSTART, Configstart::OnConfigstartPageChanging )
	EVT_WIZARD_CANCEL( ID_CONFIGSTART, Configstart::OnConfigstartCancel )
	EVT_WIZARD_FINISHED( ID_CONFIGSTART, Configstart::OnConfigstartFinished )

////@end Configstart event table entries

END_EVENT_TABLE()


/*
 * Configstart constructors
 */

Configstart::Configstart()
{
	Init();
}

Configstart::Configstart( wxWindow* parent, wxWindowID id, const wxPoint& pos )
{
	Init();
	Create(parent, id, pos);
}


/*
 * Configstart creator
 */

bool Configstart::Create( wxWindow* parent, wxWindowID id, const wxPoint& pos )
{
////@begin Configstart creation
	SetExtraStyle(wxWS_EX_BLOCK_EVENTS);
	wxBitmap wizardBitmap(wxNullBitmap);
	wxWizard::Create( parent, id, _("Configure Starting .Ini"), wizardBitmap, pos, wxDEFAULT_DIALOG_STYLE|wxCAPTION|wxRESIZE_BORDER|wxSYSTEM_MENU|wxCLOSE_BOX|wxMINIMIZE_BOX );

	CreateControls();
////@end Configstart creation
	return true;
}


/*
 * Configstart destructor
 */

Configstart::~Configstart()
{
////@begin Configstart destruction
////@end Configstart destruction
}


/*
 * Member initialisation
 */

void Configstart::Init()
{
////@begin Configstart member initialisation
	m_wizardpage1 = NULL;
	m_wizardpage1a = NULL;
	m_wizardpage2 = NULL;
	m_wizardpage2a = NULL;
	m_wizardpage3 = NULL;
	m_wizardpage4 = NULL;
	m_wizardpage5 = NULL;
	m_wizardpageend = NULL;
////@end Configstart member initialisation

	//m_configpduse = true;
	m_configdvuse = true;
}


/*
 * Control creation for Configstart
 */

void Configstart::CreateControls()
{    
////@begin Configstart content construction
	Configstart* itemWizard1 = this;

	m_wizardpage1 = new WizardPage1( itemWizard1 );
	itemWizard1->GetPageAreaSizer()->Add(m_wizardpage1);

	m_wizardpage1a = new WizardPage1A( itemWizard1 );
	itemWizard1->GetPageAreaSizer()->Add(m_wizardpage1a);

	m_wizardpage2 = new WizardPage2( itemWizard1 );
	itemWizard1->GetPageAreaSizer()->Add(m_wizardpage2);

	m_wizardpage2a = new WizardPage2A( itemWizard1 );
	itemWizard1->GetPageAreaSizer()->Add(m_wizardpage2a);

	m_wizardpage3 = new WizardPage3( itemWizard1 );
	itemWizard1->GetPageAreaSizer()->Add(m_wizardpage3);

	m_wizardpage4 = new WizardPage4( itemWizard1 );
	itemWizard1->GetPageAreaSizer()->Add(m_wizardpage4);

	m_wizardpage5 = new WizardPage5( itemWizard1 );
	itemWizard1->GetPageAreaSizer()->Add(m_wizardpage5);

	m_wizardpageend = new WizardPageEnd( itemWizard1 );
	itemWizard1->GetPageAreaSizer()->Add(m_wizardpageend);

	wxWizardPageSimple* lastPage = NULL;
////@end Configstart content construction

	wxUnusedVar(lastPage);

	SetAllLabels();
	SetAllToolTips();

	wxString *str = NULL;
	str = new wxString(m_wizardpage3->m_confightml->GetLabelText());
	str->Replace("HTML", m_configouttype->c_str(), false); 
	m_wizardpage3->m_confightml->SetLabel(*str);

	str = new wxString(m_wizardpage5->m_configrtf->GetLabelText());
	str->Replace("RTF", m_configouttype->c_str(), false); 
	m_wizardpage5->m_configrtf->SetLabel(*str);
}


/*
 * Runs the wizard.
 */

bool Configstart::Run()
{
	wxWindowList::compatibility_iterator node = GetChildren().GetFirst();
	while (node)
	{
		wxWizardPage* startPage = wxDynamicCast(node->GetData(), wxWizardPage);
		if (startPage) return RunWizard(startPage);
		node = node->GetNext();
	}
	return false;
}


/*
 * Should we show tooltips?
 */

bool Configstart::ShowToolTips()
{
	return true;
}

/*
 * Get bitmap resources
 */

wxBitmap Configstart::GetBitmapResource( const wxString& name )
{
	// Bitmap retrieval
////@begin Configstart bitmap retrieval
	wxUnusedVar(name);
	return wxNullBitmap;
////@end Configstart bitmap retrieval
}

/*
 * Get icon resources
 */

wxIcon Configstart::GetIconResource( const wxString& name )
{
	// Icon retrieval
////@begin Configstart icon retrieval
	wxUnusedVar(name);
	return wxNullIcon;
////@end Configstart icon retrieval
}


/*
 * wxEVT_WIZARD_PAGE_CHANGED event handler for ID_CONFIGSTART
 */

void Configstart::OnConfigstartPageChanged( wxWizardEvent& event )
{
////@begin wxEVT_WIZARD_PAGE_CHANGED event handler for ID_CONFIGSTART in Configstart.
	// Before editing this code, remove the block markers.
	event.Skip();
////@end wxEVT_WIZARD_PAGE_CHANGED event handler for ID_CONFIGSTART in Configstart. 
}


/*
 * wxEVT_WIZARD_PAGE_CHANGING event handler for ID_CONFIGSTART
 */

void Configstart::OnConfigstartPageChanging( wxWizardEvent& event )
{
////@begin wxEVT_WIZARD_PAGE_CHANGING event handler for ID_CONFIGSTART in Configstart.
	// Before editing this code, remove the block markers.
	event.Skip();
////@end wxEVT_WIZARD_PAGE_CHANGING event handler for ID_CONFIGSTART in Configstart. 
}


/*
 * wxEVT_WIZARD_CANCEL event handler for ID_CONFIGSTART
 */

void Configstart::OnConfigstartCancel( wxWizardEvent& event )
{
////@begin wxEVT_WIZARD_CANCEL event handler for ID_CONFIGSTART in Configstart.
	// Before editing this code, remove the block markers.
	event.Skip();
////@end wxEVT_WIZARD_CANCEL event handler for ID_CONFIGSTART in Configstart. 
}


/*
 * wxEVT_WIZARD_FINISHED event handler for ID_CONFIGSTART
 */

void Configstart::OnConfigstartFinished( wxWizardEvent& event )
{
	wxUnusedVar(event);

	// page 1 values
	//m_configpduse = m_wizardpage1->m_configinifile->GetValue();
	//if (m_configpduse)
		m_configdocini = new wxString(m_wizardpage1->m_configinisel->GetPath());
	//else
	//	m_configdocini = NULL;

	m_configdvuse = m_wizardpage1a->m_configdvalfile->GetValue();
	if (m_configdvuse)
		m_configdval = new wxString(m_wizardpage1a->m_configdvalsel->GetPath());
	else
		m_configdval = NULL;

	// page 2 values
	m_configwasdef = m_wizardpage1->m_configwasyes->GetValue();
	wxString wxs = m_wizardpage2->m_configanametxt->GetValue();
	m_configanamestr = new wxString(wxs);
	wxString wxs2 = m_wizardpage2->m_configavertxt->GetValue();
	m_configaverstr = new wxString(wxs2);
	m_configgrdirstr = new wxString(m_wizardpage2a->m_configgrsel->GetPath());
	wxString wxs3 = m_wizardpage2a->m_configgrtxt->GetValue();
	m_configgrfilesstr = new wxString(wxs3);

	// page 3, 4, and 5 values read directly from pages
}



/*
 * WizardPage1 type definition
 */

IMPLEMENT_DYNAMIC_CLASS( WizardPage1, wxWizardPage )


/*
 * WizardPage1 event table definition
 */

BEGIN_EVENT_TABLE( WizardPage1, wxWizardPage )

////@begin WizardPage1 event table entries
////@end WizardPage1 event table entries

END_EVENT_TABLE()


/*
 * WizardPage1 constructors
 */

WizardPage1::WizardPage1()
{
	Init();
}

WizardPage1::WizardPage1( wxWizard* parent )
{
	Init();
	Create( parent );
}


/*
 * WizardPage creator
 */

bool WizardPage1::Create( wxWizard* parent )
{
  configstart = (Configstart *) parent;
////@begin WizardPage1 creation
	wxBitmap wizardBitmap(wxNullBitmap);
	wxWizardPage::Create( parent, wizardBitmap );

	CreateControls();
	if (GetSizer())
		GetSizer()->Fit(this);
////@end WizardPage1 creation

	return true;
}


/*
 * WizardPage1 destructor
 */

WizardPage1::~WizardPage1()
{
////@begin WizardPage1 destruction
////@end WizardPage1 destruction
}


/*
 * Member initialisation
 */

void WizardPage1::Init()
{
////@begin WizardPage1 member initialisation
	m_configall = NULL;
	m_configstart1 = NULL;
	m_configstart2 = NULL;
	m_configinisel = NULL;
	m_configwas = NULL;
	m_configwas2 = NULL;
	m_configwasyes = NULL;
	m_configwasno = NULL;
	m_configlocmsg = NULL;
////@end WizardPage1 member initialisation
}


/*
 * Control creation for WizardPage
 */

void WizardPage1::CreateControls()
{    
////@begin WizardPage1 content construction
	WizardPage1* itemWizardPage2 = this;

	wxBoxSizer* itemBoxSizer3 = new wxBoxSizer(wxVERTICAL);
	itemWizardPage2->SetSizer(itemBoxSizer3);

	wxBoxSizer* itemBoxSizer4 = new wxBoxSizer(wxHORIZONTAL);
	itemBoxSizer3->Add(itemBoxSizer4, 0, wxALIGN_CENTER_HORIZONTAL|wxLEFT|wxRIGHT|wxTOP, 5);

	m_configall = new wxStaticText( itemWizardPage2, ID_CONFIGALL, _("Configure Starting Ini File"), wxDefaultPosition, wxDefaultSize, 0 );
	m_configall->SetFont(wxFont(8, wxSWISS, wxNORMAL, wxBOLD, false, wxT("Tahoma")));
	itemBoxSizer4->Add(m_configall, 0, wxALIGN_CENTER_VERTICAL|wxLEFT|wxRIGHT|wxTOP, 5);

	wxBoxSizer* itemBoxSizer6 = new wxBoxSizer(wxHORIZONTAL);
	itemBoxSizer3->Add(itemBoxSizer6, 0, wxALIGN_CENTER_HORIZONTAL|wxLEFT|wxRIGHT|wxBOTTOM, 5);

	wxStaticLine* itemStaticLine7 = new wxStaticLine( itemWizardPage2, wxID_STATIC, wxDefaultPosition, wxSize(100, -1), wxLI_HORIZONTAL );
	itemBoxSizer6->Add(itemStaticLine7, 0, wxGROW|wxALL, 5);

	wxBoxSizer* itemBoxSizer8 = new wxBoxSizer(wxHORIZONTAL);
	itemBoxSizer3->Add(itemBoxSizer8, 0, wxGROW|wxALL, 5);

	m_configstart1 = new wxStaticText( itemWizardPage2, ID_CONFIGSTART1, _("This wizard will help you set up your starting .ini file for the project you just \ncreated or modified. If you would rather edit the file yourself, click Cancel."), wxDefaultPosition, wxDefaultSize, 0 );
	itemBoxSizer8->Add(m_configstart1, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

	wxBoxSizer* itemBoxSizer10 = new wxBoxSizer(wxHORIZONTAL);
	itemBoxSizer3->Add(itemBoxSizer10, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);

	wxStaticLine* itemStaticLine11 = new wxStaticLine( itemWizardPage2, wxID_STATIC, wxDefaultPosition, wxSize(100, -1), wxLI_HORIZONTAL );
	itemBoxSizer10->Add(itemStaticLine11, 0, wxGROW|wxALL, 5);

	wxBoxSizer* itemBoxSizer12 = new wxBoxSizer(wxHORIZONTAL);
	itemBoxSizer3->Add(itemBoxSizer12, 0, wxGROW|wxALL, 5);

	m_configstart2 = new wxStaticText( itemWizardPage2, ID_CONFIGSTART2, _("Set the path and the name of the document-specific .ini file to use:"), wxDefaultPosition, wxDefaultSize, 0 );
	itemBoxSizer12->Add(m_configstart2, 0, wxALIGN_CENTER_VERTICAL|wxLEFT|wxRIGHT|wxTOP, 5);

	wxBoxSizer* itemBoxSizer14 = new wxBoxSizer(wxHORIZONTAL);
	itemBoxSizer3->Add(itemBoxSizer14, 0, wxGROW|wxLEFT|wxRIGHT, 5);

	m_configinisel = new wxFilePickerCtrl( itemWizardPage2, ID_CONFIGINISEL, wxEmptyString, _("Source-specific .ini file to use for this project:"), wxEmptyString, wxDefaultPosition, wxDefaultSize, wxFLP_USE_TEXTCTRL|wxFLP_OPEN );
	itemBoxSizer14->Add(m_configinisel, 100, wxALIGN_CENTER_VERTICAL|wxLEFT|wxRIGHT, 5);

	wxBoxSizer* itemBoxSizer16 = new wxBoxSizer(wxHORIZONTAL);
	itemBoxSizer3->Add(itemBoxSizer16, 0, wxGROW|wxALL, 5);

	m_configwas = new wxStaticText( itemWizardPage2, ID_CONFIGWAS, _("Set the default value to be used in the document-specific .ini file for:"), wxDefaultPosition, wxDefaultSize, 0 );
	itemBoxSizer16->Add(m_configwas, 0, wxALIGN_CENTER_VERTICAL|wxLEFT|wxRIGHT|wxTOP, 5);

	wxBoxSizer* itemBoxSizer18 = new wxBoxSizer(wxHORIZONTAL);
	itemBoxSizer3->Add(itemBoxSizer18, 0, wxGROW|wxALL, 5);

	m_configwas2 = new wxStaticText( itemWizardPage2, ID_CONFIGWAS2, _("Wrap and ship:"), wxDefaultPosition, wxDefaultSize, 0 );
	itemBoxSizer18->Add(m_configwas2, 60, wxALIGN_CENTER_VERTICAL|wxALL, 5);

	m_configwasyes = new wxRadioButton( itemWizardPage2, ID_CONFIGWASYES, _("Yes"), wxDefaultPosition, wxDefaultSize, wxRB_GROUP );
	m_configwasyes->SetValue(true);
	itemBoxSizer18->Add(m_configwasyes, 20, wxALIGN_CENTER_VERTICAL|wxALL, 5);

	m_configwasno = new wxRadioButton( itemWizardPage2, ID_CONFIGWASNO, _("No"), wxDefaultPosition, wxDefaultSize, 0 );
	m_configwasno->SetValue(false);
	itemBoxSizer18->Add(m_configwasno, 20, wxALIGN_CENTER_VERTICAL|wxALL, 5);

	wxBoxSizer* itemBoxSizer22 = new wxBoxSizer(wxHORIZONTAL);
	itemBoxSizer3->Add(itemBoxSizer22, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);

	wxStaticLine* itemStaticLine23 = new wxStaticLine( itemWizardPage2, wxID_STATIC, wxDefaultPosition, wxSize(100, -1), wxLI_HORIZONTAL );
	itemBoxSizer22->Add(itemStaticLine23, 0, wxGROW|wxALL, 5);

	wxBoxSizer* itemBoxSizer24 = new wxBoxSizer(wxHORIZONTAL);
	itemBoxSizer3->Add(itemBoxSizer24, 0, wxGROW|wxALL, 5);

	m_configlocmsg = new wxStaticText( itemWizardPage2, ID_CONFIGLOCMSG, _("On the next three pages, decide whether each setting applies to all\noutput types for this DITA source, or just to this particular project."), wxDefaultPosition, wxDefaultSize, 0 );
	itemBoxSizer24->Add(m_configlocmsg, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

////@end WizardPage1 content construction

	(m_configinisel->GetTextCtrl())->SetEditable(false);

	//m_configinifile->SetValue(configstart->m_configpduse);
	//m_configinino->SetValue(!configstart->m_configpduse);
	m_configinisel->SetPath(*configstart->m_configdocini);

	m_configwasyes->SetValue(configstart->m_configwasdef);
	m_configwasno->SetValue(!configstart->m_configwasdef);
}


/*
 * Gets the previous page.
 */

wxWizardPage* WizardPage1::GetPrev() const
{
	// return the previous page
	return NULL;
}


/*
 * Gets the next page.
 */

wxWizardPage* WizardPage1::GetNext() const
{
	// return the next page
	return configstart->m_wizardpage1a;
}


/*
 * Should we show tooltips?
 */

bool WizardPage1::ShowToolTips()
{
	return true;
}

/*
 * Get bitmap resources
 */

wxBitmap WizardPage1::GetBitmapResource( const wxString& name )
{
	// Bitmap retrieval
////@begin WizardPage1 bitmap retrieval
	wxUnusedVar(name);
	return wxNullBitmap;
////@end WizardPage1 bitmap retrieval
}

/*
 * Get icon resources
 */

wxIcon WizardPage1::GetIconResource( const wxString& name )
{
	// Icon retrieval
////@begin WizardPage1 icon retrieval
	wxUnusedVar(name);
	return wxNullIcon;
////@end WizardPage1 icon retrieval
}



/*
 * WizardPage1A type definition
 */

IMPLEMENT_DYNAMIC_CLASS( WizardPage1A, wxWizardPage )


/*
 * WizardPage1A event table definition
 */

BEGIN_EVENT_TABLE( WizardPage1A, wxWizardPage )

////@begin WizardPage1A event table entries
////@end WizardPage1A event table entries

END_EVENT_TABLE()


/*
 * WizardPage1A constructors
 */

WizardPage1A::WizardPage1A()
{
	Init();
}

WizardPage1A::WizardPage1A( wxWizard* parent )
{
	Init();
	Create( parent );
}


/*
 * WizardPage1A creator
 */

bool WizardPage1A::Create( wxWizard* parent )
{
  configstart = (Configstart *) parent;
////@begin WizardPage1A creation
	wxBitmap wizardBitmap(wxNullBitmap);
	wxWizardPage::Create( parent, wizardBitmap );

	CreateControls();
	if (GetSizer())
		GetSizer()->Fit(this);
////@end WizardPage1A creation
	return true;
}


/*
 * WizardPage1A destructor
 */

WizardPage1A::~WizardPage1A()
{
////@begin WizardPage1A destruction
////@end WizardPage1A destruction
}


/*
 * Member initialisation
 */

void WizardPage1A::Init()
{
////@begin WizardPage1A member initialisation
	m_configcond = NULL;
	m_configstart3 = NULL;
	m_configdvalfile = NULL;
	m_configdvalsel = NULL;
	m_configdvalno = NULL;
	m_configdvalapp = NULL;
	m_configdvaldoc = NULL;
	m_configdvalstart = NULL;
////@end WizardPage1A member initialisation
}


/*
 * Control creation for WizardPage1A
 */

void WizardPage1A::CreateControls()
{    
////@begin WizardPage1A content construction
	WizardPage1A* itemWizardPage26 = this;

	wxBoxSizer* itemBoxSizer27 = new wxBoxSizer(wxVERTICAL);
	itemWizardPage26->SetSizer(itemBoxSizer27);

	wxBoxSizer* itemBoxSizer28 = new wxBoxSizer(wxHORIZONTAL);
	itemBoxSizer27->Add(itemBoxSizer28, 0, wxALIGN_CENTER_HORIZONTAL|wxLEFT|wxRIGHT|wxTOP, 5);

	m_configcond = new wxStaticText( itemWizardPage26, ID_CONFIGCOND, _("Conditional Options"), wxDefaultPosition, wxDefaultSize, 0 );
	m_configcond->SetFont(wxFont(8, wxSWISS, wxNORMAL, wxBOLD, false, wxT("Tahoma")));
	itemBoxSizer28->Add(m_configcond, 0, wxALIGN_CENTER_VERTICAL|wxLEFT|wxRIGHT|wxTOP, 5);

	wxBoxSizer* itemBoxSizer30 = new wxBoxSizer(wxHORIZONTAL);
	itemBoxSizer27->Add(itemBoxSizer30, 0, wxALIGN_CENTER_HORIZONTAL|wxLEFT|wxRIGHT|wxBOTTOM, 5);

	wxStaticLine* itemStaticLine31 = new wxStaticLine( itemWizardPage26, wxID_STATIC, wxDefaultPosition, wxSize(100, -1), wxLI_HORIZONTAL );
	itemBoxSizer30->Add(itemStaticLine31, 0, wxGROW|wxALL, 5);

	wxBoxSizer* itemBoxSizer32 = new wxBoxSizer(wxHORIZONTAL);
	itemBoxSizer27->Add(itemBoxSizer32, 0, wxGROW|wxALL, 5);

	m_configstart3 = new wxStaticText( itemWizardPage26, ID_CONFIGSTART3, _("Specify path and name of ditaval file, if any:"), wxDefaultPosition, wxDefaultSize, 0 );
	itemBoxSizer32->Add(m_configstart3, 0, wxALIGN_CENTER_VERTICAL|wxLEFT|wxRIGHT|wxTOP, 5);

	wxBoxSizer* itemBoxSizer34 = new wxBoxSizer(wxHORIZONTAL);
	itemBoxSizer27->Add(itemBoxSizer34, 0, wxGROW|wxLEFT|wxRIGHT, 5);

	m_configdvalfile = new wxRadioButton( itemWizardPage26, ID_CONFIGDVALFILE, _("Use:"), wxDefaultPosition, wxDefaultSize, wxRB_GROUP );
	m_configdvalfile->SetValue(true);
	itemBoxSizer34->Add(m_configdvalfile, 0, wxALIGN_CENTER_VERTICAL|wxLEFT, 5);

	m_configdvalsel = new wxFilePickerCtrl( itemWizardPage26, ID_CONFIGDVALSEL, wxEmptyString, _("Select the ditaval file for this project:"), wxEmptyString, wxDefaultPosition, wxDefaultSize, wxFLP_USE_TEXTCTRL|wxFLP_OPEN|wxFLP_FILE_MUST_EXIST );
	itemBoxSizer34->Add(m_configdvalsel, 100, wxALIGN_CENTER_VERTICAL|wxLEFT|wxRIGHT, 5);

	wxBoxSizer* itemBoxSizer37 = new wxBoxSizer(wxHORIZONTAL);
	itemBoxSizer27->Add(itemBoxSizer37, 0, wxGROW|wxLEFT|wxRIGHT|wxBOTTOM, 5);

	m_configdvalno = new wxRadioButton( itemWizardPage26, ID_CONFIGDVALNO, _("None, no .ditaval file is needed."), wxDefaultPosition, wxDefaultSize, 0 );
	m_configdvalno->SetValue(false);
	itemBoxSizer37->Add(m_configdvalno, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

	wxBoxSizer* itemBoxSizer39 = new wxBoxSizer(wxHORIZONTAL);
	itemBoxSizer27->Add(itemBoxSizer39, 0, wxGROW|wxLEFT|wxRIGHT|wxTOP, 5);

	m_configdvalapp = new wxStaticText( itemWizardPage26, ID_CONFIGDVALAPP, _("Apply this setting to:"), wxDefaultPosition, wxDefaultSize, 0 );
	itemBoxSizer39->Add(m_configdvalapp, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

	wxBoxSizer* itemBoxSizer41 = new wxBoxSizer(wxHORIZONTAL);
	itemBoxSizer27->Add(itemBoxSizer41, 0, wxGROW|wxLEFT|wxRIGHT|wxTOP, 5);

	m_configdvaldoc = new wxRadioButton( itemWizardPage26, ID_CONFIGDVALDOC, _("All output types from this DITA source"), wxDefaultPosition, wxDefaultSize, wxRB_GROUP );
	m_configdvaldoc->SetValue(true);
	itemBoxSizer41->Add(m_configdvaldoc, 0, wxALIGN_CENTER_VERTICAL|wxLEFT, 5);

	wxBoxSizer* itemBoxSizer43 = new wxBoxSizer(wxHORIZONTAL);
	itemBoxSizer27->Add(itemBoxSizer43, 0, wxGROW|wxLEFT|wxRIGHT|wxBOTTOM, 5);

	m_configdvalstart = new wxRadioButton( itemWizardPage26, ID_CONFIGDVALSTART, _("This output type only"), wxDefaultPosition, wxDefaultSize, 0 );
	m_configdvalstart->SetValue(false);
	itemBoxSizer43->Add(m_configdvalstart, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

////@end WizardPage1A content construction

	(m_configdvalsel->GetTextCtrl())->SetEditable(false);
	m_configdvalfile->SetValue(configstart->m_configdvuse);
	m_configdvalno->SetValue(!configstart->m_configdvuse);
	if (configstart->m_configdval)
		m_configdvalsel->SetPath(*configstart->m_configdval);
	else
		m_configdvalsel->SetPath(*configstart->m_configsrcdir);
}


/*
 * Gets the previous page.
 */

wxWizardPage* WizardPage1A::GetPrev() const
{
	// return the previous page
	return configstart->m_wizardpage1;
}


/*
 * Gets the next page.
 */

wxWizardPage* WizardPage1A::GetNext() const
{
	// return the next page
	return configstart->m_wizardpage2;
}


/*
 * Should we show tooltips?
 */

bool WizardPage1A::ShowToolTips()
{
	return true;
}

/*
 * Get bitmap resources
 */

wxBitmap WizardPage1A::GetBitmapResource( const wxString& name )
{
	// Bitmap retrieval
////@begin WizardPage1A bitmap retrieval
	wxUnusedVar(name);
	return wxNullBitmap;
////@end WizardPage1A bitmap retrieval
}

/*
 * Get icon resources
 */

wxIcon WizardPage1A::GetIconResource( const wxString& name )
{
	// Icon retrieval
////@begin WizardPage1A icon retrieval
	wxUnusedVar(name);
	return wxNullIcon;
////@end WizardPage1A icon retrieval
}



/*
 * WizardPage2 type definition
 */

IMPLEMENT_DYNAMIC_CLASS( WizardPage2, wxWizardPageSimple )


/*
 * WizardPage2 event table definition
 */

BEGIN_EVENT_TABLE( WizardPage2, wxWizardPageSimple )

////@begin WizardPage2 event table entries
////@end WizardPage2 event table entries

END_EVENT_TABLE()


/*
 * WizardPage2 constructors
 */

WizardPage2::WizardPage2()
{
	Init();
}

WizardPage2::WizardPage2( wxWizard* parent )
{
	Init();
	Create( parent );
}


/*
 * WizardPage2 creator
 */

bool WizardPage2::Create( wxWizard* parent )
{
  configstart = (Configstart *) parent;
////@begin WizardPage2 creation
	wxBitmap wizardBitmap(wxNullBitmap);
	wxWizardPage::Create( parent, wizardBitmap );

	CreateControls();
	if (GetSizer())
		GetSizer()->Fit(this);
////@end WizardPage2 creation

	return true;
}


/*
 * WizardPage2 destructor
 */

WizardPage2::~WizardPage2()
{
////@begin WizardPage2 destruction
////@end WizardPage2 destruction
}


/*
 * Member initialisation
 */

void WizardPage2::Init()
{
////@begin WizardPage2 member initialisation
	m_configarchive = NULL;
	m_configaname = NULL;
	m_configanametxt = NULL;
	m_configanameapp = NULL;
	m_configanamedoc = NULL;
	m_configanamestart = NULL;
	m_configaver = NULL;
	m_configavertxt = NULL;
	m_configaverapp = NULL;
	m_configaverdoc = NULL;
	m_configaverstart = NULL;
////@end WizardPage2 member initialisation
}


/*
 * Control creation for WizardPage2
 */

void WizardPage2::CreateControls()
{    
////@begin WizardPage2 content construction
	WizardPage2* itemWizardPage45 = this;

	wxBoxSizer* itemBoxSizer46 = new wxBoxSizer(wxVERTICAL);
	itemWizardPage45->SetSizer(itemBoxSizer46);

	wxBoxSizer* itemBoxSizer47 = new wxBoxSizer(wxHORIZONTAL);
	itemBoxSizer46->Add(itemBoxSizer47, 0, wxALIGN_CENTER_HORIZONTAL|wxLEFT|wxRIGHT|wxTOP, 5);

	m_configarchive = new wxStaticText( itemWizardPage45, ID_CONFIGARCHIVE, _("Archive Name"), wxDefaultPosition, wxDefaultSize, 0 );
	m_configarchive->SetFont(wxFont(8, wxSWISS, wxNORMAL, wxBOLD, false, wxT("Tahoma")));
	itemBoxSizer47->Add(m_configarchive, 0, wxALIGN_CENTER_VERTICAL|wxLEFT|wxRIGHT|wxTOP, 5);

	wxBoxSizer* itemBoxSizer49 = new wxBoxSizer(wxHORIZONTAL);
	itemBoxSizer46->Add(itemBoxSizer49, 0, wxALIGN_CENTER_HORIZONTAL|wxLEFT|wxRIGHT|wxBOTTOM, 5);

	wxStaticLine* itemStaticLine50 = new wxStaticLine( itemWizardPage45, wxID_STATIC, wxDefaultPosition, wxSize(100, -1), wxLI_HORIZONTAL );
	itemBoxSizer49->Add(itemStaticLine50, 0, wxGROW|wxALL, 5);

	wxBoxSizer* itemBoxSizer51 = new wxBoxSizer(wxHORIZONTAL);
	itemBoxSizer46->Add(itemBoxSizer51, 0, wxGROW|wxALL, 5);

	m_configaname = new wxStaticText( itemWizardPage45, ID_CONFIGANAME, _("Archive name:"), wxDefaultPosition, wxDefaultSize, 0 );
	itemBoxSizer51->Add(m_configaname, 0, wxALIGN_CENTER_VERTICAL|wxLEFT|wxRIGHT|wxTOP, 5);

	m_configanametxt = new wxTextCtrl( itemWizardPage45, ID_CONFIGANAMETXT, wxEmptyString, wxDefaultPosition, wxSize(100, -1), wxTE_LEFT );
	itemBoxSizer51->Add(m_configanametxt, 100, wxALIGN_CENTER_VERTICAL|wxLEFT|wxRIGHT|wxTOP, 5);

	wxBoxSizer* itemBoxSizer54 = new wxBoxSizer(wxHORIZONTAL);
	itemBoxSizer46->Add(itemBoxSizer54, 0, wxGROW|wxLEFT|wxRIGHT|wxTOP, 5);

	m_configanameapp = new wxStaticText( itemWizardPage45, ID_CONFIGANAMEAPP, _("Apply this setting to:"), wxDefaultPosition, wxDefaultSize, 0 );
	itemBoxSizer54->Add(m_configanameapp, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

	wxBoxSizer* itemBoxSizer56 = new wxBoxSizer(wxHORIZONTAL);
	itemBoxSizer46->Add(itemBoxSizer56, 0, wxGROW|wxLEFT|wxRIGHT|wxTOP, 5);

	m_configanamedoc = new wxRadioButton( itemWizardPage45, ID_CONFIGANAMEDOC, _("All output types from this DITA source"), wxDefaultPosition, wxDefaultSize, wxRB_GROUP );
	m_configanamedoc->SetValue(true);
	itemBoxSizer56->Add(m_configanamedoc, 0, wxALIGN_CENTER_VERTICAL|wxLEFT, 5);

	wxBoxSizer* itemBoxSizer58 = new wxBoxSizer(wxHORIZONTAL);
	itemBoxSizer46->Add(itemBoxSizer58, 0, wxGROW|wxLEFT|wxRIGHT|wxBOTTOM, 5);

	m_configanamestart = new wxRadioButton( itemWizardPage45, ID_CONFIGANAMESTART, _("This output type only"), wxDefaultPosition, wxDefaultSize, 0 );
	m_configanamestart->SetValue(false);
	itemBoxSizer58->Add(m_configanamestart, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

	wxBoxSizer* itemBoxSizer60 = new wxBoxSizer(wxHORIZONTAL);
	itemBoxSizer46->Add(itemBoxSizer60, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);

	wxStaticLine* itemStaticLine61 = new wxStaticLine( itemWizardPage45, wxID_STATIC, wxDefaultPosition, wxSize(100, -1), wxLI_HORIZONTAL );
	itemBoxSizer60->Add(itemStaticLine61, 0, wxGROW|wxALL, 5);

	wxBoxSizer* itemBoxSizer62 = new wxBoxSizer(wxHORIZONTAL);
	itemBoxSizer46->Add(itemBoxSizer62, 0, wxGROW|wxALL, 5);

	m_configaver = new wxStaticText( itemWizardPage45, ID_CONFIGAVER, _("Archive version:"), wxDefaultPosition, wxDefaultSize, 0 );
	itemBoxSizer62->Add(m_configaver, 0, wxALIGN_CENTER_VERTICAL|wxLEFT|wxRIGHT|wxTOP, 5);

	m_configavertxt = new wxTextCtrl( itemWizardPage45, ID_CONFIGAVERTXT, wxEmptyString, wxDefaultPosition, wxSize(100, -1), 0 );
	itemBoxSizer62->Add(m_configavertxt, 100, wxALIGN_CENTER_VERTICAL|wxLEFT|wxRIGHT|wxTOP, 5);

	wxBoxSizer* itemBoxSizer65 = new wxBoxSizer(wxHORIZONTAL);
	itemBoxSizer46->Add(itemBoxSizer65, 0, wxGROW|wxLEFT|wxRIGHT|wxTOP, 5);

	m_configaverapp = new wxStaticText( itemWizardPage45, ID_CONFIGAVERAPP, _("Apply this setting to:"), wxDefaultPosition, wxDefaultSize, 0 );
	itemBoxSizer65->Add(m_configaverapp, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

	wxBoxSizer* itemBoxSizer67 = new wxBoxSizer(wxHORIZONTAL);
	itemBoxSizer46->Add(itemBoxSizer67, 0, wxGROW|wxLEFT|wxRIGHT|wxTOP, 5);

	m_configaverdoc = new wxRadioButton( itemWizardPage45, ID_CONFIGAVERDOC, _("All output types from this DITA source"), wxDefaultPosition, wxDefaultSize, wxRB_GROUP );
	m_configaverdoc->SetValue(true);
	itemBoxSizer67->Add(m_configaverdoc, 0, wxALIGN_CENTER_VERTICAL|wxLEFT, 5);

	wxBoxSizer* itemBoxSizer69 = new wxBoxSizer(wxHORIZONTAL);
	itemBoxSizer46->Add(itemBoxSizer69, 0, wxGROW|wxLEFT|wxRIGHT|wxBOTTOM, 5);

	m_configaverstart = new wxRadioButton( itemWizardPage45, ID_CONFIGAVERSTART, _("This output type only"), wxDefaultPosition, wxDefaultSize, 0 );
	m_configaverstart->SetValue(false);
	itemBoxSizer69->Add(m_configaverstart, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

////@end WizardPage2 content construction

	m_configanametxt->SetValue(*configstart->m_configanamestr);
	m_configavertxt->SetValue(*configstart->m_configaverstr);
}


/*
 * Gets the previous page.
 */

wxWizardPage* WizardPage2::GetPrev() const
{
	// return the previous page
	return configstart->m_wizardpage1a;
}


/*
 * Gets the next page.
 */

wxWizardPage* WizardPage2::GetNext() const
{
	// return the next page
	return configstart->m_wizardpage2a;
}


/*
 * Should we show tooltips?
 */

bool WizardPage2::ShowToolTips()
{
	return true;
}

/*
 * Get bitmap resources
 */

wxBitmap WizardPage2::GetBitmapResource( const wxString& name )
{
	// Bitmap retrieval
////@begin WizardPage2 bitmap retrieval
	wxUnusedVar(name);
	return wxNullBitmap;
////@end WizardPage2 bitmap retrieval
}

/*
 * Get icon resources
 */

wxIcon WizardPage2::GetIconResource( const wxString& name )
{
	// Icon retrieval
////@begin WizardPage2 icon retrieval
	wxUnusedVar(name);
	return wxNullIcon;
////@end WizardPage2 icon retrieval
}



/*
 * WizardPage2A type definition
 */

IMPLEMENT_DYNAMIC_CLASS( WizardPage2A, wxWizardPage )


/*
 * WizardPage2A event table definition
 */

BEGIN_EVENT_TABLE( WizardPage2A, wxWizardPage )

////@begin WizardPage2A event table entries
////@end WizardPage2A event table entries

END_EVENT_TABLE()


/*
 * WizardPage2A constructors
 */

WizardPage2A::WizardPage2A()
{
	Init();
}

WizardPage2A::WizardPage2A( wxWizard* parent )
{
	Init();
	Create( parent );
}


/*
 * WizardPage2A creator
 */

bool WizardPage2A::Create( wxWizard* parent )
{
  configstart = (Configstart *) parent;
////@begin WizardPage2A creation
	wxBitmap wizardBitmap(wxNullBitmap);
	wxWizardPage::Create( parent, wizardBitmap );

	CreateControls();
	if (GetSizer())
		GetSizer()->Fit(this);
////@end WizardPage2A creation
	return true;
}


/*
 * WizardPage2A destructor
 */

WizardPage2A::~WizardPage2A()
{
////@begin WizardPage2A destruction
////@end WizardPage2A destruction
}


/*
 * Member initialisation
 */

void WizardPage2A::Init()
{
////@begin WizardPage2A member initialisation
	m_configgraph = NULL;
	m_configgrdir = NULL;
	m_configgrsel = NULL;
	m_configgrdapp = NULL;
	m_configgrddoc = NULL;
	m_configgrdstart = NULL;
	m_configgrfiles = NULL;
	m_configgrtxt = NULL;
	m_configgrfapp = NULL;
	m_configgrfdoc = NULL;
	m_configgrfstart = NULL;
////@end WizardPage2A member initialisation
}


/*
 * Control creation for WizardPage2A
 */

void WizardPage2A::CreateControls()
{    
////@begin WizardPage2A content construction
	WizardPage2A* itemWizardPage71 = this;

	wxBoxSizer* itemBoxSizer72 = new wxBoxSizer(wxVERTICAL);
	itemWizardPage71->SetSizer(itemBoxSizer72);

	wxBoxSizer* itemBoxSizer73 = new wxBoxSizer(wxHORIZONTAL);
	itemBoxSizer72->Add(itemBoxSizer73, 0, wxALIGN_CENTER_HORIZONTAL|wxLEFT|wxRIGHT|wxTOP, 5);

	m_configgraph = new wxStaticText( itemWizardPage71, ID_CONFIGGRAPH, _("Graphics Copying"), wxDefaultPosition, wxDefaultSize, 0 );
	m_configgraph->SetFont(wxFont(8, wxSWISS, wxNORMAL, wxBOLD, false, wxT("Tahoma")));
	itemBoxSizer73->Add(m_configgraph, 0, wxALIGN_CENTER_VERTICAL|wxLEFT|wxRIGHT|wxTOP, 5);

	wxBoxSizer* itemBoxSizer75 = new wxBoxSizer(wxHORIZONTAL);
	itemBoxSizer72->Add(itemBoxSizer75, 0, wxALIGN_CENTER_HORIZONTAL|wxLEFT|wxRIGHT|wxBOTTOM, 5);

	wxStaticLine* itemStaticLine76 = new wxStaticLine( itemWizardPage71, wxID_STATIC, wxDefaultPosition, wxSize(100, -1), wxLI_HORIZONTAL );
	itemBoxSizer75->Add(itemStaticLine76, 0, wxGROW|wxALL, 5);

	wxBoxSizer* itemBoxSizer77 = new wxBoxSizer(wxHORIZONTAL);
	itemBoxSizer72->Add(itemBoxSizer77, 0, wxGROW|wxALL, 5);

	m_configgrdir = new wxStaticText( itemWizardPage71, ID_CONFIGGRDIR, _("Copy graphics from:"), wxDefaultPosition, wxDefaultSize, 0 );
	itemBoxSizer77->Add(m_configgrdir, 0, wxALIGN_CENTER_VERTICAL|wxLEFT|wxRIGHT|wxTOP, 5);

	m_configgrsel = new wxDirPickerCtrl( itemWizardPage71, ID_CONFIGGRSEL, wxEmptyString, _("Select directory containing graphics to copy:"), wxDefaultPosition, wxSize(100, -1), wxDIRP_USE_TEXTCTRL|wxDIRP_DIR_MUST_EXIST );
	itemBoxSizer77->Add(m_configgrsel, 100, wxALIGN_CENTER_VERTICAL|wxLEFT|wxRIGHT|wxTOP, 5);

	wxBoxSizer* itemBoxSizer80 = new wxBoxSizer(wxHORIZONTAL);
	itemBoxSizer72->Add(itemBoxSizer80, 0, wxGROW|wxLEFT|wxRIGHT|wxTOP, 5);

	m_configgrdapp = new wxStaticText( itemWizardPage71, ID_CONFIGGRDAPP, _("Apply this setting to:"), wxDefaultPosition, wxDefaultSize, 0 );
	itemBoxSizer80->Add(m_configgrdapp, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

	wxBoxSizer* itemBoxSizer82 = new wxBoxSizer(wxHORIZONTAL);
	itemBoxSizer72->Add(itemBoxSizer82, 0, wxGROW|wxLEFT|wxRIGHT|wxTOP, 5);

	m_configgrddoc = new wxRadioButton( itemWizardPage71, ID_CONFIGGRDDOC, _("All output types from this DITA source"), wxDefaultPosition, wxDefaultSize, wxRB_GROUP );
	m_configgrddoc->SetValue(true);
	itemBoxSizer82->Add(m_configgrddoc, 0, wxALIGN_CENTER_VERTICAL|wxLEFT, 5);

	wxBoxSizer* itemBoxSizer84 = new wxBoxSizer(wxHORIZONTAL);
	itemBoxSizer72->Add(itemBoxSizer84, 0, wxGROW|wxLEFT|wxRIGHT|wxBOTTOM, 5);

	m_configgrdstart = new wxRadioButton( itemWizardPage71, ID_CONFIGGRDSTART, _("This output type only"), wxDefaultPosition, wxDefaultSize, 0 );
	m_configgrdstart->SetValue(false);
	itemBoxSizer84->Add(m_configgrdstart, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

	wxBoxSizer* itemBoxSizer86 = new wxBoxSizer(wxHORIZONTAL);
	itemBoxSizer72->Add(itemBoxSizer86, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);

	wxStaticLine* itemStaticLine87 = new wxStaticLine( itemWizardPage71, wxID_STATIC, wxDefaultPosition, wxSize(100, -1), wxLI_HORIZONTAL );
	itemBoxSizer86->Add(itemStaticLine87, 0, wxGROW|wxALL, 5);

	wxBoxSizer* itemBoxSizer88 = new wxBoxSizer(wxHORIZONTAL);
	itemBoxSizer72->Add(itemBoxSizer88, 0, wxGROW|wxALL, 5);

	m_configgrfiles = new wxStaticText( itemWizardPage71, ID_CONFIGGRFILES, _("Graphic files to copy:"), wxDefaultPosition, wxDefaultSize, 0 );
	itemBoxSizer88->Add(m_configgrfiles, 0, wxALIGN_CENTER_VERTICAL|wxLEFT|wxRIGHT|wxTOP, 5);

	m_configgrtxt = new wxTextCtrl( itemWizardPage71, ID_CONFIGGRTXT, wxEmptyString, wxDefaultPosition, wxSize(100, -1), wxTE_LEFT );
	itemBoxSizer88->Add(m_configgrtxt, 100, wxALIGN_CENTER_VERTICAL|wxLEFT|wxRIGHT|wxTOP, 5);

	wxBoxSizer* itemBoxSizer91 = new wxBoxSizer(wxHORIZONTAL);
	itemBoxSizer72->Add(itemBoxSizer91, 0, wxGROW|wxLEFT|wxRIGHT|wxTOP, 5);

	m_configgrfapp = new wxStaticText( itemWizardPage71, ID_CONFIGGRFAPP, _("Apply this setting to:"), wxDefaultPosition, wxDefaultSize, 0 );
	itemBoxSizer91->Add(m_configgrfapp, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

	wxBoxSizer* itemBoxSizer93 = new wxBoxSizer(wxHORIZONTAL);
	itemBoxSizer72->Add(itemBoxSizer93, 0, wxGROW|wxLEFT|wxRIGHT|wxTOP, 5);

	m_configgrfdoc = new wxRadioButton( itemWizardPage71, ID_CONFIGGRFDOC, _("All output types from this DITA source"), wxDefaultPosition, wxDefaultSize, wxRB_GROUP );
	m_configgrfdoc->SetValue(true);
	itemBoxSizer93->Add(m_configgrfdoc, 0, wxALIGN_CENTER_VERTICAL|wxLEFT, 5);

	wxBoxSizer* itemBoxSizer95 = new wxBoxSizer(wxHORIZONTAL);
	itemBoxSizer72->Add(itemBoxSizer95, 0, wxGROW|wxLEFT|wxRIGHT|wxBOTTOM, 5);

	m_configgrfstart = new wxRadioButton( itemWizardPage71, ID_CONFIGGRFSTART, _("This output type only"), wxDefaultPosition, wxDefaultSize, 0 );
	m_configgrfstart->SetValue(false);
	itemBoxSizer95->Add(m_configgrfstart, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

////@end WizardPage2A content construction

	(m_configgrsel->GetTextCtrl())->SetEditable(false);

	m_configgrsel->SetPath(*configstart->m_configgrdirstr);
	m_configgrtxt->SetValue(*configstart->m_configgrfilesstr);
}


/*
 * Gets the previous page.
 */

wxWizardPage* WizardPage2A::GetPrev() const
{
	// return the previous page
	return configstart->m_wizardpage2;
}


/*
 * Gets the next page.
 */

wxWizardPage* WizardPage2A::GetNext() const
{
	// return the next page
	if (!configstart->m_configouttype->Cmp("Eclipse Help"))
		return configstart->m_wizardpage4;

	if (!configstart->m_configouttype->Cmp("Word RTF"))
		return configstart->m_wizardpage5;
	if (!configstart->m_configouttype->Cmp("WinHelp"))
		return configstart->m_wizardpage5;

	if (!configstart->m_configouttype->Cmp("Flat XML")
	 || !configstart->m_configouttype->Cmp("DITA")
	 || !configstart->m_configouttype->Cmp("DocBook")) {
		configstart->SetFinalText(2);
		return configstart->m_wizardpageend;
	}

	return configstart->m_wizardpage3;
}


/*
 * Should we show tooltips?
 */

bool WizardPage2A::ShowToolTips()
{
	return true;
}

/*
 * Get bitmap resources
 */

wxBitmap WizardPage2A::GetBitmapResource( const wxString& name )
{
	// Bitmap retrieval
////@begin WizardPage2A bitmap retrieval
	wxUnusedVar(name);
	return wxNullBitmap;
////@end WizardPage2A bitmap retrieval
}

/*
 * Get icon resources
 */

wxIcon WizardPage2A::GetIconResource( const wxString& name )
{
	// Icon retrieval
////@begin WizardPage2A icon retrieval
	wxUnusedVar(name);
	return wxNullIcon;
////@end WizardPage2A icon retrieval
}



/*
 * WizardPage3 type definition
 */

IMPLEMENT_DYNAMIC_CLASS( WizardPage3, wxWizardPage )


/*
 * WizardPage3 event table definition
 */

BEGIN_EVENT_TABLE( WizardPage3, wxWizardPage )

////@begin WizardPage3 event table entries
////@end WizardPage3 event table entries

END_EVENT_TABLE()


/*
 * WizardPage3 constructors
 */

WizardPage3::WizardPage3()
{
	Init();
}

WizardPage3::WizardPage3( wxWizard* parent )
{
	Init();
	Create( parent );
}


/*
 * WizardPage3 creator
 */

bool WizardPage3::Create( wxWizard* parent )
{
  configstart = (Configstart *) parent;
////@begin WizardPage3 creation
	wxBitmap wizardBitmap(wxNullBitmap);
	wxWizardPage::Create( parent, wizardBitmap );

	CreateControls();
	if (GetSizer())
		GetSizer()->Fit(this);
////@end WizardPage3 creation

	return true;
}


/*
 * WizardPage3 destructor
 */

WizardPage3::~WizardPage3()
{
////@begin WizardPage3 destruction
////@end WizardPage3 destruction
}


/*
 * Member initialisation
 */

void WizardPage3::Init()
{
////@begin WizardPage3 member initialisation
	m_confightml = NULL;
	m_configtitle = NULL;
	m_configtitletxt = NULL;
	m_confighp = NULL;
	m_confighpftxt = NULL;
	m_confightitle = NULL;
	m_confightitletxt = NULL;
	m_confightopic = NULL;
	m_confightopictxt = NULL;
	m_configohtl = NULL;
	m_configohtltxt = NULL;
////@end WizardPage3 member initialisation
}


/*
 * Control creation for WizardPage3
 */

void WizardPage3::CreateControls()
{    
////@begin WizardPage3 content construction
	WizardPage3* itemWizardPage97 = this;

	wxBoxSizer* itemBoxSizer98 = new wxBoxSizer(wxVERTICAL);
	itemWizardPage97->SetSizer(itemBoxSizer98);

	wxBoxSizer* itemBoxSizer99 = new wxBoxSizer(wxHORIZONTAL);
	itemBoxSizer98->Add(itemBoxSizer99, 0, wxALIGN_CENTER_HORIZONTAL|wxLEFT|wxRIGHT|wxTOP, 5);

	m_confightml = new wxStaticText( itemWizardPage97, ID_CONFIGHTML, _("HTML Settings"), wxDefaultPosition, wxDefaultSize, 0 );
	m_confightml->SetFont(wxFont(8, wxSWISS, wxNORMAL, wxBOLD, false, wxT("Tahoma")));
	itemBoxSizer99->Add(m_confightml, 0, wxALIGN_CENTER_VERTICAL|wxLEFT|wxRIGHT|wxTOP, 5);

	wxBoxSizer* itemBoxSizer101 = new wxBoxSizer(wxHORIZONTAL);
	itemBoxSizer98->Add(itemBoxSizer101, 0, wxALIGN_CENTER_HORIZONTAL|wxLEFT|wxRIGHT|wxBOTTOM, 5);

	wxStaticLine* itemStaticLine102 = new wxStaticLine( itemWizardPage97, wxID_STATIC, wxDefaultPosition, wxSize(100, -1), wxLI_HORIZONTAL );
	itemBoxSizer101->Add(itemStaticLine102, 0, wxGROW|wxALL, 5);

	wxBoxSizer* itemBoxSizer103 = new wxBoxSizer(wxHORIZONTAL);
	itemBoxSizer98->Add(itemBoxSizer103, 0, wxGROW|wxALL, 5);

	m_configtitle = new wxStaticText( itemWizardPage97, ID_CONFIGTITLE, _("Default HTML title:"), wxDefaultPosition, wxDefaultSize, 0 );
	itemBoxSizer103->Add(m_configtitle, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

	m_configtitletxt = new wxTextCtrl( itemWizardPage97, ID_CONFIGTITLETXT, wxEmptyString, wxDefaultPosition, wxSize(100, -1), wxTE_LEFT );
	itemBoxSizer103->Add(m_configtitletxt, 100, wxALIGN_CENTER_VERTICAL|wxALL, 5);

	wxBoxSizer* itemBoxSizer106 = new wxBoxSizer(wxHORIZONTAL);
	itemBoxSizer98->Add(itemBoxSizer106, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);

	wxStaticLine* itemStaticLine107 = new wxStaticLine( itemWizardPage97, wxID_STATIC, wxDefaultPosition, wxSize(100, -1), wxLI_HORIZONTAL );
	itemBoxSizer106->Add(itemStaticLine107, 0, wxGROW|wxALL, 5);

	wxBoxSizer* itemBoxSizer108 = new wxBoxSizer(wxHORIZONTAL);
	itemBoxSizer98->Add(itemBoxSizer108, 0, wxGROW|wxALL, 5);

	m_confighp = new wxStaticText( itemWizardPage97, ID_CONFIGHPF, _("Help project file:"), wxDefaultPosition, wxDefaultSize, 0 );
	itemBoxSizer108->Add(m_confighp, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

	m_confighpftxt = new wxTextCtrl( itemWizardPage97, ID_CONFIGHPFTXT, wxEmptyString, wxDefaultPosition, wxSize(100, -1), wxTE_LEFT );
	itemBoxSizer108->Add(m_confighpftxt, 100, wxALIGN_CENTER_VERTICAL|wxALL, 5);

	wxBoxSizer* itemBoxSizer111 = new wxBoxSizer(wxHORIZONTAL);
	itemBoxSizer98->Add(itemBoxSizer111, 0, wxGROW|wxALL, 5);

	m_confightitle = new wxStaticText( itemWizardPage97, ID_CONFIGHTITLE, _("Help file title:"), wxDefaultPosition, wxDefaultSize, 0 );
	itemBoxSizer111->Add(m_confightitle, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

	m_confightitletxt = new wxTextCtrl( itemWizardPage97, ID_CONFIGHTITLETXT, wxEmptyString, wxDefaultPosition, wxSize(100, -1), wxTE_LEFT );
	itemBoxSizer111->Add(m_confightitletxt, 100, wxALIGN_CENTER_VERTICAL|wxLEFT|wxRIGHT, 5);

	wxBoxSizer* itemBoxSizer114 = new wxBoxSizer(wxHORIZONTAL);
	itemBoxSizer98->Add(itemBoxSizer114, 0, wxGROW|wxALL, 5);

	m_confightopic = new wxStaticText( itemWizardPage97, ID_CONFIGHTOPIC, _("Starting topic name:"), wxDefaultPosition, wxDefaultSize, 0 );
	itemBoxSizer114->Add(m_confightopic, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

	m_confightopictxt = new wxTextCtrl( itemWizardPage97, ID_CONFIGHTOPICTXT, wxEmptyString, wxDefaultPosition, wxSize(100, -1), wxTE_LEFT );
	itemBoxSizer114->Add(m_confightopictxt, 100, wxALIGN_CENTER_VERTICAL|wxALL, 5);

	wxBoxSizer* itemBoxSizer117 = new wxBoxSizer(wxHORIZONTAL);
	itemBoxSizer98->Add(itemBoxSizer117, 0, wxGROW|wxALL, 5);

	m_configohtl = new wxStaticText( itemWizardPage97, ID_CONFIGOHTL, _("Top left panel code: single line, no apostrophes, end with backslash:"), wxDefaultPosition, wxDefaultSize, 0 );
	itemBoxSizer117->Add(m_configohtl, 0, wxALIGN_CENTER_VERTICAL|wxLEFT|wxRIGHT|wxTOP, 5);

	wxBoxSizer* itemBoxSizer119 = new wxBoxSizer(wxHORIZONTAL);
	itemBoxSizer98->Add(itemBoxSizer119, 0, wxGROW|wxLEFT|wxRIGHT|wxBOTTOM, 5);

	m_configohtltxt = new wxTextCtrl( itemWizardPage97, ID_CONFIGOHTLTXT, wxEmptyString, wxDefaultPosition, wxSize(100, 20), 0 );
	itemBoxSizer119->Add(m_configohtltxt, 100, wxALIGN_CENTER_VERTICAL|wxALL, 5);

////@end WizardPage3 content construction

	if (configstart->m_configouttype->Cmp("OmniHelp")) {
		// hide OH elements
		m_configohtl->Hide();
		m_configohtltxt->Hide();
	}
	else {
		m_configohtl->Show();
		m_configohtltxt->Show();
	}

	if (!configstart->m_configouttype->Cmp("HTML 4.0")
	 || !configstart->m_configouttype->Cmp("XHTML 1.0")) {
		// hide Help elements
		m_confighp->Hide();
		m_confighpftxt->Hide();
		m_confightitle->Hide();
		m_confightitletxt->Hide();
		m_confightopic->Hide();
		m_confightopictxt->Hide();
	}
}


/*
 * Gets the previous page.
 */

wxWizardPage* WizardPage3::GetPrev() const
{
	// return the previous page
	return configstart->m_wizardpage2a;
}


/*
 * Gets the next page.
 */

wxWizardPage* WizardPage3::GetNext() const
{
	configstart->SetFinalText(3);

	// return the next page
	return configstart->m_wizardpageend;
}


/*
 * Should we show tooltips?
 */

bool WizardPage3::ShowToolTips()
{
	return true;
}

/*
 * Get bitmap resources
 */

wxBitmap WizardPage3::GetBitmapResource( const wxString& name )
{
	// Bitmap retrieval
////@begin WizardPage3 bitmap retrieval
	wxUnusedVar(name);
	return wxNullBitmap;
////@end WizardPage3 bitmap retrieval
}

/*
 * Get icon resources
 */

wxIcon WizardPage3::GetIconResource( const wxString& name )
{
	// Icon retrieval
////@begin WizardPage3 icon retrieval
	wxUnusedVar(name);
	return wxNullIcon;
////@end WizardPage3 icon retrieval
}



/*
 * WizardPage4 type definition
 */

IMPLEMENT_DYNAMIC_CLASS( WizardPage4, wxWizardPage )


/*
 * WizardPage4 event table definition
 */

BEGIN_EVENT_TABLE( WizardPage4, wxWizardPage )

////@begin WizardPage4 event table entries
////@end WizardPage4 event table entries

END_EVENT_TABLE()


/*
 * WizardPage4 constructors
 */

WizardPage4::WizardPage4()
{
	Init();
}

WizardPage4::WizardPage4( wxWizard* parent )
{
	Init();
	Create( parent );
}


/*
 * WizardPage4 creator
 */

bool WizardPage4::Create( wxWizard* parent )
{
  configstart = (Configstart *) parent;
////@begin WizardPage4 creation
	wxBitmap wizardBitmap(wxNullBitmap);
	wxWizardPage::Create( parent, wizardBitmap );

	CreateControls();
	if (GetSizer())
		GetSizer()->Fit(this);
////@end WizardPage4 creation

	return true;
}


/*
 * WizardPage4 destructor
 */

WizardPage4::~WizardPage4()
{
////@begin WizardPage4 destruction
////@end WizardPage4 destruction
}


/*
 * Member initialisation
 */

void WizardPage4::Init()
{
////@begin WizardPage4 member initialisation
	m_configeh = NULL;
	m_configehtitle = NULL;
	m_configehtitletxt = NULL;
	m_configehpn = NULL;
	m_configehpntxt = NULL;
	m_configehpid = NULL;
	m_configehpidtxt = NULL;
	m_configehprov = NULL;
	m_configehprovtxt = NULL;
	m_configehtopic = NULL;
	m_configehtopictxt = NULL;
////@end WizardPage4 member initialisation
}


/*
 * Control creation for WizardPage4
 */

void WizardPage4::CreateControls()
{    
////@begin WizardPage4 content construction
	WizardPage4* itemWizardPage121 = this;

	wxBoxSizer* itemBoxSizer122 = new wxBoxSizer(wxVERTICAL);
	itemWizardPage121->SetSizer(itemBoxSizer122);

	wxBoxSizer* itemBoxSizer123 = new wxBoxSizer(wxHORIZONTAL);
	itemBoxSizer122->Add(itemBoxSizer123, 0, wxALIGN_CENTER_HORIZONTAL|wxLEFT|wxRIGHT|wxTOP, 5);

	m_configeh = new wxStaticText( itemWizardPage121, ID_CONFIGEH, _("Eclipse Settings"), wxDefaultPosition, wxDefaultSize, 0 );
	m_configeh->SetFont(wxFont(8, wxSWISS, wxNORMAL, wxBOLD, false, wxT("Tahoma")));
	itemBoxSizer123->Add(m_configeh, 0, wxALIGN_CENTER_VERTICAL|wxLEFT|wxRIGHT|wxTOP, 5);

	wxBoxSizer* itemBoxSizer125 = new wxBoxSizer(wxHORIZONTAL);
	itemBoxSizer122->Add(itemBoxSizer125, 0, wxALIGN_CENTER_HORIZONTAL|wxLEFT|wxRIGHT|wxBOTTOM, 5);

	wxStaticLine* itemStaticLine126 = new wxStaticLine( itemWizardPage121, wxID_STATIC, wxDefaultPosition, wxSize(100, -1), wxLI_HORIZONTAL );
	itemBoxSizer125->Add(itemStaticLine126, 0, wxGROW|wxALL, 5);

	wxBoxSizer* itemBoxSizer127 = new wxBoxSizer(wxHORIZONTAL);
	itemBoxSizer122->Add(itemBoxSizer127, 0, wxGROW|wxALL, 5);

	m_configehtitle = new wxStaticText( itemWizardPage121, ID_CONFIGEHTITLE, _("Default HTML title:"), wxDefaultPosition, wxDefaultSize, 0 );
	itemBoxSizer127->Add(m_configehtitle, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

	m_configehtitletxt = new wxTextCtrl( itemWizardPage121, ID_CONFIGEHTITLETXT, wxEmptyString, wxDefaultPosition, wxSize(100, -1), wxTE_LEFT );
	itemBoxSizer127->Add(m_configehtitletxt, 100, wxALIGN_CENTER_VERTICAL|wxALL, 5);

	wxBoxSizer* itemBoxSizer130 = new wxBoxSizer(wxHORIZONTAL);
	itemBoxSizer122->Add(itemBoxSizer130, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);

	wxStaticLine* itemStaticLine131 = new wxStaticLine( itemWizardPage121, wxID_STATIC, wxDefaultPosition, wxSize(100, -1), wxLI_HORIZONTAL );
	itemBoxSizer130->Add(itemStaticLine131, 0, wxGROW|wxALL, 5);

	wxBoxSizer* itemBoxSizer132 = new wxBoxSizer(wxHORIZONTAL);
	itemBoxSizer122->Add(itemBoxSizer132, 0, wxGROW|wxALL, 5);

	m_configehpn = new wxStaticText( itemWizardPage121, ID_CONFIGEHPN, _("Plugin name:"), wxDefaultPosition, wxDefaultSize, 0 );
	itemBoxSizer132->Add(m_configehpn, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

	m_configehpntxt = new wxTextCtrl( itemWizardPage121, ID_CONFIGEHPNTXT, wxEmptyString, wxDefaultPosition, wxSize(100, -1), wxTE_LEFT );
	itemBoxSizer132->Add(m_configehpntxt, 100, wxALIGN_CENTER_VERTICAL|wxALL, 5);

	wxBoxSizer* itemBoxSizer135 = new wxBoxSizer(wxHORIZONTAL);
	itemBoxSizer122->Add(itemBoxSizer135, 0, wxGROW|wxALL, 5);

	m_configehpid = new wxStaticText( itemWizardPage121, ID_CONFIGEHPID, _("Plugin ID:"), wxDefaultPosition, wxDefaultSize, 0 );
	itemBoxSizer135->Add(m_configehpid, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

	m_configehpidtxt = new wxTextCtrl( itemWizardPage121, ID_CONFIGEHPIDTXT, wxEmptyString, wxDefaultPosition, wxSize(100, -1), wxTE_LEFT );
	itemBoxSizer135->Add(m_configehpidtxt, 100, wxALIGN_CENTER_VERTICAL|wxALL, 5);

	wxBoxSizer* itemBoxSizer138 = new wxBoxSizer(wxHORIZONTAL);
	itemBoxSizer122->Add(itemBoxSizer138, 0, wxGROW|wxALL, 5);

	m_configehprov = new wxStaticText( itemWizardPage121, ID_CONFIGEHPROV, _("Plugin provider:"), wxDefaultPosition, wxDefaultSize, 0 );
	itemBoxSizer138->Add(m_configehprov, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

	m_configehprovtxt = new wxTextCtrl( itemWizardPage121, ID_CONFIGEHPROVTXT, wxEmptyString, wxDefaultPosition, wxSize(100, -1), wxTE_LEFT );
	itemBoxSizer138->Add(m_configehprovtxt, 100, wxALIGN_CENTER_VERTICAL|wxALL, 5);

	wxBoxSizer* itemBoxSizer141 = new wxBoxSizer(wxHORIZONTAL);
	itemBoxSizer122->Add(itemBoxSizer141, 0, wxGROW|wxALL, 5);

	m_configehtopic = new wxStaticText( itemWizardPage121, ID_CONFIGEHTOPIC, _("Starting topic name:"), wxDefaultPosition, wxDefaultSize, 0 );
	itemBoxSizer141->Add(m_configehtopic, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

	m_configehtopictxt = new wxTextCtrl( itemWizardPage121, ID_CONFIGEHTOPICTXT, wxEmptyString, wxDefaultPosition, wxSize(100, -1), wxTE_LEFT );
	itemBoxSizer141->Add(m_configehtopictxt, 100, wxALIGN_CENTER_VERTICAL|wxALL, 5);

////@end WizardPage4 content construction
}


/*
 * Gets the previous page.
 */

wxWizardPage* WizardPage4::GetPrev() const
{
	// return the previous page
	return configstart->m_wizardpage2a;
}


/*
 * Gets the next page.
 */

wxWizardPage* WizardPage4::GetNext() const
{
	configstart->SetFinalText(4);

	// return the next page
	return configstart->m_wizardpageend;
}


/*
 * Should we show tooltips?
 */

bool WizardPage4::ShowToolTips()
{
	return true;
}

/*
 * Get bitmap resources
 */

wxBitmap WizardPage4::GetBitmapResource( const wxString& name )
{
	// Bitmap retrieval
////@begin WizardPage4 bitmap retrieval
	wxUnusedVar(name);
	return wxNullBitmap;
////@end WizardPage4 bitmap retrieval
}

/*
 * Get icon resources
 */

wxIcon WizardPage4::GetIconResource( const wxString& name )
{
	// Icon retrieval
////@begin WizardPage4 icon retrieval
	wxUnusedVar(name);
	return wxNullIcon;
////@end WizardPage4 icon retrieval
}



/*
 * WizardPage5 type definition
 */

IMPLEMENT_DYNAMIC_CLASS( WizardPage5, wxWizardPage )


/*
 * WizardPage5 event table definition
 */

BEGIN_EVENT_TABLE( WizardPage5, wxWizardPage )

////@begin WizardPage5 event table entries
////@end WizardPage5 event table entries

END_EVENT_TABLE()


/*
 * WizardPage5 constructors
 */

WizardPage5::WizardPage5()
{
	Init();
}

WizardPage5::WizardPage5( wxWizard* parent )
{
	Init();
	Create( parent );
}


/*
 * WizardPage5 creator
 */

bool WizardPage5::Create( wxWizard* parent )
{
  configstart = (Configstart *) parent;
////@begin WizardPage5 creation
	wxBitmap wizardBitmap(wxNullBitmap);
	wxWizardPage::Create( parent, wizardBitmap );

	CreateControls();
	if (GetSizer())
		GetSizer()->Fit(this);
////@end WizardPage5 creation

	return true;
}


/*
 * WizardPage5 destructor
 */

WizardPage5::~WizardPage5()
{
////@begin WizardPage5 destruction
////@end WizardPage5 destruction
}


/*
 * Member initialisation
 */

void WizardPage5::Init()
{
////@begin WizardPage5 member initialisation
	m_configrtf = NULL;
	m_configrtfid = NULL;
	m_configrtfidtxt = NULL;
	m_configwv = NULL;
	m_configwv97 = NULL;
	m_configwv2k = NULL;
	m_configwvxp = NULL;
	m_configwv2003 = NULL;
	m_confighpj = NULL;
	m_confighpjtxt = NULL;
	m_configcnttitle = NULL;
	m_configcnttitletxt = NULL;
	m_configwhtopic = NULL;
	m_configwhtopictxt = NULL;
////@end WizardPage5 member initialisation
}


/*
 * Control creation for WizardPage5
 */

void WizardPage5::CreateControls()
{    
////@begin WizardPage5 content construction
	WizardPage5* itemWizardPage144 = this;

	wxBoxSizer* itemBoxSizer145 = new wxBoxSizer(wxVERTICAL);
	itemWizardPage144->SetSizer(itemBoxSizer145);

	wxBoxSizer* itemBoxSizer146 = new wxBoxSizer(wxHORIZONTAL);
	itemBoxSizer145->Add(itemBoxSizer146, 0, wxALIGN_CENTER_HORIZONTAL|wxLEFT|wxRIGHT|wxTOP, 5);

	m_configrtf = new wxStaticText( itemWizardPage144, ID_CONFIGRTF, _("RTF Settings"), wxDefaultPosition, wxDefaultSize, 0 );
	m_configrtf->SetFont(wxFont(8, wxSWISS, wxNORMAL, wxBOLD, false, wxT("Tahoma")));
	itemBoxSizer146->Add(m_configrtf, 0, wxALIGN_CENTER_VERTICAL|wxLEFT|wxRIGHT|wxTOP, 5);

	wxBoxSizer* itemBoxSizer148 = new wxBoxSizer(wxHORIZONTAL);
	itemBoxSizer145->Add(itemBoxSizer148, 0, wxALIGN_CENTER_HORIZONTAL|wxLEFT|wxRIGHT|wxBOTTOM, 5);

	wxStaticLine* itemStaticLine149 = new wxStaticLine( itemWizardPage144, wxID_STATIC, wxDefaultPosition, wxSize(100, -1), wxLI_HORIZONTAL );
	itemBoxSizer148->Add(itemStaticLine149, 0, wxGROW|wxALL, 5);

	wxBoxSizer* itemBoxSizer150 = new wxBoxSizer(wxHORIZONTAL);
	itemBoxSizer145->Add(itemBoxSizer150, 0, wxGROW|wxALL, 5);

	m_configrtfid = new wxStaticText( itemWizardPage144, ID_CONFIGRTFID, _("File ID:"), wxDefaultPosition, wxDefaultSize, 0 );
	itemBoxSizer150->Add(m_configrtfid, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

	m_configrtfidtxt = new wxTextCtrl( itemWizardPage144, ID_CONFIGRTFIDTXT, wxEmptyString, wxDefaultPosition, wxSize(100, -1), wxTE_LEFT );
	itemBoxSizer150->Add(m_configrtfidtxt, 100, wxALIGN_CENTER_VERTICAL|wxALL, 5);

	wxBoxSizer* itemBoxSizer153 = new wxBoxSizer(wxHORIZONTAL);
	itemBoxSizer145->Add(itemBoxSizer153, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);

	wxStaticLine* itemStaticLine154 = new wxStaticLine( itemWizardPage144, wxID_STATIC, wxDefaultPosition, wxSize(100, -1), wxLI_HORIZONTAL );
	itemBoxSizer153->Add(itemStaticLine154, 0, wxGROW|wxALL, 5);

	wxBoxSizer* itemBoxSizer155 = new wxBoxSizer(wxHORIZONTAL);
	itemBoxSizer145->Add(itemBoxSizer155, 0, wxGROW|wxALL, 5);

	m_configwv = new wxStaticText( itemWizardPage144, ID_CONFIGWV, _("Word version:"), wxDefaultPosition, wxDefaultSize, 0 );
	itemBoxSizer155->Add(m_configwv, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

	m_configwv97 = new wxRadioButton( itemWizardPage144, ID_CONFIGWV97, _("97"), wxDefaultPosition, wxDefaultSize, wxRB_GROUP );
	m_configwv97->SetValue(false);
	itemBoxSizer155->Add(m_configwv97, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

	m_configwv2k = new wxRadioButton( itemWizardPage144, ID_CONFIGWV2K, _("2000"), wxDefaultPosition, wxDefaultSize, 0 );
	m_configwv2k->SetValue(true);
	itemBoxSizer155->Add(m_configwv2k, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

	m_configwvxp = new wxRadioButton( itemWizardPage144, ID_CONFIGWVXP, _("XP"), wxDefaultPosition, wxDefaultSize, 0 );
	m_configwvxp->SetValue(false);
	itemBoxSizer155->Add(m_configwvxp, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

	m_configwv2003 = new wxRadioButton( itemWizardPage144, ID_CONFIGWV2003, _("2003 and later"), wxDefaultPosition, wxDefaultSize, 0 );
	m_configwv2003->SetValue(false);
	itemBoxSizer155->Add(m_configwv2003, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

	wxBoxSizer* itemBoxSizer161 = new wxBoxSizer(wxHORIZONTAL);
	itemBoxSizer145->Add(itemBoxSizer161, 0, wxGROW|wxALL, 5);

	m_confighpj = new wxStaticText( itemWizardPage144, ID_CONFIGHPJ, _("WinHelp project file:"), wxDefaultPosition, wxDefaultSize, 0 );
	itemBoxSizer161->Add(m_confighpj, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

	m_confighpjtxt = new wxTextCtrl( itemWizardPage144, ID_CONFIGHPJTXT, wxEmptyString, wxDefaultPosition, wxSize(100, -1), wxTE_LEFT );
	itemBoxSizer161->Add(m_confighpjtxt, 100, wxALIGN_CENTER_VERTICAL|wxALL, 5);

	wxBoxSizer* itemBoxSizer164 = new wxBoxSizer(wxHORIZONTAL);
	itemBoxSizer145->Add(itemBoxSizer164, 0, wxGROW|wxALL, 5);

	m_configcnttitle = new wxStaticText( itemWizardPage144, ID_CONFIGCNTTITLE, _("WinHelp file title:"), wxDefaultPosition, wxDefaultSize, 0 );
	itemBoxSizer164->Add(m_configcnttitle, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

	m_configcnttitletxt = new wxTextCtrl( itemWizardPage144, ID_CONFIGCNTTITLETXT, wxEmptyString, wxDefaultPosition, wxSize(100, -1), wxTE_LEFT );
	itemBoxSizer164->Add(m_configcnttitletxt, 100, wxALIGN_CENTER_VERTICAL|wxLEFT|wxRIGHT, 5);

	wxBoxSizer* itemBoxSizer167 = new wxBoxSizer(wxHORIZONTAL);
	itemBoxSizer145->Add(itemBoxSizer167, 0, wxGROW|wxALL, 5);

	m_configwhtopic = new wxStaticText( itemWizardPage144, ID_CONFIGWHTOPIC, _("Starting topic name:"), wxDefaultPosition, wxDefaultSize, 0 );
	itemBoxSizer167->Add(m_configwhtopic, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

	m_configwhtopictxt = new wxTextCtrl( itemWizardPage144, ID_CONFIGWHTOPICTXT, wxEmptyString, wxDefaultPosition, wxSize(100, -1), wxTE_LEFT );
	itemBoxSizer167->Add(m_configwhtopictxt, 100, wxALIGN_CENTER_VERTICAL|wxALL, 5);

////@end WizardPage5 content construction


	if (!configstart->m_configouttype->Cmp("Word RTF")) {
		// hide winhelp elements
		m_confighpj->Hide();
		m_confighpjtxt->Hide();
		m_configcnttitle->Hide();
		m_configcnttitletxt->Hide();
		m_configwhtopic->Hide();
		m_configwhtopictxt->Hide();
	}
	if (!configstart->m_configouttype->Cmp("WinHelp")) {
		// hide word elements
		m_configwv->Hide();
		m_configwv97->Hide();
		m_configwv2k->Hide();
		m_configwvxp->Hide();
		m_configwv2003->Hide();
	}

}


/*
 * Gets the previous page.
 */

wxWizardPage* WizardPage5::GetPrev() const
{
	// return the previous page
	return configstart->m_wizardpage2a;
}


/*
 * Gets the next page.
 */

wxWizardPage* WizardPage5::GetNext() const
{
	configstart->SetFinalText(5);

	// return the next page
	return configstart->m_wizardpageend;
}


/*
 * Should we show tooltips?
 */

bool WizardPage5::ShowToolTips()
{
	return true;
}

/*
 * Get bitmap resources
 */

wxBitmap WizardPage5::GetBitmapResource( const wxString& name )
{
	// Bitmap retrieval
////@begin WizardPage5 bitmap retrieval
	wxUnusedVar(name);
	return wxNullBitmap;
////@end WizardPage5 bitmap retrieval
}

/*
 * Get icon resources
 */

wxIcon WizardPage5::GetIconResource( const wxString& name )
{
	// Icon retrieval
////@begin WizardPage5 icon retrieval
	wxUnusedVar(name);
	return wxNullIcon;
////@end WizardPage5 icon retrieval
}



/*
 * WizardPageEnd type definition
 */

IMPLEMENT_DYNAMIC_CLASS( WizardPageEnd, wxWizardPage )


/*
 * WizardPageEnd event table definition
 */

BEGIN_EVENT_TABLE( WizardPageEnd, wxWizardPage )

////@begin WizardPageEnd event table entries
////@end WizardPageEnd event table entries

END_EVENT_TABLE()


/*
 * WizardPageEnd constructors
 */

WizardPageEnd::WizardPageEnd()
{
	Init();
}

WizardPageEnd::WizardPageEnd( wxWizard* parent )
{
	Init();
	Create( parent );
}


/*
 * WizardPageEnd creator
 */

bool WizardPageEnd::Create( wxWizard* parent )
{
  configstart = (Configstart *) parent;
////@begin WizardPageEnd creation
	wxBitmap wizardBitmap(wxNullBitmap);
	wxWizardPage::Create( parent, wizardBitmap );

	CreateControls();
	if (GetSizer())
		GetSizer()->Fit(this);
////@end WizardPageEnd creation

	return true;
}


/*
 * WizardPageEnd destructor
 */

WizardPageEnd::~WizardPageEnd()
{
////@begin WizardPageEnd destruction
////@end WizardPageEnd destruction
}


/*
 * Member initialisation
 */

void WizardPageEnd::Init()
{
////@begin WizardPageEnd member initialisation
	m_configreview = NULL;
	m_configend = NULL;
	m_configrevtext = NULL;
////@end WizardPageEnd member initialisation
}


/*
 * Control creation for WizardPageEnd
 */

void WizardPageEnd::CreateControls()
{    
////@begin WizardPageEnd content construction
	WizardPageEnd* itemWizardPage170 = this;

	wxBoxSizer* itemBoxSizer171 = new wxBoxSizer(wxVERTICAL);
	itemWizardPage170->SetSizer(itemBoxSizer171);

	wxBoxSizer* itemBoxSizer172 = new wxBoxSizer(wxHORIZONTAL);
	itemBoxSizer171->Add(itemBoxSizer172, 0, wxALIGN_CENTER_HORIZONTAL|wxLEFT|wxRIGHT|wxTOP, 5);

	m_configreview = new wxStaticText( itemWizardPage170, ID_CONFIGREVIEW, _("Review Settings"), wxDefaultPosition, wxDefaultSize, 0 );
	m_configreview->SetFont(wxFont(8, wxSWISS, wxNORMAL, wxBOLD, false, wxT("Tahoma")));
	itemBoxSizer172->Add(m_configreview, 0, wxALIGN_CENTER_VERTICAL|wxLEFT|wxRIGHT|wxTOP, 5);

	wxBoxSizer* itemBoxSizer174 = new wxBoxSizer(wxHORIZONTAL);
	itemBoxSizer171->Add(itemBoxSizer174, 0, wxALIGN_CENTER_HORIZONTAL|wxLEFT|wxRIGHT|wxBOTTOM, 5);

	wxStaticLine* itemStaticLine175 = new wxStaticLine( itemWizardPage170, wxID_STATIC, wxDefaultPosition, wxSize(100, -1), wxLI_HORIZONTAL );
	itemBoxSizer174->Add(itemStaticLine175, 0, wxGROW|wxALL, 5);

	wxBoxSizer* itemBoxSizer176 = new wxBoxSizer(wxHORIZONTAL);
	itemBoxSizer171->Add(itemBoxSizer176, 0, wxGROW|wxLEFT|wxRIGHT|wxTOP, 5);

	m_configend = new wxStaticText( itemWizardPage170, ID_CONFIGEND, _("Review your settings; to change any, click Back, otherwise click Finish."), wxDefaultPosition, wxDefaultSize, 0 );
	itemBoxSizer176->Add(m_configend, 0, wxALIGN_CENTER_VERTICAL|wxLEFT|wxRIGHT|wxTOP, 5);

	wxBoxSizer* itemBoxSizer178 = new wxBoxSizer(wxHORIZONTAL);
	itemBoxSizer171->Add(itemBoxSizer178, 0, wxGROW|wxALL, 5);

	m_configrevtext = new wxTextCtrl( itemWizardPage170, ID_CONFIGREVTEXT, wxEmptyString, wxDefaultPosition, wxSize(250, 240), wxTE_MULTILINE|wxTE_READONLY|wxTE_LEFT|wxHSCROLL );
	itemBoxSizer178->Add(m_configrevtext, 100, wxALIGN_CENTER_VERTICAL|wxALL, 5);

////@end WizardPageEnd content construction

}


void Configstart::SetFinalText(int page)
{
	wxString *form = m_configouttype;
	bool ishtm = (!form->Cmp("HTML 4.0") || !form->Cmp("XHTML 1.0")) ? true : false;

	wxString *str = new wxString();
	char *sect = NULL;
	char *pfile = NULL;

	bool docsetdv = m_wizardpage1a->m_configdvaldoc->GetValue();
	bool docsetan = m_wizardpage2->m_configanamedoc->GetValue();
	bool docsetav = m_wizardpage2->m_configaverdoc->GetValue();
	bool docsetgd = m_wizardpage2a->m_configgrddoc->GetValue();
	bool docsetgf = m_wizardpage2a->m_configgrfdoc->GetValue();

	// doc file items
	str->Append("\nIn ");
	str->Append((m_wizardpage1->m_configinisel->GetPath()).c_str());
	str->Append(":\n");
	str->Append("\n[Automation]\n");
	//AddSetting(str, "WrapAndShip",
	// m_wizardpage1->m_configwasyes->GetValue() ? "Yes" : "No");
	if (docsetan)
		AddSetting(str, "ArchiveName",
		 (m_wizardpage2->m_configanametxt->GetValue()).c_str());
	if (docsetav)
		AddSetting(str, "ArchiveVer",
		 (m_wizardpage2->m_configavertxt->GetValue()).c_str());
	if (docsetgd)
		AddSetting(str, "CopyGraphicsFrom",
		 (m_wizardpage2a->m_configgrsel->GetPath()).c_str());
	if (docsetgf)
		AddSetting(str, "GraphCopyFiles",
		 (m_wizardpage2a->m_configgrtxt->GetValue()).c_str());
	if (docsetdv) {
		if (m_wizardpage1a->m_configdvalfile->GetValue()) {
			str->Append("\n[ConditionOptions]\n");
			AddSetting(str, "DitavalFile",
			 (m_wizardpage1a->m_configdvalsel->GetPath()).c_str());
		}
	}
	str->Append("\n");

	// starting ini file items
	str->Append("\nIn ");
	str->Append(m_configstartini->c_str());
	str->Append(":\n");
	str->Append("\n[Templates]\n");
	AddSetting(str, "Document",
	 (m_wizardpage1->m_configinisel->GetPath()).c_str());

	str->Append("\n[Automation]\n");
	AddSetting(str, "WrapAndShip",
	 m_wizardpage1->m_configwasyes->GetValue() ? "Yes" : "No");
	if (!docsetan)
		AddSetting(str, "ArchiveName",
		 (m_wizardpage2->m_configanametxt->GetValue()).c_str());
	if (!docsetav)
		AddSetting(str, "ArchiveVer",
		 (m_wizardpage2->m_configavertxt->GetValue()).c_str());
	if (!docsetgd)
		AddSetting(str, "CopyGraphicsFrom",
		 (m_wizardpage2a->m_configgrsel->GetPath()).c_str());
	if (!docsetgf)
		AddSetting(str, "GraphCopyFiles",
		 (m_wizardpage2a->m_configgrtxt->GetValue()).c_str());

	if (!docsetdv) {
		if (m_wizardpage1a->m_configdvalfile->GetValue()) {
			str->Append("\n[ConditionOptions]\n");
			AddSetting(str, "DitavalFile",
			 (m_wizardpage1a->m_configdvalsel->GetPath()).c_str());
		}
	}

	if (page == 3) {  // html
		str->Append("\n[HTMLOptions]\n");
		AddSetting(str, "Title",
			(m_wizardpage3->m_configtitletxt->GetValue()).c_str());
		if (!ishtm) {
			if (!form->Cmp("OmniHelp")) {
				sect = "OmniHelpOptions";
				pfile = "ProjectName";
			}
			else if (!form->Cmp("HTML Help")) {
				sect = "MSHtmlHelpOptions";
				pfile = "HHPFileName";
			}
			else if (!form->Cmp("Java Help")) {
				sect = "JavaHelpOptions";
				pfile = "HSFileName";
			}
			else if (!form->Cmp("Oracle Help")) {
				sect = "OracleHelpOptions";
				pfile = "HSFileName";
			}
			str->Append("\n[");
			str->Append(sect);
			str->Append("]\n");
			AddSetting(str, pfile,
				(m_wizardpage3->m_confighpftxt->GetValue()).c_str());
			AddSetting(str, "HelpFileTitle",
				(m_wizardpage3->m_confightitletxt->GetValue()).c_str());
			AddSetting(str, "DefaultTopicFile",
				(m_wizardpage3->m_confightopictxt->GetValue()).c_str());

			if (!form->Cmp("OmniHelp")
			 && !m_wizardpage3->m_configohtltxt->IsEmpty()) {
				str->Append("\n[OHTopLeftNav]\n");
				str->Append((m_wizardpage3->m_configohtltxt->GetValue()).c_str());
				str->Append("\n");
			}
		}
	}
	else if (page == 4) { // eclipse
		str->Append("\n[EclipseHelpOptions]\n");
		AddSetting(str, "TocLabel",
			(m_wizardpage4->m_configehtitletxt->GetValue()).c_str());
		AddSetting(str, "PluginName",
			(m_wizardpage4->m_configehpntxt->GetValue()).c_str());
		AddSetting(str, "PluginID",
			(m_wizardpage4->m_configehpidtxt->GetValue()).c_str());
		AddSetting(str, "PluginProvider",
			(m_wizardpage4->m_configehprovtxt->GetValue()).c_str());
		AddSetting(str, "TocTopic",
			(m_wizardpage4->m_configehtopictxt->GetValue()).c_str());
	}
	else if (page == 5) {  // rtf
		str->Append("\n[FileIDs]\n");
		AddSetting(str, m_configname->c_str(),
			(m_wizardpage5->m_configrtfidtxt->GetValue()).c_str());
		if (!form->Cmp("Word RTF")) {
			str->Append("\n[WordOptions]\n");
			if (m_wizardpage5->m_configwv97->GetValue())
				AddSetting(str, "Word8", "Yes");
			else if (m_wizardpage5->m_configwv2k->GetValue())
				AddSetting(str, "Word2000", "Yes");
			else if (m_wizardpage5->m_configwvxp->GetValue())
				AddSetting(str, "Word2002", "Yes");
			else if (m_wizardpage5->m_configwv2003->GetValue())
				AddSetting(str, "Word2003", "Yes");
		}
		else {
			str->Append("\n[HelpOptions]\n");
			AddSetting(str, "HPJFileName",
				(m_wizardpage5->m_confighpjtxt->GetValue()).c_str());
			str->Append("\n[HelpContents]\n");
			AddSetting(str, "CntTitle",
				(m_wizardpage5->m_configcnttitletxt->GetValue()).c_str());
			AddSetting(str, "CntTopic",
				(m_wizardpage5->m_configwhtopictxt->GetValue()).c_str());
		}
	}

	m_wizardpageend->m_configrevtext->Clear();
	m_wizardpageend->m_configrevtext->SetValue(*str);
}


void Configstart::AddSetting(wxString *str, const char *key, const char *val)
{
	str->Append(key);
	str->Append("=");
	str->Append(val);
	str->Append("\n");
}


/*
 * Gets the previous page.
 */

wxWizardPage* WizardPageEnd::GetPrev() const
{
	// return the previous page
	if (!configstart->m_configouttype->Cmp("Eclipse Help"))
		return configstart->m_wizardpage4;

	if (!configstart->m_configouttype->Cmp("Word RTF")
	 || !configstart->m_configouttype->Cmp("WinHelp"))
		return configstart->m_wizardpage5;

	if (!configstart->m_configouttype->Cmp("Flat XML")
	 || !configstart->m_configouttype->Cmp("DITA")
	 || !configstart->m_configouttype->Cmp("DocBook"))
		return configstart->m_wizardpage2a;

	return configstart->m_wizardpage3;
}


/*
 * Gets the next page.
 */

wxWizardPage* WizardPageEnd::GetNext() const
{
	// return the next page
	return NULL;
}


/*
 * Should we show tooltips?
 */

bool WizardPageEnd::ShowToolTips()
{
	return true;
}

/*
 * Get bitmap resources
 */

wxBitmap WizardPageEnd::GetBitmapResource( const wxString& name )
{
	// Bitmap retrieval
////@begin WizardPageEnd bitmap retrieval
	wxUnusedVar(name);
	return wxNullBitmap;
////@end WizardPageEnd bitmap retrieval
}

/*
 * Get icon resources
 */

wxIcon WizardPageEnd::GetIconResource( const wxString& name )
{
	// Icon retrieval
////@begin WizardPageEnd icon retrieval
	wxUnusedVar(name);
	return wxNullIcon;
////@end WizardPageEnd icon retrieval
}


// end of configstart.cpp

