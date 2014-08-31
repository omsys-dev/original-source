
// dcl.h is the header for the dcl application CLI, JHG
// Copyright (c) 1993-1997 by Omni Systems, Inc.  All rights reserved.

#ifndef DCL_H
#define DCL_H

#include "dcglob.h"


typedef char path[PATHMAX];

struct mods {
  char *msuf;
  char *mname;
  short mopt;
  unc mstart[6];
  bool mavail;
  bool mdll;
  char *mdname;
  char *mappl;
};



// dcl application CLI


class DCcli {
  char *rfn;
  char *wfn;
  dc_ftyp wtyp;

  char *wfs;
  char *rfs;
  char *xfs;
  char *dafs;
  char *dbfs;

  char *wfp;

  short verb;

  int argpos;

 public:

  DCcli(char *x = ".doc", char *da = ".dcl", char *db = ".dcb")
    { xfs = x; dafs = da; dbfs = db; rfn = wfn = wfp = wfs = rfs = NULL;
      wtyp = ftyps; argpos = 1; verb = 0; }
  virtual ~DCcli(void) {}

  bool Proc(int argc, char **argv, char **outsw);
  char *rfile(void) { return rfn; }
  char *wfile(void) { return wfn; }
  dc_ftyp wtype(void) { return wtyp; }
  short verbose(void) { return verb; }

  static void SetEndian(void);
};

#endif


