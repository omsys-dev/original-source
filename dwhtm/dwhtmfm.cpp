// dwhtmfm.cpp is the code for dcl HTM format processing
// Copyright (c) 2009 by Omni Systems, Inc.  All rights reserved.

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




bool HTMwr::MergeCurrFormatProps = false;
bool HTMwr::FormatBaseSet = false;


// block and inline formats


fmtvals *
HTMwr::GetFormatFileInfo(char *fmtname, pprop *prop)
{
	// update prop based on format info, if any

	if (!FmtFile
	 || !FmtFile->CheckFormat(fmtname))
		return NULL;

	// set fmtvals from prop
	fmtvals *fv = new fmtvals;

	if (MergeCurrFormatProps) {
		// char props
		fv->color = ColorValue(prop->colr);
		fv->bcolor = ColorValue(prop->bcolr);
		fv->font = FontNameList.find(prop->fnum);
		fv->fsize = prop->psize;
		fv->fstyle = prop->ital;
		fv->fvar = prop->scaps;
		fv->fweight = prop->fweight;
		fv->fkern = prop->kern;
		fv->tdecor = prop->line;
		fv->tstrike = prop->strike;
		fv->ttrans = prop->caps;

		// para props
		fv->lhigh = prop->lspace;
		fv->lmode = prop->lsmode;
		fv->talign = prop->algn;
		fv->valign = prop->valgn;
		fv->margl = prop->lind;
		fv->margr = prop->rind;
		fv->margt = prop->bspace;
		fv->margb = prop->aspace;
		fv->tind = prop->find - prop->lind;

		// list props
		fv->listnum = prop->list;
		switch (prop->list) {
			case 1:
				fv->liststyle = "decimal";
				fv->listelem = "ol";
				break;

			case 2:
				fv->liststyle = "lower-roman";
				fv->listelem = "ol";
				break;

			case 3:
				fv->liststyle = "upper-roman";
				fv->listelem = "ol";
				break;

			case 4:
				fv->liststyle = "lower-alpha";
				fv->listelem = "ol";
				break;

			case 5:
				fv->liststyle = "upper-alpha";
				fv->listelem = "ol";
				break;

			case 10:
				fv->listnum = 6;
			case 6:
				fv->liststyle = "disc";
				fv->listelem = "ul";
				break;

			case 7:
				fv->liststyle = "circle";
				fv->listelem = "ul";
				break;

			case 8:
				fv->liststyle = "square";
				fv->listelem = "ul";
				break;

			default:
				fv->listnum = 0;
				break;
		}
		fv->listlevel = prop->llev;
	}

	// modify vals based on named format
	fv = FmtFile->Format(fmtname, fv);
	if (FormatBaseSet) {  // vals set by first call to Format
		CssBodyFontTwips = (long) floor(FmtFile->BodyBaseVal + 0.5);
		CssIndentBaseTwips = (long) floor(FmtFile->IndentBaseVal + 0.5);
		FormatBaseSet = false;
	}

	// reset prop from format
	prop->colr = GetColorNum(fv->color);
	prop->bcolr = GetColorNum(fv->bcolor);

	if (fv->font) {
		uns fn = (uns) FontNameList.getitem(fv->font);
		if (fn)
			prop->fnum = fn;
		else {
			finfo *finf = new finfo;
			finf->fname = NewName(fv->font);
			finf->ftype = 0;
			finf->ftsym = !_stricmp(finf->fname, "Symbol") ? 1 :
									 (!_stricmp(finf->fname, "Wingdings") ? 2 : 0);
			finf->fnum = ++FontNum;
			FontList.add(finf, FontNum);
			FontNameList.add(finf->fname, FontNum);
			prop->fnum = FontNum;
			prop->fsym = finf->ftsym;
		}
	}
	prop->psize = (uns) fv->fsize;
	prop->ital = fv->fstyle;
	prop->scaps = fv->fvar;
	prop->fweight = fv->fweight;
	switch (fv->fweight) {
		case 0:
			break;
		case 1:
		case 2:
			prop->bold = 1;
			break;
		default:
			prop->bold = 2;
			break;
	}
	if (fv->fkern)
		prop->kern = (short) fv->fkern;
	prop->line = fv->tdecor;
	prop->strike = fv->tstrike;
	prop->caps = fv->ttrans;

	prop->lspace = (short) fv->lhigh;
	prop->lsmode = fv->lmode;
	if (fv->talign == 4)
		prop->algn = 1;
	else
		prop->algn = fv->talign;
	prop->valgn = fv->valign;
	prop->lind = (short) fv->margl;
	prop->rind = (short) fv->margr;
	prop->bspace = (short) fv->margt;
	prop->aspace = (short) fv->margb;
	prop->find = prop->lind + (short) fv->tind;

	if (prop->stype == 1) {
		if (fv->tabs && fv->tcount) {
			prop->tabs = fv->tabs;
			prop->tcount = (uns) fv->tcount;
		}

		if (!prop->maccb
		 && fv->before) {
			prop->maccb = fv->before;
			prop->macpos |= MacBefore;
		}
		if (!prop->maccs
		 && fv->start) {
			prop->maccs = fv->start;
			prop->macpos |= MacStart;
		}
		if (!prop->macce
		 && fv->end) {
			prop->macce = fv->end;
			prop->macpos |= MacEnd;
		}
		if (!prop->macca
		 && fv->after) {
			prop->macca = fv->after;
			prop->macpos |= MacAfter;
		}
	}
	else if (prop->stype == 2) {
		if (!prop->cmaccb
		 && fv->before) {
			prop->cmaccb = fv->before;
			prop->cmacpos |= MacBefore;
		}
		if (!prop->cmaccs
		 && fv->start) {
			prop->cmaccs = fv->start;
			prop->cmacpos |= MacStart;
		}
		if (!prop->cmacce
		 && fv->end) {
			prop->cmacce = fv->end;
			prop->cmacpos |= MacEnd;
		}
		if (!prop->cmacca
		 && fv->after) {
			prop->cmacca = fv->after;
			prop->cmacpos |= MacAfter;
		}
	}

	if (fv->brdrt)
		prop->cbrdrt = (brdrinfo *) fv->brdrt;
	if (fv->brdrb)
		prop->cbrdrb = (brdrinfo *) fv->brdrb;
	if (fv->brdrl)
		prop->cbrdrl = (brdrinfo *) fv->brdrl;
	if (fv->brdrr)
		prop->cbrdrr = (brdrinfo *) fv->brdrr;

	if (fv->shade) {
		prop->cshd = new shdinfo;
		*(prop->cshd) = *((shdinfo *) fv->shade);
		//prop->cshd = (shdinfo *) fv->shade;
		prop->cshd->color = GetColorNum(fv->shade->color);
		prop->cshd->bcolor = GetColorNum(fv->shade->bcolor);
	}

	// list properties
	prop->list = (unc) fv->listnum;
	if (prop->list)
		prop->hpname = (prop->list < 11) ? "li" :
			             (prop->listdd ? "dd" : "dt");
	prop->llev = (unc) fv->listlevel;			
	prop->lend = (!fv->listlevel) ? true : false;
	prop->lnest = !prop->list || (fv->listlevel > 1);
	prop->lfirst = fv->listfirst;

	prop->cssadd = fv->css;

	return fv;
}


