 
// dwhtmtb.cc is the code for dcl Microsoft HTM table writing, JHG, 1/5/97
// Copyright (c) 1996 by Omni Systems, Inc.  All rights reserved.

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



// processing switch


bool
HTMwr::ProcTb(HTMctl *cp)
{
  switch((cp->rp)->scan_op) {
    case nothing:   // just put with its group for later
      break;

    case pass:      // ignore, don't save
      return false;

		case tbstart:
		  Tbl = new tbl;
			TblCpList.add(Tbl, (long) cp);
			EmptyTable = true;
			InTbl = true;
			TblCpID = (long) cp;
			TxtParaUID = ParaUID;
			SaveCondState();
			SaveParaVars();
			if (!InTblDef  // collect ID from next hypunq
			 && (LastPage > LastMasterPage)) {
				ProcCondTable();
				TableIDPend = true;
			}
			break;

		case tbend:
			if (EmptyTable)
				EmptyTableList.add(TblCpID);
			MarkList.all(GetTblAttrs);
			if (DCini::PendingTblGroup) {
				Tbl->tbgroup = DCini::PendingTblGroup;
				DCini::PendingTblGroup = NULL;
			}
			TblIni.nm[0] = Tbl->tblid;
			if (TblIni.nm[0])
				DCini::TblSetList.all(DCini::SetTblID);
			InTbl = false;
			TblCpID = 0;
			TblAttrLp = NULL;
			HXMwr::CurrDITATblID = NULL;
			if (HXMwr::DITA)
				StripTable = SavedStrip;
			RestoreParaVars();
			RestoreCondState();
			TableConds = NULL;
			break;

		case tblchk:
			break;

		case tblform:
			TblForm = (tbl *) TblFormList.find(cp->siz());
			assert(TblForm);
			assert(Tbl);
			if (TblForm && Tbl)
				Tbl->tblfname = TblForm->tblfname;
			if (HXMwr::DITA) {
				SetTblIni();
				DCini::ProcIniSect("DITATables");
				SavedStrip = StripTable;
				Tbl->dtname = CTbl.dtname;
				if ((Tbl->dtname
					|| ((Tbl->dtname = HXMwr::DefDITATbl) != NULL))
				 && !stricmp(Tbl->dtname, "strip")) {
					StripTable = true;
				}
			}
			break;

		case tbltits:
			InTbTitle = true;
			break;

		case tbltite:
			InTbTitle = false;
			break;

		case trstart:
			HideTbRow = ShowTbRow = false;
      if (InTblDef)
        ProcTblProp(NULL, cp);
			else {
				RowCpID = (long) cp;
				FirstCol = true;
			}
			break;

		case condrset:
			ProcTbCondUse(cp);
			if (HideTbRow)
				break;
			EmptyTable = false;
      if (!InTblDef
			 && !InTDefaults
			 && FirstCol
			 && (LastPage > LastMasterPage))
				ProcCondRowUse(cp->dat());
			break;

		case trend:
      if (InTblDef)
        ProcTblProp(NULL, cp);
			else {
				if (!InTDefaults
				 && (LastPage > LastMasterPage))
					ProcCondRowEnd();
				MarkList.all(GetRowAttrs);
				RowAttrLp = NULL;
				RowCpID = 0;
			}
			if (!HideTbRow
			 || ShowTbRow)
				EmptyTable = false;
			HideTbRow = ShowTbRow = false;
			break;

		case tcstart:
      if (InTblDef)
        ProcTblProp(NULL, cp);
			else {
 				if (FirstCol) {
					//ProcCondRowStart();
					FirstCol = false;
				}
				InTbCell = true;
				CellCpID = (long) cp;
			}
			break;

		case tcend:
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
      if (InTblDef)
        ProcTblProp(NULL, cp);
			else {
				MarkList.all(GetCellAttrs);
				InTbCell = false;
				CellAttrLp = NULL;
				CellCpID = 0;
			}
			break;

    case ruleprop:  // process rule properties
      if (!InRuleDef)
        return ProcGr(cp);
      ProcRuleProp(cp);
      return false;

    case tformck:   // process props in table formats
      if (InTblDef)
        ProcTblProp(NULL, cp);
			else switch ((cp->rp)->write_op) {
				case tbldflt:
					InTDefaults = true;
					break;
				case tbedflt:
					InTDefaults = false;
					break;
				default:
					break;
			}
      break;

    case tmargck:   // process cell margins in formats
      if (InFormDef) {
				SetCellMarg(cp);
        return false;
      }
      break;

		case colrdef:   // process color definition
			if ((cp->siz() & 0xFF000000) == 0x01000000)  // RGB
				ProcColorDef(cp->dat() + 1, cp->siz() & 0x00FFFFFF);
			break;
		case colrname:
			ColorNames.add((char *) cp->ex(), cp->dat());
			if (IniFile
			 && IniFile->Section("Colors")
			 && (IniFile->Find((char *) cp->ex())
			  || IniFile->Find(cp->dat() + 1)))
				ProcColorDef(cp->dat() + 1, IniFile->HexRight());
			break;

    default:
      assert(0);
			LogEvent(logdebug, 1, "Invalid case in ProcTb()");
      return false;
  }
  return true;
}



// table properties


void
HTMwr::ProcTblProp(HTMwfile *wf, HTMctl *cp)
{
  HTMctl *ncp;
  long *lptr;
  short i;

  if (cp->rp == NULL)
    return;

  switch ((cp->rp)->write_op) {
    case ignore:
      break;

    case formtbl:
      UseTblForm(cp);
      break;
		case ftaguse:
			UseTblTag(cp);
			break;
		case ftagspc:
			UseSpecTblTag(cp);
			break;

    case tbplace:
      SaveState();
      Curr.lind = Curr.aspace = Curr.bspace = 0;
      Curr.algn = 0;
      Curr.pbreak = 0;
      break;
    case putalgn:
			if (Cell) {
				if (cp->dat() > 3)
					Cell->algn = 1;
				else 
					Cell->algn = (unc) cp->dat();
			}
			// fall through
    case putindt:
    case putpspc:
    case putbreak:
    case putkeep:
    case putkeepn:
      SetCurrProps(wf, cp);
      if (InTbCell)
	      AddCellCont(cp);
      break;
    case tbeplac:
      Tbl->tblalgn = Curr.algn;
      Tbl->tbllind = Curr.lind;
      Tbl->tblbspc = Curr.bspace;
      Tbl->tblaspc = Curr.aspace;
      Tbl->tblbrk = Curr.pbreak;
      RestoreState();
      break;

    case tbtplace:
			Tbl->titleplace = TableTitles ? TableTitles : (uns) cp->siz();
      break;
    case tbtgap:
      Tbl->titlegap = (uns) cp->siz(); // will convert to twips later
      break;
    case tbltitl:
      if (!InTDefaults) {  // set up group to get title
        InTbTitle = true;
        Tbl->titlecont = new HTMgroup;
      }
      break;
    case tbetitl:
      if (!InTDefaults)   // done collecting title
        InTbTitle = false;
      break;

    case tbldflt:
      InTDefaults = true;
      break;
    case tbedflt:
      InTDefaults = false;
			if (Tbl->tblfname == NULL)  // still no format name set
				Tbl->tblfname = "Default";
			TableIDPend = true;
      break;
		case hypunq:
			if (!TableIDPend) // ID is for a cell
				goto normal;
			SetTblID(cp);
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

		case tblalrc:
      assert(Tbl);
			Tbl->altrowcol = (uns) cp->siz() + 1;
      break;
		case tblncnt:
      assert(Tbl);
			Tbl->normcount = (uns) cp->siz();
      break;
		case tblacnt:
      assert(Tbl);
			Tbl->altcount = (uns) cp->siz();
      break;
    case tblaclr:
      assert(Tbl);
      //GetColor(cp, &Tbl->altcolr);
			GetShadeColor(cp, &Tbl->tblaltshd);
      break;
    case tblafll:
      assert(Tbl);
      //GetFill(cp, &Tbl->altfill);
			GetShading(cp, &Tbl->tblaltshd);
      break;

    case tbcol:
      ProcTblCol(cp);
      break;
    case tbcend:
      TbCol = 0;
      break;
		case tbcols:
			CellRowStrad = new uns[cp->siz() + 1];
			CurrRowStrad = new uns[cp->siz() + 1];
			break;
    case tbcwid:
 			TableIDPend = false;
      if (TbCol) {
				if (!TableWidthType)  // fixed values
					Col->colwid = GetTwips(cp->siz(), pg_unit);
				else  { // percentages
					Col->colpct = (uns) cp->siz();
					Col->colwid = (uns) cp->siz() * 100;
				}
			}
      break;
		case tbcwtyp:
			TableWidthType = (bool) cp->siz();
			break;
    case tbcwids:
			TableIDPend = false;
      lptr = (long *) cp->ex();
      Tbl->tbccount = (uns) (cp->siz() / sizeof(long));
			Tbl->vstrad = (cell **) new cell *[Tbl->tbrcount * (Tbl->tbccount + 1)];
      if (Tbl->collist == NULL)
        Tbl->collist = new DCvlist;
      for (i = 1; i <= Tbl->tbccount; i++) {
        if ((Col = (col *) ((Tbl->collist)->find(i))) == NULL) {
          (Tbl->collist)->add((Col = new col), i);
					Col->brdrl = Tbl->brdrcl;
					Col->brdrr = Tbl->brdrcr;
				}
				if (*lptr != 1) { // placeholder, means use default
					if (!TableWidthType)  // fixed values
						Col->colwid = GetTwips(*lptr, pg_unit);
					else  { // percentages
						Col->colpct = (uns) *lptr;
						Col->colwid = ((uns) *lptr) * 100; // 500pt ~= 7"
					}
				}
				if (!Col->colwid)
					Col->colwid = DefColWidth;  // default to one inch
				if (UseAltShading && Tbl->altcount && Tbl->normcount
				 && (Tbl->altrowcol == 2)
				 && (((i - 1) % (Tbl->normcount + Tbl->altcount)) >= Tbl->normcount))
					Col->alt = true;
        lptr++;
      }
      if (TbCol)
        Col = (col *) (Tbl->collist)->find(TbCol);
      else RowNum = 0;
      break;

    case tbcfill:
      assert(Col);
			if (FMSource)
				GetFill(cp, &Col->colfill);
			else
				GetShading(cp, &Col->colshd);
      break;
    case tbccolr:
      assert(Col);
			if (FMSource)
				GetColorS(cp, &Col->colcolr);
			else
				GetShadeColor(cp, &Col->colshd);
      break;
    case tbcrull:
      GetRule(cp, TbCol ? &Col->brdrl : &Tbl->brdrcl);
      break;
    case tbcrulr:
      GetRule(cp, TbCol ? &Col->brdrr : &Tbl->brdrcr);
      break;

		case conduse:
 			if (InTDefaults)
				break;
      if (InTbTitle)
        (Tbl->titlecont)->add(*cp);
      else if (InTbCell)
        AddCellCont(cp);
			else {
				ProcTbCondUse(cp);   // commented out right after conduse ???
				CondStart(cp->dat());
			}
			break;
		case condend:
 			if (InTDefaults)
				break;
      if (InTbTitle)
        (Tbl->titlecont)->add(*cp);
      else if (InTbCell)
        AddCellCont(cp);
			else if (FirstCol)
				ProcNextCellCont(cp);
			else
				CondEnd(cp->dat());
			break;
		case condruse:
			// handled during scan
			ProcTbCondUse(cp);
			break;

    case tbrow:
      ProcRow(cp);
      LastCellwid = 0;
			ShowTbRow = false;
			HideTbRow = false;
			FirstCol = true;
      break;
    case tbrend:
			if ((ShowTbRow == false)
			 && (HideTbRow == true))
				Row->rtype = 0;
      ProcRowEnd();
			ShowTbRow = false;
			HideTbRow = false;
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
			if (FMSource)
				GetColorS(cp, &Row->rowcolr);
			else
				GetShadeColor(cp, &Row->rowshd);
      break;
    case tbrfill:
      assert(Row);
			if (FMSource)
				GetFill(cp, &Row->rowfill);
			else
				GetShading(cp, &Row->rowshd);
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
    case tbclcolr:
      assert(Cell);
			if (FMSource)
	      GetColorS(cp, &Cell->cellcolr);
			else
				GetShadeColor(cp, &Cell->cellshd);
      break;
    case tbclfill:
      assert(Cell);
			if (FMSource)
	      GetFill(cp, &Cell->cellfill);
			else
				GetShading(cp, &Cell->cellshd);
      break;
    case tbclrull:
      assert(Cell);
      GetRule(cp, &Cell->brdrl);
      break;
    case tbclrulr:
      assert(Cell);
      GetRule(cp, &Cell->brdrr);
      break;
    case tbclrult:
      assert(Cell);
      GetRule(cp, &Cell->brdrt);
      break;
    case tbclrulb:
      assert(Cell);
      GetRule(cp, &Cell->brdrb);
      break;

    case tbclmarl:
      if (InTbCell)
        Cell->cellmargl = GetTwips(cp->siz(), ld_unit);
      else Tbl->tblmargl = GetTwips(cp->siz(), ld_unit);
      break;
    case tbclmarr:
      if (InTbCell)
        Cell->cellmargr = GetTwips(cp->siz(), ld_unit);
      else Tbl->tblmargr = GetTwips(cp->siz(), ld_unit);
      break;
    case tbclmart:
      if (InTbCell)
        Cell->cellmargt = GetTwips(cp->siz(), ld_unit);
      else Tbl->tblmargt = GetTwips(cp->siz(), ld_unit);
      break;
    case tbclmarb:
      if (InTbCell)
        Cell->cellmargb = GetTwips(cp->siz(), ld_unit);
      else Tbl->tblmargb = GetTwips(cp->siz(), ld_unit);
      break;
    case tbclmrtl:
      if (InTbCell)
        Cell->cellmartl = (unc) cp->siz();
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

		case putfr: 
		case puttx:
      if (InTbTitle) {
        if (Tbl->titlegap) { // insert space above or below for gap
          ncp = new HTMctl(ilong, 2, 22, (Tbl->titleplace == 1) ? 3 : 1,
                           Tbl->titlegap);  // raw size, convert later
          ncp->rp = new htm;
          (ncp->rp)->write_op = putpspc;
          Tbl->titlegap = 0;
          (Tbl->titlecont)->add(*ncp);
        }
        (Tbl->titlecont)->add(*cp);
        break;
      }
      StartCellText(cp);
      AddCellCont(cp);
      break;

    case putftn:
			if (InTDefaults)
				break;
			SaveCondStateFtn(true);
      if (InTbTitle)
        (Tbl->titlecont)->add(*cp);
      else if (InTbCell) {
				InFootnote = true;
        AddCellCont(cp);
			}
      break;

    case endftn:
			if (InTDefaults)
				break;
      if (InTbTitle)
        (Tbl->titlecont)->add(*cp);
      else if (InTbCell) {
				InFootnote = false;
        AddCellCont(cp);
			}
			RestoreCondStateFtn();
      break;

		case formch:
			//if (!InPara)
			//	break;
		case formcall:
			UseParaForm(cp);
			goto normal;

		case setxref:
 			if (InTDefaults)
				break;
      if (InTbTitle)
        (Tbl->titlecont)->add(*cp);
      else if (InTbCell)
        AddCellCont(cp);
			else
				PutXrefAName(wf, cp);
			break;

    default:
		normal:
      if (InTbTitle) {
        (Tbl->titlecont)->add(*cp);
        break;
      }
      if (InTbCell)
        SetCurrProps(wf, cp);
      AddCellCont(cp);
      break;
  }
}



