// mxlparser.cpp is the DLL for parsing MicroXML files.
// Created May 26, 2013, by Jeremy H. Griffith
// Copyright (c) 2013 Jeremy H. Griffith.  All rights reserved.


#include "stdafx.h"

//  Defines the entry point for the DLL application.
BOOL APIENTRY DllMain( HANDLE hModule, 
                       DWORD  ul_reason_for_call, 
                       LPVOID lpReserved )
{
    switch (ul_reason_for_call)	{
			case DLL_PROCESS_ATTACH:
			case DLL_THREAD_ATTACH:
			case DLL_THREAD_DETACH:
			case DLL_PROCESS_DETACH:
				break;
    }
    return TRUE;
}


// public member functions


element *MxlParser::ParseFile(const char *filename, char *lang)
{
	if (filename) {
		fname = (char *) filename;
		ifile = fopen(fname, "r");
		if (!ifile)
			return NULL;
	}
	else
		ifile = stdin;

	unc *cp = txt8buf;
	unc *err = cp;
	char *str = NULL;
	unc *end = NULL;
	unl u32 = 0;
	long cnt = 0;
	EndDoc = false;
	InDoc = false;
	EndFile = false;
	Line = 0;

	while (!EndFile) {
		*txt8buf = '\0';
		if (!fgets((char *) txt8buf, MAX_CHAR_COUNT, ifile)) {
			if (ferror(ifile))
				ReportError(Line, "Error reading from ", fname ? fname : "stdin", true);
			EndFile = true;
		}
		err = cp = txt8buf;
		Line++;

		// proc items continued from previous line
		if (InComment) {
			if ((end = (unc *) strstr((char *) cp, "-->")) != NULL) {
				// ends in this line
				if (((str = strstr((char *) cp, "--")) != NULL)
				 && (str != (char *) end))
					ReportError(Line, "Invalid comment content: \"--\"", NULL, false);
				str = MakeStr(err, end);
				if (UseSAX
				 && FullXML)
					ReportComment(str);
				delete [] str;
				InComment = false;
				cp = (unc *) end + 3;
			}
			else {  // includes entire line
				if ((str = strstr((char *) cp, "--")) != NULL)
					ReportError(Line, "Invalid comment content: \"--\"", NULL, false);
				if (UseSAX
				 && FullXML)
					ReportComment(str);
				continue;
			}
		}
		else if (InDoctype) {
			if ((end = (unc *) strchr((char *) cp, '>')) != NULL) {
				InDoctype = false;  // ends in this line
				str = MakeStr(err, end);
				ReportXMLDoctype((unc *) str);
				delete [] str;
				cp = (unc *) end + 3;
			}
			else {  // includes entire line
				ReportXMLDoctype((unc *) cp);
				continue;
			}
		}
		else if (InCDATA) {
			if ((end = (unc *) strstr((char *) cp, "]]>")) != NULL) {
				InCDATA = false;  // ends in this line
				str = MakeStr(err, end);
				ReportXMLCData((unc *) str);
				delete [] str;
				cp = (unc *) end + 3;
			}
			else {  // includes entire line
				ReportXMLCData(cp);
				continue;
			}
		}
		else if (InPI) {  // continue processing PI content
			if ((end = (unc *) strstr((char *) cp, "?>")) != NULL) {
				InPI = false;  // ends in this line
				str = MakeStr(err, end);
				ReportXMLPI((unc *) str);
				delete [] str;
				cp = (unc *) end + 3;
			}
			else {  // includes entire line
				ReportXMLPI((unc *) str);
				continue;
			}
		}
		else if (InStartTag) {  // continue processing attributes, end
			if ((end = (unc *) strchr((char *) cp, '>')) != NULL) {
				InStartTag = false;  // ends in this line
				if (*(end - 1) == '/') // empty element
					IsEmptyElement = true;
				GetAttributes(&cp, end);
				EndStartTag();
			}
			else {
				GetAttributes(&cp);
				continue;
			}
		}
		while (*cp) {
			switch (*cp) {
				case '<':  // start of tag or comment
					++cp;
					if (*cp == '!') { // comment
						if (!strncmp((char *) cp + 1, "DOCTYPE", 7)) {
							// report as a comment, invalid in mxl
							err = cp + 1;
							if ((end = (unc *) strchr((char *) cp, '>')) != NULL) {
								// ends in same line
								str = MakeStr(err, end);
								ReportXMLDoctype((unc *) str);
								delete [] str;
								cp = end + 3;
							}
							else {  // more than one line
								InDoctype = true;
								ReportXMLDoctype(cp);
							}
						}
						else if (!strncmp((char *) cp + 1, "[CDATA[", 7)) {
							// report as a comment, invalid in mxl
							err = cp + 1;
							if ((end = (unc *) strstr((char *) cp, "]]>")) != NULL) {
								// ends in same line
								str = MakeStr(err, end);
								ReportXMLCData((unc *) str);
								delete [] str;
								cp = end + 3;
							}
							else {  // more than one line
								InCDATA = true;
								ReportXMLCData(cp);
							}
						}
						else if ((*++cp != '-')
						 || (*++cp != '-')) {
							str = MakeStr(cp, end);
							ReportError(Line, "Invalid comment start: ", str, false);
							delete [] str;
						}
						else { // valid comment start
							err = cp + 1;
							if ((end = (unc *) strstr((char *) cp, "-->")) != NULL) {
								// ends in same line
								if (((str = strstr((char *) cp, "--")) != NULL)
								 && (str != (char *) end))
									ReportError(Line, "Invalid comment content: \"--\"", NULL, false);
								str = MakeStr(err, end);
								if (UseSAX
								 && FullXML)
									ReportComment(str);
								delete [] str;
								cp = end + 3;
							}
							else {  // more than one line
								if ((str = strstr((char *) cp, "--")) != NULL)
									ReportError(Line, "Invalid comment content: \"--\"", NULL, false);
								InComment = true;
								if (UseSAX
								 && FullXML)
									ReportComment((char *) cp);
							}
						}
					}
					else if (*cp == '?') { // start of PI, invalid in mxl
						err = cp + 1;
						StartPI = true;
						if ((end = (unc *) strstr((char *) cp, "?>")) != NULL) {
							// ends in same line
							str = MakeStr(err, end);
							ReportXMLPI((unc *) str);
							delete [] str;
							cp = end + 3;
						}
						else {  // more than one line
							InPI = true;
							ReportXMLPI((unc *) str);
						}
						StartPI = false;
					}
					else if (*cp == '/') { // start of end tag, match to start
						if (cnt) {
							AddTextContent(cnt);
							cnt = 0;
						}
						if (EndDoc) {
							ReportError(Line, "Invalid MicroXML: end tag after root end", NULL, false);
						}
						else {
							++cp;
							GetEndTag(&cp);
						}
					}
					else {  // start of start tag
						if (cnt) {
							AddTextContent(cnt);
							cnt = 0;
						}
						if (EndDoc) {
							ReportError(Line, "Invalid MicroXML: start tag after root end", NULL, false);
						}
						else {
							IsEmptyElement = false;
							u32 = U8to32Chr(&cp);
							if (IsValidUNameStart(u32))
								GetStartTag(u32, &cp);
							if (!InStartTag)  // ends in this line
								EndStartTag();
						}
					}
					break;

				// text content from here down

				case '>':  // stray end in text, allowed in XML
					if (InEndTag) // left over from previous line
						InEndTag = false;
					else
						ReportError(Line, "Invalid MicroXML char. \">\"", NULL, false);
					cp++;
					break;

				case '&':  // entity
					u32 = GetCharReference(&cp);
					txt32buf[cnt++] = u32;
					break;

				case '\r': // return, discard
					cp++;
					if (*cp == 0x0A)  // discard if before \n
						cp++;
					// either way, process it as \n or process the \n
					txt32buf[cnt++] = 0x0A;
					break;

				case '\n': // newline
					txt32buf[cnt++] = 0x0A;
					cp++;
					break;

				case '\t': // tab
					txt32buf[cnt++] = 0x09;
					cp++;
					break;

				case ' ': // space
					txt32buf[cnt++] = 0x20;
					cp++;
					break;

				default:
					u32 = U8to32Chr(&cp);
					if (IsValidUChar(u32))
						txt32buf[cnt++] = u32;
					break;
			}
			if (InDoctype
			 || InPI
			 || InComment
			 || InStartTag)
				break;
		}
	}
	if (cnt)
		AddTextContent(cnt);

	fclose(ifile);
	return UseModel ? rootelem : NULL;
}


