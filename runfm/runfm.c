
// Run FM Mif2Go project from external console application.
// Copyright (c) 2005-2009 Jeremy H. Griffith.  All rights reserved.

#define DONT_REDEFINE // Console app needs native types
#define MAXMSG 512
#define WBUFLEN 512
#define OPENERRS 11

#include "fdetypes.h"
#include "futils.h"
#include "fapi.h"
#include "fstrings.h"
#include "fchannel.h"

#include <windows.h>
#include <assert.h>
#include <time.h>


extern BoolT GetArgs(int argc, char **argv);
extern BoolT ConnectToFmSession(VoidT);
extern F_ObjHandleT OpenFmDoc(BoolT book);
extern IntT RunM2gProject(VoidT);
extern VoidT CloseFrameFiles(VoidT);
extern VoidT AppendToLog(StringT docname);


StringT runfm_ver = "\nrunfm Ver. 4.0, Build 005\n";
StringT opt_progid = "FrameMaker.M2G";
StringT opt_book = NULL;
StringT opt_doc = NULL;
StringT opt_remote = NULL;
StringT opt_client = "OmniBookExport";
StringT opt_log = "mif2go.log";
StringT opt_project = NULL;
StringT opt_print = NULL;
StringT opt_pdf = NULL;
StringT opt_printer = NULL;
StringT setprint = "SetPrint";
StringT bookname = NULL;
StringT docname = NULL;
char bookIdStr[11];
char docIdStr[11];
IntT opt_pause = 0;
BoolT isBook = False;
BoolT isDoc = False;
BoolT closeBook = False;
BoolT closeDoc = False;
BoolT closeFrame = False;
BoolT printBook = False;
BoolT printDoc = False;
BoolT pdfBook = False;
BoolT pdfDoc = False;
BoolT pdfSaveBook = False;
BoolT pdfSaveDoc = False;
BoolT diag = False;
BoolT revOn = False;
BoolT revOff = False;
F_ObjHandleT bookId;
F_ObjHandleT docId;

IntT openErrs[OPENERRS] = {
	FV_BadFileType,
	FV_BadFileName,
	FV_DocHeadersDamaged,
	FV_DocWrongSize,
	FV_ChecksumDamage,
	FV_TooManyWindows,
	FV_BadTemplate,
	FV_FileNotReadable,
	FV_LockCouldntBeReset,
	FV_FileWasInUse,
	FV_LockWasInvalid
//	FV_CancelTempDiskFull
};

StringT openErrMsgs[OPENERRS] = {
	"File is not a FrameMaker document",
	"File could not be found",
	"File is damaged",
	"File is damaged (size problem)",
	"File has a bad checksum",
	"Too many windows are already open", // Brrr!
	"Template is damaged",               // What template??
	"You do not have read permission for this file",
	"File is locked; could not reset lock",
	"File is in use by another process",
	"File has an invalid lock"
//	"Temporary disk area is full"
};