char *
HTMwr::GetCSSBorderStyle(brdrtyp btyp)
{
	char *btype = NULL;

	switch (btyp) {
		case brdrdb:
			btype = "double";
			break;

		case brdrdot:
			btype = "dotted";
			break;

		case brdrdash:
			btype = "dashed";
			break;

		case brdrridge:
			btype = "ridge";
			break;

		case brdroutset:
			btype = "outset";
			break;

		case brdrgroove:
			btype = "groove";
			break;

		case brdrinset:
			btype = "inset";
			break;

		case brdrhidden:
			btype = "hidden";
			break;

		default:
		case brdrs:
			btype = "solid";
			break;
	}

	return btype;
}



// table formats


tbfmtvals *
HTMwr::GetTblFormatFileInfo(char *tbfmtname)
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
	if (KeepTableClassSpace)
		Tbl->cssclass = fv->cssclass = MakeID(tbfmtname, NULL, 0, 
			                             ' ', false, true, LowerCaseCSS);
	else	
		Tbl->cssclass = fv->cssclass = MakeID(tbfmtname, NULL, 0, 
		                             ClassSpaceChar, false, true, LowerCaseCSS);
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
		Tbl->brdrt->color = GetColorNum(fv->brdrt->color);
	}
	if (fv->brdrb) {
		Tbl->brdrb = (brdrinfo *) fv->brdrb;
		Tbl->brdrb->color = GetColorNum(fv->brdrb->color);
	}
	if (fv->brdrl) {
		Tbl->brdrl = (brdrinfo *) fv->brdrl;
		Tbl->brdrl->color = GetColorNum(fv->brdrl->color);
	}
	if (fv->brdrr) {
		Tbl->brdrr = (brdrinfo *) fv->brdrr;
		Tbl->brdrr->color = GetColorNum(fv->brdrr->color);
	}

	if (fv->colbrdrl) {
		Tbl->brdrcl = (brdrinfo *) fv->colbrdrl;
		Tbl->brdrcl->color = GetColorNum(fv->colbrdrl->color);
	}
	if (fv->colbrdrr) {
		Tbl->brdrcr = (brdrinfo *) fv->colbrdrr;
		Tbl->brdrcr->color = GetColorNum(fv->colbrdrr->color);
	}

	// tblbdyfill, tblbdycolr
	if (fv->shade) {
		Tbl->tblbdyshd = new shdinfo;
		*(Tbl->tblbdyshd) = *((shdinfo *) fv->shade);
		//Tbl->tblbdyshd = (shdinfo *) fv->shade;
		Tbl->tblbdyshd->color = GetColorNum(fv->shade->color);
		Tbl->tblbdyshd->bcolor = GetColorNum(fv->shade->bcolor);
	}

	// set up col widths
	long i = 0;
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

	if (CssTableStylesUsed(fv->cssclass) == 0) {
		CssTableStylesUsed.addsorted(fv->cssclass, ++CssTableStyleCount);
		CssTableStyles.add(fv, CssTableStyleCount);
	}
	return fv;
}


rowfmtvals *
HTMwr::GetRowFormatFileInfo(char *rowfmtname, long rnum)
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
	tr->cssclass = fv->cssclass = NewName(rowfmtname);

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
		tr->brdrt->color = GetColorNum(fv->brdrt->color);
	}
	if (fv->brdrb) {
		tr->brdrb = (brdrinfo *) fv->brdrb;
		tr->brdrb->color = GetColorNum(fv->brdrb->color);
	}
	if (fv->brdrx) {
		tr->brdrx = (brdrinfo *) fv->brdrx;
		tr->brdrx->color = GetColorNum(fv->brdrx->color);
	}

	// rowfill, rowcolr
	if (fv->shade) {
		tr->rowshd = new shdinfo;
		*(tr->rowshd) = *((shdinfo *) fv->shade);
		//tr->rowshd = (shdinfo *) fv->shade;
		tr->rowshd->color = GetColorNum(fv->shade->color);
		tr->rowshd->bcolor = GetColorNum(fv->shade->bcolor);
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


	if (CssRowStylesUsed(fv->cssclass) == 0) {
		CssRowStylesUsed.addsorted(fv->cssclass, ++CssRowStyleCount);
		CssRowStyles.add(fv, CssRowStyleCount);
	}
	return fv;
}


cellfmtvals *
HTMwr::GetCellFormatFileInfo(char *cellfmtname, long rnum, long cnum)
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
		fv->valign = tc->valgn;

		// shading
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
	tc->cssclass = fv->cssclass = NewName(cellfmtname);

	tc->cellmargl = (uns) fv->margl;
	tc->cellmargr = (uns) fv->margr;
	tc->cellmargt = (uns) fv->margt;
	tc->cellmargb = (uns) fv->margb;
	tc->cellmartl = 0;
	tc->cellmartr = 0;
	tc->cellmartt = 0;
	tc->cellmartb = 0;

	tc->valgn = fv->valign;

	if (fv->brdrl) {
		tc->brdrl = (brdrinfo *) fv->brdrl;
		tc->brdrl->color = GetColorNum(fv->brdrl->color);
	}
	if (fv->brdrr) {
		tc->brdrr = (brdrinfo *) fv->brdrr;
		tc->brdrr->color = GetColorNum(fv->brdrr->color);
	}
	if (fv->brdrt) {
		tc->brdrt = (brdrinfo *) fv->brdrt;
		tc->brdrt->color = GetColorNum(fv->brdrt->color);
	}
	if (fv->brdrb) {
		tc->brdrb = (brdrinfo *) fv->brdrb;
		tc->brdrb->color = GetColorNum(fv->brdrb->color);
	}

	// cellfill, cellcolr
	if (fv->shade) {
		tc->cellshd = new shdinfo;
		*(tc->cellshd) = *((shdinfo *) fv->shade);
		//tc->cellshd = (shdinfo *) fv->shade;
		tc->cellshd->color = GetColorNum(fv->shade->color);
		tc->cellshd->bcolor = GetColorNum(fv->shade->bcolor);
	}

	if (CssCellStylesUsed(fv->cssclass) == 0) {
		CssCellStylesUsed.addsorted(fv->cssclass, ++CssCellStyleCount);
		CssCellStyles.add(fv, CssCellStyleCount);
	}
	return fv;
}


