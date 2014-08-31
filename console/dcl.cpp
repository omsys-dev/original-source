
// dcl.cpp contains code for driving dcl DLLs in Win32, JHG
// Copyright (c) 1995-2008 by Omni Systems, Inc.  All rights reserved.

#ifdef _DCLDLL
//#include "impdcx.h"
//#define DllImport  __declspec( dllimport )
typedef int (*DCLdll)(int, char*[]);

#include <windows.h>
#endif

#ifdef VC4
#define bool unsigned char
#define false 0
#define true 1
#endif

#include "dcl.h"

mods smods[] = {
  { "dcb", NULL, 1, { 0xA5, 0xC3, 0xC3, 0xA5 }, true, true, "dcx.dll", "DCL binary" },
  { "dcl", NULL, 0, "(DCL", true, true, "dcx.dll", "DCL ASCII" },
  { "xml", "drxml", 0, "<?xm", true, true, "drxml.dll", "XML" },
  { "doc", "drilf", 0, { 0x88, 'O', 'P', 'S' }, false, false, "", "Interleaf Fast" },
  { "ilf", "drilf", 0, "<!OP", false, false, "", "Interleaf ASCII" },
  { "int", "drint", 0, "Inte", false, false, "", "Interpress/Xerox" },
  { "doc", "drmak", 0, "<Mak", false, false, "", "non-MIF FrameMaker" },
  { "mif", "drmif", 0, "<MIF", true, true, "drmif.dll", "FrameMaker" },
  { "ps",  "drps",  0, "%!PS", false, false, "", "PostScript" },
  { "rtf", "drrtf", 0, "{\\rt", true, true, "drrtf.dll", "WinWord RTF" },
  { "doc", "drwin", 0, { 0xDB, 0xA5, 0x2D, 0 }, false, false, "", "non-RTF WinWord" },
  { "wpf", "drwpf", 0, { 0xFF, 'W', 'P', 'C' }, true, true, "drwpf.dll", "WordPerfect" },
  { "gpo", "drgpo", 0, "", true, true, "drgpo.dll", "GPO MicroComp" },
  { "tpl", "drtpl", 0, "", true, true, "drtpl.dll", "Read Template" },
  { "dita", "drxml", 0, "<?xm", true, true, "drxml.dll", "DITA" },
  { "ditamap", "drxml", 0, "<?xm", true, true, "drxml.dll", "DITA Map" },
  { "bookmap", "drxml", 0, "<?xm", true, true, "drxml.dll", "DITA Bookmap" },
  { "dcx", NULL, 2, "<?xm", true, true, "dcx.dll", "XDCL" },
  { "lst", NULL, 0, "LIST", true, false, "", "List of Files" },
  { NULL, NULL, 0, { 0, 0, 0, 0 }, false, false, NULL, NULL }
};

#define WPFSRC  11
#define GPOSRC  12
#define MIFSRC   7
#define DITASRC 14
#define DITACNT  3


mods tmods[] = {
  { "dcb", NULL, 1, { 0xA5, 0xC3, 0xC3, 0xA5 }, true, true, "dcx.dll", "DCL binary" },
  { "dcl", NULL, 0, "(DCL", true, true, "dcx.dll", "DCL ASCII" },
  { "dcx", NULL, 2, "<?xm", true, true, "dcx.dll", "XDCL" },
  { "ilf", "dwilf", 0, "<!OP", false, false, "", "Interleaf" },
  { "mif", "dwmif", 0, "<MIF", false, false, "", "FrameMaker" },
  { "rtf", "dwrtf", 0, "{\\rt", true, true, "dwrtf.dll", "WinWord RTF" },
  { "doc", "dwrtf", 0, "{\\rt", true, true, "dwrtf.dll", "WinWord RTF" },
  { "wpf", "dwwpf", 0, { 0xFF, 'W', 'P', 'C' }, false, false, "", "WordPerfect" },
  { "xrf", "dwxrf", 0, { 0, 0, 0, 0 }, true, true, "dwxrf.dll", "Xroff" },
  { "htm", "dwhtm", 0, "<!DO", true, true, "dwhtm.dll", "HTML" },
  { "xml", "dwhtm", 0, "<?xm", true, true, "dwhtm.dll", "XML" },
  { NULL, NULL, 0, { 0, 0, 0, 0 }, false, false, NULL, NULL }
};

