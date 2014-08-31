
// dcmacr.cpp contains code for the dcl macro class, JHG, 3/6/05
// Copyright (c) 2005 by Omni Systems, Inc.  All rights reserved.

#include "dcappl.h"
#include "dcfili.h"
#include "dcini.h"
#include "dcmacr.h"

DCwfile *DCmacr::DestFile = NULL;
DCirfile *DCmacr::IniFile = NULL;
DCirfile *DCmacr::MacroIniFile = NULL;

// ini-file settings

secinfo DCmacr::MacroIniSects[] = {
 { "HTMLOptions", set, "MacrosSet", ISecMacros, true },
 { "Macros", set, "MacrosSet", ISecMacros, true },
 { NULL, unknown, NULL, NULL, false }
};

setinfo DCmacr::ISecMacros[] = {
 { "MacroFile", path, &MacroFile, NULL },
 { "MacroNestMax", usnum, &MacroNestMax, NULL },
 { "RepeatMax", usnum, &RepeatMax, NULL },
 { "WhileMax", usnum, &WhileMax, NULL },
 { "NameUndefinedMacros", yesno, &NameUndefMacros, NULL },
 { "NameUndefinedMacroVars", yesno, &NameUndefMacroVars, NULL },
 { "FixMacroQuotes", yesno, &FixMacroQuotes, NULL },
 { "OmitMacroReturns", yesno, &OmitMacroReturns, NULL },
 { "MacroVarNesting", yesno, &MacroVarNesting, NULL },
 { "ReplaceFrameVars", yesno, &ReplaceFrameVars, NULL },
 { NULL, spec, NULL, NULL }
};


/* template

setinfo DCmacr::ISec[] = {
 { "", yesno, &, NULL },
 { "", cap, &, NULL },
 { "", str, &, NULL },
 { "", usnum, &, NULL },
 { NULL, spec, NULL, NULL }
};

void
DCmacr::Set(char *)
{
}

*/

// local settings vars

char *DCmacr::MacroFile = NULL;

bool DCmacr::ValidOnly = false;
bool DCmacr::NameUndefMacroVars = false;
bool DCmacr::NameUndefMacros = false;

short DCmacr::MacroNestLevel = 0;
short DCmacr::MacroNestMax = MacroNestLevMax;
short DCmacr::RepeatMax = MacroNestLevMax;
short DCmacr::WhileMax = MacroNestLevMax;

bool DCmacr::OmitMacroReturns = false;
bool DCmacr::MacroVarNesting = true;
bool DCmacr::FixMacroQuotes = false;
bool DCmacr::ReplaceFrameVars = false;


// internal counters

long DCmacr::RepCount = 0;
long DCmacr::RepDCount = 0;
long DCmacr::WhileCount = 0;

// callback functions
//void (*DCmacr::Write...)(DCwfile *df, unc uch);
pbWMC DCmacr::WriteMacroChar = StubbWMC;
//bool (*DCmacr::Write...)(DCwfile *df, char *name);
pbWCP DCmacr::WritePredefinedMacro = StubbWCP;
pbWCP DCmacr::WriteCodeStoreVar = StubbWCP;
pcWCP DCmacr::GetCodeStoreVar = StubcWCP;
//char *(*DCmacr::Get...Var)(char *vp);
pcGV DCmacr::GetTextStore = StubcGV;
pcWCP DCmacr::GetPredefinedVar = StubcWCP;
pcGV DCmacr::GetFrameUserVar = StubcGV;
plPCL DCmacr::GetFrameUserArrVar = StublPCL;

pvWUL DCmacr::WriteMacroTextSingleChar = NULL;


// initialization functions

void
DCmacr::InitMacroModule(DCwfile *df,
	 DCirfile *ifl, char *mfl,
	 pbWMC fwmc, pbWCP fwpm,
	 pbWCP fwcsv, pcWCP fgcsv,
	 pcGV fgts, pcWCP fgpv, 
	 pcGV fgfuv, plPCL fgfuav,
	 bool vo, bool numv, bool num,
	 bool fmq, bool mvn, bool omr,
	 uns umnm, uns urm, uns uwm)
{
	if (df)
		DestFile = df;
	if (ifl)
		IniFile = ifl;
	if (mfl)
		MacroFile = mfl;

	if (fwmc)
		WriteMacroChar = fwmc;
	if (fwpm)
		WritePredefinedMacro = fwpm;
	if (fwcsv)
		WriteCodeStoreVar = fwcsv;
	if (fgcsv)
		GetCodeStoreVar = fgcsv;

	if (fgts)
		GetTextStore = fgts;
	if (fgpv)
		GetPredefinedVar = fgpv;
	if (fgfuv)
		GetFrameUserVar = fgfuv;
	if (fgfuav)
		GetFrameUserArrVar = fgfuav;

	if (vo)
		ValidOnly = vo;

	// set only for back compatibility, [Macros] overrides
	if (numv)
		NameUndefMacroVars = numv;
	if (num)
		NameUndefMacros = num;
	if (fmq)
		FixMacroQuotes = fmq;
	if (!mvn)  // default is true
		MacroVarNesting = mvn;
	if (omr)
		OmitMacroReturns = omr;
	if (umnm)
		MacroNestMax = umnm;
	if (urm)
		RepeatMax = urm;
	if (uwm)
		WhileMax = uwm;

	if (df)
		InitMacroConfigTemplates(df->getBaseName());
}


void
DCmacr::InitPluginMacroModule(DCirfile *ifl, char *snm, char *mfl,
	 pbWMC fwmc, pbWCP fwpm, pcWCP fgpv,
	 pcGV fgfuv, plPCL fgfuav, bool vo)
{
	if (ifl)
		IniFile = ifl;
	if (mfl)
		MacroFile = mfl;

	if (fwmc)
		WriteMacroChar = fwmc;
	if (fwpm)
		WritePredefinedMacro = fwpm;

	if (fgpv)
		GetPredefinedVar = fgpv;
	if (fgfuv)
		GetFrameUserVar = fgfuv;
	if (fgfuav)
		GetFrameUserArrVar = fgfuav;

	if (vo)
		ValidOnly = vo;

	InitMacroConfigTemplates(snm);
}


void
DCmacr::InitMacroConfigTemplates(char *snm)
{
	DCini::InitIniModule(IniFile, MacroIniSects);
	DCini::ProcAllSects();

	if (MacroFile
	 && *MacroFile) {
		// snm is specific .ini basename, can be NULL
		MacroIniFile = new DCirfile(MacroFile, snm);
		if (MacroIniFile->FErr() != fok) {
			MacroIniFile->CloseFile();
			MacroIniFile = NULL;
		}
		else
			MacroIniFile = MacroIniFile->SetConfigTemplate("Templates", "Macros");
	}
}


// stub functions for uninitialized function pointers

bool
DCmacr::StubbWMC(DCwfile *df, unc uch)
{
	return false;
}

bool
DCmacr::StubbWCP(DCwfile *df, char *vp)
{
	return false;
}

char *
DCmacr::StubcWCP(DCwfile *df, char *vp)
{
	return NULL;
}

char *
DCmacr::StubcGV(char *vp)
{
	return NULL;
}

long 
DCmacr::StublPCL(char *vp, long idx)
{
	return 0;
}


// obtain macros from a library file

DCblock *
DCmacr::GetMacroFile(char *fname)
{
	DCrfile *rf = NULL;
	DCblock *bp = NULL;

	rf = new DCrfile(fname);
	if (rf->FErr())
		return NULL;

	long len = rf->getSize() + 1;
	bp = new DCblock(len);
	*bp = *rf;
	((char *)(bp->p()))[len - 1] = '\0';

	rf->CloseFile();
	delete rf;
	return bp;
}


// macro processing

char *
DCmacr::GetMacro(char *mactxt)
{
	if (!DestFile)
		return NULL;

	char *chp = NULL;
	long cdiv = DestFile->GetDiv();
	long mdiv = DestFile->NewDiv();

	WriteMacro(DestFile, mactxt);
	if (DestFile->LenDiv())
		chp = NewName(DestFile->ReadDiv());

	DestFile->DeleteDiv(mdiv);
	DestFile->SelectDiv(cdiv);
	return chp;
}

