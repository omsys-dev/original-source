 
// drmiftb.cc is the code for dcl FrameMaker tables reading, JHG, 9/15/93
// Copyright (c) 1993 by Omni Systems, Inc.  All rights reserved.

#include "dcappl.h"
#include "dcctlg.h"
#include "dclist.h"
#include "dcfili.h"
#include "dcini.h"
#include "drmif.h"
#include "drmiffl.h"


mif MIFrd::miftbpile[] = {
  { "RulingCatalog",     newgroup,   group, 0,  5,  0, pseudogroup },
  { "Ruling",            newgroup,   group, 2, 80,  0, rugroup },
  { "RulingTag",         nameval,    ename, 2, 81,  1, runame },
  { "RulingPenWidth",    glineval,   ilong, 4, 20,  3, none },
  { "RulingGap",         glineval,   ilong, 4, 20,  4, none },
  { "RulingSeparation",  separ,      imnem, 4, 20,  2, none },
  { "RulingColor",       nameval,    imnem, 4, 20,  2, rucolor },
  { "RulingPen",         fillval,    imnem, 4, 21,  1, none },
  { "RulingLines",       numval,     ilong, 4, 20,  5, none },

  { "TblCatalog",        newgroup,   group, 0,  6,  0, pseudogroup },
  { "TblFormat",         newgroup,   group, 2, 80,  0, tbform },
  { "TblTag",            nameval,    ename, 2, 81,  1, tbname },
  { "TblColumn",         newgroup,   group, 2, 52,  0, tbcol },
  { "TblColumnNum",      numval,     ilong, 2, 52,  0, tbcoln },
  { "TblColumnWidth",    sizeval,    ilong, 2, 53,  2, tbwcol },
  { "TblColumnH",        newgroup,   group, 2, 54,  2, dclgroup },
  { "TblColumnBody",     newgroup,   group, 2, 54,  1, dclgroup },
  { "TblColumnF",        newgroup,   group, 2, 54,  3, dclgroup },
  { "TblCellMargins",    leadrect,   ilong, 2, 57,  0, tcmarg },

  { "TblLIndent",        sizeval,    ilong, 2, 32,  2, tbplg },
  { "TblRIndent",        sizeval,    ilong, 2, 32,  3, tbplg },
  { "TblAlignment",      l_gralign,  imnem, 2, 31,  0, tbalign },
  { "TblPlacement",      l_tbplace,  imnem, 2, 11,  0, tbplace },
  { "TblSpBefore",       leadval,    ilong, 2, 22,  1, tbplg },
  { "TblSpAfter",        leadval,    ilong, 2, 22,  3, tbplg },
  { "TblBlockSize",      numval,     ilong, 2, 12,  0, tbblock },

  { "TblHFFill",         fillval,    imnem, 2, 55,  8, tbhfprop },
  { "TblHFSeparation",   separ,      imnem, 2, 55,  7, tbhfprop },
  { "TblHFColor",        nameval,    imnem, 2, 55,  7, tbhfcolor },
  { "TblBodyFill",       fillval,    imnem, 2, 55,  8, tbbprop },
  { "TblBodySeparation", separ,      imnem, 2, 55,  7, tbbprop },
  { "TblBodyColor",      nameval,    imnem, 2, 55,  7, tbbcolor },
  { "TblShadeByColumn",  yorn,       imnem, 2, 51,  8, none },
  { "TblShadePeriod",    numval,     ilong, 2, 51,  9, none },
  { "TblXFill",          fillval,    imnem, 2, 51, 12, none },
  { "TblXSeparation",    separ,      imnem, 2, 51, 11, none },
  { "TblXColor",         nameval,    imnem, 2, 51, 11, tbxcolor },
  { "TblAltShadePeriod", numval,     ilong, 2, 51, 10, none },

  { "TblLRuling",        nameval,    imnem, 2, 51,  5, tbrul },
  { "TblRRuling",        nameval,    imnem, 2, 51,  6, tbrul },
  { "TblBRuling",        nameval,    imnem, 2, 51,  4, tbrul },
  { "TblTRuling",        nameval,    imnem, 2, 51,  3, tbrul },
  { "TblColumnRuling",   nameval,    imnem, 2, 53,  6, tbrul },
  { "TblXColumnNum",     numval,     ilong, 0,  0,  0, tbxcnum },
  { "TblXColumnRuling",  nameval,    imnem, 2, 53,  6, tbxcrul },
  { "TblBodyRowRuling",  nameval,    imnem, 2, 55, 10, tbbrul },
  { "TblXRowRuling",     nameval,    imnem, 2, 55, 13, tbbrul },
  { "TblRulingPeriod",   numval,     ilong, 2, 55, 12, tbbprop },
  { "TblHFRowRuling",    nameval,    imnem, 2, 55, 10, tbhfrul },
  { "TblSeparatorRuling", nameval,   imnem, 2, 55, 11, tbhfrul },
  { "TblLastBRuling",    yorn,       imnem, 2, 51, 13, none },

  { "TblTitlePlacement", l_ttplace,  imnem, 2, 58,  1, tbtplace },
  { "TblTitlePgf1",      newgroup,   group, 2, 58,  3, dclgroup },
  { "TblTitleGap",       glineval,   ilong, 2, 58,  2, none },
  { "TblInitNumColumns", numval,     ilong, 2, 53,  1, none },
  { "TblInitNumHRows",   numval,     ilong, 2, 55,  6, tbhprop },
  { "TblInitNumBodyRows", numval,    ilong, 2, 55,  6, tbbprop },
  { "TblInitNumFRows",   numval,     ilong, 2, 55,  6, tbfprop },
  { "TblNumByColumn",    yorn,       imnem, 2, 51,  7, tbanum },

  { "Tbls",              newgroup,   group, 0, 12,  0, pseudogroup },
  { "Tbl",               newgroup,   group, 2, 50,  0, tbggroup },
  { "TblID",             numval,     ilong, 0,  0,  0, tbid },
  { "TblNumColumns",     numval,     ilong, 2, 53,  1, tbncol },
  { "TblTitle",          newgroup,   group, 0,  0,  0, ignore },
  { "TblTitleContent",   newgroup,   group, 2, 58,  3, dclgroup },
  { "TblH",              newgroup,   group, 2, 54,  2, tbrtyp },
  { "TblBody",           newgroup,   group, 2, 54,  1, tbrtyp },
  { "TblF",              newgroup,   group, 2, 54,  3, tbrtyp },

  { "Row",               newgroup,   group, 2, 54,  0, tbrow },
  { "RowWithPrev",       yorn,       imnem, 2, 12,  1, none },
  { "RowWithNext",       yorn,       imnem, 2, 12,  2, none },
  { "RowMinHeight",      sizeval,    ilong, 2, 55,  4, none },
  { "RowMaxHeight",      sizeval,    ilong, 2, 55,  5, none },
  { "RowHeight",         sizeval,    ilong, 2, 55,  2, none },
  { "RowPlacement",      l_pplace,   imnem, 2, 11,  0, pplace },

  { "Cell",              newgroup,   group, 2, 56,  1, tbcell },
  { "CellFill",          fillval,    imnem, 2, 57,  6, none },
  { "CellSeparation",    separ,      imnem, 2, 57,  5, none },
  { "CellColor",         nameval,    imnem, 2, 57,  5, tbccolor },
  { "CellLRuling",       nameval,    imnem, 2, 57,  9, tbrul },
  { "CellRRuling",       nameval,    imnem, 2, 57, 10, tbrul },
  { "CellTRuling",       nameval,    imnem, 2, 57,  7, tbrul },
  { "CellBRuling",       nameval,    imnem, 2, 57,  8, tbrul },
  { "CellColumns",       numval,     ilong, 2, 57,  1, none },
  { "CellRows",          numval,     ilong, 2, 57,  2, none },
  { "CellAngle",         numval,     ilong, 2, 57,  3, tbcang },
  { "CellContent",       newgroup,   group, 2, 56,  1, tbccon },

  { "ATbl",              numval,     imnem, 0,  0,  0, atbl },

  { "PgfCellMargins",    leadrect,   ilong, 2, 57,  0, tcmarg },
  { "PgfCellLMarginFixed", yorn,     imnem, 2, 57, 17, tclmt },
  { "PgfCellRMarginFixed", yorn,     imnem, 2, 57, 18, tcrmt },
  { "PgfCellTMarginFixed", yorn,     imnem, 2, 57, 15, tctmt },
  { "PgfCellBMarginFixed", yorn,     imnem, 2, 57, 16, tcbmt },

  { "PgfCellAlignment",  l_tcalign,  imnem, 2, 57, 19, tcalign }
};

