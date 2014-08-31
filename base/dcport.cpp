
// dcport.cc is the portability code for dcl C++ modules, JHG, 1/10/93
// Copyright (c) 1993 by Omni Systems, Inc.  All rights reserved.

#pragma warning ( disable : 4237 4244 4514 )

#include "dcport.h"

#ifndef _DLL

char *
strstr(const char *s1, const char *s2)
{
  short len = strlen(s2);

  while (*s1) {
    if (!strncmp(s1, s2, len))
      return (char *) s1;
    s1++;
  }
  return NULL;
}


#endif

// end of dcport.cc


