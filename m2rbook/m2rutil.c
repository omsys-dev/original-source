/* m2rutil.c is the conversion utility for mif2rtf for FM 5.5
 * it is used by both the m2r and the m2rbook modules.
 * Last edited on 11/28/98 by Jeremy H. Griffith.
 * Copyright 1998 Omni Systems, Inc.  All Rights Reserved.
 */


#include "m2rbook.h"
#include "htmlhelp.h"


/* globals */

IntT StartTime = 0;
BoolT DocAlreadyOpen = False;



/* doc utility functions */

VoidT SaveDoc(F_ObjHandleT docId, StringT filename, BoolT saveas)
{
	IntT i;
	F_PropValsT saveParams;
	F_PropValsT *saveRetParamsp;

	FA_errno = FE_Success;

	saveParams = F_ApiGetSaveDefaultParams();
	if (saveParams.len == 0)
		return;

	i = F_ApiGetPropIndex(&saveParams, FS_FileType);
	if (FrameVersion >= 12)
			saveParams.val[i].propVal.u.ival = FV_SaveFmtBinary120;
	else if (FrameVersion == 11)
			saveParams.val[i].propVal.u.ival = FV_SaveFmtBinary110;
	else if (FrameVersion == 10)
			saveParams.val[i].propVal.u.ival = FV_SaveFmtBinary100;
	else if (FrameVersion == 9)
			saveParams.val[i].propVal.u.ival = FV_SaveFmtBinary90;
	else if (FrameVersion == 8)
		saveParams.val[i].propVal.u.ival = FV_SaveFmtBinary80;
	else
		saveParams.val[i].propVal.u.ival = FV_SaveFmtBinary;
	i = F_ApiGetPropIndex(&saveParams, FS_RetainNameStripe);
	saveParams.val[i].propVal.u.ival = False;

	if (saveas) {
		//i = F_ApiGetPropIndex(&saveParams, FS_AutoBackupOnSave);
		//saveParams.val[i].propVal.u.ival = FV_SaveNoAutoBackup;
		i = F_ApiGetPropIndex(&saveParams, FS_SaveMode);
		saveParams.val[i].propVal.u.ival = FV_ModeSaveAs;
		i = F_ApiGetPropIndex(&saveParams, FS_SaveAsModeName);
		saveParams.val[i].propVal.u.ival = FV_SaveAsNameProvided;
	}

	saveRetParamsp = NULL;

	F_ApiSave(docId, filename, &saveParams, &saveRetParamsp);

	F_ApiDeallocatePropVals(&saveParams);
	F_ApiDeallocatePropVals(saveRetParamsp);
}

VoidT SaveAsPDF(F_ObjHandleT docId, StringT filename)
{
	IntT i;
	F_PropValsT saveParams;
	F_PropValsT *saveRetParamsp;

	FA_errno = FE_Success;

	saveParams = F_ApiGetSaveDefaultParams();
	if (saveParams.len == 0)
		return;

	i = F_ApiGetPropIndex(&saveParams, FS_FileType);
	saveParams.val[i].propVal.u.ival = FV_SaveFmtPdf;
	i = F_ApiGetPropIndex(&saveParams, FS_RetainNameStripe);
	saveParams.val[i].propVal.u.ival = True;

	i = F_ApiGetPropIndex(&saveParams, FS_AutoBackupOnSave);
	saveParams.val[i].propVal.u.ival = FV_SaveNoAutoBackup;
	i = F_ApiGetPropIndex(&saveParams, FS_SaveMode);
	saveParams.val[i].propVal.u.ival = FV_ModeSaveAs;
	i = F_ApiGetPropIndex(&saveParams, FS_SaveAsModeName);
	saveParams.val[i].propVal.u.ival = FV_SaveAsNameProvided;

	saveRetParamsp = NULL;

	F_ApiSave(docId, filename, &saveParams, &saveRetParamsp);

	F_ApiDeallocatePropVals(&saveParams);
	F_ApiDeallocatePropVals(saveRetParamsp);
}


