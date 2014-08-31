/////////////////////////////////////////////////////////////////////////////
// Name:        configstart.h
// Purpose:     
// Author:      Jeremy H. Griffith
// Modified by: 
// Created:     26/12/2009 14:55:00
// RCS-ID:      
// Copyright:   (c) Copyright 2009 Omni Sytems, Inc.
// Licence:     
/////////////////////////////////////////////////////////////////////////////

#ifndef _CONFIGSTART_H_
#define _CONFIGSTART_H_


/*!
 * Includes
 */

////@begin includes
#include "wx/wizard.h"
#include "wx/statline.h"
#include "wx/filepicker.h"
////@end includes

/*!
 * Forward declarations
 */

////@begin forward declarations
class WizardPage1;
class wxFilePickerCtrl;
class WizardPage1A;
class WizardPage2;
class WizardPage2A;
class wxDirPickerCtrl;
class WizardPage3;
class WizardPage4;
class WizardPage5;
class WizardPageEnd;
////@end forward declarations

/*!
 * Control identifiers
 */

////@begin control identifiers
#define ID_CONFIGSTART 13000
#define ID_WIZARDPAGE1 13001
#define ID_CONFIGALL 13071
#define ID_CONFIGSTART1 13002
#define ID_CONFIGSTART2 13043
#define ID_CONFIGINISEL 13009
#define ID_CONFIGWAS 13046
#define ID_CONFIGWAS2 13076
#define ID_CONFIGWASYES 13018
#define ID_CONFIGWASNO 13019
#define ID_CONFIGLOCMSG 13086
#define ID_WIZARDPAGE1A 13072
#define ID_CONFIGCOND 13073
#define ID_CONFIGSTART3 13044
#define ID_CONFIGDVALFILE 13011
#define ID_CONFIGDVALSEL 13012
#define ID_CONFIGDVALNO 13013
#define ID_CONFIGDVALAPP 13066
#define ID_CONFIGDVALDOC 13008
#define ID_CONFIGDVALSTART 13010
#define ID_WIZARDPAGE2 13003
#define ID_CONFIGARCHIVE 13045
#define ID_CONFIGANAME 13047
#define ID_CONFIGANAMETXT 13014
#define ID_CONFIGANAMEAPP 13077
#define ID_CONFIGANAMEDOC 13078
#define ID_CONFIGANAMESTART 13079
#define ID_CONFIGAVER 13048
#define ID_CONFIGAVERTXT 13015
#define ID_CONFIGAVERAPP 13062
#define ID_CONFIGAVERDOC 13064
#define ID_CONFIGAVERSTART 13080
#define ID_WIZARDPAGE2A 13074
#define ID_CONFIGGRAPH 13075
#define ID_CONFIGGRDIR 13049
#define ID_CONFIGGRSEL 13016
#define ID_CONFIGGRDAPP 13082
#define ID_CONFIGGRDDOC 13083
#define ID_CONFIGGRDSTART 13084
#define ID_CONFIGGRFILES 13050
#define ID_CONFIGGRTXT 13017
#define ID_CONFIGGRFAPP 13081
#define ID_CONFIGGRFDOC 13063
#define ID_CONFIGGRFSTART 13065
#define ID_WIZARDPAGE3 13004
#define ID_CONFIGHTML 13042
#define ID_CONFIGTITLE 13051
#define ID_CONFIGTITLETXT 13021
#define ID_CONFIGHPF 13052
#define ID_CONFIGHPFTXT 13022
#define ID_CONFIGHTITLE 13053
#define ID_CONFIGHTITLETXT 13023
#define ID_CONFIGHTOPIC 13054
#define ID_CONFIGHTOPICTXT 13024
#define ID_CONFIGOHTL 13026
#define ID_CONFIGOHTLTXT 13027
#define ID_WIZARDPAGE4 13020
#define ID_CONFIGEH 13055
#define ID_CONFIGEHTITLE 13056
#define ID_CONFIGEHTITLETXT 13029
#define ID_CONFIGEHPN 13057
#define ID_CONFIGEHPNTXT 13034
#define ID_CONFIGEHPID 13058
#define ID_CONFIGEHPIDTXT 13028
#define ID_CONFIGEHPROV 13059
#define ID_CONFIGEHPROVTXT 13031
#define ID_CONFIGEHTOPIC 13067
#define ID_CONFIGEHTOPICTXT 13068
#define ID_WIZARDPAGE5 13033
#define ID_CONFIGRTF 13060
#define ID_CONFIGRTFID 13061
#define ID_CONFIGRTFIDTXT 13025
#define ID_CONFIGWV 13037
#define ID_CONFIGWV97 13030
#define ID_CONFIGWV2K 13032
#define ID_CONFIGWVXP 13035
#define ID_CONFIGWV2003 13036
#define ID_CONFIGHPJ 13039
#define ID_CONFIGHPJTXT 13038
#define ID_CONFIGCNTTITLE 13041
#define ID_CONFIGCNTTITLETXT 13040
#define ID_CONFIGWHTOPIC 13069
#define ID_CONFIGWHTOPICTXT 13070
#define ID_WIZARDPAGEEND 13005
#define ID_CONFIGREVIEW 13085
#define ID_CONFIGEND 13006
#define ID_CONFIGREVTEXT 13007
#define SYMBOL_CONFIGSTART_IDNAME ID_CONFIGSTART
////@end control identifiers


