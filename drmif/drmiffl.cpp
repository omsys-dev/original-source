 
// drmiffl.cc is the code for dcl FrameMaker MIF file reading, JHG
// Copyright (c) 1993-1997 by Omni Systems, Inc.  All rights reserved.

// define VECINSET to process FrameVector and PICT insets
// or just PICTOK to permit PICT (for export only)
#define PICTOK


#include "dcappl.h"
#include "dcctlg.h"
#include "dcfili.h"
#include "dcini.h"
#include "drmif.h"
#include "drmiffl.h"


char *MIFrfile::ErrMsg[] = {
  "opening", "not MIF format", "reading"
};
logfunc MIFrfile::LogEventFunc = NULL;



// file opening


MIFrfile::MIFrfile(char *fname) : DCrfile(fname)
{
  Err();

  // verify file type
#if 0
  DCblock word_buf(11);
  word_buf = *this;
  if (strncmp( (char *) word_buf.p(), "<MIFFile ", 9)) {
    Ferr = fform;
    return;
  }
  MIFver = ((char *) word_buf.p())[9] - '0';
  skipLine();
#else
	char head[80];
	char *fm = NULL;
	if (!getLine(head, 80)
	 || strncmp(head, "<MIFFile ", 9)
	 || ((fm = strstr(head, "FrameMaker")) == NULL)) {
    Ferr = fform;
    return;
  }
  MIFver = head[9] - '0';
	if (MIFver == 1) {
    if (isdigit(head[10]))
			MIFver = (head[10] - '0') + 10;
		else {
			Ferr = fform;
			return;
		}
	}
	FMver = NewName(fm + 11);
#endif
  SetUnits = inch;
}

void
MIFrfile::Err(void)
{
  if (Ferr == fok)
    return;
	LogEvent(logwarn, 1, "Error reading file ", fname, ",",
	         ErrMsg[((int) Ferr) - 1]);
  fprintf(stderr, "Readfile %s: error %s.\n",
    fname ? fname : "stdin", ErrMsg[((int) Ferr) - 1]);
  CloseFile();
  exit( (int) ErrTypes[Ferr - 1]);
}


void
MIFrfile::LogEvent(logtype lt, unc sev, const char *desc,
	const char *d2, const char *d3, const char *d4, const char *d5)
{
	if (!LogEventFunc)
		return;

	LogEventFunc(lt, sev, desc, d2, d3, d4, d5);
}



// reading statements

char *
MIFrfile::getWord()
{
  static char Word[LineMax];
  char *cptr = Word;
  int ch;

  skipWhite();
  while ((ch = getChar()) != EOF) {
    if (isalnum(ch))
      *cptr++ = ch;
    else {
      ungetChar(ch);
      break;
    }
  }
  *cptr = '\0';
  skipWhite();
  return Word;
}


void
MIFrfile::skipStatement()
{
  int ch;
  uns level = 1;

  while ((ch = getChar()) != EOF) {
    if (ch == '>') {
      if (--level == 0)
      break;
    }
    else if (ch == '<')
      level++;
    else if (ch == '\\')
      getChar();
    else if (ch == '`')
      skipString();
  }
  skipWhite();
}


void
MIFrfile::skipString()
{
  int ch;

  while ((ch = getChar()) != EOF) {
    if (ch == '\'')
      break;
    if (ch == '\\')
      getChar();
  }
}



// reading import objects


