/* m2gframe.cpp is the Frame FDK plugin for mif2go
 * Last edited by Jeremy H. Griffith.
 * Copyright 2008 Omni Systems, Inc.  All Rights Reserved.
 */

#include "m2gframe.h"

StringT M2GFrame::PluginVersion = (StringT) "f009";

/* called by api at startup */

VoidT F_ApiInitialize(IntT init)
{

	/* only initialize once */

	if (init != FA_Init_First)
		return;

	M2GFrame::SetPMFlag();

	__try {
		/* request notification from m2r.c */
		F_ApiNotification(FA_Note_ClientCall, True);
	}
	__except(EXCEPTION_EXECUTE_HANDLER) {
		F_ApiAlert((ConStringT) "Error initializing m2gframe.", FF_ALERT_CONTINUE_NOTE);
	}
}


/* called when m2r is invoked by a SaveAs mif2rtf operation */

VoidT F_ApiNotify(IntT notification, F_ObjHandleT docId,
                  StringT filename, IntT iparm)
{

	__try {
		/* docId is not passed, but must be the active doc */
		if (docId == 0)
			docId = F_ApiGetId(0, FV_SessionId, FP_ActiveDoc);

		if (notification == FA_Note_ClientCall) {
			// filename is really the str argument to the call
			// docId is always 0, iparm is also unused
			if (!strncmp((char *) filename, "m2rb:", 5)) { // caller
				char *chp = (char *) filename + 5;
				char *spc = strchr(chp, ' ');  // space delimits request
				char *req = NULL;
				unl val = 0;
				if (spc) {
					req = NewName(chp, spc - chp);
					chp = spc + 1;
					if (!strcmp(req, "IniSetup"))
						M2GFrame::SetUpIni(chp);
					else if (!strcmp(req, "IniSects"))
						M2GFrame::GetIniSects(chp);  // always in CurrIni
					else if (!strcmp(req, "IniGroup"))
						M2GFrame::GetIniGroup(chp);
					else if (!strcmp(req, "IniMacroString"))
						M2GFrame::GetIniSetting(chp, true);
					else if (!strcmp(req, "IniString"))
						M2GFrame::GetIniSetting(chp);
					else if (!strcmp(req, "IniSectList"))
						M2GFrame::GetIniSectList(chp);
					else if (!strcmp(req, "IniSect"))
						M2GFrame::GetIniSection(chp);
					else if (!strcmp(req, "IniProcDone"))
						M2GFrame::IniProcDone();

					else if (!strcmp(req, "IniGetSetting"))
						M2GFrame::PGetSetting(chp);
					else if (!strcmp(req, "IniPutSetting"))
						M2GFrame::PPutSetting(chp);
					else if (!strcmp(req, "IniPRenSect"))
						M2GFrame::PRenSect(chp);
					else if (!strcmp(req, "IniPUpdate"))
						M2GFrame::PUpdate(chp);
					else if (!strcmp(req, "IniPClose"))
						M2GFrame::PClose(chp);

					else if (!strcmp(req, "MacroPreVars"))
						M2GFrame::MacroPreVars(chp);
					else if (!strcmp(req, "MacroProc"))
						M2GFrame::MacroProc(chp);
					else if (!strcmp(req, "MacroProcDone"))
						M2GFrame::MacroProcDone();

					else if (!strcmp(req, "GetOpenFile"))
						M2GFrame::GetOpenFile(chp);
					else if (!strcmp(req, "GetOpenDir"))
						M2GFrame::GetOpenDir(chp);
					else if (!strcmp(req, "GetMyDocDir"))
						M2GFrame::GetMyDocDir(chp);
					else if (!strcmp(req, "WaitFileDone"))
						M2GFrame::WaitFileDone(chp);

					else if (!strcmp(req, "OpenProject"))
						M2GFrame::OpenProject(chp);
					else if (!strcmp(req, "CloseProject"))
						M2GFrame::CloseProject(chp);
					else if (!strcmp(req, "OpenLog"))
						M2GFrame::OpenLog(chp);
					else if (!strcmp(req, "CloseLog"))
						M2GFrame::CloseLog(chp);
					else if (!strcmp(req, "LogEvent"))
						M2GFrame::LogEvent(chp);

					else { // next request with params
					}
				}
				else { // requests without params
				}
			}
		}
	}
	__except(EXCEPTION_EXECUTE_HANDLER) {
		F_ApiAlert((ConStringT) "Error processing m2gframe notification.", FF_ALERT_CONTINUE_NOTE);
	}
}


/* called in response to a hypertext message */

VoidT F_ApiMessage(StringT message, F_ObjHandleT docId, F_ObjHandleT objId)
{

	__try {
		/* if docId is not passed, must be the active doc */
		if (docId == 0)
			docId = F_ApiGetId(0, FV_SessionId, FP_ActiveDoc);
	}
	__except(EXCEPTION_EXECUTE_HANDLER) {
		F_ApiAlert((ConStringT) "Error processing m2gframe hypertext command.", FF_ALERT_CONTINUE_NOTE);
	}
}


/* called when one of our commands is invoked */

VoidT F_ApiCommand(IntT command)
{
	__try {
	}
	__except(EXCEPTION_EXECUTE_HANDLER) {
		F_ApiAlert((ConStringT) "Error processing m2gframe command.", FF_ALERT_CONTINUE_NOTE);
	}
}


/* called when dialog is up */

VoidT F_ApiDialogEvent(IntT dlgNum, IntT itemNum, IntT modifiers)
{
	__try {
	}
	__except(EXCEPTION_EXECUTE_HANDLER) {
		F_ApiAlert((ConStringT) "Error processing m2gframe dialog event.", FF_ALERT_CONTINUE_NOTE);
	}
}


