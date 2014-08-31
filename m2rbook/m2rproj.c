/* m2rproj.c is the FDK project dialog handler for mif2rtf FM 5.5  
 * Last edited on 12/1/98 by Jeremy H. Griffith.
 * Copyright 1998 Omni Systems, Inc.  All Rights Reserved.
 */

#include "m2rbook.h"


/* dialog definitions */
IntT DlgIds[DLG_COUNT];

/* globals */
StringT CurrProject = NULL;
StringT CurrProjPath = NULL;
UCharT CurrProjType = '\0';

StringT CurrPrjFilePath = NULL;

/* choose project dialog */
#define DLG_CP_NAMEBOX  2
#define DLG_CP_NAMELIST 3
#define DLG_CP_PATHBOX  5
#define DLG_CP_PATHBTN  6
#define DLG_CP_TYPELIST 8
#define DLG_CP_CFGBTN   9
#define DLG_CP_ADDBTN  10
#define DLG_CP_MODBTN  11
#define DLG_CP_DELBTN  12
#define DLG_CP_HELP    14
#define DLG_CP_OK      15
#define DLG_CP_CANCEL  16

F_ObjHandleT NameBoxId = 0;
F_ObjHandleT NameListId = 0;
F_ObjHandleT PathBoxId = 0;
F_ObjHandleT TypeListId = 0;

F_StringsT dlgNameStrings;
F_StringsT dlgPathStrings;
F_StringsT dlgTypeStrings;

StringT TypeNameList[PROJ_TYPES + 1] = {
	"Available Types",
	"Word 7/95 Print RTF",
	"Word 8/97 Print RTF",
	"WordPerfect Print RTF",
	"WinHelp 3 RTF",
	"WinHelp 4/95 RTF",
	"Standard HTML",
	"JavaHelp",
	"MS HTML Help",
	"OmniHelp",
	"OracleHelp",
	"EclipseHelp",
	"XHTML",
	"DITA",
	"DocBook",
	"Generic XML",
	"Frame MIF only",
	"ASCII DCL only"
//	"PDF",
//	"Multi-project"
};
UCharT TypeIdList[PROJ_TYPES] = {
 'W', '8', 'P',
 '3', 'H',
 'N', 'J', 'M', 'C', 'O', 'E', 'X', 'D', 'B', 'G',
 'F', 'A'
// 'S', 'T'
};

StringT *NameList = NULL;
StringT *PathList = NULL;
UCharT *TypeList = NULL;
IntT ProjAlloc = 0;
IntT ProjCount = 0;
IntT ProjSel = 0;
IntT ProjSelType = PT_WORD8;

BoolT NeedNew = False;
BoolT ProjChanged = False;
BoolT ProjCancelled = False;
BoolT ProjRemote = False;


/* setup project dialog */

