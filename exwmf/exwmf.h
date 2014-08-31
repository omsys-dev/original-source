// exwmf.h is thhe header for the RTF Metafile Extractor for DCL, JHG
// Copyright (c) 1997 Omni Systems, Inc.  All rights reserved.

#ifndef EXWMF_H
#define EXWMF_H

// definitions

enum bool {false = 0, true };

typedef unsigned char unc;
typedef unsigned short uns;
typedef unsigned long unl;

#define PATHMAX 1024


// function prototypes

void ProcRtfFile(char *fname);
bool ProcField(char **fnames);
void GetMetafile(void);

bool ScanForCtrl(char *cname, int minnest = 1, long *argval = NULL);
char *GetNextCtrl(int minnest = 1);
char *GetRtfCtrl(long *argval = NULL);

uns CheckName(char *name, char **list);
char *ScanForName(int minnest = 1);


#endif

// end of exwmf.h

