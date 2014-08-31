
// dwrtffl.h is the header for rtf file writing classes, JHG, 4/19/97
// Copyright (c) 1993-1997 by Omni Systems, Inc.  All rights reserved.

#ifndef DWRTFFL_H
#define DWRTFFL_H


class RTFwfile : public DCwfile {
  short LinePos;
	bool LastGE;
	bool AutoCR;

	char Meta;
	int metanamewid;
	int metanumwid;
	char metaname[PATHMAX];
	short metanum;
	DCLwfile *mfile;

  void CheckHot(void)
    { if (LinePos > LineHot) putEnd(); }

 public:
  RTFwfile(const char *fname);
  virtual void CloseFile(void);

  virtual int putChar(int ch);
  virtual int putStr(const char *s, size_t len = 0);
  void putShort(short n, bool neg = false);
  void putLong(long n, bool neg = false);

  void putEnd(void);
	void SetCR (bool cr) { AutoCR = cr; }
  void putHexB(int ch);
  void putText(int ch);

  void putCS(int ch);
  void putCW(const char *s);
  void putCWN(const char *s, short n, bool neg = false);

  void putGS(void);
  void putGE(bool spset = false);
  void putSpec(unc ch);
	void putField(char *fname, char *ftext = NULL, bool fmerge = true,
    char *foptions = NULL, char *fresult = NULL, bool wrap = true,
	  bool lock = false);

	void startMeta(char mt, int namewid = 0, int numwid = 0);
	void endMeta();
	void putMetaName()
		{ putStr(metaname); }  // write last metafile name into file
	char *getMetaName()
		{ return metaname; }

  void putMB(int ch);
  void putMS(short s);
  void putML(long l);
  void putMEnd(void)
		{ if (Meta == 'I') putEnd(); }
  void putMChar(int ch)
		{ if (Meta == 'I') putChar(ch); }

	long NewMDiv(void);
  void SelectMDiv(long div = 0);
  void WriteMDiv(long div = 0);
  void DeleteMDiv(long div = 0);

  enum WrtfFileConstants { LineHot = 70, LineMax = 78 };

  DCblock& operator= (DCblock& b) { putBlock(b.p(), b.l()); return b; }
};

#endif