void
DCmacr::WriteMacro(DCwfile *wf, char *mactxt)
{
	if (!mactxt)
		return;

	if (MacroNestLevel > MacroNestMax)
		return;
	MacroNestLevel++;

	bool sys = false; // used to detect undercore names

	unc ifstate = 0; // 0=not in if, 1=executing then clause,
	                 // 2=skip to else, 3 = skip to endif

	unc repeat = 0;  // 0=not in repeat, 1=executing repeat,
	                 // 2=skip to endrepeat
	char *repstart = NULL;  // last char of <$repeat value>
	char *repend = NULL;    // last char of <$endrepeat> or end of macro
	long count = 0;
	long dcount = 0;
	char *tok = NULL;

	unc whilestate = 0; // 0=not in while, 1=executing,
	                    // 2=skip to endwhile
	char *whilecond = NULL;  // start of cond to test
	char *whileend = NULL;   // last char of <$endwhile>
	long wcount = 0;         // counter to prevent runaways
	bool until = false; // condition reverser for while/until

	unc looptype = 0;   // 0=not in loop, 1=repeat, 2=while

	while (*mactxt == ';') { // allow starting comment lines
		mactxt++;
		while (*mactxt && (*mactxt != '\n'))
			mactxt++;
		if (*mactxt == '\n')  // remove trailing newline
			mactxt++;
	}
	
	char *macstart = repstart = mactxt;  // allow continue to restart macro
	bool point = false;
	bool act = true;

	while (*mactxt) {
		act = ((ifstate < 2) && (repeat < 2) && (whilestate < 2));

		switch ((unc) *mactxt) {
			case '\r':
				break;
			case '\n':
				if ((!OmitMacroReturns)
				 && act)
					wf->putCr();
				while (*(mactxt + 1) == ';') { // allow comment lines
					mactxt++;
					while (*mactxt && (*mactxt != '\n'))
						mactxt++;
				}
				break;

			case '\\':
				if (*(mactxt + 1) == '\r') {
					if (*(mactxt + 2) == '\n')
						mactxt += 2;
					else
						mactxt++;
				}
				else if (*(mactxt + 1) == '\n')
					mactxt++;
				else if ((*(mactxt + 1) == 'n')
				 || (*(mactxt + 1) == 'r')) {
					mactxt++;
					if (act)
						wf->putCr();
				}
				else if ((*(mactxt + 1) == '~')
				 || (*(mactxt + 1) == ' ')) {
					mactxt++;
					if (act)
						wf->putChar(' ');
				}
				else if ((*(mactxt + 1) == '<')
				 || (*(mactxt + 1) == '>')
				 || (*(mactxt + 1) == '$')
				 || (*(mactxt + 1) == '\\')
				 || (*(mactxt + 1) == '"')
				 || (*(mactxt + 1) == ';')) {
					mactxt++;
					goto normal;
				}
				else
					goto normal;
				break;


			case 130:  // baseline
			case 145:  // left single
			case 146:  // right single
				if (!FixMacroQuotes
				 && !ValidOnly)
					goto normal;
				if (act)
					wf->putChar('\'');
				break;

			case 132:  // baseline
			case 147:  // left dbl
			case 148:  // right dbl
			case 171:  // left guillemot
			case 187:  // right guillemot
				if (!FixMacroQuotes
				 && !ValidOnly)
					goto normal;
				if (act)
					wf->putChar('"');
				break;

			case 149:  // bullet
				if (!ValidOnly)
					goto normal;
				if (act
				 && !WriteMacroChar(wf, 183))  // middle dot is valid
					goto normal;
				//	wf->putSpec(183);
				break;

			case 150:  // en dash
				if (!ValidOnly)
					goto normal;
				if (act)
					wf->putChar('-');
				break;

			case 151:  // em dash
				if (!ValidOnly)
					goto normal;
				if (act)
					wf->putStr("--");
				break;


			case 0x1A:
				break;
			case '<':
				if (*(mactxt + 1) == '*') {  //pointer ref
					mactxt++;
					point = true;
				}
				else
					point = false;

				if (*(mactxt + 1) == '$') {  // macro ref
					mactxt += 2;  // start of name
					while (*mactxt == ' ')
						mactxt++;

					if (*mactxt == '_') {
						sys = true;
						mactxt++;
					}
					else
						sys = false;

					// conditional processing
					if (!_strnicmp(mactxt, "if not", 6)
					 && (*(mactxt + 6) == ' ')
					 && (repeat < 2) && (whilestate < 2)) {
						mactxt += 7;
						ifstate = CheckCond(wf, &mactxt) ? 2 : 1;
					}
					else if (!_strnicmp(mactxt, "if", 2)
					 && (*(mactxt + 2) == ' ')
					 && (repeat < 2) && (whilestate < 2)) {
						mactxt += 3;
						ifstate = CheckCond(wf, &mactxt) ? 1 : 2;
					}
					else if (!_strnicmp(mactxt, "elseif not", 10)
					 && (*(mactxt + 10) == ' ')
					 && (ifstate != 3)
					 && (repeat < 2) && (whilestate < 2)) {
						mactxt += 11;
						ifstate = (ifstate == 1) ? 3 : (CheckCond(wf, &mactxt) ? 2 : 1);
					}
					else if (!_strnicmp(mactxt, "elseif", 6)
					 && (*(mactxt + 6) == ' ')
					 && (ifstate != 3)
					 && (repeat < 2) && (whilestate < 2)) {
						mactxt += 7;
						ifstate = (ifstate == 1) ? 3 : (CheckCond(wf, &mactxt) ? 1 : 2);
					}
					else if (!_strnicmp(mactxt, "else", 4)
					 && ((*(mactxt + 4) == ' ')
					  || (*(mactxt + 4) == '>'))
					 && (repeat < 2) && (whilestate < 2)) {
						ifstate = (ifstate == 2) ? 1 : 3;
						if (ifstate == 1)
							mactxt += 4;
					}
					else if (!_strnicmp(mactxt, "endif", 5)
					 && ((*(mactxt + 5) == ' ')
					  || (*(mactxt + 5) == '>'))
					 && (repeat < 2) && (whilestate < 2)) {
						ifstate = 0;
						mactxt += 5;
					}

					// repeat loop processing
					else if (!_strnicmp(mactxt, "repeat", 6)
					 && ((*(mactxt + 6) == ' ')
					  || (*(mactxt + 6) == '>'))
					 && (ifstate < 2) && (whilestate < 2)) {
						repeat = 1;
						RepCount = count = 1;
						mactxt += 6;
						if ((*mactxt == ' ')
						 && ((dcount = CheckCond(wf, &mactxt)) != 0))
							RepDCount = dcount;
						else
							RepDCount = dcount = (long) RepeatMax;
						repstart = mactxt;
						looptype = 1;
					}
					else if (!_strnicmp(mactxt, "endrepeat", 9)
					 && ((*(mactxt + 9) == ' ')
					  || (*(mactxt + 9) == '>'))
					 && (ifstate < 2) && (whilestate < 2)) {
						mactxt += 9;
						repend = mactxt;
						if ((dcount == 1)
						 || (repeat == 2)) {
							RepDCount = dcount = 0;
							repeat = 0;
							repstart = repend = NULL;
							looptype = whilestate ? 2 : 0;
						}
						else if (dcount > 1) {
							RepDCount = --dcount;
							RepCount = ++count;
							mactxt = repstart;
							repeat = 1;
						}
					}

					else if (!_strnicmp(mactxt, "while", 5)
					 && (*(mactxt + 5) == ' ')
					 && ((ifstate < 2) && (repeat < 2))) {
						mactxt += 5;
						WhileCount = wcount = 1;
						whilecond = mactxt;
						whilestate = CheckCond(wf, &mactxt) ? 1 : 2;
						looptype = (whilestate == 1) ? 2 : 0;
						until = false;
					}
					else if (!_strnicmp(mactxt, "until", 5)
					 && (*(mactxt + 5) == ' ')
					 && ((ifstate < 2) && (repeat < 2))) {
						mactxt += 5;
						WhileCount = wcount = 1;
						whilecond = mactxt;
						whilestate = CheckCond(wf, &mactxt) ? 2 : 1;
						looptype = (whilestate == 1) ? 2 : 0;
						until = true;
					}
					else if ((!_strnicmp(mactxt, "endwhile", 8)
					  || (!_strnicmp(mactxt, "enduntil", 8)))
					 && ((*(mactxt + 8) == ' ')
					  || (*(mactxt + 8) == '>'))
					 && ((ifstate < 2) && (repeat < 2))) {
						mactxt += 8;
						whileend = mactxt;
						WhileCount = ++wcount;
						if ((whilestate == 2)
						 || (whilecond == NULL)
						 || (wcount >= WhileMax))
							whilestate = 0;
						else {
							mactxt = whilecond;
							whilestate = until ? (CheckCond(wf, &mactxt) ? 0 : 1)
							                   : (CheckCond(wf, &mactxt) ? 1 : 0);
						}
						if (whilestate == 0) {
							mactxt = whileend;
							whilecond = whileend = NULL;
							looptype = repeat ? 1 : 0;
						}
					}

					// break and continue work with both repeat and while
					else if (!_strnicmp(mactxt, "break", 5)
					 && ((*(mactxt + 5) == ' ')
					  || (*(mactxt + 5) == '>'))
					 && act) {
						mactxt += 5;
						if ((*mactxt == ' ')
						 && (*(mactxt + 1) == 'i')
						 && (*(mactxt + 2) == 'f')
						 && (*(mactxt + 3) == ' ')) {
							mactxt += 4;
							if (!CheckCond(wf, &mactxt))
								break;
						}
						if ((repeat == 1) && (looptype != 2)) {
							if (repend == NULL)
								repeat = 2;
							else {
								repeat = 0;
								looptype = whilestate ? 2 : 0;
								mactxt = repend;
							}
						}
						else if ((whilestate == 1) && (looptype == 2)) {
							if (whileend == NULL)
								whilestate = 2;
							else {
								whilestate = 0;
								looptype = repeat ? 1 : 0;
								mactxt = whileend;
							}
						}
					}
					else if (!_strnicmp(mactxt, "continue", 8)
					 && ((*(mactxt + 8) == ' ')
					  || (*(mactxt + 8) == '>'))
					 && act) {
						mactxt += 8;
						if ((*mactxt == ' ')
						 && (*(mactxt + 1) == 'i')
						 && (*(mactxt + 2) == 'f')
						 && (*(mactxt + 3) == ' ')) {
							mactxt += 4;
							if (!CheckCond(wf, &mactxt))
								break;
						}
						if ((repeat == 1) && (looptype != 2)) {
							if (dcount == 1) {
								RepDCount = dcount = 0;
								if (repend == NULL)
									repeat = 2;
								else {
									repeat = 0;
									mactxt = repend;
									looptype = whilestate ? 2 : 0;
								}
								repstart = repend = NULL;
							}
							else if (dcount > 1) {
								RepDCount = --dcount;
								RepCount = ++count;
								mactxt = repstart ? repstart : macstart;
								repeat = 1;
							}
						}
						else if ((whilestate == 1) && (looptype == 2)) {
							WhileCount = ++wcount;
							if (wcount >= WhileMax)
								whilestate = 0;
							else {
								mactxt = whilecond;
								whilestate = until ? (CheckCond(wf, &mactxt) ? 0 : 1)
								                   : (CheckCond(wf, &mactxt) ? 1 : 0);
							}
							if (whilestate == 0) {
								if (whileend == NULL)
									whilestate = 2;
								else {
									mactxt = whileend;
									looptype = repeat ? 1 : 0;
								}
								whilecond = whileend = NULL;
							}
						}
					}

					else if (act) {
						if (sys)
							mactxt--;
						WriteMacroName(wf, &mactxt, point); // sets ptr to end of name
						RepCount = count;  // restore globals on return from macro
						RepDCount = dcount;
						WhileCount = wcount;
					}
					break;
				}
				// else not macro ref, fall through
				goto normal;

			normal:
			default:
				if (act
				 && (!ValidOnly || (*mactxt < 128) || (*mactxt > 160)))
					wf->putChar(*mactxt);
#if 0
				if (act) {
					if (WriteMacroTextSingleChar)
						(*WriteMacroTextSingleChar)(wf, (*mactxt & 0xFF));
					else if (!ValidOnly
					 || (*mactxt < 128)
					 || (*mactxt > 160))
						wf->putChar(*mactxt);
				}
#endif
				break;
		}

		if ((*++mactxt == '\0') // end is implied end of loop if no <$end...>
		 && ((repeat == 1) || (whilestate == 1))) {
			if ((repeat == 1) && (looptype != 2)
			 && (dcount > 1)) {
				RepDCount = --dcount;
				RepCount = ++count;
				mactxt = repstart ? repstart : macstart;
				repeat = 1;
			}
			else if ((whilestate == 1) && (looptype == 2)
			 && ((wcount + 1) < WhileMax)) {
				WhileCount = ++wcount;
				mactxt = whilecond;
				whilestate = until ? (CheckCond(wf, &mactxt) ? 0 : 1)
				                   : (CheckCond(wf, &mactxt) ? 1 : 0);
				if (whilestate == 0)
					break;
				mactxt++;
			}
		}
	}

	MacroNestLevel--;
}