/*!
 * Configstart class declaration
 */

class Configstart: public wxWizard
{    
	DECLARE_DYNAMIC_CLASS( Configstart )
	DECLARE_EVENT_TABLE()

public:
	/// Constructors
	Configstart();
	Configstart( wxWindow* parent, wxWindowID id = SYMBOL_CONFIGSTART_IDNAME, const wxPoint& pos = wxDefaultPosition );

	/// Creation
	bool Create( wxWindow* parent, wxWindowID id = SYMBOL_CONFIGSTART_IDNAME, const wxPoint& pos = wxDefaultPosition );

	/// Destructor
	~Configstart();

	/// Initialises member variables
	void Init();

	/// Creates the controls and sizers
	void CreateControls();

////@begin Configstart event handler declarations

	/// wxEVT_WIZARD_PAGE_CHANGED event handler for ID_CONFIGSTART
	void OnConfigstartPageChanged( wxWizardEvent& event );

	/// wxEVT_WIZARD_PAGE_CHANGING event handler for ID_CONFIGSTART
	void OnConfigstartPageChanging( wxWizardEvent& event );

	/// wxEVT_WIZARD_CANCEL event handler for ID_CONFIGSTART
	void OnConfigstartCancel( wxWizardEvent& event );

	/// wxEVT_WIZARD_FINISHED event handler for ID_CONFIGSTART
	void OnConfigstartFinished( wxWizardEvent& event );

////@end Configstart event handler declarations

////@begin Configstart member function declarations

	/// Runs the wizard
	bool Run();

	/// Retrieves bitmap resources
	wxBitmap GetBitmapResource( const wxString& name );

	/// Retrieves icon resources
	wxIcon GetIconResource( const wxString& name );
////@end Configstart member function declarations

	/// Should we show tooltips?
	static bool ShowToolTips();

////@begin Configstart member variables
	WizardPage1* m_wizardpage1;
	WizardPage1A* m_wizardpage1a;
	WizardPage2* m_wizardpage2;
	WizardPage2A* m_wizardpage2a;
	WizardPage3* m_wizardpage3;
	WizardPage4* m_wizardpage4;
	WizardPage5* m_wizardpage5;
	WizardPageEnd* m_wizardpageend;
////@end Configstart member variables

	// starting values
	wxString *m_configouttype;
	wxString *m_configname;
	wxString *m_configsrcdir;
	wxString *m_configstartini;

	// page 1 results
	bool m_configpduse;
	wxString *m_configdocini;
	bool m_configdvuse;
	wxString *m_configdval;

	// page 2 results
	bool m_configwasdef;
	wxString *m_configanamestr;
	wxString *m_configaverstr;
	wxString *m_configgrdirstr;
	wxString *m_configgrfilesstr;

	// page 3 results

	// page 4 results

	// page 5 results

	// end page writing
	void SetFinalText(int page);
	void AddSetting(wxString *str, const char *key, const char *val);

	// wizard text settings
	void SetAllToolTips(void);
	void SetAllLabels(void);
	wxString *GetSysIni(const char *key);
};


/*!
 * WizardPage1 class declaration
 */

class WizardPage1: public wxWizardPage
{    
	DECLARE_DYNAMIC_CLASS( WizardPage1 )
	DECLARE_EVENT_TABLE()

public:
	/// Constructors
	WizardPage1();