VoidT ImportDocTemplate(F_ObjHandleT docId, F_ObjHandleT templateId)
{
	IntT flags = 0;

	FA_errno = FE_Success;

	if (AppliedTemplateFlagsSet
	 && (AppliedTemplateFlags != 0)) {
		if (ImportDocPropsSet && ImportDocProps)
			flags = AppliedTemplateFlags | FF_UFF_DOCUMENT_PROPS;
		else
			flags = AppliedTemplateFlags & ~FF_UFF_DOCUMENT_PROPS;
	}
	else {  // use default of everything except dangerous doc props
		flags = FF_UFF_PGF | FF_UFF_FONT | FF_UFF_PAGE | FF_UFF_TABLE |
					  FF_UFF_COND | FF_UFF_COMBINED_FONTS | FF_UFF_REFPAGE |
		        FF_UFF_VAR | FF_UFF_XREF | FF_UFF_COLOR | FF_UFF_MATH |
		        FF_UFF_REMOVE_EXCEPTIONS | FF_UFF_REMOVE_PAGE_BREAKS;
		if (ImportDocPropsSet && ImportDocProps)
			flags |= FF_UFF_DOCUMENT_PROPS;
	}
	ImportTemplateFlags(docId, templateId, flags, True);
}

VoidT ImportTemplateFlags(F_ObjHandleT docId, F_ObjHandleT templateId,
                         IntT flags, BoolT inter)
{
	extern StringT TemplateFail2;
	extern StringT TemplateFail3;
	extern StringT TemplateFail4;
	extern StringT TemplateFail5;

	/* import formats to file and update everything */
	F_ApiSimpleImportFormats(docId, templateId, flags);
	if (FA_errno == FE_AsianSystemRequired) {
		FA_errno = FE_Success;
		F_ApiSimpleImportFormats(docId, templateId, flags & ~FF_UFF_COMBINED_FONTS);
	}
	if (FA_errno != FE_Success) {
		if (inter
		 && F_ApiAlert(TemplateFail2, FF_ALERT_NO_DEFAULT) == -1)
			return;
		FA_errno = FE_Success;
	}
	F_ApiUpdateXRefs(docId, (IntT) (FF_XRUI_EVERYTHING | FF_XRUI_FORCE_UPDATE));
	if (FA_errno != FE_Success) {
		if (inter
		 && F_ApiAlert(TemplateFail3, FF_ALERT_NO_DEFAULT) == -1)
			return;
		FA_errno = FE_Success;
	}
	F_ApiUpdateVariables(docId);
	if (FA_errno != FE_Success) {
		if (inter
		 && F_ApiAlert(TemplateFail4, FF_ALERT_NO_DEFAULT) == -1)
			return;
		FA_errno = FE_Success;
	}
	F_ApiReformat(docId);
	if (FA_errno != FE_Success) {
		if (inter
		 && F_ApiAlert(TemplateFail5, FF_ALERT_NO_DEFAULT) == -1)
			return;
		FA_errno = FE_Success;
	}
}

VoidT RevertDoc(F_ObjHandleT *docIdp)
{
	StringT filename;

	FA_errno = FE_Success;

	filename = F_ApiGetString(FV_SessionId, *docIdp, FP_Name);
	F_ApiClose(*docIdp, FF_CLOSE_MODIFIED);
	*docIdp = OpenDocName(filename);
}


F_ObjHandleT IdentifyDocName(StringT docwanted)
{
	F_ObjHandleT docId = 0;
	StringT docname = NULL;

	FA_errno = FE_Success;

	docId = F_ApiGetId(0, FV_SessionId, FP_FirstOpenDoc);

	while (docId) {
		docname = F_ApiGetString(FV_SessionId, docId, FP_Name);
		if (!F_StrICmp(docwanted, docname))
			break;
		F_StrFree(docname);
		docId = F_ApiGetId(FV_SessionId, docId, FP_NextOpenDocInSession);
	}

  if (docId)
		F_StrFree(docname);
	return docId;
}


