// dwhtmxf.cpp is the code for dcl HTM external file writing, JHG
// Copyright (c) 1996-2005 by Omni Systems, Inc.  All rights reserved.

#include "dcappl.h"
#include "dcctlg.h"
#include "dclist.h"
#include "dcfili.h"
#include "dctxfx.h"
#include "dcfmtr.h"
#include "dcini.h"
#include "dcmacr.h"
#include "dcauto.h"
#include "dwhtm.h"
#include "dwhtmfl.h"
#include <locale.h>
#include "dcicu.h"
#include <windows.h>
#undef min

HANDLE PMFlag2 = NULL;;


// write external contents and index files for HH, JH, and OH

uns HTMwr::Contents = false;
bool HTMwr::MakeContents = false;
bool HTMwr::NoContLink = false;
bool HTMwr::NoContRef = false;
char *HTMwr::ContentsRef = NULL;
char *HTMwr::ContentsWin = NULL;
uns HTMwr::ContentsLevel = 0;
uns HTMwr::ContentsTabLev = 0;
bool HTMwr::ContentsLevEmpty = false;
DClist HTMwr::ContentsLevSkip;
char *HTMwr::TocIdxFilePrefix = NULL;
uns HTMwr::IndexLevel = 0;
char *HTMwr::IndexLevelClass = "IndexEntry";
char *HTMwr::IndexLetterClass = "IndexLetters";
char *HTMwr::IndexTopLettersClass = "IndexTopLetters";
char *HTMwr::IndexHeadingClass = "IndexTitle";
char *HTMwr::IndexTOCClass = "IndexTOC";
char *HTMwr::IndexRefClass = "IndexRef";
char *HTMwr::IndexLetPrefix = "ixlet";
char *HTMwr::IndexNumPrefix = "ixnum";
unc HTMwr::IndexNavType = 'H';
bool HTMwr::WriteIndexCssLink = true;
bool HTMwr::UseIndexLevelNum = false;
bool HTMwr::UseIndexLetters = true;
bool HTMwr::UseIndexTopLetters = true;
bool HTMwr::UseListProps = false;
bool HTMwr::UseIXLets = true;
bool HTMwr::UseIXTopLets = true;
char *HTMwr::IndexLetterSymbol = "Sym";
char *HTMwr::IndexLetterNumber = "Num";
bool HTMwr::UseIndexHeading = true;
char *HTMwr::IndexFileName = NULL;
char *HTMwr::IndexFileSuffix = NULL;
char *HTMwr::IDXSuffix = NULL;
char *HTMwr::IndexFileTitle = "Index";
char *HTMwr::IndexRefStartSep = " ";
char *HTMwr::IndexRefSep = ", ";
void **HTMwr::IndexRefs = NULL;
long HTMwr::IndexRefCount = 0;
long HTMwr::IndexRefAlloc = 0;
bool HTMwr::UseIndexentryTag = true;
void **HTMwr::ALinkRefs = NULL;
long HTMwr::ALinkAlloc = 0;
unc HTMwr::ALinkFileRefs = 'F';
void **HTMwr::GlossRefs = NULL;
long HTMwr::ContentsID = 0;
bool HTMwr::ContentsUsed = false;
DCvlist HTMwr::ContentsList;
DCilist HTMwr::ParaContentsList;
DCilist HTMwr::ContentsSplitList;
DCnlist HTMwr::ContentsWindowList;
long HTMwr::NewlinkCount = 0;
bool HTMwr::UseRawNewlinks = false;
DCilist HTMwr::ContentsPartList;
long HTMwr::ContentsPartCount = 0;

bool HTMwr::Eclipse = false;
bool HTMwr::IncludeVersionPI = true;
char *HTMwr::EclipseVer = "3";
char *HTMwr::EclipsePluginSchemaVersion = NULL;
char *HTMwr::EclipseTocLabel = NULL;
char *HTMwr::EclipseTocTopic = "introduction.htm";
char *HTMwr::EclipseTocFilename = "toc.xml";
char *HTMwr::EclipseTocLink = NULL;
bool HTMwr::EclipseTocPrimary = true;
bool HTMwr::EclipseTocFileOnly = true;
char *HTMwr::EclipseTocExtradir = NULL;
char *HTMwr::EclipseIdxFilename = "index.xml";
bool HTMwr::EclipseUseIndex = false;
bool HTMwr::EclipseUsePlugin = true;
bool HTMwr::EclipseWritePlugin = true;
bool HTMwr::EclipseUseManifest = true;
bool HTMwr::EclipseWriteManifest = true;
char *HTMwr::EclipseManifestName = "MANIFEST.MF";  // put in META-INF dir
bool HTMwr::EclipseUseSingleton = false;
bool HTMwr::EclipseUseRuntime = false;
char *HTMwr::EclipsePlugName = "Mif2Go Eclipse Help";
char *HTMwr::EclipsePlugID = "com.mif2go.help";
char *HTMwr::EclipsePlugVer = "1.0";
char *HTMwr::EclipsePlugProvider = "mif2go.com";
bool HTMwr::EclipseUseContext = true;
bool HTMwr::EclipseWriteContext = true;
char *HTMwr::EclipseContextName = "contexts.xml";
char *HTMwr::EclipseContextPlugName = NULL;
bool HTMwr::EclipseDescription = true;
bool HTMwr::DescriptionIsFirstLabel = false;
bool HTMwr::EclipseContextAnchors = false;
long HTMwr::EclipseContextCount = 0;
DCnlist HTMwr::EclipseContextNames;
DCvlist HTMwr::EclipseContextRefs;
DCvlist HTMwr::EclipseContextTopics;

DCvlist HTMwr::MapNewlinkList;
DCnlist HTMwr::MapNewlinkNameList;
DCvlist HTMwr::MapPopupList;
long HTMwr::PopupCount = 0;
DCvlist HTMwr::MapTopicList;
long HTMwr::TopicCount = 0;


DCwfile *HTMwr::ContentsFile;
bool HTMwr::ContentsFileStarting = true;
DCwfile *HTMwr::IndexFile;
bool HTMwr::IndexFileStarting = true;
DCwfile *HTMwr::MapFile;
DCwfile *HTMwr::SearchFile;
DCwfile *HTMwr::AliasFile;
DCwfile *HTMwr::AliasTitleFile = NULL;
bool HTMwr::AliasFileStarting = true;
DCwfile *HTMwr::ALinkFile;
DCwfile *HTMwr::GlossFile;
long HTMwr::GlossCount = 0;

char *HTMwr::CshMapFile = NULL;
bool HTMwr::MakeCshMapFile = false;
unl HTMwr::CshMapFileNumStart = 10000;
unl HTMwr::CshMapFileNumIncrement = 10;

char *HTMwr::HelpFileTitle = NULL;
char *HTMwr::HelpFileLanguage = "0x409 English (United States)";
char *HTMwr::HomeID = "top";
char *HTMwr::HomeURL = NULL;
bool HTMwr::WriteHelpProjFile = true;
bool HTMwr::StoreCshFiles = true;
bool HTMwr::WriteHelpProjFileSet = false;
bool HTMwr::WriteHelpSetFile = true;
char *HTMwr::DefaultHelpProjectFile = NULL;
char *HTMwr::DefaultHelpSetFile = NULL;
char *HTMwr::DefaultTopicFile = NULL;
char *HTMwr::HHContValPx = NULL;
bool HTMwr::HHContFileOnly = true;
bool HTMwr::HSPathNames = false;
bool HTMwr::UseFTS = true;
bool HTMwr::UseSubHelpSets = true;
bool HTMwr::UseGlossary = false;
bool HTMwr::UseFavorites = false;
bool HTMwr::JHVersion2 = true;
bool HTMwr::JHImages = false;
DCnlist HTMwr::JHImagesName;
DCnlist HTMwr::JHImagesLoc;
DCilist HTMwr::ExpandLevel;
DCnlist HTMwr::ImageLevel;
char *HTMwr::GlossPrefix = "GLO_";
char *HTMwr::GlossSuffix = NULL;
char *HTMwr::GlossSpace = NULL;
char *HTMwr::JHDtdVer = "1_0";
char *HTMwr::JHFileVer = "1.0";
char *HTMwr::HHCProperties = NULL;
char *HTMwr::HHKProperties = NULL;

char *HTMwr::OHProjectName = "omnihelp";
char *HTMwr::OHProjFileSuffix = "";
char *HTMwr::OHProjFilePrefix = "_";
bool HTMwr::OHProjFileXhtml = false;
char *HTMwr::ProjectTemplate = "ohtpl.ini";
DCirfile *HTMwr::OHTemplate = NULL;
char *HTMwr::OHTitle = "OmniHelp Project";
char *HTMwr::MainName = NULL;
char *HTMwr::NavElems = NULL;
char *HTMwr::FrameLeft = "-1";
char *HTMwr::FrameTop = "-1";
char *HTMwr::FrameHigh = "350";
char *HTMwr::FrameWide = "600";
char *HTMwr::FrameOptions = "";
char *HTMwr::TopHigh = "50";
char *HTMwr::LeftWide = "220";
char *HTMwr::MidHigh = "90";

bool HTMwr::TopFirst = true;
bool HTMwr::UseTopButtons = true;
bool HTMwr::UseStart = true;
bool HTMwr::UsePrevNext = true;
bool HTMwr::UseBackForward = true;
bool HTMwr::UseHideShow = true;
bool HTMwr::ShowNavLeft = true;
bool HTMwr::NewWindow = true;
bool HTMwr::CloseOldWindow = false;
bool HTMwr::PersistSettings = true;
bool HTMwr::OHLowMem = true;
bool HTMwr::OHTocGroupsOpen = false;
char *HTMwr::OHTocOpenLevel = "0";
char *HTMwr::OHTocIcoBase = "ohtc";
bool HTMwr::OHTocExpand = true;
bool HTMwr::OHTocButtons = true;
bool HTMwr::OHIdxGroupsOpen = false;
char *HTMwr::OHIdxOpenLevel = "0";
char *HTMwr::OHIdxIcoBase = "ohtc";
bool HTMwr::OHIdxExpand = true;
bool HTMwr::OHIdxButtons = true;
bool HTMwr::OHRelShowSubjects = false;

char *HTMwr::CtrlCssName = "ohctrl.css";
char *HTMwr::MainCssName = "ohmain.css";
char *HTMwr::IECtrlCssName = "ohctrl.css";
char *HTMwr::N6CtrlCssName = "ohctrl.css";
char *HTMwr::N4CtrlCssName = "ohctn4.css";
char *HTMwr::IECssName = NULL;
char *HTMwr::N6CssName = NULL;
char *HTMwr::N4CssName = NULL;
DClist HTMwr::MergeList;
DCnlist HTMwr::MergeLocList;
bool HTMwr::MergeFirst = false;



void
HTMwr::WriteReferenceFiles(DCwfile *wf)
{
	char *ext = NULL;
	char *name = NULL;
	char *filename = CurrFileName;


	if (RefFileType != 'B') {
		if (OmniHelp
		 && OHProjectName
		 && *OHProjectName)
			filename = OHProjectName;
		else if ((HelpType == 'M')
		 && DefaultHelpProjectFile
		 && *DefaultHelpProjectFile)
			filename = DefaultHelpProjectFile;
		else if ((HelpType == 'J')
		 && DefaultHelpSetFile
		 && *DefaultHelpSetFile)
			filename = DefaultHelpSetFile;
		else if (Eclipse)
			filename = NULL;
		if (filename)  // remove ext if any
			filename = ReplaceExt(filename);
	}

		// write to hhc or bhc file along with full hyperref

	if ((ListType == 'B')
	 || (ListType == 'C')) {
		if (Eclipse
		 && (RefFileType != 'B'))
			name = EclipseTocFilename;
		else {
			ext = (RefFileType == 'B') ? ".bhc" :  // same name for either format
						((HelpType == 'M') ? ".hhc" : 
						((HelpType == 'C') ? "_ohc.js" : 
						((HelpType == 'J') ? "TOC.xml" : ".toc")));
			name = NewName(filename, strlen(filename) + strlen(ext) + 1);
			strcat(name, ext);
		}
		ContentsFile = (RefFileType == 'B') ? new DCLwfile(name, fasc) :
																					new DCwfile(name, ftxt);
		if (ContentsFile->FErr() != fok)
			ContentsFile = NULL;
	}
	else
		ContentsFile = NULL;

		// write to hhk or bhk file along with full hyperref

	if ((ListType == 'B')
	 || (ListType == 'I')) {
		if (Eclipse
		 && (RefFileType != 'B'))
			name = EclipseIdxFilename;
		else {
			ext = (RefFileType == 'B') ? ".bhk" :  // same name for either format
						((HelpType == 'M') ? ".hhk" : 
						((HelpType == 'C') ? "_ohk.js" : 
						((HelpType == 'J') ? "Index.xml" : ".idx")));
			name = NewName(filename, strlen(filename) + strlen(ext) + 1);
			strcat(name, ext);
		}
		IndexFile = (RefFileType == 'B') ? new DCLwfile(name, fasc) :
																			 new DCwfile(name, ftxt);
		if (IndexFile->FErr() != fok)
			IndexFile = NULL;
	}
	else
		IndexFile = NULL;

	if (OmniHelp
	 || (OracleHelp && MakeALinkFile)) {
		ext = (RefFileType == 'B') ? ".bhl" :
					((HelpType == 'C') ? "_ohl.js" : "Links.xml");
		name = NewName(filename, strlen(filename) + strlen(ext) + 1);
		strcat(name, ext);

		ALinkFile = (RefFileType == 'B') ? new DCLwfile(name, fasc) :
																			 new DCwfile(name, ftxt);
		if (ALinkFile->FErr() != fok)
			ALinkFile = NULL;
	}

	if (OmniHelp
	 || (MakeAliasFile && AliasCount)) {
		ext = (RefFileType == 'B') ? ".bha" :
					((HelpType == 'C') ? "_oha.js" : ".hha");
		name = NewName(filename, strlen(filename) + strlen(ext) + 1);
		strcat(name, ext);

		AliasFile = (RefFileType == 'B') ? new DCLwfile(name, fasc) :
																			 new DCwfile(name, ftxt);
		if (AliasFile->FErr() != fok)
			AliasFile = NULL;
		else if (AliasTitle) {
			char *dest = ReplaceExt(NewName(name), ".hht");
			AliasTitleFile = new DCwfile(dest, ftxt);
		}
	}
	else
		AliasFile = NULL;

	if (UseGlossary && GlossCount) {
		if (RefFileType == 'B') {
			ext = ".bhg";
			name = NewName(filename, strlen(filename) + strlen(ext) + 1);
			strcat(name, ext);
		}
		else
			name = "glossary.xml";

		GlossFile = (RefFileType == 'B') ? new DCLwfile(name, fasc) :
																			 new DCwfile(name, ftxt);
		if (GlossFile->FErr() != fok)
			GlossFile = NULL;
	}
	else
		GlossFile = NULL;

	if (!ContentsFile && !IndexFile && !AliasFile && !ALinkFile && !GlossFile)
		return;

	if (HelpType == 'J') {
		ext = (RefFileType == 'B') ? ".bhm" : ".jhm";
		name = NewName(filename, strlen(filename) + strlen(ext) + 1);
		strcat(name, ext);
		MapFile = (RefFileType == 'B') ? new DCLwfile(name, fasc) :
																		 new DCwfile(name, ftxt);
		if (MapFile->FErr() != fok)
			return;
		if (JHVersion2
		 && IniFile)
			SetJHLists();
	}
	else
		MapFile = NULL;

	if (OmniHelp && UseFTS) {
		ext = (RefFileType == 'B') ? ".bhs" : "_ohs.js";
		name = NewName(filename, strlen(filename) + strlen(ext) + 1);
		strcat(name, ext);
		SearchFile = (RefFileType == 'B') ? new DCLwfile(name, fasc) :
																				new DCwfile(name, ftxt);
		if (SearchFile->FErr() != fok)
			return;
	}
	else
		SearchFile = NULL;


	SplitID = -1;

	if (RefFileType == 'F') {
		if (HelpType == 'J')
			WriteMapFileStart();

		if (ContentsFile) {
			WriteContentsStart();
			ContentsList.writeall(ContentsFile, WriteContentsItem);
			WriteContentsEnd();
		}

		if (SearchFile) {
			WriteSearchStart();
			SearchWordList.writeall(SearchFile, WriteSearchItem);
			WriteSearchEnd();
		}

		if (HelpType == 'J')
			MapFile->putCr();

		if (IndexFile) {
			WriteIndexStart();
			SortIndexRefs();
			WriteIndexRefs();
			WriteIndexEnd();
		}

		if (HelpType == 'J')
			MapFile->putCr();

		if (ALinkFile) {
			WriteALinkStart();
			if (HelpType == 'C')
				OHALinkList.writeall(ALinkFile, WriteALinkItem);
			else {
				SortALinkRefs();
				WriteALinkRefs();
			}
			WriteALinkEnd();
		}

		if (GlossFile) {
			WriteGlossStart();
			SortGlossRefs();
			WriteGlossRefs();
			WriteGlossEnd();
		}

		if (Eclipse) {
			if (EclipseUsePlugin)
				WriteEclipsePlugin();
			if (EclipseUseContext)
				WriteEclipseContext();
			if (EclipseUseManifest)
				WriteEclipseManifest();
		}
		else if (HelpType == 'M') {
			WriteHelpProjectFile(filename);
			if (MakeCshMapFile)
				WriteCSHMapFile(filename);
		}
		else if (OmniHelp)
			WriteOHProjectFile(filename);

		if (HelpType == 'J') {
			WriteHelpSet(filename);
			if (NewlinkCount) {
				MapFile->putCr();
				MapNewlinkList.all(WriteMapRef);
			}
			if (PopupCount) {
				MapFile->putCr();
				MapPopupList.all(WriteMapRef);
			}
			if (TopicCount) {
				MapFile->putCr();
				MapTopicList.all(WriteMapRef);
			}
			WriteMapFileEnd();
			MapFile->CloseFile();
		}

		if (AliasFile && (OmniHelp || AliasCount))
			WriteAliasFullFile();

	}
	else if (RefFileType == 'B') {
		if (ContentsFile)
			ContentsList.writeall(ContentsFile, WriteContentsDCL);
		if (SearchFile)
			SearchWordList.writeall(SearchFile, WriteSearchDCL);
		if (IndexFile) {
			SortIndexRefs();
			WriteDCLIndexRefs();
		}
		if (ALinkFile) {
			if (HelpType == 'C')  // same DCL as for search
				OHALinkList.writeall(ALinkFile, WriteSearchDCL);
			else {
				SortALinkRefs();
				WriteDCLALinkRefs();
			}
		}
		if (GlossFile) {
			SortGlossRefs();
			WriteDCLGlossRefs();
		}

		if (HelpType == 'J') {
			MapNewlinkList.writeall(MapFile, WriteMapDCL);
			MapPopupList.writeall(MapFile, WriteMapDCL);
			MapTopicList.writeall(MapFile, WriteMapDCL);
			MapFile->CloseFile();
		}

		if (AliasFile && (OmniHelp || AliasCount))
			AliasList.writeall(AliasFile, WriteMapDCL); // same DCL format

		if (Eclipse && EclipseUseContext && EclipseContextCount) {
			char *name = NewName(filename, strlen(filename) + 5);
			strcat(name, ".bha");
			DCwfile *wf = new DCLwfile(name, fasc);
			if (wf->FErr() == fok)
				EclipseContextNames.writeall(wf, WriteContextDCL);
		}
	}

	if (ContentsFile)
		ContentsFile->CloseFile();
	if (SearchFile)
		SearchFile->CloseFile();
	if (IndexFile)
		IndexFile->CloseFile();
	if (ALinkFile)
		ALinkFile->CloseFile();
	if (GlossFile)
		GlossFile->CloseFile();
	if (AliasFile)
		AliasFile->CloseFile();
	if (AliasTitleFile)
		AliasTitleFile->CloseFile();
}


void
HTMwr::SetJHLists(void)
{
	if ((JHImages = IniFile->Section("JHImages")) != false) {
		long ix = 0;
		while (IniFile->NextItem()) {
			JHImagesName.add(IniFile->StrLeft(), ++ix);
			JHImagesLoc.add(IniFile->StrRight(), ix);
		}
		if (IniFile->Section("TocLevelExpand")) {
			while (IniFile->NextItem())
				ExpandLevel.append((IniFile->BoolRight() ? 2 : 1), IniFile->IntLeft());
		}
		if (IniFile->Section("TocLevelImage")) {
			while (IniFile->NextItem())
				ImageLevel.add(IniFile->StrRight(), IniFile->IntLeft());
		}
	}
}


void
HTMwr::WriteHelpProjectFile(char *fname)
{
	if (!WriteHelpProjFile)
		return;

	if (DefaultHelpProjectFile
	 && (*DefaultHelpProjectFile != '\0'))
		fname = DefaultHelpProjectFile;

	char *name = ReplaceExt(fname, ".hhp");

	if (!WriteHelpProjFileSet) {
		// do not overwrite if file exists
		DCrfile *rf = new DCrfile(name);
		if (rf->FErr() == fok) {
			rf->CloseFile();
			DeleteName(name);
			return;
		}
	}

	DCwfile *wf = new DCwfile(name, ftxt);
	if (wf->FErr() != fok) {
		DeleteName(name);
		return;
	}

	char *tname = NULL;
	if (DefaultTopicFile
	 && (*DefaultTopicFile != '\0'))
		tname = NewName(DefaultTopicFile);
	else
		tname = NewName(fname);

	char *per = NULL;
	if ((per = strrchr(tname, '.')) != NULL)
		*per = '\0';  // truncate extension if any

	char *bname = NewName(fname);
	if (((per = strrchr(bname, '.')) != NULL)
	 && !stricmp(per, ".hhp"))
		*per = '\0';
  

	//char *topic = NewName(strlen(tname) + 2 + strlen(XrefFileSuffix));
	//sprintf(topic, "%s.%s", tname, XrefFileSuffix);

	wf->putCr();
	wf->putStr("[OPTIONS]");
	wf->putCr();
	wf->putStr("Binary Index=");
	wf->putStr(BinaryIndex ? "Yes" : "No");
	wf->putCr();
	wf->putStr("Binary TOC=");
	wf->putStr(BinaryTOC ? "Yes" : "No");
	wf->putCr();
	wf->putStr("Compatibility=1.1 or later");
	wf->putCr();
	wf->putStr("Compiled file=");
	wf->putStr(DefaultChmFile ? DefaultChmFile : bname);
	wf->putStr(".chm");
	wf->putCr();
	if (ContentsFile) {
		wf->putStr("Contents file=");
		wf->putStr(bname);
		wf->putStr(".hhc");
		wf->putCr();
	}
	wf->putStr("Default Font=Arial,8,0");
	wf->putCr();
	wf->putStr("Default Window=main");
	wf->putCr();
	wf->putStr("Default topic=");
	wf->putStr(tname);
	wf->putChar('.');
	wf->putStr(XrefFileSuffix);
	wf->putCr();
	wf->putStr("Display compile progress=No");
	wf->putCr();
	wf->putStr("Full-text search=");
	wf->putStr(UseFTS ? "Yes" : "No");
	wf->putCr();
	if (IndexFile) {
		wf->putStr("Index file=");
		wf->putStr(bname);
		wf->putStr(".hhk");
		wf->putCr();
	}
	if (HelpFileLanguage) {
		// 0x409 English, 0x408 Greek, 0x419 Russian,
		// 0x41F Turkish, 0x405 Czech (use for CE),
		// 0x411 Japanese, 0x404 Chinese (Traditional),
		// 0x804 Chinese (Simplified), 0x412 Korean
		wf->putStr("Language=");
		wf->putStr(HelpFileLanguage);
		wf->putCr();
	}
	if (HelpFileTitle) {
		wf->putStr("Title=");
		wf->putStr(HelpFileTitle);
		wf->putCr();
	}

	wf->putCr();
	wf->putStr("[WINDOWS]");
	wf->putCr();
	if (IniFile
	 && IniFile->Section("HHWindows"))
		wf->putStr(IniFile->SecCont());
	else {  // write a line for main at least
	//main=,"ugdita2go.hhc","ugdita2go.hhk","ugd2g.htm",,,,,,0x3520,,0x60383e,,,,,,,,0
		wf->putStr("main=");
		if (HelpFileTitle) {
			wf->putChar('"');
			wf->putStr(HelpFileTitle);
			wf->putChar('"');
		}
		wf->putChar(',');
		if (ContentsFile) {
			wf->putChar('"');
			wf->putStr(bname);
			wf->putStr(".hhc\"");
		}
		wf->putChar(',');
		if (IndexFile) {
			wf->putChar('"');
			wf->putStr(bname);
			wf->putStr(".hhk\"");
		}
		wf->putChar(',');
		wf->putChar('"');
		wf->putStr(tname);
		wf->putChar('.');
		wf->putStr(XrefFileSuffix);
		wf->putChar('"');
		wf->putStr(",,,,,,0x");
		if (!IndexFile)
			wf->putChar('2');
		wf->putStr("3520,,0x60383e,,,,,,,,0");
	}
	wf->putCr();

	wf->putCr();
	wf->putStr("[FILES]");
	wf->putCr();
	//wf->putStr("*.htm"); // can't determine names now, not split yet
	wf->putChar('*');
	wf->putChar('.');
	wf->putStr(XrefFileSuffix);
	wf->putCr();
	wf->putStr(bname);     // include .hhp itself for any decompilation
	wf->putStr(".hhp");
	wf->putCr();
	if (StoreCshFiles) {
		if (CshMapFile) {      // this isn't included automatically
			wf->putStr(CshMapFile);
			wf->putCr();
		}
		if (AliasFile && AliasCount) {  // nor is this one
			wf->putStr(AliasFile->getFileName());
			wf->putCr();
		}
	}

	wf->putCr();
	wf->putStr("[MAP]");
	if (CshMapFile) {
		wf->putCr();
		wf->putStr("#include \"");
		wf->putStr(CshMapFile);
		wf->putChar('"');
	}
	wf->putCr();

	wf->putCr();
	wf->putStr("[ALIAS]");
	if (AliasFile && AliasCount) {
		wf->putCr();
		wf->putStr("#include \"");
		wf->putStr(AliasFile->getFileName());
		wf->putChar('"');
	}
	wf->putCr();

	wf->putCr();
	wf->putStr("[MERGE FILES]");
	wf->putCr();
	if (IniFile
	 && IniFile->Section("HelpMerge")) {
		while (IniFile->NextItem()) {
			wf->putStr(IniFile->StrLeft());
			wf->putStr(".chm");
			wf->putCr();
		}
	}

	wf->putCr();
	wf->putStr("[TEXT POPUPS]");
	wf->putCr();
	wf->putCr();

	wf->CloseFile();
	DeleteName(name);
	DeleteName(tname);
	DeleteName(bname);
}


