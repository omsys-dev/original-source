/* m2rdes.c is the FDK designer for mif2rtf FM 5.5 book export 
 * Last edited on 12/24/98 by Jeremy H. Griffith.
 * Copyright 1998 Omni Systems, Inc.  All Rights Reserved.
 */


#include "m2rbook.h"


/* Properties dropdown changes depending on target and style:

			All targets:
	*fdk	FDK settings for target, build, etc.

			For WinHelp:
				Styles
m2rdshb		Basic
						Name: Remap, Replace, or Retain
						Rads for major types below, plus key-only and not used
					Help
m2rdshs			Topic
m2rdshh			Hotspot
m2rdsht			Target
m2rdshm			Macro
m2rdfnt	Fonts
					Basic
						Remapped name
						Metrics
m2rdtbl	Tables
					Basic
					Ruling 
				Graphics
m2rdgr		Basic
						File paths, names
						Picture format
m2rdgi		Bitmaps  
						DPI, Flip, wrapping
m2rdgt			Text
						Scaling, shifting
	*ref	Reference frame usage
m2rdtl	Layout  
m2rdtx		Basic
						Sideheads
						Run-ins
						Footnotes
m2rdtpl	Templates
					Frame
m2rdhpj	Build .hlp
	*hb		Basic Help
					Contents
					Browse
					Topic title props

			For Word:
m2rdshb	Styles
					Basic
						Remapped name
m2rdfnt	Fonts
					Basic
m2rdtbl	Tables
					Basic
m2rdgr	Graphics
m2rdtl	Layout
					Columns
					Text frames
					Headers/Footers
m2rdtx	Footnotes
	*			Templates
					Frame
					Word

	*		For HTML:
				Styles
					Basic
						Associated HTML type (H1, P, etc.)
						Props for [HTMLStyles]
						Splitting and title usage
					Macros
				CSS
				Tables
					Basic
				Templates
					Frame
*/


/* essential structures */

// set of properties for output type: WinHelp, Word, HTML, etc.
typedef struct {
	IntT propCount;  // count of designers in set
	IntT desPanels[DLG_DES_COUNT];  // array of indexes to DesPanels of panels in set
	StringT *propList;  // made up from propNames of ComDesTs at runtime
} PropSetT;


// 0=Word, 1=WinHelp, 2=HTML, etc.
PropSetT DesPropSets[] = {
	{ 15, { 1, 6, 13, 12, 7, 11, 8, 9, 22, 21, 10, 18, 15, 19, 20 },             NULL }, // Word
	{ 22, { 1, 2, 3, 4, 5, 19, 16, 17, 6, 13, 14, 12, 7, 11, 8, 9, 22, 21, 10, 18, 15, 20 }, NULL }, // WinHelp
	{  1, { 1 },                                     NULL }  // dummy for HTML 
};
PropSetT *CurrPropSet = &DesPropSets[0];

// maps ProjSelType 1..6 to prop sets 0..2
// 1=Word, 2=Word8, 3=WPerf, 4=WinHelp3, 5=WinHelp4, 6=HTML,  etc.
IntT DesSelTypes[] = { 0, 0, 0, 0, 1, 1, 2 };

// index to desPanels for next designer
IntT NextDesPropNum = 1;


// set of designer dialog panels
typedef struct {
	StringT res;      // resource name
	StringT propName; // name for this dialog in properties
	IntT nameType;    // index for its NameListT in NameLists
	F_ObjHandleT desId;
#ifdef WIN32
	HWND wnd;
#endif
} PanelT;

// index is designer number
PanelT DesPanels[DLG_DES_COUNT + 1] = {
 { NULL,      NULL,              0, 0, 0 }, // desNum 0 is reserved
 { "m2rdshb", "Basic Style",     1, 0, 0 },
 { "m2rdshs", "Help Topic",      1, 0, 0 },
 { "m2rdshh", "Help Hotspot",    1, 0, 0 },
 { "m2rdsht", "Help Target",     1, 0, 0 },
 { "m2rdshm", "Help Macro",      1, 0, 0 }, // 5
 { "m2rdtxb", "Basic Text",      0, 0, 0 },
 { "m2rdtl",  "Text Layout",     2, 0, 0 },
 { "m2rdgr",  "Basic Graphics",  3, 0, 0 },
 { "m2rdgi",  "Import Graphics", 4, 0, 0 },
 { "m2rdgt",  "Graphic Text",    5, 0, 0 }, // 10
 { "m2rdtbl", "Tables",          0, 0, 0 },
 { "m2rdfnt", "Fonts",           6, 0, 0 },
 { "m2rdtxa", "Advanced Text",   0, 0, 0 },
 { "m2rdtxh", "Help Text",       0, 0, 0 },
 { "m2rdver", "Version",         0, 0, 0 }, // 15
 { "m2rdcnt", "Help Contents",   0, 0, 0 },
 { "m2rdhpj", "Help Compiler",   0, 0, 0 },
 { "m2rdtpl", "Template",        0, 0, 0 },
 { "m2rdhmk", "Help Marker",     0, 0, 0 },
 { "m2rdmf",  "Multifile",       7, 0, 0 }, // 20
#ifdef REFF_USED
 { "m2rdgra", "Graphic Frames",  8, 0, 0 },
#else
 { "m2rdgra", "Graphic Frames",  3, 0, 0 },
#endif
#ifdef BMC_USED
 { "m2rdgrb", "Bitmaps",         9, 0, 0 }
#else
 { "m2rdgrb", "Bitmaps",         3, 0, 0 }
#endif
};

PanelT *CurrPanel = &DesPanels[1];


// set of lists used by various des dialogs

// single-item list used by basic text dialog
StringT TextDesNames[2] = {
 "Styles", "All Styles"
};

// single-item list used by basic graphics dialog
StringT GraphDesNames[2] = {
 "Formats", "All Formats"
};

// single-item list used by graphic text dialog
StringT GrTextDesNames[2] = {
 "Formats", "FM Drawing"
};

