
// dcfilr.cc contains code for the dcl file read classes, JHG
// Copyright (c) 1993-1996 by Omni Systems, Inc.  All rights reserved.

#include "dcglob.h"
#include "dcbase.h"
#include "dcnode.h"
#include "dcfile.h"
#include "dcfilr.h"

#pragma warning( disable : 4244 4514 4710)

char *DCrfile::ErrMsg[] = {
  "opening", "not DCL format", "reading"
};

dc_err DCrfile::ErrTypes[] = {
  dc_err_noread, dc_err_badform, dc_err_read
};

static int FirstCh;


// DCL file read initialization

DCLrbfile::DCLrbfile(const char *fname) : DCrfile(fname)
{
  int ch;

  if (Ferr)
    return;

  if ((ch = getChar()) != 0xA5) {
    FirstCh = ch;
    ftype = fasc;  // hopefully
    Ferr = fform;  // to be checked if called from DCLrfile
    return;
  }

  if ((getChar() == 0xC3)
   && (getChar() == 0xC3)
   && (getChar() == 0xA5)
   && ((ch = getChar()) == getChar())
   && (ch == getChar())
   && (ch == getChar())) {
    switch (ch) {
      case 0:
        ftype = fmsb;
        getShort = getMShort;
        getLong = getMLong;
        getULong = getMULong;
        getDouble = getMDouble;
        return;
      case 0x81:
        ftype = flsb;
        getShort = getIShort;
        getLong = getILong;
        getULong = getIULong;
        getDouble = getIDouble;
        return;
      case 0x42:
        ftype = fpdp;
        getShort = getIShort;
        getLong = getPLong;
        getULong = getPULong;
        getDouble = getIDouble;
        return;
      default:
        break;
    }
  }

  Ferr = fform; // defective binary
}


DCLrfile::DCLrfile(const char *fname) : DCLrbfile(fname)
{
  if ((Ferr != fform) || (ftype != fasc))
    return; // bad file or good binary

  if (FirstCh != '(')
    return; // not ASCII either, leave err

  Ferr = fok; // clear the format err

  if ((getChar() == 'D')
   && (getChar() == 'C')
   && (getChar() == 'L')
   && (getAShort() <= 100)
   && (getChar() == ')')) {
    ftype = fasc;
    getShort = getAShort;
    getLong = getALong;
    getULong = getAULong;
    return;
  }

  Ferr = fform; // defective ASCII
}


bool
DCrfile::FileExists(char *fname)
{
	if (!fname || (*fname == '\0'))
		return false;

	DCrfile *rf = new DCrfile(fname);
	if (rf->FErr() != fok)
		return false;

  rf->CloseFile();
	delete rf;

	return true;
}



// common file read functions, DCL or non-DCL


bool
DCrfile::getLine(char *p, size_t s)
{
	if (Ferr)
		return false;
	if (fgets(p, s, fptr) == NULL)
		return false;
	return true;
}


void
DCrfile::skipLine()
{
	int ch;

	while (((ch = getChar()) != EOF) && (ch != '\n')) {
		if (ch == '\r')	{
			if ((ch = getChar()) != '\n')
				ungetChar(ch);
			break;
		}
	}
}

void
DCrfile::Err()
{
  if (Ferr == fok)
    return;
  fprintf(stderr, "Readfile %s: error %s.\n",
    fname ? fname : "stdin", ErrMsg[((int) Ferr) - 1]);
  CloseFile();
  exit( (int) ErrTypes[Ferr - 1]);
}


DCblock&
DCblock::operator= (DCrfile& rf)
{
  rf.getBlock(ptr, len);
  return *this;
}



// binary file reading functions

short
DCrfile::getIShort(void)
{
	short tmp = (short) getChar();
  tmp |= (short) (getChar() << 8);
	return tmp;
}


long
DCrfile::getILong(void)
{
	long tmp = ((long) getIShort()) & 0xFFFF;
	tmp |= ((long) getIShort()) << 16; 
	return tmp; 
}


unl
DCrfile::getIULong(void)
{
	unl tmp = ((unl) getIShort()) & 0xFFFF;
	tmp |= ((unl) getIShort()) << 16; 
	return tmp; 
}


short
DCrfile::getMShort(void)
{
	short tmp = (short) (getChar() << 8);
	tmp |= (short) getChar(); 
	return tmp;
}


