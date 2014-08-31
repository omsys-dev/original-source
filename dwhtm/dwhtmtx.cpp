 
// dwhtmtx.cpp is the code for dcl HTM text writing, JHG
// Copyright (c) 1996-1998 by Omni Systems, Inc.  All rights reserved.

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
#include "dcmxl.h"


// selection switch


bool
HTMwr::ProcTx(HTMctl *cp)
{
  switch((cp->rp)->scan_op) {
    case nothing:   // just put with its group for later
      break;

    case pass:      // ignore, don't save 
      return false;

    case docdefs:   // done storing doc defaults
      SetDefForm();
			InResources = true;
      return false;

    case docres:    // done storing doc resources
      FormsStored = FormNum;
			SetCharRangeConds();
			InResources = false;
      return false;

    case txfile:    // process during write phase 
      break;

    case fontlist:  // store font info
      ProcFont(cp);
      if (InGraphic) {
        ProcGr(cp);
        return false;
      }
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
    case formtag:
      break;
    case formspc:
      break;

		case formpfx:
			InFormDefPfx = true;
			break;
		case formpfe:
			InFormDefPfx = false;
			break;
		case formsfx:
			InFormDefSfx = true;
			break;
		case formsfe:
			InFormDefSfx = false;
			break;

		case formscch:
			if (InResources)
				return true;
			if (!InPara)
				return false;
		case formscan:
			if (InGraphic
			 || InGraphicText)
				return false;

			if (!InFormDef
			 && (InFormDefPfx || InFormDefSfx))
				return false;

			if (InTDefaults)
				return false;

			if (GettingMapDescText)
				return false;

			CheckForm(cp);
			break;

    case formchk:
			if (InGraphic
			 || InGraphicText)
				return false;
			if (InRunGroupDef)
				break;
			if (((cp->rp)->write_op == putbase)
			 && (InFtnAnum || InTbFtnAnum)) {
				if (InFtnAnum) {
					if (InAnFormat)
						FootElem = (cp->siz() == 1) ? "sub" :
						          ((cp->siz() == 2) ? "sup" : "");
					else if (InAnRefFormat)
						FootRElem = (cp->siz() == 1) ? "sub" :
						           ((cp->siz() == 2) ? "sup" : "");
				}
				else if (InTbFtnAnum) {
					if (InAnFormat)
						TbFootElem = (cp->siz() == 1) ? "sub" :
						            ((cp->siz() == 2) ? "sup" : "");
					else if (InAnRefFormat)
						TbFootRElem = (cp->siz() == 1) ? "sub" :
						             ((cp->siz() == 2) ? "sup" : "");
				}
				return false;
			}	
			if (InCondDef) {
        SetCondProps(cp);
        return false;
      }
			if (InFormDef) {
        SetCurrProps(NULL, cp);
        return false;
      }
			if (InFormDefPfx || InFormDefSfx)
				return false;
			switch ((cp->rp)->write_op) {
				case parstart:
					WhiteText = false;
					ProcParaStart(cp);
					break;
				case parend:
					if (StoreText
					 && (InDocBody || TextFrameContent)
					 && (!HideTbRow)
					 && (!InIdxGroup)
					 && (!InRunGroupDef)
					 && (!InTbl || AllowTbTitle || (OmniHelp && UseFTS))
					 && (!InAnumVal || !SkipAnums)
					 && (!InFormPx || !SkipAnums)
					 && (!WhiteText || !HideWhite)
					 && (SectNum || TextFrameContent))
						PutTitleBuf(' ');
					ProcParaEnd(cp);
					break;
				case putcolr:
					WhiteText = HideWhite && (cp->siz() == 2);
					ProcHyperColor(cp);
					break;

				case formpx:
				case formsx:
					InFormPx = true;
					break;
				case formrest:
					InFormPx = false;
					break;

				case puttab:
					if (StoreText
					 && (InDocBody || TextFrameContent)
					 && (!HideTbRow)
					 && (!InIdxGroup)
					 && (!InRunGroupDef)
					 && (!InTbl || AllowTbTitle || (OmniHelp && UseFTS))
					 && (!InAnumVal || !SkipAnums)
					 && (!InFormPx || !SkipAnums)
					 && (!WhiteText || !HideWhite)
					 && (SectNum || TextFrameContent))
						PutTitleBuf(' ');
					if (GettingMapDescText) {
						PutTitleBuf(' ');
						return false;
					}
					break;

				case linend:
					if ((cp->siz() == 0)
					 && DescHyph) {
						DescHyph = false;
						return false;
					}
					if (GettingMapDescText) {
						PutTitleBuf(' ');
						return false;
					}
					if (StoreText
					 && (!HideTbRow)
					 && (ConfigHere
					  || (UseFTS && OmniHelp))) {
					// && (cp->siz() == 1)) { // hard return
						PutTitleBuf(' ');
						if (ConfigHere && (cp->siz() == 1))
							PutTitleBuf('\0');
					}
					break;

				default:
					break;
			}
      break;

		case sideflow:
			if (cp->siz() > 0) {
				if (SideHFlowList.find(CurrFlow) == false)
					SideHFlowList.add(CurrFlow);
			}
			return false;
		case setrunin:
			Curr.rungroup = new HTMgroup(*cp);
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


		case idxchk:
			CheckIndexTOCItem(cp);
			break;

#ifdef UNUSED
    case language:  // determine default language
      ProcLang(cp);
      return false;
    case charset:   // determine default character set
      CurrCharSet = cp->dat();
      return false;
#endif

		case conddef:
			StartCondDef(cp->dat());
			break;
		case condnam:
			ProcCondName(cp);
			break;
		case condvis:
			if (CurrCond
			 && (cp->siz() == 1))
				HiddenConds.add(CurrCond);
			break;

		case condind:  // group of condition indicators
			// just open, uses putrevb and putline
			break;
		case condproc: // store condition indicators and color
			ProcCondDef();
			break;

		case condfcol:  // fgcolor for condition
			if (CurrCondProps)
				CurrCondProps->fcolor = cp->siz();
			break;
		case condbcol:  // bgcolor for condition
			if (CurrCondProps)
				CurrCondProps->bcolor = cp->siz();
			break;
		case condchcol:
			if (CurrCondProps)
				CurrCondProps->chcolor = cp->siz();
			break;
		case condcchr:
			if (CurrCondProps)
				CurrCondProps->chchars = NewName((char *) cp->ex(), cp->siz());
			break;
		case condstim:
			if (CurrCondProps)
				CurrCondProps->startimage = NewName((char *) cp->ex(), cp->siz());
			break;
		case condstalt:
			if (CurrCondProps)
				CurrCondProps->startalt = NewName((char *) cp->ex(), cp->siz());
			break;
		case condenim:
			if (CurrCondProps)
				CurrCondProps->endimage = NewName((char *) cp->ex(), cp->siz());
			break;
		case condenalt:
			if (CurrCondProps)
				CurrCondProps->endalt = NewName((char *) cp->ex(), cp->siz());
			break;


		case condset:  // work out div/span usage during scan phase
			ProcCondStart(cp->dat());
			break;
		case condres:
			ProcCondEnd(cp->dat());
			break;

		case conddis:
			ShowHiddenCond = (cp->siz() == 1) ? true : false;
			break;


    case flowlist:  // collect segments of each text stream
      ProcFlow(cp);
      break;
		case flowname:
			ProcFlowName(cp);
			break;
		case flowend:
			SectNum = 0;
			if (InGraphicText)
				ProcGrTextFlowEnd(cp);
			InGraphicText = false;
			GrTextFrame = NULL;
			break;

    case flowused:
			if (InFtnAnum) {
				if (InAnFormat) {
					if (InFtPx)
						cp->append(FootPx);
					else
						cp->append(FootSx);
				}
				else if (InAnRefFormat) {
					if (InFtPx)
						cp->append(FootRPx);
					else
						cp->append(FootRSx);
				}
				else if (InAnSyms)
					cp->append(FootSyms);
			}
			else if (InTbFtnAnum) {
				if (InAnFormat) {
					if (InTbFtPx)
						cp->append(TbFootPx);
					else
						cp->append(TbFootSx);
				}
				else if (InAnRefFormat) {
					if (InTbFtPx)
						cp->append(TbFootRPx);
					else
						cp->append(TbFootRSx);
				}
				else if (InAnSyms)
					cp->append(TbFootSyms);
			}

      if (InGraphic
			 || InGraphicText)
        return false;

			if (!InFormDef
			 && (InFormDefPfx || InFormDefSfx))
				return false;

      if (LastPage > LastMasterPage) {
        MarkSectUsed(cp);

				HyperFormEnding = false;  // turn off after real text
				HyperColorEnding = false;
				HyperJumpStarting = false;


				if (!(InIdxGroup || InRunGroupDef || InAnumVal || InFormPx)) {
					if (ParaStartScan) {
						ProcCondParaStart();
						ParaStartScan = false;
					}
					if (CondCharPending) {
						ProcCondCharStart();
						CondCharPending = false;
					}
				}

#if CONDSPANS
				StartPoint++;
#endif

				if (GettingMapDescText) {
					ProcTitleText(cp);
					return false;
				}

				if (StoreText
				 && (InDocBody || TextFrameContent)
				 && (!HideTbRow)
				 && (!InIdxGroup)
				 && (!InRunGroupDef)
				 && (!InTbl || AllowTbTitle || AttrHere || (OmniHelp && UseFTS))
				 && (!InAnumVal || !SkipAnums)
				 && (!InFormPx || !SkipAnums)
				 && (!WhiteText || !HideWhite)
				 && (SectNum || TextFrameContent)) {
					if (StartingFootnote) {
						StartingFootnote = false;
						PutTitleBuf(' ');
					}
					ProcTitleText(cp);
				}

				if (InGlossTitle)
					AddToGlossTitle(cp);

				if (!InRunGroupDef
				 && !GettingMapDescText)
					NonWhiteText = true;  // presumably

				if (UseCharRangeClasses
				 && ProcCharRangeText(cp))
					return false; // replaced cp
			}
      break;

		case tinset:
			ProcStartTextInset(cp);
			break;
		case tiname:
			TextInsetName.add(NewName((char *)cp->ex(), cp->siz()), TextInsetNum);
			break;
		case tifile:
			TextInsetFile.add(NewName((char *)cp->ex(), cp->siz()), TextInsetNum);
			break;
		case tinend:
			ProcEndTextInset(cp);
			break;
		case tiimph:
		case tiaupd:
		case tilupd:
		case tilock:
			break;

		case ticlnt:
		case ticlname:
		case ticlfile:
		case ticltype:
		case ticldata:
			break;

		case tiflow:
			break;
		case timflw:
			if (cp->siz())
				TextInsetMainFlow.add(TextInsetNum);
			break;
		case tipgsp:
			TextInsetFlowPg.append(cp->siz() + 1, TextInsetNum);
			break;
		case tiftag:
			TextInsetFlowTag.add(NewName((char *)cp->ex(), cp->siz()), TextInsetNum);
			break;
		case tiform:
		case tinovr:
		case tinpgb:
			break;

		case mapref:
			// mapref ctls are always there
			if (UseMapDescAsTitle)
				ProcMapTopicHead(cp);
			break;
		case mapdsc:
			if (UseMapDescAsTitle)
				ProcMapDesc(cp);
			break;
		case relref:
			// relref ctls are only added if needed
			ProcMapTopicHead(cp);
			break;
		case reldsc:
			ProcMapDesc(cp);
			break;

    default:
#ifdef UNUSED
      assert(0);
#endif
			LogEvent(logdebug, 1, "Invalid case in ProcTx()");
      return false;
  }
  return true;
}


// properties processing


pprop HTMwr::Last;
pprop HTMwr::Curr;
pprop HTMwr::DefForm;
pprop HTMwr::ParaForm;
bool HTMwr::InResources = false;


void
HTMwr::SetDefForm(void)
{
	uns val = 0;

  DefForm = Curr;
  DefForm.stag = "Normal";
  DefForm.stype = 1;
  Last = DefForm;
  FormList.add(&DefForm, 1L);
  FormNameList.add(DefForm.stag, 1L);
  if (DefForm.tcount)
    FormTabList.add(DefForm.tabs, 1L);

	// get color overrides from .ini file now
	if (IniFile
	 && IniFile->Section("Colors")) {
		// step through entries and ProcColorDef(num, val)
		while (IniFile->NextItem()) {
			if (((val = (uns) ColorNames(IniFile->StrLeft())) == 0)  // not already done
			 && ((val = (uns) IniFile->IntLeft()) != 0)) // and numeric val given
				ProcColorDef(val + 1, IniFile->HexRight());
		}
	}
}



// format setting and calling


DCvlist HTMwr::FormList;
DCnlist HTMwr::FormNameList;
DCilist HTMwr::FormRefList;
DCvlist HTMwr::FormTabList;
uns HTMwr::FormNum = 1;
uns HTMwr::DCLFormNum;
uns HTMwr::FormsStored = 1;
char *HTMwr::FormName;
bool HTMwr::InFormDef;
bool HTMwr::InFormDefPfx;
bool HTMwr::InFormDefSfx;


void
HTMwr::ProcForm(HTMctl *cp)
{
  Curr = DefForm;
  DCLFormNum = cp->dat();
}


void
HTMwr::ProcFormType(HTMctl *cp)
{
  if ((Curr.stype = (uns) cp->siz()) == 4) {  // for ruling formats
    ProcRuleForm();
  }
  else if (Curr.stype == 3) {           // for table formats
    ProcTblForm();
  }
  else if (Curr.stype == 2) {           // for inherited formats
    Curr.fnum = Curr.fsize = Curr.bold = Curr.ital = Curr.strike = 0;
    Curr.base = Curr.caps = Curr.scaps = Curr.fsym = Curr.kern = 0;
    Curr.line = 0;
		Curr.hsname = NULL;
		Curr.colr = 0;
  }
}


char *HTMwr::HTStyles[HtmStyleMax] = {
 "h1", "h2", "h3", "h4", "h5", "h6",
 "address", "blockquote", "pre", "div"
};

char *HTMwr::HTMLListInit[ListInitMax] = {
 "ol", "ol", "ol", "ol", "ol",
 "ul", "ul", "ul",
 "dir", "menu",
 "dl", "dl"
};

char *HTMwr::HTMLListType[ListTypeMax] = {
 "1", "i", "I", "a", "A",
 "disc", "circle", "square"
};

list HTMwr::CurrList = { false, 0, 0, 0 };

bool HTMwr::InFormPx = false;
bool HTMwr::CSSLeading = true;
bool HTMwr::LinkClassIsParaClass = false;
bool HTMwr::XrefFormatIsXrefClass = false;
bool HTMwr::NoClassLists = true;
bool HTMwr::NoAttribLists = false;
bool HTMwr::UseListTypeAttribute = false;
bool HTMwr::ProhibitPrefix = false;
bool HTMwr::AllowPrefix = false;
bool HTMwr::ProhibitSuffix = false;
bool HTMwr::AllowSuffix = false;
bool HTMwr::ZeroCSSMargins = false;


void
HTMwr::ProcFormName(HTMctl *cp)
{
	if (cp->siz() == 0)
		return;
  FormName = NewName((char *) cp->ex(), cp->siz());
}


void
HTMwr::StoreForm(void)
{
	//uns i;
	unc align;

  if (Curr.stype == 4) { // ruling format
    StoreRuleForm();
    return;
  }
  else if (Curr.stype == 3) { // table format
    StoreTblForm();
    return;
  }

  pprop *form = new pprop;
  Curr.snum = FormNum++;  // stored num is 1 less than ref
  Curr.stag = FormName;
	Curr.hpname = (ClassIsTag) ? FormName : "p";
	Curr.hrpara = HardRetPara;  // set default for hard returna in HXML

	if (UseSpanAsDefault
	 && (Curr.stype == 2))
		Curr.hsname = "span";

	if (IniFile) {

		if (Curr.stype == 1)
			DCini::ProcIniGroup("P1StyleSet");
		else if (Curr.stype == 2)
			DCini::ProcIniGroup("C1StyleSet");
		DCini::ProcIniGroup("Style1Set");

		if (Curr.stype == 1)
			DCini::ProcIniGroup("PStyleSet");
		else if (Curr.stype == 2)
			DCini::ProcIniGroup("CStyleSet");
		DCini::ProcIniGroup("StyleSet");

		if (HXMwr::DITA) {
			//Curr.anskip = true;
			Curr.dtmhd = 'T';
			if (!DCini::ProcIniSect("DITAAliases"))
				DCini::ProcIniSect("DITAAlias");
			if (Curr.dstag) {
				Curr.tstag = Curr.stag;
				Curr.stag = Curr.dstag;
			}
			if (Curr.stype == 1)
				DCini::ProcIniGroup("DStyleSet");
		}

		if (HXMwr::DocBook) {
			if (!DCini::ProcIniSect("DocBookAliases"))
				DCini::ProcIniSect("DocBookAlias");
			if (Curr.dstag) {
				Curr.tstag = Curr.stag;
				Curr.stag = Curr.dstag;
			}
			if (Curr.stype == 1)
				DCini::ProcIniGroup("DBStyleSet");
		}

		Curr.osize = Curr.fsize;
		Curr.obold = Curr.bold;
		Curr.oital = Curr.ital;
		Curr.oline = Curr.line;
		Curr.ostrike = Curr.strike;

		if (Curr.contents) {
			Curr.cntform = 1;
			if (Curr.split
			 && !UseChunking) {
				Curr.splitlevel = Curr.contents;
				if (MakeLocalTOC) {
					if (!Curr.loctocset)
						Curr.loctoclevel = Curr.contents;
				}
				else Curr.loctoclevel = 0;
			}
			if (MakeTrail) {
				if (!Curr.trailset)
					Curr.traillevel = Curr.contents;
			}
			else Curr.traillevel = 0;
#if 0
			if (NestXML) {
				if (!Curr.xnlev)
					Curr.xnlev = Curr.contents;
			}
			else Curr.xnlev = 0;
#endif
		}
		if (HXMwr::DITA) {
			if (!Curr.dtmap)
				Curr.dtmap = Curr.contents ? Curr.contents : 1;
		}
		else Curr.dtmap = 0;

		if (ProhibitPrefix && !AllowPrefix)
			Curr.pfx = NULL;
		AllowPrefix = false;
		if (ProhibitSuffix && !AllowSuffix)
			Curr.sfx = NULL;
		AllowSuffix = false;

		if (Curr.tstag)  // back to original tag name (if DITA)
			Curr.stag = Curr.tstag;

		if (Curr.stype == 1) {  // for para styles only
			if (Curr.list)
				Curr.hpname = (Curr.list < 11) ? "li" : (Curr.listdd ? "dd" : "dt");
			else if (DCini::ProcIniSect("ParaTags")
			 || DCini::ProcIniSect("ParaStyles")) {
				if (!Curr.hpname)
					Curr.nopara = true;
				else {
					if ((Curr.hcname = GetStyleClassName(Curr.hpname)) != NULL)
						Curr.hpclass = true;
					if (!stricmp(Curr.hpname, "pre")
					 || ((strlen(Curr.hpname) > 3)
					  && !strnicmp(Curr.hpname, "pre ", 4)))
						Curr.preform = true;
					if (!stricmp(Curr.hpname, "script"))
						Curr.notags = true;
				}
			}
			Curr.hcname = NameCssClass(Curr.hpclass? Curr.hcname :
																 ((ClassIsTag && Curr.hpname) ? Curr.hpname :
			                            Curr.stag));
			DCini::ProcIniSect("Classes");
			DCini::ProcIniSect("ParaClasses");
			if ((Curr.plclass == (char *) 1)
			 && !DCini::ProcIniSect("HtmlStyleParaLinkClass")
			 && !DCini::ProcIniSect("StyleParaLinkClass")
			 && !DCini::ProcIniSect("ParaStyleParaLinkClass"))
				Curr.plclass = Curr.hcname;
		}
		if (Curr.stype == 2) {
			if (DCini::ProcIniSect("CharTags")
			 || DCini::ProcIniSect("CharStyles")) {
				if ((Curr.hscname = GetStyleClassName(Curr.hsname)) != NULL)
					Curr.hsclass = Curr.hsnclass = true;
			}
			else if (ClassIsTag) {
				Curr.hsname = Curr.hscname = NewName(FormName);
				Curr.hsclass = true;
			}
			Curr.hscname = NameCssClass(Curr.hsclass? Curr.hscname :
																 ((ClassIsTag && Curr.hsname) ? Curr.hsname : Curr.stag));
			DCini::ProcIniSect("Classes");
			DCini::ProcIniSect("CharClasses");
			if ((Curr.plclass == (char *) 1)
			 && !DCini::ProcIniSect("HtmlStyleParaLinkClass")
			 && !DCini::ProcIniSect("StyleParaLinkClass")
			 && !DCini::ProcIniSect("CharStyleParaLinkClass"))
				Curr.plclass = Curr.hcname;
		}

		if (HXMwr::DITA) {
			if (Curr.dstag)  // back to DITA alias
				Curr.stag = Curr.dstag;
			SetDITATags();
		}
		if (HXMwr::DocBook) {
			if (Curr.dstag)  // back to DocBook alias
				Curr.stag = Curr.dstag;
			SetDocBookTags();
		}
	}

	if ((HelpType == 'M')
	 && (RefFileType == 'H')
	 && Curr.hpname
	 && (strlen(Curr.hpname) > 1)
	 && ((Curr.hpname[0] == 'H')
		|| (Curr.hpname[0] == 'h'))
	 && (Curr.hpname[1] >= '1')
	 && (Curr.hpname[1] <= '9')
	 && (Curr.hpname[2] == '\0'))
		Curr.hcwtoc = true;

	char *fmtname = Curr.stag;
	char *fmt2 = Curr.stag;
	align = Curr.algn;
	if (Curr.sideh > 1)  // in side head col
		Curr.algn = 0;     // turn off in CSS

	if (Curr.stype == 1) {
		if ((Curr.hcname == NULL)
		 && (ClassIsTag
			|| (WriteClassAttributes
			 && !Curr.notags
			 && !Curr.comment)))
			Curr.hcname = fmtname = NameCssClass(Curr.stag);
		else if (Curr.hcname)
			fmtname = Curr.hcname;
	}
	else if (Curr.stype == 2) {
		if ((Curr.hscname == NULL)
		 && (ClassIsTag
			|| (WriteClassAttributes
			 && !Curr.notags
			 && !Curr.comment)))
			Curr.hscname = fmtname = NameCssClass(Curr.stag);
		else if (Curr.hscname)
			fmtname = Curr.hscname;
	}

	fmtvals *fv = NULL;
	fv = GetFormatFileInfo(fmtname, &Curr);  // override Curr with format info
	if (!fv)
		fv = GetFormatFileInfo(fmt2, &Curr);  // try original name

	if (((WriteCssStylesheet == 'A')
		|| ((WriteCssStylesheet == 'O') && CssFile))
	 && !Curr.notags
	 && ((Curr.stype != 2)
	  || (Curr.hsname != NULL))
	 && !Curr.comment)
		WriteCssStyle(fv, &Curr);
	Curr.algn = align;

	if (!UseFormatTypo
	 || ((Curr.stype == 1) && !UsePFormatTypo)
	 || ((Curr.stype == 2) && !UseCFormatTypo)) {
		Curr.obold = Curr.bold = 1;
		Curr.oital = Curr.ital = 1;
		Curr.oline = Curr.line = 1;
		Curr.ostrike = Curr.strike = 1;
		Curr.base= 1;
	}

  *form = Curr;
  FormList.add(form, FormNum);
  FormRefList.append(DCLFormNum, FormNum);
  FormNameList.add(FormName, FormNum);
  FormName = NULL;
  if (form->tcount)
    FormTabList.add(form->tabs, FormNum);
}


void
HTMwr::SetDITATags(void)
{
	DCini::ProcIniSect("DITATags");
	DCini::ProcIniSect("DITATypographics");
	if (Curr.stype == 1) {  // for para styles only
		DCini::ProcIniSect("DITAParaTags");
		if (!Curr.dtcname)
			Curr.dtcname = HXMwr::UseFormatAsTag ? Curr.hcname : HXMwr::DefDITAPar;
		DCini::ProcIniSect("DITAParaTypographics");
		if (Curr.dtptyp)
			HXMwr::DITASetTypographics(true);
	}
	if (Curr.stype == 2) {
		DCini::ProcIniSect("DITACharTags");
		if (!Curr.dtscname)
			Curr.dtscname = HXMwr::UseFormatAsTag ? Curr.hscname : HXMwr::DefDITAChar;
		DCini::ProcIniSect("DITACharTypographics");
		if (Curr.dtctyp)
			HXMwr::DITASetTypographics(false);
	}
}


