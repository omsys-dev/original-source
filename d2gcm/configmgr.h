/////////////////////////////////////////////////////////////////////////////
// Name:        configmgr.h
// Purpose:     
// Author:      Jeremy H Griffith
// Modified by: 
// Created:     12/03/2012 14:13:49
// RCS-ID:      
// Copyright:   Copyright (c) 2012 Omni Systems
// Licence:     
/////////////////////////////////////////////////////////////////////////////

#ifndef _CONFIGMGR_H_
#define _CONFIGMGR_H_


/*!
 * Includes
 */

////@begin includes
#include "wx/propdlg.h"
#include "wx/dirctrl.h"
#include "wx/treectrl.h"
#include "wx/statline.h"
#include "wx/listctrl.h"
#include "wx/clrpicker.h"
////@end includes

#include "wx/textfile.h"


/*!
 * Forward declarations
 */

////@begin forward declarations
class wxGenericDirCtrl;
class wxTreeCtrl;
class wxListCtrl;
class wxBoxSizer;
class wxColourPickerCtrl;
////@end forward declarations

class CMgr;
class CIni;


/*!
 * Control identifiers
 */

////@begin control identifiers
#define ID_CONFIGMGR 10000
#define ID_FILEPANEL 10001
#define wxID_PROJSELCONFIG 10067
#define ID_PROJNAMEBUTTON 10064
#define wxID_PROJNAME 10054
#define ID_PROJNAMESEL 10055
#define ID_PROJSTARTTPLBUTTON 10065
#define wxID_PROJSELSTARTTPL 10062
#define ID_PROJSTARTTPLSEL 10063
#define ID_FILEBROWSEBUTTON 10070
#define wxID_FILEBROWSE 10071
#define ID_PROJDIRCTRL 10086
#define wxID_PROJSTARTFILE 10003
#define ID_PROJSTARTTEXT 10002
#define ID_PROJRESULT 10017
#define ID_APPLYPROJ 10022
#define ID_PROJHELPBUTTON 10006
#define ID_CATPANEL 10068
#define wxID_CATSTARTFILE 10075
#define ID_CATSTARTTEXT 10076
#define ID_CATINITYPEBUTTON 10201
#define wxID_PROJSELCAT 10066
#define ID_CATINICHOICE 10202
#define ID_CATSELBUTTON 10200
#define ID_SECTYPTREE 10056
#define ID_CATFILEBUTTON 10072
#define ID_CATALLBUTTON 10074
#define wxID_PROJSTART 10144
#define wxID_PROJSTARTSECT 10058
#define ID_PROJSTARTSECTEXTCTRL 10016
#define wxID_PROJSTARTSET 10059
#define ID_PROJSTARTSETTEXTCTRL 10057
#define wxID_PROJSTARTWARN 10143
#define ID_CATRESULT 10017
#define ID_APPLYCAT 10069
#define ID_CATHELPBUTTON 10018
#define ID_ACTPANEL 10168
#define wxID_ACTSTARTFILE 10169
#define ID_ACTSTARTTEXT 10170
#define wxID_ACTSEL 10192
#define wxID_SECTADD 10110
#define ID_SECTADDBUTTON 10027
#define ID_SECTVNADDBUTTON 10029
#define ID_SECTADDSETBUTTON 10156
#define ID_SECTADDVKSETBUTTON 10148
#define wxID_SECTEDIT 10111
#define ID_SECTCHANGEBUTTON 10025
#define ID_SECTSETEDITBUTTON 10053
#define wxID_SECTDEL 10154
#define ID_SECTDELBUTTON 10051
#define ID_SECTDELSETBUTTON 10157
#define wxID_SECTREST 10109
#define ID_SECTRESTBUTTON 10112
#define ID_SECTRESTSETBUTTON 10139
#define wxID_SECTREN 10155
#define ID_SECTRENBUTTON 10035
#define ID_SECTRENSETBUTTON 10158
#define wxID_SECTMOV 10026
#define ID_SECTMOVBUTTON 10185
#define ID_SECTMOVFBUTTON 10188
#define ID_SECTMOVSETBUTTON 10190
#define ID_SECTMOVSETFBUTTON 10191
#define wxID_SECTCPY 10013
#define ID_SECTCPYBUTTON 10032
#define ID_SECTCPYSETBUTTON 10050
#define wxID_SECTDUP 10145
#define ID_SECTDUPBUTTON 10146
#define ID_SECTDUPSETBUTTON 10147
#define ID_ACTRESULT 10023
#define ID_APPLYACT 10150
#define ID_ACTHELPBUTTON 10171
#define ID_SECTIONPANEL 10008
#define wxID_SECTSTARTFILE 10077
#define ID_SECTSTARTTEXT 10078
#define wxID_SECTCAT 10180
#define ID_SECTCATTEXT 10181
#define wxID_SECTACT 10193
#define ID_SECTACTTEXT 10194
#define wxID_SELSECT 10009
#define ID_SECTCTRL 10085
#define ID_SECTHELPTEXT 10105
#define wxID_SECTVARNAME 10184
#define wxID_SECTVNADD 10186
#define ID_SECTADDTEXT 10187
#define wxID_SECTVNREN 10189
#define ID_SECTVNRENTEXT 10028
#define ID_SECTFORMALIAS 10199
#define ID_SECTRESULT 10172
#define ID_APPLYSECT 10173
#define ID_SECTHELPBUTTON 10174
#define ID_SETTINGPANEL 10011
#define wxID_SETSTARTFILE 10079
#define ID_SETSTARTTEXT 10080
#define wxID_SETCAT 10182
#define ID_SETCATTEXT 10183
#define wxID_SETACT 10159
#define ID_SETACTTEXT 10160
#define wxID_SETSECT 10037
#define ID_SETSECTTEXT 10036
#define wxID_SETSECTHELP 10122
#define ID_SETSECTHELPTEXT 10123
#define wxID_SELSET 10012
#define ID_SETCTRL 10083
#define ID_MACROTEXTCTRL 10084
#define ID_SETHELPTEXT 10030
#define wxID_SETVARKEY 10052
#define ID_SETVARKEYBUTTON 10179
#define wxID_SETVKADD 10161
#define ID_SETVKADDTEXT 10031
#define wxID_SETVKREN 10162
#define ID_SETVKRENTEXT 10033
#define ID_SETRESULT 10017
#define ID_APPLYSET 10024
#define ID_SETHELPBUTTON 10151
#define ID_VALPANEL 10014
#define wxID_VALSTARTFILE 10081
#define ID_VALSTARTTEXT 10082
#define wxID_VALCAT 10195
#define ID_VALCATTEXT 10196
#define wxID_VALACT 10163
#define ID_VALACTTEXT 10164
#define wxID_VALSECT 10044
#define ID_VALSECTTEXT 10045
#define wxID_VALSECTHELP 10120
#define ID_VALSECTHELPTEXT 10121
#define wxID_VALSET 10038
#define ID_VALSETTEXT 10039
#define wxID_VALSETHELP 10117
#define ID_VALSETHELPTEXT 10118
#define wxID_SELINI 10015
#define ID_SELINICTRL 10004
#define ID_VALHELPTEXT 10167
#define wxID_CURRVAL 10020
#define ID_CURRVALTEXT 10021
#define ID_VALRESULT 10017
#define ID_APPLYVAL 10019
#define ID_VALHELPBUTTON 10152
#define ID_VSELPANEL 10087
#define wxID_VSELSTARTFILE 10088
#define ID_VSELSTARTTEXT 10089
#define wxID_VSELCAT 10197
#define ID_VSELCATTEXT 10198
#define wxID_VSELACT 10165
#define ID_VSELACTTEXT 10166
#define wxID_VSELSECT 10090
#define ID_VSELSECTTEXT 10091
#define wxID_VSELSECTHELP 10034
#define ID_VSELSECTHELPTEXT 10119
#define wxID_VSELSET 10092
#define ID_VSELSETTEXT 10093
#define wxID_VSELSETHELP 10114
#define ID_VSELSETHELPTEXT 10113
#define wxID_VSELINI 10102
#define ID_VSELINITEXT 10103
#define wxID_VSELDEST 10060
#define ID_VSELDESTTEXT 10061
#define wxID_VSELREN 10005
#define ID_VSELRENTEXT 10007
#define wxID_VSELNAME 10149
#define wxID_VSELVAL 10100
#define ID_VSELVALTEXT 10101
#define wxID_VSELVALHELP 10116
#define wxID_VSELNAMEHELP 10175
#define ID_VSELVALBUTTON 10178
#define ID_VSELVALHELPTEXT 10115
#define wxID_SELVAL 10094
#define wxID_VSELFIX 10177
#define wxID_VSELDEL 10108
#define wxID_VSELMOV 10107
#define ID_SELVALCTRL 10095
#define wxID_VSELMACADD 10199
#define wxID_VSELMACEDIT 10048
#define ID_VSELMACTEXT 10049
#define wxID_VSELONE 10073
#define ID_VSELONETEXT 10098
#define wxID_VSELCURRVAL 10096
#define ID_VSELCURRVALTEXT 10097
#define ID_VSELUPBUTTON 10104
#define ID_VSELDNBUTTON 10106
#define ID_VSELRESULT 10017
#define ID_APPLYVSEL 10099
#define ID_VSELHELPBUTTON 10153
#define ID_OPTPANEL 10040
#define wxID_OPTSYS 10043
#define ID_OPTSYSYES 10041
#define ID_OPTSYSNO 10042
#define wxID_OPTCOLORS 10124
#define wxID_OPTCOLORSECT 10010
#define ID_OPTCOLORSECTTEXT 10126
#define ID_OPTCOLORSECTCTRL 10125
#define wxID_OPTCOLORSET 10133
#define ID_OPTCOLORSETTEXT 10131
#define ID_OPTCOLORSETCTRL 10132
#define wxID_OPTCOLORVAR 10136
#define ID_OPTCOLORVARTEXT 10137
#define ID_OPTCOLORVARCTRL 10138
#define wxID_OPTCOLORDEF 10134
#define ID_OPTCOLORDEFTEXT 10127
#define ID_OPTCOLORDEFCTRL 10128
#define wxID_OPTCOLORSYS 10135
#define ID_OPTCOLORSYSTEXT 10129
#define ID_OPTCOLORSYSCTRL 10130
#define ID_OPTADDINIHIST 10142
#define wxID_OPTUSERNAME 10140
#define ID_OPTUSERNAMETEXT 10141
#define wxID_OPTBROWSER 10046
#define ID_OPTBROWSERCTRL 10047
#define ID_OPTHELPBUTTON 10176
#define SYMBOL_CONFIGMGR_STYLE wxCAPTION|wxRESIZE_BORDER|wxSYSTEM_MENU|wxCLOSE_BOX|wxMINIMIZE_BOX

