
// dcfilw.cpp contains code for the dcl file write classes, JHG
// Copyright (c) 1995-2007 by Omni Systems, Inc.  All rights reserved.

#include "dcglob.h"
#include "dcbase.h"
#include "dcnode.h"
#include "dclist.h"
#include "dcfile.h"
#include "dcfilw.h"

#pragma warning( disable : 4244 4514 )


// DCwfile is the general-purpose file writing class

char *DCwfile::ErrMsg[] = {
  "creating", "", "", "writing"
};

dc_err DCwfile::ErrTypes[] = {
  dc_err_nowrite, dc_ok, dc_ok, dc_err_write
};


DCwfile::DCwfile(const char *fname, dc_ftyp ft, dc_fmod fm) : DCfile(fname, fm)
{
	fp = fptr;
	Fr = Ferr;
  ftype = ft;
	aux = NULL;

	wfname = NewName(fname);

  if (Ferr)
    return;

  CurrDiv = NULL;
  LastDiv = 0;

	auxnum = 1;
	auxnamewid = 5;
	auxnumwid = 3;
}

DCwfile::~DCwfile()
{
	DeleteName(wfname);
}

void
DCwfile::ReopenFile(const char *fn, bool close)
{
	if (close)
		CloseFile();
	DCfile::ReopenFile(fn, close);
	fp = fptr;
	Fr = Ferr;
	wfname = NewName(fn);
}

void
DCwfile::CloseFile()
{
	DCfile::CloseFile();
	if (wfname) {
		DeleteName(wfname);
		wfname = NULL;
	}
	fp = NULL;
}


short
DCwfile::StartAux(char* ext, short num, int namewid, int numwid, long subnum)
{
	if (num > 0)
		auxnum = num;
	if (namewid > 0)
		auxnamewid = namewid;
	if (numwid > 0)
		auxnumwid = numwid;

	if (subnum > 0)
		sprintf(auxname, "%s%.*s%0.*dX%d.%.3s",
		        basepath, auxnamewid, basename, auxnumwid, auxnum, subnum, ext);
	else
		sprintf(auxname, "%s%.*s%0.*d.%.3s",
		        basepath, auxnamewid, basename, auxnumwid, auxnum, ext);

	if (!StartAuxFile(auxname, false))
		return 0;
	return auxnum;
}


bool
DCwfile::StartAuxFile(char *aname, bool addpath)
{
	if (addpath
	 && (aname != auxname)) {
		strcpy(auxname, basepath);
		strcat(auxname, aname);
	}

	aux = new DCfile(auxname, mwr);
	if (aux == NULL)
		return false;
	fp = aux->fptr;
	Fr = aux->FErr();
	return true;
}


short
DCwfile::EndAux()
{
	if (aux == NULL)
		return 0;
	EndAuxFile();
	return auxnum++;
}


void
DCwfile::EndAuxFile()
{
	if (aux == NULL)
		return;
	aux->CloseFile();
	delete aux;
	aux = NULL;
	fp = fptr;
	Fr = Ferr;
}



void
DCwfile::nextFile(void)
{
  if (!fname)  // in a pipe, no dividing
    return;
 
	if (aux == NULL)
		fp = NULL;

  if (fptr)
    fclose(fptr);

  char* pch = strrchr(fname, '.');
  int i = (pch - fname) - 1;
  
  while (--i > 0) {
    --pch;
    if (!isdigit(*pch))  // **err** out of digits
      break;
    if (*pch < '9') {
      *pch++;
      break;
    }
    *pch = '0'; // and increment digit to the left
  }

  if ((fptr = fopen(fname, "wb")) == NULL)
    Ferr = fno;
  else Ferr = fok;

	if (aux == NULL) {
		fp = fptr;
		Fr = FErr();
	}
}


void
DCwfile::Err()
{
  if (Fr == fok)
    return;

  fprintf(stderr, "Writefile %s: error %s.\n",
    aux ? auxname : (fname ? fname : "stdin"),
	  ErrMsg[(int) Fr - 1]);
  exit( (int) ErrTypes[(int) Fr - 1]);
}


