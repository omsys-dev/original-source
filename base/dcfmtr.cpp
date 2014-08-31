// dcfmtr.cpp contains code for the format ini file reading, JHG
// Copyright (c) 2009 by Omni Systems, Inc.  All rights reserved.

#include "dcglob.h"
#include "dcbase.h"
#include "dclist.h"
#include "dcnode.h"
#include "dcfile.h"
#include "dcfilr.h"
#include "dcfilw.h"
#include "dctxfx.h"
#include "dclog.h"
#include "dcfili.h"
#include "dcfmtr.h"
#include <direct.h>

#pragma warning( disable : 4189 4244 )


// public statics

fmtouttyp DCfrfile::FmtOutputType = fmoutnone;

char *DCfrfile::BodyBaseSize = NULL;
char *DCfrfile::IndentBaseWidth = NULL;
double DCfrfile::BodyBaseVal = 200.0;
double DCfrfile::IndentBaseVal = 8640.0;
bool DCfrfile::BaseValuesSet = false;

DCfrfile *DCfrfile::Formats = NULL;
DCfrfile *DCfrfile::Tables = NULL;
DCfrfile *DCfrfile::Pages = NULL;
DCfrfile *DCfrfile::SubFormats = NULL;



// public members

DCfrfile::DCfrfile(const char *fname) : DCirfile(fname)
{
}


DCfrfile *
DCfrfile::SetConfigTemplate(const char *sect, const char *setting,
		                        bool link)
{
	DCfrfile *bottomIni = SecFile ? (DCfrfile *) SecFile : this;
	char *startdir =  _getcwd(NewName(1024), 1024);
	char *currdir = startdir;
	char *newdir = NULL;
	int startdrive = _getdrive();
	int currdrive = startdrive;
	int newdrive = 0;

	IniLevel = 0;

	if (SecFile != NULL) {
		IniFileList.add(SecFile, ++IniLevel);
		IniNameList.add(SecFile->getBaseName(), IniLevel);
		IniChainList.add(SecFile->getFilePath(), IniLevel);
	}
	IniFileList.add(this, ++IniLevel);
	IniNameList.add(getBaseName(), IniLevel);
	IniChainList.add(getFilePath(), IniLevel);

	DCfrfile *configTpl = NULL;
	char *configTplName = NULL;

	// SecFile (specific ini) overrides tpl choice in this
	DCfrfile *topIni = this;

	if (Section(sect)
	 && Find("Scope"))
		Scope = StrRight();

	if (!Section(sect)) {
		if (Section("Setup"))
			sect = "Setup";
		else if (Section("FDK"))
			sect = "FDK";
	}
	while (topIni->Section(sect)) {
		if (!topIni->Find(setting))
			break;  // end of chain with no end flag ****
		if ((configTplName = topIni->StrRight()) == NULL)
			break;  // end of chain with no end flag ****
		if (!stricmp(configTplName, "end")) {
			DeleteName(configTplName);
			configTplName = NULL;
			topIni->ChainEnded = true;
			break;  // end of chain with end flag
		}
		if ((configTpl = new DCfrfile(configTplName)) == NULL) {
			LogEvent(logwarn, 1, topIni->getFilePath(), " refers to unopenable ",
				setting, " ", NewName(configTplName));
			DeleteName(configTplName);
			configTplName = NULL;
			break;  // can't open file
		}
		if (configTpl->FErr() != fok) {
			LogEvent(logwarn, 1, topIni->getFilePath(), " refers to nonexistent ",
				setting, " ", configTpl->getFilePath());
			DeleteName(configTplName);
			configTplName = NULL;
			delete configTpl;
			break;  // file not found
		}
		newdir = configTpl->getBasePath();
		if (*(newdir + 1) == ':')
			newdrive = (toupper(*newdir) - 'A') + 1;
		if (newdrive
		 && (currdrive != newdrive))
			_chdrive(currdrive = newdrive);
		if (stricmp(newdir, currdir))
			_chdir(currdir = newdir);

		if (IniNameList(configTpl->getBaseName())) {
			LogEvent(logwarn, 1, topIni->getFilePath(), " repeats ",
				setting, " ", configTpl->getFilePath());
			DeleteName(configTplName);
			configTplName = NULL;
			delete configTpl;
			break;  // deja vu, end of the line
		}

		topIni = configTpl;  // step up
		if (topIni->Section("Templates")
		 && topIni->Find("Scope"))
			topIni->Scope = topIni->StrRight();

		DeleteName(configTplName);
		configTplName = NULL;
		IniFileList.add(topIni, ++IniLevel);
		IniNameList.add(topIni->getBaseName(), IniLevel);
		IniChainList.add(topIni->getFilePath(), IniLevel);
	}

	// topIni is now at the top of the chain, and
	// bottomIni is at the bottom, which may be same.
	if (link) {
		DCfrfile *ifl = bottomIni;
		DCfrfile *ift = NULL;
		for (long ilev = 1; ilev < IniLevel; ilev++) {
			ift = (DCfrfile *) IniFileList.find(ilev + 1);
			ift->SetSecFile(ifl);
			ifl = ift;
			if (ifl == topIni)
				break;
		}
	}

	WriteIniChain(setting);

	if (currdrive != startdrive)
		_chdrive(startdrive);
	if (stricmp(startdir, currdir))
		_chdir(startdir);

	if (this != topIni) {
		topIni->IniFileList = IniFileList;
		topIni->IniNameList = IniNameList;
		topIni->IniChainList = IniChainList;
		topIni->IniLevel = IniLevel;
	}

	return topIni;
}


bool 
DCfrfile::GetSection(const char *oname)
{
	DCnlist aliasused;
	char *fname = NewName(oname);
	char *nname = NULL;
	_strlwr(fname);
	aliasused.add(fname, 1);

	while (fname) {
		if (Section("FormatAliases")
		 && Find(fname)
		 && ((nname = StrRight()) != NULL)) {
			if (aliasused(nname)) {
				LogEvent(logwarn, 1, "Format alias repeated in loop: ", nname);
				break;
			}
			aliasused.add(nname, 1);
			fname = _strlwr(nname);
		}
		else
			break;
	}

	return Section(fname);
}


bool 
DCfrfile::GetFmtSection(const char *oname)
{
	if (!Formats)
		return NULL;

	return Formats->GetSection(oname);
}


bool 
DCfrfile::GetTblSection(const char *oname)
{
	if (!Tables)
		return NULL;

	return Tables->GetSection(oname);
}


bool 
DCfrfile::GetPgSection(const char *oname)
{
	if (!Pages)
		return NULL;

	return Pages->GetSection(oname);
}


bool 
DCfrfile::GetSubSection(const char *oname)
{
	if (!SubFormats)
		return NULL;

	return SubFormats->GetSection(oname);
}



bool 
DCfrfile::CheckFormat(const char *fname)
{
	return GetFmtSection(fname);
}


bool 
DCfrfile::CheckTblFormat(const char *fname)
{
	return GetTblSection(fname);
}


bool 
DCfrfile::CheckPgFormat(const char *fname)
{
	return GetPgSection(fname);
}


