/////////////////////////////////////////////////////////////////////////////
// Name:        d2gutil.cpp
// Purpose:     D2g utility functions
// Author:      Jeremy H. Griffith
// Modified by: 
// Created:     28/12/2009
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

#pragma warning( disable : 4189 )

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

#include "d2gutil.h"



// global variables

DCirfile *ProjectFile = NULL;
char *ProjectFileName = NULL;
char ProjBuf[PBSIZE];

char *SystemFileName = NULL;
DCirfile *SysIni = NULL;

DCirfile *IniFile = NULL;
char *WrapPath = NULL;

char *ProjectsIni = {
 "; projects.ini is the container for DITA2Go project info\n"
 "; on the current system\n\n"
 "\n[Options]\n"
 "PosX=0\n"
 "PosY=0\n"
 "SizeX=0\n"
 "SizeY=0\n"
 "Editor=notepad\n"
#ifdef DITA
 "d2gpmVersion=1\n"
 "NewIniPath=%omsyshome%\\d2g\\local\\starts\n"
 "LogFileName=_d2g_log.txt\n"
 "CentralProjDir=%omsyshome%\\d2g\\documents\n"
#endif
#ifdef UDOC
 "x2gpmVersion=1\n"
 "NewIniPath=%omsyshome%\\x2g\\local\\starts\n"
 "LogFileName=_x2g_log.txt\n"
 "CentralProjDir=%omsyshome%\\x2g\\documents\n"
#endif
 "ShowDelInCreate=No\n"
 "ShowLog=No\n"
 "ShowConsole=No\n"
 "ProjDirUsed=Yes\n"
 "LocalProjDir=No\n"
 "LocalProjDirRel=..\\_config\n"
 "\n[Projects]\n"
 "NextID=1\n"
 "LastActive=0\n"
 "LastDeleted=0\n"
 "NextDirNum=1\n"
 "\n[Active]\n"
 "\n[Deleted]\n"
 "\n[SourceDir]\n"
 "\n[SourceMap]\n"
 "\n[OutputType]\n"
 "\n[OutputDir]\n"
 "\n[LocalProjDir]\n"
 "\n[ProjDir]\n"
 "\n[ProjFile]\n"
 "\n[DvalUsed]\n"
 "\n[DvalFile]\n"
 "\n[End]\n\n"
};


DCnlist SharedSrcDir;
DCnlist SharedSrcMap;
DCnlist SharedSrcID;


char *Unescape(char *chp)
{
	// process backslash escapes
	char *dst = NewName(strlen(chp));
	char *ret = dst;
	while (*chp) {
		if (*chp == '\\') {
			switch (*++chp) {;
				case 'n':
				case 'r':
					*dst++ = '\n';
					break;
				case '~':
					*dst++ = ' ';
					break;
				default:
					*dst++ = *chp;
					break;
			}
			chp++;
		}
		else
			*dst++ = *chp++;
	}
	*dst = '\0';
	return ret;
}



// globals for running commands

char *ComSpec = NULL;
unl ExitType = 0;
unl LocalExitCode = 0;
char PathBuf[MAXPATH];
bool StopCommand = false;
HANDLE CurrProc = NULL;
LPSTARTUPINFO startInfo = NULL;
PROCESS_INFORMATION *procInfo = NULL;


void RunCommand(const char *command, const char *dir, bool hide, bool synch)
{
#ifdef WIN32
	int fmHigh = 0;
	int fmWide = 0;
	int fmLeft = 0;
	int fmTop = 0;

	ExitType = 3;

	startInfo = (LPSTARTUPINFO) NewMem(sizeof(STARTUPINFO));
	if (startInfo == NULL)
		return;

	startInfo->cb = sizeof(STARTUPINFO);
	startInfo->dwFlags = STARTF_USESHOWWINDOW;
	startInfo->wShowWindow = hide ? SW_HIDE : SW_SHOWNORMAL;

#if 0
	if (sized) {
		fmHigh = F_ApiGetInt(0, FV_SessionId, FP_ScreenHeight);
		fmWide = F_ApiGetInt(0, FV_SessionId, FP_ScreenWidth);
		fmLeft = F_ApiGetInt(0, FV_SessionId, FP_ScreenX);
		fmTop = F_ApiGetInt(0, FV_SessionId, FP_ScreenY);

		startInfo->dwXSize = (fmWide < 540) ? fmWide : 540;
		startInfo->dwYSize = (fmHigh < 400) ? fmHigh : 400;
		startInfo->dwX = (fmLeft > 100) ? fmLeft - 100 : fmLeft + 100;
		startInfo->dwY = (fmTop < 80) ? fmTop + 80 : fmTop - 80;
		startInfo->dwFlags |= (STARTF_USESIZE | STARTF_USEPOSITION);
	}
#endif

	procInfo = (PROCESS_INFORMATION *) NewMem(sizeof(PROCESS_INFORMATION));
	if (procInfo == NULL) {
		DeleteMem(startInfo);
		return;
	}

	ExitType = 0;

	if (!dir) {
		PathBuf[0] = '\0';
		dir = _getcwd(PathBuf, MAXPATH);
	}

	StopCommand = false;

	if (CreateProcess(NULL, (char *) command, NULL, NULL, TRUE, 0,
										NULL, dir, startInfo, procInfo)) {
		CurrProc = procInfo->hProcess;
		if (synch)
			WaitForCurrProcess();
		//WaitForSingleObject(procInfo->hProcess, INFINITE);
	}
	else {
		ExitType = 1;
		LocalExitCode = GetLastError();
		DeleteMem(startInfo);
		DeleteMem(procInfo);
	}

#else

	/* for Mac and UNIX */
	ExitType = 0;
	if ((ExitCode = system(command)) != 0)
		ExitType = 4;

#endif
}


