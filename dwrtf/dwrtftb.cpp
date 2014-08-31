 
// dwrtftb.cc is the code for dcl Microsoft RTF table writing, JHG, 2/6/97
// Copyright (c) 1993-1997 by Omni Systems, Inc.  All rights reserved.

#include "dcappl.h"
#include "dcctlg.h"
#include "dcfili.h"
#include "dctxfx.h"
#include "dcfmtr.h"
#include "dcini.h"
#include "dcmacr.h"
#include "dwrtf.h"
#include "dwrtffl.h"



// processing switch


RTFctl* RTFwr::TableStartCtl = NULL;


bool
RTFwr::ProcTb(RTFctl *cp)
{
  switch((cp->rp)->scan_op) {
    case nothing:   // just put with its group for later
      break;

    case pass:      // ignore, don't save
      return false;

    case ruleprop:  // process rule properties
      if (!InRuleDef)
        return ProcGr(cp);
      ProcRuleProp(cp);
      return false;

		case tblstart:  // track state during scan
			InTbl = true;
			TableStartCtl = cp;
	    cp->siz(++ItemRefNum);
			PrevStartCtl = TableStartCtl;
			PrevParaIsTb = true;
			if (StripTables)
				StripTableList.add((long) cp);
			break;

		case tblend:
			InTbl = false;
			TableStartCtl = NULL;
			CellStradCount = 0;
			break;

		case tbcstart:
		  InTbCell = true;
			StartingTbCell = true;
			TbCellParaStartCtl = NULL;
			if (CellStradCount) {
				InStradTbCell = true;
				CellStradCount--;
				if (!CellStradCount)
					LastStradTbCell = true;
			}
			break;

		case endtbc:
		  InTbCell = false;
			StartingTbCell = false;
			if (TbCellParaStartCtl != NULL)
				TbCellParaEndList.add(ParaRefNum);
			if (RuninPara)
				NoRuninPara.add(ParaRefNum);
			ParaEnding = false;
			InStradTbCell = false;
			FirstStradTbCell = LastStradTbCell = false;
			break;

		case tbcstrd:
			CellStradCount = (uns) (cp->siz() - 1);
			if (CellStradCount)
				FirstStradTbCell = true;
			break;

    case tformck:   // process props in table formats
      if (InTblDef)
        ProcTblProp(cp);
      break;

    case formchk:   // process cell props in formats
      if (InTblDef)
        ProcTblProp(cp);
      else if (InFormDef) {
        SetCellMarg(cp);
        return false;
      }
      break;

		case condrset:
#if 0
      if (!InTblDef
			 && !InTDefaults
			 //&& FirstCol
			 && (LastPage > LastMasterPage))
				ProcCondRowUse(cp->dat());
#endif
			break;

    default:
      assert(0);
      return false;
  }
  return true;
}



// table properties


