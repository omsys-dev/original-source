// dcauto.cpp contains code for outout file automation, JHG
// Copyright (c) 2009 by Omni Systems, Inc.  All rights reserved.

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
#include "dcini.h"
#include "dcmacr.h"
#include "dcauto.h"

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


// public statics

outtyp DCauto::OutType = otnone;
logfunc DCauto::LogEventFunc = NULL;
bool DCauto::XMLSource = false;
bool DCauto::OnlyAuto = false;
unc DCauto::IndexNavType = 0;

bool DCauto::CopyOriginalGraphics = false;
DCnlist *DCauto::OriginalGraphics = NULL;
DCnlist *DCauto::OriginalFixedGraphics = NULL;
long DCauto::OriginalGraphicsCount = 0;


// protected statics

char *DCauto::CurrDir = NULL;
int DCauto::CurrDrive = 0;
bool DCauto::UseExistingDCL = false;
bool DCauto::DeleteExistingDCL = false;
bool DCauto::MakeHLP = false;
char *DCauto::GraphPath = NULL;
char *DCauto::GraphSubdir = "graphics";
char *DCauto::HTMLSubdir = "html";
char *DCauto::OHVName = NULL;
char *DCauto::CopyGraphicsFrom = NULL;
char *DCauto::CopyCSSFrom = NULL;
char *DCauto::CopyBeforeFrom = NULL;
char *DCauto::CopyAfterFrom = NULL;
bool DCauto::KeepCompileWindow = false;
int DCauto::ExitCode = 0;
int DCauto::ExitType = 0;
char *DCauto::ComSpec = NULL;
char *DCauto::Compiler = NULL;

char *DCauto::ProjectName = NULL;
char *DCauto::PrjPath = NULL;
char *DCauto::FTSCommand = NULL;
char *DCauto::JarCommand = NULL;
char *DCauto::ArchiveCommand = NULL;
char *DCauto::ArchiveStartParams = NULL;
char *DCauto::ArchiveEndParams = NULL;
char *DCauto::ArchiveFrom = NULL;

char *DCauto::ArchiveName = NULL;
char *DCauto::ArchiveVer = NULL;
char *DCauto::ArchiveExt = NULL;

char *DCauto::WrapPath = ".\\_wrap";
char *DCauto::ShipPath = "..\\..\\_ship";
char *DCauto::CSSPath = NULL;
char *DCauto::WrapCopyFiles = NULL;
char *DCauto::GraphCopyFiles = NULL;
char *DCauto::CSSCopyFiles = NULL;
char *DCauto::CopyBeforeFiles = NULL;
char *DCauto::CopyAfterFiles = NULL;
char *DCauto::JavaRootFiles = NULL;
char *DCauto::ZipCommand = NULL;
char *DCauto::ZipParams = NULL;
char *DCauto::OHVFiles = NULL;
bool DCauto::OHProjFileXhtml = false;
char *DCauto::EmptyOutputFiles = NULL;

char *DCauto::SystemStartCommand = NULL;
char *DCauto::SystemWrapCommand = NULL;
char *DCauto::SystemEndCommand = NULL;
char *DCauto::SystemCommandWindow = NULL;
char *DCauto::BatFileName = "Mif2GoSystemCommand.bat";
char *DCauto::AskForUserVars = NULL;


int DCauto::EmptyOutputDir = 0;
char *DCauto::EmptyOutputDirList[] = {
 "Never", "File", "Book",
 NULL };
#define EOD_NEVER 1
#define EOD_FILE  2
#define EOD_BOOK  3
#define EOD_ALWAYS 4


bool DCauto::WrapAndShip = false;
bool DCauto::InWrapShip = false;
bool DCauto::EmptyWrapPath = true;
bool DCauto::EmptyGraphPath = false;
bool DCauto::EmptyJavaHTMLSubdir = true;
bool DCauto::EmptyJavaGraphSubdir = false;
bool DCauto::LogAuto = false;
bool DCauto::MakeFts = true;
bool DCauto::MakeJar = true;
bool DCauto::MakeArchive = true;
bool DCauto::MoveArchive = false;
bool DCauto::EclipseUseManifest = true;
bool DCauto::FilterConditions = false;


char *
DCauto::BaseFileName(char *fname)
{
	char *basename = NewName(strlen(fname));
	GetBaseName(fname, NULL, basename);
	return basename;
}


char *
DCauto::PathName(const char *fname)
{
	char *pathname = NewName(fname);
	char *sep = strrchr(pathname, '\\');
	if (!sep)
		sep = strrchr(pathname, '/');
	if (sep) {
		*sep = '\0';
		return pathname;
	}
	else {
		DeleteName(pathname);
		return "";
	}
}


char *
DCauto::AddFileExt(char *filename, char *newext)
{
	char *extension = NULL;
	char *newfile = NULL;
	int len = 0;

	len = strlen(filename) + 1;

	newfile = NewName(len + strlen(newext));
	strncpy(newfile, filename, len);
	strcat(newfile, newext);
	return newfile;
}


char *
DCauto::NewFileExt(char *filename, char *newext)
{
	char *extension = NULL;
	char *newfile = NULL;
	int len = 0;

	if ((extension = strrchr(filename, '.')) != NULL)
		len = (extension - filename) + 1;
	else
		len = strlen(filename) + 1;

	newfile = NewName(len + strlen(newext) + 1);
	strncpy(newfile, filename, len);
	if (newfile[len - 1] != '.')
		strcat(newfile, ".");
	if (*newext == '.')
		newext++;
	strcat(newfile, newext);
	return newfile;
}


void 
DCauto::DeleteDirContents(char *cd)
{
	char *command = NewName(strlen(cd) + 10);
	sprintf(command, "del \"%s\" /q", cd);
	RunDOSCommand(command, true, false);
	DeleteName(command);
}


void 
DCauto::DeleteOneFile(char *fname)
{
	char *command = NewName(strlen(fname) + 10);
	sprintf(command, "del \"%s\" /q", fname);
	RunDOSCommand(command, true, false);
	DeleteName(command);
}


char *
DCauto::MakeFileName(char *pathname, char *filename)
{
	char *name = NULL;
	unc ch = 0;

	name = NewName(pathname, strlen(pathname) + strlen(filename) + 2);
	ch = pathname[strlen(pathname) - 1]; 
	if ((ch != '\\')
	 && (ch != '/')
	 && (*filename != '\\')
	 && (*filename != '/'))
		strcat(name, "\\");
 	strcat(name, filename);

	return name;
}



char *
DCauto::AddSlash(char *newpath)
{
	char *tmpstr = NULL;
	int plen = 0;

	if (!newpath)
		return NULL;

	plen = strlen(newpath);
	if ((newpath[plen - 1] != '/')
	 && (newpath[plen - 1] != '\\')) {
		tmpstr = NewName(plen + 2);
		strcpy(tmpstr, newpath);
		tmpstr[plen] = strchr(newpath, '/') ? '/' : '\\';
		//DeleteName(newpath);
		return tmpstr;
	}
	return newpath;
}


void 
DCauto::RemoveSlash(char *path)
{
	int plen = 0;

	plen = strlen(path);
	if ((path[plen - 1] == '/')
	 || (path[plen - 1] == '\\'))
		path[plen - 1] = '\0'; 
}