F_ObjHandleT IdentifyDocLabel(StringT docwanted)
{
	F_ObjHandleT docId = 0;
	StringT label = NULL;
	StringT docname = NULL;

	FA_errno = FE_Success;

	docId = F_ApiGetId(0, FV_SessionId, FP_FirstOpenDoc);

	while (docId) {
		docname = F_ApiGetString(FV_SessionId, docId, FP_Name);
		if (*docname == 0) {  // must have empty name
			label = F_ApiGetString(FV_SessionId, docId, FP_Label);
			if (!F_StrICmp(docwanted, label)) {
				F_StrFree(label);
				F_StrFree(docname);
				break;
			}
			F_StrFree(label);
		}
		F_StrFree(docname);
		docId = F_ApiGetId(FV_SessionId, docId, FP_NextOpenDocInSession);
	}

	return docId;
}


F_ObjHandleT IdentifyBookOfDoc(StringT docwanted)
{
	F_ObjHandleT bookId = 0;
	F_ObjHandleT docId = 0;
	StringT docname = NULL;

	FA_errno = FE_Success;

	bookId = F_ApiGetId(0, FV_SessionId, FP_FirstOpenBook);

	while (bookId) {
		docId = F_ApiGetId(FV_SessionId, bookId, FP_FirstComponentInBook);
		while (docId) {
			docname = F_ApiGetString(bookId, docId, FP_Name);
			if (!F_StrICmp(docwanted, docname))
				break;
			F_StrFree(docname);
			docname = NULL;
			docId = F_ApiGetId(bookId, docId, FP_NextComponentInBook);
		}
		if (docname)
			break;
		bookId = F_ApiGetId(FV_SessionId, bookId, FP_NextOpenBookInSession);
	}

	if (bookId)
		F_StrFree(docname);
	return bookId;
}


F_ObjHandleT OpenDocName(StringT docwanted)
{
	IntT i;
	F_ObjHandleT docId = 0;
	F_PropValsT openParams;
	F_PropValsT *openRetParamsp;

	FA_errno = FE_Success;
	DocAlreadyOpen = False;

	/* see if already open */
	if ((docId = IdentifyDocName(docwanted)) != 0) {
		DocAlreadyOpen = True;
		return docId;
	}

	/* no, must open it */
	openParams = F_ApiGetOpenDefaultParams();
	if (openParams.len == 0)
		return 0;

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
#if 1
// was ifndef FM50VER
	i = F_ApiGetPropIndex(&openParams, FS_OpenFileNotWritable);
	openParams.val[i].propVal.u.ival = (ProjRemote ? FV_DoOK : FV_DoShowDialog);
#endif
	i = F_ApiGetPropIndex(&openParams, FS_RefFileNotFound);
	openParams.val[i].propVal.u.ival = FV_AllowAllRefFilesUnFindable;
	i = F_ApiGetPropIndex(&openParams, FS_FileIsStructured);
	openParams.val[i].propVal.u.ival = (ProjRemote ? FV_StripStructureAndOpen : FV_DoShowDialog);
	i = F_ApiGetPropIndex(&openParams, FS_UseAutoSaveFile);
	openParams.val[i].propVal.u.ival = (ProjRemote ? FV_DoYes : FV_DoShowDialog);
	i = F_ApiGetPropIndex(&openParams, FS_UseRecoverFile);
	openParams.val[i].propVal.u.ival = (ProjRemote ? FV_DoYes : FV_DoShowDialog);

	i = F_ApiGetPropIndex(&openParams, FS_MakeIconic);
	openParams.val[i].propVal.u.ival = True;
 
	openRetParamsp = NULL;

	docId = F_ApiOpen(docwanted, &openParams, &openRetParamsp);

	F_ApiDeallocatePropVals(&openParams);
	F_ApiDeallocatePropVals(openRetParamsp);

	return docId;
}


//  Save and restore active doc or book

F_ObjHandleT ActiveDocId = 0;
F_ObjHandleT ActiveBookId = 0;