#define XRFTARGET 8
#define RTFTARGET 5
#define DCLTARGET 1
#define HTMTARGET 9


#define XSUFMAX 10
#define XPROGPX  2
char sxsuf[XSUFMAX];
char sxprog[XSUFMAX + XPROGPX] = { "dr" };
mods sxmod = { sxsuf, sxprog, 0, { 0, 0, 0, 0 }, true, true, sxprog, sxsuf };
char txsuf[XSUFMAX];
char txprog[XSUFMAX + XPROGPX] = { "dw" };
mods txmod = { txsuf, txprog, 0, { 0, 0, 0, 0 }, true, true, txprog, txsuf };


#ifdef W2X
mods *smodule = &smods[WPFSRC];
mods *tmodule = &tmods[XRFTARGET];
#else
mods *smodule = &smods[MIFSRC];
mods *tmodule = &tmods[RTFTARGET];
#endif

char sysstr[1024];


#ifdef _DCLDLL

#define ERRMAX 14
char* ErrMsg[ERRMAX + 1] = {
 "Success",
 "Arguments unacceptable",
 "Name incorrect",
 "Memory unavailable",
 "Cannot open file to read",
 "File format incorrect",
 "Error during file read",
 "Cannot open file to write",
 "Error during file write",
 "Error in STDDEF.DCL",
 "Incorrect file type",
 ".ini-file missing",
 "Not a DCL DLL",
 "DLL missing",
 "Unknown error"
};

char DllMsg[1024];

#endif