void
HTMwr::SetDocBookTags(void)
{
	DCini::ProcIniSect("DocBookTags");
	DCini::ProcIniSect("DocBookTypographics");
	if (Curr.stype == 1) {  // for para styles only
		DCini::ProcIniSect("DocBookParaTags");
		if (!Curr.dtcname)
			Curr.dtcname = HXMwr::UseFormatAsTag ? Curr.hcname : HXMwr::DefDITAPar;
		//DCini::ProcIniSect("DocBookParaTypographics");
		//if (Curr.dtptyp)
		//	DITASetTypographics(true);
	}
	if (Curr.stype == 2) {
		DCini::ProcIniSect("DocBookCharTags");
		if (!Curr.dtscname)
			Curr.dtscname = HXMwr::UseFormatAsTag ? Curr.hscname : HXMwr::DefDITAChar;
		//DCini::ProcIniSect("DocBookCharTypographics");
		//if (Curr.dtctyp)
		//	DITASetTypographics(false);
	}
}


char *
HTMwr::GetStyleClassName(char *nm)
{
	if (!nm || (*nm == '\0'))
		return NULL;

	char *cpt = NULL;
	if ((cpt = strstr(nm, " class=")) != NULL) {
		cpt += 7;
		if (*cpt == '"')
			cpt++;
		char *scpt = cpt;
		while (!((*cpt == '\0') || (*cpt == ' ') || (*cpt == '"')))
			cpt++;
		return NewName(scpt, cpt - scpt);
	}
	return NULL;
}


char HTMwr::EuroChars[EuroCharMax] = {
 'A', 'A', 'A', 'A', 'A', 'A', 0, 'C',
 'E', 'E', 'E', 'E', 'I', 'I', 'I', 'I',
 'D', 'N', 'O', 'O', 'O', 'O', 'O', 0,
 'O', 'U', 'U', 'U', 'U', 'Y', 0, 0,
 'a', 'a', 'a', 'a', 'a', 'a', 0, 'c',
 'e', 'e', 'e', 'e', 'i', 'i', 'i', 'i',
 'd', 'n', 'o', 'o', 'o', 'o', 'o', 0,
 'o', 'u', 'u', 'u', 'u', 'y', 0, 'y'
};


char *
HTMwr::MakeID(char *txt, char *buf, uns buflen, char space,
 bool undscr, bool nodigit, bool lowercase)
{
	uns i = 0;
	unc ch = 0;
	char nch = 0;

	if (!buflen)
		buflen = strlen(txt) + 2;  // allow for leading 'x'
	if (!buf)
		buf = NewName(buflen);

	while ((*txt != '\0') && (i < (buflen - 1))) {
		if (*txt == ' ') {
			if (space) 
				buf[i++] = space;
		}
		else if ((*txt < 0x7E)
		 && (*txt > 0x20)
		 && (isalnum((int)(*txt & 0x7F))
		  || (undscr && (*txt == '_')))) {
			if (nodigit
			 && isdigit((int)(*txt & 0x7F)))
				buf[i++] = 'x';
			buf[i++] = *txt;
		}
		else {
			ch = (unc) (*txt);
			if (ch >= 0xC0) {
			  if ((nch = EuroChars[ch - 0xC0]) == 0) {
					switch (ch) {
						case 0xC6: // AE
							buf[i++] = 'A';
							buf[i++] = 'E';
							break;
						case 0xDF: // ss
							buf[i++] = 's';
							buf[i++] = 's';
							break;
						case 0xE6: // ae
							buf[i++] = 'a';
							buf[i++] = 'e';
							break;
						default:
							break;
					}
				}
				else
					buf[i++] = nch;
			}
		}

		txt++;
		nodigit = false; // applies only to starting char
	}
	buf[i] = '\0';

	if (lowercase) {  // lower-case everything
		txt = buf;
		while (*txt) {
			*txt = tolower(*txt);
			txt++;
		}
	}
	return buf;
}


char *
HTMwr::NameCssClass(char *name)
{
	if (!name || (*name == '\0'))
		return NULL;

	static char buf[CssClassNameMax];

	MakeID(name, buf, CssClassNameMax, ClassSpaceChar, false, true, LowerCaseCSS);

	return NewName(buf);
}


uns HTMwr::CharFormLevel = 0;
uns HTMwr::HyperFormLevel = 0;
uns HTMwr::CurrFormNum = 0;


void
HTMwr::CheckForm(HTMctl *cp)
{
	if (LastPage <= LastMasterPage)
		return;

  uns formnum = (uns) cp->siz();
	bool isInit = (cp->dat() == 1) ? true : false;

	if (!isInit   // only inherit format can end hyperlink
	 && !ParaFullLink) {
		if (HyperForm // hyper ending, may be new or restore
		 && ((XMLSource && !formnum && (CharFormLevel == HyperFormLevel))
		  || (!XMLSource && (formnum != HyperFormNumber)))) {
			HyperForm = false;
			HyperFormNumber = 0;
			HyperFormLevel = 0;
			HyperFormEndID = (long) cp;
			if (HyperJump) {
				HyperFormEndList.add((long) cp);
				HyperJump = HyperJumpStarting = false;
			}
			HyperFormEnding = true;
		}
	}

	if (formnum == 0) { 
		if (isInit) { // init store may need update
			if (HyperColor
			 && !ParaFullLink)
				ParaHyperColor = true;
			ParaWhiteText = WhiteText;
		}
		else {    // inherit restore
			if (!InPara)
				return;
			if (CharRefNum) {  // collect char attr markers
				ProcCondCharEnd();
				MarkList.all(GetCharAttrs);
				if (InGlossTitle) {
					SetGlossTitle();
					InGlossTitle = false;
				}
				CharAttrLp = NULL;
				CharRefNum = 0;
			}
			if (!ParaFullLink) {
				if (HyperColor       // was in color
				 && !ParaHyperColor)
					ProcHyperColorEnd(cp);

				if (ParaHyperColor) // restoring color
					ProcHyperColorStart(cp);
			}

			WhiteText = ParaWhiteText;

			if (!ParaFullLink
			 && !XMLSource) {
				HyperForm = true;
				HyperFormStartID = (long) cp;
				if (HyperJumpStarting)
					HyperFormStartList.append(HyperJumpID, HyperFormStartID);
				else if (HyperJump) {
					HyperParaList.append(HyperJumpID, HyperParaID);
					HyperFormEndList.add(HyperFormStartID);
					HyperJump = false;
					HyperFormNumber = 0;
					HyperFormLevel = 0;
				}
				HyperFormEnding = false;
			}
			CharFormLevel--;
		}
		return;
	}

	pprop	*cform = (pprop *) FormList.find(FormRefList(formnum));

	if (cform == NULL)
		return;

	WhiteText = HideWhite && (cform->colr == 3);

	if (isInit) {  // init format
		Curr = *cform;
		CurrFormNum = formnum;
		FormName = cform->stag;

		DCini::CurrPendList.all(DCini::SetCurrID);

		if (UseHyperColor)
			ParaHyperColor = (cform->colr > 2) ? true : false;

		SplitLevel = cform->splitlevel;
		NoSplit = cform->nosplit;

		if (HXMwr::HXML) {
			HXMwr::DITAOpenBeforeArr = cform->dtopb;
			HXMwr::DITAOpenAfterArr = cform->dtopa;
			HXMwr::DITACloseBeforeArr = cform->dtclb;
			HXMwr::DITACloseAfterArr = cform->dtcla;
		}
		if (HXMwr::DITA) 
			HXMwr::ProcDITATopicStart(cform);

		if (!InTbl || AllowTbSplit) {
			if (cform->split
			 && !SplitHere
			 && !UseChunking
			 && SplittingOK()) {
				SplittingFile = true;
				SplitHere = true;
			}
		}
		if ((!InTbl
		  || AllowTbTitle)
		 && cform->title) {
			TitlePrefix = cform->ttlpx;
			TitleSuffix = cform->ttlsx;
			TitleHere = true;
			StoreText = true;
		}
		if ((Contents = cform->contents) != 0) {
			ContentsID++;
			StoreText = true;
			NoContLink = cform->noclink;
			NoContRef = cform->noref;
		}
		ContentsWin = cform->cntwin; // also used for index

		if ((ALinkHere = cform->alink) == true)
			StoreText = true;
		if ((TStoreName = cform->tstore) != NULL)
			TStoreHere = StoreText = true;
		if (OmniHelp && UseFTS)
			StoreText = true;
		if (UseGlossary && cform->gloss) {
			GlossHere = true;
			StoreText = true;
		}

		if ((FileNameHere = cform->flname) == true) {
			NamePrefix = cform->flnpx;
			NameSuffix = cform->flnsx;
			NameHere = true;
			StoreText = true;
		}
		if ((FinalIniNameHere = cform->finame) == true) {
			NameHere = true;
			StoreText = true;
		}
		if ((IniPartNameHere = cform->ipname) == true) {
			NameHere = true;
			StoreText = true;
		}

		if ((MetaName = cform->meta) != NULL)
			StoreText = true;

		if ((ConfigHere = cform->config) != NULL)
			StoreText = true;


		if (!InTbl || AllowTbSplit) {
			if ((PgBreaks == 'S')
			 && ((cform->pbreak == 4)
			  || (cform->pbreak == 5)
			  || (cform->pbreak == 6))
			 && !SplitHere
			 && !UseChunking
			 && SplittingOK()) {
				SplittingFile = true;
				SplitHere = true;
				TitleHere = StoreText = true;
			}
		}

		InRaw = cform->raw;

		if ((AttrHere = cform->attr) == true) {
			AttrMarkerName = cform->attrname;
			StoreText = true;
		}

		if ((cform->extract & 8) == 8)
			ExtractEnable = true;
		if ((cform->extract & 16) == 16)
			ExtractEnable = false;
		if (!InTbl
		 && Extracting) {
			if ((cform->extract & 2) == 2)
				ExtrFinishing = true;
			if ((cform->extract & 4) == 4)
				ExtrEnding = true;
		}
		if (!InTbl
		 && !SplitHere
		 && ExtractEnable
		 && ((cform->extract & 1) == 1)) {
			ExtrStarting = true;
			ExtrStartTag = cform->stag;
			ExtrStartForm = cform;
		}

		SkipAnums = cform->anskip || (!UseAnums && !cform->anuse);
		if (cform->cntform)
			KeepParaUID = true;
		ParaWhiteText = WhiteText;
		ParaFullLink = cform->plink;

		if ((TrailCurrLevel = cform->traillevel) > 0) {
			TrailPrefix = cform->trailpx;
			TrailSuffix = cform->trailsx;
		}
		else
			TrailPrefix = TrailSuffix = NULL;

		LocalTOCLevel = cform->loctoclevel;
		if (TrailCurrLevel || LocalTOCLevel)
			StoreText = true;

		if (cform->txframes)
			TextFrameIsText = true;

		if (cform->keepn
		 && !ExtrEnding)
			ParaKeepNext = true;

		if ((cform->keep || PrevParaKeepNext)
		 && !SplitHere
		 && !ExtrStarting
		 && PrevParaFrameAfter
		 && GraphANamesFirst)   // set to put anames out before graphic
			FrameLinks = PrevParaFrameAfter;

		if (!FileStarted
		 && InDocBody)
			StartingParaRef = ParaRefNum;

		if (cform->cstore)
			CodeStoreParaList.add(cform->cstore, ParaRefNum);

		CharFormLevel = HyperFormLevel = 0;
	}
	else  { // inherit format
		CharFormLevel++;
		CondCharPending = true;
		if (InCharCond)
			ProcCondCharEnd(); // end previous one
		CharRefNum = (long) cp;

		if (cform->glosstitle) {
			InGlossTitle = true;
			GlossTitleCount = 0;
		}

		if (!ParaFullLink
		 && !HyperJump) {
			HyperForm = true;
			HyperFormStartID = (long) cp;
			if (HyperJumpStarting)
				HyperFormStartList.append(HyperJumpID, HyperFormStartID);
			else if (HyperJump
			 && (formnum != HyperFormNumber)) {
				HyperParaList.append(HyperJumpID, HyperParaID);
				HyperFormEndList.add(HyperFormStartID);
				HyperJump = false;
			}
			HyperFormEnding = false;
			HyperFormNumber = formnum;
			HyperFormLevel = CharFormLevel;
		}
		if (InXref
		 && cform->secwin
		 && !XrefSecWin) {  // first one wins
			XrefSecWin = true;
			XrefSecWinName = cform->secwin;
		}
		if (XrefFormatIsXrefClass && !InXref)
			XrefName = FormNameList.find(formnum);
	}

	if (UseHyperColor
	 && !ParaFullLink) {
		if (HyperColor
		 && (cform->colr == 2))
			ProcHyperColorEnd(cp);
		if ((HyperForm == false)
		 && (cform->colr > 2))
			ProcHyperColorStart(cp);
	}
}


bool HTMwr::InGlossTitle = false;
unc HTMwr::GlossTitleBuf[GlossTitleBufLen];
uns HTMwr::GlossTitleCount = 0;
char *HTMwr::GlossTitlePath = "./";

void
HTMwr::AddToGlossTitle(DCctl *cp)
{
	unc ch;

	if (cp->dt() == imnem) { // single char
		if ((cp->siz() < 0x7F)
		 && ((ch = (unc) cp->siz()) > 0x2F)
		 && isalnum(ch)
		 && (GlossTitleCount < GlossTitleBufLen))
			GlossTitleBuf[GlossTitleCount++] = ch;
	}
	else { //string
		for (unl i = 0; i < cp->siz(); i++) {
			ch = ((unc *) cp->ex())[i];
			if ((ch < 0x7F)
			 && (ch > 0x2F)
			 && isalnum(ch)
			 && (GlossTitleCount < GlossTitleBufLen))
				GlossTitleBuf[GlossTitleCount++] = ch;
		}
	}
}

void
HTMwr::SetGlossTitle(void)
{
	// for GlossTitle, set CharAttrLp entry for CharTitle
	//  if not already present *****
	long lpcnt = 1;
	char *marker = NULL;

	if (!GlossTitleCount)
		return;
	GlossTitleBuf[GlossTitleCount] = '\0';

	if (CharAttrLp) {
		if (CharAttrLp->getwild("title=*"))
			return;
		lpcnt += CharAttrLp->count();
	}
	else {
		CharAttrLp = new DCnlist;
		CharAttrLists.add(CharAttrLp, CharRefNum);
	}

	if (GlossTitlePath == NULL)
		GlossTitlePath = "./";

	if (IniFile->Section("GlossTitles")
	 && IniFile->Find((const char *) GlossTitleBuf)) {
		char *gt = IniFile->StrRight();
		marker = NewName("title=\"", strlen(gt) + 10);
		strcat(marker, gt);
		strcat(marker, "\"");
	}
	else if (IniFile->Section("GlossFiles")
	 && IniFile->Find((const char *) GlossTitleBuf)) {
		char *gf = IniFile->StrRight();
		marker = NewName("title=\"<$",
		 strlen(gf) + strlen(GlossTitlePath) + 18);
		strcat(marker, GlossTitlePath);
		if (marker[strlen(marker) - 1] != '/')
			strcat(marker, "/");
		strcat(marker, gf);
		strcat(marker, ".txt>\"");
	}
	else {
		marker = NewName("title=\"<$",
		 GlossTitleCount + strlen(GlossTitlePath) + 18);
		strcat(marker, GlossTitlePath);
		if (marker[strlen(marker) - 1] != '/')
			strcat(marker, "/");
		strcat(marker, (const char *) GlossTitleBuf);
		strcat(marker, ".txt>\"");
	}
	CharAttrLp->add(marker, lpcnt);
}


void
HTMwr::UseForm(HTMwfile *wf, HTMctl *cp)
{
  uns formnum = (uns) cp->siz();

	if (formnum == 0) {	
		if (cp->dat() == 1) // for "Init Store" after para overrides
			ParaForm = Curr;
		else if (cp->dat() == 2) { // for "Inherit Restore"
			if (Curr.list
			 && (InFormPx
				|| (InAnumGroup
				 && !Curr.anuse)))
				return;
			if ((InAnumGroup || InAnumVal)
			 && (Curr.anskip || (!UseAnums && !Curr.anuse)))
				return;
			if (XMLSource
			 && InlineStackLevel)
				RestoreInline(wf);
			else if (XMLSource
			 && wf
			 && InHyper
			 && (InlineHyperLevel >= InlineStackLevel)) {
				EndAHref(wf);
				CharBlockState = CbEnd;
				EndOneCharBlock(wf);
				RestoreForm();
			}
			else if (XMLSource) {
				CharBlockState = CbEnd;
				EndOneCharBlock(wf);
				RestoreForm();
			}
			else
				RestoreForm();
			CheckHyper(cp);
		}
		DCini::CurrPendList.all(DCini::GetCurrSettings);
		return;
	}

	pprop	*cform = (pprop *) FormList.find(FormRefList(formnum));
	if (cform == NULL)
		return;

  if (cp->dat() == 1)  // init format
    InitForm(wf, cform);
  else if (cp->dat() == 2) { // inherit format
		if (Curr.list
		 && (InFormPx
			|| (InAnumGroup
			 && !Curr.anuse)))
			return;
		if ((InAnumGroup || InAnumVal)
		 && (Curr.anskip || (!UseAnums && !Curr.anuse)))
			return;
		if (Curr.stype == 1) { // not in char format
			ParaForm.macpos = Curr.macpos;  // save any added macros
			ParaForm.maccb = Curr.maccb;    // before char format wipes them
			ParaForm.maccs = Curr.maccs;
			ParaForm.macce = Curr.macce;
			ParaForm.macca = Curr.macca;
			ParaForm.maccr = Curr.maccr;
		}

		if (XMLSource
		 && (Curr.stype > 1))
			SetInline(cform);
		else {
			if ((CharBlockState == CbStarted)
			 || (CharBlockState == CbChanged)
			 || (CharBlockState == CbEnd))
				CharBlockState = CbChanged;
			else
				CharBlockState = CbStart;
			InlineStackLevel = InlineHyperLevel = 0;
		}
		CharBlockMacro = cform->cmacpos;
		CurrCSName = cform->stag;
		CurrCSForm = cform;

		CharRefNum = (long) cp;
		CharAttrLp = (DCnlist *) CharAttrLists.find(CharRefNum);

		InheritForm(cform);
		LastCharForm = cform;
	}

	DCini::CurrPendList.all(DCini::GetCurrSettings);
	CheckHyper(cp);
}


// for XML stacked char formats
DCvlist HTMwr::InlineStack;
DCvlist HTMwr::InlineSrcStack;
DCilist HTMwr::InlineRefStack;
long HTMwr::InlineStackLevel = 0;
long HTMwr::InlineHyperLevel = 0;
pprop *HTMwr::LastCharForm = NULL;

void 
HTMwr::SetInline(pprop *cform)
{
	bool startprev = false;

	if (CharBlockState == CbStart) {
		StartCharBlock(HTMFile);
		startprev = true;
	}
	InlineStackLevel++;  // level 1 is second inline invoked
	pprop *oform = new pprop;
	*oform = Curr;
	InlineStack.append(oform, InlineStackLevel);
	InlineSrcStack.append(LastCharForm, InlineStackLevel);
	CharBlockState = CbStart;
	InlineRefStack.append(CharRefNum, InlineStackLevel);
}


void 
HTMwr::RestoreInline(HTMwfile *wf)
{
	if (InlineStackLevel < 1)
		return;

	if (CharBlockState == CbStarted) {
		if (wf
		 && InHyper
		 && (InlineHyperLevel >= InlineStackLevel))
			EndAHref(wf);
		CharBlockState = CbEnd;
		EndOneCharBlock(wf);
	}

	pprop *oform = (pprop *) InlineStack.find(InlineStackLevel);
	if (oform) {
		unc hyp = Curr.hyper;
		Curr = *oform;
		Curr.hyper = hyp;
		delete oform;
		InlineStack.remove(InlineStackLevel);
	}
	pprop *cform = (pprop *) InlineSrcStack.find(InlineStackLevel);
	if (cform) {
		CharBlockMacro = cform->cmacpos;
		CurrCSName = cform->stag;
		CurrCSForm = cform;
		InlineSrcStack.remove(InlineStackLevel);
	}
	CharRefNum = (long) InlineRefStack.find(InlineStackLevel);
	if (CharRefNum) {
		CharAttrLp = (DCnlist *) CharAttrLists.find(CharRefNum);
		InlineRefStack.remove(InlineStackLevel);
	}
	if (wf
	 && InHyper
	 && (InlineHyperLevel == InlineStackLevel))
		EndAHref(wf);

	CharBlockState = CbStarted;
	LastCTagName = Curr.hsname;

	//if (--InlineStackLevel)
	LastCharForm = (pprop *) InlineSrcStack.find(InlineStackLevel);
	InlineStackLevel--;
	//else
	//	LastCharForm = NULL;
}


void
HTMwr::RestoreForm()
{
	if (CharBlockState == CbStarted)
		CharBlockState = CbEnd;
	else
		CharBlockState = CbNone;
	LastCSName = CurrCSName;
	CurrCSName = NULL;
	LastCSForm = CurrCSForm;
	CurrCSForm = NULL;

	//InheritForm(&ParaForm);
	RestoreCurrForm();
  Curr.secwin = ParaForm.secwin; // even if NULL
	Curr.target = ParaForm.target;
	Curr.tabrep = ParaForm.tabrep;

	if (Curr.cmacpos & MacReplace)  // get rid of leftover tabs from deleted cform
		CurrTab->tpos = 0;

	Curr.macpos = ParaForm.macpos;
	//CharBlockMacro = Curr.cmacpos = ParaForm.cmacpos;
}


void
HTMwr::UseFTag(HTMctl *cp)
{
#if 0
  uns formnum = FormNameList((char *) cp->ex());
  pprop *cform = (pprop *) FormList.find(formnum);
  if (cform == NULL)
    return;

  if (cform->stype == 1)
    InitForm(cform);
  else if (cform->stype == 2)
    InheritForm(cform);
#endif
}


void
HTMwr::UseSpecFmtTag(HTMwfile *wf, HTMctl *cp)
{
	// cp->dat() is format type, cp->ex() is name  ****
	if (cp->dat() == 6) {  // title span
		if (!HXMwr::HXML) {  // not for DITA
			WriteText(wf);
			if (cp->siz() > 2) {// has text content
				wf->startBlock("span");
				wf->strAttr("title", NewName((char *) cp->ex(), cp->siz()));
				wf->endElem(true);
			}
			else {  // no text, ending
				wf->endBlock("span");
			}
		}
	}
}



void
HTMwr::InitForm(HTMwfile *wf, pprop *cform)
{
#if 0
  if (Curr.tcount && !FormTabList(Curr.tabs)) {
    delete Curr.tabs;
    Curr.tabs = NULL;
  }
#endif
  Curr = *cform;

	char *nm = NULL;
	if (HXMwr::HXML
	 && (((nm = HXMwr::DITAParaAliasList.find(ParaRefNum)) != NULL)
		// see if dstag is a macro and if so eval it to a string
	  || (Curr.dstag && (*Curr.dstag == '<') && ((nm = DCmacr::GetMacro(Curr.dstag)) != NULL)))
	 && (!Curr.dstag
	  || (nm && strcmp(Curr.dstag, nm)))) {
		if (!Curr.tstag)
			Curr.tstag = Curr.stag;
		char *nme = nm + strlen(nm) - 1;
		while (*nme == ' ')
			*nme-- = '\0';
		Curr.stag = Curr.dstag = nm;
		if (HXMwr::DITA) {
			DCini::ProcIniGroup("DStyleSet");
			SetDITATags();
		}
		else if (HXMwr::DocBook) {
			DCini::ProcIniGroup("DBStyleSet");
			SetDocBookTags();
		}
		Curr.stag = Curr.tstag;
	}
	if (wf) {
		if (Curr.nowrap)
			wf->SetNoWrap(true);
		else
			wf->SetNoWrap(NoWrap);
	}

	ParaForm = Curr; // set up for future "Inherit Restore"
	CharBlockState = CbNone;
	CharBlockMacro = 0;
	if (Curr.config)
		DCini::ProcParaConfig(ParaRefNum);
	if (HXMwr::HXML
	 && (Curr.dtcname != NULL))
		HXMwr::DITANoBlock = !stricmp(Curr.dtcname, "no") ? true : false;
}


