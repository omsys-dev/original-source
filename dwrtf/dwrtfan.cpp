 
// dwrtfan.cpp is the code for dcl RTF xrefs, autonums, ix, and ftn, JHG
// Copyright (c) 1993-1998 by Omni Systems, Inc.  All rights reserved.

#include "dcappl.h"
#include "dcctlg.h"
#include "dcfili.h"
#include "dctxfx.h"
#include "dcfmtr.h"
#include "dcini.h"
#include "dcmacr.h"
#include "dwrtf.h"
#include "dwrtffl.h"


// selection switch


bool
RTFwr::ProcAn(RTFctl *cp)
{
  switch((cp->rp)->scan_op) {
    case nothing:   // just put with its group for later
      break;

    case pass:      // ignore, don't save 
      return false;

		case anumscan:
			if (InFormDef) {
				if (InFormPx)
					Curr.anumpos = 1;
				else if (InFormSx)
					Curr.anumpos = 2;
				if ((XrfType == 'S')
				 || SeqAnums)
					ProcAnumDef(cp);
			}
			else if (!(InFormPx || InFormSx))
				ProcAnumVal(cp);   // store anum value as a group 
			return false;

    case formchk:
			if (InFormPx || InFormSx)
				return false;
      if (InGraphic) {
        ProcGr(cp);
        return false;
      }
      else if (InFormDef) {
        SetCurrProps(cp);
        return false;
      }  
      else if (InFormTagDef)
        SetCurrProps(cp);
			else switch ((cp->rp)->write_op) {
				case sethref:
					if (UseHL)
						ProcHyperMark(cp);
					break;
				case hyplnk:
					if (UseHL) {
						if (HelpForm)
							ProcHyperLink();
						else {
							char *txt = (char *) cp->ex();
							if (_strnicmp(txt + 1, "link:", 5))
								ProcHyperLink();
						}
					}
					break;
				case hypobj:
					ProcHyperObj(cp);
					break;
				case hyppgn:
					SetCurrPgn(cp);
					break;
				case hypunq:
					ProcHyperUnique(cp);
					break;
				default:
					break;
			}
      break;

		case setmark:
			ProcMarker(cp);
			return false;

		case varnum:
			if (StoreText
			 && (!WhiteText))
				ProcTextStoreVar((uns) cp->siz());
			break;

		case vardef:
			InVarDef = true;
			CurrVar = cp->dat();
			break;
		case varend:
			InVarDef = false;
			break;
		case varname:
			CheckVarName(cp);
			break;
		case varform:
			SetVarForm(cp); // add to var list
			break;

		case xrfmdef:  // start of xref format definition
      XrefID = (long) cp->dat();
			if (XrfType == 'S')
				CurrXRForm = cp->dat();
			break;
		case xrfname:
			ProcXrefFormatName(cp);
			return false;

		case xrform:	 // add to xrform list
			if (XrfType == 'S')
				SetXRForm(cp);
			break;
		case xrftok:	 // add to xrform token list
			if (XrfType == 'S')
				return SetXRToken(cp);
			break;
		case xrfval:	 // add to xrvalues list
			XrefInstID = (long) cp;
			if (XrfType == 'S')
				SetXRVal(cp);
			break;

		case getxref:	 // xr reference, get ref num
			if (XrfType == 'S')
				CurrXRStr = GetXRStr(cp, true);
			break;
		case numxrfm:	 // xr reference, get form num
			if (XrfType == 'S') {
				CurrXRForm = (short) cp->siz();
				XrefIDUseList.add(CurrXRForm, XrefInstID);
			}
			break;
		case usexrfm:	 // add xr reference ref num and form num to list
			XrefIni.nm[0] = XrefInstStr = itoa(XrefInstID, NewName(NumStrMax), 10);
			DCini::XrefSetList.all(DCini::SetXrefID);
			if (XrfType == 'S')
				SetXRRef();
			break;
		case setxloc:	 // record para start as containing xr src by ref num
			if (XrfType == 'S') {
				CurrXRStr = GetXRStr(cp);
				SetXRSrc();
			}
			break;
		case gtxfile: // proc xref file name so that correct px used
			if (XrfType == 'S')
				SetXrefFileName((char *) cp->ex());
			break;

    case ftnstart:
      if (FootType == 'N') {
        SkipCurrGroup = true;
        SkipLevel = DCLlevel;
        return false;
      }
			FtnParaEnding = false;
			InFootnote = true;
			// save para ref info for later graphics
			PreFtnParaStartCtl = TxtParaStartCtl;
			PreFtnTxtParaRefNum = TxtParaRefNum;
			PreFtnParaRefNum = ParaRefNum;
      break;
		case ftnend:
			ParaStartCtl = TxtParaStartCtl = PreFtnParaStartCtl;
			TxtParaRefNum = PreFtnTxtParaRefNum;
			ParaRefNum = PreFtnParaRefNum;
			FtnParaEnding = false;
			InFootnote = false;
			break;
    case ftnform:
      FtnForm = (uns) cp->siz();
      return false;

		case anumdef:
			InAnumStreamDef = true;
			AnumDefID = cp->dat();
			return false;
		case anumid:
			if (InAnumStreamDef)
				AnumNameList.add(NewName((char *)cp->ex(), cp->siz()), AnumDefID);
			break;
		case anumlvl:
			if (InAnumStreamDef)
				AnumLimitList.add(cp->siz(), AnumDefID);
			break;
		case anumstrm:
			break;
		case anumtype:
			switch (cp->siz()) {
				case 1:
					InPgnAnum = true;
					break;
				case 2:
					InFtnAnum = true;
					break;
				case 8:
					InChapNum = true;
					break;
				case 9:
					InVolNum = true;
					break;
				case 10:
					InSectionNum = true;
					break;
				case 11:
					InSSectNum = true;
					break;
				case 12:
					InTbFtnAnum = true;
					break;
				default:
					break;
			}
			return false;
		case anumdtok:
			break;
		case endanum:
			InAnumStreamDef = false;
      InPgnAnum = false;
      InFtnAnum = false;
      InTbFtnAnum = false;
			InChapNum = false;
 			InVolNum = false;
 			InSectionNum = false;
 			InSSectNum = false;
	    return false;

    case anumset:
      if (InFtnAnum == true)
        FtnStart = cp->dat();
      else if (InPgnAnum == true)
        PgnStart = cp->dat();
			else if (InChapNum == true)
				ChapNum = SetNumVal(ChapNumType, (ChapNumVal = cp->dat()));
			else if (InVolNum == true)
				VolNum = SetNumVal(VolNumType, (VolNumVal = cp->dat()));
			else if (InSectionNum == true)
				SectionNum = SetNumVal(SectionNumType, (SectionNumVal = cp->dat()));
			else if (InSSectNum == true)
				SSectNum = SetNumVal(SSectNumType, (SSectNumVal = cp->dat()));
			else if (InFormDef && SeqAnums)
				break;
      return false;

		case anumtxt:
			if (InChapNum == true)
				ChapNum = NewName((char *)cp->ex(), cp->siz());
			else if (InVolNum == true)
				VolNum = NewName((char *)cp->ex(), cp->siz());
			else if (InSectionNum == true)
				SectionNum = NewName((char *)cp->ex(), cp->siz());
			else if (InSSectNum == true)
				SSectNum = NewName((char *)cp->ex(), cp->siz());
			return false;

    case anumfmt:
      if (InFtnAnum == true) {
				FtnFormGroup = new RTFgroup;
			  if (DCLlevel < LevelMax)
					GroupStack[DCLlevel] = FtnFormGroup;
			}
      else if (InTbFtnAnum == true) {
				TbFtnFormGroup = new RTFgroup;
			  if (DCLlevel < LevelMax)
					GroupStack[DCLlevel] = TbFtnFormGroup;
			}
			InAnumDef = true;
      return false;
    case anumefmt:
			InAnumDef = false;
      return false;

    case anumrfmt:
      if (InFtnAnum == true) {
				FtnRFormGroup = new RTFgroup;
			  if (DCLlevel < LevelMax)
					GroupStack[DCLlevel] = FtnRFormGroup;
			}
      else if (InTbFtnAnum == true) {
				TbFtnRFormGroup = new RTFgroup;
			  if (DCLlevel < LevelMax)
					GroupStack[DCLlevel] = TbFtnRFormGroup;
			}
      return false;
    case anumform:
      if (InPgnAnum == true)
        SetPgnProps(cp);
			else if (InChapNum)
				ChapNumType = cp->dat();
			else if (InVolNum)
				VolNumType = cp->dat();
			else if (InSectionNum)
				SectionNumType = cp->dat();
			else if (InSSectNum)
				SSectNumType = cp->dat();
			else if (InFormDef
			 || InFtnAnum
			 || InTbFtnAnum)
				break;
      return false;

		case idxchk:
      if (IdxType == 'N') {
        SkipCurrGroup = true;
        SkipLevel = DCLlevel;
        return false;
      }
			InIdxGroup = true;
			break;

		case idxchke:
      if (IdxType == 'N')
        return false;
			InIdxGroup = false;
			break;

    default:
      assert(0);
      return false;
  }
  return true;
}


// list markers


DCnlist RTFwr::MarkList;
DCnlist RTFwr::MarkTypeList;
long RTFwr::MarkTypes = 0;


void
RTFwr::ProcMarker(RTFctl *cp)
{
	MarkName = (char *) cp->ex();
	char *txt = strchr(MarkName, ':');

	if (txt) {
		*txt = '\0';  // terminate type name
		txt++;        // point at marker text
		txt = NewName(txt);
	}

	if (IniFile
	 && IniFile->Section("MarkerTypes")
	 && IniFile->Find(MarkName)
	 && IniFile->ListRight()) {
		char *item = NULL;
		while ((item = IniFile->ListItem()) != NULL)
			ProcMarkerType(cp, item, txt);
	}
	else
		ProcMarkerType(cp, MarkName, txt);
}


char *RTFwr::MarkerProps[MarkerPropsMax] = {
 "Delete", "Code", "ALink",
 "HTMConfig", "Config", "RTFConfig",
 "ExtCodeStartLine", "ExtCodeEndLine", "ExtCodeFileLen",
 "ExtCodeStartChar", "ExtCodeEndChar", "ExtCodeFileEnc"
};

char *RTFwr::MarkName = NULL;
DCilist RTFwr::TextMarkerList;


void
RTFwr::ProcMarkerType(RTFctl *cp, char *kp, char *txt)
{
	short key;
	size_t len = 0;
	unc arg = 0;
	bool val = true;
	bool mtext = false;

//	if (!_strnicmp(kp, NoProp, len = strlen(NoProp))) {
//		val = false;
//		kp += len;
//	}

	// identify key
	for (key = 0; key < MarkerPropsMax; key++) {
		len = strlen(MarkerProps[key]);
		if (!_strnicmp(kp, MarkerProps[key], len))
			break;
	}
	//if (key == MarkerPropsMax)
	//	return;

	if (isdigit(*(kp + len)))
		arg = (unc) atoi(kp + len);

	switch (key) {
		case 0:  // Delete, do nothing
			break;

		case 1:  // Code
		case 2:  // ALink
			mtext = true;
			break;

		case 3: // HTMConfig, ignore it
			break;

		case 4: // Config
		case 5: // RTFConfig
			if (DCini::SetDirective(txt, (long) cp)) {
				key = 4;
				mtext = true;
			}
			break;

		case 6:  //ExtCodeStartLine,
		case 7:  //ExtCodeEndLine
		case 8:  //ExtCodeFileLen,
		case 9:  //ExtCodeStartChar
		case 10:  //ExtCodeEndChar,
		case 11:  //ExtCodeFileEnc
			mtext = true;
			break;

		default:
			break;
	}

	if (mtext) {
		// for text-containing or referencing markers,
		TextMarkerList.add((long) key, (long) cp); // id is ptr value
		// put in the ctl as a flag for later processing in write phase
		if (DCLlevel < LevelMax)
			GroupStack[DCLlevel]->add(*cp);
	}
}


void
RTFwr::WriteMarker(RTFwfile *wf, RTFctl *cp)
{
	long kind = TextMarkerList.find((long) cp);
	char *name = (char *) cp->ex();
	size_t len = strlen(name);
	char *txt = (cp->siz() > (len + 2)) ? name + len + 1 : "";
	char *code = NULL;
	setting *s = NULL;

	switch (kind) {

		case 1: // code
			WriteText(wf); // set props
			code = NewName(txt);
			AddMarkerCode(name, &code);
			DCmacr::WriteMacro(wf, code);
			DeleteName(code);
			break;

		case 2:  // alink
			PutHyperSubject(wf, cp);
			break;

		case 4:  // directive
			s = (setting *) DCini::SetList.find((long) cp);
			if (s != NULL)
				DCini::ProcMarkSect((char *) s->sect, (char *) s->key, (char *) s->val);
			break;

		case 6:  //ExtCodeStartLine
			ExtFileStartLine = strtoul(txt, NULL, 10);
			break;

		case 7:  //ExtCodeEndLine
			ExtFileEndLine = strtoul(txt, NULL, 10);
			break;

		case 8:  //ExtCodeFileLen
			ExtCodeFileLen = strtoul(txt, NULL, 10);
			break;

		case 9:  //ExtCodeStartChar
			ExtFileStartChar = strtoul(txt, NULL, 10);
			break;

		case 10:  //ExtCodeEndChar
			ExtFileEndChar = strtoul(txt, NULL, 10);
			break;

		case 11:  //ExtCodeFileEnc
			ExtCodeFileEnc = NewName(txt);
			break;

		default:
			break;
	}

}


void
RTFwr::AddMarkerCode(char *name, char **ptxt)
{
	char *codeb = NULL;
	char *codea = NULL;

	if (IniFile
	 && IniFile->Section("MarkerTypeCodeBefore")
	 && IniFile->Find(name))
		codeb = IniFile->StrRight();
	if (IniFile
	 && IniFile->Section("MarkerTypeCodeAfter")
	 && IniFile->Find(name))
		codea = IniFile->StrRight();

	if (!codeb
	 && !codea)
		return;

	char *code = NewName((codeb ? strlen(codeb) : 0) + strlen(*ptxt) +
		                   (codea ? strlen(codea) : 0) + 1);
	sprintf(code, "%s%s%s", (codeb ? codeb : ""), *ptxt,
		                      (codea ? codea : ""));
	DeleteName(codeb);
	DeleteName(codea);
	DeleteName(*ptxt);
	*ptxt = code;
}



// hypertext markers


bool RTFwr::HyperJump = false;
bool RTFwr::PopOver = false;
uns RTFwr::HyperVal = 0;
long RTFwr::HotDiv = 0;
long RTFwr::PreHotDiv = 0;
bool RTFwr::HyperStart = false;
bool RTFwr::HyperEnd = false;
bool RTFwr::LockHyper = false;

uns RTFwr::HyperPara = 0;
DCilist RTFwr::HyperParaList;

bool RTFwr::HyperForm = false;
bool RTFwr::HyperFormEnding = false;
long RTFwr::HyperFormStartID;
long RTFwr::HyperFormEndID;
DCilist RTFwr::HyperFormStartList;
DClist RTFwr::HyperFormEndList;


void
RTFwr::SetHyperStart(uns val)
{
	if (HelpForm) {
		if ((Curr.line != 2) // no override of popup
		 || (Curr.help == 0)
		 || NoXRPopup)
			Curr.line = (unc) val;
		Curr.lmode = 1;
	}
	else {
		if (HotDiv)
			SetHyperEnd();
		HyperStart = true;
		if (val == 2)
			PopOver = true;
	}

	Curr.hyper |= 1;  // hypertext marker zone
}


void
RTFwr::SetHyperEnd(void)
{
	Curr.hyper = 2;  // need to write ref str now
	if (HelpForm)
		Curr.line = 1;
	else {
		HyperEnd = true;
	}
}


void
RTFwr::CheckHyper(RTFctl *cp)
{
	if (HyperPara) {
		if (Curr.hyper == 0)
			SetHyperStart(HyperPara);
	}
	else {
		if (HyperFormEndList.find((long) cp))
			SetHyperEnd();
		uns val;
		if ((val = (uns) HyperFormStartList.find((long) cp)) != 0)
			SetHyperStart(val);
	}
}


void
RTFwr::CheckHyperEnd(RTFctl *cp)
{
	if (!HyperPara
	 && (HyperFormEndList.find((long) cp)
	  || HyperColorEndList.find((long) cp)))
		SetHyperEnd();
}


// hyper color interpretation


bool RTFwr::UseHyperColor = false;
bool RTFwr::HyperColor = false;
bool RTFwr::HyperColorEnding = false;
bool RTFwr::ParaHyperColor = false;
long RTFwr::HyperColorStartID;
long RTFwr::HyperColorEndID;
DCilist RTFwr::HyperColorStartList;
DClist RTFwr::HyperColorEndList;


void
RTFwr::ProcHyperColor(RTFctl *cp)
{
	// processing during scan phase
	if ((UseHyperColor == false)
	 //|| (HelpForm == false)
	 || HyperForm)
		return;

	if (cp->siz() > 1)  // color
		ProcHyperColorStart(cp);
	else if ((cp->siz() == 1)  // black
	 && HyperColor)     // was in color
		ProcHyperColorEnd(cp);
}


void
RTFwr::ProcHyperColorStart(RTFctl *cp)
{
	HyperColor = true;
	HyperColorEnding = false;
	HyperColorStartID = (long) cp;
	if (HyperJump)   // link was before start
		HyperColorStartList.add(HyperVal, HyperColorStartID);
}


void
RTFwr::ProcHyperColorEnd(RTFctl *cp)
{
	HyperColor = false;
	HyperColorEndID = (long) cp;
	if (HyperJump) {
		HyperColorEndList.add(HyperColorEndID);
		HyperJump = false;
	}
	// leave IDs in case HyperJump is right after
	HyperColorEnding = true;  // turn off after real text
}


void
RTFwr::ProcTextColor(RTFctl *cp)
{
	// processing during write phase
	GetColor(cp->siz(), &Curr.colr);
	if (InheritPending)
		InheritPendForm.colr = Curr.colr;
	if (HyperColorEndList.find((long) cp))
		SetHyperEnd();

	uns val;
	if ((val = (uns) HyperColorStartList.find((long) cp)) != 0)
		SetHyperStart(val);
}



// variable formats

