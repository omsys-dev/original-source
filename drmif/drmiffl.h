
// drmiffl.h is the header for dcl FrameMaker MIF file reading, JHG
// Copyright (c) 1993-1997 by Omni Systems, Inc.  All rights reserved.

#ifndef DRMIFFL_H
#define DRMIFFL_H


class MIFrfile;

typedef void (*logfunc)(logtype lt, unc sev, const char *desc,
 const char *d2, const char *d3, const char *d4, const char *d5);


// classes

class MIFrfile: public DCrfile {
  friend class MIFrd;
  unc MIFver;         // MIF version major number, 1... (not '1'...)
	char *FMver;        // FrameMaker version identifier
  mifunit CurrUnits;  // set by getVal, returned by getUnit
  mifunit SetUnits;   // set by Units statement
  DCblock *MIFraster; // holds last raster read
  DCblock *MIFvector; // holds last vector read
  DCblock *MIFepsi;   // holds last epsi object read
  rastyp MIFrastype;  // specifies type of last raster read
  vectyp MIFvectype;  // specifies type of last vector read
  long MIFlines;      // line count in epsi object

 public:
  MIFrfile(char *fname = NULL);

  static char *ErrMsg[];
  virtual void Err();

  void skipStatement(); // skips thru next '>' and whitespace
  void skipString();    // skips thru next '\''
  void getImport();     // gets entire import object
  void skipImport();    // skips import object lines
  char *getWord();      // these two point at static bufs,
  char *getString(long len = LineMax); // set len > LineMax or 
                        // copy elsewhere if persistence needed
  long getVal();        // sets CurrUnits
  mifunit getUnit() { return CurrUnits; }

	static void LogEvent(logtype lt, unc sev, const char *desc,
	              const char *d2 = NULL, const char *d3 = NULL,
	              const char *d4 = NULL, const char *d5 = NULL);
	static logfunc LogEventFunc;
};



#endif