VoidT EditProjDialog(StringT filename)
{
	F_ObjHandleT dlgId = 0;
	F_ObjHandleT itemId = 0;
	StringT pathname = NULL;
	StringT projname = NULL;
	StringT nm = NULL;
	StringT fn = NULL;
	StringT pn = NULL;

	IntT i = 0;
	IntT pathlen = 0;
	IntT lastsel = 0;
	FILE *prj;

	StringT temp1;
	StringT temp2;
	StringT nbuf;

	/* get dir of selected file */
	pathlen = SetPathName(&pathname, filename, True);

	/* name.prj has list of project names, dirs, and types */
	projname = NewFileExt(filename, PrjExt);
	//if (CurrPrjFilePath)
		//F_StrFree(CurrPrjFilePath);

	/* pre-allocate for plenty of projects */
	ProjAlloc = 12;

	/* get info from project file in sel file dir */
	if (!FileExists(projname)) {
		temp1 = (StringT) F_Alloc(F_StrLen(pathname)
		                        + F_StrLen(PrjExt) + 7, NO_DSE);
		F_StrCpy(temp1, pathname);
		F_StrCat(temp1, "mif2go");
		F_StrCat(temp1, PrjExt);
		if (FileExists(temp1))
			projname = temp1;
		else
			F_Free(temp1);
	}
	else
		_chmod(projname, _S_IWRITE);

	CurrPrjFilePath = F_StrCopyString(projname);

	//if (CurrFileIDPath)
	//	F_StrFree(CurrFileIDPath);
	CurrFileIDPath = (StringT) F_Alloc(F_StrLen(pathname)
		                      + F_StrLen(IniExt) + 7, NO_DSE);
	F_StrCpy(CurrFileIDPath, pathname);
	F_StrCat(CurrFileIDPath, "mif2go");
	F_StrCat(CurrFileIDPath, IniExt);
	nm = Fm2a(CurrFileIDPath);
	fn = Fm2a(filename);
	if (!FileExists(CurrFileIDPath)) {
		if ((prj = fopen(nm, "w")) != NULL) {
			fprintf(prj, ";DCL ID file for %s\n", fn);
			fprintf(prj, "\n[FDK]\nNextFileID=aa\n");
			fprintf(prj, "\n[FileIDs]\n\n");
			fprintf(prj, "\n[End]\n\n;end of ID file\n\n");
			fclose(prj);
		}
	}
	else
		_chmod(nm, _S_IWRITE);
	F_Free(nm);

	pn = Fm2a(projname);
	if (!FileExists(projname)) { /* recheck */
		NeedNew = True;
		ProjCount = 1;
	}
	else {
		NeedNew = False;
		GetIniInt(pn, "State", "Total", &ProjCount);
		if (ProjCount > 6)
			ProjAlloc = ProjCount * 2;
	}

	NameList = (StringT *) F_Alloc((ProjAlloc + 1) * sizeof(StringT), NO_DSE);
	PathList = (StringT *) F_Alloc(ProjAlloc * sizeof(StringT), NO_DSE);
	TypeList = (UCharT *) F_Alloc(ProjAlloc, NO_DSE);
	ProjSelType = PT_WORD8;
	ProjCancelled = False;
	NameList[0] = "Project Names";
	for (i = 0; i < ProjAlloc; i++) {
		NameList[i + 1] = NULL;
		PathList[i] = NULL;
		TypeList[i] = 0;
	}

	if (NeedNew) {
		ProjSel = 0;
		NameList[1] = BaseFileName(filename); 
		PathList[0] = F_StrCopyString(pathname);
		TypeList[0] = '8';
	}
	else {
		GetIniInt(pn, "State", "Total", &ProjCount);
		GetIniInt(pn, "State", "Last", &ProjSel);
		if (ProjSel)
			ProjSel--;
		for (i = 0; i < ProjCount; i++) {
			GetIniStrInt(pn, "Projects", i + 1, &NameList[i + 1]);
			nm = Fm2a(NameList[i + 1]);
			GetIniString(pn, "Paths", nm, &PathList[i]);
			PathList[i] = AddSlash(PathList[i]); 
			GetIniUC(pn, "Types", nm, &TypeList[i]); 
		}
		for (i = 0; i < PROJ_TYPES; i++) {
			if (TypeList[ProjSel] == TypeIdList[i]) {
				ProjSelType = i + 1;
				break;
			}
		}
	}

	if (pathlen)
		F_StrFree(pathname);

	/* open resource for Choose Project dialog box. */
	dlgId = F_ApiOpenResource(FO_DialogResource, "m2rproj");
	DlgIds[DLG_CHOOSE_PROJECT - 1] = dlgId;

	/* set up pathname of selected project */
	PathBoxId = F_ApiDialogItemId(dlgId, DLG_CP_PATHBOX);
	F_ApiSetString(dlgId, PathBoxId, FP_Text, PathList[ProjSel]);

	/* set up existing project names, bookname if none */
	NameListId = F_ApiDialogItemId(dlgId, DLG_CP_NAMELIST);
	dlgNameStrings.val = NameList;
	dlgNameStrings.len = ProjCount + 1;
	F_ApiSetStrings(dlgId, NameListId, FP_Labels, &dlgNameStrings);
	F_ApiSetInt(dlgId, NameListId, FP_State, ProjSel + 1);	
	NameBoxId = F_ApiDialogItemId(dlgId, DLG_CP_NAMEBOX);
	F_ApiSetString(dlgId, NameBoxId, FP_Text, NameList[ProjSel + 1]);	

	/* set up list of all project types */
	TypeListId = F_ApiDialogItemId(dlgId, DLG_CP_TYPELIST);
	dlgTypeStrings.val = TypeNameList;
	dlgTypeStrings.len = PROJ_TYPES + 1;
	F_ApiSetStrings(dlgId, TypeListId, FP_Labels, &dlgTypeStrings);
	F_ApiSetInt(dlgId, TypeListId, FP_State, ProjSelType);

	EnableDlgItem(dlgId, DLG_CP_DELBTN, (ProjCount < 2) ? False : True);
 
	/* display dialog box. */
	lastsel = ProjSel;
	F_ApiModalDialog(DLG_CHOOSE_PROJECT, dlgId);
	if ((lastsel != ProjSel)
	 && !ProjCancelled)
		ProjChanged = True;

	/* save the selected project props */
	if (!ProjCancelled) {
		if (CurrProject)
			F_StrFree(CurrProject);
		ProjectName = CurrProject = F_StrCopyString(NameList[ProjSel + 1]);
		if (CurrProjPath)
			F_StrFree(CurrProjPath);
		CurrProjPath = F_StrCopyString(PathList[ProjSel]);
		FixPathEnvironment(&CurrProjPath);
		CurrProjType = TypeList[ProjSel];
	}

	/* rewrite proj file if anything was changed */
	if (NeedNew
	 && !ProjCancelled
	 && ((prj = fopen(pn, "w")) != NULL)) {
		fprintf(prj, ProjStartStr, fn);
		fprintf(prj, "\n[State]\nLast=%ld\nTotal=%ld\n", ProjSel + 1, ProjCount);
		/* write [Projects]num=name, [Paths]name=path, [Types]name=ID */
		fprintf(prj, "\n[Projects]\n");
		for (i = 0; i < ProjCount; i++) {
			temp1 = Fm2a(NameList[i + 1]);
			fprintf(prj, "%ld=%s\n", i + 1, temp1);
			F_Free(temp1);
		}
		fprintf(prj, "\n[Paths]\n");
		for (i = 0; i < ProjCount; i++) {
			temp1 = Fm2a(NameList[i + 1]);
			temp2 = Fm2a(PathList[i]);
			fprintf(prj, "%s=%s\n", temp1, temp2);
			F_Free(temp1);
			F_Free(temp2);
		}
		fprintf(prj, "\n[Types]\n");
		for (i = 0; i < ProjCount; i++) {
			temp1 = Fm2a(NameList[i + 1]);
			fprintf(prj, "%s=%c\n", temp1, TypeList[i]);
			F_Free(temp1);
		}
		fprintf(prj, "\n\n\n[End]\n");
		fclose(prj);
	}
	else if (ProjChanged
	 && !ProjCancelled) {
		nbuf = (StringT) F_Alloc(10, NO_DSE);
		SetIniInt(pn, "State", "Last", ProjSel + 1);
		SetIniInt(pn, "State", "Total", ProjCount);
		for (i = 0; i < ProjCount; i++) {
			if (((temp2 = NameList[i + 1]) == NULL)
			 || (*temp2 == '\0'))
				continue;
			sprintf(nbuf, "%ld", i + 1);
			SetIniStr(pn, "Projects", nbuf, temp2);
			temp1 = Fm2a(temp2);
			if (PathList[i])
				SetIniStr(pn, "Paths", temp1, PathList[i]);
			if (TypeList[i]) {
				nbuf[0] = TypeList[i];
				nbuf[1] = 0;
				SetIniStr(pn, "Types", temp1, nbuf);
			}
			F_Free(temp1);
		}
		F_Free(nbuf);
	}

	for (i = 0; i < ProjCount; i++) {
		F_StrFree(NameList[i + 1]);
		F_StrFree(PathList[i]);
	}
	F_Free(NameList);
	F_Free(PathList);
	F_Free(TypeList);
	ProjAlloc = 0;
	F_StrFree(projname);
	F_Free(pn);
	F_Free(fn);
}



