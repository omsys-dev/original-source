
//dcfilr.h is the header for dcl file reading classes, JHG
// Copyright (c) 1993-1996 by Omni Systems, Inc.  All rights reserved.

#ifndef DCFILR_H
#define DCFILR_H


// class for general file reading

class DCrfile : public DCfile {
 public:
  DCrfile(const char *fname = NULL) : DCfile(fname, mrd) { }
  bool eof(void) { return (feof(fptr)); }

  static char *ErrMsg[];
  static dc_err ErrTypes[];
  virtual void Err(void);

	static bool FileExists(char *fname);

  int getChar(void) { return (Ferr ? EOF : fgetc(fptr)); }
  int ungetChar(int ch) { return (Ferr ? EOF : ungetc(ch, fptr)); }

  void getBlock(void *v, size_t s)
    { if (Ferr) return; if (fread(v, 1, s, fptr) != s) Ferr = frd; }
  bool getLine(char *p, size_t s);
  void skipLine(void);
  void skipWhite(void);

  short getAShort(void) { return (short) getALong(); }
  long getALong(void);
  unl getAULong(void);
  unl getABytes(void);
	double getADouble(void);

  short getIShort(void);
  long getILong(void);
  unl getIULong(void);

  short getMShort(void);
  long getMLong(void);
  unl getMULong(void);

  double getIDouble(void);
  double getMDouble(void);

  long getPLong(void);
  unl getPULong(void);

  enum DCrfileConstants { BytesMax = 8 };
};


// class for DCL binary file reading

class DCLrbfile : public DCrfile {

 protected:
  DCctl& getBCtl(DCctl& c);

 public:
  DCLrbfile(const char *fname = NULL);

  virtual DCctl& getCtl(DCctl& c)
    { return getBCtl(c); }
  void skipGroup(void);

  short (DCLrbfile::*getShort)(void);
  long (DCLrbfile::*getLong)(void);
  unl (DCLrbfile::*getULong)(void);
  double (DCLrbfile::*getDouble)(void);
};


// class for DCL ASCII or binary file reading

class DCLrfile : public DCLrbfile {
  void getExtBytes(DCctl& c);
  void getExtShorts(DCctl& c);
  void getExtLongs(DCctl& c);
  void getExtDoubs(DCctl& c);
  void getExtStr(DCctl& c);
  void getExtCode(DCctl& c);

  DCctl& getACtl(DCctl& c);

  void pushGroup(DCctl& c);
  void popGroup(DCctl& c);

 public:
  DCLrfile(const char *fname = NULL);

  virtual DCctl& getCtl(DCctl& c)
    { return DCnode::ProcNodeCtl((ftype == fasc) ? getACtl(c) : getBCtl(c)); }

  enum DCLrfileConstants { GroupMax = 128, TokenMax = 80 };
  static unc GNest;
  static DCctl GroupNest[GroupMax];
};

#endif

