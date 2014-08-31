// dwrtfcn.cpp is the code for dcl  RTF condition processing
// Copyright (c) 2009 by Omni Systems, Inc.  All rights reserved.

#include "dcappl.h"
#include "dcctlg.h"
#include "dcfili.h"
#include "dctxfx.h"
#include "dcfmtr.h"
#include "dcini.h"
#include "dcmacr.h"
#include "dwrtf.h"
#include "dwrtffl.h"




// conditional text

long RTFwr::CurrCond = 0;
DCnlist RTFwr::CondNames;


void 
RTFwr::StartCondDef(RTFctl *cp)
{
	CurrCond = cp->dat();
	CurrCondInfo = new condinfo;
	CurrCondInfo->cond = CurrCond;
	CondInfos.add(CurrCondInfo, CurrCond);
}


void 
RTFwr::SetCondName(RTFctl *cp)
{
	CurrCondInfo->name = NewName((char *) cp->ex());
	CondNames.add(CurrCondInfo->name, CurrCond);
	if (!stricmp(CurrCondInfo->name, "conflict"))
		ConflictCondInfo = CurrCondInfo;
}



// condition indicators

/*
struct condinfo {
	long cond;
	char *name;
	char *chchs;
	char *simage;
	char *salt;
	char *eimage;
	char *ealt;
	uns colr;
	uns bcolr;
	uns chcolr;
  unc bold;
  unc ital;
	unc line;
	bool hidden;
};
*/

bool RTFwr::InCondInds = false;
DCvlist RTFwr::CondInfos;
condinfo *RTFwr::CurrCondInfo = NULL;
condinfo *RTFwr::ConflictCondInfo = NULL;


void
RTFwr::SetCondInd(RTFctl *cp)
{
	char *val = NewName( (char *) cp->ex(), cp->siz() + 1);
	char *oname = NULL;
	char *nname = NULL;

	switch (cp->rp->scan_op) {
		case condcch:
			CurrCondInfo->chchs = val;
			break;

		case condsimg:
			oname = GraphFileName(val);
			nname = NewName(oname);
			delete [] oname;
			CurrCondInfo->simage = nname;
			break;

		case condsalt:
			CurrCondInfo->salt = val;
			break;

		case condeimg:
			oname = GraphFileName(val);
			nname = NewName(oname);
			delete [] oname;
			CurrCondInfo->eimage = nname;
			break;

		case condealt:
			CurrCondInfo->ealt = val;
			break;

		default:
			break;
	}
}


void
RTFwr::SetCondTypo(RTFctl *cp)
{
	switch (cp->rp->write_op) {
		case putbold:
      CurrCondInfo->bold = (cp->siz() > 2) ? 2 : 1;
			break;

		case putital:
			CurrCondInfo->ital = cp->siz() ? 2 : 1;
			break;

		case putline:
      CurrCondInfo->line = (unc) cp->siz() + 1;
			break;

		default:
			break;
	}
}


void
RTFwr::SetCondColor(RTFctl *cp)
{
	uns color = 0;
	if (cp->dt() == ibyte)
		color = CheckColorValue(cp->siz());
	else if (cp->dt() == imnem)
		GetColor(cp->siz(), &color);

	switch (cp->rp->scan_op) {
		case condcol:  // color for condition
			CurrCondInfo->colr = color;
			break;
		case condbcol: // background color
			CurrCondInfo->bcolr = color;
			break;
		case condccol: // changebar color
			CurrCondInfo->chcolr = color;
			break;

		default:
			break;
	}
}


// condition usage

DClist RTFwr::CondsStarting;
DClist RTFwr::CondsEnding;
DClist RTFwr::CondsInEffect;
bool RTFwr::CondStartPend;
bool RTFwr::CondEndPend;
bool RTFwr::CondsEffective;
long RTFwr::CondFlagDiv = 0;
long RTFwr::PreCondFlagDiv = 0;


void
RTFwr::SetCond(RTFctl *cp)
{
	uns cond = cp->dat();

	if (CondsInEffect.find(cond)) {
		if (CondsEnding.find(cond)) {
			CondsEnding.remove(cond);
			if (!CondsEnding.count())
				CondEndPend = false;
		}
		return;  // already in effect
	}

	CondsStarting.add(cond);
	CondStartPend = true;
}


void
RTFwr::EndCond(RTFctl *cp)
{
	uns cond = cp->dat();

	if (!CondsInEffect.find(cond)) {
		if (CondsStarting.find(cond)) {
			CondsStarting.remove(cond);
			if (!CondsStarting.count())
				CondStartPend = false;
		}
		return;  // never started
	}

	CondsEnding.add(cond);
	CondEndPend = true;
}



// condition display

// conds start and end anywhere, and can nest and overlap
// cond properties are additive to other format properties
// so regular properties in Curr must be left unchanged
// so they can be restored after the end of the condition
// proerties changes should accrue until the next text or
// end of file so that we don't switch on and off pointlessly