/* callback used when project dialog is up */

VoidT ProcProjDialogEvent(IntT itemNum)
{
	F_ObjHandleT dlgId = DlgIds[DLG_CHOOSE_PROJECT - 1];
	StringT pathname = NULL;
	static StringT newpath = NULL;
	static StringT newname = NULL;
	static IntT newtype = 0;
	IntT i = 0;
	IntT err = 0;

#ifdef NORECURSE
	if (GettingProjPath) {
		F_ApiReturnValue(FR_ModalStayUp);
		return;
	}
#endif

	switch (itemNum) {

		case DLG_CP_NAMEBOX:
			newname = F_ApiGetString(dlgId, NameBoxId, FP_Text);
			for (i = 0; i < ProjCount; i++) {
				if (!F_StrICmp(newname, NameList[i + 1])) {
					SelectProject(i);
					if (newname)
						F_StrFree(newname);
					newname = NULL;
					if (newpath)
						F_StrFree(newpath);
					newpath = NULL;
					newtype = 0;
					break;
				}
			}
			break;

		case DLG_CP_NAMELIST:
			if ((i = F_ApiGetInt(dlgId, NameListId, FP_State)) != (ProjSel + 1)) {
				SelectProject(i - 1);
				if (newname)
					F_StrFree(newname);
				newname = NULL;
				if (newpath)
					F_StrFree(newpath);
				newpath = NULL;
				newtype = 0;
			}
			break;

		case DLG_CP_PATHBOX:
			/* prohibit direct changes from typing in box */
			F_ApiSetString(dlgId, PathBoxId, FP_Text, PathList[ProjSel]);
			// newpath = F_ApiGetString(dlgId, PathBoxId, FP_Text);
			break;

		case DLG_CP_PATHBTN:
			GetProjPath(&newpath);
			break;

		case DLG_CP_TYPELIST:
			if ((i = F_ApiGetInt(dlgId, TypeListId, FP_State)) != ProjSelType)
				newtype = i;
			break;

		case DLG_CP_MODBTN:
		case DLG_CP_OK:
			if (newname) {  /* change name if new */
				if (NameList[ProjSel + 1] != NULL)
					F_StrFree(NameList[ProjSel + 1]);
				NameList[ProjSel + 1] = newname;
				ProjChanged = True;
				newname = NULL;
			}
			if (newpath) {
				if ((PathList[ProjSel] != NULL)
				 && (PathList[ProjSel] != newpath))
					F_StrFree(PathList[ProjSel]);
				PathList[ProjSel] = newpath;
				ProjChanged = True;
				newpath = NULL;
			}
			if (newtype) {
				ProjSelType = newtype;
				TypeList[ProjSel] = TypeIdList[ProjSelType - 1];
				ProjChanged = True;
				newtype = 0;
			}
			if (itemNum == DLG_CP_OK) {
				F_ApiClose(dlgId, 0);
				return;
			}
			break;

		case DLG_CP_CFGBTN:
			ConfigureProject();
			break;

		case DLG_CP_ADDBTN:
			if (!newname)  /* only add to project list if name new */
				break;

			AddProject(newname, newpath, newtype);
			newname = NULL;
			newpath = NULL;
			newtype = 0;

			SelectProject(ProjCount - 1);
			ProjChanged = True;
			EnableDlgItem(dlgId, DLG_CP_DELBTN, (ProjCount < 2) ? False : True);
			break;

		case DLG_CP_DELBTN:
			DelProject();

			SelectProject(0);
			ProjChanged = True;
			EnableDlgItem(dlgId, DLG_CP_DELBTN, (ProjCount < 2) ? False : True);

			if (newname)
				F_StrFree(newname);
			newname = NULL;
			if (newpath)
				F_StrFree(newpath);
			newpath = NULL;
			newtype = 0;
			break;

		case DLG_CP_CANCEL:
			F_ApiClose(dlgId, 0);
			/* fall through */
		case FV_DlgClose:
			ProjChanged = NeedNew = False;
			ProjCancelled = True;
			if (newname)
				F_StrFree(newname);
			newname = NULL;
			if (newpath)
				F_StrFree(newpath);
			newpath = NULL;
			return;

		case DLG_CP_HELP:
			RunWinHlp(IDH_ChooseProject);
			break;

		default:
			break;
	}
	F_ApiReturnValue(FR_ModalStayUp);
}


