 
// dwrtftx.cc is the code for dcl RTF text writing, JHG
// Copyright (c) 1993-1998 by Omni Systems, Inc.  All rights reserved.

#include "dcappl.h"
#include "dcctlg.h"
#include "dcfili.h"
#include "dctxfx.h"
#include "dcfmtr.h"
#include "dcini.h"
#include "dcmacr.h"
#include "dcmxl.h"
#include "dwrtf.h"
#include "dwrtffl.h"
#include "dcicu.h"


// selection switch


bool
RTFwr::ProcTx(RTFctl *cp)
{
  switch((cp->rp)->scan_op) {
    case nothing:   // just put with its group for later
      break;

    case pass:      // ignore, don't save 
      return false;

		case supsiz:    // get metrics for supers and subs
			SuperSize = (uns) cp->siz();
			return false;
		case supoff:
			SuperShift = cp->siz();
			return false;
		case subsiz:
			SubSize = (uns) cp->siz();
			return false;
		case suboff:
			SubShift = cp->siz();
			return false;
		case scapsiz:
			SCapSize = (uns) cp->siz();
			return false;

    case docdefs:   // done storing doc defaults
      SetCharSet();
      SetDefForm();
      return false;

    case docres:    // done storing doc resources
      FormsStored = FormNum;
      return false;

    case txfile:    // process during write phase 
      break;

    case fontlist:  // store font info
      ProcFont(cp);
      if (InGraphic) {
        ProcGr(cp);
        return false;
      }
			if (RemFonts
			 && (FormsStored > 1))  // past resources, so override
				MarkFontUsed(Curr.fnum);
      break;

    case formdef:   // store style info
		  InFormDef = true;
      ProcForm(cp);
      return false;
    case formend:
      StoreForm();
		  InFormDef = false;
      return false;
    case formname:
      ProcFormName(cp);
      return false;
    case formtype:
      ProcFormType(cp);
      return false;

    case fmpxdef:   // prefix info
			InFormPx = true;
			if (InFormDef)
	      return false;
			break;
    case fmpxend:
		  InFormPx = false;
			if (InFormDef)
	      return false;
			break;
    case fmsxdef:   // suffix info
			InFormSx = true;
			if (InFormDef)
	      return false;
			break;
    case fmsxend:
		  InFormSx = false;
			if (InFormDef)
	      return false;
			break;

    case formtag:
			return false; // ignore tags for now
      TagForm(cp);
      break;
    case formspc:  // ****
      break;
		case formscan:
			if (InStradTbCell)
				break;
			if (RepStyles || RemStyles || RemFonts
			 || WriteMissingForms)
				MarkFormUsed(cp);
			if (WriteMissingForms
			 && (LastPage > LastMasterPage)
			 && !InVarDef)
				AddFormsFound(cp);
			if (InFormPx || InFormSx)
				return false;
			if (InGraphic) {
				ProcGr(cp);
				return false;
			}
			if (HelpForm
			 || UseHL)
				CheckHelpForm(cp);
			if (!HelpForm)
				CheckSideHeads(cp);
			CheckFormat(cp);
			break;
		case fmtxbeg:
			InFormText = true;
			break;
		case fmtxend:
			InFormText = false;
			break;

    case formchk:
			if (InStradTbCell)
				break;
			if ((InFormPx || InFormSx)
			 && !InAnumDef) {
				if ((cp->rp)->write_op == puttab)
					return true;
				return false;
			}
      if (InGraphic) {
        ProcGr(cp);
        return false;
      }
			else if (InRunGroupDef)
				break;
			else if (InCondInds) {
				SetCondTypo(cp);
				return false;
			}
      else if (InFormDef) {
        SetCurrProps(cp);
        return false;
      }  
      else if (InFormTagDef)
        SetCurrProps(cp);
			else switch ((cp->rp)->write_op) {
				case parstart:
					ProcParaStart(cp);
					break;
				case linend:
		      ProcLineEnd(cp);
					if (StoreText
					 && ConfigHere
					 && (cp->siz() == 1)) // hard return
						PutTextStoreBuf('\0');
					break;
				case puttab:
					if (StoreText
					 && (InDocBody)
					 && (!InIdxGroup)
					 && (!InRunGroupDef)
					 && (!WhiteText || !HideWhite)
					 && SectNum)
						PutTextStoreBuf(' ');
					break;
				case parend:
					ProcParaEnd(cp);
					break;
				case putcolr:
					ProcHyperColor(cp);
					break;
				default:
					break;
			}
      break;


		case ukeyset:
			break;
		case ukeyout:
			MXLwr::KeylibOutput = NewName((char *) cp->ex(), cp->siz());
			break;
		case ukeylib:
			MXLwr::OutputKeydefLibPath = NewName((char *) cp->ex(), cp->siz());
			break;
		case ukeyid:
			MXLwr::OutputKeydefLibID = NewName((char *) cp->ex(), cp->siz());
			break;
		case ukeyitm:
			break;
		case ukeykeys:
			MXLwr::OutputKey = NewName((char *) cp->ex(), cp->siz());
			break;
		case ukeysrc:
			MXLwr::OutputKeySrc = NewName((char *) cp->ex(), cp->siz());
			break;
		case ukeyiref:
			if (MXLwr::OutputKeySrc) {
				long len = strlen(MXLwr::OutputKeySrc) + cp->siz() + 2;
				char *nsrc = NewName(MXLwr::OutputKeySrc, len);
				strcat(nsrc, "#");
				char *nid = NewName((char *) cp->ex(), cp->siz());
				strcat(nsrc, nid);
				MXLwr::OutputKeySrc = nsrc;
				DeleteName(nid);
			}
			break;
		case ukeyeitm:
			MXLwr::SourceKeydefs.add(MXLwr::OutputKey, (long) MXLwr::OutputKeySrc);
			MXLwr::SourceKeydefCount++;
			MXLwr::OutputKey = NULL;
			MXLwr::OutputKeySrc = NULL;
			break;
		case ukeyeset:
			break;


    case language:  // determine default language
      ProcLang(cp);
      return false;
    case charset:   // determine default character set
      CurrCharSet = cp->dat();
      return false;


		case conddef:
			StartCondDef(cp);
			break;
		case condedef:
			CurrCondInfo = NULL;
			break;

		case condnam:
			SetCondName(cp);
			break;

		case condsind: // group of condition indicators 2, 15:
			InCondInds = true;  // lining 11 0-3, bold 4 0-6, ital 5 0-1
			break;
		case condeind:
			InCondInds = false;
			break;

		case condcol:  // color for condition
		case condbcol: // background color
		case condccol: // changebar color
			SetCondColor(cp);
      return false;

		case condcch:  // changebar chars
		case condsimg: // filename of start image
		case condsalt: // starting alt text
		case condeimg: // filename of end image
		case condealt: // ending alt text
			SetCondInd(cp);
      return false;

		case condvis:
			if (CurrCond
			 && (cp->siz() == 1)) {
				HiddenConds.add(CurrCond);
				CurrCondInfo->hidden = true;
			}
			break;

		case condset:  // scan phase
			//SetCond(cp);
			break;
		case condres:
			//EndCond(cp);
			break;

		case conddis:
			ShowHiddenCond = (cp->siz() == 1) ? true : false;
			break;


    case flowlist:  // collect segments of each text stream
			FlowFrameNum = 0;
      ProcFlow(cp);
      break;
		case flowprev:
			if (cp->dat() == 0)
				FirstFlowFrame = true;
			return false;
		case flowframe:
			FlowFrameCtl = cp;
			FlowFrameNum = cp->dat();
			if (FirstFlowFrame) {
				PageStartList.add(FlowFrameNum);
				FirstFlowFrame = false;
			}
			break;
		case flowname:
			ProcFlowName(cp);
			break;

    case flowused:
			if (InFormPx || InFormSx)
				break;
      if (InGraphic) {
        ProcGr(cp);
        return false;
      }
      if (LastPage > LastMasterPage)
        MarkSectUsed(cp);
			HyperFormEnding = false;  // turn off after real text
			HyperColorEnding = false;

			if (StoreText
			 && (InDocBody)
			 && (!InIdxGroup)
			 && (!InRunGroupDef)
			 && (!WhiteText || !HideWhite)
			 && SectNum)
				ProcTextStoreText(cp);
      break;

		case sideflow:
			if (cp->siz() > 0) {
				if (SideHFlowList.find(CurrFlow) == false)
					SideHFlowList.add(CurrFlow);
			}
			return false;
		case sidepos:
			if (ForceSideHeadPos) {
				SideHFlip = FSideHFlip;
				SideHPos = FSideHPos;
				return false;
			}
			switch (cp->siz()) {
				case 1:  // left
					SideHFlip = false;
					SideHPos = 1;
					break;
				case 2:  // right
					SideHFlip = false;
					SideHPos = 2;
					break;
				case 3:  // inner
					SideHFlip = true;
					SideHPos = 1;
					break;
				case 4:  // outer
					SideHFlip = true;
					SideHPos = 2;
					break;
				default:
					SideHFlip = false;
					SideHPos = 0;
					break;
			}
			return false;
		case sidewid:
			SideHWide = SHWidth ? SHWidth : (short) GetTwips(cp->siz(), pg_unit);
			return false;
		case sidegap:
			SideHGap = SHGap ? SHGap : (short) GetTwips(cp->siz(), pg_unit);
			if (CurrFlow
			 && FlowFrameNum
			 && (SideHFrameList.find(FlowFrameNum) == NULL)) { // FM 4.x
				SideHPropsInFlow = true;
				shprop *shp = new shprop;
				shp->shwid = SideHWide;
				shp->shgap = SideHGap;
				shp->shpos = SideHPos;
				shp->shflip = SideHFlip;
				SideHFrameList.add(shp, FlowFrameNum);
				if (SideHTFlowList.find(CurrFlow) == NULL)
					SideHTFlowList.add(shp, CurrFlow);
			}
			return false;
		case setrunin:
			Curr.rungroup = new RTFgroup(*cp);
		  if (DCLlevel < LevelMax)
				GroupStack[DCLlevel] = Curr.rungroup;
			InRunGroupDef = true;
			break;
		case endrunin:
			if (!InFormDef
			 && Curr.runin)
				RuninOverrides.add(Curr.rungroup, ParaRefNum);
			InRunGroupDef = false;
			break;

    default:
      assert(0);
      return false;
  }
  return true;
}



// properties processing


pprop RTFwr::Curr;
pprop RTFwr::Last;
pprop RTFwr::DefForm;
pprop RTFwr::ParaForm;
pprop RTFwr::CForm;
pprop RTFwr::LastCF;
pprop RTFwr::NoForm;
pprop RTFwr::FtnCurr;
pprop RTFwr::FtnLast;
pprop RTFwr::FtnLastCF;

bool RTFwr::CaselessMatch = true;
bool RTFwr::SpacelessMatch = true;
bool RTFwr::WildcardMatch = true;

char RTFwr::OutType = 'S';
char RTFwr::HelpType = 'S';
char RTFwr::QuoType = 'S';
char RTFwr::BulType = 'S';
bool RTFwr::NoDing = false;
char RTFwr::SmallCaps = 'S';
char RTFwr::IdxType = 'S';
short RTFwr::KeywordLimit = 63;
char RTFwr::DisambiguateIndex = 'T';
char RTFwr::XrfType = 'S';
bool RTFwr::FieldHyper = true;
bool RTFwr::WrapXrefs = true;
bool RTFwr::ExtXrefPages = true;
bool RTFwr::WordPerf = false;
bool RTFwr::Word8 = true;
bool RTFwr::Word2000 = false;
bool RTFwr::Word2002 = false;
bool RTFwr::Word2003 = true;
bool RTFwr::WordVerSet = false;
char *RTFwr::IDFileName = NULL;
char *RTFwr::PrjFileName = NULL;
bool RTFwr::UseFrameIX = true;
bool RTFwr::UseLocalFileID = false;
char RTFwr::HUType = 'R';
bool RTFwr::UseHL = true;
bool RTFwr::UseGreen = true;
char RTFwr::FootType = 'S';
char *RTFwr::FtnSeparator = NULL;
bool RTFwr::EndFtnWithSpace = true;
char RTFwr::HFType = 'S';
char *RTFwr::SectionPrefix = "#";
bool RTFwr::UseTextFrames = true;
bool RTFwr::WrapTFs = true;
bool RTFwr::WrapHTFs = true;
short RTFwr::HFGap = 0;
char RTFwr::SideHType = 'L';
short RTFwr::SHVAdjust = 0;
char RTFwr::PSpcType = 'F';
char RTFwr::NegLead = 'N';
bool RTFwr::KeepAnchors = false;
char RTFwr::PageType = 'N';
char RTFwr::PBMethod = 'P';
bool RTFwr::LTabRMarg = true;
bool RTFwr::RTabRMarg = true;
bool RTFwr::UnderlineTabs = false;
char RTFwr::OccTabType = 'R';
char RTFwr::RMarginTabs = 'B';
char RTFwr::EmpFrType = 'S';
char RTFwr::RunInFrames = 'S';
bool RTFwr::EmbedEqs = true;
short RTFwr::EqVert = 8;
short RTFwr::EqHor = 2;
char RTFwr::TbTitleType = 0;
char RTFwr::TbRuleType = 'C';
bool RTFwr::TbRuleAdjust = false;
char RTFwr::TbGraphics = 'S';
bool RTFwr::StripTables = false;
bool RTFwr::StrippedCellPar = false;
bool RTFwr::TbFixed = true;
bool RTFwr::ForceTbLineBreaks = false;
bool RTFwr::FrameTbTitle = false;
bool RTFwr::ForcedBreak = false;
bool RTFwr::ShiftWideTablesLeft = true;
bool RTFwr::HideWhite = false;
bool RTFwr::TextColor = true;
bool RTFwr::BackColor = true;
bool RTFwr::AllowZeroFSize = true;
char *RTFwr::RefFrameFormat = "";
char *RTFwr::RefFrameDefFormat = NULL;
char RTFwr::RefFrames = 'G';
char *RTFwr::DefFontName = "Times New Roman";
long RTFwr::DefFontSize = 0;
uns RTFwr::DefFontWidth = 144;
uns RTFwr::DefLineSpace = 0;
uns RTFwr::DefTabWidth = 0;
bool RTFwr::ExtXRefs = true;

bool RTFwr::XML = false;
char *RTFwr::Template = NULL;
bool RTFwr::TemplateAutoUpdate = true;
uns RTFwr::TextRescale = 0;
uns RTFwr::TableRescale = 0;
uns RTFwr::GraphicRescale = 0;

bool RTFwr::HelpForm = false;
char RTFwr::HlpVer = '4';
char *RTFwr::HPJName = NULL;
bool RTFwr::WriteHelpProjectFile = false;
char *RTFwr::HelpCopyright = NULL;
bool RTFwr::HelpCopyDate = false;
bool RTFwr::HlpVerSet = false;
bool RTFwr::Altura = false;
bool RTFwr::HyperHelp = false;
bool RTFwr::ForceBmc = false;
bool RTFwr::HelpSectBreaks = true;
bool RTFwr::BareFnSym = false;
char RTFwr::FtnSpace = 'A';
bool RTFwr::HelpLineBreak = true;
bool RTFwr::HelpTitlePending = false;
bool RTFwr::HelpTitleScroll = false;
bool RTFwr::ExtendHelpNoScroll = false;
bool RTFwr::NonscrollZone = false;
bool RTFwr::HelpTitleSpace = false;
bool RTFwr::HelpTitleIndent = false;
uns RTFwr::HelpTabLimit = 32;
bool RTFwr::NoXRPopup = false;
bool RTFwr::ULineOver = false;
char *RTFwr::FrameStyle = NULL;

bool RTFwr::TitleBrowse = false;
bool RTFwr::AutoBrowse = false;
bool RTFwr::BrowseStart = true;
int  RTFwr::BrowseCurr = 5;
int  RTFwr::BrowseStep = 5;
int  RTFwr::BrowseDigits = 4;
char *RTFwr::BrowsePrefix = NULL;
char *RTFwr::CurrBrowseID = NULL;

char RTFwr::CntType = 'N';
char *RTFwr::CntBase = NULL;
char *RTFwr::CntTitle = NULL;
char *RTFwr::CntTopic = NULL;
char *RTFwr::CntTopHead = NULL;
char *RTFwr::CntName = NULL;
char *RTFwr::CntStartFile = NULL;

char RTFwr::BmcStr[BmStrMax] = "bmct ";
char RTFwr::BmlStr[BmStrMax] = "bmlt ";
char RTFwr::BmrStr[BmStrMax] = "bmrt ";
char *RTFwr::BulletStr = "bullet.bmp";
bool RTFwr::TransparentBitmaps = false;

char RTFwr::GrTxType = 'E';
bool RTFwr::ClipText = false;
char RTFwr::ClipType = 'M';
short RTFwr::ClipLimit = 20;
bool RTFwr::UseGrPreviews = false;
char RTFwr::GrPaths = 'R';
char RTFwr::GrNames = 'R';
bool RTFwr::RemoveGraphics = false;
bool RTFwr::RemoveNontextFrames = false;
bool RTFwr::FrameBorders = false;
short RTFwr::GrVert = 4;
bool RTFwr::GrTbSpace = true;
char RTFwr::MetaType = 'I';
bool RTFwr::MetaMetric = false;
short RTFwr::PicScale = 176;
int RTFwr::MetaNameWid = 5;
int RTFwr::MetaNumWid = 3;
char RTFwr::DashLType = 'N';
bool RTFwr::RectIsPoly = true;
short RTFwr::GrBkMode = 1;
short RTFwr::GrStretchMode = 0;
char *RTFwr::GrDefFont = "Arial";
short RTFwr::GrDefFSize = 180;
short RTFwr::GrTxScale = 100;
short RTFwr::GrTxWidth = 0;
short RTFwr::GrTxVAdj = 0;
short RTFwr::BitmapDPI = 0;
short RTFwr::EqExDPI = 120;
short RTFwr::EqExpand = 125;
short RTFwr::Ras24Mode = 0;
bool RTFwr::CompRasters = true;
char RTFwr::RasBorders = 'N';
char RTFwr::FrBorders = 'C';
bool RTFwr::BitmapFlip = false;
bool RTFwr::UseTopSpaceAbove = false;
bool RTFwr::NoBlankFirstGTLine = true;
bool RTFwr::NoGTULine = false;
bool RTFwr::FrExHigh = false;
bool RTFwr::WrapAllFrames = false;
bool RTFwr::EmbedBMPs = true;
bool RTFwr::EmbedWMFs = true;
bool RTFwr::NameWMFsAsBMPs = false;
bool RTFwr::NameGraphics = false;

char *RTFwr::CustCode = NULL;
bool RTFwr::CharStyles = true;  // true for best Word effect, false for WP
bool RTFwr::RepStyles = false;  // true if styles replaced
bool RTFwr::RemStyles = false;  // true to minimize style use
bool RTFwr::RemFonts = false;  // true to minimize style use
bool RTFwr::UseLang = true;     // true causes WP to lose mind

bool RTFwr::UseLog = true;
bool RTFwr::ShowLog = false;
char *RTFwr::LogIniPrefix = "_m2";
char *RTFwr::LogFileName = "_m2g_log.txt";
char *RTFwr::EditorFileName = "notepad.exe";
DClog *RTFwr::Log = NULL;
unc RTFwr::LogLevels[logmax] = { 0, 1, 1, 1, 1, 0 };

bool RTFwr::MergeCurrFormatProps = false;
bool RTFwr::FormatBaseSet = false;



void
RTFwr::SetDefForm(void)
{
  DefForm = Curr;
  DefForm.stag = "Normal";
  DefForm.stype = 1;
	DefForm.bcolr = 9;
	if (DefaultLanguage)
		DefForm.lang = DefaultLanguage;
	if (DefaultCodePage)
		DefForm.cpg = DefaultCodePage;

	DefForm.fwid = DefFontWidth;
	if (DefFontSize)
		DefForm.fsize = (uns) DefFontSize;
	if (DefLineSpace)
		DefForm.lspace = DefLineSpace;

  Last = DefForm;
  FormList.add(&DefForm, 1L);
  FormNameList.add(DefForm.stag, 1L);
  if (DefForm.tcount)
    FormTabList.add(DefForm.tabs, 1L);
}



// format setting and calling


DCvlist RTFwr::FormList;
DCnlist RTFwr::FormNameList;
DCnlist RTFwr::FormRenameList;
DCnlist RTFwr::FormNewNameList;
DCilist RTFwr::FormRefList;
DCilist RTFwr::FormTypeList;
DCvlist RTFwr::FormTabList;
DCvlist RTFwr::CharFormStack;
uns RTFwr::FormNum = 1;
uns RTFwr::DCLFormNum;
uns RTFwr::FormsStored = 1;
uns RTFwr::CharFormLevel = 0;
char *RTFwr::FormName;
char *RTFwr::FinalFormName;
bool RTFwr::InFormDef;
bool RTFwr::InFormTagDef;
bool RTFwr::InFormPx;
bool RTFwr::InFormSx;
bool RTFwr::InFormText;

bool RTFwr::CharStylesUsed = false;
bool RTFwr::StartCharForm = false;
bool RTFwr::InCharForm = false;
bool RTFwr::EndCharForm = false;
bool RTFwr::CharFormSaved = false;
bool RTFwr::CharFormsNest = false;
bool RTFwr::StartFtnCharForm = false;
bool RTFwr::InFtnCharForm = false;
bool RTFwr::EndFtnCharForm = false;
bool RTFwr::FtnCharFormSaved = false;


void
RTFwr::ProcForm(RTFctl *cp)
{
  Curr = DefForm;
  DCLFormNum = cp->dat();
}


void
RTFwr::ProcFormType(RTFctl *cp)
{
	Curr.stype = (uns) cp->siz();
	FormTypeList.replace(Curr.stype, DCLlevel);

  if ((Curr.stype = (uns) cp->siz()) == 4) {  // for ruling formats
    ProcRuleForm();
  }
  else if (Curr.stype == 3) {           // for table formats
    ProcTblForm();
  }
  else if (Curr.stype == 2) {           // for inherited formats
    Curr.fnum = Curr.fsize = Curr.bold = Curr.ital = Curr.strike = 0;
    Curr.base = Curr.caps = Curr.scaps = Curr.outl = Curr.shad = 0;
    Curr.colr = Curr.fenc = 0;
		Curr.cpg = 0;
    Curr.line = Curr.fnote = Curr.hide = 0;
	  Curr.lmode = 1;
  }
}


void
RTFwr::ProcFormName(RTFctl *cp)
{
	if (cp->siz() == 0)
		return;
  FormName = NewName((char *) cp->ex(), (size_t) cp->siz());
}


void
RTFwr::StoreForm(void)
{
	char *tname = NULL;

	Curr.stype = (uns) FormTypeList.find(DCLlevel + 1);

  if (Curr.stype == 4) { // ruling format
    StoreRuleForm();
    return;
  }
  else if (Curr.stype == 3) { // table format
    StoreTblForm();
    return;
  }
  pprop *form = new pprop;
  Curr.snum = Curr.snext = FormNum++;  // stored num is 1 less than ref

	if (IniFile
	 && IniFile->Section("Styles")
	 && IniFile->Find(FormName)
	 && ((tname = IniFile->StrRight()) != NULL)) { // mapped to an RTF style
	  FormRenameList.add(FormName, FormNum);
		FormName = tname;
	  FormNewNameList.add(FormName, FormNum);
	}

  Curr.stag = FormName;
	FinalFormName = FormName;

	if (IniFile) {
		if (IniFile->Section("StyleReplacements")
		 && IniFile->Find(FormName)
		 && ((tname = IniFile->StrRight()) != NULL)) { // mapped to another RTF style
			FormRenameList.add(FormName, FormNum);
			FormsReplaced.add(FormNum);
			RepFormName.add(tname, FormNum);
			FormNewNameList.add(tname, FormNum);
			RepStyles = true;
			FinalFormName = tname;
		}

		if (HelpForm)
			DCini::ProcIniSectGroup("HelpStyles", "HStyleSet");
		else
			DCini::ProcIniGroup("WStyleSet");

		if ((HelpForm == true)
		 && IniFile->Section("HelpTags")  // fallback
		 && IniFile->Find(FormName)) {
			ProcHelpForm();
			if ((CntType != 'N')
			 && IniFile->Section("HelpCntStyles")
			 && IniFile->Find(FormName))
				ProcHelpCntForm();
		}

	}

	GetFormatFileInfo(FinalFormName);  // override Curr with format info

	*form = Curr;
  FormList.add(form, FormNum);
  FormRefList.add(DCLFormNum, FormNum);
  FormNameList.add(FormName, FormNum);
  FormName = NULL;
  if (form->tcount)
    FormTabList.add(form->tabs, FormNum);
}


void
RTFwr::ProcHelpCntForm(char *hform)
{
	if (hform == NULL)
		hform = IniFile->StrRight();

	switch (toupper(*hform)) {
		case 'H':
			Curr.cntform = 1;
			break;
		case 'T':
			Curr.cntform = 2;
			break;
		case 'B':
			Curr.cntform = 3;
			break;
		case 'V':
			Curr.cntform = 4;
			break;
		default:
			break;
	}
	if (Curr.cntform
	 && isdigit(hform[1])) {
		if ((Curr.cntlev = hform[1]) == '0')
			Curr.cntlev = '9';
	}
	else if (isdigit(hform[0])) { // just a digit
		if ((Curr.cntlev = hform[0]) == '0')
			Curr.cntlev = '9';
		Curr.cntform = 4;           // default to V
	}
	DeleteName(hform);
}


void
RTFwr::ProcHelpForm(void)
{
	Curr.help = IniFile->UCRight();
	if (!isdigit(Curr.help)) {  // character value
		switch (Curr.help) {
			case '$':	 // title
			case '+':	 // browse
			case '#':	 // context
			case '!':	 // entry macro
			case '*':	 // build tag
			case '>':  // window type
			case 'K':	 // keywords
			case 'A':	 // alt keywords
			case '@':  // comment
				Curr.fnote = Curr.help;
				Curr.hide = 0;
				Curr.line = 1;
				Curr.lmode = 1;
				break;
			default:
				if (isalpha(Curr.help)) { // more alternate keywords
					Curr.fnote = Curr.help;
					Curr.hide = 0;
					Curr.line = 1;
					Curr.lmode = 1;
				}
				break;
		}
	}
	else {	// numeric value
		Curr.help = (char) IniFile->IntRight();

		// bits used for Curr.clone:
		// 1 hidden ref
		// 2 context fn
		// 4 title fn
		// 8 keyword fn
		// 16	local, gets browse prefix
		// 32	popup topic
		// 64 slider topic; used alone to end any current slider
		// 128 unused

		switch (Curr.help) {
			case 4:
				Curr.clone = 1;	// hidden ref
			case 1:
				Curr.line = 3;	// jump
				Curr.lmode = 1;
				break;
			case 5:
				Curr.clone = 1;	// hidden ref
			case 2:
				Curr.line = 2;	// popup
				Curr.lmode = 1;
				break;
			case 3:
				Curr.hide = 2;	// ref
				Curr.line = 1;
				Curr.lmode = 1;
				break;
			case 6:
				Curr.clone = 2; // context fn
				break;
			case 7:
				Curr.clone = 4; // title fn
				break;
			case 8:
				Curr.clone = 6; // context and title fns
				break;
			case 9:
				Curr.clone = 34; // popstart 32 + context fn 2
				Curr.slide = 1;	 // popups always slide
				break;
			case 10:
				Curr.clone = 17;	// hidden local 16 + jump ref 1
				Curr.line = 3;
				Curr.lmode = 1;
				break;
			case 11:
				Curr.clone = 17;	// hidden local 16 + popup ref 1
				Curr.line = 2;
				Curr.lmode = 1;
				break;
			case 12:
				Curr.clone = 18; // local 16 + context fn 2
				break;
			case 13:
				Curr.clone = 50; // popstart 32 + local 16 + 2
				Curr.slide = 1;	 // popups always slide
				break;
			case 14:
				Curr.clone = 8;  // keyword fn
				break;
			case 15:	 // topic with title and keyword
				Curr.clone = 14; // context 2 + title 4 + keyword 8 fn
				break;
			case 16:	 // topic with title and key that slides to next topic
				Curr.clone = 14; // context 2 + title 4 + keyword 8 fn
				Curr.slide = 1;
				break;
			case 17:   // tag that ends sliding topic, resumes previous
				Curr.slide = 2; // slide suspend, ignored if not in slider
				break;

			case 20:	 // set start of hypertext marker zone
				SetHyperStart(3);
				break;

			default:
				break;
		}
		if (IniFile->Section("HelpData")
		 && IniFile->Find(FormName))
			Curr.hsuf = IniFile->StrRight();
	}
}


void
RTFwr::TagForm(RTFctl *cp)
{
	if (InFormDef)
		return;  // do not recurse if inside table def
	InFormTagDef = true;
  ProcForm(cp);
  ProcFormName(cp);
	Curr.stype = 1;
}


void
RTFwr::UseForm(RTFctl *cp)
{
  uns formnum = (uns) cp->siz();

	if (formnum == 0) {
		if (cp->dat() == 1) {
			if (InFormTagDef) {  // end of a form_tag definition
				StoreForm();
				FormsStored = FormNum;
				InFormTagDef = false;
			} 
			else
				ParaForm = Curr;	// for "Init Store" after para overrides
		}
		else if (cp->dat() == 2) {	// for "Inherit Restore"
			RestoreInheritForm();
			CheckHyperEnd(cp);
		}
		DCini::CurrPendList.all(DCini::GetCurrSettings);
		return;
	}

	pprop	*cform = (pprop *) FormList.find(FormRefList(formnum));
	if (cform == NULL)
		return;

  if (cp->dat() == 1)  // init format
    InitForm(cform);
  else if (cp->dat() == 2)  // inherit format
    InheritForm(cform);

	DCini::CurrPendList.all(DCini::GetCurrSettings);
	CheckHyper(cp);
}



DClist RTFwr::FormsUsed;
DCilist RTFwr::FinalForms;
long RTFwr::FinalFormNum = 0;
DClist RTFwr::FormsReplaced;
DCnlist RTFwr::RepFormName;


void
RTFwr::MarkFormUsed(RTFctl *cp)
{
	FormsUsed.add(FormRefList(cp->siz()));
}


void
RTFwr::SetFinalForms(void)
{
	pprop	*cform = NULL;
	FinalFormNum = 1;
	FormsUsed.add(1);  // ensure "Normal" is around
	FontsUsed.add(1);  // ensure default font is around
	if (FrameStyle
	 && *FrameStyle)
		FormsUsed.add(FormNameList(FrameStyle));

	for (long i = 1; i <= FormNum; i++) {
		if (RemStyles
		 && !FormsUsed.find(i))
			continue;  // skip unused styles
		if (RepStyles
		 && FormsReplaced.find(i))
			continue;  // skip replaced styles
		// really using this style
		if ((cform = (pprop *) FormList.find(i)) != NULL)
			FontsUsed.add(cform->fnum);
		FinalForms.add(FinalFormNum++, i);
	}

	if (!RepStyles)
		return;  // done if no styles replaced

	char *cfname = NULL;
	long cfnum = 1;
	long ffnum = 1;

	for (i = 1; i <= FormNum; i++) { // replace with final of replacement
		if (RemStyles
		 && !FormsUsed.find(i))
			continue;  // still skip unused styles
		if (FormsReplaced.find(i)) {
			if (((cfname = (char *) RepFormName.find(i)) == NULL) // name of replacement
			 || ((cfnum = FormNameList(cfname)) == 0)) // find replacement
				continue;
			if ((ffnum = FinalForms.find(cfnum)) != 0) // replacement mapped,
				FinalForms.add(ffnum, i);  // so map to final of replacement
			else                                  // not mapped, error
				FinalForms.add(FinalFormNum++, i);  // so use after all
		}
	}
}