// protected member functions

void MxlParser::GetStartTag(unl u32, unc **cpt)
{
	long cnt = 0;
	txt32buf[cnt++] = u32;

	IsEmptyElement = false;
	unc *end = (unc *) strchr((char *) *cpt, '>');
	unc *spc = (unc *) strchr((char *) *cpt, ' ');
	if (!spc) {
		if (!end) {
			unc *cr = *cpt + strlen((char *) *cpt) - 1;
			if (*cr == '\n') {
				if (*(cr - 1) == '\r')
					spc = cr - 1;
				else
					spc = cr;
			}
			else if (*cr == '\t')
				spc = cr;
		}
	}
	if (end
	 && (*(end - 1) == '/')) {
		IsEmptyElement = true;
		end--;
	}

	while (**cpt) {
		if ((*cpt == end)
		 || (*cpt == spc))
			break;
		if (**cpt == ':') {
			ReportError(Line, "Invalid element name char: \":\"", NULL, false);
			(*cpt)++;
			continue;
		}
		u32 = U8to32Chr(cpt);
		if (IsValidUName(u32))
			txt32buf[cnt++] = u32;
	}
	if (cnt) {  // have valid start tag name now in txt32buf
		txt32buf[cnt] = 0;
		currelem = new element;
		currelem->name = NULL;
		currelem->attrs = NULL;
		currelem->content = NULL;
		currelem->namelen = currelem->attrcnt = currelem->contcnt = 0;
		currelem->parent = NULL;

		attrcnt = 0;
		if (!elemlevel) { // at root
			rootelem = currelem;
			InDoc =  true;
		}
		else {  // add to content list of parent
			element *parent = elevels[elemlevel];
			currelem->parent = parent;
			cont *newcont = new cont;
			newcont->cnt = 0;
			newcont->it = currelem;
			cont **oldcont = parent->content;
			parent->content = new cont*[parent->contcnt + 1];
			if (oldcont)
				memcpy(parent->content, oldcont, parent->contcnt * sizeof(cont *));
			parent->content[parent->contcnt++] = newcont;
		}
		elevels[++elemlevel] = currelem;

		currelem->name = new unl[cnt + 1];
		memcpy(currelem->name, txt32buf, (cnt + 1) * sizeof(unl));
		currelem->namelen = cnt;
	}

	if (spc) { // may have attributes
		if (end) {
			if (spc < (end - 2))
				GetAttributes(cpt, end);
		}
		else
			GetAttributes(cpt);
	}

	if (!end) {
		InStartTag = true;
		SkipSpace(cpt);
	}
	else if (IsEmptyElement)
		*cpt = end + 2;
	else
		*cpt = end + 1;
}


void MxlParser::EndStartTag(void)
{
	AddAttributes();
	if (UseSAX)
		SetStart(currelem->name, currelem->namelen,
						 currelem->attrs, currelem->attrcnt);
	if (!UseModel)
		DeleteAttributes();
	if (IsEmptyElement) {
		if (UseSAX)
			SetEnd(currelem->name, currelem->namelen);
		if (!UseModel) {
			delete [] (currelem->name);
			delete currelem;
		}
		IsEmptyElement = false;
		currelem = elevels[--elemlevel];
		if (elemlevel < 1) {
			InDoc = false;
			EndDoc = true;
		}
	}
}


