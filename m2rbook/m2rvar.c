/* m2rvar.c is the user variable processor for m2rbook 
 * Copyright 2000-2004 Omni Systems, Inc.  All Rights Reserved.
 */

#include "m2rbook.h"


/* globals */

#define DLG_DV_NAMEBOX 3
#define DLG_DV_PREV 4
#define DLG_DV_PROMPT 5
#define DLG_DV_DEFBOX 6
#define DLG_DV_HELP 8
#define DLG_DV_STOP 9
#define DLG_DV_OK 10
#define DLG_DV_REVERT 11


StringT UserVarList[] = {
 "Never", "Always", "Once",
 NULL };
IntT AskForUserVars = 0;
StringT VarName = NULL;
StringT VarDef = NULL;

#define MAX_VARLIST 1024
static IntT SectItemNum = 0;
static IntT SectItemCount = 0;
static StringT SectItems[MAX_VARLIST];
static StringT SectValues[MAX_VARLIST];
static StringT SectOrigs[MAX_VARLIST];
static BoolT VarUnget = False;
static StringT *SectNextVal = SectValues;
static StringT *SectNextOrig = SectOrigs;
static StringT DefPrompt = "Edit content of variable, or supply new content:";

VoidT ProcUserVars(StringT bkininame)
{
	//IntT userVars = 0;
	StringT varName = NULL;
	StringT varContent = NULL;
	StringT varResponse = NULL;
	StringT varPrompt = NULL;
	StringT bin = NULL;

	bin = Fm2a(bkininame);
	//GetIniMatch(bkininame, "FDK", "AskForUserVars", UserVarList, &AskForUserVars);
	if (AskForUserVars > 1) {  /* AskForUserVars = Never, Always, Once */
		GetIniSect(bin, "UserVars");
		SectItemNum = 0;
		SectItemCount = 0;
		SectNextVal = SectValues;
		SectNextOrig = SectOrigs;

		while ((varName = GetVarSectKey(&varContent)) != NULL) {
			GetIniString(bin, "UserVarPrompts", varName, &varPrompt);
			if (!varContent) {
				GetIniString(bin, "UserVars", varName, &varContent);
				*SectNextVal = *SectNextOrig = varContent;
			}
			if (GetVarDef(&varResponse, varName, varContent, varPrompt) == 0) {
				SetIniStr(bin, "UserVars", varName, varResponse);
				*SectNextVal = varResponse;
			}

			// do not free, stored for possible backup
			varResponse = NULL;
			varName = NULL;
			varContent = NULL;

			F_StrFree(varPrompt);
			varPrompt = NULL;

			if (Quit) 
				return;
		}
		if (AskForUserVars == 3)
			SetIniStr(bin, "BookFileCommands", "AskForUserVars", "Never");
	}
	F_Free(bin);
}


/* replacement for F_ApiPromptString() */

IntT GetVarDef(StringT *response, StringT name, StringT content, StringT prompt)
{
	F_ObjHandleT dlgId = 0;
	F_ObjHandleT itemId = 0;

	dlgId = F_ApiOpenResource(FO_DialogResource, "m2rvar");
	DlgIds[DLG_DEF_VAR - 1] = dlgId;

	SetDlgLabel(dlgId, DLG_DV_NAMEBOX, name);
	SetDlgLabel(dlgId, DLG_DV_PROMPT, prompt ? prompt : DefPrompt);
	SetDlgText(dlgId, DLG_DV_DEFBOX, content);
	EnableDlgItem(dlgId, DLG_DV_PREV, (SectItemNum > 1) ? True : False);

	VarDef = *SectNextOrig;
	VarName = name;
	F_ApiModalDialog(DLG_DEF_VAR, dlgId);
	VarName = NULL;

	if (VarUnget || Quit)
		return 1;

	if (VarDef == NULL)
		return 1;

	*response = VarDef;
	return 0;
}


/* callback used when project dialog is up */

