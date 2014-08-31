//dcauto.h is the header for output file automation, JHG
// Copyright (c) 2009 by Omni Systems, Inc.  All rights reserved.

#ifndef DCAUTO_H
#define DCAUTO_H


typedef void (*logfunc)(logtype lt, unc sev, const char *desc,
 const char *d2, const char *d3, const char *d4, const char *d5);


enum outtyp {
 otnone = 0, otrtf, otwh,
 othtm, otxhtm,
 otxml, otdita, otdbk, otudoc,
 oteh, othh, otjh, otomh, otohj,
 otdcl, otmif, otpdf, otprn
};


class DCauto {
 protected:
	static outtyp OutType;

	static char *CurrDir;
	static int CurrDrive;
  static bool DeleteExistingDCL;
	static char *GraphPath;
	static char *GraphSubdir;
	static char *HTMLSubdir;
	static char *OHVName;
	static char *CopyGraphicsFrom;
	static char *CopyCSSFrom;
	static char *CopyBeforeFrom;
	static char *CopyAfterFrom;
	static bool KeepCompileWindow;
	static int ExitCode;
	static int ExitType;
	static char *ComSpec;
 	static char *ProjectName;
 	static char *PrjPath;
  static char *JarCommand;
  static char *ArchiveCommand;
  static char *ArchiveStartParams;
  static char *ArchiveEndParams;
  static char *ArchiveFrom;
  static char *ArchiveName;
  static char *ArchiveVer;
  static char *ArchiveExt;
  static char *WrapPath;
  static char *ShipPath;
  static char *CSSPath;
  static char *WrapCopyFiles;
  static char *GraphCopyFiles;
  static char *CSSCopyFiles;
  static char *CopyBeforeFiles;
  static char *CopyAfterFiles;
  static char *JavaRootFiles;
  static char *ZipCommand;
  static char *ZipParams;
  static char *EmptyOutputFiles;
  static char *SystemStartCommand;
  static char *SystemWrapCommand;
  static char *SystemEndCommand;
  static char *SystemCommandWindow;
  static char *BatFileName;
	static char *AskForUserVars;
  static char *OHVFiles;
  static bool OHProjFileXhtml;
  static bool WrapAndShip;
  static bool InWrapShip;
  static int EmptyOutputDir;
  static char *EmptyOutputDirList[];
  static bool EmptyGraphPath;
  static bool EmptyJavaHTMLSubdir;
  static bool EmptyJavaGraphSubdir;
  static bool LogAuto;
  static bool MakeFts;
  static bool MakeJar;
  static bool MakeArchive;
  static bool MoveArchive;
  static bool EclipseUseManifest;
  static bool FilterConditions;

	static char *BaseFileName(char *fname);
	static char *PathName(const char *fname);
	static char *AddFileExt(char *filename, char *newext);
	static char *NewFileExt(char *filename, char *newext);
	static void DeleteDirContents(char *cd);
	static void DeleteOneFile(char *fname);
	static char *MakeFileName(char *pathname, char *filename);

	static char *AddSlash(char *newpath);
	static void RemoveSlash(char *path);
	static char *Unquote(char *newpath);
	static void MakeDir(char *cd);
	static void RemoveDir(char *cd);

	static bool WritePredefinedMacro(DCwfile *df, char *name);
  static char *GetPredefinedVar(DCwfile *df, char *vp);

	static void RunCommand(char *command, char *cd, bool synch,
	                       bool hide, bool sized);

  static char *MakeFileList(char *filenames);
  static char *MakeDirPath(char *base, char *path, bool empty = false);
  static void CopyCompFiles(char *filelist, char *dest);
  static void CopyCompFilesSrc(char *filelist, char *dest, char *src);
  static void DelCompFiles(char *filelist);
  static void ProcCompFileList(char *filelist, char *dest, char *src, bool del);

  static void DoEmptyOutputProc(void);

 public:
  DCauto() { }
  virtual ~DCauto() { }

	static void LogEvent(logtype lt, unc sev, const char *desc,
	              const char *d2 = NULL, const char *d3 = NULL,
	              const char *d4 = NULL, const char *d5 = NULL);
	static logfunc LogEventFunc;
	static char *LogFileName;
	static char *AutoLogFile;
	static char *AutoLogFileTmp;
	static DClog *AutoLog;
	static long AutoLogNum;
	static bool LogAutoStarted;
	static void StartLogAuto(void);
	static void LogAutoEvent(char *txt);
	static void EndLogAuto(void);

  static bool SetUpAutomation(outtyp outtype, DCirfile *ini,
	               char *csspath = NULL, char *graphpath = NULL, bool book = false);
	static void RunDOSCommand(char *cmd, bool hide = true, bool keep = false,
		                        bool synch = true);

	static bool XMLSource;
  static bool OnlyAuto;
	static bool UseExistingDCL;
  static bool EmptyWrapPath;
	static bool MakeHLP;
	static char *Compiler;
	static char *FTSCommand;
	static unc IndexNavType;
	static char *MacroFile;
	static DCirfile *IniFile;
  static bool CopyOriginalGraphics;
	static DCnlist *OriginalGraphics;
	static DCnlist *OriginalFixedGraphics;
	static long OriginalGraphicsCount;

  static void DeleteDCL(void);
  static void SetSystemCommands(DCirfile *ini, bool book);
	static void ExecSystemCommand(const char *command, const char *name);
  static void WrapShipPreProc(bool isBook);

  static bool EndingAuto;
  static void WrapShipProc(char *bookname);
  static void CopyOriginalGraphicsProc(DCnlist *gpaths, DCnlist *gfixpaths, 
		                                   long gcount, char *gdir = NULL);
  static void CopyOriginalGraphicsEndAuto(char *gdir);
  static char *SetUpGraphDir(char *gdir);
  static void CopyOneOrigGraphic(char *cpt, char *fpt, char *gdir);
};



#endif