void MxlParser::GetAttributes(unc **cpt, unc *end)
{
	long u32 = 0;

	if (!end)
		end = *cpt + strlen((char *) *cpt);

	while (*cpt < end) {
		if ((**cpt == '=')
		 && !AttrVal) {
			if (AttrNameCnt)
				SetAttrName();
			else  // otherwise error
				ReportError(Line, "Found \"=\" not after an attribute name", NULL, false);
			// if not first attr, amy be previous attr missing a quote
			(*cpt)++;
			SkipSpace(cpt);
		}
		else if (AttrName) {
			if (!AttrNameCnt) {   // name not started
				SkipSpace(cpt);
				if ((*cpt == end)
				 || ((**cpt == '/')
				  && (*cpt == (end - 1))))
					break;           // at end of start tag
			}
			if (**cpt == ' ') {  // delimits name 
				if (AttrNameCnt)
					SetAttrName();
				SkipSpace(cpt);
				// if not = next, error
				if (**cpt != '=')
					ReportError(Line, "Missing \"=\" after attribute name", NULL, false);
			}
			else if (**cpt == ':') {
				ReportError(Line, "Invalid attribute name char: \":\"", NULL, false);
				(*cpt)++;
			}
			else {
				u32 = U8to32Chr(cpt);
				if (AttrNameCnt ? IsValidUName(u32) : IsValidUNameStart(u32))
					txt32buf[AttrNameCnt++] = u32;
			}
		}
		else if (AttrVal) {  // in value
			if (!AttrValCnt    // at start
			 && !AttrQuote) {  // starting quote
				SkipSpace(cpt);
				AttrQuote = **cpt;
				// make sure it is " or ', else error
				if ((AttrQuote != '"')
				 && (AttrQuote != '\'')) {
					ReportError(Line, "Invalid attribute quote char.", NULL, false);
					AttrQuote = '"';  // try to fix it
				}
				else
					(*cpt)++;
			}
			else if (**cpt == AttrQuote) { // ending quote
				SetAttrVal();
				(*cpt)++;
				SkipSpace(cpt);
			}
			else if (**cpt == '&') {  // char ref
				u32 = GetCharReference(cpt);
				txt32buf[AttrValCnt++] = u32;
			}
			else if (**cpt == '>') {  // may be end of tag if missing quote
				ReportError(Line, "Invalid attribute value char: ", "\">\"", false);
				if (((*(*cpt - 1)) == ' ')
				 || ((*(*cpt - 1)) == '/')) { // treat as end of tag
					ReportError(Line, "Unexpected start tag ending", NULL, false);
					if (AttrValCnt)
						AttrValCnt--;
					SetAttrVal();
					break;
				}					
			}
			else { // attr value text
				if ((**cpt == '<')
				 || (**cpt == '&')) {
					char buf[2];
					buf[0] = **cpt;
					buf[1] = '\0';
					ReportError(Line, "Invalid attribute value char: ", buf, false);
					(*cpt)++;
					continue;
				}
				u32 = U8to32Chr(cpt);
				if ((u32 == 0x0A)
				 || IsValidUChar(u32))
					txt32buf[AttrValCnt++] = u32;
			}
		}
	}

	// if tag ended, make sure attrs did too
	if (**cpt) {
		SkipSpace(cpt);
		if (**cpt == '/')
			(*cpt)++;
		if (**cpt == '>')
			(*cpt)++;
	}
}


void MxlParser::SkipSpace(unc **cpt)
{
	while ((**cpt == ' ')
	 || (**cpt == '\t')
	 || (**cpt == '\n')
	 || (**cpt == '\r'))
		(*cpt)++;
}


void MxlParser::SetAttrName(void)
{
	CurrAttr = new pair;
	CurrAttr->name = new unl[AttrNameCnt + 1];
	memcpy(CurrAttr->name, txt32buf, AttrNameCnt * sizeof(unl));
	CurrAttr->namelen = AttrNameCnt;
	CurrAttr->name[AttrNameCnt] = 0;

	AttrName = false;
	AttrNameCnt = 0;
	AttrVal = true;
	AttrValCnt = 0;
	AttrQuote = 0;
}


void MxlParser::SetAttrVal(void)
{
	if (AttrValCnt) {
		CurrAttr->val = new unl[AttrValCnt + 1];
		memcpy(CurrAttr->val, txt32buf, AttrValCnt * sizeof(unl));
		CurrAttr->val[AttrValCnt] = 0;
		CurrAttr->vallen = AttrValCnt;
	}
	else {
		CurrAttr->val = NULL;
		CurrAttr->vallen = 0;
	}

	// add to attrbuf
	if ((CurrAttr->namelen == 5)
	 && (CurrAttr->name[0] == 'x')
	 && (CurrAttr->name[1] == 'm')
	 && (CurrAttr->name[2] == 'l')
	 && (CurrAttr->name[3] == 'n')
	 && (CurrAttr->name[4] == 's'))
		ReportError(Line, "Invalid attribute name: \"xmlns\"", NULL, false);
	else
		attrbuf[attrcnt++] = CurrAttr;

	CurrAttr = NULL;
	AttrName = true;
	AttrNameCnt = 0;
	AttrVal = false;
	AttrValCnt = 0;
	AttrQuote = 0;
}


void  MxlParser::AddAttributes(void)
{
	if (!attrcnt)
		currelem->attrs = NULL;
	else {
		currelem->attrs = new pair*[attrcnt];
		memcpy(currelem->attrs, attrbuf, attrcnt * sizeof(pair *));
	}
	currelem->attrcnt = attrcnt;
	attrcnt = 0;
}


void  MxlParser::DeleteAttributes(void)
{
	pair *attr = NULL;
	for (long i = 0; i < currelem->attrcnt; i++) {
		attr = currelem->attrs[i];
		delete [] (attr->name);
		delete [] (attr->val);
	}
	delete [] (currelem->attrs);
}


