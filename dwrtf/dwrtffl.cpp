
// dwrtffl.cc contains code for the rtf file write classes, JHG, 4/19/97
// Copyright (c) 1993-1997 by Omni Systems, Inc.  All rights reserved.

#include "dcglob.h"
#include "dcbase.h"
#include "dclist.h"
#include "dcfile.h"
#include "dcfilw.h"
#include "dwrtffl.h"



RTFwfile::RTFwfile(const char *fname) : DCwfile(fname, ftxt)
{
  if (Ferr
   || (fname == NULL))
    return;


  LinePos = 0;
	AutoCR = true;

	Meta = 'I';  // Internal is default
	metanamewid = 5;
	metanumwid = 3;
	metanum = 0;
	*metaname = '\0';

  putStr("{\\rtf1 ");
}


void
RTFwfile::CloseFile()
{
  putStr("\n}\n");
  DCwfile::CloseFile();
}


int
RTFwfile::putChar(int ch)
{
  ch = DCwfile::putChar(ch);
	LastGE = false;
	if (AutoCR)
	  LinePos++;
  CheckErr();
  return ch;
}


int
RTFwfile::putStr(const char *s, size_t len)
{
  int ret = DCwfile::putStr(s, len);
	if (AutoCR)
	  LinePos += len ? len : strlen(s);
	LastGE = false;
  CheckErr();
  return ret;
}


