/////////////////////////////////////////////////////////////////////////////
// Name:        d2gmain.h
// Purpose:     
// Author:      Jeremy H. Griffith
// Modified by: 
// Created:     02/12/2009 16:01:53
// RCS-ID:      
// Copyright:   (c) Copyright 2009 Omni Systems, Inc.
// Licence:     
/////////////////////////////////////////////////////////////////////////////

#ifndef _D2GMAIN_H_
#define _D2GMAIN_H_


/*!
 * Includes
 */

////@begin includes
#include "wx/propdlg.h"
#include "wx/filepicker.h"
////@end includes

#include "optrest.h"
#include "initoedit.h"


/*!
 * Forward declarations
 */

////@begin forward declarations
class wxFilePickerCtrl;
class wxDirPickerCtrl;
////@end forward declarations

class Configstart;

/*!
 * Control identifiers
 */

////@begin control identifiers
#define ID_D2GMAIN 10000
#define ID_RUNPANEL 10001
#define ID_RUNPROJECT 10015
#define ID_RUNPROJECTSEL 10016
#define ID_RUNSRCDIR 10049
#define ID_RUNSRCDIRTEXT 10050
#define ID_RUNSRCMAP 10017
#define ID_RUNSRCMAPSEL 10018
#define ID_RUNFORMAT 10022
#define ID_RUNFORMATTEXT 10023
#define ID_RUNOUTDIR 10024
#define ID_RUNOUTDIRTEXT 10025
#define ID_RUNSTART 10020
#define ID_RUNSTOP 10021
#define ID_RUNCM 10133
#define ID_RUNEDITINI 10071
#define ID_RUNSHOWLOG 10088
#define ID_RUNVIEWOUT 10128
#define ID_RUNRESULT 10029
#define ID_RUNRESULTTEXT 10033
#define ID_RUNOPTPANEL 10081
#define ID_RUNOPTAUTO 10120
#define ID_RUNWAS 10080
#define ID_RUNWASYES 10117
#define ID_RUNWASNO 10118
#define ID_RUNCOMP 10057
#define ID_RUNCOMPYES 10059
#define ID_RUNCOMPNO 10061
#define ID_RUNCWIN 10122
#define ID_RUNCWINYES 10123
#define ID_RUNCWINNO 10124
#define ID_RUNFTS 10077
#define ID_RUNFTSYES 10078
#define ID_RUNFTSNO 10079
#define ID_RUNJAR 10084
#define ID_RUNJARYES 10085
#define ID_RUNJARNO 10086
#define ID_RUNARCH 10087
#define ID_RUNARCHYES 10089
#define ID_RUNARCHNO 10090
#define ID_RUNSHIP 10091
#define ID_RUNSHIPYES 10092
#define ID_RUNSHIPNO 10093
#define ID_RUNOPTADV 10121
#define ID_RUNDCL 10094
#define ID_RUNDCLSRC 10102
#define ID_RUNDCLOUT 10119
#define ID_RUNDCLNO 10103
#define ID_RUNDCLYES 10129
#define ID_RUNONLY 10125
#define ID_RUNONLYYES 10126
#define ID_RUNONLYNO 10127
#define ID_CREATEPANEL 10006
#define ID_PROJECTNAME 10011
#define ID_PROJECTSEL 10003
#define ID_SOURCEDIR 10052
#define ID_SRCDIRSELECT 10053
#define ID_SOURCEMAP 10014
#define ID_SRCMAPDEFSEL 10005
#define ID_FORMATNAME 10012
#define ID_FORMATSEL 10002
#define ID_OUTPUTDIR 10013
#define ID_OUTDIRSELECT 10004
#define ID_CREATEPROJECT 10008
#define ID_INICONFIG 10116
#define ID_CREATECM 10134
#define ID_EDITINI 10044
#define ID_RESULT 10009
#define ID_RESULTTEXT 10010
#define ID_MODIFYPANEL 10060
#define ID_MPROJECTNAME 10007
#define ID_MPROJECTSEL 10026
#define ID_MSOURCEDIR 10030
#define ID_MSRCDIRSELECT 10031
#define ID_MSOURCEMAP 10032
#define ID_MSRCMAPDEFSEL 10104
#define ID_MFORMATNAME 10034
#define ID_MFORMATSEL 10035
#define ID_MOUTPUTDIR 10036
#define ID_MOUTDIRSELECT 10037
#define ID_MODIFYPROJECT 10040
#define ID_MINICONFIG 10083
#define ID_MODIFYCM 10135
#define ID_MEDITINI 10115
#define ID_MRESULT 10027
#define ID_MRESULTTEXT 10028
#define ID_DELETEPANEL 10039
#define ID_DPROJECTNAME 10112
#define ID_DPROJECTSEL 10106
#define ID_DSOURCEDIR 10113
#define ID_DSRCDIRTEXT 10114
#define ID_DSOURCEMAP 10109
#define ID_DSRCMAPTEXT 10051
#define ID_DFORMATNAME 10110
#define ID_DFORMATTEXT 10058
#define ID_DOUTPUTDIR 10111
#define ID_DOUTDIRTEXT 10105
#define ID_DELETEPROJECT 10041
#define ID_DRESULT 10107
#define ID_DRESULTTEXT 10108
#define ID_RESTOREPANEL 10045
#define ID_RPROJECTNAME 10042
#define ID_RPROJECTSEL 10055
#define ID_RSOURCEDIR 10063
#define ID_RSRCDIRTEXT 10043
#define ID_RSOURCEMAP 10065
#define ID_RSRCMAPTEXT 10066
#define ID_RFORMATNAME 10067
#define ID_RFORMATTEXT 10068
#define ID_ROUTPUTDIR 10069
#define ID_ROUTDIRTEXT 10070
#define ID_RESTOREPROJECT 10038
#define ID_RRESULT 10073
#define ID_RRESULTTEXT 10074
#define ID_OPTIONSPANEL 10046
#define ID_OPTEDITOR 10054
#define ID_OPTEDITORTEXT 10056
#define ID_OPTSHDEL 10132
#define ID_OPTSHDELYES 10130
#define ID_OPTSHDELNO 10131
#define ID_OPTSHOW 10047
#define ID_OPTSHOWCON 10048
#define ID_OPTSHOWLOG 10019
#define ID_OPTLOGFILE 10072
#define ID_OPTLOGFILETEXT 10075
#define ID_OPTNEWINI 10062
#define ID_OPTNEWINISEL 10064
#define ID_OPTPROJINI 10095
#define ID_OPTPROJINID2GBUT 10100
#define ID_OPTPROJINILOCBUT 10101
#define ID_OPTPROJINID2G 10096
#define ID_OPTPROJINID2GSEL 10097
#define ID_OPTPROJINILOC 10098
#define ID_OPTPROJINILOCTEXT 10099
#define ID_OPTSET 10076
#define ID_OPTRESET 10082
#define SYMBOL_D2GMAIN_STYLE wxCAPTION|wxRESIZE_BORDER|wxSYSTEM_MENU|wxCLOSE_BOX|wxMINIMIZE_BOX
#ifdef DITA
#define SYMBOL_D2GMAIN_TITLE _("DITA2Go Project Manager")
#endif
#ifdef UDOC
#define SYMBOL_D2GMAIN_TITLE _("uDoc2Go Project Manager")
#endif
#define SYMBOL_D2GMAIN_IDNAME ID_D2GMAIN
#define SYMBOL_D2GMAIN_SIZE wxSize(575, -1)
#define SYMBOL_D2GMAIN_POSITION wxDefaultPosition
////@end control identifiers


