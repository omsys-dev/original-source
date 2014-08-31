/* m2gini.cpp is the Frame FDK ini-file module for mif2go
 * Last edited by Jeremy H. Griffith.
 * Copyright 2005 Omni Systems, Inc.  All Rights Reserved.
 */

#include "m2gframe.h"

DCirfile *M2GFrame::CurrIni = NULL;
DCirfile *M2GFrame::CurrMacros = NULL;
DCirfile *M2GFrame::OrigIni = NULL;
char *M2GFrame::IniName = NULL;
char *M2GFrame::SecIniName = NULL;
char *M2GFrame::MacrosName = NULL;
secinfo *M2GFrame::FDKIniSects = NULL;

unl
M2GFrame::GetArgs(char *chp, char **argv, unl arlim)
{
	unl argc = 0;
	bool quote = false;

	while (*chp) {
		while (*chp && isspace(*chp))
			chp++;
		if (*chp == '"') {
			quote = true;
			argv[argc] = ++chp;
			while (*chp && (*chp != '"'))
				chp++;
		}
		else {
			argv[argc] = chp;
			while (*chp && (*chp != ' '))
				chp++;
		}
		if (*chp)
			*chp++ = '\0';
		if (++argc >= arlim)
			break;
	}

	return argc;
}

void
M2GFrame::SetUpIni(char *chp)
{
	// ininame filename
	// as in:  mif2htm.ini intro
	char *argv[2];
	unl argc = GetArgs(chp, argv, 2);

	IniProcDone();  // for re-init

	IniName = NULL;
	SecIniName = NULL;
	CurrIni = NULL;
	MacrosName = NULL;
	CurrMacros = NULL;

	switch (argc) {
		case 2:  // two args
			SecIniName = NewName(argv[1]);
			//fall through
		case 1:  // one arg
			IniName = NewName(argv[0]);
			break;

		default:
			return;
	}
	if (IniName
	 && *IniName
	 && ((CurrIni = new DCirfile(IniName, SecIniName)) != NULL)) {
		OrigIni = CurrIni;
		CurrIni = CurrIni->SetConfigTemplate("Templates", "Configs");
		if (((CurrIni->Section("Templates")
		   && CurrIni->Find("Macros"))
		  || (CurrIni->Section("Macros")
	 	   && CurrIni->Find("MacroFile")))
		 && (((MacrosName = CurrIni->PathRight()) != NULL)
			&& ((CurrMacros = new DCirfile(MacrosName)) != NULL)))
			CurrMacros = CurrMacros->SetConfigTemplate("Templates", "Macros");
	}
}


void
M2GFrame::GetIniSects(char *chp)
{
	// numeric string version of secinfo *
	unl val = strtoul(chp, NULL, 10);
	FDKIniSects = (secinfo *) val;
	DCini::InitIniModule(CurrIni, FDKIniSects);
	DCini::ProcAllSects();
	SetUpMacros();
}

void
M2GFrame::GetIniGroup(char *chp)
{
	// groupname
	DCini::ProcIniGroup(chp);
}



void
M2GFrame::GetIniSetting(char *chp, bool macro)
{
	// [sect]key to be found in CurrIni template chain
	// = address of char * to set for string result

	if (!IniName || !CurrIni)
		return;

	char *sect = NULL;
	char *key = NULL;
	if (*chp == '[') {
		sect = ++chp;
		while (*chp && (*chp != ']'))
			chp++;
		sect = NewName(sect, chp - sect);
		if (*chp)
			chp++;

	}
	if (*chp) {
		char *equ = strchr(chp, '=');
		if (equ) {
			key = NewName(chp, equ - chp);
			char **val = (char **) strtoul(equ + 1, NULL, 10);
			if (macro) {
				if ((CurrIni->Section(sect)
				  && CurrIni->Find(key))
				 || (CurrMacros->Section(sect)
				  && CurrMacros->Find(key)))
					*val = CurrMacros->StrRight();
			}
			else if ((!strcmp(sect, "FDK")
			  && CurrIni->Section("setup")
			  && CurrIni->Find(key))
			 || (CurrIni->Section(sect)
				&& CurrIni->Find(key)))
				*val = CurrIni->StrRight();
		}
	}
}


