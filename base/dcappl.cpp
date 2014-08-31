
// dcappl.cc contains code for the dcl application class, JHG, 4/4/97
// Copyright (c) 1993-1997 by Omni Systems, Inc.  All rights reserved.

#include "dcappl.h"



// initialization functions


void
DCapp::Init(int ac, char **av)
{
  static bool Initialized = false;

  // initialize args for later Proc
  argc = ac;
  argv = av;

  target[0] = '\0';
  Processed = false;

	// only need to do the rest once per load
  if (Initialized)
    return;
  Initialized = true;

  // determine endian-ness of current machine
  SetEndian();

  // set up the ascii mnemonic definitions as a tree
	if (atyp != dn)
	  DCnode::InitNodes(NULL, NULL);
}
 


void
DCnode::InitNodes(char *nFile, DCnode *Top)
{
  if (!Top)             // normal case, use standard top node
    TopNode = new DCnode( 0, 5, "root");
  else TopNode = Top;   // special, user wants own tree

  if (!nFile) {         // normal, use internal table for mnemonics
    if (InitNodeList[0].c) {
      ListInit(TopNode);
      return;
    }
    nFile = NodeFile;   // unless it is empty, then use standard file
  }

  // obtain mnemonics from a file

  DCLrfile nf((const char *) nFile);

  if (nf.FErr() == fok) {
    DCctl c;
    while (!nf.eof())
      c = nf;           // which calls ProcNodeCtl(c) to load mnemonics
  }

  nf.CloseFile();
  nf.Err();
}


void
DCapp::SetEndian(void)
{
  union {
    long l;
    char ch[4];
  } tmp;

  tmp.l = 0x10203;
  switch (tmp.ch[0]) {
    case 0:            // msw and msb first, Motorola, SPARC, SGI MIPS
      dc_mach = fmsb;
      dc_msb_index = 0;
      dc_lsb_index = 1;
      break;
    case 3:            // lsw and lsb first, Intel, VAX, DEC MIPS
      dc_mach = flsb;
      dc_msb_index = 1;
      dc_lsb_index = 0;
      break;
    case 1:            // msw and lsb first, PDP-11
      dc_mach = fpdp;
      dc_msb_index = 1;
      dc_lsb_index = 0;
      break;
    case 2:            // lsw and msb first, unknown
    default:
      dc_mach = ftyps;
      dc_msb_index = 0;
      dc_lsb_index = 1;
      break;
  }
}



// argument processing