/*!
 * D2gmain class declaration
 */

class D2gmain: public wxPropertySheetDialog
{    
	DECLARE_DYNAMIC_CLASS( D2gmain )
	DECLARE_EVENT_TABLE()

public:
	/// Constructors
	D2gmain();
	D2gmain( wxWindow* parent, wxWindowID id = SYMBOL_D2GMAIN_IDNAME, const wxString& caption = SYMBOL_D2GMAIN_TITLE, const wxPoint& pos = SYMBOL_D2GMAIN_POSITION, const wxSize& size = SYMBOL_D2GMAIN_SIZE, long style = SYMBOL_D2GMAIN_STYLE );

	/// Creation
	bool Create( wxWindow* parent, wxWindowID id = SYMBOL_D2GMAIN_IDNAME, const wxString& caption = SYMBOL_D2GMAIN_TITLE, const wxPoint& pos = SYMBOL_D2GMAIN_POSITION, const wxSize& size = SYMBOL_D2GMAIN_SIZE, long style = SYMBOL_D2GMAIN_STYLE );

	/// Destructor
	~D2gmain();

	/// Initialises member variables
	void Init();

	/// Creates the controls and sizers
	void CreateControls();

////@begin D2gmain event handler declarations

	/// wxEVT_INIT_DIALOG event handler for ID_D2GMAIN
	void OnInitDialog( wxInitDialogEvent& event );

