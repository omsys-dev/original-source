// dcini.cpp contains code for the dcl ini class, JHG, 3/20/05
// Copyright (c) 2005 by Omni Systems, Inc.  All rights reserved.

#include "dcappl.h"
#include "dcfili.h"
#include "dcini.h"

DCirfile *DCini::IniFile = NULL;
secinfo *DCini::IniSects = NULL;
bool DCini::IsIni = false;
bool DCini::Persistent = false;

secinfo *DCini::CurrSect = NULL;
spprop *DCini::Curr = NULL;
tbprop *DCini::Tbl = NULL;
anyini *DCini::TblIni = NULL;
anyini *DCini::Gr = NULL;
anyini *DCini::XrefIni = NULL;
char **DCini::Xrnmp = NULL;
char **DCini::Fmnmp = NULL;


// initialization

void
DCini::InitIniModule(DCirfile *ifl, secinfo *isects,
      spprop *curr, tbprop *tbl, 
			anyini *gr, anyini *tbini, anyini *xrini,
			char **xrnmp, char **fmnmp)
{
	if (ifl)
		IniFile = ifl;

	if (isects)
		IniSects = isects;

	if (curr)
		Curr = curr;

	if (tbl)
		Tbl = tbl;

	if (gr)
		Gr = gr;

	if (tbini)
		TblIni = tbini;

	if (xrini)
		XrefIni = xrini;

	if (xrnmp)
		Xrnmp = xrnmp;

	if (fmnmp)
		Fmnmp = fmnmp;
}


// ini-file processing

void
DCini::ProcAllSects(void)
{
	secinfo *isec = NULL;
	setinfo *is = NULL;
	IsIni = true;

	for (isec = IniSects; isec->all != false; isec++) {
		if (IniFile->Section(isec->secname)
		 //&& isec->all  // flag for sections in starting batch
		 && (isec->sectype == set)) {  // only proc set names
			CurrSect = isec;
			for (is = isec->setarr; is->sname != NULL; is++)
				ProcSet(is);
		}
	}
}


bool
DCini::ProcIniSect(char *sectname)
{
	secinfo *isec = NULL;
	bool retval = false;
	IsIni = true;

	for (isec = IniSects; isec->secname != NULL; isec++) {
		if (!_stricmp(sectname, isec->secname)) {
			if (ProcOneIniSect(isec))
				retval = true;
		}
	}
	return retval;
}


bool
DCini::ProcIniGroup(char *groupname)
{
	secinfo *isec = NULL;
	bool retval = false;
	IsIni = true;

	for (isec = IniSects; isec->secname != NULL; isec++) {
		if (isec->secgroup
		 && !_stricmp(groupname, isec->secgroup)) {
			if (ProcOneIniSect(isec))
				retval = true;
		}
	}
	return retval;
}


bool
DCini::ProcIniSectGroup(char *sectname, char *groupname)
{
	secinfo *isec = NULL;
	bool retval = false;
	IsIni = true;

	for (isec = IniSects; isec->secname != NULL; isec++) {
		if (isec->secgroup
		 && !_stricmp(groupname, isec->secgroup)
		 && !_stricmp(sectname, isec->secname)) {
			if (ProcOneIniSect(isec))
				retval = true;
		}
	}
	return retval;
}


bool
DCini::ProcIniSet(char *setname, char *sectname)
{
	setinfo *is = NULL;
	IsIni = true;

	if (sectname
	 && !SetSect(sectname))
		return false;

	if (!CurrSect)
		return false;

	if (CurrSect->sectype == set) // only search for set names
		for (is = CurrSect->setarr; is->sname != NULL; is++)
			if (!_stricmp(setname, is->sname))
				return ProcSet(is);

	return ProcUnknownSet(setname);
}



// protected ini functions