long
DCmacr::CheckCond(DCwfile *wf, char **ctxt)
{
	char *txt = EvalCond(wf, ctxt, false);
	char *etxt = NULL;
	int base = 10;
	long ret = 0;

	if (txt && *txt) {
		if (isdigit(*txt)) {  // numeric string goes by value
			if ((*txt == '0')
			 && ((*(txt + 1) == 'x')
				|| (*(txt + 1) == 'X')))
				base = 16;
			ret = (long) strtoul(txt, &etxt, base);
		}
		else  // non-numeric string with content is true
			ret = 1;
	}
	
	DeleteName(txt);
	return ret;
}


char *
DCmacr::EvalCond(DCwfile *wf, char **ctxt, bool sub)
{
	long ret = 1;
	char *ct = *ctxt;
	char *ltok = NULL;
	char *rtok = NULL;
	char *ntok = NULL;
	char *retstr = NULL;
	bool grouped = false;
	bool unary = false;
	bool ternary = false;
	char *nested = NULL;
	char *txt = NULL;
	long nc = 0;

	unc op = 0; // 0=unknown, 1=is, 2=is not, 3==, 4=<>, 5=<, 6=>, many more

	// ltok op rtok, where tok is an unquoted spaceless string or macrovar

	while (*ct == ' ')
		ct++;

	if (*ct == '(') {
		grouped = true;
		ct++;
		ltok = EvalCond(wf, &ct, true);
		if (*ct == ')')
			nested = ct++;
		else if (*ct == '>')
			nested = ct;
	}
	else {
		ltok = GetToken(wf, &ct);
		if (ltok == NULL) {
			*ctxt = ct;
			return NewName("0");
		}
	}

	int lbase = 0;
	long lval = GetLongTokVal(ltok, &lbase);

	while (*ct == ' ')
		ct++, nc++;

	if (isalpha(*ct)) {
		if (!strncmp(ct, "as ", 3)) {  // not an expression
			*ctxt = ct;
			return ltok;
		}

		if (!strncmp(ct, "is not ", 7))
			op = 2, ct += 6;
		else if (!strncmp(ct, "is ", 3))
			op = 1, ct += 2;
		else if (!strncmp(ct, "in ", 3))
			op = 31, ct += 2;
		else if (!strncmp(ct, "and not ", 8))
			op = 10, ct += 7;
		else if (!strncmp(ct, "and ", 4))
			op = 9, ct += 3;
		else if (!strncmp(ct, "or not ", 7))
			op = 12, ct += 6;
		else if (!strncmp(ct, "or ", 3))
			op = 11, ct += 2;
		else if (!strncmp(ct, "xor not ", 8))
			op = 32, ct += 7;
		else if (!strncmp(ct, "xor ", 4))
			op = 13, ct += 3;
		else if (!strncmp(ct, "plus ", 5))
			op = 26, ct += 4;
		else if (!strncmp(ct, "before ", 7))
			op = 27, ct += 6;
		else if (!strncmp(ct, "after ", 6))
			op = 28, ct += 5;
		else if (!strncmp(ct, "first", 5))
			op = 29, ct += 5;
		else if (!strncmp(ct, "last", 4))
			op = 30, ct += 4;
		else if (!strncmp(ct, "length", 6))
			op = 35, unary = true, ct += 6;
		else if (!strncmp(ct, "char ", 5))
			op = 36, ct += 4;
		else if (!strncmp(ct, "trim first", 10))
			op = 37, ct += 10;
		else if (!strncmp(ct, "trim last", 9))
			op = 38, ct += 9;
		else if (!strncmp(ct, "starts ", 7))
			op = 39, ct += 6;
		else if (!strncmp(ct, "ends ", 5))
			op = 40, ct += 4;
		else if (!strncmp(ct, "contains ", 9))
			op = 41, ct += 8;
		else if (!strncmp(ct, "strip ", 6))
			op = 42, ct += 5;
		else if (!strncmp(ct, "replace ", 8))
			op = 43, ternary = true, ct += 7;
		else if (!strncmp(ct, "lowercase", 9))
			op = 44, unary = true, ct += 9;
		else if (!strncmp(ct, "lower", 5))
			op = 44, unary = true, ct += 5;
		else if (!strncmp(ct, "uppercase", 9))
			op = 45, unary = true, ct += 9;
		else if (!strncmp(ct, "upper", 5))
			op = 45, unary = true, ct += 5;
	}
	else switch (*ct) {
		case '=':
			if (*(ct + 1) == '=')
				op = 3, ct += 2;
			else
				op = 3, ct++;
			break;
		case '!':
			if (*(ct + 1) == '=')
				op = 4, ct += 2;
			break;
		case '<':
			if (*(ct + 1) == '>')
				op = 4, ct += 2;
			else if (*(ct + 1) == '=')
				op = 6, ct += 2;
			else if (*(ct + 1) == '<')
				op = 19, ct += 2;
			else if (*(ct + 1) == ' ')
				op = 5, ct++;
			break;
		case '>':
			if (nested
			 && ((*nested == '>')
			 || (ct == (nested + 1 + nc))))
				goto nest;
			if (*(ct + 1) == '=')
				op = 8, ct += 2;
			else if (*(ct + 1) == '>')
				op = 20, ct += 2;
			else if (*(ct + 1) == ' ')
				op = 7, ct++;
			else
				goto nest;
			break;

		case '&':
			if (*(ct + 1) == '~')
				op = 15, ct += 2;
			else if (*(ct + 1) == '&') {
				if (*(ct + 2) == '!')
					op = 10, ct += 3;
				else 
					op = 9, ct += 2;
			}
			else
				op = 14, ct++;
			break;
		case '|':
			if (*(ct + 1) == '~')
				op = 17, ct += 2;
			else if (*(ct + 1) == '|') {
				if (*(ct + 2) == '!')
					op = 12, ct += 3;
				else 
					op = 11, ct += 2;
			}	
			else
				op = 16, ct++;
			break;
		case '^':
			if (*(ct + 1) == '~')
				op = 33, ct += 2;
			else if (*(ct + 1) == '!')
				op = 32, ct += 2;
			else
				op = 18, ct++;
			break;

		case '+':
			op = 21, ct++;
			break;
		case '-':
			op = 22, ct++;
			break;
		case '*':
			op = 23, ct++;
			break;
		case '/':
			op = 24, ct++;
			break;
		case '%':
			op = 25, ct++;
			break;

		case '?':
			op = 34, ternary = true, ct++;
			break;

		nest:
		case ')':
		default: 
			if (sub && nested)
				ct = nested;
			*ctxt = ct;
			return ltok;
	}

	while (*ct == ' ')
		ct++;

	if (*ct == '(') {
		ct++;
		rtok = EvalCond(wf, &ct, true);
		if (!sub)
			ct++;
	}
	else if (unary) {
		rtok = NewName("0");
	}
	else {
		rtok = GetToken(wf, &ct, (op == 31));
		if (rtok == NULL) {
			if ((op == 29) || (op == 30)  // first and last
			 || (op == 37) || (op == 38)) // trim first and last
				rtok = NewName("1");
			else {
				*ctxt = ct;
				return NewName("0");
			}
		}
	}

	int rbase = 0;
	long rval = GetLongTokVal(rtok, &rbase);

	if (ternary) { 
		while (*ct == ' ')
			ct++;
		switch (op) {
			case 34:    // l ? r : n
				if (*ct != ':') {
					*ctxt = ct;
					return NewName("0");
				}
				ct++;
				break;

			case 43:   // l replace r with n
				if (_strnicmp(ct, "with ", 5)) {
					*ctxt = ct;
					return ltok;
				}
				ct += 4;
				break;
			default:
				return NewName("0");
		}
		while (*ct == ' ')
			ct++;
		if (*ct == '(') {
			ct++;
			ntok = EvalCond(wf, &ct, true);
			if (!sub)
				ct++;
		}
		else {
			ntok = GetToken(wf, &ct, false);
			if (ntok == NULL) {
				*ctxt = ct;
				return (op == 43) ? ltok : NewName("0");
			}
		}
	}


	// evaluate

	switch (op) {
		case 0:  // not recognized
			break;

		case 1: // is
			if (*rtok == '\0')
				ret = (*ltok == '\0') ? 1 : 0;
			else if (*ltok == '\0')
				ret = 0;
			else
				ret = stricmp(ltok, rtok) ? 0 : 1;
			break;
		case 2: // is not
			if (*rtok == '\0')
				ret = (*ltok == '\0') ? 0 : 1;
			else if (*ltok == '\0')
				ret = 1;
			else
				ret = stricmp(ltok, rtok) ? 1 : 0;
			break;

		case 3: // =
			ret = (lval == rval) ? 1 : 0;
			break;
		case 4: // <> or !=
			ret = (lval != rval) ? 1 : 0;
			break;
		case 5: // <
			ret = (lval < rval) ? 1 : 0;
			break;
		case 6: // <=
			ret = (lval <= rval) ? 1 : 0;
			break;
		case 7: // >
			ret = (lval > rval) ? 1 : 0;
			break;
		case 8: // >=
			ret = (lval >= rval) ? 1 : 0;
			break;

		case 9:  // and
			ret = (lval && rval) ? 1 : 0;
			break;
		case 10: // and not
			ret = (lval && !rval) ? 1 : 0;
			break;
		case 11: // or
			ret = (lval || rval) ? 1 : 0;
			break;
		case 12: // or not
			ret = (lval || !rval) ? 1 : 0;
			break;
		case 13: // xor
			ret = (lval ^ rval) ? 1 : 0;
			break;
		case 32: // xor not
			ret = (lval ^ rval) ? 0 : 1;
			break;

		case 14: // &
			ret = (lval & rval);
			break;
		case 15: // &~
			ret = (lval & ~rval);
			break;
		case 16: // |
			ret = (lval | rval);
			break;
		case 17: // |~
			ret = (lval | ~rval);
			break;
		case 18: // ^
			ret = (lval ^ rval);
			break;
		case 33: // ^~
			ret = (lval ^ ~rval);
			break;

		case 19: // <<
			ret = (lval << rval);
			break;
		case 20: // >>
			ret = (lval >> rval);
			break;

		case 21:
			ret = (lval + rval);
			break;
		case 22:
			ret = (lval - rval);
			break;
		case 23:
			ret = (lval * rval);
			break;
		case 24:
			ret = (rval == 0) ? 0 : (lval / rval);
			break;
		case 25:
			ret = (rval == 0) ? 0 : (lval % rval);
			break;

		case 26: // plus
			retstr = NewName(strlen(ltok) + strlen(rtok));
			strcpy(retstr, ltok);
			strcat(retstr, rtok);
			break;

		case 27: // before
			txt = strstr(ltok, rtok);
			retstr = NewName(((txt == ltok) ? "" : ltok), (txt ? (txt - ltok) : 0));
			break;
		case 28: // after
			txt = strstr(ltok, rtok);
			retstr = NewName(txt ? (txt + strlen(rtok)) : "");
			break;

		case 29: // first
			retstr = NewName((rval ? ltok : ""), rval);
			break;
		case 30: // last
			retstr = NewName(ltok + (strlen(ltok) - rval));
			break;
		case 35: // unary string length
			ret = strlen(ltok);
			break;
		case 36: // string char at 1-based pos
			retstr = NewName((rval > 0) ? (ltok + (rval - 1)) : "", 1);
			break;
		case 37: // trim first
			if (rval >= (long) strlen(ltok))
				retstr = NewName("");
			else if (rval <= 0)
				retstr = NewName(ltok);
			else
				retstr = NewName(ltok + rval);
			break;
		case 38: // trim last
			if (rval >= (long) strlen(ltok))
				retstr = NewName("");
			else if (rval <= 0)
				retstr = NewName(ltok);
			else
				retstr = NewName(ltok, (strlen(ltok) - rval));
			break;
		case 39: // starts
			if (*rtok == '\0')
				ret = (*ltok == '\0') ? 1 : 0;
			else if (*ltok == '\0')
				ret = 0;
			else
				ret = strnicmp(ltok, rtok, strlen(rtok)) ? 0 : 1;
			break;
		case 40: // ends
			if (*rtok == '\0')
				ret = (*ltok == '\0') ? 1 : 0;
			else if (*ltok == '\0')
				ret = 0;
			else
				ret = stricmp(ltok + (strlen(ltok) - strlen(rtok)), rtok) ? 0 : 1;
			break;
		case 41: // contains
			if (*rtok == '\0')
				ret = (*ltok == '\0') ? 1 : 0;
			else if (*ltok == '\0')
				ret = 0;
			else
				ret = strstr(ltok, rtok) ? 1 : 0;
			break;

		case 31: // in list[]
			if ((txt = strstr(rtok, "[]")) != NULL)
				*txt = '\0';
			retstr = GetMacListIndex(rtok + 2, ltok);
			break;

		case 34: // ternary operator
			retstr = NewName(lval ? rtok : ntok);
			break;

		case 42:  // strip chars in rtok from ltok
			retstr = StripChars(ltok, rtok);
			break;

		case 43:  // in ltok, replace str in rtok with ntok
			retstr = ReplaceChars(ltok, rtok, ntok);
			break;

		case 44:  // lowercase ltok
			retstr = NewName(ltok);
			_strlwr(retstr);
			break;

		case 45:  // uppercase ltok
			retstr = NewName(ltok);
			_strupr(retstr);
			break;

		// next available case is 46

		default:
			break;
	}

	DeleteName(ltok);
	DeleteName(rtok);

	while (*ct == ' ')
		ct++;
	if (grouped && (*ct == ')'))
		ct++;

	*ctxt = ct;  // set ptr at end of expression

	if (!retstr) {  // convert numeric result to string
		int nbase = lbase ? lbase : (rbase ? rbase : 10);
		if (nbase == 16)  // must provide own prefix
			_itoa(ret, (retstr = NewName("0x", 19)) + 2, 16);
		else
			_itoa(ret, retstr = NewName(17), nbase);
	}

	return retstr;
}