VoidT SaveActiveDocBook(VoidT)
{
	ActiveBookId = F_ApiGetId(0, FV_SessionId, FP_ActiveBook);
	ActiveDocId = F_ApiGetId(0, FV_SessionId, FP_ActiveDoc);
}


VoidT RestoreActiveDocBook(VoidT)
{
	if (ActiveBookId)
		F_ApiSetId(0, FV_SessionId, FP_ActiveBook, ActiveBookId);
	if (ActiveDocId)
		F_ApiSetId(0, FV_SessionId, FP_ActiveDoc, ActiveDocId);

}




/* file utility functions */


VoidT FixPathEnvironment(StringT *path)
{
	StringT envar = NULL;
	StringT varend = NULL;
	StringT varname = NULL;
	StringT enval = NULL;
	StringT newpath = NULL;
	IntT len = 0;

	if (!(envar = F_StrChr(*path, '%'))
	 || !(varend = F_StrChr(envar + 1, '%')))
		return;

	varname = F_StrNew(varend - envar);
	F_StrCpyN(varname, envar + 1, (varend - envar));
	len = F_StrLen(varend);
	if (envar > *path)
		len += (envar - *path);

	if ((enval = getenv(varname)) != NULL) {
		len += F_StrLen(enval);
		newpath = F_StrNew(len);
		if (envar > *path)
			F_StrCpyN(newpath, *path, (envar - *path));
		F_StrCat(newpath, enval);
		F_StrCat(newpath, varend + 1);
		*path = newpath;
	}

	F_StrFree(varname);
}


BoolT PathExists(StringT pathname)
{
	BoolT pathExists = False;
	StringT pn;
	pn = Fm2a(pathname);
#if 0
	IntT pflags;
	FilePathT *fpath;
	StringT pn;

	pn = A2fm(pathname);
	pflags = FF_FilePathDirectory | FF_FilePathExist;
	fpath = F_PathNameToFilePath(pn, NULL, FDefaultPath);
	pathExists = (BoolT) (F_FilePathProperty(fpath, pflags) == pflags);
	F_FilePathFree(fpath);
#else
	pathExists = (BoolT) PathIsDirectory(pn);
	F_Free(pn);
#endif
	return pathExists;
}


BoolT FileExists(StringT filename)
{
	BoolT fileExists = False;
	StringT fn;
	fn = Fm2a(filename);
#if 0
	IntT pflags;
	FilePathT *fpath;
	StringT fn;

	fn = A2fm(filename);
	pflags = FF_FilePathFile | FF_FilePathExist;
	fpath = F_PathNameToFilePath(fn, NULL, FDefaultPath);
	fileExists = (BoolT) (F_FilePathProperty(fpath, pflags) == pflags);
	F_FilePathFree(fpath);
#else
	fileExists = (BoolT) PathFileExists(fn);
	F_Free(fn);
#endif
	return fileExists;
}


VoidT ChangeDir(StringT dir)
{
	StringT cd;

	cd = Fm2a(dir);
	_chdir(cd);
	F_Free(cd);
}

VoidT MakeDir(StringT dir)
{
	StringT command = NULL;
	StringT cd;

	cd = Fm2a(dir);
	command = F_StrNew(F_StrLen(cd) + 9);
	sprintf(command, "mkdir \"%s\"", cd);
	RunDOSCommand(command, True, False);
	F_StrFree(command);
	F_Free(cd);
}

VoidT RemoveDir(StringT dir)
{
	StringT command = NULL;
	StringT cd;

	cd = Fm2a(dir);
	command = F_StrNew(F_StrLen(cd) + 9);
	sprintf(command, "rmdir \"%s\"", cd);
	RunDOSCommand(command, True, False);
	F_StrFree(command);
	F_Free(cd);
}

VoidT DeleteDirContents(StringT dir)
{
	StringT command = NULL;
	StringT cd;

	cd = Fm2a(dir);
	command = F_StrNew(F_StrLen(cd) + 10);
	sprintf(command, "del \"%s\" /q", cd);
	RunDOSCommand(command, True, False);
	F_StrFree(command);
	F_Free(cd);
}