void
MIFrfile::getImport()
{
  int ch;
  unc val = 0;
  unc digit = 0;
  char *cpt;
	unc raspriority = 0;
	unc vecpriority = 0;
  bool raster = false;
  bool vector = false;
  bool epsi = false;
  bool starting = false;
  bool skipping = false;
  bool inhex = false;
  size_t i = 0;
  size_t bcount;
  size_t bstart = 4096;
  size_t bincr = 4096;
  DCblock *b = NULL;
  char *p;

  // preset globals
  MIFvector = MIFraster = MIFepsi = NULL;
	MIFvectype = novect;
  MIFrastype = norast;

  bool linestart = true;
	bool needLF = false;
	bool noLF = true;

  while ((ch = getChar()) != EOF) {
    switch (ch) {
      case '\r':
      case '\n':
        linestart = true;
				if (needLF == true) {
					ch = '\n';
					needLF = false;
					break;
				}
        continue;
      case '=':  // facet name or end of insets
        if (linestart == false)
          break;  // regular epsi character

				if (b) {	// have stored info, put it away
	        if (epsi)
		        p[i++] = '\0';
	        if (i < bcount)
		        b->set(i);
					if (raster)
						MIFraster = b;
					else if (vector)
						MIFvector = b;
					else if (epsi)
						MIFepsi = b;
					else delete b;
					b = NULL;
				}
        epsi = false;
        raster = false;
				vector = false;
				noLF = true;

        cpt = getWord();
        if (!strcmp(cpt, "EndInset"))  // finished with insets
          return;
        else if (!strcmp(cpt, "DIB")) {
					if (MIFrastype != norast) {
						if (raspriority > 100) {
							skipping = true;
							continue;
						}
						else delete MIFraster;
					}
					raspriority = 100;		
          raster = true;
          MIFrastype = bmp;
          b = new DCblock(bcount = bstart);
          p = (char *) b->p();
					i = 0;
          starting = true;
          continue;
        }
        else if (!strcmp(cpt, "FrameImage")) {
					if (MIFrastype != norast) {
						if (raspriority > 50) {
							skipping = true;
							continue;
						}
						else delete MIFraster;
					}
					raspriority = 50;		
          raster = true;
          MIFrastype = rf;
          b = new DCblock(bcount = bstart);
          p = (char *) b->p();
					i = 0;
          starting = true;
          continue;
        }
        else if (!strcmp(cpt, "TIFF")) {
					if (MIFrastype != norast) {
						if (raspriority > 40) {
							skipping = true;
							continue;
						}
						else delete MIFraster;
					}
					raspriority = 40;		
          raster = true;
          MIFrastype = tif;
          b = new DCblock(bcount = bstart);
          p = (char *) b->p();
					i = 0;
          starting = true;
          continue;
        }
        else if (!strcmp(cpt, "PCX")) {
					if (MIFrastype != norast) {
						if (raspriority > 30) {
							skipping = true;
							continue;
						}
						else delete MIFraster;
					}
					raspriority = 30;		
          raster = true;
          MIFrastype = pcx;
          b = new DCblock(bcount = bstart);
          p = (char *) b->p();
					i = 0;
          starting = true;
          continue;
        }
        else if (!strcmp(cpt, "GIF")) {
					if (MIFrastype != norast) {
						if (raspriority > 20) {
							skipping = true;
							continue;
						}
						else delete MIFraster;
					}
					raspriority = 20;		
          raster = true;
          MIFrastype = gif;
          b = new DCblock(bcount = bstart);
          p = (char *) b->p();
					i = 0;
          starting = true;
          continue;
        }
        else if (!strcmp(cpt, "JPEG")) {
					if (MIFrastype != norast) {
						if (raspriority > 15) {
							skipping = true;
							continue;
						}
						else delete MIFraster;
					}
					raspriority = 15;		
          raster = true;
          MIFrastype = jpg;
          b = new DCblock(bcount = bstart);
          p = (char *) b->p();
					i = 0;
          starting = true;
          continue;
        }
        else if (!strcmp(cpt, "PNG")) {
					if (MIFrastype != norast) {
						if (raspriority > 10) {
							skipping = true;
							continue;
						}
						else delete MIFraster;
					}
					raspriority = 10;		
          raster = true;
          MIFrastype = png;
          b = new DCblock(bcount = bstart);
          p = (char *) b->p();
					i = 0;
          starting = true;
          continue;
        }
        else if (!strcmp(cpt, "WMF")) {
					if (MIFvectype != novect) {
						if (vecpriority > 100) {
							skipping = true;
							continue;
						}
						else delete MIFvector;
					}
					vecpriority = 100;		
          vector = true;
          MIFvectype = wmf;
          b = new DCblock(bcount = bstart);
          p = (char *) b->p();
					i = 0;
          starting = true;
          continue;
        }
				else if (!strcmp(cpt, "QuickDraw")) {
					p = getWord(); // discard "PICT"
					assert(!strcmp(p, "PICT"));
#ifdef PICTOK
					if (MIFvectype != novect) {
						if (vecpriority > 50) {
							skipping = true;
							continue;
						}
						else delete MIFvector;
					}
					vecpriority = 50;		
          vector = true;
          MIFvectype = pct;
          b = new DCblock(bcount = bstart);
          p = (char *) b->p();
					i = 0;
          starting = true;
					noLF = true;
#else
					skipping = true;
#endif
					continue;
				}
#ifdef VECINSET
        else if (!strcmp(cpt, "FrameVector")) {
					if (MIFvectype != novect) {
						if (vecpriority > 40) {
							skipping = true;
							continue;
						}
						else delete MIFvector;
					}
					vecpriority = 40;		
          vector = true;
          MIFvectype = fmv;
          b = new DCblock(bcount = bstart);
          p = (char *) b->p();
					i = 0;
          starting = true;
          continue;
        }
#endif
        else if (!strcmp(cpt, "WPG")) {
					if (MIFvectype != novect) {
						if (vecpriority > 30) {
							skipping = true;
							continue;
						}
						else delete MIFvector;
					}
					vecpriority = 30;		
          vector = true;
          MIFvectype = wpg;
          b = new DCblock(bcount = bstart);
          p = (char *) b->p();
					i = 0;
          starting = true;
          continue;
        }
        else if (!strcmp(cpt, "CDR")) {
					if (MIFvectype != novect) {
						if (vecpriority > 20) {
							skipping = true;
							continue;
						}
						else delete MIFvector;
					}
					vecpriority = 20;		
          vector = true;
          MIFvectype = cdr;
          b = new DCblock(bcount = bstart);
          p = (char *) b->p();
					i = 0;
          starting = true;
          continue;
        }
        else if (!strncmp(cpt, "OLE", 3)) {
					if (MIFvectype != novect) {
						if (vecpriority > 10) {
							skipping = true;
							continue;
						}
						else delete MIFvector;
					}
					vecpriority = 10;		
          vector = true;
          MIFvectype = ole;
          b = new DCblock(bcount = bstart);
          p = (char *) b->p();
					i = 0;
          starting = true;
          continue;
        }
        else if (!strcmp(cpt, "EPSI")) {
          epsi = true;
					MIFlines = 0;
          b = new DCblock(bcount = bstart);
          p = (char *) b->p();
					i = 0;
          starting = true;
					noLF = false;
          continue;
        }
        else {
					skipping = true;  // for other facets
					skipLine();       // may be multi-word
					linestart = true;
				}
        continue;
      case '&':  // data line
        if (linestart == false)
          break;  // regular epsi character
        if (starting) {  // check %v
          if ((getChar() == '%') && (getChar() == 'v'))
            skipping = false;
          else
            skipping = true;
#ifdef VECINSET
					if (MIFvector && (MIFvectype == frvect)) {
						if (skipping) {  // not a %v, must be version line
							skipLine();
							skipping = false;
							starting = false; // real start now
							continue;
						} // otherwise was %v, still starting
					}
          else
#endif
					starting = false;
        }
        if (skipping)
          skipLine();
        else linestart = false;
        continue;
      case '\\':  // backslash escape
        if ((ch = getChar()) == 'x') { // hex switch in raster
          inhex = !inhex;
          continue;
        }
				else if (ch == 'r') { // may be end of epsi line
					ch = '\r';
					MIFlines++;
					if (!noLF)
						needLF = true;
				}
        else if (ch == 'n') { // end of epsi line
          ch = '\n';
					if (needLF)
						needLF = false;
					else
						MIFlines++;
        }
        break; // real char in data (may be \), add it
      default:
        if (inhex) {
          if (isxdigit(ch))
            val = (val << 4)
                 + (ch - ((ch > '9') ? ((ch > 'F') ? 'a'-10 : 'A'-10 ) : '0'));
          if (digit) {
            digit = 0;
            ch = val;
            val = 0;
          }
          else {
            digit = 1;
            continue;
          }
        }
        break;
    }
    p[i] = ch;
    if (++i == bcount) {
      b->set(bcount += bincr); 
      p = (char *) b->p();
    }
  }
}


