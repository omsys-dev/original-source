/////////////////////////////////////////////////////////////////////////////
// Name:        configmgr.cpp
// Purpose:     
// Author:      Jeremy H Griffith
// Modified by: 
// Created:     12/03/2012 14:13:49
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
#include "wx/bookctrl.h"
#include "wx/imaglist.h"
////@end includes

#include "wx/filedlg.h"


#include "configmgr.h"

#pragma warning( disable : 4089 4189 4710)

#include "dcglob.h"
#include "dcbase.h"
#include "dclist.h"
#include "dcnode.h"
#include "dcfile.h"
#include "dcfilr.h"
#include "dcfilw.h"
#include "dclog.h"
#include "dcfili.h"
#include "dcfilp.h"
#include "dctxfx.h"
#include "dcfmtr.h"

#include "cmdcl.h"


////@begin XPM images
////@end XPM images


/*
 * ConfigMgr type definition
 */

IMPLEMENT_DYNAMIC_CLASS( ConfigMgr, wxPropertySheetDialog )


/*
 * ConfigMgr event table definition
 */

BEGIN_EVENT_TABLE( ConfigMgr, wxPropertySheetDialog )

////@begin ConfigMgr event table entries
	EVT_INIT_DIALOG( ConfigMgr::OnInitDialog )

	EVT_RADIOBUTTON( ID_PROJNAMEBUTTON, ConfigMgr::OnProjnamebuttonSelected )

	EVT_CHOICE( ID_PROJNAMESEL, ConfigMgr::OnProjnameselSelected )

	EVT_RADIOBUTTON( ID_PROJSTARTTPLBUTTON, ConfigMgr::OnProjstarttplbuttonSelected )

	EVT_CHOICE( ID_PROJSTARTTPLSEL, ConfigMgr::OnProjstarttplselSelected )

	EVT_RADIOBUTTON( ID_FILEBROWSEBUTTON, ConfigMgr::OnFilebrowsebuttonSelected )

	EVT_TREE_SEL_CHANGED( wxID_TREECTRL, ConfigMgr::OnProjdirctrlSelChanged )

	EVT_BUTTON( ID_APPLYPROJ, ConfigMgr::OnApplyprojClick )

	EVT_BUTTON( ID_PROJHELPBUTTON, ConfigMgr::OnProjhelpbuttonClick )

	EVT_RADIOBUTTON( ID_CATINITYPEBUTTON, ConfigMgr::OnCatinitypebuttonSelected )

	EVT_CHOICE( ID_CATINICHOICE, ConfigMgr::OnCatinichoiceSelected )

	EVT_RADIOBUTTON( ID_CATSELBUTTON, ConfigMgr::OnCatselbuttonSelected )

	EVT_TREE_SEL_CHANGED( ID_SECTYPTREE, ConfigMgr::OnSectyptreeSelChanged )
	EVT_TREE_ITEM_ACTIVATED( ID_SECTYPTREE, ConfigMgr::OnSectyptreeItemActivated )

	EVT_RADIOBUTTON( ID_CATFILEBUTTON, ConfigMgr::OnCatfilebuttonSelected )

	EVT_RADIOBUTTON( ID_CATALLBUTTON, ConfigMgr::OnCatallbuttonSelected )

	EVT_TEXT( ID_PROJSTARTSECTEXTCTRL, ConfigMgr::OnProjstartsectextctrlTextUpdated )
	EVT_TEXT_ENTER( ID_PROJSTARTSECTEXTCTRL, ConfigMgr::OnProjstartsectextctrlEnter )

	EVT_TEXT( ID_PROJSTARTSETTEXTCTRL, ConfigMgr::OnProjstartsettextctrlTextUpdated )
	EVT_TEXT_ENTER( ID_PROJSTARTSETTEXTCTRL, ConfigMgr::OnProjstartsettextctrlEnter )

	EVT_BUTTON( ID_APPLYCAT, ConfigMgr::OnApplycatClick )

	EVT_BUTTON( ID_CATHELPBUTTON, ConfigMgr::OnCathelpbuttonClick )

	EVT_RADIOBUTTON( ID_SECTADDBUTTON, ConfigMgr::OnSectaddbuttonSelected )

	EVT_RADIOBUTTON( ID_SECTVNADDBUTTON, ConfigMgr::OnSectvnaddbuttonSelected )

	EVT_RADIOBUTTON( ID_SECTADDSETBUTTON, ConfigMgr::OnSectaddsetbuttonSelected )

	EVT_RADIOBUTTON( ID_SECTADDVKSETBUTTON, ConfigMgr::OnSectaddvksetbuttonSelected )

	EVT_RADIOBUTTON( ID_SECTCHANGEBUTTON, ConfigMgr::OnSectchangebuttonSelected )

	EVT_RADIOBUTTON( ID_SECTSETEDITBUTTON, ConfigMgr::OnSectseteditbuttonSelected )

	EVT_RADIOBUTTON( ID_SECTDELBUTTON, ConfigMgr::OnSectdelbuttonSelected )

	EVT_RADIOBUTTON( ID_SECTDELSETBUTTON, ConfigMgr::OnSectdelsetbuttonSelected )

	EVT_RADIOBUTTON( ID_SECTRESTBUTTON, ConfigMgr::OnSectrestbuttonSelected )

	EVT_RADIOBUTTON( ID_SECTRESTSETBUTTON, ConfigMgr::OnSectrestsetbuttonSelected )

	EVT_RADIOBUTTON( ID_SECTRENBUTTON, ConfigMgr::OnSectrenbuttonSelected )

	EVT_RADIOBUTTON( ID_SECTRENSETBUTTON, ConfigMgr::OnSectrensetbuttonSelected )

	EVT_RADIOBUTTON( ID_SECTMOVBUTTON, ConfigMgr::OnSectmovbuttonSelected )

	EVT_RADIOBUTTON( ID_SECTMOVFBUTTON, ConfigMgr::OnSectmovfbuttonSelected )

	EVT_RADIOBUTTON( ID_SECTMOVSETBUTTON, ConfigMgr::OnSectmovsetbuttonSelected )

	EVT_RADIOBUTTON( ID_SECTMOVSETFBUTTON, ConfigMgr::OnSectmovsetfbuttonSelected )

	EVT_RADIOBUTTON( ID_SECTCPYBUTTON, ConfigMgr::OnSectcpybuttonSelected )

	EVT_RADIOBUTTON( ID_SECTCPYSETBUTTON, ConfigMgr::OnSectcpysetbuttonSelected )

	EVT_RADIOBUTTON( ID_SECTDUPBUTTON, ConfigMgr::OnSectdupbuttonSelected )

	EVT_RADIOBUTTON( ID_SECTDUPSETBUTTON, ConfigMgr::OnSectdupsetbuttonSelected )

	EVT_BUTTON( ID_APPLYACT, ConfigMgr::OnApplyactClick )

	EVT_BUTTON( ID_ACTHELPBUTTON, ConfigMgr::OnActhelpbuttonClick )

	EVT_LIST_ITEM_SELECTED( ID_SECTCTRL, ConfigMgr::OnSectctrlSelected )
	EVT_LIST_ITEM_ACTIVATED( ID_SECTCTRL, ConfigMgr::OnSectctrlItemActivated )

	EVT_TEXT( ID_SECTADDTEXT, ConfigMgr::OnSectaddtextTextUpdated )
	EVT_TEXT_ENTER( ID_SECTADDTEXT, ConfigMgr::OnSectaddtextEnter )

	EVT_TEXT( ID_SECTVNRENTEXT, ConfigMgr::OnSectvnrentextTextUpdated )
	EVT_TEXT_ENTER( ID_SECTVNRENTEXT, ConfigMgr::OnSectvnrentextEnter )

	EVT_CHECKBOX( ID_SECTFORMALIAS, ConfigMgr::OnSectformaliasClick )

	EVT_BUTTON( ID_APPLYSECT, ConfigMgr::OnApplysectClick )

	EVT_BUTTON( ID_SECTHELPBUTTON, ConfigMgr::OnSecthelpbuttonClick )

	EVT_LIST_ITEM_SELECTED( ID_SETCTRL, ConfigMgr::OnSetctrlSelected )
	EVT_LIST_ITEM_ACTIVATED( ID_SETCTRL, ConfigMgr::OnSetctrlItemActivated )

	EVT_BUTTON( ID_SETVARKEYBUTTON, ConfigMgr::OnSetvarkeybuttonClick )

	EVT_TEXT( ID_SETVKADDTEXT, ConfigMgr::OnSetvkaddtextTextUpdated )
	EVT_TEXT_ENTER( ID_SETVKADDTEXT, ConfigMgr::OnSetvkaddtextEnter )

	EVT_TEXT( ID_SETVKRENTEXT, ConfigMgr::OnSetvkrentextTextUpdated )
	EVT_TEXT_ENTER( ID_SETVKRENTEXT, ConfigMgr::OnSetvkrentextEnter )

	EVT_BUTTON( ID_APPLYSET, ConfigMgr::OnApplysetClick )

	EVT_BUTTON( ID_SETHELPBUTTON, ConfigMgr::OnSethelpbuttonClick )

	EVT_LIST_ITEM_SELECTED( ID_SELINICTRL, ConfigMgr::OnSelinictrlSelected )
	EVT_LIST_ITEM_DESELECTED( ID_SELINICTRL, ConfigMgr::OnSelinictrlDeselected )
	EVT_LIST_ITEM_ACTIVATED( ID_SELINICTRL, ConfigMgr::OnSelinictrlItemActivated )

	EVT_BUTTON( ID_APPLYVAL, ConfigMgr::OnApplyvalClick )

	EVT_BUTTON( ID_VALHELPBUTTON, ConfigMgr::OnValhelpbuttonClick )

	EVT_TEXT_ENTER( ID_VSELVALTEXT, ConfigMgr::OnVselvaltextEnter )

	EVT_BUTTON( ID_VSELVALBUTTON, ConfigMgr::OnVselvalbuttonClick )

	EVT_LIST_ITEM_SELECTED( ID_SELVALCTRL, ConfigMgr::OnSelvalctrlSelected )
	EVT_LIST_ITEM_DESELECTED( ID_SELVALCTRL, ConfigMgr::OnSelvalctrlDeselected )
	EVT_LIST_BEGIN_LABEL_EDIT( ID_SELVALCTRL, ConfigMgr::OnSelvalctrlBeginLabelEdit )
	EVT_LIST_END_LABEL_EDIT( ID_SELVALCTRL, ConfigMgr::OnSelvalctrlEndLabelEdit )
	EVT_LIST_ITEM_ACTIVATED( ID_SELVALCTRL, ConfigMgr::OnSelvalctrlItemActivated )

	EVT_TEXT_ENTER( ID_VSELMACTEXT, ConfigMgr::OnVselmactextEnter )

	EVT_TEXT_ENTER( ID_VSELONETEXT, ConfigMgr::OnVselonetextEnter )

	EVT_BUTTON( ID_VSELUPBUTTON, ConfigMgr::OnVselupbuttonClick )

	EVT_BUTTON( ID_VSELDNBUTTON, ConfigMgr::OnVseldnbuttonClick )

	EVT_BUTTON( ID_APPLYVSEL, ConfigMgr::OnApplyvselClick )

	EVT_BUTTON( ID_VSELHELPBUTTON, ConfigMgr::OnVselhelpbuttonClick )

	EVT_RADIOBUTTON( ID_OPTSYSYES, ConfigMgr::OnOptsysyesSelected )

	EVT_RADIOBUTTON( ID_OPTSYSNO, ConfigMgr::OnOptsysnoSelected )

	EVT_COLOURPICKER_CHANGED( ID_OPTCOLORSECTCTRL, ConfigMgr::OnOptcolorsectctrlColourPickerChanged )

	EVT_COLOURPICKER_CHANGED( ID_OPTCOLORSETCTRL, ConfigMgr::OnOptcolorsetctrlColourPickerChanged )

	EVT_COLOURPICKER_CHANGED( ID_OPTCOLORVARCTRL, ConfigMgr::OnOptcolorvarctrlColourPickerChanged )

	EVT_COLOURPICKER_CHANGED( ID_OPTCOLORDEFCTRL, ConfigMgr::OnOptcolordefctrlColourPickerChanged )

	EVT_COLOURPICKER_CHANGED( ID_OPTCOLORSYSCTRL, ConfigMgr::OnOptcolorsysctrlColourPickerChanged )

	EVT_CHECKBOX( ID_OPTADDINIHIST, ConfigMgr::OnOptaddinihistClick )

	EVT_TEXT( ID_OPTUSERNAMETEXT, ConfigMgr::OnOptusernametextTextUpdated )
	EVT_TEXT_ENTER( ID_OPTUSERNAMETEXT, ConfigMgr::OnOptusernametextEnter )

	EVT_COMBOBOX( ID_OPTBROWSERCTRL, ConfigMgr::OnOptbrowserctrlSelected )
	EVT_TEXT_ENTER( ID_OPTBROWSERCTRL, ConfigMgr::OnOptbrowserctrlEnter )

	EVT_BUTTON( ID_OPTHELPBUTTON, ConfigMgr::OnOpthelpbuttonClick )

////@end ConfigMgr event table entries

END_EVENT_TABLE()


/*
 * ConfigMgr constructors
 */

ConfigMgr::ConfigMgr()
{
  Init();
}

ConfigMgr::ConfigMgr( wxWindow* parent, wxWindowID id, const wxString& caption, const wxPoint& pos, const wxSize& size, long style )
{
	Init();

	wxUnusedVar(pos);
	wxUnusedVar(size);
	Create(parent, id, caption, *InitPos, *InitSize, style);
}


/*
 * ConfigMgr creator
 */

bool ConfigMgr::Create( wxWindow* parent, wxWindowID id, const wxString& caption, const wxPoint& pos, const wxSize& size, long style )
{
  SetExtraStyle(wxWS_EX_VALIDATE_RECURSIVELY|wxWS_EX_BLOCK_EVENTS);
  SetSheetStyle(wxPROPSHEET_DEFAULT);
  wxPropertySheetDialog::Create( parent, id, caption, pos, size, style );

  CreateControls();
  SetIcon(GetIconResource(wxT("favicon.xpm")));

	if ((InitPos->x == -1)
	 && (InitPos->y == -1)
	 && (InitSize->GetWidth() == -1)
	 && (InitSize->GetHeight() == -1))
		LayoutDialog();  // remove to prevent centering and shrinkwrapping

  return true;
}


/*
 * ConfigMgr destructor
 */

ConfigMgr::~ConfigMgr()
{
	EndingUpdate();

////@begin ConfigMgr destruction
////@end ConfigMgr destruction
}


/*
 * Member initialisation
 */

void ConfigMgr::Init()
{
////@begin ConfigMgr member initialisation
	m_filepanel = NULL;
	m_projnamebutton = NULL;
	m_projname = NULL;
	m_projnamesel = NULL;
	m_projstarttplbutton = NULL;
	m_projstarttpl = NULL;
	m_projstarttplsel = NULL;
	m_filebrowselbutton = NULL;
	m_filebrowse = NULL;
	m_projdirctrl = NULL;
	m_projstartfile = NULL;
	m_projstarttext = NULL;
	m_projresult = NULL;
	m_applyproj = NULL;
	m_projhelpbutton = NULL;
	m_catpanel = NULL;
	m_catstartfile = NULL;
	m_catstarttext = NULL;
	m_catinitypebutton = NULL;
	m_catinichoice = NULL;
	m_catselbutton = NULL;
	m_sectyptree = NULL;
	m_catfilebutton = NULL;
	m_catallbutton = NULL;
	m_projstart = NULL;
	m_selstartsect = NULL;
	m_projstartsecttxt = NULL;
	m_selstartset = NULL;
	m_projstartsettxt = NULL;
	m_projstartwarn = NULL;
	m_catresult = NULL;
	m_applycat = NULL;
	m_cathelpbutton = NULL;
	m_actpanel = NULL;
	m_actstartfile = NULL;
	m_actstarttext = NULL;
	m_sectaddbutton = NULL;
	m_sectvnaddbutton = NULL;
	m_sectaddsetbutton = NULL;
	m_sectaddvksetbutton = NULL;
	m_sectchangebutton = NULL;
	m_sectseteditbutton = NULL;
	m_sectdelbutton = NULL;
	m_sectdelsetbutton = NULL;
	m_sectrestbutton = NULL;
	m_sectrestsetbutton = NULL;
	m_sectrenbutton = NULL;
	m_sectrensetbutton = NULL;
	m_sectmovbutton = NULL;
	m_sectmovfbutton = NULL;
	m_sectmovsetbutton = NULL;
	m_sectmovsetfbutton = NULL;
	m_sectcpybutton = NULL;
	m_sectcpysetfbutton = NULL;
	m_sectdupbutton = NULL;
	m_sectdupsetbutton = NULL;
	m_actresult = NULL;
	m_applyact = NULL;
	m_acthelpbutton = NULL;
	m_sectionpanel = NULL;
	m_sectstartfile = NULL;
	m_sectstarttext = NULL;
	m_sectcat = NULL;
	m_sectcattext = NULL;
	m_sectact = NULL;
	m_sectacttext = NULL;
	m_selsect = NULL;
	m_sectctrl = NULL;
	m_secthelptext = NULL;
	m_sectvarname = NULL;
	m_sectvnadd = NULL;
	m_sectvnaddtext = NULL;
	m_sectvnren = NULL;
	m_sectvnrentext = NULL;
	m_sectformalias = NULL;
	m_sectresult = NULL;
	m_applysect = NULL;
	m_secthelpbutton = NULL;
	m_settingpanel = NULL;
	m_setstartfile = NULL;
	m_setstarttext = NULL;
	m_setcat = NULL;
	m_setcattext = NULL;
	m_setact = NULL;
	m_setacttext = NULL;
	m_setsect = NULL;
	m_setsecttext = NULL;
	m_setsecthelp = NULL;
	m_setsecthelptext = NULL;
	m_selset = NULL;
	m_setlistctrl = NULL;
	m_macrotext = NULL;
	m_sethelptext = NULL;
	m_setvarkeysizer = NULL;
	m_setvarkey = NULL;
	m_setvarkeybutton = NULL;
	m_setvkadd = NULL;
	m_setvkaddtext = NULL;
	m_setvkren = NULL;
	m_setvkrentext = NULL;
	m_setresult = NULL;
	m_applyset = NULL;
	m_sethelpbutton = NULL;
	m_valpanel = NULL;
	m_valpanelsizer = NULL;
	m_valstartfile = NULL;
	m_valstarttext = NULL;
	m_valcat = NULL;
	m_valcattext = NULL;
	m_valact = NULL;
	m_valacttext = NULL;
	m_valsect = NULL;
	m_valsecttext = NULL;
	m_valsecthelp = NULL;
	m_valsecthelptext = NULL;
	m_valsetsizer = NULL;
	m_valset = NULL;
	m_valsettext = NULL;
	m_valsethelpsizer = NULL;
	m_valsethelp = NULL;
	m_valsethelptext = NULL;
	m_selini = NULL;
	m_selinictrl = NULL;
	m_valhelptext = NULL;
	m_valcurrvalsizer = NULL;
	m_currval = NULL;
	m_currvaltext = NULL;
	m_valresult = NULL;
	m_applyval = NULL;
	m_valhelpbutton = NULL;
	m_vselpanel = NULL;
	m_vselpanelsizer = NULL;
	m_vselstartfile = NULL;
	m_vselstarttext = NULL;
	m_vselcat = NULL;
	m_vselcattext = NULL;
	m_vselact = NULL;
	m_vselacttext = NULL;
	m_vselsect = NULL;
	m_vselsecttext = NULL;
	m_vselsecthelp = NULL;
	m_vselsecthelptext = NULL;
	m_vselsetsizer = NULL;
	m_vselset = NULL;
	m_vselsettext = NULL;
	m_vselsethelpsizer = NULL;
	m_vselsethelp = NULL;
	m_vselsethelptext = NULL;
	m_vselini = NULL;
	m_vselinitext = NULL;
	m_vseldestsizer = NULL;
	m_vseldest = NULL;
	m_vseldesttext = NULL;
	m_vselrensizer = NULL;
	m_vselren = NULL;
	m_vselrentext = NULL;
	m_vselvalsizer = NULL;
	m_vselname = NULL;
	m_vselval = NULL;
	m_vselvaltext = NULL;
	m_vselvalhelpsizer = NULL;
	m_vselvalhelpvertsizer = NULL;
	m_vselvalhelp = NULL;
	m_vselnamehelp = NULL;
	m_vselvalbutton = NULL;
	m_vselvalhelptext = NULL;
	m_vselctrlsizer = NULL;
	m_selval = NULL;
	m_vselfix = NULL;
	m_vseldel = NULL;
	m_vselmov = NULL;
	m_selvalctrl = NULL;
	m_vselmacadd = NULL;
	m_vselmacedit = NULL;
	m_vselmactext = NULL;
	m_vselone = NULL;
	m_vselonetext = NULL;
	m_vselcurrvalsizer = NULL;
	m_vselcurrval = NULL;
	m_vselcurrvaltext = NULL;
	m_vselmovsizer = NULL;
	m_vselupbutton = NULL;
	m_vseldnbutton = NULL;
	m_vselresult = NULL;
	m_applyvsel = NULL;
	m_vselhelpbutton = NULL;
	m_optpanel = NULL;
	m_optsyssizer = NULL;
	m_optsys = NULL;
	m_optsysyes = NULL;
	m_optsysno = NULL;
	m_optcolors = NULL;
	m_optcolorsect = NULL;
	m_optcolorsecttext = NULL;
	m_optcolorsectctrl = NULL;
	m_optcolorset = NULL;
	m_optcolorsettext = NULL;
	m_optcolorsetctrl = NULL;
	m_optcolorvar = NULL;
	m_optcolorvartext = NULL;
	m_optcolorvarctrl = NULL;
	m_optcolordef = NULL;
	m_optcolordeftext = NULL;
	m_optcolordefctrl = NULL;
	m_optcolorsys = NULL;
	m_optcolorsystext = NULL;
	m_optcolorsysctrl = NULL;
	m_optaddinihist = NULL;
	m_optusername = NULL;
	m_optusernametext = NULL;
	m_optbrowser = NULL;
	m_optbrowserctrl = NULL;
	m_optlhelpbutton = NULL;
////@end ConfigMgr member initialisation

	// init for cmdcl variables
	InitializeDCL();
}