int 
DCwfile::putChar(int ch)
{
  char c = ch & 0xFF;
	//if ((ftype == fasc)
	// && (ch == '\0'))
	//	return 0;

  if (CurrDiv) {  // write to diversion
		CurrDiv->Write(&c, 1);
    return ch;
  }
	Fr = aux ? aux->FErr() : Ferr;
  return (Fr ? EOF : fputc(c, fp));
}


int 
DCwfile::putCr(void)
{
	putChar('\r');
	return putChar('\n');
}

int  
DCwfile::putHex(int ch)
{
  return putChar((ch < 10) ? (ch + '0') : ((ch - 10) + 'A'));
}


int  
DCwfile::putNum(long n)
{
	char chp[13];
	_ltoa(n, chp, 10);
  return putStr(chp);
}


int  
DCwfile::putStr(const char *s, size_t len)
{
	if (s == NULL)
		return 0;

  if (len == 0)
		len = strlen(s);

	if (len == 0)
		return 0;

  if (CurrDiv) {  // write to diversion
		CurrDiv->Write(s, len);
    return len;
  }

	Fr = aux ? aux->FErr() : Ferr;
	if (Fr)
		return EOF;

	if (len < strlen(s)) {
		while (len > 1) {
			if (putc(*s++, fp) == EOF)
				return EOF;
			else len--;
		}
		return putc(*s, fp);
	}		
  else return fputs(s, fp);
}


int  
DCwfile::putQStr(const char *s, size_t len, int quo)
{
	if (s == NULL)
		return 0;

  if (len == 0)
		len = strlen(s);

	char *n = NewName(len * 2);
	char *t = n;
	bool esc = false;
	while (*s) {
		if (*s == '\\') {
			if (esc) {
				*t++ = '\\';
				*t++ = '\\';
				esc = false;
			}
			else
				esc = true;
		}
		else if (*s == quo) {
			if (esc)
				esc = false;
			else
				len++;
			*t++ = '\\';
			*t++ = quo;
		}
		else {
			if (esc) {
				*t++ = '\\';
				esc = false;
			}
			*t++ = *s;
		}
		s++;
	}
	*t = '\0';

	int ret = putStr(n, len);
	DeleteName(n);
	return ret;
}


void  
DCwfile::putBlock(const void *v, size_t s)
{
  if (CurrDiv) {  // write to diversion
		CurrDiv->Write((const char *) v, s);
    return;
  }
	Fr = aux ? aux->FErr() : Ferr;
  if (Fr)
    return;
  if (fwrite(v, 1, s, fp) != s) {
    Fr = fwr;
    Err();
  }
}


int
DCwfile::seek(long pos, int mode)
{
	if (Fr)
		return -1;
	return fseek(fp, pos, mode);
}

long 
DCwfile::tell()
{
	if (Fr)
		return -1;
	return ftell(fp);
}

long
DCwfile::getSize(void)
{
	if (Fr)
		return 0L;

	long pos = ftell(fp);
	if (pos == -1)
		return 0L;

	if (fseek(fp, 0L, SEEK_END) != 0)
		return 0L;

	long epos = ftell(fp);
	if (epos != pos)
		fseek(fp, pos, SEEK_SET);

	return epos;
}



// numeric writing

int  
DCwfile::putIShort(short i)
{ 
	putChar( (int) i); 
	return putChar( (int) i >> 8); 
}

int  
DCwfile::putILong(long l)
{ 
	putIShort( (short) (l & 0xFFFF)); 
	return putIShort( (short) (l >> 16)); 
}

int  
DCwfile::putIULong(unl l)
{ 
	putIShort( (short) (l & 0xFFFF)); 
	return putIShort( (short) (l >> 16)); 
}

int
DCwfile::putIDouble(double d)
{
  union {
    double d2;
    unl l[2];
  } tmp;
 
  tmp.d2 = d;
  putIULong(tmp.l[dc_lsb_index]);
  return putIULong(tmp.l[dc_msb_index]);
}


int  
DCwfile::putMShort(short i)
{ 
	putChar( (int) i >> 8); 
	return putChar( (int) i); 
}

int  
DCwfile::putMLong(long l)
{ 
	putMShort( (short) (l >> 16)); 
	return putMShort( (short) (l & 0xFFFF)); 
}

int  
DCwfile::putMULong(unl l)
{ 
	putMShort( (short) (l >> 16)); 
	return putMShort( (short) (l & 0xFFFF)); 
}