// logging interface to DClog

bool M2GFrame::UseLog = false;
bool M2GFrame::ShowLog = true;
bool M2GFrame::Logging = false;
bool M2GFrame::LogMsg = true;
char *M2GFrame::LogFileName = "_m2g_log.txt";
char *M2GFrame::HistoryFileName = "_m2g_history.txt";
char *M2GFrame::EditorFileName = "notepad.exe";
DClog *M2GFrame::Log = NULL;
unc M2GFrame::LogLevels[logmax] = { 0, 1, 1, 1, 1, 0 };


void
M2GFrame::OpenProject(char *chp)
{
	// char* has m2rbook version, comma, six digits of levels,
  // then log file name, history file name, editor file name,
	// current directory, and a flag for ShowLog

	unl len = 0;
	char *ver = NULL;
	char *proj = NULL;
	char *cpt = strchr(chp, ',');
	if (!cpt)
		return;
	*cpt++ = '\0';  // end of version
	proj = cpt;
	cpt = strchr(proj, ',');
	if (!cpt) {
		cpt = proj;
		proj = NULL;
	}
	else
		*cpt++ = '\0';  // end of project
	len = strlen(chp) + strlen((char *) PluginVersion) + 6;
	if (proj)
		len += strlen(proj) + 12;
	ver = NewName((short) len);
	if (proj)
		sprintf(ver, "%s and %s, project \"%s\"", chp, PluginVersion, proj);
	else
		sprintf(ver, "%s and %s", chp, PluginVersion);

	for (int i = 0; i < logmax; i++)
		LogLevels[i] = *cpt++ - '0';

	char *comma = strchr(cpt, ',');
	if (comma)
		*comma = '\0';
	LogFileName = NewName(cpt);
	if (comma) {
		cpt = ++comma;
		comma = strchr(cpt, ',');
		if (comma)
			*comma = '\0';
		HistoryFileName = NewName(cpt);
	}
	if (comma) {
		cpt = ++comma;
		comma = strchr(cpt, ',');
		if (comma)
			*comma = '\0';
		EditorFileName = NewName(cpt);
	}
	if (comma) {
		cpt = ++comma;
		comma = strchr(cpt, ',');
		if (comma)
			*comma = '\0';
		CurrDir = NewName(cpt);
	}
	if (comma) {
		cpt = ++comma;
		if (*cpt == '1')
			ShowLog = true;
		else if (*cpt == '0')
			ShowLog = false;
	}

	if (DCrfile::FileExists(LogFileName)) {  // append to history
		char *cmd = NewName("type ", strlen(LogFileName) + 
				                         strlen(HistoryFileName) + 8);
		strcat(cmd, LogFileName);
		strcat(cmd, " >>");
		strcat(cmd, HistoryFileName);
		RunDOSCommand(cmd);
		strcpy(cmd, "del /Q ");
		strcat(cmd, LogFileName);
		RunDOSCommand(cmd);
		DeleteName(cmd);
	}

	Log = new DClog(LogFileName, "m2rbook", ver, LogLevels, true);
	UseLog = (Log->FErr() == fok) ? true : false;
	Logging = false;
	DeleteName(ver);
}

void
M2GFrame::CloseProject(char *chp)
{
	// char* has "0" for temp close (while running dcl)
	// or "1" for final close for book/file processing
	Logging = (*chp == '0') ? true : false;
	if (UseLog) {
		Log->logEvent(loginfo, 1, Logging ? "Suspending" : "Closing",
		   " log for m2rbook");
		Log->putCr();
		Log->putCr();
		Log->CloseFile();
		UseLog = false;

		if (!Logging) {
			if (ShowLog
			 && CheckLogErrors())
				ShowLogFile();

			delete Log;
			Log = NULL;
			UseLog = false;
			LogFileName = "_m2g_log.txt";
			LogLevels[logerr] = 1;
			LogLevels[logwarn] = 1;
			LogLevels[logquery] = 1;
			LogLevels[loginfo] = 1;
			LogLevels[logdebug] = 0;
		}
	}
}


void
M2GFrame::OpenLog(char *chp)
{
	// char* has current .fm file name
	if (Logging
	 && !UseLog) { // reopen same log file
		Log->ReopenFile(LogFileName, false);
		UseLog = (Log->FErr() == fok) ? true : false;
		Logging = false;
		//if (LogMsg)
		//	Log->logEvent(loginfo, 1, "Resuming log for m2rbook");
	}
	else if (!Logging
	 && UseLog)
		Log->logEvent(loginfo, 1, "Opened file ", chp);
}

void
M2GFrame::CloseLog(char *chp)
{
	// char* has "0" for temp close (while running dcl)
	// or "1" for final close for file
	//LogMsg = (*chp == '1') ? true : false;
	if (UseLog) {
		Log->CloseFile();
		UseLog = false;
		Logging = true;
	}
}

void
M2GFrame::LogEvent(char *chp)
{
	// char* has two digits for type and severity, then desc
	logtype lt = (logtype) (*chp++ - '0');
	unc sev = *chp++ - '0';

	if (UseLog)
		Log->logEvent(lt, sev, chp);
}


void
M2GFrame::ShowLogFile(void)
{
	char *cmd = NewName(EditorFileName, strlen(EditorFileName) +
																			strlen(LogFileName) + 1);
	strcat(cmd, " ");
	strcat(cmd, LogFileName);
	RunDOSCommand(cmd, true, false, false);
	DeleteName(cmd);
}





/* end of m2gframe.cpp */

