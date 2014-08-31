// dcini.h is for table-driven ini-file processing, JHG, 3/20/05
// Copyright (c) 2005 by Omni Systems, Inc.  All rights reserved.

#ifndef DCINI_H
#define DCINI_H


class dcini;

typedef void (*psfunc)(char *);  // char ptr is not used yet, remove?

enum styp {    // type of var to be set
 yesno = 1, chr, cap, str, path, sarr, mac, cmp,
 snum, usnum, lnum, larr, ulnum, ucnum, hexnum,
 ilist, val, bval, cval, ushval, bit, nbit,
 lbit, nlbit, spec = 0
};

enum sectyp {  // type of key used to index section
 set = 1, file, ch, fnt, flow, fsize, colr,
 pstyle, cstyle, pcstyle, macr, macvar,
 tstyle, tabl, graphic, xrstyle, mark, reffr,
 tag, loc, suf, jhimg, toclev, proj, wind,
 unknown = 0
};

struct setinfo {
	char *sname;  // key name of setting
	styp stype;   // type of var to set
	void *svar;   // ptr to var to set
	psfunc sfunc; // function to call after setting
	unl sval;     // numeric value to set for property
	char *cpval;  // text value to set for property
	char *ovsec;  // section containing overrides for this
	char **match; // array of strings to match against
};

struct secinfo {
	char *secname;   // key name of section
	sectyp sectype;  // kind of values used to find setting keys
	char *secgroup;  // name of group to which section belongs
	setinfo *setarr; // array of settings in section
	bool all;        // true if used when running ProcAllSects
	styp stype;      // type of var to set if no array
	void *svar;      // ptr to var to set if no array
	psfunc sfunc;    // function to call after setting if no array
};

struct spprop {  // first three items of pprop
  char *stag;    // format tag name used for key lookup
  uns stype;     // 1 for para formats, 2 for char, 3 for table
	char *cstag;   // name of current char format (from its stag)
};

struct tbprop {  // first four items of tbl
	char *tblfname;  // format name
	char *tblid;     // FDK id name
	char *tbgroup;   // group name
	uns tblncnt;     // count of names used for ini (2 or 3)
};

struct setting { // used when parsing Config markers
	const char *sect;
	const char *key;
	const char *val;
};

class DCini {

 protected:

	static DCirfile *IniFile;  // ptr to ini set by using module
	static secinfo *IniSects;  // list of sections set by using module

	static spprop *Curr;
	static tbprop *Tbl;
	static anyini *TblIni;
	static anyini *Gr;
	static anyini *XrefIni;
	static char **Xrnmp;
	static char **Fmnmp;

  // processing of ini settings
	static bool ProcOneIniSect(secinfo *isec);
	static bool ProcOneIniVarSect(secinfo *isec);
	static bool ProcSet(setinfo *is);

  // processing of marker settings
	static bool ProcOneMarkSect(secinfo *isec, char *setname, char *setval);
	static bool ProcOneMarkVarSect(secinfo *isec, char *setval);
	static bool ProcSetVal(secinfo *isec, setinfo *is, char *setval);
	static bool ProcVal(styp typ, void *var, char *val);
 	static bool MakeBool(char *setval);
	//static bool ProcStyleVal(setinfo *is);  // same as ProcStyleSet

	// getting values from program for macros
	static char *GetOneSectStr(secinfo *isec, char *setname);
	static char *GetSetVal(setinfo *is);
	static char *GetVarVal(secinfo *isec, char *setname);
	static char *GetVal(styp typ, void *var);
	static char *GetStyleVal(setinfo *is);

	// utils common to ini and marker
	static char *ListHead;
	static char *ListPtr;

	static bool ProcStyleSet(setinfo *is);
	static bool ProcUnknownSet(char *setname, char *setval = NULL);

 public:

	static secinfo *CurrSect;  // section being (or last) processed
	static bool IsIni; // true if processing ini, false if marker
	static bool Persistent; // true if marker setting is persistent

	DCini() {}
  virtual ~DCini(void) {}

  // init called once, may be called again if need be
	static void InitIniModule(DCirfile *ifl, secinfo *isects = NULL,
		spprop *curr = NULL, tbprop *tbl = NULL,
		anyini *gr = NULL, anyini *tbini = NULL, anyini *xrini = NULL,
		char **xrnmp = NULL, char **fmnmp = NULL);

  // proc ini sections
	static void ProcAllSects(void);	// proc sections flagged for "All"
	static bool ProcIniSect(char *sectname);  // proc individual setting
	static bool ProcIniGroup(char *groupname); // proc named group
	static bool ProcIniSectGroup(char *sectname, char *groupname); // unused
	static bool ProcIniSet(char *setname, char *sectname = NULL); // unused

  // proc marker setting
	static bool ProcMarkSect(char *sectname, char *setname, char *setval);
	static void ProcConfigPara(char *txt, long pref);
	static void ProcParaConfig(long pref);
	static void SetParaConfig(void *vp);
 	static setting *ParseDirective(char *txt, bool para = false);

	// get val from program
	static char *GetStrVal(char *sectname, char *setname = NULL);

	// utils used when processing markers and macros
	static bool SetSect(char *sectname, bool ini = true);
	static bool CheckCurrFormName(char *key);

	static void StartList(char *lptr);
	static char *GetListItem();
	static char **GetStrArr(char *sptr);
	static long *GetIntArr(char *sptr);

	// pending setting processing
	static DCvlist SetList;
	static DCvlist ParaSetList;
	static DClist CurrPendList;
	static DClist TblSetList;
	static DClist GraphSetList;
	static DClist XrefSetList;
	static char *PendingGraphGroup;
	static char *PendingTblGroup;

	static bool SetDirective(char *txt, long cpid);
	static bool ProcDirective(setting *s, long cpid, bool para = false);

  static void SetCurrID(long i);
  static void GetCurrSettings(long i);
  static void SetXrefID(long i);
  static void GetXrefSettings(long i);
  static void SetTblID(long i);
  static void GetTblSettings(long i);
  static void SetGraphID(long i);
  static void GetGraphSettings(long i);
	static short MatchKeys(anyini *ks, char *key);

  static void GetPropSettings(long i);

};

#endif

// end of dcini.h