// page formats







// border and shading formats


void
HTMwr::GetShading(HTMctl *cp, shdinfo **fptr)
{
	uns fill = 0;

  if (cp->siz() < FillMapMax)
    fill = FillMap[cp->siz()];

	if ((fill == CellFillEmpty)
	 && !*fptr)
		return;

	if (!*fptr)
		*fptr = new shdinfo;

	if (fill > CellFillMax)
		(*fptr)->tint = fill;
	else
		(*fptr)->type = (shdtyp) fill;
}


void
HTMwr::GetShadeColor(HTMctl *cp, shdinfo **fptr)
{
	long colornum = 0;
	GetColor(cp, &colornum);

	if (!colornum)
		return;

	if (!*fptr)
		*fptr = new shdinfo;

	(*fptr)->color = colornum;
}


void
HTMwr::WriteShadingFmt(HTMwfile *wf, shdinfo *sh)
{
	if ((sh->type == shdnone)
	 && (sh->tint == 0))
		return;

	wf->colorAttr("bgcolor", FillColorValue((uns) sh->tint,
		                           ColorValue((uns) sh->color)));
}


brdrinfo *
HTMwr::GetTableFileBorder(char *bfmt)
{
	if (!bfmt
	 || !TablesFile)
		return NULL;

	brinfo *br = TablesFile->GetBorder(bfmt);

	return (brdrinfo *) br;
}


// element format settings


DCnlist HTMwr::ElementFormNames;
npair HTMwr::ElementFormNamesInit[ElementFormsMax] = {
 { 1, "BodyElement" },
 { 2, "OrderedListElement" },
 { 3, "UnorderedListElement" },
 { 4, "AElement" },
 { 5, "AUnvisitedElement" },
 { 6, "AVisitedElement" },
 NULL
};

DCnlist HTMwr::ElementSelectors;
npair HTMwr::ElementSelectorsInit[ElementFormsMax] = {
 { 1, "body" },
 { 2, "ol" },
 { 3, "ul" },
 { 4, "a" },
 { 5, "a:link" },
 { 6, "a:visited" },
 NULL
};

DCnlist HTMwr::ElementCssDefaults;
npair HTMwr::ElementCssDefaultsInit[ElementFormsMax] = {
 { 1, "padding: 6pt;" },
 { 2, "margin-left: 12pt; padding-left: 12pt;" },
 { 3, "margin-left: 12pt; padding-left: 12pt;" },
 { 4, "text-decoration: none;" },
 { 5, "color: blue;" },
 { 6, "color: purple;" },
 NULL
};

DCvlist HTMwr::ElementCSSProps;
DCvlist HTMwr::ElementCSSVals;


void 
HTMwr::InitElementForms(void)
{
	if (!CssFile)
		return;

	long i = 0;
	char *fname = NULL;
	pprop *prop = NULL;
	fmtvals *fv = NULL;
	char *selector = NULL;

	ElementFormNames.init(ElementFormNamesInit);
	ElementSelectors.init(ElementSelectorsInit);
	ElementCssDefaults.init(ElementCssDefaultsInit);

	for (i = 1; i < ElementFormsMax; i++) {
		fname = ElementFormNames.find(i);
		prop = new pprop;
		prop->stype = (i < 4) ? 1 : 2;
		fv = GetFormatFileInfo(fname, prop);
		if ((i == 1)
		 && CssBodyFontTwips)
			prop->psize = (uns) CssBodyFontTwips;
		prop->cssadd = fv ? fv->css : ElementCssDefaults.find(i);
		selector = ElementSelectors.find(i);
		if ((i < 2) || (i > 3))  // don't write list styles
			WriteCssStyle(fv, prop, selector);
		ElementCSSProps.append(prop, i);
		ElementCSSVals.append(fv, i);
	}

}


void 
HTMwr::DefineRequiredFormats(DCfrfile *fmts)
{
	char *fname = NULL;
	char *ftype = NULL;
	uns tnum = 0;
	DCnlist fnames;
	DCilist ftyps;
	long count = 0;
#if 0
	char *selector = NULL;
	pprop *prop = NULL;
	fmtvals *fv = NULL;
#endif

	// already in section
	while (fmts->NextItem()) {
		fname = fmts->StrLeft();
		if (CSSDone(fname))
			continue;
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
		ftyps.append(tnum, count);
	}

	for (long i = 1; i <= count; i++) {
		WriteOneCSSFormat(fnames.find(i), (uns) ftyps.find(i));
#if 0
		fname = fnames.find(i);
		selector = NewName(".", strlen(fname) + 2);
		strcat(selector, fname);
		prop = new pprop;
		prop->stype = (uns) ftyps.find(i);
		if ((fv = GetFormatFileInfo(fname, prop)) != NULL) {
			prop->cssadd = fv->css;
			WriteCssStyle(fv, prop, selector);
		}
		delete prop;
		DeleteName(selector);
#endif
	}
}


void
HTMwr::WriteOneCSSFormat(char *fname, uns ftype)
{
	if (!fname)
		return;

	char *selector = NewName(".", strlen(fname) + 2);
	strcat(selector, fname);
	pprop *prop = new pprop;
	prop->stype = ftype;  // block = 1, inline = 2
	fmtvals *fv = NULL;
	if ((fv = GetFormatFileInfo(fname, prop)) != NULL) {
		prop->cssadd = fv->css;
		WriteCssStyle(fv, prop, selector);
	}
	delete prop;
	DeleteName(selector);
}



// CSS writing


DCnlist HTMwr::CSSDone;