void
HTMwr::WriteEclipsePlugin(void)
{
	/* Eclipse example of plugin.xml
		<?xml version="1.0"?>
		<plugin name="Agitator Help" id="com.agitar.help" version="2.0" provider-name="Agitar.com">
		<runtime/>
		<extension point="org.eclipse.help.toc">
			<toc file="toc.xml" primary="true"/>
		</extension>
		<extension point="org.eclipse.help.contexts">
			<contexts name="contexts.xml"/>
		</extension>
		</plugin>
	*/

	char *name = "plugin.xml";

	if (!EclipseWritePlugin) {
		// do not overwrite if file exists
		DCrfile *rf = new DCrfile(name);
		if (rf->FErr() == fok) {
			rf->CloseFile();
			//DeleteName(name);
			return;
		}
	}

	DCwfile *wf = new DCwfile(name, ftxt);
	if (wf->FErr() != fok)
		return;

	wf->putStr("<?xml version=\"1.0\"?>");
	wf->putCr();
	if (IncludeVersionPI) {
		wf->putStr("<?eclipse version=\"");
		if (!EclipsePluginSchemaVersion)
			EclipsePluginSchemaVersion = EclipseUseManifest ? "3.2" : "3.1";
		wf->putStr(EclipsePluginSchemaVersion);
		wf->putStr("\"?>");
		wf->putCr();
	}
	wf->putStr("<plugin");
	if (!EclipseUseManifest) {
		wf->putStr(" name=\"");
		wf->putStr(EclipsePlugName);
		wf->putStr("\" id=\"");
		wf->putStr(EclipsePlugID);
		wf->putStr("\" version=\"");
		wf->putStr(EclipsePlugVer);
		wf->putChar('"');
		wf->putCr();
		wf->putStr(" provider-name=\"");
		wf->putStr(EclipsePlugProvider);
		wf->putChar('"');
		if (EclipseUseSingleton)
			wf->putStr(" singleton=\"true\"");
	}
	wf->putChar('>');
	wf->putCr();
	if (EclipseUseRuntime) {
		wf->putStr("  <runtime/>");
		wf->putCr();
	}
	wf->putStr("  <extension point=\"org.eclipse.help.toc\">");
	wf->putCr();
	wf->putStr("    <toc file=\"");
	wf->putStr(EclipseTocFilename);
	wf->putStr("\" primary=\"");
	wf->putStr(EclipseTocPrimary ? "true" : "false");
	if (EclipseTocExtradir) {
		wf->putStr("\" extradir=\"");
		wf->putStr(EclipseTocExtradir);
	}
	wf->putStr("\"/>");
	wf->putCr();
	wf->putStr("  </extension>");
	wf->putCr();
	if (EclipseUseIndex) {
		wf->putStr("  <extension point=\"org.eclipse.help.index\">");
		wf->putCr();
		wf->putStr("    <index file=\"");
		wf->putStr(EclipseIdxFilename);
		wf->putStr("\"/>");
		wf->putCr();
		wf->putStr("  </extension>");
		wf->putCr();
	}
	if (UseFTS) {
		wf->putStr("  <extension point=\"org.eclipse.help.base.luceneSearchParticipants\">");
		wf->putCr();
		wf->putStr("    <binding participantId=\"org.eclipse.help.base.xhtml\"/>");
		wf->putCr();
		wf->putStr("  </extension>");
		wf->putCr();
	}
	if (EclipseUseContext && EclipseContextCount) {
		wf->putStr("  <extension point=\"org.eclipse.help.contexts\">");
		wf->putCr();
		wf->putStr("    <contexts ");
		wf->putStr((*EclipseVer > '2') ? "file=\"" : "name=\"");
		wf->putStr(EclipseContextName);
		if (EclipseContextPlugName) {
			wf->putStr("\" plug-in=\"");
			wf->putStr(EclipseContextPlugName);
		}
		wf->putStr("\"/>");
		wf->putCr();
		wf->putStr("  </extension>");
		wf->putCr();
	}
	wf->putStr("</plugin>");
	wf->CloseFile();
}

void
HTMwr::WriteEclipseManifest(void)
{
	char *name = EclipseManifestName;
	if (!EclipseWriteManifest) {
		// do not overwrite if file exists
		DCrfile *rf = new DCrfile(name);
		if (rf->FErr() == fok) {
			rf->CloseFile();
			return;
		}
	}
	DCwfile *wf = new DCwfile(name, ftxt);

	if (wf->FErr() != fok)
		return;

	// Manifest-Version: 1.0
	// Bundle-ManifestVersion: 2
	// Bundle-Name: Your DITA Plug-in Name
	// Bundle-SymbolicName: org.dita.sample.doc; singleton:=true
	// Bundle-Version: 1.0.0
	wf->putStr("Manifest-Version: 1.0");
	wf->putCr();
	wf->putStr("Bundle-ManifestVersion: 2");
	wf->putCr();
	wf->putStr("Bundle-Name: ");
	wf->putStr(EclipsePlugName);
	wf->putCr();
	wf->putStr("Bundle-SymbolicName: ");
	wf->putStr(EclipsePlugID);
	if (EclipseUseSingleton)
		wf->putStr("; singleton:=true");
	wf->putCr();
	wf->putStr("Bundle-Version: ");
	wf->putStr(EclipsePlugVer);
	wf->putCr();
	wf->putStr("Bundle-Vendor: ");
	wf->putStr(EclipsePlugProvider);
	wf->putCr();
	wf->CloseFile();
}

void
HTMwr::WriteEclipseContext(void)
{
	if (!EclipseWriteContext
	 || !EclipseContextCount)
		return;

	char *name = EclipseContextName;
	DCwfile *wf = new DCwfile(name, ftxt);

	if (wf->FErr() != fok)
		return;

	/* context file format
	<contexts>
		<context id="panic_button">
			<description>Brief description of this control.</description>
			<topic href="file_name_link1.html" label="Link1 Topic Title"/>
			<topic href="file_name_link2.html" label="Link2 Topic Title"/>
		</context>
		. . .
	</contexts>
	*/

	wf->putStr("<contexts>");
	wf->putCr();
	EclipseContextNames.writeall(wf, WriteContext);
	wf->putStr("</contexts>");
	wf->putCr();
	wf->CloseFile();
}

void
HTMwr::WriteContext(DCwfile *mwf, char *chp, long i)
{
	if ((chp == NULL) || (*chp == '\0') || (i == 0))
		return;
	DCwfile *wf = (DCwfile *) mwf;

	// write each context section per DCnlist EclipseContextNames
	// which has cid strings, and DCvlist EclipseContextRefs has
	// a crefs items with numbers matching the ECN cid list numbers,
	// each item a struct with ctitle and cfile string members.
	// These are populated by placing markers in the ctref topics
	// containing the cids.  During final write, [EclipseHelpContexts] 
	// section is checked to get the description string for each cid.

	char *desc = "Missing description!";
	if (DescriptionIsFirstLabel) {
		ecrefs *mp = (ecrefs *) EclipseContextRefs.find(i);
		if (mp)
			desc = mp->ctitle;
	}
	if (IniFile
	 && IniFile->Section("EclipseHelpContexts")
	 && IniFile->Find(chp))
		desc = IniFile->StrRight();

	wf->putStr("  <context id=\"");
	wf->putStr(chp);
	wf->putStr("\">");
	wf->putCr();
	if (EclipseDescription) {
		wf->putStr("    <description>");
		wf->putStr(desc);
		wf->putStr("</description>");
		wf->putCr();
	}
	EclipseContextRefs.write(i, mwf, WriteContextTopic);

	wf->putStr("  </context>");
	wf->putCr();
}

void
HTMwr::WriteContextTopic(DCfile *mwf, void *vp)
{
	if (vp == NULL)
		return;

	ecrefs *mp = (ecrefs *) vp;
	DCwfile *wf = (DCwfile *) mwf;

	wf->putStr("    <topic href=\"");
	if (TocIdxFilePrefix && *TocIdxFilePrefix)
		wf->putStr(TocIdxFilePrefix);
	wf->putStr(mp->cfile);
	if (mp->cref)
		wf->putStr(mp->cref);
	wf->putStr("\" label=\"");
	wf->putStr(mp->ctitle);
	wf->putStr("\"/>");
	wf->putCr();
}



void
HTMwr::WriteHelpSet(char *fname)
{
	if (DefaultHelpSetFile
	 && (*DefaultHelpSetFile != '\0'))
		fname = DefaultHelpSetFile;

	char *name = ReplaceExt(fname, ".hs");
	if (HSPathNames
	 || (((fname = strrchr(name, '/')) == NULL)
	  && ((fname = strrchr(name, '\\')) == NULL)))
		fname = name;

	char *tname = NULL;
	if (DefaultTopicFile
	 && (*DefaultTopicFile != '\0'))
		tname = NewName(DefaultTopicFile);
	else
		tname = NewName(HomeID ? HomeID : fname);

	StripSuf(tname);  // truncate extensions if any

	if (HomeURL) {
		char *pound = strrchr(HomeURL, '#');
		if (pound)
			*pound = '\0';
	}

	WriteMapFileItem(tname, HomeURL ? HomeURL : fname);

	if (!WriteHelpSetFile) {
		DeleteName(name);
		return;
	}

	DCwfile *wf = new DCwfile(name, ftxt);
	if (wf->FErr() != fok) {
		DeleteName(name);
		return;
	}
	StripSuf(fname);

	wf->putStr("<?xml version='1.0' encoding='ISO-8859-1' ?>");
	wf->putCr();

	if (!OracleHelp) {
		wf->putStr("<!DOCTYPE helpset");
		wf->putCr();
		wf->putStr("  PUBLIC \"-//Sun Microsystems Inc.//DTD JavaHelp HelpSet Version ");
		wf->putStr(JHFileVer);
		wf->putStr("//EN\"");
		wf->putCr();
		wf->putStr("         \"http://java.sun.com/products/javahelp/helpset_");
		wf->putStr(JHDtdVer);
		wf->putStr(".dtd\">");
		wf->putCr();
	}

	wf->putStr("<helpset version=\"");
	wf->putStr(JHFileVer);
	wf->putStr("\">");
	wf->putCr();

	wf->putCr();
	wf->putStr("  <title>");
	WriteIndexTitle(wf, HelpFileTitle ? HelpFileTitle : fname);
	wf->putStr("</title>");
	wf->putCr();

	wf->putCr();
	wf->putStr("  <maps>");
	wf->putCr();

	if (OracleHelp) {
		wf->putStr("     <homeid>");
		wf->putStr(tname);
		wf->putStr("</homeid>");
		wf->putCr();
	}
	else {
		wf->putStr("     <homeID>");
		wf->putStr(tname);
		wf->putStr("</homeID>");
		wf->putCr();
	}

	wf->putStr("     <mapref location=\"");
	wf->putStr(fname);
	wf->putStr(".jhm\"/>");
	wf->putCr();
	wf->putStr("  </maps>");
	wf->putCr();

	if (OracleHelp
	 && IniFile
	 && IniFile->Section("OracleHelpWindows")) {
		bool first = true;
		bool last = false;
		char *str = NULL;
		char *sp = NULL;
		char *ep = NULL;
		while (IniFile->NextItem()) {
			wf->putCr();
			wf->putStr("  <wintype");
			if (first) {
				wf->putStr(" default=\"true\"");
				first = false;
			}
			wf->putStr(">");
			wf->putCr();
			str = IniFile->StrLeft();
			wf->putStr("    <name>");
			wf->putStr(str);
			wf->putStr("</name>");
			wf->putCr();
			DeleteName(str);
			sp = ep = str = IniFile->StrRight();
			last = false;
			for (int i = 0; i < 9; i++) {
				while (*ep && (*ep != ','))
					ep++;
				if (*ep == '\0')
					last = true;
				else if (i < 8) // allow commas in title
					*ep = '\0';
				switch (i) {
					case 0: // height
						if (*sp) {
							wf->putStr("    <height>");
							wf->putStr(sp);
							wf->putStr("</height>");
							wf->putCr();
						}
						break;
					case 1: // width
						if (*sp) {
							wf->putStr("    <width>");
							wf->putStr(sp);
							wf->putStr("</width>");
							wf->putCr();
						}
						break;
					case 2: // xpos
						if (*sp) {
							wf->putStr("    <x>");
							wf->putStr(sp);
							wf->putStr("</x>");
							wf->putCr();
						}
						break;
					case 3: // ypos
						if (*sp) {
							wf->putStr("    <y>");
							wf->putStr(sp);
							wf->putStr("</y>");
							wf->putCr();
						}
						break;
					case 4: // textcolor
						if (*sp) {
							wf->putStr("    <textfg>");
							wf->putStr(sp);
							wf->putStr("</textfg>");
							wf->putCr();
						}
						break;
					case 5: // linkcolor
						if (*sp) {
							wf->putStr("    <linkfg>");
							wf->putStr(sp);
							wf->putStr("</linkfg>");
							wf->putCr();
						}
						break;
					case 6: // background
						if (*sp) {
							wf->putStr("    <bg>");
							wf->putStr(sp);
							wf->putStr("</bg>");
							wf->putCr();
						}
						break;
					case 7: // buttons
						if (*sp) {
							wf->putStr("    <toolbar>");
							wf->putStr(sp);
							wf->putStr("</toolbar>");
							wf->putCr();
						}
						break;
					case 8: // title
						if (*sp) {
							wf->putStr("    <title>");
							wf->putStr(sp);
							wf->putStr("</title>");
							wf->putCr();
						}
						break;
					default:
						break;
				}
				sp = ++ep;
				if (last)
					break;
			}
			DeleteName(str);
			wf->putStr("  </wintype>");
			wf->putCr();
		}
	}

	if (OracleHelp && MakeALinkFile) {
		wf->putCr();
		wf->putStr("  <links>");
		wf->putCr();
		wf->putStr("     <linkref location=\"");
		wf->putStr(fname);
		wf->putStr("Links.xml\"/>");
		wf->putCr();
		wf->putStr("  </links>");
		wf->putCr();
	}

	wf->putCr();
	if (JHVersion2) {
		wf->putStr("  <view ");
		wf->putStr(XHLangAttr);
		wf->putStr("=\"");
		wf->putStr(XhtmlLang);
		wf->putStr("\" mergetype=\"javax.help.UniteAppendMerge\">");
	}
	else
		wf->putStr("  <view>");
	wf->putCr();
	wf->putStr("    <name>TOC</name>");
	wf->putCr();
	wf->putStr("    <label>Table Of Contents</label>");
	wf->putCr();

	if (OracleHelp)
		wf->putStr("    <type>oracle.help.navigator.tocNavigator.TOCNavigator</type>");
	else
		wf->putStr("    <type>javax.help.TOCView</type>");

	wf->putCr();

	if (OracleHelp)
		wf->putStr("    <data engine=\"oracle.help.engine.XMLTOCEngine\">");
	else
		wf->putStr("    <data>");

	wf->putStr(fname);
	wf->putStr("TOC.xml</data>");
	wf->putCr();
	wf->putStr("  </view>");
	wf->putCr();

	wf->putCr();
	if (JHVersion2) {
		wf->putStr("  <view ");
		wf->putStr(XHLangAttr);
		wf->putStr("=\"");
		wf->putStr(XhtmlLang);
		wf->putStr("\" mergetype=\"javax.help.SortMerge\">");
	}
	else
		wf->putStr("  <view>");
	wf->putCr();
	wf->putStr("    <name>Index</name>");
	wf->putCr();
	wf->putStr("    <label>Index</label>");
	wf->putCr();

	if (OracleHelp)
		wf->putStr("    <type>oracle.help.navigator.keywordNavigator.KeywordNavigator</type>");
	else
		wf->putStr("    <type>javax.help.IndexView</type>");

 	wf->putCr();

	if (OracleHelp)
		wf->putStr("    <data engine=\"oracle.help.engine.XMLIndexEngine\">");
	else
		wf->putStr("    <data>");

	wf->putStr(fname);
	wf->putStr("Index.xml</data>");
 	wf->putCr();
	wf->putStr(" </view>");
	wf->putCr();

	if (UseFTS) {
		wf->putCr();
		if (JHVersion2) {
			wf->putStr("  <view ");
			wf->putStr(XHLangAttr);
			wf->putStr("=\"");
			wf->putStr(XhtmlLang);
			wf->putStr("\">");
		}
		else
			wf->putStr("  <view>");
		wf->putCr();
		wf->putStr("    <name>Search</name>");
		wf->putCr();
		wf->putStr("    <label>Search</label>");
		wf->putCr();

		if (OracleHelp) {
			wf->putStr("    <title>");
			WriteIndexTitle(wf, HelpFileTitle ? HelpFileTitle : fname);
			wf->putStr("</title>");
			wf->putCr();
   		wf->putStr("    <type>oracle.help.navigator.searchNavigator.SearchNavigator</type>");
			wf->putCr();
			wf->putStr("    <data engine=\"oracle.help.engine.SearchEngine\">");
			wf->putStr(fname);
			wf->putStr(".idx</data>");
		}
		else {
			wf->putStr("    <type>javax.help.SearchView</type>");
			wf->putCr();
			wf->putStr("    <data engine=\"com.sun.java.help.search.DefaultSearchEngine\">");
			wf->putCr();
			wf->putStr("      JavaHelpSearch");
			wf->putCr();
			wf->putStr("    </data>");
		}

		wf->putCr();
		wf->putStr("  </view>");
		wf->putCr();
	}

	if (UseGlossary) {
		wf->putCr();
		wf->putStr("  <view mergetype=\"javax.help.SortMerge\">");
		wf->putCr();
		wf->putStr("    <name>Glossary</name>");
		wf->putCr();
		wf->putStr("    <label>Glossary</label>");
		wf->putCr();
		wf->putStr("    <type>javax.help.GlossaryView</type>");
		wf->putCr();
		wf->putStr("    <data>glossary.xml</data>");
		wf->putCr();
		wf->putStr("  </view>");
		wf->putCr();
	}

	if (UseFavorites) {
		wf->putCr();
		wf->putStr("  <view>");
		wf->putCr();
		wf->putStr("    <name>Favorites</name>");
		wf->putCr();
		wf->putStr("    <label>Favorites</label>");
		wf->putCr();
		wf->putStr("    <type>javax.help.FavoritesView</type>");
		wf->putCr();
		wf->putStr("  </view>");
		wf->putCr();
	}

/*
   <!-- This window is the default one for the helpset. 
     *  Its title bar says "Project X Help". It
     *  is a tri-paned window because displayviews, not
     *  defined, defaults to true and because a toolbar is defined.
     *  The toolbar has a back arrow, a forward arrow, and
     *  a home button that has a user-defined image.
   -->
   <presentation default=true>
       <name>main window</name>
       <size width=400 height=400 /> 
       <location x=200 y=200 />
       <title>Project X Help</title>
       <toolbar>
           <helpaction>javax.help.BackAction</helpaction>
           <helpaction>javax.help.ForwardAction</helpaction>
           <helpaction image="homeicon">javax.help.HomeAction</helpaction>
       </toolbar>
   </presentation>

   <!-- This window is simpler than the main window. 
     *  It's intended to be used a secondary window.
     *  It has no navigation pane or toolbar.
   -->
   <presentation displayviews=false>
       <name>secondary window</name>
       <size width=200 height=200 /> 
       <location x=200 y=200 />
   </presentation>
 
*/
	if (JHVersion2
	 && IniFile
	 && IniFile->Section("JavaHelpOptions")
	 && IniFile->Find("Windows")
	 && IniFile->ListRight()) {
		char *item = NULL;
		DCnlist *winds = new DCnlist;
		long val = 0;
		while ((item = IniFile->ListItem()) != NULL)
			winds->add(item, ++val);
		winds->writeall(wf, PutJHPresentation);
	}

	if (UseSubHelpSets) {
		if (IniFile
		 && IniFile->Find("HelpMergePaths")) {
			char *cpt = NULL;
			while (IniFile->NextItem()) {
				wf->putStr("  <subhelpset location=\"");
				cpt = IniFile->StrRight();
				wf->putStr(cpt);
				if (!strstr(cpt, ".hs")) {  // add filename
					if (cpt[strlen(cpt) - 1] != '/')
						wf->putChar('/');
					wf->putStr("HelpSet.hs\"/>");
				}
				wf->putCr();
				DeleteName(cpt);
			}
		}
	}

	wf->putCr();

/*
   <!-- implementation section -->
   <impl>
      <helpsetregistry helpbrokerclass="javax.help.DefaultHelpBroker" />
      <viewerregistry viewertype="text/html" 
         viewerclass="com.sun.java.help.impl.CustomKit />
      <viewerregistry viewertype="text/xml" 
         viewerclass="com.sun.java.help.impl.CustomXMLKit />
   </impl>

*/
	if (JHVersion2
	 && IniFile
	 && IniFile->Section("JH2_HelpsetAddition"))
		wf->putStr(IniFile->SecCont());

	wf->putStr("</helpset>");
	wf->putCr();

	wf->CloseFile();
	DeleteName(name);
	if (HomeID) {
		DeleteName(HomeID);
		HomeID = NULL;
	}
}

void
HTMwr::PutJHPresentation(DCwfile *wf, char *item, long id)
{
	if (id == 0)
		return;

	char *chp = NULL;

	if (!IniFile->Section(item))
		return;

	wf->putCr();
	wf->putStr("  <presentation ");
	wf->putStr(XHLangAttr);
	wf->putStr("=\"");
	wf->putStr(XhtmlLang);
	wf->putStr("\"");
	if (IniFile->Find("Default")
	 && (IniFile->BoolRight() == true))
		wf->putStr(" default=\"true\"");
	if (IniFile->Find("NavPane")
	 && (IniFile->BoolRight() == false))
		wf->putStr(" displayviews=\"false\"");
	if (IniFile->Find("NavIcons")
	 && (IniFile->BoolRight() == false))
		wf->putStr(" displayviewimages=\"false\"");
	wf->putStr(">");
	wf->putCr();
	if (IniFile->Find("Name")) {
		wf->putStr("    <name>");
		wf->putStr(IniFile->StrRight());
		wf->putStr("</name>");
		wf->putCr();
	}

	wf->putStr("    <size");
	wf->putStr(" width=");
	chp = NULL;
	if (IniFile->Find("Width"))
		chp = IniFile->StrRight();
	wf->putStr(chp ? chp : "400");
	wf->putStr(" height=");
	chp = NULL;
	if (IniFile->Find("Height"))
		chp = IniFile->StrRight();
	wf->putStr(chp ? chp : "400");
	wf->putStr(" />");
	wf->putCr();

	wf->putStr("    <location");
	wf->putStr(" x=");
	chp = NULL;
	if (IniFile->Find("Left"))
		chp = IniFile->StrRight();
	wf->putStr(chp ? chp : "200");
	wf->putStr(" y=");
	chp = NULL;
	if (IniFile->Find("Top"))
		chp = IniFile->StrRight();
	wf->putStr(chp ? chp : "200");
	wf->putStr(" />");
	wf->putCr();

	if (IniFile->Find("Title")) {
		wf->putStr("    <title>");
		wf->putStr(IniFile->StrRight());
		wf->putStr("</title>");
		wf->putCr();
	}
	if (IniFile->Find("Image")) {
		char *cpt = NULL;
		wf->putStr("    <image>");
		wf->putStr(cpt = IniFile->StrRight());
		MapJHImage(cpt);
		DeleteName(cpt);
		wf->putStr("</image>");
		wf->putCr();
	}
	if (IniFile->Find("Toolbar")) {
		wf->putStr("    <toolbar>");
		wf->putCr();
		if (IniFile->ListRight()) {
			char *action = NULL;
			while ((action = IniFile->ListItem()) != NULL)
				PutJHHelpAction(wf, action);
		}
		wf->putStr("    </toolbar>");
		wf->putCr();
	}
	wf->putStr("  </presentation>");
	wf->putCr();
}