// when checking, first we sum up the conds, removing the ending,
// adding the starting, and computing the total effect, including
// the need to use any Conflict colors, in:
condinfo *RTFwr::PendCondInfo = NULL;
condinfo *RTFwr::NonCondInfo = NULL;
DCnlist RTFwr::EndFlags;
DCnlist RTFwr::EndAlts;
DCnlist RTFwr::StartFlags;
DCnlist RTFwr::StartAlts;


// then we compare that to CurrCondInfo, then to Curr, and
// put out any that are different from the sum of those
// finally we set CurrCondInfo = PendCondInfo, and we're done

void 
RTFwr::WriteCondProps(RTFwfile *wf)
{
	long cond = 0;
	condinfo * ci = NULL;
	bool condchange = false;

	if (CondEndPend) {
		while ((cond = CondsEnding.dequeue()) != 0) {
			if ((ci = (condinfo *) CondInfos.find(cond)) != NULL) {
				if (ci->eimage)
					EndFlags.add(ci->eimage, cond);
				else if (ci->ealt)
					EndAlts.add(ci->ealt, cond);
			}
			CondsInEffect.remove(cond);
		}
		CondEndPend = false;
	}
	if (CondStartPend) {
		while ((cond = CondsStarting.dequeue()) != 0) {
			if ((ci = (condinfo *) CondInfos.find(cond)) != NULL) {
				if (ci->simage)
					StartFlags.add(ci->simage, cond);
				else if (ci->salt)
					StartAlts.add(ci->salt, cond);
			}
			CondsInEffect.add(cond);
		}
		CondStartPend = false;
	}

	PendCondInfo = ResetCondInfo();
	if (CondsInEffect.count() != 0) {
		// PendCondInfo = new condinfo;
		CondsInEffect.all(AddCondProps);
		CondsEffective = true;
		condchange = true;
	}
	else if (CondsEffective) {
		condchange = true;
		CondsEffective = false;
	}

	long cdiv = 0;
	long hcdiv = wf->GetDiv();

	if (CondFlagDiv) {
		cdiv = wf->GetDiv();
		wf->SelectDiv(PreCondFlagDiv);
	}

	// put out end flags, then start flags
	WriteFlags(wf, &EndFlags, &EndAlts);

	if (CondFlagDiv) {
		if (PreHotDiv == CondFlagDiv)
			PreHotDiv = PreCondFlagDiv;
		wf->WriteDiv(CondFlagDiv);
		wf->DeleteDiv(CondFlagDiv);
		CondFlagDiv = 0;
		wf->SelectDiv(PreCondFlagDiv);
	}

	WriteFlags(wf, &StartFlags, &StartAlts);

	if (condchange) {
		// set props not already set in Curr
		if ((PendCondInfo->bold > 1)
		 && (PendCondInfo->bold > Curr.bold)) {
			Curr.bold = PendCondInfo->bold;
			wf->putCW("b");
		}
		else if ((PendCondInfo->bold == 1)
		 && (PendCondInfo->bold < Curr.bold)) {
			Curr.bold = PendCondInfo->bold;
			wf->putCW("b0");
		}

		if ((PendCondInfo->ital > 1)
		 && (PendCondInfo->ital > Curr.ital)) {
			Curr.ital = PendCondInfo->ital;
			wf->putCW("i");
		}
		else if ((PendCondInfo->ital == 1)
		 && (PendCondInfo->ital < Curr.ital)) {
			Curr.ital = PendCondInfo->ital;
			wf->putCW("i0");
		}

		if (PendCondInfo->line > Curr.line) {
			Curr.line = PendCondInfo->line;
			switch (PendCondInfo->line) {
				case 2:
				case 5:
					wf->putCW("ul");
					break;
				case 3:
					wf->putCW("uldb");
					break;
				case 4:
					wf->putCW("uld");
					break;
				default:
					break;
			}
		}
		else if ((PendCondInfo->line == 1) 
		 && (PendCondInfo->line < Curr.line)) {
			switch (Curr.line) {
				case 2:
				case 5:
					wf->putCW("ul0");
					break;
				case 3:
					wf->putCW("uldb0");
					break;
				case 4:
					wf->putCW("uld0");
					break;
				default:
					break;
			}
			Curr.line = PendCondInfo->line;
		}

		if ((PendCondInfo->colr > 2)
		 && (Curr.colr != PendCondInfo->colr)) {
			Curr.colr = PendCondInfo->colr;
			wf->putCWN("cf", PendCondInfo->colr - 1);
		}
		else if ((Curr.colr > 2)
		 && (Curr.colr != PendCondInfo->colr)) {
			Curr.colr = PendCondInfo->colr;
			wf->putCWN("cf", PendCondInfo->colr - 1);
		}

		if ((PendCondInfo->bcolr > 1)
		 && (PendCondInfo->bcolr != 9)
		 && (Curr.bcolr != PendCondInfo->bcolr)) {
			Curr.bcolr = PendCondInfo->bcolr;
			wf->putCWN("cb", PendCondInfo->bcolr - 1);
		}
		else if (((Curr.bcolr > 1)
			|| (Curr.bcolr == 9))
		 && (Curr.bcolr != PendCondInfo->bcolr)) {
			Curr.bcolr = PendCondInfo->bcolr;
			wf->putCWN("cb", PendCondInfo->bcolr - 1);
		}

		// what to do about changebar props?  ****
	}

	// update current state
	if (cdiv && (cdiv == HotDiv))
		wf->SelectDiv(HotDiv);
	if (hcdiv == HelpCloneDiv)
		wf->SelectDiv(HelpCloneDiv);
	//if (CurrCondInfo)
	//	delete CurrCondInfo;
	CurrCondInfo = NULL;
	if (CondsEffective)
		CurrCondInfo = PendCondInfo;
	PendCondInfo = NULL;
}


