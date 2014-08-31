
// dcbase.h is the header for dcl base classes, JHG, 3/13/93
// Copyright (c) 1993 by Omni Systems, Inc.  All rights reserved.

#ifndef DCBASE_H
#define DCBASE_H



enum dc_dtyp {
  ibyte = 0, ishort, ilong, imnem,
  ebyte, eshort, elong, edoub,
  ename, etext, ecode, stamp,
  res12, res13,
  endgroup, group, dtyps
};


class DCctl {
 friend class DCblock;
 protected:
  dc_dtyp dtype;
  unc major, minor;
  uns data;
  unl size;
  void *ext;
 public:
  DCctl() { set(ilong); }
  DCctl(dc_dtyp dtp, uns mj = 0, uns mn = 0, uns d = 0, unl s = 0)
    { dtype = dtp; major = (unc) mj; minor = (unc) mn; data = d; size = s; ext = NULL; }
  DCctl(DCctl& c)
    { dtype = c.dtype; major = c.major; minor = c.minor; data = c.data;
      size = c.ext ? 0 : c.size; ext = NULL; }
  DCctl(DCLrbfile& rf) { *this = rf; }
  virtual ~DCctl() { if (ext) delete [] ext; }

  // access
  dc_dtyp dt() { return dtype; }
  dc_dtyp dt(dc_dtyp d) { return (dtype = d); }
  uns maj() { return (uns) major; }
  uns maj(uns m) { return (uns) (major = (unc) m); }
  uns maj(char *n)
    { return (uns) (major = (unc) getMajor(n)); }
  uns min() { return (uns) minor; }
  uns min(uns m) { return (uns) (minor = (unc) m); }
  uns min(char *n, uns m = 0)
    { return (uns) (minor = (unc) getMinor(n, m)); }
  uns dat() { return data; }
  uns dat(uns d) { return (data = (uns) d); }
  uns dat(char *n, uns mn = 0, uns mj = 0)
    { return (data = getData(n, mn, mj)); }
  unl siz() { return size; }
  unl siz(unl s) { return (size = s); }
  unl siz(char *n, uns d = 0, uns mn = 0, uns mj = 0)
    { return (size = getSize(n, d, mn, mj)); }
  void *ex() { return ext; }
  void *ex(const void *e, size_t l = 0);
  void *ex(const char *e);

  // overloaded operators
  DCctl& operator= (DCctl& c);
  DCctl& operator= (DCLrbfile& rf);
  DCctl& operator= (DCblock& b);

  // others
  DCctl& set(dc_dtyp dtp, uns mj = 0, uns mn = 0, uns d = 0, unl s = 0)
    { dtype = dtp; major = (unc) mj; minor = (unc) mn; data = d; size = s;
      ext = NULL; return *this; } // only for new ctls, so ext was unused
  void reset()  // use this before using set() to recycle existing ctls
    {if (ext) delete [] ext; set(ilong); }

	virtual bool match(uns mj, uns mn, uns sdat = 0, unl siz = 0);
	virtual short cmp(DCctl& cp);
  virtual void write(DCLwfile& wf, long lev = 0);

	void append(char *pt);

  void aprint();
  void nprint();

  uns getMajor(char *name);
  char *getMajor(uns val = 0);
  uns getMinor(char *name, uns majval = 0);
  char *getMinor(uns val = 0, uns majval = 0);
  uns getData(char *name, uns minval = 0, uns majval = 0);
  char *getData(uns val, uns minval = 0, uns majval = 0);
  unl getSize(char *name, uns dval = 0, uns minval = 0, uns majval = 0);
  char *getSize(unl val, uns dval = 0, uns minval = 0, uns majval = 0);
};

#endif