// 0=all styles, 1=style, 2=flow, 3=all formats, 4=graphic, 5=font, 6=tbl, 7=refframe
NameListT DesNameLists[DES_NAMES_COUNT] = {
 { "Styles",  "Applies to:",     "To Project",         TextDesNames,   1, 1 },
 { "Styles",  "Style Tag:",      "To Selected Style",  NULL,           0, 1 },
 { "Flows",   "Flow Tag:",       "To Selected Flow",   NULL,           0, 1 },
 { "Formats", "Applies to:",     "To Project",         GraphDesNames,  1, 1 },
 { "Formats", "Graphic Format:", "To Selected Format", ImportDesNames, DES_GRI_FORM_COUNT, 1 },
 { "Formats", "Graphic Format:", "To Project",         GrTextDesNames, 1, 1 },
 { "Fonts",   "Font Tag:",       "To Selected Font",   NULL,           0, 2 },
 { "Files",   "Base Name:",      "To Selected File",   NULL,           0, 1 },
 { "Frames",  "Ref Frame Name:", "To Selected Frame",  NULL,           0, 1 },
 { "Bitmaps", "Bitmap Char:",    "To Selected Char",   NULL,           0, 1 }
};

NameListT *CurrNameList = &DesNameLists[0];
IntT CurrNameListNum = 0;


/* globals */

StringT CurrIni = NULL;
F_ObjHandleT CurrDocId = 0;
IntT CurrSelType = 0;
IntT CurrPropSetNum = 0;

BoolT DesignerInitializing = True;
BoolT DesignerChanged = False;

#ifdef WIN32
HWND FrameMakerWnd = 0;
POINT DesPt;
HWND hCurrDes = 0;
#endif

IntT CurrDesigner = 0;
IntT NextDesigner = 0;

IntT DesX = 0;
IntT DesY = 0;

F_StringsT NameStrings;
F_StringsT PropStrings;

#define DLG_DES_COMMON_COUNT  17


/* commands */

#define DLG_DES_CMD_EDIT    1
#define DLG_DES_CMD_RELOAD  2
#define DLG_DES_CMD_COPY    3
#define DLG_DES_CMD_PASTE   4
#define DLG_DES_CMD_RESET   5
#define DLG_DES_CMD_CLEAR   6
#define DLG_DES_CMD_PARA    7
#define DLG_DES_CMD_CHAR    8
#define DLG_DES_CMD_BOTH    9

#define DLG_DES_CMD_COUNT   9

StringT CmdNameList[DLG_DES_CMD_COUNT + 1] = {
	"Commands",
	"Edit .ini file",
	"Reload from .ini",
	"Copy properties",
	"Paste properties",
	"Reset properties",
	"Clear properties",
	"Show only para",
	"Show only char",
	"Show para and char"
};
F_StringsT CmdStrings;
IntT LastCmd = 1;



/* Common designer functions */

VoidT InitDesigner(IntT selType, StringT ininame, F_ObjHandleT docId, BoolT isBook)
{
	F_ObjHandleT dlgId = 0;
	BoolT newStyles = False;
	BoolT newSet = False;
	IntT dlgNum = 1;

#ifdef WIN32
	HWND first = 0;
#endif

	if (CurrDesigner) {
		if ((docId != CurrDocId)
		 || F_StrICmp(ininame, CurrIni)) {
			NextDesigner = CurrDesigner;
			FreeDesignerStyles();
			FreeDesignerFlows();
			FreeDesignerFonts();
			FreeDesignerCompFiles();
			F_StrFree(CurrIni);
			CurrIni = NULL;
			newStyles = True;
		}
		if (DesSelTypes[selType] != CurrPropSetNum) {
			NextDesigner = DesPropSets[DesSelTypes[selType]].desPanels[0];
			NextDesPropNum = 1;
			newSet = True;
		}
	}

	CurrDocId = docId;

	CurrSelType = selType;
	CurrPropSetNum = DesSelTypes[selType];
	CurrPropSet = &DesPropSets[CurrPropSetNum];
	switch (CurrPropSetNum) {
		case DES_PROPSET_WORD:
			DefDesText = WDefDesText;
			SubOptions = WordOptions;
			DefDesGraph = WDefDesGraph;
			break;
		case DES_PROPSET_HELP:
			DefDesText = HDefDesText;
			SubOptions = HelpOptions;
			DefDesGraph = HDefDesGraph;
			break;
		case DES_PROPSET_HTML: // dummy entries
			DefDesText = WDefDesText;
			SubOptions = WordOptions;
			DefDesGraph = WDefDesGraph;
			break;
		default:
			break;
	}

	if (!CurrIni)
		CurrIni = F_StrCopyString(ininame);

	if (!DesStyles) {   /* change of document or .ini file */
		GetDocInfo(docId, isBook);
		InitDesignerStyles();
		InitDesignerFlows();
		InitDesignerFonts();
		InitDesignerCompFiles();
		ReadTextPropsFromIni(CurrIni, True);
		ReadGraphPropsFromIni(CurrIni, True);
	}
	else if (newSet) {  /* change of properties, as in Word to Help */
		ReadTextPropsFromIni(CurrIni, True);
		ReadGraphPropsFromIni(CurrIni, True);
	}

#ifdef WIN32
	if (!CurrDesigner) {
		for (dlgNum = 1; dlgNum <= DLG_DES_COUNT; dlgNum++) {
			dlgId = F_ApiOpenResource(FO_DialogResource, DesPanels[dlgNum].res);
			if (!dlgId)
				break;
			DesPanels[dlgNum].desId = dlgId;
		}
		for (dlgNum = 1; dlgNum <= DLG_DES_COUNT; dlgNum++) {
			dlgId = DesPanels[dlgNum].desId;
			if (!dlgId)
				break;
			F_ApiModelessDialog(dlgNum + DLG_DESIGNER_BASE, dlgId);
			DesPt.x = F_ApiGetInt(FV_SessionId, dlgId, FP_ScreenX) + 10;
			DesPt.y = F_ApiGetInt(FV_SessionId, dlgId, FP_ScreenY) + 10;
			hCurrDes = WindowFromPoint(DesPt);
			DesPanels[dlgNum].wnd = hCurrDes;
			if (dlgNum == 1) {
				first = hCurrDes;
				LockWindowUpdate(first);
				DesX = DesPt.x - 10;
				DesY = DesPt.y - 10;
			}
			else {
				ShowWindow(hCurrDes, SW_HIDE);
				SetActiveWindow(first);
			}
		}
		LockWindowUpdate(NULL);
		NextDesigner = CurrPropSet->desPanels[0];
		NextDesPropNum = 1;
		DesPt.x = F_ApiGetInt(0, FV_SessionId, FP_ScreenX) + 30;
		DesPt.y = F_ApiGetInt(0, FV_SessionId, FP_ScreenY) + 10;
		FrameMakerWnd = WindowFromPoint(DesPt);
		InitNewDesigner(NextDesigner);
	}
	else if (newStyles || newSet)
		InitNewDesigner(NextDesigner);
	else {
		dlgId = DesPanels[CurrDesigner].desId;
		F_ApiSetInt(FV_SessionId, dlgId, FP_ScreenX, DesX);
		F_ApiSetInt(FV_SessionId, dlgId, FP_ScreenY, DesY);
		hCurrDes = DesPanels[CurrDesigner].wnd;
		ShowWindow(hCurrDes, SW_SHOW);
	}

#else
	if (CurrDesigner)
		dlgNum = CurrDesigner;

	if (NextDesigner == 0)
		NextDesigner = dlgNum;

	DesX = F_ApiGetInt(0, FV_SessionId, FP_ScreenX) + 50;
	DesY = F_ApiGetInt(0, FV_SessionId, FP_ScreenY) + 50;

	/* create basic designer in response to menu pick */
	if (CurrDesigner != NextDesigner)
		InitNewDesigner(NextDesigner);
	else if (newStyles)
		InitNewDesigner(CurrDesigner);
	else {	/* start up as modeless dialog */
		dlgId = DesPanels[dlgNum].desId;
		F_ApiSetInt(FV_SessionId, dlgId, FP_ScreenX, DesX);
		F_ApiSetInt(FV_SessionId, dlgId, FP_ScreenY, DesY);
		F_ApiModelessDialog(dlgNum + DLG_DESIGNER_BASE, dlgId);
	}
#endif
}


