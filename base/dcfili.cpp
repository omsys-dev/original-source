
// dcfili.cc contains code for the dcl ini file classes, JHG
// Copyright (c) 1993-2003 by Omni Systems, Inc.  All rights reserved.

#include "dcglob.h"
#include "dcbase.h"
#include "dclist.h"
#include "dcnode.h"
#include "dcfile.h"
#include "dcfilr.h"
#include "dcfilw.h"
#include "dclog.h"
#include "dcfili.h"

#include <direct.h>
#include <windows.h>
#include <iostream.h>
#include "Shlwapi.h"


#pragma warning( disable : 4244 4514 4710 )

DCvlist DCirfile::StoredLogMsgs;
long DCirfile::StoredLogMsgCount = 0;

logfunc DCirfile::LogEventFunc = NULL;
bool DCirfile::SendingStoredMsgs = false;


void
DCirfile::StoreLogMsg(logtype lt, unc sev, const char *desc,
	const char *d2, const char *d3, const char *d4, const char *d5)
{
	logmsg *lm = new logmsg;
	lm->lt = lt;
	lm->sev = sev;
	lm->desc = desc;
	lm->d2 = d2;
	lm->d3 = d3;
	lm->d4 = d4;
	lm->d5 = d5;

	StoredLogMsgs.add(lm, ++StoredLogMsgCount);
}

void
DCirfile::SendStoredMsgs(void)
{
	if (!LogEventFunc
	 || !StoredLogMsgCount)
		return;

	SendingStoredMsgs = true;
	StoredLogMsgs.all(&SendStoredMsg);
	StoredLogMsgs.empty();
	StoredLogMsgCount = 0;
	SendingStoredMsgs = false;
}

void
DCirfile::SendStoredMsg(void *vp, long i)
{
	if (!i || !vp)
		return;
	logmsg *lm = (logmsg *) vp;
	LogEvent(lm->lt, lm->sev, lm->desc, lm->d2, lm->d3, lm->d4, lm->d5);
}


void
DCirfile::LogEvent(logtype lt, unc sev, const char *desc,
	const char *d2, const char *d3, const char *d4, const char *d5)
{

	if (!LogEventFunc) {
		StoreLogMsg(lt, sev, desc, d2, d3, d4, d5);
		return;
	}
	else if (StoredLogMsgCount
	 && !SendingStoredMsgs)
		SendStoredMsgs();

	LogEventFunc(lt, sev, desc, d2, d3, d4, d5);
}



// DCL ini file read initialization

DCirfile::DCirfile(const char *fname) : DCrfile(fname)
{
  CurrSect = CurrLeft = CurrRight = CurrSectName = NULL;
	CurrPos = 0;
	CurrFile = SecSect = SecNext = SecFound = SecList = false;
	SubType = CurrSubSect = NULL;
	SubSect = SubNext = SubFound = false;
	SecFile = NULL;
	SecName = NULL;

  if (Ferr)
    return;

  if ((BufMax = getSize()) == 0) {
    Ferr = frd;
    return;
  }

	CurrFile = true;
  Buf = new char[BufMax + 2];
  getBlock(Buf, (size_t) BufMax);
  Buf[BufMax] = '\n';
  Buf[BufMax + 1] = '\0';
	SetSectPos();
}

DCirfile::DCirfile(const char *fname, DCfile *bfp) : DCrfile(fname)
{
  CurrSect = CurrLeft = CurrRight = CurrSectName = NULL;
	CurrPos = 0;
	CurrFile = SecSect = SecNext = SecFound = SecList = false;
	SubType = CurrSubSect = NULL;
	SubSect = SubNext = SubFound = false;
	SecFile = NULL;
	SecName = NULL;

	if (bfp) {
		char *sn = bfp->getBaseName();
		SecName = NewName(strlen(sn) + 5);
		strcpy(SecName, sn);
		strcat(SecName, ".ini");
		if (_stricmp(fname, SecName)) { // prevent recursion
			SecFile = new DCirfile(SecName);
			if (SecFile->FErr() != fok) {
				delete SecFile;
				SecFile = NULL;
				DeleteName(SecName);
				SecName = NULL;
			}
		}
		else {
			SecFile = NULL;
			DeleteName(SecName);
			SecName = NULL;
		}
	}

  if (Ferr)
    return;

  if ((BufMax = getSize()) == 0) {
    Ferr = frd;
    return;
  }

	CurrFile = true;
  Buf = new char[BufMax + 2];
  getBlock(Buf, (size_t) BufMax);
  Buf[BufMax] = '\n';
  Buf[BufMax + 1] = '\0';
	SetSectPos();
}


DCirfile::DCirfile(const char *fname, const char *sn) : DCrfile(fname)
{
  CurrSect = CurrLeft = CurrRight = CurrSectName = NULL;
	CurrPos = 0;
	CurrFile = SecSect = SecNext = SecFound = SecList = false;
	SubType = CurrSubSect = NULL;
	SubSect = SubNext = SubFound = false;

	if (sn) {
		char *per = strrchr(sn, '.');
		if (per) {
			SecName = NewName((per - sn) + 5);
			strncpy(SecName, sn, per - sn);
		}
		else {
			SecName = NewName(strlen(sn) + 5);
			strcpy(SecName, sn);
		}
		strcat(SecName, ".ini");
		if (_stricmp(fname, SecName)) { // prevent recursion
			SecFile = new DCirfile(SecName);
			if (SecFile->FErr() != fok) {
				delete SecFile;
				SecFile = NULL;
				DeleteName(SecName);
				SecName = NULL;
			}
		}
		else {
			SecFile = NULL;
			DeleteName(SecName);
			SecName = NULL;
		}
	}
	else {
		SecFile = NULL;
		SecName = NULL;
	}

  if (Ferr)
    return;

  if ((BufMax = getSize()) == 0) {
    Ferr = frd;
    return;
  }

	CurrFile = true;
  Buf = new char[BufMax + 2];
  getBlock(Buf, (size_t) BufMax);
  Buf[BufMax] = '\n';
  Buf[BufMax + 1] = '\0';
	SetSectPos();
}


