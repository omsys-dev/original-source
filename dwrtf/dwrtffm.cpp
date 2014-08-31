// dwrtffm.cpp is the code for RTF format file processing, JHG
// Copyright (c) 2009 by Omni Systems, Inc.  All rights reserved.

#include "dcappl.h"
#include "dcctlg.h"
#include "dcfili.h"
#include "dctxfx.h"
#include "dcfmtr.h"
#include "dcfilp.h"
#include "dcini.h"
#include "dcmacr.h"
#include "dwrtf.h"
#include "dwrtffl.h"



// block and inline format processing


fmtvals *
RTFwr::GetFormatFileInfo(char *fmtname)
{
	// update Curr based on format info, if any

	if (!FmtFile
	 || !FmtFile->CheckFormat(fmtname))
		return NULL;

	// set fmtvals from Curr
	fmtvals *fv = new fmtvals;

	if (MergeCurrFormatProps) {
		// char props
		fv->color = CheckColorNum(Curr.colr);
		fv->bcolor = CheckColorNum(Curr.bcolr);
		fv->font = FontNameList.find(Curr.fnum);
		fv->fsize = Curr.fsize * 10;  // half points
		fv->fstyle = Curr.ital;
		fv->fvar = Curr.scaps;
		fv->fweight = (Curr.bold == 2) ? 4 : 0;
		fv->fkern = Curr.kern;
		fv->tdecor = Curr.line;
		fv->tstrike = Curr.strike;
		fv->ttrans = Curr.caps;

		// para props
		if (Curr.stype == 1) {
			fv->lhigh = Curr.lspace;
			fv->lmode = Curr.lsmode;
			fv->talign = Curr.algn + 1;
			fv->valign = Curr.cellvalgn;
			fv->keep = Curr.keep + (Curr.keepn * 2);
			fv->pbreak = (unc) Curr.pbreak;
			fv->margl = Curr.lind;
			fv->margr = Curr.rind;
			fv->margt = Curr.bspace;
			fv->margb = Curr.aspace;
			fv->tind = Curr.find - Curr.lind;

			// borders and shading?
		}
	}
	else { // initialize as needed
		fv->bcolor = 0xFFFFFF;
		fv->talign = 1;
	}

	// modify vals based on named format
	fv = FmtFile->Format(fmtname, fv);
	if (FormatBaseSet) {  // vals set by first call to Format
		//CssBodyFontTwips = (long) floor(FmtFile->BodyBaseVal + 0.5);
		//CssIndentBaseTwips = (long) floor(FmtFile->IndentBaseVal + 0.5);
		FormatBaseSet = false;
	}

	// reset Curr from format
	Curr.colr = CheckColorValue(fv->color);
	Curr.bcolr = CheckColorValue(fv->bcolor, 9);

	if (fv->font) {
		char *comma = strchr(fv->font, ',');  // truncate CSS list
		if (comma)
			*comma = '\0';
		Curr.fnum = GetFontNum(fv->font, &Curr);
	}
	Curr.fsize = (uns) ((fv->fsize + 5) / 10); // convert to half points

	if (!fv->fstyle
	 && (Curr.stype == 1))
		Curr.ital = 1;
	else
		Curr.ital = fv->fstyle;
	if (!fv->fvar
	 && (Curr.stype == 1))
		Curr.scaps = 1;
	else
		Curr.scaps = fv->fvar;
	switch (fv->fweight) {
		case 0:  // not set
			if (Curr.stype == 2)  // char default is as-is
				break;
		case 1:  // 200 and down, normal
		case 2:
			Curr.bold = 1;
			break;
		default: // 300 and up, bold
			Curr.bold = 2;
			break;
	}
	if (fv->fkern)
		Curr.kern = (short) fv->fkern;
	if (!fv->tdecor
	 && (Curr.stype == 1))
		Curr.line = 1;
	else
		Curr.line = fv->tdecor;
	if (!fv->tstrike
	 && (Curr.stype == 1))
		Curr.strike = 1;
	else
		Curr.strike = fv->tstrike;
	if (!fv->ttrans
	 && (Curr.stype == 1))
		Curr.caps = 1;
	else
		Curr.caps = fv->ttrans;

	if (Curr.stype == 1) {
		Curr.lspace = (short) fv->lhigh;
		Curr.lsmode = fv->lmode;

		if (fv->talign) 
			Curr.algn = fv->talign - 1;
		Curr.cellvalgn = fv->valign;

		Curr.lind = (short) fv->margl;
		Curr.rind = (short) fv->margr;
		Curr.bspace = (short) fv->margt;
		Curr.aspace = (short) fv->margb;
		Curr.find = Curr.lind + (short) fv->tind;

		Curr.keep = (fv->keep & 1) ? 1 : 0;
		Curr.keepn = (fv->keep & 2) ? 1 : 0;
		if (fv->pbreak)
			Curr.pbreak = Curr.pxbreak = (fv->pbreak == 8) ? 0 : fv->pbreak;

		if (fv->section) {
			Curr.section = fv->section;
			if (fv->sectstart)
				Curr.sectstart = true;
			if (fv->sectbreak)
				SectBreakType = SectBreakTypes[fv->sectbreak];
			else
				SectBreakType = NULL;
		}

		if (fv->brdrt) {
			Curr.brdrt = (brdrinfo *) fv->brdrt;
			Curr.brdrt->color = CheckColorValue(fv->brdrt->color);
		}
		if (fv->brdrb) {
			Curr.brdrb = (brdrinfo *) fv->brdrb;
			Curr.brdrb->color = CheckColorValue(fv->brdrb->color);
		}
		if (fv->brdrl) {
			Curr.brdrl = (brdrinfo *) fv->brdrl;
			Curr.brdrl->color = CheckColorValue(fv->brdrl->color);
		}
		if (fv->brdrr) {
			Curr.brdrr = (brdrinfo *) fv->brdrr;
			Curr.brdrr->color = CheckColorValue(fv->brdrr->color);
		}
		if (fv->brdrbar) {
			Curr.brdrbar = (brdrinfo *) fv->brdrbar;
			Curr.brdrbar->color = CheckColorValue(fv->brdrbar->color);
		}
		if (fv->brdrbox) {
			Curr.brdrbox = (brdrinfo *) fv->brdrbox;
			Curr.brdrbox->color = CheckColorValue(fv->brdrbox->color);
		}
		if (fv->brdrbtw) {
			Curr.brdrbtw = (brdrinfo *) fv->brdrbtw;
			Curr.brdrbtw->color = CheckColorValue(fv->brdrbtw->color);
		}

		if (fv->shade) {
			Curr.shading = (shdinfo *) fv->shade;
			Curr.shading->color = CheckColorValue(fv->shade->color);
			Curr.shading->bcolor = CheckColorValue(fv->shade->bcolor, 9);
		}

		if (fv->tabs && fv->tcount) {
			Curr.tabs = fv->tabs;
			Curr.tcount = (uns) fv->tcount;
		}
	}

	if (!Curr.maccb)
		Curr.maccb = fv->before;
	if (!Curr.maccs)
		Curr.maccs = fv->start;
	if (!Curr.macce)
		Curr.macce = fv->end;
	if (!Curr.macca)
		Curr.macca = fv->after;

	Curr.styadd = fv->rtf;

	return fv;
}