VoidT InitNewDesigner(IntT dlgNum)
{
	F_ObjHandleT dlgId;
	IntT i = 0;

	if (CurrDesigner)
		CheckDesChanges();

#ifdef WIN32
	if ((dlgId = DesPanels[dlgNum].desId) == 0)
		return;

#else
	/* select resource template and init it, store ID */
	if ((dlgId = DesPanels[dlgNum].desId) == 0) {
		dlgId = F_ApiOpenResource(FO_DialogResource, DesPanels[dlgNum].Res);
		if (dlgId == 0)
			return;
		DesPanels[dlgNum].desId = dlgId;
	}
#endif

	if (StyleListChanged)
		InitStyleList();

	/* init common section for style-setting dialogs */
	CurrNameListNum = DesPanels[dlgNum].nameType;
	CurrNameList = &DesNameLists[CurrNameListNum];

	if (CurrPropSet->propList == NULL) {
		CurrPropSet->propList = (StringT *)
		 F_Alloc((CurrPropSet->propCount + 1) * sizeof(StringT), NO_DSE);
		CurrPropSet->propList[0] = "Properties";
		for (i = 1; i <= CurrPropSet->propCount; i++)
			CurrPropSet->propList[i] = DesPanels[CurrPropSet->desPanels[i - 1]].propName;
	}

	InitCommonDesigner(dlgId);

	/* init specific section */
	SetCurrDesigner(dlgId, dlgNum);

	/* position over current designer */
#ifdef WIN32
	if (CurrDesigner != dlgNum) {
		F_ApiSetInt(FV_SessionId, dlgId, FP_ScreenX, DesX);
		F_ApiSetInt(FV_SessionId, dlgId, FP_ScreenY, DesY);
		hCurrDes = DesPanels[dlgNum].wnd;
		if (!DesignerInitializing) {
			ShowWindow(hCurrDes, SW_SHOW);
			if (CurrDesigner)
				ShowWindow(DesPanels[CurrDesigner].wnd, SW_HIDE);
		}
		SetActiveWindow(hCurrDes);
		CurrDesigner = dlgNum;
		DesignerInitializing = False;
	}

#else
	F_ApiSetInt(0, FV_SessionId, FP_Displaying, False);
	if (CurrDesigner != NextDesigner)
		FreeOldDesigner(CurrDesigner);
	F_ApiModelessDialog(dlgNum + DLG_DESIGNER_BASE, dlgId);

	F_ApiSetInt(FV_SessionId, dlgId, FP_ScreenX, DesX);
	F_ApiSetInt(FV_SessionId, dlgId, FP_ScreenY, DesY);
	F_ApiSetInt(0, FV_SessionId, FP_Displaying, True);
	CurrDesigner = NextDesigner;
#endif
}