uns
RTFwr::GetFinalForm(uns snum)
{
	if (!(RemStyles || RepStyles))
		return snum;
	snum = (uns) FinalForms.find(snum + 1);
	return (snum ? (snum - 1) : 0);
}


void
RTFwr::CheckHelpForm(RTFctl *cp)
{
	if (InFormText)
		return;

  uns formnum = (uns) cp->siz();
	bool isInit = (cp->dat() == 1) ? true : false;

	if (!isInit) {  // only inherit format can end
		if (HyperForm) {  // hyper ending, may be new or restore
			HyperForm = false;
			HyperFormEndID = (long) cp;
			if (HyperJump) {
				HyperFormEndList.add((long) cp);
				HyperJump = false;
			}
			HyperFormEnding = true;
		}
	}

	if (formnum == 0) { 
		if (isInit) { // init store may need update
			if (HyperColor)
				ParaHyperColor = true;
		}
		else {    // inherit restore
			if (HyperColor       // was in color
			 && !ParaHyperColor)
				ProcHyperColorEnd(cp);

			if (ParaHyperColor) // restoring color
				ProcHyperColorStart(cp);

			PopOver = false;
		}
		return;
	}

	pprop	*cform = (pprop *) FormList.find(FormRefList(formnum));

	if (cform == NULL)
		return;

	if (isInit) {  // init format
		DCini::CurrPendList.all(DCini::SetCurrID);
		Curr = *cform;
		if (cform->algn == 3) // justified
			JustPara = ParaRefNum;
		if (UseHyperColor)
			ParaHyperColor = (cform->colr > 2) ? true : false;
		if (cform->runin)
			RuninPara = ParaRefNum;
		if (!HyperPara
		 && ((HyperPara = cform->paralink) > 0))
			HyperParaList.add(HyperPara, ParaRefNum);
	}
	else  { // inherit format
		HyperForm = true;
		HyperFormStartID = (long) cp;
		if (cform->help == 2) { // PopOver
			HyperVal = 2;
			PopOver = true;
		}
		if (HyperJump)
			HyperFormStartList.add(HyperVal, (long) cp);
		HyperFormEnding = false;
	}

	if (UseHyperColor) {
		if (HyperColor
		 && (cform->colr == 2))
			ProcHyperColorEnd(cp);
		if ((HyperForm == false)
		 && (cform->colr > 2))
			ProcHyperColorStart(cp);
	}

	if (!HelpForm)    // not generating help topic file
		return;

	long hptype = cform->slide * 4;	// for 0, 4, or 8

	if ((cform->clone & 4) // title property set
	 || (cform->fnote == '$'))	// raw title fnote
		hptype |= 1;
	else if (cform->clone & 32) // popup context
		hptype |= 2;
	else if (!cform->slide
	 && !cform->cntform)	// no special treatment required
		return;

	if (cform->cntform)
		hptype |= 16;

	long refnum = InTbl ? TableStartCtl->siz() : ParaRefNum;
	long hprev = HelpTitleParaList.find(refnum); 
	HelpTitleParaList.replace(hptype | hprev, refnum);

	if (cform->mref) {
		MakeRef = true;
		CurrRef = 0;
	}
	if ((cform->pref)
	 && CurrRef) { 
		MakeRefList.add(CurrRef, refnum);
	}
}


void
RTFwr::CheckSideHeads(RTFctl *cp)
{
	if (HelpForm == true)
		return;

  pprop *cform = (pprop *) FormList.find(FormRefList(cp->siz()));
  if (cform == NULL)
    return;
	Curr = *cform;

	if ((cform->sideh > 1) 
	 && (cform->bspace > 0)
	 && PrevStartCtl)
		SideHSpaceList.add(cform->bspace, PrevStartCtl->siz());

	if (cp->dat() == 1) {  // init format
		DCini::CurrPendList.all(DCini::SetCurrID);
		if (cform->algn == 3) // justified
			JustPara = ParaRefNum;
		if (!HyperPara
		 && ((HyperPara = cform->paralink) > 0))
			HyperParaList.add(HyperPara, ParaRefNum);
	}
}


void
RTFwr::CheckFormat(RTFctl *cp)
{
	// Curr already set per format
	TextStoreHere = Curr.tstore ? true : false;
	TextStoreName = Curr.tstore;
	ConfigHere = Curr.config;
	StoreText = (TextStoreHere || ConfigHere) ? true : false;

	if (Curr.section) {
		if (!CurrSection
		 || stricmp(CurrSection, Curr.section)
		 || Curr.sectstart)
			SectionStart.add(Curr.section, ParaRefNum);
		CurrSection = Curr.section;
	}
}

void
RTFwr::UseFTag(RTFctl *cp)
{
  uns formnum = (uns) FormNameList((char *) cp->ex());
  pprop *cform = (pprop *) FormList.find(formnum);
  if (cform == NULL)
    return;

  if (cform->stype == 1)
    InitForm(cform);
  else if (cform->stype == 2)
    InheritForm(cform);
}


void
RTFwr::UseSpecFmtTag(RTFctl *cp)
{
	// cp->dat() is format type, cp->ex() is name  ****

}



void
RTFwr::InitForm(pprop *cform)
{
  Curr = *cform;
	if (Curr.runin
	 && NoRuninPara.find(ParaRefNum))
		Curr.runin = 0;

	ParaForm = Curr; // set up for future "Inherit Restore"

	if (InPxSxFrame) {
		Curr.help &= 64;
		Curr.clone = 0;
		Curr.slide &= 2;
		Curr.fnote = 0;
		Curr.hyper = 0;
		Curr.line = 1;
		Curr.lmode = 1;
		Curr.hide = 0;
	}

	if (!HyperPara
	 && Curr.pref
	 && CurrRef) {
		HyperPara = ((Curr.help == 2) && (Curr.line == 2)) ? 2 : 3;
		PrepRefStr(CurrRef);
		HyperParaList.add(HyperPara, ParaRefNum);
	}

	//CodeStoreVar = Curr.cstore;

	//if (HyperPara)
	//	SetHyperStart(HyperPara);

	if (Curr.config)
		DCini::ProcParaConfig(ParaRefNum);
}


void
RTFwr::RestoreInheritForm(void)
{
	if (InIndexGroup) {
		ProcIdxCharForm(&ParaForm);
		return;
	}

	if (InheritPending)
		InheritPending = false;

	if (CharFormsNest
	 && (CharFormLevel > 0)) {  // restore from preceding Curr if any
		pprop *oform = (pprop *) CharFormStack.find(CharFormLevel);
		if (oform) {
			Curr = *oform;
			CharFormStack.remove(CharFormLevel--);
		}
		else
			oform = &ParaForm;
		RestoreInheritParaForm(oform);
		// more fixup here ****
	}
	else
		RestoreInheritParaForm(&ParaForm);

 	if (CharStylesUsed) {
		EndCharForm = true;
		if (CharFormSaved
		 && (!CharFormsNest
		  || (CharFormLevel == 0))) {
			RestoreState(LastState);
			CharFormSaved = false;
		}
		StartCharForm = false;
	}
}


bool RTFwr::InheritPending = false;
pprop RTFwr::InheritPendForm;


void
RTFwr::InheritForm(pprop *cform)
{
	if (InIndexGroup) {
		ProcIdxCharForm(cform);
		return;
	}

	if (!ParaPropsWritten
	 && !InCharForm
	 && !WritingHF) {
		InheritPending = true;
		InheritPendForm = *cform;
		return;
	}

	if (CharFormsNest) {  // store preceding Curr for later restore
		pprop *oform = new pprop;
		*oform = Curr;
		CharFormStack.add(oform, ++CharFormLevel);
		// more fixup here ****
	}

	if (CharStylesUsed) {
		if (CharFormSaved
		 && (!CharFormsNest
		  || (CharFormLevel == 1)))
			RestoreInheritForm();
		StartCharForm = true;
		if ((CharFormSaved == false)
		 && (!CharFormsNest
		  || (CharFormLevel == 1))) {
			SaveState();
			CharFormSaved = true;
		}
		Curr.cfnum = cform->snum;
	}

	if (Curr.stype == 1) { // not in char format
		ParaForm.maccb = Curr.maccb;    // save any added macros
		ParaForm.maccs = Curr.maccs;    // before char format wipes them
		ParaForm.macce = Curr.macce;
		ParaForm.macca = Curr.macca;
		ParaForm.maccr = Curr.maccr;
	}

	if (InheritPending) {
		UseInheritForm(&InheritPendForm);
		InheritPending = false;
	}
	else
		UseInheritForm(cform);
	AnumUsedCS = true; // just in case
}


void
RTFwr::UseInheritForm(pprop *cform)
{
	// make Curr.cstag identify the char format if any
	bool wasChar = false;
	if (Curr.cstag) {  // restore or change para format
		Last.cstag = Curr.cstag;
		Curr.cstag = NULL;
		wasChar = true;
		//Last.cform = Curr.cform;
		//Curr.cform = NULL;
		LastCF = CForm;
		CForm = NoForm;
	}
	if (cform->stype == 2) {  // use char format
		Curr.cstag = cform->stag;
		//Curr.cform = cform;
		CForm = *cform;
	}
	Curr.stype = cform->stype;  // 2 means in char span, else 1

	// set style macros
	if ((Curr.stype == 2)  // to or from char format
	 || wasChar) {
		Curr.maccb = cform->maccb;
		Curr.maccs = cform->maccs;
		Curr.macce = cform->macce;
		Curr.macca = cform->macca;
		Curr.maccr = cform->maccr;
	}

  if (cform->fnum) {
    Curr.fnum = cform->fnum;
		Curr.ding = cform->ding;
		Curr.fenc = cform->fenc;
		Curr.cpg = cform->cpg;
	}
  if (cform->fsize)
    Curr.fsize = cform->fsize;
  if (cform->bold)
    Curr.bold = cform->bold;
  if (cform->ital)
    Curr.ital = cform->ital;
  if (cform->colr)
    Curr.colr = cform->colr;
  if (cform->bcolr)
    Curr.bcolr = cform->bcolr;
  if (cform->base)
    Curr.base = cform->base;
  if (cform->caps)
    Curr.caps = cform->caps;
  if (cform->scaps)
    Curr.scaps = cform->scaps;
  if (cform->outl)
    Curr.outl = cform->outl;
  if (cform->shad)
    Curr.shad = cform->shad;
  if (cform->strike)
    Curr.strike = cform->strike;

	if (cform->kern || Curr.kern)
		Curr.kern = cform->kern;

  if ((cform->tcount)
	 || (cform == &ParaForm)) {
    Curr.tcount = cform->tcount;
    Curr.tabs = cform->tabs;
  }

	if (cform->del
	 || (cform == &ParaForm)) {
		Curr.del = cform->del;
		Curr.replace = cform->replace;
	}

	if (InHelpXref)	{
		if (NoXRPopup == true)
			Curr.line = 3; // force jump
		else if ((cform->line == 2)
		 && (cform->help != 0))
			Curr.line = 2; // allow popup as override
		if (cform->hwindow)
			Curr.hwindow = cform->hwindow;
		Curr.hvis = cform->hvis;
		return;
	}

	if (HelpForm == false) {
		if (cform->line) {
			Curr.line = cform->line;
			Curr.lmode = cform->lmode;
		}
		return;
	}

  if ((cform->line)
		 && (Curr.hyper == 0)
		 && !HyperPara) {
    Curr.line = cform->line;
    Curr.lmode = cform->lmode;
  }

	if (cform->slide)	  // aside from the slider properties, the
		Curr.slide = cform->slide;

	if (ParaForm.help   // char formats don't override para help
	 && (ParaForm.help != 17) // unless the para only stops sliders
	 && (cform != &ParaForm)) // or we are doing inherit restore
		return;

	if ((cform->help == 0)
	 && Curr.help) {
		Curr.fnote = Curr.fnote ? 1 : 0; // ending fnote
		if ((cform->line == 0)
		 && (Curr.hyper == 0)
		 && !HyperPara)
			Curr.line = Curr.lmode = 1;		 // ending underline
		Curr.hide = Curr.hide ? 1 : 0;   // ending hidden
	}
	else {
		if (cform->fnote)
			Curr.fnote = cform->fnote;
		if (cform->hide)
			Curr.hide = cform->hide;
	}
	Curr.help = cform->help;
	Curr.clone = cform->clone;
	Curr.hkey = cform->hkey;
	Curr.hvis = cform->hvis;

	Curr.href = cform->href;
	Curr.hbrowse = cform->hbrowse;
	Curr.hbuild = cform->hbuild;
	Curr.hmacro = cform->hmacro;

	if (cform->hwindow)
		Curr.hwindow = cform->hwindow;

	Curr.hjfile = cform->hjfile;

	Curr.cntform = cform->cntform;
	Curr.cntlev = cform->cntlev;

	//if (RefStr) {
		Curr.hyper = cform->hyper;
		Curr.hsuf = cform->hsuf;
	//}
}


void
RTFwr::RestoreInheritParaForm(pprop *cform)
{
	// make Curr.cstag identify the char format if any
	if (Curr.cstag) {  // restore or change para format
		Last.cstag = Curr.cstag;
		Curr.cstag = NULL;
		LastCF = CForm;
		CForm = NoForm;
	}
	Curr.stype = cform->stype;

	Curr.maccb = cform->maccb;
	Curr.maccs = cform->maccs;
	Curr.macce = cform->macce;
	Curr.macca = cform->macca;
	Curr.maccr = cform->maccr;

  Curr.fnum = cform->fnum;
	Curr.ding = cform->ding;
	Curr.fenc = cform->fenc;
	Curr.cpg = cform->cpg;
  Curr.fsize = cform->fsize;
  Curr.bold = cform->bold;
  Curr.ital = cform->ital;
  Curr.colr = cform->colr;
  Curr.bcolr = cform->bcolr;
  Curr.base = cform->base;
  Curr.caps = cform->caps;
  Curr.scaps = cform->scaps;
  Curr.outl = cform->outl;
  Curr.shad = cform->shad;
  Curr.strike = cform->strike;

	Curr.kern = cform->kern;

  Curr.tcount = cform->tcount;
  Curr.tabs = cform->tabs;

	Curr.del = cform->del;
	Curr.replace = cform->replace;

	if (InHelpXref)	{
		if (NoXRPopup == true)
			Curr.line = 3; // force jump
		else if ((cform->line == 2)
		 && (cform->help != 0))
			Curr.line = 2; // allow popup as override
		Curr.hwindow = cform->hwindow;
		Curr.hvis = cform->hvis;
		return;
	}

	if (HelpForm == false) {
		Curr.line = cform->line;
		Curr.lmode = cform->lmode;
		return;
	}

  if ((cform->line)
		 && (Curr.hyper == 0)
		 && !HyperPara) {
    Curr.line = cform->line;
    Curr.lmode = cform->lmode;
  }

	Curr.slide = cform->slide;

	if ((cform->help == 0)
	 && Curr.help) {
		Curr.fnote = Curr.fnote ? 1 : 0; // ending fnote
		if ((cform->line == 0)
		 && (Curr.hyper == 0)
		 && !HyperPara)
			Curr.line = Curr.lmode = 1;		 // ending underline
		Curr.hide = Curr.hide ? 1 : 0;   // ending hidden
	}
	else {
		Curr.fnote = cform->fnote;
		Curr.hide = cform->hide;
	}

	Curr.help = cform->help;
	Curr.clone = cform->clone;
	Curr.hkey = cform->hkey;
	Curr.hvis = cform->hvis;

	Curr.href = cform->href;
	Curr.hbrowse = cform->hbrowse;
	Curr.hbuild = cform->hbuild;
	Curr.hmacro = cform->hmacro;

	Curr.hwindow = cform->hwindow;

	Curr.hjfile = cform->hjfile;

	Curr.cntform = cform->cntform;
	Curr.cntlev = cform->cntlev;

	Curr.hyper = cform->hyper;
	Curr.hsuf = cform->hsuf;
}



// character set

uns RTFwr::CurrCharSet = 64;
uns RTFwr::DefCharSet;
char *RTFwr::CharSet;
char *RTFwr::CharSNames[CharSMax] = { "ansi", "mac", "pca", "pc" };
uns RTFwr::CharSNums[CharSMax]    = {   64,     48,    11,    2 };


void
RTFwr::SetCharSet(void)
{
  DefCharSet = CurrCharSet;
  CharSet = CharSNames[0];
  for (uns i = 0; i < CharSMax; i++) {
    if (DefCharSet == CharSNums[i]) {
      CharSet = CharSNames[i];
      break;
    }
  }
}


// language

uns RTFwr::Fr2MsLang[FrLangMax] = {
 0x400, 0x409, 0x809, 0x40C, 0x413, 0x407, 0x410, 0xC0A,
 0x414, 0x816, 0x416, 0x406, 0x40B, 0x411, 0x804, 0x404,
 0x412
};

uns RTFwr::Fr2Cpg[FrLangMax] = {
 0, 1252, 1252, 1252, 1252, 1252, 1252, 1252,
 1252, 1252, 1252, 1252, 1252, 932, 936, 950,
 949
};

//        lang   cpg   enc
// USEng  0x409  1252    0
// symbol   -      42    2
// ShJIS  0x411   932  128
// ChSimp 0x804   936  134
// ChTrad 0x404   950  136
// Korean 0x412   949  129
// Greek  0x408  1253  161
// Russ   0x419  1251  204
// Czech  0x405  1250  238
// Turk   0x41F  1254  162


char *RTFwr::DefLang = "0x409 English";
short RTFwr::DefaultLanguage = 0x409;
unl RTFwr::DefaultCodePage = 1252;
bool RTFwr::SpaceAfterUnicode = false;


void
RTFwr::ProcLang(RTFctl *cp)
{
  switch (cp->dat()) {
    case 1:  // ISO
      break;
    case 2:  // Frame
      if (cp->siz() < FrLangMax) {
        Curr.lang = Fr2MsLang[cp->siz()];
				Curr.cpg = Fr2Cpg[cp->siz()];
			}
      break;
    case 3:  // Interleaf
      break;
    case 4:  // Microsoft
      Curr.lang = (short) cp->siz();
      break;
    default:
      assert(0);
      break;
  }
}



// paragraph properties

char *RTFwr::ParAlgns[ParAlgnsMax] = { "ql", "qc", "qr", "qj" };

bool RTFwr::InTabDef;
bool RTFwr::TabsChanged;
uns RTFwr::TabCount;
uns RTFwr::TabFillChar;
unc RTFwr::TabAlign;
unc RTFwr::TabLine;
tinfo *RTFwr::CurrTab;
DCvlist *RTFwr::TabList;

bool RTFwr::InRunGroupDef = false;


void
RTFwr::SetCurrProps(RTFctl *cp)
{
  if (cp->rp == NULL)
    return;

  switch ((cp->rp)->write_op) {
    case ignore:
      break;
    case skip:
      break;

		case gpstart:
			SaveState();
			break;
		case gpend:
			RestoreState(CurrState);
			break;

    case fmpxset:   // prefix info
			InFormPx = true;
			break;
    case fmpxres:
		  InFormPx = false;
			break;
    case fmsxset:   // suffix info
			InFormSx = true;
			break;
    case fmsxres:
		  InFormSx = false;
			break;
		case frmval:
			if (cp->dat() == 4) {  // shared master ID
				if (InFormPx)
					Curr.pxframe = (uns) cp->siz();
				else if (InFormSx)
					Curr.sxframe = (uns) cp->siz();
			}
			break;

    case formcall:
			// if (!InIndexGroup)
			UseForm(cp);
      break;
    case ftaguse:
 			if (!InIndexGroup)
				UseFTag(cp);
      break;
    case ftagspc:  // ****
  		if (!InIdxGroup)
				UseSpecFmtTag(cp);
      break;

    case formsave:
			InTextPx = true;
			if (!WriteAnums)
				break;
			if (!CharStylesUsed)
				ParaForm = Curr;
      break;
		case formrest:
			InTextPx = false;
			if (!WriteAnums)
				break;
			if (!CharStylesUsed) {
				RestoreInheritForm();
				InheritPending = false;
			}
			break;
    case formtbl:
      UseTblForm(cp);
      break;

    case putfont:
			ProcFont(cp);
      break;
    case putfsize:
      Curr.fsize = GetFontSize(cp);
			if (InheritPending)
				InheritPendForm.fsize = Curr.fsize;
      break;
    case putcomp:  // compressed or expanded, modify name ****
      break;
    case putbold:
			if (InIndexGroup)
	      IdxBold = (cp->siz() > 2) ? 2 : 1;
			else {
	      Curr.bold = (cp->siz() > 2) ? 2 : 1;
				if (InheritPending)
					InheritPendForm.bold = Curr.bold;
			}
      break;
    case putital:
			if (InIndexGroup)
				IdxItal = cp->siz() ? 2 : 1;
			else {
				Curr.ital = cp->siz() ? 2 : 1;
				if (InheritPending)
					InheritPendForm.ital = Curr.ital;
			}
      break;
    case putbase:
      Curr.base = (unc) cp->siz() + 1;
			if (InheritPending)
				InheritPendForm.base = Curr.base;
      break;
    case putcaps:
      switch (cp->siz()) {
        case 0:
          Curr.caps = 1;
          Curr.scaps = 1;
					if (InheritPending) {
						InheritPendForm.caps = Curr.caps;
						InheritPendForm.scaps = Curr.scaps;
					}
          break;
        case 1:
          Curr.caps = 3;
          Curr.scaps = 1;
					if (InheritPending) {
						InheritPendForm.caps = Curr.caps;
						InheritPendForm.scaps = Curr.scaps;
					}
          break;
        case 2:
          Curr.caps = 2;
          Curr.scaps = 1;
					if (InheritPending) {
						InheritPendForm.caps = Curr.caps;
						InheritPendForm.scaps = Curr.scaps;
					}
          break;
        case 4:
          Curr.caps = 1;
          Curr.scaps = 2;
					if (InheritPending) {
						InheritPendForm.caps = Curr.caps;
						InheritPendForm.scaps = Curr.scaps;
					}
          break;
        default:
          break;
      }
      break;
    case putstyl:
      switch (cp->siz()) {
        case 0:
          Curr.outl = 1;
          Curr.shad = 1;
					if (InheritPending) {
						InheritPendForm.outl = Curr.outl;
						InheritPendForm.shad = Curr.shad;
					}
          break;
        case 1:
          Curr.outl = 1;
          Curr.shad = 2;
					if (InheritPending) {
						InheritPendForm.outl = Curr.outl;
						InheritPendForm.shad = Curr.shad;
					}
          break;
        case 2:
          Curr.outl = 2;
          Curr.shad = 1;
					if (InheritPending) {
						InheritPendForm.outl = Curr.outl;
						InheritPendForm.shad = Curr.shad;
					}
          break;
        default:
          break;
      }
      break;
    case putrevb:
			if (cp->siz() < 3) {
				Curr.strike = (cp->siz() == 2) ? 2 : 1;
				if (InheritPending)
					InheritPendForm.strike = Curr.strike;
			}
      break;
    case putline:
			if (HelpForm  // no overrides outside of formats
			 && !(InFormDef || InFormTagDef || ULineOver))
				break;
			if (InIndexGroup)
				IdxLine = (unc) cp->siz() + 1;
			else {
				Curr.line = (unc) cp->siz() + 1;
				if (InheritPending)
					InheritPendForm.line = Curr.line;
			}
      break;
    case putlmod:
      Curr.lmode = (unc) cp->siz();
			if (InheritPending)
				InheritPendForm.lmode = Curr.lmode;
      break;
    case putkern:
      SetKern(cp);
			if (InheritPending)
				InheritPendForm.kern = Curr.kern;
      break;

		case putspan:
			Curr.sideh = Curr.runin = 0;
			switch (cp->siz()) {
				case 1:
				case 2:
					Curr.sideh = 0;
					break;
				case 3:
					Curr.sideh = 1;
					break;
				case 4:
					if ((RuninType == 'R')
					 && (Curr.clone == 0))
						Curr.runin = 1;
					break;
			}
			break;
		case putside:
			if ((cp->siz() > 0)
			 && (cp->siz() < 4))
				Curr.sideh = (unc) cp->siz() + 1;
			break;
		case putrun:  // default run-in punct
			if (cp->siz())
				Curr.runpunct = NewName((char *) cp->ex(), (size_t) cp->siz());
			break;
#if 0
		case putrunin:
			Curr.rungroup = new RTFgroup(*cp);
		  if (DCLlevel < LevelMax)
				GroupStack[DCLlevel] = Curr.rungroup;
			InRunGroupDef = true;
			break;
		case eptrunin:
			InRunGroupDef = false;
			break;
#endif

    case putlspc:
      switch (Curr.lsmode = (unc) cp->dat()) {
        case 1:  // fixed
        case 2:  // fixed, allow
        case 3:  // add
        case 4:  // add, allow
          Curr.lspace = GetTwips(cp->siz(), ld_unit);
          break;
        case 5:  // prop
          Curr.lspace = (short) cp->siz();
          break;
        default:
          break;
      }
      break;
    case putpspc:
      switch (cp->dat()) {
        case 1: // fixed before
        case 2: // min before
          if ((Curr.bspace = GetTwips(cp->siz(), ld_unit)) < 0)
            Curr.bspace = 0;
          break;
        case 3: // fixed after
        case 4: // min after
          if ((Curr.aspace = GetTwips(cp->siz(), ld_unit)) < 0)
            Curr.aspace = 0;
          break;
        default:
          break;
      }
      break;
    case putalgn:
      if (cp->dat() == 5)
        Curr.algn = 3;
      else if ((cp->dat() > 0) && (cp->dat() < (ParAlgnsMax + 1)))
        Curr.algn = cp->dat() - 1;
      else Curr.algn = 0;
      break;
    case putindt:
      switch (cp->dat()) {
        case 1: // first left
          Curr.find = GetTwips(cp->siz(), pg_unit);
          break;
        case 2: // body left
          Curr.lind = GetTwips(cp->siz(), pg_unit);
          break;
        case 3: // right
          Curr.rind = GetTwips(cp->siz(), pg_unit);
          break;
        default:
          break;
      }
      break;

    case putlang:
      ProcLang(cp);
      break;
    case putcset:
      break;
    case putcolr:
			if (InFormDef)
				GetColor(cp->siz(), &Curr.colr);
			else
				ProcTextColor(cp);
      break;

    case putbreak:
		case colend:
		case pagend:
      if (cp->siz() == 1) {
        if (cp->dat() < 7) {
					if ((PageType == 'N')
					 || (InFormDef && (PageType == 'F'))) 
					  Curr.pbreak = cp->dat();
				}
        else if (cp->dat() == 8)
          Curr.pbreak = 0;
      }
      break;
    case putkeep:
      Curr.keep = (unc) cp->siz();
      break;
    case putkeepn:
      Curr.keepn = (unc) cp->siz();
      break;

    case puttdef:
      TabsChanged = true;
      InTabDef = true;
      Curr.tcount = cp->dat();
			if (HelpForm
			 && (Curr.tcount > HelpTabLimit))
				Curr.tcount = HelpTabLimit;
      TabList = new DCvlist;
      TabCount = 0;
      break;
    case endtdef:
      InTabDef = false;
			Curr.tcount = TabCount; // not all may have been added to list
      Curr.tabs = TabList;
      CurrTab->tpos = 0;
      break;
    case puttab:
			if (SuppressSpace)
				break;
			if (InTabDef
			 && HelpForm
			 && (TabCount >= HelpTabLimit))
				break;
			if (cp->dat() != 10)  // indexed tab
				CurrTab->tpos = GetTwips(cp->siz(), pg_unit);
			else
				CurrTab->tpos = cp->siz();

			if (!InTabDef
			 && !InFormPx
			 && !InFormSx
			 && (cp->dat() == 8)) {
				if (Curr.tcount > 0)
					*CurrTab = *((tinfo *) (Curr.tabs->find(1)));
				else CurrTab->ttype = 0;
				if (CurrTab->tpos == 0)
					CurrTab->tpos = Curr.lind;
				break;
			}
			if (cp->dat() >= 9)  // bar or indexed tab
        CurrTab->ttype = cp->dat();
      else if (cp->dat() > TabTypeMax)
        CurrTab->ttype = 3;
      else if (cp->dat() > 0)
        CurrTab->ttype = cp->dat() - 1;
      CurrTab->tlead = (unc) TabFillChar;
      if (InTabDef) {
        TabList->add(CurrTab, ++TabCount);
        CurrTab = new tinfo;
      }
			else TabLine = Curr.line;
      break;
    case tabfill:
      TabFillChar = (uns) cp->siz();
      break;
    case tabfstr:
      TabFillChar = *( (char *) cp->ex());
			if (TabFillChar == ' ') { // try harder
				if (strchr((char *) cp->ex(), '.'))
					TabFillChar = '.';
				else if (strchr((char *) cp->ex(), '-'))
					TabFillChar = '-';
			}
      break;

    case setftn:
      FtnMarker = (uns) cp->siz();
      break;

    case putidx:
      PutIndexEntry(cp);
      break;
    case idxgrp:
      //SaveState();
			StartingIndexGroup = InIndexGroup = true;
			IdxSeeAlso = IdxStartRange = IdxEndRange = false;
			ResetIdxCharForm();
      break;
    case idxend:
      //RestoreState(FullState);
      StartingIndexGroup = InIndexGroup = false;
      break;
		case idxtyp:
			ProcIdxType(cp);
			break;

    case setfile:
      SetXrefFileName((char *) cp->ex());
     break;

    default:
      break;
  }
}



// font processing

uns RTFwr::FontNum = 1;
DCvlist RTFwr::FontList;
DCnlist RTFwr::FontNameList;
DCilist RTFwr::FontWidthList;

char *RTFwr::FontTypes[FTypesMax] = {
  "fnil", "froman", "fswiss", "fmodern", "fscript", "fdecor", "ftech"
};
uns RTFwr::FTypes[FNamesMax + FNamesXtra] = { // Xtra is for type checking
 0, 1, 2, 3,  2, 1, 1, 2,  2, 2, 1, 1,  1, 4, 5, 6, 5,  2, 5, 5, 5
};
char *RTFwr::FontNames[FNamesMax + FNamesXtra] = {
  "unknown", "Times", "Helvetica", "Courier",
  "AvantGarde", "Bookman", "Garamond", "Helvetica-Black",
  "Helvetica-Light", "Helvetica-Narrow", "Korinna", "NewCenturySchlbk",
  "Palatino", "ZapfChancery", "ZapfDingbats", "Symbol", "Webdings",
  "Arial", "Dingbats", "Wingdings", "Bullets" // for type checking only
};
uns RTFwr::FCharWids[FNamesMax] = { // twips for 12pt en space
  144, 134, 138, 144, 144, 150, 145, 155,
  130, 115, 140, 145, 141, 121, 180, 130, 180
};
bool RTFwr::FixedWid;  // true for fonts 3, 14, 15
uns RTFwr::CharMetrics[CharMetricMax] = {  // std PostScript from ref manual
   0,  0,  0,  0,  0,  0,  0,  0,
   0,  0,  0,  0,  0,  0,  0,  0,
   0,  0,  0,  0,  0,  0,  0,  0,
   0,  0,  0,  0,  0,  0,  0,  0,
  56, 28, 33, 56, 56, 89, 67, 22,
  33, 33, 39, 58, 28, 33, 28, 28,
  56, 56, 56, 56, 56, 56, 56, 56,
  56, 56, 28, 28, 58, 58, 58, 56,
 102, 67, 67, 72, 72, 67, 61, 78,
  72, 28, 50, 67, 56, 83, 72, 78,
  67, 78, 72, 67, 61, 72, 67, 94,
  67, 67, 61, 28, 28, 28, 33, 56,
  22, 56, 56, 50, 56, 56, 28, 56,
  56, 22, 22, 50, 22, 83, 56, 56,
  56, 56, 33, 50, 28, 56, 50, 72,
  50, 50, 50, 33, 26, 33, 58, 0
};