int main(int argc, char **argv)
{
	F_ObjHandleT bookIdAct = 0;
	F_ObjHandleT docIdAct = 0;
	StringT actBookname = NULL;
	StringT actDocname = NULL;
	IntT ltime = 0;
	IntT ret = 0;
	char msg[MAXMSG] = "";
	int errorCode = 0;

	fprintf(stderr, runfm_ver);

	// Parse arguments
	if (!GetArgs(argc, argv)) {
		fprintf(stderr, "Usage: %s [-progid FrameMaker.M2G] [-remote sysname]\n", argv[0]);
		fprintf(stderr, "[-book [\\path\\bookname.book]] [-doc [\\path\\docname.fm]]\n");
		fprintf(stderr, "[-project \"project name\"] [-client OmniBookExport]\n");
		fprintf(stderr, "[[-print (book | doc) [\\path\\printfile.prn]]\n");
		fprintf(stderr, "| [-pdf (book | doc) [\\path\\pdffile.pdf]]\n");
		fprintf(stderr, "| [-pdfsave (book | doc) [\\path\\pdffile.pdf]]]\n");
		fprintf(stderr, "[-printer \"name of printer to set via SetPrint\"]\n");
		fprintf(stderr, "[-reverse (yes | no)] [-pause (seconds)] [-diag]\n");
		fprintf(stderr, "[-close (book | doc | all)] [-log logfile.txt]\n");
		return 1;
	}

	if (!ConnectToFmSession())
		return 1;

	F_Printf(NULL, runfm_ver);

	time(&ltime);
	sprintf(msg, "\nrunfm project \"%s\" started: %s\n",
	  (opt_project ? opt_project :
	  (printDoc  ? "print doc" :
	  (printBook ? "print book" :
	  (pdfDoc  ? "pdf doc" :
	  (pdfBook ? "pdf book" :
	  (pdfSaveDoc  ? "pdfsave doc" :
	  (pdfSaveBook ? "pdfsave book" :
	  "none specified"))))))),
	  (StringT) ctime(&ltime));
	F_Printf(NULL, (StringT) msg);
	fprintf(stderr, msg);

	// load the book or the doc for -doc, if specified
	if (isBook && opt_book) {
		if (!(bookId = OpenFmDoc(True))) {
			fprintf(stderr, "Failed to open book: %s\n", opt_book);
			return 1;
		}
		bookname = F_ApiGetString(FO_Session, bookId, FP_Name);
		F_Printf(NULL, "Opened book: %s\n", bookname);
	}

	if (isDoc && opt_doc) {
		if (!(docId = OpenFmDoc(False))) {
			fprintf(stderr, "Failed to open doc: %s\n", opt_doc);
			return 1;
		}
		docname = F_ApiGetString(FO_Session, docId, FP_Name);
		F_Printf(NULL, "Opened document: %s\n", docname);
	}

	// get the name of the current book or document
	if ((isBook || !isDoc)  // unless using just -doc
	 && ((bookIdAct = F_ApiGetId(0, FV_SessionId, FP_ActiveBook)) != 0)) {
		actBookname = F_ApiGetString(FO_Session, bookIdAct, FP_Name);
		F_Printf(NULL, "Current active book: %s\n", actBookname);
		if (!opt_book) { // using existing book
			isBook = True;
			bookId = bookIdAct;
			if (bookname)
				F_Free(bookname);
			bookname = actBookname;
			actBookname = NULL;
		}
	}
	if ((isDoc || !isBook)  // unless using just -book
	 && ((docIdAct = F_ApiGetId(0, FV_SessionId, FP_ActiveDoc)) != 0)) {
		actDocname = F_ApiGetString(FO_Session, docIdAct, FP_Name);
		F_Printf(NULL, "Current active document: %s\n", actDocname);
		if (!opt_doc) { // using existing document
			isDoc = True;
			docId = docIdAct;
			if (docname)
				F_Free(docname);
			docname = actDocname;
			actDocname = NULL;
		}
	} 

	if (!bookIdAct && !docIdAct) {
		time(&ltime);
		F_Printf(NULL, "No active FrameMaker book or document\n");
		fprintf(stderr, "No active FrameMaker book or document\n");
		sprintf(msg, "%s: runfm cannot find active document\n",
		              (StringT) ctime(&ltime));
		errorCode = 2;
	}
	else if (opt_project
	 || printBook || printDoc
	 || pdfBook || pdfDoc
	 || pdfSaveBook || pdfSaveDoc) {
		ret = 0;
		if (opt_printer) {
			ret = F_ApiCallClient(setprint, "/save");
			if (ret != 0) {
				F_Printf(NULL, "SetPrint: not installed\n");
				fprintf(stderr, "SetPrint: not installed\n");
			}
			else {
				ret = F_ApiCallClient(setprint, opt_printer);
				if (ret == 0) {
					F_Printf(NULL, "SetPrint: printer is %s\n", opt_printer);
					fprintf(stderr, "SetPrint: printer is %s\n", opt_printer);
				}
				else if (ret == -1) {
					F_Printf(NULL, "SetPrint: printer not found: %s\n", opt_printer);
					fprintf(stderr, "SetPrint: printer not found: %s\n", opt_printer);
				}
				else {
					F_Printf(NULL, "SetPrint: error setting printer\n");
					fprintf(stderr, "SetPrint: error setting printer\n");
				}
			}
		}
		// run the project named for -project projname using m2rbook
		if (bookId)
			sprintf(bookIdStr, "!%d", bookId);
		if (docId)
			sprintf(docIdStr, "!%d", docId);

		errorCode = RunM2gProject();
		time(&ltime);

		if (errorCode)  // nonzero is failure
			sprintf(msg, "%s: runfm project failed: %d\n",
				           (StringT) ctime(&ltime), errorCode);
		else
			sprintf(msg, "%s: runfm project completed\n",
			             (StringT) ctime(&ltime));

		if (opt_printer && !ret) {
			ret = F_ApiCallClient(setprint, "/restore");
			if (ret == -3) {
				F_Printf(NULL, "SetPrint: nothing to restore\n");
				fprintf(stderr, "SetPrint: nothing to restore\n");
			}
		}
	}
	else {
		time(&ltime);
		sprintf(msg, "%s: runfm cannot run project\n", (StringT) ctime(&ltime));
		errorCode = 3;
	}
	F_Printf(NULL, (StringT) msg);
	fprintf(stderr, msg);

	CloseFrameFiles();

	if (docname)
		F_Free(docname);
	if (actDocname)
		F_Free(actDocname);
	if (bookname)
		F_Free(bookname);
	if (actBookname)
		F_Free(actBookname);

	if (closeFrame)
		F_ApiShutDown();
	else
		F_ApiDisconnectFromSession();

	if (opt_pause)
		Sleep(opt_pause * 1000);

	return errorCode;
}


