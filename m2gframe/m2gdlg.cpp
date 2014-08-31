/* m2gdlg.cpp is the dialog processor for mif2go
 * Copyright 2005 Omni Systems, Inc.  All Rights Reserved.
 */

#include "m2gframe.h"


enum M2GDlgConstants {
 maxFileName = 1024, maxDlgTitle = 80,
 ofnFlags = (OFN_CREATEPROMPT | OFN_LONGNAMES | OFN_NOCHANGEDIR | OFN_SHAREAWARE),
 ofnHookFlags = (OFN_ENABLEHOOK | OFN_EXPLORER | OFN_NOVALIDATE),
 brinfFlags = (BIF_RETURNONLYFSDIRS | 0x40)
};


/* globals */

char M2GFrame::FileName[maxFileName] = "";
char M2GFrame::DlgTitle[maxDlgTitle] = "Select";
char M2GFrame::DlgDir[maxFileName] = "";

OPENFILENAME M2GFrame::OfnStruct = {
	sizeof(OPENFILENAME),
							 //    DWORD         lStructSize; 
	NULL,        //    HWND          hwndOwner; 
	NULL,        //    HINSTANCE     hInstance; 
	NULL,        //    LPCTSTR       lpstrFilter; 
	NULL,        //    LPTSTR        lpstrCustomFilter; 
	0,           //    DWORD         nMaxCustFilter; 
	0,           //    DWORD         nFilterIndex; 
	FileName,    //    LPTSTR        lpstrFile; 
	maxFileName, //    DWORD         nMaxFile; 
	NULL,        //    LPTSTR        lpstrFileTitle; 
	0,           //    DWORD         nMaxFileTitle; 
	DlgDir,      //    LPCTSTR       lpstrInitialDir; 
	DlgTitle,    //    LPCTSTR       lpstrTitle; 
	ofnFlags,    //    DWORD         Flags; 
	0,           //    WORD          nFileOffset; 
	0,           //    WORD          nFileExtension; 
	NULL,        //    LPCTSTR       lpstrDefExt; 
	0,           //    DWORD         lCustData; 
	NULL,        //    LPOFNHOOKPROC lpfnHook; 
	NULL         //    LPCTSTR       lpTemplateName; 
};


char M2GFrame::PathName[MAX_PATH] = "";
int M2GFrame::FolderImg = 0;

BROWSEINFO M2GFrame::BrinfStruct = {
	NULL,        //  HWND hwndOwner;
	NULL,        //  LPCITEMIDLIST pidlRoot;
	PathName,    //  LPSTR pszDisplayName;
	DlgTitle,    //  LPCSTR lpszTitle;
	brinfFlags,  //  UINT ulFlags;
	NULL,        //  BFFCALLBACK lpfn;
	0,           //  LPARAM lParam;
	FolderImg    //  int iImage;
};


/* functions */

char **
M2GFrame::ParseOpenArgs(char *chp)
{
	char *argv[3];
	unl argc = GetArgs(chp, argv, 3);

	char *title = NULL;
	char *dir = NULL;
	char **vstr = NULL;

	switch (argc) {
		case 3:
			vstr = (char **) strtoul(argv[2], NULL, 10);
			//fall through
		case 2:
			dir = (char *) strtoul(argv[1], NULL, 10);
			//fall through
		case 1:
			title = (char *) strtoul(argv[0], NULL, 10);
			break;

		default:
			return NULL;
	}

	strncpy(DlgTitle, title, maxDlgTitle);
	strncpy(DlgDir, dir, maxFileName);
	return vstr;
}


void
M2GFrame::GetOpenFile(char *chp)
{
	char **vstr = ParseOpenArgs(chp);
	if (!vstr)
		return;

	FileName[0] = '\0';

	// remove any hook proc
	OfnStruct.Flags = ofnFlags;
	OfnStruct.lpfnHook = NULL;

	// call the common dialog box
	if (::GetOpenFileName(&OfnStruct))
		*vstr = FileName;
	else
		*vstr = NULL;
}


UINT APIENTRY 
M2GFrame::OFNHook(HWND hdlg, UINT uiMsg, WPARAM wParam, LPARAM lParam)
{
	LPOFNOTIFY lpon = (LPOFNOTIFY) lParam; 
	LPNMHDR pnmh = (LPNMHDR) lParam;
	bool bval = false;
	int ret = 0;

	switch (uiMsg) {
		case WM_NOTIFY:
			switch (pnmh->code) {
				case CDN_FILEOK:
					// FileName has name for return
					break;
				case NM_CLICK:
					if (pnmh->idFrom == IDOK) {  // clicked OK, terminate
						ret = CommDlg_OpenSave_GetFolderPath(hdlg, PathName, MAX_PATH);
						bval = PostMessage(hdlg, WM_COMMAND, IDABORT, NULL);
						return 1;
					}
					break;

				case CDN_FOLDERCHANGE:
					// set PathName to new folder with CDM_GETFOLDERPATH
					ret = CommDlg_OpenSave_GetFolderPath(hdlg, PathName, MAX_PATH);
					break;

				default:
					break;
			}
			break;
 
		default:
			break;
	}

	return 0;
}
  