fmtvals * 
DCfrfile::Format(const char *name, fmtvals *fv)
{
	if (!BaseValuesSet) {
		if (BodyBaseSize)
			BodyBaseVal = (double) GetLength(BodyBaseSize);
		if (IndentBaseWidth)
			IndentBaseVal = (double) GetLength(IndentBaseWidth);
		BaseValuesSet = true;
	}

	BasedUsed.empty();
	fmtinfo *fmt = GetFmtInfo(name);

	if (!fmt)
		return fv;

	// preferably existing values are passed in fv, so that
	// it's clear whether a setting was missing or turned off
	if (!fv)
		fv = new fmtvals;

	// convert from strings in fmtinfo to Curr values
	// all numeric sizes are converted to twips
	// colors are converted to RGB, -1 = transparent

	char *nname = NULL;
	long nval = 0;

	if (fmt->display) {
		fv->cdisp = fmt->display;
		if (!stricmp(fmt->display, "block"))
			fv->stype = 1;
		else if (!stricmp(fmt->display, "inline"))
			fv->stype = 2;
	}

	if (fmt->based)
		fv->based = fmt->based;

	if (fmt->selector)
		fv->selector = fmt->selector;

	if (fmt->color) {
		nval = GetColor(fmt->color, &fv->ccolor);
		if ((nval != 0)  // color found
		 || !fv->ccolor) // name not set if real black
			fv->color = nval;
		fv->ccolor = fmt->color;  // set name anyway
	}
 
	if (fmt->bcolor) {
		nval = GetColor(fmt->bcolor, &fv->cbcolor);
		if ((nval != 0)  // color found
		 || !fv->cbcolor) // name not set if real black
			fv->bcolor = nval;
		fv->cbcolor = fmt->bcolor;  // set name anyway
	}


	// inline formats

	if (fmt->fntname) {
		nname = GetFontName(fmt->fntname, &fv->cfont);
		if (nname)
			fv->font = nname;
	}

	CurrPercentBase = BodyBaseVal;

	if (fmt->fntsize) {
		nval = GetLength(fmt->fntsize, &fv->cfsize);
		if (nval)
			fv->fsize = nval;
	}

	if (fmt->fntstyle) {
		fv->cfstyle = fmt->fntstyle;
		if (!stricmp(fmt->fntstyle, "italic"))
			fv->fstyle = 2;
		else if (!stricmp(fmt->fntstyle, "oblique"))
			fv->fstyle = 2;
		else if (!stricmp(fmt->fntstyle, "normal"))
			fv->fstyle = 1;
		else
			fv->fstyle = 1;
	}

	if (fmt->fntvar) {
		fv->cfvar = fmt->fntvar;
		if (!stricmp(fmt->fntvar, "small-caps"))
			fv->fvar = 2;
		else if (!stricmp(fmt->fntvar, "normal"))
			fv->fvar = 1;
		else
			fv->fvar = 1;
	}

	if (fmt->fntweight) {
		fv->cfweight = fmt->fntweight;
		if (!stricmp(fmt->fntweight, "bold"))
			fv->fweight = 4;
		else if (!stricmp(fmt->fntweight, "normal"))
			fv->fweight = 0;
		else {
			if (strstr(fmt->fntweight, "100"))
				fv->fweight = 1;
			else if (strstr(fmt->fntweight, "200"))
				fv->fweight = 2;
			else if (strstr(fmt->fntweight, "300"))
				fv->fweight = 3;
			else if (strstr(fmt->fntweight, "700"))
				fv->fweight = 5;
			else if (strstr(fmt->fntweight, "900"))
				fv->fweight = 6;
			else
				fv->fweight = 0;
		}
	}

	CurrPercentBase = fv->fsize;

	if (fmt->fntkern) {
		nval = GetLength(fmt->fntkern, &fv->cfkern);
		if (nval)
			fv->fkern = nval;
	}

	CurrPercentBase = BodyBaseVal;

	if (fmt->textdecor) {
		fv->ctdecor = fmt->textdecor;
		if (strstr(fmt->textdecor, "underline"))
			fv->tdecor = 2;
		if (strstr(fmt->textdecor, "overline"))
			fv->tdecor = 3;
		if (strstr(fmt->textdecor, "line-through"))
			fv->tstrike = 2;
		if (!stricmp(fmt->textdecor, "none")) {
			fv->tdecor = 1;
			fv->tstrike = 1;
		}
	}

	if (fmt->texttrans) {
		fv->cttrans = fmt->texttrans;
		if (!stricmp(fmt->texttrans, "uppercase"))
			fv->ttrans = 2;
		else if (!stricmp(fmt->texttrans, "lowercase"))
			fv->ttrans = 3;
		else if (!stricmp(fmt->texttrans, "capitalize"))
			fv->ttrans = 4;
		else if (!stricmp(fmt->texttrans, "none"))
			fv->ttrans = 1;
		else
			fv->ttrans = 1;
	}



	// block formats

	if (fmt->section)
		fv->section = fmt->section;
	if (fmt->sectstart)
		fv->sectstart = fmt->bsectstart;
	if (fmt->sectbreak) {
		if (!stricmp(fmt->sectbreak, "column"))
			fv->sectbreak = 3;
		else if (!stricmp(fmt->sectbreak, "page"))
			fv->sectbreak = 4;
		else if (!stricmp(fmt->sectbreak, "left"))
			fv->sectbreak = 5;
		else if (!stricmp(fmt->sectbreak, "right"))
			fv->sectbreak = 6;
		else if (!stricmp(fmt->sectbreak, "none"))
			fv->sectbreak = 8;
	}
	if (fmt->pbreak) {
		if (!stricmp(fmt->pbreak, "column"))
			fv->pbreak = 3;
		else if (!stricmp(fmt->pbreak, "page"))
			fv->pbreak = 4;
		else if (!stricmp(fmt->pbreak, "left"))
			fv->pbreak = 5;
		else if (!stricmp(fmt->pbreak, "right"))
			fv->pbreak = 6;
		else if (!stricmp(fmt->pbreak, "none"))
			fv->pbreak = 8;
	}

	if (fmt->lineheight) {
		nval = GetLength(fmt->lineheight, &fv->clhigh);
		if (nval
		 || (*(fv->clhigh) == '0')) { // a real zero
			fv->lhigh = nval;
			fv->lmode = 1;
		}
	}

	if (fmt->textalign) {
		if (!stricmp(fmt->textalign, "left")) {
			fv->talign = 1;
			fv->ctalign = fmt->textalign;
		}
		else if (!stricmp(fmt->textalign, "center"))
			fv->talign = 2;
		else if (!stricmp(fmt->textalign, "right"))
			fv->talign = 3;
		else if (!stricmp(fmt->textalign, "justify")) {
			fv->talign = 4;  // treat as left internally
			fv->ctalign = fmt->textalign;
		}
		else {  // anything else means left
			fv->talign = 1;
			fv->ctalign = fmt->textalign;
		}
	}

	if (fmt->vertalign) {
		if (!stricmp(fmt->vertalign, "middle"))
			fv->valign = 1;
		else if (!stricmp(fmt->vertalign, "top"))
			fv->valign = 2;
		else if (!stricmp(fmt->vertalign, "bottom"))
			fv->valign = 3;
		else
			fv->cvalign = fmt->vertalign;
	}


	if (fmt->keep) {
		fv->keep = 0;
		if (strstr(fmt->keep, "together"))
			fv->keep = 1;
		if (strstr(fmt->keep, "next"))
			fv->keep += 2;
		if (strstr(fmt->keep, "none"))
			fv->keep = 0;
	}


	CurrPercentBase = IndentBaseVal;

	if (fmt->margleft) {
		nval = GetLength(fmt->margleft, &fv->cmargl);
		if (nval
		 || (*(fv->cmargl) == '0'))  // a real zero
			fv->margl = nval;
	}
	if (fmt->margright) {
		nval = GetLength(fmt->margright, &fv->cmargr);
		if (nval
		 || (*(fv->cmargr) == '0'))  // a real zero
			fv->margr = nval;
	}
	if (fmt->margtop) {
		nval = GetLength(fmt->margtop, &fv->cmargt);
		if (nval
		 || (*(fv->cmargt) == '0'))  // a real zero
			fv->margt = nval;
	}
	if (fmt->margbottom) {
		nval = GetLength(fmt->margbottom, &fv->cmargb);
		if (nval
		 || (*(fv->cmargb) == '0'))  // a real zero
			fv->margb = nval;
	}

	if (fmt->padleft) {
		nval = GetLength(fmt->padleft, &fv->cpadl);
		if (nval
		 || (*(fv->cpadl) == '0'))  // a real zero
			fv->padl = nval;
	}
	if (fmt->padright) {
		nval = GetLength(fmt->padright, &fv->cpadr);
		if (nval
		 || (*(fv->cpadr) == '0'))  // a real zero
			fv->padr = nval;
	}
	if (fmt->padtop) {
		nval = GetLength(fmt->padtop, &fv->cpadt);
		if (nval
		 || (*(fv->cpadt) == '0'))  // a real zero
			fv->padt = nval;
	}
	if (fmt->padbottom) {
		nval = GetLength(fmt->padbottom, &fv->cpadb);
		if (nval
		 || (*(fv->cpadb) == '0'))  // a real zero
			fv->padb = nval;
	}

	if (fmt->txindent) {
		nval = GetLength(fmt->txindent, &fv->ctind);
		if (nval
		 || (*(fv->ctind) == '0'))  // a real zero
			fv->tind = nval;
	}


	if (fmt->brdrt) {
		BorderBasedUsed.empty();
		fv->brdrt = GetBorder(fmt->brdrt);
	}
	if (fmt->brdrb) {
		BorderBasedUsed.empty();
		fv->brdrb = GetBorder(fmt->brdrb);
	}
	if (fmt->brdrl) {
		BorderBasedUsed.empty();
		fv->brdrl = GetBorder(fmt->brdrl);
	}
	if (fmt->brdrr) {
		BorderBasedUsed.empty();
		fv->brdrr = GetBorder(fmt->brdrr);
	}
	if (fmt->brdrbar) {
		BorderBasedUsed.empty();
		fv->brdrbar = GetBorder(fmt->brdrbar);
	}
	if (fmt->brdrbox) {
		BorderBasedUsed.empty();
		fv->brdrbox = GetBorder(fmt->brdrbox);
	}
	if (fmt->brdrbtw) {
		BorderBasedUsed.empty();
		fv->brdrbtw = GetBorder(fmt->brdrbtw);
	}


	if (fmt->shade) {
		ShadeBasedUsed.empty();
		fv->shade = GetShade(fmt->shade);
	}

	if (fmt->tabs) {
		fv->tabs = new DCvlist;
		fv->tcount = GetTabDefs(fmt->tabs, fv->tabs);
	}

	if (fmt->liststyle) {
		char *url = NULL;
		fv->listelem = "ol";
		if (strstr(fmt->liststyle, "disc")) {
			fv->listelem = "ul";
			fv->liststyle = "disc";
			fv->listnum = 6;
		}
		else if (strstr(fmt->liststyle, "circle")) {
			fv->listelem = "ul";
			fv->liststyle = "circle";
			fv->listnum = 7;
		}
		else if (strstr(fmt->liststyle, "square")) {
			fv->listelem = "ul";
			fv->liststyle = "square";
			fv->listnum = 8;
		}
		else if (strstr(fmt->liststyle, "nobullet")) {
			fv->listelem = "ul";
			fv->liststyle = "nobullet";
		}
		else if (strstr(fmt->liststyle, "decimal")) {
			fv->liststyle = "decimal";
			fv->listnum = 1;
		}
		else if (strstr(fmt->liststyle, "decimal-leading-zero")) {
			fv->liststyle = "decimal-leading-zero";
			fv->listnum = 1;
		}
		else if (strstr(fmt->liststyle, "upper-alpha")) {
			fv->liststyle = "upper-alpha";
			fv->listnum = 5;
		}
		else if (strstr(fmt->liststyle, "upper-latin")) {
			fv->liststyle = "upper-alpha";
			fv->listnum = 5;
		}
		else if (strstr(fmt->liststyle, "lower-alpha")) {
			fv->liststyle = "lower-alpha";
			fv->listnum = 4;
		}
		else if (strstr(fmt->liststyle, "lower-latin")) {
			fv->liststyle = "lower-alpha";
			fv->listnum = 4;
		}
		else if (strstr(fmt->liststyle, "upper-roman")) {
			fv->liststyle = "upper-roman";
			fv->listnum = 3;
		}
		else if (strstr(fmt->liststyle, "lower-roman")) {
			fv->liststyle = "lower-roman";
			fv->listnum = 2;
		}
		else if (strstr(fmt->liststyle, "lower-greek")) {
			fv->liststyle = "lower-greek";
			fv->listnum = 2;
		}
		else if (strstr(fmt->liststyle, "armenian"))
			fv->liststyle = "armenian";
		else if (strstr(fmt->liststyle, "georgian"))
			fv->liststyle = "georgian";
		else if (strstr(fmt->liststyle, "nonumber"))
			fv->liststyle = "nonumber";
		else if (strstr(fmt->liststyle, "none")) {
			fv->listelem = NULL;
			fv->liststyle = "none";
		}
		if ((url = strstr(fmt->liststyle, "url(")) != NULL) {
			char *paren = strchr(url, ')');
			if (paren)
				fv->listurl = NewName(url, paren - url);
		}
		if (strstr(fmt->liststyle, "inside"))
			fv->listpos = "inside";
		else if (strstr(fmt->liststyle, "outside"))
			fv->listpos = "outside";
	}
	if (fmt->listlevel)
		fv->listlevel = fmt->listlevel;
	if (fmt->number) {
		NumBasedUsed.empty();
		numforminfo *nf = GetNumFormat(fmt->number);
		if (nf)
			fv->listfirst = (nf->start != -1) ? true : false;
	}

	if (fmt->css)
		fv->css = Unescape(fmt->css);
	if (fmt->rtf)
		fv->rtf = Unescape(fmt->rtf);

	if (fmt->before)
		fv->before = Unescape(fmt->before);
	if (fmt->start)
		fv->start = Unescape(fmt->start);
	if (fmt->end)
		fv->end = Unescape(fmt->end);
	if (fmt->after)
		fv->after = Unescape(fmt->after);

	return fv;
}



tbfmtvals * 
DCfrfile::TableFormat(const char *name, tbfmtvals *fv)
{
	BasedUsed.empty();
	tbfmtinfo *fmt = GetTableFmtInfo(name);

	if (!fmt)
		return fv;

	// preferably existing values are passed in fv, so that
	// it's clear whether a setting was missing or turned off
	if (!fv)
		fv = new tbfmtvals;

	// convert from strings in fmtinfo to Curr values
	// all numeric sizes are converted to twips

	char *nname = NULL;
	long nval = 0;
	fv->cssclass = fmt->cssclass;

	if (fmt->brdrmodel)
		fv->brdrmodel = fmt->brdrmodel;
	if (fmt->brdrspacing)
		fv->brdrspacing = fmt->brdrspacing;
	if (fmt->emptycells)
		fv->emptycells = fmt->emptycells;
	if (fmt->tbllayout)
		fv->tbllayout = fmt->tbllayout;

	if (fmt->tbalign) {
		if (!stricmp(fmt->tbalign, "left"))
			fv->tbalign = 1;
		else if (!stricmp(fmt->tbalign, "center"))
			fv->tbalign = 2;
		else if (!stricmp(fmt->tbalign, "right"))
			fv->tbalign = 3;
		else {  // default to none
			fv->tbalign = 0;
			fv->ctbalign = fmt->tbalign;
		}
	}

	if (fmt->tbbreak) {
		if (!stricmp(fmt->tbbreak, "column"))
			fv->tbbreak = 3;
		else if (!stricmp(fmt->tbbreak, "page"))
			fv->tbbreak = 4;
		else if (!stricmp(fmt->tbbreak, "left"))
			fv->tbbreak = 5;
		else if (!stricmp(fmt->tbbreak, "right"))
			fv->tbbreak = 6;
		else if (!stricmp(fmt->tbbreak, "none"))
			fv->tbbreak = 8;
	}

	if (fmt->keep) {
		if (!stricmp(fmt->keep, "together"))
			fv->keep = 1;
		else if (!stricmp(fmt->keep, "none"))
			fv->keep = 0;
	}

	CurrPercentBase = IndentBaseVal;

	if (fmt->tbwide)
		fv->tbwide = GetLength(fmt->tbwide, &fv->ctbwide);

	CurrPercentBase = fv->tbwide ? fv->tbwide : IndentBaseVal;

	if (fmt->tbcwide) {
		fv->cwids = new DCilist;
		fv->colcount = GetColWidths(fmt->tbcwide, fv->cwids);
	}

	CurrPercentBase = IndentBaseVal;

	if (fmt->margleft) {
		nval = GetLength(fmt->margleft, &fv->cmargl);
		if (nval
		 || (*(fv->cmargl) == '0'))  // a real zero
			fv->margl = nval;
	}
	if (fmt->margtop) {
		nval = GetLength(fmt->margtop, &fv->cmargt);
		if (nval
		 || (*(fv->cmargt) == '0'))  // a real zero
			fv->margt = nval;
	}
	if (fmt->margbottom) {
		nval = GetLength(fmt->margbottom, &fv->cmargb);
		if (nval
		 || (*(fv->cmargb) == '0'))  // a real zero
			fv->margb = nval;
	}

	if (fmt->margcell) {
		nval = GetLength(fmt->margcell, &fv->ccellmarg);
		if (nval
		 || (*(fv->ccellmarg) == '0'))  // a real zero
			fv->cellmarg = nval;
	}

	if (fmt->cellmargleft) {
		nval = GetLength(fmt->cellmargleft, &fv->ccellmargl);
		if (nval
		 || (*(fv->ccellmargl) == '0'))  // a real zero
			fv->cellmargl = nval;
	}
	if (fmt->cellmargright) {
		nval = GetLength(fmt->cellmargright, &fv->ccellmargr);
		if (nval
		 || (*(fv->ccellmargr) == '0'))  // a real zero
			fv->cellmargr = nval;
	}
	if (fmt->cellmargtop) {
		nval = GetLength(fmt->cellmargtop, &fv->ccellmargt);
		if (nval
		 || (*(fv->ccellmargt) == '0'))  // a real zero
			fv->cellmargt = nval;
	}
	if (fmt->cellmargbottom) {
		nval = GetLength(fmt->cellmargbottom, &fv->ccellmargb);
		if (nval
		 || (*(fv->ccellmargb) == '0'))  // a real zero
			fv->cellmargb = nval;
	}

	if (fmt->cellpadleft) {
		nval = GetLength(fmt->cellpadleft, &fv->ccellpadl);
		if (nval
		 || (*(fv->ccellpadl) == '0'))  // a real zero
			fv->cellpadl = nval;
	}
	if (fmt->cellpadright) {
		nval = GetLength(fmt->cellpadright, &fv->ccellpadr);
		if (nval
		 || (*(fv->ccellpadr) == '0'))  // a real zero
			fv->cellpadr = nval;
	}
	if (fmt->cellpadtop) {
		nval = GetLength(fmt->cellpadtop, &fv->ccellpadt);
		if (nval
		 || (*(fv->ccellpadt) == '0'))  // a real zero
			fv->cellpadt = nval;
	}
	if (fmt->cellpadbottom) {
		nval = GetLength(fmt->cellpadbottom, &fv->ccellpadb);
		if (nval
		 || (*(fv->ccellpadb) == '0'))  // a real zero
			fv->cellpadb = nval;
	}

	if (fmt->hrows) {
		fv->hrows = new DCnlist;
		fv->hrcount = GetRowDefs(fmt->hrows, fv->hrows);
	}
	if (fmt->brows) {
		fv->brows = new DCnlist;
		fv->brcount = GetRowDefs(fmt->brows, fv->brows);
	}
	if (fmt->frows) {
		fv->frows = new DCnlist;
		fv->frcount = GetRowDefs(fmt->frows, fv->frows);
	}

	if (fmt->brdrt) {
		BorderBasedUsed.empty();
		fv->brdrt = GetBorder(fmt->brdrt);
	}
	if (fmt->brdrb) {
		BorderBasedUsed.empty();
		fv->brdrb = GetBorder(fmt->brdrb);
	}
	if (fmt->brdrl) {
		BorderBasedUsed.empty();
		fv->brdrl = GetBorder(fmt->brdrl);
	}
	if (fmt->brdrr) {
		BorderBasedUsed.empty();
		fv->brdrr = GetBorder(fmt->brdrr);
	}

	if (fmt->colbrdrl) {
		BorderBasedUsed.empty();
		fv->colbrdrl = GetBorder(fmt->colbrdrl);
	}
	if (fmt->colbrdrr) {
		BorderBasedUsed.empty();
		fv->colbrdrr = GetBorder(fmt->colbrdrr);
	}

	if (fmt->shade) {
		ShadeBasedUsed.empty();
		fv->shade = GetShade(fmt->shade);
	}

	if (fmt->css)
		fv->css = Unescape(fmt->css);
	if (fmt->rtf)
		fv->rtf = Unescape(fmt->rtf);

	if (fmt->before)
		fv->before = Unescape(fmt->before);
	if (fmt->after)
		fv->after = Unescape(fmt->after);

	return fv;
}