BoolT GetArgs(int argc, char **argv)
{
	StringT arg = NULL;
	IntT i = 0;
	IntT sw = 0;

	if (argc == 1)
		return False;

	for (i = 1; i < argc; ++i) {
		arg = argv[i];
		switch (sw) {
			case 0:  // get switch ID
			getid:
				if (arg[0] == '-' || arg[0] == '/') {
	 				if (!stricmp(++arg, "progid"))
						sw = 1;
	 				else if (!stricmp(arg, "book"))
						sw = 2;
	 				else if (!stricmp(arg, "doc"))
						sw = 3;
	 				else if (!stricmp(arg, "project"))
						sw = 4;
	 				else if (!stricmp(arg, "close"))
						sw = 5;
	 				else if (!stricmp(arg, "client"))
						sw = 6;
	 				else if (!stricmp(arg, "remote"))
						sw = 7;
	 				else if (!stricmp(arg, "log"))
						sw = 8;
	 				else if (!stricmp(arg, "print"))
						sw = 9;
	 				else if (!stricmp(arg, "pdfsave"))
						sw = 16;
	 				else if (!stricmp(arg, "pdf"))
						sw = 11;
	 				else if (!stricmp(arg, "printer"))
						sw = 13;
	 				else if (!stricmp(arg, "reverse"))
						sw = 14;
	 				else if (!stricmp(arg, "pause"))
						sw = 15;
					else if (!stricmp(arg, "diag"))
						diag = True;
	 				else if (!stricmp(arg, "help")
	 				 || !stricmp(arg, "?"))
						return False;
					else {
						fprintf(stderr, "Invalid switch: %s\n", --arg);
						return False;
					}
				}
				else {
					fprintf(stderr, "Unrecognized argument: %s\n", arg);
					return False;
				}
				break;

			case 1:  // get FrameMaker program ID
				opt_progid = (StringT) arg;
				sw = 0;
				break;

			case 2:  // get name and path of book to open
				isBook = True;
				if (arg[0] == '-' || arg[0] == '/')
					goto getid;  // not a book name
				opt_book = (StringT) arg;
				sw = 0;
				break;

			case 3:  // get name and path of doc to open
				isDoc = True;
				if (arg[0] == '-' || arg[0] == '/')
					goto getid;  // not a doc name
				opt_doc = (StringT) arg;
				sw = 0;
				break;

			case 4:  // get name of M2G project to run
				opt_project = (StringT) arg;
				sw = 0;
				break;

			case 5:  // get close option
				if (arg[0] == '-' || arg[0] == '/')
					goto getid;  // not a close option
	 			if (!stricmp(arg, "book")) {
					closeBook = True;
					closeDoc = False;
					closeFrame = False;
				}
	 			else if (!stricmp(arg, "doc")) {
					closeBook = False;
					closeDoc = True;
					closeFrame = False;
				}
	 			else if (!stricmp(arg, "all")) {
					closeBook = True;
					closeDoc = True;
					closeFrame = True;
				}
				else {
					fprintf(stderr, "Unrecognized close option: %s\n", arg);
					return False;
				}
				sw = 0;
				break;

			case 6:  // get name of client to run
				opt_client = (StringT) arg;
				sw = 0;
				break;

			case 7:  // get name of remote system if any
				opt_remote = (StringT) arg;
				sw = 0;
				break;

			case 8:  // get name for log file
				opt_log = (StringT) arg;
				sw = 0;
				break;

			case 9:  // get print option
	 			if (!stricmp(arg, "book")) {
					printBook = True;
					printDoc = False;
				}
	 			else if (!stricmp(arg, "doc")) {
					printBook = False;
					printDoc = True;
				}
				else {
					fprintf(stderr, "Unrecognized print option: %s\n", arg);
					return False;
				}
				sw = 10;
				break;

			case 10:  // get print file
				if (arg[0] == '-' || arg[0] == '/')
					goto getid;  // not a print file
				opt_print = (StringT) arg;
				sw = 0;
				break;

			case 11:  // get pdf option
	 			if (!stricmp(arg, "book")) {
					pdfBook = True;
					pdfDoc = False;
				}
	 			else if (!stricmp(arg, "doc")) {
					pdfBook = False;
					pdfDoc = True;
				}
				else {
					fprintf(stderr, "Unrecognized pdf option: %s\n", arg);
					return False;
				}
				sw = 12;
				break;

			case 12:  // get pdf file
				if (arg[0] == '-' || arg[0] == '/')
					goto getid;  // not a print file
				opt_pdf = (StringT) arg;
				sw = 0;
				break;

			case 13:  // get printer name
				if (arg[0] == '-' || arg[0] == '/')
					goto getid;  // not a print file
				opt_printer = (StringT) arg;
				sw = 0;
				break;

			case 14:  // get reverse option
				if (arg[0] == '-' || arg[0] == '/') {
					fprintf(stderr, "Reverse option missing: yes | no.\n");
					return False;
				}
	 			else if (!stricmp(arg, "yes"))
					revOn = True;
	 			else if (!stricmp(arg, "no"))
					revOff = True;
				else {
					fprintf(stderr, "Unrecognized reverse option: %s\n", arg);
					return False;
				}
				sw = 0;
				break;

			case 15:  // get pause duration
				if (arg[0] == '-' || arg[0] == '/')
					goto getid;  // not a duration
				opt_pause = F_StrAlphaToInt((StringT) arg);
				sw = 0;
				break;

			case 16:  // get pdf save option
	 			if (!stricmp(arg, "book")) {
					pdfSaveBook = True;
					pdfSaveDoc = False;
				}
	 			else if (!stricmp(arg, "doc")) {
					pdfSaveBook = False;
					pdfSaveDoc = True;
				}
				else {
					fprintf(stderr, "Unrecognized pdfsave option: %s\n", arg);
					return False;
				}
				sw = 12;
				break;

			default:  // can't happen  ;-)
				assert(0);
				return False;
		}
	}

	//if (!opt_project) {
	//	fprintf(stderr, "\nNo runfm -project specified.\n");
	//	return False;
	//}

	return True;
}