condinfo *
RTFwr::ResetCondInfo(void)
{
	// set PendCondInfo per ParaForm and CForm if (CForm != NoForm)
	// to undo whatever CurrCondInfo did without wiping other settings
	condinfo *ci = new condinfo;

	ci->bold = ParaForm.bold;
	ci->ital = ParaForm.ital;
	ci->line = ParaForm.line;
	ci->colr = ParaForm.colr;
	ci->bcolr = ParaForm.bcolr;

	if (Curr.cstag) {
		if (CForm.bold)
			ci->bold = CForm.bold;
		if (CForm.ital)
			ci->ital = CForm.ital;
		if (CForm.line)
			ci->line = CForm.line;
		if (CForm.colr)
			ci->colr = CForm.colr;
		if (CForm.bcolr)
			ci->bcolr = CForm.bcolr;
	}

	NonCondInfo = ci;
	return ci;
}


void 
RTFwr::AddCondProps(long cond)
{
	// add current cond props to PendCondInfo
	condinfo *ci = (condinfo *) CondInfos.find(cond);
	if (!ci)
		return;

	if (ci->bold > PendCondInfo->bold)
		PendCondInfo->bold = ci->bold;
	if (ci->ital > PendCondInfo->ital)
		PendCondInfo->ital = ci->ital;

	// single, double, over, numeric
	if (ci->line > PendCondInfo->line)
		PendCondInfo->line = ci->line;

	if (ci->colr) {
		if (!PendCondInfo->colr
		 || (NonCondInfo->colr == PendCondInfo->colr))
			PendCondInfo->colr = ci->colr;
		else if (ConflictCondInfo
		 && (ci->colr != PendCondInfo->colr))
			PendCondInfo->colr = ConflictCondInfo->colr;
	}
	if (ci->bcolr) {
		if (!PendCondInfo->bcolr
		 || (NonCondInfo->bcolr == PendCondInfo->bcolr))
			PendCondInfo->bcolr = ci->bcolr;
		else if (ConflictCondInfo
		 && (ci->bcolr != PendCondInfo->bcolr))
			PendCondInfo->bcolr = ConflictCondInfo->bcolr;
	}

	// how to handle conflict in ch props?
	if (ci->chchs) {
		if (!PendCondInfo->chchs)
			PendCondInfo->chchs = ci->chchs;
	}
	if (ci->chcolr) {
		if (!PendCondInfo->chcolr)
			PendCondInfo->chcolr = ci->chcolr;
	}
}


void 
RTFwr::WriteFlags(RTFwfile *wf, DCnlist *flg, DCnlist *alt)
{
	const char *img = NULL;
	const char *calt = NULL;
	DCnlist *fp = flg;
	DCnlist *ap = alt;

	while (fp->next) {
		fp = fp->next;
		img = fp->name;
		alt->remove(fp->id);
		wf->putCr();
		if (HelpForm) {
			wf->putCS('{');
			wf->putStr(BmcStr);
			wf->putStr(img);
			wf->putCS('}');
		}
		else
			wf->putField("INCLUDEPICTURE", (char *) img);
	}

	while (ap->next) {
		ap = ap->next;
		calt = ap->name;
		wf->putCr();
		wf->putStr(calt);
	}

	flg->empty();
	alt->empty();
}



// table row conds

DClist RTFwr::HiddenConds;
bool RTFwr::ShowHiddenCond = false;
bool RTFwr::ShowTbRow = false;
bool RTFwr::HideTbRow = false;


void
RTFwr::ProcTbCondUse(RTFctl *cp)
{
	if ((ShowHiddenCond == false)
	 && (Row != NULL)) { // only used to suppress table rows
		if (HiddenConds.find(cp->dat()))
			HideTbRow = true;
		else               // handle multiple conditions properly
			ShowTbRow = true;
	}
}




// end of dwrtf.cn