void
M2GFrame::GetIniSection(char *chp)
{
	// IniSect [%s]=%ld in CurrIni or CurrMacros template chain
	// = address of char * to set for string result

	if (!IniName || !CurrIni)
		return;

	char *sect = NULL;
	if (*chp == '[') {
		sect = ++chp;
		while (*chp && (*chp != ']'))
			chp++;
		sect = NewName(sect, chp - sect);
		if (*chp)
			chp++;

	}
	if (*chp) {
		char *equ = strchr(chp, '=');
		if (equ) {
			char **val = (char **) strtoul(equ + 1, NULL, 10);
			if (val) {
				if (sect) {
					if (CurrIni->Section(sect))
						*val = CurrIni->SecCont(true);
					else if (CurrMacros->Section(sect))
						*val = CurrMacros->SecCont(true);
				}
				else {  // get all section names with nul separators
					DCnlist *names = CurrIni->SectNames();
					if (names)
						*val = names->concat();
				}
			}
		}
	}
}


void
M2GFrame::GetIniSectList(char *chp)
{
	// IniSectList %ld in CurrIni or CurrMacros template chain
	// = address of char * to set for string result
	if (!IniName || !OrigIni)
		return;
	char **val = (char **) strtoul(chp, NULL, 10);
	if (val) {
		DCnlist *names = OrigIni->SectNames();
		if (names)
			*val = names->concat();
	}
}

void
M2GFrame::IniProcDone(void)
{
	if (CurrIni) {
		CurrIni->CloseFile();
		delete CurrIni;
		CurrIni = NULL;
	}
}


// functions to work with dcfilp



void
M2GFrame::PGetSetting(char *chp)
{
	// "path/name" [sect]key=address of val to be read

	char *fname = NULL;
	if (*chp == '"') {
		fname = ++chp;
		while (*chp && (*chp != '"'))
			chp++;
		fname = NewName(fname, chp - fname);
		if (*chp)
			chp++;
	}
	while (*chp == ' ')
		chp++;

	char *sect = NULL;
	if (*chp == '[') {
		sect = ++chp;
		while (*chp && (*chp != ']'))
			chp++;
		sect = NewName(sect, chp - sect);
		if (*chp)
			chp++;
	}

	char *key = NULL;
	if (*chp) {
		char *equ = strchr(chp, '=');
		if (equ) {
			key = NewName(chp, equ - chp);
			char **val = (char **) strtoul(equ + 1, NULL, 10);
			DCpfile::getval(sect, key,  "", *val, 2048, fname);
		}
	}
}


void
M2GFrame::PPutSetting(char *chp)
{
	// "path/name" [sect]key=val to be set

	char *fname = NULL;
	if (*chp == '"') {
		fname = ++chp;
		while (*chp && (*chp != '"'))
			chp++;
		fname = NewName(fname, chp - fname);
		if (*chp)
			chp++;
	}
	while (*chp == ' ')
		chp++;

	char *sect = NULL;
	if (*chp == '[') {
		sect = ++chp;
		while (*chp && (*chp != ']'))
			chp++;
		sect = NewName(sect, chp - sect);
		if (*chp)
			chp++;
	}

	char *key = NULL;
	if (*chp) {
		char *equ = strchr(chp, '=');
		if (equ) {
			key = NewName(chp, equ - chp);
			char *val = NewName(equ + 1);
			DCpfile::putval(sect, key, val, fname);
		}
	}
}


void
M2GFrame::PRenSect(char *chp)
{
	// "path/name" sect=new name to be set
	bool ret = false;

	char *fname = NULL;
	if (*chp == '"') {
		fname = ++chp;
		while (*chp && (*chp != '"'))
			chp++;
		fname = NewName(fname, chp - fname);
		if (*chp)
			chp++;
	}
	while (*chp == ' ')
		chp++;

	char *sect = NULL;
	char *nsect = NULL;
	if (*chp) {
		char *equ = strchr(chp, '=');
		if (equ) {
			sect = NewName(chp, equ - chp);
			nsect = NewName(equ + 1);
			ret = DCpfile::rensect(sect, nsect, fname);
		}
	}

	if (!ret
	 && UseLog)
		Log->logEvent(logwarn, 1, "Could not rename section: ", chp);

}


void
M2GFrame::PUpdate(char *chp)
{
	// path/name to be closed

	DCpfile::pupdate(chp);
}



void
M2GFrame::PClose(char *chp)
{
	// path/name to be closed

	DCpfile::pclose(chp);
}





// end of m2gini.cpp