long
DCrfile::getMLong(void)
{
	long tmp = ((long) getMShort()) << 16;
	tmp |= ((long) getMShort()) & 0xFFFF;
	return tmp;
}


unl
DCrfile::getMULong(void)
{
	unl tmp = ((unl) getMShort()) << 16;
	tmp |= ((unl) getMShort()) & 0xFFFF; 
	return tmp; 
}


double
DCrfile::getIDouble()
{
  union {
    double d;
    unl l[2];
  } tmp;

  tmp.l[dc_lsb_index] = getIULong();
  tmp.l[dc_msb_index] = getIULong();
  return tmp.d;
}


double
DCrfile::getMDouble()
{
  union {
    double d;
    unl l[2];
  } tmp;

  tmp.l[dc_msb_index] = getMULong();
  tmp.l[dc_lsb_index] = getMULong();
  return tmp.d;
}


long
DCrfile::getPLong(void)
{
	long tmp = ((long) getIShort()) << 16;
	tmp |= ((long) getIShort()) & 0xFFFF; 
	return tmp; 
}


unl
DCrfile::getPULong(void)
{
	unl tmp = ((unl) getIShort()) << 16;
	tmp |= ((unl) getIShort()) & 0xFFFF;
	return tmp;
}



// ASCII file reading functions


long
DCrfile::getALong()
{
  int ch;
  unc digs = 0;
  int sign = 1;
  long val = 0;

  while ((ch = getChar()) != EOF) {
    if (isdigit(ch)) {
      digs++;
      val = (val *10) + (ch - '0');
    }
    else if (ch == '-')
      sign = -1;
    else if (!isspace(ch)) {
      ungetChar(ch);
      break;
    }
    else if (digs)
      break;
  }
  return (val * sign);
}


unl
DCrfile::getAULong()
{
  int ch;
  unc digs = 0;
  long val = 0;

  while ((ch = getChar()) != EOF) {
    if (isdigit(ch)) {
      digs++;
      val = (val *10) + (ch - '0');
    }
    else if (!isspace(ch)) {
      ungetChar(ch);
      break;
    }
    else if (digs)
      break;
  }
  return val;
}


unl
DCrfile::getABytes()
{
  int ch;
  unc tok = 0;
  unl val = 0;

  while ((ch = getChar()) != EOF) {
    if (isxdigit(ch)) {
      ch -= (ch > '9') ? ((ch > 'F') ? 'a'-10 : 'A'-10) : '0';
      val = (val << 4) | ch;
      if (++tok == BytesMax)
        break;
    }
    else if (!isspace(ch)) {
      ungetChar(ch);
      break;
    }
  }
  return val;
}


double
DCrfile::getADouble()
{
	char buf[100];  // limit imposed by atof()
	unc pos = 0;
  int ch;

  while ((ch = getChar()) != EOF) {
		if (isdigit(ch)
		 || (ch == '.')
		 || (ch == '+')
		 || (ch == '-'))
			buf[pos++] = ch;
    else if (isspace(ch)) {
			if (pos > 0)
				break;
		}
		else {
      ungetChar(ch);
			break;
		}
		if (pos >= 99)
			break;
	}
	buf[pos] = '\0';

	return atof(buf);
}


void
DCrfile::skipWhite()
{
  int ch;

  while ((ch = getChar()) != EOF) {
    if (!isspace(ch)) {
      ungetChar(ch);
      return;
    }
  }
}



// DCL binary file reading function


void
DCLrbfile::skipGroup()
{
  short skiplevel = 1;
  DCctl *cp = new DCctl;
  while (!eof()) {
    getCtl(*cp);
    if (cp->dt() == group)
      skiplevel++;
    else if (cp->dt() == endgroup) {
      if (--skiplevel == 0)
        break;
    }
    else cp->reset();
  }
  delete cp;
}



// ASCII DCL external data reading functions