void
RTFwfile::putShort(short n, bool neg)
{
  if (n > 31680)
    n = 31680;
  else if (n < -31680)
    n = -31680;

  if ((n == 0)
	 || ((n < 0) && !neg)) {
    putChar('0');
    return;
  }

  if (n < 0) {
    putChar('-');
    n = -n;
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


void
RTFwfile::putLong(long n, bool neg)
{
  if ((n == 0)
	 || ((n < 0) && !neg)) {
    putChar('0');
    return;
  }

  if (n < 0) {
    putChar('-');
    n = -n;
  }

  if (n >= 1000000000) {
    putChar((n / 1000000000) + '0');
    n %= 1000000000;
    putChar((n / 100000000) + '0');
    n %= 100000000;
    putChar((n / 10000000) + '0');
    n %= 10000000;
    putChar((n / 1000000) + '0');
    n %= 1000000;
    putChar((n / 100000) + '0');
    n %= 100000;
    putChar((n / 10000) + '0');
    n %= 10000;
    putChar((n / 1000) + '0');
    n %= 1000;
    putChar((n / 100) + '0');
    n %= 100;
    putChar((n / 10) + '0');
    n %= 10;
  }
  else if (n >= 100000000) {
    putChar((n / 100000000) + '0');
    n %= 100000000;
    putChar((n / 10000000) + '0');
    n %= 10000000;
    putChar((n / 1000000) + '0');
    n %= 1000000;
    putChar((n / 100000) + '0');
    n %= 100000;
    putChar((n / 10000) + '0');
    n %= 10000;
    putChar((n / 1000) + '0');
    n %= 1000;
    putChar((n / 100) + '0');
    n %= 100;
    putChar((n / 10) + '0');
    n %= 10;
  }
  else if (n >= 10000000) {
    putChar((n / 10000000) + '0');
    n %= 10000000;
    putChar((n / 1000000) + '0');
    n %= 1000000;
    putChar((n / 100000) + '0');
    n %= 100000;
    putChar((n / 10000) + '0');
    n %= 10000;
    putChar((n / 1000) + '0');
    n %= 1000;
    putChar((n / 100) + '0');
    n %= 100;
    putChar((n / 10) + '0');
    n %= 10;
  }
  else if (n >= 1000000) {
    putChar((n / 1000000) + '0');
    n %= 1000000;
    putChar((n / 100000) + '0');
    n %= 100000;
    putChar((n / 10000) + '0');
    n %= 10000;
    putChar((n / 1000) + '0');
    n %= 1000;
    putChar((n / 100) + '0');
    n %= 100;
    putChar((n / 10) + '0');
    n %= 10;
  }
  else if (n >= 100000) {
    putChar((n / 100000) + '0');
    n %= 100000;
    putChar((n / 10000) + '0');
    n %= 10000;
    putChar((n / 1000) + '0');
    n %= 1000;
    putChar((n / 100) + '0');
    n %= 100;
    putChar((n / 10) + '0');
    n %= 10;
  }
  else if (n >= 10000) {
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


void
RTFwfile::putEnd(void)
{
	DCwfile::putChar('\n');
	LinePos = 0; 
}


void
RTFwfile::putHexB(int ch)
{ 
	putHex((ch >> 4) & 0xF); 
	putHex(ch & 0xF); 
}


void
RTFwfile::putText(int ch)
{ 
	if ((LastGE == true)
	 && (ch == ' ')) 
		putChar(' '); 
	if (AutoCR
	 && (LinePos >= LineMax))
		putEnd(); 
	putChar(ch); 
}


void
RTFwfile::putCS(int ch)
{ 
	CheckHot(); 
	putChar('\\'); 
	putChar(ch); 
}


void
RTFwfile::putCW(const char *s)
{ 
	CheckHot(); 
	putChar('\\'); 
	putStr(s); 
	putChar(' '); 
}


void
RTFwfile::putCWN(const char *s, short n, bool neg)
{ 
	CheckHot(); 
	putChar('\\'); 
	putStr(s); 
	putShort(n, neg); 
	putChar(' '); 
}


void
RTFwfile::putGS(void)
{ 
	CheckHot(); 
	putChar(0x7b); 
}


void
RTFwfile::putGE(bool spset)
{ 
	putChar(0x7d); 
	CheckHot();
	if (spset)
		LastGE = true; 
}


void
RTFwfile::putSpec(unc ch)
{ 
	putCS('\''); 
	putHex((ch >> 4) & 0xF); 
	putHex(ch & 0xF); 
}

void
RTFwfile::putField(char *fname, char *ftext, bool fmerge,
                   char *foptions, char *fresult, bool wrap, bool lock)
{
	putGS();
	putCW("field");
	if (lock)
		putCW("fldlock ");
	putGS();
	putCS('*');
	putCW("fldinst ");	 // field codes
	assert(fname);
	putStr(fname);			 // field name, always present
	if (ftext) {
		putStr(" \"\\f1 ");  // use default font to ensure correct encoding
		putStr(ftext);
		putStr("\" ");
	}
	if (fmerge)
		putStr("\\\\* MERGEFORMAT ");
	if (foptions)
		putStr(foptions);
	putGE();
	putGS();
	putCW("fldrslt");	   // result field
	if (fresult) {
		putGS();
		putStr(fresult);
		putGE();
	}
	if (wrap) {
		putGE();
		putGE();
	}
}




// metafile functions


void
RTFwfile::startMeta(char mt, int namewid, int numwid)
{
	if ((Meta = mt) == 'E') { // open new extern metafile if mt == 'E'
		if (namewid > 0)
			metanamewid = namewid;
		if (numwid > 0)
			metanumwid = numwid;
		sprintf(metaname, "%s%.*s%0.*d.wmf",
		           basepath, metanamewid, basename, metanumwid, ++metanum);
		mfile = new DCLwfile(metaname, fbin);
	}
	else Meta = 'I';	
}


void
RTFwfile::endMeta()
{
	if (Meta == 'E') {
		Meta = 'I';
		mfile->CloseFile();  // close the metafile
	}
}


void 
RTFwfile::putMB(int ch)
{
	if (Meta == 'E') {
		mfile->putChar(ch);
		return;
	}
	putHexB(ch);
	CheckHot();
}


void 
RTFwfile::putMS(short s)
{
	if (Meta == 'E') {
		mfile->putBShort(s);
		return;
	}
	putHexB(s);
	putHexB(s >> 8);
	CheckHot();
	putChar(' ');
}


void 
RTFwfile::putML(long l)
{
	if (Meta == 'E') {
		mfile->putBLong(l);
		return;
	}
	putHexB(l);
	putHexB(l >> 8);
	putHexB(l >> 16);
	putHexB(l >> 24);
  CheckHot();
	putChar(' ');
}



// metafile diversion functions

long
RTFwfile::NewMDiv(void)
{
	if (Meta == 'E')
		return mfile->NewDiv();
  return NewDiv();
}


void
RTFwfile::SelectMDiv(long div)
{
  if (Meta == 'E')
    mfile->SelectDiv(div);
  else
    SelectDiv(div);
}


void
RTFwfile::WriteMDiv(long div)
{
  if (Meta == 'E')
    mfile->WriteDiv(div);
  else
    WriteDiv(div);
}


void
RTFwfile::DeleteMDiv(long div)
{
  if (Meta == 'E')
    mfile->DeleteDiv(div);
  else
    DeleteDiv(div);
}



// end of dwrtffl.cpp