StringT NewFileExt(StringT filename, StringT newext)
{
	StringT extension = NULL;
	StringT newfile = NULL;
	IntT len = 0;

	if ((extension = F_StrRChr(filename, '.')) != NULL)
		len = (extension - filename) + 1;
	else
		len = F_StrLen(filename) + 1;

	newfile = F_StrNew(len + F_StrLen(newext) + 1);
	F_StrCpyN(newfile, filename, len);
	if (newfile[len - 1] != '.')
		F_StrCat(newfile, ".");
	if (*newext == '.')
		newext++;
	F_StrCat(newfile, newext);
	return newfile;
}


StringT AddFileExt(StringT filename, StringT newext)
{
	StringT extension = NULL;
	StringT newfile = NULL;
	IntT len = 0;

	len = F_StrLen(filename) + 1;

	newfile = F_StrNew(len + F_StrLen(newext));
	F_StrCpyN(newfile, filename, len);
	F_StrCat(newfile, newext);
	return newfile;
}


StringT BaseFileName(StringT filename)
{
	StringT pathend = NULL;
	StringT extension = NULL;
	StringT newfile = NULL;
	IntT len = 0;

	len = F_StrLen(filename) + 1;

	if (((pathend = F_StrRChr(filename, '\\')) != NULL)
	 || ((pathend = F_StrRChr(filename, '/')) != NULL))
		len -= (pathend - filename) + 1;

	if ((extension = F_StrRChr(filename, '.')) != NULL)
		len -= F_StrLen(extension);

	newfile = F_StrNew(len);
	F_StrCpyN(newfile, (pathend ? pathend + 1 : filename), len);
	return newfile;
}


IntT SetPathName(StringT *pathp, StringT filename, BoolT endsep)
{
	StringT pathend = NULL;
	StringT newpath = NULL;
	IntT len = 0;

	if (((pathend = F_StrRChr(filename, '\\')) == NULL)
	 && ((pathend = F_StrRChr(filename, '/')) == NULL)) {
		*pathp = "";
		return 0;
	}

	len = (pathend - filename) + 1;
	if (endsep)
		len++;
	*pathp = F_StrNew(len);
	F_StrCpyN(*pathp, filename, len);
	return len;
}


StringT MakeFileName(StringT pathname, StringT filename)
{
	StringT name = NULL;
	UCharT ch = 0;

	name = F_StrNew(F_StrLen(pathname) + F_StrLen(filename) + 2);
	F_StrCpy(name, pathname);
	ch = pathname[F_StrLen(pathname) - 1]; 
	if ((ch != '\\')
	 && (ch != '/')
	 && (*filename != '\\')
	 && (*filename != '/'))
		F_StrCat(name, "\\");
 	F_StrCat(name, filename);

	return name;
}


VoidT SetCurrPath(StringT *fnp)
{
	StringT tfnp = NULL;

	if (fnp == NULL)
		return;

	tfnp = *fnp;
	if (F_StrChr(tfnp, '/')
	 || F_StrChr(tfnp, '\\'))
		return;

	*fnp = MakeFileName(CurrDir, tfnp);
	F_StrFree(tfnp);
}


StringT AddSlash(StringT newpath)
{
	StringT tmpstr = NULL;
	IntT plen = 0;

	if (!newpath)
		return NULL;

	plen = F_StrLen(newpath);
	if ((newpath[plen - 1] != '/')
	 && (newpath[plen - 1] != '\\')) {
		tmpstr = F_StrNew(plen + 2);
		F_StrCpy(tmpstr, newpath);
		tmpstr[plen] = F_StrChr(newpath, '/') ? '/' : '\\';
		//F_StrFree(newpath);
		return tmpstr;
	}
	return newpath;
}