VoidT FreeOldDesigner(IntT dlgNum)
{
#ifndef WIN32
	F_ObjHandleT dlgId = 0;

	/* remove previous one from under us */
	dlgId = DesPanels[dlgNum].desId;

	switch (dlgNum) {
		case DLG_DES_HS_BASIC:
			FreeBasicStyleDesigner(dlgId);
			break;
		case DLG_DES_HS_TOPIC:
			FreeHelpTopicDesigner(dlgId);
			break;
		case DLG_DES_HS_HOTSP:
			FreeHelpHotspotDesigner(dlgId);
			break;
		case DLG_DES_HS_TARGET:
			FreeHelpTargetDesigner(dlgId);
			break;
		case DLG_DES_HS_MACRO:
			FreeHelpMacroDesigner(dlgId);
			break;
		case DLG_DES_TX_BASIC:
			FreeBasicTextDesigner(dlgId);
			break;
		case DLG_DES_TX_LAYOUT:
			FreeTextLayoutDesigner(dlgId);
			break;
		case DLG_DES_GR_BASIC:
			FreeBasicGraphicsDesigner(dlgId);
			break;
		case DLG_DES_GR_IMPORT:
			FreeImportedGraphicsDesigner(dlgId);
			break;
		case DLG_DES_GR_TEXT:
			FreeGraphicsTextDesigner(dlgId);
			break;
		case DLG_DES_TX_TABLE:
			FreeTableDesigner(dlgId);
			break;
		case DLG_DES_TX_FONTS:
			FreeFontDesigner(dlgId);
			break;
		case DLG_DES_TX_ADV:
			FreeAdvancedTextDesigner(dlgId);
			break;
		case DLG_DES_TX_HELP:
			FreeHelpTextDesigner(dlgId);
			break;
		case DLG_DES_TX_VER:
			FreeVersionDesigner(dlgId);
			break;
		case DLG_DES_TX_CNT:
			FreeHelpContentsDesigner(dlgId);
			break;
		case DLG_DES_TX_HPJ:
			FreeHelpCompileDesigner(dlgId);
			break;
		case DLG_DES_TX_TPL:
			FreeTemplateDesigner(dlgId);
			break;
		case DLG_DES_TX_HMK:
			FreeHelpMarkerDesigner(dlgId);
			break;
		case DLG_DES_TX_MULT:
			FreeMultiFileDesigner(dlgId);
			break;
		case DLG_DES_GR_FRM:
			FreeGraphicFramesDesigner(dlgId);
			break;
		case DLG_DES_GR_BMP:
			FreeBitmapsDesigner(dlgId);
			break;
		default:
			break;
	}
	FreeCommonDesigner(dlgId);

	if (dlgId)
		F_ApiClose(dlgId, 0);

	DesPanels[dlgNum].desId = 0;
	CurrDesigner = 0;
#endif
}


VoidT CheckDesChanges(VoidT)
{
	F_ObjHandleT dlgId = DesPanels[CurrDesigner].desId;

	if (DesignerChanged
	 && (F_ApiAlert(ApplyChanges, FF_ALERT_YES_DEFAULT) != -1)) {
		GetCurrDesigner(dlgId, CurrDesigner);
	}
	DesignerChanged = False;
}



/* document info */


VoidT GetDocInfo(F_ObjHandleT docId, BoolT isBook)
{
	F_ObjHandleT bookId = 0;
	F_ObjHandleT tocId = 0;
	F_ObjHandleT chapId = 0;
	F_ObjHandleT compId = 0;

	BoolT tocOpened = False;
	BoolT chapOpened = False;

	StringT sname = NULL;
	StringT fname = NULL;

	/* if book, include TOC and first non-generated chapter */
	if (isBook) {
		bookId = docId;
		compId = F_ApiGetId(FV_SessionId, bookId, FP_FirstComponentInBook);
		while (compId) {
			if ((tocId == 0)
			 && (F_ApiGetInt(bookId, compId, FP_BookComponentType) == FV_BK_TOC)
			 && ((fname = F_ApiGetString(bookId, compId, FP_Name)) != NULL)) {
				if ((tocId = IdentifyDocName(fname)) == 0) { /* not already open */
					if ((tocId = OpenDocName(fname)) != 0)
						tocOpened = True;
				}
				F_StrFree(fname);
			}
			else if (!F_ApiGetInt(bookId, compId, FP_BookComponentIsGeneratable)
			 && ((fname = F_ApiGetString(bookId, compId, FP_Name)) != NULL)) {
				if ((chapId = IdentifyDocName(fname)) == 0) { /* not already open */
					if ((chapId = OpenDocName(fname)) != 0)
						chapOpened = True;
				}
				F_StrFree(fname);
				if (chapId != 0)
					break;
			}
			compId = F_ApiGetId(bookId, compId, FP_NextComponentInBook);
		}
	}

	/* get full list of styles */
	LastDesStyle = &FirstDesStyle;
	FirstDesStyle.next = NULL;

	/* get list of text flow tags */
	LastTextFlow = &FirstTextFlow;
	FirstTextFlow.next = NULL;

	/* get list of files */
	LastCompFile = &FirstCompFile;
	FirstCompFile.next = NULL;

	/* get list of font tags */
	LastFont = &FirstFont;
	FirstFont.next = NULL;

	GetSessionFonts();

	if (isBook) {
		GetDocStyles(tocId, True);
		GetDocStyles(tocId, False);
		GetDocStyles(chapId, True);
		GetDocStyles(chapId, False);
		GetDocFlows(chapId);
		GetBookCompFiles(bookId);
	}
	else {
		GetDocStyles(docId, True);
		GetDocStyles(docId, False);
		GetDocFlows(docId); 
		GetDocCompFile(docId);
	}

	DesStyles = FirstDesStyle.next;
	DesTextFlows = FirstTextFlow.next;
	DesFonts = FirstFont.next;
	DesCompFiles = FirstCompFile.next;
	
	if (isBook) {
		if (tocOpened == True)
			F_ApiClose(tocId, FF_CLOSE_MODIFIED);
		if (chapOpened == True)
			F_ApiClose(chapId, FF_CLOSE_MODIFIED);
	}
}



/* event processing */