uns RTFwr::SuperSize = 8000;  // 80% of normal font size
uns RTFwr::SubSize = 8000;
uns RTFwr::SCapSize = 8000;

long RTFwr::SuperShift = 4000; // 40% up from baseline
long RTFwr::SubShift = 2500;   // 25% down from baseline
long RTFwr::DefShift = 3;   // in halfpoints, if no font size
char *RTFwr::LastShiftDir = "up";

DClist RTFwr::FontsUsed;
DCilist RTFwr::FinalFonts;
long RTFwr::FinalFontNum = 0;

finfo RTFwr::FontData[FDataMax] = {
	{ "Times", 0, 1, 120 },
	{ "Times New Roman", 0, 1, 120 },
	{ "Helvetica", 0, 2, 130 },
	{ "Arial", 0, 2, 120 },
	{ "Courier", 0, 3, 144 },
	{ "Courier New", 0, 3, 144 },
	{ "AvantGarde", 0, 2, 144 },
	{ "Bookman", 0, 1, 150 },
	{ "Garamond", 0, 1, 145 },
	{ "Helvetica-Black", 0, 2, 155 },
	{ "Helvetica-Light", 0, 2, 130 },
	{ "Helvetica-Narrow", 0, 2, 115 },
	{ "Arial Narrow", 0, 2, 115 },
	{ "Korinna", 0, 1, 140 },
	{ "NewCenturySchlbk", 0, 1, 135 },
	{ "MS Serif", 0, 1, 144 },
	{ "Palatino", 0, 1, 141 },
	{ "ZapfChancery", 0, 4, 121 },
	{ "ZapfDingbats", 0, 5, 180 },
	{ "Dingbats", 0, 5, 180 },
	{ "Webdings", 0, 5, 180 },
	{ "Wingdings", 0, 5, 180 },
	{ "Bullets", 0, 5, 180 },
	{ "CommonBullets", 0, 5, 180 },
	{ "Symbol", 0, 6, 130 }
};

finfo RTFwr::UnicodeFontData[UnicodeFDataMax] = {
	{ "Times New Roman Cyr", 0, 1, 120, 204 },
	{ "Times New Roman CE", 0, 1, 120, 238 },
	{ "Times New Roman Greek", 0, 1, 120, 161 },
	{ "Times New Roman Tur", 0, 1, 120, 162 },
	{ "Arial Cyr", 0, 2, 120, 204 },
	{ "Arial CE", 0, 2, 120, 238 },
	{ "Arial Greek", 0, 2, 120, 161 },
	{ "Arial Tur", 0, 2, 120, 162 },
	{ "Courier New Cyr", 0, 3, 144, 204 },
	{ "Courier New CE", 0, 3, 144, 238 },
	{ "Courier New Greek", 0, 3, 144, 161 },
	{ "Courier New Tur", 0, 3, 144, 162 },
};

void
RTFwr::MarkFontUsed(uns fnum)
{
	FontsUsed.add(fnum);
}


void
RTFwr::SetFinalFonts(void)
{
	FinalFontNum = 1;

	for (long i = 1; i <= FontNum; i++) {
		if (FontsUsed.find(i))
			FinalFonts.add(FinalFontNum++, i);
	}
}


uns
RTFwr::GetFinalFont(uns fnum)
{
	if (!RemFonts)
		return fnum;
	return (uns) FinalFonts.find(fnum);
}


void
RTFwr::SetDefaultFont(void)
{
	unc i = 0;

  finfo *finf = new finfo;

	finf->fname = DefFontName;

	finf->fnum = Curr.fnum = FontNum;
  finf->ftype = 0;
	finf->fwid = Curr.fwid = DefFontWidth;

	for (i = 0; i < (FNamesMax + FNamesXtra); i++) {
    if (strstr(finf->fname, FontNames[i])) {
      finf->ftype = FTypes[i];
      break;
    }
  }

	for (i = 0; i < FDataMax; i++) {
		if (!stricmp(finf->fname, FontData[i].fname)) {
			finf->fwid = FontData[i].fwid;
			finf->ftype = FontData[i].ftype;
			break;
		}
	}

	if (IniFile
	 && IniFile->Section("FontTypes")
	 && IniFile->Find(finf->fname))
		finf->ftype = (uns) IniFile->IntRight();
  Curr.ding = ((finf->ftype == 5) || (finf->ftype == 6)) ? 1 : 0;
	finf->fenc = ((finf->ftype == 5) || (finf->ftype == 6)) ? 2 : 0;
  FontList.add(finf, FontNum);
  FontNameList.add(finf->fname, FontNum);

	if (IniFile
	 && IniFile->Section("FontEncoding")
	 && IniFile->Find(finf->fname)) {
		finf->fenc = (uns) IniFile->IntRight();
		finf->cpg = GetCodePageFromEncoding(finf->fenc);
	}

	if (IniFile
	 && IniFile->Section("FontWidths")
	 && IniFile->Find(finf->fname))
		finf->fwid = (uns) IniFile->IntRight();
	FontWidthList.add(finf->fwid, FontNum);

	Curr.fwid = finf->fwid;
}


void
RTFwr::SetUnicodeFonts(void)
{
	for (int i = 0; i < UnicodeFDataMax; i++)
		SetUnicodeFont(i);
}


void
RTFwr::SetUnicodeFont(uns ufnum)
{
  finfo *finf = new finfo;

	*finf = UnicodeFontData[ufnum];
	finf->fnum = ++FontNum;
  FontList.add(finf, FontNum);
	FontsUsed.add(FontNum);  // force retention of font

	if (IniFile
	 && IniFile->Section("DefaultUnicodeFonts")
	 && IniFile->Find(finf->fname))
		finf->fname = IniFile->StrRight();
  FontNameList.add(finf->fname, FontNum);

	if (IniFile
	 && IniFile->Section("FontEncoding")
	 && IniFile->Find(finf->fname)) {
		finf->fenc = (uns) IniFile->IntRight();
		finf->cpg = GetCodePageFromEncoding(finf->fenc);
	}

	if (IniFile
	 && IniFile->Section("FontWidths")
	 && IniFile->Find(finf->fname))
		finf->fwid = (uns) IniFile->IntRight();
	FontWidthList.add(finf->fwid, FontNum);

	Curr.fwid = finf->fwid;
}


unl 
RTFwr::GetCodePageFromEncoding(uns enc)
{
	unl cpg = 0;

	switch (enc) {
		case 128:
			cpg = 932;
			break;

		case 134:
			cpg = 936;
			break;

		case 136:
			cpg = 950;
			break;

		case 129:
			cpg = 949;
			break;

		case 161:
			cpg = 1253;
			break;

		case 204:
			cpg = 1251;
			break;

		case 238:
			cpg = 1250;
			break;

		case 162:
			cpg = 1254;
			break;

		case 0:
		default:
			cpg = 1252;
			break;
	}
	return cpg;
}


uns 
RTFwr::GetEncodingFromCodePage(unl cpg)
{
	uns enc = 0;

	switch (cpg) {
		case 932:
			enc = 128;
			break;

		case 936:
			enc = 134;
			break;

		case 950:
			enc = 136;
			break;

		case 949:
			enc = 129;
			break;

		case 1253:
			enc = 161;
			break;

		case 1251:
			enc = 204;
			break;

		case 1250:
			enc = 238;
			break;

		case 1254:
			enc = 162;
			break;

		case 1252:
		case 0:
		default:
			enc = 0;
			break;
	}
	return enc;
}


void
RTFwr::ProcFont(RTFctl *cp)
{
  char *fp;
  uns ft = 0;
	uns fw = 0;
	uns fe = 0;
	unl fpg = 0;
	uns fntnum = 0;
  finfo *finf;
	bool delfp = false;
	unc i = 0;

  if (cp->dt() == ename) {
    fp = (char *) cp->ex();
		if (*fp == '\0')
			return;
	}
  else if ((cp->siz() > 0) && (cp->siz() < (FNamesMax + 1))) {
		fntnum = (uns) cp->siz();
    fp = FontNames[fntnum - 1];
		fw = FCharWids[fntnum - 1];
		ft = FTypes[fntnum - 1];
	}
  else
    fp = (char *) FontNameList.find(1);	// unknown font number

	if (IniFile) {
		if (IniFile->Section("Fonts")
		 && IniFile->Find(fp)) {  // mapped
			fp = IniFile->StrRight();
			delfp = true;
			fntnum = 0;
		}
	}

	for (i = 0; i < FDataMax; i++) {
		if (!stricmp(fp, FontData[i].fname)) {
			fw = FontData[i].fwid;
			ft = FontData[i].ftype;
			break;
		}
	}

	if (IniFile) {
		if (IniFile->Section("FontWidths")
		 && IniFile->Find(fp))
			fw = (uns) IniFile->IntRight();
		else if (fw == 0)
			fw = DefFontWidth;  // average metric
	}

	Curr.fwid = fw;
	if (InheritPending)
		InheritPendForm.fwid = Curr.fwid;

	if (fntnum == 0) {
		for (i = 0; i < (FNamesMax + FNamesXtra); i++) {
			if (strstr(fp, FontNames[i])) {
				ft = FTypes[i];
				break;
			}
		}
	}

	if (IniFile) {
		if ((HelpForm == true)
		 && ((IniFile->Section("BitmapChars")
		   && IniFile->Find("BitmapFont"))
		  || (IniFile->Section("Options")
		   && IniFile->Find("BitmapFont")))
		 && !stricmp(fp, IniFile->StrRight()))
			ft = 31;  // reserved value for bitmap font
		else if (IniFile->Section("FontTypes")
		 && IniFile->Find(fp))
			ft = (uns) IniFile->IntRight();
		fe = ((ft == 5) || (ft == 6)) ? 2 : 0;
		if (IniFile->Section("FontEncoding")
		 && IniFile->Find(fp)) {
			fe = (uns) IniFile->IntRight();
			fpg = GetCodePageFromEncoding(fe);
		}
	}

	Curr.ding = (ft > 30) ? (unc) ft : ((ft == 5) ? 1 : 0);
	if (InheritPending)
		InheritPendForm.ding = Curr.ding;

  if ((Curr.fnum = (uns) FontNameList(fp)) == 0) {
		finf = new finfo;
		finf->fname = NewName(fp);
		finf->ftype = (ft < 7) ? ft : 5;
		finf->fenc = fe;
		finf->cpg = fpg;
		finf->fwid = fw;
		finf->fnum = ++FontNum;
		FontList.add(finf, FontNum);
		FontNameList.add(finf->fname, FontNum);
		FontWidthList.add(fw, FontNum);
		Curr.fnum = FontNum;
	}

	if (InheritPending)
		InheritPendForm.fnum = Curr.fnum;

	if (delfp)
		DeleteName(fp);
}


char *
RTFwr::GetFontName(RTFctl *cp)
{
  char *fp;

  if (cp->dt() == ename) {
    fp = (char *) cp->ex();
  }
  else {
    if ((cp->siz() > 0) && (cp->siz() < 17))
      fp = FontNames[cp->siz() - 1];
    else fp = FontNames[0];
  }
  return fp;
}


uns
RTFwr::GetFontSize(RTFctl *cp)
{
  // font units are in half-points
  return ((GetTwips(cp->siz(), ft_unit) + 5) / 10);
}


void
RTFwr::SetKern(RTFctl *cp)
{
  long factor = cp->siz();

  if ((factor == 0) || (factor == 1000))
    Curr.kern = 0;
  else if (factor > 1000)  // expand
    Curr.kern = (((Curr.fsize * 10) * (factor - 1000L)) + 500) / 1000;
  else                     // compress
    Curr.kern = -((((Curr.fsize * 10) * (1000L - factor)) + 500) / 1000);
}



// text flow processing


DCvlist RTFwr::FlowList;
DCilist RTFwr::SectList;
DClist RTFwr::SectUsedList;
DClist RTFwr::SectSkipList;
long RTFwr::SectNum = 0;
DClist RTFwr::FrameSkipList;
uns RTFwr::FlowFrameNum = 0;
RTFctl *RTFwr::FlowFrameCtl = NULL;
bool RTFwr::InDocBody;
bool RTFwr::WritingStyles = false;
bool RTFwr::InPara;
bool RTFwr::InParaGroup;
unc RTFwr::PGStarted;
bool RTFwr::ParaStarting;
bool RTFwr::ParaEnding;
bool RTFwr::FtnParaEnding;
char *RTFwr::FtnEndMacro = NULL;
long RTFwr::PreFtnParaRefNum = 0;
short RTFwr::ParaSpace = 0;
short RTFwr::BodyParaSpace = 0;
RTFgroup *RTFwr::FlowGroup;
uns RTFwr::CurrFlow;
RTFgroup *RTFwr::NormalFlowGroup = NULL;
uns RTFwr::NormalFlow = 0;
DClist RTFwr::SideHFlowList;


void
RTFwr::ProcFlow(RTFctl *cp)
{
  InDocBody = DocBodyList.find(CurrFlow = cp->dat()) ? true : false;
	if ((FlowGroup = (RTFgroup *) FlowList.find(CurrFlow)) == NULL) {
		FlowGroup = new RTFgroup;
		FlowList.add(FlowGroup, CurrFlow);
		if (InDocBody)
			SectList.add(CurrFlow, ++SectNum);
	}
	else if (FlowGroup == NormalFlowGroup)
		CurrFlow = NormalFlow;
	if (SingleSect
	 && InDocBody
	 && (LastPage > LastMasterPage))
		FlowGroup = TextFlow;
  if (DCLlevel < LevelMax)
    GroupStack[DCLlevel] = FlowGroup;
}


void
RTFwr::ProcFlowName(RTFctl *cp)
{
	if (IniFile
	 && IniFile->Section("TextFlows")
	 && IniFile->Find((char*) cp->ex())) {
		switch (IniFile->UCRight()) {
			case 'S':  // skip this flow
				SectSkipList.add(SectNum);
				if (FlowFrameNum)
					FrameSkipList.add(FlowFrameNum);
				if (SingleSect
				 && (DCLlevel < LevelMax))
					GroupStack[DCLlevel] = SkippedFlow;
				break;
			case 'N':  // treat this flow as normal
				if (LastPage <= LastMasterPage)
					break;
				if (NormalFlowGroup) { // already one defined
					FlowList.replace(NormalFlowGroup, CurrFlow);
					SectSkipList.add(SectNum);
					FlowGroup = NormalFlowGroup;
					FlowGroup->add(*FlowFrameCtl); // preceded name
					CurrFlow = NormalFlow;
				  if (DCLlevel < LevelMax)
				    GroupStack[DCLlevel] = FlowGroup;
				}
				else {    // first one encountered
					NormalFlow = CurrFlow;
					NormalFlowGroup = FlowGroup;
				}
				if (SingleSect
				 && (DCLlevel < LevelMax))
					GroupStack[DCLlevel] = TextFlow;
				break;
			default:
				break;
		}
	}
}


void
RTFwr::MarkSectUsed(RTFctl *cp)
{
	if (SingleSect)
		return;
  if (!InDocBody || SectUsedList.find(SectNum))
    return;
  if ((cp->siz() < 4)
   && !memcmp("\\r", (char *) cp->ex(), 2))
    return;
  SectUsedList.add(SectNum);
}



// RTF writing phase

bool RTFwr::RevTrack = false;
bool RTFwr::RevProt = false;

void
RTFwr::WriteDocProps(RTFwfile *wf)
{
  wf->putEnd();
  wf->putCWN("paperw", PaperWide);
  wf->putCWN("paperh", PaperHigh);
  wf->putCWN("margl", LMarg);
  wf->putCWN("margr", RMarg);
  wf->putCWN("margt", TMarg);
  wf->putCWN("margb", BMarg);
	if (Gutter)
	  wf->putCWN("gutter", Gutter);
	if (TwoSided) {
		wf->putCW("facingp");
		if (MirrorMarg)
			wf->putCW("margmirror");
	}
	wf->putCWN("uc", 0);  // unicode chars put out with no equivs

  WriteFtnProps(wf);
	if ((FirstRight && ((PgnStart & 1) == 0))
	 || ((!FirstRight) && ((PgnStart & 1) == 1)))
		PgnStart++;
  wf->putCWN("pgnstart", PgnStart);
	wf->putCW("widowctrl");
	wf->putCWN("deftab", DefTabWidth);
  if (UseLang)
    wf->putCWN("deflang", DefForm.lang);

	if (PSpcType == 'F') {
		// wf->putCW("sprstsp"); // bad idea, clips top of headings
		wf->putCW("sprsspbf");   // seems not to work, at least with pagebb
	}

	if (RevTrack)
		wf->putCW("revisions");
	if (RevProt)
		wf->putCW("revprot");


	if (!HelpForm)             // no auto tab for hanging indent
		wf->putCW("notabind");

	if (Template) {
		bool quote = false;
		wf->putEnd();
		if (TemplateAutoUpdate)
			wf->putCW("linkstyles");
		wf->putGS();
		wf->putCS('*');
		wf->putCW("template");
		char *txt = Template;
		if ((*txt != '"')
		 && strchr(txt, ' ')) {
			quote = true;
			wf->putChar('"');
		}
		while (*txt) {
			if (*txt == '\\')
				wf->putCS('\\');
			else
				wf->putChar(*txt);
			txt++;
		}
		if (quote == true)
			wf->putChar('"');
		wf->putGE();
	}

}


void
RTFwr::WriteFonts(RTFwfile *wf)
{
  finfo *finf;
	short ftnum;

	if (RepStyles || RemStyles || RemFonts)
		SetFinalForms();  // marks fonts used too
	if (RemFonts)
		SetFinalFonts();

  wf->putEnd();
  wf->putGS();
  wf->putCW("fonttbl");
  for (uns i = 1; i <= FontNum; i++) {
    finf = (finfo *) FontList.find(i);
    assert(finf);
		if ((ftnum = GetFinalFont(finf->fnum)) == 0)
			continue;
    wf->putEnd();
    wf->putGS();
    wf->putCWN("f", ftnum);
    wf->putCW(FontTypes[finf->ftype]);
		wf->putCWN("fcharset", finf->fenc);
    wf->putStr(finf->fname);
    wf->putChar(';');
    wf->putGE();
  }
  wf->putGE();
}



void
RTFwr::WriteStyles(RTFwfile *wf)
{
  pprop *style;
	short stnum;

  wf->putEnd();
  wf->putGS();
  wf->putCW("stylesheet");
	WritingStyles = true;

  for (uns i = 1; i <= FormNum; i++) {
    style = (pprop *) FormList.find(i);
    assert(style);
    if ((style->stype == 2) && !CharStyles)
      continue;
		if (RepStyles
		 && FormsReplaced.find(i))
			continue;
		if ((i > 1)
		 && ((stnum = (short) GetFinalForm(style->snum)) == 0))
			continue;
    wf->putEnd();
    wf->putGS();
    if (i > 1) { // style 1 is "Normal" (put out as zero with no \sN)
			if (style->stype == 2) {
				wf->putCS('*');
	      wf->putCWN("cs", stnum);
			}
			else
	      wf->putCWN("s", stnum);
		}

		if (!XML) { //XML
			if (style->stype != 2)
				WriteParaStyles(wf, style);
			if (CharStyles)
				WriteCharStyles(wf, style);
			if (style->stype == 2)
				wf->putCW("additive");
			else if (style->tcount)
				WriteTabDefs(wf, style);
			if (style->stype != 2) {
				WriteBorders(wf, style);
				WriteShading(wf, style->shading);
			}
		}
		else {
			if (style->stype == 2)
				wf->putCW("additive");
		}

		if (style->styadd)
			DCmacr::WriteMacro(wf, style->styadd);

		if (style->stype != 2)
	    wf->putCWN("snext", GetFinalForm(style->snext));
    wf->putStr(TmpStyleName(style->stag));
    wf->putChar(';');
    wf->putGE();
  }
  wf->putGE();
	WritingStyles = false;
}


char*
RTFwr::TmpStyleName(char *name)
{
	static char nbuf[StringMax + 1];
	char *np = nbuf;
	char ch;

	// replace forbidden chars with spaces
	while (*name) {
		ch = *name++;
		if ((ch == ';')
		 || (ch == ',')
		 || (ch == '\\')
		 || (ch == '{')
		 || (ch == '}'))
			ch = ' ';
		*np++ = ch;
		if ((np - nbuf) >= StringMax)
			break;
	}
	*np = '\0';
	return nbuf;
}

bool RTFwr::BordersShadingDone = false;

void
RTFwr::WriteBorders(RTFwfile *wf, pprop *style)
{
	if (style->brdrt)
		WriteOneBorder(wf, style->brdrt, "brdrt");
	if (style->brdrb)
		WriteOneBorder(wf, style->brdrb, "brdrb");

	if (style->brdrl)
		WriteOneBorder(wf, style->brdrl, "brdrl");
	if (style->brdrr)
		WriteOneBorder(wf, style->brdrr, "brdrr");

	if (style->brdrbar)
		WriteOneBorder(wf, style->brdrbar, "brdrbar");
	if (style->brdrbox)
		WriteOneBorder(wf, style->brdrbox, "box");
	if (style->brdrbtw)
		WriteOneBorder(wf, style->brdrbox, "brdrbtw");
}


void
RTFwr::WriteOneBorder(RTFwfile *wf, brdrinfo *br, char *cw, bool hlp)
{
	if (br->type == brdrhidden)
		return;

	if (cw)
		wf->putCW(cw);

	switch (br->type) {
		case brdrth:
			wf->putCW("brdrth");
			break;

		case brdrsh:
			wf->putCW("brdrsh");
			break;

		case brdrdb:
			wf->putCW("brdrdb");
			break;

		case brdrdot:
			wf->putCW("brdrdot");
			break;

		case brdrdash:
			wf->putCW("brdrdash");
			break;

		case brdrhair:
			wf->putCW("brdrhair");
			break;

		case brdrnone:
		case brdrs:
		default:
			wf->putCW("brdrs");
			break;
	}

	if (!hlp) {
		if (br->thick)
			wf->putCWN("brdrw", (short) br->thick);

		if (br->space)
			wf->putCWN("brsp", (short) br->space);

		if (br->color > 2)
			wf->putCWN("brdrcf", (short) br->color - 1);
	}
}


void
RTFwr::WriteShading(RTFwfile *wf, shdinfo *sh)
{
	if (!sh)
		return;

	if ((sh->type == shdnone)
	 && (sh->tint == 0))
		return;

	switch (sh->type) {
		case shdhoriz:
			wf->putCW("bghoriz");
			break;

		case shdvert:
			wf->putCW("bgvert");
			break;

		case shdfdiag:
			wf->putCW("bgfdiag");
			break;

		case shdbdiag:
			wf->putCW("bgbdiag");
			break;

		case shd2cross:
			wf->putCW("bgcross");
			break;

		case shddcross:
			wf->putCW("bgdcross");
			break;

		case shddkhoriz:
			wf->putCW("bgdkhoriz");
			break;

		case shddkvert:
			wf->putCW("bgdkvert");
			break;

		case shddkfdiag:
			wf->putCW("bgdkfdiag");
			break;

		case shddkbdiag:
			wf->putCW("bgdkbdiag");
			break;

		case shddkcross:
			wf->putCW("bgdkcross");
			break;

		case shddkdcross:
			wf->putCW("bgdkdcross");
			break;

		case shdnone:
		default:
			wf->putCWN("shading", (short) sh->tint);
			break;
	}

	if (sh->color > 2)
		wf->putCWN("cfpat", (short) sh->color - 1);
	if ((sh->bcolor > 1)
	 && (sh->bcolor != 9))
		wf->putCWN("cbpat", (short) sh->bcolor - 1);
}


void
RTFwr::WriteCharStyles(RTFwfile *wf, pprop *style)
{
  if (style->fnum)
    wf->putCWN("f", GetFinalFont(style->fnum));
  if (style->fsize)
    wf->putCWN("fs", style->fsize);
  if (style->bold > 1)
    wf->putCW("b");
  if (style->ital > 1)
    wf->putCW("i");
  if (style->base > 1)
    wf->putCWN((style->base > 2 ? "up" : "dn"),
                  (style->fsize ? style->fsize / 3 : 3));
  if (style->caps == 2)
    wf->putCW("caps");
	if ((SmallCaps == 'S')
   && (style->scaps > 1))
    wf->putCW("scaps");
  if (style->outl > 1)
    wf->putCW("outl");
  if (style->shad > 1)
    wf->putCW("shad");
  if (style->strike > 1)
    wf->putCW("strike");
  if (style->line > 1)
    wf->putCW(style->line == 4 ? "uld" :   // dotted for overline
             (style->line == 3 ? "uldb" :
             (style->lmode ? "ul" : "ulw" )));  // for 2 and 5
  if (style->kern != 0)
    wf->putCWN("expndtw", style->kern, true);
	if ((TextColor == true)
	 && ((style->colr > 2)
	  || ((style->colr == 2)
	   && (style->stype == 2))))
		wf->putCWN("cf", style->colr - 1);
	if ((BackColor == true)
	 && (((style->bcolr > 1)
	   && (style->bcolr != 9))
	  || ((style->bcolr == 9)
	   && (style->stype == 2)))) {
		uns bc = style->bcolr - 1;
		wf->putCW("chshdng0");
		wf->putCWN("chcbpat", bc);
		wf->putCWN("cb", bc);
	}
}


void
RTFwr::WriteParaStyles(RTFwfile *wf, pprop *style)
{
  short val;
	bool minus = false;

  if (style->keep)
    wf->putCW("keep");
  if (style->keepn)
    wf->putCW("keepn");
	if (style->pbreak) {
		switch (style->pbreak) {
			case 4:
			case 5:
			case 6:
				wf->putCW("pagebb");
				break;
			default:
				break;
		}
	}
  if (style->algn > 0)
    wf->putCW(ParAlgns[style->algn]);
  if (style->stype != 2) {
    switch (style->lsmode) {
      case 1:  // fixed
				minus = true;
				// fall through
      case 2:  // fixed, allow
      default:
        val = style->lspace;
        break;
      case 3:  // add
				minus = true;
				// fall through
      case 4:  // add, allow
        val = style->lspace + (style->fsize * 10);
        break;
      case 5:  // prop
        val = (style->lspace * style->fsize) / 100;
        break;
    }
		if (val < 0)
			val = 0;
    wf->putCWN("sl", minus ? -val : val, true);
  }
  if (style->bspace)
    wf->putCWN("sb", style->bspace);
  if (style->aspace)
    wf->putCWN("sa", style->aspace);
  if (style->lind)
    wf->putCWN("li", style->lind);
  if (style->find != style->lind)
    wf->putCWN("fi", style->find - style->lind, true);
  if (style->rind)
    wf->putCWN("ri", style->rind);
}



// section processing

DCvlist RTFwr::SideHTFlowList;
DCvlist RTFwr::SideHFrameList;
DCilist RTFwr::SideHSpaceList;
short RTFwr::SideHWide = 0;
short RTFwr::SideHGap = 0;
short RTFwr::SHWidth = 0;
short RTFwr::SHGap = 0;
unc RTFwr::SideHPos = 0;
bool RTFwr::SideHFlip = false;
bool RTFwr::SideHUsed = false;
bool RTFwr::ForceSideHeadPos = false;
unc RTFwr::FSideHPos = 0;
bool RTFwr::FSideHFlip = false;
bool RTFwr::SideHAnchor = false;
bool RTFwr::SHSpannerAnchors = true;
bool RTFwr::SideHPropsInFlow = false;

short RTFwr::SideHRind = 0;
short RTFwr::SideHLind = 0;
short RTFwr::TextRind = 0;
short RTFwr::TextLind = 0;
short RTFwr::TextLMarg = 0;
short RTFwr::TextRMarg = 0;
bool RTFwr::FirstTop = false;
short RTFwr::FirstHBottom = 0;

bool RTFwr::SingleSect = false;
RTFgroup *RTFwr::TextFlow = NULL;
RTFgroup *RTFwr::SkippedFlow = NULL;
bool RTFwr::SectStarting = false;
bool RTFwr::KeepSectBreaks = true;

short RTFwr::HeaderYPos = 360;
short RTFwr::FooterYPos = 360;
char *RTFwr::SectBreakType = NULL;
char *RTFwr::SectBreakTypes[9] = {
 NULL, NULL, NULL, "sbkcol",
 "sbkpage", "sbkeven", "sbkodd", NULL,
 "sbknone" 
};


void
RTFwr::WriteFlow(RTFwfile *wf, uns flownum)
{
	frame *fr = NULL;
	page *mpp = NULL;
  RTFgroup *gp = TextFlow;

	CurrFlow = flownum;
	if (flownum)
		gp = (RTFgroup *) FlowList.find(flownum);
  assert(gp);
	if (gp == NULL)
		return;

	NextFirstMaster = 0;
  StartingPage = true;
  StartingPageBody = true;
	ParaSpace = BodyParaSpace = 0;
  InPara = false;
  wf->putEnd();

	if (FlowWritten
	 && !HelpForm) {  // write section break
		wf->putEnd();
		if (SectBreakType) {
			wf->putCW(SectBreakType);
			SectBreakType = NULL;
		}
		wf->putCW("sect");
		wf->putEnd();
		SectStarting = true;
	}

	if (HelpForm
	 && FlowWritten) {  // change of section in WinHelp
		InPara = false;
	}
	else {  // not WinHelp, or else first section
		wf->putCW("sectd");
		FirstHBottom = TMarg;
		if (KeepSectBreaks)
			PutSectPageProps(wf, FirstMaster);
		else {
			wf->putCW(PgnForms[PgnForm]);
			CurrCols = DefCols ? DefCols : 1;
			wf->putCWN("cols", CurrCols);
			if (DefCols > 1) {
				wf->putCWN("colsx", ColGap ? ColGap : 360);
				if (ColLine)
					wf->putCW("linebetcol");
				wf->putCW("sbkcol");
			}
		}
		if (NoLineNumbers)
			wf->putCW("noline");
		wf->putEnd();

		if ((HFType != 'N')
		 && !HFDone) {       // headers and footers
			if (CurrSection
			 && ((mpp = (page *) GetSectFormatFileInfo(CurrSection)) != NULL))
				WriteAllHFFmts(wf, mpp);
			else {
				mpp = (page *) PageList.find(RightMaster);
				if (mpp != NULL) {
					if ((fr = (frame *) HeaderList.find(RightMaster)) != NULL)
						wf->putCWN("headery", fr->ypos);
					if ((fr = (frame *) FooterList.find(RightMaster)) != NULL)
						wf->putCWN("footery", mpp->phigh - fr->ypos);
				}
				else {
					DifferentFirst = TitlePage;
					wf->putCWN("headery", HeaderYPos);
					wf->putCWN("footery", FooterYPos);
				}
				if (DifferentFirst) {
					wf->putCW("titlepg");
					if (FirstMaster == 0)
						FirstMaster = FirstRight ? RightMaster : LeftMaster;
					if (FirstHeaderInsert)
						DCmacr::WriteMacro(wf, FirstHeaderInsert);
					else
						WriteHF(wf, "headerf", FirstMaster, true, FirstRight);
					if (HFBottom > TMarg)
						FirstHBottom = HFBottom;
					if (FirstFooterInsert)
						DCmacr::WriteMacro(wf, FirstFooterInsert);
					else
						WriteHF(wf, "footerf", FirstMaster, false, FirstRight);
				}
				if (!TwoSided) {	 // use Right for single side
					if (HeaderInsert)
						DCmacr::WriteMacro(wf, HeaderInsert);
					else
						WriteHF(wf, "header", RightMaster, true);
					if (FooterInsert)
						DCmacr::WriteMacro(wf, FooterInsert);
					else
						WriteHF(wf, "footer", RightMaster, false);
				}
				else {
					if (LeftHeaderInsert)
						DCmacr::WriteMacro(wf, LeftHeaderInsert);
					else
						WriteHF(wf, "headerl", LeftMaster, true, false);
					if (LeftFooterInsert)
						DCmacr::WriteMacro(wf, LeftFooterInsert);
					else
						WriteHF(wf, "footerl", LeftMaster, false, false);
					if (RightHeaderInsert)
						DCmacr::WriteMacro(wf, RightHeaderInsert);
					else
						WriteHF(wf, "headerr", RightMaster, true);
					if (RightFooterInsert)
						DCmacr::WriteMacro(wf, RightFooterInsert);
					else
						WriteHF(wf, "footerr", RightMaster, false);
				}
			}
		}
		HFDone = true;
		wf->putEnd();
		PGStarted = 0;
		if (MissingFormNum)
			WriteMissingFormParas(wf);
		if (SeqAnums && FMSource)
			WriteChapVolFields(wf);

		if (DifferentFirst
		 && !HelpForm)
			FirstTop = true;
		else
			PageBroke = true;
	}

  Last = DefForm;
	if (HelpForm == true) {
		wf->putCW("page");
		if (HelpSectBreaks)
			wf->putCW("sect");
	  wf->putEnd();
		StartingHelpTopic = true;
	}

	if (TopInsert)
		DCmacr::WriteMacro(wf, TopInsert);

  gp->write(*wf);  // write entire flow here
	if (RuninDiv) {  // last para was runin, very bad
		Curr.runin = false;
		WriteProps(wf);
		WriteParaEndCode(wf);
	}

	if (CondEndPend)
		WriteCondProps(wf);

	if (HelpForm == true)
		WriteTopicEnding(wf);

	if (BottomInsert)
		DCmacr::WriteMacro(wf, BottomInsert);

  if (PGStarted && !InTbCell) {
    wf->putGE();
    PGStarted = 0;
  }

  InParaGroup = false;
}