VoidT ProcVarDialogEvent(IntT itemNum)
{
	F_ObjHandleT dlgId = DlgIds[DLG_DEF_VAR - 1];

	switch (itemNum) {
		case DLG_DV_PREV:
			VarUnget = True;
			return;

		case DLG_DV_HELP:
			RunWinHlp(IDH_UserVar);
			break;

		case DLG_DV_STOP:
			Quit = True;
			return;

		case DLG_DV_OK:
			VarDef = GetDlgText(dlgId, DLG_DV_DEFBOX);
			return;

		case DLG_DV_REVERT:
			SetDlgText(dlgId, DLG_DV_DEFBOX, VarDef);
			break;

		default:
			break;
	}
	F_ApiReturnValue(FR_DialogStayUp);
}


StringT GetVarSectKey(StringT *pstr)
{
	StringT str = NULL;

	if (VarUnget) {
		if (SectItemNum > 1)
			SectItemNum -= 2;
		else if (SectItemNum > 0)
			SectItemNum = 0;
		VarUnget = False;
	}

	SectNextVal = &SectValues[SectItemNum];
	SectNextOrig = &SectOrigs[SectItemNum];

	if (SectItemNum < SectItemCount) {  // already stored
		*pstr = SectValues[SectItemNum];
		return SectItems[SectItemNum++];
	}

	if ((str = GetIniSectKey()) == NULL)
		return NULL;

	if (SectItemNum < MAX_VARLIST) {
		SectOrigs[SectItemNum] = NULL;
		SectValues[SectItemNum] = NULL;
		SectItems[SectItemNum] = str;
	}
	
	SectItemCount = ++SectItemNum;

	return str;
}


// new system commands, run in DLL

VoidT RunSystemCommands(BoolT book, BoolT start)
{
	StringT typ = NULL;
	StringT command = NULL;

	if (OldAuto)
//	 || (FormType == PT_MIF))
		return;

	typ = book? "L" : "F";
	command = F_StrNew(F_StrLen(CurrFullForm) + 30);
	F_StrCpy(command, "dcl -f ");
	F_StrCat(command, CurrFullForm);
	F_StrCat(command, typ);

	if (start) { // invoke SystemStartCommand and WrapShipPreproc
		// call drmif with -f suffix of L (libros, book) or F (file)
		F_StrCat(command, " -t dcl StartAuto.mif");
	}
	else {  // invoke SystemWrapCommand, WrapShipProc, and SystemEndCommand
		// call dwrtf or dwhtm with -f suffix of L (libros, book) or F (file)
		F_StrCat(command, " -s dcl EndAuto.dcl");
	}

	RunCommand(command, CurrDir, True, True, False);
	F_StrFree(command);
}


#if 0

VoidT ProcBookCommand(F_ObjHandleT bookId, StringT setting, BoolT start,
                      BoolT hide, BoolT keep, StringT inifile)
{
	StringT command = F_StrNew(1024);
	StringT next = setting;
	StringT pct = NULL;
	StringT bookname = NULL;
	StringT bkbasename = NULL;
	StringT bkpathname = NULL;
	StringT varName = NULL;
	StringT varContent = NULL;
	StringT scan = NULL;

	bookname = F_ApiGetString(FV_SessionId, bookId, FP_Name);
	bkbasename = BaseFileName(bookname);

	F_StrCpy(command, (StringT) getenv("comspec"));
	if (*command == '\0')
		F_StrCpy(command, "command");
	
	F_StrCat(command, keep ? " /k " : " /c ");

	while ((pct = F_StrChr(next, '%')) != NULL) {
		strncat(command, next, pct - next);
		next = pct + 1;
		if (!F_StrICmpN(next, "basename", 8)) {
			F_StrCat(command, bkbasename);
			next = pct + 9;
		}
		else if (!F_StrICmpN(next, "prjpath", 7)) {
			SetPathName(&bkpathname, bookname, False);
			F_StrCat(command, bkpathname);
			F_StrFree(bkpathname);
			bkpathname = NULL;
			next = pct + 8;
		}
		else if (!F_StrICmpN(next, "currpath", 8)) {
			strncat(command, CurrDir,
              F_StrLen(CurrDir) - 1); /* no trailing slash */
			next = pct + 9;
		}
		else {
			while (F_CharIsAlphaNumeric(*next))
				next++;
			if (next > (pct + 1)) {
				varName = F_StrNew(next - pct);
				strncpy(varName, pct + 1, next - (pct + 1));
				GetIniString(inifile, "UserVars", varName, &varContent);
				if (varContent != NULL) {
					for (scan = varContent; *scan != '\0'; scan++) {
						if (*scan == '/')  /* change to DOS path backslash */
							*scan = '\\';
					}
					F_StrCat(command, varContent);
				}
				F_StrFree(varName);
				F_StrFree(varContent);
				varContent = NULL;
			}
		}
	}

	F_StrCat(command, next);

	ShowStatus(bookId, bkbasename, start ? "Running BookStart Command"
																		: "Running BookEnd Command");
	RunCommand(command, CurrDir, True, hide, False);

	F_StrFree(command);
	ClearStatus(bookId);
	F_StrFree(bookname);
	F_StrFree(bkbasename);
}