	/// wxEVT_INIT_DIALOG event handler for ID_RUNPANEL
	void OnInitRunDialog( wxInitDialogEvent& event );

	/// wxEVT_COMMAND_CHOICE_SELECTED event handler for ID_RUNPROJECTSEL
	void OnRunprojectselSelected( wxCommandEvent& event );

	/// wxEVT_UPDATE_UI event handler for ID_RUNPROJECTSEL
	void OnRunprojectselUpdate( wxUpdateUIEvent& event );

	/// wxEVT_FILEPICKER_CHANGED event handler for ID_RUNSRCMAPSEL
	void OnRunsrcmapselFilePickerChanged( wxFileDirPickerEvent& event );

	/// wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_RUNSTART
	void OnRunstartClick( wxCommandEvent& event );

	/// wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_RUNSTOP
	void OnRunstopClick( wxCommandEvent& event );

	/// wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_RUNCM
	void OnRuncmClick( wxCommandEvent& event );

	/// wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_RUNEDITINI
	void OnRuneditiniClick( wxCommandEvent& event );

	/// wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_RUNSHOWLOG
	void OnRunshowlogClick( wxCommandEvent& event );

	/// wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_RUNVIEWOUT
	void OnRunviewoutClick( wxCommandEvent& event );

	/// wxEVT_COMMAND_RADIOBUTTON_SELECTED event handler for ID_RUNWASYES
	void OnRunwasyesSelected( wxCommandEvent& event );

	/// wxEVT_COMMAND_RADIOBUTTON_SELECTED event handler for ID_RUNWASNO
	void OnRunwasnoSelected( wxCommandEvent& event );

	/// wxEVT_COMMAND_RADIOBUTTON_SELECTED event handler for ID_RUNCOMPYES
	void OnRuncompyesSelected( wxCommandEvent& event );

	/// wxEVT_COMMAND_RADIOBUTTON_SELECTED event handler for ID_RUNCOMPNO
	void OnRuncompnoSelected( wxCommandEvent& event );

	/// wxEVT_COMMAND_RADIOBUTTON_SELECTED event handler for ID_RUNCWINYES
	void OnRuncwinyesSelected( wxCommandEvent& event );

	/// wxEVT_COMMAND_RADIOBUTTON_SELECTED event handler for ID_RUNCWINNO
	void OnRuncwinnoSelected( wxCommandEvent& event );

	/// wxEVT_COMMAND_RADIOBUTTON_SELECTED event handler for ID_RUNFTSYES
	void OnRunftsyesSelected( wxCommandEvent& event );

	/// wxEVT_COMMAND_RADIOBUTTON_SELECTED event handler for ID_RUNFTSNO
	void OnRunftsnoSelected( wxCommandEvent& event );

	/// wxEVT_COMMAND_RADIOBUTTON_SELECTED event handler for ID_RUNJARYES
	void OnRunjaryesSelected( wxCommandEvent& event );

	/// wxEVT_COMMAND_RADIOBUTTON_SELECTED event handler for ID_RUNJARNO
	void OnRunjarnoSelected( wxCommandEvent& event );

	/// wxEVT_COMMAND_RADIOBUTTON_SELECTED event handler for ID_RUNARCHYES
	void OnRunarchyesSelected( wxCommandEvent& event );

	/// wxEVT_COMMAND_RADIOBUTTON_SELECTED event handler for ID_RUNARCHNO
	void OnRunarchnoSelected( wxCommandEvent& event );

	/// wxEVT_COMMAND_RADIOBUTTON_SELECTED event handler for ID_RUNSHIPYES
	void OnRunshipyesSelected( wxCommandEvent& event );

	/// wxEVT_COMMAND_RADIOBUTTON_SELECTED event handler for ID_RUNSHIPNO
	void OnRunshipnoSelected( wxCommandEvent& event );

	/// wxEVT_COMMAND_RADIOBUTTON_SELECTED event handler for ID_RUNDCLSRC
	void OnRundclsrcSelected( wxCommandEvent& event );

	/// wxEVT_COMMAND_RADIOBUTTON_SELECTED event handler for ID_RUNDCLOUT
	void OnRundcloutSelected( wxCommandEvent& event );