DCilist RTFwr::SectBreakList;
short RTFwr::CurrCols = 1;


void
RTFwr::WriteSectBreak(RTFwfile *wf, uns first)
{
	if (!KeepSectBreaks)
		return;

  if (PGStarted && !InTbCell) {
    wf->putGE();
    PGStarted = 0;
  }
  InParaGroup = false;

	WriteTopicEnding(wf);
	PageBreakPending = false;

	//wf->putCW("page");
  wf->putEnd();

	if (HelpForm) {
		wf->putCW("page");
		if (HelpSectBreaks)
			wf->putCW("sect");
	  wf->putEnd();
		InPara = false;
		StartingHelpTopic = true;
		return;
	}

	if (!SectStarting) {
		if (FlowWritten) {
			if (SectBreakType) {
				wf->putCW(SectBreakType);
				SectBreakType = NULL;
			}
			wf->putCW("sect");
		}
		StartingPage = true;
	  StartingPageBody = true;
		ParaSpace = BodyParaSpace = 0;
	}

	if (StartingSection
	 || first) {          // put out page props
		wf->putCW("sectd");
		PutSectPageProps(wf, first);
	}

	FirstHBottom = TMarg;

	page *mpp = NULL;
	if (CurrSection
	 && ((mpp = (page *) GetSectFormatFileInfo(CurrSection)) != NULL))
		WriteAllHFFmts(wf, mpp);
	else {
		if (HFType != 'N') {   // may want to put out a FirstMaster
			if ((first != 0)
			 && (first != FirstMaster)) {
				wf->putCW("titlepg");
				FirstMaster = first;
				if (FirstHeaderInsert)
					DCmacr::WriteMacro(wf, FirstHeaderInsert);
				else
					WriteHF(wf, "headerf", FirstMaster, true, FirstRight);
				if (HFBottom > TMarg)
					FirstHBottom = HFBottom;
				if (FirstFooterInsert)
					DCmacr::WriteMacro(wf, FirstFooterInsert);
				else
					WriteHF(wf, "footerf", FirstMaster, false, FirstRight);
				if (MissingFormNum)
					WriteMissingFormParas(wf);
			}
			else if (DifferentFirst)
				wf->putCW("titlepg");
		}
	}

	if (DifferentFirst)
		FirstTop = true;
	else
		PageBroke = true;
}


void
RTFwr::PutSectPageProps(RTFwfile *wf, uns first)
{
	shprop *shp = NULL;
	SideHAnchor = false;

	page *fp = NULL;

	if (StartingSection)
		fp = GetSectFormatFileInfo(CurrSection);
	if (!fp && first)
		fp = (page *) PageList.find(first);

	if (CurrFlow) {
		ResetSideHProps();
		SideHUsed = (bool) SideHFlowList.find(CurrFlow);
	}

	if (!fp) {
		if (CurrFlow
		 && SideHUsed)
			SetSideHProps(wf, (shprop *) SideHTFlowList.find(CurrFlow), NULL, true);
		return;
	}

	if (fp->porient > 0)
		wf->putCW("lndscpsxn");

	if (PaperWide != fp->pwide)
		wf->putCWN("pgwsxn", PaperWide = fp->pwide);
	if (PaperHigh != fp->phigh)
		wf->putCWN("pghsxn", PaperHigh = fp->phigh);

	if (fp->ptmrg != TMarg)
		wf->putCWN("margtsxn", TMarg = fp->ptmrg);
	if (fp->pbmrg != BMarg)
		wf->putCWN("margbsxn", BMarg = fp->pbmrg);

	if (CurrFlow != 0) {
		if (SideHUsed)
			SetSideHProps(wf, (shprop *) SideHTFlowList.find(CurrFlow), fp, true);
		else {
			if (fp->plmrg != LMarg)
				wf->putCWN("marglsxn", LMarg = fp->plmrg);
			if (fp->prmrg != RMarg)
				wf->putCWN("margrsxn", RMarg = fp->prmrg);
			if (fp->gutter)
				wf->putCWN("guttersxn", Gutter = fp->gutter);
		}

		Margins = LMarg + RMarg + Gutter;
		BodyWide = PaperWide - Margins;
		if (!TblFullWidthWasSet)
			TblFullWidth = BodyWide;
	}
	if (fp->brdrt)
		WriteOneBorder(wf, fp->brdrt, "pgbrdrt");
	if (fp->brdrb)
		WriteOneBorder(wf, fp->brdrb, "pgbrdrb");

	if (fp->brdrl)
		WriteOneBorder(wf, fp->brdrl, "pgbrdrl");
	if (fp->brdrr)
		WriteOneBorder(wf, fp->brdrr, "pgbrdrr");
	if (fp->brwraphd)
		wf->putCW("brwraphd");
	if (fp->brwrapft)
		wf->putCW("brwrapft");

	if (StartingSection) {
		wf->putCW(PgnForms[fp->pgnform]);
		if (fp->pgnstart) {
			wf->putCW("pgnrestart");
			wf->putCWN("pgnstart", fp->pgnstart);
		}
		CurrCols = fp->pcols ? fp->pcols : 1;
		wf->putCWN("cols", CurrCols);
		if (fp->pcols > 1) {
			wf->putCWN("colsx", fp->pcgap ? fp->pcgap : 360);
			if (fp->linebetcol)
				wf->putCW("linebetcol");
			wf->putCW("sbkcol");
		}
		StartingSection = false;
	}
}


// write index field

char *RTFwr::IndexStyle = "letter";
char *RTFwr::IndexLeader = "dot";

DCnlist RTFwr::IndexStyles;
npair RTFwr::IndexStylesInit[] = {
 { 1, "none"},
 { 2, "space"},
 { 3, "letter"},
 NULL
};

DCnlist RTFwr::IndexLeaders;
npair RTFwr::IndexLeadersInit[] = {
 { 1, "none"},
 { 2, "space"},
 { 3, "dot"},
 { 4, "line"},
 NULL
};

uns RTFwr::IndexColumns = 2;
char RTFwr::CurrIdxLetter = 0;
bool RTFwr::UseIndexLetters = true;
bool RTFwr::UseIndexTopLetters = true;
bool RTFwr::UseListProps = false;
bool RTFwr::UseIXLets = true;
bool RTFwr::UseIXTopLets = true;

char *RTFwr::IndexLetterSymbol = "Sym";
char *RTFwr::IndexLetterNumber = "Num";
char *RTFwr::IndexLetPrefix = "ixlet";
char *RTFwr::IndexTopLettersForm = "IndexTopLetters";
char *RTFwr::IndexLettersForm = "IndexLetters";


void 
RTFwr::InsertIndexTOCItem(RTFwfile *wf, RTFctl *cp)
{
	if (cp->siz() == 0) {
		return;
	}
	else if (cp->siz() == 1) {
		// written by drxml when idx in map,
		// idx toc entry and head already written

		PutIndexField(wf, cp);
		return;
	}
	else if (cp->siz() == 2) {  // starting drxml index entries
		// write index top letters if used
		CurrIdxLetter = 0;
		UseListProps = true;
		SetIndexTopLetters(wf);
		return;
	}
	else if (cp->siz() == 3) {  // ending drxml index entries
		if (UseIXLets) {
			wf->putEnd();
			wf->putGS();
			wf->putCW("pard");
			wf->putCW("plain");
			wf->putEnd();
			while (++CurrIdxLetter <= 'Z')
				PutIXBookmark(wf, CurrIdxLetter);
			wf->putCW("par");
			wf->putGE();
			wf->putEnd();
		}
		UseListProps = false;
		return;
	}
	else if (cp->siz() == 4) {  // starting drxml index entry
		return;
	}
	else if (cp->siz() == 5) {  // ending drxml index entry after refs and subs
		return;
	}
	else if (cp->siz() == 13) {  // leader for compact
		wf->putCW("tab");
		return;
	}
	else if (cp->siz() > 0x20) {  // changing drxml index letter
		SetIndexLetter(wf, (char) (cp->siz() & 0xFF));
		return;
	}
}


void
RTFwr::SetIXListProps(RTFctl *cp)
{
	if (cp->dat() == 2)
		UseIXLets = cp->siz() ? true : false;
	else if (cp->dat() == 3)
		UseIXTopLets = cp->siz() ? true : false;
}


void 
RTFwr::PutIndexField(RTFwfile *wf, RTFctl *cp)
{
// {\f0\fs22\uld  INDEX \\e "\tab " \\h "A" \\c "2" \\z "1033" }}
// switches:
// \c "N"   columns (2-4), omit switch for 1
// \h "A"   letter headings, Index Heading format, "" for space
// \z "1033"  language, use DefForm.lang
// \e "\tab " set for right, omit switch for none
//            determine leader with \ul, \uld, or none at start
	if (!IndexStyles.next)
		IndexStyles.init(IndexStylesInit);
	if (!IndexLeaders.next)
		IndexLeaders.init(IndexLeadersInit);
	long style = IndexStyles(IndexStyle);
	long leader = IndexLeaders(IndexLeader);

	wf->putEnd();
	wf->putGS();
	wf->putCW("pard");
	wf->putCW("plain");
	wf->putGS();
	wf->putCW("field");
	wf->putGS();
	wf->putCS('*');
	wf->putCW("fldinst");
	wf->putGS();
	if (leader == 3)
		wf->putCW("uld");
	else if (leader == 4)
		wf->putCW("ul");

	wf->putStr("INDEX ");
	if (leader > 1)
		wf->putStr("\\\\e \"\\tab \" ");
	if (style > 1) {
		wf->putStr("\\\\h ");
		wf->putStr((style == 2) ? "\"\" " : "\"A\" ");
	}
	if (IndexColumns > 1) {
		wf->putStr("\\\\c \"");
		wf->putShort((IndexColumns > 4) ? 4 : IndexColumns);
		wf->putStr("\" ");
	}
	if (DefForm.lang) {
		wf->putStr("\\\\z \"");
		wf->putShort(DefForm.lang);
		wf->putStr("\" ");
	}
	wf->putGE();
	wf->putGE();
	wf->putEnd();
	wf->putGS();
	wf->putCW("fldrslt");
	wf->putGE();
	wf->putGE();
	wf->putCW("par");
	wf->putGE();
	wf->putEnd();
}


void 
RTFwr::SetIndexTopLetters(RTFwfile *wf)
{
	if (UseListProps ? UseIXLets : UseIndexLetters) {
		if (UseListProps ? UseIXTopLets : UseIndexTopLetters) {
			wf->putEnd();
			wf->putGS();
			wf->putCW("pard");
			wf->putCW("plain");
			PutIndexForm(wf, IndexTopLettersForm);
			wf->putEnd();

			// {\field {\*\fldinst {HYPERLINK \\l RdtsRIDX}}{\fldrslt {Index}}}
			if (IndexLetterSymbol) {
				wf->putGS();
				wf->putCW("field");
				wf->putGS();
				wf->putCS('*');
				wf->putCW("fldinst");
				wf->putGS();
				wf->putStr("HYPERLINK \\\\l ");
				wf->putStr(IndexLetPrefix);
				wf->putGE();
				wf->putGE();
				wf->putGS();
				wf->putCW("fldrslt");
				wf->putGS();
				wf->putStr(IndexLetterSymbol);
				wf->putGE();
				wf->putGE();
				wf->putGE();
				wf->putCS('~');
				wf->putEnd();
			}
			if (IndexLetterNumber) {
				wf->putGS();
				wf->putCW("field");
				wf->putGS();
				wf->putCS('*');
				wf->putCW("fldinst");
				wf->putGS();
				wf->putStr("HYPERLINK \\\\l ");
				wf->putStr(IndexLetPrefix);
				wf->putChar('0');
				wf->putGE();
				wf->putGE();
				wf->putGS();
				wf->putCW("fldrslt");
				wf->putGS();
				wf->putStr(IndexLetterNumber);
				wf->putGE();
				wf->putGE();
				wf->putGE();
				wf->putCS('~');
				wf->putEnd();
			}
			for (int anum = 0; anum < 26; anum++) {
				wf->putGS();
				wf->putCW("field");
				wf->putGS();
				wf->putCS('*');
				wf->putCW("fldinst");
				wf->putGS();
				wf->putStr("HYPERLINK \\\\l ");
				wf->putStr(IndexLetPrefix);
				wf->putChar('A' + anum);
				wf->putGE();
				wf->putGE();
				wf->putGS();
				wf->putCW("fldrslt");
				wf->putGS();
				wf->putChar('A' + anum);
				wf->putGE();
				wf->putGE();
				wf->putGE();
				wf->putCS('~');
				wf->putEnd();
			}
			wf->putCW("par");
			wf->putGE();
			wf->putEnd();
		}
	}
}


void 
RTFwr::SetIndexLetter(RTFwfile *wf, char ch)
{
	if (!isupper(ch)) {
		if (islower(ch))
			ch = _toupper(ch);
		else if (isdigit(ch))
			ch = '0';
		else ch = '!';
	}
	if (ch > CurrIdxLetter) {
		wf->putEnd();
		wf->putGS();
		wf->putCW("pard");
		wf->putCW("plain");
		PutIndexForm(wf, IndexLettersForm);
		wf->putEnd();

		char nch = CurrIdxLetter;
		if ((nch == 0)
		 && (ch > '!'))
			PutIXBookmark(wf, nch = '!');
		if ((nch == '!')
		 && (ch > '0'))
			PutIXBookmark(wf, nch = '0');
		if ((nch == '0')
		 && (ch >= 'A'))
			nch = 'A' - 1;
		while (nch < ch)
			PutIXBookmark(wf, ++nch);
		CurrIdxLetter = ch;

		if ((ch == '!')
		 && IndexLetterSymbol)
			wf->putStr(IndexLetterSymbol);
		else if ((ch == '0')
		 && IndexLetterNumber)
			wf->putStr(IndexLetterNumber);
		else
			wf->putChar(ch);

		wf->putCW("par");
		wf->putGE();
		wf->putEnd();
	}
}


void 
RTFwr::PutIXBookmark(RTFwfile *wf, char ch)
{
	// {\*\bkmkstart RdtsRIDX}{\*\bkmkend RdtsRIDX}Index

	wf->putGS();
	wf->putCS('*');
	wf->putCW("bkmkstart");
	wf->putStr(IndexLetPrefix);
	if (ch != '!')
		wf->putChar(ch);
	wf->putGE();

	wf->putGS();
	wf->putCS('*');
	wf->putCW("bkmkend");
	wf->putStr(IndexLetPrefix);
	if (ch != '!')
		wf->putChar(ch);
	wf->putGE();
	wf->putEnd();
}


void 
RTFwr::PutIndexForm(RTFwfile *wf, char *fname)
{
	long formnum = FormNameList(fname);
	pprop	*cform = (pprop *) FormList.find(formnum);
	if (cform != NULL) {
		InitForm(cform);
		wf->putCWN("s", GetFinalForm(Curr.snum));
		WriteParaProps(wf);
		WriteSimpleCharProps(wf);
		WriteOutTabs(wf);
	  Last = Curr;
	}
	else
		wf->putCWN("s", 0);
}


// write empty hidden paras for missing formats referenced in H/F

bool RTFwr::WriteMissingForms = true;
bool RTFwr::WriteAllVarForms = false;
DCilist RTFwr::MissingFormList;
long RTFwr::MissingFormNum = 0;
DClist RTFwr::FormsFound;

void
RTFwr::AddMissingForm(long snum)
{
	if ((snum = (long) GetFinalForm((uns) snum)) == 0)
		return;  // no use, gone entirely

	if (!MissingFormList.find(snum))
		MissingFormList.add(snum, ++MissingFormNum);
}


void
RTFwr::AddFormsFound(RTFctl *cp)
{
	FormsFound.add(FormRefList(cp->siz()));
}


void
RTFwr::WriteMissingFormParas(RTFwfile *wf)
{
	MissingFormList.writeall(wf, WriteHiddenPara);
	MissingFormList.empty();
	MissingFormNum = 0;
}


void
RTFwr::WriteHiddenPara(DCwfile *wf, long fnum, long item)
{
	if (!item)
		return;

	((RTFwfile *) wf)->putGS();
	((RTFwfile *) wf)->putCW("v");
	((RTFwfile *) wf)->putCWN("s", (short) fnum - 1);
	((RTFwfile *) wf)->putCW("par");
	((RTFwfile *) wf)->putGE();
	((RTFwfile *) wf)->putEnd();
}


void
RTFwr::WriteAllHFFmts(RTFwfile *wf, page *mpp)
{
	if (!mpp)
		return;

	DifferentFirst = mpp->titlepg;
	wf->putCWN("headery", (short) mpp->headery);
	wf->putCWN("footery", (short) mpp->footery);
	if (DifferentFirst) {
		wf->putCW("titlepg");
		if (mpp->hdf && mpp->hdf->cont)
			WriteFmtHF(wf, "headerf", mpp->hdf, true);
		if (mpp->ftf && mpp->ftf->cont)
			WriteFmtHF(wf, "footerf", mpp->ftf, false);
	}
	if (!TwoSided) {
		if (mpp->hdb && mpp->hdb->cont)
			WriteFmtHF(wf, "header", mpp->hdb, true);
		if (mpp->ftb && mpp->ftb->cont)
			WriteFmtHF(wf, "footer", mpp->ftb, false);
	}
	else {
		if (mpp->hdl && mpp->hdl->cont)
			WriteFmtHF(wf, "headerl", mpp->hdl, true);
		if (mpp->ftl && mpp->ftl->cont)
			WriteFmtHF(wf, "footerl", mpp->ftl, false);
		if (mpp->hdr && mpp->hdr->cont)
			WriteFmtHF(wf, "headerr", mpp->hdr, true);
		if (mpp->ftr && mpp->ftr->cont)
			WriteFmtHF(wf, "footerr", mpp->ftr, false);
	}
	wf->putEnd();

	FlowWritten = false;
}

void 
RTFwr::WriteFmtHF(RTFwfile *wf, char *cw, hfinfo *hf, bool head)
{
	if (!hf || !cw)
		return;
	char *defname = head ? "Header" : "Footer";

	wf->putEnd();
	wf->putGS();
	wf->putCW(cw);

	// get format
	if (!hf->fmtname)
		hf->fmtname = defname;
	long fnum = FormNameList(hf->fmtname);
	if (!fnum)  // not defined yet, add it
		fnum = DefineFormat(hf->fmtname, 1);
	if (!fnum) {
		LogEvent(logwarn, 1, defname, " format not found: ", hf->fmtname);
		fnum = FormNameList(defname);
	}
	if (!fnum)
		fnum = 1;  // give up, use Normal

	pprop	*cform = (pprop *) FormList.find(fnum);
	if (cform)
    InitForm(cform);

	// write para start code
	SetPardProps(wf);
	wf->putCWN("s", GetFinalForm(Curr.snum));
  WriteParaProps(wf);
	WriteBorders(wf, &Curr);
	WriteShading(wf, Curr.shading);
	WriteSimpleCharProps(wf);
	WriteTabDefs(wf, &Curr);
  Last = Curr;
	if (Curr.styadd)
		DCmacr::WriteMacro(wf, Curr.styadd);

	// write macro content
	DCmacr::WriteMacro(wf, hf->cont);

	// write para end code
	wf->putCW("par");

	wf->putGE();
}


void 
RTFwr::DefineRequiredFormats(DCfrfile *fmts)
{
	char *fname = NULL;
	char *ftype = NULL;
	uns tnum = 0;
	DCnlist fnames;
	DCilist ftyps;
	long count = 0;

	// already in section
	while (fmts->NextItem()) {
		fname = fmts->StrLeft();
		ftype = fmts->StrRight();
		tnum = 0;
		if (ftype) {
			if (!stricmp(ftype, "block"))
				tnum = 1;
			else if (!stricmp(ftype, "inline"))
				tnum = 2;
		}
		if (!tnum)
			tnum = 1;
		fnames.add(fname, ++count);
		ftyps.add(tnum, count);
	}

	for (long i = 1; i <= count; i++) {
		fname = fnames.find(i);
		tnum = (uns) ftyps.find(i);
		DefineFormat(fname, tnum);
	}
}


uns 
RTFwr::DefineFormat(char *fname, uns ftype)
{
	Curr = DefForm;
	FormName = fname;
	DCLFormNum++;
	Curr.stype = ftype;
	pprop *form = new pprop;
	Curr.snum = Curr.snext = FormNum++;
	Curr.stag = FormName;
	GetFormatFileInfo(FormName);
	*form = Curr;
	FormList.add(form, FormNum);
	FormRefList.add(DCLFormNum, FormNum);
	FormNameList.add(FormName, FormNum);
	FormsUsed.add(FormNum);
	FormName = NULL;
	if (form->tcount)
		FormTabList.add(form->tabs, FormNum);
	return FormNum;
}



bool RTFwr::FrameTop = false;
bool RTFwr::HFDone = false;
bool RTFwr::FrameHF = true;
bool RTFwr::WritingHF = false;
bool RTFwr::PosHFText = true;
bool RTFwr::WritingHFGraph = false;
char *RTFwr::HFHRef;
short RTFwr::HFHPos;
short RTFwr::HFWide;
short RTFwr::HFVPos;
short RTFwr::HFBottom;
short RTFwr::HFVAdjust = 100;


void
RTFwr::WriteHF(RTFwfile *wf, char *chp, uns mpg, bool head, bool right)
{
	frame *fr = (frame *) (head ? HeaderList.find(mpg) : FooterList.find(mpg));
	HFBottom = head ? TMarg : BodyHigh - BMarg;

	CurrTextFrame = NULL;
	wf->putEnd();
	wf->putGS();
  wf->putCW(chp);

	if (fr != NULL) {
		// always frame headers and footers, and use page vert positioning
		HFHRef = "phcol";
		if ((SideHType == 'F')
		 && SideHFlip
		 && !right)
			HFHPos = RMarg - TextRMarg;
		else
			//HFHPos = fr->xpos - TextLMarg;
			HFHPos = LMarg - TextLMarg;
		HFWide = BodyWide;
		HFVPos = fr->ypos + HFVAdjust;  // 5pt adjustment down
		HFBottom = HFVPos + fr->high;

		// write the text paragraphs (with variables, but not ftns or xrfs)
		PosHFText = false; // already set
		NonWhiteText = false;
		WriteHFText(wf, fr);
		if (!NonWhiteText)    // if nothing is there
			HFBottom = HFVPos;  // don't count frame

		// put out any graphics frames; can include metafiles
		if ((HFType != 'T')
		 && (fr->grcount > 0))
			(fr->grlist)->writeall(wf, WriteHFGraphic);
	}
	else {  // put empty header/footer
		SetPardProps(wf);
	  wf->putCW("par");
	}

	// put out any additional text frames
	if ((HFType != 'T')
	 && (head == true)
	 && (FrameHF == true)) {
		HFHRef = "phpg";
		PosHFText = true;
		MPTextList.write(mpg, wf, WriteHFText);
	}

	wf->putGE();
}


void
RTFwr::WriteHFText(DCfile *dwf, void *vp)
{
	RTFwfile *wf = (RTFwfile *) dwf;
	frame *fr = (frame *) vp;
	RTFgroup* gp = NULL;

	if (fr->ftext
	 && ((gp = (RTFgroup *) FlowList.find(fr->ftext)) != NULL)) {
		WritingHF = true;
		FrameTop = true;
		Last = DefForm;
		LinePos = 0;
		if (PosHFText) {
			HFHPos = fr->xpos;
			HFWide = fr->wide;
			HFVPos = fr->ypos;
			if ((HFVPos + fr->high) > HFBottom)
				HFBottom = HFVPos + fr->high;
		}
		gp->write(*wf);
		if (Curr.runin) {
			Curr.runin = 0;
			WriteText(wf);
			EndPara(wf);
		}
		if (PGStarted) {
			wf->putGE();
			PGStarted = 0;
			wf->putEnd();
		}
		WritingHF = false;
	}
}


void
RTFwr::WriteHFGraphic(DCfile *wf, void *vp)
{
	graph *gr = new graph;
	*gr = *((graph *) vp);

#if 0
	if (gr->gtype == 0) {  // frame wrapper
		WriteHFGrFrame(wf, (frame *) gr->gparm);
		return;
	}
#endif

	short len;
	short *vpt = gr->gparm;
	switch (gr->gtype) {
		case 0x324:  // polygon
		case 0x325:  // polyline
			len = (*vpt * 2) + 1;
			break;
		case 0x41B:  // rect
			len = 4;
			break;
		case 0x61C:  // rounded rect
			len = 6;
			break;
		case 0:      // import object
		case 0xF43:  // raster
			len = 0;
			break;

		default:
			delete gr;
			return;
	}
	if (len) {
		gr->gparm = new short[len];
		memcpy(gr->gparm, vpt, len * sizeof(short));
	}

	// construct a new frame for each object
	frame *gfr = new frame;
	gfr->grcount = 1;
	gfr->grlist = new DCvlist;
	(gfr->grlist)->add(gr, 1);

	gfr->ftext = 0;
	gfr->fpos = 3;
	gfr->href = 1;
	gfr->vref = 1;
	gfr->halgn = 0;
	gfr->valgn = 0;

	if (gr->gtype == 0) { // import object
		frame *ofr = (frame *) gr->gparm;
		gfr->xpos = ofr->xpos;
		gfr->ypos = ofr->ypos;
		gfr->wide = ofr->wide;
		gfr->high = ofr->high; 
		ofr->xpos = ofr->ypos = 0;
	}
	else if (gr->gtype == 0xF43) { // raster)
		rast *ofr = (rast *) gr->gparm;
		gfr->xpos = ofr->ixpos;
		gfr->ypos = ofr->iypos;
		gfr->wide = ofr->iwide;
		gfr->high = ofr->ihigh; 
		ofr->ixpos = ofr->iypos = 0;
	}
	else {
		// set frame pos and size to object bounding box
		GetGraphicBounds(gr, gfr);

		// make object xypos coords relative to frame pos
		SetGraphicPos(gr, gfr->xpos, gfr->ypos);
	}

	WriteHFGrFrame(wf, gfr);
	delete gfr;
}


void
RTFwr::WriteHFGrFrame(DCfile *wf, frame *gfr)
{
	WritingHFGraph = true;
	WriteFrame((RTFwfile *) wf, gfr);
	if ((gfr->xpos <= (LMarg + (BodyWide/2)))
	 && ((gfr->ypos + gfr->high) > HFBottom))
		HFBottom = gfr->ypos + gfr->high;
	WritingHFGraph = false;
}


void
RTFwr::GetGraphicBounds(graph *gr, frame *gfr)
{
	// frame pos is the xy of object top left corner
	// frame width and height is object bounding box
	short left = 0;
	short top = 0;
	short right = 0;
	short bot = 0;
	short len;
	short *gpt = gr->gparm;
	short val;
	short wid;

	switch (gr->gtype) {
		case 0x324:  // polygon
		case 0x325:  // polyline
			len = *gpt++;
			left = right = *gpt++;
			top = bot = *gpt++;
			while (--len) {
				val = *gpt++;  // xval
				if (val < left)
					left = val;
				else if (val > right)
					right = val;
				val = *gpt++;  // yval
				if (val < top)
					top = val;
				else if (val > bot)
					bot = val;
			}				 
			break;
		case 0x61C:  // round rect
			gpt += 2; // skip corner radii
			// fall through
		case 0x41B:  // rect
			bot = *gpt++;
			right = *gpt++;
			top = *gpt++;
			left = *gpt++;
			if ((bot - top) < 20)
				gr->gparm[0] = bot = top + 20;
			if ((right - left) < 20)
				gr->gparm[1] = right = left + 20;
			break;
		default:
			break;
	}

	if (gr->gpen)
		wid = gr->gpen->pwidth;
	else wid = 20;
	
	gfr->xpos = left - wid;
	gfr->ypos = top - wid;

	val = (right - left) + wid;
	gfr->wide = (val < 120) ? 120 : val;

	val = (bot - top) + wid;
	gfr->high = (val < 120) ? 120 : val; 
}


void
RTFwr::SetGraphicPos(graph *gr, short xp, short yp)
{
 	// adjust gr->gparm[i] by subtracting xp and yp from xypos values
	short len;
	short *gpt = gr->gparm;

	switch (gr->gtype) {
		case 0x324:  // polygon
		case 0x325:  // polyline
			len = *gpt++;
			while (len--) {
				*gpt -= xp ;  // xval
				gpt++;
				*gpt -= yp;  // yval
				gpt++;
			}				 
			break;
		case 0x61C:  // round rect
			gpt += 2; // skip corner radii
			// fall through
		case 0x41B:  // rect
			*gpt -= yp;
			gpt++;
			*gpt -= xp;
			gpt++;
			*gpt -= yp;
			gpt++;
			*gpt -= xp;
			gpt++;
			break;
		default:
			break;
	}
}


unl RTFwr::ExtFileStartLine = 0;
unl RTFwr::ExtFileEndLine = 0;
unl RTFwr::ExtFileStartChar = 0;
unl RTFwr::ExtFileEndChar = 0;
unl RTFwr::ExtCodeFileLen = 0;
char *RTFwr::ExtCodeFileEnc = NULL;