void MxlParser::GetEndTag(unc **cpt)
{
	unl u32 = 0;
	long cnt = 0;
	InEndTag = false;

	unc *end = (unc *) strchr((char *) *cpt, '>');
	if (!end) {  // because lineend between name and >
		end = *cpt + strlen((char *) *cpt);
		InEndTag = true;
	}
	while (*cpt < end) {
		u32 = U8to32Chr(cpt);
		if ((u32 == 0x20)
		 || (u32 == 0x09)
		 || (u32 == 0x0D)
		 || (u32 == 0x0A))
			continue;
		if (cnt ? IsValidUName(u32) : IsValidUNameStart(u32))
			txt32buf[cnt++] = u32;
	}
	if (*end)
		*cpt = end + 1;
	else
		*cpt = end;

	if ((cnt != currelem->namelen)
	 || memcmp(txt32buf, currelem->name, cnt * sizeof(unl))) {
		// mismatch of start and end
		ReportError(Line, "Mismatch of end tag to start tag: ",
		            (char *) U32to8Str(txt32buf, cnt), false);
		long i = 0;
		for (i = elemlevel - 1; i > 1; i--) {
			if ((cnt == elevels[i]->namelen)
			 && !memcmp(txt32buf, elevels[i]->name, cnt * sizeof(unl)))
				break;
		}
		if (i > 1) { // matched higher level
			ReportError(Line, "Recovering by closing up to matching tag", NULL, false);
			elemlevel = i;
			currelem = elevels[i];
			if (UseSAX)
				SetEnd(currelem->name, currelem->namelen);
			if (!UseModel) {
				delete [] (currelem->name);
				delete currelem;
			}
			currelem = elevels[--elemlevel];
			if (elemlevel < 1) {
				InDoc = false;
				EndDoc = true;
			}
		}
	}
	else if (elemlevel > 1) { // all done with currelem
		if (UseSAX)
			SetEnd(currelem->name, currelem->namelen);
		if (!UseModel) {
			delete [] (currelem->name);
			delete currelem;
		}
		currelem = elevels[--elemlevel];
	}
	else { // all done with this doc
		if (UseSAX)
			SetEnd(currelem->name, currelem->namelen);
		if (!UseModel) {
			delete [] (currelem->name);
			delete currelem;
		}
		InDoc = false;
		EndDoc = true;
	}
}


unl MxlParser::GetCharReference(unc **cpt)
{
//[15] charRef ::= numericCharRef | namedCharRef
//[16] numericCharRef ::= '&#x' charNumber ';'
//[17] charNumber ::= [0-9a-fA-F]+ 
//[18] namedCharRef ::= '&' charName ';'
//[19] charName ::= 'amp' | 'lt' | 'gt' | 'quot' | 'apos'

	if (!cpt || !*cpt || !**cpt)
		return 0;

	if (**cpt != '&')
		return (*(*cpt++));

	unl u32 = 0;
	char *err = (char *) *cpt;
	char *msg = NULL;

	(*cpt)++;
	if (**cpt == '#') {
		if (*(++(*cpt)) == 'x') {
			(*cpt)++;
			u32 = strtoul((char *) (*cpt), (char **) cpt, 16);
		}
		else
			msg = "Invalid numeric char reference: ";
		if (u32 == 0x0D)
			msg = "Invalid numeric char reference to \\r: ";
	}
	// must be named
	else if (!strncmp((char *) (*cpt), "amp", 3)) {
		//  * for amp, it is & (#x26);
		*cpt += 3;
		u32 = 0x26;
	}
	else if (!strncmp((char *) (*cpt), "lt", 2)) {
		//  * for lt, it is < (#x3C);
		*cpt += 2;
		u32 = 0x3C;
	}
	else if (!strncmp((char *) (*cpt), "gt", 2)) {
		//  * for gt, it is > (#x3E);
		*cpt += 2;
		u32 = 0x3E;
	}
	else if (!strncmp((char *) (*cpt), "quot", 4)) {
		//  * for quot, it is " (#x22);
		*cpt += 4;
		u32 = 0x22;
	}
	else if (!strncmp((char *) (*cpt), "apos", 4)) {
		//  * for apos, it is ' (#x27).
		*cpt += 4;
		u32 = 0x27;
	}
	else
		msg = "Invalid named char reference: ";

	if (msg) {
		while (**cpt && (**cpt != ';'))
			(*cpt)++;
		char *str = new char[(((char *) *cpt) - err) + 1];
		strncpy(str, err, ((char *) *cpt) - err);
		ReportError(Line, msg, str, false);
		delete [] str;
	}

	if (**cpt == ';')
		(*cpt)++;
	return u32;
}


void MxlParser::AddTextContent(long cnt)
{
	if (!currelem
	 && !UseSAX)
		return;

	// add text in txt32buf to element content
	cont *newcont = new cont;
	newcont->cnt = cnt;
	newcont->it = new unl[cnt + 1];;
	memcpy(newcont->it, txt32buf, cnt * sizeof(unl));
	((unl *)(newcont->it))[cnt] = 0;
	if (UseSAX)
		SetText((unl *) newcont->it, newcont->cnt);
	if (UseModel
	 && currelem) {
		cont **oldcont = currelem->content;
		currelem->content = new cont*[currelem->contcnt + 1];
		if (oldcont)
			memcpy(currelem->content, oldcont, currelem->contcnt * sizeof(cont *));
		currelem->content[currelem->contcnt++] = newcont;
	}
	else {
		delete [] (newcont->it);
		delete newcont;
	}
}



unl MxlParser::txt32buf[MAX_CHAR_COUNT];  // for writing data model
unc MxlParser::out8buf[MAX_CHAR_COUNT];   // for writing UTF-8