void
HTMwr::InheritForm(pprop *cform)
{
	// make Curr.cstag identify the char format if any
	bool wasChar = false;
	if (Curr.cstag) {  // restore or change para format
		Last.cstag = Curr.cstag;
		Curr.cstag = NULL;
		wasChar = true;
		//LastCF = CForm;
		//CForm = NoForm;
	}
	if (cform->stype == 2) {  // use char format
		Curr.cstag = cform->stag;
		//CForm = *cform;
	}

	Curr.stype = cform->stype;  // 2 means in char span, else 1

	Curr.hsname = cform->hsname;  // even if null
	Curr.hscname = cform->hscname;
	Curr.hsclass = cform->hsclass;
	Curr.hsnclass = cform->hsnclass;

	Curr.dtscname = cform->dtscname;  // DITA char tag and typographics
	Curr.dtctyps = cform->dtctyps;
	Curr.dtctype = cform->dtctype;

	if ((Curr.stype == 2)  // to or from char format
	 || wasChar) {
		Curr.keeplink = cform->keeplink;
	}

	if (Curr.stype == 2) { // to char format
		Curr.cmacpos = cform->cmacpos;  // was cform->macpos
		// set style macros
		Curr.cmaccep = Curr.cmacce;
		Curr.cmaccap = Curr.cmacca;
		Curr.cmaccb = cform->cmaccb;
		Curr.cmaccs = cform->cmaccs;
		Curr.cmacce = cform->cmacce;
		Curr.cmacca = cform->cmacca;
		Curr.cmaccr = cform->cmaccr;
	}
	else
		Curr.cmacpos = 0;

	// used only as para properties, not char
	//Curr.notags = cform->notags;
	//Curr.raw = cform->raw;

  if (cform->secwin)
    Curr.secwin = cform->secwin;
  if (cform->target)
    Curr.target = cform->target;
  if (cform->tabrep)
    Curr.tabrep = cform->tabrep;
  if (cform->noref || !ParaForm.noref)
   Curr.noref = cform->noref;

  if (cform->fnum) {
    Curr.fnum = cform->fnum;
    Curr.fmap = cform->fmap;
    Curr.fsym = cform->fsym;
	}

  if (cform->osize)
    Curr.osize = cform->osize;
  if (cform->obold)
    Curr.obold = cform->obold;
  if (cform->oital)
    Curr.oital = cform->oital;
  if (cform->oline)
    Curr.oline = cform->oline;
  if (cform->ostrike)
    Curr.ostrike = cform->ostrike;

  if (cform->fsize)
    Curr.fsize = cform->fsize;
  if (cform->bold)
    Curr.bold = cform->bold;
  if (cform->ital)
    Curr.ital = cform->ital;
  if (cform->line)
    Curr.line = cform->line;
  if (cform->strike)
    Curr.strike = cform->strike;

  if (cform->base)
    Curr.base = cform->base;
  if (cform->caps)
    Curr.caps = cform->caps;
  if (cform->scaps)
    Curr.scaps = cform->scaps;
	if (cform->colr)
		Curr.colr = cform->colr;
	if (cform->nocolor)
		Curr.nocolor = cform->nocolor;
	if (cform->nosize)
		Curr.nosize = cform->nosize;
}


void
HTMwr::RestoreCurrForm(void)
{
	pprop *cform = &ParaForm;

	Last.cstag = Curr.cstag;
	Curr.cstag = NULL;

	Curr.stype = cform->stype;  // 2 means in char span, else 1

	Curr.hsname = cform->hsname;  // even if null
	Curr.hscname = cform->hscname;
	Curr.hsclass = cform->hsclass;
	Curr.hsnclass = cform->hsnclass;

	Curr.dtscname = cform->dtscname;  // DITA char tag and typographics
	Curr.dtctyps = cform->dtctyps;
	Curr.dtctype = cform->dtctype;

	Curr.keeplink = cform->keeplink;

	Curr.cmacpos = 0;

  Curr.secwin = cform->secwin;
  Curr.target = cform->target;
  Curr.tabrep = cform->tabrep;
  Curr.noref = cform->noref;

  Curr.fnum = cform->fnum;
  Curr.fmap = cform->fmap;
  Curr.fsym = cform->fsym;

  Curr.osize = cform->osize;
  Curr.obold = cform->obold;
  Curr.oital = cform->oital;
  Curr.oline = cform->oline;
  Curr.ostrike = cform->ostrike;

  Curr.fsize = cform->fsize;
  Curr.bold = cform->bold;
  Curr.ital = cform->ital;
  Curr.line = cform->line;
  Curr.strike = cform->strike;

  Curr.base = cform->base;
  Curr.caps = cform->caps;
  Curr.scaps = cform->scaps;
	Curr.colr = cform->colr;
	Curr.nocolor = cform->nocolor;
	Curr.nosize = cform->nosize;
}



// text properties


bool HTMwr::InTabDef;
bool HTMwr::TabsChanged;
uns HTMwr::TabCount;
uns HTMwr::TabFillChar;
tinfo *HTMwr::CurrTab;
DCvlist *HTMwr::TabList;

DClist HTMwr::SideHFlowList;
bool HTMwr::InRunGroupDef = false;
bool HTMwr::SideHUsed = false;



void
HTMwr::SetCurrProps(HTMwfile *wf, HTMctl *cp)
{
  if (cp->rp == NULL)
    return;

  switch ((cp->rp)->write_op) {
    case ignore:
      break;
    case skip:
      break;

    case puttx:
      break;

		case formch:
			if (!InPara)
				break;
    case formcall:
			if (!InIdxGroup)
				UseForm(wf, cp);
      break;
    case ftaguse:
  		if (!InIdxGroup)
				UseFTag(cp);
      break;
		case ftagspc:
  		if (!InIdxGroup)
				UseSpecFmtTag(wf, cp);
      break;

		case formpx:
			InFormPx = true;
			break;
		case formrest:
			InFormPx = false;
			EndingAnum = true;
			RestoreForm();
			break;

    case formtbl:
      UseTblForm(cp);
      break;
		case tbclvalg:
			Curr.valgn = (unc) cp->siz() + 1;
			if (Cell)
				Cell->valgn = Curr.valgn;
			break;

    case putfont:
      Curr.fnum = GetFontNum(cp);
			Curr.fmap = FontMapList.find(Curr.fnum);
			Curr.fsym = ((finfo *) (FontList.find(Curr.fnum)))->ftsym;
      // Curr.fnum = (uns) FontNameList(GetFontName(cp));
      break;
    case putfsize:
			Curr.psize = GetTwips(cp->siz(), ft_unit);
      Curr.fsize = GetFontSize(cp);
      break;
    case putbold:
      Curr.bold = (cp->siz() > 2) ? 2 : 1;
			Curr.fweight = (unc) cp->siz();
      break;
    case putital:
      Curr.ital = cp->siz() ? 2 : 1;
      break;
    case putbase:
      Curr.base = (unc) cp->siz() + 1;
      break;

    case putcaps:
      switch (cp->siz()) {
        case 0:
          Curr.caps = 1;
          Curr.scaps = 1;
          break;
        case 1:
          Curr.caps = 3;
          Curr.scaps = 1;
          break;
        case 2:
          Curr.caps = 2;
          Curr.scaps = 1;
          break;
        case 3:
          Curr.caps = 4;
          Curr.scaps = 1;
          break;
        case 4:
          Curr.caps = 1;
          Curr.scaps = 2;
          break;
        default:
          break;
      }
      break;

    case putrevb:
			switch (cp->siz()) {
				case 0:  // none
					Curr.strike = 1;
					break;
				case 1:  // chbar
				case 3:
					Curr.strike = 3;
					break;
				case 2:  // strikethrough
					Curr.strike = 2;
					break;
				default:
					break;
			}
			//if (cp->siz() < 3)
	    //  Curr.strike = (cp->siz() == 2) ? 2 : 1;

      break;
    case putline:
			switch (cp->siz()) {
				case 0:  // none
					Curr.line = 1;
					break;
				case 3:  // over
					Curr.line = 3;
					break;
				default: // single, double, numeric
					Curr.line = 2;
					break;
			}
      //Curr.line = (unc) cp->siz() + 1;
      break;

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
      if (cp->dat() > 3)
        Curr.algn = 1;
      else Curr.algn = (unc) cp->dat();
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
					if (RuninType == 'R')
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
			if (cp->siz()
			 && (RuninType == 'R'))
				Curr.runpunct = NewName((char *) cp->ex(), (size_t) cp->siz());
			break;
#if 0
		case putrunin:
			if (RuninType != 'R')
				break;
			Curr.rungroup = new HTMgroup(*cp);
		  if (DCLlevel < LevelMax)
				GroupStack[DCLlevel] = Curr.rungroup;
			InRunGroupDef = true;
			break;
		case eptrunin:
			InRunGroupDef = false;
			break;
#endif

    case putcolr:
			if (!Curr.nocolor)
	      ProcTextColor(cp);
      break;

    case putbreak:
      if (cp->siz() == 1) {
        if (cp->dat() < 7)
          Curr.pbreak = cp->dat();
        else if (cp->dat() == 8)
          Curr.pbreak = 0;
      }
      break;

		case putkeep:
			Curr.keep = (bool) cp->siz();
			break;
		case putkeepn:
			Curr.keepn = (bool) cp->siz();
			break;

    case puttdef:
      TabsChanged = true;
      InTabDef = true;
      Curr.tcount = cp->dat();
      TabList = new DCvlist;
      TabCount = 0;
      break;
    case endtdef:
      InTabDef = false;
      Curr.tabs = TabList;
      CurrTab->tpos = 0;
      break;
    case puttab:
			if ((!InTabDef)
			 && Curr.list
			 && (InFormPx          // strip bullet tabs if in list format
				|| (InAnumVal        // also remove anum tabs unless specified
				&& !Curr.anuse)))
				break;
			if ((!InTabDef)
			 && (InAnumGroup || InAnumVal)
			 && (Curr.anskip || (!UseAnums && !Curr.anuse)))
				break;
      CurrTab->tpos = (GetTwips(cp->siz(), pg_unit) - Curr.find) / TabCharWidth;
			if ((cp->dat() == 8)  // make unknown tabs produce spaces
			 && (CurrTab->tpos == 0))
				CurrTab->tpos = 1;
      if (cp->dat() > TabTypeMax)
        CurrTab->ttype = 3;
      else if (cp->dat() > 0)
        CurrTab->ttype = cp->dat() - 1;
      CurrTab->tlead = (unc) TabFillChar;
      if (InTabDef) {
        TabList->add(CurrTab, ++TabCount);
        CurrTab = new tinfo;
      }
      break;
    case tabfill:
      TabFillChar = (uns) cp->siz();
      break;
    case tabfstr:
      TabFillChar = *( (char *) cp->ex());
      break;

		case anstart:
			InAnumVal = true;
			break;
		case anend:
			InAnumVal = false;
			EndingAnum = true;
			break;

		case ansgrp:
			InAnumGroup = true;
			break;
		case anegrp:
			InAnumGroup = false;
			break;

    case setftn:
      FtnMarker = (uns) cp->siz();
      break;

#ifdef UNUSED
    case putidx:
      PutIndexEntry(cp);
      break;
    case idxgrp:
      SaveState();
      StartingIndexGroup = InIndexGroup = true;
      break;
    case idxend:
      RestoreState();
      StartingIndexGroup = InIndexGroup = false;
      break;
#endif

    case setfile:
      SetXrefFileName((char *) cp->ex());
      break;

    case settitle:
      SetXrefTitle(cp);
      break;

		case hypurl:
			InlineHyperLevel = InlineStackLevel;
			break;

    default:
      break;
  }
}



// font processing

uns HTMwr::FontNum = 1;
DCvlist HTMwr::FontList;
DCnlist HTMwr::FontNameList;
DCnlist HTMwr::FontMapList;

char HTMwr::MCDec[10];
char HTMwr::MCHexU[10] = "x00";
char HTMwr::MCHex4[10] = "x00";
char HTMwr::MCHex2[10] = "x";
char HTMwr::MCChr[3];
anyini HTMwr::MapChar = {
 {MCDec, MCHexU, MCHex4, MCHex2, MCChr }, 5
};
bool HTMwr::CharConvertSpaces = false;

char *HTMwr::FontTypes[FTypesMax] = {
  "fnil", "froman", "fswiss", "fmodern", "fscript", "fdecor", "ftech"
};
unc HTMwr::FTypes[FNamesMax] = {
 0, 1, 2, 3,  2, 1, 1, 2,  2, 2, 1, 1,  1, 4, 5, 6,  5
};
char *HTMwr::FontNames[FNamesMax] = {
  "unknown", "Times", "Helvetica", "Courier",
  "AvantGarde", "Bookman", "Garamond", "Helvetica-Black",
  "Helvetica-Light", "Helvetica-Narrow", "Korinna", "NewCenturySchlbk",
  "Palatino", "ZapfChancery", "ZapfDingbats", "Symbol", "Webdings"
};
uns HTMwr::FCharWids[FNamesMax] = { // twips for 12pt en space
  144, 134, 138, 144, 144, 150, 145, 155,
  130, 115, 140, 145, 141, 121, 180, 130, 180
};
bool HTMwr::FixedWid;  // true for fonts 3, 14, 15
uns HTMwr::CharMetrics[CharMetricMax] = {  // std PostScript from ref manual
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


void
HTMwr::ProcFont(HTMctl *cp)
{
  char *fp;
  unc ft;
  finfo *finf;
	bool delfp = false;

  if (cp->dt() == ename) {
    fp = (char *) cp->ex();
    ft = !_strnicmp(fp, "wingdings", 9) ? 5 : 0;
  }
  else {
    if ((cp->siz() > 0) && (cp->siz() < (FNamesMax + 1))) {
      fp = FontNames[cp->siz() - 1];
      if ((ft = FTypes[cp->siz() - 1]) >= FTypesMax)
        ft = 0;
    }
    else {
      fp = FontNames[0];
      ft = 0;
    }
  }
	if (IniFile
	 && IniFile->Section("Fonts")
	 && IniFile->Find(fp)) {  // mapped
		fp = IniFile->StrRight();
		delfp = true;
	}

  if ((Curr.fnum = (uns) FontNameList(fp)) == 0) {
		finf = new finfo;
		finf->fname = NewName(fp);
		finf->ftype = ft;
		finf->ftsym = !_stricmp(finf->fname, "Symbol") ? 1 :
		             (!_stricmp(finf->fname, "Wingdings") ? 2 : 0);
		finf->fnum = ++FontNum;
		FontList.add(finf, FontNum);
		FontNameList.add(finf->fname, FontNum);
		Curr.fnum = FontNum;
		Curr.fsym = finf->ftsym;
	}
	else {
		finf = (finfo *) FontList.find(Curr.fnum);
		if (finf)
			Curr.fsym = finf->ftsym;
	}

  if (((Curr.fmap = FontMapList.find(Curr.fnum)) == NULL)
	 && IniFile
	 && IniFile->Section("MacroFonts")
	 && IniFile->Find(fp)) {  // mapped to section
		Curr.fmap = finf->fmap = IniFile->StrRight();
		FontMapList.add(finf->fmap, Curr.fnum);
	}

	if (delfp)
		DeleteName(fp);
}


uns
HTMwr::GetFontNum(HTMctl *cp)
{
  char *fp;
	uns fnum;

  if (cp->dt() == ename)
    fp = (char *) cp->ex();
  else {
    if ((cp->siz() > 0) && (cp->siz() < (FNamesMax + 1)))
      fp = FontNames[cp->siz() - 1];
    else fp = FontNames[0];
  }

	if (IniFile
	 && IniFile->Section("Fonts")
	 && IniFile->Find(fp)) { // name is mapped
			fp = IniFile->StrRight();
			fnum = (uns) FontNameList(fp);
			DeleteName(fp);
	}
	else fnum = (uns) FontNameList(fp);

  return fnum;
}


uns HTMwr::FontSizeVector[FontSizesMax - 1] =
 { 8, 10, 14, 20, 28, 36 };

uns
HTMwr::GetFontSize(HTMctl *cp)
{
  // get font size in points
  uns pts = ((GetTwips(cp->siz(), ft_unit) + 10) / 20);

	// map to value 1..7
	for (uns i = 0; i < (FontSizesMax - 1); i++) {
		if (pts < FontSizeVector[i])
			break;
	}

	return (i + 1);
}


long HTMwr::FontColorVector[FontColorMax] = {
 -1, 0, 0xFFFFFF, 0xFF0000, 0x00FF00,
 0x0000FF, 0x00FFFF, 0xFF00FF, 0xFFFF00
};

long
HTMwr::GetFontColor(HTMctl *cp)
{
	if (cp->siz() < FontColorMax)
    return FontColorVector[cp->siz()];
	return 0;  // black
}



// text flow processing


DCvlist HTMwr::FlowList;
DCilist HTMwr::SectList;
DClist HTMwr::SectUsedList;
DClist HTMwr::SectSkipList;
long HTMwr::SectNum = 0;
long HTMwr::NextSectNum = 0;
uns HTMwr::CurrFlow;
bool HTMwr::InDocBody;
bool HTMwr::InPara;
bool HTMwr::InParaGroup;
unc HTMwr::PGStarted;
bool HTMwr::ParaStartScan;
bool HTMwr::ParaStarting;
bool HTMwr::ParaEnding;
HTMgroup *HTMwr::FlowGroup;
HTMgroup *HTMwr::NormalFlowGroup = NULL;
uns HTMwr::NormalFlow = 0;


void
HTMwr::ProcFlow(HTMctl *cp)
{
	TextFrameContent = false;

	if ((GrTextFrame = (frame *) GrTextFrameList.find(cp->dat())) != NULL) {
		if (!TextFrameIsTextList.find((long) (GrTextFrame->parent))) {
			InGraphicText = true;
			return;
		}
		else
			TextFrameContent = true;
	}

	if (LastPage <= LastMasterPage)
		return;

  InDocBody = (bool) DocBodyList.find(CurrFlow = cp->dat());
  if ((FlowGroup = (HTMgroup *) FlowList.find(CurrFlow)) == NULL) {
    FlowGroup = new HTMgroup;
    FlowList.add(FlowGroup, CurrFlow);
    if (InDocBody)
      SectList.append(CurrFlow, SectNum = ++NextSectNum);
  }
	else if (FlowGroup == NormalFlowGroup) {
		CurrFlow = NormalFlow;
    if (InDocBody)
			SectNum = SectList(CurrFlow);
	}
	else SectNum = SectList(CurrFlow);
  if (DCLlevel < LevelMax)
    GroupStack[DCLlevel] = FlowGroup;
}


void
HTMwr::ProcFlowName(HTMctl *cp)
{
	if (LastPage <= LastMasterPage)
		return;
	if (IniFile
	 && IniFile->Section("TextFlows")
	 && IniFile->Find((char*) cp->ex())) {
		switch (IniFile->UCRight()) {
			case 'S':  // skip this flow
				SectSkipList.add(SectNum);
				break;
			case 'N':  // treat this flow as normal
				if (NormalFlowGroup) { // already one defined
					FlowList.replace(NormalFlowGroup, CurrFlow);
					SectSkipList.add(SectNum);
					FlowGroup = NormalFlowGroup;
					CurrFlow = NormalFlow;
				  if (DCLlevel < LevelMax)
				    GroupStack[DCLlevel] = FlowGroup;
				}
				else {    // first one encountered
					NormalFlow = CurrFlow;
					NormalFlowGroup = FlowGroup;
				}
				break;
			default:
				break;
		}
	}
}


void
HTMwr::MarkSectUsed(HTMctl *cp)
{
  if (!InDocBody || !SectNum || SectUsedList.find(SectNum))
    return;
  if ((cp->siz() < 4)
   && !memcmp("\\r", (char *) cp->ex(), 2))
    return;
  SectUsedList.add(SectNum);
}



// HTM writing phase


char *HTMwr::Title = DefTitle;
char *HTMwr::DefTitle = "Test File from Mif2Go";
char *HTMwr::HeadMacroName = "FirstHead";
char *HTMwr::HeadEndMacroName = "FirstHeadEnd";
char HTMwr::CssType = 'I';
char *HTMwr::CssName = "local.css";
char *HTMwr::CssPath = NULL;
char *HTMwr::CssFullPath = NULL;
bool HTMwr::UseCSS = true;
bool HTMwr::WriteClassAttributes = true;
unc HTMwr::WriteCssStylesheet = 'O';
bool HTMwr::WriteCssLink = true;
char *HTMwr::CssBrowserDetect = NULL;
bool HTMwr::CSSLinkNS4 = false;
bool HTMwr::UseSpanAsDefault = true;
bool HTMwr::Helen = false;


void
HTMwr::WriteHead(HTMwfile *wf, vfphtm cbfunc)
{
	wf->startSBlock("head");

	if (OmniHelp) {
		wf->startBlock("script");
		wf->strAttr("language", "JavaScript");
		wf->strAttr("type", "text/javascript");
		wf->strAttr("src", "ohmain.js");
		wf->endElem(true);
		wf->putEnd();
		wf->endBlock("script");
	}
	else if (WriteCssLink) {
		if (CssBrowserDetect)
			DCmacr::WriteMacro(wf, CssBrowserDetect);
		else
			WriteOneCSSLink(wf, CssFullPath? CssFullPath : CssName);

		if (UseConditionalFlagging
		 && CSSFlagsFile
		 && ReferenceFlagsFile)
			WriteOneCSSLink(wf, FlagsFullPath? FlagsFullPath : CSSFlagsFile);
	}

	wf->startSBlock("title");
	DCmacr::WriteMacro(wf, Title);
	wf->endBlock("title");
	wf->putEnd();

	// write MOTW here if used:  ****
	// See issues on http://msdn.microsoft.com/en-us/library/ms537628%28v=vs.85%29.aspx
	// The about:internet comment specifies the Internet zone
	//  <!-- saved from url=(0014)about:internet -->
	// The http://localhost comment specifies the Local Intranet zone
	//  <!-- saved from url=(0016)http://localhost -->

	WriteDropDownHead(wf);
	if (FilterConditions) {
		wf->startBlock("script");
		wf->strAttr("type", "text/JavaScript");
		wf->strAttr("src", FilterJSFile);
		wf->endElem(true);
		wf->endBlock("script");
		wf->putEnd();

		wf->startBlock("script");
		wf->strAttr("language", "JavaScript");
		wf->endElem(true);
		WriteFilterDialogSettings(wf);
		wf->endBlock("script");
		wf->putEnd();
	}

	if (!cbfunc) {  // which does its own insert
		if (!Extracting)
			PutInsertMacro(wf, HeadMacroName);
		else {
			char *txt = ExtrHeadMark.find(ExtrID);
			if (txt)
				DCmacr::WriteMacro(wf, txt);
			else if (!PutStyleMacro(wf, MacExtrHead, ExtrStartTag, ExtrStartForm))
				PutInsertMacro(wf, "ExtrHead");
		}
	}

	if (GeneratorTag == 'M') {
		wf->startElem("meta");
		wf->strAttr("name", "Generator");
		wf->startAttr("content");
		wf->putStr(FilterVer);
		wf->endAttr();
		wf->endElem(false);
		wf->putEnd();
	}

	wf->startElem("meta");
	wf->strAttr("http-equiv", "Content-type");
	wf->startAttr("content");
	wf->putStr(ContentType);
	wf->putStr("; charset=");
	if (QuotedEncoding)
		wf->putChar('\'');
	wf->putStr(XMLEncoding);
	if (QuotedEncoding)
		wf->putChar('\'');
	wf->endAttr();
	wf->endElem(false);
	wf->putEnd();

	PutMeta(wf);

	if (cbfunc)
		(*cbfunc)(wf);
	else if (!Extracting)
		PutInsertMacro(wf, HeadEndMacroName);
	else 
		PutInsertMacro(wf, "ExtrHeadEnd");

	wf->endBlock("head");
	wf->putEnd();
}


void
HTMwr::WriteOneCSSLink(HTMwfile *wf, char *fname)
{
	wf->startElem("link");
	//if (Xhtml || Xml)
	//	wf->strAttr("xmlns", "http://www.w3.org/1999/xhtml");
	wf->strAttr("rel", "stylesheet");
	wf->strAttr("href", fname);
	wf->strAttr("type", CSSLinkNS4 ? "text/css1" : "text/css");
	wf->endElem();
	wf->putEnd();
}


void
HTMwr::PutMeta(HTMwfile *wf)
{
	long id = Extracting ? ExtrID : SplitID;
	if (!id)
		id = 1;

	if (MetaCount) {
		for (uns i = 1; i <= MetaCount; i++) {
			if (((long) MetaPartList.find(i)) != id)
				continue;
			wf->startElem("meta");
			wf->strAttr("name", MetaNameList.find(i));
			wf->strAttr("content", MetaList.find(i));
			wf->endElem(false);
			wf->putEnd();
		}		
	}
	if (UseHVIndex)
		WriteHVIndexItems(wf, id);
}


uns HTMwr::BaseSize = 3;
char *HTMwr::BaseFontName = "Times";
bool HTMwr::Basefont = false;


void
HTMwr::WriteBaseProps(HTMwfile *wf)
{
	if (!Basefont)
		return;

	wf->startElem("basefont");
	if (UseFontSize)
		wf->numAttr("size", BaseSize);
	if (UseFontFace)
		wf->strAttr("face", FontNameList.find(1));
	wf->endElem(); 
	wf->putEnd();
}


void
HTMwr::WriteFlow(HTMwfile *wf, uns flownum)
{
  HTMgroup *gp = (HTMgroup *) FlowList.find(flownum);
  assert(gp);

	if (flownum != 0)
		SideHUsed = (bool) SideHFlowList.find(flownum);

  StartingPage = true;
  InPara = false;
  Last = DefForm;
  gp->write(*wf);
	if (!MinimizeLines)
		wf->putEnd();
	//if (InsertIndexTOC)
	//	InsertIndexTOCItem(wf);


	while (CurrList.level)  // trailing cleanup
		PutListEnd(wf, true);
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
HTMwr::WriteFlowNum(HTMwfile *wf, HTMctl *cp)
{
	uns cflow = cp->dat();

	if (CurrFlow == cflow)
		return;
	SideHUsed = (bool) SideHFlowList.find(cflow);
	CurrFlow = cflow;
}


unl HTMwr::ExtFileStartLine = 0;
unl HTMwr::ExtFileEndLine = 0;
unl HTMwr::ExtFileStartChar = 0;
unl HTMwr::ExtFileEndChar = 0;
unl HTMwr::ExtCodeFileLen = 0;
char *HTMwr::ExtCodeFileEnc = NULL;
char *HTMwr::PreFormatClass = "CodePara";

void
HTMwr::WriteFromExtFile(HTMwfile *wf, HTMctl *cp)
{
	// write contents of named file as text as is ****
	WriteText(wf, NULL);

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

	bool pre = wf->PreForm;
	if (!pre) {
		wf->startBlock("pre");
		wf->strAttr("class", PreFormatClass);
		wf->endElem(true);
	}

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
					wf->putChar('\t');
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
						WriteTextChar(wf, uch, uch);
					}
				}
				else if (!skip) {
					if (ExtFileStartChar) {
						ExtFileStartChar--;
						break;
					}
					else if (climit)
						ccount++;
					WriteTextChar(wf, ch & 0xFF, ch & 0xFF);
				}
				break;
		}
		if (climit
		 && (ccount >= climit))
			break;
	}

	if (!pre)
		wf->endBlock("pre");
	ExtFileStartLine = 0;
	ExtFileEndLine = 0;
	ExtCodeFileLen = 0;
	ExtFileStartChar = 0;
	ExtFileEndChar = 0;
	ExtCodeFileEnc = NULL;
}