int
DCwfile::putMDouble(double d)
{
  union {
    double d2;
    unl l[2];
  } tmp;
 
  tmp.d2 = d;
  putMULong(tmp.l[dc_msb_index]);
  return putMULong(tmp.l[dc_lsb_index]);
}


int  
DCwfile::putPLong(long l)
{ 
	putIShort( (short) (l >> 16)); 
	return putIShort( (short) (l & 0xFFFF)); 
}

int  
DCwfile::putPULong(unl l)
{ 
	putIShort( (short) (l >> 16)); 
	return putIShort( (short) (l & 0xFFFF)); 
}



// diversions

long  
DCwfile::NewDiv()
{
  CurrDiv = new DCdiv;
  DivList.add(CurrDiv, ++LastDiv);
  return LastDiv;
}


void  
DCwfile::SelectDiv(long div)
{
  DCdiv *p;

	if (div == 0)
    CurrDiv = NULL;
  else if ((p = (DCdiv *) DivList.find(div)) != NULL)
    CurrDiv = p;
}


void  
DCwfile::WriteDiv(long div)
{
  if (Ferr)
    return;

	DCdiv *dest = CurrDiv;

  if (div != 0)
    SelectDiv(div);

  if (!CurrDiv)
    return;

	if (dest == CurrDiv)
		dest = NULL;

  size_t s = CurrDiv->Used();

  if (dest) {  // write to current diversion
		dest->Write(CurrDiv->Read(), s);
		CurrDiv = dest;
    return;
  }


	Fr = aux ? aux->FErr() : Ferr;
  if (Fr)
    return;
  if (fwrite(CurrDiv->Read(), 1, s, fp) != s)
    Fr = fwr;
}


void  
DCwfile::DeleteDiv(long div)
{
  if (div != 0)
    SelectDiv(div);

  if (div == 0)
    div = DivList(CurrDiv);

	if (div == LastDiv) {
		LastDiv--;
		DivList.remove(div);
	}
	else
		DivList.replace(NULL, div);
  delete CurrDiv;
  CurrDiv = NULL;
}



// DCdiv handles diversions for DCwfile

const size_t DCdiv::alsize = 1024;

void
DCdiv::Write(const char *p, size_t s)
{
	if (p == NULL)
	 //|| (*p == '\0'))
		return;

  if ((s > avail)		 // avail will be 0 initially
	 || (s > (avail - 1))) { // alloc at first use
    while ((s > avail) || (s > (avail - 1)))	{
      avail += alsize;	// allow for terminating null
      len += alsize;
    }
    if (start)
      start = (char *) ResizeMem(start, len);
    else
      start = (char *) NewMem(len);
    ptr = start + (len - avail);
  }

	if (s == 1) {
    *ptr++ = *p;
    avail--;
  }
  else {
    memcpy(ptr, p, s);
    ptr += s;
    avail -= s;
  }
	*ptr = '\0';  // keep null terminated
}

void
DCdiv::Clear(void)
{
	avail = len;
	ptr = start;
	if (ptr)
		*ptr = '\0';
}


// DCLwfile is the class for writing DCL ctls


DCLwfile::DCLwfile(char *fname, dc_ftyp ft) : DCwfile(fname, ft)
{
  if (Ferr)
    return;
  PutDclHeader();
}


void
DCLwfile::CloseFile()
{
  PutDclFooter();
	DCwfile::CloseFile();
}


void
DCLwfile::ReopenFile(const char *fn)
{
	CloseFile();
	DCwfile::ReopenFile(fn);
  PutDclHeader();
}


void
DCLwfile::nextFile(void)
{
  if (!fname)  // in a pipe, no dividing
    return;
 
  DCwfile::nextFile();

  PutDclHeader();
}