bool RTFwr::InVarDef = false;
bool RTFwr::InVar = false;
uns RTFwr::CurrVar;
DCvlist* RTFwr::VarList = NULL;
DCnlist RTFwr::VarNameList;
uns RTFwr::TbContVar;
uns RTFwr::TbSheetVar;
bool RTFwr::TbContUse = false;
bool RTFwr::TbSheetUse = false;
char *RTFwr::TbContVarName = "Table Continuation"; 
char *RTFwr::TbSheetVarName = "Table Sheet"; 
bool RTFwr::InDateToken = false;
long RTFwr::DateDiv = 0;
long RTFwr::PreDateDiv = 0;
uns RTFwr::ParaVarType = 0;


void
RTFwr::SetVarForm(RTFctl* cp)
{
	if (VarList == NULL)
		VarList = new DCvlist;
	RTFgroup* gp = new RTFgroup;
	VarList->add(gp, CurrVar);
  if (DCLlevel < LevelMax)
    GroupStack[DCLlevel] = gp;
}


void
RTFwr::CheckVarName(RTFctl* cp)
{
	char *nm = (char *) cp->ex();
	if (!strcmp(nm, TbContVarName))
		TbContVar = CurrVar;
	if (!strcmp(nm, TbSheetVarName))
		TbSheetVar = CurrVar;
	VarNameList.addnew(nm, CurrVar);
}

void
RTFwr::ProcTextStoreVarText(RTFctl *cp)
{
	if ((cp->maj() == 2)
	 && (cp->min() == 10)
	 && (cp->dat() == 1)
	 && ((cp->dt() == imnem)
	  || (cp->dt() == etext)))
		ProcTextStoreText(cp);
}

void
RTFwr::ProcTextStoreVar(uns vnum, bool macvar)
{
	char *nm = NULL;
	char *txt = NULL;

	if (macvar
	 && DCmacr::ReplacingFrameVars()
	 && ((nm = (char *) VarNameList.find(vnum)) != NULL)
	 && ((txt = DCmacr::GetUserMacVar(nm, false)) != NULL)
	 && ((TextStoreBufCnt + strlen(txt)) < TextStoreBufMax)) {
		strcpy((char *) &TextStoreBuf[TextStoreBufCnt], txt);
		TextStoreBuf[(TextStoreBufCnt += strlen(txt))] = '\0';
		return;
	}

	RTFgroup* gp = (RTFgroup*) VarList->find(vnum);
	if (gp)
		gp->execu(ProcTextStoreVarText);
}


void
RTFwr::PutVarForm(RTFwfile* wf, RTFctl* cp)
{
	CurrVar = (uns) cp->siz();

	//if (InTbTitle == true) {
		if ((HelpForm == true)
		 && ((CurrVar == TbContVar)
		  || (CurrVar == TbSheetVar)))
			return;
		if ((CurrVar == TbContVar)
		 && (TbContUse == false))
			return;
		if ((CurrVar == TbSheetVar)
		 && (TbSheetUse == false))
			return;
	//}

	InVar = true;  // turned off by PutHyperUnique()
	WriteText(wf);  // set props

	char *txt = NULL;
	char *vname = NULL;
	if (((vname = (char *) VarNameList.find(CurrVar)) != NULL)
	 && DCmacr::ReplacingFrameVars()
	 && ((txt = DCmacr::GetUserMacVar(vname, false)) != NULL)) {
		wf->putText(*txt++);
		wf->putStr(txt);
		return;
	}

	SaveState();
	RTFgroup* gp = (RTFgroup*) VarList->find(CurrVar);
	if (gp)
		gp->write(*wf);
	WriteText(wf);
	RestoreState(CurrState);
	if (InDateToken) {
		// write start of field
		wf->SelectDiv(PreDateDiv);
		wf->putGS();
		wf->putCW("field");
		wf->putGS();
		wf->putCS('*');
		wf->putCW("fldinst");
		wf->putStr("DATE ");
		wf->putStr(" \\\\@ \"");
		// write field instructions
		wf->WriteDiv(DateDiv);
		wf->DeleteDiv(DateDiv);
		wf->SelectDiv(PreDateDiv);
		wf->putStr("\" \\\\* MERGEFORMAT ");
		wf->putGE();
		wf->putGS();
		wf->putCW("fldrslt");	   // empty result field
		wf->putGE();
		wf->putGE();
		DateDiv = 0;
		InDateToken = false;
	}
}


void
RTFwr::PutVarToken(RTFwfile* wf, RTFctl* cp)
{
	WriteText(wf);              // set props
	if (cp->dat() == 21) {      // page number
		switch (cp->siz()) {
			case 1:
				wf->putCW("chpgn");
				break;
			case 2:  // last page, use field
				wf->putField("numpages ", NULL, true, NULL, "1");
				break;
			default:
				break;
		}
	}
	else if (cp->dat() == 24) {  // doc number
		switch (cp->siz()) {
			case 1:  // chapter num
				if (ChapNum && *ChapNum)
					wf->putStr(ChapNum);
				else if (SectionPrefix && *SectionPrefix)
					wf->putStr(SectionPrefix);
				break;
			case 2:  // volume num
				if (VolNum && *VolNum)
					wf->putStr(VolNum);
				break;
			case 3:  // section num
				if (SectionNum && *SectionNum)
					wf->putStr(SectionNum);
				break;
			case 4:  // section num
				if (SSectNum && *SSectNum)
					wf->putStr(SSectNum);
				break;
			default:
				break;
		}
	}
	else if (cp->dat() == 20) {  // file name
		switch (cp->siz()) {
			case 1:  // file name only
				wf->putField("filename ", NULL, true, NULL, wf->getFileName());
				break;
			case 2:  // full path
				wf->putField("filename ", NULL, true, "\\\\p", wf->getFilePath());
				break;
			default:
				break;
		}
	}
	else if (cp->dat() == 30) {  // para
		ParaVarType = (uns) cp->siz();
		// 1=num_only, 2=num_str, 3=tag, 4=text, 5=page
	}
	else if (cp->dat() == 31) { // first para tag
		switch (ParaVarType) {
			case 1:
			case 2:
				if (SectionPrefix && *SectionPrefix)
					wf->putStr(SectionPrefix);
				break;
			case 3:
				wf->putStr(TmpStyleName(GetVarStyle((char *) cp->ex())));
				break;
			case 4:
				wf->putField("styleref ", TmpStyleName(GetVarStyle((char *) cp->ex())),
				              true, NULL, NULL);
				break;
			case 5:
				wf->putCW("chpgn");
				break;
			default:
				break;
		}
	}
	else if (cp->dat() == 32) { // last para tag
		switch (ParaVarType) {
			case 1:
			case 2:
				if (SectionPrefix && *SectionPrefix)
					wf->putStr(SectionPrefix);
				break;
			case 3:
				wf->putStr(TmpStyleName(GetVarStyle((char *) cp->ex())));
				break;
			case 4:
				wf->putField("styleref ", TmpStyleName(GetVarStyle((char *) cp->ex())),
				              true, "\\\\l", NULL);
				break;
			case 5:
				wf->putCW("chpgn");
				break;
			default:
				break;
		}
	}
	else if (cp->dat() < 16) {  // date or time
		if (InDateToken == false) {
			PreDateDiv = wf->GetDiv();
			DateDiv = wf->NewDiv();
			InDateToken = true;
		}
		switch (cp->dat()) {
			case 1:  // simple local time
				wf->putStr("DDD MMM dd HH:mm:ss yyyy");
				break;
			case 2:  // date
				wf->putStr(cp->siz() == 2 ? "MMMM d, yyyy" : "M/D/yy");
				break;
			case 3:  // year
				wf->putStr(cp->siz() == 2 ? "yyyy" : "yy");
				break;
			case 4:  // month
				switch (cp->siz()) {
					case 1:
						wf->putStr("MMM");
						break;
					case 2:
						wf->putStr("MMMM");
						break;
					case 3:
						wf->putStr("MM");
						break;
					case 4:
						wf->putStr("M");
						break;
					default:
						break;
				}
				break;
			case 6:  // day
				switch (cp->siz()) {
					case 1:
						wf->putStr("DDD");
						break;
					case 2:
						wf->putStr("DDDD");
						break;
					case 3:
						wf->putStr("DD");
						break;
					case 4:
						wf->putStr("D");
						break;
					default:
						break;
				}
				break;
			case 10:
				wf->putStr("h:mm AM/PM");  
				break;
			case 12:
				wf->putStr(cp->siz() == 2 ? "am/pm" : "AM/PM");
				break;
			case 13:
				switch (cp->siz()) {
					case 1:
						wf->putStr("hh");
						break;
					case 2:
						wf->putStr("h");
						break;
					case 3:
						wf->putStr("HH");
						break;
					case 4:
						wf->putStr("h");
						break;
					default:
						break;
				}
				break;
			case 14:
				wf->putStr(cp->siz() == 2 ? "m" : "mm");
				break;
			case 15:
				wf->putStr(cp->siz() == 2 ? "s" : "ss");
				break;
			default:
				break;
		}
	}
}


char *
RTFwr::GetVarStyle(char *sname)
{
	char *comma;
	if ((comma = strchr(sname, ',')) != NULL)
		*comma = '\0';

	long snum = 0;
	char *nname = NULL;
	if ((snum = FormRenameList(sname)) != 0)
		nname = (char *) FormNewNameList.find(snum);

	if (WriteMissingForms) {
		if (!snum)
			snum = FormNameList(sname);
		if (WriteAllVarForms
		 || !FormsFound.find(snum))
			AddMissingForm(snum);
	}

	return nname ? nname : sname;
}


char *
RTFwr::SetNumVal(uns ntyp, uns nval)
{
	char *nstr = NULL;
	char *tstr = NULL;
	char *tp = NULL;

	switch (ntyp) {
		case 2:  //arabic
		default:
			nstr = NewName(NumStrMax);
			sprintf(nstr, "%ld", nval);
			break;
		case 3:  // lc Roman
			tstr = NewName(RomanMax);
			tp = tstr;
			while (nval >= 1000) {
				*tp++ = 'm';
				nval -= 1000;
			}
			if (nval >= 900) {
				*tp++ = 'c';
				*tp++ = 'm';
				nval -= 900;
			}
			while (nval >= 100) {
				*tp++ = 'c';
				nval -= 100;
			}
			if (nval >= 50) {
				*tp++ = 'l';
				nval -= 50;
			}
			if (nval >= 40) {
				*tp++ = 'x';
				*tp++ = 'l';
				nval -= 40;
			}
			while (nval >= 10) {
				*tp++ = 'x';
				nval -= 10;
			}
			if (nval >= 9) {
				*tp++ = 'i';
				*tp++ = 'x';
				nval -= 9;
			}
			if (nval >= 5) {
				*tp++ = 'v';
				nval -= 5;
			}
			if (nval >= 4) {
				*tp++ = 'i';
				*tp++ = 'v';
				nval -= 4;
			}
			while (nval >= 1) {
				*tp++ = 'i';
				nval--;
			}
			*tp = '\0';
			nstr = NewName(tstr);
			DeleteName(tstr);
			break;
		case 4:  // uc Roman
			tstr = NewName(RomanMax);
			tp = tstr;
			while (nval >= 1000) {
				*tp++ = 'M';
				nval -= 1000;
			}
			if (nval >= 900) {
				*tp++ = 'C';
				*tp++ = 'M';
				nval -= 900;
			}
			while (nval >= 100) {
				*tp++ = 'C';
				nval -= 100;
			}
			if (nval >= 50) {
				*tp++ = 'L';
				nval -= 50;
			}
			if (nval >= 40) {
				*tp++ = 'X';
				*tp++ = 'L';
				nval -= 40;
			}
			while (nval >= 10) {
				*tp++ = 'X';
				nval -= 10;
			}
			if (nval >= 9) {
				*tp++ = 'I';
				*tp++ = 'X';
				nval -= 9;
			}
			if (nval >= 5) {
				*tp++ = 'V';
				nval -= 5;
			}
			if (nval >= 4) {
				*tp++ = 'I';
				*tp++ = 'V';
				nval -= 4;
			}
			while (nval >= 1) {
				*tp++ = 'I';
				nval--;
			}
			*tp = '\0';
			nstr = NewName(tstr);
			DeleteName(tstr);
			break;
		case 5:  // lc alpha
			if (nval) {
				nstr = NewName(2);
				*nstr = 'a' + (unc) ((nval - 1) % 26);
			}
			break;
		case 6:  // uc alpha
			if (nval) {
				nstr = NewName(2);
				*nstr = 'A' + (unc) ((nval - 1) % 26);
			}
			break;
		case 7:  // custom
			break;
	}
	return nstr;
}


// page numbering


bool RTFwr::InPgnAnum = false;
bool RTFwr::InChapNum = false;
bool RTFwr::InVolNum = false;
bool RTFwr::InSectionNum = false;
bool RTFwr::InSSectNum = false;
char *RTFwr::ChapNum = NULL;
char *RTFwr::VolNum = NULL;
char *RTFwr::SectionNum = NULL;
char *RTFwr::SSectNum = NULL;
long RTFwr::CurrPgn = 1;
uns RTFwr::PgnStart = 1;
uns RTFwr::PgnForm = 0;
uns RTFwr::ChapNumType = 0;
uns RTFwr::ChapNumVal = 0;
uns RTFwr::VolNumType = 0;
uns RTFwr::VolNumVal = 0;
uns RTFwr::SectionNumType = 0;
uns RTFwr::SectionNumVal = 0;
uns RTFwr::SSectNumType = 0;
uns RTFwr::SSectNumVal = 0;
char *RTFwr::PgnForms[PgnFormsMax] = {
 "pgndec", "pgnlcrm", "pgnucrm", "pgnlcltr", "pgnucltr"
};


void
RTFwr::SetPgnProps(RTFctl *cp)
{
  if ((PgnForm = cp->dat() - 2) >= PgnFormsMax)
    PgnForm = 0;
}

void
RTFwr::SetCurrPgn(RTFctl *cp)
{
	CurrPgn = (long) cp->siz();
}



// anumbers

DCvlist RTFwr::AnumDefList;
DCvlist RTFwr::AnumValList;
bool RTFwr::InAnumDef = false;
bool RTFwr::InAnumStreamDef = false;
uns RTFwr::AnumDefID = 0;
DCnlist RTFwr::AnumNameList;
DCilist RTFwr::AnumLimitList;
DCilist RTFwr::AnumSetList;
bool RTFwr::AnumPending = false;
bool RTFwr::WriteAnums = true;
bool RTFwr::SeqAnums = false;
bool RTFwr::AnumMergeformat = false;
bool RTFwr::WritingAnum = false;
bool RTFwr::WritingAnumVal = false;
bool RTFwr::WhiteAnumVal = false;
bool RTFwr::AnumUsedCS = false;
uns RTFwr::AnumTokenCount = 0;
uns RTFwr::AnumTokenLimit = 0;
char *RTFwr::AnumTokenName = NULL;
char *RTFwr::CurrAnumValStr = NULL;
size_t RTFwr::CurrAnumSepLen = 0;
uns RTFwr::CurrAnumToken = 0;
uns RTFwr::CurrAnumLevel = 0;
uns RTFwr::CurrAnumChange = 0;
uns RTFwr::CurrAnumSetLevel = 0;
uns RTFwr::CurrAnumVal = 0;
bool RTFwr::InTextPx = false;


void
RTFwr::ProcAnumDef(RTFctl* cp)
{
	RTFgroup* gp = new RTFgroup(*cp);
	GroupStack[DCLlevel] = gp;      // to collect anum def
  AnumDefList.add(gp, FormNum);		// for later retrieval
}

void
RTFwr::ProcAnumVal(RTFctl* cp)
{
	RTFgroup* gp = new RTFgroup(*cp);
  GroupStack[DCLlevel]->add(*cp); // to provide a trigger
	GroupStack[DCLlevel] = gp;      // to collect anum ctls
	if (WriteAnums)
	  AnumValList.add(gp, ParaRefNum);
}


void
RTFwr::PutAnumVal(RTFwfile *wf, bool atstart)
{
	if ((AnumPending == false)	// no anum in use
	 || (atstart && (Curr.anumpos == 2)))
		return;  // not time yet, wait for line break

	if (Curr.del)
		return;

	RTFgroup *avp = (RTFgroup *) AnumValList.find(ParaRefNum);
	if (!avp)
		return;

	AnumUsedCS = false;

	if (!Curr.del && Curr.macacb) {
		DCmacr::WriteMacro(wf, Curr.macacb);
		Curr.macacb = NULL;
	}

	if (XrfType != 'S') {
		bool inherit = InheritPending;
		InheritPending = false;
		WritingAnumVal = true;
		WhiteAnumVal = false;
		avp->write(*wf);
		WriteText(wf);
		WritingAnumVal = false;
		WhiteAnumVal = false;
		InheritPending = inherit;
	}
	else
		WriteAnumBkmks(wf, avp);

	if (!Curr.del && Curr.macaca) {
		DCmacr::WriteMacro(wf, Curr.macaca);
		Curr.macaca = NULL;
	}

	if (AnumUsedCS)
		RestoreInheritForm();
	AnumPending = false;
	ParaStarting = false;
}