BoolT ConnectToFmSession(VoidT)
{
	CLSID serverClass;
	HRESULT res = S_OK;
	OLECHAR wbuf[WBUFLEN];
	ConStringT start = NULL;

	// Convert a process name into a GUID
	MultiByteToWideChar(CP_ACP, 0, (char *) opt_progid, -1, wbuf, WBUFLEN);
	if (wbuf[0] == '{')     // {clsid} is required for remote systems
		res = CLSIDFromString(wbuf, &serverClass);
	else                    // FrameMaker.M2G works only for local system
		res = CLSIDFromProgID(wbuf, &serverClass);
	if (res != S_OK) {
		fprintf(stderr, "This ID is not registered for FrameMaker: %s\n",
		                                                   opt_progid);
		if (wbuf[0] == '{')
			fprintf(stderr, "Obtain the correct {Class} from the remote registry.\n");
		else
			fprintf(stderr, "You must first set up FrameMaker with -progid:%s\n",
                                                           opt_progid);
		return False;
	}

	// Connect to the FrameMaker process.
	if (opt_remote)
		fprintf(stderr, "Connecting to remote system: %s\n", opt_remote);

	FA_errno = 0;
	F_ApiWinConnectSession(0, opt_remote, &serverClass);
	if (!F_ApiAlive()) {
		fprintf(stderr, "Couldn't connect to %s\n", opt_progid);
		return False;
	}
	start = F_ApiStartUp(NULL);
	if (start) {
		fprintf(stderr, "Couldn't start: %s\n", start);
		return False;
	}
	return True;
}


