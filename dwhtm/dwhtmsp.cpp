
// dwhtmsp.cpp is the code for dcl HTM file split and extract, JHG
// Copyright (c) 1996-2000 by Omni Systems, Inc.  All rights reserved.

#include "dcappl.h"
#include "dcctlg.h"
#include "dclist.h"
#include "dcfili.h"
#include "dctxfx.h"
#include "dcfmtr.h"
#include "dcini.h"
#include "dcmacr.h"
#include "dwhtm.h"
#include "dwhtmfl.h"
#include "dcicu.h"
#include <windows.h>
#include <winnls.h>

#undef min


// file splitting


DCilist HTMwr::ParaSplitList;
DCnlist HTMwr::SplitNameList;
DCnlist HTMwr::SplitRefList;
DCnlist HTMwr::BaseNameList;
DCnlist HTMwr::SplitTitleList;
DCilist HTMwr::PrevSplitList;
DCilist HTMwr::NextSplitList;
DCnlist HTMwr::SplitNumList;
DClist HTMwr::ExcludeFromPrint;
DClist HTMwr::ExcludeFromSearch;
DClist HTMwr::ExcludeFromView;
DClist *HTMwr::ExcludedTopics = &ExcludeFromView;


bool HTMwr::FileStarted = false;
bool HTMwr::SplittingFile = false;
bool HTMwr::SplitHere = false;
bool HTMwr::StartingSplit = true;
bool HTMwr::SmartSplit = false;
bool HTMwr::NoSplit = false;
bool HTMwr::UseChunking = false;
uns HTMwr::SplitLevel = 0;
uns HTMwr::LastSplitLevel = 0;
bool HTMwr::AllowTbSplit = false;
long HTMwr::SplitID = 0;
long HTMwr::FirstSplitID = 0;
long HTMwr::LastSplitID = 0;
bool HTMwr::FirstPart = true;
bool HTMwr::LastPart = false;
long HTMwr::SplitNum = 0;
long HTMwr::NextSplitNum = 0;
char *HTMwr::SplitNumStart = "0";
long HTMwr::SplitNumIncrement = 1;
char HTMwr::SplitIDRef[NumStrMax] = {"1"};
char HTMwr::FirstSplitIDRef[NumStrMax] = {"1"};
char HTMwr::LastSplitIDRef[NumStrMax] = {"1"};
long HTMwr::ParaUID = 0;
char HTMwr::ParaUIDRef[NumStrMax] = {"0"};
long HTMwr::TxtParaUID = 0;
bool HTMwr::KeepParaUID = false;


void 
HTMwr::ProcSplit(void)
{
	long prevID = SplitID;

	MarkList.all(GetMetaAttrs);  // proc for previous part
	MarkList.all(GetRefAttrs);  // proc for previous part

	if (!XMLSource) {  // for DITA do at ProcPgEnd
		SearchHere = true;
		PrintHere = true;
		PrintOnly = false;
	}

	CurrHVIndex = NULL;  // reset for new list for next part

	SplitID = (InTbl && !StripTable) ? TxtParaUID : ParaUID;
	if (FirstSplitID == 0) {
		FirstSplitID = SplitID;
		if (CurrXFileTitle == NULL)
			CurrXFileTitle = Title;
		if (SmartSplit)
			sprintf(FirstSplitIDRef, "%ld", SplitID);
		else
			FirstPart = false;
	}
	else
		FirstPart = false;

	LastSplitID = SplitID;
	sprintf(SplitIDRef, "%ld", SplitID);
	strcpy(LastSplitIDRef, SplitIDRef);

	if (prevID == 0)
		prevID = 1;
	PrevSplitList.append(prevID, SplitID);
	NextSplitList.append(SplitID, prevID);

	if (Extracting)
		ProcEndExtract();

	ProcSplitName(SplitID);

	if (MarkerSetSplit
	 && Contents) {
		SplitLevel = Contents;
		if (MakeLocalTOC) {
			if (!LocalTOCLevel)
				LocalTOCLevel = Contents;
		}
		else LocalTOCLevel = 0;
		if (MakeTrail
		 && SplitTrail
		 && !TrailCurrLevel)
			TrailCurrLevel = Contents;
	}
}


void 
HTMwr::ProcSplitName(long sid)
{
	char *basenm = NewName(strlen(CurrFilePrefix) + 11);
	sprintf(basenm, "%s%ld", CurrFilePrefix, sid);

	char *baseid = NewName(strlen(basenm) + 3);
	sprintf(baseid, "#X%s", basenm);
	SplitRefList.add(baseid, sid);

	if (IniFile
	 && IniFile->Section("HtmlFiles")
	 && IniFile->Find(basenm)) {
		char *filenm = IniFile->StrRight();
		char *nm = NewName(strlen(filenm) + 2 + strlen(XrefFileSuffix));
		sprintf(nm, "%s.%s", filenm, XrefFileSuffix);
		SplitNameList.add(nm, sid);
		BaseNameList.add(filenm, sid);
	}
	else {
		char *nm = NewName(strlen(CurrFilePrefix) + 13 + strlen(XrefFileSuffix));
		sprintf(nm, "%s%ld.%s", CurrFilePrefix, sid, XrefFileSuffix);
		SplitNameList.add(nm, sid);
		BaseNameList.add(basenm, sid);
	}

	if (IniFile
	 && IniFile->Section("Titles")
	 && IniFile->Find(basenm))  // specified title takes precedence
		SplitTitleList.add(IniFile->StrRight(), sid);

	SplitNum = NextSplitNum;
	char *snum = NewName(NumStrMax);
	sprintf(snum, "%d", SplitNum);
	SplitNumList.add(snum, sid);
	NextSplitNum += SplitNumIncrement;
	ContentsWin = NULL;
}


bool
HTMwr::SplittingOK(void)
{
	if (!FileStarted) {   // at beginning
		if (!StartingSplit) // don't split here
			return false;
		StartSplit = true;  // did split at start
	}

	if (SmartSplit
	 && SplitLevel
	 && LastSplitLevel
	 && (SplitLevel > LastSplitLevel))
		return false;

	return true;
}


void
HTMwr::CloseFileBlocks(HTMwfile *wf)
{
	// wrap up any existing blocks, like open lists
	while (CurrList.level)
		PutListEnd(wf, false);
	Last.list = 0;
	if (InComment) {  // ending comment
		if (!InRaw) {
			wf->putEnd();
			if (ScriptTag)
				wf->putStr("//");
			wf->putStr("--");
			wf->endElem(true);
			wf->putEnd();
		}
		if (ScriptTag) {
			wf->endBlock(ScriptTag);
			wf->putEnd();
			ScriptTag = NULL;
		}
		wf->putEnd(); // blank line after comment
		LinePos = 0;
		InRaw = InComment = false;
	}
	if (InPreform) {
		WriteEndCharProps(wf);
		wf->endBlock("pre");
		wf->putEnd();
		wf->putEnd(); // blank line between paras
		LinePos = 0;
		InPreform = false;
	}
	if (CodeStoreDiv) {  // end any pending div
		wf->SelectDiv(PreCodeStoreDiv);
		CodeStoreDiv = 0;
	}
}


