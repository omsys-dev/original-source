
// dwhtmfl.cc contains code for the htm file write classes, JHG
// Copyright (c) 1996-2000 by Omni Systems, Inc.  All rights reserved.

#include "dcglob.h"
#include "dcbase.h"
#include "dclist.h"
#include "dcfile.h"
#include "dcfilw.h"
#include "dwhtmfl.h"



HTMwfile::HTMwfile(char *fname) : DCwfile(fname, ftxt)
{
  LinePos = 0;
	PreForm = false;
	NoWrap = false;
	Xhtml = false;
	Xml = false;
	ClassAttrName = "class";
	AttrDiv = 0;
	AttrCount = 0;
	InAttrs = false;
	AttrAdd = false;
	EmptyAttr = false;
}


void
HTMwfile::ReopenFile(char *fname)
{
  DCwfile::ReopenFile(fname);
	LinePos = 0;
}


int
HTMwfile::putChar(int ch)
{
	if (!InAttrs)
		LinePos++;
	if ((ch & 0xFF) == '\0')
		return 0;
	return DCwfile::putChar(ch);
}


int
HTMwfile::putHexB(int ch)
{
	putHex((ch >> 4) & 0xF);
	return putHex(ch & 0xF); 
}


int
HTMwfile::putStr(char *s, size_t len)
{
	if (s == NULL)
		return 0;
	if (len == 0)
		len = strlen(s);
	if (!InAttrs)
		LinePos += len;
	return DCwfile::putStr(s, len); 
}


void
HTMwfile::putTextStr(char *s)
{
	while (*s)
		putText(*s++);
}


void
HTMwfile::putEscStr(char *s, size_t len)
{
	if (s == NULL)
		return;
	if (len == 0)
		len = strlen(s);
	while (*s && (len > 0)) {
		if (*s == '<') {
			s++;
			putEntity("lt");
		}
		else if (*s == '>') {
			s++;
			putEntity("gt");
		}
		else if (*s == '&') {
			s++;
			putEntity("amp");
		}
		else
			putChar(*s++);
		len--;
	}
}


int
HTMwfile::putSpec(int ch)
{
	putChar('&');
	putChar('#');
	putUShort((uns) ch);
	return putChar(';'); 
}


int
HTMwfile::putEntity(char *str)
{
	putChar('&');
	putStr(str);
	return putChar(';'); 
}


int
HTMwfile::putEnd(void)
{
	//if (InAttrs)
	//	return 0;
	LinePos = 0;
	return DCwfile::putCr(); 
}


void
HTMwfile::putText(int ch)
{
#if 0
	putChar(ch);

	if ((ch == ' ')
	 && (!PreForm)
	 && (!NoWrap)
	 && (LinePos >= LineHot))
		putEnd();
#else

	if ((ch == ' ')
	 && (!PreForm)
	 && (!NoWrap)
	 && (LinePos >= LineHot))
		putEnd();
	else
		putChar(ch);
#endif
}


void
HTMwfile::putShort(short n)
{
  if (n < 0) {
    putChar('-');
    n = -n;
  }

	putUShort((uns) n);
}


void
HTMwfile::putUShort(uns n)
{
  if (n == 0) {
    putChar('0');
    return;
  }

  if (n >= 10000) {
    putChar((n / 10000) + '0');
    n %= 10000;
    putChar((n / 1000) + '0');
    n %= 1000;
    putChar((n / 100) + '0');
    n %= 100;
    putChar((n / 10) + '0');
    n %= 10;
  }
  else if (n >= 1000) {
    putChar((n / 1000) + '0');
    n %= 1000;
    putChar((n / 100) + '0');
    n %= 100;
    putChar((n / 10) + '0');
    n %= 10;
  }
  else if (n >= 100) {
    putChar((n / 100) + '0');
    n %= 100;
    putChar((n / 10) + '0');
    n %= 10;
  }
  else if (n >= 10) {
    putChar((n / 10) + '0');
    n %= 10;
  }

  putChar(n + '0');
}


// elements and attributes


void
HTMwfile::putElem(char *str, bool block)
{
	putText('<');
  putStr(str);

	if ((Xhtml || Xml) && !block) {
		putChar(' ');
		putChar('/');
	}
	putChar('>');
}


void
HTMwfile::startElem(char *str)
{
	putText('<');
  putStr(str);
	startAttrs();
}

void
HTMwfile::startAttrs(void)
{
	bool bug = false;
	if (AttrCount) {
		bug = true;
		//assert(0);
		AttrNames.empty();
		AttrVals.empty();
		AttrCount = 0;
	}
	if (AttrDiv) {
		bug = true;
		assert(0);
	}
	InAttrs = true;
}


