/* m2rwrt.c is the FDK ini file writer for mif2go
 * Copyright 2000-2006 Omni Systems, Inc.  All Rights Reserved.
 */

#include "m2rbook.h"


/* globals */
BookComp *BookComps = NULL;
StringT TOCname = NULL;
F_ObjHandleT TOCId = 0;
IntT Fid = 0;
IntT ChapLenMax = 0;

ParaStyle *ParaStyles = NULL;
ParaStyle guard = { NULL, 0, NULL, 255, 0x7FFFFFFF, 0, 0, &guard };
ParaStyle *z = &guard;
UCharT Level[3] = { 'V', '1', '\0' };


/* info needed for creating .ini file */
StringT ProjectDescription = NULL;

StringT FileSuffix = NULL;
StringT WordSuffix = NULL;
StringT WordPerfectSuffix = NULL;
StringT HTMLSuffix = NULL;
StringT XMLSuffix = NULL;
StringT MIFSuffix = NULL;
StringT MIFBookSuffix = NULL;
StringT DCLSuffix = NULL;

BoolT UseTemplate = False;
StringT TemplateName = "template.fm";
IntT TemplateFlags = 0;

IntT GrVertAdjust = 8;
IntT EqVertAdjust = 18;

// used in Automation but not in .ini
StringT GraphPath = NULL;


/* info for WinHelp versions */
StringT HelpFileName = NULL;
StringT CntFileName = NULL;
BoolT UseFmTOC = True;
BoolT UseFmIX = True;
BoolT UseFmGenFiles = True;

BoolT MakeCombCnt = True;
StringT HPJName = NULL;
StringT OHVName = "%";
StringT DefaultChmFile = NULL;
StringT Compiler = NULL;

IntT BrowseTopicsMin = 200;
IntT BrowseDigits = 5;
IntT BrowseStart = 1;


/* info for Word versions */
BoolT UseWordTemplate = False;
StringT WordTemplateName = "template.dot";
BoolT WordTemplateAuto = False;

UCharT Sideheads = 'F';
UCharT Headfeet = 'S';
BoolT FramedHF = True;


/* info for HTML versions */
BoolT UseCSS = True;
BoolT CreateCSS = True;
StringT CssFileName = "local.css";

UCharT TableSizing = 'A';
StringT TableDPI = "96";
StringT TblBorder = "3";
StringT TblSpacing = "2";
StringT TblPadding = "6";


/* info needed for .hpj file */
StringT HelpContentsTopic = NULL;
StringT HelpTitle = "Your Title Here";
StringT HelpCopyright = "(c) 2005 Omni Systems, Inc.";
BoolT HelpCopyDate = True;


VoidT WriteStartingIni(StringT inipath, StringT filename,
										BoolT book, UCharT form)
{
	StringT ohome = NULL;
	StringT ininame = NULL;
	StringT newini = NULL;
	StringT destpath = NULL;
	StringT destini = NULL;
	StringT olddest = NULL;
	StringT oldmsg = NULL;
	StringT command = NULL;
	StringT arcname = NULL;
	BoolT docfile = False;
	BoolT msword = False;
	BoolT winhelp = False;

	// identify start in %omsyshome%\m2g\local\starts or system\starts
	ohome = getenv("omsyshome");
	if ((FormType = ProjSelType) > 0)
		ininame = IniNameList[FormType - 1];
	if (ohome && ininame) {
		newini = F_StrNew(F_StrLen(ohome) + F_StrLen(ininame) + 20);
		sprintf(newini, "%s\\m2g\\local\\starts\\%s", ohome, ininame);
		if (!FileExists(newini)) {
			sprintf(newini, "%s\\m2g\\system\\starts\\%s", ohome, ininame);
			if (!FileExists(newini)) {
				F_StrFree(newini);
				newini = NULL;
			}
		}
	}

	destpath = Fm2a(inipath);
	olddest = MakeFileName(destpath, StdIni);

	// if none, write old form of ini using WriteBasicIni
	if (!newini) {
		CurrIniName = olddest;
		WriteBasicIni(inipath, filename, book, form);
		return;
	}

	// see if start is already present
	destini = MakeFileName(destpath, ininame);
	if (FileExists(destini)) {
		EditIniFile(A2fm(destini));
		return;
	}

	// what if older version mif2*.ini exists?
	if (FileExists(olddest)) {
		oldmsg = F_StrNew(F_StrLen(ReplacingOldIni) + F_StrLen(StdIni)
		                  + F_StrLen(ininame) - 3);
		sprintf(oldmsg, ReplacingOldIni, StdIni, ininame);
		F_ApiAlert(oldmsg, FF_ALERT_CONTINUE_NOTE);
		F_Free(oldmsg);
	}
	F_Free(olddest);
	olddest = NULL;

	// copy start to inipath (dest directory)
	command = F_StrNew(F_StrLen(newini) + F_StrLen(destini) + 14);
	sprintf(command, "copy /Y \"%s\" \"%s\"", newini, destini);
	RunDOSCommand(command, True, False);
	if (ExitCode == 1) {
		ExitCode = 0;  // missing file error
		WriteBasicIni(inipath, filename, book, form);
		return;
	}
 
	// modify start per setup dialog results
	CurrIniName = destini;
	arcname = BaseFileName(filename);

#if 0
	switch (form) {
		case 'P':
			if (WordPerfectSuffix == NULL)
				WordPerfectSuffix = RtfExt;
			M2GIniPutSetting(CurrIniName, Setup, "WordPerfectSuffix", WordPerfectSuffix);
			break;

		case 'G':
			XMLSuffix = XmlExt;
			M2GIniPutSetting(CurrIniName, Setup, "XMLSuffix", XMLSuffix);
			break;

		case 'D':
			XMLSuffix = DITAExt;
			M2GIniPutSetting(CurrIniName, Setup, "XMLSuffix", XMLSuffix);
			break;

		case 'B':
			XMLSuffix = DocBookExt;
			M2GIniPutSetting(CurrIniName, Setup, "XMLSuffix", XMLSuffix);
			break;

		case 'A':
			if (DCLSuffix == NULL)
				DCLSuffix = DclExt;
			M2GIniPutSetting(CurrIniName, Setup, "DCLSuffix", DCLSuffix);
			break;

		case 'F':
			if (MIFSuffix == NULL)
				MIFSuffix = MifExt;
			M2GIniPutSetting(CurrIniName, Setup, "MIFSuffix", MIFSuffix);
			if (MIFBookSuffix == NULL)
				MIFBookSuffix = MIFSuffix;
			M2GIniPutSetting(CurrIniName, Setup, "MIFBookSuffix", MIFBookSuffix);
			break;

		case 'E':
			HTMLSuffix = ".html";
			M2GIniPutSetting(CurrIniName, Setup, "HTMLSuffix", HTMLSuffix);
			break;

		case 'N':
		case 'J':
		case 'O':
		case 'M':
		case 'C':
		case 'X':
			if (HTMLSuffix == NULL)
				HTMLSuffix = HtmExt;
			M2GIniPutSetting(CurrIniName, Setup, "HTMLSuffix", HTMLSuffix);
			break;

		case 'W':
		case '8':
			if (WordSuffix == NULL)
				WordSuffix = RtfExt;
			M2GIniPutSetting(CurrIniName, Setup, "WordSuffix", WordSuffix);
			break;

		case '3':
		case 'H':
			// fixed suffix for WH
		default:
			break;
	}
#endif

	switch (form) {
		case 'F':  // Frame MIF
			WriteIniFVars();	// m2rsetf.c
			break;

		case 'A':  // ASCII DCL
			WriteIniAVars();	// m2rseta.c
			break;

		case 'B':  // DocBook
			SetAutoIniOptions(arcname);
			WriteIniBVars();  // m2rsetb.c
			break;

		case 'D':  // DITA
			SetAutoIniOptions(arcname);
			WriteIniDVars();  // m2rsetd.c
			break;

		case 'W':  // Word
		case '8':
		case 'P':
			SetAutoIniOptions(arcname);
			WriteIniWVars();  // m2rsetw.c
			docfile = True;
			msword = True;
			break;

		case 'N':  // HTML/XHTML
		case 'X':
			SetAutoIniOptions(arcname);
			WriteIniNVars();  // m2rsetn.c
			docfile = True;
			break;

		case 'G':  // XML
			SetAutoIniOptions(arcname);
			WriteIniCommonVars(True, True);
			SetCSSIniOptions();
			break;

		case 'M':  // HTML Help
			SetAutoIniOptions(arcname);
			WriteIniMVars(filename, book);  // m2rsetm.c
			if (WriteHelpProjectFile)
				M2GIniGetSetting(CurrIniName, Setup, "HTMLSuffix", &HTMLSuffix);
			docfile = True;
			break;

		case 'C':  // OmniHelp
			SetAutoIniOptions(arcname);
			WriteIniCVars(book);  // m2rsetc.c
			docfile = True;
			break;

		case 'J':  // Java
		case 'O':
			SetAutoIniOptions(arcname);
			WriteIniJVars(filename, book, form);  // m2rsetj.c
			docfile = True;
			break;

		case 'E':  // Eclipse
			SetAutoIniOptions(arcname);
			WriteIniEVars(book);  // m2rsete.c
			docfile = True;
			break;

		case '3': // WinHelp
		case 'H':
			SetAutoIniOptions(arcname);
			WriteIniHVars(book, form);  // m2rseth.c
			docfile = True;
			winhelp = True;
			break;

		default:
			// assert(0);  // case not handled
			break;
	}

	// add project-specific settings for format mappings to Documents file
	if (docfile)
		WriteDocFileInfo(destpath, book, winhelp, msword);

	// update and close new .ini
	CloseIni();
	F_StrFree(arcname);
}


StringT LocalConfigPath = "..\\_config\\";
StringT WinHelpDocName = "winhelp_doc.ini";
StringT WordDocName = "word_doc.ini";
StringT HTMLDocName = "html_doc.ini";


