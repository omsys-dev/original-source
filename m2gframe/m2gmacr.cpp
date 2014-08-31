/* m2gmacr.cpp is the Frame FDK macro module for mif2go
 * Last edited by Jeremy H. Griffith.
 * Copyright 2005 Omni Systems, Inc.  All Rights Reserved.
 */

#include "m2gframe.h"

char *M2GFrame::MacroFileName = NULL;
bool M2GFrame::ValidOnly = false;
DCwfile *M2GFrame::MacroTemp = NULL;
long M2GFrame::MacroDiv = 0;
char *M2GFrame::BaseName = NULL;
char *M2GFrame::PrjPath = NULL;
char *M2GFrame::CurrPath = NULL;
unl M2GFrame::MacroDivPtr = 0;

void
M2GFrame::SetUpMacros(void)
{
	MacroProcDone();  // for re-init

	DCmacr::InitPluginMacroModule(CurrIni, SecIniName, MacroFileName,
	 WriteMacroChar, WritePredefinedMacro, GetPredefinedVar,
	 GetFrameUserVar, GetFrameUserArrVar, ValidOnly);

	// restore inimod globals changed by macrmod
	DCini::InitIniModule(CurrIni, FDKIniSects);

	// set up MacroTemp
	if (!MacroTemp) {
		MacroTemp = new DCwfile(NULL, ftxt);  // defaults to stdout
		MacroDiv = MacroTemp->NewDiv();  // but will be used as buffer
	}
}

void
M2GFrame::MacroPreVars(char *chp)
{
	char *argv[3];
	unl argc = GetArgs(chp, argv, 3);

	// numeric ptrs for basename, prjpath, and currpath
	switch (argc) {
		case 3:
			CurrPath = (char *) strtoul(argv[2], NULL, 10);
			//fall through
		case 2:
			PrjPath = (char *) strtoul(argv[1], NULL, 10);
			//fall through
		case 1:
			BaseName = (char *) strtoul(argv[0], NULL, 10);
			break;

		default:
			return;

	}
}


void
M2GFrame::MacroProc(char *chp)
{
	char *argv[3];
	unl argc = GetArgs(chp, argv, 3);

	char *src = NULL;
	char *buf = NULL;
	char **bufptr = NULL;

	// numeric ptrs for source, buf, bufendptr
	switch (argc) {
		case 3:
			bufptr = (char **) strtoul(argv[2], NULL, 10);
			//fall through
		case 2:
			buf = (char *) strtoul(argv[1], NULL, 10);
			//fall through
		case 1:
			src = (char *) strtoul(argv[0], NULL, 10);
			break;

		default:
			return;
	}

	WriteMacro(src, buf, bufptr);
}


// macromod calls


void
M2GFrame::WriteMacro(char *mactxt, char *buf, char **bufptr)
{
	unl len = 0;
	unl blen = (*bufptr - buf);

	if (mactxt) { // request to process
		DCmacr::WriteMacro(MacroTemp, mactxt);
		MacroDivPtr = 0;
	}
	len = (unl) MacroTemp->LenDiv();
	if (len > MacroDivPtr) {
		if ((len - MacroDivPtr) >= blen) {
			memcpy(buf, MacroTemp->ReadDiv() + MacroDivPtr, blen);
			MacroDivPtr += blen;
		}
		else {
			*bufptr = buf + (len - MacroDivPtr); // after last char written
			memcpy(buf, MacroTemp->ReadDiv() + MacroDivPtr, len - MacroDivPtr);
			MacroTemp->ClearDiv();
			MacroDivPtr = 0;
		}
	}
}

char *
M2GFrame::WriteMacroName(char **pmacname, bool point)
{
	// not used yet

	char *txt = NULL;

	DCmacr::WriteMacroName(MacroTemp, pmacname, point);
	if (MacroTemp->LenDiv() > 0) {
		txt = NewName(MacroTemp->ReadDiv(), MacroTemp->LenDiv());
		MacroTemp->ClearDiv();
	}

	return txt;
}

void
M2GFrame::MacroProcDone(void)
{
	if (MacroTemp) {
		MacroTemp->CloseFile();
		delete MacroTemp;
		MacroTemp = NULL;
	}
}


// macromod callbacks

bool
M2GFrame::WriteMacroChar(DCwfile *df, unc uch)
{
	return false;  // bullet not valid in bat files anyway
}


bool
M2GFrame::WritePredefinedMacro(DCwfile *df, char *name)
{
	return false;  // no predefined macros yet
}


char *
M2GFrame::GetPredefinedVar(DCwfile *df, char *vp)
{
	char *ret = NULL;

	if (!stricmp(vp, "BaseName"))
		ret = BaseName;
	else if (!stricmp(vp, "PrjPath"))
		ret = PrjPath;
	else if (!stricmp(vp, "CurrPath"))
		ret = CurrPath;

	if (ret)
		return NewName(ret);

	return NULL;
}


char *
M2GFrame::GetFrameUserVar(char *vp)
{
	return NULL; // no access to Frame vars in doc yet
}


long 
M2GFrame::GetFrameUserArrVar(char *vp, long idx)
{
	return 0;
}


// end of m2gmacr.cpp