size_t MIFrd::miftb_size = (sizeof(miftbpile) / sizeof(mif));

char *MIFrd::wordtbpile[] = {
  (char *) (int) l_tbplace, "Anywhere", "ColumnTop", "PageTop",
    "LPageTop", "RPageTop", "Float", NULL,
  (char *) (int) l_tcalign, "Middle", "Top", "Bottom", NULL,
  (char *) (int) l_ttplace, "None", "InHeader", "InFooter", NULL,
  NULL
};


// start of code


void
MIFrd::ProcTb(mif *mp)
{
  switch (mp->spec) {

    case rugroup:
      ProcRForm(mp);
      break;
    case runame:
      ProcRName(mp);
      break;
		case rucolor:
      ProcColor();
			if (MIFval)
				MakeCtl(mp);
      break;

    case tbform:
      ProcTForm(mp);
      break;
    case tbname:
      ProcTName(mp);
      break;
    case tbcol:
      ProcTCol(mp);
      break;
    case tbcoln:
      ProcTCNum();
      break;
    case tcmarg:
      ProcTCMarg(mp);
      break;
		case tclmt:
			CurrPgf.cplmtyp = (unc) (MIFval = (MIFval ? 0 : 1));
			if (!TagPF)
	      MakeCtl(mp);
			break;
		case tcrmt:
			CurrPgf.cprmtyp = (unc) (MIFval = (MIFval ? 0 : 1));
			if (!TagPF)
	      MakeCtl(mp);
			break;
		case tctmt:
			CurrPgf.cptmtyp = (unc) (MIFval = (MIFval ? 0 : 1));
			if (!TagPF)
	      MakeCtl(mp);
			break;
		case tcbmt:
			CurrPgf.cpbmtyp = (unc) (MIFval = (MIFval ? 0 : 1));
			if (!TagPF)
	      MakeCtl(mp);
			break;

    case tbalign:
      if (MIFval < 0)
        break;
      ProcTBAlign(mp);
      break;
    case tbplace:
      if ((MIFval < 1) || (MIFval > 4))
        break;
      ProcTBPlace(mp);
      break;
    case tbblock:
      ProcTBBlock(mp);
      break;
    case tbplg:
      ProcTPlaceGroup(mp);
      break;

    case tbwcol:
      if (TbColWidCount == 0)
        MakeCtl(mp);
      else ProcTColWid(mp);
      break;
    case tbncol:
      TbColWidCount = (uns) MIFval;
      MakeCtl(mp);
      break;

    case tbhprop:
      ProcHRowProp(mp);
      break;
    case tbfprop:
      ProcFRowProp(mp);
      break;

    case tbhfrul:
      MIFval = RuleNameList.getnum(MIFstring);
    case tbhfprop:
      ProcHFRowProp(mp);
      break;
    case tbhfcolor:
			ProcColor();
			if (MIFval)
	      ProcHFRowProp(mp);
      break;

    case tbbrul:
      MIFval = RuleNameList.getnum(MIFstring);
    case tbbprop:
      ProcBRowProp(mp);
      break;
    case tbbcolor:
			ProcColor();
			if (MIFval)
	      ProcBRowProp(mp);
      break;

    case tbxcnum:
      TbXCol = (uns) MIFval;
      if (TbXColRule)
	      ProcTXColRule();
      break;
    case tbxcrul:
      TbXColRule = (uns) RuleNameList.getnum(MIFstring);
      if (TbXCol)
	      ProcTXColRule();
      break;
		case tbxcolor:
      ProcColor();
			if (MIFval)
				MakeCtl(mp);
      break;

    case tbtplace:
      if (MIFval < 1)
        break;
      MakeCtl(mp);
      break;
    case tbanum:
      if (MIFval < 0)
        break;
      MakeCtl(mp);
      break;

    case tbggroup:
      ProcTbl(mp);
      break;
    case tbid:
      TbID = (uns) MIFval;
      break;
    case tbrtyp:
      TbSect = mp->dat;
      break;

    case tbrow:
      ProcTbRow(mp);
      break;

    case tbcell:
      ProcTbCell(mp);
      break;

    case tbrul:
      MIFval = RuleNameList.getnum(MIFstring);
      MakeCtl(mp);
      break;
    case tbcang:
      MIFval *= 100;
      MakeCtl(mp);
      break;
    case tbccon:
      break;
		case tbccolor:
      ProcColor();
			if (MIFval)
				MakeCtl(mp);
      break;

    case atbl:
      ProcATbl();
      break;

    case tcalign:
      if (MIFval < 0)
        break;
			CurrPgf.tcalgn = (unc) MIFval;
      MakeCtl(mp);
      break;

    default:
      assert(0);
      break;
  }
}
 