#endif


BoolT ProcBookCmdHide = False;
BoolT ProcBookCmdKeep = False;
StringT ProcBookCmdName = NULL;
StringT ProcBookCmdBase = NULL;
StringT ProcBookCmdIni = NULL;
StringT ProcFileCmdIni = NULL;
StringT ProcBookCmdBuf = NULL;
StringT ProcBookCmdPtr = NULL;
IntT ProcBookCmdLimit = 250;
IntT ProcBookCmdLevel = 0;
StringT ComSpec = NULL;
//StringT MacroFilePath = NULL;
//StringT *ExtMacros = NULL;
//IntT ExtMacroCount = 0;


VoidT ProcBookCommand(F_ObjHandleT bookId, StringT setting, IntT start,
   BoolT book, BoolT hide, BoolT keep, StringT inifile, StringT specini)
{
	StringT macroBuf;

	ProcBookCmdName = F_ApiGetString(FV_SessionId, bookId, FP_Name);
	ProcBookCmdBase = BaseFileName(ProcBookCmdName);
	ProcBookCmdIni = Fm2a(inifile);
	ProcFileCmdIni = Fm2a(specini);

#if 0
	GetIniPath(ProcBookCmdIni, "Templates", "Macros", &MacroFilePath);
	if (!MacroFilePath)  // check previous locations
		GetIniPath(ProcBookCmdIni, "Macros", "MacroFile", &MacroFilePath);
	if (!MacroFilePath)
		GetIniPath(ProcBookCmdIni, "HTMLOptions", "MacroFile", &MacroFilePath);
#endif

	ShowStatus(bookId, ProcBookCmdBase,
	 (start == BF_START_CMD) ? "Running Start Command" :
	 ((start == BF_WRAP_CMD) ? "Running Wrap Command"	:
	 /* BF_END_CMD */ "Running End Command"));

	ProcBookCmdHide = hide;
	ProcBookCmdKeep = keep;
	ProcBookCmdBuf = F_StrNew(1024);
	*ProcBookCmdBuf = '\0';
	ProcBookCmdPtr = ProcBookCmdBuf;
	ProcBookCmdLimit = 250;  // make larger on NT
	ProcBookCmdLevel = 0;

	SetUpBookCommandBat(start, book);
	macroBuf = ProcMacro(setting); // process with DCmacr
	RunBookCommand(macroBuf);  // can recurse 
	RunBookDOSCommand();      // handle any part left at end
	F_StrFree(macroBuf);

	ClearStatus(bookId);
	F_StrFree(ProcBookCmdName);
	ProcBookCmdName = NULL;
	F_StrFree(ProcBookCmdBase);
	ProcBookCmdBase = NULL;
	F_Free(ProcBookCmdIni);
	ProcBookCmdIni = NULL;
	F_Free(ProcFileCmdIni);
	ProcFileCmdIni = NULL;

#if 0
	if (MacroFilePath) {
		F_StrFree(MacroFilePath);
		MacroFilePath = NULL;
	}
#endif
}


// process with DCmacr