bool HTMwr::HideWhite = false;
bool HTMwr::WhiteText = false;
bool HTMwr::ParaWhiteText = false;
bool HTMwr::NonWhiteText = false;
bool HTMwr::RemEmptyParas = false;
bool HTMwr::RemEmptyTbParas = false;
bool HTMwr::WordBreak = false;
bool HTMwr::TableWordBreak = false;
uns HTMwr::TableTitles = 0;
uns HTMwr::TableDPI = 96;
bool HTMwr::UseCodePage = false;


void
HTMwr::WriteText(HTMwfile *wf, HTMctl *cp)
{
	if (((CharBlockState == CbStart)
	  || (CharBlockState == CbChanged))
	 && ((CharBlockMacro & MacReplace)
		|| Curr.cmaccr)) { // CodeReplace
		if (Curr.hyper & 1) {  // starting hyperlink
			if (KeepReplacedCharLinks || Curr.keeplink) {
				if (HyperVal == 2)   // popup in HTML Help
					WriteHyperPopup(wf, HyperJumpStr);
				else
					WriteHyperAHref(wf);
			}
			Curr.hyper = 0;
		}
		if (Curr.hyper & 2) {  // ending hyperlink
			EndAHref(wf);
			WritePendingANames(wf);
		}
		if (Curr.cmaccr)
			DCmacr::WriteMacro(wf, Curr.cmaccr);
		else
			PutStyleMacro(wf, MacReplace, CurrCSName, CurrCSForm);
		CharBlockState = CbEnded;
		CurrCSName = LastCSName = NULL;
		CurrCSForm = LastCSForm = NULL;
		return;
	}

	if ((Curr.macpos & MacReplace)  // replacing para
	 && InPara)    // but only once, at start of para
		return;

	if ((CharBlockState == CbStarted)
	 && ((CharBlockMacro & MacReplace)
	  || (Curr.cmaccr)))  // still in replaced char text
		return;

	if (HideWhite
	 && (Curr.colr == 3)
	 && !ParaEnding) {
		WhiteText = true;
		return;
	}

  if (cp && InIdxGroup) {
		if (UseIndex)
			PutIndexEntry(cp);
    return;
  }

	if (Curr.list
	 && (InFormPx          // strip bullets, etc if in list format
	  || (InAnumVal        // also remove anums unless specified
	   && !Curr.anuse)))
		return;

	if ((InAnumGroup || InAnumVal)
	 && (Curr.anskip || (!UseAnums && !Curr.anuse)))
		return;

	if (EndingAnum) {
		EndingAnum = false;
		if (Last.hsname
		 || (CharBlockMacro & (MacEnd | MacAfter)))
			EndCharBlock(wf);
	}

	if ((Last.hsname != Curr.hsname)
	 || (CharBlockState == CbEnd)
	 || (CharBlockState == CbChanged)) {
		if (Last.hsname != NULL) {
			CPEndAll(wf);
			EndCharBlock(wf);
		}
		else if (CharBlockState == CbEnd)
			CharBlockState = CbNone;
		else if (CharBlockState == CbChanged)
			CharBlockState = CbStart;
	}

	if (XrefDelete)
		return;

  if (InPara == false) {  // style changes at start of para
		WriteParaProps(wf);
		InPara = true;
		WritePendingANames(wf);
		if (!ParaEnding)
			CondChanged = true;  // force check at start of para
		else
			CondChanged = false;
	}

	ParaStarting = false;
  StartingPage = false;

	if (Curr.hyper & 2) {  // ending hyperlink
		EndAHref(wf);
		WritePendingANames(wf);
	}

	if (HULevel
	 && !InHyper)
		WriteHyperUnique(wf);

	if (ParaForm.macpos & MacReplace)  // replacing para
		return;

	if (Curr.hyper & 1) {  // starting hyperlink
		if (HyperVal == 2)   // popup in HTML Help
			WriteHyperPopup(wf, HyperJumpStr);
		else
			WriteHyperAHref(wf);
		if ((CharBlockState == CbStart)
		 && (InlineHyperLevel >= InlineStackLevel))
			StartCharBlock(wf);
	}
	Curr.hyper = 0;

	if (!InTextRun) {      // don't write redundant tags
		if (CharBlockState == CbStart)
			StartCharBlock(wf);
		WriteProps(wf);
	}

	if (IndexEntry)
		WriteIndexEntry(wf);

  if ((cp == NULL)      // forced text start for non-text item
	 || (cp->siz() == 0))
    return;

	if (Curr.cmacpos & MacReplace)  // replacing text in char format
		return;

	if (CondChanged)
		CondTextChange(wf);

	NonWhiteText = true;
	WordBreak = false;

  if (cp->dt() == imnem)  // single char form
		WriteTextSingleChar(wf, (unl) cp->siz());
	else
		WriteTextStringChars(wf, (unc *) cp->ex(), cp->siz());
}


char *
HTMwr::GetEncodedText(DCwfile *dwf, char *txt)
{
	if (!dwf)
		return NULL;

	char *chp = NULL;
	long cdiv = dwf->GetDiv();
	long mdiv = dwf->NewDiv();

	while (*txt)
		WriteTextSingleChar(dwf, (unl) *txt++);

	if (dwf->LenDiv())
		chp = NewName(dwf->ReadDiv());

	dwf->DeleteDiv(mdiv);
	dwf->SelectDiv(cdiv);
	return chp;
}



void 
HTMwr::WriteTextSingleChar(DCwfile *dwf, unl wch)
{
	HTMwfile *wf = (HTMwfile *) dwf;
	unl uch = 0;
	unl xch = 0;
	bool mapped = false;
	unc *cpt = NULL;

	if (AnsiMap
	 && (MIFVer < 8)
	 && !UseCodePage
	 && (wch < 0xFF))
		wch = MapAnsiChar((uns) (wch & 0xFF));

	if (InComment) {
		if (wch <= 0xFF) {
			wf->putChar((int) wch);
			LinePos++;
		}
		return;
	}

	if ((wch > 0xFF)
	 || (MIFVer > 7))
		uch = wch;  // already Unicode
	else if (!UseCodePage) {  // set uch for standard Frame chars
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
			case 134:
				uch = 0x2020;
				break;
			case 135:
				uch = 0x2021;
				break;
			default:
				break;
		}
	}

	if (IniFile
	 && ((Curr.fmap && IniFile->Section(Curr.fmap))
		|| IniFile->Section("CharConvert"))) {  //  may be mapped

		// set up MapChar
		itoa(wch, MCDec, 10);
		if (uch)  // 4 or 5 hex digits
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
				*MCChr = (unc) (wch & 0xFF);
				MapChar.cnt++;
			}
		}

		if ((Curr.fmap
			&& IniFile->Section(Curr.fmap)
			&& IniFile->FindAny(&MapChar))
		 || (IniFile->Section("CharConvert")
			&& IniFile->FindAny(&MapChar))) {  // is mapped
			cpt = (unc *) IniFile->StrRight();
			if (!cpt || !*cpt)
				return;
			if (isdigit(*cpt)) {
				DeleteName((char *) cpt);
				uch = wch = (uns) IniFile->IntRight();
				if (wch == 0)
					return;
			}
			else if ((tolower(*cpt) == 'x')
			 && isxdigit(*(cpt + 1))) {
				uch = wch = (uns) strtoul((char *) cpt + 1, NULL, 16);
				DeleteName((char *) cpt);
				if (wch == 0)
					return;
			}
			else if ((tolower(*cpt) == 'u')
			 && (*(cpt + 1) == '+')
			 && isxdigit(*(cpt + 2))) {
				uch = wch = (uns) strtoul((char *) cpt + 2, NULL, 16);
				DeleteName((char *) cpt);
				if (wch == 0)
					return;
			}
			else {
				DCmacr::WriteMacro(wf, (char *) cpt);
				DeleteName((char *) cpt);
				return;
			}
			mapped = true;
		}
	}

	if (!mapped   // not already mapped, see if premapped
	 && Curr.fsym
	 && ((xch = CheckWingSym(Curr.fsym, wch)) != 0))
		wch = uch = xch;

	bool syfnt = false;

	if (SymbolFont
	 || (((finfo *) FontList.find(Curr.fnum))->ftype > 4))
		syfnt = true;

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
			AddChar(' ');
			if (!NumericCharRefs)
				goto normal;
			wf->putEntity("#160");
			break;
		case 0xAD:
			if (UseCodePage)
				goto normal;
			if (syfnt) {
				AddChar(' ');
				if (!NumericCharRefs)
					goto normal;
				wf->putEntity("#173");
			}
			else {
				if (InTbCell && TableWordBreak)
					WordBreak = true;
				if (!NumericCharRefs)
					goto normal;
				wf->putEntity("#173");
			}
			break;
		case 0x2011:
			if (NumericCharRefs)
				wf->putEntity("#x2011");
			else
				wf->putChar('-');
			AddChar('-');
#if 0
			if (AllowNobr)
				wf->startSBlock("nobr"); // see if this works
			wf->putChar('-');
			AddChar('-');
			if (AllowNobr)
				wf->endBlock("nobr");
#endif
			break;
		case 0x2044:
			wf->putText('/');
			AddChar('/');
			break;
#if 0
		case '&':
			wf->putEntity("amp");
			LinePos += CharWid;
			break;
		case '<':
			wf->putEntity("lt");
			LinePos += CharWid;
			break;
		case '>':
			wf->putEntity("gt");
			LinePos += CharWid;
			break;
#endif
		case 130:
		case 145:
		case 146:
			if (UseCodePage)
				goto normal;
			if (ValidOnly
			 || (!syfnt
				&& (HelpType == 'J')))
				wch = '\'';
			goto normal;

		case 132:
		case 147:
		case 148:
			if (UseCodePage)
				goto normal;
			if (ValidOnly
			 || (!syfnt
				&& (HelpType == 'J')))
				wch = '"';
			goto normal;

		case 149:
			if (UseCodePage)
				goto normal;
			if (ValidOnly
			 || ((HelpType == 'J') && !OracleHelp))
				wch = 183;  // mid-dot for bullet
			goto normal;

		case 150:
			if (UseCodePage)
				goto normal;
			if (ValidOnly
			 || (!syfnt
				&& (HelpType == 'J')))
				wch = '-';
			goto normal;

		case 151:
			if (UseCodePage)
				goto normal;
			if (ValidOnly
			 || (!syfnt
				&& (HelpType == 'J'))) {
				wf->putText('-');
				AddChar('-');
				wch = '-';
			}
			goto normal;

		normal:
		default:
			WriteTextChar(wf, wch, uch);
#if 0
			if (wch > 0x7F) {
				if (ValidOnly && (wch < 160) && uch)
					wf->putSpec(uch);
				else if (NumericCharRefs)
					wf->putSpec(uch ? uch : wch);
				else if (NoEnc
				 && (wch <= 0xFF))
					wf->putChar((unc) wch);
				else if (UTF8) {
					unc ubuf[5];
					long ucnt = 0;
					if (uch == 0)
						uch = wch;
					ucnt = ::UTF8(uch, ubuf) - ubuf;
					for (long u = 0; u < ucnt; u++)
						wf->putChar(ubuf[u]);
				}
				else
					wf->putSpec(wch);
				LinePos += CharWid;
			}
			else {
				wf->putText(wch);
				AddChar((unc) wch);
			}
#endif
			break;
	}
}

void 
HTMwr::WriteTextStringChars(HTMwfile *wf, unc *cpt, size_t len)
{
  unl wch = 0;
	unl uch = 0;
	unl xch = 0;
	bool mapped = false;
	unc ulen = 0;
  size_t i = 0;

  // string has only plain text
  for (i = len; i && *cpt; i--, cpt++) {
		if (InComment) {
			wf->putChar(*cpt);
			LinePos++;
			continue;
		}
		wch = *cpt;
		uch = xch = 0;
		mapped = false;
		if (((wch != ' ') || CharConvertSpaces)
		 && IniFile
		 && ((Curr.fmap && IniFile->Section(Curr.fmap))
		  || IniFile->Section("CharConvert"))) {  // check if mapped
			*MCChr = (unc) (wch & 0xFF);
			if (IniFile->Find(MCChr)) {  // is mapped
				char *cmp = IniFile->StrRight();
				if (!cmp || !*cmp)  // mapped to nothing
					continue; 
				if (isdigit(*cmp)) {
					DeleteName(cmp);
					uch = wch = (uns) IniFile->IntRight();
					if (wch == 0)
						continue;
				}
				else if ((tolower(*cmp) == 'x')
				 && isxdigit(*(cmp + 1))) {
					uch = wch = (uns) strtoul(cmp + 1, NULL, 16);
					DeleteName(cmp);
					if (wch == 0)
						continue;
				}
				else if ((tolower(*cmp) == 'u')
				 && (*(cmp + 1) == '+')
				 && isxdigit(*(cmp + 2))) {
					uch = wch = (uns) strtoul(cmp + 2, NULL, 16);
					DeleteName(cmp);
					if (wch == 0)
						continue;
				}
				else {
					DCmacr::WriteMacro(wf, cmp);
					DeleteName(cmp);
					continue;
				}
				mapped = true;
			}
		}

		if (!mapped      // not mapped, see if premapped
		 && Curr.fsym
		 && ((xch = CheckWingSym(Curr.fsym, wch)) != 0))
			wch = uch = xch;

		if (!uch
		 && (wch > 0x7F)
		 && ((ulen = ::IsUTF8(cpt)) > 0)) {  // pass UTF-8 through
			while (--ulen) {
				wf->putChar(*cpt++);
				i--;
			}
			wf->putChar(*cpt);
      LinePos += CharWid;
		}
		else
			WriteTextChar(wf, wch, uch);
#if 0
		if (wch > 0x7F) {
			if (wch > 0x7F) {
				if (NumericCharRefs)
					wf->putSpec(wch);
				else if (NoEnc
				 && (wch <= 0xFF))
					wf->putChar((unc) wch);
				else if (UTF8) {
					unc ubuf[5];
					unl ucnt = ::UTF8(wch, ubuf) - ubuf;
					for (unl u = 0; u < ucnt; u++)
						wf->putChar(ubuf[u]);
				}
				else
					wf->putSpec(wch);
				LinePos += CharWid;
			}
			else if (wch) {
				wf->putText(wch);
				AddChar((unc) wch);
			}
		}
    else switch (wch) {
      case '&':
        wf->putEntity("amp");
        LinePos += CharWid;
        break;
      case '<':
        wf->putEntity("lt");
        LinePos += CharWid;
        break;
      case '>':
        wf->putEntity("gt");
        LinePos += CharWid;
        break;
      default:
        wf->putText(wch);
        AddChar(wch);
        break;
    }
#endif
  }
}


void
HTMwr::AddChar(unc ch)
{
	if (InPreform)
		LinePos++;
  else if (FixedWid || (ch > CharMetricMax))
    LinePos += CharWid;
  else
    LinePos += (CharWid * CharMetrics[ch]) / 56;
}


void
HTMwr::WriteTextChar(HTMwfile *wf, unl wch, unl uch)
{
	if (wch > 0x7F) {
#if 0
		if (UseCodePage) {
			if (MIFVer < 8) {  // already using Windows code page
				if (wch > 0xFF)  // ignore other syms for now
					return;
			}
			else {  // Unicode, map to code page
				if ((wch = GetUnicodeEquiv(uch ? uch : wch)) == 0)
					return;
			}
	    wf->putChar((unc) wch);
		}
#endif
		if (UseCodePage) {
			if (MIFVer < 8) {  // already using Windows code page
				if (wch > 0xFF)  // ignore other syms for now
					return;
			}
			else if (wch > 0x7F) {  // Unicode, map to code page
				if ((wch = DCicu::GetUnicodeEquiv(uch ? uch : wch)) == 0) {
					if (WriteWingSym(wf, uch ? uch : wch))
						return;
					if (NumericCharRefs)
						wf->putSpec(uch ? uch : wch);
					return;
				}
			}
			wf->putChar((unc) (wch & 0xFF));
			if (wch > 0xFF)  // two byte return
				wf->putChar((unc) ((wch >> 8) & 0xFF));
		}
		else if (ValidOnly && (wch < 160) && uch)
	    wf->putSpec(uch);
		else if (NumericCharRefs)
	    wf->putSpec(uch ? uch : wch);
		else if (NoEnc
		 && (wch <= 0xFF))
	    wf->putChar((unc) wch);
		else if (UTF8)
			wf->putStr((char *) ::UTF8(uch ? uch : wch));
		else
	    wf->putSpec(wch);
    LinePos += CharWid;
  }
  else switch (wch) {
    case '&':
      wf->putEntity("amp");
      LinePos += CharWid;
      break;
    case '<':
      wf->putEntity("lt");
      LinePos += CharWid;
      break;
    case '>':
      wf->putEntity("gt");
      LinePos += CharWid;
      break;
    default:
      wf->putText(wch);
      AddChar((unc)wch);
      break;
  }
}


unl HTMwr::SymbolUniMap[0xE0] = {
 0x20, 0x21, 8704, 0x23, 8707, 0x25, 0x36, 8717,  // 0x20  32
 0x28, 0x29, 8727, 0x2B, 0x2C, 8722, 0x2E, 0x2F,  // 0x28  40
 0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37,  // 0x30  48
 0x38, 0x39, 0x3A, 0x3B, 0x3C, 0x3D, 0x3E, 0x3F,  // 0x38  56
 8773, 913,  914,  935,  916,  917,  934,  915,   // 0x40  64 @
 919,  921,  977,  922,  923,  924,  925,  927,   // 0x48  72 H
 928,  920,  929,  931,  932,  933,  962,  937,   // 0x50  80 P
 926,  936,  918,  0x5B, 8756, 0x5D, 8869, 0x5F,  // 0x58  88 X
 0x0,  945,  946,  967,  948,  949,  966,  947,   // 0x60  96
 951,  953,  981,  954,  955,  956,  957,  959,   // 0x68 104 h
 960,  952,  961,  963,  964,  965,  982,  969,   // 0x70 112 p
 958,  968,  950,  0x7B, 0x7C, 0x7D, 8764, 0x0,   // 0x78 120 x
 0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,   // 0x80 128
 0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,   // 0x88 136
 0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,   // 0x90 144
 0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,   // 0x98 152
 0x0,  978,  8242, 8804, 8260, 8734, 402,  9827,  // 0xA0 160
 9830, 9829, 9824, 8596, 8592, 8593, 8594, 8595,  // 0xA8 168
 0xB0, 177,  8243, 8805, 215,  8733, 8706, 8226,  // 0xB0 176
 247,  8800, 8801, 8776, 8230, 9168, 9135, 8629,  // 0xB8 184
 8501, 8465, 8476, 8472, 8855, 8853, 8709, 8745,  // 0xC0 192
 8746, 8835, 8839, 8836, 8834, 8838, 8712, 8713,  // 0xC8 200
 8736, 8711, 174,  169,  8482, 8719, 8730, 8901,  // 0xD0 208
 172,  8743, 8744, 8660, 8656, 8657, 8658, 8659,  // 0xD8 216
 9674, 9001, 174,  169,  8482, 8721, 9115, 9116,  // 0xE0 224
 9117, 9121, 9122, 9123, 9127, 9128, 9129, 9130,  // 0xE8 232
 8364, 9002, 8747, 8992, 9134, 8993, 9118, 9119,  // 0xF0 240
 9120, 9124, 9125, 9126, 9131, 9132, 9133, 0x0    // 0xF8 248
};


unl HTMwr::WingdingsUniMap[0xE0] = {
 0x20,   0x270F, 0x2702, 0x2701, 0x0,    0x0,    0x0,    0x0,     // 0x20
 0x260E, 0x2706, 0x2709, 0x0,    0x0,    0x0,    0x0,    0x0,
 0x0,    0x0,    0x0,    0x0,    0x0,    0x0,    0x231B, 0x2328,  // 0x30
 0x0,    0x0,    0x0,    0x0,    0x0,    0x0,    0x2707, 0x270D,
 0x0,    0x270C, 0x0,    0x0,    0x0,    0x261C, 0x261E, 0x261D,  // 0x40
 0x261F, 0x0,    0x263A, 0x0,    0x2639, 0x0,    0x2620, 0x2690,
 0x0,    0x2708, 0x263C, 0x0,    0x2744, 0x0,    0x271E, 0x0,     // 0x50
 0x2720, 0x2721, 0x262A, 0x262F, 0x0950, 0x2638, 0x2648, 0x2649,
 0x264A, 0x264B, 0x264C, 0x264D, 0x264E, 0x264F, 0x2650, 0x2651,  // 0x60
 0x2652, 0x2653, 0x0026, 0x0026, 0x25CF, 0x274D, 0x25A0, 0x25A1,
 0x0,    0x2751, 0x2752, 0x0,    0x2666, 0x25C6, 0x2756, 0x0,     // 0x70
 0x2327, 0x2353, 0x2318, 0x2740, 0x273F, 0x275D, 0x275E, 0x25AF,
 0x24EA, 0x2460, 0x2461, 0x2462, 0x2463, 0x2464, 0x2465, 0x2466,  // 0x80
 0x2467, 0x2468, 0x2469, 0x24FF, 0x2776, 0x2777, 0x2778, 0x2779,
 0x277A, 0x277B, 0x277C, 0x277D, 0x277E, 0x277F, 0x0,    0x0,     // 0x90
 0x0,    0x0,    0x0,    0x0,    0x0,    0x0,    0x00B7, 0x2022,
 0x25AA, 0x25CB, 0x0,    0x0,    0x25C9, 0x25CE, 0x0,    0x25AA,  // 0xA0
 0x25FB, 0x0,    0x2726, 0x2605, 0x2736, 0x2734, 0x2739, 0x2735,
 0x0,    0x2316, 0x2727, 0x2311, 0x0,    0x272A, 0x2730, 0x0,     // 0xB0
 0x0,    0x0,    0x0,    0x0,    0x0,    0x0,    0x0,    0x0,
 0x0,    0x0,    0x0,    0x0,    0x0,    0x0,    0x0,    0x0,     // 0xC0
 0x0,    0x0,    0x0,    0x0,    0x0,    0x0,    0x0,    0x0,
 0x0,    0x0,    0x0,    0x0,    0x0,    0x232B, 0x2326, 0x0,     // 0xD0
 0x27A2, 0x0,    0x0,    0x0,    0x27B2, 0x0,    0x0,    0x0,
 0x0,    0x0,    0x0,    0x0,    0x0,    0x0,    0x0,    0x0,     // 0xE0
 0x2794, 0x0,    0x0,    0x0,    0x0,    0x0,    0x0,    0x21E6,
 0x21E8, 0x21E7, 0x21E9, 0x2B04, 0x21F3, 0x2B00, 0x2B01, 0x2B03,  // 0xF0
 0x2B02, 0x25AD, 0x25AB, 0x2717, 0x2713, 0x2612, 0x2611, 0x0
};