void
DCLrfile::getExtBytes(DCctl& c)
{
  int ch;
  unc nib = 0;
  unc *cbuf;
  size_t pos = 0;
  size_t maxbuf = 1024;

  if (Ferr)
    return;

  DCblock *b = new DCblock(maxbuf);
  cbuf = (unc *) b->p();

  while ((ch = getChar()) != EOF) {
    if (isxdigit(ch)) {
      ch -= (ch > '9') ? ((ch > 'F') ? 'a'-10 : 'A'-10) : '0';
      if (!nib) {
        cbuf[pos] = ch << 4;
        nib = 1;
      }
      else {
        cbuf[pos++] |= ch;
        nib = 0;
        if (pos == maxbuf)
          cbuf = (unc *) b->set(maxbuf *= 2);
      }
    }
    else if (!isspace(ch)) {
      ungetChar(ch);
      break;
    }
  }
  b->set(pos);
  c = *b;
  delete b;
}


void
DCLrfile::getExtShorts(DCctl& c)
{
  int ch;
  short *sbuf;
  size_t pos = 0;
  size_t maxbuf = 512;

  if (Ferr)
    return;

  DCblock *b = new DCblock(maxbuf * sizeof(short));
  sbuf = (short *) b->p();

  while ((ch = getChar()) != EOF) {
    if ((ch == '-') || isdigit(ch))
      ungetChar(ch);
    else if (isspace(ch))
      continue;
    else {
      ungetChar(ch);
      break;
    }
    sbuf[pos++] = getAShort();
    if (pos == maxbuf)
      sbuf = (short *) b->set((maxbuf *= 2) * sizeof(short));
  }
  b->set(pos * sizeof(short));
  c = *b;
  delete b;
}

void
DCLrfile::getExtLongs(DCctl& c)
{
  int ch;
  long *lbuf;
  size_t pos = 0;
  size_t maxbuf = 256;

  if (Ferr)
    return;

  DCblock *b = new DCblock(maxbuf * sizeof(long));
  lbuf = (long *) b->p();

  while ((ch = getChar()) != EOF) {
    if ((ch == '-') || isdigit(ch))
      ungetChar(ch);
    else if (isspace(ch))
      continue;
    else {
      ungetChar(ch);
      break;
    }
    lbuf[pos++] = getALong();
    if (pos == maxbuf)
      lbuf = (long *) b->set((maxbuf *= 2) * sizeof(long));
  }
  b->set(pos * sizeof(long));
  c = *b;
  delete b;
}

void
DCLrfile::getExtDoubs(DCctl& c)
{
  int ch;
  double *dbuf;
  size_t pos = 0;
  size_t maxbuf = 128;

  if (Ferr)
    return;

  DCblock *b = new DCblock(maxbuf * sizeof(double));
  dbuf = (double *) b->p();

  while ((ch = getChar()) != EOF) {
    if ((ch == '-') || (ch == '+') || isdigit(ch))
      ungetChar(ch);
    else if (isspace(ch))
      continue;
    else break;
    if (fscanf(fptr, "%lg", &dbuf[pos++]) == EOF)
      break;
    if (pos == maxbuf)
      dbuf = (double *) b->set((maxbuf *= 2) * sizeof(double));
  }
  b->set(pos * sizeof(double));
  c = *b;
  delete b;
}

void
DCLrfile::getExtStr(DCctl& c)
{
  int ch, val;
  char *buf;
  size_t pos = 0;
  size_t maxbuf = 1024;
  unc ending = 0;
#if 0
  bool litbs = false;
#endif

  if (Ferr)
    return;

  DCblock *b = new DCblock(maxbuf);
  buf = (char *) b->p();

  while ((ch = getChar()) != EOF) {
    switch (ch) {
      case '"':
        if (c.dt() == etext)
          ending = 1;
        break;
      case '\'':
        if (c.dt() == ename)
          ending = 1;
        break;
      case '\\':
#if 0
        if (litbs) {
          litbs = false;
          break;
        }
#endif
        switch (ch = getChar()) {
          case '\\':  // leave literal backslash
#if 0
            ungetChar(ch);
            litbs = true;
            break;
#endif
          case '"':   // remove backslash from quotes
          case '\'':
            break;
          case '(':   // remove "\(" and "\)" altogether
          case ')':
            continue;
          case '0':   // octal number
            val = 0;
            while ((ch = getChar()) != EOF) {
              if (isdigit(ch)) {
                ch -= '0';
                val = (val << 3) | ch;
              }
              else {
                if (ch != ' ')
                  ungetChar(ch);
                break;
              }
            }
            ch = val;
            break;
          case 'x':   // hex number
          case 'X':
            val = 0;
            while ((ch = getChar()) != EOF) {
              if (isxdigit(ch)) {
                ch -= (ch > '9') ? ((ch > 'F') ? 'a'-10 : 'A'-10) : '0';
                val = (val << 4) | ch;
              }
              else {
                if (ch != ' ')
                  ungetChar(ch);
                break;
              }
            }
            ch = val;
            break;
          default:
            ungetChar(ch);
            if (isdigit(ch))   // decimal number
              ch = (unc) getAULong();
            else ch = '\\';    // leave others, including "\n" and "\r", alone
            break;
        }
        break;
      default:
        break;
    }
    if (ending)
      break;
    buf[pos++] = ch;
    if (pos == maxbuf)
      buf = (char *) b->set(maxbuf *= 2);
  }
  buf[pos++] = '\0';
  b->set(pos);
  c = *b;
  delete b;
}