void
HTMwr::WriteCssStyle(fmtvals *fv, pprop *prop, char *selector)
{
	if (!CssFile)
		return;

	if (prop->nocss)
		return;

	if (prop->cssrep) {
		if (prop->maccss) {
			char *txt = NULL;
			DCmacr::WriteMacro(CssFile, txt = NewName(prop->maccss));
			CssFile->putCr();
			CssFile->putCr();
			DeleteName(txt);
		}
		return;
	}

	char *cname = "unknown";

	if (!fv
	 && XMLSource
	 && ShowUndefinedFormats
	 && !(!strcmp(prop->stag, "Delete")
	  || !strcmp(prop->stag, "ph")
	  || !strcmp(prop->stag, "data")))
		LogEvent(logwarn, 1, "Undefined ", (prop->stype == 1) ? "block" : "inline",
		 " format: ", prop->stag);

	if (fv
	 && fv->selector) {
		CSSDone.add(fv->selector, 1);
		CssFile->putStr(fv->selector);
	}
	else if (selector) {
		CSSDone.addnew(selector, 1);
		CssFile->putStr(selector);
	}
	else {
		char *nname = NULL;

		cname = (prop->stype == 1) ? prop->hcname : prop->hscname;
		if (!cname)
			return;

		char *spc = strchr(cname, ' ');
		if (spc) {
			spc -= (long) cname;
			cname = NewName(cname);
			spc += (long) cname;
			*spc = '\0';
		}

		uns len = strlen(cname) + 2;

		char *tname = (prop->stype == 1) ? prop->hpname : prop->hsname;
		if (HXMwr::DITA
		 || !SelectorIncludesTag) { // specify class only, not tag
			nname = NewName(".", len);
			strcat(nname, cname);
			//CssFile->putChar('.');
		}
		else if (!ClassIsTag) {
			uns tlen = 0;
			if (tname) {
				spc = strchr(tname, ' ');
				tlen = spc ? spc - tname : strlen(tname);
				//CssFile->putStr(tname, spc ? spc - tname : 0);
			}
			nname = NewName(len + tlen);
			if (tname)
				strncpy(nname, tname, tlen);
			strcat(nname, "."); 
			strcat(nname, cname); 
			//CssFile->putChar('.');
		}
		else
			nname = cname;

		if (CSSDone(nname))
			return;
		CSSDone.add(nname, 1);

		CssFile->putStr(nname);
	}

	CssFile->putChar(' ');
	CssFile->putChar('{');
	CssFile->putCr();

	if (Xml) {
		CssFile->putStr(" display: ");
		if (fv && fv->cdisp
		 && (fv->stype == prop->stype))
			CssFile->putStr(fv->cdisp);
		else {
			CssFile->putStr((prop->stype == 1) ? "block" : "inline");
			if (fv && fv->cdisp
			 && (fv->stype != prop->stype))
				LogEvent(logwarn, 1, "Display type mismatch for class: ", cname);
		}
		CssFile->putChar(';');
		CssFile->putCr();
	}


	// size and family are required for "font:"
	bool usefont = false;

	if ((prop->psize || (fv && fv->cfsize))
	 && (prop->fnum || (fv && fv->cfont))) {
		// style variant weight size/height "family"
		usefont = true;
		CssFile->putStr(" font: ");
	}

	if ((prop->ital > 1)
	 || (fv && fv->cfstyle)) {
		if (!usefont)
			CssFile->putStr(" font-style: ");
		if (fv && fv->cfstyle)
			CssFile->putStr(fv->cfstyle);
		else
			CssFile->putStr("italic");
		if (!usefont) {
			CssFile->putChar(';');
			CssFile->putCr();
		}
		else
			CssFile->putChar(' ');
	}

	if ((prop->scaps > 1)
	 || (fv && fv->cfvar)) {
		if (!usefont)
			CssFile->putStr(" font-variant: ");
		if (fv && fv->cfvar)
			CssFile->putStr(fv->cfvar);
		else
			CssFile->putStr("small-caps");
		if (!usefont) {
			CssFile->putChar(';');
			CssFile->putCr();
		}
		else
			CssFile->putChar(' ');
	}

	if (prop->fweight
	 || (fv && fv->cfweight)) {
		if (!usefont)
			CssFile->putStr(" font-weight: ");
		if (fv && fv->cfweight)
			CssFile->putStr(fv->cfweight);
		else {
			switch (prop->fweight) {
				case 1:
					CssFile->putStr("100");
					break;
				case 2:
					CssFile->putStr("200");
					break;
				case 3:
					CssFile->putStr("300");
					break;
				case 4:
					CssFile->putStr("bold");
					break;
				case 5:
					CssFile->putStr("700");
					break;
				case 6:
					CssFile->putStr("900");
					break;
				default:
					break;
			}
		}
		if (!usefont) {
			CssFile->putChar(';');
			CssFile->putCr();
		}
		else
			CssFile->putChar(' ');
	}

	if (prop->psize
	 || (fv && fv->cfsize)) {
		if (!usefont)
			CssFile->putStr(" font-size: ");
		if (fv && fv->cfsize)
			CssFile->putStr(fv->cfsize);
		else
			WriteCssPts(prop->psize, fnth);
		if (!usefont) {
			CssFile->putChar(';');
			CssFile->putCr();
		}
		else if (!prop->lsmode
		 || (prop->stype != 1)
		 || !CSSLeading) 
			CssFile->putChar(' ');
	}

	if ((prop->lsmode
	  && (prop->stype == 1)
	  && CSSLeading)
	 || (fv && fv->clhigh)) {
		if (!usefont)
			CssFile->putStr(" line-height: ");

		if (fv && fv->clhigh) {
			if (usefont)
				CssFile->putChar('/');
			CssFile->putStr(fv->clhigh);
		}
		else {
			switch (prop->lsmode) {
				case 1:
				case 2:
					if (usefont)
						CssFile->putChar('/');
					WriteCssPts(prop->lspace, fnth);
					break;
				case 3:
				case 4:
					if (usefont)
						CssFile->putChar('/');
					WriteCssPts(prop->lspace + prop->psize, fnth);
					break;
				case 5:
					if (prop->lspace > 0) {
						if (usefont)
							CssFile->putChar('/');
						WriteCssPts((((long) prop->lspace) * prop->psize) / 1000L, fnth);
					}
					break;
				default:
					break;
			}
		}
		if (!usefont) {
			CssFile->putChar(';');
			CssFile->putCr();
		}
		else
			CssFile->putChar(' ');
	}

	if (prop->fnum
	 || (fv && fv->cfont)) {
		if (!usefont)
			CssFile->putStr(" font-family: ");
		if (fv && fv->cfont)
			CssFile->putStr(fv->cfont);
		else {
			char *face = FontNameList.find(prop->fnum);
			if (strchr(face, '"')  // user quotes override
			 || strchr(face, ',')
			 || !strchr(face, ' '))
				CssFile->putStr(face);
			else {
				CssFile->putChar('"');
				CssFile->putStr(face);
				CssFile->putChar('"');
			}
		}
		if (!usefont) {
			CssFile->putChar(';');
			CssFile->putCr();
		}
	}

	if (usefont) {
		CssFile->putChar(';');
		CssFile->putCr();
	}

#if 0
	if (prop->bspace
	 || prop->rind
	 || prop->aspace
	 || prop->lind
	 || ZeroCSSMargins
	 || (fv
		&& (fv->cmargt
		 || fv->cmargr
		 || fv->cmargb
		 || fv->cmargl))) {

		CssFile->putStr(" margin: "); // top, right, bottom, left

		// combined values with mixed strings and numerics 
		// is too hard to get right, just use all four
		if ((prop->bspace == prop->aspace)
		 && (prop->rind == prop->lind)) {
			if (prop->bspace == prop->rind) {  // single value
				if (prop->bspace == 0)
					CssFile->putChar('0');
				else {
					if (fv && fv->cmargt)
						CssFile->putStr(fv->cmargt);
					else
						WriteCssPts(prop->bspace, pspc);
				}
			}
			else {  // two: vert horiz
				if (fv && fv->cmargt)
					CssFile->putStr(fv->cmargt);
				else
					WriteCssPts(prop->bspace, pspc);
				CssFile->putChar(' ');
				if (fv && fv->cmargr)
					CssFile->putStr(fv->cmargr);
				else
					WriteCssPts(prop->rind, hpos);
			}
		}
		else {  // all four
#endif

	if (prop->bspace
	 || (fv	&& fv->cmargt)) {
		CssFile->putStr(" margin-top: ");
		if (fv && fv->cmargt)
			CssFile->putStr(fv->cmargt);
		else
			WriteCssPts(prop->bspace, pspc);
		CssFile->putChar(';');
		CssFile->putCr();
	}

	if (prop->rind
	 || (fv && fv->cmargr)) {
		CssFile->putStr(" margin-right: ");
		if (fv && fv->cmargr)
			CssFile->putStr(fv->cmargr);
		else
			WriteCssPts(prop->rind, hpos);
		CssFile->putChar(';');
		CssFile->putCr();
	}
	if (prop->aspace
	 || (fv	&& fv->cmargb)) {
		CssFile->putStr(" margin-bottom: ");
		if (fv && fv->cmargb)
			CssFile->putStr(fv->cmargb);
		else
			WriteCssPts(prop->aspace, pspc);
		CssFile->putChar(';');
		CssFile->putCr();
	}
	if (prop->lind
	 || (fv	&& fv->cmargl)) {
		CssFile->putStr(" margin-left: ");
		if (fv && fv->cmargl)
			CssFile->putStr(fv->cmargl);
		else
			WriteCssPts(prop->lind, hpos);
		CssFile->putChar(';');
		CssFile->putCr();
	}
#if 0
		}
		CssFile->putChar(';');
		CssFile->putCr();
	}
#endif

	if (fv
	 && (fv->padt || fv->cpadt)) {
		CssFile->putStr(" padding-top: ");
		if (fv->cpadt)
			CssFile->putStr(fv->cpadt);
		else
			WriteCssPts(fv->padt, pspc);
		CssFile->putChar(';');
		CssFile->putCr();
	}

	if (fv
	 && (fv->padr || fv->cpadr)) {
		CssFile->putStr(" padding-right: ");
		if (fv->cpadr)
			CssFile->putStr(fv->cpadr);
		else
			WriteCssPts(fv->padr, hpos);
		CssFile->putChar(';');
		CssFile->putCr();
	}
	if (fv
	 && (fv->padb || fv->cpadb)) {
		CssFile->putStr(" padding-bottom: ");
		if (fv->cpadb)
			CssFile->putStr(fv->cpadb);
		else
			WriteCssPts(fv->padb, pspc);
		CssFile->putChar(';');
		CssFile->putCr();
	}
	if (fv
	 && (fv->padl || fv->cpadl)) {
		CssFile->putStr(" padding-left: ");
		if (fv->cpadl)
			CssFile->putStr(fv->cpadl);
		else
			WriteCssPts(fv->padl, hpos);
		CssFile->putChar(';');
		CssFile->putCr();
	}

	if (prop->stype == 1) {
		if ((prop->find != prop->lind)
		 || (fv && fv->ctind)) {
			CssFile->putStr(" text-indent: ");
			if (fv && fv->ctind)
				CssFile->putStr(fv->ctind);
			else
				WriteCssPts(prop->find - prop->lind, hpos);
			CssFile->putChar(';');
			CssFile->putCr();
		}

		if ((Xml || (prop->algn > 1))
		 || (fv && fv->ctalign)) {
			CssFile->putStr(" text-align: ");
			if (fv && fv->ctalign)
				CssFile->putStr(fv->ctalign);
			else {
				switch (prop->algn) {
					case 1:
						if (Xml)
							CssFile->putStr("left");
						break;
					case 2:
						CssFile->putStr("center");
						break;
					case 3:
						CssFile->putStr("right");
						break;
					default:
						break;
				}
			}
			CssFile->putChar(';');
			CssFile->putCr();
		}
	}

	if (!CellAlignAttributes
	 && ((prop->valgn > 0)
	  || (fv && fv->cvalign))) {
		CssFile->putStr(" vertical-align: ");
		if (fv && fv->cvalign)
			CssFile->putStr(fv->cvalign);
		else {
			switch (prop->valgn) {
				case 1:
					CssFile->putStr("middle");
					break;
				case 2:
					CssFile->putStr("top");
					break;
				case 3:
					CssFile->putStr("bottom");
					break;
				default:
					break;
			}
		}
		CssFile->putChar(';');
		CssFile->putCr();
	}

	if ((prop->kern != 0)
	 || (fv && fv->fkern)) {
		CssFile->putStr(" letter-spacing: ");
		if (fv && fv->fkern)
			WriteCssPts(fv->fkern, hpos);
		else
			WriteCssPts(prop->kern, hpos);
		CssFile->putChar(';');
		CssFile->putCr();
	}

	if ((prop->line > 1)
	 || (prop->strike > 1)
	 || (fv && fv->ctdecor)) {
		CssFile->putStr(" text-decoration: ");
		if (fv && fv->ctdecor)
			CssFile->putStr(fv->ctdecor);
		else if (prop->line == 3)
			CssFile->putStr("overline");
		else if (prop->line == 2)
			CssFile->putStr("underline");
		else if (prop->strike == 2)
			CssFile->putStr("line-through");
		CssFile->putChar(';');
		CssFile->putCr();
	}

	if ((prop->caps > 1)
	 || (fv && fv->cttrans)) {
		CssFile->putStr(" text-transform: ");
		if (fv && fv->cttrans)
			CssFile->putStr(fv->cttrans);
		else {
			switch (prop->caps) {
				case 2:
					CssFile->putStr("uppercase");
					break;
				case 3:
					CssFile->putStr("lowercase");
					break;
				case 4:
					CssFile->putStr("capitalize");
					break;
				default:
					break;
			}
		}
		CssFile->putChar(';');
		CssFile->putCr();
	}

	if (((prop->colr > 2)
	  && !prop->nocolor)
	 || (fv && fv->ccolor)) {
		CssFile->putStr(" color: ");
		if (fv && fv->ccolor)
			CssFile->putStr(fv->ccolor);
		else
			WriteCssColor(CssFile, ColorValue(prop->colr));
		CssFile->putChar(';');
		CssFile->putCr();
	}


	if (fv && fv->shade)
		WriteCssShading(CssFile, fv->shade);
	else if (fv && fv->cbcolor) {
		CssFile->putStr(" background-color: ");
		CssFile->putStr(fv->cbcolor);
		CssFile->putChar(';');
		CssFile->putCr();
	}

	if (prop->cbrdrt)
		WriteCssBorder(CssFile, "top", prop->cbrdrt);
	if (prop->cbrdrb)
		WriteCssBorder(CssFile, "bottom", prop->cbrdrb);
	if (prop->cbrdrl)
		WriteCssBorder(CssFile, "left", prop->cbrdrl);
	if (prop->cbrdrr)
		WriteCssBorder(CssFile, "right", prop->cbrdrr);

	if (prop->cssadd) {
		char *txt = NewName(prop->cssadd);
		if (*txt != ' ')
			CssFile->putChar(' ');
		DCmacr::WriteMacro(CssFile, txt);
		CssFile->putCr();
		DeleteName(txt);
	}

	CssFile->putStr(" }");
	CssFile->putCr();
	CssFile->putCr();

	if (cname
	 && fv
	 && (fv->liststyle
	  || fv->listelem)) {
		if (!fv->listelem) {
			CssFile->putStr("ol.");
			CssFile->putStr(cname);
			CssFile->putStr(", ul.");
			CssFile->putStr(cname);
		}
		else {
			CssFile->putStr(fv->listelem);
			CssFile->putChar('.');
			CssFile->putStr(cname);
		}
		CssFile->putStr(" {");
		CssFile->putCr();
		if (fv->listelem) {
			long etyp = ElementSelectors(fv->listelem);
			if ((etyp == 2)
			 || (etyp == 3)) {
				pprop *pp = (pprop *) ElementCSSProps.find(etyp);
				fmtvals *fp = (fmtvals *) ElementCSSVals.find(etyp);
				if (pp || fp) {
					if ((pp && pp->lind)
					 || (fp && fp->cmargl)) {
						CssFile->putStr(" margin-left: ");
						if (fp && fp->cmargl)
							CssFile->putStr(fp->cmargl);
						else
							WriteCssPts(pp->lind, hpos);
						CssFile->putChar(';');
						CssFile->putCr();
					}
					if (fp && (fp->padl || fp->cpadl)) {
						CssFile->putStr(" padding-left: ");
						if (fp->cpadl)
							CssFile->putStr(fp->cpadl);
						else
							WriteCssPts(fp->padl, hpos);
						CssFile->putChar(';');
						CssFile->putCr();
					}
					if (pp && pp->cssadd) {
						char *txt = NewName(pp->cssadd);
						if (*txt != ' ')
							CssFile->putChar(' ');
						DCmacr::WriteMacro(CssFile, txt);
						CssFile->putCr();
						DeleteName(txt);
					}
				}
			}
		}
		CssFile->putStr(" list-style: ");
		if (fv->liststyle) {
			if (!stricmp(fv->liststyle, "nonumber")
			 || !stricmp(fv->liststyle, "nobullet"))
				CssFile->putStr("none");
			else
				CssFile->putStr(fv->liststyle);
			if (fv->listurl
			 || fv->listpos)
				CssFile->putChar(' ');
		}
		if (fv->listurl) {
			CssFile->putStr("url(");
			CssFile->putStr(fv->listurl);
			CssFile->putChar(')');
			if (fv->listpos)
				CssFile->putChar(' ');
		}
		if (fv->listpos)
			CssFile->putStr(fv->listpos);
		CssFile->putChar(';');
		CssFile->putCr();
		CssFile->putStr(" }");
		CssFile->putCr();
		CssFile->putCr();
	}
}