VoidT ProcDesignerEvent(IntT dlgNum, IntT itemNum)
{
	F_ObjHandleT dlgId = 0;

	if ((CurrDesigner == 0)
	 || (CurrDesigner != dlgNum))
		return;

	dlgId = DesPanels[dlgNum].desId;

	if (itemNum == FV_DlgClose) {
		if (DesignerInitializing)
			return;
		CheckDesChanges();
		DesX = F_ApiGetInt(FV_SessionId, dlgId, FP_ScreenX);
		DesY = F_ApiGetInt(FV_SessionId, dlgId, FP_ScreenY);
#ifdef WIN32
		hCurrDes = DesPanels[dlgNum].wnd;
		SetActiveWindow(FrameMakerWnd);
		ShowWindow(hCurrDes, SW_HIDE);
		F_ApiReturnValue(FR_DialogStayUp);
#else
		DesPanels[dlgNum].desId = 0;
		CurrDesigner = 0;
#endif
		return;
	}

	if (itemNum < DLG_DES_COMMON_COUNT)
		ProcCommonDesignerEvent(dlgId, itemNum);
	else switch (dlgNum) {
		case DLG_DES_HS_BASIC:
			ProcBasicStyleDesignerEvent(dlgId, itemNum);
			break;
		case DLG_DES_HS_TOPIC:
			ProcHelpTopicDesignerEvent(dlgId, itemNum);
			break;
		case DLG_DES_HS_HOTSP:
			ProcHelpHotspotDesignerEvent(dlgId, itemNum);
			break;
		case DLG_DES_HS_TARGET:
			ProcHelpTargetDesignerEvent(dlgId, itemNum);
			break;
		case DLG_DES_HS_MACRO:
			ProcHelpMacroDesignerEvent(dlgId, itemNum);
			break;
		case DLG_DES_TX_BASIC:
			ProcBasicTextDesignerEvent(dlgId, itemNum);
			break;
		case DLG_DES_TX_LAYOUT:
			ProcTextLayoutDesignerEvent(dlgId, itemNum);
			break;
		case DLG_DES_GR_BASIC:
			ProcBasicGraphicsDesignerEvent(dlgId, itemNum);
			break;
		case DLG_DES_GR_IMPORT:
			ProcImportedGraphicsDesignerEvent(dlgId, itemNum);
			break;
		case DLG_DES_GR_TEXT:
			ProcGraphicsTextDesignerEvent(dlgId, itemNum);
			break;
		case DLG_DES_TX_TABLE:
			ProcTableDesignerEvent(dlgId, itemNum);
			break;
		case DLG_DES_TX_FONT:
			ProcFontDesignerEvent(dlgId, itemNum);
			break;
		case DLG_DES_TX_ADV:
			ProcAdvancedTextDesignerEvent(dlgId, itemNum);
			break;
		case DLG_DES_TX_HELP:
			ProcHelpTextDesignerEvent(dlgId, itemNum);
			break;
		case DLG_DES_TX_VER:
			ProcVersionDesignerEvent(dlgId, itemNum);
			break;
		case DLG_DES_TX_CNT:
			ProcHelpContentsDesignerEvent(dlgId, itemNum);
			break;
		case DLG_DES_TX_HPJ:
			ProcHelpCompileDesignerEvent(dlgId, itemNum);
			break;
		case DLG_DES_TX_TPL:
			ProcTemplateDesignerEvent(dlgId, itemNum);
			break;
		case DLG_DES_TX_HMK:
			ProcHelpMarkerDesignerEvent(dlgId, itemNum);
			break;
		case DLG_DES_TX_MULT:
			ProcMultiFileDesignerEvent(dlgId, itemNum);
			break;
		case DLG_DES_GR_FRM:
			ProcGraphicFramesDesignerEvent(dlgId, itemNum);
			break;
		case DLG_DES_GR_BMP:
			ProcBitmapsDesignerEvent(dlgId, itemNum);
			break;
		default:
			break;
	}
}