char *
DCauto::Unquote(char *newpath)
{
	char *tmpstr = NULL;
	int plen = 0;
	int i = 0;

	plen = strlen(newpath);
	for (i = 0; i < plen; i++) {
		if (newpath[i] == '/')
			newpath[i] = '\\';
	}
	if ((newpath[0] == '"')
	 && (newpath[plen - 1] == '"')) {
		tmpstr = NewName(plen - 1);
		strncpy(tmpstr, newpath + 1, plen - 1);
		plen = strlen(tmpstr);
		if (tmpstr[plen - 1] == '"')
			tmpstr[plen - 1] = '\0';
		return tmpstr;
	}
	return newpath;
}


void 
DCauto::MakeDir(char *cd)
{
	char *command = NewName(strlen(cd) + 9);
	sprintf(command, "mkdir \"%s\"", cd);
	RunDOSCommand(command, true, false);
	DeleteName(command);
}


void 
DCauto::RemoveDir(char *cd)
{
	char *command = NewName(strlen(cd) + 15);
	sprintf(command, "rmdir /S /Q \"%s\"", cd);
	RunDOSCommand(command, true, false);
	DeleteName(command);
}


void 
DCauto::RunDOSCommand(char *cmd, bool hide, bool keep, bool synch)
{
	char *command = NULL;

	if (!cmd
	 || (*cmd == '\0'))
		return;

	if (ComSpec == NULL) {
		ComSpec = (char *) getenv("comspec");
		if (ComSpec == NULL)
			strcpy(ComSpec, "command");
	}

	command = NewName(strlen(ComSpec) + strlen(cmd) + 5);
	strcpy(command, ComSpec);
	strcat(command, keep ? " /k " : " /c ");
	strcat(command, cmd);

	if (LogAuto) {
		StartLogAuto();
		LogAutoEvent(cmd);
		char *alog = NewName(strlen(AutoLogFile) + 10);
		sprintf(alog, " > %s%03d.tmp", AutoLogFile, ++AutoLogNum);
		LogAutoEvent(alog);
		char *xcmd = NewName(command, strlen(command) + strlen(alog));
		strcat(xcmd, alog);
		DeleteName(alog);
		DeleteName(command);
		command = xcmd;
	}

	RunCommand(command, CurrDir, synch, hide, false);

	DeleteName(command);
}


char *DCauto::LogFileName = NULL;
char *DCauto::AutoLogFile = NULL;
long DCauto::AutoLogNum = 0;
bool DCauto::LogAutoStarted = false;
char *DCauto::AutoLogFileTmp = NULL;
DClog *DCauto::AutoLog = NULL;


void
DCauto::StartLogAuto(void)
{
	if (LogAutoStarted)
		return;

	LogAutoStarted = true;
	if (PathIsRelative(LogFileName)
	 && CurrDir)
		LogFileName = MakeFileName(CurrDir, LogFileName);
		
	AutoLogFile = NewName(LogFileName);
	char *per = strchr(AutoLogFile, '.');
	if (per)
		*per = '\0';

	AutoLogFileTmp = NewName(AutoLogFile, strlen(AutoLogFile) + 8);
	strcat(AutoLogFileTmp, "Auto.tmp");

	// redirect LogEvent to a .tmp file
	AutoLog = new DClog(AutoLogFileTmp, NULL, NULL, NULL);

	AutoLogNum = 0;
}


void 
DCauto::LogAutoEvent(char *txt)
{
	LogEvent(loginfo, 1, txt);
}


void
DCauto::EndLogAuto(void)
{
	// direct LogEvent back to original LogEventFunc
	delete AutoLog;
	AutoLog = NULL;
	LogAuto = false;
	LogAutoStarted = false;

	// merge tmp log and command logs at end of regular log
	char lbuf[1024];
	DCrfile *alog = new DCrfile(AutoLogFileTmp);
	DCrfile *tlog = NULL;
	while ((alog->getLine(lbuf, 1024)) == true) {
		long len = strlen(lbuf) - 1;
		while (len
		 && ((lbuf[len] == '\n')
		  || (lbuf[len] == '\r')))
			lbuf[len--] = '\0';
		// if tmp log line starts "I1:  > " replace with named log
		if (!strnicmp(lbuf, "I1:  > ", 7)) {
			char *tname = NewName(lbuf + 7);
			tlog = new DCrfile(tname);
			while ((tlog->getLine(lbuf, 1024)) == true) {
				long tlen = strlen(lbuf) - 1;
				while (tlen
				 && ((lbuf[tlen] == '\n')
					|| (lbuf[tlen] == '\r')))
					lbuf[tlen--] = '\0';
				if (tlen)
					LogEvent(loginfo, 1, lbuf);
			}
			delete tlog;
			DeleteOneFile(tname);
			DeleteName(tname);
		}
		else if ((len > 5)
		 && (lbuf[2] == ':'))  // exclude old timestamps
			LogEvent(loginfo, 1, lbuf + 4);
	}
	delete alog;
	DeleteOneFile(AutoLogFileTmp);
	DeleteName(AutoLogFileTmp);
}


void 
DCauto::RunCommand(char *command, char *cd, bool synch,
	                      bool hide, bool sized)
{
#ifdef WIN32
	LPSTARTUPINFO startInfo;
	PROCESS_INFORMATION *procInfo;
	//IntT fmHigh;
	//IntT fmWide;
	//IntT fmLeft;
	//IntT fmTop;

	ExitType = 3;

	startInfo = (LPSTARTUPINFO) new char[sizeof(STARTUPINFO)];
	if (startInfo == NULL)
		return;

	//F_ClearPtr(startInfo, sizeof(STARTUPINFO));
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

	procInfo = (PROCESS_INFORMATION *) new char[sizeof(PROCESS_INFORMATION)];
	if (procInfo == NULL) {
		delete [] startInfo;
		return;
	}

	//F_ApiAlert(command, FF_ALERT_CONTINUE_NOTE);

	ExitType = 0;
	
	if (CreateProcess(NULL, command, NULL, NULL, TRUE, 0,
										NULL, cd, startInfo, procInfo)) {
		if (synch) {
			bool stop = false;
			while (!stop) {
				unl objs = 1;
				unl res = 0;
				HANDLE proc[2];
				proc[0] = procInfo->hProcess;
				proc[1] = 0;

				res = MsgWaitForMultipleObjects(objs, proc, false,
				                                INFINITE, QS_ALLINPUT);
				if (res == WAIT_OBJECT_0)
					break;
				if (res == WAIT_FAILED) {
					ExitType = 1;
					ExitCode = GetLastError();
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
							ExitCode = 1;
							stop = true;
							break;
						}
					} // end of PeekMessage while loop 
				}
			}
			//WaitForSingleObject(procInfo->hProcess, INFINITE);
			if (!stop
			 && !GetExitCodeProcess(procInfo->hProcess, (unl *) &ExitCode)) {
				ExitType = 2;
				ExitCode = GetLastError();
			}
		}
		CloseHandle(procInfo->hProcess);
		CloseHandle(procInfo->hThread);
	}
	else {
		ExitType = 1;
		ExitCode = GetLastError();
	}

	delete [] startInfo;
	delete [] procInfo;

#else

	/* for Mac and UNIX */
	ExitType = 0;
	if ((ExitCode = system(command)) != FE_Success)
		ExitType = 4;

#endif
}



// public functions

void
DCauto::LogEvent(logtype lt, unc sev, const char *desc,
	const char *d2, const char *d3, const char *d4, const char *d5)
{
	if (!LogEventFunc)
		return;

	if (AutoLog)
		AutoLog->logEvent(lt, sev, desc, d2, d3, d4, d5);
	else
		LogEventFunc(lt, sev, desc, d2, d3, d4, d5);
}