unl MxlParser::U8to32Chr(unc **cpt)
{
	// convert char or UTF-8 sequence in cpt to UTF-32
	// advance *cpt to next char

	unl uch = 0;

	if (!cpt || !*cpt || !**cpt)
		return 0;

	if ((**cpt & 0xF8) == 0xF0) { // start of 4-byte sequence
		uch = (*(*cpt)++ & 0x7) << 18;
		uch |= (*(*cpt)++ & 0x3F) << 12;
		uch |= (*(*cpt)++ & 0x3F) << 6;
		uch |= (*(*cpt)++ & 0x3F);
	}
	else if ((**cpt & 0xF0) == 0xE0) { // start of 3-byte sequence
		uch = (*(*cpt)++ & 0xF) << 12;
		uch |= (*(*cpt)++ & 0x3F) << 6;
		uch |= (*(*cpt)++ & 0x3F);
	}
	else if ((**cpt & 0xE0) == 0xC0) { // start of 2-byte sequence
		uch = (*(*cpt)++ & 0x1F) << 6;
		uch |= (*(*cpt)++ & 0x3F);
	}
	else {
		uch = **cpt;
		(*cpt)++;
	}

	return uch;
}


unc *MxlParser::U32to8Chr(unl uch, unc *cpt)
{
	// convert UTF-32 char in u32 to UTF-8 string
	// null-terminate string
	static unc ubuf[5] = { 0, 0, 0, 0, 0 };
	unc *ret = NULL;
	if (!cpt)
		ret = cpt = ubuf;

	if (!uch)
		*cpt = '\0';
	else if (uch < 0x80)
		*cpt++ = (unc) (uch & 0xFF);
	else if (uch < 0x0800) {
		*cpt++ = (unc) (0xC0 | ((uch >> 6) & 0x1F));
	  *cpt++ = (unc) (0x80 | (uch & 0x3F));
	}
	else if (uch < 0x10000) {
		*cpt++ = (unc) (0xE0 | ((uch >> 12) & 0xF));
		*cpt++ = (unc) (0x80 | ((uch >> 6) & 0x3F));
	  *cpt++ = (unc) (0x80 | (uch & 0x3F));
	}
	else {
		*cpt++ = (unc) (0xF0 | ((uch >> 18) & 0x7));
		*cpt++ = (unc) (0x80 | ((uch >> 12) & 0x3F));
		*cpt++ = (unc) (0x80 | ((uch >> 6) & 0x3F));
	  *cpt++ = (unc) (0x80 | (uch & 0x3F));
	}
	*cpt = '\0';
	return (ret ? ret : cpt);
}


unl *MxlParser::U8to32Str(unc **u8)
{
	// convert UTF-8 sequence in *u8 to UTF-32 array
	// advance *u8 to next char
	if (!u8 || !*u8 || !**u8)
		return NULL;

	unl u32 = 0;
	unl *ubuf = txt32buf;
	long ucnt = 0;

	while ((u32 = U8to32Chr(u8)) != 0)
		ubuf[ucnt++] = u32;

	if (!ucnt)
		return NULL;

	ubuf = new unl[ucnt + 1];
	memcpy(ubuf, txt32buf, ucnt * sizeof(unl));
	ubuf[ucnt] = 0;

	return ubuf;
}


unc *MxlParser::U32to8Str(unl *u32, long u32cnt)
{
	// convert u32cnt UTF-32 chars at *u32 to UTF-8 string
	// null-terminate string

	if (!u32cnt || !u32 || !*u32)
		return NULL;

	long ucnt = 0;
	unc *u8 = out8buf;
	long len = 0;
	unc *last = u8;

	while (ucnt < u32cnt) {
		u8 = U32to8Chr(u32[ucnt], u8);
		if (*last == '\0')
			u8 = last;
		last = u8;
		ucnt++;
	}

	len = u8 - out8buf;
	if (!len)
		return NULL;

	u8 = new unc[len + 1];
	memcpy(u8, out8buf, len * sizeof(unc));
	u8[len] = '\0';

	return u8;
}

  
bool MxlParser::IsValidUChar(unl u32)
{
//[23] char ::= s | ([#x0-#x10FFFF] - forbiddenCodePoint)
//[25] forbiddenCodePoint ::= controlCodePoint | surrogateCodePoint | nonCharacterCodePoint
//[26] controlCodePoint ::= [#x0-#x1F] | [#x7F-#x9F]
//[27] surrogateCodePoint ::= [#xD800-#xDFFF]
//[28] nonCharacterCodePoint ::= [#xFDD0-#xFDEF] | [#xFFFE-#xFFFF] | [#x1FFFE-#x1FFFF]
///                            | [#x2FFFE-#x2FFFF] | [#x3FFFE-#x3FFFF] | [#x4FFFE-#x4FFFF]
//                             | [#x5FFFE-#x5FFFF] | [#x6FFFE-#x6FFFF] | [#x7FFFE-#x7FFFF]
//                             | [#x8FFFE-#x8FFFF] | [#x9FFFE-#x9FFFF] | [#xAFFFE-#xAFFFF]
//                             | [#xBFFFE-#xBFFFF] | [#xCFFFE-#xCFFFF] | [#xDFFFE-#xDFFFF]
//                             | [#xEFFFE-#xEFFFF] | [#xFFFFE-#xFFFFF] | [#x10FFFE-#x10FFFF]

	if ((u32 < 0x20)
	 || (u32 > 0x10FFFF)
	 || ((u32 >= 0x7F) && (u32 <= 0x9F))
	 || ((u32 >= 0xD800) && (u32 <= 0xDFFF))
	 || ((u32 >= 0xFDD0) && (u32 <= 0xFDEF))
	 || ((u32 & 0xFFFE) == 0xFFFE)
	 || ((u32 & 0xFFFF) == 0xFFFF)) {
		char buf[10];
		ReportError(Line, "Invalid MicroXML char. ", ultoa(u32, buf, 16), false);
		return false;
	}

	return true;
}

bool MxlParser::IsValidUName(unl u32)
{
//[22] nameChar ::= nameStartChar | [0-9] | "-" | "." | #xB7 | [#x0300-#x036F] | [#x203F-#x2040]

	if (IsValidUNameStart(u32)
	 || ((u32 >= '0') && (u32 <= '9'))
	 || (u32 == '-')
	 || (u32 == '.')
	 || (u32 == 0xB7)
	 || ((u32 >= 0x300) && (u32 <= 0x36F))
	 || ((u32 >= 0x203F) && (u32 <= 0x2040)))
		return true;

	char buf[10];
	ReportError(Line, "Invalid name char. ", ultoa(u32, buf, 16), false);
	return false;
}