int
main(int argc, char *argv[])
{
  static path rpath;
  static path tpath;
  static path wpath;
  DCcli cli;
	char *OutSwitch = NULL;
  static int RetVal = 0;

#define DLLMAX 5
  static char* TempFilePath[DLLMAX - 1];

#ifdef _DCLDLL
#define ARGMAX 6
  HMODULE hLibs[DLLMAX];
  DCLdll pMain;
  // DllEntries[DLLMAX];
  static int DllArgCounts[DLLMAX];
  static char* DllArgs[DLLMAX * ARGMAX];
  static int DllNum = 0;
  static int DllArgNum = 0;
  static int DllCount = 0;
  static int DllMissing = 0;
#endif

  // determine endian-ness of current machine
  DCcli::SetEndian();

  while (cli.Proc(argc, argv, &OutSwitch)) { // process a file

#ifdef _DCLDLL
		DllCount = 0;  // default if DLL not available
		DllMissing = 0;
#endif

		RetVal = 0;

		if ((OutSwitch == NULL)
		 && !strcmp(tmodule->msuf, "htm")) // clue needed by drmif
			OutSwitch = "-fN";  // N for Net (Help and Word are in use)

    // write filenames into system string, must be real files not pipes
    if (cli.rfile())
      strcpy(rpath, cli.rfile());
    else *rpath = '\0';

		TempFilePath[0] = NULL;

    if (cli.wfile())
      strcpy(wpath, cli.wfile());
    else *wpath = '\0';

    char *binary = (tmodule->mopt == 1) ? "-b" : "";

    if (smodule->mname && tmodule->mname) { // passing through DCL
			if (*rpath) {  // not from a pipe, create temp in src dir
				strcpy(tpath, rpath);
				char *pch = strrchr(tpath, '.');
				if (pch)  // replace ext
					strcpy(pch, ".dcb");  // binary DCL file
				else      // append an ext
					strcat(tpath, ".dcb");
				TempFilePath[0] = tpath;
			}
#ifdef _DCLDLL
      if (smodule->mdll && tmodule->mdll) {
				if (TempFilePath[0] == NULL)
    			TempFilePath[0] = ::_tempnam(NULL, "dcl");
    		DllCount = 2;
				DllArgs[0] = smodule->mdname; // conventional, not used by dll
				DllArgs[1] = "-b";	     // no leading space, binary for speed
				if (OutSwitch) {
					DllArgs[2] = OutSwitch;
					DllArgs[3] = "-o";	     // spec the output file first
					DllArgs[4] = TempFilePath[0];
					DllArgs[5] = rpath;
					DllArgCounts[0] = 6;
    			if ((hLibs[0] = LoadLibrary(DllArgs[0])) == NULL)
    				DllMissing = 1;
					DllArgs[6] = tmodule->mdname; // conventional, not used by dll
					DllArgs[7] = OutSwitch;
					DllArgs[8] = "-o";	     // spec the output file first
					DllArgs[9] = wpath;
					DllArgs[10] = TempFilePath[0];
					DllArgCounts[1] = 5;
					if ((DllMissing == 0)
  	  		 && ((hLibs[1] = LoadLibrary(DllArgs[6])) == NULL))
  					DllMissing = 7;
					sprintf(sysstr, "%s %s %s -o %s %s\"\n \"%s %s -o %s %s",
													 DllArgs[0], DllArgs[1], DllArgs[2], DllArgs[4], DllArgs[5],
													 DllArgs[6], DllArgs[7], DllArgs[9], DllArgs[10]);
				}
				else {
					DllArgs[2] = "-o";	     // spec the output file first
					DllArgs[3] = TempFilePath[0];
					DllArgs[4] = rpath;
					DllArgCounts[0] = 5;
    			if ((hLibs[0] = LoadLibrary(DllArgs[0])) == NULL)
    				DllMissing = 1;
					DllArgs[5] = tmodule->mdname; // conventional, not used by dll
					DllArgs[6] = "-o";	     // spec the output file first
					DllArgs[7] = wpath;
					DllArgs[8] = TempFilePath[0];
					DllArgCounts[1] = 4;
					if ((DllMissing == 0)
  	  		 && ((hLibs[1] = LoadLibrary(DllArgs[5])) == NULL))
  					DllMissing = 6;
					sprintf(sysstr, "%s %s -o %s %s\"\n \"%s -o %s %s",
													 DllArgs[0], DllArgs[1], DllArgs[3], DllArgs[4],
													 DllArgs[5], DllArgs[7], DllArgs[8]);
				}
  	  }
#else
			if (TempFilePath[0] == NULL)
   			TempFilePath[0] = tempnam(NULL, "dcl");
			if (OutSwitch)
				sprintf(sysstr, "%s -b %s -o %s %s ; %s %s -o %s %s",	   // always binary within a pipe
										 smodule->mname, OutSwitch, TempFilePath[0], rpath,
										 tmodule->mname, OutSwitch, wpath, TempFilePath[0]);
			else
				sprintf(sysstr, "%s -b -o %s %s ; %s -o %s %s",	   // always binary within a pipe
										 smodule->mname, TempFilePath[0], rpath,
										 tmodule->mname, wpath, TempFilePath[0]);
#endif
    }
    else if (smodule->mname) {			   // going into DCL
#ifdef _DCLDLL
      if (smodule->mdll) {
				DllCount = 1;
				DllArgs[0] = smodule->mdname;	 // conventional, not used by dll
				DllArgs[1] = (tmodule->mopt == 1) ? "-b" :	// no leading space, never empty
					           ((tmodule->mopt == 2) ? "-x" : "-a");
				if (OutSwitch) {
					DllArgs[2] = OutSwitch;
					DllArgs[3] = "-o";	     // spec the output file first
					DllArgs[4] = wpath;
					DllArgs[5] = rpath;
					DllArgCounts[0] = 6;
					sprintf(sysstr, "%s %s %s -o %s %s",
													DllArgs[0], DllArgs[1], DllArgs[2], DllArgs[4], DllArgs[5]);
				}
				else {
					DllArgs[2] = "-o";	     // spec the output file first
					DllArgs[3] = wpath;
					DllArgs[4] = rpath;
					DllArgCounts[0] = 5;
					sprintf(sysstr, "%s %s -o %s %s",
													DllArgs[0], DllArgs[1], DllArgs[3], DllArgs[4]);
				}
				if ((hLibs[0] = LoadLibrary(DllArgs[0])) == NULL)
					DllMissing = 1;
	    }
#else
			if (OutSwitch)
		    sprintf(sysstr, "%s %s %s -o %s %s",
		                 smodule->mname, binary, OutSwitch, wpath, rpath);
			else
		    sprintf(sysstr, "%s %s -o %s %s",
		                 smodule->mname, binary, wpath, rpath);
#endif
    }
    else if (tmodule->mname) {	 		   // coming from DCL
#ifdef _DCLDLL
      if (tmodule->mdll) {
				DllCount = 1;
				DllArgs[0] = tmodule->mdname; // conventional, not used by dll
				if (OutSwitch) {
					DllArgs[1] = OutSwitch;
					DllArgs[2] = "-o";	     // spec the output file first
					DllArgs[3] = wpath;
					DllArgs[4] = rpath;
					DllArgCounts[0] = 5;
					sprintf(sysstr, "%s %s -o %s %s",
												 DllArgs[0], DllArgs[1], DllArgs[3], DllArgs[4]);
				}
				else {
					DllArgs[1] = "-o";	     // spec the output file first
					DllArgs[2] = wpath;
					DllArgs[3] = rpath;
					DllArgCounts[0] = 4;
					sprintf(sysstr, "%s -o %s %s",
												 DllArgs[0], DllArgs[2], DllArgs[3]);
				}
		    if ((hLibs[0] = LoadLibrary(DllArgs[0])) == NULL)
		      DllMissing = 1;
	    }
#else
			if (OutSwitch)
	      sprintf(sysstr, "%s %s %s -o %s %s",
		                 tmodule->mname, binary, OutSwitch, wpath, rpath);
			else
	      sprintf(sysstr, "%s %s -o %s %s",
		                 tmodule->mname, binary, wpath, rpath);
#endif
    }
    else {								   // dcx conversion of DCL
#ifdef _DCLDLL
      DllCount = 1;
  	  DllArgs[0] = "dcx.dll";	   // conventional, not used by dcx
			DllArgs[1] = (tmodule->mopt == 1) ? "-b" :	// no leading space, never empty
					         ((tmodule->mopt == 2) ? "-x" : "-a");
  	  DllArgs[2] = "-o";	   // spec the output file first
  	  DllArgs[3] = wpath;  // skip the leading redirection " > "
  	  DllArgs[4] = rpath;  // skip the leading redirection " < "
  	  DllArgCounts[0] = 5;
  	  if ((hLibs[0] = LoadLibrary(DllArgs[0])) == NULL)
  	    DllMissing = 1;
      sprintf(sysstr, "%s %s -o %s %s",
                      DllArgs[0], DllArgs[1], DllArgs[3], DllArgs[4]);

#else
      sprintf(sysstr, "dcx %s -o %s %s", binary, wpath, rpath);
#endif
	  }

#ifdef _DCLDLL
  	if (DllCount != 0) {
		  if (cli.verbose()) {
        fprintf(cli.wfile() ? stdout : stderr,
                "%s: executing %d DLL%s:\n \"%s\"\n to convert from %s to %s.\n",
                argv[0], DllCount, DllCount > 1 ? "s" : "", sysstr, smodule->mappl, tmodule->mappl);
  	  }
  	  // run all the DLLs, let them handle error announcements
  	  for (DllNum = DllArgNum = 0; DllNum < DllCount; DllArgNum += DllArgCounts[DllNum++]) {
				if (DllMissing && ((DllMissing - 1) == DllArgNum))
					RetVal = ERRMAX - 1;  // no DLL found
    		else if ((pMain = (DCLdll) ::GetProcAddress(hLibs[DllNum], "DCLmain")) != NULL)
  	      RetVal = (*pMain)(DllArgCounts[DllNum], &DllArgs[DllArgNum]);
    		else RetVal = ERRMAX - 2;  // opened DLL but no DCLmain present
    		if (RetVal) {
  	  	  sprintf(DllMsg, "Error %d running DLL %s during:\n \"%s\"\n %s\n",
  		      RetVal, DllArgs[DllArgNum], sysstr, ErrMsg[(RetVal > ERRMAX) ? ERRMAX : RetVal]);
  		    MessageBox(NULL, DllMsg, "DCL Conversion Error", MB_OK);
  		  }
  	  }
      for (DllNum = 1; DllNum < DllCount; DllNum++) {
  	    unlink(TempFilePath[DllNum - 1]);
				if (TempFilePath[DllNum - 1] != tpath)
  				free(TempFilePath[DllNum - 1]);
  	  }
  	  for (DllNum = 0; DllNum < DllCount; DllNum++) // free all DLLs
#if 0
  		  if (DllNum < (DllCount - 1))  // don't free the last DLL
#endif
  		    ::FreeLibrary(hLibs[DllNum]); // may cause error R6018 and leave temp file behind

  	  continue;
  	}
#else
    if (cli.verbose())
      fprintf(cli.wfile() ? stdout : stderr,
              "%s: executing \"%s\"\n to convert from %s to %s.\n",
              argv[0], sysstr, smodule->mappl, tmodule->mappl);
    system(sysstr);
#endif
  }
  return RetVal;
}
 