VoidT ReadTopConfigIni(StringT cfgini)
{
	StringT ohome = NULL;
	StringT inipath = NULL;
	StringT locpath = "\\m2g\\local\\config\\local_m2g_config.ini";
	StringT syspath = "\\m2g\\system\\config\\m2g_config.ini";
	IntT loclen = 0;
	IntT syslen = 0;

	ohome = getenv("omsyshome");
	if (cfgini) {
		if (!F_StrCmpN(cfgini, "%omsyshome%", 11))
			inipath = MakeFileName(ohome, cfgini + 11);
		else
			inipath = F_StrCopyString(cfgini);
	}
	else if (ohome) {
		loclen = F_StrLen(locpath);
		syslen = F_StrLen(syspath);
		inipath = F_StrNew(F_StrLen(ohome) + max(loclen, syslen) + 1);
		sprintf(inipath, "%s%s", ohome, locpath);
		if (!FileExists(inipath)) {
			sprintf(inipath, "%s%s", ohome, syspath);
			if (!FileExists(inipath)) {
				F_StrFree(inipath);
				inipath = NULL;
			}
		}
	}

	if (inipath) {
		M2GSetIni(inipath, NULL);
		M2GIniGetString("Setup", "LocalConfigPath", &LocalConfigPath);
		if (!LocalConfigPath)
			LocalConfigPath = "..\\_config\\";
		M2GIniGetString("Setup", "WinHelpDocName", &WinHelpDocName);
		if (!WinHelpDocName)
			WinHelpDocName = "winhelp_doc.ini";
		M2GIniGetString("Setup", "WordDocName", &WordDocName);
		if (!WordDocName)
			WordDocName = "word_doc.ini";
		M2GIniGetString("Setup", "HTMLDocName", &HTMLDocName);
		if (!HTMLDocName)
			HTMLDocName = "html_doc.ini";
		CallM2GFrame("IniProcDone ");
		F_StrFree(inipath);
	}
	else {
		LocalConfigPath = "..\\_config\\";
		WinHelpDocName = "winhelp_doc.ini";
		WordDocName = "word_doc.ini";
		HTMLDocName = "html_doc.ini";
	}
}


VoidT WriteDocFileInfo(StringT destpath, BoolT book, BoolT winhelp, BoolT msword)
{
	F_ObjHandleT bookId = 0;
	F_ObjHandleT docId = 0;
	BookComp **bcp = &BookComps;
	ParaStyle **psp = &ParaStyles;
	StringT cfgini = NULL;
	StringT cfgpath = NULL;
	BoolT relcfg = False;
	StringT docpath = NULL;
	StringT docname = NULL;
	StringT lastpart = NULL;
	StringT docini = NULL;
	StringT temp1 = NULL;
	StringT temp2 = NULL;
	IntT brStart = 0;
	FILE *fini = NULL;

	// determine path to Documents files
	// read cfgpath from a config .ini
	M2GIniGetSetting(CurrIniName, "Templates", "Configs", &cfgini);
	ReadTopConfigIni(cfgini);
	cfgpath = F_StrCopyString(LocalConfigPath);

	for (lastpart = cfgpath; *lastpart; lastpart++) {
		if (*lastpart == '/')
			*lastpart = '\\';
	}
	for (lastpart = destpath; *lastpart; lastpart++) {
		if (*lastpart == '/')
			*lastpart = '\\';
	}
	while (*cfgpath == '.') {
		relcfg = True;
		while (!F_StrCmpN(cfgpath, "..\\", 3)) {
			cfgpath += 3;
			if (destpath[F_StrLen(destpath) - 1] == '\\')
				destpath[F_StrLen(destpath) - 1] = '\0';
			if ((lastpart = F_StrRChr(destpath, '\\')) != NULL)
				*(lastpart + 1) = '\0';
		}
		while (!F_StrCmpN(cfgpath, ".\\", 2))
			cfgpath += 2;
	}

	if (relcfg)
		docpath = MakeFileName(destpath, cfgpath);
	else
		docpath = F_StrCopyString(cfgpath);

	if (!PathExists(docpath))
		MakeDir(docpath);

	docname = winhelp ? WinHelpDocName :
		        (msword ? WordDocName :
		                  HTMLDocName);

	docini = MakeFileName(docpath, docname);

	if (!FileExists(docini)  // if already present, leave it
	 && ((fini = fopen(docini, "wt")) == NULL)) { // can't open, skip it
		F_Free(docini);
		docini = NULL;
	}

	if (fini) {
		// get book and style info
		if (book)
			GetChapters(bookId = F_ApiGetId(0, FV_SessionId, FP_ActiveBook));
		else if ((bookId = IdentifyBookOfDoc(filename)) != 0)
			TOCname = GetBookTOC(bookId);		/* see if part of an open book */
		if (TOCname) {
			if ((TOCId = IdentifyDocName(TOCname)) != 0) { /* already open */
				GetStyles(TOCId, True);
			}
			else if ((TOCId = OpenDocName(TOCname)) != 0) {
				GetStyles(TOCId, True);
				F_ApiClose(TOCId, FF_CLOSE_MODIFIED);  /* put away TOC doc */
			}
		}
		else {  /* pick styles from doc itself */
			if (book) { /* choose a file to pick from */
				docId = F_ApiGetId(FV_SessionId, bookId, FP_FirstComponentInBook);
				while (docId) {
					if (!F_ApiGetInt(bookId, docId, FP_BookComponentIsGeneratable)
					 && ((TOCname = F_ApiGetString(bookId, docId, FP_Name)) != NULL)) {
						if ((TOCId = IdentifyDocName(TOCname)) != 0) {
							GetStyles(TOCId, False);
							break;
						}
						else if ((TOCId = OpenDocName(TOCname)) != 0) {
							GetStyles(TOCId, False);
							F_ApiClose(TOCId, FF_CLOSE_MODIFIED);  /* put away doc */
							break;
						}
					}
					docId = F_ApiGetId(bookId, docId, FP_NextComponentInBook);
				}
			}
			else {
				docId = IdentifyDocName(filename);
				GetStyles(docId, False);
			}
		}

		fprintf(fini, "; Document file %s\n; referenced by %s\n", docini, CurrIniName);

		if (winhelp) {
			// determine HelpStyles, HelpCntStyles, and BrowseStart
			fprintf(fini, "\n\n[HelpStyles]\n");
			psp = &ParaStyles;
			while (*psp && (*psp)->slev) {
				temp1 = Fm2a((*psp)->psname);
				fprintf(fini, "%s=Topic Browse Contents\n", temp1);
				psp = &((*psp)->psnext);
				F_Free(temp1);
			}
			fprintf(fini, "\n[HelpCntStyles]\n");
			psp = &ParaStyles;
			while (*psp && (*psp)->slev) {
				temp1 = Fm2a((*psp)->psname);
				fprintf(fini, "%s=%s\n", temp1, (*psp)->slev);
				psp = &((*psp)->psnext);
				F_Free(temp1);
			}
			if (book) {
				fprintf(fini, "\n[BrowseStart]\n");
				bcp = &BookComps;
				brStart = BrowseStart;
				while (*bcp) {	/* list chapters with starting number */
					temp1 = Fm2a((*bcp)->bcname);
					fprintf(fini, "%s=%ld\n", temp1, brStart);
					if (brStart == BrowseStart)  /* add real topic counting later */
						brStart = BrowseTopicsMin;
					else
						brStart += BrowseTopicsMin;
					bcp = &((*bcp)->bcnext);
					F_Free(temp1);
				}
			}
			fprintf(fini, "\n[End]\n");
			fclose(fini);
		}
		else if (msword) {
			// determine WordSectionStyles, and WordCntStyles
			if (book) {
				fprintf(fini, "\n\n[WordSectionFiles]\n");
				bcp = &BookComps;
				while (*bcp) {	/* list chapters with section nums */
					if ((*bcp)->px) {
						temp1 = Fm2a((*bcp)->bcname);
						temp2 = Fm2a((*bcp)->px);
						fprintf(fini, "%s=%s\n", temp1, temp2);
						F_Free(temp1);
						F_Free(temp2);
					}
					bcp = &((*bcp)->bcnext);
				}
			}
			fprintf(fini, "\n[WordCntStyles]\n");
			psp = &ParaStyles;
			temp2 = "";
			while (*psp && (*psp)->slev) {
				temp1 = Fm2a((*psp)->psname);
				if (F_StrICmp(temp1, temp2)) {
					fprintf(fini, "%s=%s\n", temp1, (*psp)->slev + 1);
					temp2 = temp1;
				}
				psp = &((*psp)->psnext);
			}
			fprintf(fini, "\n[End]\n");
			fclose(fini);
		}
		else {
			// determine HTMLStyles and HelpContentsLevels
			fprintf(fini, "\n\n[HTMLStyles]\n");
			psp = &ParaStyles;
			while (*psp && (*psp)->slev) {
				temp1 = Fm2a((*psp)->psname);
				fprintf(fini, "%s=Title Split\n", temp1);
				psp = &((*psp)->psnext);
				F_Free(temp1);
			}
			fprintf(fini, "\n[HelpContentsLevels]\n");
			fprintf(fini, "; Fill in FM head para formats and their level here\n");
			psp = &ParaStyles;
			while (*psp && (*psp)->slev) {
				temp1 = Fm2a((*psp)->psname);
				fprintf(fini, "%s=%s\n", temp1, (*psp)->slev + 1);
				psp = &((*psp)->psnext);
				F_Free(temp1);
			}
		}

		fprintf(fini, "\n[End]\n");
		fclose(fini);
	}

	// add reference to file in [Templates]Documents of start
	if (docini) {
		M2GIniPutSetting(CurrIniName, "Templates", "Document", docini);
		F_Free(docini);
	}

	F_Free(docpath);
}


VoidT SetCSSIniOptions(VoidT)
{
	StringT temp1 = NULL;

	M2GIniPutBool(CurrIniName, "CSS", "UseCSS", UseCSS);
	if (UseCSS) {
		M2GIniPutSetting(CurrIniName, "CSS", "WriteCssStylesheet",
											(UseCSS && CreateCSS) ? "Once" : "Never");
		M2GIniPutSetting(CurrIniName, "CSS", "CssFileName", temp1 = Fm2a(CssFileName));
		F_Free(temp1);
	}
}


VoidT SetAutoIniOptions(StringT filename)
{
	StringT temp1 = NULL;

	M2GIniPutSetting(CurrIniName, "Automation", "ArchiveName", temp1 = Fm2a(filename));
	F_Free(temp1);
}


/* ini-file write functions */