void
HTMwr::PutJHHelpAction(DCwfile *wf, char *action)
{
	char *image = NewName(action, strlen(action) + 5);
	strcat(image, "Image");

	wf->putStr("      <helpaction");
	if (IniFile->Find(image)) {
		wf->putStr(" image=\"");
		if (!JHImages) {  // older syntax
			char *cpt = NULL;
			wf->putStr(image);
			wf->putStr("\"");
			WriteMapFileItem(image, cpt = IniFile->StrRight());
			DeleteName(cpt);
		}
		else {
			wf->putStr(image = IniFile->StrRight());
			wf->putStr("\"");
			MapJHImage(image);
			DeleteName(image);
		}
	}
	wf->putStr(">javax.help.");
	wf->putStr(action);
	wf->putStr("Action</helpaction>");
	wf->putCr();
}

void
HTMwr::MapJHImage(char *image)
{
	long ix = 0;

	// add image to map file
	if ((ix = JHImagesName(image)) != 0)
		WriteMapFileItem(image, JHImagesLoc.find(ix));
}


void
HTMwr::WriteOHProjectFile(char *fname)
{
	if (DefaultTopicFile && *DefaultTopicFile) {
		char *per = strrchr(DefaultTopicFile, '.');
		if (per)
			*per = '\0';
		MainName = NewName(strlen(DefaultTopicFile) + strlen(XrefFileSuffix) + 1);
		sprintf(MainName, "%s.%s", DefaultTopicFile, XrefFileSuffix);
	}

	// write settings (JS) file from its template
	OHTemplate = new DCirfile(ProjectTemplate);
	char *pname = NewName(OHProjFilePrefix, strlen(fname)
	                      + strlen(OHProjFilePrefix) + strlen(OHProjFileSuffix));
	strcat(pname, fname);
	strcat(pname, OHProjFileSuffix);
	WriteOHProjFile("Project", pname, XrefFileSuffix, OHPFileDef);
	DeleteName(pname);
	WriteOHProjFile("Settings", fname, "_ohx.js", OHXFileDef);
	OHTemplate->CloseFile();
	OHTemplate = NULL;
}


char *HTMwr::OHProjHeadHtml = {
 "<!DOCTYPE html PUBLIC \"-//W3C//DTD HTML 4.01 Transitional//EN\">\n"
 "<html lang=\"en\"><head><title><$$_ohtitle></title>\n"
 "<meta http-equiv=\"Content-type\" content=\"text/html; charset='iso-8859-1'\">\n"
 "<script language=\"JavaScript\" type=\"text/javascript\" src=\"ohlang.js\">\n"
 "</script>\n"
};


char *HTMwr::OHProjHeadXhtml = {
// "<?xml version=\"1.0\" encoding=\"UTF-8\" ?>\n"
 "<!DOCTYPE html PUBLIC \"-//W3C//DTD XHTML 1.0 Transitional//EN\"\n"
 "  \"http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd\">\n"
 "<html xmlns=\"http://www.w3.org/1999/xhtml\" xml:lang=\"en\" lang=\"en\">\n"
 "<head><title><$$_ohtitle></title>\n"
 "<meta http-equiv=\"Content-type\" content=\"text/html; charset='iso-8859-1'\" />\n"
 "<script language=\"JavaScript\" type=\"text/javascript\" src=\"oxlang.js\">\n"
 "</script>\n"
};

char *HTMwr::OHRunMsg = {
"OmniHelp loading...<br />If the OmniHelp viewer "
 "fails to open, viewer files were not found."
};

char *HTMwr::OHPFileDef = {
 "<$_ohprojhead>"
 "<script language=\"JavaScript\" type=\"text/javascript\">\n"
 "<!--\n"
 "var projName = \"<$$_ohprojname>\"\n"
 "var projTitle = \"<$$_ohtitle>\"\n"
 "//-->\n"
 "</script>\n"
 "<script language=\"JavaScript\" type=\"text/javascript\" src=\"<$$_ohprojname>_ohx.js\">\n"
 "</script>\n"
 "<script language=\"JavaScript\" type=\"text/javascript\" src=\"ohstart.js\">\n"
 "</script>\n"
 "</head><body><p><$$_ohrunmsg>"
//OmniHelp loading...<br />If the OmniHelp viewer\n"
// "fails to open, viewer files were not found.
 "</p>\n"
 "<script language=\"JavaScript\" type=\"text/javascript\">\n"
 "<!--\n"
 "if (loadFrameset() == true) {\n"
 "  window.setTimeout(\"window.close()\",500)\n"
 "  window.opener = top\n"
 "}\n"
 "//-->\n"
 "</script></body></html>\n"
};

char *HTMwr::OHXFileDef = {
 "// OmniHelp JS user-settable variables file, OHver 0.8\n"
 "\n"
 "var mainName = \"<$$_ohmainname>\"\n"
 "\n"
 "var newWindow = <$$_ohnewwin>\n"
 "var closeWindow = <$$_oholdwin>\n"
 "var frameHigh = <$$_ohframehigh>\n"
 "var frameWide = <$$_ohframewide>\n"
 "var frameOptions = \"<$$_ohframeopt>\"\n"
 "var frameBorder = false\n"
 "var topFirst = <$$_ohtopfirst>\n"
 "var topHigh = <$$_ohtophigh>\n"
 "var leftWide = <$$_ohleftwide>\n"
 "var midHigh = <$$_ohmidhigh>\n"
 "\n"
 "var showNavLeft = <$$_ohshownav>\n"
 "var topLeft = '<$OHTopLeftNav>'\n"
 "var topButtons = <$$_ohtopbut>\n"
 "var useStart = <$$_ohusestart>\n"
 "var usePrevNext = <$$_ohuseprev>\n"
 "var useBackForward = <$$_ohuseback>\n"
 "var useHideShow = <$$_ohusehide>\n"
 "var validImg = '<$$_ohvalidimg>'\n"
 "var validAlt = '<$$_ohvalidalt>'\n"
 "var topRight = '<$OHTopRightNav>'\n"
 "\n"
 "var lowMem = <$$_ohlowmem>\n"
 "var persistSettings = <$$_ohpersist>\n"
 "var ignoreCharsIX = \"<$$_ohignoreix>\"\n"
 "var ignoreLeadCharsIX = \"<$$_ohigleadix>\"\n"
 "\n"
 "var ctrlCssName = \"<$$_ohctrlcssname>\"\n"
 "var mainCssName = \"<$$_ohmaincssname>\"\n"
 "var IECtrlCssName = \"<$$_ohiectrlcssname>\"\n"
 "var N6CtrlCssName = \"<$$_ohn6ctrlcssname>\"\n"
 "var N4CtrlCssName = \"<$$_ohn4ctrlcssname>\"\n"
 "var IECssName = \"<$$_ohiecssname>\"\n"
 "var N6CssName = \"<$$_ohn6cssname>\"\n"
 "var N4CssName = \"<$$_ohn4cssname>\"\n"
 "\n"
 "var useNavToc = <$$_ohusetoc>\n"
 "var useNavIdx = <$$_ohuseidx>\n"
 "var useNavFts = <$$_ohusefts>\n"
 "var useNavRel = <$$_ohuserel>\n"
 "\n"
 "var tocGroupsOpen = <$$_ohtocgrp>\n"
 "var tocOpenLevel = <$$_ohtoclev>\n"
 "var tocIcoBase = \"<$$_ohtocico>\"\n"
 "var tocExpand = <$$_ohtocexp>\n"
 "var tocOpenCloseButtons = <$$_ohtocbut>\n"
 "\n"
 "var idxGroupsOpen = <$$_ohidxgrp>\n"
 "var idxOpenLevel = <$$_ohidxlev>\n"
 "var idxIcoBase = \"<$$_ohidxico>\"\n"
 "var idxExpand = <$$_ohidxexp>\n"
 "var idxOpenCloseButtons = <$$_ohidxbut>\n"
 "var idxSeeTerm = \"<$$_ohidxst>\"\n"
 "var idxSeeAlsoTerm = \"<$$_ohidxsat>\"\n"
 "\n"
 "var relShowSubjects = <$$_ohrelshow>\n"
 "\n"
 "var listButton = <$$_ohlistbut>\n"
 "var ftsHighlight  = <$$_ohftshigh>\n"
 "var ftsHighlightStyle  = \"<$$_ohftshsty>\"\n"
 "\n"
 "var mergeProjects = [ <$_ohmerge> ]\n"
 "var mergeFirst = <$$_ohmergefirst>\n"
 "\n"
 "// end of <$$_ohprojname>_ohx.js\n"
 "\n"
};


void
HTMwr::WriteOHProjFile(char *tpl, char *name, char *ext, char *cp)
{
	char *dest = NewName(strlen(name) + strlen(ext) + 2);
	sprintf(dest, (strchr(ext, '.') ? "%s%s" : "%s.%s"), name, ext);
	HTMwfile *wf = new HTMwfile(dest);
	if (wf->FErr() == fok) {
		DCmacr::InitMacroModule(wf);
		if (OHTemplate
		 && OHTemplate->Section(tpl)) {
			char *txt = NULL;
			DCmacr::WriteMacro(wf, txt = OHTemplate->SecCont());
			DeleteName(txt);
		}
		else if (cp != NULL)
			DCmacr::WriteMacro(wf, cp);
		wf->CloseFile();
	}
	DeleteName(dest);
}


char *
HTMwr::FixOHString(unc *chp)
{
	char *tstr = NewName(strlen((char *) chp) * 6);
	char *nstr = NewName(NumStrMax + 2);
	char *tptr = tstr;
	size_t i = 0;
	size_t len = 0;

	for (i = 0; i < strlen((char *) chp); i++) {
		switch (chp[i]) {
			case '\\':
				if (chp[i + 1] != '"') {
					//*tptr++ = '\\';
					strcpy(tptr, "&#092;");
					tptr += 6;
				}
				break;

			case '"':
			case 132:  // baseline
			case 147:  // left dbl
			case 148:  // right dbl
			case 171:  // left guillemot
			case 187:  // right guillemot
				//*tptr++ = '\\';
				//*tptr++ = '"';
				strcpy(tptr, "&quot;");
				tptr += 6;
				break;

			case 130:  // baseline
			case 145:  // left single
			case 146:  // right single
				//*tptr++ = '\'';
				strcpy(tptr, "&#039;");
				tptr += 6;
				break;

      case 0xA0: // fixed space
				*tptr++ = ' ';
        break;

      case 0xAD: // disc hyph
				break;

			case 150:  // en dash
				*tptr++ = '-';
				break;
			case 151:  // em dash
				*tptr++ = '-';
				*tptr++ = '-';
				break;

			case '<':
				strcpy(tptr, "&lt;");
				tptr += 4;
				break;
			case '>':
				strcpy(tptr, "&gt;");
				tptr += 4;
				break;

			case '&': // if already an entity, don't double code it
				if ((len = IsEntity((char *) chp + i + 1)) > 0) {
					strncpy(tptr, (char *) chp + i, len + 1);
					tptr += len + 1;
					i += len;
				}
				else {
					strcpy(tptr, "&amp;");
					tptr += 5;
				}
				break;

			case '\n':
			case '\r':
				break;

			default:
				if (chp[i] < ' ')
					break;
				else if (chp[i] > 0x7E) {
					unl wch = 0;
					unc *cpt = &(chp[i]);
					if ((wch = ::UTF8(&cpt)) != 0) {
						i = (cpt - chp) - 1;
						sprintf(nstr, "&#%0.3ld;", wch);
					}
					else
						sprintf(nstr, "&#%0.3hd;", ((short) chp[i]) & 0xFF);
					strcpy(tptr, nstr);
					tptr += strlen(nstr);
				}
				else
					*tptr++ = chp[i];
				break;
		}
	}
	*tptr = '\0';

	char *ret = NewName(tstr);
	DeleteName(tstr);
	DeleteName(nstr);
	return ret;
}


size_t
HTMwr::IsEntity(char *str)
{
	// called with possible entity, starting after '&'
	size_t len = 0;
	if (*str == '#') {
		len = 1;
		str++;
	}
	while (*str
	 && isalnum(*str)) {
		len++;
		str++;
	}
	return (*str == ';') ? len + 1 : 0;
}


char *
HTMwr::OHMacroVars(char *name)
{
	char *txt = NULL;

	name = name + 2;  // remove the oh

	if (!stricmp(name, "projname"))
		return OHProjectName;
	else if (!stricmp(name, "title"))
		return OHTitle;
	else if (!stricmp(name, "mainname"))
		return MainName ? MainName : "";
	else if (!stricmp(name, "runmsg"))
		return OHRunMsg;

	else if (!stricmp(name, "frameleft"))
		return FrameLeft;
	else if (!stricmp(name, "frametop"))
		return FrameTop;
	else if (!stricmp(name, "framehigh"))
		return FrameHigh;
	else if (!stricmp(name, "framewide"))
		return FrameWide;
	else if (!stricmp(name, "frameopt"))
		return FrameOptions;
	else if (!stricmp(name, "topfirst"))
		return TopFirst ?  "true" : "false";
	else if (!stricmp(name, "tophigh"))
		return TopHigh;
	else if (!stricmp(name, "leftwide"))
		return LeftWide;
	else if (!stricmp(name, "midhigh"))
		return MidHigh;
	else if (!stricmp(name, "validimg"))
		return OHProjFileXhtml ? "ohvalidx.gif" : "ohvalidh.gif";
	else if (!stricmp(name, "validalt"))
		return OHProjFileXhtml ? "Valid XHTML 1.0!" : "Valid HTML 4.01!";

	else if (!stricmp(name, "topbut"))
		return UseTopButtons ?  "true" : "false";
	else if (!stricmp(name, "usestart"))
		return UseStart ?  "true" : "false";
	else if (!stricmp(name, "useprev"))
		return UsePrevNext ?  "true" : "false";
	else if (!stricmp(name, "useback"))
		return UseBackForward ?  "true" : "false";
	else if (!stricmp(name, "usehide"))
		return UseHideShow ?  "true" : "false";
	else if (!stricmp(name, "shownav"))
		return ShowNavLeft ?  "true" : "false";
	else if (!stricmp(name, "newwin"))
		return NewWindow ?  "true" : "false";
	else if (!stricmp(name, "oldwin"))
		return CloseOldWindow ?  "true" : "false";
	else if (!stricmp(name, "persist"))
		return PersistSettings ?  "true" : "false";
	else if (!stricmp(name, "lowmem"))
		return OHLowMem ?  "true" : "false";
	else if (!stricmp(name, "ignoreix"))
		return IgnoreCharsIX;
	else if (!stricmp(name, "igleadix"))
		return IgnoreLeadingCharsIX;

	else if (!stricmp(name, "ctrlcssname"))
		return CtrlCssName;
	else if (!stricmp(name, "maincssname"))
		return MainCssName;
	else if (!stricmp(name, "iectrlcssname"))
		return IECtrlCssName;
	else if (!stricmp(name, "n6ctrlcssname"))
		return N6CtrlCssName;
	else if (!stricmp(name, "n4ctrlcssname"))
		return N4CtrlCssName;
	else if (!stricmp(name, "iecssname"))
		return IECssName ? IECssName : MainCssName;
	else if (!stricmp(name, "n6cssname"))
		return N6CssName ? N6CssName : MainCssName;
	else if (!stricmp(name, "n4cssname"))
		return N4CssName ? N4CssName : MainCssName;

	else if (!stricmp(name, "usetoc")) {
		if (!NavElems)
			return "true";
		else
			return strstr(NavElems, "Toc") ? "true" : "false";
	}
	else if (!stricmp(name, "useidx")) {
		if (!NavElems)
			return "true";
		else
			return strstr(NavElems, "Idx") ? "true" : "false";
	}
	else if (!stricmp(name, "usefts")) {
		if ((!NavElems) || UseFTS)
			return "true";
		else
			return strstr(NavElems, "Fts") ? "true" : "false";
	}
	else if (!stricmp(name, "userel")) {
		if (!NavElems)
			return "true";
		else
			return strstr(NavElems, "Rel") ? "true" : "false";
	}

	else if (!stricmp(name, "tocgrp"))
		return OHTocGroupsOpen ?  "true" : "false";
	else if (!stricmp(name, "toclev"))
		return OHTocOpenLevel;
	else if (!stricmp(name, "tocico"))
		return OHTocIcoBase;
	else if (!stricmp(name, "tocexp"))
		return OHTocExpand ?  "true" : "false";
	else if (!stricmp(name, "tocbut"))
		return OHTocButtons ?  "true" : "false";

	else if (!stricmp(name, "idxgrp"))
		return OHIdxGroupsOpen ?  "true" : "false";
	else if (!stricmp(name, "idxlev"))
		return OHIdxOpenLevel;
	else if (!stricmp(name, "idxico"))
		return OHIdxIcoBase;
	else if (!stricmp(name, "idxexp"))
		return OHIdxExpand ?  "true" : "false";
	else if (!stricmp(name, "idxbut"))
		return OHIdxButtons ?  "true" : "false";
	else if (!stricmp(name, "idxst"))
		return SeeTerm;
	else if (!stricmp(name, "idxsat"))
		return SeeAlsoTerm;

	else if (!stricmp(name, "relshow"))
		return OHRelShowSubjects ?  "true" : "false";
	else if (!stricmp(name, "listbut"))
		return UseListButton ?  "true" : "false";
	else if (!stricmp(name, "ftshigh"))
		return UseSearchHighlight ?  "true" : "false";
	else if (!stricmp(name, "ftshsty"))
		return SearchHighlightStyle;

	else if (!stricmp(name, "mergefirst"))
		return MergeFirst ?  "true" : "false";

	else if (NameUndefMacroVars) // give up and punt
		return name - 2;
	else
		return "";
}


void
HTMwr::WriteOHProjHead(HTMwfile *wf)
{
	if (OHTemplate
	 && OHTemplate->Section(OHProjFileXhtml ? "ProjHeadXhtml" : "ProjHeadHtml")) {
		char *txt = NULL;
		DCmacr::WriteMacro(wf, txt = OHTemplate->SecCont());
		DeleteName(txt);
	}
	else
		DCmacr::WriteMacro(wf, OHProjFileXhtml ? OHProjHeadXhtml : OHProjHeadHtml);
}


void
HTMwr::WriteOHMergeInfo(HTMwfile *wf)
{
	if (IniFile
	 && IniFile->Section("HelpMerge")) {
		while (IniFile->NextItem())
			MergeList.add((long) IniFile->StrLeft());
		if (MergeList.count()) {
			wf->putCr();
			MergeList.writeall(wf, WriteOHMergeItem);
		}
	}
}

void
HTMwr::WriteOHMergeItem(DCwfile *dwf, long id)
{
	static bool starting = true;

	if (!id) {
		starting = true;
		return;
	}

	HTMwfile *wf = (HTMwfile *) dwf;
	char *name = (char *) id;

	if (!starting) {
		wf->putChar(',');
		wf->putCr();
	}
	else
		starting = false;

	wf->putStr("[\"");
	wf->putStr(name);
	wf->putStr("\",0,0,");  // status, not merged yet and size, not set yet

	wf->putShort((short) MergeLocList(name));  // number in TOC for merge marker pos

	wf->putStr(",[");
	bool first = true;
	if (IniFile
	 && IniFile->Section("OHMergeFiles")) {
		while (IniFile->NextItem()) {
			if (!stricmp(IniFile->StrRight(), name)) {
				if (!first)
					wf->putChar(',');
				else
					first = false;
				wf->putChar('"');
				wf->putStr(IniFile->StrLeft());
				wf->putChar('"');
			}
		}
	}
	wf->putStr("]]");
}



void
HTMwr::WriteContentsStart(void)
{
	ContentsFileStarting = true;

	// write head for full file
	if (HelpType == 'M') {
		ContentsFile->putStr("<!DOCTYPE HTML PUBLIC \"-//IETF//DTD HTML//EN\">");
		ContentsFile->putCr();
		ContentsFile->putStr("<html><head>");
		ContentsFile->putCr();
		if (GeneratorTag == 'M') {
			ContentsFile->putStr("<meta name=\"GENERATOR\" content=\"Omni Systems ");
			ContentsFile->putStr(FilterVer);
			ContentsFile->putStr("\">");
			ContentsFile->putCr();
		}
		ContentsFile->putStr("<!-- Sitemap 1.0 -->");
		ContentsFile->putCr();
		ContentsFile->putStr("</head><body>");
		ContentsFile->putCr();
		if (HHCProperties) {
			ContentsFile->putStr("<object type=\"text/site properties\">");
			ContentsFile->putCr();
			DCmacr::WriteMacro((HTMwfile *) ContentsFile, HHCProperties);
			ContentsFile->putCr();
			ContentsFile->putStr("</object>");
			ContentsFile->putCr();
		}
		ContentsLevEmpty = false;
	}
	else if (HelpType == 'E') {
		ContentsFile->putStr("<?xml version=\"1.0\"?>");
		ContentsFile->putCr();
		ContentsFile->putStr("<toc label=\"");
		ContentsFile->putStr(EclipseTocLabel ? EclipseTocLabel : EclipsePlugName);
		if (EclipseTocLink) {
			ContentsFile->putStr("\" link_to=\"");
			ContentsFile->putStr(EclipseTocLink);
		}
		if (EclipseTocTopic) {
			ContentsFile->putStr("\" topic=\"");
			if (TocIdxFilePrefix && *TocIdxFilePrefix)
				ContentsFile->putStr(TocIdxFilePrefix);
			ContentsFile->putStr(EclipseTocTopic);
		}
		ContentsFile->putStr("\">");
		ContentsFile->putCr();
		ContentsFile->putCr();
	}
	else if (HelpType == 'J') {
		ContentsFile->putStr("<?xml version='1.0' encoding='ISO-8859-1' ?>");
		ContentsFile->putCr();
		if (!OracleHelp) {
			ContentsFile->putStr("<!DOCTYPE toc");
			ContentsFile->putCr();
			ContentsFile->putStr("  PUBLIC \"-//Sun Microsystems Inc.//DTD JavaHelp TOC Version");
	 		ContentsFile->putStr(JHFileVer);
			ContentsFile->putStr("//EN\"");
			ContentsFile->putCr();
			ContentsFile->putStr("         \"http://java.sun.com/products/javahelp/toc_");
			ContentsFile->putStr(JHDtdVer);
			ContentsFile->putStr(".dtd\">");
			ContentsFile->putCr();
		}
		ContentsFile->putStr("<toc version=\"");
		ContentsFile->putStr(JHFileVer);
		if (JHVersion2
		 && JHImages
		 && IniFile
		 && IniFile->Section("JavaHelpOptions")) {
			char *cpt = NULL;
			if (IniFile->Find("TocClosedImage")) {
				cpt = IniFile->StrRight();
				ContentsFile->putStr("\" categoryclosedimage=\"");
				ContentsFile->putStr(cpt);
				MapJHImage(cpt);
				DeleteName(cpt);
				cpt = NULL;
			}
			if (IniFile->Find("TocOpenImage")) {
				cpt = IniFile->StrRight();
				ContentsFile->putStr("\" categoryopenimage=\"");
				ContentsFile->putStr(cpt);
				MapJHImage(cpt);
				DeleteName(cpt);
				cpt = NULL;
			}
			if (IniFile->Find("TocTopicImage")) {
				cpt = IniFile->StrRight();
				ContentsFile->putStr("\" topicimage=\"");
				ContentsFile->putStr(cpt);
				MapJHImage(cpt);
				DeleteName(cpt);
				cpt = NULL;
			}
		}
		ContentsFile->putStr("\">");
		ContentsFile->putCr();
		ContentsFile->putCr();
	}
	else if (HelpType == 'C') {
		ContentsFile->putStr("var tocItems = [");
		ContentsFile->putCr();
	}

	ContentsLevel = 0;
	ContentsID = 0;
}