	/// wxEVT_COMMAND_RADIOBUTTON_SELECTED event handler for ID_RUNDCLNO
	void OnRundclnoSelected( wxCommandEvent& event );

	/// wxEVT_COMMAND_RADIOBUTTON_SELECTED event handler for ID_RUNDCLYES
	void OnRundclyesSelected( wxCommandEvent& event );

	/// wxEVT_COMMAND_RADIOBUTTON_SELECTED event handler for ID_RUNONLYYES
	void OnRunonlyyesSelected( wxCommandEvent& event );

	/// wxEVT_COMMAND_RADIOBUTTON_SELECTED event handler for ID_RUNONLYNO
	void OnRunonlynoSelected( wxCommandEvent& event );

	/// wxEVT_INIT_DIALOG event handler for ID_CREATEPANEL
	void OnInitSetupDialog( wxInitDialogEvent& event );

	/// wxEVT_COMMAND_COMBOBOX_SELECTED event handler for ID_PROJECTSEL
	void OnProjectselSelected( wxCommandEvent& event );

	/// wxEVT_COMMAND_TEXT_ENTER event handler for ID_PROJECTSEL
	void OnProjectselEnter( wxCommandEvent& event );

	/// wxEVT_UPDATE_UI event handler for ID_PROJECTSEL
	void OnProjectselUpdate( wxUpdateUIEvent& event );

	/// wxEVT_DIRPICKER_CHANGED event handler for ID_SRCDIRSELECT
	void OnSrcdirselectDirPickerChanged( wxFileDirPickerEvent& event );

	/// wxEVT_FILEPICKER_CHANGED event handler for ID_SRCMAPDEFSEL
	void OnSrcmapdefselFilePickerChanged( wxFileDirPickerEvent& event );

	/// wxEVT_COMMAND_CHOICE_SELECTED event handler for ID_FORMATSEL
	void OnFormatselSelected( wxCommandEvent& event );

	/// wxEVT_UPDATE_UI event handler for ID_FORMATSEL
	void OnFormatselUpdate( wxUpdateUIEvent& event );

	/// wxEVT_DIRPICKER_CHANGED event handler for ID_OUTDIRSELECT
	void OnOutdirselectDirPickerChanged( wxFileDirPickerEvent& event );

	/// wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_CREATEPROJECT
	void OnCreateprojectClick( wxCommandEvent& event );

	/// wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_INICONFIG
	void OnIniconfigClick( wxCommandEvent& event );

	/// wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_CREATECM
	void OnCreatecmClick( wxCommandEvent& event );

	/// wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_EDITINI
	void OnEditiniClick( wxCommandEvent& event );

	/// wxEVT_INIT_DIALOG event handler for ID_MODIFYPANEL
	void OnInitModifyDialog( wxInitDialogEvent& event );

	/// wxEVT_COMMAND_CHOICE_SELECTED event handler for ID_MPROJECTSEL
	void OnMprojectselSelected( wxCommandEvent& event );

	/// wxEVT_UPDATE_UI event handler for ID_MPROJECTSEL
	void OnMprojectselUpdate( wxUpdateUIEvent& event );

	/// wxEVT_DIRPICKER_CHANGED event handler for ID_MSRCDIRSELECT
	void OnMsrcdirselectDirPickerChanged( wxFileDirPickerEvent& event );

	/// wxEVT_FILEPICKER_CHANGED event handler for ID_MSRCMAPDEFSEL
	void OnMsrcmapdefselFilePickerChanged( wxFileDirPickerEvent& event );

	/// wxEVT_COMMAND_CHOICE_SELECTED event handler for ID_MFORMATSEL
	void OnMformatselSelected( wxCommandEvent& event );

	/// wxEVT_UPDATE_UI event handler for ID_MFORMATSEL
	void OnMformatselUpdate( wxUpdateUIEvent& event );

	/// wxEVT_DIRPICKER_CHANGED event handler for ID_MOUTDIRSELECT
	void OnMoutdirselectDirPickerChanged( wxFileDirPickerEvent& event );

	/// wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_MODIFYPROJECT
	void OnModifyprojectClick( wxCommandEvent& event );

	/// wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_MINICONFIG
	void OnMiniconfigClick( wxCommandEvent& event );

	/// wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_MODIFYCM
	void OnModifycmClick( wxCommandEvent& event );

	/// wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_MEDITINI
	void OnMeditiniClick( wxCommandEvent& event );