uns 
RTFwr::GetFontNum(char *fname, pprop *form)
{
	uns fn = (uns) FontNameList.getitem(fname);
	if (fn) {
		if (form)
			form->fnum = fn;
	}
	else {
		finfo *finf = new finfo;
		finf->fname = NewName(fname);
		finf->ftype = 0;
		finf->fwid = DefFontWidth;
		if (form)
			form->fwid = finf->fwid;
		//finf->ftsym = !_stricmp(finf->fname, "Symbol") ? 1 :
		//						 (!_stricmp(finf->fname, "Wingdings") ? 2 : 0);
		//Curr.fsym = finf->ftsym;
		if (IniFile
		 && IniFile->Section("FontTypes")
		 && IniFile->Find(finf->fname))
			finf->ftype = (uns) IniFile->IntRight();
		if (form)
			form->ding = ((finf->ftype == 5) || (finf->ftype == 6)) ? 1 : 0;
		finf->fenc = ((finf->ftype == 5) || (finf->ftype == 6)) ? 2 : 0;
		finf->fnum = ++FontNum;
		FontList.add(finf, FontNum);
		FontNameList.add(finf->fname, FontNum);
		if (form)
			form->fnum = FontNum;
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
		if (form)
			form->fwid = finf->fwid;
		fn = FontNum;
	}
	return fn;
}