void
HTMwr::SplitFile(HTMwfile *wf, long spID)
{
	if (HXMwr::DITA)
		HXMwr::EndAllDITATags(wf);

	if (Extracting)
		EndFileExtract(wf);

	//if (InsertIndexTOC)
	//	InsertIndexTOCItem(wf);

	CloseFileBlocks(wf);

	// write file ending stuff, including split end macros
	// even if we are still at start of file
	FinishFtns(wf, false);

	long pid = ParaRefNum;
	ParaRefNum = StartSplitParaRef ? StartSplitParaRef : StartingParaRef;

	PutInsertMacro(wf, FirstPart ? "FirstBottom" : "SplitBottom");
	PutScript(wf);
	PutObjects(wf, false);

	if (!BodyContentOnly) {
		if (UseHeadAndBody) {
			wf->putEnd();
			wf->endBlock("body");  // wrap it up
			PutInsertMacro(wf, FirstPart ? "FirstEnd" : "SplitEnd");
		}
	}
	UseFrameSet = false;

	if (!BodyContentOnly) {
		if (!HXMwr::DITA) {
			char *cpt = NewName(50);
			sprintf(cpt, "Split file for ID %ld here", spID);
			wf->putComment(cpt);
			DeleteName(cpt);
			wf->putEnd();
		}
		if (UseXMLRoot) {
			wf->endBlock(XMLRoot);
			wf->putEnd();
		}
		if (HXMwr::WrapTopicFiles) {
			wf->endBlock("dita");
			wf->putEnd();
		}
	}
	else if (!MinimizeLines)
		wf->putEnd();

	PutExtRefInfo(wf);
	SplitID = spID;

	if ((SplitID != FirstSplitID)
	 || !StartSplit)
		FirstPart = false;

	if (SplitID == LastSplitID)
		LastPart = true;

	// use SplitID to get new name from ref file
	char *fn = SplitNameList.find(SplitID);
	if (fn == NULL) {  // should never happen
		fn = NewName(strlen(CurrFilePrefix) + 13 + strlen(XrefFileSuffix));
		sprintf(fn, "%s%ld.%s", CurrFilePrefix, SplitID, XrefFileSuffix);
		SplitNameList.add(fn, SplitID);
	}

	// close file and reopen with pointer unchanged
	wf->ReopenFile(fn);

	ParaRefNum = StartSplitParaRef = pid;

	EvalTopicStartMacros(wf, SplitID);

	// write file starting stuff, including split start macros
	// use SplitID to get title from ref file, or make one up
	if (!BodyContentOnly) {
		PutInsertMacro(wf, "BeginFile");
		WriteProlog(wf, SplitID);
		if (!HXMwr::DITA)
			PutInsertMacro(wf, "Entities");

		if (UseHeadAndBody) {
			HeadMacroName = LastPart ? "LastHead" : "SplitHead";
			HeadEndMacroName = LastPart ? "LastHeadEnd" : "SplitHeadEnd";
			Title = SplitTitleList.find(SplitID);
			if (Title == NULL) {
				Title = wf->getBaseName();
				SplitTitleList.add(NewName(Title), SplitID);
			}
			if (UseDITAIndex
			 && IndexSplitFile.find(SplitID))
				WriteHead(wf, WriteIndexCSS);
			else
				WriteHead(wf);
			PutInsertMacro(wf, LastPart ? "LastFrames" : "SplitFrames");
			wf->startBlock("body");
			if (FilterConditions)
				wf->putAttrStr(FilterBarBodyAttr);
			WriteAttrib(wf, "body");
			wf->endElem(true);
			WriteBaseProps(wf);
			if (UseDITAIndex
			 && IndexSplitFile.find(SplitID))
				WriteIndexJS(wf);
		}
	}
	if (!HXMwr::DITA)
		PutInsertMacro(wf, LastPart ? "LastTop" : "SplitTop");
	FileStarted = false;

	ContentsID = ContentsSplitList.find(SplitID ? SplitID : 1);
	ContentsWin = ContentsWindowList.find(SplitID ? SplitID : 1);

	// create entries for in-range keywords
	if ((HelpType == 'M')
	 && ((RefFileType == 'H')
	 || ((ListType != 'B')
		&& (ListType != 'I'))))
		IndexRangeList.writeall(wf, PutRangeKeyword);
	else
	 	IndexRangeList.all(ResetRangeKeyword);

	// write .ref file info for [SplitFileNames] (with suf) 
	sprintf(SplitIDRef, "%ld", SplitID);
	ProcFileNameRef(SplitIDRef, fn);
	// SplitID persists for use processing ahrefs
}



// file extraction

DCilist HTMwr::ExtrStartList;
DCilist HTMwr::ExtrEndList;
DCnlist HTMwr::ExtrTagList;
DCvlist HTMwr::ExtrFormList;
bool HTMwr::Extracting = false;
bool HTMwr::ExtractEnable = false;
bool HTMwr::ExtrStarting = false;
bool HTMwr::ExtrFinishing = false;
bool HTMwr::ExtrEnding = false;
char *HTMwr::ExtrFile = NULL;
long HTMwr::ExtrID = 0;
unc HTMwr::ExtrGraphThumbnail = 'N';
bool HTMwr::OrigSizedThumbnail = false;
char *HTMwr::ExtrGraphSuffix = NULL;
char *HTMwr::ExtrGraphClass = NULL;
char *HTMwr::ExtrGraphTarget = "_blank";
char *HTMwr::ExtrGraphHigh = "96";
char *HTMwr::ExtrGraphWide = "96";
char *HTMwr::ExtrGraphOrigHigh = "0";
char *HTMwr::ExtrGraphOrigWide = "0";
char *HTMwr::ExtrGraphID = NULL;
char *HTMwr::ExtrGraph = NULL;
char *HTMwr::ExtrStartTag = NULL;
pprop *HTMwr::ExtrStartForm = NULL;
char HTMwr::ExtrIDRef[NumStrMax] = {""};