bool MxlParser::IsValidUNameStart(unl u32)
{
//[21] nameStartChar ::= [A-Z] | [a-z] | "_" | [#xC0-#xD6] | [#xD8-#xF6] | [#xF8-#x2FF] | [#x370-#x37D]
//                     | [#x37F-#x1FFF] | [#x200C-#x200D] | [#x2070-#x218F] | [#x2C00-#x2FEF]
//                     | [#x3001-#xD7FF] | ([#xF900-#xEFFFF] - nonCharacterCodePoint)
	if (IsValidUChar(u32)
	 || ((u32 >= 'A') && (u32 <= 'Z'))
	 || ((u32 >= 'a') && (u32 <= 'z'))
	 || (u32 == '_')
	 || ((u32 >= 0xC0) && (u32 <= 0xD6))
	 || ((u32 >= 0xD8) && (u32 <= 0xF6))
	 || ((u32 >= 0xF8) && (u32 <= 0x2FF))
	 || ((u32 >= 0x370) && (u32 <= 0x37D))
	 || ((u32 >= 0x37F) && (u32 <= 0x1FFF))
	 || ((u32 >= 0x200C) && (u32 <= 0x200D))
	 || ((u32 >= 0x2070) && (u32 <= 0x218F))
	 || ((u32 >= 0x2C00) && (u32 <= 0x2FEF))
	 || ((u32 >= 0x3001) && (u32 <= 0xD7FF))
	 || ((u32 >= 0xF900) && (u32 <= 0xEFFFF)))
		return true;

	char buf[10];
	ReportError(Line, "Invalid name start char. ", ultoa(u32, buf, 16), false);
	return false;
}


char *MxlParser::MakeStr(unc *start, unc *end)
{
	char *str = new char[(end - start) + 1];
	strncpy(str, (char *) start, end - start);
	str[end - start] = '\0';
	return str;
}


char *MxlParser::GetAttrVal(char *str, char **end)
{
	char *xstr = str;
	SkipSpace((unc **) &xstr);
	while (*xstr
	 && (*xstr != '='))
		xstr++;
	if (!*xstr)
		return NULL;
	return GetQuotedVal(xstr, end);
}


char *MxlParser::GetQuotedVal(char *str, char **end)
{
	char *xstr = str;
	SkipSpace((unc **) &xstr);
	while (*xstr
	 && (*xstr != '"')
	 && (*xstr != '\''))
		xstr++;
	if (!*xstr)
		return NULL;

	char quote = *xstr;
	char *start = ++xstr;

	while (*xstr
	 && (*xstr != quote))
		xstr++;
	if (!*xstr)
		return NULL;

	char *ret = new char[(xstr - start) + 1];
	memcpy(ret, start, xstr - start);
	ret[xstr - start] = '\0';

	if (end)
		*end = xstr + 1;
	return ret;
}


void MxlParser::ReportXMLCData(unc *cp)
{
	if (UseSAX
	 && FullXML)
		SetCData((char *) cp);
	else if (!FullXML)
		ReportError(Line, "CDATA is not allowed", NULL, false);
}

void MxlParser::ReportXMLDoctype(unc *cp)
{
	if (UseSAX
	 && FullXML)
		SetDoctype((char *) cp);
	else if (!FullXML)
		ReportError(Line, "DOCTYPE is not allowed", NULL, false);
}


void MxlParser::ReportXMLPI(unc *cp)
{
	if (UseSAX
	 && FullXML)
		SetPI((char *) cp);
	else if (!FullXML)
		ReportError(Line, "PIs are not allowed", NULL, false);
}



// adapter layer for expat emulation

void MxlParser::SetExpatCallbacks(char *ofile, reperr re, 
	              expatstart st, expatend et, expattxt tc, 
	              expdata scd, expdata ecd, 
                expatpi rp, expxmldec rxd,
								expsdtdec sdt, expdata edt, expcmt rc, void *ud)
{
	if (ofile)
		OutFile = fopen(ofile, "w");

	if (!OutFile)
		OutFile = stderr;
	if (re)
		ReportError = re;

	ExpatMode = true;

	if (st)
		ExpatStartTag = st;
	if (et)
		ExpatEndTag = et;
	if (tc)
		ExpatTextContent = tc;

	if (scd)
		ExpatStartCdataSection = scd;
	if (ecd)
		ExpatEndCdataSection = ecd;

	if (rp)
		ExpatPI = rp;
	if (rxd)
		ExpatXMLDecl = rxd;

	if (sdt)
		ExpatStartDoctypeDecl = sdt;
	if (edt)
		ExpatEndDoctypeDecl = edt;

	if (rc)
		ExpatComment = rc;
	if (ud)
		ExpatUserData = ud;
}



void MxlParser::SetStart(unl *name, long namecnt,
	                       pair **attrs, long attrcnt)
{
	if (!ExpatMode)
		StartTag(name, namecnt, attrs, attrcnt);
	else {
		// report attrs as NULL-terminated list of char *
		// with name and val alternating, empty val just '\0'
		// report in UTF-8

		char *name8 = (char *) U32to8Str(name, namecnt);
		char **attrs8 = NULL;
		pair *attr = NULL;
		char *attstr = NULL;

		if (attrs) {
			attrs8 = new char*[(attrcnt * 2) + 1];
			for (long i = 0; i < attrcnt; i++) {
				attr = attrs[i];
				if (!attr)
					break;
				attstr = (char *) U32to8Str(attr->name, attr->namelen);
				attrs8[i * 2] = attstr;
				attstr = (char *) U32to8Str(attr->val, attr->vallen);
				attrs8[(i * 2) + 1] = attstr;
			}
			attrs8[i * 2] = NULL;
		}

		ExpatStartTag(ExpatUserData, name8, (const char **) attrs8);
	}
}