rowfmtvals * 
DCfrfile::RowFormat(const char *name, rowfmtvals *fv)
{
	BasedUsed.empty();
	rowfmtinfo *fmt = GetRowFmtInfo(name);

	if (!fmt)
		return fv;

	// preferably existing values are passed in fv, so that
	// it's clear whether a setting was missing or turned off
	if (!fv)
		fv = new rowfmtvals;

	// convert from strings in fmtinfo to Curr values
	// all numeric sizes are converted to twips

	char *nname = NULL;
	long nval = 0;
	fv->cssclass = fmt->cssclass;

	if (fmt->rowtype)
		fv->rowtype = GetRowType(fmt->rowtype);

	if (fmt->rowhigh)
		fv->rowhigh = GetRowHigh(fmt->rowhigh, &fv->rhtype);

	if (fmt->keep) {
		if (!stricmp(fmt->keep, "together"))
			fv->keep = 1;
		else if (!stricmp(fmt->keep, "none"))
			fv->keep = 0;
	}

	if (fmt->hcells) {
		fv->hcells = new DCnlist;
		fv->hccount = GetCellDefs(fmt->hcells, fv->hcells);
	}
	if (fmt->bcells) {
		fv->bcells = new DCnlist;
		fv->bccount = GetCellDefs(fmt->bcells, fv->bcells);
	}

	if (fmt->brdrt) {
		BorderBasedUsed.empty();
		fv->brdrt = GetBorder(fmt->brdrt);
	}
	if (fmt->brdrb) {
		BorderBasedUsed.empty();
		fv->brdrb = GetBorder(fmt->brdrb);
	}
	if (fmt->brdrx) {
		BorderBasedUsed.empty();
		fv->brdrx = GetBorder(fmt->brdrx);
	}
	if (fmt->brdrl) {
		BorderBasedUsed.empty();
		fv->brdrl = GetBorder(fmt->brdrl);
	}
	if (fmt->brdrr) {
		BorderBasedUsed.empty();
		fv->brdrr = GetBorder(fmt->brdrr);
	}

	if (fmt->shade) {
		ShadeBasedUsed.empty();
		fv->shade = GetShade(fmt->shade);
	}

	if (fmt->css)
		fv->css = Unescape(fmt->css);
	if (fmt->rtf)
		fv->rtf = Unescape(fmt->rtf);

	return fv;
}


cellfmtvals * 
DCfrfile::CellFormat(const char *name, cellfmtvals *fv)
{
	BasedUsed.empty();
	cellfmtinfo *fmt = GetCellFmtInfo(name);

	if (!fmt)
		return fv;

	// preferably existing values are passed in fv, so that
	// it's clear whether a setting was missing or turned off
	if (!fv)
		fv = new cellfmtvals;

	// convert from strings in fmtinfo to Curr values
	// all numeric sizes are converted to twips

	long nval = 0;
	fv->cssclass = fmt->cssclass;

	CurrPercentBase = IndentBaseVal;

	if (fmt->margleft) {
		nval = GetLength(fmt->margleft, &fv->cmargl);
		if (nval
		 || (*(fv->cmargl) == '0'))  // a real zero
			fv->margl = nval;
	}
	if (fmt->margright) {
		nval = GetLength(fmt->margright, &fv->cmargr);
		if (nval
		 || (*(fv->cmargr) == '0'))  // a real zero
			fv->margr = nval;
	}
	if (fmt->margtop) {
		nval = GetLength(fmt->margtop, &fv->cmargt);
		if (nval
		 || (*(fv->cmargt) == '0'))  // a real zero
			fv->margt = nval;
	}
	if (fmt->margbottom) {
		nval = GetLength(fmt->margbottom, &fv->cmargb);
		if (nval
		 || (*(fv->cmargb) == '0'))  // a real zero
			fv->margb = nval;
	}

	if (fmt->padleft) {
		nval = GetLength(fmt->padleft, &fv->cpadl);
		if (nval
		 || (*(fv->cpadl) == '0'))  // a real zero
			fv->padl = nval;
	}
	if (fmt->padright) {
		nval = GetLength(fmt->padright, &fv->cpadr);
		if (nval
		 || (*(fv->cpadr) == '0'))  // a real zero
			fv->padr = nval;
	}
	if (fmt->padtop) {
		nval = GetLength(fmt->padtop, &fv->cpadt);
		if (nval
		 || (*(fv->cpadt) == '0'))  // a real zero
			fv->padt = nval;
	}
	if (fmt->padbottom) {
		nval = GetLength(fmt->padbottom, &fv->cpadb);
		if (nval
		 || (*(fv->cpadb) == '0'))  // a real zero
			fv->padb = nval;
	}

	if (fmt->vertalign) {
		if (!stricmp(fmt->vertalign, "middle"))
			fv->valign = 1;
		else if (!stricmp(fmt->vertalign, "top"))
			fv->valign = 2;
		else if (!stricmp(fmt->vertalign, "bottom"))
			fv->valign = 3;
		else
			fv->cvalign = fmt->vertalign;
	}

	if (fmt->brdrt) {
		BorderBasedUsed.empty();
		fv->brdrt = GetBorder(fmt->brdrt);
	}
	if (fmt->brdrb) {
		BorderBasedUsed.empty();
		fv->brdrb = GetBorder(fmt->brdrb);
	}
	if (fmt->brdrl) {
		BorderBasedUsed.empty();
		fv->brdrl = GetBorder(fmt->brdrl);
	}
	if (fmt->brdrr) {
		BorderBasedUsed.empty();
		fv->brdrr = GetBorder(fmt->brdrr);
	}

	ShadeBasedUsed.empty();

	if (fmt->shade)
		fv->shade = GetShade(fmt->shade);

	if (fmt->css)
		fv->css = Unescape(fmt->css);
	if (fmt->rtf)
		fv->rtf = Unescape(fmt->rtf);

	if (fmt->start)
		fv->start = Unescape(fmt->start);
	if (fmt->end)
		fv->end = Unescape(fmt->end);

	return fv;
}


pagefmtvals * 
DCfrfile::PageFormat(const char *name, pagefmtvals *fv)
{
	BasedUsed.empty();
	pagefmtinfo *fmt = GetPageFmtInfo(name);

	if (!fmt)
		return fv;

	// preferably existing values are passed in fv, so that
	// it's clear whether a setting was missing or turned off
	if (!fv)
		fv = new pagefmtvals;

	// convert from strings in fmtinfo to Curr values
	// all numeric sizes are converted to twips

	long nval = 0;
	fv->cssclass = fmt->cssclass;

	if (fmt->twosided)
		fv->twosided = fmt->btwosided;
	if (fmt->firstright)
		fv->firstright = fmt->bfirstright;

	fv->startsection = fmt->startsection;

	if (fmt->margmirror)
		fv->margmirror = fmt->bmargmirror;
	if (fmt->pgnform) {
		if (!strnicmp(fmt->pgnform, "num", 3))
			fv->pgnform = 0;
		else if (!strnicmp(fmt->pgnform, "lcrom", 5))
			fv->pgnform = 1;
		else if (!strnicmp(fmt->pgnform, "ucrom", 5))
			fv->pgnform = 2;
		else if (!stricmp(fmt->pgnform, "lcalpha"))
			fv->pgnform = 3;
		else if (!stricmp(fmt->pgnform, "ucalpha"))
			fv->pgnform = 4;
	}
	if (fmt->pgnstart)
		fv->pgnstart = (uns) atoi(fmt->pgnstart);

	if (fmt->orient) {
		if (!stricmp(fmt->orient, "landscape"))
			fv->orient = 1;
		else if (!stricmp(fmt->orient, "portrait"))
			fv->orient = 0;
	}

	if (fmt->paperw)
		fv->paperw = (uns) GetLength(fmt->paperw, &fv->cpaperw);
	if (fmt->paperh)
		fv->paperh = (uns) GetLength(fmt->paperh, &fv->cpaperh);

	CurrPercentBase = IndentBaseVal;

	if (fmt->margleft) {
		nval = GetLength(fmt->margleft, &fv->cmargl);
		if (nval
		 || (*(fv->cmargl) == '0'))  // a real zero
			fv->margl = nval;
	}
	if (fmt->margright) {
		nval = GetLength(fmt->margright, &fv->cmargr);
		if (nval
		 || (*(fv->cmargr) == '0'))  // a real zero
			fv->margr = nval;
	}
	if (fmt->margtop) {
		nval = GetLength(fmt->margtop, &fv->cmargt);
		if (nval
		 || (*(fv->cmargt) == '0'))  // a real zero
			fv->margt = nval;
	}
	if (fmt->margbottom) {
		nval = GetLength(fmt->margbottom, &fv->cmargb);
		if (nval
		 || (*(fv->cmargb) == '0'))  // a real zero
			fv->margb = nval;
	}

	if (fmt->gutter)
		fv->gutter = (uns) GetLength(fmt->gutter, &fv->cgutter);
	if (fmt->cols)
		fv->cols = (uns) atoi(fmt->cols);
	if (fmt->colsx)
		fv->colsx = (uns) GetLength(fmt->colsx, &fv->ccolsx);
	if (fmt->linebetcol)
		fv->linebetcol = fmt->blinebetcol;

	if (fmt->vertalign) {
		if (!stricmp(fmt->vertalign, "middle"))
			fv->valign = 1;
		else if (!stricmp(fmt->vertalign, "top"))
			fv->valign = 2;
		else if (!stricmp(fmt->vertalign, "bottom"))
			fv->valign = 3;
		else
			fv->cvalign = fmt->vertalign;
	}

	if (fmt->brdrt) {
		BorderBasedUsed.empty();
		fv->brdrt = GetBorder(fmt->brdrt);
	}
	if (fmt->brdrb) {
		BorderBasedUsed.empty();
		fv->brdrb = GetBorder(fmt->brdrb);
	}
	if (fmt->brdrl) {
		BorderBasedUsed.empty();
		fv->brdrl = GetBorder(fmt->brdrl);
	}
	if (fmt->brdrr) {
		BorderBasedUsed.empty();
		fv->brdrr = GetBorder(fmt->brdrr);
	}

	if (fmt->brwraphd)
		fv->brwraphd = fmt->bbrwraphd;
	if (fmt->brwrapft)
		fv->brwrapft = fmt->bbrwrapft;

	if (fmt->headery)
		fv->headery = (uns) GetLength(fmt->headery, &fv->cheadery);
	if (fmt->footery)
		fv->footery = (uns) GetLength(fmt->footery, &fv->cfootery);

	if (fmt->css)
		fv->css = Unescape(fmt->css);
	if (fmt->rtf)
		fv->rtf = Unescape(fmt->rtf);

	return fv;
}