// table format processing

tbfmtvals *RTFwr::DefTable = NULL;
rowfmtvals *RTFwr::DefRow = NULL;
cellfmtvals *RTFwr::DefCell = NULL;
bool RTFwr::DefTblSettings = false;


void 
RTFwr::GetDefTblFormats(void)
{
	if (DefTblSettings)
		return;

	DefTable = TablesFile->TableFormat("Table", NULL);
	DefRow = TablesFile->RowFormat("Row", NULL);
	DefCell = TablesFile->CellFormat("Cell", NULL);
	DefTblSettings = true;
}


tbfmtvals *
RTFwr::GetTblFormatFileInfo(char *tbfmtname)
{
	// update Tbl based on format info, if any
	if (!TablesFile)
		return NULL;

	tbfmtvals *fv = new tbfmtvals;
	if (MergeCurrFormatProps) {
		// set fv from Tbl
		fv->tbalign = Tbl->tblalgn + 1;
		fv->tbbreak = Tbl->tblbrk;
		fv->keep = Tbl->tblkeep;

		fv->margl = Tbl->tbllind;
		fv->margt = Tbl->tblbspc;
		fv->margb = Tbl->tblaspc;

		fv->cellmargl = Tbl->tblmargl;
		fv->cellmargr = Tbl->tblmargr;
		fv->cellmargt = Tbl->tblmargt;
		fv->cellmargb = Tbl->tblmargb;

		// shading
	}

	if (!TablesFile->CheckTblFormat(tbfmtname))
		tbfmtname = "Table";

	fv = TablesFile->TableFormat(tbfmtname, fv);

	// set Tbl from fv
	Tbl->fv = fv;

	if (fv->tbalign)
		Tbl->tblalgn = fv->tbalign - 1;
	Tbl->tblbrk = fv->tbbreak;
	Tbl->tblkeep = fv->keep;

	Tbl->tbllind = (short) fv->margl;
	Tbl->tblbspc = (short) fv->margt;
	Tbl->tblaspc = (short) fv->margb;

	Tbl->tblmargl = (short) fv->cellmargl;
	Tbl->tblmargr = (short) fv->cellmargr;
	Tbl->tblmargt = (short) fv->cellmargt;
	Tbl->tblmargb = (short) fv->cellmargb;

	if (fv->brdrt) {
		Tbl->brdrt = (brdrinfo *) fv->brdrt;
		Tbl->brdrt->color = CheckColorValue(fv->brdrt->color);
	}
	if (fv->brdrb) {
		Tbl->brdrb = (brdrinfo *) fv->brdrb;
		Tbl->brdrb->color = CheckColorValue(fv->brdrb->color);
	}
	if (fv->brdrl) {
		Tbl->brdrl = (brdrinfo *) fv->brdrl;
		Tbl->brdrl->color = CheckColorValue(fv->brdrl->color);
	}
	if (fv->brdrr) {
		Tbl->brdrr = (brdrinfo *) fv->brdrr;
		Tbl->brdrr->color = CheckColorValue(fv->brdrr->color);
	}

	if (fv->colbrdrl) {
		Tbl->brdrcl = (brdrinfo *) fv->colbrdrl;
		Tbl->brdrcl->color = CheckColorValue(fv->colbrdrl->color);
	}
	if (fv->colbrdrr) {
		Tbl->brdrcr = (brdrinfo *) fv->colbrdrr;
		Tbl->brdrcr->color = CheckColorValue(fv->colbrdrr->color);
	}

	// tblbdyfill, tblbdycolr
	if (fv->shade) {
		Tbl->tblbdyshd = (shdinfo *) fv->shade;
		Tbl->tblbdyshd->color = CheckColorValue(fv->shade->color);
		Tbl->tblbdyshd->bcolor = CheckColorValue(fv->shade->bcolor, 9);
	}

	long i = 0;

	// set up col widths
	col *tcol = NULL;
	long lastwid = 0;
	if (fv->cwids) {
		if (!Tbl->tbccount)
			Tbl->tbccount = fv->colcount ? fv->colcount : 2;
		for (i = 1; i <= Tbl->tbccount; i++) {
			if (!Tbl->collist)
				Tbl->collist = new DCvlist;
			if ((tcol = (col *) ((Tbl->collist)->find(i))) == NULL)
				(Tbl->collist)->add((tcol = new col), i);
			if (!tcol->colwid)
				tcol->colwid = (uns) fv->cwids->find(i);
			if (!tcol->colwid)
				tcol->colwid = (uns) lastwid;
			else
				lastwid = tcol->colwid;
		}
	}

	// set up row formats
	rowfmtvals *rfv = NULL;
	long rnum = 0;
	Tbl->hrcount = Tbl->brcount = Tbl->frcount = 0;

	if (fv->hrcount) {
		if (!Tbl->hrfmts)
			Tbl->hrfmts = new DCvlist;
		for (i = 1; i <= fv->hrcount; i++) {
			rfv = GetRowFormatFileInfo(fv->hrows->find(i), ++rnum);
			if (rfv) {
				Tbl->hrfmts->add(rfv, i);
				Tbl->hrcount++;
			}
		}
	}
	if (fv->brcount) {
		if (!Tbl->brfmts)
			Tbl->brfmts = new DCvlist;
		for (i = 1; i <= fv->brcount; i++) {
			rfv = GetRowFormatFileInfo(fv->brows->find(i), ++rnum);
			if (rfv) {
				Tbl->brfmts->add(rfv, i);
				Tbl->brcount++;
			}
		}
	}
	if (fv->frcount) {
		if (!Tbl->frfmts)
			Tbl->frfmts = new DCvlist;
		for (i = 1; i <= fv->frcount; i++) {
			rfv = GetRowFormatFileInfo(fv->frows->find(i), ++rnum);
			if (rfv) {
				Tbl->frfmts->add(rfv, i);
				Tbl->frcount++;
			}
		}
	}

	return fv;
}


