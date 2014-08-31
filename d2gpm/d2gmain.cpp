/////////////////////////////////////////////////////////////////////////////
// Name:        d2gmain.cpp
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
#include "wx/bookctrl.h"
////@end includes

#include "d2gmain.h"

////@begin XPM images
////@end XPM images

#pragma warning( disable : 4189 )

/*
 * D2gmain type definition
 */

IMPLEMENT_DYNAMIC_CLASS( D2gmain, wxPropertySheetDialog )


/*
 * D2gmain event table definition
 */

BEGIN_EVENT_TABLE( D2gmain, wxPropertySheetDialog )

////@begin D2gmain event table entries
	EVT_INIT_DIALOG( D2gmain::OnInitDialog )

	EVT_CHOICE( ID_RUNPROJECTSEL, D2gmain::OnRunprojectselSelected )
	EVT_UPDATE_UI( ID_RUNPROJECTSEL, D2gmain::OnRunprojectselUpdate )

	EVT_FILEPICKER_CHANGED( ID_RUNSRCMAPSEL, D2gmain::OnRunsrcmapselFilePickerChanged )

	EVT_BUTTON( ID_RUNSTART, D2gmain::OnRunstartClick )

	EVT_BUTTON( ID_RUNSTOP, D2gmain::OnRunstopClick )

	EVT_BUTTON( ID_RUNCM, D2gmain::OnRuncmClick )

	EVT_BUTTON( ID_RUNEDITINI, D2gmain::OnRuneditiniClick )

	EVT_BUTTON( ID_RUNSHOWLOG, D2gmain::OnRunshowlogClick )

	EVT_BUTTON( ID_RUNVIEWOUT, D2gmain::OnRunviewoutClick )

	EVT_RADIOBUTTON( ID_RUNWASYES, D2gmain::OnRunwasyesSelected )

	EVT_RADIOBUTTON( ID_RUNWASNO, D2gmain::OnRunwasnoSelected )

	EVT_RADIOBUTTON( ID_RUNCOMPYES, D2gmain::OnRuncompyesSelected )

	EVT_RADIOBUTTON( ID_RUNCOMPNO, D2gmain::OnRuncompnoSelected )

	EVT_RADIOBUTTON( ID_RUNCWINYES, D2gmain::OnRuncwinyesSelected )

	EVT_RADIOBUTTON( ID_RUNCWINNO, D2gmain::OnRuncwinnoSelected )

	EVT_RADIOBUTTON( ID_RUNFTSYES, D2gmain::OnRunftsyesSelected )

	EVT_RADIOBUTTON( ID_RUNFTSNO, D2gmain::OnRunftsnoSelected )

	EVT_RADIOBUTTON( ID_RUNJARYES, D2gmain::OnRunjaryesSelected )

	EVT_RADIOBUTTON( ID_RUNJARNO, D2gmain::OnRunjarnoSelected )

	EVT_RADIOBUTTON( ID_RUNARCHYES, D2gmain::OnRunarchyesSelected )

	EVT_RADIOBUTTON( ID_RUNARCHNO, D2gmain::OnRunarchnoSelected )

	EVT_RADIOBUTTON( ID_RUNSHIPYES, D2gmain::OnRunshipyesSelected )

	EVT_RADIOBUTTON( ID_RUNSHIPNO, D2gmain::OnRunshipnoSelected )

	EVT_RADIOBUTTON( ID_RUNDCLSRC, D2gmain::OnRundclsrcSelected )

	EVT_RADIOBUTTON( ID_RUNDCLOUT, D2gmain::OnRundcloutSelected )

	EVT_RADIOBUTTON( ID_RUNDCLNO, D2gmain::OnRundclnoSelected )

	EVT_RADIOBUTTON( ID_RUNDCLYES, D2gmain::OnRundclyesSelected )

	EVT_RADIOBUTTON( ID_RUNONLYYES, D2gmain::OnRunonlyyesSelected )

	EVT_RADIOBUTTON( ID_RUNONLYNO, D2gmain::OnRunonlynoSelected )

	EVT_COMBOBOX( ID_PROJECTSEL, D2gmain::OnProjectselSelected )
	EVT_TEXT_ENTER( ID_PROJECTSEL, D2gmain::OnProjectselEnter )
	EVT_UPDATE_UI( ID_PROJECTSEL, D2gmain::OnProjectselUpdate )

	EVT_DIRPICKER_CHANGED( ID_SRCDIRSELECT, D2gmain::OnSrcdirselectDirPickerChanged )

	EVT_FILEPICKER_CHANGED( ID_SRCMAPDEFSEL, D2gmain::OnSrcmapdefselFilePickerChanged )

	EVT_CHOICE( ID_FORMATSEL, D2gmain::OnFormatselSelected )
	EVT_UPDATE_UI( ID_FORMATSEL, D2gmain::OnFormatselUpdate )

	EVT_DIRPICKER_CHANGED( ID_OUTDIRSELECT, D2gmain::OnOutdirselectDirPickerChanged )

	EVT_BUTTON( ID_CREATEPROJECT, D2gmain::OnCreateprojectClick )

	EVT_BUTTON( ID_INICONFIG, D2gmain::OnIniconfigClick )

	EVT_BUTTON( ID_CREATECM, D2gmain::OnCreatecmClick )

	EVT_BUTTON( ID_EDITINI, D2gmain::OnEditiniClick )

	EVT_CHOICE( ID_MPROJECTSEL, D2gmain::OnMprojectselSelected )
	EVT_UPDATE_UI( ID_MPROJECTSEL, D2gmain::OnMprojectselUpdate )

	EVT_DIRPICKER_CHANGED( ID_MSRCDIRSELECT, D2gmain::OnMsrcdirselectDirPickerChanged )

	EVT_FILEPICKER_CHANGED( ID_MSRCMAPDEFSEL, D2gmain::OnMsrcmapdefselFilePickerChanged )

	EVT_CHOICE( ID_MFORMATSEL, D2gmain::OnMformatselSelected )
	EVT_UPDATE_UI( ID_MFORMATSEL, D2gmain::OnMformatselUpdate )

	EVT_DIRPICKER_CHANGED( ID_MOUTDIRSELECT, D2gmain::OnMoutdirselectDirPickerChanged )

	EVT_BUTTON( ID_MODIFYPROJECT, D2gmain::OnModifyprojectClick )

	EVT_BUTTON( ID_MINICONFIG, D2gmain::OnMiniconfigClick )

	EVT_BUTTON( ID_MODIFYCM, D2gmain::OnModifycmClick )

	EVT_BUTTON( ID_MEDITINI, D2gmain::OnMeditiniClick )

	EVT_CHOICE( ID_DPROJECTSEL, D2gmain::OnDprojectselSelected )
	EVT_UPDATE_UI( ID_DPROJECTSEL, D2gmain::OnDprojectselUpdate )

	EVT_BUTTON( ID_DELETEPROJECT, D2gmain::OnDeleteprojectClick )

	EVT_CHOICE( ID_RPROJECTSEL, D2gmain::OnRprojectselSelected )
	EVT_UPDATE_UI( ID_RPROJECTSEL, D2gmain::OnRprojectselUpdate )

	EVT_BUTTON( ID_RESTOREPROJECT, D2gmain::OnRestoreprojectClick )

	EVT_TEXT( ID_OPTEDITORTEXT, D2gmain::OnOpteditortextTextUpdated )
	EVT_TEXT_ENTER( ID_OPTEDITORTEXT, D2gmain::OnOpteditortextEnter )

	EVT_RADIOBUTTON( ID_OPTSHDELYES, D2gmain::OnOptshdelyesSelected )

	EVT_RADIOBUTTON( ID_OPTSHDELNO, D2gmain::OnOptshdelnoSelected )

	EVT_CHECKBOX( ID_OPTSHOWCON, D2gmain::OnOptshowconClick )

	EVT_CHECKBOX( ID_OPTSHOWLOG, D2gmain::OnOptshowlogClick )

	EVT_TEXT( ID_OPTLOGFILETEXT, D2gmain::OnOptlogfiletextTextUpdated )
	EVT_TEXT_ENTER( ID_OPTLOGFILETEXT, D2gmain::OnOptlogfiletextEnter )

	EVT_DIRPICKER_CHANGED( ID_OPTNEWINISEL, D2gmain::OnOptnewiniselDirPickerChanged )

	EVT_RADIOBUTTON( ID_OPTPROJINID2GBUT, D2gmain::OnOptprojinid2gbutSelected )

	EVT_RADIOBUTTON( ID_OPTPROJINILOCBUT, D2gmain::OnOptprojinilocbutSelected )

	EVT_DIRPICKER_CHANGED( ID_OPTPROJINID2GSEL, D2gmain::OnOptprojinid2gselDirPickerChanged )

	EVT_TEXT( ID_OPTPROJINILOCTEXT, D2gmain::OnOptprojiniloctextTextUpdated )

	EVT_BUTTON( ID_OPTSET, D2gmain::OnOptsetClick )

	EVT_BUTTON( ID_OPTRESET, D2gmain::OnOptresetClick )

////@end D2gmain event table entries

END_EVENT_TABLE()


/*
 * D2gmain constructors
 */

D2gmain::D2gmain()
{
	Init();
}

D2gmain::D2gmain( wxWindow* parent, wxWindowID id, const wxString& caption, const wxPoint& pos, const wxSize& size, long style )
{
	Init();

	wxUnusedVar(pos);
	wxUnusedVar(size);
	Create(parent, id, caption, *InitPos, *InitSize, style);
}


/*
 * D2gmain creator
 */

bool D2gmain::Create( wxWindow* parent, wxWindowID id, const wxString& caption, const wxPoint& pos, const wxSize& size, long style )
{
	SetExtraStyle(wxWS_EX_VALIDATE_RECURSIVELY|wxWS_EX_BLOCK_EVENTS);
	SetSheetStyle(wxPROPSHEET_DEFAULT);
	wxPropertySheetDialog::Create( parent, id, caption, pos, size, style );

	CreateControls();
	SetIcon(GetIconResource(wxT("favicon.xpm")));
	//LayoutDialog();  // removed to prevent centering and shrinkwrapping
	return true;
}


/*
 * D2gmain destructor
 */

D2gmain::~D2gmain()
{
  CloseProjectsFile();

////@begin D2gmain destruction
////@end D2gmain destruction
}


/*
 * Member initialisation
 */

void D2gmain::Init()
{
////@begin D2gmain member initialisation
	m_runpanel = NULL;
	m_runproject = NULL;
	m_runprojectsel = NULL;
	m_runsrcdir = NULL;
	m_runsrcdirtext = NULL;
	m_runsrcmap = NULL;
	m_runsrcmapsel = NULL;
	m_runformat = NULL;
	m_runformattext = NULL;
	m_runoutputdir = NULL;
	m_runoutdirtext = NULL;
	m_runstart = NULL;
	m_runstop = NULL;
	m_runcm = NULL;
	m_runeditini = NULL;
	m_runshowlog = NULL;
	m_runviewout = NULL;
	m_runresult = NULL;
	m_runresulttext = NULL;
	m_runoptpanel = NULL;
	m_runoptauto = NULL;
	m_runwas = NULL;
	m_runwasyes = NULL;
	m_runwasno = NULL;
	m_runcomp = NULL;
	m_runcompyes = NULL;
	m_runcompno = NULL;
	m_runcwin = NULL;
	m_runcwinyes = NULL;
	m_runcwinno = NULL;
	m_runfts = NULL;
	m_runftsyes = NULL;
	m_runftsno = NULL;
	m_runjar = NULL;
	m_runjaryes = NULL;
	m_runjarno = NULL;
	m_runarch = NULL;
	m_runarchyes = NULL;
	m_runarchno = NULL;
	m_runship = NULL;
	m_runshipyes = NULL;
	m_runshipno = NULL;
	m_runoptadv = NULL;
	m_rundcl = NULL;
	m_rundclsrc = NULL;
	m_rundclout = NULL;
	m_rundclno = NULL;
	m_rundclyes = NULL;
	m_runonly = NULL;
	m_runonlyyes = NULL;
	m_runonlyno = NULL;
	m_createpanel = NULL;
	m_projectname = NULL;
	m_projectselect = NULL;
	m_sourcedir = NULL;
	m_srcdirselect = NULL;
	m_sourcemap = NULL;
	m_srcmapselect = NULL;
	m_formatname = NULL;
	m_formatselect = NULL;
	m_outputdir = NULL;
	m_outdirselect = NULL;
	m_createproject = NULL;
	m_iniconfig = NULL;
	m_createcm = NULL;
	m_editini = NULL;
	m_result = NULL;
	m_resulttext = NULL;
	m_modifypanel = NULL;
	m_mprojectname = NULL;
	m_mprojectselect = NULL;
	m_msourcedir = NULL;
	m_msrcdirselect = NULL;
	m_msourcemap = NULL;
	m_msrcmapselect = NULL;
	m_mformatname = NULL;
	m_mformatselect = NULL;
	m_moutputdir = NULL;
	m_moutdirselect = NULL;
	m_modifyproject = NULL;
	m_miniconfig = NULL;
	m_modifycm = NULL;
	m_meditini = NULL;
	m_mresult = NULL;
	m_mresulttext = NULL;
	m_deletepanel = NULL;
	m_dprojectname = NULL;
	m_dprojectselect = NULL;
	m_dsourcedir = NULL;
	m_dsrcdirtext = NULL;
	m_dsourcemap = NULL;
	m_dsrcmaptext = NULL;
	m_dformatname = NULL;
	m_dformattext = NULL;
	m_doutputdir = NULL;
	m_doutdirtext = NULL;
	m_deleteproject = NULL;
	m_dresult = NULL;
	m_dresulttext = NULL;
	m_restorepanel = NULL;
	m_rprojectname = NULL;
	m_rprojectselect = NULL;
	m_rsourcedir = NULL;
	m_rsrcdirtext = NULL;
	m_rsourcemap = NULL;
	m_rsrcmaptext = NULL;
	m_rformatname = NULL;
	m_rformattext = NULL;
	m_routputdir = NULL;
	m_routdirtext = NULL;
	m_restoreproject = NULL;
	m_rresult = NULL;
	m_rresulttext = NULL;
	m_optionspanel = NULL;
	m_opteditor = NULL;
	m_opteditortext = NULL;
	m_optshdel = NULL;
	m_optshdelyes = NULL;
	m_optshdelno = NULL;
	m_optshow = NULL;
	m_optshowcon = NULL;
	m_optshowlog = NULL;
	m_optlogfile = NULL;
	m_optlogfiletext = NULL;
	m_optnewini = NULL;
	m_optnewinisel = NULL;
	m_optprojini = NULL;
	m_optprojinid2gbut = NULL;
	m_optprojinilocbut = NULL;
	m_optprojinid2g = NULL;
	m_optprojinid2gsel = NULL;
	m_optprojiniloc = NULL;
	m_optprojiniloctext = NULL;
	m_optset = NULL;
	m_optreset = NULL;
////@end D2gmain member initialisation

	// init for d2gdcl variables
	InitializeDCL();
}