VoidT WriteBasicIni(StringT inipath, StringT filename,
										BoolT book, UCharT form)
{
	FILE *fini;
	StringT basename = NULL;
	StringT ininame = NULL;
	StringT fname = NULL;

	F_ObjHandleT bookId = 0;
	F_ObjHandleT docId = 0;

	BookComp **bcp = &BookComps;
	ParaStyle **psp = &ParaStyles;
	DFPropT *fnt = DefFontProps;

	BoolT hform = False;
	BoolT html = False;
	BoolT help = False;
	BoolT compiled = False;
	BoolT eclipse = False;
	StringT outstr = "Standard";
	IntT mnChars = 0;
	IntT brStart = 1;

	IntT tval;
#ifdef _OLD_IDS
	CharT ch;
#endif
	StringT temp1 = NULL;
	StringT temp2 = NULL;
	StringT HPJLabel = NULL;
	StringT bname = NULL;

	if (book)
		GetChapters(bookId = F_ApiGetId(0, FV_SessionId, FP_ActiveBook));
	else if ((bookId = IdentifyBookOfDoc(filename)) != 0)
		TOCname = GetBookTOC(bookId);		/* see if part of an open book */

	basename = Fm2a(temp1 = BaseFileName(filename));
	F_Free(temp1);

	switch (form) {
		case 'W':
		case '8':
		case 'P':
			break;
		case 'F':
			outstr = "FrameMIF";
			break;
		case '3':
		case 'H':
			hform = True;
			help = True;
			compiled = True;
			outstr = "Help";
			HPJLabel = "HPJFileName";
			break;
		case 'N':
			html = True;
			break;
		case 'A':
			html = True;
			outstr = "AsciiDCL";
			break;
		case 'J':
			html = True;
			help = True;
			outstr = "JavaHelp";
			HPJLabel = "HSFileName";
			if ((HPJName == NULL)
			 || (F_StrSubString(HPJName, ".hs") == -1)) {
				HPJName = F_Alloc(F_StrLen(basename) + 4, NO_DSE);
				F_StrCpy(HPJName, basename);
				F_StrCat(HPJName, ".hs");
			}
			break;
		case 'O':
			html = True;
			help = True;
			outstr = "OracleHelp";
			HPJLabel = "HSFileName";
			if ((HPJName == NULL)
			 || (F_StrSubString(HPJName, ".hs") == -1)) {
				HPJName = F_Alloc(F_StrLen(basename) + 4, NO_DSE);
				F_StrCpy(HPJName, basename);
				F_StrCat(HPJName, ".hs");
			}
			break;
		case 'M':
			html = True;
			help = True;
			compiled = True;
			outstr = "MSHHelp";
			HPJLabel = "HHPFileName";
			if ((HPJName == NULL)
			 || (F_StrSubString(HPJName, ".hhp") == -1)) {
				HPJName = F_Alloc(F_StrLen(basename) + 5, NO_DSE);
				F_StrCpy(HPJName, basename);
				F_StrCat(HPJName, ".hhp");
			}
			DefaultChmFile = F_Alloc(F_StrLen(basename) + 5, NO_DSE);
			F_StrCpy(DefaultChmFile, basename);
			F_StrCat(DefaultChmFile, ".chm");
			break;
		case 'E':
			eclipse = True;
			html = True;
			help = True;
			outstr = "EclipseHelp";
			break;
		case 'C':
			html = True;
			help = True;
			outstr = "Cross-platform OmniHelp";
			break;
		case 'X':
			html = True;
			outstr = "XHTML";
			break;
		case 'D':
			html = True;
			outstr = "DITA";
			break;
		case 'B':
			html = True;
			outstr = "Book";
			break;
		case 'G':
			html = True;
			outstr = "GenXML";
			break;

		default:
			return;
	}

	ininame = Fm2a(MakeFileName(inipath, StdIni));
	CurrIniName = F_StrCopyString(ininame);

	/* if already present, leave it */
	if ((fini = fopen(ininame, "rt")) != NULL) {
		fclose(fini);
		if (book)
			temp1 = Fm2a(BookComps->bcname);
		else
			GetIniString(ininame, "FileIDs", basename, &temp1); 
		if ((temp1 == NULL)
		 || (*temp1 == '\0'))
			UpdateBasicIni(ininame, basename, book, form);
		F_StrFree(temp1);
		F_StrFree(ininame);
		F_StrFree(basename);
		return;
	}

	if ((fini = fopen(ininame, "wt")) == NULL) {
		F_StrFree(ininame);
		F_StrFree(basename);
		return;
	}

	/* write header for .ini file */
	time(&tval);
	fprintf(fini, IniStartStr, (StringT) ctime(&tval), temp1 = Fm2a(filename));
	F_Free(temp1);
	F_StrFree(ininame);

	/* mif2rtf */
	fprintf(fini, "\n[Options]\n");
	fprintf(fini, "Output=%s\n", outstr);
	//fprintf(fini, "MathType=3\n");
	if (!html)
		fprintf(fini, "EqVertAdjust=%ld\n", EqVertAdjust);

	/* generic sections used by both */
	fprintf(fini, "\n[FDK]\n");
	if (ConfigTemplate) {
		fprintf(fini, "ConfigTemplate=%s\n", temp1 = Fm2a(ConfigName));
		F_Free(temp1);
	}
	else
		fprintf(fini, ";ConfigTemplate=%s\n", "D:/path/to/YourTemplate.ini");
	fprintf(fini, "UseExistingMIF=%s\n", UseMIF ? "Yes" : "No");
	fprintf(fini, "DeleteMIF=No\n");  //%s\n", DeleteMIF ? "Yes" : "No");
	if (!html)
		fprintf(fini, "ConversionDesigner=%s\n", CondDes ? "Yes" : "No");

	if (form == 'A') {
		if (DCLSuffix == NULL)
			DCLSuffix = DclExt;
		fprintf(fini, "FileSuffix=%s\n", DCLSuffix);
	}
	else if (form == 'F') {
		if (MIFSuffix == NULL)
			MIFSuffix = MifExt;
		fprintf(fini, "FileSuffix=%s\n", MIFSuffix);
		if (MIFBookSuffix == NULL)
			MIFBookSuffix = MIFSuffix;
		fprintf(fini, "MIFBookSuffix=%s\n", MIFBookSuffix);
	}
	else
		fprintf(fini, "FileSuffix=%s\n", FileSuffix);

#if 0
	if (form == 'P') {
		if (WordPerfectSuffix == NULL)
			WordPerfectSuffix = RtfExt;
		fprintf(fini, "WordPerfectSuffix=%s\n", WordPerfectSuffix);
	}
	else if (form == 'G') {
		XMLSuffix = XmlExt;
		fprintf(fini, "XMLSuffix=%s\n", XMLSuffix);
	}
	else if (form == 'D') {
		XMLSuffix = DITAExt;
		fprintf(fini, "XMLSuffix=%s\n", XMLSuffix);
	}
	else if (form == 'B') {
		fprintf(fini, "XMLSuffix=%s\n", XMLSuffix = DocBookExt);
	}
	else if (form == 'E') {
		HTMLSuffix = ".html";
		fprintf(fini, "HTMLSuffix=%s\n", HTMLSuffix);
	}
	else if (html) {
		if (HTMLSuffix == NULL)
			HTMLSuffix = HtmExt;
		fprintf(fini, "HTMLSuffix=%s\n", HTMLSuffix);
	}
	else if (!hform) {
		if (WordSuffix == NULL)
			WordSuffix = RtfExt;
		fprintf(fini, "WordSuffix=%s\n", WordSuffix);
	}
#endif

	if (!ConfigTemplate
	 || UseTemplate) {
		fprintf(fini, "ApplyTemplateFile=%s\n", UseTemplate ? "Yes" : "No");
		if (UseTemplate) {
			fprintf(fini, "TemplateFileName=%s\n", temp1 = Fm2a(TemplateName));
			F_Free(temp1);
			fprintf(fini, "AppliedTemplateFlags=%ld\n", TemplateFlags);
		}
	}

	if (!ConfigTemplate
	 || ConvertVars)
		fprintf(fini, "ConvertVariables=%s\n", ConvertVars ? "Yes" : "No");

	if (!ConfigTemplate
	 || GenUpBook)
		fprintf(fini, "GenerateBook=%s\n", GenUpBook ? "Yes" : "No");

	if (!ConfigTemplate) {
		fprintf(fini, "UseFrameTOC=%s\n", UseFmTOC ? "Yes" : "No");
		fprintf(fini, "UseFrameIX=%s\n", UseFmIX ? "Yes" : "No");
		fprintf(fini, "UseFrameGenFiles=%s\n", UseFmGenFiles ? "Yes" : "No");

		if (form == 'F') {
			fprintf(fini, "MakeBookMIF=%s\n", MakeBook ? "Yes" : "No");
			fprintf(fini, "OrigExtForMIF=%s\n", OrigExtForMIF ? "Yes" : "No");
		}
	}

	fprintf(fini, "WriteEquations=%s\n", WriteEqns ? "Yes" : "No");
	fprintf(fini, "WriteAllGraphics=%s\n", WriteAllGraphics ? "Yes" : "No");

	if (!ConfigTemplate) {
		WriteRefGraphics = True;
		WriteMpgGraphics = html ? False : True;
		fprintf(fini, "WriteRefPageGraphics=%s\n", WriteRefGraphics ? "Yes" : "No");
		fprintf(fini, "WriteMasterPageGraphics=%s\n", WriteMpgGraphics ? "Yes" : "No");

		fprintf(fini, "UseGraphicFileID=Yes\n");
		fprintf(fini, "GraphicNameDigits=6\n");

		fprintf(fini, "GraphicExportDPI=%ld\n", GrDpiValue);
		fprintf(fini, "EquationExportDPI=%ld\n", EqDpiValue);
		fprintf(fini, "EquationFrameExpand=125\n");
		fprintf(fini, "GraphicExportFormat=%s\n", temp1 = Fm2a(GrFormList[GrFormMap[GrFormat]]));
		F_Free(temp1);
	}

	fprintf(fini, "PrjFileName=%s\n", temp1 = Fm2a(CurrPrjFilePath));
	F_Free(temp1);

	if (form == 'F') {  // Frame MIF, done now
		fclose(fini);
		F_StrFree(basename);
		return;
	}

	if (CurrFileIDPath) {
		fprintf(fini, "IDFileName=%s\n", CurrFileIDPath);
		_chmod(CurrFileIDPath, _S_IWRITE);
		SetFileID(NULL); // sets NextFileID
	}
#if 0
	else if (!html) {
		NextFileID = F_StrCopyString("aa");
		fprintf(fini, "NextFileID=%s\n", NextFileID);
	}
#endif
	if (form == 'A') {  // ASCII DCL done now
		PutGraphExport(fini);
		fclose(fini);
		F_StrFree(basename);
		return;
	}

	fprintf(fini, "\n[Automation]\n");
	if (compiled)
		fprintf(fini, "CompileHelp=%s\n", MakeHLP ? "Yes" : "No");
	fprintf(fini, "WrapAndShip=%s\n", WrapAndShip ? "Yes" : "No");

	if (!ConfigTemplate) {
		if (WrapPath) {
			fprintf(fini, "WrapPath=%s\n", temp1 = Fm2a(WrapPath));
			F_Free(temp1);
		}
		else
			fprintf(fini, "WrapPath=.\\wrap\n");
		if (ArchiveCommand) {
			fprintf(fini, "ArchiveCommand=%s\n", temp1 = Fm2a(ArchiveCommand));
			F_Free(temp1);
		}
		if (ShipPath) {
			fprintf(fini, "ShipPath=%s\n", temp1 = Fm2a(WrapPath));
			F_Free(temp1);
		}
		else
			fprintf(fini, "ShipPath=..\\ship\n");

		fprintf(fini, "\n[Fonts]\n");
		while (fnt->name) {
			if (fnt->rmap) {
				temp1 = Fm2a(fnt->name);
				temp2 = Fm2a(fnt->rmap);
				fprintf(fini, "%s=%s\n", temp1, temp2);
				F_Free(temp1);
				F_Free(temp2);
			}
			fnt++;
		}

		fprintf(fini, "\n[Defaults]\n");
	}

	fprintf(fini, "\n[Graphics]\n");
	fprintf(fini, "UseGraphicPreviews=%s\n", WriteAllGraphics ? "Yes" : "No");
	if (!html) {
		fprintf(fini, "GrVertAdjust=%ld\n", GrVertAdjust);
		if (hform) {
			if (book  /* find min number of name chars needed to distinguish */
			 && ((mnChars = GetMetaNameMin()) > 4)) {
				fprintf(fini, "MetaNameChars=%ld\n", mnChars + 1);
				if (mnChars == 5)
					fprintf(fini, "MetaNumDigits=2\n");
			}
		}
	}
	else {
		fprintf(fini, "StripGraphPath=No\n");
		fprintf(fini, "UseOriginalGraphicNames=No\n");
	}


	if (ConfigTemplate) {  // done writing short .ini
		fclose(fini);
		F_StrFree(basename);
		return;
	}

	fprintf(fini, "\n[GraphFiles]\n");

	if (html)  /* for HTML */
		fprintf(fini, "\n[GraphALT]\n");

	PutGraphExport(fini);

	if (!CurrFileIDPath) {
		fprintf(fini, "\n[FileIDs]\n");

		/* we need a FileID registry */
#ifdef _OLD_IDS
		if (book) {
			bcp = &BookComps;
			if (Fid < 676) /* limit for two letters is 26 * 26 */
				while (*bcp) {	/* list chapters with prefix name */
					temp1 = Fm2a((*bcp)->bcname);
					fprintf(fini, "%s=%c%c\n", temp1,
									 (((*bcp)->bfid) / 26) + 'a',
									 (((*bcp)->bfid) % 26) + 'a');
					bcp = &((*bcp)->bcnext);
					F_Free(temp1);
				}
			else  /* FM books won't exceed 17576 chapters */
				while (*bcp) {
					temp1 = Fm2a((*bcp)->bcname);
					fprintf(fini, "%s=%c%c%c\n", temp1,
									 (((*bcp)->bfid) / 676) + 'a',
									 ((((*bcp)->bfid) / 26) % 26) + 'a',
									 (((*bcp)->bfid) % 26) + 'a');

					bcp = &((*bcp)->bcnext);
					F_Free(temp1);
				}
		}
		else {
			ch = tolower(basename[F_StrLen(basename) - 1]);
			if (isdigit(ch))  /* 1 = a, so put zero last in sequence */
				ch = (ch == '0') ? 'j' : ('a' + (ch - '1'));
			fprintf(fini, "%s=%c%c\n", basename, tolower(basename[0]), ch);
		}
#else
		if (book) {
			bcp = &BookComps;
			while (*bcp) {
				temp1 = Fm2a((*bcp)->bcname);
				fprintf(fini, "%s=%s\n", temp1, NextFileID);
				if (!IncAlpha(NextFileID)) {
					temp2 = F_StrNew(F_StrLen(NextFileID) + 2);
					F_StrCpy(temp2, NextFileID);
					F_StrFree(NextFileID);
					NextFileID = temp2;
					F_StrCat(NextFileID, "a");
				}
				bcp = &((*bcp)->bcnext);
				F_Free(temp1);
			}
		}
		else {
			fprintf(fini, "%s=%s\n", basename, NextFileID);
		}
#endif
	}
	else {
		if (book) {
			bcp = &BookComps;
			while (*bcp) {
				temp1 = Fm2a((*bcp)->bcname);
				fname = NULL;
				GetIniString(CurrFileIDPath, "FileIDs", temp1, &fname);
				if (!fname)   /* need to create entry for file */
					SetFileID(temp1);
				bcp = &((*bcp)->bcnext);
				F_Free(temp1);
			}
		}
		else {
			fname = NULL;
			GetIniString(CurrFileIDPath, "FileIDs", basename, &fname);
			if (!fname)   /* need to create entry for file */
				SetFileID(basename);
		}
	}

	if (hform
	 || help
	 || (form == 'W')
	 || (form == '8')) {
		// compose list of styles and levels from TOC file
		// to fill in the next two sections
		if (TOCname) {
			if ((TOCId = IdentifyDocName(TOCname)) != 0) { /* already open */
				GetStyles(TOCId, True);
			}
			else if ((TOCId = OpenDocName(TOCname)) != 0) {
				GetStyles(TOCId, True);
				F_ApiClose(TOCId, FF_CLOSE_MODIFIED);  /* put away TOC doc */
			}
		}
		else {  /* pick styles from doc itself */
			if (book) { /* choose a file to pick from */
				docId = F_ApiGetId(FV_SessionId, bookId, FP_FirstComponentInBook);
				while (docId) {
					if (!F_ApiGetInt(bookId, docId, FP_BookComponentIsGeneratable)
					 && ((TOCname = F_ApiGetString(bookId, docId, FP_Name)) != NULL)) {
						if ((TOCId = IdentifyDocName(TOCname)) != 0) {
							GetStyles(TOCId, False);
							break;
						}
						else if ((TOCId = OpenDocName(TOCname)) != 0) {
							GetStyles(TOCId, False);
							F_ApiClose(TOCId, FF_CLOSE_MODIFIED);  /* put away doc */
							break;
						}
					}
					docId = F_ApiGetId(bookId, docId, FP_NextComponentInBook);
				}
			}
			else {
				docId = IdentifyDocName(filename);
				GetStyles(docId, False);
			}
		}
	}

	if (hform) {  /* options for WinHelp */
#if 0
		fprintf(fini, "\n[WordOptions]\n");
		fprintf(fini, "RefFrames=Graphic\n");
#endif
		fprintf(fini, "\n[HelpOptions]\n");
		fprintf(fini, "%s=%s\n", HPJLabel, temp1 = Fm2a(HPJName));
		F_Free(temp1);
		fprintf(fini, "HelpVersion=%c\n", (form == '3') ? '3' : '4');
		fprintf(fini, "MakeCombinedCnt=%s\n", MakeCombCnt ? "Yes" : "No");
		bname = Fm2a(temp1 = BaseFileName(HelpFileName));
		F_Free(temp1);
		fprintf(fini, "XrefFileDefault=%s\n", bname);
#if 0
		if ((book || MakeCombCnt)
		 && (UseFmTOC || UseFmIX)) {
			fprintf(fini, "ObjectIDs=Referenced\n"); /* if TOC or IX present */
		}
#endif
		//fprintf(fini, "RefFrames=Graphic\n");

		fprintf(fini, "\n[HelpStyles]\n");
		psp = &ParaStyles;
		while (*psp && (*psp)->slev) {
			temp1 = Fm2a((*psp)->psname);
			fprintf(fini, "%s=Topic Browse Contents\n", temp1);
			psp = &((*psp)->psnext);
			F_Free(temp1);
		}

		fprintf(fini, "\n[HelpCntStyles]\n");
		psp = &ParaStyles;
		while (*psp && (*psp)->slev) {
			temp1 = Fm2a((*psp)->psname);
			fprintf(fini, "%s=%s\n", temp1, (*psp)->slev);
			psp = &((*psp)->psnext);
			F_Free(temp1);
		}

		fprintf(fini, "\n[HelpContents]\n");
		fprintf(fini, "CntType=%s\n", book || MakeCombCnt ? "Body" : "Full");
		if (book || MakeCombCnt) {
			SetCntVars(bname, NULL);
			fprintf(fini, "CntTopic=%s\n", temp1 = Fm2a(HelpContentsTopic));
			F_Free(temp1);
			fprintf(fini, "CntBase=%s\n", temp1 = Fm2a(HelpFileName));
			F_Free(temp1);
			fprintf(fini, "CntName=%s\n", temp1 = Fm2a(CntFileName));
			F_Free(temp1);
		}
		fprintf(fini, "CntBStyleText=Overview\n");

		fprintf(fini, "\n[HelpBrowse]\n");
		fprintf(fini, "Step=1\n");
		fprintf(fini, "Digits=%ld\n", BrowseDigits);
		fprintf(fini, "Start=%ld\n", BrowseStart);
		fprintf(fini, "Prefix=HLP\n");

		if (book) {
#if 0
			fprintf(fini, "\n[BrowsePrefix]\n");
			bcp = &BookComps;
			while (*bcp) {	/* list chapters with prefix name */
				if ((*bcp)->px)
					fprintf(fini, "%s=%s\n", (*bcp)->bcname, (*bcp)->px);
				else
					fprintf(fini, "%s=%c%c\n", (*bcp)->bcname,
									((*bcp)->bfid/26) + 'A', ((*bcp)->bfid % 26) + 'A');
				bcp = &((*bcp)->bcnext);
			}
#else
			fprintf(fini, "\n[BrowseStart]\n");
			bcp = &BookComps;
			brStart = BrowseStart;
			while (*bcp) {	/* list chapters with starting number */
				temp1 = Fm2a((*bcp)->bcname);
				fprintf(fini, "%s=%ld\n", temp1, brStart);
				if (brStart == BrowseStart)  /* add real topic counting later */
					brStart = BrowseTopicsMin;
				else
					brStart += BrowseTopicsMin;
				bcp = &((*bcp)->bcnext);
				F_Free(temp1);
			}
#endif
		}

		F_StrFree(bname);
	}
	else if (html) {  /* options for HTML use */
		fprintf(fini, "\n[HTMLOptions]\n");
		fprintf(fini, "ObjectIDs=%s\n", help ? "All" : "Referenced"); 
		fprintf(fini, "FileSuffix=%s\n", (form == 'D') ? ".dita" :
																		((form == 'B') ? DocBookExt :
																	  ((form == 'G') ? XMLSuffix :
																		  HTMLSuffix)));
		fprintf(fini, "NoFonts=Yes\n"); 
		fprintf(fini, "AlignAttributes=No\n"); 
		fprintf(fini, "AllowOverrides=No\n"); 
		fprintf(fini, "StartingSplit=No\n"); 

		if (((form != 'D')
		  && (form != 'B'))
		 || UseCSS) {
			fprintf(fini, "\n[CSS]\n");
			fprintf(fini, "UseCSS=%s\n", UseCSS ? "Yes" : "No");
			fprintf(fini, "WriteCssStylesheet=%s\n",
											(UseCSS && CreateCSS) ? "Once" : "Never");
			fprintf(fini, "CssFileName=%s\n", temp1 = Fm2a(CssFileName));
			F_Free(temp1);
		}
		else {
			fprintf(fini, "\n[CSS]\n");
			fprintf(fini, "UseCSS=No\n");
		}

		if (form != 'D') {
			fprintf(fini, "\n[Tables]\n");
			if (form == 'B')
				fprintf(fini, "UseCALSModel=%s\n", UseCALSTableModel ? "Yes" : "No"); 
			else {
				fprintf(fini, "TableSizing=%s\n", (TableSizing == 'A') ? "Adaptive" :
															 ((TableSizing == 'F') ? "Fixed" : "Percent"));
				if (TableSizing == 'F')
					fprintf(fini, "TableDPI=%s\n", TableDPI);

				fprintf(fini, "Border=%s\n", TblBorder);
				fprintf(fini, "Spacing=%s\n", TblSpacing);
				fprintf(fini, "Padding=%s\n", TblPadding);
			}
			fprintf(fini, "\n[Titles]\n");
			fprintf(fini, "\n[HtmlFiles]\n");
			fprintf(fini, "\n[XrefFiles]\n");
			fprintf(fini, "\n[ParaStyles]\n");
			fprintf(fini, "\n[CharStyles]\n");
			fprintf(fini, "\n[Classes]\n");
			fprintf(fini, "\n[HtmlStyles]\n");
			if ((form == 'C')
			 || (form == 'M')
			 || (form == 'E')
			 || (form == 'O')
			 || (form == 'J')) {
				psp = &ParaStyles;
				while (*psp && (*psp)->slev) {
					temp1 = Fm2a((*psp)->psname);
					fprintf(fini, "%s=Title Split\n", temp1);
					psp = &((*psp)->psnext);
					F_Free(temp1);
				}
			}
		}

		if (form == 'D') {
			fprintf(fini, "\n[DITAOptions]\n");
			fprintf(fini, "DITAVer=%s\n", DITADefVer11 ? "1" : "0");
			fprintf(fini, "DefTopic=%s\n", DITADefTopic);
			fprintf(fini, "DefParaElem=%s\n", DITADefPara);
			fprintf(fini, "DefCharElem=%s\n", DITADefChar);

			fprintf(fini, "\n[DITAParaTags]\n");
			fprintf(fini, "Body=p\n");
			fprintf(fini, "Heading*=title\n");
			fprintf(fini, "Numbered=li\n");
			fprintf(fini, "Numbered1=li\n");
			fprintf(fini, "Bulleted=li\n");
			fprintf(fini, "FigureTitle=title\n");
			fprintf(fini, "TableTitle=title\n");
			fprintf(fini, "CellBody=p\n");
			fprintf(fini, "CellHeading=p\n");

			fprintf(fini, "\n[DITAParaAttributes]\n");
			fprintf(fini, "\n[DITAParaTypographics]\n");

			fprintf(fini, "\n[DITACharTags]\n");
			fprintf(fini, "Emphasis=i\n");

			fprintf(fini, "\n[DITACharAttributes]\n");
			fprintf(fini, "\n[DITACharTypographics]\n");

			fprintf(fini, "\n[DITALevels]\n");
			fprintf(fini, "Title=1\n");
			fprintf(fini, "Heading*=3\n");

			fprintf(fini, "\n[DITAParents]\n");
			fprintf(fini, "Title=topic\n");
			fprintf(fini, "Heading*=section\n");
			fprintf(fini, "Numbered1=ol\n");
			fprintf(fini, "Numbered=ol\n");
			fprintf(fini, "Bulleted=ul\n");
			fprintf(fini, "FigureTitle=fig\n");

			fprintf(fini, "\n[DITAParentAttributes]\n");

			fprintf(fini, "\n[DITAFirst]\n");
			fprintf(fini, "Heading*=section\n");
			fprintf(fini, "Numbered1=ol\n");

			fprintf(fini, "\n\n[DITAAliases]\n");
			fprintf(fini, "\n[DITACloseBefore]\n");
			fprintf(fini, "\n[DITACloseAfter]\n");
			fprintf(fini, "\n[DITAOpenBefore]\n");
			fprintf(fini, "\n[DITAOpenAfter]\n");

			fprintf(fini, "\n\n[DITAElementSets]\n");
			fprintf(fini, "$top=body section step\n");
			fprintf(fini, "$text=body conbody taskbody refbody section example step refsyn entry\n");
			fprintf(fini, "$list=ol ul\n");

			fprintf(fini, "\n\n[DITATopics]\n");
			fprintf(fini, "\n[DITATables]\n");

			fprintf(fini, "\n\n[DITAMapLevels]\n");
			fprintf(fini, "\n[DITAMapUsage]\n");
			fprintf(fini, "\n[DITARelGroups]\n");
			fprintf(fini, "\n[DITARelBookGroups]\n");

			fprintf(fini, "\n\n[DITAContentModels]\n");

			fprintf(fini, "\n\n[MarkerTypes]\n\n");
		}

		if (form == 'B') {
			fprintf(fini, "\n[DocBookOptions]\n");
			fprintf(fini, "DocBookRoot=%s\n", DocBookRoot);
			fprintf(fini, "WriteBookFile=%s\n", DocBookChapFull ? "No" : "Yes"); 
			if (!DocBookChapFull) {
				fprintf(fini, "BookFileName=%s\n", temp1 = Fm2a(DocBookName));
				F_Free(temp1);
				fprintf(fini, "BookFileTitle=%s\n", temp1 = Fm2a(DocBookTitle));
				F_Free(temp1);
			}
			fprintf(fini, "DefParaElem=para\n");
			fprintf(fini, "DefCharElem=phrase\n");

			fprintf(fini, "\n\n[DocBookParaTags]\n");
			fprintf(fini, "\n[DocBookParaIDs]\n");
			fprintf(fini, "\n[DocBookParaAttributes]\n");

			fprintf(fini, "\n\n[DocBookCharTags]\n");
			fprintf(fini, "\n[DocBookCharAttributess]\n");

			fprintf(fini, "\n\n[DocBookParents]\n");
			fprintf(fini, "\n[DocBookParentIDs]\n");
			fprintf(fini, "\n[DocBookParentAttributes]\n");

			fprintf(fini, "\n\n[DocBookFirst]\n");

			fprintf(fini, "\n\n[DocBookCloseBefore]\n");
			fprintf(fini, "\n[DocBookCloseAfter]\n");
			fprintf(fini, "\n[DocBookOpenBefore]\n");
			fprintf(fini, "\n[DocBookOpenAfter]\n");

			fprintf(fini, "\n\n[DocBookAliases]\n");
			fprintf(fini, "\n[DocBookElementSets]\n");
			fprintf(fini, "\n$top=chapter appendix preface article");
			fprintf(fini, "\n$sections=sect1 sect2 sect3 sect4 sect5 section simplesect");
			fprintf(fini, "\n$text=sect1 sect2 sect3 sect4 sect5 section simplesect chapter appendix preface article entry");
			fprintf(fini, "\n$list=itemizedlist orderedlist");

			fprintf(fini, "\n\n[MarkerTypes]\n\n");
		}

		fprintf(fini, "\n[Inserts]\n");

		if (form == 'M') {
			fprintf(fini, "\n[MSHtmlHelpOptions]\n");
			fprintf(fini, "%s=%s\n", HPJLabel, temp1 = Fm2a(HPJName));
			F_Free(temp1);
			fprintf(fini, "WriteHelpProjectFile=%s\n", WriteHelpProjectFile ? "Yes" : "No");
			fprintf(fini, "HelpFileTitle=%s\n", temp1 = Fm2a(HelpTitle));
			F_Free(temp1);
			fprintf(fini, "DefaultChmFile=%s\n", DefaultChmFile);
			fprintf(fini, "DefaultTopicFile=%s\n", DefaultTopicFile);
			fprintf(fini, "UseFTS=%s\n", UseFTS ? "Yes" : "No");
			fprintf(fini, "RefFileType=%s\n", (!GenTOC && !GenIX) ? "None" :
			                                  book ? "Body" : "Full");
			fprintf(fini, "ListType=%s\n", (GenTOC && GenIX) ? "Both" :
			                              GenTOC ? "Contents" :
																		GenIX  ? "Index" : "Both");
			if (CshMapFile)
				fprintf(fini, "CshMapFile=%s\n", CshMapFile);
			fprintf(fini, "MakeAliasFile=%s\n", UseAlias ? "Yes" : "No");
		}
		else if ((form == 'J')
		 || (form == 'O')) {
			fprintf(fini, "\n[JavaHelpOptions]\n");
			fprintf(fini, "%s=%s\n", HPJLabel, temp1 = Fm2a(HPJName));
			F_Free(temp1);
			if (FTSCommand) {
				fprintf(fini, "FTSCommand=%s\n", temp1 = Fm2a(FTSCommand));
				F_Free(temp1);
			}
			fprintf(fini, "WriteHelpSetFile=%s\n", WriteHelpSetFile ? "Yes" : "No");
			fprintf(fini, "DefaultTopic=%s\n", DefaultTopicFile);
			fprintf(fini, "HelpFileTitle=%s\n", temp1 = Fm2a(HelpTitle));
			F_Free(temp1);
			fprintf(fini, "RefFileType=%s\n", (!GenTOC && !GenIX) ? "None" :
			                                  book ? "Body" : "Full");
			fprintf(fini, "ListType=%s\n", (GenTOC && GenIX) ? "Both" :
			                              GenTOC ? "Contents" :
																		GenIX  ? "Index" : "Both");
			if (MapFilePrefix && *MapFilePrefix)
				fprintf(fini, "MapFilePrefix=%s\n", MapFilePrefix);
		}
		else if (form == 'E') {
			fprintf(fini, "\n[EclipseHelpOptions]\n");
			fprintf(fini, "PluginName=%s\n", temp1 = Fm2a(PluginName));
			F_Free(temp1);
			fprintf(fini, "PluginID=%s\n", temp1 = Fm2a(PluginID));
			F_Free(temp1);
			fprintf(fini, "PluginProvider=%s\n", temp1 = Fm2a(Provider));
			F_Free(temp1);
			fprintf(fini, "WritePlugin=%s\n", WritePluginFile ? "Yes" : "No");
			fprintf(fini, "TocTopic=%s\n", DefaultTopicFile);
			fprintf(fini, "UseIndex=%s\n", GenIX ? "Yes" : "No");
			fprintf(fini, "UseContext=%s\n", GenCSH ? "Yes" : "No");
			fprintf(fini, "WriteContext=%s\n", WriteContextFile ? "Yes" : "No");
			fprintf(fini, "RefFileType=%s\n", book ? "Body" : "Full");
			fprintf(fini, "ListType=%s\n", GenIX ? "Both" : "Contents" );
		}
		else if (form == 'C') {
			fprintf(fini, "\n[OmniHelpOptions]\n");
			fprintf(fini, "ProjectName=%s\n", temp1 = Fm2a(OHProjName));
			F_Free(temp1);
			fprintf(fini, "HelpFileTitle=%s\n", temp1 = Fm2a(HelpTitle));
			F_Free(temp1);
			fprintf(fini, "DefaultTopic=%s\n", DefaultTopicFile ? DefaultTopicFile : "");
			if (UseOHTOC || UseOHIX || UseOHFTS || UseOHREL) {
				fprintf(fini, "NavElems=");
				if (UseOHTOC)
					fprintf(fini, " Toc");
				if (UseOHIX)
					fprintf(fini, " Idx");
				if (UseOHFTS)
					fprintf(fini, " Fts");
				if (UseOHREL)
					fprintf(fini, " Rel");
				fprintf(fini, "\n");
			}
			fprintf(fini, "\nMainCssName=%s\n",temp1 = Fm2a(CssFileName));
			fprintf(fini, "IECssName=%s\n", temp1);
			fprintf(fini, "N6CssName=%s\n", temp1);
			fprintf(fini, "N4CssName=%s\n", temp1);
			F_Free(temp1);
			fprintf(fini, "\nUseFTS=%s\n", UseOHFTS ? "Yes" : "No");
			fprintf(fini, "RefFileType=%s\n", book ? "Body" : "Full");
		}

		if ((form == 'C')
		 || (form == 'M')
		 || (form == 'E')
		 || (form == 'J')
		 || (form == 'O')) {
			fprintf(fini, "\n[HelpContentsLevels]\n");
			fprintf(fini, "; Fill in FM head para formats and their level here\n");
			psp = &ParaStyles;
			while (*psp && (*psp)->slev) {
				temp1 = Fm2a((*psp)->psname);
				fprintf(fini, "%s=%s\n", temp1, (*psp)->slev + 1);
				psp = &((*psp)->psnext);
				F_Free(temp1);
			}
		}
	}
	else {  /* options for print use */
#if 0
		fprintf(fini, "\n[HelpOptions]\n");
		fprintf(fini, "RefFrames=Graphic\n");
#endif
		fprintf(fini, "\n[WordOptions]\n");
		fprintf(fini, "Word8=%s\n", (form == '8') ? "Yes" : "No");
		fprintf(fini, "WordPerfect=%s\n", (form == 'P') ? "Yes" : "No");

		if (UseWordTemplate) {
			fprintf(fini, "Template=%s\n", temp1 = Fm2a(WordTemplateName));
			F_Free(temp1);
			fprintf(fini, "TemplateAutoUpdate=%s\n", WordTemplateAuto ? "Yes" : "No");
		}

		//fprintf(fini, "ObjectIDs=None\n");

		fprintf(fini, "HeadFoot=%s\n", (Headfeet == 'S') ? "Standard" :
																	((Headfeet == 'T') ? "Text" : "None"));
		fprintf(fini, "HFFramed=%s\n", FramedHF ? "Yes" : "No");

		fprintf(fini, "Sideheads=%s\n", (Sideheads == 'F') ? "Frame" :
																	 ((Sideheads == 'I') ? "Indent" : "Left"));
		//fprintf(fini, "RefFrames=Graphic\n");

		fprintf(fini, "\n[WordStyles]\n");

		fprintf(fini, "\n[WordSectionFiles]\n");
		if (book) {
			bcp = &BookComps;
			while (*bcp) {	/* list chapters with section nums */
				if ((*bcp)->px) {
					temp1 = Fm2a((*bcp)->bcname);
					temp2 = Fm2a((*bcp)->px);
					fprintf(fini, "%s=%s\n", temp1, temp2);
					F_Free(temp1);
					F_Free(temp2);
				}
				bcp = &((*bcp)->bcnext);
			}
		}
#if 0
		else {
			docId = IdentifyDocName(filename);
			basename = BaseFileName(filename);
			fprintf(fini, "%s=%s\n", basename, /* section numbering px */);
			F_StrFree(basename);
		}
#endif

		if ((form == 'W')
		 || (form == '8')) {
			fprintf(fini, "\n[WordCntStyles]\n");
			psp = &ParaStyles;
			temp2 = "";
			while (*psp && (*psp)->slev) {
				temp1 = Fm2a((*psp)->psname);
				if (F_StrICmp(temp1, temp2)) {
					fprintf(fini, "%s=%s\n", temp1, (*psp)->slev + 1);
					temp2 = temp1;
				}
				psp = &((*psp)->psnext);
			}
		}
	}

	// macros for all formats now
	fprintf(fini, "\n\n[MacroVariables]\n");
	fprintf(fini, "; Put any macro definition sections before this section.\n");

	fclose(fini);


	/* clean up */
	F_StrFree(basename);
	if (BookComps)
		FreeChapters();
	if (TOCname) {
		F_StrFree(TOCname);
		TOCname = NULL;
		TOCId = 0;
	}
	if (ParaStyles)
		FreeStyles();
}


