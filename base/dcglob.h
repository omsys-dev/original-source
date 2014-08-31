
// dcglob.h is the header for dcl global vars and funcs, JHG, 4/9/97
// Copyright (c) 1993-1997 by Omni Systems, Inc.  All rights reserved.

#ifndef DCGLOB_H
#define DCGLOB_H

#pragma warning ( disable : 4237 )
#pragma warning ( disable : 4514 )

#include "dcport.h"

class DCapp;
class DCblock;
class DCctl;
class DCnode;
class DCfile;
class DCrfile;
class DCLrbfile;
class DCLrfile;
class DCwfile;
class DCLwbfile;
class DCLwfile;


// global enums

enum dc_err {
  dc_ok, dc_err_arg, dc_err_name, dc_err_mem,
  dc_err_noread, dc_err_badform, dc_err_read,
  dc_err_nowrite, dc_err_write, dc_err_node,
  dc_err_ftyp, dc_err_ini, dc_err_max
};

enum dc_ftyp {
  fasc, fmsb, flsb, fpdp, ftxt, fbin, fxml, ftyps
};


// global variables

extern unc dc_msb_index; // endian index values
extern unc dc_lsb_index;
extern dc_ftyp dc_mach;  // machine int storage type

extern bool NoNameDel;
extern bool NoMemDel;


// global operators

void *::operator new(size_t BlockLen);
void ::operator delete(void *m);


// global functions

char *NewName(short NameLen);
char *NewName(const char *str, size_t len = 0, bool mem = false);
void IncName(char *c, bool inc = true);
void DeleteName(char *c);
void *NewMem(size_t MemLen);
void *ResizeMem(void *m, size_t MemLen);
void DeleteMem(void *m);
void MemErr(const char *c, size_t l);

unc IsUTF8(unc *cpt);
unl UTF8(unc **cpt);
unc *UTF8(unl uch, unc *cpt = NULL);

void GetBaseName(char *fname, char *basepath, char *basename);


// utility classes

class DCblock {
  void *ptr;
  size_t len;
 public:
  DCblock() { ptr = NULL; len = 0; }
  DCblock(size_t l) { ptr = NewMem(len = l); }
  ~DCblock() { if (ptr) DeleteMem(ptr); }
  void *p()  { return ptr; }
  size_t l() { return len; }
  void *set(size_t l)
    { ptr = ResizeMem(ptr, len = l); return ptr; }
	void wrap(void *p, size_t l)
		{ ptr = p; len = l; }

  // the clear() function should be used only if deallocation
  // will be done as part of another object's destructor
  void clear() { ptr = NULL; len = 0; }

  DCblock& operator= (DCblock& b)
    { set(b.len); memcpy(ptr, b.ptr, len); return *this; }
  DCblock& operator= (DCrfile& rf);
  DCblock& operator= (DCctl& c);
};


#endif