unl
HTMwr::CheckWingSym(unc sym, unl wch)
{
	if ((wch < 0x20) || (wch > 0xFF))
		return 0;

	if (UseCodePage)
		return 0;

	if (sym == 1)  // Symbol font
		return SymbolUniMap[wch - 0x20];
	else if (sym == 2)  // Wingdings font
		return WingdingsUniMap[wch - 0x20];
	else
		return 0;
}


bool HTMwr::CSSSymFontClass = false;
bool HTMwr::CSSWingFontClass = false;

bool
HTMwr::WriteWingSym(HTMwfile *wf, unl uch)
{
	char *classatt = NULL;
	unl ch = GetWingSym(uch, &classatt);

	if (ch == 0)
		return false;

	if (!NoFont) {
		wf->startElem("font");
		wf->strAttr("face", classatt);
		wf->endElem(true);
		wf->putChar(ch);
		wf->endBlock("font");
	}
	else {
		wf->startElem("span");
		wf->strAttr("class", classatt);
		wf->endElem(true);
		wf->putChar(ch);
		wf->endBlock("span");

		if (!strcmp(classatt, "Symbol"))
			CSSSymFontClass = true;
		else if (!strcmp(classatt, "Wingdings"))
			CSSWingFontClass = true;
	}
	return true;
}


void 
HTMwr::WriteWingSymFormats(void)
{
	if (!CssFile)
		return;

	if (CSSSymFontClass) {
		CssFile->putCr();
		CssFile->putStr(".Symbol { font-family: Symbol; }");
	}
	if (CSSWingFontClass) {
		CssFile->putCr();
		CssFile->putStr(".Wingdings { font-family: Wingdings; }");
	}
}


unl
HTMwr::GetWingSym(unl uch, char **classatt)
{
	unl i = 0;

	for (i = 0; i < 0xE0; i++) { // Symbol font
		if (SymbolUniMap[i] == uch) {
			*classatt = "Symbol";
			return (i + 0x20);
		}
	}

	for (i = 0; i < 0xE0; i++) {  // Wingdings font
		if (WingdingsUniMap[i] == uch) {
			*classatt = "Wingdings";
			return (i + 0x20);
		}
	}

	return 0;
}



// properties

char HTMwr::RuninType = 'R';
bool HTMwr::UseRunInTag = false;
long HTMwr::RuninDiv = 0;
long HTMwr::PreRuninDiv = 0;
pprop HTMwr::RuninProps;
DCvlist HTMwr::RuninOverrides;
bool HTMwr::InPreform = false;
bool HTMwr::MergePre = true;
char *HTMwr::PreClass = NULL;
bool HTMwr::UseXMLbr = false;
bool HTMwr::InComment = false;
bool HTMwr::InRaw = false;
bool HTMwr::MinimizeLines = false;
bool HTMwr::HardRetPara = false;
bool HTMwr::ParaStartProps = false;
bool HTMwr::AlignAttributes = true;
bool HTMwr::CellAlignAttributes = true;
bool HTMwr::CellColorAttributes = true;
char *HTMwr::ScriptTag = NULL;
char *HTMwr::ScriptType = "text/javascript";


void
HTMwr::WriteProps(HTMwfile *wf)
{
	if ((Curr.macpos & MacReplace)
	 || Curr.notags
	 || Curr.comment)
		return;

	if (CurrTab->tpos
	 && (CurrTab->tpos > LinePos)
	 && Curr.tabrep) {
		WriteEndCharProps(wf);
		WriteTabs(wf);
    CurrTab->tpos = 0;  // turn off
  }

  WriteCharProps(wf);    // character properties

  if (CurrTab->tpos
	 && !Curr.tabrep) {   // tabs (not tab defs)
    if (CurrTab->tpos > LinePos)
      WriteTabs(wf);
    else wf->putChar(' ');
    CurrTab->tpos = 0;  // turn off
  }
}


void
HTMwr::WriteParaStartProps(HTMwfile *wf)
{
	if (InFootnote)
		return;

	if (CurrList.level  // have been in a list lately
	 && (!Curr.list)) {
	//  || ListTypesDiffer(CurrList.type[CurrList.level], Curr.list))) {     // but not in one now
		if (Curr.lend || !Curr.lnest) { // item ends list or does not nest
			while (CurrList.level)
				PutListEnd(wf, Curr.lend);  // unnesting
			Last.list = Curr.list;
			if (Curr.lend)   // end all lists
				CurrList.last = CurrList.level;
		}
		else if (Curr.lnest
		 && (Curr.llev < CurrList.level)) {
			while (CurrList.level > Curr.llev)
				PutListEnd(wf, true);
		}
	}

	EndSpecialBlocks(wf, false, true);

  if (Curr.pbreak)
    WriteBreak(wf);

	if (EndingFtn)
		return;

	if (Curr.macpos & MacReplace)   // macro replace
		PutStyleMacro(wf, MacReplace);
	else if (Curr.maccr)
		DCmacr::WriteMacro(wf, Curr.maccr);

	ParaStartProps = true;
}


void
HTMwr::EndSpecialBlocks(HTMwfile *wf, bool endall, bool line)
{
	if (InComment && (endall || !Curr.comment)) {  // ending comment
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
		if (line && !MinimizeLines)
			wf->putEnd(); // blank line after comment
		LinePos = 0;
		InRaw = InComment = false;
	}

	if (InPreform
	 && (endall
	  || !Curr.preform
	  || !(WriteClassAttributes  // pre with different classes
		 && MergePre
	   && Curr.hcname
	   && PreClass
	   && !strcmp(Curr.hcname, PreClass)))) {  // ending PRE
		WriteEndCharProps(wf);
		wf->endBlock("pre");
		wf->putEnd();
		if (line)
			wf->putEnd(); // blank line between paras
		LinePos = 0;
		InPreform = false;
		PreClass = NULL;
	}

	if (CodeStoreDiv) {  // end any pending div
		wf->SelectDiv(PreCodeStoreDiv);
		CodeStoreDiv = 0;
	}
}


void
HTMwr::WriteParaProps(HTMwfile *wf)
{
	if (InFootnote) {
		if (ParaForm.macpos & MacReplace)   // macro replace
			return;
		if (Curr.macpos & MacBefore)        // macro before
			PutStyleMacro(wf, MacBefore);
		else if (Curr.maccb)
			DCmacr::WriteMacro(wf, Curr.maccb);
		if (Curr.macpos & MacStart)         // macro begin
			PutStyleMacro(wf, MacStart);
		else if (Curr.maccs)
			DCmacr::WriteMacro(wf, Curr.maccs);
		if (FootInlineParaTag
		 && (FootType == 'I'))
			wf->startSBlock(FootInlineParaTag);
		return;
	}

	if (!ParaStartProps)
		WriteParaStartProps(wf);

	char *tag = HXMwr::HXML ? Curr.dtcname :
	 (ClassIsTag ? Curr.hcname : Curr.hpname);
#if 0
	if (Xml && NestXML)
		StartNestedXmlBlock(wf, tag);
#endif
	if (ParaForm.macpos & MacReplace) {  // macro replace
		InComment = Curr.comment;
		InRaw = Curr.raw;
		return;
	}

	// for DITA, start storing head info for next block here ****
	if (HXMwr::HXML
	// && !HXMwr::DITANoBlock
	 && !HXMwr::PreDITARuninDiv)
		HXMwr::SetDITAStart(wf);

	if (MakeTrail
	 && Curr.trail
	 && (TrailPosition == 'B'))
		WriteTrail(wf);

	if (Curr.ddlpar)
		WriteDropLinkPara(wf);
	if (Curr.ddbstb
	 && DropDownUsed)
		DCmacr::WriteMacro(wf, "<$DropBlockStart>");

	if (Curr.macpos & MacBefore)    // macro before
		PutStyleMacro(wf, MacBefore);
	else if (Curr.maccb)
		DCmacr::WriteMacro(wf, Curr.maccb);

	if (Curr.list) {        // list is next
		if (ListTypesDiffer(Last.list, Curr.list)
		 || (Last.llev != Curr.llev)) { // next is different from prev list
			ResetListLevel(wf);
			Last.list = Curr.list;
			Last.llev = Curr.llev;
		}
	}
	else if (Curr.lend)   // ensure list values restart
		CurrList.last = 0;

	if (PRefFrameList.find(ParaRefNum)
	 && GraphANamesFirst)
		WritePendingANames(wf);  // before putting out the images

  PRefFrameList.write(ParaRefNum, (DCfile *) wf, ExecFrame);

	if (PRuninFrameList.find(ParaRefNum))
		RuninRefNum = ParaRefNum;
	else RuninRefNum = 0;

	if (Curr.pfx) { // prefix frame
		frame *fr = (frame *) FrameList.find(Curr.pfx);
		if (fr)
			WriteFrame(wf, fr, true);
	}

	if (CodeStoreVar) {  // start diversion for macro var
		DCnlist *dcn = NULL;
		PreCodeStoreDiv = HXMwr::DITAStartDiv ? HXMwr::PreDITADiv : wf->GetDiv();
		if ((CodeStoreDiv = CodeStoreDivList.getitem(CodeStoreVar, &dcn, true, false)) != 0) {  // existing div
			//CodeStoreDivList.remove(CodeStoreDiv);
			wf->DeleteDiv(CodeStoreDiv);
			 dcn->id = CodeStoreDiv = wf->NewDiv();
		}
		else {
			CodeStoreDiv = wf->NewDiv();
			CodeStoreDivList.add(CodeStoreVar, CodeStoreDiv);
		}
		CodeStoreVar = NULL;
	}

	if (Curr.runin) {   // runin para starting
		if (!RuninDiv) {  // new runin, start diversion
			HTMgroup *tcp = NULL;
			if ((tcp = (HTMgroup *) RuninOverrides.find(ParaRefNum)) != NULL)
				Curr.rungroup = tcp;
			PreRuninDiv = wf->GetDiv();
			if (HXMwr::HXML
			// && !HXMwr::DITANoBlock
			 && (PreRuninDiv == HXMwr::DITAStartDiv))
				PreRuninDiv = HXMwr::PreDITARuninDiv = wf->NewDiv();
			RuninDiv = wf->NewDiv();
			RuninProps = Curr;  // store para props
		}
		InPara = true;        // write only char props for runin
	}
	else {  // not run-in, normal processing
		if (RuninDiv) {  // normal para after runin starting
			InPara = true;
			wf->SelectDiv(HXMwr::PreDITARuninDiv ? HXMwr::DITAStartDiv : PreRuninDiv);  // end diversion
			Curr.find = RuninProps.find; // adjust para props
			Curr.bspace = RuninProps.bspace;
			if (UseRunInTag)
				tag = HXMwr::HXML ? RuninProps.dtcname :
						 (ClassIsTag ? RuninProps.hcname : RuninProps.hpname);
		}

		if (Curr.list) {  // in list item sequence, don't use for DITA
			if ((Xml || Xhtml)
			 && CurrList.iname[CurrList.level]) {
				wf->endBlock(CurrList.iname[CurrList.level]);
				wf->putEnd();
				CurrList.iname[CurrList.level] = NULL;
			}
			CurrList.iname[CurrList.level] = tag;
			if (Xml || Xhtml)
				wf->startBlock(CurrList.iname[CurrList.level]);
			else
				wf->startElem(CurrList.iname[CurrList.level]);
			if (Curr.list < OrderedListMax) {
				if (Curr.lfirst)
					CurrList.pos[CurrList.level] = 1;
				else
					CurrList.pos[CurrList.level]++;
				if (!NoAttribLists)
					wf->numAttr("value", (short) CurrList.pos[CurrList.level]);
			}
		}
		else if (Curr.comment) {
			if (!Curr.raw
			 && Curr.hpname
			 && !stricmp(Curr.hpname, "script")) {
				wf->startBlock(ScriptTag = tag);
				wf->strAttr("type", ScriptType);
				wf->endElem(true);
				wf->putEnd();
			}
			if (!InComment) {
				if (Curr.raw)
					InRaw = true;
				else
					wf->startElem("!-- ");
				InComment = true;
				if (!MinimizeLines)
					wf->putEnd();
				LinePos = 0;
			}
		}
		else if (Curr.preform) {
			if (!InPreform) {
				wf->startBlock(HXMwr::HXML ? Curr.dtcname : (ClassIsTag ? Curr.hcname : "pre"));
				InPreform = true;
				StartCondParaBlock(wf);
				if (AlignAttributes)
					wf->setAlign((HTMwfile::ParaAlign) Curr.algn);
				if (WriteClassAttributes) {
					if (HXMwr::HXML
					 || !ClassIsTag)
						wf->setClass(Curr.hcname);
					PreClass = Curr.hcname;
				}
				wf->endElem(true);
			}
		}
		else if (Curr.hpname
		 && !stricmp(Curr.hpname, "script")) {
			wf->startBlock(ScriptTag = tag);
			wf->strAttr("type", ScriptType);
			wf->endElem(true);
			wf->putEnd();
		}
		else {
			if (HXMwr::HXML) { // put out saved endings and then close div for start
				// if (!HXMwr::DITANoBlock) 
				wf->SelectDiv(HXMwr::DITAStartDiv);
				HXMwr::WriteDITAStartPTag(wf, ParaRefNum, tag, Curr.dtpar, Curr.dtfirst,
													 Curr.dtlev, Curr.dstag ? Curr.dstag : Curr.stag);
				StartCondParaBlock(wf);
				if (ParaAttrLp) {
					ParaAttrLp->writeall(wf, WriteAttrListItem);
					ParaAttrLp = NULL;
				}
			}
			else if (!Curr.nopara) {
				wf->startBlock(tag);
				StartCondParaBlock(wf);
				if (ParaAttrLp) {
					ParaAttrLp->writeall(wf, WriteAttrListItem);
					ParaAttrLp = NULL;
				}
			}

			if (!Curr.hcwtoc) {
				if ((SideHUsed == true)
				 && (Curr.sideh > 1))   // in side head col
					Curr.algn = 0;
				if (!Curr.notags
				 && !Curr.nopara
				 && AlignAttributes
				 && !(InTbCell && CellAlignAttributes))
					wf->setAlign((HTMwfile::ParaAlign) Curr.algn);
			}
		}

		if (!InPreform) {
			wf->SetAttrAdd(true);
			if (CurrCondClass) {
				wf->strAttr("class", CurrCondClass);
				CurrCondClass = NULL;
			}
			if (WriteClassAttributes
			 && !ClassIsTag
			 && !Curr.hcwtoc
			 && ((Curr.list == 0) || !NoClassLists)
			 && !Curr.notags
			 && !(HXMwr::HXML && HXMwr::DITANoBlock)
			 && !Curr.nopara
			 && !Curr.comment
			 && !Curr.hpclass)
				wf->setClass(Curr.hcname);

			wf->SetAttrAdd(false);

			if (CurrMapDescription) {
				wf->strAttr("title", CurrMapDescription);
				CurrMapDescription = NULL;
			}

			if (!Curr.comment
			 && !Curr.nopara
			 && !(HXMwr::HXML && HXMwr::DITANoBlock)) {
				wf->endElem(true);
				wf->SetAttrAdd(false);
				HXMwr::WritePendingAliases(wf);
			}
		}

		SetCondFlags(wf);

		if (RuninRefNum) {
			PRuninFrameList.write(RuninRefNum, (DCfile *) wf, ExecFrame);
			RuninRefNum = 0;
		}
	}

	if (Curr.macpos & MacStart)   // macro begin
		PutStyleMacro(wf, MacStart);
	else if (Curr.maccs)
		DCmacr::WriteMacro(wf, Curr.maccs);

	if (Curr.ddlpst)
		WriteDropLinkStart(wf);

	char *glossterm = GlossParaList.find(ParaRefNum);
	if (glossterm) 
		SetGlossTerm(wf, glossterm, GlossIDList.find(ParaRefNum));

	if (RuninDiv
	 && !Curr.runin) {  // para after runin
		wf->SelectDiv(RuninDiv);
		char *rdiv = wf->ReadDiv();
		long rdlen = wf->LenDiv();  // may end with newline
		if (rdlen
		 && (rdiv[rdlen - 1] == '\n'))
			rdlen--;                  // remove any newline
		long tlen = 0;
		char *etag = NULL;
		while (rdlen
		 && (rdiv[rdlen - 1] == '>')) {  // move before closing char tag
			rdlen--, tlen++;
			while (rdlen && (rdiv[rdlen] != '<'))
				rdlen--, tlen++;
		}
		if (tlen)
			etag = NewName(rdiv + rdlen, tlen);
		bool rdovr = (rdlen && ispunct(rdiv[rdlen - 1])) ? true : false;  // real string punct
		wf->SelectDiv(PreRuninDiv);
		if (rdlen)
			wf->putStr(rdiv, rdlen);  // write up to char tag, including typed punct
		wf->DeleteDiv(RuninDiv);
		wf->SelectDiv(PreRuninDiv);
		RuninDiv = 0;
		InPara = true;
		long xdiv = wf->NewDiv();  // capture runpunct
		if (RuninProps.runpunct
		 && *(RuninProps.runpunct))
			wf->putStr(RuninProps.runpunct);
		else if (RuninProps.rungroup)
			(RuninProps.rungroup)->write(*wf);
		rdiv = wf->ReadDiv();
		rdlen = wf->LenDiv();
		long alen = 0;
		while (rdlen && (rdiv[alen] == '<')) { // move past leading tags
			while (rdlen && (rdiv[alen] != '>'))
				alen++, rdlen--;
			alen++, rdlen--; // step past closing >
		}
		tlen = alen;  // index of first char after tag
		if (rdovr) {  // strip leading punct if typed in before
			while (rdlen && rdiv[alen] && ispunct(rdiv[alen]))
				alen++, rdlen--;
		}
		wf->SelectDiv(PreRuninDiv);
		if (rdlen)   // write remaining space or other non-punct
			wf->putStr(rdiv + alen, rdlen);
		if (etag) {  // write any closing char tag
			wf->putStr(etag);
			DeleteName(etag);
		}	
		if (tlen)    // write any tag from before punct
			wf->putStr(rdiv, tlen);
		wf->DeleteDiv(xdiv);
		wf->SelectDiv(PreRuninDiv);
	}

	if (HXMwr::HXML) {
	// && !HXMwr::DITANoBlock) {
		if (HXMwr::PreDITARuninDiv
		 && !Curr.runin) {  // para after runin
			wf->SelectDiv(HXMwr::PreDITARuninDiv);
			char *prdiv = wf->ReadDiv();
			long prdlen = wf->LenDiv();  // may end with newline
			if (prdiv && prdlen) {
				if (prdiv[prdlen - 1] == '\n')
					prdlen--;                  // remove any newline
				wf->SelectDiv(HXMwr::PreDITADiv);
				//char *ctag = RuninProps.dtcname;  // name already in div
				//if (ctag)
				//	wf->startSBlock(ctag);
				wf->putStr(prdiv, prdlen);
				//if (ctag)
				//	wf->endBlock(ctag);
			}
			wf->DeleteDiv(HXMwr::PreDITARuninDiv);
			PreRuninDiv = HXMwr::PreDITARuninDiv = 0;
			wf->SelectDiv(HXMwr::PreDITADiv);
		}
		if (Curr.dtptyps)
			wf->putStr(Curr.dtptyps);
	}

	FileStarted = true;
}

#if 0
bool HTMwr::InList = false;
unc HTMwr::ListTypeVal = 0;
#endif

bool HTMwr::AlwaysNestLists = false;
char *HTMwr::LastItemName = NULL;

void
HTMwr::PutListStart(HTMwfile *wf, unc ltype, bool lstart)
{
	assert(ltype > 0);  // not used for DITA
	if ((WriteClassAttributes && !AlwaysNestLists)
	 && CurrList.used
	 && CurrList.typeval
	 && (ltype != CurrList.typeval)) {
		if ((Xml || Xhtml)
		 && CurrList.iname[CurrList.level]) {
			wf->endBlock(CurrList.iname[CurrList.level]);
			wf->putEnd();
			CurrList.iname[CurrList.level] = NULL;
		}
		wf->endBlock(HTMLListInit[CurrList.typeval - 1]);
		wf->putEnd();
		wf->putEnd(); // blank line sets off list end
		//CurrListLevel--;
		CurrList.used = false;
	}

	if (CurrList.level < (ListLevMax - 1))
		CurrList.level++;
	CurrList.type[CurrList.level] = ltype;

	if ((WriteClassAttributes && !AlwaysNestLists)
	 && CurrList.used) {
		if (ltype == CurrList.typeval)
			return;
	}
	CurrList.typeval = ltype;
	CurrList.used = true;

	if ((Xml || Xhtml)
	 && CurrList.iname[CurrList.level]
	 //&& (!(!strcmp(LastItemName, "li")
	 // && AlwaysNestLists))
		) {
		wf->endBlock(CurrList.iname[CurrList.level]);
		wf->putEnd();
		CurrList.iname[CurrList.level] = NULL;
	}
	wf->startBlock(HTMLListInit[ltype - 1]);
	if ((Curr.list == ListInitMax)
	 && !Xhtml)
		wf->putStr("compact");
	else if ((ltype <= ListTypeMax)
	 && (UseListTypeAttribute || !NoAttribLists))
		wf->strAttr("type", HTMLListType[ltype - 1]);
	if (ltype < OrderedListMax) { // starting ordered list
		if (lstart)  // really starting it
			CurrList.pos[CurrList.level] = 0;
		else if (!NoAttribLists)   // no, continuing it
			wf->numAttr("start", CurrList.pos[CurrList.level] + 1);
	}
	if (!NoClassLists
	 && WriteClassAttributes)
		wf->setClass(Curr.hcname);

	wf->endElem(true);
	wf->putEnd();
	wf->putEnd(); // blank line sets off list start
}


void
HTMwr::PutListEnd(HTMwfile *wf, bool lend)
{
	// not used for DITA
	unc ltype = CurrList.type[CurrList.level];
	if (lend)
		CurrList.pos[CurrList.level] = 0;
	if (CurrList.level > 0) 
		CurrList.level--;

	if ((WriteClassAttributes && !AlwaysNestLists)
	 && !CurrList.used)
		return;
	CurrList.used = false;
	CurrList.typeval = 0;

	if (InPreform) {
		WriteEndCharProps(wf);
		wf->endBlock("pre");
		wf->putEnd();
		wf->putEnd(); // blank line between paras
		LinePos = 0;
		InPreform = false;
	}

	if ((Xml || Xhtml)
	 && CurrList.iname[CurrList.level + 1]) {  // was LastItemName
		wf->endBlock(CurrList.iname[CurrList.level + 1]);
		wf->putEnd();
		CurrList.iname[CurrList.level + 1] = NULL;
	}
	wf->endBlock(HTMLListInit[ltype - 1]);
	wf->putEnd();
	wf->putEnd(); // blank line sets off list end

	if ((WriteClassAttributes && !AlwaysNestLists)
	 && CurrList.level
	 && Curr.list) {
		unc ltype = CurrList.type[CurrList.level];
		CurrList.typeval = ltype;
		CurrList.used = true;

		wf->startBlock(HTMLListInit[ltype - 1]);
		if ((Curr.list == ListInitMax)
		 && !Xhtml)
			wf->putStr("compact");
		else if ((ltype <= ListTypeMax)
		 && (UseListTypeAttribute || !NoAttribLists))
			wf->strAttr("type", HTMLListType[ltype - 1]);
		if ((ltype < OrderedListMax)  // restarting ordered list
		 && !NoAttribLists)
			wf->numAttr("start", CurrList.pos[CurrList.level] + 1);
		if (!NoClassLists
		 && WriteClassAttributes)
			wf->setClass(Curr.hcname);

		wf->endElem(true);
		wf->putEnd();
		wf->putEnd(); // blank line sets off list start
	}
}


void
HTMwr::ResetListLevel(HTMwfile *wf)
{
	static DCnlist listitems;

	if (CurrList.level    // unnest as required for current level
	 && ((Curr.llev && (CurrList.level > Curr.llev))  // new level lower
		|| (!Curr.lnest))) {  // or cannot nest new list in old
		while ((CurrList.level > Curr.llev)	// restart level
		 || ((CurrList.level > 0)
		  && ListTypesDiffer(CurrList.type[CurrList.level], Curr.list))) {
			PutListEnd(wf, Curr.lfirst || Curr.lend);  // unnesting
			if (CurrList.level > 0) {
				CurrList.iname[CurrList.level] = listitems.find(CurrList.level);
				listitems.remove(CurrList.level);
			}
		}
	}

	if ((CurrList.level == Curr.llev)
	 && !ListTypesDiffer(CurrList.type[CurrList.level], Curr.list))
		return;  // unnesting got back to correct level

	if ((CurrList.level == 0)  // not in a list, must start it
	 || (Curr.lnest) // next is a list that nests
	 || (Curr.llev > CurrList.level)) { // next is nested deeper
		// get up to next lower level if interrupted
		while ((CurrList.level < Curr.llev - 1)	// restart level
		 && (CurrList.level < CurrList.last)) {
			if (CurrList.iname[CurrList.level]
			 && (CurrList.level > 0)) {
				listitems.add(CurrList.iname[CurrList.level], CurrList.level);
				CurrList.iname[CurrList.level] = NULL;
			}
			PutListStart(wf, CurrList.type[CurrList.level + 1], false);
		}

		if (CurrList.iname[CurrList.level]
		 && (CurrList.level > 0)) {
			listitems.add(CurrList.iname[CurrList.level], CurrList.level);
			//CurrList.iname[CurrList.level] = NULL;
		}
		PutListStart(wf, Curr.list, Curr.lfirst);	// start current level
		CurrList.last = CurrList.level;
	}
}


