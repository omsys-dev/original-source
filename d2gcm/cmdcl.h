/////////////////////////////////////////////////////////////////////////////
// Name:        cmdcl.h
// Purpose:     
// Author:      Jeremy H Griffith
// Modified by: 
// Created:     06/05/2012 14:13:49
// RCS-ID:      
// Copyright:   Copyright (c) 2012 Omni Systems
// Licence:     
/////////////////////////////////////////////////////////////////////////////

#ifndef _CMDCL_H_
#define _CMDCL_H_


class CMgr;
class CIni;
class CSection;
class CSetting;
class SIni;
class SSection;
class SSetting;
class SEnum;
enum pntyp;
enum sectstat;
enum actyp;
enum optyp;
enum inityp;
enum sectyp;
//enum valtyp;


// variables and functions used in cmdcl.cpp

class CMgr
{
 public:
	static char *CMexePath;
	static char *CMResPath;
	static char *CMProgName;
	static char *CMVersion;
	static char *AltHelpCommand;

	static DCirfile *CMIni;
	static char *CMIniName;
	static long PosX;
	static long PosY;
	static long SizeX;
	static long SizeY;
	static long ChildPosX;
	static long ChildPosY;
	static pntyp CurrPanel;
	static bool AllowSysIniSelect;
	static bool IncludeAllSections;
	static bool IncludeAllSettings;
	static long cmVersion;
	static bool CMIniChanged;
	static void SetCMIni(void);
	static void UpdateCMIni(void);

	static DCirfile *SysIni;
	static char *SysIniName;
	static void SetSysIni(void);

	static bool UseLog;
	static char *LogFileName;
	static DClog *Log;
	static unc LogLevels[logmax];
	static bool LogIniChains;
	static void SetUpLog(void);
	static void LogEvent(logtype lt, unc sev, const char *desc,
	              const char *d2 = NULL, const char *d3 = NULL,
	              const char *d4 = NULL, const char *d5 = NULL);

	static DCnlist SectReplaceNames;
	static DCilist SectReplaceIdx;
	static long SectReplaceNameCount;
	static DCnlist SettingValueNames;
	static DCnlist SetValueHelpText;
	static long SetValueCount;
	static DCnlist MacroNames;
	static DCnlist MacroHelp;
	static long MacroCount;
	//static DCnlist SectionAlias;
	static bool CheckAlias;

	static DCilist IniTypes;
	static ipair IniTypesInit[];
	static DCvlist AvailIniFiles;
	static SIni *AvailIni;
	static SSection *AvailSection;
	static SSetting *AvailSetting;
	static DCnlist AvailSectionNames;
	static DCvlist AvailSections;
	static long AvailSectionNameCount;

	static DCvlist AvailSuffixIniFiles;
	static SIni *SuffixIni;
	static DCnlist AvailVNSuffixes;
	static DCvlist AvailVNSuffixSections;
	static DCvlist AvailVNSuffixSets;
	static DCnlist AvailVNSuffixHelp;
	static long AvailVNSuffixCount;

	static DCvlist VKEnums2Proc;
	static DCnlist VKEnumNames;
	static long VKEnums2ProcCount;
	static DCvlist FKEnums2Proc;
	static DCnlist FKEnumNames;
	static long FKEnums2ProcCount;
	static DCnlist EnumHelp;
	static DCnlist AvailSEnums;

	static DCnlist SectCats;
	static DCnlist SuffCats;
	static DCnlist ValueCats;
	static DCnlist FilePicker;
	static DCnlist FilePickerOptions;
	static DCnlist FilePickerFilter;
	static long FilePickerCount;

	static void SetUpHelp(void);
	static void GetVKSettings(SSection *sect, char *vkset);
	static void ProcVKSpecialSetting(SSection *sect, char *sname);
	static void ProcVKSpecialValue(SSection *sect, char *val);
	static void ProcVKEnum(SSection *sect, char *sectenum = NULL);

	static SEnum *GetEnumSet(char *name, char **htp = NULL,
		 bool plus = false, SEnum *se = NULL);