rowfmtvals *
RTFwr::GetRowFormatFileInfo(char *rowfmtname, long rnum)
{
	// get row format referenced by Tbl
	if (!TablesFile)
		return NULL;

	if (!Tbl)
		return NULL;

	if (!Tbl->rowlist)
		Tbl->rowlist = new DCvlist;

	rowfmtvals *fv = new rowfmtvals;
	row *tr = (row *) Tbl->rowlist->find(rnum);

	if (tr && MergeCurrFormatProps) {
		fv->rowtype = (unc) tr->rtype;
		if ((tr->rhmin == tr->rhmax)
		 && (tr->rhmin != 0)) {
			fv->rhtype = 3;
			fv->rowhigh = tr->rhmin;
		}
		else if (tr->rhigh) {
			fv->rhtype = 2;
			fv->rowhigh = tr->rhigh;
		}
		else {
			fv->rhtype = 1;
			fv->rowhigh = 0;
		}

		// shading
	}

	if (!TablesFile->CheckTblFormat(rowfmtname))
		rowfmtname = "Row";

	fv = TablesFile->RowFormat(rowfmtname, fv);

	// set row from fv
	if (!tr) {
		tr = new row;
		Tbl->rowlist->add(tr, rnum);
	}
	tr->fv = fv;

	if (fv->rowtype)
		tr->rtype = fv->rowtype;

	switch (fv->rhtype) {
		default:
		case 1:  // adapt
			tr->rhigh = tr->rhmin = tr->rhmax = 0;
			break;

		case 2:  // minimum
			tr->rhigh = (short) fv->rowhigh;
			break;

		case 3:  //fixed
			tr->rhmin = tr->rhmax = (short) fv->rowhigh;
			break;
	}

	if (fv->brdrt) {
		tr->brdrt = (brdrinfo *) fv->brdrt;
		tr->brdrt->color = CheckColorValue(fv->brdrt->color);
	}
	if (fv->brdrb) {
		tr->brdrb = (brdrinfo *) fv->brdrb;
		tr->brdrb->color = CheckColorValue(fv->brdrb->color);
	}
	if (fv->brdrx) {
		tr->brdrx = (brdrinfo *) fv->brdrx;
		tr->brdrx->color = CheckColorValue(fv->brdrx->color);
	}

	// rowfill, rowcolr
	if (fv->shade) {
		tr->rowshd = (shdinfo *) fv->shade;
		tr->rowshd->color = CheckColorValue(fv->shade->color);
		tr->rowshd->bcolor = CheckColorValue(fv->shade->bcolor, 9);
	}

	// set up cell formats
	long i = 0;
	cellfmtvals *cfv = NULL;
	long cnum = 0;
	tr->hccount = tr->bccount = 0;

	if (fv->hccount) {
		if (!tr->hcfmts)
			tr->hcfmts = new DCvlist;
		for (i = 1; i <= fv->hccount; i++) {
			cfv = GetCellFormatFileInfo(fv->hcells->find(i), rnum, ++cnum);
			if (cfv) {
				tr->hcfmts->add(cfv, i);
				tr->hccount++;
			}
		}
	}
	if (fv->bccount) {
		if (!tr->bcfmts)
			tr->bcfmts = new DCvlist;
		for (i = 1; i <= fv->bccount; i++) {
			cfv = GetCellFormatFileInfo(fv->bcells->find(i), rnum, ++cnum);
			if (cfv) {
				tr->bcfmts->add(cfv, i);
				tr->bccount++;
			}
		}
	}

	return fv;
}