void
DCauto::SetSystemCommands(DCirfile *ini, bool book)
{
	if (ini->Section("FDK")) {
		if (book) {
			if (ini->Find("BookStartCommand"))
				SystemStartCommand = ini->StrRight();
			if (ini->Find("BookWrapCommand"))
				SystemWrapCommand = ini->StrRight();
			if (ini->Find("BookEndCommand"))
				SystemEndCommand = ini->StrRight();
			if (ini->Find("BookCommandWindow"))
				SystemCommandWindow = ini->StrRight();
		}
		else {
			if (ini->Find("FileStartCommand"))
				SystemStartCommand = ini->StrRight();
			if (ini->Find("FileWrapCommand"))
				SystemWrapCommand = ini->StrRight();
			if (ini->Find("FileEndCommand"))
				SystemEndCommand = ini->StrRight();
			if (ini->Find("FileCommandWindow"))
				SystemCommandWindow = ini->StrRight();
		}
		if (ini->Find("AskForUserVars"))
			AskForUserVars = ini->StrRight();
	}

	if (ini->Section("BookFileCommands")) {
		if (book) {
			if (ini->Find("BookStartCommand"))
				SystemStartCommand = ini->StrRight();
			if (ini->Find("BookWrapCommand"))
				SystemWrapCommand = ini->StrRight();
			if (ini->Find("BookEndCommand"))
				SystemEndCommand = ini->StrRight();
			if (ini->Find("BookCommandWindow"))
				SystemCommandWindow = ini->StrRight();
		}
		else {
			if (ini->Find("FileStartCommand"))
				SystemStartCommand = ini->StrRight();
			if (ini->Find("FileWrapCommand"))
				SystemWrapCommand = ini->StrRight();
			if (ini->Find("FileEndCommand"))
				SystemEndCommand = ini->StrRight();
			if (ini->Find("FileCommandWindow"))
				SystemCommandWindow = ini->StrRight();
		}
		if (ini->Find("AskForUserVars"))
			AskForUserVars = ini->StrRight();
	}

	if (ini->Section("Automation")) {
		if (book) {
			if (ini->Find("BookStartCommand"))
				SystemStartCommand = ini->StrRight();
			if (ini->Find("BookWrapCommand"))
				SystemWrapCommand = ini->StrRight();
			if (ini->Find("BookEndCommand"))
				SystemEndCommand = ini->StrRight();
			if (ini->Find("BookCommandWindow"))
				SystemCommandWindow = ini->StrRight();
		}
		else {
			if (ini->Find("FileStartCommand"))
				SystemStartCommand = ini->StrRight();
			if (ini->Find("FileWrapCommand"))
				SystemWrapCommand = ini->StrRight();
			if (ini->Find("FileEndCommand"))
				SystemEndCommand = ini->StrRight();
			if (ini->Find("FileCommandWindow"))
				SystemCommandWindow = ini->StrRight();
		}
		if (ini->Find("SystemStartCommand"))
			SystemStartCommand = ini->StrRight();
		if (ini->Find("SystemWrapCommand"))
			SystemWrapCommand = ini->StrRight();
		if (ini->Find("SystemEndCommand"))
			SystemEndCommand = ini->StrRight();
		if (ini->Find("SystemCommandWindow"))
			SystemCommandWindow = ini->StrRight();
		if (ini->Find("AskForUserVars"))
			AskForUserVars = ini->StrRight();
	}
}


bool
DCauto::SetUpAutomation(outtyp outtype, DCirfile *ini,
	                      char *csspath, char *graphpath, bool book)
{
	IniFile = ini;

	CurrDrive = _getdrive();
	CurrDir = _getcwd(NewName(1024), 1024);
	OutType = outtype;
	CSSPath = csspath;
	GraphPath = graphpath;

	if (ini->Section("Setup")
	 || ini->Section("FDK")) {
		if (ini->Find("UseExistingDCL"))
			UseExistingDCL = ini->BoolRight();
		if (ini->Find("DeleteExistingDCL"))
			DeleteExistingDCL = ini->BoolRight();
		if (ini->Find("PrjFileName"))
			PrjPath = ini->PathRight();
	}

	if (ini->Section("Graphics")) {
		if (ini->Find("GraphPath"))
			GraphPath = ini->StrRight();
	}
	if (ini->Section("FilterConditions")) {
		if (ini->Find("FilterConditions"))
			FilterConditions = ini->BoolRight();
	}

	SetSystemCommands(ini, book);

	if (!ini->Section("Automation"))
		return false;

	if (ini->Find("WrapAndShip"))
		WrapAndShip = ini->BoolRight();
	if (ini->Find("EmptyWrapPath"))
		EmptyWrapPath = ini->BoolRight();
	if (ini->Find("EmptyGraphPath"))
		EmptyGraphPath = ini->BoolRight();
	if (ini->Find("LogAuto"))
		LogAuto = ini->BoolRight();
	if (ini->Find("OnlyAuto"))
		OnlyAuto = ini->BoolRight();
	if (ini->Find("MakeFTS"))
		MakeFts = ini->BoolRight();
	if (ini->Find("MakeJar"))
		MakeJar = ini->BoolRight();
	if (ini->Find("MakeArchive"))
		MakeArchive = ini->BoolRight();
	if (ini->Find("MoveArchive"))
		MoveArchive = ini->BoolRight();
	if (ini->Find("CompileHelp"))
		MakeHLP = ini->BoolRight();
	if (ini->Find("KeepCompileWindow"))
		KeepCompileWindow = ini->BoolRight();

	if (ini->Find("ArchiveCommand"))
		ArchiveCommand = ini->StrRight();
	if (ini->Find("ArchiveStartParams"))
		ArchiveStartParams = ini->StrRight();
	if (ini->Find("ArchiveEndParams"))
		ArchiveEndParams = ini->StrRight();
	if (ini->Find("ArchiveFrom"))
		ArchiveFrom = ini->StrRight();
	if (ini->Find("ArchiveName"))
		ArchiveName = ini->StrRight();
	if (ini->Find("ArchiveVer"))
		ArchiveVer = ini->StrRight();
	if (ini->Find("ArchiveExt"))
		ArchiveExt = ini->StrRight();

	if (ini->Find("WrapPath"))
		WrapPath = ini->StrRight();
	if (ini->Find("ShipPath"))
		ShipPath = ini->StrRight();
	if (ini->Find("WrapCopyFiles"))
		WrapCopyFiles = ini->StrRight();
	if (ini->Find("GraphCopyFiles"))
		GraphCopyFiles = ini->StrRight();
	if (ini->Find("CopyGraphicsFrom"))
		CopyGraphicsFrom = ini->StrRight();
	if (ini->Find("CopyOriginalGraphics"))
		CopyOriginalGraphics = ini->BoolRight();
	if (ini->Find("CSSCopyFiles"))
		CSSCopyFiles = ini->StrRight();
	if (ini->Find("CopyCSSFrom"))
		CopyCSSFrom = ini->StrRight();
	if (ini->Find("EmptyOutputFiles"))
		EmptyOutputFiles = ini->StrRight();

	if (ini->Find("CopyBeforeFiles"))
		CopyBeforeFiles = ini->StrRight();
	if (ini->Find("CopyBeforeFrom"))
		CopyBeforeFrom = ini->StrRight();
	if (ini->Find("CopyAfterFiles"))
		CopyAfterFiles = ini->StrRight();
	if (ini->Find("CopyAfterFrom"))
		CopyAfterFrom = ini->StrRight();

	if (ini->Find("EmptyOutputDir")) {
		char *eodir = ini->StrRight();
		if (!stricmp(eodir, "never"))
			EmptyOutputDir = EOD_NEVER;
		else if (!stricmp(eodir, "file"))
			EmptyOutputDir = EOD_FILE;
		else if (!stricmp(eodir, "book"))
			EmptyOutputDir = EOD_BOOK;
		else if (!stricmp(eodir, "always"))
			EmptyOutputDir = EOD_ALWAYS;
		DeleteName(eodir);
	}

	// in help options
	char *sect = NULL;
	switch (OutType) {
		case otwh:
			sect = "HelpOptions";
			break;

		case oteh:
			sect = "EclipseHelpOptions";
			break;

		case othh:
			sect = "MSHtmlHelpOptions";
			break;

		case otjh:
			sect = "JavaHelpOptions";
			break;

		case otohj:
			sect = "OracleHelpOptions";
			break;

		case otomh:
			sect = "OmniHelpOptions";
			break;

		default:
			return WrapAndShip;
	}

	if (!ini->Section(sect))
		return WrapAndShip;

	if (OutType == otwh) {
		if (ini->Find("HPJFileName"))
			ProjectName = ini->StrRight();
		if (ini->Find("Compiler"))
			Compiler = ini->StrRight();
	}

	if ((OutType == otjh)
	 || (OutType == otohj)) {
		if (ini->Find("HSFileName"))
			ProjectName = ini->StrRight();
		if (ini->Find("HTMLSubdir"))
			HTMLSubdir = ini->StrRight();
		if (ini->Find("EmptyJavaHTMLSubdir"))
			EmptyJavaHTMLSubdir = ini->BoolRight();
		if (ini->Find("GraphSubdir"))
			GraphSubdir = ini->StrRight();
		if (ini->Find("EmptyJavaGraphSubdir"))
			EmptyJavaGraphSubdir = ini->BoolRight();
		if (ini->Find("FTSCommand"))
			FTSCommand = ini->StrRight();
		if (ini->Find("JarCommand"))
			JarCommand = ini->StrRight();
		if (ini->Find("JavaRootFiles"))
			JavaRootFiles = ini->StrRight();
	}

	if (OutType == oteh) {
		if (ini->Find("UseManifest"))
			EclipseUseManifest = ini->BoolRight();
		if (ini->Find("ZipCommand"))
			ZipCommand = ini->StrRight();
		if (ini->Find("ZipParams"))
			ZipParams = ini->StrRight();
	}
	
	if (OutType == otomh) {
		if (ini->Find("ProjectName"))
			ProjectName = ini->StrRight();
		if (ini->Find("OHProjFileXhtml"))
			OHProjFileXhtml = ini->BoolRight();
		if (ini->Find("OHVFiles"))
			OHVFiles = ini->StrRight();
		if (ini->Find("OHViewPath"))
			OHVName = ini->StrRight();
		else if (OHProjFileXhtml)
			OHVName = "%omsyshome%\\common\\system\\omnihelp\\ohvxml";
		else
			OHVName = "%omsyshome%\\common\\system\\omnihelp\\ohvhtm";
	}

	if (OutType == othh) {
		if (ini->Find("HHPFileName"))
			ProjectName = ini->StrRight();
		if (ini->Find("Compiler"))
			Compiler = ini->StrRight();
	}

	return WrapAndShip;
}