bool 
HTMwr::ListTypesDiffer(unc t1, unc t2)
{
	if (t1 == t2)
		return false;

	if ((t1 == 0) || (t2 == 0))
		return true;  // starting or ending

	if ((t1 >= 1) && (t1 <= 5)) { // ol
		if ((t2 >= 1) && (t2 <= 5))
			return false;
		return true;
	}
	else if ((t1 >= 6) && (t1 <= 8)) { // ul
		if ((t2 >= 6) && (t2 <= 8))
			return false;
		return true;
	}
	else if ((t1 >= 9) && (t1 <= 12)) { // dl etc.
		return true;
	}
	return true;  // both out of range
}

// character properties are stored on a stack


chprop HTMwr::CPTypes[CPTypesMax] = {  // sync with CPtyp enum
 { "b", CPEndBold },
 { "i", CPEndItalic },
 { "u", CPEndUline },
 { "over", CPEndOline },
 { "strike", CPEndStrike },
 { "chbar", CPEndChbar },
 { "sub", CPEndBase },
 { "sup", CPEndBase },
 { "font", CPEndFont }
};

cptype HTMwr::CPStack[CPStackMax];
short HTMwr::CPStackPos = 0;
bool HTMwr::AllowOver = true;
bool HTMwr::NoFont = true;
bool HTMwr::UseFontFace = true;
bool HTMwr::UseFontSize = true;
bool HTMwr::SymbolFont = false;
bool HTMwr::NoSymbolFont = true;
bool HTMwr::UseTypographicElements = true;
bool HTMwr::UseFormatTypo = true;
bool HTMwr::UsePFormatTypo = true;
bool HTMwr::UseCFormatTypo = true;

bool HTMwr::UseTypographicStyles = true;

char **HTMwr::CPStyleTypes[CPTypesMax] = {  // sync with CPtyp enum
 &TypoB, &TypoI, &TypoU, &TypoOline, &TypoStrike, &TypoChbar, &TypoSub, &TypoSup, NULL
};

char *HTMwr::TypoB = NULL;
char *HTMwr::TypoI = NULL;
char *HTMwr::TypoU = NULL;
char *HTMwr::TypoOline = NULL;
char *HTMwr::TypoStrike = NULL;
char *HTMwr::TypoChbar = NULL;
char *HTMwr::TypoSub = NULL;
char *HTMwr::TypoSup = NULL;


void
HTMwr::CPSet(HTMwfile *wf, cptype CPt)
{
	if (CPStackPos >= CPStackMax)  // overflow, error
		return;
	if (CPt >= CPTypesMax)
		return;

	CPStack[CPStackPos] = CPt;
	char *elem = CPTypes[CPt].pstr;

	if (UseTypographicStyles
	 && (CPt != CPFont)
	 && *CPStyleTypes[CPt]
	 && **CPStyleTypes[CPt])
		elem = *CPStyleTypes[CPt];

	CPStackPos++;
	wf->startBlock(elem);
}


void
HTMwr::CPSetBold(HTMwfile *wf)
{
	CPSet(wf, CPBold);
	wf->endElem(true);
	Last.bold = 2;
}


void
HTMwr::CPSetItalic(HTMwfile *wf)
{
	CPSet(wf, CPItalic);
	wf->endElem(true);
	Last.ital = 2;
}


void
HTMwr::CPSetUline(HTMwfile *wf)
{
	CPSet(wf, CPUline);
	wf->endElem(true);
	Last.line = 2;
}


void
HTMwr::CPSetOline(HTMwfile *wf)
{
	CPSet(wf, CPOline);
	wf->endElem(true);
	Last.line = 3;
}


void
HTMwr::CPSetStrike(HTMwfile *wf)
{
	CPSet(wf, CPStrike);
	wf->endElem(true);
	Last.strike = 2;
}


void
HTMwr::CPSetChbar(HTMwfile *wf)
{
	CPSet(wf, CPChbar);
	wf->endElem(true);
	Last.strike = 3;
}


void
HTMwr::CPSetSub(HTMwfile *wf)
{
	if (Xhtml && InPreform)
		return;

	CPSet(wf, CPSub);
	wf->endElem(true);
	Last.base = 2;
}


void
HTMwr::CPSetSup(HTMwfile *wf)
{
	if (Xhtml && InPreform)
		return;

	CPSet(wf, CPSup);
	wf->endElem(true);
	Last.base = 3;
}


void
HTMwr::CPSetFont(HTMwfile *wf)
{
	if ((((finfo *) FontList.find(Curr.fnum))->ftype > 4)
	 && (Curr.fnum != Last.fnum))
		SymbolFont = true;
	else
		SymbolFont = false;

	if ((!SymbolFont && NoFont)
	 || (SymbolFont && NoSymbolFont))
		return;

	bool doFace = false;
	bool doSize = false;
	bool doColor = false;

	if (SymbolFont
	 || ((Curr.fnum != 1)
	  && UseFontFace))
		doFace = true;

	if (!NoFont) {
		if ((Curr.fsize != BaseSize)
		 && !Curr.nosize
		 && UseFontSize)
			doSize = true;
		if ((Curr.colr > 2)
		 && !Curr.nocolor)
			doColor = true;
	}

	if (doFace || doSize || doColor) { // prevent empty font tags
		CPSet(wf, CPFont);
		if (doFace)
			wf->strAttr("face", FontNameList.find(Curr.fnum));
		if (doSize)
			wf->numAttr("size", Curr.fsize);
		if (doColor)
			wf->colorAttr("color", ColorValue(Curr.colr));
		wf->endElem(true);
	}

	Last.fnum = Curr.fnum;
	Last.fsize = Curr.fsize;
	Last.colr = Curr.colr;
}


// ending character props, CPEnd is called for specific props

void
HTMwr::CPEnd(HTMwfile *wf, cptype CPt)
{
	short i;

	for (i = CPStackPos; i > 0; i--)
		if (CPStack[i - 1] == CPt)
			break;

	if (i == 0)  // not on the stack, no action
		return;

	CPEndLevel(wf, i);
}


// CPEndAll clears char props before <A HREF> or <FONT>
void
HTMwr::CPEndAll(HTMwfile *wf)
{
	// clear entire stack
	CPEndLevel(wf, 1);
}


void
HTMwr::CPEndLevel(HTMwfile *wf, short lev)
{
	short i;
	char *elem = NULL;
	char *spc = NULL;
	cptype CPt = CPTypesMax;

	if (lev > CPStackPos)   // underflow
		return;

	//if (HXMwr::DITA)
	//	assert(0);

	for (i = CPStackPos; i >= lev; i--) {
		CPt = CPStack[i - 1];
		elem = CPTypes[CPt].pstr;
		if (UseTypographicStyles
		 && (CPt < CPTypesMax)
		 && (CPt != CPFont)
		 && *CPStyleTypes[CPt]
		 && **CPStyleTypes[CPt]) {
			elem = *CPStyleTypes[CPt];
			if ((spc = strchr(elem, ' ')) != NULL)
				*spc = '\0';
		}
		wf->endBlock(elem);
		if (spc) {
			*spc = ' ';
			spc = NULL;
		}
		(*(CPTypes[CPt].efunc))(wf);
	}

	CPStackPos = i;
}


// these are called only from CPEndLevel

void
HTMwr::CPEndBold(HTMwfile *wf)
{
	Last.bold = 1;
}

void
HTMwr::CPEndItalic(HTMwfile *wf)
{
	Last.ital = 1;
}

void
HTMwr::CPEndUline(HTMwfile *wf)
{
	Last.line = 1;
}

void
HTMwr::CPEndOline(HTMwfile *wf)
{
	Last.line = 1;
}

void
HTMwr::CPEndStrike(HTMwfile *wf)
{
	Last.strike = 1;
}

void
HTMwr::CPEndChbar(HTMwfile *wf)
{
	Last.strike = 1;
}

void
HTMwr::CPEndBase(HTMwfile *wf)
{
	Last.base = 1;
}

void
HTMwr::CPEndFont(HTMwfile *wf)
{
	Last.fnum = 1;
	Last.fsize = BaseSize;
	Last.colr = 0;
	Last.fsym = 0;
	SymbolFont = false;
}



unc HTMwr::CharBlockMacro = MacNone;
unc HTMwr::CharBlockState = CbNone;
char *HTMwr::CurrCSName = NULL;
char *HTMwr::LastCSName = NULL;
char *HTMwr::LastCTagName = NULL;
char *HTMwr::LastDTypoEnd = NULL;
pprop *HTMwr::CurrCSForm = NULL;
pprop *HTMwr::LastCSForm = NULL;


void
HTMwr::StartCharBlock(HTMwfile *wf)
{
	bool usectag = false;
	bool runin = (RuninDiv && (RuninDiv == wf->GetDiv()));

	if (EndingAnum) {
		EndingAnum = false;
		EndCharBlock(wf);
	}

	if ((!XMLSource
	  && Last.hsname
	  && Curr.hsname
	  && _stricmp(Curr.hsname, Last.hsname))
	 || (CharBlockState == CbChanged)) {
		EndCharBlock(wf);
		// WriteText(wf);
	}

	//if (XMLSource)
		CPEndAll(wf);

	if (HXMwr::HXML
	 && !(CharBlockMacro & MacReplace)) {
		if (((LastCTagName = Curr.dtscname) != NULL)
		 && strnicmp(LastCTagName, "No", 2)) {
			usectag = true;
			if (!runin)
				HXMwr::SetDITAStart(wf);
		}
	}

	if (InTextCond) { // existing ph or span, may end it
		if (CheckCondCharBlock()
		 && !(HXMwr::HXML && !usectag)) {
			CondTextEnd(wf);
			CharFormInTextCond = false;
		}
		else
			CharFormInTextCond = true;
	}
	else
		CharFormInTextCond = false;

	if (InHyper) {
		CharFormInLink = true;
		//LinkInTextCond = false;
	}

	if ((CharBlockState == CbStart)
	 && ((CharBlockMacro & MacBefore)
	  || Curr.cmaccb)) {  // CodeBefore
		if (Curr.cmaccb)
			DCmacr::WriteMacro(wf, Curr.cmaccb);
		else
			PutStyleMacro(wf, MacBefore, CurrCSName, CurrCSForm);
	}

	if (CharBlockMacro & MacReplace)
		return;

	SetCondFlags(wf);

	if (HXMwr::HXML) {
		if (usectag) {
			HXMwr::WriteDITAStartCTag(wf, LastCTagName, Curr.cstag, runin);
			wf->SetAttrAdd(true);
			if (CharAttrLp) {
				CharAttrLp->writeall(wf, WriteAttrListItem);
				CharAttrLp = NULL;
			}
			StartCondCharBlock(wf);
			if (CurrCondClass) {
				wf->strAttr("class", CurrCondClass);
				CurrCondClass = NULL;
			}
			if (WriteClassAttributes
			 && !Curr.hsnclass) // already in name string
				wf->setClass(Curr.hscname);
			if (!runin)
				HXMwr::SetDITADivOff(wf);
			wf->endElem(true);
			wf->SetAttrAdd(false);
		}
	}
	else if (Curr.hsname) {
		wf->SetAttrAdd(true);
		if (!Curr.hscname) {
			wf->startBlock(LastCTagName = Curr.hsname);
			if (CharAttrLp) {
				CharAttrLp->writeall(wf, WriteAttrListItem);
				CharAttrLp = NULL;
			}
			StartCondCharBlock(wf);
		}
		else if (ClassIsTag) {
			wf->startBlock(LastCTagName = Curr.hscname);
			if (CharAttrLp) {
				CharAttrLp->writeall(wf, WriteAttrListItem);
				CharAttrLp = NULL;
			}
			StartCondCharBlock(wf);
		}
		else {
			wf->startElem(LastCTagName = Curr.hsname);
			if (CharAttrLp) {
				CharAttrLp->writeall(wf, WriteAttrListItem);
				CharAttrLp = NULL;
			}
			StartCondCharBlock(wf);
			if (WriteClassAttributes
			 && !Curr.hsnclass) // already in name string
				wf->setClass(Curr.hscname);
		}
		wf->endElem(true);
		wf->SetAttrAdd(false);
	}
	Last.hsname = Curr.hsname;

	if ((CharBlockState == CbStart)
	 && ((CharBlockMacro & MacStart)
	  || Curr.cmaccs)) {  // CodeStart
		if (Curr.cmaccs)
			DCmacr::WriteMacro(wf, Curr.cmaccs);
		else
			PutStyleMacro(wf, MacStart, CurrCSName, CurrCSForm);
	}

	if (HXMwr::HXML
	 && usectag
	 && Curr.dtctyps) {
		wf->putStr(Curr.dtctyps);
		LastDTypoEnd = Curr.dtctype;
	}

	if (CharBlockState == CbStart) {
		if (Curr.ddlcst
		 && DropDownUsed) {
			WriteDropLinkStart(wf);
			if (Curr.ddlcend)
				DropLinkCharSet = true;
		}
		CharBlockState = CbStarted;
	}
}


void
HTMwr::EndCharBlock(HTMwfile *wf, bool endcond)
{
	if (!XMLSource
	 || !InlineStackLevel) {
		EndOneCharBlock(wf, endcond);
		return;
	}
	if (!InPara) {      // ending para
		while (InlineStackLevel) {
			EndOneCharBlock(wf, endcond);
			RestoreInline(wf);
		}
	}
}


void
HTMwr::EndOneCharBlock(HTMwfile *wf, bool endcond)
{
	bool usectag = false;
	bool runin = (RuninDiv && (RuninDiv == wf->GetDiv()));

	if (XMLSource)
		CPEndAll(wf);

	if (InTextCond
	 && ((endcond && ChangedCharCond)
	 || !CharFormInTextCond))  { // nested ph or span, end it
		CondTextEnd(wf);
		CharFormInTextCond = false;
	}

	if (!(CharBlockMacro & MacReplace)) {
		if (((CharBlockState == CbEnd)
			|| (CharBlockState == CbChanged))
		 && DropLinkCharSet) {
			DropLinkCharSet = false;
			WriteDropLinkEnd(wf);
		}

		if (HXMwr::HXML) {
			if ((LastCTagName != NULL)
			 && strnicmp(LastCTagName, "No", 2)) {
				usectag = true;
				if (!runin)
					HXMwr::SetDITAEnd(wf);
				if (LastDTypoEnd) {
					wf->putStr(LastDTypoEnd);
					LastDTypoEnd = NULL;
				}
			}
		}

		if (((CharBlockState == CbEnd)
			|| (CharBlockState == CbChanged))
		 && ((CharBlockMacro & MacEnd)
			|| (Curr.cmaccep || Curr.cmacce))) { // CodeEnd
			if (Curr.cmaccep) {
				DCmacr::WriteMacro(wf, Curr.cmaccep);
				Curr.cmaccep = NULL;
			}
			else if (Curr.cmacce) {
				DCmacr::WriteMacro(wf, Curr.cmacce);
				Curr.cmacce = NULL;
				Curr.cmaccep = NULL;
			}
			else
				PutStyleMacro(wf, MacEnd, LastCSName, LastCSForm);
		}

		if (HXMwr::HXML) {
			if (usectag)
				HXMwr::WriteDITAEndCTag(wf, LastCTagName, runin);
		}
		else if (LastCTagName)
			wf->endBlock(LastCTagName);
		else if (Last.hsname)
			wf->endBlock(Last.hsname);
	}

	SetCondFlags(wf);

	Last.hsname = Last.hscname = LastCTagName = NULL;

	if (((CharBlockState == CbEnd)
	  || (CharBlockState == CbChanged))
	 && ((CharBlockMacro & MacAfter)
	  || (Curr.cmaccap || Curr.cmacca))) { // CodeAfter
		if (Curr.cmaccap) {
			DCmacr::WriteMacro(wf, Curr.cmaccap);
			Curr.cmaccap = NULL;
		}
		else if (Curr.cmacca) {
			DCmacr::WriteMacro(wf, Curr.cmacca);
			Curr.cmacca = NULL;
			Curr.cmaccap = NULL;
		}
		else
			PutStyleMacro(wf, MacAfter, LastCSName, LastCSForm);
	}

	if (HXMwr::HXML
	 && usectag
	 && !runin)
		HXMwr::SetDITADivOff(wf);

	if (CharBlockState == CbEnd)
		CharBlockState = CbEnded;
	else if (CharBlockState == CbChanged)
		CharBlockState = CbStart;
	LastCSName = NULL;

	if (CharConds || ChangedCharCond) {
		CharConds = NULL;
		CondChanged = true;
	}
}


void
HTMwr::WriteCharProps(HTMwfile *wf)
{
	//if (InPara)   
	// end blocks before starting them
	WriteEndCharProps(wf);

#if 0
	if (InFormPx  // ignore props for bullets if in list format
	 && Curr.list
	 && !Curr.anuse)
		return;
#endif

	if (Curr.list
	 && (InFormPx          // strip bullets, etc if in list format
	  || (InAnumVal        // also remove anums unless specified
	   && !Curr.anuse)))
		return;

	if ((InAnumGroup || InAnumVal)
	 && (Curr.anskip || (!UseAnums && !Curr.anuse)))
		return;

	if (CondChanged)
		CondTextChange(wf);

	if ((CharBlockState != CbStarted)
	 && ((Curr.hsname
	  && !Last.hsname
	  && (CharBlockState == CbStart))
	 || (CharBlockMacro & (MacBefore | MacStart))
	 || Curr.cmaccb
	 || Curr.cmaccs)) {
		CPEndAll(wf);
		StartCharBlock(wf);
	}

	if (!UseTypographicElements)
		return;

	if (!(Curr.cpuse || AllowOver)
	 && (Curr.osize > 0))
		Curr.fsize = Curr.osize;

	if ((Curr.fnum && (Curr.fnum != 1)
	  && (Curr.fnum != Last.fnum))
	 || (Curr.fsize && (Curr.fsize != BaseSize)
	  && (Curr.fsize != Last.fsize))
	 || (Curr.colr && (ColorValue(Curr.colr) != 0)
	  && (Curr.colr != Last.colr)))
		CPSetFont(wf);

	if (!(Curr.cpuse || AllowOver)) {
		if ((Curr.obold > 1)
		 && (Last.bold < 2)) {
			Curr.bold = Curr.obold;
			CPSetBold(wf);
		}
		if ((Curr.oital > 1)
		 && (Last.ital < 2)) {
			Curr.ital = Curr.oital;
			CPSetItalic(wf);
		}
		if ((Curr.oline > 1)
		 && (Last.line < 2)) {
			Curr.line = Curr.oline;
			if (Curr.oline == 2)
				CPSetUline(wf);
			else if (Curr.oline == 3)
				CPSetOline(wf);
		}
		if ((Curr.ostrike > 1)
		 && (Last.strike < 2)) {
			Curr.strike = Curr.ostrike;
			if (Curr.ostrike == 2)
				CPSetStrike(wf);
			else if (Curr.ostrike == 3)
				CPSetChbar(wf);
		}
	}
	else {
		if ((Curr.bold > 1)
		 && (Last.bold < 2))
			CPSetBold(wf);

		if ((Curr.ital > 1)
		 && (Last.ital < 2))
			CPSetItalic(wf);

		if ((Curr.line > 1)
		 && (Last.line < 2)) {
			if (Curr.line == 2)
				CPSetUline(wf);
			else if (Curr.line == 3)
				CPSetOline(wf);
		}

		if ((Curr.strike > 1)
		 && (Last.strike < 2)) {
			if (Curr.strike == 2)
				CPSetStrike(wf);
			else if (Curr.strike == 3)
				CPSetChbar(wf);
		}
	}

	if ((Curr.base > 1)
	 && (Curr.base != Last.base)) {
		if (Curr.base > 2)
			CPSetSup(wf);
		else CPSetSub(wf);
	}
}


void
HTMwr::WriteEndCharProps(HTMwfile *wf, bool ending)
{
	if ((Last.hsname
	  && (ending
	   || (!InPara)      // ending para
	   || (!Curr.hsname)  // ending style
		 || (!XMLSource
	    && Curr.hsname
	    && _stricmp(Curr.hsname, Last.hsname)))) // changing
	 || ((CharBlockState == CbEnd)
	  && (CharBlockMacro & (MacEnd | MacAfter)))) {
		CPEndAll(wf);
		EndCharBlock(wf);
		return;
	}

	if (!UseTypographicElements)
		return;

	if (ending) {
		CPEndAll(wf);
		return;
	}

	if ((!InPara)
	 || (Curr.fnum && (Curr.fnum != Last.fnum))
	 || (Curr.fsize && (Curr.fsize != Last.fsize))
	 || (Curr.colr && (Curr.colr != Last.colr))) {
		CPEndAll(wf);  // font will always be at top
		return;
	}

	if (Curr.base
	 && (Curr.base != Last.base)
	 && (Last.base > 1))
		CPEnd(wf, (Last.base > 2) ? CPSup : CPSub);

	if ((Curr.strike == 1)
	 && (Last.strike > 1)) {
		if (Last.strike == 2)
			CPEnd(wf, CPStrike);
		else if (Last.strike == 3)
			CPEnd(wf, CPChbar);
	}

	if ((Curr.line == 1)
	 && (Last.line > 1)) {
		if (Last.line == 2)
			CPEnd(wf, CPUline);
		else if (Last.line == 3)
			CPEnd(wf, CPOline);
	}

	if ((Curr.ital == 1)
	 && (Last.ital > 1))
		CPEnd(wf, CPItalic);

	if ((Curr.bold == 1)
	 && (Last.bold > 1))
		CPEnd(wf, CPBold);
}



// title attributes in d2g map references


bool HTMwr::UseMapDescAsTitle = false;
long HTMwr::CurrMapTopicCtl = 0;
bool HTMwr::GettingMapDescText = false;
char *HTMwr::CurrMapDescription = NULL;
DCnlist HTMwr::MapDescList;   // map desc text indexed by topic ctl
DClist HTMwr::MapSkipList;


void 
HTMwr::ProcMapTopicHead(HTMctl *cp)
{
	if (cp->siz() != 0)  // start topic  or head
		CurrMapTopicCtl = ParaRefNum;
}


void 
HTMwr::ProcMapDesc(HTMctl *cp)
{
	if (cp->siz() != 0) {  // start desc
		TitleBufCount = 0;   // use TitleBuf to store desc text
		GettingMapDescText = true;
		CurrMapDescription = NULL;
	}
	else {
		GettingMapDescText = false;
		// get TitleBuf contents into CurrMapDescription
		if ((CurrMapDescription = GetTitleBuf()) == NULL)
			return;

		TitleBufCount = 0;
		MapDescList.add(CurrMapDescription, CurrMapTopicCtl);
		MapSkipList.add(ParaRefNum);
		EmptyParaList.add(ParaRefNum);
		CurrMapDescription = NULL;
		CurrMapTopicCtl = 0;
		NonWhiteText = false;
		WhiteText = true;
	}
}



// tabbing

uns HTMwr::LinePos = 0;
uns HTMwr::CharWid = 1;
char *HTMwr::TabTypes[TabTypeMax] = { "tqc", "tqr", "tqdec" };
uns HTMwr::TabCharWidth = 90;
uns HTMwr::TabCharsPerInch = 16;
bool HTMwr::AnumTabs = true;
bool HTMwr::UseAnums = true;


void
HTMwr::WriteTabDefs(HTMwfile *wf, pprop *style)
{
#ifdef UNUSED
  tinfo *tab;

  for (uns i = 1; i <= style->tcount; i++) {
    tab = (tinfo *) (style->tabs)->find(i);
    assert(tab);
    if (tab->ttype)
      wf->putCW(TabTypes[tab->ttype - 1]);
    if (tab->tlead == '.')
      wf->putCW("tldot");
    else if (tab->tlead == '-')
      wf->putCW("tlhyph");
    wf->putCWN("tx", tab->tpos);
  }
#endif
}


void
HTMwr::WriteTabs(HTMwfile *wf)
{
	//if (InFormPx    // ignore tabs for bullets if in list format
	//if (Curr.list || !AnumTabs)
	//	return;

	if (Curr.list
	 && (InFormPx          // if in list format
	  || (InAnumVal        // remove anums unless specified
	   && !Curr.anuse)))
		return;

	if ((InAnumGroup || InAnumVal) // also if skipping anums elsewhere
	 && (Curr.anskip || (!UseAnums && !Curr.anuse)))
		return;

	if (EndingAnum         // block if starting text but not later
	 && !AnumTabs)
		return;

	if (Curr.tabrep) {
		DCmacr::WriteMacro(wf, Curr.tabrep);
		return;
	}

	if (!(InPreform || HXMwr::CurrPreform)
	 && !InComment) {
		wf->putText(' ');
		return;
	}

	// PRE: put out enough spaces to get to tab stop

	while (LinePos < CurrTab->tpos) {
		wf->putText(' ');
		LinePos++;
	}
}