char *
DCmacr::StripChars(char *ltok, char *rtok)
{
	char *retstr = NewName(strlen(ltok));
	char *ckstr = NULL;
	char nstr[5];
	uns i = 0;
	uns j = 0;

	for (i = 0; ltok[i]; i++) {
		if ((ltok[i] < ' ')
		 && (ltok[i] > 0)) {
			switch (ltok[i]) {
				case 9: // tab
					ckstr = "\t";
					break;
				case 10: // nl
					ckstr = "\n";
					break;
				case 13: // cr
					ckstr = "\r";
					break;
				default:
					sprintf(nstr, "\\x%0.2x", (int) ltok[i]);
					ckstr = nstr;
					break;
			}
			if (ckstr
			 && !strstr(rtok, ckstr))
				retstr[j++] = ltok[i];
		}
		else if (!strchr(rtok, ltok[i]))
			retstr[j++] = ltok[i];
	}
	retstr[j] = '\0';

	return retstr;
}


char *
DCmacr::ReplaceChars(char *ltok, char *rtok, char *ntok)
{
	if (!ltok || !*ltok
	 || !rtok || !*rtok
	 || !ntok || !*ntok)
		return NewName(ltok);

	short llen = strlen(ltok);
	short rlen = strlen(rtok);
	short nlen = strlen(ntok);
	short sublen = nlen - rlen;

	char *str = ltok;
	char *sst = NULL;
	char *retstr = NULL;

	if (sublen != 0) {  // count substitutions
		short cnt = 0;
		while ((sst = strstr(str, rtok)) != NULL) {
			cnt++;
			str = sst + rlen;
		}
		if (!cnt)
			return NewName(ltok);

		retstr = NewName(llen + (sublen * cnt));
		str = ltok;
	}
	else
		retstr = NewName(llen);

	while ((sst = strstr(str, rtok)) != NULL) {
		if (sst > str)
			strncat(retstr, str, sst - str);
		strcat(retstr, ntok);
		str = sst + rlen;
	}
	if (*str)
		strcat(retstr, str);

	return retstr;
}