// ruling catalog


DCnlist MIFrd::RuleNameList;


void
MIFrd::ProcRForm(mif *mp)
{
  DCgroup *gp = new DCgroup(DCctl(mp->dtype, mp->maj, mp->min, ++FormatID));
  GroupList[psg_rule]->add(*gp);
  GroupStack[MIFlevel] = gp;
}


void
MIFrd::ProcRName(mif *mp)
{
  char *chp = strcpy(NewName(strlen(MIFstring) + 1), MIFstring);
  RuleNameList.add(chp, FormatID);  // for safer lookup of rules
  FormTypeList.add(4, FormatID);
  MakeCtl(mp);
  DCctl *cp = new DCctl(imnem, 2, 81, 2, 4);
  GroupStack[MIFlevel]->add(*cp);
}



// table format catalog


DCnlist MIFrd::TblNameList;


void
MIFrd::ProcTForm(mif *mp)
{
  DCgroup *gp = NULL;

  if (CurrPG == psg_tbl)  // defining format
    gp = new DCgroup(DCctl(mp->dtype, mp->maj, mp->min, ++FormatID));
  else  // otherwise overriding format within tbl
    gp = new DCgroup(DCctl(group, 2, 51, 14));
  GroupStack[MIFlevel] = gp;

  TPlaceGroup = new DCgroup(DCctl(group, 2, 51, 2));
  TPUsed = false;

  BRowProps = new DCgroup(DCctl(group, 2, 54, 1));
  BPUsed = false;

  HRowProps = new DCgroup(DCctl(group, 2, 54, 2));
  HPUsed = false;

  FRowProps = new DCgroup(DCctl(group, 2, 54, 3));
  FPUsed = false;

  ColPropList = new DCclist;
  TbXCol = TbCols = 0;

  EndProc[MIFlevel] = EndTForm;
}


