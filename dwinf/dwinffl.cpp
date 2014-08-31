
// dwinffl.cc contains code for the inf file write classes, JHG
// Copyright (c) 1998 by Omni Systems, Inc.  All rights reserved.

#include "dcglob.h"
#include "dcbase.h"
#include "dclist.h"
#include "dcfile.h"
#include "dcfilw.h"
#include "dwinffl.h"



INFwfile::INFwfile(char *fname) : DCwfile(fname, ftxt)
{
  if (Ferr)
    return;
}


void
INFwfile::CloseFile()
{
	putEnd();
  DCwfile::CloseFile();
}


void
INFwfile::putShort(short n)
{
  if (n == 0) {
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


// elements


void
INFwfile::putSection(char *str)
{
	putEnd();
	putChar('[');
  putStr(str);
	putChar(']');
}


void
INFwfile::putComment(char *str)
{
	putEnd();
	putChar(';');
  putStr(str);
}


void
INFwfile::putKey(char *str)
{
	putEnd();
	putStr(str);
	putChar('=');
}


// end of dwinffl.cc