sectfmtvals * 
DCfrfile::SectFormat(const char *name, sectfmtvals *fv)
{
	BasedUsed.empty();
	sectfmtinfo *fmt = GetSectFmtInfo(name);

	if (!fmt)
		return fv;

	// preferably existing values are passed in fv, so that
	// it's clear whether a setting was missing or turned off
	if (!fv)
		fv = new sectfmtvals;

	// convert from strings in fmtinfo to Curr values
	// all numeric sizes are converted to twips

	long nval = 0;
	fv->cssclass = fmt->cssclass;

	if (fmt->startside) {
		if (!stricmp(fmt->startside, "right"))
			fv->startside = 1;
		else if (!stricmp(fmt->startside, "left"))
			fv->startside = 2;
		else if (!stricmp(fmt->startside, "page"))
			fv->startside = 0;
	}

	if (fmt->margmirror)
		fv->margmirror = fmt->bmargmirror;

	if (fmt->pgnform) {
		if (!strnicmp(fmt->pgnform, "num", 3))
			fv->pgnform = 0;
		else if (!strnicmp(fmt->pgnform, "lcrom", 5))
			fv->pgnform = 1;
		else if (!strnicmp(fmt->pgnform, "ucrom", 5))
			fv->pgnform = 2;
		else if (!stricmp(fmt->pgnform, "lcalpha"))
			fv->pgnform = 3;
		else if (!stricmp(fmt->pgnform, "ucalpha"))
			fv->pgnform = 4;
	}
	if (fmt->pgnstart)
		fv->pgnstart = (uns) atoi(fmt->pgnstart);

	if (fmt->orient) {
		if (!stricmp(fmt->orient, "landscape"))
			fv->lndscp = 1;
		else if (!stricmp(fmt->orient, "portrait"))
			fv->lndscp = 0;
	}

	if (fmt->paperw)
		fv->paperw = (uns) GetLength(fmt->paperw, &fv->cpaperw);
	if (fmt->paperh)
		fv->paperh = (uns) GetLength(fmt->paperh, &fv->cpaperh);

	CurrPercentBase = IndentBaseVal;

	if (fmt->margleft) {
		nval = GetLength(fmt->margleft, &fv->cmargl);
		if (nval
		 || (*(fv->cmargl) == '0'))  // a real zero
			fv->margl = nval;
	}
	if (fmt->margright) {
		nval = GetLength(fmt->margright, &fv->cmargr);
		if (nval
		 || (*(fv->cmargr) == '0'))  // a real zero
			fv->margr = nval;
	}
	if (fmt->margtop) {
		nval = GetLength(fmt->margtop, &fv->cmargt);
		if (nval
		 || (*(fv->cmargt) == '0'))  // a real zero
			fv->margt = nval;
	}
	if (fmt->margbottom) {
		nval = GetLength(fmt->margbottom, &fv->cmargb);
		if (nval
		 || (*(fv->cmargb) == '0'))  // a real zero
			fv->margb = nval;
	}

	if (fmt->gutter)
		fv->gutter = (uns) GetLength(fmt->gutter, &fv->cgutter);
	if (fmt->cols)
		fv->cols = (uns) atoi(fmt->cols);
	if (fmt->colsx)
		fv->colsx = (uns) GetLength(fmt->colsx, &fv->ccolsx);
	if (fmt->linebetcol)
		fv->linebetcol = fmt->blinebetcol;

	if (fmt->vertalign) {
		if (!stricmp(fmt->vertalign, "middle"))
			fv->valign = 1;
		else if (!stricmp(fmt->vertalign, "top"))
			fv->valign = 2;
		else if (!stricmp(fmt->vertalign, "bottom"))
			fv->valign = 3;
		else
			fv->cvalign = fmt->vertalign;
	}

	if (fmt->brdrt) {
		BorderBasedUsed.empty();
		fv->brdrt = GetBorder(fmt->brdrt);
	}
	if (fmt->brdrb) {
		BorderBasedUsed.empty();
		fv->brdrb = GetBorder(fmt->brdrb);
	}
	if (fmt->brdrl) {
		BorderBasedUsed.empty();
		fv->brdrl = GetBorder(fmt->brdrl);
	}
	if (fmt->brdrr) {
		BorderBasedUsed.empty();
		fv->brdrr = GetBorder(fmt->brdrr);
	}

	if (fmt->brwraphd)
		fv->brwraphd = fmt->bbrwraphd;
	if (fmt->brwrapft)
		fv->brwrapft = fmt->bbrwrapft;

	if (fmt->headery)
		fv->headery = (uns) GetLength(fmt->headery, &fv->cheadery);
	if (fmt->footery)
		fv->footery = (uns) GetLength(fmt->footery, &fv->cfootery);

	if (fmt->titlepg)
		fv->titlepg = fmt->btitlepg;
	if (fmt->titlespa)
		fv->titlespa = GetLength(fmt->titlespa, &fv->ctitlespa);

	if (fmt->hdb) {
		HFBasedUsed.empty();
		fv->hdb = GetHFForm(fmt->hdb);
	}
	if (fmt->ftb) {
		HFBasedUsed.empty();
		fv->ftb = GetHFForm(fmt->ftb);
	}
	if (fmt->hdf) {
		HFBasedUsed.empty();
		fv->hdf = GetHFForm(fmt->hdf);
	}
	if (fmt->ftf) {
		HFBasedUsed.empty();
		fv->ftf = GetHFForm(fmt->ftf);
	}
	if (fmt->hdl) {
		HFBasedUsed.empty();
		fv->hdl = GetHFForm(fmt->hdl);
	}
	if (fmt->ftl) {
		HFBasedUsed.empty();
		fv->ftl = GetHFForm(fmt->ftl);
	}
	if (fmt->hdr) {
		HFBasedUsed.empty();
		fv->hdr = GetHFForm(fmt->hdr);
	}
	if (fmt->ftr) {
		HFBasedUsed.empty();
		fv->ftr = GetHFForm(fmt->ftr);
	}

	if (fmt->css)
		fv->css = Unescape(fmt->css);
	if (fmt->rtf)
		fv->rtf = Unescape(fmt->rtf);

	return fv;
}



DCnlist DCfrfile::BorderBasedUsed;

brinfo *
DCfrfile::GetBorder(char *brfmt)
{
	// read border format, may be based
	brinfo *fmt = NULL;
	brinfo *nfmt = NULL;
	char *bname = NULL;

	if (!GetSubSection(brfmt))
		return NULL;

	if (SubFormats->Find("based")) {
		bname = SubFormats->StrRight();
		if (BorderBasedUsed(bname))
			LogEvent(logwarn, 1, "Based border format repeated: ", bname);
		else {
			BorderBasedUsed.add(bname, 1);
			nfmt = SubFormats->GetBorder(bname);
			if (!nfmt)
				LogEvent(logwarn, 1, "Based border format not found: ", bname);
			else
				fmt = nfmt;
			SubFormats->GetSubSection(brfmt);
		}
	}

	if (!fmt)
		fmt = new brinfo;

	char *str = NULL;
	char *nstr = NULL;
	long nval = 0;

	if (SubFormats->Find("color")) {
		str = SubFormats->StrRight();
		if (str) {
			nval = GetColor(str, &nstr);
			if ((nval != 0)  // color found
			 || !nstr) // name not set if real black
				fmt->color = nval;
		}
	}

	if (SubFormats->Find("type")) {
		str = SubFormats->StrRight();
		if (!stricmp(str, "single"))
			fmt->type = brs;
		else if (!stricmp(str, "solid"))
			fmt->type = brs;
		else if (!stricmp(str, "shadow"))
			fmt->type = brsh;
		else if (!stricmp(str, "double"))
			fmt->type = brdb;
		else if (!stricmp(str, "dot"))
			fmt->type = brdot;
		else if (!stricmp(str, "dotted"))
			fmt->type = brdot;
		else if (!stricmp(str, "dash"))
			fmt->type = brdash;
		else if (!stricmp(str, "dashed"))
			fmt->type = brdash;
		else if (!stricmp(str, "hairline"))
			fmt->type = brhair;
		else if (!stricmp(str, "ridge"))
			fmt->type = brridge;
		else if (!stricmp(str, "outset"))
			fmt->type = broutset;
		else if (!stricmp(str, "groove"))
			fmt->type = brgroove;
		else if (!stricmp(str, "inset"))
			fmt->type = brinset;
		else if (!stricmp(str, "hidden"))
			fmt->type = brhidden;
		else {
			fmt->type = brs;
			LogEvent(logwarn, 2, "Border type \"", str, "\" unknown for ", brfmt);
		}
	}

	if (SubFormats->Find("thick")) {
		str = SubFormats->StrRight();
		if (str) {
			nval = GetLength(str, &nstr);
			if (nval
			 || (*nstr == '0'))  // a real zero
				fmt->thick = nval;
		}
	}

	if (SubFormats->Find("space")) {
		str = SubFormats->StrRight();
		if (str) {
			nval = GetLength(str, &nstr);
			if (nval
			 || (*nstr == '0'))  // a real zero
				fmt->space = nval;
		}
	}

	if (FmtOutputType == fmoutrtf) {
		if (fmt->thick > 75) {
			if (fmt->type == brs) {
				fmt->type = brth;
				fmt->thick /= 2;
			}
			if (fmt->thick > 75) {
				fmt->thick = 75;
				LogEvent(logwarn, 2, "Border thickness excessive for ", brfmt);
			}
		}
	}

	return fmt;
}


DCnlist DCfrfile::ShadeBasedUsed;

shinfo *
DCfrfile::GetShade(char *shfmt)
{
	// read shading format, may be based
	shinfo *fmt = NULL;
	shinfo *nfmt = NULL;
	char *bname = NULL;

	if (!GetSubSection(shfmt))
		return NULL;

	if (SubFormats->Find("based")) {
		bname = SubFormats->StrRight();
		if (ShadeBasedUsed(bname))
			LogEvent(logwarn, 1, "Based shading format repeated: ", bname);
		else {
			ShadeBasedUsed.add(bname, 1);
			nfmt = SubFormats->GetShade(bname);
			if (!nfmt)
				LogEvent(logwarn, 1, "Based shading format not found: ", bname);
			else
				fmt = nfmt;
			SubFormats->GetSubSection(shfmt);
		}
	}

	if (!fmt) {
		fmt = new shinfo;
		fmt->type = shnone;
		fmt->color = 0;
		fmt->bcolor = 0xFFFFFF;
		fmt->tint = 10000;
	}		

	char *str = NULL;
	char *nstr = NULL;
	long nval = 0;

	if (SubFormats->Find("color")) {
		str = SubFormats->StrRight();
		if (str) {
			nval = GetColor(str, &nstr);
			if ((nval != 0)  // color found
			 || !nstr) // name not set if real black
				fmt->color = nval;
		}
	}

	if (SubFormats->Find("background color")) {
		str = SubFormats->StrRight();
		if (str) {
			nval = GetColor(str, &nstr);
			if ((nval != 0)  // color found
			 || !nstr) // name not set if real black
				fmt->bcolor = nval;
		}
	}

	if (SubFormats->Find("type")) {
		str = SubFormats->StrRight();
		if (!stricmp(str, "horiz"))
			fmt->type = shhoriz;
		else if (!stricmp(str, "vert"))
			fmt->type = shvert;
		else if (!stricmp(str, "fdiag"))
			fmt->type = shfdiag;
		else if (!stricmp(str, "bdiag"))
			fmt->type = shbdiag;
		else if (!stricmp(str, "cross"))
			fmt->type = shcross;
		else if (!stricmp(str, "dcross"))
			fmt->type = shdcross;
		else if (!stricmp(str, "dkhoriz"))
			fmt->type = shdkhoriz;
		else if (!stricmp(str, "dkvert"))
			fmt->type = shdkvert;
		else if (!stricmp(str, "dkfdiag"))
			fmt->type = shdkfdiag;
		else if (!stricmp(str, "dkbdiag"))
			fmt->type = shdkbdiag;
		else if (!stricmp(str, "dkcross"))
			fmt->type = shdkcross;
		else if (!stricmp(str, "dkdcross"))
			fmt->type = shdkdcross;
		else
			LogEvent(logwarn, 2, "Shading type \"", str, "\" unknown for ", shfmt);
	}

	if (SubFormats->Find("tint")) {
		str = SubFormats->StrRight();
		if (str) {
			nval = GetPercent(str, &nstr);
			if (nval
			 || (*nstr == '0'))  // a real zero
				fmt->tint = nval;
		}
	}
	return fmt;
}