	/// wxEVT_INIT_DIALOG event handler for ID_DELETEPANEL
	void OnInitDeleteDialog( wxInitDialogEvent& event );

	/// wxEVT_COMMAND_CHOICE_SELECTED event handler for ID_DPROJECTSEL
	void OnDprojectselSelected( wxCommandEvent& event );

	/// wxEVT_UPDATE_UI event handler for ID_DPROJECTSEL
	void OnDprojectselUpdate( wxUpdateUIEvent& event );

	/// wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_DELETEPROJECT
	void OnDeleteprojectClick( wxCommandEvent& event );

	/// wxEVT_INIT_DIALOG event handler for ID_RESTOREPANEL
	void OnInitRestoreDialog( wxInitDialogEvent& event );

	/// wxEVT_COMMAND_CHOICE_SELECTED event handler for ID_RPROJECTSEL
	void OnRprojectselSelected( wxCommandEvent& event );

	/// wxEVT_UPDATE_UI event handler for ID_RPROJECTSEL
	void OnRprojectselUpdate( wxUpdateUIEvent& event );

	/// wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_RESTOREPROJECT
	void OnRestoreprojectClick( wxCommandEvent& event );

	/// wxEVT_INIT_DIALOG event handler for ID_OPTIONSPANEL
	void OnInitOptionsDialog( wxInitDialogEvent& event );

	/// wxEVT_COMMAND_TEXT_UPDATED event handler for ID_OPTEDITORTEXT
	void OnOpteditortextTextUpdated( wxCommandEvent& event );

	/// wxEVT_COMMAND_TEXT_ENTER event handler for ID_OPTEDITORTEXT
	void OnOpteditortextEnter( wxCommandEvent& event );

	/// wxEVT_COMMAND_RADIOBUTTON_SELECTED event handler for ID_OPTSHDELYES
	void OnOptshdelyesSelected( wxCommandEvent& event );

	/// wxEVT_COMMAND_RADIOBUTTON_SELECTED event handler for ID_OPTSHDELNO
	void OnOptshdelnoSelected( wxCommandEvent& event );

	/// wxEVT_COMMAND_CHECKBOX_CLICKED event handler for ID_OPTSHOWCON
	void OnOptshowconClick( wxCommandEvent& event );

	/// wxEVT_COMMAND_CHECKBOX_CLICKED event handler for ID_OPTSHOWLOG
	void OnOptshowlogClick( wxCommandEvent& event );

	/// wxEVT_COMMAND_TEXT_UPDATED event handler for ID_OPTLOGFILETEXT
	void OnOptlogfiletextTextUpdated( wxCommandEvent& event );

	/// wxEVT_COMMAND_TEXT_ENTER event handler for ID_OPTLOGFILETEXT
	void OnOptlogfiletextEnter( wxCommandEvent& event );

	/// wxEVT_DIRPICKER_CHANGED event handler for ID_OPTNEWINISEL
	void OnOptnewiniselDirPickerChanged( wxFileDirPickerEvent& event );

	/// wxEVT_CHAR event handler for ID_OPTNEWINISEL
	void OnChar( wxKeyEvent& event );

	/// wxEVT_COMMAND_RADIOBUTTON_SELECTED event handler for ID_OPTPROJINID2GBUT
	void OnOptprojinid2gbutSelected( wxCommandEvent& event );

	/// wxEVT_COMMAND_RADIOBUTTON_SELECTED event handler for ID_OPTPROJINILOCBUT
	void OnOptprojinilocbutSelected( wxCommandEvent& event );

	/// wxEVT_DIRPICKER_CHANGED event handler for ID_OPTPROJINID2GSEL
	void OnOptprojinid2gselDirPickerChanged( wxFileDirPickerEvent& event );

	/// wxEVT_COMMAND_TEXT_UPDATED event handler for ID_OPTPROJINILOCTEXT
	void OnOptprojiniloctextTextUpdated( wxCommandEvent& event );

	/// wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_OPTSET
	void OnOptsetClick( wxCommandEvent& event );

	/// wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_OPTRESET
	void OnOptresetClick( wxCommandEvent& event );

////@end D2gmain event handler declarations

////@begin D2gmain member function declarations

	/// Retrieves bitmap resources
	wxBitmap GetBitmapResource( const wxString& name );

	/// Retrieves icon resources
	wxIcon GetIconResource( const wxString& name );
////@end D2gmain member function declarations