DCnlist HTMwr::CssTableStylesUsed;
DCvlist HTMwr::CssTableStyles;
long HTMwr::CssTableStyleCount = 0;
DCnlist HTMwr::CssRowStylesUsed;
DCvlist HTMwr::CssRowStyles;
long HTMwr::CssRowStyleCount = 0;
DCnlist HTMwr::CssCellStylesUsed;
DCvlist HTMwr::CssCellStyles;
long HTMwr::CssCellStyleCount = 0;

void
HTMwr::WriteCssTableFormats(void)
{
	if (!CssFile)
		return;

	long i = 0;
	DCnlist *lp = NULL;

	lp = &CssTableStylesUsed;
	while (lp->next) {
		lp = lp->next;
		WriteCssTableStyle((tbfmtvals *) CssTableStyles.find(lp->id));
	}

	lp = &CssRowStylesUsed;
	while (lp->next) {
		lp = lp->next;
		WriteCssRowStyle((rowfmtvals *) CssRowStyles.find(lp->id));
	}

	lp = &CssCellStylesUsed;
	while (lp->next) {
		lp = lp->next;
		WriteCssCellStyle((cellfmtvals *) CssCellStyles.find(lp->id));
	}
}


void
HTMwr::WriteCssTableStyle(tbfmtvals *fv)
{
	if (!CssFile
	 || !fv)
		return;

	CssFile->putStr("table.");
	CssFile->putStr(fv->cssclass);
	CssFile->putChar(' ');
	CssFile->putChar('{');
	CssFile->putCr();

	if (fv->brdrmodel) {
		CssFile->putStr(" border-collapse: ");
		CssFile->putStr(fv->brdrmodel);
	}

	if (fv->brdrspacing) {
		CssFile->putStr(" border-spacing: ");
		CssFile->putStr(fv->brdrspacing);
	}

	if (fv->emptycells) {
		CssFile->putStr(" empty-cells: ");
		CssFile->putStr(fv->emptycells);
	}

	if (fv->tbllayout) {
		CssFile->putStr(" table-layout: ");
		CssFile->putStr(fv->tbllayout);
	}

	if (fv->ctbwide) {
		CssFile->putStr(" width: ");
		CssFile->putStr(fv->ctbwide);
	}

	if (fv->margl || fv->cmargl) {
		CssFile->putStr(" margin-left: ");
		if (fv->cmargl)
			CssFile->putStr(fv->cmargl);
		else
			WriteCssPts(fv->margl, hpos);
		CssFile->putChar(';');
		CssFile->putCr();
	}

	if (fv->margt || fv->cmargt) {
		CssFile->putStr(" margin-top: ");
		if (fv->cmargt)
			CssFile->putStr(fv->cmargt);
		else
			WriteCssPts(fv->margt, pspc);
		CssFile->putChar(';');
		CssFile->putCr();
	}

	if (fv->margb || fv->cmargb) {
		CssFile->putStr(" margin-bottom: ");
		if (fv->cmargb)
			CssFile->putStr(fv->cmargb);
		else
			WriteCssPts(fv->margb, pspc);
		CssFile->putChar(';');
		CssFile->putCr();
	}

	if (fv->brdrt)
		WriteCssBorder(CssFile, "top", (brdrinfo *) fv->brdrt);
	if (fv->brdrb)
		WriteCssBorder(CssFile, "bottom", (brdrinfo *) fv->brdrb);
	if (fv->brdrl)
		WriteCssBorder(CssFile, "left", (brdrinfo *) fv->brdrl);
	if (fv->brdrr)
		WriteCssBorder(CssFile, "right", (brdrinfo *) fv->brdrr);

	if (fv->shade)
		WriteCssShading(CssFile, fv->shade);

	if (fv->css) {
		char *txt = NULL;
		DCmacr::WriteMacro(CssFile, txt = NewName(fv->css));
		CssFile->putCr();
		DeleteName(txt);
	}

	CssFile->putStr(" }");
	CssFile->putCr();
	CssFile->putCr();
}