VoidT UpdateBasicIni(StringT ininame, StringT baename,
                     BoolT book, UCharT form)
{
	/* add FileIDs */

	/* add SectionNum setting */

	/* see about formats */

}



/* chapter processing */

VoidT GetChapters(F_ObjHandleT bookId)
{
	BookComp **bcp = &BookComps;
	F_ObjHandleT compId = 0;

	IntT bctype = FV_BK_NOT_GENERATABLE;
	StringT chap = NULL;
	StringT px = NULL;
	IntT chaplen = ChapLenMax = 0;

	// go through book, getting chap name, fid, and {browse px or sectId}
	// into a list so that the three filename sections can be filled out
	// note the UseFmTOC and UseFmIX settings
	compId = F_ApiGetId(FV_SessionId, bookId, FP_FirstComponentInBook);
	Fid = -1;
	bcp = &BookComps;
	while (compId) {
		bctype = F_ApiGetInt(bookId, compId, FP_BookComponentType);
		if (bctype == FV_BK_TOC)
			TOCname = F_ApiGetString(bookId, compId, FP_Name);
		if (((bctype == FV_BK_TOC) && UseFmTOC)
		 || ((bctype == FV_BK_INDEX_STAN) && UseFmIX)
		 || ((bctype != FV_BK_TOC) && (bctype != FV_BK_INDEX_STAN))) {
			*bcp = F_Alloc(sizeof(BookComp), NO_DSE);
			(*bcp)->bcId = compId;
			chap = F_ApiGetString(bookId, compId, FP_Name);
			(*bcp)->bcname = BaseFileName(chap);
			if ((chaplen = F_StrLen((*bcp)->bcname)) > ChapLenMax)
				ChapLenMax = chaplen;
			F_StrFree(chap);
			chap = F_ApiGetString(bookId, compId, FP_PagePrefix);
			(*bcp)->px = *chap ? chap : NULL;
			chap = NULL;
			(*bcp)->bfid = ++Fid;
			(*bcp)->bcnext = NULL;
			bcp = &((*bcp)->bcnext);
		}
		compId = F_ApiGetId(bookId, compId, FP_NextComponentInBook);
	}
}