	/// Should we show tooltips?
	static bool ShowToolTips();

////@begin D2gmain member variables
	wxPanel* m_runpanel;
	wxStaticText* m_runproject;
	wxChoice* m_runprojectsel;
	wxStaticText* m_runsrcdir;
	wxTextCtrl* m_runsrcdirtext;
	wxStaticText* m_runsrcmap;
	wxFilePickerCtrl* m_runsrcmapsel;
	wxStaticText* m_runformat;
	wxTextCtrl* m_runformattext;
	wxStaticText* m_runoutputdir;
	wxTextCtrl* m_runoutdirtext;
	wxButton* m_runstart;
	wxButton* m_runstop;
	wxButton* m_runcm;
	wxButton* m_runeditini;
	wxButton* m_runshowlog;
	wxButton* m_runviewout;
	wxStaticText* m_runresult;
	wxTextCtrl* m_runresulttext;
	wxPanel* m_runoptpanel;
	wxStaticText* m_runoptauto;
	wxStaticText* m_runwas;
	wxRadioButton* m_runwasyes;
	wxRadioButton* m_runwasno;
	wxStaticText* m_runcomp;
	wxRadioButton* m_runcompyes;
	wxRadioButton* m_runcompno;
	wxStaticText* m_runcwin;
	wxRadioButton* m_runcwinyes;
	wxRadioButton* m_runcwinno;
	wxStaticText* m_runfts;
	wxRadioButton* m_runftsyes;
	wxRadioButton* m_runftsno;
	wxStaticText* m_runjar;
	wxRadioButton* m_runjaryes;
	wxRadioButton* m_runjarno;
	wxStaticText* m_runarch;
	wxRadioButton* m_runarchyes;
	wxRadioButton* m_runarchno;
	wxStaticText* m_runship;
	wxRadioButton* m_runshipyes;
	wxRadioButton* m_runshipno;
	wxStaticText* m_runoptadv;
	wxStaticText* m_rundcl;
	wxRadioButton* m_rundclsrc;
	wxRadioButton* m_rundclout;
	wxRadioButton* m_rundclno;
	wxRadioButton* m_rundclyes;
	wxStaticText* m_runonly;
	wxRadioButton* m_runonlyyes;
	wxRadioButton* m_runonlyno;
	wxPanel* m_createpanel;
	wxStaticText* m_projectname;
	wxComboBox* m_projectselect;
	wxStaticText* m_sourcedir;
	wxDirPickerCtrl* m_srcdirselect;
	wxStaticText* m_sourcemap;
	wxFilePickerCtrl* m_srcmapselect;
	wxStaticText* m_formatname;
	wxChoice* m_formatselect;
	wxStaticText* m_outputdir;
	wxDirPickerCtrl* m_outdirselect;
	wxButton* m_createproject;
	wxButton* m_iniconfig;
	wxButton* m_createcm;
	wxButton* m_editini;
	wxStaticText* m_result;
	wxTextCtrl* m_resulttext;
	wxPanel* m_modifypanel;
	wxStaticText* m_mprojectname;
	wxChoice* m_mprojectselect;
	wxStaticText* m_msourcedir;
	wxDirPickerCtrl* m_msrcdirselect;
	wxStaticText* m_msourcemap;
	wxFilePickerCtrl* m_msrcmapselect;
	wxStaticText* m_mformatname;
	wxChoice* m_mformatselect;
	wxStaticText* m_moutputdir;
	wxDirPickerCtrl* m_moutdirselect;
	wxButton* m_modifyproject;
	wxButton* m_miniconfig;
	wxButton* m_modifycm;
	wxButton* m_meditini;
	wxStaticText* m_mresult;
	wxTextCtrl* m_mresulttext;
	wxPanel* m_deletepanel;
	wxStaticText* m_dprojectname;
	wxChoice* m_dprojectselect;
	wxStaticText* m_dsourcedir;
	wxTextCtrl* m_dsrcdirtext;
	wxStaticText* m_dsourcemap;
	wxTextCtrl* m_dsrcmaptext;
	wxStaticText* m_dformatname;
	wxTextCtrl* m_dformattext;
	wxStaticText* m_doutputdir;
	wxTextCtrl* m_doutdirtext;
	wxButton* m_deleteproject;
	wxStaticText* m_dresult;
	wxTextCtrl* m_dresulttext;
	wxPanel* m_restorepanel;
	wxStaticText* m_rprojectname;
	wxChoice* m_rprojectselect;
	wxStaticText* m_rsourcedir;
	wxTextCtrl* m_rsrcdirtext;
	wxStaticText* m_rsourcemap;
	wxTextCtrl* m_rsrcmaptext;
	wxStaticText* m_rformatname;
	wxTextCtrl* m_rformattext;
	wxStaticText* m_routputdir;
	wxTextCtrl* m_routdirtext;
	wxButton* m_restoreproject;
	wxStaticText* m_rresult;
	wxTextCtrl* m_rresulttext;
	wxPanel* m_optionspanel;
	wxStaticText* m_opteditor;
	wxTextCtrl* m_opteditortext;
	wxStaticText* m_optshdel;
	wxRadioButton* m_optshdelyes;
	wxRadioButton* m_optshdelno;
	wxStaticText* m_optshow;
	wxCheckBox* m_optshowcon;
	wxCheckBox* m_optshowlog;
	wxStaticText* m_optlogfile;
	wxTextCtrl* m_optlogfiletext;
	wxStaticText* m_optnewini;
	wxDirPickerCtrl* m_optnewinisel;
	wxStaticText* m_optprojini;
	wxRadioButton* m_optprojinid2gbut;
	wxRadioButton* m_optprojinilocbut;
	wxStaticText* m_optprojinid2g;
	wxDirPickerCtrl* m_optprojinid2gsel;
	wxStaticText* m_optprojiniloc;
	wxTextCtrl* m_optprojiniloctext;
	wxButton* m_optset;
	wxButton* m_optreset;
////@end D2gmain member variables