VoidT ProcDesCommand(F_ObjHandleT dlgId, IntT command)
{
	DesStyleT *currTempStyle;
	TextPropT currTempText;
	TextFlowT *currTempTextFlow;
	FontT *currTempFont;
	CompFileT *currTempCompFile;
	GraphPropT currTempGraph;
	GrExPropT *currTempExportGraph;

	BoolT tmpStylesChanged;
	BoolT tmpTextChanged;
	BoolT tmpTextFlowChanged;
	BoolT tmpFontChanged;
	BoolT tmpCompFileChanged;
	BoolT tmpGraphicsChanged;
	BoolT tmpExportGraphsChanged;


	switch (command) {
		case DLG_DES_CMD_EDIT:
			if (FileExists(CurrIni))
				EditIniFile(CurrIni);
			break;
		case DLG_DES_CMD_RELOAD:
			if (FileExists(CurrIni)) {
				InitDesignerStyles();
				InitDesignerFlows();
				InitDesignerFonts();
				ReadTextPropsFromIni(CurrIni, True);
				ReadGraphPropsFromIni(CurrIni, True);
				InitNewDesigner(CurrDesigner);
			}
			break;
		case DLG_DES_CMD_COPY:
			switch (CurrNameListNum) {
				case DES_NAMES_ALL_STYLES:
					currTempText = CurrDesText;
					CurrDesText = ClipboardText;
					tmpTextChanged = TextChanged;
					GetCurrDesigner(dlgId, CurrDesigner);
					ClipboardText = CurrDesText;
					TextChanged = tmpTextChanged;
					CurrDesText = currTempText;
					break;
				case DES_NAMES_FLOWS:
					currTempText = CurrDesText;
					CurrDesText = ClipboardText;
					tmpTextChanged = TextChanged;
					currTempTextFlow = CurrTextFlow;
					CurrTextFlow = &ClipboardTextFlow;
					tmpTextFlowChanged = TextFlowChanged;
					GetCurrDesigner(dlgId, CurrDesigner);
					ClipboardText = CurrDesText;
					TextChanged = tmpTextChanged;
					CurrDesText = currTempText;
					TextFlowChanged = tmpTextFlowChanged;
					CurrTextFlow = currTempTextFlow;
					break;
				case DES_NAMES_FONTS:
					currTempText = CurrDesText;
					CurrDesText = ClipboardText;
					tmpTextChanged = TextChanged;
					currTempFont = CurrFont;
					CurrFont = &ClipboardFont;
					tmpFontChanged = FontChanged;
					GetCurrDesigner(dlgId, CurrDesigner);
					ClipboardText = CurrDesText;
					TextChanged = tmpTextChanged;
					CurrDesText = currTempText;
					FontChanged = tmpFontChanged;
					CurrFont = currTempFont;
					break;
				case DES_NAMES_FILES:
					currTempText = CurrDesText;
					CurrDesText = ClipboardText;
					tmpTextChanged = TextChanged;
					currTempCompFile = CurrCompFile;
					CurrCompFile = &ClipboardCompFile;
					tmpCompFileChanged = CompFileChanged;
					GetCurrDesigner(dlgId, CurrDesigner);
					ClipboardText = CurrDesText;
					TextChanged = tmpTextChanged;
					CurrDesText = currTempText;
					CompFileChanged = tmpCompFileChanged;
					CurrCompFile = currTempCompFile;
					break;
				case DES_NAMES_STYLES:
					currTempStyle = CurrDesStyle;
					CurrDesStyle = &ClipboardStyle;
					tmpStylesChanged = StylesChanged;
					GetCurrDesigner(dlgId, CurrDesigner);
					StylesChanged = tmpStylesChanged;
					CurrDesStyle = currTempStyle;
					break;
				case DES_NAMES_ALL_FORMS:
				case DES_NAMES_GR_TEXT:
					currTempGraph = CurrDesGraph;
					CurrDesGraph = ClipboardGraph;
					tmpGraphicsChanged = GraphicsChanged;
					GetCurrDesigner(dlgId, CurrDesigner);
					ClipboardGraph = CurrDesGraph;
					GraphicsChanged = tmpGraphicsChanged;
					CurrDesGraph = currTempGraph;
					break;
				case DES_NAMES_GR_FORMS:
					currTempGraph = CurrDesGraph;
					CurrDesGraph = ClipboardGraph;
					tmpGraphicsChanged = GraphicsChanged;
					currTempExportGraph = CurrExportGraph;
					CurrExportGraph = &ClipboardExportGraph;
					tmpExportGraphsChanged = ExportGraphsChanged;
					GetCurrDesigner(dlgId, CurrDesigner);
					ClipboardGraph = CurrDesGraph;
					GraphicsChanged = tmpGraphicsChanged;
					CurrDesGraph = currTempGraph;
					CurrExportGraph = currTempExportGraph;
					ExportGraphsChanged = tmpExportGraphsChanged;
					break;
				default:
					break;
			}
			break;
		case DLG_DES_CMD_PASTE:
			switch (CurrNameListNum) {
				case DES_NAMES_ALL_STYLES:
					currTempText = CurrDesText;
					CurrDesText = ClipboardText;
					SetCurrDesigner(dlgId, CurrDesigner);
					DesignerChanged = True;
					CurrDesText = currTempText;
					break;
				case DES_NAMES_FLOWS:
					currTempText = CurrDesText;
					CurrDesText = ClipboardText;
					currTempTextFlow = CurrTextFlow;
					CurrTextFlow = &ClipboardTextFlow;
					SetCurrDesigner(dlgId, CurrDesigner);
					DesignerChanged = True;
					CurrDesText = currTempText;
					CurrTextFlow = currTempTextFlow;
					break;
				case DES_NAMES_FONTS:
					currTempText = CurrDesText;
					CurrDesText = ClipboardText;
					currTempFont = CurrFont;
					CurrFont = &ClipboardFont;
					SetCurrDesigner(dlgId, CurrDesigner);
					DesignerChanged = True;
					CurrDesText = currTempText;
					CurrFont = currTempFont;
					break;
				case DES_NAMES_FILES:
					currTempText = CurrDesText;
					CurrDesText = ClipboardText;
					currTempCompFile = CurrCompFile;
					CurrCompFile = &ClipboardCompFile;
					SetCurrDesigner(dlgId, CurrDesigner);
					DesignerChanged = True;
					CurrDesText = currTempText;
					CurrCompFile = currTempCompFile;
					break;
				case DES_NAMES_STYLES:
					currTempStyle = CurrDesStyle;
					CurrDesStyle = &ClipboardStyle;
					SetCurrDesigner(dlgId, CurrDesigner);
					DesignerChanged = True;
					CurrDesStyle = currTempStyle;
					break;
				case DES_NAMES_ALL_FORMS:
				case DES_NAMES_GR_TEXT:
					currTempGraph = CurrDesGraph;
					CurrDesGraph = ClipboardGraph;
					SetCurrDesigner(dlgId, CurrDesigner);
					DesignerChanged = True;
					CurrDesGraph = currTempGraph;
					break;
				case DES_NAMES_GR_FORMS:
					currTempGraph = CurrDesGraph;
					CurrDesGraph = ClipboardGraph;
					currTempExportGraph = CurrExportGraph;
					CurrExportGraph = &ClipboardExportGraph;
					SetCurrDesigner(dlgId, CurrDesigner);
					DesignerChanged = True;
					CurrDesGraph = currTempGraph;
					CurrExportGraph = currTempExportGraph;
					break;
				default:
					break;
			}
			break;
		case DLG_DES_CMD_RESET:
			SetCurrDesigner(dlgId, CurrDesigner);
			DesignerChanged = False;
			break;
		case DLG_DES_CMD_CLEAR:
			switch (CurrNameListNum) {
				case DES_NAMES_ALL_STYLES:
					currTempText = CurrDesText;
					CurrDesText = DefDesText;
					SetCurrDesigner(dlgId, CurrDesigner);
					DesignerChanged = True;
					CurrDesText = currTempText;			
					break;
				case DES_NAMES_FLOWS:
					currTempText = CurrDesText;
					CurrDesText = DefDesText;
					currTempTextFlow = CurrTextFlow;
					CurrTextFlow = &DefTextFlow;
					SetCurrDesigner(dlgId, CurrDesigner);
					DesignerChanged = True;
					CurrDesText = currTempText;			
					CurrTextFlow = currTempTextFlow;
					break;
				case DES_NAMES_FONTS:
					currTempText = CurrDesText;
					CurrDesText = DefDesText;
					currTempFont = CurrFont;
					CurrFont = &DefFont;
					SetCurrDesigner(dlgId, CurrDesigner);
					DesignerChanged = True;
					CurrDesText = currTempText;			
					CurrFont = currTempFont;
					break;
				case DES_NAMES_FILES:
					currTempText = CurrDesText;
					CurrDesText = DefDesText;
					currTempCompFile = CurrCompFile;
					CurrCompFile = &DefCompFile;
					SetCurrDesigner(dlgId, CurrDesigner);
					DesignerChanged = True;
					CurrDesText = currTempText;			
					CurrCompFile = currTempCompFile;
					break;
				case DES_NAMES_STYLES:
					currTempStyle = CurrDesStyle;
					CurrDesStyle = &EmptyStyle;
					SetCurrDesigner(dlgId, CurrDesigner);
					DesignerChanged = True;
					CurrDesStyle = currTempStyle;			
					break;
				case DES_NAMES_ALL_FORMS:
				case DES_NAMES_GR_TEXT:
					currTempGraph = CurrDesGraph;
					CurrDesGraph = DefDesGraph;
					SetCurrDesigner(dlgId, CurrDesigner);
					DesignerChanged = True;
					CurrDesGraph = currTempGraph;
					break;
				case DES_NAMES_GR_FORMS:
					currTempGraph = CurrDesGraph;
					CurrDesGraph = DefDesGraph;
					currTempExportGraph = CurrExportGraph;
					CurrExportGraph = &ExportGraphDef[CurrExportGraphNum];
					SetCurrDesigner(dlgId, CurrDesigner);
					DesignerChanged = True;
					CurrDesGraph = currTempGraph;
					CurrExportGraph = currTempExportGraph;
					break;
				default:
					break;
			}
			break;
		case DLG_DES_CMD_PARA:
			if (CurrNameListNum != DES_NAMES_STYLES)
				break;
			ShowPara = True;
			ShowChar = False;
			StyleListChanged = True;
			InitNewDesigner(CurrDesigner);
			break;
		case DLG_DES_CMD_CHAR:
			if (CurrNameListNum != DES_NAMES_STYLES)
				break;
			ShowPara = False;
			ShowChar = True;
			StyleListChanged = True;
			InitNewDesigner(CurrDesigner);
			break;
		case DLG_DES_CMD_BOTH:
			if (CurrNameListNum != DES_NAMES_STYLES)
				break;
			ShowPara = True;
			ShowChar = True;
			StyleListChanged = True;
			InitNewDesigner(CurrDesigner);
			break;
		default:
			break;
	}
}


