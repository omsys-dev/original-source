/* m2rbook.h is the FDK header for mif2rtf FM 5.5 book export 
 * Last edited on 11/30/98 by Jeremy H. Griffith.
 * Copyright 1998 Omni Systems, Inc.  All Rights Reserved.
 */

#ifndef M2RBOOK_H
#define M2RBOOK_H

/* system headers */
#include <stdio.h>
#include <time.h>
#include <direct.h>
#include <io.h>
#include <sys\stat.h>

// Windows specific
#include <windows.h>
#include <iostream.h>
#include "Shlwapi.h"

/* FDK headers */
#include "fapi.h"
#include "f_types.h"
//#include "fdetypes.h"
#include "fcodes.h"
#include "fstrings.h"
#include "fmemory.h"
#include "futils.h"
#include "fmetrics.h"
#include "fcharmap.h"

#undef getenv
#undef strncat
#undef strncpy

/* help system IDs */
#include "m2rhids.h"

/* global definitions */
#define EXPORT_FILE 1
#define EXPORT_BOOK 2
#define EXPORT_SETUP 3
#define EXPORT_BK_SETUP 4
#define CONVERSION_DES 5
#define CVT_FILE 6
#define CVT_BOOK 7
#define REP_CVT  8
#define NEW_PRJ  9
#define MOD_PRJ 10
#define SEL_PRJ 11
#define CON_DES 12
#define EDIT_INI 13
#define WASH_FILE 14
#define WASH_BOOK 15
#define WASH_BOOK_ALL 16

#define DLG_CHOOSE_PROJECT  1
#define DLG_SETUP_HLP_INI   2
#define DLG_SETUP_WRD_INI   3
#define DLG_SETUP_HTML_INI  4
#define DLG_SETUP_FMIF_INI  5
#define DLG_SETUP_ADCL_INI  6
#define DLG_SETUP_MSHH_INI  7
#define DLG_SETUP_JAVH_INI  8
#define DLG_SETUP_OMH_INI   9
#define DLG_SETUP_EH_INI   10
#define DLG_SETUP_DITA_INI 11
#define DLG_SETUP_DCBK_INI 12
#define DLG_INIT    13
#define DLG_DEF_VAR 14

#define DLG_COUNT 14

#define DLG_DESIGNER_BASE 16
#define DLG_DES_HS_BASIC   1
#define DLG_DES_HS_TOPIC   2
#define DLG_DES_HS_HOTSP   3
#define DLG_DES_HS_TARGET  4
#define DLG_DES_HS_MACRO   5
#define DLG_DES_TX_BASIC   6
#define DLG_DES_TX_LAYOUT  7
#define DLG_DES_GR_BASIC   8
#define DLG_DES_GR_IMPORT  9
#define DLG_DES_GR_TEXT   10
#define DLG_DES_TX_TABLE  11
#define DLG_DES_TX_FONT   12
#define DLG_DES_TX_ADV    13
#define DLG_DES_TX_HELP   14
#define DLG_DES_TX_VER    15
#define DLG_DES_TX_CNT    16
#define DLG_DES_TX_HPJ    17
#define DLG_DES_TX_TPL    18
#define DLG_DES_TX_HMK    19
#define DLG_DES_TX_MULT   20
#define DLG_DES_GR_FRM    21
#define DLG_DES_GR_BMP    22

#define DLG_DES_COUNT     22

#define PT_WORD7  1
#define PT_WORD8  2
#define PT_WORDP  3
#define PT_WH3    4
#define PT_WH4    5
#define PT_HTML   6
#define PT_JAVH   7
#define PT_MSHH   8
#define PT_OMH    9
#define PT_ORH   10
#define PT_EH    11
#define PT_XHTM  12
#define PT_DITA  13
#define PT_DCBK  14
#define PT_XML   15
#define PT_MIF   16
#define PT_DCL   17
#define PROJ_TYPES 17
#define PT_PDF   18
#define PT_MUL   19

#define PT_wh(x) (((x)==PT_WH3)||((x)==PT_WH4))
#define PT_java(x) (((x)==PT_JAVH)||((x)==PT_ORH))
#define PT_hh(x) (((x)==PT_JAVH)||((x)==PT_MSHH)||((x)==PT_ORH)||((x)==PT_EH)||((x)==PT_OMH))
#define PT_help(x) (PT_wh(x)||PT_hh(x))
#define PT_htm(x) (((x)>=PT_HTML)&&((x)<=PT_XML))
#define PT_ghtm(x) (((x)==PT_HTML)||((x)==PT_XHTM)||((x)==PT_XML))
#define PT_xml(x) (((x)==PT_DITA)||((x)==PT_DCBK)||((x)==PT_XML))
#define PT_word(x) (((x)==PT_WORD7)||((x)==PT_WORD8)||((x)==PT_WORDP))
#define PT_rtf(x) (PT_wh(x)||PT_word(x))
#define PT_comp(x) (((x)==PT_WH3)||((x)==PT_WH4)||((x)==PT_MSHH))

#define GT_EQN  1
#define GT_ALL  2

#define GF_BMP   1
#define GF_TIFF  2
#define GF_WMF   3
#define GF_JPEG  4
#define GF_PNG   5
#define GF_EPS   6
#define GF_PICT  7
#define GF_CGM   8
#define GF_IGES  9
#define GF_GIF  10
#define GF_SVG  11
#define GF_PDF  12
#define GF_LAST 12

#define BF_START_CMD 1
#define BF_WRAP_CMD 2
#define BF_END_CMD 3

#define BF_FILE False
#define BF_BOOK True

#define CNT_NONE 1
#define CNT_BODY 2
#define CNT_FULL 3

#define RF_NONE 1
#define RF_BODY 2
#define RF_FULL 3
#define RF_HHW  4

#define LST_NONE 1
#define LST_TOC  2
#define LST_IX   3
#define LST_BOTH 4

#define EOD_NEVER 1
#define EOD_FILE  2
#define EOD_BOOK  3
#define EOD_ALWAYS 4

#define PP1  1
#define PP2  2
#define PP3  3

#define DES_PROPSET_WORD 0
#define DES_PROPSET_HELP 1
#define DES_PROPSET_HTML 2

#define DES_NAMES_ALL_STYLES 0
#define DES_NAMES_STYLES     1
#define DES_NAMES_FLOWS      2
#define DES_NAMES_ALL_FORMS  3
#define DES_NAMES_GR_FORMS   4
#define DES_NAMES_GR_TEXT    5
#define DES_NAMES_FONTS      6
#define DES_NAMES_FILES      7
#define DES_NAMES_FRAMES     8
#define DES_NAMES_BITMAPS    9

#define DES_NAMES_COUNT     10


/* in m2rbook.c */
extern StringT PluginVersion;
extern IntT FrameVersion;

// need this until using FDK8/9
#define FV_SaveFmtInterchange70	14
#define FV_SaveFmtInterchange80	15
#define FV_SaveFmtInterchange90	17
#define FV_SaveFmtInterchange100	22
#define FV_SaveFmtInterchange110	24
#define FV_SaveFmtInterchange120	26

#define FV_SaveFmtBinary70			12	
#define FV_SaveFmtBinary80			13
#define FV_SaveFmtBinary90			16
#define FV_SaveFmtBinary100			21
#define FV_SaveFmtBinary110			23
#define FV_SaveFmtBinary120			25

extern VoidT ExportFile(VoidT);
extern VoidT ExportBook(VoidT);

extern VoidT SetupExport(VoidT);
extern VoidT SetupBkExport(F_ObjHandleT bookId);
extern VoidT RunSetupDialog(StringT name, F_ObjHandleT id);

extern VoidT WashFile(VoidT);
extern VoidT WashBook(VoidT);
extern VoidT WashBookAll(VoidT);
extern F_ObjHandleT WashOne(F_ObjHandleT docId);

/* in m2rmenu.c */
extern VoidT CreateFMMenu(VoidT);

/* in m2rproj.c */
extern IntT DlgIds[DLG_COUNT];

extern StringT CurrProject;
extern StringT CurrProjPath;
extern UCharT CurrProjType;
extern StringT CurrPrjFilePath;
extern IntT ProjSelType;

extern StringT TypeNameList[PROJ_TYPES + 1];
extern UCharT TypeIdList[PROJ_TYPES];
extern BoolT ProjCancelled;
extern BoolT ProjRemote;

extern VoidT EditProjDialog(StringT filename);
extern VoidT ProcProjDialogEvent(IntT itemNum);

extern VoidT SelectProject(IntT sel);
extern VoidT ConfigureProject(VoidT);
extern VoidT AddProject(StringT newname, StringT newpath, IntT newtype);
extern VoidT DelProject(VoidT);
extern VoidT GetProjPath(StringT *npp);
#ifdef NORECURSE
extern BoolT GettingProjPath;
#endif