	// added variables used in d2gdcl.cpp
	long d2gpmVersion;
	bool UMXL;
	wxPoint *InitPos;
	wxSize *InitSize;

	wxString *CurrProjectID;
	long NextProjectID;
	wxString *NewProjectID;
	wxString *CurrDelProjectID;
	wxString *CurrProject;
	wxString *CurrDelProject;
	wxArrayString *ActiveProjects;
	wxArrayString *DeletedProjects;

	bool Creating;
	bool Modifying;
	bool Running;

	bool ProjectsChanged;
	bool CurrMProjectChanged;
	bool CurrDProjectChanged;
	bool CurrRProjectChanged;
	bool RunProjectChanged;
	bool ActiveMProjectsChanged;
	bool ActiveDProjectsChanged;
	bool ActiveRunProjectsChanged;
	bool DeletedProjectsChanged;

	bool CurrProjDirUsed;
	bool CurrProjDirLocal;
	wxString *CurrProjDir;
	wxString *CurrProjDirD2G;
	wxString *CurrProjDirLoc;
	wxString *CurrProjFile;

	bool CurrDitavalUsed;
	wxString *CurrDitaval;

	wxString *CurrSourceDir;
	wxString *CurrSourceMap;
	wxString *CurrOutputDir;
	wxString *CurrWrapDir;
	wxString *CurrFormat;

	wxString *CreateSourceDir;
	wxString *CreateSourceMap;
	wxString *CreateOutputDir;
	wxString *CreateFormat;

	wxString *CurrDelSourceDir;
	wxString *CurrDelSourceMap;
	wxString *CurrDelOutputDir;
	wxString *CurrDelFormat;

	bool FormatsChanged;
	bool MFormatsChanged;
	bool CurrFormatChanged;
	bool MCurrFormatChanged;
	wxArrayString *OutputFormats;
	wxString *CurrType;
	wxString *CurrIni;
	wxString *NewIniPath;
	wxArrayString *OutputInis;

	wxString *Editor;
	wxString *EditCommand;
	wxString *EditDocCommand;
	wxString *CMStart;
	wxString *CMCommand;

	bool ProjDirUsed;
	bool ProjDirLocal;
	bool SetCreateProjIni;
	wxString *ProjDirD2GPar;
	wxString *ProjDirLocRel;

	const wxChar *InitEditor;
	bool InitShowLog;
	bool InitShowConsole;
	bool InitShowDelInCreate;
	const wxChar *InitNewIniPath;
	const wxChar *InitLogFileName;
	bool InitProjDirUsed;
	bool InitProjDirLocal;
	wxString *InitProjDirD2GPar;
	wxString *InitProjDirLocRel;

	char *DocFileName;
	char *IniFileName;