	static void GetFKSettings(SSection *sect,
		                     char *setname, bool suf = false);
	static void ProcessFKSet(SSection *sect, char *setname,
			                   char *sectname, char *newhelp = NULL,
			                   bool minus = false, bool setalias = false);
	static void ProcFKSpecialValue(SSetting *set, char *val);
	static bool CheckFKSuffix(SSection *sect,
		                     char *setname, bool help = false);
	static void ProcFKEnum(SSetting *set, char *setenum = NULL);

;
  static bool FilePanelEnabled;
	static DCirfile *ProjIni;
	static char *ProjIniName;

	static DCnlist ProjFormatNameList;
	static DCnlist ProjIniNameList;
	static long ProjFormCount;

	static DCnlist FormatCodeKeys;
	static DCnlist FormatCodes;
	static long CodesCount;
	static char FormatCode;

	static DCnlist ProjNameList;
	static DCilist ProjNumList;
	static DCnlist ProjFormatList;
	static DCnlist ProjSrcList;
	static DCnlist ProjDirList;
	static long ProjCount;

#ifdef _M2GCM
	static DCnlist ProjFmBookList;
	static DCnlist ProjFmChapList;
	static long NextProjID;
#endif

	static void SetProjIni(void);

	static bool CmdLineStart;
	static bool CmdLineCat;
	static bool CmdLineAct;
	static char *SelectedAction;
	static DCnlist Actions;
	static npair ActInit[];
	static bool CmdLineSect;
	static bool CmdLineSet;
	static bool CmdLineIni;

#ifdef _M2GCM
	static void CMgr::VerifyProjectFileEntry(char *sfile);
#endif

	static DCnlist StartFormatList;
	static DCnlist StartNameList;
	static long StartCount;
  static bool IncludeStarts;
  static bool NeedLocalStart;
  static char *LocalStartDir;
  static char *SystemStartDir;


  static bool CatPanelEnabled;
	static DCnlist CatNameList;
	static DCilist CatLevelList;
	static DCvlist CatIDList;
	static DCvlist CatParentList;
	static wxTreeItemId CatParents[16];
	static unc CategoryCode;
  static char *SelectedCategory;
  static char *TopCategory;
	static long CatCount;
	static long CatNum;
	static long SelCatLevel;
	static bool SelAllTopCat;
	static inityp SelIniTyp;
	static DCilist CatParentNums;
	static DCilist CatParentLevels;
	static char *ConfigsCat;
	static char *ParaCharFormatsCat;
	static char *TableFormatsCat;
	static char *SubformatsCat;
	static char *PageFormatsCat;
	static char *LanguagesCat;
	static char *MacrosCat;
	static char *ContentModelsCat;


  static char *SelectedSection;
  static char *SelectedSetting;


  static bool ActPanelEnabled;
	static actyp CurrAct;

	static DCnlist SecsToCheck;
	static DCvlist SectionsToCheck;
	static DCilist SecNumsToCheck;
	static DCnlist SecsToShow;
	static DCvlist SectionsToShow;
	static DCnlist SectionsShownData;
	static DCvlist SectionsShownSection;
	static long SectionsShownDataCount;
	static long SectionsShownDataCurr;
	static void SelectSectionsToShow(DCnlist &sectlist,
		                      long sectcount, bool avail);
	static bool VNSectsOnly;
	static bool FNSectsOnly;
	static bool VKSectsOnly;
	static bool FKSectsOnly;
	static bool DelSectsOnly;
	static bool DupSectsOnly;
	static bool DupSectsFound;
	static bool DelSetsOnly;
	static bool DupSetsOnly;
	static bool DupSetsFound;
	static bool DupWarning;
	static bool NoMacroSects;

	static char *FixedMacroSectionsCat;
	static char *MacroNamesCat;
	static char *MacrosSect;
	static char *MacroVariablesSect;

	static bool CheckSection(void *sect, bool avail);


  static bool SectPanelEnabled;
	static wxString VNSectNewName;
	static wxString VNSectRename;
	static bool CurrSectExists;
	static bool CurrSectVN;
	static bool CurrSectFN;
	static bool CurrSectVK;
	static bool CurrSectFK;
	static bool CurrSectDel;
	static bool CurrSectDup;
	static bool CurrSectMacro;
	static bool AutoAlias;
	static wxString AutoAliasSet;
	static wxString AutoAliasVal;
	static sectstat SectionExists(const char *sname);


  static bool SetPanelEnabled;
	static DCvlist SettingObjsShown;
  static wxString VKSetNewName;
  static wxString VKSetRename;
	static sectstat SettingExists(const char *sname);