VoidT GetCurrDesigner(F_ObjHandleT dlgId, IntT dlgNum)
{
	switch (dlgNum) {
		case DLG_DES_HS_BASIC:
			GetBasicStyleDesigner(dlgId);
			break;
		case DLG_DES_HS_TOPIC:
			GetHelpTopicDesigner(dlgId);
			break;
		case DLG_DES_HS_HOTSP:
			GetHelpHotspotDesigner(dlgId);
			break;
		case DLG_DES_HS_TARGET:
			GetHelpTargetDesigner(dlgId);
			break;
		case DLG_DES_HS_MACRO:
			GetHelpMacroDesigner(dlgId);
			break;
		case DLG_DES_TX_BASIC:
			GetBasicTextDesigner(dlgId);
			break;
		case DLG_DES_TX_LAYOUT:
			GetTextLayoutDesigner(dlgId);
			break;
		case DLG_DES_GR_BASIC:
			GetBasicGraphicsDesigner(dlgId);
			break;
		case DLG_DES_GR_IMPORT:
			GetImportedGraphicsDesigner(dlgId);
			break;
		case DLG_DES_GR_TEXT:
			GetGraphicsTextDesigner(dlgId);
			break;
		case DLG_DES_TX_TABLE:
			GetTableDesigner(dlgId);
			break;
		case DLG_DES_TX_FONT:
			GetFontDesigner(dlgId);
			break;
		case DLG_DES_TX_ADV:
			GetAdvancedTextDesigner(dlgId);
			break;
		case DLG_DES_TX_HELP:
			GetHelpTextDesigner(dlgId);
			break;
		case DLG_DES_TX_VER:
			GetVersionDesigner(dlgId);
			break;
		case DLG_DES_TX_CNT:
			GetHelpContentsDesigner(dlgId);
			break;
		case DLG_DES_TX_HPJ:
			GetHelpCompileDesigner(dlgId);
			break;
		case DLG_DES_TX_TPL:
			GetTemplateDesigner(dlgId);
			break;
		case DLG_DES_TX_HMK:
			GetHelpMarkerDesigner(dlgId);
			break;
		case DLG_DES_TX_MULT:
			GetMultiFileDesigner(dlgId);
			break;
		case DLG_DES_GR_FRM:
			GetGraphicFramesDesigner(dlgId);
			break;
		case DLG_DES_GR_BMP:
			GetBitmapsDesigner(dlgId);
			break;
		default:
			break;
	}
}


VoidT SetCurrDesigner(F_ObjHandleT dlgId,  IntT dlgNum)
{
	switch (dlgNum) {
		case DLG_DES_HS_BASIC:
			SetBasicStyleDesigner(dlgId);
			break;
		case DLG_DES_HS_TOPIC:
			SetHelpTopicDesigner(dlgId);
			break;
		case DLG_DES_HS_HOTSP:
			SetHelpHotspotDesigner(dlgId);
			break;
		case DLG_DES_HS_TARGET:
			SetHelpTargetDesigner(dlgId);
			break;
		case DLG_DES_HS_MACRO:
			SetHelpMacroDesigner(dlgId);
			break;
		case DLG_DES_TX_BASIC:
			SetBasicTextDesigner(dlgId);
			break;
		case DLG_DES_TX_LAYOUT:
			SetTextLayoutDesigner(dlgId);
			break;
		case DLG_DES_GR_BASIC:
			SetBasicGraphicsDesigner(dlgId);
			break;
		case DLG_DES_GR_IMPORT:
			SetImportedGraphicsDesigner(dlgId);
			break;
		case DLG_DES_GR_TEXT:
			SetGraphicsTextDesigner(dlgId);
			break;
		case DLG_DES_TX_TABLE:
			SetTableDesigner(dlgId);
			break;
		case DLG_DES_TX_FONT:
			SetFontDesigner(dlgId);
			break;
		case DLG_DES_TX_ADV:
			SetAdvancedTextDesigner(dlgId);
			break;
		case DLG_DES_TX_HELP:
			SetHelpTextDesigner(dlgId);
			break;
		case DLG_DES_TX_VER:
			SetVersionDesigner(dlgId);
			break;
		case DLG_DES_TX_CNT:
			SetHelpContentsDesigner(dlgId);
			break;
		case DLG_DES_TX_HPJ:
			SetHelpCompileDesigner(dlgId);
			break;
		case DLG_DES_TX_TPL:
			SetTemplateDesigner(dlgId);
			break;
		case DLG_DES_TX_HMK:
			SetHelpMarkerDesigner(dlgId);
			break;
		case DLG_DES_TX_MULT:
			SetMultiFileDesigner(dlgId);
			break;
		case DLG_DES_GR_FRM:
			SetGraphicFramesDesigner(dlgId);
			break;
		case DLG_DES_GR_BMP:
			SetBitmapsDesigner(dlgId);
			break;
		default:
			break;
	}
}