void
RTFwr::WriteAnumBkmks(RTFwfile *wf, RTFgroup *avp)
{
	RTFgroup *adp = (RTFgroup *) AnumDefList.find(Curr.snum);
	bool bkmkreport = false;
	if (!adp)
		return;
	if (LogBookmarkCount
	 && (((BkmkstartCount - BkmkErrCount) != BkmkendCount)
	 || (BkmkstartCount > (LastBkmkCount + 100))))
		bkmkreport = true;
	WritingAnum = true;

	// at start of anum usage, must insert bookmarks
  // for each element in the anum definition
	// interspersed with the values actually used
	WriteProps(wf);
	WriteCondProps(wf);

	WriteHyperUnique(wf);
	long cdiv = wf->GetDiv();
	wf->SetCR(false);

	// write avp to div as reference
	long valdiv = wf->NewDiv();
	//SaveState();                  // save state of props
	//bool pstate = InPara;
	//bool spage = StartingPage;
	CurrAnumToken = 0;
	Last = Curr;                  // suppress props here
	InPara = true;
	uns lstartpos = LinePos;
	SaveState();                  // save state of props
	//wf->SelectDiv(valdiv);
	if (avp)
		avp->write(*wf);
	WriteText(wf);
	WriteTabs(wf);
	uns lendpos = LinePos;
	//InCharForm = 0;
	//wf->SelectDiv(valdiv);
	char *valp = CurrAnumValStr = wf->ReadDiv(); 	// get ptr to the div str
	size_t vallen = wf->LenDiv();
	wf->SelectDiv(cdiv);

	if (bkmkreport) {
		char numstart[20];
		char numend[28];
		sprintf(numstart, "%ld ", BkmkstartCount);
		sprintf(numend, "%ld before ", BkmkendCount);
		LogEvent(logwarn, 1, "Bookmarks started = ", numstart,
		                     ", ended = ", numend, valp);
		BkmkErrCount = BkmkstartCount - BkmkendCount;
		LastBkmkCount = BkmkstartCount;
	}

	if (!vallen) {
		RestoreState(FullState);      // set props back to start
		return;
	}

	// scan adp to get count of anum tokens
	RTFitem *ip = NULL;
	RTFctl *cp = NULL;
	AnumSetList.empty();
	AnumTokenCount= CurrAnumLevel = CurrAnumChange = CurrAnumSetLevel = CurrAnumVal = 0;
	while ((cp = adp->getnext(&ip)) != NULL) { // count levels in def
		if ((cp->rp->scan_op == anumform)
		 || (cp->rp->scan_op == anumdtok))
			AnumTokenCount++;
		if (cp->rp->scan_op == anumstrm) {
			AnumTokenName = (char *) AnumNameList.find(cp->siz());
			AnumTokenLimit = (uns) AnumLimitList.find(cp->siz());
		}
		if (cp->rp->scan_op == anumlvl)
			CurrAnumChange = (uns) cp->siz();
		if (cp->rp->scan_op == anumset) {
			CurrAnumSetLevel = (uns) cp->siz();
			CurrAnumVal = cp->dat();
			// offset in list to allow setting zero
			AnumSetList.add(CurrAnumVal + 1, CurrAnumSetLevel);
		}
	}

	// write start of anum str
	//RestoreState(FullState);      // set props back to start
	//InPara = pstate;
	//StartingPage = spage;
	//WriteProps(wf);
	//InPara = true;

	// adjust valp to get past any initial formatting
	char *ivp = NULL;
	size_t ivlen = 0;
	if (valp) {  // may have formatting
		ivp = valp;
		ivlen = GetNonTextLength(ivp);
		ivp += ivlen;
		CurrAnumValStr = ivp;  // so anum value can be found
	}

	// write adp to div, marking anum value with '\0'
	long defdiv = wf->NewDiv();
	LinePos = lstartpos;
	RestoreState(FullState);      // set props back to start
	if (adp)
		adp->write(*wf);
	LinePos = lendpos;
	char *defp = wf->ReadDiv(); 	// get ptr to the div str
	size_t deflen = wf->LenDiv(); //  and total real length (with null)
	size_t pxlen = (deflen && defp) ? strlen(defp) : 0;

	wf->SelectDiv(cdiv);

	if (ivlen
	 && (ivp >= (valp + ivlen))) {
		if ((pxlen >= ivlen) // match to formatting
		 && !strncmp(valp, defp, ivlen)) {
			pxlen -= ivlen;
			defp += ivlen;
		}
		wf->putStr(valp, ivlen);
		valp += ivlen;
	}

	XRStartAnumStr(wf);	// add str bookmark before prefix

	// write prefix of anum
	if (pxlen)
		wf->putStr(defp, pxlen);
	XRStartAnumNum(wf);	// add num bookmark after prefix

	// add anum value	from valp
	char *pxp;
	if (pxlen) {
		if (valp
		 && ((pxp = strstr(valp, defp)) != NULL))
			valp = pxp + pxlen;  // point after prefix
	}
	
	if (defp)
		defp += (pxlen + 1);   // for def string also

	if (CurrAnumValStr > valp) {  // because anum token took value off 
		if (*CurrAnumValStr == '~')
			CurrAnumValStr++;
		valp = CurrAnumValStr;
	}

	size_t seqlen = defp ? strlen(defp) : 0;
	char *seqp = defp;
	if (defp)
		defp += (seqlen + 1);

	size_t alen = valp ? strlen(valp) : 0;  // limit for num
	size_t sxlen = defp ? strlen(defp) : 0;

	char *sxp;
	if (sxlen
	 && valp
	 && defp
	 && ((sxp = strstr(valp, defp)) != NULL)) {  // match suffix
		alen = sxp - valp;
	}

	if (SeqAnums)               // write SEQ field ****
		wf->putStr(seqp, seqlen);
	else if (alen && valp) {
		if (!sxlen) {
			char *axp = valp;
			size_t axlen = 0;
			if (isdigit(*axp)) {
				while (isdigit(*axp)) {
					axp++;
					axlen++;
				}
			}
			else if (isalpha(*axp)) {
				while (isalpha(*axp)) {
					axp++;
					axlen++;
				}
			}
			if (axlen)
				alen = axlen;
		}
		wf->putStr(valp, alen);  // part before suffix is number
		valp += alen;
		CurrAnumValStr = valp;
	}

	XREndAnumNum(wf);

	// rest of num str is remaining part of val
	if (sxlen)
		wf->putStr(CurrAnumValStr, sxlen);

	if (valp
	 && defp
	 && sxlen
	 && !_strnicmp(defp, valp, sxlen))  // if matches, skip matched
		valp += sxlen;

	XREndAnumStr(wf);

	// remaining part, usually a tab, is after num str
	wf->putStr(CurrAnumValStr + sxlen);

	wf->DeleteDiv(defdiv);  // release divs in reverse order
	wf->DeleteDiv(valdiv);
	WritingAnum = false;
	wf->SetCR(true);
}


size_t
RTFwr::GetNonTextLength(char *ivp)
{
	size_t ivlen = 0;

	while (*ivp) {
		while (isspace(*ivp)) {
			ivp++;
			ivlen++;
		}
		if (*ivp == '{') {
			if (!strncmp(ivp, "{\\pict", 6)) {
				while (*ivp && (*ivp != '}')) {
					ivp++;
					ivlen++;
				}
			}
			ivp++;
			ivlen++;
		}
		else if (*ivp == '\\') {
			ivp++;
			ivlen++;
			if (isalnum(*ivp)) {
				while (isalnum(*ivp)
				 || (*ivp == '-')) {
					ivp++;
					ivlen++;
				}
			}
			else if (*ivp == '\'') {
				ivp += 3;
				ivlen += 3;
			}
			else {
				ivp++;
				ivlen++;
			}
		}
		else if (*ivp == '}') {
			ivp++;
			ivlen++;
		}
		else
			break;
	}
	return ivlen;
}


void
RTFwr::WriteAnumToken(RTFwfile *wf, RTFctl *cp)
{
	if (WritingFtnGroup) {
		WriteFtnAnumToken(wf, cp);
		return;
	}

	if (WritingAnum == false) {
		PutVarToken(wf, cp);
		return;
	}

	// may also be volnum and chapnum tokens
	static size_t divpos;

	if (++CurrAnumToken == 1) { // first one
		if (SeqAnums) {
			divpos = wf->LenDiv();
			CurrAnumValStr += divpos++;  // pos at first anum val plus nul
		}
		wf->putChar(0);  // mark start of value
	}

	if (SeqAnums) {               // write SEQ field
		bool field = false;
		bool keep = false;
		bool zero = false;
		bool hide = false;
		bool set = false;
		uns val = 0;
		char valstr[NumStrMax] = "0";
		uns anumtype = 0;
		uns aform = 0;
		uns anumlev = 0;
		CurrAnumSepLen = wf->LenDiv() - divpos;

		if (cp->rp->scan_op == anumdtok) {
			anumtype = (uns) cp->siz();
			field = true;
			keep = true;
			switch (anumtype) {
				case 1:  // chapnum
					aform = ChapNumType;
					break;
				case 2:  // volnum
					aform = VolNumType;
					break;
				case 3:  // sectionnum
					aform = SectionNumType;
					break;
				case 4:  // subsectionnum
					aform = SSectNumType;
					break;
				default:
					break;
			}
			if (aform > 1)
				aform -= 2;
			else
				aform = 0;
		}
		if (cp->rp->scan_op == anumform)
			anumlev = (uns) cp->siz();

		if (anumtype == 0) {
			if (anumlev)
				CurrAnumLevel = anumlev;
			else
				CurrAnumLevel++;

			if (CurrAnumLevel <= CurrAnumChange) // curr or higher level
				field = true;

			if (CurrAnumLevel != CurrAnumChange) // increment only curr level
				keep = true;

			//if (CurrAnumLevel == CurrAnumSetLevel) {
			if ((val = (uns) AnumSetList.find(CurrAnumLevel)) > 0) {
				val--;  // offset to allow setting zero
				set = true;
				//val = CurrAnumVal;
				field = true;
				keep = false;
			}

			if (cp->dat() == 1) {
				hide = true;
				if (!set)  // hidden, not needed unless it is being set
					field = false;
			}
			else {
				aform = cp->dat() - 2;
				if (aform == 7) // chapnum
					aform = ChapNumType - 2;
				else if (aform == 8) // volnum
					aform = VolNumType - 2;
				else if (aform == 9) // sectionnum
					aform = SectionNumType - 2;
				else if (aform == 10) // subsectionnum
					aform = SSectNumType - 2;
				field = true; // not hidden, put out even if lower level
			}
		}

		if (field)
			WriteAnumField(wf, CurrAnumLevel, anumtype, aform,
			               keep, hide, zero, set, val);
		else if (CurrAnumSepLen) {
			CurrAnumValStr += CurrAnumSepLen; // size of added text to def str
			CurrAnumSepLen = 0;
		}
		divpos = wf->LenDiv();  // pos after field written
	}

	if (CurrAnumToken == AnumTokenCount) { // last one
		if (SeqAnums
		 && (CurrAnumLevel < AnumTokenLimit)) {  
			// set any missing lower levels to 0 and hide them
			CurrAnumLevel++;
			CurrAnumSepLen = 0;
			for (; CurrAnumLevel <= AnumTokenLimit ; CurrAnumLevel++)
				WriteAnumField(wf, CurrAnumLevel, 0, 0, false, true, true);
		}
		wf->putChar(0);  // mark end of value
	}
}


void
RTFwr::WriteAnumField(RTFwfile *wf, uns lev, uns typ, uns aform,
                      bool keep, bool hide, bool zero, bool set, uns val)
{

	wf->putEnd();
	wf->putGS();
	wf->putCW("field");
	wf->putGS();
	wf->putCS('*');
	wf->putCW("fldinst");	 // field codes
		wf->putStr("SEQ "); // field name, always present
	if (typ == 0) {
		if (*AnumTokenName == '0')  // digit zero, not nul
			wf->putStr("XX");
		else
			wf->putStr(AnumTokenName);
		wf->putShort(lev);
	}
	else {  // chapnum is 1 and volnum is 2
		wf->putStr((typ == 1) ? "CHAP" : "VOL");
	}
	if (aform && !hide){
		switch (aform) {
			case 1:
				wf->putStr(" \\\\*roman");
				break;
			case 2:
				wf->putStr(" \\\\*ROMAN");
				break;
			case 3:
				wf->putStr(" \\\\*alphabetic");
				break;
			case 4:
				wf->putStr(" \\\\*ALPHABETIC");
				break;
			default:
				break;
		}
	}
	if (keep)
		wf->putStr(" \\\\c ");
	if (zero)
		wf->putStr(" \\\\r0 ");
	if (set) {
		wf->putStr(" \\\\r");
		wf->putShort(val);
	}
	if (hide)
		wf->putStr(" \\\\h ");
	else if (AnumMergeformat)
		wf->putStr(" \\\\* MERGEFORMAT ");
	wf->putGE();
	wf->putEnd();
	wf->putGS();
	wf->putCW("fldrslt");	   // result field
	wf->putGS();
	//wf->putChar(' ');
	if (!hide)
		PutAnumTokVal(wf, aform);
	else if (CurrAnumSepLen) {
		CurrAnumValStr += CurrAnumSepLen; // size of added text to def str
		CurrAnumSepLen = 0;
	}
	wf->putGE();
	wf->putGE();
	wf->putGE();
}


void
RTFwr::PutAnumTokVal(RTFwfile *wf, uns aform)
{
	CurrAnumValStr += CurrAnumSepLen; // size of added text to def str
	char *chp = CurrAnumValStr;

	// chop likely number off CurrAnumValStr
	switch (aform) {
		case 0:  // numeric
			while (*chp && isdigit(*chp))
				chp++;
			break;
		default: // alphabetic and roman
			while (*chp && isalpha(*chp))
				chp++;
			break;
	}

	size_t len = chp - CurrAnumValStr;
	if (len) {
		wf->putStr(CurrAnumValStr, len);
		CurrAnumValStr += len;
	}
}


void
RTFwr::WriteChapVolFields(RTFwfile *wf)
{
	WriteAnumField(wf, 0, 1, 0, false, true, false, true, ChapNumVal);
	WriteAnumField(wf, 0, 2, 0, false, true, false, true, VolNumVal);
	WriteAnumField(wf, 0, 3, 0, false, true, false, true, SectionNumVal);
	WriteAnumField(wf, 0, 4, 0, false, true, false, true, SSectNumVal);
}


// cross-references

char *RTFwr::XrefFile = NULL;
char *RTFwr::XrefRawFile = NULL;
char *RTFwr::XrefWindow = NULL;
char RTFwr::XrefHvis = '\0';
bool RTFwr::XrefMapped = false;
char *RTFwr::RefStr = NULL;
DCnlist RTFwr::RefStrList;
short RTFwr::RefLevel = 0;
bool RTFwr::IdxEnded = false;
long RTFwr::HyperUnique[HULevMax];
short RTFwr::HULevel = 0;
short RTFwr::AlertNum = 0;
char *RTFwr::IXnewlinkPrefix;

char *RTFwr::CurrFileName;
char *RTFwr::CurrFilePath;
char *RTFwr::CurrFileFull;
char *RTFwr::CurrPath;
char *RTFwr::GlobalXrefFileName;
DCnlist RTFwr::XRFNames;
DCnlist RTFwr::XRFNewNames;
long RTFwr::XRFCount = 0;

bool RTFwr::UseXrefFilePrefix = true;
char *RTFwr::CurrFilePrefix = NULL;
char *RTFwr::XrefFilePrefix = NULL;
char *RTFwr::XrefFileSuffix = NULL;
short RTFwr::XrefLenLimit = 63;
DCnlist RTFwr::FIDNames;
DCnlist RTFwr::FIDPrefixes;
long RTFwr::FIDCount = 0;


char *
RTFwr::StripSuf(char *name)
{
	char *suf;

	if (((suf = strrchr(name, '.')) != NULL)
	 && (suf > name))
		*suf = '\0';
	return name;
}


void
RTFwr::SetFIDLists(void)
{

	if (!UseLocalFileID
	 && IDFileName)
		return;

	if (IniFile
	 && IniFile->Section("FileIDs")) {
		while (IniFile->NextItem()) {
			FIDNames.add(StripSuf(IniFile->StrLeft()), ++FIDCount);
			FIDPrefixes.add(IniFile->StrRight(), FIDCount);
		}
	}	
}


char *
RTFwr::GetFIDPrefix(char *fname)
{
	long i;

	if (!UseLocalFileID
	 && IDFileName)
		return GetFileID(fname);

	if ((i = FIDNames(fname)) != 0)
		return (char *) FIDPrefixes.find(i);

	// no prefix, so make one up
	char *nn = NewName(fname);
	char *px = NULL;
	px = nn + (strlen(nn) - 3);
	while (FIDNames(px)  // already in use
	 && (px > nn))       // more of name available
		px--;              // use more of name
	FIDNames.add(nn, ++FIDCount);
	FIDPrefixes.add(px, FIDCount);
	return px;
}


void
RTFwr::SetXRFLists(char *secname)
{
	char *str;

	if (IniFile
	 && IniFile->Section(secname)) {
		while (IniFile->NextItem()) {
			str = IniFile->StrRight();
			if (*str == '\0') {  // empty new name
				delete str;
				continue;
			}
			XRFNames.add(StripSuf(IniFile->StrLeft()), ++XRFCount);
			XRFNewNames.add(StripSuf(str), XRFCount);
		}
	}	
}


char *
RTFwr::GetXrefName(char *fname)
{
	long i;

	if ((i = XRFNames(fname)) != 0)
		return (char *) XRFNewNames.find(i);

	if (GlobalXrefFileName)
		return GlobalXrefFileName;

	// no xref name listed
	return HelpForm ? CurrFileName : fname;
}


void
RTFwr::ProcHyperMark(RTFctl *cp)
{
	char *txt = (char *) cp->ex();

	if (!_strnicmp(txt, "alert", 5))
		ProcHyperLink(2);

	else if (!_strnicmp(txt, "gotolink", 8)
	 || !_strnicmp(txt, "openlink", 8))
		ProcHyperLink(3);
}


void
RTFwr::ProcHyperLink(uns val)
{
	// gotolink or openlink text

	if ((UseHL == false)
	 || Curr.del)
		return;

	HyperJump = true;

	if (!PopOver)  // may have been set for PopOver
		HyperVal = val;
	else val = HyperVal;

	if (HyperForm)
		HyperFormStartList.add(val, HyperFormStartID);
	else if (HyperFormEnding) {
		HyperFormStartList.add(val, HyperFormStartID);
		HyperFormEndList.add(HyperFormEndID);
		HyperJump = false;
	}
	else if (HyperColor)
		HyperColorStartList.add(val, HyperColorStartID);
	else if (HyperColorEnding) {
		HyperColorStartList.add(val, HyperColorStartID);
		HyperColorEndList.add(HyperColorEndID);
		HyperJump = false;
	}

	//HyperParaList.add(HyperVal, ParaRefNum);
}


void
RTFwr::ProcHyperObj(RTFctl *cp)
{
	// long object id to open
	// add it to database of required object IDs later
	ProcHyperLink(HelpForm ? 3 : 2);
}


bool RTFwr::MakeRef = false;
long RTFwr::CurrRef = 0;
DCilist RTFwr::MakeRefList;


void
RTFwr::ProcHyperUnique(RTFctl *cp)
{
	if (FrameStarting) {
		RFrame->id = (long) cp->siz();
		FrameStarting = false;
	}
	if (MakeRef) {
		CurrRef = (long) cp->siz();
		MakeRef = false;
	}
}