// table format processing


DCvlist HTMwr::TblFormList;
tbl *HTMwr::TblForm = NULL;
bool HTMwr::InTblDef = false;

col *HTMwr::ColForm = NULL;
uns HTMwr::TbCol = 0;
bool HTMwr::TableWidthType = false;
bool HTMwr::TableIDPend = false;
bool HTMwr::KeepTableClassSpace = false;


void
HTMwr::ProcTblForm(void)
{
  InTblDef = true;
  Tbl = new tbl;
  TblFormList.add(Tbl, DCLFormNum);
}


void
HTMwr::StoreTblForm(void)
{
	// replace format name if mapped
	if (IniFile->Section("TableClasses")
	 && IniFile->Find(FormName))
		FormName = IniFile->StrRight();

	Tbl->tblfname = FormName ? FormName : "Default";

	GetTblFormatFileInfo(FormName);  // override Tbl with format info
	FormName = NULL;
  InTblDef = false;
}


void
HTMwr::UseTblForm(HTMctl *cp)
{
  col *tcol, *ncol;
  row *trow, *nrow;
  cell *tcell, *ncell;
  uns i, j;

  TblForm = (tbl *) TblFormList.find(cp->siz());
  assert(TblForm);
  assert(Tbl);

	char *id = Tbl->tblid;
	char *group = Tbl->tbgroup;
  uns rows = Tbl->tbrcount;
	DCnlist *al = Tbl->attrlist;
	long tbltitleprn = Tbl->titleprn;
	long tblpref = Tbl->tblpref;
  *Tbl = *TblForm;
	Tbl->titleprn = tbltitleprn;
	Tbl->attrlist = al;
  Tbl->tbrcount = rows;
	Tbl->tbgroup = group;
	Tbl->tblid = id;
	Tbl->tblpref = tblpref;

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


void
HTMwr::UseTblTag(HTMctl *cp)
{
	if (Tbl->tblfname)
		return;

	// tagged and no format name set
	Tbl->tblfname = NewName((char *) cp->ex());

  Tbl->collist = new DCvlist;
  Tbl->rowtlist = new DCvlist;
  Tbl->rowlist = new DCvlist;
}


void
HTMwr::UseSpecTblTag(HTMctl *cp)
{
	// cp->dat() is format type, cp->ex() is name  ****


}

void
HTMwr::SetTblID(HTMctl *cp)
{
	if (!TableIDPend)   // get table ID
		return;
	if (!Tbl->tblid) {
		Tbl->tblid = NewName(strlen(CurrFilePrefix) + 11);
		sprintf(Tbl->tblid, "%s%ld", CurrFilePrefix, (long) cp->siz());
		if (!Tbl->tblpref)
			Tbl->tblpref = ParaRefNum;
		HXMwr::DITATableIDList.add(Tbl->tblid, Tbl->tblpref);
	}
	if (TableConds) {
		CondTables.add(TableConds, (long) Tbl->tblid);
		TableConds = NULL;
	}
	TableIDPend = false;

	Tbl->tblncnt = 2;  // id comes after format set, so have tblfname
	DCini::ProcIniSect("TableGroup");
}



// table processing


DCvlist HTMwr::TblCpList;
DClist *HTMwr::TblList = NULL;
DClist *HTMwr::PreSufTbls = NULL;
tbl *HTMwr::Tbl = NULL;
tbl *HTMwr::FigTable = NULL;
tbl HTMwr::CTbl;
anyini HTMwr::TblIni;

bool HTMwr::CALS = false;

bool HTMwr::InTbl = false;
bool HTMwr::InTbTitle = false;
bool HTMwr::InTDefaults = false;
bool HTMwr::IntTbCaption = true;
bool HTMwr::TableAttributes = true;
bool HTMwr::UseAltShading = false;
bool HTMwr::StripTable = false;
bool HTMwr::SavedStrip = false;
bool HTMwr::CloseStrippedTables = true;
bool HTMwr::UseInformaltableTag = false;
bool HTMwr::UsingInformaltableTag = false;
char *HTMwr::TblCaptionTag = "caption";

col *HTMwr::Col;
uns HTMwr::ColNum;
uns HTMwr::LastColwid;
uns HTMwr::DefColWidth = 1440;
uns *HTMwr::CellRowStrad = NULL;
uns *HTMwr::CurrRowStrad = NULL;

uns HTMwr::TblBorderThick = 1;
uns HTMwr::TblCellPadding	= 6;
uns HTMwr::TblCellSpacing	= 2;

char HTMwr::TblWidType = 'A';
char HTMwr::TTblWidType = 'A';
uns HTMwr::TblDPI = 96;
long HTMwr::TblWid = 0;
long HTMwr::TblCpID = 0;

DCilist HTMwr::TblDivRefList;
long HTMwr::TblDivType = 0;

char *HTMwr::PixelSpacerImage = "1p.gif";
char *HTMwr::SpacerAlt = "[spacer]";
bool HTMwr::UseSpacers = false;
bool HTMwr::SpacerUsed = false;
bool HTMwr::WriteSpacerFile = false;
unc HTMwr::SpacerImage[SpacerImageSize] = {
 'G', 'I', 'F', '8', '9', 'a', 1, 0,
 1, 0, 0x80, 0xff, 0, 0xff, 0xff, 0xff,
 0, 0, 0, 0x21, 0xf9, 4, 1, 0,
 0, 0, 0, 0x2c, 0, 0, 0, 0,
 1, 0, 1, 0, 0, 2, 2, 0x44,
 1, 0, 0x3b
};
uns HTMwr::TblSpacerWide = 0;
short HTMwr::TblIndents = -1;

char *HTMwr::RowAttrib = NULL;
char *HTMwr::RowStartMacro = NULL;
char *HTMwr::RowEndMacro = NULL;
char *HTMwr::CellAttrib = NULL;
char *HTMwr::CellStartMacro = NULL;
char *HTMwr::CellEndMacro = NULL;

long HTMwr::TblFigureDiv = 0;
long HTMwr::PreTblFigureDiv = 0;


void
HTMwr::ProcTbl(HTMwfile *wf, HTMctl *cp)
{
  if (!HXMwr::HXML && !InPara) {  // style changes at start of para
		WriteParaProps(wf);
		InPara = true;
		ParaStarting = false;
	}

  SaveState();
	if (EmptyTableList.find((long) cp))
		EmptyTable = true;  // all rows conditioned out

	Tbl = (tbl *) TblCpList.find((long) cp);
	assert(Tbl);
  InTbl = true;
  Tbl->tbrcount = cp->dat();
	if (DCini::PendingTblGroup) {
		Tbl->tbgroup = DCini::PendingTblGroup;
		DCini::PendingTblGroup = NULL;
	}

	if (HXMwr::DITA) {
		SetTblIni();
		DCini::ProcIniSect("DITATables");
		SavedStrip = StripTable;
		Tbl->dtname = CTbl.dtname;
		if ((Tbl->dtname
		  || ((Tbl->dtname = HXMwr::DefDITATbl) != NULL))
		 && !stricmp(Tbl->dtname, "strip")) {
			StripTable = true;
		}
	}

	FirstBodyRow = 0;
  Curr = DefForm;

	Tbl->attrlist = (DCnlist *) TblAttrLists.find((long) cp);
}


void
HTMwr::SetTblIni(void)
{
	if (Tbl->tblid == NULL)
		Tbl->tblid = Tbl->tblfname;

	Tbl->tblncnt = (Tbl->tbgroup != NULL) ? 3 : 2;

	CTbl = *Tbl;
	TblIni.nm[0] = Tbl->tblid;     //  FDK id
	TblIni.nm[1] = Tbl->tblfname;  //  format
	TblIni.nm[2] = Tbl->tbgroup;   //  group
	TblIni.cnt = Tbl->tblncnt;     // names for ini

	if (!Tbl->tbgroup) {  // not set by marker
		DCini::ProcIniSect("TableGroup"); // check ini
		if (CTbl.tbgroup) {
			TblIni.nm[2] = CTbl.tbgroup;
			TblIni.cnt = 3;
		}
	}
}


void
HTMwr::ProcTblCol(HTMctl *cp)
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
  RowNum = 0;
}