dc_ferr
DCirfile::FErr()
{
	if (SecFile
	 && (SecFile->FErr() == fok))
		return fok;
	return Ferr;
}


void
DCirfile::CloseFile(void)
{
	if (SecFile)
		SecFile->CloseFile();
	DCfile::CloseFile();
	//if (SectPos)
	//	delete [] SectPos;
	//if (LinePos)
	//	delete [] LinePos;
}


void
DCirfile::SetSectPos(void)
{
	char *cpt = Buf;
	char *bend = Buf + BufMax;
	long scnt = 0;
	long lcnt = 0;
	long i = 0;

	while (cpt < bend) {
		while (*cpt == '\n') {
			lcnt++, cpt++;
			if (*cpt == '\r')
				cpt++;
			if (*cpt == '[')
				scnt++;
		}
		cpt++;
	}

	SectPos = (char **) new char *[scnt];
	for (i = 0; i < scnt; i++)
		SectPos[i] = NULL;

	LinePos = (char **) new char *[lcnt];
	for (i = 0; i < lcnt; i++)
		LinePos[i] = NULL;

	cpt = Buf;
	long spos = 0;
	long lpos = 0;
	while (cpt < bend) {
		while (*cpt == '\n') {
			LinePos[lpos++] = ++cpt;
			if (*cpt == '\r')
				cpt++;
			if (*cpt == '[')
				SectPos[spos++] = cpt + 1;
		}
		cpt++;
	}
	LineIdx = 0;
	LineLim = lpos;
	SectIdx = 0;
	SectLim = spos;
}


char *
DCirfile::GetSectPos(const char *cpt)
{
	if (cpt == SectPos[SectIdx]) {
		if (SectIdx >= (SectLim - 1)) {
			SectIdx = 0;
			return NULL;
		}
		return SectPos[++SectIdx];
	}

	SectIdx = 0;
	while ((SectIdx < SectLim)
	 && (cpt > SectPos[SectIdx]))
		SectIdx++;

	if (SectIdx >= SectLim)
		return NULL;

	return SectPos[SectIdx];
}


char *
DCirfile::GetLinePos(const char *cpt)
{
	if (cpt == LinePos[LineIdx]) {
		if (LineIdx >= (LineLim - 1)) {
			LineIdx = 0;
			return NULL;
		}
		return LinePos[++LineIdx];
	}

	LineIdx = 0;
	while ((LineIdx < LineLim)
	 && (cpt > LinePos[LineIdx]))
		LineIdx++;

	if (LineIdx >= LineLim)
		return NULL;

	return LinePos[LineIdx];
}


char *
DCirfile::GetIniName(char *start, char *fmt, char *flag,
	                   char *endflag, char **oname)
{
	// determine new style ini name based on -f format flag
	bool newform = false;
	bool rtf = false;
	unc cnt = 0;

	char ch = toupper(*fmt);

	if (strlen(fmt) > 2) {
		newform = true;
		if (!strnicmp(fmt, "html", 4))
			ch = 'N', cnt = 4;
		else if (!strnicmp(fmt, "xhtml", 5))
			ch = 'X', cnt = 5;

		else if (!strnicmp(fmt, "htmlhelp", 8))
			ch = 'M', cnt = 8;
		else if (!strnicmp(fmt, "javahelp", 8))
			ch = 'J', cnt = 8;
		else if (!strnicmp(fmt, "oraclehelp", 10))
			ch = 'O', cnt = 10;
		else if (!strnicmp(fmt, "eclipse", 7))
			ch = 'E', cnt = 7;
		else if (!strnicmp(fmt, "omnihelp", 8))
			ch = 'C', cnt = 8;

		else if (!strnicmp(fmt, "xml", 3))
			ch = 'G', cnt = 3;
		else if (!strnicmp(fmt, "docbook", 7))
			ch = 'B', cnt = 7;
		else if (!strnicmp(fmt, "dita", 4))
			ch = 'D', cnt = 4;
		else if (!strnicmp(fmt, "udoc", 4))
			ch = 'U', cnt = 4;

		else if (!strnicmp(fmt, "dcl", 3))
			ch = 'A', cnt = 3;
		else if (!strnicmp(fmt, "mif", 3))
			ch = 'F', cnt = 3;

		else if (!strnicmp(fmt, "rtf", 3))
			ch = '8', cnt = 3, rtf = true;
		else if (!strnicmp(fmt, "wordperfect", 11))
			ch = 'P', cnt = 11, rtf = true;
		else if (!strnicmp(fmt, "word7", 5))
			ch = '7', cnt = 5, rtf = true;
		else if (!strnicmp(fmt, "word8", 5))
			ch = '8', cnt = 5, rtf = true;
		else if (!strnicmp(fmt, "word", 4))
			ch = 'W', cnt = 4, rtf = true;
		else if (!strnicmp(fmt, "winhelp3", 8))
			ch = '3', cnt = 8, rtf = true;
		else if (!strnicmp(fmt, "winhelp4", 8))
			ch = '4', cnt = 8, rtf = true;
		else if (!strnicmp(fmt, "winhelp", 7))
			ch = 'H', cnt = 7, rtf = true;

		else
			ch = 'Z', cnt = 4; // treat as html
	}

	char *nm = NewName(start, strlen(start) + (cnt ? (cnt + 5) : 20));

	switch (ch) {
		case 'N':
			strcat(nm, "html");
			break;
		case 'X':  // XHTML
			strcat(nm, "xhtml");
			break;

		case 'J':  // JavaHelp
			strcat(nm, "javahelp");
			break;
		case 'O':  // OracleHelp
			strcat(nm, "oraclehelp");
			break;
		case 'E':  // Eclipse
			strcat(nm, "eclipse");
			break;
		case 'M':  // MS HTML Help
			strcat(nm, "htmlhelp");
			break;
		case 'C':  // OmniHelp
			strcat(nm, "omnihelp");
			break;

		case 'G':  // XML
			strcat(nm, "xml");
			break;
		case 'B':  // DocBook XML
			strcat(nm, "docbook");
			break;
		case 'D':  // DITA
			strcat(nm, "dita");
			break;
		case 'U':  // uDoc
			strcat(nm, "udoc");
			break;

		case 'A':  // DCL
			strcat(nm, "dcl");
			if (DCrfile::FileExists("mif2rtf.ini"))
				rtf = true;
			break;
		case 'F':  // MIF
			strcat(nm, "mif");
			break;

		case 'W':  // Word
		case '8':  // RTF, Word8
		case '7':  // Word7
		case 'P':  // WordPerfect
			rtf = true;
			strcat(nm, "rtf");
			break;
		case 'H':  // WinHelp
		case '3':  // WinHelp3
		case '4':  // WinHelp4
			rtf = true;
			strcat(nm, "winhelp");
			break;
		default:  // unknown
			strcat(nm, "html");
			ch = 'Z';
			break;
	}
	strcat(nm, ".ini");

	// pass back single-char form
	*flag = ch;

	// second char specifies list type
	if (endflag
	 && ((newform
	   && cnt
	   && (ch != 'Z')
	   && ((ch = *(fmt + cnt)) != '\0'))
	  || (!newform
	   && ((ch = *(fmt + 1)) != '\0'))))
			*endflag = toupper(ch);

	if (oname)
		*oname =  rtf ? "mif2rtf.ini" : "mif2htm.ini";

	return nm;
}