// numbering


char *
DCfrfile::GetNumFmtName(const char *fmtname)
{
	if (!fmtname)
		return NULL;

	BasedUsed.empty();
	fmtinfo *fmt = GetFmtInfo(fmtname);
	if (!fmt)
		return NULL;

	return fmt->number;
}


char *
DCfrfile::GetNumRefFmtName(const char *fmtname)
{
	if (!fmtname)
		return NULL;

	BasedUsed.empty();
	fmtinfo *fmt = GetFmtInfo(fmtname);
	if (!fmt)
		return NULL;

	return fmt->numref;
}



DCnlist DCfrfile::NumBasedUsed;

numforminfo *
DCfrfile::GetNumFormat(char *numfmt)
{
	// read number format, may be based
	numforminfo *nf = NULL;
	numforminfo *nfmt = NULL;
	char *bname = NULL;

	if (!strcmp(numfmt, "none"))
		return NULL;

	if (!GetSubSection(numfmt))
		return NULL;

	if (SubFormats->Find("based")) {
		bname = SubFormats->StrRight();
		if (NumBasedUsed(bname))
			LogEvent(logwarn, 1, "Based number format repeated: ", bname);
		else {
			NumBasedUsed.add(bname, 1);
			nfmt = SubFormats->GetNumFormat(bname);
			if (!nfmt)
				LogEvent(logwarn, 1, "Based number format not found: ", bname);
			else
				nf = nfmt;
			SubFormats->GetSubSection(numfmt);
		}
	}

	if (!nf) {
		nf = new numforminfo;
		nf->start = -1;
		nf->xrefpx = nf->xrefsx = true;
		nf->xreftab = false;
	}

	nf->name = numfmt;
	nf->based = bname;

	if (SubFormats->Find("stream"))
		nf->sname = SubFormats->StrRight();

	if (SubFormats->Find("counter"))
		nf->level = SubFormats->IntRight();

	if (SubFormats->Find("position")) {
		char *pos = SubFormats->StrRight();
		if (pos && *pos) {
			nf->atend = !stricmp(pos, "end") ? true : false;
			DeleteName(pos);
		}
	}

	if (SubFormats->Find("start"))
		nf->start = SubFormats->IntRight();

	if (SubFormats->Find("keep")) {
		long *keeps = SubFormats->IntArrRight();
		while (*keeps)
			nf->keep.add(*keeps++);
	}

	if (SubFormats->Find("form"))
		nf->form = SubFormats->StrRight();

	if (SubFormats->Find("name"))
		nf->lname = SubFormats->StrRight();

	return nf;
}


DCnlist DCfrfile::NumStrBasedUsed;

numstrinfo *
DCfrfile::GetNumStream(char *numstrfmt)
{
	// read stream format, may be based
	numstrinfo *sf = NULL;
	numstrinfo *sfmt = NULL;
	char *bname = NULL;

	if (!numstrfmt
	 || !GetSubSection(numstrfmt))
		return NULL;

	if (SubFormats->Find("based")) {
		bname = SubFormats->StrRight();
		if (NumStrBasedUsed(bname))
			LogEvent(logwarn, 1, "Based number stream repeated: ", bname);
		else {
			NumStrBasedUsed.add(bname, 1);
			sfmt = SubFormats->GetNumStream(bname);
			if (!sfmt)
				LogEvent(logwarn, 1, "Based number stream not found: ", bname);
			else
				sf = sfmt;
			SubFormats->GetSubSection(numstrfmt);
		}
	}

	if (!sf) {
		sf = new numstrinfo;
		sf->seqid = "X";
		sf->levels = 1;
		sf->levseqs = new DCilist;
		sf->levstrts = new DCilist;
		sf->levtyps = new DCilist;
		sf->tcount = 1;
	}

	sf->name = numstrfmt;

	if (SubFormats->Find("id"))
		sf->seqid = SubFormats->StrRight();

	if (SubFormats->Find("counters"))
		sf->levels = SubFormats->IntRight();

	if (SubFormats->Find("repeat"))
		sf->rep = !stricmp(SubFormats->StrRight(), "all") ? true : false;

	long levs = 1;
	nlevtyp typ = ntNum;

	if (SubFormats->Find("counter types")) {
		char **ltyps = SubFormats->StrArrRight();
		if (ltyps) {
			while (*ltyps) {
				if (!stricmp(*ltyps, "num"))
					typ = ntNum;
				else if (!stricmp(*ltyps, "ucalpha"))
					typ = ntUCAlpha;
				else if (!stricmp(*ltyps, "lcalpha"))
					typ = ntLCAlpha;
				else if (!stricmp(*ltyps, "ucrom"))
					typ = ntUCRom;
				else if (!stricmp(*ltyps, "lcrom"))
					typ = ntLCRom;
				else if (!stricmp(*ltyps, "sym"))
					typ = ntSym;
				sf->levtyps->add(typ, levs++);
				ltyps++;
			}
			sf->tcount = --levs;
		}
		else
			sf->levtyps->add(ntNum, 1);
	}
	else if (!sf->levtyps->count())
		sf->levtyps->add(ntNum, 1);

	while (levs < sf->levels) {
		typ = (nlevtyp) ((sf->rep && (sf->tcount > 1))
						? sf->levtyps->find((levs % sf->tcount) + 1)
						: sf->levtyps->find(sf->tcount));
		sf->levtyps->add(typ, ++levs);
	}

	if (SubFormats->Find("symbols")) {
		sf->syms = SubFormats->StrArrRight();
		sf->scount = 0;
		if (sf->syms) {
			char **syms = sf->syms;
			while (*syms++)
				sf->scount++;
		}
	}

	if (SubFormats->Find("reset")) {  // for footnotes
		char *res = SubFormats->StrRight();
		if (res && *res) {
			if (!stricmp(res, "none"))
				sf->reset = rtNone;
			else if (!stricmp(res, "topic"))
				sf->reset = rtTopic;
			else if (!stricmp(res, "table"))
				sf->reset = rtTable;
		}
	}

	return sf;
}


char *
DCfrfile::GetXrefFmtName(char *fmtname)
{
	if (!fmtname)
		return NULL;

#if 0
	long fnum = FmtName(fmtname);
	if (!fnum) {
		if (!GetFmtInfo(fmtname))
			return NULL;
		fnum = FmtName(fmtname);
	}

	fmtinfo *fmt = (fmtinfo *) FmtInfo.find(fnum);
	if (!fmt)
		return NULL;
#endif

	BasedUsed.empty();
	fmtinfo *fmt = GetFmtInfo(fmtname);
	if (!fmt)
		return NULL;

	return fmt->xref;
}


char *
DCfrfile::GetXrefFmtForm(char *xfmtname)
{
	if (!xfmtname)
		return NULL;

	long xfnum = XrefName(xfmtname);
	if (!xfnum) {
		XrefBasedUsed.empty();
		if (!GetXrefFmtInfo(xfmtname))
			return NULL;
		xfnum = XrefName(xfmtname);
	}

	return XrefForm.find(xfnum);
}


char *
DCfrfile::GetIndexFmtForm(char *fmtname)
{
	if (!fmtname)
		return NULL;

	long fnum = IndexName(fmtname);
	if (!fnum) {
		IndexBasedUsed.empty();
		if (!GetIndexFmtInfo(fmtname))
			return NULL;
		fnum = IndexName(fmtname);
	}

	return IndexForm.find(fnum);
}


char *
DCfrfile::GetMarkFmtForm(char *fmtname)
{
	if (!fmtname)
		return NULL;

	long fnum = MarkName(fmtname);
	if (!fnum) {
		MarkBasedUsed.empty();
		if (!GetMarkFmtInfo(fmtname))
			return NULL;
		fnum = MarkName(fmtname);
	}

	return MarkForm.find(fnum);
}


char *
DCfrfile::GetRuninFmtName(char *fmtname)
{
	if (!fmtname)
		return NULL;

	BasedUsed.empty();
	fmtinfo *fmt = GetFmtInfo(fmtname);
	if (!fmt
	 || !fmt->runin
	 || !stricmp(fmt->runin, "none"))
		return NULL;

	return fmt->runin;
}


char *
DCfrfile::GetRuninFmtForm(char *rfmtname)
{
	if (!rfmtname)
		return NULL;

	long rfnum = RuninName(rfmtname);
	if (!rfnum) {
		RuninBasedUsed.empty();
		if (!GetRuninFmtInfo(rfmtname))
			return NULL;
		rfnum = RuninName(rfmtname);
	}

	return RuninForm.find(rfnum);
}



long 
DCfrfile::GetTabDefs(char *str, DCvlist *tdef)
{
	if (!str || !tdef)
		return 0;

	long tcnt = 0;
	tabinfo *tab = NULL;
	unc lead = ' ';
	unc type = 0;
	long pos = 0;

	while (*str) {
		while (*str && isspace(*str))
			str++;
		if (!strnicmp(str, "dot ", 4)) {
			lead = '.';
			str += 4;
		}
		else if (!strnicmp(str, "hyph ", 5)) {
			lead = '-';
			str += 5;
		}
		else if (!strnicmp(str, "ul ", 3)) {
			lead = '_';
			str += 3;
		}
		else if (!strnicmp(str, "th ", 3)) {
			lead = '~';
			str += 3;
		}
		else if (!strnicmp(str, "eq ", 3)) {
			lead = '=';
			str += 3;
		}
		else if ((*str == 'c') || (*str == 'C')) {
			type = 1;
			str++;
		}
		else if ((*str == 'r') || (*str == 'R')) {
			type = 2;
			str++;
		}
		else if ((*str == 'd') || (*str == 'D')) {
			type = 3;
			str++;
		}
		else if ((*str == 'b') || (*str == 'B')) {
			type = 9;
			str++;
		}
		else if (isdigit(*str) || (*str == '.')) {
			char *val = str;
			while (*str && (*str != ' '))
				str++;
			val = NewName(val, str - val);
			pos = GetLength(val);
			DeleteName(val);
			tab = new tabinfo;
			tab->tlead = lead;
			tab->ttype = type;
			tab->tpos = (short) pos;
			tdef->add(tab, ++tcnt);
			tab = NULL;
			lead = ' ';
			type = 0;
			pos = 0;
		}
	}

	return tcnt;
}


DCnlist DCfrfile::HFBasedUsed;


hfinfo *
DCfrfile::GetHFForm(char *hffmt)
{
	// read header/footer format, may be based
	hfinfo *fmt = NULL;
	hfinfo *nfmt = NULL;
	char *bname = NULL;

	if (!GetPgSection(hffmt))
		return NULL;

	if (Pages->Find("based")) {
		bname = Pages->StrRight();
		if (HFBasedUsed(bname))
			LogEvent(logwarn, 1, "Based header/footer format repeated: ", bname);
		else {
			HFBasedUsed.add(bname, 1);
			nfmt = Pages->GetHFForm(bname);
			if (!nfmt)
				LogEvent(logwarn, 1, "Based header/footer format not found: ", bname);
			else
				fmt = nfmt;
			Pages->GetPgSection(hffmt);
		}
	}

	if (!fmt) {
		fmt = new hfinfo;
		fmt->name = hffmt;
		fmt->fmtname = NULL;
		fmt->cont = NULL;
	}
	else
		fmt->name = hffmt;

	char *str = NULL;
	char *nstr = NULL;
	long nval = 0;

	if (Pages->Find("format"))
		fmt->fmtname = Pages->StrRight();

	if (Pages->Find("content")) {
		str = Pages->StrRight();
		if (str)
			fmt->cont = Unescape(str);
	}

	return fmt;
}