bool
DCini::ProcOneIniSect(secinfo *isec)
{
	setinfo *is = NULL;
	bool retval = false;

	if (IniFile->Section(isec->secname))
		CurrSect = isec;
	else return false;  // not in .ini

	switch (isec->sectype) {

		case set:     // proc global set names
			for (is = isec->setarr; is->sname != NULL; is++)
				ProcSet(is);
			retval = true;
			break;

		case pstyle:  // proc para settings
			if (Curr->stype == 1) {
				if (!IniFile->Find(Curr->stag))
					break;
				retval = ProcOneIniVarSect(isec);
			}
			//	goto styles;
			break;
		case cstyle:  // proc char settings
			if (Curr->stype == 2)
				goto styles;
			break;
		case pcstyle:  // proc all style settings
		styles:
			if (!IniFile->Find(Curr->stag))
				break;
			retval = ProcOneIniVarSect(isec);
			break;

		case tabl:      // proc specific table settings
			if (Tbl == NULL)
				break;
			if (!IniFile->FindAny(TblIni))
				break;
			retval = ProcOneIniVarSect(isec);
			break;

		case graphic:     // proc specific graphic settings
			if (!Gr || (Gr->cnt == 0))
				break;
			if (!IniFile->FindAny(Gr))
				break;
			retval = ProcOneIniVarSect(isec);
			break;

		case xrstyle:   // proc specific xref settings
			if (!Xrnmp)
				break;
			if (!IniFile->Find(*Xrnmp))
				break;
			retval = ProcOneIniVarSect(isec);
			break;

		default:
			break;
	}
	return retval;
}


bool
DCini::ProcOneIniVarSect(secinfo *isec)
{
	setinfo *is = NULL;
	bool retval = false;
	char *lname = NULL;
	size_t len = 0;
	size_t nlen = 0;
	char *cptr = NULL;
	bool setnum = false;

	if (isec->setarr) { // list of properties
		if (IniFile->ListRight()) {  // get list from .ini
			while ((lname = IniFile->ListItem()) != NULL) {  // get items
				len = strlen(lname);
				nlen = 0;
				for (cptr = (lname + (len - 1)) ; cptr > lname; cptr--) {
					if (!isdigit(*cptr))  // point at last non-digit
						break;
					nlen++;   // count of digits, exclude from match
				}
				for (is = isec->setarr; is->sname != NULL; is++) {
					if (!_strnicmp(is->sname, lname, len - nlen)) {
						if (nlen && !is->cpval) { // point at number on end
							setnum = true;
							is->cpval = cptr + 1;
						}
						else setnum = false;
						ProcStyleSet(is);
						if (setnum)
							is->cpval = NULL;
						retval = true;
						break;
					}
				}
			}
		}
	}
	else if (isec->svar) { // single prop for style
		switch (isec->stype) {
			case yesno:
				*((bool *) (isec->svar)) = IniFile->BoolRight();
				retval = true;
				break;
			case cap:
				*((unc *) (isec->svar)) = (unc) IniFile->UCRight();
				retval = true;
				break;
			case snum:
				*((short *) (isec->svar)) = (short) IniFile->IntRight();
				retval = true;
				break;
			case usnum:
				*((uns *) (isec->svar)) = (uns) IniFile->IntRight();
				retval = true;
				break;
			case lnum:
				*((long *) (isec->svar)) = IniFile->IntRight();
				retval = true;
				break;
			case str:
				*((char **) (isec->svar)) = IniFile->StrRight();
				retval = true;
				break;
			case path:
				*((char **) (isec->svar)) = IniFile->PathRight();
				retval = true;
				break;
			case sarr:
				*((char ***) (isec->svar)) = IniFile->StrArrRight();
				retval = true;
				break;
			case larr:
				*((long **) (isec->svar)) = IniFile->IntArrRight();
				break;
			default:
				break;
		}
		if (isec->sfunc)
			(*(isec->sfunc))(IniFile->StrRight());
	}
	return retval;
}


bool
DCini::ProcSet(setinfo *is)
{
	// get value from .ini
	if (!CurrSect
	 || !IniFile->Find(is->sname))
		return false;

	char *txt = NULL;
	uns val = 0;

	switch (is->stype) {
		case yesno:
			*((bool *) (is->svar)) = IniFile->BoolRight();
			break;

		case chr:
			*((char *) (is->svar)) = IniFile->CharRight();
			break;

		case cap:
			*((unc *) (is->svar)) = (unc) IniFile->UCRight();
			break;

		case str:
			*((char **) (is->svar)) = IniFile->StrRight();
			break;

		case path:
			*((char **) (is->svar)) = IniFile->PathRight();
			break;

		case sarr:
			*((char ***) (is->svar)) = IniFile->StrArrRight();
			break;

		case mac:  // process as string for now
			*((char **) (is->svar)) = IniFile->StrRight();
			break;

		case cmp:
			txt = IniFile->StrRight();
			val = 0;
			if (txt && is->match) {
				for (uns i = 0; is->match[i] != NULL; i++) {
					if (!_stricmp(txt, is->match[i])) {
						val = i + 1;
						break;
					}
				}
			}
			*((uns *) (is->svar)) = val;
			break;

		case snum:
			*((short *) (is->svar)) = (short) IniFile->IntRight();
			break;

		case usnum:
			*((uns *) (is->svar)) = (uns) IniFile->IntRight();
			break;

		case lnum:
			*((long *) (is->svar)) = IniFile->IntRight();
			break;

		case larr:
			*((long **) (is->svar)) = IniFile->IntArrRight();
			break;

		case ulnum:
			*((unl *) (is->svar)) = (unl) IniFile->IntRight();
			break;

		case ucnum:
			*((unc *) (is->svar)) = (unc) IniFile->IntRight();
			break;

		case hexnum:
			*((unl *) (is->svar)) = (unl) IniFile->HexRight();
			break;

		case ilist:  // use str for now, later maybe DCnlist
			*((char **) (is->svar)) = IniFile->StrRight();
			break;

		case spec:
			break;

		default:
			return false;
	}

	if (is->sfunc)
		(*(is->sfunc))(IniFile->StrRight());

	return true;
}