/*
 * Control creation for ConfigMgr
 */

void ConfigMgr::CreateControls()
{    
////@begin ConfigMgr content construction
	ConfigMgr* itemPropertySheetDialog1 = this;

	m_filepanel = new wxPanel( GetBookCtrl(), ID_FILEPANEL, wxDefaultPosition, wxDefaultSize, wxSUNKEN_BORDER|wxTAB_TRAVERSAL );
	wxBoxSizer* itemBoxSizer3 = new wxBoxSizer(wxVERTICAL);
	m_filepanel->SetSizer(itemBoxSizer3);

	wxBoxSizer* itemBoxSizer4 = new wxBoxSizer(wxHORIZONTAL);
	itemBoxSizer3->Add(itemBoxSizer4, 0, wxGROW|wxALL, 5);
	wxBoxSizer* itemBoxSizer5 = new wxBoxSizer(wxVERTICAL);
	itemBoxSizer4->Add(itemBoxSizer5, 10, wxALIGN_CENTER_VERTICAL|wxALL, 5);
	wxBoxSizer* itemBoxSizer6 = new wxBoxSizer(wxHORIZONTAL);
	itemBoxSizer5->Add(itemBoxSizer6, 0, wxGROW|wxALL, 5);
	wxStaticText* itemStaticText7 = new wxStaticText( m_filepanel, wxID_PROJSELCONFIG, _("Select starting .ini configuration file to use: "), wxDefaultPosition, wxDefaultSize, 0 );
	itemBoxSizer6->Add(itemStaticText7, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

	wxBoxSizer* itemBoxSizer8 = new wxBoxSizer(wxHORIZONTAL);
	itemBoxSizer5->Add(itemBoxSizer8, 0, wxGROW|wxLEFT|wxRIGHT|wxTOP, 5);
	m_projnamebutton = new wxRadioButton( m_filepanel, ID_PROJNAMEBUTTON, _("Select project: "), wxDefaultPosition, wxDefaultSize, wxRB_GROUP );
	m_projnamebutton->SetValue(true);
	itemBoxSizer8->Add(m_projnamebutton, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

	m_projname = new wxStaticText( m_filepanel, wxID_PROJNAME, _("Select project: "), wxDefaultPosition, wxDefaultSize, 0 );
	m_projname->Show(false);
	m_projname->Enable(false);
	itemBoxSizer8->Add(m_projname, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

	wxArrayString m_projnameselStrings;
	m_projnameselStrings.Add(_("Current projects:"));
	m_projnamesel = new wxChoice( m_filepanel, ID_PROJNAMESEL, wxDefaultPosition, wxDefaultSize, m_projnameselStrings, 0 );
	m_projnamesel->SetStringSelection(_("Current projects:"));
	itemBoxSizer8->Add(m_projnamesel, 20, wxALIGN_CENTER_VERTICAL|wxALL, 5);

	wxBoxSizer* itemBoxSizer12 = new wxBoxSizer(wxHORIZONTAL);
	itemBoxSizer5->Add(itemBoxSizer12, 0, wxGROW|wxLEFT|wxRIGHT|wxBOTTOM, 5);
	m_projstarttplbutton = new wxRadioButton( m_filepanel, ID_PROJSTARTTPLBUTTON, _("Or start template: "), wxDefaultPosition, wxDefaultSize, 0 );
	m_projstarttplbutton->SetValue(false);
	itemBoxSizer12->Add(m_projstarttplbutton, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

	m_projstarttpl = new wxStaticText( m_filepanel, wxID_PROJSELSTARTTPL, _("Select start template: "), wxDefaultPosition, wxDefaultSize, 0 );
	m_projstarttpl->Show(false);
	m_projstarttpl->Enable(false);
	itemBoxSizer12->Add(m_projstarttpl, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

	wxArrayString m_projstarttplselStrings;
	m_projstarttplselStrings.Add(_("Start templates for: "));
	m_projstarttplsel = new wxChoice( m_filepanel, ID_PROJSTARTTPLSEL, wxDefaultPosition, wxDefaultSize, m_projstarttplselStrings, 0 );
	m_projstarttplsel->SetStringSelection(_("Start templates for: "));
	itemBoxSizer12->Add(m_projstarttplsel, 20, wxALIGN_CENTER_VERTICAL|wxALL, 5);

	wxBoxSizer* itemBoxSizer16 = new wxBoxSizer(wxHORIZONTAL);
	itemBoxSizer5->Add(itemBoxSizer16, 0, wxGROW|wxLEFT|wxRIGHT|wxBOTTOM, 5);
	m_filebrowselbutton = new wxRadioButton( m_filepanel, ID_FILEBROWSEBUTTON, _("Or browse for starting ini: "), wxDefaultPosition, wxDefaultSize, 0 );
	m_filebrowselbutton->SetValue(false);
	itemBoxSizer16->Add(m_filebrowselbutton, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

	m_filebrowse = new wxStaticText( m_filepanel, wxID_FILEBROWSE, _("Browse for starting ini: "), wxDefaultPosition, wxDefaultSize, 0 );
	m_filebrowse->Show(false);
	m_filebrowse->Enable(false);
	itemBoxSizer16->Add(m_filebrowse, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

	m_projdirctrl = new wxGenericDirCtrl( m_filepanel, ID_PROJDIRCTRL, wxEmptyString, wxDefaultPosition, wxSize(100, 100), 0, _T("Ini files (*.ini)|*.ini"), 0 );
	m_projdirctrl->Enable(false);
	itemBoxSizer3->Add(m_projdirctrl, 50, wxGROW|wxALL, 5);

	wxBoxSizer* itemBoxSizer20 = new wxBoxSizer(wxHORIZONTAL);
	itemBoxSizer3->Add(itemBoxSizer20, 0, wxGROW|wxALL, 5);
	m_projstartfile = new wxStaticText( m_filepanel, wxID_PROJSTARTFILE, _("Selected  start file is: "), wxDefaultPosition, wxDefaultSize, 0 );
	itemBoxSizer20->Add(m_projstartfile, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

	m_projstarttext = new wxTextCtrl( m_filepanel, ID_PROJSTARTTEXT, _("Starting .ini configuration file not selected yet."), wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER|wxTE_READONLY );
	itemBoxSizer20->Add(m_projstarttext, 20, wxALIGN_CENTER_VERTICAL|wxALL, 5);

	wxBoxSizer* itemBoxSizer23 = new wxBoxSizer(wxVERTICAL);
	itemBoxSizer3->Add(itemBoxSizer23, 20, wxGROW|wxALL, 5);
	wxBoxSizer* itemBoxSizer24 = new wxBoxSizer(wxHORIZONTAL);
	itemBoxSizer23->Add(itemBoxSizer24, 20, wxGROW|wxALL, 5);
	m_projresult = new wxTextCtrl( m_filepanel, ID_PROJRESULT, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER|wxTE_READONLY );
	itemBoxSizer24->Add(m_projresult, 20, wxALIGN_BOTTOM|wxALL, 5);

	itemBoxSizer24->Add(5, 5, 0, wxALIGN_BOTTOM|wxALL, 5);

	m_applyproj = new wxButton( m_filepanel, ID_APPLYPROJ, _("Apply file"), wxDefaultPosition, wxDefaultSize, 0 );
	itemBoxSizer24->Add(m_applyproj, 0, wxALIGN_BOTTOM|wxALL, 5);

	m_projhelpbutton = new wxButton( m_filepanel, ID_PROJHELPBUTTON, _("?"), wxDefaultPosition, wxSize(20, -1), 0 );
	itemBoxSizer24->Add(m_projhelpbutton, 0, wxALIGN_BOTTOM|wxALL, 5);

	GetBookCtrl()->AddPage(m_filepanel, _("Start"));

	m_catpanel = new wxPanel( GetBookCtrl(), ID_CATPANEL, wxDefaultPosition, wxDefaultSize, wxSUNKEN_BORDER|wxTAB_TRAVERSAL );
	wxBoxSizer* itemBoxSizer30 = new wxBoxSizer(wxVERTICAL);
	m_catpanel->SetSizer(itemBoxSizer30);

	wxBoxSizer* itemBoxSizer31 = new wxBoxSizer(wxHORIZONTAL);
	itemBoxSizer30->Add(itemBoxSizer31, 0, wxGROW|wxALL, 5);
	m_catstartfile = new wxStaticText( m_catpanel, wxID_CATSTARTFILE, _("Selected start file: "), wxDefaultPosition, wxDefaultSize, 0 );
	itemBoxSizer31->Add(m_catstartfile, 0, wxALIGN_CENTER_VERTICAL|wxLEFT|wxRIGHT|wxTOP, 5);

	m_catstarttext = new wxTextCtrl( m_catpanel, ID_CATSTARTTEXT, _("Starting .ini configuration file not selected yet."), wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER|wxTE_READONLY );
	itemBoxSizer31->Add(m_catstarttext, 20, wxALIGN_CENTER_VERTICAL|wxLEFT|wxRIGHT|wxTOP, 5);

	m_catinitypebutton = new wxRadioButton( m_catpanel, ID_CATINITYPEBUTTON, _("Select type of .ini file to configure:"), wxDefaultPosition, wxDefaultSize, wxRB_GROUP );
	m_catinitypebutton->SetValue(true);
	itemBoxSizer30->Add(m_catinitypebutton, 0, wxALIGN_LEFT|wxLEFT|wxRIGHT|wxTOP, 5);

	wxStaticText* itemStaticText35 = new wxStaticText( m_catpanel, wxID_PROJSELCAT, _("Select category:"), wxDefaultPosition, wxDefaultSize, 0 );
	itemStaticText35->Show(false);
	itemBoxSizer30->Add(itemStaticText35, 0, wxALIGN_LEFT|wxALL, 5);

	wxBoxSizer* itemBoxSizer36 = new wxBoxSizer(wxHORIZONTAL);
	itemBoxSizer30->Add(itemBoxSizer36, 0, wxALIGN_LEFT|wxALL, 5);
	itemBoxSizer36->Add(30, 5, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

	wxArrayString m_catinichoiceStrings;
	m_catinichoiceStrings.Add(_("Configuration"));
#ifndef _M2GCM
	m_catinichoiceStrings.Add(_("Text Formats"));
	m_catinichoiceStrings.Add(_("Table Formats"));
	m_catinichoiceStrings.Add(_("Page Formats"));
	m_catinichoiceStrings.Add(_("Subformats"));
	m_catinichoiceStrings.Add(_("Languages"));
#endif
	m_catinichoiceStrings.Add(_("Macros"));
	m_catinichoiceStrings.Add(_("Content Models"));
	m_catinichoice = new wxChoice( m_catpanel, ID_CATINICHOICE, wxDefaultPosition, wxSize(150, -1), m_catinichoiceStrings, 0 );
	m_catinichoice->SetStringSelection(_("Configuration"));
	itemBoxSizer36->Add(m_catinichoice, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

	m_catselbutton = new wxRadioButton( m_catpanel, ID_CATSELBUTTON, _("Select category:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_catselbutton->SetValue(false);
	itemBoxSizer30->Add(m_catselbutton, 0, wxALIGN_LEFT|wxALL, 5);

	m_sectyptree = new wxTreeCtrl( m_catpanel, ID_SECTYPTREE, wxDefaultPosition, wxSize(300, 250), wxTR_HAS_BUTTONS |wxTR_LINES_AT_ROOT|wxTR_HIDE_ROOT|wxTR_SINGLE );
	itemBoxSizer30->Add(m_sectyptree, 0, wxGROW|wxALL, 5);

	wxBoxSizer* itemBoxSizer41 = new wxBoxSizer(wxHORIZONTAL);
	itemBoxSizer30->Add(itemBoxSizer41, 0, wxGROW|wxALL, 5);
	wxBoxSizer* itemBoxSizer42 = new wxBoxSizer(wxVERTICAL);
	itemBoxSizer41->Add(itemBoxSizer42, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
	m_catfilebutton = new wxRadioButton( m_catpanel, ID_CATFILEBUTTON, _("Show only current sections and settings; easiest when only changing or deleting\n"), wxDefaultPosition, wxDefaultSize, wxRB_GROUP );
	m_catfilebutton->SetValue(true);
	m_catfilebutton->Show(false);
	m_catfilebutton->Enable(false);
	itemBoxSizer42->Add(m_catfilebutton, 0, wxALIGN_LEFT|wxALL, 5);

	m_catallbutton = new wxRadioButton( m_catpanel, ID_CATALLBUTTON, _("Show all available sections and settings; needed when adding sections or settings"), wxDefaultPosition, wxDefaultSize, 0 );
	m_catallbutton->SetValue(false);
	m_catallbutton->Show(false);
	m_catallbutton->Enable(false);
	itemBoxSizer42->Add(m_catallbutton, 0, wxALIGN_LEFT|wxALL, 5);

	wxBoxSizer* itemBoxSizer45 = new wxBoxSizer(wxHORIZONTAL);
	itemBoxSizer30->Add(itemBoxSizer45, 0, wxGROW|wxLEFT|wxRIGHT|wxTOP, 0);
	m_projstart = new wxStaticText( m_catpanel, wxID_PROJSTART, _("Match section and/or setting (wildcards OK, leave empty for all in category): "), wxDefaultPosition, wxDefaultSize, 0 );
	itemBoxSizer45->Add(m_projstart, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

	wxBoxSizer* itemBoxSizer47 = new wxBoxSizer(wxHORIZONTAL);
	itemBoxSizer30->Add(itemBoxSizer47, 0, wxGROW|wxLEFT|wxRIGHT, 5);
	m_selstartsect = new wxStaticText( m_catpanel, wxID_PROJSTARTSECT, _("Section matches: "), wxDefaultPosition, wxDefaultSize, 0 );
	itemBoxSizer47->Add(m_selstartsect, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

	m_projstartsecttxt = new wxTextCtrl( m_catpanel, ID_PROJSTARTSECTEXTCTRL, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER );
	itemBoxSizer47->Add(m_projstartsecttxt, 20, wxALIGN_CENTER_VERTICAL|wxALL, 5);

	wxBoxSizer* itemBoxSizer50 = new wxBoxSizer(wxHORIZONTAL);
	itemBoxSizer30->Add(itemBoxSizer50, 0, wxGROW|wxLEFT|wxRIGHT|wxBOTTOM, 5);
	m_selstartset = new wxStaticText( m_catpanel, wxID_PROJSTARTSET, _("Setting matches:"), wxDefaultPosition, wxDefaultSize, 0 );
	itemBoxSizer50->Add(m_selstartset, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

	m_projstartsettxt = new wxTextCtrl( m_catpanel, ID_PROJSTARTSETTEXTCTRL, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER );
	itemBoxSizer50->Add(m_projstartsettxt, 20, wxALIGN_CENTER_VERTICAL|wxALL, 5);

	wxBoxSizer* itemBoxSizer53 = new wxBoxSizer(wxHORIZONTAL);
	itemBoxSizer30->Add(itemBoxSizer53, 0, wxGROW|wxALL, 5);
	m_projstartwarn = new wxStaticText( m_catpanel, wxID_PROJSTARTWARN, _("NOTE: If no matching sections or settings are found to show, \nall that are appropriate for the selected action will be shown."), wxDefaultPosition, wxDefaultSize, 0 );
	m_projstartwarn->SetFont(wxFont(9, wxSWISS, wxNORMAL, wxNORMAL, false, wxT("")));
	itemBoxSizer53->Add(m_projstartwarn, 0, wxALIGN_TOP|wxALL, 5);

	wxBoxSizer* itemBoxSizer55 = new wxBoxSizer(wxVERTICAL);
	itemBoxSizer30->Add(itemBoxSizer55, 10, wxGROW|wxALL, 5);
	wxBoxSizer* itemBoxSizer56 = new wxBoxSizer(wxHORIZONTAL);
	itemBoxSizer55->Add(itemBoxSizer56, 10, wxGROW|wxALL, 5);
	m_catresult = new wxTextCtrl( m_catpanel, ID_CATRESULT, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER|wxTE_READONLY );
	itemBoxSizer56->Add(m_catresult, 20, wxALIGN_BOTTOM|wxALL, 5);

	itemBoxSizer56->Add(5, 5, 0, wxALIGN_BOTTOM|wxALL, 5);

	m_applycat = new wxButton( m_catpanel, ID_APPLYCAT, _("Apply selections"), wxDefaultPosition, wxDefaultSize, 0 );
	itemBoxSizer56->Add(m_applycat, 0, wxALIGN_BOTTOM|wxALL, 5);

	m_cathelpbutton = new wxButton( m_catpanel, ID_CATHELPBUTTON, _("?"), wxDefaultPosition, wxSize(20, -1), 0 );
	itemBoxSizer56->Add(m_cathelpbutton, 0, wxALIGN_BOTTOM|wxALL, 5);

	GetBookCtrl()->AddPage(m_catpanel, _("Category"));

	m_actpanel = new wxPanel( GetBookCtrl(), ID_ACTPANEL, wxDefaultPosition, wxDefaultSize, wxSUNKEN_BORDER|wxTAB_TRAVERSAL );
	wxBoxSizer* itemBoxSizer62 = new wxBoxSizer(wxVERTICAL);
	m_actpanel->SetSizer(itemBoxSizer62);

	wxBoxSizer* itemBoxSizer63 = new wxBoxSizer(wxHORIZONTAL);
	itemBoxSizer62->Add(itemBoxSizer63, 0, wxGROW|wxALL, 5);
	m_actstartfile = new wxStaticText( m_actpanel, wxID_ACTSTARTFILE, _("Selected start file: "), wxDefaultPosition, wxDefaultSize, 0 );
	itemBoxSizer63->Add(m_actstartfile, 0, wxALIGN_CENTER_VERTICAL|wxLEFT|wxRIGHT|wxTOP, 5);

	m_actstarttext = new wxTextCtrl( m_actpanel, ID_ACTSTARTTEXT, _("Starting .ini configuration file not selected yet."), wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER|wxTE_READONLY );
	itemBoxSizer63->Add(m_actstarttext, 20, wxALIGN_CENTER_VERTICAL|wxLEFT|wxRIGHT|wxTOP, 5);

	wxBoxSizer* itemBoxSizer66 = new wxBoxSizer(wxHORIZONTAL);
	itemBoxSizer62->Add(itemBoxSizer66, 0, wxALIGN_LEFT|wxALL, 5);
	wxStaticText* itemStaticText67 = new wxStaticText( m_actpanel, wxID_ACTSEL, _("Select action to be performed:"), wxDefaultPosition, wxDefaultSize, 0 );
	itemBoxSizer66->Add(itemStaticText67, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

	wxBoxSizer* itemBoxSizer68 = new wxBoxSizer(wxHORIZONTAL);
	itemBoxSizer62->Add(itemBoxSizer68, 0, wxGROW|wxALL, 5);
	wxBoxSizer* itemBoxSizer69 = new wxBoxSizer(wxVERTICAL);
	itemBoxSizer68->Add(itemBoxSizer69, 100, wxALIGN_CENTER_VERTICAL|wxLEFT|wxRIGHT, 5);
	wxStaticLine* itemStaticLine70 = new wxStaticLine( m_actpanel, wxID_STATIC, wxDefaultPosition, wxDefaultSize, wxLI_HORIZONTAL|wxSIMPLE_BORDER );
	itemBoxSizer69->Add(itemStaticLine70, 0, wxGROW|wxLEFT|wxRIGHT, 5);

	wxStaticText* itemStaticText71 = new wxStaticText( m_actpanel, wxID_SECTADD, _("ADD new item:"), wxDefaultPosition, wxDefaultSize, 0 );
	itemBoxSizer69->Add(itemStaticText71, 0, wxALIGN_LEFT|wxLEFT|wxRIGHT, 5);

	m_sectaddbutton = new wxRadioButton( m_actpanel, ID_SECTADDBUTTON, _("Add new fixed-name section to file"), wxDefaultPosition, wxDefaultSize, wxRB_GROUP );
	m_sectaddbutton->SetValue(false);
	itemBoxSizer69->Add(m_sectaddbutton, 0, wxALIGN_LEFT|wxLEFT|wxRIGHT|wxTOP, 5);

	m_sectvnaddbutton = new wxRadioButton( m_actpanel, ID_SECTVNADDBUTTON, _("Add new variable-name section to file"), wxDefaultPosition, wxDefaultSize, 0 );
	m_sectvnaddbutton->SetValue(false);
	itemBoxSizer69->Add(m_sectvnaddbutton, 0, wxGROW|wxLEFT|wxRIGHT|wxTOP, 5);

	m_sectaddsetbutton = new wxRadioButton( m_actpanel, ID_SECTADDSETBUTTON, _("Add new setting to section"), wxDefaultPosition, wxDefaultSize, 0 );
	m_sectaddsetbutton->SetValue(false);
	itemBoxSizer69->Add(m_sectaddsetbutton, 0, wxALIGN_LEFT|wxALL, 5);

	m_sectaddvksetbutton = new wxRadioButton( m_actpanel, ID_SECTADDVKSETBUTTON, _("Add new variable-key setting to section"), wxDefaultPosition, wxDefaultSize, 0 );
	m_sectaddvksetbutton->SetValue(false);
	m_sectaddvksetbutton->Show(false);
	itemBoxSizer69->Add(m_sectaddvksetbutton, 0, wxALIGN_LEFT|wxALL, 5);

	wxStaticLine* itemStaticLine76 = new wxStaticLine( m_actpanel, wxID_STATIC, wxDefaultPosition, wxDefaultSize, wxLI_HORIZONTAL|wxSIMPLE_BORDER );
	itemBoxSizer69->Add(itemStaticLine76, 0, wxGROW|wxLEFT|wxRIGHT, 5);

	wxStaticText* itemStaticText77 = new wxStaticText( m_actpanel, wxID_SECTEDIT, _("EDIT existing item:"), wxDefaultPosition, wxDefaultSize, 0 );
	itemBoxSizer69->Add(itemStaticText77, 0, wxALIGN_LEFT|wxLEFT|wxRIGHT|wxTOP, 5);

	m_sectchangebutton = new wxRadioButton( m_actpanel, ID_SECTCHANGEBUTTON, _("Edit full section content"), wxDefaultPosition, wxDefaultSize, 0 );
	m_sectchangebutton->SetValue(false);
	itemBoxSizer69->Add(m_sectchangebutton, 0, wxALIGN_LEFT|wxLEFT|wxRIGHT|wxTOP, 5);

	m_sectseteditbutton = new wxRadioButton( m_actpanel, ID_SECTSETEDITBUTTON, _("Edit one setting in section"), wxDefaultPosition, wxDefaultSize, 0 );
	m_sectseteditbutton->SetValue(false);
	itemBoxSizer69->Add(m_sectseteditbutton, 0, wxALIGN_LEFT|wxALL, 5);

	wxStaticLine* itemStaticLine80 = new wxStaticLine( m_actpanel, wxID_STATIC, wxDefaultPosition, wxDefaultSize, wxLI_HORIZONTAL|wxSIMPLE_BORDER );
	itemBoxSizer69->Add(itemStaticLine80, 0, wxGROW|wxLEFT|wxRIGHT, 5);

	wxStaticText* itemStaticText81 = new wxStaticText( m_actpanel, wxID_SECTDEL, _("DELETE existing item:"), wxDefaultPosition, wxDefaultSize, 0 );
	itemBoxSizer69->Add(itemStaticText81, 0, wxALIGN_LEFT|wxLEFT|wxRIGHT|wxTOP, 5);

	m_sectdelbutton = new wxRadioButton( m_actpanel, ID_SECTDELBUTTON, _("Delete section from one file"), wxDefaultPosition, wxDefaultSize, 0 );
	m_sectdelbutton->SetValue(false);
	itemBoxSizer69->Add(m_sectdelbutton, 0, wxALIGN_LEFT|wxLEFT|wxRIGHT|wxTOP, 5);

	m_sectdelsetbutton = new wxRadioButton( m_actpanel, ID_SECTDELSETBUTTON, _("Delete setting from one section"), wxDefaultPosition, wxDefaultSize, 0 );
	m_sectdelsetbutton->SetValue(false);
	itemBoxSizer69->Add(m_sectdelsetbutton, 0, wxALIGN_LEFT|wxALL, 5);

	wxStaticLine* itemStaticLine84 = new wxStaticLine( m_actpanel, wxID_STATIC, wxDefaultPosition, wxDefaultSize, wxLI_HORIZONTAL|wxSIMPLE_BORDER );
	itemBoxSizer69->Add(itemStaticLine84, 0, wxGROW|wxLEFT|wxRIGHT, 5);

	wxStaticText* itemStaticText85 = new wxStaticText( m_actpanel, wxID_SECTREST, _("RESTORE deleted item:"), wxDefaultPosition, wxDefaultSize, 0 );
	itemBoxSizer69->Add(itemStaticText85, 0, wxALIGN_LEFT|wxLEFT|wxRIGHT|wxTOP, 5);

	m_sectrestbutton = new wxRadioButton( m_actpanel, ID_SECTRESTBUTTON, _("Restore deleted section in one file"), wxDefaultPosition, wxDefaultSize, 0 );
	m_sectrestbutton->SetValue(false);
	itemBoxSizer69->Add(m_sectrestbutton, 0, wxALIGN_LEFT|wxLEFT|wxRIGHT|wxTOP, 5);

	m_sectrestsetbutton = new wxRadioButton( m_actpanel, ID_SECTRESTSETBUTTON, _("Restore deleted setting in one section"), wxDefaultPosition, wxDefaultSize, 0 );
	m_sectrestsetbutton->SetValue(false);
	itemBoxSizer69->Add(m_sectrestsetbutton, 0, wxALIGN_LEFT|wxALL, 5);

	wxStaticLine* itemStaticLine88 = new wxStaticLine( m_actpanel, wxID_STATIC, wxDefaultPosition, wxDefaultSize, wxLI_HORIZONTAL|wxSIMPLE_BORDER );
	itemBoxSizer69->Add(itemStaticLine88, 0, wxGROW|wxLEFT|wxRIGHT, 5);

	wxStaticText* itemStaticText89 = new wxStaticText( m_actpanel, wxID_SECTREN, _("RENAME existing item:"), wxDefaultPosition, wxDefaultSize, 0 );
	itemBoxSizer69->Add(itemStaticText89, 0, wxALIGN_LEFT|wxLEFT|wxRIGHT|wxTOP, 5);

	m_sectrenbutton = new wxRadioButton( m_actpanel, ID_SECTRENBUTTON, _("Rename variable-name section in one file"), wxDefaultPosition, wxDefaultSize, 0 );
	m_sectrenbutton->SetValue(false);
	itemBoxSizer69->Add(m_sectrenbutton, 0, wxALIGN_LEFT|wxLEFT|wxRIGHT|wxTOP, 5);

	m_sectrensetbutton = new wxRadioButton( m_actpanel, ID_SECTRENSETBUTTON, _("Rename variable-key setting in one section"), wxDefaultPosition, wxDefaultSize, 0 );
	m_sectrensetbutton->SetValue(false);
	itemBoxSizer69->Add(m_sectrensetbutton, 0, wxALIGN_LEFT|wxALL, 5);

	wxStaticLine* itemStaticLine92 = new wxStaticLine( m_actpanel, wxID_STATIC, wxDefaultPosition, wxDefaultSize, wxLI_HORIZONTAL|wxSIMPLE_BORDER );
	itemBoxSizer69->Add(itemStaticLine92, 0, wxGROW|wxLEFT|wxRIGHT, 5);

	wxStaticText* itemStaticText93 = new wxStaticText( m_actpanel, wxID_SECTMOV, _("MOVE existing item:"), wxDefaultPosition, wxDefaultSize, 0 );
	itemBoxSizer69->Add(itemStaticText93, 0, wxALIGN_LEFT|wxLEFT|wxRIGHT|wxTOP, 5);

	m_sectmovbutton = new wxRadioButton( m_actpanel, ID_SECTMOVBUTTON, _("Move section within one file"), wxDefaultPosition, wxDefaultSize, 0 );
	m_sectmovbutton->SetValue(false);
	itemBoxSizer69->Add(m_sectmovbutton, 0, wxALIGN_LEFT|wxLEFT|wxRIGHT|wxTOP, 5);

	m_sectmovfbutton = new wxRadioButton( m_actpanel, ID_SECTMOVFBUTTON, _("Move section between files"), wxDefaultPosition, wxDefaultSize, 0 );
	m_sectmovfbutton->SetValue(false);
	itemBoxSizer69->Add(m_sectmovfbutton, 0, wxALIGN_LEFT|wxLEFT|wxRIGHT|wxTOP, 5);

	m_sectmovsetbutton = new wxRadioButton( m_actpanel, ID_SECTMOVSETBUTTON, _("Move setting within one section"), wxDefaultPosition, wxDefaultSize, 0 );
	m_sectmovsetbutton->SetValue(false);
	itemBoxSizer69->Add(m_sectmovsetbutton, 0, wxALIGN_LEFT|wxLEFT|wxRIGHT|wxTOP, 5);

	m_sectmovsetfbutton = new wxRadioButton( m_actpanel, ID_SECTMOVSETFBUTTON, _("Move setting between files"), wxDefaultPosition, wxDefaultSize, 0 );
	m_sectmovsetfbutton->SetValue(false);
	itemBoxSizer69->Add(m_sectmovsetfbutton, 0, wxALIGN_LEFT|wxALL, 5);

	wxStaticLine* itemStaticLine98 = new wxStaticLine( m_actpanel, wxID_STATIC, wxDefaultPosition, wxDefaultSize, wxLI_HORIZONTAL|wxSIMPLE_BORDER );
	itemBoxSizer69->Add(itemStaticLine98, 0, wxGROW|wxLEFT|wxRIGHT, 5);

	wxStaticText* itemStaticText99 = new wxStaticText( m_actpanel, wxID_SECTCPY, _("COPY existing item:"), wxDefaultPosition, wxDefaultSize, 0 );
	itemBoxSizer69->Add(itemStaticText99, 0, wxALIGN_LEFT|wxLEFT|wxRIGHT|wxTOP, 5);

	m_sectcpybutton = new wxRadioButton( m_actpanel, ID_SECTCPYBUTTON, _("Copy section between files"), wxDefaultPosition, wxDefaultSize, 0 );
	m_sectcpybutton->SetValue(false);
	itemBoxSizer69->Add(m_sectcpybutton, 0, wxALIGN_LEFT|wxLEFT|wxRIGHT|wxTOP, 5);

	m_sectcpysetfbutton = new wxRadioButton( m_actpanel, ID_SECTCPYSETBUTTON, _("Copy setting between files"), wxDefaultPosition, wxDefaultSize, 0 );
	m_sectcpysetfbutton->SetValue(false);
	itemBoxSizer69->Add(m_sectcpysetfbutton, 0, wxALIGN_LEFT|wxALL, 5);

	wxStaticLine* itemStaticLine102 = new wxStaticLine( m_actpanel, wxID_STATIC, wxDefaultPosition, wxDefaultSize, wxLI_HORIZONTAL|wxSIMPLE_BORDER );
	itemBoxSizer69->Add(itemStaticLine102, 0, wxGROW|wxLEFT|wxRIGHT, 5);

	wxStaticText* itemStaticText103 = new wxStaticText( m_actpanel, wxID_SECTDUP, _("MERGE duplicate items:"), wxDefaultPosition, wxDefaultSize, 0 );
	itemBoxSizer69->Add(itemStaticText103, 0, wxALIGN_LEFT|wxLEFT|wxRIGHT|wxTOP, 5);

	m_sectdupbutton = new wxRadioButton( m_actpanel, ID_SECTDUPBUTTON, _("Merge duplicate sections in one file"), wxDefaultPosition, wxDefaultSize, 0 );
	m_sectdupbutton->SetValue(false);
	itemBoxSizer69->Add(m_sectdupbutton, 0, wxALIGN_LEFT|wxLEFT|wxRIGHT|wxTOP, 5);

	m_sectdupsetbutton = new wxRadioButton( m_actpanel, ID_SECTDUPSETBUTTON, _("Merge duplicate settings in one section"), wxDefaultPosition, wxDefaultSize, 0 );
	m_sectdupsetbutton->SetValue(false);
	itemBoxSizer69->Add(m_sectdupsetbutton, 0, wxALIGN_LEFT|wxALL, 5);

	wxStaticLine* itemStaticLine106 = new wxStaticLine( m_actpanel, wxID_STATIC, wxDefaultPosition, wxDefaultSize, wxLI_HORIZONTAL|wxSIMPLE_BORDER );
	itemBoxSizer69->Add(itemStaticLine106, 0, wxGROW|wxLEFT|wxRIGHT, 5);

	wxBoxSizer* itemBoxSizer107 = new wxBoxSizer(wxVERTICAL);
	itemBoxSizer62->Add(itemBoxSizer107, 20, wxGROW|wxALL, 5);
	wxBoxSizer* itemBoxSizer108 = new wxBoxSizer(wxHORIZONTAL);
	itemBoxSizer107->Add(itemBoxSizer108, 20, wxGROW|wxALL, 5);
	m_actresult = new wxTextCtrl( m_actpanel, ID_ACTRESULT, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER|wxTE_READONLY );
	itemBoxSizer108->Add(m_actresult, 20, wxALIGN_BOTTOM|wxALL, 5);

	itemBoxSizer108->Add(5, 5, 0, wxALIGN_BOTTOM|wxALL, 5);

	m_applyact = new wxButton( m_actpanel, ID_APPLYACT, _("Apply action"), wxDefaultPosition, wxDefaultSize, 0 );
	itemBoxSizer108->Add(m_applyact, 0, wxALIGN_BOTTOM|wxALL, 5);

	m_acthelpbutton = new wxButton( m_actpanel, ID_ACTHELPBUTTON, _("?"), wxDefaultPosition, wxSize(20, -1), 0 );
	itemBoxSizer108->Add(m_acthelpbutton, 0, wxALIGN_BOTTOM|wxALL, 5);

	GetBookCtrl()->AddPage(m_actpanel, _("Action"));

	m_sectionpanel = new wxPanel( GetBookCtrl(), ID_SECTIONPANEL, wxDefaultPosition, wxDefaultSize, wxSUNKEN_BORDER|wxTAB_TRAVERSAL );
	wxBoxSizer* itemBoxSizer114 = new wxBoxSizer(wxVERTICAL);
	m_sectionpanel->SetSizer(itemBoxSizer114);

	wxBoxSizer* itemBoxSizer115 = new wxBoxSizer(wxHORIZONTAL);
	itemBoxSizer114->Add(itemBoxSizer115, 0, wxGROW|wxALL, 5);
	m_sectstartfile = new wxStaticText( m_sectionpanel, wxID_SECTSTARTFILE, _("Selected start file: "), wxDefaultPosition, wxDefaultSize, 0 );
	itemBoxSizer115->Add(m_sectstartfile, 0, wxALIGN_CENTER_VERTICAL|wxLEFT|wxRIGHT|wxTOP, 5);

	m_sectstarttext = new wxTextCtrl( m_sectionpanel, ID_SECTSTARTTEXT, _("Starting .ini configuration file not selected yet."), wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER|wxTE_READONLY );
	itemBoxSizer115->Add(m_sectstarttext, 20, wxALIGN_CENTER_VERTICAL|wxLEFT|wxRIGHT|wxTOP, 5);

	wxBoxSizer* itemBoxSizer118 = new wxBoxSizer(wxHORIZONTAL);
	itemBoxSizer114->Add(itemBoxSizer118, 0, wxGROW|wxLEFT|wxRIGHT|wxBOTTOM, 5);
	m_sectcat = new wxStaticText( m_sectionpanel, wxID_SECTCAT, _("Selected category: "), wxDefaultPosition, wxDefaultSize, 0 );
	itemBoxSizer118->Add(m_sectcat, 0, wxALIGN_CENTER_VERTICAL|wxLEFT|wxRIGHT|wxBOTTOM, 5);

	m_sectcattext = new wxTextCtrl( m_sectionpanel, ID_SECTCATTEXT, _("Category not selected yet."), wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER|wxTE_READONLY );
	itemBoxSizer118->Add(m_sectcattext, 20, wxALIGN_CENTER_VERTICAL|wxLEFT|wxRIGHT|wxBOTTOM, 5);

	wxBoxSizer* itemBoxSizer121 = new wxBoxSizer(wxHORIZONTAL);
	itemBoxSizer114->Add(itemBoxSizer121, 0, wxGROW|wxALL, 5);
	m_sectact = new wxStaticText( m_sectionpanel, wxID_SECTACT, _("Current action:"), wxDefaultPosition, wxDefaultSize, 0 );
	itemBoxSizer121->Add(m_sectact, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

	m_sectacttext = new wxTextCtrl( m_sectionpanel, ID_SECTACTTEXT, _("Section action not selected yet"), wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER|wxTE_READONLY );
	itemBoxSizer121->Add(m_sectacttext, 100, wxALIGN_CENTER_VERTICAL|wxALL, 5);

	wxBoxSizer* itemBoxSizer124 = new wxBoxSizer(wxVERTICAL);
	itemBoxSizer114->Add(itemBoxSizer124, 0, wxGROW|wxLEFT|wxRIGHT|wxTOP, 5);
	m_selsect = new wxStaticText( m_sectionpanel, wxID_SELSECT, _("Select section for action:"), wxDefaultPosition, wxDefaultSize, wxALIGN_LEFT );
	itemBoxSizer124->Add(m_selsect, 0, wxALIGN_LEFT|wxLEFT|wxRIGHT|wxBOTTOM, 5);

	m_sectctrl = new wxListCtrl( m_sectionpanel, ID_SECTCTRL, wxDefaultPosition, wxSize(100, 300), wxLC_REPORT|wxLC_SINGLE_SEL );
	itemBoxSizer124->Add(m_sectctrl, 100, wxGROW|wxLEFT|wxRIGHT, 5);

	m_secthelptext = new wxTextCtrl( m_sectionpanel, ID_SECTHELPTEXT, _("Section help text"), wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER|wxTE_READONLY );
	itemBoxSizer124->Add(m_secthelptext, 0, wxGROW|wxALL, 5);

	wxBoxSizer* itemBoxSizer128 = new wxBoxSizer(wxHORIZONTAL);
	itemBoxSizer114->Add(itemBoxSizer128, 0, wxGROW|wxALL, 5);
	wxBoxSizer* itemBoxSizer129 = new wxBoxSizer(wxVERTICAL);
	itemBoxSizer128->Add(itemBoxSizer129, 100, wxGROW|wxLEFT|wxRIGHT|wxTOP, 5);
	wxBoxSizer* itemBoxSizer130 = new wxBoxSizer(wxHORIZONTAL);
	itemBoxSizer129->Add(itemBoxSizer130, 0, wxGROW|wxALL, 0);

	m_sectvarname = new wxStaticText( m_sectionpanel, wxID_SECTVARNAME, _("For variable-name sections only:"), wxDefaultPosition, wxDefaultSize, 0 );
	itemBoxSizer129->Add(m_sectvarname, 0, wxALIGN_LEFT|wxLEFT|wxRIGHT|wxTOP, 5);

	wxStaticLine* itemStaticLine132 = new wxStaticLine( m_sectionpanel, wxID_STATIC, wxDefaultPosition, wxDefaultSize, wxLI_HORIZONTAL|wxSIMPLE_BORDER );
	itemBoxSizer129->Add(itemStaticLine132, 0, wxGROW|wxALL, 5);

	wxBoxSizer* itemBoxSizer133 = new wxBoxSizer(wxHORIZONTAL);
	itemBoxSizer129->Add(itemBoxSizer133, 0, wxGROW|wxLEFT|wxRIGHT, 5);
	m_sectvnadd = new wxStaticText( m_sectionpanel, wxID_SECTVNADD, _("Name for new section: "), wxDefaultPosition, wxDefaultSize, 0 );
	itemBoxSizer133->Add(m_sectvnadd, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

	m_sectvnaddtext = new wxTextCtrl( m_sectionpanel, ID_SECTADDTEXT, _("Name for new variable-name section"), wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER );
	m_sectvnaddtext->Enable(false);
	itemBoxSizer133->Add(m_sectvnaddtext, 100, wxALIGN_CENTER_VERTICAL|wxTOP, 0);

	wxBoxSizer* itemBoxSizer136 = new wxBoxSizer(wxHORIZONTAL);
	itemBoxSizer129->Add(itemBoxSizer136, 0, wxGROW|wxLEFT|wxRIGHT, 5);
	m_sectvnren = new wxStaticText( m_sectionpanel, wxID_SECTVNREN, _("Rename section: "), wxDefaultPosition, wxDefaultSize, 0 );
	itemBoxSizer136->Add(m_sectvnren, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

	m_sectvnrentext = new wxTextCtrl( m_sectionpanel, ID_SECTVNRENTEXT, _("New name for selected variable-name section"), wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER );
	m_sectvnrentext->Enable(false);
	itemBoxSizer136->Add(m_sectvnrentext, 100, wxALIGN_CENTER_VERTICAL|wxTOP, 0);

	wxBoxSizer* itemBoxSizer139 = new wxBoxSizer(wxHORIZONTAL);
	itemBoxSizer129->Add(itemBoxSizer139, 0, wxGROW|wxALL, 5);
	itemBoxSizer139->Add(5, 5, 4, wxALIGN_CENTER_VERTICAL|wxLEFT|wxRIGHT|wxBOTTOM, 5);

	m_sectformalias = new wxCheckBox( m_sectionpanel, ID_SECTFORMALIAS, _("Add old name to [FormatAliases]"), wxDefaultPosition, wxDefaultSize, 0 );
	m_sectformalias->SetValue(false);
	itemBoxSizer139->Add(m_sectformalias, 0, wxALIGN_CENTER_VERTICAL|wxLEFT|wxRIGHT|wxBOTTOM, 5);

	itemBoxSizer139->Add(5, 5, 3, wxALIGN_CENTER_VERTICAL|wxLEFT|wxRIGHT|wxBOTTOM, 5);

	wxStaticLine* itemStaticLine143 = new wxStaticLine( m_sectionpanel, wxID_STATIC, wxDefaultPosition, wxDefaultSize, wxLI_HORIZONTAL|wxSIMPLE_BORDER );
	itemBoxSizer129->Add(itemStaticLine143, 0, wxGROW|wxALL, 5);

	wxBoxSizer* itemBoxSizer144 = new wxBoxSizer(wxVERTICAL);
	itemBoxSizer114->Add(itemBoxSizer144, 1, wxGROW|wxALL, 5);
	wxBoxSizer* itemBoxSizer145 = new wxBoxSizer(wxHORIZONTAL);
	itemBoxSizer144->Add(itemBoxSizer145, 10, wxGROW|wxALL, 5);
	m_sectresult = new wxTextCtrl( m_sectionpanel, ID_SECTRESULT, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER|wxTE_READONLY );
	itemBoxSizer145->Add(m_sectresult, 20, wxALIGN_BOTTOM|wxALL, 5);

	itemBoxSizer145->Add(5, 5, 0, wxALIGN_BOTTOM|wxALL, 5);

	m_applysect = new wxButton( m_sectionpanel, ID_APPLYSECT, _("Apply section"), wxDefaultPosition, wxDefaultSize, 0 );
	itemBoxSizer145->Add(m_applysect, 0, wxALIGN_BOTTOM|wxALL, 5);

	m_secthelpbutton = new wxButton( m_sectionpanel, ID_SECTHELPBUTTON, _("?"), wxDefaultPosition, wxSize(20, -1), 0 );
	itemBoxSizer145->Add(m_secthelpbutton, 0, wxALIGN_BOTTOM|wxALL, 5);

	GetBookCtrl()->AddPage(m_sectionpanel, _("Section"));

	m_settingpanel = new wxPanel( GetBookCtrl(), ID_SETTINGPANEL, wxDefaultPosition, wxDefaultSize, wxSUNKEN_BORDER|wxTAB_TRAVERSAL );
	wxBoxSizer* itemBoxSizer151 = new wxBoxSizer(wxVERTICAL);
	m_settingpanel->SetSizer(itemBoxSizer151);

	wxBoxSizer* itemBoxSizer152 = new wxBoxSizer(wxHORIZONTAL);
	itemBoxSizer151->Add(itemBoxSizer152, 0, wxGROW|wxALL, 5);
	m_setstartfile = new wxStaticText( m_settingpanel, wxID_SETSTARTFILE, _("Selected start file: "), wxDefaultPosition, wxDefaultSize, 0 );
	itemBoxSizer152->Add(m_setstartfile, 0, wxALIGN_CENTER_VERTICAL|wxLEFT|wxRIGHT|wxTOP, 5);

	m_setstarttext = new wxTextCtrl( m_settingpanel, ID_SETSTARTTEXT, _("Starting .ini configuration file not selected yet."), wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER|wxTE_READONLY );
	itemBoxSizer152->Add(m_setstarttext, 20, wxALIGN_CENTER_VERTICAL|wxLEFT|wxRIGHT|wxTOP, 5);

	wxBoxSizer* itemBoxSizer155 = new wxBoxSizer(wxHORIZONTAL);
	itemBoxSizer151->Add(itemBoxSizer155, 0, wxGROW|wxLEFT|wxRIGHT|wxBOTTOM, 5);
	m_setcat = new wxStaticText( m_settingpanel, wxID_SETCAT, _("Selected category: "), wxDefaultPosition, wxDefaultSize, 0 );
	itemBoxSizer155->Add(m_setcat, 0, wxALIGN_CENTER_VERTICAL|wxLEFT|wxRIGHT|wxBOTTOM, 5);

	m_setcattext = new wxTextCtrl( m_settingpanel, ID_SETCATTEXT, _("Category not selected yet."), wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER|wxTE_READONLY );
	itemBoxSizer155->Add(m_setcattext, 20, wxALIGN_CENTER_VERTICAL|wxLEFT|wxRIGHT|wxBOTTOM, 5);

	wxBoxSizer* itemBoxSizer158 = new wxBoxSizer(wxHORIZONTAL);
	itemBoxSizer151->Add(itemBoxSizer158, 0, wxGROW|wxALL, 5);
	m_setact = new wxStaticText( m_settingpanel, wxID_SETACT, _("Current action:"), wxDefaultPosition, wxDefaultSize, 0 );
	itemBoxSizer158->Add(m_setact, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

	m_setacttext = new wxTextCtrl( m_settingpanel, ID_SETACTTEXT, _("Setting action not selected yet"), wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER|wxTE_READONLY );
	itemBoxSizer158->Add(m_setacttext, 100, wxALIGN_CENTER_VERTICAL|wxALL, 5);

	wxBoxSizer* itemBoxSizer161 = new wxBoxSizer(wxHORIZONTAL);
	itemBoxSizer151->Add(itemBoxSizer161, 0, wxGROW|wxLEFT|wxRIGHT|wxTOP, 5);
	m_setsect = new wxStaticText( m_settingpanel, wxID_SETSECT, _("Current section: "), wxDefaultPosition, wxDefaultSize, 0 );
	itemBoxSizer161->Add(m_setsect, 0, wxALIGN_CENTER_VERTICAL|wxLEFT|wxRIGHT, 5);

	m_setsecttext = new wxTextCtrl( m_settingpanel, ID_SETSECTTEXT, _("[CurrentSection]"), wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER|wxTE_READONLY );
	itemBoxSizer161->Add(m_setsecttext, 100, wxALIGN_CENTER_VERTICAL|wxLEFT|wxRIGHT, 5);

	wxBoxSizer* itemBoxSizer164 = new wxBoxSizer(wxHORIZONTAL);
	itemBoxSizer151->Add(itemBoxSizer164, 0, wxGROW|wxLEFT|wxRIGHT|wxBOTTOM, 5);
	m_setsecthelp = new wxStaticText( m_settingpanel, wxID_SETSECTHELP, _("Section purpose:"), wxDefaultPosition, wxDefaultSize, 0 );
	itemBoxSizer164->Add(m_setsecthelp, 0, wxALIGN_CENTER_VERTICAL|wxLEFT|wxRIGHT|wxBOTTOM, 5);

	m_setsecthelptext = new wxTextCtrl( m_settingpanel, ID_SETSECTHELPTEXT, _("Help for current section"), wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER|wxTE_READONLY );
	itemBoxSizer164->Add(m_setsecthelptext, 100, wxALIGN_CENTER_VERTICAL|wxLEFT|wxRIGHT|wxBOTTOM, 5);

	wxBoxSizer* itemBoxSizer167 = new wxBoxSizer(wxVERTICAL);
	itemBoxSizer151->Add(itemBoxSizer167, 0, wxGROW|wxLEFT|wxRIGHT|wxTOP, 5);
	m_selset = new wxStaticText( m_settingpanel, wxID_SELSET, _("Select setting for action:"), wxDefaultPosition, wxDefaultSize, wxALIGN_LEFT );
	itemBoxSizer167->Add(m_selset, 0, wxALIGN_LEFT|wxALL, 5);

	m_setlistctrl = new wxListCtrl( m_settingpanel, ID_SETCTRL, wxDefaultPosition, wxSize(100, 240), wxLC_REPORT|wxLC_SINGLE_SEL );
	itemBoxSizer167->Add(m_setlistctrl, 100, wxGROW|wxLEFT|wxRIGHT, 5);

	m_macrotext = new wxTextCtrl( m_settingpanel, ID_MACROTEXTCTRL, wxEmptyString, wxDefaultPosition, wxSize(100, 150), wxTE_MULTILINE|wxTE_PROCESS_ENTER|wxTE_LEFT );
	m_macrotext->Show(false);
	m_macrotext->Enable(false);
	itemBoxSizer167->Add(m_macrotext, 100, wxGROW|wxLEFT|wxRIGHT, 5);

	m_sethelptext = new wxTextCtrl( m_settingpanel, ID_SETHELPTEXT, _("Setting help text, or variable-key section help"), wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER|wxTE_READONLY );
	itemBoxSizer167->Add(m_sethelptext, 0, wxGROW|wxALL, 5);

	wxBoxSizer* itemBoxSizer172 = new wxBoxSizer(wxHORIZONTAL);
	itemBoxSizer151->Add(itemBoxSizer172, 0, wxGROW|wxALL, 5);
	wxBoxSizer* itemBoxSizer173 = new wxBoxSizer(wxVERTICAL);
	itemBoxSizer172->Add(itemBoxSizer173, 100, wxGROW|wxLEFT|wxRIGHT|wxTOP, 5);
	m_setvarkeysizer = new wxBoxSizer(wxHORIZONTAL);
	itemBoxSizer173->Add(m_setvarkeysizer, 0, wxGROW|wxALL, 5);
	m_setvarkey = new wxStaticText( m_settingpanel, wxID_SETVARKEY, _("For variable-key settings only:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_setvarkeysizer->Add(m_setvarkey, 0, wxALIGN_CENTER_VERTICAL|wxLEFT|wxRIGHT|wxTOP, 5);

	m_setvarkeysizer->Add(5, 5, 5, wxALIGN_CENTER_VERTICAL|wxALL, 5);

	m_setvarkeybutton = new wxButton( m_settingpanel, ID_SETVARKEYBUTTON, _("Select setting"), wxDefaultPosition, wxDefaultSize, 0 );
	m_setvarkeysizer->Add(m_setvarkeybutton, 0, wxALIGN_CENTER_VERTICAL|wxLEFT|wxRIGHT|wxTOP, 5);

	wxStaticLine* itemStaticLine178 = new wxStaticLine( m_settingpanel, wxID_STATIC, wxDefaultPosition, wxDefaultSize, wxLI_HORIZONTAL|wxSIMPLE_BORDER );
	itemBoxSizer173->Add(itemStaticLine178, 0, wxGROW|wxALL, 5);

	wxBoxSizer* itemBoxSizer179 = new wxBoxSizer(wxHORIZONTAL);
	itemBoxSizer173->Add(itemBoxSizer179, 0, wxGROW|wxLEFT|wxRIGHT, 5);
	m_setvkadd = new wxStaticText( m_settingpanel, wxID_SETVKADD, _("Name for new setting: "), wxDefaultPosition, wxDefaultSize, 0 );
	itemBoxSizer179->Add(m_setvkadd, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

	m_setvkaddtext = new wxTextCtrl( m_settingpanel, ID_SETVKADDTEXT, _("Name for new variable-key setting"), wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER );
	m_setvkaddtext->Enable(false);
	itemBoxSizer179->Add(m_setvkaddtext, 100, wxALIGN_CENTER_VERTICAL|wxTOP, 0);

	wxBoxSizer* itemBoxSizer182 = new wxBoxSizer(wxHORIZONTAL);
	itemBoxSizer173->Add(itemBoxSizer182, 0, wxGROW|wxLEFT|wxRIGHT, 5);
	m_setvkren = new wxStaticText( m_settingpanel, wxID_SETVKREN, _("Rename setting: "), wxDefaultPosition, wxDefaultSize, 0 );
	itemBoxSizer182->Add(m_setvkren, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

	m_setvkrentext = new wxTextCtrl( m_settingpanel, ID_SETVKRENTEXT, _("New name for selected variable-key setting"), wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER );
	m_setvkrentext->Enable(false);
	itemBoxSizer182->Add(m_setvkrentext, 100, wxALIGN_CENTER_VERTICAL|wxTOP, 0);

	wxStaticLine* itemStaticLine185 = new wxStaticLine( m_settingpanel, wxID_STATIC, wxDefaultPosition, wxDefaultSize, wxLI_HORIZONTAL|wxSIMPLE_BORDER );
	itemBoxSizer173->Add(itemStaticLine185, 0, wxGROW|wxALL, 5);

	wxBoxSizer* itemBoxSizer186 = new wxBoxSizer(wxVERTICAL);
	itemBoxSizer151->Add(itemBoxSizer186, 20, wxGROW|wxALL, 5);
	wxBoxSizer* itemBoxSizer187 = new wxBoxSizer(wxHORIZONTAL);
	itemBoxSizer186->Add(itemBoxSizer187, 20, wxGROW|wxALL, 5);
	m_setresult = new wxTextCtrl( m_settingpanel, ID_SETRESULT, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER|wxTE_READONLY );
	itemBoxSizer187->Add(m_setresult, 20, wxALIGN_BOTTOM|wxALL, 5);

	itemBoxSizer187->Add(5, 5, 0, wxALIGN_BOTTOM|wxALL, 5);

	m_applyset = new wxButton( m_settingpanel, ID_APPLYSET, _("Apply setting"), wxDefaultPosition, wxDefaultSize, 0 );
	itemBoxSizer187->Add(m_applyset, 0, wxALIGN_BOTTOM|wxALL, 5);

	m_sethelpbutton = new wxButton( m_settingpanel, ID_SETHELPBUTTON, _("?"), wxDefaultPosition, wxSize(20, -1), 0 );
	itemBoxSizer187->Add(m_sethelpbutton, 0, wxALIGN_BOTTOM|wxALL, 5);

	GetBookCtrl()->AddPage(m_settingpanel, _("Setting"));

	m_valpanel = new wxPanel( GetBookCtrl(), ID_VALPANEL, wxDefaultPosition, wxDefaultSize, wxSUNKEN_BORDER|wxTAB_TRAVERSAL );
	m_valpanelsizer = new wxBoxSizer(wxVERTICAL);
	m_valpanel->SetSizer(m_valpanelsizer);

	wxBoxSizer* itemBoxSizer194 = new wxBoxSizer(wxHORIZONTAL);
	m_valpanelsizer->Add(itemBoxSizer194, 0, wxGROW|wxALL, 5);
	m_valstartfile = new wxStaticText( m_valpanel, wxID_VALSTARTFILE, _("Selected start file: "), wxDefaultPosition, wxDefaultSize, 0 );
	itemBoxSizer194->Add(m_valstartfile, 0, wxALIGN_CENTER_VERTICAL|wxLEFT|wxRIGHT|wxTOP, 5);

	m_valstarttext = new wxTextCtrl( m_valpanel, ID_VALSTARTTEXT, _("Starting .ini configuration file not selected yet."), wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER|wxTE_READONLY );
	itemBoxSizer194->Add(m_valstarttext, 20, wxALIGN_CENTER_VERTICAL|wxLEFT|wxRIGHT|wxTOP, 5);

	wxBoxSizer* itemBoxSizer197 = new wxBoxSizer(wxHORIZONTAL);
	m_valpanelsizer->Add(itemBoxSizer197, 0, wxGROW|wxLEFT|wxRIGHT|wxBOTTOM, 5);
	m_valcat = new wxStaticText( m_valpanel, wxID_VALCAT, _("Selected category: "), wxDefaultPosition, wxDefaultSize, 0 );
	itemBoxSizer197->Add(m_valcat, 0, wxALIGN_CENTER_VERTICAL|wxLEFT|wxRIGHT|wxBOTTOM, 5);

	m_valcattext = new wxTextCtrl( m_valpanel, ID_VALCATTEXT, _("Category not selected yet."), wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER|wxTE_READONLY );
	itemBoxSizer197->Add(m_valcattext, 20, wxALIGN_CENTER_VERTICAL|wxLEFT|wxRIGHT|wxBOTTOM, 5);

	wxBoxSizer* itemBoxSizer200 = new wxBoxSizer(wxHORIZONTAL);
	m_valpanelsizer->Add(itemBoxSizer200, 0, wxGROW|wxALL, 5);
	m_valact = new wxStaticText( m_valpanel, wxID_VALACT, _("Current action:"), wxDefaultPosition, wxDefaultSize, 0 );
	itemBoxSizer200->Add(m_valact, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

	m_valacttext = new wxTextCtrl( m_valpanel, ID_VALACTTEXT, _("File action not selected yet"), wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER|wxTE_READONLY );
	itemBoxSizer200->Add(m_valacttext, 100, wxALIGN_CENTER_VERTICAL|wxALL, 5);

	wxBoxSizer* itemBoxSizer203 = new wxBoxSizer(wxHORIZONTAL);
	m_valpanelsizer->Add(itemBoxSizer203, 0, wxGROW|wxLEFT|wxRIGHT|wxTOP, 5);
	m_valsect = new wxStaticText( m_valpanel, wxID_VALSECT, _("Current section: "), wxDefaultPosition, wxDefaultSize, 0 );
	itemBoxSizer203->Add(m_valsect, 0, wxALIGN_CENTER_VERTICAL|wxLEFT|wxRIGHT, 5);

	m_valsecttext = new wxTextCtrl( m_valpanel, ID_VALSECTTEXT, _("[CurrentSection]"), wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER|wxTE_READONLY );
	itemBoxSizer203->Add(m_valsecttext, 100, wxALIGN_CENTER_VERTICAL|wxLEFT|wxRIGHT, 5);

	wxBoxSizer* itemBoxSizer206 = new wxBoxSizer(wxHORIZONTAL);
	m_valpanelsizer->Add(itemBoxSizer206, 0, wxGROW|wxLEFT|wxRIGHT|wxBOTTOM, 5);
	m_valsecthelp = new wxStaticText( m_valpanel, wxID_VALSECTHELP, _("Section purpose:"), wxDefaultPosition, wxDefaultSize, 0 );
	itemBoxSizer206->Add(m_valsecthelp, 0, wxALIGN_CENTER_VERTICAL|wxLEFT|wxRIGHT|wxBOTTOM, 5);

	m_valsecthelptext = new wxTextCtrl( m_valpanel, ID_VALSECTHELPTEXT, _("Help for current section"), wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER|wxTE_READONLY );
	itemBoxSizer206->Add(m_valsecthelptext, 100, wxALIGN_CENTER_VERTICAL|wxLEFT|wxRIGHT|wxBOTTOM, 5);

	m_valsetsizer = new wxBoxSizer(wxHORIZONTAL);
	m_valpanelsizer->Add(m_valsetsizer, 0, wxGROW|wxLEFT|wxRIGHT|wxTOP, 5);
	m_valset = new wxStaticText( m_valpanel, wxID_VALSET, _("Current setting: "), wxDefaultPosition, wxDefaultSize, 0 );
	m_valsetsizer->Add(m_valset, 0, wxALIGN_CENTER_VERTICAL|wxLEFT|wxRIGHT, 5);

	m_valsettext = new wxTextCtrl( m_valpanel, ID_VALSETTEXT, _("Current setting"), wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER|wxTE_READONLY );
	m_valsetsizer->Add(m_valsettext, 100, wxALIGN_CENTER_VERTICAL|wxLEFT|wxRIGHT, 5);

	m_valsethelpsizer = new wxBoxSizer(wxHORIZONTAL);
	m_valpanelsizer->Add(m_valsethelpsizer, 0, wxGROW|wxLEFT|wxRIGHT|wxBOTTOM, 5);
	m_valsethelp = new wxStaticText( m_valpanel, wxID_VALSETHELP, _("Setting purpose:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_valsethelpsizer->Add(m_valsethelp, 0, wxALIGN_CENTER_VERTICAL|wxLEFT|wxRIGHT|wxBOTTOM, 5);

	m_valsethelptext = new wxTextCtrl( m_valpanel, ID_VALSETHELPTEXT, _("Help for current setting"), wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER|wxTE_READONLY );
	m_valsethelpsizer->Add(m_valsethelptext, 100, wxALIGN_CENTER_VERTICAL|wxLEFT|wxRIGHT|wxBOTTOM, 5);

	wxBoxSizer* itemBoxSizer215 = new wxBoxSizer(wxVERTICAL);
	m_valpanelsizer->Add(itemBoxSizer215, 0, wxGROW|wxLEFT|wxRIGHT|wxTOP, 5);
	m_selini = new wxStaticText( m_valpanel, wxID_SELINI, _("Select ini to change for this action:"), wxDefaultPosition, wxDefaultSize, 0 );
	itemBoxSizer215->Add(m_selini, 0, wxALIGN_LEFT|wxALL, 5);

	m_selinictrl = new wxListCtrl( m_valpanel, ID_SELINICTRL, wxDefaultPosition, wxSize(300, 260), wxLC_REPORT|wxLC_SINGLE_SEL );
	itemBoxSizer215->Add(m_selinictrl, 100, wxGROW|wxLEFT|wxRIGHT|wxTOP, 5);

	m_valhelptext = new wxTextCtrl( m_valpanel, ID_VALHELPTEXT, _("ini-file help text"), wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER|wxTE_READONLY );
	itemBoxSizer215->Add(m_valhelptext, 0, wxGROW|wxALL, 5);

	m_valcurrvalsizer = new wxBoxSizer(wxHORIZONTAL);
	m_valpanelsizer->Add(m_valcurrvalsizer, 0, wxGROW|wxALL, 5);
	m_currval = new wxStaticText( m_valpanel, wxID_CURRVAL, _("Effective value: "), wxDefaultPosition, wxDefaultSize, 0 );
	m_valcurrvalsizer->Add(m_currval, 0, wxALIGN_CENTER_VERTICAL|wxLEFT|wxRIGHT|wxBOTTOM, 5);

	m_currvaltext = new wxTextCtrl( m_valpanel, ID_CURRVALTEXT, _("Effective value from above settings"), wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER|wxTE_READONLY );
	m_valcurrvalsizer->Add(m_currvaltext, 100, wxALIGN_CENTER_VERTICAL|wxLEFT|wxRIGHT, 5);

	wxBoxSizer* itemBoxSizer222 = new wxBoxSizer(wxVERTICAL);
	m_valpanelsizer->Add(itemBoxSizer222, 20, wxGROW|wxALL, 5);
	wxBoxSizer* itemBoxSizer223 = new wxBoxSizer(wxHORIZONTAL);
	itemBoxSizer222->Add(itemBoxSizer223, 20, wxGROW|wxALL, 5);
	m_valresult = new wxTextCtrl( m_valpanel, ID_VALRESULT, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER|wxTE_READONLY );
	itemBoxSizer223->Add(m_valresult, 20, wxALIGN_BOTTOM|wxALL, 5);

	itemBoxSizer223->Add(5, 5, 0, wxALIGN_BOTTOM|wxALL, 5);

	m_applyval = new wxButton( m_valpanel, ID_APPLYVAL, _("Apply ini file"), wxDefaultPosition, wxDefaultSize, 0 );
	itemBoxSizer223->Add(m_applyval, 0, wxALIGN_BOTTOM|wxALL, 5);

	m_valhelpbutton = new wxButton( m_valpanel, ID_VALHELPBUTTON, _("?"), wxDefaultPosition, wxSize(20, -1), 0 );
	itemBoxSizer223->Add(m_valhelpbutton, 0, wxALIGN_BOTTOM|wxALL, 5);

	GetBookCtrl()->AddPage(m_valpanel, _(".ini File"));

	m_vselpanel = new wxPanel( GetBookCtrl(), ID_VSELPANEL, wxDefaultPosition, wxDefaultSize, wxSUNKEN_BORDER|wxTAB_TRAVERSAL );
	m_vselpanelsizer = new wxBoxSizer(wxVERTICAL);
	m_vselpanel->SetSizer(m_vselpanelsizer);

	wxBoxSizer* itemBoxSizer230 = new wxBoxSizer(wxHORIZONTAL);
	m_vselpanelsizer->Add(itemBoxSizer230, 0, wxGROW|wxALL, 5);
	m_vselstartfile = new wxStaticText( m_vselpanel, wxID_VSELSTARTFILE, _("Selected start file: "), wxDefaultPosition, wxDefaultSize, 0 );
	itemBoxSizer230->Add(m_vselstartfile, 0, wxALIGN_CENTER_VERTICAL|wxLEFT|wxRIGHT|wxTOP, 5);

	m_vselstarttext = new wxTextCtrl( m_vselpanel, ID_VSELSTARTTEXT, _("Starting .ini configuration file not selected yet."), wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER|wxTE_READONLY );
	itemBoxSizer230->Add(m_vselstarttext, 20, wxALIGN_CENTER_VERTICAL|wxLEFT|wxRIGHT|wxTOP, 5);

	wxBoxSizer* itemBoxSizer233 = new wxBoxSizer(wxHORIZONTAL);
	m_vselpanelsizer->Add(itemBoxSizer233, 0, wxGROW|wxLEFT|wxRIGHT|wxBOTTOM, 5);
	m_vselcat = new wxStaticText( m_vselpanel, wxID_VSELCAT, _("Selected category: "), wxDefaultPosition, wxDefaultSize, 0 );
	itemBoxSizer233->Add(m_vselcat, 0, wxALIGN_CENTER_VERTICAL|wxLEFT|wxRIGHT|wxBOTTOM, 5);

	m_vselcattext = new wxTextCtrl( m_vselpanel, ID_VSELCATTEXT, _("Category not selected yet."), wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER|wxTE_READONLY );
	itemBoxSizer233->Add(m_vselcattext, 20, wxALIGN_CENTER_VERTICAL|wxLEFT|wxRIGHT|wxBOTTOM, 5);

	wxBoxSizer* itemBoxSizer236 = new wxBoxSizer(wxHORIZONTAL);
	m_vselpanelsizer->Add(itemBoxSizer236, 0, wxGROW|wxALL, 5);
	m_vselact = new wxStaticText( m_vselpanel, wxID_VSELACT, _("Current action:"), wxDefaultPosition, wxDefaultSize, 0 );
	itemBoxSizer236->Add(m_vselact, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

	m_vselacttext = new wxTextCtrl( m_vselpanel, ID_VSELACTTEXT, _("Action not selected yet"), wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER|wxTE_READONLY );
	itemBoxSizer236->Add(m_vselacttext, 100, wxALIGN_CENTER_VERTICAL|wxALL, 5);

	wxBoxSizer* itemBoxSizer239 = new wxBoxSizer(wxHORIZONTAL);
	m_vselpanelsizer->Add(itemBoxSizer239, 0, wxGROW|wxLEFT|wxRIGHT|wxTOP, 5);
	m_vselsect = new wxStaticText( m_vselpanel, wxID_VSELSECT, _("Current section: "), wxDefaultPosition, wxDefaultSize, 0 );
	itemBoxSizer239->Add(m_vselsect, 0, wxALIGN_CENTER_VERTICAL|wxLEFT|wxRIGHT, 5);

	m_vselsecttext = new wxTextCtrl( m_vselpanel, ID_VSELSECTTEXT, _("[CurrentSection]"), wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER|wxTE_READONLY );
	itemBoxSizer239->Add(m_vselsecttext, 100, wxALIGN_CENTER_VERTICAL|wxLEFT|wxRIGHT, 5);

	wxBoxSizer* itemBoxSizer242 = new wxBoxSizer(wxHORIZONTAL);
	m_vselpanelsizer->Add(itemBoxSizer242, 0, wxGROW|wxLEFT|wxRIGHT|wxBOTTOM, 5);
	m_vselsecthelp = new wxStaticText( m_vselpanel, wxID_VSELSECTHELP, _("Section purpose:"), wxDefaultPosition, wxDefaultSize, 0 );
	itemBoxSizer242->Add(m_vselsecthelp, 0, wxALIGN_CENTER_VERTICAL|wxLEFT|wxRIGHT|wxBOTTOM, 5);

	m_vselsecthelptext = new wxTextCtrl( m_vselpanel, ID_VSELSECTHELPTEXT, _("Help for current section"), wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER|wxTE_READONLY );
	itemBoxSizer242->Add(m_vselsecthelptext, 100, wxALIGN_CENTER_VERTICAL|wxLEFT|wxRIGHT|wxBOTTOM, 5);

	m_vselsetsizer = new wxBoxSizer(wxHORIZONTAL);
	m_vselpanelsizer->Add(m_vselsetsizer, 0, wxGROW|wxLEFT|wxRIGHT|wxTOP, 5);
	m_vselset = new wxStaticText( m_vselpanel, wxID_VSELSET, _("Current setting: "), wxDefaultPosition, wxDefaultSize, 0 );
	m_vselsetsizer->Add(m_vselset, 0, wxALIGN_CENTER_VERTICAL|wxLEFT|wxRIGHT, 5);

	m_vselsettext = new wxTextCtrl( m_vselpanel, ID_VSELSETTEXT, _("Current setting"), wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER|wxTE_READONLY );
	m_vselsetsizer->Add(m_vselsettext, 100, wxALIGN_CENTER_VERTICAL|wxLEFT|wxRIGHT, 5);

	m_vselsethelpsizer = new wxBoxSizer(wxHORIZONTAL);
	m_vselpanelsizer->Add(m_vselsethelpsizer, 0, wxGROW|wxLEFT|wxRIGHT|wxBOTTOM, 5);
	m_vselsethelp = new wxStaticText( m_vselpanel, wxID_VSELSETHELP, _("Setting purpose:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_vselsethelpsizer->Add(m_vselsethelp, 0, wxALIGN_CENTER_VERTICAL|wxLEFT|wxRIGHT|wxBOTTOM, 5);

	m_vselsethelptext = new wxTextCtrl( m_vselpanel, ID_VSELSETHELPTEXT, _("Help for current setting"), wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER|wxTE_READONLY );
	m_vselsethelpsizer->Add(m_vselsethelptext, 100, wxALIGN_CENTER_VERTICAL|wxLEFT|wxRIGHT|wxBOTTOM, 5);

	wxBoxSizer* itemBoxSizer251 = new wxBoxSizer(wxHORIZONTAL);
	m_vselpanelsizer->Add(itemBoxSizer251, 0, wxGROW|wxALL, 5);
	m_vselini = new wxStaticText( m_vselpanel, wxID_VSELINI, _("Current ini file: "), wxDefaultPosition, wxDefaultSize, 0 );
	itemBoxSizer251->Add(m_vselini, 0, wxALIGN_CENTER_VERTICAL|wxLEFT|wxRIGHT, 5);

	m_vselinitext = new wxTextCtrl( m_vselpanel, ID_VSELINITEXT, _("Current ini to edit"), wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER|wxTE_READONLY );
	itemBoxSizer251->Add(m_vselinitext, 100, wxALIGN_CENTER_VERTICAL|wxLEFT|wxRIGHT, 5);

	m_vseldestsizer = new wxBoxSizer(wxHORIZONTAL);
	m_vselpanelsizer->Add(m_vseldestsizer, 0, wxGROW|wxLEFT|wxRIGHT|wxBOTTOM, 5);
	m_vseldest = new wxStaticText( m_vselpanel, wxID_VSELDEST, _("Destination ini: "), wxDefaultPosition, wxDefaultSize, 0 );
	m_vseldest->Show(false);
	m_vseldestsizer->Add(m_vseldest, 0, wxALIGN_CENTER_VERTICAL|wxLEFT|wxRIGHT, 5);

	m_vseldesttext = new wxTextCtrl( m_vselpanel, ID_VSELDESTTEXT, _("Destination ini for move or copy"), wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER|wxTE_READONLY );
	m_vseldesttext->Show(false);
	m_vseldestsizer->Add(m_vseldesttext, 100, wxALIGN_CENTER_VERTICAL|wxLEFT|wxRIGHT, 5);

	m_vselrensizer = new wxBoxSizer(wxHORIZONTAL);
	m_vselpanelsizer->Add(m_vselrensizer, 0, wxGROW|wxALL, 5);
	m_vselren = new wxStaticText( m_vselpanel, wxID_VSELREN, _("New name:       "), wxDefaultPosition, wxDefaultSize, 0 );
	m_vselren->Show(false);
	m_vselrensizer->Add(m_vselren, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

	m_vselrentext = new wxTextCtrl( m_vselpanel, ID_VSELRENTEXT, _("New name for renamed section or setting"), wxDefaultPosition, wxDefaultSize, 0 );
	m_vselrentext->Show(false);
	m_vselrensizer->Add(m_vselrentext, 100, wxALIGN_CENTER_VERTICAL|wxALL, 5);

	m_vselvalsizer = new wxBoxSizer(wxHORIZONTAL);
	m_vselpanelsizer->Add(m_vselvalsizer, 0, wxGROW|wxLEFT|wxRIGHT|wxTOP, 5);
	m_vselname = new wxStaticText( m_vselpanel, wxID_VSELNAME, _("Selected name: "), wxDefaultPosition, wxDefaultSize, 0 );
	m_vselname->Show(false);
	m_vselvalsizer->Add(m_vselname, 0, wxALIGN_CENTER_VERTICAL|wxLEFT|wxRIGHT, 5);

	m_vselval = new wxStaticText( m_vselpanel, wxID_VSELVAL, _("Current value: "), wxDefaultPosition, wxDefaultSize, 0 );
	m_vselvalsizer->Add(m_vselval, 0, wxALIGN_CENTER_VERTICAL|wxLEFT|wxRIGHT, 5);

	m_vselvaltext = new wxTextCtrl( m_vselpanel, ID_VSELVALTEXT, _("Current value"), wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER );
	m_vselvalsizer->Add(m_vselvaltext, 100, wxALIGN_CENTER_VERTICAL|wxLEFT|wxRIGHT, 5);

	m_vselvalhelpsizer = new wxBoxSizer(wxHORIZONTAL);
	m_vselpanelsizer->Add(m_vselvalhelpsizer, 0, wxGROW|wxLEFT|wxRIGHT|wxBOTTOM, 5);
	m_vselvalhelpvertsizer = new wxBoxSizer(wxVERTICAL);
	m_vselvalhelpsizer->Add(m_vselvalhelpvertsizer, 0, wxGROW|wxALL, 5);
	m_vselvalhelp = new wxStaticText( m_vselpanel, wxID_VSELVALHELP, _("Value purpose: "), wxDefaultPosition, wxDefaultSize, 0 );
	m_vselvalhelpvertsizer->Add(m_vselvalhelp, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);

	m_vselnamehelp = new wxStaticText( m_vselpanel, wxID_VSELNAMEHELP, _("Purpose: "), wxDefaultPosition, wxDefaultSize, 0 );
	m_vselnamehelp->Show(false);
	m_vselvalhelpvertsizer->Add(m_vselnamehelp, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);

	m_vselvalbutton = new wxButton( m_vselpanel, ID_VSELVALBUTTON, _("Select value"), wxDefaultPosition, wxDefaultSize, 0 );
	m_vselvalhelpvertsizer->Add(m_vselvalbutton, 0, wxALIGN_CENTER_HORIZONTAL|wxLEFT|wxRIGHT|wxTOP, 5);

	m_vselvalhelptext = new wxTextCtrl( m_vselpanel, ID_VSELVALHELPTEXT, _("Help for current value"), wxDefaultPosition, wxSize(-1, 50), wxTE_MULTILINE|wxTE_PROCESS_ENTER|wxTE_READONLY );
	m_vselvalhelpsizer->Add(m_vselvalhelptext, 100, wxALIGN_CENTER_VERTICAL|wxALL, 5);

	m_vselctrlsizer = new wxBoxSizer(wxVERTICAL);
	m_vselpanelsizer->Add(m_vselctrlsizer, 40, wxGROW|wxLEFT|wxRIGHT|wxTOP, 5);
	m_selval = new wxStaticText( m_vselpanel, wxID_SELVAL, _("Select new value for this setting to change in selected .ini:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_vselctrlsizer->Add(m_selval, 0, wxALIGN_LEFT|wxALL, 5);

	m_vselfix = new wxStaticText( m_vselpanel, wxID_VSELFIX, _("Select new name for this setting to change in selected .ini:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_vselfix->Show(false);
	m_vselctrlsizer->Add(m_vselfix, 0, wxALIGN_LEFT|wxALL, 5);

	m_vseldel = new wxStaticText( m_vselpanel, wxID_VSELDEL, _("Review content before deletion:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_vseldel->Show(false);
	m_vselctrlsizer->Add(m_vseldel, 0, wxALIGN_LEFT|wxALL, 5);

	m_vselmov = new wxStaticText( m_vselpanel, wxID_VSELMOV, _("Move selected item up or down to desired position:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_vselmov->Show(false);
	m_vselctrlsizer->Add(m_vselmov, 0, wxALIGN_LEFT|wxALL, 5);

	m_selvalctrl = new wxListCtrl( m_vselpanel, ID_SELVALCTRL, wxDefaultPosition, wxSize(300, 200), wxLC_REPORT|wxLC_EDIT_LABELS|wxLC_SINGLE_SEL );
	m_vselctrlsizer->Add(m_selvalctrl, 100, wxGROW|wxLEFT|wxRIGHT|wxTOP, 5);

	m_vselmacadd = new wxStaticText( m_vselpanel, wxID_VSELMACADD, _("Add content of this section to file:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_vselmacadd->Show(false);
	m_vselctrlsizer->Add(m_vselmacadd, 0, wxALIGN_LEFT|wxALL, 5);

	m_vselmacedit = new wxStaticText( m_vselpanel, wxID_VSELMACEDIT, _("Edit content of this section:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_vselmacedit->Show(false);
	m_vselctrlsizer->Add(m_vselmacedit, 0, wxALIGN_LEFT|wxALL, 5);

	m_vselmactext = new wxTextCtrl( m_vselpanel, ID_VSELMACTEXT, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_MULTILINE|wxTE_PROCESS_ENTER|wxTE_LEFT );
	m_vselmactext->Show(false);
	m_vselctrlsizer->Add(m_vselmactext, 100, wxGROW|wxALL, 5);

	m_vselone = new wxStaticText( m_vselpanel, wxID_VSELONE, _("Edit value for this setting to use in selected .ini:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_vselctrlsizer->Add(m_vselone, 0, wxALIGN_LEFT|wxALL, 5);

	m_vselonetext = new wxTextCtrl( m_vselpanel, ID_VSELONETEXT, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER );
	m_vselctrlsizer->Add(m_vselonetext, 0, wxGROW|wxALL, 5);

	m_vselcurrvalsizer = new wxBoxSizer(wxHORIZONTAL);
	m_vselpanelsizer->Add(m_vselcurrvalsizer, 0, wxGROW|wxALL, 5);
	m_vselcurrval = new wxStaticText( m_vselpanel, wxID_VSELCURRVAL, _("Effective value: "), wxDefaultPosition, wxDefaultSize, 0 );
	m_vselcurrvalsizer->Add(m_vselcurrval, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

	m_vselcurrvaltext = new wxTextCtrl( m_vselpanel, ID_VSELCURRVALTEXT, _("Effective value from all inis with this setting"), wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER|wxTE_READONLY );
	m_vselcurrvalsizer->Add(m_vselcurrvaltext, 100, wxALIGN_CENTER_VERTICAL|wxALL, 5);

	m_vselmovsizer = new wxBoxSizer(wxHORIZONTAL);
	m_vselpanelsizer->Add(m_vselmovsizer, 0, wxGROW|wxALL, 5);
	m_vselmovsizer->Add(5, 5, 30, wxALIGN_CENTER_VERTICAL|wxLEFT|wxRIGHT|wxTOP, 5);

	m_vselupbutton = new wxButton( m_vselpanel, ID_VSELUPBUTTON, _("Move up"), wxDefaultPosition, wxDefaultSize, 0 );
	m_vselupbutton->Show(false);
	m_vselmovsizer->Add(m_vselupbutton, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

	m_vselmovsizer->Add(5, 5, 30, wxALIGN_CENTER_VERTICAL|wxLEFT|wxRIGHT|wxTOP, 5);

	m_vseldnbutton = new wxButton( m_vselpanel, ID_VSELDNBUTTON, _("Move down"), wxDefaultPosition, wxDefaultSize, 0 );
	m_vseldnbutton->Show(false);
	m_vselmovsizer->Add(m_vseldnbutton, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

	m_vselmovsizer->Add(5, 5, 30, wxALIGN_CENTER_VERTICAL|wxLEFT|wxRIGHT|wxTOP, 5);

	wxBoxSizer* itemBoxSizer290 = new wxBoxSizer(wxVERTICAL);
	m_vselpanelsizer->Add(itemBoxSizer290, 20, wxGROW|wxALL, 5);
	wxBoxSizer* itemBoxSizer291 = new wxBoxSizer(wxHORIZONTAL);
	itemBoxSizer290->Add(itemBoxSizer291, 20, wxGROW|wxALL, 5);
	m_vselresult = new wxTextCtrl( m_vselpanel, ID_VSELRESULT, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER|wxTE_READONLY );
	itemBoxSizer291->Add(m_vselresult, 20, wxALIGN_BOTTOM|wxALL, 5);

	itemBoxSizer291->Add(5, 5, 0, wxALIGN_BOTTOM|wxALL, 5);

	m_applyvsel = new wxButton( m_vselpanel, ID_APPLYVSEL, _("Finish action"), wxDefaultPosition, wxDefaultSize, 0 );
	itemBoxSizer291->Add(m_applyvsel, 0, wxALIGN_BOTTOM|wxALL, 5);

	m_vselhelpbutton = new wxButton( m_vselpanel, ID_VSELHELPBUTTON, _("?"), wxDefaultPosition, wxSize(20, -1), 0 );
	itemBoxSizer291->Add(m_vselhelpbutton, 0, wxALIGN_BOTTOM|wxALL, 5);

	GetBookCtrl()->AddPage(m_vselpanel, _("Finish"));

	m_optpanel = new wxPanel( GetBookCtrl(), ID_OPTPANEL, wxDefaultPosition, wxDefaultSize, wxSUNKEN_BORDER|wxTAB_TRAVERSAL );
	wxBoxSizer* itemBoxSizer297 = new wxBoxSizer(wxVERTICAL);
	m_optpanel->SetSizer(itemBoxSizer297);

	m_optsyssizer = new wxBoxSizer(wxHORIZONTAL);
	itemBoxSizer297->Add(m_optsyssizer, 0, wxGROW|wxALL, 5);
	m_optsys = new wxStaticText( m_optpanel, wxID_OPTSYS, _("Allow updates to system files: "), wxDefaultPosition, wxDefaultSize, 0 );
	m_optsys->Show(false);
	m_optsys->Enable(false);
	m_optsyssizer->Add(m_optsys, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

	m_optsyssizer->Add(5, 5, 50, wxALIGN_CENTER_VERTICAL|wxALL, 5);

	m_optsysyes = new wxRadioButton( m_optpanel, ID_OPTSYSYES, _("Yes"), wxDefaultPosition, wxDefaultSize, wxRB_GROUP );
	m_optsysyes->SetValue(false);
	m_optsysyes->Show(false);
	m_optsysyes->Enable(false);
	m_optsyssizer->Add(m_optsysyes, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

	m_optsyssizer->Add(20, 5, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

	m_optsysno = new wxRadioButton( m_optpanel, ID_OPTSYSNO, _("No"), wxDefaultPosition, wxDefaultSize, 0 );
	m_optsysno->SetValue(true);
	m_optsysno->Show(false);
	m_optsysno->Enable(false);
	m_optsyssizer->Add(m_optsysno, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

	wxBoxSizer* itemBoxSizer304 = new wxBoxSizer(wxHORIZONTAL);
	itemBoxSizer297->Add(itemBoxSizer304, 0, wxGROW|wxALL, 5);
	wxBoxSizer* itemBoxSizer305 = new wxBoxSizer(wxVERTICAL);
	itemBoxSizer304->Add(itemBoxSizer305, 100, wxALIGN_CENTER_VERTICAL|wxALL, 5);
	wxBoxSizer* itemBoxSizer306 = new wxBoxSizer(wxHORIZONTAL);
	itemBoxSizer305->Add(itemBoxSizer306, 0, wxGROW|wxALL, 5);
	m_optcolors = new wxStaticText( m_optpanel, wxID_OPTCOLORS, _("Select colors to use for:"), wxDefaultPosition, wxDefaultSize, 0 );
	itemBoxSizer306->Add(m_optcolors, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

	m_optcolorsect = new wxStaticText( m_optpanel, wxID_OPTCOLORSECT, _(" Available fixed-name sections:"), wxDefaultPosition, wxDefaultSize, 0 );
	itemBoxSizer305->Add(m_optcolorsect, 0, wxALIGN_LEFT|wxLEFT|wxRIGHT|wxTOP, 10);

	wxBoxSizer* itemBoxSizer309 = new wxBoxSizer(wxHORIZONTAL);
	itemBoxSizer305->Add(itemBoxSizer309, 100, wxGROW|wxALL, 5);
	itemBoxSizer309->Add(5, 5, 0, wxALIGN_CENTER_VERTICAL|wxLEFT|wxRIGHT|wxBOTTOM, 5);

	m_optcolorsecttext = new wxTextCtrl( m_optpanel, ID_OPTCOLORSECTTEXT, _("Use this color for available fixed-name sections not in current files: "), wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER|wxTE_READONLY|wxNO_BORDER );
	itemBoxSizer309->Add(m_optcolorsecttext, 100, wxALIGN_CENTER_VERTICAL|wxLEFT|wxRIGHT|wxBOTTOM, 5);

	m_optcolorsectctrl = new wxColourPickerCtrl( m_optpanel, ID_OPTCOLORSECTCTRL, wxColour(), wxDefaultPosition, wxDefaultSize, wxCLRP_DEFAULT_STYLE );
	itemBoxSizer309->Add(m_optcolorsectctrl, 0, wxALIGN_CENTER_VERTICAL|wxLEFT|wxRIGHT|wxBOTTOM, 5);

	m_optcolorset = new wxStaticText( m_optpanel, wxID_OPTCOLORSET, _(" Available fixed-key settings:"), wxDefaultPosition, wxDefaultSize, 0 );
	itemBoxSizer305->Add(m_optcolorset, 0, wxALIGN_LEFT|wxLEFT|wxRIGHT|wxTOP, 10);

	wxBoxSizer* itemBoxSizer314 = new wxBoxSizer(wxHORIZONTAL);
	itemBoxSizer305->Add(itemBoxSizer314, 100, wxGROW|wxALL, 5);
	itemBoxSizer314->Add(5, 5, 0, wxALIGN_CENTER_VERTICAL|wxLEFT|wxRIGHT|wxBOTTOM, 5);

	m_optcolorsettext = new wxTextCtrl( m_optpanel, ID_OPTCOLORSETTEXT, _("Use this color for available fixed-key settings not in current files: "), wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER|wxTE_READONLY|wxNO_BORDER );
	itemBoxSizer314->Add(m_optcolorsettext, 100, wxALIGN_CENTER_VERTICAL|wxLEFT|wxRIGHT|wxBOTTOM, 5);

	m_optcolorsetctrl = new wxColourPickerCtrl( m_optpanel, ID_OPTCOLORSETCTRL, wxColour(), wxDefaultPosition, wxDefaultSize, wxCLRP_DEFAULT_STYLE );
	itemBoxSizer314->Add(m_optcolorsetctrl, 0, wxALIGN_CENTER_VERTICAL|wxLEFT|wxRIGHT|wxBOTTOM, 5);

	m_optcolorvar = new wxStaticText( m_optpanel, wxID_OPTCOLORVAR, _(" Variable names and keys:"), wxDefaultPosition, wxDefaultSize, 0 );
	itemBoxSizer305->Add(m_optcolorvar, 0, wxALIGN_LEFT|wxLEFT|wxRIGHT|wxTOP, 10);

	wxBoxSizer* itemBoxSizer319 = new wxBoxSizer(wxHORIZONTAL);
	itemBoxSizer305->Add(itemBoxSizer319, 100, wxGROW|wxALL, 5);
	itemBoxSizer319->Add(5, 5, 0, wxALIGN_CENTER_VERTICAL|wxLEFT|wxRIGHT|wxBOTTOM, 5);

	m_optcolorvartext = new wxTextCtrl( m_optpanel, ID_OPTCOLORVARTEXT, _("Use this color for variable-name sections and variable-key settings: "), wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER|wxTE_READONLY|wxNO_BORDER );
	itemBoxSizer319->Add(m_optcolorvartext, 100, wxALIGN_CENTER_VERTICAL|wxLEFT|wxRIGHT|wxBOTTOM, 5);

	m_optcolorvarctrl = new wxColourPickerCtrl( m_optpanel, ID_OPTCOLORVARCTRL, wxColour(), wxDefaultPosition, wxDefaultSize, wxCLRP_DEFAULT_STYLE );
	itemBoxSizer319->Add(m_optcolorvarctrl, 0, wxALIGN_CENTER_VERTICAL|wxLEFT|wxRIGHT|wxBOTTOM, 5);

	m_optcolordef = new wxStaticText( m_optpanel, wxID_OPTCOLORDEF, _(" Internal defaults:"), wxDefaultPosition, wxDefaultSize, 0 );
	itemBoxSizer305->Add(m_optcolordef, 0, wxALIGN_LEFT|wxLEFT|wxRIGHT|wxTOP, 10);

	wxBoxSizer* itemBoxSizer324 = new wxBoxSizer(wxHORIZONTAL);
	itemBoxSizer305->Add(itemBoxSizer324, 100, wxGROW|wxALL, 5);
	itemBoxSizer324->Add(5, 5, 0, wxALIGN_CENTER_VERTICAL|wxLEFT|wxRIGHT|wxBOTTOM, 5);

	m_optcolordeftext = new wxTextCtrl( m_optpanel, ID_OPTCOLORDEFTEXT, _("Use this color for program default values in the ini-file panel:"), wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER|wxTE_READONLY|wxNO_BORDER );
	itemBoxSizer324->Add(m_optcolordeftext, 100, wxALIGN_CENTER_VERTICAL|wxLEFT|wxRIGHT|wxBOTTOM, 5);

	m_optcolordefctrl = new wxColourPickerCtrl( m_optpanel, ID_OPTCOLORDEFCTRL, wxColour(), wxDefaultPosition, wxDefaultSize, wxCLRP_DEFAULT_STYLE );
	itemBoxSizer324->Add(m_optcolordefctrl, 0, wxALIGN_CENTER_VERTICAL|wxLEFT|wxRIGHT|wxBOTTOM, 5);

	m_optcolorsys = new wxStaticText( m_optpanel, wxID_OPTCOLORSYS, _(" System ini files:"), wxDefaultPosition, wxDefaultSize, 0 );
	itemBoxSizer305->Add(m_optcolorsys, 0, wxALIGN_LEFT|wxLEFT|wxRIGHT|wxTOP, 10);

	wxBoxSizer* itemBoxSizer329 = new wxBoxSizer(wxHORIZONTAL);
	itemBoxSizer305->Add(itemBoxSizer329, 100, wxGROW|wxLEFT|wxRIGHT|wxBOTTOM, 5);
	itemBoxSizer329->Add(5, 5, 0, wxALIGN_CENTER_VERTICAL|wxLEFT|wxRIGHT|wxBOTTOM, 5);

	m_optcolorsystext = new wxTextCtrl( m_optpanel, ID_OPTCOLORSYSTEXT, _("Use this color for system-file values in the ini-file panel:\n"), wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER|wxTE_READONLY|wxNO_BORDER );
	itemBoxSizer329->Add(m_optcolorsystext, 100, wxALIGN_CENTER_VERTICAL|wxLEFT|wxRIGHT|wxBOTTOM, 5);

	m_optcolorsysctrl = new wxColourPickerCtrl( m_optpanel, ID_OPTCOLORSYSCTRL, wxColour(), wxDefaultPosition, wxDefaultSize, wxCLRP_DEFAULT_STYLE );
	itemBoxSizer329->Add(m_optcolorsysctrl, 0, wxALIGN_CENTER_VERTICAL|wxLEFT|wxRIGHT|wxBOTTOM, 5);

	wxBoxSizer* itemBoxSizer333 = new wxBoxSizer(wxHORIZONTAL);
	itemBoxSizer297->Add(itemBoxSizer333, 0, wxGROW|wxLEFT|wxRIGHT|wxTOP, 5);
	m_optaddinihist = new wxCheckBox( m_optpanel, ID_OPTADDINIHIST, _("Include history comments for all ini-file changes"), wxDefaultPosition, wxDefaultSize, 0 );
	m_optaddinihist->SetValue(false);
	itemBoxSizer333->Add(m_optaddinihist, 0, wxALIGN_CENTER_VERTICAL|wxLEFT|wxRIGHT|wxTOP, 5);

	wxBoxSizer* itemBoxSizer335 = new wxBoxSizer(wxHORIZONTAL);
	itemBoxSizer297->Add(itemBoxSizer335, 0, wxGROW|wxLEFT|wxRIGHT|wxBOTTOM, 5);
	itemBoxSizer335->Add(5, 5, 1, wxALIGN_CENTER_VERTICAL|wxLEFT|wxRIGHT|wxBOTTOM, 5);

	m_optusername = new wxStaticText( m_optpanel, wxID_OPTUSERNAME, _("User name for history comments: "), wxDefaultPosition, wxDefaultSize, 0 );
	itemBoxSizer335->Add(m_optusername, 0, wxALIGN_CENTER_VERTICAL|wxLEFT|wxRIGHT|wxBOTTOM, 5);

	m_optusernametext = new wxTextCtrl( m_optpanel, ID_OPTUSERNAMETEXT, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER );
	itemBoxSizer335->Add(m_optusernametext, 10, wxALIGN_CENTER_VERTICAL|wxLEFT|wxRIGHT|wxBOTTOM, 5);

	wxBoxSizer* itemBoxSizer339 = new wxBoxSizer(wxHORIZONTAL);
	itemBoxSizer297->Add(itemBoxSizer339, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);
	m_optbrowser = new wxStaticText( m_optpanel, wxID_OPTBROWSER, _("Select browser command for Help display:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_optbrowser->Show(false);
	m_optbrowser->Enable(false);
	itemBoxSizer339->Add(m_optbrowser, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

	wxArrayString m_optbrowserctrlStrings;
	m_optbrowserctrlStrings.Add(_("Select or enter path for browser"));
	m_optbrowserctrl = new wxComboBox( m_optpanel, ID_OPTBROWSERCTRL, _("Select or enter path for browser"), wxDefaultPosition, wxSize(220, -1), m_optbrowserctrlStrings, wxCB_DROPDOWN );
	m_optbrowserctrl->SetStringSelection(_("Select or enter path for browser"));
	m_optbrowserctrl->Show(false);
	m_optbrowserctrl->Enable(false);
	itemBoxSizer339->Add(m_optbrowserctrl, 10, wxALIGN_CENTER_VERTICAL|wxALL, 5);

	wxBoxSizer* itemBoxSizer342 = new wxBoxSizer(wxVERTICAL);
	itemBoxSizer297->Add(itemBoxSizer342, 20, wxGROW|wxALL, 5);
	wxBoxSizer* itemBoxSizer343 = new wxBoxSizer(wxHORIZONTAL);
	itemBoxSizer342->Add(itemBoxSizer343, 10, wxALIGN_RIGHT|wxALL, 5);
	m_optlhelpbutton = new wxButton( m_optpanel, ID_OPTHELPBUTTON, _("?"), wxDefaultPosition, wxSize(20, -1), 0 );
	itemBoxSizer343->Add(m_optlhelpbutton, 0, wxALIGN_BOTTOM|wxALL, 5);

	GetBookCtrl()->AddPage(m_optpanel, _("Preferences"));

	// Connect events and objects
	m_filepanel->Connect(ID_FILEPANEL, wxEVT_ENTER_WINDOW, wxMouseEventHandler(ConfigMgr::OnEnterFileWindow), NULL, this);
	m_projdirctrl->Connect(ID_PROJDIRCTRL, wxEVT_LEFT_DCLICK, wxMouseEventHandler(ConfigMgr::OnLeftDClick), NULL, this);
	m_catpanel->Connect(ID_CATPANEL, wxEVT_ENTER_WINDOW, wxMouseEventHandler(ConfigMgr::OnEnterCatWindow), NULL, this);
	m_actpanel->Connect(ID_ACTPANEL, wxEVT_ENTER_WINDOW, wxMouseEventHandler(ConfigMgr::OnEnterActWindow), NULL, this);
	m_sectionpanel->Connect(ID_SECTIONPANEL, wxEVT_ENTER_WINDOW, wxMouseEventHandler(ConfigMgr::OnEnterSectWindow), NULL, this);
	m_settingpanel->Connect(ID_SETTINGPANEL, wxEVT_ENTER_WINDOW, wxMouseEventHandler(ConfigMgr::OnEnterSetWindow), NULL, this);
	m_valpanel->Connect(ID_VALPANEL, wxEVT_ENTER_WINDOW, wxMouseEventHandler(ConfigMgr::OnEnterValWindow), NULL, this);
	m_vselpanel->Connect(ID_VSELPANEL, wxEVT_ENTER_WINDOW, wxMouseEventHandler(ConfigMgr::OnEnterVselWindow), NULL, this);
	m_optpanel->Connect(ID_OPTPANEL, wxEVT_ENTER_WINDOW, wxMouseEventHandler(ConfigMgr::OnEnterOptWindow), NULL, this);
////@end ConfigMgr content construction
}


/*
 * Should we show tooltips?
 */

bool ConfigMgr::ShowToolTips()
{
    return true;
}

/*
 * Get bitmap resources
 */

wxBitmap ConfigMgr::GetBitmapResource( const wxString& name )
{
    // Bitmap retrieval
////@begin ConfigMgr bitmap retrieval
	wxUnusedVar(name);
	return wxNullBitmap;
////@end ConfigMgr bitmap retrieval
}

/*
 * Get icon resources
 */

wxIcon ConfigMgr::GetIconResource( const wxString& name )
{
	// Icon retrieval
	wxIcon icon(name);
  return icon;
}


/*
 * wxEVT_INIT_DIALOG event handler for ID_CONFIGMGR
 */

void ConfigMgr::OnInitDialog( wxInitDialogEvent& event )
{
	InitColors();
  SetUpCategoriesTree();
	SetUpProjList();
	SetUpStartsList();
  SetUpSectionCtrl();
  SetUpSettingsCtrl();
  SetUpSelIniCtrl();
  SetUpSelValCtrl();

	m_projnamesel->Enable();
	m_projstarttplsel->Disable();
	//m_filebrowsectl->Disable();
	m_projdirctrl->Disable();

	m_catinitypebutton->SetValue(true);
	m_catselbutton->SetValue(false);
	m_catinichoice->Enable();
	m_sectyptree->Disable();


	if (CMgr::AllowSysIniSelect)
		m_optsysyes->SetValue(true);
	//if (CMgr::IncludeAllSections)
	//	m_catallbutton->SetValue(true);

	m_optaddinihist->SetValue(CMgr::AddIniHistory);
	if (CMgr::UserName)
		m_optusernametext->WriteText(wxString(CMgr::UserName));

	CMgr::CurrPanel = pnfile;
	CMgr::FilePanelEnabled = true;
	CMgr::CatPanelEnabled = false;
	CMgr::ActPanelEnabled = false;
	CMgr::SectPanelEnabled = false;
	CMgr::SetPanelEnabled = false;
	CMgr::ValPanelEnabled = false;
	CMgr::VselPanelEnabled = false;

	SetCommandLineSwitches();

  event.Skip();
}



// PROJECT tab



/*
 * wxEVT_ENTER_WINDOW event handler for ID_FILEPANEL
 */

void ConfigMgr::OnEnterFileWindow( wxMouseEvent& event )
{
	if (CMgr::FilePanelEnabled) {
		CMgr::CurrPanel = pnfile;
		ProjAlias = wxString("IDH_ZSelectProject");
	}
	else
		(GetBookCtrl())->ChangeSelection((size_t) CMgr::CurrPanel);

	event.Skip();
}


/*
 * wxEVT_COMMAND_RADIOBUTTON_SELECTED event handler for ID_PROJNAMEBUTTON
 */

void ConfigMgr::OnProjnamebuttonSelected( wxCommandEvent& event )
{
	if (m_projnamebutton->GetValue()) {
		m_projnamesel->Enable();
		m_projstarttplsel->Disable();
		//m_filebrowsectl->Disable();
		m_projdirctrl->Disable();
		ProjAlias = wxString("IDH_ZSelectProject");
	}
	event.Skip();
}

// when m_projnamebutton set, enable m_projnamesel, disable m_projstarttplsel

/*
 * wxEVT_COMMAND_CHOICE_SELECTED event handler for ID_PROJNAMESEL
 */

void ConfigMgr::OnProjnameselSelected( wxCommandEvent& event )
{
	SetIniChoice();
	event.Skip();
}


//  wxChoice* m_projstarttplsel is project list from projects.ini
//  set wxTextCtrl* m_projstarttext with its start name and path


/*
 * wxEVT_COMMAND_RADIOBUTTON_SELECTED event handler for ID_STARTTPLBUTTON
 */

void ConfigMgr::OnProjstarttplbuttonSelected( wxCommandEvent& event )
{
	if (m_projstarttplbutton->GetValue()) {
		m_projnamesel->Disable();
		m_projstarttplsel->Enable();
		//m_filebrowsectl->Disable();
		m_projdirctrl->Disable();
		ProjAlias = wxString("IDH_ZSelectStart");
	}
	event.Skip();
}

// when m_projstarttplbutton set, enable m_projstarttplsel, disable m_projnamesel

/*
 * wxEVT_COMMAND_CHOICE_SELECTED event handler for ID_PROJSTARTTPLSEL
 */

void ConfigMgr::OnProjstarttplselSelected( wxCommandEvent& event )
{
	SetIniChoice();
	event.Skip();
}


// 	wxChoice* m_projstarttplsel is StartFormatList, int GetSelection()
//  ProcStartsListChoice(int idx)
//  set wxTextCtrl* m_projstarttext with its start name and path


/*
 * wxEVT_COMMAND_RADIOBUTTON_SELECTED event handler for ID_FILEBROWSEBUTTON
 */

void ConfigMgr::OnFilebrowsebuttonSelected( wxCommandEvent& event )
{
	if (m_filebrowselbutton->GetValue()) {
		m_projnamesel->Disable();
		m_projstarttplsel->Disable();
		//m_filebrowsectl->Enable();
		m_projdirctrl->Enable();
		ProjAlias = wxString("IDH_ZBrowseStart");
	}
	event.Skip();
}



/*
 * wxEVT_COMMAND_TREE_SEL_CHANGED event handler for ID_PROJDIRCTRL
 */

void ConfigMgr::OnProjdirctrlSelChanged( wxTreeEvent& event )
{
	SetIniChoice();
	event.Skip();
}


/*
 * wxEVT_LEFT_DCLICK event handler for ID_PROJDIRCTRL
 */

void ConfigMgr::OnLeftDClick( wxMouseEvent& event )
{
////@begin wxEVT_LEFT_DCLICK event handler for ID_PROJDIRCTRL in ConfigMgr.
	// Before editing this code, remove the block markers.
	event.Skip();
////@end wxEVT_LEFT_DCLICK event handler for ID_PROJDIRCTRL in ConfigMgr. 
}



/*
 * wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_APPLYPROJ
 */

void ConfigMgr::OnApplyprojClick( wxCommandEvent& event )
{
	SetIniChoice();
	ApplyFiles();
	event.Skip();
}



// CATEGORY tab

/*
 * wxEVT_ENTER_WINDOW event handler for ID_CATPANEL
 */

void ConfigMgr::OnEnterCatWindow( wxMouseEvent& event )
{
	if (CMgr::CatPanelEnabled) {
		CMgr::CurrPanel = pncat;
		CatAlias = wxString("IDH_ZSelectCategory");
	}
	else
		(GetBookCtrl())->ChangeSelection((size_t) CMgr::CurrPanel);

	event.Skip();
}


/*
 * wxEVT_COMMAND_RADIOBUTTON_SELECTED event handler for ID_CATINITYPEBUTTON
 */

void ConfigMgr::OnCatinitypebuttonSelected( wxCommandEvent& event )
{
	if (m_catinitypebutton->GetValue()) {
		m_catinichoice->Enable();
		m_sectyptree->Disable();
	}
	event.Skip();
}


/*
 * wxEVT_COMMAND_CHOICE_SELECTED event handler for ID_CATINICHOICE
 */

void ConfigMgr::OnCatinichoiceSelected( wxCommandEvent& event )
{
	if (!CMgr::CmdLineCat) {
		CMgr::SelectedCategory = NULL;
		CMgr::SelAllTopCat = true;
		SelectIniType();
		event.Skip();
	}
}


/*
 * wxEVT_COMMAND_RADIOBUTTON_SELECTED event handler for ID_CATSELBUTTON
 */

void ConfigMgr::OnCatselbuttonSelected( wxCommandEvent& event )
{
	if (m_catselbutton->GetValue()) {
		m_catinichoice->Disable();
		m_sectyptree->Enable();
	}
	event.Skip();
}


/*
 * wxEVT_COMMAND_TREE_SEL_CHANGED event handler for ID_SECTYPTREE
 */

void ConfigMgr::OnSectyptreeSelChanged( wxTreeEvent& event )
{
	if (!CMgr::CmdLineCat) {
		CMgr::SelectedCategory = NULL;
		CMgr::SelAllTopCat = false;
		SelectCategories();
		event.Skip();
	}
}


/*
 * wxEVT_COMMAND_TREE_ITEM_ACTIVATED event handler for ID_SECTYPTREE
 */

void ConfigMgr::OnSectyptreeItemActivated( wxTreeEvent& event )
{
	if (!CMgr::CmdLineCat) {
		CMgr::SelectedCategory = NULL;
		CMgr::SelAllTopCat = false;
		SelectCategories();
		event.Skip();
	}
}


/*
 * wxEVT_COMMAND_RADIOBUTTON_SELECTED event handler for ID_CATFILEBUTTON
 */

void ConfigMgr::OnCatfilebuttonSelected( wxCommandEvent& event )
{
	CMgr::IncludeAllSections = false;
	event.Skip();
}


/*
 * wxEVT_COMMAND_RADIOBUTTON_SELECTED event handler for ID_CATALLBUTTON
 */

void ConfigMgr::OnCatallbuttonSelected( wxCommandEvent& event )
{
	CMgr::IncludeAllSections = true;
	event.Skip();
}


// wxTextCtrl* m_projstartsecttxt for section name,
// use IDH_ZSectionMatch

/*
 * wxEVT_COMMAND_TEXT_UPDATED event handler for ID_PROJSTARTSECTEXTCTRL
 */

void ConfigMgr::OnProjstartsectextctrlTextUpdated( wxCommandEvent& event )
{
	CatAlias = wxString("IDH_ZSectionMatch");
	event.Skip();
}


/*
 * wxEVT_COMMAND_TEXT_ENTER event handler for ID_PROJSTARTSECTEXTCTRL
 */

void ConfigMgr::OnProjstartsectextctrlEnter( wxCommandEvent& event )
{
	CatAlias = wxString("IDH_ZSectionMatch");
	event.Skip();
}


// wxTextCtrl* m_projstartsectxt for setting name
// use IDH_ZSettingMatch

/*
 * wxEVT_COMMAND_TEXT_UPDATED event handler for ID_PROJSTARTSETTEXTCTRL
 */

void ConfigMgr::OnProjstartsettextctrlTextUpdated( wxCommandEvent& event )
{
	CatAlias = wxString("IDH_ZSettingMatch");
	event.Skip();
}


/*
 * wxEVT_COMMAND_TEXT_ENTER event handler for ID_PROJSTARTSETTEXTCTRL
 */

void ConfigMgr::OnProjstartsettextctrlEnter( wxCommandEvent& event )
{
	CatAlias = wxString("IDH_ZSettingMatch");
	event.Skip();
}



/*
 * wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_APPLYCAT
 */

void ConfigMgr::OnApplycatClick( wxCommandEvent& event )
{
	ApplyCategories();
	event.Skip();
}


// ACTION tab



/*
 * wxEVT_ENTER_WINDOW event handler for ID_ACTPANEL
 */

void ConfigMgr::OnEnterActWindow( wxMouseEvent& event )
{
	if (CMgr::ActPanelEnabled)
		CMgr::CurrPanel = pnact;
	else
		(GetBookCtrl())->ChangeSelection((size_t) CMgr::CurrPanel);

	event.Skip();
}


/*
 * wxEVT_COMMAND_RADIOBUTTON_SELECTED event handler for ID_SECTADDBUTTON
 */

void ConfigMgr::OnSectaddbuttonSelected( wxCommandEvent& event )
{
	if (m_sectaddbutton->GetValue()) {
		CMgr::CurrAct = actadd;
		m_sectvnaddtext->Disable();
		m_sectvnrentext->Disable();
		CMgr::IncludeAllSections = true;
		m_actresult->Clear();
		ShowAction();
	}
	event.Skip();
}


/*
 * wxEVT_COMMAND_RADIOBUTTON_SELECTED event handler for ID_SECTVNADDBUTTON
 */

void ConfigMgr::OnSectvnaddbuttonSelected( wxCommandEvent& event )
{
	if (m_sectvnaddbutton->GetValue()) {
		CMgr::CurrAct = actaddv;
		m_sectvnaddtext->Enable();
		m_sectvnrentext->Disable();
		CMgr::IncludeAllSections = true;
		m_actresult->Clear();
		ShowAction();
	}
	event.Skip();
}


/*
 * wxEVT_COMMAND_RADIOBUTTON_SELECTED event handler for ID_SECTADDSETBUTTON
 */

void ConfigMgr::OnSectaddsetbuttonSelected( wxCommandEvent& event )
{
	if (m_sectaddsetbutton->GetValue()) {
		CMgr::CurrAct = actadds;
		CMgr::IncludeAllSettings = true;
		m_actresult->Clear();
		ShowAction();
	}
	event.Skip();
}


/*
 * wxEVT_COMMAND_RADIOBUTTON_SELECTED event handler for ID_SECTADDVKSETBUTTON
 */

void ConfigMgr::OnSectaddvksetbuttonSelected( wxCommandEvent& event )
{
	if (m_sectaddvksetbutton->GetValue()) {
		CMgr::CurrAct = actaddsv;
		CMgr::IncludeAllSettings = true;
		m_actresult->Clear();
		ShowAction();
	}
	event.Skip();
}


/*
 * wxEVT_COMMAND_RADIOBUTTON_SELECTED event handler for ID_SECTCHANGEBUTTON
 */

void ConfigMgr::OnSectchangebuttonSelected( wxCommandEvent& event )
{
	if (m_sectchangebutton->GetValue()) {
		CMgr::CurrAct = actedit;
		m_sectvnaddtext->Disable();
		m_sectvnrentext->Disable();
		m_actresult->Clear();
		ShowAction();
	}
	event.Skip();
}


/*
 * wxEVT_COMMAND_RADIOBUTTON_SELECTED event handler for ID_SECTSETEDITBUTTON
 */

void ConfigMgr::OnSectseteditbuttonSelected( wxCommandEvent& event )
{
	if (m_sectseteditbutton->GetValue()) {
		CMgr::CurrAct = actedits;
		m_actresult->Clear();
		ShowAction();
	}
	event.Skip();
}


/*
 * wxEVT_COMMAND_RADIOBUTTON_SELECTED event handler for ID_SECTDELBUTTON
 */

void ConfigMgr::OnSectdelbuttonSelected( wxCommandEvent& event )
{
	if (m_sectdelbutton->GetValue()) {
		CMgr::CurrAct = actdel;
		m_sectvnaddtext->Disable();
		m_sectvnrentext->Disable();
		m_actresult->Clear();
		ShowAction();
	}
	event.Skip();
}


/*
 * wxEVT_COMMAND_RADIOBUTTON_SELECTED event handler for ID_SECTDELSETBUTTON
 */

void ConfigMgr::OnSectdelsetbuttonSelected( wxCommandEvent& event )
{
	if (m_sectdelsetbutton->GetValue()) {
		CMgr::CurrAct = actdels;
		m_actresult->Clear();
		ShowAction();
	}
	event.Skip();
}


/*
 * wxEVT_COMMAND_RADIOBUTTON_SELECTED event handler for ID_SECTRESTBUTTON
 */

void ConfigMgr::OnSectrestbuttonSelected( wxCommandEvent& event )
{
	if (m_sectrestbutton->GetValue()) {
		CMgr::CurrAct = actrest;
		m_actresult->Clear();
		ShowAction();
	}
	event.Skip();
}


/*
 * wxEVT_COMMAND_RADIOBUTTON_SELECTED event handler for ID_RADIOBUTTON1
 */

void ConfigMgr::OnSectrestsetbuttonSelected( wxCommandEvent& event )
{
	if (m_sectrestsetbutton->GetValue()) {
		CMgr::CurrAct = actrests;
		m_actresult->Clear();
		ShowAction();
	}
	event.Skip();
}


/*
 * wxEVT_COMMAND_RADIOBUTTON_SELECTED event handler for ID_SECTRENBUTTON
 */

void ConfigMgr::OnSectrenbuttonSelected( wxCommandEvent& event )
{
	if (m_sectrenbutton->GetValue()) {
		CMgr::CurrAct = actrenv;
		m_sectvnrentext->Enable();
		m_sectvnaddtext->Disable();
		m_actresult->Clear();
		ShowAction();
	}
	event.Skip();
}


/*
 * wxEVT_COMMAND_RADIOBUTTON_SELECTED event handler for ID_SECTRENSETBUTTON
 */

void ConfigMgr::OnSectrensetbuttonSelected( wxCommandEvent& event )
{
	if (m_sectrensetbutton->GetValue()) {
		CMgr::CurrAct = actrensv;
		m_actresult->Clear();
		ShowAction();
	}
	event.Skip();
}


/*
 * wxEVT_COMMAND_RADIOBUTTON_SELECTED event handler for ID_SECTMOVBUTTON
 */

void ConfigMgr::OnSectmovbuttonSelected( wxCommandEvent& event )
{
	if (m_sectmovbutton->GetValue()) {
		CMgr::CurrAct = actmov;
		m_sectvnaddtext->Disable();
		m_sectvnrentext->Disable();
		m_actresult->Clear();
		ShowAction();
	}
	event.Skip();
}


/*
 * wxEVT_COMMAND_RADIOBUTTON_SELECTED event handler for ID_SECTMOVFBUTTON
 */

void ConfigMgr::OnSectmovfbuttonSelected( wxCommandEvent& event )
{
	if (m_sectmovfbutton->GetValue()) {
		CMgr::CurrAct = actmovf;
		m_sectvnaddtext->Disable();
		m_sectvnrentext->Disable();
		m_actresult->Clear();
		ShowAction();
	}
	event.Skip();
}


/*
 * wxEVT_COMMAND_RADIOBUTTON_SELECTED event handler for ID_SECTMOVSETBUTTON
 */

void ConfigMgr::OnSectmovsetbuttonSelected( wxCommandEvent& event )
{
	if (m_sectmovsetbutton->GetValue()) {
		CMgr::CurrAct = actmovs;
		m_actresult->Clear();
		ShowAction();
	}
	event.Skip();
}


/*
 * wxEVT_COMMAND_RADIOBUTTON_SELECTED event handler for ID_SECTMOVSETFBUTTON
 */

void ConfigMgr::OnSectmovsetfbuttonSelected( wxCommandEvent& event )
{
	if (m_sectmovsetfbutton->GetValue()) {
		CMgr::CurrAct = actmovsf;
		m_actresult->Clear();
		ShowAction();
	}
	event.Skip();
}


/*
 * wxEVT_COMMAND_RADIOBUTTON_SELECTED event handler for ID_SECTCPYBUTTON
 */

void ConfigMgr::OnSectcpybuttonSelected( wxCommandEvent& event )
{
	if (m_sectcpybutton->GetValue()) {
		CMgr::CurrAct = actcpy;
		m_actresult->Clear();
		ShowAction();
	}
	event.Skip();
}


/*
 * wxEVT_COMMAND_RADIOBUTTON_SELECTED event handler for ID_SECTCPYSETBUTTON
 */

void ConfigMgr::OnSectcpysetbuttonSelected( wxCommandEvent& event )
{
	if (m_sectcpysetfbutton->GetValue()) {
		CMgr::CurrAct = actcpys;
		m_actresult->Clear();
		ShowAction();
	}
	event.Skip();
}


/*
 * wxEVT_COMMAND_RADIOBUTTON_SELECTED event handler for ID_SECTDUPBUTTON
 */

void ConfigMgr::OnSectdupbuttonSelected( wxCommandEvent& event )
{
	if (m_sectdupbutton->GetValue()) {
		CMgr::CurrAct = actdup;
		m_actresult->Clear();
		ShowAction();
	}
	event.Skip();
}


/*
 * wxEVT_COMMAND_RADIOBUTTON_SELECTED event handler for ID_SECTDUPSETBUTTON
 */

void ConfigMgr::OnSectdupsetbuttonSelected( wxCommandEvent& event )
{
	if (m_sectdupsetbutton->GetValue()) {
		CMgr::CurrAct = actdups;
		m_actresult->Clear();
		ShowAction();
	}
	event.Skip();
}

// more actions here ****


/*
 * wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_APPLYACT
 */

void ConfigMgr::OnApplyactClick( wxCommandEvent& event )
{
	ApplyAction();
	event.Skip();
}



// SECTION tab


/*
 * wxEVT_ENTER_WINDOW event handler for ID_SECTIONPANEL
 */

void ConfigMgr::OnEnterSectWindow( wxMouseEvent& event )
{
	if (CMgr::SectPanelEnabled)
		CMgr::CurrPanel = pnsec;
	else
		(GetBookCtrl())->ChangeSelection((size_t) CMgr::CurrPanel);

	event.Skip();
}


/*
 * wxEVT_COMMAND_LIST_ITEM_SELECTED event handler for ID_SECTCTRL
 */

void ConfigMgr::OnSectctrlSelected( wxListEvent& event )
{
	SetSectButtons(event.GetIndex());
	event.Skip();
}


/*
 * wxEVT_COMMAND_LIST_ITEM_ACTIVATED event handler for ID_SECTCTRL
 */

void ConfigMgr::OnSectctrlItemActivated( wxListEvent& event )
{
	SetSectButtons(event.GetIndex());

	if (CMgr::ChildCM) {
		WriteResultFile(SectionSelected);
		EndChildCM();
	}
	else {
		ApplySections();
		event.Skip();
	}
}


/*
 * wxEVT_COMMAND_TEXT_UPDATED event handler for ID_SECTADDTEXT
 */

void ConfigMgr::OnSectaddtextTextUpdated( wxCommandEvent& event )
{
	DeselectSections();
	SectAlias = wxString("IDH_ZAddVNSection");
	event.Skip();
}


/*
 * wxEVT_COMMAND_TEXT_ENTER event handler for ID_SECTADDTEXT
 */

void ConfigMgr::OnSectaddtextEnter( wxCommandEvent& event )
{
	DeselectSections();
	SectAlias = wxString("IDH_ZAddVNSection");
	event.Skip();
}


/*
 * wxEVT_COMMAND_TEXT_UPDATED event handler for ID_RENSECTTEXT
 */

void ConfigMgr::OnSectvnrentextTextUpdated( wxCommandEvent& event )
{
	SectAlias = wxString("IDH_ZRenameVNSection");
	event.Skip();
}


/*
 * wxEVT_COMMAND_TEXT_ENTER event handler for ID_RENSECTTEXT
 */

void ConfigMgr::OnSectvnrentextEnter( wxCommandEvent& event )
{
	SectAlias = wxString("IDH_ZRenameVNSection");
	event.Skip();
}


/*
 * wxEVT_COMMAND_CHECKBOX_CLICKED event handler for ID_SECTFORMALIAS
 */

void ConfigMgr::OnSectformaliasClick( wxCommandEvent& event )
{
////@begin wxEVT_COMMAND_CHECKBOX_CLICKED event handler for ID_SECTFORMALIAS in ConfigMgr.
	// Before editing this code, remove the block markers.
	event.Skip();
////@end wxEVT_COMMAND_CHECKBOX_CLICKED event handler for ID_SECTFORMALIAS in ConfigMgr. 
}



/*
 * wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_APPLYSECT
 */

void ConfigMgr::OnApplysectClick( wxCommandEvent& event )
{
	ApplySections();
	event.Skip();
}

//  use m_sectlist->GetSelection() to get idx of selected item,
//  then (m_sectlist->GetString(idx)).c_str() to get text,
//  then look up in CurrSectionNames to get idx2,
//  then CurrSectionNameRefs where you get a DClist of idx3s
//  for all the CurrSections with that name in all inis



// SETTINGS tab


/*
 * wxEVT_ENTER_WINDOW event handler for ID_SETTINGPANEL
 */

void ConfigMgr::OnEnterSetWindow( wxMouseEvent& event )
{
	if (CMgr::SetPanelEnabled)
		CMgr::CurrPanel = pnset;
	else
		(GetBookCtrl())->ChangeSelection((size_t) CMgr::CurrPanel);

	event.Skip();
}


/*
 * wxEVT_COMMAND_LIST_ITEM_SELECTED event handler for ID_SETCTRL
 */

void ConfigMgr::OnSetctrlSelected( wxListEvent& event )
{
	SetSetButtons(event.GetIndex());
	event.Skip();
}


/*
 * wxEVT_COMMAND_LIST_ITEM_ACTIVATED event handler for ID_SETCTRL
 */

void ConfigMgr::OnSetctrlItemActivated( wxListEvent& event )
{
	SetSetButtons(event.GetIndex());
	if (CMgr::ChildCM) {
		WriteResultFile(SettingNameShown);
		EndChildCM();
	}
	else {
		ApplySettings();
		event.Skip();
	}
}


/*
 * wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_SETVARKEYBUTTON
 */

void ConfigMgr::OnSetvarkeybuttonClick( wxCommandEvent& event )
{
	if (!CMgr::ChildCM)
		ProcVarKeySelect();
	event.Skip();
}


/*
 * wxEVT_COMMAND_TEXT_UPDATED event handler for ID_ADDVKSETTEXT
 */

void ConfigMgr::OnSetvkaddtextTextUpdated( wxCommandEvent& event )
{
	DeselectSettings();
	SetAlias = wxString("IDH_ZAddVKSetting");
	event.Skip();
}


/*
 * wxEVT_COMMAND_TEXT_ENTER event handler for ID_ADDVKSETTEXT
 */

void ConfigMgr::OnSetvkaddtextEnter( wxCommandEvent& event )
{
	DeselectSettings();
	SetAlias = wxString("IDH_ZAddVKSetting");
	event.Skip();
}


/*
 * wxEVT_COMMAND_TEXT_UPDATED event handler for ID_RENVKSETTEXT
 */

void ConfigMgr::OnSetvkrentextTextUpdated( wxCommandEvent& event )
{
	SetAlias = wxString("IDH_ZRenameVKSetting");
	event.Skip();
}


/*
 * wxEVT_COMMAND_TEXT_ENTER event handler for ID_RENVKSETTEXT
 */

void ConfigMgr::OnSetvkrentextEnter( wxCommandEvent& event )
{
	SetAlias = wxString("IDH_ZRenameVKSetting");
	event.Skip();
}


/*
 * wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_APPLYSET
 */

void ConfigMgr::OnApplysetClick( wxCommandEvent& event )
{
	ApplySettings();
	event.Skip();
}



// INI-FILE tab


/*
 * wxEVT_ENTER_WINDOW event handler for ID_VALPANEL
 */

void ConfigMgr::OnEnterValWindow( wxMouseEvent& event )
{
	if (CMgr::ValPanelEnabled)
		CMgr::CurrPanel = pnini;
	else
		(GetBookCtrl())->ChangeSelection((size_t) CMgr::CurrPanel);

	event.Skip();
}


/*
 * wxEVT_COMMAND_LIST_ITEM_SELECTED event handler for ID_SELINICTRL
 */

void ConfigMgr::OnSelinictrlSelected( wxListEvent& event )
{
////@begin wxEVT_COMMAND_LIST_ITEM_SELECTED event handler for ID_SELINICTRL in ConfigMgr.
	// Before editing this code, remove the block markers.
	event.Skip();
////@end wxEVT_COMMAND_LIST_ITEM_SELECTED event handler for ID_SELINICTRL in ConfigMgr. 
}


/*
 * wxEVT_COMMAND_LIST_ITEM_DESELECTED event handler for ID_SELINICTRL
 */

void ConfigMgr::OnSelinictrlDeselected( wxListEvent& event )
{
////@begin wxEVT_COMMAND_LIST_ITEM_DESELECTED event handler for ID_SELINICTRL in ConfigMgr.
	// Before editing this code, remove the block markers.
	event.Skip();
////@end wxEVT_COMMAND_LIST_ITEM_DESELECTED event handler for ID_SELINICTRL in ConfigMgr. 
}


/*
 * wxEVT_COMMAND_LIST_ITEM_ACTIVATED event handler for ID_SELINICTRL
 */

void ConfigMgr::OnSelinictrlItemActivated( wxListEvent& event )
{
	if (CMgr::ValPanelEnabled)
		ApplyIniVals();
	event.Skip();
}





/*
 * wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_APPLYVAL
 */

void ConfigMgr::OnApplyvalClick( wxCommandEvent& event )
{
	ApplyIniVals();
	event.Skip();
}



// VALUE tab


/*
 * wxEVT_ENTER_WINDOW event handler for ID_VSELPANEL
 */

void ConfigMgr::OnEnterVselWindow( wxMouseEvent& event )
{
	if (CMgr::VselPanelEnabled)
		CMgr::CurrPanel = pnval;
	else
		(GetBookCtrl())->ChangeSelection((size_t) CMgr::CurrPanel);

	event.Skip();
}


/*
 * wxEVT_COMMAND_TEXT_ENTER event handler for ID_VSELVALTEXT
 */

void ConfigMgr::OnVselvaltextEnter( wxCommandEvent& event )
{
////@begin wxEVT_COMMAND_TEXT_ENTER event handler for ID_VSELVALTEXT in ConfigMgr.
	// Before editing this code, remove the block markers.
	event.Skip();
////@end wxEVT_COMMAND_TEXT_ENTER event handler for ID_VSELVALTEXT in ConfigMgr. 
}


/*
 * wxEVT_COMMAND_LIST_ITEM_SELECTED event handler for ID_SELVALCTRL
 */

void ConfigMgr::OnSelvalctrlSelected( wxListEvent& event )
{
	long pos = event.GetIndex();
	ProcSelValSelect(pos);
	event.Skip();
}


/*
 * wxEVT_COMMAND_LIST_ITEM_DESELECTED event handler for ID_SELVALCTRL
 */

void ConfigMgr::OnSelvalctrlDeselected( wxListEvent& event )
{
////@begin wxEVT_COMMAND_LIST_ITEM_DESELECTED event handler for ID_SELVALCTRL in ConfigMgr.
	// Before editing this code, remove the block markers.
	event.Skip();
////@end wxEVT_COMMAND_LIST_ITEM_DESELECTED event handler for ID_SELVALCTRL in ConfigMgr. 
}


/*
 * wxEVT_COMMAND_LIST_ITEM_ACTIVATED event handler for ID_SELVALCTRL
 */

void ConfigMgr::OnSelvalctrlItemActivated( wxListEvent& event )
{
	long pos = event.GetIndex();
	if (SettingValsAvailable.GetCount() == 1) {
		EditedValPos = pos;
		EditedSelVal.Clear();
		m_selvalctrl->EditLabel(EditedValPos);
	}
	else
		ProcSelValSelect(pos);
	event.Skip();
}


/*
 * wxEVT_COMMAND_LIST_BEGIN_LABEL_EDIT event handler for ID_SELVALCTRL
 */

void ConfigMgr::OnSelvalctrlBeginLabelEdit( wxListEvent& event )
{
	EditingVal = true;
	EditedValPos = event.GetIndex();
	event.Skip();
}


/*
 * wxEVT_COMMAND_LIST_END_LABEL_EDIT event handler for ID_SELVALCTRL
 */

void ConfigMgr::OnSelvalctrlEndLabelEdit( wxListEvent& event )
{
	m_selvalctrl->EndEditLabel(false);
	EditedSelVal = event.GetText();
	EditingVal = false;
	ValEdited();
	event.Skip();
}


/*
 * wxEVT_COMMAND_TEXT_ENTER event handler for ID_VSELMACTEXT
 */

void ConfigMgr::OnVselmactextEnter( wxCommandEvent& event )
{
////@begin wxEVT_COMMAND_TEXT_ENTER event handler for ID_VSELMACTEXT in ConfigMgr.
	// Before editing this code, remove the block markers.
	event.Skip();
////@end wxEVT_COMMAND_TEXT_ENTER event handler for ID_VSELMACTEXT in ConfigMgr. 
}


/*
 * wxEVT_COMMAND_TEXT_ENTER event handler for ID_VSELONETEXT
 */

void ConfigMgr::OnVselonetextEnter( wxCommandEvent& event )
{
////@begin wxEVT_COMMAND_TEXT_ENTER event handler for ID_VSELONETEXT in ConfigMgr.
	// Before editing this code, remove the block markers.
	event.Skip();
////@end wxEVT_COMMAND_TEXT_ENTER event handler for ID_VSELONETEXT in ConfigMgr. 
}


/*
 * wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_VSELVALBUTTON
 */

void ConfigMgr::OnVselvalbuttonClick( wxCommandEvent& event )
{
	if (!CMgr::ChildCM)
		ProcSelButtonSelect();
	event.Skip();
}


/*
 * wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_VSELUPBUTTON
 */

void ConfigMgr::OnVselupbuttonClick( wxCommandEvent& event )
{
	MoveSelVal(true);
	event.Skip();
}


/*
 * wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_VSELDNBUTTON
 */

void ConfigMgr::OnVseldnbuttonClick( wxCommandEvent& event )
{
	MoveSelVal(false);
	event.Skip();
}



/*
 * wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_APPLYVSEL
 */

void ConfigMgr::OnApplyvselClick( wxCommandEvent& event )
{
	ApplySelVals();
	event.Skip();
}



// PREFERENCES tab


/*
 * wxEVT_ENTER_WINDOW event handler for ID_SETTINGPANEL
 */

void ConfigMgr::OnEnterOptWindow( wxMouseEvent& event )
{
	OptAlias = wxString("IDH_ZTextColors");
	event.Skip();
}


/*
 * wxEVT_COMMAND_RADIOBUTTON_SELECTED event handler for ID_OPTSYSYES
 */

void ConfigMgr::OnOptsysyesSelected( wxCommandEvent& event )
{
	//SysIniSelect(true);
	event.Skip();
}


/*
 * wxEVT_COMMAND_RADIOBUTTON_SELECTED event handler for ID_OPTSYSNO
 */

void ConfigMgr::OnOptsysnoSelected( wxCommandEvent& event )
{
	//SysIniSelect(false);
	event.Skip();
}


// IDH_ZTextColors

/*
 * wxEVT_COLOURPICKER_CHANGED event handler for ID_OPTCOLORSECTCTRL
 */

void ConfigMgr::OnOptcolorsectctrlColourPickerChanged( wxColourPickerEvent& event )
{
	wxString otext = SectColor->GetAsString(wxC2S_CSS_SYNTAX);
	*SectColor = m_optcolorsectctrl->GetColour();
	wxString ntext = SectColor->GetAsString(wxC2S_CSS_SYNTAX);

	if (otext != ntext) {
		m_optcolorsecttext->SetForegroundColour(*SectColor);
		CMgr::SectColorText = NewName(ntext.c_str());

		DCpfile::putval("Colors", "AvailableSection", CMgr::SectColorText,
										CMgr::CMIniName);
		DCpfile::pclose(CMgr::CMIniName);
	}

	OptAlias = wxString("IDH_ZTextColors");

	event.Skip();
}


/*
 * wxEVT_COLOURPICKER_CHANGED event handler for ID_OPTCOLORSETCTRL
 */

void ConfigMgr::OnOptcolorsetctrlColourPickerChanged( wxColourPickerEvent& event )
{
	wxString otext = SetColor->GetAsString(wxC2S_CSS_SYNTAX);
	*SetColor = m_optcolorsetctrl->GetColour();
	wxString ntext = SetColor->GetAsString(wxC2S_CSS_SYNTAX);

	if (otext != ntext) {
		m_optcolorsettext->SetForegroundColour(*SetColor);
		CMgr::SetColorText = NewName(ntext.c_str());

		DCpfile::putval("Colors", "AvailableSetting", CMgr::SetColorText,
										CMgr::CMIniName);
		DCpfile::pclose(CMgr::CMIniName);
	}

	OptAlias = wxString("IDH_ZTextColors");

	event.Skip();
}


/*
 * wxEVT_COLOURPICKER_CHANGED event handler for ID_OPTCOLORVARCTRL
 */

void ConfigMgr::OnOptcolorvarctrlColourPickerChanged( wxColourPickerEvent& event )
{
	wxString otext = VarColor->GetAsString(wxC2S_CSS_SYNTAX);
	*VarColor = m_optcolorvarctrl->GetColour();
	wxString ntext = VarColor->GetAsString(wxC2S_CSS_SYNTAX);

	if (otext != ntext) {
		m_optcolorvartext->SetForegroundColour(*VarColor);
		CMgr::VarColorText = NewName(ntext.c_str());

		DCpfile::putval("Colors", "VariableNameKey", CMgr::VarColorText,
										CMgr::CMIniName);
		DCpfile::pclose(CMgr::CMIniName);
	}

	OptAlias = wxString("IDH_ZTextColors");

	event.Skip();
}


/*
 * wxEVT_COLOURPICKER_CHANGED event handler for ID_OPTCOLORDEFCTRL
 */

void ConfigMgr::OnOptcolordefctrlColourPickerChanged( wxColourPickerEvent& event )
{
	wxString otext = DefColor->GetAsString(wxC2S_CSS_SYNTAX);
	*DefColor = m_optcolordefctrl->GetColour();
	wxString ntext = DefColor->GetAsString(wxC2S_CSS_SYNTAX);

	if (otext != ntext) {
		m_optcolordeftext->SetForegroundColour(*DefColor);
		CMgr::DefColorText = NewName(ntext.c_str());

		DCpfile::putval("Colors", "ProgramDefault", CMgr::DefColorText,
										CMgr::CMIniName);
		DCpfile::pclose(CMgr::CMIniName);
	}

	OptAlias = wxString("IDH_ZTextColors");

	event.Skip();
}


/*
 * wxEVT_COLOURPICKER_CHANGED event handler for ID_OPTCOLORSYSCTRL
 */

void ConfigMgr::OnOptcolorsysctrlColourPickerChanged( wxColourPickerEvent& event )
{
	wxString otext = SysColor->GetAsString(wxC2S_CSS_SYNTAX);
	*SysColor = m_optcolorsysctrl->GetColour();
	wxString ntext = SysColor->GetAsString(wxC2S_CSS_SYNTAX);

	if (otext != ntext) {
		m_optcolorsystext->SetForegroundColour(*SysColor);
		CMgr::SysColorText = NewName(ntext.c_str());

		DCpfile::putval("Colors", "SystemIni", CMgr::SysColorText,
										CMgr::CMIniName);
		DCpfile::pclose(CMgr::CMIniName);
	}

	OptAlias = wxString("IDH_ZTextColors");

	event.Skip();
}



// IDH_ZAnnotate

/*
 * wxEVT_COMMAND_CHECKBOX_CLICKED event handler for ID_OPTADDINIHIST
 */

void ConfigMgr::OnOptaddinihistClick( wxCommandEvent& event )
{
	bool oldhist = CMgr::AddIniHistory;
	CMgr::AddIniHistory = m_optaddinihist->GetValue();
	if (oldhist != CMgr::AddIniHistory)
		CMgr::CMIniChanged = true;
	OptAlias = wxString("IDH_ZAnnotate");

	event.Skip();
}


/*
 * wxEVT_COMMAND_TEXT_UPDATED event handler for ID_OPTUSERNAMETEXT
 */

void ConfigMgr::OnOptusernametextTextUpdated( wxCommandEvent& event )
{
	wxString name = m_optusernametext->GetValue();
	if (strcmp(name.c_str(), CMgr::UserName)) {
		CMgr::UserName = NewName(name.c_str());
		CMgr::CMIniChanged = true;
	}
	OptAlias = wxString("IDH_ZAnnotate");

	event.Skip();
}


/*
 * wxEVT_COMMAND_TEXT_ENTER event handler for ID_OPTUSERNAMETEXT
 */

void ConfigMgr::OnOptusernametextEnter( wxCommandEvent& event )
{
	wxString name = event.GetString();
	if (strcmp(name.c_str(), CMgr::UserName)) {
		CMgr::UserName = NewName(name.c_str());
		CMgr::CMIniChanged = true;
	}
	OptAlias = wxString("IDH_ZAnnotate");

	event.Skip();
}


/*
 * wxEVT_COMMAND_COMBOBOX_SELECTED event handler for ID_OPTBROWSERCTRL
 */

void ConfigMgr::OnOptbrowserctrlSelected( wxCommandEvent& event )
{
	CMgr::Browser = NewName((m_optbrowserctrl->GetValue()).c_str());
	event.Skip();
}


/*
 * wxEVT_COMMAND_TEXT_ENTER event handler for ID_OPTBROWSERCTRL
 */

void ConfigMgr::OnOptbrowserctrlEnter( wxCommandEvent& event )
{
	CMgr::Browser = NewName((m_optbrowserctrl->GetValue()).c_str());
	event.Skip();
}

// END of dialog panels


// Help buttons for all panels

/*
 * wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_PROJHELPBUTTON
 */

void ConfigMgr::OnProjhelpbuttonClick( wxCommandEvent& event )
{
	CallHelp(ProjAlias);	
	event.Skip();
}


/*
 * wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_CATHELPBUTTON
 */

void ConfigMgr::OnCathelpbuttonClick( wxCommandEvent& event )
{
	CallHelp(CatAlias);	
	event.Skip();
}


/*
 * wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_ACTHELPBUTTON
 */

void ConfigMgr::OnActhelpbuttonClick( wxCommandEvent& event )
{
	CallHelp(ActAlias);	
	event.Skip();
}


/*
 * wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_SECTHELPBUTTON
 */

void ConfigMgr::OnSecthelpbuttonClick( wxCommandEvent& event )
{
	CallHelp(SectAlias);	
	event.Skip();
}


/*
 * wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_SETHELPBUTTON
 */

void ConfigMgr::OnSethelpbuttonClick( wxCommandEvent& event )
{
	CallHelp(SetAlias);	
	event.Skip();
}


/*
 * wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_VALHELPBUTTON
 */

void ConfigMgr::OnValhelpbuttonClick( wxCommandEvent& event )
{
	CallHelp(ValAlias);	
	event.Skip();
}


/*
 * wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_VSELHELPBUTTON
 */

void ConfigMgr::OnVselhelpbuttonClick( wxCommandEvent& event )
{
	CallHelp(ValAlias);	
	event.Skip();
}


/*
 * wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_BUTTON1
 */

void ConfigMgr::OnOpthelpbuttonClick( wxCommandEvent& event )
{
	CallHelp(OptAlias);	
	event.Skip();
}



// END of Help buttons