/**************************************************************/
/* Common designer section of all pages */

/* common designer items */

#define DLG_DC_PROJ_LAB  1

#define DLG_DC_NAME_LAB  2
#define DLG_DC_NAME_BOX  3
#define DLG_DC_NAME_LIST 4

#define DLG_DC_PROP_LIST 6

#define DLG_DC_APP_BTN   8
#define DLG_DC_APP_LAB   9

#define DLG_DC_UPD_BTN  11
#define DLG_DC_UPD_LAB  12

#define DLG_DC_CMD_LIST 15



VoidT ProcCommonDesignerEvent(F_ObjHandleT dlgId, IntT itemNum)
{
	IntT newNameSel = 0;

	switch (itemNum) {
		case DLG_DC_APP_BTN:  /* set properties for curr item */
			if (DesignerChanged == False)
				break;
			DesignerChanged = False;
			GetCurrDesigner(dlgId, CurrDesigner);
			break;

		case DLG_DC_UPD_BTN:
			if (FileExists(CurrIni)) {
				WriteStylesToIni(CurrIni, False);
				WriteTextPropsToIni(CurrIni, False);
				WriteFlowsToIni(CurrIni, False);
				WriteFontsToIni(CurrIni, False);
				WriteGraphPropsToIni(CurrIni, False);
				WriteCompFilesToIni(CurrIni, False);
			}
			break;

		case DLG_DC_NAME_LIST:
			if (CurrNameList->nameCount == 1)
				break;

			newNameSel = GetDlgState(dlgId, itemNum);
			if (CurrNameList->nameSel == newNameSel)
				break;

			CheckDesChanges();
			switch (CurrNameListNum) {
				case DES_NAMES_ALL_STYLES:
				case DES_NAMES_ALL_FORMS:
				case DES_NAMES_GR_TEXT:
					/* fixed single-item name lists, should never come here */
					break;
				case DES_NAMES_STYLES:
					SetNewStyle(newNameSel);
					CurrNameList->nameSel = CurrStyle;
					CurrNameList->listLab = CurrDesStyle->para ? ParaDesLab : CharDesLab;
					SetDlgText(dlgId, DLG_DC_NAME_BOX, DesStyleList[CurrStyle]);	
					SetDlgLabel(dlgId, DLG_DC_NAME_LAB, CurrNameList->listLab);
					break;
				case DES_NAMES_FLOWS:
					SetNewTextFlow(newNameSel);
					CurrNameList->nameSel = CurrTextFlowNum;
					SetDlgText(dlgId, DLG_DC_NAME_BOX, 
														DesTextFlowList[CurrTextFlowNum]);	
					break;
				case DES_NAMES_GR_FORMS:
					SetNewGrForm(newNameSel);
					CurrNameList->nameSel = CurrExportGraphNum;
					SetDlgText(dlgId, DLG_DC_NAME_BOX, 
														ImportDesNames[CurrExportGraphNum]);	
					break;
				case DES_NAMES_FONTS:
					SetNewFont(newNameSel);
					CurrNameList->nameSel = CurrFontNum;
					SetDlgText(dlgId, DLG_DC_NAME_BOX, 
														DesFontList[CurrFontNum]);	
					break;
				case DES_NAMES_FILES:
					SetNewCompFile(newNameSel);
					CurrNameList->nameSel = CurrCompFileNum;
					SetDlgText(dlgId, DLG_DC_NAME_BOX, 
														DesCompFileList[CurrCompFileNum]);	
					break;
				default:
					break;
			}
			SetCurrDesigner(dlgId, CurrDesigner);
			DesignerChanged = False;
			break;

		case DLG_DC_PROP_LIST:
			/* set NextDesigner if choosing different properties */
			NextDesPropNum = GetDlgState(dlgId, itemNum);
			NextDesigner = CurrPropSet->desPanels[NextDesPropNum - 1];
			if (NextDesigner != CurrDesigner) {
				DesX = F_ApiGetInt(FV_SessionId, dlgId, FP_ScreenX);
				DesY = F_ApiGetInt(FV_SessionId, dlgId, FP_ScreenY);
				InitNewDesigner(NextDesigner);
			}
			break;

		case DLG_DC_CMD_LIST:
			LastCmd = GetDlgState(dlgId, itemNum);
			ProcDesCommand(dlgId, LastCmd);
			break;

		default:
			break;
	}
}


VoidT InitCommonDesigner(F_ObjHandleT dlgId)
{
	SetDlgProject(dlgId, DLG_DC_PROJ_LAB);

	/* set up name list */
	NameStrings.val = CurrNameList->nameList;
	NameStrings.len = CurrNameList->nameCount + 1;
	SetLabels(dlgId, DLG_DC_NAME_LIST, &NameStrings);
	SetDlgState(dlgId, DLG_DC_NAME_LIST, CurrNameList->nameSel);
	SetDlgText(dlgId, DLG_DC_NAME_BOX,
						 CurrNameList->nameList[CurrNameList->nameSel]);
	SetDlgLabel(dlgId, DLG_DC_NAME_LAB, CurrNameList->listLab);
	SetDlgLabel(dlgId, DLG_DC_APP_LAB, CurrNameList->applyTo);

	/* set up properties list */
	PropStrings.val = CurrPropSet->propList;
	PropStrings.len = CurrPropSet->propCount + 1;
	SetLabels(dlgId, DLG_DC_PROP_LIST, &PropStrings);
	SetDlgState(dlgId, DLG_DC_PROP_LIST, NextDesPropNum);	

	/* set up command list */
	CmdStrings.val = CmdNameList;
	CmdStrings.len = DLG_DES_CMD_COUNT + 1;
	SetLabels(dlgId, DLG_DC_CMD_LIST, &CmdStrings);
	SetDlgState(dlgId, DLG_DC_CMD_LIST, LastCmd);	
}


VoidT FreeCommonDesigner(F_ObjHandleT dlgId)
{
	/* no resources to release */
}



/* end of file m2rdes.c */