void
DCLwfile::PutDclHeader(void)
{
  switch (ftype) {
    case fasc:
      putStr("(DCL 100)\n");
		case ftxt:
      putShort = putAShort;
      putLong = putALong;
      putULong = putAULong;
      return;
    case fxml:
			putStr("<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n");
			putStr("<!DOCTYPE xdcl PUBLIC \"-//OMSYS//DTD XDCL 1.0//EN\"\n");
			putStr("  \"http://www.omsys.com/dcl/dtd/xdcl.dtd\">\n");
      putStr("<xdcl ver=\"1.0\">\n");
      putShort = putAShort;
      putLong = putALong;
      putULong = putAULong;
      return;
    case fmsb:
      putMLong(0xA5C3C3A5L);
      putMLong(0L);
      putShort = putMShort;
      putLong = putMLong;
      putULong = putMULong;
      putDouble = putMDouble;
      return;
    case flsb:
      putMLong(0xA5C3C3A5L);
      putMLong(0x81818181L);
		case fbin:
      putShort = putIShort;
      putLong = putILong;
      putULong = putIULong;
      putDouble = putIDouble;
      return;
    case fpdp:
      putMLong(0xA5C3C3A5L);
      putMLong(0x42424242L);
      putShort = putIShort;
      putLong = putPLong;
      putULong = putPULong;
      putDouble = putIDouble;
      return;
    default:
      break;
  }
  Ferr = fform;
  CloseFile();
}


void
DCLwfile::PutDclFooter(void)
{
  if (ftype == fxml)
		putStr("</xdcl>\n");
}



// DCctl write function

void
DCctl::write(DCLwfile& wf, long lev)
{
	static bool parend = true;
	static bool tblend = false;
	static bool inftn = false;

	if (lev > 32)
		return;
  if (major) {
		if (major == 2) {
			if (minor == 50) {  // table
				if (dtype == group) {
					tblend = parend;
					parend = true;
				}
				else if (dtype == endgroup)
					parend = tblend;
			}
			else if (minor == 71) {  // footnote
				if (dtype == group)
					inftn = true;
				else if (dtype == endgroup)
					inftn = false;
			}
			else if (!inftn       // which has its own para start/end
			 && (minor == 11)) {  // para break
				if (data == 2) {
					if (parend)
						return;
					parend = true;
				}
				else if (data == 1) {
					if (!parend)
						wf.putCtl(*(new DCctl(imnem, 2, 11, 2, 0)));
					parend = false;
				}
			}
		}
	  wf.putCtl(*this);
	}
}

 

// functions for writing ASCII DCL


int
DCLwfile::putAShort(short s)
{
  char buf[8];

  sprintf(buf, " %hd", s);
  return putStr(buf);
}

int
DCLwfile::putALong(long l)
{
  char buf[12];

  sprintf(buf, " %ld", l);
  return putStr(buf);
}

int
DCLwfile::putAULong(unl l)
{
  char buf[12];

  sprintf(buf, " %lu", l);
  return putStr(buf);
}



// functions for writing external ASCII data


void
DCLwfile::putExtBytes(DCctl& c)
{
  unl i;
  unl count = c.siz();
  unc *u = (unc *) c.ex();

  for (i = 0; i < count; i++) {
    if ((i % 32 == 0) && (count > 24))
      putChar('\n');
    else if (i % 4 == 0)
      putChar(' ');
    putHex((*u >> 4) & 0xF);
    putHex(*u++ & 0xF);
  }
}

void
DCLwfile::putExtShorts(DCctl& c)
{
  unl i;
  unc per_line = 10;
  unl count = c.siz() / sizeof(short);
  short *s = (short *) c.ex();

  for (i = 0; i < count; i++) {
    if ((i % per_line == 0) && (count > 8))
      putChar('\n');
    putAShort(*s++);
  }
}

void
DCLwfile::putExtLongs(DCctl& c)
{
  unl i;
  unc per_line = 10;
  unl count = c.siz() / sizeof(long);
  long *l = (long *) c.ex();

  for (i = 0; i < count; i++) {
    if ((i % per_line == 0) && (count > 8))
      putChar('\n');
    putALong(*l++);
  }
}

void
DCLwfile::putExtDoubs(DCctl& c)
{
  unl i;
  unc per_line = 10;
  unl count = c.siz() / sizeof(double);
  double *d = (double *) c.ex();
  char buf[20];

  for (i = 0; i < count; i++) {
    if ((i % per_line == 0) && (count > 8))
      putChar('\n');
    sprintf(buf, " %g", *d++);
    putStr(buf);
  }
}