void
RTFwr::ProcTblProp(RTFctl *cp)
{
  //RTFctl *ncp;
  unl *lptr;
  short i;

  if (cp->rp == NULL)
    return;
	if (InTblCell)
		return;

  switch ((cp->rp)->write_op) {
    case ignore:
      break;

    case formtbl:
      UseTblForm(cp);
      break;

    case tbplace:
      SaveState();
			if (InTblDef) {
	      Curr.lind = Curr.aspace = Curr.bspace = 0;
		    Curr.algn = Curr.keep = 0;
			  Curr.pbreak = 0;
			}
			else {
				Curr.algn = (unc) Tbl->tblalgn;
				Curr.lind = Tbl->tbllind;
				Curr.bspace = Tbl->tblbspc;
				Curr.aspace = Tbl->tblaspc;
				Curr.pbreak = Tbl->tblbrk;
				Curr.keep = (unc) Tbl->tblkeep;
			}
      break;

    case putalgn:
    case putindt:
    case putpspc:
    case putbreak:
    case putkeep:
      SetCurrProps(cp);
      if (InTbCell)
        AddCellCont(cp);
      break;

    case tbeplac:
      Tbl->tblalgn = Curr.algn;
      Tbl->tbllind = Curr.lind;
      Tbl->tblbspc = Curr.bspace;
		 	if (Curr.aspace > Tbl->tblaspc)
				Tbl->tblaspc = Curr.aspace;
      Tbl->tblbrk = Curr.pbreak;
      Tbl->tblkeep = Curr.keep;
      RestoreState(FullState);
      break;

    case tbtplace:
			if (TbTitleType != 0)
				Tbl->titleplace = (uns) TbTitleType;
			else
				Tbl->titleplace = (uns) cp->siz();
      break;
    case tbtgap:
      //Tbl->titlegap = (uns) cp->siz(); // will convert to twips later
			Tbl->titlegap = GetTwips(cp->siz(), ld_unit);
			break;
    case tbltitl:
      InTbTitle = true;
      if (!InTDefaults) {  // set up group to get title
        Tbl->titlecont = new RTFgroup;
				TblTitleTextAdded = false;
      }
      break;
    case tbetitl:
      if (!InTDefaults) {  // done collecting title
#if 0
				if (TblTitleParaStarted == false)	{
          ncp = new RTFctl(imnem, 2, 11, 1);
          ncp->rp = new rtf;
          (ncp->rp)->write_op = parstart;
	        (Tbl->titlecont)->add(*ncp);
				}
				if (TblTitleParaEnded == false)	{
					if (TblTitleTextAdded == false) {
						ncp = new RTFctl(imnem, 2, 10, 1, 0xA0);  // nonbreaking space
						ncp->rp = new rtf;
						(ncp->rp)->write_op = puttx;
						ProcTblProp(ncp);  // recurse to ensure title gap
					}
          ncp = new RTFctl(imnem, 2, 11, 2);
          ncp->rp = new rtf;
          (ncp->rp)->write_op = parend;
	        (Tbl->titlecont)->add(*ncp);
				}
#endif
			}
      InTbTitle = false;
      break;

    case tbldflt:
      InTDefaults = true;
      break;
    case tbedflt:
      InTDefaults = false;
      break;

    case tblrull:
      assert(Tbl);
      GetRule(cp, &Tbl->brdrl);
      break;
    case tblrulr:
      assert(Tbl);
      GetRule(cp, &Tbl->brdrr);
      break;
    case tblrult:
      assert(Tbl);
      GetRule(cp, &Tbl->brdrt);
      break;
    case tblrulb:
      assert(Tbl);
      GetRule(cp, &Tbl->brdrb);
      break;

    case tbrrper:
      assert(Tbl);
			Tbl->tblrrper = (uns) cp->siz();
			break;
    case tbrralt:
      assert(Tbl);
      GetRule(cp, &Tbl->brdra);
      break;
    case tbatype:
      assert(Tbl);
			Tbl->tblshtyp = (uns) cp->siz() + 1;
			break;
    case tbapern:
      assert(Tbl);
			Tbl->tblshper = (uns) cp->siz();
			break;
    case tbapera:
      assert(Tbl);
			Tbl->tblaltper = (uns) cp->siz();
			break;
    case tbafill:
      assert(Tbl);
      //GetFill(cp, &Tbl->tblaltfill);
			GetShading(cp, &Tbl->tblaltshd);
      break;
    case tbacolr:
      assert(Tbl);
      //GetColor(cp->siz(), &Tbl->tblaltcolr);
			//printf("Tbl->tblaltshd is at %0.4lx containing %0.4lx.\n",
			//                         &Tbl->tblaltshd, Tbl->tblaltshd);
			GetShadeColor(cp, &Tbl->tblaltshd);
      break;

    case tbcol:
      ProcTblCol(cp);
      break;
    case tbcend:
      TbCol = 0;
      break;
		case tbcwtyp:
			TbColWidPercent = cp->siz() ? true : false;
			break;
    case tbcwid:
      if (TbCol)
        Col->colwid = AdjColWid(cp->siz());
      break;
    case tbcwids:
      lptr = (unl *) cp->ex();
      Tbl->tbccount = (uns) (cp->siz() / sizeof(unl));
      if (Tbl->collist == NULL)
        Tbl->collist = new DCvlist;
      for (i = 1; i <= Tbl->tbccount; i++) {
        if ((Col = (col *) ((Tbl->collist)->find(i))) == NULL) {
          (Tbl->collist)->add((Col = new col), i);
					Col->brdrl = Tbl->brdrcl;
					Col->brdrr = Tbl->brdrcr;
				}
				if (Tbl->tblshtyp == 2) {  // col shading alternates
					uns pos = (i - 1) % (Tbl->tblshper + Tbl->tblaltper);
					if ((pos + 1) > Tbl->tblshper) {
						//Col->colfill = Tbl->tblaltfill;
						//Col->colcolr = Tbl->tblaltcolr;
						Col->colshd = Tbl->tblaltshd;
					}
					else {
						//Col->colfill = Tbl->tblbdyfill;
						//Col->colcolr = Tbl->tblbdycolr;
						Col->colshd = Tbl->tblbdyshd;
					}
				}
				if (*lptr != 1)  // placeholder, means use default
					Col->colwid = AdjColWid(*lptr);
				if (!Col->colwid)
					Col->colwid = (uns) DefColWidth;  // default to one inch
        lptr++;
      }
      if (TbCol)
        Col = (col *) (Tbl->collist)->find(TbCol);
      else RowNum = BodyRowNum = 0;
      break;
    case tbcfill:
      assert(Col);
      //GetFill(cp, &Col->colfill);
			GetShading(cp, &Col->colshd);
      break;
    case tbccolr:
      assert(Col);
      //GetColor(cp->siz(), &Col->colcolr);
			//printf("Col->colshd is at %0.4lx containing %0.4lx.\n",
			//                         &Col->colshd, Col->colshd);
			GetShadeColor(cp, &Col->colshd);
      break;
    case tbcrull:
      GetRule(cp, TbCol ? &Col->brdrl : &Tbl->brdrcl);
      break;
    case tbcrulr:
      GetRule(cp, TbCol ? &Col->brdrr : &Tbl->brdrcr);
      break;

		//case conduse:
		case condruse:
			ProcTbCondUse(cp);
			break;

#if 0
		case condruse:
			// handled during scan
			break;
#endif

    case tbrow:
      ProcRow(cp);
      LastRpos = 0;
			ShowTbRow = false;
			HideTbRow = false;
			if (!InTblDef)
				RowCpID = (long) cp;
      break;
    case tbrend:
			if ((ShowTbRow == false)
			 && (HideTbRow == true))
				Row->rtype = 0;
      ProcRowEnd();
			if (!InTblDef)
				RowCpID = 0;
      break;
    case tbrhigh:
      Row->rhigh = GetTwips(cp->siz(), pg_unit);
      break;
    case tbrhfix:
      Row->rhfix = GetTwips(cp->siz(), pg_unit);
      break;
    case tbrhmin:
      Row->rhmin = GetTwips(cp->siz(), pg_unit);
      break;
    case tbrhmax:
      Row->rhmax = GetTwips(cp->siz(), pg_unit);
      break;
    case tbrcolr:
      assert(Row);
      //GetColor(cp->siz(), &Row->rowcolr);
			//printf("Row->rowshd is at %0.4lx containing %0.4lx.\n",
			//                         &Row->rowshd, Row->rowshd);
			GetShadeColor(cp, &Row->rowshd);
			if (Row->rtype == 1)
				//Tbl->tblbdycolr = Row->rowcolr;
				Tbl->tblbdyshd = Row->rowshd;
      break;
    case tbrfill:
      assert(Row);
      //GetFill(cp, &Row->rowfill);
			GetShading(cp, &Row->rowshd);
			if (Row->rtype == 1)
				//Tbl->tblbdyfill = Row->rowfill;
				Tbl->tblbdyshd = Row->rowshd;
      break;
    case tbrrult:
      assert(Row);
      GetRule(cp, &Row->brdrt);
      break;
    case tbrrulb:
      assert(Row);
      GetRule(cp, &Row->brdrb);
      break;
    case tbrruls:
      assert(Row);
      GetRule(cp, &Row->brdrx);
      break;

    case tbcell:
      ProcCell(cp);
      break;
    case tbclend:
      ProcCellEnd();
      break;
    case tbclstrd:
      ProcCellStrad(cp);
      break;
    case tbclrstr:
      ProcCellRowStrad(cp);
      break;
    case tbclrotr:
      assert(Cell);
      Cell->cellrotr = (uns) (cp->siz() / 100);
      break;
    case tbclcolr:
      assert(Cell);
      //GetColor(cp->siz(), &Cell->cellcolr);
			//printf("Cell->cellshd is at %0.4lx containing %0.4lx.\n",
			//                         &Cell->cellshd, Cell->cellshd);
			GetShadeColor(cp, &Cell->cellshd);
      break;
    case tbclfill:
      assert(Cell);
      //GetFill(cp, &Cell->cellfill);
			GetShading(cp, &Cell->cellshd);
      break;
    case tbclrull:
      assert(Cell);
      GetRule(cp, &Cell->brdrl);
			Cell->cellrlov = true;
      break;
    case tbclrulr:
      assert(Cell);
      GetRule(cp, &Cell->brdrr);
			Cell->cellrrov = true;
      break;
    case tbclrult:
      assert(Cell);
      GetRule(cp, &Cell->brdrt);
			Cell->cellrtov = true;
      break;
    case tbclrulb:
      assert(Cell);
      GetRule(cp, &Cell->brdrb);
			Cell->cellrbov = true;
      break;

    case tbclmarl:
      if (InTbCell)
        Cell->cellmargl = GetTwips(cp->siz(), ld_unit);
      else if ((TbCol == 0) && !InTbTitle)
				Tbl->tblmargl = GetTwips(cp->siz(), ld_unit);
      break;
    case tbclmarr:
      if (InTbCell)
        Cell->cellmargr = GetTwips(cp->siz(), ld_unit);
      else if ((TbCol == 0) && !InTbTitle)
				Tbl->tblmargr = GetTwips(cp->siz(), ld_unit);
      break;
    case tbclmart:
      if (InTbCell)
        Cell->cellmargt = GetTwips(cp->siz(), ld_unit);
      else if ((TbCol == 0) && !InTbTitle)
				Tbl->tblmargt = GetTwips(cp->siz(), ld_unit);
      break;
    case tbclmarb:
      if (InTbCell)
        Cell->cellmargb = GetTwips(cp->siz(), ld_unit);
      else if ((TbCol == 0) && !InTbTitle)
				Tbl->tblmargb = GetTwips(cp->siz(), ld_unit);
      break;
    case tbclmrtl:
      if (InTbCell)
        Cell->cellmartl = (unc) cp->siz();
				// 0 = replace defaults, 1 = add to defaults
      break;
    case tbclmrtr:
      if (InTbCell)
        Cell->cellmartr = (unc) cp->siz();
      break;
    case tbclmrtt:
      if (InTbCell)
        Cell->cellmartt = (unc) cp->siz();
      break;
    case tbclmrtb:
      if (InTbCell)
        Cell->cellmartb = (unc) cp->siz();
      break;

    case tbclvalg:
      if (InTbCell)
        Cell->cellvalgn = (unc) cp->siz() + 1;
      break;

		case putfr:
    case puttx:
		case putvar:
		case putanum:
			if (InTDefaults
			 || InTblTitleRow)
				break;
      if (InTbTitle) {
        //if (Tbl->titlegap) { // insert space above or below for gap
        //  ncp = new RTFctl(ilong, 2, 22, (Tbl->titleplace == 1) ? 3 : 1,
        //                   Tbl->titlegap);  // raw size, convert later
        //  ncp->rp = new rtf;
        //  (ncp->rp)->write_op = putpspc;
        //  Tbl->titlegap = 0;
        //  (Tbl->titlecont)->add(*ncp);
        //}
        (Tbl->titlecont)->add(*cp);
				TblTitleTextAdded = true;
        break;
      }
			if (Tbl && Row && Cell) {
				StartCellText(cp);
				AddCellCont(cp);
			}
      break;

    case putftn:
    case endftn:
			if (InTDefaults)
				break;
      if (InTbTitle)
        (Tbl->titlecont)->add(*cp);
      else if (InTbCell)
        AddCellCont(cp);
      break;

#if 0
		case parstart:
      if (InTbTitle) {
				TblTitleParaStarted = true;
        (Tbl->titlecont)->add(*cp);
        break;
      }
      else if (InTbCell)	{
        SetCurrProps(cp);
				AddCellCont(cp);
			}
      break;

		case parend:
      if (InTbTitle) {
				TblTitleParaEnded = true;
        (Tbl->titlecont)->add(*cp);
        break;
      }
      else if (InTbCell)	{
        SetCurrProps(cp);
				AddCellCont(cp);
			}
      break;
#endif

    default:
			if (InTDefaults
			 || InTblTitleRow)
				break;
      if (InTbTitle) {
#if 0
				if (TblTitleParaStarted == false)	{
          ncp = new RTFctl(imnem, 2, 11, 1);
          ncp->rp = new rtf;
          (ncp->rp)->write_op = parstart;
	        (Tbl->titlecont)->add(*ncp);
					TblTitleParaStarted = true;
				}
#endif
        (Tbl->titlecont)->add(*cp);
        break;
      }
      else if (InTbCell)	{
        SetCurrProps(cp);
				AddCellCont(cp);
			}
      break;
  }
}



// table format processing


DCvlist RTFwr::TblFormList;
DCnlist RTFwr::TblFormNameList;
DCvlist RTFwr::RowFormList;
DCnlist RTFwr::RowFormNameList;
DCvlist RTFwr::CellFormList;
DCnlist RTFwr::CellFormNameList;
long RTFwr::RowFormNum = 0;
long RTFwr::CellFormNum = 0;


tbl *RTFwr::TblForm = NULL;;
bool RTFwr::InTblDef = false;
bool RTFwr::TbColWidPercent = false;

col *RTFwr::ColForm = NULL;
uns RTFwr::TbCol = 0;
long RTFwr::DefColWidth = 1440;


void
RTFwr::ProcTblForm(void)
{
  InTblDef = true;
  Tbl = new tbl;
  TblFormList.add(Tbl, DCLFormNum);
}


void
RTFwr::StoreTblForm(void)
{
  TblFormNameList.add(FormName, DCLFormNum);
	Tbl->tblfname = FormName;
	GetTblFormatFileInfo(FormName);  // override Tbl with format info
  InTblDef = false;
}