void
MIFrd::EndTForm(void)
{
  // put out stored row prop groups, then col prop groups
  DCgroup *gp = GroupStack[MIFlevel];

  if (TPUsed)
    gp->add(*TPlaceGroup);
  else delete TPlaceGroup;

  if (HPUsed)
    gp->add(*HRowProps);
  else delete HRowProps;

  if (BPUsed)
    gp->add(*BRowProps);
  else delete BRowProps;

  if (FPUsed)
    gp->add(*FRowProps);
  else delete FRowProps;

  TPlaceGroup = HRowProps = BRowProps = FRowProps = NULL;

  for (uns i = 1; i <= TbCols; i++)
    gp->add(*(ColPropList->find(i)));
  delete ColPropList;

  if (CurrPG == psg_tbl)  // defining format
    GroupList[psg_tbl]->add(*gp);
  else 
    GroupStack[MIFlevel - 1]->add(*gp);
}


void
MIFrd::ProcTName(mif *mp)
{
  DCctl *cp = NULL;

  if (CurrPG == psg_tbl) {  // defining format, get dcl ID from global
    char *chp = strcpy(NewName(strlen(MIFstring) + 1), MIFstring);
    TblNameList.add(chp, FormatID);
    FormTypeList.add(3, FormatID);
    MakeCtl(mp);
    cp = new DCctl(imnem, 2, 81, 2, 3);
    GroupStack[MIFlevel]->add(*cp);
  }
  else {                    // using format, get dcl ID from list
    MIFval = TblNameList(MIFstring);
    if (MIFval)
      cp = new DCctl(imnem, 2, 82, 3, MIFval);
    else {
      cp = new DCctl(ename, 2, 84, 1);
      cp->ex(MIFstring, strlen(MIFstring) + 1);
    }
    GroupStack[MIFlevel]->add(*cp);
  }
}