void
DCLwfile::putExtStr(DCctl& c)
{
  unc *p = (unc *) c.ex();
  unl lim = c.siz();
  unl i = 0;
  unc ch;

  while (i++ < lim) {
    ch = *p++;
    switch (ch) {
      case '\0':  // ignore terminating 0, should be last char
       continue;
      case '"':   // put backslash before quotes
      case '\'':
      case '\\':  // and before literal backslash
       putChar('\\');
       break;
      case '(':   // balance left parens by adding "\)"
       putChar('(');
       putChar('\\');
       ch = ')';
       break;
      case ')':   // balance right parens by inserting "\("
       putChar('\\');
       putChar('(');
       break;
      default:    // leave others, including \n and \r, alone
       break;
    }
#if 0
    if (ch > 126) {
      putChar('\\');
      putChar((ch / 100) + '0');
      ch %= 100;
      putChar((ch / 10) + '0');
      ch %= 10;
      putChar(ch + '0');
      ch = ' ';
    }
#endif
    putChar(ch);
  }
}

void
DCLwfile::putExtCode(DCctl& c)
{
  putChar('\n');
  putBlock(c.ex(), c.siz() - 1); // trim off the ending 0 byte
}



// functions for writing XDCL


void
DCLwfile::putExtXStr(DCctl& c)
{
  unc *p = (unc *) c.ex();
  unl lim = c.siz();
  unl i = 0;
  unc ch;

  while (i++ < lim) {
    ch = *p++;
    switch (ch) {
      case '\0':  // ignore terminating 0, should be last char
				continue;
      case '<':   // make <&> into entities
				putStr("&lt;");
				break;
      case '&':
				putStr("&amp;");
				break;
      case '>':
				putStr("&gt;");
				break;
      default:    // leave others, including \n and \r, alone
				putChar(ch);
				break;
    }
  }
}

void
DCLwfile::putExtXCode(DCctl& c)
{
  putStr("\n<![CDATA[\n");
  putBlock(c.ex(), c.siz() - 1); // trim off the ending 0 byte
	putStr("\n]]>");
}



// functions for writing ctls


DCctl&
DCLwfile::putACtl(DCctl& c)
{
  //unc tok = 0;
  char buf[TokenMax + 1];
  short i;
  static short level = 0;
  unl ulval;
  time_t val, val2;
  char *timeptr;

  if (c.dt() == group) {
    for (i = 0; i < level; i++)
      putChar(' ');
    putChar('(');
    level++;
  }
  else if (c.dt() == endgroup) {
    level--;
    for (i = 0; i < level; i++)
      putChar(' ');
    putChar(')');
    putChar('\n');
    return c;
  }
  else {
    for (i = 0; i < level; i++)
      putChar(' ');
  }

  putChar('(');
  if (c.min() & 0x80)
    putChar('*');  // marker for user-defined ctl

  sprintf(buf, "%s", c.getMajor());
  putStr(buf);
  sprintf(buf, " %s", c.getMinor());
  putStr(buf);
  sprintf(buf, " %s", c.getData(c.dat()));
  putStr(buf);

  switch (c.dt()) {
    case ibyte:
      ulval = c.siz();
      sprintf(buf, " =X %0.2lX %0.2lX %0.2lX %0.2lX)",
        (ulval >> 24) & 0xFF, (ulval >> 16) & 0xFF,
        (ulval >> 8) & 0xFF, ulval & 0xFF);
      putStr(buf);
      break;
    case ishort:
      ulval = c.siz();
      sprintf(buf, " =H %ld %ld)",
        (ulval >> 16) & 0xFFFF, ulval & 0xFFFF);
      putStr(buf);
      break;
    case ilong:
      sprintf(buf, " = %ld)", c.siz());
      putStr(buf);
      break;
    case imnem:
      sprintf(buf, " %s)", c.getSize(c.siz()));
      putStr(buf);
      break;
    case stamp:
      val = val2 = (time_t) c.siz();
#ifdef DOSTIME
      if (val < 2209104000L)
        val2 += 2209104000L;
#else
      if (val > 2209104000L)
        val2 -= 2209104000L;
#endif
      timeptr = ctime(&val2);
      sprintf(buf, " : %lu) #%.24s", val, timeptr);
      putStr(buf);
      break;
    case ebyte:
      putChar(' ');
      putChar('+');
      putChar('X');
      putExtBytes(c);
      putChar(')');
      break;
    case eshort:
      putChar(' ');
      putChar('+');
      putChar('H');
      putExtShorts(c);
      putChar(')');
      break;
    case elong:
      putChar(' ');
      putChar('+');
      putExtLongs(c);
      putChar(')');
      break;
    case edoub:
      putChar(' ');
      putChar('+');
      putChar('D');
      putExtDoubs(c);
      putChar(')');
      break;
    case ename:
      putChar(' ');
      putChar('\'');
      putExtStr(c);
      putChar('\'');
      putChar(')');
      break;
    case etext:
      putChar(' ');
      putChar('"');
      putExtStr(c);
      putChar('"');
      putChar(')');
      break;
    case ecode:
      putChar(' ');
      sprintf(buf, "%%%ld", c.siz());
      putStr(buf);  // starting ID
      putExtCode(c);
      putStr(buf);  // ending ID
      putChar(')');
      break;
    case group:
    default:
      putChar(')');
      break;
  }

  putChar('\n');
  return c;
}