	WizardPage1( wxWizard* parent );

	/// Creation
	bool Create( wxWizard* parent );

	/// Destructor
	~WizardPage1();

	/// Initialises member variables
	void Init();

	/// Creates the controls and sizers
	void CreateControls();

////@begin WizardPage1 event handler declarations

////@end WizardPage1 event handler declarations

////@begin WizardPage1 member function declarations

	/// Gets the previous page
	virtual wxWizardPage* GetPrev() const;

	/// Gets the next page
	virtual wxWizardPage* GetNext() const;

	/// Retrieves bitmap resources
	wxBitmap GetBitmapResource( const wxString& name );

	/// Retrieves icon resources
	wxIcon GetIconResource( const wxString& name );
////@end WizardPage1 member function declarations

	/// Should we show tooltips?
	static bool ShowToolTips();

////@begin WizardPage1 member variables
	wxStaticText* m_configall;
	wxStaticText* m_configstart1;
	wxStaticText* m_configstart2;
	wxFilePickerCtrl* m_configinisel;
	wxStaticText* m_configwas;
	wxStaticText* m_configwas2;
	wxRadioButton* m_configwasyes;
	wxRadioButton* m_configwasno;
	wxStaticText* m_configlocmsg;
////@end WizardPage1 member variables

  Configstart* configstart;
};


/*!
 * WizardPage1A class declaration
 */

class WizardPage1A: public wxWizardPage
{    
	DECLARE_DYNAMIC_CLASS( WizardPage1A )
	DECLARE_EVENT_TABLE()

public:
	/// Constructors
	WizardPage1A();

	WizardPage1A( wxWizard* parent );

	/// Creation
	bool Create( wxWizard* parent );

	/// Destructor
	~WizardPage1A();

	/// Initialises member variables
	void Init();

	/// Creates the controls and sizers
	void CreateControls();

////@begin WizardPage1A event handler declarations

////@end WizardPage1A event handler declarations

////@begin WizardPage1A member function declarations

	/// Gets the previous page
	virtual wxWizardPage* GetPrev() const;

	/// Gets the next page
	virtual wxWizardPage* GetNext() const;

	/// Retrieves bitmap resources
	wxBitmap GetBitmapResource( const wxString& name );

	/// Retrieves icon resources
	wxIcon GetIconResource( const wxString& name );
////@end WizardPage1A member function declarations

	/// Should we show tooltips?
	static bool ShowToolTips();

////@begin WizardPage1A member variables
	wxStaticText* m_configcond;
	wxStaticText* m_configstart3;
	wxRadioButton* m_configdvalfile;
	wxFilePickerCtrl* m_configdvalsel;
	wxRadioButton* m_configdvalno;
	wxStaticText* m_configdvalapp;
	wxRadioButton* m_configdvaldoc;
	wxRadioButton* m_configdvalstart;
////@end WizardPage1A member variables

  Configstart* configstart;
};


/*!
 * WizardPage2 class declaration
 */

class WizardPage2: public wxWizardPageSimple
{    
	DECLARE_DYNAMIC_CLASS( WizardPage2 )
	DECLARE_EVENT_TABLE()

public:
	/// Constructors
	WizardPage2();

	WizardPage2( wxWizard* parent );

	/// Creation
	bool Create( wxWizard* parent );

	/// Destructor
	~WizardPage2();

	/// Initialises member variables
	void Init();

	/// Creates the controls and sizers
	void CreateControls();

////@begin WizardPage2 event handler declarations

////@end WizardPage2 event handler declarations

////@begin WizardPage2 member function declarations

	/// Gets the previous page
	virtual wxWizardPage* GetPrev() const;

	/// Gets the next page
	virtual wxWizardPage* GetNext() const;

	/// Retrieves bitmap resources
	wxBitmap GetBitmapResource( const wxString& name );

	/// Retrieves icon resources
	wxIcon GetIconResource( const wxString& name );
////@end WizardPage2 member function declarations

	/// Should we show tooltips?
	static bool ShowToolTips();

////@begin WizardPage2 member variables
	wxStaticText* m_configarchive;
	wxStaticText* m_configaname;
	wxTextCtrl* m_configanametxt;
	wxStaticText* m_configanameapp;
	wxRadioButton* m_configanamedoc;
	wxRadioButton* m_configanamestart;
	wxStaticText* m_configaver;
	wxTextCtrl* m_configavertxt;
	wxStaticText* m_configaverapp;
	wxRadioButton* m_configaverdoc;
	wxRadioButton* m_configaverstart;
////@end WizardPage2 member variables