#ifdef _M2GCM
#define SYMBOL_CONFIGMGR_TITLE _("Mif2Go Configuration Manager")
#else
#define SYMBOL_CONFIGMGR_TITLE _("DITA2Go Configuration Manager")
#endif

#define SYMBOL_CONFIGMGR_IDNAME ID_CONFIGMGR
#define SYMBOL_CONFIGMGR_SIZE wxSize(250, 250)
#define SYMBOL_CONFIGMGR_POSITION wxDefaultPosition
////@end control identifiers


class MyTreeItemData : public wxTreeItemData
{
private:
  wxString m_desc;
  int m_val;

public:
  MyTreeItemData(const wxString& desc) : m_desc(desc) { }
  MyTreeItemData(const int val) : m_val(val) { }

  const wxString& GetDesc() const { return m_desc; }
  int GetVal() { return m_val; }
};


/*!
 * ConfigMgr class declaration
 */

class ConfigMgr: public wxPropertySheetDialog
{    
  DECLARE_DYNAMIC_CLASS( ConfigMgr )
  DECLARE_EVENT_TABLE()

public:
  /// Constructors
  ConfigMgr();
  ConfigMgr( wxWindow* parent, wxWindowID id = SYMBOL_CONFIGMGR_IDNAME, const wxString& caption = SYMBOL_CONFIGMGR_TITLE, const wxPoint& pos = SYMBOL_CONFIGMGR_POSITION, const wxSize& size = SYMBOL_CONFIGMGR_SIZE, long style = SYMBOL_CONFIGMGR_STYLE );