void
RTFwr::UseTblForm(RTFctl *cp)
{
  col *tcol, *ncol;
  row *trow, *nrow;
  cell *tcell, *ncell;
  uns i, j;

  TblForm = (tbl *) TblFormList.find(cp->siz());
	if (!TblForm || !Tbl)
		return;

  uns rows = Tbl->tbrcount;
	long hlp = Tbl->tblhlp;
	short aspc = Tbl->tblaspc;
	bool strip = Tbl->tblstrip;
  *Tbl = *TblForm;
	Tbl->tblstrip = strip;
  Tbl->tbrcount = rows;
	Tbl->tblhlp = hlp;
	if (aspc > Tbl->tblaspc)
		Tbl->tblaspc = aspc;

  Tbl->collist = new DCvlist;
  if (TblForm->collist) {
		for (i = 1; tcol = (col *) (TblForm->collist)->find(i); i++) {
			ncol = new col;
			*ncol = *tcol;
			(Tbl->collist)->add(ncol, i);
		}
	}

	Tbl->rowtlist = new DCvlist;
  if (TblForm->rowtlist) {
		for (i = 1; trow = (row *) (TblForm->rowtlist)->find(i); i++) {
			nrow = new row;
			*nrow = *trow;
			(Tbl->rowtlist)->add(nrow, i);
			nrow->celllist = new DCvlist;
			assert(trow->celllist);
			for (j = 1; tcell = (cell *) (trow->celllist)->find(j); j++) {
				ncell = new cell;
				*ncell = *tcell;
				(nrow->celllist)->add(ncell, j);
			}
		}
	}

  Tbl->rowlist = new DCvlist;
}



// table processing


DCvlist *RTFwr::TblList = NULL;
bool RTFwr::TblListStarted = false;
uns RTFwr::TblCount = 0;
tbl *RTFwr::Tbl;
tbl RTFwr::CTbl;
anyini RTFwr::TblIni;
bool RTFwr::InTbl;
bool RTFwr::InTbTitle;
bool RTFwr::TblTitleParaStarted;
bool RTFwr::TblTitleTextAdded;
bool RTFwr::TblTitleParaEnded;
bool RTFwr::InTDefaults;
bool RTFwr::StripTb = false;
DClist RTFwr::StripTableList;

col *RTFwr::Col;
uns RTFwr::ColNum;
uns RTFwr::LastColwid;

bool RTFwr::TitleInRow = false;
bool RTFwr::InTopTitleRow = false;
bool RTFwr::InTblTitleRow = false;
uns RTFwr::TitleRowAlign = 0;
short RTFwr::TitleRowLind = 0;
long RTFwr::TitleRowEnd = 0;



void
RTFwr::ProcTbl(RTFwfile *wf, RTFctl *cp)
{
	if (InTblCell)
		return;

	if (!InHelpTitlePara)
		WriteText(wf);

#if 0
	if (ParaStarting && !InPara) {
		InPara = true;
		if (!InTbCell)
			ParaStarting = false;
		StartingPage = false;
	}
#endif
  SaveState();
  Tbl = new tbl;
  InTbl = true;
  Tbl->tbrcount = cp->dat();
	Tbl->tblaspc = 0;
	Tbl->tblstrip = StripTableList.find((long) cp);

	if (cp->siz()) {  // ParaRefNum put in to table ctl
		if (HelpForm == true) 
			Tbl->tblhlp = HelpTitleParaList.find(cp->siz());
		else
			Tbl->tblaspc = (short) SideHSpaceList.find(cp->siz());
	}

	TblTitleParaStarted = false;
	TblTitleTextAdded = false;
	TblTitleParaEnded = false;

  Curr = DefForm;
	TblForm = NULL;
}


void
RTFwr::ProcTblCol(RTFctl *cp)
{
  TbCol = cp->dat();
  assert(Tbl);
  if (TbCol > Tbl->tbccount)
    Tbl->tbccount = TbCol;

  if (Tbl->collist == NULL)
    Tbl->collist = new DCvlist;
  if ((Col = (col *) (Tbl->collist)->find(TbCol)) == NULL) {
    Col = new col;
    (Tbl->collist)->add(Col, TbCol);
		Col->brdrl = Tbl->brdrcl;
		Col->brdrr = Tbl->brdrcr;
  }
	else {
		if (!Col->brdrl)
			Col->brdrl = Tbl->brdrcl;
		if (!Col->brdrr)
			Col->brdrr = Tbl->brdrcr;
	}
	if (Tbl->tblshtyp == 2) {  // col shading alternates
		uns pos = ((Tbl->tblshper + Tbl->tblaltper) > 0) ?
		         ((TbCol - 1) % (Tbl->tblshper + Tbl->tblaltper)) : 0 ;
		if ((pos + 1) > Tbl->tblshper) {
			//Col->colfill = Tbl->tblaltfill;
			//Col->colcolr = Tbl->tblaltcolr;
			Col->colshd = Tbl->tblaltshd;
		}
	}
  RowNum = BodyRowNum = 0;
}



short RTFwr::TblColWidPct = 100;
short RTFwr::TblColWidAdd = 0;
unl RTFwr::TblFullWidth = 9360;  // 6.5in
bool RTFwr::TblFullWidthWasSet = false;


uns
RTFwr::AdjColWid(unl cwid)
{
	uns twips = 0;

	if (TbColWidPercent)  // cwid is a percent of table width
		twips = (uns) ((TblFullWidth * cwid) / 100);
	else                  // cwid is an actual column width
		twips = GetTwips(cwid, pg_unit);

	if (((TblColWidPct == 100)
	  || (TblColWidPct == 0))
	 && (TblColWidAdd == 0))
		return twips;

	return (uns) (((((long) TblColWidPct) * (long) twips) / 100L)
                + TblColWidAdd);
}


void
RTFwr::WriteTbl(RTFwfile *wf)
{

	if (InTblCell)
		return;

#ifdef OLD_TABLE
  if (InPara) {
    InPara = false;
    wf->putCW("par");
    wf->putEnd();
    if (PGStarted) {
      wf->putGE();
      PGStarted--;
      wf->putEnd();
    }
    LinePos = Curr.find;
	}
#else
  if ((InPara || ParaStarting) && !TblListStarted) {
	  InTbl = false;
		//? TabsChanged = true;
		if (CharStylesUsed) {
			EndCharForm = false;
			StartCharForm = false;
		}
	  RestoreState(FullState);
		HyperStart = HyperEnd = false;
		// store Tbl in TblList
		if (!TblList) {
			TblList = new DCvlist;
			TblCount = 0;
		}
		TblList->add(Tbl, ++TblCount);
		Tbl = NULL;
		return;
	}
	if (!Tbl)  // error
		return;
	InTbl = true;
#endif

	if ((PHtype = Tbl->tblhlp) != 0) {
		Tbl->titleplace = 1;  // force title first
		SetParaHelp(wf);
	}

	bool tblGS = false;
	StripTb = Tbl->tblstrip;
	unc tblsideh = Curr.sideh;

  if (InHelpTitlePara
	 && !HelpSectBreaks)
		StripTb = true;
	else if (SideHAnchor == true)
		SideHAnchor = false;
	else if (!InHelpTitlePara
	 && !StripTb){
		wf->putGS();
		tblGS = true;
		if (Tbl->tblbspc)
			PutTbSpacer(wf, Tbl->tblbspc);
	}

	//if (Tbl->tbllind < Curr.lind)	 // is this really true? ****
	//	Tbl->tbllind += Curr.lind;

	// if table indent plus width is > 360 twips past right margin,
	// and TextLind (or temp lmarg) is in effect, deduct TextLind
	// to match Frame's interpretation of indent (wrt lmarg)
	long tabwide = 0;
	col *tcol = NULL;
	for (uns i = 0; i < Tbl->tbccount; i++) {
		if (tcol = (col *) (Tbl->collist)->find(i + 1))
			tabwide += tcol->colwid;
	}

	if ((SideHUsed == true)
	 && (SideHPos == 1)) {  // sideheads on the left
		short lside = SideHWide + SideHGap;

		if ((SideHType == 'I')
		 && (Tbl->tbllind < TextLind))
			Tbl->tbllind += TextLind;

		if (SideHType == 'L') {
			if (Tbl->tbllind >= lside)
				Tbl->tbllind -= lside;
			else Tbl->tbllind = 0;
		}

		if ((TextLMarg + Tbl->tbllind + tabwide)  // right edge of table
				> ((PaperWide - RMarg) + 360)) {   // right margin + .25"
			if (SideHType == 'I')
				Tbl->tbllind -= TextLind;  // reposition from LMarg
			else if (SideHType == 'F')
				Tbl->tbllind -= lside;  // negative is OK
		}		
	}
	else if (ShiftWideTablesLeft) {   // sideheads not involved
		if ((Tbl->tbllind + tabwide) > (BodyWide + 360)) {
			if ((Tbl->tbllind -= Curr.lind) < 0)
				Tbl->tbllind = 0;
		}			
	}

	if (TitleInRow) {
		if (Tbl->tblalgn == 1) { // centered
			TitleRowAlign = 1;
			TitleRowLind = Tbl->tbllind;
			TitleRowEnd = BodyWide;
		}
		else if (Tbl->tblalgn == 2) { // right
			TitleRowAlign = 2;
			TitleRowLind = 0;
			TitleRowEnd = tabwide;
		}
		else { // left
			TitleRowAlign = 0;
			TitleRowLind = 0;
			TitleRowEnd = (Tbl->tbllind * 2) + tabwide;
		}
	}

  if (Tbl->titleplace == 1) {
	 //&& TblTitleTextAdded) {
		if (TitleInRow)
			StartTitleRow(wf, true);
    WriteTbTitle(wf);
		InTbl = false;
		InTbTitle = true;
		if (Tbl->titlegap)
			PutTbTitleSpacer(wf, Tbl->titlegap);
		if (TitleInRow)
			EndTitleRow(wf);
		InTbTitle = false;
	}

  if ((tblGS == false)
	 && !InHelpTitlePara) {
		wf->putGS();
		tblGS = true;
	}

	PHtype = 0;
	TbFootNum = 0;

  Row = NULL;
  NextRow = (row *) ((Tbl->rowlist)->find(1));
	short RowMarg = NextRow->cmargx;

	CellRowStradCount = new uns[Tbl->tbccount];
	CellRowColStrad = new uns[Tbl->tbccount];
	CellRowStradCell = (cell **) new char[sizeof(cell *) * Tbl->tbccount];

  for (RowNum = 1; RowNum <= Tbl->tbrcount; RowNum++) {
    LastRow = Row;
    if ((Row = NextRow) == NULL)
      break;
    NextRow = (row *) ((Tbl->rowlist)->find(RowNum + 1));
		Row->cmargx = RowMarg;  // make all match the first
    WriteTbRow(wf);
  }

	delete [] CellRowStradCount;
	delete [] CellRowColStrad;
	delete [] CellRowStradCell;
	CellRowStradCount = NULL;
	CellRowColStrad = NULL;
	CellRowStradCell = NULL;

  if (Tbl->titleplace == 2) {
	 // && TblTitleTextAdded) {
		if (TitleInRow)
			StartTitleRow(wf, false);
		InTbTitle = true;
		if (Tbl->titlegap)
			PutTbTitleSpacer(wf, Tbl->titlegap);
		InTbTitle = false;
    WriteTbTitle(wf);
		InTbl = false;
		if (TitleInRow)
			EndTitleRow(wf);
	}

  if ((!InHelpTitlePara) 
	 && Tbl->tblaspc) {
		Curr.sideh = tblsideh;
		PutTbSpacer(wf, Tbl->tblaspc);
	}
	
	if (tblGS)
	  wf->putGE();

  wf->putEnd();
  // delete Tbl;
	Tbl = NULL;
  InTbl = false;

#ifdef OLD_TABLE
  InPara = false;
  TabsChanged = true;
  RestoreState();
#endif
}