void
HTMwr::WriteCssRowStyle(rowfmtvals *fv)
{
	if (!CssFile
	 || !fv)
		return;

	CssFile->putStr("tr.");
	CssFile->putStr(fv->cssclass);
	CssFile->putChar(' ');
	CssFile->putChar('{');
	CssFile->putCr();

	if (fv->rowhigh) {
		CssFile->putStr(" height: ");
		WriteCssPts(fv->rowhigh, pspc);
	}

	if (fv->brdrt)
		WriteCssBorder(CssFile, "top", (brdrinfo *) fv->brdrt);
	if (fv->brdrb)
		WriteCssBorder(CssFile, "bottom", (brdrinfo *) fv->brdrb);
	if (fv->brdrl)
		WriteCssBorder(CssFile, "left", (brdrinfo *) fv->brdrl);
	if (fv->brdrr)
		WriteCssBorder(CssFile, "right", (brdrinfo *) fv->brdrr);

	if (fv->shade)
		WriteCssShading(CssFile, fv->shade);

	if (fv->css) {
		char *txt = NULL;
		DCmacr::WriteMacro(CssFile, txt = NewName(fv->css));
		CssFile->putCr();
		DeleteName(txt);
	}

	CssFile->putStr(" }");
	CssFile->putCr();
	CssFile->putCr();
}