void
RTFwr::WriteFromExtFile(RTFwfile *wf, RTFctl *cp)
{
	// write contents of named file as text as is ****
	char *fname = (char *) cp->ex();
	char *hash = strrchr(fname, '#');
	if (hash) {
		unl sline = 0;
		unl eline = 0;
		unl schar = 0;
		unl echar = 0;
		unl slen = 0;
		char *eval = NULL;
		char *senc = NULL;
		char *eenc = NULL;
		*hash++ = '\0';
		while (*hash) {
			if (!strncmp(hash, "line=", 5)) {
				hash += 5;
				sline = strtoul(hash, &eval, 10) + 1;
				if (*eval == ',')
					eline = strtoul(eval + 1, &eval, 10);
				hash = eval + 1;
			}
			else if (!strncmp(hash, "char=", 5)) {
				hash += 5;
				schar = strtoul(hash, &eval, 10);
				if (*eval == ',')
					echar = strtoul(eval + 1, &eval, 10);
				hash = eval + 1;
			}
			else if (!strncmp(hash, "length=", 7)) {
				hash += 7;
				slen = strtoul(hash, &eval, 10);
				if (*eval++ == ',') {
					if ((eenc = strchr(eval, ';')) != NULL)
						senc = NewName(eval + 1, eenc - eval);
					else 
						senc = NewName(eval + 1);
					hash = eval + 1 + strlen(senc);
				}
				else
					hash = eval + 1;
			}
			else if (*hash == ';')
				hash++;
			else
				break;
		}
		if (sline)
			ExtFileStartLine = sline;
		if (eline)
			ExtFileEndLine = eline;
		if (schar)
			ExtFileStartChar = schar;
		if (echar)
			ExtFileEndChar = echar;
		if (slen)
			ExtCodeFileLen = slen;
		if (senc)
			ExtCodeFileEnc = senc;
	}

	DCrfile *rf = new DCrfile(fname);
	if (!rf || rf->FErr()) {
		LogEvent(logwarn, 1, "Cannot find included file \"", fname, "\".");
		return;
	}
	long len = rf->getSize();
	if (!len) {
		LogEvent(logwarn, 1, "Included file \"", fname, "\" is empty.");
		return;
	}
	else if (ExtCodeFileLen
	 && (ExtCodeFileLen != len)) {
		char *lstr = NewName(30);
		sprintf(lstr, "%ld not %ld", len, ExtCodeFileLen);
		LogEvent(logwarn, 1, "Included file \"", fname, "\" is wrong length:", lstr);
		DeleteName(lstr);
		return;
	}

	unc *block = new unc[len + 1];
	rf->getBlock(block, len);
	if (rf->FErr()) {
		LogEvent(logwarn, 1, "Error reading included file \"", fname, "\".");
		return;
	}

	wf->putGS();

	wf->putCW("line");  // start with line break
	wf->putEnd();

	unc ch = 0;
	unl uch = 0;
	unc *cpt = block;
	unc *ept = block + len;
	unl lcount = 1;
	bool skip = ExtFileStartLine ? true : false;
	unl ccount = 0;
	unl climit = 0;
	if (ExtFileStartChar
	 || ExtFileEndChar)
		climit = (ExtFileEndChar - ExtFileStartChar) + 1;

	for (; cpt < ept; cpt++) {
		ch = *cpt;
		switch (ch) {
			case '\r':
				break;

			case '\n':
				lcount++;
				if (!skip) {
					if (ExtFileStartChar) {
						ExtFileStartChar--;
						break;
					}
					else if (climit)
						ccount++;
					wf->putCW("line");  // make line break
					CurrTab->tpos = 0;
					wf->putEnd();
				}
				if (ExtFileStartLine
				 && (lcount >= ExtFileStartLine)) {
					ExtFileStartLine = 0;
					skip = false;
				}
				if (ExtFileEndLine
				 && (lcount > ExtFileEndLine)) {
					ExtFileEndLine = 0;
					skip = true;
				}
				break;

			case '\t':
				if (!skip) {
					if (ExtFileStartChar) {
						ExtFileStartChar--;
						break;
					}
					else if (climit)
						ccount++;
			    wf->putCW("tab");  // ensure format used has tabs set
				}
				break;

			case '\\':
			case '{':
			case '}':
				if (!skip) {
					if (ExtFileStartChar) {
						ExtFileStartChar--;
						break;
					}
					else if (climit)
						ccount++;
					wf->putCS(ch);
				}
				break;

			default:
				if ((uch = ::UTF8(&cpt)) != 0) { // unicode sequence
					cpt--;
					if (!skip
					 && (uch != 0xFEFF)) { // BOM, ignore
						if (ExtFileStartChar) {
							ExtFileStartChar--;
							break;
						}
						else if (climit)
							ccount++;
						if (!InUnicode) {
							InUnicode = true;
							if (InAnsi == true) {
								if (SpaceAfterUnicode) {
									wf->putCS('~');
									wf->putCS('~');
								}
								InAnsi = false;
							}
						}
						wf->putCS('u');
						wf->putLong((short) uch, true);
						wf->putChar(' ');
						if (SpaceAfterUnicode)  // space needed for Asian display
							wf->putChar(' ');
					}
				}
				else if (!skip) {
					InUnicode = false;
					InAnsi = true;
					if (ExtFileStartChar) {
						ExtFileStartChar--;
						break;
					}
					else if (climit)
						ccount++;
					if (ch > 0x7E)
						wf->putSpec(ch);
					else
						wf->putText(ch);
				}
				break;
		}
		if (climit
		 && (ccount >= climit))
			break;
	}

	wf->putGE();

	ExtFileStartLine = 0;
	ExtFileEndLine = 0;
	ExtCodeFileLen = 0;
	ExtFileStartChar = 0;
	ExtFileEndChar = 0;
	ExtCodeFileEnc = NULL;
}




bool RTFwr::FirstFlowFrame = false;
unl RTFwr::FlowFramePage = 0;
DClist RTFwr::PageStartList;
frame *RTFwr::CurrTextFrame = NULL;
bool RTFwr::FrameEndPara = true;
bool RTFwr::EndingTextFrame = false;

void
RTFwr::CheckTextFrame(RTFwfile *wf, RTFctl *cp)
{
	if (HelpForm
	 || (!UseTextFrames)
	 || WritingHF)
		return;

	FlowFrameNum = cp->dat();
	if (SideHPropsInFlow == false) {
		ResetSideHProps();
		if (SideHUsed)
			SetSideHProps(wf, (shprop *) SideHFrameList.find(FlowFrameNum));
	}

	frame *fr = NULL;
	if (CurrTextFrame) {  // end any current frame
		if (Curr.pbreak == 3)
			Curr.pbreak = 0;  // no column break after
		if (FrameEndPara) {
			SetPardProps(wf);   // add anchoring para
			wf->putCW("par");
		}
		if (PGStarted) {
			wf->putGE();
			PGStarted = 0;
			wf->putEnd();
		}
		EndingTextFrame = true;
	}

	if (PageStartList.find(FlowFrameNum)
	 && (FlowFramePage != cp->siz())
	 && !FirstTop
	 && !StartingPage
	 && !StartingPageBody) {
		if (PBMethod == 'P')
			PageBreakPending = true;
		else if (PBMethod == 'C')
			wf->putCW("page");
    wf->putEnd();
    StartingPage = true;
	  StartingPageBody = true;
		ParaSpace = BodyParaSpace = 0;
	  Curr.pbreak = 0;
  }

	if (((fr = (frame *) FrameList.find(FlowFrameNum)) != NULL)
	 && ((fr->wide < (BodyWide / 2))
	  || (fr->high < (BodyHigh / 3)))) {
		CurrTextFrame = fr;
		if (Curr.pbreak == 3)
			Curr.pbreak = 0;  // no column break before
		if (FirstTop
		 && ((fr->high + fr->pypos) > FirstHBottom))
			FirstHBottom = fr->high + fr->pypos;
	}
	else
		CurrTextFrame = NULL;

	FlowFramePage = cp->siz();
}


void
RTFwr::WriteFlowNum(RTFwfile *wf, RTFctl *cp)
{
	uns cflow = cp->dat();

	if (CurrFlow == cflow)
		return;
 	if (SingleSect) {
		SideHUsed = (bool) SideHFlowList.find(cflow);
		if (CurrFlow == 0) {
			ResetSideHProps();
			if (SideHUsed)
				SetSideHProps(wf, (shprop *) SideHTFlowList.find(cflow), NULL, true);
		}
	}
	CurrFlow = cflow;
}


void
RTFwr::SetSideHProps(RTFwfile *wf, shprop *shp, page *fp, bool sec)
{
	bool rset = false;
	bool lset = false;

	if (shp != NULL) {
		SideHWide = shp->shwid;
		SideHGap = shp->shgap;
		SideHPos = shp->shpos;
		SideHFlip = shp->shflip;
	}

	if (SideHPos == 1) {		// on the left
		if (SideHType == 'I') {  // left indent for text, right for heads
			TextLind = SideHWide + SideHGap;
			SideHRind = BodyWide - SideHWide;
		}
		else if (SideHType == 'F') {
			TextLMarg = LMarg + SideHWide + SideHGap;
			if (sec) {
				wf->putCWN("marglsxn", TextLMarg);
				lset = true;
			}
		}
	}
	else if (SideHPos == 2) {  // right		
		if (SideHType == 'I') {  // right indent for text, left for heads
			TextRind = SideHWide + SideHGap;
			SideHLind = BodyWide - SideHWide;
		}
		else if (SideHType == 'F') {
			TextRMarg = RMarg + SideHWide + SideHGap;
			if (sec) {
				wf->putCWN("margrsxn", TextRMarg);
				rset = true;
			}
		}
	}

	if (fp) {
		if ((fp->plmrg != LMarg) && !lset)
			wf->putCWN("marglsxn", LMarg = fp->plmrg);
		if ((fp->prmrg != RMarg) && !rset)
			wf->putCWN("margrsxn", RMarg = fp->prmrg);
	}

}


void
RTFwr::ResetSideHProps(void)
{
	SideHWide = 0;
	SideHGap = 0;
	SideHPos = 0;
	SideHFlip = false;
	SideHRind = 0;
	SideHLind = 0;
	TextRind = 0;
	TextLind = 0;
	TextLMarg = LMarg;
	TextRMarg = RMarg;
}


bool RTFwr::EndUL = false;  // set if underlining a space
bool RTFwr::DiscHyphen = false;  // set if last char was disc hyphen
bool RTFwr::HardHyphen = false;  // set if last char was real hyphen
bool RTFwr::HardRetSpace = false;  // set if last char was hard return


void
RTFwr::WriteText(RTFwfile *wf, RTFctl *cp)
{
  uns wch;
	uns clwch;
  size_t i;
  unc *cpt;
	unc ch;

	if (CondEndPend
	 && !CondFlagDiv) {
		PreCondFlagDiv = wf->GetDiv();
		CondFlagDiv = wf->NewDiv();
	}
	
	if (ParaStartFrPend)
		WriteParaStartFrames(wf);

	if (InFormPx || InFormSx)
		return;

	if (WritingAnum
	 && !SeqAnums
	 && (CurrAnumToken > 0))
		return;

  if (cp && InIndexGroup) {
    PutIndexEntry(cp);
    return;
  }


  if (cp && (cp->siz() == 0))
    return;

	if ((HideWhite == true)
	 && (Curr.colr == 9)) {
		WhiteText = true;
		if (WritingAnumVal)
			WhiteAnumVal = true;
		return;
	}

	if (InTextPx && !WriteAnums)
		return;

	if (HelpTitlePending
	 && (Curr.help == 0))
		return;

	if ((HelpForm == true)
	 && (MetaType == 'E')
	 && (Curr.ding < 31)
	 && (Curr.ding != 1)) {
		if (InTextMetafile) {
			if (Curr.base == 1)
				EndTextMetafile(wf);
			else if (cp) {
				ProcTextMetafile(cp);
				return;
			}
		}
		else if (Curr.base > 1) {
			if (Curr.clone)
				Curr.base = 1;
			else if (cp) {
				if (!InPara) {  // unlikely
					unc tbase = Curr.base;
					Curr.base = 1;
					WriteProps(wf);
					InPara = true;
					Curr.base = tbase;
				}
				StartTextMetafile();
				ProcTextMetafile(cp);
				return;
			}
		}
	}

	if (XrefDelete)
		return;

	if (!Curr.del && Curr.maccb) {
		DCmacr::WriteMacro(wf, Curr.maccb);
		Curr.maccb = NULL;
	}

  WriteProps(wf);

	WriteCondProps(wf);

	if (!Curr.del) {
		if (ParaForm.maccs) {
			DCmacr::WriteMacro(wf, ParaForm.maccs);
			ParaForm.maccs = Curr.maccs = NULL;
		}
		if (CForm.maccs) {
			DCmacr::WriteMacro(wf, CForm.maccs);
			CForm.maccs = Curr.maccs = NULL;
		}
		if (TopicStartInsert
		 && StartingHelpTopic) {
			DCmacr::WriteMacro(wf, TopicStartInsert);
			StartingHelpTopic = false;
		}
	}

	if ((HideWhite == true)
	 && (Curr.colr == 9)) { // check again, in case of InheritPending
		WhiteText = true;
		if (WritingAnumVal)
			WhiteAnumVal = true;
		return;
	}
	NonWhiteText = true;

	if ((XRTused == 1)
	 && (WritingAnum == false)) {
		// write index entries for this para, if any, here
		XRStartPara(wf);
	}

  InPara = true;

	//if (!InTbCell)
	ParaStarting = false;

	if (!WritingHF) {
		StartingPage = false;
		if (!CurrTextFrame
		 && !(SideHUsed
		  && (Curr.sideh > 0)))
			StartingPageBody = false;
		if (HULevel)
			WriteHyperUnique(wf);
		if (RefLevel)
			WriteRefStrs(wf);
		if (IndexEntry) {
			if (HyperHelp
			 && (cp != NULL)
			 && (!SuppressSpace)
			 && (((cp->dt() == imnem)
			   && (cp->siz() == 0x20))
			  || ((cp->dt() == etext)
			   && (*((unc *)(cp->ex())) == 0x20)))) {
				if (Curr.clone)
					PutHelpCloneBuf(wf, ' ');
				if (!Curr.del)
					wf->putText(' ');
        AddChar(' ');
				SuppressSpace = true;
			}
			WriteIndexEntry(wf);  // unless already written
		}
	}

	if (Curr.del != DeletingText) {  // starting or ending deleted format
		DeletingText = Curr.del;
		if (Curr.del) {  // starting it
			if (Curr.replace) {
				// wf->putStr(Curr.replace);  // raw RTF string
				DCmacr::WriteMacro(wf, Curr.replace);
				Curr.replace = NULL;
				if (Curr.cstag)
					CForm.replace = NULL;
				else
					ParaForm.replace = NULL;
			}
			else if (Curr.maccr) {
				DCmacr::WriteMacro(wf, Curr.maccr);
				Curr.maccr = NULL;
				if (Curr.cstag)
					CForm.maccr = NULL;
				else
					ParaForm.maccr = NULL;
			}
		}
	}

	if (cp == NULL)
		return;

	if (!InHelpTitlePara)
		TopicEmpty = false;

	if (ExactLSUsed)
		GrTbSpace = true;

  CharWid = (Curr.fwid * Curr.fsize) / 24;
  if (TabAlign == 1)
    CharWid = CharWid / 2;    // center tab
  else if (TabAlign == 2)
    CharWid =  0;             // right tab
  FixedWid = ((Curr.fnum == 3) || (Curr.fnum == 14) || (Curr.fnum == 15));

	if ((HelpForm == true)
	 && (NoDing == true)
	 && (Curr.ding == 1)) {
		if ((Curr.clone == 0)
		  || (HelpCloneDiv != 0)) {  // replace any dingbats with a bullet
			if (Curr.fnum != 1)
				wf->putCWN("f", Last.fnum = Curr.fnum = 1);
			PutBullet(wf);
		}
		return;
	}

	if (ForcedBreak) {
		if (DiscHyphen
		 && !Curr.del)
			wf->putText('-');
		wf->putCW("line");
		CurrTab->tpos = 0;
		wf->putEnd();
		LinePos = Curr.lind;
		ForcedBreak = false;
	}

	EndUL = false;
	DiscHyphen = false;
	HardHyphen = false;
	HardRetSpace = false;
	LastLineEnd = NULL;

  if (cp->dt() == imnem) {  // single char form
		clwch = 0;
		wch = (uns) cp->siz();

		bool asia = false;
		if ((Curr.fenc >= 128)
		 && (Curr.fenc <= 136))
			asia = true;
#if 0
		else if ((cp->siz() > 0x10000) // sign extended char
		 || (((wch >> 8) & 0xFF) == 0xFF))
			wch &= 0xFF;
#endif

		if (HelpForm) {
			CheckDing(wf, wch);
			if (wch == 0x95) {
				PutBullet(wf);
				return;
			}
		}
		if (SuppressSpace) {
			if (wch == 0x20)
				return;
			SuppressSpace = false;
		}
		if (WritingXRVal) {
			if (XRValSpace
			 && ((wch == 0x20)
			  || (wch == 0xA0)))
				return;
			else if (XRValSpace)
				XRValSpace = false;
			else if ((wch == 0x20)
			 || (wch == 0xA0))
				XRValSpace = true;
		}

		finfo *finf = NULL;
		if ((finf = (finfo *) FontList.find(Curr.fnum)) != NULL) {
			Curr.ftype = finf->ftype;
			Curr.fenc = finf->fenc;
			Curr.cpg = finf->cpg;
		}
		else {
			Curr.ftype = 1;
			Curr.fenc = 0;
			Curr.cpg = 0;
		}

    switch (wch) {
			case 0:      // replaced by bitmap
				return;
      case 0xA0:
				if (asia)
					goto normal;
				if (Curr.ding || Curr.fenc) {
					wf->putSpec(0xA0);
					AddChar(' ');
					break;
				}
      case 0x2002:
      case 0x2003:
      case 0x2007:
      case 0x2009: // fixed spaces
				if (Curr.clone)
					PutHelpCloneBuf(wf, ' ');
				if (Curr.del)
					break;
				if (HelpForm && (HlpVer == '3'))
	        wf->putText(' ');
				else
	        wf->putCS('~');
        AddChar(' ');
        break;
      case 0xAD:  // disc hyphen
				if (asia)
					goto normal;
				if (Curr.ding || Curr.fenc) {
					wf->putSpec(0xAD);
					AddChar(' ');
					break;
				}
				DiscHyphen = true;
				if (HelpForm) {
					if (HlpVer == '3')
						break;
					if (ForceTbLineBreaks
					 && InTbCell
					 && !InFootnote)
						break;
				}
				if (Curr.del)
					break;
        wf->putCS('-');
        break;
      case 0x2011:
				HardHyphen = true;
				if (Curr.clone)
					PutHelpCloneBuf(wf, '-');
				if (Curr.del)
					break;
				if (HelpForm)       // && (HlpVer == '3'))
	        wf->putText('-');
				else
					wf->putCS('_');
        AddChar('-');
        break;
			case 0x2020:
				wf->putSpec(0x86);
				break;
			case 0x20AC:
				wf->putSpec(0x80);
				break;
			case 0x2021:
				wf->putSpec(0x87);
				break;
			case 0x2026:
				wf->putSpec(0x85);
				break;
			case 0x2030:
				wf->putSpec(0x89);
				break;
			case 0x2039:
				wf->putSpec(0x8B);
				break;
			case 0x203A:
				wf->putSpec(0x9B);
				break;
      case 0x2044:
				if (Curr.clone)
					PutHelpCloneBuf(wf, '/');
				if (Curr.del)
					break;
        wf->putText('/');
        AddChar('/');
        break;

			case 0x201A:
				wch = 130;
				goto squote;
			case 0x2018:
				wch = 145;
				goto squote;
			case 0x2019:
				wch = 146;
				goto squote;

			case 130:
			case 145:
			case 146:
				if (asia)
					goto normal;
			squote:
				if (QuoType == 'H')
					wch = '\'';
				else if (HelpForm) {
					if (Curr.clone)
						PutHelpCloneBuf(wf, '\'');
					if (Curr.del)
						break;
					if (wch == 130)
						wf->putSpec(130);
					else if (QuoType == 'N')
						wf->putSpec((unc) wch);
					else
						wf->putCW((wch == 145) ? "lquote" : "rquote");
          LinePos += CharWid;
					break;
				}
				goto normal;

			case 0x201e:
				wch = 132;
				goto dquote;
			case 0x201c:
				wch = 147;
				goto dquote;
			case 0x201d:
				wch = 148;
				goto dquote;

			case 132:
			case 147:
			case 148:
				if (asia)
					goto normal;
			dquote:
				if (QuoType == 'H')
					wch = '"';
				else if (HelpForm) {
					if (Curr.clone)
						PutHelpCloneBuf(wf, '"');
					if (Curr.del)
						break;
					if (wch == 132)
						wf->putSpec(132);
					else if (QuoType == 'N')
						wf->putSpec((unc) wch);
					else
						wf->putCW((wch == 147) ? "ldblquote" : "rdblquote");
          LinePos += CharWid;
					break;
				}
				goto normal;

			case 0x2022:  // bullet
				wch = 149;
			case 149:
				goto normal;

			case 0x2122:  // trademark
				wch = 153;
			case 153:
				goto normal;

			case 0x2013:
				wch = 150;
				goto dash;
			case 0x2014:
				wch = 151;
				goto dash;

			case 150:
			case 151:
				if (asia)
					goto normal;
			dash:
				HardHyphen = true;
				if (HelpForm && (HlpVer == '3'))
					wch = '-';
				else if (HelpForm) {
					if (Curr.clone)
						PutHelpCloneBuf(wf, '-');
					if (Curr.del)
						break;
					if (QuoType == 'N')
						wf->putCW((wch == 150) ? "endash" : "emdash");
					else
						wf->putSpec((unc) wch);
          LinePos += CharWid;
					break;
				}
				if (Curr.clone)
					clwch = '-';
				goto normal;

      case '\\':
      case 0x7B:
      case 0x7D:
				if (asia)
					goto normal;
				if (Curr.del)
					break;
        wf->putCS(wch);
        LinePos += CharWid;
        break;

			case '-':
				if (asia)
					goto normal;
				HardHyphen = true;
				goto normal;

			case 0x20:
				if (Curr.line > 1)
					EndUL = true;
				if (HelpForm
				 && ForceTbLineBreaks
				 && (HlpVer != '3')
				 && InTbCell
				 && !InFootnote) {
					if (Curr.clone)
						PutHelpCloneBuf(wf, ' ');
					if (Curr.del)
						break;
	        wf->putCS('~');
					AddChar(' ');
					break;
				}
				// fall through

			normal:
      default:
				if (MIFVer < 8) {  // already using Windows code page
					if (wch > 0xFF)  // ignore other syms for now
						return;
				}
				else if (wch >= 0xA0) {  // Unicode, write as \uNNNN
					if (Curr.del)
						return;
					if (!InUnicode) {
						InUnicode = true;
						if (InAnsi == true) {
							if (SpaceAfterUnicode) {
								wf->putCS('~');
								wf->putCS('~');
							}
							InAnsi = false;
						}
					}
					wf->putCS('u');
					wf->putLong((short) wch, true);
					wf->putChar(' ');
					if (SpaceAfterUnicode)  // space needed for Asian display
						wf->putChar(' ');
					return;
#if 0
					uns nch = 0;  // Unicode, map to code page
					if ((nch = ProcUnicode(wf, wch)) != 0) {
						PutUText(wf, (unc) (nch & 0xFF));
						if (nch > 0xFF)  // Asian, may be two bytes
							PutUText(wf, (unc) ((nch >> 8) & 0xFF));
		        LinePos += CharWid;
						return;
					}
#endif
				}
				else {
					InUnicode = false;
					InAnsi = true;
				}

				if (Curr.clone
				 && !Curr.ding) {
					if (clwch == 0)
						clwch = wch;
					PutHelpCloneBuf(wf, clwch);
				}

				if (Curr.del)
					break;

        if (wch > 0x7E) {
          wf->putSpec((unc) wch);
          LinePos += CharWid;
        }
        else {
					if (Curr.caps == 3)
						wch = tolower(wch);
          wf->putText(wch);
          AddChar((unc) wch);
        }
        break;
    }
    return;
  }

  // string has only plain text
	InUnicode = false;
	InAnsi = true;
  for (i = cp->siz(), cpt = (unc *) cp->ex(); i && *cpt; i--, cpt++) {
		ch = *cpt;

		if (SuppressSpace) {
			if (ch == ' ')
				continue;
			SuppressSpace = false;
		}
		if (WritingXRVal) {
			if (XRValSpace
			 && (ch == ' '))
				continue;
			else if (XRValSpace)
				XRValSpace = false;
			else if (ch == ' ')
				XRValSpace = true;
		}

    switch (ch) {
			case 130:
			case 145:
			case 146:
				if (Curr.clone)
					PutHelpCloneBuf(wf, '\'');
				if (Curr.del)
					break;
				if (QuoType == 'H')
					wf->putText('\'');
        else wf->putText(ch);
        AddChar(ch);
        break;
			case 132:
			case 147:
			case 148:
				if (Curr.clone)
					PutHelpCloneBuf(wf, '"');
				if (Curr.del)
					break;
				if (QuoType == 'H')
					wf->putText('"');
        else wf->putText(ch);
        AddChar(ch);
        break;
      case '\\':
      case 0x7B:
      case 0x7D:
				if (Curr.del)
					break;
        wf->putCS(ch);
        LinePos += CharWid;
        break;

			case '-':
				HardHyphen = true;
				goto normal2;

			case 0x20:
				if (Curr.line > 1)
					EndUL = true;
				if (HelpForm
				 && ForceTbLineBreaks
				 && (HlpVer != '3')
				 && InTbCell
				 && !InFootnote) {
					if (Curr.clone)
						PutHelpCloneBuf(wf, ' ');
					if (Curr.del)
						break;
	        wf->putCS('~');
					AddChar(' ');
					break;
				}
				// fall through

			normal2:
      default:
				if (Curr.clone
				 && !Curr.ding)
					PutHelpCloneBuf(wf, (uns) ch);
				if (Curr.del)
					break;
				if (Curr.caps == 3)
					ch = tolower(ch);
        wf->putText(ch);
        AddChar(ch);
        break;
    }
  }
}


bool RTFwr::InUnicode = false;
bool RTFwr::InAnsi = false;


#if 0
void 
RTFwr::PutUText(RTFwfile *wf, unc ch)
{
#if 0
	switch (ch) {
		//case '{':
		//case '}':
		//case '\\':
    //  wf->putSpec(ch);
		//	break;

		default:
			wf->putText(ch);
			break;
	}
#endif
}


unc RTFwr::CyrLatinSet[64] = {
 0, 0, 1, 0, 1, 1, 1, 1,   // 0x80
 0, 1, 0, 1, 0, 0, 0, 0,   // 0x88
 0, 1, 1, 1, 1, 1, 1, 1,   // 0x90
 0, 1, 0, 1, 0, 0, 0, 0,   // 0x98
 1, 0, 0, 0, 1, 0, 1, 1,   // 0xA0
 0, 1, 0, 1, 1, 1, 1, 0,   // 0xA8
 1, 1, 0, 0, 0, 1, 1, 1,   // 0xB0
 0, 0, 0, 1, 0, 0, 0, 0    // 0xB8
};

uns RTFwr::CyrLowSet[16] = {
 0x0, 0xAB, 0x80, 0x81, 0xAA, 0xBD, 0xB2, 0xAF,
 0xA3, 0x8A, 0x8C, 0x8E, 0x8D, 0x0, 0xA1, 0x8F
};

uns RTFwr::CyrHighSet[16] = {
 0x0, 0xB8, 0x90, 0x83, 0xBA, 0xBE, 0xB3, 0xBF,
 0xBC, 0x9A, 0x9C, 0x9E, 0x9D, 0x0, 0xA2, 0x9F
};

unc RTFwr::GreekLatinSet[64] = {
 1, 1, 1, 1, 1, 1, 1, 1,   // 0x80
 0, 1, 0, 1, 0, 1, 1, 1,   // 0x88
 1, 1, 1, 1, 1, 1, 1, 1,   // 0x90
 0, 1, 0, 1, 0, 1, 1, 0,   // 0x98
 1, 0, 0, 1, 1, 1, 1, 1,   // 0xA0
 1, 1, 0, 1, 1, 1, 1, 0,   // 0xA8
 1, 1, 1, 1, 0, 1, 1, 1,   // 0xB0
 0, 0, 0, 1, 0, 1, 0, 0    // 0xB8
};

unc RTFwr::EasternEuroLatinSet[128] = {
 1, 1, 1, 0, 1, 1, 1, 1,   // 0x80
 0, 1, 1, 1, 0, 0, 0, 0,   // 0x88
 1, 1, 1, 1, 1, 1, 1, 1,   // 0x90
 0, 1, 1, 1, 0, 0, 0, 0,   // 0x98
 1, 0, 0, 0, 1, 0, 1, 1,   // 0xA0
 1, 1, 0, 1, 1, 1, 1, 0,   // 0xA8
 1, 1, 0, 0, 1, 1, 1, 1,   // 0xB0
 1, 0, 0, 1, 0, 0, 0, 0,   // 0xB8
 0, 1, 1, 0, 1, 0, 0, 1,   // 0xC0
 0, 1, 0, 1, 0, 1, 1, 0,   // 0xC8
 0, 0, 0, 1, 1, 0, 1, 1,   // 0xD0
 0, 0, 1, 0, 1, 1, 0, 1,   // 0xD8
 0, 1, 1, 0, 1, 0, 0, 1,   // 0xE0
 0, 1, 0, 1, 0, 1, 1, 0,   // 0xE8
 0, 0, 0, 1, 1, 0, 1, 1,   // 0xF0
 0, 0, 1, 0, 1, 1, 0, 0    // 0xF8
};

uns RTFwr::EasternEuroSet[128] = {
 0x0, 0x0, 0xC3, 0xE3, 0xA5, 0xB9, 0xC6, 0x0, // 0x100
 0xE6, 0x0, 0x0, 0x0, 0xC8, 0xE8, 0xCF, 0xEF,
 0xD0, 0xF0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, // 0x110
 0xCA, 0xEA, 0xCC, 0xEC, 0x0, 0x0, 0x0, 0x0,
 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, // 0x120
 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, // 0x130
 0x0, 0xC5, 0xE5, 0x0, 0x0, 0xBC, 0xBE, 0x0,
 0x0, 0xA3, 0xB3, 0xD1, 0xF1, 0x0, 0x0, 0xD2, // 0x140
 0xF2, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
 0xD5, 0xF5, 0x0, 0x0, 0xC0, 0xE0, 0x0, 0x0, // 0x150
 0xD8, 0xF8, 0x8C, 0x9C, 0x0, 0x0, 0xAA, 0xBA,
 0x0, 0x0, 0xDE, 0xFE, 0x8D, 0x9D, 0x0, 0x0, // 0x160
 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0xD9, 0xF9,
 0xDB, 0xFB, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, // 0x170
 0x0, 0x8F, 0x9F, 0xAF, 0xBF, 0x8E, 0x9E, 0x0
};

uns RTFwr::EncodingVals[5] = {
 204, 238, 161, 162, 0 
};

char *RTFwr::EncodingSuffixes[5] = { 
 "Cyr", "CE", "Greek", "Tur", NULL
};

unl RTFwr::EncodingCpgs[5] = {
 1251, 1250, 1253, 1254, 0
};

uns
RTFwr::ProcUnicode(RTFwfile *wf, uns wch)
{
	uns nch = 0;
	uns enc = 0;
	unl cpg = 0;

	if (Curr.ftype > 3)  // pi font
		return (wch <= 0xFF) ? wch : 0;
			 
	// convert Unicode chars to charset chars
	// write font call for code page if necessary

#if 0
	if ((nch = GetUnicodeEquiv(wch, Curr.fenc)) != 0)
		return nch;  // valid in current code page
	
	for (int i = 0; (enc = EncodingVals[i]) != 0; i++) {
		if ((nch = GetUnicodeEquiv(wch, enc)) != 0) {
			// compute font number for current ftype and enc
			Curr.fnum = i + ((Curr.ftype - 1) * 4) + 2;
			WriteFontProps(wf);
			return nch;   // in different encoding
		}
	}
#endif

	if ((cpg = Curr.cpg) == 0) {
		cpg = DefaultCodePage;
		Curr.cpg = cpg;
		enc = GetEncodingFromCodePage(cpg);
		Curr.fenc = enc;
	}

	if ((nch = (uns) DCicu::GetUnicodeCpg(wch, cpg)) != 0)
		return nch;  // valid in current code page
	
	if (wch < 0x2E80) {
		for (int i = 0; (cpg = EncodingCpgs[i]) != 0; i++) {
			if ((nch = (uns) DCicu::GetUnicodeCpg(wch, cpg)) != 0) {
				// compute font number for current ftype and enc
				Curr.fnum = i + ((Curr.ftype - 1) * 4) + 2;
				WriteFontProps(wf);
				return nch;   // in different encoding
			}
		}
	}

	return 0;  // not valid in any available encoding
}