DCnlist DCirfile::IniChainList;
//DCvlist IniFileList;
//DCnlist IniNameList;
//long IniLevel = 0;
char *DCirfile::LastConfigSetting = NULL;
bool DCirfile::LogIniChain = false;

void DCirfile::WriteIniChain(const char *setting)
{
	SendStoredMsgs();

	if (!LogEventFunc
	 || !IniChainList.next)
		return;
	if (LogIniChain) {
		LogEvent(loginfo, 1, "Ini chain for ", setting ? setting : "unknown", ":");
		IniChainList.all(&WriteIniChainItem);
	}
	//IniChainList.empty();
}


void DCirfile::WriteIniChainItem(char *path, long i)
{
	if (!i || !path)
		return;

	LogEvent(loginfo, 1, "  ", path);
}


DCirfile *
DCirfile::SetConfigTemplate(const char *sect, const char *setting,
		                        bool link)
{
	DCirfile *bottomIni = SecFile ? SecFile : this;
	bool secfound = false;
	char *startdir =  _getcwd(NewName(1024), 1024);
	char *currdir = startdir;
	char *newdir = NULL;
	int startdrive = _getdrive();
	int currdrive = startdrive;
	int newdrive = 0;

	IniLevel = 0;

	if (!LogEventFunc)
		LastConfigSetting = NewName(setting);

	if (SecFile != NULL) {
		IniFileList.add(SecFile, ++IniLevel);
		IniNameList.add(SecFile->getBaseName(), IniLevel);
		IniChainList.add(SecFile->getFilePath(), IniLevel);
	}
	IniFileList.add(this, ++IniLevel);
	IniNameList.add(getBaseName(), IniLevel);
	IniChainList.add(getFilePath(), IniLevel);

	DCirfile *configTpl = NULL;
	char *configTplName = NULL;

	// SecFile (specific ini) overrides tpl choice in this
	DCirfile *topIni = this;
	secfound = Section(sect);

	if (secfound
	 && Find("Scope"))
		Scope = StrRight();

	if (secfound
	 && !stricmp(setting, "configs")
	 && Find("Document")) {   // insert document chain here
		DCirfile *topdoc = this;
		while (topdoc->Section(sect)) {
			if (!topdoc->Find("Document"))
				break;  // end of chain with no end flag ****
			char *nm = topdoc->StrRight();
			if (!nm)
				break;  // end of chain with no end flag ****
			if (!stricmp(nm, "end")) {
				DeleteName(nm);
				topdoc->ChainEnded = true;
				break;  // end of chain with end flag
			}
			DeleteName(nm);
			if ((configTplName = topdoc->PathRight()) == NULL)
				break;  // end of chain with no end flag ****
			if ((configTpl = new DCirfile(configTplName)) == NULL) {
				LogEvent(logwarn, 1, topdoc->getFilePath(), " refers to unopenable ",
					"Document template", " ", NewName(configTplName));
				DeleteName(configTplName);
				configTplName = NULL;
				break;
			}
			if (configTpl->FErr() != fok) {
				LogEvent(logwarn, 1, topdoc->getFilePath(), " refers to nonexistent ",
					"Document template", " ", NewName(configTpl->getFilePath()));
				DeleteName(configTplName);
				configTplName = NULL;
				delete configTpl;
				break;  // file not found
			}
			newdir = configTpl->getBasePath();
			if (*(newdir + 1) == ':')
				newdrive = (toupper(*newdir) - 'A') + 1;
			if (newdrive
			 && (currdrive != newdrive))
				_chdrive(currdrive = newdrive);
			if (stricmp(newdir, currdir))
				_chdir(currdir = newdir);

			if (IniNameList(configTpl->getBaseName())) {
				LogEvent(logwarn, 1, topdoc->getFilePath(), " repeats ",
					"Document template", " ", NewName(configTpl->getFilePath()));
				DeleteName(configTplName);
				configTplName = NULL;
				delete configTpl;
				break;  // deja vu, end of the line
			}

			topdoc = configTpl;  // step up
			if (topdoc->Section("Templates")
			 && topdoc->Find("Scope"))
				topdoc->Scope = topdoc->StrRight();
			DeleteName(configTplName);
			configTplName = NULL;
			IniFileList.add(topdoc, ++IniLevel);
			IniNameList.add(topdoc->getBaseName(), IniLevel);
			IniChainList.add(topdoc->getFilePath(), IniLevel);
		}
	}

	if (!secfound
	 || !Find(setting)) {  // check for old setting
		if (!stricmp(setting, "configs")) {
			if (Section("Setup")
			 && Find("ConfigTemplate")) {
				sect = "Setup";
				setting = "ConfigTemplate";
			}
			else if (Section("FDK")
			 && Find("ConfigTemplate")) {
				sect = "FDK";
				setting = "ConfigTemplate";
			}
		}
		else if (!stricmp(setting, "languages")) {
			if (Section("Setup")
			 && Find("LanguageText"))
				sect = "Setup";
				setting = "LanguageText";
		}
		else if (!stricmp(setting, "macros")) {
			if (Section("Macros")
			 && Find("MacroFile")) {
				sect = "Macros";
				setting = "MacroFile";
			}
		}
	}

	while (topIni->Section(sect)) {
		if (!topIni->Find(setting))
			break;  // end of chain with no end flag ****
		char *nm = topIni->StrRight();
		if (!nm)
			break;  // end of chain with no end flag ****
		if (!stricmp(nm, "end")) {
			DeleteName(nm);
			topIni->ChainEnded = true;
			break;  // end of chain with end flag
		}
		DeleteName(nm);
		if ((configTplName = topIni->PathRight()) == NULL)
			break;  // end of chain with no end flag ****
		if ((configTpl = new DCirfile(configTplName)) == NULL) {
			LogEvent(logwarn, 1, topIni->getFilePath(), " refers to unopenable ",
				setting, " ", NewName(configTplName));
			DeleteName(configTplName);
			configTplName = NULL;
			break;  // can't open file
		}
		if (configTpl->FErr() != fok) {
			LogEvent(logwarn, 1, topIni->getFilePath(), " refers to nonexistent ",
				setting, " ", NewName(configTpl->getFilePath()));
			DeleteName(configTplName);
			configTplName = NULL;
			delete configTpl;
			break;  // file not found
		}
		newdir = configTpl->getBasePath();
		if (*(newdir + 1) == ':')
			newdrive = (toupper(*newdir) - 'A') + 1;
		if (newdrive
		 && (currdrive != newdrive))
			_chdrive(currdrive = newdrive);
		if (stricmp(newdir, currdir))
			_chdir(currdir = newdir);

		if (IniNameList(configTpl->getBaseName())) {
			LogEvent(logwarn, 1, topIni->getFilePath(), " repeats ",
				setting, " ", NewName(configTpl->getFilePath()));
			DeleteName(configTplName);
			configTplName = NULL;
			delete configTpl;
			break;  // deja vu, end of the line
		}

		topIni = configTpl;  // step up
		if (topIni->Section("Templates")
		 && topIni->Find("Scope"))
			topIni->Scope = topIni->StrRight();
		DeleteName(configTplName);
		configTplName = NULL;
		IniFileList.add(topIni, ++IniLevel);
		IniNameList.add(topIni->getBaseName(), IniLevel);
		IniChainList.add(topIni->getFilePath(), IniLevel);
	}

	// topIni is now at the top of the chain, and
	// bottomIni is at the bottom, which may be same.
	if (link) {
		DCirfile *ifl = bottomIni;
		DCirfile *ift = NULL;
		for (long ilev = 1; ilev < IniLevel; ilev++) {
			ift = (DCirfile *) IniFileList.find(ilev + 1);
			if (!ift)
				break;
			ift->SetSecFile(ifl);
			ifl = ift;
			//if (ifl == topIni)
			//	break;
		}
	}

	WriteIniChain(setting);

	if (currdrive != startdrive)
		_chdrive(startdrive);
	if (stricmp(startdir, currdir))
		_chdir(startdir);

	if (this != topIni) {
		topIni->IniFileList = IniFileList;
		topIni->IniNameList = IniNameList;
		topIni->IniChainList = IniChainList;
		topIni->IniLevel = IniLevel;
	}

	return topIni;
}