F_ObjHandleT OpenFmDoc(BoolT book)
{
	IntT i = 0;
	F_PropValsT openParams;
	F_PropValsT *openRetParamsp;
	IntT ErrFlag = 0;
	F_ObjHandleT id = 0;
	BoolT rev = False;

	FA_errno = FE_Success;
	openParams = F_ApiGetOpenDefaultParams();
	if (openParams.len == 0) {
		F_Printf(NULL, "Error getting default params for %s: %s\n",
		                (book ? "book" : "doc"), (book ? opt_book : opt_doc));
		return 0;
	}

	/* Get indexes of properties and change them. */
	i = F_ApiGetPropIndex(&openParams, FS_FontNotFoundInDoc);
	openParams.val[i].propVal.u.ival = FV_DoOK;
	i = F_ApiGetPropIndex(&openParams, FS_FontNotFoundInCatalog);
	openParams.val[i].propVal.u.ival = FV_DoOK;
	i = F_ApiGetPropIndex(&openParams, FS_LanguageNotAvailable);
	openParams.val[i].propVal.u.ival = FV_DoOK;
	i = F_ApiGetPropIndex(&openParams, FS_FontChangedMetric);
	openParams.val[i].propVal.u.ival = FV_DoOK;
	i = F_ApiGetPropIndex(&openParams, FS_FileIsOldVersion);
	openParams.val[i].propVal.u.ival = FV_DoOK;
	i = F_ApiGetPropIndex(&openParams, FS_UpdateXRefs);
	openParams.val[i].propVal.u.ival = FV_DoYes;
	i = F_ApiGetPropIndex(&openParams, FS_OpenFileNotWritable);
	openParams.val[i].propVal.u.ival = FV_DoOK;
	i = F_ApiGetPropIndex(&openParams, FS_RefFileNotFound);
	openParams.val[i].propVal.u.ival = FV_AllowAllRefFilesUnFindable;
	i = F_ApiGetPropIndex(&openParams, FS_FileIsStructured);
	openParams.val[i].propVal.u.ival = FV_StripStructureAndOpen;
	i = F_ApiGetPropIndex(&openParams, FS_UseAutoSaveFile);
	openParams.val[i].propVal.u.ival = FV_DoYes;
	i = F_ApiGetPropIndex(&openParams, FS_UseRecoverFile);
	openParams.val[i].propVal.u.ival = FV_DoYes;
	i = F_ApiGetPropIndex(&openParams, FS_MakeIconic);
	openParams.val[i].propVal.u.ival = True;
 
	openRetParamsp = NULL;

	id = F_ApiOpen(book ? opt_book : opt_doc, &openParams, &openRetParamsp);

	i = F_ApiGetPropIndex(openRetParamsp, FS_OpenStatus);
	ErrFlag = openRetParamsp->val[i].propVal.u.ival;

	F_ApiDeallocatePropVals(&openParams);

	if (!id) {
		F_Printf(NULL, "Error %d opening FrameMaker document, flag %d\n",
		                FA_errno,                             ErrFlag);
		for (i = 0; i < OPENERRS; i++) {
			if (F_ApiCheckStatus(openRetParamsp, openErrs[i]))
				F_Printf(NULL, "%s: %s\n", openErrMsgs[i],
											 book ? opt_book : opt_doc);
		}
	}
	else if (diag) {
		rev = F_ApiGetInt(0, id, FP_PrintLastSheetFirst);
		F_Printf(NULL, "Last Sheet First is %s\n", rev ? "On" : "Off");
	}

	F_ApiDeallocatePropVals(openRetParamsp);
	return id;
}