void
HTMwr::WriteTbl(HTMwfile *wf)
{
  if (InPara || ParaStarting) {
	  InTbl = false;
	  RestoreState();
		//RestoreCondState();
		TableStartConds = CurrConds ? new DClist(CurrConds) : NULL;
		if (HXMwr::DITA)
			StripTable = SavedStrip;
		if (HXMwr::HXML)
			HXMwr::DITANoBlock = !stricmp(Curr.dtcname, "no") ? true : false;
		// store Tbl in TblList
		if (!EmptyTable) {
			if (Tbl->tblbrk || !Curr.sfx) {  // after suffix
				if (!TblList)
					TblList = new DClist;
				TblList->add((long) Tbl);
			}
			else {
				if (!PreSufTbls)
					PreSufTbls = new DClist;
				PreSufTbls->add((long) Tbl);
			}
		}
		else
			EmptyTable = false;
		Tbl = NULL;
		HXMwr::CurrDITATblID = NULL;
		return;
	}

	if (!Tbl)  // error
		return;
	InTbl = true;
	list savedlist = CurrList;
	unc savedlast = Last.list;
	CurrList.used = false;
	CurrList.level = 0;
	CurrList.last = 0;
	CurrList.typeval = 0;

	// set Tbl options per all defaults for tables
	TblSpacerWide = 0;
	Tbl->tbind = TblIndents;
	Tbl->tbsiz = TTblWidType;
	Tbl->attr = NULL;
	Tbl->tburc = TUseRowColor;
	Tbl->macrs = NULL;
	Tbl->macre = NULL;
	Tbl->attc = NULL;
	Tbl->maccs = NULL;
	Tbl->macce = NULL;

	// set default WAI attribs for table
	ColIDs = TColIDs;
	RowIDs = TRowIDs;
	ColSpanIDs = TColSpanIDs;
	RowSpanIDs = TRowSpanIDs;
	HeaderCols = THeaderCols;
	HeaderRows = THeaderRows;
	FooterRows = TFooterRows;
	ScopeCol = TScopeCol;
	ScopeColGroup = TScopeColGroup;
	ScopeRow = TScopeRow;
	ScopeRowGroup = TScopeRowGroup;
	ColGroupIDs = TColGroupIDs;
	RowGroupIDs = TRowGroupIDs;
	HeadFootBodyTags = THeadFootBodyTags;
	ColGroupElements = TColGroupElements;
	GroupNum = 0;
	ColGroupNum = 0;
	RowGroupNum = 0;


	// set table-specific overrides
	SetTblIni();
	DCini::ProcIniSect("TableAccess");
	DCini::ProcIniSect("TableClasses");
	DCini::ProcIniGroup("TblPropSet");
	DCini::ProcIniGroup("TblAttrSet");
	DCini::ProcIniGroup("TblMacroSet");

	if (HXMwr::DITA)
		DCini::ProcIniSect("DITATables");

	if (HXMwr::HXML) {
		CTbl.dtpar = HXMwr::DITATableParents;
		DCini::ProcIniGroup("TblParentSet");
	}

	// set marker overrides
	DCini::TblSetList.all(DCini::GetTblSettings);
	*Tbl = CTbl;

	if ((FigTable != Tbl)
	 && !Tbl->macr
	 && !StripTable
	 && HXMwr::DITA
	 && (Tbl->dtname
	  || ((Tbl->dtname = HXMwr::DefDITATbl) != NULL))
	 && !stricmp(Tbl->dtname, "figure")) {
		FigTable = Tbl;
		Tbl = NULL;
		InTbl = false;
		CurrList = savedlist;
		Last.list = savedlast;
		return;
	}

	// update row and cell globals from Tbl

	TblWidType = Tbl->tbsiz;
	RowAttrib = Tbl->attr;
	UseRowColor = Tbl->tburc;
	RowStartMacro = Tbl->macrs;
	RowEndMacro = Tbl->macre;
	CellAttrib = Tbl->attc;
	CellStartMacro = Tbl->maccs;
	CellEndMacro = Tbl->macce;

	// before acting on settings
	long tableStartLevel = 0;
	bool strip = false;
	bool origstrip = false;

	if (FigureDiv)
		HXMwr::CloseFigureDiv(wf);

	if (HXMwr::HXML) {
		IntTbCaption = true;
		HeadFootBodyTags = true;

		if (HXMwr::DITA
		 && (Tbl->dtname
		  || ((Tbl->dtname = HXMwr::DefDITATbl) != NULL))
		 && !stricmp(Tbl->dtname, "strip")) {
			strip = true;
			origstrip = StripTable;
			StripTable = true;
#if 0
			tableStartLevel = CurrLevel + 1;  // allow for wrapper
			if (CurrLevel == 1)
				tableStartLevel++;  // allow for addition of body
#endif
		}

		if (!(Tbl->macr
		 || StripTable))
			HXMwr::SetDITAStart(wf);
	}

	if (Tbl->macb)
		DCmacr::WriteMacro(wf, Tbl->macb);

	if (Tbl->macr)
		DCmacr::WriteMacro(wf, Tbl->macr);
	else if (StripTable) {
		SetCondTable(wf);
		if (IntTbCaption
		 && Tbl->titlecont
		 && strip)
			WriteTbTitle(wf);
		Row = NULL;
		NextRow = (row *) ((Tbl->rowlist)->find(1));
		for (RowNum = 1; RowNum <= Tbl->tbrcount; RowNum++) {
			LastRow = Row;
			if ((Row = NextRow) == NULL)
				break;
			NextRow = (row *) ((Tbl->rowlist)->find(RowNum + 1));
			WriteTbRow(wf);
		}
	}
	else {

		if (Tbl->tbind > 0)
			WriteSpacer(wf, TblSpacerWide = Tbl->tbind);
		else if ((Tbl->tbind == -1)
		 && Tbl->tbllind)
			WriteSpacer(wf, TblSpacerWide = PixelsFromTwips(Tbl->tbllind));

		if (!IntTbCaption
		 && (Tbl->titleplace == 1)) {
			WriteTbTitle(wf);
			if (TblSpacerWide)
				WriteSpacer(wf, TblSpacerWide);
		}

		if (IntTbCaption
		 && !Tbl->titlecont
		 && UseInformaltableTag)
			UsingInformaltableTag = true;
		else
			UsingInformaltableTag = false;

		SetCondFlags(wf);

		if (HXMwr::DITA) {
			if (!Tbl->dtname)
				Tbl->dtname = HXMwr::DefDITATbl;
			if (!stricmp(Tbl->dtname, "figure")) {
				PreTblFigureDiv = HXMwr::PreDITADiv;
				long sdiv = wf->GetDiv();
				TblFigureDiv = wf->NewDiv();
				HXMwr::PreDITADiv = TblFigureDiv;
				wf->SelectDiv(sdiv);
				HXMwr::SetDITATable("simple");
			}
			else {
				HXMwr::SetDITATable(Tbl->dtname);
			}
			if ((HXMwr::CurrDITATbl->ccmax > 0)  // if wrong col count, punt
			 && (HXMwr::CurrDITATbl->ccmax != Tbl->tbccount))
				HXMwr::SetDITATable("table");

			if (!stricmp(HXMwr::CurrDITATbl->ttyp, "fig"))
				HXMwr::DITAElemTypeList.replace(4, Tbl->tblpref);

			HXMwr::WriteDITAStartTbl(wf, HXMwr::CurrDITATbl->ttyp, Tbl->dtpar);
			Tbl->tblid = HXMwr::DITATableIDList.find(Tbl->tblpref);
			wf->strAttr(HXMwr::IDattr, HXMwr::CurrDITATblID = Tbl->tblid);
			tableStartLevel = HXMwr::CurrLevel;
			HXMwr::SetDITADivOff(wf);
		}
		else if (HXMwr::HXML) {
			HXMwr::WriteDITAStartTbl(wf, UsingInformaltableTag ? "informaltable" : "table", Tbl->dtpar);
			wf->strAttr(HXMwr::IDattr, HXMwr::CurrDITATblID = Tbl->tblid);
			tableStartLevel = HXMwr::CurrLevel;
			HXMwr::SetDITADivOff(wf);
		}
		else {
			wf->putEnd();
			wf->startBlock(UsingInformaltableTag ? "informaltable" : "table");
		}

		StartCondTable(wf);

		if (WriteClassAttributes) {
			if (Tbl->cssclass)
				wf->setClass(Tbl->cssclass);
			else {
				char *cn = MakeID(Tbl->tblfname, NULL, 0, ClassSpaceChar,
													false, true, LowerCaseCSS);
				wf->setClass(cn);
				DeleteName(cn);
			}
		}

		if (((XrefIDRef = XrefParaList.find(Tbl->titleprn)) == NULL)
		 || (*XrefIDRef == '\0'))
			XrefIDRef = "0";

		if (Tbl->attrlist)
			(Tbl->attrlist)->writeall(wf, WriteAttrListItem);

		if (Tbl->att)
			wf->putAttrStr(DCmacr::GetMacro(Tbl->att));
		else {
			if (TableAttributes && !HXMwr::HXML) {
				wf->numAttr("border", TblBorderThick);
				wf->numAttr("cellpadding", TblCellPadding);
				wf->numAttr("cellspacing", TblCellSpacing);
			}
			WriteAttrib(wf, "table");
		}
		wf->endElem(true);
		wf->putEnd();

		if (IntTbCaption
		 && Tbl->titlecont)
			WriteTbTitle(wf);

		if ((TblWidType == 'P')  // needed for percent or fixed
		 || (TblWidType == 'F')) {
			col *tcol;
			TblWid = 0;
			for (ColNum = 1; ColNum <= Tbl->tbccount; ColNum++) {
				if (tcol = (col *) (Tbl->collist)->find(ColNum)) {
					if (!TableWidthType)  // fixed values
						TblWid += tcol->colwid;
					else  // percentages
						TblWid += tcol->colpct;
				}
			}
		}

		DCmacr::WriteMacro(wf, Tbl->macs);

		if (HXMwr::DITA) { // also write desc ****
			if (HXMwr::CurrDITATbl->tgroup) {
				HXMwr::SetDITAStart(wf);
				//wf->putEnd();
				HXMwr::WriteDITAStartTbl(wf, HXMwr::CurrDITATbl->tgroup);
				wf->numAttr("cols", Tbl->tbccount);
				wf->endElem(true);
				wf->putEnd();
				HXMwr::SetDITADivOff(wf);
			}

			if (HXMwr::CurrDITATbl->tcspec) {
				col *tcol;
				for (ColNum = 1; ColNum <= Tbl->tbccount; ColNum++) {
					if (tcol = (col *) (Tbl->collist)->find(ColNum)) {
						HXMwr::WriteDITAStartTbl(wf, HXMwr::CurrDITATbl->tcspec);
						if (HXMwr::CurrDITATbl->csiatt)
							wf->numAttr(HXMwr::CurrDITATbl->csiatt, ColNum);
						if (HXMwr::CurrDITATbl->csnatt) {
							char *px = NULL;
							if ((px = HXMwr::CurrDITATbl->csnpx) == NULL)
								px = "col";
							tcol->dtname = NewName(px, strlen(px) + NumStrMax + 1);
							_ltoa((long) ColNum, tcol->dtname + strlen(px), 10);
							wf->strAttr(HXMwr::CurrDITATbl->csnatt, tcol->dtname);
						}
						if (HXMwr::CurrDITATbl->cswatt) {
							if (!TableWidthType)  // fixed values
								//wf->starAttr(HXMwr::CurrDITATbl->cswatt, (tcol->colwid) / 20);
								wf->numAttr(HXMwr::CurrDITATbl->cswatt, (tcol->colwid) / 20, "pt");
							else  // percentages
								wf->starAttr(HXMwr::CurrDITATbl->cswatt, tcol->colpct);
						}
						HXMwr::WriteDITAFinishTag(wf, HXMwr::CurrDITATbl->tcspec);
						wf->putEnd();
					}
				}
			}
		}
		else if (CALS) {
			col *tcol;
			if (HXMwr::HXML) {
				HXMwr::SetDITAStart(wf);
				HXMwr::WriteDITAStartTbl(wf, "tgroup");
			}
			else
				wf->startBlock("tgroup");
			wf->numAttr("cols", Tbl->tbccount);
			wf->endElem(true);
			wf->putEnd();
			if (HXMwr::HXML)
				HXMwr::SetDITADivOff(wf);

			for (ColNum = 1; ColNum <= Tbl->tbccount; ColNum++) {
				if (tcol = (col *) (Tbl->collist)->find(ColNum)) {
					if (HXMwr::HXML) {
						HXMwr::SetDITAStart(wf);
						HXMwr::WriteDITAStartTbl(wf, "colspec");
					}
					else
						wf->startBlock("colspec");
					wf->numAttr("colnum", ColNum);
					tcol->dtname = NewName("col", NumStrMax + 4);
					_ltoa((long) ColNum, tcol->dtname + 3, 10);
					wf->strAttr("colname", tcol->dtname);
					//wf->starAttr("colwidth", (tcol->colwid) / 20);
					if (!TableWidthType)  // fixed values
						//wf->starAttr("colwidth", (tcol->colwid) / 20);
						wf->numAttr("colwidth", (tcol->colwid) / 20, "pt");
					else  // percentages
						wf->starAttr("colwidth", tcol->colpct);
					if (HXMwr::HXML)
						HXMwr::WriteDITAFinishTag(wf, "colspec");
					else
						wf->endElem();
					wf->putEnd();
					if (HXMwr::HXML)
						HXMwr::SetDITADivOff(wf);
				}
			}

			// add spanspec ****
		}

		sprintf(TableID, "t%u", ++TableIDVal);

		if (ColIDs)
			ColHdr = new short[Tbl->tbccount + 1];

		if (ColGroupElements)
			ColGrpStart = new bool[Tbl->tbccount + 1];

		if (ColGroupIDs)
			ColGrp = new short[Tbl->tbccount + 1];

		bool inhead = false;
		bool inbody = false;
		bool infoot = false;

		long prevdiv = wf->GetDiv();
		long headdiv = 0;
		long bodydiv = 0;
		long footdiv = 0;

		uns firstbody = 1 + HeaderRows;
		uns lastbody = Tbl->tbrcount - FooterRows;

		DCvlist *theadrows = new DCvlist;
		DCvlist *tbodyrows = new DCvlist;
		DCvlist *tfootrows = new DCvlist;
		DCilist *theadrownums = new DCilist;
		DCilist *tbodyrownums = new DCilist;
		DCilist *tfootrownums = new DCilist;
		DCvlist *theadlastrows = new DCvlist;
		DCvlist *tbodylastrows = new DCvlist;
		DCvlist *tfootlastrows = new DCvlist;
		DCvlist *theadnextrows = new DCvlist;
		DCvlist *tbodynextrows = new DCvlist;
		DCvlist *tfootnextrows = new DCvlist;
		long headcount = 0;
		long bodycount = 0;
		long footcount = 0;
		long rnum = 0;

		Row = NULL;
		NextRow = (row *) ((Tbl->rowlist)->find(1));
		for (RowNum = 1; RowNum <= Tbl->tbrcount; RowNum++) {
			LastRow = Row;
			if ((Row = NextRow) == NULL)
				break;
			NextRow = (row *) ((Tbl->rowlist)->find(RowNum + 1));

			if (HeadFootBodyTags || ColGroupElements) {
				if (((RowNum < firstbody)
				  || (Row->rtype == 2))
				 && !infoot
				 && !inbody
				 && !inhead) {              // start head
					theadrows->add(Row, ++headcount);
					theadrownums->append(RowNum, headcount);
					theadlastrows->add(LastRow, headcount);
					theadnextrows->add(NextRow, headcount);
				}
				else if ((RowNum >= firstbody)
				 && ((Row->rtype == 1)
				  || HXMwr::DITA)
				 && !infoot
				 && !inbody) {              // start body
					tbodyrows->add(Row, ++bodycount);
					tbodyrownums->append(RowNum, bodycount);
					tbodylastrows->add(LastRow, bodycount);
					tbodynextrows->add(NextRow, bodycount);
				}
				else if (((RowNum > lastbody)
				  || (Row->rtype == 3))
				 && !HXMwr::DITA
				 && !infoot) {              // start foot
					tfootrows->add(Row, ++footcount);
					tfootrownums->append(RowNum, footcount);
					tfootlastrows->add(LastRow, footcount);
					tfootnextrows->add(NextRow, footcount);
				}
			}
			else  // no packaging, just write rows as-is
				WriteTbRow(wf);
		}

		if (HeadFootBodyTags || ColGroupElements) {
			if (headcount) {
				if (HXMwr::DITA) {
					if (HXMwr::CurrDITATbl->hgroup) {
						HXMwr::SetDITAStart(wf);
						HXMwr::WriteDITAStartTbl(wf, HXMwr::CurrDITATbl->hgroup);
						wf->endElem(true);
						wf->putEnd();
						HXMwr::SetDITADivOff(wf);
					}
				}
				else if (HXMwr::HXML) {
					HXMwr::SetDITAStart(wf);
					HXMwr::WriteDITAStartTbl(wf, "thead");
					wf->endElem(true);
					wf->putEnd();
					HXMwr::SetDITADivOff(wf);
				}
				else {
					headdiv = wf->NewDiv();
					TblDivType = tHeadType;
				}
				inhead = true;
				for (rnum = 1; rnum <= headcount; rnum++) {
					Row = (row *) theadrows->find(rnum);
					RowNum = (uns) theadrownums->find(rnum);
					LastRow = (row *) theadlastrows->find(rnum);
					NextRow = (row *) theadnextrows->find(rnum);
					WriteTbRow(wf);
				}
				if (HXMwr::DITA) {
					if (HXMwr::CurrDITATbl->hgroup) {
						HXMwr::SetDITAEnd(wf);
						wf->endBlock(HXMwr::CurrDITATbl->hgroup);
						wf->putEnd();
						HXMwr::SetDITADivOff(wf);
					}
				}
				else if (HXMwr::HXML) {
					HXMwr::SetDITAEnd(wf);
					wf->endBlock("thead");
					wf->putEnd();
					HXMwr::SetDITADivOff(wf);
				}
				inhead = false;
			}

			if (footcount) {  // no footers in DITA, until we added them
				if (HXMwr::DITA) {
					if (HXMwr::CurrDITATbl->fgroup) {
						HXMwr::SetDITAStart(wf);
						HXMwr::WriteDITAStartTbl(wf, HXMwr::CurrDITATbl->fgroup);
						wf->endElem(true);
						wf->putEnd();
						HXMwr::SetDITADivOff(wf);
					}
				}
				else if (HXMwr::HXML) {
					HXMwr::SetDITAStart(wf);
					HXMwr::WriteDITAStartTbl(wf, "tfoot");
					wf->endElem(true);
					wf->putEnd();
					HXMwr::SetDITADivOff(wf);
				}
				else {
					footdiv = wf->NewDiv();
					TblDivType = tFootType;
				}
				infoot = true;
				for (rnum = 1; rnum <= footcount; rnum++) {
					Row = (row *) tfootrows->find(rnum);
					RowNum = (uns) tfootrownums->find(rnum);
					LastRow = (row *) tfootlastrows->find(rnum);
					NextRow = (row *) tfootnextrows->find(rnum);
					WriteTbRow(wf);
				}
				if (HXMwr::DITA) {
					if (HXMwr::CurrDITATbl->fgroup) {
						HXMwr::SetDITAEnd(wf);
						wf->endBlock(HXMwr::CurrDITATbl->fgroup);
						wf->putEnd();
						HXMwr::SetDITADivOff(wf);
					}
				}
				else if (HXMwr::HXML) {
					HXMwr::SetDITAEnd(wf);
					wf->endBlock("tfoot");
					wf->putEnd();
					HXMwr::SetDITADivOff(wf);
				}
			}

			if (bodycount) {
				if (HXMwr::DITA) {
					if (HXMwr::CurrDITATbl->bgroup) {
						HXMwr::SetDITAStart(wf);
						HXMwr::WriteDITAStartTbl(wf, HXMwr::CurrDITATbl->bgroup);
						wf->endElem(true);
						wf->putEnd();
						HXMwr::SetDITADivOff(wf);
					}
				}
				else if (HXMwr::HXML) {
					HXMwr::SetDITAStart(wf);
					HXMwr::WriteDITAStartTbl(wf, "tbody");
					wf->endElem(true);
					wf->putEnd();
					HXMwr::SetDITADivOff(wf);
				}
				else {
					bodydiv = wf->NewDiv();
					TblDivType = tBodyType;
				}
				inbody = true;
				for (rnum = 1; rnum <= bodycount; rnum++) {
					Row = (row *) tbodyrows->find(rnum);
					RowNum = (uns) tbodyrownums->find(rnum);
					LastRow = (row *) tbodylastrows->find(rnum);
					NextRow = (row *) tbodynextrows->find(rnum);
					if (Row->startgroup
					 && HeadFootBodyTags
					 && !(HXMwr::DITA || HXMwr::HXML)
					 && (rnum > 1)) {                // new body rowgroup
						wf->endBlock("tbody");
						wf->putEnd();
						wf->putEnd();
						wf->startBlock("tbody");
						if (Tbl->attb) {
							wf->putChar(' ');
							DCmacr::WriteMacro(wf, Tbl->attb);
						}
						wf->endElem(true);
						wf->putEnd();
					}
					WriteTbRow(wf);
				}
				if (HXMwr::DITA) {
					if (HXMwr::CurrDITATbl->bgroup) {
						HXMwr::SetDITAEnd(wf);
						wf->endBlock(HXMwr::CurrDITATbl->bgroup);
						wf->putEnd();
						HXMwr::SetDITADivOff(wf);
					}
				}
				else if (HXMwr::HXML) {
					HXMwr::SetDITAEnd(wf);
					wf->endBlock(inbody ? "tbody" : (inhead ? "thead" : "tfoot"));
					wf->putEnd();
					HXMwr::SetDITADivOff(wf);
				}
				else if (HeadFootBodyTags || ColGroupElements) {
					wf->SelectDiv(prevdiv);  // close last div
					TblDivType = 0;
				}
			}
		}

		if (!HXMwr::DITA
		 && !HXMwr::HXML
		 && (HeadFootBodyTags || ColGroupElements)) {  // process divs 
			wf->SelectDiv(prevdiv);  // close last div
			TblDivType = 0;

			// put out colgroups after getting info from cells
			if (ColGroupElements
			 && (Tbl->tbccount > 1)) {
				wf->putEnd();
				wf->startBlock("colgroup");
				uns cstart = 1;
				uns col = 2;
				for (; col <= Tbl->tbccount; col++) {
					if (ColGrpStart[col]) {
						wf->numAttr("span", col - cstart);
						cstart = col;
						wf->endElem(true);
						wf->endBlock("colgroup");
						wf->putEnd();
						wf->startBlock("colgroup");
					}
				}
				wf->numAttr("span", col - cstart);
				wf->endElem(true);
				wf->endBlock("colgroup");
				wf->putEnd();
			}

			WriteTblDiv(wf, headdiv, "thead", Tbl->atth, tHeadType);
			if (HeadFootBodyTags
			 && !FootTagLast)
				WriteTblDiv(wf, footdiv, "tfoot", Tbl->attf, tFootType);
			WriteTblDiv(wf, bodydiv, "tbody", Tbl->attb, tBodyType);
			if (!HeadFootBodyTags
			 || FootTagLast)
				WriteTblDiv(wf, footdiv, "tfoot", Tbl->attf, tFootType);

			TblDivRefList.empty();
		}

		if (ColHdr) {
			delete [] ColHdr;
			ColHdr = NULL;
		}

		if (ColGroupElements) {
			delete [] ColGrpStart;
			ColGrpStart = NULL;
		}

		if (ColGroupIDs) {
			delete [] ColGrp;
			ColGrp = NULL;
		}

		if (CellSpan) {
			CellSpan = 0;
			ColSpanList.empty();
			RowSpanList.empty();
		}

		if (HXMwr::DITA) {
			if (HXMwr::CurrDITATbl->tgroup) {
				HXMwr::SetDITAEnd(wf);
				wf->endBlock(HXMwr::CurrDITATbl->tgroup);
				wf->putEnd();
				HXMwr::SetDITADivOff(wf);
			}
		}
		else if (CALS) {
			if (HXMwr::HXML)
				HXMwr::SetDITAEnd(wf);
			wf->endBlock("tgroup");
			wf->putEnd();
			if (HXMwr::HXML)
				HXMwr::SetDITADivOff(wf);
		}

		if (HXMwr::HXML)
			HXMwr::SetDITAEnd(wf);

		DCmacr::WriteMacro(wf, Tbl->mace);

		if (HXMwr::DITA)
			wf->endBlock(HXMwr::CurrDITATbl->ttyp);
		else
			wf->endBlock(UsingInformaltableTag ? "informaltable" : "table");
		wf->putEnd();
		wf->putEnd();
		if (TbFootDiv)
			FinishFtns(wf, false, true);

		if (!IntTbCaption
		 && (Tbl->titleplace == 2)) {
			if (TblSpacerWide)
				WriteSpacer(wf, TblSpacerWide);
			WriteTbTitle(wf);
		}
	}

	SetCondFlags(wf);

	DCmacr::WriteMacro(wf, Tbl->maca);

	if (HXMwr::HXML
	 && !StripTable) {
		HXMwr::SetDITADivOff(wf);
		if (tableStartLevel)
			HXMwr::WriteDITAEndTbl(wf, tableStartLevel - 1);
		HXMwr::CurrDITATblID = NULL;
		if (TblFigureDiv) {
			FigureDiv = TblFigureDiv;
			HXMwr::PreDITADiv = PreTblFigureDiv;
			wf->SelectDiv(PreTblFigureDiv);
			TblFigureDiv = 0;
		}
		if (FigTable == Tbl)
			FigTable = NULL;
	}

	if (strip) {
		StripTable = origstrip;
		if (CloseStrippedTables
		 && tableStartLevel)
			HXMwr::WriteDITAEndTbl(wf, tableStartLevel - 1);
	}

	TableConds = NULL;
	CurrConds = TableEndConds;
	TableEndConds = NULL;
	CellRowStrad = NULL;
	CurrRowStrad = NULL;
  delete Tbl;
	Tbl = NULL;
	HXMwr::CurrDITATblID = NULL;
  InTbl = false;
	CurrList = savedlist;
	Last.list = savedlast;
}


