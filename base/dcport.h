
// dcport.h is the portability header for dcl C++ modules, JHG, 9/18/93
// Copyright (c) 1993 by Omni Systems, Inc.  All rights reserved.

#ifndef DCPORT_H
#define DCPORT_H


// request Posix compatibility (for the C library)

#define _POSIX_SOURCE


// make the Standard C library available

extern "C" {
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <signal.h>
#include <setjmp.h>
#include <stddef.h>
#include <time.h>
#include <stdlib.h>
#include <float.h>
#include <assert.h>
}

// fill in any holes by adding code in dcport.cc

#ifndef _DLL
char *strstr(const char *s1, const char *s2);
#endif

// DOS vs UNIX issues

#ifdef MSDOS
#undef toupper
#undef tolower
extern "C" {
#include <math.h>
#include <malloc.h>
#include <sys\types.h>
#include <sys\stat.h>
}
#define S_ISBLK(m)      (((m)&S_IFMT) == S_IFBLK)
#define S_ISCHR(m)      (((m)&S_IFMT) == S_IFCHR)
#define S_ISDIR(m)      (((m)&S_IFMT) == S_IFDIR)
#define S_ISFIFO(m)     (((m)&S_IFMT) == S_IFIFO)
#define S_ISREG(m)      (((m)&S_IFMT) == S_IFREG)
#define PATHMAX 64
#define path_sep  '\\'
#define isswitch(c) (((c) == '-') || ((c) == '/'))
#define DOSTIME
#define DOSSTAT
#define DOSMEM

#else

// see if NT or Win95
#ifdef _WIN32
#undef toupper
#undef tolower
#include <math.h>
extern "C" {
#include <malloc.h>
#include <sys\types.h>
#include <sys\stat.h>
}
#define PATHMAX _MAX_PATH
#define path_sep  '\\'
#define isswitch(c) (((c) == '-') || ((c) == '/'))
// required for Win 32, where stbuf.inode is always 0
#define DOSSTAT
#define S_ISCHR(m)      (((m)&_S_IFMT) == _S_IFCHR)
#define S_ISDIR(m)      (((m)&_S_IFMT) == _S_IFDIR)
#define S_ISREG(m)      (((m)&_S_IFMT) == _S_IFREG)
#pragma warning(disable:4800)

// must be UNIX
#else
extern "C" {
#include <math.h>
#include <sys/types.h>
#include <sys/stat.h>
}
#define PATHMAX 1024
#define path_sep  '/'
#define isswitch(c) ((c) == '-')
typedef unc bool;
enum bools { false = 0, true = 1 };
#endif

#endif


// dcl modules standard typedefs

typedef unsigned char  unc;
typedef unsigned short uns;
typedef unsigned long  unl;

typedef char **cpp;


#endif