VoidT SelectProject(IntT sel)
{
	F_ObjHandleT dlgId = DlgIds[DLG_CHOOSE_PROJECT - 1];
	IntT i = 0;

	F_ApiSetInt(dlgId, NameListId, FP_State, sel + 1);	
	F_ApiSetString(dlgId, NameBoxId, FP_Text, NameList[sel + 1]);
	F_ApiSetString(dlgId, PathBoxId, FP_Text, PathList[sel]);
	for (i = 0; i < PROJ_TYPES; i++) {
		if (TypeList[sel] == TypeIdList[i]) {
			ProjSelType = i + 1;
			break;
		}
	}
	F_ApiSetInt(dlgId, TypeListId, FP_State, ProjSelType);
	ProjSel = sel;
}


VoidT ConfigureProject(VoidT)
{
	StringT command = NULL;
	StringT path = NULL;
	StringT inipath = NULL;
	StringT ininame = NULL;
	StringT oldininame = NULL;
	IntT form = 0;

	// get ini name for curr project, new or old
	path = PathList[ProjSel];
	form = ProjSelType;
	SetStrings(form);
	oldininame = StdIni;
	FixPathEnvironment(&path);

	ininame = IniNameList[form - 1];
	inipath = MakeFileName(path, ininame);

	if (!FileExists(inipath)) {  // no new ini, try old
		F_StrFree(inipath);
		inipath = MakeFileName(path, oldininame);
	}
	if (!FileExists(inipath)) {  // no old ini either, give up
		F_StrFree(inipath);
		return;
	}

	// run m2gcm for ProjSel
	command = F_StrNew(F_StrLen(inipath) + 10);
	F_StrCpy(command, "m2gcm -s ");
	F_StrCat(command, inipath);
	M2GCloseLog(False);
	RunCommand(command, CurrDir, False, False, False);
	F_StrFree(inipath);
	F_StrFree(command);
}


VoidT AddProject(StringT newname, StringT newpath, IntT newtype)
{
	F_ObjHandleT dlgId = DlgIds[DLG_CHOOSE_PROJECT - 1];

	ProjCount++;

	if (ProjCount > ProjAlloc) {
		ProjAlloc = ProjCount * 2;
		NameList = (StringT *) F_Realloc(NameList, (ProjAlloc + 1) * sizeof(StringT), NO_DSE);
		PathList = (StringT *) F_Realloc(PathList, ProjAlloc * sizeof(StringT), NO_DSE);
		TypeList = (UCharT *) F_Realloc(TypeList, ProjAlloc, NO_DSE);
	}

	NameList[ProjCount] = newname;
	dlgNameStrings.val = NameList;
	dlgNameStrings.len = ProjCount + 1;
	F_ApiSetStrings(dlgId, NameListId, FP_Labels, &dlgNameStrings);

	newname = NULL;

	if (newpath) {
		PathList[ProjCount - 1] = newpath;
		newpath = NULL;
	}
	else
		PathList[ProjCount - 1] = F_StrCopyString(PathList[ProjSel]);

	if (newtype) {
		ProjSelType = newtype;
		TypeList[ProjCount - 1] = TypeIdList[newtype - 1];
	}
	else
		TypeList[ProjCount - 1] = TypeList[ProjSel];

	ProjSel = ProjCount - 1;
	ProjChanged = True;
}


VoidT DelProject(VoidT)
{
	F_ObjHandleT dlgId = DlgIds[DLG_CHOOSE_PROJECT - 1];
	IntT i = 0;

	ProjCount--;

	F_StrFree(NameList[ProjSel + 1]);
	F_StrFree(PathList[ProjSel]);

	for (i = (ProjSel + 1); i < (ProjCount + 1); i++) {
		NameList[i] = NameList[i + 1];
		PathList[i - 1] = PathList[i];
		TypeList[i - 1] = TypeList[i];
	}
	
	NameList[i] = NULL;
	PathList[i - 1] = NULL;
	TypeList[i - 1] = '\0';

	dlgNameStrings.len = ProjCount + 1;
	F_ApiSetStrings(dlgId, NameListId, FP_Labels, &dlgNameStrings);

	ProjSel = 0;

	ProjChanged = True;
}


VoidT GetProjPath(StringT *npp)
{
	F_ObjHandleT dlgId = DlgIds[DLG_CHOOSE_PROJECT - 1];
	StringT pathname = PathList[ProjSel];
	StringT newpath = NULL;
	StringT pathseg = NULL;
	StringT pn = NULL;
	StringT np = NULL;
	IntT err = 0;

#ifdef NORECURSE
	GettingProjPath = True;
#endif

	FixPathEnvironment(&pathname);

	while (!PathExists(pathname)) {
		if ((((pathseg = F_StrRChr(pathname, '\\')) == NULL)
		  && ((pathseg = F_StrRChr(pathname, '/')) == NULL))
		 || (pathseg == &pathname[2])) {
			if (pathname[1] == ':') {
				if (((pathname[2] == '/')
				  || (pathname[2] == '\\'))
				 && (pathname[3] == '\0')) {
					if (pathname[0] == 'C')
						return;
					pathname[0] = 'C';
				}
				else {
					pathname[2] = '/';
					pathname[3] = '\0';
				}
			}
		}
		else *pathseg = '\0';
	}

	// or use shBrowseForFolder()??
#if 0
	err = F_ApiChooseFile(&newpath, ChooseDestDirStr,
						pathname, (StringT) "", FV_ChooseOpenDir, (StringT) "");
#else
	pn = Fm2a(pathname);
	M2GGetOpenDir(ChooseDestDirStr, pn, &newpath);
	newpath = F_StrCopyString(newpath);
	F_Free(pn);
#endif

	if (newpath != NULL) {
		np = A2fm(newpath);
		F_StrFree(newpath);
		np = AddSlash(np);
		if (!F_StrCmp(PathList[ProjSel], np))
			F_Free(np);
		else if (!err) {
			*npp = np;
			F_ApiSetString(dlgId, PathBoxId, FP_Text, np);
		}
	}

#ifdef NORECURSE
	GettingProjPath = False;
#endif
}