// Marker processing

bool
DCini::ProcMarkSect(char *sectname, char *setname, char *setval)
{
	secinfo *isec = NULL;
	bool retval = false;
	IsIni = false;
	Persistent = setname ? true : false;

	for (isec = IniSects; isec->secname != NULL; isec++) {
		if (!_stricmp(sectname, isec->secname)) {
			if (ProcOneMarkSect(isec, setname, setval))
				retval = true;
		}
	}
	return retval;
}


// protected marker processing functions

bool
DCini::ProcOneMarkSect(secinfo *isec, char *setname, char *setval)
{
	setinfo *is = NULL;
	size_t len = 0;
	size_t nlen = 0;
	char *cptr = NULL;
	bool setnum = false;
	bool retval = false;

	switch (isec->sectype) {
		case set:  //globals
			if (!setname || !setval)
				return false;
			for (is = isec->setarr; is->sname != NULL; is++) {
				if (!_stricmp(setname, is->sname))
					return ProcSetVal(isec, is, setval);
			}
			return ProcUnknownSet(setname, setval);
			break;

		case cstyle:   // proc char-only settings only in char span
			if (Curr->stype == 2)
				goto styles;
			break;
		case pstyle:   // proc para settings anytime
		case pcstyle:  // proc all style settings
		styles:
			if (!setval) // only setval used (which may be list of props)
				break;     // no setname provided (in .ini, it is the stylename)
			retval = ProcOneMarkVarSect(isec, setval);
			break;

		case tabl:
			if (!setval)
				break;
			if (Tbl == NULL)
				break;
			retval = ProcOneMarkVarSect(isec, setval);
			break;

		case graphic:     // proc specific graphic settings
			if (!setval)
				break;
			retval = ProcOneMarkVarSect(isec, setval);
			break;

		case xrstyle:     // proc specific xref settings
			if (!setval)
				break;
			retval = ProcOneMarkVarSect(isec, setval);
			break;

		default:
			break;
	}
	return retval;
}


bool
DCini::ProcOneMarkVarSect(secinfo *isec, char *setval)
{
	setinfo *is = NULL;
	size_t len = 0;
	size_t nlen = 0;
	char *cptr = NULL;
	bool setnum = false;
	bool retval = false;

	if (isec->setarr) {  // items are available props
		while (*setval) {  // get items
			len = nlen = 0;
			for (cptr = setval; *cptr; cptr++) {
				if ((*cptr == ',') || isspace(*cptr)) {
					*cptr = '\0';
					if (len == 0)
						len = cptr - setval;
					else
						nlen = (cptr - setval) - len;
					while (*cptr && ((*cptr == ',') || isspace(*cptr)))
						cptr++;
					break;
				}
				else if (isdigit(*cptr)) {
					if (len == 0)
						len = cptr - setval;
				}
			}
			if (len == 0)
				len = cptr - setval;
			for (is = isec->setarr; is->sname != NULL; is++) {
				if (!_strnicmp(is->sname, setval, len)) {
					if (nlen && !is->cpval) { // point at number on end
						setnum = true;
						is->cpval = setval + len;
					}
					else setnum = false;
					ProcStyleSet(is);
					if (setnum)
						is->cpval = NULL;
					retval = true;
					break;
				}
			}
			setval = cptr;
		}
	}
	else if (isec->svar) {  // set single item
		switch (isec->stype) {
			case yesno:
				*((bool *) (isec->svar)) = MakeBool(setval);
				retval = true;
				break;
			case cap:
				*((unc *) (isec->svar)) = toupper(*setval);
				retval = true;
				break;
			case snum:
				*((short *) (isec->svar)) = (short) atoi(setval);
				retval = true;
				break;
			case usnum:
				*((uns *) (isec->svar)) = (uns) atoi(setval);
				retval = true;
				break;
			case str:
			case path:
				*((char **) (isec->svar)) = setval;
				retval = true;
				break;
			case sarr:
				*((char ***) (isec->svar)) = GetStrArr(setval);
				retval = true;
				break;
			case larr:
				*((long **) (isec->svar)) = GetIntArr(setval);
				retval = true;
				break;
			default:
				break;
		}
		if (isec->sfunc)
			(*(isec->sfunc))(setval);
	}

	return retval;
}