extern BoolT RunfmDiagnostics;
extern BoolT revOn;
extern BoolT revOff;
extern BoolT printBook;
extern BoolT printDoc;
extern BoolT pdfBook;
extern BoolT pdfDoc;
extern BoolT pdfSaveBook;
extern BoolT pdfSaveDoc;
extern BoolT isBook;
extern BoolT isDoc;
extern StringT filename;
extern StringT bookname;
extern StringT printname;
extern StringT pdfname;
extern StringT projname;
extern F_ObjHandleT bookId;
extern F_ObjHandleT docId;
extern StringT ProjSetting;

typedef VoidT (*setfunc)(StringT);
extern setfunc ProjFunc;

extern VoidT ProcProjSet(StringT projset);
extern VoidT InitProjSet(StringT arg);
extern VoidT LastFirstProjSet(StringT arg);
extern VoidT PrintBookSet(StringT arg);
extern VoidT PrintDocSet(StringT arg);
extern VoidT PdfBookSet(StringT arg);
extern VoidT PdfDocSet(StringT arg);
extern VoidT PdfSaveBookSet(StringT arg);
extern VoidT PdfSaveDocSet(StringT arg);
extern VoidT RunBookSet(StringT arg);
extern VoidT RunDocSet(StringT arg);
extern VoidT TemplateSet(StringT arg);
extern VoidT ProjectSet(StringT arg);

extern VoidT ProcProject(F_ObjHandleT docId, StringT projname);

/* in m2rset.c */
#ifndef CFGTPL
#define DLG_TF_HELP     23
#define DLG_TF_OK       24
#define DLG_TF_CANCEL   25
#endif

#ifdef CFGTPL
#define DLG_TF_HELP     26
#define DLG_TF_OK       27
#define DLG_TF_CANCEL   28
#endif

extern BoolT SetupCancelled;

extern BoolT ConfigTemplate;
extern StringT ConfigName;

extern VoidT SetFrameTemplateDlg(F_ObjHandleT dlgId, IntT first);
extern VoidT GetFrameTemplateDlg(F_ObjHandleT dlgId, IntT first);
extern VoidT ProcFrameTemplateDlgEvent(F_ObjHandleT dlgId, IntT first, IntT itemNum);
extern VoidT WriteIniCommonVars(BoolT fmfiles, BoolT idpath);

/* in m2rseta.c */
extern VoidT SetupADCLIniDialog(StringT filename, F_ObjHandleT bookId);
extern VoidT ProcSetupADCLIniDialogEvent(IntT itemNum);

extern VoidT WriteIniAVars(VoidT);
extern VoidT SetImpGrDef(F_ObjHandleT dlgId, IntT gdef, BoolT set);
extern VoidT PutGraphExport(FILE *fini);
extern VoidT PutGraphExportP(VoidT);

/* in m2rsetf.c */
extern BoolT MakeBook;
extern BoolT OrigExtForMIF;

extern VoidT SetupFMIFIniDialog(StringT filename, F_ObjHandleT bookId);
extern VoidT ProcSetupFMIFIniDialogEvent(IntT itemNum);
extern VoidT WriteIniFVars(VoidT);

/* in m2rseth.c */
extern VoidT SetupHelpIniDialog(StringT filename, F_ObjHandleT bookId);
extern VoidT ProcSetupHelpIniDialogEvent(IntT itemNum);
extern VoidT WriteIniHVars(BoolT book, UCharT form);

/* in m2rsetw.c */
extern VoidT SetupWordIniDialog(StringT filename, F_ObjHandleT bookId);
extern VoidT ProcSetupWordIniDialogEvent(IntT itemNum);
extern VoidT WriteIniWVars(VoidT);

/* in m2rsetn.c */
extern VoidT SetupHTMLIniDialog(StringT filename, F_ObjHandleT bookId);
extern VoidT ProcSetupHTMLIniDialogEvent(IntT itemNum);
extern VoidT WriteIniNVars(VoidT);

/* in m2rsetd.c */
extern StringT DITADefTopic;
extern StringT DITADefPara;
extern StringT DITADefChar;
extern BoolT DITADefVer11;
extern VoidT SetupDITAIniDialog(StringT filename, F_ObjHandleT bookId);
extern VoidT ProcSetupDITAIniDialogEvent(IntT itemNum);
extern VoidT WriteIniDVars(VoidT);

/* in m2rsetb.c */
extern BoolT DocBookChapFull;
extern StringT DocBookRoot;
extern BoolT UseCALSTableModel;
extern StringT DocBookName;
extern StringT DocBookTitle;
extern VoidT SetupDocBookIniDialog(StringT filename, F_ObjHandleT bookId);
extern VoidT ProcSetupDocBookIniDialogEvent(IntT itemNum);
extern VoidT WriteIniBVars(VoidT);

/* in m2rsetj.c */
extern VoidT SetupJHIniDialog(StringT filename, F_ObjHandleT bookId);
extern VoidT ProcSetupJHIniDialogEvent(IntT itemNum);
extern VoidT WriteIniJVars(StringT filename, BoolT book, UCharT form);

/* in m2rsete.c */
extern VoidT SetupEHIniDialog(StringT filename, F_ObjHandleT bookId);
extern VoidT ProcSetupEHIniDialogEvent(IntT itemNum);
extern StringT PluginName;
extern StringT PluginID;
extern StringT Provider;
extern BoolT WritePluginFile;
extern BoolT WriteContextFile;
extern BoolT GenCSH;
extern VoidT WriteIniEVars(BoolT book);

/* in m2rsetm.c */
extern BoolT CshMapFileSet;
extern VoidT SetupHHIniDialog(StringT filename, F_ObjHandleT bookId);
extern VoidT ProcSetupHHIniDialogEvent(IntT itemNum);
extern VoidT WriteIniMVars(StringT filename, BoolT book);

/* in m2rsetc.c */
extern VoidT SetupOMHIniDialog(StringT filename, F_ObjHandleT bookId);
extern VoidT ProcSetupOMHIniDialogEvent(IntT itemNum);
extern VoidT WriteIniCVars(BoolT book);



/* in m2rdlg.c */
extern VoidT EnableItems(F_ObjHandleT dlgId, IntT first, IntT last, BoolT state);
extern VoidT EnableDlgItem(F_ObjHandleT dlgId, IntT item, BoolT state);
extern IntT GetDlgState(F_ObjHandleT dlgId, IntT item);
extern VoidT SetDlgState(F_ObjHandleT dlgId, IntT item, IntT state);
extern StringT GetDlgText(F_ObjHandleT dlgId, IntT item);
extern VoidT SetDlgText(F_ObjHandleT dlgId, IntT item, StringT text);
extern VoidT SetDlgLabel(F_ObjHandleT dlgId, IntT item, StringT text);
extern VoidT SetLabels(F_ObjHandleT dlgId, IntT item, F_StringsT *psList);
extern VoidT SetStyleListState(F_ObjHandleT dlgId, IntT item, StringT text);
extern StringT GetStyleListText(F_ObjHandleT dlgId, IntT item);
extern VoidT SetGrFormListState(F_ObjHandleT dlgId, IntT item, StringT text);
extern StringT GetGrFormListText(F_ObjHandleT dlgId, IntT item);
extern VoidT SetGrExFormListState(F_ObjHandleT dlgId, IntT item, StringT text);
extern StringT GetGrExFormListText(F_ObjHandleT dlgId, IntT item);
extern VoidT SetFontListState(F_ObjHandleT dlgId, IntT item, StringT text);
extern StringT GetFontListText(F_ObjHandleT dlgId, IntT item);
extern IntT GetRadValue(F_ObjHandleT dlgId, IntT first, IntT last);
extern VoidT SetDlgProject(F_ObjHandleT dlgId, IntT item);


/* in m2rsty.c */
typedef struct {
	StringT name;
	BoolT para;
	BoolT used;
	BoolT remap;
	StringT rem_name;
	BoolT replace;
	StringT rep_name;
	IntT helptyp;
	BoolT usekey;
	BoolT akey;
	StringT keys;
	IntT topictyp;
	BoolT browse;
	StringT br_px;
	BoolT cont;
	IntT conttyp;
	StringT contlev;
	BoolT window;
	StringT win_name;
	BoolT notitle;
	BoolT titsx;
	StringT title_sx;
	BoolT refer;
	StringT ref_sx;
	BoolT noscroll;
	BoolT noscr_used;
	BoolT xtscroll;
	BoolT xtscr_used;
	IntT hotsptyp;
	BoolT fileref;
	StringT file_name;
	BoolT local;
	BoolT fixed;
	StringT fixed_name;
	BoolT ungreen;
	BoolT nouline;
	BoolT pop_cont;
	StringT mac_name;
	BoolT build;
	StringT build_tag;
	VoidT *next;
} DesStyleT;

extern DesStyleT *DesStyles;
extern IntT DesStyleCount;
extern DesStyleT *CurrDesStyle;
extern DesStyleT FirstDesStyle;
extern DesStyleT *LastDesStyle;
extern DesStyleT ClipboardStyle;
extern DesStyleT EmptyStyle;
extern BoolT StylesChanged;