IntT GetMetaNameMin(VoidT)
{
	BookComp **bcp = &BookComps;
	IntT pos = 0;
	IntT i = 0;
	IntT n = 0;
	StringT *nm = NULL;
	BoolT match = False;

	nm = (StringT *) F_Alloc((Fid + 1) * sizeof(StringT), NO_DSE);
	for (i = 0; i < ChapLenMax; i++) {
		match = False;
		bcp = &BookComps;
		for (pos = 0; pos < (Fid + 1); pos++) {
			nm[pos] = F_StrNew(i + 2);
			F_StrCpyN(nm[pos], (*bcp)->bcname, i + 2);
			bcp = &((*bcp)->bcnext);
		}
		for (pos = 0; pos < Fid; pos++) {
			for (n = pos + 1; n < (Fid + 1); n++) {
				if (!F_StrICmp(nm[pos], nm[n])) {
					match = True;
					break;
				}
			}
			if (match)
				break;
		}
		for (pos = 0; pos < (Fid + 1); pos++)
			F_StrFree(nm[pos]);
		if (!match)
			break;
	}				
	F_Free(nm);
	if (match) /* matching names to limit, use FileIDs */
		return 0;
	return i;
}


StringT GetBookTOC(F_ObjHandleT bookId)
{
	F_ObjHandleT compId = 0;
	IntT bctype = FV_BK_NOT_GENERATABLE;

	compId = F_ApiGetId(FV_SessionId, bookId, FP_FirstComponentInBook);
	while (compId) {
		bctype = F_ApiGetInt(bookId, compId, FP_BookComponentType);
		if (bctype == FV_BK_TOC)
			return F_ApiGetString(bookId, compId, FP_Name);
		compId = F_ApiGetId(bookId, compId, FP_NextComponentInBook);
	}
	return NULL;
}