char *
DCmacr::GetToken(DCwfile *wf, char **tptr, bool lit)
{
	char *tok = *tptr;

	while (*tok == ' ')
		tok++;

	char *etok = tok;
	size_t len = 0;
	bool list = false;
	char *lix = NULL;
	char *elix = NULL;
	char *nn = NULL;

	bool deref = (*tok == '*') ? true : false;

	if (deref) { // dereferencing
		tok++;     // go to first $
		etok++;
	}

	if ((*tok == '>')
	 || (*tok == ')')) {
		*tptr = tok;
		return NULL;
	}

	if (*tok == '"') {  // double-quoted string
		tok++;
		while (*++etok) {
			if (*etok == '"')
				break;
			if (*etok == '\\')
				etok++, len++;
			len++;
		}
		if (*etok == '"')
			etok++;
	}
	else if (*tok == '\'') {  // either char lit or string
		if ((*(tok + 1) == '\\')      // '\x'
		 && (*(tok + 3) == '\''))
			etok = tok + 4, len = 4;
		else if (*(tok + 1) == '\'')  // '' (empty)
			etok = tok + 2, len = 2;
		else if (*(tok + 2) == '\'')  // 'x'
			etok = tok + 3, len = 3;
		else {                        // single-quoted string
			tok++;
			while (*++etok) {
				if (*etok == '\'')
					break;
				if (*etok == '\\')
					etok++, len++;
				len++;
			}
			if (*etok == '\'')
				etok++;
		}
	}
	else for (etok = tok; *etok; etok++) {
		if ((*etok == ' ')
		 || (*etok == ')')
		 || (*etok == '=')
		 || (*etok == '!')
		 || (*etok == '&')
		 || (*etok == '|')
		 || (*etok == '^')
		 || (*etok == '+')
		 || (*etok == '-')
		 || (*etok == '*')
		 || (*etok == '/')
		 || (*etok == '%')
		 || (*etok == '?')
		 || (*etok == '<')
		 || (*etok == '>'))
			break;
		if (*etok == '[') {
			list = true;
			lix = etok;
		}
		else if (*etok == ']')
			elix = etok;
		len++;
	}

	while (*etok == ' ')
		etok++;
	*tptr = etok;

	if (*tok == '$') {
		char tch = *etok;
		char *var = NULL;
		DCblock *bp = NULL;
    *etok = '\0';
		if ((strchr(tok, '\\')
		  || strchr(tok, '/'))
		 && ((bp = GetMacroFile(tok + 1)) != NULL))
			var = (char *) bp->p();
		*etok = tch;
		if (var)
			return (var);
	}

	if (!lit
	 && (*tok == '$')
	 && (*(tok + 1) == '$')) {
		char *npos = tok + 2;
		size_t nlen = len - 2;
		bool sys = false;
		if (*npos == '_') {
			sys = true;
			npos++;
			nlen--;
		}
		bool initype = false;
		char *lname = NULL;
		char *lindx = NULL;
		char *ntok = NULL;
		if (deref && list) {
			lname = SplitListName(npos, &lindx, &initype, lix, elix);
#if 0
			if (lix == npos) {  // ini syntax, [var]idx
				initype = true;
				npos++;
				nlen = elix - npos;
			}
			else                // C syntax, var[idx]
				nlen = lix - npos;
#endif
			ntok = lname;
		}
		else
			ntok = NewName(npos, nlen);
		char *var = GetMacroVar(wf, ntok, sys);
		//char *varp = var;
		if (deref) {
			//if ((*var == '$') && (*(var + 1) == '$'))
			//	var += 2;
			ntok = GetPtrVal(wf, var);
			//if (ntok != var)
			//	DeleteName(varp);
			if (list) {
				lname = ntok;
				var = CombineListName(ntok, lindx, initype);
			}
			else
				var = ntok;
#if 0
			int cref = 0;  // circular reference stopper
			while ((*var == '$') && (*(var + 1) == '$')) {
				char *nvar = GetMacroVar(wf, var);
				if (!nvar)
					break;
				if (!strcmp(nvar, var)  // self-referential stopper
				 || (cref++ > 128)) {
					DeleteName(nvar);
					break;
				}
				//DeleteName(var);
				var = nvar;
			}
			if (list) {  // put index part back on
				if (initype) {   // [var]idx
					nn = NewName("[", strlen(var + 2) + strlen(elix) + 1);
					strcat(nn, var + 2);
					strcat(nn, elix);
				}
				else {           // var[idx]
					nn = NewName(var + 2, strlen(var + 2) + strlen(lix));
					strcat(nn, lix);
				}
				//DeleteName(var);
				var = GetMacroVar(wf, nn);
			}
#endif
		}
		DeleteName(ntok);
		return var;
	}

	return NewName(len ? tok : "", len);
}