DCgroup *MIFrd::TPlaceGroup;
bool MIFrd::TPUsed;


void
MIFrd::ProcTPlaceGroup(mif *mp)
{
  DCctl *cp = new DCctl(mp->dtype, mp->maj, mp->min, mp->dat, MIFval);
  TPlaceGroup->add(*cp);
  TPUsed = true;
}


void
MIFrd::ProcTBAlign(mif *mp)
{
  DCctl *cp = new DCctl(mp->dtype, mp->maj, mp->min, MIFval + 1);
  TPlaceGroup->add(*cp);
  TPUsed = true;
}


void
MIFrd::ProcTBPlace(mif *mp)
{
  DCctl *cp = new DCctl(mp->dtype, mp->maj, mp->min, MIFval + 2, 1);
  TPlaceGroup->add(*cp);
  TPUsed = true;
}


void
MIFrd::ProcTBBlock(mif *mp)
{
  DCctl *cp = new DCctl(mp->dtype, mp->maj, mp->min, 4, MIFval);
  TPlaceGroup->add(*cp);
  cp = new DCctl(mp->dtype, mp->maj, mp->min, 5, MIFval);
  TPlaceGroup->add(*cp);
  TPUsed = true;
}


DCclist *MIFrd::ColPropList;
uns MIFrd::TbCols;
uns MIFrd::TbXCol;
uns MIFrd::TbXColRule;


void
MIFrd::ProcTCol(mif *mp)
{
  // set up column property groups, but don't link them in yet
  DCgroup *gp = new DCgroup(DCctl(group, 2, 52, 0));
  GroupStack[MIFlevel] = gp;
}


void
MIFrd::ProcTCNum()
{
  // add the current col property group to the index, and set its dat
  DCgroup *gp = GroupStack[MIFlevel];
  gp->dat(MIFval + 1);
  //assert(MIFval == TbCols);
  ColPropList->add(gp, ++TbCols);
}


void
MIFrd::ProcTXColRule()
{
  DCgroup *gp = (DCgroup *) ColPropList->find(TbXCol);
  if (gp) {
    DCctl *cp = new DCctl(imnem, 2, 53,  6, TbXColRule);
    gp->add(*cp);
  }
	TbXCol = TbXColRule = 0;
}


void
MIFrd::ProcTCMarg(mif *mp)
{
	CurrPgf.cplmval = MIFrect.l;
	CurrPgf.cprmval = MIFrect.w;
	CurrPgf.cptmval = MIFrect.t;
	CurrPgf.cpbmval = MIFrect.h;

	if (TagPF)
		return;

  DCctl *cp = new DCctl(mp->dtype, mp->maj, mp->min, 13, MIFrect.l);
  GroupStack[MIFlevel]->add(*cp);
  cp = new DCctl(mp->dtype, mp->maj, mp->min, 14, MIFrect.w);
  GroupStack[MIFlevel]->add(*cp);
  cp = new DCctl(mp->dtype, mp->maj, mp->min, 11, MIFrect.t);
  GroupStack[MIFlevel]->add(*cp);
  cp = new DCctl(mp->dtype, mp->maj, mp->min, 12, MIFrect.h);
  GroupStack[MIFlevel]->add(*cp);
}


uns MIFrd::TbColWidCount;
DCblock *MIFrd::ColWids;