bool
DCini::ProcSetVal(secinfo *isec, setinfo *is, char *setval)
{
	IsIni = false;

	// marker, no need to get value from .ini
	switch (isec->sectype) { 
		case set:  // set global per value
			if (!ProcVal(is->stype, is->svar, setval))
				return false;  // unhandled type
			break;

		case pcstyle:  // var is Curr member address
			if (!ProcStyleSet(is))
				return false;  // unhandled type
			break;

		default:
			return false;
	}

	if (is->sfunc)
		(*(is->sfunc))(setval);

	return true;
}


bool
DCini::ProcVal(styp typ, void *var, char *val)
{
	switch (typ) {
		case yesno:
			*((bool *) var) = MakeBool(val);
			break;

		case chr:
			*((char *) var) = *val;
			break;

		case cap:
			*((unc *) var) = toupper(*val);
			break;

		case str:
		case path:
			*((char **) var) = NewName(val);
			break;

		case sarr:
			*((char ***) var) = GetStrArr(val);
			break;

		case mac:  // process as string for now
			*((char **) var) = NewName(val);
			break;

		case snum:
			*((short *) var) = (short) atoi(val);
			break;

		case usnum:
			*((uns *) var) = (uns) atoi(val);
			break;

		case lnum:
			*((long *) var) = atol(val);
			break;

		case larr:
			*((long **) var) = GetIntArr(val);
			break;

		case ulnum:
			*((unl *) var) = (unl) atol(val);
			break;

		case ucnum:
			*((unc *) var) = (unc) atoi(val);
			break;

		case hexnum:
			*((unl *) var) = strtoul(val, NULL, 16);
			break;

		case ilist:  // use str for now, later maybe DCnlist
			*((char **) var) = NewName(val);
			break;

		case spec:
			break;

		default:
			return false;
	}

	return true;
}

#if 0
bool
DCini::ProcStyleVal(setinfo *is)
{
	uns usval = 0;
	unc ucval = 0;

	// get value from setinfo sval or cpval
	switch (is->stype) {
		case usnum:
			usval = (uns) atoi(is->cpval);
			if (usval > (uns) (is->sval))
				*((uns *) (is->svar)) = (uns) (is->sval);
			else
				*((uns *) (is->svar)) = usval;
			break;

		case ucnum:
			ucval = (unc) atoi(is->cpval);
			if (ucval > (unc) (is->sval))
				*((unc *) (is->svar)) = (unc) (is->sval);
			else
				*((unc *) (is->svar)) = ucval;
			break;

		case val:
			*((unc *) (is->svar)) = (unc) (is->sval);
			break;

		case bval:
			*((bool *) (is->svar)) = (bool) (is->sval);
			break;

		case cval:
			*((char **) (is->svar)) = is->cpval;
			break;

		case bit:  // OR in the value
			*((unc *) (is->svar)) |= (unc) (is->sval);
			break;

		case nbit:  // remove the value
			*((unc *) (is->svar)) &= ~((unc) (is->sval));
			break;

		case spec:
			break;

		default:
			return false;
	}

	if (is->sfunc)
		(*(is->sfunc))(is->cpval);

	return true;
}
#endif


bool
DCini::MakeBool(char *setval)
{
	if (!_stricmp(setval, "yes")
	 || !_stricmp(setval, "true")
	 || (*setval == '1'))
		return true;

	return false;
}


void
DCini::ProcConfigPara(char *txt, long pref)
{
	if (!txt || !*txt)
		return;

	// process lines ending with hard ret '\0' as either
	// section names, settings, or comments, double null ends it

	setting *s = NULL;
	char *csect = NULL;

	for (size_t len = strlen(txt); *txt; txt += (len + 1)) {
		if (*txt == ';')
			continue;
		if ((s = ParseDirective(txt, true)) == NULL)
			continue;
		if (s->sect) {   // save section name
			csect = (char *) s->sect;
			if (!s->key && !s->val) { // just the section name
				delete s;
				continue;
			}
		}
		else if (csect) // continuation of same section
			s->sect = csect;
		else            // error
			continue;
		// call for each setting found
		ProcDirective(s, pref, true);
	}
}