void
HTMwr::WriteTblDiv(HTMwfile *wf, long div, char *name, char *attr, long dtype)
{
	if (StripTable || HXMwr::HXML)
		return;
	if (div) {
		long prevdiv = wf->GetDiv();
		wf->SelectDiv(div);
		size_t len = wf->LenDiv();
		if (len != 0) {
			if (HeadFootBodyTags) {
				wf->SelectDiv(prevdiv);
				wf->startBlock(name);
				if (attr) {
					wf->putChar(' ');
					DCmacr::WriteMacro(wf, attr);
				}
				wf->endElem(true);
				wf->putEnd();
			}
			TblDivRefList.write(dtype, wf, FixOneDivRef);
			wf->WriteDiv(div);
			if (HeadFootBodyTags) {
				wf->SelectDiv(prevdiv);
				wf->endBlock(name);
				wf->putEnd();
				wf->putEnd();
			}
		}
		wf->DeleteDiv(div);
		wf->SelectDiv(prevdiv);
	}
}


void
HTMwr::WriteSpacer(HTMwfile *wf, short width, short height)
{
	if (!UseSpacers)
		return;

	if (width < 1)
		return;

	if (height < 1)
		height = 10;

	static char *image = NULL;

	if (!image)
		image = FixGrFileName(PixelSpacerImage);

	wf->startElem("img");
	//wf->strAttr("src", PixelSpacerImage);
	wf->startAttr("src");
	DCmacr::WriteMacro(wf, image);
	wf->endAttr();

	wf->numAttr("width", width);
	wf->numAttr("height", height);
	wf->strAttr("align", "left");
	wf->numAttr("border", 0);
	wf->strAttr("alt", SpacerAlt);
	wf->endElem();
	wf->putEnd();

	SpacerUsed = true;
}


