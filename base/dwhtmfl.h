
// dwhtmfl.h is the header for htm file writing classes, JHG
// Copyright (c) 1996-1998 by Omni Systems, Inc.  All rights reserved.

#ifndef DWHTMFL_H
#define DWHTMFL_H


class HTMwfile : public DCwfile {

  enum WhtmFileConstants { HtmLevelMax = 50, LineHot = 70, LineMax = 78 };

  short LinePos;
	char *ClassAttrName;

	long AttrCount;
	DCnlist AttrNames;
	DCnlist AttrVals;

	long SavedAttrCount;
	DCnlist SavedAttrNames;
	DCnlist SavedAttrVals;

	char *AttrName;
	long AttrDiv;
	long PreAttrDiv;
	bool InAttrs;
	bool AttrAdd;
	bool EmptyAttr;
	void startAttrs(void);
	void endAttrs(void);
	void WriteAttrs(void);

 public:
  HTMwfile(char *fname);
  virtual void ReopenFile(char *fname);

	bool PreForm;
	bool NoWrap;
	bool Xhtml;
	bool Xml;

	void SetXhtml(bool val) { Xhtml = val; }
	void SetXml(bool val) { Xml = val; }
	void SetNoWrap(bool val) { NoWrap = val; }
	void SetPreForm(bool val) { PreForm = val; }
	void SetAttrAdd(bool val) { AttrAdd = val; }
	void SetEmptyAttr(bool val) { EmptyAttr = val; }

	void SaveAttrs(void);
	void RestoreAttrs(void);

#if 0
  int putChar(int ch)
    { LinePos++; return DCwfile::putChar(ch);}
  int putHexB(int ch)
    { putHex((ch >> 4) & 0xF); return putHex(ch & 0xF); }
  int putStr(char *s)
    { size_t len = strlen(s); LinePos += len; return DCwfile::putStr(s, len); }
  int putSpec(int ch)
    { putChar('&'); putChar('#'); putShort(ch & 0xFF); return putChar(';'); }
  int putEntity(char *str)
    { putChar('&'); putStr(str); return putChar(';'); }
  int putEnd(void)
    { LinePos = 0; return DCwfile::putCr(); }
#else
  int putChar(int ch);
  int putHexB(int ch);
  int putStr(char *s, size_t len = 0);
  int putSpec(int ch);
  int putEntity(char *str);
  int putEnd(void);
#endif

  void putText(int ch);
  void putTextStr(char *s);
  void putEscStr(char *s, size_t len = 0);
  void putShort(short n);
  void putUShort(uns n);
  
	void putElem(char *str, bool block = false);
	void startElem(char *str);
	void endElem(bool block = false);
	void putXML(char *enc = NULL, char *ver = NULL);
	void putDT(char *doc, char *dtype, char *dtd, bool rets = true, bool ents = false);
	void startPI(char *str);
	void endPI(void);
	void startComment(void);
	void endComment(void);
	void putComment(char *str);
	void startError(void);
	void endError(void);
	void putError(char *str);

	void startAttr(char *attr);
	void endAttr(void);
	void putAttrValue(char *str);
	size_t IsEntity(char *str);
	void putAttrStr(char *str);
	void strAttr(char *attr, char *str);
	void numAttr(char *attr, short num, char *str = NULL);
	void pctAttr(char *attr, short num);
	void pxAttr(char *attr, short num);
	void starAttr(char *attr, short num);
	void numPlusAttr(char *attr, short num);
	void numMinusAttr(char *attr, short num);
	void colorAttr(char *attr, long color);

	void endLine();

#ifdef BLOCKSTACK
	static int HTMlevel;
	static char *HTMstack[HtmLevelMax];
#endif

	void writeBlock(char *str, char *txt);
	void writeTextBlock(char *str, char *txt);
	void startBlock(char *str);
	void startSBlock(char *str);
	void endBlock(char *str);
	void putBlockEnd(char *str);

	typedef enum {PA_unknown = 0, PA_left, PA_center, PA_right} ParaAlign;
	void setAlign(ParaAlign align = PA_unknown);
	void setClass(char *clname);
	void setClassAttrName(char *clattrname);
	void endPara();

	void putAName(char *str, char *tag = NULL, bool linebreak = true);
	void putAHref(char *str, char *targ = NULL, bool linebreak = true);

  void putBlock(void *v, size_t s)
    { if (fwrite(v, 1, s, fptr) != s) { Ferr = fwr; Err(); } }

  DCblock& operator= (DCblock& b) { putBlock(b.p(), b.l()); return b; }
};

#endif

