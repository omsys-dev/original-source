/////////////////////////////////////////////////////////////////////////////
// Name:        d2gutil.h
// Purpose:     
// Author:      Jeremy H. Griffith
// Modified by: 
// Created:     28/12/2009 16:01:53
// RCS-ID:      
// Copyright:   (c) Copyright 2009 Omni Systems, Inc.
// Licence:     
/////////////////////////////////////////////////////////////////////////////

#ifndef _D2GUTIL_H_
#define _D2GUTIL_H_


#define PBSIZE (32 * 1024)
#define MAXPATH 1024


extern DCirfile *ProjectFile;
extern char *ProjectFileName;
extern char ProjBuf[PBSIZE];

extern char *SystemFileName;
extern DCirfile *SysIni;

extern DCirfile *IniFile;
extern char *WrapPath;

extern char *ProjectsIni;

extern DCnlist SharedSrcDir;
extern DCnlist SharedSrcMap;
extern DCnlist SharedSrcID;

char *Unescape(char *chp);


extern char *ComSpec;
extern unl ExitType;
extern unl LocalExitCode;
extern char PathBuf[MAXPATH];
extern bool StopCommand;
extern HANDLE CurrProc;
extern LPSTARTUPINFO startInfo;
extern PROCESS_INFORMATION *procInfo;


void RunCommand(const char *command, const char *dir = NULL,
	 bool hide = true, bool synch = true);

void WaitForCurrProcess(void);

void RunDOSCommand(const char *cmd, const char *dir = NULL,
	 bool hide = true, bool keep = false, bool synch = false);

void LogResults(char *cmd);

extern char *LogFileName;
extern DClog *Log;
extern bool UseLog;
extern unc LogLevels[6];


void LogEvent(logtype lt, unc sev, const char *desc,
 const char *d2, const char *d3, const char *d4, const char *d5);

void SetPMFlag(void);
bool CheckLogErrors(void);

void OpenLog();
void CloseLog();

#endif
	// _D2GUTIL_H_