void 
DCauto::DeleteDCL(void)
{
	if (CurrDrive)
		_chdrive(CurrDrive);
	_chdir(CurrDir);

	// DeleteExistingDCL process
	if (DeleteExistingDCL
	 && !UseExistingDCL)
		DelCompFiles("*.dcl\0*.dcb\0\0");
}


DCirfile *DCauto::IniFile = NULL;
char *DCauto::MacroFile = "m2hmacro.ini";


bool
DCauto::WritePredefinedMacro(DCwfile *df, char *name)
{

	// basename, prjpath, and currpath
	if (!stricmp(name, "basename"))
		df->putStr(PrjPath ? BaseFileName(PrjPath) : ProjectName);
	else if (!stricmp(name, "prjpath"))
		df->putStr(PrjPath ? PathName(PrjPath) : "");
	else if (!stricmp(name, "currpath"))
		df->putStr(CurrDir);
	else if (!stricmp(name, "macroparam"))
		df->putStr(DCmacr::MacroParam);

	else
		return false;

	return true;
}


char *
DCauto::GetPredefinedVar(DCwfile *df, char *vp)
{
	char *ret = NULL;
	char *txt = NULL;

	if (!stricmp(vp, "basename"))
		ret = PrjPath ? BaseFileName(PrjPath) : ProjectName;
	else if (!stricmp(vp, "prjpath"))
		ret = PrjPath ? PathName(PrjPath) : "";
	else if (!stricmp(vp, "currpath"))
		ret = CurrDir;

	if (ret)
		return NewName(ret);

	return NULL;
}



void
DCauto::ExecSystemCommand(const char *command, const char *name)
{
	bool hide = true;
	bool keep = false;

	if (!command
	 || (*command == '\0'))
		return;

	if (LogAuto)
		LogEvent(loginfo, 1, "Running system ", name, " command:", command);

	if (SystemCommandWindow) {  // Hide, Show, Keep
		hide = !stricmp(SystemCommandWindow, "Hide");
		keep = !stricmp(SystemCommandWindow, "Keep");
	}

	// create and run bat file ****
	char *bat = NewName("System", strlen(name) + 18);
	strcat(bat, name);
	strcat(bat, "Command.bat");

	DCwfile *bf = new DCwfile(bat, ftxt);

	// init macro module for system commands
	if (bf->FErr() != fok) {
		if (LogAuto)
			LogEvent(logwarn, 1, "System ", name, " command failed, cannot write ", bat);
		return;
	}

	DCmacr::InitMacroModule(bf, IniFile, MacroFile, NULL,
	    WritePredefinedMacro, NULL, NULL, NULL, GetPredefinedVar);

	bf->putStr("REM System ");
	bf->putStr(name);
	bf->putStr(" command batch file");
	bf->putCr();

	DCmacr::WriteMacro(bf, (char *) command);
	bf->CloseFile();

	RunDOSCommand(bat, hide, keep);
}


void 
DCauto::WrapShipPreProc(bool isBook)
{
	bool getvars = false;

	if (CurrDrive)
		_chdrive(CurrDrive);
	_chdir(CurrDir);

	//ShowStatus(docId, filename, StatClean);

	// ask for user vars
	if (AskForUserVars)  // Never, Always, Once
		getvars = !strcmp(AskForUserVars, "Always")
		       || !strcmp(AskForUserVars, "Once");
	// get and set user var vakues ****


	ExecSystemCommand(SystemStartCommand, "Start");

	// EmptyOutputDir process
	if (EmptyOutputDir == (isBook ? EOD_BOOK : EOD_FILE))
		DoEmptyOutputProc();

	// CopyBefore process
	if (CopyBeforeFiles
	 && CopyBeforeFrom) {
		CopyBeforeFrom = AddSlash(Unquote(NewName(CopyBeforeFrom)));
		char *cbfiles = MakeFileList(CopyBeforeFiles);
		CopyCompFilesSrc(cbfiles, CurrDir, CopyBeforeFrom);
	}
	if (LogAutoStarted)
		EndLogAuto();
}


void  
DCauto::CopyOriginalGraphicsProc(DCnlist *gpaths, DCnlist *gfixpaths, 
	                               long gcount, char *gdir)
{
	gdir = SetUpGraphDir(gdir);

	for (long i = 1; i <= gcount; i++) {
		char *cpt = gpaths->find(i);
		char *fpt = gfixpaths->find(i);
		CopyOneOrigGraphic(cpt, fpt, gdir);
	}
	if (!InWrapShip
	 && LogAutoStarted)
		EndLogAuto();
}