void
HTMwfile::endElem(bool block)
{
	endAttrs();

	if ((Xhtml || Xml) && !block) {
		putChar(' ');
		putChar('/');
	}
	putChar('>');
}


void
HTMwfile::endAttrs(void)
{
	InAttrs = false;

	if (AttrCount) {
		WriteAttrs();
		AttrNames.empty();
		AttrVals.empty();
		AttrCount = 0;
	}
}

void
HTMwfile::SaveAttrs(void)
{
	if (AttrCount) {
		SavedAttrCount = AttrCount;
		SavedAttrNames.next = AttrNames.next;
		SavedAttrVals.next = AttrVals.next;
		AttrNames.next = AttrVals.next = NULL;
		AttrCount = 0;
	}
}

void
HTMwfile::RestoreAttrs(void)
{
	if (SavedAttrCount) {
		AttrCount = SavedAttrCount;
		AttrNames.next = SavedAttrNames.next;
		AttrVals.next = SavedAttrVals.next;
		SavedAttrNames.next = SavedAttrVals.next = NULL;
		SavedAttrCount = 0;
	}
}


void
HTMwfile::putDT(char *doc, char *dtype, char *dtd, bool rets, bool ents)
{
	if (!dtype
	 && (!dtd || !*dtd))
		return;
	putText('<');
	putStr("!DOCTYPE ");
  putStr(doc);
	if (dtype) {
		putStr(" PUBLIC ");
		if (rets)
			putEnd();
		putStr(dtype);
	}
	else {
		putStr(" SYSTEM ");
	}
	if (dtd && *dtd) {
		if (strlen(dtd) > 16)
			putEnd();
		else if (*dtd != ' ')
			putChar(' ');
		putStr(dtd);
	}
	if (!ents) {
		endElem(true);
		putEnd();
	}
}


void
HTMwfile::putXML(char *enc, char *ver)
{
	LinePos = 0;
	putText('<');
	putStr("?xml");
	startAttrs();
	strAttr("version", ver ? ver : "1.0");
	strAttr("encoding", enc ? enc : "ISO-8859-1");
	endAttrs();
	putStr(" ?>");
	putEnd();
}



void
HTMwfile::startPI(char *str)
{
	putText('<');
	putChar('?');
  putStr(str);
	startAttrs();
}


void
HTMwfile::endPI(void)
{
	endAttrs();
	putStr(" ?>");
}


void
HTMwfile::startAttr(char *attr)
{
	if (!attr)
		return;

	PreAttrDiv = GetDiv();
	AttrDiv = NewDiv();
	AttrName = attr;

#if 0
	putChar(' ');
	putStr(attr);
	putChar('=');
	putChar('"');
#endif
}


void
HTMwfile::endAttr(void)
{
	long len = 0;
	char *val = NULL;
	char *quo = NULL;
	char *nv = NULL;
	long id = 0;

	assert(AttrDiv);

	if (((len = LenDiv()) != 0)
	 || EmptyAttr) {
		val = ReadDiv();
		if ((id = AttrNames(AttrName)) != 0) { // exists
			if (val) {  // not empty
				while ((quo = strchr(val, '"')) != NULL) {
					nv = NewName(strlen(val) + 6);
					if (quo == val) {
						strcpy(nv, "&#34;");
						strcat(nv, val + 1);
					}
					else {
						strncpy(nv, val, quo - val);
						strcat(nv, "&#34;");
						strcat(nv, quo + 1);
					}
					val = nv;
				}
				if (AttrAdd) {
					char *oval = AttrVals.find(id);
					if (oval) {  // not empty
						char *nval = NewName(oval, strlen(oval) + strlen(val) + 1);
						strcat(nval, " ");
						strcat(nval, val);
						AttrVals.replace(nval, id);
					}
					else  // empty, just replace
						AttrVals.replace(NewName(val), id);
				}
				else   // replace it
					AttrVals.replace(NewName(val), id);
			}
		}
		else { // add even if empty
			AttrNames.addnew(AttrName, ++AttrCount);
			AttrVals.addnew(val, AttrCount);
		}
	}
	DeleteDiv(AttrDiv);
	SelectDiv(PreAttrDiv);		
	AttrDiv = 0;

#if 0
	putChar('"');
#endif
}