void
HTMwr::ProcStartExtract(void)
{
	// after ParaUID set for para starting extract during scan
	if (Extracting)
		ProcEndExtract();

	if ((SplitID == 0)
	 && (CurrXFileTitle == NULL))
		CurrXFileTitle = Title;
	//ProcDITATopicVars(SplitID ? SplitID : 1);
	ExtrID = ParaUID;
	ExtrStartList.append(ExtrID, TxtParaRefNum);
	ExtrTagList.add(ExtrStartTag, ExtrID);
	ExtrFormList.add(ExtrStartForm, ExtrID);
	ProcSplitName(ExtrID);
	ExtrStarting = false;
	Extracting = true;
}


void
HTMwr::ProcEndExtract(void)
{
	// after ParaUID for para after end of extract during scan
	ExtrEndList.append(ExtrID, TxtParaRefNum);
	MarkList.all(GetMetaAttrs);  // metas for extracted file
	//ProcDITATopicVars(ExtrID);
	ExtrFinishing = ExtrEnding = Extracting = false;
	ExtrID = 0;
}


void
HTMwr::StartFileExtract(HTMwfile *wf, long extrID)
{
	char *cpt = NewName(50);
	sprintf(cpt, "Extracting file for ID %ld here", extrID);
	wf->putComment(cpt);
	DeleteName(cpt);
	wf->putEnd();

	Extracting = true;
	ExtrID = extrID;
	sprintf(ExtrIDRef, "%ld", ExtrID);

	ExtrStartTag = ExtrTagList.find(ExtrID);
	ExtrStartForm = (pprop *) ExtrFormList.find(ExtrID);
	ExtrGraph = ExtrGraphID = NULL;
	ExtrGraphOrigHigh = "0";
	ExtrGraphOrigWide = "0";

	// switch to extract file
	ExtrFile = SplitNameList.find(ExtrID);
	if (ExtrFile == NULL) {  // should never happen
		ExtrFile = NewName(strlen(CurrFilePrefix) + 13 + strlen(XrefFileSuffix));
		sprintf(ExtrFile, "%s%ld.%s", CurrFilePrefix, ExtrID, XrefFileSuffix);
		SplitNameList.add(ExtrFile, ExtrID);
	}

	ProcFileNameRef(ExtrIDRef, ExtrFile);

	wf->StartAuxFile(ExtrFile, true);

	EvalTopicStartMacros(wf, ExtrID);

	if (!BodyContentOnly) {
		PutInsertMacro(wf, "BeginFile");
		WriteProlog(wf, ExtrID);
		if (!HXMwr::DITA)
			PutInsertMacro(wf, "Entities");

		if (UseHeadAndBody) {
			//HeadMacroName = "ExtrHead";

			char *lastTitle = Title;
			Title = SplitTitleList.find(ExtrID);
			if (Title == NULL) {
				if (ExtrStartForm->maceti) {
					Title = ExtrStartForm->maceti;
					SplitTitleList.add(NewName(Title), ExtrID);
				}
			}
			if (Title == NULL) {
				Title = wf->getAuxName();
				SplitTitleList.add(NewName(Title), ExtrID);
			}
			WriteHead(wf);
			Title = lastTitle;

			wf->startBlock("body");
			if (FilterConditions)
				wf->putAttrStr(FilterBarBodyAttr);
			WriteAttrib(wf, "body");
			wf->endElem(true);

			WriteBaseProps(wf);
		}
	}

	if (!HXMwr::DITA) {
		char *txt = ExtrTopMark.find(ExtrID);
		if (txt)
			DCmacr::WriteMacro(wf, txt);
		else if (!PutStyleMacro(wf, MacExtrTop, ExtrStartTag, ExtrStartForm))
			PutInsertMacro(wf, "ExtrTop");
		wf->putEnd();
	}

	FileStarted = false;
}


void
HTMwr::EndFileExtract(HTMwfile *wf)
{
	CloseFileBlocks(wf);

	//FinishFtns(wf, false);
	wf->putEnd();
	char *txt = ExtrBottomMark.find(ExtrID);
	if (txt)
		DCmacr::WriteMacro(wf, txt);
	else if (!PutStyleMacro(wf, MacExtrBottom, ExtrStartTag, ExtrStartForm))
		PutInsertMacro(wf, "ExtrBottom");
	wf->putEnd();
	PutScript(wf);
	PutObjects(wf, false);
	if (!BodyContentOnly) {
		if (UseHeadAndBody) {
			wf->putEnd();
			wf->endBlock("body");  // wrap it up
		}

		if (UseXMLRoot) {
			wf->endBlock(XMLRoot);
			wf->putEnd();
		}
	}
	else
		wf->putEnd();
	PutExtRefInfo(wf);

	// switch back to original file
	wf->EndAuxFile();
	Extracting = false;

	if (txt = ExtrReplaceMark.find(ExtrID))
		DCmacr::WriteMacro(wf, txt);
	else 
		PutStyleMacro(wf, MacExtrReplace, ExtrStartTag, ExtrStartForm);  // replace macro in orig file
	wf->putEnd();
}


void
HTMwr::WriteExtrThumb(HTMwfile *wf)
{
// <p class="<$$_extrgraphclass>"><a href="<$$_extrfile>"
//  target="<$$_extrgraphtarget>"><img src="<$$_extrgraph>" \
// <$_if ($$_extrgraphhigh > 0)> height="<$$_extrgraphhigh>"<$_endif>\
// <$_if ($$_extrgraphwide > 0)> width="<$$_extrgraphwide>"<$_endif>\
//  alt="<$$_extrtitle>" /></a></p>

	wf->startBlock("p");
	if (WriteClassAttributes
	 && ExtrGraphClass)
		wf->setClass(ExtrGraphClass);
	wf->endElem(true);
	wf->startBlock("a");
	wf->strAttr("href", ExtrFile);
	if (ExtrGraphTarget)
		wf->strAttr("target", ExtrGraphTarget);
	wf->endElem(true);
	wf->startElem("img");
	wf->strAttr("src", ExtrGraph);
	if (OrigSizedThumbnail) {
		if (ExtrGraphOrigHigh
		 && (*ExtrGraphOrigHigh != '0'))
			wf->strAttr("height", ExtrGraphOrigHigh);
		if (ExtrGraphOrigWide
		 && (*ExtrGraphOrigWide != '0'))
			wf->strAttr("width", ExtrGraphOrigWide);
	}
	else {
		if (ExtrGraphHigh
		 && (*ExtrGraphHigh != '0'))
			wf->strAttr("height", ExtrGraphHigh);
		if (ExtrGraphWide
		 && (*ExtrGraphWide != '0'))
			wf->strAttr("width", ExtrGraphWide);
	}

	char *txt = NULL;
	if (ExtrID
	 && ((txt = SplitTitleList.find(ExtrID)) != NULL))
		wf->strAttr("alt", txt);
	wf->endElem(false);
	wf->endBlock("a");
	if (NoParaClose)
		wf->putEnd();
	else
		wf->endPara();
}