uns
RTFwr::GetUnicodeEquiv(uns wch, uns enc)
{
	uns nch = 0;

	switch (enc) {
		case 0:    // ANSI, 1252
			if (wch <= 0xFF)
				nch = wch;
			else if (wch == 0x192)
				nch = 0x83;
			else if (wch == 0x2C6)
				nch = 0x88;
			else if (wch == 0x160)
				nch = 0x8A;
			else if (wch == 0x152)
				nch = 0x8C;
			else if (wch == 0x2DC)
				nch = 0x98;
			else if (wch == 0x161)
				nch = 0x9A;
			else if (wch == 0x153)
				nch = 0x9C;
			else if (wch == 0x178)
				nch = 0x9F;
			break;

		case 204:  // Cyrillic, 1251
			if (wch <= 0xFF) {
				if ((wch < 0xC0)
				 && CyrLatinSet[wch - 0x80])
					nch = wch;
			}
			else if ((wch >= 0x410)
			 && (wch <= 0x44F))
				nch = wch - 0x350;
			else if ((wch >= 0x400)
			 && (wch <= 0x40F))
				nch = CyrLowSet[wch - 0x400];
			else if ((wch >= 0x450)
			 && (wch <= 0x45F))
				nch = CyrHighSet[wch - 0x450];
			else if (wch == 0x490)
				nch = 0xA5;
			else if (wch == 0x491)
				nch = 0xB4;
			else if (wch == 0x2116)
				nch = 0xB9;
			break;

		case 238:  // Eastern European, 1250
			if (wch <= 0xFF) {
				if (EasternEuroLatinSet[wch - 0x80])
					nch = wch;
			}
			else if ((wch >= 0x100)
			 && (wch <= 0x17F))
				nch = EasternEuroSet[wch - 0x100];
			else if (wch == 0x2C7)
				nch = 0xA1;
			else if (wch == 0x2D8)
				nch = 0xA2;
			else if (wch == 0x2DB)
				nch = 0xB2;
			else if (wch == 0x2DD)
				nch = 0xBD;
			else if (wch == 0x2D9)
				nch = 0xFF;
			break;

		case 161:  // Greek, 1253
			if (wch <= 0xFF) {
				if ((wch < 0xC0)
				 && GreekLatinSet[wch - 0x80])
					nch = wch;
			}
			else if ((wch >= 0x388)
			 && (wch <= 0x3CF))
				nch = wch - 0x2D0;
			else if (wch == 0x386)
				nch = 0xA2;
			else if (wch == 0x3F3)
				nch = 0xB4;
			else if (wch == 0x3F4)
				nch = 0xA1;
			else if (wch == 0x2015)
				nch = 0xAF;
			break;

		case 162:  // Turkish, 1254
			if (wch <= 0xFF) {
				if ((wch != 0xD0)
				 && (wch != 0xF0)
				 && (wch != 0xDD)
				 && (wch != 0xFD)
				 && (wch != 0xDE)
				 && (wch != 0xFE))
					nch = wch;
			}
			else if (wch == 0x11E)
				nch = 0xD0;
			else if (wch == 0x11F)
				nch = 0xF0;
			else if (wch == 0x130)
				nch = 0xDD;
			else if (wch == 0x131)
				nch = 0xFD;
			else if (wch == 0x15E)
				nch = 0xDE;
			else if (wch == 0x15F)
				nch = 0xFE;
			break;

		default:
			break;
	}
	return nch;
}
#endif

void
RTFwr::CheckDing(RTFwfile *wf, uns& wch)
{
	char wstr[2];
	wstr[0] = (char) wch;
	wstr[1] = '\0';

	if ((Curr.ding == 31)    // remapped as bitmap
	 && IniFile->Section("BitmapChars")
	 && (IniFile->Find(wch)
		|| IniFile->Find(wstr))) {
		wf->putCS('{');
		wf->putStr(BmcStr);
		wf->putStr(IniFile->StrRight());
		wf->putCS('}');
		LinePos += CharWid;
		SuppressSpace = false;
		wch = 0;
	}
	else if (Curr.ding > 31) // remapped as char
		wch = Curr.ding;
}


void
RTFwr::PutBullet(RTFwfile *wf)
{
	if (Curr.del)
		return;
	if (BulType == 'S')
		wf->putSpec(0x95);
	else {
		wf->putCS('{');
		wf->putStr(BmcStr);
		wf->putStr(BulletStr);
		wf->putCS('}');
	}
	LinePos += (CharWid / 2);
	SuppressSpace = false;
}


RTFctl *RTFwr::LastLineEnd = NULL;
DClist RTFwr::CancelLineEnd;


void
RTFwr::ProcLineEnd(RTFctl *cp)
{
	if (JustPara
	 && (cp->siz() == 1))
		JustLineEndCount++;

	HyperFormEnding = false;  // turn off after line break
	HyperColorEnding = false;
	LastLineEnd = (cp->siz() == 1) ? cp : NULL;
}


void
RTFwr::EndLine(RTFwfile *wf, RTFctl *cp)
{
	if (InHelpTitlePara) {
		if (Curr.help == 0)
			return;
		if ((cp->siz() == 0) // soft return becomes space
		 && Curr.clone
		 && !DiscHyphen
		 && !HardRetSpace
		 && !(HelpCloneCount && (HelpCloneBuf[HelpCloneCount - 1] == ' ')))
			PutHelpCloneBuf(wf, ' ');
	}
	else if (HelpForm
	 && (cp->siz() == 0)  // soft
	 && InTbCell
	 && !Curr.del
	 && !InFootnote) {
		if (ForceTbLineBreaks
	   && !HardRetSpace)
			ForcedBreak = true;
		else if (HardHyphen
	   && HelpCloneCount
	   && (HelpCloneBuf[HelpCloneCount - 1] == '-'))
		wf->putCW("line");  // make line break
	}
	else if (SuppressSpace) {
		SuppressSpace = false;
		return;
	}
	else if (WritingXRVal) {
		//wf->putChar(' ');
		return;
	}
	if (cp->siz() == 1) {  // hard only
		if (HelpForm) {
			if (HelpLineBreak) {
				ProcHelpBreak(wf);
				if (Curr.hyper > 0) {
					EndAnchor(wf);
					Curr.line = 3;  // retain hypertext format
				}
			}
			else if (Curr.help && Curr.clone)
				PutHelpCloneBuf(wf, ' ');
			HardRetSpace = true;
		}
		if (JustLineEndCount) {  // make para break with no spacing
			wf->putCW("par");
			wf->putEnd();
			if (PageBroke) {  // this gets ugly
				SetPardProps(wf);
				TabsChanged = true;
				wf->putCWN("s", GetFinalForm(Curr.snum));
		    WriteParaProps(wf);
				WriteSimpleCharProps(wf);
			  Last = Curr;
				WriteOutTabs(wf);
				PageBroke = false;
			}
			wf->putCWN("fi", 0);
			wf->putCWN("sb", 0);
			if (--JustLineEndCount == 0)  // last one
				wf->putCWN("sa", JustParaEndSpace);
			else
				wf->putCWN("sa", 0);
		}
		else {
			if (!InPara)
				WriteText(wf);
			if (!CancelLineEnd.find((long) cp))
				wf->putCW("line");
		}
		CurrTab->tpos = 0;  // turn off
		LinePos = Curr.lind;
	}
	else if (!Curr.runin)
		LinePos = Curr.lind;
	// soft ret does not move back to start of line for run-ins
	wf->putEnd();
}


void
RTFwr::EndColumn(RTFwfile *wf, RTFctl *cp)
{
	if ((CurrCols > 1)
	 && (cp->siz() > 0)) {
		wf->putCW("sect"); 
		PageBroke = true;
	}
}


void
RTFwr::EndPage(RTFwfile *wf, RTFctl *cp)
{
	if (cp->siz() > 0) {
		wf->putCW("pagebb");
		PageBroke = true;
	}
}


void
RTFwr::AddChar(unc ch)
{
  if ((FixedWid) || (ch > CharMetricMax))
    LinePos += CharWid;
  else {
		if (Curr.caps == 2)
			ch = toupper(ch);
    LinePos += (CharWid * CharMetrics[ch]) / 56;
	}
}


bool RTFwr::PageBroke = false;
char RTFwr::RuninType = 'R'; // Runin or Normal para
long RTFwr::RuninDiv = 0;
long RTFwr::PreRuninDiv = 0;
long RTFwr::RuninPara = 0;
DClist RTFwr::NoRuninPara;
pprop RTFwr::RuninProps;
DCvlist RTFwr::RuninOverrides;
uns RTFwr::RuninLinePos = 0;
tinfo *RTFwr::RuninTab = NULL;
short RTFwr::TabDiff;

void
RTFwr::WriteProps(RTFwfile *wf)
{
	if (Curr.del
	 && !Curr.replace)
		return;

	if (RuninDiv
	 && !Curr.runin) {  // para after runin para, so
		wf->SelectDiv(PreRuninDiv);  // end diversion
		RuninLinePos = LinePos;
	}

	if (!HelpForm
	 && (Curr.hyper > 1)
	 && !EndingAnchor) {
		HyperEnd = true;
		EndAnchor(wf);
		Curr.hyper = (Curr.hyper == 3) ? 1 : 0;
	}

	if (HelpForm) {
		if ((Curr.hide != 0)
		 && (Curr.hide != Last.hide)) {
			wf->putEnd();
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
		}
		else if ((Curr.hide == 0)
		 && (Curr.hide != Last.hide)) {
			wf->putGE();
			Last.hide = Curr.hide;
		}

		if ((Curr.hyper > 1)
		 && !EndingAnchor) {
			EndAnchor(wf);
			Curr.hyper = (Curr.hyper == 3) ? 1 : 0;
		}

		if ((Curr.fnote > 1)
		 && (Curr.fnote != Last.fnote)
		 && (Last.fnote)) {		// changing footnotes
			RestoreState(LastState);
			WriteFtnEnd(wf);
		}
		else if (Curr.fnote == 1) {	 // ending footnote
			RestoreState(LastState);
			Curr.fnote = 0;
			WriteFtnEnd(wf);
		}

		if (Curr.clone != Last.clone) {
			if (Last.clone)  // ending a HelpTag process
				WriteHelpClone(wf, &Last);
			else HelpCloneCount = 0;	 // ready to start diversion
		}
		if ((Curr.hmacro != Last.hmacro)
		 && (Last.hmacro != NULL))
			WriteHelpClone(wf, &Last);

		if ((Curr.fnote > 1)
		 && (Curr.fnote != Last.fnote)) {	 // starting footnote
			SaveState();
			if (Curr.fnote == '$') {
				WriteBrowseID(Curr.hbrowse);
				if (TitleBrowse
				 || AutoBrowse
				 || Curr.hbrowse)
					WriteBrowseFtn(wf);
			}
			FtnMarker = Curr.fnote;
			WriteFtnStart(wf);
			WriteFtnNum(wf);
		}
	}
	if (!HelpForm
	 && (Curr.hide != Last.hide)
	 && (!Curr.hide)) {
		wf->putEnd();
		wf->putCW("v0");
		Last.hide = Curr.hide;
	}

  if ((InPara == false)  // style changes at start of para
   && (InFootnote == false)) {
    if (ParaEnding) {
      wf->putCW("par");
      wf->putEnd();
			if (CodeStoreDiv)  // end any pending div
				EndCodeStore(wf);
      ParaEnding = false;
    }

		if ((!WritingFrame
			|| WritingFrameAsText
			|| InTextFrame)
		 && !StartingTbCell
		 && !Curr.runin
		 && (InHelpTitlePara || !InTbCell)
		 && !WritingFrameAsText
		 && !InTextFrame
		 && !WritingHF
		 && PGStarted) {
				wf->putGE();
				PGStarted = 0;
				wf->putEnd();
		}
			
    if (Curr.pbreak)
      WriteBreak(wf);
		else
			CheckAnchorPara(wf);

		EndingTextFrame = false;

		if (Curr.pxframe   // handle prefix frame before start of para
		 && !StartingHelpTopic
		 && !InPxSxFrame)
			WritePreSufFrame(wf, Curr.pxframe, true);

		if (CodeStoreVar)  // start diversion for macro var
			StartCodeStore(wf);

		if (Curr.runin) {   // runin para starting
			RTFgroup *tcp = NULL;
			if ((tcp = (RTFgroup *) RuninOverrides.find(ParaRefNum)) != NULL)
				Curr.rungroup = tcp;
			if (!RuninDiv) {  // new runin, start diversion
				PreRuninDiv = wf->GetDiv();
				RuninDiv = wf->NewDiv();
				LinePos = Curr.find;
			}
			else { // two runins in a row
				WriteRunin(wf, true);
			}
			RuninProps = Curr;  // store para props
			SetPardProps();
			if (!HelpForm
			 && (Curr.hide != Last.hide)
			 && (Curr.hide)) {
				wf->putEnd();
				wf->putCW("v");
				Last.hide = Curr.hide;
			}
			ParaPropsWritten = true;
			InPara = true;        // write only char props for runin
		}
		else {  // not run-in, normal processing
			if (RuninDiv) {  // normal para after runin starting
				if ((Curr.find > RuninProps.find) // first indent may make gap
				 && (RuninLinePos < Curr.find)) {  // not past it yet, tab
					DCvlist *ntab = new DCvlist;
					tinfo *ti = new tinfo;
					ti->tpos = Curr.find;
					ntab->add(ti, 1);
					RuninTab = ti;
					if (!Curr.tcount) {
						Curr.tcount = 1;
						Curr.tabs = ntab;
					}
					else
						Curr.tcount = MergeTabs(&Curr.tabs, &ntab);
					TabsChanged = true;
				}
					
				Curr.find = RuninProps.find; // adjust para props
				Curr.bspace = RuninProps.bspace;
				if (InheritPending)
					InheritPendForm.runin = 0;
				if (RuninProps.tcount) { // insert tab stops
					if (!Curr.tcount) {
						Curr.tcount = RuninProps.tcount;
						Curr.tabs = RuninProps.tabs;
					}
					else
						Curr.tcount = MergeTabs(&Curr.tabs, &RuninProps.tabs);
					TabsChanged = true;
				}
			}

			if (InCharForm)
				return;

			if ((!WritingFrame
			  || WritingFrameAsText
			  || InTextFrame)
			 && !StartingTbCell) {
				if ((InHelpTitlePara || !InTbCell)
				 && !WritingFrameAsText
				 && !InTextFrame
				 && !WritingHF) {
#if 0
					if (PGStarted) {  // moved before CodeStore
						wf->putGE();
						PGStarted = 0;
						wf->putEnd();
					}
#endif
					if (!InTbTitle) {  // only place where PGStarted
						wf->putGS();
						PGStarted++;
					}
				}
				SetPardProps(wf);
			}
			else if (!WritingFrame
			 || WritingFrameAsText
			 || InTextFrame)
				wf->putCW("plain");
			InParaGroup = true;
			TabsChanged = true;

			if (!HelpForm
			 && (Curr.hide != Last.hide)
			 && (Curr.hide)) {
				wf->putEnd();
				wf->putCW("v");
				Last.hide = Curr.hide;
			}

			if (PageBreakPending && !WritingFrame) {
				wf->putCW("pagebb");
				PageBreakPending = false;
				PageBroke = true;
			}

			if (Curr.snum
			 && (CharStyles || (Curr.stype != 2))
			 && ((Curr.snum != Last.snum)
				|| (!InPara && InParaGroup && (Curr.snum != DefForm.snum))))
				wf->putCWN("s", GetFinalForm(Curr.snum));
		}

		TabDiff = 0;
		if (InTbCell)
			TabDiff = Cell->cellmargl;
		else if (SideHUsed
		 && (SideHType == 'I')) {
			if (InTbTitle)
				TabDiff = Tbl->tbllind;
			else if ((Curr.sideh == 0)
			 && (WritingHF == false)
			 && (SideHPos == 1))
				TabDiff = SideHWide + SideHGap;
		}

	}
	else if (InFootnote
	 && !InPara) {
		SetPardProps(wf);
    wf->putCWN("s", GetFinalForm(Curr.snum));
		if (InTbCell)
			TabDiff = Tbl->tbllind;
	}

  if (StartingFtnCont
	 && FootDiv
	 && (FootType == 'P')) {
		wf->putCW("pard");
		wf->putCW("plain");
	}

  if (InPara == false) {  // paragraph properties
    WriteParaProps(wf);
		//WriteSimpleCharProps(wf);
		InPara = true;
	}
	//else

	if (IndexEntry)
		WriteIndexEntry(wf);

	WriteSimpleCharProps(wf);

	PageBroke = false;

	if (CurrTab->tpos
	 && (TabLine != Curr.line))
		WriteOutTabs(wf);

	if ((Curr.stype != 2)
	 && !BordersShadingDone) {
		WriteBorders(wf, &Curr);
		WriteShading(wf, Curr.shading);
		BordersShadingDone = true;
	}

	WriteHyperUnique(wf);

  Last = Curr;

	if (InheritPending
	 && ParaPropsWritten) {  // handle any postponed char format
		InheritForm(&InheritPendForm);
		WriteSimpleCharProps(wf);
		InheritPending = false;
	  Last = Curr;
	}

	if (Curr.styadd)
		DCmacr::WriteMacro(wf, Curr.styadd);

  if (StartingFtnCont)
    WriteTextFtnNum(wf);

	if (InTextMetafile
	 && (Curr.base == 1))
		EndTextMetafile(wf);

	if (RuninDiv
	 && !Curr.runin)  // para after runin
		WriteRunin(wf, false);

	WriteOutTabs(wf);

	if (HyperStart && !HotDiv) {
		PreHotDiv = wf->GetDiv();
		HotDiv = wf->NewDiv();
		HyperStart = false;
	}
}


void 
RTFwr::WriteRunin(RTFwfile *wf, bool cont)
{
	// cont is true if this para is also runin
	long cdiv = wf->GetDiv(); // if !cont, cdiv == PreRuninDiv
	char *rdivp = NULL;
	long rdlen = 0;
	char *sdivp = NULL;
	bool rdovr = false;
	long xdiv = 0;

	wf->SelectDiv(RuninDiv);  // get contents of existing div
	rdivp = wf->ReadDiv();
	rdlen = wf->LenDiv();  // ends with newline
	if (rdivp[rdlen - 1] == '\n')
		rdlen--;                  // remove newline
	rdovr = ispunct(rdivp[rdlen - 1]) ? true : false;
	sdivp = NewName(rdivp, rdlen);
	wf->DeleteDiv(RuninDiv);
	RuninDiv = 0;

	if (cont)  // continuing runin, start new runin div
		cdiv = wf->NewDiv();
	wf->SelectDiv(cdiv);
	wf->putGS();
	wf->putStr(sdivp);
	InPara = true;
	LinePos = RuninLinePos;
	ParaStarting = false;
	xdiv = wf->NewDiv();
	if (RuninProps.runpunct
	 && *(RuninProps.runpunct))
		wf->putStr(RuninProps.runpunct);
	if (RuninProps.rungroup)
		(RuninProps.rungroup)->write(*wf);
	rdivp = wf->ReadDiv();
	if (rdivp) {  // something written
		rdlen = wf->LenDiv();
		if (*rdivp == '\n')
			rdivp++, rdlen--;
		if (rdovr) {
			while (*rdivp && ispunct(*rdivp))
				rdivp++, rdlen--;
		}
	}
	wf->SelectDiv(cdiv);
	if (cont)
		RuninDiv = cdiv;
	if (rdivp && rdlen)
		wf->putStr(rdivp);
	wf->DeleteDiv(xdiv);
	wf->SelectDiv(cdiv);
	wf->putGE();
	wf->putCr();
	if (RuninTab) {
		CurrTab = RuninTab;
		RuninTab = NULL;
	}
	if (cont && RuninProps.tcount) { // insert tab stops
		if (!Curr.tcount) {
			Curr.tcount = RuninProps.tcount;
			Curr.tabs = RuninProps.tabs;
		}
		else
			Curr.tcount = MergeTabs(&Curr.tabs, &RuninProps.tabs);
		TabsChanged = true;
	}
}


uns
RTFwr::MergeTabs(DCvlist **ctab, DCvlist **rtab)
{
	DCvlist *tlist = new DCvlist;
	long ri = 1;
	long ci = 1;
	long ti = 0;
	tinfo *rinf = (tinfo *) (*rtab)->find(ri);
	tinfo *cinf = (tinfo *) (*ctab)->find(ci);
	tinfo *tinf = NULL;
	short rpos = 0;
	short cpos = 0;

	while (rinf && cinf) {
		tinf = new tinfo;
		rpos = rinf->tpos;
		cpos = cinf->tpos;
		if (rpos == cpos) {
			*tinf = *rinf;     // right side wins
			rinf = (tinfo *) (*rtab)->find(++ri);
			cinf = (tinfo *) (*ctab)->find(++ci);
		}
		else if (rpos < cpos) {
			*tinf = *rinf;
			rinf = (tinfo *) (*rtab)->find(++ri);
		}
		else if (cpos < rpos) {
			*tinf = *cinf;
			cinf = (tinfo *) (*ctab)->find(++ci);
		}
		tlist->add(tinf, ++ti);
	}

	while (rinf) {
		tinf = new tinfo;
		*tinf = *rinf;
		rinf = (tinfo *) (*rtab)->find(++ri);
		tlist->add(tinf, ++ti);
	}

	while (cinf) {
		tinf = new tinfo;
		*tinf = *cinf;
		cinf = (tinfo *) (*ctab)->find(++ci);
		tlist->add(tinf, ++ti);
	}

	*ctab = tlist;
	return (uns) ti;
}

void
RTFwr::WriteOutTabs(RTFwfile *wf)
{
	if (Curr.del)
		return;
  if (CurrTab->tpos   // tabs (not tab defs)
	 && !WritingFtnGroup) {
    if (((CurrTab->tpos + TabDiff) > LinePos)
		 || (CurrTab->ttype == 10)
     || (OccTabType == 'T'))
      WriteTabs(wf);
    else if (OccTabType == 'S')
      wf->putChar(' ');
    CurrTab->tpos = 0;  // turn off
  }
}


void
RTFwr::SetPardProps(RTFwfile *wf)
{
	if (Curr.del)
		return;

	if (wf) {
	  wf->putCW("pard");
		if (InTbCell
		 && (!WritingFrame || InTextFrame)
		 && !InFootnote
		 && !Tbl->tblstrip
		 && !InHelpTitlePara)
			wf->putCW("intbl");
	  wf->putCW("plain");
	}

	BordersShadingDone = false;

	// set Last props to DefForm props
	Last.snum = DefForm.snum;
	Last.snext = DefForm.snext;
	Last.cfnum = DefForm.cfnum;
	Last.fnum = DefForm.fnum;
	Last.fsize = DefForm.fsize;
	Last.fenc = 0;
	Last.fwid = DefForm.fwid;
	Last.bold = DefForm.bold;
	Last.ital = DefForm.ital;
	Last.base = DefForm.base;
	Last.caps = DefForm.caps;
	Last.scaps = DefForm.scaps;
	Last.outl = DefForm.outl;
	Last.shad = DefForm.shad;
	Last.strike = DefForm.strike;
	Last.line = DefForm.line;
	Last.lmode = DefForm.lmode;
	Last.algn = DefForm.algn;
	Last.keep = DefForm.keep;
	Last.keepn = DefForm.keepn;
	Last.lsmode = DefForm.lsmode;
	Last.kern = DefForm.kern;
	Last.lspace = DefForm.lspace;
	Last.aspace = DefForm.aspace;
	Last.bspace = DefForm.bspace;
	Last.lind = DefForm.lind;
	Last.find = DefForm.find;
	Last.rind = DefForm.rind;
	Last.pbreak = DefForm.pbreak;
	Last.lang = DefForm.lang;
	Last.colr = DefForm.colr;
	Last.bcolr = DefForm.bcolr;
	Last.cellmargl = DefForm.cellmargl;
	Last.cellmargr = DefForm.cellmargr;
	Last.cellmargt = DefForm.cellmargt;
	Last.cellmargb = DefForm.cellmargb;
	Last.cellmartl = DefForm.cellmartl;
	Last.cellmartr = DefForm.cellmartr;
	Last.cellmartt = DefForm.cellmartt;
	Last.cellmartb = DefForm.cellmartb;
	Last.cellvalgn = DefForm.cellvalgn;
	Last.tcount = DefForm.tcount;
	Last.pxframe = DefForm.pxframe;
	Last.sxframe = DefForm.sxframe;
}


// very hard to determine whether this para is really
// meant to be part of the current frame, or to be new
bool RTFwr::UseParaAnchors = false;

void
RTFwr::CheckAnchorPara(RTFwfile *wf)
{
	if (Curr.del)
		return;

	if (ParaFrameProps
	 && !ParaPropsWritten
	 && UseParaAnchors
	 && (CurrTextFrame
		|| (FrameHF && WritingHF)
		|| (SideHUsed
		 && ((Curr.sideh > 0)
			|| WritingHF)
		 && (SideHType == 'F')))) {  // will frame the next para too
		SetPardProps(wf);  // put anchoring para
		wf->putCW("par");
		wf->putEnd();
		ParaFrameProps = false;
	}
}


#if 0

void
RTFwr::WriteCharProps(RTFwfile *wf)
{
	if (DeletingText)
		return;

  if (Curr.fnum 
   && ((Curr.fnum != Last.fnum)
    || (!InPara && InParaGroup && (Curr.fnum != DefForm.fnum))))
    wf->putCWN("f", GetFinalFont(Last.fnum = Curr.fnum));

  if (Curr.fsize 
   && ((Curr.fsize != Last.fsize)
    || (!InPara && InParaGroup && (Curr.fsize != DefForm.fsize))))
    wf->putCWN("fs", Last.fsize = Curr.fsize);

  if ((Curr.bold > 1) 
   && ((Curr.bold != Last.bold)
    || (!InPara && InParaGroup && (Curr.bold != DefForm.bold))))
    wf->putCW("b");
  else if ((Curr.bold == 1) 
   && ((Curr.bold < Last.bold)
    || (!InPara && InParaGroup && (Curr.bold < DefForm.bold))))
    wf->putCW("b0");

  if ((Curr.ital > 1) 
   && ((Curr.ital != Last.ital)
    || (!InPara && InParaGroup && (Curr.ital != DefForm.ital))))
    wf->putCW("i");
  else if ((Curr.ital == 1) 
   && ((Curr.ital < Last.ital)
    || (!InPara && InParaGroup && (Curr.ital < DefForm.ital))))
    wf->putCW("i0");

  if (Curr.base 
   && ((Curr.base != Last.base)
    || (!InPara && InParaGroup && (Curr.base != DefForm.base)))) {
    if (Last.base > 1)
      wf->putCWN((Last.base > 2 ? "up" : "dn"), 0);
    if (Curr.base > 1)
      wf->putCWN((Curr.base > 2 ? "up" : "dn"),
                  (Curr.fsize ? Curr.fsize / 3 : 3));
  }

  if ((Curr.caps > 1) 
   && ((Curr.caps != Last.caps)
    || (!InPara && InParaGroup && (Curr.caps != DefForm.caps))))
    wf->putCW("caps");
  else if ((Curr.caps == 1) 
   && ((Curr.caps < Last.caps)
    || (!InPara && InParaGroup && (Curr.caps < DefForm.caps))))
    wf->putCW("caps0");

  if ((Curr.scaps > 1) 
   && ((Curr.scaps != Last.scaps)
    || (!InPara && InParaGroup && (Curr.scaps != DefForm.scaps))))
    wf->putCW("scaps");
  else if ((Curr.scaps == 1) 
   && ((Curr.scaps < Last.scaps)
    || (!InPara && InParaGroup && (Curr.scaps < DefForm.scaps))))
    wf->putCW("scaps0");

  if ((Curr.outl > 1) 
   && ((Curr.outl != Last.outl)
    || (!InPara && InParaGroup && (Curr.outl != DefForm.outl))))
    wf->putCW("outl");
  else if ((Curr.outl == 1) 
   && ((Curr.outl < Last.outl)
    || (!InPara && InParaGroup && (Curr.outl < DefForm.outl))))
    wf->putCW("outl0");

  if ((Curr.shad > 1) 
   && ((Curr.shad != Last.shad)
    || (!InPara && InParaGroup && (Curr.shad != DefForm.shad))))
    wf->putCW("shad");
  else if ((Curr.shad == 1) 
   && ((Curr.shad < Last.shad)
    || (!InPara && InParaGroup && (Curr.shad < DefForm.shad))))
    wf->putCW("shad0");

  if ((Curr.strike > 1) 
   && ((Curr.strike != Last.strike)
    || (!InPara && InParaGroup && (Curr.strike != DefForm.strike))))
    wf->putCW("strike");
  else if ((Curr.strike == 1) 
   && ((Curr.strike < Last.strike)
    || (!InPara && InParaGroup && (Curr.strike < DefForm.strike))))
    wf->putCW("strike0");

  if ((Curr.line > 1) 
   && ((Curr.line != Last.line)
    || (!InPara && InParaGroup && (Curr.line != DefForm.line))))
    wf->putCW(Curr.line > 3 ? "uld" :
             (Curr.line > 2 ? "uldb" :
             (Curr.lmode ? "ul" : "ulw" )));
  else if ((Curr.line == 1) 
   && ((Curr.line < Last.line)
    || (!InPara && InParaGroup && (Curr.line < DefForm.line))))
    wf->putCW("ulnone");

  if ((Curr.kern != 0) 
   && ((Curr.kern != Last.kern)
    || (!InPara && InParaGroup && (Curr.kern != DefForm.kern))))
    wf->putCWN("expnd", Curr.kern);
  else if ((Curr.kern == 0) 
   && ((Curr.kern != Last.kern)
    || (!InPara && InParaGroup && (Curr.kern != DefForm.kern))))
    wf->putCW("expnd0");

	if (HelpForm == true)
		return;

  if ((Curr.colr > 2) && (Curr.colr != Invisible)
   && ((Curr.colr != Last.colr)
    || (!InPara && InParaGroup && (Curr.colr != DefForm.colr))))
    wf->putCWN("cf", Curr.colr);
  else if (((Curr.colr <= 2) || (Curr.colr == Invisible))
   && ((Curr.colr != Last.colr)
    || (!InPara && InParaGroup && (Curr.colr != DefForm.colr))))
    wf->putCW("cf1");
}

#endif

void
RTFwr::WriteCharForm(RTFwfile *wf)
{
	short fnum = 0;

	if (Curr.del) {
		if (CForm.maccr) {
			DCmacr::WriteMacro(wf, CForm.maccr);
			CForm.maccr = NULL;
		}
		return;
	}

	if (EndCharForm) {
		if (InCharForm) {
			// mac end
			if (!(Curr.del || LastCF.del)
			 && LastCF.macce) {
				DCmacr::WriteMacro(wf, LastCF.macce);
				LastCF.macce = NULL;
			}
			wf->putChar(0x7d); // GE, but prevent extra space after it
			// mac after
			if (!(Curr.del || LastCF.del)
			 && LastCF.macca) {
				DCmacr::WriteMacro(wf, LastCF.macca);
				LastCF.macca = NULL;
			}
		}
		EndCharForm = false;
		InCharForm = false;
		Last.cfnum = 0;
	}

	if (StartCharForm) {
		if (InCharForm) {
			// mac end
			if (!(Curr.del || LastCF.del)
			 && LastCF.macce) {
				DCmacr::WriteMacro(wf, LastCF.macce);
				LastCF.macce = NULL;
			}
			wf->putGE();
			// mac after
			if (!(Curr.del || LastCF.del)
			 && LastCF.macca) {
				DCmacr::WriteMacro(wf, LastCF.macca);
				LastCF.macca = NULL;
			}
		}
		if ((Curr.cfnum == 0)
		 || ((fnum = GetFinalForm(Curr.cfnum)) == 0)) {
			if (CharFormSaved) {
				RestoreState(LastState);
				CharFormSaved = false;
			}
		}
		else {
			// mac before
			if (!Curr.del && CForm.maccb) {
				DCmacr::WriteMacro(wf, CForm.maccb);
				CForm.maccb = NULL;
			}
			wf->putGS();
			wf->putCWN("cs", fnum);
			// mac start is handled in WriteText
			Last.cfnum = Curr.cfnum;
			InCharForm = true;
		}
		StartCharForm = false;
	}
}