bool
DCapp::Proc(swlist *swused)
{
  struct stat stbuf;
  swlist *swptr;
	uns argerr = (argc == 1) ? 1 : 0;

  rfn = wfn = NULL;

  for (; argpos < argc; argpos++) {
    if (isswitch(*argv[argpos])) {
      switch(argv[argpos][1]) {
        case 'A':
        case 'a':
          wtyp = fasc;
          break;
        case 'B':
        case 'b':
          wtyp = dc_mach;
          break;
        case 'M':
        case 'm':
          wtyp = fmsb;
          break;
        case 'L':
        case 'l':
          wtyp = flsb;
          break;
        case 'O':
        case 'o':
          if (argv[argpos][2] == 0) {  // -o filename
            if (((argpos + 1) < argc)
             && !isswitch(*argv[argpos + 1]))
              wfn = argv[++argpos];
          }
          else {   // space omitted, use name here
            wfn = &argv[argpos][2];
          }
          // categorize wfn as suffix, path, or filename
          if ((wfn[0] == '.') && !strchr(wfn, path_sep)) {  // assume suffix
            wfs = wfn;
            wfn = NULL;
            break;
          }
          if (!stat(wfn, &stbuf) && S_ISDIR(stbuf.st_mode)) { // dir is path
            wfp = wfn;
            wfn = NULL;
          }
          break;
        case 'P':
        case 'p':
          wtyp = fpdp;
          break;
        case 'V':
        case 'v':
          if (argv[argpos][2] == 0)
            verb = 1;
          else verb = atoi(&argv[argpos][2]);
          break;
        case 'X':
        case 'x':
          wtyp = fxml;
          break;
        default:
					argerr++;
          if (!swused)
            break;
          for (swptr = swused; swptr->swmatch; swptr++) {
            uns slen = strlen(swptr->swmatch);
            if (!strncmp(&argv[argpos][1], swptr->swmatch, slen)) {
							argerr--;
              if (swptr->swarg) {
                if ((argpos + 1 < argc) && (argv[argpos][slen + 1] == 0))
                  swptr->swval = argv[++argpos];
                else swptr->swval = &argv[argpos][slen + 1];
              }
              if (swptr->swfunc)
                (*swptr->swfunc)(swptr->swval);
							if ((slen == 1)
							 && ((*(swptr->swmatch) == 'u')
							  || (*(swptr->swmatch) == 'U')))
								wtyp = fasc;  // for uxml, use ascii dcl for now
              break;
            }
          }
          break;
      }
    }
    else {  // source file name, so not in a pipe
      rfn = argv[argpos++];
      if (wfn == NULL) {   // generate target file name
        if (wfp) {
          strcpy(target, wfp);
          char *cpt = strrchr(rfn, path_sep);
          strcat(target, (cpt == NULL) ? rfn : (cpt + 1));
        }
        else strcpy(target, rfn);
        int len;
				if (atyp == dn) {  // dcl not used, dafs is output suffix
          if ((len = strlen(target) - strlen(xfs)),
               !strcmp(xfs, &target[len]))
            strcpy(&target[len], dafs);
				}
        else if (atyp == dw) {       // source is dcl
          if (((len = strlen(target) - strlen(dafs)),
               !strcmp(dafs, &target[len]))
           || ((len = strlen(target) - strlen(dbfs)),
               !strcmp(dbfs, &target[len])))
            strcpy(&target[len], wfs ? wfs : xfs);
        }
        else if (atyp == dc) {  // source and target are dcl
          if ((len = strlen(target) - strlen(dafs)),
               !strcmp(dafs, &target[len])) {
            if (wtyp == ftyps)
              wtyp = dc_mach;
            strcpy(&target[len], wfs ? wfs : 
						                     ((wtyp == fasc) ? dafs :
						                     ((wtyp == fxml) ? dxfs : dbfs)));
          }
          else if ((len = strlen(target) - strlen(dbfs)),
               !strcmp(dbfs, &target[len])) {
            if (wtyp == ftyps)
              wtyp = fasc;
            strcpy(&target[len], wfs ? wfs : 
						                     ((wtyp == fasc) ? dafs :
						                     ((wtyp == fxml) ? dxfs : dbfs)));
          }
          else if ((len = strlen(target) - strlen(dxfs)),
               !strcmp(dxfs, &target[len])) {
            if (wtyp == ftyps)
              wtyp = fxml;
            strcpy(&target[len], wfs ? wfs : 
						                     ((wtyp == fasc) ? dafs :
						                     ((wtyp == fxml) ? dxfs : dbfs)));
          }
        }
        else if (atyp == dr) {  // target is dcl, name per type
          if ((len = strlen(target) - strlen(xfs)),
               !strcmp(xfs, &target[len]))
            strcpy(&target[len], wfs ? wfs : 
						                     ((wtyp == fasc) ? dafs :
						                     ((wtyp == fxml) ? dxfs : dbfs)));
        }
        wfn = target;
      }
#ifdef DOSSTAT
      if (!stat(wfn, &stbuf) && !strcmp(rfn, wfn)) { // target & source same name
#else
      stat(rfn, &stbuf);
      unl rdev = stbuf.st_dev;
      unl rino = stbuf.st_ino;
      int wst = stat(wfn, stbuf);
      unl wdev = stbuf.st_dev;
      unl wino = stbuf.st_ino;
      if ((wst == 0) && (rdev == wdev) && (rino == wino)) {  // target is source
#endif
        return false;
      }
      return true;
    }
  }

  if (argerr
	 || Processed)  // pipes are only used once
    return false;
  Processed = true;
  return true;  // if no source file, must be using pipe
}


// end of dcappl.cc