  static bool ValPanelEnabled;
	static DCnlist ScopeNames;
	static DCnlist ScopeVals;
	static long ScopeCount;

  static CIni *SelectedIni;
  static CSection *SelectedSect;
  static CSetting *SelectedSet;
  static long SelIniPos;
  static bool SelectingSrcIni;
  static bool SectInIni;


  static bool VselPanelEnabled;
  static long SelValPos;


  static char MacroBuffer[4096];


	static bool AddIniHistory;
  static const char *UserName;

  static char *SectColorText;
  static char *SetColorText;
  static char *VarColorText;
  static char *DefColorText;
  static char *SysColorText;
  static char *InvalidColorText;

	static char *HelpCommand;
	static char *Browser;
	static char *Firefox;
	static char *IExplore;
	static char *Chrome;
	static char *Opera;
	static char *DefaultAlias;

	static bool UseHTMLHelp;
	static DCnlist HelpAlias;
	static char *CHMPath;
	static char *CHMPathSet;


  static char *StartFileName;
  static char *MacroFileName;
  static char *LangFileName;
  static char *FmtFileName;
  static char *PagesFileName;
  static char *TablesFileName;
  static char *SubFormatsName;
  
  static bool UseConfigs;
  static bool UseFormats;
  static bool UseLangs;
  static bool UseMacros;
  static bool UseModels;
	static uns Usage;
  
  static DCirfile *IniFile;
  static DCirfile *MacroFile;
  static DCirfile *LangFile;
  static DCfrfile *FmtFile;
  static DCfrfile *PagesFile;
  static DCfrfile *TablesFile;
  static DCfrfile *SubFormatsFile;

  static DCvlist CurrIniFiles;
  static long CurrIniCount;
  static DCvlist CurrSections;
  static long CurrSectionCount;
  static DCnlist CurrSectionNames;
  static DCvlist CurrSectionNameRefs;
  static long CurrSectionNameCount;
  static DCvlist CurrSettings;
  static long CurrSettingCount;
  static DCnlist CurrSettingNames;
  static DCvlist CurrSettingNameRefs;
  static long CurrSettingNameCount;

  static void GetIniFiles(void);

  static bool GetAllInis;
  static long ConfigLevel;
  static long MacroLevel;
  static long LangLevel;
  static DCnlist LangNameList;
  static DCnlist LangChainList;
  static DCvlist LangFileList;
  static long FormatLevel;
  static DCnlist FormatNameList;
  static DCnlist FormatChainList;
  static DCvlist FormatFileList;
  static long PagesLevel;
  static DCnlist PagesNameList;
  static DCnlist PagesChainList;
  static DCvlist PagesFileList;
  static long TablesLevel;
  static DCnlist TablesNameList;
  static DCnlist TablesChainList;
  static DCvlist TablesFileList;
  static long SubFormatsLevel;
  static DCnlist SubFormatsNameList;
  static DCnlist SubFormatsChainList;
  static DCvlist SubFormatsFileList;
  static void GetIniFileChains(void);
  static void SetPagesFiles(void);
  static void SetTablesFiles(void);
  static void SetSubFormatsFiles(void);

  static void ClearDupIniRefs(DCvlist *flist, long start);
  static void ClearDupFmtIniRefs(DCvlist *flist, long start);

  static void SetCInisForChain(DCvlist *chain, inityp it);
  static void GetIniSectionInfo(void *vp, long i);
  static bool CheckCfgMacro(const char* name);

	static char *ResultFileName;
	static char *ChildResult
;	static bool ChildCM;

#ifdef WIN32
;	static char OFNFilterBuffer[256];
;	static char OFNPathBuffer[1024];
;	static char OFNFileBuffer[256];
	static LPOPENFILENAME SetOpenInfo(char *dir,
		 char *filter, char *title, bool exist);
#endif

  static bool FileExists(char *fname);
};



enum pntyp {
 pnfile = 0, pncat, pnact, pnsec, pnset, pnini, pnval, pnopt
};

enum sectstat {
 sectnone = 0, sectcurr, sectavail
};

enum actyp {
 actnone = 0, actadd, actaddv,
 actadds, actaddsv, actaddsb,
 actedit, actedits,
 actdel, actdels,
 actrest, actrests,
 actrenv,  actrensv,
 actmov, actmovs,
 actmovf, actmovsf,
 actcpy, actcpys,
 actdup, actdups,
 actfix, actfixs,
 actcount
};