extern StringT *DesStyleList;
extern IntT DesListCount;
extern IntT CurrStyle;
extern BoolT StyleListChanged;

extern BoolT ShowPara;
extern BoolT ShowChar;

extern F_StringsT StyleStrings;

extern VoidT InitDesignerStyles(VoidT);
extern VoidT InitStyleList(VoidT);
extern VoidT GetDocStyles(F_ObjHandleT docId, BoolT isPara);
extern BoolT InDesList(StringT sname);
extern IntT CmpStrings(const VoidT *p1, const VoidT *p2);
extern DesStyleT *CreateDesignerStyle(StringT sname);
extern VoidT FreeDesignerStyles(VoidT);
extern VoidT SetNewStyle(IntT stylenum);
extern DesStyleT *FindStyleInDesStyles(StringT sname);
extern IntT FindStyleInList(StringT sname);

extern VoidT ReadStylesFromIni(StringT ininame, BoolT confirm);
extern VoidT ReadStylesFromIniSect(StringT ininame, IntT sectnum);
extern VoidT GetIniStyleProps(StringT ininame, IntT sectnum,
															StringT props, DesStyleT *curr);
extern VoidT WriteStylesToIni(StringT ininame, BoolT confirm);


/* in m2rtxt.c */
typedef struct {
	// Basic Styles
	BoolT rem_unused_sty;  // remove unused styles

	// Fonts
	StringT def_fwid;
	StringT def_lspace;
	StringT def_font;
	StringT def_fsize;

	StringT wmf_font;
	StringT wmf_fsize;

	BoolT bmc_used;
	StringT bmc_font;

	BoolT rem_unused_fonts;

	// Basic Text:
	BoolT hide_white;
	BoolT keep_color;
	BoolT hyper_color;
	BoolT keep_anchor;
	BoolT char_styles_used;
	BoolT exact_space;
	IntT small_caps;  // Standard, None
	IntT runin_heads; // Normal, RunIn

	IntT rm_tabs;   // None, Left, Right, Both

	IntT sidehds;   // Left, Indent, Frame, Normal
	BoolT anchor_spanners;
	StringT sidehd_vs; // vert space in twips

	IntT fnotes;    // W: Standard*, Embed, None, Variable=5
									// H: Standard, Embed, None, Jump*, Popup

	// Text Layout:
	BoolT single_flow;
	BoolT text_frames;
	BoolT tframe_wrap;
	StringT tcols;

	IntT pg_break;  // Normal, Format, Remove

	IntT hd_ft;     // Standard, TextOnly, None
	StringT hdft_vs;   // vert twips
	BoolT hf_framed;
	BoolT hfframe_wrap;
	StringT hfframe_sp;

	// Tables:
	IntT tbl_titles;

	StringT tbcol_scale;
	StringT tbcol_add;

	IntT tbl_rules;

	IntT tbl_graphics;

	BoolT tbl_strip;

	BoolT tbl_use_cont;
	StringT tbl_cont_var;
	BoolT tbl_use_sheet;
	StringT tbl_sheet_var;

	// Advanced Text
	IntT idx_usage;
	IntT xref_usage;
	IntT xref_type;
	IntT quote_usage;
	IntT occluded_tabs;

	BoolT frame_spacing;
	BoolT char_styles;
	BoolT secbk_help;
	BoolT secbk_word;

	// Help Text
	BoolT scroll_title;
	BoolT xnscroll_title;
	BoolT ind_title;
	BoolT space_title;
	BoolT linebk_title;

	StringT browse_digits;
	StringT browse_step;
	StringT browse_start;
	StringT browse_prefix;

	BoolT bullet_bmc;
	StringT bullet_file;
	BoolT dingbat_bmp;

	StringT keyword_limit;
	StringT help_tab_limit;

	// Version
	IntT output;
	BoolT word8;
	BoolT wordperf;

	StringT word_suf;
	StringT wp_suf;

	IntT helpver;
	BoolT altura;
	BoolT hyperhelp;
	BoolT force_bmc;

	// Help Contents
	IntT cnt_type;

	BoolT cnt_name_used;
	StringT cnt_name;

	BoolT cnt_base_used;
	StringT cnt_base;

	BoolT	cnt_title_used;
	StringT	cnt_title;

	BoolT cnt_start_file_used;
	StringT cnt_start_file;

	BoolT cnt_top_head_used;
	StringT cnt_top_head;

	BoolT cnt_b_topic_used;
	StringT cnt_b_topic;

	BoolT cnt_files;
	BoolT cnt_wins;
	StringT cnt_main_win;

	// Help Compile
	BoolT comb_cnt;
	BoolT comp_hlp;

	StringT hpj_name;
	StringT hlp_name;
	StringT hlp_title;
	StringT contents_ref;

	StringT copyr_txt;
	BoolT copyr_date;

	// Templates
	BoolT fm_tpl_apply;
	StringT fm_tpl_name;
	IntT fm_tpl_flags;

	BoolT wd_tpl_apply;
	StringT wd_tpl_name;
	BoolT wd_tpl_auto;

	// Help Markers
	IntT alink_mark;
	StringT alink_cust;

	IntT typ11_mark;

	IntT directive_mark;
	StringT directive_cust;

	BoolT no_xref_popup;

	// MultiFile
	BoolT use_file_ids;
	BoolT use_graph_fids;
	BoolT use_fm_toc;
	BoolT use_fm_ix;
	IntT object_ids;
	StringT hlp_xref_default;
	StringT hlp_xref_suf;

} TextPropT;

extern TextPropT CurrDesText;
extern TextPropT DefDesText;
extern TextPropT WDefDesText;
extern TextPropT HDefDesText;
extern TextPropT ClipboardText;
extern BoolT TextChanged;

extern StringT WordOptions;
extern StringT HelpOptions;
extern StringT SubOptions;
extern StringT Options;

extern VoidT ReadTextPropsFromIni(StringT ininame, BoolT confirm);
extern VoidT WriteTextPropsToIni(StringT ininame, BoolT confirm);

typedef struct {
	StringT name;
	IntT status;   // Seperate, Normal, Skip
	VoidT *next;
} TextFlowT;

extern TextFlowT *CurrTextFlow;
extern TextFlowT *DesTextFlows;
extern TextFlowT DefTextFlow;
extern TextFlowT FirstTextFlow;
extern TextFlowT *LastTextFlow;
extern TextFlowT ClipboardTextFlow;
extern BoolT TextFlowChanged;

extern StringT *DesTextFlowList;
extern IntT CurrTextFlowNum;
extern IntT DesTextFlowCount;
extern IntT DesTextFlowAlloc;
extern BoolT TextFlowListChanged;

extern VoidT InitDesignerFlows(VoidT);
extern VoidT GetDocFlows(F_ObjHandleT docId);
extern BoolT InFlowList(StringT sname);
extern VoidT InitFlowList(VoidT);
extern VoidT SetNewTextFlow(IntT flownum);
extern VoidT FreeDesignerFlows(VoidT);
extern VoidT ReadFlowsFromIni(StringT ininame, BoolT confirm);
extern VoidT WriteFlowsToIni(StringT ininame, BoolT confirm);

typedef struct {
	StringT family;
	IntT ftyp;
	StringT fwid;
	BoolT remap;
	StringT rem_name;
	VoidT *next;
} FontT;

extern FontT *CurrFont;
extern FontT *DesFonts;
extern FontT DefFont;
extern FontT FirstFont;
extern FontT *LastFont;
extern FontT ClipboardFont;
extern BoolT FontChanged;

extern StringT *DesFontList;
extern IntT CurrFontNum;
extern IntT DesFontCount;
extern IntT DesFontAlloc;
extern BoolT FontListChanged;

extern F_StringsT FontFamilies;
extern F_StringsT FontStrings;

typedef struct {
	StringT name;
	IntT typ;
	StringT wid;
	StringT rmap;
} DFPropT;

extern DFPropT DefFontProps[];
extern FontT *LastDefFont;

extern VoidT InitDesignerFonts(VoidT);
extern VoidT GetSessionFonts(VoidT);
extern VoidT GetDefFontProps(VoidT);
extern VoidT AddDesFont(StringT fam);
extern BoolT InFontList(StringT sname);
extern VoidT InitFontList(VoidT);
extern VoidT SetNewFont(IntT fontnum);
extern VoidT FreeDesignerFonts(VoidT);
extern IntT FindFontInList(StringT sname);
extern VoidT ReadFontsFromIni(StringT ininame, BoolT confirm);
extern VoidT WriteFontsToIni(StringT ininame, BoolT confirm);


/* in m2rgr.c */