  Configstart* configstart;
};


/*!
 * WizardPage2A class declaration
 */

class WizardPage2A: public wxWizardPage
{    
	DECLARE_DYNAMIC_CLASS( WizardPage2A )
	DECLARE_EVENT_TABLE()

public:
	/// Constructors
	WizardPage2A();

	WizardPage2A( wxWizard* parent );

	/// Creation
	bool Create( wxWizard* parent );

	/// Destructor
	~WizardPage2A();

	/// Initialises member variables
	void Init();

	/// Creates the controls and sizers
	void CreateControls();

////@begin WizardPage2A event handler declarations

////@end WizardPage2A event handler declarations

////@begin WizardPage2A member function declarations

	/// Gets the previous page
	virtual wxWizardPage* GetPrev() const;

	/// Gets the next page
	virtual wxWizardPage* GetNext() const;

	/// Retrieves bitmap resources
	wxBitmap GetBitmapResource( const wxString& name );

	/// Retrieves icon resources
	wxIcon GetIconResource( const wxString& name );
////@end WizardPage2A member function declarations

	/// Should we show tooltips?
	static bool ShowToolTips();

////@begin WizardPage2A member variables
	wxStaticText* m_configgraph;
	wxStaticText* m_configgrdir;
	wxDirPickerCtrl* m_configgrsel;
	wxStaticText* m_configgrdapp;
	wxRadioButton* m_configgrddoc;
	wxRadioButton* m_configgrdstart;
	wxStaticText* m_configgrfiles;
	wxTextCtrl* m_configgrtxt;
	wxStaticText* m_configgrfapp;
	wxRadioButton* m_configgrfdoc;
	wxRadioButton* m_configgrfstart;
////@end WizardPage2A member variables

  Configstart* configstart;
};


/*!
 * WizardPage3 class declaration
 */

class WizardPage3: public wxWizardPage
{    
	DECLARE_DYNAMIC_CLASS( WizardPage3 )
	DECLARE_EVENT_TABLE()

public:
	/// Constructors
	WizardPage3();

	WizardPage3( wxWizard* parent );

	/// Creation
	bool Create( wxWizard* parent );

	/// Destructor
	~WizardPage3();

	/// Initialises member variables
	void Init();

	/// Creates the controls and sizers
	void CreateControls();

////@begin WizardPage3 event handler declarations

////@end WizardPage3 event handler declarations

////@begin WizardPage3 member function declarations

	/// Gets the previous page
	virtual wxWizardPage* GetPrev() const;

	/// Gets the next page
	virtual wxWizardPage* GetNext() const;

	/// Retrieves bitmap resources
	wxBitmap GetBitmapResource( const wxString& name );

	/// Retrieves icon resources
	wxIcon GetIconResource( const wxString& name );
////@end WizardPage3 member function declarations

	/// Should we show tooltips?
	static bool ShowToolTips();

////@begin WizardPage3 member variables
	wxStaticText* m_confightml;
	wxStaticText* m_configtitle;
	wxTextCtrl* m_configtitletxt;
	wxStaticText* m_confighp;
	wxTextCtrl* m_confighpftxt;
	wxStaticText* m_confightitle;
	wxTextCtrl* m_confightitletxt;
	wxStaticText* m_confightopic;
	wxTextCtrl* m_confightopictxt;
	wxStaticText* m_configohtl;
	wxTextCtrl* m_configohtltxt;
////@end WizardPage3 member variables

  Configstart* configstart;
};

/*!
 * WizardPage4 class declaration
 */

class WizardPage4: public wxWizardPage
{    
	DECLARE_DYNAMIC_CLASS( WizardPage4 )
	DECLARE_EVENT_TABLE()

public:
	/// Constructors
	WizardPage4();

	WizardPage4( wxWizard* parent );

	/// Creation
	bool Create( wxWizard* parent );

	/// Destructor
	~WizardPage4();

	/// Initialises member variables
	void Init();

	/// Creates the controls and sizers
	void CreateControls();

////@begin WizardPage4 event handler declarations

////@end WizardPage4 event handler declarations

////@begin WizardPage4 member function declarations