char *
DCfrfile::Unescape(char *chp)
{
	// process backslash escapes
	char *dst = NewName(strlen(chp));
	char *ret = dst;
	while (*chp) {
		if (*chp == '\\') {
			switch (*++chp) {;
				case 'n':
				case 'r':
					*dst++ = '\n';
					break;
				case '~':
					*dst++ = ' ';
					break;
				default:
					*dst++ = *chp;
					break;
			}
			chp++;
		}
		else
			*dst++ = *chp++;
	}
	*dst = '\0';
	return ret;
}



/*
// CSS writing:
CSS macro = .cssrep, .maccss

display = .stype

color = .colr
background color = .bcolr
font family = .fnum/FontNameList (single only)
font size = .psize
font style = .ital
font variant = .scaps
font weight = .fweight
text decoration = .line, .strike
text transform = .caps

line height = .lspace, .lsmode
text align = .algn (lcr = 123)
vertical align = .valgn (mtb = 123)
margin lrtb = .lind, .rind, .bspace, .aspace
text indent = .lind, .find

css = .cssadd
rtf = .rtfadd

// formats also has:
based = .stype

before = .maccb, .cmaccb
start = .maccs, .cmaccs
end = .macce, .cmacce
after = .macca, .cmacca

*/


// protected members


long DCfrfile::FmtNums = 0;
DCnlist DCfrfile::FmtName;  // fmt names (block/inline) by FmtNums
DCvlist DCfrfile::FmtInfo;  // fmtinfo by FmtNums

DCnlist DCfrfile::BasedUsed;


fmtinfo *
DCfrfile::GetFmtInfo(const char *fname)
{
	//long fnum = FmtName(fname);
	//if (fnum)
	//	return (fmtinfo *) FmtInfo.find(fnum);

	fmtinfo *fmt = NULL;
	fmtinfo *nfmt = NULL;
	char *bname = NULL;

	if (!GetFmtSection(fname))
		return NULL;

	if (Find("based")) {
		bname = StrRight();
		if (BasedUsed(bname))
			LogEvent(logwarn, 1, "Based format repeated: ", bname);
		else {
			BasedUsed.add(bname, 1);
			nfmt = GetFmtInfo(bname);
			if (!nfmt)
				LogEvent(logwarn, 1, "Based format not found: ", bname);
			else
				fmt = nfmt;
			GetFmtSection(fname);
		}
	}

	if (Find("inline")) {
		bname = StrRight();
		if (BasedUsed(bname))
			LogEvent(logwarn, 1, "Based format repeated as inline: ", bname);
		else {
			BasedUsed.add(bname, 1);
			nfmt = GetFmtInfo(bname);
			if (!nfmt)
				LogEvent(logwarn, 1, "Inline format not found: ", bname);
			else
				fmt = nfmt;
			GetFmtSection(fname);
		}
	}

	if (!fmt)
		fmt = new fmtinfo;

	//FmtName.addnew(fname, ++FmtNums); // even if not found

	//FmtInfo.add(fmt, FmtNums);  // something found, cache it

	fmt->based = bname;  // store last format used, for RTF

	// fill in rest of fmtinfo from ini

	if (Find("display"))
		fmt->display = StrRight();
	if (Find("selector"))
		fmt->selector = StrRight();
	if (Find("color"))
		fmt->color = StrRight();
	if (Find("background color"))
		fmt->bcolor = StrRight();

	if (Find("font name"))
		fmt->fntname = StrRight();
	if (Find("font family"))
		fmt->fntname = StrRight();
	if (Find("font size"))
		fmt->fntsize = StrRight();
	if (Find("font style"))
		fmt->fntstyle = StrRight();
	if (Find("font variant"))
		fmt->fntvar = StrRight();
	if (Find("font weight"))
		fmt->fntweight = StrRight();
	if (Find("font kern"))
		fmt->fntkern = StrRight();
	if (Find("text decoration"))
		fmt->textdecor = StrRight();
	if (Find("text transform"))
		fmt->texttrans = StrRight();

	if (Find("section"))
		fmt->section = StrRight();
	if (Find("section start")) {
		fmt->sectstart = StrRight();
		fmt->bsectstart = BoolRight();
	}
	if (Find("section break"))
		fmt->sectbreak = StrRight();
	if (Find("break"))
		fmt->pbreak = StrRight();

	if (Find("line height"))
		fmt->lineheight = StrRight();
	if (Find("text align"))
		fmt->textalign = StrRight();
	if (Find("vertical align"))
		fmt->vertalign = StrRight();
	if (Find("keep"))
		fmt->keep = StrRight();

	if (Find("margin left"))
		fmt->margleft = StrRight();
	if (Find("margin right"))
		fmt->margright = StrRight();
	if (Find("margin top"))
		fmt->margtop = StrRight();
	if (Find("margin bottom"))
		fmt->margbottom = StrRight();
	if (Find("padding left"))
		fmt->padleft = StrRight();
	if (Find("padding right"))
		fmt->padright = StrRight();
	if (Find("padding top"))
		fmt->padtop = StrRight();
	if (Find("padding bottom"))
		fmt->padbottom = StrRight();
	if (Find("text indent"))
		fmt->txindent = StrRight();

	if (Find("tabs"))
		fmt->tabs = StrRight();

	if (Find("border top"))
		fmt->brdrt = StrRight();
	if (Find("border bottom"))
		fmt->brdrb = StrRight();
	if (Find("border left"))
		fmt->brdrl = StrRight();
	if (Find("border right"))
		fmt->brdrr = StrRight();
	if (Find("border outer"))
		fmt->brdrbar = StrRight();
	if (Find("border box"))
		fmt->brdrbox = StrRight();
	if (Find("border between"))
		fmt->brdrbtw = StrRight();

	if (Find("shading"))
		fmt->shade = StrRight();

	if (Find("list style"))
		fmt->liststyle = StrRight();
	if (Find("list level"))
		fmt->listlevel = IntRight();

	if (Find("xref"))
		fmt->xref = StrRight();
	if (Find("runin"))
		fmt->runin = StrRight();
	if (Find("number"))
		fmt->number = StrRight();
	if (Find("numref"))
		fmt->numref = StrRight();

	if (Find("css"))
		fmt->css = StrRight();
	if (Find("rtf"))
		fmt->rtf = StrRight();

	if (Find("before"))
		fmt->before = StrRight();
	if (Find("start"))
		fmt->start = StrRight();
	if (Find("end"))
		fmt->end = StrRight();
	if (Find("after"))
		fmt->after = StrRight();

	return fmt;
}


tbfmtinfo *
DCfrfile::GetTableFmtInfo(const char *fname)
{
	tbfmtinfo *fmt = NULL;
	tbfmtinfo *nfmt = NULL;
	char *bname = NULL;

	if (!GetTblSection(fname))
		return NULL;

	if (Find("based")) {
		bname = StrRight();
		if (BasedUsed(bname))
			LogEvent(logwarn, 1, "Based format repeated: ", bname);
		else {
			BasedUsed.add(bname, 1);
			nfmt = GetTableFmtInfo(bname);
			if (!nfmt)
				LogEvent(logwarn, 1, "Based format not found: ", bname);
			else
				fmt = nfmt;
			GetTblSection(fname);
		}
	}

	if (!fmt)
		fmt = new tbfmtinfo;

	fmt->cssclass = NewName(fname);
	fmt->based = bname;  // store last format used, for RTF

	// fill in rest of fmtinfo from ini

	if (Find("border model"))
		fmt->brdrmodel = StrRight();
	if (Find("border spacing"))
		fmt->brdrspacing = StrRight();
	if (Find("empty cells"))
		fmt->emptycells = StrRight();
	if (Find("table layout"))
		fmt->tbllayout = StrRight();

	if (Find("table align"))
		fmt->tbalign = StrRight();
	if (Find("table break"))
		fmt->tbbreak = StrRight();
	if (Find("keep"))
		fmt->keep = StrRight();

	if (Find("column widths"))
		fmt->tbcwide = StrRight();
	if (Find("table width"))
		fmt->tbwide = StrRight();

	if (Find("margin left"))
		fmt->margleft = StrRight();
	if (Find("margin top"))
		fmt->margtop = StrRight();
	if (Find("margin bottom"))
		fmt->margbottom = StrRight();
	if (Find("cell margins"))
		fmt->margcell = StrRight();

	if (Find("cell margin left"))
		fmt->cellmargleft = StrRight();
	if (Find("cell margin right"))
		fmt->cellmargright = StrRight();
	if (Find("cell margin top"))
		fmt->cellmargtop = StrRight();
	if (Find("cell margin bottom"))
		fmt->cellmargbottom = StrRight();

	if (Find("cell padding left"))
		fmt->cellpadleft = StrRight();
	if (Find("cell padding right"))
		fmt->cellpadright = StrRight();
	if (Find("cell padding top"))
		fmt->cellpadtop = StrRight();
	if (Find("cell padding bottom"))
		fmt->cellpadbottom = StrRight();

	if (Find("header rows"))
		fmt->hrows = StrRight();
	if (Find("body rows"))
		fmt->brows = StrRight();
	if (Find("footer rows"))
		fmt->frows = StrRight();

	if (Find("border top"))
		fmt->brdrt = StrRight();
	if (Find("border bottom"))
		fmt->brdrb = StrRight();
	if (Find("border left"))
		fmt->brdrl = StrRight();
	if (Find("border right"))
		fmt->brdrr = StrRight();
	if (Find("column border left"))
		fmt->colbrdrl = StrRight();
	if (Find("column border right"))
		fmt->colbrdrr = StrRight();

	if (Find("shading"))
		fmt->shade = StrRight();

	if (Find("css"))
		fmt->css = StrRight();
	if (Find("rtf"))
		fmt->rtf = StrRight();

	if (Find("before"))
		fmt->before = StrRight();
	if (Find("after"))
		fmt->after = StrRight();

	return fmt;
}


rowfmtinfo *
DCfrfile::GetRowFmtInfo(const char *fname)
{
	rowfmtinfo *fmt = NULL;
	rowfmtinfo *nfmt = NULL;
	char *bname = NULL;

	if (!GetTblSection(fname))
		return NULL;

	if (Find("based")) {
		bname = StrRight();
		if (BasedUsed(bname))
			LogEvent(logwarn, 1, "Based format repeated: ", bname);
		else {
			BasedUsed.add(bname, 1);
			nfmt = GetRowFmtInfo(bname);
			if (!nfmt)
				LogEvent(logwarn, 1, "Based format not found: ", bname);
			else
				fmt = nfmt;
			GetTblSection(fname);
		}
	}

	if (!fmt)
		fmt = new rowfmtinfo;

	fmt->cssclass = NewName(fname);
	fmt->based = bname;  // store last format used, for RTF

	// fill in rest of fmtinfo from ini

	if (Find("row type"))
		fmt->rowtype = StrRight();
	if (Find("row height"))
		fmt->rowhigh = StrRight();
	if (Find("keep"))
		fmt->keep = StrRight();
	if (Find("header cells"))
		fmt->hcells = StrRight();
	if (Find("body cells"))
		fmt->bcells = StrRight();

	if (Find("border top"))
		fmt->brdrt = StrRight();
	if (Find("border bottom"))
		fmt->brdrb = StrRight();
	if (Find("border left"))
		fmt->brdrl = StrRight();
	if (Find("border right"))
		fmt->brdrr = StrRight();
	if (Find("border sep"))
		fmt->brdrx = StrRight();

	if (Find("shading"))
		fmt->shade = StrRight();

	if (Find("css"))
		fmt->css = StrRight();
	if (Find("rtf"))
		fmt->rtf = StrRight();

	return fmt;
}