StringT Unquote(StringT newpath)
{
	StringT tmpstr = NULL;
	IntT plen = 0;
	IntT i = 0;

	plen = F_StrLen(newpath);
	for (i = 0; i < plen; i++) {
		if (newpath[i] == '/')
			newpath[i] = '\\';
	}
	if ((newpath[0] == '"')
	 && (newpath[plen - 1] == '"')) {
		tmpstr = F_StrNew(plen - 1);
		F_StrCpyN(tmpstr, newpath + 1, plen - 1);
		plen = F_StrLen(tmpstr);
		if (tmpstr[plen - 1] == '"')
			tmpstr[plen - 1] = '\0';
		return tmpstr;
	}
	return newpath;
}


VoidT RemoveSlash(StringT path)
{
	IntT plen = 0;

	plen = F_StrLen(path);
	if ((path[plen - 1] == '/')
	 || (path[plen - 1] == '\\'))
		path[plen - 1] = '\0'; 
}


BoolT IniExists(StringT pathname, StringT filename)
{
	StringT ininame = NULL;
	BoolT exist = True;

	ininame = MakeFileName(pathname, filename);

	exist = FileExists(ininame);

	F_StrFree(ininame);

	return exist;
}


#if 0

StringT MakeRelPath(StringT afrom, StringT ato)
{
	StringT rel = NULL;
	StringT nf = NULL;
	StringT nt = NULL;
	StringT rel = NULL;
	IntT i = 0;
	IntT slash = 0;

	nf = NormalizePath(afrom);
	nt = NormalizePath(ato);

	// match elems of abs paths afrom and ato until mismatch

	for (i = 0; (nf[i] != '\0') && (nf[i] == nt[i]); i++) {
		if (nf[i] == '/')
			slash = i;
	}

	if (i > 3) {  // same drive letter, proceed
		if (nf[i] == '\0') {  // no more from, so to is sub or same

		}
		else {  // generate ../ for each remaining element in from

		}

		if (nt[i] != '\0') {  // append rest of original path in ato

		}
	}

	F_StrFree(nf);
	F_StrFree(nt);
	return rel;
}

#endif

/* reporting functions */


VoidT ShowStatus(F_ObjHandleT docId, StringT filename, StringT status)
{
	StringT msg;
	StringT pathend;

	if (!filename || !status)
		return;

	if (IsBookFile)
		docId = CurrBookId;

	if (((pathend = F_StrRChr(filename, '\\')) == NULL)
	 && ((pathend = F_StrRChr(filename, '/')) == NULL))
		pathend = filename;
	else pathend++;

	msg = F_StrNew(F_StrLen(pathend) + F_StrLen(status) + 3);
	F_Sprintf(msg, "%s: %s", pathend, status);
	F_ApiSetString(FV_SessionId, docId, FP_StatusLine, msg);
	F_StrFree(msg);
}


VoidT ClearStatus(F_ObjHandleT docId)
{
	if (IsBookFile)
		docId = CurrBookId;
	F_ApiSetString(FV_SessionId, docId, FP_StatusLine, "");
}


