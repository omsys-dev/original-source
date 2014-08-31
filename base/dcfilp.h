//dcfilp.h is the header for dcl file profile classes, JHG
// Copyright (c) 2001 by Omni Systems, Inc.  All rights reserved.

#ifndef DCFILP_H
#define DCFILP_H

class DCpfile;

// class for profile (.ini, .ref) file read and write

class DCpfile : public DCrfile {
 public:
  DCpfile(const char *fname = NULL) : DCrfile(fname)
	 {pfname = NewName(getFilePath()); }
  virtual ~DCpfile() { }

	char *pfname;
	DCnlist sections;
	bool changed;
	bool moverr;
	long fucked;

	static char *histitem;

	static char *valbuf;
	static unl valsize;
	
	unl pread(const char *sect, const char *key, char *val, const char *defval, unl vsize);

	DCnlist keylists;
	DCilist *pgetkeys(const char *sect);
	void prelkeys(const char *sect);
	static void DCpfile::delkeys(long v, long i);

	static void addval(const char *str, bool nl = true);
	bool pwrite(const char *sect, const char *key, const char *val);
	bool rsect(const char *sect, const char *nsect);

	void insertsethist(DClist *lines, long ni = 0);
	void insertsecthist(DCnlist *keys);

	unl preadall(const char *sect, const char *key, char *val, unl vsize);
	long putline(char *line, char *buf, unl blen, unl bsize);
	bool delall(const char *sect, const char *key);

	bool csect(const char *sect, const char *val);
	void addsectlines(const char *sect, DCnlist *keys, const char *val);
	bool istuple(const char *cpt);

	bool mvsect(const char *sect, long opos, long npos);
	bool mvset(const char *sect, const char *set, long opos, long npos);
	bool rset(const char *sect, const char *set, const char *nset);
	bool restore(const char *sect, const char *set);
	void fixend(void);

	static void dolist(const char *key, long );
	static char *secthead;
	static void doline(const char *key, long vp);

	static DCnlist *remsets(DCnlist *keys);

	bool pset(void);
	void addblank(DClist *lines, DClist *lastlines, long &bcnt);

	void pflush(void);

	static void pwsect(DCwfile *wf, char *n, long i);
	static void pwkey(DCwfile *wf, char *n, long i);
	static char *sectname;
	static char *setname;
	static void pwline(DCwfile *wf, long i);
	static void pflushx(const char *fname, long ptr);

 public:
	static DCnlist ProfileFiles;
	static bool ending;
	static DCpfile *MovErrFptr;
	static long MovErrVal;

	static unl getval(const char *sect, const char *key, const char *defval,
	 char *val, unl vsize, const char *fname);

	static unl getvalall(const char *sect, const char *key,
	 char *val, unl vsize, const char *fname);
	static unl deleteall(const char *sect, const char *key, const char *fname);

	static DCilist *Keylist;
	static DCilist *Keyptr;
	static char *KeyFile;
	static char *KeySect;
	static bool getkeys(const char *sect, const char *fname);
	static bool getnextkey(char **key, char **val);
	static void releasekeys(const char *sect, const char *fname);

	static bool putval(const char *sect, const char *key,	const char *val, const char *fname);
	static bool movsect(const char *sect, long opos, long npos, const char *fname);
	static bool movset(const char *sect, const char *set, long opos, long npos, const char *fname);
	static bool rensect(const char *sect, const char *nsect, const char *fname);
	static bool chsect(const char *sect, const char *val, const char *fname);
	static bool renset(const char *sect, const char *set, const char *nset, const char *fname);
	static bool rest(const char *sect, const char *set, const char *fname);

	static void psetup(const char *fname);
	static void pupdate(const char *fname);
	static bool pclose(const char *fname);
	static void pflushall(void);
	static void addhist(const char *hist);
};

#endif

// end of dcfilp.h

// putval syntax, extende from PPS protocol:

//  sect    key    val    result
//  ----    ---    ---    ------
//  NULL    any    any    Ignore
//  ""                    Ignore
//  new     NULL   NULL   Add section only
//                 new    Add section with val as comment or macro
//          new    NULL   Add section with empty key
//                 new    Add section, key and val 
//  old     NULL   NULL   Delete section
//                 new    Add val as comment or macro
//          ""     NULL   Delete all keys, leave comments
//                 new    Same but add val as comment or macro
//          old    NULL   Delete key
//                 new    Change key value to new
//          new    NULL   Add key only
//                 new    Add key and val