typedef struct {

	// Basic Graphics
	BoolT external;

	BoolT fdkprv_used;
	StringT fdkprv_dpi;

	StringT fdkgr_form;

	BoolT fdkeqn_used;
	StringT fdkeqn_dpi;
	StringT fdkeqn_frm;

	BoolT fexact;
	StringT eqn_vert;
	StringT gr_vert;

	BoolT name_bmp;
	StringT wmfname_chars;
	StringT wmfname_digits;

	// Imported Graphics
	StringT exname_chars;
	StringT exname_digits;

	BoolT strip_path;
	BoolT remap_name;

	BoolT wrap_bmp;
	BoolT wrap_wmf;
	BoolT wrap_eqn;

	IntT epsi_usage;

	// Graphic Text
	IntT graph_text;

	IntT clip_type;
	StringT clip_limit;
	
	StringT text_scale;
	StringT text_width;
	StringT text_vert;

	IntT back_mode;

	BoolT sp_above;
	BoolT blank_first;

	// Graphic Frames
	IntT refframe_typ;
	StringT refframe_form;

	BoolT frstyle_used;
	StringT frstyle_name;

	IntT empty_frames;
	IntT ras_borders;

	// Bitmaps
	BoolT bitmap_rescale;
	StringT bitmap_dpi;

	BoolT bitmap_flip;
	BoolT bitmap_trans;

	BoolT compress_bmps;
	IntT stretch_mode;

} GraphPropT;

extern BoolT GraphicsChanged;
extern GraphPropT CurrDesGraph;
extern GraphPropT WDefDesGraph;
extern GraphPropT HDefDesGraph;
extern GraphPropT DefDesGraph;
extern GraphPropT ClipboardGraph;
extern StringT FDK;
extern StringT Graphics;


#define DES_GRX_FORM_COUNT GF_LAST

extern StringT DesGrFormList[DES_GRX_FORM_COUNT + 2];
extern IntT DesGrFormVal[DES_GRX_FORM_COUNT + 1];
extern IntT DesGrFormMap[DES_GRX_FORM_COUNT + 1];
extern F_StringsT GrFormStrings;
extern IntT FindGrFormInList(StringT fname);

#define DES_GRI_FORM_COUNT 12
extern StringT ImportDesNames[DES_GRI_FORM_COUNT + 1];

typedef struct {
	StringT suf;
	BoolT retain;
	StringT remap;
} GrExPropT;

extern BoolT ExportGraphsChanged;
extern GrExPropT ExportGraphs[DES_GRI_FORM_COUNT + 1];
extern GrExPropT ExportGraphDef[DES_GRI_FORM_COUNT + 1];
extern GrExPropT *CurrExportGraph;
extern IntT CurrExportGraphNum;
extern GrExPropT ClipboardExportGraph;
extern IntT ClipboardExGrNum;

extern IntT FindGrExFormInList(StringT fname);
extern VoidT SetNewGrForm(IntT formnum);
extern VoidT ReadExportGraphPropsFromIni(StringT ininame, BoolT confirm);
extern VoidT WriteExportGraphPropsToIni(StringT ininame, BoolT confirm);
extern VoidT ReadGraphPropsFromIni(StringT ininame, BoolT confirm);
extern VoidT WriteGraphPropsToIni(StringT ininame, BoolT confirm);


/* in m2rdes.c */

typedef struct {
	StringT listHead;  // first term in list
	StringT listLab;  // label for list in dialog, like "Paragraph Tag:"
	StringT applyTo;  // label under Apply, like "Selected Style"
	StringT *nameList; // made up from names in baseSet at runtime
	IntT nameCount;
	IntT nameSel;
} NameListT;

extern NameListT DesNameLists[DES_NAMES_COUNT];

extern IntT CurrDesigner;
extern StringT CurrIni;
extern IntT CurrPropSetNum;
extern BoolT DesignerChanged;
extern F_StringsT NameStrings;

extern VoidT InitDesigner(IntT selType, StringT ininame,
													F_ObjHandleT docId, BoolT isBook);
extern VoidT InitNewDesigner(IntT dlgNum);
extern VoidT FreeOldDesigner(IntT dlgNum);
extern VoidT CheckDesChanges(VoidT);

extern VoidT GetDocInfo(F_ObjHandleT docId, BoolT isBook);

extern VoidT ProcDesignerEvent(IntT dlgNum, IntT itemNum);
extern VoidT ProcDesCommand(F_ObjHandleT dlgId, IntT command);
extern VoidT GetCurrDesigner(F_ObjHandleT dlgId, IntT dlgNum);
extern VoidT SetCurrDesigner(F_ObjHandleT dlgId, IntT dlgNum);

extern VoidT ProcCommonDesignerEvent(F_ObjHandleT dlgId, IntT itemNum);
extern VoidT InitCommonDesigner(F_ObjHandleT dlgId);
extern VoidT FreeCommonDesigner(F_ObjHandleT dlgId);


/* in m2rdshb.c */

extern VoidT ProcBasicStyleDesignerEvent(F_ObjHandleT dlgId, IntT itemNum);
extern VoidT SetBasicStyleDesigner(F_ObjHandleT dlgId);
extern VoidT GetBasicStyleDesigner(F_ObjHandleT dlgId);
extern VoidT FreeBasicStyleDesigner(F_ObjHandleT dlgId);


/* in m2rdshs.c */

extern VoidT ProcHelpTopicDesignerEvent(F_ObjHandleT dlgId, IntT itemNum);
extern VoidT SetHelpTopicDesigner(F_ObjHandleT dlgId);
extern VoidT GetHelpTopicDesigner(F_ObjHandleT dlgId);
extern VoidT FreeHelpTopicDesigner(F_ObjHandleT dlgId);


/* in m2rdshh.c */

extern VoidT ProcHelpHotspotDesignerEvent(F_ObjHandleT dlgId, IntT itemNum);
extern VoidT SetHelpHotspotDesigner(F_ObjHandleT dlgId);
extern VoidT GetHelpHotspotDesigner(F_ObjHandleT dlgId);
extern VoidT FreeHelpHotspotDesigner(F_ObjHandleT dlgId);


/* in m2rdsht.c */

extern VoidT ProcHelpTargetDesignerEvent(F_ObjHandleT dlgId, IntT itemNum);
extern VoidT SetHelpTargetDesigner(F_ObjHandleT dlgId);
extern VoidT GetHelpTargetDesigner(F_ObjHandleT dlgId);
extern VoidT FreeHelpTargetDesigner(F_ObjHandleT dlgId);


/* in m2rdshm.c */

extern VoidT ProcHelpMacroDesignerEvent(F_ObjHandleT dlgId, IntT itemNum);
extern VoidT SetHelpMacroDesigner(F_ObjHandleT dlgId);
extern VoidT GetHelpMacroDesigner(F_ObjHandleT dlgId);
extern VoidT FreeHelpMacroDesigner(F_ObjHandleT dlgId);


/* in m2rdtxb.c */

extern VoidT ProcBasicTextDesignerEvent(F_ObjHandleT dlgId, IntT itemNum);
extern VoidT SetBasicTextDesigner(F_ObjHandleT dlgId);
extern VoidT GetBasicTextDesigner(F_ObjHandleT dlgId);
extern VoidT FreeBasicTextDesigner(F_ObjHandleT dlgId);


/* in m2rdtl.c */

extern VoidT ProcTextLayoutDesignerEvent(F_ObjHandleT dlgId, IntT itemNum);
extern VoidT SetTextLayoutDesigner(F_ObjHandleT dlgId);
extern VoidT GetTextLayoutDesigner(F_ObjHandleT dlgId);
extern VoidT FreeTextLayoutDesigner(F_ObjHandleT dlgId);


/* in m2rdgr.c */

extern VoidT ProcBasicGraphicsDesignerEvent(F_ObjHandleT dlgId, IntT itemNum);
extern VoidT SetBasicGraphicsDesigner(F_ObjHandleT dlgId);
extern VoidT GetBasicGraphicsDesigner(F_ObjHandleT dlgId);
extern VoidT FreeBasicGraphicsDesigner(F_ObjHandleT dlgId);


/* in m2rdgi.c */

extern VoidT ProcImportedGraphicsDesignerEvent(F_ObjHandleT dlgId, IntT itemNum);
extern VoidT SetImportedGraphicsDesigner(F_ObjHandleT dlgId);
extern VoidT GetImportedGraphicsDesigner(F_ObjHandleT dlgId);
extern VoidT FreeImportedGraphicsDesigner(F_ObjHandleT dlgId);


/* in m2rdgt.c */

extern VoidT ProcGraphicsTextDesignerEvent(F_ObjHandleT dlgId, IntT itemNum);
extern VoidT SetGraphicsTextDesigner(F_ObjHandleT dlgId);
extern VoidT GetGraphicsTextDesigner(F_ObjHandleT dlgId);
extern VoidT FreeGraphicsTextDesigner(F_ObjHandleT dlgId);


/* in m2rdgra.c */
#ifdef REFF_USED

typedef struct rft {
	StringT name;
	IntT typ;
	StringT form;
	struct rft *next;
} RefFrameT;

