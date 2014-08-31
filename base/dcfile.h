
// dcfile.h is the header for dcl file classes, JHG
// Copyright (c) 1993-2007 by Omni Systems, Inc.  All rights reserved.

#ifndef DCFILE_H
#define DCFILE_H


// dcl file handling

enum dc_fmod { mrd = 0, mwr, mrw, mwa, mno };
enum dc_ferr { fok = 0, fno, fform, frd, fwr };

enum FileConstants {
	TextBufMax = 8192
};


class DCfile {
 protected:
  char *fname;
  dc_ftyp ftype;
	dc_fmod fmode;

 	char basepath[PATHMAX];
	char basename[PATHMAX];

  void OpenFile(const char *fn = NULL, dc_fmod fm = mno);

 public:
  DCfile(const char *fn = NULL, dc_fmod fm = mrd);
  virtual ~DCfile();

  dc_ferr Ferr;
  FILE *fptr;
	long fpos;
  virtual dc_ferr FErr() { return Ferr; }

	virtual void ReopenFile(const char *fn, bool close = true);
  virtual void CloseFile();

 	char *getFilePath(void)
		{ return fname; }

 	char *getFileName(void)
		{ return fname + strlen(basepath); }

 	char *getBaseName(void)
		{ return basename; }

 	char *getBasePath(void)
		{ return basepath; }

  virtual long tell();
  virtual int seek(long pos, int mode = SEEK_SET);
	virtual long getSize();
};

#endif