void
RTFwr::PutTbSpacer(RTFwfile *wf, short spc)
{
	wf->putEnd();
	SetPardProps(wf);
	WriteParaFrameProps(wf);
	wf->putCWN("sl", -spc, true);
	wf->putCS('~');
	wf->putCW("par");
	wf->putEnd();
}


void
RTFwr::PutTbTitleSpacer(RTFwfile *wf, short spc)
{
	if (!Tbl->titlecont)
		return;
	if (InTblTitleRow) {
		InTbl = false;
		InTbTitle = true;
		InTbCell = true;
	}
	if (InTopTitleRow)
		wf->putCW("par");
	wf->putEnd();
	SetPardProps(wf);
	wf->putCWN("sl", -spc, true);
	wf->putCS('~');
	if (!InTopTitleRow)
		wf->putCW("par");
	wf->putEnd();
}


void
RTFwr::WriteTbTitle(RTFwfile *wf)
{
	if (!Tbl->titlecont)
		return;
	InTbl = false;
	InTbTitle = true;
	Curr.algn = (unc) Tbl->tblalgn;
	Curr.find += (Tbl->tbllind - Curr.lind);
	Curr.lind = Tbl->tbllind;
	long lasttell = wf->tell();
	(Tbl->titlecont)->write(*wf);
	InTbTitle = false;
	InTbl = true;
	if (InHelpTitlePara   // only use the title, if really present
	 && (wf->tell() > (lasttell + 3))) {
		InHelpTitlePara = false;
		HelpTitlePending = false;
		StripTb = Tbl->tblstrip;
	}
}


void
RTFwr::StartTitleRow(RTFwfile *wf, bool top)
{
	InTblTitleRow = true;
	wf->putCW("trowd");
	if (TitleRowAlign)
		wf->putCW(RowAlgns[TitleRowAlign]);
	if (top)
		wf->putCW("trhdr");
	wf->putCWN("trleft", TitleRowLind, true);
  wf->putCWN("cellx", (short) TitleRowEnd);
	wf->putEnd();
	if (top)
		InTopTitleRow = true;
	InTbCell = true;
	InTblCell = true;
}


void
RTFwr::EndTitleRow(RTFwfile *wf)
{
	wf->putCW("cell");
	wf->putCW("intbl");
	wf->putCW("row");
	wf->putEnd();
	InTopTitleRow = false;
	InTbCell = false;
	InTblCell = false;
	InTblTitleRow = false;
	InTbl = true;
	ParaEnding = false;
}



// row processing and writing


row *RTFwr::Row;
row *RTFwr::RowForm;
row *RTFwr::LastRow;
row *RTFwr::NextRow;
uns RTFwr::TbRowType;
uns RTFwr::RowNum;
uns RTFwr::BodyRowNum;
long RTFwr::RowCpID = 0;
char *RTFwr::RowAlgns[RowAlgnMax] = { "trql", "trqc", "trqr" };


void
RTFwr::ProcRow(RTFctl *cp)
{
  row *trow;
  DCvlist *trowlist;

  TbRowType = cp->dat();
  ColNum = 0;
  LastColwid = 0;
	if (++RowNum > Tbl->tbrcount)
		Tbl->tbrcount = RowNum;

	if (TablesFile
	 && TblForm
	 && TblForm->rowlist
	 && TblForm->rowlist->next) {
		trowlist = TblForm->rowlist; // contains template rows
		if (TbRowType == 2) {  // header
			if (!Tbl->hrcount)
				TbRowType = 1;
			else {
				Tbl->tbhcount++;
				if (Tbl->tbhcount <= Tbl->hrcount)
					trow = (row *) trowlist->find(Tbl->tbhcount);
				else
					trow = (row *) trowlist->find(Tbl->hrcount);
			}
		}
		if (TbRowType == 3) {  // footer
			if (!Tbl->frcount)
				TbRowType = 1;
			else {
				Tbl->tbfcount++;
				if (Tbl->tbfcount <= Tbl->frcount)
					trow = (row *) trowlist->find(Tbl->hrcount + Tbl->brcount
															+ Tbl->tbfcount);
				else
					trow = (row *) trowlist->find(Tbl->hrcount + Tbl->brcount
															+ Tbl->frcount);
			}
		}
		if (TbRowType == 1) {  // body
			Tbl->tbbcount++;
			if (Tbl->tbbcount <= Tbl->brcount)
				trow = (row *) trowlist->find(Tbl->hrcount + Tbl->tbbcount);
			else if (Tbl->brcount == 1)
				trow = (row *) trowlist->find(Tbl->hrcount + 1);
			else
				trow = (row *) trowlist->find(Tbl->hrcount +
				                     ((Tbl->tbbcount - 1) % Tbl->brcount) + 1);
		}
    RowForm = trow;
    Row = new row;
    *Row = *trow;
		Row->rtype = TbRowType;
		Row->celllist = new DCvlist;
		Tbl->rowlist->add(Row, RowNum);
	}
	else {  // Frame, or no table format used
		if ((trowlist = Tbl->rowtlist) == NULL)
			Tbl->rowtlist = trowlist = new DCvlist;
		if ((trow = (row *) trowlist->find(TbRowType)) == NULL) {
			trow = Row = new row;
			Row->rtype = TbRowType;
			Row->cmargx = CellMargMax;
			Row->celllist = new DCvlist;
			trowlist->add(Row, TbRowType);
		}
		else Row = trow;

		if (TbCol) {
			if ((Cell = (cell *) (Row->celllist)->find(TbCol)) == NULL) {
				Cell = new cell;
				(Row->celllist)->add(Cell, TbCol);
			}
		}

		if (!InTblDef && !InTDefaults) {
			RowForm = trow;
			Row = new row;
			*Row = *trow;
			if (TbRowType == 1) {  // body row
				++BodyRowNum;
				if ((Tbl->tblrrper > 0)
				 && ((BodyRowNum % Tbl->tblrrper) == 0))
					Row->brdrb = Tbl->brdra;
				if ((Tbl->tblshtyp == 1)   // row shading alternates
				 && (Tbl->tblaltper > 0)) {
					uns pos = ((Tbl->tblshper + Tbl->tblaltper) > 0) ?
									 ((BodyRowNum - 1) % (Tbl->tblshper + Tbl->tblaltper)) : 0;
					if ((pos + 1) > Tbl->tblshper) {
						//Row->rowfill = Tbl->tblaltfill;
						//Row->rowcolr = Tbl->tblaltcolr;
						Row->rowshd = Tbl->tblaltshd;
					}
				}
			}
			Row->celllist = new DCvlist;
			if (Tbl->rowlist == NULL)
				Tbl->rowlist = new DCvlist;
			Tbl->rowlist->add(Row, RowNum);
		}
	}
	Row->rowref = (long) cp;
}


void
RTFwr::ProcRowEnd(void)
{
  TbRowType = 0;
	Row = NULL;
}