void  
DCauto::CopyOriginalGraphicsEndAuto(char *gdir)
{
	DCirfile *gf = new DCirfile("EndAuto.grc");
	if ((gf->FErr() != fok)
	 || !gf->Section("GraphCopyFiles")) {
		delete gf;
		return;
	}

	gdir = SetUpGraphDir(gdir);

	while (gf->NextItem()) {
		char *cpt = gf->StrLeft();
		char *fpt = gf->StrRight();
		CopyOneOrigGraphic(cpt, (*fpt ? fpt : NULL), gdir);
	}

	delete gf;
	_unlink("EndAuto.grc");

}


char *
DCauto::SetUpGraphDir(char *gdir)
{
	EmptyWrapPath = false;
	if (CurrDrive)
		_chdrive(CurrDrive);
	_chdir(CurrDir);

	long glen = 0;
	if (gdir) {
		glen = strlen(gdir) + 3;
		if (!InWrapShip) {
			for (char *slash = gdir; *slash; slash++) {
				if (*slash == '/')
					*slash = '\\';
			}
			if (WrapPath && *WrapPath) {
				glen += (strlen(WrapPath) + 2);
				char *gtmp = NewName(WrapPath, glen);
				strcat(gtmp, "\\");
				strcat(gtmp, gdir);
				gdir = gtmp;
			}
			MakeDir(gdir);
		}	
	}
	else if (!InWrapShip
	 && (WrapPath && *WrapPath)) {
		glen = strlen(WrapPath) + 3;
		gdir = WrapPath;
	}
	return gdir;
}


void  
DCauto::CopyOneOrigGraphic(char *cpt, char *fpt, char *gdir)
{
	long len = strlen(cpt) + 16;
	if (fpt) {
		len += strlen(fpt) + 5;
		if (WrapPath
		 && *WrapPath)
			len += strlen(WrapPath) + 5;
	}
	if (gdir)
		len += strlen(gdir) + 3;
	char *cmd = NewName("copy /y /b \"", len);
	for (char *slash = cpt; *slash; slash++) {
		if (*slash == '/')
			*slash = '\\';
	}
	strcat(cmd, cpt);
	strcat(cmd, "\"");

	if (fpt) {  // always includes graphdir
		if (WrapPath
		 && *WrapPath) {
			strcat(cmd, " \"");
			strcat(cmd, WrapPath);
			strcat(cmd, "\\");
		}
		else
			strcat(cmd, " \".\\");
		for (char *slash = fpt; *slash; slash++) {
			if (*slash == '/')
				*slash = '\\';
		}
		strcat(cmd, fpt);
		strcat(cmd, "\"");
	}
	else if (gdir) {
		strcat(cmd, " \"");
		strcat(cmd, gdir);
		strcat(cmd, "\"");
	}
	else
		strcat(cmd, " .");

	if (fpt)
		LogEvent(loginfo, 1, "Copying graphic from \"", cpt, "\" to ", fpt);
	else
		LogEvent(loginfo, 1, "Copying graphic from \"", cpt, "\"");
	RunDOSCommand(cmd);
}


void  
DCauto::DoEmptyOutputProc()
{
	char *odfiles = NULL;

	if (EmptyOutputFiles
	 && *EmptyOutputFiles) {
		_strlwr(EmptyOutputFiles);
		if (strstr(EmptyOutputFiles, ".dcl")
		 || strstr(EmptyOutputFiles, ".dcb")
		 || strstr(EmptyOutputFiles, ".dc?")
		 || strstr(EmptyOutputFiles, ".dc*")
		 || strstr(EmptyOutputFiles, ".d??")
		 || strstr(EmptyOutputFiles, ".d*")
		 || strstr(EmptyOutputFiles, ".*")
		 || strstr(EmptyOutputFiles, ".?cl")
		 || strstr(EmptyOutputFiles, ".?cb")
		 || strstr(EmptyOutputFiles, ".?c?")
		 || strstr(EmptyOutputFiles, ".?c*")
		 || strstr(EmptyOutputFiles, ".d?l")
		 || strstr(EmptyOutputFiles, ".d?b")) {
			LogEvent(logwarn, 1, "Illegal file extension in EmptyOutputFiles: ",
													 "matches dcl or dcb");
			return;
		}
		odfiles = MakeFileList(EmptyOutputFiles);
	}
	else switch (OutType) {
		case otwh:  // WinHelp 
		case otrtf:
			if (!odfiles)
				odfiles = "*.rtf\0*.grx\0\0";
			break;

		case otjh:    // JavaHelp
		case otohj:   // OracleHelp
		case othh:    // MS HTML Help
		case otomh:   // OmniHelp
		case oteh:    // EclipseHelp
		case othtm:
		case otxhtm:
			if (!odfiles)
				odfiles = "*.htm\0*.html\0*.ref\0*.grx\0\0";
			break;

		case otxml:
			if (!odfiles)
				odfiles = "*.xml\0*.ref\0*.grx\0\0";
			break;

		case otdita:
			if (!odfiles)
				odfiles = "*.dita\0*.ditamap\0*.bookmap\0*.ref\0*.grx\0\0";
			break;

		case otdbk:
			if (!odfiles)
				odfiles = "*.xml\0*.ent\0*.ref\0*.grx\0\0";
			break;

		case otmif:
		case otdcl:
		default:
			break;
	}

	if (odfiles) {
		if (LogAuto) {
			LogEvent(loginfo, 1, "Cleaning directories...");
			LogEvent(loginfo, 1, "cd ", CurrDir);
		}
		DelCompFiles(odfiles);
	}
}


// move all required files into the compile dir, then compile

bool DCauto::EndingAuto = false;