cellfmtinfo *
DCfrfile::GetCellFmtInfo(const char *fname)
{
	cellfmtinfo *fmt = NULL;
	cellfmtinfo *nfmt = NULL;
	char *bname = NULL;

	if (!GetTblSection(fname))
		return NULL;

	if (Find("based")) {
		bname = StrRight();
		if (BasedUsed(bname))
			LogEvent(logwarn, 1, "Based format repeated: ", bname);
		else {
			BasedUsed.add(bname, 1);
			nfmt = GetCellFmtInfo(bname);
			if (!nfmt)
				LogEvent(logwarn, 1, "Based format not found: ", bname);
			else
				fmt = nfmt;
			GetTblSection(fname);
		}
	}

	if (!fmt)
		fmt = new cellfmtinfo;

	fmt->cssclass = NewName(fname);
	fmt->based = bname;  // store last format used, for RTF

	// fill in rest of fmtinfo from ini

	if (Find("margin left"))
		fmt->margleft = StrRight();
	if (Find("margin right"))
		fmt->margright = StrRight();
	if (Find("margin top"))
		fmt->margtop = StrRight();
	if (Find("margin bottom"))
		fmt->margbottom = StrRight();

	if (Find("padding left"))
		fmt->padleft = StrRight();
	if (Find("padding right"))
		fmt->padright = StrRight();
	if (Find("padding top"))
		fmt->padtop = StrRight();
	if (Find("padding bottom"))
		fmt->padbottom = StrRight();

	if (Find("vertical align"))
		fmt->vertalign = StrRight();

	if (Find("border top"))
		fmt->brdrt = StrRight();
	if (Find("border bottom"))
		fmt->brdrb = StrRight();
	if (Find("border left"))
		fmt->brdrl = StrRight();
	if (Find("border right"))
		fmt->brdrr = StrRight();

	if (Find("shading"))
		fmt->shade = StrRight();

	if (Find("css"))
		fmt->css = StrRight();
	if (Find("rtf"))
		fmt->rtf = StrRight();

	if (Find("start"))
		fmt->start = StrRight();
	if (Find("end"))
		fmt->end = StrRight();

	return fmt;
}


pagefmtinfo *
DCfrfile::GetPageFmtInfo(const char *fname)
{
	pagefmtinfo *fmt = NULL;
	pagefmtinfo *nfmt = NULL;
	char *bname = NULL;

	if (!GetPgSection(fname))
		return NULL;

	if (Pages->Find("based")) {
		bname = Pages->StrRight();
		if (BasedUsed(bname))
			LogEvent(logwarn, 1, "Based format repeated: ", bname);
		else {
			BasedUsed.add(bname, 1);
			nfmt = Pages->GetPageFmtInfo(bname);
			if (!nfmt)
				LogEvent(logwarn, 1, "Based format not found: ", bname);
			else
				fmt = nfmt;
			Pages->GetPgSection(fname);
		}
	}

	if (!fmt)
		fmt = new pagefmtinfo;

	fmt->cssclass = NewName(fname);
	fmt->based = bname;  // store last format used, for RTF

	// fill in rest of fmtinfo from ini
	if (Pages->Find("two sided")) {
		fmt->twosided = Pages->StrRight();
		fmt->btwosided = Pages->BoolRight();
	}
	if (Pages->Find("start right")) {
		fmt->firstright = Pages->StrRight();
		fmt->bfirstright = Pages->BoolRight();
	}
	if (Pages->Find("start section"))
		fmt->startsection = Pages->StrRight();

	if (Pages->Find("mirror margins")) {
		fmt->margmirror = Pages->StrRight();
		fmt->bmargmirror = Pages->BoolRight();
	}

	if (Pages->Find("page number type"))
		fmt->pgnform = Pages->StrRight();
	if (Pages->Find("page number start"))
		fmt->pgnstart = Pages->StrRight();
	if (Pages->Find("orient"))
		fmt->orient = Pages->StrRight();
	if (Pages->Find("page wide"))
		fmt->paperw = Pages->StrRight();
	if (Pages->Find("page high"))
		fmt->paperh = Pages->StrRight();

	if (Pages->Find("margin left"))
		fmt->margleft = Pages->StrRight();
	if (Pages->Find("margin right"))
		fmt->margright = Pages->StrRight();
	if (Pages->Find("margin top"))
		fmt->margtop = Pages->StrRight();
	if (Pages->Find("margin bottom"))
		fmt->margbottom = Pages->StrRight();

	if (Pages->Find("gutter"))
		fmt->gutter = Pages->StrRight();
	if (Pages->Find("column count"))
		fmt->cols = Pages->StrRight();
	if (Pages->Find("column gap"))
		fmt->colsx = Pages->StrRight();
	if (Pages->Find("column rule")) {
		fmt->linebetcol = Pages->StrRight();
		fmt->blinebetcol = Pages->BoolRight();
	}

	if (Pages->Find("vertical align"))
		fmt->vertalign = Pages->StrRight();

	if (Pages->Find("border top"))
		fmt->brdrt = Pages->StrRight();
	if (Pages->Find("border bottom"))
		fmt->brdrb = Pages->StrRight();
	if (Pages->Find("border left"))
		fmt->brdrl = Pages->StrRight();
	if (Pages->Find("border right"))
		fmt->brdrr = Pages->StrRight();

	if (Pages->Find("border header")) {
		fmt->brwraphd = Pages->StrRight();
		fmt->bbrwraphd = Pages->BoolRight();
	}
	if (Pages->Find("border footer")) {
		fmt->brwrapft = Pages->StrRight();
		fmt->bbrwrapft = Pages->BoolRight();
	}

	if (Pages->Find("header top"))
		fmt->headery = Pages->StrRight();
	if (Pages->Find("footer bottom"))
		fmt->footery = Pages->StrRight();

	if (Pages->Find("css"))
		fmt->css = Pages->StrRight();
	if (Pages->Find("rtf"))
		fmt->rtf = Pages->StrRight();

	return fmt;
}


sectfmtinfo *
DCfrfile::GetSectFmtInfo(const char *fname)
{
	sectfmtinfo *fmt = NULL;
	sectfmtinfo *nfmt = NULL;
	char *bname = NULL;

	if (!GetPgSection(fname))
		return NULL;

	if (Pages->Find("based")) {
		bname = Pages->StrRight();
		if (BasedUsed(bname))
			LogEvent(logwarn, 1, "Based format repeated: ", bname);
		else {
			BasedUsed.add(bname, 1);
			nfmt = Pages->GetSectFmtInfo(bname);
			if (!nfmt)
				LogEvent(logwarn, 1, "Based format not found: ", bname);
			else
				fmt = nfmt;
			Pages->GetPgSection(fname);
		}
	}

	if (!fmt)
		fmt = new sectfmtinfo;

	fmt->cssclass = NewName(fname);
	fmt->based = bname;  // store last format used, for RTF

	// fill in rest of fmtinfo from ini
	if (Pages->Find("start side"))
		fmt->startside = Pages->StrRight();

	if (Pages->Find("mirror margins")) {
		fmt->margmirror = Pages->StrRight();
		fmt->bmargmirror = Pages->BoolRight();
	}

	if (Pages->Find("page number type"))
		fmt->pgnform = Pages->StrRight();
	if (Pages->Find("page number start"))
		fmt->pgnstart = Pages->StrRight();
	if (Pages->Find("orient"))
		fmt->orient = Pages->StrRight();
	if (Pages->Find("page wide"))
		fmt->paperw = Pages->StrRight();
	if (Pages->Find("page high"))
		fmt->paperh = Pages->StrRight();

	if (Pages->Find("margin left"))
		fmt->margleft = Pages->StrRight();
	if (Pages->Find("margin right"))
		fmt->margright = Pages->StrRight();
	if (Pages->Find("margin top"))
		fmt->margtop = Pages->StrRight();
	if (Pages->Find("margin bottom"))
		fmt->margbottom = Pages->StrRight();

	if (Pages->Find("gutter"))
		fmt->gutter = Pages->StrRight();
	if (Pages->Find("column count"))
		fmt->cols = Pages->StrRight();
	if (Pages->Find("column gap"))
		fmt->colsx = Pages->StrRight();
	if (Pages->Find("column rule")) {
		fmt->linebetcol = Pages->StrRight();
		fmt->blinebetcol = Pages->BoolRight();
	}

	if (Pages->Find("vertical align"))
		fmt->vertalign = Pages->StrRight();

	if (Pages->Find("border top"))
		fmt->brdrt = Pages->StrRight();
	if (Pages->Find("border bottom"))
		fmt->brdrb = Pages->StrRight();
	if (Pages->Find("border left"))
		fmt->brdrl = Pages->StrRight();
	if (Pages->Find("border right"))
		fmt->brdrr = Pages->StrRight();

	if (Pages->Find("border header")) {
		fmt->brwraphd = Pages->StrRight();
		fmt->bbrwraphd = Pages->BoolRight();
	}
	if (Pages->Find("border footer")) {
		fmt->brwrapft = Pages->StrRight();
		fmt->bbrwrapft = Pages->BoolRight();
	}

	if (Pages->Find("header top"))
		fmt->headery = Pages->StrRight();
	if (Pages->Find("footer bottom"))
		fmt->footery = Pages->StrRight();

	if (Pages->Find("first page")) {
		fmt->titlepg = Pages->StrRight();
		fmt->btitlepg = Pages->BoolRight();
	}
	if (Pages->Find("first space above"))
		fmt->titlespa = Pages->StrRight();

	if (Pages->Find("header"))
		fmt->hdb = Pages->StrRight();
	if (Pages->Find("footer"))
		fmt->ftb = Pages->StrRight();
	if (Pages->Find("header first"))
		fmt->hdf = Pages->StrRight();
	if (Pages->Find("footer first"))
		fmt->ftf = Pages->StrRight();
	if (Pages->Find("header left"))
		fmt->hdl = Pages->StrRight();
	if (Pages->Find("footer left"))
		fmt->ftl = Pages->StrRight();
	if (Pages->Find("header right"))
		fmt->hdr = Pages->StrRight();
	if (Pages->Find("footer right"))
		fmt->ftr = Pages->StrRight();

	if (Pages->Find("css"))
		fmt->css = Pages->StrRight();
	if (Pages->Find("rtf"))
		fmt->rtf = Pages->StrRight();

	return fmt;
}


long 
DCfrfile::GetColWidths(char *str, DCilist *wids)
{
	// read space-delimited list of lengths,
	// store twips for each in list, return count

	if (!str || !wids)
		return 0;

	long colcount = 0;
	char *val = NULL;
	long wid = 0;

	while (*str) {
		while (*str && isspace(*str))
			str++;
		if (isdigit(*str) || (*str == '.')) {
			val = str;
			while (*str && (*str != ' '))
				str++;
			val = NewName(val, str - val);
			wid = GetLength(val);
			DeleteName(val);
			wids->append(wid, ++colcount);
		}
		else // something else, give up
			return colcount;
	}

	return colcount;
}


long 
DCfrfile::GetRowDefs(char *str, DCnlist *fmts)
{
	// read space-delimited list of row formats,
	// store name for each in list, return row count

	if (!str || !fmts)
		return 0;

	long rowcount = 0;
	char *name = NULL;
	char *val = NULL;

	while (*str) {
		while (*str && isspace(*str))
			str++;
		val = str;
		while (*str && (*str != ' '))
			str++;
		name = NewName(val, str - val);
		fmts->add(name, ++rowcount);
	}

	return rowcount;
}


unc 
DCfrfile::GetRowType(char *rtype)
{
	if (!rtype)
		return 0;

	unc rtyp = 1;

	if (!stricmp(rtype, "body"))
		rtyp = 1;
	else if (!stricmp(rtype, "header"))
		rtyp = 2;
	else if (!stricmp(rtype, "footer"))
		rtyp = 3;

	return rtyp;
}


long 
DCfrfile::GetRowHigh(char *str, unc *rhtype)
{
	if (!str || !rhtype)
		return 0;

	char *val = NULL;
	long pos = 0;

	*rhtype = 3;  // default fixed

	while (*str) {
		while (*str && isspace(*str))
			str++;
		if (!strnicmp(str, "adapt", 5)) {
			*rhtype = 1;
			str += 5;
		}
		else if (!strnicmp(str, "minimum", 7)) {
			*rhtype = 2;
			str += 7;
		}
		else if (!strnicmp(str, "fixed", 5)) {
			*rhtype = 3;
			str += 5;
		}
		else if (isdigit(*str) || (*str == '.')) {
			val = str;
			while (*str && (*str != ' '))
				str++;
			val = NewName(val, str - val);
			pos = GetLength(val);
			DeleteName(val);
			break;  // just one value
		}
		else // something else, give up
			return pos;
	}

	return pos;
}


long 
DCfrfile::GetCellDefs(char *str, DCnlist *fmts)
{
	// read space-delimited list of row formats,
	// store name for each in list, return row count

	if (!str || !fmts)
		return 0;

	long cellcount = 0;
	char *name = NULL;
	char *val = NULL;

	while (*str) {
		while (*str && isspace(*str))
			str++;
		val = str;
		while (*str && (*str != ' '))
			str++;
		name = NewName(val, str - val);
		fmts->add(name, ++cellcount);
	}

	return cellcount;
}


long DCfrfile::XrefFmtNums = 0;
DCnlist DCfrfile::XrefName; // xref fmt names by XrefFmtNums
DCnlist DCfrfile::XrefForm; // xref form by XrefFmtNums

DCnlist DCfrfile::XrefBasedUsed;