void
HTMwr::WriteSpacerImage(void)
{
	if (!SpacerUsed
	 || !WriteSpacerFile)
		return;

	DCwfile psi(PixelSpacerImage, fbin);
  if (psi.FErr() != fok)
		return;

	psi.putBlock(SpacerImage, SpacerImageSize);
	psi.CloseFile();
}


void
HTMwr::WriteTbTitle(HTMwfile *wf)
{
  if (Tbl->titlecont == NULL)
    return;

	if (HXMwr::DITA
	 && !HXMwr::CurrDITATbl->ttitle)
		return;

  InTbl = false;
  InTbTitle = true;

  Curr.algn = (unc) Tbl->tblalgn;

	if (!StripTable
	 && IntTbCaption
	 && !HXMwr::HXML) {
		wf->startBlock(TblCaptionTag);
		wf->strAttr("align", (Tbl->titleplace == 1) ? "top" : "bottom");
		wf->endElem(true);
	}
  (Tbl->titlecont)->write(*wf);  // para tag is suppressed for DITA
	if (!StripTable
	 && IntTbCaption
	 && !HXMwr::HXML)
		wf->endBlock(TblCaptionTag);

	if (!HXMwr::HXML)
		wf->putEnd();
	wf->putEnd();

  InTbTitle = false;
  InTbl = true;
}


// row processing and writing


row *HTMwr::Row;
row *HTMwr::RowForm;
row *HTMwr::LastRow;
row *HTMwr::NextRow;
uns HTMwr::TbRowType;
uns HTMwr::RowNum;
uns HTMwr::FirstBodyRow;
char *HTMwr::RowAlgns[RowAlgnMax] = { "trql", "trqc", "trqr" };
uns HTMwr::RowColor = 3;     // default white
uns HTMwr::RowFill = 10000;  // tint 100%
long HTMwr::RowCpID = 0;
bool HTMwr::TUseRowColor = false;
bool HTMwr::UseRowColor = false;
bool HTMwr::FirstCol = false;


void
HTMwr::ProcRow(HTMctl *cp)
{
  row *trow;
  DCvlist *trowlist;

  TbRowType = cp->dat();
	if (HXMwr::DITA
	 && (TbRowType == 3)
	 && !HXMwr::UseTableFooterClass)
		TbRowType = 1;

  ColNum = 0;
  LastColwid = 0;

  if (++RowNum > Tbl->tbrcount) {
		if (Tbl->vstrad) {
			cell **vsnew = (cell **) new cell *[RowNum * (Tbl->tbccount + 1)];
			memcpy(vsnew, Tbl->vstrad, (Tbl->tbrcount * (Tbl->tbccount + 1) * sizeof(cell *)));
			delete [] Tbl->vstrad;
			Tbl->vstrad = vsnew;
		}
    Tbl->tbrcount = RowNum;
	}

	if (TablesFile
	 && Tbl->fv
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
			else if (Tbl->brcount > 1)
				trow = (row *) trowlist->find(Tbl->hrcount +
				                     ((Tbl->tbbcount - 1) % Tbl->brcount) + 1);
		}
		if (!InTblDef && !InTDefaults) {
			RowForm = trow;
			Row = new row;
			*Row = *trow;
			Row->rtype = TbRowType;
			Row->celllist = new DCvlist;
			Tbl->rowlist->add(Row, RowNum);

			Row->colstrad = 0;
			if ((RowNum > 1)
			 && Tbl->vstrad) { // top row can't be straddled
				uns rpos = (RowNum - 1) * (Tbl->tbccount + 1);
				cell *cellp = NULL;
				cell *lastcellp = NULL;
				uns col = 0;
				for (col = 1; col < Tbl->tbccount + 1; col++) {
					if (Tbl->vstrad[rpos + col] != NULL)  // is straddled
						Row->colstrad++;
				}
				if (Row->colstrad >= Tbl->tbccount)  { // empty row, fixup cells
					for (col = 1; col < Tbl->tbccount + 1; col++) {
						cellp = Tbl->vstrad[rpos + col];
						if (cellp == lastcellp)
							continue;
						if (--(cellp->rowstrad) == 1)
							cellp->rowstrad = 0;
						lastcellp = cellp;
					}
				}
			}

			if (CellRowStrad == NULL) {
				CellRowStrad = new uns[Tbl->tbccount + 1];
				CurrRowStrad = new uns[Tbl->tbccount + 1];
			}
			for (uns i = 1; i < (Tbl->tbccount + 1); i++) {
				CurrRowStrad[i] = CellRowStrad[i];
			}
		}
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

		Row->colstrad = 0;
		if ((RowNum > 1)
		 && Tbl->vstrad) { // top row can't be straddled
			uns rpos = (RowNum - 1) * (Tbl->tbccount + 1);
			cell *cellp = NULL;
			cell *lastcellp = NULL;
			uns col = 0;
			for (col = 1; col < Tbl->tbccount + 1; col++) {
				if (Tbl->vstrad[rpos + col] != NULL)  // is straddled
					Row->colstrad++;
			}
			if (Row->colstrad >= Tbl->tbccount)  { // empty row, fixup cells
				for (col = 1; col < Tbl->tbccount + 1; col++) {
					cellp = Tbl->vstrad[rpos + col];
					if (cellp == lastcellp)
						continue;
					if (--(cellp->rowstrad) == 1)
						cellp->rowstrad = 0;
					lastcellp = cellp;
				}
			}
		}

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
			Row->celllist = new DCvlist;
			if (Tbl->rowlist == NULL)
				Tbl->rowlist = new DCvlist;
			(Tbl->rowlist)->add(Row, RowNum);
			if (TbRowType == 1) { // body row
				if (FirstBodyRow == 0)
					FirstBodyRow = RowNum;
				else if (UseAltShading && Tbl->altcount && Tbl->normcount
				 && (Tbl->altrowcol == 1)
				 && (((RowNum - FirstBodyRow) %
						 (Tbl->normcount + Tbl->altcount)) >= Tbl->normcount))
					Row->alt = true;
			}
			if (CellRowStrad == NULL) {
				CellRowStrad = new uns[Tbl->tbccount + 1];
				CurrRowStrad = new uns[Tbl->tbccount + 1];
			}
			for (uns i = 1; i < (Tbl->tbccount + 1); i++)
				CurrRowStrad[i] = CellRowStrad[i];
		}
	}
	Row->attrlist = (DCnlist *) RowAttrLists.find((long) cp);
	Row->rowref = (long) cp;
}


void
HTMwr::ProcRowEnd(void)
{
  TbRowType = 0;
}


void
HTMwr::WriteRowAttrListItem(DCwfile *w, char *n, long i)
{
	if (!w || !n || !i
	 || (*n == '\0'))
		return;

	HTMwfile *wf = (HTMwfile *) w;
	wf->putAttrStr(DCmacr::GetMacro(n));

	char *equ = NULL;
	if ((equ = strchr(n, '=')) != NULL) {
		*equ = '\0';
		if (!stricmp(n, "bgcolor"))
			UseRowColor = false;
		*equ = '=';
	}
}


void
HTMwr::WriteTbRow(HTMwfile *wf)
{
	if (Row->rtype == 0) {  // hidden
		Row = LastRow;  // so separators work
		return;
	}
	if (Row->colstrad >= Tbl->tbccount)
		return;  // fully straddled

	char *rowtag = NULL;
	long rowlev = 0;
	bool ditafoot = false;

	if (!StripTable) {
		if (HXMwr::DITA) {
			if ((HXMwr::CurrDITATbl->hrow != NULL)
			 && ((HXMwr::CurrDITATbl->hrmax == 0)
			  || (HXMwr::CurrDITATbl->hrmax >= RowNum))
			 && (Row->rtype == 2)) {
				rowtag = HXMwr::CurrDITATbl->hrow;
				HXMwr::InDITATblHeader = true;
			}
			else if (HXMwr::UseTableFooterClass
			 && (HXMwr::CurrDITATbl->frow != NULL)
			 && ((HXMwr::CurrDITATbl->frmax == 0)
			  || (HXMwr::CurrDITATbl->frmax >= (Tbl->tbrcount - RowNum)))
			 && (Row->rtype == 3)) {
				ditafoot = true;
				rowtag = HXMwr::CurrDITATbl->frow;
				HXMwr::InDITATblHeader = true;
			}
			else {
				rowtag = HXMwr::CurrDITATbl->brow;
				HXMwr::InDITATblHeader = false;
			}
			HXMwr::SetDITAStart(wf);
			HXMwr::WriteDITAStartTbl(wf, rowtag);
			rowlev = HXMwr::CurrLevel;
			UseRowColor = false;
			HXMwr::SetDITADivOff(wf);
		}
		else if (CALS) {
			if (!HXMwr::HXML)
				wf->startBlock(rowtag = "row");
			else {
				HXMwr::SetDITAStart(wf);
				HXMwr::WriteDITAStartTbl(wf, rowtag = "row");
				rowlev = HXMwr::CurrLevel;
				HXMwr::SetDITADivOff(wf);
			}
			UseRowColor = false;
		}
		else if (HXMwr::HXML) {
			HXMwr::SetDITAStart(wf);
			HXMwr::WriteDITAStartTbl(wf, rowtag = "tr");
			rowlev = HXMwr::CurrLevel;
			UseRowColor = false;
			HXMwr::SetDITADivOff(wf);
		}
		else
			wf->startBlock("tr");
		RowColor = 3;    // default white
		RowFill = 10000; // at 100% tint

		StartCondRow(wf);

		if (Row->cssclass)
			wf->setClass(Row->cssclass);

		if (ditafoot
		 && HXMwr::DITATableFooterClass
		 && *HXMwr::DITATableFooterClass)
			wf->strAttr("outputclass", HXMwr::DITATableFooterClass);

		if (Row->attrlist)
			(Row->attrlist)->writeall(wf, WriteRowAttrListItem);
		Row->attrlist = NULL;

		if (UseRowColor) {
			if (Row->alt)
				Row->rowshd = Tbl->tblaltshd;
			if (FMSource) {
				uns colr = Row->alt ? Tbl->altcolr : Row->rowcolr;
				uns fill = Row->alt ? Tbl->altfill : Row->rowfill;
				if ((colr > 1)    // not invisible
				 //&& (colr != 3)  // not white
				 && (fill > 100)) {
					wf->colorAttr("bgcolor", FillColorValue(fill, ColorValue(colr)));
					RowColor = colr;
					RowFill = fill;
				}
			}
			else
				WriteShadingFmt(wf, Row->rowshd);
		}

		if (Row->styleattr) {
			wf->putAttrStr(DCmacr::GetMacro(Row->styleattr));
			Row->styleattr = NULL;
		}

		if (RowAttrib)
			wf->putAttrStr(DCmacr::GetMacro(RowAttrib));
		else
			WriteAttrib(wf, CALS ? "row" : "tr");

		wf->endElem(true);
		wf->SetAttrAdd(false);
	}
  wf->putEnd();
	if (RowStartMacro)
		DCmacr::WriteMacro(wf, RowStartMacro);

  InTbCell = true;
	RowIDSet = false;
	ColGroupNum = 0;

  for (ColNum = 1; ColNum <= Tbl->tbccount; ColNum++) {
    if ((Cell = (cell *) ((Row->celllist)->find(ColNum))) == NULL)
      break;
    WriteCell(wf);
  }
  InTbCell = false;

	if (HXMwr::HXML
	 && !StripTable)
		HXMwr::SetDITAEnd(wf);

	if (RowEndMacro)
		DCmacr::WriteMacro(wf, RowEndMacro);
	if (!StripTable) {
		if (HXMwr::HXML) {
			wf->endBlock(rowtag);
			wf->putEnd();
			HXMwr::SetDITADivOff(wf);
			HXMwr::WriteDITAEndTbl(wf, rowlev - 1);
		}
		else if (CALS) {
			wf->endBlock(rowtag);
			wf->putEnd();
		}
		else {
			wf->endBlock("tr");
			wf->putEnd();
		}
	}
}