void HTMwfile::WriteAttrs()
{
	DCnlist *att = &AttrNames;
	char q = '"';

	while (att->next != NULL) {
		att = att->next;
		char *n = (char *) att->name;
		char *v = AttrVals.find(att->id);
		long vlen = 0;
		if (!v)       // because of EmptyAttr
			q = '"';
		else {
			q = (strchr(v, '"') != NULL) ? '\'' : '"';
			vlen = strlen(v);
		}
		if (!NoWrap
		 && (LinePos > 0)
		 && ((LinePos + strlen(n) + vlen + 4) > LineMax))
			putEnd();
		else
			putChar(' ');

		putStr(n);
		putChar('=');
		putChar(q);
		if (v)
			putAttrValue(v);
		putChar(q);
	}
}


void
HTMwfile::putAttrValue(char *str)
{
	size_t len = 0;
	char *nstr = NewName(10);

	while (*str) {
		switch (*str) {
			case '<':
				putStr("&lt;");
				break;
			case '>':
				putStr("&gt;");
				break;

			case '&': // if already an entity, don't double code it
				if ((len = IsEntity((char *) (str + 1))) > 0) {
					putStr(str, len + 1);
					str += len;
				}
				else
					putStr("&amp;");
				break;

			case '\'':
				putStr("&apos;");
				break;

			case '\n':
			case '\r':
				break;

			default:
				if ((*str > 0) && (*str < ' '))
					break;
				//else if (*str > 0x7E) {
				//	sprintf(nstr, "&#%0.3hd;", ((short) *str) & 0xFF);
				//	putStr(nstr);
				//}
				//else
				putChar(*str);
				break;
		}
		str++;
	}
}


size_t
HTMwfile::IsEntity(char *str)
{
	// called with possible entity, starting after '&'
	size_t len = 0;
	if (*str == '#') {
		len = 1;
		str++;
	}
	while (*str
	 && isalnum(*str)) {
		len++;
		str++;
	}
	return (*str == ';') ? len + 1 : 0;
}



void
HTMwfile::putAttrStr(char *str)
{
	//if (!str || !*str || !InAttrs)
	if (!str || !*str)
		return;

	// process str containing attrs
	char *nm = NULL;
	char *val = NULL;
	char q = 0;

	char *xstr = NewName(str);  // make local copy
	str = xstr;

	while (str) {
		while (*str == ' ')
			str++;
		nm = str;
		while (*str && (*str != '='))
			str++;
		if (*str != '=')
			return;
		*str++ = '\0';
		val = str;
		if ((*str == '"') || (*str == '\''))
			q = *str++;
		else
			q = ' ';
		val = str;
		while (*str && (*str != q))
			str++;
		if (*str == q)
			*str++ = '\0';
		strAttr(nm, val);
	}
}


void
HTMwfile::strAttr(char *attr, char *str)
{
	if (!attr
	 || !str)
		return;

	startAttr(attr);
	if (str)
		putStr(str);
	endAttr();
}


void
HTMwfile::numAttr(char *attr, short num, char *str)
{
	if (!attr)
		return;

	startAttr(attr);
  putShort(num);
	if (str)
		putStr(str);
	endAttr();
}


void
HTMwfile::pctAttr(char *attr, short num)
{
	if (!attr)
		return;

	startAttr(attr);
	if (num > 100) {
		putShort(num / 100);
		putChar('.');
		putShort(num % 100);
	}
	else
	  putShort(num);
	putChar('%');
	endAttr();
}


void
HTMwfile::pxAttr(char *attr, short num)
{
	if (!attr)
		return;

	startAttr(attr);
  putShort(num);
	putChar('p');
	putChar('x');
	endAttr();
}


void
HTMwfile::starAttr(char *attr, short num)
{
	if (!attr)
		return;

	startAttr(attr);
  putShort(num);
	putChar('*');
	endAttr();
}


void
HTMwfile::numPlusAttr(char *attr, short num)
{
	if (!attr)
		return;

	startAttr(attr);
	putChar('+');
  putShort(num);
	endAttr();
}


void
HTMwfile::numMinusAttr(char *attr, short num)
{
	if (!attr)
		return;

	startAttr(attr);
	putChar('-');
  putShort(num);
	endAttr();
}


void
HTMwfile::colorAttr(char *attr, long color)
{
	if (!attr)
		return;

	color &= 0xFFFFFF;  // so -1 becomes white

	startAttr(attr);
	putChar('#');
  putHexB((color >> 16) & 0xFF); // red
  putHexB((color >> 8) & 0xFF);  // green
  putHexB(color & 0xFF);         // blue
	endAttr();
}


void
HTMwfile::endLine()
{
	if (!PreForm)
	// && !NoWrap)
		putElem("br");
	putEnd();
}