  /// Creation
  bool Create( wxWindow* parent, wxWindowID id = SYMBOL_CONFIGMGR_IDNAME, const wxString& caption = SYMBOL_CONFIGMGR_TITLE, const wxPoint& pos = SYMBOL_CONFIGMGR_POSITION, const wxSize& size = SYMBOL_CONFIGMGR_SIZE, long style = SYMBOL_CONFIGMGR_STYLE );

  /// Destructor
  ~ConfigMgr();

  /// Initialises member variables
  void Init();

  /// Creates the controls and sizers
  void CreateControls();

////@begin ConfigMgr event handler declarations

	/// wxEVT_INIT_DIALOG event handler for ID_CONFIGMGR
	void OnInitDialog( wxInitDialogEvent& event );

	/// wxEVT_ENTER_WINDOW event handler for ID_FILEPANEL
	void OnEnterFileWindow( wxMouseEvent& event );

	/// wxEVT_COMMAND_RADIOBUTTON_SELECTED event handler for ID_PROJNAMEBUTTON
	void OnProjnamebuttonSelected( wxCommandEvent& event );

	/// wxEVT_COMMAND_CHOICE_SELECTED event handler for ID_PROJNAMESEL
	void OnProjnameselSelected( wxCommandEvent& event );

	/// wxEVT_COMMAND_RADIOBUTTON_SELECTED event handler for ID_PROJSTARTTPLBUTTON
	void OnProjstarttplbuttonSelected( wxCommandEvent& event );

	/// wxEVT_COMMAND_CHOICE_SELECTED event handler for ID_PROJSTARTTPLSEL
	void OnProjstarttplselSelected( wxCommandEvent& event );

	/// wxEVT_COMMAND_RADIOBUTTON_SELECTED event handler for ID_FILEBROWSEBUTTON
	void OnFilebrowsebuttonSelected( wxCommandEvent& event );

	/// wxEVT_COMMAND_TREE_SEL_CHANGED event handler for ID_PROJDIRCTRL
	void OnProjdirctrlSelChanged( wxTreeEvent& event );

	/// wxEVT_LEFT_DCLICK event handler for ID_PROJDIRCTRL
	void OnLeftDClick( wxMouseEvent& event );

	/// wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_APPLYPROJ
	void OnApplyprojClick( wxCommandEvent& event );

	/// wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_PROJHELPBUTTON
	void OnProjhelpbuttonClick( wxCommandEvent& event );

	/// wxEVT_ENTER_WINDOW event handler for ID_CATPANEL
	void OnEnterCatWindow( wxMouseEvent& event );

	/// wxEVT_COMMAND_RADIOBUTTON_SELECTED event handler for ID_CATINITYPEBUTTON
	void OnCatinitypebuttonSelected( wxCommandEvent& event );

	/// wxEVT_COMMAND_CHOICE_SELECTED event handler for ID_CATINICHOICE
	void OnCatinichoiceSelected( wxCommandEvent& event );

	/// wxEVT_COMMAND_RADIOBUTTON_SELECTED event handler for ID_CATSELBUTTON
	void OnCatselbuttonSelected( wxCommandEvent& event );

	/// wxEVT_COMMAND_TREE_SEL_CHANGED event handler for ID_SECTYPTREE
	void OnSectyptreeSelChanged( wxTreeEvent& event );

	/// wxEVT_COMMAND_TREE_ITEM_ACTIVATED event handler for ID_SECTYPTREE
	void OnSectyptreeItemActivated( wxTreeEvent& event );

	/// wxEVT_COMMAND_RADIOBUTTON_SELECTED event handler for ID_CATFILEBUTTON
	void OnCatfilebuttonSelected( wxCommandEvent& event );

	/// wxEVT_COMMAND_RADIOBUTTON_SELECTED event handler for ID_CATALLBUTTON
	void OnCatallbuttonSelected( wxCommandEvent& event );

	/// wxEVT_COMMAND_TEXT_UPDATED event handler for ID_PROJSTARTSECTEXTCTRL
	void OnProjstartsectextctrlTextUpdated( wxCommandEvent& event );

	/// wxEVT_COMMAND_TEXT_ENTER event handler for ID_PROJSTARTSECTEXTCTRL
	void OnProjstartsectextctrlEnter( wxCommandEvent& event );

	/// wxEVT_COMMAND_TEXT_UPDATED event handler for ID_PROJSTARTSETTEXTCTRL
	void OnProjstartsettextctrlTextUpdated( wxCommandEvent& event );

	/// wxEVT_COMMAND_TEXT_ENTER event handler for ID_PROJSTARTSETTEXTCTRL
	void OnProjstartsettextctrlEnter( wxCommandEvent& event );

	/// wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_APPLYCAT
	void OnApplycatClick( wxCommandEvent& event );

	/// wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_CATHELPBUTTON
	void OnCathelpbuttonClick( wxCommandEvent& event );

	/// wxEVT_ENTER_WINDOW event handler for ID_ACTPANEL
	void OnEnterActWindow( wxMouseEvent& event );