void
MIFrd::ProcTColWid(mif *mp)
{
  static uns WidPos = 0;

  assert(TbColWidCount);
  if (ColWids == NULL)
    ColWids = new DCblock(TbColWidCount * sizeof(long));
  long *lp = (long *) ColWids->p();
  lp[WidPos++] = MIFval;
  if (--TbColWidCount == 0) {
    DCctl *cp = new DCctl(elong, mp->maj, mp->min, mp->dat);
    *cp = *ColWids;
    GroupStack[MIFlevel]->add(*cp);
    WidPos = 0;
    ColWids = NULL;
  }
}


DCgroup *MIFrd::BRowProps;
DCgroup *MIFrd::HRowProps;
DCgroup *MIFrd::FRowProps;
bool MIFrd::BPUsed;
bool MIFrd::HPUsed;
bool MIFrd::FPUsed;


void
MIFrd::ProcBRowProp(mif *mp)
{
  // add body row prop to its group
  DCctl *cp = new DCctl(mp->dtype, mp->maj, mp->min, mp->dat, MIFval);
  BRowProps->add(*cp);
  BPUsed = true;
}

void
MIFrd::ProcHRowProp(mif *mp)
{
  // add header row prop to its group
  DCctl *cp = new DCctl(mp->dtype, mp->maj, mp->min, mp->dat, MIFval);
  HRowProps->add(*cp);
  HPUsed = true;
}

void
MIFrd::ProcFRowProp(mif *mp)
{
  // add footer row prop to its group
  DCctl *cp = new DCctl(mp->dtype, mp->maj, mp->min, mp->dat, MIFval);
  FRowProps->add(*cp);
  FPUsed = true;
}


void
MIFrd::ProcHFRowProp(mif *mp)
{
  // add header/footer row prop to its groups (separate copies)
  DCctl *cp = new DCctl(mp->dtype, mp->maj, mp->min, mp->dat, MIFval);
  HRowProps->add(*cp);
  cp = new DCctl(*cp);
  FRowProps->add(*cp);
  HPUsed = FPUsed = true;
}



// table instance


DCclist MIFrd::TblList;
uns MIFrd::TbID;
uns MIFrd::TbSect;
uns MIFrd::TbRows;
bool MIFrd::InTbRow = false;
bool MIFrd::InTbCell = false;


void
MIFrd::ProcTbl(mif *mp)
{
  DCgroup *gp = new DCgroup(DCctl(mp->dtype, mp->maj, mp->min, mp->dat));
  TbRows = 0;
  FNType = 1;
  FNoteList = new DCclist;
  OldConds = new DClist;
  GroupStack[MIFlevel] = gp;
	StartingFrame = true;

  EndProc[MIFlevel] = EndTbl;
}


void
MIFrd::EndTbl()
{
  DCgroup *gp = GroupStack[MIFlevel];
  gp->dat(TbRows);
  FNType = 0;
  assert(OldConds);
  ProcCondEnd(0);
  delete OldConds;
	OldConds = NULL;
  delete FNoteList;
  TblList.add(gp, TbID);
	TbID = 0;
	StartingFrame = false;
}


void
MIFrd::ProcTbRow(mif *mp)
{
  MIFval = TbSect;
  TbRows++;
  MakeDclGroup(mp);
  EndProc[MIFlevel] = EndTbRow;
	InTbRow = true;
}


void
MIFrd::EndTbRow()
{
#if 0
  uns i; 
  long cond;
  DCctl *cp = NULL;
  DCgroup *gp = GroupStack[MIFlevel];

  // turn off all row conditionals that are on
  for (i = 1; cond = RowConds->number(i); i++) {
    cp = new DCctl(imnem, 2, 63, i);
    gp->add(*cp);
  }
  delete RowConds;
#else
	InTbRow = false;
  //ProcCondEnd(0);
#endif
}


void
MIFrd::ProcTbCell(mif *mp)
{
  MakeDclGroup(mp);
  EndProc[MIFlevel] = EndTbCell;
	InTbCell = true;
}


void
MIFrd::EndTbCell()
{
	InTbCell = false;
}


void
MIFrd::ProcATbl()
{
	FlushText();
  GroupStack[MIFlevel]->add(*TblList.find(MIFval));

	// process anchored frames in table
	TblAFIndex.execu(MIFval, (void (__cdecl *)(void *)) ProcAFLinks);
}



// end of drmiftb.cc