VoidT ReportResults(StringT filename)
{
	IntT ltime;
	IntT ptime;
	StringT temp = NULL;

	temp = Fm2a(filename);
	filename = temp;

	/* report results in console window */
	time(&ltime);
	ptime = ltime - StartTime;

	if ((ExitType == 0) 
	 && (ExitCode == 0)) {
		F_ApiReturnValue(0);
		F_Printf(NULL, ReportOK1, (StringT) filename);
		F_Printf(NULL, ReportOK2,
                   (IntT) (ptime / 60), (IntT) (ptime % 60), (StringT) ctime(&ltime));
	}
	else {
		F_ApiReturnValue(1);
		if (ExitType == 0) {
			F_Printf(NULL, ReportFail1, (StringT) filename);
			F_Printf(NULL, ReportFailCode, (IntT) ExitCode, (StringT) ctime(&ltime));
			switch (ExitCode) {
				case 1:
					F_Printf(NULL, ReportFailCode1);
					break;
				case 2:
					F_Printf(NULL, ReportFailCode2);
					break;
				case 3:
					F_Printf(NULL, ReportFailCode3);
					break;
				case 4:
					F_Printf(NULL, ReportFailCode4);
					break;
				case 5:
					F_Printf(NULL, ReportFailCode5);
					break;
				case 6:
					F_Printf(NULL, ReportFailCode6);
					break;
				case 7:
					F_Printf(NULL, ReportFailCode7);
					break;
				case 8:
					F_Printf(NULL, ReportFailCode8);
					break;
				case 9:
					F_Printf(NULL, ReportFailCode9);
					break;
				case 10:
					F_Printf(NULL, ReportFailCode10);
					break;
				case 11:
					F_Printf(NULL, ReportFailCode11);
					break;
				case 12:
					F_Printf(NULL, ReportFailCode12);
					break;
				default:
					break;
			}
			F_Printf(NULL, "\n");
		}
		else {
			F_Printf(NULL, ReportFail1, (StringT) filename);
			F_Printf(NULL, ReportFailTypeCode,
							 (IntT) ExitType, (IntT) ExitCode, (StringT) ctime(&ltime));
			switch (ExitType) {
				case 1:
					F_Printf(NULL, ReportFailType1);
					break;
				case 2:
					F_Printf(NULL, ReportFailType2);
					break;
				case 3:
					F_Printf(NULL, ReportFailType3);
					break;
				case 4:
					F_Printf(NULL, ReportFailType4);
					break;
				case 5:
					F_Printf(NULL, ReportFailType5);
					break;
				case 6:
					F_Printf(NULL, ReportFailType6);
					break;
				case 7:
					F_Printf(NULL, ReportFailType7);
					break;
				default:
					F_Printf(NULL, ReportFailTypeX);
					break;
			}
			if (ExitType == 1) {
				F_Printf(NULL, "\n");
				switch (ExitCode) {   /* from winerror.h */
					case 2:
						F_Printf(NULL, "File not found.");
						break;
					case 3:
					case 53:
						F_Printf(NULL, "Path not found.");
						break;
					case 5:
					case 65:
						F_Printf(NULL, "Access denied.");
						break;
					case 15:
						F_Printf(NULL, "Invalid drive specified.");
						break;
					case 32:
					case 33:
						F_Printf(NULL, "File is unavailable.");
						break;
					case 111:
					case 206:
						F_Printf(NULL, "File name is too long.");
						break;
					case 123:
					case 161:
						F_Printf(NULL, "Invalid file or directory name.");
						break;
					case 190:
					case 191:
					case 192:
					case 193:
						F_Printf(NULL, "Program file is damaged.");
						break;
					case 267:
						F_Printf(NULL, "Invalid directory name, use only letters and digits.");
						break;
					case 1154:
						F_Printf(NULL, "Required DLL is damaged.");
						break;
					case 1157:
						F_Printf(NULL, "Required DLL is missing.");
						break;
					default:
						break;
				}
			}
			F_Printf(NULL, "\n\n");
		}
		if (ExitType == 6)
			F_ApiAlert(UserCancelled, FF_ALERT_CONTINUE_NOTE);
		else if ((ExitType == 0)
		 && (ExitCode == 7)) {
			if (F_ApiAlert(WriteFailed, FF_ALERT_NO_DEFAULT) == -1)
				Quit = True;
		}
		else {
			if (F_ApiAlert(MiscFailure, FF_ALERT_NO_DEFAULT) == -1)
				Quit = True;
		}
	}
	F_Free(temp);
}