void MxlParser::SetEnd(unl *name, long namecnt)
{
	if (!ExpatMode)
		EndTag(name, namecnt);
	else {
		// report in UTF-8
		char *name8 = (char *) U32to8Str(name, namecnt);

		ExpatEndTag(ExpatUserData, name8);
	}
}
;
void MxlParser::SetText(unl *text, long textcnt)
{
	if (!ExpatMode)
		TextContent(text, textcnt);
	else {
		// report in UTF-8
		char *text8 = (char *) U32to8Str(text, textcnt);

		ExpatTextContent(ExpatUserData, text8, strlen(text8));
	}
}


void MxlParser::SetPI(char *pi)
{
	if (!ExpatMode)
		ReportPI(pi);
	else {
		// report target and data separately, already UTF-8
		// handle multi-line PIs in ExPIBuf

		// add to buffer
		strcpy(ExPIBuf + ExPICnt, pi);
		ExPICnt += strlen(pi);
		
		if (!InPI)  {  // put out, clear buffer
			if ((Line == 1)
			 && !strncmp(ExPIBuf, "xml ", 4))
				ReportExpatXMLDecl(ExPIBuf);
			else {
				char *target = ExPIBuf;
				char *data = strchr(ExPIBuf, ' ');
				if (data)
					*data++ = '\0';
				ExpatPI(ExpatUserData, target, data);
			}
			ExPIBuf[0] = '\0';
			ExPICnt = 0;
		}
	}
}


void MxlParser::ReportExpatXMLDecl(char *cp)
{
	char *ver = NULL;
	char *enc = NULL;
	int standalone = -1;

	char *str = strstr(cp, "version");
	if (str)
		ver = GetAttrVal(str + 7);
	str = strstr(cp, "encoding");
	if (str)
		enc = GetAttrVal(str + 8);

	ExpatXMLDecl(ExpatUserData, ver, enc, standalone);
}


void MxlParser::SetDoctype(char *doctype)
{
	if (!ExpatMode)
		ReportDoctype(doctype);
	else {
		// add to buffer
		strcpy(ExPIBuf + ExPICnt, doctype);
		ExPICnt += strlen(doctype);
		
		if (!InDoctype)  {  // put out, clear buffer
			ReportExpatDoctypeDecl(ExPIBuf);
			ExPIBuf[0] = '\0';
			ExPICnt = 0;
		}
	}
}


void MxlParser::ReportExpatDoctypeDecl(char *cp)
{
	char *name = NULL;
	char *sys = NULL;
	char *pub = NULL;
	int intsub = 0;
	char *str = NULL;
	char *end = NULL;

	if ((str = strstr(cp, "DOCTYPE")) != NULL) {
		str += 7;
		SkipSpace((unc **) &str);
	}

	char *spc = strchr(str, ' ');
	if (spc) {
		name = new char[(spc - str) + 1];
		memcpy(name, str, spc - str);
		name[spc - str] = '\0';
		if ((str = strstr(spc, "SYSTEM")) != NULL)
			sys = GetQuotedVal(str + 6);
		if ((str = strstr(spc, "PUBLIC")) != NULL) {
			pub = GetQuotedVal(str + 6, &end);
			if (pub && end)
				sys = GetQuotedVal(end);
		}
	}
	else
		name = cp;	

	ExpatStartDoctypeDecl(ExpatUserData, name, sys, pub, intsub);
	ExpatEndDoctypeDecl(ExpatUserData);
}


void MxlParser::SetCData(char *cdata)
{
	if (!ExpatMode)
		ReportCData(cdata);
	else {
		// add to buffer
		strcpy(ExPIBuf + ExPICnt, cdata);
		ExPICnt += strlen(cdata);
		
		if (!InCDATA)  {  // put out, clear buffer
			ReportExpatCDataSection(ExPIBuf);
			ExPIBuf[0] = '\0';
			ExPICnt = 0;
		}
	}
}


void MxlParser::ReportExpatCDataSection(char *cp)
{
	ExpatStartCdataSection(ExpatUserData);
	ExpatTextContent(ExpatUserData, cp, strlen(cp));
	ExpatEndCdataSection(ExpatUserData);
}


void MxlParser::Put8JSON(char *u8)
{
	unl u32 = 0;
	char **cpt = &u8;

	fputc('"', OutFile);
	while (**cpt) {
		u32 = **cpt;
		if (u32 < 0x20) {
			if (u32 == 0x09)
				fputs("\\t", OutFile);
			else if (u32 == 0x0A)
				fputs("\\n", OutFile);
			(*cpt)++;
		}
		else if (u32 > 0x7F) {
			u32 = U8to32Chr((unc **) cpt);  // advances ptr
			fprintf(OutFile, "\\u%04lX", u32);
		}
		else {
			fputc(u32, OutFile);
			(*cpt)++;
		}
	}
	fputc('"', OutFile);
}



void MxlParser::StubExpatStart(void *user, const char *name, const char **attrs)
{
	fputs("ExStartTag: ", OutFile);
	Put8JSON((char *) name);
	fputs("\n", OutFile);

	char *attr = NULL;

	if (attrs) {
		while ((attr = (char *) *attrs++) != NULL) {
			fputs("  ", OutFile);
			Put8JSON(attr);
			fputs(" = ", OutFile);
			attr = (char *) *attrs++;
			Put8JSON(attr);
			fputs("\n", OutFile);
		}
	}
}

void MxlParser::StubExpatEnd(void *user, const char *name)
{
	fputs("ExEndTag: ", OutFile);
	Put8JSON((char *) name);
	fputs("\n", OutFile);
}

void MxlParser::StubExpatText(void *user, const char *text, int textcnt)
{
	fputs("ExTextContent: ", OutFile);
	Put8JSON((char *) text);
	fputs("\n", OutFile);
}


void MxlParser::StubExpatStartCdataSection(void *user)
{
	fputs("ExStartCdataSection\n", OutFile);
}