void
HTMwr::WriteSearchStart(void)
{
	SearchFile->putStr("var ftsItems = [");
	SearchFile->putCr();
}


void
HTMwr::WriteIndexStart(void)
{
	IndexFileStarting = true;

	// write head for full file
	if (HelpType == 'M') {
		IndexFile->putStr("<!DOCTYPE HTML PUBLIC \"-//IETF//DTD HTML//EN\">");
		IndexFile->putCr();
		IndexFile->putStr("<html><head>");
		IndexFile->putCr();
		if (GeneratorTag == 'M') {
			IndexFile->putStr("<meta name=\"GENERATOR\" content=\"Omni Systems ");
			IndexFile->putStr(FilterVer);
			IndexFile->putStr("\">");
			IndexFile->putCr();
		}
		IndexFile->putStr("<!-- Sitemap 1.0 -->");
		IndexFile->putCr();
		IndexFile->putStr("</head><body>");
		IndexFile->putCr();
		if (HHKProperties) {
			IndexFile->putStr("<object type=\"text/site properties\">");
			IndexFile->putCr();
			DCmacr::WriteMacro((HTMwfile *) IndexFile, HHKProperties);
			IndexFile->putCr();
			IndexFile->putStr("</object>");
			IndexFile->putCr();
		}
	}
	else if (HelpType == 'E') {
		IndexFile->putStr("<?xml version=\"1.0\"?>");
		IndexFile->putCr();
		IndexFile->putStr("<index label=\"");
		IndexFile->putStr(EclipseTocLabel ? EclipseTocLabel : EclipsePlugName);
		IndexFile->putStr("\">");
		IndexFile->putCr();
		IndexFile->putCr();
	}
	else if (HelpType == 'J') {
		IndexFile->putStr("<?xml version='1.0' encoding='ISO-8859-1' ?>");
		IndexFile->putCr();
		if (!OracleHelp) {
			IndexFile->putStr("<!DOCTYPE index");
			IndexFile->putCr();
			IndexFile->putStr("  PUBLIC \"-//Sun Microsystems Inc.//DTD JavaHelp Index Version ");
			IndexFile->putStr(JHFileVer);
			IndexFile->putStr("//EN\"");
			IndexFile->putCr();
			IndexFile->putStr("         \"http://java.sun.com/products/javahelp/index_");
			IndexFile->putStr(JHDtdVer);
			IndexFile->putStr(".dtd\">");
			IndexFile->putCr();
		}
		IndexFile->putStr("<index version=\"");
		IndexFile->putStr(JHFileVer);
		IndexFile->putStr("\">");
		IndexFile->putCr();
		IndexFile->putCr();
	}
	else if (HelpType == 'C') {
		IndexFile->putStr("var idxItems = [");
		IndexFile->putCr();
	}

#if 0
	else if ((HelpType == 'S')
	 && UseIndex) {
		HTMwfile *wf = (HTMwfile *) IndexFile;
		if (SplittingFile) {
			WriteProlog(wf, 1);
			Title = IndexFileTitle;
			WriteHead(wf, WriteIndexCSS);
			wf->startBlock("body");
			WriteAttrib(wf, "body");
			wf->endElem(true);
			WriteBaseProps(wf);
			wf->putCr();
			WriteIndexJS(wf);
			PutInsertMacro(wf, "LastTop");
		}
		else {
			PutInsertMacro(wf, "TopicBreak");
			IndexFile->putStr("<a name=\"genindex\"></a>");
			IndexFile->putCr();
		}

		if (UseIndexHeading) {
			IndexFile->putStr("<p class=\"");
			IndexFile->putStr(IndexHeadingClass);
			IndexFile->putStr("\">");
			IndexFile->putStr(IndexFileTitle);
			IndexFile->putStr("</p>");
			IndexFile->putCr();
		}

		WriteSpecialIndexStart();
	}
#endif

	IndexLevel = 0;
}


void
HTMwr::WriteSpecialIndexStart(void)
{
	HTMwfile *wf = (HTMwfile *) IndexFile;

	PutInsertMacro(wf, "IndexTop");

	if (UseListProps ? UseIXLets : UseIndexLetters) {
		if (UseListProps ? UseIXTopLets : UseIndexTopLetters) {
			IndexFile->putStr("<br /><p class=\"");
			IndexFile->putStr(IndexTopLettersClass);
			IndexFile->putStr("\">");
			if (IndexLetterSymbol) {
				IndexFile->putStr("<a href=\"#");
				IndexFile->putStr(IndexLetPrefix);
				IndexFile->putStr("\">");
				IndexFile->putStr(IndexLetterSymbol);
				IndexFile->putStr("</a> ");
			}
			if (IndexLetterNumber) {
				IndexFile->putStr("<a href=\"#");
				IndexFile->putStr(IndexLetPrefix);
				IndexFile->putStr("0\">");
				IndexFile->putStr(IndexLetterNumber);
				IndexFile->putStr("</a> ");
			}
			IndexFile->putCr();
			for (int anum = 0; anum < 26; anum++) {
				IndexFile->putStr("<a href=\"#");
				IndexFile->putStr(IndexLetPrefix);
				IndexFile->putChar('A' + anum);
				IndexFile->putStr("\">");
				IndexFile->putChar('A' + anum);
				IndexFile->putStr("</a> ");
				if ((anum % 4) == 3)
					IndexFile->putCr();
			}
			IndexFile->putStr("</p>");
			IndexFile->putCr();
		}
		IndexFile->putStr("<ul id=\"indextop\" class=\"");
		IndexFile->putStr(IndexLetterClass);
		IndexFile->putStr("\">");
		IndexFile->putCr();
	}
}


char *HTMwr::IndexTopInsert = NULL;
char *HTMwr::IndexHeadInsert = NULL;
char *HTMwr::IndexBottomInsert = NULL;
char *HTMwr::IndexTOCInsert = NULL;
bool HTMwr::InsertIndexTOC = false;
DClist HTMwr::IndexSplitFile;
bool HTMwr::UseDITAIndex = false;


void 
HTMwr::CheckIndexTOCItem(HTMctl *cp)
{
#if 0
	if (cp->siz() == 0) {
		// always written by drxml if no idx in map
		if ((HelpType != 'S')
		 || !UseIndex)
			return;
		// add index toc entry here later
		// will need to generate index file
	}
	else 
#endif

	if (cp->siz() == 1) {
		// written by drxml when idx in map,
		// idx toc entry and head written in dcl
		if (HelpType != 'S')
			return;
		// include CSS/JS in this file's head
		if (SplitID)
			IndexSplitFile.add(SplitID);
		UseIndex = true; 
	}
	else if (cp->siz() == 2) {  // starting drxml index entries
		if (SplitID)
			IndexSplitFile.add(SplitID);
		UseDITAIndex = true;
	}
}


void
HTMwr::InsertIndexTOCItem(HTMwfile *wf, HTMctl *cp)
{
	static long fnum = 0;
	static bool done = false;

	if (cp->siz() == 0) {
		// always written by drxml if no idx in map
		if ((HelpType != 'S')
		 || !UseIndex)
			return;
		// add index toc entry here
		// will need to generate index file
	}
	else if (cp->siz() == 1) {
		// written by drxml when idx in map,
		// idx toc entry and head already written
		if (HelpType != 'S')
			return;
		// add index content after head here
		IndexFile = wf;
		IndexLevel = 0;
		CurrOHLetter = 0;
		WriteSpecialIndexStart();
		SortIndexRefs();
		WriteIndexRefs();
		WriteSpecialIndexEnd();
		// do not generate another index file
		UseIndex = false; 
		return;
	}
	else if (cp->siz() == 2) {  // starting drxml index entries
		UseDITAIndex = true;
		IndexFile = wf;
		IndexLevel = 0;
		CurrOHLetter = 0;
		UseListProps = true;
		WriteSpecialIndexStart();
		return;
	}
	else if (cp->siz() == 3) {  // ending drxml index entries
		WriteSpecialIndexEnd();
		if (UseListProps ? UseIXLets : UseIndexLetters) {
			while (++CurrOHLetter <= 'Z')
				PutIxId(CurrOHLetter);
		}
		PutInsertMacro((HTMwfile *) IndexFile, "IndexBottom");
		UseListProps = false;
		CurrOHLetter = 0;
		IndexLevel = 0;
		UseIndex = false;
		IndexFile = NULL;
		return;
	}
	else if (cp->siz() == 4) {  // starting drxml index entry
		StartIndexEnt();
		return;
	}
	else if (cp->siz() == 5) {  // ending drxml index entry after refs and subs
		EndIndexEnt();
		return;
	}
	else if (cp->siz() == 6) {  // starting drxml index entry refs
		StartIndexRefs();
		return;
	}
	else if (cp->siz() == 7) {  // ending drxml index entry refs
		EndIndexRefs();
		return;
	}
	else if (cp->siz() == 8) {  // starting drxml index entry ref
		StartIndexRef();
		return;
	}
	else if (cp->siz() == 9) {  // ending drxml index entry ref
		EndIndexRef();
		return;
	}
	else if (cp->siz() == 10) {  // inc drxml index level before subs
		StartIndexSubs();
		return;
	}
	else if (cp->siz() == 11) {  // dec drxml index level after subs
		EndIndexSubs();
		return;
	}
	else if (cp->siz() == 12) {  // zero index level
		ZeroIndexLevel();
		return;
	}
	else if (cp->siz() == 13) {  // leader for compact
		wf->putStr(IndexRefStartSep);
		return;
	}
	else if (cp->siz() > 0x20) {  // changing drxml index letter
		SetIndexLetter((char) (cp->siz() & 0xFF));
		return;
	}
	else  // marker for another idx state
		return;

#if 0
	else {
		fnum++;
		if (IniFile->Section("MapText")
		 && IniFile->Find("TOCFileName")) {
			char *toc = IniFile->StrRight();
			char *fn = wf->getBaseName();
			if (stricmp(toc, fn)
			 && (fnum == 1))
				return;
		}
	}
#endif

	if (done)
		return;

	done = true;

	if (IndexTOCInsert)
		PutInsertMacro(wf, "IndexTOC");
	else {
		wf->putStr("<p class=\"");
		wf->putStr(IndexTOCClass);
		wf->putStr("\"><a href=\"");
		if (SplittingFile)
			wf->putStr(SetIndexFileName(CurrXFileBase));
		else
			wf->putStr("#genindex");
		wf->putStr("\">");
		wf->putStr(IndexFileTitle);
		wf->putStr("</a></p>\n");
	}
	InsertIndexTOC = false;
}


void
HTMwr::SetIXListProps(HTMctl *cp)
{
	if (cp->dat() == 2)
		UseIXLets = cp->siz() ? true : false;
	else if (cp->dat() == 3)
		UseIXTopLets = cp->siz() ? true : false;
}


char *
HTMwr::SetIndexFileName(char *fname)
{
	char *name = NULL;
	char *dot = NULL;

	if (IndexFileName) {  // already set in .ini
		int len = strlen(IndexFileName) - strlen(XrefFileSuffix);
		if ((IndexFileName[len - 1] == '.')
		 && !stricmp(IndexFileName + len, XrefFileSuffix))
			return IndexFileName;
		//if ((dot = strrchr(IndexFileSuffix, '.')) != NULL)
		//	*dot = '\0';
		name = NewName(IndexFileName, strlen(IndexFileName)
				                        + strlen(XrefFileSuffix) + 2);
		strcat(name, ".");
		strcat(name, XrefFileSuffix);
		IndexFileName = name;
	}
	else if (IDXSuffix) {  // from drxml
		IndexFileName = name = NewName(fname, strlen(fname)
				        + strlen(IDXSuffix) + strlen(XrefFileSuffix) + 2);
		strcat(name, IDXSuffix);
		strcat(name, ".");
		strcat(name, XrefFileSuffix);
	}
	else if (IndexFileSuffix) {
		if ((dot = strrchr(IndexFileSuffix, '.')) != NULL)
			*dot = '\0';
		IndexFileName = name = NewName(fname, strlen(fname)
				        + strlen(IndexFileSuffix) + strlen(XrefFileSuffix) + 2);
		strcat(name, IndexFileSuffix);
		strcat(name, ".");
		strcat(name, XrefFileSuffix);
	}
	else {
		IndexFileName = name = NewName(fname, strlen(fname)
				                                  + strlen(XrefFileSuffix) + 5);
		strcat(name, "IDX");
		strcat(name, ".");
		strcat(name, XrefFileSuffix);
	}

#if 0
	else if (replace)
		IndexFileName = name = ReplaceExt(fname, IndexFileSuffix);
	else {
		IndexFileName = name = NewName(fname, strlen(fname)
				                           + strlen(IndexFileSuffix) + 1);
		strcat(name, IndexFileSuffix);
	}
#endif

	DCauto::IndexNavType = IndexNavType;

	return name;
}


void
HTMwr::WriteIndexCSS(HTMwfile *wf)
{
	char *fname = NULL;

	switch (IndexNavType) {
		case 'C':   // pure CSS
			fname = "idxcss.css";
			break;

		case 'J':   // JavaScript
			fname = "idxjs.css";
			break;

		case 'H':   // HTML for fully exposed lists
		default:
			fname = "idxhtm.css";
			break;
	}

	if (WriteIndexCssLink)
		WriteOneCSSLink(wf, fname);

	PutInsertMacro(wf, "IndexHead");
}


void
HTMwr::WriteIndexJS(HTMwfile *wf)
{
	if (IndexNavType == 'J') {  //  JavaScript
		// <script type="text/javascript" src="idxjs.js"></script>
		wf->startBlock("script");
		wf->strAttr("type", "text/javascript");
		wf->strAttr("src", "idxjs.js");
		wf->endElem(true);
		wf->endBlock("script");
		wf->putEnd();
	}
}


void
HTMwr::WriteIndexFileFormats(void)
{
	if (UseDITAIndex) {
		WriteOneCSSFormat(IndexTOCClass);
		WriteOneCSSFormat(IndexHeadingClass);
		WriteOneCSSFormat(IndexLevelClass);
		WriteOneCSSFormat(IndexRefClass);
		if (UseIndexLetters
		 || (IndexNavType == 'J')) {
			WriteOneCSSFormat(IndexTopLettersClass);
			WriteOneCSSFormat(IndexLetterClass);
		}
	}
}


void
HTMwr::WriteALinkStart(void)
{
	if (HelpType == 'C') {
		ALinkFile->putStr("var relItems = [");
	}
	else {
		ALinkFile->putStr("<?xml version='1.0' encoding='ISO-8859-1' ?>");
		ALinkFile->putCr();
		ALinkFile->putStr("<link version=\"");
		ALinkFile->putStr(JHFileVer);
		ALinkFile->putStr("\">");
		ALinkFile->putCr();
	}
	ALinkFile->putCr();
}


void
HTMwr::WriteGlossStart(void)
{
	GlossFile->putStr("<?xml version='1.0' encoding='ISO-8859-1' ?>");
	GlossFile->putCr();
	GlossFile->putStr("<!DOCTYPE index");  // yes, uses index format
	GlossFile->putCr();
	GlossFile->putStr("  PUBLIC \"-//Sun Microsystems Inc.//DTD JavaHelp Index Version ");
	GlossFile->putStr(JHFileVer);
	GlossFile->putStr("//EN\"");
	GlossFile->putCr();
	GlossFile->putStr("         \"http://java.sun.com/products/javahelp/index_");
	GlossFile->putStr(JHDtdVer);
	GlossFile->putStr(".dtd\">");
	GlossFile->putCr();
	GlossFile->putStr("<index version=\"");
	GlossFile->putStr(JHFileVer);
	GlossFile->putStr("\">");
	GlossFile->putCr();
	GlossFile->putCr();
}


void
HTMwr::WriteMapFileStart(void)
{
	MapFile->putStr("<?xml version='1.0' encoding='ISO-8859-1' ?>");
	MapFile->putCr();

	if (!OracleHelp) {
		MapFile->putStr("<!DOCTYPE map");
		MapFile->putCr();
		MapFile->putStr("  PUBLIC \"-//Sun Microsystems Inc.//DTD JavaHelp Map Version ");
		MapFile->putStr(JHFileVer);
		MapFile->putStr("//EN\"");
		MapFile->putCr();
		MapFile->putStr("         \"http://java.sun.com/products/javahelp/map_");
		MapFile->putStr(JHDtdVer);
		MapFile->putStr(".dtd\">");
		MapFile->putCr();
	}

	MapFile->putStr("<map version=\"");
	MapFile->putStr(JHFileVer);
	MapFile->putStr("\">");
	MapFile->putCr();
	MapFile->putCr();

	MapItemList = new DCnlist;
}



void
HTMwr::WriteContentsEnd(void)
{
	// write tail for full file
	if (HelpType == 'M') {
		while (ContentsLevel)
			DecContentsLevel();
		ContentsFile->putStr("</body></html>");
		ContentsFile->putCr();
	}
	else if ((HelpType == 'J')
	 || (HelpType == 'E')) {
		if (ContentsLevel > 0) {
			ContentsFile->putStr(" />");  // close last real item
			ContentsFile->putCr();
		}
		while (ContentsLevel > 1) {
			ContentsLevel--;
			WriteContentsSpaces();
			ContentsFile->putStr(Eclipse ? "</topic>" : "</tocitem>");
			ContentsFile->putCr();
		}
		ContentsFile->putCr();
		ContentsFile->putStr("</toc>");
		ContentsFile->putCr();
	}
	else if (HelpType == 'C') {
		ContentsFile->putChar(']');
		ContentsFile->putCr();
	}
}


void
HTMwr::WriteSearchEnd(void)
{
	SearchFile->putStr(" ]");
	SearchFile->putCr();
}


void
HTMwr::WriteIndexEnd(void)
{
	// write tail for full file
	if (HelpType == 'M') {
		while (IndexLevel) {
			IndexLevel--;
			WriteIndexSpaces();
			IndexFile->putStr("</ul>");
			IndexFile->putCr();
		}
		IndexFile->putStr("</body></html>");
		IndexFile->putCr();
	}
	else if (HelpType == 'E') {
		while (IndexLevel > 0) {
			WriteIndexSpaces();
			IndexFile->putStr("</entry>");
			IndexFile->putCr();
			IndexLevel--;
		}
		IndexFile->putCr();
		IndexFile->putStr("</index>");
		IndexFile->putCr();
	}
	else if (HelpType == 'J') {
		if (IndexLevel > 0) {
			if (OracleHelp && UseIndexentryTag) {  // at line after last <indexentry ... />
				WriteIndexSpaces();
				IndexFile->putStr("</indexitem>");
			}
			else
				IndexFile->putStr(" />");  // close last real item
			IndexFile->putCr();
		}
		while (IndexLevel > 1) {
			IndexLevel--;
			WriteIndexSpaces();
			IndexFile->putStr("</indexitem>");
			IndexFile->putCr();
		}
		IndexFile->putCr();
		IndexFile->putStr("</index>");
		IndexFile->putCr();
	}
	else if (HelpType == 'C') {
		IndexFile->putChar(']');
		IndexFile->putCr();
	}

#if 0
	else if ((HelpType == 'S')
	 && UseIndex) {
		HTMwfile *wf = (HTMwfile *) IndexFile;
		WriteSpecialIndexEnd();
		PutInsertMacro(wf, "IndexBottom");
		if (SplittingFile) {
			wf->endBlock("body");  // wrap it up
			if (UseXMLRoot)
				wf->endBlock(XMLRoot);
		}
		wf->putEnd();
	}
#endif

}


void
HTMwr::WriteSpecialIndexEnd(void)
{
	HTMwfile *wf = (HTMwfile *) IndexFile;
	if (IndexLevel) {
		while (IndexLevel > 1) {
			IndexLevel--;
			WriteIndexTabs();
			IndexFile->putStr("</ul></li>");
			IndexFile->putCr();
		}
		IndexFile->putStr("</ul>");
	}
	if (UseListProps ? UseIXLets : UseIndexLetters
	 && CurrOHLetter) { // close last letter
		IndexFile->putStr("</li></ul>");
		IndexFile->putCr();
	}
	wf->putEnd();
}


void
HTMwr::WriteALinkEnd(void)
{
	if (HelpType == 'C') {
		ALinkFile->putChar(']');
	}
	else {
		ALinkFile->putCr();
		ALinkFile->putStr("</link>");
	}
	ALinkFile->putCr();
}


void
HTMwr::WriteGlossEnd(void)
{
	GlossFile->putCr();
	GlossFile->putStr("</index>");
	GlossFile->putCr();
}

void
HTMwr::WriteMapFileEnd(void)
{
	MapFile->putCr();
	MapFile->putStr("</map>");
	MapFile->putCr();

	MapItemList->clear();
	delete MapItemList;
}