void
DCLrfile::getExtCode(DCctl& c)
{
  int ch;
  size_t pos = 0;
  size_t maxbuf = 1024;
  long lines = c.dat(); // may be 0 or have rolled over

  if (Ferr)
    return;

  DCblock *b = new DCblock(maxbuf);
  char *buf = (char *) b->p();

	unl len = getAULong(); // eats line ending
	long tfpos = 0;

  while ((ch = getChar()) != EOF) {
    buf[pos++] = ch;
    if ((ch == '\n')
		 && (--lines <= 0)) {  // start checking after count
			ch = getChar();
			if (ch == '%') { // percent in first col may be ending
				tfpos = tell();
				if ((getAULong() == len) // matching number
				 && ((ch = getChar()) == ')')) {
					ungetChar(ch);
					break;
				}
				seek(tfpos);
			}
			else
				ungetChar(ch);
		}
    if (pos == maxbuf)
      buf = (char *) b->set(maxbuf *= 2);
  }
  buf[pos++] = '\0';
  b->set(pos);
  c = *b;
  delete b;
}



// ctl reading functions


DCctl&
DCctl::operator= (DCLrbfile& rf)
{
  return rf.getCtl(*this);
}


DCctl&
DCLrfile::getACtl(DCctl& c)
{
  int ch;
  unc step = 0;
  unc paren = 0;
  unc tok = 0;
  unc ending_tok = 0;
  unc ending_ctl = 0;
  unc user_def = 0;
  char tbuf[TokenMax + 1];
  unl val;

  c.reset();

  if (Ferr)
    return c;
  c.dt(imnem); // default to mnemonic

  while ((ch = getChar()) != EOF) {
    switch (ch) {
      case '(':
        if (paren++)
          c.dt(group);
        break;
      case ')':
        if (!paren) {
          c.dt(endgroup);
          popGroup(c);
          return c;
        }
        ending_ctl = 1;
        if (tok)
          ending_tok = 1;
        break;
      case '*':
        user_def = 1;
        break;
      case '#':
        if (tok)
          ending_tok = 1;
        skipLine();
        break;
      case '+':
        if (tok)
          ending_tok = 1;
        switch (ch = getChar()) {
          case 'X':
          case 'x':
            c.dt(ebyte);
            getExtBytes(c);
            break;
          case 'H':
          case 'h':
            c.dt(eshort);
            getExtShorts(c);
            break;
          default:
          case 'L':
          case 'l':
            c.dt(elong);
            getExtLongs(c);
            break;
          case 'D':
          case 'd':
            c.dt(edoub);
            getExtDoubs(c);
            break;
        }
        break;
      case '=':
        if (tok)
          ending_tok = 1;
        switch (ch = getChar()) {
          case 'X':
          case 'x':
            c.dt(ibyte);
            c.siz(getABytes());
            break;
          case 'H':
          case 'h':
            c.dt(ishort);
            val = (unl) getAShort() << 16;
            val |= getAShort();
            c.siz(val);
            break;
          default:
          case 'L':
          case 'l':
            c.dt(ilong);
            c.siz((unl) getALong());
            break;
        }
        break;
      case '\'':
        if (tok)
          ending_tok = 1;
        c.dt(ename);
        getExtStr(c);
        break;
      case '"':
        if (tok)
          ending_tok = 1;
        c.dt(etext);
        getExtStr(c);
        break;
      case '%':
        if (tok) { // must know line count now
          tbuf[tok] = '\0';
          tok = 0;
          c.dat(tbuf); 
          step++;
        }
        c.dt(ecode);
        getExtCode(c);
        break;
      case ':':
        if (tok)
          ending_tok = 1;
        c.dt(stamp);
        c.siz(getAULong());
        break;
      default:
        if (isspace(ch)) {
          if (tok)
            ending_tok = 1;
        }
        else if (tok < TokenMax)
          tbuf[tok++] = ch;
        break;
    }

    if (ending_tok) {
      tbuf[tok] = '\0';
      tok = 0;
      ending_tok = 0;
      switch (step++) {
        case 0:  // major type
          c.maj(tbuf);
          break;
        case 1:  // minor type
          c.min(tbuf);
          if (user_def)
            c.min(c.min() | 0x80);
          break;
        case 2:  // data field
          c.dat(tbuf);
          break;
        case 3:  // size field
          if (c.siz())
            break;
          c.siz(tbuf);
          break;
        default: // badness
          break;
      }
    }
    if (ending_ctl) {
      if (c.dt() == group)
        pushGroup(c);
      skipWhite();
      break;
    }
  }
	fpos = tell();
  return c;
}