long
DCmacr::GetLongTokVal(char *tok, int *base)
{
	char *tend = NULL;
	*base = ((*tok == '0')
	     && ((*(tok + 1) == 'x')
	      || (*(tok + 1) == 'X'))) ? 16 : 10;

	long val = (long) strtoul(tok, &tend, *base);

	if (!val && (tend == tok)) {
		*base = 0; // not a number
		if (*tok == '\'') {  // character constant
			if (*(tok + 1) == '\'')  // empty
				val = 0;
			else if ((*(tok + 1) == '\\')
			 && (*(tok + 3) == '\''))
				switch (*(tok + 2)) {
					case 'r':
						val = 13;
						break;
					case 'n':
						val = 10;
						break;
					default:
						val = ((long) *(tok + 2)) & 0xFF;
						break;
				}
			else if (*(tok + 2) == '\'')
				val = ((long) *(tok + 1)) & 0xFF;
		}
	}

	return val;
}


char *
DCmacr::GetMacroVar(DCwfile *wf, char *vp, bool sys)
{
	char *var = NULL;

	if (*vp == '_') {
		sys = true;
		vp++;
	}

	if (strchr(vp, '['))
		return GetMacListValue(wf, vp, sys);

	else if (sys && ((var = GetPredefinedVar(wf, vp)) != NULL))
		return var;
	else if (sys && ((var = GetMacroLoopVar(vp)) != NULL))
		return var;

	// or user defined variables
	else if ((var = GetCodeStoreVar(wf, vp)) != NULL)
		return var;
	else if ((var = GetUserMacVar(vp)) != NULL)
		return var;

	else if (!sys && ((var = GetPredefinedVar(wf, vp)) != NULL))
		return var;
	else if (!sys && ((var = GetMacroLoopVar(vp)) != NULL))
		return var;

	else  // all else failed, put in literally
		return NewName(sys ? (vp - 1) : vp);
}


char *
DCmacr::GetMacroLoopVar(char *vp)
{
	if (!stricmp(vp, "count"))
		return _itoa(RepCount, NewName(NumberStrMax), 10);
	if (!stricmp(vp, "dcount"))
		return _itoa(RepDCount, NewName(NumberStrMax), 10);
	if (!stricmp(vp, "wcount"))
		return _itoa(WhileCount, NewName(NumberStrMax), 10);
	return NULL;
}


char *
DCmacr::GetUserMacVar(char *vp, bool fmvar)
{
	long pos = 0;
	char *txt = NULL;

	if ((txt = GetTextStore(vp)) != NULL)
		return txt;
	else if ((pos = MacroVarNames(vp)) != 0)
		return (NewName(MacroVarVals.find(pos)));
	else if (IniFile
	 && IniFile->Section("MacroVariables")
	 && IniFile->Find(vp))
		return (IniFile->StrRight());
	else if (IniFile
	 && IniFile->Section("UserVars")
	 && IniFile->Find(vp))
		return (IniFile->StrRight());
	else if (MacroIniFile
	 && MacroIniFile->Section("MacroVariables")
	 && MacroIniFile->Find(vp))
		return (MacroIniFile->StrRight());

	// look in Frame file for macro def as user variable
	else if (fmvar
	 && ((txt = GetFrameUserVar(vp)) != NULL))
		return txt;

	return NULL;
}


DCnlist DCmacr::MacListNames;    // relates list name to listnum
DCvlist DCmacr::MacListIndexes;  // relates ptr to list's indexes to listnum
DCvlist DCmacr::MacListValues;   // relates ptr to list's values to listnum
DCnlist DCmacr::MacListDefaults; // relates default list value to listnum
DCilist DCmacr::MacListCounts;   // specifies next num for each listnum
long DCmacr::MacListCount = 1;


char *
DCmacr::GetMacListValue(DCwfile *wf, char *vp, bool sys)
{
	char *lix = strchr(vp, '[');
	if (!lix)
		return NewName("0");

	size_t len = strlen(vp);

	char *lname = NULL;
	char *lindx = NULL;
	bool initype = false;

	lname = SplitListName(vp, &lindx, &initype, lix);

#if 0
	if (lix == vp) {  // ini syntax
		initype = true;
		lix = strchr(vp, ']');
		lname = NewName(vp + 1, (lix - vp) - 1);
		lindx = NewName(lix + 1, strlen(lix) - 1);
	}
	else {  // array syntax
		lname = NewName(vp, lix - vp);
		lindx = NewName(lix + 1, strlen(lix) - 2);
	}
#endif

	char *val = GetMacListValue(wf, lname, lindx, initype || sys);
	DeleteName(lname);
	DeleteName(lindx);

	return val;
}


char *
DCmacr::GetMacListValue(DCwfile *wf, char *lname, char *lindx, bool ini)
{
	long listnum = 0;
	DCnlist *lilist = NULL;
	DCnlist *lvlist = NULL;
	long linum = 0;
	const char *txt = NULL;
	char nbuf[17];

	lindx = GetMacListIxVal(wf, lindx);

	if (ini)  // check for ini references first (or sys list refs)
		return DCini::GetStrVal(lname, lindx);

	GetMacListInfo(lname, &lilist, &lvlist, &listnum);

	if (*lindx == '\0')
		txt = ltoa(MacListCounts.find(listnum), nbuf, 10);
	else {
		linum = (*lilist)(lindx);
		txt = linum ? lvlist->find(linum) : NULL;
	}

	DeleteName(lindx);

	if (!txt)
		txt = MacListDefaults.find(listnum);

	return NewName(txt ? txt : "0");
}


char *
DCmacr::GetMacListIndex(char *lname, char *lval)
{
	DCnlist *lilist = NULL;
	DCnlist *lvlist = NULL;
	long linum = 0;

	GetMacListInfo(lname, &lilist, &lvlist);
	linum = (*lvlist)(lval);

	const char *txt = linum ? lilist->find(linum) : NULL;

	return NewName(txt ? txt : "0");
}


void
DCmacr::SetMacListValue(DCwfile *wf, char *lname, char *lindx, char *lval,
												 bool ini)
{
	long listnum = 0;
	DCnlist *lilist = NULL;
	DCnlist *lvlist = NULL;
	long linum = 0;

	lindx = GetMacListIxVal(wf, lindx);

	if (ini) {  // use callback through main module to queue as needed
		ProcMacroSetting(lname, lindx, lval);
		return;
	}

	GetMacListInfo(lname, &lilist, &lvlist, &listnum);

	if (!strcmp(lindx, "0")) {
		MacListDefaults.replace(lval, listnum);
		return;
	}

	linum = (*lilist)(lindx);

	if (linum == 0) {
		linum = MacListCounts.find(listnum);
		lilist->add(lindx, linum);
		MacListCounts.replace(linum + 1, listnum);
	}
	else
		DeleteName(lindx);

	lvlist->replace(lval, linum);
}


void
DCmacr::GetMacListInfo(char *lname, DCnlist **pilist, DCnlist **pvlist, long *plistnum)
{
	long listnum = 0;
	long count = 0;
	DCnlist *lilist = NULL;
	DCnlist *lvlist = NULL;
	char nbuf[17];
	char *cpt = NULL;

	if ((listnum = MacListNames(lname)) == 0) { // new list needed
		MacListNames.addnew(lname, listnum = MacListCount);
		MacListIndexes.add(lilist = new DCnlist, MacListCount);
		MacListValues.add(lvlist = new DCnlist, MacListCount);
		MacListCounts.append(1, MacListCount);

		if (IniFile
		 && IniFile->Section(lname)) {  // load from .ini
			uns pos = 0;
			while (IniFile->FindPos(++pos) ) {  // not last
				lilist->add(IniFile->StrLeft(), (long) pos);
				lvlist->add(IniFile->StrRight(), (long) pos);
			}
			MacListCounts.replace(pos, MacListCount);
		}
		else if ((count = GetFrameUserArrVar(lname, 0)) != 0) {
			for (long i = 1; i <= count; i++) {
				if ((cpt = (char *) GetFrameUserArrVar(lname, i)) != NULL) {
					lilist->add(NewName(ltoa(i, nbuf, 10)), i);
					lvlist->add(cpt, i);
				}
			}
			MacListCounts.replace(count, MacListCount);
		}

		MacListCount++;
	}
	else {
		lilist = (DCnlist *) MacListIndexes.find(listnum);
		lvlist = (DCnlist *) MacListValues.find(listnum);
	}

	*pilist = lilist;
	*pvlist = lvlist;

	if (plistnum)
		*plistnum = listnum;
}


char *
DCmacr::GetMacListIxVal(DCwfile *wf, char *lindx)
{
	char *var = NULL;
	char *nindx = NULL;

	if ((*lindx == '$') && (*(lindx + 1) == '$'))
		nindx = GetMacroVar(wf, lindx + 2);
	else if (*lindx == '(')
		nindx = EvalCond(wf, &lindx);
	else
		nindx = NewName(lindx);

	return nindx;
}