void
HTMwr::WriteCssCellStyle(cellfmtvals *fv)
{
	if (!CssFile
	 || !fv)
		return;

	CssFile->putStr("td.");
	CssFile->putStr(fv->cssclass);
	CssFile->putStr(", th.");
	CssFile->putStr(fv->cssclass);
	CssFile->putChar(' ');
	CssFile->putChar('{');
	CssFile->putCr();

	if (!CellAlignAttributes
	 && ((fv->valign > 0)
	  || fv->cvalign)) {
		CssFile->putStr(" vertical-align: ");
		if (fv->cvalign)
			CssFile->putStr(fv->cvalign);
		else {
			switch (fv->valign) {
				case 1:
					CssFile->putStr("middle");
					break;
				case 2:
					CssFile->putStr("top");
					break;
				case 3:
					CssFile->putStr("bottom");
					break;
				default:
					break;
			}
		}
		CssFile->putChar(';');
		CssFile->putCr();
	}

#if 0
	if (fv->margl || fv->cmargl) {
		CssFile->putStr(" margin-left: ");
		if (fv->cmargl)
			CssFile->putStr(fv->cmargl);
		else
			WriteCssPts(fv->margl, hpos);
		CssFile->putChar(';');
		CssFile->putCr();
	}

	if (fv->margr || fv->cmargr) {
		CssFile->putStr(" margin-right: ");
		if (fv->cmargr)
			CssFile->putStr(fv->cmargr);
		else
			WriteCssPts(fv->margr, hpos);
		CssFile->putChar(';');
		CssFile->putCr();
	}

	if (fv->margt || fv->cmargt) {
		CssFile->putStr(" margin-top: ");
		if (fv->cmargt)
			CssFile->putStr(fv->cmargt);
		else
			WriteCssPts(fv->margt, pspc);
		CssFile->putChar(';');
		CssFile->putCr();
	}

	if (fv->margb || fv->cmargb) {
		CssFile->putStr(" margin-bottom: ");
		if (fv->cmargb)
			CssFile->putStr(fv->cmargb);
		else
			WriteCssPts(fv->margb, pspc);
		CssFile->putChar(';');
		CssFile->putCr();
	}
#endif

	if (fv->padl || fv->cpadl) {
		CssFile->putStr(" padding-left: ");
		if (fv->cpadl)
			CssFile->putStr(fv->cpadl);
		else
			WriteCssPts(fv->padl, hpos);
		CssFile->putChar(';');
		CssFile->putCr();
	}

	if (fv->padr || fv->cpadr) {
		CssFile->putStr(" padding-right: ");
		if (fv->cpadr)
			CssFile->putStr(fv->cpadr);
		else
			WriteCssPts(fv->padr, hpos);
		CssFile->putChar(';');
		CssFile->putCr();
	}

	if (fv->padt || fv->cpadt) {
		CssFile->putStr(" padding-top: ");
		if (fv->cpadt)
			CssFile->putStr(fv->cpadt);
		else
			WriteCssPts(fv->padt, pspc);
		CssFile->putChar(';');
		CssFile->putCr();
	}

	if (fv->padb || fv->cpadb) {
		CssFile->putStr(" padding-bottom: ");
		if (fv->cpadb)
			CssFile->putStr(fv->cpadb);
		else
			WriteCssPts(fv->padb, pspc);
		CssFile->putChar(';');
		CssFile->putCr();
	}

	if (fv->brdrt)
		WriteCssBorder(CssFile, "top", (brdrinfo *) fv->brdrt);
	if (fv->brdrb)
		WriteCssBorder(CssFile, "bottom", (brdrinfo *) fv->brdrb);
	if (fv->brdrl)
		WriteCssBorder(CssFile, "left", (brdrinfo *) fv->brdrl);
	if (fv->brdrr)
		WriteCssBorder(CssFile, "right", (brdrinfo *) fv->brdrr);

	if (fv->shade)
		WriteCssShading(CssFile, fv->shade);

	if (fv->css) {
		char *txt = NULL;
		DCmacr::WriteMacro(CssFile, txt = NewName(fv->css));
		CssFile->putCr();
		DeleteName(txt);
	}

	CssFile->putStr(" }");
	CssFile->putCr();
	CssFile->putCr();
}


// CSS writing utilities


