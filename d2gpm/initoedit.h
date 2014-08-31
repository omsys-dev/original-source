/////////////////////////////////////////////////////////////////////////////
// Name:        initoedit.h
// Purpose:     
// Author:      Jeremy H. Griffith
// Modified by: 
// Created:     23/01/2010 23:02:03
// RCS-ID:      
// Copyright:   (c) Copyright 2009 Omni Sytems, Inc.
// Licence:     
/////////////////////////////////////////////////////////////////////////////

#ifndef _INITOEDIT_H_
#define _INITOEDIT_H_


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
#define ID_INITOEDIT 10057
#define ID_I2EDIT 10059
#define ID_I2ESTART 10061
#define ID_I2ESTARTNAME 10077
#define ID_I2EDOC 10078
#define ID_I2EDOCNAME 10079
#define SYMBOL_INITOEDIT_STYLE wxCAPTION|wxRESIZE_BORDER|wxSYSTEM_MENU|wxCLOSE_BOX|wxFULL_REPAINT_ON_RESIZE|wxTAB_TRAVERSAL
#define SYMBOL_INITOEDIT_TITLE _("Ini To Edit")
#define SYMBOL_INITOEDIT_IDNAME ID_INITOEDIT
#define SYMBOL_INITOEDIT_SIZE wxSize(400, 300)
#define SYMBOL_INITOEDIT_POSITION wxDefaultPosition
////@end control identifiers


/*!
 * IniToEdit class declaration
 */

class IniToEdit: public wxDialog
{    
	DECLARE_DYNAMIC_CLASS( IniToEdit )
	DECLARE_EVENT_TABLE()

public:
	/// Constructors
	IniToEdit();
	IniToEdit( wxWindow* parent, wxWindowID id = SYMBOL_INITOEDIT_IDNAME, const wxString& caption = SYMBOL_INITOEDIT_TITLE, const wxPoint& pos = SYMBOL_INITOEDIT_POSITION, const wxSize& size = SYMBOL_INITOEDIT_SIZE, long style = SYMBOL_INITOEDIT_STYLE );

	/// Creation
	bool Create( wxWindow* parent, wxWindowID id = SYMBOL_INITOEDIT_IDNAME, const wxString& caption = SYMBOL_INITOEDIT_TITLE, const wxPoint& pos = SYMBOL_INITOEDIT_POSITION, const wxSize& size = SYMBOL_INITOEDIT_SIZE, long style = SYMBOL_INITOEDIT_STYLE );

	/// Destructor
	~IniToEdit();

	/// Initialises member variables
	void Init();

	/// Creates the controls and sizers
	void CreateControls();

////@begin IniToEdit event handler declarations

	/// wxEVT_INIT_DIALOG event handler for ID_INITOEDIT
	void OnInitDialog( wxInitDialogEvent& event );

	/// wxEVT_COMMAND_BUTTON_CLICKED event handler for wxID_OK
	void OnOkClick( wxCommandEvent& event );

	/// wxEVT_COMMAND_BUTTON_CLICKED event handler for wxID_CANCEL
	void OnCancelClick( wxCommandEvent& event );

////@end IniToEdit event handler declarations

////@begin IniToEdit member function declarations

	/// Retrieves bitmap resources
	wxBitmap GetBitmapResource( const wxString& name );

	/// Retrieves icon resources
	wxIcon GetIconResource( const wxString& name );
////@end IniToEdit member function declarations

	/// Should we show tooltips?
	static bool ShowToolTips();

////@begin IniToEdit member variables
	wxStaticText* m_i2edit;
	wxRadioButton* m_i2estart;
	wxStaticText* m_i2estartname;
	wxRadioButton* m_i2edoc;
	wxStaticText* m_i2edocname;
	wxButton* m_i2etok;
	wxButton* m_i2ecan;
////@end IniToEdit member variables

	void SetAllToolTips(void);
	void SetAllLabels(void);
	wxString *GetSysIni(const char *key);
};

#endif
	// _INITOEDIT_H_