	/// Gets the previous page
	virtual wxWizardPage* GetPrev() const;

	/// Gets the next page
	virtual wxWizardPage* GetNext() const;

	/// Retrieves bitmap resources
	wxBitmap GetBitmapResource( const wxString& name );

	/// Retrieves icon resources
	wxIcon GetIconResource( const wxString& name );
////@end WizardPage4 member function declarations

	/// Should we show tooltips?
	static bool ShowToolTips();

////@begin WizardPage4 member variables
	wxStaticText* m_configeh;
	wxStaticText* m_configehtitle;
	wxTextCtrl* m_configehtitletxt;
	wxStaticText* m_configehpn;
	wxTextCtrl* m_configehpntxt;
	wxStaticText* m_configehpid;
	wxTextCtrl* m_configehpidtxt;
	wxStaticText* m_configehprov;
	wxTextCtrl* m_configehprovtxt;
	wxStaticText* m_configehtopic;
	wxTextCtrl* m_configehtopictxt;
////@end WizardPage4 member variables

  Configstart* configstart;
};

/*!
 * WizardPage5 class declaration
 */

class WizardPage5: public wxWizardPage
{    
	DECLARE_DYNAMIC_CLASS( WizardPage5 )
	DECLARE_EVENT_TABLE()

public:
	/// Constructors
	WizardPage5();

	WizardPage5( wxWizard* parent );

	/// Creation
	bool Create( wxWizard* parent );

	/// Destructor
	~WizardPage5();

	/// Initialises member variables
	void Init();

	/// Creates the controls and sizers
	void CreateControls();

////@begin WizardPage5 event handler declarations

////@end WizardPage5 event handler declarations

////@begin WizardPage5 member function declarations

	/// Gets the previous page
	virtual wxWizardPage* GetPrev() const;

	/// Gets the next page
	virtual wxWizardPage* GetNext() const;

	/// Retrieves bitmap resources
	wxBitmap GetBitmapResource( const wxString& name );

	/// Retrieves icon resources
	wxIcon GetIconResource( const wxString& name );
////@end WizardPage5 member function declarations

	/// Should we show tooltips?
	static bool ShowToolTips();

////@begin WizardPage5 member variables
	wxStaticText* m_configrtf;
	wxStaticText* m_configrtfid;
	wxTextCtrl* m_configrtfidtxt;
	wxStaticText* m_configwv;
	wxRadioButton* m_configwv97;
	wxRadioButton* m_configwv2k;
	wxRadioButton* m_configwvxp;
	wxRadioButton* m_configwv2003;
	wxStaticText* m_confighpj;
	wxTextCtrl* m_confighpjtxt;
	wxStaticText* m_configcnttitle;
	wxTextCtrl* m_configcnttitletxt;
	wxStaticText* m_configwhtopic;
	wxTextCtrl* m_configwhtopictxt;
////@end WizardPage5 member variables

  Configstart* configstart;
};

/*!
 * WizardPageEnd class declaration
 */

class WizardPageEnd: public wxWizardPage
{    
	DECLARE_DYNAMIC_CLASS( WizardPageEnd )
	DECLARE_EVENT_TABLE()

public:
	/// Constructors
	WizardPageEnd();

	WizardPageEnd( wxWizard* parent );

	/// Creation
	bool Create( wxWizard* parent );

	/// Destructor
	~WizardPageEnd();

	/// Initialises member variables
	void Init();

	/// Creates the controls and sizers
	void CreateControls();

////@begin WizardPageEnd event handler declarations

////@end WizardPageEnd event handler declarations

////@begin WizardPageEnd member function declarations

	/// Gets the previous page
	virtual wxWizardPage* GetPrev() const;

	/// Gets the next page
	virtual wxWizardPage* GetNext() const;

	/// Retrieves bitmap resources
	wxBitmap GetBitmapResource( const wxString& name );

	/// Retrieves icon resources
	wxIcon GetIconResource( const wxString& name );
////@end WizardPageEnd member function declarations

	/// Should we show tooltips?
	static bool ShowToolTips();

////@begin WizardPageEnd member variables
	wxStaticText* m_configreview;
	wxStaticText* m_configend;
	wxTextCtrl* m_configrevtext;
////@end WizardPageEnd member variables

  Configstart* configstart;
};

#endif
	// _CONFIGSTART_H_