// text breaks

char HTMwr::PgBreaks = 'N';
bool HTMwr::UnwrapPRE = false;


void
HTMwr::EndLine(HTMwfile *wf, HTMctl *cp)
{
	if (Curr.macpos & MacReplace)
		return;


	if (InComment)
		wf->putEnd();
  else if (cp->siz() == 1) { // hard return
		InTextRun = false;
		if (Xml && !Xhtml) {
			WriteText(wf);
			if (HXMwr::InCurrPreform) { // write closing inline tags first
				wf->putEnd();
			}
			else if (Curr.hrpara) {
				if (HXMwr::HXML)
					HXMwr::EndLevels(wf, HXMwr::DITAParaTagLevel);
				wf->endBlock(HXMwr::CurrTagNames[HXMwr::CurrLevel]);
				wf->putEnd();
				wf->startSBlock(HXMwr::CurrTagNames[HXMwr::CurrLevel]);
			}
			else if (UseXMLbr)
				wf->endLine();
			else
				wf->putChar(' ');
		}
		else {  // HTML or XHTML
			if (Curr.nobreak)
				wf->putChar(' ');
			else if (!InPreform
			 || wf->PreForm
			 || UnwrapPRE) // prevent double return
				wf->endLine();
		}
	}
	else if (WordBreak && InTbCell && TableWordBreak) {
		WordBreak = false;
    wf->putChar('-');
    AddChar('-');
    wf->endLine();
	}
	else if ((InPreform
	  || HXMwr::InCurrPreform)
	 && !UnwrapPRE
	 && !Curr.ignorewrap)
		wf->putEnd();

	LinePos = 0;

#ifdef UNUSED
  LinePos = Curr.lind;
#endif
}


void
HTMwr::WriteBreak(HTMwfile *wf)
{
  if (!StartingPage) {
    switch (Curr.pbreak) {
      case 4:  // page
      case 5:  // left page
      case 6:  // right page
				if (PgBreaks == 'R') {
					wf->putEnd();
		      wf->putElem("hr");
				}
        break;
      case 3:  // column
      default:
        return;
    }
		if (!HXMwr::HXML)
	    wf->putEnd();
    StartingPage = true;
  }
  Curr.pbreak = 0;
}


// starting paragraph


HTMctl *HTMwr::ParaStartCtl = NULL;
HTMctl *HTMwr::TxtParaStartCtl = NULL;
HTMctl *HTMwr::TbCellParaStartCtl = NULL;
HTMctl *HTMwr::PreFtnParaStartCtl = NULL;
HTMctl *HTMwr::PreFtnTxtParaStartCtl = NULL;
HTMctl *HTMwr::PrevStartCtl = NULL;
bool HTMwr::PrevParaIsTb = false;
long HTMwr::ItemRefNum = 0;
long HTMwr::ParaRefNum = 0;
long HTMwr::PrevParaRefNum = 0;
long HTMwr::TxtParaRefNum = 0;
long HTMwr::CharRefNum = 0;
long HTMwr::StartingParaRef = 0;
long HTMwr::StartSplitParaRef = 0;
long HTMwr::PreFtnParaRefNum = 0;
long HTMwr::PreFtnTxtParaRefNum = 0;
bool HTMwr::PrevParaKeepNext = false;
bool HTMwr::ParaKeepNext = false;
frame *HTMwr::PrevParaFrameAfter = NULL;
frame *HTMwr::ParaFrameAfter = NULL;
frame *HTMwr::FrameLinks = NULL;
DClist HTMwr::EmptyParaList;


// scan phase