char *
DCmacr::SplitListName(char *fname, char **plindx, bool *pinitype,
	                    char *lix, char *elix, char *term)
{
	char *lname = NULL;

	if (!fname)
		return NULL;
	if (!lix)
		lix = strchr(fname, '[');
	if (lix && !elix)
		elix = strchr(lix, ']');
	if (!lix || !elix)
		return fname;  //can't split it
	if (!term)
		term = fname + strlen(fname);

	if (lix == fname) {  // ini syntax
		*pinitype = true;
		lname = NewName(fname + 1, (elix - lix) - 1);
		if (*(elix + 1) == '"') { // quoted key
			char *equote = strchr(elix + 2, '"');
			if (equote)  // if no closing, use term
				term = equote;
			elix++;
		}
		if ((term - elix) == 1)  // empty
			*plindx = NewName("");
		else
			*plindx = NewName(elix + 1, (term - elix) - 1);
	}
	else {  // array syntax
		*pinitype = false;
		lname = NewName(fname, lix - fname);
		if ((elix - lix) == 1)  // empty
			*plindx = NewName("");
		else
			*plindx = NewName(lix + 1, (elix - lix) - 1);
	}

	return lname;
}


char *
DCmacr::CombineListName(char *ptname, char *lindx, bool initype)
{
	char *nn = NewName(strlen(ptname) + strlen(lindx) + 3);
	if (initype) {
		strcat(nn, "[");
		strcat(nn, ptname);
		strcat(nn, "]");
		strcat(nn, lindx);  // if quoted, quotes were dropped
	}
	else {
		strcat(nn, ptname);
		strcat(nn, "[");
		strcat(nn, lindx);
		strcat(nn, "]");
	}
	return nn;
}


char *
DCmacr::GetPtrVal(DCwfile *wf, char *ptname)
{
	for (int cref = 0; cref < 128; cref++) {
		char *nval = GetMacroVar(wf, ptname);
		if (!nval)
			break;
		if (((*nval != '$') || (*(nval + 1) != '$')) // not a pointer
		 || (!stricmp(nval + 2, ptname))) {  // self-referential stopper
			DeleteName(nval);
			break;
		}
		DeleteName(ptname);
		ptname = NewName(nval + 2);
		DeleteName(nval);
	}
	return ptname;
}


DCnlist DCmacr::MacroVarNames;
DCnlist DCmacr::MacroVarVals;
long DCmacr::MacroVarCount = 0;
char *DCmacr::MacroParam = NULL;


void
DCmacr::WriteMacroName(DCwfile *wf, char **pmacname, bool point)
{
	char *cpt = *pmacname;
	char *first = cpt;
	char *start = cpt;
	char *term = NULL;
	char *macfunc = NULL;
	bool macvar = false;
	bool expr = false;
	bool assign = false;
	bool incr = false;
	bool decr = false;

	bool sys = false;
	bool list = false;
	bool initype = false;
	char *lix = NULL;
	char *elix = NULL;
	char *lname = NULL;
	char *lindx = NULL;

	bool rsys = false;
	bool deref = false;
	bool rlist = false;
	bool rinitype = false;
	char *rlix = NULL;
	char *relix = NULL;
	char *rname = NULL;
	char *rindx = NULL;

	char *macname = NULL;
	long pos = 0;
	char *txt = NULL;
	char *val = NULL;

	long div = 0;
	long cdiv = 0;
	//int cref = 0;  // circular reference stopper


	if ((macvar = (*first == '$') ? true : false) == true)
		first++;
	if ((sys = (*first == '_') ? true : false) == true)
		first++;

	for (; *cpt; cpt++) {
		if (*cpt == ' ') {
			if (!term)
				term = cpt;
			while (*++cpt == ' ')
				;
			if ((*cpt == 'a')
			 && (*(cpt + 1) == 's')
			 && (*(cpt + 2) == ' '))
				break;
		}

		if (*cpt == '(') {  // direct expression
			expr = true;
			break;
		}
		else if (macvar
		 && (*cpt == '=')) {  // variable assignment
			if (!term)
				term = cpt;
			assign = true;
			break;
		}
		else if (macvar
		 && (*cpt == '+')) {  // variable increment
			if (!term)
				term = cpt;
			if (*(cpt+1) == '=') {  // plus assign
				assign = true;
				cpt++;
			}
			incr = true;
			break;
		}
		else if (macvar
		 && (*cpt == '-')) {  // variable decrement
			if (!term)
				term = cpt;
			decr = true;
			break;
		}
		else if (macvar
		 && (*cpt == '[')) {  // list reference, get it all
			list = true;
			if (!lix)
				lix = cpt;
		}
		else if (macvar
		 && (*cpt == ']')) {  // list reference, get ending
			list = true;
			if (!elix)
				elix = cpt;
		}

		else if (*cpt == '>') { // end of macro
			if (!term)
				term = cpt;
			break;
		}
	}

	if (!term)
		term = cpt;

	if (expr) {
		if (first < term) // macro with an arg
			macfunc = NewName(first, term - first);
		val = EvalCond(wf, &cpt);
		if (*cpt == ')')
			cpt++;
		if (macfunc) {
			MacroParam = val;
			*pmacname = cpt;
		}
		else {
			if (*cpt != '>')  // more to do
				WriteMacroFormatted(wf, &cpt, val);
			else
				WriteMacro(wf, val);
			*pmacname = cpt;
			return;
		}
	}

	if (macfunc)
		macname = macfunc;
	else
		macname = NewName(first, term - first); // skip the $ and _

	if (list)
		lname = SplitListName(first, &lindx, &initype, lix, elix, term);

	if (point) {
		char *ptname = NewName(list ? lname : macname);
		ptname = GetPtrVal(wf, ptname);
		DeleteName(macname);
		if (list) {
			lname = ptname;
			macname = CombineListName(ptname, lindx, initype);
		}
		else
			macname = ptname;
	}

	if (assign || incr || decr) {
		if (assign) {      // variable assignment statement <$$name=val>
			while (*++cpt == ' ')
				;
			first = cpt;     // now refers to first char on right of =
			if ((*cpt == '<')  // for compatibility with old syntax
			 && (*(cpt + 1) == '$')
			 && MacroVarNesting) {
				first = ++cpt;
				while (*cpt && (*cpt != '>') && (*cpt != ' ')) {
					if (*cpt == '[')
						rlist = true;
					cpt++;
				}
				term = cpt;
				if (*cpt == '>')  // skip nested closing
					cpt++;
			}
			else
				term = NULL;

			if ((deref = (*first == '*') ? true : false) == true)
				first++, cpt++;

			if ((*first == '$')  // current syntax doesn't use <>s
			 && (*(first + 1) == '$')) {  // macvar on right
				first += 2;

				if (*first == '_') {
					rsys = true;
					first++;
				}

				if (!term) {
					while (*cpt && (*cpt != '>') && (*cpt != ' ')) {
						if (*cpt == '[') {
							rlix = cpt;
							rlist = true;
						}
						if (*cpt == ']') {
							relix = cpt;
							rlist = true;
						}
						cpt++;
					}
					term = cpt;
				}
				val = NewName(first, term - first);

				if (rlist)
					rname = SplitListName(val, &rindx, &rinitype);

				if (deref) {
					char *rptname = NewName(rlist ? rname : val);
					rptname = GetPtrVal(wf, rptname);
					DeleteName(val);
					if (rlist) {
						rname = rptname;
						val = CombineListName(rptname, rindx, rinitype);
					}
					else
						val = rptname;
				}

				if ((txt = GetMacroVar(wf, val, rsys)) != NULL) {
					DeleteName(val);
					val = txt;
				}

#if 0
				if (rlist) {
					txt = GetMacListValue(wf, val, rsys);
					DeleteName(val);
					val = txt;
				}
				else if (rsys && ((txt = GetPredefinedVar(val)) != NULL)) {
					DeleteName(val);
					val = txt;
				}
				else if ((txt = GetUserMacVar(val)) != NULL) {
					DeleteName(val);
					val = txt;
				}
				else if (!sys && ((txt = GetPredefinedVar(val)) != NULL)) {
					DeleteName(val);
					val = txt;
				}
#endif

			}
			else if (*cpt == '(') {  // val is expression
				val = EvalCond(wf, &cpt);
				if (*cpt == ')')
					cpt++;
			}
			else if (*cpt == '"') {  // quoted string
				first++;
				while (*++cpt) {
					if (*cpt == '"')
						break;
					if (*cpt == '\\')
						cpt++;
				}
				val = NewName(first, cpt - first);
				if (*cpt == '"')
					cpt++;
			}
			else if (*cpt == '\'') { // char constant
				if ((*(cpt + 1) == '\\') && (*(cpt + 3) == '\''))
					cpt += 4;
				else if (*(cpt + 2) == '\'')
					cpt += 3;
				else {  // or single-quoted string
					first++;
					while (*++cpt) {
						if (*cpt == '\'')
							break;
						if (*cpt == '\\')
							cpt++;
					}
				}
				val = NewName(first, cpt - first);
				if (*cpt == '\'')
					cpt++;
			}
			else {  // val is unquoted string
				while (*cpt && (*cpt != '>'))  // go to end, no formatting
					cpt++;
				val = NewName(first, cpt - first);
			}
		}

		else if (incr || decr) {  // variable increment/decrement <$$name+> or -
			if (*(cpt + 1) == (incr ? '+' : '-'))  // allow var++ or var-- forms
				cpt++;
			while (*++cpt == ' ')
				;
			if (list)
				val = GetMacListValue(wf, lname, lindx, initype);
			else if ((val = GetUserMacVar(macname)) == NULL)
				val = NewName("000");
			IncName(val, incr);
		}

		if (list) {
			SetMacListValue(wf, lname, lindx, val, initype);
			DeleteName(lname);
			DeleteName(lindx);
		}
		else {
			if ((pos = MacroVarNames(macname)) == 0) // not in list already
				MacroVarNames.add(macname, pos = ++MacroVarCount);
			MacroVarVals.replace(val, pos);
		}

		if (*cpt != '>')  // more to do
			WriteMacroFormatted(wf, &cpt, val);

		*pmacname = cpt;
		return;
	}

	// must be a simple macro or macrovar invocation

	// macname = NewName(first, term ? term - first : 0);

	if (strchr(macname, '\\')
	 || strchr(macname, '/')) {
		DCblock *bp;
		if ((bp = GetMacroFile(macname)) != NULL) {
			WriteMacro(wf, (char *) bp->p());
			delete bp;
		}
	}
	else if (macvar) {  // system or user defined vars
		char *mn = (*macname == '$') ? macname + 1 : macname;  //temporarily
		if (*mn == '_') {
			sys = true;
			mn++;
		}

		if (list) {
			txt = GetMacListValue(wf, mn, sys);
			if (*term == '>')
				WriteMacro(wf, txt);
			else
				WriteMacroFormatted(wf, &++term, txt);
			DeleteName(txt);
		}
		else if ((sys && ((txt = GetPredefinedVar(wf, mn)) != NULL))
		 || (sys && ((txt = GetMacroLoopVar(mn)) != NULL))) {
			if (*cpt == '>')
				wf->putStr(txt);
			else
				WriteMacroFormatted(wf, &cpt, txt);
		}

		// or user defined variables
		else if (WriteCodeStoreVar(wf, mn))
			;
		else if ((txt = GetUserMacVar(mn)) != NULL) {
			if (*term == '>')
				WriteMacro(wf, txt);
			else
				WriteMacroFormatted(wf, &++term, txt);
			DeleteName(txt);
		}
		else if ((!sys && ((txt = GetPredefinedVar(wf, mn)) != NULL))
		 || (!sys && ((txt = GetMacroLoopVar(mn)) != NULL))) {
			if (*cpt == '>')
				wf->putStr(txt);
			else
				WriteMacroFormatted(wf, &cpt, txt);
		}

		else if (NameUndefMacroVars) // all else failed, put in undefined macrovar literally
			wf->putStr(mn);
	}

	// process ini-file macros
	else if (sys  // new syntax for predefined macros
	 && WritePredefinedMacro(wf, macname))
		MacroParam = NULL;
	else if (IniFile
	 && IniFile->Section(macname))
		ProcMacroSection(wf, IniFile, macname);
	else if (MacroIniFile
	 && MacroIniFile->Section(macname))
		ProcMacroSection(wf, MacroIniFile, macname);
	else if (WritePredefinedMacro(wf, macname))
	  // not user, see if default exists as sys
		MacroParam = NULL;

	// undefined macros are ignored totally by design
	else if (NameUndefMacros)  // unless this debug switch is on
		wf->putStr(macname);

	DeleteName(macname);
	while (*cpt && (*cpt != '>'))
		cpt++;
	*pmacname = cpt;
}


