/////////////////////////////////////////////////////////////////////////////
// Name:        optrest.h
// Purpose:     
// Author:      Jeremy H. Griffith
// Modified by: 
// Created:     08/12/2009 22:57:11
// RCS-ID:      
// Copyright:   (c) Copyright 2009 Omni Sytems, Inc.
// Licence:     
/////////////////////////////////////////////////////////////////////////////

#ifndef _OPTREST_H_
#define _OPTREST_H_


/*!
 * Includes
 */

////@begin includes
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
#define ID_OPTREST 10083
#define ID_OPTRESTDLG 10084
#define ID_OPTRESTSET 10085
#define ID_OPTRESTSTART 10086
#define ID_OPTRESTDEF 10087
#define SYMBOL_OPTREST_STYLE wxCAPTION|wxCLOSE_BOX|wxTAB_TRAVERSAL
#define SYMBOL_OPTREST_TITLE _("Options Restore")
#define SYMBOL_OPTREST_IDNAME ID_OPTREST
#define SYMBOL_OPTREST_SIZE wxDefaultSize
#define SYMBOL_OPTREST_POSITION wxDefaultPosition
////@end control identifiers


/*!
 * OptRest class declaration
 */

class OptRest: public wxDialog
{    
	DECLARE_DYNAMIC_CLASS( OptRest )
	DECLARE_EVENT_TABLE()

public:
	/// Constructors
	OptRest();
	OptRest( wxWindow* parent, wxWindowID id = SYMBOL_OPTREST_IDNAME, const wxString& caption = SYMBOL_OPTREST_TITLE, const wxPoint& pos = SYMBOL_OPTREST_POSITION, const wxSize& size = SYMBOL_OPTREST_SIZE, long style = SYMBOL_OPTREST_STYLE );

	/// Creation
	bool Create( wxWindow* parent, wxWindowID id = SYMBOL_OPTREST_IDNAME, const wxString& caption = SYMBOL_OPTREST_TITLE, const wxPoint& pos = SYMBOL_OPTREST_POSITION, const wxSize& size = SYMBOL_OPTREST_SIZE, long style = SYMBOL_OPTREST_STYLE );

	/// Destructor
	~OptRest();

	/// Initialises member variables
	void Init();

	/// Creates the controls and sizers
	void CreateControls();

////@begin OptRest event handler declarations

	/// wxEVT_INIT_DIALOG event handler for ID_OPTREST
	void OnInitDialog( wxInitDialogEvent& event );

	/// wxEVT_COMMAND_BUTTON_CLICKED event handler for wxID_OK
	void OnOkClick( wxCommandEvent& event );

	/// wxEVT_COMMAND_BUTTON_CLICKED event handler for wxID_CANCEL
	void OnCancelClick( wxCommandEvent& event );

////@end OptRest event handler declarations

////@begin OptRest member function declarations

	/// Retrieves bitmap resources
	wxBitmap GetBitmapResource( const wxString& name );

	/// Retrieves icon resources
	wxIcon GetIconResource( const wxString& name );
////@end OptRest member function declarations

	/// Should we show tooltips?
	static bool ShowToolTips();

////@begin OptRest member variables
	wxStaticText* m_optrestdlg;
	wxRadioButton* m_optrestset;
	wxRadioButton* m_optreststart;
	wxRadioButton* m_optrestdef;
	wxButton* m_optrestok;
	wxButton* m_optrestcan;
////@end OptRest member variables

	void SetAllToolTips(void);
	void SetAllLabels(void);
	wxString *GetSysIni(const char *key);
};

#endif
	// _OPTREST_H_