void 
RTFwr::PutHypertext(RTFwfile *wf, RTFctl *cp)
{
	if ((UseHL == false)
	 || Curr.del)
		return;

	// process FrameMaker gotolink, newlink, and openlink
	char *txt = (char *) cp->ex();
	short len = (short) cp->siz();
	char *ptr = NULL;
	static char str[128];

	if (!_strnicmp(txt, "gotolink", 8)    // FrameMaker specific
	 || !_strnicmp(txt, "openlink", 8)) {	// anchor href
		//if (Curr.hyper == 0)  // ensure hyper char format
		//	return;	// in Frame, this means entire para is active area
		if (RefStr)  // second hypertext, so put out previous one now
			PutRefStr(wf);
		txt += 9; // skip tag and following space
		len -= 9;	// may be "path/filename.ext:ref"
		if ((ptr = strrchr(txt, ':')) != NULL) { // strip filename
			if (!strncmp(ptr + 1, "firstpage", 9)		// nonunique names,
			 || !strncmp(ptr + 1, "lastpage", 8)) { // use filename as ref
				len = ptr - txt;
				if (len > 127)
					len = 127;
				strncpy(str, txt, len); // terminate there
				txt = str;
				if ((ptr = strrchr(str, '.')) != NULL)
					*ptr = 0;  // strip file suffix
				if ((ptr = strrchr(str, '/')) != NULL) // always uses / as sep
					txt = ptr + 1; // strip leading path
			}																
			else {  // use ref without filename, must be unique
        SetXrefFileName(txt, ptr - txt);
				txt = ptr + 1;
			}
			len = strlen(txt) + 1;
		}
		RefStr = NewName(txt, len);
		XrefWindow = Curr.hwindow;
		XrefHvis = Curr.hvis;
		// put into place as hidden text at end of char tag later
	}
	else if (!_strnicmp(txt, "newlink", 7)) {	 // anchor name
		txt += 8; // skip tag and following space
		WriteProps(wf);
		FtnMarker = '#';
		WriteFtnStart(wf);
		WriteFtnNum(wf);
		if (IXnewlinkPrefix)
			PutFtnContext(wf, IXnewlinkPrefix);
		PutFtnContext(wf, txt);
		WriteFtnEnd(wf);
	}
	else if (!_strnicmp(txt, "alert", 5)) {	 // treat as a popup
		if (!_strnicmp(txt, "alerttitle", 10)) {
			ptr = strchr(txt, ':');
			if (ptr == NULL)
				ptr = txt + 11;
			else ptr++;
		}
		else ptr = txt + 6;

		if (!HelpForm
		 || (*ptr == '~')) // PDF setting
			return;

		//if (Curr.hyper == 0)  // ensure hyper char format
		//	return;	// in Frame, this means entire para is active area
		if (RefStr)  // second hypertext, so put out previous one now
			PutRefStr(wf);

		// create ref to popup
		strcpy(str, wf->getBaseName());
		strcat(str, "Alert");
		len = strlen(str);
		sprintf(str + len, "%0.4d", ++AlertNum);
		RefStr = NewName(str);
			
		// write popup itself
		if (CurrSlider == 0)            // either start new
			CurrSlider = wf->NewDiv();
		else wf->SelectDiv(CurrSlider);	// or resume current
		PreSlideDiv = CurrentDiv;
		CurrentDiv = CurrSlider;
		wf->putCW("page");
	  wf->putEnd();
		wf->putCW("pard");
		wf->putCW("plain");

		// put in reference string for popup
		FtnMarker = '#';
		WriteFtnStart(wf);
		WriteFtnNum(wf);
		wf->putStr(str);
		WriteFtnEnd(wf);
	  wf->putEnd();

		while (*ptr != '\0') {
			if ((len = strlen(ptr)) < StringMax) {  // fits, write it
				wf->putStr(ptr);
				break;
			}
			for (txt = ptr + StringMax; txt > ptr; txt--) { // find blank
				if (*txt == ' ')
					break;
			}
			if (txt == ptr) {  // no blanks, just break it
				wf->putStr(ptr, StringMax);
				ptr += StringMax;
			}
			else {  // break at blank
				*txt = '\0';
				wf->putStr(ptr);
				*txt = ' ';
				ptr = txt;
			}
			wf->putEnd();
		}
		wf->putCW("par");
	  wf->putEnd();
		wf->SelectDiv(CurrentDiv);
	}
	// ignore anything else, like nextpage
}


void 
RTFwr::PutHyperLoc(RTFwfile *wf, RTFctl *cp)
{
	if ((UseHL == false)
	 || Curr.del)
		return;

	char *str = NULL;

	// newlink text
	WriteProps(wf);
	if (IXnewlinkPrefix) {
		str = NewName(IXnewlinkPrefix, strlen(IXnewlinkPrefix) + cp->siz());
		strcat(str, (char *) cp->ex());
	}
	else if (XMLSource)
		str = GetXRStr(cp);
	else
		str = NewName((char *) cp->ex());
	SetRefStr(str);
	WriteRefStrs(wf);
}


void 
RTFwr::PutHyperAlt(RTFwfile *wf, RTFctl *cp)
{
	if (XrfType != 'H')
		return;

	// newlink text
	WriteProps(wf);
	SetRefStr(NewName((char *) cp->ex()));
	WriteRefStrs(wf);
}


void 
RTFwr::PutHyperUnique(RTFwfile *wf, RTFctl *cp)
{
	if (!UseHL)
		return;

	if (InVar) {  // never need the refs to vars
		InVar = false;
		return;
	}

	if (XrefMarker) { // no need for the marker IDs
		XrefMarker = false;
		// return;  // but we can't get the condition right
	}

	if (InIndexGroup  // no need for IX marker IDs
	 && !KeepIXMarkerIDs
	 && !UseFrameIX)  // if not using Frame IX
		return;

	if (Curr.mref == false) {  // MakeRef is always referenced
		if ((HUType == 'N')
		 || ((HUType == 'R')     // use referenced para ids only
		  && (InIndexGroup == false)  // not needed for IX
		  && (Curr.cntform == 0)))    // not needed for TOC
			return;
	}
	HyperUnique[HULevel++] = cp->siz();
	if (HULevel == HULevMax)
		HULevel--;

	if (InPara)
		WriteHyperUnique(wf);
}


void 
RTFwr::WriteHyperUnique(RTFwfile *wf)
{
  char buf[12];
	char *ref;

	if ((HULevel == 0)
	 || (HelpForm && (Curr.line > 1))
	 || !UseHL
	 || WritingAnumVal)
		return;

	for (short i = 0; i < HULevel; i++) {
		if (UseXrefFilePrefix) {
			ref = NewName(strlen(CurrFilePrefix) + 12);
			sprintf(ref, "X%s%ld", CurrFilePrefix, HyperUnique[i]);
		}
		else {
			sprintf(buf, "X%ld", HyperUnique[i]); // prefix number with 'X'
			ref = buf;
		}

		if (HelpForm) {
			FtnMarker = '#';
			WriteFtnStart(wf);
			WriteFtnNum(wf);
			wf->putStr(ref);
			WriteFtnEnd(wf);
		}
		else
			PutWordBookmarks(wf, ref);
	}

	HULevel = 0;
}


void 
RTFwr::PutHyperFile(RTFwfile *wf, RTFctl *cp)
{
	if (XrfType == 'H') {
		if (RefStr)   // second hypertext, so put out previous one now
			PutRefStr(wf);
	}

	if (UMXL) {
		long len = cp->siz();
		char *fn = NewName((char *) cp->ex(), len);
		char *str = NewName(len * 4);
		long pos = 0;
		for (long i = 0; i < len; i++) {
			str[pos++] = fn[i];
			if (fn[i] == '\\') {
				strcat(str, "\\\\\\");
				pos += 3;
			}
		}
		str[pos] = '\0';
		XrefFile = NewName(str);
		DeleteName(str);
		DeleteName(fn);
		return;
	}

	SetXrefFileName((char*) cp->ex(), cp->siz());
}


void 
RTFwr::PutHyperLnk(RTFwfile *wf, RTFctl *cp)
{
	if (!UseHL
	 || Curr.del)
		return;

	char *txt = (char *) cp->ex();
	short len = (short) cp->siz();
	char *ptr = NULL;
	static char str[128];

	if (RefStr) {  // second hypertext, so put out previous one now
		if (HelpForm
		 || HotDiv)
			PutRefStr(wf);
		if (!HelpForm)
			HyperStart = true;
	}

	if (cp->dat() == 7) {  // add ExecFile macro as message URL wrapper
		if (!_strnicmp(txt, "file:///", 8)) {
			txt += 8;
		}
		else if (!_strnicmp(txt, "alink:", 6)) {
			txt += 6;
			if (HelpForm) {
				RefStr = NewName(len + 1);
				strcpy(RefStr, "!AL(`");
				strcat(RefStr, txt);
				strcat(RefStr, "')");
			}
			else {
				XrefFile = NULL;
				RefStr = NULL;
			}
			return;
		}
		else if (!_strnicmp(txt, "klink:", 6)) {
			txt += 6;
			if (HelpForm) {
				RefStr = NewName(len + 4);
				strcpy(RefStr, "!KLink(`");
				strcat(RefStr, txt);
				strcat(RefStr, "')");
			}
			else {
				XrefFile = NULL;
				RefStr = NULL;
			}
			return;
		}
		else if (!_strnicmp(txt, "http", 4)) {
			PopOver = true;
			XrefFile = NewName(txt, len);
			RefStr = NewName("#");
			return;
		}
		if (HelpForm) {
			RefStr = NewName(len + 7);
			strcpy(RefStr, "!EF(`");
			strcat(RefStr, txt);
			strcat(RefStr, "')");
		}
		else {
			PopOver = true;
			XrefFile = NewName(txt, len);
			RefStr = NewName("#");
			//RefStr = NULL;
		}
		return;
	}
	if (cp->dat() == 5)  // object IDs always linked to new window
		PopOver = true;

	if ((*txt == '\0')  // openlink without destination
	 || !strncmp(txt, "firstpage", 9)		// nonunique names,
	 || !strncmp(txt, "lastpage", 8)) { // use filename as ref
		if (XrefFile) {
			if (XrefMapped) {
				len = strlen(XrefRawFile);
				if (len > 127)
					len = 127;
				strncpy(str, XrefRawFile, len); // terminate there
			}
			else {
				len = strlen(XrefFile);
				if (len > 127)
					len = 127;
				strncpy(str, XrefFile, len); // terminate there
			}
			txt = str;
			//if ((ptr = strrchr(str, '.')) != NULL)
			//	*ptr = 0;  // strip file suffix
			if ((ptr = strrchr(str, '/')) != NULL) // always uses / as sep
				txt = ptr + 1; // strip leading path
			//if (!XrefMapped) {
			//	DeleteName(XrefFile);  // assume part of current file
			//	XrefFile = NULL;
			//}
		}
		else   // use our own base name
			txt = wf->getBaseName();
	}

	if (UMXL) {
		txt = NewName((char *) cp->ex(), cp->siz());
		if ((*txt == 'R')
		 && (*(txt + 4) == 'U')
		 && XrefFile) {
			RefStr = NewName(txt, len);
			return;
		}
	}

	if (XMLSource)
		txt = GetXRStr(cp);
											
	len = strlen(txt) + 1;
	if (IXnewlinkPrefix && !XrefFile) {
		len += strlen(IXnewlinkPrefix);
		RefStr = NewName(IXnewlinkPrefix, len);
		strcat(RefStr, txt);
	}
	else
		RefStr = NewName(txt, len);
	XrefWindow = Curr.hwindow;
	XrefHvis = Curr.hvis;
}



void 
RTFwr::PutHyperObj(RTFwfile *wf, RTFctl *cp)
{
	if (!UseHL)
		return;

	if (RefStr)   // second hypertext, so put out previous one now
		PutRefStr(wf);

	PrepRefStr((long) cp->siz());
}



void 
RTFwr::PrepRefStr(long num)
{
	XrefWindow = Curr.hwindow;
	XrefHvis = Curr.hvis;

	if (UseXrefFilePrefix) {
		RefStr = NewName(strlen(XrefFilePrefix) + 12);
		sprintf(RefStr, "X%s%ld", XrefFilePrefix, num);
		//XrefFilePrefix = CurrFilePrefix;
		return;
	}

  char buf[12];
  sprintf(buf, "X%ld", num); // prefix number with 'X'
	RefStr = NewName(buf);

	// add ID value to database of required values later
}


void 
RTFwr::PutHyperSubject(RTFwfile *wf, RTFctl *cp)
{
	if ((HelpForm != true)
	 || (cp->siz() < 2))
		return;

	WriteExHelpFtn(wf, 'A', (char *) cp->ex());
}


void 
RTFwr::PutRefStr(RTFwfile *wf)
{
	if (((XrefWindow == NULL)
	  || (*XrefWindow == '\0'))
	 && (Curr.hwindow != NULL)
	 && (*(Curr.hwindow) != '\0'))
		XrefWindow = Curr.hwindow;

	if (XrefHvis == 0)
		XrefHvis = Curr.hvis;

	if (HelpForm) {
		if (Last.line > 1) {
			if (HyperHelp) {
				RestoreState(LastState);
				wf->putGE();
			}
			else
				wf->putCW((Last.line == 3) ? "uldb0" : "ul0");
			Last.line = 1;
		}
		wf->putGS();
		wf->putCW("v");
		PutContext(wf, RefStr);
		wf->putGE();

		DeleteName(RefStr);
		RefStr = NULL;
		XrefWindow = NULL;
		XrefHvis = '\0';

		XrefFile = NULL;
		XrefFilePrefix = CurrFilePrefix;
	}
	else  // wrap up current hyperlink
		PutWordHyper(wf);
}


void 
RTFwr::PutWordHyper(RTFwfile *wf)
{
	//if (!HotDiv) // it would prevent a crash, but
	//	return;  // this will produce a broken index

	long cdiv = wf->GetDiv();
	bool condmess = false;

	if ((cdiv == CondFlagDiv)
	 && (HotDiv == PreCondFlagDiv)) { // untangle this
		condmess = true;
		PreCondFlagDiv = PreHotDiv;
	}
	wf->SelectDiv(PreHotDiv);

	if (RefStr)
		PutHyperFieldStart(wf, RefStr, PopOver);
	PopOver = false;
	if (HotDiv) {
		wf->WriteDiv(HotDiv);
		wf->DeleteDiv(HotDiv);
	}
	HotDiv = 0;
	wf->SelectDiv(PreHotDiv);
	if (RefStr)
		PutHyperFieldEnd(wf);
	HyperEnd = false;
	PreHotDiv = 0;
	RefStr = NULL;
	if (condmess)
		wf->SelectDiv(CondFlagDiv);
}


void 
RTFwr::PutHyperFieldStart(RTFwfile *wf, char *ref, bool pop)
{
	wf->putEnd();
	wf->putGS();
	wf->putCW("field");
	//wf->putCW("fldedit");
	if (LockHyper)
		wf->putCW("fldlock");
	wf->putGS();
	wf->putCS('*');
	wf->putCW("fldinst");
	wf->putGS();
	wf->putStr("HYPERLINK ");
	if (XrefFile) {
		wf->putGS();
		wf->putStr(XrefFile);
		if (!strchr(XrefFile, '.')) {
			wf->putText('.');
			wf->putStr(XrefFileSuffix ? XrefFileSuffix : "doc");
		}
		wf->putGE();
	}
	if (pop)
		wf->putStr("\\\\n ");
	if (ref) {
		wf->putStr("\\\\l ");
		//wf->putGS();
		if ((*ref != '#')
		 || (*(ref + 1) != '\0'))
			wf->putStr(ref);
		//else
		XrefFile = NULL;
		LastPageRefBookmark = NewName(ref);
		//wf->putGE();
	}
	wf->putGE();
	wf->putGE();
	wf->putGS();
	wf->putCW("fldrslt");
	wf->putGS();
}


void 
RTFwr::PutHyperFieldEnd(RTFwfile *wf)
{
	wf->putGE();
	wf->putGE();
	wf->putGE();
	wf->putEnd();
}



// xref format processing

char *RTFwr::XrefName;
DCnlist RTFwr::XrefNameList;
long RTFwr::XrefID = 0;
long RTFwr::XrefUsage = 1;
DCilist RTFwr::XrefUseList;
long RTFwr::XrefInstID = 0;
char *RTFwr::XrefInstStr = NULL;
DCilist RTFwr::XrefIDUseList;

anyini RTFwr::XrefIni;
bool RTFwr::XrefTextOnly = false;
bool RTFwr::XrefDelete = false;


void 
RTFwr::ProcXrefFormatName(RTFctl *cp)
{
	XrefName = NewName((char *) cp->ex());
	XrefNameList.add(XrefName, XrefID);

	XrefUsage = 1;

	XrefTextOnly = XrefDelete = false;
	DCini::ProcIniSect("XrefStyles"); // based on XrefName
	if (XrefTextOnly)
		XrefUsage |= 2;
	if (XrefDelete)
		XrefUsage |= 4;

	XrefUseList.add(XrefUsage, XrefID);
	XrefID = 0;
}


void 
RTFwr::ProcXrefFormatUse(long id)
{
	// write phase, immediately after group start
	XrefTextOnly = XrefDelete = false;

	XrefID = id; // format ID
	XrefUsage = XrefUseList.find(XrefID);
	XrefTextOnly = ((XrefUsage & 2) == 2) ? true : false;
	XrefDelete = ((XrefUsage & 4) == 4) ? true : false;

	XrefIni.nm[1] = XrefName = (char *) XrefNameList.find(XrefID);
	XrefIni.nm[0] = XrefInstStr = itoa(XrefInstID, NewName(NumStrMax), 10);
	XrefIni.cnt = 2;
	DCini::XrefSetList.all(DCini::GetXrefSettings);  // marker overrides replace format props
}


// xref processing

short RTFwr::CurrXRForm;
char *RTFwr::CurrXRStr;
char *RTFwr::CurrXRBkmkStr = NULL;
long RTFwr::XRValNum = 0;
bool RTFwr::WritingXRVal = false;
bool RTFwr::XRValSpace = false;
bool RTFwr::LockXrefs = true;

DCvlist RTFwr::XRFormList;
DCvlist RTFwr::XRValList;
DCnlist RTFwr::XRStrList;
DCnlist RTFwr::XRLocList;

DCilist RTFwr::XRVFormList;
DCnlist RTFwr::XRVStrList;
DCnlist RTFwr::XRVFileList;