void 
DCauto::WrapShipProc(char *bookname)
{
	char *filename = ProjectName;
	char *command = NULL;
	char *compiler = NULL;
	char *suffix = NULL;
	//char *bookname = NULL;
	char *bkbasename = NULL;
	char *compdir = NULL;
	//char *wrapdir = NULL;
	char *tempcurrdir = NULL;
	char *graphdir = NULL;
	char *htmdir = NULL;
	char *cssdir = NULL;
	char *tempdir = NULL;
	char *tempname = NULL;
	char *ixfilename = NULL;
	char *wcfiles = NULL;
	char *gcfiles = NULL;
	char *ccfiles = NULL;
	char *jrfiles = NULL;
	char *ohfiles = NULL;
	unc pbuf[_MAX_PATH]; 
	int nlen = 0;
	bool usecss = false;
	bool shippable = false;
	bool archived = false;

	InWrapShip = true;

	if (!strcmp(bookname, "EndAuto.dcl"))
		EndingAuto = true;

	// ensure correct starting dir
	if (CurrDrive)
		_chdrive(CurrDrive);
	_chdir(CurrDir);

	if (WrapCopyFiles && *WrapCopyFiles)
		wcfiles = MakeFileList(WrapCopyFiles);
	if (GraphCopyFiles && *GraphCopyFiles)
		gcfiles = MakeFileList(GraphCopyFiles);
	if (CSSCopyFiles && *CSSCopyFiles)
		ccfiles = MakeFileList(CSSCopyFiles);

	switch (OutType) {
		case otwh:  // WinHelp 4
			suffix = ".hpj";
			// *.rtf, *.bmp, *.wmf, *.shg, *.cnt, *.hpj, *.h
			if (!wcfiles)
				wcfiles = "*.rtf\0*.cnt\0*.hpj\0*.h\0\0";
			if (!gcfiles)
				gcfiles = "*.bmp\0*.wmf\0*.shg\0\0";
			if (MakeHLP)
				compiler = Compiler ? Compiler : "hcw /c /e";
			break;
		case otrtf:
			// *.rtf, *.bmp, *.wmf
			if (!wcfiles)
				wcfiles = "*.rtf\0\0";
			if (!gcfiles)
				gcfiles = "*.bmp\0*.wmf\0\0";
			break;
		case otjh:  // JavaHelp
		case otohj:   // OracleHelp
			suffix = ".hs";
			// *.htm, *.js, *.xml, *.hs, *.jhm, *.gif, *.jpg, *.png, *.svg
			if (!wcfiles)
				wcfiles = "*.htm\0*.js\0\0";
			if (!gcfiles)
				gcfiles = "*.gif\0*.jpg\0*.png\0*.svg\0\0";
			if (JavaRootFiles && *JavaRootFiles)
				jrfiles = MakeFileList(JavaRootFiles);
			if (!jrfiles)
				jrfiles = "*.hs\0*.jhm\0*.xml\0\0";
			usecss = true;
			break;
		case oteh:   // EclipseHelp
			suffix = ".xml";
			// *.htm, *.js, *.xml, *.MF, *.gif, *.jpg, *.png, *.svg
			if (!wcfiles)
				wcfiles = "*.htm\0*.js\0*.xml\0*.MF\0\0";
			if (!gcfiles)
				gcfiles = "*.gif\0*.jpg\0*.png\0*.svg\0\0";
			jrfiles = "doc.zip\0*.xml\0\0";
			usecss = true;
			break;
		case othh:  // MS HTML Help
			//filename = NULL;
			suffix = ".hhp";
			// *.htm, *.js, *.hh?, *.h, *.gif, *.jpg, *.png, *.svg
			if (!wcfiles)
				wcfiles = "*.htm\0*.js\0*.hh?\0*.h\0\0";
			if (!gcfiles)
				gcfiles = "*.gif\0*.jpg\0*.png\0*.svg\0\0";
			if (MakeHLP)
				compiler = Compiler ? Compiler : "hhc";
			usecss = true;
			break;
		case otomh:  // OmniHelp
			// *.htm, *.js, *.gif, *.jpg, *.png, *.svg
			if (!wcfiles)
				wcfiles = "*.htm\0*.js\0\0";
			if (!gcfiles)
				gcfiles = "*.gif\0*.jpg\0*.png\0*.svg\0\0";
			if (OHVFiles && *OHVFiles)
				ohfiles = MakeFileList(OHVFiles);
			if (!ohfiles)
				ohfiles = OHProjFileXhtml ? "ox*.*\0oh*.*\0\0" : "oh*.*\0\0";
			usecss = true;
			break;
		case othtm:
		case otxhtm:
		case otxml:
			// *.htm, *.html, *.xhtml, *.xhtm, *.xml, *.js, *.dtd, *.mod, *.ent, and *.xsd
			// *.gif, *.jpg, *.png, *.svg
			if (!wcfiles)
				wcfiles = "*.htm\0*.html\0*.xhtml\0*.xhtm\0*.xml\0*.js\0*.dtd\0*.mod\0*.ent\0*.xsd\0\0";
			if (!gcfiles)
				gcfiles = "*.gif\0*.jpg\0*.png\0*.svg\0\0";
			usecss = true;
			break;
		case otdita:
			// *.dita, *.ditamap, *.bookmap, *.dtd, *.mod, *.ent, and *.xsd
			// *.gif, *.jpg, *.png, *.svg
			if (!wcfiles)
				wcfiles = "*.dita\0*.ditamap\0*.bookmap\0*.dtd\0*.mod\0*.ent\0*.xsd\0\0";
			if (!gcfiles)
				gcfiles = "*.gif\0*.jpg\0*.png\0*.svg\0\0";
			usecss = true;
			break;
		case otdbk:
			// *.xml, *.ent
			// *.gif, *.jpg, *.png, *.svg
			if (!wcfiles)
				wcfiles = "*.xml\0*.ent\0\0";
			if (!gcfiles)
				gcfiles = "*.gif\0*.jpg\0*.png\0*.svg\0\0";
			usecss = true;
			break;
		case otmif:
			if (!wcfiles)
				wcfiles = "*.mif\0\0";
			if (!gcfiles)
				gcfiles = "*.bmp\0*.wmf\0*.gif\0*.jpg\0*.png\0*.svg\0*.tif\0\0";
			break;
		case otdcl:
			if (!wcfiles)
				wcfiles = "*.dcl\0\0";
			if (!gcfiles)
				gcfiles = "*.bmp\0*.wmf\0*.gif\0*.jpg\0*.png\0*.svg\0*.tif\0\0";
			break;
		default:
			break;
	}

	if (!ccfiles && usecss)
		ccfiles = "*.css\0\0";

	// if no output file specified, use bookname
	if (!filename
	 || !*filename) {
		//bookname = F_ApiGetString(FV_SessionId, docId, FP_Name);
		bkbasename = BaseFileName(bookname);
		if (!suffix)
			filename = NewName(bkbasename);
		else
			filename = AddFileExt(bkbasename, suffix);
		DeleteName(bkbasename);
	}


	// move files to WrapPath and compile
	if ((WrapAndShip
	  || compiler
	  || FTSCommand)
	 && WrapPath
	 && *WrapPath) {
		//ShowStatus(docId, filename, StatWrap);
		if (LogAuto)
			LogEvent(loginfo, 1, "Gathering files...");
		compdir = MakeDirPath(CurrDir, WrapPath);  // empty after graphdir known

		if (EmptyWrapPath
		 && stricmp(compdir, CurrDir)) {
			//if (LogAuto)
			//	LogEvent(loginfo, 1, "del ", compdir, " /q");
			DeleteDirContents(compdir);
		}

		if ((OutType == otjh)
		 || (OutType == otohj)) {  
			CopyCompFiles(jrfiles, compdir);
			graphdir = MakeDirPath(compdir, GraphSubdir, EmptyJavaGraphSubdir);
			// use [JavaHelpOptions]MapFilePrefix minus trailing / as suffix
			htmdir = MakeDirPath(compdir, HTMLSubdir, EmptyJavaHTMLSubdir);
			cssdir = htmdir;
			if (usecss)
				cssdir = MakeDirPath(htmdir, CSSPath);
		}
		else {  // normal case, non-java
			graphdir = MakeDirPath(compdir, GraphPath, EmptyGraphPath
					&& (!graphdir || stricmp(graphdir, compdir) || EmptyWrapPath));
			htmdir = compdir;
			if (usecss)
				cssdir = MakeDirPath(compdir, CSSPath);
		}

		CopyCompFiles(wcfiles, htmdir);
		//if (usecss)  // splittable types
		//	RemoveStartSplitFiles(docId, filename, htmdir);

		if ((GraphCopyFiles && *GraphCopyFiles)
		 || !(OriginalGraphics && OriginalGraphicsCount))
			CopyCompFiles(gcfiles, graphdir);

		if (XMLSource && EndingAuto)
			CopyOriginalGraphicsEndAuto(graphdir);
		else if (OriginalGraphics
		 && OriginalGraphicsCount)
			CopyOriginalGraphicsProc(OriginalGraphics, OriginalFixedGraphics, 
			                         OriginalGraphicsCount, graphdir);

		if (usecss)
			CopyCompFiles(ccfiles, cssdir);

		if ((OutType == otomh)
		 && OHVName
		 && (strlen(OHVName) > 2)) {
			OHVName = AddSlash(Unquote(OHVName));
			CopyCompFilesSrc(ohfiles, compdir, OHVName);
		}
	}

	ExecSystemCommand(SystemWrapCommand, "Wrap");

	if ((WrapAndShip
	  || compiler
	  || FTSCommand)
	 && CopyGraphicsFrom) {
		CopyGraphicsFrom = AddSlash(Unquote(NewName(CopyGraphicsFrom)));
		CopyCompFilesSrc(gcfiles, graphdir, CopyGraphicsFrom);
	}

	if ((WrapAndShip
	  || compiler
	  || FTSCommand)
	 && CopyCSSFrom
	 && usecss) {
		CopyCSSFrom = AddSlash(Unquote(NewName(CopyCSSFrom)));
		CopyCompFilesSrc(ccfiles, cssdir, CopyCSSFrom);
	}

	// copy CSS and JS for HTML generated index
	if (IndexNavType) {
		char *fname = NULL;
		char *jname = NULL;
		switch (IndexNavType) {
			case 'C':   // pure CSS
				fname = "idxcss.css";
				break;

			case 'J':   // JavaScript
				fname = "idxjs.css";
				jname = "idxjs.js";
				break;

			case 'H':   // HTML for fully exposed lists
			default:
				fname = "idxhtm.css";
				break;
		}
		long clen = strlen(fname);
		long jlen = jname ? strlen(jname) : 0;
		char *idxfiles = NewName(clen + jlen + 3);
		strcpy(idxfiles, fname);
		if (jname) {
			strcpy(idxfiles + clen + 1, jname);
			clen += jlen + 1;
		}
		idxfiles[clen + 1] = '\0';
		// copy from %omsyshome%\common\local\htmlidx if present,
		// else from %omsyshome%\common\system\htmlidx
		CopyCompFilesSrc(idxfiles, compdir, "%omsyshome%\\common\\system\\htmlidx\\");
		CopyCompFilesSrc(idxfiles, compdir, "%omsyshome%\\common\\local\\htmlidx\\");
	}

	if (FilterConditions) {
		CopyCompFilesSrc("runcond.js", compdir, "%omsyshome%\\common\\system\\htmlcond\\");
		CopyCompFilesSrc("runcond.js", compdir, "%omsyshome%\\common\\local\\htmlcond\\");
	}

	if (WrapAndShip
	  || compiler
	  || FTSCommand) {
		tempcurrdir = CurrDir;
		CurrDir = _fullpath((char *) pbuf, compdir, _MAX_PATH);  // set for subsequent commands
		if (LogAuto)
			LogEvent(loginfo, 1, "cd ", CurrDir);
	}


	// CopyAfter process, before compile and archive
	if (CopyAfterFiles
	 && CopyAfterFrom) {
		CopyAfterFrom = AddSlash(Unquote(NewName(CopyAfterFrom)));
		char *cafiles = MakeFileList(CopyAfterFiles);
		CopyCompFilesSrc(cafiles, compdir, CopyAfterFrom);
	}


	// do the compile for WH and HH
	if (compiler) {
		//ShowStatus(docId, filename, StatCompile);
		if (LogAuto)
			LogEvent(loginfo, 1, "Compiling Help...");
		tempname = filename;  // name the project file
		command = NewName(strlen(tempname) + strlen(compiler) + 2);
		sprintf(command, "%s %s", compiler, tempname);
		//if (LogAuto)
		//	LogEvent(loginfo, 1, command);
		RunDOSCommand(command, false, KeepCompileWindow);  // always show
		DeleteName(command);
		if (ShipPath && *ShipPath) {  // do .hlp or .chm
			if (OutType == otwh)
				filename = NewFileExt(filename, ".hlp");
			else if (OutType == othh)
				filename = NewFileExt(filename, ".chm");
			shippable = true;
		}
	}


	// do the packaging for Eclipse
	if (OutType == oteh) {
		if (EclipseUseManifest) {
			if (!PathIsDirectory("META-INF")) {
				//if (LogAuto)
				//	LogEvent(loginfo, 1, "mkdir META-INF");
				MakeDir("META-INF");
			}
			command = "move /Y MANIFEST.MF META-INF";
			//if (LogAuto)
			//	LogEvent(loginfo, 1, command);
			RunDOSCommand(command, false, KeepCompileWindow);  // always show
		}
		if (ZipCommand && *ZipCommand && ZipParams && *ZipParams) { // make doc.zip
			ZipCommand = Unquote(ZipCommand);
			command = NewName(strlen(ZipCommand) + strlen(ZipParams) + 4);
			sprintf(command, "\"%s\" %s", ZipCommand, ZipParams);
			//if (LogAuto)
			//	LogEvent(loginfo, 1, command);
			RunDOSCommand(command, false, KeepCompileWindow);  // always show
			DeleteName(command);
		}
	}


	// do the FTS indexing and jarring for Java
	if ((OutType == otjh)
	 || (OutType == otohj)) {
		if (MakeFts
		 && FTSCommand
		 && *FTSCommand) {
			FTSCommand = Unquote(FTSCommand);
			if (OutType == otjh) {
				//ShowStatus(docId, filename, StatJHFTS);
				if (LogAuto)
					LogEvent(loginfo, 1, "Creating FTS files...");
				tempname = "JavaHelpSearch";
				RemoveDir(tempname);  // remove any previous version
				//if (LogAuto)
				//	LogEvent(loginfo, 1, "rmdir ", tempname, " \n");
				tempname = HTMLSubdir;    // name the dir, not the .hs, for jhindexer
			}
			else if (OutType == otohj) {  // name the dir and the index file
				//ShowStatus(docId, filename, StatOHJFTS);
				if (LogAuto)
					LogEvent(loginfo, 1, "Creating FTS file...");
				ixfilename = NewFileExt(filename, ".idx");
				//tempname = NewName(strlen(ixfilename) + strlen(HTMLSubdir) + 2);
				tempname = NewName(strlen(CurrDir) + strlen(ixfilename) + 2);
				sprintf(tempname, "%s %s", CurrDir, ixfilename);  // name the dir and the .idx
			}
			command = NewName(strlen(tempname) + strlen(FTSCommand) + 2);
			sprintf(command, "\"%s\" %s", FTSCommand, tempname);
			//if (LogAuto)
			//	LogEvent(loginfo, 1, command);
			RunDOSCommand(command, false, KeepCompileWindow);  // always show
			DeleteName(command);
		}

		if (MakeJar
		 && JarCommand
		 && *JarCommand) {
			//ShowStatus(docId, filename, StatJar);
			if (LogAuto)
				LogEvent(loginfo, 1, "Making jar file...");
			JarCommand = Unquote(JarCommand);
			filename = NewFileExt(filename, ".jar");
			// allow for cvf, *, 2 quotes, 3 spaces, and 1 nul
			command = NewName(strlen(JarCommand) + strlen(filename) + 10);
			sprintf(command, "\"%s\" cvf %s *", JarCommand, filename);
			//if (LogAuto)
			//	LogEvent(loginfo, 1, command);
			RunDOSCommand(command, true, false);
			DeleteName(command);
			shippable = true;
		}
	}

	char *tmpcurr = NULL;
	char *arcfromdir = NULL;

	// do zip or other archiving
	if (WrapAndShip
	 && MakeArchive
	 && ArchiveCommand
	 && *ArchiveCommand) {
		//ShowStatus(docId, filename, StatArchive);
		if (LogAuto)
			LogEvent(loginfo, 1, "Archiving...");
		ArchiveCommand = Unquote(ArchiveCommand);
		if (ArchiveFrom) {
			tmpcurr = CurrDir;
			CurrDir = arcfromdir = MakeDirPath(CurrDir, ArchiveFrom);
			if (LogAuto)
				LogEvent(loginfo, 1, "cd ", CurrDir);
		}
		if (ArchiveName) {
			//tempname = (OutType == oteh) ? NULL : strchr(ArchiveName, '.');
			nlen = strlen(ArchiveName);

			if (ArchiveVer)
				nlen += strlen(ArchiveVer);

			if (ArchiveExt)
				nlen += strlen(ArchiveExt);
			else
				nlen += 3;
			filename = NewName(nlen + 2);

			//if (tempname)
			//	strncpy(filename, ArchiveName, tempname - ArchiveName);
			//else
				strcpy(filename, ArchiveName);

			if (ArchiveVer)
				strcat(filename, ArchiveVer);

			if (ArchiveExt) {
				if (ArchiveExt[0] != '.')
					strcat(filename, ".");
				strcat(filename, ArchiveExt);
			}
			//else if (tempname)
			//	strcat(filename, tempname);
			else  // no extension, add one
				strcat(filename, ".zip");
		}
		else if (OutType == oteh)
			filename = "plugin.zip";
		else
			filename = NewFileExt(filename, ".zip");

		nlen = strlen(ArchiveCommand) + strlen(filename);
		if (ArchiveStartParams)
			nlen += strlen(ArchiveStartParams);
		if (ArchiveEndParams)
			nlen += strlen(ArchiveEndParams);
		nlen += 6; // 2 quotes, 3 spaces, and 1 nul
		command = NewName(nlen);

		sprintf(command, "\"%s\" %s %s %s", ArchiveCommand,
											(ArchiveStartParams ? ArchiveStartParams : ""),
											filename,
											(ArchiveEndParams ? ArchiveEndParams : ""));

		//if (LogAuto)
		//	LogEvent(loginfo, 1, command);
		RunDOSCommand(command, true, false);
		DeleteName(command);
		if (tmpcurr) {
			CurrDir = tmpcurr;
			if (LogAuto)
				LogEvent(loginfo, 1, "cd ", CurrDir);
		}
		shippable = true;
		archived = true;
	}

	// copy to ship path if any
	if (WrapAndShip
	 && ShipPath && *ShipPath
	 && shippable) {
		//ShowStatus(docId, filename, StatShip);
		if (LogAuto)
			LogEvent(loginfo, 1, "Putting in shipping directory...");
		ShipPath = Unquote(ShipPath);
		if (stricmp(ShipPath, CurrDir)) {  // make it first
			if (!PathIsDirectory(ShipPath)) {
				//if (LogAuto)
				//	LogEvent(loginfo, 1, "mkdir ", ShipPath);
				MakeDir(ShipPath);       // harmless even if it exists
			}
		}
		if (arcfromdir) {
			CurrDir = arcfromdir;
			if (LogAuto)
				LogEvent(loginfo, 1, "cd ", CurrDir);
		}
		command = NewName(strlen(filename) + strlen(ShipPath) + 14);
		sprintf(command, "%s /Y \"%s\" \"%s\"", (MoveArchive ? "move" : "copy"),
		                      filename, ShipPath);
		//if (LogAuto)
		//	LogEvent(loginfo, 1, command);
		RunDOSCommand(command, true, false);
		if (tmpcurr) {
			CurrDir = tmpcurr;
			if (LogAuto)
				LogEvent(loginfo, 1, "cd ", CurrDir);
		}
		if ((OutType == otwh)
		 && !archived) {
			filename = NewFileExt(filename, ".cnt");
			sprintf(command, "%s /Y \"%s\" \"%s\"", (MoveArchive ? "move" : "copy"),
														filename, ShipPath);
			//if (LogAuto)
			//	LogEvent(loginfo, 1, command);
			RunDOSCommand(command, true, false);
		}
		DeleteName(command);
	}

	if (tempcurrdir)
		CurrDir = tempcurrdir;
	if (compdir
	 && graphdir
	 && stricmp(compdir, graphdir))
		DeleteName(graphdir);
	DeleteName(compdir);

	ExecSystemCommand(SystemEndCommand, "End");

	//ClearStatus(docId);
	InWrapShip = false;
	if (LogAutoStarted)
		EndLogAuto();
}