cellfmtvals *
RTFwr::GetCellFormatFileInfo(char *cellfmtname, long rnum, long cnum)
{
	// get cell format referenced by row in Tbl
	if (!TablesFile)
		return NULL;

	if (!Tbl)
		return NULL;
	row *tr = (row *) Tbl->rowlist->find(rnum);
	if (!tr)
		return NULL;
	if (!tr->celllist)
		tr->celllist = new DCvlist;

	cellfmtvals *fv = new cellfmtvals;

	cell *tc = (cell *) tr->celllist->find(cnum);

	if (tc && MergeCurrFormatProps) {
		fv->margl = tc->cellmargl;
		fv->margr = tc->cellmargr;
		fv->margt = tc->cellmargt;
		fv->margb = tc->cellmargb;
		fv->valign = tc->cellvalgn;

		// shading
	}
	else if (Tbl) {        // use defaults from table format
		fv->margl = Tbl->tblmargl;
		fv->margr = Tbl->tblmargr;
		fv->margt = Tbl->tblmargt;
		fv->margb = Tbl->tblmargb;
	}

	if (!TablesFile->CheckTblFormat(cellfmtname))
		cellfmtname = "Cell";

	fv = TablesFile->CellFormat(cellfmtname, fv);

	// set cell from fv
	if (!tc) {
		tc = new cell;
		tr->celllist->add(tc, cnum);
	}
	tc->fv = fv;

	// rpos = distance from start of row to right edge

	tc->cellmargl = (uns) fv->margl;
	tc->cellmargr = (uns) fv->margr;
	tc->cellmargt = (uns) fv->margt;
	tc->cellmargb = (uns) fv->margb;
	tc->cellmartl = 0;
	tc->cellmartr = 0;
	tc->cellmartt = 0;
	tc->cellmartb = 0;

	tc->cellvalgn = fv->valign;

	if (fv->brdrl) {
		tc->brdrl = (brdrinfo *) fv->brdrl;
		tc->brdrl->color = CheckColorValue(fv->brdrl->color);
	}
	if (fv->brdrr) {
		tc->brdrr = (brdrinfo *) fv->brdrr;
		tc->brdrr->color = CheckColorValue(fv->brdrr->color);
	}
	if (fv->brdrt) {
		tc->brdrt = (brdrinfo *) fv->brdrt;
		tc->brdrt->color = CheckColorValue(fv->brdrt->color);
	}
	if (fv->brdrb) {
		tc->brdrb = (brdrinfo *) fv->brdrb;
		tc->brdrb->color = CheckColorValue(fv->brdrb->color);
	}

	// cellfill, cellcolr
	if (fv->shade) {
		tc->cellshd = (shdinfo *) fv->shade;
		tc->cellshd->color = CheckColorValue(fv->shade->color);
		tc->cellshd->bcolor = CheckColorValue(fv->shade->bcolor, 9);
	}

	return fv;
}