void
RTFwr::WriteFontProps(RTFwfile *wf)
{
	finfo *finf = NULL;

	if (XML)  //XML
		return;

  if (Curr.fnum 
   && ((Curr.fnum != Last.fnum) || !InPara)) {
    wf->putCWN("f", GetFinalFont(Last.fnum = Curr.fnum));
		if ((finf = (finfo *) FontList.find(Curr.fnum)) != NULL) {
			Curr.ftype = finf->ftype;
			Curr.fenc = finf->fenc;
			Curr.cpg = finf->cpg;
		}
		else {
			Curr.ftype = 1;
			Curr.fenc = 0;
		}
		Last.fnum = Curr.fnum;
		Last.ftype = Curr.ftype;
		Last.fenc = Curr.fenc;
	}
}


void
RTFwr::WriteSimpleCharProps(RTFwfile *wf)
{
	if (Curr.del
	 && !Curr.replace)
		return;

	if (XML)  //XML
		return;

	if (CharStylesUsed)
		WriteCharForm(wf);

	WriteFontProps(wf);

  if ((Curr.bold > 1) 
   && ((Curr.bold != Last.bold) || !InPara))
    wf->putCW("b");
  else if ((Curr.bold == 1) 
   && (Curr.bold < Last.bold)
	 && (ParaForm.bold < 2))
    wf->putCW("b0");

  if ((Curr.ital > 1) 
   && ((Curr.ital != Last.ital) || !InPara))
    wf->putCW("i");
  else if ((Curr.ital == 1) 
   && (Curr.ital < Last.ital)
	 && (ParaForm.ital < 2))
    wf->putCW("i0");

	uns fontsize = Curr.fsize;
	bool fschange = false;

  if ((Curr.base || Last.base)
   && ((Curr.base != Last.base) || !InPara)) {
    if (Last.base > 1)
      wf->putCWN(LastShiftDir, 0);
    if (Curr.base > 2) {
			if (SuperShift >= 0)
				wf->putCWN(LastShiftDir = "up", (short)
				 (Curr.fsize ? ((Curr.fsize * SuperShift) / 10000L) : DefShift));
			else
				wf->putCWN(LastShiftDir = "dn", (short)
				 (Curr.fsize ? ((Curr.fsize * -SuperShift) / 10000L) : DefShift));
			fontsize = (uns) ((fontsize * (long) SuperSize) / 10000L);
		}
    else if (Curr.base > 1) {
			if (SubShift >= 0)
	      wf->putCWN(LastShiftDir = "dn", (short)
				 (Curr.fsize ? ((Curr.fsize * SubShift) / 10000L) : DefShift));
			else
	      wf->putCWN(LastShiftDir = "up", (short)
				 (Curr.fsize ? ((Curr.fsize * -SubShift) / 10000L) : DefShift));
			fontsize = (uns) ((fontsize * (long) SubSize) / 10000L);
		}
		fschange = true;
  }

  if ((Curr.caps == 2) 
   && ((Curr.caps != Last.caps) || !InPara))
    wf->putCW("caps");
  else if ((Curr.caps == 1) 
   && (Curr.caps < Last.caps)
	 && (ParaForm.caps < 2))
    wf->putCW("caps0");

	if (SmallCaps == 'S') {
		if ((Curr.scaps > 1) 
		 && ((Curr.scaps != Last.scaps) || !InPara))
			wf->putCW("scaps");
		else if ((Curr.scaps == 1) 
		 && (Curr.scaps < Last.scaps)
		 && (ParaForm.scaps < 2))
			wf->putCW("scaps0");
	}
	else if (SmallCaps == 'F') {
		if ((Curr.scaps > 1) 
		 && ((Curr.scaps != Last.scaps) || !InPara)) {
			wf->putCW("caps");
			fontsize = (uns) ((fontsize * (long) SCapSize) / 10000L);
		}
		else if ((Curr.scaps == 1) 
		 && (Curr.scaps < Last.scaps)
		 && (ParaForm.scaps < 2))
			wf->putCW("caps0");
		fschange = true;
	}

  if ((fontsize || AllowZeroFSize)
   && (fschange || (fontsize != Last.fsize) || !InPara))
    wf->putCWN("fs", Last.fsize = fontsize);


  if ((Curr.outl > 1) 
   && ((Curr.outl != Last.outl) || !InPara))
    wf->putCW("outl");
  else if ((Curr.outl == 1) 
   && (Curr.outl < Last.outl)
	 && (ParaForm.outl < 2))
    wf->putCW("outl0");

  if ((Curr.shad > 1) 
   && ((Curr.shad != Last.shad) || !InPara))
    wf->putCW("shad");
  else if ((Curr.shad == 1) 
   && (Curr.shad < Last.shad)
	 && (ParaForm.shad < 2))
    wf->putCW("shad0");

  if ((Curr.strike > 1) 
   && ((Curr.strike != Last.strike) || !InPara))
    wf->putCW("strike");
  else if ((Curr.strike == 1) 
   && (Curr.strike < Last.strike)
	 && (ParaForm.strike < 2))
    wf->putCW("strike0");

  if ((Curr.line > 1) 
   && ((Curr.line != Last.line) || !InPara)) {
		if (HelpForm && HyperHelp) {
			if (Last.line < 2) {
				SaveState();
				wf->putGS();
				wf->putCW((Curr.line == 3) ? "uldb" : "ul");
			}
		}
		else 
			wf->putCW(Curr.line == 4 ? "uld" :
			         (Curr.line == 3 ? "uldb" :
			         (Curr.lmode ? "ul" : "ulw" )));
		Last.line = Curr.line;
	}
  else if ((Curr.line == 1) 
   && (Curr.line < Last.line)
	 && (ParaForm.line < 2)) {
		if (HelpForm && HyperHelp) {
			RestoreState(LastState);
			wf->putGE();
		}
		else if (InHelpXref || EndingAnchor)
			Curr.line = Last.line;
		else if (HelpForm)
			wf->putCW((Last.line == 3) ? "uldb0" : "ul0");
		else
			wf->putCW("ulnone");
		Last.line = Curr.line;
	}

	//.short expansion = Curr.kern;

  if ((Curr.kern != 0) 
   && ((Curr.kern != Last.kern) || !InPara))
    wf->putCWN("expndtw", Curr.kern, true);
  else if ((Curr.kern == 0) 
   && (Curr.kern != Last.kern)
	 && (ParaForm.kern < 2))
    wf->putCW("expndtw0");

	if (TextColor == true) {
		if ((Curr.colr > 2) && (Curr.colr != Invisible)
		 && ((Curr.colr != Last.colr) || !InPara))
			wf->putCWN("cf", Curr.colr - 1);
		else if (((Curr.colr <= 2) || (Curr.colr == Invisible))
		 && (Curr.colr != Last.colr)
		 && (ParaForm.colr <= 2))
			wf->putCW("cf1");
	}
	if (BackColor == true) {
		if ((Curr.bcolr > 1) && (Curr.bcolr != Invisible)
		 && (Curr.bcolr != 9)  // white
		 && ((Curr.bcolr != Last.bcolr) || !InPara)) {
			uns bc = Curr.bcolr - 1;
			wf->putCWN("cf", Curr.colr - 1);
			wf->putCW("chshdng0");
			wf->putCWN("chcbpat", bc);
			wf->putCWN("cb", bc);
		}
		else if (((Curr.bcolr < 2) || (Curr.bcolr == 9)
		  || (Curr.bcolr == Invisible))
		 && (Curr.bcolr != Last.bcolr)
		 && (ParaForm.bcolr < 2)) {
			wf->putCW("chshdng0");
			wf->putCW("chcbpat8");
			wf->putCW("cb8");
		}
	}
}


void
RTFwr::WriteParaProps(RTFwfile *wf, frame *fr)
{
	if (Curr.del)
		return;

	if (XML)  //XML
		return;

	if (!ParaPropsWritten)
		ParaFrameProps = false;

	if (HelpForm == true) {
		if (((Curr.hscroll == 0)
		  && !HelpTitleScroll)
		 || (Curr.hscroll == 2)) {	// nonscrolling title
			if (InHelpTitlePara) {
				wf->putCW("keepn");
				NonscrollZone = true;
				if (!HelpTitleSpace) {
					Curr.aspace = 0;
					Curr.bspace = 0;
				}
				if (Curr.algn > 1)  // right or justified (allow center)
					Curr.algn = 0;		// make left
				if (!HelpTitleIndent)
					Curr.lind = Curr.find = Curr.rind = 0;
			}
			else if ((((Curr.xscroll == 0)
			   && ExtendHelpNoScroll)
		    || (Curr.xscroll == 2))
			 && NonscrollZone
			 && Curr.keepn)
				wf->putCW("keepn");
			else NonscrollZone = false;
		}
		else if (((Curr.xscroll == 0)
		  && ExtendHelpNoScroll)
		 || (Curr.xscroll == 2)) { // title can scroll on request
			if (InHelpTitlePara
			 && Curr.keepn) {
				wf->putCW("keepn");
				NonscrollZone = true;
			}
			else if (NonscrollZone
			 && Curr.keepn)
				wf->putCW("keepn");
			else NonscrollZone = false;
		}

		if (InTbCell
		 && (TbRuleType != 'N')
		 && !InFootnote)
			WriteHelpCellBorder(wf);
	}
	else {
		if (Curr.keep)
			wf->putCW("keep");
		if (Curr.pbreak) {
			switch (Curr.pbreak) {
				case 4:
				case 5:
				case 6:
					wf->putCW("pagebb");
					break;
				default:
					break;
			}
		}
		if (Curr.pxbreak) {
			switch (Curr.pxbreak) {
				case 4:
				case 5:
				case 6:
					wf->putCW("pagebb");
					break;
				default:
					break;
			}
		}
		if (Curr.keepn
		 || (FramePendList && !WritingFrame)
		 || (Curr.sxframe && RefFrameFormat)
		 || (InTbTitle && TitleInRow)
		 || (SideHUsed && (SideHType == 'F') && Curr.sideh))
			wf->putCW("keepn");
		if (InTbCell
		 && (TbRuleType != 'C')
		 && (TbRuleType != 'N')
		 && !InFootnote)
			WriteHelpCellBorder(wf);
	}

	if ((SideHUsed == true)
	 && (SideHType == 'L')
	 && (Curr.sideh > 1))   // in side head col
		Curr.algn = 0;
  if ((Curr.algn != Last.algn)
   || (InParaGroup && (Curr.algn != DefForm.algn)))
    wf->putCW(ParAlgns[Curr.algn]);

	WriteParaFrameProps(wf);

	PutParaSpace(wf);

	PutLineSpace(wf);

  LinePos = Curr.find;
	PutParaIndent(wf, fr);

  if (TabsChanged && Curr.tcount
	 && ((WritingFrame == false) || InTextFrame))
    WriteTabDefs(wf, &Curr);
  TabsChanged = false;
	ParaPropsWritten = true;
}


void
RTFwr::WriteParaFrameProps(RTFwfile *wf)
{
	if (InTbCell
	 || InTbl
	 || InTextFrame
	 || WordPerf)
		return;

	if (InTbTitle  // table cannot be in frame, so title should not
	 && !FrameTbTitle)
		return;

	if (XML)  //XML
		return;
	if (XMLSource)
		return;

	unc SideH = InTbTitle ? AnchorProp.sideh : Curr.sideh;

	if (CurrTextFrame) {
		if (!WrapTFs)
			wf->putCW("nowrap");
		wf->putCW("phpg");
		wf->putCWN("posnegx", CurrTextFrame->xpos, true);
		wf->putCWN("absw", CurrTextFrame->wide);
		wf->putCW("pvpg");
		wf->putCWN("posnegy", CurrTextFrame->ypos, true);
		ParaFrameProps = true;
	}
	else if (FrameHF && WritingHF) {
		if (!WrapHTFs)
			wf->putCW("nowrap");
		wf->putCW(HFHRef);
		wf->putCWN("posnegx", HFHPos, true);
		wf->putCWN("absw", HFWide);
		wf->putCW("pvpg");
		wf->putCWN("posy", HFVPos);
		if (HFGap > 0) {
			wf->putCWN("dxfrtext", HFGap);
			wf->putCWN("dfrmtxtx", HFGap);
			wf->putCWN("dfrmtxty", HFGap);
		}
		ParaFrameProps = true;
	}
	else if (SideHUsed
	 && ((SideH > 0) || WritingHF)
	 && (SideHType == 'F')) {  // frame the para
		wf->putCW("nowrap");
		if ((SideH == 1)         // spanner
		 || InTbTitle) {
			wf->putCW("phcol");
			if (!InTbTitle)
				wf->putCWN("posnegx", LMarg - TextLMarg, true);
			wf->putCWN("absw", BodyWide);
			if (Curr.aspace > 20)
				Curr.aspace -= 20;
		}
		else if (WritingHF)	{
			wf->putCW("phcol");
			wf->putCWN("posnegx", HFHPos, true);
			wf->putCWN("absw", HFWide);
			if (Curr.aspace > 20)
				Curr.aspace -= 20;
		}
		else {
			wf->putCW("pvpara");
			wf->putCWN("posnegy", SHVAdjust, true);  // adjust vert pos here
			wf->putCW("phpg");
			wf->putCWN("absw", SideHWide);
			if (SideHGap > 0) {
				wf->putCWN("dxfrtext", SideHGap);
				wf->putCWN("dfrmtxtx", SideHGap);
				wf->putCWN("dfrmtxty", 0);  // always 0
			}
		}
		ParaFrameProps = true;
	}
}


bool RTFwr::ExactLSUsed = false;
bool RTFwr::ExactLSpace = false;
short RTFwr::CurrLineSpace = 0;
short RTFwr::LastLineSpace = 0;
short RTFwr::NegLineSpace = 0;


void
RTFwr::CalcLineSpace()
{
  short val = 0;
	ExactLSpace = false;

  switch (Curr.lsmode) {
    case 1:  // fixed
			ExactLSpace = true;
			// fall through
    case 2:  // fixed, allow
    default:
      val = Curr.lspace;
      break;
    case 3:  // add
			ExactLSpace = true;
			// fall through
    case 4:  // add, allow
      val = Curr.lspace + (ParaForm.fsize * 10);
      break;
    case 5:  // prop
      val = (Curr.lspace * ParaForm.fsize) / 100;
      break;
  }

	if (HelpForm
	 || (ExactLSUsed == false))
		ExactLSpace = false;


	if (val < 0) {
		NegLineSpace = -val;
		CurrLineSpace = 0;  // best we can do is zero it if neg number ****
	}
	else {
		NegLineSpace = 0;
		CurrLineSpace = val;
	}
}


void
RTFwr::PutLineSpace(RTFwfile *wf)
{
	if (WritingFrame
	 && !InTextFrame)	 // graphics frames
		return;

	if (XML)  //XML
		return;

	if (WritingHFGraph == true) {  // fixed 10pt frame
		wf->putCWN("sl", -200, true);
		return;
	}

	CalcLineSpace();

	if (NegLineSpace > 40) {  // negative leading more than 2pt
		if ((NegLead != 'F')       // option not selected
		 || (FrameHF && WritingHF) // already using a frame
		 || (SideHUsed
		  && ((Curr.sideh > 0) || WritingHF)
		  && (SideHType == 'F'))) {
			// do nothing with frames  
		}
		else {        // frame the para and move it up by val
			wf->putCW("phcol");
			if (Curr.algn
			 && (Curr.algn < 3))
				wf->putCW(Curr.algn == 1 ? "posxc" : "posxr");
			wf->putCW("pvpara");
			wf->putCWN("posnegy", - (NegLineSpace - (Curr.fsize * 10)), true);
			// put out enough space not to clip type
			CurrLineSpace = Curr.fsize * 10;
			AnchorPara = true;
			ParaFrameProps = true;
		}
	}

	wf->putCWN("sl", ExactLSpace ? -CurrLineSpace : CurrLineSpace, true);
	LastLineSpace = CurrLineSpace;
}


void
RTFwr::PutParaSpace(RTFwfile *wf)
{
	short sval;
	frame *fr = NULL;
	short tspace = 0;
	short bsp = 0;

	if (XML)  //XML
		return;

	if (BodyParaSpace
	 && ((!SideHUsed)
	  || (Curr.sideh < 2)
	  || (SideHType != 'F'))) {  // not a framed sidehead para
		ParaSpace = BodyParaSpace;
		BodyParaSpace = 0;
	}

	if (SideHUsed
	 && (SideHType == 'F')
	 && ((sval = (short) SideHSpaceList.find(ParaRefNum)) > 0)
	 && (sval > Curr.aspace))
			Curr.aspace = sval;

	if (JustLineEndCount) {
		JustParaEndSpace = Curr.aspace;
		Curr.aspace = 0;
	}

	if (SideHUsed
	 && (Curr.sideh > 1)
	 && (SideHType == 'F')) {  // framed sidehead para
    wf->putCWN("sb", 0);     // space before already given in prev para
		if (Curr.aspace == 0)    // put space after always
			Curr.aspace = 20;
		if (!Curr.sxframe)
			wf->putCWN("sa", Curr.aspace);
	}
	else if (SideHAnchor == true) {
    wf->putCWN("sb", 0);
		if (Curr.aspace == 0)    // put space after always
			Curr.aspace = 20;
		if (!Curr.sxframe)
			wf->putCWN("sa", Curr.aspace);
	}
	else if (PSpcType == 'F') {	// reduce space before to give max
		// for Frame, use max of space after and before, not sum
		if (InHelpTitlePara)
			bsp = Curr.bspace;
		else if (FrameTop)
			FrameTop = false;
		else if (FirstTop) {
			if (!CurrTextFrame
			 && ((fr = (frame *) FrameList.find(FlowFrameNum)) != NULL)
			 && (fr->ypos > TMarg)
			 && (fr->ypos > FirstHBottom)
			 &&	((tspace = fr->ypos - FirstHBottom) > LastLineSpace))
				bsp = tspace - LastLineSpace;
			if (bsp > 1440)
				bsp = 0;
			FirstTop = false;
		}
		else if (StartingTbCell) {
			bsp = Cell->cellmargt;
			StartingTbCell = false;
		}
		else if (InTbCell && TbRuleAdjust)  // prevent gaps in rules
			bsp = 0;
		else if (Curr.bspace > ParaSpace)
			bsp = Curr.bspace - ParaSpace;
		else bsp = 0;	// space already at max

		if ((NegLineSpace != 0)
		 && (bsp != 0)) {
			if (NegLineSpace > bsp) {
				NegLineSpace -= bsp;
				bsp = 0;
			}
			else {
				bsp -= NegLineSpace;
				NegLineSpace = 0;
			}
		}

		if (PageBroke
		 || (InHelpTitlePara
		  && !HelpTitleSpace))
			bsp = 0;

    wf->putCWN("sb", bsp);

		if (EndingTbCell) {
			wf->putCWN("sa", Cell->cellmargb);
			EndingTbCell = false;
		}
		else if (InTbCell && TbRuleAdjust)  // prevent gaps in rules
			wf->putCWN("sa", 0);
		else if ((!Curr.sxframe)
		 || (RefFrameFormat == NULL))
			wf->putCWN("sa", Curr.aspace);  // put regardless of last
	}
	else {
		if (InHelpTitlePara)
			bsp = Curr.bspace;
		else if (FrameTop)
			FrameTop = false;
		else if (FirstTop) {
			if (!CurrTextFrame) {
				if (((fr = (frame *) FrameList.find(FlowFrameNum)) != NULL)
				 && (fr->ypos > TMarg)
				 && (fr->ypos > FirstHBottom))
					tspace = fr->ypos - TMarg;
				if (tspace > LastLineSpace)
					bsp = tspace - LastLineSpace;
			}
			if (bsp > 1440)
				bsp = 0;
			FirstTop = false;
		}
		else
			bsp = Curr.bspace;

		if ((NegLineSpace != 0)
		 && (bsp != 0)) {
			if (NegLineSpace > bsp) {
				NegLineSpace -= bsp;
				bsp = 0;
			}
			else {
				bsp -= NegLineSpace;
				NegLineSpace = 0;
			}
		}

		if (PageBroke
		 || (InHelpTitlePara
		  && !HelpTitleSpace))
			bsp = 0;

		if (StartingTbCell) {
			bsp = Cell->cellmargt;
			StartingTbCell = false;
		}

		if ((bsp != Last.bspace)
		 || (InParaGroup && (bsp != DefForm.bspace)))
			wf->putCWN("sb", bsp);

		if (EndingTbCell) {
			wf->putCWN("sa", Cell->cellmargb);
			EndingTbCell = false;
		}
		else if ((!Curr.sxframe)
		 && ((Curr.aspace != Last.aspace)
		  || (InParaGroup
		   && (Curr.aspace != DefForm.aspace))))
			wf->putCWN("sa", Curr.aspace);
	}

	ParaSpace = Curr.aspace; // record space after para
	if ((!SideHUsed)
	 || (Curr.sideh < 2)
	 || (SideHType != 'F')) // not a framed sidehead para
		BodyParaSpace = ParaSpace;  // space w/o sideheads
}


void
RTFwr::PutParaIndent(RTFwfile *wf, frame *fr)
{
	if (WritingHFGraph)
		return;

	if (XML)  //XML
		return;

	unc SideH = InTbTitle ? AnchorProp.sideh : Curr.sideh;

	short Lind = 0;
	short Rind = 0;

	if (InTbCell
	 && !InFootnote) {
    wf->putCWN("li", Curr.lind + Cell->cellmargl);
		wf->putCWN("fi", Curr.find - Curr.lind, true);
    wf->putCWN("ri", Curr.rind + Cell->cellmargr);
		return;
	}

	short ind_l = Curr.lind;
	short ind_f = Curr.find;
	short ind_r = Curr.rind;

	if ((SideHUsed == true)
	 && (SideHType == 'I')
	 && (WritingHF == false)
	 && (InTbTitle == false)
	 && (InTbCell == false)) {
		if (SideH > 1) {   // in side head col
			Rind = SideHRind;
			Lind = SideHLind;
		}
		else if (SideH == 0) { // in text col
			Rind = TextRind;
			Lind = TextLind;
		}
	}

	if ((SideHUsed == true)
	 && (SideHPos == 1)
	 && (InTbTitle == true)
	 && (SideH <= 1)) {   // in spanner or text col
		if (SideHType == 'L') {
			short lside = SideHWide + SideHGap;
			if (ind_l >= lside)
				ind_l -= lside;
			else ind_l = 0;
			if (ind_f >= lside)
				ind_f -= lside;
			else ind_f = 0;
		}
		else if (SideHType == 'I') {
			ind_l += Tbl->tbllind;
			ind_f += Tbl->tbllind;
		}
	}

	if ((fr != NULL)	 // writing frame
	 && (fr->fwrap != 1)
	 && (fr->pxpos != 0)
	 && !FrameStyleUsed) {
		short left = LMarg;
		if ((SideHUsed == true)
		 && (SideHType != 'L') 
		 && (InTbCell == false)) {
			if ((SideH > 1)    // in side head col
			 && (SideHPos == 2))    // sideheads on right
				left += BodyWide - SideHWide;
			else if ((SideH <= 1)  // in text col or spanner
			 && (SideHType == 'F') 
			 && (SideHPos == 1))        // sideheads on left
				left += SideHWide + SideHGap;
		}
		if ((DefCols > 1)
		 && (fr->href > 1)
		 && (SideH == 0)) {
			ind_l = 0;
		}
		else if ((fr->href > 1) // column reference
		 && ((fr->halgn == 2) // centered
		  || (fr->halgn == 3)) // or right
		 && (SideH == 0)) {  // in text col
			if ((SideHUsed == true)
			 && (SideHType == 'I')) {
				ind_l = Lind;
				if ((ind_l + fr->wide) > (BodyWide + 360))
					ind_l = BodyWide - fr->wide;
				if (fr->halgn == 2)
					ind_l /= 2;
			}
			else
				ind_l = 0;
		}
		else
			ind_l = fr->pxpos - left;
		if (ind_l < 0)
			ind_l = 0;
		ind_f = ind_l;
		Lind = 0;
	}

	// add table indent to table footnote indent
	if (InTbCell
	 && InFootnote) {
		ind_l += Tbl->tbllind;
		ind_f += Tbl->tbllind;
	}

  if (((ind_l + Lind) != 0)
	 || (ind_l != Last.lind))
    wf->putCWN("li", ind_l + Lind);
  if (((ind_f - ind_l) != 0)
	 || (ind_f != Last.find)
   || (ind_l != Last.lind))
    wf->putCWN("fi", ind_f - ind_l, true);
  if (((ind_r + Rind) != 0)
	 || (ind_r != Last.rind))
    wf->putCWN("ri", ind_r + Rind);

	LinePos = ind_f;
}


uns RTFwr::LinePos;
uns RTFwr::CharWid;
char *RTFwr::TabTypes[TabTypeMax] = { "tqc", "tqr", "tqdec" };
bool RTFwr::SuppressSpace = false;
bool RTFwr::FirstTabSet = false;
bool RTFwr::TrailingTabs = false;


void
RTFwr::WriteTabDefs(RTFwfile *wf, pprop *style)
{
  tinfo *tab;
	uns count = style->tcount;

	if (XML)  //XML
		return;

	if (InTbCell
	 && !InFootnote) // write left tab at 0 in tables
	   wf->putCW("tx0");

	if (FirstTabSet
	 && !WritingStyles) {  // to handle bizarre Help Compiler bug
		if (count > 30)      // and prevent crash with "corrupted heap"
			count = 30;
		FirstTabSet = false;
	}

  for (uns i = 1; i <= count; i++) {
    tab = (tinfo *) (style->tabs)->find(i);
    assert(tab);
    if (tab->ttype
		 && (tab->ttype < 9)
		 && !InHelpTitlePara)
      wf->putCW(TabTypes[tab->ttype - 1]);
    if (tab->tlead == '.')
      wf->putCW("tldot");
    else if (tab->tlead == '-')
      wf->putCW("tlhyph");
    else if (tab->tlead == '_')
      wf->putCW("tlul");
    else if (tab->tlead == '~')
      wf->putCW("tlth");
    else if (tab->tlead == '=')
      wf->putCW("tleq");
		if ((tab->ttype == 2)    // right tab past right margin
		 && (tab->tpos > BodyWide))
			tab->tpos = BodyWide;
    wf->putCWN((tab->ttype == 9) ? "tb" : "tx", tab->tpos + TabDiff, true);
  }
}


void
RTFwr::WriteTabs(RTFwfile *wf)
{
  tinfo *tab;
  bool started = false;

	//if (Template)  //XML
	//	return;

	if (SuppressSpace
	 || (HelpTitlePending && ParaStarting)
	 || WhiteAnumVal
	 || (Curr.tcount == 0))	{
		CurrTab->tpos = 0;
		return;
	}

	if (CurrTab->ttype == 10) {   // indexed tab, get from settings
		if (CurrTab->tpos > Curr.tcount)
			CurrTab->tpos = Curr.tcount;
    CurrTab = (tinfo *) Curr.tabs->find(CurrTab->tpos);
	}

	if ((CurrTab->ttype == 0)    // left tab near right margin is return
	 && (LTabRMarg == true)
	 && ((CurrTab->tpos + 240) > BodyWide)) {
		wf->putCW("line");
		CurrTab->tpos = 0;  // turn off
		wf->putEnd();
		SuppressSpace = true;
		LinePos = Curr.lind;
		return;
	}

	short tdiff = TabDiff;
	if (SideHUsed
	 && (SideHType == 'F')
	 && (SideHPos == 1)
	 && (Curr.sideh == 0) // in column only
	 && (!InTbCell)
	 && (!InTbTitle)
	 && (!WritingHF))
		tdiff = SideHWide + SideHGap;

	if ((CurrTab->ttype == 2)    // right tab past right margin
	 && ((CurrTab->tpos + tdiff) > BodyWide)) {
		if (RTabRMarg == true)
			CurrTab->tpos = BodyWide - tdiff;
		else {
			CurrTab->tpos = 0;  // turn off
			return;  // ignore it, Frame does
		}
	}

	if (CurrTab->tpos == 0)
		return;

	if ((Last.line > 1)
	 && (UnderlineTabs == false)
	 && (OutType != 'H')) {  // Frame doesn't underline tabs
    wf->putCW("ulnone");
		Last.line = 1;
	}

  // get from LinePos to CurrTab->tpos in Curr.tabs
  for (uns i = 1; i <= Curr.tcount; i++) {
    tab = (tinfo *) Curr.tabs->find(i);
    assert(tab);
    if (tab->tpos > CurrTab->tpos)
      break;
    if ((tab->tpos + TabDiff) > LinePos) {
      wf->putCW("tab");
      LinePos = tab->tpos + TabDiff;
      TabAlign = tab->ttype;
      started = true;
    }
  }

	if (!started) {
		if ((WritingAnum && Curr.tcount)
			|| (OccTabType == 'T')) {
			wf->putCW("tab");
			LinePos = CurrTab->tpos + TabDiff;
			TabAlign = CurrTab->ttype;
		}
		else if (CurrTab->ttype == 10)  // indexed tab
			wf->putText(' ');
	}

	if (Curr.clone)
		PutHelpCloneBuf(wf, ' ');
	CurrTab->tpos = 0;
	CurrTab->ttype = 0;
}


bool RTFwr::PageBreakPending = false;

void
RTFwr::WriteBreak(RTFwfile *wf)
{
	if ((OutType != 'H')  // ignore coded breaks in help
   && !StartingPage
   && !StartingPageBody
	 && !InTbCell) {
    switch (Curr.pbreak) {
      case 3:  // column
				if ((CurrTextFrame == NULL)
				 && (EndingTextFrame == false))
					wf->putCW("column");
        break;
      case 4:  // page
      case 5:  // left page
      case 6:  // right page
				if (PBMethod == 'P')
					PageBreakPending = true;
				else if (PBMethod == 'C')
					wf->putCW("page");
        break;
      default:
        return;
    }
    wf->putEnd();
    StartingPage = true;
    StartingPageBody = true;
		ParaSpace = 0;
  }
  Curr.pbreak = 0;
}


bool RTFwr::InPxSxFrame = false;
DCnlist RTFwr::ExtMetaList;

