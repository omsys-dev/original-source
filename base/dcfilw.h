
//dcfilw.h is the header for dcl file writing classes, JHG, 6/15/97
// Copyright (c) 1993-2007 by Omni Systems, Inc.  All rights reserved.

#ifndef DCFILW_H
#define DCFILW_H

class DCdiv;
class DCwfile;
class DCLwfile;


class DCdiv {
 friend class DCwfile;

 protected:
  static const size_t alsize;

  char *start;
  char *ptr;
  size_t len;
  size_t avail;

 public:
  DCdiv() { start = ptr = NULL; len = avail = 0; }
  ~DCdiv() { if (start) DeleteMem(start); }

  void Write(const char *p, size_t s);
  char *Read(void) { return start; }
  size_t Used(void) { return (len - avail); }
	void Clear(void);
};


class DCwfile : public DCfile {
 protected:
  DCdiv *CurrDiv;
  DCvlist DivList;
	long LastDiv;

 	FILE* fp;
  dc_ferr Fr;

	DCfile* aux;
	int auxnamewid;
	int auxnumwid;
	char auxname[PATHMAX];
	short auxnum;

 public:
	char *wfname;
  DCwfile(const char *fname, dc_ftyp ft, dc_fmod fm = mwr);
  virtual ~DCwfile();

	virtual void ReopenFile(const char *fn, bool close = true);
	virtual void CloseFile();

  virtual void nextFile(void);

  static char *ErrMsg[];
  static dc_err ErrTypes[];
  void Err(void);
  void CheckErr(void)
    { if (ferror(fp)) { Fr = fwr; Err(); } }

	// file writing primatives
  virtual int putChar(int ch);
  virtual int putCr(void);
  virtual int putHex(int ch);
  virtual int putNum(long n);
  virtual int putStr(const char *s, size_t len = 0);
  virtual int putQStr(const char *s, size_t len = 0, int quo = '"');
  virtual void putBlock(const void *v, size_t s);

	// overrides to handle aux file
  virtual long tell();
  virtual int seek(long pos, int mode = SEEK_SET);
	virtual long getSize();

  // diversions
	long NewDiv(void);
  void SelectDiv(long div = 0);
  void WriteDiv(long div = 0);
  void DeleteDiv(long div = 0);
	long GetDiv(void) { return CurrDiv ? DivList(CurrDiv) : 0L; }
	char *ReadDiv(void) { return CurrDiv ? CurrDiv->Read() : NULL; }
	size_t LenDiv(void) { return CurrDiv ? CurrDiv->Used() : 0L; }
	void ClearDiv(void) { if (CurrDiv) CurrDiv->Clear(); }

	// auxiliaries
	short StartAux(char *ext, short num = 0, int namewid = 0, int numwid = 0, long subnum = 0);
	short EndAux();
	bool StartAuxFile(char *aname, bool addpath = false);
	void EndAuxFile();
 	char *getAuxName(void)
		{ return auxname; }

	// numeric writing
  int putIShort(short i);
  int putILong(long l);
  int putIULong(unl l);
  int putIDouble(double d);

  int putMShort(short i);
  int putMLong(long l);
  int putMULong(unl l);
  int putMDouble(double d);

  int putPLong(long l);
  int putPULong(unl l);

  DCblock& operator= (DCblock& b) { putBlock(b.p(), b.l()); return b; }
};


class DCLwfile : public DCwfile {
 protected:
  int putAShort(short s);
  int putALong(long l);
  int putAULong(unl l);

  void putExtBytes(DCctl& c);
  void putExtShorts(DCctl& c);
  void putExtLongs(DCctl& c);
  void putExtDoubs(DCctl& c);

  void putExtStr(DCctl& c);
  void putExtCode(DCctl& c);

  void putExtXStr(DCctl& c);
  void putExtXCode(DCctl& c);

  DCctl& putACtl(DCctl& c);
  DCctl& putBCtl(DCctl& c);
  DCctl& putXCtl(DCctl& c);

 public:
  DCLwfile(char *fname, dc_ftyp ft);
  virtual void CloseFile(void);
	virtual void ReopenFile(const char *fn);
  virtual void nextFile(void);
  void PutDclHeader(void);
  void PutDclFooter(void);

	int putBShort(short i)
		{ return putIShort(i); }
	int putBLong(long l)
		{ return putILong(l); }
	int putBULong(unl l)
		{ return putIULong(l); }
	int putBDouble(double d)
		{ return putIDouble(d); }

  enum WFileConstants { TokenMax = 80 };
  int (DCLwfile::*putShort)(short i);
  int (DCLwfile::*putLong)(long l);
  int (DCLwfile::*putULong)(unl l);
  int (DCLwfile::*putDouble)(double d);

  DCctl& putCtl(DCctl& c)
    { return ((ftype == fasc) ? putACtl(c) :
		          ((ftype == fxml) ? putXCtl(c) : putBCtl(c))); }

  DCctl& operator= (DCctl& c) { return putCtl(c); }
  DCblock& operator= (DCblock& b) { putBlock(b.p(), b.l()); return b; }
};

#endif