StringT GetTopicName(IntT idhval)
{
	switch (idhval) {
		case IDH_ChooseProject: // ChooseProject
			return "ChooseProject.htm";
		case IDH_SetDCL:        // SetDCL
			return "SetDCL.htm";
		case IDH_SetMIF:        // SetMIF
			return "SetMIF.htm";
		case IDH_SetJava:       // SetJava
			return "SetJava.htm";
		case IDH_SetEclipse:    // SetEclipse
			return "SetEclipse.htm";
		case IDH_SetHHelp:      // SetHHelp
			return "SetHHelp.htm";
		case IDH_SetOMHelp:     // SetOMHelp
			return "SetOMHelp.htm";
		case IDH_SetHTML:       // SetHTML
			return "SetHTML.htm";
		case IDH_SetWinH:       // SetWinH
			return "SetWinH.htm";
		case IDH_SetPRTF:       // SetPRTF
			return "SetPRTF.htm";
		case IDH_SetDITA:       // SetDITA
			return "SetDITA.htm";
		case IDH_SetDocBook:    // SetDocBook
			return "SetDocBook.htm";
		case IDH_Export:        // Export
			return "Export.htm";
		case IDH_UserVar:       // UserVar
			return "UserVar.htm";
		default:
			return "ugmif2go.htm";
	}
}



VoidT RunWinHlp(IntT idhval)
{
	//WinHelp(NULL, "UserGd.hlp", HELP_CONTEXT, idhval);
	//HtmlHelp(NULL, "UserGd.chm", HELP_CONTEXT, idhval);
	StringT ohome = getenv("omsyshome");
	StringT path = MakeFileName(ohome, "\\m2g\\usersguide\\ugmif2go.chm");

	HtmlHelp(NULL, path, HH_DISPLAY_TOPIC, (unsigned long) GetTopicName(idhval));
}


/* exec function */


VoidT RunCommand(StringT command, StringT dir, BoolT synch, BoolT hide, BoolT sized)
{
#ifdef WIN32
	LPSTARTUPINFO startInfo;
	PROCESS_INFORMATION *procInfo;
	IntT fmHigh;
	IntT fmWide;
	IntT fmLeft;
	IntT fmTop;
	StringT cd;

	ExitType = 3;

	cd = Fm2a(dir);
	startInfo = (LPSTARTUPINFO) F_Alloc(sizeof(STARTUPINFO), NO_DSE);
	if (startInfo == NULL)
		return;

	F_ClearPtr(startInfo, sizeof(STARTUPINFO));
	startInfo->cb = sizeof(STARTUPINFO);
	startInfo->dwFlags = STARTF_USESHOWWINDOW;
	startInfo->wShowWindow = hide ? SW_HIDE : SW_SHOWNORMAL;

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

	procInfo = (PROCESS_INFORMATION *) F_Alloc(sizeof(PROCESS_INFORMATION), NO_DSE);
	if (procInfo == NULL) {
		F_Free(startInfo);
		return;
	}

	//F_ApiAlert(command, FF_ALERT_CONTINUE_NOTE);

	ExitType = 0;
	
	if (CreateProcess(NULL, command, NULL, NULL, TRUE, 0,
										NULL, cd, startInfo, procInfo)) {
		if (synch) {
			BoolT stop = False;
			while (!stop) {
				UIntT objs = 1;
				UIntT res = 0;
				HANDLE proc[2];
				proc[0] = procInfo->hProcess;
				proc[1] = 0;

				res = MsgWaitForMultipleObjects(objs, proc, False,
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
            if (msg.message == WM_CLOSE)
							continue;
						// otherwise dispatch it 
						TranslateMessage(&msg);
						DispatchMessage(&msg); 
						if (((msg.message == WM_KEYDOWN)
						  && (msg.wParam == VK_CANCEL))
						 || ((msg.message == WM_CHAR)
						  && (msg.wParam == 0x03))) {
							ExitType = 6;
							ExitCode = 1;
							stop = True;
							break;
						}
					} // end of PeekMessage while loop 
				}
			}
			//WaitForSingleObject(procInfo->hProcess, INFINITE);
			if (!stop
			 && !GetExitCodeProcess(procInfo->hProcess, &ExitCode)) {
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

	F_Free(startInfo);
	F_Free(procInfo);
	F_Free(cd);

#else

	/* for Mac and UNIX */
	ExitType = 0;
	if ((ExitCode = system(command)) != FE_Success)
		ExitType = 4;

#endif
}



/* End of m2rutil.c */