void
DCini::ProcParaConfig(long pref)
{
	ParaSetList.execu(pref, SetParaConfig);
}

void
DCini::SetParaConfig(void *vp)
{
	if (!vp)
		return;
	setting *s = (setting *) vp;
	DCini::ProcMarkSect((char *) s->sect, (char *) s->key, (char *) s->val);
}


setting *
DCini::ParseDirective(char *txt, bool para)
{
	char *sect = NULL;
	char *key = NULL;
	char *val = NULL;
	char *cpt = NULL;
	bool kquote = false;
	bool vquote = false;
	setting *s = NULL;

	// [section]key=val for globals, or [section]val
	// skip anything before the [
	if (((cpt = strchr(txt, '[')) == NULL)
	 && !para)  // para config may not have sect on line
		return NULL;
	if (cpt) {  // section name started
		txt = cpt + 1;
		sect = txt;
		if ((cpt = strchr(txt, ']')) == NULL)
			return NULL;  // no section name terminator, error
		*cpt = '\0';
		txt = cpt + 1;
		if (!SetSect(sect, false))
			return NULL;  // invalid section name, forget it
	}
	while (*txt && isspace(*txt))  // skip any space after section
		txt++;
	if (!*txt) {
		if (para      // para config may have only sect on line
		 && sect) {
			s = new setting;
			s->sect = sect;
			s->key = NULL;
			s->val = NULL;
			return s;
		}
		return NULL;  // nothing after section, ignore
	}

	// get key and value
	if (*txt == '=') // equal right after section, remove equal
		val = ++txt;   // no key, val only
	else {  // see if key is next
		key = txt;
		if (*key == '"') {
			cpt = ++key;
			while (*cpt) {
				if (*cpt == '"')
					break;
				cpt++;
			}
			if (!*cpt)
				return NULL;
			kquote = true;
			*cpt = '\0';
			txt = cpt + 1; // char after quoted key
		}
		if ((cpt = strchr(txt, '=')) != NULL) {
			*cpt = '\0';
			val = cpt + 1;
		}
		else {  // section without key or equals
			val = key;
			vquote = kquote;
			key = NULL;
		}
	}

	// start of val, remove only one space after equals
	if (!vquote) {
		if (*val && isspace(*val))
			val++;
		if (*val == '"') {
			cpt = val + strlen(val);
			if (*cpt == '"') {
				vquote = true;
				*cpt-- = '\0';
				val++;
			}
		}
	}

	// valid config, make setting struct
	s = new setting;  
	s->sect = sect;
	s->key = key;
	s->val = val;

	return s;
}


// get val from program (for macro)

char *
DCini::GetStrVal(char *sectname, char *setname)
{
	secinfo *isec = NULL;
	char *ret = NULL;
	IsIni = false;

	for (isec = IniSects; isec->secname != NULL; isec++) {
		if (!_stricmp(sectname, isec->secname)) {
			CurrSect = isec;
			if ((ret = GetOneSectStr(isec, setname)) != NULL)
				break;
		}
	}
	return ret;
}

char *
DCini::GetOneSectStr(secinfo *isec, char *setname)
{
	setinfo *is = NULL;
	char *ret = NULL;

	switch (isec->sectype) {
		case set:  //globals
			if (setname) {
				for (is = isec->setarr; is->sname != NULL; is++) {
					if (!_stricmp(setname, is->sname)) {
						ret = GetSetVal(is);
						break;
					}
				}
			}
			break;

		case cstyle:   // proc char-only settings only in char span
			if (Curr->stype == 2)
				goto styles;
			break;
		case pstyle:   // proc para settings anytime
		case pcstyle:  // proc all style settings
		styles:
			ret = GetVarVal(isec, setname);
			break;

		case tabl:
			if (Tbl == NULL)
				break;
		case graphic:     // proc specific graphic settings
		case xrstyle:     // proc specific xref settings
			ret = GetVarVal(isec, setname);
			break;

		default:
			break;
	}
	return ret;
}


char *
DCini::GetSetVal(setinfo *is)
{
	IsIni = false;
	char *ret = NULL;

	if (is->svar == NULL)
		return NULL;

	switch (CurrSect->sectype) { 
		case set:  // set global per value
			ret = GetVal(is->stype, is->svar);
			break;

		case pcstyle:  // var is Curr member address
			ret = GetStyleVal(is);
			break;

		default:
			break;
	}

	return ret;
}