void
HTMwr::WriteContentsItem(DCfile *cwf, void *ip)
{
	if (!ip)
		return;

	cntprop *ctp = (cntprop *) ip;

	// write contents entries in final format
	if (HelpType == 'M') {
		while (ContentsLevel > ctp->level)
			DecContentsLevel();
		while (ctp->level > ContentsLevel)
			IncContentsLevel();

		WriteContentsTabs();
		ContentsFile->putStr("<li> <object type=\"text/sitemap\">");
		ContentsFile->putCr();

		if ((*(ctp->ref) == '*')
		 && ((ctp->title == NULL)
		  || (*(ctp->title) == '\0'))) {  // merge entry
			WriteContentsTabs();
			ContentsFile->putStr("\t<param name=\"Merge\" value=\"");
			char *nref = NewName((uns) ((strlen(ctp->ref) - 1) * 2) + 11);
			sprintf(nref, "%s.chm::\\%s.hhc", ctp->ref + 1, ctp->ref + 1);
			ContentsFile->putStr(nref);
			DeleteName(nref);
			ContentsFile->putStr("\">");
			ContentsFile->putCr();
		}
		else {
			WriteContentsTabs();
			ContentsFile->putStr("\t<param name=\"Name\" value=\"");
			//ContentsFile->putStr(ctp->title);
			//if (UTF8 || NumericCharRefs || NoEnc)
			WriteContentsAttr(ContentsFile, ctp->title);
			//else
			//	WriteIndexTitle(ContentsFile, ctp->title);
			ContentsFile->putStr("\">");
			ContentsFile->putCr();

			if (*(ctp->ref) != '\0') {
				WriteContentsTabs();
				ContentsFile->putStr("\t<param name=\"Local\" value=\"");
				if (HHContValPx)
					ContentsFile->putStr(HHContValPx);
				if (cwf) {  // used as flag, NULL for list file processing
					XrefFile = NULL;
					ProcExtRef(ctp->ref);
					ContentsFile->putStr(XrefFile);
				}
				if (HHContFileOnly
				 || NoANames
				 || ctp->noref) {
					char *pound = strrchr(ctp->ref, '#');
					if (pound)
						*pound = '\0';
				}
				ContentsFile->putStr(ctp->ref);
				ContentsFile->putStr("\">");
				ContentsFile->putCr();
				if (ctp->win) {
					WriteContentsTabs();
					ContentsFile->putStr("\t<param name=\"WindowName\" value=\"");
					ContentsFile->putStr(ctp->win);
					ContentsFile->putStr("\">");
					ContentsFile->putCr();
				}
			}
		}

		WriteContentsTabs();
		ContentsFile->putStr("\t</object>");
		ContentsFile->putCr();

		ContentsLevEmpty = false;
	}
	else if (HelpType == 'E') {
		char *refer = NULL;
		char *pound = strrchr(ctp->ref, '#');
		long val = 0;
		char *cpt = NULL;

		if (pound) {
			refer = ctp->ref;
			if (cwf) {  // used as flag, NULL for list file processing
				XrefFile = NULL;
				ProcExtRef(pound);
				refer = NewName(XrefFile, strlen(XrefFile) + strlen(ctp->ref) + 1);
				strcat(refer, ctp->ref);
			}
		}

		if (ContentsLevel >= ctp->level) {
			ContentsFile->putStr("/>");  // close last real item unnested
			ContentsFile->putCr();
			while (ContentsLevel > ctp->level) {
				ContentsLevel--;
				WriteContentsSpaces();
				ContentsFile->putStr("</topic>");
				ContentsFile->putCr();
				ContentsFile->putCr();
			}
		}
		else if (ContentsLevel > 0) {
			ContentsFile->putStr(">");  // close last real item for nesting
			ContentsFile->putCr();
			while (ctp->level > ContentsLevel) {
				ContentsLevel++;
				if (ctp->level > ContentsLevel) {
					WriteContentsSpaces();
					ContentsFile->putStr("<topic>");
					ContentsFile->putCr();
					LogEvent(logwarn,1,"Level skipped in TOC before ", ctp->title);
				}
			}
		}
		else // just starting, nothing to close
			ContentsLevel = 1;

		if (!ctp->title
		 || !*ctp->title) { // either link or anchor
			char *cref = ctp->ref + 1;
			if (*(ctp->ref) == '*') {  // set by EclipseAnchor marker, specifies anchor
			                           // to be referenced by another toc link_to attr
				WriteContentsSpaces();
				ContentsFile->putStr("<anchor id=\"");
				if (UTF8 || NumericCharRefs || NoEnc)
					WriteContentsAttr(ContentsFile, cref);
				else
					WriteIndexTitle(ContentsFile, cref);
				ContentsFile->putChar('"');  // leave off ending
			}
			else if (*(ctp->ref) == '=') {  // set by EclipseLink marker, specifies name
			                                // of another toc.xml file to merge here
				WriteContentsSpaces();
				ContentsFile->putStr("<link toc=\"");
				if (UTF8 || NumericCharRefs || NoEnc)
					WriteContentsAttr(ContentsFile, cref);
				else
					WriteIndexTitle(ContentsFile, cref);
				ContentsFile->putChar('"');  // leave off ending
			}
		}
		else {
			WriteContentsSpaces();
			ContentsFile->putStr("<topic label=\"");
			if (UTF8 || NumericCharRefs || NoEnc)
				WriteContentsAttr(ContentsFile, ctp->title);
			else
				WriteIndexTitle(ContentsFile, ctp->title);

			// may add Eclipse filter="name=value" attribute to topic ****
			if (refer) { // may be omitted
				ContentsFile->putChar('"');
				ContentsFile->putCr();
				WriteContentsSpaces();
				ContentsFile->putStr(" href=\"");
				if (pound
				 && (EclipseTocFileOnly
				  || NoANames
				  || ctp->noref))
					*(pound = strrchr(refer, '#')) = '\0';
				if (TocIdxFilePrefix && *TocIdxFilePrefix)
					ContentsFile->putStr(TocIdxFilePrefix);
				ContentsFile->putStr(refer);
			}
			ContentsFile->putChar('"');  // leave off ending > to permit /> instead
		}

		// may add Eclipse <filter name="..." value="..."/> elements inside topic ****
	}
	else if (HelpType == 'J') {
		char *refer = NULL;
		char *pound = strrchr(ctp->ref, '#');
		long val = 0;
		char *cpt = NULL;

		if (pound) {
			refer = ctp->ref;
			if (cwf) {  // used as flag, NULL for list file processing
				XrefFile = NULL;
				ProcExtRef(pound);
				refer = NewName(XrefFile, strlen(XrefFile) + strlen(ctp->ref) + 1);
				strcat(refer, ctp->ref);
			}
		}

		if (ContentsLevel >= ctp->level) {
			ContentsFile->putStr(" />");  // close last real item
			ContentsFile->putCr();
			while (ContentsLevel > ctp->level) {
				ContentsLevel--;
				WriteContentsSpaces();
				ContentsFile->putStr("</tocitem>");
				ContentsFile->putCr();
				ContentsFile->putCr();
			}
		}
		else if (ContentsLevel > 0) {
			ContentsFile->putStr(" >");  // close last real item
			ContentsFile->putCr();
			while (ctp->level > ContentsLevel) {
				ContentsLevel++;
				if (ctp->level > ContentsLevel) {
					WriteContentsSpaces();
					ContentsFile->putStr("<tocitem>");
					ContentsFile->putCr();
				}
			}
		}
		else {// just starting, nothing to close
			ContentsLevel = 1;
			HomeID = NewName(pound ? pound + 1 : ctp->ref);
			HomeURL = NewName(refer);
		}

		WriteContentsSpaces();
		ContentsFile->putStr("<tocitem text=\"");
		if (UTF8 || NumericCharRefs || NoEnc)
			WriteContentsAttr(ContentsFile, ctp->title);
		else
			WriteIndexTitle(ContentsFile, ctp->title);
		ContentsFile->putStr("\" target=\"");
		if (pound) {
			ContentsFile->putStr(pound + 1);
			if (NoANames
			 || ctp->noref)
				*(strrchr(refer, '#')) = '\0';
			WriteMapFileItem(pound + 1, refer);
		}
		else
			ContentsFile->putStr(ctp->ref);
		ContentsFile->putChar('"');  // leave off ending

		if (val = ExpandLevel.find(ContentsLevel)) {
			ContentsFile->putStr(" expand=\"");
			if (val == 1)
				ContentsFile->putStr("false\"");
			else if (val == 2)
				ContentsFile->putStr("true\"");
		}
		if (cpt = ImageLevel.find(ContentsLevel)) {
			ContentsFile->putStr(" image=\"");
			ContentsFile->putStr(cpt);
			ContentsFile->putStr("\"");
			MapJHImage(cpt);
		}
	}
	else if (HelpType == 'C') {
		if (ContentsFileStarting)
			ContentsFileStarting = false;
		else {
			ContentsFile->putChar(',');
			ContentsFile->putCr();
		}
		ContentsFile->putChar('[');
		char *lev = NewName(NumStrMax + 4);
		sprintf(lev, "%d,\"", ctp->level);
		ContentsFile->putStr(lev);
		DeleteName(lev);
		ContentsFile->putStr(FixOHString((unc *) ctp->title));
		ContentsFile->putStr("\",\"");
		if (*(ctp->ref) == '*')   // merge entry
			MergeLocList.addnew(ctp->ref + 1, ContentsID);
		else if (cwf  // used as flag, NULL for list file processing
		 && (*(ctp->ref) != '!')) {
			XrefFile = NULL;
			ProcExtRef(ctp->ref);
			ContentsFile->putStr(XrefFile);
		}
		if (NoANames
		 || ctp->noref) {
			char *pound = strrchr(ctp->ref, '#');
			if (pound)
				*pound = '\0';
		}
		ContentsFile->putStr(ctp->ref);
		ContentsFile->putStr("\"]");
	}
	ContentsID++;
}


void
HTMwr::WriteContentsAttr(DCwfile *wf, char *txt)
{
	if (!txt)
		return;

	unc ch = (unc) *txt;

	while (ch) {
		switch (ch) {
			case '"':
				wf->putStr("&quot;");
				break;
			default:
				wf->putChar(ch);
				break;
		}
		ch = (unc) *++txt;
	}
}


void 
HTMwr::WriteSearchItem(DCwfile *cwf, char *cp, long id)
{
	static bool starting = true;

	if ((cp == NULL) || (id == 0)) {
		starting = true;
		return;
	}

	if (starting)
		starting = false;
	else {
		SearchFile->putChar(',');
		SearchFile->putCr();
	}
	SearchFile->putStr("[\"");
	SearchFile->putStr(cp);
	SearchFile->putStr("\", [");

	DClist *idl = (DClist *) id;
	idl->writeall(cwf, WriteSearchVal);

	SearchFile->putStr("]]");
}


void 
HTMwr::WriteSearchVal(DCwfile *cwf, long id)
{
	static bool starting = true;

	if (id == 0) {
		starting = true;
		return;
	}

	if (starting)
		starting = false;
	else
		SearchFile->putChar(',');
	char *val = NewName(NumStrMax);
	sprintf(val, "%d", id - 1);
	SearchFile->putStr(val);
	DeleteName(val);
}


// index items

char HTMwr::IndexSortType = 'N';
char *HTMwr::IndexSortLocale = "";
bool HTMwr::IndexRanges = true;

unc HTMwr::AlphaSortChars[AlphaSortCharMax] = {
 0x80, 0x81, 0x82, 0x83, 0x84, 0x85, 0x86, 0x87,
 0x88, 0x89, 'S', 0x8B, 0x8C, 'O', 'Z', 0x8F,
 0x90, 0x91, 0x92, 0x93, 0x94, 0x95, 0x96, 0x97,
 0x98, 0x99, 's', 0x9B, 0x9C, 'o', 'z', 'Y',
 0xA0, 0xA1, 0xA2, 0xA3, 0xA4, 0xA5, 0xA6, 0xA7,
 0xA8, 0xA9, 'a', 0xAB, 0xAC, 0xAD, 0xAE, 0xAF,
 0xB0, 0xB1, 0xB2, 0xB3, 0xB4, 0xB5, 0xB6, 0xB7,
 0xB8, 0xB9, 'o', 0xBB, 0xBC, 0xBD, 0xBE, 0xBF,
 'A', 'A', 'A', 'A', 'A', 'A', 'A', 'C',
 'E', 'E', 'E', 'E', 'I', 'I', 'I', 'I',
 0xD0, 'N', 'O', 'O', 'O', 'O', 'O', 0xD7,
 'O', 'U', 'U', 'U', 'U', 'Y', 0xDE, 's',
 'a', 'a', 'a', 'a', 'a', 'a', 'a', 'c',
 'e', 'e', 'e', 'e', 'i', 'i', 'i', 'i',
 0xF0, 'n', 'o', 'o', 'o', 'o', 'o', 0xF7,
 'o', 'u', 'u', 'u', 'u', 'y', 0xFE, 'y'
};


void
HTMwr::SortIndexRefs(void)
{
	// sort index refs
	IndexRefs = new (void *[IndexRefCount + 1]);
	IndexRefs[IndexRefCount] = NULL;  // guard entry
	if (IndexSortType == 'L')
		setlocale(LC_ALL, IndexSortLocale);
	IndexRefList.all(SetIndexArray);
	qsort(IndexRefs, (size_t) IndexRefCount, sizeof(void *), CompIdxNames);
}


void
HTMwr::SetIndexArray(void *vp, long id)
{
	if ((id == 0)
	 || (vp == NULL))
		return;

	IndexRefs[id - 1] = vp;

	ixprop *ip = (ixprop *) vp;

	if (ip->sort) {
		char *cpt = ip->sort;
		char *npt = NewName(strlen(ip->sort) * 4);
		char *lt = cpt;
		char *ft = npt;
		unl uch = 0;
		while (*cpt) {
			lt = cpt;
			if (*cpt < 0) {
				uch =  ::UTF8((unc **) &cpt);
				if (uch == 0)  // not unicode
					cpt++;
				else if (uch <= 0x7F)
					*npt++ = (char) (uch & 0x7F);
				else {
					while (lt < cpt)
						*npt++ = *lt++;
				}	
			}
			else
				*npt++ = *cpt++;
		}
		ip->sort = ft;
#if 0
		if (IndexSortType == 'L') {
			size_t len = strxfrm(NULL, ip->sort, 0);
			char *tmp = NewName(len);
			strxfrm(tmp, ip->sort, len + 1);
			ip->sort = tmp;
		}
		else 
#endif
		if (IndexSortType == 'A') {
			for (unc *cpt = (unc *) ip->sort; *cpt ; cpt++) {
				if (*cpt >= 0x80)
					*cpt = AlphaSortChars[*cpt - 0x80];
			}
		}
	}

	if (!ip->ref)
		return;

	XrefFile = NULL;
	ProcExtRef(ip->ref);

	if (KeywordRefs == 'F')
		ip->ref = NewName(XrefFile);
	else if (XrefFile) {
		char *full = NewName(XrefFile, strlen(XrefFile) + strlen(ip->ref) + 1);
		strcat(full, ip->ref);
		ip->ref = full;
	}
}

int
HTMwr::CompIdxNames(const void *p1, const void *p2)
{
	ixprop *ip1 = *(ixprop **)p1;
	ixprop *ip2 = *(ixprop **)p2;

	int rval = 0;
	char *s1 = _strlwr(_strdup(ip1->sort));
	char *xs1 = s1;
	char *s2 = _strlwr(_strdup(ip2->sort));
	char *xs2 = s2;
	char lm = 0;

	for ( ; (*s1 || *s2) ; s1++, s2++) {
		if (lm == IndexLevMark) { // skip next char
			lm = 0;
			continue;
		}
		if (*s1 < *s2) {
			rval = -1;
			break;
		}
		if (*s1 > *s2) {
			rval = 1;
			break;
		}
		lm = *s1;
	}

	free(xs1);
	free(xs2);

	//int rval = _stricmp(ip1->sort, ip2->sort);

	if (IndexSortType == 'L') {
		if (CaseSensitiveIndexCompare)
			rval = strcoll(ip1->sort, ip2->sort);
		else
			rval = _stricoll(ip1->sort, ip2->sort);

		if ((rval == 0)
		 && (ip1->title != NULL)
		 && (ip2->title != NULL)) {
			if (CaseSensitiveIndexCompare)
				rval = strcoll(ip1->title, ip2->title);
			else
				rval = _stricoll(ip1->title, ip2->title);
		}
	}
	else {
		if ((rval == 0)  // matched case-insensitive
		 && CaseSensitiveIndexCompare
		 && (*(ip1->sort) != *(ip2->sort)))  // must be case difference
			rval = (*(ip1->sort) < *(ip2->sort)) ? -1 : 1;  // put UC first

		if ((rval == 0)
		 && (ip1->title != NULL)
		 && (ip2->title != NULL))
			rval = _stricmp(ip1->title, ip2->title);
	}

	return rval;
}


void
HTMwr::WriteIndexRefs(void)
{
	// scan sorted list and coalesce matching case-insensitive entries
	long pos = 0;
	while ((pos = ScanIndexRefs(pos, IndexRefCount, 0, 1)) != 0) {
		if (pos == IndexRefCount)
			break;
	}

	// clean up
	ixprop **ipp = (ixprop **) IndexRefs;
	while (*ipp) {
		delete *ipp;
		ipp++;
	}

	delete [] IndexRefs;
}


long
HTMwr::ScanIndexRefs(long start, long end, long levoff, long levnum)
{
	// called recursively for IndexRefs from index start to end
	// initial call from WriteIndexRefs has levoff == 0 and levnum == 1
	// returns index for start of next invocation on same level

	// level breaks may be at:
	//   comma only if matching entry and UseCommaAsSeparator
	//   Frame level break, forced if not CombineIndexLevels, else if match

	ixprop *fip = (ixprop *) IndexRefs[start];
	if (!fip)
		return 0;  // terminates top-level calls

	// determine first char to be part of level match
	char *first = SkipLevMark(fip->name, &levoff);
	char *last = NULL;

	// determine length to check for level match
	long len = strlen(first);
	long cklen = GetLevLen(first);

	// find how many IndexRefs match for current level
	long pos = start;
	while (IndexRefs[++pos]) {
		if (pos >= end) // past last possible match
			break;
		last = ((ixprop *)IndexRefs[pos])->name + levoff;
		if (_strnicmp(first, last, cklen))
			break;
		if (last[cklen]
		 && (last[cklen] != IndexLevMark))
			break;
		if ((first[cklen] == IndexLevMark)
		 && !UseCommaAsSeparator
		 && (first[cklen + 1] != last[cklen + 1])
		 && ((first[cklen + 1] == ';')
		  || (last[cklen + 1] == ';')))
			break;
	}
	long next = IndexRefs[pos] ? pos : IndexRefCount;
	pos--; // back up to last match

	long suboff = levoff + cklen;
	long sublen = 0;
	long flen = suboff;

	if (CombineIndexLevels) {
		if (pos > start) {
			// check any more levels after this one in last item found for longest possible match
			char *firstsub = NULL;
			char *lastsub = NULL;
			for (;;) {
				suboff += sublen;
				flen = suboff;
				firstsub = SkipLevMark(fip->name, &suboff);
				if ((sublen = GetLevLen(firstsub)) == 0)
					break;
				lastsub = ((ixprop *)IndexRefs[pos])->name + suboff;  // last match
				if (_strnicmp(firstsub, lastsub, sublen + 1))
					break;
			}
		}
		else { // just this entry, use it all
			suboff = strlen(first) + levoff;
			flen = suboff;
		}
	}
	else {
		SkipLevMark(fip->name, &suboff);
	}

	long tlen = suboff - levoff;  // includes ending levmark (flen does not)

#if 0
	// sync sort and name
	long nlevoff = levoff;
	long ntlen = tlen;
	long i = 0;
	long j = 0;
	bool err = false;
	bool startfix = true;
	char sch = '\0';
	char nch = '\0';

	// fix differences between sort and name to show here ***
	while (i < (levoff + tlen)) {
		if (i == levoff)
			nlevoff = j;
		if (!fip->sort[i])
			break;
		if (!fip->name[j]) { // no more name left
			if (nlevoff > j)
				nlevoff = j;
			if (fip->sort[i] != IndexLevMark)
				err = true;  // not in sync
			break;
		}
		while ((fip->name[j] != '\0')
		 && ((startfix
		   && IgnoreLeadingCharsIX
		   && strchr(IgnoreLeadingCharsIX, fip->name[j]))
		  || (IgnoreCharsIX
		   && strchr(IgnoreCharsIX, fip->name[j]))))
			j++;

		startfix = false;
		sch = fip->sort[i];
		if (islower(sch))
			sch = _toupper(sch);

		nch = fip->name[j];
		if (islower(nch))
			nch = _toupper(nch);

		if ((sch == IndexLevMark)
		 && (nch == IndexLevMark)) {
			if (i > ((levoff + tlen) - 5))
				break;
			if (((sch = fip->sort[i + 1]) != ':')
			 && (sch != ','))
				i++;  // allow for see-also sort indicator
			i += 2;
			j += 2;
		}
		else if (sch == IndexLevMark) { // must be see-also
			i += 3;
			if (nch != ' ')
				j++;
		}
		else if ((sch == nch)
		 || ((sch == '~')
		  && (nch == ' ')))
			i++, j++;
		else if ((sch == ' ')
		 || (sch == ',')
		 || (sch == '~'))
			i++;
		else {
			assert(0);
			err = true;
			break;
		}
	}
	if (!err)
		ntlen = j - nlevoff;
	else {
		LogEvent(logwarn, 1, "Index sort problem with ", fip->name,
			                   " at ", fip->ref);
		assert(0);
	}

	if (ntlen < 0) {
		ntlen = 0;
		nlevoff = j;
	}
	if (ntlen
	 && (fip->name[nlevoff] == ' '))
		nlevoff++, ntlen--;
#endif

	// set up struct to write index entry
	ixitem *ip = new ixitem;
	ip->self = NULL;
	ip->reflist = NULL;
	ip->level = levnum;
	ip->sort = (levnum == 1) ? fip->sort : NULL;
	ip->title = NewName(fip->name + levoff, tlen);

	char *tptr = ip->title;
	char *nptr = fip->name + levoff;
	char *levmark = ip->title;
	long lmcnt = 0;
	for (long i = 0; i < tlen; i++) {
		if (nptr[i] == IndexLevMark) {
			lmcnt++;
			levmark = tptr - 1;
			switch (nptr[++i]) {
				case ':':
				case ',':
				case ';':
					*tptr++ = ',';
					break;

				case '.':
					*tptr++ = '.';
					break;

				case ' ':
				default:
					break;
			}
		}
		else
			*tptr++ = nptr[i];
	}
	
	while (tptr > levmark) {
		if ((*tptr == ' ')
		 || (*tptr == ':')
		 || (*tptr == ','))
			tptr--;
		else
			break;
	}
	if (tptr < ((ip->title + tlen) - lmcnt))
		tptr++;
	*tptr = '\0';
	long selflen = tptr - ip->title;	

	// now look for full matches to combine into one entry
	char *cptr = NULL;

	if (!first[tlen]) {  // this level has refs (or SeeAlsos)
		ip->reflist = new DClist;
		ip->reflist->add(start + 1); // offset by 1 to avoid valid 0 value
		pos = start + 1;
		while (pos < next) {
			cptr = ((ixprop *)IndexRefs[pos])->name + levoff;
			if (_stricmp(first, cptr)
			 || (CaseSensitiveIndexCompare
			  && (*first != *cptr)))
				break;
			ip->reflist->add(pos + 1);
			pos++;
		}
	}
	else {
		pos = start;
		ip->self = NewName(fip->name, levoff + selflen);
	}

	// we've made the most complete entry for this level, write it
	WriteIndexItem(ip, start);
	DeleteName(ip->title);
	if (ip->self)
		DeleteName(ip->self);
	delete ip;

	if (pos < next) { // pos is at first entry to proc for next level
		// recurse to process next lower level
		while ((pos = ScanIndexRefs(pos, next, flen, levnum + 1)) != 0)
			if (pos >= next)
				break;
	}

	// done with all lower levels, go back to do next on this level
	return next;
}


char *
HTMwr::SkipLevMark(char *first, long *levpt)
{
	first += *levpt;
	if (*first == IndexLevMark) {  // skip sep and its punct
		if ((*++first == ',')
		 || (*first == ':')
		 || (*(first + 1) == '\0')) { // not See
			(*levpt) += 2;
			first++;
		}
		else {  // See has sort selector too
			(*levpt) += 3;
			first += 2;
		}
		if (*first == ' ') {  // remove *one* leading space
			(*levpt)++;
			first++;
		}
	}
	return first;
}


long
HTMwr::GetLevLen(char *first)
{
	char *levmk = strchr(first, IndexLevMark);
	if (!LevelBreakForSee) {
		while (levmk) {
			if ((*(levmk + 1) == ':')
			 || (*(levmk + 1) == ','))
				break;
			levmk = strchr(levmk + 1, IndexLevMark);
		}
	}
	return (levmk ? (levmk - first) : strlen(first));
}


char *HTMwr::CurrIdxTitle = NULL;
char *HTMwr::SeeTerm = "See";
char *HTMwr::SeeAlsoTerm = "See also";
bool HTMwr::SortSeeAlsoFirst = false;
bool HTMwr::UseSortString = false;
char HTMwr::CurrOHLetter = 0;

void
HTMwr::WriteIndexTitle(DCwfile *wf, char *txt)
{
	if (!txt)
		return;

	if (UseCodePage) {
		if (DCicu::CodePage == 932) {  // japanese
			while (*txt) {
				if ((*txt > 0x7F)
				 || (*txt < 0)) {
					wf->putChar(*txt++);
					wf->putChar(*txt++);
				}
				else if (*txt == ',') {
					wf->putChar(0x81);
					wf->putChar(0x41);
					txt++;
				}
				else
					wf->putChar(*txt++);
			}
		}
		else
			wf->putStr(txt);
		return;
	}
	char *str = FixOHString((unc *) txt);
	wf->putStr(str);
	DeleteName(str);

#if 0
	unc ch = (unc) *txt;
	char str[6];

	while (ch) {
		switch (ch) {
			//case '&':  // already entity, don't double-code
			//	wf->putStr("&amp;");
			//	break;
			case '<':
				wf->putStr("&lt;");
				break;
			case '>':
				wf->putStr("&gt;");
				break;
			case '"':
				wf->putStr("&quot;");
				break;
			default:
				if (ch > 0x7F) {
					sprintf(str, "&#%0.3hd;", ((short) ch) & 0xFF);
					wf->putStr(str);
				}
				else
					wf->putChar(ch);
				break;
		}
		ch = (unc) *++txt;
	}
#endif
}