	/// wxEVT_COMMAND_RADIOBUTTON_SELECTED event handler for ID_SECTADDBUTTON
	void OnSectaddbuttonSelected( wxCommandEvent& event );

	/// wxEVT_COMMAND_RADIOBUTTON_SELECTED event handler for ID_SECTVNADDBUTTON
	void OnSectvnaddbuttonSelected( wxCommandEvent& event );

	/// wxEVT_COMMAND_RADIOBUTTON_SELECTED event handler for ID_SECTADDSETBUTTON
	void OnSectaddsetbuttonSelected( wxCommandEvent& event );

	/// wxEVT_COMMAND_RADIOBUTTON_SELECTED event handler for ID_SECTADDVKSETBUTTON
	void OnSectaddvksetbuttonSelected( wxCommandEvent& event );

	/// wxEVT_COMMAND_RADIOBUTTON_SELECTED event handler for ID_SECTCHANGEBUTTON
	void OnSectchangebuttonSelected( wxCommandEvent& event );

	/// wxEVT_COMMAND_RADIOBUTTON_SELECTED event handler for ID_SECTSETEDITBUTTON
	void OnSectseteditbuttonSelected( wxCommandEvent& event );

	/// wxEVT_COMMAND_RADIOBUTTON_SELECTED event handler for ID_SECTDELBUTTON
	void OnSectdelbuttonSelected( wxCommandEvent& event );

	/// wxEVT_COMMAND_RADIOBUTTON_SELECTED event handler for ID_SECTDELSETBUTTON
	void OnSectdelsetbuttonSelected( wxCommandEvent& event );

	/// wxEVT_COMMAND_RADIOBUTTON_SELECTED event handler for ID_SECTRESTBUTTON
	void OnSectrestbuttonSelected( wxCommandEvent& event );

	/// wxEVT_COMMAND_RADIOBUTTON_SELECTED event handler for ID_SECTRESTSETBUTTON
	void OnSectrestsetbuttonSelected( wxCommandEvent& event );

	/// wxEVT_COMMAND_RADIOBUTTON_SELECTED event handler for ID_SECTRENBUTTON
	void OnSectrenbuttonSelected( wxCommandEvent& event );

	/// wxEVT_COMMAND_RADIOBUTTON_SELECTED event handler for ID_SECTRENSETBUTTON
	void OnSectrensetbuttonSelected( wxCommandEvent& event );

	/// wxEVT_COMMAND_RADIOBUTTON_SELECTED event handler for ID_SECTMOVBUTTON
	void OnSectmovbuttonSelected( wxCommandEvent& event );

	/// wxEVT_COMMAND_RADIOBUTTON_SELECTED event handler for ID_SECTMOVFBUTTON
	void OnSectmovfbuttonSelected( wxCommandEvent& event );

	/// wxEVT_COMMAND_RADIOBUTTON_SELECTED event handler for ID_SECTMOVSETBUTTON
	void OnSectmovsetbuttonSelected( wxCommandEvent& event );

	/// wxEVT_COMMAND_RADIOBUTTON_SELECTED event handler for ID_SECTMOVSETFBUTTON
	void OnSectmovsetfbuttonSelected( wxCommandEvent& event );

	/// wxEVT_COMMAND_RADIOBUTTON_SELECTED event handler for ID_SECTCPYBUTTON
	void OnSectcpybuttonSelected( wxCommandEvent& event );

	/// wxEVT_COMMAND_RADIOBUTTON_SELECTED event handler for ID_SECTCPYSETBUTTON
	void OnSectcpysetbuttonSelected( wxCommandEvent& event );

	/// wxEVT_COMMAND_RADIOBUTTON_SELECTED event handler for ID_SECTDUPBUTTON
	void OnSectdupbuttonSelected( wxCommandEvent& event );

	/// wxEVT_COMMAND_RADIOBUTTON_SELECTED event handler for ID_SECTDUPSETBUTTON
	void OnSectdupsetbuttonSelected( wxCommandEvent& event );

	/// wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_APPLYACT
	void OnApplyactClick( wxCommandEvent& event );

	/// wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_ACTHELPBUTTON
	void OnActhelpbuttonClick( wxCommandEvent& event );

	/// wxEVT_ENTER_WINDOW event handler for ID_SECTIONPANEL
	void OnEnterSectWindow( wxMouseEvent& event );

	/// wxEVT_COMMAND_LIST_ITEM_SELECTED event handler for ID_SECTCTRL
	void OnSectctrlSelected( wxListEvent& event );

	/// wxEVT_COMMAND_LIST_ITEM_ACTIVATED event handler for ID_SECTCTRL
	void OnSectctrlItemActivated( wxListEvent& event );

	/// wxEVT_COMMAND_TEXT_UPDATED event handler for ID_SECTADDTEXT
	void OnSectaddtextTextUpdated( wxCommandEvent& event );

	/// wxEVT_COMMAND_TEXT_ENTER event handler for ID_SECTADDTEXT
	void OnSectaddtextEnter( wxCommandEvent& event );

	/// wxEVT_COMMAND_TEXT_UPDATED event handler for ID_SECTVNRENTEXT
	void OnSectvnrentextTextUpdated( wxCommandEvent& event );

	/// wxEVT_COMMAND_TEXT_ENTER event handler for ID_SECTVNRENTEXT
	void OnSectvnrentextEnter( wxCommandEvent& event );

	/// wxEVT_COMMAND_CHECKBOX_CLICKED event handler for ID_SECTFORMALIAS
	void OnSectformaliasClick( wxCommandEvent& event );

	/// wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_APPLYSECT
	void OnApplysectClick( wxCommandEvent& event );

	/// wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_SECTHELPBUTTON
	void OnSecthelpbuttonClick( wxCommandEvent& event );