StringT ProcMacro(StringT mactxt)
{
	StringT macroBuf = NULL;
	StringT macroPtr = NULL;
	StringT tmpBuf = NULL;
	StringT arg = NULL;
	IntT bufLen = 2048;
	IntT tmpLen = 0;
	IntT bufPos = 0;
	StringT basename = NULL;
	StringT prjpath = NULL;
	StringT currpath = NULL;

	macroBuf = F_StrNew(bufLen);

	arg = F_StrNew(49);
	basename = ProcBookCmdBase;
	SetPathName(&prjpath, ProcBookCmdName, False);
	currpath = F_StrCopyString(CurrDir);
	currpath[F_StrLen(currpath) - 1] = '\0'; /* no trailing slash */

	sprintf(arg, "MacroPreVars %ld %ld %ld", (UIntT) basename,
	   (UIntT) prjpath, (UIntT) currpath);
	CallM2GFrame(arg);

	macroPtr = macroBuf + (bufLen - 1);
	sprintf(arg, "MacroProc %ld %ld %ld", (UIntT) mactxt, (UIntT) macroBuf,
	        (UIntT) &macroPtr);
	CallM2GFrame(arg);

	while ((macroPtr - macroBuf) == (bufLen - 1)) { // probable overflow, get more
		tmpBuf = macroBuf;
		tmpLen = bufLen;
		bufLen *= 2;
		macroBuf = F_StrNew(bufLen);
		F_StrCpyN(macroBuf + bufPos, tmpBuf, (tmpLen - 1) - bufPos);
		bufPos = (tmpLen - 1);
		F_StrFree(tmpBuf);
		macroPtr = macroBuf + (bufLen - 1);
		sprintf(arg, "MacroProc 0 %ld %ld", (UIntT) macroBuf + bufPos,
					(UIntT) &macroPtr);
		CallM2GFrame(arg);
	}

	F_StrFree(arg);
	F_StrFree(prjpath);
	F_StrFree(currpath);

	return macroBuf;
}


// original process, keep to handle %var form

VoidT RunBookCommand(StringT next)
{
	if ((next == NULL)
	 || (*next == '\0'))
		return;

	if (++ProcBookCmdLevel > 16) {
		F_Printf(NULL, "Command is nested too deep:\n%s\n", (StringT) ProcBookCmdPtr);
		F_Printf(NULL, "Depth is %ld, limit is 16.\n", (IntT) ProcBookCmdLevel);
		if (F_ApiAlert("Command is nested too deep, continue?", FF_ALERT_NO_DEFAULT) == -1)
			Quit = True;
		return;
	}

	CheckComment(&next);

	while (*next) {
		switch (*next) {
#if 0
			case '<':  // start of macro?
				if (*(next + 1) == '$')  // should already be processed
					next = RunBookCmdMacro(next + 2);
				else
					goto normal;
				break;
#endif
			case '%':  // start of old var?
				next = RunBookCmdPercentVar(next + 1);
				break;
			case '\n': // run it
				//RunBookDOSCommand();
				AddToBookCommandBat();
				next++;
				CheckComment(&next);
				break;
#if 0
			normal:
#endif
			default:
				*ProcBookCmdPtr++ = *next++;
				break;
		}
	}

	ProcBookCmdLevel--;
}

VoidT CheckComment(StringT *np)
{
	StringT next = *np;

	while (*next == ';') {
		while (*next && (*next != '\n'))
			next++;
		if (*next)
			next++;
	}

	*np = next;
}


#if 0

StringT RunBookCmdMacro(StringT next)
{
	StringT name = NULL;
	StringT contents = NULL;

	// next points at start of macro name
	if (*next == '$') {  // macro variable
		name = ++next;
		if (*name == '_')  // skip predefined mark if any
			name = ++next;
		while (*next) {
			if (*next == '>') {
				*next = '\0';
				next++;
				break;
			}
			next++;
		}
		// look up name of variable
		if (!F_StrICmp(name, "basename"))
			contents = ProcBookCmdBase;
		else if (!F_StrICmp(name, "prjpath"))
			SetPathName(&contents, ProcBookCmdName, False);
		else if (!F_StrICmp(name, "currpath")) {
			contents = F_StrCopyString(CurrDir);
			contents[F_StrLen(CurrDir) - 1] = '\0'; /* no trailing slash */
		}
		else {
			if (ProcFileCmdIni != NULL) {
				GetRawIniString(ProcFileCmdIni, "MacroVariables", name, &contents);
				if (contents == NULL)
					GetRawIniString(ProcFileCmdIni, "UserVars", name, &contents);
			}
			if (contents == NULL)
				GetRawIniString(ProcBookCmdIni, "MacroVariables", name, &contents);
			if (contents == NULL)
				GetRawIniString(ProcBookCmdIni, "UserVars", name, &contents);
			if (contents == NULL)
				GetRawMacroString("MacroVariables", name, &contents);
		}
	}
	else {  // macro name, look in usual places
		name = next;
		while (*next) {
			if (*next == '>') {
				*next = '\0';
				next++;
				break;
			}
			next++;
		}
		// look up name of macro
		contents = GetMacro(name);
	}

	RunBookCommand(contents);

	// return ptr to char after terminator
	return next;
}