void
HTMwfile::startComment(void)
{
	startElem("!-- ");
}


void
HTMwfile::endComment(void)
{
	putStr(" --");
	endElem(true);
}


void
HTMwfile::putComment(char *str)
{
	if (!str)
		return;

	startComment();
	putStr(str);
	endComment();
}


void
HTMwfile::startError(void)
{
	startComment();
	putStr("Error: ");
}


void
HTMwfile::endError(void)
{
	endComment();
}


void
HTMwfile::putError(char *str)
{
	if (!str)
		return;

	startError();
	putStr(str);
	endError();
}



// HTM block processing

#ifdef BLOCKSTACK
int HTMwfile::HTMlevel = 0;
char *HTMwfile::HTMstack[HtmLevelMax];
#endif

void
HTMwfile::writeBlock(char *str, char *txt)
{
	startSBlock(str);
	putStr(txt);
	endBlock(str);
	putEnd();
}

void
HTMwfile::writeTextBlock(char *str, char *txt)
{
	startSBlock(str);
	putTextStr(txt);
	endBlock(str);
	putEnd();
}

void
HTMwfile::startBlock(char *str)
{
#ifdef BLOCKSTACK

	// first check if same tag on stack...
	// unless it's nestable, close prev block

	// put tag out
  startElem(str);

	if (HTMlevel < HtmLevelMax) {
		HTMstack[HTMlevel] = str;
		HTMlevel++;
	}
	else putError("HTMstack overflow");

#else

	// start block with attributes
  startElem(str);
	if (!stricmp(str, "pre"))
		PreForm = true;

#endif
}


void
HTMwfile::startSBlock(char *str)
{
#ifdef BLOCKSTACK

	startBlock(str);
	endElem(true);

#else

	// block start without attributes
  putElem(str, true);

#endif
}


void
HTMwfile::endBlock(char *str)
{
#ifdef BLOCKSTACK

	int i;

	if (str != NULL) {  // find match
		for (i = HTMlevel; i > 0; i--) {
			if (!strcmp(HTMstack[i - 1], str))
				break;
		}
		if (i == 0) {  // not found on stack, logic error
			putBlockEnd(str);
			putError("BlockEnd string not on HTMstack");
			return;
		}
		while (i <= HTMlevel) {  // close nested blocks
			putBlockEnd(HTMstack[--HTMlevel]);
			HTMstack[HTMlevel] = NULL;
		}
	}
	else if (HTMlevel) { // null string, use last on stack
		putBlockEnd(HTMstack[--HTMlevel]);
		HTMstack[HTMlevel] = NULL;
	}
	else {  // null string, empty stack; error
		putBlockEnd("ERROR");
		putError("No BlockEnd string, HTMstack empty");
	}

#else

	putBlockEnd(str);
	if (!stricmp(str, "pre"))
		PreForm = false;

#endif
}


void
HTMwfile::putBlockEnd(char *str)
{
	putChar('<');
	putChar('/');	

	while (*str) {
		if (*str == ' ')
			break;
		putChar(*str++);
	}
  //putStr(str);
	putChar('>');
}


void
HTMwfile::setAlign(ParaAlign align)
{
	switch(align) {
		default:
		case PA_left:
			strAttr("align", "left");
			break;
		case PA_center:
			strAttr("align", "center");
			break;
		case PA_right:
			strAttr("align", "right");
			break;
	}
}


void 
HTMwfile::setClass(char *clname)
{
	strAttr(ClassAttrName, clname);
}


void 
HTMwfile::setClassAttrName(char *clattrname)
{
	ClassAttrName = clattrname;
}


void
HTMwfile::endPara()
{
	endBlock("p");
	putEnd();
}


void 
HTMwfile::putAName(char *str, char *tag, bool linebreak)
{
	if (!str || (*str == '\0'))
		return;
	if (!tag)
		tag = "a";

	startBlock(tag);    // DITA notify ****
	if (linebreak && !PreForm && !NoWrap)
		putEnd();
	if (Xhtml || Xml)
		strAttr("id", str);
	if (!Xml)
		strAttr("name", str);
	endElem(true);
	endBlock(tag);  // DITA notify ****
}


void 
HTMwfile::putAHref(char *str, char *targ, bool linebreak)
{
	startBlock("a");
	if (linebreak && !PreForm && !NoWrap)
		putEnd();  // put inside element to avoid unwanted spacing
	strAttr("href", str);
	if (targ)
		strAttr("target", targ);
	endElem(true);
	// endBlock("a") later, after text of anchor
}


// end of dwhtmfl.cpp