// page processing

char *RTFwr::CurrSection = NULL;     // section currently in effect
DCnlist RTFwr::SectionStart;  // section name indexed by ParaRefNum
bool RTFwr::StartingSection = false;
DCvlist RTFwr::SectProps;
DCnlist RTFwr::SectNames;
long RTFwr::SectCount = 0;
page *RTFwr::DocFormatProps = NULL;


void
RTFwr::GetPageFormatFileInfo(void)
{
	if (!PagesFile)
		return;

	if (!PagesFile->CheckPgFormat("Document")) {
		LogEvent(logwarn, 1, "Pages file missing [Document], page formats ignored");
		return;
	}

	pagefmtvals *fv = new pagefmtvals;
	page *dfp = new page;
	DocFormatProps = dfp;  // save for re-init of sects

	// in WriteDocProps
	fv->twosided = TwoSided;
	fv->firstright = FirstRight;

	// page props changeable in sections
	fv->margmirror = MirrorMarg;
	fv->pgnform = PgnForm;
	fv->pgnstart = PgnStart;
	fv->paperw = PaperWide;
	fv->paperh = PaperHigh;
	fv->margl = LMarg;
	fv->margr = RMarg;
	fv->margt = TMarg;
	fv->margb = BMarg;
	fv->gutter = Gutter;
	fv->cols = DefCols;
	fv->colsx = ColGap;
	fv->linebetcol = ColLine;
	fv->headery = HeaderYPos;
	fv->footery = FooterYPos;

	fv = PagesFile->PageFormat("Document", fv);

	// set globals from fv
	TwoSided = fv->twosided;
	FirstRight = fv->firstright;
	if (fv->startsection) {
		CurrSection = fv->startsection;
		SectBreakType = "sbknone";
		//StartingSection = true;
	}

	dfp->margmirror = MirrorMarg = fv->margmirror;
	dfp->pgnform = PgnForm = fv->pgnform;
	dfp->pgnstart = PgnStart = fv->pgnstart;
	dfp->porient = 0;
	dfp->pwide = PaperWide = fv->paperw;
	dfp->phigh = PaperHigh = fv->paperh;
	dfp->margl = LMarg = (uns) fv->margl;
	dfp->margr = RMarg = (uns) fv->margr;
	dfp->margt = TMarg = (uns) fv->margt;
	dfp->margb = BMarg = (uns) fv->margb;
	dfp->gutter = Gutter = fv->gutter;
	dfp->pcols = DefCols = fv->cols;
	dfp->pcgap = ColGap = fv->colsx;
	dfp->linebetcol = ColLine = fv->linebetcol;

	// store for later sections

	dfp->titlepg = TitlePage;
	dfp->titlespa = FirstPageSpaceAbove;
	dfp->headery = HeaderYPos = fv->headery;
	dfp->footery = FooterYPos = fv->footery;
	dfp->hdb = NULL;
	dfp->ftb = NULL;
	dfp->hdf = NULL;
	dfp->ftf = NULL;
	dfp->hdl = NULL;
	dfp->ftl = NULL;
	dfp->hdr = NULL;
	dfp->ftr = NULL;
}