#ifdef NORECURSE
BoolT GettingProjPath = False;
#endif


// called with project name by another plugin
// projname prefix is B: for book, D: for doc, or C: for both

BoolT RunfmDiagnostics = False;
BoolT revOn = False;
BoolT revOff = False;
BoolT printBook = False;
BoolT printDoc = False;
BoolT pdfBook = False;
BoolT pdfDoc = False;
BoolT pdfSaveBook = False;
BoolT pdfSaveDoc = False;
BoolT isBook = False;
BoolT isDoc = False;
StringT filename = NULL;
StringT bookname = NULL;
StringT printname = NULL;
StringT pdfname = NULL;
StringT projname = NULL;
F_ObjHandleT bookId = 0;
F_ObjHandleT docId = 0;

StringT ProjSettings[] = {
 "rfinit", "lastfirst",
 "printbook", "printdoc",
 "pdfbook", "pdfdoc",
 "pdfsavebook", "pdfsavedoc",
 "runbook", "rundoc",
 "runtemplate", "runproj",
	NULL
};

setfunc ProjFuncs[] = {
  InitProjSet, LastFirstProjSet,
	PrintBookSet, PrintDocSet,
	PdfBookSet, PdfDocSet,
	PdfSaveBookSet, PdfSaveDocSet,
	RunBookSet, RunDocSet,
	TemplateSet, ProjectSet,
	NULL
};

VoidT ProcProjSet(StringT projset)
{
	StringT equal = NULL;
	StringT chk = NULL;
	UIntT len = 0;
	UIntT i = 0;
	BoolT arg = False;

	if (!projset
	 || !(equal = F_StrChr(projset, '='))
	 || ((len = (equal - projset)) < 5))
		return;

	if (*(equal + 1) != '\0')
		arg = True;

	for (i = 0; (chk = ProjSettings[i]) != NULL; i++) {
		if (F_StrIEqualN(chk, projset, len)) {
			(*(ProjFuncs[i]))(arg ? (equal + 1) : NULL);
			return;
		}
	}
}

VoidT InitProjSet(StringT arg)
{
	revOn = False;
	revOff = False;
	printBook = False;
	printDoc = False;
	pdfBook = False;
	pdfDoc = False;
	pdfSaveBook = False;
	pdfSaveDoc = False;
	isBook = False;
	isDoc = False;
	filename = NULL;
	bookname = NULL;
	printname = NULL;
	pdfname = NULL;
	bookId = 0;
	docId = 0;

	if (F_StrIEqual(arg, "yes"))
		RunfmDiagnostics = True;
	else
		RunfmDiagnostics = False;
}

VoidT LastFirstProjSet(StringT arg)
{
	if (F_StrIEqual(arg, "yes")) {
		revOn = True;
		if (RunfmDiagnostics)
			F_Printf(NULL, "Last Sheet First set On\n");
	}
	else if (F_StrIEqual(arg, "no")) {
		revOff = True;
		if (RunfmDiagnostics)
			F_Printf(NULL, "Last Sheet First set Off\n");
	}
}

VoidT PrintBookSet(StringT arg)
{
	printBook = True;
	if (arg)
		printname = F_StrCopyString(arg);
	if (RunfmDiagnostics) {
		F_Printf(NULL, "Print set for book\n");
		if (arg)
			F_Printf(NULL, "Print destination: %s\n", printname);
	}
}

VoidT PrintDocSet(StringT arg)
{
	printDoc = True;
	if (arg)
		printname = F_StrCopyString(arg);
	if (RunfmDiagnostics) {
		F_Printf(NULL, "Print set for doc\n");
		if (arg)
			F_Printf(NULL, "Print destination: %s\n", printname);
	}
}

VoidT PdfBookSet(StringT arg)
{
	pdfBook = True;
	if (arg)
		pdfname = F_StrCopyString(arg);
	if (RunfmDiagnostics) {
		F_Printf(NULL, "PDF set for book\n");
		if (arg)
			F_Printf(NULL, "PDF destination: %s\n", pdfname);
	}
}

VoidT PdfDocSet(StringT arg)
{
	pdfDoc = True;
	if (arg)
		pdfname = F_StrCopyString(arg);
	if (RunfmDiagnostics) {
		F_Printf(NULL, "PDF set for doc\n");
		if (arg)
			F_Printf(NULL, "PDF destination: %s\n", pdfname);
	}
}

VoidT PdfSaveBookSet(StringT arg)
{
	pdfSaveBook = True;
	pdfSaveDoc = False;
	if (arg)
		pdfname = F_StrCopyString(arg);
	if (RunfmDiagnostics) {
		F_Printf(NULL, "PDF Save set for book\n");
		if (arg)
			F_Printf(NULL, "PDF destination: %s\n", pdfname);
	}
}