void
HTMwr::ProcParaStart(HTMctl *cp)
{
	PrevParaRefNum = ParaRefNum;
  cp->siz(ParaRefNum = ++ItemRefNum);  // assign ref to all paras

	if (!InTbl) {  // keep ref to containing text para
		if (PrevParaIsTb == true)
			PrevParaIsTb = false;
		else PrevStartCtl = TxtParaStartCtl;
		TxtParaStartCtl = cp;
		TxtParaRefNum = ParaRefNum;  // leave as para before table
		TxtParaUID = 0;
	}

	if (InTbCell)
		TbCellParaStartCtl = cp;

  ParaStartCtl = cp;
	HyperParaID = (long) cp;

	ParaUID = 0;
	InPara = true;
	Contents = false;
	NoContRef = false;
	LocalTOCLevel = 0;
	XrefLocNext = false;

	ParaStartScan = true;
#if CONDSPANS
	StartPoint = 0;
#endif
	if (!InTbl || AllowTbSplit) {
		SplitHere = false;
		SplitLevel = 0;
	}

	if (!InTbl || AllowTbTitle)
		TitleHere = false;

	if (IntRefPend) {
		IntRefPend->empty();
		delete IntRefPend;
		IntRefPend = NULL;
	}

	PrevParaKeepNext = ParaKeepNext;
	ParaKeepNext = false;
	PrevParaFrameAfter = ParaFrameAfter;
	ParaFrameAfter = NULL;
	FrameLinks = NULL;

	NameHere = false;
	FileNameHere = false;
	IniPartNameHere = false;
	FinalIniNameHere = false;
	ALinkHere = false;
	AttrHere = false;
	GlossHere = false;
	InRaw = false;
	MetaName = NULL;
	ConfigHere = NULL;
	StoreText = TitleHere;

	KeepParaUID = false;

	TextFrameIsText = false;
	WhiteText = NonWhiteText = false;

	if (HXMwr::HXML) {
		HXMwr::DITATag = NULL;
		HXMwr::DITATagID = NULL;
		HXMwr::DITAParent = NULL;
		HXMwr::DITAFirst = NULL;
		HXMwr::DITAOpenBefore = NULL;
		HXMwr::DITAOpenAfter = NULL;
		HXMwr::DITACloseBefore = NULL;
		HXMwr::DITACloseAfter = NULL;
	}
	if (HXMwr::DITA) {
		char *sname = NULL;
		long tnum = 0;
		tagset *dtag = NULL;
		if (HXMwr::DITAStarting) {
			if (((tnum = HXMwr::DTopicTypes->getitem(HXMwr::DefDITATopic)) != 0)
			 && ((dtag = (tagset *) HXMwr::DTagSets->find(tnum)) != NULL)
			 && ((sname = dtag->start) != NULL))
				HXMwr::DITAStartElem = sname;
		}
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

	HyperJump = HyperJumpStarting = false;
	HyperJumpID = 0;
	HyperVal = 0;
	HyperColor = HyperColorEnding = false;
	HyperForm = HyperFormEnding = false;
	HyperFormNumber = 0;
}


pvars HTMwr::SavedParaVars;
tinfo HTMwr::SavedTab;

void
HTMwr::SaveParaVars(void)
{
	// save before table processing
	SavedParaVars.storeText = StoreText;
	SavedParaVars.nonWhiteText = NonWhiteText;
	SavedParaVars.metaName = MetaName;
	SavedParaVars.configHere = ConfigHere;
	SavedParaVars.alinkHere = ALinkHere;
	SavedParaVars.tStoreHere = TStoreHere;
	SavedParaVars.nameHere = NameHere;
	SavedParaVars.splitHere = SplitHere;
	SavedParaVars.fileNameHere = FileNameHere;
	SavedParaVars.finalIniNameHere = FinalIniNameHere;
	SavedParaVars.iniPartNameHere = IniPartNameHere;
	SavedParaVars.contents = Contents;
	SavedParaVars.contentsRef = ContentsRef;
	SavedParaVars.contentsWin = ContentsWin;
	SavedParaVars.contentsId = ContentsID;
	SavedParaVars.localTOCLevel = LocalTOCLevel;
	SavedParaVars.currFormNum = CurrFormNum;
	SavedParaVars.trailLevel = TrailCurrLevel;
	SavedParaVars.titleHere = TitleHere;
	SavedParaVars.titleSet = TitleSet;
	SavedParaVars.inRaw = InRaw;
	SavedParaVars.attrHere = AttrHere;
	SavedParaVars.attrMarkerName = AttrMarkerName;
	SavedParaVars.glossHere = GlossHere;

	SavedParaVars.titleBufCount = TitleBufCount;
	SavedParaVars.titleBuf = TitleBufCount ? (unc *) NewName((char *) TitleBuf) : NULL;

	SavedParaVars.nameBufCount = NameBufCount;
	SavedParaVars.nameBuf = NameBufCount ? (unc *) NewName((char *) NameBuf) : NULL;

	SavedParaVars.dITATag = HXMwr::DITATag;
	SavedParaVars.dITATagID = HXMwr::DITATagID;
	SavedParaVars.dITAAlias = HXMwr::DITAAlias;
	SavedParaVars.dITALevel = HXMwr::DITALevel;
	SavedParaVars.dITAParent = HXMwr::DITAParent;
	SavedParaVars.dITAOpenBefore = HXMwr::DITAOpenBefore;
	SavedParaVars.dITAOpenAfter = HXMwr::DITAOpenAfter;
	SavedParaVars.dITAOpenBeforeArr = HXMwr::DITAOpenBeforeArr;
	SavedParaVars.dITAOpenAfterArr = HXMwr::DITAOpenAfterArr;
	SavedParaVars.dITACloseBefore = HXMwr::DITACloseBefore;
	SavedParaVars.dITACloseAfter = HXMwr::DITACloseAfter;
	SavedParaVars.dITACloseBeforeArr = HXMwr::DITACloseBeforeArr;
	SavedParaVars.dITACloseAfterArr = HXMwr::DITACloseAfterArr;
	SavedParaVars.dITAFirst = HXMwr::DITAFirst;
	SavedParaVars.paraRefNum = ParaRefNum;
	SavedParaVars.intRefPend = IntRefPend;
	IntRefPend = NULL;
	SavedTab = *CurrTab;
}


void
HTMwr::RestoreParaVars(void)
{
	// restore after table processing
	StoreText = SavedParaVars.storeText;
	NonWhiteText = SavedParaVars.nonWhiteText;
	MetaName = SavedParaVars.metaName;
	ConfigHere = SavedParaVars.configHere;
	ALinkHere = SavedParaVars.alinkHere;
	TStoreHere = SavedParaVars.tStoreHere;
	NameHere = SavedParaVars.nameHere;
	SplitHere = SavedParaVars.splitHere;
	FileNameHere = SavedParaVars.fileNameHere;
	FinalIniNameHere = SavedParaVars.finalIniNameHere;
	IniPartNameHere = SavedParaVars.iniPartNameHere;
	Contents = SavedParaVars.contents;
	ContentsRef = SavedParaVars.contentsRef;
	ContentsWin = SavedParaVars.contentsWin;
	ContentsID = SavedParaVars.contentsId;
	LocalTOCLevel = SavedParaVars.localTOCLevel;
	CurrFormNum = SavedParaVars.currFormNum;
	TrailCurrLevel = SavedParaVars.trailLevel;
	TitleHere = SavedParaVars.titleHere;
	TitleSet = SavedParaVars.titleSet;
	InRaw = SavedParaVars.inRaw;
	AttrHere = SavedParaVars.attrHere;
	AttrMarkerName = SavedParaVars.attrMarkerName;
	GlossHere = SavedParaVars.glossHere;

	TitleBufCount = SavedParaVars.titleBufCount;
	if (TitleBufCount) {
		strcpy((char *) TitleBuf, (char *) SavedParaVars.titleBuf);
		DeleteName((char *) SavedParaVars.titleBuf);
	}

	NameBufCount = SavedParaVars.nameBufCount;
	if (NameBufCount) {
		strcpy((char *) NameBuf, (char *) SavedParaVars.nameBuf);
		DeleteName((char *) SavedParaVars.nameBuf);
	}

	HXMwr::DITATag = SavedParaVars.dITATag;
	HXMwr::DITATagID = SavedParaVars.dITATagID;
	HXMwr::DITAAlias = SavedParaVars.dITAAlias;
	HXMwr::DITALevel = SavedParaVars.dITALevel;
	HXMwr::DITAParent = SavedParaVars.dITAParent;
	HXMwr::DITAOpenBefore = SavedParaVars.dITAOpenBefore;
	HXMwr::DITAOpenAfter = SavedParaVars.dITAOpenAfter;
	HXMwr::DITAOpenBeforeArr = SavedParaVars.dITAOpenBeforeArr;
	HXMwr::DITAOpenAfterArr = SavedParaVars.dITAOpenAfterArr;
	HXMwr::DITACloseBefore = SavedParaVars.dITACloseBefore;
	HXMwr::DITACloseAfter = SavedParaVars.dITACloseAfter;
	HXMwr::DITACloseBeforeArr = SavedParaVars.dITACloseBeforeArr;
	HXMwr::DITACloseAfterArr = SavedParaVars.dITACloseAfterArr;
	HXMwr::DITAFirst = SavedParaVars.dITAFirst;
	ParaRefNum = SavedParaVars.paraRefNum;
	IntRefPend = SavedParaVars.intRefPend;
	*CurrTab = SavedTab;
}


pvars HTMwr::SavedParaVarsFtn;

void
HTMwr::SaveParaVarsFtn(void)
{
	// save before footnote processing
	SavedParaVarsFtn.storeText = StoreText;
	SavedParaVarsFtn.metaName = MetaName;
	SavedParaVarsFtn.configHere = ConfigHere;
	SavedParaVarsFtn.alinkHere = ALinkHere;
	SavedParaVarsFtn.tStoreHere = TStoreHere;
	SavedParaVarsFtn.nameHere = NameHere;
	SavedParaVarsFtn.splitHere = SplitHere;
	SavedParaVarsFtn.fileNameHere = FileNameHere;
	SavedParaVarsFtn.finalIniNameHere = FinalIniNameHere;
	SavedParaVarsFtn.iniPartNameHere = IniPartNameHere;
	SavedParaVarsFtn.contents = Contents;
	SavedParaVarsFtn.contentsRef = ContentsRef;
	SavedParaVarsFtn.contentsWin = ContentsWin;
	SavedParaVarsFtn.contentsId = ContentsID;
	SavedParaVarsFtn.localTOCLevel = LocalTOCLevel;
	SavedParaVarsFtn.currFormNum = CurrFormNum;
	SavedParaVarsFtn.trailLevel = TrailCurrLevel;
	SavedParaVarsFtn.titleHere = TitleHere;
	SavedParaVarsFtn.titleSet = TitleSet;
	SavedParaVarsFtn.inRaw = InRaw;
	SavedParaVarsFtn.attrHere = AttrHere;
	SavedParaVarsFtn.attrMarkerName = AttrMarkerName;
	SavedParaVarsFtn.glossHere = GlossHere;

	SavedParaVarsFtn.titleBufCount = TitleBufCount;
	SavedParaVarsFtn.titleBuf = TitleBufCount ? (unc *) NewName((char *) TitleBuf) : NULL;

	SavedParaVarsFtn.nameBufCount = NameBufCount;
	SavedParaVarsFtn.nameBuf = NameBufCount ? (unc *) NewName((char *) NameBuf) : NULL;

	SavedParaVarsFtn.dITATag = HXMwr::DITATag;
	SavedParaVarsFtn.dITATagID = HXMwr::DITATagID;
	SavedParaVarsFtn.dITAAlias = HXMwr::DITAAlias;
	SavedParaVarsFtn.dITALevel = HXMwr::DITALevel;
	SavedParaVarsFtn.dITAParent = HXMwr::DITAParent;
	SavedParaVarsFtn.dITAOpenBefore = HXMwr::DITAOpenBefore;
	SavedParaVarsFtn.dITAOpenAfter = HXMwr::DITAOpenAfter;
	SavedParaVarsFtn.dITAOpenBeforeArr = HXMwr::DITAOpenBeforeArr;
	SavedParaVarsFtn.dITAOpenAfterArr = HXMwr::DITAOpenAfterArr;
	SavedParaVarsFtn.dITACloseBefore = HXMwr::DITACloseBefore;
	SavedParaVarsFtn.dITACloseAfter = HXMwr::DITACloseAfter;
	SavedParaVarsFtn.dITACloseBeforeArr = HXMwr::DITACloseBeforeArr;
	SavedParaVarsFtn.dITACloseAfterArr = HXMwr::DITACloseAfterArr;
	SavedParaVarsFtn.dITAFirst = HXMwr::DITAFirst;
	SavedParaVarsFtn.paraRefNum = ParaRefNum;
	SavedParaVarsFtn.intRefPend = IntRefPend;
	IntRefPend = NULL;
}


void
HTMwr::RestoreParaVarsFtn(void)
{
	// restore after footnote processing
	StoreText = SavedParaVarsFtn.storeText;
	MetaName = SavedParaVarsFtn.metaName;
	ConfigHere = SavedParaVarsFtn.configHere;
	ALinkHere = SavedParaVarsFtn.alinkHere;
	TStoreHere = SavedParaVarsFtn.tStoreHere;
	NameHere = SavedParaVarsFtn.nameHere;
	SplitHere = SavedParaVarsFtn.splitHere;
	FileNameHere = SavedParaVarsFtn.fileNameHere;
	FinalIniNameHere = SavedParaVarsFtn.finalIniNameHere;
	IniPartNameHere = SavedParaVarsFtn.iniPartNameHere;
	Contents = SavedParaVarsFtn.contents;
	ContentsRef = SavedParaVarsFtn.contentsRef;
	ContentsWin = SavedParaVarsFtn.contentsWin;
	ContentsID = SavedParaVarsFtn.contentsId;
	LocalTOCLevel = SavedParaVarsFtn.localTOCLevel;
	CurrFormNum = SavedParaVarsFtn.currFormNum;
	TrailCurrLevel = SavedParaVarsFtn.trailLevel;
	TitleHere = SavedParaVarsFtn.titleHere;
	TitleSet = SavedParaVarsFtn.titleSet;
	InRaw = SavedParaVarsFtn.inRaw;
	AttrHere = SavedParaVarsFtn.attrHere;
	AttrMarkerName = SavedParaVarsFtn.attrMarkerName;
	GlossHere = SavedParaVarsFtn.glossHere;

	TitleBufCount = SavedParaVarsFtn.titleBufCount;
	if (TitleBufCount) {
		strcpy((char *) TitleBuf, (char *) SavedParaVarsFtn.titleBuf);
		DeleteName((char *) SavedParaVarsFtn.titleBuf);
	}

	NameBufCount = SavedParaVarsFtn.nameBufCount;
	if (NameBufCount) {
		strcpy((char *) NameBuf, (char *) SavedParaVarsFtn.nameBuf);
		DeleteName((char *) SavedParaVarsFtn.nameBuf);
	}

	HXMwr::DITATag = SavedParaVarsFtn.dITATag;
	HXMwr::DITATagID = SavedParaVarsFtn.dITATagID;
	HXMwr::DITAAlias = SavedParaVarsFtn.dITAAlias;
	HXMwr::DITALevel = SavedParaVarsFtn.dITALevel;
	HXMwr::DITAParent = SavedParaVarsFtn.dITAParent;
	HXMwr::DITAOpenBefore = SavedParaVarsFtn.dITAOpenBefore;
	HXMwr::DITAOpenAfter = SavedParaVarsFtn.dITAOpenAfter;
	HXMwr::DITAOpenBeforeArr = SavedParaVarsFtn.dITAOpenBeforeArr;
	HXMwr::DITAOpenAfterArr = SavedParaVarsFtn.dITAOpenAfterArr;
	HXMwr::DITACloseBefore = SavedParaVarsFtn.dITACloseBefore;
	HXMwr::DITACloseAfter = SavedParaVarsFtn.dITACloseAfter;
	HXMwr::DITACloseBeforeArr = SavedParaVarsFtn.dITACloseBeforeArr;
	HXMwr::DITACloseAfterArr = SavedParaVarsFtn.dITACloseAfterArr;
	HXMwr::DITAFirst = SavedParaVarsFtn.dITAFirst;
	ParaRefNum = SavedParaVarsFtn.paraRefNum;
	IntRefPend = SavedParaVarsFtn.intRefPend;
}


void
HTMwr::ProcParaEnd(HTMctl *cp)
{
	// store ID for any open HyperJump for HyperPara
	if (HyperJump
	 && !(HyperColor || HyperForm))
		HyperParaList.append(HyperJumpID, HyperParaID);

	if (SplitHere
	 && (!InTbl || AllowTbSplit)) {
		ParaSplitList.append(SplitID, StripTable ? ParaRefNum : TxtParaRefNum);
		if (HXMwr::DITA)
			HXMwr::DITATopicNums.append(HXMwr::DITATopicNum, SplitID);
		if (!SearchHere)
			ExcludeFromSearch.add(SplitID);
		if (!PrintHere)
			ExcludeFromPrint.add(SplitID);
		if (PrintOnly)
			ExcludeFromView.add(SplitID);
	}

	if (!NoSplit)
		LastSplitLevel = SplitLevel;

	if (ExtrFinishing) {
		ExtrEnding = true;
		ExtrFinishing = false;
	}

	if (HXMwr::HXML)
		HXMwr::ProcDITAElemVars(ParaRefNum);

	ProcCondParaEnd();

	if (CharRefNum) {  // collect char attr markers
		MarkList.all(GetCharAttrs);
		SetGlossTitle();
				// for GlossTitle, set CharAttrLp entry for CharTitle
				//  if not already present *****

		CharAttrLp = NULL;
		CharRefNum = 0;
	}
	MarkList.all(GetParaAttrs);  // using ParaRefNum

	if (!NonWhiteText)
		EmptyParaList.add(ParaRefNum);

	char *chp = NULL;
	size_t len = 0;
	char *nmf = NULL;
	long sid = Extracting ? ExtrID : (SplitID ? SplitID : 1);
	char *tref = NULL;

	if (IntRefPend) {
		while ((tref = (char *) IntRefPend->dequeue()) != NULL)
			SetIntRef(tref);
		delete IntRefPend;
		IntRefPend = NULL;
	}

	if (StoreText) {
		PutTitleBuf('\0');  // final terminator
		PutTitleBuf('\0');  // for config settings
		chp = GetTitleBuf(ConfigHere);
	}

	if (chp) {
		if (ConfigHere) {
			DCini::ProcConfigPara(chp, ParaRefNum);
			ConfigHere = false;
		}

		if (MetaName) {
			MetaList.add(chp, ++MetaCount);
			MetaNameList.addnew(MetaName, MetaCount);
			MetaPartList.add((void *) sid, MetaCount);
		}

		if (ALinkHere) {
			ALinkList.addnew(chp, TxtParaRefNum);
			ALinkParaRefList.add(SetXIntRef(), TxtParaRefNum);
		}

		if (NameHere
		 && NameBufCount) {
			if (FileNameHere
			 && (SplitID || Extracting)) {
				if (!UseRawName
				 && (NameBufCount > 0)) {
					if (ChangeFileNameSpaces
					 && (NameBuf[NameBufCount - 1] == FileNameSpaceChar))
						NameBufCount--;
					else if (KeepFileNameSpaces
					 && (NameBuf[NameBufCount - 1] == ' '))
						NameBufCount--;
				}
				char *nm = UseRawName ? NewName(chp) : NewName((char *) NameBuf, NameBufCount);
				char *nmpx = NULL;
				char *nmsx = NULL;
				char *nmorig = BaseNameList.find(sid);

				if (MakeFileHrefsLower) {
					uns i;
					for (i = 0; i < strlen(nm); i++)
						nm[i] = tolower(nm[i]);
				}
				BaseNameList.replace(nm, sid);
				len = NameBufCount + strlen(XrefFileSuffix) + 2;
				if (NamePrefix) {
					nmpx = FixObjectID(NamePrefix, nmorig);
					len += strlen(nmpx);
				}
				if (NameSuffix) {
					nmsx = FixObjectID(NameSuffix, nmorig);
					len += strlen(nmsx);
				}

				if (UseFilePxSxInBaseName
				 && (NamePrefix || NameSuffix)) {
					char *nmb = NewName(len - strlen(XrefFileSuffix));
					sprintf(nmb, "%s%s%s", (NamePrefix ? nmpx : ""), nm,
																 (NameSuffix ? nmsx : ""));
					BaseNameList.replace(nmb, sid);
				}
				nmf = NewName(len);
				sprintf(nmf, "%s%s%s.%s", (NamePrefix ? nmpx : ""), nm,
																 (NameSuffix ? nmsx : ""),
																 XrefFileSuffix);
				SplitNameList.replace(nmf, sid);
			}

			if (FinalIniNameHere) {
				char *nm = NewName((char *) NameBuf, NameBufCount + 4);
				strcat(nm, ".ini");
				FinalIniNameList.add(nm, TxtParaRefNum);
			}

			if (IniPartNameHere) {
				char *nm = NewName((char *) NameBuf, NameBufCount);
				IniPartNameList.add(nm, TxtParaRefNum);
			}
		}

		if (Contents
		 && (LastPage > LastMasterPage)
		 && ContentsRef) {
			cntprop *cpt = new cntprop;
			cpt->title = NewName(chp); 
			cpt->ref = ContentsRef;
			cpt->win = ContentsWin;
			cpt->level = Contents;
			cpt->noref = NoContRef;
			ContentsList.add(cpt, ContentsID);
			ContentsSplitList.append(ContentsID, sid);
			if (!ContentsWindowList.find(sid))
				ContentsWindowList.add(NewName(ContentsWin), sid);
			ParaContentsList.append(ContentsID, TxtParaRefNum);
			ContentsUsed = true;
		}

		if (TitleHere
		 && ((!InTbl)
		  || StripTable
		  || AllowTbTitle)) {
			len = strlen(chp);
			if (TitlePrefix)
				len += strlen(TitlePrefix);
			if (TitleSuffix)
				len += strlen(TitleSuffix);
			nmf = NewName(len);
			sprintf(nmf, "%s%s%s", (TitlePrefix ? TitlePrefix : ""), chp,
														 (TitleSuffix ? TitleSuffix : ""));
			if (SplitID || Extracting)
				SplitTitleList.add(nmf, sid);
			else if (!TitleSet) {
				TitleSet = true;
				Title = nmf;
				SplitTitleList.replace(nmf, 1);
			}
		}

		if (HXMwr::DITA)
			HXMwr::ProcDITAParaVars(chp);

		// if stored text is for use as attribute, treat as marker:
		if (AttrHere) {
			if (AttrMarkerName)
				AddAttrMarker(AttrMarkerName, NewName(chp));
			AttrHere = false;
			AttrMarkerName = NULL;
		}

		if (TStoreHere) {
			//long val;
			//if ((val = TextStoreNames(TStoreName)) != 0)
			//	TextStoreVals.replace(NewName(chp), val);
			//else {
				uns len = strlen(chp);
				if (len > 0) {
					TextStoreNames.add(TStoreName, ++TextStoreCount);
					if ((chp[len - 1] == ' ')
					 || (chp[len - 1] == '\n'))
						len--;
					TextStoreVals.add(NewName(chp, len), TextStoreCount);
					TextStoreIDs.append(sid, TextStoreCount);
				}
			//}
			TStoreHere = false;
		}

		// handle local TOC
		if (MakeLocalTOC
		 && LocalTOCLevel) {
			if (SplitID || Extracting)
				LocalTOCTitleList.add(NewName(chp), sid);
			else 
				LocalTOCTitleList.replace(NewName(chp), 1);
			AddLocalTOCItem();
		}

		// handle trails
		if (MakeTrail
		 && TrailCurrLevel) {
			len = strlen(chp);
			if (TrailPrefix)
				len += strlen(TrailPrefix);
			if (TrailSuffix)
				len += strlen(TrailSuffix);
			nmf = NewName(len);
			sprintf(nmf, "%s%s%s", (TrailPrefix ? TrailPrefix : ""), chp,
														 (TrailSuffix ? TrailSuffix : ""));
			if (SplitID || Extracting)
				TrailTitleList.add(nmf, sid);
			else 
				TrailTitleList.replace(nmf, 1);
			AddTrailItem();
		}

		if (OmniHelp
		 && UseFTS
		 && SearchHere
		 && !ExcludedTopics->find(SplitID))
			(UnicodeFTS ? ProcOHUnicodeSearch : ProcOHSearch)(chp, ContentsID);

		if (GlossHere) {
			GlossParaList.addnew(chp, TxtParaRefNum);
			char *id = MakeGlossID(chp);
			GlossIDList.add(id, TxtParaRefNum);
			SetIntRef(id + 1);
		}
	}

	TitleBufCount = 0;
	NameBufCount = 0;
	InPara = false;
	if (StartingParaRef)
		FileStarted = true;
}


char *
HTMwr::FixObjectID(char *mac, char *orig)
{
	char *nmac = mac;
	long len = strlen(mac);
	char *imac = NewName(mac);
	_strlwr(imac);
	char *obid = strstr(imac, "<$$_objectid>");

	if (obid != NULL) {
		obid = (obid - imac) + mac;
		nmac = NewName((len - 13) + strlen(orig));
		if (obid > mac)
			strncpy(nmac, mac, obid - mac);
		strcat(nmac, orig);
		if ((obid + 13) < (mac + len))
			strcat(nmac, obid + 13);
	}
	DeleteName(imac);

	return nmac;
}

#if 0
char *
HTMwr::FixVariables(char *mac)
{
	char *nmac = mac;
	long len = strlen(mac);
	char *mvar = NULL;
	long pos = 0;
	char *evar = NULL;
	char *varname = NULL;
	char *var = NULL;
	char *pmac = NULL;

// look for <$$
	while ((mvar = strstr(nmac + pos, "<$$")) != NULL) {
		if ((evar = strchr(mvar, '>')) == NULL)
			break;
		varname = NewName(mvar + 3, (evar - mvar) - 3);
		if (((var = GetUserMacVar(varname)) != NULL)
		 && strcmp(var, "0")) {  // if var defined with value, replace
			pmac = ReplaceMacro(nmac, mvar, evar, var);
			pos = mvar - nmac;
			if (nmac != mac)
				DeleteName(nmac);
			nmac = pmac;
		}
		else  // leave var in place for later eval
			pos++;
	}
}

char *
HTMwr::ReplaceMacro(char *str, char *sname, char *ename, char *val)
{
	// replace str content from sname through ename with val
}
#endif


DCvlist HTMwr::PRefFrameList;
DCvlist HTMwr::PRuninFrameList;
long HTMwr::RuninRefNum = 0;
bool HTMwr::AnchorFrames = true;
bool HTMwr::GraphANamesFirst = true;
long HTMwr::CurrGraphFrameID = 0;


void
HTMwr::ReAnchorFrame(void *vp)
{
	if (!AnchorFrames)
		return;
  PRefFrameList.add(vp, ParaRefNum);
  if (Verbose)
    printf("anchor frame at 0x%x to para %d\n", vp, ParaRefNum);
}


void
HTMwr::ProcFrAnchor(HTMctl *cp)
{
  frame *fr;

  if ((fr = (frame *) FrameList.find(cp->dat())) != NULL) {
		if (fr->ftext
		 && TextFrameIsText)
			TextFrameIsTextList.add( (long) fr);

    if (fr->fpos == 2) {  // need to put out frame at start of para
      PRefFrameList.add(fr, ParaRefNum);
      if (Verbose)
        printf("add preceding frame at 0x%x to para %d\n", fr, ParaRefNum);
    }
    else if (fr->fpos == 3) {  // need to put out runin frame
      PRuninFrameList.add(fr, ParaRefNum);
      if (Verbose)
        printf("add runin frame at 0x%x to para %d\n", fr, ParaRefNum);
    }
		else if (!ParaFrameAfter) // frame goes after para
			ParaFrameAfter = fr;

    if (!SectUsedList.find(SectNum))
      SectUsedList.add(SectNum);

		GraphIni.nm[0] = fr->grname;
		GraphIni.cnt = 1;

		DCini::GraphSetList.all(DCini::SetGraphID);
		fr->grgroup = DCini::PendingGraphGroup;
		DCini::PendingGraphGroup = NULL;

		CurrGraphFrameID = fr->id;
		MarkList.all(GetGraphAttrs);
		fr->attrlist = GraphAttrLp;
		GraphAttrLp = NULL;
  }
}


void
HTMwr::ExecFrame(DCfile *wf, void *vp)
{
	long pddiv = 0;
	long dsdiv = 0;
	if (HXMwr::HXML) {
		pddiv = HXMwr::PreDITADiv;
		dsdiv = HXMwr::DITAStartDiv;
	}
  if (Verbose)
    printf("  write frame at 0x%x\n", vp);
  WriteFrame( (HTMwfile *) wf, (frame *) vp);
	if (HXMwr::HXML) {
		HXMwr::PreDITADiv = pddiv;
		HXMwr::DITAStartDiv = dsdiv;
	}
}



// write phase

DCnlist HTMwr::CodeStoreParaList;
DCnlist HTMwr::CodeStoreDivList;
char *HTMwr::CodeStoreVar = NULL;
long HTMwr::CodeStoreDiv = 0;
long HTMwr::PreCodeStoreDiv = 0;
bool HTMwr::NoParaClose = false;
bool HTMwr::NoWrap = false;


void
HTMwr::StartPara(HTMwfile *wf, HTMctl *cp)
{
#ifdef UNUSED
  if (WritingFrame)
		return;
#endif

	ParaStarting = true;
	ParaStartProps = false;
	PrevParaRefNum = ParaRefNum;

	if (!(ParaRefNum = cp->siz()))
    return;
	if (((XrefIDRef = XrefParaList.find(ParaRefNum)) == NULL)
	 || (*XrefIDRef == '\0'))
		XrefIDRef = "0";

	long num = 0;

	if ((num = ParaContentsList.find(ParaRefNum)) != 0)
		ContentsID = num;

	if (ExtrEndList.find(ParaRefNum))
		EndFileExtract(wf);

	long spID = ParaSplitList.find(ParaRefNum);
	if (spID != 0)
		SplitFile(wf, spID);
	else if ((spID = ExtrStartList.find(ParaRefNum)) != 0)
		StartFileExtract(wf, spID);

	if (HXMwr::DITA) {
		if ((num = HXMwr::DITAParaTopicNums.find(ParaRefNum)) != 0) {
			if (HXMwr::DITATopicNum != num) {
				HXMwr::DITATopicNum = num;
				HXMwr::InitDITATopicFile(wf, spID ? spID : 1);
			}
		}
		else if (HXMwr::DITATopicNum == 0) {
			// HXMwr::DITATopicNum = HXMwr::DTopicTypes->getitem(HXMwr::DefDITATopic);
			HXMwr::DITATopicNum = 1;
			HXMwr::InitDITATopicFile(wf, 1);
		}
	}

	// see if HyperPara, set to start anchor if so
	if ((HyperJumpID = HyperParaList.find((long) cp)) != 0)
		HyperPara = true;
	else HyperPara = false;

	ParaAttrLp = (DCnlist *) ParaAttrLists.find(ParaRefNum);

	CurrMapDescription = MapDescList.find(ParaRefNum);

	if (MapSkipList.find(ParaRefNum)) {
		NonWhiteText = false;
		WhiteText = true;
	}
	else
		WhiteText = NonWhiteText = false;

	char *nm = NULL;
	if (((nm = FinalIniNameList.find(ParaRefNum)) != NULL)
	 && (!FinalIniName || strcmp(FinalIniName, nm))) {
		if (FinalIniName)  // changing, remove old one
			delete FinalIniFile;
		if (!strcmp(nm, "0"))
			nm = NULL;
		FinalIniName = nm;
		FinalIniFile = nm ? new DCirfile(nm) : NULL;
		FinalIniParent->SetSecFile(FinalIniFile);
	}

	if (((nm = IniPartNameList.find(ParaRefNum)) != NULL)
	 && (!IniPartName || strcmp(IniPartName, nm))) {
		IniPartName = nm;
		IniFile->SetType(IniPartName);
	}

	char *aname = NULL;
	ALinkPara = false;
	ALinkRef = NULL;
	if ((aname = ALinkList.find(ParaRefNum)) != NULL) {
		ALinkRef = ALinkParaRefList.find(ParaRefNum);
		WriteALinkObject(wf, aname);
		ALinkPara = true;
	}
	if (ALinkMarkerList.find(ParaRefNum))
		ALinkPara = true;

	if (!FootDiv || !InFootnote)
		CodeStoreVar = CodeStoreParaList.find(ParaRefNum);

  InPara = false;
	ParaUID = 0;
	DropLinkStarted = false;
	DropLinkCharSet = false;

  if (Verbose)
    printf("at para %ld:\n", ParaRefNum);
}


void
HTMwr::EndPara(HTMwfile *wf)
{
	bool xmlEnded = true;

	if (IndexEntry)
		WriteIndexEntry(wf);

	if (InTextCond
	 && !InHyper)  { // existing ph or span, end it
		if (CharFormInTextCond) {
			RestoreForm();
			if (!InPreform)
				WriteEndCharProps(wf, true);
		}
		CondTextEnd(wf);
		CharFormInTextCond = false;
	}

	RestoreForm();

	if ((Curr.macpos & MacReplace)   // end of replacement
	 && (!FootDiv || !InFootnote)) {
		if (!ParaStartProps)
			WriteParaStartProps(wf);
		Curr.macpos &= ~MacReplace;  // 0xFB
		if (CodeStoreDiv) {  // end any pending div
			wf->SelectDiv(PreCodeStoreDiv);
			CodeStoreDiv = 0;
		}

		if (MakeTrail
		 && Curr.trail
		 && ((TrailPosition == 'A')
		  || (TrailPosition == 'B')))
			WriteTrail(wf);

		InPara = false;
		ParaStarting = false;
		ParaEnding = false;
		if (PreSufFrames || PreSufTbls)
			WriteParaFramesAndTables(wf, &PreSufFrames, &PreSufTbls);
		if (FramePendList || TblList)
			WriteParaFramesAndTables(wf, &FramePendList, &TblList, &FigTable);
		return;
	}

	if (CondsEffective
	 && !CondFlagDiv) {
		PreCondFlagDiv = wf->GetDiv();
		CondFlagDiv = wf->NewDiv();
	}

	if (InComment) {
		if (!MinimizeLines)
			wf->putEnd();
		if (Curr.macpos & MacEnd)   // macro end
			PutStyleMacro(wf, MacEnd);
		else if (Curr.macce)
			DCmacr::WriteMacro(wf, Curr.macce);
		if (CodeStoreDiv) {  // end any pending div
			wf->SelectDiv(PreCodeStoreDiv);
			CodeStoreDiv = 0;
		}
		if (Curr.macpos & MacAfter)	// macro after (before comment end, arrrgh,
			PutStyleMacro(wf, MacAfter);  // but no good way to put it after...)
		else if (Curr.macca)
			DCmacr::WriteMacro(wf, Curr.macca);
		InPara = false;
		return;
	}

	if (ParaStarting) {
		if ((WhiteText
		  || (InTbCell ? RemEmptyTbParas : RemEmptyParas)
		  || HXMwr::DITANoBlock)
		 && !(NonWhiteText
			|| HXMwr::InCurrPreform
			|| InPreform
			// || (PreSufFrames || PreSufTbls)
			// || (FramePendList || TblList)
		  || Curr.pfx
		  || Curr.sfx
		  || PRefFrameList.find(ParaRefNum))) {
			ParaStarting = InPara = false;
			SaveState();
			if (PreSufFrames || PreSufTbls)
				WriteParaFramesAndTables(wf, &PreSufFrames, &PreSufTbls);
			frame *fr = NULL;
			if (RetainRuninImagesForEmptyParagraphs
			 && ((fr = (frame *) PRuninFrameList.find(ParaRefNum)) != NULL)
			 && (RuninRefNum != ParaRefNum)) { // was skipped
				RuninRefNum = ParaRefNum;
				WriteFrame(wf, fr);
				RuninRefNum = 0;
			}
			if (FramePendList || TblList)
				WriteParaFramesAndTables(wf, &FramePendList, &TblList, &FigTable);
			RestoreState();
			if (HXMwr::HXML)
				HXMwr::WriteDITACloseBlocks(wf, (char **) HXMwr::DITAParaCloseAfterList.find(ParaRefNum));
			return;
		}
		ParaEnding = true;
		WriteText(wf);
		WritePendingANames(wf);
		if (!InPreform) {
			if (Last.strike > 1) {
				Curr.strike = 1;
				if (Last.strike == 2)
					CPEnd(wf, CPStrike);
				else if (Last.strike == 3)
					CPEnd(wf, CPChbar);
			}
			if (Last.line > 1) {
				Curr.line = 1;
				if (Last.line == 2)
					CPEnd(wf, CPUline);
				else if (Last.line == 3)
					CPEnd(wf, CPOline);
			}
			if (!Curr.txframes
			 && !Curr.raw
			 && !Curr.nopara)
				wf->putStr(InTbCell ? EmptyTbCellContent : EmptyParaContent);
		}
		InPara = true;
		StartingPage = false;
	}

	ParaStarting = false;

	if (!InPreform)
		WriteEndCharProps(wf, true);

	// write ending anchor for HyperPara
	if (InHyper)
		EndAHref(wf);

#if 0
	if (RuninDiv) {
		if (RuninProps.runpunct
		 && *(RuninProps.runpunct))
			wf->putStr(RuninProps.runpunct);
		else if (RuninProps.rungroup)
			(RuninProps.rungroup)->write(*wf);
	}
#endif

	if (HXMwr::HXML
	// && !HXMwr::DITANoBlock
	 && Curr.dtptype)
		wf->putStr(Curr.dtptype);

	if (Curr.ddlpend)
		WriteDropLinkEnd(wf);

	if (Curr.macpos & MacEnd)   // macro end
		PutStyleMacro(wf, MacEnd);
	else if (Curr.macce)
		DCmacr::WriteMacro(wf, Curr.macce);

	if (CodeStoreDiv   // end any pending div
	 && (!FootDiv || !InFootnote)) {
		wf->SelectDiv(PreCodeStoreDiv);
		// CodeStoreDiv = 0;
	}

	// for DITA, start closing div here, save div number in own stack ****
	if (HXMwr::HXML
	// && !HXMwr::DITANoBlock
	 && !InFootnote
	 && !HXMwr::PreDITARuninDiv)
		HXMwr::SetDITAEnd(wf);

	char *endtag = NULL;

	if (!RuninDiv) {
		char *tag = HXMwr::HXML ?
		           (HXMwr::DropInvalidParaTag ? HXMwr::CurrTagNames[HXMwr::DITAParaTagLevel] :
		            Curr.dtcname) :
		           (ClassIsTag ? Curr.hcname :
		            Curr.hpname);
		if (!InPreform) {
			if (!Curr.list
			 && !Curr.notags
			 && !Curr.nopara
			 && (!(HXMwr::HXML && HXMwr::DITANoBlock))
			 && (!NoParaClose || strcmp(tag, "p"))) {
				if (!InFootnote) {
#if 0
					if (Xml && NestXML)
						xmlEnded = EndNestedXmlBlock(wf, tag);
					else
#endif
					if (CodeStoreDiv)
						wf->SelectDiv(CodeStoreDiv);
					wf->endBlock(endtag = tag);
				}
				else if (FootInlineParaTag
				 && (FootType == 'I'))
					wf->endBlock(endtag = FootInlineParaTag);
			}
			if (!HXMwr::HXML
			 || Curr.ignorewrap)
				wf->putEnd();
			else if (!InTbCell
			 && !InTbTitle
			 && !(HXMwr::HXML && InFootnote)
			 && !(InPreform || HXMwr::InCurrPreform))
				wf->putEnd(); // blank line between paras
		}
		else if (UnwrapPRE
		 || Curr.ignorewrap)
			wf->putEnd(); // not using soft rets, so use para end
		LinePos = 0;
	}

  InPara = false;
	if (CodeStoreDiv) {
		if (HXMwr::DITAEndDiv)
			wf->SelectDiv(HXMwr::DITAEndDiv);
		else
			wf->SelectDiv(PreCodeStoreDiv);
		CodeStoreDiv = 0;
	}

	// Frames at anchor, outside column, or outside frame, and
	// tables at "anywhere" or float, go before the suffix frame
	if (!HXMwr::HXML && (PreSufFrames || PreSufTbls) && Curr.sfx) {
		SaveState();
		WriteParaFramesAndTables(wf, &PreSufFrames, &PreSufTbls);
		RestoreState();
	}

	if (Curr.sfx) { // suffix frame
		frame *fr = (frame *) FrameList.find(Curr.sfx);
		if (fr)
			WriteFrame(wf, fr, true);
	}

	// macro after
	if ((Curr.macpos & MacAfter)
	 && xmlEnded)
		PutStyleMacro(wf, MacAfter);
	else if (Curr.macca
	 && xmlEnded)
		DCmacr::WriteMacro(wf, Curr.macca);

	if (Curr.ddbend
	 && DropDownUsed)
		DCmacr::WriteMacro(wf, "<$DropBlockEnd>");
	if (Curr.ddbsta
	 && DropDownUsed)
		DCmacr::WriteMacro(wf, "<$DropBlockStart>");

	if (MakeTrail
	 && Curr.trail
	 && (TrailPosition == 'A'))
		WriteTrail(wf);

	ParaEnding = false;

	// for DITA, end closing div here; contains end tag and CodeAfter
	if (HXMwr::HXML
	// && !HXMwr::DITANoBlock
	 && !InFootnote)
		HXMwr::WriteDITAEndPTag(wf, endtag);

	if (HXMwr::HXML && (PreSufFrames || PreSufTbls))
		WriteParaFramesAndTables(wf, &PreSufFrames, &PreSufTbls);

	WriteParaFramesAndTables(wf, &FramePendList, &TblList, &FigTable);
}

#if 0
; ANameTagsFirst = Yes (default, puts out <a name=...> tags before frames
;  placed above their titles, if the anchor para has KeepWithNext and/or the
;  title para has KeepWithPrev), or No (as before, leave tags in title para)
ANameTagsFirst=Yes
#endif

void
HTMwr::WriteParaFramesAndTables(HTMwfile *wf,
	          DClist **fmlist, DClist **tblist, tbl **ftbl)
{
	tbl *ftb = NULL;
	if (ftbl)
		ftb = *ftbl;
	tbl *tb = NULL;
	DClist *flist = *fmlist;
  *fmlist = NULL;
	DClist *tlist = *tblist;
	*tblist = NULL;
	tbl *ctb = Tbl;

  if (flist && !WritingFrame) {
    uns fnum = 1;
    frame *fr;
    while ((fr = (frame *) flist->number(fnum++)) != NULL) {
			WritePendingANames(wf);
      WriteFrame(wf, fr);
		}
    delete flist;
  }
	else if (flist)
		*fmlist = flist;

	if (tlist && !InTbl && !InTbCell && !InTbTitle) {
		uns tnum = 1;
		while ((tb = (tbl *) tlist->number(tnum++)) != NULL) {
			WritePendingANames(wf);
			Tbl = tb;
			WriteTbl(wf);
		}
		delete tlist;
	}
	else if (tlist)
		*tblist = tlist;

	if (ftb
	 && !FigureOpen
	 && ftb->titlecont) {
		FigureOpen = true;
		(ftb->titlecont)->write(*wf);
		wf->putEnd();
	}

	Tbl = ctb;
}

#if 0

bool HTMwr::NestXML = false;
long HTMwr::XMLNestLevel = 0;
DCvlist HTMwr::XMLNestList;


void
HTMwr::StartNestedXmlBlock(HTMwfile *wf, char *tag)
{
	if (Curr.xnlev == 0)  // para does not cause unnesting
		return;

	// put out end tags and CodeAfter for all in stack with
  // xnlev >= Curr.xnlev and remove them from stack
	if (!XMLNestLevel)
		return;
	
	xnest *xitem = NULL;
	while (XMLNestLevel) {
		xitem = (xnest *) XMLNestList.find(XMLNestLevel);
		if (!xitem)  // error, item missing
			break;
		if (xitem->lev < Curr.xnlev)  // higher level item, done
			break;
		wf->endBlock(xitem->tag);
		if (xitem->mactxt) {
			DCmacr::WriteMacro(wf, xitem->mactxt);
			DeleteName(xitem->mactxt);
		}
		XMLNestList.remove(XMLNestLevel--);
	}
	wf->putEnd();
}

bool
HTMwr::EndNestedXmlBlock(HTMwfile *wf, char *tag)
{
	if (Curr.xnlev == 0) {  // para doesn't contain others
		wf->endBlock(tag);
		return true;
	}

	// add current tag to stack
	xnest *xitem = new xnest;
	xitem->tag = tag;
	xitem->lev = Curr.xnlev;
	if (IniFile->Section("HtmlStyleCodeAfter")
	 && IniFile->Find(Curr.stag))
		xitem->mactxt = IniFile->StrRight();
	XMLNestList.add(xitem, ++XMLNestLevel);

	return false;  // not closed, don't do Code After yet
}

void
HTMwr::EndAllNestedXmlBlocks(HTMwfile *wf)
{
	if (!XMLNestLevel)
		return;
	
	xnest *xitem = NULL;
	while (XMLNestLevel) {
		xitem = (xnest *) XMLNestList.find(XMLNestLevel);
		if (!xitem)  // error, item missing
			break;
		wf->endBlock(xitem->tag);
		if (xitem->mactxt) {
			DCmacr::WriteMacro(wf, xitem->mactxt);
			DeleteName(xitem->mactxt);
		}
		XMLNestList.remove(XMLNestLevel--);
	}
	wf->putEnd();
}
#endif

// end of dwhtmtx.cc