enum optyp {
 opnone = 0, opchange, opadd, opren, oprenset,
 opdel, opdelset, opdelall, operr
};

enum inityp {
 ininone = 0, inicfg, inilang, inimac,
 inifmt, initbl, inisub, inipage,
 inimod, inimix, inisuf, inilast
};

enum sectyp {
 stnone = 0, stfix, stvar, stboth
};

//enum valtyp {
// valnone = 0, valyn, valuc, valstr, valpath, valenum
//};


class CIni
{
 public:
	char *name;
	DCirfile *ini;
	DCfrfile *fini;
	inityp ityp;
	SIni *si;
	bool sys;
	char *path;
	char *scope;
	CIni *previni;
	CIni *nextini;
	bool hassectdup;
	bool hasinvalidsect;
	DCnlist sectnames;
	DCvlist sections;
	long sectcount;
	long lastsect;

	CIni(DCirfile *ifl, inityp it);
	CIni(DCfrfile *ifl, inityp it);
	~CIni();
	void GetSectionInfo(long i);
	void GetFSectionInfo(long i);
};


class CSection
{
 public:
	char *name;
	CIni *ifile;
	bool isinvalid;
	bool ismacro;
	bool hassectdup;
	bool hassetdup;
	bool hasinvalidset;
	SSection *ssect;
	char *htext;
	//CSectprops *cs;
	DCnlist setnames;
	DCvlist settings;
	long setcount;
	long lastset;
	char **cats;

	CSection(const char *nm, CIni *cini);
	CSection() { }
	~CSection();
};

class CSetting
{
 public:
	char *name;
	CSection *sect;
	bool hassetdup;
	bool isinvalid;
	SSetting *sset;
	//CSetprops *set;
	char *cval;
	sectyp settyp;

	CSetting(char *nm, char *val, CSection *s);
	CSetting() { }
	~CSetting();
};


// classes for section/setting definitions

class SIni
{
 public:
	inityp ityp;
	DCnlist sectnames;
	DCvlist sections;
	long sectcount;

	SIni(inityp it);
	~SIni();
};

class SSection
{
 public:
	char *name;
	SIni *ifile;
	char *htext;
	char *alias;
	sectyp nametyp;
	sectyp settyp;
	bool show;

	char *vktypes;
	char *vkshtext;

	char *vkvals;
	char *vkvhtext;
	bool plus;

	SEnum *vkenum;
	DClist vkelist;
	long vkecount;

	//DCnlist vkenums;
	//DCnlist vkenumhtext;
	//long vkenumcount;

	char *fkset;
	DCnlist fksetnames;
	DCvlist fksettings;
	long fksetcount;
	char **cats;
	char *formats;

	SSection(char *nm, SIni *cini);
	~SSection();
};

class SSetting
{
 public:
	char *name;
	SSection *sect;
	char *htext;
	char *alias;
	bool show;

	char *fkvals;
	//valtyp val;
	char *vhtext;
	bool plus;
	char *dval;
	char *formats;

	SEnum *fkenum;
	DClist fkelist;
	long fkecount;

	//DCnlist vals;
	//DCnlist valhtext;
	//long valcount;

	SSetting(char *nm, SSection *s);
	~SSetting();
};

class SEnum
{
 public:
	char *name;
	char *vhtext;

	// from SSection:
	//DCnlist vkenums;
	//DCnlist vkenumhtext;
	//long vkenumcount;

	// from SSetting:
	DCnlist vals;
	DCnlist valhtext;
	long valcount;

	SEnum(char *nm);
	~SEnum();
};


// globals for system commands

#define MAXPATH 1024

extern char *ComSpec;
extern unl ExitType;
extern unl LocalExitCode;
extern char PathBuf[MAXPATH];
extern bool StopCommand;
extern HANDLE CurrProc;
extern LPSTARTUPINFO startInfo;
extern PROCESS_INFORMATION *procInfo;


void RunCommand(const char *command, const char *dir = NULL,
	 bool hide = true, bool synch = true);

void WaitForCurrProcess(void);

void RunDOSCommand(const char *cmd, const char *dir = NULL,
	 bool hide = true, bool keep = false, bool synch = false);

char *ConvertEnvironment(char *str);

#endif