	/// wxEVT_ENTER_WINDOW event handler for ID_SETTINGPANEL
	void OnEnterSetWindow( wxMouseEvent& event );

	/// wxEVT_COMMAND_LIST_ITEM_SELECTED event handler for ID_SETCTRL
	void OnSetctrlSelected( wxListEvent& event );

	/// wxEVT_COMMAND_LIST_ITEM_ACTIVATED event handler for ID_SETCTRL
	void OnSetctrlItemActivated( wxListEvent& event );

	/// wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_SETVARKEYBUTTON
	void OnSetvarkeybuttonClick( wxCommandEvent& event );

	/// wxEVT_COMMAND_TEXT_UPDATED event handler for ID_SETVKADDTEXT
	void OnSetvkaddtextTextUpdated( wxCommandEvent& event );

	/// wxEVT_COMMAND_TEXT_ENTER event handler for ID_SETVKADDTEXT
	void OnSetvkaddtextEnter( wxCommandEvent& event );

	/// wxEVT_COMMAND_TEXT_UPDATED event handler for ID_SETVKRENTEXT
	void OnSetvkrentextTextUpdated( wxCommandEvent& event );

	/// wxEVT_COMMAND_TEXT_ENTER event handler for ID_SETVKRENTEXT
	void OnSetvkrentextEnter( wxCommandEvent& event );

	/// wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_APPLYSET
	void OnApplysetClick( wxCommandEvent& event );

	/// wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_SETHELPBUTTON
	void OnSethelpbuttonClick( wxCommandEvent& event );

	/// wxEVT_ENTER_WINDOW event handler for ID_VALPANEL
	void OnEnterValWindow( wxMouseEvent& event );

	/// wxEVT_COMMAND_LIST_ITEM_SELECTED event handler for ID_SELINICTRL
	void OnSelinictrlSelected( wxListEvent& event );

	/// wxEVT_COMMAND_LIST_ITEM_DESELECTED event handler for ID_SELINICTRL
	void OnSelinictrlDeselected( wxListEvent& event );

	/// wxEVT_COMMAND_LIST_ITEM_ACTIVATED event handler for ID_SELINICTRL
	void OnSelinictrlItemActivated( wxListEvent& event );

	/// wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_APPLYVAL
	void OnApplyvalClick( wxCommandEvent& event );

	/// wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_VALHELPBUTTON
	void OnValhelpbuttonClick( wxCommandEvent& event );

	/// wxEVT_ENTER_WINDOW event handler for ID_VSELPANEL
	void OnEnterVselWindow( wxMouseEvent& event );

	/// wxEVT_COMMAND_TEXT_ENTER event handler for ID_VSELVALTEXT
	void OnVselvaltextEnter( wxCommandEvent& event );

	/// wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_VSELVALBUTTON
	void OnVselvalbuttonClick( wxCommandEvent& event );

	/// wxEVT_COMMAND_LIST_ITEM_SELECTED event handler for ID_SELVALCTRL
	void OnSelvalctrlSelected( wxListEvent& event );

	/// wxEVT_COMMAND_LIST_ITEM_DESELECTED event handler for ID_SELVALCTRL
	void OnSelvalctrlDeselected( wxListEvent& event );

	/// wxEVT_COMMAND_LIST_BEGIN_LABEL_EDIT event handler for ID_SELVALCTRL
	void OnSelvalctrlBeginLabelEdit( wxListEvent& event );

	/// wxEVT_COMMAND_LIST_END_LABEL_EDIT event handler for ID_SELVALCTRL
	void OnSelvalctrlEndLabelEdit( wxListEvent& event );

	/// wxEVT_COMMAND_LIST_ITEM_ACTIVATED event handler for ID_SELVALCTRL
	void OnSelvalctrlItemActivated( wxListEvent& event );

	/// wxEVT_COMMAND_TEXT_ENTER event handler for ID_VSELMACTEXT
	void OnVselmactextEnter( wxCommandEvent& event );

	/// wxEVT_COMMAND_TEXT_ENTER event handler for ID_VSELONETEXT
	void OnVselonetextEnter( wxCommandEvent& event );

	/// wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_VSELUPBUTTON
	void OnVselupbuttonClick( wxCommandEvent& event );

	/// wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_VSELDNBUTTON
	void OnVseldnbuttonClick( wxCommandEvent& event );

	/// wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_APPLYVSEL
	void OnApplyvselClick( wxCommandEvent& event );

	/// wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_VSELHELPBUTTON
	void OnVselhelpbuttonClick( wxCommandEvent& event );

	/// wxEVT_ENTER_WINDOW event handler for ID_OPTPANEL
	void OnEnterOptWindow( wxMouseEvent& event );

	/// wxEVT_COMMAND_RADIOBUTTON_SELECTED event handler for ID_OPTSYSYES
	void OnOptsysyesSelected( wxCommandEvent& event );

	/// wxEVT_COMMAND_RADIOBUTTON_SELECTED event handler for ID_OPTSYSNO
	void OnOptsysnoSelected( wxCommandEvent& event );

	/// wxEVT_COLOURPICKER_CHANGED event handler for ID_OPTCOLORSECTCTRL
	void OnOptcolorsectctrlColourPickerChanged( wxColourPickerEvent& event );

	/// wxEVT_COLOURPICKER_CHANGED event handler for ID_OPTCOLORSETCTRL
	void OnOptcolorsetctrlColourPickerChanged( wxColourPickerEvent& event );

	/// wxEVT_COLOURPICKER_CHANGED event handler for ID_OPTCOLORVARCTRL
	void OnOptcolorvarctrlColourPickerChanged( wxColourPickerEvent& event );

	/// wxEVT_COLOURPICKER_CHANGED event handler for ID_OPTCOLORDEFCTRL
	void OnOptcolordefctrlColourPickerChanged( wxColourPickerEvent& event );