void
HTMwr::WriteIndexItem(ixitem *ip, long pos)
{
	static bool lastsub = false;
	static long lastpos = -1;

	if (HelpType == 'M') {
		while (IndexLevel > ip->level) {
			IndexLevel--;
			WriteIndexTabs();
			IndexFile->putStr("</ul>");
			IndexFile->putCr();
		}
		while (ip->level > IndexLevel) {
			WriteIndexTabs();
			IndexFile->putStr("<ul>");
			IndexFile->putCr();
			IndexLevel++;
		}

		WriteIndexTabs();
		IndexFile->putStr("<li> <object type=\"text/sitemap\">");
		IndexFile->putCr();

		WriteIndexItemParam("Name", CurrIdxTitle = ip->title);

		if (ip->self)  // used for heads without refs before sublevel heads
			WriteIndexItemParam("See Also", ip->self);
		else if (ip->reflist) {   //list of positions (+ 1) in IndexRefs for ref items
			IndexItemRefCounter = 0;
			if (ip->reflist->count() > 128)
				WriteIndexItemRef(ip->reflist->next->id);  // just first one
			else
				ip->reflist->all(WriteIndexItemRef);
		}

		WriteIndexTabs();
		IndexFile->putStr("\t</object>");
		IndexFile->putCr();
	}
	else if (HelpType == 'E') {
		if (IndexLevel >= ip->level) {
			WriteIndexSpaces();
			IndexFile->putStr("</entry>");  // close last real item
			IndexFile->putCr();
			while (IndexLevel > ip->level) {
				IndexLevel--;
				WriteIndexSpaces();
				IndexFile->putStr("</entry>");
				IndexFile->putCr();
				IndexFile->putCr();
			}
		}
		else if (IndexLevel > 0) {
			while (ip->level > IndexLevel) {
				IndexLevel++;
				if (ip->level > IndexLevel) {  // only if level skipped
					WriteIndexSpaces();
					IndexFile->putStr("<entry>");
					IndexFile->putCr();
				}
			}
		}
		else // just starting, nothing to close
			IndexLevel = 1;

		WriteIndexSpaces();
		IndexFile->putStr("<entry keyword=\"");
		WriteIndexTitle(IndexFile, ip->title);

		// may add Eclipse filter="name=value" attribute to entry ****

		IndexFile->putStr("\">");  // close last item
		IndexFile->putCr();

		// may add Eclipse <filter name="..." value="..."/> elements inside entry ****

		if ((!ip->self)
		 && ip->reflist) {
			DClist *rlst = NULL;
			ixprop *ixp = NULL;
			char *ref = NULL;
			char *label = NULL;
			char *pound = NULL;
			for (rlst = ip->reflist->next; rlst != NULL; rlst = rlst->next) {
				ixp = (ixprop *) IndexRefs[rlst->id - 1];
				ref = (char *) ixp->ref;
				label = (char *) ixp->title;
				if (ref) {
					WriteIndexSpaces();
					IndexFile->putStr("   <topic href=\"");
					pound = strrchr(ref, '#');
					if (NoANames)
						*pound = '\0';
					if (TocIdxFilePrefix && *TocIdxFilePrefix)
						IndexFile->putStr(TocIdxFilePrefix);
					IndexFile->putStr(ref);
					if ((*EclipseVer > '1')
					 && label
					 && *label) {
						IndexFile->putStr("\" label=\"");
						WriteIndexTitle(IndexFile, label);
					}
					IndexFile->putStr("\"/>");  // close topic entry
					IndexFile->putCr();
				}
			}
		}
	}
	else if (HelpType == 'J') {
		if (IndexLevel >= ip->level) {
			if ((OracleHelp && UseIndexentryTag) || lastsub) {
				WriteIndexSpaces();
				IndexFile->putStr("</indexitem>");
				lastsub = false;
			}
			else
				IndexFile->putStr(" />");  // close last real item
			IndexFile->putCr();
			while (IndexLevel > ip->level) {
				IndexLevel--;
				WriteIndexSpaces();
				IndexFile->putStr("</indexitem>");
				IndexFile->putCr();
				IndexFile->putCr();
			}
		}
		else if (IndexLevel > 0) {
			if (lastsub)
				lastsub = false;
			else if (!(OracleHelp && UseIndexentryTag)) {
				IndexFile->putStr(" >");  // close last real item
				IndexFile->putCr();
			}
			while (ip->level > IndexLevel) {
				IndexLevel++;
				if (ip->level > IndexLevel) {  // only if level skipped
					WriteIndexSpaces();
					IndexFile->putStr("<indexitem>");
					IndexFile->putCr();
				}
			}
		}
		else // just starting, nothing to close
			IndexLevel = 1;

		WriteIndexSpaces();
		IndexFile->putStr("<indexitem text=\"");
		WriteIndexTitle(IndexFile, ip->title);

		bool sublist = (ip->reflist && (((DClist *) (ip->reflist))->count() > 1)) ? true : false;
		if ((OracleHelp && UseIndexentryTag)
		 || sublist) {
			IndexFile->putStr("\">");  // close last item
			IndexFile->putCr();
		}
		else
			IndexFile->putChar('"');

		if ((!ip->self)
		 && ip->reflist) {
			DClist *rlst = ip->reflist->next;
			ixprop *ixp = NULL;
			char *ref = NULL;
			char *pound = NULL;
			while (rlst) {
				ixp = (ixprop *) IndexRefs[rlst->id - 1];
				ref = (char *) ixp->ref;
				if (ref) {
					if (OracleHelp && UseIndexentryTag) {
						WriteIndexSpaces();
						IndexFile->putStr("  <indexentry text=\"");
						WriteIndexTitle(IndexFile, ixp->title);
						IndexFile->putChar('"');
					}
					else if (sublist) {
						WriteIndexSpaces();
						IndexFile->putStr("  <indexitem text=\"  ");
						WriteIndexTitle(IndexFile, ixp->title);
						IndexFile->putChar('"');
					}
					IndexFile->putStr(" target=\"");
					pound = strrchr(ref, '#');
					if (pound) {
						IndexFile->putStr(pound + 1);
						if (NoANames)
							*pound = '\0';
						WriteMapFileItem(pound + 1, ref);
					}
					else
						IndexFile->putStr(ref);
					if (OracleHelp || sublist) {
						IndexFile->putStr("\" />");  // close last entry
						IndexFile->putCr();
					}
					else
						IndexFile->putChar('"');  // leave off ending
					if (sublist)
						lastsub = true;
				}
				if (rlst->next == NULL)
					break;
				rlst = rlst->next;
				if (!(OracleHelp && UseIndexentryTag)
				 && !sublist) {
					IndexFile->putStr(" />");  // close last item
					IndexFile->putCr();
					WriteIndexSpaces();
					IndexFile->putStr("<indexitem text=\"");
					WriteIndexTitle(IndexFile, ip->title);  // repeat name for next, yuck
					IndexFile->putStr("\"");
				}
			}
		}
	}
	else if (HelpType == 'C') {
		if (IndexFileStarting)
			IndexFileStarting = false;
		else {
			IndexFile->putChar(',');
			IndexFile->putCr();
		}
		if (ip->sort) {
			char ch = ip->sort[0];
			if (ch < 0)  // not utf-8 any more, unless CJK...****
				ch = AlphaSortChars[((unc) ch) - 0x80];
			if (!isupper(ch)) {
				if (islower(ch))
					ch = _toupper(ch);
				else if (isdigit(ch))
					ch = '0';
				else ch = '!';
			}
			if (ch > CurrOHLetter) {
				CurrOHLetter = ch;
				IndexFile->putStr("[\"");
				IndexFile->putChar(ch);
				IndexFile->putStr("\"],");
				IndexFile->putCr();
			}
		}
		IndexFile->putStr("[");
		char *lev = NewName(NumStrMax + 4);
		sprintf(lev, "%d,\"", ip->level);
		IndexLevel = (uns) ip->level;
		IndexFile->putStr(lev);
		IndexFile->putStr(FixOHString((unc *) ip->title));
		IndexFile->putStr("\"]");
		if ((!ip->self)
		 && ip->reflist) {   //list of positions (+ 1) in IndexRefs for ref items
			IndexItemRefCounter = 0;
			if (ip->reflist->count() > 128)
				WriteIndexItemRef(ip->reflist->next->id);  // just first one
			else
				ip->reflist->all(WriteIndexItemRef);
		}
		DeleteName(lev);
	}

#if 0
	else if ((HelpType == 'S')
	 && UseIndex) {

		if (UseIndexLetters
		 && ip->sort)
			SetIndexLetter(ip->sort[0]);
		ChangeIndexLevel(ip->level);

		StartIndexEnt();

		// write ID for see/see-also targets
		if (pos > lastpos) {
			IndexFile->putStr("<a name=\"");
			IndexFile->putStr(IndexNumPrefix);
			IndexFile->putNum(pos);
			IndexFile->putStr("\"></a>");
			lastpos = pos;
		}

		IndexFile->putStr(FixOHString((unc *) ip->title));
		//IndexFile->putStr(ip->title);

		if (!ip->self
		 && ip->reflist) {   //list of positions (+ 1) in IndexRefs for ref items
			IndexItemRefCounter = 0;
			StartIndexRefs();

			if (ip->reflist->count() > 128)
				WriteIndexItemRef(ip->reflist->next->id);  // just first one
			else
				ip->reflist->all(WriteIndexItemRef);

			EndIndexRefs();
		}
		EndIndexEnt();

		IndexFile->putCr();
	}
#endif

}


void
HTMwr::SetIndexLetter(char ch)
{
	if (!isupper(ch)) {
		if (islower(ch))
			ch = _toupper(ch);
		else if (isdigit(ch))
			ch = '0';
		else ch = '!';
	}
	if (ch > CurrOHLetter) {
		ZeroIndexLevel();
		if (CurrOHLetter) {
			IndexFile->putStr("</li>");
			IndexFile->putCr();
		}
		IndexFile->putStr("<li>");

		char nch = CurrOHLetter;
		if ((nch == 0)
		 && (ch > '!'))
			PutIxId(nch = '!');
		if ((nch == '!')
		 && (ch > '0'))
			PutIxId(nch = '0');
		if ((nch == '0')
		 && (ch >= 'A'))
			nch = 'A' - 1;
		while (nch < ch)
			PutIxId(++nch);

		CurrOHLetter = ch;

		if ((ch == '!')
		 && IndexLetterSymbol)
			IndexFile->putStr(IndexLetterSymbol);
		else if ((ch == '0')
		 && IndexLetterNumber)
			IndexFile->putStr(IndexLetterNumber);
		else
			IndexFile->putChar(ch);
		IndexFile->putCr();
	}
}


void
HTMwr::PutIxId(char ch)
{
	IndexFile->putStr("<a name=\"");
	IndexFile->putStr(IndexLetPrefix);
	if (ch != '!')
		IndexFile->putChar(ch);
	IndexFile->putStr("\"></a>");
}


void
HTMwr::ChangeIndexLevel(long nlev)
{
	if (IndexLevel > nlev) {
		while (IndexLevel > nlev) {
			IndexLevel--;
			WriteIndexTabs();
			IndexFile->putStr("</ul>");
			IndexFile->putCr();
		}
	}
	else if (nlev > IndexLevel) {
		// always the case for the first entry
		while (nlev > IndexLevel) {
			WriteIndexTabs();
			IndexFile->putStr("<ul ");
			if (!IndexLevel
			 && !(UseListProps ? UseIXLets : UseIndexLetters))
				IndexFile->putStr("id=\"indextop\" ");
			IndexFile->putStr("class=\"");
			IndexFile->putStr(IndexLevelClass);
			IndexLevel++;
			if (UseIndexLevelNum)
				IndexFile->putNum(IndexLevel);
			IndexFile->putStr("\">");
			IndexFile->putCr();
			if (nlev > IndexLevel) {
				StartIndexEnt();
				IndexFile->putCr();
			}
		}
	}
}


void
HTMwr::StartIndexSubs(void)
{
	ChangeIndexLevel(IndexLevel + 1);
}


void
HTMwr::EndIndexSubs(void)
{
	if (IndexLevel)
		ChangeIndexLevel(IndexLevel - 1);
}


void
HTMwr::ZeroIndexLevel(void)
{
	if (IndexLevel)
		ChangeIndexLevel(0);
}


void
HTMwr::StartIndexEnt(void)
{
	if (!IndexLevel)  // ay start or after letter
		ChangeIndexLevel(IndexLevel + 1);
	WriteIndexTabs();
	IndexFile->putStr("<li>");
}


void
HTMwr::EndIndexEnt(void)
{
	WriteIndexTabs();
	IndexFile->putStr("</li>");
	IndexFile->putCr();
}


void
HTMwr::StartIndexRefs(void)
{
	IndexLevel++;
	IndexFile->putCr();
	WriteIndexTabs();
	IndexFile->putStr("<ul class=\"");
	IndexFile->putStr(IndexRefClass);
	if (UseIndexLevelNum)
		IndexFile->putNum(IndexLevel);
	IndexFile->putStr("\">");
	IndexFile->putCr();
}


void
HTMwr::StartIndexRef(void)
{
	WriteIndexTabs();
	IndexFile->putStr("<li>");
}


void
HTMwr::EndIndexRef(void)
{
	IndexFile->putStr("</li>");
	IndexFile->putCr();
}


void
HTMwr::EndIndexRefs(void)
{
	WriteIndexTabs();
	IndexFile->putStr("</ul>");
	IndexLevel--;
}


void
HTMwr::WriteIndexItemParam(char *name, char *val, bool lit)
{
	WriteIndexTabs();
	IndexFile->putStr("\t<param name=\"");
	IndexFile->putStr(name);
	IndexFile->putStr("\" value=\"");
	if (lit)
		IndexFile->putStr(val);
	else
		WriteIndexTitle(IndexFile, val);
	IndexFile->putStr("\">");
	IndexFile->putCr();
}


long HTMwr::IndexItemRefCounter = 0;


void
HTMwr::WriteIndexItemRef(long pos)
{
	if (pos <= 0)
		return;

	if (++IndexItemRefCounter > 128)
		return;

	// used for OmniHelp and HH only

	ixprop *sap = (ixprop *) IndexRefs[pos - 1];

	if (sap->ref) {
		if (HelpType == 'C') {
			if (IndexFileStarting)
				IndexFileStarting = false;
			else {
				IndexFile->putChar(',');
				IndexFile->putCr();
			}
			char *lev = NewName(NumStrMax + 4);
			sprintf(lev, "[%d,", IndexLevel + 1);
			IndexFile->putStr(lev);
			IndexFile->putStr("\"");
			IndexFile->putStr(sap->ref);
			IndexFile->putStr("\",");
			sprintf(lev, "%d]", sap->refid - 1);
			IndexFile->putStr(lev);
			DeleteName(lev);
		}
		else if (HelpType == 'M') {
			WriteIndexItemParam("Name", sap->title, true); // already escaped
			WriteIndexItemParam("Local", sap->ref);
			if (sap->win)
				WriteIndexItemParam("WindowName", sap->win, true);
		}

#if 0
		else if ((HelpType == 'S')
		 && UseIndex) {  // use topic titles as links, one per line
			WriteIndexTabs();
			IndexFile->putStr("<li><a href=\"");
			IndexFile->putStr(sap->ref);
			IndexFile->putStr("\">");
			IndexFile->putStr(sap->title);
			IndexFile->putStr("</a></li>");
			IndexFile->putCr();
		}
#endif

		return;
	}

	// must be See Also entry
	// try to find matching first-level entry in IndexRefs
	// stepping back one word at a time from end until at front
	// retaining the longest match found
	char *fname = sap->name;
	long spos = strlen(fname) - 1;
	bool see = false;
	long stlen = SeeTerm ? strlen(SeeTerm) : 0;
	long satlen = SeeAlsoTerm ? strlen(SeeAlsoTerm) : 0;
	char *rname = (HelpType == 'C') ? NULL : CurrIdxTitle; // if no entry found, make ref to self
	char *xname = NULL;
	long i = 0;
	long len = 0;
	long val = 0;
	char *chp = NULL;
	bool matched = false;
	char *seerefs[128];
	long refpos = 0;
	long seepos[128];
	long multi = 0;

	if (fname[spos] == '.')
		fname[spos--] = '\0';
	spos--;  // move back to include at least one character

	while (spos > 0) {
		while (spos > 0) {
			while ((spos > 0)
			 && (fname[spos] != ' ')
			 && (fname[spos] != ';'))
				spos--;
			if (spos == 0) {
				if (multi && rname)
					multi++;
				break; 
			}

			see = false;
			if ((stlen && !_strnicmp(SeeTerm, &fname[spos + 1], stlen)) // stop if at See or
			 || (satlen && !_strnicmp(SeeAlsoTerm, &fname[spos + 1], satlen))) { // See also
				see = true;
				if (multi && rname)
					multi++;
				break;
			}

			xname = NewName(&fname[spos + 1]);
			matched = false;
			len = strlen(xname);
			for (i = 0; i < IndexRefCount; i++) {
				if (i == (pos - 1))
					continue;
				chp = ((ixprop *) IndexRefs[i])->name;
				if (IdxNameMatch(chp, xname, len)) {
					matched = true;
					refpos = i;
					break;
				}
			}
			if (matched)
				rname = NewName(xname); // always use short form
			DeleteName(xname);
			xname = NULL;

			if (fname[spos] == ';') {
				multi++;
				break;
			}
			spos--;
		}

		if (multi && (multi < 128)) {
			seerefs[multi - 1] = rname;
			seepos[multi - 1] = refpos;
		}
		else {
			WriteSeeAlsoRef(rname, refpos);
			if (rname != CurrIdxTitle)
				DeleteName(rname);
		}
		rname = NULL;
		if (see)
			break;
		if (fname[spos] == ';') {
			fname[spos] = '\0';
			spos--;
		}
	}

	while (multi > 0) {
		if (seerefs[--multi]) {
			WriteSeeAlsoRef(rname = seerefs[multi], seepos[multi]);
			DeleteName(rname);
		}
	}
}

bool
HTMwr::IdxNameMatch(char *ixname, char *rname, long len)
{
	for (long i = 0, j = 0; j < len; i++, j++) {
		if (ixname[i] == IndexLevMark) {
			if (rname[j] == IndexLevMark) {
				i++, j++;
				if (((ixname[i] == ':')
				  || (ixname[i] == ','))
				 && ((rname[j] == ':')
				  || (rname[j] == ',')))
					continue;
			}
			else if (((ixname[++i] == ':')
			  || (ixname[i] == ','))
			 && ((rname[j] == ':')
			  || (rname[j] == ',')))
				continue;
			else
				return false;
		}
		else if (CaseSensitiveIndexCompare) {
			if (ixname[i] != rname[j])
				return false;
		}
		else if (tolower(ixname[i]) != tolower(rname[j]))
			return false;
	}
	return true;
}

void
HTMwr::WriteSeeAlsoRef(char *rname, long refpos)
{
	if (HelpType == 'C') {
		if (rname) {
			//if (IndexFileStarting)  // IX will never start with a See
			//	IndexFileStarting = false;
			//else {
				IndexFile->putChar(',');
				IndexFile->putCr();
			//}
			char *lev = NewName(NumStrMax + 4);
			sprintf(lev, "[%d,\"#", IndexLevel + 1);
			IndexFile->putStr(lev);
			IndexFile->putStr(rname);
			IndexFile->putStr("\",\"");
			IndexFile->putStr(rname);
			IndexFile->putStr("\"]");
			DeleteName(lev);
		}
	}
	else if (HelpType == 'M')
		WriteIndexItemParam("See Also", rname);

#if 0
	else if ((HelpType == 'S')
	 && UseIndex) {
		if (rname) {
			WriteIndexTabs();
			IndexFile->putStr("<li><a href=\"#");
			IndexFile->putStr(IndexNumPrefix);
			IndexFile->putNum(refpos);
			IndexFile->putStr("\">");
			IndexFile->putStr(rname);
			IndexFile->putStr("</a></li>");
			IndexFile->putCr();
		}
	}
#endif

}


void
HTMwr::WriteDCLIndexRefs(void)
{
	// write sorted list in dcl
	ixprop **ipp = (ixprop **) IndexRefs;
	while (*ipp) {
		WriteIndexDCL((DCLwfile *) IndexFile, *ipp);
		delete *ipp;
		ipp++;
	}

	delete [] IndexRefs;
}


// ALink file writing


void
HTMwr::SortALinkRefs(void)
{
	if (!ALinkCount)
		return;

	// sort ALink refs
	ALinkRefs = new (void *[ALinkCount + 1]);
	ALinkRefs[ALinkCount] = NULL;  // guard entry
	OracleALinkList.all(SetALinkArray);
	qsort(ALinkRefs, (size_t) ALinkCount, sizeof(void *), CompALinkNames); // Idx comp works
}


int
HTMwr::CompALinkNames(const void *p1, const void *p2)
{
	ixprop *ip1 = *(ixprop **)p1;
	ixprop *ip2 = *(ixprop **)p2;
	int rval = _stricmp(ip1->sort, ip2->sort);

	if ((rval == 0)
	 && (ip1->title != NULL)
	 && (ip2->title != NULL))
		rval = _stricmp(ip1->title, ip2->title);

	return rval;
}


void
HTMwr::SetALinkArray(void *vp, long id)
{
	if ((id == 0)
	 || (vp == NULL))
		return;

	ALinkRefs[id - 1] = vp;

	ixprop *ip = (ixprop *) vp;
	if (!ip->ref)
		return;

	XrefFile = NULL;
	ProcExtRef(ip->ref);

	//if (ALinkFileRefs == 'F')  // ref to file only, not para
	//	ip->ref = NewName(XrefFile);
	//else {
		char *full = NewName(XrefFile, strlen(XrefFile) + strlen(ip->ref) + 1);
		strcat(full, ip->ref);
		ip->ref = full;
	//}
}


void
HTMwr::WriteALinkRefs(void)
{
	if (!ALinkCount)
		return;

	// scan sorted list and coalesce matching case-insensitive entries
	long pos = 0;
	ixprop *ip = (ixprop *) ALinkRefs[0];
	char *currname = ip->name;
	char *pound = NULL;

	// <linkitem topic="link1">
	//  <linkentry target="file1" text="Item in the first link" />
	//  <linkentry target="file2" text="Another item in the first link" />
	//  <linkentry target="file3" text="Last item in the first link" />
	// </linkitem>

	// start first item
	ALinkFile->putStr(" <linkitem topic=\"");
	ALinkFile->putStr(currname);
	ALinkFile->putStr("\" >");
	ALinkFile->putCr();

	while (pos < ALinkCount) {
		ip = (ixprop *) ALinkRefs[pos];
		// if name different, end last item and start new one
		if (stricmp(currname, ip->name)) {
			ALinkFile->putStr(" </linkitem>");
			ALinkFile->putCr();
			ALinkFile->putCr();
			ALinkFile->putStr(" <linkitem topic=\"");
			ALinkFile->putStr(currname = ip->name);
			ALinkFile->putStr("\">");
			ALinkFile->putCr();
		}
		ALinkFile->putStr("  <linkentry target=\"");
		pound = strrchr(ip->ref, '#');
		if (pound) {
			ALinkFile->putStr(pound + 1);
			if (ALinkFileRefs == 'F')
				*pound = '\0';
			WriteMapFileItem(pound + 1, ip->ref);
		}
		else
			ALinkFile->putStr(ip->ref);
		ALinkFile->putStr("\" ");
		ALinkFile->putStr("text=\"");
		WriteIndexTitle(ALinkFile, ip->title); // works for ALinks
		ALinkFile->putStr("\" />");
		ALinkFile->putCr();
		pos++;
	}

	// end last item
	ALinkFile->putStr(" </linkitem>");
	ALinkFile->putCr();

	// clean up
	ixprop **ipp = (ixprop **) ALinkRefs;
	while (*ipp) {
		delete (*ipp)->sort;
		delete *ipp;
		ipp++;
	}

	delete [] ALinkRefs;
}


void
HTMwr::WriteDCLALinkRefs(void)
{
	if (!ALinkRefs)
		return;

	// write sorted list in dcl
	ixprop **ipp = (ixprop **) ALinkRefs;
	while (*ipp) {
		WriteIndexDCL((DCLwfile *) ALinkFile, *ipp); // same as for Index
		delete *ipp;
		ipp++;
	}

	delete [] ALinkRefs;
}


void 
HTMwr::WriteALinkItem(DCwfile *cwf, char *cp, long id)
{
	static bool starting = true;

	if ((cp == NULL) || (id == 0)){
		starting = true;
		return;
	}

	if (starting)
		starting = false;
	else {
		ALinkFile->putChar(',');
		ALinkFile->putCr();
	}
	ALinkFile->putStr("[\"");
	ALinkFile->putStr(cp);
	ALinkFile->putStr("\",[");

	DClist *idl = (DClist *) id;
	idl->writeall(cwf, WriteALinkVal);

	ALinkFile->putStr("]]");
}


void 
HTMwr::WriteALinkVal(DCwfile *cwf, long id)
{
	static bool starting = true;

	if (id == 0) {
		starting = true;
		return;
	}

	if (starting)
		starting = false;
	else 
		ALinkFile->putChar(',');
	char *val = NewName(NumStrMax);
	sprintf(val, "%d", id - 1);
	ALinkFile->putStr(val);
	DeleteName(val);
}


// glossary operations

bool HTMwr::GlossHere = false;
DCvlist HTMwr::GlossList;
DCnlist HTMwr::GlossParaList;
DCnlist HTMwr::GlossIDList;
long HTMwr::GlossAlloc = 512;


void
HTMwr::SetGlossTerm(HTMwfile *wf, char *term, char *id)
{
	if (!term || (*term == '\0'))
		return;

	wf->putAName(id + 1, NULL, ATagLineBreak);

	ixprop *gl = new ixprop;
	gl->name = term;
	gl->ref = id;
	gl->title = NewName(id + 1);
	GlossList.add(gl, ++GlossCount);
}


char *
HTMwr::MakeGlossID(char *term)
{
	if (!term)
		return NULL;

	short len = GlossSpace ? strlen(GlossSpace) : 1;
	char *tid = NewName((strlen(term) * len) + 1);
	char *id = term;
	char *tp = tid;
	while (*id != '\0') {
		if (*id > 0x7F)
			; // handle euro and russian here ****
		else if (isalnum(*id))
			*tp++ = *id;
		else if (*id == ' ') {
			if (GlossSpace) {
				strcat(tp, GlossSpace);
				tp += len;
			}
		}
		id++;
	}
	*tp = '\0';

	len = (GlossPrefix ? strlen(GlossPrefix) : 0)
		  + (GlossSuffix ? strlen(GlossSuffix) : 0)
		  + strlen(tid) + 1;

	id = NewName("#", len);
	if (GlossPrefix)
		strcat(id, GlossPrefix);
	strcat(id, tid);
	if (GlossSuffix)
		strcat(id, GlossSuffix);

	//DeleteName(tid);
	return id;
}