VoidT PdfSaveDocSet(StringT arg)
{
	pdfSaveBook = False;
	pdfSaveDoc = True;
	if (arg)
		pdfname = F_StrCopyString(arg);
	if (RunfmDiagnostics) {
		F_Printf(NULL, "PDF Save set for doc\n");
		if (arg)
			F_Printf(NULL, "PDF destination: %s\n", pdfname);
	}
}

VoidT RunBookSet(StringT arg)
{
	if (arg)
		bookname = F_StrCopyString(arg);
	else {
		if (RunfmDiagnostics)
			F_Printf(NULL, "Book name not found.\n");
		return;
	}
	arg = F_StrChr(bookname, '!');
	if (!arg) {
		if (RunfmDiagnostics)
			F_Printf(NULL, "Book ID not found.\n");
		return;
	}
	*arg++ = '\0';
	bookId = F_StrAlphaToInt(arg);
	if (RunfmDiagnostics)
		F_Printf(NULL, "Book is: %s, %d\n", bookname, bookId);
	isBook = True;
}

VoidT RunDocSet(StringT arg)
{
	if (arg)
		filename = F_StrCopyString(arg);
	else {
		if (RunfmDiagnostics)
			F_Printf(NULL, "Document name not found.\n");
		return;
	}
	arg = F_StrChr(filename, '!');
	if (!arg) {
		if (RunfmDiagnostics)
			F_Printf(NULL, "Document ID not found.\n");
		return;
	}
	*arg++ = '\0';
	docId = F_StrAlphaToInt(arg);
	if (RunfmDiagnostics)
		F_Printf(NULL, "Doc is: %s, %d\n", filename, docId);
	isDoc = True;
}

VoidT TemplateSet(StringT arg)
{
}

VoidT ProjectSet(StringT arg)
{
	if (arg)
		projname = F_StrCopyString(arg);
	if (RunfmDiagnostics)
		F_Printf(NULL, "project name: %s\n", arg ? projname : "none");
	ProcProject(docId, projname);
}