// cell processing and writing


cell *HTMwr::Cell = NULL;
bool HTMwr::InTbCell = false;
bool HTMwr::UseTbHeaderCode = false;
uns HTMwr::THeaderCols = 0;
uns HTMwr::HeaderCols = 0;
uns HTMwr::THeaderRows = 0;
uns HTMwr::HeaderRows = 0;
uns HTMwr::TFooterRows = 0;
uns HTMwr::FooterRows = 0;
char *HTMwr::CellFillPats[CellFillMax] = {
  "clbghoriz", "clbgvert", "clbgfdiag", "clbgbdiag",
  "clbgcross", "clbgdcross",
  "clbgdkhor", "clbgdkvert", "clbgdkfdiag", "clbgdkbdiag",
  "clbgdkcross", "clbgdkdcross"
};
char *HTMwr::CellBordStyles[] = {
  "brdrs", "brdrdb", "brdrth"
};
uns HTMwr::CellStradCount = 0;
short HTMwr::StradCellwid = 0;
short HTMwr::LastCellwid = 0;
long HTMwr::CellCpID = 0;


void
HTMwr::SetCellMarg(HTMctl *cp)
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

    default:
      break;
  }
}


void
HTMwr::ProcCell(HTMctl *cp)
{
  InTbCell = true;
  Cell = new cell;
  assert(Row);
  (Row->celllist)->add(Cell, ++ColNum);

	cell *tc = NULL;
	DCvlist *tclist = NULL;

	if (TablesFile
	 && Tbl->fv
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
			else if (Row->bccount > 1)
				tc = (cell *) tclist->find(Row->hccount + 
				                     ((Row->rbcount - 1) % Row->bccount) + 1);
		}
    *Cell = *tc;
	}

  if ((Col = (col *) (Tbl->collist)->find(ColNum)) != NULL)
    LastColwid = Col->colwid;
  LastCellwid += LastColwid;
	if (CurrRowStrad[ColNum]) {
    Cell->straddled = true;
    CellRowStrad[ColNum]--;
	}
  if (CellStradCount) {
    Cell->straddled = true;
    CellStradCount--;
    Cell->cwid = StradCellwid;
  }
  else Cell->cwid = LastCellwid;
	LastCellwid = 0;

	Cell->attrlist = (DCnlist *) CellAttrLists.find((long) cp);
	Cell->colgroup = CellColGroupList.find((long) cp);
	Cell->rowgroup = CellRowGroupList.find((long) cp);
	Cell->span = CellSpanList.find((long) cp);
	Cell->scope = CellScopeList.find((long) cp);
	Cell->id = CellIDList.find((long) cp);

	if (Cell->rowgroup)
		Row->startgroup = true;

	if (NextCellCont) {
		DClist *dl = NextCellCont->next;
		while (dl) {
			AddCellCont((HTMctl *) dl->id);
			dl = dl->next;
		}
		NextCellCont = NULL;
	}
}


void
HTMwr::ProcCellEnd(void)
{
  InTbCell = false;
  ParaEnding = false;
  if (!Cell->brdrl) {
    if ((ColNum == 1) && Tbl->brdrl)
      Cell->brdrl = Tbl->brdrl;
    else if (Col)
      Cell->brdrl = Col->brdrl;
  }
  if (!Cell->brdrr) {
    if ((ColNum == Tbl->tbccount) && Tbl->brdrr)
      Cell->brdrr = Tbl->brdrr;
    else if (Col)
      Cell->brdrr = Col->brdrr;
  }
  if (!Cell->brdrt) {
    if ((RowNum == 1) && Tbl->brdrt)
      Cell->brdrt = Tbl->brdrt;
    else if (Row)
      Cell->brdrt = Row->brdrt;
  }
  if (!Cell->brdrb) {
    if ((RowNum == Tbl->tbrcount) && Tbl->brdrb)
      Cell->brdrb = Tbl->brdrb;
    else if (Row)
      Cell->brdrb = Row->brdrb;
  }

	bool alt = false;
	if (FirstBodyRow
	 && (RowNum >= FirstBodyRow)
	 && ((Row && Row->alt)
	  || (Col && Col->alt)))
		alt = true;

	if (!Cell->cellshd) {
		if (alt && Tbl->tblaltshd)
			Cell->cellshd = Tbl->tblaltshd;
		else if (Row && Row->rowshd)
			Cell->cellshd = Row->rowshd;
		else if (Col && Col->colshd)
			Cell->cellshd = Col->colshd;
	}

	if (Cell->cellshd
	 && (Cell->cellshd->color == Invisible))
		Cell->cellshd->color = 0;

	if (FMSource) {
		if (Cell->cellfill == 0) {
			if (alt && Tbl->altfill)
				Cell->cellfill = Tbl->altfill;
			else if (Row && Row->rowfill)
				Cell->cellfill = Row->rowfill;
			else if (Col && Col->colfill)
				Cell->cellfill = Col->colfill;
		}

		if (Cell->cellcolr == 0) {
			if (alt && Tbl->altcolr)
				Cell->cellcolr = Tbl->altcolr;
			else if (Row && Row->rowcolr)
				Cell->cellcolr = Row->rowcolr;
			else if (Col && Col->colcolr)
				Cell->cellcolr = Col->colcolr;
		}

		if ((Cell->cellfill == CellFillEmpty)
		 || (Cell->cellfill == CellFillWhite))
			Cell->cellfill = 0;
		if (Cell->cellcolr == Invisible)
			Cell->cellcolr = 0;
	}
}


void
HTMwr::ProcCellRowStrad(HTMctl *cp)
{
	uns col = 0;
	uns row = 0;
	uns rpos = 0;

	Cell->rowstrad = (uns) cp->siz();

	if (CellRowStrad == NULL) {
		CellRowStrad = new uns[Tbl->tbccount + 1];
		CurrRowStrad = new uns[Tbl->tbccount + 1];
	}

  CellRowStrad[ColNum] = Cell->rowstrad - 1;
	if (Cell->straddling) {  // both col and row
		for (col = 1; col < Cell->straddling; col++)
			CellRowStrad[ColNum + col] = Cell->rowstrad - 1;
	}

	// mark all cells that are vertically straddled
	if (Tbl->vstrad) {
		for (row = 0; row < Cell->rowstrad - 1; row++) {
			rpos = (RowNum + row) * (Tbl->tbccount + 1);
			Tbl->vstrad[rpos + ColNum] = Cell;
			if (Cell->straddling) {  // both col and row
				for (col = 1; col < Cell->straddling; col++)
					Tbl->vstrad[rpos + ColNum + col] = Cell;
			}
		}
	}
}


void
HTMwr::ProcCellStrad(HTMctl *cp)
{
  col *tcol;

  CellStradCount = (uns) cp->siz();
  if (CellStradCount > 1)
    Cell->straddling = CellStradCount;
  else return;

  StradCellwid = Cell->cwid;  // already includes curr colwid
	CellStradCount--;
  for (uns i = (ColNum + 1); i < (ColNum + CellStradCount + 1); i++) {
    if (tcol = (col *) (Tbl->collist)->find(i))
      LastColwid = tcol->colwid;
    StradCellwid += LastColwid;
  }
  Cell->cwid = StradCellwid;
}


void
HTMwr::StartCellText(HTMctl *cp)
{
	if (InFootnote
	 || XMLSource)
		return;

  Cell->algn = Curr.algn;
  Cell->valgn = Curr.valgn;
  Cell->cellmartl = Curr.cellmartl;
  Cell->cellmargl = Curr.cellmargl;
  if ((!Cell->cellmartl)
   || (Cell->cellmargl == 0))
    Cell->cellmargl += Tbl->tblmargl;
  if (Cell->cellmargl < Row->cmargx)
    Row->cmargx = Cell->cellmargl;

  Cell->cellmartr = Curr.cellmartr;
  Cell->cellmargr = Curr.cellmargr;
  if ((!Cell->cellmartr)
   || (Cell->cellmargr == 0))
    Cell->cellmargr += Tbl->tblmargr;
  if (Cell->cellmargr < Row->cmargx)
    Row->cmargx = Cell->cellmargr;

  Cell->cellmartt = Curr.cellmartt;
  Cell->cellmargt = Curr.cellmargt;
  if ((!Cell->cellmartt)
   || (Cell->cellmargt == 0))
    Cell->cellmargt += Tbl->tblmargt;

  Cell->cellmartb = Curr.cellmartb;
  Cell->cellmargb = Curr.cellmargb;
  if ((!Cell->cellmartb)
   || (Cell->cellmargb == 0))
    Cell->cellmargb += Tbl->tblmargb;
}


void
HTMwr::AddCellCont(HTMctl *cp)
{
  if (Cell) {
    if (!Cell->cont)
      Cell->cont = new HTMgroup;
    (Cell->cont)->add(*cp);
  }
}


DClist *HTMwr::NextCellCont = NULL;

void
HTMwr::ProcNextCellCont(HTMctl *cp)
{
	if (!NextCellCont)
		NextCellCont = new DClist;
  NextCellCont->add((long) cp);
}


void
HTMwr::UseParaForm(HTMctl *cp)
{
	if ((cp->dat() != 1)
	 || (cp->siz() == 0)
	 || !InTbCell
	 || !Cell
	 || InTDefaults
	 || InFootnote
	 || Cell->straddled)
		return;

	pprop	*cform = (pprop *) FormList.find(FormRefList((uns) cp->siz()));
	if (cform == NULL)
		return;

	if (cform->rowattr
	 && (!Row->styleattr))
		Row->styleattr = cform->macrat;

	if (cform->cellattr
	 && (!Cell->styleattr))
		Cell->styleattr = cform->maccat;

	if (cform->cellnocolid)
		Cell->nocolid = true;

	if (cform->cellscope
	 && (!Cell->scope))
		Cell->scope = cform->maccsc;

	if (cform->cellabbr
	 && (!Cell->abbr))
		Cell->abbr = cform->maccab;

	if (cform->cellaxis
	 && (!Cell->axis))
		Cell->axis = cform->maccax;

	if (cform->cellspan)
		Cell->span = true;

	if (cform->cellcolgroup)
		Cell->colgroup = true;
	if (cform->cellrowgroup)
		Cell->rowgroup = Row->startgroup = true;

	Cell->tbhead = cform->tbhead;
	Cell->tbbody = cform->tbbody;
}



// WAI headers support

unc HTMwr::AccessMethod = 'N';
char HTMwr::TableID[NumStrMax + 2] = "";
unl  HTMwr::TableIDVal = 0;

bool HTMwr::TColIDs = false;
bool HTMwr::TRowIDs = false;
bool HTMwr::TColSpanIDs = false;
bool HTMwr::TRowSpanIDs = false;

bool HTMwr::ColIDs = false;
char *HTMwr::ColHead = "col";
bool HTMwr::RowIDs = false;
bool HTMwr::RowIDSet = false;
char *HTMwr::RowHead = "row";
bool HTMwr::ColSpanIDs = false;
char *HTMwr::ColSpanHead = "span";
bool HTMwr::RowSpanIDs = false;
char *HTMwr::RowSpanHead = "span";
bool HTMwr::SpanHeadSpace = false;