#endif


StringT RunBookCmdPercentVar(StringT next)
{
	StringT var = next;
	StringT varName = NULL;
	StringT varContent = NULL;
	StringT scan = NULL;

	if (!F_StrICmpN(next, "basename", 8)) {
		//RunBookCommand(ProcBookCmdBase);
		next += 8;
	}
	else if (!F_StrICmpN(next, "prjpath", 7)) {
		SetPathName(&varContent, ProcBookCmdName, False);
		RunBookCommand(varContent);
		//F_StrFree(varContent);
		next += 7;
	}
	else if (!F_StrICmpN(next, "currpath", 8)) {
		varContent = F_StrCopyString(CurrDir);
    varContent[F_StrLen(CurrDir) - 1] = '\0'; /* no trailing slash */
		RunBookCommand(varContent);
		//F_StrFree(varContent);
		next += 8;
	}
	else {
		while (F_CharIsAlphaNumeric(*next))
			next++;
		if (next > var) {
			varName = F_StrNew((next - var) + 1);
			strncpy(varName, var, next - var);
			if (ProcFileCmdIni != NULL)
				GetRawIniString(ProcFileCmdIni, "UserVars", varName, &varContent);
			if (varContent == NULL)
				GetRawIniString(ProcBookCmdIni, "UserVars", varName, &varContent);
			if (varContent != NULL) {
				for (scan = varContent; *scan != '\0'; scan++) {
					if (*scan == '/')  /* change to DOS path backslash */
						*scan = '\\';
				}
				RunBookCommand(varContent);
			}
			F_StrFree(varName);
			//F_StrFree(varContent);
		}
	}

	return next;
}

// run book command as a .bat
StringT BookCommandBatName = NULL;
FILE *BookCommandBatFile = NULL;

VoidT SetUpBookCommandBat(IntT start, BoolT book)
{
	BookCommandBatName = F_StrNew(F_StrLen(CurrDir) + 22);
	F_StrCpy(BookCommandBatName, CurrDir);
	F_StrCat(BookCommandBatName, book ? "Book" : "File");
	F_StrCat(BookCommandBatName,
	 (start == BF_START_CMD) ? "Start" :
	 ((start == BF_WRAP_CMD) ? "Wrap" :
	 /* BF_END_CMD */ "End"));
	F_StrCat(BookCommandBatName, "Command.bat");

	if ((BookCommandBatFile = fopen(BookCommandBatName, "wt")) == NULL)
		return;

	fprintf(BookCommandBatFile, "REM For: %s\n", ProcBookCmdName);
	if (!ProcBookCmdHide) {
		fprintf(BookCommandBatFile, "@ECHO Running %s:\n", BookCommandBatName);
		fprintf(BookCommandBatFile, "@ECHO ON\n");
	}
	fflush(BookCommandBatFile);

}

VoidT AddToBookCommandBat(VoidT)
{
	if (ProcBookCmdPtr <= ProcBookCmdBuf)
		return;
	*ProcBookCmdPtr = '\0';

	if (!BookCommandBatFile) { // pass through
		RunBookDOSCommand();
		return;
	}

	fprintf(BookCommandBatFile, "%s\n", ProcBookCmdBuf);
	fflush(BookCommandBatFile);

	ProcBookCmdPtr = ProcBookCmdBuf;
	*ProcBookCmdPtr = '\0';
}