IntT RunM2gProject(VoidT)
{
	StringT arg = NULL;
	IntT ret = FE_Success;
	BoolT m2g = True;
	IntT len = 0;

	m2g = stricmp(opt_client, "OmniBookExport") ? False : True;

	if (m2g) {
		arg = F_StrNew(11);
		F_StrCpy(arg, "rfinit=");
		F_StrCat(arg, (diag) ? "yes" : "no");
		F_ApiCallClient(opt_client, arg);
		F_Free(arg);

		if (isBook) {
			if (!bookname) {
				F_Printf(NULL, "Frame book not found.\n");
				return 1;
			}
			arg = F_StrNew(10 + F_StrLen(bookname) + F_StrLen(bookIdStr));
			F_StrCpy(arg, "runbook=");
			F_StrCat(arg, bookname);
			F_StrCat(arg, bookIdStr);
			F_ApiCallClient(opt_client, arg);
			F_Free(arg);
		}
		if (isDoc) {
			if (!docname) {
				F_Printf(NULL, "Frame document not found.\n");
				return 1;
			}
			arg = F_StrNew(10 + F_StrLen(docname) + F_StrLen(docIdStr));
			F_StrCpy(arg, "rundoc=");
			F_StrCat(arg, docname);
			F_StrCat(arg, docIdStr);
			F_ApiCallClient(opt_client, arg);
			F_Free(arg);
		}

		if (revOn || revOff) {
			arg = F_StrNew(14);
			F_StrCpy(arg, "lastfirst=");
			F_StrCat(arg, (revOn) ? "yes" : "no");
			F_ApiCallClient(opt_client, arg);
			F_Free(arg);
		}

		if (printBook || printDoc) {
			len = 11;
			if (opt_print)
				len += F_StrLen(opt_print);
			arg = F_StrNew(len);
			F_StrCpy(arg, (printBook ? "printbook=" : "printdoc="));
			if (opt_print)
				F_StrCat(arg, opt_print);
			F_ApiCallClient(opt_client, arg);
			F_Free(arg);
		}

		if (pdfBook || pdfDoc) {
			len = 9;
			if (opt_pdf)
				len += F_StrLen(opt_pdf);
			arg = F_StrNew(len);
			F_StrCpy(arg, (pdfBook ? "pdfbook=" : "pdfdoc="));
			if (opt_pdf)
				F_StrCat(arg, opt_pdf);
			F_ApiCallClient(opt_client, arg);
			F_Free(arg);
		}

		if (pdfSaveBook || pdfSaveDoc) {
			len = 13;
			if (opt_pdf)
				len += F_StrLen(opt_pdf);
			arg = F_StrNew(len);
			F_StrCpy(arg, (pdfSaveBook ? "pdfsavebook=" : "pdfsavedoc="));
			if (opt_pdf)
				F_StrCat(arg, opt_pdf);
			F_ApiCallClient(opt_client, arg);
			F_Free(arg);
		}

		len = 9;
		if (opt_project)
			len += F_StrLen(opt_project);
		arg = F_StrNew(len);
		F_StrCpy(arg, "runproj=");
		if (opt_project)
			F_StrCat(arg, opt_project);
		ret = F_ApiCallClient(opt_client, arg);
		F_Free(arg);
	}
	else
		ret = F_ApiCallClient(opt_client, opt_project);

	if (ret == FE_NameNotFound) {
		if (m2g)
			F_Printf(NULL, "Mif2Go m2rbook.dll missing from frame\\fminit\\Plugins\n");
		else
			F_Printf(NULL, "Unrecognized Frame FDK client: %s\n", opt_client);
		return 1;
	}
	else if (ret == 1) { // our own error return
		F_Printf(NULL, "Failure in Frame FDK client: %s\n", opt_client);
		return 1;
	}
	else if (ret != FE_Success) { // undocumented FDK error
		F_Printf(NULL, "Error %d running Mif2Go project\n", ret);
		F_Printf(NULL, "Undocumented FDK error; kindly alert Omni Systems.\n");
		return 1;
	}
	return 0;
}