void
RTFwr::WriteTbRow(RTFwfile *wf)
{
	if (Row->rtype == 0) {  // hidden
		Row = LastRow;  // so separators work
		return;
	}

	if (Tbl->tblstrip) {
		if (HelpForm) {
			wf->putCW("page");
			if (HelpSectBreaks)
				wf->putCW("sect");
		}
	  wf->putEnd();
	}

	if (!StripTb) {
		wf->putCW("trowd");
		if ((OutType != 'H')
		 && (Tbl->tblalgn)
		 && (Tbl->tblalgn < RowAlgnMax))
			wf->putCW(RowAlgns[Tbl->tblalgn]);
		else if ((OutType == 'H')
		 && (!TbFixed)
		 && (Tbl->tblalgn == 1))  // centered
			wf->putCW(RowAlgns[1]); // means variable width
		if (Row->cmargx == CellMargMax)
			Row->cmargx = 20;
		if ((Row->rtype == 2)
		 || (Row->rtype == 4))
			wf->putCW("trhdr");
		//wf->putCWN("trgaph", Row->cmargx);
		if (Tbl->tbllind)
			wf->putCWN("trleft", Tbl->tbllind, true);
		if (OutType != 'H') {
			if (Row->rhmin)
				wf->putCWN("trrh", (Row->rhmin == Row->rhmax) ? -(Row->rhmin) : Row->rhmin, true);
			else if (Row->rhigh)
				wf->putCWN("trrh", Row->rhigh, true);
		}
		wf->putEnd();
		if (TbRuleType == 'C') {
			for (ColNum = 1; ColNum <= Tbl->tbccount; ColNum++) {
				if ((Cell = (cell *) ((Row->celllist)->find(ColNum))) == NULL)
					break;
				SetCellBorder();
			}
		}
		for (ColNum = 1; ColNum <= Tbl->tbccount; ColNum++) {
			if ((Cell = (cell *) ((Row->celllist)->find(ColNum))) == NULL)
				break;
			WriteCellProps(wf);
		}
	  InTbCell = true;
	}

	InTblCell = true;

  for (ColNum = 1; ColNum <= Tbl->tbccount; ColNum++) {
    if ((Cell = (cell *) ((Row->celllist)->find(ColNum))) == NULL)
      break;
    WriteCell(wf);
  }
  InParaGroup = false;
	InTblCell = false;

	if (!StripTb) {
		InTbCell = false;
		SetPardProps(wf);
		wf->putCW("intbl");
		wf->putCW("row");
		wf->putEnd();
	}
}



// cell processing and writing


cell *RTFwr::Cell;
bool RTFwr::InTbCell = false;
bool RTFwr::InTblCell = false;
bool RTFwr::StartingTbCell = false;
bool RTFwr::EndingTbCell = false;
char *RTFwr::CellFillPats[CellFillMax] = {
  "clbghoriz", "clbgvert", "clbgfdiag", "clbgbdiag",
  "clbgcross", "clbgdcross",
  "clbgdkhor", "clbgdkvert", "clbgdkfdiag", "clbgdkbdiag",
  "clbgdkcross", "clbgdkdcross"
};
char *RTFwr::CellBordStyles[] = {
  "brdrs", "brdrdb", "brdrth"
};
uns RTFwr::CellStradCount = 0;
uns *RTFwr::CellRowStradCount = NULL;
uns *RTFwr::CellRowColStrad = NULL;
cell **RTFwr::CellRowStradCell = NULL;
bool RTFwr::InStradTbCell = false;
bool RTFwr::FirstStradTbCell = false;
bool RTFwr::LastStradTbCell = false;
bool RTFwr::MergeStradCells = false;
bool RTFwr::RowStrad = false;
bool RTFwr::CellStradAdjust = false;
short RTFwr::StradRpos;
short RTFwr::LastRpos;
char RTFwr::TableFill = 'A';
bool RTFwr::TblFillColor = true;
bool RTFwr::TblFillShade = true;


void
RTFwr::SetCellMarg(RTFctl *cp)
{
  if (cp->rp == NULL)
    return;

  switch ((cp->rp)->write_op) {
    case tbclmarl:
      Curr.cellmargl = GetTwips(cp->siz(), ld_unit);
      break;
    case tbclmarr:
      Curr.cellmargr = GetTwips(cp->siz(), ld_unit);
      break;
    case tbclmart:
      Curr.cellmargt = GetTwips(cp->siz(), ld_unit);
      break;
    case tbclmarb:
      Curr.cellmargb = GetTwips(cp->siz(), ld_unit);
      break;

    case tbclmrtl:
      Curr.cellmartl = (unc) cp->siz();
      break;
    case tbclmrtr:
      Curr.cellmartr = (unc) cp->siz();
      break;
    case tbclmrtt:
      Curr.cellmartt = (unc) cp->siz();
      break;
    case tbclmrtb:
      Curr.cellmartb = (unc) cp->siz();
      break;

    case tbclvalg:
      Curr.cellvalgn = (unc) cp->siz() + 1;
      break;

    default:
      break;
  }
}


void
RTFwr::ProcCell(RTFctl *cp)
{
  InTbCell = true;
	StartingTbCell = true;
	TbCellParaStartCtl = NULL;
  Cell = new cell;
  assert(Row);
  Row->celllist->add(Cell, ++ColNum);

	cell *tc = NULL;
	DCvlist *tclist = NULL;

	if (TablesFile
	 && RowForm
	 && RowForm->celllist
	 && RowForm->celllist->next) {
		tclist = RowForm->celllist;
		if (ColNum <= Row->hccount) { // row header cell
			Row->rhcount++;
			tc = (cell *) tclist->find(Row->rhcount);
		}
		else {
			Row->rbcount++;
			if (Row->rbcount <= Row->bccount)
				tc = (cell *) tclist->find(Row->hccount + Row->rbcount);
			else if (Row->bccount == 1)
				tc = (cell *) tclist->find(Row->hccount + 1);
			else
				tc = (cell *) tclist->find(Row->hccount + 
				                     ((Row->rbcount - 1) % Row->bccount) + 1);
		}
    //CellForm = tc;
    *Cell = *tc;
	}
	if (ColNum <= Tbl->tbccount) {
		Col = (col *) Tbl->collist->find(ColNum);
		assert(Col);
		LastColwid = Col->colwid;
		LastRpos += LastColwid;
	}
  if (CellStradCount) {
    Cell->straddled = true;
    CellStradCount--;
		if (!CellStradCount)
			LastStradTbCell = Cell->laststrad = true;
    Cell->rpos = CellStradAdjust ? StradRpos : LastRpos;
  }
  else Cell->rpos = LastRpos;
}


void
RTFwr::ProcCellEnd(void)
{
	RestoreInheritForm(); // inherit restore at end of cell
  InTbCell = false;
	StartingTbCell = false;
  ParaEnding = false;
	FirstStradTbCell = LastStradTbCell = false;

  if (!Cell->brdrl) {
    if (Col)
      Cell->brdrl = Col->brdrl;
  }
  if (!Cell->brdrr) {
    if (Col)
      Cell->brdrr = Col->brdrr;
  }
  if (!Cell->brdrt) {
		if (Row)
			Cell->brdrt = Row->brdrt;
  }
  if (!Cell->brdrb) {
		if (Row)
			Cell->brdrb = Row->brdrb;
	}
  //if (Cell->cellfill == 0) {
	if (!Cell->cellshd) {
		if ((Row->rtype != 1)		 // header or footer row
		 || (Tbl->tblshtyp == 1)) {  // body row shading alternates
	    //if (Row && Row->rowfill)
		  //  Cell->cellfill = Row->rowfill;
	    if (Row && Row->rowshd)
		    Cell->cellshd = Row->rowshd;
		}
    else if ((Row->rtype == 1)
		 && (Tbl->tblshtyp == 2)) {
			//if (Col && Col->colfill)
			//	Cell->cellfill = Col->colfill;
			if (Col && Col->colshd)
				Cell->cellshd = Col->colshd;
		}
  }
  //if ((Cell->cellfill == CellFillEmpty)
  // || (Cell->cellfill == CellFillWhite))
  //  Cell->cellfill = 0;

	if (Cell->cellshd) {
	  if (((Cell->cellshd->tint == CellFillEmpty)
		  || (Cell->cellshd->tint == CellFillWhite))
		 && (Cell->cellshd->type == shdnone))
		  Cell->cellshd = NULL;
	}

#if 0
  if (Cell->cellcolr == 0) {
		if ((Row->rtype != 1)		 // header or footer row
		 || (Tbl->tblshtyp == 1)) {  // body row shading alternates
	    if (Row && Row->rowcolr)
		    Cell->cellcolr = Row->rowcolr;
		}
    else if ((Row->rtype == 1)
		 && (Tbl->tblshtyp == 2)) {
			if (Col && Col->colcolr)
				Cell->cellcolr = Col->colcolr;
		}
  }
#endif
  //if (Cell->cellcolr == Invisible)
  //  Cell->cellcolr = 0;
  if (Cell->cellshd
	 && (Cell->cellshd->color == Invisible))
    Cell->cellshd->color = 0;
}


void
RTFwr::ProcCellStrad(RTFctl *cp)
{
  col *tcol;

  CellStradCount = (uns) cp->siz();
  if (--CellStradCount) {
    Cell->straddling = true;
		FirstStradTbCell = true;
	}
  else return;
  StradRpos = LastRpos;  // already includes curr colwid
	if (CellStradAdjust) {
		for (uns i = ColNum; i < ColNum + CellStradCount; i++) {
			if (tcol = (col *) (Tbl->collist)->find(i + 1))
				LastColwid = tcol->colwid;
			StradRpos += LastColwid;
		}
	}
  Cell->rpos = StradRpos;
}


