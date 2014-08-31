
// dcx.cc provides two-way dcl ascii-binary conversion, JHG
// Copyright (c) 1993-2009 by Omni Systems, Inc.  All rights reserved.

#include "dcappl.h"


#ifdef _DLL

#define DllExport extern "C" __declspec( dllexport)

DllExport int
DCLmain(int argc, char* argv[])
{
  static bool dll_init = false;

  if (dll_init) {
    fprintf(stderr, "%s: DLL reinitialization error\n", argv[0]);
    exit(dc_err_arg);
  }
  dll_init = true;


#else

int
main(int argc, char *argv[])
{

#endif

  DCapp app(dc);

  app.Init(argc, argv);

  if (!app.Proc()) {
    fprintf(stderr, "usage: %s [-abmlp] [-o new_dcl_file] dcl_file\n", argv[0]);
    exit(dc_err_arg);
  }

  DCLrfile rf(app.rfile());
  if (rf.FErr() != fok)
    rf.Err();

  DCLwfile wf(app.wfile(), app.wtype());
  if (wf.FErr() != fok)
    wf.Err();

  // here all the work is done, one dcl ctl at a time

  DCctl c;
  while (!rf.eof()) {
    c = rf;
    wf = c;
  }
  rf.CloseFile();
  wf.CloseFile();

  // the envelope . . .

  rf.Err();      // these two just return if no err
  wf.Err();      // otherwise they exit with the err (and a message)
  return dc_ok;  // so if we get here, it worked
}


// end of dcx.cc