	wxString *RunSourceMap;
	wxString *ShowLogCommand;
	wxString *ViewOutputCommand;
	wxString *LogFileName;
	wxString *HistoryFileName;
	wxString *FullLogName;
	wxString *FullHistoryName;
	wxString *OutputExt;
	void *LogEventFunc;
	//bool UseLog;
	bool ShowLog;
	bool LogIniChains;
	bool ShowConsole;
	bool ShowDelInCreate;
	bool WrapAndShip;

	wxString *CreateExistsText;
	wxString *CreateNothingText;
	wxString *CreatedProjectText;
	wxString *ModifiedProjectText;
	wxString *DeletedProjectText;
	wxString *RestoredProjectText;
	wxString *RunningText;
	wxString *RunningDoneText;
	wxString *RunningErrorText;
	wxString *RunningStopText;
	wxString *RunningAsychText;
	wxString *NoIniStart;
	wxString *NoIniMessage;
	wxString *NoIniTitle;

	// added functions in d2gdcl.cpp
	void InitializeDCL(void);
  void SetUpSysini(void);
  void SetUpLogging(void);
  void SetUpProjects(void);
	void SetAllToolTips(void);
	void SetAllLabels(void);
	void SetAllResults(void);
	wxArrayString *GetSysIniSect(const char *sect);
	wxString *GetSysIni(const char *key);

  void GetOptionsSettings(void);
  void GetOptionsValues(void);
  void GetStartOptions(void);
  void GetDefOptions(void);
  void SetOptionsValues(void);
  void UpdateOptions(void);
  void UpdateProjectsFile(void);
  void CloseProjectsFile(void);

	wxString *GetProjStr(const char *sect, const char *key);
	wxArrayString *GetProjSect(const char *sect);
	long GetProjInt(const char *sect, const char *key);
	bool GetProjBool(const char *sect, const char *key);
	void SetProjStr(const char *sect, const char *key, const char *val);
	void SetProjInt(const char *sect, const char *key, long num);
	void SetProjBool(const char *sect, const char *key, bool val);

	wxString *GetIniStr(const char *sect, const char *key, char *ini);
	bool GetIniBool(const char *sect, const char *key, char *ini);
	void SetIniStr(const char *sect, const char *key, const char *val, char *ini);
	void SetIniBool(const char *sect, const char *key, bool val, char *ini);

	long NextProjDirNum;
	char *GetSharedProjectName(void);

	void GetActiveProjects(void);
	void GetDeletedProjects(void);
	void GetAllFormats(void);
	void GetProjectInfo(void);
	void GetDelProjectInfo(void);
	void GetCurrIni(wxString *format);
	void GetProjectFileData(const char *id, wxString *outdir);
	void GetCurrProjectID(void);
	void GetCurrDelProjectID(void);
  bool CurrProjectExists(void);

	void SelectCreateProject(wxString *project);
	wxString *GetCreateSourceMap(void);
	void SetCreateProjectControls(bool active);
	void SetModifyProjectControls(void);
	void SetDeleteProjectControls(void);
	void SetRestoreProjectControls(void);
	void SetRunProjectControls(void);
	bool GetRunAuto(char *key, bool set = true);
	void SetRunAuto(char *key, bool set);
	bool UseDCLSource;
	bool UseDCLOutput;
	void SetNextProjectID(void);
	void ModifyProjectData(void);
	void ModifyDelProjectData(void);
	void ModifyProjectFileData(const char *id);
	void SetCurrProjDirLoc(wxString *outdir);
	void SetNewIni(void);
	bool SetNewIniFile(const char *iname, wxString *isrc, bool isdoc = false);
	void EditIniFile(void);
	void LaunchCM(void);
	void ShowLogFile(void);
	void ViewOutput(void);
	void RunConfigWizard(wxString *form);
	bool CreateDocFile(void);
	void SetWizardPageVars(wxString *form, Configstart *cfs);
	void GetWizardPageVars(wxString *form, Configstart *cfs);

	void ModifyProject(void);
	void DeleteProject(void);
	void RestoreProject(void);
	void CreateProject(void);
	void SetViewOutputCommand(bool run = false);
	void StartRun(void);
	void StopRun(void);

	bool FileExists(char *fname);
	wxString *MakeFullPath(wxString *dir, wxString *fn);
	wxString *D2gmain::FileFromPath(wxString *path);
	wxString *D2gmain::DirFromPath(wxString *path);
	wxString *FixEnvPath(wxString *str);
};

#endif
	// _D2GMAIN_H_