page *
RTFwr::GetSectFormatFileInfo(char *sectfmtname)
{
	// get section format referenced in Document
	if (!PagesFile)
		return NULL;

	page *fp = NULL;
	long fnum = 0;
	if (((fnum = SectNames(sectfmtname)) != 0)
	 && ((fp = (page *) SectProps.find(fnum)) != NULL))
		return fp;

	sectfmtvals *fv = new sectfmtvals;
	fp = new page;  // section properties
	SectNames.add(sectfmtname, ++SectCount);
	SectProps.add(fp, SectCount);
	page *dfp = DocFormatProps;

	// set defaults from document
	fv->margmirror = dfp->margmirror;
	fv->pgnform = dfp->pgnform;
	fv->pgnstart = dfp->pgnstart;
	fv->lndscp = dfp->porient;
	fv->paperw = dfp->pwide;
	fv->paperh = dfp->phigh;
	fv->margl = dfp->margl;
	fv->margr = dfp->margr;
	fv->margt = dfp->margt;
	fv->margb = dfp->margb;
	fv->gutter = dfp->gutter;
	fv->cols = dfp->pcols;
	fv->colsx = dfp->pcgap;
	fv->linebetcol = dfp->linebetcol;

	fv->titlepg = dfp->titlepg;
	fv->titlespa = dfp->titlespa;
	fv->headery = (uns) dfp->headery;
	fv->footery = (uns) dfp->footery;
	fv->hdb = dfp->hdb;
	fv->ftb = dfp->ftb;
	fv->hdf = dfp->hdf;
	fv->ftf = dfp->ftf;
	fv->hdl = dfp->hdl;
	fv->ftl = dfp->ftl;
	fv->hdr = dfp->hdr;
	fv->ftr = dfp->ftr;

	if (!PagesFile->CheckPgFormat(sectfmtname))
		sectfmtname = "Section";

	fv = PagesFile->SectFormat(sectfmtname, fv);

	// set globals from fv

	fp->margmirror = fv->margmirror;
	fp->pgnform = fv->pgnform;
	fp->pgnstart = fv->pgnstart;
	fp->porient = fv->lndscp;
	fp->pwide = fv->paperw;
	fp->phigh = fv->paperh;
	fp->plmrg = (short) fv->margl;
	fp->prmrg = (short) fv->margr;
	fp->ptmrg = (short) fv->margt;
	fp->pbmrg = (short) fv->margb;
	fp->gutter = fv->gutter;
	fp->pcols = fv->cols;
	fp->pcgap = fv->colsx;
	fp->linebetcol = fv->linebetcol;

	// borders
	if (fv->brdrt) {
		fp->brdrt = (brdrinfo *) fv->brdrt;
		fp->brdrt->color = CheckColorValue(fv->brdrt->color);
	}
	if (fv->brdrb) {
		fp->brdrb = (brdrinfo *) fv->brdrb;
		fp->brdrb->color = CheckColorValue(fv->brdrb->color);
	}
	if (fv->brdrl) {
		fp->brdrl = (brdrinfo *) fv->brdrl;
		fp->brdrl->color = CheckColorValue(fv->brdrl->color);
	}
	if (fv->brdrr) {
		fp->brdrr = (brdrinfo *) fv->brdrr;
		fp->brdrr->color = CheckColorValue(fv->brdrr->color);
	}

	fp->brwraphd = fv->brwraphd;
	fp->brwrapft = fv->brwrapft;

	// headers and footers

	fp->titlepg = fv->titlepg;
	fp->titlespa = fv->titlespa;
	fp->headery = fv->headery;
	fp->footery = fv->footery;

	if (fv->hdb)
		fp->hdb = fv->hdb;
	if (fv->ftb)
		fp->ftb = fv->ftb;
	if (fv->hdf)
		fp->hdf = fv->hdf;
	if (fv->ftf)
		fp->ftf = fv->ftf;
	if (fv->hdl)
		fp->hdl = fv->hdl;
	if (fv->ftl)
		fp->ftl = fv->ftl;
	if (fv->hdr)
		fp->hdr = fv->hdr;
	if (fv->ftr)
		fp->ftr = fv->ftr;

	return fp;
}