void
HTMwr::WriteCssBorder(DCwfile *wf, char *pos, brdrinfo *br)
{
	if (!CssFile
	 || !wf
	 || !br)
		return;

	wf->putStr(" border-");
	wf->putStr(pos);
	wf->putStr(": ");
	WriteCssPts(br->thick, fnth);
	wf->putChar(' ');
	wf->putStr(GetCSSBorderStyle(br->type));
	wf->putChar(' ');
	WriteCssColor(wf, ColorValue((uns) br->color));
	wf->putChar(';');
	wf->putCr();
}


void
HTMwr::WriteCssShading(DCwfile *wf, shinfo *sh)
{
	if (!CssFile
	 || !wf
	 || !sh)
		return;

	long color = FillColorValue((uns) sh->tint, sh->color);
	wf->putStr(" background-color: ");
	WriteCssColor(wf, color);
	wf->putChar(';');
	wf->putCr();
}


void
HTMwr::WriteCssColor(DCwfile *wf, long color)
{
	unc cval[3];
	short i = 0;

	wf->putChar('#');
	for (i = 2; i >= 0; i--)
		cval[i] = (color >> (i * 8)) & 0xFF;

#if 0
	if ((((cval[0] >> 4) & 0x0F) == (cval[0] & 0x0F))
	 && (((cval[1] >> 4) & 0x0F) == (cval[1] & 0x0F))
	 && (((cval[2] >> 4) & 0x0F) == (cval[2] & 0x0F))) {
		for (i = 2; i >= 0; i--)
			wf->putHex(cval[i] & 0x0F);
	}
	else {
#endif
		for (i = 2; i >= 0; i--) {
			wf->putHex((cval[i] >> 4) & 0x0F);
			wf->putHex(cval[i] & 0x0F);
		}
	//}
}

unc HTMwr::CssFontUnits = 0;
unc HTMwr::CssIndentUnits = 0;
unc HTMwr::CssFontUnitDec = 2;
unc HTMwr::CssIndentUnitDec = 2;
long HTMwr::CssBodyFontSize = 10;
long HTMwr::CssBodyFontTwips = 200;
unc HTMwr::CssBodyFontUnit = 0;
bool HTMwr::CssBodyFontTag = true;
long HTMwr::CssIndentBaseSize = 6;
long HTMwr::CssIndentBaseTwips = 8640;
unc HTMwr::CssIndentBaseUnit = 2;

char *HTMwr::CssUnits[CssUnitsMax] = {
 "pt", "pc", "in", "cm", "mm",
 "em", "ex", "px", "%"
 };


void
HTMwr::SetCssFontTwips(void)
{
	switch (CssBodyFontUnit) {
		case 1:  //pc
			CssBodyFontTwips = CssBodyFontSize * 240;
			break;
		case 2:  //in
			CssBodyFontTwips = CssBodyFontSize * 1440;
			break;
		case 3:  //cm
			CssBodyFontTwips = CssBodyFontSize * 567;
			break;
		case 4:  //mm
			CssBodyFontTwips = (CssBodyFontSize * 567) / 10;
			break;
		case 8:  //px
			CssBodyFontTwips = CssBodyFontSize * 16;
			break;
		case 0:  //pt
		default:
			CssBodyFontUnit = 0;
			CssBodyFontTwips = CssBodyFontSize * 20;
			break;
	}
}


void
HTMwr::SetCssIndentTwips(void)
{
	switch (CssIndentBaseUnit) {
		case 0:  //pt
			CssIndentBaseTwips = CssIndentBaseSize * 20;
			break;
		case 1:  //pc
			CssIndentBaseTwips = CssIndentBaseSize * 240;
			break;
		case 3:  //cm
			CssIndentBaseTwips = CssIndentBaseSize * 567;
			break;
		case 4:  //mm
			CssIndentBaseTwips = (CssIndentBaseSize * 567) / 10;
			break;
		case 8:  //px
			CssIndentBaseTwips = CssIndentBaseSize * 16;
			break;
		case 2:  //in
		default:
			CssIndentBaseUnit = 2;
			CssIndentBaseTwips = CssIndentBaseSize * 1440;
			break;
	}
}


void
HTMwr::WriteCssPts(long val, csstyp use)
{
	if (val == 0) {
		CssFile->putChar('0');
		return;
	}

	char buf[15];  // convert from twips
	unc unit = 0;
	unc digs = 2;
	long refval = 240;

	switch (use) {
		case fnth:
			unit = CssFontUnits;
			digs = CssFontUnitDec;
			refval = CssBodyFontTwips;
			break;
		case pspc:
		case hpos:
			unit = CssIndentUnits;
			digs = CssIndentUnitDec;
			refval = (CssIndentUnits == 8) ? CssIndentBaseTwips : CssBodyFontTwips;
			break;
		default:
			break;
	}
 
	char *ustr = CssUnits[(unit < CssUnitsMax) ? unit : 0];
	long lval = 20;

	switch (unit) { // val is in twips
		case 1:  // picas
			lval = 240;
			break;
		case 2:  // inches
			lval = 1440;
			break;
		case 3:  // cm
			lval = 567;
			break;
		case 4:  // mm
			val *= 10;
			lval = 567;
			break;
		case 5:  // em, default 10pt
			lval = refval;
			break;
		case 6:  // ex, x-height of font, use 0.5em
			lval = refval / 2;
			break;
		case 7:  // px (90/inch)
			lval = 16;
			break;
		case 8:  // % of refval, default 10pt 
			val *= 100;
			lval = refval;
			break;
		case 0:  // pt
		default:
			break;
	}

	bool minus = false;
	if (val < 0) {
		minus = true;
		val = -val;
	}

	switch (digs) {
		case 0:
			if ((val % lval) >= (lval / 2))
				val += lval / 2;
			sprintf(buf, "%ld", val / lval);
			break;
		case 1:
			if ((((val % lval) * 10) % lval) >= (lval / 2))
				val += (lval / 20);
			sprintf(buf, "%ld.%0.1ld", val / lval, ((val % lval) * 10) / lval);
			break;
		case 2:
		default:  // for higher values
			if ((((val % lval) * 100) % lval) >= (lval / 2))
				val += (lval / 200);
			sprintf(buf, "%ld.%0.2ld", val / lval, ((val % lval) * 100) / lval);
			break;
	}
	char *per = strchr(buf, '.');
	if (per) {  // remove trailing zeros after period
		char *chp = buf + strlen(buf) - 1;
		while (*chp == '0')
			chp--;
		if (chp != per)
			chp++;
		*chp = '\0';
	}
	if (minus)
		CssFile->putChar('-');
	CssFile->putStr(buf);
	CssFile->putStr(ustr);
}



// end of dwhtmfm.cpp