void 
DCirfile::CloseIniChain(void)
{
	long cnt = IniFileList.count();
	if (!cnt)
		return;

	DCirfile *iptr = NULL;

	while (cnt) {
		iptr = (DCirfile *) IniFileList.dequeue();
		cnt--;
		if (iptr != this) {
			iptr->CloseFile();
			delete iptr;
		}
	}
	CloseFile();
}


void
DCirfile::SetType(const char *id)
{
	SubType = id;
	if (SecFile)
		SecFile->SetType(id);
}


bool
DCirfile::Section(const char *name)
{
	if (!name)
		return false;

  CurrSect = CurrSubSect = NULL;
	CurrPos = 0;
  CurrLeft = Buf;
	if (CurrSectName
	 && stricmp(CurrSectName, name)) {
		DeleteName(CurrSectName);
		CurrSectName = NULL;
	}
	if (!CurrSectName)
		CurrSectName = NewName(name);
  uns len = strlen(name);
	uns sublen = 0;

	SubSect = SubNext = SubFound = false;

	if (SecFile
	 && ((SecSect = SecFile->Section(name)) == true))
		SecNext = true;
	SecFound = false;

	if (CurrFile == false)
		return SecSect;

  // scan for section [name]
  // if not found by EOF, return false

	while (CurrLeft && *CurrLeft != '\0') {
		CurrLeft = GetSectPos(CurrLeft);
    if (CurrLeft
     && !_strnicmp(CurrLeft, name, len)) {  // match
      if (*(CurrLeft + len) == ']') {
				if (!CurrSect)
					CurrSect = CurrLeft;
				if (!SubType)
					break;
			}
			else if ((*(CurrLeft + len) == '-')  // check sub
			 && (SubType != NULL)
			 && ((sublen = strlen(SubType)) != 0)
			 && !_strnicmp(CurrLeft + len + 1, SubType, sublen)
			 && (*(CurrLeft + len + sublen + 1) == ']')) {
				CurrSubSect = CurrLeft;
				SubSect = SubNext = true;
				SubFound = false;
				break;
			}
		}
    //skipLine();
  }

  CurrLeft = CurrRight = NULL;
  LeftLen = RightLen = 0;
	if (CurrSect || CurrSubSect)
		return true;
  return SecSect;
}