void MxlParser::StubExpatEndCdataSection(void *user)
{
	fputs("ExEndCdataSection\n", OutFile);
}


void MxlParser::StubExpatPI(void *user, const char *target, const char *data)
{
	// write PI to OutFile
	fputs("ExPI: ", OutFile);
	char *cr = strchr(data, '\n');
	if (cr)
		*cr = '\0';
	fputs(target, OutFile);
	fputc(' ', OutFile);
	fputs(data, OutFile);
	fputs("\n", OutFile);
}


void MxlParser::StubExpatXMLDecl(void *user, const char *ver, const char *enc, int stand)
{
	fputs("ExXMLDecl: ", OutFile);
	if (ver) {
		fputs("version = ", OutFile);
		Put8JSON((char *) ver);
		if (enc)
			fputs(", ", OutFile);
	}
	if (enc) {
		fputs("encoding = ", OutFile);
		Put8JSON((char *) enc);
	}
	fputs("\n", OutFile);
}


void MxlParser::StubExpatStartDoctypeDecl(void *user, const char *name,
	                  const char *sys, const char *pub, int intsub)
{
	fputs("ExStartDoctype: ", OutFile);
	if (name)
		Put8JSON((char *) name);
	fputs("\n", OutFile);
	if (pub) {
		fputs("  Public: ", OutFile);
		Put8JSON((char *) pub);
		fputs("\n", OutFile);
	}
	if (sys) {
		fputs("  System: ", OutFile);
		Put8JSON((char *) sys);
		fputs("\n", OutFile);
	}
}


void MxlParser::StubExpatEndDoctypeDecl(void *user)
{
	fputs("ExEndDoctype\n", OutFile);
}


void MxlParser::StubExpatComment(void *user, char *comment)
{
	fputs("ExComment: ", OutFile);
	Put8JSON(comment);
	fputs("\n", OutFile);
}
  

// functions for regular callbacks


FILE *MxlParser::OutFile = stderr;

void MxlParser::SetOptions(bool us, bool um, bool fx)
{
	UseSAX = us;
	UseModel = um;
	FullXML = fx;
}

;
void MxlParser::SetCallbacks(char *ofile, reperr re,
	                           starttag st, endtag et, txtcont tc, 
	                           repcmt rcd, repcmt rp,
														 repcmt rdt, repcmt rc)
{
	if (ofile)
		OutFile = fopen(ofile, "w");

	if (!OutFile)
		OutFile = stderr;
	if (re)
		ReportError = re;

	if (st)
		StartTag = st;
	if (et)
		EndTag = et;
	if (tc)
		TextContent = tc;
	if (rcd)
		ReportCData = rcd;

	if (rp)
		ReportPI = rp;
	if (rdt)
		ReportDoctype = rdt;
	if (rc)
		ReportComment = rc;
}


void MxlParser::Put32JSON(unl *u32arr, long cnt)
{
	unl u32 = 0;

	fputc('"', OutFile);
	for (long i = 0; i < cnt; i++) {
		u32 = u32arr[i];
		if (u32 < 0x20) {
			if (u32 == 0x09)
				fputs("\\t", OutFile);
			else if (u32 == 0x0A)
				fputs("\\n", OutFile);
		}
		else if (u32 > 0x7F)
			fprintf(OutFile, "\\u%04lX", u32);
		else
			fputc(u32, OutFile);
	}
	fputc('"', OutFile);
}


void MxlParser::StubStart(unl *name, long namecnt, pair **attrs, long attrcnt)
{
	fputs("StartTag: ", OutFile);
	Put32JSON(name, namecnt);
	fputs("\n", OutFile);

	pair *attr = NULL;

	if (attrcnt) {
		for (long i = 0; i < attrcnt; i++) {
			attr = attrs[i];
			fputs("  ", OutFile);
			Put32JSON(attr->name, attr->namelen);
			fputs(" = ", OutFile);
			Put32JSON(attr->val, attr->vallen);
			fputs("\n", OutFile);
		}
	}
}

void MxlParser::StubEnd(unl *name, long namecnt)
{
	fputs("EndTag: ", OutFile);
	Put32JSON(name, namecnt);
	fputs("\n", OutFile);
}

void MxlParser::StubText(unl *text, long textcnt)
{
	fputs("TextContent: ", OutFile);
	Put32JSON(text, textcnt);
	fputs("\n", OutFile);
}

void MxlParser::StubError(long line, char *warning, char *cpt, bool fatal)
{
	char buf[10];

	// write warning to OutFile
	fputs("Warning, on line ", OutFile);
	fputs(ultoa(line, buf, 10), OutFile);
	fputs(": ", OutFile);
	fputs(warning, OutFile);
	if (cpt)
		fputs(cpt, OutFile);
	fputs("\n", OutFile);
}


void MxlParser::StubComment(char *comment)
{
	// write comment to OutFile
	fputs("Comment: ", OutFile);
	char *cr = strchr(comment, '\n');
	if (cr)
		*cr = '\0';
	fputs(comment, OutFile);
	fputs("\n", OutFile);
}

void MxlParser::StubPI(char *pi)
{
	// write PI to OutFile
	fputs("PI: ", OutFile);
	char *cr = strchr(pi, '\n');
	if (cr)
		*cr = '\0';
	fputs(pi, OutFile);
	fputs("\n", OutFile);
}

void MxlParser::StubDoctype(char *doc)
{
	// write PI to OutFile
	fputs("DOCTYPE: ", OutFile);
	char *cr = strchr(doc, '\n');
	if (cr)
		*cr = '\0';
	fputs(doc, OutFile);
	fputs("\n", OutFile);
}

void MxlParser::StubCData(char *cdata)
{
	// write PI to OutFile
	fputs("CDATA: ", OutFile);
	char *cr = strchr(cdata, '\n');
	if (cr)
		*cr = '\0';
	fputs(cdata, OutFile);
	fputs("\n", OutFile);
}


// end of mxlparser.cpp