DClist RTFwr::XRFPnList;  // para num only formats
DClist RTFwr::XRFPsList;	// para num string formats
DClist RTFwr::XRFPtList;  // para text formats

unc RTFwr::XRNused = 0;
unc RTFwr::XRSused = 0;
unc RTFwr::XRTused = 0;

DCnlist RTFwr::XRNStrList;
DCnlist RTFwr::XRSStrList;
DCnlist RTFwr::XRTStrList;

DCnlist RTFwr::XRNEndStrList;
DCnlist RTFwr::XRSEndStrList;
DCnlist RTFwr::XRTEndStrList;

bool RTFwr::LogBookmarkCount = false;
long RTFwr::LastBkmkCount = 0;
long RTFwr::BkmkstartCount = 0;
long RTFwr::BkmkendCount = 0;
long RTFwr::BkmkErrCount = 0;


void
RTFwr::SetXRForm(RTFctl *cp)
{
  // add to xrform list
	RTFgroup* gp = new RTFgroup(*cp);
	GroupStack[DCLlevel] = gp;      // to collect format ctls
	XRFormList.add(gp, (long) CurrXRForm);
}


bool
RTFwr::SetXRToken(RTFctl *cp)
{
	static short tag = 0;
	static short page = 0;

  // add to xrf token list
	switch (cp->siz()) {
		case 1:  // num only, "1.2.3"
			XRFPnList.add((long) CurrXRForm);
			break;
		case 2:	 // num string, "Section 1.2.3"
			XRFPsList.add((long) CurrXRForm);
			break;
		case 3:  // tag
			tag++;
			return true;
		case 4:	 // text, "Topic Name"
			XRFPtList.add((long) CurrXRForm);
			break;
		case 5:  // page
			page++;
			return true;
		default:
			break;
	}

	return false;
}


void
RTFwr::SetXRVal(RTFctl* cp)
{
	// scan phase, group start
	cp->siz(++XRValNum);
	RTFgroup* gp = new RTFgroup(*cp);
  GroupStack[DCLlevel]->add(*cp); // to provide a trigger
	GroupStack[DCLlevel] = gp;      // to collect xref ctls
  XRValList.add(gp, XRValNum);
}


char*
RTFwr::GetXRStr(RTFctl *cp, bool jump)
{
  // get ref str from src string, return new allocated string
	char *refstr = NULL;
	char *prefix = NULL;
	short xlen = 0;
	if (UseXrefFilePrefix)
		xlen = strlen(prefix = (jump ? XrefFilePrefix : CurrFilePrefix)); 

	if (cp->dt() == etext) {
		short len = 0;
		if ((len = (short) cp->siz()) > (XrefLenLimit - xlen))
			len = XrefLenLimit - xlen;
		refstr = NewName(len + xlen + 2);
		*refstr = 'R'; // prefix for refs
		if (xlen) {
			strcat(refstr, prefix);
			//if (jump)
			//	XrefFilePrefix = CurrFilePrefix;
		}
		memcpy(refstr + xlen + 1, cp->ex(), len);
		*(refstr + xlen + len + 1) = '\0';
		if (XrfType == 'S')
			ScanBookmark(refstr);
		return refstr;
	}
	else if (cp->dt() == ilong) {
		if (xlen) {
			refstr = NewName(xlen + 12);
			sprintf(refstr, "R%s%ld", prefix, cp->siz());
			//if (jump)
			//	XrefFilePrefix = CurrFilePrefix;
			return refstr;
		}
		char buf[12];
		sprintf(buf, "R%ld", cp->siz()); // prefix number with 'R'
		return refstr = NewName(buf);
	}
	else
		return NULL;
}


void
RTFwr::SetXRRef(void)
{
	// scan phase, end of group
  // add xr reference ref str and form num to lists
	XRStrList.add(CurrXRStr, CurrXRForm);
	XRVFormList.add(CurrXRForm, XRValNum);
	XRVStrList.add(CurrXRStr, XRValNum);
	XRVFileList.addnew(XrefFile, XRValNum);
	// may have multiple uses of same ref str with different forms
}


void
RTFwr::SetXRSrc(void)
{
  // record para start as containing xr src by ref str
	XRLocList.add(CurrXRStr, ParaRefNum);
}


bool RTFwr::NoFreakingBookmarks = false;


void
RTFwr::SetXRBookmarks(void)
{
	// at start of para, retrieve strs and find out what formats are needed
  // set up variables to create the bookmarks required per formats
  //  for para num_only (N), para num_str (S), and para text (T).
	// will write at start of anum (S), start of actual number (N),
  //  end of anum (S, N), start of text (T), and end of para (T).
	// need start and end lists for each type, empty() all at end of para
	//  write end lists backwards so bookmarks nest and don't overlap
	DCnlist* nlp = &XRLocList;
	DCnlist* slp = &XRStrList;

	char* bkstr;
	char* lastbkstr = "";
	long xfm;
	CurrXRBkmkStr = NULL;

	if (NoFreakingBookmarks)
		return;

	do {
		if ((bkstr = (char *) nlp->find(ParaRefNum, &nlp)) == NULL) // get each ref str
			break;
		if (CurrXRBkmkStr == NULL)
			CurrXRBkmkStr = bkstr; // get first one only
		if (strcmp(bkstr, lastbkstr)) {
			lastbkstr = bkstr;			
			slp = &XRStrList;
			do {
				if (ExtXRefs)
					slp = NULL;
				else if ((xfm = (*slp)(bkstr, &slp)) == 0) // get each fmt for ref str
					break;
				if (ExtXRefs
				 || XRFPnList.find(xfm)) { // fmt includes num-only token
					XRNused = 1;
					XRNStrList.add(bkstr, 'N');	// letter is suffix to use
					XRNEndStrList.insert(bkstr, 'N' + 0x100);  // flag for end
				}
				if (ExtXRefs
				 || XRFPsList.find(xfm)) { // fmt includes paranum token
					XRSused = 1;
					XRSStrList.add(bkstr, 'S');
					XRSEndStrList.insert(bkstr, 'S' + 0x100);
				}
				if (ExtXRefs
				 || XRFPtList.find(xfm)) { // fmt includes paratext token
					XRTused = 1;
					XRTStrList.add(bkstr, 'T');
					XRTEndStrList.insert(bkstr, 'T' + 0x100);
				}
			} while (slp != NULL);		
		}
	} while (nlp != NULL);
}


void
RTFwr::XRStartAnumNum(RTFwfile *wf)
{
	if (XRNused == 1) {
		XRNStrList.writeall(wf, PutBkmk);
		XRNused = 2;
	}
}


void
RTFwr::XREndAnumNum(RTFwfile *wf)
{
	if (XRNused == 2)	{
		XRNEndStrList.writeall(wf, PutBkmk);
		XRNused = 3;
	}
}


void
RTFwr::XRStartAnumStr(RTFwfile *wf)
{
	if (XRSused == 1)	{
		XRSStrList.writeall(wf, PutBkmk);
		XRSused = 2;
	}
}


void
RTFwr::XREndAnumStr(RTFwfile *wf)
{
	if (XRSused == 2)	{
		XRSEndStrList.writeall(wf, PutBkmk);
		XRSused = 3;
	}
}


void
RTFwr::XRStartPara(RTFwfile *wf)
{
	if (NoFreakingBookmarks)
		return;

#if 0
	if ((XRSused != 3)  // no anum, so put out the empty bkmks
	 && CurrXRBkmkStr) {
		PutBkmk(wf, CurrXRBkmkStr, 'S');
		PutBkmk(wf, CurrXRBkmkStr, 'N');
		PutBkmk(wf, CurrXRBkmkStr, 'N' + 0x100);
		PutBkmk(wf, CurrXRBkmkStr, 'S' + 0x100);
		XRSused = XRNused = 3;
	}
#endif

	if (XRTused == 1)	{
		XRTStrList.writeall(wf, PutBkmk);
		XRTused = 2;
	}

	long prevdiv = wf->GetDiv();
	IdxDiv = wf->NewDiv();
	wf->SelectDiv(prevdiv);
}


void
RTFwr::XREndPara(RTFwfile *wf)
{
	if (NoFreakingBookmarks)
		return;

	if (XRTused == 2)	{
		XRTEndStrList.writeall(wf, PutBkmk);
		XRTused = 3;
	}
	ResetXRBookmarks();

	if (ExtXrefPages  // write hidden field for page refs
	 && ExtXRefs
	 && CurrXRBkmkStr) {
		PutBkmk(wf, CurrXRBkmkStr, 'P');
		WriteRefFieldStart(wf, CurrXRBkmkStr, true, false, "\\v");
		if (CurrPgn) {
			char *str = NewName(NumStrMax);
			sprintf(str, "%ld", CurrPgn);
			wf->putStr(str);
		}
		else
			wf->putStr("33");
		WriteRefFieldEnd(wf);
		PutBkmk(wf, CurrXRBkmkStr, 'P' + 0x100);
		CurrXRBkmkStr = NULL;
	}

	if (IdxDiv) {
		long prevdiv = wf->GetDiv();
		wf->WriteDiv(IdxDiv);
		wf->DeleteDiv(IdxDiv);
		IdxDiv = 0;
		wf->SelectDiv(prevdiv);
	}	
}


void
RTFwr::ResetXRBookmarks(void)
{
	if (NoFreakingBookmarks)
		return;

	// clear the para lists at end of para
	if (XRNused > 0) {
		XRNused = 0;
		XRNStrList.empty();
		XRNEndStrList.empty();
	}
	if (XRSused > 0) {
		XRSused = 0;
		XRSStrList.empty();
		XRSEndStrList.empty();
	}
	if (XRTused > 0) {
		XRTused = 0;
		XRTStrList.empty();
		XRTEndStrList.empty();
	}
}


void
RTFwr::PutBkmk(DCwfile *w, char *bkstr, long val)
{
	if (NoFreakingBookmarks)
		return;

	// used as List.writeall() arg
	if (*bkstr == '\0')
		return;  // ignore empty strings

	static char lastbkmk[128];  // prevent duplicates
	static long lastval = 0;
	if (*lastbkmk
	 && !strcmp(bkstr, lastbkmk)
	 && (val == lastval))
		return;
	if (strlen(bkstr) < 128)
		strcpy(lastbkmk, bkstr);
	lastval = val;

	RTFwfile *wf = (RTFwfile *) w;
	wf->putEnd();
	wf->putGS();
	wf->putCS('*');
	wf->putCW(((val & 0x100) == 0x100) ? "bkmkend" : "bkmkstart");
	if ((val & 0x100) == 0x100)
		BkmkstartCount++;
	else
		BkmkendCount++;
	wf->putStr(bkstr);
	wf->putChar((int) val & 0xFF);
	wf->putGE();
}


void
RTFwr::WriteXRVal(RTFwfile *wf, RTFctl *cp)
{
	// write phase, start of group
	XrefInstID = (long) cp;
	if ((XrfType != 'S')	 // rest of group is inline
	 || WritingXRVal)  // do not recurse
		return;

	long id = XrefIDUseList.find(XrefInstID);
	ProcXrefFormatUse(id);

	if (XrefDelete)
		return;

	WritingXRVal = true;
	XRValSpace = false;
	XRValNum = cp->siz();
	RTFgroup *gp = (RTFgroup*) XRValList.find(XRValNum);

	if (XrefTextOnly) {
		WriteText(wf);
		if (gp)
			gp->write(*wf);
		return;
	}

	CurrXRStr = (char *) XRVStrList.find(XRValNum);
	XrefFile = (char *) XRVFileList.find(XRValNum);

	WriteText(wf);

	if (Word8 && FieldHyper && WrapXrefs)
		PutHyperFieldStart(wf, CurrXRStr);
	if (gp)
		gp->write(*wf);
	if (Word8 && FieldHyper && WrapXrefs)
		PutHyperFieldEnd(wf);

	WritingXRVal = false;	
	XRValSpace = false;	
}

void
RTFwr::WriteXRCont(RTFwfile *wf, RTFctl *cp, bool start)
{
	// write phase, start or end of content group
	if ((XrfType != 'S')
	 || XrefTextOnly
	 || XrefDelete)
		return;

	bool pg = (cp->dat() == 5) ? true : false;
	if (pg && XrefFile  // illegal, no extern page refs
	 && !ExtXrefPages)
		return;

	if (cp->dat() > 5) // chapnum and volnum, make text
		return;

	if (!start) {
		WriteRefFieldEnd(wf);
		if (pg && XrefFile)
			WriteRefFieldEnd(wf);
		return;
	}

	if (CurrXRStr == NULL) {
		PutVarToken(wf, cp);
		return;
	}

	char *rstr = NewName(CurrXRStr, strlen(CurrXRStr) + 2);
	char *estr = rstr + strlen(rstr);  // pointer to suffix

	switch (cp->dat()) {
		case 1:
			*estr = 'N'; // for para num only
			break;
		case 2:
			*estr = 'S'; // for para num str
			break;
		case 4:
			*estr = 'T'; // for para text
			break;
		case 5:  // page
		case 6:  // chapnum
		case 7:  // volnum
		default:
			break;
	}

	WriteText(wf);
	if (pg && XrefFile) {
		*estr = 'P';
		WriteRefFieldStart(wf, rstr, pg, true, "\\v0");
		*estr = '\0';
		WriteRefFieldStart(wf, rstr, pg, false, "\\v0", true);
	}
	else
		WriteRefFieldStart(wf, rstr, pg);

}



#if 0
	CurrXRForm = (short) XRVFormList.find(XRValNum);
	RTFgroup *fp = (RTFgroup*) XRFormList.find(CurrXRForm);

	// at start of XRef usage, must insert field starts/ends
  // for each element in the ref definition
	// interspersed with the plain text from the definition

	WriteText(wf);

	// step through the pair of DCL (RTF) groups writing matching
	// elements.  At a mismatch, where form is ref_token, put in
	// field, with the related val content (up to next match) as
	// the field result.  Ignore soft returns in val, may need to
	// stick text obj lit string items together to get match.

	// original method, make RTF and compare; big problems

	long cdiv = wf->GetDiv();
	wf->SetCR(false);

	// write gp to div as reference
	long valdiv = wf->NewDiv();
	bool empty = false;
	SaveState();                  // save state of props
	if (gp)
		gp->write(*wf);
	char *valp = wf->ReadDiv(); 	// get ptr to the div str
	size_t vallen = wf->LenDiv();
	if (!vallen || !valp)
		empty = true;  // indicates unresolved xref in Frame

	wf->SelectDiv(cdiv);
	RestoreState(FullState);      // set props back to start

	// write fp to div, marking empty result fields with '\0'
	long defdiv = wf->NewDiv();
	if (fp)
		fp->write(*wf);
	char *defp = wf->ReadDiv(); 	// get ptr to the div str
	size_t deflen = wf->LenDiv(); //  and total real length (with nulls)
	size_t defstrlen = strlen(defp);
	char *lastp = defp + deflen;

	// write to file, inserting field result vals into each field
	wf->SelectDiv(cdiv);
	char *newp;	// ptr to start of non-token text in val str
	char *endp; // ptr to ending part of ref field
	if (Word8 && FieldHyper && WrapXrefs)
		PutHyperFieldStart(wf, CurrXRStr);

	// problem here, when part of first section repeats so
	// that some element like a quote appears twice
	wf->putStr(defp);  // write first section up to start of ref field
	// to avoid that, throws away anything before
	// the matching section, like a needed brace
	if (defstrlen
	 && vallen
	 && ((newp = strstr(valp, defp)) != NULL)) // find str before result
		valp = newp + defstrlen;  // end of that text str is start of result

	char *refp;
	while (defp < lastp) {  // more to go
		defp += defstrlen + 1; // start of ref field code
		defstrlen = strlen(defp);
		refp = defp;
		defp += defstrlen + 1; // ending of ref field code
		defstrlen = strlen(defp);
		endp = defp;					 // save ptr to ending of ref field
		defp += defstrlen + 1; // after ref field code
		if (((defstrlen = strlen(defp)) != 0)
		 && vallen
		 && ((newp = strstr(valp, defp)) != NULL)) // find str after result
			vallen = newp - valp;
		else if (!empty)
			vallen = strlen(valp);
		if (vallen || empty) {  // write field even if value empty
			wf->putStr(refp);			 // write ref field code up to result field
			if (vallen)
				wf->putStr(valp, vallen);	// write result field if not empty
			wf->putStr(endp);      // write ending of ref field
		}
		if (!empty) {
			valp += vallen;
			if (defstrlen && newp)
				valp += defstrlen;
			vallen = strlen(valp);
		}
		wf->putStr(defp);			 // write str after ref field
	}

	if (Word8 && FieldHyper && WrapXrefs)
		PutHyperFieldEnd(wf);

	// clean up
	wf->DeleteDiv(defdiv);
	wf->DeleteDiv(valdiv);
	wf->SetCR(true);
#endif



void
RTFwr::WriteXRToken(RTFwfile *wf, RTFctl *cp)
{
	if (XrfType != 'S')
		return;

	if (CurrXRStr == NULL) {
		PutVarToken(wf, cp);
		return;
	}

	bool pg = (cp->siz() == 5) ? true : false;
	if (pg && XrefFile  // illegal, no extern page refs
	 && !ExtXrefPages)
		return;

	char *rstr = NewName(CurrXRStr, strlen(CurrXRStr) + 2);
	char *estr = rstr + strlen(rstr);  // pointer to suffix

	switch (cp->siz()) {
		case 1:
			*estr = 'N'; // for para num only
			break;
		case 2:
			*estr = 'S'; // for para num str
			break;
		case 4:
			*estr = 'T'; // for para text
			break;
		case 5:
			*estr = '\0'; // for para page number ref
			break;
		default:
			break;
	}
	//WriteProps(wf);
	WriteText(wf);
	wf->putChar(0);
	if (pg && XrefFile) {
		*estr = 'P';
		WriteRefFieldStart(wf, rstr, pg, true, "\\v0");
		*estr = '\0';
		WriteRefFieldStart(wf, rstr, pg, false, "\\v0", true);
	}
	else
		WriteRefFieldStart(wf, rstr, pg);
	wf->putChar(0); // may be able to leave result fields empty
	WriteRefFieldEnd(wf);
	if (pg && XrefFile)
		WriteRefFieldEnd(wf);
	wf->putChar(0);

	// DeleteName(rstr);
	LastPageRefBookmark = rstr;
}