char *
DCini::GetVarVal(secinfo *isec, char *setname)
{
	setinfo *is = NULL;
	size_t len = 0;
	char *ret = NULL;

	if (isec->setarr) {  // items are available props
		len = strlen(setname);
		for (is = isec->setarr; is->sname != NULL; is++) {
			if (!_strnicmp(is->sname, setname, len)) {
				if (is->svar)
					ret = GetStyleVal(is);
				break;
			}
		}
	}
	else if (isec->svar) {  // single item
		switch (isec->stype) {
			case usnum:
				ret = NewName(6);  // room for 5 digits
				_itoa((int) (*((uns *) (isec->svar))), ret, 10);
				break;
			case str:
			case path:
				ret = *((char **) (isec->svar));
				if (ret)
					ret = NewName(*((char **) (isec->svar)));
				else
					ret = "";
				break;
			default:
				break;
		}
	}

	return ret;
}


char *
DCini::GetVal(styp typ, void *var)
{
	char *ret = NULL;

	switch (typ) {
		case yesno:
			ret = *((bool *) var) ? "1" : "0";
			break;

		case chr:
		case cap:
			ret = NewName(2);
			if (*((char *) var))
				ret[0] = *((char *) var);
			break;

		case str:
		case path:
		case mac:  // process as string for now
		case ilist:  // use str for now, later maybe DCnlist
			if (*((char **) var))
				ret = NewName(*((char **) var));
			else
				ret = "";
			break;

		case snum:
			ret = NewName(7); // 5 digits and minus
			_itoa((int) (*((short *) var)), ret, 10);
			break;

		case usnum:
			ret = NewName(6);
			_itoa((int) (*((uns *) var)), ret, 10);
			break;

		case lnum:
			ret = NewName(12);
			_itoa((int) (*((long *) var)), ret, 10);
			break;

		case ulnum:
			ret = NewName(11);
			_itoa((int) (*((unl *) var)), ret, 10);
			break;

		case ucnum:
			ret = NewName(4);
			_itoa((int) (*((unc *) var)), ret, 10);
			break;

		case hexnum:
			ret = NewName("0x", 13);
			_itoa((int) (*((unl *) var)), ret + 2, 16);
			break;

		default:
			break;
	}

	return ret;
}


char *
DCini::GetStyleVal(setinfo *is)
{
	char *ret = NULL;

	switch (is->stype) {
		case usnum:
		case ushval:
			ret = NewName(6);  // room for 5 digits
			_itoa((int) (*((uns *) (is->svar))), ret, 10);
			break;

		case ucnum:
			ret = NewName(4);  // room for 3 digits
			_itoa((int) (*((unc *) (is->svar))), ret, 10);
			break;

		case val:
			ret = NewName(2);
			if (*((unc *) (is->svar)))
				ret[0] = *((unc *) (is->svar));
			break;

		case bval:
			ret = *((bool *) (is->svar)) ? "1" : "0";
			break;

		case cval:
			if (*((char **) (is->svar)))
				ret = NewName(*((char **) (is->svar)));
			else
				ret = "";
			break;

		case bit:  // AND the bit value
			ret = (*((unc *) (is->svar)) & (unc) (is->sval)) ? "1" : "0";
			break;

		case nbit:  // negate the bit value
			ret = (*((unc *) (is->svar)) & (unc) (is->sval)) ? "0" : "1";
			break;

		case lbit:  // AND the bit value
			ret = (*((unl *) (is->svar)) & (unl) (is->sval)) ? "1" : "0";
			break;

		case nlbit:  // negate the bit value
			ret = (*((unl *) (is->svar)) & (unl) (is->sval)) ? "0" : "1";
			break;

		default:
			break;
	}

	return ret;
}



// common utilities for marker, macro, and ini processing


bool
DCini::SetSect(char *sectname, bool ini)
{
	secinfo *isec = NULL;

	if (!sectname)
		return false;

	if (!stricmp(sectname, CurrSect->secname))
		return true;  //already set

	for (isec = IniSects; isec->secname != NULL; isec++)
		if (!_stricmp(sectname, isec->secname)) 
			break;

	if (isec->secname) {
		if (ini) {
			if (IniFile->Section(isec->secname)) {
				CurrSect = isec;
				return true; // section is in IniFile 
			}
		}
		else {
			CurrSect = isec;
			return true;  // section is in IniSects
		}
	}

	return false;  // not found in list
}