char *
DCirfile::SecCont(bool secfirst)
{
	if (SecFound)
		return SecFile->StrRight();

	if (secfirst
	 && SecSect
	 && SecNext)
		return SecFile->SecCont(true);

	if (CurrFile == false)
		return NULL;

	if (CurrSubSect)
		return SecContStr(CurrSubSect);

	if (CurrSect)
		return SecContStr(CurrSect);

	return NULL;
}


char *
DCirfile::SecContStr(const char *sect)
{
	//CurrLeft = sect;
	CurrLeft = GetLinePos(sect);
	if (!CurrLeft)
		return NULL;
	//skipLine(); // skip section title
	const char *cstart = CurrLeft;
	CurrLeft = GetSectPos(sect);
	if (!CurrLeft)
		return NULL;
	//while ((*CurrLeft != '\0') && (*CurrLeft != '['))
	//	skipLine();
	return NewName(cstart, (CurrLeft - cstart) - 1);
}

DCnlist *
DCirfile::SectNames(void)
{
	DCnlist *names = new DCnlist();
	char *nbuf = NULL;
	char *bptr = NULL;
	char *pos = NULL;
	long cnt = 0;

	// possibly later:
	// get all section names in file as nul-separated list
	// with double-nul terminator as in GetPrivateProfileString

	
  CurrLeft = Buf;
	while (CurrLeft && *CurrLeft != '\0') {
		if ((CurrLeft = GetSectPos(CurrLeft)) == NULL)
			break;
		pos = strchr(CurrLeft, ']');
		if (!pos
		 || (pos < CurrLeft))
			continue;
		nbuf = bptr = NewName(pos - CurrLeft);
		while (CurrLeft && *CurrLeft && (*CurrLeft != ']'))
			*bptr++ = *CurrLeft++;
		*bptr = '\0';
		names->addnew(nbuf, ++cnt);
	}
  CurrLeft = CurrRight = NULL;
  LeftLen = RightLen = 0;

	if (!cnt) {
		delete names;
		names = NULL;
	}

	return names;
}


bool
DCirfile::NextItem(void)
{
	if (SecFile
	 && SecSect) {
		if ((SecNext = SecFile->NextItem()) == true)
			return (SecFound = true);
		else
			SecSect = SecFound = false;
	}

	if (CurrFile == false)
		return false;

  // scan for next non-comment line
  // if none before end of section, return false

	if (CurrLeft)
		return NextItemStr(CurrLeft);

	if (SubNext
	 && CurrSubSect
	 && (NextItemStr(CurrSubSect) == true))
		return true;

	if (CurrSect)
		return NextItemStr(CurrSect);

	return false;
}


long
DCirfile::GetItemCount(void)
{
	long count = 0;

	while (NextItem())
		count++;

	Section(CurrSectName);

	return count;
}


bool
DCirfile::NextItemStr(const char *pos)
{
	CurrLeft = pos;

  for (;;) {
		CurrLeft = GetLinePos(CurrLeft);
		if (!CurrLeft)
			return false;
    //skipLine();
    if (isalnum(*CurrLeft)
     || (ispunct(*CurrLeft)
		  && ((*CurrLeft != ';')
		   || (*(CurrLeft + 1) == '=')) // deleted setting
		  && (*CurrLeft != '['))
		 || (*CurrLeft < 0)) {    // allow high-bit chars
			if (*CurrLeft == '\\')
				CurrLeft++;
      break;
		}
    if ((*CurrLeft == '\0')
     || (*CurrLeft == '[')) {
      CurrLeft = CurrRight = NULL;
      LeftLen = RightLen = 0;
      return false;
    }
  }

  setRight();
  return true;
}


bool
DCirfile::Find(uns num)
{
	if (SecFile
	 && SecSect
	 && ((SecFound = SecFile->Find(num)) == true))
		return true;

	if (SubSect
	 && ((SubFound = FindNum(CurrSubSect, num)) == true))
		return true;

  // scan current section for line starting with num
  // if not found in section, return false
	if (CurrSect)
		return FindNum(CurrSect, num);

	return false;
}