void
MIFrfile::skipImport()
{
  int ch;

  skipLine();

  while ((ch = getChar()) != EOF) {
    switch (ch) {
      case '&':
      case '=':
        skipLine();
        break;
      case '\n':
      case '\r':
      case ' ':
        break;
      default:
        ungetChar(ch);
        return;
    }
  }
}



// reading arguments

char *
MIFrfile::getString(long len)
{
  static char String[LineMax];
  char *cptr = String;
	long pos = 0;
  int ch = 0;

	if (len > LineMax)
		cptr = new char[len];

  skipWhite();

	while ((ch = getChar()) != EOF) {
		if (ch == '`')     // skip opening quote
			continue;
		if (ch == '\'')    // stop at ending quote
			break;
		if (ch == '\\') {  // process some backslashs
			ch = getChar();
			if (ch == 'q')
				ch = '\'';
			else if (ch == 'Q')
				ch = '`';
			//else if (ch == '\\')
			//	;
			//	cptr[pos++] = ch;
			else if (ch != '>') {  // leave \\ and \x and \t alone
				ungetChar(ch);
				ch = '\\';
			}
		}
		cptr[pos++] = ch;  // keep the rest
		if (pos > (len - 3))
			break;
	}
	cptr[pos] = '\0';

  return cptr;
}


