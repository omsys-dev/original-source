
//dcfili.h is the header for dcl ini file classes, JHG
// Copyright (c) 1993-2001 by Omni Systems, Inc.  All rights reserved.

#ifndef DCFILI_H
#define DCFILI_H


struct anyini {  // used for FindAny
	const char *nm[5];   // names used for check
	uns cnt;       // count of names used, 1 to 5
};

struct logmsg {
 logtype lt;
 unc sev;
 const char *desc;
 const char *d2;
 const char *d3;
 const char *d4;
 const char *d5;
};

typedef void (*logfunc)(logtype lt, unc sev, const char *desc,
 const char *d2, const char *d3, const char *d4, const char *d5);


// class for ini file reading

class DCirfile : public DCrfile {
 protected:
	bool CurrFile;
  char *Buf;
  long BufMax;

	char **SectPos;
	long SectIdx;
	long SectLim;

	char **LinePos;
	long LineIdx;
	long LineLim;

  const char *CurrSect;
  char *CurrSectName;
	uns CurrPos;
  const char *CurrLeft, *CurrRight;
  short LeftLen, RightLen;

	char *SecName;
	DCirfile *SecFile;
	bool SecSect, SecNext, SecFound, SecList;

	const char *SubType;
	const char *CurrSubSect;
	bool SubSect, SubNext, SubFound;

	const char *ListHead;
	char *ListEnd;

	static bool MatchNoCase;
	static bool MatchNoSpace;
	static bool MatchWild;

	void SetSectPos(void);
	char *GetSectPos(const char *cpt);
	char *GetLinePos(const char *cpt);

  char *SecContStr(const char *sect);
  bool NextItemStr(const char *sect);
  bool FindPosNum(const char *sect, uns num);
  bool FindNum(const char *sect, uns num);
  bool Find(const char **names, short count = 0, bool ep = false);
  bool FindStr(const char *sect, const char **names, short count);
	bool match(const char *cl, const char *name);

  //void skipLine(void);
  void setRight(void);

 public:
  DCirfile(const char *fname);
  DCirfile(const char *fname, DCfile *bfp);
  DCirfile(const char *fname, const char *sn);
  virtual ~DCirfile() { } // delete [] Buf; }

	static DCnlist IniChainList;
	static char *LastConfigSetting;
	static bool LogIniChain;
	static void WriteIniChain(const char *setting);
	static void WriteIniChainItem(char *path, long i);

	static void LogEvent(logtype lt, unc sev, const char *desc,
	              const char *d2 = NULL, const char *d3 = NULL,
	              const char *d4 = NULL, const char *d5 = NULL);
	static logfunc LogEventFunc;
	static DCvlist StoredLogMsgs;
	static long StoredLogMsgCount;
	static bool SendingStoredMsgs;
	static void StoreLogMsg(logtype lt, unc sev, const char *desc,
	              const char *d2 = NULL, const char *d3 = NULL,
	              const char *d4 = NULL, const char *d5 = NULL);
	static void SendStoredMsgs(void);
	static void SendStoredMsg(void *vp, long i);

  virtual dc_ferr FErr();
  virtual void CloseFile();

	static char *GetIniName(char *start, char *fmt, char *flag,
	                        char *endflag = NULL, char **oname = NULL);

	DCirfile *SetConfigTemplate(const char *sect, const char *setting,
		                          bool link = true);
	DCvlist IniFileList;
	DCnlist IniNameList;
	long IniLevel;
	char *Scope;
	bool ChainEnded;

	void SetSecFile(DCirfile *fp)
		{ SecFile = fp; }
	DCirfile *GetSecFile(void)
		{ return SecFile; }
	void CloseIniChain(void);

	void SetType(const char *id);

  bool Section(const char *name);
  char *SecCont(bool secfirst = false);
  DCnlist *SectNames(void);
  bool NextItem(void);
	long GetItemCount(void);
  bool Find(uns num);
  bool FindPos(uns num);
	uns GetPos(void);
  bool FindAny(const char **names, short count = 0);
  bool FindAny(const anyini *any);

	bool InElemPath;
	static DCnlist ElemSetNames;
	static DCvlist ElemSets;
	static long ElemSetNum;
	bool FindElemPath(const char *name);
	void GetElemSets(void);

  bool Find(const char *name);
	static void SetMatch(unc matchrule, bool bval);

  char *StrLeft(void);
  char *StrRight(void);
  char *PathRight(void);
	char CharRight(void);
	char UCRight(void);
  long IntLeft(void);
  long IntRight(void);
  long HexRight(void);
	bool BoolRight(void);

	bool ListRight(void);
	char *ListItem(void);
	char **StrArrRight(void);
	long *IntArrRight(void);
};


#endif