void
DCmacr::ProcMacroSection(DCwfile *wf, DCirfile *file, char *name)
{
	char *txt = file->SecCont(true);
	if (!txt)
		return;

	// trim off trailing returns if last line ends with backslash
	char *etxt = (txt + strlen(txt)) - 1;
	while ((etxt > txt)
	 && ((*etxt == '\n')
	  || (*etxt == '\r')))
		etxt--;
	if (*etxt == '\\') // remove ending \\ and returns
		*etxt = '\0';
	else if (!OmitMacroReturns) // leave just one \n at end
		*(etxt + 2) = '\0';
	else
		*++etxt = '\0';   // remove ending \n

	WriteMacro(wf, txt);
	DeleteName(txt);
}


void
DCmacr::WriteMacroFormatted(DCwfile *wf, char **fmt, char *val)
{
	char *cpt = *fmt;

	while (*cpt == ' ')
		cpt++;

	if (_strnicmp(cpt, "as ", 3))  // format string, write result
		return;

	cpt += 2;
	while (*++cpt == ' ')
		;

	bool cform = false;
	bool nform = false;
	bool sform = false;

	char *first = cpt;
	while (*cpt && (*cpt != '>')) {
		if (*cpt == ' ')
			break;
		switch (*cpt) {
			case 'c':
				cform = true;
				break;

			case 'd':
			case 'i':
			case 'n':
			case 'o':
			case 'u':
			case 'x':
			case 'X':
				nform = true;
				break;

			case 's':
				sform = true;
				break;

			default:
				break;
		}
		cpt++;
	}
	char *nfmt = (cpt > first) ? NewName(first, cpt - first) : NULL;

	long num = 0;

	if (nfmt && (nform || cform)) {  // numeric or char
		if (*val == '\'') {
			val++;
			if (*val == '\\') {
				switch (*++val) {
					case 'n':
						num = 10;
						break;
					case 'r':
						num = 13;
						break;
					case 't':
						num = 9;
						break;
					case '0':
						num = 0;
						break;
					default:
						num = *val;
						break;
				}
			}
			else
				num = *val;
		}
		else
			num = atol(val); // numeric value
		char *nval = NewName(128);
		sprintf(nval, nfmt, cform ? (num & 0xFF) : num);
		WriteMacro(wf, nval);
		DeleteName(nval);
	}
	else if (nfmt && sform) {        // string
		char *sval = NewName(strlen(val) + 128);
		sprintf(sval, nfmt, val);
		WriteMacro(wf, sval);
		DeleteName(sval);
	}
	else  // unformatted
		WriteMacro(wf, val);

	if (nfmt)
		DeleteName(nfmt);

	while (*cpt && (*cpt != '>'))
		cpt++;

	*fmt = cpt;
}


// setting functions


bool 
DCmacr::ProcMacroSetting(char *sect, char *key, char *val)
{
	// this is write phase
	if (key && (*key == '\0'))
		key = NULL;

	if (!sect
	 || !DCini::SetSect(sect, false))
		return false;  // invalid section name, forget it

	if (DCini::CurrSect->sectype == set) {  // global
		if (!key || !val)  // require key and val
			return false;
		return DCini::ProcMarkSect(sect, key, val); // do now
	}

	if ((DCini::CurrSect->sectype == cstyle)  // char setting
	 || (DCini::CurrSect->sectype == pstyle)  // para setting
	 || (DCini::CurrSect->sectype == pcstyle)) {  // char or para setting
		if (DCini::CheckCurrFormName(key))
			return DCini::ProcMarkSect(sect, key, val); // do now
	}

	// store in list for object
	setting *s = new setting;  
	s->sect = NewName(sect);
	s->key = key ? NewName(key) : NULL;
	s->val = NewName(val);

	if (DCini::CurrSect->sectype == tabl) {  // table setting
		DCini::TblSetList.add((long) s);
		return false;
	}
	if (DCini::CurrSect->sectype == graphic) {  // graphic setting
		DCini::GraphSetList.add((long) s);  // retrieve at next graphic
		return false;
	}
	if (DCini::CurrSect->sectype == xrstyle) {  // xref setting
		DCini::XrefSetList.add((long) s);  // retrieve at next xref
		return false;
	}
	DCini::CurrPendList.add((long) s);  // whatever is left is for Curr
	return true;
}



// end of dcmacr.cpp