char *
DCfrfile::GetXrefFmtInfo(const char *fname)
{
	long fnum = XrefName(fname);
	if (fnum)
		return XrefForm.find(fnum);

	char *fmt = NULL;
	char *nfmt = NULL;
	char *bname = NULL;

	if (!GetSubSection(fname))
		return NULL;

	if (SubFormats->Find("based")) {
		bname = StrRight();
		if (XrefBasedUsed(bname))
			LogEvent(logwarn, 1, "Based xref format repeated: ", bname);
		else {
			XrefBasedUsed.add(bname, 1);
			nfmt = GetXrefFmtInfo(bname);
			if (!nfmt)
				LogEvent(logwarn, 1, "Based xref format not found: ", bname);
			else
				fmt = nfmt;
			GetSubSection(fname);
		}
	}

	if (SubFormats->Find("form"))    // only one property so far
		fmt = SubFormats->StrRight();

	XrefName.addnew(fname, ++XrefFmtNums); // found
	XrefForm.add(fmt, XrefFmtNums);        // cache it

	return fmt;
}


long DCfrfile::RuninFmtNums = 0;
DCnlist DCfrfile::RuninName; // runin fmt names by RuninFmtNums
DCnlist DCfrfile::RuninForm; // runin form by RuninFmtNums

DCnlist DCfrfile::RuninBasedUsed;


char *
DCfrfile::GetRuninFmtInfo(const char *fname)
{
	long fnum = RuninName(fname);
	if (fnum)
		return RuninForm.find(fnum);

	char *fmt = NULL;
	char *nfmt = NULL;
	char *bname = NULL;

	if (!GetSubSection(fname))
		return NULL;

	if (SubFormats->Find("based")) {
		bname = StrRight();
		if (RuninBasedUsed(bname))
			LogEvent(logwarn, 1, "Based runin format repeated: ", bname);
		else {
			RuninBasedUsed.add(bname, 1);
			nfmt = GetRuninFmtInfo(bname);
			if (!nfmt)
				LogEvent(logwarn, 1, "Based runin format not found: ", bname);
			else
				fmt = nfmt;
			GetSubSection(fname);
		}
	}

	if (SubFormats->Find("form"))    // only one property so far
		fmt = SubFormats->StrRight();

	RuninName.addnew(fname, ++RuninFmtNums); // found
	RuninForm.add(fmt, RuninFmtNums);        // cache it

	return fmt;
}


long DCfrfile::IndexFmtNums = 0;
DCnlist DCfrfile::IndexName; // Index fmt names by IndexFmtNums
DCnlist DCfrfile::IndexForm; // Index form by IndexFmtNums

DCnlist DCfrfile::IndexBasedUsed;


char *
DCfrfile::GetIndexFmtInfo(const char *fname)
{
	long fnum = IndexName(fname);
	if (fnum)
		return IndexForm.find(fnum);

	char *fmt = NULL;
	char *nfmt = NULL;
	char *bname = NULL;

	if (!GetSubSection(fname))
		return NULL;

	if (SubFormats->Find("based")) {
		bname = StrRight();
		if (IndexBasedUsed(bname))
			LogEvent(logwarn, 1, "Based Index format repeated: ", bname);
		else {
			IndexBasedUsed.add(bname, 1);
			nfmt = GetIndexFmtInfo(bname);
			if (!nfmt)
				LogEvent(logwarn, 1, "Based Index format not found: ", bname);
			else
				fmt = nfmt;
			GetSubSection(fname);
		}
	}

	if (SubFormats->Find("form"))    // only one property so far
		fmt = SubFormats->StrRight();

	IndexName.addnew(fname, ++IndexFmtNums); // found
	IndexForm.add(fmt, IndexFmtNums);        // cache it

	return fmt;
}



long DCfrfile::MarkFmtNums = 0;
DCnlist DCfrfile::MarkName; // Mark fmt names by MarkFmtNums
DCnlist DCfrfile::MarkForm; // Mark form by MarkFmtNums

DCnlist DCfrfile::MarkBasedUsed;


char *
DCfrfile::GetMarkFmtInfo(const char *fname)
{
	long fnum = MarkName(fname);
	if (fnum)
		return MarkForm.find(fnum);

	char *fmt = NULL;
	char *nfmt = NULL;
	char *bname = NULL;

	if (!GetSubSection(fname))
		return NULL;

	if (SubFormats->Find("based")) {
		bname = StrRight();
		if (MarkBasedUsed(bname))
			LogEvent(logwarn, 1, "Based Mark format repeated: ", bname);
		else {
			MarkBasedUsed.add(bname, 1);
			nfmt = GetMarkFmtInfo(bname);
			if (!nfmt)
				LogEvent(logwarn, 1, "Based Mark format not found: ", bname);
			else
				fmt = nfmt;
			GetSubSection(fname);
		}
	}

	if (SubFormats->Find("form"))    // only one property so far
		fmt = SubFormats->StrRight();

	MarkName.addnew(fname, ++MarkFmtNums); // found
	MarkForm.add(fmt, MarkFmtNums);        // cache it

	return fmt;
}



char *
DCfrfile::GetFontName(char *fname, char **rstr)
{
	char *font = NULL;
	char *comma = NULL;
	char *quote = NULL;

	if (rstr)
		*rstr = fname;

	if ((comma = strchr(fname, ',')) != NULL) {
		// extract first name from list
		font = NewName(comma - fname);
		if (*(fname) == '"') {
			quote = strchr(++fname, '"');
			if (quote && (quote < comma))
				strncpy(font, fname, quote - fname);
			else
				strncpy(font, fname, comma - fname);
		}
		else if (*(fname) == '\'') {
			quote = strchr(++fname, '\'');
			if (quote && (quote < comma))
				strncpy(font, fname, quote - fname);
			else
				strncpy(font, fname, comma - fname);
		}
		else
			strncpy(font, fname, comma - fname);
	}
	else {
		//font = fname;
		font = NewName(strlen(fname));
		if (*(fname) == '"') {
			quote = strchr(++fname, '"');
			if (quote)
				strncpy(font, fname, quote - fname);
			else
				strcpy(font, fname);
		}
		else if (*(fname) == '\'') {
			quote = strchr(++fname, '\'');
			if (quote)
				strncpy(font, fname, quote - fname);
			else
				strcpy(font, fname);
		}
		else
			strcpy(font, fname);
	}
	return font;
}


DCnlist DCfrfile::LengthNames;
npair DCfrfile::LengthNamesInit[] = {
	{ lenin, "in" },
	{ lencm, "cm" },
	{ lenmm, "mm" },
	{ lenpt, "pt" },
	{ lenpc, "pc" },
	{ lenem, "em" },
	{ lenex, "ex" },
	{ lenpx, "px" },
	NULL
};
bool DCfrfile::LengthNamesSet = false;


long
DCfrfile::GetLength(char *fstr, char **rstr)
{
	if (!LengthNamesSet) {
		LengthNames.init(LengthNamesInit);
		LengthNamesSet = true;
	}

	char *fptr = NULL;
	double dval = 0.0;
	lenunits un = lennone;

	// allow and convert pt, pc, cm, mm, px (at 96dpi)
	// if em, ex, or percent, store in *rstr and return 0
	dval = strtod(fstr, &fptr);
	if (dval == 0.0) {  // units don't matter for zero
		if (rstr)
			*rstr = "0";      // record as real zero
		return 0;
	}
	while (*fptr
	 && (*fptr == ' '))
		fptr++;
	if (*fptr) {
		char *nptr = fptr + strlen(fptr);
		while (--nptr > fptr) {  // remove trailing spaces
			if (*nptr == ' ')
			 *nptr = '\0';
			else
				break;
		}

		if (*fptr == '%')
			un = lenpct;
		else
			un = (lenunits) LengthNames(fptr);
		if (un == lennone) {
			LogEvent(logwarn, 1, "Unrecognized size unit: ", fstr);
			if (rstr)
				*rstr = fstr;
			return 0;
		}
		if (un >= lenem) {
			if (rstr)
				*rstr = fstr;
			// return 0;
		}
		switch (un) {
			case lenin:
				dval *= 1440;
				break;

			case lencm:
				dval = (dval * 1440.0) / 2.44;
				break;

			case lenmm:
				dval = (dval * 1440.0) / 24.4;
				break;

			case lenpc:
				dval *= 240;
				break;

			case lenpt:
				dval *= 20;
				break;

			case lenpx:
				dval = (dval * 1440.0) / 96.0;
				break;

			case lenem:
				dval *= BodyBaseVal;
				break;

			case lenex:
				dval *= (BodyBaseVal / 2.0);
				break;

			case lenpct:
				dval = (dval * CurrPercentBase) / 100.0;
				break;

			default:
				break;
		}
	}
	else   // no units, use pixels
		dval = (dval * 1440.0) / 96.0;

	return (long) floor(dval + 0.5);
}


long
DCfrfile::GetPercent(char *fstr, char **rstr)
{
	char *fptr = NULL;
	double dval = 0.0;

	// allow only percent
	dval = strtod(fstr, &fptr);
	if (dval == 0.0) {  // units don't matter for zero
		if (rstr)
			*rstr = "0";      // record as real zero
		return 0;
	}

	while (*fptr
	 && (*fptr == ' '))
		fptr++;
	if (*fptr) {
		if (*fptr != '%') {
			LogEvent(logwarn, 1, "Required percent value not found ", fstr);
			if (rstr)
				*rstr = fstr;
			return 0;
		}
	}

	dval = (dval * 100.0);  // return hundredths of a percent
	return (long) floor(dval + 0.5);
}



DCnlist DCfrfile::ColorNames;
npair DCfrfile::ColorNamesInit[] = {
	{ 0x00FFFF, "aqua" },
	{ 0x000001, "black" }, // to distinguish from none
	{ 0x0000FF, "blue" },
	{ 0xFF00FF, "fuchsia" },
	{ 0x808080, "gray" },
	{ 0x008000, "green" },
	{ 0x00FF00, "lime" },
	{ 0x800000, "maroon" },
	{ 0x000080, "navy" },
	{ 0x808000, "olive" },
	{ 0xFFA500, "orange" },
	{ 0x800080, "purple" },
	{ 0xFF0000, "red" },
	{ 0xC0C0C0, "silver" },
	{ 0x008080, "teal" },
	{ 0xFFFFFF, "white" },
	{ 0xFFFF00, "yellow" },
	NULL
};

bool DCfrfile::ColorNamesSet = false;


long 
DCfrfile::GetColor(char *cstr, char **rstr)
{
	return GetColorValue(cstr, rstr);
}


long 
DCfrfile::GetColorValue(char *cstr, char **rstr)
{
	long cval = 0;
	if (!ColorNamesSet) {
		ColorNames.init(ColorNamesInit);
		ColorNamesSet = true;
	}

	// accept names, #nnn, #nnnnnn, rgb(n%,n%,n%), rgb(n,n,n)
	if (*cstr == '#') {
		char cbuf[7];
		if (strlen(cstr + 1) == 3) {
			cbuf[0] = cbuf[1] = cstr[1];
			cbuf[2] = cbuf[3] = cstr[2];
			cbuf[4] = cbuf[5] = cstr[3];
			cbuf[7] = 0;
			cstr = cbuf;
		}
		else
			cstr++;
		cval = strtol(cstr, NULL, 16);
	}
	else if (!stricmp(cstr, "transparent")) {
		cval = -1;
		if (rstr)
			*rstr = cstr;
	}
	else if (!strnicmp(cstr, "rgb(", 4)) {
		char *cptr = cstr + 4;
		double dcol = 0.0;
		long ccol = 0;
		for (long i = 0; i < 3; i++) {
			while (*cptr
			 && ((*cptr == ',')
			  || (*cptr == ' ')))
				cptr++;
			dcol = strtod(cptr, &cptr);
			if (dcol < 0)
				dcol = 0;
			if (*cptr == '%') {
				if (dcol > 100.0)
					dcol = 100.0;
				dcol = (dcol * 255.0) / 100.0;
				cptr++;
			}
			else if (dcol > 255.0)
				dcol = 255.0;
			ccol = (long) ceil(dcol);
			cval = (cval << 8) + ccol;
		}
		while (*cptr
		 && (*cptr == ' '))
			cptr++;
		if (*cptr != ')')
			LogEvent(logwarn, 1, "Malformed RGB color value: ", cstr);
	}
	else if ((cval = ColorNames(cstr)) == 0) {
		LogEvent(logwarn,1,"Unrecognized color value: ", cstr);
		if (rstr)
			*rstr = cstr;
	}
	else if (cval == 1)
		cval = 0;  // actual black

	return cval;
}



// end of dcfmtr.cpp