VoidT RunBookDOSCommand(VoidT)
{
	*ProcBookCmdPtr = '\0';

	if (BookCommandBatFile) {
		if (ProcBookCmdPtr > ProcBookCmdBuf)
			AddToBookCommandBat();  // handle any leftover
		if (!ProcBookCmdHide) {
			fprintf(BookCommandBatFile, "@ECHO Finished %s\n", BookCommandBatName);
			if (ProcBookCmdKeep)
				fprintf(BookCommandBatFile, "\n@PAUSE\n");
		}
		fclose(BookCommandBatFile);
		BookCommandBatFile = NULL;
		F_StrCpy(ProcBookCmdBuf, BookCommandBatName);
		if (!ProcBookCmdHide)
			F_StrCat(ProcBookCmdBuf, " | more");
	}
	else if (ProcBookCmdPtr <= ProcBookCmdBuf)
		return;

	if ((ProcBookCmdPtr - ProcBookCmdBuf) > ProcBookCmdLimit) {
		F_Printf(NULL, "Command is too long:\n%s\n", (StringT) ProcBookCmdPtr);
		F_Printf(NULL, "Length is %ld, limit is %ld.\n",
		    (IntT) (ProcBookCmdPtr - ProcBookCmdBuf), (IntT) ProcBookCmdLimit);
		if (F_ApiAlert("Command is too long, continue?", FF_ALERT_NO_DEFAULT) == -1)
			Quit = True;
	}
	else
		RunDOSCommand(ProcBookCmdBuf, ProcBookCmdHide, ProcBookCmdKeep);
	ProcBookCmdPtr = ProcBookCmdBuf;
	*ProcBookCmdPtr = '\0';
}


VoidT RunDOSCommand(StringT cmd, BoolT hide, BoolT keep)
{
	StringT command = NULL;

	if (!cmd
	 || (*cmd == '\0'))
		return;

	if (ComSpec == NULL) {
		ComSpec = (StringT) getenv("comspec");
		if (ComSpec == NULL)
			F_StrCpy(ComSpec, "command");
	}

	command = F_StrNew(F_StrLen(ComSpec) + F_StrLen(cmd) + 5);
	F_StrCpy(command, ComSpec);
	F_StrCat(command, keep ? " /k " : " /c ");
	F_StrCat(command, cmd);

	RunCommand(command, CurrDir, True, hide, False);

	F_StrFree(command);
}

#if 0
VoidT GetMacroFile(VoidT)
{
	IntT count;

	// identify all macros in the file
	ClearExtMacros();

	if (MacroFilePath == NULL)
		return;

	GetIniSect(MacroFilePath, NULL);
	ExtMacroCount = GetIniSectCount();
	ExtMacros = F_Alloc(ExtMacroCount * sizeof(StringT), NO_DSE);
	for (count = 0; count < ExtMacroCount; count++)
		ExtMacros[count] = GetRawIniSectKey();
}


VoidT ClearExtMacros(VoidT)
{
	//IntT count;

	if (ExtMacroCount) {
		//for (count = 0; count < ExtMacroCount; count++)
		//	F_StrFree(ExtMacros[count]);
		//F_Free(ExtMacros);
		ExtMacros = NULL;
		ExtMacroCount = 0;
	}
}
#endif

StringT GetMacro(StringT name)
{
	StringT secname = NULL;
	StringT macro = NULL;
	IntT i = 0;

	if (((macro = GetFullIniSect(ProcFileCmdIni, name)) != NULL)
	 || ((macro = GetFullIniSect(ProcBookCmdIni, name)) != NULL))
		return macro;
	
	return NULL;

#if 0
	// scan section names of main .ini and macro file
	if (ProcFileCmdIni != NULL) {
		GetIniSect(ProcFileCmdIni, NULL);
		while ((secname = GetRawIniSectKey()) != NULL) {
			if (!F_StrICmp(name, secname))
				break;
		}
		if (secname != NULL)
			return GetFullIniSect(ProcFileCmdIni, name);
	}
	GetIniSect(ProcBookCmdIni, NULL);
	while ((secname = GetRawIniSectKey()) != NULL) {
		if (!F_StrICmp(name, secname))
			break;
	}
	if (secname != NULL)
		return GetFullIniSect(ProcBookCmdIni, name);

	if (ExtMacroCount == 0)
		GetMacroFile();
	if (ExtMacroCount != 0) {
		for (i = 0; i < ExtMacroCount; i++) {
			if (!F_StrICmp(name, ExtMacros[i]))
				break;
		}
		if (i < ExtMacroCount)
			return GetFullIniSect(MacroFilePath, name);
	}
	return NULL;		
#endif
}



/* end of m2rvar.c */