void
RTFwr::WriteRefFieldStart(RTFwfile *wf, char *rstr, bool pg,
                          bool extr, char *fmt, bool lock)
{
	wf->putEnd();
	wf->putGS();
	wf->putCW("field");
	if ((lock || LockXrefs) && !pg)
		wf->putCW("fldlock");

	wf->putGS();
	wf->putCS('*');
	wf->putCW("fldinst");	 // field codes
	if (fmt) {
		wf->putGS();
		wf->putStr(fmt);
		wf->putChar(' ');
	}
	if (XrefFile
	 && (extr || !fmt)) {
		wf->putStr("INCLUDETEXT "); // field name, always present
		wf->putGS();
		wf->putStr(XrefFile);
		if (!strchr(XrefFile, '.')) {
			wf->putText('.');
			wf->putStr(XrefFileSuffix ? XrefFileSuffix : "doc");
		}
		wf->putGE();
		wf->putChar(' ');
		//wf->putGS();
		wf->putStr(rstr);
		//wf->putGE();
		wf->putStr(" \\\\! ");
	}
	else {
		wf->putStr(pg ? "PAGEREF " : "REF ");
		//wf->putGS();
		wf->putStr(rstr);
		//wf->putGE();
		if (Word8 && FieldHyper)
			wf->putStr(" \\\\h ");
	}
	wf->putStr(" \\\\* MERGEFORMAT ");
	if (fmt)
		wf->putGE();
	wf->putGE();
	wf->putEnd();
	wf->putGS();
	wf->putCW("fldrslt");	   // result field
	wf->putGS();
	if (fmt && !extr) {
		wf->putStr(fmt);
		wf->putChar(' ');
	}
}


void
RTFwr::WriteRefFieldEnd(RTFwfile *wf)
{
	wf->putGE();
	wf->putGE();
	wf->putGE();
}


void
RTFwr::SetXrefFileName(char *name, size_t len)
{
	//if ((XrfType != 'H')
	// && (XrfType != 'F'))
	//	return;

//  if (XrefMapped)
  DeleteName(XrefRawFile);
//  else if (XrefFile)
//    DeleteName(XrefFile);

  char *xfname = NewName(name, len);
	if (len == 0)
		len = strlen(xfname);
  char *cpt = strrchr(xfname, '/');	// remove any path
	char *bpt = strrchr(xfname, '>'); // and any Frame path codes
	if (bpt > cpt)
		cpt = bpt;
  if (cpt)	 // shorten but no need to reallocate str
    memmove(xfname, cpt + 1, len - (cpt - xfname));
	StripSuf(xfname);

  // remap name
	XrefFile = GetXrefName(xfname);
  XrefRawFile = xfname;
	XrefMapped = true;

	if (XrefFile
	 && CurrHelpBase
	 && !stricmp(XrefFile, CurrHelpBase))
		XrefFile = NULL;

//  else {
//    XrefFile = xfname;
//		XrefRawFile = NULL;
//		XrefMapped = false;
//	}

	XrefFilePrefix = GetFIDPrefix(xfname);
}


bool RTFwr::InHelpXref = false;
bool RTFwr::XrefMarker = false;


void 
RTFwr::StartAnchor(RTFwfile *wf, RTFctl *cp, bool href)
{
	XrefMarker = true;

	if ((XrfType == 'N')
	 || XrefTextOnly
	 || XrefDelete)
		return;

	char *ref = NULL;

	if ((ref = GetXRStr(cp, href)) == NULL)
		return;

	if (href) {
		if (HelpForm) {	// mark text for jump
			if ((Curr.line != 2) // no override of popup
			 || (Curr.help == 0)
			 || NoXRPopup)
				Curr.line = 3;
			Curr.lmode = 1;
			InHelpXref = true;
			RefStr = ref;
		}
		else CurrXRStr = ref;
	}	
	else {	 // target of cross-reference
		WriteProps(wf);
		SetRefStr(ref);
		XrefFile = NULL;
		if (InPara)
			WriteRefStrs(wf);  // try to put right away
	}
}


void
RTFwr::SetRefStr(char *str)
{
	static char *lastXRstr = NULL;

	if (lastXRstr
	 && str
	 && !strcmp(lastXRstr, str))
		return;

	RefStrList.add(str, ++RefLevel);
	lastXRstr = str;
}


void
RTFwr::WriteRefStrs(RTFwfile *wf)
{
	if ((RefLevel == 0)
	 || (HelpForm && (Curr.line > 1))
	 || WritingAnumVal)
		return;

	RefStrList.writeall((DCwfile *) wf, WriteRefStrListItem);
	RefStrList.empty();
	RefLevel = 0;
}


void
RTFwr::WriteRefStrListItem(DCwfile *pf, char *ref, long rnum)
{
	if (!rnum
	 || !ref)
		return;

	RTFwfile *wf = (RTFwfile *) pf;

	if (HelpForm) {
		FtnMarker = '#';
		WriteFtnStart(wf);
		WriteFtnNum(wf);
		PutFtnContext(wf, ref);
		WriteFtnEnd(wf);
		DeleteName(ref);
	}
	else   // write bookmark pair
		PutWordBookmarks(wf, ref);
}


void
RTFwr::PutWordBookmarks(RTFwfile *wf, char *ref)
{
	if (!ref
	 || (*ref == '\0'))
		return;

	if (XMLSource
	 && (*ref != 'R'))
		return;

	wf->putGS();
	wf->putCS('*');
	wf->putCW("bkmkstart");
	wf->putStr(ref);
	BkmkstartCount++;
	wf->putGE();
	wf->putGS();
	wf->putCS('*');
	wf->putCW("bkmkend");
	wf->putStr(ref);
	BkmkendCount++;
	wf->putGE();
	wf->putEnd();
}


void
RTFwr::ScanBookmark(char *chp)
{
	char *p = chp;
	char ref[37];

	for (short i = 0; i < 36; p++) {	// 36 chars max, allow for BM and suf
		if (*p == '\0')
			break;
		if (isalnum(*p) || (*p == '_'))	// only alnum and '_'
			ref[i++] = *p;
	}
	ref[i] = '\0';
	strcpy(chp, ref);  // update original string
}



bool RTFwr::EndingAnchor = false;

void 
RTFwr::EndAnchor(RTFwfile *wf)
{
	if ((XrfType == 'S')
	 && !HelpForm
	 && HyperEnd
	 && HotDiv) {
		PutWordHyper(wf);
		return;
	}

	if ((XrfType != 'H')
	 || EndingAnchor)
		return;

	if (XrefTextOnly
	 || XrefDelete) {
		XrefTextOnly = XrefDelete = false;
		return;
	}

	EndingAnchor = true;
	WriteText(wf);   // force out anything waiting
	if (RefStr)
		PutRefStr(wf);
	Curr.line = 1;   // end jump or popup text
	InHelpXref = false;
	//WriteText(wf);   // write changed props
	EndingAnchor = false;
}



// footnote writing


uns RTFwr::FtnStart = 1;
uns RTFwr::FtnForm = 1;
uns RTFwr::FtnMarker;
uns RTFwr::FtnPosition = 0;
bool RTFwr::InFootnote;

bool RTFwr::InFtnAnum;
bool RTFwr::InTbFtnAnum;
RTFgroup *RTFwr::FtnFormGroup;
RTFgroup *RTFwr::FtnRFormGroup;
RTFgroup *RTFwr::TbFtnFormGroup;
RTFgroup *RTFwr::TbFtnRFormGroup;
bool RTFwr::WritingFtnGroup = false;

bool RTFwr::StartingFtnCont;

long RTFwr::FootDiv = 0;
short RTFwr::FootnoteNum = 0;
short RTFwr::TxFootNum = 0;
short RTFwr::TbFootNum = 0;


void
RTFwr::WriteFtnProps(RTFwfile *wf)
{
  wf->putEnd();
  wf->putCW("ftnbj");
  wf->putCWN("ftnstart", FtnStart);
}


void
RTFwr::WriteFtnGroup(RTFwfile *wf, bool inftn)
{
	WritingFtnGroup = true;

	RTFgroup *gp = NULL;
	if (!inftn)
		gp = (InTbCell) ? TbFtnFormGroup : FtnFormGroup;
	else
		gp = (InTbCell) ? TbFtnRFormGroup : FtnRFormGroup;

	if (gp)
		gp->write(*wf);

	WritingFtnGroup = false;
}


void
RTFwr::WriteFtnAnumToken(RTFwfile *wf, RTFctl *cp)
{
	if (FootType == 'V') {
		WriteText(wf);
		wf->putCW("chftn");
		return;
	}

	short ftnum = InTbCell ? TbFootNum : TxFootNum;
	RTFctl *ncp = new RTFctl(etext, 2, 10, 1);
	char *fstr = NULL;
	short i = 0;

	switch (cp->dat()) {
		case 2:
		default:
			fstr = new char[12];
			sprintf(fstr, "%d", ftnum);
			break;
		case 5:
			fstr = new char[2];
			*fstr = 'a' + (ftnum - 1);
			*(fstr + 1) = '\0';
			break;
		case 6:
			fstr = new char[2];
			*fstr = 'A' + (ftnum - 1);
			*(fstr + 1) = '\0';
			break;
		case 7:
			fstr = new char[ftnum + 1];
			for (i = 0; i < ftnum; i++)
				fstr[i] = '*';
			fstr[ftnum] = '\0';
			break;
	}
	ncp->ex(fstr);
	WriteText(wf, ncp);
	delete [] fstr;
}


void
RTFwr::WriteTextFtnStart(RTFwfile *wf)
{
	if (WritingHF)
		return;
	SaveState();
	FtnPosition = LinePos;
	PreFtnParaRefNum = ParaRefNum;

	if (FtnMarker == '$')
		HelpTitlePending = false;
	else if (FtnMarker == 0) {
		FootnoteNum++;
		if (InTbCell)
			TbFootNum++;
		else
			TxFootNum++;
	}

	if ((FootType == 'S')
	 || (FootType == 'V')) {
		WriteFtnStart(wf);
	  InPara = false;
	}
	else if (FootDiv) {
		// write fn into text as ftn metafile with underscore
		wf->putGS();
		wf->putCW((FootType == 'P') ? "ul" : "uldb");
		unc tbase = Curr.base;
		Curr.base = 3;
		StartTextMetafile();
		WriteFtnGroup(wf, false);
		EndTextMetafile(wf);
		Curr.base = tbase;
		wf->putGE();
		// write ref string	using local ID
		short len = BrowseDigitsMax + 10;	// allows 6 for ftn
		if (BrowsePrefix)
			len += strlen(BrowsePrefix);
		char *ref = new char[len];
		sprintf(ref, "%s%dFtn%d", BrowsePrefix, BrowseCurr, FootnoteNum);
		wf->putGS();
		wf->putCW("v");
		wf->putStr(ref);
		wf->putGE();
		// switch to footnote div
		wf->SelectDiv(FootDiv);
		// for popup, write topic start	page break
		if (FootType == 'P')
			wf->putCW("page");
		// write ref fn matching ref string
		FtnMarker = '#';
		WriteFtnStart(wf);
		WriteFtnNum(wf);
		wf->putStr(ref);
		WriteFtnEnd(wf);
		delete [] ref;
	  InPara = false;
	}
  InFootnote = true;
  StartingFtnCont = true;

	if (FootType != 'E') {
		StartFtnCharForm = StartCharForm;
		EndFtnCharForm = EndCharForm;
		InFtnCharForm = InCharForm;
		FtnCharFormSaved = CharFormSaved;
		FtnLastCF = LastCF;
		FtnCurr = Curr;
		FtnLast = Last;
		StartCharForm = false;
		EndCharForm = false;
		InCharForm = false;
		CharFormSaved = false;
	}
}


void
RTFwr::WriteFtnStart(RTFwfile *wf)
{
	if (FtnMarker == '$')
		HelpTitlePending = false;
	if (HelpForm
	 && BareFnSym) {
		wf->putEnd();
		wf->putChar(FtnMarker);
	}
	else {
		wf->putGS();
		if (FtnMarker) {
			wf->putCWN("up", 6);
			wf->putChar(FtnMarker);
		}
		else
			WriteFtnGroup(wf, false);
		if (HelpForm == true)
			wf->putGE();
		else if (FootType != 'V')
		 // if no space, ftn number at bottom is superscript too
		 // && (FootType != 'S'))
			wf->putChar(' ');
		wf->putEnd();
	}
	wf->putGS();
	wf->putCW("footnote");
	FtnParaEnding = false;
	TabsChanged = true;
}


void
RTFwr::WriteTextFtnNum(RTFwfile *wf)
{
  StartingFtnCont = false;
	InPara = true;

	if ((FootType == 'S')
	 || (FootType == 'V'))
		WriteFtnNum(wf);
	else if (FootType == 'E')
		wf->putStr(" [");
	else if (FootDiv) {
		wf->putGS();
		WriteFtnGroup(wf, true);
		wf->putGE();
		Curr.base = 1;
	}
}


void
RTFwr::WriteFtnNum(RTFwfile *wf)
{
	if (HelpForm
	 && BareFnSym) {
		if (FtnMarker)
			wf->putChar(FtnMarker);
		else
			WriteFtnGroup(wf, true);
	}
	else {
		wf->putGS();
		if (FtnMarker) {
			wf->putCW("plain");
			wf->putCWN("up", 6);
			wf->putChar(FtnMarker);
			if (FtnSpace == 'B')
				wf->putChar(' ');
			wf->putGE();
			if (FtnSpace == 'A')
				wf->putChar(' ');
		}
		else {
			WriteFtnGroup(wf, true);
			wf->putGE();
		}
	}
}


void
RTFwr::WriteTextFtnEnd(RTFwfile *wf)
{
	if (WritingHF)
		return;

	if ((FootType == 'S')
	 || (FootType == 'V')) {
		WriteFtnEnd(wf);
	  TabsChanged = true;
	}
	else if (FootType == 'E')
		wf->putStr("] ");
	else if (FootDiv) {
		//wf->putCW("par");
		wf->putCW("par");
		wf->putEnd();
		if (FtnEndMacro) {
			DCmacr::WriteMacro(wf, FtnEndMacro);
			FtnEndMacro = NULL;
		}
		// switch back from footnote div
		wf->SelectDiv(CurrentDiv);
	}
	ParaRefNum = PreFtnParaRefNum;
	FtnParaEnding = false;
  InFootnote = false;

	if (FootType != 'E') {
		RestoreInheritForm(); // inherit restore at end of ftn
		if (CharStylesUsed)  // ensure ending brace written
			WriteCharForm(wf);
		StartCharForm = StartFtnCharForm;
		EndCharForm = EndFtnCharForm;
		InCharForm = InFtnCharForm;
		CharFormSaved = FtnCharFormSaved;
		LastCF = FtnLastCF;
		Curr = FtnCurr;
		Last = FtnLast;
	}

  FtnMarker = 0;
  InPara = true;

	if (FootType == 'E')
		RestoreState(CurrState);
	else {
	  TabsChanged = true;
		LinePos = FtnPosition;
		RestoreState(FullState);
	}
}


void
RTFwr::WriteFtnEnd(RTFwfile *wf)
{
	if (HelpForm == true)
		wf->putGE(EndFtnWithSpace);
	else {
		wf->putGE();
		wf->putGE();
	}
	wf->putEnd();
  FtnMarker = 0;
}



// index entries


bool RTFwr::IndexEntry = false;
bool RTFwr::InIndexGroup = false;
bool RTFwr::IndexCounted = false;
bool RTFwr::StartingIndexGroup = false;
bool RTFwr::KeepIXMarkerIDs = false;
short RTFwr::IndexCount = 0;
char *RTFwr::IndexBuf = NULL;
short RTFwr::IndexPos = 0;
short RTFwr::IndexSPos = 0;
short RTFwr::IndexAllocUnit = 128;
short RTFwr::IndexAlloc = 0;
bool RTFwr::IdxColon = false;
short RTFwr::IndexColon = 0;
bool RTFwr::InIdxSort = false;
bool RTFwr::NoIdxSort = true;
bool RTFwr::IdxSeeAlso = false;
bool RTFwr::NoSeeAlso = false;
DClist RTFwr::SeeAlsoList;
bool RTFwr::IdxStartRange = false;
DClist RTFwr::StartRangeList;
bool RTFwr::IdxEndRange = false;
DClist RTFwr::EndRangeList;
bool RTFwr::IndexRanges = true;
bool RTFwr::InIdxGroup = false;
bool RTFwr::BookmarkIXRanges = true;


void
RTFwr::ProcIdxType(RTFctl *cp)
{
	switch (cp->siz()) {  // write phase
		case 1:  // no_page marks See Also entries
			IdxSeeAlso = true;
			break;
		case 2:  // one_page (default) returns to normal
			IdxSeeAlso = false;
			break;
		case 3:  // start range, add to CurrRanges
			if (IndexRanges)
				IdxStartRange = true;
			break;
		case 4:  // end range, delete from CurrRanges
			if (IndexRanges)
				IdxEndRange = true;
			break;
		default:
			break;
	}
}