void
RTFwr::WritePreSufFrame(RTFwfile* wf, uns framenum, bool start)
{
	// get frame name	from FrameNameList
	char* frname = (char *) FrameNameList.find(framenum);
	if (frname == NULL)
		return;

	// set frame format name to default in .ini, or NULL
	char* frformname = RefFrameFormat;

	// look up format name for specific frame name in .ini
	if (IniFile
	 && IniFile->Section("RefFrameFormats")) {	// read frame format info
		if (IniFile->Find(frname)) {
			if (((frformname = IniFile->StrRight()) == NULL)
			 || !stricmp(frformname, "Graphic"))
				frformname = "";
			else if (!stricmp(frformname, "None"))
				frformname = NULL;
		}
	}

	// if format name still NULL, don't put anything out
	if (frformname == NULL)
		return;

	short rcrop = 0;
	short startpos = TextLMarg;

	if (*frformname == '\0') {  // empty name, use frame itself
		frame* fr = (frame *) FrameList.find(framenum);
		if (fr == NULL)
			return;
		if ((fr->grlist == NULL)
		 || (fr->grcount == 0)) {
			if (UseGrPreviews  // preview, give it a chance anyway
			 && fr->grfile)
				WriteFrameCont(wf, fr, 0);
			return;
		}

		graph *gr = NULL;
		frame *gfr = NULL;

		if ((fr->grcount == 1)   // single item in frame
		 && ((gr = (graph *) ((fr->grlist)->find(1))) != NULL)
		 && (gr->gtype == 0)     // the item is a frame itself
		 && ((gfr = (frame *) gr->gparm) != NULL)
		 && (gfr->ftext != 0)    // the only frame is a text frame
		 && (GrTxType != 'E')) {
			WriteFrameCont(wf, fr, 0);
			return;
		}

		// put out para with height of frame, space before of para
		if (!InTbCell) {
			SetPardProps(wf);
			if (WritingHF)
				WriteParaFrameProps(wf); // ensure same frame props in h/f
		}
		else
			wf->putCW("plain");

    if (start && PageBreakPending) {
      wf->putCW("pagebb");
			PageBreakPending = false;
			PageBroke = true;
		}

		if ((SideHUsed == true)
		 && (SideHType == 'I')
		 && (Curr.sideh == 0)
		 && (!WritingHF)
		 && (TextLind > 0)
		 && (!InTbTitle)
		 && (!InTbCell)) {
	    wf->putCWN("li", TextLind);
			startpos += TextLind;
		}

		if (!InTbCell) {
			wf->putCWN("sl", fr->high); // formerly -(fr->high)
			Last.lspace = fr->high;
		}
		if (start == true) {
			if (HelpForm == false)
 				wf->putCW("keepn");
			if (BodyParaSpace > Curr.bspace)  // compensate for sa of
				Curr.bspace = BodyParaSpace;    //  prev para in main flow
			wf->putCWN("sb", Curr.bspace);
			Curr.bspace = 0;  // suppress for next para
		}
		else { // space after not rendered before if sxframe
			wf->putCWN("sa", Curr.aspace);
			if ((Curr.aspace > BodyParaSpace)
			 && ((!SideHUsed)
			  || (Curr.sideh < 2)
			  || (SideHType != 'F')))
				BodyParaSpace = Curr.aspace;
		}
 
		if ((SideHUsed == true)
		 && (SideHType == 'F')
		 && (!WritingHF)
		 && (!InTbCell)) {
			if (Curr.sideh == 1) {  //  1 = spanner
				wf->putCW("phcol");
				startpos = LMarg;
				wf->putCWN("posnegx", LMarg - TextLMarg, true);
				wf->putCWN("absw", PaperWide - Margins);
			}
			else if (Curr.sideh > 1) {  // 2+ = sidehead
				wf->putCW("pvpara");
				wf->putCW("phpg");
				startpos = LMarg;
				wf->putCWN("posnegy", 0, true);
				wf->putCWN("absw", SideHWide);
				if (SideHGap > 0) {
					wf->putCWN("dxfrtext", SideHGap);
					wf->putCWN("dfrmtxtx", SideHGap);
					wf->putCWN("dfrmtxty", 0);  // always 0
				}
			}
			wf->putCW("nowrap");
		}

		char *chp = NULL;
		if ((MetaType == 'E')  // external, and already created
		 && ((chp = (char *) ExtMetaList.find(framenum)) != NULL)
		 && (*chp != '\0')) {
			if (OutType == 'H') {
				wf->putCS('{');
				wf->putStr(BmlStr);
				wf->putStr(chp);
				wf->putCS('}');
			}
			else
				wf->putField("INCLUDEPICTURE", chp, true, "\\\\d ");
 		}
		else {
			// compute cropping required to fit before right margin
			if (!WritingHF) {
				if (SideHUsed
				 && (Curr.sideh > 1)
				 && (fr->wide > SideHWide))
					rcrop = fr->wide - SideHWide;
				else if ((startpos + fr->wide) > (PaperWide - RMarg))
					rcrop = (startpos + fr->wide) - (PaperWide - RMarg);
			}
			WriteFrameCont(wf, fr, rcrop);
			if (MetaType == 'E')
				ExtMetaList.addnew(wf->getMetaName(), framenum);
		}
		// end the para
		wf->putCW("par");
		return;
	}

	// find format with that name in FormNameList
	long formnum = FormNameList(frformname);

	// save Curr
	SaveState();
	InPxSxFrame = true;

	// write out frame name as para using format found
	RTFgroup* gp = new RTFgroup;
	RTFctl* cp = new RTFctl(imnem, 2, 11, 1, 1);
	cp->rp = new rtf;
	cp->rp->write_op = parstart;
	gp->add(*cp);
	cp = new RTFctl(imnem, 2, 82, 1, FormRefList.find(formnum));
	cp->rp = new rtf;
	cp->rp->write_op = formcall;
	gp->add(*cp);
	cp = new RTFctl(etext, 2, 10, 1);
	cp->ex(frname, strlen(frname) + 1);
	cp->rp = new rtf;
	cp->rp->write_op = puttx;
	gp->add(*cp);
	cp = new RTFctl(imnem, 2, 11, 10);
	cp->rp = new rtf;
	cp->rp->write_op = linend;
	gp->add(*cp);
	cp = new RTFctl(imnem, 2, 11, 2);
	cp->rp = new rtf;
	cp->rp->write_op = parend;
	gp->add(*cp);
	gp->write(*wf);
	delete gp;

	// restore Curr
	InPxSxFrame = false;
	RestoreState(CurrState);
	if (!InTbCell) {
		wf->putGE();
		wf->putEnd();
	}
}


// Text Store

bool RTFwr::StoreText = false;
bool RTFwr::TextStoreHere = false;
bool RTFwr::ConfigHere = false;
char *RTFwr::TextStoreName = NULL;
unc RTFwr::TextStoreBuf[TextStoreBufMax];
long RTFwr::TextStoreBufCnt = 0;
long RTFwr::TextStoreCount = 0;
DCnlist RTFwr::TextStoreNames;
DCnlist RTFwr::TextStoreVals;

void 
RTFwr::PutTextStoreBuf(unc ch)
{
	if ((ch == ' ')
	 && ((TextStoreBufCnt == 0)
	  || (TextStoreBuf[TextStoreBufCnt - 1] == ' ')))
		return;

	if (TextStoreBufCnt < TextStoreBufMax)
		TextStoreBuf[TextStoreBufCnt++] = ch;
}

void
RTFwr::PutTextStoreCS(unc ch)
{
	PutTextStoreBuf('\\'); 
	PutTextStoreBuf(ch); 
}

void
RTFwr::PutTextStoreHex(unc ch)
{
	ch = (ch <= 9) ? (ch + '0') : ((ch - 10) + 'A'); 
	PutTextStoreBuf(ch); 
}

void
RTFwr::PutTextStoreSpec(unc ch)
{
	PutTextStoreCS('\''); 
	PutTextStoreHex((ch >> 4) & 0xF); 
	PutTextStoreHex(ch & 0xF); 
}

void
RTFwr::ProcTextStoreText(RTFctl *cp) 
{
	uns wch = 0;
	unl i = 0;
	unc *cpt = NULL;
	unc ch = 0;

  if (cp->dt() == imnem) {  // single char form
		if ((cp->siz() > 0x10000) // sign extended char
		 || (((cp->siz() >> 8) & 0xFF) == 0xFF))
			wch = (uns) (cp->siz() & 0xFF);
		else
			wch = (uns) cp->siz();

    switch (wch) {
			case 0:      // replaced by bitmap
				return;

      case 0xA0:
      case 0x2002:
      case 0x2003:
      case 0x2007:
      case 0x2009: // fixed spaces
        PutTextStoreCS('~');
        break;
      case 0xAD:  // disc hyphen
        PutTextStoreCS('-');
        break;
      case 0x2011:
				PutTextStoreCS('_');
        break;
      case 0x2044:
        PutTextStoreBuf('/');
        break;

			case 130:
			case 145:
			case 146:
				PutTextStoreBuf('\'');
				break;

			case 132:
			case 147:
			case 148:
				PutTextStoreBuf('"');
				break;

			case 150:
			case 151:
				PutTextStoreBuf('-');
				break;

      case '\\':
      case 0x7B:
      case 0x7D:
        PutTextStoreCS((unc) wch);
        break;

      default:
        if (wch > 0xFF)  // ignore other syms for now
					return;
        if (wch > 0x7E)
          PutTextStoreSpec((unc) wch);
        else
          PutTextStoreBuf((unc) wch);
        break;
    }
    return;
  }

  // string has only plain text
  for (i = cp->siz(), cpt = (unc *) cp->ex(); i && *cpt; i--, cpt++) {
		ch = *cpt;

    switch (ch) {
			case 130:
			case 145:
			case 146:
				PutTextStoreBuf('\'');
				break;

			case 132:
			case 147:
			case 148:
				PutTextStoreBuf('"');
        break;

			case 150:
			case 151:
				PutTextStoreBuf('-');
				break;

      case '\\':
      case 0x7B:
      case 0x7D:
        PutTextStoreCS(ch);
        break;

      default:
				PutTextStoreBuf(ch);
        break;
    }
  }
}


char *
RTFwr::GetTextStoreBuf(bool cfg)
{
	if (TextStoreBufCnt == 0)
		return NULL;

	while (TextStoreBufCnt > 1) {
		if (TextStoreBuf[TextStoreBufCnt - 1] == ' ')
			TextStoreBufCnt--;
		else
			break;
	}

	char *cpt = NewName((char *) TextStoreBuf, TextStoreBufCnt, cfg);
	return cpt;
}

char *
RTFwr::GetTextStore(char *vp)
{
	long pos = 0;
	long lastpos = 0;
	
	DCnlist *lp = &TextStoreNames;

	while ((pos = lp->getitem(vp, &lp)) != 0) {
		lastpos = pos;
		if ((lp = lp->next) == NULL)
			break;
	}
	if (lastpos)
		return NewName(TextStoreVals.find(lastpos));
	return NULL;
}


// Para start and end

RTFctl *RTFwr::ParaStartCtl = NULL;
RTFctl *RTFwr::TxtParaStartCtl = NULL;
RTFctl *RTFwr::TbCellParaStartCtl = NULL;
RTFctl *RTFwr::PreFtnParaStartCtl = NULL;
RTFctl *RTFwr::PrevStartCtl = NULL;
bool RTFwr::PrevParaIsTb = false;
long RTFwr::ItemRefNum = 0;
long RTFwr::ParaRefNum = 0;
long RTFwr::TxtParaRefNum = 0;
long RTFwr::PreFtnTxtParaRefNum = 0;
long RTFwr::JustPara = 0;
long RTFwr::JustLineEndCount = 0;
uns RTFwr::JustParaEndSpace = 0;
DCilist RTFwr::JustLineEndList;
DCvlist RTFwr::PRefFrameList;
DClist RTFwr::FrPendList;
DClist RTFwr::TbCellParaEndList;


void
RTFwr::ProcParaStart(RTFctl *cp)
{
  cp->siz(ParaRefNum = ++ItemRefNum);  // assign ref to all paras

	if (!InTbl) {  // keep ref to containing text para
		if (PrevParaIsTb == true)
			PrevParaIsTb = false;
		else PrevStartCtl = TxtParaStartCtl;
		TxtParaStartCtl = cp;
		TxtParaRefNum = ParaRefNum;  // leave as para before table
	}

	if (InTbCell)
		TbCellParaStartCtl = cp;

  ParaStartCtl = cp;
	RuninPara = 0;
	JustPara = 0;
	JustLineEndCount = 0;
	XrefFile = NULL;
	XrefFilePrefix = CurrFilePrefix;
	LastLineEnd = NULL;

	if (NextFirstMaster != 0) {
		SectBreakList.add(NextFirstMaster, TxtParaRefNum);
		NextFirstMaster = 0;
	}

  if (StartingPage && (LastPage > LastMasterPage)
   && (FramePageList.find(LastPage) != NULL)) {
    if (Verbose)
      printf("start page %d with para %ld\n", LastPage, TxtParaRefNum);
    FramePageList.execu(LastPage, ReAnchorFrame);
    StartingPage = false;
    if (!SectUsedList.find(SectNum))
      SectUsedList.add(SectNum);
  }

	HyperPara = 0;
	HyperJump = false;
	HyperVal = 0;
	PopOver = false;
	HyperColor = HyperColorEnding = false;
	HyperForm = HyperFormEnding = false;
}


void
RTFwr::ReAnchorFrame(void *vp)
{
  PRefFrameList.add(vp, ParaRefNum);
  if (Verbose)
    printf("anchor frame at 0x%x to para %ld\n", vp, TxtParaRefNum);
}


void
RTFwr::ProcFrAnchor(RTFctl *cp)
{
  frame *fr;
	long pref = ParaRefNum;

  if ((fr = (frame *) FrameList.find(cp->dat())) != NULL) {
    if (!SectUsedList.find(SectNum)
     && (LastPage > LastMasterPage))
      SectUsedList.add(SectNum);

		if (fr->fpos >= 1) {  // not inline
			if ((InTbCell  // determine whether frame is in cell or not
			  || InTbl)
			 && (fr->fwrap != 1)) { // very difficult ****
				if (TbGraphics == 'N')
					return;
				if (TbGraphics == 'O') {
					pref = TxtParaRefNum;
					//fr->fpos = 2; // put at start of prev para
					fr->vref = 3;  // attached to para
					fr->valgn = 4; // no offset
					PRefFrameList.add(fr, pref);
					if (Verbose)
						printf("add frame at 0x%x to para %ld\n", fr, pref);
					return;
				}
			}

			if (fr->fpos == 2) // need to put out frame at start of para
				PRefFrameList.add(fr, pref);
			else   // postpone to end of para
				FrPendList.add(pref);

			if (Verbose)
				printf("add frame at 0x%x to para %ld\n", fr, pref);
		}
  }
}


void
RTFwr::ExecFrame(DCfile *wf, void *vp)
{
  if (Verbose)
    printf("  write frame at 0x%x\n", vp);

	InPara = false;
	ParaStarting = false;
  WriteFrame( (RTFwfile *) wf, (frame *) vp);
}


DCilist RTFwr::HelpTitleParaList;
bool RTFwr::InHelpTitlePara = false;
bool RTFwr::StartingHelpTopic = false;
bool RTFwr::AnchorPara = false;
long RTFwr::CurrSlider;
long RTFwr::PreSlideDiv;
long RTFwr::PHtype;
bool RTFwr::WhiteText = false;
bool RTFwr::NonWhiteText = false;
bool RTFwr::DeletingText = false;

bool RTFwr::TopicEmpty = true;
bool RTFwr::LastTopicEmpty = true;

bool RTFwr::ParaStartFrames = false;
bool RTFwr::ParaStartFrPend = false;
bool RTFwr::ParaPropsWritten = false;
bool RTFwr::ParaFrameProps = false;
bool RTFwr::PStartSeen = false;

pprop RTFwr::AnchorProp;

void
RTFwr::StartPara(RTFwfile *wf, RTFctl *cp)
{
  if (WritingFrame
	 && !InTextFrame)
		return;
	PStartSeen = true;
	DeletingText = false;
	ParaPropsWritten = false;
	InheritPending = false;
	XrefFile = NULL;
	XrefFilePrefix = CurrFilePrefix;
	XrefMarker = false;

  if (StartingSection
	 || NextFirstMaster) { // do not recurse
		ParaStarting = true;
		InPara = false;
		return;
	}

	if (ParaEnding == true) {
		if (ParaStarting)
			WriteText(wf);
		wf->putCW("par");
		wf->putEnd();
		ParaEnding = false;
	}


	ParaRefNum = cp->siz();
	//assert(ParaRefNum);
	if (Verbose)
		printf("at para %ld:\n", ParaRefNum);

	if (!InTbCell)
		TxtParaRefNum = ParaRefNum;  // leave as para before table

	if ((CurrSection = SectionStart.find(ParaRefNum)) != 0) {
		StartingSection = true;
		WriteSectBreak(wf, 0);
		StartingSection = false;
	}
	else if ((NextFirstMaster = (uns) SectBreakList.find(ParaRefNum)) != 0) {
		WriteSectBreak(wf, NextFirstMaster);
		NextFirstMaster = 0;
	}

	ForcedBreak = false;
	SectStarting = false;
	PHtype = 0;
	CurrRef = 0;
	//CurrTab->tpos = 0;

	if (HelpForm == true) {
		PHtype = HelpTitleParaList.find(ParaRefNum);
		CurrRef = MakeRefList.find(ParaRefNum);
	}

	if ((PHtype & 7) == 0)  // wait until para start format is seen
		ParaStartFrPend = true;

	if (InTbCell == true)
		EndingTbCell = TbCellParaEndList.find(ParaRefNum);

	InPara = false;
	ParaStarting = true;
  TabAlign = 0;
	WhiteText = false;
	NonWhiteText = false;

	if (PHtype)
		SetParaHelp(wf);

	if ((!WritingFrame)
	 && FrPendList.find(ParaRefNum)
	 && (FramePendList == NULL))
		FramePendList = new DClist;

	HyperPara = (uns) HyperParaList.find(ParaRefNum);

	if (AnumValList.find(ParaRefNum) != NULL)
		AnumPending = true;

	if (XRLocList.find(ParaRefNum))
		SetXRBookmarks();

	JustLineEndCount = JustLineEndList.find(ParaRefNum);

	if (!FootDiv || !InFootnote)
		CodeStoreVar = (char *) CodeStoreParaList.find(ParaRefNum);
}


void
RTFwr::WriteParaStartFrames(RTFwfile *wf)
{
	ParaStartFrPend = false;
	ParaStartFrames = true;
	bool IhPend = InheritPending;
	InheritPending = false;
	pprop IhForm = InheritPendForm;
	SaveState();
	if (PRefFrameList.find(ParaRefNum) != NULL) {
		if (ParaFrameProps  // prevent improper merging of frames
		 && !InTextFrame)
			WriteAnchorPara(wf, -20, false);
		PRefFrameList.write(ParaRefNum, (DCfile *) wf, ExecFrame);
	}
	RestoreState(FullState);
	InheritPending = IhPend;
	InheritPendForm = IhForm;
	ParaStartFrames = false;
}


void
RTFwr::SetParaHelp(RTFwfile *wf)
{
	if (InPxSxFrame)
		PHtype &= 8;  // only allow stop of slider

	if ((PHtype & 1) == 1) { // topic title, not popup
		InHelpTitlePara = true;
		HelpTitlePending = true;
		if ((DisambiguateIndex == 'T')  // reset top-level key list
		 && TopIdxCount) {
			TopIdxList.clear();
			TopIdxList.empty();
			TopIdxCount = 0;
		}
	}

	if ((PHtype & 16) == 16) {  // contents entry
		LastTopicEmpty = TopicEmpty;
		TopicEmpty = true;
		if (PHtype == 16) { // this is only process needed
			PHtype = 0;
			return;
		}
	}

	if ((PHtype & 4) == 4) {      // slider code
		if (PGStarted) { //set before changing div
			wf->putGE();
			PGStarted = 0;
			wf->putEnd();
		}
		if (CurrSlider == 0) {           // either start new
			CurrSlider = wf->NewDiv();
			PreSlideDiv = CurrentDiv;
		}
		else wf->SelectDiv(CurrSlider);	// or resume current
		CurrentDiv = CurrSlider;
	}
	else if ((PHtype & 8) == 8) { // suspend slider code
		if (CurrSlider) {
			if (PGStarted) { //set before changing div
				wf->putGE();
				PGStarted = 0;
				wf->putEnd();
			}
			wf->SelectDiv(CurrentDiv = PreSlideDiv);
		}
	}
	else if ((PHtype & 1) == 1)   // non slider topic, write slider 
		WriteTopicEnding(wf);

	if ((((PHtype & 8) != 8)    // don't start topic if suspending slider
	  || ((PHtype & 3) != 0))   // unless para really is a topic start
	 && !StartingHelpTopic) {
		if (PGStarted) {
			wf->putGE();
			PGStarted = 0;
			wf->putEnd();
		}
		wf->putCW("page");
		if (HelpSectBreaks)
			wf->putCW("sect");
	  wf->putEnd();
		InPara = false;
		StartingHelpTopic = true;
	}
}


void
RTFwr::WriteTopicEnding(RTFwfile *wf)
{
	if (PGStarted) { //set before changing div
		wf->putGE();
		PGStarted = 0;
		wf->putEnd();
	}
	// write out end-of-topic footnotes, visible (jump) or popup
	if (FootnoteNum
	 && FootDiv) {
		DCmacr::WriteMacro(wf, FtnSeparator);
		wf->WriteDiv(FootDiv);
		wf->ClearDiv();
		wf->SelectDiv(CurrentDiv);
		FootnoteNum = 0;
		TxFootNum = 0;
	}
	if (!Curr.del && TopicEndInsert)
		DCmacr::WriteMacro(wf, TopicEndInsert);

	if (CurrSlider)	{
		wf->SelectDiv(PreSlideDiv);
		if (!Curr.del && SlideStartInsert)
			DCmacr::WriteMacro(wf, SlideStartInsert);
		wf->WriteDiv(CurrSlider);
		if (!Curr.del && SlideEndInsert)
			DCmacr::WriteMacro(wf, SlideEndInsert);
		wf->DeleteDiv(CurrSlider);
		CurrSlider = 0;
		CurrentDiv = PreSlideDiv;
	}

	if (CondEndPend)
		WriteCondProps(wf);
}


void
RTFwr::ProcParaEnd(RTFctl *cp)
{
	if (JustPara
	 && JustLineEndCount)
		JustLineEndList.add(JustLineEndCount, JustPara);

	if (HyperJump
	 && !(HyperColor || HyperForm))
		HyperParaList.add(HyperVal, ParaRefNum);

	if (Curr.cstore)
		CodeStoreParaList.add(Curr.cstore, ParaRefNum);

#if 0
	if (Tbl && Row && Cell
	 && InTbCell && StartingTbCell
	 && (TbRuleType != 'N')
	 && (TbRuleType != 'C')) {
		RTFctl *rcp = new RTFctl(imnem, 2, 10, 1, 32);
		rcp->rp->write_op = puttx;
		StartCellText(rcp);
		AddCellCont(rcp);
	}
#endif

	char *chp = NULL;

	if (StoreText) {
		PutTextStoreBuf('\0');  // final terminator
		PutTextStoreBuf('\0');  // for config settings
		chp = GetTextStoreBuf(ConfigHere);
	}

	if (chp) {
		if (ConfigHere) {
			DCini::ProcConfigPara(chp, ParaRefNum);
			ConfigHere = false;
		}
		if (TextStoreHere) {
			uns len = strlen(chp);
			if (len > 0) {
				TextStoreNames.add(TextStoreName, ++TextStoreCount);
				if ((chp[len - 1] == ' ')
				 || (chp[len - 1] == '\n'))
					len--;
				TextStoreVals.add(NewName(chp, len), TextStoreCount);
			}
			TextStoreHere = false;
		}
	}
	TextStoreBufCnt = 0;
	StoreText = false;

	if (LastLineEnd) {
		CancelLineEnd.add((long) LastLineEnd);
		LastLineEnd = NULL;
	}
}


void
RTFwr::EndPara(RTFwfile *wf)
{
	if (!PStartSeen)
		return;

	bool RealEnd = true;
	bool hiding = ((!HelpForm) && Curr.hide);
	ForcedBreak = false;
	
	if (ParaForm.del) {
		if (CodeStoreDiv)  // end any pending div
			EndCodeStore(wf);
		WriteFramesAndTables(wf);
		CurrTab->tpos = 0;
		return;
	}

	if (CondsEffective
	 && !CondFlagDiv) {
		PreCondFlagDiv = wf->GetDiv();
		CondFlagDiv = wf->NewDiv();
	}

	if ((FramePendList
	 || TblList)
	&& !NonWhiteText
	&& !KeepAnchors)   // anchor holder, props already written
		RealEnd = false;

	if (AnumPending) {  // write trailing anum now
		SaveState();
		PutAnumVal(wf, false);
		RestoreState(CurrState);
	}

	if (EndUL
	 && !HelpForm) {
		wf->putCS('~');
		EndUL = false;
	}
	
	if ((HelpForm == true)
	 && !InPxSxFrame)	{
		ProcHelpBreak(wf);
		if ((Curr.hyper > 0)
		 || (Last.hyper > 0)
		 || HyperPara
		 || HyperColor
		 || HyperForm
		 || RefStr) {
			EndAnchor(wf);
			Last.hyper = 0;
			Curr.hyper = 0;
		}
		if ((Last.line > 1)  // actually error, missing ref string
		 && HyperHelp) {     // but just balance the braces for now
			RestoreState(LastState);
			wf->putGE();
			Last.line = 1;
		}
		WriteHyperUnique(wf); // dump any remaining IDs now
		WriteRefStrs(wf);
	}

	if (!HelpForm
	 && ((Curr.hyper > 0)
		|| (Last.hyper > 0)
		|| HyperPara
		|| HyperColor
		|| HyperForm
		|| RefStr)
	 && !EndingAnchor) {
		HyperEnd = true;
		EndAnchor(wf);
		Curr.hyper = (Curr.hyper == 3) ? 1 : 0;
	}


	FlowWritten = true;

	if ((WhiteText == true)
	 && (NonWhiteText == false)) {
		InPara = false;
		CurrTab->tpos = 0;
		TabAlign = 0;
		ParaStarting = false;
		if (CodeStoreDiv)  // end any pending div
			EndCodeStore(wf);
		return;
	}

	if (!HelpTitlePending
	 && ParaStarting
	 && !InTextMetafile
	 && !InPara) {
		CalcLineSpace();
		if (NegLineSpace) {  // empty placeholder, skip
			if (Curr.pbreak)
				WriteBreak(wf);
			EndingTextFrame = false;
			InPara = false;
			CurrTab->tpos = 0;
			TabAlign = 0;
			ParaStarting = false;
			if (CodeStoreDiv)  // end any pending div
				EndCodeStore(wf);
			return;
		}
		//if (InTbCell
		// && (TbRuleType != 'N')
		// && (TbRuleType != 'C'))
		//	WriteText(wf, &RTFctl(imnem, 2, 10, 1, 32));
		//else
			WriteText(wf);
	}

	XREndPara(wf);
  InPara = false;

	if (SideHUsed
	 && (SideHType == 'F')  // ending framed sidehead para
	 && (!InTbCell)
	 && (!InTbTitle)
	 && (!Curr.sxframe)
	 && ((Curr.sideh > 1)
	  || ((Curr.sideh == 1)
	   && ((SideHSpaceList.find(ParaRefNum) > 0)
	    || SHSpannerAnchors)))) {
		SideHAnchor = true;     // next is its anchor
		AnchorPara = true;
	}
	else
		SideHAnchor = false;

//	if (HyperPara)
//		ResetHyperPara();

	RestoreInheritForm(); // inherit restore at end of para
	WriteSimpleCharProps(wf);

	if (CharStylesUsed)  // ensure ending brace written
		WriteCharForm(wf);

	if (InTextMetafile)
		EndTextMetafile(wf);

	if (Curr.runin)  // no closing \par for runin paras
		return;

	AnchorProp = Curr;

	if (TrailingTabs)
		WriteOutTabs(wf);
	else
		CurrTab->tpos = 0;      // turn off any pending tab
  TabAlign = 0;

	if (AnchorPara  // framed spanner, write anchor para
	 && !InTextFrame
	 && RealEnd) {
		WriteAnchorPara(wf, -20);
		AnchorPara = false;
	}

  if (InTbCell) {
    ParaEnding = true;
		if (InHelpTitlePara
		 && HelpCloneCount) {
			wf->putCW("keepn");
			if (Curr.clone == 0)
				Curr.clone = 14;
			WriteHelpClone(wf, &Curr);
			Curr.clone = 0;
			InHelpTitlePara = false;
		}
		else if (TbRuleAdjust
		 && !NonWhiteText)
			WriteText(wf);
		if (IndexEntry)
			WriteIndexEntry(wf);
		if (Curr.sxframe) {
			if (!ParaStarting)
				WriteParaEndCode(wf);  // end prev para
			WritePreSufFrame(wf, Curr.sxframe, false);
			if (!Curr.del && Curr.macca) {
				DCmacr::WriteMacro(wf, Curr.macca);
				Curr.macca = NULL;
			}
		}
	}
  else {
		if (IndexEntry)
			WriteIndexEntry(wf);
		if ((!HelpTitlePending) || InTbTitle) {
			ParaStarting = false;
			if (RealEnd)
				WriteParaEndCode(wf);   // otherwise end para
			wf->putEnd();
			InHelpTitlePara = false;
			LinePos = Curr.find;
			if (Curr.sxframe) {  // handle suffix frame after end of para
				WritePreSufFrame(wf, Curr.sxframe, false);
				ParaFrameProps = true;
			}
			if (!Curr.del && Curr.macca) {
				DCmacr::WriteMacro(wf, Curr.macca);
				Curr.macca = NULL;
			}
		}
	}

	if (InTbTitle)
		return;
  if (!InTbCell) {
		if (InHelpTitlePara) {	// give table a chance
			ParaStarting = false;
			if (TblList && !InTbl && !TblListStarted) {
				TblListStarted = true;
				uns tnum = 1;
				while ((Tbl = (tbl *) TblList->find(tnum++)) != NULL)
					WriteTbl(wf);
				delete TblList;
				TblList = NULL;
				TblListStarted = false;
			}
			if (IndexEntry)
				WriteIndexEntry(wf);
			if (RealEnd)
				WriteParaEndCode(wf);
			wf->putEnd();
			LinePos = Curr.find;
			InHelpTitlePara = false;
			if (Curr.sxframe)   // handle suffix frame after end of para
				WritePreSufFrame(wf, Curr.sxframe, false);
			if (!Curr.del && Curr.macca) {
				DCmacr::WriteMacro(wf, Curr.macca);
				Curr.macca = NULL;
			}
		}
		StartingHelpTopic = false;
		HelpTitlePending = false;
  }

	//if ((PHtype & 7) != 0)
	//	WriteParaStartFrames(wf);
	//if (InTblCell && RemoveNontextFrames)
	//	return;

	WriteFramesAndTables(wf);

	if (hiding) {
		wf->putCW("v0");
		Curr.hide = 0;
	}
}


void
RTFwr::WriteParaEndCode(RTFwfile *wf)
{
	if (InFootnote) {
		FtnParaEnding = true;
		if (Curr.macce)
			FtnEndMacro = Curr.macce;
		//wf->putCW("par");
		return;
	}

	if (!Curr.del && Curr.macce) {
		DCmacr::WriteMacro(wf, Curr.macce);
		Curr.macce = NULL;
	}

	wf->putCW("par");
	InUnicode = false;
	InAnsi = false;

	if (CodeStoreDiv)  // end any pending div
		EndCodeStore(wf);
}


void
RTFwr::WriteFramesAndTables(RTFwfile *wf)
{
  if (FramePendList && !WritingFrame && !InTextFrame) {
    uns fnum = 1;
		frame *fr;
		while ((fr = (frame *) FramePendList->number(fnum++)) != NULL)
			WriteFrame(wf, fr);
    delete FramePendList;
    FramePendList = NULL;
  }
	if (TblList && !InTbl && !InTbCell && !TblListStarted) {
		TblListStarted = true;
		uns tnum = 1;
		while ((Tbl = (tbl *) TblList->find(tnum++)) != NULL)
			WriteTbl(wf);
		delete TblList;
		TblList = NULL;
		StartingHelpTopic = false;
		TblListStarted = false;
	}
}

void
RTFwr::WriteAnchorPara(RTFwfile *wf, short lsize, bool atend)
{
	if (InTbCell
	 || InTextFrame
	 || CurrTextFrame)
		return;

	if (atend) {
		wf->putCW("par");
		wf->putEnd();
	}
	SetPardProps(wf);
	if (Curr.keepn
	 && ((HelpForm != true)
	 || InHelpTitlePara))
		wf->putCW("keepn");
	wf->putCWN("sl", lsize, true);
	if (!atend) {
		wf->putCW("par");
		wf->putEnd();
	}
	ParaFrameProps = false;
}



// end of dwrtftx.cpp