extern RefFrameT *CurrRefFrame;

#endif

extern VoidT ProcGraphicFramesDesignerEvent(F_ObjHandleT dlgId, IntT itemNum);
extern VoidT SetGraphicFramesDesigner(F_ObjHandleT dlgId);
extern VoidT GetGraphicFramesDesigner(F_ObjHandleT dlgId);
extern VoidT FreeGraphicFramesDesigner(F_ObjHandleT dlgId);


/* in m2rdgrb.c */

#ifdef BMC_USED

typedef struct bmc {
	StringT name;
	BoolT used;
	IntT num;
	StringT graphic;
	struct bmc *next;
} BitmapCharT;

extern BitmapCharT *CurrBitmapChar;
extern StringT *BmcCharacterStrings;
extern StringT *BmcBitmapStrings;

#define DEF_BITMAP_COUNT 10

extern BitmapCharT *CurrBitmapChar;
extern BitmapCharT *DesBitmapChars;
extern BitmapCharT DefBitmapChar;
extern BitmapCharT FirstBitmapChar;
extern BitmapCharT *LastBitmapChar;
extern BitmapCharT ClipboardBitmapChar;
extern BoolT BitmapCharChanged;

extern StringT *DesBitmapCharList;
extern IntT CurrBitmapCharNum;
extern IntT DesBitmapCharCount;
extern IntT DesBitmapCharAlloc;
extern BoolT BitmapCharListChanged;

extern VoidT InitDesignerBitmapChars(VoidT);
extern VoidT AddDesBitmapChar(StringT fam);
extern BoolT InBitmapCharList(StringT sname);
extern VoidT InitBitmapCharList(VoidT);
extern VoidT SetNewBitmapChar(IntT fontnum);
extern VoidT FreeDesignerBitmapChars(VoidT);
extern IntT FindBitmapCharInList(StringT sname);
extern VoidT ReadBitmapCharsFromIni(StringT ininame, BoolT confirm);
extern VoidT WriteBitmapCharsToIni(StringT ininame, BoolT confirm);

extern BoolT SetBitmapChar(StringT *chp, BoolT to_num);

#endif

extern VoidT ProcBitmapsDesignerEvent(F_ObjHandleT dlgId, IntT itemNum);
extern VoidT SetBitmapsDesigner(F_ObjHandleT dlgId);
extern VoidT GetBitmapsDesigner(F_ObjHandleT dlgId);
extern VoidT FreeBitmapsDesigner(F_ObjHandleT dlgId);


/* in m2rdtbl.c */

extern VoidT ProcTableDesignerEvent(F_ObjHandleT dlgId, IntT itemNum);
extern VoidT SetTableDesigner(F_ObjHandleT dlgId);
extern VoidT GetTableDesigner(F_ObjHandleT dlgId);
extern VoidT FreeTableDesigner(F_ObjHandleT dlgId);


/* in m2rdfnt.c */

extern VoidT ProcFontDesignerEvent(F_ObjHandleT dlgId, IntT itemNum);
extern VoidT SetFontDesigner(F_ObjHandleT dlgId);
extern VoidT GetFontDesigner(F_ObjHandleT dlgId);
extern VoidT FreeFontDesigner(F_ObjHandleT dlgId);


/* in m2rdtxa.c */

extern VoidT ProcAdvancedTextDesignerEvent(F_ObjHandleT dlgId, IntT itemNum);
extern VoidT SetAdvancedTextDesigner(F_ObjHandleT dlgId);
extern VoidT GetAdvancedTextDesigner(F_ObjHandleT dlgId);
extern VoidT FreeAdvancedTextDesigner(F_ObjHandleT dlgId);


/* in m2rdtxh.c */

extern VoidT ProcHelpTextDesignerEvent(F_ObjHandleT dlgId, IntT itemNum);
extern VoidT SetHelpTextDesigner(F_ObjHandleT dlgId);
extern VoidT GetHelpTextDesigner(F_ObjHandleT dlgId);
extern VoidT FreeHelpTextDesigner(F_ObjHandleT dlgId);


/* in m2rdver.c */

extern VoidT ProcVersionDesignerEvent(F_ObjHandleT dlgId, IntT itemNum);
extern VoidT SetVersionDesigner(F_ObjHandleT dlgId);
extern VoidT GetVersionDesigner(F_ObjHandleT dlgId);
extern VoidT FreeVersionDesigner(F_ObjHandleT dlgId);


/* in m2rdcnt.c */

extern VoidT ProcHelpContentsDesignerEvent(F_ObjHandleT dlgId, IntT itemNum);
extern VoidT SetHelpContentsDesigner(F_ObjHandleT dlgId);
extern VoidT GetHelpContentsDesigner(F_ObjHandleT dlgId);
extern VoidT FreeHelpContentsDesigner(F_ObjHandleT dlgId);


/* in m2rdhpj.c */

extern VoidT ProcHelpCompileDesignerEvent(F_ObjHandleT dlgId, IntT itemNum);
extern VoidT SetHelpCompileDesigner(F_ObjHandleT dlgId);
extern VoidT GetHelpCompileDesigner(F_ObjHandleT dlgId);
extern VoidT FreeHelpCompileDesigner(F_ObjHandleT dlgId);


/* in m2rdtpl.c */

extern VoidT ProcTemplateDesignerEvent(F_ObjHandleT dlgId, IntT itemNum);
extern VoidT SetTemplateDesigner(F_ObjHandleT dlgId);
extern VoidT GetTemplateDesigner(F_ObjHandleT dlgId);
extern VoidT FreeTemplateDesigner(F_ObjHandleT dlgId);


/* in m2rdhmk.c */

extern VoidT ProcHelpMarkerDesignerEvent(F_ObjHandleT dlgId, IntT itemNum);
extern VoidT SetHelpMarkerDesigner(F_ObjHandleT dlgId);
extern VoidT GetHelpMarkerDesigner(F_ObjHandleT dlgId);
extern VoidT FreeHelpMarkerDesigner(F_ObjHandleT dlgId);


/* in m2rdmf.c */

typedef struct {
	StringT name;
	BoolT is_TOC;
	BoolT is_IX;
	BoolT compile;

	BoolT fileID_set;
	StringT fileID;

	BoolT word_sect_set;
	StringT word_sect;

	BoolT browse_start_set;
	StringT browse_start;
	BoolT browse_px_set;
	StringT browse_px;

	BoolT dest_hlp_set;
	StringT dest_hlp;

	BoolT bct_first_set;
	StringT bct_first;

	VoidT *next;
} CompFileT;

extern CompFileT *CurrCompFile;
extern CompFileT *DesCompFiles;
extern CompFileT DefCompFile;
extern CompFileT FirstCompFile;
extern CompFileT *LastCompFile;
extern CompFileT ClipboardCompFile;
extern BoolT CompFileChanged;

extern StringT *DesCompFileList;
extern IntT CurrCompFileNum;
extern IntT DesCompFileCount;
extern IntT DesCompFileAlloc;
extern BoolT CompFileListChanged;

extern VoidT InitDesignerCompFiles(VoidT);
extern VoidT GetBookCompFiles(F_ObjHandleT bookId);
extern VoidT GetDocCompFile(F_ObjHandleT docId);
extern VoidT AddCompFile(StringT sname);
extern BoolT InCompFileList(StringT sname);
extern VoidT InitCompFileList(VoidT);
extern VoidT SetNewCompFile(IntT filenum);
extern VoidT FreeDesignerCompFiles(VoidT);
extern CompFileT *FindCompFileInDesCompFiles(StringT sname);
extern IntT FindCompFileInList(StringT sname);
extern VoidT ReadCompFilesFromIni(StringT ininame, BoolT confirm);
extern VoidT ReadCompFilesFromIniSect(StringT ininame, IntT sectnum);
extern VoidT WriteCompFilesToIni(StringT ininame, BoolT confirm);
extern StringT GetHpjName(StringT defname);

extern VoidT ProcMultiFileDesignerEvent(F_ObjHandleT dlgId, IntT itemNum);
extern VoidT SetMultiFileDesigner(F_ObjHandleT dlgId);
extern VoidT GetMultiFileDesigner(F_ObjHandleT dlgId);
extern VoidT FreeMultiFileDesigner(F_ObjHandleT dlgId);


/* in m2rinit.c */

extern BoolT UseInit;
extern BoolT UseDone;
extern BoolT DclOnly;
extern StringT DestSuffix;

extern BoolT UseInitDialog(StringT projdesc);
extern VoidT ProcInitDialogEvent(IntT itemNum);
extern VoidT SetDestSuffix(VoidT);


/* in m2rproc.c */

extern BoolT IsBookFile;
extern F_ObjHandleT CurrBookId;
extern F_ObjHandleT MIFdocId;

extern BoolT UseGrFileID;