bool 
M2GFrame::SetItemIdList(char *path)
{
	if (!path || !*path)
		return false;

	OLECHAR uniPath[MAX_PATH];
	if (!MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, path, -1, uniPath, MAX_PATH))
		return false;

	LPSHELLFOLDER pSh = NULL;
	if (SHGetDesktopFolder(&pSh) != NOERROR)
		return false;

	ITEMIDLIST *pIDl = NULL;
	HRESULT hr = 0;
	unl chUsed = 0;
	hr = pSh->ParseDisplayName(NULL, NULL, uniPath, &chUsed, &pIDl, NULL);

	pSh->Release();

	if (FAILED(hr))
		return false;

	BrinfStruct.pidlRoot = pIDl;
	return true;
}


int __stdcall
M2GFrame::BrinfCB(HWND hwnd, UINT uMsg, LPARAM lParam, LPARAM lpData)
{
	switch (uMsg) {
		case BFFM_INITIALIZED:
			SendMessage(hwnd, BFFM_SETSELECTION, (WPARAM) TRUE, (LPARAM) DlgDir);
			break;

		case BFFM_SELCHANGED:
			break;

		default:
			break;
	}

	return 0;
}


void
M2GFrame::GetOpenDir(char *chp)
{
	char **vstr = ParseOpenArgs(chp);
	if (!vstr)
		return;

	FileName[0] = '\0';
	PathName[0] = '\0';
	bool UseSHBF = true;

	if (UseSHBF) {   // use SHBrowseForFolder
		// SetItemIdList(DlgDir);  unfortunately prevents going above DlgDir
		BrinfStruct.lpfn = &BrinfCB;
		LPITEMIDLIST RetID = ::SHBrowseForFolder(&BrinfStruct);

		if (RetID
		 && ::SHGetPathFromIDList(RetID, PathName))
			*vstr = PathName;
		else
			*vstr = NULL;
	}
	else {           // use OFN common dialog
		// set hook proc
		OfnStruct.Flags = ofnFlags | ofnHookFlags;
		OfnStruct.lpfnHook = &OFNHook;

		if (::GetOpenFileName(&OfnStruct)) {
			if (FileName[strlen(FileName) - 1] == '*')
				FileName[strlen(FileName) - 1] = '\0';
			*vstr = FileName;
		}
		else if (PathName[0] != '\0')
			*vstr = PathName;
		else
			*vstr = NULL;
	}
}


void
M2GFrame::GetMyDocDir(char *chp)
{
	HRESULT hr = 0;
	ITEMIDLIST *pidl = NULL;
	char path[MAX_PATH] = "";
	char rval[15] = "";
	char *rch = NULL;

	hr = ::SHGetSpecialFolderLocation(NULL, CSIDL_PERSONAL, &pidl);
	if (!FAILED(hr)
	 && pidl
	 && (::SHGetPathFromIDList(pidl, path) == TRUE)) {
		sprintf(rval, "%d", (long) NewName(path, strlen(path) + 2));
		rch = (char *) strtoul(chp, NULL, 10);
		strncpy(rch, rval, 14);
	}
}


void
M2GFrame::WaitFileDone(char *chp)
{
	char *path = (char *) strtoul(chp, NULL, 10);
	HANDLE hand = ::FindFirstChangeNotification(path,
	                FALSE, FILE_NOTIFY_CHANGE_SIZE);

	DWORD ret = WAIT_FAILED;

	// wait until size starts to change for 60 seconds.
	ret = ::WaitForSingleObject(hand, (DWORD) 60000);
	if (ret != WAIT_OBJECT_0)
		return;  // no change, may be done already

	// wait until size stops changing for 5 seconds.
	while (ret == WAIT_OBJECT_0) {
		if (::FindNextChangeNotification(hand) == FALSE)
			break;
		ret = ::WaitForSingleObject(hand, (DWORD) 5000);
	}
	::FindCloseChangeNotification(hand);
}



HANDLE PMFlag = NULL;

void 
M2GFrame::SetPMFlag(void)
{
	// set flag to tell drmif not to move log file we just started
	PMFlag = CreateSemaphore(NULL, 0L, 1L, "m2gpl");
}


bool 
M2GFrame::CheckLogErrors(void)
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


int M2GFrame::ExitCode = 0;
int M2GFrame::ExitType = 0;
char *M2GFrame::ComSpec = NULL;
char *M2GFrame::CurrDir = NULL;


void 
M2GFrame::RunDOSCommand(char *cmd, bool hide, bool keep, bool synch)
{
	char *command = NULL;
	//char buffer[_MAX_PATH];

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

	//CurrDir = ::_getcwd(buffer, _MAX_PATH);
	RunCommand(command, CurrDir, synch, hide, false);

	DeleteName(command);
}


void 
M2GFrame::RunCommand(char *command, char *cd, bool synch,
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

// end of m2gdlg.cpp