void WaitForCurrProcess(void)
{
	unl objs = 1;
	unl res = 0;
	HANDLE proc[2];
	proc[0] = CurrProc;
	proc[1] = 0;

	while (!StopCommand) {
		res = MsgWaitForMultipleObjects(objs, proc, FALSE,
				                            INFINITE, QS_ALLINPUT);
		if (res == WAIT_OBJECT_0)
			break;
		if (res == WAIT_FAILED) {
			ExitType = 1;
			LocalExitCode = GetLastError();
			break;
		}
		if (res == (WAIT_OBJECT_0 + objs)) {
			MSG msg;
			while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) { 
				// if it's a quit message we're out of here 
        if (msg.message == WM_QUIT)
					return; 
				// otherwise dispatch it 
				TranslateMessage(&msg);
				DispatchMessage(&msg); 
				if (((msg.message == WM_KEYDOWN)
					&& (msg.wParam == VK_CANCEL))
				 || ((msg.message == WM_CHAR)
					&& (msg.wParam == 0x03))) {
					ExitType = 6;
					LocalExitCode = 1;
					StopCommand = true;
					break;
				}
			} // end of PeekMessage while loop 
		}
	}
	if (!StopCommand
	 && !GetExitCodeProcess(procInfo->hProcess, &LocalExitCode)) {
		ExitType = 2;
		LocalExitCode = GetLastError();
	}
	CloseHandle(procInfo->hProcess);
	CloseHandle(procInfo->hThread);
	DeleteMem(startInfo);
	DeleteMem(procInfo);
}


void RunDOSCommand(const char *cmd, const char *dir,
                   bool hide, bool keep, bool synch)
{
	char *command = NULL;

	if (!cmd
	 || (*cmd == '\0'))
		return;

	if (ComSpec == NULL) {
		ComSpec = getenv("comspec");
		if (ComSpec == NULL)
			strcpy(ComSpec, "command");
	}

	command = NewName(ComSpec, strlen(ComSpec) + strlen(cmd) + 5);
	strcat(command, keep ? " /k " : " /c ");
	strcat(command, cmd);

	RunCommand(command, dir, hide, synch);

	DeleteName(command);
}

char *ReportFail1 = "DITA2Go failed.for: ";

char *ReportFailType1 = "Could not run DCL filter or other program.";
char *ReportFailType2 = "Could not get program exit code.";
char *ReportFailType3 = "Could not allocate process structures.";
char *ReportFailType4 = "system() function failed.";
char *ReportFailType6 = "Cancelled by user.";

char *ReportFailCode1 = "Arguments unacceptable";
char *ReportFailCode2 = "Name incorrect";
char *ReportFailCode3 = "Memory unavailable";
char *ReportFailCode4 = "Cannot open file to read";
char *ReportFailCode5 = "File format incorrect";
char *ReportFailCode6 = "Error during file read";
char *ReportFailCode7 = "Cannot open file to write";
char *ReportFailCode8 = "Error during file write";
char *ReportFailCode9 = "Error in STDDEF.DCL";
char *ReportFailCode10 = "Incorrect file type";
char *ReportFailCode11 = "Could not run DLL, corrupted.";
char *ReportFailCode12 = "Could not run DLL, missing.";