bool
DCirfile::FindPos(uns num)
{
	if (!num)
		return false;

	if (SecFile
	 && SecSect
	 && ((SecFound = SecFile->FindPos(num)) == true))
		return true;

	if (SubSect
	 && ((SubFound = FindPosNum(CurrSubSect, num)) == true))
		return true;

  // scan current section for line at position num
  // if not found in section, return false
	if (CurrSect)
		return FindPosNum(CurrSect, num);

	return false;
}


uns 
DCirfile::GetPos(void)
{
	if (SecFound)
		return SecFile->GetPos();

	return CurrPos;
}


bool
DCirfile::FindPosNum(const char *sect, uns num)
{
  CurrLeft = sect;
	CurrPos = 0;

  for (;;) {
		CurrLeft = GetLinePos(CurrLeft);
		if (!CurrLeft)
			return false;
    //skipLine();
		if ((*CurrLeft == ';')
     || (*CurrLeft == '\r')
     || (*CurrLeft == '\n'))
			continue;
    if ((*CurrLeft == '\0')
     || (*CurrLeft == '[')) {
      CurrLeft = CurrRight = NULL;
      LeftLen = RightLen = 0;
			CurrPos = 0;
      return false;
    }
		CurrPos++;
    if (CurrPos >= num)
      break;
  }

  setRight();
  return true;
}


bool
DCirfile::FindNum(const char *sect, uns num)
{
  CurrLeft = sect;
	CurrPos = 0;

  for (;;) {
		CurrLeft = GetLinePos(CurrLeft);
		if (!CurrLeft)
			return false;
    //skipLine();
		if ((*CurrLeft == ';')
     || (*CurrLeft == '\r')
     || (*CurrLeft == '\n'))
			continue;
    if ((*CurrLeft == '\0')
     || (*CurrLeft == '[')) {
      CurrLeft = CurrRight = NULL;
      LeftLen = RightLen = 0;
			CurrPos = 0;
      return false;
    }
		CurrPos++;
    if (isdigit(*CurrLeft)) {
      if (atol(CurrLeft) == num)
        break;
    }
  }

  setRight();
  return true;
}


bool
DCirfile::FindAny(const char **names, short count)
{
	if (!names
	 || !count)
		return false;

	bool ret = Find(names, count);  // pass first as dummy

	return ret;
}


bool
DCirfile::FindAny(const anyini *any)
{
	if (!any->nm
	 || !any->cnt)
		return false;

	bool ret = Find((const char **) any->nm, any->cnt);  // pass first as dummy

	return ret;
}


bool
DCirfile::Find(const char *name)
{
	if (!name)
		return false;

	return Find(&name, 1);
}


bool
DCirfile::FindElemPath(const char *name)
{
	bool ret = false;

	if (!name)
		return false;

	InElemPath = true;
	ret = Find(&name, 1, true);
	InElemPath = false;
	return ret;

#if 0
	char **paths = NULL;
	char *hat = NULL;
	short cnt = 0;
	short i = 0;

	hat = strchr(name, '^');
	if (!hat)
		return Find(&name, 1);

	cnt = 2;  // name plus one variant
	while ((hat = strchr(hat + 1, '^')) != NULL)
		cnt++;

	paths = (char **) new char[sizeof(char *) * cnt];
	paths[0] = (char *) name;
	for (i = 1; i < cnt; i++) {  // change left to right
		paths[i] = NewName(paths[i - 1]);
		*(hat = strchr(paths[i], '^')) = '/';
	}

	ret = Find((const char **) paths, cnt);

	for (i = 1; i < cnt; i++)
		DeleteName(paths[i]);
	delete [] paths;

	return ret;
#endif
}


DCnlist DCirfile::ElemSetNames;
DCvlist DCirfile::ElemSets;
long DCirfile::ElemSetNum = 0;

void
DCirfile::GetElemSets(void)
{
	if (!Section("ElementSets"))
		return;

	char *nm = NULL;
	long len = 0;
	while (NextItem()) {  // this gets higher-priority items first
		if ((nm = StrLeft()) == NULL)
			break;
		if (*nm == '$')
			nm++;
		len = strlen(nm);
		while (nm[len - 1] == ' ')
			nm[--len] = '\0';
		if (!*nm)
			continue;
		if (!ElemSetNames(nm)) {  // don't replace higher-priority ones
			ElemSetNames.add(nm, ++ElemSetNum);
			ElemSets.add(StrArrRight(), ElemSetNum);
		}
	}
}


bool
DCirfile::Find(const char **names, short count, bool ep)
{
	if (!names
	 || !count)
		return false;

	InElemPath = ep;

	if (SecFile
	 && SecSect
	 && ((SecFound = SecFile->Find(names, count, ep)) == true))
		return true;

	if (SubSect
	 && ((SubFound = FindStr(CurrSubSect, names, count)) == true))
		return true;

  // scan current section for line starting with name
  // if not found in section, return false
	if (CurrSect)
		return FindStr(CurrSect, names, count);

	return false;
}


bool
DCirfile::FindStr(const char *sect, const char **names, short count)
{
	if (!names
	 || !count)
		return false;

  CurrLeft = sect;
	CurrPos = 0;

	if (!names[0])
		return false;

  uns len = strlen(names[0]);
	if (len == 0)
		return false;

  for (;;) {
		CurrLeft = GetLinePos(CurrLeft);
		if (!CurrLeft)
			return false;
    //skipLine();
		if ((*CurrLeft == ';')
     || (*CurrLeft == '\r')
     || (*CurrLeft == '\n'))
			continue;
    if ((*CurrLeft == '\0')
     || (*CurrLeft == '[')) {
      CurrLeft = CurrRight = NULL;
      LeftLen = RightLen = 0;
			CurrPos = 0;
      return false;
    }
		CurrPos++;
    if (isalnum(*CurrLeft)
     || ispunct(*CurrLeft)   // allow punct, may start style
		 || (*CurrLeft < 0)) {   // allow high-bit chars
			if (*CurrLeft == '\\')  // backslash escapes anything
				CurrLeft++;
			bool found = false;
			for (short i = 0; i < count; i++) {
				if (match(CurrLeft, names[i])) {
					found = true;
					break;
				}
			}
			if (found)
				break;
		}
  }

  setRight();
  return true;
}