// protected functions

char *
DCauto::MakeFileList(char *filenames)
{
	char *flist = NULL;
	char *fp = NULL;
	char *flp = NULL;
	bool sp = false;
	bool start = true;

	flp = flist = NewName(strlen(filenames) + 2);
	for (fp = filenames; *fp; fp++) {
		if ((*fp == ' ')  // delimiters
		 || (*fp == '\t')
		 || (*fp == ',')) {
			if (start || sp)  // leading or repeated
				continue;
			sp = true;
			*flp++ = '\0';
		}
		else {
			sp = false;
			start = false;
			*flp++ = *fp;
		}
	}
	if (!sp) // last delimiter
		*flp++ = '\0'; 
	*flp = '\0'; // terminator
	return flist;
}


char *
DCauto::MakeDirPath(char *base, char *path, bool empty)
{
	char *dir = base;
	unc pbuf[_MAX_PATH]; 

	if (path && *path) {
		path = Unquote(path);
		RemoveSlash(path);
		if ((path[0] != '\\')
		 && (path[1] != ':')) {
			dir = NewName(strlen(base) + strlen(path) + 2);
			strcpy(dir, base);
			strcat(dir, "\\");
			strcat(dir, path);
		}
		else
			dir = NewName(path);

		if (stricmp(dir, CurrDir)) {
			dir = NewName(_fullpath((char *) pbuf, dir, _MAX_PATH));
			if (!PathIsDirectory(dir)) {
				//if (LogAuto)
				//	LogEvent(loginfo, 1, "mkdir ", dir);
				MakeDir(dir);
			}
			else if (empty) {
				//if (LogAuto)
				//	LogEvent(loginfo, 1, "del ", dir, " /q");
				DeleteDirContents(dir);
			}
		}
	}
	return dir;
}