void LogResults(char *cmd)
{
	char *msg = NULL;
	char *msg2 = NULL;

	if ((ExitType == 0) 
	 && (LocalExitCode == 0))
		return;

	if (ExitType == 0) {
		msg = ReportFail1;
		switch (LocalExitCode) {
			case 1:
				msg2 = ReportFailCode1;
				break;
			case 2:
				msg2 = ReportFailCode2;
				break;
			case 3:
				msg2 = ReportFailCode3;
				break;
			case 4:
				msg2 = ReportFailCode4;
				break;
			case 5:
				msg2 = ReportFailCode5;
				break;
			case 6:
				msg2 = ReportFailCode6;
				break;
			case 7:
				msg2 = ReportFailCode7;
				break;
			case 8:
				msg2 = ReportFailCode8;
				break;
			case 9:
				msg2 = ReportFailCode9;
				break;
			case 10:
				msg2 = ReportFailCode10;
				break;
			case 11:
				msg2 = ReportFailCode11;
				break;
			case 12:
				msg2 = ReportFailCode12;
				break;
			default:
				break;
		}
	}
	else {
		switch (ExitType) {
			case 1:
				msg = ReportFail1;
				msg2 = ReportFailType1;
				break;
			case 2:
				msg = ReportFailType2;
				break;
			case 3:
				msg = ReportFailType3;
				break;
			case 4:
				msg = ReportFailType4;
				break;
			case 5:
				//msg = ReportFailType5;
				break;
			case 6:
				msg = ReportFailType6;
				break;
			case 7:
				//msg = ReportFailType7;
				break;
			default:
				//msg = ReportFailTypeX;
				break;
		}
		if (ExitType == 1) {
			switch (LocalExitCode) {   /* from winerror.h */
				case 2:
					msg2 = "File not found.";
					break;
				case 3:
				case 53:
					msg2 = "Path not found.";
					break;
				case 5:
				case 65:
					msg2 = "Access denied.";
					break;
				case 15:
					msg2 = "Invalid drive specified.";
					break;
				case 32:
				case 33:
					msg2 = "File is unavailable.";
					break;
				case 111:
				case 206:
					msg2 = "File name is too long.";
					break;
				case 123:
				case 161:
					msg2 = "Invalid file or directory name.";
					break;
				case 190:
				case 191:
				case 192:
				case 193:
					msg2 = "Program file is damaged.";
					break;
				case 267:
					msg2 = "Invalid directory name, use only letters and digits.";
					break;
				case 1154:
					msg2 = "Required DLL is damaged.";
					break;
				case 1157:
					msg2 = "Required DLL is missing.";
					break;
				default:
					break;
			}
		}
	}

	if (msg)
		LogEvent(logerr, 1, msg, cmd, NULL, NULL, NULL);
	if (msg2)
		LogEvent(logcont, 1, msg2, NULL, NULL, NULL, NULL);

#if 0
		if (ExitType == 6)
			F_ApiAlert(UserCancelled, FF_ALERT_CONTINUE_NOTE);
		else if ((ExitType == 0)
		 && (LocalExitCode == 7)) {
			if (F_ApiAlert(WriteFailed, FF_ALERT_NO_DEFAULT) == -1)
				Quit = True;
		}
		else {
			if (F_ApiAlert(MiscFailure, FF_ALERT_NO_DEFAULT) == -1)
				Quit = True;
		}
#endif
}


char *LogFileName = NULL;
DClog *Log = NULL;
bool UseLog = true;
unc LogLevels[6];


void LogEvent(logtype lt, unc sev, const char *desc,
 const char *d2, const char *d3, const char *d4, const char *d5)
{
	if (!UseLog)
		return;

	Log->logEvent(lt, sev, desc, d2, d3, d4, d5);
}


HANDLE PMFlag = NULL;

void SetPMFlag(void)
{
	// set flag to tell drxml/drmxl not to move log file we just started
#ifdef DITA
	PMFlag = CreateSemaphore(NULL, 0L, 1L, "d2gpm");
#endif
#ifdef UDOC
	PMFlag = CreateSemaphore(NULL, 0L, 1L, "x2gpm");
#endif
}


bool CheckLogErrors(void)
{
	// in DLLs, use ReleaseSemaphore(PMFlag, 1L, NULL);
	// to signal errors by incrementing the semaphore

	switch (WaitForSingleObject(PMFlag, 1L)) {
		case WAIT_OBJECT_0:  //flag was incremented, now decremented to 0
			return true;   // errors detected by DLLs
		case WAIT_TIMEOUT:  // flag was still zero after one millisecond
			return false;  // no errors were detected
	}
	return false;  // should not get here
}


void OpenLog(void)
{
	// reopen same log file
	if (UseLog) {
		Log->ReopenFile(LogFileName, false);
		UseLog = (Log->FErr() == fok) ? true : false;
#ifdef DITA
		Log->logEvent(loginfo, 1, "Resuming log for d2gpm.");
#endif
#ifdef UDOC
		Log->logEvent(loginfo, 1, "Resuming log for x2gpm.");
#endif
	}
}

void CloseLog(void)
{
	if (UseLog)
		Log->CloseFile();
}


// end of d2gutil.cpp