DCctl&
DCLwfile::putBCtl(DCctl& c)
{
  putChar((c.dt() << 4) + (c.maj() & 0xF));
  putChar(c.min());
  (this->*putShort)(c.dat());

  switch (c.dt()) {
    case stamp:
    case ibyte:
      (this->*putULong)(c.siz());
      break;
    case etext:
    case ename:
    case ecode:
			if (c.ex() && c.siz() && (((char *) c.ex())[c.siz() - 1] != '\0')) {
	      (this->*putLong)(c.siz() + 1); // make room to null-terminate	it
				break;
			}
    default:
      (this->*putLong)(c.siz());
      break;
  }

  if (c.ex() && c.siz()) {   // add the ext data
    size_t i, lim;

    switch(c.dt()) {
      case eshort:
        if (dc_mach != ftype) {
          short *s = (short *) c.ex();
          lim = c.siz() / sizeof(short);
          for (i = 0; i < lim; i++)
            (this->*putShort)(*s++);
          break;
        }  // fall through
      case elong:
        if (dc_mach != ftype) {
          long *l = (long *) c.ex();
          lim = c.siz() / sizeof(long);
          for (i = 0; i < lim; i++)
            (this->*putLong)(*l++);
          break;
        }  // fall through
      case edoub:
        if (dc_mach != ftype) {
          double *d = (double *) c.ex();
          lim = c.siz() / sizeof(double);
          for (i = 0; i < lim; i++)
            (this->*putDouble)(*d++);
          break;
        }  // fall through
      case ebyte:
        putBlock(c.ex(), c.siz());
        break;

      case etext:
      case ename:
      case ecode:
        putBlock(c.ex(), c.siz());
				if (((char *) c.ex())[c.siz() - 1] != '\0')
					putChar('\0');  // add null terminator
        break;
      default:
        break;
    }
  }

  return c;
}