	/// wxEVT_COLOURPICKER_CHANGED event handler for ID_OPTCOLORSYSCTRL
	void OnOptcolorsysctrlColourPickerChanged( wxColourPickerEvent& event );

	/// wxEVT_COMMAND_CHECKBOX_CLICKED event handler for ID_OPTADDINIHIST
	void OnOptaddinihistClick( wxCommandEvent& event );

	/// wxEVT_COMMAND_TEXT_UPDATED event handler for ID_OPTUSERNAMETEXT
	void OnOptusernametextTextUpdated( wxCommandEvent& event );

	/// wxEVT_COMMAND_TEXT_ENTER event handler for ID_OPTUSERNAMETEXT
	void OnOptusernametextEnter( wxCommandEvent& event );

	/// wxEVT_COMMAND_COMBOBOX_SELECTED event handler for ID_OPTBROWSERCTRL
	void OnOptbrowserctrlSelected( wxCommandEvent& event );

	/// wxEVT_COMMAND_TEXT_ENTER event handler for ID_OPTBROWSERCTRL
	void OnOptbrowserctrlEnter( wxCommandEvent& event );

	/// wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_OPTHELPBUTTON
	void OnOpthelpbuttonClick( wxCommandEvent& event );

////@end ConfigMgr event handler declarations

////@begin ConfigMgr member function declarations

	/// Retrieves bitmap resources
	wxBitmap GetBitmapResource( const wxString& name );

	/// Retrieves icon resources
	wxIcon GetIconResource( const wxString& name );
////@end ConfigMgr member function declarations