void
RTFwr::ProcCellRowStrad(RTFctl *cp)
{
	if (Word8)
	  Cell->vstrad = (uns) cp->siz();
}


void
RTFwr::StartCellText(RTFctl *cp)
{
	if (StartingTbCell == false)
		return;

  Cell->algn = Curr.algn;
	if (Curr.cellvalgn)
	  Cell->cellvalgn = Curr.cellvalgn;
  Cell->cellmartl = Curr.cellmartl;
	if (Curr.cellmargl > Cell->cellmargl)
	  Cell->cellmargl = Curr.cellmargl;
  if (Cell->cellmartl)
    Cell->cellmargl += Tbl->tblmargl;
  if (Cell->cellmargl < Row->cmargx)
    Row->cmargx = Cell->cellmargl;

  Cell->cellmartr = Curr.cellmartr;
	if (Curr.cellmargr > Cell->cellmargr)
	  Cell->cellmargr = Curr.cellmargr;
  if (Cell->cellmartr)
    Cell->cellmargr += Tbl->tblmargr;
  if (Cell->cellmargr < Row->cmargx)
    Row->cmargx = Cell->cellmargr;

  Cell->cellmartt = Curr.cellmartt;
	if (Curr.cellmargt > Cell->cellmargt)
	  Cell->cellmargt = Curr.cellmargt;
  if (Cell->cellmartt)
    Cell->cellmargt += Tbl->tblmargt;

  Cell->cellmartb = Curr.cellmartb;
	if (Curr.cellmargb > Cell->cellmargb)
	  Cell->cellmargb = Curr.cellmargb;
  if (Cell->cellmartb)
    Cell->cellmargb += Tbl->tblmargb;

	StartingTbCell = false;
}


void
RTFwr::AddCellCont(RTFctl *cp)
{
  if (Cell) {
    if (!Cell->cont)
      Cell->cont = new RTFgroup;
    (Cell->cont)->add(*cp);
  }
}


void
RTFwr::WriteCellProps(RTFwfile *wf)
{
	cell *sc = NULL;

	if (Cell->vstrad) {  // start of vert straddle
		CellRowStradCell[ColNum - 1] = Cell;
		CellRowStradCount[ColNum - 1] = Cell->vstrad - 1;
		Cell->vstrad = 0;  // row straddling not straddled
    if (Cell->straddling) { // col strad too
			RowStrad = true;
			CellRowColStrad[ColNum - 1] = 2;
		}
		if (HelpForm)
			Cell->brdrb = NULL;
		else
	    wf->putCW("clvmgf");
	}
	else if (CellRowStradCount[ColNum - 1]) {
		Cell->vstrad = 1; // row straddled
		CellRowStradCount[ColNum - 1]--;
		if (HelpForm) {
			if ((sc = CellRowStradCell[ColNum - 1]) != NULL) {
				Cell->brdrl = sc->brdrl;
				Cell->brdrr = sc->brdrr;
			}
			Cell->brdrt = NULL;
			if (CellRowStradCount[ColNum - 1] != 0)
				Cell->brdrb = NULL;
		}
		else
	    wf->putCW("clvmrg");
		// duplicate orig row col strad features
		Cell->straddled = Cell->straddling = false;
		if (CellRowColStrad[ColNum - 1] == 1)
			Cell->straddled = true;
		else if (CellRowColStrad[ColNum - 1] == 2)
			Cell->straddling = true;
		if (CellRowStradCount[ColNum - 1] == 0) {  // end of row strad
			CellRowStradCell[ColNum - 1] = NULL;
			CellRowColStrad[ColNum - 1] = 0;
		}
		RowStrad = false;
	}
	else if (Cell->straddled
	 && RowStrad) { // row straddling, col straddled
		if (HelpForm)
			Cell->brdrb = NULL;
		else
			wf->putCW("clvmgf");
		CellRowStradCell[ColNum - 1] = Cell;
		CellRowStradCount[ColNum - 1] = CellRowStradCount[ColNum - 2];
		CellRowColStrad[ColNum - 1] = 1;
	}
	else {  // no row strad
		RowStrad = false;
		CellRowColStrad[ColNum - 1] = 0;
	}

	if (Cell->straddled) {
		if (!MergeStradCells)
			wf->putCW("clmrg");
	}
  else {
    if (Cell->straddling
		 && !MergeStradCells)
      wf->putCW("clmgf");

    if (Word8 && Cell->cellvalgn)
      wf->putCW((Cell->cellvalgn == 1) ? "clvertalc" :
			         ((Cell->cellvalgn == 2) ? "clvertalt" : "clvertalb"));

		//WriteCellShading(wf);
		WriteCellShadingFmt(wf);

		if (Cell->cellrotr) {
#if 0
		  if (Cell->cellrotr == 90)
				wf->putCW("cltxlrtbv");
		  else if (Cell->cellrotr == 270)
				wf->putCW("cltxtbrlv");
#endif
		  if (Cell->cellrotr == 90)
				wf->putCW("cltxtbrlv");
		  else if (Cell->cellrotr == 270)
				wf->putCW("cltxbtlr");
		}
	}

	if (TbRuleType == 'C')
		WriteCellBorder(wf);

	if (MergeStradCells) {
	  if (Cell->straddling)
			return;
	  if (Cell->straddled
		 && !Cell->laststrad)
			return;
	}

  wf->putCWN("cellx", Cell->rpos + Tbl->tbllind, true);
	//wf->putCWN("cellx", (Tbl->tbllind > 0) ? Cell->rpos + Tbl->tbllind : Cell->rpos);
  wf->putEnd();
}


void
RTFwr::SetCellBorder(void)
{
	static brdrinfo stradrt;
	static bool stradrov = false;
	static cell *stradcell = NULL;

  if (!Cell->straddled
	 || Cell->vstrad)	{

		// table ruling overrides cell ruling for left of first col
		if ((ColNum == 1)
		 && Tbl->brdrl
		 && (!Cell->brdrl
		  || (Cell->brdrl->type != brdrhidden))
		 && (Cell->cellrlov == false))
			Cell->brdrl = Tbl->brdrl;

		// table ruling overrides cell ruling for top of first row
		if ((RowNum == 1)
		 && Tbl->brdrt
		 && (!Cell->brdrt
		  || (Cell->brdrt->type != brdrhidden))
		 && (Cell->cellrtov == false))
			Cell->brdrt = Tbl->brdrt;

		// table ruling overrides cell ruling for bottom of last row
		if ((!NextRow)
		 && Tbl->brdrb
		 && (!Cell->brdrb
		  || (Cell->brdrb->type != brdrhidden))
		 && (Cell->cellrbov == false))
			Cell->brdrb = Tbl->brdrb;

		// head and foot separator ruling overrides cell ruling
		if ((Row->rtype == 2)
		 && NextRow
		 && (NextRow->rtype == 1)
		 && Row->brdrx
		 && (!Cell->brdrb
		  || (Cell->brdrb->type != brdrhidden))
		 && (Cell->cellrbov == false))
			Cell->brdrb = Row->brdrx;
		else if ((Row->rtype == 3)
		 && LastRow
		 && (LastRow->rtype == 1)
		 && Row->brdrx
		 && (!Cell->brdrt
		  || (Cell->brdrt->type != brdrhidden))
		 && (Cell->cellrtov == false))
			Cell->brdrt = Row->brdrx;
  }

  // write right cell rule even if cell is straddled
	if (Cell->straddling) {
		stradcell = Cell;
		stradrt = Cell->brdrr ? *Cell->brdrr : NoBrdr;
		stradrov = Cell->cellrrov;
	}

	if (Cell->straddled
	 && !Cell->vstrad) {
		if (!Cell->brdrr)
			Cell->brdrr = new brdrinfo;
		if (Cell->brdrr->type != brdrhidden) {
			*Cell->brdrr = stradrt;
			Cell->cellrrov = stradrov;
		}
	}

	// table ruling overrides cell ruling for right of last col
	if ((ColNum == Tbl->tbccount)
	 && Tbl->brdrr) {
		if (!Cell->brdrr)
			Cell->brdrr = new brdrinfo;
		if ((Cell->brdrr->type != brdrhidden)
		 && (Cell->cellrrov == false))
			*Cell->brdrr = *Tbl->brdrr;
		if (Cell->straddled
		 && stradcell) {
			if (!stradcell->brdrr)
				stradcell->brdrr = new brdrinfo;
			*stradcell->brdrr = *Cell->brdrr;
			stradcell = NULL;
		}
	}	
}


