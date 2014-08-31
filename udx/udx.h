// udx.h is the main header file for udx.exe, which converts uDoc
// files between the full tagged and tag minimized forms
// Copyright 2013 by Jeremy H. Griffith, under Apache license

#ifndef UDX_H
#define UDX_H


// classes needed:
// UDnlist with add, find, getitem, no hash
// UDfile, UDrfile, UDwfile, UDifile
// UDutf for UTF-8 r/w

enum udvals {
	LineMax = 1024
};


struct lpair {
	long id;
	char *name;
};

struct vpair {
	void *id;
	char *name;
};

struct npair {
	char *id;
	char *name;
};

struct setting {
	char *key;
	char *val;
};

class UDlist {
 public:
  const char *name;
  long id;
  UDlist *next;

	UDlist() : name(NULL), id(0), next(NULL) { }
  UDlist(const char *n, long i) : id(i), next(NULL), name(n) { }
	~UDlist() { }

	void init(lpair *np);
	void add(const char *n, long i);
  void replacename(const char *n, long i);
  void replaceval(const char *n, long i);
  char *find(long i);
  long getitem(const char *n);
  void remove(const char *n);
  void remove(long i);
  void empty(void);
};

class UDvlist {
 public:
  const char *name;
  void *id;
  UDvlist *next;

	UDvlist() : name(NULL), id(0), next(NULL) { }
  UDvlist(const char *n, void *i) : id(i), next(NULL), name(n) { }
	~UDvlist() { }

	void init(vpair *np);
	void add(const char *n, void *i);
  void replacename(const char *n, void *i);
  void replaceval(const char *n, void *i);
  char *find(void *i);
  void *getitem(const char *n);
  void remove(const char *n);
  void remove(void *i);
  void empty(void);
};

class UDnlist {
 public:
  const char *name;
  char *id;
  UDnlist *next;

	UDnlist() : name(NULL), id(NULL), next(NULL) { }
  UDnlist(const char *n, char *i) : id(i), next(NULL), name(n) { }
	~UDnlist() { }

	void init(npair *np);
	void add(const char *n, char *i);
  void replacename(const char *n, char *i);
  void replaceval(const char *n, char *i);
  char *find(char *i);
  char *getitem(const char *n);
  void remove(const char *n);
  void remove(char *i);
  void empty(void);
};

class UDilist {
 public:
  long val;
  long id;
  UDilist *next;

	UDilist() : val(0), id(0), next(NULL) { }
  UDilist(long n, long i) : val(n), id(i), next(NULL) { }
	~UDilist() { }

  void add(long n, long i);
  long find(long i);
  void remove(long i);
};


class UDini {
 protected:
	static char *DefaultIniFileName;
	static char *SectNames[];
	static vpair InitNames[];

	static vpair udxFullTaggingOptions[];
	static npair ShortTagContainers[];
	static npair ListShortTagReplacement[];
	static npair PairListShortTagReplacement[];
	static npair TableShortTagReplacement[];
	static npair ListShortTagEnds[];
	static npair PairListShortTagEnds[];
	static npair TableShortTagEnds[];
	static npair WikiSymbolReplacement[];
	static vpair udxTagMinimizingOptions[];
	static npair ListLongTagReplacement[];
	static npair PairListLongTagReplacement[];
	static npair TableLongTagReplacement[];
	static npair WikiTagReplacement[];
	static npair CodeElements[];
	static npair ElementTypes[];

	char *IFname;
	UDvlist *sections;

 public:
	UDini();
	UDini(char *fname);
	~UDini() { }

	void Setup(void);
	void Process(FILE *fini);
	char *GetVal(char *sect, char *set);
	bool GetBool(char *sect, char *set, char *stype = NULL);
	UDnlist *GetSect(char *sect, char *stype = NULL);
};


// globals

void CheckUdxTag(pair **attrs, long attrcnt);
void CheckUdxEnd(void);
int FileInfo(char *fname);

extern bool DoFull;
extern bool DoMin;
extern bool NoFull;
extern bool NoMin;

extern bool DoPara;
extern bool DoBlank;
extern bool DoListShort;
extern bool DoPairShort;
extern bool DoTableShort;
extern bool DoWiki;

extern bool DoParaM;
extern bool DoBlankM;
extern bool DoListShortM;
extern bool DoPairShortM;
extern bool DoTableShortM;
extern bool DoWikiM;

extern char *OutArg;
extern char *ErrArg;

extern bool RenameOfile;

extern UDini *IniFile;

extern FILE *InFile;
extern FILE *OutFile;
extern char *SwitchHelp;

void ShowHelp(void);
void ProcessIni(void);
void ProcessFile(char *fname);



extern long CurrDepth;
extern long LastFullEndDepth;
extern long NestingLevel;
extern bool EmptyTag;
extern bool ShortTag;
extern char *PendingText;
extern unc InCode;
extern char *WikiLT;
extern char *WikiGT;
extern char *StartTextPend;
extern char *EndTextPend;
extern bool WrappedText;
extern bool IsPTag;
extern bool EndingPTag;
extern bool SkippedPTag;

extern char *CurrElem;
extern long CurrEtype;
extern UDlist *CurrElems;
extern UDilist *CurrEtypes;

extern UDnlist *CurrSTR;
extern UDnlist *CurrSTE;
extern UDnlist *CurrLTR;
extern UDnlist *CurrW2T;
extern UDnlist *CurrT2W;

extern UDilist *Nesting;
extern UDlist *CurrStype;
extern UDlist *CurrShort;
extern UDlist *CurrShortEnd;
extern UDlist *CurrFull;

void StartTag(unl *tag, long len, pair **attrs, long attrcnt);
void WriteStartTag(char *name, pair **attrs, long attrcnt);

void EndTag(unl *tag, long len);
long GetCurrEtype(char *name);
void WriteEndFullTag(void);

extern UDilist *WikiSymsUsed;
void ProcChar(unl *str, long len);
bool IsCodeTag(char *tag);
void ParseWikiTag(char *wval, char **wtag,
		           char **sch, char **ech);


void ProcComment(char *cmt);
char *WriteTextWithoutTrailingSpace(char *ptext, bool lead = false);

void WriteText(char *txt);
void WriteCode(char *txt);
bool WriteWikiSym(char *name, bool start);
void AddStartTextPend(char *txt);
char *CheckEntities(char *txt);
void WriteChar(char ch);
void WriteAttr(char *name, char *val);


char *NewName(char *cp, int len = 0);
char *NewName(int len);
bool CheckList(char *list, char *name);

#endif

// end of udx.h