// file title and name collection


bool HTMwr::TitleHere = false;
bool HTMwr::NameHere = false;
bool HTMwr::FileNameHere = false;
bool HTMwr::UseRawName = false;
bool HTMwr::KeepFileNameUnderscores = false;
bool HTMwr::ChangeFileNameSpaces = false;
bool HTMwr::KeepFileNameSpaces = false;
bool HTMwr::UseFilePxSxInBaseName = false;
unc HTMwr::FileNameSpaceChar = '_';
unc HTMwr::NameBuf[NameBufMax];
uns HTMwr::NameBufCount = 0;
char *HTMwr::NamePrefix = NULL;
char *HTMwr::NameSuffix = NULL;
char *HTMwr::TitlePrefix = NULL;
char *HTMwr::TitleSuffix = NULL;
char *HTMwr::MetaName = NULL;
uns HTMwr::MetaCount = 0;
DCnlist HTMwr::MetaList;
DCnlist HTMwr::MetaNameList;
DCvlist HTMwr::MetaPartList;
bool HTMwr::StoreText = false;
bool HTMwr::TStoreHere = false;
char *HTMwr::TStoreName = NULL;
bool HTMwr::ConfigHere = false;
bool HTMwr::AllowTbTitle = false;
bool HTMwr::TitleSet = false;
unc HTMwr::TitleBuf[TitleBufMax];
uns HTMwr::TitleBufCount = 0;
bool HTMwr::SkipAnums = false;
bool HTMwr::StripTitleSpaces = true;

bool HTMwr::FinalIniNameHere = false;
bool HTMwr::IniPartNameHere = false;
DCnlist HTMwr::FinalIniNameList;
DCnlist HTMwr::IniPartNameList;
char *HTMwr::IniPartName = NULL;
char *HTMwr::FinalIniName = NULL;
DCirfile *HTMwr::FinalIniFile = NULL;
DCirfile *HTMwr::FinalIniParent = NULL;

bool HTMwr::SearchHere = true;
bool HTMwr::PrintHere = true;
bool HTMwr::PrintOnly = false;
bool HTMwr::PrintProject = false;
bool HTMwr::DescHyph = false;



void
HTMwr::ProcTitleText(HTMctl *cp)
{
	uns wch = 0;
	uns uch = 0;
	unl i = 0;
	unc *cpt = NULL;

  if (cp->dt() == imnem) {  // single char form
    wch = (uns) cp->siz();

		if (wch == 173) {
			DescHyph = true;
			return;
		}
		DescHyph = false;

		if (AnsiMap
		 && (MIFVer < 8)
		 && !UseCodePage)
			wch = MapAnsiChar(wch);

		// set uch for standard Frame chars
		if (!UseCodePage)
			switch (wch) {
				case 130:
					uch = 0x201A;
					break;
				case 145:
					uch = 0x2018;
					break;
				case 146:
					uch = 0x2019;
					break;
				case 132:
					uch = 0x201E;
					break;
				case 147:
					uch = 0x201C;
					break;
				case 148:
					uch = 0x201D;
					break;
				case 149:
					uch = 0x2022;
					break;
				case 150:
					uch = 0x2013;
					break;
				case 151:
					uch = 0x2014;
					break;
				default:
					break;
			}

		if (IniFile) {
			// set up MapChar
			itoa(wch, MCDec, 10);
			if (uch)  // always 4 hex digits
				itoa(uch, MCHexU + 1, 16);
			else if (wch > 0xFFF)
				itoa(wch, MCHexU + 1, 16);
			else if (wch > 0xFF) {
				MCHexU[1] = '0';
				itoa(wch, MCHexU + 2, 16);
			}
			else if (wch > 0xF) {
				MCHexU[1] = '0';
				MCHexU[2] = '0';
				itoa(wch, MCHexU + 3, 16);
			}
			else {
				MCHexU[1] = '0';
				MCHexU[2] = '0';
				MCHexU[3] = '0';
				itoa(wch, MCHexU + 4, 16);
			}
			itoa(wch, MCHex4 + 3, 16);
			MapChar.cnt = 3;

			if (wch <= 0xFF) {
				if (wch <= 0xF) {
					MCHex2[1] = '0';
					itoa(wch, MCHex2 + 2, 16);
				}
				else
					itoa(wch, MCHex2 + 1, 16);
				MapChar.cnt++;
				if (wch != ' ') {
					*MCChr = wch & 0xFF;
					MapChar.cnt++;
				}
			}

			if ((Curr.fmap
				&& IniFile->Section(Curr.fmap)
				&& IniFile->FindAny(&MapChar))
			 || (IniFile->Section("CharConvert")
				&& IniFile->FindAny(&MapChar))) {  // mapped
				char *cpt = IniFile->StrRight();
				if (!cpt)
					return;
				if (isdigit(*cpt)) {
					DeleteName(cpt);
					wch = (uns) IniFile->IntRight();
					if (wch == 0)
						return;
				}
				else if ((tolower(*cpt) == 'x')
				 && isxdigit(*(cpt + 1))) {
					wch = (uns) strtoul(cpt + 1, NULL, 16);
					DeleteName(cpt);
				}
				else if ((tolower(*cpt) == 'u')
				 && (*(cpt + 1) == '+')
				 && isxdigit(*(cpt + 2))) {
					wch = (uns) strtoul(cpt + 2, NULL, 16);
					DeleteName(cpt);
				}
				else {
					char *cppt = NULL;
					char *cpps = NULL;
					cppt = cpps = DCmacr::GetMacro(cpt);
					while (*cppt)
						PutTitleBuf(*cppt++);
					DeleteName(cpt);
					DeleteName(cpps);
					return;
				}
			}
		}

    switch (wch) {
      case 0xA0:
				if (UseCodePage)
					goto normal;
      case 0x2000:
      case 0x2001:
      case 0x2002:
      case 0x2003:
      case 0x2004:
      case 0x2005:
      case 0x2006:
      case 0x2007:
      case 0x2008:
      case 0x2009:
      case 0x200A:
				PutTitleBuf(' ');
        break;
      case '&':
      case '<':
      case '>':
				if (AttrHere || InRaw)
					PutTitleBuf((unc) wch);
				else
					PutTitleBufEnt((unc) wch);
        break;
      case 0xAD:
				if (UseCodePage)
					goto normal;
				break;
      case 0x2011:
				PutTitleBuf('-');
        break;
      case 0x2044:
				PutTitleBuf('/');
        break;

			case 130:
			case 145:
			case 146:
				if (UseCodePage)
					goto normal;
				if (ValidOnly
				 || (HelpType == 'J'))
					wch = '\'';
				goto normal;

			case 132:
			case 147:
			case 148:
				if (UseCodePage)
					goto normal;
				if (ValidOnly
				 || (HelpType == 'J'))
					wch = '"';
				goto normal;

			case 149:
				if (UseCodePage)
					goto normal;
				if (ValidOnly)
					wch = 183;
				goto normal;

			case 150:
			case 151:
				if (UseCodePage)
					goto normal;
				if (ValidOnly
				 || (HelpType == 'J'))
					wch = '-';
				goto normal;

			normal:
      default:
				if (UseCodePage) {
					if (MIFVer < 8) {  // already using Windows code page
						if (wch > 0xFF)  // ignore other syms for now
							break;
					}
					else if (wch > 0x7F) {  // Unicode, map to code page
						if ((wch = (uns) DCicu::GetUnicodeEquiv(uch ? uch : wch)) == 0)
							break;
					}
					PutTitleBuf((unc) (wch & 0xFF));
					if (wch > 0xFF)  // two byte return
						PutTitleBuf((unc) ((wch >> 8) & 0xFF));
				}
				else if (wch > 0x7E) {
					if (!ValidOnly || (wch >= 160))
						PutTitleBufSpec(wch);
				}
        else
					PutTitleBuf((unc) wch);
        break;
    }
    return;
  }

  // string has only plain text
	DescHyph = false;
  for (i = cp->siz(), cpt = (unc *) cp->ex(); i && *cpt; i--, cpt++) {
    switch (*cpt) {
      case '&':
      case '<':
      case '>':
				if (AttrHere || InRaw)
					PutTitleBuf(*cpt);
				else
					PutTitleBufEnt(*cpt);
        break;
      default:
				PutTitleBuf(*cpt);
        break;
    }
  }
}