extern StringT CurrDir;
extern BoolT DeleteMIF;
extern BoolT UseMIF;
extern BoolT UseFrame8MIF;
extern BoolT UseFrame9MIF;
extern BoolT UseFrame10MIF;
extern BoolT UseFrame11MIF;
extern BoolT UseFrame12MIF;
extern BoolT UseDCL;
extern BoolT MIFExisted;
extern BoolT DCLExisted;
extern BoolT NeedFMFile;

extern IntT ListType;
extern StringT ListTypeList[];
extern IntT RefFileType;
extern StringT RefFileTypeList[];
extern BoolT ListFileUsed;
extern StringT ListFileName;
extern BoolT FormOver;
extern IntT FormLen;
extern UCharT FormStr[5];
extern IntT CntType;
extern StringT CntTypeList[];
extern StringT CntName;
extern StringT CntStartFile;
extern StringT CntBase;
extern StringT CntTitle;

extern StringT FileStartCommand;
extern StringT FileWrapCommand;
extern StringT FileEndCommand;
extern IntT FileCommandWindow;
extern BoolT FileCommandHide;
extern BoolT FileCommandKeep;

extern StringT BookStartCommand;
extern StringT BookWrapCommand;
extern StringT BookEndCommand;
extern IntT BookCommandWindow;
extern BoolT BookCommandHide;
extern BoolT BookCommandKeep;

extern BoolT OldAuto;

extern StringT BookCommandWindowList[];

extern BoolT UseTOC;
extern BoolT UseIX;
extern BoolT UseGenFiles;

extern BoolT UseOHTOC;
extern BoolT UseOHIX;
extern BoolT UseOHFTS;
extern BoolT UseOHREL;
extern StringT OHProjName;

extern BoolT MakeCNT;
extern BoolT MakeHLP;
extern BoolT KeepCompileWindow;
extern StringT CopyGraphicsFrom;
extern StringT CopyCSSFrom;

extern BoolT ApplyTemplateFile;
extern StringT TemplateFileName;
//extern StringT ConfigTemplate;

extern BoolT WriteAllGraphics;
extern BoolT WriteRefGraphics;
extern BoolT WriteMpgGraphics;
extern BoolT UseOrigGrName;
extern BoolT GraphicsFirst;
extern BoolT WriteEqns;
extern BoolT WriteGraphicsOnly;
extern IntT GrFileNumLen;
extern IntT GrFileNumMask;
extern IntT EqDpiValue;
extern IntT GrDpiValue;
extern IntT GrFormat;
extern StringT GrFormList[DES_GRX_FORM_COUNT + 1];
extern IntT GrFormVal[DES_GRX_FORM_COUNT + 1];
extern IntT GrFormMap[DES_GRX_FORM_COUNT + 1];

extern IntT FormType;
//extern BoolT FormOver;
//extern IntT FormLen;
//extern UCharT FormStr[];

extern BoolT AppliedTemplateFlagsSet;
extern IntT AppliedTemplateFlags;
extern BoolT ImportDocProps;
extern BoolT ImportDocPropsSet;
extern BoolT ConvertVars;
extern BoolT SetFrameConditions;
extern BoolT ConditionsSet;
extern BoolT GenUpBook;
extern BoolT CondDes;

extern IntT ExitCode;
extern IntT ExitType;
extern BoolT Quit;

extern VoidT ProcSaveAs(F_ObjHandleT docId, StringT filename);
extern VoidT ProcSaveAsBook(F_ObjHandleT bookId, StringT bookname, StringT destpath);

extern VoidT BuildListFile(F_ObjHandleT bookId, StringT defname);
extern VoidT StartListFile(StringT defname, FILE **cfilep);
extern VoidT RunFileList(void);

extern VoidT BuildCntFile(F_ObjHandleT bookId, StringT defname);
extern VoidT StartCntFile(StringT defname, FILE **cfilep);
extern BoolT CopyBctFile(StringT bctname, FILE *dfile);

extern StringT LastIniSet;
extern StringT LastSpecIniSet;
//extern VoidT GetIniSettings(StringT ininame, StringT fname);

extern BoolT SavingBook;
extern BoolT UsingFile(StringT basename);

extern StringT CurrFileIDPath;
extern StringT NextFileID;
extern VoidT GetFileIDSetting(StringT ininame, StringT basename);
extern VoidT SetFileID(StringT name);
extern BoolT FileIDUsed(StringT id);
extern BoolT IncAlpha(StringT str);

//extern VoidT UpdateIniSettings(StringT ininame);

extern VoidT ConvertOneFile(F_ObjHandleT docId, StringT filename,
 StringT mifname, StringT dclname);
extern StringT SetCommand(StringT filename, StringT mifname,
 StringT dcbname, StringT dclname, IntT pass);

extern BoolT SaveMIFforDCL(F_ObjHandleT docId, StringT filename);
extern VoidT ConvertAllVars(F_ObjHandleT docId);
extern VoidT SetConditions(F_ObjHandleT docId, StringT ininame);
extern VoidT ConvertToText(F_ObjHandleT docId, F_ObjHandleT itemId);
extern BoolT OnMasterRefPage(F_ObjHandleT docId, F_ObjHandleT tfr);

extern StringT FTSCommand;
extern StringT JarCommand;
extern StringT ArchiveCommand;
extern StringT ArchiveStartParams;
extern StringT ArchiveEndParams;
extern StringT ArchiveName;
extern StringT ArchiveVer;
extern StringT ArchiveExt;
extern StringT WrapPath;
extern StringT ShipPath;
extern StringT CSSPath;
extern StringT WrapCopyFiles;
extern StringT GraphCopyFiles;
extern StringT CSSCopyFiles;
extern StringT JavaRootFiles;
extern StringT ZipCommand;
extern StringT ZipParams;
extern StringT EmptyOutputFiles;
extern StringT OHVFiles;
extern BoolT OHProjFileXhtml;
extern BoolT WrapAndShip;
extern BoolT DeleteExistingMIF;
extern BoolT DeleteExistingDCL;
extern IntT EmptyOutputDir;
extern StringT EmptyOutputDirList[];
extern BoolT EmptyWrapPath;
extern BoolT EmptyGraphPath;
extern BoolT EmptyJavaHTMLSubdir;
extern BoolT EmptyJavaGraphSubdir;
extern BoolT LogAuto;
extern BoolT OnlyAuto;
extern BoolT MoveArchive;
extern BoolT EclipseUseManifest;
extern VoidT WrapShipPreProc(BoolT isBook, F_ObjHandleT docId, StringT filename);
extern VoidT WrapShipProc(F_ObjHandleT docId, StringT filename);
extern StringT MakeFileList(StringT filenames);
extern StringT MakeDirPath(StringT base, StringT path, BoolT empty);
extern VoidT CopyCompFiles(StringT filelist, StringT dest);
extern VoidT CopyCompFilesSrc(StringT filelist, StringT dest, StringT src);
extern VoidT DelCompFiles(StringT filelist);
extern VoidT ProcCompFileList(StringT filelist, StringT dest, StringT src, BoolT del);
extern VoidT RemoveStartSplitFiles(F_ObjHandleT docId, StringT filename, StringT htmdir);

/* in m2rvar.c */

extern IntT AskForUserVars;
extern StringT UserVarList[];

extern VoidT ProcUserVars(StringT bkininame);
extern IntT GetVarDef(StringT *response, StringT name,
 StringT content, StringT prompt);
extern VoidT ProcVarDialogEvent(IntT itemNum);
extern StringT GetVarSectKey(StringT *pstr);

extern VoidT RunSystemCommands(BoolT book, BoolT start);

extern VoidT ProcBookCommand(F_ObjHandleT bookId, StringT setting,
 IntT start, BoolT book, BoolT hide, BoolT keep, StringT inifile,
 StringT specini);

extern StringT ProcMacro(StringT mactxt);

extern VoidT RunBookCommand(StringT next);
extern VoidT CheckComment(StringT *np);
//extern StringT RunBookCmdMacro(StringT next);
extern StringT RunBookCmdPercentVar(StringT next);

extern StringT BookCommandBatName;
extern FILE *BookCommandBatFile;
extern VoidT SetUpBookCommandBat(IntT start, BoolT book);
extern VoidT AddToBookCommandBat(VoidT);
extern VoidT RunBookDOSCommand(VoidT);

extern VoidT RunDOSCommand(StringT cmd, BoolT hide, BoolT keep);
extern VoidT GetMacroFile(VoidT);
extern VoidT ClearExtMacros(VoidT);
extern StringT GetMacro(StringT name);


/* in m2rutil.c */

extern IntT StartTime;
extern BoolT DocAlreadyOpen;

extern VoidT SaveDoc(F_ObjHandleT docId, StringT filename, BoolT saveas);
extern VoidT SaveAsPDF(F_ObjHandleT docId, StringT filename);
extern VoidT ImportDocTemplate(F_ObjHandleT docId, F_ObjHandleT templateId);
extern VoidT ImportTemplateFlags(F_ObjHandleT docId, F_ObjHandleT templateId,
                                 IntT flags, BoolT inter);