VoidT ProcProject(F_ObjHandleT docId, StringT projname)
{
	StringT arg = NULL;
	StringT pathname = NULL;
	StringT projfile = NULL;
	StringT basename = NULL;
	StringT filext = NULL;
	StringT newname = NULL;
	StringT mydocptrstr = NULL;
	StringT mydocstr = NULL;
	StringT pdffile = NULL;
	StringT pdfpath = NULL;
	StringT command = NULL;
	BoolT prjSet = False;
	BoolT revOrig = False;
	IntT pathlen = 0;
	IntT i = 0;
	IntT pause = 0;
	UCharT projtype = '\0';

	F_ApiReturnValue(1);

#if 0
	if (*projname == '*') {
		RunfmDiagnostics = True;
		F_Printf(NULL, "m2rbook: %s\n", ++projname);
	}

	if (*projname == 'R') {
		revOn = True;
		projname++;
		if (RunfmDiagnostics)
			F_Printf(NULL, "Last Sheet First set On\n");
	}

	if (*projname == 'F') {
		revOff = True;
		projname++;
		if (RunfmDiagnostics)
			F_Printf(NULL, "Last Sheet First set Off\n");
	}

	if (*projname == 'P') {
		projname++;
		switch (*projname++) {
			case 'B': // print book
				printBook = True;
				break;
			case 'D': // print doc
				printDoc = True;
				break;
			default:
				break;
		}
		if (RunfmDiagnostics)
			F_Printf(NULL, "Print set for %s\n", (printBook? "book" : "doc"));
		if (*projname == ':') {
			arg = printname = ++projname;
			projname = F_StrChr(arg, '!');
			if (!projname)
				return;
			*projname++ = '\0';
			if (RunfmDiagnostics)
				F_Printf(NULL, "Print destination: %s\n", printname);
		}
		else if (*projname++ != '!')
			return;
	}
	else if (*projname == 'A') {
		projname++;
		switch (*projname++) {
			case 'B': // print book
				pdfBook = True;
				break;
			case 'D': // print doc
				pdfDoc = True;
				break;
			default:
				break;
		}
		if (RunfmDiagnostics)
			F_Printf(NULL, "PDF set for %s\n", (pdfBook? "book" : "doc"));
		if (*projname == ':') {
			arg = pdfname = ++projname;
			projname = F_StrChr(arg, '!');
			if (!projname)
				return;
			*projname++ = '\0';
			if (RunfmDiagnostics)
				F_Printf(NULL, "PDF destination: %s\n", pdfname);
		}
		else if (*projname++ != '!')
			return;
	}

	switch (*projname) {
		case 'B':  // set for running full book
		case 'b':
			isBook = True;
			break;

		case 'D':  // set for running doc standalone
		case 'd':
			isDoc = True;
			break;

		case 'C':  // set for running doc in book
		case 'c':
			isBook = True;
			isDoc = True;
			break;

		default:
			return;
	}

	if (*++projname == ':')
		projname++;
	else
		return;
	arg = projname;
	if (*arg == '!')
		projname = NULL;
	if (RunfmDiagnostics)
		F_Printf(NULL, "project name: %s\n", projname ? projname : "none");

	if (isBook) {
		bookname = F_StrChr(arg, '!');
		if (!bookname) {
			if (RunfmDiagnostics)
				F_Printf(NULL, "Book name not found.\n");
			return;
		}
		*bookname++ = '\0';
		arg = F_StrChr(bookname, '!');
		if (!arg) {
			if (RunfmDiagnostics)
				F_Printf(NULL, "Book ID not found.\n");
			return;
		}
		*arg++ = '\0';
		bookId = F_StrAlphaToInt(arg);
		if (RunfmDiagnostics)
			F_Printf(NULL, "Book is: %s, %d\n", bookname, bookId);
	}

	if (isDoc) {
		filename = F_StrChr(arg, '!');
		if (!filename) {
			if (RunfmDiagnostics)
				F_Printf(NULL, "Document name not found.\n");
			return;
		}
		*filename++ = '\0';
		arg = F_StrChr(filename, '!');
		if (!arg) {
			if (RunfmDiagnostics)
				F_Printf(NULL, "Document ID not found.\n");
			return;
		}
		*arg++ = '\0';
		docId = F_StrAlphaToInt(arg);
		if (RunfmDiagnostics)
			F_Printf(NULL, "Doc is: %s, %d\n", filename, docId);
	}
#endif

	FA_errno = FE_Success;
	if (revOn || revOff) {
		revOrig = F_ApiGetInt(0, (printBook || pdfBook) ? bookId : docId,
		                      FP_PrintLastSheetFirst);
		if (RunfmDiagnostics)
			F_Printf(NULL, "Last Sheet First was %s\n", revOrig ? "On" : "Off");
		F_ApiSetInt(0, (printBook || pdfBook) ? bookId : docId,
		            FP_PrintLastSheetFirst, revOn ? True : False);
	}

	if (printBook) {
		F_Printf(NULL, "Printing: %s\n", bookname);
		if (printname) {
			F_Printf(NULL, "Printing to: %s\n", printname);
			F_ApiSetInt(0, bookId, FP_PrintToFile, True);
			if (RunfmDiagnostics)
				F_Printf(NULL, "PrintToFile set for book.\n");
			F_ApiSetString(0, bookId, FP_PrintFileName, printname);
			if (RunfmDiagnostics)
				F_Printf(NULL, "PrintFileName set for book.\n");
			FA_errno = FE_Success;
		}
		F_ApiSilentPrintDoc(bookId);
	}
	else if (printDoc) {
		F_Printf(NULL, "Printing: %s\n", filename);
		if (printname) {
			F_Printf(NULL, "Printing to: %s\n", printname);
			F_ApiSetInt(0, docId, FP_PrintToFile, True);
			if (RunfmDiagnostics)
				F_Printf(NULL, "PrintToFile set for doc.\n");
			F_ApiSetString(0, docId, FP_PrintFileName, printname);
			if (RunfmDiagnostics)
				F_Printf(NULL, "PrintFileName set for doc.\n");
			FA_errno = FE_Success;
		}
		F_ApiSilentPrintDoc(docId);
	}
	else if (pdfBook) {
		F_Printf(NULL, "Making PDF for: %s\n", bookname);
		if (pdfname)
			F_Printf(NULL, "PDF file: %s\n", pdfname);
		F_ApiSetInt(0, bookId, FP_PrintScope, FV_PR_ALL);
		F_ApiSetInt(0, bookId, FP_PrintToFile, False);
		F_ApiSetInt(0, bookId, FP_PrintThumbnails, False);
		F_ApiSetInt(0, bookId, FP_PrintSeps, False);
		F_ApiSetInt(0, bookId, FP_PrintBlankPages, True);
		F_ApiSetInt(0, bookId, FP_PrintLastSheetFirst, False);
		F_ApiSetInt(0, bookId, FP_PrintNumCopies, 1);
		F_ApiSetInt(0, bookId, FP_PrintOddPages, True);
		F_ApiSetInt(0, bookId, FP_PrintEvenPages, True);
		if (RunfmDiagnostics)
			F_Printf(NULL, "PDF properties set for book.\n");
		FA_errno = FE_Success;
		F_ApiSilentPrintDoc(bookId);
	}
	else if (pdfDoc) {
		F_Printf(NULL, "Making PDF for: %s\n", filename);
		if (pdfname)
			F_Printf(NULL, "PDF file: %s\n", pdfname);
		F_ApiSetInt(0, docId, FP_PrintScope, FV_PR_ALL);
		F_ApiSetInt(0, docId, FP_PrintToFile, False);
		F_ApiSetInt(0, docId, FP_PrintThumbnails, False);
		F_ApiSetInt(0, docId, FP_PrintSeps, False);
		F_ApiSetInt(0, docId, FP_PrintBlankPages, True);
		F_ApiSetInt(0, docId, FP_PrintLastSheetFirst, False);
		F_ApiSetInt(0, docId, FP_PrintNumCopies, 1);
		F_ApiSetInt(0, docId, FP_PrintOddPages, True);
		F_ApiSetInt(0, docId, FP_PrintEvenPages, True);
		if (RunfmDiagnostics)
			F_Printf(NULL, "PDF properties set for doc.\n");
		FA_errno = FE_Success;
		F_ApiSilentPrintDoc(docId);
	}
	else if (pdfSaveBook) {
		F_Printf(NULL, "Saving as PDF for: %s\n", bookname);
		if (pdfname)
			F_Printf(NULL, "PDF file: %s\n", pdfname);
		FA_errno = FE_Success;
		SaveAsPDF(bookId, pdfname);
	}
	else if (pdfSaveDoc) {
		F_Printf(NULL, "Saving as PDF for: %s\n", filename);
		if (pdfname)
			F_Printf(NULL, "PDF file: %s\n", pdfname);
		FA_errno = FE_Success;
		SaveAsPDF(docId, pdfname);
	}
	if (revOn || revOff)
		F_ApiSetInt(0, (printBook || pdfBook) ? bookId : docId,
		            FP_PrintLastSheetFirst, revOrig);

	if (printBook || printDoc) {
		if (RunfmDiagnostics)
			F_Printf(NULL, "Print finished.\n");
		if (FA_errno != FE_Success) {
			F_Printf(NULL, "Printing failed: %s (%d)\n",
				(FA_errno == FE_SystemError) ? "Can't open or close printer." :
				((FA_errno == FE_BadParameter) ? "Print setting incorrect." :
				 "Unknown error."), FA_errno);
			return;
		}
	}
	else if (pdfBook || pdfDoc) {
		if (RunfmDiagnostics)
			F_Printf(NULL, "PDF finished.\n");
		if (FA_errno != FE_Success) {
			F_Printf(NULL, "PDF failed: %s (%d)\n",
				(FA_errno == FE_SystemError) ? "Can't open or close PDF printer." :
				((FA_errno == FE_BadParameter) ? "PDF setting incorrect." :
				 "Unknown error."), FA_errno);
			return;
		}
		if (pdfname) {
			// assume Adobe PDF, so file is in MyDocuments as b/fname.pdf
			pdffile = NewFileExt(BaseFileName(pdfDoc? filename : bookname),
													 PdfExt);
			mydocptrstr = F_StrNew(15);
			M2GGetMyDocDir(mydocptrstr);
			mydocstr = (StringT) F_StrAlphaToInt(mydocptrstr);
			F_StrFree(mydocptrstr);
			pdfpath = MakeFileName(mydocstr, pdffile);
			// wait to run command until PDF is done, really when no file in
			M2GWaitFileDone(mydocstr); // MyDocs is changing length any more
			while (!FileExists(pdfpath) && (pause < 10)) {
				Sleep(1000);
				pause++;
			}
			if (FileExists(pdfpath)) {
				command = F_StrNew(F_StrLen(pdfpath) + F_StrLen(pdfname) + 14);
				sprintf(command, "move /Y \"%s\" \"%s\"", pdfpath, pdfname);
				RunDOSCommand(command, True, False);
				F_StrFree(command);
			}
			else
				F_Printf(NULL, "PDF not found: %s\n", pdfpath);
			// do not free mydocstr, which was allocated by C++ in m2gframe
			F_StrFree(pdfpath);
			F_StrFree(pdffile);
		}
	}

	F_ApiReturnValue(0);
	if (!projname)
		return;
	ProjRemote = True;

	if (isBook) {
		pathlen = SetPathName(&pathname, bookname, True);
		projfile = NewFileExt(bookname, PrjExt);
		if (FileExists(projfile))
			prjSet = True;
		else
			F_Printf(NULL, "Could not locate a project file for %s\n",
				              bookname);
	}

	if (!prjSet
	 && filename) {
		if (pathlen)
			F_StrFree(pathname);
		F_StrFree(projfile);
		pathlen = SetPathName(&pathname, filename, True);
		projfile = NewFileExt(filename, PrjExt);
		if (FileExists(projfile))
			prjSet = True;
		else
			F_Printf(NULL, "Could not locate a project file for %s\n",
				              filename);
	}

	if (!prjSet
	 && pathname
	 && *pathname) {
		projfile = F_StrNew(F_StrLen(pathname) + F_StrLen(PrjExt) + 7);
		F_StrCpy(projfile, pathname);
		F_StrCat(projfile, "mif2go");
		F_StrCat(projfile, PrjExt);
		if (FileExists(projfile))
			prjSet = True;
	}

	if (prjSet)
		F_Printf(NULL, "Using project file: %s\n", projfile);
	else {
		F_Printf(NULL, "Could not locate a project file\n");
		F_ApiReturnValue(1);
		goto AllDone;
	}

	// get info from .prj for projname
	CurrProjPath = NULL;
	GetIniString(projfile, "Paths", projname, &CurrProjPath);
	if (!CurrProjPath) {
		F_Printf(NULL, "Could not locate project: %s\n", projname);
		F_Printf(NULL, " in project file: %s\n", projfile);
		F_ApiReturnValue(1);
		goto AllDone;
	}

	GetIniUC(projfile, "Types", projname, &CurrProjType); 
	ProjSelType = 0;
	for (i = 0; i < PROJ_TYPES; i++) {
		if (TypeIdList[i] == CurrProjType) {
			ProjSelType = i + 1;
			break;
		}
	}
	if (!ProjSelType) {
		F_ApiReturnValue(1);
		goto AllDone;
	}
	CurrProject = F_StrCopyString(projname);

	SetStrings(ProjSelType);
	FormType = ProjSelType;

	if (isDoc) {
		basename = BaseFileName(filename);
		filext = AddFileExt(basename, DestExt);
		newname = MakeFileName(CurrProjPath, filext);
		ProcSaveAs(docId, newname);
		F_StrFree(newname);
		F_StrFree(filext);
		F_StrFree(basename);
	}
	else if (isBook) {
		ProcSaveAsBook(bookId, bookname, CurrProjPath);
	}

	/* clean up */
AllDone:
	if (pathlen)
		F_StrFree(pathname);
	F_StrFree(projfile);
	ProjRemote = False;
}


/* end of m2rproj.c */