void
HTMwr::ProcTitleVarText(HTMctl *cp)
{
	if ((cp->maj() == 2)
	 && (cp->min() == 10)
	 && (cp->dat() == 1)
	 && ((cp->dt() == imnem)
	  || (cp->dt() == etext)))
		ProcTitleText(cp);
}

void
HTMwr::ProcTitleVar(uns vnum, bool macvar)
{
	char *nm = NULL;
	char *txt = NULL;

	if (macvar
	 && DCmacr::ReplacingFrameVars()
	 && ((nm = VarNameList.find(vnum)) != NULL)
	 && ((txt = DCmacr::GetUserMacVar(nm, false)) != NULL)
	 && ((TitleBufCount + strlen(txt)) < TitleBufMax)) {
		strcpy((char *) &TitleBuf[TitleBufCount], txt);
		TitleBuf[(TitleBufCount += strlen(txt))] = '\0';
		return;
	}

	HTMgroup* gp = (HTMgroup*) VarList.find(vnum);
	if (gp)
		gp->execu(ProcTitleVarText);
}


char *
HTMwr::ProcOutputText(HTMctl *cp)
{
  // borrow TitleBuf to get text out
	uns tcount = TitleBufCount;
	char *txt = (char *) &TitleBuf[tcount];
	StripTitleSpaces = false;
	bool nhere = NameHere;
	NameHere = false;
	ProcTitleText(cp);
	NameHere = nhere;
	StripTitleSpaces = true;
	uns ncount = TitleBufCount - tcount;
	TitleBufCount = tcount;
	return (ncount ? NewName(txt, ncount) : NULL);
}


void
HTMwr::PutTitleBuf(unc ch)
{
	if ((ch == ' ')
	 && StripTitleSpaces
	 && ((TitleBufCount == 0)
	  || (TitleBuf[TitleBufCount - 1] == ' ')))
		return;

	if (TitleBufCount < TitleBufMax)
		TitleBuf[TitleBufCount++] = ch;

#if 0
	if ((ch == ' ')
	 && FileNameHere
	 && (!KeepFileNameSpaces || ChangeFileNameSpaces)
	 && ((NameBufCount == 0)
	  || (NameBuf[NameBufCount - 1] == ' ')))
		return;
#endif

	if (NameHere
	 && (isalnum(ch)
	  || (FileNameHere && (ch == '_') && KeepFileNameUnderscores)
	  || (FileNameHere && (ch == ' ') && KeepFileNameSpaces)
	  || (FileNameHere && (ch == ' ') && ChangeFileNameSpaces
	   && ((ch = FileNameSpaceChar) != '\0')))
	 && (NameBufCount < NameBufMax))
		NameBuf[NameBufCount++] = ch;
}


void
HTMwr::PutTitleBufSpec(uns ch)
{
	char buf[16];
	size_t len = 0;

	if (NumericCharRefs
	 || (NoEnc
	  && (ch > 0xFF))
	 || !(NoEnc || UTF8)) {
	//if (!(NoEnc || UTF8)
	// && (NumericCharRefs
	// || (ch > 0xFF))) {
		sprintf(buf, "&#%0.3d;", ch);
		len = strlen(buf);
		if (TitleBufCount < (TitleBufMax - len)) {
			strcpy((char *) &TitleBuf[TitleBufCount], buf);
			TitleBufCount += len;
		}
	}
	else if (NoEnc) {
		if (TitleBufCount < TitleBufMax)
			TitleBuf[TitleBufCount++] = (unc) ch;
	}
	else if (UTF8) {
		unl uch = ch;
		switch (ch) {
			case 130:
				uch = 0x201A;
				break;
			case 145:
				uch = 0x2018;
				break;
			case 146:
				uch = 0x2019;
				break;
			case 132:
				uch = 0x201E;
				break;
			case 147:
				uch = 0x201C;
				break;
			case 148:
				uch = 0x201D;
				break;
			case 149:
				uch = 0x2022;
				break;
			case 150:
				uch = 0x2013;
				break;
			case 151:
				uch = 0x2014;
				break;
			default:
				uch = ch;
				break;
		}

		if (((uch < 0x0800)
		  && (TitleBufCount < (TitleBufMax - 2)))
		 || ((uch < 0x10000)
		  && (TitleBufCount < (TitleBufMax - 3)))
		 || (TitleBufCount < (TitleBufMax - 4)))
			TitleBufCount = ::UTF8(uch, TitleBuf + TitleBufCount) - TitleBuf;
	}


	if (NameHere
	 && (NoEnc
	  || ((ch < 0x80)
	   && (isalnum(ch)
	    || (FileNameHere && (ch == '_') && KeepFileNameUnderscores)
	    || (FileNameHere && (ch == ' ') && KeepFileNameSpaces)
	    || (FileNameHere && (ch == ' ') && ChangeFileNameSpaces
  	   && ((ch = FileNameSpaceChar) != '\0'))))
	  || ((ch >= 0xC0)
	   && ((ch - 0xC0) < EuroCharMax)
	   && (EuroChars[ch - 0xC0] != 0))
		|| (ch == 0xC6) || (ch == 0xDF) || (ch == 0xE6))
	 && (NameBufCount < NameBufMax))
		NameBuf[NameBufCount++] = (unc) ch;
}