void
HTMwr::SortGlossRefs(void)
{
	GlossRefs = new (void *[GlossCount + 1]);
	GlossRefs[GlossCount] = NULL;  // guard entry
	if (IndexSortType == 'L')
		setlocale(LC_ALL, IndexSortLocale);
	GlossList.all(SetGlossArray);
	qsort(GlossRefs, (size_t) GlossCount, sizeof(void *), CompGlossNames);
}

int
HTMwr::CompGlossNames(const void *p1, const void *p2)
{
	// used for glossary sorts only
	return _stricmp((*(ixprop **)p1)->name, (*(ixprop **)p2)->name);
}

void
HTMwr::SetGlossArray(void *vp, long id)
{
	if (!vp || (id == 0))
		return;

	ixprop *ip = (ixprop *) vp;

	GlossRefs[id - 1] = vp;

	if (!ip->ref)
		return;

	XrefFile = NULL;
	ProcExtRef(ip->ref);

	if (XrefFile) {
		if (KeywordRefs == 'F')
			ip->ref = NewName(XrefFile);
		else {
			char *full = NewName(XrefFile, strlen(XrefFile) + strlen(ip->ref) + 1);
			strcat(full, ip->ref);
			ip->ref = full;
		}
	}

	mapprop *mp = new mapprop;
	mp->name = ip->title;
	mp->ref = ip->ref;
	MapNewlinkList.add(mp, ++NewlinkCount);
}


void
HTMwr::WriteGlossRefs(void)
{
	// all on same level, no nesting
	ixprop *ip = NULL;
	IndexLevel = 1;
	for (int i = 0; GlossRefs[i] != NULL; i++) {
		ip = (ixprop *) GlossRefs[i];
		GlossFile->putStr("<indexitem text=\"");
		WriteIndexTitle(GlossFile, ip->name);
		GlossFile->putStr("\" target=\"");
		GlossFile->putStr(ip->title);
		GlossFile->putStr("\" />");
		GlossFile->putCr();
	}

	// clean up
	ixprop **ipp = (ixprop **) GlossRefs;
	while (*ipp) {
		delete *ipp;
		ipp++;
	}

	delete [] GlossRefs;
}


void
HTMwr::WriteDCLGlossRefs(void)
{
	// write sorted list in dcl
	ixprop **ipp = (ixprop **) GlossRefs;
	while (*ipp) {
		WriteIndexDCL((DCLwfile *) GlossFile, *ipp);
		delete *ipp;
		ipp++;
	}

	delete [] GlossRefs;
}



// map file processing

DCnlist *HTMwr::MapItemList;
long HTMwr::MapItemCount = 0;
char *HTMwr::MapFilePrefix = NULL;


void
HTMwr::WriteMapFileItem(char *id, char *url, char *window)
{
	if (MapItemList->getnum(id))
		return;
	MapItemList->addnew(id, ++MapItemCount);

	MapFile->putStr("<mapID target=\"");
	MapFile->putStr(id);
	MapFile->putStr("\" url=\"");
	if (MapFilePrefix)
		MapFile->putStr(MapFilePrefix);
	MapFile->putStr(url);
	if (window) {
		MapFile->putStr("\" wintype=\"");
		MapFile->putStr(window);
	}
	MapFile->putStr("\" />");
	MapFile->putCr();
}


void 
HTMwr::WriteMapRef(void *vp, long i)
{
	if (vp == NULL)
		return;

	mapprop *mp = (mapprop *) vp;
	WriteMapFileItem(mp->name, mp->ref, mp->win);
}



// utilities


void
HTMwr::IncContentsLevel(void)
{
	if (ContentsLevEmpty)
		ContentsLevSkip.add(ContentsLevel);
	else {
		WriteContentsTabs();
		ContentsFile->putStr("<ul>");
		ContentsFile->putCr();
		ContentsTabLev++;
	}
	ContentsLevel++;
	ContentsLevEmpty = true;
}


void
HTMwr::DecContentsLevel(void)
{
	ContentsLevel--;
	if (ContentsLevel
	 && ContentsLevSkip.find(ContentsLevel))
		ContentsLevSkip.remove(ContentsLevel);
	else {
		ContentsTabLev--;
		WriteContentsTabs();
		ContentsFile->putStr("</ul>");
		ContentsFile->putCr();
	}
}


void
HTMwr::WriteContentsTabs(void)
{
	uns i;

	for (i = 0; i < ContentsTabLev; i++)
		ContentsFile->putChar('\t');
}


void
HTMwr::WriteContentsSpaces(void)
{
	uns i;

	for (i = 1; i < ContentsLevel; i++)
		ContentsFile->putStr("   ");
}



void
HTMwr::WriteIndexTabs(void)
{
	uns i;

	for (i = 0; i < IndexLevel; i++)
		IndexFile->putChar('\t');
}

void
HTMwr::WriteIndexSpaces(void)
{
	uns i;

	for (i = 1; i < IndexLevel; i++)
		IndexFile->putStr("   ");
}


// Alias file writing

void
HTMwr::WriteAliasFullFile(void)
{
	if (OmniHelp) {
		AliasFile->putStr("var cshItems = [");
		AliasFile->putCr();
		WriteAlias(AliasFile, NULL);
	}
	AliasList.writeall(AliasFile, WriteAlias);
	if (OmniHelp) {
		AliasFile->putChar(']');
		AliasFile->putCr();
	}
}


void
HTMwr::WriteAlias(DCfile *mwf, void *vp)
{
	static bool starting = true;

	if (vp == NULL) {
		starting = true;
		return;
	}

	mapprop *mp = (mapprop *) vp;
	DCwfile *wf = (DCwfile *) mwf;

	if (OmniHelp) {
		if (starting)
			starting = false;
		else {
			wf->putChar(',');
			wf->putCr();
		}
		wf->putStr("[\"");
		wf->putStr(mp->name);
		wf->putStr("\",\"");
		wf->putStr(mp->ref);
		wf->putStr("\"]");
	}
	else {
		wf->putStr(mp->name);
		wf->putChar('=');
		if (AliasFileStarting) {
			if ((HelpType == 'M')
			 && UseAliasAName) {
				char *pound = strrchr(mp->ref, '#');
				if (pound)
					*pound = '\0';
			}
			AliasFileStarting = false;
		}
		wf->putStr(mp->ref);
		wf->putCr();
		if (AliasTitle) {
			AliasTitleFile->putStr(mp->name);
			AliasTitleFile->putStr(" \"");
			AliasTitleFile->putStr(mp->win ? mp->win : "NONE" );
			AliasTitleFile->putChar('"');
			AliasTitleFile->putCr();
		}
	}
}


// CSH Map file writing


void
HTMwr::WriteCSHMapFile(char *projname)
{
	if (!MakeCshMapFile
	 || !CshMapFile
	 || !AliasCount)
		return;

	// use AliasList and CshMapFile name
	// start with header, first number is CshMapFileNumStart
	// for each alias file item, increment by CshMapFileNumIncrement
  // #define name number

	DCwfile *cshmap = new DCwfile(CshMapFile, ftxt);
	if (!cshmap
	 || (cshmap->FErr() != fok)) {
		LogEvent(logwarn, 1, "Cannot write CSH map file: ", CshMapFile);
		return;
	}
	cshmap->putStr("/* ");
	cshmap->putStr(CshMapFile);
	cshmap->putStr(" is the list of Help IDs used in ");
	cshmap->putStr(projname);
	cshmap->putStr(" */");
	cshmap->putCr();
	cshmap->putCr();

	unl aliasnum = CshMapFileNumStart;
	DCvlist *dv = &AliasList;
	while ((dv = dv->next) != NULL) {
		cshmap->putStr("#define ");
		cshmap->putStr(((mapprop *)(dv->vp))->name);
		cshmap->putChar(' ');
		cshmap->putILong(aliasnum);
		cshmap->putCr();
		aliasnum += CshMapFileNumIncrement;
	}
	cshmap->putCr();
	cshmap->CloseFile();
	delete cshmap;
}



// DCL file writing


void
HTMwr::WriteContentsDCL(DCfile *cwf, void *ip)
{
	if (ip == NULL)
		return;

	cntprop *ctp = (cntprop *) ip;
	DCLwfile *wf = (DCLwfile *) cwf;

	// write contents entries as DCL groups
	DCctl *gp = new DCctl(group, 2, 110, 2, 0);
	wf->putCtl(*gp);

	DCctl *cp = new DCctl(ilong, 2, 110, 2, ctp->level);
	wf->putCtl(*cp);

	cp->dt(ishort);
	cp->siz((long) ctp->noref);
	wf->putCtl(*cp);

	cp->dt(etext);
	cp->siz((long) 0);
	cp->ex(ctp->title);
	wf->putCtl(*cp);

	cp->dt(ename);
	cp->siz((long) 0);
	if ((*(ctp->ref) != '*')
	 && (*(ctp->ref) != '\0')
	 && (*(ctp->ref) != '!')) {
		XrefFile = NULL;
		ProcExtRef(ctp->ref);
		if (XrefFile) {
			char *full = NewName(XrefFile, strlen(XrefFile) + strlen(ctp->ref) + 1);
			strcat(full, ctp->ref);
			cp->ex(full);
		}
		else
			cp->ex(ctp->ref);
	}
	else {
		cp->ex(ctp->ref);
	}
	wf->putCtl(*cp);

	if (ctp->win
	 && (*(ctp->win) != '\0')) {
		cp->siz((long) 0);
		cp->ex(ctp->win);
		wf->putCtl(*cp);
	}
	delete cp;

	gp->dt(endgroup);
	wf->putCtl(*gp);
	delete gp;
}


void
HTMwr::WriteSearchDCL(DCwfile *cwf, char *chp, long id)
{
	if ((chp == NULL) || (id == 0))
		return;

	DClist *lp = (DClist *) id;
	DCLwfile *wf = (DCLwfile *) cwf;

	// write contents entries as DCL groups
	DCctl *gp = new DCctl(group, 2, 110, 3, 0);
	wf->putCtl(*gp);

	DCctl *cp = new DCctl(ename, 2, 110, 3, 0);
	cp->ex(chp);
	wf->putCtl(*cp);

	cp->dt(ilong);
	long len = lp->count();
	for (long i = 1; i <= len; i++) {
		cp->siz(lp->number(i) - 1);
		wf->putCtl(*cp);
	}

	delete cp;

	gp->dt(endgroup);
	wf->putCtl(*gp);
	delete gp;
}


void
HTMwr::WriteIndexDCL(DCLwfile *wf, ixprop *ip)
{
	if (ip == NULL)
		return;

	// write index or ALink entries as DCL groups
	DCctl *gp = new DCctl(group, 2, 110, 1, 0);
	wf->putCtl(*gp);

	DCctl *cp = new DCctl(etext, 2, 110, 1, 0);
	cp->ex(ip->sort);
	wf->putCtl(*cp);

	cp->dt(etext);
	cp->siz((long) 0);
	cp->ex(ip->name);
	wf->putCtl(*cp);

	if (ip->ref) {
		cp->dt(ename);
		cp->siz((long) 0);
		cp->ex(ip->title);
		wf->putCtl(*cp);

		cp->dt(ename);
		cp->siz((long) 0);
		cp->ex(ip->ref);
		wf->putCtl(*cp);
		if (ip->win
		 && (*(ip->win) != '\0')) {
			cp->siz((long) 0);
			cp->ex(ip->win);
			wf->putCtl(*cp);
		}

		cp->dt(ilong);
		cp->siz(ip->refid);
		wf->putCtl(*cp);
	}

	delete cp;

	gp->dt(endgroup);
	wf->putCtl(*gp);
	delete gp;
}


void
HTMwr::WriteMapDCL(DCfile *mwf, void *vp)
{
	if (vp == NULL)
		return;

	mapprop *mp = (mapprop *) vp;
	DCLwfile *wf = (DCLwfile *) mwf;

	// write map entries as DCL groups
	DCctl *gp = new DCctl(group, 2, 110, 3, 0);
	wf->putCtl(*gp);

	DCctl *cp = new DCctl(etext, 2, 110, 3, 0);
	cp->ex(mp->name);
	wf->putCtl(*cp);

	cp->dt(ename);
	cp->siz((long) 0);
	cp->ex(mp->ref);
	wf->putCtl(*cp);

	if (mp->win) {
		cp->dt(etext);
		cp->siz((long) 0);
		cp->ex(mp->win);
		wf->putCtl(*cp);
	}
	delete cp;

	gp->dt(endgroup);
	wf->putCtl(*gp);
	delete gp;
}


void
HTMwr::WriteContextDCL(DCwfile *mwf, char *chp, long i)
{
	if (chp == NULL)
		return;

	DCLwfile *wf = (DCLwfile *) mwf;

	// write context entries as DCL groups
	DCctl *gp = new DCctl(group, 2, 110, 3, 0);
	wf->putCtl(*gp);

	DCctl *cp = new DCctl(ename, 2, 110, 3, 0);
	cp->ex(chp);
	wf->putCtl(*cp);
	delete cp;

	EclipseContextRefs.write(i, mwf, WriteContextTopicDCL);

	gp->dt(endgroup);
	wf->putCtl(*gp);
	delete gp;
}

void
HTMwr::WriteContextTopicDCL(DCfile *mwf, void *vp)
{
	if (vp == NULL)
		return;

	ecrefs *ep = (ecrefs *) vp;
	DCLwfile *wf = (DCLwfile *) mwf;

	// write context topic entries as DCL groups
	DCctl *gp = new DCctl(group, 2, 110, 3, 0);
	wf->putCtl(*gp);

	DCctl *cp = new DCctl(etext, 2, 110, 3, 0);
	cp->ex(ep->ctitle);
	wf->putCtl(*cp);

	cp->dt(ename);
	cp->siz((long) 0);
	cp->ex(ep->cfile);
	wf->putCtl(*cp);

	if (ep->cref) {
		cp->siz((long) 0);
		cp->ex(ep->cref);
		wf->putCtl(*cp);
	}

	delete cp;

	gp->dt(endgroup);
	wf->putCtl(*gp);
	delete gp;
}



// processing LIST file to merge DCL parts of index


void
HTMwr::ProcListFile(DCrfile *rf, char *destname)
{
	if ((rf == NULL)
	 || !(destname && *destname))
		return;

	if (XMLSource) {  // set defaults for DITA2Go
		LogFileName = "_d2g_log.txt";
		ShowLog = true;
	}
	if (!Suffixes.next)
		Suffixes.init(SuffixesInit);

	DCini::InitIniModule(IniFile, HTMIniSects);
	DCmacr::InitMacroModule(NULL, IniFile, MacroFile,
	 WriteMacroChar, WritePredefinedMacro,
	 NULL, NULL, NULL, GetPredefinedVar);
	// restore inimod globals changed by macrmod
	DCini::InitIniModule(IniFile, HTMIniSects);

	DCini::ProcIniSect("Setup");
	DCini::ProcIniSect("Index");
	DCini::ProcIniSect("Logging");

	if (UseLog) {
		PMFlag2 = OpenSemaphore(SEMAPHORE_ALL_ACCESS, FALSE,
			 (XMLSource ? "d2gpm" : "m2gpl"));
		Log = new DClog(LogFileName, "dwhtm", DWhtmVer, LogLevels);
		if (Log->FErr() != fok)
			UseLog = false;
		LogFileName = NewName(Log->getFilePath());
	}
	DCirfile::LogIniChain = LogIniChains;
	DCirfile::LogEventFunc = (logfunc) LogEvent;
	DCicu::LogEventFunc = (logfunc) LogEvent;

	if (UseLog)
		Log->logEvent(loginfo, 1, "Processing book info in \"", 
			rf->getFileName(), "\"");

	if (LogIniChains)
		DCirfile::WriteIniChain("Configs");

	char *buf = NewName(TextBufMax);
	if (!rf->getLine(buf, TextBufMax)
	 || _strnicmp(buf, "list", 4))
		return;

	char *cpt = FilterVer;
	FilterVer = NewName(strlen(cpt) + strlen(DWhtmVer) + 2);
	sprintf(FilterVer, "%s %s", cpt, DWhtmVer);
	
	StripSuf(destname);
	CurrFileName = NewName(destname);
	CurrBaseName = rf->getBaseName();
	CurrFilePath = rf->getFilePath();
	CurrFileFull = rf->getFileName();

	long pos = rf->tell();

	if (IniFile) {
		if ((IniFile->Section("Setup")
		  || IniFile->Section("FDK"))
		 && (IniFile->Find("FileSuffix") 
		  || IniFile->Find("XMLSuffix"))) 
			XrefFileSuffix = IniFile->StrRight();
		else if (IniFile->Section("HtmlOptions")
		 && IniFile->Find("FileSuffix"))
			XrefFileSuffix = IniFile->StrRight();
		if (*XrefFileSuffix == '.')
			XrefFileSuffix++;

		// XMLSource already set
		if (IniFile->Section("Options")
		 && IniFile->Find("UsePartFiles"))
			UsePartFiles = IniFile->BoolRight();
		if (IniFile->Section("Automation")
		 && IniFile->Find("CopyOriginalGraphics"))
			CopyOriginalGraphics = IniFile->BoolRight();
	}

	if (CopyOriginalGraphics)
		ProcGraphCopyFiles(rf, buf);

	if (!(UseIndex || HXMwr::DocBook || HXMwr::DITA
	  || (ListType == 'B') || (ListType == 'C') || (ListType == 'I'))
	 || !(UseIndex || HXMwr::DocBook || HXMwr::DITA
	  || (HelpType == 'M') || (HelpType == 'J')
	  || (HelpType == 'E') || (HelpType == 'C')))
		return;

	if (HXMwr::DocBook) {
		HXMwr::IniFile = IniFile;
		HXMwr::InitDocBook();
		DCini::ProcIniGroup("OptionsSet");
		DCini::ProcIniGroup("DocBookSet");
		if (HXMwr::WriteBookFile)
			HXMwr::WriteDocBookFile(rf, destname, buf);
		return;
	}

	if (HXMwr::DITA) {
		HXMwr::IniFile = IniFile;
		HXMwr::InitDITA();
		DCini::ProcIniGroup("OptionsSet");
		DCini::ProcIniSect("DITAOptions");
		if (HXMwr::WriteDitamaps)
			HXMwr::WriteDITABookMap(rf, destname, buf);
		return;
	}

	// for each type, create appropriate output file,
	if (HelpType == 'M') {
		UseCodePage = true;
		MakeAliasFile = true;
		if (IniFile
		 && (IniFile->Section("FDK")
			|| IniFile->Section("Setup"))
		 && IniFile->Find("HHPFileName")) 
			DefaultHelpProjectFile = IniFile->StrRight();
		DCini::ProcIniGroup("HHSet");
		if (HelpFileLanguage) {
			DCicu::SetCodePage(HelpFileLanguage);
			XMLEncoding = DCicu::XMLEncoding;
			if (DCicu::CodePage == 0) {
				UseCodePage = false;
				NoEnc = false;
				LogEvent(logwarn, 1, "Reqested locale requires Unicode: ",
				 HelpFileLanguage);
			}
			else if ((DCicu::CodePage == 1255)
			 || (DCicu::CodePage == 1256))
				LogEvent(logwarn, 1, "RTL locale not fully supported: ",
				 HelpFileLanguage);
		}
	}
	else if (HelpType == 'E') {
		UseFTS = true;
		DCini::ProcIniGroup("EHSet");
	}
	else if (HelpType == 'J') {
		if (IniFile
		 && (IniFile->Section("FDK")
			|| IniFile->Section("Setup"))
		 && IniFile->Find("HSFileName")) 
			DefaultHelpSetFile = IniFile->StrRight();
		DCini::ProcIniGroup("JavaSet");
		DCini::ProcIniGroup(OracleHelp ? "OHJSet" : "JHSet");
		if (OracleHelp)
			JHVersion2 = false;
		else if (JHVersion2) {
			JHDtdVer = "2_0";
			JHFileVer = "2.0";
			if (IniFile)
				SetJHLists();
		}
	}
	else if (HelpType == 'C') {
		MakeAliasFile = true;
		if (IniFile
		 && (IniFile->Section("FDK")
			|| IniFile->Section("Setup"))
		 && IniFile->Find("OHProjectName")) 
			OHProjectName = IniFile->StrRight();
		DCini::ProcIniSect("HTMLOptions");
		DCini::ProcIniGroup("OHSet");
		destname = OHProjectName;
	}

#if 0
	else if ((HelpType == 'S')
	 && UseIndex)
		destname = SetIndexFileName(destname);
#endif

	// process each input to it
	if (HelpType == 'J') {
		char *dest = ReplaceExt(destname, ".jhm");
		MapFile = new DCwfile(dest, ftxt);
		if (MapFile->FErr() != fok)
			return;
		WriteMapFileStart();
		if (!JHVersion2)
			NoANames = true;
	}

	// make contents file
	if ((ListType == 'B')
	 || (ListType == 'C')) {
		WriteContentsFile(rf, destname, buf);
		if (ListType == 'B')
			rf->seek(pos);
	}

	if (HelpType == 'J')
		MapFile->putCr();

	// make index file
	if ((ListType == 'B')
	 || (ListType == 'I')
	 || UseIndex)
		WriteIndexFile(rf, destname, buf);

	if (HelpType == 'J')
		MapFile->putCr();

	if (UseFTS && OmniHelp) {
		rf->seek(pos);
		WriteSearchFile(rf, destname, buf);
	}

	// make alias file
	if (OmniHelp
	 || ((HelpType == 'M') && MakeAliasFile)) {
		rf->seek(pos);
		WriteAliasFile(rf, destname, buf);
	}

	if (OmniHelp
	 || (OracleHelp && MakeALinkFile)) {
		rf->seek(pos);
		WriteALinkFile(rf, destname, buf);
	}

	if (UseGlossary) {
		rf->seek(pos);
		WriteGlossFile(rf, "glossary.xml", buf);
	}

	if (HelpType == 'M') {
		WriteHelpProjectFile(destname);
		if (MakeCshMapFile)
			WriteCSHMapFile(destname);
	}
	else if (OmniHelp)
		WriteOHProjectFile(destname);
	else if (HelpType == 'J')
		WriteHelpSet(destname);
	else if (HelpType == 'E') {
		if (EclipseUseContext) {
			EclipseContextCount = 0;
			// process each line in list
			rf->seek(pos);
			while (rf->getLine(buf, TextBufMax))
				ProcContextPartFile(buf);
			if (EclipseContextCount)
				WriteEclipseContext();
		}
		if (EclipseUsePlugin)
			WriteEclipsePlugin();
		if (EclipseUseManifest)
			WriteEclipseManifest();
	}

	if (HelpType == 'J') {
		rf->seek(pos);
		// process each line in list
		while (rf->getLine(buf, TextBufMax))
			ProcMapPartFile(buf);
		WriteMapFileEnd();
		MapFile->CloseFile();
	}

	if (AliasFile)
	  AliasFile->CloseFile();
	if (AliasTitleFile)
	  AliasTitleFile->CloseFile();

	if (UseCodePage)
		DCicu::CloseCodePage();

	if (UseLog) {
		LogEvent(loginfo, 1, "Book processing completed by dwhtm.");
		Log->CloseFile();
		if (Log->ErrWarn) {
			if (PMFlag2)  // running from plugin or PM
				ReleaseSemaphore(PMFlag2, 1L, NULL);  // puts semaphore in signalled state
			else if (ShowLog) { // display log file
				char *cmd = NewName(EditorFileName, strlen(EditorFileName) +
																						strlen(LogFileName) + 1);
				strcat(cmd, " ");
				strcat(cmd, LogFileName);
				DCauto::RunDOSCommand(cmd, true, false, false);
			}
		}
	}
}

// graphics copy file processing