/*
 * Control creation for D2gmain
 */

void D2gmain::CreateControls()
{    
////@begin D2gmain content construction
	D2gmain* itemPropertySheetDialog1 = this;

	m_runpanel = new wxPanel( GetBookCtrl(), ID_RUNPANEL, wxDefaultPosition, wxDefaultSize, wxSUNKEN_BORDER|wxTAB_TRAVERSAL );
	wxBoxSizer* itemBoxSizer3 = new wxBoxSizer(wxVERTICAL);
	m_runpanel->SetSizer(itemBoxSizer3);

	wxBoxSizer* itemBoxSizer4 = new wxBoxSizer(wxHORIZONTAL);
	itemBoxSizer3->Add(itemBoxSizer4, 100, wxGROW|wxALL, 5);
	m_runproject = new wxStaticText( m_runpanel, ID_RUNPROJECT, _("Run project:  "), wxDefaultPosition, wxDefaultSize, wxALIGN_LEFT );
	itemBoxSizer4->Add(m_runproject, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

	wxArrayString m_runprojectselStrings;
	m_runprojectsel = new wxChoice( m_runpanel, ID_RUNPROJECTSEL, wxDefaultPosition, wxSize(200, -1), m_runprojectselStrings, 0 );
	itemBoxSizer4->Add(m_runprojectsel, 100, wxALIGN_CENTER_VERTICAL|wxALL, 5);

	wxBoxSizer* itemBoxSizer7 = new wxBoxSizer(wxHORIZONTAL);
	itemBoxSizer3->Add(itemBoxSizer7, 100, wxGROW|wxALL, 5);
	m_runsrcdir = new wxStaticText( m_runpanel, ID_RUNSRCDIR, _("Source directory:"), wxDefaultPosition, wxDefaultSize, 0 );
	itemBoxSizer7->Add(m_runsrcdir, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

	m_runsrcdirtext = new wxTextCtrl( m_runpanel, ID_RUNSRCDIRTEXT, wxEmptyString, wxDefaultPosition, wxSize(200, -1), wxTE_READONLY|wxTE_NOHIDESEL|wxTE_LEFT );
	m_runsrcdirtext->SetBackgroundColour(wxColour(192, 192, 192));
	itemBoxSizer7->Add(m_runsrcdirtext, 100, wxALIGN_CENTER_VERTICAL|wxALL, 5);

	wxBoxSizer* itemBoxSizer10 = new wxBoxSizer(wxHORIZONTAL);
	itemBoxSizer3->Add(itemBoxSizer10, 100, wxGROW|wxALL, 5);
	m_runsrcmap = new wxStaticText( m_runpanel, ID_RUNSRCMAP, _("Source map file:"), wxDefaultPosition, wxDefaultSize, 0 );
	itemBoxSizer10->Add(m_runsrcmap, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

	m_runsrcmapsel = new wxFilePickerCtrl( m_runpanel, ID_RUNSRCMAPSEL, wxEmptyString, _("Select a ditamap or bookmap to use for the current project:"), wxEmptyString, wxDefaultPosition, wxSize(200, -1), wxFLP_USE_TEXTCTRL|wxFLP_OPEN|wxFLP_FILE_MUST_EXIST );
	itemBoxSizer10->Add(m_runsrcmapsel, 100, wxALIGN_CENTER_VERTICAL|wxALL, 5);

	wxBoxSizer* itemBoxSizer13 = new wxBoxSizer(wxHORIZONTAL);
	itemBoxSizer3->Add(itemBoxSizer13, 100, wxGROW|wxALL, 5);
	m_runformat = new wxStaticText( m_runpanel, ID_RUNFORMAT, _("Output type:"), wxDefaultPosition, wxDefaultSize, wxALIGN_LEFT );
	itemBoxSizer13->Add(m_runformat, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

	m_runformattext = new wxTextCtrl( m_runpanel, ID_RUNFORMATTEXT, wxEmptyString, wxDefaultPosition, wxSize(200, -1), wxTE_READONLY|wxTE_NOHIDESEL|wxTE_LEFT );
	m_runformattext->SetMaxLength(200);
	m_runformattext->SetBackgroundColour(wxColour(192, 192, 192));
	itemBoxSizer13->Add(m_runformattext, 100, wxALIGN_CENTER_VERTICAL|wxALL, 5);

	wxBoxSizer* itemBoxSizer16 = new wxBoxSizer(wxHORIZONTAL);
	itemBoxSizer3->Add(itemBoxSizer16, 100, wxGROW|wxALL, 5);
	m_runoutputdir = new wxStaticText( m_runpanel, ID_RUNOUTDIR, _("Output directory:"), wxDefaultPosition, wxDefaultSize, wxALIGN_LEFT );
	itemBoxSizer16->Add(m_runoutputdir, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

	m_runoutdirtext = new wxTextCtrl( m_runpanel, ID_RUNOUTDIRTEXT, wxEmptyString, wxDefaultPosition, wxSize(200, -1), wxTE_READONLY|wxTE_NOHIDESEL|wxTE_LEFT );
	m_runoutdirtext->SetBackgroundColour(wxColour(192, 192, 192));
	itemBoxSizer16->Add(m_runoutdirtext, 100, wxALIGN_CENTER_VERTICAL|wxALL, 5);

	wxBoxSizer* itemBoxSizer19 = new wxBoxSizer(wxHORIZONTAL);
	itemBoxSizer3->Add(itemBoxSizer19, 100, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);
	m_runstart = new wxButton( m_runpanel, ID_RUNSTART, _("Start"), wxDefaultPosition, wxDefaultSize, 0 );
	m_runstart->SetDefault();
	itemBoxSizer19->Add(m_runstart, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

	m_runstop = new wxButton( m_runpanel, ID_RUNSTOP, _("Stop"), wxDefaultPosition, wxDefaultSize, 0 );
	m_runstop->Enable(false);
	itemBoxSizer19->Add(m_runstop, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

	m_runcm = new wxButton( m_runpanel, ID_RUNCM, _("Configure"), wxDefaultPosition, wxDefaultSize, 0 );
	itemBoxSizer19->Add(m_runcm, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

	m_runeditini = new wxButton( m_runpanel, ID_RUNEDITINI, _("Edit Ini File"), wxDefaultPosition, wxDefaultSize, 0 );
	itemBoxSizer19->Add(m_runeditini, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

	m_runshowlog = new wxButton( m_runpanel, ID_RUNSHOWLOG, _("Show Log File"), wxDefaultPosition, wxDefaultSize, 0 );
	itemBoxSizer19->Add(m_runshowlog, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

	m_runviewout = new wxButton( m_runpanel, ID_RUNVIEWOUT, _("View Output"), wxDefaultPosition, wxDefaultSize, 0 );
	m_runviewout->Enable(false);
	itemBoxSizer19->Add(m_runviewout, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

	wxBoxSizer* itemBoxSizer26 = new wxBoxSizer(wxHORIZONTAL);
	itemBoxSizer3->Add(itemBoxSizer26, 100, wxGROW|wxLEFT|wxRIGHT|wxBOTTOM, 5);
	m_runresult = new wxStaticText( m_runpanel, ID_RUNRESULT, _("Results:"), wxDefaultPosition, wxDefaultSize, wxALIGN_LEFT );
	itemBoxSizer26->Add(m_runresult, 0, wxALIGN_CENTER_VERTICAL|wxLEFT|wxRIGHT|wxBOTTOM, 5);

	m_runresulttext = new wxTextCtrl( m_runpanel, ID_RUNRESULTTEXT, wxEmptyString, wxDefaultPosition, wxSize(230, -1), wxTE_READONLY );
	itemBoxSizer26->Add(m_runresulttext, 100, wxALIGN_CENTER_VERTICAL|wxLEFT|wxRIGHT|wxBOTTOM, 5);

	GetBookCtrl()->AddPage(m_runpanel, _("Run Project"));

	m_runoptpanel = new wxPanel( GetBookCtrl(), ID_RUNOPTPANEL, wxDefaultPosition, wxDefaultSize, wxSUNKEN_BORDER|wxTAB_TRAVERSAL );
	wxBoxSizer* itemBoxSizer30 = new wxBoxSizer(wxVERTICAL);
	m_runoptpanel->SetSizer(itemBoxSizer30);

	wxBoxSizer* itemBoxSizer31 = new wxBoxSizer(wxHORIZONTAL);
	itemBoxSizer30->Add(itemBoxSizer31, 0, wxGROW|wxLEFT|wxRIGHT|wxTOP, 3);
	m_runoptauto = new wxStaticText( m_runoptpanel, ID_RUNOPTAUTO, _("Automation Options"), wxDefaultPosition, wxDefaultSize, 0 );
	m_runoptauto->SetFont(wxFont(9, wxSWISS, wxNORMAL, wxBOLD, false, wxT("Tahoma")));
	itemBoxSizer31->Add(m_runoptauto, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

	wxBoxSizer* itemBoxSizer33 = new wxBoxSizer(wxHORIZONTAL);
	itemBoxSizer30->Add(itemBoxSizer33, 0, wxGROW|wxALL, 2);
	m_runwas = new wxStaticText( m_runoptpanel, ID_RUNWAS, _("Wrap and ship:"), wxDefaultPosition, wxDefaultSize, 0 );
	itemBoxSizer33->Add(m_runwas, 50, wxALIGN_CENTER_VERTICAL|wxALL, 5);

	m_runwasyes = new wxRadioButton( m_runoptpanel, ID_RUNWASYES, _("Yes"), wxDefaultPosition, wxDefaultSize, wxRB_GROUP );
	m_runwasyes->SetValue(true);
	itemBoxSizer33->Add(m_runwasyes, 25, wxALIGN_CENTER_VERTICAL|wxALL, 5);

	m_runwasno = new wxRadioButton( m_runoptpanel, ID_RUNWASNO, _("No"), wxDefaultPosition, wxDefaultSize, 0 );
	m_runwasno->SetValue(false);
	itemBoxSizer33->Add(m_runwasno, 25, wxALIGN_CENTER_VERTICAL|wxALL, 5);

	wxBoxSizer* itemBoxSizer37 = new wxBoxSizer(wxHORIZONTAL);
	itemBoxSizer30->Add(itemBoxSizer37, 0, wxGROW|wxLEFT|wxRIGHT|wxTOP, 2);
	m_runcomp = new wxStaticText( m_runoptpanel, ID_RUNCOMP, _("Compile Help (HTML Help, WinHelp):"), wxDefaultPosition, wxDefaultSize, 0 );
	itemBoxSizer37->Add(m_runcomp, 50, wxALIGN_CENTER_VERTICAL|wxALL, 5);

	m_runcompyes = new wxRadioButton( m_runoptpanel, ID_RUNCOMPYES, _("Yes"), wxDefaultPosition, wxDefaultSize, wxRB_GROUP );
	m_runcompyes->SetValue(true);
	itemBoxSizer37->Add(m_runcompyes, 25, wxALIGN_CENTER_VERTICAL|wxALL, 5);

	m_runcompno = new wxRadioButton( m_runoptpanel, ID_RUNCOMPNO, _("No"), wxDefaultPosition, wxDefaultSize, 0 );
	m_runcompno->SetValue(false);
	itemBoxSizer37->Add(m_runcompno, 25, wxALIGN_CENTER_VERTICAL|wxALL, 5);

	wxBoxSizer* itemBoxSizer41 = new wxBoxSizer(wxHORIZONTAL);
	itemBoxSizer30->Add(itemBoxSizer41, 0, wxGROW|wxLEFT|wxRIGHT|wxBOTTOM, 2);
	itemBoxSizer41->Add(5, 5, 5, wxALIGN_CENTER_VERTICAL|wxALL, 5);

	m_runcwin = new wxStaticText( m_runoptpanel, ID_RUNCWIN, _("Keep compile window:"), wxDefaultPosition, wxDefaultSize, 0 );
	itemBoxSizer41->Add(m_runcwin, 45, wxALIGN_CENTER_VERTICAL|wxALL, 5);

	m_runcwinyes = new wxRadioButton( m_runoptpanel, ID_RUNCWINYES, _("Yes"), wxDefaultPosition, wxDefaultSize, wxRB_GROUP );
	m_runcwinyes->SetValue(true);
	itemBoxSizer41->Add(m_runcwinyes, 25, wxALIGN_CENTER_VERTICAL|wxALL, 5);

	m_runcwinno = new wxRadioButton( m_runoptpanel, ID_RUNCWINNO, _("No"), wxDefaultPosition, wxDefaultSize, 0 );
	m_runcwinno->SetValue(false);
	itemBoxSizer41->Add(m_runcwinno, 25, wxALIGN_CENTER_VERTICAL|wxALL, 5);

	wxBoxSizer* itemBoxSizer46 = new wxBoxSizer(wxHORIZONTAL);
	itemBoxSizer30->Add(itemBoxSizer46, 0, wxGROW|wxALL, 2);
	m_runfts = new wxStaticText( m_runoptpanel, ID_RUNFTS, _("Generate search index (Java Help, Oracle Help):"), wxDefaultPosition, wxDefaultSize, 0 );
	itemBoxSizer46->Add(m_runfts, 50, wxALIGN_CENTER_VERTICAL|wxALL, 5);

	m_runftsyes = new wxRadioButton( m_runoptpanel, ID_RUNFTSYES, _("Yes"), wxDefaultPosition, wxDefaultSize, wxRB_GROUP );
	m_runftsyes->SetValue(true);
	itemBoxSizer46->Add(m_runftsyes, 25, wxALIGN_CENTER_VERTICAL|wxALL, 5);

	m_runftsno = new wxRadioButton( m_runoptpanel, ID_RUNFTSNO, _("No"), wxDefaultPosition, wxDefaultSize, 0 );
	m_runftsno->SetValue(false);
	itemBoxSizer46->Add(m_runftsno, 25, wxALIGN_CENTER_VERTICAL|wxALL, 5);

	wxBoxSizer* itemBoxSizer50 = new wxBoxSizer(wxHORIZONTAL);
	itemBoxSizer30->Add(itemBoxSizer50, 0, wxGROW|wxALL, 2);
	m_runjar = new wxStaticText( m_runoptpanel, ID_RUNJAR, _("Make .jar file (Java Help):"), wxDefaultPosition, wxDefaultSize, 0 );
	itemBoxSizer50->Add(m_runjar, 50, wxALIGN_CENTER_VERTICAL|wxALL, 5);

	m_runjaryes = new wxRadioButton( m_runoptpanel, ID_RUNJARYES, _("Yes"), wxDefaultPosition, wxDefaultSize, wxRB_GROUP );
	m_runjaryes->SetValue(true);
	itemBoxSizer50->Add(m_runjaryes, 25, wxALIGN_CENTER_VERTICAL|wxALL, 5);

	m_runjarno = new wxRadioButton( m_runoptpanel, ID_RUNJARNO, _("No"), wxDefaultPosition, wxDefaultSize, 0 );
	m_runjarno->SetValue(false);
	itemBoxSizer50->Add(m_runjarno, 25, wxALIGN_CENTER_VERTICAL|wxALL, 5);

	wxBoxSizer* itemBoxSizer54 = new wxBoxSizer(wxHORIZONTAL);
	itemBoxSizer30->Add(itemBoxSizer54, 0, wxGROW|wxLEFT|wxRIGHT|wxTOP, 2);
	m_runarch = new wxStaticText( m_runoptpanel, ID_RUNARCH, _("Archive output in a .zip file:"), wxDefaultPosition, wxDefaultSize, 0 );
	itemBoxSizer54->Add(m_runarch, 50, wxALIGN_CENTER_VERTICAL|wxALL, 5);

	m_runarchyes = new wxRadioButton( m_runoptpanel, ID_RUNARCHYES, _("Yes"), wxDefaultPosition, wxDefaultSize, wxRB_GROUP );
	m_runarchyes->SetValue(true);
	itemBoxSizer54->Add(m_runarchyes, 25, wxALIGN_CENTER_VERTICAL|wxALL, 5);

	m_runarchno = new wxRadioButton( m_runoptpanel, ID_RUNARCHNO, _("No"), wxDefaultPosition, wxDefaultSize, 0 );
	m_runarchno->SetValue(false);
	itemBoxSizer54->Add(m_runarchno, 25, wxALIGN_CENTER_VERTICAL|wxALL, 5);

	wxBoxSizer* itemBoxSizer58 = new wxBoxSizer(wxHORIZONTAL);
	itemBoxSizer30->Add(itemBoxSizer58, 0, wxGROW|wxLEFT|wxRIGHT|wxBOTTOM, 2);
	itemBoxSizer58->Add(5, 5, 5, wxALIGN_CENTER_VERTICAL|wxALL, 5);

	m_runship = new wxStaticText( m_runoptpanel, ID_RUNSHIP, _("Move archive to shipping directory:"), wxDefaultPosition, wxDefaultSize, 0 );
	itemBoxSizer58->Add(m_runship, 45, wxALIGN_CENTER_VERTICAL|wxALL, 5);

	m_runshipyes = new wxRadioButton( m_runoptpanel, ID_RUNSHIPYES, _("Yes"), wxDefaultPosition, wxDefaultSize, wxRB_GROUP );
	m_runshipyes->SetValue(true);
	itemBoxSizer58->Add(m_runshipyes, 25, wxALIGN_CENTER_VERTICAL|wxALL, 5);

	m_runshipno = new wxRadioButton( m_runoptpanel, ID_RUNSHIPNO, _("No"), wxDefaultPosition, wxDefaultSize, 0 );
	m_runshipno->SetValue(false);
	itemBoxSizer58->Add(m_runshipno, 25, wxALIGN_CENTER_VERTICAL|wxALL, 5);

	wxBoxSizer* itemBoxSizer63 = new wxBoxSizer(wxHORIZONTAL);
	itemBoxSizer30->Add(itemBoxSizer63, 0, wxGROW|wxLEFT|wxRIGHT|wxTOP, 3);
	m_runoptadv = new wxStaticText( m_runoptpanel, ID_RUNOPTADV, _("Advanced Options"), wxDefaultPosition, wxDefaultSize, 0 );
	m_runoptadv->SetFont(wxFont(9, wxSWISS, wxNORMAL, wxBOLD, false, wxT("Tahoma")));
	itemBoxSizer63->Add(m_runoptadv, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

	wxBoxSizer* itemBoxSizer65 = new wxBoxSizer(wxHORIZONTAL);
	itemBoxSizer30->Add(itemBoxSizer65, 0, wxGROW|wxALL, 2);
	m_rundcl = new wxStaticText( m_runoptpanel, ID_RUNDCL, _("Use ASCII DCL as:"), wxDefaultPosition, wxDefaultSize, 0 );
	itemBoxSizer65->Add(m_rundcl, 50, wxALIGN_CENTER_VERTICAL|wxALL, 5);

	m_rundclsrc = new wxRadioButton( m_runoptpanel, ID_RUNDCLSRC, _("Source"), wxDefaultPosition, wxDefaultSize, wxRB_GROUP );
	m_rundclsrc->SetValue(false);
	itemBoxSizer65->Add(m_rundclsrc, 25, wxALIGN_CENTER_VERTICAL|wxALL, 5);

	m_rundclout = new wxRadioButton( m_runoptpanel, ID_RUNDCLOUT, _("Output"), wxDefaultPosition, wxDefaultSize, 0 );
	m_rundclout->SetValue(false);
	itemBoxSizer65->Add(m_rundclout, 25, wxALIGN_CENTER_VERTICAL|wxALL, 5);

	m_rundclno = new wxRadioButton( m_runoptpanel, ID_RUNDCLNO, _("Neither"), wxDefaultPosition, wxDefaultSize, 0 );
	m_rundclno->SetValue(true);
	itemBoxSizer65->Add(m_rundclno, 25, wxALIGN_CENTER_VERTICAL|wxALL, 5);

	m_rundclyes = new wxRadioButton( m_runoptpanel, ID_RUNDCLYES, _("Both"), wxDefaultPosition, wxDefaultSize, 0 );
	m_rundclyes->SetValue(false);
	itemBoxSizer65->Add(m_rundclyes, 25, wxALIGN_CENTER_VERTICAL|wxALL, 5);

	wxBoxSizer* itemBoxSizer71 = new wxBoxSizer(wxHORIZONTAL);
	itemBoxSizer30->Add(itemBoxSizer71, 0, wxGROW|wxALL, 2);
	m_runonly = new wxStaticText( m_runoptpanel, ID_RUNONLY, _("Run only automation tasks:"), wxDefaultPosition, wxDefaultSize, 0 );
	itemBoxSizer71->Add(m_runonly, 50, wxALIGN_CENTER_VERTICAL|wxALL, 5);

	m_runonlyyes = new wxRadioButton( m_runoptpanel, ID_RUNONLYYES, _("Yes"), wxDefaultPosition, wxDefaultSize, wxRB_GROUP );
	m_runonlyyes->SetValue(false);
	itemBoxSizer71->Add(m_runonlyyes, 25, wxALIGN_CENTER_VERTICAL|wxALL, 5);

	m_runonlyno = new wxRadioButton( m_runoptpanel, ID_RUNONLYNO, _("No"), wxDefaultPosition, wxDefaultSize, 0 );
	m_runonlyno->SetValue(true);
	itemBoxSizer71->Add(m_runonlyno, 25, wxALIGN_CENTER_VERTICAL|wxALL, 5);

	GetBookCtrl()->AddPage(m_runoptpanel, _("Run Options"));

	m_createpanel = new wxPanel( GetBookCtrl(), ID_CREATEPANEL, wxDefaultPosition, wxDefaultSize, wxSUNKEN_BORDER|wxTAB_TRAVERSAL );
	wxBoxSizer* itemBoxSizer76 = new wxBoxSizer(wxVERTICAL);
	m_createpanel->SetSizer(itemBoxSizer76);

	wxBoxSizer* itemBoxSizer77 = new wxBoxSizer(wxHORIZONTAL);
	itemBoxSizer76->Add(itemBoxSizer77, 100, wxGROW|wxALL, 5);
	m_projectname = new wxStaticText( m_createpanel, ID_PROJECTNAME, _("Project name:  "), wxDefaultPosition, wxDefaultSize, wxALIGN_LEFT );
	itemBoxSizer77->Add(m_projectname, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

	wxArrayString m_projectselectStrings;
	m_projectselect = new wxComboBox( m_createpanel, ID_PROJECTSEL, wxEmptyString, wxDefaultPosition, wxSize(340, -1), m_projectselectStrings, wxCB_DROPDOWN|wxCB_SORT );
	itemBoxSizer77->Add(m_projectselect, 100, wxALIGN_CENTER_VERTICAL|wxALL, 5);

	wxBoxSizer* itemBoxSizer80 = new wxBoxSizer(wxHORIZONTAL);
	itemBoxSizer76->Add(itemBoxSizer80, 100, wxGROW|wxALL, 5);
	m_sourcedir = new wxStaticText( m_createpanel, ID_SOURCEDIR, _("Source directory:"), wxDefaultPosition, wxDefaultSize, 0 );
	itemBoxSizer80->Add(m_sourcedir, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

	m_srcdirselect = new wxDirPickerCtrl( m_createpanel, ID_SRCDIRSELECT, wxEmptyString, _("Select the DITA source directory for this project:"), wxDefaultPosition, wxSize(200, -1), wxDIRP_USE_TEXTCTRL|wxDIRP_DIR_MUST_EXIST|wxDIRP_CHANGE_DIR );
	itemBoxSizer80->Add(m_srcdirselect, 100, wxALIGN_CENTER_VERTICAL|wxALL, 5);

	wxBoxSizer* itemBoxSizer83 = new wxBoxSizer(wxHORIZONTAL);
	itemBoxSizer76->Add(itemBoxSizer83, 100, wxGROW|wxALL, 5);
	m_sourcemap = new wxStaticText( m_createpanel, ID_SOURCEMAP, _("Source map file:  "), wxDefaultPosition, wxDefaultSize, wxALIGN_LEFT );
	itemBoxSizer83->Add(m_sourcemap, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

	m_srcmapselect = new wxFilePickerCtrl( m_createpanel, ID_SRCMAPDEFSEL, wxEmptyString, _("Select the default ditamap or bookmap to convert:"), wxEmptyString, wxDefaultPosition, wxSize(200, -1), wxFLP_USE_TEXTCTRL|wxFLP_OPEN );
	itemBoxSizer83->Add(m_srcmapselect, 100, wxALIGN_CENTER_VERTICAL|wxALL, 5);

	wxBoxSizer* itemBoxSizer86 = new wxBoxSizer(wxHORIZONTAL);
	itemBoxSizer76->Add(itemBoxSizer86, 100, wxGROW|wxALL, 5);
	m_formatname = new wxStaticText( m_createpanel, ID_FORMATNAME, _("Output type:"), wxDefaultPosition, wxDefaultSize, wxALIGN_LEFT );
	itemBoxSizer86->Add(m_formatname, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

	wxArrayString m_formatselectStrings;
	m_formatselect = new wxChoice( m_createpanel, ID_FORMATSEL, wxDefaultPosition, wxSize(200, -1), m_formatselectStrings, 0 );
	m_formatselect->SetStringSelection(_("OmniHelp"));
	itemBoxSizer86->Add(m_formatselect, 100, wxALIGN_CENTER_VERTICAL|wxALL, 5);

	wxBoxSizer* itemBoxSizer89 = new wxBoxSizer(wxHORIZONTAL);
	itemBoxSizer76->Add(itemBoxSizer89, 100, wxGROW|wxALL, 5);
	m_outputdir = new wxStaticText( m_createpanel, ID_OUTPUTDIR, _("Output directory:"), wxDefaultPosition, wxDefaultSize, wxALIGN_LEFT );
	itemBoxSizer89->Add(m_outputdir, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

	m_outdirselect = new wxDirPickerCtrl( m_createpanel, ID_OUTDIRSELECT, wxEmptyString, _("Select the output directory for this project:"), wxDefaultPosition, wxSize(200, -1), wxDIRP_USE_TEXTCTRL|wxDIRP_CHANGE_DIR );
	itemBoxSizer89->Add(m_outdirselect, 100, wxALIGN_CENTER_VERTICAL|wxALL, 5);

	wxBoxSizer* itemBoxSizer92 = new wxBoxSizer(wxHORIZONTAL);
	itemBoxSizer76->Add(itemBoxSizer92, 100, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);
	m_createproject = new wxButton( m_createpanel, ID_CREATEPROJECT, _("Create Project"), wxDefaultPosition, wxDefaultSize, 0 );
	m_createproject->SetDefault();
	m_createproject->Enable(false);
	itemBoxSizer92->Add(m_createproject, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

	m_iniconfig = new wxButton( m_createpanel, ID_INICONFIG, _("Set Up Ini File"), wxDefaultPosition, wxDefaultSize, 0 );
	itemBoxSizer92->Add(m_iniconfig, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

	m_createcm = new wxButton( m_createpanel, ID_CREATECM, _("Configure"), wxDefaultPosition, wxDefaultSize, 0 );
	itemBoxSizer92->Add(m_createcm, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

	m_editini = new wxButton( m_createpanel, ID_EDITINI, _("Edit Ini File"), wxDefaultPosition, wxDefaultSize, 0 );
	itemBoxSizer92->Add(m_editini, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

	wxBoxSizer* itemBoxSizer97 = new wxBoxSizer(wxHORIZONTAL);
	itemBoxSizer76->Add(itemBoxSizer97, 100, wxGROW|wxLEFT|wxRIGHT|wxBOTTOM, 5);
	m_result = new wxStaticText( m_createpanel, ID_RESULT, _("Results:"), wxDefaultPosition, wxDefaultSize, wxALIGN_LEFT );
	itemBoxSizer97->Add(m_result, 0, wxALIGN_CENTER_VERTICAL|wxLEFT|wxRIGHT|wxBOTTOM, 5);

	m_resulttext = new wxTextCtrl( m_createpanel, ID_RESULTTEXT, wxEmptyString, wxDefaultPosition, wxSize(230, -1), wxTE_READONLY );
	itemBoxSizer97->Add(m_resulttext, 100, wxALIGN_CENTER_VERTICAL|wxLEFT|wxRIGHT|wxBOTTOM, 5);

	GetBookCtrl()->AddPage(m_createpanel, _("Create Project"));

	m_modifypanel = new wxPanel( GetBookCtrl(), ID_MODIFYPANEL, wxDefaultPosition, wxDefaultSize, wxSUNKEN_BORDER|wxTAB_TRAVERSAL );
	wxBoxSizer* itemBoxSizer101 = new wxBoxSizer(wxVERTICAL);
	m_modifypanel->SetSizer(itemBoxSizer101);

	wxBoxSizer* itemBoxSizer102 = new wxBoxSizer(wxHORIZONTAL);
	itemBoxSizer101->Add(itemBoxSizer102, 100, wxGROW|wxALL, 5);
	m_mprojectname = new wxStaticText( m_modifypanel, ID_MPROJECTNAME, _("Project name:  "), wxDefaultPosition, wxDefaultSize, wxALIGN_LEFT );
	itemBoxSizer102->Add(m_mprojectname, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

	wxArrayString m_mprojectselectStrings;
	m_mprojectselect = new wxChoice( m_modifypanel, ID_MPROJECTSEL, wxDefaultPosition, wxSize(200, -1), m_mprojectselectStrings, 0 );
	itemBoxSizer102->Add(m_mprojectselect, 100, wxALIGN_CENTER_VERTICAL|wxALL, 5);

	wxBoxSizer* itemBoxSizer105 = new wxBoxSizer(wxHORIZONTAL);
	itemBoxSizer101->Add(itemBoxSizer105, 100, wxGROW|wxALL, 5);
	m_msourcedir = new wxStaticText( m_modifypanel, ID_MSOURCEDIR, _("Source directory:"), wxDefaultPosition, wxDefaultSize, 0 );
	itemBoxSizer105->Add(m_msourcedir, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

	m_msrcdirselect = new wxDirPickerCtrl( m_modifypanel, ID_MSRCDIRSELECT, wxEmptyString, _("Select the DITA source directory for this project:"), wxDefaultPosition, wxSize(200, -1), wxDIRP_USE_TEXTCTRL|wxDIRP_DIR_MUST_EXIST|wxDIRP_CHANGE_DIR );
	itemBoxSizer105->Add(m_msrcdirselect, 100, wxALIGN_CENTER_VERTICAL|wxALL, 5);

	wxBoxSizer* itemBoxSizer108 = new wxBoxSizer(wxHORIZONTAL);
	itemBoxSizer101->Add(itemBoxSizer108, 100, wxGROW|wxALL, 5);
	m_msourcemap = new wxStaticText( m_modifypanel, ID_MSOURCEMAP, _("Source map file:  "), wxDefaultPosition, wxDefaultSize, wxALIGN_LEFT );
	itemBoxSizer108->Add(m_msourcemap, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

	m_msrcmapselect = new wxFilePickerCtrl( m_modifypanel, ID_MSRCMAPDEFSEL, wxEmptyString, _("Select the default ditamap or bookmap to convert:"), wxEmptyString, wxDefaultPosition, wxSize(200, -1), wxFLP_USE_TEXTCTRL|wxFLP_OPEN );
	itemBoxSizer108->Add(m_msrcmapselect, 100, wxALIGN_CENTER_VERTICAL|wxALL, 5);

	wxBoxSizer* itemBoxSizer111 = new wxBoxSizer(wxHORIZONTAL);
	itemBoxSizer101->Add(itemBoxSizer111, 100, wxGROW|wxALL, 5);
	m_mformatname = new wxStaticText( m_modifypanel, ID_MFORMATNAME, _("Output type:"), wxDefaultPosition, wxDefaultSize, wxALIGN_LEFT );
	itemBoxSizer111->Add(m_mformatname, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

	wxArrayString m_mformatselectStrings;
	m_mformatselect = new wxChoice( m_modifypanel, ID_MFORMATSEL, wxDefaultPosition, wxSize(200, -1), m_mformatselectStrings, 0 );
	m_mformatselect->SetStringSelection(_("OmniHelp"));
	itemBoxSizer111->Add(m_mformatselect, 100, wxALIGN_CENTER_VERTICAL|wxALL, 5);

	wxBoxSizer* itemBoxSizer114 = new wxBoxSizer(wxHORIZONTAL);
	itemBoxSizer101->Add(itemBoxSizer114, 100, wxGROW|wxALL, 5);
	m_moutputdir = new wxStaticText( m_modifypanel, ID_MOUTPUTDIR, _("Output directory:"), wxDefaultPosition, wxDefaultSize, wxALIGN_LEFT );
	itemBoxSizer114->Add(m_moutputdir, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

	m_moutdirselect = new wxDirPickerCtrl( m_modifypanel, ID_MOUTDIRSELECT, wxEmptyString, _("Select the default output directory for this project:"), wxDefaultPosition, wxSize(200, -1), wxDIRP_USE_TEXTCTRL|wxDIRP_CHANGE_DIR );
	itemBoxSizer114->Add(m_moutdirselect, 100, wxALIGN_CENTER_VERTICAL|wxALL, 5);

	wxBoxSizer* itemBoxSizer117 = new wxBoxSizer(wxHORIZONTAL);
	itemBoxSizer101->Add(itemBoxSizer117, 100, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);
	m_modifyproject = new wxButton( m_modifypanel, ID_MODIFYPROJECT, _("Modify Project"), wxDefaultPosition, wxDefaultSize, 0 );
	m_modifyproject->SetDefault();
	itemBoxSizer117->Add(m_modifyproject, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

	m_miniconfig = new wxButton( m_modifypanel, ID_MINICONFIG, _("Set Up Ini File"), wxDefaultPosition, wxDefaultSize, 0 );
	itemBoxSizer117->Add(m_miniconfig, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

	m_modifycm = new wxButton( m_modifypanel, ID_MODIFYCM, _("Configure"), wxDefaultPosition, wxDefaultSize, 0 );
	itemBoxSizer117->Add(m_modifycm, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

	m_meditini = new wxButton( m_modifypanel, ID_MEDITINI, _("Edit Ini File"), wxDefaultPosition, wxDefaultSize, 0 );
	itemBoxSizer117->Add(m_meditini, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

	wxBoxSizer* itemBoxSizer122 = new wxBoxSizer(wxHORIZONTAL);
	itemBoxSizer101->Add(itemBoxSizer122, 100, wxGROW|wxLEFT|wxRIGHT|wxBOTTOM, 5);
	m_mresult = new wxStaticText( m_modifypanel, ID_MRESULT, _("Results:"), wxDefaultPosition, wxDefaultSize, wxALIGN_LEFT );
	itemBoxSizer122->Add(m_mresult, 0, wxALIGN_CENTER_VERTICAL|wxLEFT|wxRIGHT|wxBOTTOM, 5);

	m_mresulttext = new wxTextCtrl( m_modifypanel, ID_MRESULTTEXT, wxEmptyString, wxDefaultPosition, wxSize(230, -1), wxTE_READONLY );
	itemBoxSizer122->Add(m_mresulttext, 100, wxALIGN_CENTER_VERTICAL|wxLEFT|wxRIGHT|wxBOTTOM, 5);

	GetBookCtrl()->AddPage(m_modifypanel, _("Modify Project"));

	m_deletepanel = new wxPanel( GetBookCtrl(), ID_DELETEPANEL, wxDefaultPosition, wxDefaultSize, wxSUNKEN_BORDER|wxTAB_TRAVERSAL );
	wxBoxSizer* itemBoxSizer126 = new wxBoxSizer(wxVERTICAL);
	m_deletepanel->SetSizer(itemBoxSizer126);

	wxBoxSizer* itemBoxSizer127 = new wxBoxSizer(wxHORIZONTAL);
	itemBoxSizer126->Add(itemBoxSizer127, 100, wxGROW|wxALL, 5);
	m_dprojectname = new wxStaticText( m_deletepanel, ID_DPROJECTNAME, _("Project name:  "), wxDefaultPosition, wxDefaultSize, wxALIGN_LEFT );
	itemBoxSizer127->Add(m_dprojectname, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

	wxArrayString m_dprojectselectStrings;
	m_dprojectselect = new wxChoice( m_deletepanel, ID_DPROJECTSEL, wxDefaultPosition, wxSize(200, -1), m_dprojectselectStrings, 0 );
	itemBoxSizer127->Add(m_dprojectselect, 100, wxALIGN_CENTER_VERTICAL|wxALL, 5);

	wxBoxSizer* itemBoxSizer130 = new wxBoxSizer(wxHORIZONTAL);
	itemBoxSizer126->Add(itemBoxSizer130, 100, wxGROW|wxALL, 5);
	m_dsourcedir = new wxStaticText( m_deletepanel, ID_DSOURCEDIR, _("Source directory:"), wxDefaultPosition, wxDefaultSize, 0 );
	itemBoxSizer130->Add(m_dsourcedir, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

	m_dsrcdirtext = new wxTextCtrl( m_deletepanel, ID_DSRCDIRTEXT, wxEmptyString, wxDefaultPosition, wxSize(200, -1), wxTE_READONLY|wxTE_NOHIDESEL|wxTE_LEFT );
	m_dsrcdirtext->SetBackgroundColour(wxColour(192, 192, 192));
	itemBoxSizer130->Add(m_dsrcdirtext, 100, wxALIGN_CENTER_VERTICAL|wxALL, 5);

	wxBoxSizer* itemBoxSizer133 = new wxBoxSizer(wxHORIZONTAL);
	itemBoxSizer126->Add(itemBoxSizer133, 100, wxGROW|wxALL, 5);
	m_dsourcemap = new wxStaticText( m_deletepanel, ID_DSOURCEMAP, _("Source map file:"), wxDefaultPosition, wxDefaultSize, 0 );
	itemBoxSizer133->Add(m_dsourcemap, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

	m_dsrcmaptext = new wxTextCtrl( m_deletepanel, ID_DSRCMAPTEXT, wxEmptyString, wxDefaultPosition, wxSize(200, -1), wxTE_READONLY|wxTE_NOHIDESEL|wxTE_LEFT );
	m_dsrcmaptext->SetBackgroundColour(wxColour(192, 192, 192));
	itemBoxSizer133->Add(m_dsrcmaptext, 100, wxALIGN_CENTER_VERTICAL|wxALL, 5);

	wxBoxSizer* itemBoxSizer136 = new wxBoxSizer(wxHORIZONTAL);
	itemBoxSizer126->Add(itemBoxSizer136, 100, wxGROW|wxALL, 5);
	m_dformatname = new wxStaticText( m_deletepanel, ID_DFORMATNAME, _("Output type:"), wxDefaultPosition, wxDefaultSize, wxALIGN_LEFT );
	itemBoxSizer136->Add(m_dformatname, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

	m_dformattext = new wxTextCtrl( m_deletepanel, ID_DFORMATTEXT, wxEmptyString, wxDefaultPosition, wxSize(200, -1), wxTE_READONLY|wxTE_NOHIDESEL|wxTE_LEFT );
	m_dformattext->SetMaxLength(200);
	m_dformattext->SetBackgroundColour(wxColour(192, 192, 192));
	itemBoxSizer136->Add(m_dformattext, 100, wxALIGN_CENTER_VERTICAL|wxALL, 5);

	wxBoxSizer* itemBoxSizer139 = new wxBoxSizer(wxHORIZONTAL);
	itemBoxSizer126->Add(itemBoxSizer139, 100, wxGROW|wxALL, 5);
	m_doutputdir = new wxStaticText( m_deletepanel, ID_DOUTPUTDIR, _("Output directory:"), wxDefaultPosition, wxDefaultSize, wxALIGN_LEFT );
	itemBoxSizer139->Add(m_doutputdir, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

	m_doutdirtext = new wxTextCtrl( m_deletepanel, ID_DOUTDIRTEXT, wxEmptyString, wxDefaultPosition, wxSize(200, -1), wxTE_READONLY|wxTE_NOHIDESEL|wxTE_LEFT );
	m_doutdirtext->SetBackgroundColour(wxColour(192, 192, 192));
	itemBoxSizer139->Add(m_doutdirtext, 100, wxALIGN_CENTER_VERTICAL|wxALL, 5);

	wxBoxSizer* itemBoxSizer142 = new wxBoxSizer(wxHORIZONTAL);
	itemBoxSizer126->Add(itemBoxSizer142, 100, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);
	m_deleteproject = new wxButton( m_deletepanel, ID_DELETEPROJECT, _("Delete Project"), wxDefaultPosition, wxDefaultSize, 0 );
	m_deleteproject->Enable(false);
	itemBoxSizer142->Add(m_deleteproject, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

	wxBoxSizer* itemBoxSizer144 = new wxBoxSizer(wxHORIZONTAL);
	itemBoxSizer126->Add(itemBoxSizer144, 100, wxGROW|wxLEFT|wxRIGHT|wxBOTTOM, 5);
	m_dresult = new wxStaticText( m_deletepanel, ID_DRESULT, _("Results:"), wxDefaultPosition, wxDefaultSize, wxALIGN_LEFT );
	itemBoxSizer144->Add(m_dresult, 0, wxALIGN_CENTER_VERTICAL|wxLEFT|wxRIGHT|wxBOTTOM, 5);

	m_dresulttext = new wxTextCtrl( m_deletepanel, ID_DRESULTTEXT, wxEmptyString, wxDefaultPosition, wxSize(230, -1), wxTE_READONLY );
	itemBoxSizer144->Add(m_dresulttext, 100, wxALIGN_CENTER_VERTICAL|wxLEFT|wxRIGHT|wxBOTTOM, 5);

	GetBookCtrl()->AddPage(m_deletepanel, _("Delete Project"));

	m_restorepanel = new wxPanel( GetBookCtrl(), ID_RESTOREPANEL, wxDefaultPosition, wxDefaultSize, wxSUNKEN_BORDER|wxTAB_TRAVERSAL );
	wxBoxSizer* itemBoxSizer148 = new wxBoxSizer(wxVERTICAL);
	m_restorepanel->SetSizer(itemBoxSizer148);

	wxBoxSizer* itemBoxSizer149 = new wxBoxSizer(wxHORIZONTAL);
	itemBoxSizer148->Add(itemBoxSizer149, 100, wxGROW|wxALL, 5);
	m_rprojectname = new wxStaticText( m_restorepanel, ID_RPROJECTNAME, _("Project name:  "), wxDefaultPosition, wxDefaultSize, wxALIGN_LEFT );
	itemBoxSizer149->Add(m_rprojectname, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

	wxArrayString m_rprojectselectStrings;
	m_rprojectselect = new wxChoice( m_restorepanel, ID_RPROJECTSEL, wxDefaultPosition, wxSize(200, -1), m_rprojectselectStrings, 0 );
	itemBoxSizer149->Add(m_rprojectselect, 100, wxALIGN_CENTER_VERTICAL|wxALL, 5);

	wxBoxSizer* itemBoxSizer152 = new wxBoxSizer(wxHORIZONTAL);
	itemBoxSizer148->Add(itemBoxSizer152, 100, wxGROW|wxALL, 5);
	m_rsourcedir = new wxStaticText( m_restorepanel, ID_RSOURCEDIR, _("Source directory:"), wxDefaultPosition, wxDefaultSize, 0 );
	itemBoxSizer152->Add(m_rsourcedir, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

	m_rsrcdirtext = new wxTextCtrl( m_restorepanel, ID_RSRCDIRTEXT, wxEmptyString, wxDefaultPosition, wxSize(200, -1), wxTE_READONLY|wxTE_NOHIDESEL|wxTE_LEFT );
	m_rsrcdirtext->SetBackgroundColour(wxColour(192, 192, 192));
	itemBoxSizer152->Add(m_rsrcdirtext, 100, wxALIGN_CENTER_VERTICAL|wxALL, 5);

	wxBoxSizer* itemBoxSizer155 = new wxBoxSizer(wxHORIZONTAL);
	itemBoxSizer148->Add(itemBoxSizer155, 100, wxGROW|wxALL, 5);
	m_rsourcemap = new wxStaticText( m_restorepanel, ID_RSOURCEMAP, _("Source map file:"), wxDefaultPosition, wxDefaultSize, 0 );
	itemBoxSizer155->Add(m_rsourcemap, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

	m_rsrcmaptext = new wxTextCtrl( m_restorepanel, ID_RSRCMAPTEXT, wxEmptyString, wxDefaultPosition, wxSize(200, -1), wxTE_READONLY|wxTE_NOHIDESEL|wxTE_LEFT );
	m_rsrcmaptext->SetBackgroundColour(wxColour(192, 192, 192));
	itemBoxSizer155->Add(m_rsrcmaptext, 100, wxALIGN_CENTER_VERTICAL|wxALL, 5);

	wxBoxSizer* itemBoxSizer158 = new wxBoxSizer(wxHORIZONTAL);
	itemBoxSizer148->Add(itemBoxSizer158, 100, wxGROW|wxALL, 5);
	m_rformatname = new wxStaticText( m_restorepanel, ID_RFORMATNAME, _("Output type:"), wxDefaultPosition, wxDefaultSize, wxALIGN_LEFT );
	itemBoxSizer158->Add(m_rformatname, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

	m_rformattext = new wxTextCtrl( m_restorepanel, ID_RFORMATTEXT, wxEmptyString, wxDefaultPosition, wxSize(200, -1), wxTE_READONLY|wxTE_NOHIDESEL|wxTE_LEFT );
	m_rformattext->SetMaxLength(200);
	m_rformattext->SetBackgroundColour(wxColour(192, 192, 192));
	itemBoxSizer158->Add(m_rformattext, 100, wxALIGN_CENTER_VERTICAL|wxALL, 5);

	wxBoxSizer* itemBoxSizer161 = new wxBoxSizer(wxHORIZONTAL);
	itemBoxSizer148->Add(itemBoxSizer161, 100, wxGROW|wxALL, 5);
	m_routputdir = new wxStaticText( m_restorepanel, ID_ROUTPUTDIR, _("Output directory:"), wxDefaultPosition, wxDefaultSize, wxALIGN_LEFT );
	itemBoxSizer161->Add(m_routputdir, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

	m_routdirtext = new wxTextCtrl( m_restorepanel, ID_ROUTDIRTEXT, wxEmptyString, wxDefaultPosition, wxSize(200, -1), wxTE_READONLY|wxTE_NOHIDESEL|wxTE_LEFT );
	m_routdirtext->SetBackgroundColour(wxColour(192, 192, 192));
	itemBoxSizer161->Add(m_routdirtext, 100, wxALIGN_CENTER_VERTICAL|wxALL, 5);

	wxBoxSizer* itemBoxSizer164 = new wxBoxSizer(wxHORIZONTAL);
	itemBoxSizer148->Add(itemBoxSizer164, 100, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);
	m_restoreproject = new wxButton( m_restorepanel, ID_RESTOREPROJECT, _("Restore Project"), wxDefaultPosition, wxDefaultSize, 0 );
	itemBoxSizer164->Add(m_restoreproject, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

	wxBoxSizer* itemBoxSizer166 = new wxBoxSizer(wxHORIZONTAL);
	itemBoxSizer148->Add(itemBoxSizer166, 100, wxGROW|wxLEFT|wxRIGHT|wxBOTTOM, 5);
	m_rresult = new wxStaticText( m_restorepanel, ID_RRESULT, _("Results:"), wxDefaultPosition, wxDefaultSize, wxALIGN_LEFT );
	itemBoxSizer166->Add(m_rresult, 0, wxALIGN_CENTER_VERTICAL|wxLEFT|wxRIGHT|wxBOTTOM, 5);

	m_rresulttext = new wxTextCtrl( m_restorepanel, ID_RRESULTTEXT, wxEmptyString, wxDefaultPosition, wxSize(230, -1), wxTE_READONLY );
	itemBoxSizer166->Add(m_rresulttext, 100, wxALIGN_CENTER_VERTICAL|wxLEFT|wxRIGHT|wxBOTTOM, 5);

	GetBookCtrl()->AddPage(m_restorepanel, _("Restore Project"));

	m_optionspanel = new wxPanel( GetBookCtrl(), ID_OPTIONSPANEL, wxDefaultPosition, wxDefaultSize, wxSUNKEN_BORDER|wxTAB_TRAVERSAL );
	wxBoxSizer* itemBoxSizer170 = new wxBoxSizer(wxVERTICAL);
	m_optionspanel->SetSizer(itemBoxSizer170);

	wxBoxSizer* itemBoxSizer171 = new wxBoxSizer(wxHORIZONTAL);
	itemBoxSizer170->Add(itemBoxSizer171, 100, wxGROW|wxLEFT|wxRIGHT|wxTOP, 5);
	m_opteditor = new wxStaticText( m_optionspanel, ID_OPTEDITOR, _("Text editor:    "), wxDefaultPosition, wxDefaultSize, 0 );
	itemBoxSizer171->Add(m_opteditor, 0, wxALIGN_CENTER_VERTICAL|wxLEFT|wxRIGHT|wxTOP, 5);

	m_opteditortext = new wxTextCtrl( m_optionspanel, ID_OPTEDITORTEXT, wxEmptyString, wxDefaultPosition, wxSize(200, -1), wxTE_PROCESS_ENTER );
	itemBoxSizer171->Add(m_opteditortext, 100, wxALIGN_CENTER_VERTICAL|wxLEFT|wxRIGHT|wxTOP, 5);

	wxBoxSizer* itemBoxSizer174 = new wxBoxSizer(wxHORIZONTAL);
	itemBoxSizer170->Add(itemBoxSizer174, 100, wxGROW|wxALL, 5);
	m_optshdel = new wxStaticText( m_optionspanel, ID_OPTSHDEL, _("Show deleted projects in Create list:"), wxDefaultPosition, wxDefaultSize, 0 );
	itemBoxSizer174->Add(m_optshdel, 100, wxALIGN_CENTER_VERTICAL|wxALL, 5);

	m_optshdelyes = new wxRadioButton( m_optionspanel, ID_OPTSHDELYES, _("Yes"), wxDefaultPosition, wxDefaultSize, wxRB_GROUP );
	m_optshdelyes->SetValue(true);
	itemBoxSizer174->Add(m_optshdelyes, 50, wxALIGN_CENTER_VERTICAL|wxALL, 5);

	m_optshdelno = new wxRadioButton( m_optionspanel, ID_OPTSHDELNO, _("No"), wxDefaultPosition, wxDefaultSize, 0 );
	m_optshdelno->SetValue(false);
	itemBoxSizer174->Add(m_optshdelno, 50, wxALIGN_CENTER_VERTICAL|wxALL, 5);

	wxBoxSizer* itemBoxSizer178 = new wxBoxSizer(wxHORIZONTAL);
	itemBoxSizer170->Add(itemBoxSizer178, 100, wxGROW|wxALL, 5);
	m_optshow = new wxStaticText( m_optionspanel, ID_OPTSHOW, _("Always show when you run a project:"), wxDefaultPosition, wxDefaultSize, wxALIGN_LEFT );
	itemBoxSizer178->Add(m_optshow, 100, wxALIGN_CENTER_VERTICAL|wxALL, 5);

	m_optshowcon = new wxCheckBox( m_optionspanel, ID_OPTSHOWCON, _("Console window"), wxDefaultPosition, wxDefaultSize, 0 );
	m_optshowcon->SetValue(false);
	itemBoxSizer178->Add(m_optshowcon, 50, wxALIGN_CENTER_VERTICAL|wxALL, 5);

	m_optshowlog = new wxCheckBox( m_optionspanel, ID_OPTSHOWLOG, _("Log file    "), wxDefaultPosition, wxDefaultSize, wxCHK_2STATE );
	m_optshowlog->SetValue(false);
	itemBoxSizer178->Add(m_optshowlog, 50, wxALIGN_CENTER_VERTICAL|wxALL, 5);

	wxBoxSizer* itemBoxSizer182 = new wxBoxSizer(wxHORIZONTAL);
	itemBoxSizer170->Add(itemBoxSizer182, 100, wxGROW|wxALL, 5);
	m_optlogfile = new wxStaticText( m_optionspanel, ID_OPTLOGFILE, _("Log file name (in output directory):"), wxDefaultPosition, wxDefaultSize, 0 );
	itemBoxSizer182->Add(m_optlogfile, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

	m_optlogfiletext = new wxTextCtrl( m_optionspanel, ID_OPTLOGFILETEXT, wxEmptyString, wxDefaultPosition, wxSize(200, -1), wxTE_PROCESS_ENTER );
	itemBoxSizer182->Add(m_optlogfiletext, 100, wxALIGN_CENTER_VERTICAL|wxALL, 5);

	wxBoxSizer* itemBoxSizer185 = new wxBoxSizer(wxHORIZONTAL);
	itemBoxSizer170->Add(itemBoxSizer185, 100, wxGROW|wxALL, 5);
	m_optnewini = new wxStaticText( m_optionspanel, ID_OPTNEWINI, _("Get starting .inis for new projects from:"), wxDefaultPosition, wxDefaultSize, 0 );
	itemBoxSizer185->Add(m_optnewini, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

	m_optnewinisel = new wxDirPickerCtrl( m_optionspanel, ID_OPTNEWINISEL, wxEmptyString, _("Select directory containing new starting .ini files:"), wxDefaultPosition, wxSize(200, -1), wxDIRP_DEFAULT_STYLE );
	itemBoxSizer185->Add(m_optnewinisel, 100, wxALIGN_CENTER_VERTICAL|wxALL, 5);

	wxBoxSizer* itemBoxSizer188 = new wxBoxSizer(wxHORIZONTAL);
	itemBoxSizer170->Add(itemBoxSizer188, 0, wxGROW|wxLEFT|wxRIGHT|wxTOP, 5);
	m_optprojini = new wxStaticText( m_optionspanel, ID_OPTPROJINI, _("Default locations for document.ini files for new projects:"), wxDefaultPosition, wxDefaultSize, wxALIGN_LEFT );
	itemBoxSizer188->Add(m_optprojini, 90, wxALIGN_CENTER_VERTICAL|wxALL, 5);

	m_optprojinid2gbut = new wxRadioButton( m_optionspanel, ID_OPTPROJINID2GBUT, _("Central"), wxDefaultPosition, wxDefaultSize, wxRB_GROUP );
	m_optprojinid2gbut->SetValue(true);
	itemBoxSizer188->Add(m_optprojinid2gbut, 30, wxALIGN_CENTER_VERTICAL|wxALL, 5);

	m_optprojinilocbut = new wxRadioButton( m_optionspanel, ID_OPTPROJINILOCBUT, _("Local"), wxDefaultPosition, wxDefaultSize, 0 );
	m_optprojinilocbut->SetValue(false);
	itemBoxSizer188->Add(m_optprojinilocbut, 30, wxALIGN_CENTER_VERTICAL|wxALL, 5);

	wxBoxSizer* itemBoxSizer192 = new wxBoxSizer(wxHORIZONTAL);
	itemBoxSizer170->Add(itemBoxSizer192, 0, wxGROW|wxALL, 5);
	itemBoxSizer192->Add(5, 5, 0, wxALIGN_CENTER_VERTICAL|wxLEFT|wxRIGHT, 5);

	m_optprojinid2g = new wxStaticText( m_optionspanel, ID_OPTPROJINID2G, _("Central, parent directory:"), wxDefaultPosition, wxDefaultSize, 0 );
	itemBoxSizer192->Add(m_optprojinid2g, 0, wxALIGN_CENTER_VERTICAL|wxLEFT|wxRIGHT, 5);

	m_optprojinid2gsel = new wxDirPickerCtrl( m_optionspanel, ID_OPTPROJINID2GSEL, wxEmptyString, _("Select the parent directory for source-specific .ini files:"), wxDefaultPosition, wxDefaultSize, wxDIRP_USE_TEXTCTRL );
	itemBoxSizer192->Add(m_optprojinid2gsel, 100, wxALIGN_CENTER_VERTICAL|wxLEFT|wxRIGHT, 5);

	wxBoxSizer* itemBoxSizer196 = new wxBoxSizer(wxHORIZONTAL);
	itemBoxSizer170->Add(itemBoxSizer196, 0, wxGROW|wxALL, 5);
	itemBoxSizer196->Add(5, 5, 0, wxALIGN_CENTER_VERTICAL|wxLEFT|wxRIGHT|wxBOTTOM, 5);

	m_optprojiniloc = new wxStaticText( m_optionspanel, ID_OPTPROJINILOC, _("Local, relative to output directory:"), wxDefaultPosition, wxDefaultSize, 0 );
	itemBoxSizer196->Add(m_optprojiniloc, 0, wxALIGN_CENTER_VERTICAL|wxLEFT|wxRIGHT|wxBOTTOM, 5);

	m_optprojiniloctext = new wxTextCtrl( m_optionspanel, ID_OPTPROJINILOCTEXT, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER|wxTE_LEFT );
	itemBoxSizer196->Add(m_optprojiniloctext, 100, wxALIGN_CENTER_VERTICAL|wxLEFT|wxRIGHT|wxBOTTOM, 5);

	wxBoxSizer* itemBoxSizer200 = new wxBoxSizer(wxHORIZONTAL);
	itemBoxSizer170->Add(itemBoxSizer200, 100, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);
	m_optset = new wxButton( m_optionspanel, ID_OPTSET, _("Set Options"), wxDefaultPosition, wxDefaultSize, 0 );
	m_optset->SetDefault();
	itemBoxSizer200->Add(m_optset, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

	m_optreset = new wxButton( m_optionspanel, ID_OPTRESET, _("Reset Options"), wxDefaultPosition, wxDefaultSize, 0 );
	itemBoxSizer200->Add(m_optreset, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

	GetBookCtrl()->AddPage(m_optionspanel, _("Preferences"));

	// Connect events and objects
	m_runpanel->Connect(ID_RUNPANEL, wxEVT_INIT_DIALOG, wxInitDialogEventHandler(D2gmain::OnInitRunDialog), NULL, this);
	m_createpanel->Connect(ID_CREATEPANEL, wxEVT_INIT_DIALOG, wxInitDialogEventHandler(D2gmain::OnInitSetupDialog), NULL, this);
	m_modifypanel->Connect(ID_MODIFYPANEL, wxEVT_INIT_DIALOG, wxInitDialogEventHandler(D2gmain::OnInitModifyDialog), NULL, this);
	m_deletepanel->Connect(ID_DELETEPANEL, wxEVT_INIT_DIALOG, wxInitDialogEventHandler(D2gmain::OnInitDeleteDialog), NULL, this);
	m_restorepanel->Connect(ID_RESTOREPANEL, wxEVT_INIT_DIALOG, wxInitDialogEventHandler(D2gmain::OnInitRestoreDialog), NULL, this);
	m_optionspanel->Connect(ID_OPTIONSPANEL, wxEVT_INIT_DIALOG, wxInitDialogEventHandler(D2gmain::OnInitOptionsDialog), NULL, this);
	m_optnewinisel->Connect(ID_OPTNEWINISEL, wxEVT_CHAR, wxKeyEventHandler(D2gmain::OnChar), NULL, this);
////@end D2gmain content construction

	SetAllLabels();
	SetAllToolTips();
	SetAllResults();
}


/*
 * Should we show tooltips?
 */

bool D2gmain::ShowToolTips()
{
	return true;
}


/*
 * Get bitmap resources
 */

wxBitmap D2gmain::GetBitmapResource( const wxString& name )
{
	// Bitmap retrieval
////@begin D2gmain bitmap retrieval
	wxUnusedVar(name);
	return wxNullBitmap;
////@end D2gmain bitmap retrieval
}

/*
 * Get icon resources
 */

wxIcon D2gmain::GetIconResource( const wxString& name )
{
	// Icon retrieval
	wxIcon icon(name);
  return icon;
}



// beginning of event handlers


/*
 * wxEVT_INIT_DIALOG event handler for ID_D2GMAIN
 */

void D2gmain::OnInitDialog( wxInitDialogEvent& event )
{
	// enable buttons
  m_createproject->Enable();

	if (ActiveProjects
	 && (ActiveProjects->GetCount() > 0)) {
		m_modifyproject->Enable();
		m_deleteproject->Enable();
		m_runstart->Enable();
	}
	else {
		m_modifyproject->Disable();
		m_deleteproject->Disable();
		m_runstart->Disable();
	}

	if (DeletedProjects
	 && (DeletedProjects->GetCount() > 0))
		m_restoreproject->Enable();
	else
		m_restoreproject->Disable();

	m_editini->Disable();
	m_createcm->Disable();
	m_iniconfig->Disable();
	m_meditini->Disable();
	m_modifycm->Disable();
	m_miniconfig->Disable();
	m_runeditini->Disable();
	m_runcm->Disable();
	m_runshowlog->Disable();
	m_runstop->Disable();

	// fill in options panel
	SetOptionsValues();
	m_optset->Disable();
	m_optreset->Enable();

	// force use of browse for all pickers
	(m_srcdirselect->GetTextCtrl())->SetEditable(false);
	(m_srcmapselect->GetTextCtrl())->SetEditable(false);
	(m_outdirselect->GetTextCtrl())->SetEditable(false);
	(m_msrcdirselect->GetTextCtrl())->SetEditable(false);
	(m_msrcmapselect->GetTextCtrl())->SetEditable(false);
	(m_moutdirselect->GetTextCtrl())->SetEditable(false);
	(m_runsrcmapsel->GetTextCtrl())->SetEditable(false);
	(m_optnewinisel->GetTextCtrl())->SetEditable(false);
	(m_optprojinid2gsel->GetTextCtrl())->SetEditable(false);

	event.Skip();
}



// setup dialog functions

/*
 * wxEVT_INIT_DIALOG event handler for ID_SETUPPANEL
 */

void D2gmain::OnInitSetupDialog( wxInitDialogEvent& event )
{

	event.Skip();
}


/*
 * wxEVT_COMMAND_TEXT_ENTER event handler for ID_PROJECTSEL
 */

void D2gmain::OnProjectselEnter( wxCommandEvent& event )
{
	// done typing a new name, leave current selections
	// no action needed
	event.Skip();
}


/*
 * wxEVT_COMMAND_COMBOBOX_SELECTED event handler for ID_PROJECTSEL
 */

void D2gmain::OnProjectselSelected( wxCommandEvent& event )
{
	// existing project selected, presumably to edit name
	// get properties for it
	wxString wxs = m_projectselect->GetValue();
	wxString *project = new wxString(wxs);

	SelectCreateProject(project);

	wxUnusedVar(event);
}


/*
 * wxEVT_UPDATE_UI event handler for ID_PROJECTSEL
 */

void D2gmain::OnProjectselUpdate( wxUpdateUIEvent& event )
{
	// ensure list has all projects, Active and Deleted, in it
	if (ProjectsChanged) {
		m_projectselect->Clear();
		if (ActiveProjects)
			m_projectselect->Append(*ActiveProjects);
		if (DeletedProjects
		 && ShowDelInCreate)
			m_projectselect->Append(*DeletedProjects);
		ProjectsChanged = false;
	}

	event.Skip();
}


/*
 * wxEVT_DIRPICKER_CHANGED event handler for ID_SRCDIRDEFSEL
 */

void D2gmain::OnSrcdirselectDirPickerChanged( wxFileDirPickerEvent& event )
{
	// set the srcmapsel to look in this dir
	// set the choice of srcdirdefsel for projects.ini
	CreateSourceDir = new wxString(m_srcdirselect->GetPath());
	if (!CreateSourceMap)
		CreateSourceMap = GetCreateSourceMap();
	if (CreateSourceMap)
	  m_srcmapselect->SetPath(*MakeFullPath(CreateSourceDir, CreateSourceMap));
	if (!CreateOutputDir)
		m_outdirselect->SetPath(*CreateSourceDir);
  event.Skip();
}


/*
 * wxEVT_FILEPICKER_CHANGED event handler for ID_SRCMAPDEFSEL
 */

void D2gmain::OnSrcmapdefselFilePickerChanged( wxFileDirPickerEvent& event )
{
	// if dir different from srcmapsel, change srcmapsel
	// set the default srcmapdefsel file for projects.ini
	CreateSourceMap = FileFromPath(new wxString(m_srcmapselect->GetPath()));
	CreateSourceDir = DirFromPath(new wxString(m_srcmapselect->GetPath()));
  m_srcdirselect->SetPath(*CreateSourceDir);
  event.Skip();
}


/*
 * wxEVT_COMMAND_CHOICE_SELECTED event handler for ID_FORMATSEL
 */

void D2gmain::OnFormatselSelected( wxCommandEvent& event )
{
	// set the selected formatsel for projects.ini
	CreateFormat = new wxString(m_formatselect->GetStringSelection());
  event.Skip();
}


/*
 * wxEVT_UPDATE_UI event handler for ID_FORMATSEL
 */

void D2gmain::OnFormatselUpdate( wxUpdateUIEvent& event )
{
	// set the list of available formats from d2gdat.ini [Formats]
	// set the format for the last active project as default
	if (FormatsChanged) {
		m_formatselect->Clear();
		m_formatselect->Append(*OutputFormats);
		FormatsChanged = false;
	}

	// set last one selected in [Projects]LastActive as current one
	if (CurrFormatChanged) {
		CurrFormatChanged = false;
		CreateFormat = new wxString("OmniHelp");
		m_formatselect->SetStringSelection(*CreateFormat);
	}

  event.Skip();
}


/*
 * wxEVT_DIRPICKER_CHANGED event handler for ID_OUTDIRDEFSEL
 */

void D2gmain::OnOutdirselectDirPickerChanged( wxFileDirPickerEvent& event )
{
	// set the selected outdirdefsel for projects.ini
	CreateOutputDir = new wxString(m_outdirselect->GetPath());
	SetCurrProjDirLoc(CreateOutputDir);
  event.Skip();
}

#if 0
/*
 * wxEVT_UPDATE_UI event handler for ID_PROJINID2G
 */

void D2gmain::OnProjinid2gUpdate( wxUpdateUIEvent& event )
{
	if (SetCreateProjIni) {
		if (!ProjDirUsed)
			m_projinino->SetValue(true);
		else if (ProjDirLocal)
			m_projiniloc->SetValue(true);
		else
			m_projinid2g->SetValue(true);

		SetCreateProjIni = false;
	}

	event.Skip();
}


/*
 * wxEVT_COMMAND_RADIOBUTTON_SELECTED event handler for ID_PROJINID2G
 */

void D2gmain::OnProjinid2gSelected( wxCommandEvent& event )
{
	CurrProjDirLocal = !m_projinid2g->GetValue();
	CurrProjDirUsed = true;
	event.Skip();
}


/*
 * wxEVT_COMMAND_RADIOBUTTON_SELECTED event handler for ID_PROJINILOC
 */

void D2gmain::OnProjinilocSelected( wxCommandEvent& event )
{
	CurrProjDirLocal = m_projiniloc->GetValue();
	CurrProjDirUsed = true;
	event.Skip();
}


/*
 * wxEVT_COMMAND_RADIOBUTTON_SELECTED event handler for ID_PROJININO
 */

void D2gmain::OnProjininoSelected( wxCommandEvent& event )
{
	CurrProjDirUsed = !m_projinino->GetValue();
	event.Skip();
}
#endif

/*
 * wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_CREATEPROJECT
 */

void D2gmain::OnCreateprojectClick( wxCommandEvent& event )
{
	// in d2g/localsys/projects.ini, for a new ID:
	// record the projectsel name in projects.ini [Active]
	wxString wxs = m_projectselect->GetValue();
	wxString *project = new wxString(wxs);
	// record the choice of srcdirdefsel in projects.ini
	CurrSourceDir = new wxString(m_srcdirselect->GetPath());
	// record the default srcmapdefsel file in projects.ini
	CurrSourceMap = FileFromPath(new wxString(m_srcmapselect->GetPath()));
	//UMXL = CurrSourceMap->Matches(_("*.mx?"));
	// record the selected formatsel in projects.ini
	CurrFormat = new wxString(m_formatselect->GetStringSelection());
	// record the selected outdirdefsel in projects.ini
	CurrOutputDir = new wxString(m_outdirselect->GetPath());
	CurrProjDirLocal = ProjDirLocal;

	// write the starting ini to outdirdefsel
	if (!project
	 || project->IsEmpty()) {
		m_resulttext->Clear();
		m_resulttext->AppendText(*CreateNothingText);
	}
	else {
		CurrProject = project;
		if (CurrProjectExists()) {
			m_resulttext->Clear();
			m_resulttext->AppendText(*CreateExistsText);
		}
		else
			CreateProject();
	}

  event.Skip();
}


/*
 * wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_INICONFIG
 */

void D2gmain::OnIniconfigClick( wxCommandEvent& event )
{
	RunConfigWizard(CreateFormat);
	wxUnusedVar(event);
	//event.Skip();
}


/*
 * wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_CREATECM
 */

void D2gmain::OnCreatecmClick( wxCommandEvent& event )
{
	LaunchCM();
	event.Skip();
}


/*
 * wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_EDITINI
 */

void D2gmain::OnEditiniClick( wxCommandEvent& event )
{
	EditIniFile();
	wxUnusedVar(event);
	//event.Skip();
}



// modify dialog functions


/*
 * wxEVT_INIT_DIALOG event handler for ID_MODIFYPANEL
 */

void D2gmain::OnInitModifyDialog( wxInitDialogEvent& event )
{

	event.Skip();
}



/*
 * wxEVT_COMMAND_CHOICE_SELECTED event handler for ID_MPROJECTSEL
 */

void D2gmain::OnMprojectselSelected( wxCommandEvent& event )
{
	// set the projectsel name for projects.ini
	// get ID and set other windows,
	
	CurrProject = new wxString(m_mprojectselect->GetStringSelection());
  GetCurrProjectID();
  GetProjectInfo();
	SetModifyProjectControls();
	SetDeleteProjectControls();
	SetRunProjectControls();
	m_meditini->Disable();
	m_modifycm->Disable();
	m_miniconfig->Disable();

	wxUnusedVar(event);
}


/*
 * wxEVT_UPDATE_UI event handler for ID_MPROJECTSEL
 */

void D2gmain::OnMprojectselUpdate( wxUpdateUIEvent& event )
{
	// set list of projects from project.ini [Active]
	// set last one selected in [Projects]LastActive as current one
	if (ActiveProjects
	 && ActiveMProjectsChanged) {
		m_mprojectselect->Clear();
		m_mprojectselect->Append(*ActiveProjects);
		ActiveMProjectsChanged = false;
	}

	// set last one selected in [Projects]LastActive as current one
	if (CurrMProjectChanged
	 && CurrProject) {
		m_mprojectselect->SetStringSelection(*CurrProject);
		GetCurrProjectID();
		GetProjectInfo();
		SetModifyProjectControls();
		SetDeleteProjectControls();
		SetRunProjectControls();
		CurrMProjectChanged = false;
	}

  event.Skip();
}


/*
 * wxEVT_DIRPICKER_CHANGED event handler for ID_MSRCDIRDEFSEL
 */

void D2gmain::OnMsrcdirselectDirPickerChanged( wxFileDirPickerEvent& event )
{
	// set the srcmapsel to look in this dir
	// set the choice of srcdirdefsel for projects.ini
	CurrSourceDir = new wxString(m_msrcdirselect->GetPath());
  m_msrcmapselect->SetPath(*MakeFullPath(CurrSourceDir, CurrSourceMap));
  event.Skip();
}


/*
 * wxEVT_FILEPICKER_CHANGED event handler for ID_MSRCMAPDEFSEL
 */

void D2gmain::OnMsrcmapdefselFilePickerChanged( wxFileDirPickerEvent& event )
{
	// if dir different from msrcmapsel, change msrcmapsel
	// set the default srcmapdefsel file for projects.ini
	CurrSourceDir = DirFromPath(new wxString(m_msrcmapselect->GetPath()));
  m_msrcdirselect->SetPath(*CurrSourceDir);
  event.Skip();
}


/*
 * wxEVT_COMMAND_CHOICE_SELECTED event handler for ID_MFORMATSEL
 */

void D2gmain::OnMformatselSelected( wxCommandEvent& event )
{
	// set the selected formatsel for projects.ini
	CurrFormat = new wxString(m_mformatselect->GetStringSelection());
  event.Skip();
}


/*
 * wxEVT_UPDATE_UI event handler for ID_MFORMATSEL
 */

void D2gmain::OnMformatselUpdate( wxUpdateUIEvent& event )
{
	// set the list of available formats from d2gdat.ini [Formats]
	// set the format for the last active project as default
	if (MFormatsChanged) {
		m_mformatselect->Clear();
		m_mformatselect->Append(*OutputFormats);
		MFormatsChanged = false;
	}

	// set last one selected in [Projects]LastActive as current one
	if (MCurrFormatChanged
	 && CurrFormat) {
		MCurrFormatChanged = false;
		m_mformatselect->SetStringSelection(*CurrFormat);
	}

  event.Skip();
}


/*
 * wxEVT_DIRPICKER_CHANGED event handler for ID_MOUTDIRSEL
 */

void D2gmain::OnMoutdirselectDirPickerChanged( wxFileDirPickerEvent& event )
{
	// set the selected outdirdefsel for projects.ini
	CurrOutputDir = new wxString(m_moutdirselect->GetPath());
	SetCurrProjDirLoc(CurrOutputDir);
  event.Skip();
}

#if 0
/*
 * wxEVT_COMMAND_RADIOBUTTON_SELECTED event handler for ID_MPROJINID2G
 */

void D2gmain::OnMprojinid2gSelected( wxCommandEvent& event )
{
	CurrProjDirUsed = true;
	CurrProjDirLocal = !m_mprojinid2g->GetValue();
	event.Skip();
}


/*
 * wxEVT_COMMAND_RADIOBUTTON_SELECTED event handler for ID_MPROJINILOC
 */

void D2gmain::OnMprojinilocSelected( wxCommandEvent& event )
{
	CurrProjDirUsed = true;
	CurrProjDirLocal = m_mprojiniloc->GetValue();
	event.Skip();
}


/*
 * wxEVT_COMMAND_RADIOBUTTON_SELECTED event handler for ID_MPROJININO
 */

void D2gmain::OnMprojininoSelected( wxCommandEvent& event )
{
	CurrProjDirUsed = !m_mprojinino->GetValue();
	event.Skip();
}
#endif

/*
 * wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_MODIFYPROJECT
 */

void D2gmain::OnModifyprojectClick( wxCommandEvent& event )
{
	// in d2g/localsys/projects.ini, for the current ID:
	// change the projectsel name in projects.ini [Active] or [Deleted]
	CurrProject = new wxString(m_mprojectselect->GetStringSelection());
  GetCurrProjectID();
	// change the choice of srcdirdefsel in projects.ini
	CurrSourceDir = new wxString(m_msrcdirselect->GetPath());
	// change the default srcmapdefsel file in projects.ini
	CurrSourceMap = FileFromPath(new wxString(m_msrcmapselect->GetPath()));
	//UMXL = CurrSourceMap->Matches(_("*.mx?"));
	// change the selected formatsel in projects.ini
	CurrFormat = new wxString(m_mformatselect->GetStringSelection());
	// change the selected outdirdefsel in projects.ini
	CurrOutputDir = new wxString(m_moutdirselect->GetPath());

	ModifyProject();
	ActiveRunProjectsChanged = true;

  event.Skip();
}


/*
 * wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_MINICONFIG
 */

void D2gmain::OnMiniconfigClick( wxCommandEvent& event )
{
	RunConfigWizard(CurrFormat);
	wxUnusedVar(event);
	//event.Skip();
}


/*
 * wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_MODIFYCM
 */

void D2gmain::OnModifycmClick( wxCommandEvent& event )
{
	LaunchCM();
	event.Skip();
}


/*
 * wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_MEDITINI
 */

void D2gmain::OnMeditiniClick( wxCommandEvent& event )
{
	EditIniFile();
	wxUnusedVar(event);
	//event.Skip();
}


// delete dialog functions


/*
 * wxEVT_INIT_DIALOG event handler for ID_DELETEPANEL
 */

void D2gmain::OnInitDeleteDialog( wxInitDialogEvent& event )
{

	event.Skip();
}


/*
 * wxEVT_COMMAND_CHOICE_SELECTED event handler for ID_DPROJECTSEL
 */

void D2gmain::OnDprojectselSelected( wxCommandEvent& event )
{
	// set the projectsel name for projects.ini
	// get ID and set other windows,

	CurrProject = new wxString(m_dprojectselect->GetStringSelection());
  GetCurrProjectID();
  GetProjectInfo();
	SetDeleteProjectControls();
	m_editini->Disable();
	m_createcm->Disable();
	m_iniconfig->Disable();
	m_meditini->Disable();
	m_modifycm->Disable();
	m_miniconfig->Disable();

	wxUnusedVar(event);
}


/*
 * wxEVT_UPDATE_UI event handler for ID_DPROJECTSEL
 */

void D2gmain::OnDprojectselUpdate( wxUpdateUIEvent& event )
{
	// set list of projects from project.ini [Active] or [Deleted]
	if (ActiveProjects
	 && ActiveDProjectsChanged) {
		m_dprojectselect->Clear();
		m_dprojectselect->Append(*ActiveProjects);
		ActiveDProjectsChanged = false;
	}

	// set last one selected in [Projects]Last* as current one
	if (CurrDProjectChanged
	 && CurrProject) {
		m_dprojectselect->SetStringSelection(*CurrProject);
		GetCurrProjectID();
		GetProjectInfo();
		SetDeleteProjectControls();
		CurrDProjectChanged = false;
	}

  event.Skip();
}


/*
 * wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_DELETEPROJECT
 */

void D2gmain::OnDeleteprojectClick( wxCommandEvent& event )
{
	// in d2g/localsys/projects.ini, for the current ID,
	CurrProject = new wxString(m_dprojectselect->GetStringSelection());
  GetCurrProjectID();

	// remove the projectsel name from [Active], and add it to [Deleted]
	// leave all other entries alone, to allow restore
	DeleteProject();

	event.Skip();
}


// restore dialog functions


/*
 * wxEVT_INIT_DIALOG event handler for ID_RESTOREPANEL
 */

void D2gmain::OnInitRestoreDialog( wxInitDialogEvent& event )
{

	event.Skip();
}


/*
 * wxEVT_COMMAND_CHOICE_SELECTED event handler for ID_RPROJECTSEL
 */

void D2gmain::OnRprojectselSelected( wxCommandEvent& event )
{
	// set the projectsel name for projects.ini
  // get ID and set other windows

	CurrDelProject = new wxString(m_rprojectselect->GetStringSelection());
  GetCurrDelProjectID();
  GetDelProjectInfo();
	SetRestoreProjectControls();

  //event.Skip();
	wxUnusedVar(event);
}


/*
 * wxEVT_UPDATE_UI event handler for ID_RPROJECTSEL
 */

void D2gmain::OnRprojectselUpdate( wxUpdateUIEvent& event )
{
	// set list of projects from project.ini [Active] or [Deleted]
	if (DeletedProjects
	 && DeletedProjectsChanged) {
		m_rprojectselect->Clear();
		m_rprojectselect->Append(*DeletedProjects);
		DeletedProjectsChanged = false;
	}

	// set last one selected in [Projects]Last* as current one
	if (CurrRProjectChanged
	 && CurrDelProject) {
		m_rprojectselect->SetStringSelection(*CurrDelProject);
		GetCurrDelProjectID();
		GetDelProjectInfo();
		SetRestoreProjectControls();
		CurrRProjectChanged = false;
	}

  event.Skip();
}


/*
 * wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_RESTOREPROJECT
 */

void D2gmain::OnRestoreprojectClick( wxCommandEvent& event )
{
	// in d2g/localsys/projects.ini, for the selected ID,
	CurrDelProject = new wxString(m_rprojectselect->GetStringSelection());
  GetCurrDelProjectID();

	// remove projectsel name from [Deleted] and add to [Active]
	RestoreProject();

	event.Skip();
}


// run dialog functions


/*
 * wxEVT_INIT_DIALOG event handler for ID_RUNPANEL
 */

void D2gmain::OnInitRunDialog( wxInitDialogEvent& event )
{

	event.Skip();
}


/*
 * wxEVT_COMMAND_CHOICE_SELECTED event handler for ID_RUNPROJECTSEL
 */

void D2gmain::OnRunprojectselSelected( wxCommandEvent& event )
{
	CurrProject = new wxString(m_runprojectsel->GetStringSelection());
  GetCurrProjectID();
  GetProjectInfo();
	SetRunProjectControls();
	m_runeditini->Disable();
	m_runcm->Disable();

	m_mprojectselect->Clear();
	m_mprojectselect->Append(*ActiveProjects);
	m_mprojectselect->SetStringSelection(*CurrProject);
	SetModifyProjectControls();

	m_dprojectselect->Clear();
	m_dprojectselect->Append(*ActiveProjects);
	m_dprojectselect->SetStringSelection(*CurrProject);
	SetDeleteProjectControls();

	wxUnusedVar(event);
}


/*
 * wxEVT_UPDATE_UI event handler for ID_RUNPROJECTSEL
 */

void D2gmain::OnRunprojectselUpdate( wxUpdateUIEvent& event )
{
	// set list of projects from project.ini [Active]
	// set last one selected in [Projects]LastActive as current one
	if (ActiveProjects
	 && ActiveRunProjectsChanged) {
		m_runprojectsel->Clear();
		m_runprojectsel->Append(*ActiveProjects);
		ActiveRunProjectsChanged = false;
		RunProjectChanged = true;
	}

	// set last one selected in [Projects]LastActive as current one
	if (RunProjectChanged
	 && CurrProject
	 && (GetProjInt("Active", (char *) CurrProject->c_str()) != 0)) {
		m_runprojectsel->SetStringSelection(*CurrProject);
		GetCurrProjectID();
		GetProjectInfo();
		SetRunProjectControls();
		RunProjectChanged = false;
	}

  event.Skip();
}


/*
 * wxEVT_FILEPICKER_CHANGED event handler for ID_RUNSRCMAPSEL
 */

void D2gmain::OnRunsrcmapselFilePickerChanged( wxFileDirPickerEvent& event )
{
	// if dir different from runsrcdirtext, change runsrcdirtext
	// set the default runsrcmapsel file for projects.ini
	CurrSourceMap = FileFromPath(new wxString(m_runsrcmapsel->GetPath()));
	//UMXL = CurrSourceMap->Matches(_("*.mx?"));
	CurrSourceDir = DirFromPath(new wxString(m_runsrcmapsel->GetPath()));
	m_runsrcdirtext->Clear();
	m_runsrcdirtext->AppendText(*CurrSourceDir);
  event.Skip();
}


/*
 * wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_RUNSTART
 */

void D2gmain::OnRunstartClick( wxCommandEvent& event )
{
	StartRun();
	event.Skip();
}


/*
 * wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_RUNSTOP
 */

void D2gmain::OnRunstopClick( wxCommandEvent& event )
{
	StopRun();
	event.Skip();
}


/*
 * wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_RUNCM
 */

void D2gmain::OnRuncmClick( wxCommandEvent& event )
{
	LaunchCM();
	event.Skip();
}


/*
 * wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_RUNEDITINI
 */

void D2gmain::OnRuneditiniClick( wxCommandEvent& event )
{
	EditIniFile();
	wxUnusedVar(event);
}


/*
 * wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_RUNSHOWLOG
 */

void D2gmain::OnRunshowlogClick( wxCommandEvent& event )
{
	ShowLogFile();
	wxUnusedVar(event);
}


/*
 * wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_RUNVIEWOUT
 */

void D2gmain::OnRunviewoutClick( wxCommandEvent& event )
{
	ViewOutput();
	wxUnusedVar(event);
}



// runopt dialog functions


/*
 * wxEVT_COMMAND_RADIOBUTTON_SELECTED event handler for ID_RUNWASYES
 */

void D2gmain::OnRunwasyesSelected( wxCommandEvent& event )
{
	SetRunAuto("WrapAndShip", true);
	WrapAndShip = true;
	event.Skip();
}


/*
 * wxEVT_COMMAND_RADIOBUTTON_SELECTED event handler for ID_RUNWASNO
 */

void D2gmain::OnRunwasnoSelected( wxCommandEvent& event )
{
	SetRunAuto("WrapAndShip", false);
	WrapAndShip = false;
	event.Skip();
}


/*
 * wxEVT_COMMAND_RADIOBUTTON_SELECTED event handler for ID_RUNCOMPYES
 */

void D2gmain::OnRuncompyesSelected( wxCommandEvent& event )
{
	SetRunAuto("CompileHelp", true);
	event.Skip();
}


/*
 * wxEVT_COMMAND_RADIOBUTTON_SELECTED event handler for ID_RUNCOMPNO
 */

void D2gmain::OnRuncompnoSelected( wxCommandEvent& event )
{
	SetRunAuto("CompileHelp", false);
	event.Skip();
}


/*
 * wxEVT_COMMAND_RADIOBUTTON_SELECTED event handler for ID_RUNCWINYES
 */

void D2gmain::OnRuncwinyesSelected( wxCommandEvent& event )
{
	SetRunAuto("KeepCompileWindow", true);
	event.Skip();
}


/*
 * wxEVT_COMMAND_RADIOBUTTON_SELECTED event handler for ID_RUNCWINNO
 */

void D2gmain::OnRuncwinnoSelected( wxCommandEvent& event )
{
	SetRunAuto("KeepCompileWindow", false);
	event.Skip();
}


/*
 * wxEVT_COMMAND_RADIOBUTTON_SELECTED event handler for ID_RUNFTSYES
 */

void D2gmain::OnRunftsyesSelected( wxCommandEvent& event )
{
	SetRunAuto("MakeFTS", true);
	event.Skip();
}


/*
 * wxEVT_COMMAND_RADIOBUTTON_SELECTED event handler for ID_RUNFTSNO
 */

void D2gmain::OnRunftsnoSelected( wxCommandEvent& event )
{
	SetRunAuto("MakeFTS", false);
	event.Skip();
}


/*
 * wxEVT_COMMAND_RADIOBUTTON_SELECTED event handler for ID_RUNJARYES
 */

void D2gmain::OnRunjaryesSelected( wxCommandEvent& event )
{
	SetRunAuto("MakeJar", true);
	event.Skip();
}


/*
 * wxEVT_COMMAND_RADIOBUTTON_SELECTED event handler for ID_RUNJARNO
 */

void D2gmain::OnRunjarnoSelected( wxCommandEvent& event )
{
	SetRunAuto("MakeJar", false);
	event.Skip();
}


/*
 * wxEVT_COMMAND_RADIOBUTTON_SELECTED event handler for ID_RUNARCHYES
 */

void D2gmain::OnRunarchyesSelected( wxCommandEvent& event )
{
	SetRunAuto("MakeArchive", true);
	event.Skip();
}


/*
 * wxEVT_COMMAND_RADIOBUTTON_SELECTED event handler for ID_RUNARCHNO
 */

void D2gmain::OnRunarchnoSelected( wxCommandEvent& event )
{
	SetRunAuto("MakeArchive", false);
	event.Skip();
}


/*
 * wxEVT_COMMAND_RADIOBUTTON_SELECTED event handler for ID_RUNSHIPYES
 */

void D2gmain::OnRunshipyesSelected( wxCommandEvent& event )
{
	SetRunAuto("MoveArchive", true);
	event.Skip();
}


/*
 * wxEVT_COMMAND_RADIOBUTTON_SELECTED event handler for ID_RUNSHIPNO
 */

void D2gmain::OnRunshipnoSelected( wxCommandEvent& event )
{
	SetRunAuto("MoveArchive", false);
	event.Skip();
}


/*
 * wxEVT_COMMAND_RADIOBUTTON_SELECTED event handler for ID_RUNONLYYES
 */

void D2gmain::OnRunonlyyesSelected( wxCommandEvent& event )
{
	SetRunAuto("OnlyAuto", true);
	event.Skip();
}


/*
 * wxEVT_COMMAND_RADIOBUTTON_SELECTED event handler for ID_RUNONLYNO
 */

void D2gmain::OnRunonlynoSelected( wxCommandEvent& event )
{
	SetRunAuto("OnlyAuto", false);
	event.Skip();
}



/*
 * wxEVT_COMMAND_RADIOBUTTON_SELECTED event handler for ID_RUNDCLSRC
 */

void D2gmain::OnRundclsrcSelected( wxCommandEvent& event )
{
	UseDCLSource = true;
	UseDCLOutput = false;
	SetRunAuto("UseDCLSource", UseDCLSource);
	SetRunAuto("UseDCLOutput", UseDCLOutput);
	event.Skip();
}


/*
 * wxEVT_COMMAND_RADIOBUTTON_SELECTED event handler for ID_RUNDCLOUT
 */

void D2gmain::OnRundcloutSelected( wxCommandEvent& event )
{
	UseDCLSource = false;
	UseDCLOutput = true;
	SetRunAuto("UseDCLSource", UseDCLSource);
	SetRunAuto("UseDCLOutput", UseDCLOutput);
	event.Skip();
}


/*
 * wxEVT_COMMAND_RADIOBUTTON_SELECTED event handler for ID_RUNDCLNO
 */

void D2gmain::OnRundclnoSelected( wxCommandEvent& event )
{
	UseDCLSource = false;
	UseDCLOutput = false;
	SetRunAuto("UseDCLSource", UseDCLSource);
	SetRunAuto("UseDCLOutput", UseDCLOutput);
	event.Skip();
}


/*
 * wxEVT_COMMAND_RADIOBUTTON_SELECTED event handler for ID_RUNDCLYES
 */

void D2gmain::OnRundclyesSelected( wxCommandEvent& event )
{
	UseDCLSource = true;
	UseDCLOutput = true;
	SetRunAuto("UseDCLSource", UseDCLSource);
	SetRunAuto("UseDCLOutput", UseDCLOutput);
	event.Skip();
}



// options dialog functions


/*
 * wxEVT_INIT_DIALOG event handler for ID_OPTIONSPANEL
 */

void D2gmain::OnInitOptionsDialog( wxInitDialogEvent& event )
{

	event.Skip();
}


/*
 * wxEVT_COMMAND_TEXT_ENTER event handler for ID_OPTEDITORTEXT
 */

void D2gmain::OnOpteditortextEnter( wxCommandEvent& event )
{
	m_optset->Enable();
	event.Skip();
}


/*
 * wxEVT_COMMAND_TEXT_UPDATED event handler for ID_OPTEDITORTEXT
 */

void D2gmain::OnOpteditortextTextUpdated( wxCommandEvent& event )
{
	m_optset->Enable();
	event.Skip();
}


/*
 * wxEVT_COMMAND_RADIOBUTTON_SELECTED event handler for ID_OPTSHDELYES
 */

void D2gmain::OnOptshdelyesSelected( wxCommandEvent& event )
{
	m_optset->Enable();
	event.Skip();
}


/*
 * wxEVT_COMMAND_RADIOBUTTON_SELECTED event handler for ID_OPTSHDELNO
 */

void D2gmain::OnOptshdelnoSelected( wxCommandEvent& event )
{
	m_optset->Enable();
	event.Skip();
}



/*
 * wxEVT_COMMAND_CHECKBOX_CLICKED event handler for ID_RUNSHOWLOG
 */

void D2gmain::OnOptshowlogClick( wxCommandEvent& event )
{
	m_optset->Enable();
	event.Skip();
}


/*
 * wxEVT_COMMAND_CHECKBOX_CLICKED event handler for ID_RUNSHOWCON
 */

void D2gmain::OnOptshowconClick( wxCommandEvent& event )
{
	m_optset->Enable();
	event.Skip();
}


/*
 * wxEVT_DIRPICKER_CHANGED event handler for ID_OPTNEWINIDIR
 */

void D2gmain::OnOptnewiniselDirPickerChanged( wxFileDirPickerEvent& event )
{
	m_optset->Enable();
	event.Skip();
}


/*
 * wxEVT_CHAR event handler for ID_OPTNEWINIDIR
 */

void D2gmain::OnChar( wxKeyEvent& event )
{
	m_optset->Enable();
	event.Skip();
}


/*
 * wxEVT_COMMAND_TEXT_ENTER event handler for ID_OPTLOGFILETEXT
 */

void D2gmain::OnOptlogfiletextEnter( wxCommandEvent& event )
{
	m_optset->Enable();
	event.Skip();
}


/*
 * wxEVT_COMMAND_TEXT_UPDATED event handler for ID_OPTLOGFILETEXT
 */

void D2gmain::OnOptlogfiletextTextUpdated( wxCommandEvent& event )
{
	m_optset->Enable();
	event.Skip();
}


/*
 * wxEVT_COMMAND_RADIOBUTTON_SELECTED event handler for ID_OPTPROJINID2GBUT
 */

void D2gmain::OnOptprojinid2gbutSelected( wxCommandEvent& event )
{
	m_optset->Enable();
	event.Skip();
}


/*
 * wxEVT_DIRPICKER_CHANGED event handler for ID_OPTPROJINID2GSEL
 */

void D2gmain::OnOptprojinid2gselDirPickerChanged( wxFileDirPickerEvent& event )
{
	m_optset->Enable();
	event.Skip();
}


/*
 * wxEVT_COMMAND_RADIOBUTTON_SELECTED event handler for ID_OPTPROJINILOCBUT
 */

void D2gmain::OnOptprojinilocbutSelected( wxCommandEvent& event )
{
	m_optset->Enable();
	event.Skip();
}


/*
 * wxEVT_COMMAND_TEXT_UPDATED event handler for ID_OPTPROJINILOCTEXT
 */

void D2gmain::OnOptprojiniloctextTextUpdated( wxCommandEvent& event )
{
	m_optset->Enable();
	event.Skip();
}

#if 0
/*
 * wxEVT_COMMAND_RADIOBUTTON_SELECTED event handler for ID_OPTPROJININOBUT
 */

void D2gmain::OnOptprojininobutSelected( wxCommandEvent& event )
{
	m_optset->Enable();
	event.Skip();
}
#endif

/*
 * wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_OPTSET
 */

void D2gmain::OnOptsetClick( wxCommandEvent& event )
{
	GetOptionsValues();
	m_optset->Disable();

	event.Skip();
}


/*
 * wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_OPTRESET
 */

void D2gmain::OnOptresetClick( wxCommandEvent& event )
{
	OptRest *optype = new OptRest(NULL);
	optype->ShowModal();
	int rval = optype->GetReturnCode();
	optype->Destroy();

	switch (rval) {
		case 1: // restore to last Set
			SetOptionsValues();
			break;

		case 2: // restore to start of session
			GetStartOptions();
			SetOptionsValues();
			break;

		case 3: // restore to original defaults
			GetDefOptions();
			SetOptionsValues();
			break;

		default:
			return;

	}

	m_optset->Disable();

	event.Skip();
}





// end of event handlers