void
RTFwr::PutIndexEntry(RTFctl *cp)
{
  char *sptr;
  char *eptr;
  char *iptr;
	short len;
	short i;
	short count;
  uns sch;
  char wch[12];
	bool ansi = true;

  if (cp->dt() == imnem) {  // single char form
#if 0
		if ((cp->siz() > 0x10000) // sign extended char
		 || (((cp->siz() >> 8) & 0xFF) == 0xFF))
			sch = (uns) (cp->siz() & 0xFF);
		else
			sch = (uns) cp->siz();
    //sch = (uns) cp->siz();
		//if (((sch >> 8) & 0xFF) == 0xFF)
		//	sch &= 0xFF;
#endif
		unl uch = cp->siz();
		iptr = wch;
		if (uch <= 0x7F) {
			wch[0] = (unc) (uch & 0x7F);
			wch[1] = '\0';
			len = 1;
		}
		else if (uch > 0xFFFF)  // BMP only
			return;
		else {
			sch = (uns) (uch & 0xFFFF);

			switch (sch) {
				case 0xA0:
				case 0x2002:
				case 0x2003:
				case 0x2007:
				case 0x2009:
					sch = ' ';
					break;
				case 0xAD:	// disc hyphen
					sch = 0;
					break;
				case 0x2011:
					sch = '-';
					break;
				case 0x2044:
					sch = '/';
					break;

				case 130:
				case 145:
				case 146:
					sch = '\'';
					break;

				case 132:
				case 147:
				case 148:
					sch = '"';
					break;

				case 150:
				case 151:
					sch = '-';
					break;

				case 133: // ...
					sch = 0;
					break;

				//case '\\':
				case 0x7B:
				case 0x7D:
					sch = 0;
					break;

				default:
					//if (sch > 0xFF)  // ignore other Unicode syms for now
					//	sch = 0;
					break;
			}
			if (sch == 0)
				return;

			if (sch > 0x7F) {
				ansi = false;
				sprintf(iptr, "\\u%-hd ", (short) sch);
				len = strlen(iptr);
			}
			else if (sch == '\\') {
				wch[0] = '\\';
				wch[1] = '\\';
				wch[2] = '\0';
				len = 2;
			}
			else {
				wch[0] = (char) (sch & 0x7F);
				wch[1] = '\0';
				len = 1;
			}

#if 0
			wch[0] = (char) sch;
			if (sch == '\\') {
				wch[1] = '\\';
				wch[2] = 0;
				len = 2;
			}
			else {
				wch[1] = 0;
				len = 1;
			}
			iptr = wch;
#endif
		}
  }
	else {	// string form, all legal chars
    if ((cp->siz() <= 1) // may be only a nul
     || (cp->ex() == NULL))
      return;
		len = (short) cp->siz();
		iptr = (char *) cp->ex();
  }


  if (StartingIndexGroup) {	// start of grouped entry
    IndexCounted = false;
    StartingIndexGroup = false;
  }
  else if (InIndexGroup) {	// continuation of entry
    if (IndexPos  // something already written
     && IndexCounted) // and not finished
			IndexPos--;	 // so overwrite last terminator
  }
  else {  // start of single string entry
    IndexCounted = false;
  }

	// add up chars that will result in a size increase
	count = 0;
	if (ansi) {
		for (i = 0; i < len; i++) {
			switch (iptr[i]) {
				case ':':
				case '\\':
				case '{':
				case '}':
					count++;
				default:
					break;
			}
		}
		len += count;
	}

	char *chform = WriteIdxCharForm();
	long chlen = 0; 
	if (chform)
		len += (short) (chlen = strlen(chform));

  if (IndexPos) { // adding on
    if (IndexAlloc < (IndexPos + len + 1)) {
			while (IndexAlloc < (IndexPos + len + 1))
				IndexAlloc += IndexAllocUnit;
			IndexBuf = (char *) ResizeMem(IndexBuf, IndexAlloc);
		}
  }
  else {
		while (IndexAlloc < (len + 1))
			IndexAlloc += IndexAllocUnit;
    IndexBuf = (char *) NewMem(IndexAlloc);
  }

  sptr = &IndexBuf[IndexSPos];
  eptr = &IndexBuf[IndexPos];

	ProcIdxFlags(IndexPos + 1);

	if (chform) {
		strcpy(eptr, chform);
		eptr += chlen;
	}

	if (ansi) {
		while (*iptr) {  // strings are always null-terminated
			if (InIdxSort) {
				if (!HelpForm
				 && !NoIdxSort) {
					while (*iptr && (*iptr != ']'))
						*eptr++ = *iptr++;
				}
				else { // just skip for help
					while (*iptr && (*iptr != ']'))
						iptr++;
				}
				if (*iptr) {
					InIdxSort = false;
					iptr++;
				}
			}
			else {
				switch (*iptr) {
					case '[':  // sort string spec
						InIdxSort = true;
						if (!HelpForm
						 && !NoIdxSort) {
							iptr++;
							*eptr++ = ';';
							while (*iptr && (*iptr != ']'))
								*eptr++ = *iptr++;
						}
						else { // just skip for help
							while (*iptr && (*iptr != ']'))
								iptr++;
						}
						if (*iptr) {
							InIdxSort = false;
							iptr++;
						}
						break;
					case ':':  // colons are subentries, escape with backslash
						if (HelpForm == true) {
							if (IdxColon
							 && !IndexColon) {
								IndexColon++;
								*eptr++ = ':';
							}
							else if (*(eptr - 1) != ',')
								*eptr++ = ',';
							*eptr++ = ' ';
							iptr++; // skip colon if making Help index
						}
						else {
							*eptr++ = '\\';
							*eptr++ = *iptr++;
						}
						break;
					case '\\': // pass existing escapes through
						//if (*(iptr + 1) == '[')
						//	iptr++;					
						//else
						//	*eptr++ = '\\';
						if (*++iptr == '\\') // keep doubled backslash
							*eptr++ = '\\';    // but don't escape other chars
						*eptr++ = *iptr++;
						break;
					case '{':  // escape braces if unescaped
					case '}':
						*eptr++ = '\\';
						*eptr++ = *iptr++;
						break;
					case ';':	 // split up multiple entries
						if (eptr > sptr) { // not empty
							ProcIdxFlags((sptr - IndexBuf) + 1);
							*eptr++ = '\0';
							sptr = eptr;
							IndexPos = IndexSPos = eptr - IndexBuf;
							ResetIdxCharForm();
							InIdxSort = false;
							IndexColon = 0;
							if (!IndexCounted)
								IndexCount++;
							else IndexCounted = false;
						}
						iptr++;
						break;
					case 130:  // single quotes
					case 145:
					case 146:
						if (HelpForm == true)
							*iptr = '\'';
						goto normal;
					case 132:  // double quote
					case 147:
					case 148:
						if (HelpForm == true)
							*iptr = '"';
						goto normal;
					case 0x99: // things to omit (TM)
						if (HelpForm == true) {
							iptr++; // skip if making Help index
							break;
						}
						goto normal;

					normal:
					default:	 // write text
						*eptr++ = *iptr++;
						break;
				}
			}
		}
	}
	else {
		strncpy(eptr, iptr, len);
		eptr += len;
	}

	if (eptr > sptr) {  // wrote something, not terminated
		*eptr++ = '\0';		// terminate it
		ProcIdxFlags((sptr - IndexBuf) + 1);
		IndexPos = eptr - IndexBuf;
		if (!IndexCounted) {  // first time
			IndexCounted = true; // indicate entry in progress
			IndexCount++;		// update entry count
		}
  }

	IndexSPos = sptr - IndexBuf;

  if (IndexPos)	// not empty
    IndexEntry = true;
  else {
    IndexEntry = false;
		IndexCounted = false;
		InIdxSort = false;
    IndexCount = 0;
		IndexSPos = 0;
    DeleteMem(IndexBuf);
    IndexAlloc = 0;
		IndexBuf = NULL;
		IndexColon = 0;
		SeeAlsoList.empty();
  }
}


void
RTFwr::ProcIdxFlags(long ent)
{
	if (IdxSeeAlso) {
		SeeAlsoList.add(ent);
		IdxSeeAlso = false;
	}
	if (IdxStartRange) {
		StartRangeList.add(ent);
		IdxStartRange = false;
	}
	if (IdxEndRange) {
		EndRangeList.add(ent);
		IdxEndRange = false;
	}
}

unc RTFwr::IdxBold = 0;
unc RTFwr::IdxItal = 0;
unc RTFwr::IdxLine = 0;
unc RTFwr::LastIdxBold = 0;
unc RTFwr::LastIdxItal = 0;
unc RTFwr::LastIdxLine = 0;

void
RTFwr::ResetIdxCharForm(void)
{
	if (HelpForm)
		return;

	IdxBold = IdxItal = IdxLine = 0;
	LastIdxBold = LastIdxItal = LastIdxLine = 1;
}

void
RTFwr::ProcIdxCharForm(pprop *cf)
{
	if (HelpForm)
		return;

	// only change bold, ital, and line; ignore any others
	IdxBold = cf->bold;
	IdxItal = cf->ital;
	IdxLine = (cf->line > 2) ? 2 : cf->line;
}

char *
RTFwr::WriteIdxCharForm(void)
{
	if (HelpForm)
		return NULL;

	char *scp = NewName(14);
	char *cp = scp;

	if (IdxBold && (LastIdxBold != IdxBold)) {
		switch (IdxBold) {
			case 1:
				strcpy(cp, "\\b0 ");
				cp += 4;
				break;
			case 2:
				strcpy(cp, "\\b ");
				cp += 3;
				break;
			default:
				break;
		}
		LastIdxBold = IdxBold;
		IdxBold = 0;
	}

	if (IdxItal && (LastIdxItal != IdxItal)) {
		switch (IdxItal) {
			case 1:
				strcpy(cp, "\\i0 ");
				cp += 4;
				break;
			case 2:
				strcpy(cp, "\\i ");
				cp += 3;
				break;
			default:
				break;
		}
		LastIdxItal = IdxItal;
		IdxItal = 0;
	}

	if (IdxLine && (LastIdxLine != IdxLine)) {
		switch (IdxLine) {
			case 1:
				strcpy(cp, "\\ul0 ");
				cp += 5;
				break;
			case 2:
				strcpy(cp, "\\ul ");
				cp += 4;
				break;
			default:
				break;
		}
		LastIdxLine = IdxLine;
		IdxLine = 0;
	}

	*cp = '\0';
	if (strlen(scp))
		return scp;
	DeleteName(scp);
	return NULL;
}


DCnlist RTFwr::TopIdxList;
long RTFwr::TopIdxCount = 0;
long RTFwr::IdxDiv = 0;


void
RTFwr::WriteIndexEntry(RTFwfile *wf)
{
	if ((IndexBuf == NULL)
	 || (IndexPos == 0))
		return;	

  char *iptr = IndexBuf;
	short len = 0;
	char *subptr = NULL;
	char *topptr = NULL;
	long toplen = 0;
	bool also = false;
	long prevdiv = 0;

	if (IdxDiv) {
		prevdiv = wf->GetDiv();
		wf->SelectDiv(IdxDiv);
	}

  for (short i = 0; i < IndexCount; i++) {
		if (((len = strlen(iptr)) == 1)
		 && (*iptr == ' ')) {   // empty
			iptr += 2;
			continue;
		}
		also = SeeAlsoList.find((long) (iptr - IndexBuf) + 1); 
		if (NoSeeAlso
		 && also)
			; // skip this part
		else if (IdxType == 'H') {		// write K footnote
			if ((HlpVer == '4')
			 && (DisambiguateIndex != 'S')    // may write level 1 keys
			 && ((subptr = strchr(iptr, IdxColon ? ':' : ',')) != NULL)) {
				toplen = (subptr - iptr) + 1;   // has a level 1 key
				topptr = NewName(iptr, toplen); // include key terminator
				if (DisambiguateIndex == 'Y')   // always write level 1
					PutHelpKeyword(wf, topptr);
				else if (TopIdxList(topptr) == NULL) { // not in list, add
					// DisambiguateIndex must be 'T' or 'N' at this point
					TopIdxList.add(topptr, ++TopIdxCount);
					PutHelpKeyword(wf, topptr);   // writes only if not listed
				}
				else DeleteName(topptr);
			}
			PutHelpKeyword(wf, iptr); // write entire keyword now
		}
		else if (BookmarkIXRanges
		 && EndRangeList.find((long)  (iptr - IndexBuf) + 1)) { // only ending bookmark 
			PutIXBook(wf, iptr, false);
		}
		else {
			char *srbook = NULL;
			if (BookmarkIXRanges
			 && StartRangeList.find((long)  (iptr - IndexBuf) + 1))
				srbook = PutIXBook(wf, iptr, true);
			wf->putGS();
			wf->putCW("xe");
			wf->putCW("v");
#if 0
			if ((len = strlen(iptr)) > KeywordLimit) {
				iptr[KeywordLimit] = '\0';	// trim it
				if (iptr[KeywordLimit - 1] == '\\')
					iptr[KeywordLimit - 1] = '\0';	// trim trailing backslash
			}					
			//if (IdxColon
			// && ((topptr = strchr(iptr, ':')) != NULL))
			//	*topptr = ',';
#endif
			wf->putGS();
			if (also) {
				uns cnt = 0;
				char *sref = strstr(iptr, " see");
				if (!sref)
					sref = strstr(iptr, " See");
				if (sref) {
					while ((*sref == ' ')
					 || (*sref == '0')
					 || (*sref == 'i')
					 || (*sref == 'b')
					 || (*sref == '\\')
					 || (*sref == ',')
					 || (*sref == '.'))
						sref--;
					sref++;
					cnt = sref - iptr;
					while ((*sref == ' ')
					 || (*sref == ',')
					 || (*sref == '.'))
						sref++;
				}
				if (cnt) {		
					wf->putStr(iptr, cnt);
					wf->putCW("txe");
					wf->putStr(sref);
				}
				else
					wf->putStr(iptr);
			}
			else
				wf->putStr(iptr);
			wf->putGE();
			if (srbook) {
				wf->putGS();
				wf->putCW("rxe");
				wf->putStr(srbook);
				wf->putGE();
			}
			wf->putGE();
			//wf->putChar(' ');
		}
    iptr += (len + 1);
  }

  IndexEntry = false;
	IndexCounted = false;
	InIdxSort = false;
  IndexCount = 0;
	IndexPos = 0;
	IndexSPos = 0;
  DeleteMem(IndexBuf);
  IndexAlloc = 0;
	IndexBuf = NULL;
	IndexColon = 0;
	//IdxEnded = true;
	SeeAlsoList.empty();

	if (IdxDiv) {
		wf->SelectDiv(prevdiv);
	}
}


char *
RTFwr::PutIXBook(RTFwfile *wf, char *iptr, bool start)
{
	if (!iptr || (*iptr == '\0'))
		return NULL;

	char *bptr = NewName("IX", strlen(iptr) + 2);
	char *eptr = bptr + 2;

	while (*iptr) {
		if (isalnum(((int) *iptr) & 0xFF))
			*eptr++ = *iptr;
		iptr++;
	}
	*eptr = '\0';

	wf->putGS();
	wf->putCS('*');
	wf->putCW(start ? "bkmkstart" : "bkmkend");
	wf->putStr(bptr);
	wf->putGE();
	if (start)
		BkmkstartCount++;
	else
		BkmkendCount++;

	return bptr;
}

void
RTFwr::PutHelpKeyword(RTFwfile *wf, char *iptr)
{
	FtnMarker = 'K';
	WriteFtnStart(wf);
	WriteFtnNum(wf);
	if (((short) strlen(iptr)) > KeywordLimit)
		iptr[KeywordLimit] = '\0';	// trim it
	wf->putStr(iptr);
	WriteFtnEnd(wf);
}



// Help cloning


char RTFwr::HelpCloneBuf[HelpCloneBufMax];
short RTFwr::HelpCloneCount;
bool RTFwr::InitFileFtn = false;
long RTFwr::HelpCloneDiv = 0;
long RTFwr::CurrentDiv = 0;


void
RTFwr::StartHelpClone(RTFwfile *wf)
{
	HelpCloneDiv = wf->NewDiv();
	//wf->SelectDiv(CurrentDiv);
}

void
RTFwr::EndHelpClone(RTFwfile *wf, char *dvch)
{
	if (CurrentDiv) {  // inside diversion
		if (dvch)
			wf->putStr(dvch);
	}
	else
		wf->WriteDiv(HelpCloneDiv);

	wf->DeleteDiv(HelpCloneDiv);
	wf->SelectDiv(CurrentDiv);
	HelpCloneDiv = 0;
}


void
RTFwr::PutHelpCloneBuf(RTFwfile *wf, uns ch)
{
	if (HelpCloneCount == 0) { // starting
		if (ch == ' ')					 // skip leading spaces
			return;
		if (HelpCloneDiv == 0)	 // turn on diversion
			StartHelpClone(wf);
	}
	wf->SelectDiv(HelpCloneDiv);

	if (HelpCloneCount < HelpCloneBufMax)
		HelpCloneBuf[HelpCloneCount++] = (char) ch;
}


