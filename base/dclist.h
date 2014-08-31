
// dclist.h is the header for dcl list classes, JHG, 8/5/93
// Copyright (c) 1993 by Omni Systems, Inc.  All rights reserved.

#ifndef DCLIST_H
#define DCLIST_H


class DClist;
class DCilist;
class DCvlist;
class DCnlist;
class DCclist;


// class for simple lists

class DClist {
 public:
  long id;
  DClist *next;

  DClist() : id(0), next(NULL) { }
  DClist(long i) : id(i), next(NULL) { }
	DClist(DClist *lp);
  ~DClist() { }

	void init(long *lpt);
  void add(long i);
  DClist *append(long i);
  void add(DClist *lst);
  void insert(long i);
  void insert(DClist *lst);
	void insertafter(long val, long nitem);
	void insertbefore(long val, long nitem);
  void remove(long i);
  bool replace(long n, long i);
	bool substitute(long start, long end, DClist *rep);
	long dequeue(void);
	long pop(void);
  void all(void (*e)(long i));
  void writeall(DCwfile *wf, void (*e)(DCwfile *w, long i));
  DClist *find(long i);
  long number(long i);
	long position(long idval);
	bool fullmatch(DClist *lst);
	long count(void);
  void empty(void);
	char *strlist(void);
};
 
 

// class for associative lists of two numeric values

struct ipair {
	long id;
	long val;
};

class DCilist {
 public:
  long val;
  long id;
  DCilist *next;

  DCilist();
  DCilist(long v, long i) : id(i), next(NULL), val(v) { }
  DCilist(DCilist *lp);
  ~DCilist() { }

	void init(ipair *ip);
  void add(long v, long i);
  void append(long v, long i);
  void replace(long v, long i);
  void insert(long v, long i);
  void insertat(long v, long i, long at);
  void remove(long i);
	long dequeue(long *ip = NULL);
  long find(long i);
  long operator()(long v);
  long getitem(long v);
	bool match(long i, long v);
	long count(void);
  void empty(void);
	long number(long i, long *vp);
  void all(void (*e)(long v, long i));
  void write(long i, DCwfile *wf, void (*e)(DCwfile *w, long v));
  void writeall(DCwfile *wf, void (*e)(DCwfile *w, long v, long i));
};
 
 

// class for associative lists of a numeric value and a void pointer

struct vpair {
	long id;
	void *vp;
};

struct vlinfo {
	bool set;
	long cnt;
	DCvlist *last;
	DClist **ids;
	DClist **idnx;
};


class DCvlist {
 public:
  void *vp;
  long id;
  DCvlist *next;

	static long VLset;
	static long VLnumhash;
	static long VLnummask;

  DCvlist();
  DCvlist(void *v, long i) : id(i), next(NULL), vp(v) { }
  ~DCvlist() { }

	void hashadd(DCvlist *that);
	void init(vpair *vpr);
	void initdl(vpair *vpr);
  void add(void *v, long i);
  void append(void *v, long i);
	void insert(void *v, long i);
  void insertat(void *v, long i, long at);
  void remove(long i);
	void clear(bool arr = false);
	long count(void);
  void empty(void);
  void replace(void *v, long i);
	void *dequeue(void);
  void *find(long i, DCvlist **dvl = NULL);
  long operator()(void *v);
  long getitem(void *v);
  void all(void (*e)(void *v, long i));
  void execu(long i, void (*e)(void *v));
  void write(long i, DCfile *wf, void (*e)(DCfile *w, void *v));
  void writeall(DCfile *wf, void (*e)(DCfile *w, void *v));
  void writeallval(DCfile *wf, void (*e)(DCfile *w, void *v, long i));
};
 


// class for associative lists of a numeric value and a string

struct npair {
	long id;
	char *name;
};

struct nlinfo {
	bool set;
	long cnt;
	DClist **hashes;
	DClist **hashnx;
	DClist **ids;
	DClist **idnx;
};

class DCnlist {
 public:
  const char *name;
  long id;
	long val;
  DCnlist *next;

	static long NLset;
	static long NLstrhash;
	static long NLstrmask;
	static long NLnumhash;
	static long NLnummask;

  DCnlist();
  DCnlist(const char *n, long i) : id(i), next(NULL), name(n) { val = hash(name); }
  ~DCnlist() { }

	long hash(const char *nm);
	void hashadd(DCnlist *that);
	void init(npair *np);
  void clear(void);
  void addnew(const char *n, long i);
  void addlist(DCnlist *nl);
  void addlist(const char *n);
  void add(const char *n, long i);
  void addsorted(const char *n, long i);
  void addsortlex(const char *n, long i, void *cmp);
  void insert(const char *n, long i);
  void insertat(const char *n, long i, long at);
  void remove(long i);
  void replace(const char *n, long i);
	bool changeid(long n, long i);
	char *dequeue(long *ip = NULL);
	char *concat(void);
	long count(void);
  void empty(void);
  void all(void (*e)(char *n, long i));
  void all(void (*e)(const char *n, long i));
  void writeall(DCwfile *wf, void (*e)(DCwfile *w, char *n, long i));
  void writeallname(DCwfile *wf, void (*e)(DCwfile *w, char **n, long i));
  char *find(long i, DCnlist **dnl = NULL);
  long getnum(const char *n, DCnlist **dnl = NULL);
  long operator()(const char *n, DCnlist **dnl = NULL);
  long checksuffix(const char *n, DCnlist **dnl = NULL);
  long getitem(const char *n, DCnlist **dnl = NULL,
			         bool usehash = true, bool getnext = true);
	long getallwild(const char *n, DCnlist *results);
	long getallwildval(const char *n, DCnlist *results);
	long getwild(const char *n, DCnlist **dnl = NULL);
	long getwildrev(const char *n, DCnlist **dnl = NULL);
	bool wildmatch(const char *cl, const char *name);
  long getfile(const char *n, DCnlist **dnl = NULL);
};

 
class DCmlist: public DCnlist {
 public:
	char *branch;
	static DClist multpend;
	static long multcount;
	static long multpos;

  DCmlist() : DCnlist() { };
  DCmlist(char *n, long i, char *b = NULL);
  ~DCmlist() { }

  void addmult(char *n, long i, char *b);
	char *findmult(long i, char **bpt);
	long getmult(const char *n, const char *b);
	long multitem(char **bpt);
};


// class for associative lists of a numeric value and a ctl

class DCclist {
 public:
  DCctl *cp;
  long id;
  DCclist *next;

  DCclist();
  DCclist(DCctl *c, long i) : id(i), next(NULL), cp(c) { }
  ~DCclist() { }

  void add(DCctl *c, long i);
  void add(DCclist *cl);
	void remove(long i);
	void remove(DCctl *c);
	DCctl *dequeue(void);
	long count(void);
  void empty(void);
  void all(void (*e)(DCctl *c, long i));
  void allval(void (*e)(DCctl *c, long i, long v), long val);
	bool any(void) { return (next ? true : false); }
  DCctl *find(long i);
	long finddat(uns i);
	DCctl *finddatcp(uns i);
  long operator()(DCctl *c);
  void write(long i, DCLwfile& wf);
};
 



#endif