VoidT FreeChapters(VoidT)
{
	BookComp *bcnp = NULL;

	while (BookComps) {
		F_StrFree(BookComps->bcname);
		if (BookComps->px)
			F_StrFree(BookComps->px);
		bcnp = BookComps->bcnext;
		F_Free(BookComps);
		BookComps = bcnp;
	}
	ChapLenMax = 0;
	Fid = 0;
}


/* style processing */

VoidT GetStyles(F_ObjHandleT fileId, BoolT inTOC)
{
	ParaStyle **psp = &ParaStyles;
	F_ObjHandleT compId = 0;
	StringT style = NULL;
	IntT toc = -1;
	BoolT skipping = False;

	IntT styleCount = 0;
	IntT maxStyles = 30;  /* arbitrary */
	UCharT maxLevel = '4';

	IntT psplace = 0;
	IntT lastplace = 0;
	MetricT lastindent = 0;
	MetricT lastpsize = 0;
	IntT lastweight = 0;

	psp = &ParaStyles;
	compId = F_ApiGetId(FV_SessionId, fileId, FP_FirstPgfFmtInDoc);
	while (compId) {
		style = F_ApiGetString(fileId, compId, FP_Name);
		if ((style && *style)
		 && ((!inTOC
			 && PgfUsedInDoc(fileId, style)
		   && ((toc = F_StrLen(style)) != 0))
		  || (inTOC
		   && (((toc = F_StrSubString(style, "TOC")) > 0)
			  && (toc == (F_StrLen(style) - 3)))))) {
			*psp = F_Alloc(sizeof(ParaStyle), NO_DSE);
			(*psp)->psname = F_StrNew(toc + 1);
			F_StrCpyN((*psp)->psname, style, toc + 1);
			(*psp)->psId = compId;
			psplace = F_ApiGetInt(fileId, compId, FP_Placement);
			switch (psplace) {
				case FV_PGF_STRADDLE:
					(*psp)->place = 1;
					break;
				case FV_PGF_SIDEHEAD_TOP:
				case FV_PGF_SIDEHEAD_FIRST_BASELINE:
				case FV_PGF_SIDEHEAD_LAST_BASELINE:
					(*psp)->place = 2;
					break;
				case FV_PGF_STRADDLE_NORMAL_ONLY:
					(*psp)->place = 3;
					break;
				case FV_PGF_SIDEBODY:
					(*psp)->place = 4;
					break;
				case FV_PGF_RUN_IN:
					(*psp)->place = 5;
					break;
				default:
					(*psp)->place = 6;
					break;
			}
			(*psp)->indent = F_ApiGetMetric(fileId, compId, FP_FirstIndent);
			(*psp)->psize = F_ApiGetMetric(fileId, compId, FP_FontSize);
			(*psp)->weight = F_ApiGetInt(fileId, compId, FP_FontWeight);
			(*psp)->psnext = NULL;
			(*psp)->slev = NULL;
			psp = &((*psp)->psnext);
		}
		F_StrFree(style);
		compId = F_ApiGetId(fileId, compId, FP_NextPgfFmtInDoc);
	}
	// sort list into desc order of importance and determine levels
	// use FP_FirstIndent and FP_FontSize, both MetricT (long) values
	// FV_METRIC_POINT is the MetricT value for one pt, 0x00010000
	// BoolT F_MetricApproxEqual(x, y) is True if x is near y
	*psp = z;  /* add guard to end for sort */
	ParaStyles = PSmergesort(ParaStyles);
	psp = &ParaStyles;
	lastplace = (*psp)->place;
	lastindent = (*psp)->indent;
	lastpsize = (*psp)->psize;
	lastweight = (*psp)->weight;
	skipping = False;
	Level[1] = '1';

	/* traverse list and assign levels */
	for (; *psp != z; psp = &((*psp)->psnext)) {
		if (skipping)
			continue;
		if (!inTOC
		 && (styleCount > maxStyles)) {
			skipping = True;
			continue;
		}
		if ((Level[1] < '9')
		 && (((*psp)->place != lastplace)
			|| !F_MetricApproxEqual((*psp)->indent, lastindent)
			|| !F_MetricApproxEqual((*psp)->psize, lastpsize)
			|| ((*psp)->weight != lastweight))) {
			lastplace = (*psp)->place;
			lastindent = (*psp)->indent;
			lastpsize = (*psp)->psize;
			lastweight = (*psp)->weight;
			(Level[1])++;
		}
		if (!inTOC
		 && (Level[1] > maxLevel)) {
			skipping = True;
			continue;
		}
		(*psp)->slev = F_StrCopyString(Level);
		styleCount++;
	}

	*psp = NULL;  /* remove guard from end */

	if (inTOC && UseFmTOC) {
		// find main flow, then first text frame, then FP_FirstPgf
		compId = F_ApiGetId(FV_SessionId, fileId, FP_MainFlowInDoc);
		compId = F_ApiGetId(TOCId, compId, FP_FirstTextFrameInFlow);
		compId = F_ApiGetId(TOCId, compId, FP_FirstPgf);
		style = F_ApiGetString(TOCId, compId, FP_Name);
		if ((toc = F_StrSubString(style, "TOC")) != -1) /* in list */ 
			F_StrFree(style);
		else if (style && *style) {  /* not in list, add at top level */
			Level[1] = '1';
			*psp = F_Alloc(sizeof(ParaStyle), NO_DSE);
			(*psp)->psname = style;
			(*psp)->slev = F_StrCopyString(Level);
			(*psp)->psnext = NULL;
		}
	}
}