bool
DCini::CheckCurrFormName(char *key)
{
	if (!key)  // null key means for current format
		return true;
	if (!strcmp(key, Curr->stag)  // case-sensitive match to tag
	 || (Curr->cstag              // either para tag or char tag
	  && !strcmp(key, Curr->cstag)))
		return true;
	return false;
}


char *DCini::ListHead = NULL;
char *DCini::ListPtr = NULL;

void 
DCini::StartList(char *lptr)
{
	ListHead = ListPtr = lptr;
}

char *
DCini::GetListItem()
{
	size_t len = 0;
	char *cptr = NULL;
	char *rptr = NULL;

	for (cptr = ListPtr; *cptr; cptr++) {
		if ((*cptr == ',') || isspace(*cptr)) {
			len = cptr - ListPtr;
			while (*cptr && ((*cptr == ',') || isspace(*cptr)))
				cptr++;
			break;
		}
	}
	if (len == 0)
		len = cptr - ListPtr;
	rptr = len ? NewName(ListPtr, len) : NULL;
	ListPtr = cptr;

	return rptr;
}


char **
DCini::GetStrArr(char *sptr)
{
	StartList(sptr);

	char *ptr = NULL;
	long count = 0;
	while ((ptr = GetListItem()) != NULL) {
		count++;
		DeleteName(ptr);
	}

	char **saptr = new char *[count + 1];
	long i = 0;
	StartList(sptr);
	while ((ptr = GetListItem()) != NULL)
		saptr[i++] = ptr;
	saptr[i] = NULL;
	return saptr;
}

long *
DCini::GetIntArr(char *sptr)
{
	StartList(sptr);

	char *ptr = NULL;
	long count = 0;
	while ((ptr = GetListItem()) != NULL) {
		count++;
		DeleteName(ptr);
	}

	long *laptr = new long[count + 1];
	long i = 0;
	StartList(sptr);
	while ((ptr = GetListItem()) != NULL)
		laptr[i++] = atol(ptr);
	laptr[i] = 0;
	return laptr;
}



bool
DCini::ProcStyleSet(setinfo *is)
{
	uns usval = 0;
	unc ucval = 0;

	// get value from setinfo sval or cpval
	switch (is->stype) {
		case usnum:
			usval = (uns) atoi(is->cpval);
			if (usval > (uns) (is->sval))
				*((uns *) (is->svar)) = (uns) (is->sval);
			else
				*((uns *) (is->svar)) = usval;
			break;

		case ucnum:
			ucval = (unc) atoi(is->cpval);
			if (ucval > (unc) (is->sval))
				*((unc *) (is->svar)) = (unc) (is->sval);
			else
				*((unc *) (is->svar)) = ucval;
			break;

		case val:
			*((unc *) (is->svar)) = (unc) (is->sval);
			break;

		case bval:
			*((bool *) (is->svar)) = (bool) (is->sval);
			break;

		case cval:
			*((char **) (is->svar)) = is->cpval;
			break;

		case ushval:
			*((uns *) (is->svar)) = (uns) (is->sval);
			break;

		case bit:  // OR in the value
			*((unc *) (is->svar)) |= (unc) (is->sval);
			break;

		case nbit:  // remove the value
			*((unc *) (is->svar)) &= ~((unc) (is->sval));
			break;

		case lbit:  // OR in the value
			*((unl *) (is->svar)) |= (unl) (is->sval);
			break;

		case nlbit:  // remove the value
			*((unl *) (is->svar)) &= ~((unl) (is->sval));
			break;

		case spec:
			break;

		default:
			return false;
	}

	if (is->sfunc)
		(*(is->sfunc))(is->cpval);

	return true;
}


bool
DCini::ProcUnknownSet(char *setname, char *setval)
{
	if (!CurrSect)
		return false;

	// no listing in CurrSect list, may just be in .ini
	// but then, what do we do with it?

	return false;
}


// process for pending settings

DCvlist DCini::SetList;
DCvlist DCini::ParaSetList;
DClist DCini::CurrPendList;
DClist DCini::TblSetList;
DClist DCini::GraphSetList;
DClist DCini::XrefSetList;
char *DCini::PendingGraphGroup = NULL;
char *DCini::PendingTblGroup = NULL;


bool
DCini::SetDirective(char *txt, long cpid)
{
	setting *s = NULL;

	if ((s = ParseDirective(txt)) == NULL)
		return false;

	return ProcDirective(s, cpid);
}