bool 
DCirfile::match(const char *cl, const char *name)
{
	int nmc, clc;

	if (!name)
		return false;

	while (*name) {
		nmc = (int) (*name & 0xFF);
		clc = (int) (*cl & 0xFF);
		if (nmc == clc) {
			name++;
			cl++;
		}
		else if (clc == '=')  // allow equals as char in name
			break;
		else if (MatchNoCase
		  && (tolower(nmc) == tolower(clc))) {
			name++;
			cl++;
		}
		else if (MatchNoSpace
		 && (isspace(nmc) || isspace(clc))) {
			while (isspace(nmc))
				nmc = (int) (*++name & 0xFF);
			while (isspace(clc))
				clc = (int) (*++cl & 0xFF);
		}
		else if (MatchWild
		 && ((clc == '*') || (clc == '?'))) {
			if (clc == '*') {
				clc = (int) (*++cl & 0xFF);
				if (clc == '=')
					return true;
				if (isspace(clc)) {
					long n = 1;
					while (isspace((int) (cl[n] & 0xFF)))
						n++;
					if (cl[n] == '=')
						return true;
				}
				// deal with non-trailing "*" ****
				while (*++name) {
					if (match(cl, name))
						return true;
				}
				return false;
			}
			else { // single-char wildcard
				name++;
				cl++;
			}
		}
		else if (InElemPath
		 && ((clc == '$') || (nmc == '^'))) {
			if (nmc == '^') {
				if (clc == '/') {
					name++;
					cl++;
				}
				else
					return false;
			}
			if (clc == '$') {
				cl++;
				char sname[128];
				char *snp = sname;
				while (*cl && (*cl != '/') && (*cl != '^'))
					*snp++ = *cl++;
				*snp = '\0';
				long esetnum = 0;
				if ((esetnum = ElemSetNames.getitem(sname)) == 0)
					return false;
				char **eset = NULL;
				if ((eset = (char **) ElemSets.find(esetnum)) == NULL)
					return false;
				snp = sname;
				const char *oname = name;
				while (*name && (*name != '/') && (*name != '^'))
					*snp++ = *name++;
				*snp = '\0';
				if (!stricmp("no", eset[0])) {  // no means ignore this set
					name = oname;
				}
				else {
					bool esetfound = false;
					for (esetnum = 0; eset[esetnum] != NULL; esetnum++) {
						if (!stricmp(sname, eset[esetnum])) {
							esetfound = true;
							break;
						}
					}
					if (!esetfound)
						return false;
					name++;
				}
				cl++;
			}
		}
		else
			return false;
	}

	clc = (int) (*cl & 0xFF);
	if (*name == 0) { // matched full name
		if (MatchWild   // allow trailing wildcards
		 && ((clc == '*')
		  || (clc == '?')))
			clc = (int) (*++cl & 0xFF);
		while (isspace(clc)) // and trailing space
			clc = (int) (*++cl & 0xFF);
		if (clc == '=')  // matched full setting
			return true;
	}

	return false;
}


bool DCirfile::MatchNoCase = true;
bool DCirfile::MatchNoSpace = true;
bool DCirfile::MatchWild = true;

void
DCirfile::SetMatch(unc matchrule, bool bval)
{
	switch (matchrule) {
		case 0:
			MatchNoCase = bval;
			MatchNoSpace = bval;
			MatchWild = bval;
			break;
		case 1:
			MatchNoCase = bval;
			break;
		case 2:
			MatchNoSpace = bval;
			break;
		case 3:
			MatchWild = bval;
			break;
		default:
			break;
	}
}

#if 0
void 
DCirfile::skipLine(void)
{
	while (*CurrLeft++ != '\n')
		;
}
#endif

void
DCirfile::setRight()
{
  const char *cpt = CurrLeft;
	if ((*cpt == ';')          // deleted setting, skip first =
	 && (*(cpt + 1) == '='))
		cpt += 2;
  while ((*cpt != '=')
	 && (*cpt != '\n'))
    cpt++;
  if (*cpt == '=') {
    CurrRight = cpt + 1;
		cpt--;
    while (isspace(*cpt))
      cpt--;
    LeftLen = (cpt - CurrLeft) + 1;
    cpt = CurrRight;
    if ((*cpt == ' ') || (*cpt == '\t'))
      cpt++;  // skip just one leading space
    CurrRight = cpt;
  }
  else if (*cpt == '\n') {
		cpt--;
    while ((cpt > CurrLeft)
		 && isspace(*cpt))
      cpt--;
    LeftLen = (cpt - CurrLeft) + 1;
    CurrRight = NULL;
		RightLen = 0;
    return;
  }

  while (*cpt != '\n')
		cpt++;   // retain trailing spaces
	if (*--cpt == '\r')
		cpt--;
  RightLen = (cpt - CurrRight) + 1;
	if (RightLen < 1) {
		RightLen = 0;
		CurrRight = NULL;
	}
}