VoidT CloseFrameFiles(VoidT)
{
	IntT i = 0;
	StringT ckname = NULL;

	if (docId && closeDoc) {
		if (F_ApiClose(docId, FF_CLOSE_MODIFIED) != FE_Success) {
			for (docId = F_ApiGetId(0, FV_SessionId, FP_FirstOpenDoc); docId != 0; 
			     docId = F_ApiGetId(FV_SessionId, docId, FP_NextOpenDocInSession)) {
				ckname = F_ApiGetString(FV_SessionId, docId, FP_Name);
				if (!stricmp(docname, ckname)) {
					F_ApiClose(docId, FF_CLOSE_MODIFIED);
					F_Free(ckname);
					break;
				}
			}
		}
		F_Printf(NULL, "Closed document: %s\n", docname);
	}
	if (bookId && closeBook) {
		F_ApiClose(bookId, FF_CLOSE_MODIFIED);
		F_Printf(NULL, "Closed book: %s\n", bookname);
	}
	if (closeFrame) {
		F_Printf(NULL, "Closing FrameMaker\n");
		AppendToLog(docname ? docname : bookname);
		while ((bookId = F_ApiGetId(0, FV_SessionId, FP_ActiveBook)) != 0)
			F_ApiClose(bookId, FF_CLOSE_MODIFIED);
		while ((docId = F_ApiGetId(0, FV_SessionId, FP_ActiveDoc)) != 0)
			F_ApiClose(docId, FF_CLOSE_MODIFIED);
		F_ApiClose(FV_SessionId, FF_CLOSE_MODIFIED);
	}
}


VoidT AppendToLog(StringT name)
{
	StringT FmHome = NULL;
	StringT ConPathName = NULL;
	FilePathT *ConPath;
	ChannelT ConChan;

	StringT LogPathName = NULL;
	StringT LogPathEnd = NULL;
	FilePathT *LogPath;
	ChannelT LogChan;

	// append consfile.txt to mif2go.log
	if (!name
	 || !(FmHome = F_ApiGetString(0, FV_SessionId, FP_FM_HomeDir)))
		return;  

	LogPathEnd = F_StrRChr(name, '/');
	if (!LogPathEnd)
		LogPathEnd = F_StrRChr(name, '\\');
	if (!LogPathEnd) {
		F_Printf(NULL, "Couldn't determine path to input directory.\n");
		F_Free(FmHome);
		return;
	}
	*LogPathEnd = '\0';
	LogPathName = F_StrNew(F_StrLen(name) + F_StrLen(opt_log) + 2);
	F_StrCpy(LogPathName, name);
	F_StrCat(LogPathName, "\\");
	F_StrCat(LogPathName, opt_log);
	LogPath = F_PathNameToFilePath(LogPathName, NULL, FDefaultPath);

	ConPathName = F_StrNew(F_StrLen(FmHome) + 14);
	F_StrCpy(ConPathName, FmHome);
	F_StrCat(ConPathName, "\\consfile.txt");
	ConPath = F_PathNameToFilePath(ConPathName, NULL, FDefaultPath);

	if ((ConChan = F_ChannelOpen(ConPath, "r")) == NULL) {
		F_Printf(NULL, "Couldn't open FrameMaker console file.\n");
		F_Free(FmHome);
		F_Free(LogPathName);
		F_Free(ConPathName);
		F_FilePathFree(LogPath);
		F_FilePathFree(ConPath);
		return;
	}

	if ((LogChan = F_ChannelOpen(LogPath, "a")) == NULL) {
		F_Printf(NULL, "Couldn't open %s file.\n", opt_log);
		F_Free(FmHome);
		F_Free(LogPathName);
		F_Free(ConPathName);
		F_FilePathFree(LogPath);
		F_FilePathFree(ConPath);
		F_ChannelClose(ConChan);
		return;
	}

	if (F_ChannelAppend(ConChan, LogChan) != FdeSuccess)
		F_Printf(NULL, "Couldn't update %s file.\n", opt_log);

	F_Free(FmHome);
	F_Free(LogPathName);
	F_Free(ConPathName);
	F_FilePathFree(LogPath);
	F_FilePathFree(ConPath);
	F_ChannelClose(LogChan);
	F_ChannelClose(ConChan);
}

// end of runfm.c