void
HTMwr::PutTitleBufEnt(unc ch)
{
	char *str = NULL;
	short val = 0;

	switch (ch) {
		case '<':
			str = "&lt;";
			val = 4;
			break;
		case '>':
			str = "&gt;";
			val = 4;
			break;
		case '&':
			str = "&amp;";
			val = 5;
			break;
		default:
			PutTitleBufSpec(ch);
			return;
	}

	if (TitleBufCount < (TitleBufMax - val)) {
		strcpy((char *) &TitleBuf[TitleBufCount], str);
		TitleBufCount += val;
	}
}


char *
HTMwr::GetTitleBuf(bool mem)
{
	if (TitleBufCount == 0)
		return NULL;

	while (TitleBufCount > 1) {
		if ((TitleBuf[TitleBufCount - 1] == '\0')
		 || (TitleBuf[TitleBufCount - 1] == ' '))
			TitleBufCount--;
		else
			break;
	}

	char *cpt = NewName((char *) TitleBuf, TitleBufCount, mem);
	return cpt;
}



// OmniHelp search term processing

DCnlist HTMwr::SearchWordList;  // string is word, num is DCilist * to contents ids
long HTMwr::SearchWordMin = 3;
DCnlist HTMwr::StopWordList;    // string is word, num is StopWordCount when added
long HTMwr::StopWordCount = 0;
bool HTMwr::UseListButton = true;
bool HTMwr::UseSearchHighlight = true;
char *HTMwr::SearchHighlightStyle = "background-color:yellow;";
bool HTMwr::UseDefaultStopWords = true;
char *HTMwr::DefStopWords[] = {
 "about", "after", "again", "all", "already", "also", "always", "and", "any", "are", 
 "been", "but", "can", "did", "does", "doing", "each", "for", "from", "has", "have",
 "having", "its", "may", "maybe", "might", "not", "see", "than", "that", "the", "their",
 "them", "then", "these", "they", "this", "those", "too", "use", "used", "uses",
 "using", "very", "want", "was", "when", "where", "which", "will", "with", "would",
 "you", "your", NULL };

bool HTMwr::UnicodeFTS = false;
char *HTMwr::UnicodeLocale = "en-US";
char *HTMwr::CompoundWordChars = ":-._+*";


void
HTMwr::InitOHSearch(void)
{
	char *txt = NULL;
	bool inWord = false;
	bool starting = true;

	if (UseDefaultStopWords) {
		for (int i = 0; DefStopWords[i] != NULL; i++)
			StopWordList.add(DefStopWords[i], ++StopWordCount);
	}

	// set up StopWordList from [StopWords] section
	if (IniFile
	 && IniFile->Section("StopWords"))
		txt = IniFile->SecCont(true);

	if (txt) {
		for (char *chp = txt; *chp; chp++) {
			if ((starting && (*chp == ';'))
			 || ((*chp == '\n') && (*(chp + 1) == ';'))) {
				while (*chp && (*chp != '\n'))
					chp++;
			}
			starting = false;
			if ((*chp == ' ')
			 || (*chp == '\t')
			 || (*chp == '\r')
			 || (*chp == '\n')
			 || (*chp == ',')) {
				if (inWord) {
					StopWordList.add(txt, ++StopWordCount);
					inWord = false;
				}
				*chp = '\0';
			}
			else {
				if (!inWord) {
					inWord = true;
					txt = chp;
				}
			}
		}
		if (inWord)
			StopWordList.add(txt, ++StopWordCount);
	}

	if (UnicodeFTS)
		DCicu::SetUpWordBreak(UnicodeLocale);
}


void
HTMwr::ProcOHUnicodeSearch(char *txt, long id)
{
	if (!txt || !strlen(txt))
		return;
	DClist *dl = DCicu::MakeWordList(txt, strlen(txt));
	char *sw = NULL;
	long inum = 0;
	DClist *join = new DClist();

	if (dl->next) {
		for (DClist *item = dl->next; item; item = item->next) {
			inum++;
			sw = (char *) item->id;
			if ((*sw == ' ')
			 && (*(sw + 1) == '\0'))
				continue;
			ProcUnicodeSearchWord(sw, id);
			if ((inum > 1)
			 && (*(sw + 1) == '\0')
			 && strchr(CompoundWordChars, *sw))
				join->append(inum);
		}
		if (join->next) {
			for (DClist *item = join->next; item; item = item->next) {
				char *cp1 = (char *) dl->number(item->id - 1);
				char *cp2 = (char *) dl->number(item->id);
				char *cp3 = (char *) dl->number(item->id + 1);
				if ((!cp1 || !cp2 || !cp3)
				 || (*cp1 == ' ')
				 || (*cp3 == ' '))
					continue;
				long len = strlen(cp1) + strlen(cp2) + strlen(cp3);
				char *str = NewName(cp1, len);
				strcat(str, cp2);
				strcat(str, cp3);
				ProcUnicodeSearchWord(str, id);
			}
			join->empty();
		}
		dl->empty();
	}
	delete dl;
}