void
DCcli::SetEndian(void)
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
DCcli::Proc(int argc, char **argv, char **outsw)
{
  struct stat stbuf;
  static path wpath;
  static path target;
  static bool Processed = false;
  static bool source_set = false;
  static bool target_set = false;
  bool source_verified = false;
  char *suf;
	char *fmt = NULL;
  mods *rmod;
  FILE *rf;
  char rbuf[4];
  short len;
	char ch = 0;

  rfn = wfn = NULL;

	if (argc == 1) {
    fprintf(stderr, "usage: %s [-abx] [-lmp] [-v] [-f format] [-s source] [-t target] [-o output] files...\n", argv[0]);
    fprintf(stderr, "  DCL format: -a ascii, -b binary, -x XDCL\n");
    fprintf(stderr, "  Endianness: -l little-endian (Intel), -m big-endian, -p PDP\n");
    fprintf(stderr, "  Verbosity: -v verbose\n");
		fprintf(stderr, "  Format: -f code, as in -fD for DITA; codes are:\n");
		// 'W', '8', 'P', '3', 'H', 'N', 'J', 'M', 'C', 'O', 'E', 'X', 'D', 'B', 'G'
		fprintf(stderr, "    RTF: W Word, 8 Word8 up, P WordPerfect, H WinHelp4, 3 WinHelp3\n");
    fprintf(stderr, "    *ML: N HTML, X XHTML, G XML, D DITA, B DocBook, M MS HTML Help,\n");
    fprintf(stderr, "      C OmniHelp, E Eclipse Help, J JavaHelp, O Oracle Help for Java\n");
    fprintf(stderr, "  Source: -s dcl, dcb, dcx, mif, lst, xml\n");
    fprintf(stderr, "  Target: -t dcl, dcb, dcx, rtf, htm, xml\n");
    fprintf(stderr, "  Output: -o .ext, -o \\path, or -o \\path\\file.ext\n");
    return false;
	}

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

 				// format ID, pass entire switch on to all DLLs (except dcx)
				case 'F':
				case 'f':
          if (argv[argpos][2] == 0) {
            if (((argpos + 1) < argc)
             && !isswitch(*argv[argpos + 1])) {
              fmt = argv[++argpos];
							*outsw = NewName("-f", strlen(fmt) + 2);
							strcat(*outsw, fmt);
						}
						else
							break;
          }
          else {
						fmt = &argv[argpos][2];
						*outsw = argv[argpos];
					}
					if (!target_set) {  // set tmodule, xfs, suf
						if (strlen(fmt) > 2) {  // new style -f switch
							if (!strnicmp(fmt, "htmlhelp", 8))
								ch = 'M';
							else if (!strnicmp(fmt, "javahelp", 8))
								ch = 'J';
							else if (!strnicmp(fmt, "oraclehelp", 10))
								ch = 'O';
							else if (!strnicmp(fmt, "eclipse", 7))
								ch = 'E';
							else if (!strnicmp(fmt, "html", 4))
								ch = 'N';
							else if (!strnicmp(fmt, "xhtml", 5))
								ch = 'X';
							else if (!strnicmp(fmt, "xml", 3))
								ch = 'G';
							else if (!strnicmp(fmt, "docbook", 7))
								ch = 'B';
							else if (!strnicmp(fmt, "dita", 4))
								ch = 'D';
							else if (!strnicmp(fmt, "omnihelp", 8))
								ch = 'C';
							else if (!strnicmp(fmt, "rtf", 3))
								ch = '8';
							else if (!strnicmp(fmt, "wordperfect", 11))
								ch = 'P';
							else if (!strnicmp(fmt, "word", 4))
								ch = '8';
							else if (!strnicmp(fmt, "winhelp", 7))
								ch = 'H';
							else if (!strnicmp(fmt, "dcl", 3))
								ch = 'A';
							else
								ch = 'Z';
						}
						else
							ch = toupper(*fmt);

						target_set = true;
						switch (ch) {
							case 'M':
							case 'J':
							case 'O':
							case 'E':
							case 'X':
							case 'G':
							case 'B':
							case 'D':
							case 'C':
							case 'N':
								tmodule = &tmods[HTMTARGET];
								xfs = suf = "htm";
								break;

							case 'A':
								tmodule = &tmods[DCLTARGET];
								xfs = suf = "dcl";
								break;

							case 'H':
							case '3':
							case '4':
							case 'W':
							case '7':
							case '8':
							case 'P':
								tmodule = &tmods[RTFTARGET];
								xfs = suf = "rtf";
								break;

							default:
								target_set = false;
								break;
						}
					}
					break;

	      case 'L':
        case 'l':
          wtyp = flsb;
          break;
        case 'M':
        case 'm':
          wtyp = fmsb;
          break;

        case 'O':
        case 'o':
          if (argv[argpos][2] == 0) {  // -o filename
            if (((argpos + 1) < argc)
             && !isswitch(*argv[argpos + 1]))
              wfn = argv[++argpos];
          }
          else wfn = &argv[argpos][2];   // space omitted, use name here

          // categorize wfn as suffix, path, or filename
          if ((wfn[0] == '.') && !strchr(wfn, path_sep)) {  // assume suffix
            wfs = wfn + 1;
            wfn = NULL;
            if (!target_set) {  // try to set target format per suffix
							strncpy(rbuf, wfs, 3);
							rbuf[3] = 0;
              for (rmod = tmods; rmod->msuf != NULL; rmod++) {
                if (!_stricmp(rmod->msuf, rbuf)) {
                  tmodule = rmod;
                  break;
                }
              }
              if (rmod->msuf == NULL) {  // not in list, make up DLL name
                strncpy(txsuf, rbuf, XSUFMAX);
                strncpy(&txprog[XPROGPX], rbuf, XSUFMAX);
                tmodule = &txmod;
              }
            }
            break;
          }
          if (!stat(wfn, &stbuf) && S_ISDIR(stbuf.st_mode)) { // dir is path
            strcpy(wpath, wfn);
            if (wpath[(len = strlen(wpath)) - 1] != path_sep) {
              wpath[len] = path_sep;
              wpath[len + 1] = '\0';
            }
            wfn = NULL;
          }
          break;

        case 'P':
        case 'p':
          wtyp = fpdp;
          break;

        case 'S':
        case 's':
          if (argv[argpos][2] == 0) {
            if (((argpos + 1) < argc)
             && !isswitch(*argv[argpos + 1]))
              suf = argv[++argpos];
          }
          else suf = &argv[argpos][2];

          for (rmod = smods; rmod->msuf != NULL; rmod++) {
            if (!_stricmp(rmod->msuf, suf)) {
              smodule = rmod;
              source_set = true;
              break;
            }
          }
          if (rmod->msuf == NULL) {
            strncpy(sxsuf, suf, XSUFMAX);
            strncpy(&sxprog[XPROGPX], suf, XSUFMAX);
            smodule = &sxmod;
            source_set = true;
          }
          break;

        case 'T':
        case 't':
          if (argv[argpos][2] == 0) {
            if (((argpos + 1) < argc)
             && !isswitch(*argv[argpos + 1]))
              suf = argv[++argpos];
          }
          else suf = &argv[argpos][2];
					if ((len = strlen(suf)) > 3)
						len = 3;  // so that htm matches html

          for (rmod = tmods; rmod->msuf != NULL; rmod++) {
            if (!_strnicmp(rmod->msuf, suf, len)) {
              tmodule = rmod;
              xfs = suf;
              target_set = true;
              break;
            }
          }
          if (rmod->msuf == NULL) {
            strncpy(txsuf, xfs = suf, XSUFMAX);
            strncpy(&txprog[XPROGPX], xfs, XSUFMAX);
            tmodule = &txmod;
            target_set = true;
          }
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
          fprintf(stderr, "%s: unknown option %s\n", argv[0], argv[argpos]);
          return false;
      }
    }
    else {  // source file name, so not in a pipe
      Processed = true;
      rfn = argv[argpos];

      // determine source file type
      if (!source_set) {
        if ((rf = fopen(rfn, "rb")) == NULL) {
          fprintf(stderr, "%s: cannot open source file %s\n", argv[0], rfn);
          return false;
        }
        fread(rbuf, 1, 4, rf);
        fclose(rf);
        source_verified = false;
				char *ssuf = strrchr(rfn, '.');
				if (ssuf) {
					ssuf++;
					int i = 0;
					for (rmod = smods; rmod->msuf != NULL; rmod++) {
						i++;
						if (!strcmp(rmod->msuf, ssuf)
						 && (!memcmp(rmod->mstart, rbuf, 4)
						  || ((i >= DITASRC) && (i < (DITASRC + DITACNT))))) {
							smodule = rmod;
							source_verified = true;
							break;
						}
					}
				}
				if (!source_verified) {
					for (rmod = smods; rmod->msuf != NULL; rmod++) {
						if ((rmod->mstart != NULL)
						 && !memcmp(rmod->mstart, rbuf, 4)) {
							smodule = rmod;
							source_verified = true;
							break;
						}
					}
				}
        if (!source_verified)  // try for GPO if not identified
          smodule = &smods[GPOSRC];
      }
      if (!smodule->mavail) {
        fprintf(stderr, "%s: cannot process %s files like %s yet\n",
                         argv[0],           smodule->mappl, rfn);
        rfn = wfn = NULL;
        continue;
      }

      // generate target file name if necessary
      if (wfn == NULL) {
        if (wpath) {
          strcpy(target, wpath);
          char *cpt = strrchr(rfn, path_sep);
          strcat(target, (cpt == NULL) ? rfn : (cpt + 1));
        }
        else strcpy(target, rfn);
        rfs = smodule->msuf;
        xfs = tmodule->msuf;
        if (!_stricmp("doc", &target[len = strlen(target) - 3])
				 || ((!strcmp("000", &target[len])) && (!strcmp("gpo", rfs))) 
         || !_stricmp(rfs, &target[len = strlen(target) - strlen(rfs)])) {
          if (target[len - 1] != '.')
            target[len++] = '.';
          strcpy(&target[len], wfs ? wfs : xfs);
        }
        else {
          strcat(target, ".");
          strcat(target, wfs ? wfs : xfs);
        }
        wfn = target;
      }
      if (!tmodule->mavail) {
        fprintf(stderr, "%s: cannot produce %s files like %s yet\n",
                         argv[0],           tmodule->mappl, wfn);
        rfn = wfn = NULL;
        continue;
      }

      // ensure source and target files are not the same
#ifdef DOSSTAT
      if (!stat(wfn, &stbuf) && !_stricmp(rfn, wfn)) { // target & source same name
#else
      stat(rfn, &stbuf);
      unl rdev = stbuf.st_dev;
      unl rino = stbuf.st_ino;
      int wst = stat(wfn, &stbuf);
      unl wdev = stbuf.st_dev;
      unl wino = stbuf.st_ino;
      if ((wst == 0) && (rdev == wdev) && (rino == wino)) { // target is source
#endif
        fprintf(stderr, "%s: cannot overwrite source file %s\n", argv[0], rfn);
        rfn = wfn = NULL;
        continue;
      }
      argpos++;
      return true;
    }
  }

  if (Processed)  // pipes are only used once
    return false;
  Processed = true;
  return true;  // if no source file, must be using pipe
}


// end of dcl.cpp