void 
DCauto::CopyCompFiles(char *filelist, char *dest)
{
	ProcCompFileList(filelist, dest, "", false);
}


void 
DCauto::CopyCompFilesSrc(char *filelist, char *dest, char *src)
{
	ProcCompFileList(filelist, dest, src, false);
}


void 
DCauto::DelCompFiles(char *filelist)
{
	ProcCompFileList(filelist, "", "", true);
}


void 
DCauto::ProcCompFileList(char *filelist, char *dest, char *src, bool del)
{
	char *command = NULL;
	char *item = NULL;
	uns maxitemlen = 9;   // allow for *.ditamap
	unc pbuf[_MAX_PATH]; 

	if (!del
	 && !stricmp(_fullpath((char *) pbuf, dest, _MAX_PATH), CurrDir))
		return;

	for (item = filelist; *item != '\0'; item += strlen(item) + 1) {
		if (strlen(item) > maxitemlen)
			maxitemlen = strlen(item);
	}

	command = (del ? NewName(maxitemlen + 10)
	               : NewName(strlen(dest) + strlen(src) + maxitemlen + 14));
	
	for (item = filelist; *item != '\0'; item += strlen(item) + 1) {
		if (del)  // del /Q item
			sprintf(command, "del /Q \"%s\"", item);
		else      // copy /Y item dest
			sprintf(command, "copy /Y \"%s%s\" \"%s\"", src, item, dest);
		RunDOSCommand(command, true, false);
		if (ExitCode == 1)
			ExitCode = 0;  // ignore missing file errors
		//else if (LogAuto)
		//	LogEvent(loginfo, 1, command); // but only log success
	}

	DeleteName(command);
}





// end of dcauto.cpp