void
RTFwr::WriteCellBorder(RTFwfile *wf)
{
	if (Cell->straddled
	 && MergeStradCells)
		return;

  if (!Cell->straddled)	{
		if (Cell->brdrl)
			WriteOneBorder(wf, Cell->brdrl, "clbrdrl");
		if (Cell->brdrt)
			WriteOneBorder(wf, Cell->brdrt, "clbrdrt");
		if (Cell->brdrb)
			WriteOneBorder(wf, Cell->brdrb, "clbrdrb");
#if 0
    if (Cell->cellrl[0] > 1) {
      wf->putCW("clbrdrl");
      WriteRule(wf, Cell->cellrl);
    }
    if (Cell->cellrt[0] > 1) {
      wf->putCW("clbrdrt");
      WriteRule(wf, Cell->cellrt);
    }
    if (Cell->cellrb[0] > 1) {
      wf->putCW("clbrdrb");
      WriteRule(wf, Cell->cellrb);
    }
#endif

	}

  // write right cell rule even if cell is straddled
	if (Cell->brdrr)
		WriteOneBorder(wf, Cell->brdrr, "clbrdrr");
#if 0
  if (Cell->cellrr[0] > 1) {
    wf->putCW("clbrdrr");
    WriteRule(wf, Cell->cellrr);
  }
#endif
}


void
RTFwr::WriteRule(RTFwfile *wf, uns *rul)
{
  wf->putCW(CellBordStyles[rul[0] - 2]);
  wf->putCWN("brdrw", (rul[1] & 0xF) * 15);
  if (((rul[1] >> 4) & 0xF) > 2)
    wf->putCWN("brdrcf", (rul[1] >> 4) & 0xF);
}


void
RTFwr::WriteHelpCellBorder(RTFwfile *wf)
{
	if (Cell->straddled
	 && MergeStradCells
	 && !Cell->vstrad)
		return;

	if (TbRuleType == 'P') {
		if ((Row->rtype == 2)
		 && Cell->brdrb)
			WriteOneBorder(wf, Cell->brdrb, "box");
		if ((Row->rtype == 1)
		 && Cell->brdrr)
			WriteOneBorder(wf, Cell->brdrr, "box");
		if ((Row->rtype == 3)
		 && Cell->brdrt)
			WriteOneBorder(wf, Cell->brdrt, "box");
	}
	else if (TbRuleType == 'B') {
		wf->putCW("box");
		wf->putCW("brdrs");
	}
	else if (TbRuleType == 'S') {
		wf->putCW("box");
		wf->putCW("brdrsh");
	}
	else if (TbRuleType == 'D') {
		wf->putCW("box");
		wf->putCW("brdrdb");
	}
	else if (TbRuleType == 'T') {
		wf->putCW("box");
		wf->putCW("brdrth");
	}
	else if (TbRuleType == 'V') {

		if (TbRuleAdjust) {
			if (!Cell->brdrl              // no left:
			 && !Cell->brdrr) {           // and no right,
				Cell->brdrt = NULL;         //  remove top
				Cell->brdrb = NULL;         //  and bot;
			}
			else {                                   // otherwise is ruled:
				Cell->brdrl = Cell->brdrr;     //  make left match right.
				if (!Cell->brdrt            //  if no top, and
				 && StartingTbCell)                    //  not a lower segment,
					Cell->brdrt = Cell->brdrb;   //   make top match bot
			}
		}

		SetCellBorder();  // override based on outer

		if (Cell->brdrl)
			WriteOneBorder(wf, Cell->brdrl, "brdrl");
		if (StartingTbCell
		 && Cell->brdrt)
			WriteOneBorder(wf, Cell->brdrt, "brdrt");
		if (EndingTbCell
		 && Cell->brdrb)
			WriteOneBorder(wf, Cell->brdrb, "brdrb");
		if (Cell->brdrr)
			WriteOneBorder(wf, Cell->brdrr, "brdrr");
	}

#if 0
	if (TbRuleType == 'P') {
		if ((Row->rtype == 2)
		 && (Cell->cellrb[0] > 1)) {
			wf->putCW("box");
			WriteHelpRule(wf, Cell->cellrb);
		}
		if ((Row->rtype == 1)
		 && (Cell->cellrr[0] > 1)) {
			wf->putCW("box");
			WriteHelpRule(wf, Cell->cellrr);
		}
		if ((Row->rtype == 3)
		 && (Cell->cellrt[0] > 1)) {
			wf->putCW("box");
			WriteHelpRule(wf, Cell->cellrt);
		}
	}
	else if (TbRuleType == 'B') {
		wf->putCW("box");
		wf->putCW("brdrs");
	}
	else if (TbRuleType == 'S') {
		wf->putCW("box");
		wf->putCW("brdrsh");
	}
	else if (TbRuleType == 'D') {
		wf->putCW("box");
		wf->putCW("brdrdb");
	}
	else if (TbRuleType == 'T') {
		wf->putCW("box");
		wf->putCW("brdrth");
	}
	else if (TbRuleType == 'V') {

		if (TbRuleAdjust) {
			if ((Cell->cellrl[0] < 2)               // no left:
			 && (Cell->cellrr[0] < 2)) {            // and no right,
				Cell->cellrt[0] = 0;                   //  remove top
				Cell->cellrb[0] = 0;                   //  and bot;
			}
			else {                                   // otherwise is ruled:
				Cell->cellrl[0] = Cell->cellrr[0];     //  make left match right.
				if ((Cell->cellrt[0] < 2)             //  if no top, and
				 && StartingTbCell)                    //  not a lower segment,
					Cell->cellrt[0] = Cell->cellrb[0];   //   make top match bot
			}
		}

		SetCellBorder();  // override based on outer

		if (Cell->cellrl[0] > 1) {
			wf->putCW("brdrl");
			WriteHelpRule(wf, Cell->cellrl);
		}
		if (StartingTbCell
		 && (Cell->cellrt[0] > 1)) {
			wf->putCW("brdrt");
			WriteHelpRule(wf, Cell->cellrt);
		}
		if (EndingTbCell
		 && (Cell->cellrb[0] > 1)) {
			wf->putCW("brdrb");
			WriteHelpRule(wf, Cell->cellrb);
		}
		if (Cell->cellrr[0] > 1) {
			wf->putCW("brdrr");
			WriteHelpRule(wf, Cell->cellrr);
		}
	}
}


void
RTFwr::WriteHelpRule(RTFwfile *wf, uns *rul)
{
  wf->putCW(CellBordStyles[rul[0] - 2]);
#endif
}


void
RTFwr::WriteCell(RTFwfile *wf)
{
	if (Cell->straddled
	 && MergeStradCells)
		return;

	if (!StripTb)
		SetPardProps(wf);

  if (!Cell->straddled
	 && (!Cell->vstrad || HelpForm)
	 && Cell->cont) {

    InTbl = false;
		if (!StripTb)
			StartingTbCell = true;
		GrTbSpace = false;
		XrefFile = NULL;
		XrefFilePrefix = CurrFilePrefix;

		long lasttell = wf->tell();

		assert(Tbl);
		tbl *xtbl = Tbl;

		if (Cell->vstrad) {
			RTFctl *cp = new RTFctl(imnem, 2, 10, 1, 160);
			FindRTF(cp);
			Cell->cont->insert(*cp, *(Cell->cont->last->ctl));
		}

    (Cell->cont)->write(*wf);

		assert(Tbl);

		if ((HelpForm == false)
		 && (wf->tell() < lasttell + 3)
		 && Curr.keepn) {
			wf->putCW("keepn");
		}


		if (InHelpTitlePara   // only use the title, if really present
		 && (wf->tell() > (lasttell + 3))) {
			InHelpTitlePara = false;
			HelpTitlePending = false;
		}

    InTbl = true;
  }

	if (!StripTb) {
	  wf->putCW("cell");
		wf->putEnd();
	}

	if (Tbl->tblstrip
	 && StrippedCellPar)
		wf->putCW("par");

  ParaEnding = false;
}


// rule properties


DCvlist RTFwr::RuleFormList;
DCnlist RTFwr::RuleNameList;
//uns RTFwr::Rule[2];
// [0] = style, 0 = unset, 1 = none, 2 = single, 3 = double, 4 = thick
// [1] = ruling width, 15, 30, 45, or 60 twips (thick is * 2)
//rule RTFwr::DCLrule;

brdrinfo *RTFwr::RuleDef = NULL;
brdrinfo RTFwr::NoBrdr;
bool RTFwr::InRuleDef;


void
RTFwr::ProcRuleForm(void)
{
  InRuleDef = true;
	RuleDef = new brdrinfo;
  //DCLrule.rwid = 0;
  //DCLrule.rlines = 0;
}


void
RTFwr::ProcRuleProp(RTFctl *cp)
{
  if ((cp->maj() != 4)   // graphics
   || (cp->min() != 20)) // line props
    return;

  switch (cp->dat()) {
    case 2:  // color
      GetColor(cp->siz(), (uns *) &RuleDef->color);
      //GetColor(cp->siz(), &DCLrule.rulcolr);
      break;
    case 3:  // thickness
      RuleDef->thick = GetTwips(cp->siz(), gl_unit);
      //DCLrule.rwid = GetTwips(cp->siz(), gl_unit);
      break;
    case 5:  // count
			switch (cp->siz()) {
				case 2:
					RuleDef->type = brdrdb;
					break;
				default:
				case 1:
					RuleDef->type = brdrs;
					break;
			}
      //DCLrule.rlines = (uns) cp->siz();
      break;
    default:
      break;
  }
}


