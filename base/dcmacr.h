
// dcmacr.h is the header for dcl macro classes, JHG, 3/6/05
// Copyright (c) 2005 by Omni Systems, Inc.  All rights reserved.

#ifndef DCMACR_H
#define DCMACR_H

#define MACRMOD


class DCmacr;

typedef bool (*pbWMC)(DCwfile *, unc);
typedef bool (*pbWCP)(DCwfile *, char *);
typedef char *(*pcWCP)(DCwfile *, char *);
typedef void (*pvWUL)(DCwfile *, unl);
typedef char *(*pcGV)(char *);
typedef long (*plPCL)(char *, long);


enum MacrConst {
  NumberStrMax = 17, MacroNestLevMax = 128
};

// dcl macro processor

class DCmacr {
 protected:
	static secinfo MacroIniSects[];
	static setinfo ISecMacros[];

	static short MacroNestLevel;
	static long RepCount;
	static long RepDCount;
	static long WhileCount;

	static DCnlist MacListNames;    // relates list name to listnum;
	static DCvlist MacListIndexes;  // relates ptr to list's indexes to listnum
	static DCvlist MacListValues;   // relates ptr to list's values to listnum
	static DCnlist MacListDefaults; // relates default list value to listnum
	static DCilist MacListCounts;   // specifies next num for each listnum
	static long MacListCount;

	static DCnlist MacroVarNames;
	static DCnlist MacroVarVals;
	static long MacroVarCount;

	static void InitMacroConfigTemplates(char *snm);

	static DCblock *GetMacroFile(char *fname);

	static long CheckCond(DCwfile *wf, char **ctxt);
	static char *EvalCond(DCwfile *wf, char **ctxt, bool sub = false);
	static char *StripChars(char *ltok, char *rtok);
	static char *ReplaceChars(char *ltok, char *rtok, char *ntok);
	static char *GetToken(DCwfile *wf, char **tptr, bool lit = false);
	static long GetLongTokVal(char *tok, int *base);
	static char *GetMacroVar(DCwfile *wf, char *vp, bool sys = false);
	static char *GetMacroLoopVar(char *vp);

	static char *GetMacListValue(DCwfile *wf, char *vp, bool sys = false);
	static char *GetMacListValue(DCwfile *wf, char *lname, char *lindx,
							                 bool ini = false);
	static char *GetMacListIndex(char *lname, char *lval);
	static void SetMacListValue(DCwfile *wf, char *lname, char *lindx, char *lval,
							                bool ini = false);
	static void GetMacListInfo(char *lname, DCnlist **pilist, DCnlist **pvlist,
														 long *plistnum = NULL);
	static char *GetMacListIxVal(DCwfile *wf, char *lindx);

	static char *SplitListName(char *fname, char **plindx, bool *pinitype,
		                         char *lix = NULL, char *elix = NULL, char *term = NULL);
	static char *CombineListName(char *ptname, char *lindx, bool initype = false);
	static char *GetPtrVal(DCwfile *wf, char *ptname);

	static void ProcMacroSection(DCwfile *wf, DCirfile *file, char *name);

	static void WriteMacroFormatted(DCwfile *wf, char **fmt, char *val);

	static bool ProcMacroSetting(char *sect, char *key, char *val);

	// stub functions for uninitialized pointers 
	static bool StubbWMC(DCwfile *df, unc uch);
	static bool StubbWCP(DCwfile *df, char *vp);
	static char *StubcWCP(DCwfile *df, char *vp);
	static char *StubcGV(char *vp);
	static long StublPCL(char *vp, long);

	// variables set by Init()
	static pbWMC WriteMacroChar;
	static pbWCP WritePredefinedMacro;
	static pbWCP WriteCodeStoreVar;
	static pcWCP GetCodeStoreVar;
	static pcGV GetTextStore;
	static pcWCP GetPredefinedVar;
	static pcGV GetFrameUserVar;
	static plPCL GetFrameUserArrVar;

	static bool ValidOnly;
	static bool NameUndefMacroVars;
	static bool NameUndefMacros;
	static bool FixMacroQuotes;
	static bool MacroVarNesting;
	static bool OmitMacroReturns;
	static bool ReplaceFrameVars;

	static DCwfile *DestFile;
	static DCirfile *IniFile;
	static char *MacroFile;

	static short MacroNestMax;
	static short RepeatMax;
	static short WhileMax;


 public:

	DCmacr() {}
  virtual ~DCmacr(void) {}

	static DCirfile *MacroIniFile;

  static void InitMacroModule(DCwfile *df = NULL,
		DCirfile *ifl = NULL, char *mfl = NULL,
		pbWMC fwmc = NULL, pbWCP fwpm = NULL,
		pbWCP fwcsv = NULL, pcWCP fgcsv = NULL, 
		pcGV fgts = NULL, pcWCP fgpv = NULL,
		pcGV fgfuv = NULL, plPCL fgfuav = NULL,
		bool vo = false, bool numv = false, bool num = false,
		bool fmq = false, bool mvn = true, bool omr = false,
		uns umnm = 0, uns urm = 0, uns uwm = 0);

	static void InitPluginMacroModule(DCirfile *ifl = NULL,
	 char *snm = NULL, char *mfl = NULL, pbWMC fwmc = NULL,
	 pbWCP fwpm = NULL, pcWCP fgpv = NULL,
	 pcGV fgfuv = NULL, plPCL fgfuav = NULL,
	 bool vo = false);

	static void WriteMacro(DCwfile *wf, char *mactxt);
	static void WriteMacroName(DCwfile *wf, char **pmacname,
														 bool point = false);
	static char *GetMacro(char *mactxt); // not for plugin
	static char *GetUserMacVar(char *vp, bool fmvar = true);
	static bool ReplacingFrameVars(void) { return ReplaceFrameVars; }
	static char *MacroParam;

	static pvWUL WriteMacroTextSingleChar;
};

#endif