BoolT PgfUsedInDoc(F_ObjHandleT docId, StringT style)
{
	F_ObjHandleT pageId = 0;
	F_ObjHandleT pframeId = 0;
	F_ObjHandleT tframeId = 0;
	F_ObjHandleT lastId = 0;
	F_ObjHandleT compId = 0;
	F_ObjHandleT subId = 0;
	StringT pstyle = NULL;

	for (pageId = F_ApiGetId(FV_SessionId, docId, FP_FirstBodyPageInDoc);
	 pageId != 0;
	 pageId = F_ApiGetId(docId, pageId, FP_PageNext)) {
		pframeId = F_ApiGetId(docId, pageId, FP_PageFrame);
		for (tframeId = F_ApiGetId(docId, pframeId, FP_FirstGraphicInFrame);
		 tframeId != 0;
		 tframeId = F_ApiGetId(docId, tframeId, FP_NextGraphicInFrame)) {
			if (F_ApiGetObjectType(docId, tframeId) != FO_TextFrame)
				continue;
			lastId = F_ApiGetId(FV_SessionId, tframeId, FP_LastPgf);
			for (compId = F_ApiGetId(FV_SessionId, tframeId, FP_FirstPgf);
			 compId != 0;
			 compId = F_ApiGetId(docId, compId, FP_NextPgfInFlow)) {
				subId = F_ApiGetId(docId, compId, FP_InTextObj);
				if ((subId == FO_Fn) || (subId == FO_Cell))
					continue;
				if ((pstyle = F_ApiGetString(docId, compId, FP_Name)) != NULL) {
					if (!F_StrICmp(style, pstyle)) {
						F_StrFree(pstyle);
						return True;
					}
					F_StrFree(pstyle);
				}
				if (compId == lastId)
					break;
			}
		}
	}
	return False;
}


VoidT FreeStyles(VoidT)
{
	ParaStyle *psnp = NULL;

	while (ParaStyles) {
		F_StrFree(ParaStyles->psname);
		F_StrFree(ParaStyles->slev);
		psnp = ParaStyles->psnext;
		F_Free(ParaStyles);
		ParaStyles = psnp;
	}
}


/* mergesort, Sedgewick, Algorithms in C, pp. 165-166 */

BoolT PScmp(ParaStyle *a, ParaStyle *b)
{
	if (a->place == b->place) {
		if (F_MetricApproxEqual(a->indent, b->indent)) {
			if (F_MetricApproxEqual(a->psize, b->psize)) {
				if (a->weight >= b->weight) /* descending boldness */
					return True;
			}
			else if (a->psize > b->psize) /* descending pt size */
				return True;
		}
		else if (a->indent < b->indent) /* ascending indent */
			return True;
	}
	else if (a->place < b->place)     /* ascending place value */
		return True; /* a and b are already ordered */

	return False;  /* a and b need to be swapped */
}


ParaStyle *PSmerge(ParaStyle *a, ParaStyle *b)
{
	ParaStyle *c;

	c = z;
	do {
		if (PScmp(a, b)) {
			c->psnext = a;
			c = a;
			a = a->psnext;
		}
		else {
			c->psnext = b;
			c = b;
			b = b->psnext;
		}
	} while (c != z);
	c = z->psnext; 
	z->psnext = z;
	return c;
}


ParaStyle *PSmergesort(ParaStyle *c)
{
	ParaStyle *a, *b;

	if (c->psnext != z) {
		a = c;
		b = c->psnext->psnext->psnext;
		while (b != z) {
			c = c->psnext;
			b = b->psnext->psnext;
		}
		b = c->psnext;
		c->psnext = z;
		return PSmerge(PSmergesort(a), PSmergesort(b));
	}
	return c;
}


/* .hpj writing function */

VoidT WriteHelpProj(StringT inipath, StringT filename,
										BoolT book, UCharT form, StringT ininame)
{
	StringT ip = NULL;
	StringT in = NULL;

	ip = Fm2a(inipath);
	in = Fm2a(ininame);

	if ((form == '3') || (form == 'H'))
		WriteHPJFile(ip, filename, book, form, in);
#if 0
	else if ((form == 'J') || (form == 'O'))
		WriteHSFile(ip, filename, book, form);
	else if (form == 'M')
		WriteHHPFile(ip, filename, book, form);
#endif
	F_Free(ip);
	F_Free(in);
}


VoidT WriteHPJFile(StringT inipath, StringT filename,
									 BoolT book, UCharT form, StringT ininame)
{
	FILE *fini = NULL;
	BookComp **bcp = &BookComps;
	StringT hpjname = NULL;
	StringT basename = NULL;
	StringT temp1 = NULL;

	basename = Fm2a(temp1 = BaseFileName(filename));
	F_Free(temp1);

	if ((HPJName == NULL)
	 || (F_StrSubString(HPJName, ".hpj") == -1)) {
		HPJName = F_Alloc(F_StrLen(basename) + 5, NO_DSE);
		F_StrCpy(HPJName, basename);
		F_StrCat(HPJName, ".hpj");
	}

	hpjname = MakeFileName(inipath, Fm2a(HPJName));

	if ((fini = fopen(hpjname, "rt")) != NULL) {
		fclose(fini);
		F_StrFree(hpjname);
		F_StrFree(basename);
		return;
	}

	if ((fini = fopen(hpjname, "wt")) == NULL) {
		F_StrFree(hpjname);
		F_StrFree(basename);
		return;
	}

	SetCntVars(basename, ininame);

	/* the exact sequence of entries in the .hpj is critical */

	fprintf(fini, "; This file is maintained by HCW.");
	fprintf(fini, " Do not modify this file directly.\n\n");
	fprintf(fini, "\n[OPTIONS]\n");
	fprintf(fini, "COMPRESS=%s\n", (form == '3') ? "High" : "12 Hall Zeck");
	if (form != '3')
		fprintf(fini, "LCID=0x409 0x0 0x0 ; U.S. English\n");
	fprintf(fini, "REPORT=Yes\n");
	if (form != '3') {
		fprintf(fini, "CONTENTS=%s\n", temp1 = Fm2a(HelpContentsTopic));
		F_Free(temp1);
	}
	fprintf(fini, "TITLE=%s\n", temp1 = Fm2a(HelpTitle));
	F_Free(temp1);
	if (form != '3') {
		fprintf(fini, "CNT=.\\%s\n", temp1 = Fm2a(CntFileName));
		F_Free(temp1);
	}
	fprintf(fini, "COPYRIGHT=%s", temp1 = Fm2a(HelpCopyright));
	F_Free(temp1);
	if (form == '3')
		fprintf(fini, "\n");
	else {
		fprintf(fini, "%s\n", HelpCopyDate ? " %date" : "");
		//fprintf(fini, "IGNORE=2001\n");
		fprintf(fini, "HLP=.\\%s\n", temp1 = Fm2a(HelpFileName));
		F_Free(temp1);
	}

	fprintf(fini, "\n[FILES]\n");
	if (book) {
		if (!BookComps)
			GetChapters(F_ApiGetId(0, FV_SessionId, FP_ActiveBook));
		bcp = &BookComps;
		while (*bcp) {
			temp1 = Fm2a((*bcp)->bcname);
			fprintf(fini, ".\\%s.rtf\n", temp1);
			bcp = &((*bcp)->bcnext);
			F_Free(temp1);
		}
		FreeChapters();
	}
	else
		fprintf(fini, ".\\%s.rtf\n", basename);

	fprintf(fini, "\n[WINDOWS]\n");
	if (form == '3') {
		fprintf(fini, "main=%s\n", "\"\",(18,10,600,435),0,,(192,192,192),0");
		fprintf(fini, "proc=%s\n", "\"\",(653,102,360,600),0,,(192,192,192),0");
	}
	else {
		fprintf(fini, "main=%s\n", "\"\",(18,10,600,435),60672,(r14876671),(r12632256),f6");
		fprintf(fini, "proc=%s\n", "\"\",(653,102,360,600),20740,(r14876671),(r12632256),f3");
	}

	fprintf(fini, "\n[CONFIG]\n");
	fprintf(fini, "BrowseButtons()\n");

	fclose(fini);
	F_StrFree(hpjname);
	F_StrFree(basename);
}