void
RTFwr::StoreRuleForm(void)
{
	if (RuleDef->thick > 75) {
		if (RuleDef->type == brdrs) {
			RuleDef->type = brdrth;
			RuleDef->thick /= 2;
		}
		if (RuleDef->thick > 75) {
			RuleDef->thick = 75;
			LogEvent(logwarn, 2, "Border thickness excessive for ", FormName);
		}
	}

#if 0
  if (DCLrule.rlines > 1)
    Rule[0] = 3;
  else if (DCLrule.rwid > 52) {
    DCLrule.rwid /= 2;
    Rule[0] = 4;
  }
  else Rule[0] = 2;

  if ((DCLrule.rwid > 52) && (Rule[0] == 4))
    Rule[1] = 4;
  else if (DCLrule.rwid > 37)
    Rule[1] = 3;
  else if ((DCLrule.rwid > 22) || (Rule[0] == 4))
    Rule[1] = 2;
  else Rule[1] = 1;

  if ((DCLrule.rulcolr > 1) && (DCLrule.rulcolr != Invisible))
    Rule[1] += DCLrule.rulcolr << 4;

  long lval = (( (long) Rule[0]) << 16) | (Rule[1] & 0xFFFF);
  RuleFormList.add(lval, DCLFormNum);
#endif

  RuleFormList.add(RuleDef, DCLFormNum);
  if (FormName)	{
		RuleNameList.add(FormName, DCLFormNum);
		brdrinfo *br = GetTableFileBorder(FormName);
		if (br)
			*RuleDef = *br;
		FormName = NULL;
	}
	RuleDef = NULL;
  InRuleDef = false;
}


brdrinfo *
RTFwr::GetTableFileBorder(char *bfmt)
{
	if (!bfmt
	 || !TablesFile)
		return NULL;

	brinfo *br = TablesFile->GetBorder(bfmt);

	return (brdrinfo *) br;
}



void
RTFwr::GetRule(RTFctl *cp, brdrinfo **rptr)
{
	if (!rptr
	 || (cp->siz() == 0))
		return;

	*rptr = new brdrinfo;
	brdrinfo *br = (brdrinfo *) RuleFormList.find(cp->siz());
	if (br) {
		*rptr = new brdrinfo;
		**rptr = *br;
	}


#if 0
  long lval;

  assert(rptr);
  if (cp->siz() == 0) {
    rptr[0] = 1;
    return;
  }

  lval = RuleFormList.find(cp->siz());
  if (lval) {
    rptr[0] = (lval >> 16) & 0xFFFF;
    rptr[1] = lval & 0xFFFF;
  }
#endif
}



// fill patterns


uns RTFwr::FillMap[FillMapMax] = {  // specific to Frame MIF
  CellFillEmpty, 10000, 9000, 7000,
  5000, 3000, 1000, 500,
  CellFillWhite, 10, 9, 8,
  7, 6, 4, 3
};


void
RTFwr::GetFill(RTFctl *cp, uns *fptr)
{
  if (cp->siz() < FillMapMax)
    *fptr = FillMap[cp->siz()];
}


void
RTFwr::GetShading(RTFctl *cp, shdinfo **fptr)
{
	uns fill = 0;

  if (cp->siz() < FillMapMax)
    fill = FillMap[cp->siz()];

	if (!*fptr)
		*fptr = new shdinfo;

	if (fill > CellFillMax)
		(*fptr)->tint = fill;
	else
		(*fptr)->type = (shdtyp) fill;
}


void
RTFwr::GetShadeColor(RTFctl *cp, shdinfo **fptr)
{
	//if (((int) fptr) < 0x100)  // prevents a bug in Release ver only
	//	return;

	uns colornum = 0;
	GetColor(cp->siz(), &colornum);

	if (!*fptr)
		*fptr = new shdinfo;

	(*fptr)->color = colornum;
}


void
RTFwr::WriteCellShadingFmt(RTFwfile *wf)
{
	shdinfo *sh = Cell->cellshd;
	if (!sh)
		return;

	if (TblFillShade) {
		if (sh->tint)              // grayscale
			wf->putCWN("clshdng", (short) sh->tint);
		else if (sh->type)         // fill pattern
			wf->putCW(CellFillPats[sh->type - 1]);
	}

  if (TblFillColor) {
		if (sh->color > 2)
			wf->putCWN("clcfpat", sh->color - 1);
		if ((sh->bcolor > 1)
		 && (sh->bcolor != 9)
		 && sh->type)
			wf->putCWN("clcbpat", sh->bcolor - 1);
	}
}


void
RTFwr::WriteCellShading(RTFwfile *wf)
{
  if (TblFillColor
	 && (Cell->cellcolr > 2))
    wf->putCWN("clcfpat", (Cell->cellcolr - 1) & 0xFF);

	if (TblFillShade) {
		if (Cell->cellfill > CellFillMax)   // grayscale
			wf->putCWN("clshdng", Cell->cellfill);
		else if (Cell->cellfill)            // fill pattern
			wf->putCW(CellFillPats[Cell->cellfill - 1]);
	}
}


// colors

uns RTFwr::ColorsUsed = 8;  // initial color table
uns RTFwr::LastColor = 10;

uns RTFwr::ColorMap[ColorMapMax] = {  // DCL to RTF mapping
  Invisible,
   2,  9,  7,  5,
   3,  4,  6,  8
//  17, 16, 14, 12,
//  10, 11, 13, 15
};

unc RTFwr::RedTable[ColorTableMax] = {
 0, 0, 0, 0,
 255, 255, 255, 255,
 0, 0, 0, 127,
 127, 127, 127, 192
};

unc RTFwr::GreenTable[ColorTableMax] = {
 0, 0, 255, 255,
 0, 0, 255, 255,
 0, 127, 127, 0,
 0, 127, 127, 192
};

unc RTFwr::BlueTable[ColorTableMax] = {
 0, 255, 255, 0,
 255, 0, 0, 255,
 127, 127, 0, 127,
 0, 0, 127, 192
};


void
RTFwr::GetColor(unl cpsiz, uns *cptr)
{
	uns col;

  if ((cpsiz <= ColorMapMax)
	 && ((col = ColorMap[cpsiz]) > 0)
	 && ((col <= (ColorsUsed + 2))
	  || (col == Invisible)))
	  *cptr = col;
	else
		*cptr = 2;  // black
}


void
RTFwr::WriteColorTable(RTFwfile *wf)
{
  wf->putEnd();
  wf->putGS();
  wf->putCW("colortbl");
  wf->putChar(';');
  for (uns i = 0; i < ColorsUsed; i++) {
    wf->putEnd();
    wf->putCWN("red", RedTable[i] & 0xFF);
    wf->putCWN("green", GreenTable[i] & 0xFF);
    wf->putCWN("blue", BlueTable[i] & 0xFF);
    wf->putChar(';');
  }
  wf->putGE();
}


unl
RTFwr::CheckColorNum(uns cnum)
{
	if (cnum == 0)
		return 0;
	if (cnum == 1)
		return 0;
	cnum -= 2;
  if (cnum <= ColorsUsed)
		return ((((unl) RedTable[cnum]) << 16)
					+ (((unl) GreenTable[cnum]) << 8)
					+ ((unl) BlueTable[cnum]));

	return 0;
}


uns
RTFwr::CheckColorValue(unl cval, uns def)
{
	unc r = (unc) ((cval >> 16) & 0xFF);
	unc g = (unc) ((cval >>  8) & 0xFF);
	unc b = (unc) (cval & 0xFF);
	bool matched = false;
	uns i = 0;
 
  for (i = 0; i < ColorsUsed; i++) {
		if ((RedTable[i] == r)
		 && (GreenTable[i] == g)
		 && (BlueTable[i] == b))
			return i + 2;
	}
	if (i < (ColorTableMax - 1)) {
		RedTable[ColorsUsed] = r;
		GreenTable[ColorsUsed] = g;
		BlueTable[ColorsUsed] = b;
		return (i = ColorsUsed++) + 2;
	}

	return def;
}


uns
RTFwr::ProcColorDef(unl cval, uns cnum)
{
	// determine closest color in map to color requested and store in list
	if (cnum >= ColorMapMax)
		return 0;

	if (cnum) {  // DCL definition, not from formats
		if (((cval >> 24) & 0xFF) == 1)  // RGB mapping
			cval &= 0xFFFFFF;
		else
			return 0; // some other mapping
	}

	uns cind = 0; // default is black
	unc r = (unc) ((cval >> 16) & 0xFF);
	unc g = (unc) ((cval >>  8) & 0xFF);
	unc b = (unc) (cval & 0xFF);
	bool matched = false;
	uns i = 0;

  for (i = 0; i < ColorsUsed; i++) {
		if ((RedTable[i] == r)
		 && (GreenTable[i] == g)
		 && (BlueTable[i] == b)) {
			cind = i;
			matched = true;
			break;
		}
	}
	if (matched && !cnum) {
		for (i = 0; i < LastColor; i++) {
			if (ColorMap[i] == (cind + 2)) {
				cnum = i;  // in map already
				break;
			}
		}
		if (!cnum) {   // not already in map
			cnum = ++LastColor;
			ColorMap[cnum] = cind + 2;
		}
		return cnum;
	}

	if (!matched) {  // add to color table
		if (ColorsUsed >= (ColorTableMax - 1))
			return 0;    // no room to add it
		RedTable[ColorsUsed] = r;
		GreenTable[ColorsUsed] = g;
		BlueTable[ColorsUsed] = b;
		cind = ColorsUsed++;
	}

	if (!cnum)
		cnum = ++LastColor;

	ColorMap[cnum] = cind + 2;

	if (LastColor < cnum)
		LastColor = cnum;

	return cnum;
}




// end of dwrtftb.cc