bool
DCini::ProcDirective(setting *s, long cpid, bool para)
{
	if (CurrSect->sectype == set) {
		// trouble with setting in scan is that it's still there
		// at the start of write phase, not what you want
		//ProcMarkSect(sect, key, val);  // global setting, do now
		if (para)
			ParaSetList.add(s, cpid);
		else
			SetList.add(s, cpid);
		//	delete s;
		return true;  // persistent, so do again in write phase
	}

	// do special cases first
	if (!_stricmp(s->sect, "TableGroup")) {
		PendingTblGroup = (char *) s->val;  // set in scan so that can get .ini defaults
		delete s;
		return false;        //  at the start of write phase for table
	}
	if (!_stricmp(s->sect, "GraphGroup")) {  // what if in text frame in graphic? ****
		PendingGraphGroup = (char *) s->val; // set in scan so that can get .ini defaults
		delete s;
		return false;         //  at the start of write phase for graphic
	}

	// this is scan phase, save object setting for write phase
	if (CurrSect->sectype == tabl) {  // table setting
		TblSetList.add((long) s);
		return false;
	}
	if (CurrSect->sectype == graphic) {  // graphic setting
		GraphSetList.add((long) s);  // retrieve at next graphic
		return false;
	}
	if (CurrSect->sectype == xrstyle) {  // xref setting
		XrefSetList.add((long) s);  // retrieve at next xref
		return false;
	}

	if ((CurrSect->sectype == pstyle)    // para setting
	 && !stricmp(s->sect, "HelpContentsLevels"))
		*((uns *) (CurrSect->svar)) = (uns) atoi(s->val); // set immediately

	// whatever is left is for Curr
	if (para)
		CurrPendList.add((long) s);
	else // marker
		SetList.add(s, cpid);

	return true;
}


void
DCini::SetCurrID(long i)
{
	if (!i)
		return;

	setting *s = (setting *) i;

	if (!s->key)
		s->key = *Fmnmp;
}


void
DCini::GetCurrSettings(long i)
{
	if (!i)
		return;

	setting *s = (setting *) i;

	if (CheckCurrFormName((char *) s->key))
		ProcMarkSect((char *) s->sect, (char *) s->key, (char *) s->val);
}


void
DCini::SetXrefID(long i)
{
	if (!i)
		return;

	setting *s = (setting *) i;

	if (!s->key)
		s->key = XrefIni->nm[0];
}


void
DCini::GetXrefSettings(long i)
{
	if (!i)
		return;

	setting *s = (setting *) i;

	if (!s->sect) // marked deleted
		return;

	short mk = -1;

	if ((mk = MatchKeys(XrefIni, (char *) s->key)) > -1) {
		ProcMarkSect((char *) s->sect, (char *) s->key, (char *) s->val);
		if (mk == 0)  // no key or matched specific ID, done
			s->sect = NULL;
	}
}


void
DCini::SetTblID(long i)
{
	if (!i)
		return;

	setting *s = (setting *) i;

	if (!s->key)
		s->key = NewName(TblIni->nm[0]);
}


void
DCini::GetTblSettings(long i)
{
	if (!i)
		return;

	setting *s = (setting *) i;

	if (!s->sect) // marked deleted
		return;

	short mk = -1;

	if ((mk = MatchKeys(TblIni, (char *) s->key)) > -1) {
		ProcMarkSect((char *) s->sect, (char *) s->key, (char *) s->val);
		if (mk == 0)  // no key or matched specific ID, done
			s->sect = NULL;
	}
}


void
DCini::SetGraphID(long i)
{
	if (!i)
		return;

	setting *s = (setting *) i;

	if (!s->key)
		s->key = Gr->nm[0];
}


void
DCini::GetGraphSettings(long i)
{
	if (!i)
		return;

	setting *s = (setting *) i;

	if (!s->sect) // marked deleted
		return;

	short mk = -1;

	if ((mk = MatchKeys(Gr, (char *) s->key)) > -1) {
		ProcMarkSect((char *) s->sect, (char *) s->key, (char *) s->val);
		if (mk == 0)  // no key or matched specific ID, done
			s->sect = NULL;
	}
}


short
DCini::MatchKeys(anyini *ks, char *key)
{
	if (!key)
		return 0;

	for (uns i = 0; i < ks->cnt; i++) {
		if (!strcmp(key, ks->nm[i]))
			return (short) i;
	}

	return -1;
}


void
DCini::GetPropSettings(long i)
{
	if (!i)
		return;

	setting *s = (setting *) i;

	ProcMarkSect((char *) s->sect, (char *) s->key, (char *) s->val);
}



// end of dcini.cpp