bool HTMwr::TColGroupIDs = false;
bool HTMwr::ColGroupIDs = false;
bool HTMwr::TRowGroupIDs = false;
bool HTMwr::RowGroupIDs = false;
char *HTMwr::ColGroupHead = "group";
char *HTMwr::RowGroupHead = "group";
short HTMwr::GroupNum = 0;
short HTMwr::ColGroupNum = 0;
short HTMwr::RowGroupNum = 0;
short *HTMwr::ColGrp = NULL;

bool HTMwr::THeadFootBodyTags = true;
bool HTMwr::HeadFootBodyTags = true;
bool HTMwr::FootTagLast = false;
bool HTMwr::TColGroupElements = false;
bool HTMwr::ColGroupElements = false;
bool *HTMwr::ColGrpStart = NULL;

short *HTMwr::ColHdr = NULL;
short HTMwr::CellSpan = 0;
DCilist HTMwr::ColSpanList;
DCilist HTMwr::RowSpanList;

bool HTMwr::TScopeCol = false;
bool HTMwr::TScopeColGroup = false;
bool HTMwr::TScopeRow = false;
bool HTMwr::TScopeRowGroup = false;

bool HTMwr::ScopeCol = false;
bool HTMwr::ScopeColGroup = false;
bool HTMwr::ScopeRow = false;
bool HTMwr::ScopeRowGroup = false;
bool HTMwr::CellScopeSet = false;


void
HTMwr::PutColSpanHead(DCwfile *dwf, long val)
{
	HTMwfile *wf = (HTMwfile *) dwf;
	if (val) {
		if (SpanHeadSpace)
			wf->putChar(' ');
		else
			SpanHeadSpace = true;
		wf->putStr(ColSpanHead);
		wf->putShort((short) val);
		wf->putStr(TableID);
	}
}


void
HTMwr::PutRowSpanHead(DCwfile *dwf, long val)
{
	HTMwfile *wf = (HTMwfile *) dwf;
	if (val) {
		if (SpanHeadSpace)
			wf->putChar(' ');
		else
			SpanHeadSpace = true;
		wf->putStr(RowSpanHead);
		wf->putShort((short) val);
		wf->putStr(TableID);
	}
}


void
HTMwr::WriteWaiAttributes(HTMwfile *wf, bool head, bool top)
{
	if (StripTable)
		return;

	// add id and headers attribs here
	short i = 0;
	bool started = false;	
	short val = 0;
	bool cellid = false;
	bool scopeid = false;
	bool colspanid = false;
	bool rowspanid = false;
	bool colid = false;
	bool rowid = false;
	bool colgroupid = false;
	bool rowgroupid = false;
	bool left = head && !top;

	if (Cell->id) {  // set by a CellID marker
		wf->strAttr(HXMwr::IDattr, Cell->id);
		cellid = true;
	}

	if (!cellid && top
	 && Cell->colgroup
	 && ScopeColGroup)
		scopeid = true;

	if (!cellid && !scopeid
	 && top
	 && ColGroupIDs
	 && Cell->colgroup) {
		wf->startAttr(HXMwr::IDattr);
		wf->putStr(ColGroupHead);
		wf->putShort(++GroupNum);
		wf->putStr(TableID);
		ColGrp[ColNum] = ColGroupNum = GroupNum;
		wf->endAttr();
		colgroupid = true;
	}

	if (ColGroupIDs) {  // set column group ID
		if (ColGrp[ColNum])
			ColGroupNum = ColGrp[ColNum];
		else
			ColGrp[ColNum] = ColGroupNum;
	}

	if (!cellid && !scopeid && !colgroupid
	 && top
	 && (Cell->span
	  || (ColSpanIDs
	   && Cell->straddling))) {
		wf->startAttr(HXMwr::IDattr);
		wf->putStr(ColSpanHead);
		wf->putShort(++CellSpan);
		wf->putStr(TableID);
		wf->endAttr();
		ColSpanList.append(CellSpan, ColNum);
		if (Cell->straddling)
			for (i = ColNum + 1; i < (ColNum + Cell->straddling); i++)
				ColSpanList.append(CellSpan, i);
		colspanid = true;
	}

	if (!cellid && !scopeid && !colgroupid && !colspanid
	 && top
	 && ColIDs
	 && !ColHdr[ColNum]
	 && !Cell->nocolid) {
		wf->startAttr(HXMwr::IDattr);
		wf->putStr(ColHead);
		wf->putShort(ColNum);
		wf->putStr(TableID);
		ColHdr[ColNum] = ColNum;
		wf->endAttr();
		colid = true;
	}
		
	if (!cellid && !scopeid && !colgroupid && !colspanid && !colid
	 && left
	 && RowGroupIDs
	 && Cell->rowgroup) {
		wf->startAttr(HXMwr::IDattr);
		wf->putStr(RowGroupHead);
		wf->putShort(++GroupNum);
		wf->putStr(TableID);
		wf->endAttr();
		RowGroupNum = GroupNum;
		rowgroupid = true;
	}

	if (!cellid && !scopeid && !colgroupid && !colspanid && !colid && !rowgroupid
	 && left
	 && (Cell->span
	  || (RowSpanIDs
	   && Cell->rowstrad))) {
		wf->startAttr(HXMwr::IDattr);
		wf->putStr(RowSpanHead);
		wf->putShort(++CellSpan);
		wf->putStr(TableID);
		wf->endAttr();
		RowSpanList.append(CellSpan, RowNum);
		if (Cell->rowstrad)
			for (i = RowNum + 1; i < (RowNum + Cell->rowstrad); i++)
				RowSpanList.append(CellSpan, i);
		rowspanid = true;
	}

	if (!cellid && !scopeid && !colgroupid && !colspanid && !colid && !rowgroupid && !rowspanid
	 && RowIDs
	 && !RowIDSet) {
		wf->startAttr(HXMwr::IDattr);
		wf->putStr(RowHead);
		wf->putShort(RowNum);
		wf->putStr(TableID);
		wf->endAttr();
		RowIDSet = true;
		rowid = true;
	}

	if (Cell->scope
	 && head) {
		wf->strAttr("scope", Cell->scope);
		scopeid = true;
	}
	else if (scopeid)
		wf->strAttr("scope", "colgroup");

	if (!scopeid
	 && top
	 && ScopeCol) {
		wf->strAttr("scope", "col");
		scopeid = true;
	}

	if (!scopeid
	 && left
	 && Cell->rowgroup
	 && ScopeRowGroup) {
		wf->strAttr("scope", "rowgroup");
		scopeid = true;
	}
	if (!scopeid
	 && left
	 && ScopeRow) {
		wf->strAttr("scope", "row");
		scopeid = true;
	}

	CellScopeSet = scopeid;

	if ((Cell->abbr)
	 && head) {
		wf->startAttr("abbr");
		wf->putStr(Cell->abbr);
		wf->endAttr();
	}

	if ((Cell->axis)
	 && head) {
		wf->startAttr("axis");
		wf->putStr(Cell->axis);
		wf->endAttr();
	}

	// now do headers

	if (ColGroupNum
	 && ColGroupIDs
	 && !colgroupid) {
		StartHeaderAttrs(wf, &started);
		wf->putStr(ColGroupHead);
		wf->putShort(ColGroupNum);
		wf->putStr(TableID);
	}

	if (!colid
	 && !Cell->nocolid
	 && ColIDs) {
		val = ColHdr[ColNum];
		if (!top && !val) {
			for (i = ColNum - 1; i > 0; i--) {
				if ((val = ColHdr[i]) != 0)
					break;
			}
			ColHdr[ColNum] = val;
		}

		if (val) {
			StartHeaderAttrs(wf, &started);
			wf->putStr(ColHead);
			wf->putShort(val);
			wf->putStr(TableID);
		}
	}

	if (RowGroupNum
	 && RowGroupIDs
	 && !rowgroupid) {
		StartHeaderAttrs(wf, &started);
		wf->putStr(RowGroupHead);
		wf->putShort(RowGroupNum);
		wf->putStr(TableID);
	}

	if (RowIDSet
	 && !rowid
	 && RowIDs) {
		StartHeaderAttrs(wf, &started);
		wf->putStr(RowHead);
		wf->putShort(RowNum);
		wf->putStr(TableID);
	}

	if (!colspanid
	 && ColSpanIDs
	 && (val = (short) ColSpanList.find(ColNum))) {
		StartHeaderAttrs(wf, &started);
		SpanHeadSpace = false;
		ColSpanList.write(ColNum, wf, PutColSpanHead);
	}

	if ((!colspanid && !rowspanid)
	 && RowSpanIDs
	 && (val = (short) RowSpanList.find(RowNum))) {
		StartHeaderAttrs(wf, &started);
		SpanHeadSpace = false;
		RowSpanList.write(RowNum, wf, PutRowSpanHead);
	}

	if (started)
		wf->endAttr();
}


void
HTMwr::StartHeaderAttrs(HTMwfile *wf, bool *pstart)
{
	if (*pstart)
		wf->putChar(' ');
	else {
		wf->startAttr("headers");
		*pstart = true;
	}
}


// cell writing

bool HTMwr::CellAttrAlign = true;
bool HTMwr::CellAttrValign = true;
bool HTMwr::CellAttrWidth = true;
bool HTMwr::CellAttrBgcolor = true;
char *HTMwr::EmptyTbCellContent = "&nbsp;";
char *HTMwr::EmptyParaContent = "&nbsp;";
long HTMwr::CellStartLevel = 0;

void
HTMwr::WriteCellAttrListItem(DCwfile *w, char *n, long i)
{
	if (!w || !n || !i
	 || (*n == '\0'))
		return;

	HTMwfile *wf = (HTMwfile *) w;
	wf->putAttrStr(DCmacr::GetMacro(n));

	char *equ = NULL;
	if ((equ = strchr(n, '=')) != NULL) {
		*equ = '\0';
		if (!stricmp(n, "align"))
			CellAttrAlign = false;
		else if (!stricmp(n, "valign"))
			CellAttrValign = false;
		else if (!stricmp(n, "width"))
			CellAttrWidth = false;
		else if (!stricmp(n, "bgcolor"))
			CellAttrBgcolor = false;
		*equ = '=';
	}
}


void
HTMwr::CheckStradCellContent(HTMctl *cp)
{
	if (!cp
	 || !cp->rp)
		return;

	switch(cp->rp->write_op) {
		case conduse:
			CondStart(cp->dat());
			break;
		case condend:
			CondEnd(cp->dat());
			break;

		default:
			break;
	}
}