long
MIFrfile::getVal()
{
  int ch;
  long val = 0;
  int decim = 0;
  long neg = 1;

  skipWhite();
  while ((ch = getChar()) != EOF) {
    if (isdigit(ch)) {
      val = (val * 10) + (ch - '0');
      if (decim)
        decim++;
    }
    else if (ch == '-')
      neg = -1;
    else if (ch == '.')
      decim = 1;  // decimal point
    else {
      ungetChar(ch);
      break;
    }
  }
  if (decim)
    decim--;

  skipWhite();
  CurrUnits = SetUnits;
  switch (ch = getChar()) {
		case '%':  // percent
			CurrUnits = percent;
			break;
    case '"':  // inches
      CurrUnits = inch;
      break;
    case 'c':
      ch = getChar();
      if (ch == 'm')
        CurrUnits = cm;
      else if (ch == 'c')
        CurrUnits = cc;
      else ungetChar(ch);
      break;
    case 'm':
      ch = getChar(); 
      if (ch == 'm')
        CurrUnits = mm;
      else ungetChar(ch);
      break;
    case 'p':
      ch = getChar(); 
      if (ch == 'c')
        CurrUnits = pc;
      else if (ch == 't') 
        CurrUnits = pt;
      else ungetChar(ch);
      break;
    case 'd':
      ch = getChar();  
      if (ch == 'd') 
        CurrUnits = dd;
      else ungetChar(ch);
      break;
    default:
      ungetChar(ch);
      break;
  }

  switch (CurrUnits) {
    case inch:
      while (decim > 5) {
        val /= 10;
        decim--;
      }
      while (decim < 5) {
        val *= 10;
        decim++;
      }
      break;
    case pc:
    case cc:
      val *= 12;
    case pt:
    case dd:
      while (decim > 2) {
        val /= 10;
        decim--;
      }
      while (decim < 2) {
        val *= 10;
        decim++;
      }
      break;
    case cm:
      val *= 10;
    case mm:
      while (decim > 3) {
        val /= 10;
        decim--;
      }
      while (decim < 3) {
        val *= 10;
        decim++;
      }
      break;
    default: // handle unitless FDW value as percent
    case percent:
      while (decim > 2) {
        val /= 10;
        decim--;
      }
      while (decim < 2) {
        val *= 10;
        decim++;
      }
      break;
  }

  return val * neg;
}



// end of drmiffl.cc


