 
// dcnode.h is the header for dcl node classes, JHG, 2/28/93
// Copyright (c) 1993 by Omni Systems, Inc.  All rights reserved.

#ifndef DCNODE_H
#define DCNODE_H


// dcl node handling

struct ndata {
  uns v;
  char *c;
  short child_id;
  short sib_id;
};

struct nstack {
  DCnode *node;
  short sibling;
};

typedef bool (*NDFunc)(ndata *);

extern ndata InitNodeList[];

struct attrs {
	uns maj;
	uns min;
	uns dat;
	uns siz;
	uns dattr;
	uns sattr;
};

typedef void (*NAFunc)(attrs *);

extern attrs InitAttrList[];

class DCnode {
  uns value;
  char *name;
  DCnode *child;
  DCnode *sib;

 public:
  DCnode() { name = NULL; }
  DCnode(uns v, short len, char *c, DCnode *ch = NULL, DCnode *si = NULL)
    { value = v; strncpy(name = new char[len], c, len); child = ch; sib = si; }
  ~DCnode() { if (name) delete [] name; }

  // access
  uns val() { return value; }
  char *nam() { return name; }
  void set(uns v, short len, char *c, DCnode *ch = NULL, DCnode *si = NULL)
    { value = v; strncpy(name = new char[len], c, len); child = ch; sib = si; }
  void renam(short len, char *c)
    { delete [] name; strncpy(name = new char[len], c, len); }
  DCnode *setChild(DCnode *n) { child = n; return n; }
  DCnode *setSib(DCnode *n) { sib = n; return n; }

  static DCnode *TopNode;
  static char *NodeFile;
  enum NodeConstants
    { FieldMax = 16, ScopeMax = 16, StackMax = 20, TokenMax = 80,
      DListMax = 2000, AListMax = 2000 };
  static unc Fields[FieldMax];
  static unc FNest;
  static unc CurrField;
  static uns Scopes[ScopeMax];
  static unc SNest;
  static uns CurrScope;
  static uns MajorScope;
  static uns MinorScope;
  static uns DataScope;
  static uns SizeScope;
  static uns DataAttr;
  static uns SizeAttr;

  static void ListInit(DCnode *Top = NULL);
  static DCctl& ProcNodeCtl(DCctl& c);
  static void Walk(NDFunc UFunc = NULL, DCnode *Top = NULL);
  static bool WalkFunc(ndata *nd);

  static bool SetMajor(uns v, short len, char *c);
  static bool SetMinor(uns v, short len, char *c, uns mj);
  static bool SetData(uns v, short len, char *c, uns mn, uns mj);
  static bool SetSize(uns v, short len, char *c, uns d, uns mn, uns mj);
  static bool SetNodeName(uns v, short len, char *c, DCnode *n);

	static attrs **AttrList;
	static uns AttrCount;

  static void SetAttr(uns da, uns sa, short len, char *c, uns s, uns d, uns mn, uns mj);
	static void MakeAttrList(NAFunc UserAttrFunc = NULL);
	static attrs *GetAttrs(uns mj, uns mn, uns d, uns s);

  static DCnode *FindMajor(uns v);
  static DCnode *FindMajor(char *c);
  static DCnode *FindMinor(uns v, uns mj);
  static DCnode *FindMinor(char *c, uns mj);
  static DCnode *FindData(uns v, uns mn, uns mj);
  static DCnode *FindData(char *c, uns mn, uns mj);
  static DCnode *FindSize(uns v, uns d, uns mn, uns mj);
  static DCnode *FindSize(char *c, uns d, uns mn, uns mj);
  static DCnode *FindNode(uns v, DCnode *n);
  static DCnode *FindNode(char *c, DCnode *n);

  static void InitNodes(char *nfile = NULL, DCnode *Top = NULL);
};

#endif