    /// Should we show tooltips?
  static bool ShowToolTips();

////@begin ConfigMgr member variables
	wxPanel* m_filepanel;
	wxRadioButton* m_projnamebutton;
	wxStaticText* m_projname;
	wxChoice* m_projnamesel;
	wxRadioButton* m_projstarttplbutton;
	wxStaticText* m_projstarttpl;
	wxChoice* m_projstarttplsel;
	wxRadioButton* m_filebrowselbutton;
	wxStaticText* m_filebrowse;
	wxGenericDirCtrl* m_projdirctrl;
	wxStaticText* m_projstartfile;
	wxTextCtrl* m_projstarttext;
	wxTextCtrl* m_projresult;
	wxButton* m_applyproj;
	wxButton* m_projhelpbutton;
	wxPanel* m_catpanel;
	wxStaticText* m_catstartfile;
	wxTextCtrl* m_catstarttext;
	wxRadioButton* m_catinitypebutton;
	wxChoice* m_catinichoice;
	wxRadioButton* m_catselbutton;
	wxTreeCtrl* m_sectyptree;
	wxRadioButton* m_catfilebutton;
	wxRadioButton* m_catallbutton;
	wxStaticText* m_projstart;
	wxStaticText* m_selstartsect;
	wxTextCtrl* m_projstartsecttxt;
	wxStaticText* m_selstartset;
	wxTextCtrl* m_projstartsettxt;
	wxStaticText* m_projstartwarn;
	wxTextCtrl* m_catresult;
	wxButton* m_applycat;
	wxButton* m_cathelpbutton;
	wxPanel* m_actpanel;
	wxStaticText* m_actstartfile;
	wxTextCtrl* m_actstarttext;
	wxRadioButton* m_sectaddbutton;
	wxRadioButton* m_sectvnaddbutton;
	wxRadioButton* m_sectaddsetbutton;
	wxRadioButton* m_sectaddvksetbutton;
	wxRadioButton* m_sectchangebutton;
	wxRadioButton* m_sectseteditbutton;
	wxRadioButton* m_sectdelbutton;
	wxRadioButton* m_sectdelsetbutton;
	wxRadioButton* m_sectrestbutton;
	wxRadioButton* m_sectrestsetbutton;
	wxRadioButton* m_sectrenbutton;
	wxRadioButton* m_sectrensetbutton;
	wxRadioButton* m_sectmovbutton;
	wxRadioButton* m_sectmovfbutton;
	wxRadioButton* m_sectmovsetbutton;
	wxRadioButton* m_sectmovsetfbutton;
	wxRadioButton* m_sectcpybutton;
	wxRadioButton* m_sectcpysetfbutton;
	wxRadioButton* m_sectdupbutton;
	wxRadioButton* m_sectdupsetbutton;
	wxTextCtrl* m_actresult;
	wxButton* m_applyact;
	wxButton* m_acthelpbutton;
	wxPanel* m_sectionpanel;
	wxStaticText* m_sectstartfile;
	wxTextCtrl* m_sectstarttext;
	wxStaticText* m_sectcat;
	wxTextCtrl* m_sectcattext;
	wxStaticText* m_sectact;
	wxTextCtrl* m_sectacttext;
	wxStaticText* m_selsect;
	wxListCtrl* m_sectctrl;
	wxTextCtrl* m_secthelptext;
	wxStaticText* m_sectvarname;
	wxStaticText* m_sectvnadd;
	wxTextCtrl* m_sectvnaddtext;
	wxStaticText* m_sectvnren;
	wxTextCtrl* m_sectvnrentext;
	wxCheckBox* m_sectformalias;
	wxTextCtrl* m_sectresult;
	wxButton* m_applysect;
	wxButton* m_secthelpbutton;
	wxPanel* m_settingpanel;
	wxStaticText* m_setstartfile;
	wxTextCtrl* m_setstarttext;
	wxStaticText* m_setcat;
	wxTextCtrl* m_setcattext;
	wxStaticText* m_setact;
	wxTextCtrl* m_setacttext;
	wxStaticText* m_setsect;
	wxTextCtrl* m_setsecttext;
	wxStaticText* m_setsecthelp;
	wxTextCtrl* m_setsecthelptext;
	wxStaticText* m_selset;
	wxListCtrl* m_setlistctrl;
	wxTextCtrl* m_macrotext;
	wxTextCtrl* m_sethelptext;
	wxBoxSizer* m_setvarkeysizer;
	wxStaticText* m_setvarkey;
	wxButton* m_setvarkeybutton;
	wxStaticText* m_setvkadd;
	wxTextCtrl* m_setvkaddtext;
	wxStaticText* m_setvkren;
	wxTextCtrl* m_setvkrentext;
	wxTextCtrl* m_setresult;
	wxButton* m_applyset;
	wxButton* m_sethelpbutton;
	wxPanel* m_valpanel;
	wxBoxSizer* m_valpanelsizer;
	wxStaticText* m_valstartfile;
	wxTextCtrl* m_valstarttext;
	wxStaticText* m_valcat;
	wxTextCtrl* m_valcattext;
	wxStaticText* m_valact;
	wxTextCtrl* m_valacttext;
	wxStaticText* m_valsect;
	wxTextCtrl* m_valsecttext;
	wxStaticText* m_valsecthelp;
	wxTextCtrl* m_valsecthelptext;
	wxBoxSizer* m_valsetsizer;
	wxStaticText* m_valset;
	wxTextCtrl* m_valsettext;
	wxBoxSizer* m_valsethelpsizer;
	wxStaticText* m_valsethelp;
	wxTextCtrl* m_valsethelptext;
	wxStaticText* m_selini;
	wxListCtrl* m_selinictrl;
	wxTextCtrl* m_valhelptext;
	wxBoxSizer* m_valcurrvalsizer;
	wxStaticText* m_currval;
	wxTextCtrl* m_currvaltext;
	wxTextCtrl* m_valresult;
	wxButton* m_applyval;
	wxButton* m_valhelpbutton;
	wxPanel* m_vselpanel;
	wxBoxSizer* m_vselpanelsizer;
	wxStaticText* m_vselstartfile;
	wxTextCtrl* m_vselstarttext;
	wxStaticText* m_vselcat;
	wxTextCtrl* m_vselcattext;
	wxStaticText* m_vselact;
	wxTextCtrl* m_vselacttext;
	wxStaticText* m_vselsect;
	wxTextCtrl* m_vselsecttext;
	wxStaticText* m_vselsecthelp;
	wxTextCtrl* m_vselsecthelptext;
	wxBoxSizer* m_vselsetsizer;
	wxStaticText* m_vselset;
	wxTextCtrl* m_vselsettext;
	wxBoxSizer* m_vselsethelpsizer;
	wxStaticText* m_vselsethelp;
	wxTextCtrl* m_vselsethelptext;
	wxStaticText* m_vselini;
	wxTextCtrl* m_vselinitext;
	wxBoxSizer* m_vseldestsizer;
	wxStaticText* m_vseldest;
	wxTextCtrl* m_vseldesttext;
	wxBoxSizer* m_vselrensizer;
	wxStaticText* m_vselren;
	wxTextCtrl* m_vselrentext;
	wxBoxSizer* m_vselvalsizer;
	wxStaticText* m_vselname;
	wxStaticText* m_vselval;
	wxTextCtrl* m_vselvaltext;
	wxBoxSizer* m_vselvalhelpsizer;
	wxBoxSizer* m_vselvalhelpvertsizer;
	wxStaticText* m_vselvalhelp;
	wxStaticText* m_vselnamehelp;
	wxButton* m_vselvalbutton;
	wxTextCtrl* m_vselvalhelptext;
	wxBoxSizer* m_vselctrlsizer;
	wxStaticText* m_selval;
	wxStaticText* m_vselfix;
	wxStaticText* m_vseldel;
	wxStaticText* m_vselmov;
	wxListCtrl* m_selvalctrl;
	wxStaticText* m_vselmacadd;
	wxStaticText* m_vselmacedit;
	wxTextCtrl* m_vselmactext;
	wxStaticText* m_vselone;
	wxTextCtrl* m_vselonetext;
	wxBoxSizer* m_vselcurrvalsizer;
	wxStaticText* m_vselcurrval;
	wxTextCtrl* m_vselcurrvaltext;
	wxBoxSizer* m_vselmovsizer;
	wxButton* m_vselupbutton;
	wxButton* m_vseldnbutton;
	wxTextCtrl* m_vselresult;
	wxButton* m_applyvsel;
	wxButton* m_vselhelpbutton;
	wxPanel* m_optpanel;
	wxBoxSizer* m_optsyssizer;
	wxStaticText* m_optsys;
	wxRadioButton* m_optsysyes;
	wxRadioButton* m_optsysno;
	wxStaticText* m_optcolors;
	wxStaticText* m_optcolorsect;
	wxTextCtrl* m_optcolorsecttext;
	wxColourPickerCtrl* m_optcolorsectctrl;
	wxStaticText* m_optcolorset;
	wxTextCtrl* m_optcolorsettext;
	wxColourPickerCtrl* m_optcolorsetctrl;
	wxStaticText* m_optcolorvar;
	wxTextCtrl* m_optcolorvartext;
	wxColourPickerCtrl* m_optcolorvarctrl;
	wxStaticText* m_optcolordef;
	wxTextCtrl* m_optcolordeftext;
	wxColourPickerCtrl* m_optcolordefctrl;
	wxStaticText* m_optcolorsys;
	wxTextCtrl* m_optcolorsystext;
	wxColourPickerCtrl* m_optcolorsysctrl;
	wxCheckBox* m_optaddinihist;
	wxStaticText* m_optusername;
	wxTextCtrl* m_optusernametext;
	wxStaticText* m_optbrowser;
	wxComboBox* m_optbrowserctrl;
	wxButton* m_optlhelpbutton;
////@end ConfigMgr member variables

	// added variables used in cmdcl.cpp

	// added functions in cmdcl.cpp
	wxPoint *InitPos;
	wxSize *InitSize;
	wxPoint *EndingPos;
	wxSize *EndingSize;
	void InitializeDCL(void);
	void EndingUpdate(void);