void 
RTFwr::WriteHelpClone(RTFwfile *wf, pprop *fmt)
{
	short len = BrowseDigitsMax + 1;
	char *prefix = NULL;
	char *dvch = NULL;
	char *extra = NULL;
	char *cp = NULL;
	char *suf = fmt->hsuf;
	unc cltype = fmt->clone;

	while (HelpCloneCount > 0) {
		if (HelpCloneBuf[HelpCloneCount - 1] == ' ')  // trailing blank
			HelpCloneBuf[--HelpCloneCount] = '\0';
		else break;
	}

	if (HelpCloneCount == 0) { // empty, and no diversion
		if (fmt->hmacro) {
			cp = fmt->hmacro;
			fmt->hmacro = NULL;
			if (Last.line > 1) {
				if (HyperHelp) {
					RestoreState(LastState);
					wf->putGE();
				}
				else
					wf->putCW((Last.line == 3) ? "uldb0" : "ul0");
				Last.line = 1;
			}
			wf->putGS();
			wf->putCW("v");
			wf->putText('!');
			wf->putStr(cp);
			wf->putGE();
		}
		return;
	}

	if (HelpCloneDiv) {
		dvch = wf->ReadDiv();
		wf->SelectDiv(CurrentDiv);  // slider or zero
	}

	//while (HelpCloneBuf[HelpCloneCount - 1] == ' ') { // trailing blank
	//	if (--HelpCloneCount == 0)
	//		return;
	//}

	if (HelpCloneCount == 1)  // add a period to avoid single-char error
		HelpCloneBuf[HelpCloneCount++] = '.';

	HelpCloneBuf[HelpCloneCount] = '\0';

	if (cltype & 16)  // make into local refs
		prefix = CurrBrowseID;

	bool cntref = true;

	if (InitFileFtn)
		WriteFileFtn(wf);

	if (cltype & 2) { // context fn
		if (fmt->href)
			WriteExHelpFtn(wf, '#', fmt->href);
		else {
			WriteContextFtn(wf, prefix, suf);
			cntref = false;
		}
	}

	if (cntref
	 && (fmt->cntform > 1)) { // need ref for cnt file
		CntRefStr = NewName(strlen(CurrFilePrefix) + 14);
		sprintf(CntRefStr, "%sCNT%ld", CurrFilePrefix, ++CntRefNum);
		WriteExHelpFtn(wf, '#', CntRefStr);
	}

	if (cltype & 4) { // title fn
		if (((Curr.hscroll == 0)
		  && (HelpTitleScroll == false))
		 || (Curr.hscroll == 2)) { // ensure nonscrolling title
			//wf->putCW("keepn");
			NonscrollZone = true;
		}
		if (fmt->htvis)
			WriteHelpFtn(wf, '$', fmt->htsuf);

		WriteBrowseID(fmt->hbrowse);  // always update ID
		if (fmt->hbrowse
		 || AutoBrowse)
			WriteBrowseFtn(wf);

		if (fmt->hmacro)
			WriteExHelpFtn(wf, '!', fmt->hmacro);
		if (fmt->hbuild)
			WriteExHelpFtn(wf, '*', fmt->hbuild);
		if (fmt->hwindow)
			WriteExHelpFtn(wf, '>', fmt->hwindow);
		if (suf != NULL) {  // entry macro, window, or build tag
			while (*suf) {
				if ((*suf != '!')
				 && (*suf != '>')
				 && (*suf != '*'))
					break;
				for (cp = suf + 1; *cp; cp++) { // scan for end
					if ((*cp == '!')
					 || (*cp == '>')
					 || (*cp == '*'))
						break;
				}
				if (cp > (suf + 1)) {  // actual content
					extra = new char[cp - suf];
					strncpy(extra, suf + 1, (cp - suf) - 1);
					extra[(cp - suf) - 1] = '\0';
					WriteExHelpFtn(wf, *suf, extra);
					delete [] extra;
				}
				suf = cp;
			}
		}
		if (fmt->infot)
			WriteExHelpFtn(wf, 'S', fmt->infot);

		HelpTitlePending = false;
	}

	if (fmt->cntform > 0) {
		unc clevel = fmt->cntlev;
		char *cfile = fmt->hjfile;
		if ((cfile == NULL)
		 && AddCntFileName)
			cfile = CurrHelpFile;
		char *cwind = fmt->hwindow;
		if ((cwind == NULL)
		 && AddCntWindowName)
			cwind = CntMainWindow;

		if (*CntHeadStr != '\0')  // last one was V
			WriteOutCntEntry(clevel);

		switch (fmt->cntform) {
			case 1:  // heading only at cntlev
				WriteCntHead(clevel);
				WriteOutCntHead();
				break;
			case 2:  // topic at cntlev
				WriteCntTopic(clevel, prefix, cfile, cwind, 
                      suf, cntref, false);
				WriteOutCntTopic();
				break;
			case 3:  // B: heading at cntlev, topic at cntlev+1
			both:
				WriteCntHead(clevel);
				WriteCntTopic(clevel + 1, prefix, cfile, cwind, 
                      suf, cntref, true);
				WriteOutCntHead();
				WriteOutCntTopic();
				break;
			case 4:  // V: like B, but postpone WriteOut
				if (CntLastForm == 0) {  // force first one to B if V
					CntLastForm = 1;
					goto both;
				}
				WriteCntHead(clevel);
				WriteCntTopic(clevel + 1, prefix, cfile, cwind, 
                      suf, cntref, true);
				break;
			default: // none
				break;
		}

		CntPrevOrigLevel = CntLastOrigLevel;
		CntLastOrigLevel = fmt->cntlev;
	}

	if (cltype & 8) { // keyword fn
		if (fmt->hkey) {  // new style
			if (fmt->hkey & 1)
				WriteHelpFtn(wf, 'K');
			if (fmt->hkey & 2)
				WriteHelpFtn(wf, 'A');
			if (fmt->hkey & 0xFC) // top six bits 1..26 is A..Z
				WriteHelpFtn(wf, ((fmt->hkey & 0xFC) >> 2) + 0x40);
		}
		else if ((suf != NULL)
		 && isalpha(*suf)) { // avoid using a jump suffix
			WriteHelpFtn(wf, *suf);
			suf = NULL;  // prevent later use in context string
		}
		else
			WriteHelpFtn(wf, 'K');
	}

	if (HelpCloneDiv)
		EndHelpClone(wf, dvch);
	else {
		for (int i = 0; i < HelpCloneCount; i++) {	// real text
      if (HelpCloneBuf[i] > 0x7E) {
        wf->putSpec(HelpCloneBuf[i]);
        LinePos += CharWid;
      }
      else {
				wf->putText(HelpCloneBuf[i]);
				AddChar(HelpCloneBuf[i]);
      }
		}
	}

	bool rest_state = false;

	if (cltype & 1) { // hidden text
		if (Last.line > 1) {
			if (HyperHelp) {
				rest_state = true;
				wf->putGE();
			}
			else
				wf->putCW((Last.line == 3) ? "uldb0" : "ul0");
			Last.line = 1;
		}
		wf->putGS();
		wf->putCW("v");
		if (fmt->hmacro) {
			wf->putText('!');
			wf->putStr(fmt->hmacro);
			fmt->hmacro = NULL;
		}
		else {
			if (fmt->hvis)
				wf->putText(fmt->hvis);
			else if (suf
			 && ((*suf == '*')
				|| (*suf == '%')))
				wf->putText(*suf++);
			else if (!UseGreen)
				wf->putText('*');
			if (prefix)
				wf->putStr(prefix);
			if (fmt->href)
				wf->putStr(fmt->href);
			else
				PutContext(wf, HelpCloneBuf, true);
			if (fmt->hwindow) {
				wf->putText('>');
				wf->putStr(fmt->hwindow);
			}
			if (fmt->hjfile) {
				wf->putText('@');
				wf->putStr(fmt->hjfile);
			}
			if (suf)
				wf->putStr(suf);
		}
		wf->putGE();
	}

	HelpCloneCount = 0;
	if (rest_state == true)
		RestoreState(LastState);
}


void 
RTFwr::WriteCntHead(unc level)
{
	unc *chp = (unc *) CntHeadStr;
	unc *lim = (unc *) CntHeadStr + CntHeadMax - 2;
	*chp++ = level;
	*chp++ = ' ';

	unc	*txt = (unc *) HelpCloneBuf;
	while (*txt) {
		switch (*txt) { 
			case '\\':  // punct that requires escape
			case '=':
			case '}':
			case '{':
				*chp++ = '\\';
				*chp++ = *txt;
				break;

			case 0x99:  // characters to omit (TM)
				break;

			default:   // everything else is permitted
				*chp++ = *txt;
				break;
		}
		txt++;
		if (chp >= lim)
			break;
	}
	*chp = '\0';
}


long RTFwr::CntRefNum = 0;
char *RTFwr::CntRefStr = NULL;
char *RTFwr::CntBTextStr = NULL;


void 
RTFwr::WriteCntTopic(unc level, char *px, char *cfile, char *wind, 
                     char *suf, bool cntref, bool bt)
{
	unc *txt = NULL;
	unc *lim = (unc *) CntTopicStr + CntTopicMax - 3;
	unc *chp = (unc *) CntTopicStr;
	*chp++ = level;
	*chp++ = ' ';

	if ((bt == true)
	 && (CntBTextStr != NULL))
		txt = (unc *) CntBTextStr;
	else
		txt = (unc *) HelpCloneBuf;

	while (*txt) {
		switch (*txt) { 
			case '\\':  // punct that requires escape
			case '=':
			case '}':
			case '{':
				*chp++ = '\\';
				*chp++ = *txt;
				break;

			case 0x99:  // characters to omit (TM)
				break;

			default:   // everything else is permitted
				*chp++ = *txt;
				break;
		}
		txt++;
		if (chp >= lim)
			break;
	}

	CntTopicRefStr = (char *) chp;  // start of ref string
	*chp++ = '=';
	*chp = '\0';

	if (px
	 && ((chp + strlen(px)) < lim)) {
		strcpy((char *) chp, px);
		chp += strlen(px);
	}
	if (cntref
	 && ((chp + strlen(CntRefStr)) < lim)) {
		strcpy((char *) chp, CntRefStr);
		chp += strlen(CntRefStr);
		DeleteName(CntRefStr);
		CntRefStr = NULL;
	}
	else {
		txt = (unc *) HelpCloneBuf;
		while (*txt) {
			if (isalnum(((int) *txt) & 0xFF))
				*chp++ = *txt;
			else switch (*txt) {   // punct to allow
				case '.':
				case '_':
					*chp++ = *txt;
					break;

				default:
					break;
			}
			txt++;
			if (chp >= lim)
				break;
		}
		if (suf
		 && ((chp + strlen(suf)) < lim)) {
			strcpy((char *) chp, suf);
			chp += strlen(suf);
		}
	}
	if (cfile
	 && ((chp + strlen(cfile) + 1) < lim)) {
		*chp++ = '@';
		strcpy((char *) chp, cfile);
		chp += strlen(cfile);
	}
	if (wind
	 && ((chp + strlen(wind) + 1) < lim)) {
		*chp++ = '>';
		strcpy((char *) chp, wind);
		chp += strlen(wind);
	}
	*chp = '\0';
}


char RTFwr::CntHeadStr[CntHeadMax];
char RTFwr::CntTopicStr[CntTopicMax];
char *RTFwr::CntTopicRefStr = NULL;
unc RTFwr::CntLastOrigLevel = '0';
unc RTFwr::CntPrevOrigLevel = '0';
unc RTFwr::CntLastLevel = '0';
unc RTFwr::CntLastForm = 0;
char *RTFwr::CurrHelpFile = NULL;
char *RTFwr::CurrHelpBase = NULL;
bool RTFwr::AddCntFileName = false;
bool RTFwr::AddCntWindowName = false;
char *RTFwr::CntMainWindow = "main";


void 
RTFwr::WriteOutCntEntry(unc& newlevel)
{
	unc promoted = 0;

	// stored entry is type V, with strings set up for B output

	if ((newlevel >= *CntTopicStr)  // same level as this topic, so child
	 && (newlevel > CntLastOrigLevel)) { // unless only promoted above it
		promoted = *CntTopicStr;  // promote current one if necessary
		WriteOutCntHead();  // write out B style
		if (!LastTopicEmpty)
			WriteOutCntTopic(); // suppress this if no body text
	}
	// if empty, and no children, no need for entry
	else if (LastTopicEmpty) {  // cancel it, leave levels alone
		if (newlevel == 0)    // ending file, may have just head
			WriteOutCntHead();  // write out B style
		else {
			*CntHeadStr = '\0';
			*CntTopicStr = '\0';
		}
	}
	else {  // no children, so try to put out as topic (merged)
		if (newlevel == CntLastOrigLevel)
			promoted = *CntHeadStr;
		if ((*CntHeadStr < CntLastLevel)
		 || ((*CntHeadStr == CntLastLevel)
		  && (*CntHeadStr < CntPrevOrigLevel))) { // need to move left
			if (*CntHeadStr < '2') {  // cannot go to level 0, so use B anyway
				WriteOutCntHead();
				WriteOutCntTopic();
			}
			else   // back up two levels to go left one
				WriteOutCntMerged(*CntHeadStr - 1);
		}
		else  // topic, but leave level alone
			WriteOutCntMerged();
	}

	if (promoted)
		newlevel = promoted;  // promote current one if necessary
}


void 
RTFwr::WriteOutCntHead(void)
{
	CntFile->putStr(CntHeadStr);
	CntFile->putCr();
	CntLastLevel = *CntHeadStr;
	CntLastForm = 1;
	*CntHeadStr = '\0';
}


void 
RTFwr::WriteOutCntTopic(void)
{
	CntFile->putStr(CntTopicStr);
	CntFile->putCr();
	CntLastLevel = *CntTopicStr;
	CntLastForm = 2;
	*CntHeadStr = '\0';
	*CntTopicStr = '\0';
}


void 
RTFwr::WriteOutCntMerged(unc level)
{
	// merge head and topic to make topic at level
	if (level != 0)
		*CntHeadStr = level;
	CntLastLevel = *CntHeadStr;
	CntFile->putStr(CntHeadStr);
	CntFile->putStr(CntTopicRefStr);
	CntFile->putCr();
	CntLastForm = 2;
	*CntHeadStr = '\0';
	*CntTopicStr = '\0';
}



void 
RTFwr::WriteHelpFtn(RTFwfile *wf, unc fntype, char *suf)
{
	unc ch = 0;
	short i = 0;
	short lim = HelpCloneCount;

	FtnMarker = fntype;
	WriteFtnStart(wf);
	WriteFtnNum(wf);
	if ((fntype == '$')
	 && (lim > 127))
		lim = 127;  // limit for title fm
	for (i = 0; i < lim; i++) {
		ch = (unc) HelpCloneBuf[i];
    if (ch > 0x7E) {
			if (ch == 0x99)  // exclusions (TM)
				continue;
      wf->putSpec(ch);
		}
    else
			wf->putText(ch);
	}
	if (suf) {
		i = 0;
		while ((ch = (unc) suf[i++]) != '\0') {
			if (ch > 0x7E) {
				if (ch == 0x99)  // exclusions (TM)
					continue;
				wf->putSpec(ch);
			}
			else
				wf->putText(ch);
		}
	}
	WriteFtnEnd(wf);
}


void 
RTFwr::WriteExHelpFtn(RTFwfile *wf, unc fntype, char *str)
{
	FtnMarker = fntype;
	WriteFtnStart(wf);
	WriteFtnNum(wf);
	wf->putStr(str);
	WriteFtnEnd(wf);
}


void 
RTFwr::WriteContextFtn(RTFwfile *wf, char *pfix, char *suf)
{
	FtnMarker = '#';
	WriteFtnStart(wf);
	WriteFtnNum(wf);
	if (pfix)
		wf->putStr(pfix);
	PutFtnContext(wf, HelpCloneBuf);
	if (suf)
		wf->putStr(suf);
	WriteFtnEnd(wf);
}


void 
RTFwr::PutContext(RTFwfile *wf, char *txt, bool min)
{
	if (txt == NULL)      // ignore if empty
		return;

	if (!min) {
		if ((*txt == '*')	    // no color or underline
		 || (*txt == '%')) {	// no color, keep underline
			wf->putText(*txt);  // allow as first char only
			txt++;
		}
		else if (XrefHvis)
			wf->putText(XrefHvis);
		else if (!UseGreen)
			wf->putText('*');
	}
	if (*txt == '!') {    // pass macro calls unchanged
		wf->putStr(txt);
		return;
	}

	bool window = false;
	bool fileid = false;
	bool filesx = false;

	while (*txt) {
		if (isalnum(((int) *txt) & 0xFF))
			wf->putText(*txt);
		else switch (*txt) {
			case '>':   // secondary window, should precede any file
				if (min)
					break;
				if (window == false) {
					if (filesx
					 && XrefFileSuffix) {
						wf->putText('.');
						wf->putStr(XrefFileSuffix);
						filesx = false;
					}
					window = true;
					wf->putText('>');
				}
				break;
			case '@':   // file identifier, must follow any window
				if (min)
					break;
				if (fileid == false) {
					fileid = true;
					filesx = true;
					wf->putText('@');
				}
				break;
			case ':':   // file drive
			case '/':   // file path
      case '-':   // allow in file name
				if (fileid == true)
					wf->putText(*txt);
				break;

			case '.':   // normal chars
			case '_':
				wf->putText(*txt);
				break;

			default:
				break;
		}
		txt++;
		if (!min && (*txt == 0)) {
			if ((window == false)
			 && (XrefWindow != NULL)
			 && (*XrefWindow != '\0')) {
				wf->putText('>');
				window = true;
				txt = XrefWindow;
			}
			else if ((fileid == false)
			 && XrefFile)	{
				wf->putText('@');
				fileid = true;
				filesx = true;
				txt = XrefFile;
			}
		}
	}
	if (filesx
	 && XrefFileSuffix) {
		wf->putText('.');
		wf->putStr(XrefFileSuffix);
		filesx = false;
	}
}


void 
RTFwr::PutFtnContext(RTFwfile *wf, char *txt)
{
	if (txt == NULL)      // ignore if empty
		return;

	while (*txt) {
		if (isalnum(((int) *txt) & 0xFF))
			wf->putText(*txt);
		else switch (*txt) {   // punct to allow
			case '.':
			case '_':
				wf->putText(*txt);
				break;

			default:
				break;
		}
		txt++;
	}
}


void 
RTFwr::WriteBrowseID(char *hbstr)
{
	char browse[BrowseDigitsMax + 1];
	size_t len = 1;

	if (BrowseStart == true)
		BrowseStart = false;
	else BrowseCurr += BrowseStep;

	if (AutoBrowse)
		BrowseDigits = 0;
	else if (hbstr)
		len = strlen(hbstr) + 2;
	else if (BrowsePrefix
	 && (*BrowsePrefix != '\0'))
		len = strlen(BrowsePrefix) + 2;

	if (BrowseDigits) {
		sprintf(browse, "%0*d", BrowseDigits, BrowseCurr);
		len += strlen(browse);
	}
	
	if (AutoBrowse) {
		CurrBrowseID = "auto";
	}
	else {
		if (CurrBrowseID)
			DeleteName(CurrBrowseID);
		CurrBrowseID = NewName(len);
		if (hbstr)
			strcpy(CurrBrowseID, hbstr);
		else if (BrowsePrefix)
			strcpy(CurrBrowseID, BrowsePrefix);
	}

	if (BrowseDigits) {
		if (strlen(CurrBrowseID) != 0)
			strcat(CurrBrowseID, ":");
		strcat(CurrBrowseID, browse);
	}
}


void 
RTFwr::WriteBrowseFtn(RTFwfile *wf)
{
	FtnMarker = '+';
	WriteFtnStart(wf);
	WriteFtnNum(wf);
	wf->putStr(CurrBrowseID);
	WriteFtnEnd(wf);
}


void
RTFwr::ProcHelpBreak(RTFwfile *wf)
{
	if (Curr.hide || Last.hide) {
		wf->putGE();
		Last.hide = Curr.hide = 0;
	}
	if (Curr.fnote) {
		RestoreState(LastState);
		WriteFtnEnd(wf);
		Curr.fnote = 0;
	}
	if (Curr.clone || Last.clone) {
		if (Last.clone)  // ending a HelpTag process
			WriteHelpClone(wf, &Last);
		else
			WriteHelpClone(wf, &Curr);
		Curr.clone = ParaForm.clone = Last.clone = 0;
	}
	if ((Curr.hmacro != Last.hmacro)
	 && (Last.hmacro != NULL))
		WriteHelpClone(wf, &Last);
}


void
RTFwr::WriteFileFtn(RTFwfile *wf)
{
	char *txt = wf->getBaseName();
	FtnMarker = '#';
	WriteFtnStart(wf);
	WriteFtnNum(wf);
	PutFtnContext(wf, txt);
	WriteFtnEnd(wf);
	InitFileFtn = false;  // turn off
}



// end of dwrtfan.cpp