VoidT SetCntVars(StringT basename, StringT ininame)
{
	if ((HelpContentsTopic == NULL) 
	 || (*HelpContentsTopic == '\0')) {
		if (ininame)
			GetIniString(ininame, "HelpContents", "CntTopic", &HelpContentsTopic);
		if ((HelpContentsTopic == NULL)
		 || (*HelpContentsTopic == '\0'))
			HelpContentsTopic = basename;
	}

	if ((CntFileName == NULL) 
	 || (*CntFileName == '\0')) {
		if (ininame)
			GetIniString(ininame, "HelpContents", "CntName", &CntFileName);
		if ((CntFileName == NULL)
		 || (*CntFileName == '\0'))
			CntFileName = AddFileExt(basename, ".cnt");
	}

	if ((HelpFileName == NULL) 
	 || (*HelpFileName == '\0')) {
		if (ininame)
			GetIniString(ininame, "HelpContents", "CntBase", &HelpFileName);
		if ((HelpFileName == NULL)
		 || (*HelpFileName == '\0'))
			HelpFileName = AddFileExt(basename, ".hlp");
	}
}


/* xml-based HelpSet files */

BoolT WriteHelpSetFile = False;
BoolT WriteHelpProjectFile = False;
BoolT HSPathNames = False;
StringT HTMLSubdir = "html";
StringT GraphSubdir = "graphics";
StringT DefaultTopicFile = NULL;
StringT CshMapFile = NULL;
StringT MapFilePrefix = NULL;
BoolT GenTOC = True;
BoolT GenIX = True;
BoolT UseFTS = True;
BoolT UseAlias = True;


VoidT WriteHSFile(StringT inipath, StringT filename,
									 BoolT book, UCharT form)
{
	FILE *fini = NULL;
	BookComp **bcp = &BookComps;
	StringT ininame = NULL;
	StringT basename = NULL;
	StringT fname = NULL;
	StringT tname = NULL;
	StringT temp1 = NULL;

	if (HPJName
	 && (*HPJName != '\0'))
		filename = HPJName;
	basename = Fm2a(temp1 = BaseFileName(filename));
	F_Free(temp1);

	ininame = MakeFileName(inipath, temp1 = AddFileExt(basename, ".hs"));
	F_Free(temp1);

	if (!WriteHelpSetFile
	 && ((fini = fopen(ininame, "rt")) != NULL)) {
		fclose(fini);
		F_StrFree(ininame);
		F_StrFree(basename);
		return;
	}

	if ((fini = fopen(ininame, "wt")) == NULL) {
		F_StrFree(ininame);
		F_StrFree(basename);
		return;
	}
	if (HSPathNames)
		fname = MakeFileName(inipath, basename);
	else
		fname = F_StrCopyString(basename);

	if ((tname = DefaultTopicFile) == NULL)
		tname = basename;

	/* the exact sequence of entries in the .hs is critical */
	/* it's an XML file... */
	fprintf(fini, "<?xml version='1.0' encoding='ISO-8859-1' ?>\n");
	if (form == 'J') {
		fprintf(fini, "<!DOCTYPE helpset\n");
		fprintf(fini, "  PUBLIC \"-//Sun Microsystems Inc.//DTD JavaHelp HelpSet Version 1.0//EN\"\n");
		fprintf(fini, "         \"http://java.sun.com/products/javahelp/helpset_1_0.dtd\">\n");
	}
	fprintf(fini, "<helpset version=\"1.0\">\n\n");

	fprintf(fini, "  <title>");
	WriteHTMLText(fini, HelpTitle ? HelpTitle : fname);
	fprintf(fini, "</title>\n\n");

	fprintf(fini, "  <maps>\n");
	if (form == 'O')
		fprintf(fini, "     <homeid>%s</homeid>\n", tname);
	else
		fprintf(fini, "     <homeID>%s</homeID>\n", tname);
	fprintf(fini, "     <mapref location=\"%s.jhm\"/>\n", fname);
	fprintf(fini, "  </maps>\n\n");

	fprintf(fini, "  <view>\n");
	fprintf(fini, "    <name>TOC</name>\n");
	fprintf(fini, "    <label>Table Of Contents</label>\n");

	if (form == 'O') {
		fprintf(fini, "    <type>oracle.help.navigator.tocNavigator.TOCNavigator</type>\n");
		fprintf(fini, "    <data engine=\"oracle.help.engine.XMLTOCEngine\">");
	}
	else {
		fprintf(fini, "    <type>javax.help.TOCView</type>\n");
		fprintf(fini, "    <data>");
	}

	fprintf(fini, "%sTOC.xml</data>\n", fname);
	fprintf(fini, "  </view>\n\n");

	fprintf(fini, "  <view>\n");
	fprintf(fini, "    <name>Index</name>\n");
	fprintf(fini, "    <label>Index</label>\n");

	if (form == 'O') {
		fprintf(fini, "    <type>oracle.help.navigator.keywordNavigator.KeywordNavigator</type>\n");
		fprintf(fini, "    <data engine=\"oracle.help.engine.XMLIndexEngine\">%sIndex.xml</data>\n", fname);
	}
	else {
		fprintf(fini, "    <type>javax.help.IndexView</type>\n");
		fprintf(fini, "    <data>");
		fprintf(fini, "%sIndex.xml</data>\n", fname);
	}

	fprintf(fini, "  </view>\n\n");

	if (UseFTS) {
		fprintf(fini, "  <view>\n");
		fprintf(fini, "    <name>Search</name>\n");
		//fprintf(fini, "    <label>Search</label>\n");

		if (form == 'O') {
			fprintf(fini, "    <title>");
			WriteHTMLText(fini, HelpTitle ? HelpTitle : fname);
			fprintf(fini, "</title>\n");
			fprintf(fini, "    <type>oracle.help.navigator.searchNavigator.SearchNavigator</type>\n");
			fprintf(fini, "    <data engine=\"oracle.help.engine.SearchEngine\">%s.idx</data>\n", fname);
		}
		else {
			fprintf(fini, "    <type>javax.help.SearchView</type>\n");
			fprintf(fini, "    <data engine=\"com.sun.java.help.search.DefaultSearchEngine\">\n");
			fprintf(fini, "      JavaHelpSearch\n");
			fprintf(fini, "    </data>\n");
		}

		fprintf(fini, "  </view>\n\n");
	}

	fprintf(fini, "</helpset>\n");

	fclose(fini);
	F_StrFree(ininame);
	F_StrFree(basename);
	F_StrFree(fname);
}


VoidT WriteHHPFile(StringT inipath, StringT filename,
									 BoolT book, UCharT form)
{
	FILE *fini = NULL;
	StringT ininame = NULL;
	StringT basename = NULL;
	StringT topic = NULL;
	StringT temp1 = NULL;

	if (HPJName
	 && (*HPJName != '\0'))
		filename = HPJName;
	basename = Fm2a(temp1 = BaseFileName(filename));
	F_Free(temp1);

	ininame = MakeFileName(inipath, temp1 = AddFileExt(basename, ".hhp"));
	F_Free(temp1);

	if (!WriteHelpProjectFile
	 && ((fini = fopen(ininame, "rt")) != NULL)) {
		fclose(fini);
		F_StrFree(ininame);
		F_StrFree(basename);
		return;
	}

	if ((fini = fopen(ininame, "wt")) == NULL) {
		F_StrFree(ininame);
		F_StrFree(basename);
		return;
	}

	if (DefaultTopicFile
	 && (*DefaultTopicFile != '\0')) {
		topic = NewFileExt(temp1 = Fm2a(DefaultTopicFile), HTMLSuffix);
		F_Free(temp1);
	}
	else
		topic = NewFileExt(basename, HTMLSuffix);

	fprintf(fini, "\n[OPTIONS]\n");
	fprintf(fini, "Binary Index=No\n");
	fprintf(fini, "Compiled file=%s\n", DefaultChmFile);
	fprintf(fini, "Contents file=%s.hhc\n", basename);
	fprintf(fini, "Default Font=Arial,8,0\n");
	fprintf(fini, "Default topic=%s\n", topic);
	fprintf(fini, "Full-text search=%s\n", UseFTS ? "Yes" : "No");
	fprintf(fini, "Index file=%s.hhk\n", basename);
	fprintf(fini, "Title=%s\n", HelpTitle);
	fprintf(fini, "Language=0x409 English (United States)\n");

	fprintf(fini, "\n[FILES]\n");
	fprintf(fini, "*.%s\n", (*HTMLSuffix == '.') ? HTMLSuffix + 1 : HTMLSuffix);
	// can't determine names now, not split yet
	fprintf(fini, "%s.hhp\n", basename);   // include .hhp itself
	if (CshMapFile)      // this isn't included automatically
		fprintf(fini, "%s\n", CshMapFile);
	if (UseAlias)       // nor is this one
		fprintf(fini, "%s.hha\n", basename);

	fprintf(fini, "\n[MAP]\n");
	if (CshMapFile)
		fprintf(fini, "#include \"%s\"\n", CshMapFile);

	fprintf(fini, "\n[ALIAS]\n");
	if (UseAlias)
		fprintf(fini, "#include \"%s.hha\"\n", basename);

	fprintf(fini, "\n[TEXT POPUPS]\n\n");
	fclose(fini);

	F_StrFree(ininame);
	F_StrFree(basename);
	F_StrFree(topic);
}


VoidT WriteHTMLText(FILE *fini, StringT txt)
{
	UCharT ch = 0;

	if (!txt)
		return;

	ch = (UCharT) *txt;

	while (ch) {
		switch (ch) {
			case '&':
				fprintf(fini, "&amp;");
				break;
			case '<':
				fprintf(fini, "&lt;");
				break;
			case '>':
				fprintf(fini, "&gt;");
				break;
			default:
				if (ch > 0x7F)
					fprintf(fini, "&#%0.3hd;", ((UShortT) ch) & 0xFF);
				else
					fprintf(fini, "%c", ch);
				break;
		}
		ch = (UCharT) *++txt;
	}
}



/* end of m2rwrt.c */


