
// dcappl.h is the header for dcl application classes, JHG, 6/18/93
// Copyright (c) 1993 by Omni Systems, Inc.  All rights reserved.

#ifndef DCAPPL_H
#define DCAPPL_H

#include "dcglob.h"
#include "dcbase.h"
#include "dclist.h"
#include "dcnode.h"
#include "dcfile.h"
#include "dcfilr.h"
#include "dcfilw.h"
#include "dclog.h"

// dcl application object

enum dc_atyp { dc, dr, dw, dn};

typedef void (*pvfcp)(char *);

struct swlist {
  char *swmatch;
  char *swval;
  pvfcp swfunc;
  bool swarg;
};


class DCapp {
 protected:
  char *rfn;
  char *wfn;
  dc_ftyp wtyp;

  char *wfs;
  char *xfs;
  char *dafs;
  char *dbfs;
  char *dxfs;

  char *wfp;

  dc_atyp atyp;
  short verb;

  int argpos;
  int argc;
  char **argv;

  char target[PATHMAX];
  bool Processed;

 public:

  DCapp(dc_atyp atype, char *x = ".doc", char *da = ".dcl", char *db = ".dcb", char *dx = ".dcx")
    { xfs = x; dafs = da; dbfs = db; dxfs = dx; rfn = wfn = wfp = wfs = NULL;
      wtyp = (atype == dr) ? fasc : ftyps; verb = 0;
      atyp = atype; argc = argpos = 1; argv = NULL; }
  virtual ~DCapp(void) {}

  void Init(int ac, char **av);
  bool Proc(swlist *swused = NULL);
  char *rfile(void) { return rfn; }
  char *wfile(void) { return wfn; }
  dc_ftyp wtype(void) { return wtyp; }
  short verbose(void) { return verb; }

  static void SetEndian(void);
};

#endif