DCctl&
DCLrbfile::getBCtl(DCctl& c)
{
  int ch;
  size_t i, lim;

  c.reset();

  if (Ferr)
    return c;

  if ((ch = getChar()) == EOF)
    return c;

  DCblock *b = new DCblock((size_t) 0);

  c.dt((dc_dtyp) ((ch >> 4) & 0xF));
  c.maj(ch & 0xF);

  c.min(getChar());
  c.dat( (uns) (this->*getShort)());
  if (c.dt() == stamp)
    c.siz((this->*getULong)());
  else c.siz( (unl) (this->*getLong)());

	if (c.siz() > 0) {
		switch (c.dt()) {
			case eshort:
				if (dc_mach != ftype) {
					short *s = (short *) b->set(c.siz());
					lim = c.siz() / sizeof(short);
					for (i = 0; i < lim; i++)
						*s++ = (this->*getShort)();
					c = *b;
					break;
				} // fall through
			case elong:
				if (dc_mach != ftype) {
					long *l = (long *) b->set(c.siz());
					lim = c.siz() / sizeof(long);
					for (i = 0; i < lim; i++)
						*l++ = (this->*getLong)();
					c = *b;
					break;
				} // fall through
			case edoub:
				if (dc_mach != ftype) {
					double *d = (double *) b->set(c.siz());
					lim = c.siz() / sizeof(double);
					for (i = 0; i < lim; i++)
						*d++ = (this->*getDouble)();
					c = *b;
					break;
				} // fall through
			case ebyte:
				b->set(c.siz());
				*b = *this;
				c = *b;
				break;

			case ename:
			case etext:
			case ecode:
				b->set(c.siz());
				*b = *this;
				// ensure that char data is null-terminated
				if ( ((char *)(b->p()))[c.siz() - 1] != '\0') {
					b->set(c.siz() + 1);
					((char *)(b->p()))[c.siz()] = '\0';
				}
				c = *b;
				break;
			case group:  // no special treatment, binary group ctls are complete
			case endgroup:
				break;
			default:
				break;
		}
	}
	else {
		switch (c.dt()) {  // ensure that char data is null-terminated
			case ename:
			case etext:
			case ecode:
				b->set(1);
				((char *)(b->p()))[0] = '\0';
				c = *b;
				break;
			default:
				break;
		}
	}

  if ((ch = getChar()) != EOF)
    ungetChar(ch);

  delete b;
  return c;
}



// stack of ctls for handling ASCII dcl group ends


DCctl DCLrfile::GroupNest[DCLrfile::GroupMax];
unc DCLrfile::GNest = 0;


void
DCLrfile::pushGroup(DCctl& c)
{
  if (GNest < GroupMax) {
    DCctl t;
    t.maj(c.maj());
    t.min(c.min());
    t.dat(c.dat());
    t.siz(c.siz());
    GroupNest[GNest] = t;
  }
  GNest++;
}

void
DCLrfile::popGroup(DCctl& c)
{
  GNest--;
  if (GNest < GroupMax) {
    DCctl t;
    t = GroupNest[GNest];
    c.maj(t.maj());
    c.min(t.min());
    c.dat(t.dat());
    c.siz(t.siz());
  }
}


// end of dcfilr.cc