char *
DCirfile::StrLeft(void)
{
	if (SecFound)
		return SecFile->StrLeft();
	if (CurrLeft == NULL)
		return NULL;

	char* chp = strncpy(NewName(LeftLen + 1), CurrLeft, LeftLen);
	chp[LeftLen] = '\0';
	return chp;
}


char *
DCirfile::StrRight(void)
{
	if (SecFound)
		return SecFile->StrRight();
	if (CurrRight == NULL)
		return NULL;

	char* chp = strncpy(NewName(RightLen + 1), CurrRight, RightLen);
	chp[RightLen] = '\0';
	return chp;
}


char *
DCirfile::PathRight(void)
{
	if (SecFound)
		return SecFile->PathRight();
	if (CurrRight == NULL)
		return NULL;

	char *chp = strncpy(NewName(RightLen + 1), CurrRight, RightLen);
	chp[RightLen] = '\0';

	char *currdir = _getcwd(NewName(1024), 1024);
	char *inidir = this->getBasePath();
	int currdrive = _getdrive();
	int inidrive = 0;

	if (stricmp(currdir, inidir)
	 && !PathIsURL(chp)
	 && PathIsRelative(chp)) { // fix rel path
		if (*(inidir + 1) == ':')
			inidrive = (toupper(*inidir) - 'A') + 1;
		if (inidrive
		 && (currdrive != inidrive))
			_chdrive(inidrive);
		_chdir(inidir);
		char *nref = NewName(MAX_PATH);
		if (_fullpath(nref, chp, MAX_PATH)) {
			DeleteName(chp);
			chp = NewName(nref);
		}
		DeleteName(nref);
		if (inidrive
		 && (currdrive != inidrive))
			_chdrive(currdrive);
		_chdir(currdir);
	}
	DeleteName(currdir);

	return chp;
}


char 
DCirfile::CharRight(void)
{
	if (SecFound)
		return SecFile->CharRight();
	if (CurrRight == NULL)
		return '\0';

	return *CurrRight;
}


char 
DCirfile::UCRight(void)
{
	if (SecFound)
		return SecFile->UCRight();
	if (CurrRight == NULL)
		return '\0';

	return toupper(*CurrRight);
}


long 
DCirfile::IntLeft(void)
{
	if (SecFound)
		return SecFile->IntLeft();
	if (CurrLeft == NULL)
		return 0L;

	return atol(CurrLeft);
}


long 
DCirfile::IntRight(void)
{
	if (SecFound)
		return SecFile->IntRight();
	if (CurrRight == NULL)
		return 0L;

	return atol(CurrRight);
}


long 
DCirfile::HexRight(void)
{
	if (SecFound)
		return SecFile->HexRight();
	if (CurrRight == NULL)
		return 0L;

	return (long) strtoul(CurrRight, NULL, 16);
}


bool
DCirfile::BoolRight()
{
	if (SecFound)
		return SecFile->BoolRight();
	if (CurrRight == NULL)
		return false;

	if (!_strnicmp(CurrRight, "yes", RightLen < 3 ? RightLen : 3)
	 || !_strnicmp(CurrRight, "true", RightLen < 4 ? RightLen : 4)
	 || (*CurrRight == '1'))
		return true;

	return false;
}


bool
DCirfile::ListRight(void)
{
	if (SecFound)
		return (SecList = SecFile->ListRight());

	ListHead = CurrRight;
	if (ListHead == NULL)
		return false;

	ListEnd = (char *) CurrRight + RightLen;
	*ListEnd = '\n';  // trim trailing spaces

	return (*ListHead != '\n') ? true : false;
}


char *
DCirfile::ListItem(void)
{
	if (SecList) {
		char *item = NULL;
		if ((item = SecFile->ListItem()) == NULL)
			SecList = false;
		return item;
	}
	if (ListHead == NULL)
		return NULL;

	// skip leading spaces and commas
	while ((*ListHead == ',')
	 || (*ListHead == ' ')
	 || (*ListHead == '\t'))
		ListHead++;

	if (ListHead >= ListEnd) {
		ListHead = ListEnd = NULL;
		return NULL;
	}

	// stop at first trailing space or comma
	const char *comma = ListHead;
	while (*comma
	 && (*comma != ',')
	 && (*comma != ' ')
	 && (*comma != '\t')
	 && (*comma != '\n'))
		comma++;

	size_t len = comma - ListHead;
	char* chp = strncpy(NewName(len), ListHead, len);
	chp[len] = '\0';

	ListHead = comma + 1;
	if (ListHead >= ListEnd)
		ListHead = ListEnd = NULL;

	return chp;
}


char **
DCirfile::StrArrRight(void)
{
	if (SecFound)
		return SecFile->StrArrRight();
	if (CurrRight == NULL)
		return NULL;
	if (!ListRight())
		return NULL;

	long count = 0;
	char *chp = NULL;
	while ((chp = ListItem()) != NULL) {
		count++;
		DeleteName(chp);
	}

	char **ptr = (char **) new char[sizeof(char *) * (count + 1)];
	ListRight();

	for (long i = 0; i < count; i++)
		ptr[i] = ListItem();

	ptr[count] = NULL;
	return ptr;
}


long *
DCirfile::IntArrRight(void)
{
	if (SecFound)
		return SecFile->IntArrRight();
	if (CurrRight == NULL)
		return NULL;
	if (!ListRight())
		return NULL;

	long count = 0;
	char *chp = NULL;
	while ((chp = ListItem()) != NULL) {
		count++;
		DeleteName(chp);
	}

	long *ptr = new long[count + 1];
	ListRight();

	for (long i = 0; i < count; i++) {
		ptr[i] = atol(chp = ListItem());
		DeleteName(chp);
	}

	ptr[count] = 0;
	return ptr;
}



// end of dcfili.cc