extern VoidT RevertDoc(F_ObjHandleT *docIdp);
extern F_ObjHandleT IdentifyDocName(StringT docwanted);
extern F_ObjHandleT IdentifyDocLabel(StringT docwanted);
extern F_ObjHandleT IdentifyBookOfDoc(StringT docwanted);
extern F_ObjHandleT OpenDocName(StringT docwanted);

extern VoidT SaveActiveDocBook(VoidT);
extern VoidT RestoreActiveDocBook(VoidT);

extern VoidT FixPathEnvironment(StringT *path);
extern BoolT PathExists(StringT pathname);
extern BoolT FileExists(StringT filename);
extern VoidT ChangeDir(StringT dir);
extern VoidT MakeDir(StringT dir);
extern VoidT RemoveDir(StringT dir);
extern VoidT DeleteDirContents(StringT dir);
extern StringT NewFileExt(StringT filename, StringT newext);
extern StringT AddFileExt(StringT filename, StringT newext);
extern StringT BaseFileName(StringT filename);
extern IntT SetPathName(StringT *pathp, StringT filename, BoolT endsep);
extern StringT MakeFileName(StringT pathname, StringT filename);
extern VoidT SetCurrPath(StringT *fnp);
extern StringT AddSlash(StringT newpath);
extern StringT Unquote(StringT newpath);
extern VoidT RemoveSlash(StringT path);
extern BoolT IniExists(StringT pathname, StringT filename);

extern VoidT ShowStatus(F_ObjHandleT docId, StringT filename, StringT status);
extern VoidT ClearStatus(F_ObjHandleT docId);
extern VoidT ReportResults(StringT filename);

extern VoidT RunWinHlp(IntT idhval);

extern VoidT RunCommand(StringT command, StringT dir,
												 BoolT synch, BoolT hide, BoolT sized);


/* in m2rgrx.c */
extern StringT CurrGrxFile;
extern StringT RefGrName;
extern StringT RefGrID;
extern StringT RefGrDpi;
extern StringT GrGenSect;
extern StringT GrDpiSect;
extern StringT GrUseSect;

extern IntT ExportGraphics(F_ObjHandleT docId, StringT filebase,
													 IntT grFormat, IntT grDPI, IntT grType);
extern BoolT IsNeededGraphic(F_ObjHandleT docId, F_ObjHandleT graph,
                           UIntT otype);
#if 0
extern BoolT IsHiddenFrame(F_ObjHandleT docId, F_ObjHandleT graph);
extern BoolT IsRefFrame(F_ObjHandleT docId, F_ObjHandleT graph);
extern VoidT ScanAFrames(F_ObjHandleT docId);
extern VoidT ProcInset(F_ObjHandleT docId, F_ObjHandleT inset, BoolT Single);
#endif

extern StringT GetFormatHint(StringT oldFormat, StringT oldVendor);
extern VoidT ExportGraphic(F_ObjHandleT docId, F_ObjHandleT graphic,
										 StringT fileName, StringT format, IntT grDPI);


/* in m2rinid.c */
extern StringT FDKIniSectList;
extern struct secinfo FDKIniSects[];
extern struct setinfo ISecFDK[];
extern VoidT GraphicNameDigitsSet(VoidT);
extern VoidT GrDpiValueSet(VoidT);
extern VoidT EqDpiValueSet(VoidT);
extern VoidT ApplyTemplateFileSet(VoidT);
extern VoidT TemplateFileNameSet(VoidT);
extern VoidT AppliedTemplFlagsSet(VoidT);
extern VoidT ImportDocPrSet(VoidT);

extern BoolT MakeFts;
extern BoolT MakeJar;
extern BoolT MakeArchive;
extern StringT ArchiveFrom;
extern StringT CopyBeforeFiles;
extern StringT CopyBeforeFrom;
extern StringT CopyAfterFiles;
extern StringT CopyAfterFrom;

extern struct setinfo ISecAutomation[];
extern struct setinfo ISecFileCmd[];
extern struct setinfo ISecBookCmd[];
extern struct setinfo ISecGraph[];
extern struct setinfo ISecCss[];

#define logerr   1
#define logwarn  2
#define logquery 3
#define loginfo  4
#define logdebug 5
#define logmax   6
extern BoolT UseLog;
extern BoolT ShowLog;
extern StringT LogFileName;
extern StringT HistoryFileName;
extern StringT EditorFileName;
extern UCharT LogLevels[logmax];
extern struct setinfo ILogOptions[];

extern struct setinfo ISecHelp[];
extern struct setinfo ISecWinHelp[];
extern struct setinfo ISecHelpCont[];
extern struct setinfo ISecHtmlHelp[];
extern struct setinfo ISecJavaHelp[];
extern struct setinfo ISecOracleHelp[];
extern struct setinfo ISecOmniHelp[];
extern struct setinfo ISecEclipseHelp[];
extern StringT IniNameList[PROJ_TYPES];
extern StringT CurrIniName;
extern StringT FullFormList[PROJ_TYPES];
extern StringT CurrFullForm;
extern StringT Setup;
extern BoolT HasFDK;
extern BoolT HasSetup;
extern BoolT RenameFDK;
extern StringT FrameBook;
extern StringT FrameChapter;
extern StringT ProjectName;

extern BoolT ProcessIni(StringT path, StringT base,
                         BoolT atstart, BoolT isbook);
extern VoidT UpdateIni(VoidT);
extern VoidT CloseIni(VoidT);

extern VoidT M2GIniGetSetting(StringT fpath, StringT sect, StringT key, StringT *vp);
extern VoidT M2GIniPutSetting(StringT fpath, StringT sect, StringT key, StringT val);
extern VoidT M2GIniPRenSect(StringT fpath, StringT sect, StringT nsect);
extern VoidT M2GIniPutBool(StringT fpath, StringT sect, StringT key, BoolT val);
extern VoidT M2GIniPutInt(StringT fpath, StringT sect, StringT key, IntT val);
extern VoidT M2GIniPUpdate(StringT fpath);
extern VoidT M2GIniPClose(StringT fpath);

/* in m2rini.c */
extern VoidT CallM2GFrame(StringT str);

extern VoidT M2GOpenProject(StringT proj);
extern VoidT M2GCloseProject(BoolT final);
extern VoidT M2GOpenLog(StringT fname);
extern VoidT M2GCloseLog(BoolT final);
extern VoidT M2GLogEvent(UCharT lt, UCharT sev, StringT d1,
                StringT d2, StringT d3, StringT d4, StringT d5);

extern VoidT M2GGetOpenFile(StringT title, StringT path, StringT *pname);
extern VoidT M2GGetOpenDir(StringT title, StringT path, StringT *pname);
extern VoidT M2GGetMyDocDir(StringT pname);
extern VoidT M2GWaitFileDone(StringT pname);

extern VoidT M2GSetIni(StringT iname, StringT fname);
extern VoidT M2GIniSects(VoidT);
extern VoidT M2GIniSectList(VoidT);
extern VoidT M2GIniGroup(StringT group);
extern VoidT M2GIniGetString(StringT sect, StringT key, StringT *val);
extern VoidT SetIniBuf(StringT ininame, StringT section, StringT key);
//extern VoidT M2GIniGetMacroString(StringT sect, StringT key, StringT *val);
//extern VoidT SetIniBufMacro(StringT section, StringT key);
extern VoidT M2GIniGetSect(StringT sect, StringT *val);
extern VoidT SetSectBuf(StringT ininame, StringT section);
extern VoidT SetSectionBuf(StringT ininame, StringT section, StringT *bptr);

#define MAX_PROFILE 2048

extern BoolT IniBufEmpty(VoidT);
extern StringT GetIniBuf(VoidT);
extern VoidT TrueFalseIni(StringT ininame, StringT section, StringT key,
                          BoolT *varp);
extern VoidT GetIniInt(StringT ininame, StringT section, StringT key,
                       IntT *varp);
extern VoidT GetIniMinMax(StringT ininame, StringT section, StringT key,
                          IntT vmin, IntT vmax, IntT *varp);
extern VoidT GetIniString(StringT ininame, StringT section, StringT key,
                          StringT *str);
extern VoidT GetRawIniString(StringT ininame, StringT section, StringT key,
                          StringT *str);
//extern VoidT GetRawMacroString(StringT section, StringT key, StringT *str);
extern VoidT GetIniStrInt(StringT ininame, StringT section, IntT kval,
                          StringT *str);
extern VoidT GetIniPath(StringT ininame, StringT section, StringT key,
                        StringT *path);
extern VoidT GetIniMatch(StringT ininame, StringT section, StringT key,
                         StringT *list, IntT *varp);
extern VoidT GetIniUC(StringT ininame, StringT section, StringT key,
                      UCharT *pch);