void
HTMwr::ProcOHSearch(char *txt, long id)
{
	bool inWord = false;
	char *fhword = NULL;

	// for all words in txt, add id to its id list via SearchWordList
	// if word is not in StopWordList, add it and create id list for it
	for (char *chp = txt; *chp; chp++) {
		if (inWord) {  // check for word endings
			if ((*chp == ' ')
			 || (*chp == '\t')
			 || (*chp == '\r')
			 || (*chp == '\n')) {
				*chp = '\0';
				ProcSearchWord(txt, id);
				if (fhword) {
					txt = fhword;
					ProcSearchWord(txt, id);
				}
				inWord = false;
				fhword = NULL;
			}
			else if (strchr(CompoundWordChars, *chp)) {
				// hyphen and others, processed two ways
				// processed here for part before hyphen, and at end
				// both as last part and as whole hyphenated sequence
				char tch = *chp;
				*chp = '\0';
				if (!fhword)
					fhword = txt;
				ProcSearchWord(txt, id);
				*chp = tch;
				txt = chp + 1;
			}
			else if ((__isascii(*chp) && ispunct(*chp))
			 || ((*chp > 0x7A) && (*chp < 0xC0))
			 || ((*chp >= 0xC0)
			  && ((*chp - 0xC0) < EuroCharMax)
			  && (EuroChars[*chp - 0xC0] == 0)
			  && (*chp != 0xC6)
			  && (*chp != 0xDF)
			  && (*chp != 0xE6))) { // other punct ends word
				*chp = '\0';
				ProcSearchWord(txt, id);
				if (fhword) {
					txt = fhword;
					ProcSearchWord(txt, id);
				}
				inWord = false;
				fhword = NULL;
			}
		}
		else if ((__isascii(*chp) && isalpha(*chp))
		 || ((*chp >= 0xC0)
	   && ((*chp - 0xC0) < EuroCharMax)
	   && ((EuroChars[*chp - 0xC0] != 0)
		  || (*chp == 0xC6)
		  || (*chp == 0xDF)
		  || (*chp == 0xE6)))) { // !inWord, letter starts word
			inWord = true;
			txt = chp;
		}
	}

	if (inWord) {  // final bit
		ProcSearchWord(txt, id);
		if (fhword) {
			txt = fhword;
			ProcSearchWord(txt, id);
		}
	}
}


void
HTMwr::ProcUnicodeSearchWord(char *txt, long id)
{
	// if word is not in StopWordList, add it and create id list for it
	if ((strlen(txt) < (size_t) SearchWordMin) || StopWordList(txt))
		return;

	char *word = NewName(txt);  //will only get shorter
	unc *rpt = (unc *) word;
	unc *wpt = (unc *) word;

	if (UTF8) {  // already UTF-8, make into code page for search list
		while (*rpt) {
			if (*rpt > 0x7F) {  // Unicode, map to code page
				unl wch = ::UTF8(&rpt); // advances rpt to next char
				unl lch = DCicu::GetUnicodeLower(wch);
				if (lch != wch)
					wch = lch;
				if ((wch = DCicu::GetUnicodeEquiv(wch)) != 0) {
					*wpt++ = ((unc) (wch & 0xFF));
					if (wch > 0xFF)  // two byte return
						*wpt++ =((unc) ((wch >> 8) & 0xFF));
				}
				else
					rpt++;
			}
			else  // must be ascii
				*wpt++ = (unc) tolower((char) *rpt++);
		}
		*wpt = '\0';
	}
	else {
		for (char *cpt = word ; *cpt ; cpt++)
			if (__isascii(*cpt))
				*cpt = tolower(*cpt);
	}

	DClist *pt = NULL;

	if ((pt = (DClist *) SearchWordList(word)) == NULL)
		SearchWordList.addsortlex(word, (long) (pt = new DClist), cmplexstrs);
	pt->add(id);
}


int
HTMwr::cmplexstrs(const char *s1, const char *s2)
{
	return (CompareString(LOCALE_SYSTEM_DEFAULT, 
	                       NORM_IGNORECASE | NORM_IGNOREKANATYPE | NORM_IGNOREWIDTH,
                         s1, -1, s2, -1) - 2);
}


void
HTMwr::ProcSearchWord(char *txt, long id)
{
	DClist *pt = NULL;
	char *word = NULL;
	char *cpt = NULL;

	// if word is not in StopWordList, add it and create id list for it
	if ((strlen(txt) < (size_t) SearchWordMin) || StopWordList(txt))
		return;
	if ((pt = (DClist *) SearchWordList(txt)) == NULL) {
		word = NewName(txt);
		for (cpt = word ; *cpt ; cpt++)
			if (__isascii(*cpt))
				*cpt = tolower(*cpt);
		SearchWordList.addsorted(word, (long) (pt = new DClist));
	}
	pt->add(id);
}



// text inset processing

long HTMwr::TextInsetNum = 0;
long HTMwr::TextInsetLev = 0;
DCilist HTMwr::ParaStartsTextInset;
DCilist HTMwr::ParaEndsTextInset;
DCilist HTMwr::TextInsetStack;
DCilist HTMwr::TextInsetStartingPara;
DClist HTMwr::TextInsetStartsPara;
DCilist HTMwr::TextInsetEndingPara;
DCnlist HTMwr::TextInsetName;
DCnlist HTMwr::TextInsetFile;
DClist HTMwr::TextInsetMainFlow;
DCilist HTMwr::TextInsetFlowPg;
DCnlist HTMwr::TextInsetFlowTag;
bool HTMwr::RemoveTextInsets = false;
bool HTMwr::TextInsetEnding = false;
bool HTMwr::TextInsetMark = false;
bool HTMwr::TextInsetNest = false;


void
HTMwr::ProcStartTextInset(HTMctl *cp)
{
	TextInsetNum = cp->siz();
	if (!TextInsetLev)
		ParaStartsTextInset.append(TextInsetNum, ParaRefNum);
	TextInsetStack.append(TextInsetNum, ++TextInsetLev);
	TextInsetStartingPara.append(ParaRefNum, TextInsetNum);
	if (ParaStartScan)
		TextInsetStartsPara.add(TextInsetNum);
}


void
HTMwr::ProcEndTextInset(HTMctl *cp)
{
	if (!TextInsetLev)
		return;
	TextInsetNum = cp->siz();
	TextInsetEndingPara.append(PrevParaRefNum, TextInsetNum);
	TextInsetStack.remove(TextInsetLev--);
	if (!TextInsetLev)
		ParaEndsTextInset.append(TextInsetNum, PrevParaRefNum);
	// text inset always ends at end of para
}


void
HTMwr::StartTextInset(HTMwfile *wf, HTMctl *cp)
{
	TextInsetNum = cp->siz();
	TextInsetLev++;
	if (!TextInsetNest
	 && (TextInsetLev > 1))
		return;  // nested, ignore
	//if (TextInsetStartsPara.find(TextInsetNum))
	//	return;  // already handled at para start

	//assert(ParaStartsTextInset.find(ParaRefNum) == TextInsetNum);
	if (!HXMwr::DITA
	 || !TextInsetMark)
		return;

	WriteText(wf);
	wf->startElem("data");

	char name[NumStrMax + 6];
	sprintf(name, "inset%d", TextInsetNum);
	wf->strAttr("datatype", "text_inset");
	wf->strAttr("name", name);
	wf->strAttr("value", "start");

	char *href = TextInsetFile.find(TextInsetNum);
	char *flow = NULL;
	bool ref = false;
	char *tref = NULL;

	if (!TextInsetMainFlow.find(TextInsetNum)) {
		flow = TextInsetFlowTag.find(TextInsetNum);
		if (flow) {
			ref = (TextInsetFlowPg.find(TextInsetNum) == 2);
			tref = NewName(href, strlen(href) + strlen(flow) + 3);
			strcat(tref, ref ? "#R" : "#B");
			strcat(tref, flow);
			href = tref;
		}
	}
	wf->strAttr("href", href);

	wf->strAttr("format", "fm");
	wf->strAttr("scope", "external");
	wf->endElem();
	if (tref)
		DeleteName(tref);
}