void
HTMwr::WriteCell(HTMwfile *wf)
{
  if (Cell->straddled) {
		// get condition start/end info out of cell content
		(Cell->cont)->execu(CheckStradCellContent);
		return;
	}

	bool top = (RowNum < HeaderRows)
	        || (Row->rtype == 2) ? true : false;

	bool head = (top || (ColNum <= HeaderCols)) ? true : false;

	if (Cell->tbhead)
		head = true;
	else if (Cell->tbbody)
		head = false;

	char *cellType = NULL;

	col *tcol = NULL;
	char *celltag = NULL;
	long celllev = 0;
	bool origCellAlignAttributes = CellAlignAttributes;

	if (!StripTable) {
		CellAttrAlign = CellAttrValign = true;

		if (HXMwr::DITA) {
			HXMwr::SetDITAStart(wf);
			if (HXMwr::InDITATblHeader) {
				if (HXMwr::CurrDITATbl->hclist)
					celltag = (HXMwr::CurrDITATbl->hclist)->find(ColNum);
				else if (HXMwr::CurrDITATbl->hcell)
					celltag = HXMwr::CurrDITATbl->hcell;
			}
			else if (HXMwr::CurrDITATbl->bclist)
				celltag = (HXMwr::CurrDITATbl->bclist)->find(ColNum);
			if (celltag == NULL)
				celltag = HXMwr::CurrDITATbl->bcell;
			HXMwr::WriteDITAStartTbl(wf, celltag);
			CellStartLevel = celllev = HXMwr::CurrLevel;
			CellAttrWidth = CellAttrBgcolor = false;
			CellAlignAttributes = CellAlignAttributes && HXMwr::CurrDITATbl->calgnatt;
			HXMwr::SetDITADivOff(wf);
			tcol = (col *) (Tbl->collist)->find(ColNum);
		}
		else if (CALS) {
			if (!HXMwr::HXML)
				wf->startBlock(celltag = "entry");
			else {
				HXMwr::SetDITAStart(wf);
				HXMwr::WriteDITAStartTbl(wf, celltag = "entry");
				CellStartLevel = celllev = HXMwr::CurrLevel;
				HXMwr::SetDITADivOff(wf);
			}
			CellAttrWidth = CellAttrBgcolor = false;
			tcol = (col *) (Tbl->collist)->find(ColNum);
		}
		else if (HXMwr::HXML) {
			HXMwr::SetDITAStart(wf);
			celltag = (UseTbHeaderCode && head) ? "th" : "td";
			HXMwr::WriteDITAStartTbl(wf, celltag);
			CellStartLevel = celllev = HXMwr::CurrLevel;
			CellAttrWidth = CellAttrBgcolor = false;
			HXMwr::SetDITADivOff(wf);
			tcol = (col *) (Tbl->collist)->find(ColNum);
		}
		else {
			cellType = (UseTbHeaderCode && head) ? "th" : "td";
			wf->startBlock(cellType);
			CellAttrWidth = CellAttrBgcolor = true;

			if (Cell->cssclass)
				wf->setClass(Cell->cssclass);

			if (Cell->cont)  // don't write for empty cells
				WriteWaiAttributes(wf, head, top);

			if (ColGroupElements
			 && top
			 && Cell->colgroup)
				ColGrpStart[ColNum] = true;
		}

		if (Cell->attrlist)
			(Cell->attrlist)->writeall(wf, WriteCellAttrListItem);
		Cell->attrlist = NULL;

		if (CellAlignAttributes) {
			if (CellAttrAlign) {
				if (Cell->algn)
					wf->strAttr("align", (Cell->algn == 1) ? "left" : 
					                  ((Cell->algn == 2) ? "center" : "right"));
#if 0
				if (HXMwr::HXML)
					wf->strAttr("align", (Cell->algn == 1) ? "left" : 
					                    ((Cell->algn == 2) ? "center" : "right"));
				else if (!head && (Cell->algn > 1))
					wf->strAttr("align", (Cell->algn == 2) ? "center" : "right");
				else if (head && Cell->algn && (Cell->algn != 2))
					wf->strAttr("align", (Cell->algn == 1) ? "left" : "right");
#endif
			}
			if (Cell->valgn)
				wf->strAttr("valign", (Cell->valgn == 1) ? "middle" : 
					                  ((Cell->valgn == 2) ? "top" : "bottom"));
#if 0
			if (HXMwr::HXML)
				wf->strAttr("valign", (Cell->valgn == 1) ? "middle" : 
					                    ((Cell->valgn == 2) ? "top" : "bottom"));
			else if ((Cell->valgn > 1) && CellAttrValign)
				wf->strAttr("valign", (Cell->valgn == 2) ? "top" : "bottom");
#endif
		}

		if (Cell->straddling > 1) {
			if (HXMwr::DITA) {
				if (!HXMwr::CurrDITATbl->csnmuse
				 && HXMwr::CurrDITATbl->cscatt)
					wf->numAttr(HXMwr::CurrDITATbl->cscatt, Cell->straddling - 1);
				else if (HXMwr::CurrDITATbl->csnmuse
				 && HXMwr::CurrDITATbl->csnsatt
				 && HXMwr::CurrDITATbl->csneatt) {
					wf->strAttr(HXMwr::CurrDITATbl->csnsatt, tcol->dtname);
					col *ecol = (col *) (Tbl->collist)->find(ColNum + Cell->straddling - 1);
					wf->strAttr(HXMwr::CurrDITATbl->csneatt, ecol->dtname);
				}
			}
			else if (CALS) {
				wf->strAttr("namest", tcol->dtname);
				col *ecol = (col *) (Tbl->collist)->find(ColNum + Cell->straddling - 1);
				wf->strAttr("nameend", ecol->dtname);
			}
			else
				wf->numAttr("colspan", Cell->straddling);
		}
		else {
			if (HXMwr::DITA) {
				if (HXMwr::CurrDITATbl->cnmatt)
					wf->strAttr(HXMwr::CurrDITATbl->cnmatt, tcol->dtname);
			}
			else if (CALS)
				wf->strAttr("colname", tcol->dtname);
		}
		if (Cell->rowstrad > 1) {
			uns rval = Cell->rowstrad;
			uns rnum = RowNum + 1;
			uns lnum = RowNum + Cell->rowstrad;
			for ( ; rnum < lnum; rnum++) {  // remove conditioned-out rows
				if (((row *) Tbl->rowlist->find(rnum))->rtype == 0)
					rval--;
			}
			if (rval > 1) {
				if (HXMwr::DITA) {
					if (HXMwr::CurrDITATbl->rsatt)
						wf->numAttr(HXMwr::CurrDITATbl->rsatt, rval - 1);
				}
				else if (CALS)
					wf->numAttr("morerows", rval - 1);
				else
					wf->numAttr("rowspan", rval);
			}
		}

		if (CellAttrWidth) {
			switch (TblWidType) {
				case 'A':  // Adaptive, leave unspecified
					break;
				case 'F':  // Fixed, use DPI
					if ((Cell->straddling == Tbl->tbccount)
					 || (Tbl->tbccount == 1))
						wf->numAttr("width", (TblWid * TblDPI) / 1440);
					else
						wf->numAttr("width", (short) (((long) Cell->cwid * TblDPI) / 1440));
					break;
				case 'P':  // Percent
					if ((Cell->straddling == Tbl->tbccount)
					 || (Tbl->tbccount == 1))
						wf->pctAttr("width", 100);
					else
						wf->pctAttr("width", (short) (((long) Cell->cwid * 100) / TblWid));
					break;
				default:
					break;
			}
		}

		if (CellAttrBgcolor
		 && CellColorAttributes) {
			if (FMSource
			 && (Cell->cellcolr > 1)    // not invisible
			 && ((Cell->cellcolr != RowColor)  // not same as row
				|| (Cell->cellfill != RowFill))
			 && (Cell->cellfill > 100))  // over 1% tint, exclude patterns
				wf->colorAttr("bgcolor", FillColorValue(Cell->cellfill, ColorValue(Cell->cellcolr)));
			else if (Cell->cellshd
			 && (Cell->cellshd->color > 1))    // not invisible
			 //&& (!Row->rowshd  // not same as row
			 // || (Cell->cellshd->color != Row->rowshd->color)
			 // || (Cell->cellshd->tint != Row->rowshd->tint)))
				WriteShadingFmt(wf, Cell->cellshd);
		}

		if (Cell->styleattr) {
			//wf->putChar(' ');
			//DCmacr::WriteMacro(wf, Cell->styleattr);
			wf->putAttrStr(DCmacr::GetMacro(Cell->styleattr));
			Cell->styleattr = NULL;
		}

		// try to use CellScopeSet to block duplication of attr
		if (CellAttrib) {
			//wf->putChar(' ');
			//DCmacr::WriteMacro(wf, CellAttrib);
			wf->putAttrStr(DCmacr::GetMacro(CellAttrib));
		}
		else
			WriteAttrib(wf, cellType);
		wf->endElem(true);
	}

	if (CellStartMacro)
		DCmacr::WriteMacro(wf, CellStartMacro);
 
	if (Cell->cont) {
    // add code to handle cell margins to the extent that they are not
    // handled by row trgaph (which is divided between left and right)
    // Ex: Curr.lind += Cell->cellmargl - (Row->cmargx / 2);
    // restore after cell; adjust .find to match -- else bugs

    // try to keep desired alignment in effect
    unc tempalgn = Curr.algn;
    if (Cell->algn)
      Curr.algn = (unc) Cell->algn; // still hit by format

		CurrList.used = false;
		CurrList.level = 0;
		CurrList.last = 0;
		CurrList.typeval = 0;
		Last.list = 0;

    InTbl = false;
    (Cell->cont)->write(*wf);
    InTbl = true;

    Curr.algn = tempalgn;
		while (CurrList.level)
			PutListEnd(wf, false);

		EndSpecialBlocks(wf, true, false);

		if (FigureDiv)
			HXMwr::CloseFigureDiv(wf);

	}
	else {
		if (!RemEmptyTbParas && !Curr.nopara)
			wf->putStr(EmptyTbCellContent);
	}
	if (HXMwr::HXML
	 && !StripTable)
		HXMwr::SetDITAEnd(wf);
	if (CellEndMacro)
		DCmacr::WriteMacro(wf, CellEndMacro);
	if (!StripTable) {
		if (HXMwr::HXML) {
			wf->endBlock(celltag);
			wf->putEnd();
			HXMwr::SetDITADivOff(wf);
			HXMwr::WriteDITAEndTbl(wf, celllev - 1);
		}
		else if (CALS) {
			wf->endBlock(celltag);
			wf->putEnd();
		}
		else {
			wf->endBlock(cellType);
			wf->putEnd();
		}
		CellAlignAttributes = origCellAlignAttributes;
	}
  ParaEnding = false;
	CellStartLevel = 0;
}



// rule properties


DCvlist HTMwr::RuleFormList;
DCnlist HTMwr::RuleNameList;
brdrinfo *HTMwr::RuleDef = NULL;
brdrinfo HTMwr::NoBrdr;
bool HTMwr::InRuleDef;


void
HTMwr::ProcRuleForm(void)
{
  InRuleDef = true;
	RuleDef = new brdrinfo;
}


void
HTMwr::ProcRuleProp(HTMctl *cp)
{
  if ((cp->maj() != 4)   // graphics
   || (cp->min() != 20)) // line props
    return;

  switch (cp->dat()) {
    case 2:  // color
      GetColor(cp, &RuleDef->color);
      break;
    case 3:  // thickness
      RuleDef->thick = GetTwips(cp->siz(), gl_unit);
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
      break;
    default:
      break;
  }
}


void
HTMwr::StoreRuleForm(void)
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


void
HTMwr::GetRule(HTMctl *cp, brdrinfo **rptr)
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
}



// fill patterns


uns HTMwr::FillMap[FillMapMax] = {  // specific to Frame MIF
  CellFillEmpty, 10000, 9000, 7000,
  5000, 3000, 1000, 500,
  CellFillWhite, 10, 9, 8,
  7, 6, 4, 3
};


void
HTMwr::GetFill(HTMctl *cp, uns *fptr)
{
  if (cp->siz() < FillMapMax)
    *fptr = FillMap[cp->siz()];
}


long
HTMwr::FillColorValue(uns fill, long colr)
{
	if ((colr == 0xFFFFFF)
	 || (fill < 100))
		return 0xFFFFFF; // white is always white

	if (fill == 10000)  // 100% tint
		return colr;

	unl red = ((((unl) colr) >> 16) & 0xFF);
	unl green = ((((unl) colr) >> 8) & 0xFF);
	unl blue = ((unl) colr & 0xFF);

	if (colr == 0)
		red = green = blue = 0xFF - ((0xFF * fill) / 10000);
	else {
		red = 0xFF - (((0xFF - red) * fill) / 10000);
		green = 0xFF - (((0xFF - green) * fill) / 10000);
		blue = 0xFF - (((0xFF - blue) * fill) / 10000);
	}

	return (red << 16) + (green << 8) + blue;
}



// colors


long HTMwr::ColorTable[ColorTableMax] = {
	0, 0x000000, 0xFFFFFF, 0xFF0000, 0x00FF00,
 0x0000FF, 0x00FFFF, 0xFF00FF, 0xFFFF00
};

DCilist HTMwr::ColorDefs;
DCnlist HTMwr::ColorNames;
long HTMwr::ColorsDefined = ColorTableMax;


void
HTMwr::GetColor(HTMctl *cp, long *cptr)
{
	*cptr = (uns) cp->siz() + 1;
}

void
HTMwr::GetColorS(HTMctl *cp, uns *cptr)
{
	*cptr = (uns) cp->siz() + 1;
}



long
HTMwr::ColorValue(long colr)
{
	if (colr == -1)
		return colr;  // transparent
	if (colr == 0)
		return 0xFFFFFF; // white for invisible
	else if (colr <= ColorTableMax)
		return ColorTable[colr - 1];
	else
		return ColorDefs.find(colr);
}



void
HTMwr::ProcColorDef(long cnum, long cval)
{
	if (cnum == 0)
		return;

	if (cnum <= ColorTableMax)    // dcl colors 0..8
		ColorTable[cnum - 1] = cval;
	else { 
		if (cnum > ColorsDefined)
			ColorsDefined = cnum;
		ColorDefs.replace(cval, cnum); // adds if missing
	}
}


long
HTMwr::GetColorNum(long cval)
{
	long cnum = 0;

	if (cval < 0)
		return cval;

	for (long i = 1; i < ColorTableMax; i++) {
		if (cval == ColorTable[i])
			return (i + 1);
	}

	if ((cnum = ColorDefs(cval)) != 0)
		return cnum;

	cnum = ColorsDefined + 1;
	ProcColorDef(cnum, cval);

	return cnum;
}





// end of dwhtmtb.cc