	wxString ProjSrcDir;
	wxString ProjOutDir;
	void SetUpProjList(void);
	void ProcProjListChoice(int idx);

	void SetCommandLineSwitches(void);
	void SetActButton(void);

	void SetUpStartsList(void);
	void ProcStartsListChoice(int idx);

	wxString ProjAlias;
	void SetIniChoice(void);
	void SetStartFileText(char *msg);
	void ApplyFiles(void);


	wxString CatAlias;
	void SelectIniType(void);
  void SetUpCategoriesTree(void);
	void SelectCategories(void);
	char *MakeShowName(char *currName);
	void ApplyCategories(void);


	wxString ActAlias
;
	void ShowAction(bool clear = false);
	void AddSectionsToShow(const char *name, void *vp, bool all);
	void ApplyAction(void);


	wxArrayString SectionsShown;
	wxString SectionSelected;
	wxString SectionHelpShown;
	long SectionSelectedPos;
	wxString SectAlias;

	void ShowSections(const char *selname = NULL);
	void SetUpSectionCtrl(void);
	void SetSectButtons(long pos);
	void SetAvailSection(void);
	void DeselectSections(void);
	void ApplySections(const char *showset = NULL);


	wxArrayString SettingsShown;
	wxArrayString SettingValsShown;
	long SettingsShownCurr;
	long SelSetPos;
	wxString SettingSelected;
	wxString SettingNameShown;
	long SettingNameShownPos;
	wxString SettingValShown;
	wxString SettingHelpShown;
	wxString SetAlias;

	void ShowSettings(const char *setname = NULL, bool vname = false);
	void SetUpSettingsCtrl(void);
	void SetSetButtons(long pos);
  void SetAvailSetting(long lpos);
	void DeselectSettings(void);
	void ProcVarKeySelect(void);
	bool CheckSettingName(wxString& name);
	void ApplySettings(void);
	void SetUpValues(bool sectop = false, bool vkset = false);
	wxString GetIniScope(CIni *ci);
	void UpdateSettingSelections(void);
	void UpdateValueSelections(void);
	void SetValVisibility(bool sectop);


	wxArrayString ValuesShown;
	wxArrayString ValueScopes;
	wxArrayString ValueFiles;
	wxArrayString ValueSys;
	wxString IniNameShown;
	wxString IniValShown;
	long SelIniSelection;
	wxString ValAlias;

	void ShowValues(bool sectop = false);
  void SetUpSelIniCtrl(void);
  void AdjustIniSel(void);

	long SrcIniPos;
	wxString SrcIniName;
	wxString SrcIniVal;
	CIni *SrcIniSel;
	long DestIniPos;
	wxString DestIniName;
	wxString DestIniVal;
	CIni *DestIniSel;
	wxStaticText *m_vselmac;

  void ApplyIniVals(void);
  void SetSrcIniVars(void);
  void SetDestIniVars(void);
	void SetVselVisibility(bool macro = false, bool list = true);


	wxArrayString SettingValsAvailable;
	wxArrayString SettingValsHelp;
	long SettingValsPos;
	bool VselSelectionLocked;
	long StartValPos;
	long EndValPos;
	wxString EditedSelVal;
	long EditedValPos;
	bool EditingVal;
	bool SingleValSet;
	bool InitialSelVal;
	wxString SelValSelected;
	wxString VselAlias;
	wxString InitialValue;

	void ShowSelValues(bool macro = false, bool list = true);
  void SetUpSelValCtrl(void);
  void ValEdited(void);
  void ProcSelButtonSelect(void);
  void ProcSelValSelect(long pos);
  void MoveSelVal(bool up);
  void ApplySelVals(void);
  void UpdateValueSelected(wxString &val);
	bool IsEffectiveVal(long inipos);
	void AddHist(const char *act, const char *obj, const char *from = NULL,
	             const char *mv = NULL, const char *fromto = NULL);


	wxString MacroValue;
	bool ShowingMacro;
	bool ShowingList;
	bool GetMacroValue(void);

	wxColour *SectColor;
	wxColour *SetColor;
	wxColour *VarColor;
	wxColour *DefColor;
	wxColour *SysColor;
	wxColour *InvalidColor;
	wxString OptAlias;

  void SysIniSelect(bool yn);
	void InitColors(void);

	wxString GetAlias(const wxString &sect, const wxString &set = wxString(""));
	void CallHelp(const wxString& alias);

	bool UpdateIniFile(const char *sect, const char *set,
	                   const char *sval, const char *path);
	bool GetIniFile(const char *sect, const char *set, const char *dval,
	                char *val, unsigned long len, const char *path);
	bool GetIniFileAll(const char *sect, const char *set, 
	                   char *val, unsigned long len, const char *path);
	bool DeleteIniFileAll(const char *sect, const char *set,
		                    const char *path);
	bool ChangeSectIniFile(const char *sect, const char *sval,
	                       const char *path);
	bool MovSectIniFile(const char *sect, long opos, long npos,
	                    const char *path);
	bool MovSetIniFile(const char *sect, const char *set,
	                   long opos, long npos, const char *path);
	bool RestoreIniFile(const char *sect, const char *set,
	                    const char *path);
	bool RenSetIniFile(const char *sect, const char *set,
			               const char *nset, const char *path);
	bool RenSectIniFile(const char *sect, const char *nsect,
			                const char *path);
	void ReportMoveErr(void);

	wxString ResultStr;
	char *InvokeChildCM(const char *valuetype, const char *sect = NULL);
	void RunChildCM(const char *childcat, const char *sect = NULL);
	void WriteResultFile(wxString &res);
	void EndChildCM(void);
	void ReadResultFile(void);

	char *GetFileInfo(long typ);

};

#endif
    // _CONFIGMGR_H_