DCctl&
DCLwfile::putXCtl(DCctl& c)
{
  //unc tok = 0;
  char buf[TokenMax + 1];
  char tag[TokenMax + 1];
	char *sval = NULL;
	uns aval = 0;
	attrs *ap = NULL;
	char *vp = NULL;
	char *sv = NULL;
  short i;
	bool att = false;
  static short level = 0;
  unl ulval;
  time_t val, val2;
  char *timeptr;

  if (c.dt() == group) {
    for (i = 0; i < level; i++)
      putChar(' ');
    putChar('<');
    level++;
  }
  else if (c.dt() == endgroup) {
    level--;
    for (i = 0; i < level; i++)
      putChar(' ');
    putChar('<');
    putChar('/');
  }
  else {
    for (i = 0; i < level; i++)
      putChar(' ');
		putChar('<');
  }

  sprintf(tag, "%s%1.1s%s", (c.min() & 0x80) ? "x" : "" ,
		                        c.getMajor(), c.getMinor());
	sval = c.getData(c.dat());

	ap = DCnode::GetAttrs(c.maj(), c.min(), c.dat(), (uns) c.siz());
	aval = ap ? ap->dattr : 0;

	if (!aval
	 && !isdigit(*sval)) {
		strcat(tag, "-");
		strcat(tag, sval);
	}
	else {
		att = true;
		sprintf(buf, " %s=\"%s\"",
		             (aval ? c.getData(aval, 4, 1) : "type"), sval);
	}

  putStr(tag);

	if (c.dt() == group) {
		//putStr(" dt=\"group\"");
	}
	else if (c.dt() == endgroup) {
		putStr(">\n");
		return c;
	}

	if (att)
	  putStr(buf);

	aval = (ap ? ap->sattr : 0);
	vp = (aval ? c.getData(aval, 4, 1) : "val");

  switch (c.dt()) {
    case ibyte:
			putStr(" dt=\"ibyte\"");
      ulval = c.siz();
      sprintf(buf, " %s=\"%0.2lX %0.2lX %0.2lX %0.2lX\"/>", vp,
        (ulval >> 24) & 0xFF, (ulval >> 16) & 0xFF,
        (ulval >> 8) & 0xFF, ulval & 0xFF);
      putStr(buf);
      break;
    case ishort:
			putStr(" dt=\"ishort\"");
      ulval = c.siz();
      sprintf(buf, " %s=\"%ld %ld\"/>", vp,
        (ulval >> 16) & 0xFFFF, ulval & 0xFFFF);
      putStr(buf);
      break;
    case ilong:
			putStr(" dt=\"ilong\"");
      sprintf(buf, " %s=\"%ld\"/>", vp, c.siz());
      putStr(buf);
      break;
    case imnem:
			//putStr(" dt=\"imnem\"");
			if (*(sv = c.getSize(c.siz())) == '\0')
				sv = "0";
			sprintf(buf, " %s=\"%s\"/>", vp, sv);
			putStr(buf);
      break;
    case stamp:
			putStr(" dt=\"stamp\"");
      val = val2 = (time_t) c.siz();
#ifdef DOSTIME
      if (val < 2209104000L)
        val2 += 2209104000L;
#else
      if (val > 2209104000L)
        val2 -= 2209104000L;
#endif
      timeptr = ctime(&val2);
      sprintf(buf, " %s=\"%lu\"/>\n", vp, val);
      putStr(buf);
	    for (i = 0; i < level; i++)
		    putChar(' ');
			sprintf(buf, "<!-- %.24s -->", timeptr);
      putStr(buf);
      break;
    case ebyte:
			putStr(" dt=\"ebyte\"");
      sprintf(buf, " %s=\"%ld\">", vp, c.siz());
      putStr(buf);
      putExtBytes(c);
			goto etag;
    case eshort:
			putStr(" dt=\"eshort\"");
      sprintf(buf, " %s=\"%ld\">", vp, c.siz() / sizeof(short));
      putStr(buf);
      putExtShorts(c);
			goto etag;
    case elong:
			putStr(" dt=\"elong\"");
      sprintf(buf, " %s=\"%ld\">", vp, c.siz() / sizeof(long));
      putStr(buf);
      putExtLongs(c);
			goto etag;
    case edoub:
			putStr(" dt=\"edoub\"");
      putStr(buf);
      sprintf(buf, " %s=\"%ld\">", vp, c.siz() / sizeof(double));
      putExtDoubs(c);
			goto etag;
    case ename:
			putStr(" dt=\"ename\"");
			if (!c.siz()
			 || (*((char *) c.ex()) == '\0'))
				putStr("/>");
			else {
	      sprintf(buf, " %s=\"%ld\">", vp, c.siz());
		    putStr(buf);
			  putExtXStr(c);
				goto etag2;
			}
      break;
    case etext:
			putStr(" dt=\"etext\"");
			if (!c.siz()
			 || (*((char *) c.ex()) == '\0'))
				putStr("/>");
			else {
				sprintf(buf, " %s=\"%ld\">", vp, c.siz());
				putStr(buf);
				putExtXStr(c);
				goto etag;
			}
      break;
    case ecode:
			putStr(" dt=\"ecode\"");
      sprintf(buf, " %s=\"%ld\">\n", vp, c.siz());
      putStr(buf);
      putExtXCode(c);
			goto etag;
    case group:
    default:
      putChar('>');
      break;
		etag:
      putChar('\n');
	    for (i = 0; i < level; i++)
		    putChar(' ');
		etag2:
      putStr("</");
      putStr(tag);
      putChar('>');
      break;
  }

  putChar('\n');
  return c;
}

// end of dcfilw.cc

