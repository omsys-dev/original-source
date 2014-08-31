/* m2gframe.h is the Frame FDK header for mif2go's C++ plugin
 * Last edited by Jeremy H. Griffith.
 * Copyright 2008 Omni Systems, Inc.  All Rights Reserved.
 */

#ifndef M2GFRAME_H
#define M2GFRAME_H

class M2GFrame;

// our own classes
#include "dcglob.h"
#include "dcbase.h"
#include "dclist.h"
#include "dcnode.h"
#include "dcfile.h"
#include "dcfilr.h"
#include "dcfilw.h"
#include "dclog.h"
#include "dcfilp.h"
#include "dcfili.h"
#include "dcini.h"
#include "dcmacr.h"

extern "C" {
// additional system headers
#include <direct.h>
#include <io.h>

// FDK headers
#include "fapi.h"

#define DONT_REDEFINE
#include "fdetypes.h"
#undef DONT_REDEFINE

#include "fcodes.h"
#include "fstrings.h"
#include "fmemory.h"
#include "futils.h"
#include "fmetrics.h"
#include "fcharmap.h"

#include <windows.h>
#include <commdlg.h>
#include <shlobj.h>
}


class M2GFrame {

// eliminate stupid warning about bool, true, and false
//  being reserved by Microsoft for future use
#pragma warning( disable : 4237 )

	// in m2gdlg.cpp
	static char FileName[];
	static char DlgTitle[];
	static char DlgDir[];
	static OPENFILENAME OfnStruct;

	static char PathName[];
	static int FolderImg;
	static BROWSEINFO BrinfStruct;

	static char **ParseOpenArgs(char *chp);
	static UINT APIENTRY OFNHook(HWND hdlg, UINT uiMsg,
	                        WPARAM wParam, LPARAM lParam);
	static bool SetItemIdList(char *path);
	static int __stdcall BrinfCB(HWND hwnd, UINT uMsg, 
													LPARAM lParam, LPARAM lpData);

	// in m2gini.cpp
	static DCirfile *CurrIni;
	static DCirfile *CurrMacros;
	static DCirfile *OrigIni;
	static char *IniName;
	static char *SecIniName;
	static char *MacrosName;
	static secinfo *FDKIniSects;
	static unl GetArgs(char *chp, char **argv, unl arlim = 1);

	// in m2gmacr.cpp
	static char *MacroFileName;
	static bool ValidOnly;
	static DCwfile *MacroTemp;
	static long MacroDiv;
	static char *BaseName;
	static char *PrjPath;
	static char *CurrPath;
	static unl MacroDivPtr;
	static bool WriteMacroChar(DCwfile *df, unc uch);
	static bool WritePredefinedMacro(DCwfile *df, char *name);
	static char *GetPredefinedVar(DCwfile *df, char *vp);
	static char *GetFrameUserVar(char *vp);
	static long GetFrameUserArrVar(char *vp, long idx);

 public:

	// in m2gframe.cpp
	static StringT PluginVersion;

	static bool UseLog;
	static bool ShowLog;
	static bool Logging;
	static bool LogMsg;
	static char *LogFileName;
	static char *HistoryFileName;
	static char *EditorFileName;
	static DClog *Log;
	static unc LogLevels[logmax];

	static void OpenProject(char *chp);
	static void CloseProject(char *chp);
	static void OpenLog(char *chp);
	static void CloseLog(char *chp);
	static void LogEvent(char *chp);

	// in m2gdlg.cpp
	static void GetOpenFile(char *chp);
	static void GetOpenDir(char *chp);
	static void GetMyDocDir(char *chp);
	static void WaitFileDone(char *chp);
	static void SetPMFlag(void);
	static bool CheckLogErrors(void);
	static void ShowLogFile(void);

	static int ExitCode;
	static int ExitType;
	static char *ComSpec;
	static char *CurrDir;

	static void RunDOSCommand(char *cmd, bool hide = true,
	             bool keep = false, bool synch = true);
	static void RunCommand(char *command, char *cd,
	             bool synch, bool hide, bool sized);

	// in m2gini.cpp
	static void SetUpIni(char *chp);
	static void GetIniSects(char *chp);
	static void GetIniGroup(char *chp);
	static void GetIniSetting(char *chp, bool macro = false);
	static void GetIniSection(char *chp);
	static void GetIniSectList(char *chp);
	static void IniProcDone(void);

	static void PGetSetting(char *chp);
	static void PPutSetting(char *chp);
	static void PRenSect(char *chp);
	static void PUpdate(char *chp);
	static void PClose(char *chp);

	// in m2gmacr.cpp
	static void SetUpMacros(void);
	static void MacroPreVars(char *chp);
	static void MacroProc(char *chp);
	static void WriteMacro(char *mactxt, char *buf, char **bufptr);
	static char *WriteMacroName(char **pmacname, bool point);
	static void MacroProcDone(void);
};


#endif

/* end of m2gframe.h */