void
HTMwr::EndTextInset(HTMwfile *wf, HTMctl *cp)
{
	if (!TextInsetLev)
		return;

	TextInsetNum = cp->siz();
	TextInsetLev--;

	if (!TextInsetNest
	 && (TextInsetLev != 0))
		return;

	//assert(ParaEndsTextInset.find(PrevParaRefNum) == TextInsetNum);
	if (!HXMwr::DITA
	 || !TextInsetMark)
		return;

	WriteText(wf);
	wf->startElem("data");

	char name[NumStrMax + 6];
	sprintf(name, "inset%d", TextInsetNum);
	wf->strAttr("datatype", "text_inset");
	wf->strAttr("name", name);
	wf->strAttr("value", "end");
	wf->endElem();
}




// language conversions

uns *HTMwr::AnsiMap = NULL;

uns HTMwr::EE1250Map[AnsiLen] = {
     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0, 0x15A, 0x164, 0x17D, 0x179,
     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0, 0x15B, 0x165, 0x17E, 0x17A,

     0, 0x2C7, 0x2D8, 0x141,     0, 0x104,     0,     0,
     0,     0, 0x15E,     0,     0,     0,     0, 0x17B,
     0,     0, 0x2DB, 0x142,     0,     0,     0,     0,
     0, 0x105, 0x15F,     0, 0x13D, 0x2DD, 0x13E, 0x17C,

 0x154,     0,     0, 0x102,     0, 0x139, 0x106,     0,
 0x10C,     0, 0x118,     0, 0x11A,     0,     0, 0x10E,
 0x110, 0x143, 0x147,     0,     0, 0x150,     0,     0,
 0x158, 0x16E,     0, 0x170,     0,     0, 0x162,     0,

 0x155,     0,     0, 0x103,     0, 0x13A, 0x107,     0,
 0x10D,     0, 0x119,     0, 0x11B,     0,     0, 0x10F,
 0x111, 0x144, 0x148,     0,     0, 0x151,     0,     0,
 0x159, 0x16F,     0, 0x171,     0,     0, 0x163, 0x2D9
};


uns HTMwr::Cyr1251Map[AnsiLen] = {
 0x402, 0x403,     0, 0x453,     0,     0,     0,     0,
     0,     0, 0x409,     0, 0x40A, 0x40C, 0x40B, 0x40F,
 0x452,     0,     0,     0,     0,     0,     0,     0,
     0,     0, 0x459,     0, 0x45A, 0x45C, 0x45B, 0x45F,

     0, 0x40E, 0x45E, 0x408,     0, 0x490,     0,     0,
 0x401,     0, 0x404,     0,     0,     0,     0, 0x407,
     0,     0, 0x406, 0x456, 0x491,     0,     0,     0,
 0x451,0x2116, 0x454,     0, 0x458, 0x405, 0x455, 0x457,

 0x410, 0x411, 0x412, 0x413, 0x414, 0x415, 0x416, 0x417,
 0x418, 0x419, 0x41A, 0x41B, 0x41C, 0x41D, 0x41E, 0x41F,
 0x420, 0x421, 0x422, 0x423, 0x424, 0x425, 0x426, 0x427,
 0x428, 0x429, 0x42A, 0x42B, 0x42C, 0x42D, 0x42E, 0x42F,

 0x430, 0x431, 0x432, 0x433, 0x434, 0x435, 0x436, 0x437,
 0x438, 0x439, 0x43A, 0x43B, 0x43C, 0x43D, 0x43E, 0x43F,
 0x440, 0x441, 0x442, 0x443, 0x444, 0x445, 0x446, 0x447,
 0x448, 0x449, 0x44A, 0x44B, 0x44C, 0x44D, 0x44E, 0x44F
};


uns HTMwr::Gr1253Map[AnsiLen] = {
     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,

     0, 0x3F4, 0x386,     0,     0,     0,     0,     0,
     0,     0, 0x404,     0,     0,     0,     0,0x2015,
     0,     0,     0,     0, 0x3F3,     0,     0,     0,
 0x388, 0x389, 0x38A,     0, 0x38C,     0, 0x38E, 0x38F,

 0x390, 0x391, 0x392, 0x393, 0x394, 0x395, 0x396, 0x397,
 0x398, 0x399, 0x39A, 0x39B, 0x39C, 0x39D, 0x39E, 0x39F,
 0x3A0, 0x3A1,     0, 0x3A3, 0x3A4, 0x3A5, 0x3A6, 0x3A7,
 0x3A8, 0x3A9, 0x3AA, 0x3AB, 0x3AC, 0x3AD, 0x3AE, 0x3AF,

 0x3B0, 0x3B1, 0x3B2, 0x3B3, 0x3B4, 0x3B5, 0x3B6, 0x3B7,
 0x3B8, 0x3B9, 0x3BA, 0x3BB, 0x3BC, 0x3BD, 0x3BE, 0x3BF,
 0x3C0, 0x3C1, 0x3C2, 0x3C3, 0x3C4, 0x3C5, 0x3C6, 0x3C7,
 0x3C8, 0x3C9, 0x3CA, 0x3CB, 0x3CC, 0x3CD, 0x3CE,     0
};


uns HTMwr::Turk1254Map[AnsiLen] = {
     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,

     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,

     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,
 0x11E,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0, 0x130, 0x15E,     0,

     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,
 0x11F,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0, 0x131, 0x15F,     0
};


void
HTMwr::SetAnsiMap(char *txt)
{
	short num = (short) atol(txt);

	switch (num) {
		case 1250:
			AnsiMap = EE1250Map;
			break;
		case 1251:
			AnsiMap = Cyr1251Map;
			break;
		case 1253:
			AnsiMap = Gr1253Map;
			break;
		case 1254:
			AnsiMap = Turk1254Map;
			break;

		case 1252:
		default:
			AnsiMap = NULL;
			break;
	}
}


uns 
HTMwr::MapAnsiChar(uns ch)
{
	uns val = 0;

	if (AnsiMap
	 && (ch >= 0x80)
	 && (ch <= 0xFF)) {
		val = AnsiMap[(ch & 0xFF) - 0x80];
		if (val)
			return val;
	}

	return ch;
}



// end of dwhtmsp.cpp