void
HTMwr::ProcGraphCopyFiles(DCrfile *rf, char *buf)
{
	char *dest = "EndAuto.grc";
	DCwfile *df = new DCwfile(dest, ftxt);
	if (df->FErr() != fok) {
		delete df;
		return;
	}

	df->putStr("; EndingAuto.grc contains the graphic files to be copied");
	df->putCr();
	df->putCr();
	df->putStr("[GraphCopyFiles]");
	df->putCr();

	DCnlist paths;
	DCnlist fixed;
	long cnt = 0;

	// read the individual grc files

	long pos = rf->tell();

	char *part = NULL;
	DCirfile *pfile = NULL;
	long idx = 0;
	char *gn = NULL;
	char *gf = NULL;
	long nidx = 0;
	long didx = 0;
	long cidx = 0;


	while (rf->getLine(buf, TextBufMax)) {
		part = ReplaceExt(buf, ".grc");
		pfile = new DCirfile(part);
		if (pfile->FErr() != fok) {
			delete pfile;
			continue;
		}
		if (pfile->Section("GrPaths")) {
			while (pfile->NextItem()) {
				idx = pfile->IntRight();
				gn = pfile->StrLeft();
				paths.add(gn, idx + nidx);
			}
		}
		if (pfile->Section("GrFixedPaths")) {
			while (pfile->NextItem()) {
				idx = pfile->IntRight();
				gf = pfile->StrLeft();
				fixed.add(gf, idx + nidx);
			}
		}
		nidx += idx;
		delete pfile;
	}
	rf->seek(pos);

	DCnlist combo;
	DCnlist *lp = &combo;

	// write combined list

	for (idx = 1; idx <= nidx; idx++) {
		gn = paths.find(idx);
		if ((gf = fixed.find(idx)) == NULL)
			gf = NewName("");
		if ((didx = combo.getitem(gn, &lp, true, false)) == 0)
			combo.add(gn, (long) gf);
		else {  // one or more matches
			if ((*((char *) didx) == '\0')
			 && (*gf == '\0'))
				continue;
			else if (!strcmp((char *) didx, gf))
				continue;
			else
				combo.add(gn, (long) gf);
		}
	}

	// write the list items
	lp = &combo;
	while ((lp = lp->next) != NULL) {
		df->putStr(lp->name);
		df->putStr("= ");
		df->putStr((char *) lp->id);
		df->putCr();
	}

	df->putCr();
	df->putStr("[End]");
	df->putCr();
	df->putCr();
	df->CloseFile();
	delete df;
}


// Help file component processing

void
HTMwr::WriteContentsFile(DCrfile *rf, char *dest, char *buf)
{
	if (HelpType == 'M')
		dest = ReplaceExt(dest, ".hhc");
	else if (HelpType == 'E')
		dest = EclipseTocFilename;
	else if (HelpType == 'J')
		dest = ReplaceExt(dest, "TOC.xml");
	else if (HelpType == 'C')
		dest = ReplaceExt(dest, "_ohc.js");
	else
		return;

	ContentsFile = new DCwfile(dest, ftxt);
	if (ContentsFile->FErr() != fok)
		return;
	WriteContentsStart();

	// process each line in list
	while (rf->getLine(buf, TextBufMax)) {
		ContentsPartList.append(ContentsID, ++ContentsPartCount);
		ProcContentsPartFile(buf);
	}

	WriteContentsEnd();

  ContentsFile->CloseFile();
  ContentsFile->Err();  // if problem, exit with the err (and a message)
}


void
HTMwr::ProcContentsPartFile(char* name)
{
	char *part = ReplaceExt(name, ".bhc");
	DCLrfile *rf = new DCLrfile(part);
  if (rf->FErr() != fok)
		return;

	DCctl *cp = NULL;
	cntprop *cpt = NULL;
	bool window = false;

  while (!rf->eof()) {
    cp = new DCctl;
    *cp = *rf;

		if ((cp->maj() == 2)
		 && (cp->min() == 110)
		 && (cp->dat() == 2))
			switch (cp->dt()) {
				case group:
					cpt = new cntprop;
					break;
				case ilong:
					cpt->level = (uns) cp->siz();
					break;
				case ishort:
					cpt->noref = (bool) cp->siz();
					break;
				case etext:
					cpt->title = NewName((char *) cp->ex());
					break;
				case ename: 
					if (!window) {
						cpt->ref = NewName((char *) cp->ex());
						window = true;
					}
					else {
						cpt->win = NewName((char *) cp->ex());
					}
					break;
				case endgroup:
					WriteContentsItem(NULL, cpt);
					DeleteName(cpt->title);
					DeleteName(cpt->ref);
					if (window) {
						DeleteName(cpt->win);
						window = false;
					}
					delete cpt;
					break;
				default:
					break;
			}
		delete cp;
	}
	DeleteName(part);
	delete rf;
}


void
HTMwr::WriteSearchFile(DCrfile *rf, char *dest, char *buf)
{
	if (!ContentsPartCount)
		return;

	long part = 0;

	dest = ReplaceExt(dest, "_ohs.js");
	SearchFile = new DCwfile(dest, ftxt);
	if (SearchFile->FErr() != fok)
		return;
	WriteSearchStart();

	// process each line in list, merge search terms into single list
	while (rf->getLine(buf, TextBufMax)) {
		ContentsID = ContentsPartList.find(++part);
		ProcSearchPartFile(buf);
	}
	SearchWordList.writeall(SearchFile, WriteSearchItem);

	WriteSearchEnd();

  SearchFile->CloseFile();
  SearchFile->Err();  // if problem, exit with the err (and a message)
}


void
HTMwr::ProcSearchPartFile(char* name)
{
	char *part = ReplaceExt(name, ".bhs");
	DCLrfile *rf = new DCLrfile(part);
  if (rf->FErr() != fok)
		return;

	DCctl *cp = NULL;
	DClist *pt = NULL;
	char *txt = NULL;

  while (!rf->eof()) {
    cp = new DCctl;
    *cp = *rf;

		if ((cp->maj() == 2)
		 && (cp->min() == 110)
		 && (cp->dat() == 3))
			switch (cp->dt()) {
				case group:
					break;
				case ename:
					txt = (char *) cp->ex() ;
					if ((pt = (DClist *) SearchWordList(txt)) == NULL) {
						if (UnicodeFTS)
							SearchWordList.addsortlex(NewName(txt), (long) (pt = new DClist), cmplexstrs);
						else
							SearchWordList.addsorted(NewName(txt), (long) (pt = new DClist));
					}
					break;
				case ilong:
					pt->add(cp->siz() + ContentsID + 1);
					break;
				case endgroup:
					break;
				default:
					break;
			}
		delete cp;
	}
	DeleteName(part);
	delete rf;
}


void
HTMwr::WriteIndexFile(DCrfile *rf, char *dest, char *buf)
{
	if (HelpType == 'M')
		dest = ReplaceExt(dest, ".hhk");
	else if (HelpType == 'E')
		dest = EclipseIdxFilename;
	else if (HelpType == 'J')
		dest = ReplaceExt(dest, "Index.xml");
	else if (HelpType == 'C') {
		dest = ReplaceExt(dest, "_ohk.js");
		if (!ContentsPartCount)
			return;
	}

#if 0
	else if ((HelpType == 'S')
	 && UseIndex) {
		; // dest is already set correctly
		if (IniFile->Section("Index")
		 && IniFile->Find("IndexFile"))
			dest = IniFile->StrRight();
		else
			dest = ReplaceExt(dest, "_idx.htm");
	}
#endif

	else
		return;

	long part = 0;

#if 0
	if ((HelpType == 'S')
	 && UseIndex)
		IndexFile = new HTMwfile(dest);
	else
#endif

	IndexFile = new DCwfile(dest, ftxt);

	if (IndexFile->FErr() != fok)
		return;
	WriteIndexStart();

	IndexRefCount = 0;
	IndexRefAlloc = 512;
	IndexRefs = new (void *[IndexRefAlloc]);

	if (IndexSortType == 'L')
		setlocale(LC_ALL, IndexSortLocale);

	// process each line in list
	while (rf->getLine(buf, TextBufMax)){
		ContentsID = ContentsPartList.find(++part);
		ProcIndexPartFile(buf);
	}

	IndexRefs[IndexRefCount] = NULL;  // guard entry

	WriteIndexRefs();

	WriteIndexEnd();

  IndexFile->CloseFile();
  IndexFile->Err();  // if problem, exit with the err (and a message)
}


void
HTMwr::ProcIndexPartFile(char* name)
{
	char *part = ReplaceExt(name, ".bhk");
	DCLrfile *rf = new DCLrfile(part);
  if (rf->FErr() != fok)
		return;

	DCctl *cp = NULL;
	ixprop *ip = NULL;
	long ipx = 0;
	long nalloc = 0;
	void **narray = NULL;
	bool sort = false;
	bool title = false;
	bool window = false;

  while (!rf->eof()) {
    cp = new DCctl;
    *cp = *rf;

		if ((cp->maj() == 2)
		 && (cp->min() == 110)
		 && (cp->dat() == 1))
			switch (cp->dt()) {
				case group:
					if ((IndexRefCount + 1) > IndexRefAlloc) {
						nalloc = IndexRefAlloc * 2;
						narray = new (void *[nalloc]);
						memcpy(narray, IndexRefs, IndexRefAlloc * sizeof(void *));
						delete [] IndexRefs;
						IndexRefs = narray;
						IndexRefAlloc = nalloc;
					}
					ip = new ixprop;
					sort = true;
					title = true;
					break;
				case etext:
					if (sort) {
						ip->sort = NewName((char *) cp->ex());
#if 0
						if (IndexSortType == 'L') {
							size_t len = strxfrm(NULL, ip->sort, 0);
							char *tmp = NewName(len);
							strxfrm(tmp, ip->sort, len + 1);
							ip->sort = tmp;
						}
						else 
#endif
						if (IndexSortType == 'A') {
							for (unc *cpt = (unc *) ip->sort; *cpt ; cpt++) {
								if (*cpt >= 0x80)
									*cpt = AlphaSortChars[*cpt - 0x80];
							}
						}

						sort = false;
					}
					else
						ip->name = NewName((char *) cp->ex());
					break;
				case ename:
					if (title) {
						ip->title = NewName((char *) cp->ex());
						title = window = false;
					}
					else if (!window) {
						ip->ref = NewName((char *) cp->ex());
						window = true;
					}
					else {
						ip->win = NewName((char *) cp->ex());
						window = false;
					}
					break;
				case ilong:
					ip->refid = cp->siz() + ContentsID;
					break;
				case endgroup:
					// merge the index entry DCL into the IndexRefs array here
					while ((ipx < IndexRefCount)
					 && (CompIdxNames(&IndexRefs[ipx], &ip) <= 0))
						ipx++;
					// use insertion sort and memmove to shift in buffer
					if (ipx < IndexRefCount)
						memmove(&IndexRefs[ipx + 1], &IndexRefs[ipx],
										(IndexRefCount - ipx) * sizeof(void *));
					IndexRefs[ipx] = ip;
					ipx++;
					IndexRefCount++;
					break;
				default:
					break;
			}
		delete cp;
	}
	DeleteName(part);
	delete rf;
}


void
HTMwr::WriteALinkFile(DCrfile *rf, char *dest, char *buf)
{
	if (HelpType == 'J')
		dest = ReplaceExt(dest, "Links.xml");
	else if (HelpType == 'C')
		dest = ReplaceExt(dest, "_ohl.js");
	else
		return;

	long part = 0;

	ALinkFile = new DCwfile(dest, ftxt);
	if (ALinkFile->FErr() != fok)
		return;
	WriteALinkStart();

	if (OmniHelp) {
		while (rf->getLine(buf, TextBufMax)) {
			ContentsID = ContentsPartList.find(++part);
			ProcOHALinkPartFile(buf);
		}
		OHALinkList.writeall(ALinkFile, WriteALinkItem);
	}
	else {
		ALinkCount = 0;
		ALinkAlloc = 512;
		ALinkRefs = new (void *[ALinkAlloc]);

		// process each line in list
		while (rf->getLine(buf, TextBufMax))
			ProcALinkPartFile(buf);

		ALinkRefs[ALinkCount] = NULL;  // guard entry

		WriteALinkRefs();
	}

	WriteALinkEnd();

  ALinkFile->CloseFile();
  ALinkFile->Err();  // if problem, exit with the err (and a message)
}


void
HTMwr::ProcALinkPartFile(char* name)
{
	char *part = ReplaceExt(name, ".bhl");
	DCLrfile *rf = new DCLrfile(part);
  if (rf->FErr() != fok)
		return;

	DCctl *cp = NULL;
	ixprop *ip = NULL;
	long ipx = 0;
	long nalloc = 0;
	void **narray = NULL;
	bool sort = false;
	bool title = false;

  while (!rf->eof()) {
    cp = new DCctl;
    *cp = *rf;

		if ((cp->maj() == 2)
		 && (cp->min() == 110)
		 && (cp->dat() == 1))
			switch (cp->dt()) {
				case group:
					if ((ALinkCount + 1) > ALinkAlloc) {
						nalloc = ALinkAlloc * 2;
						narray = new (void *[nalloc]);
						memcpy(narray, ALinkRefs, ALinkAlloc * sizeof(void *));
						delete [] ALinkRefs;
						ALinkRefs = narray;
						ALinkAlloc = nalloc;
					}
					ip = new ixprop;
					sort = true;
					title = true;
					break;
				case etext:
					if (sort) {
						ip->sort = NewName((char *) cp->ex());
						sort = false;
					}
					else
						ip->name = NewName((char *) cp->ex());
					break;
				case ename:
					if (title) {
						ip->title = NewName((char *) cp->ex());
						title = false;
					}
					else
						ip->ref = NewName((char *) cp->ex());
					break;
				case endgroup:
					// merge the alink entry DCL into the ALinkRefs array here
					while ((ipx < ALinkCount)
					 && (CompALinkNames(&ALinkRefs[ipx], &ip) <= 0))
						ipx++;
					// use insertion sort and memmove to shift in buffer
					if (ipx < ALinkCount)
						memmove(&ALinkRefs[ipx + 1], &ALinkRefs[ipx],
										(ALinkCount - ipx) * sizeof(void *));
					ALinkRefs[ipx] = ip;
					ipx++;
					ALinkCount++;
					break;
				default:
					break;
			}
		delete cp;
	}
	DeleteName(part);
	delete rf;
}


void
HTMwr::ProcOHALinkPartFile(char* name)
{
	char *part = ReplaceExt(name, ".bhl");
	DCLrfile *rf = new DCLrfile(part);
  if (rf->FErr() != fok)
		return;

	DCctl *cp = NULL;
	DClist *pt = NULL;
	char *txt = NULL;

  while (!rf->eof()) {
    cp = new DCctl;
    *cp = *rf;

		if ((cp->maj() == 2)
		 && (cp->min() == 110)
		 && (cp->dat() == 3))
			switch (cp->dt()) {
				case group:
					break;
				case ename:
					txt = (char *) cp->ex() ;
					if ((pt = (DClist *) OHALinkList(txt)) == NULL) {
						if (UnicodeFTS)
							OHALinkList.addsortlex(NewName(txt), (long) (pt = new DClist), cmplexstrs);
						else
							OHALinkList.addsorted(NewName(txt), (long) (pt = new DClist));
					}
					break;
				case ilong:
					pt->add(cp->siz() + ContentsID + 1);
					break;
				case endgroup:
					break;
				default:
					break;
			}
		delete cp;
	}
	DeleteName(part);
	delete rf;
}


void
HTMwr::WriteGlossFile(DCrfile *rf, char *dest, char *buf)
{
	long part = 0;

	GlossFile = new DCwfile(dest, ftxt);
	if (GlossFile->FErr() != fok)
		return;
	WriteGlossStart();

	GlossCount = 0;
	GlossAlloc = 512;
	GlossRefs = new (void *[GlossAlloc]);

	// process each line in list
	while (rf->getLine(buf, TextBufMax))
		ProcGlossPartFile(buf);

	GlossRefs[GlossCount] = NULL;  // guard entry

	WriteGlossRefs();

	WriteGlossEnd();

  GlossFile->CloseFile();
  GlossFile->Err();  // if problem, exit with the err (and a message)
}

void
HTMwr::ProcGlossPartFile(char* name)
{
	char *part = ReplaceExt(name, ".bhg");
	DCLrfile *rf = new DCLrfile(part);
  if (rf->FErr() != fok)
		return;

	DCctl *cp = NULL;
	ixprop *ip = NULL;
	long ipx = 0;
	long nalloc = 0;
	void **narray = NULL;
	bool sort = false;
	bool title = false;

  while (!rf->eof()) {
    cp = new DCctl;
    *cp = *rf;

		if ((cp->maj() == 2)
		 && (cp->min() == 110)
		 && (cp->dat() == 1))
			switch (cp->dt()) {
				case group:
					if ((GlossCount + 1) > GlossAlloc) {
						nalloc = GlossAlloc * 2;
						narray = new (void *[nalloc]);
						memcpy(narray, GlossRefs, GlossAlloc * sizeof(void *));
						delete [] GlossRefs;
						GlossRefs = narray;
						GlossAlloc = nalloc;
					}
					ip = new ixprop;
					sort = true;
					title = true;
					break;
				case etext:
					if (sort) {
						ip->sort = NewName((char *) cp->ex());
						sort = false;
					}
					else
						ip->name = NewName((char *) cp->ex());
					break;
				case ename:
					if (title) {
						ip->title = NewName((char *) cp->ex());
						title = false;
					}
					else
						ip->ref = NewName((char *) cp->ex());
					break;
				case endgroup:
					// merge the Gloss entry DCL into the GlossRefs array here
					while ((ipx < GlossCount)
					 && (CompGlossNames(&GlossRefs[ipx], &ip) <= 0))
						ipx++;
					// use insertion sort and memmove to shift in buffer
					if (ipx < GlossCount)
						memmove(&GlossRefs[ipx + 1], &GlossRefs[ipx],
										(GlossCount - ipx) * sizeof(void *));
					GlossRefs[ipx] = ip;
					ipx++;
					GlossCount++;
					break;
				default:
					break;
			}
		delete cp;
	}
	DeleteName(part);
	delete rf;
}


void
HTMwr::ProcContextPartFile(char* name)
{
	char *part = ReplaceExt(name, ".bha");
	DCLrfile *rf = new DCLrfile(part);
  if (rf->FErr() != fok)
		return;

	DCctl *cp = NULL;
	ecrefs *ep = NULL;
	int lev = 0;
	bool topic = false;
	bool file = true;
	long cnum = 0;
	char *id = NULL;
	char *cname = NULL;
	char *title = NULL;
	char *ref = NULL;

  while (!rf->eof()) {
    cp = new DCctl;
    *cp = *rf;

		if ((cp->maj() == 2)
		 && (cp->min() == 110)
		 && (cp->dat() == 3))
			switch (cp->dt()) {
				case group:
					if (++lev == 2) {
						topic = true;
						file = true;
					}
					break;
				case etext:
					if (topic)
						title = NewName((char *) cp->ex());
					break;
				case ename:
					if (topic) {
						if (file) {
							cname = NewName((char *) cp->ex());
							file = false;
							ref = NULL;
						}
						else
							ref = NewName((char *) cp->ex());
					}
					else {
						id = NewName((char *) cp->ex());
						if ((cnum = EclipseContextNames(id)) == 0)  // new id
							EclipseContextNames.add(id, cnum = ++EclipseContextCount);
						else
							DeleteName(id);
					}
					break;
				case endgroup:
					if (--lev == 0)  // end of topics for this ID
						id = NULL;
					else { // end of one topic ref
						ep = new ecrefs;
						ep->ctitle = title;
						ep->cfile = cname;
						ep->cref = ref;
						EclipseContextRefs.add(ep, cnum);
						title = cname = NULL;
						topic = false;
					}
					break;
				default:
					break;
			}
		delete cp;
	}
	DeleteName(part);
	delete rf;
}


void
HTMwr::ProcMapPartFile(char* name)
{
	char *part = ReplaceExt(name, ".bhm");
	DCLrfile *rf = new DCLrfile(part);
  if (rf->FErr() != fok)
		return;

	DCctl *cp = NULL;
	mapprop *mp = NULL;
	bool wind = false;

  while (!rf->eof()) {
    cp = new DCctl;
    *cp = *rf;

		if ((cp->maj() == 2)
		 && (cp->min() == 110)
		 && (cp->dat() == 3))
			switch (cp->dt()) {
				case group:
					mp = new mapprop;
					mp->win = NULL;
					wind = false;
					break;
				case etext:
					if (wind)
						mp->win = NewName((char *) cp->ex());
					else {
						mp->name = NewName((char *) cp->ex());
						wind = true;
					}
					break;
				case ename: 
					mp->ref = NewName((char *) cp->ex());
					break;
				case endgroup:
					WriteMapFileItem(mp->name, mp->ref, mp->win);
					DeleteName(mp->name);
					DeleteName(mp->ref);
					if (mp->win)
						DeleteName(mp->win);
					delete mp;
					break;
				default:
					break;
			}
		delete cp;
	}
	DeleteName(part);
	delete rf;
}


void
HTMwr::WriteAliasFile(DCrfile *rf, char *dest, char *buf)
{
	if (HelpType == 'M')
		dest = ReplaceExt(dest, ".hha");
	else if (OmniHelp)
		dest = ReplaceExt(dest, "_oha.js");
	else
		return;

	AliasCount = 0;
	AliasFile = new DCwfile(dest, ftxt);
	if (AliasFile->FErr() != fok)
		return;

	if (AliasTitle) {
		dest = ReplaceExt(dest, ".hht");
		AliasTitleFile = new DCwfile(dest, ftxt);
	}

	if (OmniHelp) {
		AliasFile->putStr("var cshItems = [");
		AliasFile->putCr();
	}

	// process each line in list
	while (rf->getLine(buf, TextBufMax))
		ProcAliasPartFile(buf);

	if (OmniHelp) {
		AliasFile->putStr(" ]");
		AliasFile->putCr();
	}

  AliasFile->Err();  // if problem, exit with the err (and a message)
	// do not close here; if not OmniHelp, more may be added
}


void
HTMwr::ProcAliasPartFile(char* name)
{
	char *part = ReplaceExt(name, ".bha");
	DCLrfile *rf = new DCLrfile(part);
  if (rf->FErr() != fok)
		return;

	DCctl *cp = NULL;
	mapprop *mp = NULL;
	bool wind = false;

  while (!rf->eof()) {
    cp = new DCctl;
    *cp = *rf;

		if ((cp->maj() == 2)
		 && (cp->min() == 110)
		 && (cp->dat() == 3))
			switch (cp->dt()) {
				case group:
					mp = new mapprop;
					AliasCount++;
					wind = false;
					break;
				case etext:
					if (wind)
						mp->win = NewName((char *) cp->ex());
					else {
						mp->name = NewName((char *) cp->ex());
						wind = true;
					}
					break;
				case ename: 
					mp->ref = NewName((char *) cp->ex());
					break;
				case endgroup:
					WriteAlias(AliasFile, mp);
					if (MakeCshMapFile)
						AliasList.add(mp, ++AliasCount);
					else {
						DeleteName(mp->name);
						DeleteName(mp->ref);
						if (mp->win)
							DeleteName(mp->win);
						delete mp;
					}
					break;
				default:
					break;
			}
		delete cp;
	}
	DeleteName(part);
	delete rf;
}



// common utilities

DCnlist HTMwr::Suffixes;
npair HTMwr::SuffixesInit[] = {
 { 1, "htm" },
 { 2, "html" },
 { 3, "xml" },
 { 4, "dita" },
 { 5, "ditamap" },
 { 6, "bookmap" },
 { 7, "ent" },
 { 8, "rtf" },
 { 9, "hlp" },
 { 10, "hhp" },
 { 11, "chm" },
 { 12, "fm" },
 { 13, "mif" },
 { 14, "doc" },
 { 15, "docx" },
 { 16, "dcl" },
 { 17, "dcb" },
 { 18, "prj" },
 { 19, "ini" },
 { 20, "lst" },
 { 21, "chp" },
 { 22, "bkm" },
 { 23, "idx" },
 { 24, "frm" },
 { 25, "hs" },
 { 26, "gen" },
 { 27, "toc" },
 { 28, "hha" },
 NULL
};


char *
HTMwr::ReplaceExt(char *name, char *ext)
{
	char *ret = strrchr(name, '\r');
	if (ret)
		*ret = '\0';
	if ((ret = strrchr(name, '\r')) != NULL)
		*ret = '\0';

	char *temp = NULL;
	char *dot = strrchr(name, '.');

	// verify that part after dot is really a suffix
	if (!dot
		|| !Suffixes(dot + 1)) {
		if (ext) {
			temp = NewName(name, strlen(name) + strlen(ext));
			strcat(temp, ext);
		}
		else
			temp = NewName(name);
		return temp;
	}

	if (!ext) {
		if (dot)
			temp = NewName(name, dot - name);
		else
			temp = NewName(name);
		return temp;
	}

	if (dot) {
		temp = NewName((dot - name) + strlen(ext) + 2);
		strncpy(temp, name, dot - name);
	}
	else
		temp = NewName(name, strlen(name) + strlen(ext) + 2);

	if (!strchr(ext, '.'))
		strcat(temp, ".");

	strcat(temp, ext);
	return temp;
}



// end of dwhtmxf.cpp