extern VoidT GetIniSect(StringT ininame, StringT section);
extern StringT GetIniSectKey(VoidT);
extern StringT GetRawIniSectKey(VoidT);
extern IntT GetIniSectCount(VoidT);
extern StringT GetFullIniSect(StringT inifile, StringT name);

extern BoolT CheckLinks;
extern StringT CheckLinkLog;
extern BoolT LinkLogAlways;
extern VoidT ProcRefLinkErrs(F_ObjHandleT bookId);

extern VoidT SetIniStr(StringT ininame, StringT section, StringT key,
											 StringT val);
extern VoidT SetRawIniStr(StringT ininame, StringT section, StringT key,
											 StringT val);
extern VoidT SetIniInt(StringT ininame, StringT section, StringT key, 
											 IntT val);
extern VoidT SetIniTF(StringT ininame, StringT section, StringT key,
											BoolT val);
extern VoidT ClearIniSect(StringT ininame, StringT section);

extern VoidT EditIniFile(StringT ininame);

extern StringT Fm2a(StringT fmname);
extern StringT A2fm(StringT aname);


/* in m2rwrt.c */

typedef struct bc {
	StringT bcname;
	F_ObjHandleT bcId;
	IntT bfid;
	StringT px;
	struct bc *bcnext;
} BookComp;

typedef struct ps {
	StringT psname;
	F_ObjHandleT psId;
	StringT slev;
	IntT place;
	MetricT indent;
	MetricT psize;
	IntT weight;
	struct ps *psnext;
} ParaStyle;

extern StringT HelpFileName;
extern StringT CntFileName;
extern BoolT UseFmTOC;
extern BoolT UseFmIX;
extern BoolT UseFmGenFiles;

extern StringT ProjectDescription;

extern StringT FileSuffix;
extern StringT WordSuffix;
extern StringT WordPerfectSuffix;
extern StringT HTMLSuffix;
extern StringT XMLSuffix;
extern StringT MIFSuffix;
extern StringT MIFBookSuffix;
extern StringT DCLSuffix;

extern BoolT UseTemplate;
extern StringT TemplateName;
extern IntT TemplateFlags;

extern StringT GraphPath;

extern BoolT MakeCombCnt;
extern StringT HPJName;
extern StringT OHVName;
extern StringT DefaultChmFile;
extern StringT Compiler;

extern StringT HelpContentsTopic;
extern StringT HelpTitle;
extern StringT HelpCopyright;
extern BoolT HelpCopyDate;


extern BoolT UseWordTemplate;
extern StringT WordTemplateName;
extern BoolT WordTemplateAuto;

extern UCharT Sideheads;
extern UCharT Headfeet;
extern BoolT FramedHF;


extern BoolT UseCSS;
extern BoolT CreateCSS;
extern StringT CssFileName;

extern UCharT TableSizing;
extern StringT TableDPI;
extern StringT TblBorder;
extern StringT TblSpacing;
extern StringT TblPadding;

extern VoidT WriteStartingIni(StringT inipath, StringT filename,
													 BoolT book, UCharT form);
extern StringT LocalConfigPath;
extern StringT WinHelpDocName;
extern StringT WordDocName;
extern StringT HTMLDocName;
extern VoidT ReadTopConfigIni(StringT cfgini);
extern VoidT WriteDocFileInfo(StringT destpath, BoolT book,
	                     BoolT winhelp, BoolT msword);

extern VoidT SetCSSIniOptions(VoidT);
extern VoidT SetAutoIniOptions(StringT filename);

extern VoidT WriteBasicIni(StringT inipath, StringT filename,
													 BoolT book, UCharT form);
extern VoidT UpdateBasicIni(StringT ininame, StringT basename,
                           BoolT book, UCharT form);

extern VoidT GetChapters(F_ObjHandleT bookId);
extern IntT GetMetaNameMin(VoidT);
extern StringT GetBookTOC(F_ObjHandleT bookId);
extern VoidT FreeChapters(VoidT);

extern VoidT GetStyles(F_ObjHandleT fileId, BoolT inTOC);
extern BoolT PgfUsedInDoc(F_ObjHandleT docId, StringT style);
extern VoidT FreeStyles(VoidT);
extern BoolT PScmp(ParaStyle *a, ParaStyle *b);
extern ParaStyle *PSmerge(ParaStyle *a, ParaStyle *b);
extern ParaStyle *PSmergesort(ParaStyle *c);

extern VoidT WriteHelpProj(StringT inipath, StringT filename,
													 BoolT book, UCharT form, StringT ininame);

extern VoidT WriteHPJFile(StringT inipath, StringT filename,
												  BoolT book, UCharT form, StringT ininame);
extern VoidT SetCntVars(StringT basename, StringT ininame);

extern BoolT WriteHelpSetFile;
extern BoolT WriteHelpProjectFile;
extern BoolT HSPathNames;
extern StringT HTMLSubdir;
extern StringT GraphSubdir;
extern StringT DefaultTopicFile;
extern StringT CshMapFile;
extern StringT MapFilePrefix;
extern BoolT GenTOC;
extern BoolT GenIX;
extern BoolT UseFTS;
extern BoolT UseAlias;


extern VoidT WriteHSFile(StringT inipath, StringT filename,
												  BoolT book, UCharT form);
extern VoidT WriteHHPFile(StringT inipath, StringT filename,
												  BoolT book, UCharT form);
extern VoidT WriteHTMLText(FILE *wf, StringT txt);


/* in m2rstr.c */

extern StringT SaveFileStr;
extern StringT SetupFileStr;
extern StringT ConvDesStr;
extern StringT WashFileStr;
extern StringT WashAllFileStr;

extern StringT DestExt;
extern StringT RtfExt;
extern StringT DocExt;
extern StringT HtmExt;
extern StringT HtmlExt;
extern StringT XmlExt;
extern StringT DITAExt;
extern StringT DocBookExt;
extern StringT PrjExt;
extern StringT IniExt;
extern StringT MifExt;
extern StringT GrxExt;
extern StringT DclExt;
extern StringT PdfExt;

extern StringT ChooseDestFileStr;
extern StringT ExportFileCancelled;
extern StringT ChooseDestDirStr;

extern StringT ChooseTemplateFileStr;
extern StringT ChooseWordTemplateFileStr;
extern StringT ChooseCSSFileStr;

extern StringT SetupCompleted;

extern StringT ProjStartStr;
extern StringT IniStartStr;

extern StringT ApplyChanges;
extern StringT DiscardChanges;
extern StringT MacroHotspot;
extern StringT TopicEntryMacro;
extern StringT ParaDesLab;
extern StringT CharDesLab;

extern StringT StdIni;
extern StringT StdRtfIni;
extern StringT StdHtmIni;
extern StringT StdPdfIni;
extern StringT StdMulIni;
extern StringT IniMissing;
extern StringT DoBookSetup;
extern StringT ReplacingOldIni;

extern StringT FileTypeErr;
extern StringT FilePathErr;
extern StringT NoDialogUse;

extern StringT InitDialogID;
extern StringT InitDialogErr;

extern StringT BookProcessDone;
extern StringT FileProcessDone;
extern StringT M2GProjectDone;

extern StringT ReportOK1;
extern StringT ReportOK2;

extern StringT ReportFail1;
extern StringT ReportFailCode;
extern StringT ReportFailTypeCode;

extern StringT ReportMIFFail;
extern StringT ReportFailWrite;
extern StringT ReportFailType1;
extern StringT ReportFailType2;
extern StringT ReportFailType3;
extern StringT ReportFailType4;
extern StringT ReportFailType5;
extern StringT ReportFailType6;
extern StringT ReportFailType7;
extern StringT ReportFailTypeX;

extern StringT ReportFailCode1;
extern StringT ReportFailCode2;
extern StringT ReportFailCode3;
extern StringT ReportFailCode4;
extern StringT ReportFailCode5;
extern StringT ReportFailCode6;
extern StringT ReportFailCode7;
extern StringT ReportFailCode8;
extern StringT ReportFailCode9;
extern StringT ReportFailCode10;
extern StringT ReportFailCode11;
extern StringT ReportFailCode12;

extern StringT UserCancelled;
extern StringT TemplateFailed;
extern StringT WriteFailed;
extern StringT EqnFailure;
extern StringT EqnFailMsg;
extern StringT CntFailure;
extern StringT MiscFailure;

extern StringT StatOpening;
extern StringT StatTemplate;
extern StringT StatConvVars;
extern StringT StatSetConds;
extern StringT StatEqWriting;
extern StringT StatGrWriting;

extern StringT StatConverting;

extern StringT StatSavingMIF;
extern StringT StatClean;
extern StringT StatList;
extern StringT StatDITA;
extern StringT StatDocBook;
extern StringT StatCheck;
extern StringT StatWrap;
extern StringT StatCompile;
extern StringT StatJHFTS;
extern StringT StatOHJFTS;
extern StringT StatJar;
extern StringT StatArchive;
extern StringT StatShip;

extern VoidT SetStrings(IntT type);


#endif


/* end of m2rbook.h */
